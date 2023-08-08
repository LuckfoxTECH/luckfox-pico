/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/version.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/platform_device.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/pm_runtime.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include <linux/printk.h>
#else
#include <linux/kernel.h>
#endif

#include <hwif/hwif.h>
#include "sdio_def.h"
#include <ssv_chip_id.h>
#include "hwif/hal/hwif_hal.h"

#include "sdio.h"
#define LOW_SPEED_SDIO_CLOCK		(25000000)
#define HIGH_SPEED_SDIO_CLOCK		(50000000)

#define MAX_RX_FRAME_SIZE 0x900
#define MAX_REG_RETRY_CNT	(3)

#define SSV_VENDOR_ID	0x3030
#define SSV_CABRIO_DEVID	0x3030

#define CHECK_IO_RET(GLUE, RET) \
    do { \
        if (RET) { \
            if ((++((GLUE)->err_count)) > MAX_ERR_COUNT) \
                printk(KERN_ERR "MAX SDIO Error\n"); \
        } else \
            (GLUE)->err_count = 0; \
    } while (0)
    
#define MAX_ERR_COUNT		(10)

struct ssv6xxx_sdio_glue
{
    struct device                *dev;
    struct platform_device       *core;
    struct ssv6xxx_platform_data *p_wlan_data;
    struct ssv6xxx_platform_data  tmp_data;
#ifdef CONFIG_MMC_DISALLOW_STACK
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u8 rreg_data[4];
    PLATFORM_DMA_ALIGNED u8 wreg_data[8];
    PLATFORM_DMA_ALIGNED u32 brreg_data[MAX_BURST_READ_REG_AMOUNT];
    PLATFORM_DMA_ALIGNED u8 bwreg_data[MAX_BURST_WRITE_REG_AMOUNT][8];
    PLATFORM_DMA_ALIGNED u32 aggr_readsz;
    PLATFORM_DMA_ALIGNED u8 dmaData[SDIO_DMA_BUFFER_LEN];
#else
    u8 rreg_data[4];
    u8 wreg_data[8];
    u32 brreg_data[MAX_BURST_READ_REG_AMOUNT];
    u8 bwreg_data[MAX_BURST_WRITE_REG_AMOUNT][8];
    u32 aggr_readsz;
#endif
#else
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u8 dmaData[SDIO_DMA_BUFFER_LEN];
#endif
#endif
    /* for ssv SDIO */
    unsigned int                dataIOPort;
    unsigned int                regIOPort;

    irq_handler_t               irq_handler;
    bool                        dev_ready;
    unsigned int                err_count;
	struct workqueue_struct *wq;
	struct ssv6xxx_sdio_work_struct rx_work;
	//struct tasklet_struct rx_tasklet;
	u32 *rx_pkt;
	u32 *rx_isr_cnt;
    u32 recv_cnt;
	void *rx_cb_args;
    int (*rx_cb)(struct sk_buff *rx_skb, void *args);
    int (*is_rx_q_full)(void *);
    struct ssv_hwif_hal_ops hwif_hal_ops;
};

static void ssv6xxx_high_sdio_clk(struct sdio_func *func);
static void ssv6xxx_low_sdio_clk(struct sdio_func *func);
static void ssv6xxx_do_sdio_reset_reinit(struct ssv6xxx_platform_data *pwlan_data, 
        struct sdio_func *func, struct ssv6xxx_sdio_glue *glue);
static void ssv6xxx_sdio_direct_int_mux_mode(struct ssv6xxx_sdio_glue *glue, bool enable);

#if 1
static bool _is_glue_invalid(struct ssv6xxx_sdio_glue *glue);
#define IS_GLUE_INVALID(glue)  _is_glue_invalid(glue)

#else
#define IS_GLUE_INVALID(glue)  \
      (   (glue == NULL) \
       || (glue->dev_ready == false) \
       || (   (glue->p_wlan_data != NULL) \
           && (glue->p_wlan_data->is_enabled == false)) \
       || (glue->err_count > MAX_ERR_COUNT))
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static const struct sdio_device_id ssv6xxx_sdio_devices[] __devinitconst =
#else
static const struct sdio_device_id ssv6xxx_sdio_devices[] =
#endif
{
    { SDIO_DEVICE(SSV_VENDOR_ID, SSV_CABRIO_DEVID) },
    {}
};
MODULE_DEVICE_TABLE(sdio, ssv6xxx_sdio_devices);
static bool _is_glue_invalid(struct ssv6xxx_sdio_glue *glue)
{
    if(NULL==glue)
        return true;

    if(false==glue->dev_ready)
        return true;

    if(NULL!=glue->p_wlan_data)
    {
        if(glue->p_wlan_data->is_enabled == false)
        {
            return true;
        }
    }
    
    if(glue->err_count > MAX_ERR_COUNT)
    {
        return true;
    }

    return false;
}
static bool ssv6xxx_is_ready (struct device *child)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue))
        return false;

    return glue->dev_ready;
} // end of - ssv6xxx_is_ready -

static int ssv6xxx_sdio_cmd52_read(struct device *child, u32 addr,
        u32 *value)
{
    int ret = -1;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func = NULL;

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        *value = sdio_readb(func, addr, &ret);
        sdio_release_host(func);
        CHECK_IO_RET(glue, ret);
    }

    return ret;
}

static int _ssv6xxx_sdio_cmd52_write(struct ssv6xxx_sdio_glue *glue, u32 addr,
        u32 value)
{
    int ret = -1;
    struct sdio_func *func = NULL;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        sdio_claim_host(func);
        sdio_writeb(func, value, addr, &ret);
        sdio_release_host(func);
        CHECK_IO_RET(glue, ret);
    }
    return ret;
}

static int ssv6xxx_sdio_cmd52_write(struct device *child, u32 addr,
        u32 value)
{
    int ret = -1;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        return _ssv6xxx_sdio_cmd52_write(glue, addr, value);
    }
    return ret;
}

static int __must_check __ssv6xxx_sdio_read_reg (struct ssv6xxx_sdio_glue *glue, u32 addr,
                                                 u32 *buf)
{    
    int ret = (-1);
    struct sdio_func *func = NULL;
#ifdef CONFIG_MMC_DISALLOW_STACK
    u8 *datap = glue->rreg_data;
#else
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u8 data[4];
#else
    u8 data[4];
#endif
    u8 *datap = data;
#endif

    if (IS_GLUE_INVALID(glue))
		return ret;
   
    //dev_err(&func->dev, "sdio read reg device[%08x] parent[%08x]\n",child,child->parent);

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        sdio_claim_host(func);

        // 4 bytes address
        datap[0] = (addr >> ( 0 )) &0xff;
        datap[1] = (addr >> ( 8 )) &0xff;
        datap[2] = (addr >> ( 16 )) &0xff;
        datap[3] = (addr >> ( 24 )) &0xff;

        //8 byte ( 4 bytes address , 4 bytes data )
        ret = sdio_memcpy_toio(func, glue->regIOPort, datap, 4);
        if (WARN_ON(ret))
        {
            dev_err(&func->dev, "sdio read reg write address failed (%d)\n", ret);
            goto io_err;
        }

        ret = sdio_memcpy_fromio(func, datap, glue->regIOPort, 4);
        if (WARN_ON(ret))
        {
            dev_err(&func->dev, "sdio read reg from I/O failed (%d)\n",ret);
        	goto io_err;
    	 }

        if(ret == 0)
        {
            *buf = (datap[0]&0xff);
            *buf = *buf | ((datap[1]&0xff)<<( 8 ));
            *buf = *buf | ((datap[2]&0xff)<<( 16 ));
            *buf = *buf | ((datap[3]&0xff)<<( 24 ));
        }
        else
            *buf = 0xffffffff;
io_err:
        sdio_release_host(func);
        //dev_dbg(&func->dev, "sdio read reg addr 0x%x, 0x%x  ret:%d\n", addr, *buf, ret);
        CHECK_IO_RET(glue, ret);
    }
    else
    {
        dev_err(&func->dev, "sdio read reg glue == NULL!!!\n");
    }

    //if (WARN_ON(ret))
    //  dev_err(&func->dev, "sdio read reg failed (%d)\n", ret);

    return ret;
}

static int __must_check ssv6xxx_sdio_read_reg(struct device *child, u32 addr,
        u32 *buf)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
	int i, ret;

	for (i = 0; i < MAX_REG_RETRY_CNT; i++) {
		ret = __ssv6xxx_sdio_read_reg(glue, addr, buf);
		if (!ret)
			return ret;
	
	}
	
	HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to read register, addr 0x%08x\n", __FUNCTION__, addr);

    return ret;
}

#ifdef ENABLE_WAKE_IO_ISR_WHEN_HCI_ENQUEUE
static int ssv6xxx_sdio_trigger_tx_rx (struct device *child)
{
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    struct mmc_host *host;

    if (IS_GLUE_INVALID(glue))
        return ret;

    func = dev_to_sdio_func(glue->dev);
    host = func->card->host;
    // Wake up SDIO IRQ handler which would call our ISR.
    mmc_signal_sdio_irq(host);

    return 0;
}
#endif // ENABLE_WAKE_IO_ISR_WHEN_HCI_ENQUEUE

static int __must_check __ssv6xxx_sdio_write_reg (struct ssv6xxx_sdio_glue *glue, u32 addr,
                                                  u32 buf)
{
    int ret = (-1);
    struct sdio_func *func = NULL;
#ifdef CONFIG_MMC_DISALLOW_STACK
    u8 *datap = glue->wreg_data;
#else
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u8 data[8];
#else
    u8 data[8];
#endif
    u8 *datap = data;
#endif

    if (IS_GLUE_INVALID(glue))
        return ret;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        //dev_dbg(&func->dev, "sdio write reg addr 0x%x, 0x%x\n",addr, buf);

        sdio_claim_host(func);

        // 4 bytes address
        datap[0] = (addr >> ( 0 )) &0xff;
        datap[1] = (addr >> ( 8 )) &0xff;
        datap[2] = (addr >> ( 16 )) &0xff;
        datap[3] = (addr >> ( 24 )) &0xff;

        // 4 bytes data
        datap[4] = (buf >> ( 0 )) &0xff;
        datap[5] = (buf >> ( 8 )) &0xff;
        datap[6] = (buf >> ( 16 )) &0xff;
        datap[7] = (buf >> ( 24 )) &0xff;

        //8 byte ( 4 bytes address , 4 bytes data )
        ret = sdio_memcpy_toio(func, glue->regIOPort, datap, 8);

        sdio_release_host(func);

        CHECK_IO_RET(glue, ret);
        //if (WARN_ON(ret))
        //  dev_err(&func->dev, "sdio write reg failed (%d)\n", ret);
    }
    else
    {
        dev_err(&func->dev, "sdio write reg glue == NULL!!!\n");
    }
    return ret;
}

static int __must_check ssv6xxx_sdio_write_reg(struct device *child, u32 addr,
        u32 buf)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
   	int i, ret;

	for (i = 0; i < MAX_REG_RETRY_CNT; i++) {
		ret = __ssv6xxx_sdio_write_reg(glue, addr, buf);
		if (!ret){

#ifdef __x86_64
			udelay(50);
#endif

			return ret;
	    }
	}
	
	HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to write register, addr 0x%08x, value 0x%08x\n", __FUNCTION__, addr, buf);

    return ret;
}

// Burst read from SSV6XXX's registers 
static int __must_check ssv6xxx_sdio_burst_read_reg(struct device *child, u32 *addr,
        u32 *buf, u8 reg_amount)
{   
#if 1
    printk("not support sdio burst read/write register\n");
    return 0;
#else
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func ;
#ifdef CONFIG_MMC_DISALLOW_STACK
    u32 *datap = glue->brreg_data;
#else
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u32 data[MAX_BURST_READ_REG_AMOUNT]={0};
#else
    u32 data[MAX_BURST_READ_REG_AMOUNT]={0};
#endif
    u32 *datap = data;
#endif
    u8 i = 0;

    if (IS_GLUE_INVALID(glue))
        return ret;

    if (reg_amount > MAX_BURST_READ_REG_AMOUNT)
    {
        HWIF_DBG_PRINT(glue->p_wlan_data, "The amount of sdio burst-read register must <= %d\n", 
				MAX_BURST_READ_REG_AMOUNT);
        return ret;
    }

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        sdio_claim_host(func);
      
        //one input is 4 bytes address 
        for (i=0; i<reg_amount; i++)
        {
            memcpy(&datap[i], &addr[i], 4);
        }        

        ret = sdio_memcpy_toio(func, IO_REG_BURST_RD_PORT_REG, datap, reg_amount*4);

        if (WARN_ON(ret))
        {
            dev_err(child->parent, "sdio burst-read reg write address failed (%d)\n", ret);
            goto io_err;
        }

        ret = sdio_memcpy_fromio(func, datap, IO_REG_BURST_RD_PORT_REG, reg_amount*4);
        if (WARN_ON(ret))
        {
            dev_err(child->parent, "sdio burst-read reg from I/O failed (%d)\n",ret);
        	goto io_err;
        }               

        //one output is 4bytes data
        if(ret == 0)
            memcpy(buf, datap, reg_amount*4);
        else
            memset(buf, 0xffffffff, reg_amount*4);
        
io_err:
        sdio_release_host(func);
        CHECK_IO_RET(glue, ret);       
    }
    else
    {
        dev_err(child->parent, "sdio burst-read reg glue == NULL!!!\n");
    }
    return ret;
#endif
}

// Burst write to SSV6XXX's registers 
static int __must_check ssv6xxx_sdio_burst_write_reg(struct device *child, u32 *addr,
        u32 *buf, u8 reg_amount)
{
#if 1
    printk("not support sdio burst read/write register\n");
    return 0;
#else
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func ;
#ifdef CONFIG_MMC_DISALLOW_STACK
    u8 (*datap)[8] = glue->bwreg_data;
#else
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u8 data[MAX_BURST_WRITE_REG_AMOUNT][8]={{0},{0}};
#else
    u8 data[MAX_BURST_WRITE_REG_AMOUNT][8]={{0},{0}};
#endif
    u8 (*datap)[8] = data;
#endif
    u8 i = 0;
    
    if (IS_GLUE_INVALID(glue))
        return ret;

    if (reg_amount > MAX_BURST_WRITE_REG_AMOUNT)
    {
        HWIF_DBG_PRINT(glue->p_wlan_data, "The amount of sdio burst-read register must <= %d\n", 
				MAX_BURST_WRITE_REG_AMOUNT);
        return ret;
    }
    
    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        sdio_claim_host(func);      

        //8byte for one output: 4bytes address + 4bytes values
        for (i=0; i<reg_amount; i++)
        {
            // 4 bytes address        
            datap[i][0] = (addr[i] >> ( 0 )) &0xff;
            datap[i][1] = (addr[i] >> ( 8 )) &0xff;
            datap[i][2] = (addr[i] >> ( 16 )) &0xff;
            datap[i][3] = (addr[i] >> ( 24 )) &0xff;

            // 4 bytes data
            datap[i][4] = (buf[i] >> ( 0 )) &0xff;
            datap[i][5] = (buf[i] >> ( 8 )) &0xff;
            datap[i][6] = (buf[i] >> ( 16 )) &0xff;
            datap[i][7] = (buf[i] >> ( 24 )) &0xff;            
        }        

        ret = sdio_memcpy_toio(func, IO_REG_BURST_WR_PORT_REG, datap, reg_amount*8);
  
        sdio_release_host(func);
        CHECK_IO_RET(glue, ret);
    }
    else
    {
        dev_err(child->parent, "sdio burst-write reg glue == NULL!!!\n");
    }
    return ret;
#endif
}

// Write to SSV6XXX's SRAM 
static int ssv6xxx_sdio_write_sram(struct device *child, u32 addr, u8 *data, u32 size)
{
    int     ret = -1;
    struct ssv6xxx_sdio_glue *glue;
    struct sdio_func *func=NULL;
    
    glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue))
        return ret;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);
    do {
        //Setting SDIO DMA address
        if (ssv6xxx_sdio_write_reg(child,0xc0000860,addr)) ;
    
        // Set data path to DMA to SRAM
        sdio_writeb(func, 0x2, REG_Fn1_STATUS, &ret);
        if (unlikely(ret)) break;

        ret = sdio_memcpy_toio(func, glue->dataIOPort, data, size);
        if (unlikely(ret)) break;
    
            // Set data path back to packet
        sdio_writeb(func, 0, REG_Fn1_STATUS, &ret);
        if (unlikely(ret)) break;
    } while (0);
    
    sdio_release_host(func);
    
    CHECK_IO_RET(glue, ret);
    
    return ret;
}

static int ssv6xxx_sdio_load_firmware(struct device *child, u32 start_addr, u8 *data, int data_length)
{
	return ssv6xxx_sdio_write_sram(child, start_addr, data, data_length);
}

static void ssv6xxx_sdio_load_fw_pre_config_hwif(struct device *child)
{
    struct ssv6xxx_sdio_glue *glue;
    struct sdio_func *func=NULL;

    glue = dev_get_drvdata(child->parent);
    if (!IS_GLUE_INVALID(glue)) {
        func = dev_to_sdio_func(glue->dev);
		ssv6xxx_low_sdio_clk(func);
	}
}

static void ssv6xxx_sdio_load_fw_post_config_hwif(struct device *child)
{
#ifndef SDIO_USE_SLOW_CLOCK
    struct ssv6xxx_sdio_glue *glue;
    struct sdio_func *func=NULL;

    glue = dev_get_drvdata(child->parent);
    if (!IS_GLUE_INVALID(glue)) {
        func = dev_to_sdio_func(glue->dev);
		ssv6xxx_high_sdio_clk(func);
	}
#endif // SDIO_USE_SLOW_CLOCK
}

static int ssv6xxx_sdio_irq_getstatus(struct device *child,int *status)
{
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue;

    struct sdio_func *func;
    glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        *status = sdio_readb(func, REG_INT_STATUS, &ret);
        sdio_release_host(func);
        CHECK_IO_RET(glue, ret);
    }
    return ret;
}

#if 0
static void _sdio_hexdump(const u8 *buf,
                             size_t len)
{
    size_t i;
    printk("\n-----------------------------\n");
    printk("hexdump(len=%lu):\n", (unsigned long) len);
    {
        for (i = 0; i < len; i++){
            
            printk(" %02x", buf[i]);
            if((i+1)%40 ==0)
                printk("\n");
        }
    }
    printk("\n-----------------------------\n");
}
#endif

static size_t ssv6xxx_sdio_get_readsz(struct device *child)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func ;
    size_t size = 0;
    int ret = -1;
    u32 addr = SD_REG_BASE+REG_CARD_PKT_LEN_0;
    u32 buf;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);
    
    ret = glue->p_wlan_data->ops->readreg(child, addr, &buf);
    if (ret) {
        dev_err(child->parent, "sdio read len failed ret[%d]\n",ret);
        size = 0;
    } else {
        size = (size_t)(buf&0xffff);
    }

    sdio_release_host(func);
    return size;
}

static size_t ssv6xxx_sdio_get_aggr_readsz(struct device *child, int mode)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func ;
#ifdef CONFIG_MMC_DISALLOW_STACK
    u32 size = 0;
    u32 *sizep = &glue->aggr_readsz;
#else
#ifdef CONFIG_FW_ALIGNMENT_CHECK
    PLATFORM_DMA_ALIGNED u32 size = 0;
#else
    u32 size = 0;
#endif
    u32 *sizep = &size;
#endif
    u32 buf = 0;
    int ret = -1;
    u32 tmp = 0;
    
    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);

    ret = sdio_memcpy_fromio(func, sizep, glue->dataIOPort, sizeof(u32)/* jmp_mpdu_len + accu_rx_len, total 4 bytes */);
    if (ret) { 
        dev_err(child->parent, "%s(): sdio read failed size ret[%d]\n", __func__, ret);
        *sizep = 0;
    }
    
    tmp = *sizep;
    size = (*sizep >> 16); // accu_rx_len
    sdio_release_host(func);
    
    if (0 == size) {
        printk("dlen = 0, read 0xc1000010, orig value 0x%08x\n", tmp);
        ret = glue->p_wlan_data->ops->readreg(child, 0xc1000010, &buf);
        if (ret) {
            printk("sdio read 0xc1000010 err %d\n", ret);
            size = 0;
        } else {
            size = (size_t)(buf & 0xffff);
            printk("read size = %d\n", (int)size);
        }
    }
    
    return (size_t)size;
}

static int __must_check ssv6xxx_sdio_read(struct device *child,
        void *buf, size_t *size, int mode)
{
    int ret = (-1), readsize = 0;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func ;

    if (IS_GLUE_INVALID(glue))
		return ret;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);

    readsize = sdio_align_size(func, *size);
	ret = sdio_memcpy_fromio(func, buf, glue->dataIOPort, readsize);
    if (ret)
        dev_err(child->parent, "%s(): sdio read failed size ret[%d]\n", __func__, ret);

    sdio_release_host(func);
    CHECK_IO_RET(glue, ret);

#if 0
    if(*size > 1500)
        _sdio_hexdump(buf,*size);
#endif

    return ret;
}

static int __must_check ssv6xxx_sdio_write(struct device *child,
        void *buf, size_t len,u8 queue_num)
{
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    int writesize;
    void *tempPointer;
    struct sk_buff *skb = (struct sk_buff *)buf;
    size_t txlen, remaining = len;

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        sdio_claim_host(func);
        while (remaining) {
#ifdef CONFIG_FW_ALIGNMENT_CHECK
            if (((unsigned long)skb->data) & (PLATFORM_DEF_DMA_ALIGN_SIZE - 1)) {
                //printk(KERN_ERR"SDIO Write: unalignmen!!!!!");
                if (remaining > SDIO_DMA_BUFFER_LEN) {
                    memcpy(glue->dmaData,skb->data, SDIO_DMA_BUFFER_LEN);
                    txlen = SDIO_DMA_BUFFER_LEN;
                    remaining -= SDIO_DMA_BUFFER_LEN;
                } else {
                    memcpy(glue->dmaData,skb->data, remaining);
                    txlen = remaining;
                    remaining = 0;
                }
                tempPointer = glue->dmaData;
            }
            else
#endif
            {
                tempPointer = skb->data;
                txlen = remaining;
                remaining = 0;
            }
#if 0
            if(len > 1500)
                _sdio_hexdump(skb->data,len);
#endif
            writesize = sdio_align_size(func, txlen);
            do
            {
                ret = sdio_memcpy_toio(func, glue->dataIOPort, tempPointer, writesize);
                if ( ret == -EILSEQ || ret == -ETIMEDOUT )
                {
                    ret = -1;
                    break;
                }
                else
                {
                    if(ret)
                        dev_err(&func->dev,"Unexpected return value ret=[%d]\n",ret);
                }
            }
            while( ret == -EILSEQ || ret == -ETIMEDOUT);
            CHECK_IO_RET(glue, ret);
            if (ret)
                dev_err(&func->dev, "sdio write failed (%d)\n", ret);
        }
        sdio_release_host(func);
    }
    return ret;
}

static void ssv6xxx_sdio_irq_handler(struct sdio_func *func)
{
    int status;
    struct ssv6xxx_sdio_glue *glue = sdio_get_drvdata(func);
    struct ssv6xxx_platform_data *pwlan_data;

    //dev_err(&func->dev, "ssv6xxx_sdio_irq_handler [%p] [%p]\n",func,glue);
    //WARN_ON(glue == NULL);

    if (IS_GLUE_INVALID(glue))
        return;

    pwlan_data = glue->p_wlan_data;

    if(glue->irq_handler != NULL)
    {
        atomic_set(&pwlan_data->irq_handling, 1);
        sdio_release_host(func);
        status = glue->irq_handler(0, glue);
        sdio_claim_host(func);
        atomic_set(&pwlan_data->irq_handling, 0);
    }
}

static void ssv6xxx_sdio_irq_setmask(struct device *child,int mask)
{
    int err_ret;    
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    
    if (IS_GLUE_INVALID(glue))
		return;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        sdio_writeb(func,mask, REG_INT_MASK, &err_ret);
        sdio_release_host(func);
        CHECK_IO_RET(glue, err_ret);
    }
}

static void ssv6xxx_sdio_irq_trigger(struct device *child)
{
    int err_ret;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;

    if (IS_GLUE_INVALID(glue))
		return;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        sdio_writeb(func,0x2, REG_INT_TRIGGER, &err_ret);
        sdio_release_host(func);
        CHECK_IO_RET(glue, err_ret);
    }
}

static int ssv6xxx_sdio_irq_getmask(struct device *child, u32 *mask)
{
    u8 imask = 0;
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        imask = sdio_readb(func,REG_INT_MASK, &ret);
        *mask = imask;
        sdio_release_host(func);
    }
    return ret;
}


static void ssv6xxx_sdio_irq_enable(struct device *child)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    int ret;
    struct ssv6xxx_platform_data *pwlan_data;

    if (IS_GLUE_INVALID(glue))
        return;

    pwlan_data = glue->p_wlan_data;
    func = dev_to_sdio_func(glue->dev);

    sdio_claim_host(func);

    //printk("%s(): enable\n", __FUNCTION__);
    //dev_err(&func->dev, "ssv6xxx_sdio_irq_enable\n");

    /* Register the isr */
    ret =  sdio_claim_irq(func, ssv6xxx_sdio_irq_handler);
    if (ret)
        dev_err(&func->dev, "Failed to claim sdio irq: %d\n", ret);

    sdio_release_host(func);
    CHECK_IO_RET(glue, ret);
}

static void ssv6xxx_sdio_irq_disable(struct device *child, bool iswaitirq)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    struct ssv6xxx_platform_data *pwlan_data;
    int ret;

    //WARN_ON(true);
    if (IS_GLUE_INVALID(glue))
		return;

    HWIF_DBG_PRINT(glue->p_wlan_data, "ssv6xxx_sdio_irq_disable\n");
    
	pwlan_data = glue->p_wlan_data;
    func = dev_to_sdio_func(glue->dev);
    if (func == NULL) {
        HWIF_DBG_PRINT(glue->p_wlan_data, "func == NULL\n");
        return;
    }
        
    sdio_claim_host(func);
        
    while (atomic_read(&pwlan_data->irq_handling)) {
        sdio_release_host(func);
        schedule_timeout(HZ / 10);
        sdio_claim_host(func);
    }

    ret = sdio_release_irq(func);
    if (ret)
        dev_err(&func->dev, "Failed to release sdio irq: %d\n", ret);

    sdio_release_host(func);

    //ssv6xxx_sdio_irq_setmask(child,mask);
}

#ifdef HWIF_SDIO_RX_IRQ
static void ssv6xxx_sdio_recv_rx_func(struct ssv6xxx_sdio_glue *glue)
{
    struct sdio_func *func = dev_to_sdio_func(glue->dev);
	struct sk_buff *rx_mpdu;
	int ret = 0, readsize = 0, rx_cnt = 0;
	size_t dlen;
	u32 status = SSV6XXX_INT_RX;
	u32 rx_mode = glue->p_wlan_data->rx_mode(glue->p_wlan_data->rx_mode_param);
	u32 frame_size = (rx_mode & RX_HW_AGG_MODE) ? MAX_HCI_RX_AGGR_SIZE : MAX_FRAME_SIZE_DMG;
    
	for (rx_cnt = 0 ; status & SSV6XXX_INT_RX ; rx_cnt++)
	{
		if (glue->is_rx_q_full(glue->rx_cb_args)) {
			goto unmask;
		}
		
		if (rx_mode == RX_NORMAL_MODE) {
			dlen = ssv6xxx_sdio_get_readsz(&glue->core->dev);
		} else {
			dlen = ssv6xxx_sdio_get_aggr_readsz(&glue->core->dev, rx_mode);
		}
		
        if ((dlen == 0) || (dlen > frame_size)) {
			printk(KERN_ERR "%s(): dlen = %d, goto skip.\n", __func__, (int)dlen);
			goto unmask;
		}
	    
        sdio_claim_host(func);
        readsize = sdio_align_size(func, dlen);
        sdio_release_host(func);
        rx_mpdu = glue->p_wlan_data->skb_alloc(glue->p_wlan_data->skb_param, readsize, GFP_KERNEL);

        if (rx_mpdu == NULL) {
			printk(KERN_ERR "%s(): Can't alloc skb.\n", __func__);
			goto unmask;
		}
		
        ret = ssv6xxx_sdio_read(&glue->core->dev, rx_mpdu->data, &dlen, rx_mode);
		if (ret < 0) {
			printk(KERN_ERR "%s(): Fail to sdio read %d\n", __FUNCTION__, ret);

			glue->p_wlan_data->skb_free(glue->p_wlan_data->skb_param, rx_mpdu);
            goto unmask;
		}
		
        skb_put(rx_mpdu, readsize);
        (*glue->rx_pkt)++;

		glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        if (0 != glue->recv_cnt) {
            if (rx_cnt > glue->recv_cnt)
                break;
        }

        ssv6xxx_sdio_irq_getstatus(&glue->core->dev, &status);
	}

unmask:
	ssv6xxx_sdio_irq_setmask(&glue->core->dev, 0xff & ~SSV6XXX_INT_RX);
	return;
}
#endif //HWIF_SDIO_RX_IRQ


static void ssv6xxx_sdio_recv_rx_work(struct work_struct *work)
{
    struct ssv6xxx_sdio_glue *glue = ((struct ssv6xxx_sdio_work_struct *)work)->glue;
    struct sdio_func *func = dev_to_sdio_func(glue->dev);
    struct sk_buff *rx_mpdu;
    int    ret = 0, rx_cnt = 0, readsize = 0;
    size_t dlen;
    u32    status = SSV6XXX_INT_RX;
	u32    rx_mode = glue->p_wlan_data->rx_mode(glue->p_wlan_data->rx_mode_param);
    u32    frame_size = (rx_mode & RX_HW_AGG_MODE) ? MAX_HCI_RX_AGGR_SIZE : MAX_FRAME_SIZE_DMG;
    
    for (rx_cnt = 0 ; status & SSV6XXX_INT_RX ; rx_cnt++)
    {
        if (glue->is_rx_q_full(glue->rx_cb_args)) {
            //printk("%s(): RX queue is full.\n", __func__);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            goto skip;
        }

        if (rx_mode == RX_NORMAL_MODE) {
            dlen = ssv6xxx_sdio_get_readsz(&glue->core->dev);
        } else {
            dlen = ssv6xxx_sdio_get_aggr_readsz(&glue->core->dev, rx_mode);
        }
        
        if ((dlen == 0) || (dlen > frame_size)) {
            printk("%s(): dlen = %d, goto skip.\n", __func__, (int)dlen);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            goto skip;
        }

        sdio_claim_host(func);
        readsize = sdio_align_size(func, dlen);
        sdio_release_host(func);
        rx_mpdu = glue->p_wlan_data->skb_alloc(glue->p_wlan_data->skb_param, readsize, GFP_KERNEL);
        
        if (rx_mpdu == NULL) {
            printk("%s(): Can't alloc skb.\n", __func__);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            goto skip;
        }
       
        ret = ssv6xxx_sdio_read(&glue->core->dev, rx_mpdu->data, &dlen, rx_mode);
        if (ret < 0)
        {
			printk(KERN_ERR "%s(): Fail to sdio read %d\n", __FUNCTION__, ret);

			glue->p_wlan_data->skb_free(glue->p_wlan_data->skb_param, rx_mpdu);
            goto unmask;
        }

        skb_put(rx_mpdu, readsize);
        (*glue->rx_pkt)++;

        glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        if (0 != glue->recv_cnt) {
            if (rx_cnt > glue->recv_cnt)
                break;
        }

        ssv6xxx_sdio_irq_getstatus(&glue->core->dev, &status);
    }

unmask:
    ssv6xxx_sdio_irq_setmask(&glue->core->dev, 0xff & ~SSV6XXX_INT_RX);
skip:
    return;
}

//static void ssv6xxx_sdio_recv_rx_tasklet(unsigned long priv)
//{
//}

static irqreturn_t ssv6xxx_sdio_isr(int irq, void *args)
{
    struct ssv6xxx_sdio_glue *glue = (struct ssv6xxx_sdio_glue *)args;
    int status = 0;

    ssv6xxx_sdio_irq_getstatus(&glue->core->dev, &status);

    if (status & SSV6XXX_INT_RX) {
        ssv6xxx_sdio_irq_setmask(&glue->core->dev, 0xff);

#ifdef HWIF_SDIO_RX_IRQ
		ssv6xxx_sdio_recv_rx_func(glue);
#else //HWIF_SDIO_RX_IRQ
        //if (ssv_rx_use_wq) {
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
        //} else {
        //    tasklet_schedule(&glue->rx_tasklet);
        //}
#endif //HWIF_SDIO_RX_IRQ
        
        (*glue->rx_isr_cnt)++;
        return IRQ_HANDLED;
    } else {
        return IRQ_NONE;
    }
}

static void ssv6xxx_sdio_irq_request(struct device *child, irq_handler_t irq_handler, void *irq_dev)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    bool isIrqEn = false;

    if (IS_GLUE_INVALID(glue))
		return;

    func = dev_to_sdio_func(glue->dev);

    //HWIF_DBG_PRINT(glue->p_wlan_data, "%s(): \n", __FUNCTION__);
    glue->irq_handler = irq_handler;

    if (isIrqEn)
    {
        ssv6xxx_sdio_irq_enable(child);
    }
}


static void ssv6xxx_sdio_read_parameter(struct sdio_func *func,
        struct ssv6xxx_sdio_glue *glue)
{
    int err_ret;
    sdio_claim_host(func);

    //get dataIOPort(Accesee packet buffer & SRAM)
    glue->dataIOPort = 0;
    glue->dataIOPort = glue->dataIOPort | (sdio_readb(func, REG_DATA_IO_PORT_0, &err_ret) << ( 8*0 ));
    glue->dataIOPort = glue->dataIOPort | (sdio_readb(func, REG_DATA_IO_PORT_1, &err_ret) << ( 8*1 ));
    glue->dataIOPort = glue->dataIOPort | (sdio_readb(func, REG_DATA_IO_PORT_2, &err_ret) << ( 8*2 ));

    //get regIOPort(Access register)
    glue->regIOPort = 0;
    glue->regIOPort = glue->regIOPort | (sdio_readb(func, REG_REG_IO_PORT_0, &err_ret) << ( 8*0 ));
    glue->regIOPort = glue->regIOPort | (sdio_readb(func, REG_REG_IO_PORT_1, &err_ret) << ( 8*1 ));
    glue->regIOPort = glue->regIOPort | (sdio_readb(func, REG_REG_IO_PORT_2, &err_ret) << ( 8*2 ));

    dev_err(&func->dev, "dataIOPort 0x%x regIOPort 0x%x\n",glue->dataIOPort,glue->regIOPort);

#ifdef CONFIG_PLATFORM_SDIO_BLOCK_SIZE
    err_ret = sdio_set_block_size(func,CONFIG_PLATFORM_SDIO_BLOCK_SIZE);
#else
    err_ret = sdio_set_block_size(func,SDIO_DEF_BLOCK_SIZE);
#endif
    if (err_ret != 0) {
        printk("SDIO setting SDIO_DEF_BLOCK_SIZE fail!!\n");
    }

    // output timing
#ifdef CONFIG_PLATFORM_SDIO_OUTPUT_TIMING
    sdio_writeb(func, CONFIG_PLATFORM_SDIO_OUTPUT_TIMING,REG_OUTPUT_TIMING_REG, &err_ret);
#else
    sdio_writeb(func, SDIO_DEF_OUTPUT_TIMING,REG_OUTPUT_TIMING_REG, &err_ret);
#endif

    // switch to normal mode
    // bit[1] , 0:normal mode, 1: Download mode(For firmware download & SRAM access)
    sdio_writeb(func, 0x00,REG_Fn1_STATUS, &err_ret);

#if 0
    //to check if support tx alloc mechanism
    sdio_writeb(func,SDIO_TX_ALLOC_SIZE_SHIFT|SDIO_TX_ALLOC_ENABLE,REG_SDIO_TX_ALLOC_SHIFT, &err_ret);
#endif

    sdio_release_host(func);
}

static void ssv6xxx_do_sdio_wakeup(struct sdio_func *func)
{
	int err_ret;
	
	if(func != NULL)
	{
		sdio_claim_host(func);
		sdio_writeb(func, 0x01, REG_PMU_WAKEUP, &err_ret);
		mdelay(10);
		sdio_writeb(func, 0x00, REG_PMU_WAKEUP, &err_ret);
		sdio_release_host(func);
	}
}

static void ssv6xxx_sdio_pmu_wakeup(struct device *child)
{
	
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func;
	if (glue != NULL) {
		func = dev_to_sdio_func(glue->dev);
		ssv6xxx_do_sdio_wakeup(func);

	}
}

static bool ssv6xxx_sdio_support_scatter(struct device *child)
{
    bool support = false;

    #if LINUX_VERSION_CODE > KERNEL_VERSION(3,0,0)
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
        
    do {
        if (IS_GLUE_INVALID(glue)) {
            dev_err(child, "ssv6xxx_sdio_enable_scatter glue == NULL!!!\n");
            break;
        }
        
        func = dev_to_sdio_func(glue->dev);                 
        if (func->card->host->max_segs < MAX_SCATTER_ENTRIES_PER_REQ) {
            dev_err(&func->dev, "host controller only supports scatter of :%d entries, driver need: %d\n",
            func->card->host->max_segs,
            MAX_SCATTER_ENTRIES_PER_REQ);
            break;
	    }
        
        support = true;
    } while (0);
    #endif

    return support;
}

static void ssv6xxx_sdio_setup_scat_data(struct sdio_scatter_req *scat_req,
					struct mmc_data *data)
{
	struct scatterlist *sg;
	int i;
    

	data->blksz = SDIO_DEF_BLOCK_SIZE;
	data->blocks = scat_req->len / SDIO_DEF_BLOCK_SIZE;

	printk("scatter: (%s)  (block len: %d, block count: %d) , (tot:%d,sg:%d)\n",
		   (scat_req->req & SDIO_WRITE) ? "WR" : "RD", 
		   data->blksz, data->blocks, scat_req->len,
		   scat_req->scat_entries);

	data->flags = (scat_req->req & SDIO_WRITE) ? MMC_DATA_WRITE :
						    MMC_DATA_READ;

	/* fill SG entries */
	sg = scat_req->sgentries;
	sg_init_table(sg, scat_req->scat_entries);

	/* assemble SG list */
	for (i = 0; i < scat_req->scat_entries; i++, sg++) {
		printk("%d: addr:0x%p, len:%d\n",
			   i, scat_req->scat_list[i].buf,
			   scat_req->scat_list[i].len);

		sg_set_buf(sg, scat_req->scat_list[i].buf,
			   scat_req->scat_list[i].len);
	}

	/* set scatter-gather table for request */
	data->sg = scat_req->sgentries;
	data->sg_len = scat_req->scat_entries;
}

static inline void ssv6xxx_sdio_set_cmd53_arg(u32 *arg, u8 rw, u8 func,
					     u8 mode, u8 opcode, u32 addr,
					     u16 blksz)
{
	*arg = (((rw & 1) << 31) |
		((func & 0x7) << 28) |
		((mode & 1) << 27) |
		((opcode & 1) << 26) |
		((addr & 0x1FFFF) << 9) |
		(blksz & 0x1FF));
}


static int ssv6xxx_sdio_rw_scatter(struct device *child,
			       struct sdio_scatter_req *scat_req)
{   
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
    struct sdio_func *func;
    
	struct mmc_request mmc_req;
	struct mmc_command cmd;
	struct mmc_data data;
    
	
	u8 opcode, rw;
	int status = 1;

	

    do{

        if(!glue){
            dev_err(child, "ssv6xxx_sdio_enable_scatter glue == NULL!!!\n");
            break;
        }
        
        func = dev_to_sdio_func(glue->dev);

//Scatter

    	memset(&mmc_req, 0, sizeof(struct mmc_request));
    	memset(&cmd, 0, sizeof(struct mmc_command));
    	memset(&data, 0, sizeof(struct mmc_data));

    	ssv6xxx_sdio_setup_scat_data(scat_req, &data);

    	opcode = 0;//FIXED ADDRESS;

    	rw = (scat_req->req & SDIO_WRITE) ? CMD53_ARG_WRITE : CMD53_ARG_READ;


    	/* set command argument */
    	ssv6xxx_sdio_set_cmd53_arg(&cmd.arg, rw, func->num,
    				  CMD53_ARG_BLOCK_BASIS, opcode, glue->dataIOPort,
    				  data.blocks);

    	cmd.opcode = SD_IO_RW_EXTENDED;
    	cmd.flags = MMC_RSP_SPI_R5 | MMC_RSP_R5 | MMC_CMD_ADTC;

    	mmc_req.cmd = &cmd;
    	mmc_req.data = &data;

    	mmc_set_data_timeout(&data, func->card);
    	/* synchronous call to process request */
    	mmc_wait_for_req(func->card->host, &mmc_req);

    	status = cmd.error ? cmd.error : data.error;


        if (cmd.error)
    		return cmd.error;
    	if (data.error)
    		return data.error;


    }while(0);


	return status;
}

/*
static void ssv6xxx_set_bus_width(struct sdio_func *func, u32 bus_width)
{
    struct mmc_host *host;
    u32 val = 0;
    u32 ret = 0;
    host = func->card->host;
    printk("%s: set bus width %d\n", __FUNCTION__, bus_width);
    sdio_claim_host(func);
    val = sdio_f0_readb(func, 0x07, &ret);
    if (ret == 0)
    {
        if (MMC_BUS_WIDTH_1 == bus_width)
        {
            val = val & 0xfc;
        }
        else if (MMC_BUS_WIDTH_4 == bus_width)
        {
            val = val & 0xfc;
            val = val | 0x02;
        }
        else
        {
            bus_width = MMC_BUS_WIDTH_1;
            val = val & 0xfc;
        }
        sdio_f0_writeb(func, val, 0x07, &ret);
        val = sdio_f0_readb(func, 0x07, &ret);
        printk("%s: set bus width %d, val = %x\n", __FUNCTION__, bus_width, val);
    }
    if (ret == 0)
    {
        host->ios.bus_width = bus_width;
        host->ops->set_ios(host, &host->ios);
    }
    else
    {
        printk("%s: set bus width %d err\n", __FUNCTION__, bus_width);
    }
    mdelay(20);
    sdio_release_host(func);
}
*/

static void ssv6xxx_set_sdio_clk(struct sdio_func *func, u32 sdio_hz)
{
	struct mmc_host *host;

	host = func->card->host;

	if (sdio_hz < host->f_min)
		sdio_hz = host->f_min;
	else if (sdio_hz > host->f_max)
		sdio_hz = host->f_max;

	printk("%s: set sdio clk %dHz\n", __FUNCTION__, sdio_hz);
	sdio_claim_host(func);
	host->ios.clock = sdio_hz;
	host->ops->set_ios(host, &host->ios);
	mdelay(20);
	sdio_release_host(func);
}

static void ssv6xxx_low_sdio_clk(struct sdio_func *func)
{
	ssv6xxx_set_sdio_clk(func, LOW_SPEED_SDIO_CLOCK);
}

static void ssv6xxx_high_sdio_clk(struct sdio_func *func)
{
#ifndef SDIO_USE_SLOW_CLOCK
	ssv6xxx_set_sdio_clk(func, HIGH_SPEED_SDIO_CLOCK);
#endif
}

static void ssv6xxx_sdio_reset(struct device *child)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func = dev_to_sdio_func(glue->dev);

    if (IS_GLUE_INVALID(glue))
		return;
	   
	HWIF_DBG_PRINT(glue->p_wlan_data, "%s\n", __FUNCTION__);
    ssv6xxx_do_sdio_reset_reinit(glue->p_wlan_data, func, glue);
}

static int ssv6xxx_sdio_property(struct device *child)
{
	return SSV_HWIF_CAPABILITY_INTERRUPT | SSV_HWIF_INTERFACE_SDIO;
}

static void ssv6xxx_sdio_sysplf_reset(struct device *child, u32 addr, u32 value)
{
    int retval = 0; 
    
    retval = ssv6xxx_sdio_write_reg(child, addr, value);
    if (retval)
        printk("Fail to reset sysplf.\n");
}

static void ssv6xxx_sdio_cleanup(struct device *child)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);

	if (IS_GLUE_INVALID(glue)) {
		printk("%s(): glue is invalid!\n", __func__);
		return;
	}

	HWIF_DBG_PRINT(glue->p_wlan_data, "%s(): \n", __FUNCTION__);

    //if (ssv_rx_use_wq) {
        cancel_work_sync((struct work_struct *)&glue->rx_work);
    //} else {
    //}
}

static void ssv6xxx_sdio_rx_task(struct device *child, 
			int (*rx_cb)(struct sk_buff *rx_skb, void *args), 
			int (*is_rx_q_full)(void *args), void *args, u32 *pkt, u32 *isr_cnt, u32 recv_cnt)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);

	glue->rx_cb = rx_cb;
	glue->rx_cb_args = args;
	glue->is_rx_q_full = is_rx_q_full;
	glue->rx_pkt = pkt;
    glue->rx_isr_cnt = isr_cnt;
    glue->recv_cnt = recv_cnt;

    ssv6xxx_sdio_irq_setmask(&glue->core->dev, 0xff);
    ssv6xxx_sdio_irq_disable(&glue->core->dev, false);

    ssv6xxx_sdio_irq_request(&glue->core->dev, ssv6xxx_sdio_isr, NULL);
}

#ifdef CONFIG_PM
static int ssv6xxx_sdio_suspend_late(struct device *child)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func = dev_to_sdio_func(glue->dev);
    int ret = 0;
    mmc_pm_flag_t flags = sdio_get_host_pm_caps(func);

    dev_info(glue->dev, "%s: suspend: PM flags = 0x%x\n",
             sdio_func_id(func), flags);

    if (!(flags & MMC_PM_KEEP_POWER))
    {
    	dev_err(&func->dev, "%s: cannot remain alive while host is suspended\n",
                sdio_func_id(func));
    	//return -ENOSYS;
    }

    if (flags & MMC_PM_KEEP_POWER) {
        ret = sdio_set_host_pm_flags(func, MMC_PM_KEEP_POWER);
        if (ret) {
    	    dev_err(&func->dev, "set sdio keep pwr flag failed %d\n", ret);
            return ret;
        }
    }

    if (flags & MMC_PM_WAKE_SDIO_IRQ) {
        ret = sdio_set_host_pm_flags(func, MMC_PM_WAKE_SDIO_IRQ);
        if (ret) {
    	    dev_err(&func->dev, "set sdio wake irq flag failed %d\n", ret);
            return ret;
        }
    }

    /*****************************************************************/
    /*                                                               */
    /* In SDIO clock always on case, lower SDIO clock can save power.*/
    /*                                                               */
    /*****************************************************************/

    ssv6xxx_low_sdio_clk(func);

    // ret = lbs_suspend(card->priv);
    //if (ret)
    //	return ret;

    cancel_work_sync((struct work_struct *)&glue->rx_work);

    HWIF_HAL_DISABLE_SDIO_RESET(glue);
#if 0
    sdio_claim_host(func);

    ret = sdio_release_irq(func);
    if (ret)
        dev_err(&func->dev, "Failed to release sdio irq: %d\n", ret);

    sdio_release_host(func);
#endif
    /*****************************************************************/
    /*                                                               */
    /* SDIO clock is about to down, so switch to async mode.         */
    /* Only in async mode, DAT1 can be toggled without SDIO clock.   */
    /*                                                               */
    /*****************************************************************/

    HWIF_HAL_ENABLE_SDIO_ASYNC_INT(glue);  // cmd52

    /*****************************************************************/
    /*                                                               */
    /* The last step before suspend: Restore REG_PMU_WAKEUP to 0.    */
    /* Turismo may suddenly fall asleep.                             */
    /* Do all register read/write before this step.                  */
    /* Set register 0xccb0b0f4=0 (ADR_PMU_RAM_13),                   */
    /* If host is awake, register ccb0b0f4 will be set to 1,         */
    /* FW will not enter sleep mode                                  */
    /* the two registers must be together                            */
    /*****************************************************************/
    ret = __ssv6xxx_sdio_write_reg(glue, 0xccb0b0f4, 0x0);
    _ssv6xxx_sdio_cmd52_write(glue, REG_PMU_WAKEUP, 0x0);

    return ret;
}
static int ssv6xxx_sdio_resume_early(struct device *child)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child->parent);
	struct sdio_func *func = NULL;
    int ret;

    if (!glue)
	    return 0;

    dev_info(glue->dev, "%s: start.\n", __FUNCTION__);
	func = dev_to_sdio_func(glue->dev);
    /*****************************************************************/
    /*                                                               */
    /* The basic step before the first step: Enabling SDIO func 1    */
    /*                                                               */
    /*****************************************************************/
    sdio_claim_host(func);
    sdio_enable_func(func);
    sdio_release_host(func);

    /*****************************************************************/
    /*                                                               */
    /* The first step after resume: Set REG_PMU_WAKEUP to wakeup PMU.*/
    /* Do all register read/write after this step.                   */
    /* FW will check 0xccb0b0f4 (ADR_PMU_RAM_13) to keep awake       */
    /* the two registers must be together */
    /*****************************************************************/
    _ssv6xxx_sdio_cmd52_write(glue, REG_PMU_WAKEUP, 0x1);
    ret = __ssv6xxx_sdio_write_reg(glue, 0xccb0b0f4, 0x1);

    /*****************************************************************/
    /*                                                               */
    /* SDIO clock is already up, so switch back to sync mode.        */
    /*                                                               */
    /*****************************************************************/

    HWIF_HAL_DISABLE_SDIO_ASYNC_INT(glue);

    /* Grab access to FN0 for ELP reg. */
    func->card->quirks |= MMC_QUIRK_LENIENT_FN0;

    /* Use block mode for transferring over one block size of data */
    func->card->quirks |= MMC_QUIRK_BLKSZ_FOR_BYTE_MODE;

    ssv6xxx_sdio_read_parameter(func, glue);

    ssv6xxx_sdio_direct_int_mux_mode(glue, false);

#if 0
    sdio_claim_host(func);

    //ret = sdio_release_irq(func);
    //if (ret)
    //    dev_err(&func->dev, "Failed to release sdio irq: %d\n", ret);
    ret =  sdio_claim_irq(func, ssv6xxx_sdio_irq_handler);
    if (ret)
        dev_err(&func->dev, "Failed to claim sdio irq: %d\n", ret);

    sdio_release_host(func);
#endif

    /*****************************************************************/
    /*                                                               */
    /* Set SDIO clock back to normal high freq.                      */
    /*                                                               */
    /*****************************************************************/

    ssv6xxx_high_sdio_clk(func);

    /*****************************************************************/
    /*                                                               */
    /* Finally, umask RX interrupt.                                  */
    /*                                                               */
    /*****************************************************************/

    ssv6xxx_sdio_irq_setmask(&glue->core->dev, 0xff & ~SSV6XXX_INT_RX);

    dev_info(glue->dev, "%s: end.\n", __FUNCTION__);

    return 0;
}
#endif

static void ssv6xxx_sdio_tx_req_cnt(struct device *dev, int *cnt)
{
    *cnt = 0;
    return;
}

static struct ssv6xxx_hwif_ops sdio_ops =
{
    .read                  		= ssv6xxx_sdio_read,
    .write                 		= ssv6xxx_sdio_write,
    .readreg               		= ssv6xxx_sdio_read_reg,
    .writereg              		= ssv6xxx_sdio_write_reg,
    .burst_readreg         		= ssv6xxx_sdio_burst_read_reg,
    .burst_writereg        		= ssv6xxx_sdio_burst_write_reg,
#ifdef ENABLE_WAKE_IO_ISR_WHEN_HCI_ENQUEUE
    .trigger_tx_rx         		= ssv6xxx_sdio_trigger_tx_rx,
#endif // ENABLE_WAKE_IO_ISR_WHEN_HCI_ENQUEUE
    .irq_getmask           		= ssv6xxx_sdio_irq_getmask,
    .irq_setmask           		= ssv6xxx_sdio_irq_setmask,
    .irq_enable            		= ssv6xxx_sdio_irq_enable,
    .irq_disable           		= ssv6xxx_sdio_irq_disable,
    .irq_getstatus         		= ssv6xxx_sdio_irq_getstatus,
    .irq_request           		= ssv6xxx_sdio_irq_request,
    .irq_trigger           		= ssv6xxx_sdio_irq_trigger,
    .pmu_wakeup            		= ssv6xxx_sdio_pmu_wakeup,
    .load_fw               		= ssv6xxx_sdio_load_firmware,
    .load_fw_pre_config_device 	= ssv6xxx_sdio_load_fw_pre_config_hwif,
    .load_fw_post_config_device = ssv6xxx_sdio_load_fw_post_config_hwif,
    .cmd52_read            		= ssv6xxx_sdio_cmd52_read,
    .cmd52_write           		= ssv6xxx_sdio_cmd52_write,
    .support_scatter       		= ssv6xxx_sdio_support_scatter,
    .rw_scatter            		= ssv6xxx_sdio_rw_scatter,
    .is_ready              		= ssv6xxx_is_ready,
    .write_sram            		= ssv6xxx_sdio_write_sram,  
    .interface_reset       		= ssv6xxx_sdio_reset, 
    .property              		= ssv6xxx_sdio_property,
    .hwif_rx_task               = ssv6xxx_sdio_rx_task,
    .sysplf_reset       		= ssv6xxx_sdio_sysplf_reset,
    .hwif_cleanup       		= ssv6xxx_sdio_cleanup,
#ifdef CONFIG_PM
    .hwif_suspend               = ssv6xxx_sdio_suspend_late, 
    .hwif_resume                = ssv6xxx_sdio_resume_early, 
#endif
    .get_tx_req_cnt             = ssv6xxx_sdio_tx_req_cnt,
};


#ifdef CONFIG_PCIEASPM
#include <linux/pci.h>
#if   (LINUX_VERSION_CODE < KERNEL_VERSION(5,4,0))
#include <linux/pci-aspm.h>
#endif 
// Disable PCIe power saving mode to ensure correct operation of SDIO interface.
static int cabrio_sdio_pm_check(struct sdio_func *func)
{
	struct pci_dev *pci_dev = NULL;
	struct mmc_card *card = func->card;
	struct mmc_host	*host = card->host;

	// If the corresponding SDIO host is PCI device. Find PCI device
	// with "sdhci-pci" driver and disable its ASPM function.

	// Check if the SDIO host is a PCI device.
	if (strcmp(host->parent->bus->name, "pci"))
	{
		dev_info(&func->dev, "SDIO host is not PCI device, but \"%s\".", host->parent->bus->name);
		return 0;
	}

	// Find the PCI device of SDHCI host
	for_each_pci_dev(pci_dev) {
		if (   ((pci_dev->class >> 8) != PCI_CLASS_SYSTEM_SDHCI)
			&& (   (pci_dev->driver == NULL)
				|| (strcmp(pci_dev->driver->name, "sdhci-pci") != 0)))
			continue;
		// Disable ASPM if it is a PCIe device.
		if (pci_is_pcie(pci_dev)) {
			u8 aspm;
			int pos;

			pos = pci_pcie_cap(pci_dev);
        	if (pos) {
        	    struct pci_dev *parent = pci_dev->bus->self;

        	    pci_read_config_byte(pci_dev, pos + PCI_EXP_LNKCTL, &aspm);
        	    aspm &= ~(PCIE_LINK_STATE_L0S | PCIE_LINK_STATE_L1);
        	    pci_write_config_byte(pci_dev, pos + PCI_EXP_LNKCTL, aspm);

        	    pos = pci_pcie_cap(parent);
        	    pci_read_config_byte(parent, pos + PCI_EXP_LNKCTL, &aspm);
        	    aspm &= ~(PCIE_LINK_STATE_L0S | PCIE_LINK_STATE_L1);
        	    pci_write_config_byte(parent, pos + PCI_EXP_LNKCTL, aspm);

        	    dev_info(&pci_dev->dev, "Clear PCI-E device and its parent link state L0S and L1 and CLKPM.\n");
        	}
		}
	}
	return 0;
}
#endif // CONFIG_PCIEASPM



static int ssv6xxx_sdio_power_on(struct ssv6xxx_platform_data * pdata, struct sdio_func *func)
{
	
	int ret = 0;

	if (pdata->is_enabled == true)
		return 0;

    printk("ssv6xxx_sdio_power_on\n");

	sdio_claim_host(func);
	ret = sdio_enable_func(func);
	sdio_release_host(func);
	
	
	if (ret) {
		printk("Unable to enable sdio func: %d)\n", ret);		
		return ret;
	}
	
	/*
	 * Wait for hardware to initialise. It should take a lot less than
	 * 10 ms but let's be conservative here.
	 */
	msleep(10);

	pdata->is_enabled = true;

	return ret;
}

static int ssv6xxx_do_sdio_init_seq_5537(struct sdio_func *func) {
    int status = 1;
    struct mmc_command cmd = {0};
 
    //Init seq - step #1: CMD5
    cmd.opcode = SD_IO_SEND_OP_COND;
    cmd.arg = 0;//Get I/O OCR.
    cmd.flags = MMC_RSP_SPI_R4 | MMC_RSP_R4 | MMC_CMD_BCR;
 
    sdio_claim_host(func);
    status = mmc_wait_for_cmd(func->card->host, &cmd, 0);
    sdio_release_host(func);
 
    if (status != 0) {
        //error handling
        printk("%s(): The 1st CMD5 failed.", __func__);
        return -1;
    }
 
    //Init seq - step #2: CMD5 with WV
    cmd.opcode = SD_IO_SEND_OP_COND;
    cmd.arg = MMC_VDD_30_31|MMC_VDD_31_32|MMC_VDD_32_33|MMC_VDD_33_34|MMC_VDD_34_35;
    cmd.flags = MMC_RSP_SPI_R4 | MMC_RSP_R4 | MMC_CMD_BCR;
 
    sdio_claim_host(func);
    status = mmc_wait_for_cmd(func->card->host, &cmd, 0);
    sdio_release_host(func);
 
    if (status != 0) {
        //error handling
        printk("%s(): The 2nd CMD5 failed.", __func__);
        return -1;
    }
 
    //Init seq - step #3: CMD3
    cmd.opcode = SD_SEND_RELATIVE_ADDR;
    cmd.arg = 0;
    cmd.flags = MMC_RSP_R6 | MMC_CMD_BCR;
 
    sdio_claim_host(func);
    status = mmc_wait_for_cmd(func->card->host, &cmd, 0);
    sdio_release_host(func);
 
    if (status == 0) {
        func->card->rca = cmd.resp[0] >> 16;
    } else {
        //error handling
        printk("%s(): CMD3 failed.", __func__);
        return -1;
    }
 
    //Init seq - step #4: CMD7
    cmd.opcode = MMC_SELECT_CARD;
 
    cmd.arg = func->card->rca << 16;
    cmd.flags = MMC_RSP_R1 | MMC_CMD_AC;
 
    sdio_claim_host(func);
    status = mmc_wait_for_cmd(func->card->host, &cmd, 0);
    sdio_release_host(func);

    if (status != 0) {
        //error handling
        printk("%s(): CMD7 failed.", __func__);
        return -1;
    }

    return 0;
}

static void ssv6xxx_do_sdio_reset_reinit(struct ssv6xxx_platform_data *pwlan_data, 
        struct sdio_func *func, struct ssv6xxx_sdio_glue *glue)
{
    int err_ret;
    struct mmc_host *host;

    if (IS_GLUE_INVALID(glue)) {
        printk("%s(): glue is invalid.\n", __func__);
        return;
    }

    // Do reset by sdio cccr06 bit3
    sdio_claim_host(func);
    sdio_f0_writeb(func, 0x08, SDIO_CCCR_ABORT, &err_ret);
    sdio_release_host(func);
    CHECK_IO_RET(glue, err_ret);

    // Do 5-5-3-7
    err_ret = ssv6xxx_do_sdio_init_seq_5537(func);
    CHECK_IO_RET(glue, err_ret);

    // Set host bus width
    sdio_claim_host(func);
    host = func->card->host;
    host->ios.bus_width = MMC_BUS_WIDTH_4;
    host->ops->set_ios(host, &host->ios);
    mdelay(20);
    sdio_release_host(func);

    // Set card bus width
    sdio_claim_host(func);
    sdio_f0_writeb(func, SDIO_BUS_WIDTH_4BIT, SDIO_CCCR_IF, &err_ret);
    sdio_release_host(func);
    CHECK_IO_RET(glue, err_ret);

    // Enable func #1 and set block size
    ssv6xxx_sdio_power_on(pwlan_data, func);
    ssv6xxx_sdio_read_parameter(func, glue);
}

static void ssv6xxx_sdio_direct_int_mux_mode(struct ssv6xxx_sdio_glue *glue, bool enable)
{
    int err_ret = (-1);
    struct sdio_func *func;
	u8 host_cfg;

    if (IS_GLUE_INVALID(glue))
		return;

    if (glue != NULL)
    {
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        host_cfg = sdio_readb(func, MCU_NOTIFY_HOST_CFG, &err_ret);
		if (err_ret == 0) {
			if (!enable) {
				host_cfg &= ~(0x04);
        		sdio_writeb(func, host_cfg, MCU_NOTIFY_HOST_CFG, &err_ret);
			} else {
				host_cfg |= (0x04);
        		sdio_writeb(func, host_cfg, MCU_NOTIFY_HOST_CFG, &err_ret);
			}
		}
        sdio_release_host(func);
        CHECK_IO_RET(glue, err_ret);
    }
}

static int ssv6xxx_sdio_power_off(struct ssv6xxx_platform_data * pdata, struct sdio_func *func)
{
	int ret;

	if (pdata->is_enabled == false)
		return 0;
    
    printk("ssv6xxx_sdio_power_off\n");

	/* Disable the card */
	sdio_claim_host(func);
	ret = sdio_disable_func(func);
	sdio_release_host(func);

	if (ret)
		return ret;

	pdata->is_enabled = false;

	return ret;
}

static void _read_chip_id (struct ssv6xxx_sdio_glue *glue)
{
    u32 regval;
    int ret;
    u8 _chip_id[SSV6XXX_CHIP_ID_LENGTH];
    u8 *c = _chip_id;
    int i = 0;

    //CHIP ID
    // Chip ID registers should be common to all SSV6xxx devices. So these registers 
    // must not come from ssv6xxx_reg.h but defined somewhere else.
    ret = __ssv6xxx_sdio_read_reg(glue, ADR_CHIP_ID_3, &regval);
    *((u32 *)&_chip_id[0]) = __be32_to_cpu(regval);

    if (ret == 0)
        ret = __ssv6xxx_sdio_read_reg(glue, ADR_CHIP_ID_2, &regval);
    *((u32 *)&_chip_id[4]) = __be32_to_cpu(regval);

    if (ret == 0)
        ret = __ssv6xxx_sdio_read_reg(glue, ADR_CHIP_ID_1, &regval);
    *((u32 *)&_chip_id[8]) = __be32_to_cpu(regval);

    if (ret == 0)
        ret = __ssv6xxx_sdio_read_reg(glue, ADR_CHIP_ID_0, &regval);
    *((u32 *)&_chip_id[12]) = __be32_to_cpu(regval);

    _chip_id[12+sizeof(u32)] = 0;

    // skip null for turimo fpga chip_id bug)
    while (*c == 0) {
        i++;
        c++;
        if (i == 16) { // max string length reached.
            c = _chip_id;
            break;
        }
    }

    if (*c != 0) {
        strncpy(glue->tmp_data.chip_id, c, SSV6XXX_CHIP_ID_LENGTH);
        dev_info(glue->dev, "CHIP ID: %s \n", glue->tmp_data.chip_id);
        strncpy(glue->tmp_data.short_chip_id, c, SSV6XXX_CHIP_ID_SHORT_LENGTH);
        glue->tmp_data.short_chip_id[SSV6XXX_CHIP_ID_SHORT_LENGTH] = 0;
    } else {
        dev_err(glue->dev, "Failed to read chip ID");
        glue->tmp_data.chip_id[0] = 0;
    }   
}

#if (defined(CONFIG_SSV_SDIO_INPUT_DELAY) && defined(CONFIG_SSV_SDIO_OUTPUT_DELAY))
static void ssv6xxx_sdio_delay_chain(struct sdio_func *func, u32 input_delay, u32 output_delay)
{
    u8 in_delay, out_delay;
    u8 delay[4];
    int ret = 0, i = 0;

    if ((input_delay == 0) && (output_delay == 0))
        return;

    for (i = 0; i < 4; i++) {
        //init delay value
        delay[i] = 0;

        in_delay = (input_delay >> ( i * 8 )) & 0xff;
        out_delay = (output_delay >> ( i * 8 )) & 0xff;

        //set delay value
        if (in_delay == SDIO_DELAY_LEVEL_OFF)
            delay[i] |= (1 << SDIO_INPUT_DELAY_SFT);
        else
            delay[i] |= ((in_delay-1) << SDIO_INPUT_DELAY_LEVEL_SFT);

        if (out_delay == SDIO_DELAY_LEVEL_OFF)
            delay[i] |= (1 << SDIO_OUTPUT_DELAY_SFT);
        else
            delay[i] |= ((out_delay-1) << SDIO_OUTPUT_DELAY_LEVEL_SFT);
    }

    printk("%s: delay chain data0[%02x], data1[%02x], data2[%02x], data3[%02x]\n", 
        __FUNCTION__, delay[0], delay[1], delay[2], delay[3]);

    //set sdio delay value
    sdio_claim_host(func);
    sdio_writeb(func, delay[0], REG_SDIO_DAT0_DELAY, &ret);
    sdio_writeb(func, delay[1], REG_SDIO_DAT1_DELAY, &ret);
    sdio_writeb(func, delay[2], REG_SDIO_DAT2_DELAY, &ret);
    sdio_writeb(func, delay[3], REG_SDIO_DAT3_DELAY, &ret);
	sdio_release_host(func);
}
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static int __devinit ssv6xxx_sdio_probe(struct sdio_func *func,
        const struct sdio_device_id *id)
#else
static int ssv6xxx_sdio_probe(struct sdio_func *func,
        const struct sdio_device_id *id)
#endif
{
    struct ssv6xxx_platform_data *pwlan_data;
    struct ssv6xxx_sdio_glue     *glue;

    //struct resource res[1];
    //mmc_pm_flag_t mmcflags;
    int ret = -ENOMEM;

    printk(KERN_INFO "=======================================\n");
    printk(KERN_INFO "==           RUN SDIO                ==\n");
    printk(KERN_INFO "=======================================\n");
    

    /* We are only able to handle the wlan function */
    if (func->num != 0x01)
        return -ENODEV;

    printk("max block count: %u\n", min(func->card->host->max_blk_count, 511u));

    glue = kzalloc(sizeof(*glue), GFP_KERNEL);
    if (!glue)
    {
        dev_err(&func->dev, "can't allocate glue\n");
        goto out;
    }

	/* INIT RX */
	//if (ssv_rx_use_wq) {
		glue->rx_work.glue = glue;
		INIT_WORK((struct work_struct *)&glue->rx_work, ssv6xxx_sdio_recv_rx_work);
		glue->wq = create_singlethread_workqueue("ssv6xxx_sdio_wq");
		if (!glue->wq) {
			dev_err(&func->dev, "Could not allocate Work Queue\n");
			goto out;
		}
	//} else {
	//	tasklet_init(&glue->rx_tasklet, ssv6xxx_sdio_recv_rx_tasklet, (unsigned long)glue);
	//}

    /* Tell PM core that we don't need the card to be powered now */
    //pm_runtime_put_noidle(&func->dev);
    
    /* 
     * Setting SDIO delay chain
     * Note: delay chain function cannot work in CABRIO 
     */
#if (defined(CONFIG_SSV_SDIO_INPUT_DELAY) && defined(CONFIG_SSV_SDIO_OUTPUT_DELAY))
    ssv6xxx_sdio_delay_chain(func, CONFIG_SSV_SDIO_INPUT_DELAY, CONFIG_SSV_SDIO_OUTPUT_DELAY);
#endif
    //Setting SDIO to 25M
	ssv6xxx_low_sdio_clk(func);

    pwlan_data = &glue->tmp_data;
    memset(pwlan_data, 0, sizeof(struct ssv6xxx_platform_data));
   
    atomic_set(&pwlan_data->irq_handling, 0);
    
    glue->dev = &func->dev;
    //init_waitqueue_head(&glue->irq_wq);

    /* Grab access to FN0 for ELP reg. */
    func->card->quirks |= MMC_QUIRK_LENIENT_FN0;

    /* Use block mode for transferring over one block size of data */
    func->card->quirks |= MMC_QUIRK_BLKSZ_FOR_BYTE_MODE;

    glue->dev_ready = true;
    /* if sdio can keep power while host is suspended, enable wow */
    //mmcflags = sdio_get_host_pm_caps(func);
    //dev_err(glue->dev, "sdio PM caps = 0x%x\n", mmcflags);
/*
    if (mmcflags & MMC_PM_KEEP_POWER)
        pwlan_data->pwr_in_suspend = true;
*/
    //store sdio vendor/device id
    pwlan_data->vendor = func->vendor;
    pwlan_data->device = func->device;

    dev_err(glue->dev, "vendor = 0x%x device = 0x%x\n",
            pwlan_data->vendor, pwlan_data->device);
    #ifdef CONFIG_PCIEASPM
    cabrio_sdio_pm_check(func);
    #endif // CONFIG_PCIEASPM
    pwlan_data->ops = &sdio_ops;

    sdio_set_drvdata(func, glue);

#ifdef CONFIG_PM
    /*
        Some platforms LDO-EN can not be pulled down. WIFI cause leakage.
        Avoid leakage issues
    */
#if 0
    ssv6xxx_do_sdio_wakeup(func);
#endif
#endif

    ssv6xxx_sdio_power_on(pwlan_data, func);
    
    ssv6xxx_sdio_read_parameter(func, glue);

    //do system reset from sdio client   
    ssv6xxx_do_sdio_reset_reinit(pwlan_data, func, glue);
    ssv6xxx_sdio_direct_int_mux_mode(glue, false);
    //ssv6xxx_set_bus_width(func, MMC_BUS_WIDTH_4);

    /* Tell PM core that we don't need the card to be powered now */
    //pm_runtime_put_noidle(&func->dev);
    _read_chip_id(glue);

    glue->core = platform_device_alloc(pwlan_data->short_chip_id, -1);
    if (!glue->core)
    {
        dev_err(glue->dev, "can't allocate platform_device");
        ret = -ENOMEM;
        goto out_free_glue;
    }

    glue->core->dev.parent = &func->dev;

    //dev_err(glue->dev, "sdio ssv6xxx_sdio_probe device[%08x] parent[%08x]\n",&glue->core->dev,&func->dev);

    /*
    	memset(res, 0x00, sizeof(res));

    	res[0].start = pwlan_data->irq;
    	res[0].flags = IORESOURCE_IRQ;
    	res[0].name = "irq";

    	ret = platform_device_add_resources(glue->core, res, ARRAY_SIZE(res));
    	if (ret) {
    		dev_err(glue->dev, "can't add resources\n");
    		goto out_dev_put;
    	}
    */    
    ret = platform_device_add_data(glue->core, pwlan_data,
                                   sizeof(*pwlan_data));
    if (ret)
    {
        dev_err(glue->dev, "can't add platform data\n");
        goto out_dev_put;
    }

    glue->p_wlan_data = glue->core->dev.platform_data;

    /* Initialize ssv6xxx HWIF HAL layer function*/
    if ((ret = HWIF_HAL_INIT(glue)) != 0) {
        goto out_dev_put;
    }

    ssv6xxx_sdio_irq_setmask(&glue->core->dev,0xff);
    
    ret = platform_device_add(glue->core);
    if (ret)
    {
        dev_err(glue->dev, "can't add platform device\n");
        goto out_dev_put;
    }

    return 0;

out_dev_put:
    platform_device_put(glue->core);

out_free_glue:
    if (glue != NULL)
        kfree(glue);
out:
    return ret;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static void __devexit ssv6xxx_sdio_remove(struct sdio_func *func)
#else
static void ssv6xxx_sdio_remove(struct sdio_func *func)
#endif
{
    struct ssv6xxx_sdio_glue *glue = sdio_get_drvdata(func);

    printk("ssv6xxx_sdio_remove..........\n");
    
    /* Undo decrement done above in ssv6xxx_probe */
    //pm_runtime_get_noresume(&func->dev);

    
    if ( glue )
    {
        // Remove IRQ handler once card is removed.
        printk("ssv6xxx_sdio_remove - ssv6xxx_sdio_irq_disable\n");
        ssv6xxx_sdio_irq_disable(&glue->core->dev,false);
        // Mark device is not ready for access.
        glue->dev_ready = false;
#if 0
        ssv6xxx_dev_remove(glue->dev);
#endif

        //Setting SDIO to 25M
		ssv6xxx_low_sdio_clk(func);

        printk("ssv6xxx_sdio_remove - disable mask\n");
        ssv6xxx_sdio_irq_setmask(&glue->core->dev,0xff);

        ssv6xxx_sdio_power_off(glue->p_wlan_data, func);

        printk("platform_device_del \n");
        platform_device_del(glue->core);

        printk("platform_device_put \n");
        platform_device_put(glue->core);
                   
	    //if (ssv_rx_use_wq) {
            cancel_work_sync((struct work_struct *)&glue->rx_work);
	    	destroy_workqueue(glue->wq);
	    //} else {
	    	//tasklet_kill(&glue->rx_tasklet);
	    //}

        kfree(glue);
    }
    sdio_set_drvdata(func, NULL);

    printk("ssv6xxx_sdio_remove leave..........\n");
}

#ifdef CONFIG_PM
static int ssv6xxx_sdio_suspend(struct device *dev)
{
    /* Moved to ssv6xxx_sdio_suspend_late(). */
    return 0;
}


static int ssv6xxx_sdio_resume(struct device *dev)
{
    /* Moved to ssv6xxx_sdio_resume_early(). */
    return 0;
}

static const struct dev_pm_ops ssv6xxx_sdio_pm_ops =
{
    .suspend    = ssv6xxx_sdio_suspend,
    .resume     = ssv6xxx_sdio_resume,
};
#endif

struct sdio_driver ssv6xxx_sdio_driver =
{
    .name		= "SSV6XXX_SDIO",
    .id_table	= ssv6xxx_sdio_devices,
    .probe		= ssv6xxx_sdio_probe,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
    .remove		= __devexit_p(ssv6xxx_sdio_remove),
#else
    .remove     = ssv6xxx_sdio_remove,
#endif
    
#ifdef CONFIG_PM
    .drv = {
        .pm = &ssv6xxx_sdio_pm_ops,
    },
#endif
};
EXPORT_SYMBOL(ssv6xxx_sdio_driver);

int ssv6xxx_sdio_init(void)
{
    printk(KERN_INFO "ssv6xxx_sdio_init\n");
    return sdio_register_driver(&ssv6xxx_sdio_driver);
}

void ssv6xxx_sdio_exit(void)
{
    printk(KERN_INFO "ssv6xxx_sdio_exit\n");
    sdio_unregister_driver(&ssv6xxx_sdio_driver);
}
EXPORT_SYMBOL(ssv6xxx_sdio_init);
EXPORT_SYMBOL(ssv6xxx_sdio_exit);

MODULE_LICENSE("GPL");

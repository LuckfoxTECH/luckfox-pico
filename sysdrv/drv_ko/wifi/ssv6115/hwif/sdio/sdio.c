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
#include <linux/mmc/mmc.h>
#include <linux/mmc/sd.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/delay.h>
#include <linux/pm_runtime.h>

#include "ssv_cfg.h"

#include <hwif/hwif.h>
#include "sdio_def.h"
#include "sdio.h"
#include "ssv_debug.h"
#include "utils/ssv_alloc_skb.h"

// #include "rf_phy/6030B1/ssv_calibration.h"
// #include "hwif/common/common.h"

#ifndef HWIF_DIS_FW_DOWNLOAD
#include "ssv6x5x-sw.h"
#include "ssv6x5x-sw_2.h"
#endif

#define LOW_SPEED_SDIO_CLOCK		(25000000)
#define HIGH_SPEED_SDIO_CLOCK		(50000000)

#define MAX_REG_RETRY_CNT	(3)

#define SSV_VENDOR_ID	0x3030
#define SSV_CABRIO_DEVID	0x3030

#define SSV_VENDOR_ID_6030		(0x5356)
#define SSV_DEVICE_ID_6030		(0x6030)

#define CHECK_IO_RET(GLUE, RET) \
    do { \
        if (RET) { \
            if ((++((GLUE)->err_count)) > MAX_ERR_COUNT) \
                SSV_LOG_DBG("MAX SDIO Error\n"); \
        } else \
            (GLUE)->err_count = 0; \
    } while (0)
    
#define MAX_ERR_COUNT		(10)

struct ssv6xxx_sdio_glue
{
    void                         *plat_hw;
    struct device                *dev;
    bool                          is_enabled;
    atomic_t                      irq_handling;
    PLATFORM_DMA_ALIGNED u8 rreg_data[4];
    PLATFORM_DMA_ALIGNED u8 wreg_data[8];
    PLATFORM_DMA_ALIGNED u32 brreg_data[MAX_BURST_READ_REG_AMOUNT];
    PLATFORM_DMA_ALIGNED u8 bwreg_data[MAX_BURST_WRITE_REG_AMOUNT][8];
    PLATFORM_DMA_ALIGNED u32 aggr_readsz;
    PLATFORM_DMA_ALIGNED u8 dmaData[SDIO_DMA_BUFFER_LEN];
    /* for ssv SDIO */
    unsigned int                dataIOPort;
    unsigned int                regIOPort;

    irq_handler_t               irq_handler;
    bool                        dev_ready;
    unsigned int                err_count;
	struct workqueue_struct *wq;
	struct ssv6xxx_sdio_work_struct rx_work;
	//struct tasklet_struct rx_tasklet;
	u32 rx_pkt;
	u32 rx_isr_cnt;
    u32 recv_cnt;
	void *rx_cb_args;
    int (*rx_cb)(struct sk_buff *rx_skb, void *args);
    int (*is_rx_q_full)(void *);
    u32 tx_pkt;
};

extern struct ssv6xxx_cfg ssv_cfg;

#if 0
static void ssv6xxx_high_sdio_clk(struct sdio_func *func);
#endif
static void ssv6xxx_low_sdio_clk(struct sdio_func *func);
static void ssv6xxx_high_sdio_clk(struct sdio_func *func);
static void ssv6xxx_do_sdio_reset_reinit(struct sdio_func *func, struct ssv6xxx_sdio_glue *glue);
static void ssv6xxx_sdio_direct_int_mux_mode(struct ssv6xxx_sdio_glue *glue, bool enable);

#if 1
static bool _ssv_is_glue_invalid(struct ssv6xxx_sdio_glue *glue);
#define IS_GLUE_INVALID(glue)  _ssv_is_glue_invalid(glue)

#else
#define IS_GLUE_INVALID(glue)  \
      (   (glue == NULL) \
       || (glue->dev_ready == false) \
       || (glue->err_count > MAX_ERR_COUNT))
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static const struct sdio_device_id ssv6xxx_sdio_devices[] __devinitconst =
#else
static const struct sdio_device_id ssv6xxx_sdio_devices[] =
#endif
{
    { SDIO_DEVICE(SSV_VENDOR_ID, SSV_CABRIO_DEVID) },
    { SDIO_DEVICE(SSV_VENDOR_ID_6030, SSV_DEVICE_ID_6030) },
    {}
};
MODULE_DEVICE_TABLE(sdio, ssv6xxx_sdio_devices);

static bool _ssv_is_glue_invalid(struct ssv6xxx_sdio_glue *glue)
{
    if(NULL == glue)
        return true;

    if(false == glue->dev_ready)
        return true;

    if (false == glue->is_enabled)
        return false;

    if(glue->err_count > MAX_ERR_COUNT)
    {
        return true;
    }

    return false;
}
#if (HWIF_SUPPORT == 2)
static int ssv6xxx_sdio_cmd52_read(struct device *child, u32 addr,
        u32 *value)
{
    int ret = -1;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
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

static int _ssv6xxx_sdio_cmd52_write(struct ssv6xxx_sdio_glue *glue, u32 addr, u32 value)
{
    int ret = -1;
    struct sdio_func *func = NULL;

    if (IS_GLUE_INVALID(glue))
		return ret;

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

static int ssv6xxx_sdio_cmd52_write(struct device *child, u32 addr, u32 value)
{
    int ret = -1;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        return _ssv6xxx_sdio_cmd52_write(glue, addr, value);
    }
    return ret;
}
#endif
static int __ssv6xxx_sdio_read_reg (struct ssv6xxx_sdio_glue *glue, u32 addr,
                                                 u32 *buf)
{    
    int ret = (-1);
    struct sdio_func *func = NULL;
    u8 *datap = glue->rreg_data;
    
    if (IS_GLUE_INVALID(glue)) {
		return -ENODEV;
    }

    //dev_err(&func->dev, "sdio read reg device[%08x] \n",child);

#ifdef SDIO_CMD52_CHK_TX_RESOURCE
    ///@FIXME: Change to use CMD52 to check TX resource.
    ///CMD53: 0x08C10050
    ///CMD52: 0x9F
    if(0x08c10050 == addr)
    {
        u8 val = 0;
        func = dev_to_sdio_func(glue->dev);

        sdio_claim_host(func);
        val = sdio_readb(func, 0x9f, &ret);
        sdio_release_host(func);
        CHECK_IO_RET(glue, ret);
        if(0 == val)
        {
            *buf = 0;
        }
        else
        {
            *buf = (((u32)val)<<16);
        }
        return ret;
    }
#endif

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

static int ssv6xxx_sdio_read_reg(struct device *child, u32 addr,
        u32 *buf)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    //int i, ret;
    int ret;

	//for (i = 0; i < MAX_REG_RETRY_CNT; i++) {
		ret = __ssv6xxx_sdio_read_reg(glue, addr, buf);
		if (!ret)
			return ret;
	
	//}
	
	SSV_LOG_DBG("%s: Fail to read register, addr 0x%08x\n", __FUNCTION__, addr);
    return ret;
}

static int __ssv6xxx_sdio_write_reg (struct ssv6xxx_sdio_glue *glue, u32 addr,
                                                  u32 buf)
{
    int ret = (-1);
    struct sdio_func *func = NULL;
    u8 *datap = glue->wreg_data;

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

static int ssv6xxx_sdio_write_reg(struct device *child, u32 addr, u32 buf)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
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
	
	SSV_LOG_DBG("%s: Fail to write register, addr 0x%08x, value 0x%08x\n", __FUNCTION__, addr, buf);
    return ret;
}

#if 0
static void ssv6xxx_sdio_save_calibr_result(struct device *child)
{
    int i;
    u32 regval = 0;
    ssv_cabrio_reg *pcalib = NULL;
    u32 calib_size = ssv_get_calibr_rsult_size();
    ssv_get_calibr_rsult_addr(&pcalib);

    //SSV_LOG_DBG("sdio save\n");
    for(i = 0; i < calib_size/sizeof(ssv_cabrio_reg); i++)
    {
        ssv6xxx_sdio_read_reg(child, pcalib->address, &regval);
        pcalib->data = regval;
        //SSV_LOG_DBG("%08X: %08X\n", pcalib->address, pcalib->data);
        pcalib++;
    }

}

static void ssv6xxx_sdio_restore_calibration(struct device *child)
{
    int i;
    ssv_cabrio_reg *pcalib = NULL;
    u32 calib_size = ssv_get_calibr_rsult_size();
    ssv_get_calibr_rsult_addr(&pcalib);

    //SSV_LOG_DBG("sdio restore\n");
    for(i = 0; i < calib_size/sizeof(ssv_cabrio_reg); i++)
    {
        ssv6xxx_sdio_write_reg(child, pcalib->address, pcalib->data);
        //SSV_LOG_DBG("%08X: %08X\n", pcalib->address, pcalib->data);
        pcalib++;
    }
}
#endif


static void ssv6xxx_sdio_load_fw_post_config_hwif(struct device *child)
{

    struct ssv6xxx_sdio_glue *glue;
    struct sdio_func *func=NULL;

    glue = dev_get_drvdata(child);
    if (!IS_GLUE_INVALID(glue)) {
        func = dev_to_sdio_func(glue->dev);
		ssv6xxx_high_sdio_clk(func);
	}
}

////load firmware function +++
#ifndef HWIF_DIS_FW_DOWNLOAD
// Write to SSV6XXX's SRAM 
static int ssv6xxx_sdio_write_sram(struct device *child, u32 addr, u8 *data, u32 size)
{
    int     ret = -1;
    
    struct ssv6xxx_sdio_glue *glue;
    struct sdio_func *func=NULL;
    
    glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue))
        return ret;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);

//SSV_LOG_DBG("[SDIO] ssv6xxx_sdio_write_sram addr = 0x%x, len = %d\n", addr, size);
//SSV_LOG_DBG(_DBG("[SDIO] %x %x %x %x %x %x %x %x .... %x %x %x %x %x %x %x %x\n", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[804], data[805], data[806], data[807], data[808], data[809], data[810], data[811]);

    do {
        //Setting SDIO DMA address
        // if (ssv6xxx_sdio_write_reg(child,0xc0000860,addr)) ;
        if (ssv6xxx_sdio_write_reg(child,0x08c10260,addr)) ;
    
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
 
    glue = dev_get_drvdata(child);
    if (!IS_GLUE_INVALID(glue)) {
        func = dev_to_sdio_func(glue->dev);
		ssv6xxx_low_sdio_clk(func);
	}
}

//new
#define FW_CHECKSUM_INIT                    (0x12345678)
#define FW_BLOCK_SIZE                       0x800
#define CHECKSUM_BLOCK_SIZE                 1024
//#define FW_CHECKSUM_ADDR 0x01032d24
#define MAX_FIRMWARE_BLOCKS_CNT 3
#define MAC_FIRMWARE_BLOCKS_LEN 8

//#define FIRMWARE_DOWNLOAD 0xf0
//#define MAX_USB_BLOCK 512
//#define FW_BLOCK_SIZE MAX_USB_BLOCK

struct ssv6xxx_block_info {
    u32 start_addr;
    u32 len;
};

struct ssv6xxx_fw_info {
    struct ssv6xxx_block_info block[MAX_FIRMWARE_BLOCKS_CNT];
    u32 block_num;
    u32 check_sum;
};

static struct ssv6xxx_fw_info fw_info;


static int ssv6xxx_read_fw_block(char *buf, int len, void *image)
{
    struct file *fp = (struct file *)image;
    int rdlen;

    if (!image)
        return 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
    rdlen = kernel_read(fp, buf, len, &fp->f_pos);
#else
    rdlen = kernel_read(fp, fp->f_pos, buf, len);
    if (rdlen > 0)
        fp->f_pos += rdlen;
#endif

    return rdlen;
}

static u32 ssv6xxx_write_firmware_to_sram(struct device *child, void *fw_buffer,
        void *fw_fp, int fw_len, u32 sram_start_addr, u8 openfile, bool need_checksum, bool *write_sram_err)
{
    int ret = 0, i = 0;
    u32 len = 0, total_len = 0;
    u32 sram_addr = sram_start_addr;
    u32 *fw_data32 = NULL;
    u32 fw_res_size = fw_len;
    u32 rsize = 0;
    u32 checksum = 0;
    u8 *pt=NULL;

    SSV_LOG_DBG("Writing firmware to SSV6XXX...\n");
    while (fw_res_size) { // load fw block size
        memset(fw_buffer, 0xA5, CHECKSUM_BLOCK_SIZE);
		rsize = (fw_res_size>CHECKSUM_BLOCK_SIZE)?CHECKSUM_BLOCK_SIZE:fw_res_size;
        if (openfile) {
            //((struct file *)fw_fp)->f_pos = fw_offset;
            if (!(len = ssv6xxx_read_fw_block((char*)fw_buffer, rsize, fw_fp))) {
                break;
            }
        } else {
            memcpy((void *)fw_buffer, (const void *)fw_fp, rsize);
            pt=(u8 *)(fw_fp);
            fw_fp = (void *)(pt+CHECKSUM_BLOCK_SIZE);
        }

        fw_res_size -= rsize;
        total_len += rsize;

        if ((ret = ssv6xxx_sdio_load_firmware(child, sram_addr, (u8 *)fw_buffer, CHECKSUM_BLOCK_SIZE)) != 0) {
            *write_sram_err = true;
            goto out;
        }

        // checksum
        if (need_checksum) {
            fw_data32 = (u32 *)fw_buffer;
            for (i = 0; i < (CHECKSUM_BLOCK_SIZE / sizeof(u32)); i++) 
                checksum += fw_data32[i];
        }

        sram_addr += CHECKSUM_BLOCK_SIZE;
    }

out:
    return checksum;
}

static int ssv6xxx_get_firmware_info(void *fw_fp, u8 openfile)
{
    int offset = 0;
    int i = 0, len = 0;
    u8 buf[MAX_FIRMWARE_BLOCKS_CNT * MAC_FIRMWARE_BLOCKS_LEN];
    u32 fw_block_info_len = 0;
    u32 start_addr = 0, fw_block_len = 0;

    memset(buf, 0, MAX_FIRMWARE_BLOCKS_CNT * MAC_FIRMWARE_BLOCKS_LEN);
    if (openfile) {
        // reset file pos
        ((struct file *)fw_fp)->f_pos = 0;

        // fw info len, 4 byte
        if (!(len = ssv6xxx_read_fw_block((char *)&fw_info.block_num, sizeof(u32), fw_fp))) {
            offset = -1;
            goto out;
        }

        // fw block info
        fw_block_info_len = fw_info.block_num * MAC_FIRMWARE_BLOCKS_LEN;
        if (!(len = ssv6xxx_read_fw_block((u8 *)buf, fw_block_info_len, fw_fp))) {
            offset = -1;
            goto out;
        }

    } else {
        memcpy(&fw_info.block_num, (u8 *)fw_fp, (sizeof(u32)));
        fw_block_info_len = fw_info.block_num * MAC_FIRMWARE_BLOCKS_LEN;
        memcpy((u8 *)buf, (u8 *)fw_fp+sizeof(u32), fw_block_info_len);
    }

    // parse fw block info
    for (i = 0; i < fw_info.block_num; i++) {
        start_addr =   (buf[i*8+0] << 0) | (buf[i*8+1] << 8) | (buf[i*8+2] << 16) | (buf[i*8+3] << 24); 
        fw_block_len = (buf[i*8+4] << 0) | (buf[i*8+5] << 8) | (buf[i*8+6] << 16) | (buf[i*8+7] << 24); 
        fw_info.block[i].len = fw_block_len;
        fw_info.block[i].start_addr = start_addr;
    }
    fw_info.check_sum = 0;
    offset = 4 + fw_block_info_len;

out:
    return offset;
}

static void *ssv6xxx_open_firmware(char *user_mainfw)
{
    struct file *fp;

    fp = filp_open(user_mainfw, O_RDONLY, 0);
    if (IS_ERR(fp))
    {
        fp = NULL;
    }

    return fp;
}

static void ssv6xxx_close_firmware(void *image)
{
    if (image)
    {
        filp_close((struct file *)image, NULL);
    }
}

static int _ssv6xxx_sdio_load_firmware(struct device *child, u8 *firmware_name, bool openfile)
{
    int ret = 0;
    u8   *fw_buffer = NULL;
    void *fw_fp = NULL;
    void *fw_fp_h = (1 == ssv_cfg.firmware_choice)?(void *)ssv6x5x_sw_bin:(void *)ssv6x5x_sw_2_bin;
    u32   checksum = FW_CHECKSUM_INIT;
    u32   retry_count = 1;
    int fw_start_offset = 0, fw_pos = 0;
    int fw_block_idx = 0;
    bool write_sram_err = false;
    u8 *pt=NULL;
    int fw_len = (int) ssv6x5x_sw_bin_len; //To avoid compile error.
    fw_len++;

    // Load firmware
    if(openfile)
    {
        fw_fp = ssv6xxx_open_firmware(firmware_name);
        if (!fw_fp) {
            SSV_LOG_DBG("failed to find firmware (%s)\n", firmware_name);
            ret = -1;
            goto out;
        }
    }
    else
    {
        fw_fp = (void *)fw_fp_h;
    }

    fw_start_offset = ssv6xxx_get_firmware_info(fw_fp, openfile);
    if (fw_start_offset < 0) {
        SSV_LOG_DBG("Failed to get firmware information\n");
        ret = -1;
        goto out;
    }

    // Allocate buffer firmware aligned with FW_BLOCK_SIZE and padding with 0xA5 in empty space.
    fw_buffer = (u8 *)kzalloc(FW_BLOCK_SIZE, GFP_KERNEL);
    if (fw_buffer == NULL) {
        SSV_LOG_DBG("Failed to allocate buffer for firmware.\n");
        ret = -1;
        goto out;
    }

    do {
        // initial value
#ifdef HOST_CALCULATE_CHECKSUM
        u32 fw_checksum = FW_CHECKSUM_INIT;
#endif
        checksum = FW_CHECKSUM_INIT;
        for (fw_block_idx = 0; fw_block_idx < fw_info.block_num; fw_block_idx++)
        {
            // calculate the fw block position
            if (fw_block_idx == 0)
                fw_pos += fw_start_offset;
            else
                fw_pos += fw_info.block[fw_block_idx-1].len;

            if (openfile)
                ((struct file *)fw_fp)->f_pos = fw_pos;
            else {
                fw_fp = (void *)fw_fp_h;
                pt=(u8 *)(fw_fp);
                fw_fp = (void *)(pt+fw_pos);
            }

            //SSV_LOG_DBG(_DBG("\nblock %d ssv6xxx_write_firmware_to_sram addr = 0x%x, len = %d\n", fw_block_idx, fw_info.block[fw_block_idx].start_addr, fw_info.block[fw_block_idx].len);
            checksum += ssv6xxx_write_firmware_to_sram(child, fw_buffer, fw_fp, fw_info.block[fw_block_idx].len, fw_info.block[fw_block_idx].start_addr, openfile, ((fw_block_idx == 1) ? false : true), &write_sram_err);

            if (write_sram_err) {
                SSV_LOG_DBG("Firmware \"%s\" fail to write sram.\n", firmware_name);
                ret = -1;
                goto out;
            }

#ifdef HOST_CALCULATE_CHECKSUM
            {  //calculate checksum on driver
                int j;
                uint32_t sram_addr = fw_info.block[fw_block_idx].start_addr;
                uint32_t sram_size = ((fw_info.block[fw_block_idx].len-1)/CHECKSUM_BLOCK_SIZE)*CHECKSUM_BLOCK_SIZE+CHECKSUM_BLOCK_SIZE;
                //SSV_LOG_DBG("idx = %d/%d,  fw_len = 0x%x\n", fw_block_idx, fw_info.block_num, fw_info.block[fw_block_idx].len);
                //SSV_LOG_DBG("sram_size = 0x%x\n", sram_size);
                if(fw_block_idx != 1)
                {
                    for(j=0;j<sram_size;)
                    {
                        uint32_t value;
                        ssv6xxx_sdio_read_reg(child, sram_addr, &value);
                        fw_checksum = fw_checksum + value;
                        sram_addr+=4;
                        j+=4;
                    }
                }
                if(fw_checksum != checksum)
                {
                    ret = -1;
                    SSV_LOG_DBG("fail! checksum not match(0x%x <> 0x%x)\n", checksum, fw_checksum);
                    goto out;
                }
            }
#endif

        }
        SSV_LOG_DBG("checksum = 0x%x\n", checksum);
        ret = 0;

        if (0 == ret) // firmware check ok
	        break;

    }
    while (--retry_count);

out:
    if (fw_buffer != NULL) {
        kfree(fw_buffer);
    }

    if (openfile) {
        if (fw_fp) {
            ssv6xxx_close_firmware(fw_fp);
        }
    }

    return ret;
}

extern char *cfgfirmwarepath;
static int ssv6xxx_sdio_load_firmware_openfile(struct device *child)
{
    u8 firmware_name[SSV_FIRMWARE_PATH_MAX] = {0};
    struct file *fp = NULL;
    bool openfile = true;

    //Populate full firmware name(path+name).
    if (NULL != cfgfirmwarepath)
    {
        snprintf(firmware_name, SSV_FIRMWARE_PATH_MAX, "%s%s", cfgfirmwarepath,
                 ssv_cfg.firmware_name);
    }
    else if (0x00 != ssv_cfg.firmware_path[0])
    {
        snprintf(firmware_name, SSV_FIRMWARE_PATH_MAX, "%s%s",
                 ssv_cfg.firmware_path, ssv_cfg.firmware_name);
    }
    else
    {
        //It should not be entered here.
        SSV_LOG_DBG("[%s][%d] error!!\n", __FUNCTION__, __LINE__);
    }

    SSV_LOG_DBG("Using firmware at %s\n", firmware_name);
    //Check firmware exist.
    fp = filp_open(firmware_name, O_RDONLY, 0);
    if (IS_ERR(fp))
    {
        SSV_LOG_DBG("\033[0;31mFirmware not exist, change to load inside firmware.\033[0m\n");
        openfile = false;
    }
    else
    {
        // SSV_LOG_DBG("Firmware exist...\n");
        filp_close((struct file *)fp, NULL);
    }

    return _ssv6xxx_sdio_load_firmware(child, firmware_name, openfile);
}


static int ssv6xxx_sdio_load_firmware_fromheader(struct device *child)
{
    return _ssv6xxx_sdio_load_firmware(child, NULL, false);
}

static int ssv6xxx_sdio_force_load_fw_fromeheader(struct device *child)
{
    int ret = 0;

#ifndef HWIF_DIS_FW_DOWNLOAD
    //Jump to ROM before load firmware.
    ssv6xxx_sdio_write_reg(child, 0x08c00004, 0x80000); //ROM code address 0x80000
    //Disable CPU, Support after r4p0
    //Stall C0
    ssv6xxx_sdio_write_reg(child, 0x08c00014, 0xff010100);
	//turn-on necessary memories
    ssv6xxx_sdio_write_reg(child, 0x08d02050, 0xcfc3);
    mdelay(500);

    //load fw before platform initialize
	//power on all sram
    ssv6xxx_sdio_write_reg(child, 0x08d02050, 0xfffff);
    ssv6xxx_sdio_write_reg(child, 0x08d02054, 0x3fff);
#endif

    ssv6xxx_sdio_load_fw_pre_config_hwif(child);

    ret = ssv6xxx_sdio_load_firmware_fromheader(child);

    if(ret)
        return ret;
    
    ssv6xxx_sdio_load_fw_post_config_hwif(child);
#ifndef HWIF_DIS_FW_DOWNLOAD
	//set IVB
    ssv6xxx_sdio_write_reg(child, 0x08c00004, 0x1000000);
	//mask sdio before reset S0
    ssv6xxx_sdio_write_reg(child, 0x08c00134, 0x100);
    //S0_RST(0x08c00014), b[0]: RST_H, 1:reset S0 AHB
	//reset S0
    ssv6xxx_sdio_write_reg(child, 0x08c00014, 0xff000101);
#ifdef HWIF_SET_PADMUX
    //Before r4p0 need this.
    ssv6xxx_sdio_write_reg(child, 0x08c00800, 0x7f80);
    ssv6xxx_sdio_write_reg(child, 0x08c00804, 0x30);
    ssv6xxx_sdio_write_reg(child, 0x08c00860, 0x31);
    ssv6xxx_sdio_write_reg(child, 0x80000, 0xd500d5);
#endif
    mdelay(500); //Wait 500ms for firmware ready.
#endif

    return ret;
}



static int ssv6xxx_sdio_load_each_fw(struct device *child)
{
    int ret = 0;

#ifndef HWIF_DIS_FW_DOWNLOAD
    //Jump to ROM before load firmware.
    ssv6xxx_sdio_write_reg(child, 0x08c00004, 0x80000); //ROM code address 0x80000
    //Disable CPU, Support after r4p0
    //Stall C0
    ssv6xxx_sdio_write_reg(child, 0x08c00014, 0xff010100);
       //turn-on necessary memories
    ssv6xxx_sdio_write_reg(child, 0x08d02050, 0xcfc3);


    //load fw before platform initialize
       //power on all sram
    ssv6xxx_sdio_write_reg(child, 0x08d02050, 0xfffff);
    ssv6xxx_sdio_write_reg(child, 0x08d02054, 0x3fff);
#endif

    ssv6xxx_sdio_load_fw_pre_config_hwif(child);

    if((NULL != cfgfirmwarepath) || (0x00 != ssv_cfg.firmware_path[0]))
    {
        ret = ssv6xxx_sdio_load_firmware_openfile(child);
    }
    else
    {
        ret = ssv6xxx_sdio_load_firmware_fromheader(child);
    }

    if(ret)
        return ret;
    
    ssv6xxx_sdio_load_fw_post_config_hwif(child);
#ifndef HWIF_DIS_FW_DOWNLOAD
	//set IVB
    ssv6xxx_sdio_write_reg(child, 0x08c00004, 0x1000000);
	//mask sdio before reset S0
    ssv6xxx_sdio_write_reg(child, 0x08c00134, 0x1100);
    //S0_RST(0x08c00014), b[0]: RST_H, 1:reset S0 AHB
	//reset S0
    ssv6xxx_sdio_write_reg(child, 0x08c00014, 0xff000101);
#ifdef HWIF_SET_PADMUX
    //Before r4p0 need this.
    ssv6xxx_sdio_write_reg(child, 0x08c00800, 0x7f80);
    ssv6xxx_sdio_write_reg(child, 0x08c00804, 0x30);
    ssv6xxx_sdio_write_reg(child, 0x08c00860, 0x31);
    ssv6xxx_sdio_write_reg(child, 0x80000, 0xd500d5);
#endif
    mdelay(500); //Wait 500ms for firmware ready.
#endif

    return ret;
}

int ssv6xxx_sdio_configure_ipc_mem(struct device *child);
static int ssv6xxx_sdio_load_fw(struct device *child)
{
    int ret;
    int fw_temp = ssv_cfg.firmware_choice;

    //shrink code size: 
    //pre load engineer fw to setup rf_phy table and calibaration

    ssv6xxx_sdio_configure_ipc_mem(child);
        
    ssv_cfg.firmware_choice = 2;
    //load engineer fw first
    ret = ssv6xxx_sdio_force_load_fw_fromeheader(child);
    //ssv6xxx_sdio_save_calibr_result(child);
    ssv_cfg.firmware_choice = fw_temp;
    if (ret) 
        return ret;

    //load SMAC FW
    ret = ssv6xxx_sdio_load_each_fw(child);
    // if (!ret) 
    //     ssv6xxx_sdio_restore_calibration(child);

    return ret;

}
#endif

////load firmware function ---

static int ssv6xxx_sdio_irq_getstatus(struct device *child,int *status)
{
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue;

    struct sdio_func *func;
    glue = dev_get_drvdata(child);

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
    SSV_LOG_DBG("\n-----------------------------\n");
    SSV_LOG_DBG("hexdump(len=%lu):\n", (unsigned long) len);
    {
        for (i = 0; i < len; i++){
            
            SSV_LOG_DBG(" %02x", buf[i]);
            if((i+1)%40 ==0)
                SSV_LOG_DBG("\n");
        }
    }
    SSV_LOG_DBG("\n-----------------------------\n");
}
#endif

static size_t _ssv6xxx_sdio_get_readsz(struct device *child)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func = NULL;
    size_t size = 0;
    int ret = -1;
#ifdef SDIO_CMD52_GET_RX_SIZE
    u8 low_byte = 0;
    u8 high_byte = 0;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);

    low_byte = sdio_readb(func, 0xbc, &ret);
    CHECK_IO_RET(glue, ret);
    high_byte = sdio_readb(func, 0xbd, &ret);
    CHECK_IO_RET(glue, ret);

    sdio_release_host(func);

    size = (size_t)((((u32)high_byte) << 8) | ((u32)low_byte));
#else
    //u32 addr = SD_REG_BASE+REG_CARD_PKT_LEN_0;
    u32 addr = 0x08C100AC;
    u32 buf = 0;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);
    
    ret = ssv6xxx_sdio_read_reg(child, addr, &buf);
    if (ret) {
        dev_err(child, "sdio read len failed ret[%d]\n",ret);
        size = 0;
    } else {
        size = (size_t)(buf&0xffff);
    }

    sdio_release_host(func);
#endif
    return size;
}

static size_t ssv6xxx_sdio_get_aggr_readsz(struct device *child)
{
    return _ssv6xxx_sdio_get_readsz(child);
}

static int ssv6xxx_sdio_read(struct device *child,
        void *buf, size_t *size)
{
    int ret = (-1), readsize = 0;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func ;

    if (IS_GLUE_INVALID(glue))
		return ret;

    func = dev_to_sdio_func(glue->dev);
    sdio_claim_host(func);

    readsize = sdio_align_size(func, *size);
	ret = sdio_memcpy_fromio(func, buf, glue->dataIOPort, readsize);
    if (ret)
        dev_err(child, "%s(): sdio read failed size ret[%d]\n", __func__, ret);

    sdio_release_host(func);
    CHECK_IO_RET(glue, ret);

#if 0
    if(*size > 1500)
        _sdio_hexdump(buf,*size);
#endif

    return ret;
}

static int ssv6xxx_sdio_write(struct device *child, void *buf, size_t len,u8 queue_num)
{
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func;
    int writesize;
    void *tempPointer;
    struct sk_buff *skb = (struct sk_buff *)buf;
    size_t txlen, remaining = len;

    (glue->tx_pkt)++;

    if (IS_GLUE_INVALID(glue))
		return ret;

    if ( glue != NULL )
    {
        func = dev_to_sdio_func(glue->dev);
        sdio_claim_host(func);
        while (remaining) {
            if (((unsigned long)skb->data) & (PLATFORM_DEF_DMA_ALIGN_SIZE - 1)) {
                SSV_LOG_DBG("SDIO Write: unalignmen!!!!!\n");
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
            {
                tempPointer = skb->data;
                txlen = remaining;
                remaining = 0;
            }
#if 0
            // if(len > 1500)
                _sdio_hexdump(skb->data,len);
#endif
#if 0
            SSV_LOG_DBG("[%s][%d] len = %d\n", __FUNCTION__, __LINE__, (int)len);
            ssv_hex_dump(skb->data, len);
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

    if (IS_GLUE_INVALID(glue))
        return;

    if(glue->irq_handler != NULL)
    {
        atomic_set(&glue->irq_handling, 1);
        sdio_release_host(func);
        status = glue->irq_handler(0, glue);
        sdio_claim_host(func);
        atomic_set(&glue->irq_handling, 0);
    }
}

static void ssv6xxx_sdio_irq_setmask(struct device *child,int mask)
{
    int err_ret;    
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
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

#if 1
static void ssv6xxx_sdio_irq_trigger(struct device *child)
{
    int err_ret;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func;

    SSV_LOG_DBG("ssv6xxx_sdio_irq_trigger\n");

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
#endif

#if 0
static int ssv6xxx_sdio_irq_getmask(struct device *child, u32 *mask)
{
    u8 imask = 0;
    int ret = (-1);
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
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
#endif

static void ssv6xxx_sdio_irq_enable(struct device *child)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func;
    int ret;

    SSV_LOG_DBG("ssv6xxx_sdio_irq_enable\n");

    if (IS_GLUE_INVALID(glue))
        return;

    func = dev_to_sdio_func(glue->dev);

    sdio_claim_host(func);

    /* Register the isr */
    ret =  sdio_claim_irq(func, ssv6xxx_sdio_irq_handler);
    if (ret)
        dev_err(&func->dev, "Failed to claim sdio irq: %d\n", ret);

    sdio_release_host(func);
    CHECK_IO_RET(glue, ret);
}

static void ssv6xxx_sdio_irq_disable(struct device *child, bool iswaitirq)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func;
    int ret;

    //WARN_ON(true);
    if (IS_GLUE_INVALID(glue))
		return;

    SSV_LOG_DBG("ssv6xxx_sdio_irq_disable\n");
    
    func = dev_to_sdio_func(glue->dev);
    if (func == NULL) {
        SSV_LOG_DBG("func == NULL\n");
        return;
    }
        
    sdio_claim_host(func);
        
    while (atomic_read(&glue->irq_handling)) {
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
	u32 frame_size = MAX_HCI_RX_AGGR_SIZE;
    
	for (rx_cnt = 0 ; status & SSV6XXX_INT_RX ; rx_cnt++)
	{
		if (glue->is_rx_q_full && glue->is_rx_q_full(glue->rx_cb_args)) {
			goto unmask;
		}
		
		dlen = ssv6xxx_sdio_get_aggr_readsz(glue->dev);
        if ((dlen == 0) || (dlen > frame_size)) {
			SSV_LOG_DBG("%s(): dlen = %d, goto skip.\n", __func__, (int)dlen);
			goto unmask;
		}
	    
        sdio_claim_host(func);
        readsize = sdio_align_size(func, dlen);
        sdio_release_host(func);
        rx_mpdu = ssv_rx_skb_alloc(readsize, GFP_KERNEL);
        if (rx_mpdu == NULL) {
			SSV_LOG_DBG("%s(): Can't alloc skb.\n", __func__);
			goto unmask;
		}
		
        ret = ssv6xxx_sdio_read(glue->dev, rx_mpdu->data, &dlen);
		if (ret < 0) {
			SSV_LOG_DBG("%s(): Fail to sdio read %d\n", __FUNCTION__, ret);
            
            ssv_rx_skb_free(rx_mpdu);
            goto unmask;
		}
		
        skb_put(rx_mpdu, dlen);
        (glue->rx_pkt)++;

#if 0
        SSV_LOG_DBG("[%s][%d] len = %d\n", __FUNCTION__, __LINE__, (int)rx_mpdu->len);
        ssv_hex_dump(rx_mpdu->data, rx_mpdu->len);
#endif
        if (glue->rx_cb)
		    glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        else
            ssv_rx_skb_free(rx_mpdu);
        
        if (0 != glue->recv_cnt) {
            if (rx_cnt > glue->recv_cnt)
                break;
        }

        ssv6xxx_sdio_irq_getstatus(glue->dev, &status);
	}

unmask:
	ssv6xxx_sdio_irq_setmask(glue->dev, 0xff & ~SSV6XXX_INT_RX);
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
    u32    frame_size = MAX_HCI_RX_AGGR_SIZE;
    
    for (rx_cnt = 0 ; status & SSV6XXX_INT_RX ; rx_cnt++)
    {
        if (glue->is_rx_q_full && glue->is_rx_q_full(glue->rx_cb_args)) {
            //SSV_LOG_DBG("%s(): RX queue is full.\n", __func__);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            goto skip;
        }

        dlen = ssv6xxx_sdio_get_aggr_readsz(glue->dev);
        if ((dlen == 0) || (dlen > frame_size)) {
            SSV_LOG_DBG("%s(): dlen = %d, goto skip.\n", __func__, (int)dlen);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            goto skip;
        }

        sdio_claim_host(func);
        readsize = sdio_align_size(func, dlen);
        sdio_release_host(func);
        rx_mpdu = ssv_rx_skb_alloc(readsize, GFP_KERNEL);
        if (rx_mpdu == NULL) {
            SSV_LOG_DBG("%s(): Can't alloc skb.\n", __func__);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            goto skip;
        }
       
        ret = ssv6xxx_sdio_read(glue->dev, rx_mpdu->data, &dlen);
        if (ret < 0)
        {
			SSV_LOG_DBG("%s(): Fail to sdio read %d\n", __FUNCTION__, ret);
            ssv_rx_skb_free(rx_mpdu);
            goto unmask;
        }

        skb_put(rx_mpdu, dlen);
        (glue->rx_pkt)++;
        
        if (glue->rx_cb)
            glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        else
            ssv_rx_skb_free(rx_mpdu);
            
        if (0 != glue->recv_cnt) {
            if (rx_cnt > glue->recv_cnt)
                break;
        }

        ssv6xxx_sdio_irq_getstatus(glue->dev, &status);
    }

unmask:
    ssv6xxx_sdio_irq_setmask(glue->dev, 0xff & ~SSV6XXX_INT_RX);
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

    ssv6xxx_sdio_irq_getstatus(glue->dev, &status);

    if (status & SSV6XXX_INT_RX) {
        ssv6xxx_sdio_irq_setmask(glue->dev, 0xff);

#ifdef HWIF_SDIO_RX_IRQ
		ssv6xxx_sdio_recv_rx_func(glue);
#else //HWIF_SDIO_RX_IRQ
        // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
        //if (ssv_rx_use_wq) {
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
        //} else {
        //    tasklet_schedule(&glue->rx_tasklet);
        //}
#endif //HWIF_SDIO_RX_IRQ
        
        (glue->rx_isr_cnt)++;
        return IRQ_HANDLED;
    } else {
        return IRQ_NONE;
    }
}

static void ssv6xxx_sdio_irq_request(struct device *child, irq_handler_t irq_handler, void *irq_dev)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);
    struct sdio_func *func;
    bool isIrqEn = true;

    if (IS_GLUE_INVALID(glue))
		return;

    func = dev_to_sdio_func(glue->dev);

    glue->irq_handler = irq_handler;

    if (isIrqEn)
    {
        //add trigger
        ssv6xxx_sdio_irq_trigger(child);
        ssv6xxx_sdio_irq_setmask(glue->dev, 0xff & ~SSV6XXX_INT_RX);
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
        SSV_LOG_DBG("SDIO setting SDIO_DEF_BLOCK_SIZE fail!!\n");
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

/*
static void ssv6xxx_set_bus_width(struct sdio_func *func, u32 bus_width)
{
    struct mmc_host *host;
    u32 val = 0;
    u32 ret = 0;
    host = func->card->host;
    SSV_LOG_DBG("%s: set bus width %d\n", __FUNCTION__, bus_width);
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
        SSV_LOG_DBG("%s: set bus width %d, val = %x\n", __FUNCTION__, bus_width, val);
    }
    if (ret == 0)
    {
        host->ios.bus_width = bus_width;
        host->ops->set_ios(host, &host->ios);
    }
    else
    {
        SSV_LOG_DBG("%s: set bus width %d err\n", __FUNCTION__, bus_width);
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

	SSV_LOG_DBG("%s: set sdio clk %dHz\n", __FUNCTION__, sdio_hz);
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

static void ssv6xxx_sdio_rx_task(struct device *child, 
			int (*rx_cb)(struct sk_buff *rx_skb, void *args), 
			int (*is_rx_q_full)(void *args), void *args, u32 recv_cnt)
{
	struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);

	glue->rx_cb = rx_cb;
	glue->rx_cb_args = args;
	glue->is_rx_q_full = is_rx_q_full;
    glue->recv_cnt = recv_cnt;

    ssv6xxx_sdio_irq_setmask(glue->dev, 0xff);
    ssv6xxx_sdio_irq_disable(glue->dev, false);

    ssv6xxx_sdio_irq_request(glue->dev, ssv6xxx_sdio_isr, NULL);
}

static void ssv6xxx_sdio_tx_req_cnt(struct device *dev, int *cnt)
{
    *cnt = 0;
    return;
}

static void ssv6xxx_sdio_tx_st(struct device *child, u32 *pkt_cnt)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    *pkt_cnt = glue->tx_pkt;
}

static void ssv6xxx_sdio_rx_st(struct device *child, u32 *pkt_cnt)
{
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    *pkt_cnt = glue->rx_pkt;
}

static void ssv6xxx_sdio_fw_reset(struct device *child)
{
    int ret = -ENOMEM;
    struct ssv6xxx_sdio_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }

#ifndef HWIF_DIS_FW_DOWNLOAD
    //load fw before platform initialize
    ret = ssv6xxx_sdio_load_fw(glue->dev);
    if (ret) 
    {
        SSV_LOG_DBG("Fail to load firmware\n");
        ret = -ENOMEM;
    }
#else
    ssv6xxx_sdio_load_fw_post_config_hwif(glue->dev);
#endif

    // HCI_TX_PAGE_THRESHOLD_INT_FUNC(0x08c10088)
    // b[7:0]:   TX_PAGE_LOW_THRESHOLD
    // b[15:8]:  TX_PAGE_HIGH_THRESHOLD
    // b[25:16]: TX_PAGE_OVER_THRESHOLD_EVENT_CNT
    // b[30:26]: reseved
    // b[31:     TX_PAGE_THRESHOLD_INT_EN
    //ssv6xxx_sdio_write_reg(glue->dev, 0x08c10088, 0x8000280c);

    //Set HCI path
    {
        #define CO_BIT_MASK(pos)                                (1UL<<(pos))
        #define REG_HCI_BASE_ADDRESS                            (0x08C10000)
        #define REG_HCI_CONTROL_REG_ADDRESS                     (REG_HCI_BASE_ADDRESS + 0x08)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS    REG_HCI_CONTROL_REG_ADDRESS
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT      (1)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK       CO_BIT_MASK(REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_sdio_read_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, &tmp_regval);
        tmp_regval &= ~REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK; //Set 0 for SDIO.
        ssv6xxx_sdio_write_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, tmp_regval);
    }
    {
        //set HCI TX I/O aggr
        //#define REG_HCI_BASE_ADDRESS                                                    (0x08C10000)
        //#define REG_HCI_CONTROL_REG_ADDRESS                                             (REG_HCI_BASE_ADDRESS + 0x08) 
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_ADDRESS                           REG_HCI_CONTROL_REG_ADDRESS
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT                             (0)
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_MASK                              CO_BIT_MASK(REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_sdio_read_reg(glue->dev, 0x08C10008, &tmp_regval);
        if(ssv_cfg.hw_caps & HW_CAP_HCI_TX_AGGR) {
            tmp_regval |= 0x01;
        }
        else {
            tmp_regval &= ~0x01;
        }
        ssv6xxx_sdio_write_reg(glue->dev, 0x08C10008, tmp_regval);
    }    
}

#ifdef SSV_PERFORMANCE_WATCH
static void ssv6xxx_sdio_get_info(struct device *child, struct ssv6xxx_hwif_info *info)
{
}

static void ssv6xxx_sdio_clr_info(struct device *child)
{
}
#endif
static struct ssv6xxx_hwif_ops sdio_ops =
{
    .read                  		= ssv6xxx_sdio_read,
    .write                 		= ssv6xxx_sdio_write,
    .readreg               		= ssv6xxx_sdio_read_reg,
    .writereg              		= ssv6xxx_sdio_write_reg,
    .hwif_rx_task               = ssv6xxx_sdio_rx_task,
    .get_tx_req_cnt             = ssv6xxx_sdio_tx_req_cnt,

#if (HWIF_SUPPORT == 2)
    .cmd52_read            		= ssv6xxx_sdio_cmd52_read,
    .cmd52_write           		= ssv6xxx_sdio_cmd52_write,
#endif
//    .irq_trigger           		= ssv6xxx_sdio_irq_trigger,
    .tx_st                      = ssv6xxx_sdio_tx_st,
    .rx_st                      = ssv6xxx_sdio_rx_st,
    .fw_reset           = ssv6xxx_sdio_fw_reset,
#ifdef SSV_PERFORMANCE_WATCH
    .get_info = ssv6xxx_sdio_get_info,
    .clr_info = ssv6xxx_sdio_clr_info,
#endif
};


#ifdef CONFIG_PCIEASPM
#include <linux/pci.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26) && LINUX_VERSION_CODE < KERNEL_VERSION(5,4,0)
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

int ssv6xxx_sdio_configure_ipc_mem(struct device *child)
{
    return ssv_configure_ipc_mem(child, &sdio_ops);
}

static void ssv6xxx_sdio_power_on(struct ssv6xxx_sdio_glue *glue, struct sdio_func *func)
{
	
	int ret = 0;
    
    if (glue->is_enabled == true)
        return;

    SSV_LOG_DBG("ssv6xxx_sdio_power_on\n");

	sdio_claim_host(func);
	ret = sdio_enable_func(func);
	sdio_release_host(func);
	
	
	if (ret) {
		SSV_LOG_DBG("Unable to enable sdio func: %d)\n", ret);		
	}
	
	/*
	 * Wait for hardware to initialise. It should take a lot less than
	 * 10 ms but let's be conservative here.
	 */
	msleep(10);
    glue->is_enabled = true;
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
        SSV_LOG_DBG("%s(): The 1st CMD5 failed.", __func__);
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
        SSV_LOG_DBG("%s(): The 2nd CMD5 failed.", __func__);
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
        SSV_LOG_DBG("%s(): CMD3 failed.", __func__);
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
        SSV_LOG_DBG("%s(): CMD7 failed.", __func__);
        return -1;
    }

    return 0;
}

static void ssv6xxx_do_sdio_reset_reinit(struct sdio_func *func, struct ssv6xxx_sdio_glue *glue)
{
    int err_ret;
    struct mmc_host *host;

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid.\n", __func__);
        return;
    }

#ifndef HWIF_DIS_FW_DOWNLOAD
    // Do reset by sdio cccr06 bit3
    sdio_claim_host(func);
    sdio_f0_writeb(func, 0x08, SDIO_CCCR_ABORT, &err_ret);
    sdio_release_host(func);
    CHECK_IO_RET(glue, err_ret);
#endif

    // Do 5-5-3-7
    err_ret = ssv6xxx_do_sdio_init_seq_5537(func);
    CHECK_IO_RET(glue, err_ret);

    // Set host bus width
    sdio_claim_host(func);
    host = func->card->host;
#ifdef SDIO_USE_ONE_BIT
    host->ios.bus_width = MMC_BUS_WIDTH_1;
#else
    host->ios.bus_width = MMC_BUS_WIDTH_4;
#endif
    host->ops->set_ios(host, &host->ios);
    mdelay(20);
    sdio_release_host(func);

    // Set card bus width
    sdio_claim_host(func);
#ifdef SDIO_USE_ONE_BIT
    sdio_f0_writeb(func, SDIO_BUS_WIDTH_1BIT, SDIO_CCCR_IF, &err_ret);
#else
    sdio_f0_writeb(func, SDIO_BUS_WIDTH_4BIT, SDIO_CCCR_IF, &err_ret);
#endif
    sdio_release_host(func);
    CHECK_IO_RET(glue, err_ret);

    // Enable func #1 and set block size
    ssv6xxx_sdio_power_on(glue, func);
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

static void ssv6xxx_sdio_power_off(struct ssv6xxx_sdio_glue *glue, struct sdio_func *func)
{
    if (glue->is_enabled == false)
        return;

    SSV_LOG_DBG("ssv6xxx_sdio_power_off\n");
	/* Disable the card */
	sdio_claim_host(func);
	sdio_disable_func(func);
	sdio_release_host(func);

    glue->is_enabled = false;
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

    SSV_LOG_DBG("%s: delay chain data0[%02x], data1[%02x], data2[%02x], data3[%02x]\n", 
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

static int ssv6xxx_sdio_set_xtal(struct device *dev, u32 xtal)
{
    //(16M/24M/26M/40M/12M/20M/25M)
    
    u32 value = 0;
    ssv6xxx_sdio_read_reg(dev, 0x08d01014, &value);
    value = value&0xffffeff0;
    
    switch(xtal)
    {  
        case 24:
            value |= 1; 
            break;
        case 25:
            value |= 6;
            value |= 0x1000;
            break;
        case 26:
            value |= 2; 
            value |= 0x1000;
            break;
        case 40:
            value |= 3;
            break;
        default:
            value |= 2; 
            value |= 0x1000;
            break;
    }

    ssv6xxx_sdio_write_reg(dev, 0x08d01014, value);
    return 0;
}

extern int ssv6xxx_platform_init(struct device *dev, struct ssv6xxx_hwif_ops *hwif_ops, void **plat_hw);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static int __devinit ssv6xxx_sdio_probe(struct sdio_func *func,
        const struct sdio_device_id *id)
#else
static int ssv6xxx_sdio_probe(struct sdio_func *func,
        const struct sdio_device_id *id)
#endif
{
    struct ssv6xxx_sdio_glue     *glue;
    void *plat_hw = NULL;
    int ret = -ENOMEM;

    SSV_LOG_INFO("=======================================\n");
    SSV_LOG_INFO("==           RUN SDIO                ==\n");
    SSV_LOG_INFO("=======================================\n");
    
    /* We are only able to handle the wlan function */
    if (func->num != 0x01) {
        ret = -ENODEV;
        goto out;
    }

    SSV_LOG_DBG("vendor = 0x%x device = 0x%x\n", func->vendor, func->device);
    SSV_LOG_DBG("max block count: %u\n", min(func->card->host->max_blk_count, 511u));
    glue = kzalloc(sizeof(*glue), GFP_KERNEL);
    if (!glue)
    {
        dev_err(&func->dev, "can't allocate glue\n");
        ret = -ENOMEM;
        goto out;
    }

	/* INIT RX */
	//if (ssv_rx_use_wq) {
		glue->rx_work.glue = glue;
		INIT_WORK((struct work_struct *)&glue->rx_work, ssv6xxx_sdio_recv_rx_work);
		glue->wq = create_singlethread_workqueue("ssv6xxx_sdio_wq");
		if (!glue->wq) {
			dev_err(&func->dev, "Could not allocate Work Queue\n");
            ret = -ENOMEM;
            goto err_crate_wq;
		}
	//} else {
	//	tasklet_init(&glue->rx_tasklet, ssv6xxx_sdio_recv_rx_tasklet, (unsigned long)glue);
	//}

    /* 
     * Setting SDIO delay chain
     * Note: delay chain function cannot work in CABRIO 
     */
#if (defined(CONFIG_SSV_SDIO_INPUT_DELAY) && defined(CONFIG_SSV_SDIO_OUTPUT_DELAY))
    ssv6xxx_sdio_delay_chain(func, CONFIG_SSV_SDIO_INPUT_DELAY, CONFIG_SSV_SDIO_OUTPUT_DELAY);
#endif
    //Setting SDIO to 25M
	ssv6xxx_low_sdio_clk(func);

    atomic_set(&glue->irq_handling, 0);
    glue->dev = &func->dev;

    /* Grab access to FN0 for ELP reg. */
    func->card->quirks |= MMC_QUIRK_LENIENT_FN0;
    /* Use block mode for transferring over one block size of data */
    func->card->quirks |= MMC_QUIRK_BLKSZ_FOR_BYTE_MODE;

    #ifdef CONFIG_PCIEASPM
    cabrio_sdio_pm_check(func);
    #endif // CONFIG_PCIEASPM

    ssv6xxx_sdio_power_on(glue, func);
    ssv6xxx_sdio_read_parameter(func, glue);

	glue->dev_ready = true;
    //do system reset from sdio client   
    ssv6xxx_do_sdio_reset_reinit(func, glue);
    ssv6xxx_sdio_direct_int_mux_mode(glue, false);
    //ssv6xxx_set_bus_width(func, MMC_BUS_WIDTH_4);

    sdio_set_drvdata(func, glue);

#ifdef HWIF_TRIM_CODE

/* The trim code rule:
 * +-------------+
 * |Set | DLDO_LV|
 * +----+--------+
 * |000 | 011    |
 * |001 | 010    |
 * |010 | 001    |
 * |011 | 000    |
 * |100 | 111    |
 * |101 | 110    |
 * |110 | 101    |
 * |111 | 100    |
 * +-------------+
 */
#define REG_TRIM_ADDR               (0x8500000+4)
#define REG_PMU_DLDO_AND_DCDC_ADDR  (0x08d01010)
#define REG_DLDO_LV_OFS             (3)
#define REG_DLDO_LV_MSK             (0x38)
#define REG_TRIM_OFS                (29)
#define REG32_R(ADDR, val)          (ssv6xxx_sdio_read_reg(glue->dev, ADDR, val))
#define REG32_W(ADDR, val)          (ssv6xxx_sdio_write_reg(glue->dev, ADDR, val))
#define REG_TRIM_MSK                (0xE0000000)
    do {
        u32 _regval, _hw_val;
        REG32_R(REG_TRIM_ADDR, &_regval);
        REG32_R(REG_PMU_DLDO_AND_DCDC_ADDR, &_hw_val);
        _hw_val = ((_hw_val) & (~REG_DLDO_LV_MSK)) | ((((_regval >> REG_TRIM_OFS) & 0x7) ^ 0x3) << REG_DLDO_LV_OFS);
        //SSV_LOG_DBG("trim code write 0x%x\n", _hw_val);
        REG32_W(REG_PMU_DLDO_AND_DCDC_ADDR, _hw_val);
    } while(0);
#endif

    //pll
    { 
        u32 value;
        u32 count=0;
        ssv6xxx_sdio_write_reg(glue->dev, 0x08c00008, 0x01010000);
        
        ssv6xxx_sdio_set_xtal(glue->dev, ssv_cfg.xtal_clock);
        //SSV_LOG_DBG("xtal_clock = %d\n", ssv_cfg.xtal_clock);
        
        ssv6xxx_sdio_read_reg(glue->dev, 0x08d01004, &value);
        //SSV_LOG_DBG("addr(0x08d01004) = 0x%x\n", value);
        value |= 0x80000000;
        ssv6xxx_sdio_write_reg(glue->dev, 0x08d01004, value);
        //ssv6xxx_sdio_read_reg(glue->dev, 0x08d01004, &value);
        //SSV_LOG_DBG("addr(0x08d01004) = 0x%x\n", value);

        ssv6xxx_sdio_read_reg(glue->dev, 0x08d0208c, &value);
        while( ( (value&0x40000000) >>30) != 1)  
        {
            count++;
            if(count >= 100)
            {  
                SSV_LOG_DBG("wait... 0x%x\n", value);
                break;
            }
            ssv6xxx_sdio_read_reg(glue->dev, 0x08d0208c, &value);
        }

        ssv6xxx_sdio_read_reg(glue->dev, 0x08d01018, &value);
        //SSV_LOG_DBG("addr(0x08d01018) = 0x%x\n", value);
        value |= 0x10000;
        ssv6xxx_sdio_write_reg(glue->dev, 0x08d01018, value);        
        
        ssv6xxx_sdio_read_reg(glue->dev, 0x08c00010, &value);
        //SSV_LOG_DBG("addr(0x08c00010) = 0x%x\n", value);
        value = 1;
        ssv6xxx_sdio_write_reg(glue->dev, 0x08c00010, value);         

        ssv6xxx_sdio_write_reg(glue->dev, 0x08c00008, 0x00010101);
    }

#ifndef HWIF_DIS_FW_DOWNLOAD
    //load fw before platform initialize

    ret = ssv6xxx_sdio_load_fw(glue->dev);
    if (ret) 
    {
        SSV_LOG_DBG("Fail to load firmware\n");
        ret = -ENOMEM;
        goto err_crate_wq;
    }
#else
    ssv6xxx_sdio_load_fw_post_config_hwif(glue->dev);
#endif

    // HCI_TX_PAGE_THRESHOLD_INT_FUNC(0x08c10088)
    // b[7:0]:   TX_PAGE_LOW_THRESHOLD
    // b[15:8]:  TX_PAGE_HIGH_THRESHOLD
    // b[25:16]: TX_PAGE_OVER_THRESHOLD_EVENT_CNT
    // b[30:26]: reseved
    // b[31:     TX_PAGE_THRESHOLD_INT_EN
    ssv6xxx_sdio_write_reg(glue->dev, 0x08c10088, 0x8000280c);

    //Set HCI path
    {
        #define CO_BIT_MASK(pos)                                (1UL<<(pos))
        #define REG_HCI_BASE_ADDRESS                            (0x08C10000)
        #define REG_HCI_CONTROL_REG_ADDRESS                     (REG_HCI_BASE_ADDRESS + 0x08)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS    REG_HCI_CONTROL_REG_ADDRESS
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT      (1)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK       CO_BIT_MASK(REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_sdio_read_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, &tmp_regval);
        tmp_regval &= ~REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK; //Set 0 for SDIO.
        ssv6xxx_sdio_write_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, tmp_regval);
    }

    {
        //set HCI TX I/O aggr
        //#define REG_HCI_BASE_ADDRESS                                                    (0x08C10000)
        //#define REG_HCI_CONTROL_REG_ADDRESS                                             (REG_HCI_BASE_ADDRESS + 0x08) 
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_ADDRESS                           REG_HCI_CONTROL_REG_ADDRESS
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT                             (0)
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_MASK                              CO_BIT_MASK(REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_sdio_read_reg(glue->dev, 0x08C10008, &tmp_regval);
        if(ssv_cfg.hw_caps & HW_CAP_HCI_TX_AGGR) {
            tmp_regval |= 0x01;
        }
        else {
            tmp_regval &= ~0x01;
        }
        ssv6xxx_sdio_write_reg(glue->dev, 0x08C10008, tmp_regval);
    }

    
    // mask rx interrupt
    ssv6xxx_sdio_irq_setmask(glue->dev,0xff);

    //add trigger
    ssv6xxx_sdio_irq_trigger(glue->dev);
	
    ret = ssv6xxx_platform_init(&func->dev, &sdio_ops, &plat_hw);
    if (ret) {
        SSV_LOG_DBG("Fail to platform init\n");
        ret = -ENOMEM;
        goto err_plat_init;
    }
    glue->plat_hw = plat_hw;
    
    return 0;
    
err_plat_init:
    if (glue) {
        cancel_work_sync((struct work_struct *)&glue->rx_work);
	    destroy_workqueue(glue->wq);
    }
err_crate_wq:
    if (glue)
        kfree(glue);
out:
    return ret;
}

extern void ssv6xxx_platform_deinit(void *plat_hw);
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static void __devexit ssv6xxx_sdio_remove(struct sdio_func *func)
#else
static void ssv6xxx_sdio_remove(struct sdio_func *func)
#endif
{
    struct ssv6xxx_sdio_glue *glue = sdio_get_drvdata(func);

    SSV_LOG_DBG("ssv6xxx_sdio_remove..........\n");
    if ( glue )
    {
        // step 1, netstack deinit
        ssv6xxx_platform_deinit(glue->plat_hw);
        
        // step 2, sdio driver deinit
        // Remove IRQ handler once card is removed.
        SSV_LOG_DBG("ssv6xxx_sdio_remove - ssv6xxx_sdio_irq_disable\n");
        ssv6xxx_sdio_irq_disable(glue->dev, false);
        //Setting SDIO to 25M
		ssv6xxx_low_sdio_clk(func);
        SSV_LOG_DBG("ssv6xxx_sdio_remove - disable mask\n");
        ssv6xxx_sdio_irq_setmask(glue->dev, 0xff);

        ssv6xxx_sdio_power_off(glue, func);

	    //if (ssv_rx_use_wq) {
            cancel_work_sync((struct work_struct *)&glue->rx_work);
	    	destroy_workqueue(glue->wq);
	    //} else {
	    	//tasklet_kill(&glue->rx_tasklet);
	    //}

        glue->dev_ready = false;
        kfree(glue);
    }
    
    sdio_set_drvdata(func, NULL);

    SSV_LOG_DBG("ssv6xxx_sdio_remove leave..........\n");
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
    SSV_LOG_DBG("ssv6xxx_sdio_init\n");
    return sdio_register_driver(&ssv6xxx_sdio_driver);
}

void ssv6xxx_sdio_exit(void)
{
    SSV_LOG_DBG("ssv6xxx_sdio_exit\n");
    sdio_unregister_driver(&ssv6xxx_sdio_driver);
}
EXPORT_SYMBOL(ssv6xxx_sdio_init);
EXPORT_SYMBOL(ssv6xxx_sdio_exit);

MODULE_LICENSE("GPL");

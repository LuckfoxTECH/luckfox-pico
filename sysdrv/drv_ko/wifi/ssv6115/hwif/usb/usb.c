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

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/kref.h>
#include <linux/uaccess.h>
#include <linux/usb.h>
#include <linux/mutex.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#include <linux/netdevice.h>

#include "ssv_cfg.h"
#include "ssv_debug.h"

#include <hwif/hwif.h>
#include "usb.h"
#include "utils/ssv_alloc_skb.h"
// #include "rf_phy/6030B1/ssv_calibration.h"
// #include "hwif/common/common.h"

//include firmware binary header
#ifndef HWIF_DIS_FW_DOWNLOAD
#include "ssv6x5x-sw.h"
#include "ssv6x5x-sw_2.h"
#endif

/* Define these values to match devices */
#define USB_SSV_VENDOR_ID  0x8065
#define USB_SSV_PRODUCT_ID 0x6011

// #define MAX_USB_RX_AGGR_SIZE        (3072)
#define MAX_USB_RX_AGGR_SIZE        (20*1024) //HCI RX sync from FW max size.
#define TRANSACTION_TIMEOUT         (100) /* ms */
#define SSV6XXX_MAX_TXCMDSZ         (sizeof(struct ssv6xxx_cmd_hdr))
#define SSV6XXX_MAX_RXCMDSZ         (sizeof(struct ssv6xxx_cmd_hdr))
#define SSV6XXX_CMD_HEADER_SIZE     (sizeof(struct ssv6xxx_cmd_hdr) - sizeof(union ssv6xxx_payload))
#define USB_CMD_SEQUENCE            255
#define MAX_RETRY_SSV6XXX_ALLOC_BUF 3

#define IS_GLUE_INVALID(glue)  \
    (   (glue == NULL) \
        || (glue->dev_ready == false) \
    )

/* table of devices that work with this driver */
static const struct usb_device_id ssv_usb_table[] = {
    { USB_DEVICE(USB_SSV_VENDOR_ID, USB_SSV_PRODUCT_ID) },
    { }                 
};
MODULE_DEVICE_TABLE(usb, ssv_usb_table);

/* module param */
extern int ssv_usb_rx_nr_recvbuff;
extern int ssv_rx_use_wq;

extern struct ssv6xxx_cfg ssv_cfg;

#ifdef CONFIG_USB_TX_MULTI_URB
static atomic_t    tx_urb_cnt;
#endif
static bool begin_download_fw = 0;
/* Structure to hold all of our device specific stuff */
struct ssv6xxx_usb_glue {
    void                            *plat_hw;
    struct device                   *dev;
    struct usb_device               *udev;                      /* the usb device for this device */
    struct usb_interface            *interface;             /* the interface for this device */
    struct ssv6xxx_cmd_endpoint      cmd_endpoint;  /* command endpoint */
    struct ssv6xxx_cmd_endpoint      rsp_endpoint;  /* response endpoint */
    struct ssv6xxx_tx_endpoint       tx_endpoint;   /* tx endpoint */
    struct ssv6xxx_rx_endpoint      rx_endpoint;    /* rx endpoint */
    struct ssv6xxx_usb_rx_buf ssv_rx_buf[SSV_USB_MAX_NR_RECVBUFF];
    struct ssv6xxx_queue ssv_rx_queue;
    struct kref     kref;
    struct mutex        io_mutex;                   /* synchronize I/O with disconnect */
    struct mutex    cmd_mutex;                      /* blocking cmd/rsp */
    u16    sequence;
    u16    err_cnt;
    bool   dev_ready;
    struct workqueue_struct *wq;
    struct ssv6xxx_usb_work_struct rx_work;
    struct tasklet_struct rx_tasklet;
    u32 rx_pkt;
    void *rx_cb_args;
    int (*rx_cb)(struct sk_buff *rx_skb, void *args);
    int (*is_rx_q_full)(void *);
    u32 tx_pkt;
    unsigned long tx_run_data_time;
    u32 *ep0_buf;
};

static void ssv6xxx_usb_recv_rx(struct ssv6xxx_usb_glue *glue, struct ssv6xxx_usb_rx_buf *ssv_rx_buf);
static int ssv6xxx_usb_read_reg(struct device *child, u32 addr, u32 *buf);
static int ssv6xxx_usb_write_reg(struct device *child, u32 addr, u32 buf);
#define to_ssv6xxx_usb_dev(d) container_of(d, struct ssv6xxx_usb_glue, kref)

static struct usb_driver ssv_usb_driver;
#if 0
static void ssv6xxx_dump_tx_desc(const u8 *buf)
{
    struct ssv6200_tx_desc *tx_desc;

    tx_desc = (struct ssv6200_tx_desc *)buf;
    SSV_LOG_DBG(">> Tx Frame:\n");
    SSV_LOG_DBG("length: %d, c_type=%d, f80211=%d, qos=%d, ht=%d, use_4addr=%d, sec=%d\n", 
            tx_desc->len, tx_desc->c_type, tx_desc->f80211, tx_desc->qos, tx_desc->ht,
            tx_desc->use_4addr, tx_desc->security);
    SSV_LOG_DBG("more_data=%d, sub_type=%x, extra_info=%d\n", tx_desc->more_data,
            tx_desc->stype_b5b4, tx_desc->extra_info);
    SSV_LOG_DBG("fcmd=0x%08x, hdr_offset=%d, frag=%d, unicast=%d, hdr_len=%d\n",
            tx_desc->fCmd, tx_desc->hdr_offset, tx_desc->frag, tx_desc->unicast,
            tx_desc->hdr_len);
    SSV_LOG_DBG("tx_burst=%d, ack_policy=%d, do_rts_cts=%d, reason=%d, payload_offset=%d\n", 
            tx_desc->tx_burst, tx_desc->ack_policy, tx_desc->do_rts_cts, 
            tx_desc->reason, tx_desc->payload_offset);
    SSV_LOG_DBG("fcmdidx=%d, wsid=%d, txq_idx=%d\n",
            tx_desc->fCmdIdx, tx_desc->wsid, tx_desc->txq_idx);
    SSV_LOG_DBG("RTS/CTS Nav=%d, frame_time=%d, crate_idx=%d, drate_idx=%d, dl_len=%d\n",
            tx_desc->rts_cts_nav, tx_desc->frame_consume_time, tx_desc->crate_idx, tx_desc->drate_idx,
            tx_desc->dl_length);
    SSV_LOG_DBG("\n\n\n");
}
#endif

#if 0
static void ssv6xxx_dump_rx_desc(const u8 *buf)
{
    struct ssv6200_rx_desc *rx_desc;

    rx_desc = (struct ssv6200_rx_desc *)buf;
    SSV_LOG_DBG(">> RX Descriptor:\n");
    SSV_LOG_DBG("len=%d, c_type=%d, f80211=%d, qos=%d, ht=%d, use_4addr=%d, l3cs_err=%d, l4_cs_err=%d\n",
            rx_desc->len, rx_desc->c_type, rx_desc->f80211, rx_desc->qos, rx_desc->ht, rx_desc->use_4addr,
            rx_desc->l3cs_err, rx_desc->l4cs_err);
    SSV_LOG_DBG("align2=%d, psm=%d, stype_b5b4=%d, extra_info=%d\n", 
            rx_desc->align2, rx_desc->psm, rx_desc->stype_b5b4, rx_desc->extra_info);
    SSV_LOG_DBG("hdr_offset=%d, reason=%d, rx_result=%d\n", rx_desc->hdr_offset,
            rx_desc->reason, rx_desc->RxResult);
    SSV_LOG_DBG("\n\n\n");

}
#endif


#define FW_CHECKSUM_INIT                    (0x12345678)
#define CHECKSUM_BLOCK_SIZE                 512
#define FW_CHECKSUM_ADDR 0x0103bfc8
#define MAX_FIRMWARE_BLOCKS_CNT 3
#define MAC_FIRMWARE_BLOCKS_LEN 8

#define FIRMWARE_DOWNLOAD 0xf0
#define MAX_USB_BLOCK 512
#define FW_BLOCK_SIZE MAX_USB_BLOCK

#ifndef HWIF_DIS_FW_DOWNLOAD
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

#if 0
static void cal_firmware_checksum(void *buffer , int len)
{
    int i = 0;
    for(i = 0; i < len; i++) {
        fw_info.check_sum += *(char *)(buffer + i)&0xff;
        //SSV_LOG_DBG("%x (%x)\n", *(char *)(buffer + i)&0xff, fw_info.check_sum);
    }
    //SSV_LOG_DBG("\nchecksum: %x\n", fw_info.check_sum);
}

void ssv6xxx_usb_save_calibr_result(struct ssv6xxx_usb_glue *glue)
{
    int i;
    u32 regval = 0;
    ssv_cabrio_reg *pcalib = NULL;
    u32 calib_size = ssv_get_calibr_rsult_size();
    ssv_get_calibr_rsult_addr(&pcalib);

    for(i = 0; i < calib_size/sizeof(ssv_cabrio_reg); i++)
    {
        ssv6xxx_usb_read_reg(glue->dev, pcalib->address, &regval);
        pcalib->data = regval;
        pcalib++;
    }
}

void ssv_usb_restore_calibration(struct ssv6xxx_usb_glue *glue)
{

    int i;
    ssv_cabrio_reg *pcalib = NULL;
    u32 calib_size = ssv_get_calibr_rsult_size();
    ssv_get_calibr_rsult_addr(&pcalib);

    for(i = 0; i < calib_size/sizeof(ssv_cabrio_reg); i++)
    {
        ssv6xxx_usb_write_reg(glue->dev, pcalib->address, pcalib->data);
        pcalib++;
    }
}
#endif

static int ssv6xxx_usb_write_sram(struct usb_device *udev, u32 start_addr, u8 *data, int data_length)
{
    u16 laddr, haddr;
    u32 addr;
    int retval = 0, max_usb_block = 512;
    u8 *pdata;
    int res_length, offset, send_length;

    offset = 0;
    pdata = data;
    addr = start_addr;
    res_length = data_length;

    while (offset < data_length) {
        int transfer = min_t(int, res_length, max_usb_block);
        laddr = (addr & 0x0000ffff);
        haddr = (addr >> 16);
        send_length = usb_control_msg(udev, usb_sndctrlpipe(udev, 0),
                FIRMWARE_DOWNLOAD, (USB_DIR_OUT | USB_TYPE_VENDOR),
                laddr, haddr, pdata, transfer, TRANSACTION_TIMEOUT);

        if (send_length < 0) {
            retval = send_length;
            SSV_LOG_DBG("Load Firmware Fail, retval=%d, sram=0x%08x\n", retval, (laddr|haddr));
            break;
        }

        addr += transfer;
        pdata += transfer;
        offset += transfer;
        res_length -= transfer;
    }

    return retval;
}

static int ssv6xxx_usb_load_firmware(struct usb_device *udev, u32 start_addr, u8 *data, int data_length)
{
	return ssv6xxx_usb_write_sram(udev, start_addr, data, data_length);
}

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

static u32 ssv6xxx_write_firmware_to_sram(struct usb_device *udev, void *fw_buffer,
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

        if ((ret = ssv6xxx_usb_load_firmware(udev, sram_addr, (u8 *)fw_buffer, CHECKSUM_BLOCK_SIZE)) != 0) {
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
static int __ssv6xxx_usb_read_reg(struct ssv6xxx_usb_glue *glue, u32 addr, u32 *buf);
static int _ssv6xxx_usb_load_firmware(struct ssv6xxx_usb_glue *glue, u8 *firmware_name, bool openfile)
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
    struct usb_device *udev = glue->udev;
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
#ifdef HOST_CALCULATE_CHECKSUM
            u32 fw_checksum = FW_CHECKSUM_INIT;
#endif 
        // initial value
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

            //SSV_LOG_DBG("\nblock %d ssv6xxx_write_firmware_to_sram addr = 0x%x, len = %d\n", fw_block_idx, fw_info.block[fw_block_idx].start_addr, fw_info.block[fw_block_idx].len);
            checksum += ssv6xxx_write_firmware_to_sram(udev, fw_buffer, fw_fp, fw_info.block[fw_block_idx].len, fw_info.block[fw_block_idx].start_addr, openfile, ((fw_block_idx == 1) ? false : true), &write_sram_err);

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
                    __ssv6xxx_usb_read_reg(glue, sram_addr, &value);
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
static int ssv6xxx_usb_load_firmware_openfile(struct ssv6xxx_usb_glue *glue)
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

    return _ssv6xxx_usb_load_firmware(glue, firmware_name, openfile);
}

static int ssv6xxx_usb_load_firmware_fromheader(struct ssv6xxx_usb_glue *glue)
{
    return _ssv6xxx_usb_load_firmware(glue, NULL, false);
}


static int ssv6xxx_usb_force_load_fw_fromeheader(struct ssv6xxx_usb_glue *glue)
{
    int ret = 0;

#ifndef HWIF_DIS_FW_DOWNLOAD
    //Jump to ROM before load firmware.
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00004, 0x80000); //ROM code address 0x80000
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00134, 0x200);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00014, 0xff000101);
    msleep(50); //Delay to avoid ROM code is not ready.
#endif


    ret = ssv6xxx_usb_load_firmware_fromheader(glue);

    if(ret)
        goto err;


#ifndef HWIF_DIS_FW_DOWNLOAD
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00004, 0x1000000);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00134, 0x200);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00014, 0xff000101);
#ifdef HWIF_SET_PADMUX
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00800, 0x7f80);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00804, 0x30);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00860, 0x31);
    ssv6xxx_usb_write_reg(glue->dev, 0x80000, 0xd500d5);
#endif
    mdelay(500); //Wait 500ms for firmware ready.
#endif

err:
    return ret;
}


static int ssv6xxx_usb_load_each_fw(struct ssv6xxx_usb_glue *glue)
{
    int ret = 0;

#ifndef HWIF_DIS_FW_DOWNLOAD
    //Jump to ROM before load firmware.
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00004, 0x80000); //ROM code address 0x80000
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00134, 0x1200);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00014, 0xff000101);
    msleep(50); //Delay to avoid ROM code is not ready.
#endif

    if((NULL != cfgfirmwarepath) || (0x00 != ssv_cfg.firmware_path[0]))
    {
        ret = ssv6xxx_usb_load_firmware_openfile(glue);
    }
    else
    {
        ret = ssv6xxx_usb_load_firmware_fromheader(glue);
    }

    if(ret)
        goto err;


#ifndef HWIF_DIS_FW_DOWNLOAD
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00004, 0x1000000);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00134, 0x1200);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00014, 0xff000101);
#ifdef HWIF_SET_PADMUX
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00800, 0x7f80);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00804, 0x30);
    ssv6xxx_usb_write_reg(glue->dev, 0x08c00860, 0x31);
    ssv6xxx_usb_write_reg(glue->dev, 0x80000, 0xd500d5);
#endif
    mdelay(500); //Wait 500ms for firmware ready.
#endif

err:
    return ret;
}

int ssv6xxx_usb_configure_ipc_mem(struct device *child);
static int ssv6xxx_usb_load_fw(struct ssv6xxx_usb_glue *glue)
{
    //shrink code size: 
    //pre load engineer fw to setup rf_phy table and calibaration
    int ret;
    int fw_temp = ssv_cfg.firmware_choice;
    begin_download_fw = 1;
    ssv6xxx_usb_configure_ipc_mem(glue->dev);

    ssv_cfg.firmware_choice = 2;
    //load engineer fw first
    ret = ssv6xxx_usb_force_load_fw_fromeheader(glue);
    // ssv6xxx_usb_save_calibr_result(glue);
    ssv_cfg.firmware_choice = fw_temp;
    if (ret) 
        return ret;

    //load original FW
    ret = ssv6xxx_usb_load_each_fw(glue);
    // if(!ret)
    //     ssv_usb_restore_calibration(glue);
    begin_download_fw = 0;
    return ret;
}
#endif

static u16 ssv6xxx_get_cmd_sequence(struct ssv6xxx_usb_glue *glue)
{
    glue->sequence = glue->sequence % USB_CMD_SEQUENCE;
    (glue->sequence)++;

    return glue->sequence;
}

#ifdef CONFIG_USB_TX_MULTI_URB
static void ssv6xxx_usb_tx_complete(struct urb *urb)
{
    struct sk_buff *skb = (struct sk_buff *)urb->context;

    if (urb->status) {
        SSV_LOG_DBG("fail tx status received:%d\n", urb->status);
    }
    atomic_dec(&tx_urb_cnt);  
    kfree_skb(skb);
    return;
}
#endif

static void ssv6xxx_usb_delete(struct kref *kref)
{
    struct ssv6xxx_usb_glue *glue = to_ssv6xxx_usb_dev(kref);
    int i;

    /* cancel urb */
    for (i = 0 ; i < SSV_USB_MAX_NR_RECVBUFF ; ++i) {
        usb_kill_urb(glue->ssv_rx_buf[i].rx_urb);
    }

    /* free buffer */
    if (glue->cmd_endpoint.buff) 
        kfree(glue->cmd_endpoint.buff);
    if (glue->rsp_endpoint.buff) 
        kfree(glue->rsp_endpoint.buff);
    if (glue->ssv_rx_buf[0].rx_buf) {
        for (i = 0 ; i < SSV_USB_MAX_NR_RECVBUFF ; ++i) {
            usb_free_coherent(glue->udev, MAX_USB_RX_AGGR_SIZE,
                    glue->ssv_rx_buf[i].rx_buf,
                    glue->ssv_rx_buf[i].rx_urb->transfer_dma);
            /* free urb */
            usb_free_urb(glue->ssv_rx_buf[i].rx_urb);
        }
    }

    if (glue->ep0_buf) {
        kfree(glue->ep0_buf);
    }
        
    if (ssv_rx_use_wq) {
        cancel_work_sync((struct work_struct *)&glue->rx_work);
        destroy_workqueue(glue->wq);
    } else {
        tasklet_kill(&glue->rx_tasklet);
    }

    usb_put_dev(glue->udev);
    kfree(glue);
}

static int ssv6xxx_usb_recv_rsp(struct ssv6xxx_usb_glue *glue, int size, int *rsp_len)
{
    int retval = 0, foolen = 0;

    if (!glue || !glue->interface) {
        retval = -ENODEV;
        return retval;
    }

    retval = usb_bulk_msg(glue->udev, 
            usb_rcvbulkpipe(glue->udev, glue->rsp_endpoint.address),
            glue->rsp_endpoint.buff, size,
            &foolen, TRANSACTION_TIMEOUT);

    if (retval) {
        *rsp_len = 0;
        //SSV_LOG_DBG("Cannot receive response, error=%d\n", retval);
        //if (((glue->err_cnt)++) > 5)
        //  WARN_ON(1);
    } else { 
        *rsp_len = foolen;
        glue->err_cnt = 0;
    }

    return retval;
}

static int ssv6xxx_usb_send_cmd(struct ssv6xxx_usb_glue *glue, u8 cmd, u16 seq, const void *data, u32 data_len)
{
    int retval = 0, foolen = 0;
    struct ssv6xxx_cmd_hdr *hdr;

    if (!glue || !glue->interface) {
        retval = -ENODEV;
        return retval;
    }

    /* fill the cmd context 
     * packet format
     * =============================================
     * |   plen   |   cmd    |    seq   | payload  |
     * |  1 byte  |  1 byte  |  2bytes  |          |
     * =============================================   
     * */
    hdr = (struct ssv6xxx_cmd_hdr *)glue->cmd_endpoint.buff;
    memset(hdr, 0, sizeof(struct ssv6xxx_cmd_hdr));
    hdr->plen = (data_len >> (0))& 0xff;
    hdr->cmd = cmd;
    hdr->seq = cpu_to_le16(seq);
    memcpy(&hdr->payload, data, data_len);

    retval = usb_bulk_msg(glue->udev, 
            usb_sndbulkpipe(glue->udev, glue->cmd_endpoint.address),
            glue->cmd_endpoint.buff, (data_len+SSV6XXX_CMD_HEADER_SIZE),
            &foolen, TRANSACTION_TIMEOUT);

    if (retval) { 
        SSV_LOG_DBG("Cannot send cmd data, error=%d\n", retval);
        //if (((glue->err_cnt)++) > 5)
        //  WARN_ON(1);
    } else {
        glue->err_cnt = 0;
    }

    return retval;
}

static int __ssv6xxx_usb_cmd(struct ssv6xxx_usb_glue *glue, u8 cmd, void *data, u32 data_len, void *result)
{
    #define MAX_RETRY 5   
    int retval = (-1), rsp_len = 0, i = 0;
    struct ssv6xxx_cmd_hdr *rsphdr;
    u16 sequence;
    u32 retry_times = 0;

    mutex_lock(&glue->cmd_mutex);
    sequence = ssv6xxx_get_cmd_sequence(glue);
    do {
        retval = ssv6xxx_usb_send_cmd(glue, cmd, sequence, data, data_len);
    } while(retval && (++retry_times < MAX_RETRY));
    
    if (retval) {
        //SSV_LOG_DBG("%s: Fail to send cmd, sequence=%d, retval=%d\n", 
        //        __FUNCTION__, sequence, retval);
        goto exit;
    }

    /* If cmd error(result is device buff), we should read previous result. */
    for (i = 0; i < USB_CMD_SEQUENCE; i++) {
        retry_times = 0;
        do {
            retval = ssv6xxx_usb_recv_rsp(glue, SSV6XXX_MAX_RXCMDSZ, &rsp_len);
        } while(retval && (++retry_times < MAX_RETRY));
        
        if (retval) {
            //SSV_LOG_DBG("%s: Fail to receive response, sequence=%d, retval=%d\n", 
            //        __FUNCTION__, sequence, retval);
            goto exit;
        }
        /* parse the response context */
        if (rsp_len < SSV6XXX_CMD_HEADER_SIZE) {
            //SSV_LOG_DBG("Receviced abnormal response length[%d]\n", rsp_len);
            goto exit; 
        }
        rsphdr = (struct ssv6xxx_cmd_hdr *)glue->rsp_endpoint.buff;
        if (sequence == rsphdr->seq) 
            break;
        else {
            //SSV_LOG_DBG("received incorrect sequence=%d[%d]\n", sequence, rsphdr->seq);
        }
    }
    switch (rsphdr->cmd) {
        case SSV6200_CMD_WRITE_REG:
            break;
        case SSV6200_CMD_READ_REG:
            if (result)
                memcpy(result, &rsphdr->payload, sizeof(struct ssv6xxx_read_reg_result));
            break;
        default:
            retval = -1;
            //SSV_LOG_DBG("%s: unknown response cmd[%d]\n", __FUNCTION__, rsphdr->cmd);
            break;
    }

exit:
    mutex_unlock(&glue->cmd_mutex);
    return retval;
}

static int ssv6xxx_usb_cmd(struct ssv6xxx_usb_glue *glue, u8 cmd, void *data, u32 data_len, void *result)
{
    #define MAX_CMD_RETRY 3   
    u32 retry_times = 0;
    int retval = -1;

    do {
        retval = __ssv6xxx_usb_cmd(glue, cmd, data, data_len, result);
    } while(retval && (++retry_times < MAX_CMD_RETRY));

    return retval;
}
    
static void ssv6xxx_usb_recv_rx_work(struct work_struct *work)
{
    struct ssv6xxx_usb_glue *glue = ((struct ssv6xxx_usb_work_struct *)work)->glue;
    struct sk_buff *rx_mpdu;
    struct ssv6xxx_usb_rx_buf *ssv_rx_buf;
    unsigned char *data;

    while (NULL != (ssv_rx_buf = (struct ssv6xxx_usb_rx_buf *)ssv6xxx_dequeue_list_node(&glue->ssv_rx_queue))) {
        if (glue->is_rx_q_full && glue->is_rx_q_full(glue->rx_cb_args)) {
            //SSV_LOG_DBG("%s(): RX queue is full.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            break;
        }
        (glue->rx_pkt)++;
        rx_mpdu = ssv_rx_skb_alloc(ssv_rx_buf->rx_filled, GFP_KERNEL);
        if (rx_mpdu == NULL) {
            //SSV_LOG_DBG("%s(): Can't alloc skb.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            break;
        }

        data = skb_put(rx_mpdu, ssv_rx_buf->rx_filled);
        memcpy(data, ssv_rx_buf->rx_buf, ssv_rx_buf->rx_filled);
        if (glue->rx_cb)
            glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        else
            ssv_rx_skb_free(rx_mpdu);
           

        ssv6xxx_usb_recv_rx(glue, ssv_rx_buf);
    }
}

static void ssv6xxx_usb_recv_rx_tasklet(unsigned long priv)
{
    struct ssv6xxx_usb_glue *glue = (struct ssv6xxx_usb_glue *)priv;
    struct sk_buff *rx_mpdu;
    struct ssv6xxx_usb_rx_buf *ssv_rx_buf;
    unsigned char *data;

    while (NULL != (ssv_rx_buf = (struct ssv6xxx_usb_rx_buf *)ssv6xxx_dequeue_list_node(&glue->ssv_rx_queue))) {
        if (glue->is_rx_q_full && glue->is_rx_q_full(glue->rx_cb_args)) {
            //SSV_LOG_DBG("%s(): RX queue is full.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            tasklet_schedule(&glue->rx_tasklet);
            break;
        }
        (glue->rx_pkt)++;
        rx_mpdu = ssv_rx_skb_alloc(ssv_rx_buf->rx_filled, GFP_ATOMIC);
        if (rx_mpdu == NULL) {
            //SSV_LOG_DBG("%s(): Can't alloc skb.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            tasklet_schedule(&glue->rx_tasklet);
            break;
        }
        
        data = skb_put(rx_mpdu, ssv_rx_buf->rx_filled);
        memcpy(data, ssv_rx_buf->rx_buf, ssv_rx_buf->rx_filled);
        ssv6xxx_usb_recv_rx(glue, ssv_rx_buf);
        
        if (glue->rx_cb)
            glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        else
            ssv_rx_skb_free(rx_mpdu);
        
    }
}

static void ssv6xxx_usb_recv_rx_complete(struct urb *urb)
{
    struct ssv6xxx_usb_rx_buf *ssv_rx_buf = (struct ssv6xxx_usb_rx_buf *)urb->context;
    struct ssv6xxx_usb_glue *glue = ssv_rx_buf->glue;
    ssv_rx_buf->rx_res = urb->status;

    if (urb->status) {
        SSV_LOG_DBG("fail rx status received:%d\n", urb->status);
        goto skip;
    }
    // SSV_LOG_DBG("[%s][%d] urb->actual_length = %d\n", __FUNCTION__, __LINE__, urb->actual_length);
    glue->err_cnt = 0;
    ssv_rx_buf->rx_filled = urb->actual_length;

    if (ssv_rx_buf->rx_filled > MAX_USB_RX_AGGR_SIZE) {
        SSV_LOG_DBG("recv invalid data length %d\n", ssv_rx_buf->rx_filled);
        goto skip;
    }
    ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
    if (ssv_rx_use_wq) {
        queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
    } else {
        tasklet_schedule(&glue->rx_tasklet);
    }

    return;

skip:
    ssv6xxx_usb_recv_rx(glue, ssv_rx_buf);
}

static void ssv6xxx_usb_recv_rx(struct ssv6xxx_usb_glue *glue, struct ssv6xxx_usb_rx_buf *ssv_rx_buf)
{
    int size = MAX_USB_RX_AGGR_SIZE; 
    int retval;

    // SSV_LOG_DBG("[%s][%d]\n", __FUNCTION__, __LINE__);
    /* prepare a read */
    usb_fill_bulk_urb(ssv_rx_buf->rx_urb,
            glue->udev, usb_rcvbulkpipe(glue->udev, glue->rx_endpoint.address),
            ssv_rx_buf->rx_buf, size,
            ssv6xxx_usb_recv_rx_complete, ssv_rx_buf);
    ssv_rx_buf->rx_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

    /* submit bulk in urb, which means no data to deliver */
    ssv_rx_buf->rx_filled = 0;

    /* do it */
    retval = usb_submit_urb(ssv_rx_buf->rx_urb, GFP_ATOMIC);
    if (retval) {
        SSV_LOG_DBG("Fail to submit rx urb, error=%d\n", retval);
    }
}

/* 
 * For usb interface,  we make use of ssv6xxx_usb_recv_rx() to receive the rx frame.
 */
static int ssv6xxx_usb_read(struct device *child,
        void *buf, size_t *size)
{
    *size = 0;
    return 0;
}

static int ssv6xxx_usb_send_tx(struct ssv6xxx_usb_glue *glue, struct sk_buff *skb, size_t size)
{
#ifndef CONFIG_USB_TX_MULTI_URB
    int foolen = 0, retval = 0;
    int tx_len = size;  
    
    if (((unsigned long)skb->data) & (PLATFORM_DEF_DMA_ALIGN_SIZE - 1))
        SSV_LOG_DBG("USB Write: unalignmen!!!!!\n");
#else
	int retval = 0;
	int tx_len = size;
	struct urb *tx_urb = usb_alloc_urb(0, GFP_ATOMIC);
	struct sk_buff *tx_skb;
#if 0
    if (((unsigned long)skb->data) & (PLATFORM_DEF_DMA_ALIGN_SIZE - 1))
        SSV_LOG_DBG("USB Write: unalignmen!!!!!\n");
#endif
	if (!tx_urb) {
		dev_err(glue->dev, "Could not allocate tx urb\n");
		retval = -1;
		return retval;
	}

	if ((tx_skb = skb_clone(skb, GFP_ATOMIC)) == NULL) {
		dev_err(glue->dev, "Could not allocate tx urb\n");
		retval = -1;
		usb_free_urb(tx_urb);
		return retval;
	}

#endif
    /* for USB 3.0 port, add padding byte and let host driver send short packet */
    if ((tx_len % glue->tx_endpoint.packet_size) == 0) {
        skb_put(skb, 1);
        tx_len++;
    }
#ifndef CONFIG_USB_TX_MULTI_URB
    retval = usb_bulk_msg(glue->udev, 
            usb_sndbulkpipe(glue->udev, glue->tx_endpoint.address),
            skb->data, tx_len, &foolen, TRANSACTION_TIMEOUT);
    if (retval) 
        SSV_LOG_DBG("Cannot send tx data, retval=%d\n", retval);
#else
	/* prepare a write */
    	atomic_inc(&tx_urb_cnt);  
	usb_fill_bulk_urb(tx_urb,
			glue->udev, usb_sndbulkpipe(glue->udev, glue->tx_endpoint.address),
			/*tx_buf*/skb->data, tx_len,
			ssv6xxx_usb_tx_complete, tx_skb);
	//tx_urb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
	//tx_urb->transfer_flags |= URB_FREE_BUFFER;
	//tx_urb->transfer_flags |= URB_NO_INTERRUPT;
	/* do it */
	retval = usb_submit_urb(tx_urb, GFP_ATOMIC);
	if (retval) {
		SSV_LOG_DBG("Fail to submit tx urb, error=%d\n", retval);
		kfree_skb(tx_skb);
	}
	usb_free_urb(tx_urb);
#endif

    return retval;
}

static int ssv6xxx_usb_write(struct device *child,
        void *buf, size_t len, u8 queue_num)
{
    int retval = (-1);
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);
#ifdef SSV_PERFORMANCE_WATCH
    unsigned long tx_now_time, tx_run_time;
    tx_now_time = jiffies;
#endif
    (glue->tx_pkt)++;

    if (IS_GLUE_INVALID(glue))
        return retval;

#ifdef BYPASS_USB_TX_TEST
    SSV_LOG_DBG("[%s][%d] test return 0.\n", __FUNCTION__, __LINE__);
    return 0;
#endif

    /* for debug */
    //ssv6xxx_dump_tx_desc(buf);
    /* use urb to send tx data */
    if ((retval = ssv6xxx_usb_send_tx(glue, (struct sk_buff *)buf, len)) < 0) {
        SSV_LOG_DBG("%s: Fail to send tx data\n", __FUNCTION__);
        //if (((glue->err_cnt)++) > 5)
        //  WARN_ON(1);
    } else {
        glue->err_cnt = 0;
    }
#ifdef SSV_PERFORMANCE_WATCH
    if (time_after(jiffies, tx_now_time)) {
        tx_run_time = jiffies_to_msecs(jiffies - tx_now_time);
        glue->tx_run_data_time += tx_run_time;
    }
#endif
    return retval;
}

static int __ssv6xxx_usb_ep0_read_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
               u32 *buf)
{
    int i, retval = (-1);
    //struct ssv6xxx_read_reg_result *result;

    if (IS_GLUE_INVALID(glue))
        return retval;

    mutex_lock(&glue->cmd_mutex);
    //result = kmalloc(sizeof(struct ssv6xxx_read_reg_result), GFP_KERNEL);
    for (i = 0; i < USB_CMD_SEQUENCE; i++) {
#if 0
        retval = usb_control_msg(glue->udev, usb_rcvctrlpipe(glue->udev, 0),
           VENDOR_SPECIFIC_REG_RW, (USB_DIR_IN | USB_TYPE_VENDOR),
           cpu_to_le32((addr>>16)&0xffff), cpu_to_le32(addr&0xffff), (void *)&result, sizeof(struct ssv6xxx_read_reg_result), TRANSACTION_TIMEOUT);
#else
        retval = usb_control_msg(glue->udev, usb_rcvctrlpipe(glue->udev, 0),
           VENDOR_SPECIFIC_REG_RW, (USB_DIR_IN | USB_TYPE_VENDOR),
           cpu_to_le32((addr>>16)&0xffff), cpu_to_le32(addr&0xffff), glue->ep0_buf, sizeof(struct ssv6xxx_read_reg_result), 1000);
#endif
        if (retval >= 0) {
            retval = 0;
            //*buf = le32_to_cpu(result->value);
            *buf = le32_to_cpu(*glue->ep0_buf);
            //if (i != 0) {
                //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Finally succeed to read %x\n", __FUNCTION__, addr);
            //}
            break;
        //} else {
            //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to read %x.. Retry\n", __FUNCTION__, addr);
        }
    }
    mutex_unlock(&glue->cmd_mutex);
    return retval;
}

static int __ssv6xxx_usb_read_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
        u32 *buf)
{
    int retval = (-1);
    struct ssv6xxx_read_reg *read_reg;
    struct ssv6xxx_read_reg_result result;
    u8 data[sizeof(struct ssv6xxx_read_reg)];
    
    read_reg = (struct ssv6xxx_read_reg*)data;
    
    if (IS_GLUE_INVALID(glue)) {
        //SSV_LOG_DBG("glue->dev_ready %d\n", glue->dev_ready);
        if (glue == NULL)
            SSV_LOG_DBG("glue is NULL\n");
        else
            SSV_LOG_DBG("glue is not NULL\n");


        return retval;
    }

    memset(read_reg, 0, sizeof(struct ssv6xxx_read_reg));  
    memset(&result, 0, sizeof(struct ssv6xxx_read_reg_result)); 
    read_reg->addr = cpu_to_le32(addr);
    retval = ssv6xxx_usb_cmd(glue, SSV6200_CMD_READ_REG, read_reg, sizeof(struct ssv6xxx_read_reg), &result);
    if (!retval) 
        *buf = le32_to_cpu(result.value);
    else { 
        *buf = 0xffffffff;
        SSV_LOG_DBG("%s: Fail to read register address %x\n", __FUNCTION__, addr);
    }

    return retval;
}


static int ssv6xxx_usb_read_reg(struct device *child, u32 addr,
        u32 *buf)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);
    if(ssv_cfg.use_ep0_rw_reg == 1 && begin_download_fw == 0) {
        return __ssv6xxx_usb_ep0_read_reg(glue, addr, buf);
    } else {
        return __ssv6xxx_usb_read_reg(glue, addr, buf);
    }
}

static int  __ssv6xxx_usb_ep0_write_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
               u32 buf)
{
    int i, retval = (-1);
    u32 regval = cpu_to_le32(buf);

    if (IS_GLUE_INVALID(glue))
        return retval;

    mutex_lock(&glue->cmd_mutex);
    for (i = 0; i < USB_CMD_SEQUENCE; i++) {
#if 1
        retval = usb_control_msg(glue->udev, usb_sndctrlpipe(glue->udev, 0),
                   VENDOR_SPECIFIC_REG_RW, (USB_DIR_OUT | USB_TYPE_VENDOR),
                   cpu_to_le32((addr>>16)&0xffff), cpu_to_le32(addr&0xffff), NULL, 0, TRANSACTION_TIMEOUT);

        if (retval < 0) {
            SSV_LOG_DBG("%s: Fail to write register address %x\n", __FUNCTION__, addr);
            continue;
        }

        retval = usb_control_msg(glue->udev, usb_sndctrlpipe(glue->udev, 0),
                   VENDOR_SPECIFIC_REG_RW_WDATA, (USB_DIR_OUT | USB_TYPE_VENDOR),
                   cpu_to_le32((regval>>16)&0xffff), cpu_to_le32(regval&0xffff), NULL, 0, TRANSACTION_TIMEOUT);
#else
        retval = usb_control_msg(glue->udev, usb_sndctrlpipe(glue->udev, 0),
                   VENDOR_SPECIFIC_REG_RW, (USB_DIR_OUT | USB_TYPE_VENDOR),
                   cpu_to_le32((addr>>16)&0xffff), cpu_to_le32(addr&0xffff), (void *)&regval, 4, TRANSACTION_TIMEOUT);
#endif
        if (retval >= 0) {
            retval = 0;
            //if (i != 0) {
                //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Finally succeed to write %x\n", __FUNCTION__, addr);
            //}
            break;
        //} else {
            //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to write %x.. Retry\n", __FUNCTION__, addr);
        }
    }
    mutex_unlock(&glue->cmd_mutex);
    return retval;
}

static int __ssv6xxx_usb_write_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
        u32 buf)
{
    int retval = (-1);
    struct ssv6xxx_write_reg *write_reg;
    u8 data[sizeof(struct ssv6xxx_write_reg)];
    
    write_reg = (struct ssv6xxx_write_reg*)data;

    if (IS_GLUE_INVALID(glue))
        return retval;

    memset(write_reg, 0, sizeof(struct ssv6xxx_write_reg));    
    write_reg->addr = cpu_to_le32(addr);
    write_reg->value = cpu_to_le32(buf);

    retval = ssv6xxx_usb_cmd(glue, SSV6200_CMD_WRITE_REG, write_reg, sizeof(struct ssv6xxx_write_reg), NULL);
    if (retval) 
        SSV_LOG_DBG("%s: Fail to write register address %x, value %x\n", __FUNCTION__, addr, buf);

    return retval;
}

static int ssv6xxx_usb_write_reg(struct device *child, u32 addr,
        u32 buf)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);
    if(ssv_cfg.use_ep0_rw_reg == 1 && begin_download_fw == 0) {
        return __ssv6xxx_usb_ep0_write_reg(glue, addr, buf);
    } else {
        return __ssv6xxx_usb_write_reg(glue, addr, buf);
    }
}

static void ssv6xxx_usb_get_usb_urb_cnt(struct device *child, int *urb_cnt)
{
#ifdef CONFIG_USB_TX_MULTI_URB
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    
	// TODO
    *urb_cnt = atomic_read(&tx_urb_cnt);  
#else
    *urb_cnt = 0;
    return;
#endif
}

static void ssv6xxx_usb_rx_task(struct device *child, 
        int (*rx_cb)(struct sk_buff *rx_skb, void *args), 
        int (*is_rx_q_full)(void *args), void *args, u32 recv_cnt)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);
    int i;
    int nr_recvbuff = (ssv_usb_rx_nr_recvbuff > SSV_USB_MAX_NR_RECVBUFF)?SSV_USB_MAX_NR_RECVBUFF:((ssv_usb_rx_nr_recvbuff < SSV_USB_MIN_NR_RECVBUFF)?SSV_USB_MIN_NR_RECVBUFF:ssv_usb_rx_nr_recvbuff);

    SSV_LOG_DBG("%s: nr_recvbuff=%d\n", __func__, nr_recvbuff);
    glue->rx_cb = rx_cb;
    glue->rx_cb_args = args;
    glue->is_rx_q_full = is_rx_q_full;
    
    for (i = 0 ; i < nr_recvbuff ; ++i) {
        ssv6xxx_usb_recv_rx(glue, &(glue->ssv_rx_buf[i]));
    }
}

static void ssv6xxx_usb_tx_st(struct device *child, u32 *pkt_cnt)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    *pkt_cnt = glue->tx_pkt;
}

static void ssv6xxx_usb_rx_st(struct device *child, u32 *pkt_cnt)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    *pkt_cnt = glue->rx_pkt;
}

static void ssv6xxx_usb_fw_reset(struct device *child)
{
    int retval = -ENOMEM;
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }

#ifndef HWIF_DIS_FW_DOWNLOAD
    //load fw before platform initialize
    retval = ssv6xxx_usb_load_fw(glue);
    if (retval) 
    {
        SSV_LOG_DBG("Fail to load firmware\n");
    }
#endif

    //Set HCI path.
    {
        #define CO_BIT_MASK(pos)                                (1UL<<(pos))
        #define REG_HCI_BASE_ADDRESS                            (0x08C10000)
        #define REG_HCI_CONTROL_REG_ADDRESS                     (REG_HCI_BASE_ADDRESS + 0x08)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS    REG_HCI_CONTROL_REG_ADDRESS
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT      (1)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK       CO_BIT_MASK(REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_usb_read_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, &tmp_regval);
        tmp_regval |= REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK;
        ssv6xxx_usb_write_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, tmp_regval);
    }
    {
        //set HCI TX I/O aggr
        //#define REG_HCI_BASE_ADDRESS                                                    (0x08C10000)
        //#define REG_HCI_CONTROL_REG_ADDRESS                                             (REG_HCI_BASE_ADDRESS + 0x08) 
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_ADDRESS                           REG_HCI_CONTROL_REG_ADDRESS
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT                             (0)
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_MASK                              CO_BIT_MASK(REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_usb_read_reg(glue->dev, 0x08C10008, &tmp_regval);
        if(ssv_cfg.hw_caps & HW_CAP_HCI_TX_AGGR) {
            tmp_regval |= 0x01;
        }
        else {
            tmp_regval &= ~0x01;
        }
        ssv6xxx_usb_write_reg(glue->dev, 0x08C10008, tmp_regval);
    }
}
#ifdef SSV_PERFORMANCE_WATCH
static void ssv6xxx_usb_get_info(struct device *child, struct ssv6xxx_hwif_info *info)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    info->tx_run_data_time = glue->tx_run_data_time;
}

static void ssv6xxx_usb_clr_info(struct device *child)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child);

    if (IS_GLUE_INVALID(glue)) {
        SSV_LOG_DBG("%s(): glue is invalid!\n", __func__);
        return;
    }
    glue->tx_run_data_time = 0;
}
#endif
static struct ssv6xxx_hwif_ops usb_ops =
{
    .read            = ssv6xxx_usb_read,
    .write           = ssv6xxx_usb_write,
    .readreg         = ssv6xxx_usb_read_reg,
    .writereg        = ssv6xxx_usb_write_reg,
    .hwif_rx_task    = ssv6xxx_usb_rx_task,
    .get_tx_req_cnt  = ssv6xxx_usb_get_usb_urb_cnt,
    .tx_st           = ssv6xxx_usb_tx_st,
    .rx_st           = ssv6xxx_usb_rx_st,
    .fw_reset           = ssv6xxx_usb_fw_reset,
#ifdef SSV_PERFORMANCE_WATCH
    .get_info = ssv6xxx_usb_get_info,
    .clr_info = ssv6xxx_usb_clr_info,
#endif
};

void *_gp_plat_hw = NULL;
extern int ssv6xxx_platform_init(struct device *dev, struct ssv6xxx_hwif_ops *hwif_ops, void **plat_hw);
int ssv6xxx_usb_configure_ipc_mem(struct device *child)
{
    return ssv_configure_ipc_mem(child, &usb_ops);
}
static int ssv_usb_probe(struct usb_interface *interface,
        const struct usb_device_id *id)
{
    struct ssv6xxx_usb_glue *glue;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i, j;
    int retval = -ENOMEM;
    unsigned int  epnum;
    void *plat_hw = NULL;

    SSV_LOG_INFO("=======================================\n");
    SSV_LOG_INFO("==          TURISMO - USB            ==\n");
    SSV_LOG_INFO("=======================================\n");
    /* allocate memory for our device state and initialize it */
    glue = kzalloc(sizeof(*glue), GFP_KERNEL);
    if (!glue) {
        SSV_LOG_DBG("Cannot alloc usb driver data\n");
        retval = -ENOMEM;
        goto error;
    }
    glue->sequence = 0;
    glue->err_cnt = 0;
#ifdef CONFIG_USB_TX_MULTI_URB
    atomic_set(&tx_urb_cnt, 0);  
#endif
    kref_init(&glue->kref);
    mutex_init(&glue->io_mutex);
    mutex_init(&glue->cmd_mutex);


    glue->dev = &interface->dev;
    /* USB core needs to know usb_device, so get usb_device form usb_interface */
    glue->udev = usb_get_dev(interface_to_usbdev(interface));
    glue->interface = interface;

    /* set up the endpoint information */
    iface_desc = interface->cur_altsetting;
    for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
        endpoint = &iface_desc->endpoint[i].desc;

        epnum = endpoint->bEndpointAddress & 0x0f;
        if (epnum == SSV_EP_CMD) {
            glue->cmd_endpoint.address = endpoint->bEndpointAddress;
            glue->cmd_endpoint.packet_size = le16_to_cpu(endpoint->wMaxPacketSize);
            glue->cmd_endpoint.buff = kmalloc(SSV6XXX_MAX_TXCMDSZ, GFP_ATOMIC);
            if (!glue->cmd_endpoint.buff) {
                dev_err(&interface->dev, "Could not allocate cmd buffer\n");
                goto error;
            }
        }

        if (epnum == SSV_EP_RSP) {
            glue->rsp_endpoint.address = endpoint->bEndpointAddress;
            glue->rsp_endpoint.packet_size = le16_to_cpu(endpoint->wMaxPacketSize);
            glue->rsp_endpoint.buff = kmalloc(SSV6XXX_MAX_RXCMDSZ, GFP_ATOMIC);
            if (!glue->rsp_endpoint.buff) {
                dev_err(&interface->dev, "Could not allocate rsp buffer\n");
                goto error;
            }
        }

        if (epnum == SSV_EP_TX) {
            glue->tx_endpoint.address = endpoint->bEndpointAddress;
            glue->tx_endpoint.packet_size = le16_to_cpu(endpoint->wMaxPacketSize);
        }

        if (epnum == SSV_EP_RX) {
            glue->rx_endpoint.address = endpoint->bEndpointAddress;
            glue->rx_endpoint.packet_size = le16_to_cpu(endpoint->wMaxPacketSize);
            for (j = 0 ; j < SSV_USB_MAX_NR_RECVBUFF ; ++j) {
                glue->ssv_rx_buf[j].rx_urb = usb_alloc_urb(0, GFP_ATOMIC);
                if (!glue->ssv_rx_buf[j].rx_urb) {
                    dev_err(&interface->dev, "Could not allocate rx urb\n");
                    goto error;
                }

                glue->ssv_rx_buf[j].rx_buf = usb_alloc_coherent(
                        glue->udev, MAX_USB_RX_AGGR_SIZE,
                         (in_interrupt() ? GFP_ATOMIC : GFP_KERNEL), &glue->ssv_rx_buf[j].rx_urb->transfer_dma);
                if (!glue->ssv_rx_buf[j].rx_buf) {
                    dev_err(&interface->dev, "Could not allocate rx buffer\n");
                    goto error;
                }
                glue->ssv_rx_buf[j].glue = glue;
                ssv6xxx_init_list_node((struct ssv6xxx_list_node *)&glue->ssv_rx_buf[j]);
            }
        }
    }
    if(ssv_cfg.use_ep0_rw_reg ==1) {
        glue->ep0_buf = kmalloc(sizeof(struct ssv6xxx_read_reg_result), GFP_ATOMIC);
        if (!glue->ep0_buf) {
            dev_err(&interface->dev, "Could not allocate ep0 buffer\n");
            goto error;
        }
    }
    if (!(glue->cmd_endpoint.address && 
                glue->rsp_endpoint.address && 
                glue->tx_endpoint.address && 
                glue->rx_endpoint.address)) 
    {
        dev_err(&interface->dev, "Could not find all endpoints\n");
        goto error;
    }

    /* save our data pointer in this interface device */
    usb_set_intfdata(interface, glue);
    glue->dev_ready = true;

#ifdef HWIF_TRIM_CODE
begin_download_fw = 1;
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
#define REG32_R(ADDR, val)          (ssv6xxx_usb_read_reg(glue->dev, ADDR, val))
#define REG32_W(ADDR, val)          (ssv6xxx_usb_write_reg(glue->dev, ADDR, val))
#define REG_TRIM_MSK                (0xE0000000)
    do {
        u32 _regval, _hw_val;
        REG32_R(REG_TRIM_ADDR, &_regval);
        REG32_R(REG_PMU_DLDO_AND_DCDC_ADDR, &_hw_val);
        _hw_val = ((_hw_val) & (~REG_DLDO_LV_MSK)) | ((((_regval >> REG_TRIM_OFS) & 0x7) ^ 0x3) << REG_DLDO_LV_OFS);
        //SSV_LOG_DBG("trim code write 0x%x\n", _hw_val);
        REG32_W(REG_PMU_DLDO_AND_DCDC_ADDR, _hw_val);
    } while(0);
begin_download_fw = 0;
#endif


#ifndef HWIF_DIS_FW_DOWNLOAD
    //load fw before platform initialize
    retval = ssv6xxx_usb_load_fw(glue);
    if (retval) 
    {
        SSV_LOG_DBG("Fail to load firmware\n");
        retval = -ENOMEM;
        goto error;
    }
#endif
    
    //Set HCI path.
    {
        #define CO_BIT_MASK(pos)                                (1UL<<(pos))
        #define REG_HCI_BASE_ADDRESS                            (0x08C10000)
        #define REG_HCI_CONTROL_REG_ADDRESS                     (REG_HCI_BASE_ADDRESS + 0x08)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS    REG_HCI_CONTROL_REG_ADDRESS
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT      (1)
        #define REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK       CO_BIT_MASK(REG_HCI_CONTROL_REG_USB20_HOST_SELRW_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_usb_read_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, &tmp_regval);
        tmp_regval |= REG_HCI_CONTROL_REG_USB20_HOST_SELRW_MASK;
        ssv6xxx_usb_write_reg(glue->dev, REG_HCI_CONTROL_REG_USB20_HOST_SELRW_ADDRESS, tmp_regval);
    }
    {
        //set HCI TX I/O aggr
        //#define REG_HCI_BASE_ADDRESS                                                    (0x08C10000)
        //#define REG_HCI_CONTROL_REG_ADDRESS                                             (REG_HCI_BASE_ADDRESS + 0x08) 
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_ADDRESS                           REG_HCI_CONTROL_REG_ADDRESS
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT                             (0)
        //#define REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_MASK                              CO_BIT_MASK(REG_HCI_CONTROL_REG_HCI_TX_AGG_EN_BIT_SHIFT)
        u32 tmp_regval = 0;
        ssv6xxx_usb_read_reg(glue->dev, 0x08C10008, &tmp_regval);
        if(ssv_cfg.hw_caps & HW_CAP_HCI_TX_AGGR) {
            tmp_regval |= 0x01;
        }
        else {
            tmp_regval &= ~0x01;
        }
        ssv6xxx_usb_write_reg(glue->dev, 0x08C10008, tmp_regval);
    }

    /* INIT RX */
    ssv6xxx_init_queue(&glue->ssv_rx_queue);
    if (ssv_rx_use_wq) {
        glue->rx_work.glue = glue;
        INIT_WORK((struct work_struct *)&glue->rx_work, ssv6xxx_usb_recv_rx_work);
        glue->wq = create_singlethread_workqueue("ssv6xxx_usb_wq");
        if (!glue->wq) {
            SSV_LOG_DBG("Could not allocate Work Queue\n");
            retval = -ENOMEM;
            goto error;
        }
    } else {
        tasklet_init(&glue->rx_tasklet, ssv6xxx_usb_recv_rx_tasklet, (unsigned long)glue);
    }

    retval = ssv6xxx_platform_init(&interface->dev, &usb_ops, &plat_hw);
    if (retval) {
        SSV_LOG_DBG("Fail to platform init\n");
        retval = -ENOMEM;
        goto error;
    }
    glue->plat_hw = plat_hw;
    _gp_plat_hw = plat_hw;
    
    return 0;

error:
    if (glue)
        /* this frees allocated memory */
        kref_put(&glue->kref, ssv6xxx_usb_delete);
    return retval;
}

extern void ssv6xxx_platform_deinit(void *plat_hw);
static void ssv_usb_disconnect(struct usb_interface *interface)
{
    struct ssv6xxx_usb_glue *glue = (struct ssv6xxx_usb_glue *)usb_get_intfdata(interface);
    
    SSV_LOG_DBG("ssv6xxx_usb_disconnect..........\n");
    if (glue) {
        if(NULL != _gp_plat_hw)
        {
            ssv6xxx_platform_deinit(glue->plat_hw);
            _gp_plat_hw = NULL;
        }
        
        // must be the last
        glue->dev_ready = false;
        /* prevent more I/O from starting */
        mutex_lock(&glue->io_mutex);
        glue->interface = NULL;
        mutex_unlock(&glue->io_mutex);
        /* decrement our usage count */
        kref_put(&glue->kref, ssv6xxx_usb_delete);
    }
    
    /* prevent more I/O from starting */
    usb_set_intfdata(interface, NULL);
    
    SSV_LOG_DBG("ssv6xxx_usb_disconnect leave..........\n");
}

#ifdef CONFIG_PM
static int ssv_usb_suspend(struct usb_interface *interface, pm_message_t message)
{
    /* Moved to ssv_usb_suspend_late(). */
    return 0;
}

static int ssv_usb_resume(struct usb_interface *interface)
{
    /* Moved to ssv_usb_resume_early(). */
    return 0;
}
#endif

static struct usb_driver ssv_usb_driver = {
    .name =     "SSV6XXX_USB",
    .probe =    ssv_usb_probe,
    .disconnect =   ssv_usb_disconnect,
#ifdef CONFIG_PM
    .suspend = ssv_usb_suspend,
    .resume = ssv_usb_resume,
#endif
    .id_table = ssv_usb_table,
    .supports_autosuspend = 1,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)    
    //disable_hub_initiated_lpm: 
    // if set to 0, the USB core will not allow hubs to initiate lower power link 
    // state transitions when an idle timeout occurs.
    .disable_hub_initiated_lpm = 0,
#endif  
};

int ssv6xxx_usb_init(void)
{
    SSV_LOG_DBG("ssv6xxx_usb_init\n");
    return usb_register(&ssv_usb_driver);
}

void ssv6xxx_usb_exit(void)
{
    SSV_LOG_DBG("ssv6xxx_usb_exit\n");
    if(NULL == _gp_plat_hw)
    {
        SSV_LOG_DBG("NULL == _gp_plat_hw\n");
    }
    else
    {
        ssv6xxx_platform_deinit(_gp_plat_hw);
        _gp_plat_hw = NULL;
    }
    usb_deregister(&ssv_usb_driver);
}

EXPORT_SYMBOL(ssv6xxx_usb_init);
EXPORT_SYMBOL(ssv6xxx_usb_exit);

MODULE_LICENSE("GPL");

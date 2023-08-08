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
#include <linux/platform_device.h>
#include <linux/version.h>
#include <linux/skbuff.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include <linux/printk.h>
#else
#include <linux/kernel.h>
#endif
#include <ssv6200.h>
//ToDo Liam this is temp for build.
// it should re-consider for multiple chip use.
#include <hwif/hwif.h>
#include "usb.h"
#include <ssv_data_struct.h>
#include "hwif/hal/hwif_hal.h"

/* Define these values to match devices */
#define USB_SSV_VENDOR_ID  0x8065
#define USB_SSV_PRODUCT_ID 0x6000

#define MAX_USB_RX_AGGR_SIZE        (3072)
#define TRANSACTION_TIMEOUT         (100) /* ms */
#define SSV6XXX_MAX_TXCMDSZ         (sizeof(struct ssv6xxx_cmd_hdr))
#define SSV6XXX_MAX_RXCMDSZ         (sizeof(struct ssv6xxx_cmd_hdr))
#define SSV6XXX_CMD_HEADER_SIZE     (sizeof(struct ssv6xxx_cmd_hdr) - sizeof(union ssv6xxx_payload))
#define USB_CMD_SEQUENCE            255
#define MAX_RETRY_SSV6XXX_ALLOC_BUF 3

#define IS_GLUE_INVALID(glue)  \
    (   (glue == NULL) \
        || (glue->dev_ready == false) \
        || (   (glue->p_wlan_data != NULL) \
            && (glue->p_wlan_data->is_enabled == false)) \
    )

/* table of devices that work with this driver */
static const struct usb_device_id ssv_usb_table[] = {
    { USB_DEVICE(USB_SSV_VENDOR_ID, USB_SSV_PRODUCT_ID) },
    { }                 
};
MODULE_DEVICE_TABLE(usb, ssv_usb_table);

extern int ssv_usb_rx_nr_recvbuff;
extern int ssv_rx_use_wq;

#ifdef CONFIG_USB_TX_MULTI_URB
static atomic_t    tx_urb_cnt;
#endif
/* Structure to hold all of our device specific stuff */
struct ssv6xxx_usb_glue {
    struct device                   *dev;
    struct platform_device          *core;
    struct usb_device               *udev;                      /* the usb device for this device */
    struct usb_interface            *interface;             /* the interface for this device */
    struct ssv6xxx_platform_data    *p_wlan_data;
    struct ssv6xxx_platform_data     tmp_data;
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
    u32 *rx_pkt;
    void *rx_cb_args;
    int (*rx_cb)(struct sk_buff *rx_skb, void *args);
    int (*is_rx_q_full)(void *);
    struct ssv_hwif_hal_ops hwif_hal_ops;
};

static void ssv6xxx_usb_recv_rx(struct ssv6xxx_usb_glue *glue, struct ssv6xxx_usb_rx_buf *ssv_rx_buf);

#define to_ssv6xxx_usb_dev(d) container_of(d, struct ssv6xxx_usb_glue, kref)

static struct usb_driver ssv_usb_driver;
#if 0
static void ssv6xxx_dump_tx_desc(const u8 *buf)
{
    struct ssv6200_tx_desc *tx_desc;

    tx_desc = (struct ssv6200_tx_desc *)buf;
    printk(">> Tx Frame:\n");
    printk("length: %d, c_type=%d, f80211=%d, qos=%d, ht=%d, use_4addr=%d, sec=%d\n", 
            tx_desc->len, tx_desc->c_type, tx_desc->f80211, tx_desc->qos, tx_desc->ht,
            tx_desc->use_4addr, tx_desc->security);
    printk("more_data=%d, sub_type=%x, extra_info=%d\n", tx_desc->more_data,
            tx_desc->stype_b5b4, tx_desc->extra_info);
    printk("fcmd=0x%08x, hdr_offset=%d, frag=%d, unicast=%d, hdr_len=%d\n",
            tx_desc->fCmd, tx_desc->hdr_offset, tx_desc->frag, tx_desc->unicast,
            tx_desc->hdr_len);
    printk("tx_burst=%d, ack_policy=%d, do_rts_cts=%d, reason=%d, payload_offset=%d\n", 
            tx_desc->tx_burst, tx_desc->ack_policy, tx_desc->do_rts_cts, 
            tx_desc->reason, tx_desc->payload_offset);
    printk("fcmdidx=%d, wsid=%d, txq_idx=%d\n",
            tx_desc->fCmdIdx, tx_desc->wsid, tx_desc->txq_idx);
    printk("RTS/CTS Nav=%d, frame_time=%d, crate_idx=%d, drate_idx=%d, dl_len=%d\n",
            tx_desc->rts_cts_nav, tx_desc->frame_consume_time, tx_desc->crate_idx, tx_desc->drate_idx,
            tx_desc->dl_length);
    printk("\n\n\n");
}
#endif

#if 0
static void ssv6xxx_dump_rx_desc(const u8 *buf)
{
    struct ssv6200_rx_desc *rx_desc;

    rx_desc = (struct ssv6200_rx_desc *)buf;
    printk(">> RX Descriptor:\n");
    printk("len=%d, c_type=%d, f80211=%d, qos=%d, ht=%d, use_4addr=%d, l3cs_err=%d, l4_cs_err=%d\n",
            rx_desc->len, rx_desc->c_type, rx_desc->f80211, rx_desc->qos, rx_desc->ht, rx_desc->use_4addr,
            rx_desc->l3cs_err, rx_desc->l4cs_err);
    printk("align2=%d, psm=%d, stype_b5b4=%d, extra_info=%d\n", 
            rx_desc->align2, rx_desc->psm, rx_desc->stype_b5b4, rx_desc->extra_info);
    printk("hdr_offset=%d, reason=%d, rx_result=%d\n", rx_desc->hdr_offset,
            rx_desc->reason, rx_desc->RxResult);
    printk("\n\n\n");

}
#endif


#ifdef CONFIG_USB_TX_MULTI_URB
static void ssv6xxx_usb_tx_complete(struct urb *urb)
{
    struct sk_buff *skb = (struct sk_buff *)urb->context;

    if (urb->status) {
        printk("fail tx status received:%d\n", urb->status);
    }
    atomic_dec(&tx_urb_cnt);  
    kfree_skb(skb);
    return;
}
#endif


static u16 ssv6xxx_get_cmd_sequence(struct ssv6xxx_usb_glue *glue)
{
    glue->sequence = glue->sequence % USB_CMD_SEQUENCE;
    (glue->sequence)++;

    return glue->sequence;
}

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
        //HWIF_DBG_PRINT(glue->p_wlan_data, "Cannot receive response, error=%d\n", retval);
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
        //HWIF_DBG_PRINT(glue->p_wlan_data, "Cannot send cmd data, error=%d\n", retval);
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
        //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to send cmd, sequence=%d, retval=%d\n", 
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
            //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to receive response, sequence=%d, retval=%d\n", 
            //        __FUNCTION__, sequence, retval);
            goto exit;
        }
        /* parse the response context */
        if (rsp_len < SSV6XXX_CMD_HEADER_SIZE) {
            //HWIF_DBG_PRINT(glue->p_wlan_data, "Receviced abnormal response length[%d]\n", rsp_len);
            goto exit; 
        }
        rsphdr = (struct ssv6xxx_cmd_hdr *)glue->rsp_endpoint.buff;
        if (sequence == rsphdr->seq) 
            break;
        else { 
            //HWIF_DBG_PRINT(glue->p_wlan_data, "received incorrect sequence=%d[%d]\n", sequence, rsphdr->seq);
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
            //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: unknown response cmd[%d]\n", __FUNCTION__, rsphdr->cmd);
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
    
	return ((retval == 0) ? 0 : -1);
}

static void ssv6xxx_usb_recv_rx_work(struct work_struct *work)
{
    struct ssv6xxx_usb_glue *glue = ((struct ssv6xxx_usb_work_struct *)work)->glue;
    struct sk_buff *rx_mpdu;
    struct ssv6xxx_usb_rx_buf *ssv_rx_buf;
    unsigned char *data;

    while (NULL != (ssv_rx_buf = (struct ssv6xxx_usb_rx_buf *)ssv6xxx_dequeue_list_node(&glue->ssv_rx_queue))) {
        if (glue->is_rx_q_full(glue->rx_cb_args)) {
            //printk("%s(): RX queue is full.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            break;
        }
        (*glue->rx_pkt)++;
        rx_mpdu = glue->p_wlan_data->skb_alloc(glue->p_wlan_data->skb_param, ssv_rx_buf->rx_filled,
                GFP_KERNEL
                );
        if (rx_mpdu == NULL) {
            //printk("%s(): Can't alloc skb.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            queue_work(glue->wq, (struct work_struct *)&glue->rx_work);
            break;
        }

        data = skb_put(rx_mpdu, ssv_rx_buf->rx_filled);
        memcpy(data, ssv_rx_buf->rx_buf, ssv_rx_buf->rx_filled);
        glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        
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
        if (glue->is_rx_q_full(glue->rx_cb_args)) {
            //printk("%s(): RX queue is full.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            tasklet_schedule(&glue->rx_tasklet);
            break;
        }
        (*glue->rx_pkt)++;
        rx_mpdu = glue->p_wlan_data->skb_alloc(glue->p_wlan_data->skb_param, ssv_rx_buf->rx_filled,
                GFP_ATOMIC
                );
        if (rx_mpdu == NULL) {
            //printk("%s(): Can't alloc skb.\n", __func__);
            ssv6xxx_enqueue_list_node((struct ssv6xxx_list_node *)ssv_rx_buf, &glue->ssv_rx_queue);
            tasklet_schedule(&glue->rx_tasklet);
            break;
        }
        data = skb_put(rx_mpdu, ssv_rx_buf->rx_filled);
        memcpy(data, ssv_rx_buf->rx_buf, ssv_rx_buf->rx_filled);
        ssv6xxx_usb_recv_rx(glue, ssv_rx_buf);
        
        glue->rx_cb(rx_mpdu, glue->rx_cb_args);
        
    }
}

static void ssv6xxx_usb_recv_rx_complete(struct urb *urb)
{
    struct ssv6xxx_usb_rx_buf *ssv_rx_buf = (struct ssv6xxx_usb_rx_buf *)urb->context;
    struct ssv6xxx_usb_glue *glue = ssv_rx_buf->glue;
    ssv_rx_buf->rx_res = urb->status;

    if (urb->status) {
        HWIF_DBG_PRINT(glue->p_wlan_data, "fail rx status received:%d\n", urb->status);
       	#if LINUX_VERSION_CODE == KERNEL_VERSION(3,0,8) // for fullhan
		if (urb->status == -2) return;
		#endif
	    goto skip;
    }
    glue->err_cnt = 0;
    ssv_rx_buf->rx_filled = urb->actual_length;

    if (ssv_rx_buf->rx_filled > MAX_USB_RX_AGGR_SIZE) {
        HWIF_DBG_PRINT(glue->p_wlan_data, "recv invalid data length %d\n", ssv_rx_buf->rx_filled);
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
        HWIF_DBG_PRINT(glue->p_wlan_data, "Fail to submit rx urb, error=%d\n", retval);
    }
}

/* 
 * For usb interface,  we make use of ssv6xxx_usb_recv_rx() to receive the rx frame.
 */
static int __must_check ssv6xxx_usb_read(struct device *child,
        void *buf, size_t *size, int mode)
{
    *size = 0;
    return 0;
}

static int ssv6xxx_usb_send_tx(struct ssv6xxx_usb_glue *glue, struct sk_buff *skb, size_t size)
{
#ifndef CONFIG_USB_TX_MULTI_URB
    int foolen = 0, retval = 0;
    int tx_len = size;  
#else
	int retval = 0;
	int tx_len = size;
	struct urb *tx_urb = usb_alloc_urb(0, GFP_ATOMIC);
	struct sk_buff *tx_skb;

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
        HWIF_DBG_PRINT(glue->p_wlan_data, "Cannot send tx data, retval=%d\n", retval);
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
		HWIF_DBG_PRINT(glue->p_wlan_data, "Fail to submit tx urb, error=%d\n", retval);
		kfree_skb(tx_skb);
	}
	usb_free_urb(tx_urb);
#endif

    return retval;
}

static int __must_check ssv6xxx_usb_write(struct device *child,
        void *buf, size_t len, u8 queue_num)
{
    int retval = (-1);
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue))
        return retval;

    /* for debug */
    //ssv6xxx_dump_tx_desc(buf);
    /* use urb to send tx data */
    if ((retval = ssv6xxx_usb_send_tx(glue, (struct sk_buff *)buf, len)) < 0) {
        HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to send tx data\n", __FUNCTION__);
        //if (((glue->err_cnt)++) > 5)
        //  WARN_ON(1);
    } else {
        glue->err_cnt = 0;
    }

    return retval;
}

#ifdef CONFIG_USB_EP0_RW_REGISTER
static int __must_check __ssv6xxx_usb_ep0_read_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
               u32 *buf)
{
    int i, retval = (-1);
    struct ssv6xxx_read_reg_result result;

    if (IS_GLUE_INVALID(glue))
        return retval;

    for (i = 0; i < USB_CMD_SEQUENCE; i++) {
        retval = usb_control_msg(glue->udev, usb_rcvctrlpipe(glue->udev, 0),
           VENDOR_SPECIFIC_REG_RW, (USB_DIR_IN | USB_TYPE_VENDOR),
           cpu_to_le32((addr>>16)&0xffff), cpu_to_le32(addr&0xffff), (void *)&result, sizeof(struct ssv6xxx_read_reg_result), TRANSACTION_TIMEOUT);

        if (retval >= 0) {
            retval = 0;

            //printk("result.value=0x%08x\n", result.value);
            *buf = le32_to_cpu(result.value);
            //if (i != 0) {
                //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Finally succeed to read %x\n", __FUNCTION__, addr);
            //}
            break;
        //} else {
            //HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to read %x.. Retry\n", __FUNCTION__, addr);
        }
    }

    return retval;
}
#endif

static int __must_check __ssv6xxx_usb_read_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
        u32 *buf)
{
    int retval = (-1);
    struct ssv6xxx_read_reg read_reg;
    struct ssv6xxx_read_reg_result result;

    if (IS_GLUE_INVALID(glue))
        return retval;

    memset(&read_reg, 0, sizeof(struct ssv6xxx_read_reg));  
    memset(&result, 0, sizeof(struct ssv6xxx_read_reg_result)); 
    read_reg.addr = cpu_to_le32(addr);
    retval = ssv6xxx_usb_cmd(glue, SSV6200_CMD_READ_REG, &read_reg, sizeof(struct ssv6xxx_read_reg), &result);
    if (!retval) 
        *buf = le32_to_cpu(result.value);
    else { 
        *buf = 0xffffffff;
        HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to read register address %x\n", __FUNCTION__, addr);
    }

    return retval;
}


static int __must_check ssv6xxx_usb_read_reg(struct device *child, u32 addr,
        u32 *buf)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    return __ssv6xxx_usb_read_reg(glue, addr, buf);
}

#ifdef CONFIG_USB_EP0_RW_REGISTER
static int __must_check ssv6xxx_usb_ep0_read_reg(struct device *child, u32 addr,
                                             u32 *buf)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    int retval = -1;

    if (IS_GLUE_INVALID(glue))
        return retval;

    mutex_lock(&glue->cmd_mutex);
    //ssv6xxx_usb_stop_acc(child, 1);
    //ssv6xxx_usb_stop_acc(child, 2);

    retval = __ssv6xxx_usb_ep0_read_reg(glue, addr, buf);

    //ssv6xxx_usb_start_acc(child, 1);
    //ssv6xxx_usb_start_acc(child, 2);
    mutex_unlock(&glue->cmd_mutex);

    return retval;
}
#endif

static int __must_check __ssv6xxx_usb_write_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
        u32 buf)
{
    int retval = (-1);
    struct ssv6xxx_write_reg write_reg;

    if (IS_GLUE_INVALID(glue))
        return retval;

    memset(&write_reg, 0, sizeof(struct ssv6xxx_write_reg));    
    write_reg.addr = cpu_to_le32(addr);
    write_reg.value = cpu_to_le32(buf);
    retval = ssv6xxx_usb_cmd(glue, SSV6200_CMD_WRITE_REG, &write_reg, sizeof(struct ssv6xxx_write_reg), NULL);
    if (retval) 
        HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to write register address %x, value %x\n", __FUNCTION__, addr, buf);

    return retval;
}

#ifdef CONFIG_USB_EP0_RW_REGISTER
static int __must_check __ssv6xxx_usb_ep0_write_reg(struct ssv6xxx_usb_glue *glue, u32 addr,
               u32 buf)
{
    int i, retval = (-1);
    u32 regval = cpu_to_le32(buf);

    if (IS_GLUE_INVALID(glue))
        return retval;

    for (i = 0; i < USB_CMD_SEQUENCE; i++) {
#if 1
        retval = usb_control_msg(glue->udev, usb_sndctrlpipe(glue->udev, 0),
                   VENDOR_SPECIFIC_REG_RW, (USB_DIR_OUT | USB_TYPE_VENDOR),
                   cpu_to_le32((addr>>16)&0xffff), cpu_to_le32(addr&0xffff), NULL, 0, TRANSACTION_TIMEOUT);

        if (retval < 0) {
            HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to write register address %x\n", __FUNCTION__, addr);
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

    return retval;
}
#endif

static int __must_check ssv6xxx_usb_write_reg(struct device *child, u32 addr,
        u32 buf)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    return __ssv6xxx_usb_write_reg(glue, addr, buf);
}

#ifdef CONFIG_USB_EP0_RW_REGISTER
static int __must_check ssv6xxx_usb_ep0_write_reg(struct device *child, u32 addr,
               u32 buf)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    int retval = -1;

    if (IS_GLUE_INVALID(glue))
        return retval;

    mutex_lock(&glue->cmd_mutex);
    //ssv6xxx_usb_stop_acc(child, 1);
    //ssv6xxx_usb_stop_acc(child, 2);

    retval = __ssv6xxx_usb_ep0_write_reg(glue, addr, buf);

    //ssv6xxx_usb_start_acc(child, 1);
    //ssv6xxx_usb_start_acc(child, 2);
    mutex_unlock(&glue->cmd_mutex);

    return retval;
}
#endif

static int __must_check ssv6xxx_usb_burst_read_reg(struct device *child, u32 *addr,
        u32 *buf, u8 reg_amount)
{
    int ret = -1, i;

    //not support
    printk("%s(): Not support atomic burst register reading!\n", __func__);
    WARN_ON(1);
    for (i = 0 ; i < reg_amount ; i++) {
        ret = ssv6xxx_usb_read_reg(child, addr[i], &buf[i]);
        if (ret != 0) {
            printk("%s(): read 0x%08x failed.\n", __func__, addr[i]);
        }
    }

    return -EOPNOTSUPP;
}

#ifdef CONFIG_USB_EP0_RW_REGISTER
static int __must_check ssv6xxx_usb_ep0_burst_read_reg(struct device *child, u32 *addr,
        u32 *buf, u8 reg_amount)
{
    int ret = -1, i;

    //not support
    printk("%s(): Not support atomic burst register reading!\n", __func__);
    WARN_ON(1);
    for (i = 0 ; i < reg_amount ; i++) {
        ret = ssv6xxx_usb_ep0_read_reg(child, addr[i], &buf[i]);
        if (ret != 0) {
            printk("%s(): read 0x%08x failed.\n", __func__, addr[i]);
        }
    }

    return -EOPNOTSUPP;
}

static int __must_check ssv6xxx_usb_ep0_burst_write_reg(struct device *child, u32 *addr,
        u32 *buf, u8 reg_amount)
{
    int ret = -1, i;

    //not support
    printk("%s(): Not support atomic burst register writing!\n", __func__);
    WARN_ON(1);
    for (i = 0 ; i < reg_amount ; i++) {
        ret = ssv6xxx_usb_ep0_write_reg(child, addr[i], buf[i]);
        if (ret != 0) {
            printk("%s(): write 0x%08x failed.\n", __func__, addr[i]);
        }
    }

    return -EOPNOTSUPP;
}
#endif

static int __must_check ssv6xxx_usb_burst_write_reg(struct device *child, u32 *addr,
        u32 *buf, u8 reg_amount)
{
    int ret = -1, i;

    //not support
    printk("%s(): Not support atomic burst register writing!\n", __func__);
    WARN_ON(1);
    for (i = 0 ; i < reg_amount ; i++) {
        ret = ssv6xxx_usb_write_reg(child, addr[i], buf[i]);
        if (ret != 0) {
            printk("%s(): write 0x%08x failed.\n", __func__, addr[i]);
        }
    }

    return -EOPNOTSUPP;
}

static int ssv6xxx_usb_load_firmware(struct device *child, u32 start_addr, u8 *data, int data_length)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    u16 laddr, haddr;
    u32 addr;
    int retval = 0, max_usb_block = 512;
    u8 *pdata;
    int res_length, offset, send_length;

    if (IS_GLUE_INVALID(glue))
        return -1;

    offset = 0;
    pdata = data;
    addr = start_addr;
    res_length = data_length;

    while (offset < data_length) {
        int transfer = min_t(int, res_length, max_usb_block);
        laddr = (addr & 0x0000ffff);
        haddr = (addr >> 16);
        send_length = usb_control_msg(glue->udev, usb_sndctrlpipe(glue->udev, 0),
                VENDOR_SPECIFIC_FW_DOWNLOAD, (USB_DIR_OUT | USB_TYPE_VENDOR),
                laddr, haddr, pdata, transfer, TRANSACTION_TIMEOUT);

        if (send_length < 0) {
            retval = send_length;
            HWIF_DBG_PRINT(glue->p_wlan_data, "Load Firmware Fail, retval=%d, sram=0x%08x\n", retval, (laddr|haddr));
            break;
        }

        addr += transfer;
        pdata += transfer;
        offset += transfer;
        res_length -= transfer;
    }

    return retval;
}

static int ssv6xxx_usb_property(struct device *child)
{
    return SSV_HWIF_CAPABILITY_POLLING | SSV_HWIF_INTERFACE_USB;
}

static int ssv6xxx_chk_usb_speed(struct ssv6xxx_usb_glue *glue)
{
    if (IS_GLUE_INVALID(glue)) {
        return -1;
    }

    return glue->udev->speed;
}

static void ssv6xxx_usb_cleanup(struct device *child)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue)) {
        printk("%s(): glue is invalid!\n", __func__);
        return;
    }

    HWIF_DBG_PRINT(glue->p_wlan_data, "%s(): \n", __FUNCTION__);

    if (ssv_rx_use_wq) {
        cancel_work_sync((struct work_struct *)&glue->rx_work);
    } else {
        tasklet_kill(&glue->rx_tasklet);
    }
}
static void ssv6xxx_usb_get_usb_urb_cnt(struct device *child, int *urb_cnt)
{
#ifdef CONFIG_USB_TX_MULTI_URB
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue)) {
        printk("%s(): glue is invalid!\n", __func__);
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
        int (*is_rx_q_full)(void *args), void *args, u32 *pkt, u32 *isr_cnt, u32 recv_cnt)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    int i;
    int nr_recvbuff = (ssv_usb_rx_nr_recvbuff > SSV_USB_MAX_NR_RECVBUFF)?SSV_USB_MAX_NR_RECVBUFF:((ssv_usb_rx_nr_recvbuff < SSV_USB_MIN_NR_RECVBUFF)?SSV_USB_MIN_NR_RECVBUFF:ssv_usb_rx_nr_recvbuff);

    printk("%s: nr_recvbuff=%d\n", __func__, nr_recvbuff);
    glue->rx_cb = rx_cb;
    glue->rx_cb_args = args;
    glue->is_rx_q_full = is_rx_q_full;
    glue->rx_pkt = pkt;
    for (i = 0 ; i < nr_recvbuff ; ++i) {
        ssv6xxx_usb_recv_rx(glue, &(glue->ssv_rx_buf[i]));
    }
}

static int ssv6xxx_usb_start_acc(struct device *child, u8 epnum)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue)) {
        printk("failed to start usb acc of ep%d\n", epnum);
        return -1;
    }

    //only high speed use USB acc
    if (ssv6xxx_chk_usb_speed(glue) == USB_SPEED_HIGH)
        glue->p_wlan_data->enable_usb_acc(glue->p_wlan_data->usb_param, epnum);

    return 0;
}

static int ssv6xxx_usb_stop_acc(struct device *child, u8 epnum)
{   
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (IS_GLUE_INVALID(glue)) {
        printk("failed to stop usb acc of ep%d\n", epnum);
        return -1;
    }

    //only high speed use USB acc
    if (ssv6xxx_chk_usb_speed(glue) == USB_SPEED_HIGH)
        glue->p_wlan_data->disable_usb_acc(glue->p_wlan_data->usb_param, epnum);

    return 0;
}

static int ssv6xxx_usb_jump_to_rom(struct device *child)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);


    if (IS_GLUE_INVALID(glue)) {
        printk("failed to jump to ROM\n");
        return -1;
    }

    glue->p_wlan_data->jump_to_rom(glue->p_wlan_data->usb_param);
    return 0;
}

static void ssv6xxx_usb_sysplf_reset(struct device *child, u32 addr, u32 value)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    int retval = (-1), rsp_len = 0;
    u16 sequence;
    struct ssv6xxx_write_reg write_reg;

    if (IS_GLUE_INVALID(glue)) 
        return;

    mutex_lock(&glue->cmd_mutex);
    sequence = ssv6xxx_get_cmd_sequence(glue);
    memset(&write_reg, 0, sizeof(struct ssv6xxx_write_reg));    
    write_reg.addr = cpu_to_le32(addr);
    write_reg.value = cpu_to_le32(value);

    /* 
     * Reset sysplf will causes sequence error.
     * It makes use of the ssv6xxx_usb_send_cmd and ssv6xxx_usb_recv_rsp to complete the command process,
     * instead of common api - ssv6xxx_usb_cmd.
     */
    retval = ssv6xxx_usb_send_cmd(glue, SSV6200_CMD_WRITE_REG, sequence, &write_reg, sizeof(struct ssv6xxx_write_reg));
    if (retval)  
        HWIF_DBG_PRINT(glue->p_wlan_data, "%s: Fail to reset sysplf\n", __FUNCTION__);

    retval = ssv6xxx_usb_recv_rsp(glue, SSV6XXX_MAX_RXCMDSZ, &rsp_len);

    mutex_unlock(&glue->cmd_mutex);
}

#ifdef CONFIG_PM
static int ssv_usb_suspend_late(struct device *child)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    int i;

    dev_info(glue->dev, "%s(): start.\n", __FUNCTION__);

    if (!glue)
        return 0;

    /* cancel rx urb */
    for (i = 0 ; i < SSV_USB_MAX_NR_RECVBUFF ; ++i) {
        usb_kill_urb(glue->ssv_rx_buf[i].rx_urb);
    }

    dev_info(glue->dev, "%s(): end.\n", __FUNCTION__);
    return 0;
}

static int ssv_usb_resume_early(struct device *child)
{
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);
    int i;
    int nr_recvbuff = (ssv_usb_rx_nr_recvbuff > SSV_USB_MAX_NR_RECVBUFF)?SSV_USB_MAX_NR_RECVBUFF:((ssv_usb_rx_nr_recvbuff < SSV_USB_MIN_NR_RECVBUFF)?SSV_USB_MIN_NR_RECVBUFF:ssv_usb_rx_nr_recvbuff);

    dev_info(glue->dev, "%s(): start.\n", __FUNCTION__);

    if (!glue)
        return 0;

    /* allocate rx urb */
    for (i = 0 ; i < nr_recvbuff ; ++i) {
        ssv6xxx_usb_recv_rx(glue, &(glue->ssv_rx_buf[i]));
    }

    dev_info(glue->dev, "%s(): end.\n", __FUNCTION__);
    return 0;
}
#endif

static void ssv6xxx_usb_load_fw_post_config_hwif(struct device *child)
{
#ifdef CONFIG_USB_EP0_RW_REGISTER
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (!IS_GLUE_INVALID(glue)) {
        glue->p_wlan_data->ops->readreg = ssv6xxx_usb_ep0_read_reg;
        glue->p_wlan_data->ops->writereg = ssv6xxx_usb_ep0_write_reg;
        glue->p_wlan_data->ops->burst_readreg = ssv6xxx_usb_ep0_burst_read_reg;
        glue->p_wlan_data->ops->burst_writereg = ssv6xxx_usb_ep0_burst_write_reg;
    }
#endif
}

static void ssv6xxx_usb_reverse_config_hwif(struct device *child)
{
#ifdef CONFIG_USB_EP0_RW_REGISTER
    struct ssv6xxx_usb_glue *glue = dev_get_drvdata(child->parent);

    if (!IS_GLUE_INVALID(glue)) {
        glue->p_wlan_data->ops->readreg = ssv6xxx_usb_read_reg;
        glue->p_wlan_data->ops->writereg = ssv6xxx_usb_write_reg;
        glue->p_wlan_data->ops->burst_readreg = ssv6xxx_usb_burst_read_reg;
        glue->p_wlan_data->ops->burst_writereg = ssv6xxx_usb_burst_write_reg;
    }
#endif
}

static struct ssv6xxx_hwif_ops usb_ops =
{
    .read            = ssv6xxx_usb_read,
    .write           = ssv6xxx_usb_write,
    .readreg         = ssv6xxx_usb_read_reg,
    .writereg        = ssv6xxx_usb_write_reg,
#ifdef CONFIG_USB_EP0_RW_REGISTER
    .mcu_readreg   = ssv6xxx_usb_ep0_read_reg,
    .mcu_writereg   = ssv6xxx_usb_ep0_write_reg,
#endif
    .burst_readreg   = ssv6xxx_usb_burst_read_reg,
    .burst_writereg  = ssv6xxx_usb_burst_write_reg,    
    .load_fw         = ssv6xxx_usb_load_firmware,
    .property        = ssv6xxx_usb_property,
    .hwif_rx_task    = ssv6xxx_usb_rx_task,
    .start_usb_acc   = ssv6xxx_usb_start_acc,
    .stop_usb_acc    = ssv6xxx_usb_stop_acc,
    .jump_to_rom     = ssv6xxx_usb_jump_to_rom,
    .sysplf_reset    = ssv6xxx_usb_sysplf_reset, 
#ifdef CONFIG_PM
    .hwif_suspend    = ssv_usb_suspend_late, 
    .hwif_resume     = ssv_usb_resume_early, 
#endif
    .hwif_cleanup    = ssv6xxx_usb_cleanup,
    .get_tx_req_cnt    = ssv6xxx_usb_get_usb_urb_cnt,
    .load_fw_post_config_device = ssv6xxx_usb_load_fw_post_config_hwif,
    .reverse_config_device = ssv6xxx_usb_reverse_config_hwif,
};

static void ssv6xxx_usb_power_on(struct ssv6xxx_platform_data * pdata, struct usb_interface *interface)
{
    if (pdata->is_enabled == true)
        return;

    pdata->is_enabled = true;
}

static void ssv6xxx_usb_power_off(struct ssv6xxx_platform_data * pdata, struct usb_interface *interface)
{
    if (pdata->is_enabled == false)
        return;

    pdata->is_enabled = false;
}


static void _read_chip_id (struct ssv6xxx_usb_glue *glue)
{
    u32 regval;
    int ret;
    u8 _chip_id[SSV6XXX_CHIP_ID_LENGTH];
    u8 *c = _chip_id;
    int i = 0;

    //CHIP ID
    // Chip ID registers should be common to all SSV6xxx devices. So these registers 
    // must not come from ssv6xxx_reg.h but defined somewhere else.
    ret = __ssv6xxx_usb_read_reg(glue, ADR_CHIP_ID_3, &regval);
    *((u32 *)&_chip_id[0]) = __be32_to_cpu(regval);

    if (ret == 0)
        ret = __ssv6xxx_usb_read_reg(glue, ADR_CHIP_ID_2, &regval);
    *((u32 *)&_chip_id[4]) = __be32_to_cpu(regval);

    if (ret == 0)
        ret = __ssv6xxx_usb_read_reg(glue, ADR_CHIP_ID_1, &regval);
    *((u32 *)&_chip_id[8]) = __be32_to_cpu(regval);

    if (ret == 0)
        ret = __ssv6xxx_usb_read_reg(glue, ADR_CHIP_ID_0, &regval);
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


static int ssv_usb_probe(struct usb_interface *interface,
        const struct usb_device_id *id)
{
    struct ssv6xxx_platform_data *pwlan_data;
    struct ssv6xxx_usb_glue *glue;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;
    int i, j;
    int retval = -ENOMEM;
    unsigned int  epnum;

    printk(KERN_INFO "=======================================\n");
    printk(KERN_INFO "==          TURISMO - USB            ==\n");
    printk(KERN_INFO "=======================================\n");
    /* allocate memory for our device state and initialize it */
    glue = kzalloc(sizeof(*glue), GFP_KERNEL);
    if (!glue) {
        dev_err(&interface->dev, "Out of memory\n");
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

    /* INIT RX */
    ssv6xxx_init_queue(&glue->ssv_rx_queue);
    if (ssv_rx_use_wq) {
        glue->rx_work.glue = glue;
        INIT_WORK((struct work_struct *)&glue->rx_work, ssv6xxx_usb_recv_rx_work);
        glue->wq = create_singlethread_workqueue("ssv6xxx_usb_wq");
        if (!glue->wq) {
            dev_err(&interface->dev, "Could not allocate Work Queue\n");
            goto error;
        }
    } else {
        tasklet_init(&glue->rx_tasklet, ssv6xxx_usb_recv_rx_tasklet, (unsigned long)glue);
    }

    /* Tell PM core that we don't need the card to be powered now */
    pwlan_data = &glue->tmp_data;
    memset(pwlan_data, 0, sizeof(struct ssv6xxx_platform_data));
    atomic_set(&pwlan_data->irq_handling, 0);
    glue->dev = &interface->dev;
    /* USB core needs to know usb_device, so get usb_device form usb_interface */
    glue->udev = usb_get_dev(interface_to_usbdev(interface));
    glue->interface = interface;
    glue->dev_ready = true;

    /* Set verdor/product id */
    pwlan_data->vendor = id->idVendor;
    pwlan_data->device = id->idProduct;

    /* Set hwif operation */
    pwlan_data->ops = &usb_ops;

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
    ssv6xxx_usb_power_on(pwlan_data, interface);

    _read_chip_id(glue);

    glue->core = platform_device_alloc(pwlan_data->short_chip_id, -1);
    if (!glue->core) {
        dev_err(glue->dev, "can't allocate platform_device");
        retval = -ENOMEM;
        goto error;
    }

    glue->core->dev.parent = &interface->dev;
    retval = platform_device_add_data(glue->core, pwlan_data, sizeof(*pwlan_data));
    if (retval) {
        dev_err(glue->dev, "can't add platform data\n");
        goto out_dev_put;
    }

    glue->p_wlan_data = glue->core->dev.platform_data;

    /* Initialize ssv6xxx HWIF HAL layer function*/
    if ((retval = HWIF_HAL_INIT(glue)) != 0) {
        goto out_dev_put;
    }

    retval = platform_device_add(glue->core);
    if (retval) {
        dev_err(glue->dev, "can't add platform device\n");
        goto out_dev_put;
    }

#ifdef SSV_SUPPORT_USB_LPM    
    printk("---------- USB LPM capability ---------- \n");
    printk("device supports LPM: %d\n", glue->udev->lpm_capable);
    printk("device can perform USB2 hardware LPM: %d\n", glue->udev->usb2_hw_lpm_capable);
    printk("USB2 (Host) hardware LPM is enabled: %d\n", glue->udev->usb2_hw_lpm_enabled);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,12,2)        
    printk("Userspace allows USB 2.0 LPM to be enabled: %d\n", glue->udev->usb2_hw_lpm_allowed);
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,11,0)        
    printk("device can perform USB2 hardware BESL LPM: %d\n", glue->udev->usb2_hw_lpm_besl_capable);
#endif       
    printk("----------------------------------------\n");
#endif

    return 0;

out_dev_put:
    platform_device_put(glue->core);

error:
    if (glue)
        /* this frees allocated memory */
        kref_put(&glue->kref, ssv6xxx_usb_delete);

    return retval;
}

static void ssv_usb_disconnect(struct usb_interface *interface)
{
    struct ssv6xxx_usb_glue *glue;

    glue = usb_get_intfdata(interface);
    usb_set_intfdata(interface, NULL);
    if (glue) {
        glue->dev_ready = false;

        ssv6xxx_usb_power_off(glue->p_wlan_data, interface);
        printk("platform_device_del \n");
        platform_device_del(glue->core);

        printk("platform_device_put \n");
        platform_device_put(glue->core);
    
#ifdef CONFIG_USB_EP0_RW_REGISTER
        glue->p_wlan_data->ops->readreg = ssv6xxx_usb_read_reg;
        glue->p_wlan_data->ops->writereg = ssv6xxx_usb_write_reg;
        glue->p_wlan_data->ops->burst_readreg = ssv6xxx_usb_burst_read_reg;
        glue->p_wlan_data->ops->burst_writereg = ssv6xxx_usb_burst_write_reg;
#endif
    }
    /* prevent more I/O from starting */
    mutex_lock(&glue->io_mutex);
    glue->interface = NULL;
    mutex_unlock(&glue->io_mutex);

    /* decrement our usage count */
    kref_put(&glue->kref, ssv6xxx_usb_delete);

    dev_info(&interface->dev, "SSV USB is disconnected");
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
    printk(KERN_INFO "ssv6xxx_usb_init\n");
    return usb_register(&ssv_usb_driver);
}

static int ssv_usb_do_device_exit(struct device *d, void *arg)
{
#if 0
    struct usb_interface *intf = to_usb_interface(d);
    struct ssv6xxx_usb_glue *glue = usb_get_intfdata(intf);
    u32 regval;
    int ret;

    if (glue != NULL) {
        //TODO: replace direct address access
        printk(KERN_INFO "ssv_usb_do_device_exit: JUMP to ROM\n");

        /* Jump to ROM for next time load up driver */
        //disable MCU N10
        ret = __ssv6xxx_usb_read_reg(glue, 0xc000001c, &regval);
        if (__ssv6xxx_usb_write_reg(glue, 0xc000001c, (regval & 0xfeffffff)));

        //swicth to boot from ILM mode
        ret = __ssv6xxx_usb_read_reg(glue, 0xc00000ec, &regval);
        if (__ssv6xxx_usb_write_reg(glue, 0xc00000ec, (regval & 0xffffefff)));

        //set IVB to ROM code address: 0x40000
        ret = __ssv6xxx_usb_read_reg(glue, 0xc00000e8, &regval);
        if (__ssv6xxx_usb_write_reg(glue, 0xc00000e8, (regval | 0x00000004)));

        //enable MCU N10
        ret = __ssv6xxx_usb_read_reg(glue, 0xc000001c, &regval);
        if (__ssv6xxx_usb_write_reg(glue, 0xc000001c, (regval | 0x01000000)));
    }    
    msleep(50);
#endif
    return 0;
}

void ssv6xxx_usb_exit(void)
{
    if (driver_for_each_device(&ssv_usb_driver.drvwrap.driver, NULL, NULL, ssv_usb_do_device_exit))
        printk("Cannot find SSV device\n");

    printk(KERN_INFO "ssv6xxx_usb_exit\n");
    usb_deregister(&ssv_usb_driver);
}

EXPORT_SYMBOL(ssv6xxx_usb_init);
EXPORT_SYMBOL(ssv6xxx_usb_exit);

MODULE_LICENSE("GPL");

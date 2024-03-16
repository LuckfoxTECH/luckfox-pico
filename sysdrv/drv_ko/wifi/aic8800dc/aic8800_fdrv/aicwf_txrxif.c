/**
 * aicwf_bus.c
 *
 * bus function declarations
 *
 * Copyright (C) AICSemi 2018-2020
 */

#include <linux/kthread.h>
#include <linux/netdevice.h>
#include <linux/printk.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/completion.h>
#include <linux/semaphore.h>
#include <linux/debugfs.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>
#include "lmac_msg.h"
#include "aicwf_txrxif.h"
#include "rwnx_platform.h"
#include "rwnx_defs.h"
#include "rwnx_msg_rx.h"
#include "rwnx_rx.h"
#include "aicwf_rx_prealloc.h"
#ifdef AICWF_SDIO_SUPPORT
#include "sdio_host.h"
#endif
#include "aic_bsp_export.h"

#ifdef CONFIG_PREALLOC_RX_SKB
void aicwf_rxframe_queue_init_2(struct rx_frame_queue *pq, int max_len)
{
    //int prio;

    memset(pq, 0, offsetof(struct rx_frame_queue, queuelist) + (sizeof(struct list_head)));
    pq->qmax = (u16)max_len;
    INIT_LIST_HEAD(&pq->queuelist);
#if 0
    memset(pq, 0, offsetof(struct rx_frame_queue, queuelist) + (sizeof(struct list_head) * num_prio));
    pq->num_prio = (u16)num_prio;
    pq->qmax = (u16)max_len;

    for (prio = 0; prio < num_prio; prio++) {
        INIT_LIST_HEAD(&pq->queuelist[prio]);
    }
#endif
}

//extern struct aic_sdio_dev *g_sdiodev;
void rxbuff_queue_flush(struct aicwf_rx_priv* rx_priv)
{

    //int prio;
	struct rx_frame_queue *pq = &rx_priv->rxq;
    struct list_head *pos;
    struct list_head *n;
    struct list_head *head;
    struct rx_buff *tempbuf = NULL;

    head = &pq->queuelist;
    list_for_each_safe(pos, n, head) {
        tempbuf = list_entry(pos, struct rx_buff, queue);
        list_del_init(&tempbuf->queue);
#if 0
        rxbuff_free(tempbuf);
#else
        aicwf_prealloc_rxbuff_free(tempbuf, &rx_priv->rxbuff_lock);
#endif
        pq->qcnt--;
    }
}
#endif

int aicwf_bus_init(uint bus_hdrlen, struct device *dev)
{
	int ret = 0;
	struct aicwf_bus *bus_if;

	if (!dev) {
		txrx_err("device not found\n");
		return -1;
	}
	bus_if = dev_get_drvdata(dev);
	bus_if->cmd_buf = kzalloc(CMD_BUF_MAX, GFP_KERNEL);
	if (!bus_if->cmd_buf) {
		ret = -ENOMEM;
		txrx_err("proto_attach failed\n");
		goto fail;
	}
	memset(bus_if->cmd_buf, '\0', CMD_BUF_MAX);

	init_completion(&bus_if->bustx_trgg);
	init_completion(&bus_if->busrx_trgg);
    //new oob feature
    init_completion(&bus_if->busirq_trgg);
#ifdef AICWF_SDIO_SUPPORT
	spin_lock_init(&bus_if->bus_priv.sdio->wslock);//AIDEN test
	bus_if->bustx_thread = kthread_run(sdio_bustx_thread, (void *)bus_if, "aicwf_bustx_thread");
	bus_if->busrx_thread = kthread_run(sdio_busrx_thread, (void *)bus_if->bus_priv.sdio->rx_priv, "aicwf_busrx_thread");
    //new oob feature
#ifdef CONFIG_OOB
    if(bus_if->bus_priv.sdio->oob_enable){
        bus_if->busirq_thread = kthread_run(sdio_busirq_thread, (void *)bus_if->bus_priv.sdio->rx_priv, "aicwf_busirq_thread");
    }
#endif //CONFIG_OOB
#endif
#ifdef AICWF_USB_SUPPORT
	bus_if->bustx_thread = kthread_run(usb_bustx_thread, (void *)bus_if, "aicwf_bustx_thread");
	bus_if->busrx_thread = kthread_run(usb_busrx_thread, (void *)bus_if->bus_priv.usb->rx_priv, "aicwf_busrx_thread");
#endif

	if (IS_ERR(bus_if->bustx_thread)) {
		bus_if->bustx_thread  = NULL;
		txrx_err("aicwf_bustx_thread run fail\n");
		goto fail;
	}

	if (IS_ERR(bus_if->busrx_thread)) {
		bus_if->busrx_thread  = NULL;
		txrx_err("aicwf_bustx_thread run fail\n");
		goto fail;
	}

	return ret;
fail:
	aicwf_bus_deinit(dev);

	return ret;
}

void aicwf_bus_deinit(struct device *dev)
{
	struct aicwf_bus *bus_if;
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *usb;
#endif
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *sdiodev;
#endif

	if (!dev) {
		txrx_err("device not found\n");
		return;
	}
	AICWFDBG(LOGINFO, "%s Enter\r\n", __func__);
	bus_if = dev_get_drvdata(dev);
	aicwf_bus_stop(bus_if);

#ifdef AICWF_USB_SUPPORT
	usb = bus_if->bus_priv.usb;
	if (g_rwnx_plat->enabled)
		rwnx_platform_deinit(usb->rwnx_hw);
#endif
#ifdef AICWF_SDIO_SUPPORT
	sdiodev = bus_if->bus_priv.sdio;
	if (g_rwnx_plat && g_rwnx_plat->enabled) {
		rwnx_platform_deinit(sdiodev->rwnx_hw);
	}
#endif

	if (bus_if->cmd_buf) {
		kfree(bus_if->cmd_buf);
		bus_if->cmd_buf = NULL;
	}

	if (bus_if->bustx_thread) {
		complete_all(&bus_if->bustx_trgg);
		kthread_stop(bus_if->bustx_thread);
		bus_if->bustx_thread = NULL;
	}
	AICWFDBG(LOGINFO, "%s Exit\r\n", __func__);
}

void aicwf_frame_tx(void *dev, struct sk_buff *skb)
{
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *sdiodev = (struct aic_sdio_dev *)dev;
	aicwf_bus_txdata(sdiodev->bus_if, skb);
#else
	struct aic_usb_dev *usbdev = (struct aic_usb_dev *)dev;

	if (!usbdev->state) {
		txrx_err("down\n");
		aicwf_usb_tx_flowctrl(usbdev->rwnx_hw, true);
		dev_kfree_skb(skb);
		return;
	}
	aicwf_bus_txdata(usbdev->bus_if, skb);
#endif
}

struct aicwf_tx_priv *aicwf_tx_init(void *arg)
{
	struct aicwf_tx_priv *tx_priv;

	tx_priv = kzalloc(sizeof(struct aicwf_tx_priv), GFP_KERNEL);
	if (!tx_priv)
		return NULL;

#ifdef AICWF_SDIO_SUPPORT
	tx_priv->sdiodev = (struct aic_sdio_dev *)arg;
#else
	tx_priv->usbdev = (struct aic_usb_dev *)arg;
#endif

	atomic_set(&tx_priv->aggr_count, 0);
#ifdef  CONFIG_RESV_MEM_SUPPORT
	tx_priv->aggr_buf = aicbsp_resv_mem_alloc_skb(MAX_AGGR_TXPKT_LEN, AIC_RESV_MEM_TXDATA);
#else
	tx_priv->aggr_buf = dev_alloc_skb(MAX_AGGR_TXPKT_LEN);
#endif
	if (!tx_priv->aggr_buf) {
		txrx_err("Alloc bus->txdata_buf failed!\n");
		kfree(tx_priv);
		return NULL;
	}
	tx_priv->head = tx_priv->aggr_buf->data;
	tx_priv->tail = tx_priv->aggr_buf->data;

	return tx_priv;
}

void aicwf_tx_deinit(struct aicwf_tx_priv *tx_priv)
{
	if (tx_priv && tx_priv->aggr_buf) {
#ifdef  CONFIG_RESV_MEM_SUPPORT
		aicbsp_resv_mem_kfree_skb(tx_priv->aggr_buf, AIC_RESV_MEM_TXDATA);
#else
		dev_kfree_skb(tx_priv->aggr_buf);
#endif
		kfree(tx_priv);
	}
}

#ifdef AICWF_SDIO_SUPPORT
#ifdef CONFIG_PREALLOC_RX_SKB
static bool aicwf_another_ptk_1(struct rx_buff *buffer)
{
    u8 *read = buffer->read;
    u16 aggr_len = 0;

    BUG_ON((read - buffer->start)%4 != 0);

    if(read == NULL || read >= buffer->end) {
        return false;
    }

    aggr_len = (*read | (*(read + 1) << 8));
    if(aggr_len == 0) {
        return false;
    }

    return true;
}
#else
static bool aicwf_another_ptk(struct sk_buff *skb)
{
	u8 *data;
	u16 aggr_len = 0;

	if (skb->data == NULL || skb->len == 0) {
		return false;
	}
	data = skb->data;
	aggr_len = (*skb->data | (*(skb->data + 1) << 8));
	if (aggr_len == 0) {
		return false;
	}

	return true;
}
#endif
#endif

int aicwf_process_rxframes(struct aicwf_rx_priv *rx_priv)
{
#ifdef AICWF_SDIO_SUPPORT
	int ret = 0;
	unsigned long flags = 0;
#ifndef CONFIG_PREALLOC_RX_SKB
	struct sk_buff *skb = NULL;
#endif
	u16 pkt_len = 0;
	struct sk_buff *skb_inblock = NULL;
	u16 aggr_len = 0, adjust_len = 0;
	u8 *data = NULL;
	u8_l *msg = NULL;
 #ifdef CONFIG_PREALLOC_RX_SKB
	struct rx_buff *buffer = NULL;

	while (1) {
		spin_lock_irqsave(&rx_priv->rxqlock, flags);
		if (!rx_priv->rxq.qcnt) {
			spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
			break;
		}
		buffer = rxbuff_dequeue(&rx_priv->rxq);
		spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
		if (buffer == NULL) {
			txrx_err("skb_error\r\n");
			break;
		}
		while (aicwf_another_ptk_1(buffer)) {
			data = buffer->read;
			pkt_len = (*data | (*(data + 1) << 8));

			if ((data[2] & SDIO_TYPE_CFG) != SDIO_TYPE_CFG) { // type : data
				aggr_len = pkt_len + RX_HWHRD_LEN;

				if (aggr_len & (RX_ALIGNMENT - 1))
					adjust_len = roundup(aggr_len, RX_ALIGNMENT);
				else
					adjust_len = aggr_len;

				skb_inblock = __dev_alloc_skb(aggr_len + CCMP_OR_WEP_INFO, GFP_KERNEL);
				if (skb_inblock == NULL) {
					txrx_err("no more space! skip\n");
					buffer->read = buffer->read + adjust_len;
					continue;
				}

				skb_put(skb_inblock, aggr_len);
				memcpy(skb_inblock->data, data, aggr_len);
				rwnx_rxdataind_aicwf(rx_priv->sdiodev->rwnx_hw, skb_inblock, (void *)rx_priv);
				buffer->read = buffer->read + adjust_len;
			} else {
				//  type : config
				aggr_len = pkt_len;

				if (aggr_len & (RX_ALIGNMENT - 1))
					adjust_len = roundup(aggr_len, RX_ALIGNMENT);
				else
					adjust_len = aggr_len;

				msg = kmalloc(aggr_len+4, GFP_KERNEL);
				if (msg == NULL) {
					txrx_err("no more space for msg!\n");
					aicwf_prealloc_rxbuff_free(buffer, &rx_priv->rxbuff_lock);
					return -EBADE;
				}

				memcpy(msg, data, aggr_len + 4);
				if (((*(msg + 2) & 0x7f) == SDIO_TYPE_CFG_CMD_RSP) && (rx_priv->sdiodev->bus_if->state != BUS_DOWN_ST))
					rwnx_rx_handle_msg(rx_priv->sdiodev->rwnx_hw, (struct ipc_e2a_msg *)(msg + 4));

				if ((*(msg + 2) & 0x7f) == SDIO_TYPE_CFG_DATA_CFM)
					aicwf_sdio_host_tx_cfm_handler(&(rx_priv->sdiodev->rwnx_hw->sdio_env), (u32 *)(msg + 4));

				if ((*(msg + 2) & 0x7f) == SDIO_TYPE_CFG_PRINT)
					rwnx_rx_handle_print(rx_priv->sdiodev->rwnx_hw, msg + 4, aggr_len);

				buffer->read = buffer->read + (adjust_len + 4);
				kfree(msg);
			}
		}

		aicwf_prealloc_rxbuff_free(buffer, &rx_priv->rxbuff_lock);

		atomic_dec(&rx_priv->rx_cnt);
	}

	#else

	while (1) {
		spin_lock_irqsave(&rx_priv->rxqlock, flags);
		if (aicwf_is_framequeue_empty(&rx_priv->rxq)) {
			spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
			break;
		}
		skb = aicwf_frame_dequeue(&rx_priv->rxq);
		spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
		if (skb == NULL) {
			txrx_err("skb_error\r\n");
			break;
		}
		while (aicwf_another_ptk(skb)) {
			data = skb->data;
			pkt_len = (*skb->data | (*(skb->data + 1) << 8));

			if ((skb->data[2] & SDIO_TYPE_CFG) != SDIO_TYPE_CFG) { // type : data
				aggr_len = pkt_len + RX_HWHRD_LEN;

				if (aggr_len & (RX_ALIGNMENT - 1))
					adjust_len = roundup(aggr_len, RX_ALIGNMENT);
				else
					adjust_len = aggr_len;

				skb_inblock = __dev_alloc_skb(aggr_len + CCMP_OR_WEP_INFO, GFP_KERNEL);
				if (skb_inblock == NULL) {
					txrx_err("no more space! skip\n");
					skb_pull(skb, adjust_len);
					continue;
				}

				skb_put(skb_inblock, aggr_len);
				memcpy(skb_inblock->data, data, aggr_len);
				rwnx_rxdataind_aicwf(rx_priv->sdiodev->rwnx_hw, skb_inblock, (void *)rx_priv);
				skb_pull(skb, adjust_len);
			} else {
				//  type : config
				aggr_len = pkt_len;

				if (aggr_len & (RX_ALIGNMENT - 1))
					adjust_len = roundup(aggr_len, RX_ALIGNMENT);
				else
					adjust_len = aggr_len;

				msg = kmalloc(aggr_len+4, GFP_KERNEL);
				if (msg == NULL) {
					txrx_err("no more space for msg!\n");
					aicwf_dev_skb_free(skb);
					return -EBADE;
				}

				memcpy(msg, data, aggr_len + 4);
				if (((*(msg + 2) & 0x7f) == SDIO_TYPE_CFG_CMD_RSP) && (rx_priv->sdiodev->bus_if->state != BUS_DOWN_ST))
					rwnx_rx_handle_msg(rx_priv->sdiodev->rwnx_hw, (struct ipc_e2a_msg *)(msg + 4));

				if ((*(msg + 2) & 0x7f) == SDIO_TYPE_CFG_DATA_CFM)
					aicwf_sdio_host_tx_cfm_handler(&(rx_priv->sdiodev->rwnx_hw->sdio_env), (u32 *)(msg + 4));

				if ((*(msg + 2) & 0x7f) == SDIO_TYPE_CFG_PRINT)
					rwnx_rx_handle_print(rx_priv->sdiodev->rwnx_hw, msg + 4, aggr_len);

				skb_pull(skb, adjust_len+4);
				kfree(msg);
			}
		}

		dev_kfree_skb(skb);
		atomic_dec(&rx_priv->rx_cnt);
	}
	#endif

    #if defined(CONFIG_SDIO_PWRCTRL)
	aicwf_sdio_pwr_stctl(rx_priv->sdiodev, SDIO_ACTIVE_ST);
    #endif

	return ret;
#else //AICWF_USB_SUPPORT
	int ret = 0;
	unsigned long flags = 0;
	struct sk_buff *skb = NULL; /* Packet for event or data frames */
	u16 pkt_len = 0;
	struct sk_buff *skb_inblock = NULL;
	u16 aggr_len = 0, adjust_len = 0;
	u8 *data = NULL;
	u8_l *msg = NULL;

	while (1) {
		spin_lock_irqsave(&rx_priv->rxqlock, flags);
		if (aicwf_is_framequeue_empty(&rx_priv->rxq)) {
			usb_info("no more rxdata\n");
			spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
			break;
		}
		skb = aicwf_frame_dequeue(&rx_priv->rxq);
		spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
		if (skb == NULL) {
			txrx_err("skb_error\r\n");
			break;
		}
		data = skb->data;
		pkt_len = (*skb->data | (*(skb->data + 1) << 8));
		//printk("p:%d, s:%d , %x\n", pkt_len, skb->len, data[2]);
		if (pkt_len > 1600) {
			dev_kfree_skb(skb);
			atomic_dec(&rx_priv->rx_cnt);
				continue;
		}

		if ((skb->data[2] & USB_TYPE_CFG) != USB_TYPE_CFG) { // type : data
			aggr_len = pkt_len + RX_HWHRD_LEN;
			if (aggr_len & (RX_ALIGNMENT - 1))
				adjust_len = roundup(aggr_len, RX_ALIGNMENT);
			else
				adjust_len = aggr_len;

			skb_inblock = __dev_alloc_skb(aggr_len + CCMP_OR_WEP_INFO, GFP_KERNEL);//8 is for ccmp mic or wep icv
			if (skb_inblock == NULL) {
				txrx_err("no more space! skip!\n");
				skb_pull(skb, adjust_len);
				continue;
			}

			skb_put(skb_inblock, aggr_len);
			memcpy(skb_inblock->data, data, aggr_len);
			rwnx_rxdataind_aicwf(rx_priv->usbdev->rwnx_hw, skb_inblock, (void *)rx_priv);
			///TODO: here need to add rx data process

			skb_pull(skb, adjust_len);
		} else { //  type : config
			aggr_len = pkt_len;
			if (aggr_len & (RX_ALIGNMENT - 1))
				adjust_len = roundup(aggr_len, RX_ALIGNMENT);
			else
				adjust_len = aggr_len;

			msg = kmalloc(aggr_len+4, GFP_KERNEL);
			if (msg == NULL) {
				txrx_err("no more space for msg!\n");
				aicwf_dev_skb_free(skb);
				return -EBADE;
			}
			memcpy(msg, data, aggr_len + 4);
			if ((*(msg + 2) & 0x7f) == USB_TYPE_CFG_CMD_RSP)
				rwnx_rx_handle_msg(rx_priv->usbdev->rwnx_hw, (struct ipc_e2a_msg *)(msg + 4));

			if ((*(msg + 2) & 0x7f) == USB_TYPE_CFG_DATA_CFM)
				aicwf_usb_host_tx_cfm_handler(&(rx_priv->usbdev->rwnx_hw->usb_env), (u32 *)(msg + 4));
			skb_pull(skb, adjust_len + 4);
			kfree(msg);
		}

		dev_kfree_skb(skb);
		atomic_dec(&rx_priv->rx_cnt);
	}

	return ret;
#endif //AICWF_SDIO_SUPPORT
}

static struct recv_msdu *aicwf_rxframe_queue_init(struct list_head *q, int qsize)
{
	int i;
	struct recv_msdu *req, *reqs;

	reqs = vmalloc(qsize*sizeof(struct recv_msdu));
	if (reqs == NULL)
		return NULL;

	req = reqs;
	for (i = 0; i < qsize; i++) {
		INIT_LIST_HEAD(&req->rxframe_list);
		list_add(&req->rxframe_list, q);
		req++;
	}

	return reqs;
}

struct aicwf_rx_priv *aicwf_rx_init(void *arg)
{
	struct aicwf_rx_priv *rx_priv;
	rx_priv = kzalloc(sizeof(struct aicwf_rx_priv), GFP_KERNEL);
	if (!rx_priv)
		return NULL;

#ifdef AICWF_SDIO_SUPPORT
	rx_priv->sdiodev = (struct aic_sdio_dev *)arg;
#else
	rx_priv->usbdev = (struct aic_usb_dev *)arg;
#endif

	#ifdef CONFIG_PREALLOC_RX_SKB
	aicwf_rxframe_queue_init_2(&rx_priv->rxq, MAX_RXQLEN);
	#else
	aicwf_frame_queue_init(&rx_priv->rxq, 1, MAX_RXQLEN);
	#endif
	spin_lock_init(&rx_priv->rxqlock);
	#ifdef CONFIG_PREALLOC_RX_SKB
	spin_lock_init(&rx_priv->rxbuff_lock);
	aicwf_prealloc_init();
	#endif
	atomic_set(&rx_priv->rx_cnt, 0);

#ifdef AICWF_RX_REORDER
	INIT_LIST_HEAD(&rx_priv->rxframes_freequeue);
	spin_lock_init(&rx_priv->freeq_lock);
	rx_priv->recv_frames = aicwf_rxframe_queue_init(&rx_priv->rxframes_freequeue, MAX_REORD_RXFRAME);
	if (!rx_priv->recv_frames) {
		txrx_err("no enough buffer for free recv frame queue!\n");
		kfree(rx_priv);
		return NULL;
	}
	spin_lock_init(&rx_priv->stas_reord_lock);
	INIT_LIST_HEAD(&rx_priv->stas_reord_list);
#endif

	return rx_priv;
}


static void aicwf_recvframe_queue_deinit(struct list_head *q)
{
	struct recv_msdu *req, *next;

	list_for_each_entry_safe(req, next, q, rxframe_list) {
		list_del_init(&req->rxframe_list);
	}
}

void aicwf_rx_deinit(struct aicwf_rx_priv *rx_priv)
{
#ifdef AICWF_RX_REORDER
	struct reord_ctrl_info *reord_info, *tmp;

	AICWFDBG(LOGINFO, "%s\n", __func__);

	spin_lock_bh(&rx_priv->stas_reord_lock);
	list_for_each_entry_safe(reord_info, tmp,
		&rx_priv->stas_reord_list, list) {
		reord_deinit_sta(rx_priv, reord_info);
	}
	spin_unlock_bh(&rx_priv->stas_reord_lock);
#endif

#ifdef AICWF_SDIO_SUPPORT
	AICWFDBG(LOGINFO, "sdio rx thread\n");
	if (rx_priv->sdiodev->bus_if->busrx_thread) {
		complete_all(&rx_priv->sdiodev->bus_if->busrx_trgg);
		kthread_stop(rx_priv->sdiodev->bus_if->busrx_thread);
		rx_priv->sdiodev->bus_if->busrx_thread = NULL;
	}
#ifdef CONFIG_OOB
    if(rx_priv->sdiodev->oob_enable){
        //new oob feature
        if (rx_priv->sdiodev->bus_if->busirq_thread) {
            complete_all(&rx_priv->sdiodev->bus_if->busirq_trgg);
            kthread_stop(rx_priv->sdiodev->bus_if->busirq_thread);
            rx_priv->sdiodev->bus_if->busirq_thread = NULL;
        }
    }
#endif //CONFIG_OOB
#endif
#ifdef AICWF_USB_SUPPORT
	if (rx_priv->usbdev->bus_if->busrx_thread) {
		complete_all(&rx_priv->usbdev->bus_if->busrx_trgg);
		kthread_stop(rx_priv->usbdev->bus_if->busrx_thread);
		rx_priv->usbdev->bus_if->busrx_thread = NULL;
	}
#endif

	#ifdef CONFIG_PREALLOC_RX_SKB
	rxbuff_queue_flush(rx_priv);
	#else
	aicwf_frame_queue_flush(&rx_priv->rxq);
	#endif

#ifdef AICWF_RX_REORDER
	aicwf_recvframe_queue_deinit(&rx_priv->rxframes_freequeue);
	if (rx_priv->recv_frames)
		vfree(rx_priv->recv_frames);
#endif

	#ifdef CONFIG_PREALLOC_RX_SKB
	aicwf_prealloc_exit();
	#endif
	kfree(rx_priv);

	AICWFDBG(LOGINFO, "%s exit \n", __func__);
}

bool aicwf_rxframe_enqueue(struct device *dev, struct frame_queue *q, struct sk_buff *pkt)
{
	return aicwf_frame_enq(dev, q, pkt, 0);
}


void aicwf_dev_skb_free(struct sk_buff *skb)
{
	if (!skb)
		return;

	dev_kfree_skb_any(skb);
}

static struct sk_buff *aicwf_frame_queue_penq(struct frame_queue *pq, int prio, struct sk_buff *p)
{
	struct sk_buff_head *q;

	if (pq->queuelist[prio].qlen >= pq->qmax)
		return NULL;

	q = &pq->queuelist[prio];
	__skb_queue_tail(q, p);
	pq->qcnt++;
	if (pq->hi_prio < prio)
		pq->hi_prio = (u16)prio;

	return p;
}

void aicwf_frame_queue_flush(struct frame_queue *pq)
{
	int prio;
	struct sk_buff_head *q;
	struct sk_buff *p, *next;

	for (prio = 0; prio < pq->num_prio; prio++) {
		q = &pq->queuelist[prio];
		skb_queue_walk_safe(q, p, next) {
			skb_unlink(p, q);
			aicwf_dev_skb_free(p);
			pq->qcnt--;
		}
	}
}

void aicwf_frame_queue_init(struct frame_queue *pq, int num_prio, int max_len)
{
	int prio;

	memset(pq, 0, offsetof(struct frame_queue, queuelist) + (sizeof(struct sk_buff_head) * num_prio));
	pq->num_prio = (u16)num_prio;
	pq->qmax = (u16)max_len;

	for (prio = 0; prio < num_prio; prio++) {
		skb_queue_head_init(&pq->queuelist[prio]);
	}
}

struct sk_buff *aicwf_frame_queue_peek_tail(struct frame_queue *pq, int *prio_out)
{
	int prio;

	if (pq->qcnt == 0)
		return NULL;

	for (prio = 0; prio < pq->hi_prio; prio++)
		if (!skb_queue_empty(&pq->queuelist[prio]))
			break;

	if (prio_out)
		*prio_out = prio;

	return skb_peek_tail(&pq->queuelist[prio]);
}

bool aicwf_is_framequeue_empty(struct frame_queue *pq)
{
	int prio, len = 0;

	for (prio = 0; prio <= pq->hi_prio; prio++)
		len += pq->queuelist[prio].qlen;

	if (len > 0)
		return false;
	else
		return true;
}

struct sk_buff *aicwf_frame_dequeue(struct frame_queue *pq)
{
	struct sk_buff_head *q;
	struct sk_buff *p;
	int prio;

	if (pq->qcnt == 0)
		return NULL;

	while ((prio = pq->hi_prio) > 0 && skb_queue_empty(&pq->queuelist[prio]))
		pq->hi_prio--;

	q = &pq->queuelist[prio];
	p = __skb_dequeue(q);
	if (p == NULL)
		return NULL;

	pq->qcnt--;

	return p;
}
#if 0
static struct sk_buff *aicwf_skb_dequeue_tail(struct frame_queue *pq, int prio)
{
	struct sk_buff_head *q = &pq->queuelist[prio];
	struct sk_buff *p = skb_dequeue_tail(q);

	if (!p)
		return NULL;

	pq->qcnt--;
	return p;
}
#endif

bool aicwf_frame_enq(struct device *dev, struct frame_queue *q, struct sk_buff *pkt, int prio)
{
	#if 0
	struct sk_buff *p = NULL;
	int prio_modified = -1;

	if (q->queuelist[prio].qlen < q->qmax && q->qcnt < q->qmax) {
		aicwf_frame_queue_penq(q, prio, pkt);
		return true;
	}
	if (q->queuelist[prio].qlen >= q->qmax) {
		prio_modified = prio;
	} else if (q->qcnt >= q->qmax) {
		p = aicwf_frame_queue_peek_tail(q, &prio_modified);
		if (prio_modified > prio)
			return false;
	}

	if (prio_modified >= 0) {
		if (prio_modified == prio)
			return false;

		p = aicwf_skb_dequeue_tail(q, prio_modified);
		aicwf_dev_skb_free(p);

		p = aicwf_frame_queue_penq(q, prio_modified, pkt);
		if (p == NULL)
			txrx_err("failed\n");
	}

	return p != NULL;
	#else
	if (q->queuelist[prio].qlen < q->qmax && q->qcnt < q->qmax) {
                aicwf_frame_queue_penq(q, prio, pkt);
                return true;
        } else
		return false;
	#endif
}

#ifdef CONFIG_PREALLOC_RX_SKB
void rxbuff_free(struct rx_buff *rxbuff)
{
   kfree(rxbuff->data);
   kfree(rxbuff);
}

struct rx_buff *rxbuff_queue_penq(struct rx_frame_queue *pq, struct rx_buff *p)
{

    struct list_head *q;
    if (pq->qcnt >= pq->qmax)
        return NULL;

    q = &pq->queuelist;
    list_add_tail(&p->queue,q);

    pq->qcnt++;

    return p;
}

struct rx_buff *rxbuff_dequeue(struct rx_frame_queue *pq)
{
    struct rx_buff *p = NULL;

    if (pq->qcnt == 0) {
        printk("%s %d, rxq is empty\n", __func__, __LINE__);
        return NULL;
    }

    if(list_empty(&pq->queuelist)) {
        printk("%s %d, rxq is empty\n", __func__, __LINE__);
        return NULL;
    } else {
        p = list_first_entry(&pq->queuelist, struct rx_buff, queue);
        list_del_init(&p->queue);
        pq->qcnt--;
    }

    return p;
}

bool aicwf_rxbuff_enqueue(struct device *dev, struct rx_frame_queue *rxq, struct rx_buff *pkt)
{
//    struct rx_buff *p = NULL;

    if ((rxq == NULL) || (pkt == NULL)) {
        printk("%s %d, rxq or pkt is NULL\n", __func__, __LINE__);
        return false;
    }

    if (rxq->qcnt < rxq->qmax) {
        if (rxbuff_queue_penq(rxq, pkt)) {
            return true;
        } else {
            printk("%s %d, rxbuff enqueue fail\n", __func__, __LINE__);
            return false;
        }
    } else {
        printk("%s %d, rxq or pkt is full\n", __func__, __LINE__);
        return false;
    }
}
#endif



/**
 * aicwf_usb.c
 *
 * USB function declarations
 *
 * Copyright (C) AICSemi 2018-2020
 */

#include <linux/usb.h>
#include <linux/kthread.h>
#include "aicwf_txrxif.h"
#include "aicwf_usb.h"
#include "rwnx_tx.h"
#include "rwnx_defs.h"
#include "usb_host.h"
#include "rwnx_platform.h"

void aicwf_usb_tx_flowctrl(struct rwnx_hw *rwnx_hw, bool state)
{
	struct rwnx_vif *rwnx_vif;
	list_for_each_entry(rwnx_vif, &rwnx_hw->vifs, list) {
		if (!rwnx_vif->up)
			continue;
		if (!rwnx_vif->ndev)
			continue;
		if (state)
			netif_stop_queue(rwnx_vif->ndev);
		else
			netif_wake_queue(rwnx_vif->ndev);
	}
}

static struct aicwf_usb_buf *aicwf_usb_tx_dequeue(struct aic_usb_dev *usb_dev,
	struct list_head *q, int *counter, spinlock_t *qlock)
{
	unsigned long flags;
	struct aicwf_usb_buf *usb_buf;

	spin_lock_irqsave(qlock, flags);
	if (list_empty(q)) {
		usb_buf = NULL;
	} else {
		usb_buf = list_first_entry(q, struct aicwf_usb_buf, list);
		list_del_init(&usb_buf->list);
		if (counter)
			(*counter)--;
	}
	spin_unlock_irqrestore(qlock, flags);
	return usb_buf;
}

static void aicwf_usb_tx_queue(struct aic_usb_dev *usb_dev,
	struct list_head *q, struct aicwf_usb_buf *usb_buf, int *counter,
	spinlock_t *qlock)
{
	unsigned long flags;

	spin_lock_irqsave(qlock, flags);
	list_add_tail(&usb_buf->list, q);
	(*counter)++;
	spin_unlock_irqrestore(qlock, flags);
}

static struct aicwf_usb_buf *aicwf_usb_rx_buf_get(struct aic_usb_dev *usb_dev)
{
	unsigned long flags;
	struct aicwf_usb_buf *usb_buf;

	spin_lock_irqsave(&usb_dev->rx_free_lock, flags);
	if (list_empty(&usb_dev->rx_free_list)) {
		usb_buf = NULL;
	} else {
		usb_buf = list_first_entry(&usb_dev->rx_free_list, struct aicwf_usb_buf, list);
		list_del_init(&usb_buf->list);
	}
	spin_unlock_irqrestore(&usb_dev->rx_free_lock, flags);
	return usb_buf;
}

static void aicwf_usb_rx_buf_put(struct aic_usb_dev *usb_dev, struct aicwf_usb_buf *usb_buf)
{
	unsigned long flags;

	spin_lock_irqsave(&usb_dev->rx_free_lock, flags);
	list_add_tail(&usb_buf->list, &usb_dev->rx_free_list);
	spin_unlock_irqrestore(&usb_dev->rx_free_lock, flags);
}

static void aicwf_usb_tx_complete(struct urb *urb)
{
	unsigned long flags;
	struct aicwf_usb_buf *usb_buf = (struct aicwf_usb_buf *) urb->context;
	struct aic_usb_dev *usb_dev = usb_buf->usbdev;
	struct sk_buff *skb;
	u8 *buf;

	if (usb_buf->cfm == false) {
		skb = usb_buf->skb;
	} else {
		buf = (u8 *)usb_buf->skb;
	}

	if (usb_buf->cfm == false) {
		dev_kfree_skb_any(skb);
	} else {
		kfree(buf);
	}
	usb_buf->skb = NULL;

	aicwf_usb_tx_queue(usb_dev, &usb_dev->tx_free_list, usb_buf,
					&usb_dev->tx_free_count, &usb_dev->tx_free_lock);

	spin_lock_irqsave(&usb_dev->tx_flow_lock, flags);
	if (usb_dev->tx_free_count > AICWF_USB_TX_HIGH_WATER) {
		if (usb_dev->tbusy) {
			usb_dev->tbusy = false;
			aicwf_usb_tx_flowctrl(usb_dev->rwnx_hw, false);
		}
	}
	spin_unlock_irqrestore(&usb_dev->tx_flow_lock, flags);
	}

static void aicwf_usb_rx_complete(struct urb *urb)
{
	struct aicwf_usb_buf *usb_buf = (struct aicwf_usb_buf *) urb->context;
	struct aic_usb_dev *usb_dev = usb_buf->usbdev;
	struct aicwf_rx_priv *rx_priv = usb_dev->rx_priv;
	struct sk_buff *skb = NULL;
	unsigned long flags = 0;

	skb = usb_buf->skb;
	usb_buf->skb = NULL;

	if (urb->actual_length > urb->transfer_buffer_length) {
		aicwf_dev_skb_free(skb);
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);
		schedule_work(&usb_dev->rx_urb_work);
		return;
	}

	if (urb->status != 0 || !urb->actual_length) {
		aicwf_dev_skb_free(skb);
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);
		schedule_work(&usb_dev->rx_urb_work);
		return;
	}

	if (usb_dev->state == USB_UP_ST) {
		skb_put(skb, urb->actual_length);

		spin_lock_irqsave(&rx_priv->rxqlock, flags);
		if (!aicwf_rxframe_enqueue(usb_dev->dev, &rx_priv->rxq, skb)) {
			spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
			usb_err("rx_priv->rxq is over flow!!!\n");
			aicwf_dev_skb_free(skb);
			aicwf_usb_rx_buf_put(usb_dev, usb_buf);
			return;
		}
		spin_unlock_irqrestore(&rx_priv->rxqlock, flags);
		atomic_inc(&rx_priv->rx_cnt);
		complete(&rx_priv->usbdev->bus_if->busrx_trgg);
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);

		schedule_work(&usb_dev->rx_urb_work);
	} else {
		aicwf_dev_skb_free(skb);
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);
	}
}

static int aicwf_usb_submit_rx_urb(struct aic_usb_dev *usb_dev,
				struct aicwf_usb_buf *usb_buf)
{
	struct sk_buff *skb;
	int ret;

	if (!usb_buf || !usb_dev)
		return -1;

	if (usb_dev->state != USB_UP_ST) {
		usb_err("usb state is not up!\n");
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);
		return -1;
	}

	skb = __dev_alloc_skb(AICWF_USB_MAX_PKT_SIZE, GFP_KERNEL);
	if (!skb) {
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);
		return -1;
	}

	usb_buf->skb = skb;

	usb_fill_bulk_urb(usb_buf->urb,
		usb_dev->udev,
		usb_dev->bulk_in_pipe,
		skb->data, skb_tailroom(skb), aicwf_usb_rx_complete, usb_buf);

	usb_buf->usbdev = usb_dev;

	usb_anchor_urb(usb_buf->urb, &usb_dev->rx_submitted);
	ret = usb_submit_urb(usb_buf->urb, GFP_ATOMIC);
	if (ret) {
		usb_err("usb submit rx urb fail:%d\n", ret);
		usb_unanchor_urb(usb_buf->urb);
		aicwf_dev_skb_free(usb_buf->skb);
		usb_buf->skb = NULL;
		aicwf_usb_rx_buf_put(usb_dev, usb_buf);

		msleep(100);
	}
	return 0;
}

static void aicwf_usb_rx_submit_all_urb(struct aic_usb_dev *usb_dev)
{
	struct aicwf_usb_buf *usb_buf;

	if (usb_dev->state != USB_UP_ST) {
		usb_err("bus is not up=%d\n", usb_dev->state);
		return;
	}

	while ((usb_buf = aicwf_usb_rx_buf_get(usb_dev)) != NULL) {
		if (aicwf_usb_submit_rx_urb(usb_dev, usb_buf)) {
			usb_err("usb rx refill fail\n");
			if (usb_dev->state != USB_UP_ST)
				return;
		}
	}
}

static void aicwf_usb_rx_prepare(struct aic_usb_dev *usb_dev)
{
	aicwf_usb_rx_submit_all_urb(usb_dev);
}

static void aicwf_usb_tx_prepare(struct aic_usb_dev *usb_dev)
{
	struct aicwf_usb_buf *usb_buf;

	while (!list_empty(&usb_dev->tx_post_list)) {
		usb_buf = aicwf_usb_tx_dequeue(usb_dev, &usb_dev->tx_post_list,
			&usb_dev->tx_post_count, &usb_dev->tx_post_lock);
		if (usb_buf->skb) {
			dev_kfree_skb(usb_buf->skb);
			usb_buf->skb = NULL;
		}
		aicwf_usb_tx_queue(usb_dev, &usb_dev->tx_free_list, usb_buf,
				&usb_dev->tx_free_count, &usb_dev->tx_free_lock);
	}
}
static void aicwf_usb_tx_process(struct aic_usb_dev *usb_dev)
{
	struct aicwf_usb_buf *usb_buf;
	int ret = 0;
	u8 *data = NULL;

	while (!list_empty(&usb_dev->tx_post_list)) {
		if (usb_dev->state != USB_UP_ST) {
			usb_err("usb state is not up!\n");
			return;
		}

		usb_buf = aicwf_usb_tx_dequeue(usb_dev, &usb_dev->tx_post_list,
						&usb_dev->tx_post_count, &usb_dev->tx_post_lock);
		if (!usb_buf) {
			usb_err("can not get usb_buf from tx_post_list!\n");
			return;
		}
		data = usb_buf->skb->data;

		ret = usb_submit_urb(usb_buf->urb, GFP_ATOMIC);
		if (ret) {
			usb_err("aicwf_usb_bus_tx usb_submit_urb FAILED\n");
			goto fail;
		}

		continue;
fail:
		dev_kfree_skb(usb_buf->skb);
		usb_buf->skb = NULL;
		aicwf_usb_tx_queue(usb_dev, &usb_dev->tx_free_list, usb_buf,
					&usb_dev->tx_free_count, &usb_dev->tx_free_lock);
	}
}

int usb_bustx_thread(void *data)
{
	struct aicwf_bus *bus = (struct aicwf_bus *)data;
	struct aic_usb_dev *usbdev = bus->bus_priv.usb;

	while (1) {
		if (kthread_should_stop()) {
			usb_err("usb bustx thread stop\n");
			break;
		}
		if (!wait_for_completion_interruptible(&bus->bustx_trgg)) {
			if (usbdev->bus_if->state == BUS_DOWN_ST)
				continue;
			if (usbdev->tx_post_count > 0)
				aicwf_usb_tx_process(usbdev);
		}
	}

	return 0;
}

int usb_busrx_thread(void *data)
{
	struct aicwf_rx_priv *rx_priv = (struct aicwf_rx_priv *)data;
	struct aicwf_bus *bus_if = rx_priv->usbdev->bus_if;

	while (1) {
		if (kthread_should_stop()) {
			usb_err("usb busrx thread stop\n");
			break;
		}
		if (!wait_for_completion_interruptible(&bus_if->busrx_trgg)) {
			if (bus_if->state == BUS_DOWN_ST)
				continue;
			aicwf_process_rxframes(rx_priv);
		}
	}

	return 0;
}

static void aicwf_usb_send_msg_complete(struct urb *urb)
{
	struct aic_usb_dev *usb_dev = (struct aic_usb_dev *) urb->context;

	usb_dev->msg_finished = true;
	if (waitqueue_active(&usb_dev->msg_wait))
		wake_up(&usb_dev->msg_wait);
}

static int aicwf_usb_bus_txmsg(struct device *dev, u8 *buf, u32 len)
{
	int ret = 0;
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_usb_dev *usb_dev = bus_if->bus_priv.usb;

	if (usb_dev->state != USB_UP_ST)
		return -EIO;

	if (buf == NULL || len == 0 || usb_dev->msg_out_urb == NULL)
		return -EINVAL;

	if (test_and_set_bit(0, &usb_dev->msg_busy)) {
		usb_err("In a control frame option, can't tx!\n");
		return -EIO;
	}

	usb_dev->msg_finished = false;

	usb_fill_bulk_urb(usb_dev->msg_out_urb,
		usb_dev->udev,
		usb_dev->bulk_out_pipe,
		buf, len, (usb_complete_t) aicwf_usb_send_msg_complete, usb_dev);
	usb_dev->msg_out_urb->transfer_flags |= URB_ZERO_PACKET;

	ret = usb_submit_urb(usb_dev->msg_out_urb, GFP_ATOMIC);
	if (ret) {
		usb_err("usb_submit_urb failed %d\n", ret);
		goto exit;
	}

	ret = wait_event_timeout(usb_dev->msg_wait,
		usb_dev->msg_finished, msecs_to_jiffies(CMD_TX_TIMEOUT));
	if (!ret) {
		if (usb_dev->msg_out_urb)
			usb_kill_urb(usb_dev->msg_out_urb);
		usb_err("Txmsg wait timed out\n");
		ret = -EIO;
		goto exit;
	}

	if (usb_dev->msg_finished == false) {
		usb_err("Txmsg timed out\n");
		ret = -ETIMEDOUT;
		goto exit;
	}
exit:
	clear_bit(0, &usb_dev->msg_busy);
	return ret;
}


static void aicwf_usb_free_urb(struct list_head *q, spinlock_t *qlock)
{
	struct aicwf_usb_buf *usb_buf, *tmp;
	unsigned long flags;

	spin_lock_irqsave(qlock, flags);
	list_for_each_entry_safe(usb_buf, tmp, q, list) {
	spin_unlock_irqrestore(qlock, flags);
		if (!usb_buf->urb) {
			usb_err("bad usb_buf\n");
			spin_lock_irqsave(qlock, flags);
			break;
		}
		usb_free_urb(usb_buf->urb);
		list_del_init(&usb_buf->list);
		spin_lock_irqsave(qlock, flags);
	}
	spin_unlock_irqrestore(qlock, flags);
}

static int aicwf_usb_alloc_rx_urb(struct aic_usb_dev *usb_dev)
{
	int i;

	for (i = 0; i < AICWF_USB_RX_URBS; i++) {
		struct aicwf_usb_buf *usb_buf = &usb_dev->usb_rx_buf[i];

		usb_buf->usbdev = usb_dev;
		usb_buf->urb = usb_alloc_urb(0, GFP_KERNEL);
		if (!usb_buf->urb) {
			usb_err("could not allocate rx data urb\n");
			goto err;
		}
		list_add_tail(&usb_buf->list, &usb_dev->rx_free_list);
	}
	return 0;

err:
	aicwf_usb_free_urb(&usb_dev->rx_free_list, &usb_dev->rx_free_lock);
	return -ENOMEM;
}

static int aicwf_usb_alloc_tx_urb(struct aic_usb_dev *usb_dev)
{
	int i;

	for (i = 0; i < AICWF_USB_TX_URBS; i++) {
		struct aicwf_usb_buf *usb_buf = &usb_dev->usb_tx_buf[i];

		usb_buf->usbdev = usb_dev;
		usb_buf->urb = usb_alloc_urb(0, GFP_KERNEL);
		if (!usb_buf->urb) {
			usb_err("could not allocate tx data urb\n");
			goto err;
		}
		list_add_tail(&usb_buf->list, &usb_dev->tx_free_list);
		(usb_dev->tx_free_count)++;
	}
	return 0;

err:
	aicwf_usb_free_urb(&usb_dev->tx_free_list, &usb_dev->tx_free_lock);
	return -ENOMEM;
}


static void aicwf_usb_state_change(struct aic_usb_dev *usb_dev, int state)
{
	int old_state;

	if (usb_dev->state == state)
		return;

	old_state = usb_dev->state;
	usb_dev->state = state;

	if (state == USB_DOWN_ST) {
		usb_dev->bus_if->state = BUS_DOWN_ST;
	}
	if (state == USB_UP_ST) {
		usb_dev->bus_if->state = BUS_UP_ST;
	}
}

static int aicwf_usb_bus_txdata(struct device *dev, struct sk_buff *skb)
{
	u8 *buf;
	u16 buf_len = 0;
	u16 adjust_len = 0;
	struct aicwf_usb_buf *usb_buf;
	int ret = 0;
	unsigned long flags;
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_usb_dev *usb_dev = bus_if->bus_priv.usb;
	struct rwnx_txhdr *txhdr = (struct rwnx_txhdr *)skb->data;
	struct rwnx_hw *rwnx_hw = usb_dev->rwnx_hw;
	u8 usb_header[4];
	u8 adj_buf[4] = {0};
	u16 index = 0;
	bool need_cfm = false;

	if (usb_dev->state != USB_UP_ST) {
		usb_err("usb state is not up!\n");
		kmem_cache_free(rwnx_hw->sw_txhdr_cache, txhdr->sw_hdr);
		dev_kfree_skb_any(skb);
		return -EIO;
	}

	usb_buf = aicwf_usb_tx_dequeue(usb_dev, &usb_dev->tx_free_list,
						&usb_dev->tx_free_count, &usb_dev->tx_free_lock);
	if (!usb_buf) {
		usb_err("free:%d, post:%d\n", usb_dev->tx_free_count, usb_dev->tx_post_count);
		kmem_cache_free(rwnx_hw->sw_txhdr_cache, txhdr->sw_hdr);
		dev_kfree_skb_any(skb);
		ret = -ENOMEM;
		goto flow_ctrl;
	}

	if (txhdr->sw_hdr->need_cfm) {
		need_cfm = true;
		buf = kmalloc(skb->len, GFP_KERNEL);
		index += sizeof(usb_header);
		memcpy(&buf[index], (u8 *)(long)&txhdr->sw_hdr->desc, sizeof(struct txdesc_api));
		index += sizeof(struct txdesc_api);
		memcpy(&buf[index], &skb->data[txhdr->sw_hdr->headroom], skb->len - txhdr->sw_hdr->headroom);
		index += skb->len - txhdr->sw_hdr->headroom;
		buf_len = index;
		if (buf_len & (TX_ALIGNMENT - 1)) {
			adjust_len = roundup(buf_len, TX_ALIGNMENT)-buf_len;
			memcpy(&buf[buf_len], adj_buf, adjust_len);
			buf_len += adjust_len;
		}
		usb_header[0] = ((buf_len) & 0xff);
		usb_header[1] = (((buf_len) >> 8)&0x0f);
		usb_header[2] = 0x01; //data
		usb_header[3] = 0; //reserved
		memcpy(&buf[0], usb_header, sizeof(usb_header));
		usb_buf->skb = (struct sk_buff *)buf;
	} else {
		skb_pull(skb, txhdr->sw_hdr->headroom);
		skb_push(skb, sizeof(struct txdesc_api));
		memcpy(&skb->data[0], (u8 *)(long)&txhdr->sw_hdr->desc, sizeof(struct txdesc_api));
		kmem_cache_free(rwnx_hw->sw_txhdr_cache, txhdr->sw_hdr);

		skb_push(skb, sizeof(usb_header));
		usb_header[0] = ((skb->len) & 0xff);
		usb_header[1] = (((skb->len) >> 8)&0x0f);
		usb_header[2] = 0x01; //data
		usb_header[3] = 0; //reserved
		memcpy(&skb->data[0], usb_header, sizeof(usb_header));

		buf = skb->data;
		buf_len = skb->len;

		usb_buf->skb = skb;
	}
	usb_buf->usbdev = usb_dev;
	if (need_cfm)
		usb_buf->cfm = true;
	else
		usb_buf->cfm = false;
	usb_fill_bulk_urb(usb_buf->urb, usb_dev->udev, usb_dev->bulk_out_pipe,
				buf, buf_len, aicwf_usb_tx_complete, usb_buf);
	usb_buf->urb->transfer_flags |= URB_ZERO_PACKET;

	aicwf_usb_tx_queue(usb_dev, &usb_dev->tx_post_list, usb_buf,
					&usb_dev->tx_post_count, &usb_dev->tx_post_lock);
	complete(&bus_if->bustx_trgg);
	ret = 0;

	flow_ctrl:
	spin_lock_irqsave(&usb_dev->tx_flow_lock, flags);
	if (usb_dev->tx_free_count < AICWF_USB_TX_LOW_WATER) {
		usb_dev->tbusy = true;
		aicwf_usb_tx_flowctrl(usb_dev->rwnx_hw, true);
	}
	spin_unlock_irqrestore(&usb_dev->tx_flow_lock, flags);

	return ret;
}

static int aicwf_usb_bus_start(struct device *dev)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_usb_dev *usb_dev = bus_if->bus_priv.usb;

	if (usb_dev->state == USB_UP_ST)
		return 0;

	aicwf_usb_state_change(usb_dev, USB_UP_ST);
	aicwf_usb_rx_prepare(usb_dev);
	aicwf_usb_tx_prepare(usb_dev);
	return 0;
}

static void aicwf_usb_cancel_all_urbs(struct aic_usb_dev *usb_dev)
{
	struct aicwf_usb_buf *usb_buf, *tmp;
	unsigned long flags;

	if (usb_dev->msg_out_urb)
		usb_kill_urb(usb_dev->msg_out_urb);

	spin_lock_irqsave(&usb_dev->tx_post_lock, flags);
	list_for_each_entry_safe(usb_buf, tmp, &usb_dev->tx_post_list, list) {
		spin_unlock_irqrestore(&usb_dev->tx_post_lock, flags);
		if (!usb_buf->urb) {
			usb_err("bad usb_buf\n");
			spin_lock_irqsave(&usb_dev->tx_post_lock, flags);
			break;
		}
		usb_kill_urb(usb_buf->urb);
		spin_lock_irqsave(&usb_dev->tx_post_lock, flags);
	}
	spin_unlock_irqrestore(&usb_dev->tx_post_lock, flags);

	usb_kill_anchored_urbs(&usb_dev->rx_submitted);
}

static void aicwf_usb_bus_stop(struct device *dev)
{
	struct aicwf_bus *bus_if = dev_get_drvdata(dev);
	struct aic_usb_dev *usb_dev = bus_if->bus_priv.usb;

	usb_dbg("%s\r\n", __func__);
	if (usb_dev == NULL)
		return;

	if (usb_dev->state == USB_DOWN_ST)
		return;

	aicwf_usb_state_change(usb_dev, USB_DOWN_ST);
	aicwf_usb_cancel_all_urbs(usb_dev);
}

static void aicwf_usb_deinit(struct aic_usb_dev *usbdev)
{
	cancel_work_sync(&usbdev->rx_urb_work);
	aicwf_usb_free_urb(&usbdev->rx_free_list, &usbdev->rx_free_lock);
	aicwf_usb_free_urb(&usbdev->tx_free_list, &usbdev->tx_free_lock);
	usb_free_urb(usbdev->msg_out_urb);
}

static void aicwf_usb_rx_urb_work(struct work_struct *work)
{
	struct aic_usb_dev *usb_dev = container_of(work, struct aic_usb_dev, rx_urb_work);

	aicwf_usb_rx_submit_all_urb(usb_dev);
}

static int aicwf_usb_init(struct aic_usb_dev *usb_dev)
{
	int ret = 0;

	usb_dev->tbusy = false;
	usb_dev->state = USB_DOWN_ST;

	init_waitqueue_head(&usb_dev->msg_wait);
	init_usb_anchor(&usb_dev->rx_submitted);

	spin_lock_init(&usb_dev->tx_free_lock);
	spin_lock_init(&usb_dev->tx_post_lock);
	spin_lock_init(&usb_dev->rx_free_lock);
	spin_lock_init(&usb_dev->tx_flow_lock);

	INIT_LIST_HEAD(&usb_dev->rx_free_list);
	INIT_LIST_HEAD(&usb_dev->tx_free_list);
	INIT_LIST_HEAD(&usb_dev->tx_post_list);

	usb_dev->tx_free_count = 0;
	usb_dev->tx_post_count = 0;

	ret =  aicwf_usb_alloc_rx_urb(usb_dev);
	if (ret) {
		goto error;
	}
	ret =  aicwf_usb_alloc_tx_urb(usb_dev);
	if (ret) {
		goto error;
	}


	usb_dev->msg_out_urb = usb_alloc_urb(0, GFP_ATOMIC);
	if (!usb_dev->msg_out_urb) {
		usb_err("usb_alloc_urb (msg out) failed\n");
		ret = ENOMEM;
		goto error;
	}

	INIT_WORK(&usb_dev->rx_urb_work, aicwf_usb_rx_urb_work);

	return ret;
	error:
	usb_err("failed!\n");
	aicwf_usb_deinit(usb_dev);
	return ret;
}


static int aicwf_parse_usb(struct aic_usb_dev *usb_dev, struct usb_interface *interface)
{
	struct usb_interface_descriptor *interface_desc;
	struct usb_host_interface *host_interface;
	struct usb_endpoint_descriptor *endpoint;
	struct usb_device *usb = usb_dev->udev;
	int i, endpoints;
	u8 endpoint_num;
	int ret = 0;

	usb_dev->bulk_in_pipe = 0;
	usb_dev->bulk_out_pipe = 0;

	host_interface = &interface->altsetting[0];
	interface_desc = &host_interface->desc;
	endpoints = interface_desc->bNumEndpoints;

	/* Check device configuration */
	if (usb->descriptor.bNumConfigurations != 1) {
		usb_err("Number of configurations: %d not supported\n",
						usb->descriptor.bNumConfigurations);
		ret = -ENODEV;
		goto exit;
	}

	/* Check deviceclass */
#ifndef CONFIG_USB_BT
	if (usb->descriptor.bDeviceClass != 0x00) {
		usb_err("DeviceClass %d not supported\n",
		usb->descriptor.bDeviceClass);
		ret = -ENODEV;
		goto exit;
	}
#endif

	/* Check interface number */
#ifdef CONFIG_USB_BT
	if (usb->actconfig->desc.bNumInterfaces != 3) {
#else
	if (usb->actconfig->desc.bNumInterfaces != 1) {
#endif
	usb_err("Number of interfaces: %d not supported\n",
		usb->actconfig->desc.bNumInterfaces);
	ret = -ENODEV;
	goto exit;
	}

	if ((interface_desc->bInterfaceClass != USB_CLASS_VENDOR_SPEC) ||
		(interface_desc->bInterfaceSubClass != 0xff) ||
		(interface_desc->bInterfaceProtocol != 0xff)) {
		usb_err("non WLAN interface %d: 0x%x:0x%x:0x%x\n",
			interface_desc->bInterfaceNumber, interface_desc->bInterfaceClass,
			interface_desc->bInterfaceSubClass, interface_desc->bInterfaceProtocol);
		ret = -ENODEV;
		goto exit;
	}

	for (i = 0; i < endpoints; i++) {
		endpoint = &host_interface->endpoint[i].desc;
		endpoint_num = usb_endpoint_num(endpoint);

		if (usb_endpoint_dir_in(endpoint) &&
			usb_endpoint_xfer_bulk(endpoint)) {
			if (!usb_dev->bulk_in_pipe) {
				usb_dev->bulk_in_pipe = usb_rcvbulkpipe(usb, endpoint_num);
			}
		}

		if (usb_endpoint_dir_out(endpoint) &&
			usb_endpoint_xfer_bulk(endpoint)) {
			if (!usb_dev->bulk_out_pipe) {
				usb_dev->bulk_out_pipe = usb_sndbulkpipe(usb, endpoint_num);
			}
		}
	}

	if (usb_dev->bulk_in_pipe == 0) {
		usb_err("No RX (in) Bulk EP found\n");
		ret = -ENODEV;
		goto exit;
	}
	if (usb_dev->bulk_out_pipe == 0) {
		usb_err("No TX (out) Bulk EP found\n");
		ret = -ENODEV;
		goto exit;
	}

	if (usb->speed == USB_SPEED_HIGH)
		printk("Aic high speed USB device detected\n");
	else
		printk("Aic full speed USB device detected\n");

	exit:
	return ret;
}



static struct aicwf_bus_ops aicwf_usb_bus_ops = {
	.start = aicwf_usb_bus_start,
	.stop = aicwf_usb_bus_stop,
	.txdata = aicwf_usb_bus_txdata,
	.txmsg = aicwf_usb_bus_txmsg,
};

static int aicwf_usb_probe(struct usb_interface *intf, const struct usb_device_id *id)
{
	int ret = 0;
	struct usb_device *usb = interface_to_usbdev(intf);
	struct aicwf_bus *bus_if ;
	struct device *dev = NULL;
	struct aicwf_rx_priv *rx_priv = NULL;
	struct aic_usb_dev *usb_dev = NULL;

	usb_dev = kzalloc(sizeof(struct aic_usb_dev), GFP_ATOMIC);
	if (!usb_dev) {
		return -ENOMEM;
	}

	usb_dev->udev = usb;
	usb_dev->dev = &usb->dev;
	usb_set_intfdata(intf, usb_dev);

	ret = aicwf_parse_usb(usb_dev, intf);
	if (ret) {
		usb_err("aicwf_parse_usb err %d\n", ret);
		goto out_free;
	}

	ret = aicwf_usb_init(usb_dev);
	if (ret) {
		usb_err("aicwf_usb_init err %d\n", ret);
		goto out_free;
	}

	bus_if = kzalloc(sizeof(struct aicwf_bus), GFP_ATOMIC);
	if (!bus_if) {
		ret = -ENOMEM;
		goto out_free_usb;
	}

	dev = usb_dev->dev;
	bus_if->dev = dev;
	usb_dev->bus_if = bus_if;
	bus_if->bus_priv.usb = usb_dev;
	dev_set_drvdata(dev, bus_if);

	bus_if->ops = &aicwf_usb_bus_ops;

	rx_priv = aicwf_rx_init(usb_dev);
	if (!rx_priv) {
		txrx_err("rx init failed\n");
		ret = -1;
		goto out_free_bus;
	}
	usb_dev->rx_priv = rx_priv;

	ret = aicwf_bus_init(0, dev);
	if (ret < 0) {
		usb_err("aicwf_bus_init err %d\n", ret);
		goto out_free_bus;
	}

	ret = aicwf_bus_start(bus_if);
	if (ret < 0) {
		usb_err("aicwf_bus_start err %d\n", ret);
		goto out_free_bus;
	}

	aicwf_rwnx_usb_platform_init(usb_dev);
	aicwf_hostif_ready();
	return 0;

out_free_bus:
	aicwf_bus_deinit(dev);
	kfree(bus_if);
out_free_usb:
	aicwf_usb_deinit(usb_dev);
out_free:
	usb_err("failed with errno %d\n", ret);
	kfree(usb_dev);
	usb_set_intfdata(intf, NULL);
	return ret;
}

static void aicwf_usb_disconnect(struct usb_interface *intf)
{
	struct aic_usb_dev *usb_dev =
			(struct aic_usb_dev *) usb_get_intfdata(intf);

	if (!usb_dev)
		return;

	aicwf_bus_deinit(usb_dev->dev);
	aicwf_usb_deinit(usb_dev);
	rwnx_cmd_mgr_deinit(&usb_dev->cmd_mgr);

	if (usb_dev->rx_priv)
		aicwf_rx_deinit(usb_dev->rx_priv);
	kfree(usb_dev->bus_if);
	kfree(usb_dev);
}

static int aicwf_usb_suspend(struct usb_interface *intf, pm_message_t state)
{
	struct aic_usb_dev *usb_dev =
		(struct aic_usb_dev *) usb_get_intfdata(intf);

	aicwf_usb_state_change(usb_dev, USB_SLEEP_ST);
	aicwf_bus_stop(usb_dev->bus_if);
	return 0;
}

static int aicwf_usb_resume(struct usb_interface *intf)
{
	struct aic_usb_dev *usb_dev =
		(struct aic_usb_dev *) usb_get_intfdata(intf);

	if (usb_dev->state == USB_UP_ST)
		return 0;

	aicwf_bus_start(usb_dev->bus_if);
	return 0;
}

static int aicwf_usb_reset_resume(struct usb_interface *intf)
{
	return aicwf_usb_resume(intf);
}

static struct usb_device_id aicwf_usb_id_table[] = {
#ifndef CONFIG_USB_BT
	{USB_DEVICE(USB_VENDOR_ID_AIC, USB_PRODUCT_ID_AIC)},
#else
	{USB_DEVICE_AND_INTERFACE_INFO(USB_VENDOR_ID_AIC, USB_PRODUCT_ID_AIC, 0xff, 0xff, 0xff)},
#endif
	{}
};

MODULE_DEVICE_TABLE(usb, aicwf_usb_id_table);

static struct usb_driver aicwf_usbdrvr = {
	.name = KBUILD_MODNAME,
	.probe = aicwf_usb_probe,
	.disconnect = aicwf_usb_disconnect,
	.id_table = aicwf_usb_id_table,
	.suspend = aicwf_usb_suspend,
	.resume = aicwf_usb_resume,
	.reset_resume = aicwf_usb_reset_resume,
	.supports_autosuspend = 1,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 5, 0)
	.disable_hub_initiated_lpm = 1,
#endif
};

void aicwf_usb_register(void)
{
	if (usb_register(&aicwf_usbdrvr) < 0) {
		usb_err("usb_register failed\n");
	}
}

void aicwf_usb_exit(void)
{
	if (g_rwnx_plat && g_rwnx_plat->enabled)
		rwnx_platform_deinit(g_rwnx_plat->usbdev->rwnx_hw);
	usb_deregister(&aicwf_usbdrvr);
	kfree(g_rwnx_plat);
}

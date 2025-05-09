/*
 *
 *  Generic Bluetooth SDIO driver
 *
 *  Copyright (C) 2007  Cambridge Silicon Radio Ltd.
 *  Copyright (C) 2007  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/skbuff.h>

#include <linux/mmc/host.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/sdio_func.h>

#include <net/bluetooth/bluetooth.h>

#include "aic_btsdio.h"
#include "rwnx_msg_tx.h"

#include <net/bluetooth/hci_core.h>

#define VERSION "0.1"
#if CONFIG_BLUEDROID == 0
struct btsdio_data {
	struct hci_dev *hdev;

	struct work_struct work;

	struct sk_buff_head txq;
};
struct hci_dev *ghdev = NULL;

void bt_data_dump(char *tag, void *data, unsigned long len)
{
	unsigned long i = 0;
	uint8_t *data_ = (uint8_t *)data;

	printk("%s %s len:(%lu)\r\n", __func__, tag, len);

	for (i = 0; i < len; i += 16) {
		printk("%02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
		       data_[0 + i], data_[1 + i], data_[2 + i], data_[3 + i],
		       data_[4 + i], data_[5 + i], data_[6 + i], data_[7 + i],
		       data_[8 + i], data_[9 + i], data_[10 + i], data_[11 + i],
		       data_[12 + i], data_[13 + i], data_[14 + i],
		       data_[15 + i]);
	}
}

static int btsdio_tx_packet(struct btsdio_data *data, struct sk_buff *skb)
{
	int err;

	AICBT_INFO("%s", data->hdev->name);

	/* Prepend Type-A header */
	skb_push(skb, 1);
	skb->data[0] = hci_skb_pkt_type(skb);

	//bt_data_dump("btwrite", skb->data, skb->len);
	err = rwnx_sdio_bt_send_req(g_rwnx_plat->sdiodev->rwnx_hw, skb->len,
				    skb);
	if (err < 0) {
		printk("%s rwnx_sdio_bt_send_req error %d", __func__, err);
		return err;
	}

	data->hdev->stat.byte_tx += skb->len;

	kfree_skb(skb);

	return 0;
}

static void btsdio_work(struct work_struct *work)
{
	struct btsdio_data *data = container_of(work, struct btsdio_data, work);
	struct sk_buff *skb;
	int err;

	AICBT_INFO("%s,%s", data->hdev->name, __func__);

	while ((skb = skb_dequeue(&data->txq))) {
		err = btsdio_tx_packet(data, skb);
		if (err < 0) {
			data->hdev->stat.err_tx++;
			skb_queue_head(&data->txq, skb);
			break;
		}
	}
}

/* Get HCI device by index.
 * Device is held on return. */
struct hci_dev *hci_dev_get(int index)
{
	if (index != 0)
		return NULL;

	return ghdev;
}

int bt_sdio_recv(u8 *data, u32 data_len)
{
	struct sk_buff *skb;
	int type = data[0];
	struct hci_dev *hdev;
	u32 len = data_len;
	int ret = 0;
	hdev = hci_dev_get(0);
	if (!hdev) {
		AICWFDBG(LOGERROR, "%s: Failed to get hci dev[NULL]", __func__);
		return -ENODEV;
	}

	skb = alloc_skb(len - 1, GFP_ATOMIC);
	if (!skb) {
		AICWFDBG(LOGERROR, "alloc skb fail %s \n", __func__);
	}
	memcpy(skb_put(skb, len - 1), (data + 1), len - 1);
	hdev->stat.byte_rx += len;
	hci_skb_pkt_type(skb) = type;
	//if(bt_bypass_event(skb)){
	//kfree_skb(skb);
	///return 0;
	//}
	AICBT_INFO("skb type %d", type);
	//bt_data_dump("bt_skb", skb, skb->len);
	ret = hci_recv_frame(hdev, skb);
	if (ret < 0) {
		AICWFDBG(LOGERROR, "hci_recv_frame fail %d\n", ret);
		hdev->stat.err_rx++;
		kfree_skb(skb);
	}
	return 0;
}

static int btsdio_open(struct hci_dev *hdev)
{
	//struct btsdio_data *data = hci_get_drvdata(hdev);
	int err = 0;

	AICBT_INFO("%s", hdev->name);

	return err;
}

static int btsdio_close(struct hci_dev *hdev)
{
	//struct btsdio_data *data = hci_get_drvdata(hdev);

	AICBT_INFO("%s,%s", hdev->name, __func__);

	return 0;
}

static int btsdio_flush(struct hci_dev *hdev)
{
	struct btsdio_data *data = hci_get_drvdata(hdev);

	AICBT_INFO("%s,%s", hdev->name, __func__);

	skb_queue_purge(&data->txq);

	return 0;
}

static int btsdio_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
{
	struct btsdio_data *data = hci_get_drvdata(hdev);

	AICBT_INFO("%s,%s", hdev->name, __func__);

	switch (hci_skb_pkt_type(skb)) {
	case HCI_COMMAND_PKT:
		hdev->stat.cmd_tx++;
		break;

	case HCI_ACLDATA_PKT:
		hdev->stat.acl_tx++;
		break;

	case HCI_SCODATA_PKT:
		hdev->stat.sco_tx++;
		break;

	default:
		return -EILSEQ;
	}

	skb_queue_tail(&data->txq, skb);

	schedule_work(&data->work);

	return 0;
}

int btsdio_init(void)
{
	struct btsdio_data *data;
	struct hci_dev *hdev;
	int err;

	AICBT_INFO("%s", __func__);

	data = kzalloc(sizeof(struct btsdio_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	INIT_WORK(&data->work, btsdio_work);

	skb_queue_head_init(&data->txq);

	hdev = hci_alloc_dev();
	if (!hdev)
		return -ENOMEM;

	hdev->bus = HCI_SDIO;
	hci_set_drvdata(hdev, data);

	hdev->dev_type = HCI_PRIMARY;

	data->hdev = hdev;

	hdev->open = btsdio_open;
	hdev->close = btsdio_close;
	hdev->flush = btsdio_flush;
	hdev->send = btsdio_send_frame;

	err = hci_register_dev(hdev);
	if (err < 0) {
		hci_free_dev(hdev);
		return err;
	}
	ghdev = hdev;

	return 0;
}

void btsdio_remove(void)
{
	struct btsdio_data *data;
	struct hci_dev *hdev;
	hdev = hci_dev_get(0);
	if (!hdev) {
		AICBT_ERR("%s: Failed to get hci dev[Null]", __func__);
		return;
	}
	data = hci_get_drvdata(hdev);

	AICBT_INFO("btsdio_remove");

	if (!data)
		return;

	kfree(data);

	hci_unregister_dev(hdev);

	hci_free_dev(hdev);
}
#endif

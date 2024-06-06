/**
 * aicwf_usb.h
 *
 * USB function declarations
 *
 * Copyright (C) AICSemi 2018-2020
 */

#ifndef _AICWF_USB_H_
#define _AICWF_USB_H_

#include <linux/usb.h>
#include "rwnx_cmds.h"

#ifdef AICWF_USB_SUPPORT

/* USB Device ID */
#define USB_VENDOR_ID_AIC                0xA69C

#ifndef CONFIG_USB_BT
#define USB_PRODUCT_ID_AIC               0x8800
#else
#define USB_PRODUCT_ID_AIC               0x8801
#endif

#define AICWF_USB_RX_URBS               (200)
#define AICWF_USB_TX_URBS               (100)
#define AICWF_USB_TX_LOW_WATER          (AICWF_USB_TX_URBS/4)
#define AICWF_USB_TX_HIGH_WATER         (AICWF_USB_TX_LOW_WATER*3)
#define AICWF_USB_MAX_PKT_SIZE          (2048)

typedef enum {
	USB_TYPE_DATA         = 0X00,
	USB_TYPE_CFG          = 0X10,
	USB_TYPE_CFG_CMD_RSP  = 0X11,
	USB_TYPE_CFG_DATA_CFM = 0X12
} usb_type;

enum aicwf_usb_state {
	USB_DOWN_ST,
	USB_UP_ST,
	USB_SLEEP_ST
};

struct aicwf_usb_buf {
	struct list_head list;
	struct aic_usb_dev *usbdev;
	struct urb *urb;
	struct sk_buff *skb;
	bool cfm;
};

struct aic_usb_dev {
	struct rwnx_hw *rwnx_hw;
	struct aicwf_bus *bus_if;
	struct usb_device *udev;
	struct device *dev;
	struct aicwf_rx_priv *rx_priv;
	enum aicwf_usb_state state;
	struct rwnx_cmd_mgr cmd_mgr;

	struct usb_anchor rx_submitted;
	struct work_struct rx_urb_work;

	spinlock_t rx_free_lock;
	spinlock_t tx_free_lock;
	spinlock_t tx_post_lock;
	spinlock_t tx_flow_lock;

	struct list_head rx_free_list;
	struct list_head tx_free_list;
	struct list_head tx_post_list;

	uint bulk_in_pipe;
	uint bulk_out_pipe;

	int tx_free_count;
	int tx_post_count;

	struct aicwf_usb_buf usb_tx_buf[AICWF_USB_TX_URBS];
	struct aicwf_usb_buf usb_rx_buf[AICWF_USB_RX_URBS];

	int msg_finished;
	wait_queue_head_t msg_wait;
	ulong msg_busy;
	struct urb *msg_out_urb;

	bool tbusy;
};

extern void aicwf_usb_exit(void);
extern void aicwf_usb_register(void);
extern void aicwf_usb_tx_flowctrl(struct rwnx_hw *rwnx_hw, bool state);
int usb_bustx_thread(void *data);
int usb_busrx_thread(void *data);
extern void aicwf_hostif_ready(void);

#endif /* AICWF_USB_SUPPORT */
#endif /* _AICWF_USB_H_       */

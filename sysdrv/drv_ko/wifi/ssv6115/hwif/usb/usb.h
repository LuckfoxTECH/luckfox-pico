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

#ifndef _USB_DEF_H_
#define _USB_DEF_H_

#include <linux/workqueue.h>
#include <ssv_data_struct.h>

#define FW_START_ADDR				0x00

#define        VENDOR_SPECIFIC_FW_DOWNLOAD             (0xf0)
#define        VENDOR_SPECIFIC_REG_RW                  (0x88)
#define        VENDOR_SPECIFIC_REG_RW_WDATA            (0x99)

/* endpoint number */
#define SSV_EP_CMD               0x01
#define SSV_EP_RSP               0x02
#define SSV_EP_TX                0x03
#define SSV_EP_RX                0x04

/* Define CMD */
#define SSV6200_CMD_WRITE_REG		0x01
#define SSV6200_CMD_READ_REG		0x02

struct ssv6xxx_read_reg_result {
	u32		value;	
}__attribute__ ((packed));

struct ssv6xxx_read_reg {
	u32		addr;
	u32		value;
}__attribute__ ((packed));

struct ssv6xxx_write_reg {
    u32		addr;
    u32		value;
}__attribute__ ((packed));

union ssv6xxx_payload {
	struct ssv6xxx_read_reg		rreg;
	struct ssv6xxx_read_reg_result rreg_res;
	struct ssv6xxx_write_reg	wreg;
};

struct ssv6xxx_cmd_hdr {
	u8		plen;
	u8		cmd;
	u16		seq;
	union ssv6xxx_payload  payload;
}__attribute__ ((packed));

struct ssv6xxx_cmd_endpoint {
	u8	 				address;
	u16					packet_size;
	void 				*buff;
};

struct ssv6xxx_tx_endpoint {
	u8					address;
	u16					packet_size;				
	int					tx_res;
};

struct ssv6xxx_rx_endpoint {
	u8	 				address;
	u16					packet_size;				
};

#define SSV_USB_MAX_NR_RECVBUFF (8)
#define SSV_USB_MIN_NR_RECVBUFF (1)

struct ssv6xxx_usb_rx_buf {
    struct ssv6xxx_list_node node;

    struct ssv6xxx_usb_glue *glue;
    struct urb *rx_urb;
    void *rx_buf;
    unsigned int rx_filled;
    int rx_res;
};

struct ssv6xxx_usb_work_struct {
    struct work_struct work;
    struct ssv6xxx_usb_glue *glue;
};

int ssv6xxx_usb_init(void);
void ssv6xxx_usb_exit(void);
#endif /* _USB_DEF_H_ */

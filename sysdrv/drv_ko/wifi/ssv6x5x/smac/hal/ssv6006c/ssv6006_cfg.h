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

#ifndef __SSV6006_H___
#define __SSV6006_H__

#include <linux/device.h>
#include <linux/interrupt.h>
#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
// Include defines from config.mak to feed eclipse defines from ccflags-y
#ifdef ECLIPSE
#include <ssv_mod_conf.h>
#endif // ECLIPSE

#include <hwif/hwif.h>
#include <hci/ssv_hci.h>
#include <ssv_cfg.h>
#include "ssv6006_common.h"

/* tx page size could not more than 255
 * tx id could not more than 63
 *  TX_ID_ALL_INFO (TX_PAGE_USE_7_0 only 8bits and TX_ID_USE_5_0 only 6bits)
 */

#define SSV6006_TOTAL_PAGE                  (256)
#define SSV6006_TOTAL_ID                    (128)

//#define SSV6006_ID_TX_THRESHOLD 			60//64

// Tx ID setting for generic case and USB WMM case
#define SSV6006_ID_SEC                      1   // Security reserved 1 NO_TYPE ID
#define SSV6006_ID_TX_USB                   1   // USB reserved 1 TX ID
#define SSV6006_ID_TX_THRESHOLD 			(63 - SSV6006_ID_TX_USB) 
#define SSV6006_ID_RX_THRESHOLD 			(SSV6006_TOTAL_ID - (SSV6006_ID_TX_THRESHOLD + SSV6006_ID_TX_USB + SSV6006_ID_SEC))    

// Tx ID settings for USB generic case, it should limit tx id to 17. Otherwise, it might occur USB FIFO full issue and let 
// USB read write fail
#define SSV6006_ID_USB_TX_THRESHOLD     	(17) 
#define SSV6006_ID_USB_RX_THRESHOLD  		(SSV6006_TOTAL_ID - (SSV6006_ID_USB_TX_THRESHOLD + SSV6006_ID_TX_USB + SSV6006_ID_SEC))    

#define SSV6006_USB_FIFO                    (4)
#define SSV6006_RESERVED_SEC_PAGE			(3)
#define SSV6006_RESERVED_USB_PAGE			(4)
#define SSV6006_PAGE_TX_THRESHOLD           (SSV6006_TOTAL_PAGE*3/4)
#define SSV6006_PAGE_RX_THRESHOLD           (SSV6006_TOTAL_PAGE*1/4)

#define SSV6006_AMPDU_DIVIDER                   (2)
#define SSV6006_TX_LOWTHRESHOLD_PAGE_TRIGGER    (SSV6006_PAGE_TX_THRESHOLD - (SSV6006_PAGE_TX_THRESHOLD/SSV6006_AMPDU_DIVIDER))
#define SSV6006_TX_LOWTHRESHOLD_ID_TRIGGER      (SSV6006_ID_TX_THRESHOLD - 1)


#define SSV6006_ID_NUMBER                   (SSV6006_TOTAL_ID)

#define PACKET_ADDR_2_ID(addr)              ((addr >> 16) & 0x7F)

#define SSV6006_ID_AC_RESERVED              1


#define SSV6006_ID_AC_BK_OUT_QUEUE          8
#define SSV6006_ID_AC_BE_OUT_QUEUE          15
#define SSV6006_ID_AC_VI_OUT_QUEUE          16
#define SSV6006_ID_AC_VO_OUT_QUEUE          16
#define SSV6006_ID_MANAGER_QUEUE            8

#define	HW_MMU_PAGE_SHIFT			0x8 //256 bytes
#define	HW_MMU_PAGE_MASK			0xff

//TX_PKT_RSVD(3) * unit(16)
#define SSV6006_TX_PKT_RSVD_SETTING     (0x3)
#define SSV6006_TX_PKT_RSVD             (SSV6006_TX_PKT_RSVD_SETTING * 16)
#define SSV6006_ALLOC_RSVD              (TXPB_OFFSET+SSV6006_TX_PKT_RSVD)

#define SSV6006_BT_PRI_SMP_TIME     0
#define SSV6006_BT_STA_SMP_TIME     (SSV6006_BT_PRI_SMP_TIME+0)
#define SSV6006_WLAN_REMAIN_TIME    0

#endif /* __SSV6006_H__ */


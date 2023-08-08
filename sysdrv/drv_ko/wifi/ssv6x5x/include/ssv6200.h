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

#ifndef _SSV6200_H_
#define _SSV6200_H_

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
#include "ssv6200_common.h"

#define PACKET_ADDR_2_ID(addr)              ((addr >> 16) & 0x7F)


#define SSV6200_ID_AC_BK_OUT_QUEUE          8
#define SSV6200_ID_AC_BE_OUT_QUEUE          15
#define SSV6200_ID_AC_VI_OUT_QUEUE          16
#define SSV6200_ID_AC_VO_OUT_QUEUE          16
#define SSV6200_ID_MANAGER_QUEUE            8

#define	HW_MMU_PAGE_SHIFT			0x8 //256 bytes
#define	HW_MMU_PAGE_MASK			0xff

#endif /* _SSV6200_H_ */


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

#ifndef __OS_DEP_H__
#define __OS_DEP_H__

#include <linux/version.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/delay.h>
#include <linux/slab.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include <linux/printk.h>
#else
#include <linux/kernel.h>
#endif

extern void ssv6xxx_sdio_tuning_set_clk(struct sdio_func *func, u32 sdio_hz);
extern int ssv6xxx_sdio_tuning_set_bit_mode(struct sdio_func *func, int bus_width);

#define OS_WRAPPER_LOG(fmt, arg...)				printk(fmt, ##arg)

#define OS_WRAPPER_MEM_ALLOC(size, flag)			kzalloc(size, flag)
#define OS_WRAPPER_MEM_FREE(p)					kfree(p)

typedef struct sdio_func					OS_WRAPPER_SDIO_FUNC;
typedef struct sdio_device_id					OS_WRAPPER_SDIO_DEVICE_ID;

#define OS_WRAPPER_SDIO_BUS_WIDTH_1				MMC_BUS_WIDTH_1
#define OS_WRAPPER_SDIO_BUS_WIDTH_4				MMC_BUS_WIDTH_4

#define OS_WRAPPER_SDIO_SET_CLK(func, hz)			ssv6xxx_sdio_tuning_set_clk(func, hz)
#define OS_WRAPPER_SDIO_SET_BUS_WIDTH(func, width)		ssv6xxx_sdio_tuning_set_bit_mode(func, width)
#define OS_WRAPPER_SDIO_HOST_LOCK(func)				sdio_claim_host(func)
#define OS_WRAPPER_SDIO_HOST_UNLOCK(func)			sdio_release_host(func)
#define OS_WRAPPER_SDIO_CMD52_RD(func, addr, ret)		sdio_readb(func, addr, ret)
#define OS_WRAPPER_SDIO_CMD52_WR(func, value, addr, ret)	sdio_writeb(func, value, addr, ret)
#define OS_WRAPPER_SDIO_CMD53_RD(func, dst, addr, cnt)		sdio_memcpy_fromio(func, dst, addr, cnt)
#define OS_WRAPPER_SDIO_CMD53_WR(func, addr, src, cnt)		sdio_memcpy_toio(func, addr, src, cnt)
#define OS_WRAPPER_SDIO_ALIGN_SIZE(func, size)			sdio_align_size(func, size)
#define OS_WRAPPER_SDIO_SET_PRIV_DRV_DATA(func, glue)		sdio_set_drvdata(func, glue)

#endif /* __OS_DEP_H__ */

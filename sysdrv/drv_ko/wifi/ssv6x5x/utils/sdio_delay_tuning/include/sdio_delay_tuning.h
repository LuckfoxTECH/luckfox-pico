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

#ifndef __SDIO_DELAY_TUNING_H__
#define __SDIO_DELAY_TUNING_H__

#include "os_dep.h"

#define SSV_VENDOR_ID			(0x3030)
#define SSV_CABRIO_DEVID		(0x3030)

#define LOW_SPEED_SDIO_CLOCK		(12500000)
#define MID_SPEED_SDIO_CLOCK		(25000000)
#define HIGH_SPEED_SDIO_CLOCK		(50000000)

int ssv6xxx_sdio_tuning_start(OS_WRAPPER_SDIO_FUNC *func, const OS_WRAPPER_SDIO_DEVICE_ID *id);
void ssv6xxx_sdio_tuning_cleanup(OS_WRAPPER_SDIO_FUNC *func);

#endif /* __SDIO_DELAY_TUNING_H__ */

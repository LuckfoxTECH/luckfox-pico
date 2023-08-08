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

#ifndef _SDIO_H_
#define _SDIO_H_

#include <linux/workqueue.h>
#include <ssv_data_struct.h>

#define SSV6XXX_INT_RX              0x00000001  //1<<0
/*Workaround solution: We use this interrupt bit for Queue 4(MNG) */
#define SSV6XXX_INT_TX              0x00000002  //1<<1
#define SSV6XXX_INT_SOC             0x00000004  //1<<2
#define SSV6XXX_INT_LOW_EDCA_0      0x00000008  //1<<3
#define SSV6XXX_INT_LOW_EDCA_1      0x00000010  //1<<4
#define SSV6XXX_INT_LOW_EDCA_2      0x00000020  //1<<5
#define SSV6XXX_INT_LOW_EDCA_3      0x00000040  //1<<6
#define SSV6XXX_INT_RESOURCE_LOW    0x00000080  //1<<7

struct ssv6xxx_sdio_work_struct {
    struct work_struct work;
    struct ssv6xxx_sdio_glue *glue;
};

int ssv6xxx_sdio_init(void);
void ssv6xxx_sdio_exit(void);
#endif

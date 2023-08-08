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

#include <linux/version.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/delay.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include <linux/printk.h>
#else
#include <linux/kernel.h>
#endif

void ssv6xxx_sdio_tuning_set_clk(struct sdio_func *func, u32 sdio_hz)
{
    struct mmc_host *host;

    host = func->card->host;
    if (sdio_hz < host->f_min)
        sdio_hz = host->f_min;
    else if (sdio_hz > host->f_max)
        sdio_hz = host->f_max;

    printk("%s: set sdio clk %dHz\n", __FUNCTION__, sdio_hz);
    sdio_claim_host(func);
    host->ios.clock = sdio_hz;
    host->ops->set_ios(host, &host->ios);
    mdelay(20);
    
    sdio_set_block_size(func, 128);
    sdio_release_host(func);
}

int ssv6xxx_sdio_tuning_set_bit_mode(struct sdio_func *func, int bus_width)
{
    int ret = 0;
    struct mmc_host *host;
    struct mmc_card *card;

    host = func->card->host;
    card = func->card;
    
    sdio_claim_host(func);
    /* bus_width: MMC_BUS_WIDTH_1, MMC_BUS_WIDTH_4 */
    func->num = 0;
    sdio_writeb(func, bus_width, SDIO_CCCR_IF, &ret);
    func->num = 1;
    host->ios.bus_width = bus_width;
    host->ops->set_ios(host, &host->ios);

    mdelay(20);
    sdio_release_host(func);
   
    return 0;
}


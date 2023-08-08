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
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/mmc/sdio.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/sdio_ids.h>
#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/slab.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include <linux/printk.h>
#else
#include <linux/kernel.h>
#endif

#include "sdio_delay_tuning.h"

extern int sdio_tuning_more_info;
module_param(sdio_tuning_more_info, int, S_IRUGO|S_IWUSR);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static const struct sdio_device_id ssv6xxx_sdio_tuning_devices[] __devinitconst =
#else
static const struct sdio_device_id ssv6xxx_sdio_tuning_devices[] =
#endif
{
    { SDIO_DEVICE(SSV_VENDOR_ID, SSV_CABRIO_DEVID) },
    {}
};
MODULE_DEVICE_TABLE(sdio, ssv6xxx_sdio_tuning_devices);

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static int __devinit ssv6xxx_sdio_tuning_probe(struct sdio_func *func,
        const struct sdio_device_id *id)
#else
static int ssv6xxx_sdio_tuning_probe(struct sdio_func *func,
        const struct sdio_device_id *id)
#endif
{
    return ssv6xxx_sdio_tuning_start(func, id);
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
static void __devexit ssv6xxx_sdio_tuning_remove(struct sdio_func *func)
#else
static void ssv6xxx_sdio_tuning_remove(struct sdio_func *func)
#endif
{
    return ssv6xxx_sdio_tuning_cleanup(func);
}

struct sdio_driver ssv6xxx_sdio_tuning_driver =
{
    .name		= "SSV6XXX_SDIO_TUNING",
    .id_table	= ssv6xxx_sdio_tuning_devices,
    .probe		= ssv6xxx_sdio_tuning_probe,
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
    .remove		= __devexit_p(ssv6xxx_sdio_tuning_remove),
#else
    .remove     = ssv6xxx_sdio_tuning_remove,
#endif
};
EXPORT_SYMBOL(ssv6xxx_sdio_tuning_driver);

static int __init ssv6xxx_sdio_tuning_init(void)
{
    printk(KERN_INFO "ssv6xxx_sdio_tuning_init\n");
    return sdio_register_driver(&ssv6xxx_sdio_tuning_driver);
}

void ssv6xxx_sdio_tuning_exit(void)
{
    printk(KERN_INFO "ssv6xxx_sdio_tuning_exit\n");
    sdio_unregister_driver(&ssv6xxx_sdio_tuning_driver);
}
module_init(ssv6xxx_sdio_tuning_init);
module_exit(ssv6xxx_sdio_tuning_exit);
MODULE_LICENSE("GPL");

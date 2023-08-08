/*
 * Copyright (c) 2015 iComm Semiconductor Ltd.
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

#include <linux/module.h>

#include <ssv_chip_id.h>
#include <hwif/hal/hwif_hal.h>

MODULE_AUTHOR("iComm Semiconductor Co., Ltd");
MODULE_DESCRIPTION("HWIF HAL Support for SSV6xxx wireless LAN cards.");
MODULE_SUPPORTED_DEVICE("SSV6xxx 802.11n WLAN cards");
MODULE_LICENSE("Dual BSD/GPL");

int ssv6xxx_init_hwif_hal(struct ssv6xxx_platform_data **priv, struct ssv_hwif_hal_ops *hwif_hal_ops)
{
    int ret = 0;
    extern void ssv_attach_hwif_hal_ssv6006(struct ssv6xxx_platform_data **priv, struct ssv_hwif_hal_ops *hwif_hal_ops);
    bool chip_supportted = false;

    // load individual HWIF HAL function
#ifdef SSV_SUPPORT_SSV6006
    if (   strstr((*priv)->chip_id, SSV6006)
        	 || strstr((*priv)->chip_id, SSV6006C)
        	 || strstr((*priv)->chip_id, SSV6006D)) {

        printk(KERN_INFO"Attach SSV6006 family HWIF HAL function  \n");

        ssv_attach_hwif_hal_ssv6006(priv, hwif_hal_ops);
        chip_supportted = true;
    }
#endif    
    if (!chip_supportted) {

        printk(KERN_ERR "Chip \"%s\" is not supported by this driver\n", (*priv)->short_chip_id);
        ret = -EINVAL;
    }

    return ret;
}

EXPORT_SYMBOL(ssv6xxx_init_hwif_hal);


int ssv6xxx_hwif_hal_init(void)
{
    return 0;
}

void ssv6xxx_hwif_hal_exit(void)
{
    return;
}

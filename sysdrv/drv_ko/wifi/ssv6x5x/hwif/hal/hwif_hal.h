/*
 * Copyright (c) 2014 South Silicon Valley Microelectronics Inc.
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

#ifndef _LHAL_H_
#define _LHAL_H_

#include "hwif/hwif.h"

/**
* struct ssv_hwif_hal_ops - the structure for ssv6xxx HWIF HAL function.
*
* This structure is used to register HWIF HAL relation function
* for different chip.
*/
struct ssv_hwif_hal_ops {
#ifdef CONFIG_PM
    // enable SDIO asynchronous interrupt
    void (*enable_sdio_async_interrupt)(struct ssv6xxx_platform_data **priv);
    // disable SDIO asynchronous interrupt
    void (*disable_sdio_async_interrupt)(struct ssv6xxx_platform_data **priv);
#endif
    void (*allow_sdio_reset)(struct ssv6xxx_platform_data **priv, int allow);
};

int ssv6xxx_init_hwif_hal(struct ssv6xxx_platform_data **priv, struct ssv_hwif_hal_ops *hwif_hal_ops);
int ssv6xxx_hwif_hal_init(void);
void ssv6xxx_hwif_hal_exit(void);

#define HWIF_HAL_INIT(glue)                                ssv6xxx_init_hwif_hal((struct ssv6xxx_platform_data **)&glue->core->dev.platform_data, &glue->hwif_hal_ops)

#ifdef CONFIG_PM
#define HWIF_HAL_ENABLE_SDIO_ASYNC_INT(glue)               do { \
                                                           if(glue->hwif_hal_ops.enable_sdio_async_interrupt) \
                                                               glue->hwif_hal_ops.enable_sdio_async_interrupt((struct ssv6xxx_platform_data **)&glue->core->dev.platform_data); \
                                                           else \
                                                               printk("%s(): Not support!!\n", __func__); \
                                                       } while(0)

#define HWIF_HAL_DISABLE_SDIO_ASYNC_INT(glue)              do { \
                                                           if(glue->hwif_hal_ops.disable_sdio_async_interrupt) \
                                                               glue->hwif_hal_ops.disable_sdio_async_interrupt((struct ssv6xxx_platform_data **)&glue->core->dev.platform_data); \
                                                           else \
                                                               printk("%s(): Not support!!\n", __func__); \
                                                       } while(0)
#endif

#define HWIF_HAL_ENABLE_SDIO_RESET(glue)               do { \
                                                           if(glue->hwif_hal_ops.allow_sdio_reset) \
                                                               glue->hwif_hal_ops.allow_sdio_reset((struct ssv6xxx_platform_data **)&glue->core->dev.platform_data, 0x1); \
                                                           else \
                                                               printk("%s(): Not support!!\n", __func__); \
                                                       } while(0)

#define HWIF_HAL_DISABLE_SDIO_RESET(glue)               do { \
                                                           if(glue->hwif_hal_ops.allow_sdio_reset) \
                                                               glue->hwif_hal_ops.allow_sdio_reset((struct ssv6xxx_platform_data **)&glue->core->dev.platform_data, 0x0); \
                                                           else \
                                                               printk("%s(): Not support!!\n", __func__); \
                                                       } while(0)

#endif /* _LHAL_H_ */


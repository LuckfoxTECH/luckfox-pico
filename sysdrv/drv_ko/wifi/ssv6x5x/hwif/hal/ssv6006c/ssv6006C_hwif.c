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

// Include defines from config.mak to feed eclipse defines from ccflags-y
#ifdef ECLIPSE
#include <ssv_mod_conf.h>
#endif // ECLIPSE

#if (defined SSV_SUPPORT_SSV6006)
#include <linux/delay.h>
#include "ssv6006C_hwif.h"
#include "ssv6006c/ssv6006C_reg.h"
#include <hwif/usb/usb.h>
#include <hwif/sdio/sdio_def.h>
#include "hwif/hal/hwif_hal.h"
#include "ssv6006C_priv_hwif_hal.h"

//#ifdef CONFIG_PM
//static struct target_state tstate;
//#endif

static u64 _ssv6006_get_ic_time_tag(struct ssv6xxx_platform_data **priv)
{
    u32 regval;
    u64 ic_time_tag;
    
    ssv_hwif_read_reg(priv, ADR_CHIP_DATE_YYYYMMDD, &regval);
    ic_time_tag = ((u64)regval<<32);
    ssv_hwif_read_reg(priv, ADR_CHIP_DATE_00HHMMSS, &regval);
    ic_time_tag |= (regval);

    return ic_time_tag;
}

#ifdef CONFIG_PM
static void ssv6006_enable_sdio_async_interrupt(struct ssv6xxx_platform_data **priv)
{
    //if (!mutex_trylock(&tstate.ts_mutex)) {
    //    printk("%s(): Async interrupt is already enabled. \n", __func__);
    //}

#if defined(CONFIG_SSV_USE_SDIO_DAT1_AS_INT)
    /**************************************************************/
    /* 1. Use CMD52 to config MCU_NOTIFY_HOST_CFG:                */
    /**************************************************************/
    LHAL_CMD52_REG_SET_BITS(priv, MCU_NOTIFY_HOST_CFG, 0x4, 0);
#else
#if !defined(CONFIG_SSV_USE_GPIO_AS_SDIO_INT)
#define CONFIG_SSV_USE_GPIO_AS_SDIO_INT 16	//By default, use GPIO 16.
#elif (CONFIG_SSV_USE_GPIO_AS_SDIO_INT != 14) && \
	(CONFIG_SSV_USE_GPIO_AS_SDIO_INT != 15) && \
	(CONFIG_SSV_USE_GPIO_AS_SDIO_INT != 16) && \
	(CONFIG_SSV_USE_GPIO_AS_SDIO_INT != 17) && \
	(CONFIG_SSV_USE_GPIO_AS_SDIO_INT != 18) && \
	(CONFIG_SSV_USE_GPIO_AS_SDIO_INT != 19)
#error "CONFIG_SSV_USE_GPIO_AS_SDIO_INT is invalid!"
#endif

#if 0
    /**************************************************************/
    /* ?. Mask tree interrupt #15 to mcu:                       */
    /**************************************************************/
    tstate.int_mask = GET_MASK_TYPMCU_INT_MAP;
    SET_MASK_TYPMCU_INT_MAP((tstate.int_mask | (u32)(1<<15)));
    printk("%s(): Set MASK_TYPMCU_INT_MAP=0x%08x. \n", __func__, GET_MASK_TYPMCU_INT_MAP);
#endif
    /**************************************************************/
    /* 1. Change PAD to manual for GPIO_N:                        */
    /**************************************************************/
    //tstate.gpio_manual_io = GET_MANUAL_IO;
    //SET_MANUAL_IO((tstate.gpio_manual_io | (u32)(1<<CONFIG_SSV_USE_GPIO_AS_SDIO_INT)));
    //printk("%s(): Set MANUAL_IO=0x%08x. \n", __func__, GET_MANUAL_IO);
    /**************************************************************/
    /* 2. Switch GPO_INT to GPIO_N:                               */
    /**************************************************************/
    //tstate.gpio_int_thru_gpio = GET_SEL_GPO_INT;
    //SET_SEL_GPO_INT((tstate.gpio_int_thru_gpio | (u32)(1<<CONFIG_SSV_USE_GPIO_AS_SDIO_INT)));
    //printk("%s(): Set SEL_GPO_INT=0x%08x. \n", __func__, GET_SEL_GPO_INT);
    /**************************************************************/
    /* 3. Set interrupt as active low mode:                       */
    /**************************************************************/
    //tstate.gpio_polarity = GET_GPO_INT_POL;
    //SET_GPO_INT_POL(1);
    //printk("%s(): Set GPO_INT_POL=0x%08x. \n", __func__, GET_GPO_INT_POL);

    /**************************************************************/
    /* 4. Use CMD52 to config MCU_NOTIFY_HOST_CFG:                */
    /**************************************************************/
    //LHAL_CMD52_REG_SET_BITS(priv, MCU_NOTIFY_HOST_CFG, 0x2, 0);

    /**************************************************************/
    /* 5. Unmask #05 of the 2nd layer mask of tree interrupt #15: */
    /**************************************************************/
    //tstate.int_15_submask = GET_MASK_TYPHOST_INT_MAP_15;
    //SET_MASK_TYPHOST_INT_MAP_15((tstate.int_15_submask & ~(u32)(1<<5)));
    //SET_MASK_TYPHOST_INT_MAP_15((tstate.int_15_submask & ~(u32)(1<<4)));
    //printk("%s(): Set MASK_TYPHOST_INT_MAP_15=0x%08x. \n", __func__, GET_MASK_TYPHOST_INT_MAP_15);
    /**************************************************************/
    /* 6. Unmask tree interrupt #15 to host:                      */
    /**************************************************************/
    //tstate.int_mask = GET_MASK_TYPHOST_INT_MAP;
    //SET_MASK_TYPHOST_INT_MAP((tstate.int_mask & ~(u32)(1<<15)));
    //printk("%s(): Set MASK_TYPHOST_INT_MAP=0x%08x. \n", __func__, GET_MASK_TYPHOST_INT_MAP);
#endif
}

static void ssv6006_disable_sdio_async_interrupt(struct ssv6xxx_platform_data **priv)
{
#if defined(CONFIG_SSV_USE_SDIO_DAT1_AS_INT)
    /**************************************************************/
    /* 1. Restore MCU_NOTIFY_HOST_CFG:                            */
    /**************************************************************/
    LHAL_CMD52_REG_SET_BITS(priv, MCU_NOTIFY_HOST_CFG, 0, 0x4);
#else
    /**************************************************************/
    /* 6. Restore interrupt mask:                                 */
    /**************************************************************/
    //SET_MASK_TYPHOST_INT_MAP(tstate.int_mask);
    //printk("%s(): Restore MASK_TYPHOST_INT_MAP=0x%08x. \n", __func__, GET_MASK_TYPHOST_INT_MAP);

    /**************************************************************/
    /* 5. Restore 2nd layer interrupt mask:                       */
    /**************************************************************/
    //SET_MASK_TYPHOST_INT_MAP_15(tstate.int_15_submask);
    //printk("%s(): Restore MASK_TYPHOST_INT_MAP_15=0x%08x. \n", __func__, GET_MASK_TYPHOST_INT_MAP_15);

    /**************************************************************/
    /* 4. Restore MCU_NOTIFY_HOST_CFG:                            */
    /**************************************************************/
    //LHAL_CMD52_REG_SET_BITS(priv, MCU_NOTIFY_HOST_CFG, 0, 0x2);

    /**************************************************************/
    /* 3. Restore interrupt polarity:                             */
    /**************************************************************/
    //SET_GPO_INT_POL(tstate.gpio_polarity);
    //printk("%s(): Restore GPO_INT_POL=0x%08x. \n", __func__, GET_GPO_INT_POL);

    /**************************************************************/
    /* 2. Restore setting about interrupt through GPIO:           */
    /**************************************************************/
    //SET_SEL_GPO_INT(tstate.gpio_int_thru_gpio);
    //printk("%s(): Restore SEL_GPO_INT=0x%08x. \n", __func__, GET_SEL_GPO_INT);

    /**************************************************************/
    /* 1. Restore setting about GPIO manual mode:                 */
    /**************************************************************/
    //SET_MANUAL_IO(tstate.gpio_manual_io);
    //printk("%s(): Restore MANUAL_IO=0x%08x. \n", __func__, GET_MANUAL_IO);
#endif

    //mutex_unlock(&tstate.ts_mutex);
}
#endif

static void ssv6006_allow_sdio_reset(struct ssv6xxx_platform_data **priv, int allow)
{
    SET_ALLOW_SD_SPI_RESET(allow);
}

static void ssv_attach_ssv6006_hwif(struct ssv_hwif_hal_ops *hwif_hal_ops)
{
#ifdef CONFIG_PM
    hwif_hal_ops->enable_sdio_async_interrupt = ssv6006_enable_sdio_async_interrupt;
    hwif_hal_ops->disable_sdio_async_interrupt = ssv6006_disable_sdio_async_interrupt;
#endif
    hwif_hal_ops->allow_sdio_reset = ssv6006_allow_sdio_reset;
}

void ssv_attach_hwif_hal_ssv6006(struct ssv6xxx_platform_data **priv, struct ssv_hwif_hal_ops *hwif_hal_ops)
{
    u32  regval, chip_type;
    char fpga_tag[5];
    u64  ic_time_tag;

    ssv_hwif_read_reg(priv, ADR_CHIP_INFO_FPGATAG, &regval);
    // get FPGA tag to identify phy v5    
    *((u32 *)&fpga_tag[0])= __be32_to_cpu(regval);
    fpga_tag[4] = 0x0;
    
    // get ic time tag to identify phy v4.
    ic_time_tag = _ssv6006_get_ic_time_tag(priv);

    ssv_hwif_read_reg(priv, ADR_CHIP_TYPE_VER, &regval);
    chip_type = regval >>24;
    
    printk(KERN_INFO"Chip type %x\n", chip_type);
    
    if (chip_type == CHIP_TYPE_CHIP){
        printk(KERN_INFO"Load SSV6006 HWIF HAL HWIF function \n");
        ssv_attach_ssv6006_hwif(hwif_hal_ops);
    } else {
        if (strstr(&fpga_tag[0], FPGA_PHY_5)){
            // TODO:for phy5
            printk(KERN_INFO"Load SSV6006 HWIF HAL HWIF function \n");
            ssv_attach_ssv6006_hwif(hwif_hal_ops);
        } else {
            // TODO:others is for phy 3
            printk(KERN_INFO"Load SSV6006 HWIF HAL HWIF function \n");
            ssv_attach_ssv6006_hwif(hwif_hal_ops);
        }
    }

//#ifdef CONFIG_PM
//    mutex_init(&tstate.ts_mutex);
//#endif
}

#endif

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

uint32_t SystemCoreClock = 300000000;

extern uint32_t __BSS_START__[];
extern uint32_t __BSS_END__[];

static void BssInit(void)
{
    uint32_t *p = __BSS_START__;
    int size;

    size = (__BSS_END__ - __BSS_START__ + 1) * 4;

    memset(p, 0x0, size);
    HAL_DCACHE_CleanByRange((uint32_t)p, size);
}

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)
{
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit(void)
{
#if defined(HAL_ICACHE_MODULE_ENABLED) || defined(HAL_DCACHE_MODULE_ENABLED)
    uint32_t status;

    /* set the mcu uncache area, usually set the devices address */
    COREGRF->CACHE_PERI_ADDR_START = 0xff000;
    COREGRF->CACHE_PERI_ADDR_END = 0xffc00;
    /* stb enable, stb_entry=7, stb_timeout enable, write back */
    DCACHE->CACHE_CTRL |= DCACHE_CACHE_CTRL_CACHE_EN_MASK |
                          (7U << DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT) |
                          DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_MASK;
    DCACHE->STB_TIMEOUT_CTRL = 1;

    do {
        status =
            DCACHE->CACHE_STATUS & DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK;
    } while (status == 0);

    DCACHE->CACHE_CTRL &= ~DCACHE_CACHE_CTRL_CACHE_BYPASS_MASK;
#endif
    BssInit();
}

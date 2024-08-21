/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "soc.h"
#include "hal_base.h"

HAL_SECTION_SRAM_DATA uint32_t SystemCoreClock = 30000000;

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
    GRF->SOC_CON5 = GRF_SOC_CON5_SOC_REMAP_MASK | 1 << 16;
    __DSB();
// Swallow a unified I/D CACHE, so we only need enable dcache
#if defined(HAL_ICACHE_MODULE_ENABLED) || defined(HAL_DCACHE_MODULE_ENABLED)
    uint32_t status;

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
}

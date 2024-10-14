/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "soc.h"
#include "hal_conf.h"
#include "hal_def.h"

HAL_SECTION_SRAM_DATA uint32_t SystemCoreClock = PLL_INPUT_OSC_RATE;

/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/
#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
extern uint32_t __Vectors[];
extern const uint32_t __vector_remap__[];
#endif

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
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  /* set CP10 and CP11 Full Access */
#endif

#if defined(__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
#ifdef __STARTUP_COPY_MULTIPLE
    SCB->VTOR = (uint32_t)(__vector_remap__);
#else
    SCB->VTOR = (uint32_t)__Vectors;
#endif
#endif

#if defined(HAL_ICACHE_MODULE_ENABLED) && defined(HAL_DCACHE_MODULE_ENABLED)
    uint32_t status;

    /* Icache configuration:
     * enable:  cache_en, cache_wt
     * disable: cache_stb, cache_bypass
     */
    ICACHE->CACHE_CTRL |= (ICACHE_CACHE_CTRL_CACHE_EN_MASK |
                           ICACHE_CACHE_CTRL_CACHE_WT_EN_MASK);
    ICACHE->CACHE_CTRL &= (~ICACHE_CACHE_CTRL_CACHE_STB_EN_MASK);

    do {
        status =
            ICACHE->CACHE_STATUS & ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK;
    } while (status == 0);

    ICACHE->CACHE_CTRL &= ~ICACHE_CACHE_CTRL_CACHE_BYPASS_MASK;

    /* Dcache configuration:
     * enable:  cache_en, cache_stb, stb_timeout, stb_entry=7, mpu_mode
     * disable: cache_bypass, cache_wt
     */
    DCACHE->CACHE_CTRL |= (DCACHE_CACHE_CTRL_CACHE_EN_MASK |
                           DCACHE_CACHE_CTRL_CACHE_STB_EN_MASK |
                           DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_MASK |
                           DCACHE_CACHE_CTRL_CACHE_MPU_MODE_MASK |
                           (7U << DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT));
    DCACHE->CACHE_CTRL &= ~DCACHE_CACHE_CTRL_CACHE_WT_EN_MASK;
    DCACHE->STB_TIMEOUT_CTRL = 1;

    do {
        status =
            DCACHE->CACHE_STATUS & DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK;
    } while (status == 0);

    DCACHE->CACHE_CTRL &= ~DCACHE_CACHE_CTRL_CACHE_BYPASS_MASK;
#endif
}

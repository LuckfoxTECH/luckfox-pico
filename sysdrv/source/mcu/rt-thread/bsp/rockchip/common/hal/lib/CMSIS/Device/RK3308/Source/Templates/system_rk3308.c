/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"
#include "soc.h"

#define  SYSTEM_CLOCK 816000000U

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate(void)
{
    SystemCoreClock = SYSTEM_CLOCK;
}

/*----------------------------------------------------------------------------
  System Initialization
 *----------------------------------------------------------------------------*/
void SystemInit(void)
{
    /* do not use global variables because this function is called before
   reaching pre-main. RW section may be overwritten afterwards.          */

    // Invalidate entire Unified TLB
    __set_TLBIALL(0);

    // Invalidate entire branch predictor array
    __set_BPIALL(0);
    __DSB();
    __ISB();

    //  Invalidate instruction cache and flush branch target cache
    __set_ICIALLU(0);
    __DSB();
    __ISB();

    //  Invalidate data cache
    L1C_InvalidateDCacheAll();

#if ((__FPU_PRESENT == 1) && (__FPU_USED == 1))
    // Enable FPU
    __FPU_Enable();
#endif

    // Create Translation Table
    MMU_CreateTranslationTable();

    // Enable MMU
    MMU_Enable();

    // Enable Caches
    L1C_EnableCaches();
    L1C_EnableBTAC();

#if (__L2C_PRESENT == 1)
    // Enable L2 Cache
    L2C_Enable();
#endif
}

void DataInit(void)
{
    typedef struct {
        uint32_t const* src;
        uint32_t* dest;
        uint32_t  wlen;
    } __copy_table_t;

    typedef struct {
        uint32_t* dest;
        uint32_t  wlen;
    } __zero_table_t;

    extern const __copy_table_t __copy_table_start__;
    extern const __copy_table_t __copy_table_end__;
    extern const __zero_table_t __zero_table_start__;
    extern const __zero_table_t __zero_table_end__;

    for (__copy_table_t const* pTable = &__copy_table_start__; pTable < &__copy_table_end__; ++pTable) {
        for(uint32_t i=0u; i<pTable->wlen; ++i) {
            pTable->dest[i] = pTable->src[i];
        }
    }

    for (__zero_table_t const *pTable = &__zero_table_start__; pTable < &__zero_table_end__; ++pTable) {
        for (unsigned long i = 0u; i < pTable->wlen; ++i) {
            pTable->dest[i] = 0u;
        }
    }


}

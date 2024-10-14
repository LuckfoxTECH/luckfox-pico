/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"
#include "hal_def.h"

#ifdef HAL_ARCHTIMER_MODULE_ENABLED

#ifndef __HAL_ARCHTIMER_H
#define __HAL_ARCHTIMER_H

#if defined(__CORTEX_A) && ((__CORTEX_A == 7U) || (__CORTEX_A == 35U) || (__CORTEX_A == 53U) || (__CORTEX_A == 55U) || (__CORTEX_A == 76U)) && \
    defined(__TIM_PRESENT) && (__TIM_PRESENT == 1U)

/** @addtogroup RK_HAL_Driver
  *  @{
  */

/** @defgroup ARCHTIMER ARCHTIMER
  * @brief The cpu generic timer driver for Cortex-A
  * @{
  */

/** @defgroup ARCHTIMER_How_To_Use How To Use
  * @{

- The arch timer is often used as os tick in the following ways:

  ```
  // Get freq of arch timer
  uint32_t freq = HAL_ARCHTIMER_GetCNTFRQ();

   // Calculate load value, RTOS_TICK_PER_SECOND is tick count of OS per second
   uint32_t tick_load = (freq / RTOS_TICK_PER_SECOND) - 1U;

   // Disable arch timer
   HAL_ARCHTIMER_SetCNTPCTL(0U);

   // Set load value, the arch timer will generate a interrupt after this count
   HAL_ARCHTIMER_SetCNTPTVAL(tick_load);

   // Clear pending interrupt for arch timer
   HAL_GIC_ClearPending(TICK_IRQn);

   // Install interrupt handler, and unmask
   rt_hw_interrupt_install(TICK_IRQn, tick_isr, RT_NULL, "tick");
   rt_hw_interrupt_umask(TICK_IRQn);

   // Enable arch timer
   HAL_ARCHTIMER_SetCNTPCTL(1U);
  ```

@} */

/***************************** MACRO Definition ******************************/

/***************************** Structure Definition **************************/

/********************* Public Function Definition ****************************/
/** @defgroup ARCHTIMER_Exported_Functions_Group5 API Functions
 *  @{
 */

/**
 * @brief  Set CNTFRQ.
 * @param  value: CNTFRQ Register value to set.
 *
 */
__STATIC_FORCEINLINE void HAL_ARCHTIMER_SetCNTFRQ(uint32_t value)
{
    __set_CP(15, 0, value, 14, 0, 0);
    __ISB();
}

/**
 * @brief  Get CNTFRQ.
 * @return
 *         The value of CNTFRQ
 */
__STATIC_FORCEINLINE uint32_t HAL_ARCHTIMER_GetCNTFRQ(void)
{
    uint32_t result;

    __get_CP(15, 0, result, 14, 0, 0);

    return result;
}

/**
 * @brief  Set CNTP_TVAL.
 * @param  value: CNTP_TVAL Register value to set.
 *
 */
__STATIC_FORCEINLINE void HAL_ARCHTIMER_SetCNTPTVAL(uint32_t value)
{
    __set_CP(15, 0, value, 14, 2, 0);
    __ISB();
}

/**
 * @brief  Get CNTP_TVAL.
 * @return
 *         The value of CNTP_TVAL
 */
__STATIC_FORCEINLINE uint32_t HAL_ARCHTIMER_GetCNTPTVAL(void)
{
    uint32_t result;

    __get_CP(15, 0, result, 14, 2, 0);

    return result;
}

/**
 * @brief  Get CNTPCT.
 * @return
 *         The value of CNTPCT
 */
__STATIC_FORCEINLINE uint64_t HAL_ARCHTIMER_GetCNTPCT(void)
{
    uint64_t result;

    __get_CP64(15, 0, result, 14);

    return result;
}

/**
 * @brief  Set CNTP_CVAL.
 * @param  value: CNTP_CVAL Register value to set.
 *
 */
__STATIC_FORCEINLINE void HAL_ARCHTIMER_SetCNTPCVAL(uint64_t value)
{
    __set_CP64(15, 2, value, 14);
    __ISB();
}

/**
 * @brief  Get CNTP_CVAL.
 * @return
 *         The value of CNTP_CVAL
 */
__STATIC_FORCEINLINE uint64_t HAL_ARCHTIMER_GetCNTPCVAL(void)
{
    uint64_t result;

    __get_CP64(15, 2, result, 14);

    return result;
}

/**
 * @brief  Set CNTP_CTL.
 * @param  value: CNTP_CTL Register value to set.
 *
 */
__STATIC_FORCEINLINE void HAL_ARCHTIMER_SetCNTPCTL(uint32_t value)
{
    __set_CP(15, 0, value, 14, 2, 1);
    __ISB();
}

/**
 * @brief  Get CNTP_CTL.
 * @return
 *         The value of CNTP_CTL
 */
__STATIC_FORCEINLINE uint32_t HAL_ARCHTIMER_GetCNTPCTL(void)
{
    uint32_t result;

    __get_CP(15, 0, result, 14, 2, 1);

    return result;
}

/** @} */

/** @} */

/** @} */

#endif /* __CORTEX_A */

#endif  /* end of __HAL_ARCHTIMER_H */

#endif  /* end of HAL_ARCHTIMER_MODULE_ENABLED */

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SYSTEM_RK3568_H_
#define __SYSTEM_RK3568_H_

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */

/**
  \brief Setup the system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit (void);

/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate (void);

extern void DataInit (void);

#if defined(HAL_AP_CORE)
extern void MMU_CreateTranslationTable(void);
#endif

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_RK3568_H_ */

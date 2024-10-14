/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#ifndef __SYSTEM_RK3308_H_
#define __SYSTEM_RK3308_H_

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

extern void MMU_CreateTranslationTable(void);
#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_RK3308_H_ */

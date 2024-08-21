/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef SYSTEM_ARMCM3_H
#define SYSTEM_ARMCM3_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    REST_SYSTEM  = 0x00U,
    REST_MASKROM = 0x01U,
    REST_LOADER  = 0x02U
} eRESET_MODE;

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */

/**
  \brief Setup the microcontroller system.

   Initialize the System and update the SystemCoreClock variable.
 */
extern void SystemInit(void);

/**
  \brief Reset the microcontroller system.

   Reset the microcontroller directly or enter maskrom/loader mode.
 */
extern void SystemReset(eRESET_MODE mode);

/**
  \brief  Update SystemCoreClock variable.

   Updates the SystemCoreClock with current core Clock retrieved from cpu registers.
 */
extern void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_ARMCM3_H */

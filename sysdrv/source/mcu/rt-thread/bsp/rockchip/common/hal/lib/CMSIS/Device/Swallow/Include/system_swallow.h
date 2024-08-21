/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SYSTEM_SWALLOW_H_
#define __SYSTEM_SWALLOW_H_

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */

extern void SystemCoreClockUpdate(void);
extern void SystemInit(void);

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_SWALLOW_H_ */

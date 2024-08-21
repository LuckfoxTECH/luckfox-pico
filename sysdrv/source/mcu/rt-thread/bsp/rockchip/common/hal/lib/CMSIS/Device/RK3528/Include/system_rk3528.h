/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#ifndef __SYSTEM_RK3528_H_
#define __SYSTEM_RK3528_H_

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */

extern void SystemCoreClockUpdate(void);
extern void SystemInit(void);

#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_RK3528_H_ */

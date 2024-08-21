/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2020 Rockchip Electronics Co., Ltd.
 */

#ifndef _HAL_CONF_H_
#define _HAL_CONF_H_

/* HAL SOC Config */
#define RKMCU_RK2106
#define SYS_TIMER TIMER1 /* System timer designation (RK TIMER) */

/* HAL Driver Config */
#define HAL_ACODEC_MODULE_ENABLED
#define HAL_CRU_MODULE_ENABLED
#define HAL_DWDMA_MODULE_ENABLED
#define HAL_I2S_MODULE_ENABLED
#define HAL_NVIC_MODULE_ENABLED
#define HAL_SYSTICK_MODULE_ENABLED
#define HAL_TIMER_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED

/* HAL_DBG SUB CONFIG */
#define HAL_DBG_ON
#define HAL_DBG_INFO_ON
#define HAL_DBG_WRN_ON
#define HAL_DBG_ERR_ON
#define HAL_ASSERT_ON

#endif

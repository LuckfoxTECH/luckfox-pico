/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd.
 */

#ifndef _HAL_CONF_H_
#define _HAL_CONF_H_

/* CPU config */
#define SOC_SWALLOW

#define SYS_TIMER TIMER5 /* System timer designation (RK TIMER) */

/* HAL Driver Config */
#define HAL_CKCAL_MODULE_ENABLED
#define HAL_BUFMGR_MODULE_ENABLED
#define HAL_FSPI_MODULE_ENABLED
#define HAL_NVIC_MODULE_ENABLED
#define HAL_PCD_MODULE_ENABLED
#define HAL_SNOR_MODULE_ENABLED
#define HAL_TIMER_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED

/* HAL_DBG SUB CONFIG */
#define HAL_DBG_USING_LIBC_PRINTF
#define HAL_DBG_ON
#define HAL_DBG_INFO_ON
#define HAL_DBG_WRN_ON
#define HAL_DBG_ERR_ON

#if defined(HAL_SNOR_MODULE_ENABLED)
#define HAL_SNOR_FSPI_HOST
#define HAL_FSPI_XIP_ENABLE
#endif

#endif

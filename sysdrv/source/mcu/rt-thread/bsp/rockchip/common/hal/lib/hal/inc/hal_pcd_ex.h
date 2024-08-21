/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_PCD_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PCDEx
 *  @{
 */

#ifndef _HAL_PCD_EX_H_
#define _HAL_PCD_EX_H_

#include "hal_def.h"

/***************************** Structure Definition **************************/
/** @defgroup PCDEx_Exported_Definition_Group1 Basic Definition
 *  @{
 */

/** PCD LPM State Structure definition */
typedef enum {
    PCD_LPM_L0_ACTIVE = 0x00U, /**< on */
    PCD_LPM_L1_ACTIVE = 0x01U, /**< LPM L1 sleep */
} ePCD_lpmMsg;

/** @} */

/***************************** Function Declare ******************************/
/** @defgroup PCDEx_Public_Function_Declare Public Function Declare
 *  @{
 */
HAL_Status HAL_PCDEx_SetTxFiFo(struct PCD_HANDLE *pPCD, uint8_t fifo, uint16_t size);
HAL_Status HAL_PCDEx_SetRxFiFo(struct PCD_HANDLE *pPCD, uint16_t size);
HAL_Status HAL_PCDEx_ActivateLPM(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCDEx_DeActivateLPM(struct PCD_HANDLE *pPCD);
HAL_Status HAL_PCDEx_BcdDetect(struct PCD_HANDLE *pPCD);
void HAL_PCDEx_LpmCallback(struct PCD_HANDLE *pPCD, ePCD_lpmMsg msg);
void HAL_PCDEx_BcdCallback(struct PCD_HANDLE *pPCD, ePCD_bcdMsg msg);

/** @} */

#endif /* _HAL_PCD_EX_H_ */

/** @} */

/** @} */

#endif /* HAL_PCD_MODULE_ENABLED */

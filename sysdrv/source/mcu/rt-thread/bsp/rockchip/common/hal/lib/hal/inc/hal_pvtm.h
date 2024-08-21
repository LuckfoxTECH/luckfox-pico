/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_PVTM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PVTM
 *  @{
 */

#ifndef _HAL_PVTM_H_
#define _HAL_PVTM_H_

#include "hal_def.h"

/***************************** MACRO Definition ******************************/

typedef enum {
    PVTM_ID_CORE = 0,
} ePVTM_ID;

/***************************** Structure Definition **************************/

struct HAL_PVTM_INFO {
    __IO uint32_t *con0;
    __IO uint32_t *con1;
    __I uint32_t *sta0;
    __I uint32_t *sta1;

    ePVTM_ID pvtmId;
    uint32_t clkGateID;
    uint32_t pclkGateID;

    uint32_t startMask;
    uint32_t startShift;
    uint32_t enMask;
    uint32_t enShift;
    uint32_t selMask;
    uint32_t selShift;
    uint32_t doneMask;
    uint32_t doneShift;
};

struct HAL_PVTM_DEV {
    const struct HAL_PVTM_INFO *info;
    uint8_t num;
};

/***************************** Function Declare ******************************/
/** @defgroup PVTM_Public_Function_Declare Public Function Declare
 *  @{
 */

HAL_Status HAL_PVTM_GetFreqCnt(ePVTM_ID id, uint32_t chn, uint32_t timeUs,
                               uint32_t *val);

/** @} */

#endif

/** @} */

/** @} */

#endif /* HAL_PVTM_MODULE_ENABLED */

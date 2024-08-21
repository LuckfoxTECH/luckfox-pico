/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2019-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_PVTM_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PVTM
 *  @{
 */

/** @defgroup PVTM_How_To_Use How To Use
 *  @{

 The PVTM driver can be used as follows:

 - invoke pvtm functions to get pvtm value with sample channel and time.

 @} */

#include "hal_bsp.h"

/** @defgroup PVTM_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup PVTM_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  get pvtm value.
 * @param  id: pvtm id.
 * @param  chn: pvtm sample channel.
 * @param  timeUs: pvtm sample time.
 * @param  val: store pvtm value.
 * @return HAL_Status.
 */
HAL_Status HAL_PVTM_GetFreqCnt(ePVTM_ID id, uint32_t chn, uint32_t timeUs,
                               uint32_t *val)
{
    HAL_Status ret = HAL_OK;
    const struct HAL_PVTM_INFO *info = NULL;
    int i, delayCount = 100;

    if (!val) {
        return HAL_INVAL;
    }

    for (i = 0; i < g_pvtmDev.num; i++) {
        if (id == g_pvtmDev.info[i].pvtmId) {
            info = &g_pvtmDev.info[i];
            break;
        }
    }
    if (!info) {
        return HAL_INVAL;
    }

    HAL_CRU_ClkEnable(info->clkGateID);
    if (info->pclkGateID) {
        HAL_CRU_ClkEnable(info->pclkGateID);
    }

    if (READ_REG(*(info->con0)) & info->startMask) {
        WRITE_REG_MASK_WE(*(info->con0), info->startMask,
                          0 << info->startShift);
    }

    WRITE_REG_MASK_WE(*(info->con0), info->enMask, 1 << info->enShift);
    WRITE_REG_MASK_WE(*(info->con0), info->selMask, chn << info->selShift);
    WRITE_REG(*(info->con1), PLL_INPUT_OSC_RATE / MHZ * timeUs);
    WRITE_REG_MASK_WE(*(info->con0), info->startMask, 1 << info->startShift);

    HAL_DelayUs(timeUs);
    while (delayCount > 0) {
        if (READ_REG(*(info->sta0)) & info->doneMask) {
            break;
        }
        HAL_DelayUs(4);
        delayCount--;
    }
    if (delayCount > 0) {
        *val = READ_REG(*(info->sta1));
    } else {
        ret = HAL_TIMEOUT;
    }

    WRITE_REG_MASK_WE(*(info->con0), info->startMask, 0 << info->startShift);
    WRITE_REG_MASK_WE(*(info->con0), info->enMask, 0 << info->enShift);

    HAL_CRU_ClkDisable(info->clkGateID);
    if (info->pclkGateID) {
        HAL_CRU_ClkDisable(info->pclkGateID);
    }

    return ret;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_PVTM_MODULE_ENABLED */

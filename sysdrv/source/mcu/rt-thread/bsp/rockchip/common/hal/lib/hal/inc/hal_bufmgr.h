/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_BUFMGR_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup BUFMGR
 *  @{
 */

#ifndef __HAL_BUFMGR_H__
#define __HAL_BUFMGR_H__

/***************************** MACRO Definition ******************************/
/** @defgroup BUFMGR_Exported_Definition_Group1 Basic Definition
 *  @{
 */

#define HAL_BUFMGR_WCH_INTSTS_FULL_BIT(n) \
    (1U << (n))
#define HAL_BUFMGR_WCH_STS_FULL_BIT(n) \
    (1U << (n + BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_STATUS_SHIFT))
#define HAL_BUFMGR_WCH_STS_EOH_BIT(n) \
    (1U << (n + BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FIRST_FLAG_STATUS_SHIFT))
#define HAL_BUFMGR_WCH_STS_EOF_BIT(n) \
    (1U << (n + BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_LAST_FLAG_STATUS_SHIFT))
#define HAL_BUFMGR_WCH_INTSTS_OVFL_BIT \
    (BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF_OVERFLOW_INTSTS_MASK)

/** @} */
/***************************** Function Declare ******************************/
/** @defgroup BUFMGR_Public_Function_Declare Public Function Declare
 *  @{
 */

uint32_t HAL_BUFMGR_WCHEndBufLenGet(int chan, int block);
uint32_t HAL_BUFMGR_WCHCurBufIdxGet(int chan, int block);
uint32_t HAL_BUFMGR_WCHIntStsGet(int chan, int block);
uint32_t HAL_BUFMGR_WCHBufSizeGet(int chan, int block);
uint32_t HAL_BUFMGR_WCHBufAddrGet(int chan, int block, int idx);
HAL_Status HAL_BUFMGR_WCHIntStsClr(int chan, int block, uint32_t val);
HAL_Status HAL_BUFMGR_WCHIntStsClrByIdx(int chan, int block, uint32_t idx);
HAL_Status HAL_BUFMGR_WCHIntStsAllClr(int chan, int block);
HAL_Status HAL_BUFMGR_WCHPpBufEn(int chan, int block, uint8_t en);
HAL_Status HAL_BUFMGR_WCHIntAllEn(int chan, int block, uint8_t en);
HAL_Check HAL_BUFMGR_WCHIntStsIsOVFL(uint32_t sts);
HAL_Check HAL_BUFMGR_WCHIntStsIsFull(uint32_t sts, int idx);
HAL_Check HAL_BUFMGR_WCHStsIsBufFull(uint32_t sts, int idx);
HAL_Check HAL_BUFMGR_WCHStsIsEOH(int chan, int block, uint32_t sts, int idx);
HAL_Check HAL_BUFMGR_WCHStsIsEOF(uint32_t sts, int idx);

/** @} */

#endif /* HAL_BUFMGR_MODULE_ENABLED */

/** @} */

/** @} */

#endif /* __HAL_BUFMGR_H__ */

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_conf.h"

#ifdef HAL_CACHE_ECC_MODULE_ENABLED
/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CACHE_ECC
 *  @{
 */

#ifndef _HAL_CACHE_ECC_H_
#define _HAL_CACHE_ECC_H_
#include "hal_base.h"

/********************* MACRO Definition ******************************/
/** @defgroup CACHE_ECC_Exported_Definition_Group1 Basic Definition
 *  @{
 */
#define ERXCTLR_ED_BIT  (1 << 0)
#define ERXCTLR_UI_BIT  (1 << 2)
#define ERXCTLR_FI_BIT  (1 << 3)
#define ERXCTLR_CFI_BIT (1 << 8)

/** error record ID, selects which error record will be accessed */
typedef enum {
    CACHE_ECC_ERR0 = 0x0, /**< L1 or L2 ECC */
    CACHE_ECC_ERR1 = 0x1, /**< DSU L3 ECC */
} eCACHE_ECC_RecodeID;
/** @} */

/***************************** Function Declare ******************************/
/** @defgroup CACHE_ECC_Public_Function_Declare Public Function Declare
 *  @{
 */
uint32_t HAL_CACHE_ECC_GetErxstatus(eCACHE_ECC_RecodeID sel);
uint32_t HAL_CACHE_ECC_GetErxmisc0(eCACHE_ECC_RecodeID sel);
uint32_t HAL_CACHE_ECC_GetErxmisc1(eCACHE_ECC_RecodeID sel);
uint32_t HAL_CACHE_ECC_GetErxmisc2(eCACHE_ECC_RecodeID sel);
uint32_t HAL_CACHE_ECC_GetErxmisc3(eCACHE_ECC_RecodeID sel);
HAL_Status HAL_CACHE_ECC_Inject(eCACHE_ECC_RecodeID errSel,
                                eCACHE_ECC_InjectFault injectFault);
HAL_Status HAL_CACHE_ECC_SetErxctlr(uint32_t err0ctlr, uint32_t err1ctlr);

/** @} */

#endif
/** @} */

/** @} */
#endif

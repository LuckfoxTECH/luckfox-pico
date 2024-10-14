/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_CACHE_ECC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup CACHE_ECC
 *  @{
 */

/** @defgroup CACHE_ECC_How_To_Use How To Use
 *  @{

 The CACHE_ECC driver can be used as follows:

 - Invoke HAL_CACHE_ECC_SetErxctlr() to set the ERXCTLR_EL1 register for enabling ecc features.
 - Invoke HAL_CACHE_ECC_Inject() to Inject cache faults or errors for testing cache ecc.
 - Invoke HAL_CACHE_ECC_GetErxmisc3() to get the value of the register ERXMISC3.
 - Invoke HAL_CACHE_ECC_GetErxmisc2() to get the value of the register ERXMISC2.
 - Invoke HAL_CACHE_ECC_GetErxmisc1() to get the value of the register ERXMISC1.
 - Invoke HAL_CACHE_ECC_GetErxmisc0() to get the value of the register ERXMISC0.
 - Invoke HAL_CACHE_ECC_GetErxstatus() to get the value of the register ERXSTATUS.

 @} */
/** @defgroup CACHE_ECC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/********************* Private Structure Definition **************************/
/********************* Private Variable Definition ***************************/
static uint32_t injectVal[8];
/********************* Private Function Definition ***************************/
/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup CACHE_ECC_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get cache ecc status:

 *  @{
 */

/**
 * @brief  Get the value of the register ERXSTATUS
 * @param  errSel:  error record id
 * @return the value of the register ERXSTATUS.
 */
uint32_t HAL_CACHE_ECC_GetErxstatus(eCACHE_ECC_RecodeID errSel)
{
    uint32_t val;

    __set_CP(15, 0, errSel, 5, 3, 1);
    __get_CP(15, 0, val, 5, 4, 2);

    return val;
}

/**
 * @brief  Get the value of the register ERXMISC0
 * @param  errSel:  error record id
 * @return the value of the register ERXMISC0.
 */
uint32_t HAL_CACHE_ECC_GetErxmisc0(eCACHE_ECC_RecodeID errSel)
{
    uint32_t val;

    __set_CP(15, 0, errSel, 5, 3, 1);
    __get_CP(15, 0, val, 5, 5, 0);

    return val;
}

/**
 * @brief  Get the value of the register ERXMISC1
 * @param  errSel:  error record id
 * @return the value of the register ERXMISC1.
 */
uint32_t HAL_CACHE_ECC_GetErxmisc1(eCACHE_ECC_RecodeID errSel)
{
    uint32_t val;

    __set_CP(15, 0, errSel, 5, 3, 1);
    __get_CP(15, 0, val, 5, 5, 1);

    return val;
}

/**
 * @brief  Get the value of the register ERXMISC2
 * @param  errSel:  error record id
 * @return the value of the register ERXMISC2.
 */
uint32_t HAL_CACHE_ECC_GetErxmisc2(eCACHE_ECC_RecodeID errSel)
{
    uint32_t val;

    __set_CP(15, 0, errSel, 5, 3, 1);
    __get_CP(15, 0, val, 5, 5, 4);

    return val;
}

/**
 * @brief  Get the value of the register ERXMISC3
 * @param  errSel:  error record id
 * @return the value of the register ERXMISC3.
 */
uint32_t HAL_CACHE_ECC_GetErxmisc3(eCACHE_ECC_RecodeID errSel)
{
    uint32_t val;

    __set_CP(15, 0, errSel, 5, 3, 1);
    __get_CP(15, 0, val, 5, 5, 5);

    return val;
}

/** @} */

/** @defgroup CACHE_ECC_Exported_Functions_Group5 Other Functions
 *  @{
 */
/**
 * @brief  Inject cache faults or errors for testing cache ecc.
 * @param  errSel:  error record id
 * @param  injectFault: the fault will be injected.
 *                      make sure that ATF have allowed the register
 *                      ERR0PFGCTLR and ERR1PFGCTLR to be configured
 *                      in EL1 or SVC.
 * @return HAL_Status.
 */
HAL_Status HAL_CACHE_ECC_Inject(eCACHE_ECC_RecodeID errSel,
                                eCACHE_ECC_InjectFault injectFault)
{
    uint32_t *addr = &injectVal[0];

    __ASM volatile (
        "mcr p15, 0, %0, c5, c3, 1 \n"
        "mcr p15, 0, %1, c15, c2, 1 \n"
        "str %1,[%2]\n"
        "str %1,[%2]\n"
        "str %1,[%2]\n"
        "str %1,[%2]\n"
        "str %0,[%2, #4]\n"
        "str %2,[%2, #8]\n"
        :
        : "r" (errSel), "r" (injectFault), "r" (addr)
        :);

    return HAL_OK;
}

/**
 * @brief  set the ERXCTLR_EL1 register for enabling ecc features.
 * @param  err0ctlr:  set the register ERR0CTLR to enable l1~l2 ecc features.
 * @param  err1ctlr:  set the register ERR1CTLR to enable DSU L3 ecc features.
 *                      make sure that ATF have allowed the register ERR0CTLR
 *                      and ERR1CTLR to be configured in EL1 or SVC.
 * @return HAL_Status.
 */
HAL_Status HAL_CACHE_ECC_SetErxctlr(uint32_t err0ctlr, uint32_t err1ctlr)
{
    __set_CP(15, 0, 0, 5, 3, 1);
    __set_CP(15, 0, err0ctlr, 5, 4, 1);

    __set_CP(15, 0, 1, 5, 3, 1);
    __set_CP(15, 0, err1ctlr, 5, 4, 1);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_SMCCC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup SMCCC
 *  @{
 */

/** @defgroup SMCCC_How_To_Use How To Use
 *  @{

 The SMCCC driver can be used as follows:

 - Invoke HAL_SMCCC_Call() to trap into Secure Monitor mode which is at EL3 or
   Secure-PL1.
 - Invoke HAL_SMCCC_SIP_AmpCpuOff() to power off the current cpu.
 - Invoke HAL_SMCCC_SIP_AmpCpuOffIrqHandler() to power off the current cpu
   in irq handler.

 @} */

/********************* Public Function Definition ****************************/

/** @defgroup SMCCC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  power off the current cpu.
 * @return HAL_Status.
 */
HAL_Status HAL_SMCCC_SIP_AmpCpuOff(void)
{
    struct SMCCC_PARAM smcArgs;

    smcArgs.a0 = SIP_AMP_CFG;
    smcArgs.a1 = SIP_AMP_SUB_FUNC_CPU_OFF;
    HAL_SMCCC_Call(&smcArgs);

    return HAL_OK;
}

/**
 * @brief  power off the current cpu in irq handler.
 * @param  irq: irq id.
 * @param  args: private parameters
 * @return HAL_Status.
 */
HAL_Status HAL_SMCCC_SIP_AmpCpuOffIrqHandler(uint32_t irq, void *args)
{
    uint32_t cpuId, needIrq;

    cpuId = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    needIrq = AMP_CPUOFF_REQ_IRQ(cpuId);

    if (irq != needIrq) {
        return HAL_INVAL;
    }

    HAL_GIC_Disable(irq);
    HAL_GIC_EndOfInterrupt(irq);
    HAL_SMCCC_SIP_AmpCpuOff();

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_KOALA) && defined(HAL_DSP_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup DSP
 *  @{
 */

/** @defgroup DSP_How_To_Use How To Use
 *  @{

 The DSP driver can be used as follows:

 @} */

/** @defgroup DSP_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

static int DSP_Ioctl(void *priv, int cmd, void *arg);
static const struct DSP_OPS dspOps = {
    .ioctl = DSP_Ioctl,
};

/********************* Private Function Definition ***************************/

/**
 * @brief  Set tcm work mode for retention or power down.
 * @param  tcmSel: itcm or dtcm "|" combination, such as DSP_DTCM0 | DSP_DTCM1.
 * @param  mode: work mode in eDSP_tcmMode.
 * @return HAL_Status
 */
static int DSP_Ioctl(void *priv, int cmd, void *arg)
{
    HAL_Status ret = HAL_OK;

    switch (cmd) {
    case DSP_IOCTL_SET_MEM_GATING:
    {
        uint32_t mask = 0;
        mask |= GRF_DSP_CON2_ICACHE_MEM_AUTO_GATING_DISABLE_MASK |
                GRF_DSP_CON2_ITAG_MEM_AUTO_GATING_DISABLE_MASK |
                GRF_DSP_CON2_DCACHE_MEM_AUTO_GATING_DISABLE_MASK |
                GRF_DSP_CON2_DTAG_MEM_AUTO_GATING_DISABLE_MASK |
                GRF_DSP_CON2_PREFETCH_RAM_AUTO_GATING_DISABLE_MASK |
                GRF_DSP_CON2_DTCM_MEM_AUTO_GATING_DISABLE_MASK |
                GRF_DSP_CON2_ITCM_MEM_AUTO_GATING_DISABLE_MASK;
        if ((uint32_t)arg) {
            // set value 0 is enable
            WRITE_REG_MASK_WE(GRF->DSP_CON2, mask, 0);
        } else {
            WRITE_REG_MASK_WE(GRF->DSP_CON2, mask, mask);
        }
        break;
    }
    default:
        break;
    }

    return ret;
}

/** @} */
/********************* Public Function Definition ***************************/

/** @defgroup DSP_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

HAL_Status HAL_DSP_SetTcmMode(uint32_t tcmSel, eDSP_tcmMode mode)
{
    return HAL_OK;
}

HAL_Status HAL_DSP_WaitForPowerSt(eDSP_powerSt status, uint32_t timeout)
{
    return HAL_OK;
}

HAL_Status HAL_DSP_SoftWakeup(void)
{
    return HAL_OK;
}

HAL_Status HAL_DSP_PowerOn(struct DSP_DEV *dsp)
{
    HAL_CRU_ClkEnable(ACLK_DSP_GATE);
    HAL_CRU_ClkEnable(PCLK_DSP_GATE);

    return HAL_OK;
}

HAL_Status HAL_DSP_PowerOff(struct DSP_DEV *dsp)
{
    /* DTCM is used by m3,so do not disable clk*/
    if (dsp->resetFlag != DSP_RESET_MODE_NOT_RESET) {
        HAL_CRU_ClkDisable(ACLK_DSP_GATE);
        HAL_CRU_ClkDisable(PCLK_DSP_GATE);
    }

    return HAL_OK;
}

HAL_Status HAL_DSP_Init(struct DSP_DEV *dsp)
{
    dsp->ops = &dspOps;
    dsp->grfReg = (struct GRF_REG *)(GRF_BASE);
    dsp->errorIrq = DSP_PFATAL_ERROR_IRQn;

    dsp->mboxIsA2B = 1;
    dsp->mboxReg = MBOX0;
    dsp->mboxIrq[0] = MAILBOX_CA0_IRQn;
    dsp->mboxIrq[1] = MAILBOX_CA1_IRQn;
    dsp->mboxIrq[2] = MAILBOX_CA2_IRQn;
    dsp->mboxIrq[3] = MAILBOX_CA3_IRQn;

    dsp->resetFlag = DSP_RESET_MODE_RESET_ALL_CLK;
    /*
     * check the value of CRU_SOFTRST4_CON,
     * the default value of CRU_SOFTRST4_CON is 0x06,
     * if m3 want use DTCM of dsp, it will reset dsp and
     * reassert reset in startup_koala.S. So here,
     * we check the value of CRU_SOFTRST4_CON to judge
     * whether DTCM is used by m3
     */
    uint32_t setMask = CRU_SOFTRST4_CON_ARESETN_DSP_NOC_REQ_MASK |
                       CRU_SOFTRST4_CON_RESETN_DSP_REQ_MASK |
                       CRU_SOFTRST4_CON_RESETN_DSP_EXTCORE_REQ_MASK;
    int value = READ_BIT(CRU->CRU_SOFTRST_CON[4], setMask);

    /* value 0 means the dsp already be reseted and reasserted reset*/
    if (value == 0) {
        dsp->resetFlag = DSP_RESET_MODE_NOT_RESET;
    }

    /* Deassert reset */
    HAL_DSP_Enable(dsp, 0);

    return HAL_OK;
}

HAL_Status HAL_DSP_DeInit(struct DSP_DEV *dsp)
{
    dsp->ops = NULL;
    dsp->grfReg = NULL;

    return HAL_OK;
}

/** @} */

/** @defgroup DSP_Exported_Functions_Group5 Other Functions
 *  @{
 */

HAL_Status HAL_DSP_Enable(struct DSP_DEV *dsp, uint32_t altAddr)
{
    uint32_t setMask = 0;

    /* Set dsp stall */
    WRITE_REG_MASK_WE(GRF->DSP_CON0, GRF_DSP_CON0_RUNSTALL_MASK,
                      GRF_DSP_CON0_RUNSTALL_MASK);
    /*
     * dsp->resetFlag == DSP_RESET_MODE_NOT_RESET means the
     * m3 is using the DTCM(the address is 0x30240000U ~ 0x3027ffffU) of dsp,
     * so do not reset dsp here.
     */
    if (dsp->resetFlag != DSP_RESET_MODE_NOT_RESET) {
        if (dsp->resetFlag == DSP_RESET_MODE_RESET_ALL_CLK) {
            setMask = CRU_SOFTRST4_CON_ARESETN_DSP_NOC_REQ_MASK |
                      CRU_SOFTRST4_CON_RESETN_DSP_REQ_MASK |
                      CRU_SOFTRST4_CON_RESETN_DSP_EXTCORE_REQ_MASK;
            dsp->resetFlag = DSP_RESET_MODE_RESET_PART_CLK;
        } else {
            setMask = CRU_SOFTRST4_CON_RESETN_DSP_REQ_MASK |
                      CRU_SOFTRST4_CON_RESETN_DSP_EXTCORE_REQ_MASK;
        }

        // reset dsp
        WRITE_REG_MASK_WE(CRU->CRU_SOFTRST_CON[4], setMask, setMask);
    }

    if (altAddr) {
        WRITE_REG_MASK_WE(GRF->DSP_CON0, GRF_DSP_CON0_STATVECTORSEL_MASK,
                          GRF_DSP_CON0_STATVECTORSEL_MASK);
        WRITE_REG(GRF->DSP_CON1, altAddr);
    }

    /* Deassert reset */
    WRITE_REG_MASK_WE(CRU->CRU_SOFTRST_CON[4], setMask, 0);

    return HAL_OK;
}

HAL_Status HAL_DSP_Disable(struct DSP_DEV *dsp)
{
    return HAL_OK;
}

HAL_Status HAL_DSP_Start(struct DSP_DEV *dsp)
{
    WRITE_REG_MASK_WE(GRF->DSP_CON0, GRF_DSP_CON0_RUNSTALL_MASK, 0);

    return HAL_OK;
}

HAL_Status HAL_DSP_Stop(struct DSP_DEV *dsp)
{
    WRITE_REG_MASK_WE(GRF->DSP_CON0, GRF_DSP_CON0_RUNSTALL_MASK,
                      GRF_DSP_CON0_RUNSTALL_MASK);

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* RKMCU_KOALA && HAL_DSP_MODULE_ENABLED */

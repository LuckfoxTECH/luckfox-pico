/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if (defined(RKMCU_PISCES) || defined(RKMCU_RK2108)) && defined(HAL_DSP_MODULE_ENABLED)

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
static HAL_Status DSP_SetTcmMode(uint32_t tcmSel, eDSP_tcmMode mode)
{
    uint32_t mask0 = 0;
    uint32_t mask1 = 0;
    uint32_t value0 = 0;
    uint32_t value1 = 0;

    if (tcmSel == DSP_ITCM) {
        mask1 = PMU_DSPTCM_CON1_DSPITCM_PGEN_SFT_MASK |
                PMU_DSPTCM_CON1_DSPITCM_RET1N_SFT_MASK |
                PMU_DSPTCM_CON1_DSPITCM_RET2N_SFT_MASK;
        switch (mode) {
        case NOR_MODE:
        {
            value1 = PMU_DSPTCM_CON1_DSPITCM_RET1N_SFT_MASK |
                     PMU_DSPTCM_CON1_DSPITCM_RET2N_SFT_MASK;
        }
        break;
        case RET1N_MODE:
        {
            value1 = PMU_DSPTCM_CON1_DSPITCM_PGEN_SFT_MASK |
                     PMU_DSPTCM_CON1_DSPITCM_RET2N_SFT_MASK;
        }
        break;
        case RET2N_MODE:
        {
            value1 = PMU_DSPTCM_CON1_DSPITCM_PGEN_SFT_MASK |
                     PMU_DSPTCM_CON1_DSPITCM_RET1N_SFT_MASK;
        }
        break;
        case PWR_DOWN_MODE:
        {
            value1 = PMU_DSPTCM_CON1_DSPITCM_PGEN_SFT_MASK |
                     PMU_DSPTCM_CON1_DSPITCM_RET1N_SFT_MASK |
                     PMU_DSPTCM_CON1_DSPITCM_RET2N_SFT_MASK;
        }
        break;
        default:
            break;
        }
    } else {
        mask0 = (PMU_DSPTCM_CON0_DSPDTCM_PGEN_SFT_MASK & tcmSel) |
                (PMU_DSPTCM_CON0_DSPDTCM_RET1N_SFT_MASK & (tcmSel << 8));
        mask1 = PMU_DSPTCM_CON1_DSPDTCM_RET2N_SFT_MASK & tcmSel;
        switch (mode) {
        case NOR_MODE:
        {
            value0 = PMU_DSPTCM_CON0_DSPDTCM_RET1N_SFT_MASK & (tcmSel << 8);
            value1 = PMU_DSPTCM_CON1_DSPDTCM_RET2N_SFT_MASK & tcmSel;
        }
        break;
        case RET1N_MODE:
        {
            value0 = PMU_DSPTCM_CON0_DSPDTCM_PGEN_SFT_MASK & tcmSel;
            value1 = PMU_DSPTCM_CON1_DSPDTCM_RET2N_SFT_MASK & tcmSel;
        }
        break;
        case RET2N_MODE:
        {
            value0 = (PMU_DSPTCM_CON0_DSPDTCM_PGEN_SFT_MASK & tcmSel) |
                     (PMU_DSPTCM_CON0_DSPDTCM_RET1N_SFT_MASK & (tcmSel << 8));
        }
        break;
        case PWR_DOWN_MODE:
        {
            value0 = (PMU_DSPTCM_CON0_DSPDTCM_PGEN_SFT_MASK & tcmSel) |
                     (PMU_DSPTCM_CON0_DSPDTCM_RET1N_SFT_MASK & (tcmSel << 8));
            value1 = PMU_DSPTCM_CON1_DSPDTCM_RET2N_SFT_MASK & tcmSel;
        }
        break;
        default:
            break;
        }
    }
    if (mask0) {
        WRITE_REG_MASK_WE(PMU->DSPTCM_CON[0], mask0, value0);
    }
    WRITE_REG_MASK_WE(PMU->DSPTCM_CON[1], mask1, value1);

    return HAL_OK;
}

static int DSP_Ioctl(void *priv, int cmd, void *arg)
{
    HAL_Status ret = HAL_OK;

    switch (cmd) {
    case DSP_IOCTL_SET_MEM_GATING:
    {
        uint32_t mask = 0;
        mask |= GRF_DSP_CON2_ICACHE_MEM_AUTO_GATING_EN_MASK |
                GRF_DSP_CON2_ITAG_MEM_AUTO_GATING_EN_MASK |
                GRF_DSP_CON2_DCACHE_MEM_AUTO_GATING_EN_MASK |
                GRF_DSP_CON2_DTAG_MEM_AUTO_GATING_EN_MASK |
                GRF_DSP_CON2_PREFETCH_RAM_AUTO_GATING_EN_MASK |
                GRF_DSP_CON2_DTCM_MEM_AUTO_GATING_EN_MASK |
                GRF_DSP_CON2_ITCM_MEM_AUTO_GATING_EN_MASK;
        if ((uint32_t)arg) {
            WRITE_REG_MASK_WE(GRF->DSP_CON2, mask, mask);
        } else {
            WRITE_REG_MASK_WE(GRF->DSP_CON2, mask, 0);
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

eDSP_powerSt HAL_DSP_GetPowerSt(void)
{
    uint32_t val = READ_REG(PMU->POWER_ST);

    val = (val & PMU_POWER_ST_DSP_POWER_STATE_MASK) >> PMU_POWER_ST_DSP_POWER_STATE_SHIFT;

    return (eDSP_powerSt)val;
}

HAL_Status HAL_DSP_WaitForPowerSt(eDSP_powerSt status, uint32_t timeout)
{
    uint32_t start;

    start = HAL_GetTick();
    while (HAL_DSP_GetPowerSt() != status) {
        if ((HAL_GetTick() - start) > timeout) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

HAL_Status HAL_DSP_SoftWakeup(void)
{
    WRITE_REG_MASK_WE(PMU->DSPAPM_CON, PMU_DSPAPM_CON_PD_DSP_SFT_WAKEUP_MASK, 0);
    WRITE_REG_MASK_WE(PMU->DSPAPM_CON, PMU_DSPAPM_CON_PD_DSP_SFT_WAKEUP_MASK,
                      PMU_DSPAPM_CON_PD_DSP_SFT_WAKEUP_MASK);

    return HAL_OK;
}

HAL_Status HAL_DSP_SetTcmMode(uint32_t tcmSel, eDSP_tcmMode mode)
{
    uint32_t sel;

    HAL_ASSERT(tcmSel);

    /* Check itcm select */
    sel = tcmSel & DSP_ITCM;
    if (sel) {
        DSP_SetTcmMode(sel, mode);
    }

    /* Check dtcm select */
    sel = tcmSel & ~sel;
    if (sel) {
        DSP_SetTcmMode(sel, mode);
    }

    return HAL_OK;
}

HAL_Status HAL_DSP_PowerOn(struct DSP_DEV *dsp)
{
    HAL_CRU_ClkEnable(ACLK_DSP_GATE);
    HAL_CRU_ClkEnable(PCLK_DSP_GATE);
#if defined(RT_USING_PMU)
    HAL_PD_On(PD_DSP);
#endif
    HAL_DSP_SetTcmMode(DSP_TCM, NOR_MODE);

    return HAL_OK;
}

HAL_Status HAL_DSP_PowerOff(struct DSP_DEV *dsp)
{
    HAL_DSP_SetTcmMode(DSP_TCM, PWR_DOWN_MODE);
#if defined(RT_USING_PMU)
    HAL_PD_Off(PD_DSP);
#endif
    HAL_CRU_ClkDisable(ACLK_DSP_GATE);
    HAL_CRU_ClkDisable(PCLK_DSP_GATE);

    return HAL_OK;
}

HAL_Status HAL_DSP_Init(struct DSP_DEV *dsp)
{
    dsp->ops = &dspOps;
    dsp->grfReg = (struct GRF_REG *)(GRF_BASE);
    dsp->resetFlag = DSP_RESET_MODE_RESET_ALL_CLK;

    dsp->mboxIsA2B = 1;
    dsp->mboxReg = MBOX0;
    dsp->errorIrq = DSP_PFATAL_ERROR_IRQn;
    dsp->mboxIrq[0] = MAILBOX0_AP_IRQn;
    dsp->mboxIrq[1] = MAILBOX0_AP_IRQn;
    dsp->mboxIrq[2] = MAILBOX0_AP_IRQn;
    dsp->mboxIrq[3] = MAILBOX0_AP_IRQn;
    dsp->clkId = ACLK_DSP;

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
    /* Set dsp reset */
    if (dsp->resetFlag == DSP_RESET_MODE_RESET_ALL_CLK) {
        setMask = CRU_CRU_SOFTRST_CON00_DRESETN_DSP_MASK |
                  CRU_CRU_SOFTRST_CON00_BRESETN_DSP_MASK |
                  CRU_CRU_SOFTRST_CON00_ARESETN_DSP_NIU_MASK;
        dsp->resetFlag = DSP_RESET_MODE_RESET_PART_CLK;
    } else {
        setMask = CRU_CRU_SOFTRST_CON00_DRESETN_DSP_MASK |
                  CRU_CRU_SOFTRST_CON00_BRESETN_DSP_MASK;
    }
    WRITE_REG_MASK_WE(CRU->CRU_SOFTRST_CON[0], setMask, setMask);

    if (altAddr) {
        WRITE_REG_MASK_WE(GRF->DSP_CON0, GRF_DSP_CON0_STATVECTORSEL_MASK,
                          GRF_DSP_CON0_STATVECTORSEL_MASK);
        WRITE_REG(GRF->DSP_CON1, altAddr);
    }

    /* Deassert reset */
    WRITE_REG_MASK_WE(CRU->CRU_SOFTRST_CON[0], setMask, 0);

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

#endif /* RKMCU_RK2108 && HAL_DSP_MODULE_ENABLED */

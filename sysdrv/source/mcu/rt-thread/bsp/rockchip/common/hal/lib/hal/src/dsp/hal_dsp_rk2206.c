/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(RKMCU_RK2206) && defined(HAL_DSP_MODULE_ENABLED)

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

#define TCM_SEL_ALIGN (64 * 1024)
#define TCM_MAX_NUM   (3)

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
    case DSP_IOCTL_SET_ITCM_SIZE:
    {
        uint32_t setBit = 0;
        if ((uint32_t)arg > 0) {
            setBit = GRF_SOC_CON0_DSP_TCM_SEL0_MASK;
        }
        WRITE_REG_MASK_WE(GRF->SOC_CON0,
                          GRF_SOC_CON0_DSP_TCM_SEL0_MASK, setBit);
        break;
    }
    case DSP_IOCTL_SET_DTCM_SIZE:
    {
        uint32_t sel = 0x7;
        uint32_t setBit = 0;
        uint32_t mask = 0;
        uint32_t num = ((uint32_t)arg + TCM_SEL_ALIGN) / (TCM_SEL_ALIGN + 1);

        if (num > TCM_MAX_NUM) {
            num = TCM_MAX_NUM;
        }
        num = TCM_MAX_NUM - num;
        sel = sel >> num;                   /* Get sel */

        if (sel & 0x1) {
            setBit |= GRF_SOC_CON0_DSP_TCM_SEL1_MASK;
        }
        if (sel & 0x2) {
            setBit |= GRF_SOC_CON0_DSP_TCM_SEL2_MASK;
        }
        if (sel & 0x4) {
            setBit |= GRF_SOC_CON0_DSP_TCM_SEL3_MASK;
        }

        mask |= GRF_SOC_CON0_DSP_TCM_SEL1_MASK |
                GRF_SOC_CON0_DSP_TCM_SEL2_MASK |
                GRF_SOC_CON0_DSP_TCM_SEL3_MASK;
        WRITE_REG_MASK_WE(GRF->SOC_CON0, mask, setBit);
        break;
    }
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
    case DSP_IOCTL_SET_DVFS_ST:
    {
        if ((bool)arg == true) {
            HAL_CRU_ClkDisable(CLK_HIFI3_GATE);
            HAL_CRU_ClkDisable(ACLK_HIFI3_NIU_GATE);
        } else {
            HAL_CRU_ClkEnable(ACLK_HIFI3_NIU_GATE);
            HAL_CRU_ClkEnable(CLK_HIFI3_GATE);
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

HAL_Status HAL_DSP_WaitForPowerSt(eDSP_powerSt status, uint32_t timeout)
{
    return HAL_OK;
}

HAL_Status HAL_DSP_SoftWakeup(void)
{
    return HAL_OK;
}

HAL_Status HAL_DSP_SetTcmMode(uint32_t tcmSel, eDSP_tcmMode mode)
{
    if (tcmSel > 0xffff) {
        return HAL_INVAL;
    }
    switch (mode) {
    case NOR_MODE:
        WRITE_REG_MASK_WE(GRF->SOC_CON14, tcmSel, 0);
        WRITE_REG_MASK_WE(GRF->SOC_CON13, tcmSel, 0);
        WRITE_REG_MASK_WE(GRF->SOC_CON12, tcmSel, 0);
        break;
    case RET1N_MODE:   /* nap mode */
        WRITE_REG_MASK_WE(GRF->SOC_CON14, tcmSel, 0);
        WRITE_REG_MASK_WE(GRF->SOC_CON13, tcmSel, 0);
        WRITE_REG_MASK_WE(GRF->SOC_CON12, tcmSel, tcmSel);
        break;
    case RET2N_MODE:  /* ret mode */
        WRITE_REG_MASK_WE(GRF->SOC_CON14, tcmSel, 0);
        WRITE_REG_MASK_WE(GRF->SOC_CON12, tcmSel, tcmSel);
        WRITE_REG_MASK_WE(GRF->SOC_CON13, tcmSel, tcmSel);
        break;
    case PWR_DOWN_MODE:
        WRITE_REG_MASK_WE(GRF->SOC_CON12, tcmSel, tcmSel);
        WRITE_REG_MASK_WE(GRF->SOC_CON13, tcmSel, tcmSel);
        WRITE_REG_MASK_WE(GRF->SOC_CON14, tcmSel, tcmSel);
        break;
    }

    return HAL_OK;
}

HAL_Status HAL_DSP_PowerOn(struct DSP_DEV *dsp)
{
    HAL_CRU_ClkEnable(HCLK_HIFI3_TCM_GATE);
    HAL_CRU_ClkEnable(ACLK_HIFI3_NIU_GATE);
    HAL_CRU_ClkEnable(CLK_HIFI3_GATE);

    return HAL_OK;
}

HAL_Status HAL_DSP_PowerOff(struct DSP_DEV *dsp)
{
    HAL_CRU_ClkDisable(CLK_HIFI3_GATE);
    HAL_CRU_ClkDisable(ACLK_HIFI3_NIU_GATE);
    HAL_CRU_ClkDisable(HCLK_HIFI3_TCM_GATE);

    return HAL_OK;
}

HAL_Status HAL_DSP_Init(struct DSP_DEV *dsp)
{
    dsp->ops = &dspOps;
    dsp->grfReg = (struct GRF_REG *)(GRF_BASE);
    dsp->resetFlag = DSP_RESET_MODE_RESET_ALL_CLK;

    dsp->mboxIsA2B = 1;
    dsp->mboxReg = MBOX1;
    dsp->errorIrq = DSP_ERROR_IRQn;
    dsp->mboxIrq[0] = B2A1_INT0_IRQn;
    dsp->mboxIrq[1] = B2A1_INT1_IRQn;
    dsp->mboxIrq[2] = B2A1_INT2_IRQn;
    dsp->mboxIrq[3] = B2A1_INT3_IRQn;
    dsp->clkId = CLK_HIFI3;

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
    setMask = CRU_SOFTRST_CON04_DRESETN_HIFI3_MASK |
              CRU_SOFTRST_CON04_BRESETN_HIFI3_MASK;
    WRITE_REG_MASK_WE(CRU->CRU_SOFTRST_CON[4], setMask, setMask);

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

#endif /* RKMCU_RK2206 && HAL_DSP_MODULE_ENABLED */

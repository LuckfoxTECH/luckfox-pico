/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_ACDCDIG_MODULE_ENABLED
#if (ACDCDIG_VERSION == 0x1U) /* ACDCDIGv1 */

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup ACDCDIG
 *  @{
 */

/** @defgroup ACDCDIG_How_To_Use How To Use
 *  @{

 The ACDCDIG driver can be used as follows:

 @} */

/** @defgroup ACDCDIG_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/
/* VUCTL */
#define ACDCDIG_VUCTL_ADCZDT_EN   (0x1U << ACDCDIG_VUCTL_ADCZDT_SHIFT)
#define ACDCDIG_VUCTL_ADCFADE_EN  (0x1U << ACDCDIG_VUCTL_ADCFADE_SHIFT)
#define ACDCDIG_VUCTL_ADC_BYPS_EN (0x1U << ACDCDIG_VUCTL_ADC_BYPS_SHIFT)
/* VUCTIME */
#define ACDCDIG_VUCTIME_VUCT(x) ((x) << ACDCDIG_VUCTIME_VUCT_SHIFT)
/* DIGEN */
#define ACDCDIG_DIGEN_I2STX_DIS (0x0U << ACDCDIG_DIGEN_I2STX_EN_SHIFT)
#define ACDCDIG_DIGEN_I2STX_EN  (0x1U << ACDCDIG_DIGEN_I2STX_EN_SHIFT)
#define ACDCDIG_DIGEN_ADC_DIS   (0x0U << ACDCDIG_DIGEN_ADC_EN_SHIFT)
#define ACDCDIG_DIGEN_ADC_EN    (0x1U << ACDCDIG_DIGEN_ADC_EN_SHIFT)
/* CLKCTRL */
#define ACDCDIG_CLKCTRL_FILTER_GATE_EN  (0x1U << ACDCDIG_CLKCTRL_FILTER_GATE_EN_SHIFT)
#define ACDCDIG_CLKCTRL_CKE_BCLK_DIS    (0x0U << ACDCDIG_CLKCTRL_CKE_BCLK_SHIFT)
#define ACDCDIG_CLKCTRL_CKE_BCLK_EN     (0x1U << ACDCDIG_CLKCTRL_CKE_BCLK_SHIFT)
#define ACDCDIG_CLKCTRL_I2STX_CKE_DIS   (0x0U << ACDCDIG_CLKCTRL_I2STX_CKE_SHIFT)
#define ACDCDIG_CLKCTRL_I2STX_CKE_EN    (0x1U << ACDCDIG_CLKCTRL_I2STX_CKE_SHIFT)
#define ACDCDIG_CLKCTRL_ADC_CKE_DIS     (0x0U << ACDCDIG_CLKCTRL_ADC_CKE_SHIFT)
#define ACDCDIG_CLKCTRL_ADC_CKE_EN      (0x1U << ACDCDIG_CLKCTRL_ADC_CKE_SHIFT)
#define ACDCDIG_CLKCTRL_CIC_DS_RATIO_16 (0x0U << ACDCDIG_CLKCTRL_CIC_DS_RATIO_SHIFT)
#define ACDCDIG_CLKCTRL_CIC_DS_RATIO_8  (0x1U << ACDCDIG_CLKCTRL_CIC_DS_RATIO_SHIFT)
#define ACDCDIG_CLKCTRL_CIC_DS_RATIO_4  (0x2U << ACDCDIG_CLKCTRL_CIC_DS_RATIO_SHIFT)
/* CLKDIV */
#define ACDCDIG_CLKDIV_INT_DIV_CON(x) ((x - 1) << ACDCDIG_CLKDIV_INT_DIV_CON_SHIFT)
/* REFCFG */
#define ACDCDIG_REFCFG_REF_TOP_TRIM(x) ((x) << ACDCDIG_REFCFG_REF_TOP_TRIM_SHIFT)
#define ACDCDIG_REFCFG_REF_ANA_TRIM(x) ((x) << ACDCDIG_REFCFG_REF_ANA_TRIM_SHIFT)
#define ACDCDIG_REFCFG_REF_TOP_PWU     (0x0U << ACDCDIG_REFCFG_REF_TOP_PWD_SHIFT)
#define ACDCDIG_REFCFG_REF_TOP_PWD     (0x1U << ACDCDIG_REFCFG_REF_TOP_PWD_SHIFT)
/* ADCCFG0 */
#define ACDCDIG_ADCCFG0_ADC_DITH_SEL(x)    ((x) << ACDCDIG_ADCCFG0_ADC_DITH_SEL_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_DITH_EN        (0x1U << ACDCDIG_ADCCFG0_ADC_DITH_EN_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_CHOP_CLK_1200K (0x0U << ACDCDIG_ADCCFG0_ADC_CHOP_CLK_SEL_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_CHOP_CLK_600K  (0x1U << ACDCDIG_ADCCFG0_ADC_CHOP_CLK_SEL_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_CHOP_EN        (0x1U << ACDCDIG_ADCCFG0_ADC_CHOP_EN_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_PWU_R          (0x0U << ACDCDIG_ADCCFG0_ADC_PWD_R_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_PWD_R          (0x1U << ACDCDIG_ADCCFG0_ADC_PWD_R_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_PWU_L          (0x0U << ACDCDIG_ADCCFG0_ADC_PWD_L_SHIFT)
#define ACDCDIG_ADCCFG0_ADC_PWD_L          (0x1U << ACDCDIG_ADCCFG0_ADC_PWD_L_SHIFT)
/* ADCCFG1 */
#define ACDCDIG_ADCCFG1_ADC_BOOST_VAGOP   (0x1U << ACDCDIG_ADCCFG1_ADC_BOOST_VAGOP_SHIFT)
#define ACDCDIG_ADCCFG1_ADC_BOOST_OPAMP   (0x1U << ACDCDIG_ADCCFG1_ADC_BOOST_OPAMP_SHIFT)
#define ACDCDIG_ADCCFG1_ADC_ATTN_OPBIAS   (0x1U << ACDCDIG_ADCCFG1_ADC_ATTN_OPBIAS_SHIFT)
#define ACDCDIG_ADCCFG1_ADC_ATTN_ALLIBIAS (0x1U << ACDCDIG_ADCCFG1_ADC_ATTN_ALLIBIAS_SHIFT)
/* ADCCFG2 */
#define ACDCDIG_ADCCFG2_CT_ADC_BOOST   (0x1U << ACDCDIG_ADCCFG2_CT_ADC_BOOST_SHIFT)
#define ACDCDIG_ADCCFG2_CT_ADC_ATTN    (0x1U << ACDCDIG_ADCCFG2_CT_ADC_ATTN_SHIFT)
#define ACDCDIG_ADCCFG2_CT_ADC_ATTN_OP (0x1U << ACDCDIG_ADCCFG2_CT_ADC_ATTN_OP_SHIFT)
#define ACDCDIG_ADCCFG2_ADC_OUT_SEL_DS (0x0U << ACDCDIG_ADCCFG2_ADC_OUT_SEL_SHIFT)
#define ACDCDIG_ADCCFG2_ADC_OUT_SEL_CT (0x1U << ACDCDIG_ADCCFG2_ADC_OUT_SEL_SHIFT)
#define ACDCDIG_ADCCFG2_CT_ADC_ZERO    (0x1U << ACDCDIG_ADCCFG2_CT_ADC_ZERO_SHIFT)
#define ACDCDIG_ADCCFG2_CT_ADC_PWU     (0x0U << ACDCDIG_ADCCFG2_CT_ADC_PWD_SHIFT)
#define ACDCDIG_ADCCFG2_CT_ADC_PWD     (0x1U << ACDCDIG_ADCCFG2_CT_ADC_PWD_SHIFT)
/* ADCCFG3 */
#define ACDCDIG_ADCCFG3_FIR_COM_BPS    (0x1U << ACDCDIG_ADCCFG3_FIR_COM_BPS_SHIFT)
#define ACDCDIG_ADCCFG3_SIG_SCALE_MODE (0x1U << ACDCDIG_ADCCFG3_SIG_SCALE_MODE_SHIFT)
#define ACDCDIG_ADCCFG3_ADCSRT(x)      ((x) << ACDCDIG_ADCCFG3_ADCSRT_SHIFT)
/* ADCVOLL */
#define ACDCDIG_ADCVOLL_ADCLV(x) ((x) << ACDCDIG_ADCVOLL_ADCLV_SHIFT)
/* ADCVOLR */
#define ACDCDIG_ADCVOLR_ADCRV(x) ((x) << ACDCDIG_ADCVOLR_ADCRV_SHIFT)
/* ALC0 */
#define ACDCDIG_ALC0_ADCRV_GAIN_POL_NEGATIVE (0x0U << ACDCDIG_ALC0_ADCRV_GAIN_POL_SHIFT)
#define ACDCDIG_ALC0_ADCRV_GAIN_POL_POSITIVE (0x1U << ACDCDIG_ALC0_ADCRV_GAIN_POL_SHIFT)
#define ACDCDIG_ALC0_ADCLV_GAIN_POL_NEGATIVE (0x0U << ACDCDIG_ALC0_ADCLV_GAIN_POL_SHIFT)
#define ACDCDIG_ALC0_ADCLV_GAIN_POL_POSITIVE (0x1U << ACDCDIG_ALC0_ADCLV_GAIN_POL_SHIFT)
#define ACDCDIG_ALC0_ALCR_EN                 (0x1U << ACDCDIG_ALC0_ALCR_SHIFT)
#define ACDCDIG_ALC0_ALCL_EN                 (0x1U << ACDCDIG_ALC0_ALCL_SHIFT)
/* ALC1 */
#define ACDCDIG_ALC1_ALCRRATE(x) ((x) << ACDCDIG_ALC1_ALCRRATE_SHIFT)
#define ACDCDIG_ALC1_ALCARATE(x) ((x) << ACDCDIG_ALC1_ALCARATE_SHIFT)
/* ALC2 */
#define ACDCDIG_ALC2_ALCMIN(x)  ((x) << ACDCDIG_ALC2_ALCMIN_SHIFT)
#define ACDCDIG_ALC2_ALCMAX(x)  ((x) << ACDCDIG_ALC2_ALCMAX_SHIFT)
#define ACDCDIG_ALC2_NGVALID_NG (0x1U << ACDCDIG_ALC2_NGVALID_SHIFT)
/* ADCNG */
#define ACDCDIG_ADCNG_NGDLY(x)      ((x) << ACDCDIG_ADCNG_NGDLY_SHIFT)
#define ACDCDIG_ADCNG_NGGATE(x)     ((x) << ACDCDIG_ADCNG_NGGATE_SHIFT)
#define ACDCDIG_ADCNG_NGBOOST_BOOST (0x1U << ACDCDIG_ADCNG_NGBOOST_SHIFT)
#define ACDCDIG_ADCNG_NGEN          (0x1U << ACDCDIG_ADCNG_NGEN_SHIFT)
#define ACDCDIG_ADCNG_NGCHL_BOTH    (0x1U << ACDCDIG_ADCNG_NGCHL_SHIFT)
/* HPFCTRL */
#define ACDCDIG_HPFCTRL_HPFCF(x) ((x) << ACDCDIG_HPFCTRL_HPFCF_SHIFT)
#define ACDCDIG_HPFCTRL_HPFRE_EN (0x1U << ACDCDIG_HPFCTRL_HPFRE_SHIFT)
#define ACDCDIG_HPFCTRL_HPFLE_EN (0x1U << ACDCDIG_HPFCTRL_HPFLE_SHIFT)
/* PGACFG */
#define ACDCDIG_PGACFG_PGA_R_DEC(x) ((x / 3) << ACDCDIG_PGACFG_PGA_R_DEC_SHIFT)
#define ACDCDIG_PGACFG_PGA_L_DEC(x) ((x / 3) << ACDCDIG_PGACFG_PGA_L_DEC_SHIFT)
#define ACDCDIG_PGACFG_PGA_R_PU     (0x0U << ACDCDIG_PGACFG_PGA_R_PD_SHIFT)
#define ACDCDIG_PGACFG_PGA_R_PD     (0x1U << ACDCDIG_PGACFG_PGA_R_PD_SHIFT)
#define ACDCDIG_PGACFG_PGA_L_PU     (0x0U << ACDCDIG_PGACFG_PGA_L_PD_SHIFT)
#define ACDCDIG_PGACFG_PGA_L_PD     (0x1U << ACDCDIG_PGACFG_PGA_L_PD_SHIFT)
#define ACDCDIG_PGACFG_PGA_CHOP_EN  (0x1U << ACDCDIG_PGACFG_PGA_CHOP_EN_SHIFT)
/* PGAGAINL */
#define ACDCDIG_PGAGAINL_PGA_GAIN_L(x) ((x) << ACDCDIG_PGAGAINL_PGA_GAIN_L_SHIFT)
/* PGAGAINR */
#define ACDCDIG_PGAGAINR_PGA_GAIN_R(x) ((x) << ACDCDIG_PGAGAINR_PGA_GAIN_R_SHIFT)
/* LILMT1 */
#define ACDCDIG_LILMT1_MIN_LILMT(x) ((x) << ACDCDIG_LILMT1_MIN_LILMT_SHIFT)
#define ACDCDIG_LILMT1_MAX_LILMT(x) ((x) << ACDCDIG_LILMT1_MAX_LILMT_SHIFT)
#define ACDCDIG_LILMT1_LMT_EN       (0x1U << ACDCDIG_LILMT1_LMT_EN_SHIFT)
/* LILMT2 */
#define ACDCDIG_LILMT2_RLS_RATE(x) ((x) << ACDCDIG_LILMT2_RLS_RATE_SHIFT)
#define ACDCDIG_LILMT2_ATK_RATE(x) ((x) << ACDCDIG_LILMT2_ATK_RATE_SHIFT)
/* LILMTNG1 */
#define ACDCDIG_LILMTNG1_NGDLY_LI(x)      ((x) << ACDCDIG_LILMTNG1_NGDLY_LI_SHIFT)
#define ACDCDIG_LILMTNG1_NGGATE_LI(x)     ((x) << ACDCDIG_LILMTNG1_NGGATE_LI_SHIFT)
#define ACDCDIG_LILMTNG1_NGBOOST_LI_BOOST (0x1U << ACDCDIG_LILMTNG1_NGBOOST_LI_SHIFT)
#define ACDCDIG_LILMTNG1_NGEN_LI_EN       (0x1U << ACDCDIG_LILMTNG1_NGEN_LI_SHIFT)
#define ACDCDIG_LILMTNG1_NGCHL_LI_BOTH    (0x1U << ACDCDIG_LILMTNG1_NGCHL_LI_SHIFT)
/* LILMTNG2 */
#define ACDCDIG_LILMTNG2_NGVALID_LI_NG (0x1U << ACDCDIG_LILMTNG2_NGVALID_LI_SHIFT)
/* PDMCTRL */
#define ACDCDIG_PDMCTRL_PDM_LR_SWITCH (0x1U << ACDCDIG_PDMCTRL_PDM_LR_SEL_SHIFT)
#define ACDCDIG_PDMCTRL_PDM_MODE_I2S  (0x0U << ACDCDIG_PDMCTRL_PDM_MODE_SHIFT)
#define ACDCDIG_PDMCTRL_PDM_MODE_PDM  (0x1U << ACDCDIG_PDMCTRL_PDM_MODE_SHIFT)
#define ACDCDIG_PDMCTRL_PDM_DIS       (0x1U << ACDCDIG_PDMCTRL_PDM_EN_SHIFT)
#define ACDCDIG_PDMCTRL_PDM_EN        (0x1U << ACDCDIG_PDMCTRL_PDM_EN_SHIFT)
/* I2SCKM */
#define ACDCDIG_I2SCKM_I2SMST_SLAVE  (0x0U << ACDCDIG_I2SCKM_I2SMST_SHIFT)
#define ACDCDIG_I2SCKM_I2SMST_MASTER (0x1U << ACDCDIG_I2SCKM_I2SMST_SHIFT)
#define ACDCDIG_I2SCKM_SCK_P_INV     (0x1U << ACDCDIG_I2SCKM_SCK_P_SHIFT)
/* I2SDIV */
#define ACDCDIG_I2SDIV_SCKDIV(x) ((x) << ACDCDIG_I2SDIV_SCKDIV_SHIFT)
/* I2STXCR0 */
#define ACDCDIG_I2STXCR0_TXRL_P_INV (0x1U << ACDCDIG_I2STXCR0_TXRL_P_SHIFT)
#define ACDCDIG_I2STXCR0_SCKDTX_64  (0x0U << ACDCDIG_I2STXCR0_SCKDTX_SHIFT)
#define ACDCDIG_I2STXCR0_SCKDTX_128 (0x1U << ACDCDIG_I2STXCR0_SCKDTX_SHIFT)
#define ACDCDIG_I2STXCR0_SCKDTX_256 (0x2U << ACDCDIG_I2STXCR0_SCKDTX_SHIFT)
/* I2STXCR1 */
#define ACDCDIG_I2STXCR1_LSB        (0x1U << ACDCDIG_I2STXCR1_LSB_SHIFT)
#define ACDCDIG_I2STXCR1_SWITCH     (0x1U << ACDCDIG_I2STXCR1_EXRL_SHIFT)
#define ACDCDIG_I2STXCR1_IBM_NORMAL (0x0U << ACDCDIG_I2STXCR1_IBM_SHIFT)
#define ACDCDIG_I2STXCR1_IBM_LJ     (0x1U << ACDCDIG_I2STXCR1_IBM_SHIFT)
#define ACDCDIG_I2STXCR1_IBM_RJ     (0x2U << ACDCDIG_I2STXCR1_IBM_SHIFT)
#define ACDCDIG_I2STXCR1_PBM_EARLY  (0x0U << ACDCDIG_I2STXCR1_PBM_SHIFT)
#define ACDCDIG_I2STXCR1_PBM_LATE1  (0x1U << ACDCDIG_I2STXCR1_PBM_SHIFT)
#define ACDCDIG_I2STXCR1_PBM_LATE2  (0x2U << ACDCDIG_I2STXCR1_PBM_SHIFT)
#define ACDCDIG_I2STXCR1_PBM_LATE3  (0x3U << ACDCDIG_I2STXCR1_PBM_SHIFT)
#define ACDCDIG_I2STXCR1_TFS_I2S    (0x0U << ACDCDIG_I2STXCR1_TFS_SHIFT)
#define ACDCDIG_I2STXCR1_TFS_PCM    (0x1U << ACDCDIG_I2STXCR1_TFS_SHIFT)
/* I2STXCR2 */
#define ACDCDIG_I2STXCR2_VDW(x) ((x - 1) << ACDCDIG_I2STXCR2_VDW_SHIFT)
/* I2STXCMD */
#define ACDCDIG_I2STXCMD_RCNT(x)   ((x) << ACDCDIG_I2STXCMD_RCNT_SHIFT)
#define ACDCDIG_I2STXCMD_TXC       (0x1U << ACDCDIG_I2STXCMD_TXC_SHIFT)
#define ACDCDIG_I2STXCMD_TXS_STOP  (0x0U << ACDCDIG_I2STXCMD_TXS_SHIFT)
#define ACDCDIG_I2STXCMD_TXS_START (0x1U << ACDCDIG_I2STXCMD_TXS_SHIFT)

/**
  * The range of gains.
  * To avoid using decimals, the scaled 1000 times dB is like Linux ALSA TLV.
  * min: -95.625dB, max: 95.625dB, step: 0.375dB.
  */
#define ACDCDIG_DB_MIN  (-95625)
#define ACDCDIG_DB_MAX  (95625)
#define ACDCDIG_DB_STEP (375)

/* The number of ADC. */
#define ACDCDIG_ADC_NUM 2
/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/
/** @defgroup ACDCDIG_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  acdcDig suspend.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Supsend(struct HAL_ACDCDIG_DEV *acdcDig)
{
    return HAL_OK;
}

/**
 * @brief  acdcDig resume.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Resume(struct HAL_ACDCDIG_DEV *acdcDig)
{
    return HAL_OK;
}

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group2 State and Errors Functions

 This section provides functions allowing to get the status of the module:

 *  @{
 */

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 ...to do or delete this row

 *  @{
 */

/**
 * @brief  Init acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  config: init config for i2s init.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Init(struct HAL_ACDCDIG_DEV *acdcDig, struct AUDIO_INIT_CONFIG *config)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;

    HAL_CRU_ClkEnable(acdcDig->hclk);

    acdcDig->format = config->format;

    switch (config->format) {
    case AUDIO_FMT_I2S:
        MODIFY_REG(reg->PDMCTRL, ACDCDIG_PDMCTRL_PDM_MODE_MASK,
                   ACDCDIG_PDMCTRL_PDM_MODE_I2S);
        if (config->master) {
            MODIFY_REG(reg->I2SCKM, ACDCDIG_I2SCKM_I2SMST_MASK,
                       ACDCDIG_I2SCKM_I2SMST_MASK);
        } else {
            MODIFY_REG(reg->I2SCKM, ACDCDIG_I2SCKM_I2SMST_MASK,
                       ACDCDIG_I2SCKM_I2SMST_SLAVE);
        }
        break;
    case AUDIO_FMT_PDM:
        MODIFY_REG(reg->PDMCTRL, ACDCDIG_PDMCTRL_PDM_MODE_MASK,
                   ACDCDIG_PDMCTRL_PDM_MODE_PDM);
        break;
    default:
        ret = HAL_INVAL;
        break;
    }

    /* PGA L/R 3DB */
    MODIFY_REG(reg->PGAGAINL, ACDCDIG_PGAGAINL_PGA_GAIN_L_MASK,
               ACDCDIG_PGAGAINL_PGA_GAIN_L(2));
    MODIFY_REG(reg->PGAGAINR, ACDCDIG_PGAGAINR_PGA_GAIN_R_MASK,
               ACDCDIG_PGAGAINR_PGA_GAIN_R(2));
    /* adc dith en */
    MODIFY_REG(reg->ADCCFG0, ACDCDIG_ADCCFG0_ADC_DITH_EN_MASK,
               ACDCDIG_ADCCFG0_ADC_DITH_EN);
    /* pga chop en */
    MODIFY_REG(reg->PGACFG, ACDCDIG_PGACFG_PGA_CHOP_EN_MASK,
               ACDCDIG_PGACFG_PGA_CHOP_EN);
    /* hpf en */
    MODIFY_REG(reg->HPFCTRL, ACDCDIG_HPFCTRL_HPFLE_MASK | ACDCDIG_HPFCTRL_HPFRE_MASK,
               ACDCDIG_HPFCTRL_HPFLE_EN | ACDCDIG_HPFCTRL_HPFRE_EN);
    /* opamp bias current: 160% */
    MODIFY_REG(reg->REFCFG, ACDCDIG_REFCFG_REF_TOP_TRIM_MASK,
               ACDCDIG_REFCFG_REF_TOP_TRIM(0x9));

    return ret;
}

/**
 * @brief  DeInit acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_DeInit(struct HAL_ACDCDIG_DEV *acdcDig)
{
    HAL_ACDCDIG_Disable(acdcDig, AUDIO_STREAM_CAPTURE);

    HAL_CRU_ClkDisable(acdcDig->hclk);

    return HAL_OK;
}

/** @} */

/** @defgroup ACDCDIG_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Enable acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Enable(struct HAL_ACDCDIG_DEV *acdcDig,
                              eAUDIO_streamType stream)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;

    HAL_ASSERT(stream == AUDIO_STREAM_CAPTURE);

    /* power up */
    MODIFY_REG(reg->REFCFG,
               ACDCDIG_REFCFG_REF_TOP_PWD_MASK, ACDCDIG_REFCFG_REF_TOP_PWU);
    MODIFY_REG(reg->ADCCFG0,
               ACDCDIG_ADCCFG0_ADC_PWD_L_MASK | ACDCDIG_ADCCFG0_ADC_PWD_R_MASK,
               ACDCDIG_ADCCFG0_ADC_PWU_L | ACDCDIG_ADCCFG0_ADC_PWU_R);
    MODIFY_REG(reg->PGACFG,
               ACDCDIG_PGACFG_PGA_L_PD_MASK | ACDCDIG_PGACFG_PGA_R_PD_MASK,
               ACDCDIG_PGACFG_PGA_L_PU | ACDCDIG_PGACFG_PGA_R_PU);

    switch (acdcDig->format) {
    case AUDIO_FMT_I2S:
        MODIFY_REG(reg->CLKCTRL,
                   ACDCDIG_CLKCTRL_ADC_CKE_MASK |
                   ACDCDIG_CLKCTRL_I2STX_CKE_MASK |
                   ACDCDIG_CLKCTRL_CKE_BCLK_MASK |
                   ACDCDIG_CLKCTRL_FILTER_GATE_EN_MASK,
                   ACDCDIG_CLKCTRL_ADC_CKE_EN |
                   ACDCDIG_CLKCTRL_I2STX_CKE_EN |
                   ACDCDIG_CLKCTRL_CKE_BCLK_EN |
                   ACDCDIG_CLKCTRL_FILTER_GATE_EN);
        MODIFY_REG(reg->DIGEN,
                   ACDCDIG_DIGEN_ADC_EN_MASK |
                   ACDCDIG_DIGEN_I2STX_EN_MASK,
                   ACDCDIG_DIGEN_ADC_EN |
                   ACDCDIG_DIGEN_I2STX_EN);
        MODIFY_REG(reg->I2STXCMD, ACDCDIG_I2STXCMD_TXS_MASK,
                   ACDCDIG_I2STXCMD_TXS_START);
        break;
    case AUDIO_FMT_PDM:
        MODIFY_REG(reg->PDMCTRL, ACDCDIG_PDMCTRL_PDM_EN_MASK,
                   ACDCDIG_PDMCTRL_PDM_EN);
        break;
    default:
        ret = HAL_INVAL;
        break;
    }

    return ret;
}

/**
 * @brief  Disable acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Disable(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;

    HAL_ASSERT(stream == AUDIO_STREAM_CAPTURE);

    switch (acdcDig->format) {
    case AUDIO_FMT_I2S:
        MODIFY_REG(reg->I2STXCMD, ACDCDIG_I2STXCMD_TXS_MASK,
                   ACDCDIG_I2STXCMD_TXS_STOP);
        MODIFY_REG(reg->I2STXCMD, ACDCDIG_I2STXCMD_TXC_MASK,
                   ACDCDIG_I2STXCMD_TXC);
        MODIFY_REG(reg->DIGEN,
                   ACDCDIG_DIGEN_ADC_EN_MASK |
                   ACDCDIG_DIGEN_I2STX_EN_MASK,
                   ACDCDIG_DIGEN_ADC_DIS |
                   ACDCDIG_DIGEN_I2STX_DIS);
        MODIFY_REG(reg->CLKCTRL,
                   ACDCDIG_CLKCTRL_ADC_CKE_MASK |
                   ACDCDIG_CLKCTRL_I2STX_CKE_MASK |
                   ACDCDIG_CLKCTRL_CKE_BCLK_MASK,
                   ACDCDIG_CLKCTRL_ADC_CKE_DIS |
                   ACDCDIG_CLKCTRL_I2STX_CKE_DIS |
                   ACDCDIG_CLKCTRL_CKE_BCLK_DIS);
        break;
    case AUDIO_FMT_PDM:
        MODIFY_REG(reg->PDMCTRL, ACDCDIG_PDMCTRL_PDM_EN_MASK,
                   ACDCDIG_PDMCTRL_PDM_DIS);
        break;
    default:
        ret = HAL_INVAL;
        break;
    }

    /* power down */
    MODIFY_REG(reg->ADCCFG0,
               ACDCDIG_ADCCFG0_ADC_PWD_L_MASK | ACDCDIG_ADCCFG0_ADC_PWD_R_MASK,
               ACDCDIG_ADCCFG0_ADC_PWD_L | ACDCDIG_ADCCFG0_ADC_PWD_R);
    MODIFY_REG(reg->PGACFG,
               ACDCDIG_PGACFG_PGA_L_PD_MASK | ACDCDIG_PGACFG_PGA_R_PD_MASK,
               ACDCDIG_PGACFG_PGA_L_PD | ACDCDIG_PGACFG_PGA_R_PD);
    MODIFY_REG(reg->REFCFG,
               ACDCDIG_REFCFG_REF_TOP_PWD_MASK, ACDCDIG_REFCFG_REF_TOP_PWD);

    return ret;
}

/**
 * @brief  Config acdcDig controller.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  params: audio params.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_Config(struct HAL_ACDCDIG_DEV *acdcDig,
                              eAUDIO_streamType stream,
                              struct AUDIO_PARAMS *params)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;
    uint32_t srt = 0;

    HAL_ASSERT(stream == AUDIO_STREAM_CAPTURE);

    switch (params->sampleRate) {
    case AUDIO_SAMPLERATE_8000:
    case AUDIO_SAMPLERATE_11025:
        srt = 0;
        break;
    case AUDIO_SAMPLERATE_16000:
    case AUDIO_SAMPLERATE_22050:
        srt = 1;
        break;
    case AUDIO_SAMPLERATE_32000:
        srt = 2;
        break;
    case AUDIO_SAMPLERATE_44100:
    case AUDIO_SAMPLERATE_48000:
        srt = 3;
        break;
    case AUDIO_SAMPLERATE_64000:
    case AUDIO_SAMPLERATE_88200:
    case AUDIO_SAMPLERATE_96000:
        srt = 4;
        break;
    case AUDIO_SAMPLERATE_176400:
    case AUDIO_SAMPLERATE_192000:
        srt = 5;
        break;
    default:
        ret = HAL_INVAL;
        srt = 0;
        break;
    }

    MODIFY_REG(reg->ADCCFG3, ACDCDIG_ADCCFG3_ADCSRT_MASK,
               ACDCDIG_ADCCFG3_ADCSRT(srt));

    switch (params->sampleBits) {
    case AUDIO_SAMPLEBITS_16:
        MODIFY_REG(reg->I2STXCR2, ACDCDIG_I2STXCR2_VDW_MASK,
                   ACDCDIG_I2STXCR2_VDW(16));
        break;
    case AUDIO_SAMPLEBITS_32:
        MODIFY_REG(reg->I2STXCR2, ACDCDIG_I2STXCR2_VDW_MASK,
                   ACDCDIG_I2STXCR2_VDW(24));
        break;
    default:
        ret = HAL_INVAL;
        break;
    }

    return ret;
}

/**
 * @brief  Set Gain for acdcDig.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  dBConfig: The gains config of codec, it includes ADC/DAC channel,
 *                   and the dB range is: -95625(-95.625dB) ~ 95625(95.625dB).
 *                   Similar to Linux ALSA TLV dB scale, they are scaled 1000
 *                   times to avoid representation of fractional parts.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_SetGain(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream,
                               struct AUDIO_DB_CONFIG *dBConfig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    uint32_t mask, val;
    int dB;

    HAL_ASSERT(dBConfig != NULL);

    dB = dBConfig->dB;

    switch (stream) {
    case AUDIO_STREAM_PLAYBACK:
        break;
    case AUDIO_STREAM_CAPTURE:
        HAL_ASSERT(dBConfig->ch <= ACDCDIG_ADC_NUM);

        if (dB > 0) {
            if (dB > ACDCDIG_DB_MAX) {
                dB = ACDCDIG_DB_MAX;
            }

            dB = dB / ACDCDIG_DB_STEP;

            if (dBConfig->ch == 0) {
                mask = ACDCDIG_ALC0_ADCLV_GAIN_POL_MASK;
                val = ACDCDIG_ALC0_ADCLV_GAIN_POL_POSITIVE;
                MODIFY_REG(reg->ADCVOLL, ACDCDIG_ADCVOLL_ADCLV_MASK,
                           ACDCDIG_ADCVOLL_ADCLV(dB));
            } else if (dBConfig->ch == 1) {
                mask = ACDCDIG_ALC0_ADCRV_GAIN_POL_MASK;
                val = ACDCDIG_ALC0_ADCRV_GAIN_POL_POSITIVE;
                MODIFY_REG(reg->ADCVOLR, ACDCDIG_ADCVOLR_ADCRV_MASK,
                           ACDCDIG_ADCVOLR_ADCRV(dB));
            } else {
                /* The dBConfig->ch == 2 and set all ADC gains. */
                mask = ACDCDIG_ALC0_ADCLV_GAIN_POL_MASK |
                       ACDCDIG_ALC0_ADCRV_GAIN_POL_MASK;
                val = ACDCDIG_ALC0_ADCLV_GAIN_POL_POSITIVE |
                      ACDCDIG_ALC0_ADCRV_GAIN_POL_POSITIVE;
                MODIFY_REG(reg->ADCVOLL, ACDCDIG_ADCVOLL_ADCLV_MASK,
                           ACDCDIG_ADCVOLL_ADCLV(dB));
                MODIFY_REG(reg->ADCVOLR, ACDCDIG_ADCVOLR_ADCRV_MASK,
                           ACDCDIG_ADCVOLR_ADCRV(dB));
            }
        } else {
            if (dB < ACDCDIG_DB_MIN) {
                dB = ACDCDIG_DB_MIN;
            }

            dB = -(dB / ACDCDIG_DB_STEP);

            if (dBConfig->ch == 0) {
                mask = ACDCDIG_ALC0_ADCLV_GAIN_POL_MASK;
                val = ACDCDIG_ALC0_ADCLV_GAIN_POL_NEGATIVE;
                MODIFY_REG(reg->ADCVOLL, ACDCDIG_ADCVOLL_ADCLV_MASK,
                           ACDCDIG_ADCVOLL_ADCLV(dB));
            } else if (dBConfig->ch == 1) {
                mask = ACDCDIG_ALC0_ADCRV_GAIN_POL_MASK;
                val = ACDCDIG_ALC0_ADCRV_GAIN_POL_NEGATIVE;
                MODIFY_REG(reg->ADCVOLR, ACDCDIG_ADCVOLR_ADCRV_MASK,
                           ACDCDIG_ADCVOLR_ADCRV(dB));
            } else {
                /* The dBConfig->ch == 2 and set all ADC gains. */
                mask = ACDCDIG_ALC0_ADCLV_GAIN_POL_MASK |
                       ACDCDIG_ALC0_ADCRV_GAIN_POL_MASK;
                val = ACDCDIG_ALC0_ADCLV_GAIN_POL_NEGATIVE |
                      ACDCDIG_ALC0_ADCRV_GAIN_POL_NEGATIVE;
                MODIFY_REG(reg->ADCVOLL, ACDCDIG_ADCVOLL_ADCLV_MASK,
                           ACDCDIG_ADCVOLL_ADCLV(dB));
                MODIFY_REG(reg->ADCVOLR, ACDCDIG_ADCVOLR_ADCRV_MASK,
                           ACDCDIG_ADCVOLR_ADCRV(dB));
            }
        }

        MODIFY_REG(reg->ALC0, mask, val);
        break;
    default:

        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
 * @brief  Get Gain for acdcDig.
 * @param  acdcDig: the handle of acdcDig.
 * @param  stream: audio stream type.
 * @param  dBConfig: The gains config of codec, it includes ADC/DAC channel,
 *                   and the dB range is: -95625(-95.625dB) ~ 95625(95.625dB).
 *                   Similar to Linux ALSA TLV dB scale, they are scaled 1000
 *                   times to avoid representation of fractional parts.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_GetGain(struct HAL_ACDCDIG_DEV *acdcDig,
                               eAUDIO_streamType stream,
                               struct AUDIO_DB_CONFIG *dBConfig)
{
    struct ACDCDIG_REG *reg = acdcDig->pReg;
    HAL_Status ret = HAL_OK;
    uint8_t vol = 0;

    HAL_ASSERT(dBConfig != NULL);

    switch (stream) {
    case AUDIO_STREAM_PLAYBACK:
        break;
    case AUDIO_STREAM_CAPTURE:
        /* Need to specify ADC channel here. */
        HAL_ASSERT(dBConfig->ch < ACDCDIG_ADC_NUM);

        /* There are the same volumes for 2ADCs */
        if (dBConfig->ch == 0) {
            vol = READ_REG(reg->ADCVOLL) & ACDCDIG_ADCVOLL_ADCLV_MASK;
            if (READ_BIT(reg->ALC0, ACDCDIG_ALC0_ADCLV_GAIN_POL_MASK)) {
                dBConfig->dB = vol * ACDCDIG_DB_STEP; /* Positive gain */
            } else {
                dBConfig->dB = -(vol * ACDCDIG_DB_STEP); /* Negative gain */
            }
        } else {  /* ch == 1 */
            vol = READ_REG(reg->ADCVOLR) & ACDCDIG_ADCVOLR_ADCRV_MASK;
            if (READ_BIT(reg->ALC0, ACDCDIG_ALC0_ADCRV_GAIN_POL_MASK)) {
                dBConfig->dB = vol * ACDCDIG_DB_STEP; /* Positive gain */
            } else {
                dBConfig->dB = -(vol * ACDCDIG_DB_STEP); /* Negative gain */
            }
        }
        break;
    default:
        ret = HAL_ERROR;
    }

    return ret;
}

/**
 * @brief  Get Gain informations from acdcDig.
 * @param  acdcDig: the handle of acdcDig.
 * @param  info: the handle of gain info.
 * @return HAL_Status
 */
HAL_Status HAL_ACDCDIG_GetGainInfo(struct HAL_ACDCDIG_DEV *acdcDig,
                                   struct AUDIO_GAIN_INFO *info)
{
    if (!info) {
        return HAL_ERROR;
    }

    info->mindB = ACDCDIG_DB_MIN;
    info->maxdB = ACDCDIG_DB_MAX;
    info->step = ACDCDIG_DB_STEP;

    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* (ACDCDIG_VERSION == 0x1U) */
#endif /* HAL_ACDCDIG_MODULE_ENABLED */

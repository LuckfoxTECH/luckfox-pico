/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK2106_ACODEC_H
#define __RK2106_ACODEC_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* ACODEC Register Structure Define */
struct ACODEC_REG {
         uint32_t RESERVED0[16];                      /* Address Offset: 0x0000 */
    __IO uint32_t VCTL;                               /* Address Offset: 0x0040 */
    __IO uint32_t VCTIME;                             /* Address Offset: 0x0044 */
    __IO uint32_t LPST;                               /* Address Offset: 0x0048 */
    __IO uint32_t LPT;                                /* Address Offset: 0x004C */
         uint32_t RESERVED1[1];                       /* Address Offset: 0x0050 */
    __IO uint32_t SRST;                               /* Address Offset: 0x0054 */
    __IO uint32_t DIGEN;                              /* Address Offset: 0x0058 */
         uint32_t RESERVED2[1];                       /* Address Offset: 0x005C */
    __IO uint32_t CLKE;                               /* Address Offset: 0x0060 */
         uint32_t RESERVED3[7];                       /* Address Offset: 0x0064 */
    __IO uint32_t RTCFG0;                             /* Address Offset: 0x0080 */
    __IO uint32_t RTCFG1;                             /* Address Offset: 0x0084 */
    __IO uint32_t RTCFG2;                             /* Address Offset: 0x0088 */
         uint32_t RESERVED4[13];                      /* Address Offset: 0x008C */
    __IO uint32_t ADCCFG0;                            /* Address Offset: 0x00C0 */
    __IO uint32_t ADCCFG1;                            /* Address Offset: 0x00C4 */
    __IO uint32_t ADCVCTLL;                           /* Address Offset: 0x00C8 */
    __IO uint32_t ADCVCTLR;                           /* Address Offset: 0x00CC */
    __IO uint32_t ADCSR;                              /* Address Offset: 0x00D0 */
    __IO uint32_t ALC0;                               /* Address Offset: 0x00D4 */
    __IO uint32_t ALC1;                               /* Address Offset: 0x00D8 */
    __IO uint32_t ALC2;                               /* Address Offset: 0x00DC */
    __IO uint32_t ADCNG;                              /* Address Offset: 0x00E0 */
    __IO uint32_t ADCNGST;                            /* Address Offset: 0x00E4 */
    __IO uint32_t ADCHPF;                             /* Address Offset: 0x00E8 */
    __IO uint32_t ADCVSTL;                            /* Address Offset: 0x00EC */
    __IO uint32_t ADCVSTR;                            /* Address Offset: 0x00F0 */
         uint32_t RESERVED5[3];                       /* Address Offset: 0x00F4 */
    __IO uint32_t DACACFG0;                           /* Address Offset: 0x0100 */
    __IO uint32_t DACACFG1;                           /* Address Offset: 0x0104 */
    __IO uint32_t DACACFG2;                           /* Address Offset: 0x0108 */
         uint32_t RESERVED6[13];                      /* Address Offset: 0x010C */
    __IO uint32_t DACPOPD;                            /* Address Offset: 0x0140 */
    __IO uint32_t DACST;                              /* Address Offset: 0x0144 */
    __IO uint32_t DACVCTLL;                           /* Address Offset: 0x0148 */
    __IO uint32_t DACVCTLR;                           /* Address Offset: 0x014C */
    __IO uint32_t DACSR;                              /* Address Offset: 0x0150 */
    __IO uint32_t LMT0;                               /* Address Offset: 0x0154 */
    __IO uint32_t LMT1;                               /* Address Offset: 0x0158 */
    __IO uint32_t LMT2;                               /* Address Offset: 0x015C */
    __IO uint32_t DACMUTE;                            /* Address Offset: 0x0160 */
    __IO uint32_t MIXCTRL;                            /* Address Offset: 0x0164 */
    __IO uint32_t DACVSTL;                            /* Address Offset: 0x0168 */
    __IO uint32_t DACVSTR;                            /* Address Offset: 0x016C */
         uint32_t RESERVED7[4];                       /* Address Offset: 0x0170 */
    __IO uint32_t LICFG0;                             /* Address Offset: 0x0180 */
    __IO uint32_t LICFG1;                             /* Address Offset: 0x0184 */
    __IO uint32_t LICFG2;                             /* Address Offset: 0x0188 */
    __IO uint32_t LICFG3;                             /* Address Offset: 0x018C */
    __IO uint32_t LICFG4;                             /* Address Offset: 0x0190 */
         uint32_t RESERVED8[1];                       /* Address Offset: 0x0194 */
    __IO uint32_t LILMT0;                             /* Address Offset: 0x0198 */
    __IO uint32_t LILMT1;                             /* Address Offset: 0x019C */
    __IO uint32_t LILMT2;                             /* Address Offset: 0x01A0 */
    __IO uint32_t ADCNGLMTCFG;                        /* Address Offset: 0x01A4 */
    __IO uint32_t ADCNGLMTST;                         /* Address Offset: 0x01A8 */
         uint32_t RESERVED9[5];                       /* Address Offset: 0x01AC */
    __IO uint32_t HPLOCFG0;                           /* Address Offset: 0x01C0 */
    __IO uint32_t HPLOCFG1;                           /* Address Offset: 0x01C4 */
    __IO uint32_t HPLOCFG2;                           /* Address Offset: 0x01C8 */
    __IO uint32_t HPLOCFG3;                           /* Address Offset: 0x01CC */
    __IO uint32_t HPLOCFG4;                           /* Address Offset: 0x01D0 */
    __IO uint32_t HPLOCFG5;                           /* Address Offset: 0x01D4 */
         uint32_t RESERVED10[10];                     /* Address Offset: 0x01D8 */
    __IO uint32_t PLLCFG0;                            /* Address Offset: 0x0200 */
    __IO uint32_t PLLCFG1;                            /* Address Offset: 0x0204 */
    __IO uint32_t PLLCFG2;                            /* Address Offset: 0x0208 */
    __IO uint32_t PLLCFG3;                            /* Address Offset: 0x020C */
    __IO uint32_t PLLCFG4;                            /* Address Offset: 0x0210 */
    __IO uint32_t PLLCFG5;                            /* Address Offset: 0x0214 */
         uint32_t RESERVED11[10];                     /* Address Offset: 0x0218 */
    __IO uint32_t I2SCKM;                             /* Address Offset: 0x0240 */
    __IO uint32_t I2SRXCR0;                           /* Address Offset: 0x0244 */
    __IO uint32_t I2SRXCR1;                           /* Address Offset: 0x0248 */
    __IO uint32_t I2SRXCR2;                           /* Address Offset: 0x024C */
    __IO uint32_t I2SRXCMD;                           /* Address Offset: 0x0250 */
         uint32_t RESERVED12[3];                      /* Address Offset: 0x0254 */
    __IO uint32_t I2STXCR0;                           /* Address Offset: 0x0260 */
    __IO uint32_t I2STXCR1;                           /* Address Offset: 0x0264 */
    __IO uint32_t I2STXCR2;                           /* Address Offset: 0x0268 */
    __IO uint32_t I2STXCR3;                           /* Address Offset: 0x026C */
    __IO uint32_t I2STXCMD;                           /* Address Offset: 0x0270 */
         uint32_t RESERVED13[35];                     /* Address Offset: 0x0274 */
    __IO uint32_t TMCFG0;                             /* Address Offset: 0x0300 */

};
#endif /* __ASSEMBLY__ */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */
#define ACODEC                 ((struct ACODEC_REG *) ACODEC_BASE)

#define IS_ACODEC_INSTANCE(instance) ((instance) == ACODEC)
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/*****************************************ACODEC*****************************************/
/* VCTL */
#define ACODEC_ADCBYPS_SHIFT                                      (7U)
#define ACODEC_ADCBYPS_MASK                                       (0x1U << ACODEC_ADCBYPS_SHIFT)
#define ACODEC_DACBYPS_SHIFT                                      (6U)
#define ACODEC_DACBYPS_MASK                                       (0x1U << ACODEC_DACBYPS_SHIFT)
#define ACODEC_ADCFADE_SHIFT                                      (5U)
#define ACODEC_ADCFADE_MASK                                       (0x1U << ACODEC_ADCFADE_SHIFT)
#define ACODEC_DACFADE_SHIFT                                      (4U)
#define ACODEC_DACFADE_MASK                                       (0x1U << ACODEC_DACFADE_SHIFT)
#define ACODEC_ADCCZDT_SHIFT                                      (1U)
#define ACODEC_ADCCZDT_MASK                                       (0x1U << ACODEC_ADCCZDT_SHIFT)
#define ACODEC_DACCZDT_SHIFT                                      (0U)
#define ACODEC_DACCZDT_MASK                                       (0x1U << ACODEC_DACCZDT_SHIFT)
/* LPST */
#define ACODEC_LPDET_SHIFT                                        (0U)
#define ACODEC_LPDET_MASK                                         (0x1U << ACODEC_LPDET_SHIFT)
/* SRST */
#define ACODEC_SOFT_SHIFT                                         (0U)
#define ACODEC_SOFT_MASK                                          (0x1U << ACODEC_SOFT_SHIFT)
/* DIGEN */
#define ACODEC_ADC_EN_SHIFT                                       (5U)
#define ACODEC_ADC_EN_MASK                                        (0x1U << ACODEC_ADC_EN_SHIFT)
#define ACODEC_I2STX_EN_SHIFT                                     (4U)
#define ACODEC_I2STX_EN_MASK                                      (0x1U << ACODEC_I2STX_EN_SHIFT)
#define ACODEC_DAC_EN_SHIFT                                       (1U)
#define ACODEC_DAC_EN_MASK                                        (0x1U << ACODEC_DAC_EN_SHIFT)
#define ACODEC_I2SRX_EN_SHIFT                                     (0U)
#define ACODEC_I2SRX_EN_MASK                                      (0x1U << ACODEC_I2SRX_EN_SHIFT)
/* CLKE */
#define ACODEC_ADC_CKE_SHIFT                                      (5U)
#define ACODEC_ADC_CKE_MASK                                       (0x1U << ACODEC_ADC_CKE_SHIFT)
#define ACODEC_I2STX_CKE_SHIFT                                    (4U)
#define ACODEC_I2STX_CKE_MASK                                     (0x1U << ACODEC_I2STX_CKE_SHIFT)
#define ACODEC_DAC_CKE_SHIFT                                      (1U)
#define ACODEC_DAC_CKE_MASK                                       (0x1U << ACODEC_DAC_CKE_SHIFT)
#define ACODEC_I2SRX_CKE_SHIFT                                    (0U)
#define ACODEC_I2SRX_CKE_MASK                                     (0x1U << ACODEC_I2SRX_CKE_SHIFT)
/* RTCFG0 */
#define ACODEC_HALF_ADC_BUF_SHIFT                                 (7U)
#define ACODEC_HALF_ADC_BUF_MASK                                  (0x1U << ACODEC_HALF_ADC_BUF_SHIFT)
#define ACODEC_HALF_VAG_BUF_SHIFT                                 (6U)
#define ACODEC_HALF_VAG_BUF_MASK                                  (0x1U << ACODEC_HALF_VAG_BUF_SHIFT)
#define ACODEC_IBIAS_ANA_SEL_SHIFT                                (4U)
#define ACODEC_IBIAS_ANA_SEL_MASK                                 (0x3U << ACODEC_IBIAS_ANA_SEL_SHIFT)
#define ACODEC_IBIAS_BIT_SHIFT                                    (0U)
#define ACODEC_IBIAS_BIT_MASK                                     (0xFU << ACODEC_IBIAS_BIT_SHIFT)
/* RTCFG1 */
#define ACODEC_BOOST_ADC_BUF_SHIFT                                (7U)
#define ACODEC_BOOST_ADC_BUF_MASK                                 (0x1U << ACODEC_BOOST_ADC_BUF_SHIFT)
#define ACODEC_BOOST_VAG_BUF_SHIFT                                (6U)
#define ACODEC_BOOST_VAG_BUF_MASK                                 (0x1U << ACODEC_BOOST_VAG_BUF_SHIFT)
#define ACODEC_REF_ADC_SEL_SHIFT                                  (5U)
#define ACODEC_REF_ADC_SEL_MASK                                   (0x1U << ACODEC_REF_ADC_SEL_SHIFT)
#define ACODEC_VAG_SEL_SHIFT                                      (3U)
#define ACODEC_VAG_SEL_MASK                                       (0x3U << ACODEC_VAG_SEL_SHIFT)
#define ACODEC_VBG_TRIM_SHIFT                                     (0U)
#define ACODEC_VBG_TRIM_MASK                                      (0x7U << ACODEC_VBG_TRIM_SHIFT)
/* RTCFG2 */
#define ACODEC_IBIAS_PWD_SHIFT                                    (2U)
#define ACODEC_IBIAS_PWD_MASK                                     (0x1U << ACODEC_IBIAS_PWD_SHIFT)
#define ACODEC_VAG_BUF_PWD_SHIFT                                  (1U)
#define ACODEC_VAG_BUF_PWD_MASK                                   (0x1U << ACODEC_VAG_BUF_PWD_SHIFT)
#define ACODEC_REF_PWD_SHIFT                                      (0U)
#define ACODEC_REF_PWD_MASK                                       (0x1U << ACODEC_REF_PWD_SHIFT)
/* ADCCFG0 */
#define ACODEC_ADC_L_PWD_SHIFT                                    (7U)
#define ACODEC_ADC_L_PWD_MASK                                     (0x1U << ACODEC_ADC_L_PWD_SHIFT)
#define ACODEC_ADC_R_PWD_SHIFT                                    (6U)
#define ACODEC_ADC_R_PWD_MASK                                     (0x1U << ACODEC_ADC_R_PWD_SHIFT)
#define ACODEC_ADC_DEM_EN_SHIFT                                   (4U)
#define ACODEC_ADC_DEM_EN_MASK                                    (0x1U << ACODEC_ADC_DEM_EN_SHIFT)
#define ACODEC_ADC_DITH_OFF_SHIFT                                 (3U)
#define ACODEC_ADC_DITH_OFF_MASK                                  (0x1U << ACODEC_ADC_DITH_OFF_SHIFT)
/* ADCCFG1 */
#define ACODEC_ADC_MUTE_L_SHIFT                                   (7U)
#define ACODEC_ADC_MUTE_L_MASK                                    (0x1U << ACODEC_ADC_MUTE_L_SHIFT)
#define ACODEC_ADC_MUTE_R_SHIFT                                   (6U)
#define ACODEC_ADC_MUTE_R_MASK                                    (0x1U << ACODEC_ADC_MUTE_R_SHIFT)
#define ACODEC_ADC_ATTN_ALLIBIAS_SHIFT                            (5U)
#define ACODEC_ADC_ATTN_ALLIBIAS_MASK                             (0x1U << ACODEC_ADC_ATTN_ALLIBIAS_SHIFT)
#define ACODEC_ADC_ATTN_OPBIAS_SHIFT                              (4U)
#define ACODEC_ADC_ATTN_OPBIAS_MASK                               (0x1U << ACODEC_ADC_ATTN_OPBIAS_SHIFT)
#define ACODEC_ADC_DLY_INC_SHIFT                                  (3U)
#define ACODEC_ADC_DLY_INC_MASK                                   (0x1U << ACODEC_ADC_DLY_INC_SHIFT)
#define ACODEC_ADC_OVERLAP_INC_SHIFT                              (2U)
#define ACODEC_ADC_OVERLAP_INC_MASK                               (0x1U << ACODEC_ADC_OVERLAP_INC_SHIFT)
#define ACODEC_ADC_BOOST_OPAMP_SHIFT                              (1U)
#define ACODEC_ADC_BOOST_OPAMP_MASK                               (0x1U << ACODEC_ADC_BOOST_OPAMP_SHIFT)
#define ACODEC_ADC_BOOST_VAGOP_SHIFT                              (0U)
#define ACODEC_ADC_BOOST_VAGOP_MASK                               (0x1U << ACODEC_ADC_BOOST_VAGOP_SHIFT)
/* ALC0 */
#define ACODEC_ALCL_SHIFT                                         (7U)
#define ACODEC_ALCL_MASK                                          (0x1U << ACODEC_ALCL_SHIFT)
#define ACODEC_ALCR_SHIFT                                         (6U)
#define ACODEC_ALCR_MASK                                          (0x1U << ACODEC_ALCR_SHIFT)
/* ALC2 */
#define ACODEC_ALCMAX_SHIFT                                       (4U)
#define ACODEC_ALCMAX_MASK                                        (0x7U << ACODEC_ALCMAX_SHIFT)
#define ACODEC_ALCMIN_SHIFT                                       (0U)
#define ACODEC_ALCMIN_MASK                                        (0x7U << ACODEC_ALCMIN_SHIFT)
/* ADCNG */
#define ACODEC_NGCHL_EITHER_SHIFT                                 (7U)
#define ACODEC_NGCHL_EITHER_MASK                                  (0x1U << ACODEC_NGCHL_EITHER_SHIFT)
#define ACODEC_NGEN_SHIFT                                         (6U)
#define ACODEC_NGEN_MASK                                          (0x1U << ACODEC_NGEN_SHIFT)
#define ACODEC_NGBOOST_SHIFT                                      (5U)
#define ACODEC_NGBOOST_MASK                                       (0x1U << ACODEC_NGBOOST_SHIFT)
#define ACODEC_NGDLY_SHIFT                                        (0U)
#define ACODEC_NGDLY_MASK                                         (0x3U << ACODEC_NGDLY_SHIFT)
/* ADCVSTL */
#define ACODEC_HPFLE_SHIFT                                        (7U)
#define ACODEC_HPFLE_MASK                                         (0x1U << ACODEC_HPFLE_SHIFT)
#define ACODEC_HPFRE_SHIFT                                        (6U)
#define ACODEC_HPFRE_MASK                                         (0x1U << ACODEC_HPFRE_SHIFT)
#define ACODEC_HPF_CF_SHIFT                                       (4U)
#define ACODEC_HPF_CF_MASK                                        (0x3U << ACODEC_HPF_CF_SHIFT)
/* DACACFG0 */
#define ACODEC_DAC_AMP_INC2DB_SHIFT                               (7U)
#define ACODEC_DAC_AMP_INC2DB_MASK                                (0x1U << ACODEC_DAC_AMP_INC2DB_SHIFT)
#define ACODEC_DAC_MAX_OUT_SHIFT                                  (5U)
#define ACODEC_DAC_MAX_OUT_MASK                                   (0x1U << ACODEC_DAC_MAX_OUT_SHIFT)
#define ACODEC_INC_DAC_RSTB_SHIFT                                 (4U)
#define ACODEC_INC_DAC_RSTB_MASK                                  (0x1U << ACODEC_INC_DAC_RSTB_SHIFT)
/* DACACFG1 */
#define ACODEC_INC_DAC_SWITCH_SHIFT                               (2U)
#define ACODEC_INC_DAC_SWITCH_MASK                                (0x1U << ACODEC_INC_DAC_SWITCH_SHIFT)
#define ACODEC_STOP_DAC_RSTB_SHIFT                                (1U)
#define ACODEC_STOP_DAC_RSTB_MASK                                 (0x1U << ACODEC_STOP_DAC_RSTB_SHIFT)
#define ACODEC_STOP_DAC_SW_SHIFT                                  (0U)
#define ACODEC_STOP_DAC_SW_MASK                                   (0x1U << ACODEC_STOP_DAC_SW_SHIFT)
/* DACACFG2 */
#define ACODEC_PWD_DACBIAS_SHIFT                                  (2U)
#define ACODEC_PWD_DACBIAS_MASK                                   (0x1U << ACODEC_PWD_DACBIAS_SHIFT)
#define ACODEC_PWD_DACL_SHIFT                                     (1U)
#define ACODEC_PWD_DACL_MASK                                      (0x1U << ACODEC_PWD_DACL_SHIFT)
#define ACODEC_PWD_DACR_SHIFT                                     (0U)
#define ACODEC_PWD_DACR_MASK                                      (0x1U << ACODEC_PWD_DACR_SHIFT)
/* DACPOPD */
#define ACODEC_ATPCE_SHIFT                                        (7U)
#define ACODEC_ATPCE_MASK                                         (0x1U << ACODEC_ATPCE_SHIFT)
#define ACODEC_HPSEL_SHIFT                                        (6U)
#define ACODEC_HPSEL_MASK                                         (0x1U << ACODEC_HPSEL_SHIFT)
#define ACODEC_SMTPO_SHIFT                                        (5U)
#define ACODEC_SMTPO_MASK                                         (0x1U << ACODEC_SMTPO_SHIFT)
#define ACODEC_ANTIPOP_SHIFT                                      (4U)
#define ACODEC_ANTIPOP_MASK                                       (0x1U << ACODEC_ANTIPOP_SHIFT)
#define ACODEC_DACATPO_SHIFT                                      (0U)
#define ACODEC_DACATPO_MASK                                       (0x1U << ACODEC_DACATPO_SHIFT)
/* LMT0 */
#define ACODEC_LMTEN_SHIFT                                        (7U)
#define ACODEC_LMTEN_MASK                                         (0x1U << ACODEC_LMTEN_SHIFT)
#define ACODEC_LMTCHL_SHIFT                                       (6U)
#define ACODEC_LMTCHL_MASK                                        (0x1U << ACODEC_LMTCHL_SHIFT)
/* DACMUTE */
#define ACODEC_DACMTE_SHIFT                                       (0U)
#define ACODEC_DACMTE_MASK                                        (0x1U << ACODEC_DACMTE_SHIFT)
/* MIXCTRL */
#define ACODEC_MIXE_SHIFT                                         (0U)
#define ACODEC_MIXE_MASK                                          (0x1U << ACODEC_MIXE_SHIFT)
/* LICFG0 */
#define ACODEC_MICBIAS_SHIFT                                      (4U)
#define ACODEC_MICBIAS_MASK                                       (0x7U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL1V5                                     (0x0U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL1V8                                     (0x1U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL2V0                                     (0x2U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL2V2                                     (0x3U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL2V5                                     (0x4U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL2V8                                     (0x5U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL3V0                                     (0x6U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MICBIAS_SEL3V3                                     (0x7U << ACODEC_MICBIAS_SHIFT)
#define ACODEC_MIC_L_BOOST_SHIFT                                  (2U)
#define ACODEC_MIC_L_BOOST_MASK                                   (0x3U << ACODEC_MIC_L_BOOST_SHIFT)
#define ACODEC_MIC_R_BOOST_SHIFT                                  (0U)
#define ACODEC_MIC_R_BOOST_MASK                                   (0x3U << ACODEC_MIC_R_BOOST_SHIFT)
/* LICFG1 */
#define ACODEC_MIC_L_DIFF_EN_SHIFT                                (5U)
#define ACODEC_MIC_L_DIFF_EN_MASK                                 (0x1U << ACODEC_MIC_L_DIFF_EN_SHIFT)
#define ACODEC_MIC_R_DIFF_EN_SHIFT                                (4U)
#define ACODEC_MIC_R_DIFF_EN_MASK                                 (0x1U << ACODEC_MIC_R_DIFF_EN_SHIFT)
#define ACODEC_MUX_L_IN_SEL_SHIFT                                 (2U)
#define ACODEC_MUX_L_IN_SEL_MASK                                  (0x3U << ACODEC_MUX_L_IN_SEL_SHIFT)
#define ACODEC_MUX_R_IN_SEL_SHIFT                                 (0U)
#define ACODEC_MUX_R_IN_SEL_MASK                                  (0x3U << ACODEC_MUX_R_IN_SEL_SHIFT)
/* LICFG2 */
#define ACODEC_MUX_L_GAIN_SHIFT                                   (4U)
#define ACODEC_MUX_L_GAIN_MASK                                    (0xFU << ACODEC_MUX_L_GAIN_SHIFT)
#define ACODEC_MUX_R_GAIN_SHIFT                                   (0U)
#define ACODEC_MUX_R_GAIN_MASK                                    (0xFU << ACODEC_MUX_R_GAIN_SHIFT)
/* LICFG3 */
#define ACODEC_CHOP_SEL_SHIFT                                     (2U)
#define ACODEC_CHOP_SEL_MASK                                      (0x3U << ACODEC_CHOP_SEL_SHIFT)
#define ACODEC_MIC_CHOP_EN_SHIFT                                  (1U)
#define ACODEC_MIC_CHOP_EN_MASK                                   (0x1U << ACODEC_MIC_CHOP_EN_SHIFT)
#define ACODEC_MUX_CHOP_EN_SHIFT                                  (0U)
#define ACODEC_MUX_CHOP_EN_MASK                                   (0x1U << ACODEC_MUX_CHOP_EN_SHIFT)
/* LICFG4 */
#define ACODEC_MIC_L_PD_SHIFT                                     (5U)
#define ACODEC_MIC_L_PD_MASK                                      (0x1U << ACODEC_MIC_L_PD_SHIFT)
#define ACODEC_MIC_R_PD_SHIFT                                     (4U)
#define ACODEC_MIC_R_PD_MASK                                      (0x1U << ACODEC_MIC_R_PD_SHIFT)
#define ACODEC_MUX_L_PD_SHIFT                                     (1U)
#define ACODEC_MUX_L_PD_MASK                                      (0x1U << ACODEC_MUX_L_PD_SHIFT)
#define ACODEC_MUX_R_PD_SHIFT                                     (0U)
#define ACODEC_MUX_R_PD_MASK                                      (0x1U << ACODEC_MUX_R_PD_SHIFT)
/* LILMT0 */
#define ACODEC_LMT_EN_SHIFT                                       (0U)
#define ACODEC_LMT_EN_MASK                                        (0x1U << ACODEC_LMT_EN_SHIFT)
/* ADCNGLMTCFG */
#define ACODEC_NGCHL_LI_SHIFT                                     (7U)
#define ACODEC_NGCHL_LI_MASK                                      (0x1U << ACODEC_NGCHL_LI_SHIFT)
#define ACODEC_NGEN_LI_SHIFT                                      (6U)
#define ACODEC_NGEN_LI_MASK                                       (0x1U << ACODEC_NGEN_LI_SHIFT)
#define ACODEC_NGBOOST_LI_SHIFT                                   (5U)
#define ACODEC_NGBOOST_LI_MASK                                    (0x1U << ACODEC_NGBOOST_LI_SHIFT)
#define ACODEC_NGDLY_LI_SHIFT                                     (0U)
#define ACODEC_NGDLY_LI_MASK                                      (0x3U << ACODEC_NGDLY_LI_SHIFT)
/* ADCNGLMTST */
#define ACODEC_NGACT_LI_SHIFT                                     (0U)
#define ACODEC_NGACT_LI_MASK                                      (0x1U << ACODEC_NGACT_LI_SHIFT)
/* HPLOCFG0 */
#define ACODEC_INC_LO_AMP_SHIFT                                   (2U)
#define ACODEC_INC_LO_AMP_MASK                                    (0x3U << ACODEC_INC_LO_AMP_SHIFT)
#define ACODEC_LO_VAG_RISE_SLOW_SHIFT                             (1U)
#define ACODEC_LO_VAG_RISE_SLOW_MASK                              (0x1U << ACODEC_LO_VAG_RISE_SLOW_SHIFT)
#define ACODEC_LO_OUT_VAG_SHIFT                                   (0U)
#define ACODEC_LO_OUT_VAG_MASK                                    (0x1U << ACODEC_LO_OUT_VAG_SHIFT)
/* HPLOCFG1 */
#define ACODEC_INC_HP_AMP_SHIFT                                   (3U)
#define ACODEC_INC_HP_AMP_MASK                                    (0x3U << ACODEC_INC_HP_AMP_SHIFT)
#define ACODEC_HP_TWO_STAGE_SHIFT                                 (2U)
#define ACODEC_HP_TWO_STAGE_MASK                                  (0x1U << ACODEC_HP_TWO_STAGE_SHIFT)
#define ACODEC_INC_OC_RANGE_SHIFT                                 (1U)
#define ACODEC_INC_OC_RANGE_MASK                                  (0x1U << ACODEC_INC_OC_RANGE_SHIFT)
#define ACODEC_HP_OPAMP_HALF_BIAS_SHIFT                           (0U)
#define ACODEC_HP_OPAMP_HALF_BIAS_MASK                            (0x1U << ACODEC_HP_OPAMP_HALF_BIAS_SHIFT)
/* HPLOCFG2 */
#define ACODEC_HP_LN_OC_SHIFT                                     (5U)
#define ACODEC_HP_LN_OC_MASK                                      (0x1U << ACODEC_HP_LN_OC_SHIFT)
#define ACODEC_HP_LP_OC_SHIFT                                     (4U)
#define ACODEC_HP_LP_OC_MASK                                      (0x1U << ACODEC_HP_LP_OC_SHIFT)
#define ACODEC_HP_RN_OC_SHIFT                                     (3U)
#define ACODEC_HP_RN_OC_MASK                                      (0x1U << ACODEC_HP_RN_OC_SHIFT)
#define ACODEC_HP_RP_OC_SHIFT                                     (2U)
#define ACODEC_HP_RP_OC_MASK                                      (0x1U << ACODEC_HP_RP_OC_SHIFT)
#define ACODEC_HP_VGND_N_OC_SHIFT                                 (1U)
#define ACODEC_HP_VGND_N_OC_MASK                                  (0x1U << ACODEC_HP_VGND_N_OC_SHIFT)
#define ACODEC_HP_VGND_P_OC_SHIFT                                 (0U)
#define ACODEC_HP_VGND_P_OC_MASK                                  (0x1U << ACODEC_HP_VGND_P_OC_SHIFT)
/* HPLOCFG3 */
#define ACODEC_PWD_HP_OSTG_SHIFT                                  (3U)
#define ACODEC_PWD_HP_OSTG_MASK                                   (0x1U << ACODEC_PWD_HP_OSTG_SHIFT)
#define ACODEC_PWD_HP_VGND_SHIFT                                  (2U)
#define ACODEC_PWD_HP_VGND_MASK                                   (0x1U << ACODEC_PWD_HP_VGND_SHIFT)
#define ACODEC_PWD_HP_BUF_SHIFT                                   (1U)
#define ACODEC_PWD_HP_BUF_MASK                                    (0x1U << ACODEC_PWD_HP_BUF_SHIFT)
#define ACODEC_HP_SHORT_OUT_SHIFT                                 (0U)
#define ACODEC_HP_SHORT_OUT_MASK                                  (0x1U << ACODEC_HP_SHORT_OUT_SHIFT)
/* HPLOCFG4 */
#define ACODEC_HP_ANTIPOP_SHIFT                                   (4U)
#define ACODEC_HP_ANTIPOP_MASK                                    (0x1U << ACODEC_HP_ANTIPOP_SHIFT)
#define ACODEC_HP_ANTIPOP_BIT_SHIFT                               (0U)
#define ACODEC_HP_ANTIPOP_BIT_MASK                                (0xFU << ACODEC_HP_ANTIPOP_BIT_SHIFT)

/* HPLOCFG5 */
#define ACODEC_PWD_LO_OSTG_SHIFT                                  (6U)
#define ACODEC_PWD_LO_OSTG_MASK                                   (0x1U << ACODEC_PWD_LO_OSTG_SHIFT)
#define ACODEC_PWD_LO_BUF_SHIFT                                   (5U)
#define ACODEC_PWD_LO_BUF_MASK                                    (0x1U << ACODEC_PWD_LO_BUF_SHIFT)
#define ACODEC_LO_ANTIPOP_SHIFT                                   (4U)
#define ACODEC_LO_ANTIPOP_MASK                                    (0x1U << ACODEC_LO_ANTIPOP_SHIFT)
#define ACODEC_LO_ANTIPOP_BIT_SHIFT                               (0U)
#define ACODEC_LO_ANTIPOP_BIT_MASK                                (0xFU << ACODEC_LO_ANTIPOP_BIT_SHIFT)
/* PLLCFG0 */
#define ACODEC_PLL_CLKIN_SEL_SHIFT                                (3U)
#define ACODEC_PLL_CLKIN_SEL_MASK                                 (0x3U << ACODEC_PLL_CLKIN_SEL_SHIFT)
#define ACODEC_PLL_OUTDIV_EN_SHIFT                                (2U)
#define ACODEC_PLL_OUTDIV_EN_MASK                                 (0x1U << ACODEC_PLL_OUTDIV_EN_SHIFT)
/* PLLCFG4 */
#define ACODEC_PLL_OUTDIV_SHIFT                                   (4U)
#define ACODEC_PLL_OUTDIV_MASK                                    (0xFU << ACODEC_PLL_OUTDIV_SHIFT)
#define ACODEC_PLL_CLKDIV_SHIFT                                   (0U)
#define ACODEC_PLL_CLKDIV_MASK                                    (0xFU << ACODEC_PLL_CLKDIV_SHIFT)
/* PLLCFG5 */
#define ACODEC_PLL_RESET_SHIFT                                    (2U)
#define ACODEC_PLL_RESET_MASK                                     (0x1U << ACODEC_PLL_RESET_SHIFT)
#define ACODEC_PLL_TEST_SHIFT                                     (1U)
#define ACODEC_PLL_TEST_MASK                                      (0x1U << ACODEC_PLL_TEST_SHIFT)
#define ACODEC_PLL_PW_SHIFT                                       (0U)
#define ACODEC_PLL_PW_MASK                                        (0x1U << ACODEC_PLL_PW_SHIFT)
/* I2SCKM */
#define ACODEC_I2S_MCK_DIV_SHIFT                                  (4U)
#define ACODEC_I2S_MCK_DIV_MASK                                   (0xFU << ACODEC_I2S_MCK_DIV_SHIFT)
#define ACODEC_SCK_EN_SHIFT                                       (2U)
#define ACODEC_SCK_EN_MASK                                        (0x1U << ACODEC_SCK_EN_SHIFT)
#define ACODEC_SCK_P_SHIFT                                        (1U)
#define ACODEC_SCK_P_MASK                                         (0x1U << ACODEC_SCK_P_SHIFT)
#define ACODEC_I2S_MST_SHIFT                                      (0U)
#define ACODEC_I2S_MST_MASK                                       (0x1U << ACODEC_I2S_MST_SHIFT)
/* I2SRXCR0 */
#define ACODEC_SCKD_RX_DIV_SHIFT                                  (1U)
#define ACODEC_SCKD_RX_DIV_MASK                                   (0x3U << ACODEC_SCKD_RX_DIV_SHIFT)
#define ACODEC_RXRL_P_SHIFT                                       (0U)
#define ACODEC_RXRL_P_MASK                                        (0x1U << ACODEC_RXRL_P_SHIFT)
/* I2SRXCR1 */
#define ACODEC_TFS_RX_SHIFT                                       (6U)
#define ACODEC_TFS_RX_MASK                                        (0x1U << ACODEC_TFS_RX_SHIFT)
#define ACODEC_PBM_RX_BUS_MODE_SHIFT                              (4U)
#define ACODEC_PBM_RX_BUS_MODE_MASK                               (0x3U << ACODEC_PBM_RX_BUS_MODE_SHIFT)
#define ACODEC_IBM_RX_BUS_MODE_SHIFT                              (2U)
#define ACODEC_IBM_RX_BUS_MODE_MASK                               (0x3U << ACODEC_IBM_RX_BUS_MODE_SHIFT)
#define ACODEC_EXRL_RX_SHIFT                                      (1U)
#define ACODEC_EXRL_RX_MASK                                       (0x1U << ACODEC_EXRL_RX_SHIFT)
#define ACODEC_LSB_RX_SHIFT                                       (0U)
#define ACODEC_LSB_RX_MASK                                        (0x1U << ACODEC_LSB_RX_SHIFT)
/* I2SRXCR2 */
#define ACODEC_VDW_RX_WIDTH_SHIFT                                 (0U)
#define ACODEC_VDW_RX_WIDTH_MASK                                  (0x1FU << ACODEC_VDW_RX_WIDTH_SHIFT)
/* I2SRXCMD */
#define ACODEC_RXS_SHIFT                                          (1U)
#define ACODEC_RXS_MASK                                           (0x1U << ACODEC_RXS_SHIFT)
#define ACODEC_RXC_SHIFT                                          (0U)
#define ACODEC_RXC_MASK                                           (0x1U << ACODEC_RXC_SHIFT)
/* I2STXCR0 */
#define ACODEC_SCKD_TX_DIV_SHIFT                                  (1U)
#define ACODEC_SCKD_TX_DIV_MASK                                   (0x3U << ACODEC_SCKD_TX_DIV_SHIFT)
#define ACODEC_TXRL_P_SHIFT                                       (0U)
#define ACODEC_TXRL_P_MASK                                        (0x1U << ACODEC_TXRL_P_SHIFT)
/* I2STXCR1 */
#define ACODEC_TFS_TX_MODE_SHIFT                                  (6U)
#define ACODEC_TFS_TX_MODE_MASK                                   (0x1U << ACODEC_TFS_TX_MODE_SHIFT)
#define ACODEC_PBM_TX_BUS_MODE_SHIFT                              (4U)
#define ACODEC_PBM_TX_BUS_MODE_MASK                               (0x3U << ACODEC_PBM_TX_BUS_MODE_SHIFT)
#define ACODEC_IBM_TX_BUS_MODE_SHIFT                              (2U)
#define ACODEC_IBM_TX_BUS_MODE_MASK                               (0x3U << ACODEC_IBM_TX_BUS_MODE_SHIFT)
#define ACODEC_EXRL_TX_SHIFT                                      (1U)
#define ACODEC_EXRL_TX_MASK                                       (0x1U << ACODEC_EXRL_TX_SHIFT)
#define ACODEC_LSB_TX_SHIFT                                       (0U)
#define ACODEC_LSB_TX_MASK                                        (0x1U << ACODEC_LSB_TX_SHIFT)
/* I2STXCR2 */
#define ACODEC_VDW_TX_WIDTH_SHIFT                                 (0U)
#define ACODEC_VDW_TX_WIDTH_MASK                                  (0x1FU << ACODEC_VDW_TX_WIDTH_SHIFT)
/* I2STXCMD */
#define ACODEC_TXS_SHIFT                                          (1U)
#define ACODEC_TXS_MASK                                           (0x1U << ACODEC_TXS_SHIFT)
#define ACODEC_TXC_SHIFT                                          (0U)
#define ACODEC_TXC_MASK                                           (0x1U << ACODEC_TXC_SHIFT)
/* TMCFG0 */
#define ACODEC_ATE_TEST_MODE_SHIFT                                (1U)
#define ACODEC_ATE_TEST_MODE_MASK                                 (0x1U << ACODEC_ATE_TEST_MODE_SHIFT)
#define ACODEC_BURN_IN_MODE_SHIFT                                 (0U)
#define ACODEC_BURN_IN_MODE_MASK                                  (0x1U << ACODEC_BURN_IN_MODE_SHIFT)

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK2106_ACODEC_H */

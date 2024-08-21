/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2018-2020 Rockchip Electronics Co., Ltd.
 */

#ifndef __RV1108_H
#define __RV1108_H
#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* I2S Register Structure Define */
struct I2S_REG {
    __IO uint32_t TXCR;                               /* Address Offset: 0x0000 */
    __IO uint32_t RXCR;                               /* Address Offset: 0x0004 */
    __IO uint32_t CKR;                                /* Address Offset: 0x0008 */
    __I  uint32_t TXFIFOLR;                           /* Address Offset: 0x000C */
    __IO uint32_t DMACR;                              /* Address Offset: 0x0010 */
    __IO uint32_t INTCR;                              /* Address Offset: 0x0014 */
    __I  uint32_t INTSR;                              /* Address Offset: 0x0018 */
    __IO uint32_t XFER;                               /* Address Offset: 0x001C */
    __IO uint32_t CLR;                                /* Address Offset: 0x0020 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0024 */
    __I  uint32_t RXDR;                               /* Address Offset: 0x0028 */
    __I  uint32_t RXFIFOLR;                           /* Address Offset: 0x002C */
};
/* CRYPTO Register Structure Define */
struct CRYPTO_REG {
    __IO uint32_t INTSTS;                             /* Address Offset: 0x0000 */
    __IO uint32_t INTENA;                             /* Address Offset: 0x0004 */
    __IO uint32_t CTRL;                               /* Address Offset: 0x0008 */
    __IO uint32_t CONF;                               /* Address Offset: 0x000C */
    __IO uint32_t BRDMAS;                             /* Address Offset: 0x0010 */
    __IO uint32_t BTDMAS;                             /* Address Offset: 0x0014 */
    __IO uint32_t BRDMAL;                             /* Address Offset: 0x0018 */
    __IO uint32_t HRDMAS;                             /* Address Offset: 0x001C */
    __IO uint32_t HRDMAL;                             /* Address Offset: 0x0020 */
         uint32_t RESERVED0[23];                      /* Address Offset: 0x0024 */
    __IO uint32_t AES_CTRL;                           /* Address Offset: 0x0080 */
    __IO uint32_t AES_STS;                            /* Address Offset: 0x0084 */
    __IO uint32_t AES_DIN[4];                         /* Address Offset: 0x0088 */
    __IO uint32_t AES_DOUT[4];                        /* Address Offset: 0x0098 */
    __IO uint32_t AES_IV[4];                          /* Address Offset: 0x00A8 */
    __IO uint32_t AES_KEY[8];                         /* Address Offset: 0x00B8 */
    __IO uint32_t AES_CNT[4];                         /* Address Offset: 0x00D8 */
         uint32_t RESERVED1[6];                       /* Address Offset: 0x00E8 */
    __IO uint32_t TDES_CTRL;                          /* Address Offset: 0x0100 */
    __IO uint32_t TDES_STS;                           /* Address Offset: 0x0104 */
    __IO uint32_t TDES_DIN[2];                        /* Address Offset: 0x0108 */
    __IO uint32_t TDES_DOUT[2];                       /* Address Offset: 0x0110 */
    __IO uint32_t TDES_IV[2];                         /* Address Offset: 0x0118 */
    __IO uint32_t TDES_KEY1[2];                       /* Address Offset: 0x0120 */
    __IO uint32_t TDES_KEY2[2];                       /* Address Offset: 0x0128 */
    __IO uint32_t TDES_KEY3[2];                       /* Address Offset: 0x0130 */
         uint32_t RESERVED2[18];                      /* Address Offset: 0x0138 */
    __IO uint32_t HASH_CTRL;                          /* Address Offset: 0x0180 */
    __IO uint32_t HASH_STS;                           /* Address Offset: 0x0184 */
    __IO uint32_t HASH_MSG_LEN;                       /* Address Offset: 0x0188 */
    __IO uint32_t HASH_DOUT[8];                       /* Address Offset: 0x018C */
    __IO uint32_t HASH_SEED[5];                       /* Address Offset: 0x01AC */
         uint32_t RESERVED3[16];                      /* Address Offset: 0x01C0 */
    __IO uint32_t TRNG_CTRL;                          /* Address Offset: 0x0200 */
    __IO uint32_t TRNG_DOUT[8];                       /* Address Offset: 0x0204 */
         uint32_t RESERVED4[23];                      /* Address Offset: 0x0224 */
    __IO uint32_t PKA_CTRL;                           /* Address Offset: 0x0280 */
         uint32_t RESERVED5[91];                      /* Address Offset: 0x0284 */
    __IO uint32_t CRYPTO_VER;                         /* Address Offset: 0x03F0 */
         uint32_t RESERVED6[3];                       /* Address Offset: 0x03F4 */
    __IO uint32_t PKA_M;                              /* Address Offset: 0x0400 */
         uint32_t RESERVED7[63];                      /* Address Offset: 0x0404 */
    __IO uint32_t PKA_C;                              /* Address Offset: 0x0500 */
         uint32_t RESERVED8[63];                      /* Address Offset: 0x0504 */
    __IO uint32_t PKA_N;                              /* Address Offset: 0x0600 */
         uint32_t RESERVED9[63];                      /* Address Offset: 0x0604 */
    __IO uint32_t PKA_E;                              /* Address Offset: 0x0700 */
};
/* SPI Register Structure Define */
struct SPI_REG {
    __IO uint32_t CTRLR[2];                           /* Address Offset: 0x0000 */
    __IO uint32_t ENR;                                /* Address Offset: 0x0008 */
    __IO uint32_t SER;                                /* Address Offset: 0x000C */
    __IO uint32_t BAUDR;                              /* Address Offset: 0x0010 */
    __IO uint32_t TXFTLR;                             /* Address Offset: 0x0014 */
    __IO uint32_t RXFTLR;                             /* Address Offset: 0x0018 */
    __I  uint32_t TXFLR;                              /* Address Offset: 0x001C */
    __I  uint32_t RXFLR;                              /* Address Offset: 0x0020 */
    __I  uint32_t SR;                                 /* Address Offset: 0x0024 */
    __IO uint32_t IPR;                                /* Address Offset: 0x0028 */
    __IO uint32_t IMR;                                /* Address Offset: 0x002C */
    __I  uint32_t ISR;                                /* Address Offset: 0x0030 */
    __I  uint32_t RISR;                               /* Address Offset: 0x0034 */
    __O  uint32_t ICR;                                /* Address Offset: 0x0038 */
    __IO uint32_t DMACR;                              /* Address Offset: 0x003C */
    __IO uint32_t DMATDLR;                            /* Address Offset: 0x0040 */
    __IO uint32_t DMARDLR;                            /* Address Offset: 0x0044 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0048 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x004C */
};
/* GPIO Register Structure Define */
struct GPIO_REG {
    __IO uint32_t SWPORTA_DR;                         /* Address Offset: 0x0000 */
    __IO uint32_t SWPORTA_DDR;                        /* Address Offset: 0x0004 */
         uint32_t RESERVED1[(0x30 - 0x04) / 4 - 1];
    __IO uint32_t INTEN;                              /* Address Offset: 0x0030 */
    __IO uint32_t INTMASK;                            /* Address Offset: 0x0034 */
    __IO uint32_t INTTYPE_LEVEL;                      /* Address Offset: 0x0038 */
    __IO uint32_t INT_POLARITY;                       /* Address Offset: 0x003c */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0040 */
    __I  uint32_t INT_RAWSTATUS;                      /* Address Offset: 0x0044 */
    __IO uint32_t DEBOUNCE;                           /* Address Offset: 0x0048 */
    __O  uint32_t PORTA_EOI;                          /* Address Offset: 0x004c */
    __I  uint32_t EXT_PORTA;                          /* Address Offset: 0x0050 */
         uint32_t RESERVED2[(0x60 - 0x50) / 4 - 1];
    __IO uint32_t LS_SYNC;                            /* Address Offset: 0x0060 */
};
/* PWM Register Structure Define */
struct PWM_REG {
    __I  uint32_t PWM0_CNT;                           /* Address Offset: 0x0000 */
    __IO uint32_t PWM0_PERIOD_HPR;                    /* Address Offset: 0x0004 */
    __IO uint32_t PWM0_DUTY_LPR;                      /* Address Offset: 0x0008 */
    __IO uint32_t PWM0_CTRL;                          /* Address Offset: 0x000C */
    __I  uint32_t PWM1_CNT;                           /* Address Offset: 0x0010 */
    __IO uint32_t PWM1_PERIOD_HPR;                    /* Address Offset: 0x0014 */
    __IO uint32_t PWM1_DUTY_LPR;                      /* Address Offset: 0x0018 */
    __IO uint32_t PWM1_CTRL;                          /* Address Offset: 0x001C */
    __I  uint32_t PWM2_CNT;                           /* Address Offset: 0x0020 */
    __IO uint32_t PWM2_PERIOD_HPR;                    /* Address Offset: 0x0024 */
    __IO uint32_t PWM2_DUTY_LPR;                      /* Address Offset: 0x0028 */
    __IO uint32_t PWM2_CTRL;                          /* Address Offset: 0x002C */
    __I  uint32_t PWM3_CNT;                           /* Address Offset: 0x0030 */
    __IO uint32_t PWM3_PERIOD_HPR;                    /* Address Offset: 0x0034 */
    __IO uint32_t PWM3_DUTY_LPR;                      /* Address Offset: 0x0038 */
    __IO uint32_t PWM3_CTRL;                          /* Address Offset: 0x003C */
    __IO uint32_t INTSTS;                             /* Address Offset: 0x0040 */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x0044 */
         uint32_t RESERVED0[2];                       /* Address Offset: 0x0048 */
    __IO uint32_t PWM_FIFO_CTRL;                      /* Address Offset: 0x0050 */
    __IO uint32_t PWM_FIFO_INTSTS;                    /* Address Offset: 0x0054 */
    __I  uint32_t PWM_FIFO_TOUTTHR;                   /* Address Offset: 0x0058 */
         uint32_t RESERVED1;                          /* Address Offset: 0x005C */
    __IO uint32_t PWM_FIFO;                           /* Address Offset: 0x0060 */
};
/* GRF Register Structure Define */
struct GRF_REG {
         uint32_t RESERVED0000[4];                    /* Address Offset: 0x0000 */
    __IO uint32_t GPIO1A_IOMUX;                       /* Address Offset: 0x0010 */
    __IO uint32_t GPIO1B_IOMUX;                       /* Address Offset: 0x0014 */
    __IO uint32_t GPIO1C_IOMUX;                       /* Address Offset: 0x0018 */
    __IO uint32_t GPIO1D_IOMUX;                       /* Address Offset: 0x001C */
    __IO uint32_t GPIO2A_IOMUX;                       /* Address Offset: 0x0020 */
    __IO uint32_t GPIO2B_IOMUX;                       /* Address Offset: 0x0024 */
    __IO uint32_t GPIO2C_IOMUX;                       /* Address Offset: 0x0028 */
    __IO uint32_t GPIO2D_IOMUX;                       /* Address Offset: 0x002C */
    __IO uint32_t GPIO3A_IOMUX;                       /* Address Offset: 0x0030 */
    __IO uint32_t GPIO3B_IOMUX;                       /* Address Offset: 0x0034 */
    __IO uint32_t GPIO3C_IOMUX;                       /* Address Offset: 0x0038 */
    __IO uint32_t GPIO3D_IOMUX;                       /* Address Offset: 0x003C */
         uint32_t RESERVED0040[52];                   /* Address Offset: 0x0040 */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0110 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0114 */
    __IO uint32_t GPIO1C_P;                           /* Address Offset: 0x0118 */
    __IO uint32_t GPIO1D_P;                           /* Address Offset: 0x011C */
    __IO uint32_t GPIO2A_P;                           /* Address Offset: 0x0120 */
    __IO uint32_t GPIO2B_P;                           /* Address Offset: 0x0124 */
    __IO uint32_t GPIO2C_P;                           /* Address Offset: 0x0128 */
    __IO uint32_t GPIO2D_P;                           /* Address Offset: 0x012C */
    __IO uint32_t GPIO3A_P;                           /* Address Offset: 0x0130 */
    __IO uint32_t GPIO3B_P;                           /* Address Offset: 0x0134 */
    __IO uint32_t GPIO3C_P;                           /* Address Offset: 0x0138 */
    __IO uint32_t GPIO3D_P;                           /* Address Offset: 0x013C */
         uint32_t RESERVED0140[52];                   /* Address Offset: 0x0140 */
    __IO uint32_t GPIO1A_E;                           /* Address Offset: 0x0210 */
    __IO uint32_t GPIO1B_E;                           /* Address Offset: 0x0214 */
    __IO uint32_t GPIO1C_E;                           /* Address Offset: 0x0218 */
    __IO uint32_t GPIO1D_E;                           /* Address Offset: 0x021C */
    __IO uint32_t GPIO2A_E;                           /* Address Offset: 0x0220 */
    __IO uint32_t GPIO2B_E;                           /* Address Offset: 0x0224 */
    __IO uint32_t GPIO2C_E;                           /* Address Offset: 0x0228 */
    __IO uint32_t GPIO2D_E;                           /* Address Offset: 0x022C */
    __IO uint32_t GPIO3A_E;                           /* Address Offset: 0x0230 */
    __IO uint32_t GPIO3B_E;                           /* Address Offset: 0x0234 */
    __IO uint32_t GPIO3C_E;                           /* Address Offset: 0x0238 */
    __IO uint32_t GPIO3D_E;                           /* Address Offset: 0x023C */
         uint32_t RESERVED0240[50];                   /* Address Offset: 0x0240 */
    __IO uint32_t GPIO1L_SR;                          /* Address Offset: 0x0308 */
    __IO uint32_t GPIO1H_SR;                          /* Address Offset: 0x030C */
    __IO uint32_t GPIO2L_SR;                          /* Address Offset: 0x0310 */
    __IO uint32_t GPIO2H_SR;                          /* Address Offset: 0x0314 */
    __IO uint32_t GPIO3L_SR;                          /* Address Offset: 0x0318 */
    __IO uint32_t GPIO3H_SR;                          /* Address Offset: 0x031C */
         uint32_t RESERVED0320[26];                   /* Address Offset: 0x0320 */
    __IO uint32_t GPIO1L_SMT;                         /* Address Offset: 0x0388 */
    __IO uint32_t GPIO1H_SMT;                         /* Address Offset: 0x038C */
    __IO uint32_t GPIO2L_SMT;                         /* Address Offset: 0x0390 */
    __IO uint32_t GPIO2H_SMT;                         /* Address Offset: 0x0394 */
    __IO uint32_t GPIO3L_SMT;                         /* Address Offset: 0x0398 */
    __IO uint32_t GPIO3H_SMT;                         /* Address Offset: 0x039C */
         uint32_t RESERVED03A0[24];                   /* Address Offset: 0x03A0 */
    __IO uint32_t SOC_CON0;                           /* Address Offset: 0x0400 */
    __IO uint32_t SOC_CON1;                           /* Address Offset: 0x0404 */
    __IO uint32_t SOC_CON2;                           /* Address Offset: 0x0408 */
    __IO uint32_t SOC_CON3;                           /* Address Offset: 0x040C */
    __IO uint32_t SOC_CON4;                           /* Address Offset: 0x0410 */
    __IO uint32_t SOC_CON5;                           /* Address Offset: 0x0414 */
    __IO uint32_t SOC_CON6;                           /* Address Offset: 0x0418 */
    __IO uint32_t SOC_CON7;                           /* Address Offset: 0x041C */
    __IO uint32_t SOC_CON8;                           /* Address Offset: 0x0420 */
    __IO uint32_t SOC_CON9;                           /* Address Offset: 0x0424 */
    __IO uint32_t SOC_CON10;                          /* Address Offset: 0x0428 */
    __IO uint32_t SOC_CON11;                          /* Address Offset: 0x042C */
         uint32_t RESERVED0430[20];                   /* Address Offset: 0x0430 */
    __IO uint32_t SOC_STATUS0;                        /* Address Offset: 0x0480 */
    __IO uint32_t SOC_STATUS1;                        /* Address Offset: 0x0484 */
         uint32_t RESERVED0488[30];                   /* Address Offset: 0x0488 */
    __IO uint32_t CPU_CON0;                           /* Address Offset: 0x0500 */
    __IO uint32_t CPU_CON1;                           /* Address Offset: 0x0504 */
         uint32_t RESERVED0508[30];                   /* Address Offset: 0x0508 */
    __IO uint32_t OS_REG0;                            /* Address Offset: 0x0580 */
    __IO uint32_t OS_REG1;                            /* Address Offset: 0x0584 */
    __IO uint32_t OS_REG2;                            /* Address Offset: 0x0588 */
    __IO uint32_t OS_REG3;                            /* Address Offset: 0x058C */
         uint32_t RESERVED0590[29];                   /* Address Offset: 0x0590 */
    __IO uint32_t DDR_STATUS;                         /* Address Offset: 0x0604 */
         uint32_t RESERVED0608[30];                   /* Address Offset: 0x0608 */
    __IO uint32_t SIG_DETECT_CON;                     /* Address Offset: 0x0680 */
         uint32_t RESERVED0684[3];                    /* Address Offset: 0x0684 */
    __IO uint32_t SIG_DETECT_STATUS;                  /* Address Offset: 0x0690 */
         uint32_t RESERVED0694[3];                    /* Address Offset: 0x0694 */
    __IO uint32_t SIG_DETECT_CLR;                     /* Address Offset: 0x06A0 */
         uint32_t RESERVED06A4[23];                   /* Address Offset: 0x06A4 */
    __IO uint32_t HOST0_CON0;                         /* Address Offset: 0x0700 */
    __IO uint32_t HOST0_CON1;                         /* Address Offset: 0x0704 */
         uint32_t RESERVED0708[2];                    /* Address Offset: 0x0708 */
    __IO uint32_t DMA_CON0;                           /* Address Offset: 0x0710 */
    __IO uint32_t DMA_CON1;                           /* Address Offset: 0x0714 */
         uint32_t RESERVED0718[539];                  /* Address Offset: 0x0718 */
    __I  uint32_t UOC_STATUS;                         /* Address Offset: 0x0F84 */
    __IO uint32_t HOST_STATUS;                        /* Address Offset: 0x0F88 */
    __IO uint32_t GMAC_CON0;                          /* Address Offset: 0x0F8C */
    __IO uint32_t CHIP_ID;                            /* Address Offset: 0x0F90 */
};
/* PMU Register Structure Define */
struct PMU_REG {
    __IO uint32_t PMU_WAKEUP_CFG[3];                  /* Address Offset: 0x0000 */
         uint32_t RESERVED0[2];                       /* Address Offset: 0x000C */
    __IO uint32_t PMU_PWRMODE_CORE_CON;               /* Address Offset: 0x0014 */
    __IO uint32_t PMU_PWRMODE_COMMON_CON;             /* Address Offset: 0x0018 */
    __IO uint32_t PMU_SFT_CON;                        /* Address Offset: 0x001C */
         uint32_t RESERVED1[7];                       /* Address Offset: 0x0020 */
    __IO uint32_t PMU_BUS_IDLE_REQ;                   /* Address Offset: 0x003C */
    __IO uint32_t PMU_BUS_IDLE_ST;                    /* Address Offset: 0x0040 */
         uint32_t RESERVED2;                          /* Address Offset: 0x0044 */
    __IO uint32_t PMU_OSC_CNT;                        /* Address Offset: 0x0048 */
    __IO uint32_t PMU_PLLLOCK_CNT;                    /* Address Offset: 0x004C */
         uint32_t RESERVED3;                          /* Address Offset: 0x0050 */
    __IO uint32_t PMU_STABLE_CNT;                     /* Address Offset: 0x0054 */
         uint32_t RESERVED4;                          /* Address Offset: 0x0058 */
    __IO uint32_t PMU_WAKEUP_RST_CLR_CNT;             /* Address Offset: 0x005C */
    __I  uint32_t PMU_DDR_SREF_ST;                    /* Address Offset: 0x0060 */
    __IO uint32_t PMU_SYS_REG[4];                     /* Address Offset: 0x0064 */
    __IO uint32_t PMU_TIMEOUT_CNT;                    /* Address Offset: 0x0074 */
};
/* PMU_GRF Register Structure Define */
struct PMU_GRF_REG {
    __IO uint32_t PMUGRF_GPIO0A_IOMUX;                /* Address Offset: 0x0000 */
    __IO uint32_t PMUGRF_GPIO0B_IOMUX;                /* Address Offset: 0x0004 */
    __IO uint32_t PMUGRF_GPIO0C_IOMUX;                /* Address Offset: 0x0008 */
         uint32_t RESERVED0;                          /* Address Offset: 0x000C */
    __IO uint32_t PMUGRF_GPIO0A_P;                    /* Address Offset: 0x0010 */
    __IO uint32_t PMUGRF_GPIO0B_P;                    /* Address Offset: 0x0014 */
    __IO uint32_t PMUGRF_GPIO0C_P;                    /* Address Offset: 0x0018 */
         uint32_t RESERVED1;                          /* Address Offset: 0x001C */
    __IO uint32_t PMUGRF_GPIO0A_E;                    /* Address Offset: 0x0020 */
    __IO uint32_t PMUGRF_GPIO0B_E;                    /* Address Offset: 0x0024 */
    __IO uint32_t PMUGRF_GPIO0C_E;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED2;                          /* Address Offset: 0x002C */
    __IO uint32_t PMUGRF_GPIO0A_SMT;                  /* Address Offset: 0x0030 */
    __IO uint32_t PMUGRF_GPIO0B_SMT;                  /* Address Offset: 0x0034 */
    __IO uint32_t PMUGRF_GPIO0C_SMT;                  /* Address Offset: 0x0038 */
         uint32_t RESERVED3;                          /* Address Offset: 0x003C */
    __IO uint32_t PMUGRF_GPIO0A_SR;                   /* Address Offset: 0x0040 */
    __IO uint32_t PMUGRF_GPIO0B_SR;                   /* Address Offset: 0x0044 */
    __IO uint32_t PMUGRF_GPIO0C_SR;                   /* Address Offset: 0x0048 */
         uint32_t RESERVED4[45];                      /* Address Offset: 0x004C */
    __IO uint32_t PMUGRF_SOC_CON[4];                  /* Address Offset: 0x0100 */
         uint32_t RESERVED5[28];                      /* Address Offset: 0x0110 */
    __IO uint32_t PMUGRF_DLL_CON[2];                  /* Address Offset: 0x0180 */
         uint32_t RESERVED6[2];                       /* Address Offset: 0x0188 */
    __IO uint32_t PMUGRF_DLL_STATUS[2];               /* Address Offset: 0x0190 */
         uint32_t RESERVED7[26];                      /* Address Offset: 0x0198 */
    __IO uint32_t PMUGRF_OS_REG[4];                   /* Address Offset: 0x0200 */
         uint32_t RESERVED8[60];                      /* Address Offset: 0x0210 */
    __IO uint32_t PMUGRF_FAST_BOOT_ADDR;              /* Address Offset: 0x0300 */
         uint32_t RESERVED9[31];                      /* Address Offset: 0x0304 */
    __IO uint32_t PMUGRF_A7_JTAG_MASK;                /* Address Offset: 0x0380 */
         uint32_t RESERVED10;                         /* Address Offset: 0x0384 */
    __IO uint32_t PMUGRF_CEVA_JTAG_MASK;              /* Address Offset: 0x0388 */
};
/* CRU Register Structure Define */
struct CRU_REG {
    __IO uint32_t APLL_CON[6];                        /* Address Offset: 0x0000 */
    uint32_t RESERVED0[2];                            /* Address Offset: 0x0018 */
    __IO uint32_t DPLL_CON[6];                        /* Address Offset: 0x0020 */
    uint32_t RESERVED1[2];                            /* Address Offset: 0x0038 */
    __IO uint32_t GPLL_CON[6];                        /* Address Offset: 0x0040 */
    uint32_t RESERVED2[2];                            /* Address Offset: 0x0058 */
    __IO uint32_t CRU_CLKSEL_CON[46];
    uint32_t RESERVED3[2];                            /* Address Offset: 0x0118 */
    __IO uint32_t CRU_CLKGATE_CON[20];
    uint32_t RESERVED4[4];                            /* Address Offset: 0x0170 */
    __IO uint32_t CRU_SOFTRST_CON[13];
    uint32_t RESERVED5[3];                            /* Address Offset: 0x01B4 */
    __IO uint32_t GLB_SRST_FST_VALUE;                 /* Address Offset: 0x01C0 */
    __IO uint32_t GLB_SRST_SND_VALUE;                 /* Address Offset: 0x01C4 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x01C8 */
    __IO uint32_t MISC_CON;                           /* Address Offset: 0x01CC */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x01D0 */
    __IO uint32_t GLB_RST_ST;                          /* Address Offset: 0x01D4 */
    __IO uint32_t SDMMC_CON[2];                       /* Address Offset: 0x01D8 */
    __IO uint32_t SDIO_CON[2];                        /* Address Offset: 0x01E0 */
    __IO uint32_t EMMC_CON[2];                        /* Address Offset: 0x01E8 */
};
/* DDR_PHY Register Structure Define */
struct DDR_PHY_REG {
    __IO uint32_t REG[4];                             /* Address Offset: 0x0000 */
         uint32_t RESERVED0;                          /* Address Offset: 0x0010 */
    __IO uint32_t REG1[2];                            /* Address Offset: 0x0014 */
         uint32_t RESERVED1[2];                       /* Address Offset: 0x001C */
    __IO uint32_t REG9;                               /* Address Offset: 0x0024 */
         uint32_t RESERVED2;                          /* Address Offset: 0x0028 */
    __IO uint32_t REGB;                               /* Address Offset: 0x002C */
    __IO uint32_t REGC;                               /* Address Offset: 0x0030 */
         uint32_t RESERVED3[4];                       /* Address Offset: 0x0034 */
    __IO uint32_t REG2[4];                            /* Address Offset: 0x0044 */
         uint32_t RESERVED4;                          /* Address Offset: 0x0054 */
    __IO uint32_t REG3[3];                            /* Address Offset: 0x0058 */
         uint32_t RESERVED5[7];                       /* Address Offset: 0x0064 */
    __IO uint32_t REG4[2];                            /* Address Offset: 0x0080 */
         uint32_t RESERVED6[4];                       /* Address Offset: 0x0088 */
    __IO uint32_t REG5[3];                            /* Address Offset: 0x0098 */
         uint32_t RESERVED7[2];                       /* Address Offset: 0x00A4 */
    __IO uint32_t REG2B;                              /* Address Offset: 0x00AC */
         uint32_t RESERVED8[2];                       /* Address Offset: 0x00B0 */
    __IO uint32_t REG2E;                              /* Address Offset: 0x00B8 */
    __IO uint32_t REF2F;                              /* Address Offset: 0x00BC */
    __IO uint32_t REG6[2];                            /* Address Offset: 0x00C0 */
         uint32_t RESERVED9[4];                       /* Address Offset: 0x00C8 */
    __IO uint32_t REG7[3];                            /* Address Offset: 0x00D8 */
         uint32_t RESERVED10[2];                      /* Address Offset: 0x00E4 */
    __IO uint32_t REG3B;                              /* Address Offset: 0x00EC */
         uint32_t RESERVED11[2];                      /* Address Offset: 0x00F0 */
    __IO uint32_t REG3E;                              /* Address Offset: 0x00F8 */
    __IO uint32_t REF3F;                              /* Address Offset: 0x00FC */
         uint32_t RESERVED12[48];                     /* Address Offset: 0x0100 */
    __IO uint32_t REG70;                              /* Address Offset: 0x01C0 */
         uint32_t RESERVED13[63];                     /* Address Offset: 0x01C4 */
    __IO uint32_t REG_B[10];                           /* Address Offset: 0x02C0 */
    __IO uint32_t REGBA;                              /* Address Offset: 0x02E8 */
    __IO uint32_t REGBB;                              /* Address Offset: 0x02EC */
    __IO uint32_t REGBC;                              /* Address Offset: 0x02F0 */
    __IO uint32_t REGBD;                              /* Address Offset: 0x02F4 */
         uint32_t RESERVED14[50];                     /* Address Offset: 0x02F8 */
    __IO uint32_t REGF[2];                            /* Address Offset: 0x03C0 */
         uint32_t RESERVED15[8];                      /* Address Offset: 0x03C8 */
    __I  uint32_t REGFA;                              /* Address Offset: 0x03E8 */
    __I  uint32_t REGFB;                              /* Address Offset: 0x03EC */
    __I  uint32_t REGFC;                              /* Address Offset: 0x03F0 */
         uint32_t RESERVED16[2];                      /* Address Offset: 0x03F4 */
    __I  uint32_t REGFF;                              /* Address Offset: 0x03FC */
};
/* DDR_PCTL Register Structure Define */
struct DDR_PCTL_REG {
    __IO uint32_t SCFG;                               /* Address Offset: 0x0000 */
    __IO uint32_t SCTL;                               /* Address Offset: 0x0004 */
    __I  uint32_t STAT;                               /* Address Offset: 0x0008 */
    __I  uint32_t INTRSTAT;                           /* Address Offset: 0x000C */
         uint32_t RESERVED0[12];                      /* Address Offset: 0x0010 */
    __IO uint32_t MCMD;                               /* Address Offset: 0x0040 */
    __IO uint32_t POWCTL;                             /* Address Offset: 0x0044 */
    __I  uint32_t POWSTAT;                            /* Address Offset: 0x0048 */
    __I  uint32_t CMDTSTAT;                           /* Address Offset: 0x004C */
    __IO uint32_t CMDTSTATEN;                         /* Address Offset: 0x0050 */
         uint32_t RESERVED1[3];                       /* Address Offset: 0x0054 */
    __IO uint32_t MRRCFG0;                            /* Address Offset: 0x0060 */
    __IO uint32_t MRRSTAT[2];                         /* Address Offset: 0x0064 */
         uint32_t RESERVED2[4];                       /* Address Offset: 0x006C */
    __IO uint32_t MCFG1;                              /* Address Offset: 0x007C */
    __IO uint32_t MCFG;                               /* Address Offset: 0x0080 */
    __IO uint32_t PPCFG;                              /* Address Offset: 0x0084 */
    __I  uint32_t MSTAT;                              /* Address Offset: 0x0088 */
    __IO uint32_t LPDDR2ZQCFG;                        /* Address Offset: 0x008C */
         uint32_t RESERVED3;                          /* Address Offset: 0x0090 */
    __I  uint32_t DTUPDES;                            /* Address Offset: 0x0094 */
    __I  uint32_t DTUNA;                              /* Address Offset: 0x0098 */
    __I  uint32_t DTUNE;                              /* Address Offset: 0x009C */
    __IO uint32_t DTUPRD[4];                          /* Address Offset: 0x00A0 */
    __IO uint32_t DTUAWDT;                            /* Address Offset: 0x00B0 */
         uint32_t RESERVED4[3];                       /* Address Offset: 0x00B4 */
    __IO uint32_t TOGCNT1U;                           /* Address Offset: 0x00C0 */
    __IO uint32_t TINIT;                              /* Address Offset: 0x00C4 */
    __IO uint32_t TRSTH;                              /* Address Offset: 0x00C8 */
    __IO uint32_t TOGCNT100N;                         /* Address Offset: 0x00CC */
    __IO uint32_t TREFI;                              /* Address Offset: 0x00D0 */
    __IO uint32_t TMRD;                               /* Address Offset: 0x00D4 */
    __IO uint32_t TRFC;                               /* Address Offset: 0x00D8 */
    __IO uint32_t TRP;                                /* Address Offset: 0x00DC */
    __IO uint32_t TRTW;                               /* Address Offset: 0x00E0 */
    __IO uint32_t TAL;                                /* Address Offset: 0x00E4 */
    __IO uint32_t TCL;                                /* Address Offset: 0x00E8 */
    __IO uint32_t TCWL;                               /* Address Offset: 0x00EC */
    __IO uint32_t TRAS;                               /* Address Offset: 0x00F0 */
    __IO uint32_t TRC;                                /* Address Offset: 0x00F4 */
    __IO uint32_t TRCD;                               /* Address Offset: 0x00F8 */
    __IO uint32_t TRRD;                               /* Address Offset: 0x00FC */
    __IO uint32_t TRTP;                               /* Address Offset: 0x0100 */
    __IO uint32_t TWR;                                /* Address Offset: 0x0104 */
    __IO uint32_t TWTR;                               /* Address Offset: 0x0108 */
    __IO uint32_t TEXSR;                              /* Address Offset: 0x010C */
    __IO uint32_t TXP;                                /* Address Offset: 0x0110 */
    __IO uint32_t TXPDLL;                             /* Address Offset: 0x0114 */
    __IO uint32_t TZQCS;                              /* Address Offset: 0x0118 */
    __IO uint32_t TZQCSI;                             /* Address Offset: 0x011C */
    __IO uint32_t TDQS;                               /* Address Offset: 0x0120 */
    __IO uint32_t TCKSRE;                             /* Address Offset: 0x0124 */
    __IO uint32_t TCKSRX;                             /* Address Offset: 0x0128 */
    __IO uint32_t TCKE;                               /* Address Offset: 0x012C */
    __IO uint32_t TMOD;                               /* Address Offset: 0x0130 */
    __IO uint32_t TRSTL;                              /* Address Offset: 0x0134 */
    __IO uint32_t TZQCL;                              /* Address Offset: 0x0138 */
    __IO uint32_t TMRR;                               /* Address Offset: 0x013C */
    __IO uint32_t TCKESR;                             /* Address Offset: 0x0140 */
    __IO uint32_t TDPD;                               /* Address Offset: 0x0144 */
    __IO uint32_t TREFI_MEM_DDR3;                     /* Address Offset: 0x0148 */
         uint32_t RESERVED5[45];                      /* Address Offset: 0x014C */
    __IO uint32_t DTUWACTL;                           /* Address Offset: 0x0200 */
    __IO uint32_t DTURACTL;                           /* Address Offset: 0x0204 */
    __IO uint32_t DTUCFG;                             /* Address Offset: 0x0208 */
    __IO uint32_t DTUECTL;                            /* Address Offset: 0x020C */
    __IO uint32_t DTUWD[4];                           /* Address Offset: 0x0210 */
    __IO uint32_t DTUWDM;                             /* Address Offset: 0x0220 */
    __IO uint32_t DTURD[4];                           /* Address Offset: 0x0224 */
    __IO uint32_t DTULFSRWD;                          /* Address Offset: 0x0234 */
    __IO uint32_t DTULFSRRD;                          /* Address Offset: 0x0238 */
    __I  uint32_t DTUEAF;                             /* Address Offset: 0x023C */
    __IO uint32_t DFITCTRLDELAY;                      /* Address Offset: 0x0240 */
    __IO uint32_t DFIODTCFG;                          /* Address Offset: 0x0244 */
    __IO uint32_t DFIODTCFG1;                         /* Address Offset: 0x0248 */
    __IO uint32_t DFIODTRANKMAP;                      /* Address Offset: 0x024C */
    __IO uint32_t DFITPHYWRDATA;                      /* Address Offset: 0x0250 */
    __IO uint32_t DFITPHYWRLAT;                       /* Address Offset: 0x0254 */
    __IO uint32_t DFITPHYWRDATALAT;                   /* Address Offset: 0x0258 */
         uint32_t RESERVED6;                          /* Address Offset: 0x025C */
    __IO uint32_t DFITRDDATAEN;                       /* Address Offset: 0x0260 */
    __IO uint32_t DFITPHYRDLAT;                       /* Address Offset: 0x0264 */
         uint32_t RESERVED7[2];                       /* Address Offset: 0x0268 */
    __IO uint32_t DFITPHYUPDTYPE[4];                  /* Address Offset: 0x0270 */
    __IO uint32_t DFITCTRLUPDMIN;                     /* Address Offset: 0x0280 */
    __IO uint32_t DFITCTRLUPDMAX;                     /* Address Offset: 0x0284 */
    __IO uint32_t DFITCTRLUPDDLY;                     /* Address Offset: 0x0288 */
         uint32_t RESERVED8;                          /* Address Offset: 0x028C */
    __IO uint32_t DFIUPDCFG;                          /* Address Offset: 0x0290 */
    __IO uint32_t DFITREFMSKI;                        /* Address Offset: 0x0294 */
    __IO uint32_t DFITCTRLUPDI;                       /* Address Offset: 0x0298 */
         uint32_t RESERVED9[4];                       /* Address Offset: 0x029C */
    __IO uint32_t DFITRCFG0;                          /* Address Offset: 0x02AC */
    __I  uint32_t DFITRSTAT0;                         /* Address Offset: 0x02B0 */
    __IO uint32_t DFITRWRLVLEN;                       /* Address Offset: 0x02B4 */
    __IO uint32_t DFITRRDLVLEN;                       /* Address Offset: 0x02B8 */
    __IO uint32_t DFITRRDLVLGATEEN;                   /* Address Offset: 0x02BC */
    __I  uint32_t DFISTSTAT0;                         /* Address Offset: 0x02C0 */
    __IO uint32_t DFISTCFG[2];                        /* Address Offset: 0x02C4 */
         uint32_t RESERVED10;                         /* Address Offset: 0x02CC */
    __IO uint32_t DFITDRAMCLKEN;                      /* Address Offset: 0x02D0 */
    __IO uint32_t DFITDRAMCLKDIS;                     /* Address Offset: 0x02D4 */
    __IO uint32_t DFISTCFG2;                          /* Address Offset: 0x02D8 */
    __IO uint32_t DFISTPARCLR;                        /* Address Offset: 0x02DC */
    __I  uint32_t DFISTPARLOG;                        /* Address Offset: 0x02E0 */
         uint32_t RESERVED11[3];                      /* Address Offset: 0x02E4 */
    __IO uint32_t DFILPCFG0;                          /* Address Offset: 0x02F0 */
         uint32_t RESERVED12[3];                      /* Address Offset: 0x02F4 */
    __IO uint32_t DFITRWRLVLRESP[3];                  /* Address Offset: 0x0300 */
    __IO uint32_t DFITRRDLVLRESP[3];                  /* Address Offset: 0x030C */
    __IO uint32_t DFITRWRLVLDELAY[3];                 /* Address Offset: 0x0318 */
    __IO uint32_t DFITRRDLVLDELAY[3];                 /* Address Offset: 0x0324 */
    __IO uint32_t DFITRRDLVLGATEDELAY[3];             /* Address Offset: 0x0330 */
    __IO uint32_t DFITRCMD;                           /* Address Offset: 0x033C */
         uint32_t RESERVED13[46];                     /* Address Offset: 0x0340 */
    __I  uint32_t IPVR;                               /* Address Offset: 0x03F8 */
    __I  uint32_t IPTR;                               /* Address Offset: 0x03FC */
};
/* USB_GRF Register Structure Define */
struct USB_GRF_REG {
    __IO uint32_t GRF_USBPHY_REG[24];                 /* Address Offset: 0x0000 */
         uint32_t RESERVED0[40];                      /* Address Offset: 0x0060 */
    __IO uint32_t GRF_USBPHY_CON[4];                  /* Address Offset: 0x0100 */
};
/* DDR_MON Register Structure Define */
struct DDR_MON_REG {
    __I  uint32_t IP_VERSION;                         /* Address Offset: 0x0000 */
    __IO uint32_t DFI_CONTROL;                        /* Address Offset: 0x0004 */
    __O  uint32_t INT_STATE;                          /* Address Offset: 0x0008 */
    __IO uint32_t INT_MASK;                           /* Address Offset: 0x000C */
    __IO uint32_t TIMER_COUNT;                        /* Address Offset: 0x0010 */
    __IO uint32_t FLOOR_NUMBER;                       /* Address Offset: 0x0014 */
    __IO uint32_t TOP_NUMBER;                         /* Address Offset: 0x0018 */
    __I  uint32_t DFI_ACT_NUMBER;                     /* Address Offset: 0x001C */
    __I  uint32_t DFI_ACT_WR_NUM;                     /* Address Offset: 0x0020 */
    __I  uint32_t DFI_ACT_RD_NUM;                     /* Address Offset: 0x0024 */
    __I  uint32_t DFI_COUNT_NUM;                      /* Address Offset: 0x0028 */
    __I  uint32_t DFI_ACT_ACCESS_NUM;                 /* Address Offset: 0x002C */
};
/* RKVENC Register Structure Define */
struct RKVENC_REG {
    __I  uint32_t VERSION;                            /* Address Offset: 0x0000 */
    __IO uint32_t ENC_STRT;                           /* Address Offset: 0x0004 */
    __IO uint32_t ENC_CLR;                            /* Address Offset: 0x0008 */
    __IO uint32_t LKT_ADDR;                           /* Address Offset: 0x000C */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x0010 */
    __IO uint32_t INT_MSK;                            /* Address Offset: 0x0014 */
    __IO uint32_t INT_CLR;                            /* Address Offset: 0x0018 */
    __IO uint32_t INT_STUS;                           /* Address Offset: 0x001C */
         uint32_t RESERVED0[4];                       /* Address Offset: 0x0020 */
    __IO uint32_t ENC_RSL;                            /* Address Offset: 0x0030 */
    __IO uint32_t ENC_PIC;                            /* Address Offset: 0x0034 */
    __IO uint32_t ENC_WDG;                            /* Address Offset: 0x0038 */
         uint32_t RESERVED1[117];                     /* Address Offset: 0x003C */
    __I  uint32_t ST_BSL;                             /* Address Offset: 0x0210 */
    __IO uint32_t ST_SEE_L32;                         /* Address Offset: 0x0214 */
         uint32_t RESERVED2[436];                     /* Address Offset: 0x0218 */
    __I  uint32_t DBG_DMA_CH_ST;                      /* Address Offset: 0x08E8 */
};
/* RKVDEC Register Structure Define */
struct RKVDEC_REG {
    __IO uint32_t SWREG0_ID;                          /* Address Offset: 0x0000 */
    __IO uint32_t SWREG1_INT;                         /* Address Offset: 0x0004 */
    __IO uint32_t SWREG2_SYSCTRL;                     /* Address Offset: 0x0008 */
    __IO uint32_t SWREG3_PICPAR;                      /* Address Offset: 0x000C */
    __IO uint32_t SWREG4_STRM_RLC_BASE;               /* Address Offset: 0x0010 */
    __IO uint32_t SWREG5_STREAM_RLC_LEN;              /* Address Offset: 0x0014 */
    __IO uint32_t SWREG6_CABACTBL_PROB_BASE;          /* Address Offset: 0x0018 */
    __IO uint32_t SWREG7_DECOUT_BASE;                 /* Address Offset: 0x001C */
    __IO uint32_t SWREG8_Y_VIRSTRIDE;                 /* Address Offset: 0x0020 */
    __IO uint32_t SWREG9_YUV_VIRSTRIDE;               /* Address Offset: 0x0024 */
    __IO uint32_t SWREG10_HEVC_REFER0_BASE;           /* Address Offset: 0x0028 */
    __IO uint32_t SWREG10_H264_REFER0_BASE;           /* Address Offset: 0x0028 */
    __IO uint32_t SWREG10_VP9_CPRHEADER_OFFSET;       /* Address Offset: 0x0028 */
    __IO uint32_t SWREG11_HEVC_REFER1_BASE;           /* Address Offset: 0x002C */
    __IO uint32_t SWREG11_H264_REFER1_BASE;           /* Address Offset: 0x002C */
    __IO uint32_t SWREG11_VP9_REFERLAST_BASE;         /* Address Offset: 0x002C */
    __IO uint32_t SWREG12_HEVC_REFER2_BASE;           /* Address Offset: 0x0030 */
    __IO uint32_t SWREG12_H264_REFER2_BASE;           /* Address Offset: 0x0030 */
    __IO uint32_t SWREG12_VP9_REFERGOLDEN_BASE;       /* Address Offset: 0x0030 */
    __IO uint32_t SWREG13_HEVC_REFER3_BASE;           /* Address Offset: 0x0034 */
    __IO uint32_t SWREG13_H264_REFER3_BASE;           /* Address Offset: 0x0034 */
    __IO uint32_t SWREG13_VP9_REFERALFTER_BASE;       /* Address Offset: 0x0034 */
    __IO uint32_t SWREG14_HEVC_REFER4_BASE;           /* Address Offset: 0x0038 */
    __IO uint32_t SWREG14_H264_REFER4_BASE;           /* Address Offset: 0x0038 */
    __IO uint32_t SWREG14_VP9COUNT_BASE;              /* Address Offset: 0x0038 */
    __IO uint32_t SWREG15_H264_REFER5_BASE;           /* Address Offset: 0x003C */
    __IO uint32_t SWREG15_VP9_SEGIDLAST_BASE;         /* Address Offset: 0x003C */
    __IO uint32_t SWREG15_HEVC_REFER5_BASE;           /* Address Offset: 0x003C */
    __IO uint32_t SWREG16_H264_REFER6_BASE;           /* Address Offset: 0x0040 */
    __IO uint32_t SWREG16_VP9_SEGIDCUR_BASE;          /* Address Offset: 0x0040 */
    __IO uint32_t SWREG16_HEVC_REFER6_BASE;           /* Address Offset: 0x0040 */
    __IO uint32_t SWREG17_H264_REFER7_BASE;           /* Address Offset: 0x0044 */
    __IO uint32_t SWREG17_VP9_FRAME_SIZE_LAST;        /* Address Offset: 0x0044 */
    __IO uint32_t SWREG17_HEVC_REFER7_BASE;           /* Address Offset: 0x0044 */
    __IO uint32_t SWREG18_H264_REFER8_BASE;           /* Address Offset: 0x0048 */
    __IO uint32_t SWREG18_VP9_FRAME_SIZE_GOLDEN;      /* Address Offset: 0x0048 */
    __IO uint32_t SWREG18_HEVC_REFER8_BASE;           /* Address Offset: 0x0048 */
    __IO uint32_t SWREG19_H264_REFER9_BASE;           /* Address Offset: 0x004C */
    __IO uint32_t SWREG19_VP9_FRAME_SIZE_ALTREF;      /* Address Offset: 0x004C */
    __IO uint32_t SWREG19_HEVC_REFER9_BASE;           /* Address Offset: 0x004C */
    __IO uint32_t SWREG20_H264_REFER10_BASE;          /* Address Offset: 0x0050 */
    __IO uint32_t SWREG20_VP9_SEGID_GRP0;             /* Address Offset: 0x0050 */
    __IO uint32_t SWREG20_HEVC_REFER10_BASE;          /* Address Offset: 0x0050 */
    __IO uint32_t SWREG21_H264_REFER11_BASE;          /* Address Offset: 0x0054 */
    __IO uint32_t SWREG21_VP9_SEGID_GRP1;             /* Address Offset: 0x0054 */
    __IO uint32_t SWREG21_HEVC_REFER11_BASE;          /* Address Offset: 0x0054 */
    __IO uint32_t SWREG22_H264_REFER12_BASE;          /* Address Offset: 0x0058 */
    __IO uint32_t SWREG22_VP9_SEGID_GRP2;             /* Address Offset: 0x0058 */
    __IO uint32_t SWREG22_HEVC_REFER12_BASE;          /* Address Offset: 0x0058 */
    __IO uint32_t SWREG23_H264_REFER13_BASE;          /* Address Offset: 0x005C */
    __IO uint32_t SWREG23_VP9_SEGID_GRP3;             /* Address Offset: 0x005C */
    __IO uint32_t SWREG23_HEVC_REFER13_BASE;          /* Address Offset: 0x005C */
    __IO uint32_t SWREG24_H264_REFER14_BASE;          /* Address Offset: 0x0060 */
    __IO uint32_t SWREG24_VP9_SEGID_GRP4;             /* Address Offset: 0x0060 */
    __IO uint32_t SWREG24_HEVC_REFER14_BASE;          /* Address Offset: 0x0060 */
    __IO uint32_t SWREG25_VP9_SEGID_GRP5;             /* Address Offset: 0x0064 */
    __IO uint32_t SWREG25_REFER0_POC;                 /* Address Offset: 0x0064 */
    __IO uint32_t SWREG26_VP9_SEGID_GRP6;             /* Address Offset: 0x0068 */
    __IO uint32_t SWREG26_REFER1_POC;                 /* Address Offset: 0x0068 */
    __IO uint32_t SWREG27_VP9_SEGID_GRP7;             /* Address Offset: 0x006C */
    __IO uint32_t SWREG27_REFER2_POC;                 /* Address Offset: 0x006C */
    __IO uint32_t SWREG28_VP9_CPRHEADER_CONFIG;       /* Address Offset: 0x0070 */
    __IO uint32_t SWREG28_REFER3_POC;                 /* Address Offset: 0x0070 */
    __IO uint32_t SWREG29_VP9_LREF_SCALE;             /* Address Offset: 0x0074 */
    __IO uint32_t SWREG29_REFER4_POC;                 /* Address Offset: 0x0074 */
    __IO uint32_t SWREG30_VP9_GREF_SCALE;             /* Address Offset: 0x0078 */
    __IO uint32_t SWREG30_REFER5_POC;                 /* Address Offset: 0x0078 */
    __IO uint32_t SWREG31_VP9_AREF_SCALE;             /* Address Offset: 0x007C */
    __IO uint32_t SWREG31_REFER6_POC;                 /* Address Offset: 0x007C */
    __IO uint32_t SWREG32_VP9_REF_DELTAS_LASTFRAME;   /* Address Offset: 0x0080 */
    __IO uint32_t SWREG32_REFER7_POC;                 /* Address Offset: 0x0080 */
    __IO uint32_t SWREG33_VP9_INFO_LASTFRAME;         /* Address Offset: 0x0084 */
    __IO uint32_t SWREG33_REFER8_POC;                 /* Address Offset: 0x0084 */
    __IO uint32_t SWREG34_VP9_INTERCMD_BASE;          /* Address Offset: 0x0088 */
    __IO uint32_t SWREG34_REFER9_POC;                 /* Address Offset: 0x0088 */
    __IO uint32_t SWREG35_VP9_INTERCMD_NUM;           /* Address Offset: 0x008C */
    __IO uint32_t SWREG35_REFER10_POC;                /* Address Offset: 0x008C */
    __IO uint32_t SWREG36_VP9_LASTTILE_SIZE;          /* Address Offset: 0x0090 */
    __IO uint32_t SWREG36_REFER11_POC;                /* Address Offset: 0x0090 */
    __IO uint32_t SWREG37_VP9_LASTF_HOR_VIRSTRIDE;    /* Address Offset: 0x0094 */
    __IO uint32_t SWREG37_REFER12_POC;                /* Address Offset: 0x0094 */
    __IO uint32_t SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE;  /* Address Offset: 0x0098 */
    __IO uint32_t SWREG38_REFER13_POC;                /* Address Offset: 0x0098 */
    __IO uint32_t SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE;  /* Address Offset: 0x009C */
    __IO uint32_t SWREG39_REFER14_POC;                /* Address Offset: 0x009C */
    __IO uint32_t SWREG40_CUR_POC;                    /* Address Offset: 0x00A0 */
    __IO uint32_t SWREG41_RLCWRITE_BASE;              /* Address Offset: 0x00A4 */
    __IO uint32_t SWREG42_PPS_BASE;                   /* Address Offset: 0x00A8 */
    __IO uint32_t SWREG43_RPS_BASE;                   /* Address Offset: 0x00AC */
    __IO uint32_t SWREG44_STRMD_ERROR_EN;             /* Address Offset: 0x00B0 */
    __IO uint32_t SWREG45_STRMD_ERROR_STATUS;         /* Address Offset: 0x00B4 */
    __IO uint32_t SWREG45_VP9_ERROR_INFO0;            /* Address Offset: 0x00B4 */
    __IO uint32_t SWREG46_STRMD_ERROR_CTU;            /* Address Offset: 0x00B8 */
    __IO uint32_t SWREG47_SAO_CTU_POSITION;           /* Address Offset: 0x00BC */
    __IO uint32_t SWREG48_H264_REFER15_BASE;          /* Address Offset: 0x00C0 */
    __IO uint32_t SWREG48_VP9_LAST_YSTRIDE;           /* Address Offset: 0x00C0 */
    __IO uint32_t SWREG49_H264_REFER15_POC;           /* Address Offset: 0x00C4 */
    __IO uint32_t SWREG49_VP9_GOLDEN_YSTRIDE;         /* Address Offset: 0x00C4 */
    __IO uint32_t SWREG50_H264_REFER16_POC;           /* Address Offset: 0x00C8 */
    __IO uint32_t SWREG50_VP9_ALTREFY_YSTRIDE;        /* Address Offset: 0x00C8 */
    __IO uint32_t SWREG51_H264_REFER17_POC;           /* Address Offset: 0x00CC */
    __IO uint32_t SWREG51_VP9_LASTREF_YUVSTRIDE;      /* Address Offset: 0x00CC */
    __IO uint32_t SWREG52_VP9_REFCOLMV_BASE;          /* Address Offset: 0x00D0 */
    __IO uint32_t SWREG52_H264_REFER18_POC;           /* Address Offset: 0x00D0 */
    __IO uint32_t SWREG53_H264_REFER19_POC;           /* Address Offset: 0x00D4 */
    __IO uint32_t SWREG54_H264_REFER20_POC;           /* Address Offset: 0x00D8 */
    __IO uint32_t SWREG55_H264_REFER21_POC;           /* Address Offset: 0x00DC */
    __IO uint32_t SWREG56_H264_REFER22_POC;           /* Address Offset: 0x00E0 */
    __IO uint32_t SWREG57_H264_REFER23_POC;           /* Address Offset: 0x00E4 */
    __IO uint32_t SWREG58_H264_REFER24_POC;           /* Address Offset: 0x00E8 */
    __IO uint32_t SWREG59_H264_REFER25_POC;           /* Address Offset: 0x00EC */
    __IO uint32_t SWREG60_H264_REFER26_POC;           /* Address Offset: 0x00F0 */
    __IO uint32_t SWREG61_H264_REFER27_POC;           /* Address Offset: 0x00F4 */
    __IO uint32_t SWREG62_H264_REFER28_POC;           /* Address Offset: 0x00F8 */
    __IO uint32_t SWREG63_H264_REFER29_POC;           /* Address Offset: 0x00FC */
    __IO uint32_t SWREG64_PERFORMANCE_CYCLE;          /* Address Offset: 0x0100 */
    __IO uint32_t SWREG65_AXI_DDR_RDATA;              /* Address Offset: 0x0104 */
    __IO uint32_t SWREG66_AXI_DDR_WDATA;              /* Address Offset: 0x0108 */
    __IO uint32_t SWREG67_FPGADEBUG_RESET;            /* Address Offset: 0x010C */
    __IO uint32_t SWREG68_PERFORMANCE_SEL;            /* Address Offset: 0x0110 */
    __IO uint32_t SWREG69_PERFORMANCE_CNT0;           /* Address Offset: 0x0114 */
    __IO uint32_t SWREG70_PERFORMANCE_CNT1;           /* Address Offset: 0x0118 */
    __IO uint32_t SWREG71_PERFORMANCE_CNT2;           /* Address Offset: 0x011C */
    __IO uint32_t SWREG72_H264_REFER30_POC;           /* Address Offset: 0x0120 */
    __IO uint32_t SWREG73_H264_REFER31_POC;           /* Address Offset: 0x0124 */
    __IO uint32_t SWREG74_H264_CUR_POC1;              /* Address Offset: 0x0128 */
    __IO uint32_t SWREG75_H264_ERRORINFO_BASE;        /* Address Offset: 0x012C */
    __IO uint32_t SWREG75_VP9_ERROR_INFO1;            /* Address Offset: 0x012C */
    __IO uint32_t SWREG76_H264_ERRORINFO_NUM;         /* Address Offset: 0x0130 */
    __IO uint32_t SWREG76_VP9_ERROR_CTU1;             /* Address Offset: 0x0130 */
    __IO uint32_t SWREG77_H264_ERROR_E;               /* Address Offset: 0x0134 */
    __IO uint32_t SWREG78_COLMV_CUR_BASE;             /* Address Offset: 0x0138 */
    __IO uint32_t SWREG79_COLMV_REF0_BASE;            /* Address Offset: 0x013C */
    __IO uint32_t SWREG80_COLMV_REF1_BASE;            /* Address Offset: 0x0140 */
    __IO uint32_t SWREG81_COLMV_REF2_BASE;            /* Address Offset: 0x0144 */
    __IO uint32_t SWREG82_COLMV_REF3_BASE;            /* Address Offset: 0x0148 */
    __IO uint32_t SWREG83_COLMV_REF4_BASE;            /* Address Offset: 0x014C */
    __IO uint32_t SWREG84_COLMV_REF5_BASE;            /* Address Offset: 0x0150 */
    __IO uint32_t SWREG85_COLMV_REF6_BASE;            /* Address Offset: 0x0154 */
    __IO uint32_t SWREG86_COLMV_REF7_BASE;            /* Address Offset: 0x0158 */
    __IO uint32_t SWREG87_COLMV_REF8_BASE;            /* Address Offset: 0x015C */
    __IO uint32_t SWREG88_COLMV_REF9_BASE;            /* Address Offset: 0x0160 */
    __IO uint32_t SWREG89_COLMV_REF10_BASE;           /* Address Offset: 0x0164 */
    __IO uint32_t SWREG90_COLMV_REF11_BASE;           /* Address Offset: 0x0168 */
    __IO uint32_t SWREG91_COLMV_REF12_BASE;           /* Address Offset: 0x016C */
    __IO uint32_t SWREG92_COLMV_REF13_BASE;           /* Address Offset: 0x0170 */
    __IO uint32_t SWREG93_COLMV_REF14_BASE;           /* Address Offset: 0x0174 */
    __IO uint32_t SWREG94_COLMV_REF15_BASE;           /* Address Offset: 0x0178 */
    __IO uint32_t SWREG95_SCANLIST_ADDR;              /* Address Offset: 0x017C */
    __IO uint32_t SWREG100_PERF_LATENCY_CTRL0;        /* Address Offset: 0x0190 */
    __IO uint32_t SWREG101_PERF_LATENCY_CTRL1;        /* Address Offset: 0x0194 */
    __I  uint32_t SWREG102_PERF_RD_MAX_LATENCY_NUM0;  /* Address Offset: 0x0198 */
    __I  uint32_t SWREG103_PERF_RD_LATENCY_SAMP_NUM;  /* Address Offset: 0x019C */
    __I  uint32_t SWREG104_PERF_RD_LATENCY_ACC_SUM;   /* Address Offset: 0x01A0 */
    __IO uint32_t SWREG105_PERF_RD_AXI_TOTAL_BYTE;    /* Address Offset: 0x01A4 */
    __IO uint32_t SWREG106_PERF_WR_AXI_TOTAL_BYTE;    /* Address Offset: 0x01A8 */
    __IO uint32_t SWREG107_PERF_WORKING_CNT;          /* Address Offset: 0x01AC */
};
/* IEP Register Structure Define */
struct IEP_REG {
    __IO uint32_t CONFIG[2];                          /* Address Offset: 0x0000 */
    __IO uint32_t STATUS;                             /* Address Offset: 0x0008 */
    __IO uint32_t INT;                                /* Address Offset: 0x000C */
    __O  uint32_t FRM_START;                          /* Address Offset: 0x0010 */
         uint32_t RESERVED0;                          /* Address Offset: 0x0014 */
    __IO uint32_t CONFIG_DONE;                        /* Address Offset: 0x0018 */
    __IO uint32_t FRM_CNT;                            /* Address Offset: 0x001C */
    __IO uint32_t VIR_IMG_WIDTH;                      /* Address Offset: 0x0020 */
         uint32_t RESERVED1;                          /* Address Offset: 0x0024 */
    __IO uint32_t SRC_IMG_SIZE;                       /* Address Offset: 0x0028 */
    __IO uint32_t DST_IMG_SIZE;                       /* Address Offset: 0x002C */
    __IO uint32_t DST_IMG_WIDTH_TILE[4];              /* Address Offset: 0x0030 */
    __IO uint32_t ENH_YUV_CNFG[3];                    /* Address Offset: 0x0040 */
    __IO uint32_t ENH_RGB_CNFG;                       /* Address Offset: 0x004C */
    __IO uint32_t ENH_C_COE;                          /* Address Offset: 0x0050 */
         uint32_t RESERVED2;                          /* Address Offset: 0x0054 */
    __IO uint32_t RAW_CONFIG[2];                      /* Address Offset: 0x0058 */
    __I  uint32_t RAW_VIR_IMG_WIDTH;                  /* Address Offset: 0x0060 */
    __I  uint32_t RAW_IMG_SCL_FCT;                    /* Address Offset: 0x0064 */
    __I  uint32_t RAW_SRC_IMG_SIZE;                   /* Address Offset: 0x0068 */
    __I  uint32_t RAW_DST_IMG_SIZE;                   /* Address Offset: 0x006C */
    __IO uint32_t RAW_ENH_YUV_CNFG[3];                /* Address Offset: 0x0070 */
    __IO uint32_t RAW_ENH_RGB_CNFG;                   /* Address Offset: 0x007C */
    __IO uint32_t SRC_ADDR_YRGB;                      /* Address Offset: 0x0080 */
    __IO uint32_t SRC_ADDR_CBCR;                      /* Address Offset: 0x0084 */
    __IO uint32_t SRC_ADDR_CR;                        /* Address Offset: 0x0088 */
    __IO uint32_t SRC_ADDR_Y1;                        /* Address Offset: 0x008C */
    __IO uint32_t SRC_ADDR_CBCR1;                     /* Address Offset: 0x0090 */
    __IO uint32_t SRC_ADDR_CR1;                       /* Address Offset: 0x0094 */
    __IO uint32_t SRC_ADDR_Y_ITEMP;                   /* Address Offset: 0x0098 */
    __IO uint32_t SRC_ADDR_CBCR_ITEMP;                /* Address Offset: 0x009C */
    __IO uint32_t SRC_ADDR_CR_ITEMP;                  /* Address Offset: 0x00A0 */
    __IO uint32_t SRC_ADDR_Y_FTEMP;                   /* Address Offset: 0x00A4 */
    __IO uint32_t SRC_ADDR_CBCR_FTEMP;                /* Address Offset: 0x00A8 */
    __IO uint32_t SRC_ADDR_CR_FTEMP;                  /* Address Offset: 0x00AC */
    __IO uint32_t DST_ADDR_YRGB;                      /* Address Offset: 0x00B0 */
    __IO uint32_t DST_ADDR_CBCR;                      /* Address Offset: 0x00B4 */
    __IO uint32_t DST_ADDR_CR;                        /* Address Offset: 0x00B8 */
    __IO uint32_t DST_ADDR_Y1;                        /* Address Offset: 0x00BC */
    __IO uint32_t DST_ADDR_CBCR1;                     /* Address Offset: 0x00C0 */
    __IO uint32_t DST_ADDR_CR1;                       /* Address Offset: 0x00C4 */
    __IO uint32_t DST_ADDR_Y_ITEMP;                   /* Address Offset: 0x00C8 */
    __IO uint32_t DST_ADDR_CBCR_ITEMP;                /* Address Offset: 0x00CC */
    __IO uint32_t DST_ADDR_CR_ITEMP;                  /* Address Offset: 0x00D0 */
    __IO uint32_t DST_ADDR_Y_FTEMP;                   /* Address Offset: 0x00D4 */
    __IO uint32_t DST_ADDR_CBCR_FTEMP;                /* Address Offset: 0x00D8 */
    __IO uint32_t DST_ADDR_CR_FTEMP;                  /* Address Offset: 0x00DC */
    __IO uint32_t DIL_MTN_TAB[8];                     /* Address Offset: 0x00E0 */
    __IO uint32_t ENH_CG_TAB;                         /* Address Offset: 0x0100 */
         uint32_t RESERVED3[191];                     /* Address Offset: 0x0104 */
    __IO uint32_t ENH_DDE_COE0;                       /* Address Offset: 0x0400 */
         uint32_t RESERVED4[63];                      /* Address Offset: 0x0404 */
    __IO uint32_t ENH_DDE_COE1;                       /* Address Offset: 0x0500 */
         uint32_t RESERVED5[191];                     /* Address Offset: 0x0504 */
    __IO uint32_t MMU_DTE_ADDR;                       /* Address Offset: 0x0800 */
    __IO uint32_t MMU_STATUS;                         /* Address Offset: 0x0804 */
    __IO uint32_t MMU_CMD;                            /* Address Offset: 0x0808 */
    __IO uint32_t MMU_PAGE_FAULT_ADDR;                /* Address Offset: 0x080C */
    __IO uint32_t MMU_ZAP_ONE_LINE;                   /* Address Offset: 0x0810 */
    __IO uint32_t MMU_INT_RAWSTAT;                    /* Address Offset: 0x0814 */
    __IO uint32_t MMU_INT_CLEAR;                      /* Address Offset: 0x0818 */
    __IO uint32_t MMU_INT_MASK;                       /* Address Offset: 0x081C */
    __IO uint32_t MMU_INT_STATUS;                     /* Address Offset: 0x0820 */
    __IO uint32_t MMU_AUTO_GATING;                    /* Address Offset: 0x0824 */
};
/* RGA Register Structure Define */
struct RGA_REG {
    __IO uint32_t SYS_CTRL;                           /* Address Offset: 0x0000 */
    __IO uint32_t CMD_CTRL;                           /* Address Offset: 0x0004 */
    __IO uint32_t CMD_BASE;                           /* Address Offset: 0x0008 */
    __I  uint32_t STATUS1;                            /* Address Offset: 0x000C */
    __IO uint32_t INT;                                /* Address Offset: 0x0010 */
    __IO uint32_t MMU_CTRL0;                          /* Address Offset: 0x0014 */
    __IO uint32_t MMU_CMD_BASE;                       /* Address Offset: 0x0018 */
    __I  uint32_t STATUS2;                            /* Address Offset: 0x001C */
    __IO uint32_t WORK_CNT;                           /* Address Offset: 0x0020 */
         uint32_t RESERVED0;                          /* Address Offset: 0x0024 */
    __IO uint32_t VERSION_INFO;                       /* Address Offset: 0x0028 */
         uint32_t RESERVED1[5];                       /* Address Offset: 0x002C */
    __IO uint32_t PERF_LATENCY_CTRL[2];               /* Address Offset: 0x0040 */
    __I  uint32_t PERF_RD_MAX_LATENCY_NUM0;           /* Address Offset: 0x0048 */
    __I  uint32_t PERF_RD_LATENCY_SAMP_NUM;           /* Address Offset: 0x004C */
    __I  uint32_t PERF_RD_LATENCY_ACC_SUM;            /* Address Offset: 0x0050 */
    __IO uint32_t PERF_RD_AXI_TOTAL_BYTE;             /* Address Offset: 0x0054 */
    __IO uint32_t PERF_WR_AXI_TOTAL_BYTE;             /* Address Offset: 0x0058 */
    __IO uint32_t PERF_WORKING_CNT;                   /* Address Offset: 0x005C */
         uint32_t RESERVED2[40];                      /* Address Offset: 0x0060 */
    __IO uint32_t MODE_CTRL;                          /* Address Offset: 0x0100 */
    __IO uint32_t SRC_INFO;                           /* Address Offset: 0x0104 */
    __IO uint32_t SRC_BASE[4];                        /* Address Offset: 0x0108 */
    __IO uint32_t SRC_VIR_INFO;                       /* Address Offset: 0x0118 */
    __IO uint32_t SRC_ACT_INFO;                       /* Address Offset: 0x011C */
    __IO uint32_t SRC_X_FACTOR;                       /* Address Offset: 0x0120 */
    __IO uint32_t SRC_Y_FACTOR;                       /* Address Offset: 0x0124 */
    __IO uint32_t SRC_BG_COLOR;                       /* Address Offset: 0x0128 */
    __IO uint32_t SRC_FG_COLOR;                       /* Address Offset: 0x012C */
    __IO uint32_t SRC_TR_COLOR0;                      /* Address Offset: 0x0130 */
    __IO uint32_t CP_GR_A;                            /* Address Offset: 0x0130 */
    __IO uint32_t SRC_TR_COLOR1;                      /* Address Offset: 0x0134 */
    __IO uint32_t CP_GR_B;                            /* Address Offset: 0x0134 */
    __IO uint32_t DST_INFO;                           /* Address Offset: 0x0138 */
    __IO uint32_t DST_BASE[3];                        /* Address Offset: 0x013C */
    __IO uint32_t DST_VIR_INFO;                       /* Address Offset: 0x0148 */
    __IO uint32_t DST_ACT_INFO;                       /* Address Offset: 0x014C */
    __IO uint32_t ALPHA_CTRL[2];                      /* Address Offset: 0x0150 */
    __IO uint32_t FADING_CTRL;                        /* Address Offset: 0x0158 */
    __IO uint32_t PAT_CON;                            /* Address Offset: 0x015C */
    __IO uint32_t ROP_CON0;                           /* Address Offset: 0x0160 */
    __IO uint32_t CP_GR_G;                            /* Address Offset: 0x0160 */
    __IO uint32_t ROP_CON1;                           /* Address Offset: 0x0164 */
    __IO uint32_t CP_GR_R;                            /* Address Offset: 0x0164 */
    __IO uint32_t MASK_BASE;                          /* Address Offset: 0x0168 */
    __IO uint32_t MMU_CTRL1;                          /* Address Offset: 0x016C */
    __IO uint32_t MMU_SRC_BASE;                       /* Address Offset: 0x0170 */
    __IO uint32_t MMU_SRC1_BASE;                      /* Address Offset: 0x0174 */
    __IO uint32_t MMU_DST_BASE;                       /* Address Offset: 0x0178 */
    __IO uint32_t MMU_ELS_BASE;                       /* Address Offset: 0x017C */
};
/* NANDC Register Structure Define */
struct NANDC_REG {
    __IO uint32_t FMCTL;                              /* Address Offset: 0x0000 */
    __IO uint32_t FMWAIT_ASYN;                        /* Address Offset: 0x0004 */
    __IO uint32_t FLCTL;                              /* Address Offset: 0x0008 */
    __IO uint32_t BCHCTL;                             /* Address Offset: 0x000C */
    __IO uint32_t MTRANS_CFG;                         /* Address Offset: 0x0010 */
    __IO uint32_t MTRANS_SADDR[2];                    /* Address Offset: 0x0014 */
    __I  uint32_t MTRANS_STAT;                        /* Address Offset: 0x001C */
    __IO uint32_t BCHST[8];                           /* Address Offset: 0x0020 */
    __IO uint32_t BCHLOC[7];                          /* Address Offset: 0x0040 */
         uint32_t RESERVED0[5];                       /* Address Offset: 0x005C */
    __IO uint32_t BCHDE0[24];                         /* Address Offset: 0x0070 */
    __IO uint32_t BCHDE1[24];                         /* Address Offset: 0x00D0 */
    __IO uint32_t DLL_CTL_REG[2];                     /* Address Offset: 0x0130 */
    __I  uint32_t DLL_OBS_REG0;                       /* Address Offset: 0x0138 */
         uint32_t RESERVED1[5];                       /* Address Offset: 0x013C */
    __IO uint32_t RANDMZ_CFG;                         /* Address Offset: 0x0150 */
         uint32_t RESERVED2;                          /* Address Offset: 0x0154 */
    __IO uint32_t FMWAIT_SYN;                         /* Address Offset: 0x0158 */
    __I  uint32_t MTRANS_STAT2;                       /* Address Offset: 0x015C */
    __I  uint32_t NANDC_VER;                          /* Address Offset: 0x0160 */
    __IO uint32_t LLP_CTL;                            /* Address Offset: 0x0164 */
    __I  uint32_t LLP_STAT;                           /* Address Offset: 0x0168 */
    __IO uint32_t INTEN;                              /* Address Offset: 0x016C */
    __IO uint32_t INTCLR;                             /* Address Offset: 0x0170 */
    __I  uint32_t INTST;                              /* Address Offset: 0x0174 */
         uint32_t RESERVED3[34];                      /* Address Offset: 0x0178 */
    __IO uint32_t SPARE0[12];                         /* Address Offset: 0x0200 */
    __IO uint32_t SPARE1[12];                         /* Address Offset: 0x0230 */
    __IO uint32_t SPARE01[16];                        /* Address Offset: 0x0260 */
    __IO uint32_t SPARE11[16];                        /* Address Offset: 0x02A0 */
         uint32_t RESERVED4[72];                      /* Address Offset: 0x02E0 */
    __IO uint32_t BCHDE01[36];                        /* Address Offset: 0x0400 */
    __IO uint32_t BCHDE11[36];                        /* Address Offset: 0x0490 */
    __IO uint32_t BCHST1[8];                          /* Address Offset: 0x0520 */
         uint32_t RESERVED5[48];                      /* Address Offset: 0x0540 */
    __IO uint32_t RANDMZ_SEED13[16];                  /* Address Offset: 0x0600 */
    __IO uint32_t RANDMZ_SEED17[16];                  /* Address Offset: 0x0640 */
    __IO uint32_t RANDMZ_SEED19[16];                  /* Address Offset: 0x0680 */
    __IO uint32_t RANDMZ_SEED23[16];                  /* Address Offset: 0x06C0 */
         uint32_t RESERVED6[64];                      /* Address Offset: 0x0700 */
    __IO uint32_t FLASH0_DATA;                        /* Address Offset: 0x0800 */
    __O  uint32_t FLASH0_ADDR;                        /* Address Offset: 0x0804 */
    __O  uint32_t FLASH0_CMD;                         /* Address Offset: 0x0808 */
    __IO uint32_t FLASH0_DATA_SYN;                    /* Address Offset: 0x080C */
         uint32_t RESERVED7[60];                      /* Address Offset: 0x0810 */
    __IO uint32_t FLASH1_DATA;                        /* Address Offset: 0x0900 */
    __O  uint32_t FLASH1_ADDR;                        /* Address Offset: 0x0904 */
    __O  uint32_t FLASH1_CMD;                         /* Address Offset: 0x0908 */
    __IO uint32_t FLASH1_DATA_SYN;                    /* Address Offset: 0x090C */
         uint32_t RESERVED8[60];                      /* Address Offset: 0x0910 */
    __IO uint32_t FLASH2_DATA;                        /* Address Offset: 0x0A00 */
    __O  uint32_t FLASH2_ADDR;                        /* Address Offset: 0x0A04 */
    __O  uint32_t FLASH2_CMD;                         /* Address Offset: 0x0A08 */
    __IO uint32_t FLASH2_DATA_SYN;                    /* Address Offset: 0x0A0C */
         uint32_t RESERVED9[60];                      /* Address Offset: 0x0A10 */
    __IO uint32_t FLASH3_DATA;                        /* Address Offset: 0x0B00 */
    __O  uint32_t FLASH3_ADDR;                        /* Address Offset: 0x0B04 */
    __O  uint32_t FLASH3_CMD;                         /* Address Offset: 0x0B08 */
    __IO uint32_t FLASH3_DATA_SYN;                    /* Address Offset: 0x0B0C */
         uint32_t RESERVED10[60];                     /* Address Offset: 0x0B10 */
    __IO uint32_t FLASH4_DATA;                        /* Address Offset: 0x0C00 */
    __O  uint32_t FLASH4_ADDR;                        /* Address Offset: 0x0C04 */
    __O  uint32_t FLASH4_CMD;                         /* Address Offset: 0x0C08 */
    __IO uint32_t FLASH4_DATA_SYN;                    /* Address Offset: 0x0C0C */
         uint32_t RESERVED11[60];                     /* Address Offset: 0x0C10 */
    __IO uint32_t FLASH5_DATA;                        /* Address Offset: 0x0D00 */
    __O  uint32_t FLASH5_ADDR;                        /* Address Offset: 0x0D04 */
    __O  uint32_t FLASH5_CMD;                         /* Address Offset: 0x0D08 */
    __IO uint32_t FLASH5_DATA_SYN;                    /* Address Offset: 0x0D0C */
         uint32_t RESERVED12[60];                     /* Address Offset: 0x0D10 */
    __IO uint32_t FLASH6_DATA;                        /* Address Offset: 0x0E00 */
    __O  uint32_t FLASH6_ADDR;                        /* Address Offset: 0x0E04 */
    __O  uint32_t FLASH6_CMD;                         /* Address Offset: 0x0E08 */
    __IO uint32_t FLASH6_DATA_SYN;                    /* Address Offset: 0x0E0C */
         uint32_t RESERVED13[60];                     /* Address Offset: 0x0E10 */
    __IO uint32_t FLASH7_DATA;                        /* Address Offset: 0x0F00 */
    __O  uint32_t FLASH7_ADDR;                        /* Address Offset: 0x0F04 */
    __O  uint32_t FLASH7_CMD;                         /* Address Offset: 0x0F08 */
    __IO uint32_t FLASH7_DATA_SYN;                    /* Address Offset: 0x0F0C */
};
/* SFC Register Structure Define */
struct SFC_REG {
    __IO uint32_t CTRL;                               /* Address Offset: 0x0000 */
    __IO uint32_t IMR;                                /* Address Offset: 0x0004 */
    __O  uint32_t ICLR;                               /* Address Offset: 0x0008 */
    __IO uint32_t FTLR;                               /* Address Offset: 0x000C */
    __IO uint32_t RCVR;                               /* Address Offset: 0x0010 */
    __IO uint32_t AX;                                 /* Address Offset: 0x0014 */
    __IO uint32_t ABIT;                               /* Address Offset: 0x0018 */
    __IO uint32_t ISR;                                /* Address Offset: 0x001C */
    __IO uint32_t FSR;                                /* Address Offset: 0x0020 */
    __IO uint32_t SR;                                 /* Address Offset: 0x0024 */
    __I  uint32_t RESERVED1;                          /* Address Offset: 0x0028 */
    __IO uint32_t VER;                                /* Address Offset: 0x002C */
         uint32_t RESERVED2[20];                      /* Address Offset: 0x0030 */
    __IO uint32_t DMATR;                              /* Address Offset: 0x0080 */
    __IO uint32_t DMAADDR;                            /* Address Offset: 0x0084 */
         uint32_t RESERVED3[30];                      /* Address Offset: 0x0088 */
    __O  uint32_t CMD;                                /* Address Offset: 0x0100 */
    __O  uint32_t ADDR;                               /* Address Offset: 0x0104 */
    __IO uint32_t DATA;                               /* Address Offset: 0x0108 */
};
/* GMAC Register Structure Define */
struct GMAC_REG {
    __IO uint32_t MAC_CONF;                           /* Address Offset: 0x0000 */
    __IO uint32_t MAC_FRM_FILT;                       /* Address Offset: 0x0004 */
    __IO uint32_t HASH_TAB_HI;                        /* Address Offset: 0x0008 */
    __IO uint32_t HASH_TAB_LO;                        /* Address Offset: 0x000C */
    __IO uint32_t GMII_ADDR;                          /* Address Offset: 0x0010 */
    __IO uint32_t GMII_DATA;                          /* Address Offset: 0x0014 */
    __IO uint32_t FLOW_CTRL;                          /* Address Offset: 0x0018 */
    __IO uint32_t VLAN_TAG;                           /* Address Offset: 0x001C */
         uint32_t RESERVED0;                          /* Address Offset: 0x0020 */
    __IO uint32_t DEBUG;                              /* Address Offset: 0x0024 */
         uint32_t RESERVED1;                          /* Address Offset: 0x0028 */
    __IO uint32_t PMT_CTRL_STA;                       /* Address Offset: 0x002C */
         uint32_t RESERVED2[2];                       /* Address Offset: 0x0030 */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0038 */
    __IO uint32_t INT_MASK;                           /* Address Offset: 0x003C */
    __IO uint32_t MAC_ADDR0_HI;                       /* Address Offset: 0x0040 */
    __IO uint32_t MAC_ADDR0_LO;                       /* Address Offset: 0x0044 */
         uint32_t RESERVED3[30];                      /* Address Offset: 0x0048 */
    __IO uint32_t AN_CTRL;                            /* Address Offset: 0x00C0 */
    __IO uint32_t AN_STATUS;                          /* Address Offset: 0x00C4 */
    __IO uint32_t AN_ADV;                             /* Address Offset: 0x00C8 */
    __I  uint32_t AN_LINK_PART_AB;                    /* Address Offset: 0x00CC */
    __I  uint32_t AN_EXP;                             /* Address Offset: 0x00D0 */
         uint32_t RESERVED4;                          /* Address Offset: 0x00D4 */
    __IO uint32_t INTF_MODE_STA;                      /* Address Offset: 0x00D8 */
         uint32_t RESERVED5[9];                       /* Address Offset: 0x00DC */
    __IO uint32_t MMC_CTRL;                           /* Address Offset: 0x0100 */
    __IO uint32_t MMC_RX_INTR;                        /* Address Offset: 0x0104 */
    __I  uint32_t MMC_TX_INTR;                        /* Address Offset: 0x0108 */
    __IO uint32_t MMC_RX_INT_MSK;                     /* Address Offset: 0x010C */
    __IO uint32_t MMC_TX_INT_MSK;                     /* Address Offset: 0x0110 */
    __IO uint32_t MMC_TXOCTETCNT_GB;                  /* Address Offset: 0x0114 */
    __IO uint32_t MMC_TXFRMCNT_GB;                    /* Address Offset: 0x0118 */
         uint32_t RESERVED6[11];                      /* Address Offset: 0x011C */
    __IO uint32_t MMC_TXUNDFLWERR;                    /* Address Offset: 0x0148 */
         uint32_t RESERVED7[5];                       /* Address Offset: 0x014C */
    __IO uint32_t MMC_TXCARERR;                       /* Address Offset: 0x0160 */
    __IO uint32_t MMC_TXOCTETCNT_G;                   /* Address Offset: 0x0164 */
    __IO uint32_t MMC_TXFRMCNT_G;                     /* Address Offset: 0x0168 */
         uint32_t RESERVED8[5];                       /* Address Offset: 0x016C */
    __IO uint32_t MMC_RXFRMCNT_GB;                    /* Address Offset: 0x0180 */
    __IO uint32_t MMC_RXOCTETCNT_GB;                  /* Address Offset: 0x0184 */
    __IO uint32_t MMC_RXOCTETCNT_G;                   /* Address Offset: 0x0188 */
         uint32_t RESERVED9;                          /* Address Offset: 0x018C */
    __IO uint32_t MMC_RXMCFRMCNT_G;                   /* Address Offset: 0x0190 */
    __IO uint32_t MMC_RXCRCERR;                       /* Address Offset: 0x0194 */
         uint32_t RESERVED10[12];                     /* Address Offset: 0x0198 */
    __IO uint32_t MMC_RXLENERR;                       /* Address Offset: 0x01C8 */
         uint32_t RESERVED11[2];                      /* Address Offset: 0x01CC */
    __IO uint32_t MMC_RXFIFOOVRFLW;                   /* Address Offset: 0x01D4 */
         uint32_t RESERVED12[10];                     /* Address Offset: 0x01D8 */
    __IO uint32_t MMC_IPC_INT_MSK;                    /* Address Offset: 0x0200 */
         uint32_t RESERVED13;                         /* Address Offset: 0x0204 */
    __I  uint32_t MMC_IPC_INTR;                       /* Address Offset: 0x0208 */
         uint32_t RESERVED14;                         /* Address Offset: 0x020C */
    __IO uint32_t MMC_RXIPV4GFRM;                     /* Address Offset: 0x0210 */
    __IO uint32_t MMC_RXIPV4HDERRFRM;                 /* Address Offset: 0x0214 */
         uint32_t RESERVED15[3];                      /* Address Offset: 0x0218 */
    __IO uint32_t MMC_RXIPV6GFRM;                     /* Address Offset: 0x0224 */
    __IO uint32_t MMC_RXIPV6HDERRFRM;                 /* Address Offset: 0x0228 */
         uint32_t RESERVED16[2];                      /* Address Offset: 0x022C */
    __IO uint32_t MMC_RXUDPERRFRM;                    /* Address Offset: 0x0234 */
         uint32_t RESERVED17;                         /* Address Offset: 0x0238 */
    __IO uint32_t MMC_RXTCPERRFRM;                    /* Address Offset: 0x023C */
         uint32_t RESERVED18;                         /* Address Offset: 0x0240 */
    __IO uint32_t MMC_RXICMPERRFRM;                   /* Address Offset: 0x0244 */
         uint32_t RESERVED19[3];                      /* Address Offset: 0x0248 */
    __IO uint32_t MMC_RXIPV4HDERROCT;                 /* Address Offset: 0x0254 */
         uint32_t RESERVED20[4];                      /* Address Offset: 0x0258 */
    __IO uint32_t MMC_RXIPV6HDERROCT;                 /* Address Offset: 0x0268 */
         uint32_t RESERVED21[2];                      /* Address Offset: 0x026C */
    __IO uint32_t MMC_RXUDPERROCT;                    /* Address Offset: 0x0274 */
         uint32_t RESERVED22;                         /* Address Offset: 0x0278 */
    __IO uint32_t MMC_RXTCPERROCT;                    /* Address Offset: 0x027C */
         uint32_t RESERVED23;                         /* Address Offset: 0x0280 */
    __IO uint32_t MMC_RXICMPERROCT;                   /* Address Offset: 0x0284 */
         uint32_t RESERVED24[862];                    /* Address Offset: 0x0288 */
    __IO uint32_t BUS_MODE;                           /* Address Offset: 0x1000 */
    __I  uint32_t TX_POLL_DEMAND;                     /* Address Offset: 0x1004 */
    __I  uint32_t RX_POLL_DEMAND;                     /* Address Offset: 0x1008 */
    __IO uint32_t RX_DESC_LIST_ADDR;                  /* Address Offset: 0x100C */
    __IO uint32_t TX_DESC_LIST_ADDR;                  /* Address Offset: 0x1010 */
    __IO uint32_t STATUS;                             /* Address Offset: 0x1014 */
    __IO uint32_t OP_MODE;                            /* Address Offset: 0x1018 */
    __IO uint32_t INT_ENA;                            /* Address Offset: 0x101C */
    __I  uint32_t OVERFLOW_CNT;                       /* Address Offset: 0x1020 */
    __IO uint32_t REC_INT_WDT_TIMER;                  /* Address Offset: 0x1024 */
    __IO uint32_t AXI_BUS_MODE;                       /* Address Offset: 0x1028 */
    __I  uint32_t AXI_STATUS;                         /* Address Offset: 0x102C */
         uint32_t RESERVED25[6];                      /* Address Offset: 0x1030 */
    __I  uint32_t CUR_HOST_TX_DESC;                   /* Address Offset: 0x1048 */
    __I  uint32_t CUR_HOST_RX_DESC;                   /* Address Offset: 0x104C */
    __I  uint32_t CUR_HOST_TX_BUF_ADDR;               /* Address Offset: 0x1050 */
    __I  uint32_t CUR_HOST_RX_BUF_ADDR;               /* Address Offset: 0x1054 */
    __IO uint32_t HW_FEA_REG;                         /* Address Offset: 0x1058 */
};
/* CEVA_GRF Register Structure Define */
struct CEVA_GRF_REG {
    __IO uint32_t GRF_DSP_CON[6];                     /* Address Offset: 0x0000 */
         uint32_t RESERVED0[2];                       /* Address Offset: 0x0018 */
    __IO uint32_t GRF_DSP_STAT[5];                    /* Address Offset: 0x0020 */
         uint32_t RESERVED1[19];                      /* Address Offset: 0x0034 */
    __IO uint32_t GRF_CEVA_EDP_PERF_CON[3];           /* Address Offset: 0x0080 */
    __I  uint32_t GRF_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM; /* Address Offset: 0x008C */
    __I  uint32_t GRF_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM; /* Address Offset: 0x0090 */
    __I  uint32_t GRF_CEVA_EDP_PERF_RD_LATENCY_ACC_SUM; /* Address Offset: 0x0094 */
    __I  uint32_t GRF_CEVA_EDP_PERF_RD_AXI_TOTAL_BYTE; /* Address Offset: 0x0098 */
    __I  uint32_t GRF_CEVA_EDP_PERF_WR_AXI_TOTAL_BYTE; /* Address Offset: 0x009C */
    __I  uint32_t GRF_CEVA_EDP_PERF_WORKING_CNT;      /* Address Offset: 0x00A0 */
         uint32_t RESERVED2[7];                       /* Address Offset: 0x00A4 */
    __IO uint32_t GRF_CEVA_EPP_PERF_CON[3];           /* Address Offset: 0x00C0 */
    __I  uint32_t GRF_CEVA_EPP_PERF_RD_MAX_LATENCY_NUM; /* Address Offset: 0x00CC */
    __I  uint32_t GRF_CEVA_EPP_PERF_RD_LATENCY_SAMP_NUM; /* Address Offset: 0x00D0 */
    __I  uint32_t GRF_CEVA_EPP_PERF_RD_LATENCY_ACC_SUM; /* Address Offset: 0x00D4 */
    __I  uint32_t GRF_CEVA_EPP_PERF_RD_AXI_TOTAL_BYTE; /* Address Offset: 0x00D8 */
    __I  uint32_t GRF_CEVA_EPP_PERF_WR_AXI_TOTAL_BYTE; /* Address Offset: 0x00DC */
    __I  uint32_t GRF_CEVA_EPP_PERF_WORKING_CNT;      /* Address Offset: 0x00E0 */
};
/* CEVA_MAILBOX Register Structure Define */
struct CEVA_MAILBOX_REG {
    __IO uint32_t A2B_INTEN;                          /* Address Offset: 0x0000 */
    __IO uint32_t A2B_STATUS;                         /* Address Offset: 0x0004 */
    __IO uint32_t A2B_CMD_0;                          /* Address Offset: 0x0008 */
    __IO uint32_t A2B_DAT_0;                          /* Address Offset: 0x000C */
    __IO uint32_t A2B_CMD_1;                          /* Address Offset: 0x0010 */
    __IO uint32_t A2B_DAT_1;                          /* Address Offset: 0x0014 */
    __IO uint32_t A2B_CMD_2;                          /* Address Offset: 0x0018 */
    __IO uint32_t A2B_DAT_2;                          /* Address Offset: 0x001C */
    __IO uint32_t A2B_CMD_3;                          /* Address Offset: 0x0020 */
    __IO uint32_t A2B_DAT_3;                          /* Address Offset: 0x0024 */
    __IO uint32_t B2A_INTEN;                          /* Address Offset: 0x0028 */
    __IO uint32_t B2A_STATUS;                         /* Address Offset: 0x002C */
    __IO uint32_t B2A_CMD_0;                          /* Address Offset: 0x0030 */
    __IO uint32_t B2A_DAT_0;                          /* Address Offset: 0x0034 */
    __IO uint32_t B2A_CMD_1;                          /* Address Offset: 0x0038 */
    __IO uint32_t B2A_DAT_1;                          /* Address Offset: 0x003C */
    __IO uint32_t B2A_CMD_2;                          /* Address Offset: 0x0040 */
    __IO uint32_t B2A_DAT_2;                          /* Address Offset: 0x0044 */
    __IO uint32_t B2A_CMD_3;                          /* Address Offset: 0x0048 */
    __IO uint32_t B2A_DAT_3;                          /* Address Offset: 0x004C */
         uint32_t RESERVED0[44];                      /* Address Offset: 0x0050 */
    __IO uint32_t ATOMIC_LOCK[32];                    /* Address Offset: 0x0100 */
};
/* CEVA_CNT Register Structure Define */
struct CEVA_CNT_REG {
    __IO uint32_t CEVA_PFM_CNT[8];                    /* Address Offset: 0x0000 */
         uint32_t RESERVED0[8];                       /* Address Offset: 0x0020 */
    __IO uint32_t CEVA_PFM_CTRL[2];                   /* Address Offset: 0x0040 */
         uint32_t RESERVED1[14];                      /* Address Offset: 0x0048 */
    __I  uint32_t CEVA_PFM_VERSION;                   /* Address Offset: 0x0080 */
};

/* UART Register Structure Define */
struct UART_REG {
    union {
        __I uint32_t RBR;                             /* Address Offset: 0x0000 */
        __O uint32_t THR;                             /* Address Offset: 0x0000 */
        __IO uint32_t DLL;                            /* Address Offset: 0x0000 */
    };

    union {
        __IO uint32_t DLH;                            /* Address Offset: 0x0004 */
        __IO uint32_t IER;                            /* Address Offset: 0x0004 */
    };

    union {
        __I  uint32_t IIR;                            /* Address Offset: 0x0008 */
        __O  uint32_t FCR;                            /* Address Offset: 0x0008 */
    };

    __IO uint32_t LCR;                                /* Address Offset: 0x000C */
    __IO uint32_t MCR;                                /* Address Offset: 0x0010 */
    __I  uint32_t LSR;                                /* Address Offset: 0x0014 */
    __I  uint32_t MSR;                                /* Address Offset: 0x0018 */
    __IO uint32_t SCR;                                /* Address Offset: 0x001C */
         uint32_t RESERVED0020[4];                    /* Address Offset: 0x0020 */

    union {
        __I  uint32_t SRBR;                           /* Address Offset: 0x0030 */
        __I  uint32_t STHR;                           /* Address Offset: 0x0030 */
    };
         uint32_t RESERVED0034[15];                   /* Address Offset: 0x0034 */

    __IO uint32_t FAR;                                /* Address Offset: 0x0070 */
    __I  uint32_t TFR;                                /* Address Offset: 0x0074 */
    __O  uint32_t RFW;                                /* Address Offset: 0x0078 */
    __I  uint32_t USR;                                /* Address Offset: 0x007C */
    __IO uint32_t TFL;                                /* Address Offset: 0x0080 */
    __I  uint32_t RFL;                                /* Address Offset: 0x0084 */
    __O  uint32_t SRR;                                /* Address Offset: 0x0088 */
    __IO uint32_t SRTS;                               /* Address Offset: 0x008C */
    __IO uint32_t SBCR;                               /* Address Offset: 0x0090 */
    __IO uint32_t SDMAM;                              /* Address Offset: 0x0094 */
    __IO uint32_t SFE;                                /* Address Offset: 0x0098 */
    __IO uint32_t SRT;                                /* Address Offset: 0x009C */
    __IO uint32_t STET;                               /* Address Offset: 0x00A0 */
    __IO uint32_t HTX;                                /* Address Offset: 0x00A4 */
    __O  uint32_t DMASA;                              /* Address Offset: 0x00A8 */
         uint32_t RESERVED00AC[18];                   /* Address Offset: 0x00AC */
    __I  uint32_t CPR;                                /* Address Offset: 0x00F4 */
    __I  uint32_t UCV;                                /* Address Offset: 0x00F8 */
    __I  uint32_t CTR;                                /* Address Offset: 0x00FC */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __O  uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
};
#endif /* __ASSEMBLY__ */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define I2S0_BASE           0x10110000U /* I2S0 base address */
#define I2S1_BASE           0x10120000U /* I2S1 base address */
#define I2S2_BASE           0x10130000U /* I2S2 base address */
#define CRYPTO_BASE         0x10140000U /* CRYPTO base address */
#define UART0_BASE          0x10230000U /* UART0 base address */
#define UART2_BASE          0x10210000U /* UART2 base address */
#define SPI_BASE            0x10270000U /* SPI base address */
#define PWM_BASE            0x10280000U /* PWM base address */
#define GRF_BASE            0x10300000U /* GRF base address */
#define GPIO1_BASE          0x10310000U /* GPIO1 base address */
#define GPIO2_BASE          0x10320000U /* GPIO2 base address */
#define GPIO3_BASE          0x10330000U /* GPIO3 base address */
#define TIMER_BASE          0x10350000U /* TIMER base address */
#define PMU_BASE            0x20010000U /* PMU base address */
#define GPIO0_BASE          0x20030000U /* GPIO0 base address */
#define PMU_GRF_BASE        0x20060000U /* PMU_GRF base address */
#define CRU_BASE            0x20200000U /* CRU base address */
#define DDR_PHY_BASE        0x20210000U /* DDR_PHY base address */
#define DDR_PCTL_BASE       0x20260000U /* DDR_PCTL base address */
#define USB_GRF_BASE        0x202A0000U /* USB_GRF base address */
#define DDR_MON_BASE        0x202C0000U /* DDR_MON base address */
#define RKVENC_BASE         0x28000000U /* RKVENC base address */
#define RKVDEC_BASE         0x30010000U /* RKVDEC base address */
#define IEP_BASE            0x30030000U /* IEP base address */
#define RGA_BASE            0x30050000U /* RGA base address */
#define NANDC_BASE          0x30100000U /* NANDC base address */
#define SFC_BASE            0x301C0000U /* SFC base address */
#define GMAC_BASE           0x30200000U /* GMAC base address */
#define GIC_BASE            0x32010000U /* GIC base address */
#define CEVA_GRF_BASE       0x33800000U /* CEVA_GRF base address */
#define CEVA_MAILBOX_BASE   0x33810000U /* CEVA_MAILBOX base address */
#define CEVA_CNT_BASE       0x33820000U /* CEVA_CNT base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define I2S0                ((struct I2S_REG *) I2S0_BASE)
#define I2S1                ((struct I2S_REG *) I2S1_BASE)
#define I2S2                ((struct I2S_REG *) I2S2_BASE)
#define CRYPTO              ((struct CRYPTO_REG *) CRYPTO_BASE)
#define SPI                 ((struct SPI_REG *) SPI_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define GPIO2               ((struct GPIO_REG *) GPIO2_BASE)
#define GPIO3               ((struct GPIO_REG *) GPIO3_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER_BASE)
#define TIMER1              ((struct TIMER_REG *) (TIMER_BASE + 0x20))
#define PWM                 ((struct PWM_REG *) PWM_BASE)
#define GRF                 ((struct GRF_REG *) GRF_BASE)
#define PMU                 ((struct PMU_REG *) PMU_BASE)
#define PMU_GRF             ((struct PMU_GRF_REG *) PMU_GRF_BASE)
#define CRU                 ((struct CRU_REG *) CRU_BASE)
#define DDR_PHY             ((struct DDR_PHY_REG *) DDR_PHY_BASE)
#define DDR_PCTL            ((struct DDR_PCTL_REG *) DDR_PCTL_BASE)
#define USB_GRF             ((struct USB_GRF_REG *) USB_GRF_BASE)
#define DDR_MON             ((struct DDR_MON_REG *) DDR_MON_BASE)
#define RKVENC              ((struct RKVENC_REG *) RKVENC_BASE)
#define RKVDEC              ((struct RKVDEC_REG *) RKVDEC_BASE)
#define IEP                 ((struct IEP_REG *) IEP_BASE)
#define RGA                 ((struct RGA_REG *) RGA_BASE)
#define NANDC               ((struct NANDC_REG *) NANDC_BASE)
#define SFC                 ((struct SFC_REG *) SFC_BASE)
#define GMAC                ((struct GMAC_REG *) GMAC_BASE)
#define CEVA_GRF            ((struct CEVA_GRF_REG *) CEVA_GRF_BASE)
#define CEVA_MAILBOX        ((struct CEVA_MAILBOX_REG *) CEVA_MAILBOX_BASE)
#define CEVA_CNT            ((struct CEVA_CNT_REG *) CEVA_CNT_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)

#define IS_CRYPTO_INSTANCE(instance) ((instance) == CRYPTO)
#define IS_SPI_INSTANCE(instance) ((instance) == SPI)
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1))
#define IS_PWM_INSTANCE(instance) ((instance) == PWM)
#define IS_GRF_INSTANCE(instance) ((instance) == GRF)
#define IS_PMU_INSTANCE(instance) ((instance) == PMU)
#define IS_PMU_GRF_INSTANCE(instance) ((instance) == PMU_GRF)
#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_DDR_PHY_INSTANCE(instance) ((instance) == DDR_PHY)
#define IS_DDR_PCTL_INSTANCE(instance) ((instance) == DDR_PCTL)
#define IS_USB_GRF_INSTANCE(instance) ((instance) == USB_GRF)
#define IS_DDR_MON_INSTANCE(instance) ((instance) == DDR_MON)
#define IS_RKVENC_INSTANCE(instance) ((instance) == RKVENC)
#define IS_RKVDEC_INSTANCE(instance) ((instance) == RKVDEC)
#define IS_IEP_INSTANCE(instance) ((instance) == IEP)
#define IS_RGA_INSTANCE(instance) ((instance) == RGA)
#define IS_NANDC_INSTANCE(instance) ((instance) == NANDC)
#define IS_SFC_INSTANCE(instance) ((instance) == SFC)
#define IS_GMAC_INSTANCE(instance) ((instance) == GMAC)
#define IS_CEVA_GRF_INSTANCE(instance) ((instance) == CEVA_GRF)
#define IS_CEVA_MAILBOX_INSTANCE(instance) ((instance) == CEVA_MAILBOX)
#define IS_CEVA_CNT_INSTANCE(instance) ((instance) == CEVA_CNT)
#define IS_I2S_INSTANCE(instance) (((instance) == I2S0) || ((instance) == I2S1) || ((instance) == I2S2))
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART2))
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO0) || ((instance) == GPIO1) || ((instance) == GPIO2) || ((instance) == GPIO3))
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************I2S*******************************************/
/* TXCR */
#define I2S_TXCR_OFFSET                                    (0x0)
#define I2S_TXCR_VDW_SHIFT                                 (0U)
#define I2S_TXCR_VDW_MASK                                  (0x1FU << I2S_TXCR_VDW_SHIFT)                                /* 0x0000001F */
#define I2S_TXCR_TFS_SHIFT                                 (5U)
#define I2S_TXCR_TFS_MASK                                  (0x1U << I2S_TXCR_TFS_SHIFT)                                 /* 0x00000020 */
#define I2S_TXCR_PBM_SHIFT                                 (7U)
#define I2S_TXCR_PBM_MASK                                  (0x3U << I2S_TXCR_PBM_SHIFT)                                 /* 0x00000180 */
#define I2S_TXCR_IBM_SHIFT                                 (9U)
#define I2S_TXCR_IBM_MASK                                  (0x3U << I2S_TXCR_IBM_SHIFT)                                 /* 0x00000600 */
#define I2S_TXCR_FBM_SHIFT                                 (11U)
#define I2S_TXCR_FBM_MASK                                  (0x1U << I2S_TXCR_FBM_SHIFT)                                 /* 0x00000800 */
#define I2S_TXCR_SJM_SHIFT                                 (12U)
#define I2S_TXCR_SJM_MASK                                  (0x1U << I2S_TXCR_SJM_SHIFT)                                 /* 0x00001000 */
#define I2S_TXCR_HWT_SHIFT                                 (14U)
#define I2S_TXCR_HWT_MASK                                  (0x1U << I2S_TXCR_HWT_SHIFT)                                 /* 0x00004000 */
#define I2S_TXCR_TCSR_SHIFT                                (15U)
#define I2S_TXCR_TCSR_MASK                                 (0x3U << I2S_TXCR_TCSR_SHIFT)                                /* 0x00018000 */
#define I2S_TXCR_RCNT_SHIFT                                (17U)
#define I2S_TXCR_RCNT_MASK                                 (0x3FU << I2S_TXCR_RCNT_SHIFT)                               /* 0x007E0000 */
/* RXCR */
#define I2S_RXCR_OFFSET                                    (0x4)
#define I2S_RXCR_VDW_SHIFT                                 (0U)
#define I2S_RXCR_VDW_MASK                                  (0x1FU << I2S_RXCR_VDW_SHIFT)                                /* 0x0000001F */
#define I2S_RXCR_TFS_SHIFT                                 (5U)
#define I2S_RXCR_TFS_MASK                                  (0x1U << I2S_RXCR_TFS_SHIFT)                                 /* 0x00000020 */
#define I2S_RXCR_PBM_SHIFT                                 (7U)
#define I2S_RXCR_PBM_MASK                                  (0x3U << I2S_RXCR_PBM_SHIFT)                                 /* 0x00000180 */
#define I2S_RXCR_IBM_SHIFT                                 (9U)
#define I2S_RXCR_IBM_MASK                                  (0x3U << I2S_RXCR_IBM_SHIFT)                                 /* 0x00000600 */
#define I2S_RXCR_FBM_SHIFT                                 (11U)
#define I2S_RXCR_FBM_MASK                                  (0x1U << I2S_RXCR_FBM_SHIFT)                                 /* 0x00000800 */
#define I2S_RXCR_SJM_SHIFT                                 (12U)
#define I2S_RXCR_SJM_MASK                                  (0x1U << I2S_RXCR_SJM_SHIFT)                                 /* 0x00001000 */
#define I2S_RXCR_HWT_SHIFT                                 (14U)
#define I2S_RXCR_HWT_MASK                                  (0x1U << I2S_RXCR_HWT_SHIFT)                                 /* 0x00004000 */
#define I2S_RXCR_RCSR_SHIFT                                (15U)
#define I2S_RXCR_RCSR_MASK                                 (0x3U << I2S_RXCR_RCSR_SHIFT)                                /* 0x00018000 */
/* CKR */
#define I2S_CKR_OFFSET                                     (0x8)
#define I2S_CKR_TSD_SHIFT                                  (0U)
#define I2S_CKR_TSD_MASK                                   (0xFFU << I2S_CKR_TSD_SHIFT)                                 /* 0x000000FF */
#define I2S_CKR_RSD_SHIFT                                  (8U)
#define I2S_CKR_RSD_MASK                                   (0xFFU << I2S_CKR_RSD_SHIFT)                                 /* 0x0000FF00 */
#define I2S_CKR_MDIV_SHIFT                                 (16U)
#define I2S_CKR_MDIV_MASK                                  (0xFFU << I2S_CKR_MDIV_SHIFT)                                /* 0x00FF0000 */
#define I2S_CKR_TLP_SHIFT                                  (24U)
#define I2S_CKR_TLP_MASK                                   (0x1U << I2S_CKR_TLP_SHIFT)                                  /* 0x01000000 */
#define I2S_CKR_RLP_SHIFT                                  (25U)
#define I2S_CKR_RLP_MASK                                   (0x1U << I2S_CKR_RLP_SHIFT)                                  /* 0x02000000 */
#define I2S_CKR_CKP_SHIFT                                  (26U)
#define I2S_CKR_CKP_MASK                                   (0x1U << I2S_CKR_CKP_SHIFT)                                  /* 0x04000000 */
#define I2S_CKR_MSS_SHIFT                                  (27U)
#define I2S_CKR_MSS_MASK                                   (0x1U << I2S_CKR_MSS_SHIFT)                                  /* 0x08000000 */
#define I2S_CKR_TRCM_SHIFT                                 (28U)
#define I2S_CKR_TRCM_MASK                                  (0x3U << I2S_CKR_TRCM_SHIFT)                                 /* 0x30000000 */
/* TXFIFOLR */
#define I2S_TXFIFOLR_OFFSET                                (0xC)
#define I2S_TXFIFOLR_TFL0_SHIFT                            (0U)
#define I2S_TXFIFOLR_TFL0_MASK                             (0x3FU << I2S_TXFIFOLR_TFL0_SHIFT)                           /* 0x0000003F */
#define I2S_TXFIFOLR_TFL1_SHIFT                            (6U)
#define I2S_TXFIFOLR_TFL1_MASK                             (0x3FU << I2S_TXFIFOLR_TFL1_SHIFT)                           /* 0x00000FC0 */
#define I2S_TXFIFOLR_TFL2_SHIFT                            (12U)
#define I2S_TXFIFOLR_TFL2_MASK                             (0x3FU << I2S_TXFIFOLR_TFL2_SHIFT)                           /* 0x0003F000 */
#define I2S_TXFIFOLR_TFL3_SHIFT                            (18U)
#define I2S_TXFIFOLR_TFL3_MASK                             (0x3FU << I2S_TXFIFOLR_TFL3_SHIFT)                           /* 0x00FC0000 */
/* DMACR */
#define I2S_DMACR_OFFSET                                   (0x10)
#define I2S_DMACR_TDL_SHIFT                                (0U)
#define I2S_DMACR_TDL_MASK                                 (0x1FU << I2S_DMACR_TDL_SHIFT)                               /* 0x0000001F */
#define I2S_DMACR_TDE_SHIFT                                (8U)
#define I2S_DMACR_TDE_MASK                                 (0x1U << I2S_DMACR_TDE_SHIFT)                                /* 0x00000100 */
#define I2S_DMACR_RDL_SHIFT                                (16U)
#define I2S_DMACR_RDL_MASK                                 (0x1FU << I2S_DMACR_RDL_SHIFT)                               /* 0x001F0000 */
#define I2S_DMACR_RDE_SHIFT                                (24U)
#define I2S_DMACR_RDE_MASK                                 (0x1U << I2S_DMACR_RDE_SHIFT)                                /* 0x01000000 */
/* INTCR */
#define I2S_INTCR_OFFSET                                   (0x14)
#define I2S_INTCR_TXEIE_SHIFT                              (0U)
#define I2S_INTCR_TXEIE_MASK                               (0x1U << I2S_INTCR_TXEIE_SHIFT)                              /* 0x00000001 */
#define I2S_INTCR_TXUIE_SHIFT                              (1U)
#define I2S_INTCR_TXUIE_MASK                               (0x1U << I2S_INTCR_TXUIE_SHIFT)                              /* 0x00000002 */
#define I2S_INTCR_TXUIC_SHIFT                              (2U)
#define I2S_INTCR_TXUIC_MASK                               (0x1U << I2S_INTCR_TXUIC_SHIFT)                              /* 0x00000004 */
#define I2S_INTCR_TFT_SHIFT                                (4U)
#define I2S_INTCR_TFT_MASK                                 (0x1FU << I2S_INTCR_TFT_SHIFT)                               /* 0x000001F0 */
#define I2S_INTCR_RXFIE_SHIFT                              (16U)
#define I2S_INTCR_RXFIE_MASK                               (0x1U << I2S_INTCR_RXFIE_SHIFT)                              /* 0x00010000 */
#define I2S_INTCR_RXOIE_SHIFT                              (17U)
#define I2S_INTCR_RXOIE_MASK                               (0x1U << I2S_INTCR_RXOIE_SHIFT)                              /* 0x00020000 */
#define I2S_INTCR_RXOIC_SHIFT                              (18U)
#define I2S_INTCR_RXOIC_MASK                               (0x1U << I2S_INTCR_RXOIC_SHIFT)                              /* 0x00040000 */
#define I2S_INTCR_RFT_SHIFT                                (20U)
#define I2S_INTCR_RFT_MASK                                 (0x1FU << I2S_INTCR_RFT_SHIFT)                               /* 0x01F00000 */
/* INTSR */
#define I2S_INTSR_OFFSET                                   (0x18)
#define I2S_INTSR_TXEI_SHIFT                               (0U)
#define I2S_INTSR_TXEI_MASK                                (0x1U << I2S_INTSR_TXEI_SHIFT)                               /* 0x00000001 */
#define I2S_INTSR_TXUI_SHIFT                               (1U)
#define I2S_INTSR_TXUI_MASK                                (0x1U << I2S_INTSR_TXUI_SHIFT)                               /* 0x00000002 */
#define I2S_INTSR_RXFI_SHIFT                               (16U)
#define I2S_INTSR_RXFI_MASK                                (0x1U << I2S_INTSR_RXFI_SHIFT)                               /* 0x00010000 */
#define I2S_INTSR_RXOI_SHIFT                               (17U)
#define I2S_INTSR_RXOI_MASK                                (0x1U << I2S_INTSR_RXOI_SHIFT)                               /* 0x00020000 */
/* XFER */
#define I2S_XFER_OFFSET                                    (0x1C)
#define I2S_XFER_TXS_SHIFT                                 (0U)
#define I2S_XFER_TXS_MASK                                  (0x1U << I2S_XFER_TXS_SHIFT)                                 /* 0x00000001 */
#define I2S_XFER_RXS_SHIFT                                 (1U)
#define I2S_XFER_RXS_MASK                                  (0x1U << I2S_XFER_RXS_SHIFT)                                 /* 0x00000002 */
/* CLR */
#define I2S_CLR_OFFSET                                     (0x20)
#define I2S_CLR_TXC_SHIFT                                  (0U)
#define I2S_CLR_TXC_MASK                                   (0x1U << I2S_CLR_TXC_SHIFT)                                  /* 0x00000001 */
#define I2S_CLR_RXC_SHIFT                                  (1U)
#define I2S_CLR_RXC_MASK                                   (0x1U << I2S_CLR_RXC_SHIFT)                                  /* 0x00000002 */
/* TXDR */
#define I2S_TXDR_OFFSET                                    (0x24)
#define I2S_TXDR_TXDR_SHIFT                                (0U)
#define I2S_TXDR_TXDR_MASK                                 (0xFFFFFFFFU << I2S_TXDR_TXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXDR */
#define I2S_RXDR_OFFSET                                    (0x28)
#define I2S_RXDR_RXDR_SHIFT                                (0U)
#define I2S_RXDR_RXDR_MASK                                 (0xFFFFFFFFU << I2S_RXDR_RXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXFIFOLR */
#define I2S_RXFIFOLR_OFFSET                                (0x2C)
#define I2S_RXFIFOLR_RFL0_SHIFT                            (0U)
#define I2S_RXFIFOLR_RFL0_MASK                             (0x3FU << I2S_RXFIFOLR_RFL0_SHIFT)                           /* 0x0000003F */
#define I2S_RXFIFOLR_RFL1_SHIFT                            (6U)
#define I2S_RXFIFOLR_RFL1_MASK                             (0x3FU << I2S_RXFIFOLR_RFL1_SHIFT)                           /* 0x00000FC0 */
#define I2S_RXFIFOLR_RFL2_SHIFT                            (12U)
#define I2S_RXFIFOLR_RFL2_MASK                             (0x3FU << I2S_RXFIFOLR_RFL2_SHIFT)                           /* 0x0003F000 */
#define I2S_RXFIFOLR_RFL3_SHIFT                            (18U)
#define I2S_RXFIFOLR_RFL3_MASK                             (0x3FU << I2S_RXFIFOLR_RFL3_SHIFT)                           /* 0x00FC0000 */
/*****************************************CRYPTO*****************************************/
/* INTSTS */
#define CRYPTO_INTSTS_BCDMA_DONE_INT_SHIFT                 (0U)
#define CRYPTO_INTSTS_BCDMA_DONE_INT_MASK                  (0x1U << CRYPTO_INTSTS_BCDMA_DONE_INT_SHIFT)                 /* 0x00000001 */
#define CRYPTO_INTSTS_BCDMA_ERR_INT_SHIFT                  (1U)
#define CRYPTO_INTSTS_BCDMA_ERR_INT_MASK                   (0x1U << CRYPTO_INTSTS_BCDMA_ERR_INT_SHIFT)                  /* 0x00000002 */
#define CRYPTO_INTSTS_HRDMA_DONE_INT_SHIFT                 (2U)
#define CRYPTO_INTSTS_HRDMA_DONE_INT_MASK                  (0x1U << CRYPTO_INTSTS_HRDMA_DONE_INT_SHIFT)                 /* 0x00000004 */
#define CRYPTO_INTSTS_HRDMA_ERR_INT_SHIFT                  (3U)
#define CRYPTO_INTSTS_HRDMA_ERR_INT_MASK                   (0x1U << CRYPTO_INTSTS_HRDMA_ERR_INT_SHIFT)                  /* 0x00000008 */
#define CRYPTO_INTSTS_HASH_DONE_INT_SHIFT                  (4U)
#define CRYPTO_INTSTS_HASH_DONE_INT_MASK                   (0x1U << CRYPTO_INTSTS_HASH_DONE_INT_SHIFT)                  /* 0x00000010 */
#define CRYPTO_INTSTS_PKA_DONE_INT_SHIFT                   (5U)
#define CRYPTO_INTSTS_PKA_DONE_INT_MASK                    (0x1U << CRYPTO_INTSTS_PKA_DONE_INT_SHIFT)                   /* 0x00000020 */
/* INTENA */
#define CRYPTO_INTENA_BCDMA_DONE_ENA_SHIFT                 (0U)
#define CRYPTO_INTENA_BCDMA_DONE_ENA_MASK                  (0x1U << CRYPTO_INTENA_BCDMA_DONE_ENA_SHIFT)                 /* 0x00000001 */
#define CRYPTO_INTENA_BCDMA_ERR_ENA_SHIFT                  (1U)
#define CRYPTO_INTENA_BCDMA_ERR_ENA_MASK                   (0x1U << CRYPTO_INTENA_BCDMA_ERR_ENA_SHIFT)                  /* 0x00000002 */
#define CRYPTO_INTENA_HRDMA_DONE_ENA_SHIFT                 (2U)
#define CRYPTO_INTENA_HRDMA_DONE_ENA_MASK                  (0x1U << CRYPTO_INTENA_HRDMA_DONE_ENA_SHIFT)                 /* 0x00000004 */
#define CRYPTO_INTENA_HRDMA_ERR_ENA_SHIFT                  (3U)
#define CRYPTO_INTENA_HRDMA_ERR_ENA_MASK                   (0x1U << CRYPTO_INTENA_HRDMA_ERR_ENA_SHIFT)                  /* 0x00000008 */
#define CRYPTO_INTENA_HASH_DONE_ENA_SHIFT                  (4U)
#define CRYPTO_INTENA_HASH_DONE_ENA_MASK                   (0x1U << CRYPTO_INTENA_HASH_DONE_ENA_SHIFT)                  /* 0x00000010 */
#define CRYPTO_INTENA_PKA_DONE_ENA_SHIFT                   (5U)
#define CRYPTO_INTENA_PKA_DONE_ENA_MASK                    (0x1U << CRYPTO_INTENA_PKA_DONE_ENA_SHIFT)                   /* 0x00000020 */
/* CTRL */
#define CRYPTO_CTRL_AES_START_SHIFT                        (0U)
#define CRYPTO_CTRL_AES_START_MASK                         (0x1U << CRYPTO_CTRL_AES_START_SHIFT)                        /* 0x00000001 */
#define CRYPTO_CTRL_TDES_START_SHIFT                       (1U)
#define CRYPTO_CTRL_TDES_START_MASK                        (0x1U << CRYPTO_CTRL_TDES_START_SHIFT)                       /* 0x00000002 */
#define CRYPTO_CTRL_BLOCK_START_SHIFT                      (2U)
#define CRYPTO_CTRL_BLOCK_START_MASK                       (0x1U << CRYPTO_CTRL_BLOCK_START_SHIFT)                      /* 0x00000004 */
#define CRYPTO_CTRL_HASH_START_SHIFT                       (3U)
#define CRYPTO_CTRL_HASH_START_MASK                        (0x1U << CRYPTO_CTRL_HASH_START_SHIFT)                       /* 0x00000008 */
#define CRYPTO_CTRL_PKA_START_SHIFT                        (4U)
#define CRYPTO_CTRL_PKA_START_MASK                         (0x1U << CRYPTO_CTRL_PKA_START_SHIFT)                        /* 0x00000010 */
#define CRYPTO_CTRL_BLOCK_FLUSH_SHIFT                      (5U)
#define CRYPTO_CTRL_BLOCK_FLUSH_MASK                       (0x1U << CRYPTO_CTRL_BLOCK_FLUSH_SHIFT)                      /* 0x00000020 */
#define CRYPTO_CTRL_HASH_FLUSH_SHIFT                       (6U)
#define CRYPTO_CTRL_HASH_FLUSH_MASK                        (0x1U << CRYPTO_CTRL_HASH_FLUSH_SHIFT)                       /* 0x00000040 */
#define CRYPTO_CTRL_PKA_FLUSH_SHIFT                        (7U)
#define CRYPTO_CTRL_PKA_FLUSH_MASK                         (0x1U << CRYPTO_CTRL_PKA_FLUSH_SHIFT)                        /* 0x00000080 */
/* CONF */
#define CRYPTO_CONF_HASHINSEL_SHIFT                        (0U)
#define CRYPTO_CONF_HASHINSEL_MASK                         (0x3U << CRYPTO_CONF_HASHINSEL_SHIFT)                        /* 0x00000003 */
#define CRYPTO_CONF_DESSEL_SHIFT                           (2U)
#define CRYPTO_CONF_DESSEL_MASK                            (0x1U << CRYPTO_CONF_DESSEL_SHIFT)                           /* 0x00000004 */
#define CRYPTO_CONF_BYTESWAP_BRFIFO_SHIFT                  (3U)
#define CRYPTO_CONF_BYTESWAP_BRFIFO_MASK                   (0x1U << CRYPTO_CONF_BYTESWAP_BRFIFO_SHIFT)                  /* 0x00000008 */
#define CRYPTO_CONF_BYTESWAP_BTFIFO_SHIFT                  (4U)
#define CRYPTO_CONF_BYTESWAP_BTFIFO_MASK                   (0x1U << CRYPTO_CONF_BYTESWAP_BTFIFO_SHIFT)                  /* 0x00000010 */
#define CRYPTO_CONF_BYTESWAP_HRFIFO_SHIFT                  (5U)
#define CRYPTO_CONF_BYTESWAP_HRFIFO_MASK                   (0x1U << CRYPTO_CONF_BYTESWAP_HRFIFO_SHIFT)                  /* 0x00000020 */
/* BRDMAS */
#define CRYPTO_BRDMAS_STARTADDR_SHIFT                      (0U)
#define CRYPTO_BRDMAS_STARTADDR_MASK                       (0xFFFFFFFFU << CRYPTO_BRDMAS_STARTADDR_SHIFT)               /* 0xFFFFFFFF */
/* BTDMAS */
#define CRYPTO_BTDMAS_STARTADDR_SHIFT                      (0U)
#define CRYPTO_BTDMAS_STARTADDR_MASK                       (0xFFFFFFFFU << CRYPTO_BTDMAS_STARTADDR_SHIFT)               /* 0xFFFFFFFF */
/* BRDMAL */
#define CRYPTO_BRDMAL_LENGTH_SHIFT                         (0U)
#define CRYPTO_BRDMAL_LENGTH_MASK                          (0xFFFFFFFFU << CRYPTO_BRDMAL_LENGTH_SHIFT)                  /* 0xFFFFFFFF */
/* HRDMAS */
#define CRYPTO_HRDMAS_STARTADDR_SHIFT                      (0U)
#define CRYPTO_HRDMAS_STARTADDR_MASK                       (0xFFFFFFFFU << CRYPTO_HRDMAS_STARTADDR_SHIFT)               /* 0xFFFFFFFF */
/* HRDMAL */
#define CRYPTO_HRDMAL_LENGTH_SHIFT                         (0U)
#define CRYPTO_HRDMAL_LENGTH_MASK                          (0xFFFFFFFFU << CRYPTO_HRDMAL_LENGTH_SHIFT)                  /* 0xFFFFFFFF */
/* AES_CTRL */
#define CRYPTO_AES_CTRL_AES_ENC_SHIFT                      (0U)
#define CRYPTO_AES_CTRL_AES_ENC_MASK                       (0x1U << CRYPTO_AES_CTRL_AES_ENC_SHIFT)                      /* 0x00000001 */
#define CRYPTO_AES_CTRL_AES_FIFOMODE_SHIFT                 (1U)
#define CRYPTO_AES_CTRL_AES_FIFOMODE_MASK                  (0x1U << CRYPTO_AES_CTRL_AES_FIFOMODE_SHIFT)                 /* 0x00000002 */
#define CRYPTO_AES_CTRL_AES_KEYSIZE_SHIFT                  (2U)
#define CRYPTO_AES_CTRL_AES_KEYSIZE_MASK                   (0x3U << CRYPTO_AES_CTRL_AES_KEYSIZE_SHIFT)                  /* 0x0000000C */
#define CRYPTO_AES_CTRL_AES_CHAINMODE_SHIFT                (4U)
#define CRYPTO_AES_CTRL_AES_CHAINMODE_MASK                 (0x3U << CRYPTO_AES_CTRL_AES_CHAINMODE_SHIFT)                /* 0x00000030 */
#define CRYPTO_AES_CTRL_AES_KEYCHANGE_SHIFT                (6U)
#define CRYPTO_AES_CTRL_AES_KEYCHANGE_MASK                 (0x1U << CRYPTO_AES_CTRL_AES_KEYCHANGE_SHIFT)                /* 0x00000040 */
#define CRYPTO_AES_CTRL_AES_BYTESWAP_DI_SHIFT              (7U)
#define CRYPTO_AES_CTRL_AES_BYTESWAP_DI_MASK               (0x1U << CRYPTO_AES_CTRL_AES_BYTESWAP_DI_SHIFT)              /* 0x00000080 */
#define CRYPTO_AES_CTRL_AES_BYTESWAP_DO_SHIFT              (8U)
#define CRYPTO_AES_CTRL_AES_BYTESWAP_DO_MASK               (0x1U << CRYPTO_AES_CTRL_AES_BYTESWAP_DO_SHIFT)              /* 0x00000100 */
#define CRYPTO_AES_CTRL_AES_BYTESWAP_IV_SHIFT              (9U)
#define CRYPTO_AES_CTRL_AES_BYTESWAP_IV_MASK               (0x1U << CRYPTO_AES_CTRL_AES_BYTESWAP_IV_SHIFT)              /* 0x00000200 */
#define CRYPTO_AES_CTRL_AES_BYTESWAP_KEY_SHIFT             (10U)
#define CRYPTO_AES_CTRL_AES_BYTESWAP_KEY_MASK              (0x1U << CRYPTO_AES_CTRL_AES_BYTESWAP_KEY_SHIFT)             /* 0x00000400 */
#define CRYPTO_AES_CTRL_AES_BYTESWAP_CNT_SHIFT             (11U)
#define CRYPTO_AES_CTRL_AES_BYTESWAP_CNT_MASK              (0x1U << CRYPTO_AES_CTRL_AES_BYTESWAP_CNT_SHIFT)             /* 0x00000800 */
/* AES_STS */
#define CRYPTO_AES_STS_AES_DONE_SHIFT                      (0U)
#define CRYPTO_AES_STS_AES_DONE_MASK                       (0x1U << CRYPTO_AES_STS_AES_DONE_SHIFT)                      /* 0x00000001 */
/* AES_DIN_0 */
#define CRYPTO_AES_DIN_0_AES_DIN_0_SHIFT                   (0U)
#define CRYPTO_AES_DIN_0_AES_DIN_0_MASK                    (0xFFFFFFFFU << CRYPTO_AES_DIN_0_AES_DIN_0_SHIFT)            /* 0xFFFFFFFF */
/* AES_DIN_1 */
#define CRYPTO_AES_DIN_1_AES_DIN_1_SHIFT                   (0U)
#define CRYPTO_AES_DIN_1_AES_DIN_1_MASK                    (0xFFFFFFFFU << CRYPTO_AES_DIN_1_AES_DIN_1_SHIFT)            /* 0xFFFFFFFF */
/* AES_DIN_2 */
#define CRYPTO_AES_DIN_2_AES_DIN_2_SHIFT                   (0U)
#define CRYPTO_AES_DIN_2_AES_DIN_2_MASK                    (0xFFFFFFFFU << CRYPTO_AES_DIN_2_AES_DIN_2_SHIFT)            /* 0xFFFFFFFF */
/* AES_DIN_3 */
#define CRYPTO_AES_DIN_3_AES_DIN_3_SHIFT                   (0U)
#define CRYPTO_AES_DIN_3_AES_DIN_3_MASK                    (0xFFFFFFFFU << CRYPTO_AES_DIN_3_AES_DIN_3_SHIFT)            /* 0xFFFFFFFF */
/* AES_DOUT_0 */
#define CRYPTO_AES_DOUT_0_AES_DOUT_0_SHIFT                 (0U)
#define CRYPTO_AES_DOUT_0_AES_DOUT_0_MASK                  (0xFFFFFFFFU << CRYPTO_AES_DOUT_0_AES_DOUT_0_SHIFT)          /* 0xFFFFFFFF */
/* AES_DOUT_1 */
#define CRYPTO_AES_DOUT_1_AES_DOUT_1_SHIFT                 (0U)
#define CRYPTO_AES_DOUT_1_AES_DOUT_1_MASK                  (0xFFFFFFFFU << CRYPTO_AES_DOUT_1_AES_DOUT_1_SHIFT)          /* 0xFFFFFFFF */
/* AES_DOUT_2 */
#define CRYPTO_AES_DOUT_2_AES_DOUT_2_SHIFT                 (0U)
#define CRYPTO_AES_DOUT_2_AES_DOUT_2_MASK                  (0xFFFFFFFFU << CRYPTO_AES_DOUT_2_AES_DOUT_2_SHIFT)          /* 0xFFFFFFFF */
/* AES_DOUT_3 */
#define CRYPTO_AES_DOUT_3_AES_DOUT_3_SHIFT                 (0U)
#define CRYPTO_AES_DOUT_3_AES_DOUT_3_MASK                  (0xFFFFFFFFU << CRYPTO_AES_DOUT_3_AES_DOUT_3_SHIFT)          /* 0xFFFFFFFF */
/* AES_IV_0 */
#define CRYPTO_AES_IV_0_AES_IV_0_SHIFT                     (0U)
#define CRYPTO_AES_IV_0_AES_IV_0_MASK                      (0xFFFFFFFFU << CRYPTO_AES_IV_0_AES_IV_0_SHIFT)              /* 0xFFFFFFFF */
/* AES_IV_1 */
#define CRYPTO_AES_IV_1_AES_IV_1_SHIFT                     (0U)
#define CRYPTO_AES_IV_1_AES_IV_1_MASK                      (0xFFFFFFFFU << CRYPTO_AES_IV_1_AES_IV_1_SHIFT)              /* 0xFFFFFFFF */
/* AES_IV_2 */
#define CRYPTO_AES_IV_2_AES_IV_2_SHIFT                     (0U)
#define CRYPTO_AES_IV_2_AES_IV_2_MASK                      (0xFFFFFFFFU << CRYPTO_AES_IV_2_AES_IV_2_SHIFT)              /* 0xFFFFFFFF */
/* AES_IV_3 */
#define CRYPTO_AES_IV_3_AES_IV_3_SHIFT                     (0U)
#define CRYPTO_AES_IV_3_AES_IV_3_MASK                      (0xFFFFFFFFU << CRYPTO_AES_IV_3_AES_IV_3_SHIFT)              /* 0xFFFFFFFF */
/* AES_KEY_0 */
#define CRYPTO_AES_KEY_0_AES_KEY_0_SHIFT                   (0U)
#define CRYPTO_AES_KEY_0_AES_KEY_0_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_0_AES_KEY_0_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_1 */
#define CRYPTO_AES_KEY_1_AES_KEY_1_SHIFT                   (0U)
#define CRYPTO_AES_KEY_1_AES_KEY_1_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_1_AES_KEY_1_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_2 */
#define CRYPTO_AES_KEY_2_AES_KEY_2_SHIFT                   (0U)
#define CRYPTO_AES_KEY_2_AES_KEY_2_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_2_AES_KEY_2_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_3 */
#define CRYPTO_AES_KEY_3_AES_KEY_3_SHIFT                   (0U)
#define CRYPTO_AES_KEY_3_AES_KEY_3_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_3_AES_KEY_3_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_4 */
#define CRYPTO_AES_KEY_4_AES_KEY_4_SHIFT                   (0U)
#define CRYPTO_AES_KEY_4_AES_KEY_4_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_4_AES_KEY_4_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_5 */
#define CRYPTO_AES_KEY_5_AES_KEY_5_SHIFT                   (0U)
#define CRYPTO_AES_KEY_5_AES_KEY_5_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_5_AES_KEY_5_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_6 */
#define CRYPTO_AES_KEY_6_AES_KEY_6_SHIFT                   (0U)
#define CRYPTO_AES_KEY_6_AES_KEY_6_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_6_AES_KEY_6_SHIFT)            /* 0xFFFFFFFF */
/* AES_KEY_7 */
#define CRYPTO_AES_KEY_7_AES_KEY_7_SHIFT                   (0U)
#define CRYPTO_AES_KEY_7_AES_KEY_7_MASK                    (0xFFFFFFFFU << CRYPTO_AES_KEY_7_AES_KEY_7_SHIFT)            /* 0xFFFFFFFF */
/* AES_CNT_0 */
#define CRYPTO_AES_CNT_0_AES_CNT_0_SHIFT                   (0U)
#define CRYPTO_AES_CNT_0_AES_CNT_0_MASK                    (0xFFFFFFFFU << CRYPTO_AES_CNT_0_AES_CNT_0_SHIFT)            /* 0xFFFFFFFF */
/* AES_CNT_1 */
#define CRYPTO_AES_CNT_1_AES_CNT_1_SHIFT                   (0U)
#define CRYPTO_AES_CNT_1_AES_CNT_1_MASK                    (0xFFFFFFFFU << CRYPTO_AES_CNT_1_AES_CNT_1_SHIFT)            /* 0xFFFFFFFF */
/* AES_CNT_2 */
#define CRYPTO_AES_CNT_2_AES_CNT_2_SHIFT                   (0U)
#define CRYPTO_AES_CNT_2_AES_CNT_2_MASK                    (0xFFFFFFFFU << CRYPTO_AES_CNT_2_AES_CNT_2_SHIFT)            /* 0xFFFFFFFF */
/* AES_CNT_3 */
#define CRYPTO_AES_CNT_3_AES_CNT_3_SHIFT                   (0U)
#define CRYPTO_AES_CNT_3_AES_CNT_3_MASK                    (0xFFFFFFFFU << CRYPTO_AES_CNT_3_AES_CNT_3_SHIFT)            /* 0xFFFFFFFF */
/* TDES_CTRL */
#define CRYPTO_TDES_CTRL_TDES_ENC_SHIFT                    (0U)
#define CRYPTO_TDES_CTRL_TDES_ENC_MASK                     (0x1U << CRYPTO_TDES_CTRL_TDES_ENC_SHIFT)                    /* 0x00000001 */
#define CRYPTO_TDES_CTRL_TDES_FIFOMODE_SHIFT               (1U)
#define CRYPTO_TDES_CTRL_TDES_FIFOMODE_MASK                (0x1U << CRYPTO_TDES_CTRL_TDES_FIFOMODE_SHIFT)               /* 0x00000002 */
#define CRYPTO_TDES_CTRL_TDES_SELECT_SHIFT                 (2U)
#define CRYPTO_TDES_CTRL_TDES_SELECT_MASK                  (0x1U << CRYPTO_TDES_CTRL_TDES_SELECT_SHIFT)                 /* 0x00000004 */
#define CRYPTO_TDES_CTRL_TDES_EEE_SHIFT                    (3U)
#define CRYPTO_TDES_CTRL_TDES_EEE_MASK                     (0x1U << CRYPTO_TDES_CTRL_TDES_EEE_SHIFT)                    /* 0x00000008 */
#define CRYPTO_TDES_CTRL_TDES_CHAINMODE_SHIFT              (4U)
#define CRYPTO_TDES_CTRL_TDES_CHAINMODE_MASK               (0x1U << CRYPTO_TDES_CTRL_TDES_CHAINMODE_SHIFT)              /* 0x00000010 */
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_DI_SHIFT            (5U)
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_DI_MASK             (0x1U << CRYPTO_TDES_CTRL_TDES_BYTESWAP_DI_SHIFT)            /* 0x00000020 */
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_DO_SHIFT            (6U)
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_DO_MASK             (0x1U << CRYPTO_TDES_CTRL_TDES_BYTESWAP_DO_SHIFT)            /* 0x00000040 */
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_IV_SHIFT            (7U)
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_IV_MASK             (0x1U << CRYPTO_TDES_CTRL_TDES_BYTESWAP_IV_SHIFT)            /* 0x00000080 */
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_KEY_SHIFT           (8U)
#define CRYPTO_TDES_CTRL_TDES_BYTESWAP_KEY_MASK            (0x1U << CRYPTO_TDES_CTRL_TDES_BYTESWAP_KEY_SHIFT)           /* 0x00000100 */
/* TDES_STS */
#define CRYPTO_TDES_STS_TDES_DONE_SHIFT                    (0U)
#define CRYPTO_TDES_STS_TDES_DONE_MASK                     (0x1U << CRYPTO_TDES_STS_TDES_DONE_SHIFT)                    /* 0x00000001 */
/* TDES_DIN_0 */
#define CRYPTO_TDES_DIN_0_TDES_DIN_0_SHIFT                 (0U)
#define CRYPTO_TDES_DIN_0_TDES_DIN_0_MASK                  (0xFFFFFFFFU << CRYPTO_TDES_DIN_0_TDES_DIN_0_SHIFT)          /* 0xFFFFFFFF */
/* TDES_DIN_1 */
#define CRYPTO_TDES_DIN_1_TDES_DIN_1_SHIFT                 (0U)
#define CRYPTO_TDES_DIN_1_TDES_DIN_1_MASK                  (0xFFFFFFFFU << CRYPTO_TDES_DIN_1_TDES_DIN_1_SHIFT)          /* 0xFFFFFFFF */
/* TDES_DOUT_0 */
#define CRYPTO_TDES_DOUT_0_TDES_DOUT_0_SHIFT               (0U)
#define CRYPTO_TDES_DOUT_0_TDES_DOUT_0_MASK                (0xFFFFFFFFU << CRYPTO_TDES_DOUT_0_TDES_DOUT_0_SHIFT)        /* 0xFFFFFFFF */
/* TDES_DOUT_1 */
#define CRYPTO_TDES_DOUT_1_TDES_DOUT_1_SHIFT               (0U)
#define CRYPTO_TDES_DOUT_1_TDES_DOUT_1_MASK                (0xFFFFFFFFU << CRYPTO_TDES_DOUT_1_TDES_DOUT_1_SHIFT)        /* 0xFFFFFFFF */
/* TDES_IV_0 */
#define CRYPTO_TDES_IV_0_TDES_IV_0_SHIFT                   (0U)
#define CRYPTO_TDES_IV_0_TDES_IV_0_MASK                    (0xFFFFFFFFU << CRYPTO_TDES_IV_0_TDES_IV_0_SHIFT)            /* 0xFFFFFFFF */
/* TDES_IV_1 */
#define CRYPTO_TDES_IV_1_TDES_IV_1_SHIFT                   (0U)
#define CRYPTO_TDES_IV_1_TDES_IV_1_MASK                    (0xFFFFFFFFU << CRYPTO_TDES_IV_1_TDES_IV_1_SHIFT)            /* 0xFFFFFFFF */
/* TDES_KEY1_0 */
#define CRYPTO_TDES_KEY1_0_TDES_KEY1_0_SHIFT               (0U)
#define CRYPTO_TDES_KEY1_0_TDES_KEY1_0_MASK                (0xFFFFFFFFU << CRYPTO_TDES_KEY1_0_TDES_KEY1_0_SHIFT)        /* 0xFFFFFFFF */
/* TDES_KEY1_1 */
#define CRYPTO_TDES_KEY1_1_TDES_KEY1_1_SHIFT               (0U)
#define CRYPTO_TDES_KEY1_1_TDES_KEY1_1_MASK                (0xFFFFFFFFU << CRYPTO_TDES_KEY1_1_TDES_KEY1_1_SHIFT)        /* 0xFFFFFFFF */
/* TDES_KEY2_0 */
#define CRYPTO_TDES_KEY2_0_TDES_KEY2_0_SHIFT               (0U)
#define CRYPTO_TDES_KEY2_0_TDES_KEY2_0_MASK                (0xFFFFFFFFU << CRYPTO_TDES_KEY2_0_TDES_KEY2_0_SHIFT)        /* 0xFFFFFFFF */
/* TDES_KEY2_1 */
#define CRYPTO_TDES_KEY2_1_TDES_KEY_1_SHIFT                (0U)
#define CRYPTO_TDES_KEY2_1_TDES_KEY_1_MASK                 (0xFFFFFFFFU << CRYPTO_TDES_KEY2_1_TDES_KEY_1_SHIFT)         /* 0xFFFFFFFF */
/* TDES_KEY3_0 */
#define CRYPTO_TDES_KEY3_0_TDES_KEY3_0_SHIFT               (0U)
#define CRYPTO_TDES_KEY3_0_TDES_KEY3_0_MASK                (0xFFFFFFFFU << CRYPTO_TDES_KEY3_0_TDES_KEY3_0_SHIFT)        /* 0xFFFFFFFF */
/* TDES_KEY3_1 */
#define CRYPTO_TDES_KEY3_1_AES_KEY3_1_SHIFT                (0U)
#define CRYPTO_TDES_KEY3_1_AES_KEY3_1_MASK                 (0xFFFFFFFFU << CRYPTO_TDES_KEY3_1_AES_KEY3_1_SHIFT)         /* 0xFFFFFFFF */
/* HASH_CTRL */
#define CRYPTO_HASH_CTRL_ENGINE_SELECTION_SHIFT            (0U)
#define CRYPTO_HASH_CTRL_ENGINE_SELECTION_MASK             (0x3U << CRYPTO_HASH_CTRL_ENGINE_SELECTION_SHIFT)            /* 0x00000003 */
#define CRYPTO_HASH_CTRL_HASH_SWAP_DO_SHIFT                (3U)
#define CRYPTO_HASH_CTRL_HASH_SWAP_DO_MASK                 (0x1U << CRYPTO_HASH_CTRL_HASH_SWAP_DO_SHIFT)                /* 0x00000008 */
/* HASH_STS */
#define CRYPTO_HASH_STS_HASH_DONE_SHIFT                    (0U)
#define CRYPTO_HASH_STS_HASH_DONE_MASK                     (0x1U << CRYPTO_HASH_STS_HASH_DONE_SHIFT)                    /* 0x00000001 */
/* HASH_MSG_LEN */
#define CRYPTO_HASH_MSG_LEN_MSG_SIZE_SHIFT                 (0U)
#define CRYPTO_HASH_MSG_LEN_MSG_SIZE_MASK                  (0xFFFFFFFFU << CRYPTO_HASH_MSG_LEN_MSG_SIZE_SHIFT)          /* 0xFFFFFFFF */
/* HASH_DOUT_0 */
#define CRYPTO_HASH_DOUT_0_HASH_RESULT_0_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_0_HASH_RESULT_0_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_0_HASH_RESULT_0_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_1 */
#define CRYPTO_HASH_DOUT_1_HASH_RESULT_1_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_1_HASH_RESULT_1_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_1_HASH_RESULT_1_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_2 */
#define CRYPTO_HASH_DOUT_2_HASH_RESULT_2_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_2_HASH_RESULT_2_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_2_HASH_RESULT_2_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_3 */
#define CRYPTO_HASH_DOUT_3_HASH_RESULT_3_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_3_HASH_RESULT_3_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_3_HASH_RESULT_3_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_4 */
#define CRYPTO_HASH_DOUT_4_HASH_RESULT_4_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_4_HASH_RESULT_4_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_4_HASH_RESULT_4_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_5 */
#define CRYPTO_HASH_DOUT_5_HASH_RESULT_5_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_5_HASH_RESULT_5_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_5_HASH_RESULT_5_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_6 */
#define CRYPTO_HASH_DOUT_6_HASH_RESULT_6_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_6_HASH_RESULT_6_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_6_HASH_RESULT_6_SHIFT)      /* 0xFFFFFFFF */
/* HASH_DOUT_7 */
#define CRYPTO_HASH_DOUT_7_HASH_RESULT_7_SHIFT             (0U)
#define CRYPTO_HASH_DOUT_7_HASH_RESULT_7_MASK              (0xFFFFFFFFU << CRYPTO_HASH_DOUT_7_HASH_RESULT_7_SHIFT)      /* 0xFFFFFFFF */
/* HASH_SEED_0 */
#define CRYPTO_HASH_SEED_0_HASH_SEED_0_SHIFT               (0U)
#define CRYPTO_HASH_SEED_0_HASH_SEED_0_MASK                (0xFFFFFFFFU << CRYPTO_HASH_SEED_0_HASH_SEED_0_SHIFT)        /* 0xFFFFFFFF */
/* HASH_SEED_1 */
#define CRYPTO_HASH_SEED_1_HASH_SEED_1_SHIFT               (0U)
#define CRYPTO_HASH_SEED_1_HASH_SEED_1_MASK                (0xFFFFFFFFU << CRYPTO_HASH_SEED_1_HASH_SEED_1_SHIFT)        /* 0xFFFFFFFF */
/* HASH_SEED_2 */
#define CRYPTO_HASH_SEED_2_HASH_SEED_2_SHIFT               (0U)
#define CRYPTO_HASH_SEED_2_HASH_SEED_2_MASK                (0xFFFFFFFFU << CRYPTO_HASH_SEED_2_HASH_SEED_2_SHIFT)        /* 0xFFFFFFFF */
/* HASH_SEED_3 */
#define CRYPTO_HASH_SEED_3_HASH_SEED_3_SHIFT               (0U)
#define CRYPTO_HASH_SEED_3_HASH_SEED_3_MASK                (0xFFFFFFFFU << CRYPTO_HASH_SEED_3_HASH_SEED_3_SHIFT)        /* 0xFFFFFFFF */
/* HASH_SEED_4 */
#define CRYPTO_HASH_SEED_4_HASH_SEED_4_SHIFT               (0U)
#define CRYPTO_HASH_SEED_4_HASH_SEED_4_MASK                (0xFFFFFFFFU << CRYPTO_HASH_SEED_4_HASH_SEED_4_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_CTRL */
#define CRYPTO_TRNG_CTRL_PERIOD_SHIFT                      (0U)
#define CRYPTO_TRNG_CTRL_PERIOD_MASK                       (0xFFFFU << CRYPTO_TRNG_CTRL_PERIOD_SHIFT)                   /* 0x0000FFFF */
#define CRYPTO_TRNG_CTRL_OSC_ENABLE_SHIFT                  (16U)
#define CRYPTO_TRNG_CTRL_OSC_ENABLE_MASK                   (0x1U << CRYPTO_TRNG_CTRL_OSC_ENABLE_SHIFT)                  /* 0x00010000 */
/* TRNG_DOUT_0 */
#define CRYPTO_TRNG_DOUT_0_TRNG_DOUT_0_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_0_TRNG_DOUT_0_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_0_TRNG_DOUT_0_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_1 */
#define CRYPTO_TRNG_DOUT_1_TRNG_DOUT_1_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_1_TRNG_DOUT_1_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_1_TRNG_DOUT_1_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_2 */
#define CRYPTO_TRNG_DOUT_2_TRNG_DOUT_2_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_2_TRNG_DOUT_2_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_2_TRNG_DOUT_2_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_3 */
#define CRYPTO_TRNG_DOUT_3_TRNG_DOUT_3_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_3_TRNG_DOUT_3_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_3_TRNG_DOUT_3_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_4 */
#define CRYPTO_TRNG_DOUT_4_TRNG_DOUT_4_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_4_TRNG_DOUT_4_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_4_TRNG_DOUT_4_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_5 */
#define CRYPTO_TRNG_DOUT_5_TRNG_DOUT_5_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_5_TRNG_DOUT_5_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_5_TRNG_DOUT_5_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_6 */
#define CRYPTO_TRNG_DOUT_6_TRNG_DOUT_6_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_6_TRNG_DOUT_6_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_6_TRNG_DOUT_6_SHIFT)        /* 0xFFFFFFFF */
/* TRNG_DOUT_7 */
#define CRYPTO_TRNG_DOUT_7_TRNG_DOUT_7_SHIFT               (0U)
#define CRYPTO_TRNG_DOUT_7_TRNG_DOUT_7_MASK                (0xFFFFFFFFU << CRYPTO_TRNG_DOUT_7_TRNG_DOUT_7_SHIFT)        /* 0xFFFFFFFF */
/* PKA_CTRL */
#define CRYPTO_PKA_CTRL_BLOCK_SIZE_SHIFT                   (0U)
#define CRYPTO_PKA_CTRL_BLOCK_SIZE_MASK                    (0x3U << CRYPTO_PKA_CTRL_BLOCK_SIZE_SHIFT)                   /* 0x00000003 */
/* CRYPTO_VER */
#define CRYPTO_CRYPTO_VER_CRYPTO_VER_SHIFT                 (0U)
#define CRYPTO_CRYPTO_VER_CRYPTO_VER_MASK                  (0xFFFFFFFFU << CRYPTO_CRYPTO_VER_CRYPTO_VER_SHIFT)          /* 0xFFFFFFFF */
/* PKA_M */
#define CRYPTO_PKA_M_M_SHIFT                               (0U)
#define CRYPTO_PKA_M_M_MASK                                (0xFFFFFFFFU << CRYPTO_PKA_M_M_SHIFT)                        /* 0xFFFFFFFF */
/* PKA_C */
#define CRYPTO_PKA_C_C_SHIFT                               (0U)
#define CRYPTO_PKA_C_C_MASK                                (0x1U << CRYPTO_PKA_C_C_SHIFT)                               /* 0x00000001 */
/* PKA_N */
#define CRYPTO_PKA_N_N_SHIFT                               (0U)
#define CRYPTO_PKA_N_N_MASK                                (0xFFFFFFFFU << CRYPTO_PKA_N_N_SHIFT)                        /* 0xFFFFFFFF */
/* PKA_E */
#define CRYPTO_PKA_E_E_SHIFT                               (0U)
#define CRYPTO_PKA_E_E_MASK                                (0xFFFFFFFFU << CRYPTO_PKA_E_E_SHIFT)                        /* 0xFFFFFFFF */
/******************************************SPI*******************************************/
/* CTRLR0 */
#define SPI_CTRLR0_DFS_SHIFT                               (0U)
#define SPI_CTRLR0_DFS_MASK                                (0x3U << SPI_CTRLR0_DFS_SHIFT)                               /* 0x00000003 */
#define SPI_CTRLR0_CFS_SHIFT                               (2U)
#define SPI_CTRLR0_CFS_MASK                                (0xFU << SPI_CTRLR0_CFS_SHIFT)                               /* 0x0000003C */
#define SPI_CTRLR0_SCPH_SHIFT                              (6U)
#define SPI_CTRLR0_SCPH_MASK                               (0x1U << SPI_CTRLR0_SCPH_SHIFT)                              /* 0x00000040 */
#define SPI_CTRLR0_SCPOL_SHIFT                             (7U)
#define SPI_CTRLR0_SCPOL_MASK                              (0x1U << SPI_CTRLR0_SCPOL_SHIFT)                             /* 0x00000080 */
#define SPI_CTRLR0_CSM_SHIFT                               (8U)
#define SPI_CTRLR0_CSM_MASK                                (0x3U << SPI_CTRLR0_CSM_SHIFT)                               /* 0x00000300 */
#define SPI_CTRLR0_SSD_SHIFT                               (10U)
#define SPI_CTRLR0_SSD_MASK                                (0x1U << SPI_CTRLR0_SSD_SHIFT)                               /* 0x00000400 */
#define SPI_CTRLR0_EM_SHIFT                                (11U)
#define SPI_CTRLR0_EM_MASK                                 (0x1U << SPI_CTRLR0_EM_SHIFT)                                /* 0x00000800 */
#define SPI_CTRLR0_FBM_SHIFT                               (12U)
#define SPI_CTRLR0_FBM_MASK                                (0x1U << SPI_CTRLR0_FBM_SHIFT)                               /* 0x00001000 */
#define SPI_CTRLR0_BHT_SHIFT                               (13U)
#define SPI_CTRLR0_BHT_MASK                                (0x1U << SPI_CTRLR0_BHT_SHIFT)                               /* 0x00002000 */
#define SPI_CTRLR0_RSD_SHIFT                               (14U)
#define SPI_CTRLR0_RSD_MASK                                (0x3U << SPI_CTRLR0_RSD_SHIFT)                               /* 0x0000C000 */
#define SPI_CTRLR0_FRF_SHIFT                               (16U)
#define SPI_CTRLR0_FRF_MASK                                (0x3U << SPI_CTRLR0_FRF_SHIFT)                               /* 0x00030000 */
#define SPI_CTRLR0_XFM_SHIFT                               (18U)
#define SPI_CTRLR0_XFM_MASK                                (0x3U << SPI_CTRLR0_XFM_SHIFT)                               /* 0x000C0000 */
#define SPI_CTRLR0_OPM_SHIFT                               (20U)
#define SPI_CTRLR0_OPM_MASK                                (0x1U << SPI_CTRLR0_OPM_SHIFT)                               /* 0x00100000 */
#define SPI_CTRLR0_MTM_SHIFT                               (21U)
#define SPI_CTRLR0_MTM_MASK                                (0x1U << SPI_CTRLR0_MTM_SHIFT)                               /* 0x00200000 */
/* CTRLR1 */
#define SPI_CTRLR1_NDM_SHIFT                               (0U)
#define SPI_CTRLR1_NDM_MASK                                (0xFFFFU << SPI_CTRLR1_NDM_SHIFT)                            /* 0x0000FFFF */
/* ENR */
#define SPI_ENR_ENR_SHIFT                                  (0U)
#define SPI_ENR_ENR_MASK                                   (0x1U << SPI_ENR_ENR_SHIFT)                                  /* 0x00000001 */
/* SER */
#define SPI_SER_SER_SHIFT                                  (0U)
#define SPI_SER_SER_MASK                                   (0x3U << SPI_SER_SER_SHIFT)                                  /* 0x00000003 */
/* BAUDR */
#define SPI_BAUDR_BAUDR_SHIFT                              (0U)
#define SPI_BAUDR_BAUDR_MASK                               (0xFFFFU << SPI_BAUDR_BAUDR_SHIFT)                           /* 0x0000FFFF */
/* TXFTLR */
#define SPI_TXFTLR_TXFTLR_SHIFT                            (0U)
#define SPI_TXFTLR_TXFTLR_MASK                             (0x1FU << SPI_TXFTLR_TXFTLR_SHIFT)                           /* 0x0000001F */
/* RXFTLR */
#define SPI_RXFTLR_RXFTLR_SHIFT                            (0U)
#define SPI_RXFTLR_RXFTLR_MASK                             (0x1FU << SPI_RXFTLR_RXFTLR_SHIFT)                           /* 0x0000001F */
/* TXFLR */
#define SPI_TXFLR_TXFLR_SHIFT                              (0U)
#define SPI_TXFLR_TXFLR_MASK                               (0x3FU << SPI_TXFLR_TXFLR_SHIFT)                             /* 0x0000003F */
/* RXFLR */
#define SPI_RXFLR_RXFLR_SHIFT                              (0U)
#define SPI_RXFLR_RXFLR_MASK                               (0x3FU << SPI_RXFLR_RXFLR_SHIFT)                             /* 0x0000003F */
/* SR */
#define SPI_SR_BSF_SHIFT                                   (0U)
#define SPI_SR_BSF_MASK                                    (0x1U << SPI_SR_BSF_SHIFT)                                   /* 0x00000001 */
#define SPI_SR_TFF_SHIFT                                   (1U)
#define SPI_SR_TFF_MASK                                    (0x1U << SPI_SR_TFF_SHIFT)                                   /* 0x00000002 */
#define SPI_SR_TFE_SHIFT                                   (2U)
#define SPI_SR_TFE_MASK                                    (0x1U << SPI_SR_TFE_SHIFT)                                   /* 0x00000004 */
#define SPI_SR_RFE_SHIFT                                   (3U)
#define SPI_SR_RFE_MASK                                    (0x1U << SPI_SR_RFE_SHIFT)                                   /* 0x00000008 */
#define SPI_SR_RFF_SHIFT                                   (4U)
#define SPI_SR_RFF_MASK                                    (0x1U << SPI_SR_RFF_SHIFT)                                   /* 0x00000010 */
/* IPR */
#define SPI_IPR_IPR_SHIFT                                  (0U)
#define SPI_IPR_IPR_MASK                                   (0x1U << SPI_IPR_IPR_SHIFT)                                  /* 0x00000001 */
/* IMR */
#define SPI_IMR_TFEIM_SHIFT                                (0U)
#define SPI_IMR_TFEIM_MASK                                 (0x1U << SPI_IMR_TFEIM_SHIFT)                                /* 0x00000001 */
#define SPI_IMR_TFOIM_SHIFT                                (1U)
#define SPI_IMR_TFOIM_MASK                                 (0x1U << SPI_IMR_TFOIM_SHIFT)                                /* 0x00000002 */
#define SPI_IMR_RFUIM_SHIFT                                (2U)
#define SPI_IMR_RFUIM_MASK                                 (0x1U << SPI_IMR_RFUIM_SHIFT)                                /* 0x00000004 */
#define SPI_IMR_RFOIM_SHIFT                                (3U)
#define SPI_IMR_RFOIM_MASK                                 (0x1U << SPI_IMR_RFOIM_SHIFT)                                /* 0x00000008 */
#define SPI_IMR_RFFIM_SHIFT                                (4U)
#define SPI_IMR_RFFIM_MASK                                 (0x1U << SPI_IMR_RFFIM_SHIFT)                                /* 0x00000010 */
/* ISR */
#define SPI_ISR_TFEIS_SHIFT                                (0U)
#define SPI_ISR_TFEIS_MASK                                 (0x1U << SPI_ISR_TFEIS_SHIFT)                                /* 0x00000001 */
#define SPI_ISR_TFOIS_SHIFT                                (1U)
#define SPI_ISR_TFOIS_MASK                                 (0x1U << SPI_ISR_TFOIS_SHIFT)                                /* 0x00000002 */
#define SPI_ISR_RFUIS_SHIFT                                (2U)
#define SPI_ISR_RFUIS_MASK                                 (0x1U << SPI_ISR_RFUIS_SHIFT)                                /* 0x00000004 */
#define SPI_ISR_RFOIS_SHIFT                                (3U)
#define SPI_ISR_RFOIS_MASK                                 (0x1U << SPI_ISR_RFOIS_SHIFT)                                /* 0x00000008 */
#define SPI_ISR_RFFIS_SHIFT                                (4U)
#define SPI_ISR_RFFIS_MASK                                 (0x1U << SPI_ISR_RFFIS_SHIFT)                                /* 0x00000010 */
/* RISR */
#define SPI_RISR_TFERIS_SHIFT                              (0U)
#define SPI_RISR_TFERIS_MASK                               (0x1U << SPI_RISR_TFERIS_SHIFT)                              /* 0x00000001 */
#define SPI_RISR_TFORIS_SHIFT                              (1U)
#define SPI_RISR_TFORIS_MASK                               (0x1U << SPI_RISR_TFORIS_SHIFT)                              /* 0x00000002 */
#define SPI_RISR_RFURIS_SHIFT                              (2U)
#define SPI_RISR_RFURIS_MASK                               (0x1U << SPI_RISR_RFURIS_SHIFT)                              /* 0x00000004 */
#define SPI_RISR_RFORIS_SHIFT                              (3U)
#define SPI_RISR_RFORIS_MASK                               (0x1U << SPI_RISR_RFORIS_SHIFT)                              /* 0x00000008 */
#define SPI_RISR_RFFRIS_SHIFT                              (4U)
#define SPI_RISR_RFFRIS_MASK                               (0x1U << SPI_RISR_RFFRIS_SHIFT)                              /* 0x00000010 */
/* ICR */
#define SPI_ICR_CCI_SHIFT                                  (0U)
#define SPI_ICR_CCI_MASK                                   (0x1U << SPI_ICR_CCI_SHIFT)                                  /* 0x00000001 */
#define SPI_ICR_CRFUI_SHIFT                                (1U)
#define SPI_ICR_CRFUI_MASK                                 (0x1U << SPI_ICR_CRFUI_SHIFT)                                /* 0x00000002 */
#define SPI_ICR_CRFOI_SHIFT                                (2U)
#define SPI_ICR_CRFOI_MASK                                 (0x1U << SPI_ICR_CRFOI_SHIFT)                                /* 0x00000004 */
#define SPI_ICR_CTFOI_SHIFT                                (3U)
#define SPI_ICR_CTFOI_MASK                                 (0x1U << SPI_ICR_CTFOI_SHIFT)                                /* 0x00000008 */
/* DMACR */
#define SPI_DMACR_RDE_SHIFT                                (0U)
#define SPI_DMACR_RDE_MASK                                 (0x1U << SPI_DMACR_RDE_SHIFT)                                /* 0x00000001 */
#define SPI_DMACR_TDE_SHIFT                                (1U)
#define SPI_DMACR_TDE_MASK                                 (0x1U << SPI_DMACR_TDE_SHIFT)                                /* 0x00000002 */
/* DMATDLR */
#define SPI_DMATDLR_TDL_SHIFT                              (0U)
#define SPI_DMATDLR_TDL_MASK                               (0x1FU << SPI_DMATDLR_TDL_SHIFT)                             /* 0x0000001F */
/* DMARDLR */
#define SPI_DMARDLR_RDL_SHIFT                              (0U)
#define SPI_DMARDLR_RDL_MASK                               (0x1FU << SPI_DMARDLR_RDL_SHIFT)                             /* 0x0000001F */
/* TXDR */
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/******************************************PWM*******************************************/
/* PWM0_CNT */
#define PWM_PWM0_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM0_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM0_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM0_PERIOD_HPR */
#define PWM_PWM0_PERIOD_HPR_PERIOD_LPR_SHIFT               (0U)
#define PWM_PWM0_PERIOD_HPR_PERIOD_LPR_MASK                (0xFFFFFFFFU << PWM_PWM0_PERIOD_HPR_PERIOD_LPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM0_DUTY_LPR */
#define PWM_PWM0_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM0_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM0_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM0_CTRL */
#define PWM_PWM0_CTRL_PWM_EN_SHIFT                         (0U)
#define PWM_PWM0_CTRL_PWM_EN_MASK                          (0x1U << PWM_PWM0_CTRL_PWM_EN_SHIFT)                         /* 0x00000001 */
#define PWM_PWM0_CTRL_PWM_MODE_SHIFT                       (1U)
#define PWM_PWM0_CTRL_PWM_MODE_MASK                        (0x3U << PWM_PWM0_CTRL_PWM_MODE_SHIFT)                       /* 0x00000006 */
#define PWM_PWM0_CTRL_DUTY_POL_SHIFT                       (3U)
#define PWM_PWM0_CTRL_DUTY_POL_MASK                        (0x1U << PWM_PWM0_CTRL_DUTY_POL_SHIFT)                       /* 0x00000008 */
#define PWM_PWM0_CTRL_INACTIVE_POL_SHIFT                   (4U)
#define PWM_PWM0_CTRL_INACTIVE_POL_MASK                    (0x1U << PWM_PWM0_CTRL_INACTIVE_POL_SHIFT)                   /* 0x00000010 */
#define PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT                    (5U)
#define PWM_PWM0_CTRL_OUTPUT_MODE_MASK                     (0x1U << PWM_PWM0_CTRL_OUTPUT_MODE_SHIFT)                    /* 0x00000020 */
#define PWM_PWM0_CTRL_LP_EN_SHIFT                          (8U)
#define PWM_PWM0_CTRL_LP_EN_MASK                           (0x1U << PWM_PWM0_CTRL_LP_EN_SHIFT)                          /* 0x00000100 */
#define PWM_PWM0_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM0_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM0_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM0_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM0_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM0_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM0_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM0_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM0_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM0_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM0_CTRL_RPT_MASK                             (0xFFU << PWM_PWM0_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* PWM1_CNT */
#define PWM_PWM1_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM1_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM1_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM1_PERIOD_HPR */
#define PWM_PWM1_PERIOD_HPR_PERIOD_LPR_SHIFT               (0U)
#define PWM_PWM1_PERIOD_HPR_PERIOD_LPR_MASK                (0xFFFFFFFFU << PWM_PWM1_PERIOD_HPR_PERIOD_LPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM1_DUTY_LPR */
#define PWM_PWM1_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM1_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM1_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM1_CTRL */
#define PWM_PWM1_CTRL_PWM_EN_SHIFT                         (0U)
#define PWM_PWM1_CTRL_PWM_EN_MASK                          (0x1U << PWM_PWM1_CTRL_PWM_EN_SHIFT)                         /* 0x00000001 */
#define PWM_PWM1_CTRL_PWM_MODE_SHIFT                       (1U)
#define PWM_PWM1_CTRL_PWM_MODE_MASK                        (0x3U << PWM_PWM1_CTRL_PWM_MODE_SHIFT)                       /* 0x00000006 */
#define PWM_PWM1_CTRL_DUTY_POL_SHIFT                       (3U)
#define PWM_PWM1_CTRL_DUTY_POL_MASK                        (0x1U << PWM_PWM1_CTRL_DUTY_POL_SHIFT)                       /* 0x00000008 */
#define PWM_PWM1_CTRL_INACTIVE_POL_SHIFT                   (4U)
#define PWM_PWM1_CTRL_INACTIVE_POL_MASK                    (0x1U << PWM_PWM1_CTRL_INACTIVE_POL_SHIFT)                   /* 0x00000010 */
#define PWM_PWM1_CTRL_OUTPUT_MODE_SHIFT                    (5U)
#define PWM_PWM1_CTRL_OUTPUT_MODE_MASK                     (0x1U << PWM_PWM1_CTRL_OUTPUT_MODE_SHIFT)                    /* 0x00000020 */
#define PWM_PWM1_CTRL_LP_EN_SHIFT                          (8U)
#define PWM_PWM1_CTRL_LP_EN_MASK                           (0x1U << PWM_PWM1_CTRL_LP_EN_SHIFT)                          /* 0x00000100 */
#define PWM_PWM1_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM1_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM1_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM1_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM1_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM1_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM1_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM1_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM1_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM1_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM1_CTRL_RPT_MASK                             (0xFFU << PWM_PWM1_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* PWM2_CNT */
#define PWM_PWM2_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM2_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM2_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM2_PERIOD_HPR */
#define PWM_PWM2_PERIOD_HPR_PERIOD_LPR_SHIFT               (0U)
#define PWM_PWM2_PERIOD_HPR_PERIOD_LPR_MASK                (0xFFFFFFFFU << PWM_PWM2_PERIOD_HPR_PERIOD_LPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM2_DUTY_LPR */
#define PWM_PWM2_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM2_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM2_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM2_CTRL */
#define PWM_PWM2_CTRL_PWM_EN_SHIFT                         (0U)
#define PWM_PWM2_CTRL_PWM_EN_MASK                          (0x1U << PWM_PWM2_CTRL_PWM_EN_SHIFT)                         /* 0x00000001 */
#define PWM_PWM2_CTRL_PWM_MODE_SHIFT                       (1U)
#define PWM_PWM2_CTRL_PWM_MODE_MASK                        (0x3U << PWM_PWM2_CTRL_PWM_MODE_SHIFT)                       /* 0x00000006 */
#define PWM_PWM2_CTRL_DUTY_POL_SHIFT                       (3U)
#define PWM_PWM2_CTRL_DUTY_POL_MASK                        (0x1U << PWM_PWM2_CTRL_DUTY_POL_SHIFT)                       /* 0x00000008 */
#define PWM_PWM2_CTRL_INACTIVE_POL_SHIFT                   (4U)
#define PWM_PWM2_CTRL_INACTIVE_POL_MASK                    (0x1U << PWM_PWM2_CTRL_INACTIVE_POL_SHIFT)                   /* 0x00000010 */
#define PWM_PWM2_CTRL_OUTPUT_MODE_SHIFT                    (5U)
#define PWM_PWM2_CTRL_OUTPUT_MODE_MASK                     (0x1U << PWM_PWM2_CTRL_OUTPUT_MODE_SHIFT)                    /* 0x00000020 */
#define PWM_PWM2_CTRL_LP_EN_SHIFT                          (8U)
#define PWM_PWM2_CTRL_LP_EN_MASK                           (0x1U << PWM_PWM2_CTRL_LP_EN_SHIFT)                          /* 0x00000100 */
#define PWM_PWM2_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM2_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM2_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM2_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM2_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM2_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM2_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM2_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM2_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM2_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM2_CTRL_RPT_MASK                             (0xFFU << PWM_PWM2_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* PWM3_CNT */
#define PWM_PWM3_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM3_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM3_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM3_PERIOD_HPR */
#define PWM_PWM3_PERIOD_HPR_PERIOD_LPR_SHIFT               (0U)
#define PWM_PWM3_PERIOD_HPR_PERIOD_LPR_MASK                (0xFFFFFFFFU << PWM_PWM3_PERIOD_HPR_PERIOD_LPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM3_DUTY_LPR */
#define PWM_PWM3_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM3_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM3_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM3_CTRL */
#define PWM_PWM3_CTRL_PWM_EN_SHIFT                         (0U)
#define PWM_PWM3_CTRL_PWM_EN_MASK                          (0x1U << PWM_PWM3_CTRL_PWM_EN_SHIFT)                         /* 0x00000001 */
#define PWM_PWM3_CTRL_PWM_MODE_SHIFT                       (1U)
#define PWM_PWM3_CTRL_PWM_MODE_MASK                        (0x3U << PWM_PWM3_CTRL_PWM_MODE_SHIFT)                       /* 0x00000006 */
#define PWM_PWM3_CTRL_DUTY_POL_SHIFT                       (3U)
#define PWM_PWM3_CTRL_DUTY_POL_MASK                        (0x1U << PWM_PWM3_CTRL_DUTY_POL_SHIFT)                       /* 0x00000008 */
#define PWM_PWM3_CTRL_INACTIVE_POL_SHIFT                   (4U)
#define PWM_PWM3_CTRL_INACTIVE_POL_MASK                    (0x1U << PWM_PWM3_CTRL_INACTIVE_POL_SHIFT)                   /* 0x00000010 */
#define PWM_PWM3_CTRL_OUTPUT_MODE_SHIFT                    (5U)
#define PWM_PWM3_CTRL_OUTPUT_MODE_MASK                     (0x1U << PWM_PWM3_CTRL_OUTPUT_MODE_SHIFT)                    /* 0x00000020 */
#define PWM_PWM3_CTRL_LP_EN_SHIFT                          (8U)
#define PWM_PWM3_CTRL_LP_EN_MASK                           (0x1U << PWM_PWM3_CTRL_LP_EN_SHIFT)                          /* 0x00000100 */
#define PWM_PWM3_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM3_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM3_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM3_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM3_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM3_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM3_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM3_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM3_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM3_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM3_CTRL_RPT_MASK                             (0xFFU << PWM_PWM3_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* INTSTS */
#define PWM_INTSTS_CH0_INTSTS_SHIFT                        (0U)
#define PWM_INTSTS_CH0_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH0_INTSTS_SHIFT)                        /* 0x00000001 */
#define PWM_INTSTS_CH1_INTSTS_SHIFT                        (1U)
#define PWM_INTSTS_CH1_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH1_INTSTS_SHIFT)                        /* 0x00000002 */
#define PWM_INTSTS_CH2_INTSTS_SHIFT                        (2U)
#define PWM_INTSTS_CH2_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH2_INTSTS_SHIFT)                        /* 0x00000004 */
#define PWM_INTSTS_CH3_INTSTS_SHIFT                        (3U)
#define PWM_INTSTS_CH3_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH3_INTSTS_SHIFT)                        /* 0x00000008 */
#define PWM_INTSTS_CH0_POL_SHIFT                           (8U)
#define PWM_INTSTS_CH0_POL_MASK                            (0x1U << PWM_INTSTS_CH0_POL_SHIFT)                           /* 0x00000100 */
#define PWM_INTSTS_CH1_POL_SHIFT                           (9U)
#define PWM_INTSTS_CH1_POL_MASK                            (0x1U << PWM_INTSTS_CH1_POL_SHIFT)                           /* 0x00000200 */
#define PWM_INTSTS_CH2_POL_SHIFT                           (10U)
#define PWM_INTSTS_CH2_POL_MASK                            (0x1U << PWM_INTSTS_CH2_POL_SHIFT)                           /* 0x00000400 */
#define PWM_INTSTS_CH3_POL_SHIFT                           (11U)
#define PWM_INTSTS_CH3_POL_MASK                            (0x1U << PWM_INTSTS_CH3_POL_SHIFT)                           /* 0x00000800 */
/* INT_EN */
#define PWM_INT_EN_CH0_INT_EN_SHIFT                        (0U)
#define PWM_INT_EN_CH0_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH0_INT_EN_SHIFT)                        /* 0x00000001 */
#define PWM_INT_EN_CH1_INT_EN_SHIFT                        (1U)
#define PWM_INT_EN_CH1_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH1_INT_EN_SHIFT)                        /* 0x00000002 */
#define PWM_INT_EN_CH2_INT_EN_SHIFT                        (2U)
#define PWM_INT_EN_CH2_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH2_INT_EN_SHIFT)                        /* 0x00000004 */
#define PWM_INT_EN_CH3_INT_EN_SHIFT                        (3U)
#define PWM_INT_EN_CH3_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH3_INT_EN_SHIFT)                        /* 0x00000008 */
/* PWM_FIFO_CTRL */
#define PWM_PWM_FIFO_CTRL_FIFO_MODE_SEL_SHIFT              (0U)
#define PWM_PWM_FIFO_CTRL_FIFO_MODE_SEL_MASK               (0x1U << PWM_PWM_FIFO_CTRL_FIFO_MODE_SEL_SHIFT)              /* 0x00000001 */
#define PWM_PWM_FIFO_CTRL_FULL_INT_EN_SHIFT                (1U)
#define PWM_PWM_FIFO_CTRL_FULL_INT_EN_MASK                 (0x1U << PWM_PWM_FIFO_CTRL_FULL_INT_EN_SHIFT)                /* 0x00000002 */
#define PWM_PWM_FIFO_CTRL_OVERFLOW_INT_EN_SHIFT            (2U)
#define PWM_PWM_FIFO_CTRL_OVERFLOW_INT_EN_MASK             (0x1U << PWM_PWM_FIFO_CTRL_OVERFLOW_INT_EN_SHIFT)            /* 0x00000004 */
#define PWM_PWM_FIFO_CTRL_WATERMARK_INT_EN_SHIFT           (3U)
#define PWM_PWM_FIFO_CTRL_WATERMARK_INT_EN_MASK            (0x1U << PWM_PWM_FIFO_CTRL_WATERMARK_INT_EN_SHIFT)           /* 0x00000008 */
#define PWM_PWM_FIFO_CTRL_ALMOST_FULL_WATERMARK_SHIFT      (4U)
#define PWM_PWM_FIFO_CTRL_ALMOST_FULL_WATERMARK_MASK       (0x7U << PWM_PWM_FIFO_CTRL_ALMOST_FULL_WATERMARK_SHIFT)      /* 0x00000070 */
#define PWM_PWM_FIFO_CTRL_DMA_MODE_EN_SHIFT                (8U)
#define PWM_PWM_FIFO_CTRL_DMA_MODE_EN_MASK                 (0x1U << PWM_PWM_FIFO_CTRL_DMA_MODE_EN_SHIFT)                /* 0x00000100 */
#define PWM_PWM_FIFO_CTRL_TIMEOUT_EN_SHIFT                 (9U)
#define PWM_PWM_FIFO_CTRL_TIMEOUT_EN_MASK                  (0x1U << PWM_PWM_FIFO_CTRL_TIMEOUT_EN_SHIFT)                 /* 0x00000200 */
/* PWM_FIFO_INTSTS */
#define PWM_PWM_FIFO_INTSTS_FIFO_FULL_INTSTS_SHIFT         (0U)
#define PWM_PWM_FIFO_INTSTS_FIFO_FULL_INTSTS_MASK          (0x1U << PWM_PWM_FIFO_INTSTS_FIFO_FULL_INTSTS_SHIFT)         /* 0x00000001 */
#define PWM_PWM_FIFO_INTSTS_FIFO_OVERFLOW_INTSTS_SHIFT     (1U)
#define PWM_PWM_FIFO_INTSTS_FIFO_OVERFLOW_INTSTS_MASK      (0x1U << PWM_PWM_FIFO_INTSTS_FIFO_OVERFLOW_INTSTS_SHIFT)     /* 0x00000002 */
#define PWM_PWM_FIFO_INTSTS_FIFO_WATERMARK_FULL_INTSTS_SHIFT (2U)
#define PWM_PWM_FIFO_INTSTS_FIFO_WATERMARK_FULL_INTSTS_MASK (0x1U << PWM_PWM_FIFO_INTSTS_FIFO_WATERMARK_FULL_INTSTS_SHIFT) /* 0x00000004 */
#define PWM_PWM_FIFO_INTSTS_TIMIEOUT_INTSTS_SHIFT          (3U)
#define PWM_PWM_FIFO_INTSTS_TIMIEOUT_INTSTS_MASK           (0x1U << PWM_PWM_FIFO_INTSTS_TIMIEOUT_INTSTS_SHIFT)          /* 0x00000008 */
#define PWM_PWM_FIFO_INTSTS_FIFO_EMPTY_STATUS_SHIFT        (4U)
#define PWM_PWM_FIFO_INTSTS_FIFO_EMPTY_STATUS_MASK         (0x1U << PWM_PWM_FIFO_INTSTS_FIFO_EMPTY_STATUS_SHIFT)        /* 0x00000010 */
/* PWM_FIFO_TOUTTHR */
#define PWM_PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_SHIFT       (0U)
#define PWM_PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_MASK        (0xFFFFFU << PWM_PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_SHIFT)   /* 0x000FFFFF */
/* PWM_FIFO */
#define PWM_PWM_FIFO_CYCLE_CNT_SHIFT                       (0U)
#define PWM_PWM_FIFO_CYCLE_CNT_MASK                        (0x7FFFFFFFU << PWM_PWM_FIFO_CYCLE_CNT_SHIFT)                /* 0x7FFFFFFF */
#define PWM_PWM_FIFO_POL_SHIFT                             (31U)
#define PWM_PWM_FIFO_POL_MASK                              (0x1U << PWM_PWM_FIFO_POL_SHIFT)                             /* 0x80000000 */
/******************************************GRF*******************************************/
/* GPIO1A_IOMUX */
#define GRF_GPIO1A_IOMUX_OFFSET                            (0x10)
#define GRF_GPIO1A_IOMUX_GPIO1A0_SEL_SHIFT                 (0U)
#define GRF_GPIO1A_IOMUX_GPIO1A0_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1A_IOMUX_GPIO1A1_SEL_SHIFT                 (2U)
#define GRF_GPIO1A_IOMUX_GPIO1A1_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1A_IOMUX_GPIO1A2_SEL_SHIFT                 (4U)
#define GRF_GPIO1A_IOMUX_GPIO1A2_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1A_IOMUX_GPIO1A3_SEL_SHIFT                 (6U)
#define GRF_GPIO1A_IOMUX_GPIO1A3_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1A_IOMUX_GPIO1A4_SEL_SHIFT                 (8U)
#define GRF_GPIO1A_IOMUX_GPIO1A4_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1A_IOMUX_GPIO1A5_SEL_SHIFT                 (10U)
#define GRF_GPIO1A_IOMUX_GPIO1A5_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1A_IOMUX_GPIO1A6_SEL_SHIFT                 (12U)
#define GRF_GPIO1A_IOMUX_GPIO1A6_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1A_IOMUX_GPIO1A7_SEL_SHIFT                 (14U)
#define GRF_GPIO1A_IOMUX_GPIO1A7_SEL_MASK                  (0x3U << GRF_GPIO1A_IOMUX_GPIO1A7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1B_IOMUX */
#define GRF_GPIO1B_IOMUX_OFFSET                            (0x14)
#define GRF_GPIO1B_IOMUX_GPIO1B0_SEL_SHIFT                 (0U)
#define GRF_GPIO1B_IOMUX_GPIO1B0_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1B_IOMUX_GPIO1B1_SEL_SHIFT                 (2U)
#define GRF_GPIO1B_IOMUX_GPIO1B1_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1B_IOMUX_GPIO1B2_SEL_SHIFT                 (4U)
#define GRF_GPIO1B_IOMUX_GPIO1B2_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1B_IOMUX_GPIO1B3_SEL_SHIFT                 (6U)
#define GRF_GPIO1B_IOMUX_GPIO1B3_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1B_IOMUX_GPIO1B4_SEL_SHIFT                 (8U)
#define GRF_GPIO1B_IOMUX_GPIO1B4_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1B_IOMUX_GPIO1B5_SEL_SHIFT                 (10U)
#define GRF_GPIO1B_IOMUX_GPIO1B5_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1B_IOMUX_GPIO1B6_SEL_SHIFT                 (12U)
#define GRF_GPIO1B_IOMUX_GPIO1B6_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1B_IOMUX_GPIO1B7_SEL_SHIFT                 (14U)
#define GRF_GPIO1B_IOMUX_GPIO1B7_SEL_MASK                  (0x3U << GRF_GPIO1B_IOMUX_GPIO1B7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1C_IOMUX */
#define GRF_GPIO1C_IOMUX_OFFSET                            (0x18)
#define GRF_GPIO1C_IOMUX_GPIO1C0_SEL_SHIFT                 (0U)
#define GRF_GPIO1C_IOMUX_GPIO1C0_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1C_IOMUX_GPIO1C1_SEL_SHIFT                 (2U)
#define GRF_GPIO1C_IOMUX_GPIO1C1_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1C_IOMUX_GPIO1C2_SEL_SHIFT                 (4U)
#define GRF_GPIO1C_IOMUX_GPIO1C2_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1C_IOMUX_GPIO1C3_SEL_SHIFT                 (6U)
#define GRF_GPIO1C_IOMUX_GPIO1C3_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1C_IOMUX_GPIO1C4_SEL_SHIFT                 (8U)
#define GRF_GPIO1C_IOMUX_GPIO1C4_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1C_IOMUX_GPIO1C5_SEL_SHIFT                 (10U)
#define GRF_GPIO1C_IOMUX_GPIO1C5_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1C_IOMUX_GPIO1C6_SEL_SHIFT                 (12U)
#define GRF_GPIO1C_IOMUX_GPIO1C6_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1C_IOMUX_GPIO1C7_SEL_SHIFT                 (14U)
#define GRF_GPIO1C_IOMUX_GPIO1C7_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1D_IOMUX */
#define GRF_GPIO1D_IOMUX_OFFSET                            (0x1C)
#define GRF_GPIO1D_IOMUX_GPIO1D0_SEL_SHIFT                 (0U)
#define GRF_GPIO1D_IOMUX_GPIO1D0_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO1D_IOMUX_GPIO1D1_SEL_SHIFT                 (2U)
#define GRF_GPIO1D_IOMUX_GPIO1D1_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO1D_IOMUX_GPIO1D2_SEL_SHIFT                 (4U)
#define GRF_GPIO1D_IOMUX_GPIO1D2_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1D_IOMUX_GPIO1D3_SEL_SHIFT                 (6U)
#define GRF_GPIO1D_IOMUX_GPIO1D3_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO1D_IOMUX_GPIO1D4_SEL_SHIFT                 (8U)
#define GRF_GPIO1D_IOMUX_GPIO1D4_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1D_IOMUX_GPIO1D5_SEL_SHIFT                 (10U)
#define GRF_GPIO1D_IOMUX_GPIO1D5_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO1D_IOMUX_GPIO1D6_SEL_SHIFT                 (12U)
#define GRF_GPIO1D_IOMUX_GPIO1D6_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1D_IOMUX_GPIO1D7_SEL_SHIFT                 (14U)
#define GRF_GPIO1D_IOMUX_GPIO1D7_SEL_MASK                  (0x3U << GRF_GPIO1D_IOMUX_GPIO1D7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2A_IOMUX */
#define GRF_GPIO2A_IOMUX_OFFSET                            (0x20)
#define GRF_GPIO2A_IOMUX_GPIO2A0_SEL_SHIFT                 (0U)
#define GRF_GPIO2A_IOMUX_GPIO2A0_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2A_IOMUX_GPIO2A1_SEL_SHIFT                 (2U)
#define GRF_GPIO2A_IOMUX_GPIO2A1_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2A_IOMUX_GPIO2A2_SEL_SHIFT                 (4U)
#define GRF_GPIO2A_IOMUX_GPIO2A2_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2A_IOMUX_GPIO2A3_SEL_SHIFT                 (6U)
#define GRF_GPIO2A_IOMUX_GPIO2A3_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2A_IOMUX_GPIO2A4_SEL_SHIFT                 (8U)
#define GRF_GPIO2A_IOMUX_GPIO2A4_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2A_IOMUX_GPIO2A5_SEL_SHIFT                 (10U)
#define GRF_GPIO2A_IOMUX_GPIO2A5_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2A_IOMUX_GPIO2A6_SEL_SHIFT                 (12U)
#define GRF_GPIO2A_IOMUX_GPIO2A6_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2A_IOMUX_GPIO2A7_SEL_SHIFT                 (14U)
#define GRF_GPIO2A_IOMUX_GPIO2A7_SEL_MASK                  (0x3U << GRF_GPIO2A_IOMUX_GPIO2A7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2B_IOMUX */
#define GRF_GPIO2B_IOMUX_OFFSET                            (0x24)
#define GRF_GPIO2B_IOMUX_GPIO2B0_SEL_SHIFT                 (0U)
#define GRF_GPIO2B_IOMUX_GPIO2B0_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2B_IOMUX_GPIO2B1_SEL_SHIFT                 (2U)
#define GRF_GPIO2B_IOMUX_GPIO2B1_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2B_IOMUX_GPIO2B2_SEL_SHIFT                 (4U)
#define GRF_GPIO2B_IOMUX_GPIO2B2_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2B_IOMUX_GPIO2B3_SEL_SHIFT                 (6U)
#define GRF_GPIO2B_IOMUX_GPIO2B3_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2B_IOMUX_GPIO2B4_SEL_SHIFT                 (8U)
#define GRF_GPIO2B_IOMUX_GPIO2B4_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2B_IOMUX_GPIO2B5_SEL_SHIFT                 (10U)
#define GRF_GPIO2B_IOMUX_GPIO2B5_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2B_IOMUX_GPIO2B6_SEL_SHIFT                 (12U)
#define GRF_GPIO2B_IOMUX_GPIO2B6_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2B_IOMUX_GPIO2B7_SEL_SHIFT                 (14U)
#define GRF_GPIO2B_IOMUX_GPIO2B7_SEL_MASK                  (0x3U << GRF_GPIO2B_IOMUX_GPIO2B7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2C_IOMUX */
#define GRF_GPIO2C_IOMUX_OFFSET                            (0x28)
#define GRF_GPIO2C_IOMUX_GPIO2C0_SEL_SHIFT                 (0U)
#define GRF_GPIO2C_IOMUX_GPIO2C0_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2C_IOMUX_GPIO2C1_SEL_SHIFT                 (2U)
#define GRF_GPIO2C_IOMUX_GPIO2C1_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2C_IOMUX_GPIO2C2_SEL_SHIFT                 (4U)
#define GRF_GPIO2C_IOMUX_GPIO2C2_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2C_IOMUX_GPIO2C3_SEL_SHIFT                 (6U)
#define GRF_GPIO2C_IOMUX_GPIO2C3_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2C_IOMUX_GPIO2C4_SEL_SHIFT                 (8U)
#define GRF_GPIO2C_IOMUX_GPIO2C4_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2C_IOMUX_GPIO2C5_SEL_SHIFT                 (10U)
#define GRF_GPIO2C_IOMUX_GPIO2C5_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2C_IOMUX_GPIO2C6_SEL_SHIFT                 (12U)
#define GRF_GPIO2C_IOMUX_GPIO2C6_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C6_SEL_SHIFT)                 /* 0x00003000 */
/* GPIO2D_IOMUX */
#define GRF_GPIO2D_IOMUX_OFFSET                            (0x2C)
#define GRF_GPIO2D_IOMUX_GPIO2D1_SEL_SHIFT                 (2U)
#define GRF_GPIO2D_IOMUX_GPIO2D1_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2D_IOMUX_GPIO2D2_SEL_SHIFT                 (4U)
#define GRF_GPIO2D_IOMUX_GPIO2D2_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2D_IOMUX_GPIO2D3_SEL_SHIFT                 (6U)
#define GRF_GPIO2D_IOMUX_GPIO2D3_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO2D_IOMUX_GPIO2D4_SEL_SHIFT                 (8U)
#define GRF_GPIO2D_IOMUX_GPIO2D4_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO2D_IOMUX_GPIO2D5_SEL_SHIFT                 (10U)
#define GRF_GPIO2D_IOMUX_GPIO2D5_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO2D_IOMUX_GPIO2D6_SEL_SHIFT                 (12U)
#define GRF_GPIO2D_IOMUX_GPIO2D6_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO2D_IOMUX_GPIO2D7_SEL_SHIFT                 (14U)
#define GRF_GPIO2D_IOMUX_GPIO2D7_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO3A_IOMUX */
#define GRF_GPIO3A_IOMUX_OFFSET                            (0x30)
#define GRF_GPIO3A_IOMUX_GPIO3A0_SEL_SHIFT                 (0U)
#define GRF_GPIO3A_IOMUX_GPIO3A0_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO3A_IOMUX_GPIO3A1_SEL_SHIFT                 (2U)
#define GRF_GPIO3A_IOMUX_GPIO3A1_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO3A_IOMUX_GPIO3A2_SEL_SHIFT                 (4U)
#define GRF_GPIO3A_IOMUX_GPIO3A2_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO3A_IOMUX_GPIO3A3_SEL_SHIFT                 (6U)
#define GRF_GPIO3A_IOMUX_GPIO3A3_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO3A_IOMUX_GPIO3A4_SEL_SHIFT                 (8U)
#define GRF_GPIO3A_IOMUX_GPIO3A4_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO3A_IOMUX_GPIO3A5_SEL_SHIFT                 (10U)
#define GRF_GPIO3A_IOMUX_GPIO3A5_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO3A_IOMUX_GPIO3A6_SEL_SHIFT                 (12U)
#define GRF_GPIO3A_IOMUX_GPIO3A6_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO3A_IOMUX_GPIO3A7_SEL_SHIFT                 (14U)
#define GRF_GPIO3A_IOMUX_GPIO3A7_SEL_MASK                  (0x3U << GRF_GPIO3A_IOMUX_GPIO3A7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO3B_IOMUX */
#define GRF_GPIO3B_IOMUX_OFFSET                            (0x34)
#define GRF_GPIO3B_IOMUX_GPIO3B0_SEL_SHIFT                 (0U)
#define GRF_GPIO3B_IOMUX_GPIO3B0_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO3B_IOMUX_GPIO3B1_SEL_SHIFT                 (2U)
#define GRF_GPIO3B_IOMUX_GPIO3B1_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO3B_IOMUX_GPIO3B2_SEL_SHIFT                 (4U)
#define GRF_GPIO3B_IOMUX_GPIO3B2_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO3B_IOMUX_GPIO3B3_SEL_SHIFT                 (6U)
#define GRF_GPIO3B_IOMUX_GPIO3B3_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO3B_IOMUX_GPIO3B4_SEL_SHIFT                 (8U)
#define GRF_GPIO3B_IOMUX_GPIO3B4_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO3B_IOMUX_GPIO3B5_SEL_SHIFT                 (10U)
#define GRF_GPIO3B_IOMUX_GPIO3B5_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO3B_IOMUX_GPIO3B6_SEL_SHIFT                 (12U)
#define GRF_GPIO3B_IOMUX_GPIO3B6_SEL_MASK                  (0x3U << GRF_GPIO3B_IOMUX_GPIO3B6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO3B_IOMUX_GPIO3B7_SEL_SHIFT                 (14U)
#define GRF_GPIO3B_IOMUX_GPIO3B7_SEL_MASK                  (0x1U << GRF_GPIO3B_IOMUX_GPIO3B7_SEL_SHIFT)                 /* 0x00004000 */
/* GPIO3C_IOMUX */
#define GRF_GPIO3C_IOMUX_OFFSET                            (0x38)
#define GRF_GPIO3C_IOMUX_GPIO3C0_SEL_SHIFT                 (0U)
#define GRF_GPIO3C_IOMUX_GPIO3C0_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO3C_IOMUX_GPIO3C1_SEL_SHIFT                 (2U)
#define GRF_GPIO3C_IOMUX_GPIO3C1_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO3C_IOMUX_GPIO3C2_SEL_SHIFT                 (4U)
#define GRF_GPIO3C_IOMUX_GPIO3C2_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO3C_IOMUX_GPIO3C3_SEL_SHIFT                 (6U)
#define GRF_GPIO3C_IOMUX_GPIO3C3_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO3C_IOMUX_GPIO3C4_SEL_SHIFT                 (8U)
#define GRF_GPIO3C_IOMUX_GPIO3C4_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO3C_IOMUX_GPIO3C5_SEL_SHIFT                 (10U)
#define GRF_GPIO3C_IOMUX_GPIO3C5_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO3C_IOMUX_GPIO3C6_SEL_SHIFT                 (12U)
#define GRF_GPIO3C_IOMUX_GPIO3C6_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO3C_IOMUX_GPIO3C7_SEL_SHIFT                 (14U)
#define GRF_GPIO3C_IOMUX_GPIO3C7_SEL_MASK                  (0x3U << GRF_GPIO3C_IOMUX_GPIO3C7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO3D_IOMUX */
#define GRF_GPIO3D_IOMUX_OFFSET                            (0x3C)
#define GRF_GPIO3D_IOMUX_GPIO3D0_SEL_SHIFT                 (0U)
#define GRF_GPIO3D_IOMUX_GPIO3D0_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO3D_IOMUX_GPIO3D1_SEL_SHIFT                 (2U)
#define GRF_GPIO3D_IOMUX_GPIO3D1_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO3D_IOMUX_GPIO3D2_SEL_SHIFT                 (4U)
#define GRF_GPIO3D_IOMUX_GPIO3D2_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO3D_IOMUX_GPIO3D3_SEL_SHIFT                 (6U)
#define GRF_GPIO3D_IOMUX_GPIO3D3_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO3D_IOMUX_GPIO3D4_SEL_SHIFT                 (8U)
#define GRF_GPIO3D_IOMUX_GPIO3D4_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO3D_IOMUX_GPIO3D5_SEL_SHIFT                 (10U)
#define GRF_GPIO3D_IOMUX_GPIO3D5_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO3D_IOMUX_GPIO3D6_SEL_SHIFT                 (12U)
#define GRF_GPIO3D_IOMUX_GPIO3D6_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO3D_IOMUX_GPIO3D7_SEL_SHIFT                 (14U)
#define GRF_GPIO3D_IOMUX_GPIO3D7_SEL_MASK                  (0x3U << GRF_GPIO3D_IOMUX_GPIO3D7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO1A_P */
#define GRF_GPIO1A_P_OFFSET                                (0x110)
#define GRF_GPIO1A_P_GPIO1A_P_SHIFT                        (0U)
#define GRF_GPIO1A_P_GPIO1A_P_MASK                         (0xFFFFU << GRF_GPIO1A_P_GPIO1A_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO1B_P */
#define GRF_GPIO1B_P_OFFSET                                (0x114)
#define GRF_GPIO1B_P_GPIO1B_P_SHIFT                        (0U)
#define GRF_GPIO1B_P_GPIO1B_P_MASK                         (0xFFFFU << GRF_GPIO1B_P_GPIO1B_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO1C_P */
#define GRF_GPIO1C_P_OFFSET                                (0x118)
#define GRF_GPIO1C_P_GPIO1C_P_SHIFT                        (0U)
#define GRF_GPIO1C_P_GPIO1C_P_MASK                         (0xFFFFU << GRF_GPIO1C_P_GPIO1C_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO1D_P */
#define GRF_GPIO1D_P_OFFSET                                (0x11C)
#define GRF_GPIO1D_P_GPIO1D_P_SHIFT                        (0U)
#define GRF_GPIO1D_P_GPIO1D_P_MASK                         (0xFFFFU << GRF_GPIO1D_P_GPIO1D_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO2A_P */
#define GRF_GPIO2A_P_OFFSET                                (0x120)
#define GRF_GPIO2A_P_GPIO2A_P_SHIFT                        (0U)
#define GRF_GPIO2A_P_GPIO2A_P_MASK                         (0xFFFFU << GRF_GPIO2A_P_GPIO2A_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO2B_P */
#define GRF_GPIO2B_P_OFFSET                                (0x124)
#define GRF_GPIO2B_P_GPIO2B_P_SHIFT                        (0U)
#define GRF_GPIO2B_P_GPIO2B_P_MASK                         (0xFFFFU << GRF_GPIO2B_P_GPIO2B_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO2C_P */
#define GRF_GPIO2C_P_OFFSET                                (0x128)
#define GRF_GPIO2C_P_GPIO2C_P_SHIFT                        (0U)
#define GRF_GPIO2C_P_GPIO2C_P_MASK                         (0xFFFFU << GRF_GPIO2C_P_GPIO2C_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO2D_P */
#define GRF_GPIO2D_P_OFFSET                                (0x12C)
#define GRF_GPIO2D_P_GPIO2D_P_SHIFT                        (0U)
#define GRF_GPIO2D_P_GPIO2D_P_MASK                         (0xFFFFU << GRF_GPIO2D_P_GPIO2D_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO3A_P */
#define GRF_GPIO3A_P_OFFSET                                (0x130)
#define GRF_GPIO3A_P_GPIO3A_P_SHIFT                        (0U)
#define GRF_GPIO3A_P_GPIO3A_P_MASK                         (0xFFFFU << GRF_GPIO3A_P_GPIO3A_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO3B_P */
#define GRF_GPIO3B_P_OFFSET                                (0x134)
#define GRF_GPIO3B_P_GPIO3B_P_SHIFT                        (0U)
#define GRF_GPIO3B_P_GPIO3B_P_MASK                         (0xFFFFU << GRF_GPIO3B_P_GPIO3B_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO3C_P */
#define GRF_GPIO3C_P_OFFSET                                (0x138)
#define GRF_GPIO3C_P_GPIO3C_P_SHIFT                        (0U)
#define GRF_GPIO3C_P_GPIO3C_P_MASK                         (0xFFFFU << GRF_GPIO3C_P_GPIO3C_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO3D_P */
#define GRF_GPIO3D_P_OFFSET                                (0x13C)
#define GRF_GPIO3D_P_GPIO3D_P_SHIFT                        (0U)
#define GRF_GPIO3D_P_GPIO3D_P_MASK                         (0xFFFFU << GRF_GPIO3D_P_GPIO3D_P_SHIFT)                     /* 0x0000FFFF */
/* GPIO1A_E */
#define GRF_GPIO1A_E_OFFSET                                (0x210)
#define GRF_GPIO1A_E_GPIO1A_E_SHIFT                        (0U)
#define GRF_GPIO1A_E_GPIO1A_E_MASK                         (0xFFFFU << GRF_GPIO1A_E_GPIO1A_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO1B_E */
#define GRF_GPIO1B_E_OFFSET                                (0x214)
#define GRF_GPIO1B_E_GPIO1B_E_SHIFT                        (0U)
#define GRF_GPIO1B_E_GPIO1B_E_MASK                         (0xFFFFU << GRF_GPIO1B_E_GPIO1B_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO1C_E */
#define GRF_GPIO1C_E_OFFSET                                (0x218)
#define GRF_GPIO1C_E_GPIO1C_E_SHIFT                        (0U)
#define GRF_GPIO1C_E_GPIO1C_E_MASK                         (0xFFFFU << GRF_GPIO1C_E_GPIO1C_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO1D_E */
#define GRF_GPIO1D_E_OFFSET                                (0x21C)
#define GRF_GPIO1D_E_GPIO1D_E_SHIFT                        (0U)
#define GRF_GPIO1D_E_GPIO1D_E_MASK                         (0xFFFFU << GRF_GPIO1D_E_GPIO1D_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO2A_E */
#define GRF_GPIO2A_E_OFFSET                                (0x220)
#define GRF_GPIO2A_E_GPIO2A_E_SHIFT                        (0U)
#define GRF_GPIO2A_E_GPIO2A_E_MASK                         (0xFFFFU << GRF_GPIO2A_E_GPIO2A_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO2B_E */
#define GRF_GPIO2B_E_OFFSET                                (0x224)
#define GRF_GPIO2B_E_GPIO2B_E_SHIFT                        (0U)
#define GRF_GPIO2B_E_GPIO2B_E_MASK                         (0xFFFFU << GRF_GPIO2B_E_GPIO2B_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO2C_E */
#define GRF_GPIO2C_E_OFFSET                                (0x228)
#define GRF_GPIO2C_E_GPIO2C_E_SHIFT                        (0U)
#define GRF_GPIO2C_E_GPIO2C_E_MASK                         (0xFFFFU << GRF_GPIO2C_E_GPIO2C_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO2D_E */
#define GRF_GPIO2D_E_OFFSET                                (0x22C)
#define GRF_GPIO2D_E_GPIO2D_E_SHIFT                        (0U)
#define GRF_GPIO2D_E_GPIO2D_E_MASK                         (0xFFFFU << GRF_GPIO2D_E_GPIO2D_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO3A_E */
#define GRF_GPIO3A_E_OFFSET                                (0x230)
#define GRF_GPIO3A_E_GPIO3A_E_SHIFT                        (0U)
#define GRF_GPIO3A_E_GPIO3A_E_MASK                         (0xFFFFU << GRF_GPIO3A_E_GPIO3A_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO3B_E */
#define GRF_GPIO3B_E_OFFSET                                (0x234)
#define GRF_GPIO3B_E_GPIO3B_E_SHIFT                        (0U)
#define GRF_GPIO3B_E_GPIO3B_E_MASK                         (0xFFFFU << GRF_GPIO3B_E_GPIO3B_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO3C_E */
#define GRF_GPIO3C_E_OFFSET                                (0x238)
#define GRF_GPIO3C_E_GPIO3C_E_SHIFT                        (0U)
#define GRF_GPIO3C_E_GPIO3C_E_MASK                         (0xFFFFU << GRF_GPIO3C_E_GPIO3C_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO3D_E */
#define GRF_GPIO3D_E_OFFSET                                (0x23C)
#define GRF_GPIO3D_E_GPIO3D_E_SHIFT                        (0U)
#define GRF_GPIO3D_E_GPIO3D_E_MASK                         (0xFFFFU << GRF_GPIO3D_E_GPIO3D_E_SHIFT)                     /* 0x0000FFFF */
/* GPIO1L_SR */
#define GRF_GPIO1L_SR_OFFSET                               (0x308)
#define GRF_GPIO1L_SR_GPIO1A_SR_SHIFT                      (0U)
#define GRF_GPIO1L_SR_GPIO1A_SR_MASK                       (0xFFU << GRF_GPIO1L_SR_GPIO1A_SR_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO1L_SR_GPIO1B_SR_SHIFT                      (8U)
#define GRF_GPIO1L_SR_GPIO1B_SR_MASK                       (0xFFU << GRF_GPIO1L_SR_GPIO1B_SR_SHIFT)                     /* 0x0000FF00 */
/* GPIO1H_SR */
#define GRF_GPIO1H_SR_OFFSET                               (0x30C)
#define GRF_GPIO1H_SR_GPIO1C_SR_SHIFT                      (0U)
#define GRF_GPIO1H_SR_GPIO1C_SR_MASK                       (0xFFU << GRF_GPIO1H_SR_GPIO1C_SR_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO1H_SR_GPIO1D_SR_SHIFT                      (8U)
#define GRF_GPIO1H_SR_GPIO1D_SR_MASK                       (0xFFU << GRF_GPIO1H_SR_GPIO1D_SR_SHIFT)                     /* 0x0000FF00 */
/* GPIO2L_SR */
#define GRF_GPIO2L_SR_OFFSET                               (0x310)
#define GRF_GPIO2L_SR_GPIO2A_SR_SHIFT                      (0U)
#define GRF_GPIO2L_SR_GPIO2A_SR_MASK                       (0xFFU << GRF_GPIO2L_SR_GPIO2A_SR_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO2L_SR_GPIO2B_SR_SHIFT                      (8U)
#define GRF_GPIO2L_SR_GPIO2B_SR_MASK                       (0xFFU << GRF_GPIO2L_SR_GPIO2B_SR_SHIFT)                     /* 0x0000FF00 */
/* GPIO2H_SR */
#define GRF_GPIO2H_SR_OFFSET                               (0x314)
#define GRF_GPIO2H_SR_GPIO2C_SR_SHIFT                      (0U)
#define GRF_GPIO2H_SR_GPIO2C_SR_MASK                       (0xFFU << GRF_GPIO2H_SR_GPIO2C_SR_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO2H_SR_GPIO2D_SR_SHIFT                      (8U)
#define GRF_GPIO2H_SR_GPIO2D_SR_MASK                       (0xFFU << GRF_GPIO2H_SR_GPIO2D_SR_SHIFT)                     /* 0x0000FF00 */
/* GPIO3L_SR */
#define GRF_GPIO3L_SR_OFFSET                               (0x318)
#define GRF_GPIO3L_SR_GPIO3A_SR_SHIFT                      (0U)
#define GRF_GPIO3L_SR_GPIO3A_SR_MASK                       (0xFFU << GRF_GPIO3L_SR_GPIO3A_SR_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO3L_SR_GPIO3B_SR_SHIFT                      (8U)
#define GRF_GPIO3L_SR_GPIO3B_SR_MASK                       (0xFFU << GRF_GPIO3L_SR_GPIO3B_SR_SHIFT)                     /* 0x0000FF00 */
/* GPIO3H_SR */
#define GRF_GPIO3H_SR_OFFSET                               (0x31C)
#define GRF_GPIO3H_SR_GPIO3C_SR_SHIFT                      (0U)
#define GRF_GPIO3H_SR_GPIO3C_SR_MASK                       (0xFFU << GRF_GPIO3H_SR_GPIO3C_SR_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO3H_SR_GPIO3D_SR_SHIFT                      (8U)
#define GRF_GPIO3H_SR_GPIO3D_SR_MASK                       (0xFFU << GRF_GPIO3H_SR_GPIO3D_SR_SHIFT)                     /* 0x0000FF00 */
/* GPIO1L_SMT */
#define GRF_GPIO1L_SMT_OFFSET                              (0x388)
#define GRF_GPIO1L_SMT_GPIO1A_SR_SHIFT                     (0U)
#define GRF_GPIO1L_SMT_GPIO1A_SR_MASK                      (0xFFU << GRF_GPIO1L_SMT_GPIO1A_SR_SHIFT)                    /* 0x000000FF */
#define GRF_GPIO1L_SMT_GPIO1B_SR_SHIFT                     (8U)
#define GRF_GPIO1L_SMT_GPIO1B_SR_MASK                      (0xFFU << GRF_GPIO1L_SMT_GPIO1B_SR_SHIFT)                    /* 0x0000FF00 */
/* GPIO1H_SMT */
#define GRF_GPIO1H_SMT_OFFSET                              (0x38C)
#define GRF_GPIO1H_SMT_GPIO1C_SR_SHIFT                     (0U)
#define GRF_GPIO1H_SMT_GPIO1C_SR_MASK                      (0xFFU << GRF_GPIO1H_SMT_GPIO1C_SR_SHIFT)                    /* 0x000000FF */
#define GRF_GPIO1H_SMT_GPIO1D_SR_SHIFT                     (8U)
#define GRF_GPIO1H_SMT_GPIO1D_SR_MASK                      (0xFFU << GRF_GPIO1H_SMT_GPIO1D_SR_SHIFT)                    /* 0x0000FF00 */
/* GPIO2L_SMT */
#define GRF_GPIO2L_SMT_OFFSET                              (0x390)
#define GRF_GPIO2L_SMT_GPIO2A_SR_SHIFT                     (0U)
#define GRF_GPIO2L_SMT_GPIO2A_SR_MASK                      (0xFFU << GRF_GPIO2L_SMT_GPIO2A_SR_SHIFT)                    /* 0x000000FF */
#define GRF_GPIO2L_SMT_GPIO2B_SR_SHIFT                     (8U)
#define GRF_GPIO2L_SMT_GPIO2B_SR_MASK                      (0xFFU << GRF_GPIO2L_SMT_GPIO2B_SR_SHIFT)                    /* 0x0000FF00 */
/* GPIO2H_SMT */
#define GRF_GPIO2H_SMT_OFFSET                              (0x394)
#define GRF_GPIO2H_SMT_GPIO2C_SR_SHIFT                     (0U)
#define GRF_GPIO2H_SMT_GPIO2C_SR_MASK                      (0xFFU << GRF_GPIO2H_SMT_GPIO2C_SR_SHIFT)                    /* 0x000000FF */
#define GRF_GPIO2H_SMT_GPIO2D_SR_SHIFT                     (8U)
#define GRF_GPIO2H_SMT_GPIO2D_SR_MASK                      (0xFFU << GRF_GPIO2H_SMT_GPIO2D_SR_SHIFT)                    /* 0x0000FF00 */
/* GPIO3L_SMT */
#define GRF_GPIO3L_SMT_OFFSET                              (0x398)
#define GRF_GPIO3L_SMT_GPIO3A_SR_SHIFT                     (0U)
#define GRF_GPIO3L_SMT_GPIO3A_SR_MASK                      (0xFFU << GRF_GPIO3L_SMT_GPIO3A_SR_SHIFT)                    /* 0x000000FF */
#define GRF_GPIO3L_SMT_GPIO3B_SR_SHIFT                     (8U)
#define GRF_GPIO3L_SMT_GPIO3B_SR_MASK                      (0xFFU << GRF_GPIO3L_SMT_GPIO3B_SR_SHIFT)                    /* 0x0000FF00 */
/* GPIO3H_SMT */
#define GRF_GPIO3H_SMT_OFFSET                              (0x39C)
#define GRF_GPIO3H_SMT_GPIO3C_SR_SHIFT                     (0U)
#define GRF_GPIO3H_SMT_GPIO3C_SR_MASK                      (0xFFU << GRF_GPIO3H_SMT_GPIO3C_SR_SHIFT)                    /* 0x000000FF */
#define GRF_GPIO3H_SMT_GPIO3D_SR_SHIFT                     (8U)
#define GRF_GPIO3H_SMT_GPIO3D_SR_MASK                      (0xFFU << GRF_GPIO3H_SMT_GPIO3D_SR_SHIFT)                    /* 0x0000FF00 */
/* SOC_CON0 */
#define GRF_SOC_CON0_OFFSET                                (0x400)
#define GRF_SOC_CON0_UPCTL_LP_RESET_MODE_SHIFT             (0U)
#define GRF_SOC_CON0_UPCTL_LP_RESET_MODE_MASK              (0x1U << GRF_SOC_CON0_UPCTL_LP_RESET_MODE_SHIFT)             /* 0x00000001 */
#define GRF_SOC_CON0_GRF_CON_UPCTL_ABURSTINT_SHIFT         (1U)
#define GRF_SOC_CON0_GRF_CON_UPCTL_ABURSTINT_MASK          (0x1U << GRF_SOC_CON0_GRF_CON_UPCTL_ABURSTINT_SHIFT)         /* 0x00000002 */
#define GRF_SOC_CON0_GRF_CON_UPCTL_ANFIFO_SHIFT            (2U)
#define GRF_SOC_CON0_GRF_CON_UPCTL_ANFIFO_MASK             (0x1U << GRF_SOC_CON0_GRF_CON_UPCTL_ANFIFO_SHIFT)            /* 0x00000004 */
#define GRF_SOC_CON0_UPCTL_C_ACTIVE_IN_SHIFT               (3U)
#define GRF_SOC_CON0_UPCTL_C_ACTIVE_IN_MASK                (0x1U << GRF_SOC_CON0_UPCTL_C_ACTIVE_IN_SHIFT)               /* 0x00000008 */
#define GRF_SOC_CON0_GRF_CON_MSCH_MAINDDR3_SHIFT           (4U)
#define GRF_SOC_CON0_GRF_CON_MSCH_MAINDDR3_MASK            (0x1U << GRF_SOC_CON0_GRF_CON_MSCH_MAINDDR3_SHIFT)           /* 0x00000010 */
#define GRF_SOC_CON0_GRF_CON_MSCH_MAINPARTIALPOP_SHIFT     (5U)
#define GRF_SOC_CON0_GRF_CON_MSCH_MAINPARTIALPOP_MASK      (0x1U << GRF_SOC_CON0_GRF_CON_MSCH_MAINPARTIALPOP_SHIFT)     /* 0x00000020 */
#define GRF_SOC_CON0_GRF_CON_DDR_16BIT_EN_SHIFT            (6U)
#define GRF_SOC_CON0_GRF_CON_DDR_16BIT_EN_MASK             (0x1U << GRF_SOC_CON0_GRF_CON_DDR_16BIT_EN_SHIFT)            /* 0x00000040 */
#define GRF_SOC_CON0_GRF_CON_ACODEC_AD2DA_LOOP_SHIFT       (7U)
#define GRF_SOC_CON0_GRF_CON_ACODEC_AD2DA_LOOP_MASK        (0x1U << GRF_SOC_CON0_GRF_CON_ACODEC_AD2DA_LOOP_SHIFT)       /* 0x00000080 */
#define GRF_SOC_CON0_GRF_CON_HOST0_ARB_PAUSE_SHIFT         (8U)
#define GRF_SOC_CON0_GRF_CON_HOST0_ARB_PAUSE_MASK          (0x1U << GRF_SOC_CON0_GRF_CON_HOST0_ARB_PAUSE_SHIFT)         /* 0x00000100 */
#define GRF_SOC_CON0_REMAP_SHIFT                           (9U)
#define GRF_SOC_CON0_REMAP_MASK                            (0x1U << GRF_SOC_CON0_REMAP_SHIFT)                           /* 0x00000200 */
#define GRF_SOC_CON0_GRF_FORCE_DEBUG_SHIFT                 (11U)
#define GRF_SOC_CON0_GRF_FORCE_DEBUG_MASK                  (0x1U << GRF_SOC_CON0_GRF_FORCE_DEBUG_SHIFT)                 /* 0x00000800 */
/* SOC_CON1 */
#define GRF_SOC_CON1_OFFSET                                (0x404)
#define GRF_SOC_CON1_GRF_CON_BUS_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT (0U)
#define GRF_SOC_CON1_GRF_CON_BUS_REQ_MSCH_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_BUS_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000001 */
#define GRF_SOC_CON1_GRF_CON_VDEC_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT (1U)
#define GRF_SOC_CON1_GRF_CON_VDEC_REQ_MSCH_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_VDEC_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000002 */
#define GRF_SOC_CON1_GRF_CON_VENC_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT (2U)
#define GRF_SOC_CON1_GRF_CON_VENC_REQ_MSCH_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_VENC_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000004 */
#define GRF_SOC_CON1_GRF_CON_VIO0_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT (3U)
#define GRF_SOC_CON1_GRF_CON_VIO0_REQ_MSCH_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_VIO0_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000008 */
#define GRF_SOC_CON1_GRF_CON_VIO1_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT (4U)
#define GRF_SOC_CON1_GRF_CON_VIO1_REQ_MSCH_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_VIO1_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000010 */
#define GRF_SOC_CON1_GRF_CON_CORE_BUS_REQ_PWRDISCTARGPWRSTALL_SHIFT (5U)
#define GRF_SOC_CON1_GRF_CON_CORE_BUS_REQ_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_CORE_BUS_REQ_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000020 */
#define GRF_SOC_CON1_GRF_CON_CORE_MSCH_REQ_PWRDISCTARGPWRSTALL_SHIFT (6U)
#define GRF_SOC_CON1_GRF_CON_CORE_MSCH_REQ_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_CORE_MSCH_REQ_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000040 */
#define GRF_SOC_CON1_GRF_CON_DSP_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT (7U)
#define GRF_SOC_CON1_GRF_CON_DSP_REQ_MSCH_PWRDISCTARGPWRSTALL_MASK (0x1U << GRF_SOC_CON1_GRF_CON_DSP_REQ_MSCH_PWRDISCTARGPWRSTALL_SHIFT) /* 0x00000080 */
#define GRF_SOC_CON1_GRF_IO_VSEL_SHIFT                     (11U)
#define GRF_SOC_CON1_GRF_IO_VSEL_MASK                      (0x1FU << GRF_SOC_CON1_GRF_IO_VSEL_SHIFT)                    /* 0x0000F800 */
/* SOC_CON2 */
#define GRF_SOC_CON2_OFFSET                                (0x408)
#define GRF_SOC_CON2_GRF_UART_CTS_SEL_SHIFT                (0U)
#define GRF_SOC_CON2_GRF_UART_CTS_SEL_MASK                 (0x7U << GRF_SOC_CON2_GRF_UART_CTS_SEL_SHIFT)                /* 0x00000007 */
#define GRF_SOC_CON2_GRF_UART_RTS_SEL_SHIFT                (3U)
#define GRF_SOC_CON2_GRF_UART_RTS_SEL_MASK                 (0x7U << GRF_SOC_CON2_GRF_UART_RTS_SEL_SHIFT)                /* 0x00000038 */
#define GRF_SOC_CON2_GRF_CON_HDMII2C_CTRL_MUX_SHIFT        (10U)
#define GRF_SOC_CON2_GRF_CON_HDMII2C_CTRL_MUX_MASK         (0x1U << GRF_SOC_CON2_GRF_CON_HDMII2C_CTRL_MUX_SHIFT)        /* 0x00000400 */
#define GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO1_OEN_SHIFT      (11U)
#define GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO1_OEN_MASK       (0x1U << GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO1_OEN_SHIFT)      /* 0x00000800 */
#define GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO2_OEN_SHIFT      (12U)
#define GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO2_OEN_MASK       (0x1U << GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO2_OEN_SHIFT)      /* 0x00001000 */
#define GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO3_OEN_SHIFT      (13U)
#define GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO3_OEN_MASK       (0x1U << GRF_SOC_CON2_GRF_CON_I2S0_8CH_SDIO3_OEN_SHIFT)      /* 0x00002000 */
#define GRF_SOC_CON2_GRF_CON_USBPHY_UART_IOMUX_SHIFT       (15U)
#define GRF_SOC_CON2_GRF_CON_USBPHY_UART_IOMUX_MASK        (0x1U << GRF_SOC_CON2_GRF_CON_USBPHY_UART_IOMUX_SHIFT)       /* 0x00008000 */
/* SOC_CON3 */
#define GRF_SOC_CON3_OFFSET                                (0x40C)
#define GRF_SOC_CON3_GRF_CON_TSADC_CLK_SEL_SHIFT           (0U)
#define GRF_SOC_CON3_GRF_CON_TSADC_CLK_SEL_MASK            (0x1U << GRF_SOC_CON3_GRF_CON_TSADC_CLK_SEL_SHIFT)           /* 0x00000001 */
#define GRF_SOC_CON3_GRF_CON_TSADC_DIG_BYPASS_SHIFT        (1U)
#define GRF_SOC_CON3_GRF_CON_TSADC_DIG_BYPASS_MASK         (0x1U << GRF_SOC_CON3_GRF_CON_TSADC_DIG_BYPASS_SHIFT)        /* 0x00000002 */
#define GRF_SOC_CON3_GRF_CON_TSADC_TSEN_PD_SHIFT           (2U)
#define GRF_SOC_CON3_GRF_CON_TSADC_TSEN_PD_MASK            (0x1U << GRF_SOC_CON3_GRF_CON_TSADC_TSEN_PD_SHIFT)           /* 0x00000004 */
#define GRF_SOC_CON3_GRF_CON_TSADC_CHSEL_SHIFT             (6U)
#define GRF_SOC_CON3_GRF_CON_TSADC_CHSEL_MASK              (0x1U << GRF_SOC_CON3_GRF_CON_TSADC_CHSEL_SHIFT)             /* 0x00000040 */
/* SOC_CON4 */
#define GRF_SOC_CON4_OFFSET                                (0x410)
#define GRF_SOC_CON4_GRF_CON_CIF0_DATA_SEL_SHIFT           (0U)
#define GRF_SOC_CON4_GRF_CON_CIF0_DATA_SEL_MASK            (0x3U << GRF_SOC_CON4_GRF_CON_CIF0_DATA_SEL_SHIFT)           /* 0x00000003 */
#define GRF_SOC_CON4_GRF_CON_DISABLE_ISP_SHIFT             (2U)
#define GRF_SOC_CON4_GRF_CON_DISABLE_ISP_MASK              (0x1U << GRF_SOC_CON4_GRF_CON_DISABLE_ISP_SHIFT)             /* 0x00000004 */
#define GRF_SOC_CON4_GRF_CON_VOP_RGB_DCLK_REV_SEL_SHIFT    (4U)
#define GRF_SOC_CON4_GRF_CON_VOP_RGB_DCLK_REV_SEL_MASK     (0x1U << GRF_SOC_CON4_GRF_CON_VOP_RGB_DCLK_REV_SEL_SHIFT)    /* 0x00000010 */
#define GRF_SOC_CON4_GRF_VOP_DCLK_SEL_SHIFT                (5U)
#define GRF_SOC_CON4_GRF_VOP_DCLK_SEL_MASK                 (0x1U << GRF_SOC_CON4_GRF_VOP_DCLK_SEL_SHIFT)                /* 0x00000020 */
#define GRF_SOC_CON4_GRF_CON_DSI0_DPISHUTDN_SHIFT          (6U)
#define GRF_SOC_CON4_GRF_CON_DSI0_DPISHUTDN_MASK           (0x1U << GRF_SOC_CON4_GRF_CON_DSI0_DPISHUTDN_SHIFT)          /* 0x00000040 */
#define GRF_SOC_CON4_GRF_CON_DSI0_DPICOLORM_SHIFT          (7U)
#define GRF_SOC_CON4_GRF_CON_DSI0_DPICOLORM_MASK           (0x1U << GRF_SOC_CON4_GRF_CON_DSI0_DPICOLORM_SHIFT)          /* 0x00000080 */
#define GRF_SOC_CON4_GRF_CON_DSI0_DPIUPDATECFG_SHIFT       (8U)
#define GRF_SOC_CON4_GRF_CON_DSI0_DPIUPDATECFG_MASK        (0x1U << GRF_SOC_CON4_GRF_CON_DSI0_DPIUPDATECFG_SHIFT)       /* 0x00000100 */
#define GRF_SOC_CON4_GRF_CON_DPHY_RX_CLK_INV_SEL_SHIFT     (9U)
#define GRF_SOC_CON4_GRF_CON_DPHY_RX_CLK_INV_SEL_MASK      (0x1U << GRF_SOC_CON4_GRF_CON_DPHY_RX_CLK_INV_SEL_SHIFT)     /* 0x00000200 */
#define GRF_SOC_CON4_GRF_CON_HDMI_I2C_FORCE_SCL_SHIFT      (11U)
#define GRF_SOC_CON4_GRF_CON_HDMI_I2C_FORCE_SCL_MASK       (0x1U << GRF_SOC_CON4_GRF_CON_HDMI_I2C_FORCE_SCL_SHIFT)      /* 0x00000800 */
#define GRF_SOC_CON4_GRF_CON_HDMI_I2C_FORCE_SDA_SHIFT      (12U)
#define GRF_SOC_CON4_GRF_CON_HDMI_I2C_FORCE_SDA_MASK       (0x1U << GRF_SOC_CON4_GRF_CON_HDMI_I2C_FORCE_SDA_SHIFT)      /* 0x00001000 */
#define GRF_SOC_CON4_GRF_HDMIP_PDATA_EN_SHIFT              (13U)
#define GRF_SOC_CON4_GRF_HDMIP_PDATA_EN_MASK               (0x1U << GRF_SOC_CON4_GRF_HDMIP_PDATA_EN_SHIFT)              /* 0x00002000 */
#define GRF_SOC_CON4_GRF_HDMIPHY_PLL_PD_SHIFT              (14U)
#define GRF_SOC_CON4_GRF_HDMIPHY_PLL_PD_MASK               (0x1U << GRF_SOC_CON4_GRF_HDMIPHY_PLL_PD_SHIFT)              /* 0x00004000 */
#define GRF_SOC_CON4_GRF_HDMI_I2C_MSK_SHIFT                (15U)
#define GRF_SOC_CON4_GRF_HDMI_I2C_MSK_MASK                 (0x1U << GRF_SOC_CON4_GRF_HDMI_I2C_MSK_SHIFT)                /* 0x00008000 */
/* SOC_CON5 */
#define GRF_SOC_CON5_OFFSET                                (0x414)
#define GRF_SOC_CON5_GRF_CON_DPHY_RX_FORCERXMODE_SHIFT     (0U)
#define GRF_SOC_CON5_GRF_CON_DPHY_RX_FORCERXMODE_MASK      (0xFU << GRF_SOC_CON5_GRF_CON_DPHY_RX_FORCERXMODE_SHIFT)     /* 0x0000000F */
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE0_TURNDISABLE_SHIFT (4U)
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE0_TURNDISABLE_MASK (0x1U << GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE0_TURNDISABLE_SHIFT) /* 0x00000010 */
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_FORCERXMODE_SHIFT     (5U)
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_FORCERXMODE_MASK      (0x1U << GRF_SOC_CON5_GRF_CON_DSI_PHY_FORCERXMODE_SHIFT)     /* 0x00000020 */
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE0_FRCTXSTPM_SHIFT (6U)
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE0_FRCTXSTPM_MASK  (0x1U << GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE0_FRCTXSTPM_SHIFT) /* 0x00000040 */
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE1_FRCTXSTPM_SHIFT (7U)
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE1_FRCTXSTPM_MASK  (0x1U << GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE1_FRCTXSTPM_SHIFT) /* 0x00000080 */
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE2_FRCTXSTPM_SHIFT (8U)
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE2_FRCTXSTPM_MASK  (0x1U << GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE2_FRCTXSTPM_SHIFT) /* 0x00000100 */
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE3_FRCTXSTPM_SHIFT (9U)
#define GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE3_FRCTXSTPM_MASK  (0x1U << GRF_SOC_CON5_GRF_CON_DSI_PHY_LANE3_FRCTXSTPM_SHIFT) /* 0x00000200 */
/* SOC_CON6 */
#define GRF_SOC_CON6_OFFSET                                (0x418)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_0_SHIFT    (0U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_0_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_0_SHIFT)    /* 0x00000003 */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_1_SHIFT    (2U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_1_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_1_SHIFT)    /* 0x0000000C */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_2_SHIFT    (4U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_2_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_2_SHIFT)    /* 0x00000030 */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_3_SHIFT    (6U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_3_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_3_SHIFT)    /* 0x000000C0 */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_4_SHIFT    (8U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_4_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_4_SHIFT)    /* 0x00000300 */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_5_SHIFT    (10U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_5_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_5_SHIFT)    /* 0x00000C00 */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_6_SHIFT    (12U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_6_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_6_SHIFT)    /* 0x00003000 */
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_7_SHIFT    (14U)
#define GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_7_MASK     (0x3U << GRF_SOC_CON6_GRF_CON_MIPI_TTL_IOMUX_SEL_7_SHIFT)    /* 0x0000C000 */
/* SOC_CON7 */
#define GRF_SOC_CON7_OFFSET                                (0x41C)
#define GRF_SOC_CON7_GRF_CON_MIPI_TTL_IOMUX_SEL_8_SHIFT    (0U)
#define GRF_SOC_CON7_GRF_CON_MIPI_TTL_IOMUX_SEL_8_MASK     (0x3U << GRF_SOC_CON7_GRF_CON_MIPI_TTL_IOMUX_SEL_8_SHIFT)    /* 0x00000003 */
#define GRF_SOC_CON7_GRF_CON_MIPI_TTL_IOMUX_SEL_9_SHIFT    (2U)
#define GRF_SOC_CON7_GRF_CON_MIPI_TTL_IOMUX_SEL_9_MASK     (0x3U << GRF_SOC_CON7_GRF_CON_MIPI_TTL_IOMUX_SEL_9_SHIFT)    /* 0x0000000C */
/* SOC_CON8 */
#define GRF_SOC_CON8_OFFSET                                (0x420)
#define GRF_SOC_CON8_GRF_CON_TSADC_ANA_REG_SHIFT           (0U)
#define GRF_SOC_CON8_GRF_CON_TSADC_ANA_REG_MASK            (0xFFFFU << GRF_SOC_CON8_GRF_CON_TSADC_ANA_REG_SHIFT)        /* 0x0000FFFF */
/* SOC_CON9 */
#define GRF_SOC_CON9_OFFSET                                (0x424)
#define GRF_SOC_CON9_CIF1TO4_CHD_ID_BYPASS_SHIFT           (0U)
#define GRF_SOC_CON9_CIF1TO4_CHD_ID_BYPASS_MASK            (0x1U << GRF_SOC_CON9_CIF1TO4_CHD_ID_BYPASS_SHIFT)           /* 0x00000001 */
#define GRF_SOC_CON9_CIF1TO4_CHC_ID_BYPASS_SHIFT           (1U)
#define GRF_SOC_CON9_CIF1TO4_CHC_ID_BYPASS_MASK            (0x1U << GRF_SOC_CON9_CIF1TO4_CHC_ID_BYPASS_SHIFT)           /* 0x00000002 */
#define GRF_SOC_CON9_CIF1TO4_CHB_ID_BYPASS_SHIFT           (2U)
#define GRF_SOC_CON9_CIF1TO4_CHB_ID_BYPASS_MASK            (0x1U << GRF_SOC_CON9_CIF1TO4_CHB_ID_BYPASS_SHIFT)           /* 0x00000004 */
#define GRF_SOC_CON9_CIF1TO4_CHA_ID_BYPASS_SHIFT           (3U)
#define GRF_SOC_CON9_CIF1TO4_CHA_ID_BYPASS_MASK            (0x1U << GRF_SOC_CON9_CIF1TO4_CHA_ID_BYPASS_SHIFT)           /* 0x00000008 */
#define GRF_SOC_CON9_CIF1TO4_CHD_ID_SEL_SHIFT              (4U)
#define GRF_SOC_CON9_CIF1TO4_CHD_ID_SEL_MASK               (0x3U << GRF_SOC_CON9_CIF1TO4_CHD_ID_SEL_SHIFT)              /* 0x00000030 */
#define GRF_SOC_CON9_CIF1TO4_CHC_ID_SEL_SHIFT              (6U)
#define GRF_SOC_CON9_CIF1TO4_CHC_ID_SEL_MASK               (0x3U << GRF_SOC_CON9_CIF1TO4_CHC_ID_SEL_SHIFT)              /* 0x000000C0 */
#define GRF_SOC_CON9_CIF1TO4_CHB_ID_SEL_SHIFT              (8U)
#define GRF_SOC_CON9_CIF1TO4_CHB_ID_SEL_MASK               (0x3U << GRF_SOC_CON9_CIF1TO4_CHB_ID_SEL_SHIFT)              /* 0x00000300 */
#define GRF_SOC_CON9_CIF1TO4_CHA_ID_SEL_SHIFT              (10U)
#define GRF_SOC_CON9_CIF1TO4_CHA_ID_SEL_MASK               (0x3U << GRF_SOC_CON9_CIF1TO4_CHA_ID_SEL_SHIFT)              /* 0x00000C00 */
#define GRF_SOC_CON9_CIF1TO4_CH_NUM_SHIFT                  (12U)
#define GRF_SOC_CON9_CIF1TO4_CH_NUM_MASK                   (0x3U << GRF_SOC_CON9_CIF1TO4_CH_NUM_SHIFT)                  /* 0x00003000 */
#define GRF_SOC_CON9_CIF1TO4_EN_SHIFT                      (15U)
#define GRF_SOC_CON9_CIF1TO4_EN_MASK                       (0x1U << GRF_SOC_CON9_CIF1TO4_EN_SHIFT)                      /* 0x00008000 */
/* SOC_CON10 */
#define GRF_SOC_CON10_OFFSET                               (0x428)
#define GRF_SOC_CON10_VADC_GAIN_SHIFT                      (0U)
#define GRF_SOC_CON10_VADC_GAIN_MASK                       (0xFFU << GRF_SOC_CON10_VADC_GAIN_SHIFT)                     /* 0x000000FF */
#define GRF_SOC_CON10_VADC_ICLMP_CTL_SHIFT                 (8U)
#define GRF_SOC_CON10_VADC_ICLMP_CTL_MASK                  (0xFU << GRF_SOC_CON10_VADC_ICLMP_CTL_SHIFT)                 /* 0x00000F00 */
#define GRF_SOC_CON10_VADC_LPF_BW_SHIFT                    (12U)
#define GRF_SOC_CON10_VADC_LPF_BW_MASK                     (0x3U << GRF_SOC_CON10_VADC_LPF_BW_SHIFT)                    /* 0x00003000 */
#define GRF_SOC_CON10_VADC_BYPASS_PRE_SHIFT                (14U)
#define GRF_SOC_CON10_VADC_BYPASS_PRE_MASK                 (0x1U << GRF_SOC_CON10_VADC_BYPASS_PRE_SHIFT)                /* 0x00004000 */
/* SOC_CON11 */
#define GRF_SOC_CON11_OFFSET                               (0x42C)
#define GRF_SOC_CON11_VADC_DN_SHIFT                        (0U)
#define GRF_SOC_CON11_VADC_DN_MASK                         (0x1U << GRF_SOC_CON11_VADC_DN_SHIFT)                        /* 0x00000001 */
#define GRF_SOC_CON11_VADC_UP_SHIFT                        (1U)
#define GRF_SOC_CON11_VADC_UP_MASK                         (0x1U << GRF_SOC_CON11_VADC_UP_SHIFT)                        /* 0x00000002 */
#define GRF_SOC_CON11_VADC_FIXDC_INP_EN_SHIFT              (2U)
#define GRF_SOC_CON11_VADC_FIXDC_INP_EN_MASK               (0x1U << GRF_SOC_CON11_VADC_FIXDC_INP_EN_SHIFT)              /* 0x00000004 */
#define GRF_SOC_CON11_VADC_PD_ADC_SHIFT                    (3U)
#define GRF_SOC_CON11_VADC_PD_ADC_MASK                     (0x1U << GRF_SOC_CON11_VADC_PD_ADC_SHIFT)                    /* 0x00000008 */
#define GRF_SOC_CON11_VADC_PD_BG_SHIFT                     (4U)
#define GRF_SOC_CON11_VADC_PD_BG_MASK                      (0x1U << GRF_SOC_CON11_VADC_PD_BG_SHIFT)                     /* 0x00000010 */
#define GRF_SOC_CON11_VADC_PD_CLMP_SHIFT                   (5U)
#define GRF_SOC_CON11_VADC_PD_CLMP_MASK                    (0x1U << GRF_SOC_CON11_VADC_PD_CLMP_SHIFT)                   /* 0x00000020 */
#define GRF_SOC_CON11_VADC_PD_PRE_SHIFT                    (6U)
#define GRF_SOC_CON11_VADC_PD_PRE_MASK                     (0x1U << GRF_SOC_CON11_VADC_PD_PRE_SHIFT)                    /* 0x00000040 */
#define GRF_SOC_CON11_VADC_CLMP_INN_AON_SHIFT              (7U)
#define GRF_SOC_CON11_VADC_CLMP_INN_AON_MASK               (0x1U << GRF_SOC_CON11_VADC_CLMP_INN_AON_SHIFT)              /* 0x00000080 */
#define GRF_SOC_CON11_VADC_CLMP_INN_EN_SHIFT               (8U)
#define GRF_SOC_CON11_VADC_CLMP_INN_EN_MASK                (0x1U << GRF_SOC_CON11_VADC_CLMP_INN_EN_SHIFT)               /* 0x00000100 */
#define GRF_SOC_CON11_VADC_CLMP_INN_SHIFT                  (9U)
#define GRF_SOC_CON11_VADC_CLMP_INN_MASK                   (0x1U << GRF_SOC_CON11_VADC_CLMP_INN_SHIFT)                  /* 0x00000200 */
#define GRF_SOC_CON11_VADC_CLMP_SEL_SHIFT                  (10U)
#define GRF_SOC_CON11_VADC_CLMP_SEL_MASK                   (0x1U << GRF_SOC_CON11_VADC_CLMP_SEL_SHIFT)                  /* 0x00000400 */
/* SOC_STATUS0 */
#define GRF_SOC_STATUS0_OFFSET                             (0x480)
#define GRF_SOC_STATUS0_ARM_PLL_LOCK_SHIFT                 (0U)
#define GRF_SOC_STATUS0_ARM_PLL_LOCK_MASK                  (0x1U << GRF_SOC_STATUS0_ARM_PLL_LOCK_SHIFT)                 /* 0x00000001 */
#define GRF_SOC_STATUS0_DSP_PLL_LOCK_SHIFT                 (1U)
#define GRF_SOC_STATUS0_DSP_PLL_LOCK_MASK                  (0x1U << GRF_SOC_STATUS0_DSP_PLL_LOCK_SHIFT)                 /* 0x00000002 */
#define GRF_SOC_STATUS0_GENERAL_PLL_LOCK1_SHIFT            (2U)
#define GRF_SOC_STATUS0_GENERAL_PLL_LOCK1_MASK             (0x1U << GRF_SOC_STATUS0_GENERAL_PLL_LOCK1_SHIFT)            /* 0x00000004 */
#define GRF_SOC_STATUS0_VDAC_DISPDET_SHIFT                 (3U)
#define GRF_SOC_STATUS0_VDAC_DISPDET_MASK                  (0x1U << GRF_SOC_STATUS0_VDAC_DISPDET_SHIFT)                 /* 0x00000008 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS0_SHIFT             (4U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS0_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS0_SHIFT)             /* 0x00000010 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS1_SHIFT             (5U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS1_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS1_SHIFT)             /* 0x00000020 */
#define GRF_SOC_STATUS0_GMAC_PORTSELECT_SHIFT              (6U)
#define GRF_SOC_STATUS0_GMAC_PORTSELECT_MASK               (0x1U << GRF_SOC_STATUS0_GMAC_PORTSELECT_SHIFT)              /* 0x00000040 */
#define GRF_SOC_STATUS0_VOP_DMA_FINISH_SHIFT               (7U)
#define GRF_SOC_STATUS0_VOP_DMA_FINISH_MASK                (0x1U << GRF_SOC_STATUS0_VOP_DMA_FINISH_SHIFT)               /* 0x00000080 */
#define GRF_SOC_STATUS0_NANDC_MASTERIDLE_SHIFT             (8U)
#define GRF_SOC_STATUS0_NANDC_MASTERIDLE_MASK              (0x1U << GRF_SOC_STATUS0_NANDC_MASTERIDLE_SHIFT)             /* 0x00000100 */
#define GRF_SOC_STATUS0_OTG_UTMI_VBUSVALID_SHIFT           (9U)
#define GRF_SOC_STATUS0_OTG_UTMI_VBUSVALID_MASK            (0x1U << GRF_SOC_STATUS0_OTG_UTMI_VBUSVALID_SHIFT)           /* 0x00000200 */
#define GRF_SOC_STATUS0_OTG_BVALID_SHIFT                   (10U)
#define GRF_SOC_STATUS0_OTG_BVALID_MASK                    (0x1U << GRF_SOC_STATUS0_OTG_BVALID_SHIFT)                   /* 0x00000400 */
#define GRF_SOC_STATUS0_OTG_UTMI_LINSTATE_SHIFT            (11U)
#define GRF_SOC_STATUS0_OTG_UTMI_LINSTATE_MASK             (0x3U << GRF_SOC_STATUS0_OTG_UTMI_LINSTATE_SHIFT)            /* 0x00001800 */
#define GRF_SOC_STATUS0_OTG_UTMI_IDDIG_SHIFT               (13U)
#define GRF_SOC_STATUS0_OTG_UTMI_IDDIG_MASK                (0x1U << GRF_SOC_STATUS0_OTG_UTMI_IDDIG_SHIFT)               /* 0x00002000 */
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_0_SHIFT      (14U)
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_0_MASK       (0x1U << GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_0_SHIFT)      /* 0x00004000 */
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_1_SHIFT      (15U)
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_1_MASK       (0x1U << GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_1_SHIFT)      /* 0x00008000 */
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_2_SHIFT      (16U)
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_2_MASK       (0x1U << GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_2_SHIFT)      /* 0x00010000 */
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_3_SHIFT      (17U)
#define GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_3_MASK       (0x1U << GRF_SOC_STATUS0_DPHY_RX_ULPSACTIVENOT_3_SHIFT)      /* 0x00020000 */
#define GRF_SOC_STATUS0_DPHY_RX_DIRECTION_SHIFT            (18U)
#define GRF_SOC_STATUS0_DPHY_RX_DIRECTION_MASK             (0x1U << GRF_SOC_STATUS0_DPHY_RX_DIRECTION_SHIFT)            /* 0x00040000 */
#define GRF_SOC_STATUS0_DPHY_RX_RXCLKACTIVEHS_SHIFT        (19U)
#define GRF_SOC_STATUS0_DPHY_RX_RXCLKACTIVEHS_MASK         (0x1U << GRF_SOC_STATUS0_DPHY_RX_RXCLKACTIVEHS_SHIFT)        /* 0x00080000 */
#define GRF_SOC_STATUS0_ACODEC_MASTER_EN_SHIFT             (20U)
#define GRF_SOC_STATUS0_ACODEC_MASTER_EN_MASK              (0x1U << GRF_SOC_STATUS0_ACODEC_MASTER_EN_SHIFT)             /* 0x00100000 */
#define GRF_SOC_STATUS0_STANDBYWFIL2_SHIFT                 (21U)
#define GRF_SOC_STATUS0_STANDBYWFIL2_MASK                  (0x1U << GRF_SOC_STATUS0_STANDBYWFIL2_SHIFT)                 /* 0x00200000 */
#define GRF_SOC_STATUS0_STANDBY_WFI_SHIFT                  (22U)
#define GRF_SOC_STATUS0_STANDBY_WFI_MASK                   (0x3U << GRF_SOC_STATUS0_STANDBY_WFI_SHIFT)                  /* 0x00C00000 */
#define GRF_SOC_STATUS0_SMPNAMP_SHIFT                      (24U)
#define GRF_SOC_STATUS0_SMPNAMP_MASK                       (0xFU << GRF_SOC_STATUS0_SMPNAMP_SHIFT)                      /* 0x0F000000 */
#define GRF_SOC_STATUS0_EVENTO_RISING_EDGE_SHIFT           (28U)
#define GRF_SOC_STATUS0_EVENTO_RISING_EDGE_MASK            (0x1U << GRF_SOC_STATUS0_EVENTO_RISING_EDGE_SHIFT)           /* 0x10000000 */
#define GRF_SOC_STATUS0_JTAGTOP_SHIFT                      (29U)
#define GRF_SOC_STATUS0_JTAGTOP_MASK                       (0x1U << GRF_SOC_STATUS0_JTAGTOP_SHIFT)                      /* 0x20000000 */
#define GRF_SOC_STATUS0_JTAGNSW_SHIFT                      (30U)
#define GRF_SOC_STATUS0_JTAGNSW_MASK                       (0x1U << GRF_SOC_STATUS0_JTAGNSW_SHIFT)                      /* 0x40000000 */
/* SOC_STATUS1 */
#define GRF_SOC_STATUS1_OFFSET                             (0x484)
#define GRF_SOC_STATUS1_CIF1TO4_CHA_FIELD_SHIFT            (0U)
#define GRF_SOC_STATUS1_CIF1TO4_CHA_FIELD_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHA_FIELD_SHIFT)            /* 0x00000001 */
#define GRF_SOC_STATUS1_CIF1TO4_CHB_FIELD_SHIFT            (1U)
#define GRF_SOC_STATUS1_CIF1TO4_CHB_FIELD_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHB_FIELD_SHIFT)            /* 0x00000002 */
#define GRF_SOC_STATUS1_CIF1TO4_CHC_FIELD_SHIFT            (2U)
#define GRF_SOC_STATUS1_CIF1TO4_CHC_FIELD_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHC_FIELD_SHIFT)            /* 0x00000004 */
#define GRF_SOC_STATUS1_CIF1TO4_CHD_FIELD_SHIFT            (3U)
#define GRF_SOC_STATUS1_CIF1TO4_CHD_FIELD_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHD_FIELD_SHIFT)            /* 0x00000008 */
#define GRF_SOC_STATUS1_CIF1TO4_CHA_VALID_SHIFT            (4U)
#define GRF_SOC_STATUS1_CIF1TO4_CHA_VALID_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHA_VALID_SHIFT)            /* 0x00000010 */
#define GRF_SOC_STATUS1_CIF1TO4_CHB_VALID_SHIFT            (5U)
#define GRF_SOC_STATUS1_CIF1TO4_CHB_VALID_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHB_VALID_SHIFT)            /* 0x00000020 */
#define GRF_SOC_STATUS1_CIF1TO4_CHC_VALID_SHIFT            (6U)
#define GRF_SOC_STATUS1_CIF1TO4_CHC_VALID_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHC_VALID_SHIFT)            /* 0x00000040 */
#define GRF_SOC_STATUS1_CIF1TO4_CHD_VALID_SHIFT            (7U)
#define GRF_SOC_STATUS1_CIF1TO4_CHD_VALID_MASK             (0x1U << GRF_SOC_STATUS1_CIF1TO4_CHD_VALID_SHIFT)            /* 0x00000080 */
/* CPU_CON0 */
#define GRF_CPU_CON0_OFFSET                                (0x500)
#define GRF_CPU_CON0_GRF_CON_DBGEN_SHIFT                   (0U)
#define GRF_CPU_CON0_GRF_CON_DBGEN_MASK                    (0x1U << GRF_CPU_CON0_GRF_CON_DBGEN_SHIFT)                   /* 0x00000001 */
#define GRF_CPU_CON0_GRF_CON_SPIDEN_SHIFT                  (1U)
#define GRF_CPU_CON0_GRF_CON_SPIDEN_MASK                   (0x1U << GRF_CPU_CON0_GRF_CON_SPIDEN_SHIFT)                  /* 0x00000002 */
#define GRF_CPU_CON0_GRF_CON_NIDEN_SHIFT                   (2U)
#define GRF_CPU_CON0_GRF_CON_NIDEN_MASK                    (0x1U << GRF_CPU_CON0_GRF_CON_NIDEN_SHIFT)                   /* 0x00000004 */
#define GRF_CPU_CON0_GRF_CON_SPNIDEN_SHIFT                 (3U)
#define GRF_CPU_CON0_GRF_CON_SPNIDEN_MASK                  (0x1U << GRF_CPU_CON0_GRF_CON_SPNIDEN_SHIFT)                 /* 0x00000008 */
#define GRF_CPU_CON0_GRF_CON_VINITHI_SHIFT                 (4U)
#define GRF_CPU_CON0_GRF_CON_VINITHI_MASK                  (0x1U << GRF_CPU_CON0_GRF_CON_VINITHI_SHIFT)                 /* 0x00000010 */
#define GRF_CPU_CON0_GRF_CON_DAPDEVICEEN_SHIFT             (5U)
#define GRF_CPU_CON0_GRF_CON_DAPDEVICEEN_MASK              (0x1U << GRF_CPU_CON0_GRF_CON_DAPDEVICEEN_SHIFT)             /* 0x00000020 */
#define GRF_CPU_CON0_GRF_CON_CP15SDISABLE_SHIFT            (6U)
#define GRF_CPU_CON0_GRF_CON_CP15SDISABLE_MASK             (0x1U << GRF_CPU_CON0_GRF_CON_CP15SDISABLE_SHIFT)            /* 0x00000040 */
#define GRF_CPU_CON0_GRF_CON_L1RSTDISABLE_SHIFT            (7U)
#define GRF_CPU_CON0_GRF_CON_L1RSTDISABLE_MASK             (0x1U << GRF_CPU_CON0_GRF_CON_L1RSTDISABLE_SHIFT)            /* 0x00000080 */
#define GRF_CPU_CON0_GRF_CON_L2RSTDISABLE_SHIFT            (8U)
#define GRF_CPU_CON0_GRF_CON_L2RSTDISABLE_MASK             (0x1U << GRF_CPU_CON0_GRF_CON_L2RSTDISABLE_SHIFT)            /* 0x00000100 */
#define GRF_CPU_CON0_GRF_CON_CFGEND_SHIFT                  (9U)
#define GRF_CPU_CON0_GRF_CON_CFGEND_MASK                   (0x1U << GRF_CPU_CON0_GRF_CON_CFGEND_SHIFT)                  /* 0x00000200 */
#define GRF_CPU_CON0_GRF_CON_CFGTE_SHIFT                   (10U)
#define GRF_CPU_CON0_GRF_CON_CFGTE_MASK                    (0x1U << GRF_CPU_CON0_GRF_CON_CFGTE_SHIFT)                   /* 0x00000400 */
/* CPU_CON1 */
#define GRF_CPU_CON1_OFFSET                                (0x504)
#define GRF_CPU_CON1_GRF_CON_DBGROMADDRV_SHIFT             (0U)
#define GRF_CPU_CON1_GRF_CON_DBGROMADDRV_MASK              (0x1U << GRF_CPU_CON1_GRF_CON_DBGROMADDRV_SHIFT)             /* 0x00000001 */
#define GRF_CPU_CON1_GRF_CON_DBGSELFADDRV_SHIFT            (1U)
#define GRF_CPU_CON1_GRF_CON_DBGSELFADDRV_MASK             (0x1U << GRF_CPU_CON1_GRF_CON_DBGSELFADDRV_SHIFT)            /* 0x00000002 */
#define GRF_CPU_CON1_GRF_CON_EVENTI_SHIFT                  (2U)
#define GRF_CPU_CON1_GRF_CON_EVENTI_MASK                   (0x1U << GRF_CPU_CON1_GRF_CON_EVENTI_SHIFT)                  /* 0x00000004 */
#define GRF_CPU_CON1_GRF_CON_EVENTO_CLEAR_SHIFT            (3U)
#define GRF_CPU_CON1_GRF_CON_EVENTO_CLEAR_MASK             (0x1U << GRF_CPU_CON1_GRF_CON_EVENTO_CLEAR_SHIFT)            /* 0x00000008 */
#define GRF_CPU_CON1_GRF_CON_CFGSDISABLE_SHIFT             (4U)
#define GRF_CPU_CON1_GRF_CON_CFGSDISABLE_MASK              (0x1U << GRF_CPU_CON1_GRF_CON_CFGSDISABLE_SHIFT)             /* 0x00000010 */
/* OS_REG0 */
#define GRF_OS_REG0_OFFSET                                 (0x580)
#define GRF_OS_REG0_OS_REG_SHIFT                           (0U)
#define GRF_OS_REG0_OS_REG_MASK                            (0xFFFFFFFFU << GRF_OS_REG0_OS_REG_SHIFT)                    /* 0xFFFFFFFF */
/* OS_REG1 */
#define GRF_OS_REG1_OFFSET                                 (0x584)
#define GRF_OS_REG1_OS_REG_SHIFT                           (0U)
#define GRF_OS_REG1_OS_REG_MASK                            (0xFFFFFFFFU << GRF_OS_REG1_OS_REG_SHIFT)                    /* 0xFFFFFFFF */
/* OS_REG2 */
#define GRF_OS_REG2_OFFSET                                 (0x588)
#define GRF_OS_REG2_OS_REG_SHIFT                           (0U)
#define GRF_OS_REG2_OS_REG_MASK                            (0xFFFFFFFFU << GRF_OS_REG2_OS_REG_SHIFT)                    /* 0xFFFFFFFF */
/* OS_REG3 */
#define GRF_OS_REG3_OFFSET                                 (0x58C)
#define GRF_OS_REG3_OS_REG_SHIFT                           (0U)
#define GRF_OS_REG3_OS_REG_MASK                            (0xFFFFFFFFU << GRF_OS_REG3_OS_REG_SHIFT)                    /* 0xFFFFFFFF */
/* DDR_STATUS */
#define GRF_DDR_STATUS_OFFSET                              (0x604)
#define GRF_DDR_STATUS_DDRUPCTL_BBFLAGS_SHIFT              (0U)
#define GRF_DDR_STATUS_DDRUPCTL_BBFLAGS_MASK               (0xFFFFU << GRF_DDR_STATUS_DDRUPCTL_BBFLAGS_SHIFT)           /* 0x0000FFFF */
#define GRF_DDR_STATUS_DDRUPCTL_STAT_SHIFT                 (16U)
#define GRF_DDR_STATUS_DDRUPCTL_STAT_MASK                  (0x7U << GRF_DDR_STATUS_DDRUPCTL_STAT_SHIFT)                 /* 0x00070000 */
#define GRF_DDR_STATUS_UPCTL_C_ACTIVE_SHIFT                (19U)
#define GRF_DDR_STATUS_UPCTL_C_ACTIVE_MASK                 (0x1U << GRF_DDR_STATUS_UPCTL_C_ACTIVE_SHIFT)                /* 0x00080000 */
#define GRF_DDR_STATUS_UPCTL_C_SYSACK_SHIFT                (20U)
#define GRF_DDR_STATUS_UPCTL_C_SYSACK_MASK                 (0x1U << GRF_DDR_STATUS_UPCTL_C_SYSACK_SHIFT)                /* 0x00100000 */
/* SIG_DETECT_CON */
#define GRF_SIG_DETECT_CON_OFFSET                          (0x680)
#define GRF_SIG_DETECT_CON_SD_DETECT_RISING_EDGE_DECTECT_EN_SHIFT (0U)
#define GRF_SIG_DETECT_CON_SD_DETECT_RISING_EDGE_DECTECT_EN_MASK (0x1U << GRF_SIG_DETECT_CON_SD_DETECT_RISING_EDGE_DECTECT_EN_SHIFT) /* 0x00000001 */
#define GRF_SIG_DETECT_CON_SD_DETECT_FALL_EDGE_DETECT_EN_SHIFT (1U)
#define GRF_SIG_DETECT_CON_SD_DETECT_FALL_EDGE_DETECT_EN_MASK (0x1U << GRF_SIG_DETECT_CON_SD_DETECT_FALL_EDGE_DETECT_EN_SHIFT) /* 0x00000002 */
#define GRF_SIG_DETECT_CON_OTG_LINESTATE_DETECT_EN_SHIFT   (2U)
#define GRF_SIG_DETECT_CON_OTG_LINESTATE_DETECT_EN_MASK    (0x1U << GRF_SIG_DETECT_CON_OTG_LINESTATE_DETECT_EN_SHIFT)   /* 0x00000004 */
#define GRF_SIG_DETECT_CON_OTG_BVALID_DETECT_EN_SHIFT      (3U)
#define GRF_SIG_DETECT_CON_OTG_BVALID_DETECT_EN_MASK       (0x1U << GRF_SIG_DETECT_CON_OTG_BVALID_DETECT_EN_SHIFT)      /* 0x00000008 */
#define GRF_SIG_DETECT_CON_HOST_LINE_STATE_DETECT_EN_SHIFT (4U)
#define GRF_SIG_DETECT_CON_HOST_LINE_STATE_DETECT_EN_MASK  (0x1U << GRF_SIG_DETECT_CON_HOST_LINE_STATE_DETECT_EN_SHIFT) /* 0x00000010 */
#define GRF_SIG_DETECT_CON_OTG_ID_RISE_EDGE_DETECT_EN_SHIFT (5U)
#define GRF_SIG_DETECT_CON_OTG_ID_RISE_EDGE_DETECT_EN_MASK (0x1U << GRF_SIG_DETECT_CON_OTG_ID_RISE_EDGE_DETECT_EN_SHIFT) /* 0x00000020 */
#define GRF_SIG_DETECT_CON_OTG_ID_FALL_EDGE_DETECT_EN_SHIFT (6U)
#define GRF_SIG_DETECT_CON_OTG_ID_FALL_EDGE_DETECT_EN_MASK (0x1U << GRF_SIG_DETECT_CON_OTG_ID_FALL_EDGE_DETECT_EN_SHIFT) /* 0x00000040 */
#define GRF_SIG_DETECT_CON_OTG_ID_FILTER_TIME_SEL_SHIFT    (8U)
#define GRF_SIG_DETECT_CON_OTG_ID_FILTER_TIME_SEL_MASK     (0x3U << GRF_SIG_DETECT_CON_OTG_ID_FILTER_TIME_SEL_SHIFT)    /* 0x00000300 */
#define GRF_SIG_DETECT_CON_OTG_LS_FILTER_TIME_SEL_SHIFT    (10U)
#define GRF_SIG_DETECT_CON_OTG_LS_FILTER_TIME_SEL_MASK     (0x3U << GRF_SIG_DETECT_CON_OTG_LS_FILTER_TIME_SEL_SHIFT)    /* 0x00000C00 */
#define GRF_SIG_DETECT_CON_HOST_LS_FILTER_TIME_SEL_SHIFT   (12U)
#define GRF_SIG_DETECT_CON_HOST_LS_FILTER_TIME_SEL_MASK    (0x3U << GRF_SIG_DETECT_CON_HOST_LS_FILTER_TIME_SEL_SHIFT)   /* 0x00003000 */
/* SIG_DETECT_STATUS */
#define GRF_SIG_DETECT_STATUS_OFFSET                       (0x690)
#define GRF_SIG_DETECT_STATUS_SD_DETECT_RISING_EDGE_DECTECT_STATUS_SHIFT (0U)
#define GRF_SIG_DETECT_STATUS_SD_DETECT_RISING_EDGE_DECTECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_SD_DETECT_RISING_EDGE_DECTECT_STATUS_SHIFT) /* 0x00000001 */
#define GRF_SIG_DETECT_STATUS_SD_DETECT_FALL_EDGE_DETECT_STATUS_SHIFT (1U)
#define GRF_SIG_DETECT_STATUS_SD_DETECT_FALL_EDGE_DETECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_SD_DETECT_FALL_EDGE_DETECT_STATUS_SHIFT) /* 0x00000002 */
#define GRF_SIG_DETECT_STATUS_OTG_LINESTATE_DETECT_STATUS_SHIFT (2U)
#define GRF_SIG_DETECT_STATUS_OTG_LINESTATE_DETECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_OTG_LINESTATE_DETECT_STATUS_SHIFT) /* 0x00000004 */
#define GRF_SIG_DETECT_STATUS_OTG_BVALID_DETECT_STATUS_SHIFT (3U)
#define GRF_SIG_DETECT_STATUS_OTG_BVALID_DETECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_OTG_BVALID_DETECT_STATUS_SHIFT) /* 0x00000008 */
#define GRF_SIG_DETECT_STATUS_HOST_LINE_STATE_DETECT_STATUS_SHIFT (4U)
#define GRF_SIG_DETECT_STATUS_HOST_LINE_STATE_DETECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_HOST_LINE_STATE_DETECT_STATUS_SHIFT) /* 0x00000010 */
#define GRF_SIG_DETECT_STATUS_OTG_ID_RISE_EDGE_DETECT_STATUS_SHIFT (5U)
#define GRF_SIG_DETECT_STATUS_OTG_ID_RISE_EDGE_DETECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_OTG_ID_RISE_EDGE_DETECT_STATUS_SHIFT) /* 0x00000020 */
#define GRF_SIG_DETECT_STATUS_OTG_ID_FALL_EDGE_DETECT_STATUS_SHIFT (6U)
#define GRF_SIG_DETECT_STATUS_OTG_ID_FALL_EDGE_DETECT_STATUS_MASK (0x1U << GRF_SIG_DETECT_STATUS_OTG_ID_FALL_EDGE_DETECT_STATUS_SHIFT) /* 0x00000040 */
/* SIG_DETECT_CLR */
#define GRF_SIG_DETECT_CLR_OFFSET                          (0x6A0)
#define GRF_SIG_DETECT_CLR_SD_DETECT_RISING_EDGE_DECTECT_CLR_SHIFT (0U)
#define GRF_SIG_DETECT_CLR_SD_DETECT_RISING_EDGE_DECTECT_CLR_MASK (0x1U << GRF_SIG_DETECT_CLR_SD_DETECT_RISING_EDGE_DECTECT_CLR_SHIFT) /* 0x00000001 */
#define GRF_SIG_DETECT_CLR_SD_DETECT_FALL_EDGE_DETECT_CLR_SHIFT (1U)
#define GRF_SIG_DETECT_CLR_SD_DETECT_FALL_EDGE_DETECT_CLR_MASK (0x1U << GRF_SIG_DETECT_CLR_SD_DETECT_FALL_EDGE_DETECT_CLR_SHIFT) /* 0x00000002 */
#define GRF_SIG_DETECT_CLR_OTG_LINESTATE_DETECT_CLR_SHIFT  (2U)
#define GRF_SIG_DETECT_CLR_OTG_LINESTATE_DETECT_CLR_MASK   (0x1U << GRF_SIG_DETECT_CLR_OTG_LINESTATE_DETECT_CLR_SHIFT)  /* 0x00000004 */
#define GRF_SIG_DETECT_CLR_OTG_BVALID_DETECT_CLR_SHIFT     (3U)
#define GRF_SIG_DETECT_CLR_OTG_BVALID_DETECT_CLR_MASK      (0x1U << GRF_SIG_DETECT_CLR_OTG_BVALID_DETECT_CLR_SHIFT)     /* 0x00000008 */
#define GRF_SIG_DETECT_CLR_HOST_LINE_STATE_DETECT_CLR_SHIFT (4U)
#define GRF_SIG_DETECT_CLR_HOST_LINE_STATE_DETECT_CLR_MASK (0x1U << GRF_SIG_DETECT_CLR_HOST_LINE_STATE_DETECT_CLR_SHIFT) /* 0x00000010 */
#define GRF_SIG_DETECT_CLR_OTG_ID_RISE_EDGE_DETECT_CLR_SHIFT (5U)
#define GRF_SIG_DETECT_CLR_OTG_ID_RISE_EDGE_DETECT_CLR_MASK (0x1U << GRF_SIG_DETECT_CLR_OTG_ID_RISE_EDGE_DETECT_CLR_SHIFT) /* 0x00000020 */
#define GRF_SIG_DETECT_CLR_OTG_ID_FALL_EDGE_DETECT_CLR_SHIFT (6U)
#define GRF_SIG_DETECT_CLR_OTG_ID_FALL_EDGE_DETECT_CLR_MASK (0x1U << GRF_SIG_DETECT_CLR_OTG_ID_FALL_EDGE_DETECT_CLR_SHIFT) /* 0x00000040 */
/* HOST0_CON0 */
#define GRF_HOST0_CON0_OFFSET                              (0x700)
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_SHIFT       (0U)
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_MASK        (0x3FU << GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_SHIFT)      /* 0x0000003F */
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_COMMON_SHIFT (6U)
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_COMMON_MASK (0x3FU << GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_COMMON_SHIFT) /* 0x00000FC0 */
/* HOST0_CON1 */
#define GRF_HOST0_CON1_OFFSET                              (0x704)
#define GRF_HOST0_CON1_GRF_CON_HOST0_APP_START_CLK_SHIFT   (0U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_APP_START_CLK_MASK    (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_APP_START_CLK_SHIFT)   /* 0x00000001 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_HUBSETUP_MIN_SHIFT    (1U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_HUBSETUP_MIN_MASK     (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_HUBSETUP_MIN_SHIFT)    /* 0x00000002 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCR16_EN_SHIFT       (2U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCR16_EN_MASK        (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_INCR16_EN_SHIFT)       /* 0x00000004 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCR4_EN_SHIFT        (3U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCR4_EN_MASK         (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_INCR4_EN_SHIFT)        /* 0x00000008 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCR8_EN_SHIFT        (4U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCR8_EN_MASK         (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_INCR8_EN_SHIFT)        /* 0x00000010 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCRX_EN_SHIFT        (5U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_INCRX_EN_MASK         (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_INCRX_EN_SHIFT)        /* 0x00000020 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_SIM_MODE_SHIFT        (6U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_SIM_MODE_MASK         (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_SIM_MODE_SHIFT)        /* 0x00000040 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_WORD_IF_SHIFT         (7U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_WORD_IF_MASK          (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_WORD_IF_SHIFT)         /* 0x00000080 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_AUTOPPD_ON_OVERCUR_EN_SHIFT (8U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_AUTOPPD_ON_OVERCUR_EN_MASK (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_AUTOPPD_ON_OVERCUR_EN_SHIFT) /* 0x00000100 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_APP_PRT_OVRCUR_SHIFT  (9U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_APP_PRT_OVRCUR_MASK   (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_APP_PRT_OVRCUR_SHIFT)  /* 0x00000200 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_CLKCKTRST_SHIFT  (10U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_CLKCKTRST_MASK   (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_CLKCKTRST_SHIFT)  /* 0x00000400 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_CNTSEL_SHIFT     (11U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_CNTSEL_MASK      (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_CNTSEL_SHIFT)     /* 0x00000800 */
#define GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_SUSP_LGCY_SHIFT  (12U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_SUSP_LGCY_MASK   (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_OHCI_SUSP_LGCY_SHIFT)  /* 0x00001000 */
#define GRF_HOST0_CON1_OTG_DBNCE_FLTR_BYPASS_SHIFT         (13U)
#define GRF_HOST0_CON1_OTG_DBNCE_FLTR_BYPASS_MASK          (0x1U << GRF_HOST0_CON1_OTG_DBNCE_FLTR_BYPASS_SHIFT)         /* 0x00002000 */
#define GRF_HOST0_CON1_OTG_SCALEDOWN_MODE_SHIFT            (14U)
#define GRF_HOST0_CON1_OTG_SCALEDOWN_MODE_MASK             (0x3U << GRF_HOST0_CON1_OTG_SCALEDOWN_MODE_SHIFT)            /* 0x0000C000 */
/* DMA_CON0 */
#define GRF_DMA_CON0_OFFSET                                (0x710)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART0_TX_SHIFT     (0U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART0_TX_MASK      (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART0_TX_SHIFT)     /* 0x00000003 */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART0_RX_SHIFT     (2U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART0_RX_MASK      (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART0_RX_SHIFT)     /* 0x0000000C */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART1_TX_SHIFT     (4U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART1_TX_MASK      (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART1_TX_SHIFT)     /* 0x00000030 */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART1_RX_SHIFT     (6U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART1_RX_MASK      (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART1_RX_SHIFT)     /* 0x000000C0 */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART2_TX_SHIFT     (8U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART2_TX_MASK      (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART2_TX_SHIFT)     /* 0x00000300 */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART2_RX_SHIFT     (10U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART2_RX_MASK      (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_UART2_RX_SHIFT)     /* 0x00000C00 */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_SPI0_TX_SHIFT      (12U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_SPI0_TX_MASK       (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_SPI0_TX_SHIFT)      /* 0x00003000 */
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_SPI0_RX_SHIFT      (14U)
#define GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_SPI0_RX_MASK       (0x3U << GRF_DMA_CON0_GRF_DRTYPE_BUSDMAC_SPI0_RX_SHIFT)      /* 0x0000C000 */
/* DMA_CON1 */
#define GRF_DMA_CON1_OFFSET                                (0x714)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S0_8CH_TX_SHIFT  (0U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S0_8CH_TX_MASK   (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S0_8CH_TX_SHIFT)  /* 0x00000003 */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S0_8CH_RX_SHIFT  (2U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S0_8CH_RX_MASK   (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S0_8CH_RX_SHIFT)  /* 0x0000000C */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S1_2CH_TX_SHIFT  (4U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S1_2CH_TX_MASK   (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S1_2CH_TX_SHIFT)  /* 0x00000030 */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S1_2CH_RX_SHIFT  (6U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S1_2CH_RX_MASK   (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S1_2CH_RX_SHIFT)  /* 0x000000C0 */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S2_2CH_TX_SHIFT  (8U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S2_2CH_TX_MASK   (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S2_2CH_TX_SHIFT)  /* 0x00000300 */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S2_2CH_RX_SHIFT  (10U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S2_2CH_RX_MASK   (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_I2S2_2CH_RX_SHIFT)  /* 0x00000C00 */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_PWM0_SHIFT         (12U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_PWM0_MASK          (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_PWM0_SHIFT)         /* 0x00003000 */
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_PWM1_SHIFT         (14U)
#define GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_PWM1_MASK          (0x3U << GRF_DMA_CON1_GRF_DRTYPE_BUSDMAC_PWM1_SHIFT)         /* 0x0000C000 */
/* UOC_STATUS */
#define GRF_UOC_STATUS_OFFSET                              (0xF84)
#define GRF_UOC_STATUS_GRF_STAT_USBPHY0_DCP_DETECTE_SHIFT  (0U)
#define GRF_UOC_STATUS_GRF_STAT_USBPHY0_DCP_DETECTE_MASK   (0x1U << GRF_UOC_STATUS_GRF_STAT_USBPHY0_DCP_DETECTE_SHIFT)  /* 0x00000001 */
#define GRF_UOC_STATUS_GRF_STAT_USBPHY0_CP_DETECTED_SHIFT  (1U)
#define GRF_UOC_STATUS_GRF_STAT_USBPHY0_CP_DETECTED_MASK   (0x1U << GRF_UOC_STATUS_GRF_STAT_USBPHY0_CP_DETECTED_SHIFT)  /* 0x00000002 */
#define GRF_UOC_STATUS_GRF_STAT_USBPHY0_DP_ATTACHED_SHIFT  (2U)
#define GRF_UOC_STATUS_GRF_STAT_USBPHY0_DP_ATTACHED_MASK   (0x1U << GRF_UOC_STATUS_GRF_STAT_USBPHY0_DP_ATTACHED_SHIFT)  /* 0x00000004 */
#define GRF_UOC_STATUS_OTG_UTMI_LS_FS_RCV_SHIFT            (3U)
#define GRF_UOC_STATUS_OTG_UTMI_LS_FS_RCV_MASK             (0x1U << GRF_UOC_STATUS_OTG_UTMI_LS_FS_RCV_SHIFT)            /* 0x00000008 */
#define GRF_UOC_STATUS_OTG_UTMI_VMI_SHIFT                  (4U)
#define GRF_UOC_STATUS_OTG_UTMI_VMI_MASK                   (0x1U << GRF_UOC_STATUS_OTG_UTMI_VMI_SHIFT)                  /* 0x00000010 */
#define GRF_UOC_STATUS_OTG_UTMI_VPI_SHIFT                  (5U)
#define GRF_UOC_STATUS_OTG_UTMI_VPI_MASK                   (0x1U << GRF_UOC_STATUS_OTG_UTMI_VPI_SHIFT)                  /* 0x00000020 */
#define GRF_UOC_STATUS_HOST0_UTMI_FS_XVER_OWN_SHIFT        (6U)
#define GRF_UOC_STATUS_HOST0_UTMI_FS_XVER_OWN_MASK         (0x1U << GRF_UOC_STATUS_HOST0_UTMI_FS_XVER_OWN_SHIFT)        /* 0x00000040 */
#define GRF_UOC_STATUS_HOST0_UTMI_HOSTDISCONNECT_SHIFT     (7U)
#define GRF_UOC_STATUS_HOST0_UTMI_HOSTDISCONNECT_MASK      (0x1U << GRF_UOC_STATUS_HOST0_UTMI_HOSTDISCONNECT_SHIFT)     /* 0x00000080 */
#define GRF_UOC_STATUS_HOST0_UTMI_LINESTATE_SHIFT          (8U)
#define GRF_UOC_STATUS_HOST0_UTMI_LINESTATE_MASK           (0x3U << GRF_UOC_STATUS_HOST0_UTMI_LINESTATE_SHIFT)          /* 0x00000300 */
#define GRF_UOC_STATUS_OTG_UTMI_BVALID_SHIFT               (10U)
#define GRF_UOC_STATUS_OTG_UTMI_BVALID_MASK                (0x1U << GRF_UOC_STATUS_OTG_UTMI_BVALID_SHIFT)               /* 0x00000400 */
#define GRF_UOC_STATUS_OTG_UTMI_HOSTDISCONNECT_SHIFT       (11U)
#define GRF_UOC_STATUS_OTG_UTMI_HOSTDISCONNECT_MASK        (0x1U << GRF_UOC_STATUS_OTG_UTMI_HOSTDISCONNECT_SHIFT)       /* 0x00000800 */
#define GRF_UOC_STATUS_OTG_UTMI_LINESTATE_SHIFT            (12U)
#define GRF_UOC_STATUS_OTG_UTMI_LINESTATE_MASK             (0x3U << GRF_UOC_STATUS_OTG_UTMI_LINESTATE_SHIFT)            /* 0x00003000 */
/* HOST_STATUS */
#define GRF_HOST_STATUS_OFFSET                             (0xF88)
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_XFER_CNT_SHIFT   (0U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_XFER_CNT_MASK    (0x7FFU << GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_XFER_CNT_SHIFT) /* 0x000007FF */
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_USBSTS_SHIFT     (11U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_USBSTS_MASK      (0x3FU << GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_USBSTS_SHIFT)    /* 0x0001F800 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_LPSMC_STATE_SHIFT (17U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_LPSMC_STATE_MASK (0xFU << GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_LPSMC_STATE_SHIFT) /* 0x001E0000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_RMTWKP_SHIFT     (21U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_RMTWKP_MASK      (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_RMTWKP_SHIFT)     /* 0x00200000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_BUFACC_SHIFT     (22U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_BUFACC_MASK      (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_BUFACC_SHIFT)     /* 0x00400000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_GLOBALSUSPEND_SHIFT (23U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_GLOBALSUSPEND_MASK (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_GLOBALSUSPEND_SHIFT) /* 0x00800000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_DRWE_SHIFT       (24U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_DRWE_MASK        (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_DRWE_SHIFT)       /* 0x01000000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_RWE_SHIFT        (25U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_RWE_MASK         (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_RWE_SHIFT)        /* 0x02000000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_CCS_SHIFT        (26U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_CCS_MASK         (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_OHCI_CCS_SHIFT)        /* 0x04000000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_XFER_PRDC_SHIFT  (27U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_XFER_PRDC_MASK   (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_XFER_PRDC_SHIFT)  /* 0x08000000 */
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_BUFACC_SHIFT     (28U)
#define GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_BUFACC_MASK      (0x1U << GRF_HOST_STATUS_GRF_ST_HOST0_EHCI_BUFACC_SHIFT)     /* 0x10000000 */
#define GRF_HOST_STATUS_HOST0_EHCI_PME_STATUS_SHIFT        (29U)
#define GRF_HOST_STATUS_HOST0_EHCI_PME_STATUS_MASK         (0x1U << GRF_HOST_STATUS_HOST0_EHCI_PME_STATUS_SHIFT)        /* 0x20000000 */
#define GRF_HOST_STATUS_HOST0_EHCI_POWER_STATE_ACK_SHIFT   (30U)
#define GRF_HOST_STATUS_HOST0_EHCI_POWER_STATE_ACK_MASK    (0x1U << GRF_HOST_STATUS_HOST0_EHCI_POWER_STATE_ACK_SHIFT)   /* 0x40000000 */
/* GMAC_CON0 */
#define GRF_GMAC_CON0_OFFSET                               (0xF8C)
#define GRF_GMAC_CON0_GMAC_MAC_SPEED_SHIFT                 (2U)
#define GRF_GMAC_CON0_GMAC_MAC_SPEED_MASK                  (0x1U << GRF_GMAC_CON0_GMAC_MAC_SPEED_SHIFT)                 /* 0x00000004 */
#define GRF_GMAC_CON0_GMAC_FLOWCTRL_SHIFT                  (3U)
#define GRF_GMAC_CON0_GMAC_FLOWCTRL_MASK                   (0x1U << GRF_GMAC_CON0_GMAC_FLOWCTRL_SHIFT)                  /* 0x00000008 */
#define GRF_GMAC_CON0_GMAC_PHY_INTF_SEL_SHIFT              (4U)
#define GRF_GMAC_CON0_GMAC_PHY_INTF_SEL_MASK               (0x7U << GRF_GMAC_CON0_GMAC_PHY_INTF_SEL_SHIFT)              /* 0x00000070 */
#define GRF_GMAC_CON0_RMII_CLK_SEL_SHIFT                   (7U)
#define GRF_GMAC_CON0_RMII_CLK_SEL_MASK                    (0x1U << GRF_GMAC_CON0_RMII_CLK_SEL_SHIFT)                   /* 0x00000080 */
/* CHIP_ID */
#define GRF_CHIP_ID_OFFSET                                 (0xF90)
#define GRF_CHIP_ID_CHIP_ID_SHIFT                          (0U)
#define GRF_CHIP_ID_CHIP_ID_MASK                           (0xFFFFFFFFU << GRF_CHIP_ID_CHIP_ID_SHIFT)                   /* 0xFFFFFFFF */

/******************************************PMU*******************************************/
/* PMU_WAKEUP_CFG0 */
#define PMU_PMU_WAKEUP_CFG0_GPIO_POSEDGE_ENABLE_SHIFT      (0U)
#define PMU_PMU_WAKEUP_CFG0_GPIO_POSEDGE_ENABLE_MASK       (0xFFFFFFFFU << PMU_PMU_WAKEUP_CFG0_GPIO_POSEDGE_ENABLE_SHIFT) /* 0xFFFFFFFF */
/* PMU_WAKEUP_CFG1 */
#define PMU_PMU_WAKEUP_CFG1_WAKEUP_GPIO_NEG_EN_SHIFT       (0U)
#define PMU_PMU_WAKEUP_CFG1_WAKEUP_GPIO_NEG_EN_MASK        (0xFFFFFFFFU << PMU_PMU_WAKEUP_CFG1_WAKEUP_GPIO_NEG_EN_SHIFT) /* 0xFFFFFFFF */
/* PMU_WAKEUP_CFG2 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_INT_CLUSTER_SHIFT       (0U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_INT_CLUSTER_MASK        (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_INT_CLUSTER_SHIFT)       /* 0x00000001 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_GPIO_INT_EN_SHIFT       (2U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_GPIO_INT_EN_MASK        (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_GPIO_INT_EN_SHIFT)       /* 0x00000004 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_SDIO_EN_SHIFT           (3U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_SDIO_EN_MASK            (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_SDIO_EN_SHIFT)           /* 0x00000008 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_SDMMC_EN_SHIFT          (4U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_SDMMC_EN_MASK           (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_SDMMC_EN_SHIFT)          /* 0x00000010 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_TIMER_EN_SHIFT          (6U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_TIMER_EN_MASK           (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_TIMER_EN_SHIFT)          /* 0x00000040 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_USBDEV_EN_SHIFT         (7U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_USBDEV_EN_MASK          (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_USBDEV_EN_SHIFT)         /* 0x00000080 */
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_TIMEOUT_EN_SHIFT        (10U)
#define PMU_PMU_WAKEUP_CFG2_WAKEUP_TIMEOUT_EN_MASK         (0x1U << PMU_PMU_WAKEUP_CFG2_WAKEUP_TIMEOUT_EN_SHIFT)        /* 0x00000400 */
/* PMU_PWRMODE_CORE_CON */
#define PMU_PMU_PWRMODE_CORE_CON_GLOBAL_INT_DISABLE_SHIFT  (0U)
#define PMU_PMU_PWRMODE_CORE_CON_GLOBAL_INT_DISABLE_MASK   (0x1U << PMU_PMU_PWRMODE_CORE_CON_GLOBAL_INT_DISABLE_SHIFT)  /* 0x00000001 */
#define PMU_PMU_PWRMODE_CORE_CON_CLR_CORE_SHIFT            (5U)
#define PMU_PMU_PWRMODE_CORE_CON_CLR_CORE_MASK             (0x1U << PMU_PMU_PWRMODE_CORE_CON_CLR_CORE_SHIFT)            /* 0x00000020 */
#define PMU_PMU_PWRMODE_CORE_CON_SCU_PD_EN_SHIFT           (6U)
#define PMU_PMU_PWRMODE_CORE_CON_SCU_PD_EN_MASK            (0x1U << PMU_PMU_PWRMODE_CORE_CON_SCU_PD_EN_SHIFT)           /* 0x00000040 */
#define PMU_PMU_PWRMODE_CORE_CON_APLL_PD_EN_SHIFT          (12U)
#define PMU_PMU_PWRMODE_CORE_CON_APLL_PD_EN_MASK           (0x1U << PMU_PMU_PWRMODE_CORE_CON_APLL_PD_EN_SHIFT)          /* 0x00001000 */
#define PMU_PMU_PWRMODE_CORE_CON_DPLL_PD_EN_SHIFT          (13U)
#define PMU_PMU_PWRMODE_CORE_CON_DPLL_PD_EN_MASK           (0x1U << PMU_PMU_PWRMODE_CORE_CON_DPLL_PD_EN_SHIFT)          /* 0x00002000 */
#define PMU_PMU_PWRMODE_CORE_CON_GPLL_PD_EN_SHIFT          (14U)
#define PMU_PMU_PWRMODE_CORE_CON_GPLL_PD_EN_MASK           (0x1U << PMU_PMU_PWRMODE_CORE_CON_GPLL_PD_EN_SHIFT)          /* 0x00004000 */
/* PMU_PWRMODE_COMMON_CON */
#define PMU_PMU_PWRMODE_COMMON_CON_POWER_MODE_EN_SHIFT     (0U)
#define PMU_PMU_PWRMODE_COMMON_CON_POWER_MODE_EN_MASK      (0x1U << PMU_PMU_PWRMODE_COMMON_CON_POWER_MODE_EN_SHIFT)     /* 0x00000001 */
#define PMU_PMU_PWRMODE_COMMON_CON_WAKEUP_RESET_EN_SHIFT   (3U)
#define PMU_PMU_PWRMODE_COMMON_CON_WAKEUP_RESET_EN_MASK    (0x1U << PMU_PMU_PWRMODE_COMMON_CON_WAKEUP_RESET_EN_SHIFT)   /* 0x00000008 */
#define PMU_PMU_PWRMODE_COMMON_CON_PLL_PD_EN_SHIFT         (4U)
#define PMU_PMU_PWRMODE_COMMON_CON_PLL_PD_EN_MASK          (0x1U << PMU_PMU_PWRMODE_COMMON_CON_PLL_PD_EN_SHIFT)         /* 0x00000010 */
#define PMU_PMU_PWRMODE_COMMON_CON_POWER_OFF_REQ_CFG_SHIFT (5U)
#define PMU_PMU_PWRMODE_COMMON_CON_POWER_OFF_REQ_CFG_MASK  (0x1U << PMU_PMU_PWRMODE_COMMON_CON_POWER_OFF_REQ_CFG_SHIFT) /* 0x00000020 */
#define PMU_PMU_PWRMODE_COMMON_CON_PMU_USE_LF_SHIFT        (6U)
#define PMU_PMU_PWRMODE_COMMON_CON_PMU_USE_LF_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_PMU_USE_LF_SHIFT)        /* 0x00000040 */
#define PMU_PMU_PWRMODE_COMMON_CON_DDRPHY_GATING_EN_SHIFT  (7U)
#define PMU_PMU_PWRMODE_COMMON_CON_DDRPHY_GATING_EN_MASK   (0x1U << PMU_PMU_PWRMODE_COMMON_CON_DDRPHY_GATING_EN_SHIFT)  /* 0x00000080 */
#define PMU_PMU_PWRMODE_COMMON_CON_OSC_24M_DIS_SHIFT       (8U)
#define PMU_PMU_PWRMODE_COMMON_CON_OSC_24M_DIS_MASK        (0x1U << PMU_PMU_PWRMODE_COMMON_CON_OSC_24M_DIS_SHIFT)       /* 0x00000100 */
#define PMU_PMU_PWRMODE_COMMON_CON_INPUT_CLAMP_EN_SHIFT    (9U)
#define PMU_PMU_PWRMODE_COMMON_CON_INPUT_CLAMP_EN_MASK     (0x1U << PMU_PMU_PWRMODE_COMMON_CON_INPUT_CLAMP_EN_SHIFT)    /* 0x00000200 */
#define PMU_PMU_PWRMODE_COMMON_CON_SREF_ENTER_EN_SHIFT     (10U)
#define PMU_PMU_PWRMODE_COMMON_CON_SREF_ENTER_EN_MASK      (0x1U << PMU_PMU_PWRMODE_COMMON_CON_SREF_ENTER_EN_SHIFT)     /* 0x00000400 */
#define PMU_PMU_PWRMODE_COMMON_CON_DDRC_GATING_EN_SHIFT    (11U)
#define PMU_PMU_PWRMODE_COMMON_CON_DDRC_GATING_EN_MASK     (0x1U << PMU_PMU_PWRMODE_COMMON_CON_DDRC_GATING_EN_SHIFT)    /* 0x00000800 */
#define PMU_PMU_PWRMODE_COMMON_CON_DDR_RET_EN_SHIFT        (12U)
#define PMU_PMU_PWRMODE_COMMON_CON_DDR_RET_EN_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_DDR_RET_EN_SHIFT)        /* 0x00001000 */
#define PMU_PMU_PWRMODE_COMMON_CON_DDR_RET_DE_REQ_SHIFT    (13U)
#define PMU_PMU_PWRMODE_COMMON_CON_DDR_RET_DE_REQ_MASK     (0x1U << PMU_PMU_PWRMODE_COMMON_CON_DDR_RET_DE_REQ_SHIFT)    /* 0x00002000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_PMU_SHIFT           (14U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_PMU_MASK            (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_PMU_SHIFT)           /* 0x00004000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_BUS_SHIFT           (16U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_BUS_MASK            (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_BUS_SHIFT)           /* 0x00010000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_DSP_SHIFT           (17U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_DSP_MASK            (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_DSP_SHIFT)           /* 0x00020000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_MSCH_SHIFT          (18U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_MSCH_MASK           (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_MSCH_SHIFT)          /* 0x00040000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_VENC_SHIFT          (19U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_VENC_MASK           (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_VENC_SHIFT)          /* 0x00080000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_PERI_SHIFT          (20U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_PERI_MASK           (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_PERI_SHIFT)          /* 0x00100000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_VDEC_SHIFT          (21U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_VDEC_MASK           (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_VDEC_SHIFT)          /* 0x00200000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_VIO_SHIFT           (22U)
#define PMU_PMU_PWRMODE_COMMON_CON_CLR_VIO_MASK            (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CLR_VIO_SHIFT)           /* 0x00400000 */
#define PMU_PMU_PWRMODE_COMMON_CON_DDRPHY_GATING_ENABLE_SHIFT (25U)
#define PMU_PMU_PWRMODE_COMMON_CON_DDRPHY_GATING_ENABLE_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_DDRPHY_GATING_ENABLE_SHIFT) /* 0x02000000 */
#define PMU_PMU_PWRMODE_COMMON_CON_WAIT_WAKEUP_BEGIN_CFG_SHIFT (28U)
#define PMU_PMU_PWRMODE_COMMON_CON_WAIT_WAKEUP_BEGIN_CFG_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_WAIT_WAKEUP_BEGIN_CFG_SHIFT) /* 0x10000000 */
#define PMU_PMU_PWRMODE_COMMON_CON_CORE_CLK_SRC_GATE_EN_SHIFT (29U)
#define PMU_PMU_PWRMODE_COMMON_CON_CORE_CLK_SRC_GATE_EN_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_CORE_CLK_SRC_GATE_EN_SHIFT) /* 0x20000000 */
#define PMU_PMU_PWRMODE_COMMON_CON_PERI_CLK_SRC_GATE_EN_SHIFT (30U)
#define PMU_PMU_PWRMODE_COMMON_CON_PERI_CLK_SRC_GATE_EN_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_PERI_CLK_SRC_GATE_EN_SHIFT) /* 0x40000000 */
#define PMU_PMU_PWRMODE_COMMON_CON_BUS_CLK_SRC_GATE_EN_SHIFT (31U)
#define PMU_PMU_PWRMODE_COMMON_CON_BUS_CLK_SRC_GATE_EN_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_BUS_CLK_SRC_GATE_EN_SHIFT) /* 0x80000000 */
/* PMU_SFT_CON */
#define PMU_PMU_SFT_CON_WAKEUP_SFT_SHIFT                   (0U)
#define PMU_PMU_SFT_CON_WAKEUP_SFT_MASK                    (0x1U << PMU_PMU_SFT_CON_WAKEUP_SFT_SHIFT)                   /* 0x00000001 */
#define PMU_PMU_SFT_CON_APLL_PD_CFG_SHIFT                  (2U)
#define PMU_PMU_SFT_CON_APLL_PD_CFG_MASK                   (0x1U << PMU_PMU_SFT_CON_APLL_PD_CFG_SHIFT)                  /* 0x00000004 */
#define PMU_PMU_SFT_CON_DPLL_PD_CFG_SHIFT                  (3U)
#define PMU_PMU_SFT_CON_DPLL_PD_CFG_MASK                   (0x1U << PMU_PMU_SFT_CON_DPLL_PD_CFG_SHIFT)                  /* 0x00000008 */
#define PMU_PMU_SFT_CON_GPLL_PD_CFG_SHIFT                  (5U)
#define PMU_PMU_SFT_CON_GPLL_PD_CFG_MASK                   (0x1U << PMU_PMU_SFT_CON_GPLL_PD_CFG_SHIFT)                  /* 0x00000020 */
#define PMU_PMU_SFT_CON_PMU_24M_ENA_SHIFT                  (9U)
#define PMU_PMU_SFT_CON_PMU_24M_ENA_MASK                   (0x1U << PMU_PMU_SFT_CON_PMU_24M_ENA_SHIFT)                  /* 0x00000200 */
#define PMU_PMU_SFT_CON_UPCTL_C_SYSREQ_SHIFT               (10U)
#define PMU_PMU_SFT_CON_UPCTL_C_SYSREQ_MASK                (0x1U << PMU_PMU_SFT_CON_UPCTL_C_SYSREQ_SHIFT)               /* 0x00000400 */
#define PMU_PMU_SFT_CON_DDR_IO_RET_SHIFT                   (11U)
#define PMU_PMU_SFT_CON_DDR_IO_RET_MASK                    (0x1U << PMU_PMU_SFT_CON_DDR_IO_RET_SHIFT)                   /* 0x00000800 */
#define PMU_PMU_SFT_CON_PMU_LF_ENA_SHIFT                   (14U)
#define PMU_PMU_SFT_CON_PMU_LF_ENA_MASK                    (0x1U << PMU_PMU_SFT_CON_PMU_LF_ENA_SHIFT)                   /* 0x00004000 */
/* PMU_BUS_IDLE_REQ */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_DSP_CFG_SHIFT        (2U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_DSP_CFG_MASK         (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_DSP_CFG_SHIFT)        /* 0x00000004 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_CORE_CFG_SHIFT       (3U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_CORE_CFG_MASK        (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_CORE_CFG_SHIFT)       /* 0x00000008 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_BUS_CFG_SHIFT        (4U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_BUS_CFG_MASK         (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_BUS_CFG_SHIFT)        /* 0x00000010 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_PERI_CFG_SHIFT       (6U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_PERI_CFG_MASK        (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_PERI_CFG_SHIFT)       /* 0x00000040 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VDEC_CFG_SHIFT       (7U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VDEC_CFG_MASK        (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VDEC_CFG_SHIFT)       /* 0x00000080 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VIO_CFG_SHIFT        (8U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VIO_CFG_MASK         (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VIO_CFG_SHIFT)        /* 0x00000100 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_MSCH_CFG_SHIFT       (11U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_MSCH_CFG_MASK        (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_MSCH_CFG_SHIFT)       /* 0x00000800 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VENC_CFG_SHIFT       (12U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VENC_CFG_MASK        (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_VENC_CFG_SHIFT)       /* 0x00001000 */
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_PMU_CFG_SHIFT        (13U)
#define PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_PMU_CFG_MASK         (0x1U << PMU_PMU_BUS_IDLE_REQ_IDLE_REQ_PMU_CFG_SHIFT)        /* 0x00002000 */
/* PMU_BUS_IDLE_ST */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CORE_SHIFT            (3U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CORE_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CORE_SHIFT)            /* 0x00000008 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS_SHIFT             (4U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS_SHIFT)             /* 0x00000010 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_DSP_SHIFT             (5U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_DSP_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_DSP_SHIFT)             /* 0x00000020 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI_SHIFT            (6U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI_SHIFT)            /* 0x00000040 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VDEC_SHIFT            (7U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VDEC_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VDEC_SHIFT)            /* 0x00000080 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VIO_SHIFT             (8U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VIO_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VIO_SHIFT)             /* 0x00000100 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MSCH_SHIFT            (11U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MSCH_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MSCH_SHIFT)            /* 0x00000800 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VENC_SHIFT            (12U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VENC_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VENC_SHIFT)            /* 0x00001000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PMU_SHIFT             (13U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PMU_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PMU_SHIFT)             /* 0x00002000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_CORE_SHIFT                (19U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_CORE_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_CORE_SHIFT)                /* 0x00080000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_BUS_SHIFT                 (20U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_BUS_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_BUS_SHIFT)                 /* 0x00100000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_DSP_SHIFT                 (21U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_DSP_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_DSP_SHIFT)                 /* 0x00200000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_PERI_SHIFT                (22U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_PERI_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_PERI_SHIFT)                /* 0x00400000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_VDEC_SHIFT                (23U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_VDEC_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_VDEC_SHIFT)                /* 0x00800000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_VIO_SHIFT                 (24U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_VIO_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_VIO_SHIFT)                 /* 0x01000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_MSCH_SHIFT                (27U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_MSCH_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_MSCH_SHIFT)                /* 0x08000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_VENC_SHIFT                (28U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_VENC_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_VENC_SHIFT)                /* 0x10000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_PMU_SHIFT                 (29U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_PMU_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_PMU_SHIFT)                 /* 0x20000000 */
/* PMU_OSC_CNT */
#define PMU_PMU_OSC_CNT_PMU_OSC_CNT_SHIFT                  (0U)
#define PMU_PMU_OSC_CNT_PMU_OSC_CNT_MASK                   (0xFFFFFU << PMU_PMU_OSC_CNT_PMU_OSC_CNT_SHIFT)              /* 0x000FFFFF */
/* PMU_PLLLOCK_CNT */
#define PMU_PMU_PLLLOCK_CNT_PMU_PLLLOCK_CNT_SHIFT          (0U)
#define PMU_PMU_PLLLOCK_CNT_PMU_PLLLOCK_CNT_MASK           (0xFFFFFU << PMU_PMU_PLLLOCK_CNT_PMU_PLLLOCK_CNT_SHIFT)      /* 0x000FFFFF */
/* PMU_STABLE_CNT */
#define PMU_PMU_STABLE_CNT_PMU_STABLE_SHIFT                (0U)
#define PMU_PMU_STABLE_CNT_PMU_STABLE_MASK                 (0xFFFFFU << PMU_PMU_STABLE_CNT_PMU_STABLE_SHIFT)            /* 0x000FFFFF */
/* PMU_WAKEUP_RST_CLR_CNT */
#define PMU_PMU_WAKEUP_RST_CLR_CNT_PMU_WAKEUP_RST_CLR_CNT_SHIFT (0U)
#define PMU_PMU_WAKEUP_RST_CLR_CNT_PMU_WAKEUP_RST_CLR_CNT_MASK (0xFFFFFU << PMU_PMU_WAKEUP_RST_CLR_CNT_PMU_WAKEUP_RST_CLR_CNT_SHIFT) /* 0x000FFFFF */
/* PMU_DDR_SREF_ST */
#define PMU_PMU_DDR_SREF_ST_DDR_SREF_ST_SHIFT              (0U)
#define PMU_PMU_DDR_SREF_ST_DDR_SREF_ST_MASK               (0x3U << PMU_PMU_DDR_SREF_ST_DDR_SREF_ST_SHIFT)              /* 0x00000003 */
/* PMU_SYS_REG0 */
#define PMU_PMU_SYS_REG0_PMU_SYS_REG0_SHIFT                (0U)
#define PMU_PMU_SYS_REG0_PMU_SYS_REG0_MASK                 (0xFFFFFFFFU << PMU_PMU_SYS_REG0_PMU_SYS_REG0_SHIFT)         /* 0xFFFFFFFF */
/* PMU_SYS_REG1 */
#define PMU_PMU_SYS_REG1_PMU_SYS_REG1_SHIFT                (0U)
#define PMU_PMU_SYS_REG1_PMU_SYS_REG1_MASK                 (0xFFFFFFFFU << PMU_PMU_SYS_REG1_PMU_SYS_REG1_SHIFT)         /* 0xFFFFFFFF */
/* PMU_SYS_REG2 */
#define PMU_PMU_SYS_REG2_PMU_SYS_REG2_SHIFT                (0U)
#define PMU_PMU_SYS_REG2_PMU_SYS_REG2_MASK                 (0xFFFFFFFFU << PMU_PMU_SYS_REG2_PMU_SYS_REG2_SHIFT)         /* 0xFFFFFFFF */
/* PMU_SYS_REG3 */
#define PMU_PMU_SYS_REG3_PMU_SYS_REG3_SHIFT                (0U)
#define PMU_PMU_SYS_REG3_PMU_SYS_REG3_MASK                 (0xFFFFFFFFU << PMU_PMU_SYS_REG3_PMU_SYS_REG3_SHIFT)         /* 0xFFFFFFFF */
/* PMU_TIMEOUT_CNT */
#define PMU_PMU_TIMEOUT_CNT_PMU_TIMEOUT_CNT_SHIFT          (0U)
#define PMU_PMU_TIMEOUT_CNT_PMU_TIMEOUT_CNT_MASK           (0xFFFFFFFFU << PMU_PMU_TIMEOUT_CNT_PMU_TIMEOUT_CNT_SHIFT)   /* 0xFFFFFFFF */
/****************************************PMU_GRF*****************************************/
/* PMUGRF_GPIO0A_IOMUX */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT      (2U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A1_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT)      /* 0x0000000C */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT      (4U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A2_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT)      /* 0x00000030 */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT      (6U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A3_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT)      /* 0x000000C0 */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT      (8U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A4_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT)      /* 0x00000300 */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT      (10U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A5_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT)      /* 0x00000C00 */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT      (12U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A6_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT)      /* 0x00003000 */
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT      (14U)
#define PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A7_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT)      /* 0x0000C000 */
/* PMUGRF_GPIO0B_IOMUX */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT      (0U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B0_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT)      /* 0x00000003 */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B1_SEL_SHIFT      (2U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B1_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B1_SEL_SHIFT)      /* 0x0000000C */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B2_SEL_SHIFT      (4U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B2_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B2_SEL_SHIFT)      /* 0x00000030 */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B3_SEL_SHIFT      (6U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B3_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B3_SEL_SHIFT)      /* 0x000000C0 */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B4_SEL_SHIFT      (8U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B4_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B4_SEL_SHIFT)      /* 0x00000300 */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B5_SEL_SHIFT      (10U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B5_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B5_SEL_SHIFT)      /* 0x00000C00 */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B6_SEL_SHIFT      (12U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B6_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B6_SEL_SHIFT)      /* 0x00003000 */
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B7_SEL_SHIFT      (14U)
#define PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B7_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0B_IOMUX_GPIO0B7_SEL_SHIFT)      /* 0x0000C000 */
/* PMUGRF_GPIO0C_IOMUX */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C0_SEL_SHIFT      (0U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C0_SEL_MASK       (0x1U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C0_SEL_SHIFT)      /* 0x00000001 */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C1_SEL_SHIFT      (2U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C1_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C1_SEL_SHIFT)      /* 0x0000000C */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C2_SEL_SHIFT      (4U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C2_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C2_SEL_SHIFT)      /* 0x00000030 */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C3_SEL_SHIFT      (6U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C3_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C3_SEL_SHIFT)      /* 0x000000C0 */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C4_SEL_SHIFT      (8U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C4_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C4_SEL_SHIFT)      /* 0x00000300 */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C5_SEL_SHIFT      (10U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C5_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C5_SEL_SHIFT)      /* 0x00000C00 */
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C6_SEL_SHIFT      (12U)
#define PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C6_SEL_MASK       (0x3U << PMU_GRF_PMUGRF_GPIO0C_IOMUX_GPIO0C6_SEL_SHIFT)      /* 0x00003000 */
/* PMUGRF_GPIO0A_P */
#define PMU_GRF_PMUGRF_GPIO0A_P_GPIO0A_P_SHIFT             (0U)
#define PMU_GRF_PMUGRF_GPIO0A_P_GPIO0A_P_MASK              (0xFFFFU << PMU_GRF_PMUGRF_GPIO0A_P_GPIO0A_P_SHIFT)          /* 0x0000FFFF */
/* PMUGRF_GPIO0B_P */
#define PMU_GRF_PMUGRF_GPIO0B_P_GPIO0B_P_SHIFT             (0U)
#define PMU_GRF_PMUGRF_GPIO0B_P_GPIO0B_P_MASK              (0xFFFFU << PMU_GRF_PMUGRF_GPIO0B_P_GPIO0B_P_SHIFT)          /* 0x0000FFFF */
/* PMUGRF_GPIO0C_P */
#define PMU_GRF_PMUGRF_GPIO0C_P_GPIO0C_P_SHIFT             (0U)
#define PMU_GRF_PMUGRF_GPIO0C_P_GPIO0C_P_MASK              (0xFFFFU << PMU_GRF_PMUGRF_GPIO0C_P_GPIO0C_P_SHIFT)          /* 0x0000FFFF */
/* PMUGRF_GPIO0A_E */
#define PMU_GRF_PMUGRF_GPIO0A_E_GPIO0A_E_SHIFT             (0U)
#define PMU_GRF_PMUGRF_GPIO0A_E_GPIO0A_E_MASK              (0xFFFFU << PMU_GRF_PMUGRF_GPIO0A_E_GPIO0A_E_SHIFT)          /* 0x0000FFFF */
/* PMUGRF_GPIO0B_E */
#define PMU_GRF_PMUGRF_GPIO0B_E_GPIO0B_E_SHIFT             (0U)
#define PMU_GRF_PMUGRF_GPIO0B_E_GPIO0B_E_MASK              (0xFFFFU << PMU_GRF_PMUGRF_GPIO0B_E_GPIO0B_E_SHIFT)          /* 0x0000FFFF */
/* PMUGRF_GPIO0C_E */
#define PMU_GRF_PMUGRF_GPIO0C_E_GPIO0C_E_SHIFT             (0U)
#define PMU_GRF_PMUGRF_GPIO0C_E_GPIO0C_E_MASK              (0xFFFFU << PMU_GRF_PMUGRF_GPIO0C_E_GPIO0C_E_SHIFT)          /* 0x0000FFFF */
/* PMUGRF_GPIO0A_SMT */
#define PMU_GRF_PMUGRF_GPIO0A_SMT_GPIO0A_SMT_SHIFT         (0U)
#define PMU_GRF_PMUGRF_GPIO0A_SMT_GPIO0A_SMT_MASK          (0xFFU << PMU_GRF_PMUGRF_GPIO0A_SMT_GPIO0A_SMT_SHIFT)        /* 0x000000FF */
/* PMUGRF_GPIO0B_SMT */
#define PMU_GRF_PMUGRF_GPIO0B_SMT_GPIO0B_SMT_SHIFT         (0U)
#define PMU_GRF_PMUGRF_GPIO0B_SMT_GPIO0B_SMT_MASK          (0xFFU << PMU_GRF_PMUGRF_GPIO0B_SMT_GPIO0B_SMT_SHIFT)        /* 0x000000FF */
/* PMUGRF_GPIO0C_SMT */
#define PMU_GRF_PMUGRF_GPIO0C_SMT_GPIO0C_SMT_SHIFT         (0U)
#define PMU_GRF_PMUGRF_GPIO0C_SMT_GPIO0C_SMT_MASK          (0xFFU << PMU_GRF_PMUGRF_GPIO0C_SMT_GPIO0C_SMT_SHIFT)        /* 0x000000FF */
/* PMUGRF_GPIO0A_SR */
#define PMU_GRF_PMUGRF_GPIO0A_SR_GPIO0A_SR_SHIFT           (0U)
#define PMU_GRF_PMUGRF_GPIO0A_SR_GPIO0A_SR_MASK            (0xFFU << PMU_GRF_PMUGRF_GPIO0A_SR_GPIO0A_SR_SHIFT)          /* 0x000000FF */
/* PMUGRF_GPIO0B_SR */
#define PMU_GRF_PMUGRF_GPIO0B_SR_GPIO0B_SR_SHIFT           (0U)
#define PMU_GRF_PMUGRF_GPIO0B_SR_GPIO0B_SR_MASK            (0xFFU << PMU_GRF_PMUGRF_GPIO0B_SR_GPIO0B_SR_SHIFT)          /* 0x000000FF */
/* PMUGRF_GPIO0C_SR */
#define PMU_GRF_PMUGRF_GPIO0C_SR_GPIO0C_SR_SHIFT           (0U)
#define PMU_GRF_PMUGRF_GPIO0C_SR_GPIO0C_SR_MASK            (0xFFU << PMU_GRF_PMUGRF_GPIO0C_SR_GPIO0C_SR_SHIFT)          /* 0x000000FF */
/* PMUGRF_SOC_CON0 */
#define PMU_GRF_PMUGRF_SOC_CON0_I2C_PMU_INT_WAKEUP_EN_SHIFT (0U)
#define PMU_GRF_PMUGRF_SOC_CON0_I2C_PMU_INT_WAKEUP_EN_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_I2C_PMU_INT_WAKEUP_EN_SHIFT) /* 0x00000001 */
#define PMU_GRF_PMUGRF_SOC_CON0_PWM_PMU_INT_WAKEUP_EN_SHIFT (1U)
#define PMU_GRF_PMUGRF_SOC_CON0_PWM_PMU_INT_WAKEUP_EN_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PWM_PMU_INT_WAKEUP_EN_SHIFT) /* 0x00000002 */
#define PMU_GRF_PMUGRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_EN_SHIFT (2U)
#define PMU_GRF_PMUGRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_EN_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_EN_SHIFT) /* 0x00000004 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PWMPMU0_SHIFT (3U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PWMPMU0_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PWMPMU0_SHIFT) /* 0x00000008 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PWMPMU1_SHIFT (4U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PWMPMU1_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PWMPMU1_SHIFT) /* 0x00000010 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_NIU_SHIFT (5U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_NIU_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_NIU_SHIFT) /* 0x00000020 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_GPIO0_SHIFT (6U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_GPIO0_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_GPIO0_SHIFT) /* 0x00000040 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUGRF_SHIFT (8U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUGRF_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUGRF_SHIFT) /* 0x00000100 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUPVTM_SHIFT (10U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUPVTM_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUPVTM_SHIFT) /* 0x00000400 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUI2C_SHIFT (12U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUI2C_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_PMUI2C_SHIFT) /* 0x00001000 */
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_INTMEM1_SHIFT (13U)
#define PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_INTMEM1_MASK (0x1U << PMU_GRF_PMUGRF_SOC_CON0_PD_PMU_RESET_HOLD_INTMEM1_SHIFT) /* 0x00002000 */
#define PMU_GRF_PMUGRF_SOC_CON0_WIFICLK_SEL_SHIFT          (15U)
#define PMU_GRF_PMUGRF_SOC_CON0_WIFICLK_SEL_MASK           (0x1U << PMU_GRF_PMUGRF_SOC_CON0_WIFICLK_SEL_SHIFT)          /* 0x00008000 */
/* PMUGRF_SOC_CON1 */
#define PMU_GRF_PMUGRF_SOC_CON1_POC_PMU_SEL18_SHIFT        (0U)
#define PMU_GRF_PMUGRF_SOC_CON1_POC_PMU_SEL18_MASK         (0x1U << PMU_GRF_PMUGRF_SOC_CON1_POC_PMU_SEL18_SHIFT)        /* 0x00000001 */
/* PMUGRF_SOC_CON2 */
#define PMU_GRF_PMUGRF_SOC_CON2_GRF_SDMMC_DETTIMER_SHIFT   (0U)
#define PMU_GRF_PMUGRF_SOC_CON2_GRF_SDMMC_DETTIMER_MASK    (0xFFFFU << PMU_GRF_PMUGRF_SOC_CON2_GRF_SDMMC_DETTIMER_SHIFT) /* 0x0000FFFF */
/* PMUGRF_SOC_CON3 */
#define PMU_GRF_PMUGRF_SOC_CON3_GRF_SDMMC_DETTIME_SHIFT    (0U)
#define PMU_GRF_PMUGRF_SOC_CON3_GRF_SDMMC_DETTIME_MASK     (0xFU << PMU_GRF_PMUGRF_SOC_CON3_GRF_SDMMC_DETTIME_SHIFT)    /* 0x0000000F */
/* PMUGRF_DLL_CON0 */
#define PMU_GRF_PMUGRF_DLL_CON0_PVTM_PMU_START_SHIFT       (0U)
#define PMU_GRF_PMUGRF_DLL_CON0_PVTM_PMU_START_MASK        (0x1U << PMU_GRF_PMUGRF_DLL_CON0_PVTM_PMU_START_SHIFT)       /* 0x00000001 */
#define PMU_GRF_PMUGRF_DLL_CON0_PVTM_PMU_OSC_EN_SHIFT      (1U)
#define PMU_GRF_PMUGRF_DLL_CON0_PVTM_PMU_OSC_EN_MASK       (0x1U << PMU_GRF_PMUGRF_DLL_CON0_PVTM_PMU_OSC_EN_SHIFT)      /* 0x00000002 */
#define PMU_GRF_PMUGRF_DLL_CON0_PVTMGRF_CON_PVTM_CLKOUT_DIV_SHIFT (2U)
#define PMU_GRF_PMUGRF_DLL_CON0_PVTMGRF_CON_PVTM_CLKOUT_DIV_MASK (0xFFFU << PMU_GRF_PMUGRF_DLL_CON0_PVTMGRF_CON_PVTM_CLKOUT_DIV_SHIFT) /* 0x00003FFC */
/* PMUGRF_DLL_CON1 */
#define PMU_GRF_PMUGRF_DLL_CON1_PVTM_PMU_CAL_CNT_SHIFT     (0U)
#define PMU_GRF_PMUGRF_DLL_CON1_PVTM_PMU_CAL_CNT_MASK      (0xFFFFFFFFU << PMU_GRF_PMUGRF_DLL_CON1_PVTM_PMU_CAL_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMUGRF_DLL_STATUS0 */
#define PMU_GRF_PMUGRF_DLL_STATUS0_PVTM_PMU_FREQ_DONE_SHIFT (0U)
#define PMU_GRF_PMUGRF_DLL_STATUS0_PVTM_PMU_FREQ_DONE_MASK (0x1U << PMU_GRF_PMUGRF_DLL_STATUS0_PVTM_PMU_FREQ_DONE_SHIFT) /* 0x00000001 */
/* PMUGRF_DLL_STATUS1 */
#define PMU_GRF_PMUGRF_DLL_STATUS1_PVTM_PMU_FREQ_CNT_SHIFT (0U)
#define PMU_GRF_PMUGRF_DLL_STATUS1_PVTM_PMU_FREQ_CNT_MASK  (0xFFFFFFFFU << PMU_GRF_PMUGRF_DLL_STATUS1_PVTM_PMU_FREQ_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMUGRF_OS_REG0 */
#define PMU_GRF_PMUGRF_OS_REG0_GRF_OS_REG0_SHIFT           (0U)
#define PMU_GRF_PMUGRF_OS_REG0_GRF_OS_REG0_MASK            (0xFFFFFFFFU << PMU_GRF_PMUGRF_OS_REG0_GRF_OS_REG0_SHIFT)    /* 0xFFFFFFFF */
/* PMUGRF_OS_REG1 */
#define PMU_GRF_PMUGRF_OS_REG1_GRF_OS_REG1_SHIFT           (0U)
#define PMU_GRF_PMUGRF_OS_REG1_GRF_OS_REG1_MASK            (0xFFFFFFFFU << PMU_GRF_PMUGRF_OS_REG1_GRF_OS_REG1_SHIFT)    /* 0xFFFFFFFF */
/* PMUGRF_OS_REG2 */
#define PMU_GRF_PMUGRF_OS_REG2_GRF_OS_REG2_SHIFT           (0U)
#define PMU_GRF_PMUGRF_OS_REG2_GRF_OS_REG2_MASK            (0xFFFFFFFFU << PMU_GRF_PMUGRF_OS_REG2_GRF_OS_REG2_SHIFT)    /* 0xFFFFFFFF */
/* PMUGRF_OS_REG3 */
#define PMU_GRF_PMUGRF_OS_REG3_GRF_OS_REG3_SHIFT           (0U)
#define PMU_GRF_PMUGRF_OS_REG3_GRF_OS_REG3_MASK            (0xFFFFFFFFU << PMU_GRF_PMUGRF_OS_REG3_GRF_OS_REG3_SHIFT)    /* 0xFFFFFFFF */
/* PMUGRF_FAST_BOOT_ADDR */
#define PMU_GRF_PMUGRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_SHIFT (0U)
#define PMU_GRF_PMUGRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_MASK  (0xFFFFFFFFU << PMU_GRF_PMUGRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_SHIFT) /* 0xFFFFFFFF */
/* PMUGRF_A7_JTAG_MASK */
#define PMU_GRF_PMUGRF_A7_JTAG_MASK_GRF_CON_A7_JTAG_MASK_SHIFT (0U)
#define PMU_GRF_PMUGRF_A7_JTAG_MASK_GRF_CON_A7_JTAG_MASK_MASK (0x1U << PMU_GRF_PMUGRF_A7_JTAG_MASK_GRF_CON_A7_JTAG_MASK_SHIFT) /* 0x00000001 */
/* PMUGRF_CEVA_JTAG_MASK */
#define PMU_GRF_PMUGRF_CEVA_JTAG_MASK_GRF_CON_CEVA_JTAG_MASK_SHIFT (0U)
#define PMU_GRF_PMUGRF_CEVA_JTAG_MASK_GRF_CON_CEVA_JTAG_MASK_MASK (0x1U << PMU_GRF_PMUGRF_CEVA_JTAG_MASK_GRF_CON_CEVA_JTAG_MASK_SHIFT) /* 0x00000001 */
/******************************************CRU*******************************************/
/* APLL_CON0 */
#define CRU_APLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_APLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_APLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
/* APLL_CON1 */
#define CRU_APLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_APLL_CON1_REFDIV_MASK                          (0x3FU << CRU_APLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_APLL_CON1_POSTDIV1_SHIFT                       (8U)
#define CRU_APLL_CON1_POSTDIV1_MASK                        (0x7U << CRU_APLL_CON1_POSTDIV1_SHIFT)                       /* 0x00000700 */
#define CRU_APLL_CON1_POSTDIV2_SHIFT                       (12U)
#define CRU_APLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_APLL_CON1_POSTDIV2_SHIFT)                       /* 0x00007000 */
/* APLL_CON2 */
#define CRU_APLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_APLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_APLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_APLL_CON2_PLL_LOCK_SHIFT                       (31U)
#define CRU_APLL_CON2_PLL_LOCK_MASK                        (0x1U << CRU_APLL_CON2_PLL_LOCK_SHIFT)                       /* 0x80000000 */
/* APLL_CON3 */
#define CRU_APLL_CON3_POWER_DOWN_SHIFT                     (0U)
#define CRU_APLL_CON3_POWER_DOWN_MASK                      (0x1U << CRU_APLL_CON3_POWER_DOWN_SHIFT)                     /* 0x00000001 */
#define CRU_APLL_CON3_BYPASS_SHIFT                         (1U)
#define CRU_APLL_CON3_BYPASS_MASK                          (0x1U << CRU_APLL_CON3_BYPASS_SHIFT)                         /* 0x00000002 */
#define CRU_APLL_CON3_DACPD_SHIFT                          (2U)
#define CRU_APLL_CON3_DACPD_MASK                           (0x1U << CRU_APLL_CON3_DACPD_SHIFT)                          /* 0x00000004 */
#define CRU_APLL_CON3_DSMPD_SHIFT                          (3U)
#define CRU_APLL_CON3_DSMPD_MASK                           (0x1U << CRU_APLL_CON3_DSMPD_SHIFT)                          /* 0x00000008 */
#define CRU_APLL_CON3_FOUTPOSTDIVPD_SHIFT                  (4U)
#define CRU_APLL_CON3_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_APLL_CON3_FOUTPOSTDIVPD_SHIFT)                  /* 0x00000010 */
#define CRU_APLL_CON3_FOUTVCOPD_SHIFT                      (5U)
#define CRU_APLL_CON3_FOUTVCOPD_MASK                       (0x1U << CRU_APLL_CON3_FOUTVCOPD_SHIFT)                      /* 0x00000020 */
#define CRU_APLL_CON3_FOUT4PHASEPD_SHIFT                   (6U)
#define CRU_APLL_CON3_FOUT4PHASEPD_MASK                    (0x1U << CRU_APLL_CON3_FOUT4PHASEPD_SHIFT)                   /* 0x00000040 */
#define CRU_APLL_CON3_PLL_WORK_MODE_SHIFT                  (8U)
#define CRU_APLL_CON3_PLL_WORK_MODE_MASK                   (0x1U << CRU_APLL_CON3_PLL_WORK_MODE_SHIFT)                  /* 0x00000100 */
/* APLL_CON4 */
#define CRU_APLL_CON4_SSMOD_BP_SHIFT                       (0U)
#define CRU_APLL_CON4_SSMOD_BP_MASK                        (0x1U << CRU_APLL_CON4_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_APLL_CON4_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_APLL_CON4_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_APLL_CON4_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_APLL_CON4_SSMOD_RESET_SHIFT                    (2U)
#define CRU_APLL_CON4_SSMOD_RESET_MASK                     (0x1U << CRU_APLL_CON4_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_APLL_CON4_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_APLL_CON4_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_APLL_CON4_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_APLL_CON4_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_APLL_CON4_SSMOD_DIVVAL_MASK                    (0xFU << CRU_APLL_CON4_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_APLL_CON4_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_APLL_CON4_SSMOD_SPREAD_MASK                    (0x1FU << CRU_APLL_CON4_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* APLL_CON5 */
#define CRU_APLL_CON5_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_APLL_CON5_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_APLL_CON5_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_APLL_CON5_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_APLL_CON5_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_APLL_CON5_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* DPLL_CON0 */
#define CRU_DPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_DPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_DPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
/* DPLL_CON1 */
#define CRU_DPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_DPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_DPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_DPLL_CON1_POSTDIV1_SHIFT                       (8U)
#define CRU_DPLL_CON1_POSTDIV1_MASK                        (0x7U << CRU_DPLL_CON1_POSTDIV1_SHIFT)                       /* 0x00000700 */
#define CRU_DPLL_CON1_POSTDIV2_SHIFT                       (12U)
#define CRU_DPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_DPLL_CON1_POSTDIV2_SHIFT)                       /* 0x00007000 */
/* DPLL_CON2 */
#define CRU_DPLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_DPLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_DPLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_DPLL_CON2_PLL_LOCK_SHIFT                       (31U)
#define CRU_DPLL_CON2_PLL_LOCK_MASK                        (0x1U << CRU_DPLL_CON2_PLL_LOCK_SHIFT)                       /* 0x80000000 */
/* DPLL_CON3 */
#define CRU_DPLL_CON3_POWER_DOWN_SHIFT                     (0U)
#define CRU_DPLL_CON3_POWER_DOWN_MASK                      (0x1U << CRU_DPLL_CON3_POWER_DOWN_SHIFT)                     /* 0x00000001 */
#define CRU_DPLL_CON3_BYPASS_SHIFT                         (1U)
#define CRU_DPLL_CON3_BYPASS_MASK                          (0x1U << CRU_DPLL_CON3_BYPASS_SHIFT)                         /* 0x00000002 */
#define CRU_DPLL_CON3_DACPD_SHIFT                          (2U)
#define CRU_DPLL_CON3_DACPD_MASK                           (0x1U << CRU_DPLL_CON3_DACPD_SHIFT)                          /* 0x00000004 */
#define CRU_DPLL_CON3_DSMPD_SHIFT                          (3U)
#define CRU_DPLL_CON3_DSMPD_MASK                           (0x1U << CRU_DPLL_CON3_DSMPD_SHIFT)                          /* 0x00000008 */
#define CRU_DPLL_CON3_FOUTPOSTDIVPD_SHIFT                  (4U)
#define CRU_DPLL_CON3_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_DPLL_CON3_FOUTPOSTDIVPD_SHIFT)                  /* 0x00000010 */
#define CRU_DPLL_CON3_FOUTVCOPD_SHIFT                      (5U)
#define CRU_DPLL_CON3_FOUTVCOPD_MASK                       (0x1U << CRU_DPLL_CON3_FOUTVCOPD_SHIFT)                      /* 0x00000020 */
#define CRU_DPLL_CON3_FOUT4PHASEPD_SHIFT                   (6U)
#define CRU_DPLL_CON3_FOUT4PHASEPD_MASK                    (0x1U << CRU_DPLL_CON3_FOUT4PHASEPD_SHIFT)                   /* 0x00000040 */
#define CRU_DPLL_CON3_PLL_WORK_MODE_SHIFT                  (8U)
#define CRU_DPLL_CON3_PLL_WORK_MODE_MASK                   (0x1U << CRU_DPLL_CON3_PLL_WORK_MODE_SHIFT)                  /* 0x00000100 */
/* DPLL_CON4 */
#define CRU_DPLL_CON4_SSMOD_BP_SHIFT                       (0U)
#define CRU_DPLL_CON4_SSMOD_BP_MASK                        (0x1U << CRU_DPLL_CON4_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_DPLL_CON4_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_DPLL_CON4_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_DPLL_CON4_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_DPLL_CON4_SSMOD_RESET_SHIFT                    (2U)
#define CRU_DPLL_CON4_SSMOD_RESET_MASK                     (0x1U << CRU_DPLL_CON4_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_DPLL_CON4_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_DPLL_CON4_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_DPLL_CON4_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_DPLL_CON4_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_DPLL_CON4_SSMOD_DIVVAL_MASK                    (0xFU << CRU_DPLL_CON4_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_DPLL_CON4_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_DPLL_CON4_SSMOD_SPREAD_MASK                    (0x1FU << CRU_DPLL_CON4_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* DPLL_CON5 */
#define CRU_DPLL_CON5_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_DPLL_CON5_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_DPLL_CON5_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_DPLL_CON5_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_DPLL_CON5_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_DPLL_CON5_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* GPLL_CON0 */
#define CRU_GPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_GPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_GPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
/* GPLL_CON1 */
#define CRU_GPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_GPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_GPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_GPLL_CON1_POSTDIV1_SHIFT                       (8U)
#define CRU_GPLL_CON1_POSTDIV1_MASK                        (0x7U << CRU_GPLL_CON1_POSTDIV1_SHIFT)                       /* 0x00000700 */
#define CRU_GPLL_CON1_POSTDIV2_SHIFT                       (12U)
#define CRU_GPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_GPLL_CON1_POSTDIV2_SHIFT)                       /* 0x00007000 */
/* GPLL_CON2 */
#define CRU_GPLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_GPLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_GPLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_GPLL_CON2_PLL_LOCK_SHIFT                       (31U)
#define CRU_GPLL_CON2_PLL_LOCK_MASK                        (0x1U << CRU_GPLL_CON2_PLL_LOCK_SHIFT)                       /* 0x80000000 */
/* GPLL_CON3 */
#define CRU_GPLL_CON3_POWER_DOWN_SHIFT                     (0U)
#define CRU_GPLL_CON3_POWER_DOWN_MASK                      (0x1U << CRU_GPLL_CON3_POWER_DOWN_SHIFT)                     /* 0x00000001 */
#define CRU_GPLL_CON3_BYPASS_SHIFT                         (1U)
#define CRU_GPLL_CON3_BYPASS_MASK                          (0x1U << CRU_GPLL_CON3_BYPASS_SHIFT)                         /* 0x00000002 */
#define CRU_GPLL_CON3_DACPD_SHIFT                          (2U)
#define CRU_GPLL_CON3_DACPD_MASK                           (0x1U << CRU_GPLL_CON3_DACPD_SHIFT)                          /* 0x00000004 */
#define CRU_GPLL_CON3_DSMPD_SHIFT                          (3U)
#define CRU_GPLL_CON3_DSMPD_MASK                           (0x1U << CRU_GPLL_CON3_DSMPD_SHIFT)                          /* 0x00000008 */
#define CRU_GPLL_CON3_FOUTPOSTDIVPD_SHIFT                  (4U)
#define CRU_GPLL_CON3_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_GPLL_CON3_FOUTPOSTDIVPD_SHIFT)                  /* 0x00000010 */
#define CRU_GPLL_CON3_FOUTVCOPD_SHIFT                      (5U)
#define CRU_GPLL_CON3_FOUTVCOPD_MASK                       (0x1U << CRU_GPLL_CON3_FOUTVCOPD_SHIFT)                      /* 0x00000020 */
#define CRU_GPLL_CON3_FOUT4PHASEPD_SHIFT                   (6U)
#define CRU_GPLL_CON3_FOUT4PHASEPD_MASK                    (0x1U << CRU_GPLL_CON3_FOUT4PHASEPD_SHIFT)                   /* 0x00000040 */
#define CRU_GPLL_CON3_PLL_WORK_MODE_SHIFT                  (8U)
#define CRU_GPLL_CON3_PLL_WORK_MODE_MASK                   (0x1U << CRU_GPLL_CON3_PLL_WORK_MODE_SHIFT)                  /* 0x00000100 */
/* GPLL_CON4 */
#define CRU_GPLL_CON4_SSMOD_BP_SHIFT                       (0U)
#define CRU_GPLL_CON4_SSMOD_BP_MASK                        (0x1U << CRU_GPLL_CON4_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_GPLL_CON4_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_GPLL_CON4_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_GPLL_CON4_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_GPLL_CON4_SSMOD_RESET_SHIFT                    (2U)
#define CRU_GPLL_CON4_SSMOD_RESET_MASK                     (0x1U << CRU_GPLL_CON4_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_GPLL_CON4_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_GPLL_CON4_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_GPLL_CON4_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_GPLL_CON4_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_GPLL_CON4_SSMOD_DIVVAL_MASK                    (0xFU << CRU_GPLL_CON4_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_GPLL_CON4_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_GPLL_CON4_SSMOD_SPREAD_MASK                    (0x1FU << CRU_GPLL_CON4_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* GPLL_CON5 */
#define CRU_GPLL_CON5_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_GPLL_CON5_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_GPLL_CON5_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_GPLL_CON5_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_GPLL_CON5_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_GPLL_CON5_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* CLKSEL0_CON */
#define CRU_CLKSEL0_CON_CLK_CORE_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL0_CON_CLK_CORE_DIV_CON_MASK              (0x1FU << CRU_CLKSEL0_CON_CLK_CORE_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL0_CON_CLK_CORE_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL0_CON_CLK_CORE_PLL_SEL_MASK              (0x3U << CRU_CLKSEL0_CON_CLK_CORE_PLL_SEL_SHIFT)             /* 0x00000300 */
/* CLKSEL1_CON */
#define CRU_CLKSEL1_CON_ACLK_CORE_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL1_CON_ACLK_CORE_DIV_CON_MASK             (0x7U << CRU_CLKSEL1_CON_ACLK_CORE_DIV_CON_SHIFT)            /* 0x00000007 */
#define CRU_CLKSEL1_CON_PCLK_DBG_DIV_CON_SHIFT             (4U)
#define CRU_CLKSEL1_CON_PCLK_DBG_DIV_CON_MASK              (0xFU << CRU_CLKSEL1_CON_PCLK_DBG_DIV_CON_SHIFT)             /* 0x000000F0 */
/* CLKSEL2_CON */
#define CRU_CLKSEL2_CON_ACLK_BUS_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL2_CON_ACLK_BUS_DIV_CON_MASK              (0x1FU << CRU_CLKSEL2_CON_ACLK_BUS_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL2_CON_ACLK_BUS_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL2_CON_ACLK_BUS_PLL_SEL_MASK              (0x3U << CRU_CLKSEL2_CON_ACLK_BUS_PLL_SEL_SHIFT)             /* 0x00000300 */
/* CLKSEL3_CON */
#define CRU_CLKSEL3_CON_HCLK_BUS_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL3_CON_HCLK_BUS_DIV_CON_MASK              (0x1FU << CRU_CLKSEL3_CON_HCLK_BUS_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL3_CON_PCLK_BUS_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL3_CON_PCLK_BUS_DIV_CON_MASK              (0x1FU << CRU_CLKSEL3_CON_PCLK_BUS_DIV_CON_SHIFT)            /* 0x00001F00 */
/* CLKSEL4_CON */
#define CRU_CLKSEL4_CON_CLK_DDR_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL4_CON_CLK_DDR_DIV_CON_MASK               (0x7U << CRU_CLKSEL4_CON_CLK_DDR_DIV_CON_SHIFT)              /* 0x00000007 */
#define CRU_CLKSEL4_CON_CLK_DDR_PLL_SEL_SHIFT              (8U)
#define CRU_CLKSEL4_CON_CLK_DDR_PLL_SEL_MASK               (0x3U << CRU_CLKSEL4_CON_CLK_DDR_PLL_SEL_SHIFT)              /* 0x00000300 */
/* CLKSEL5_CON */
#define CRU_CLKSEL5_CON_CLK_I2S0_PLL_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL5_CON_CLK_I2S0_PLL_DIV_CON_MASK          (0x7FU << CRU_CLKSEL5_CON_CLK_I2S0_PLL_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL5_CON_CLK_I2S0_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL5_CON_CLK_I2S0_PLL_SEL_MASK              (0x1U << CRU_CLKSEL5_CON_CLK_I2S0_PLL_SEL_SHIFT)             /* 0x00000100 */
#define CRU_CLKSEL5_CON_CLK_I2S0_SEL_SHIFT                 (12U)
#define CRU_CLKSEL5_CON_CLK_I2S0_SEL_MASK                  (0x3U << CRU_CLKSEL5_CON_CLK_I2S0_SEL_SHIFT)                 /* 0x00003000 */
#define CRU_CLKSEL5_CON_I2S0_OUT_SEL_SHIFT                 (15U)
#define CRU_CLKSEL5_CON_I2S0_OUT_SEL_MASK                  (0x1U << CRU_CLKSEL5_CON_I2S0_OUT_SEL_SHIFT)                 /* 0x00008000 */
/* CLKSEL6_CON */
#define CRU_CLKSEL6_CON_CLK_I2S1_PLL_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL6_CON_CLK_I2S1_PLL_DIV_CON_MASK          (0x7FU << CRU_CLKSEL6_CON_CLK_I2S1_PLL_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL6_CON_CLK_I2S1_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL6_CON_CLK_I2S1_PLL_SEL_MASK              (0x1U << CRU_CLKSEL6_CON_CLK_I2S1_PLL_SEL_SHIFT)             /* 0x00000100 */
#define CRU_CLKSEL6_CON_CLK_I2S1_SEL_SHIFT                 (12U)
#define CRU_CLKSEL6_CON_CLK_I2S1_SEL_MASK                  (0x3U << CRU_CLKSEL6_CON_CLK_I2S1_SEL_SHIFT)                 /* 0x00003000 */
/* CLKSEL7_CON */
#define CRU_CLKSEL7_CON_CLK_I2S2_PLL_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL7_CON_CLK_I2S2_PLL_DIV_CON_MASK          (0x7FU << CRU_CLKSEL7_CON_CLK_I2S2_PLL_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL7_CON_CLK_I2S2_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL7_CON_CLK_I2S2_PLL_SEL_MASK              (0x1U << CRU_CLKSEL7_CON_CLK_I2S2_PLL_SEL_SHIFT)             /* 0x00000100 */
#define CRU_CLKSEL7_CON_CLK_I2S2_SEL_SHIFT                 (12U)
#define CRU_CLKSEL7_CON_CLK_I2S2_SEL_MASK                  (0x3U << CRU_CLKSEL7_CON_CLK_I2S2_SEL_SHIFT)                 /* 0x00003000 */
/* CLKSEL8_CON */
#define CRU_CLKSEL8_CON_I2S0_FRAC_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL8_CON_I2S0_FRAC_DIV_CON_MASK             (0xFFFFFFFFU << CRU_CLKSEL8_CON_I2S0_FRAC_DIV_CON_SHIFT)     /* 0xFFFFFFFF */
/* CLKSEL9_CON */
#define CRU_CLKSEL9_CON_I2S1_FRAC_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL9_CON_I2S1_FRAC_DIV_CON_MASK             (0xFFFFFFFFU << CRU_CLKSEL9_CON_I2S1_FRAC_DIV_CON_SHIFT)     /* 0xFFFFFFFF */
/* CLKSEL10_CON */
#define CRU_CLKSEL10_CON_I2S2_FRAC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL10_CON_I2S2_FRAC_DIV_CON_MASK            (0xFFFFFFFFU << CRU_CLKSEL10_CON_I2S2_FRAC_DIV_CON_SHIFT)    /* 0xFFFFFFFF */
/* CLKSEL11_CON */
#define CRU_CLKSEL11_CON_CLK_CRYPTO_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL11_CON_CLK_CRYPTO_DIV_CON_MASK           (0x1FU << CRU_CLKSEL11_CON_CLK_CRYPTO_DIV_CON_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL11_CON_CLK_CRYPTO_PLL_SEL_SHIFT          (7U)
#define CRU_CLKSEL11_CON_CLK_CRYPTO_PLL_SEL_MASK           (0x1U << CRU_CLKSEL11_CON_CLK_CRYPTO_PLL_SEL_SHIFT)          /* 0x00000080 */
#define CRU_CLKSEL11_CON_CLK_SPI_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL11_CON_CLK_SPI_DIV_CON_MASK              (0x7FU << CRU_CLKSEL11_CON_CLK_SPI_DIV_CON_SHIFT)            /* 0x00007F00 */
#define CRU_CLKSEL11_CON_CLK_SPI_PLL_SEL_SHIFT             (15U)
#define CRU_CLKSEL11_CON_CLK_SPI_PLL_SEL_MASK              (0x1U << CRU_CLKSEL11_CON_CLK_SPI_PLL_SEL_SHIFT)             /* 0x00008000 */
/* CLKSEL12_CON */
#define CRU_CLKSEL12_CON_PWM0_PMU_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL12_CON_PWM0_PMU_DIV_CON_MASK             (0x7FU << CRU_CLKSEL12_CON_PWM0_PMU_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL12_CON_PWM0_PMU_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL12_CON_PWM0_PMU_PLL_SEL_MASK             (0x1U << CRU_CLKSEL12_CON_PWM0_PMU_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL12_CON_PWM1_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL12_CON_PWM1_DIV_CON_MASK                 (0x7FU << CRU_CLKSEL12_CON_PWM1_DIV_CON_SHIFT)               /* 0x00007F00 */
#define CRU_CLKSEL12_CON_PWM1_PLL_SEL_SHIFT                (15U)
#define CRU_CLKSEL12_CON_PWM1_PLL_SEL_MASK                 (0x1U << CRU_CLKSEL12_CON_PWM1_PLL_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL13_CON */
#define CRU_CLKSEL13_CON_CLK_UART0_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL13_CON_CLK_UART0_DIV_CON_MASK            (0x7FU << CRU_CLKSEL13_CON_CLK_UART0_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL13_CON_CLK_UART0_SEL_SHIFT               (8U)
#define CRU_CLKSEL13_CON_CLK_UART0_SEL_MASK                (0x3U << CRU_CLKSEL13_CON_CLK_UART0_SEL_SHIFT)               /* 0x00000300 */
#define CRU_CLKSEL13_CON_UART0_PLL_SEL_SHIFT               (12U)
#define CRU_CLKSEL13_CON_UART0_PLL_SEL_MASK                (0x3U << CRU_CLKSEL13_CON_UART0_PLL_SEL_SHIFT)               /* 0x00003000 */
/* CLKSEL14_CON */
#define CRU_CLKSEL14_CON_CLK_UART1_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL14_CON_CLK_UART1_DIV_CON_MASK            (0x7FU << CRU_CLKSEL14_CON_CLK_UART1_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL14_CON_CLK_UART1_SEL_SHIFT               (8U)
#define CRU_CLKSEL14_CON_CLK_UART1_SEL_MASK                (0x3U << CRU_CLKSEL14_CON_CLK_UART1_SEL_SHIFT)               /* 0x00000300 */
#define CRU_CLKSEL14_CON_UART1_PLL_SEL_SHIFT               (12U)
#define CRU_CLKSEL14_CON_UART1_PLL_SEL_MASK                (0x3U << CRU_CLKSEL14_CON_UART1_PLL_SEL_SHIFT)               /* 0x00003000 */
/* CLKSEL15_CON */
#define CRU_CLKSEL15_CON_CLK_UART2_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL15_CON_CLK_UART2_DIV_CON_MASK            (0x7FU << CRU_CLKSEL15_CON_CLK_UART2_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL15_CON_CLK_UART2_SEL_SHIFT               (8U)
#define CRU_CLKSEL15_CON_CLK_UART2_SEL_MASK                (0x3U << CRU_CLKSEL15_CON_CLK_UART2_SEL_SHIFT)               /* 0x00000300 */
#define CRU_CLKSEL15_CON_UART2_PLL_SEL_SHIFT               (12U)
#define CRU_CLKSEL15_CON_UART2_PLL_SEL_MASK                (0x3U << CRU_CLKSEL15_CON_UART2_PLL_SEL_SHIFT)               /* 0x00003000 */
/* CLKSEL16_CON */
#define CRU_CLKSEL16_CON_UART0_FRAC_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL16_CON_UART0_FRAC_DIV_CON_MASK           (0xFFFFFFFFU << CRU_CLKSEL16_CON_UART0_FRAC_DIV_CON_SHIFT)   /* 0xFFFFFFFF */
/* CLKSEL17_CON */
#define CRU_CLKSEL17_CON_UART1_FRAC_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL17_CON_UART1_FRAC_DIV_CON_MASK           (0xFFFFFFFFU << CRU_CLKSEL17_CON_UART1_FRAC_DIV_CON_SHIFT)   /* 0xFFFFFFFF */
/* CLKSEL18_CON */
#define CRU_CLKSEL18_CON_UART2_FRAC_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL18_CON_UART2_FRAC_DIV_CON_MASK           (0xFFFFFFFFU << CRU_CLKSEL18_CON_UART2_FRAC_DIV_CON_SHIFT)   /* 0xFFFFFFFF */
/* CLKSEL19_CON */
#define CRU_CLKSEL19_CON_I2C0_PMU_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL19_CON_I2C0_PMU_DIV_CON_MASK             (0x7FU << CRU_CLKSEL19_CON_I2C0_PMU_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL19_CON_I2C0_PMU_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL19_CON_I2C0_PMU_PLL_SEL_MASK             (0x1U << CRU_CLKSEL19_CON_I2C0_PMU_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL19_CON_I2C1_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL19_CON_I2C1_DIV_CON_MASK                 (0x7FU << CRU_CLKSEL19_CON_I2C1_DIV_CON_SHIFT)               /* 0x00007F00 */
#define CRU_CLKSEL19_CON_I2C1_PLL_SEL_SHIFT                (15U)
#define CRU_CLKSEL19_CON_I2C1_PLL_SEL_MASK                 (0x1U << CRU_CLKSEL19_CON_I2C1_PLL_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL20_CON */
#define CRU_CLKSEL20_CON_I2C2_DIV_CON_SHIFT                (0U)
#define CRU_CLKSEL20_CON_I2C2_DIV_CON_MASK                 (0x7FU << CRU_CLKSEL20_CON_I2C2_DIV_CON_SHIFT)               /* 0x0000007F */
#define CRU_CLKSEL20_CON_I2C2_PLL_SEL_SHIFT                (7U)
#define CRU_CLKSEL20_CON_I2C2_PLL_SEL_MASK                 (0x1U << CRU_CLKSEL20_CON_I2C2_PLL_SEL_SHIFT)                /* 0x00000080 */
#define CRU_CLKSEL20_CON_I2C3_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL20_CON_I2C3_DIV_CON_MASK                 (0x7FU << CRU_CLKSEL20_CON_I2C3_DIV_CON_SHIFT)               /* 0x00007F00 */
#define CRU_CLKSEL20_CON_I2C3_PLL_SEL_SHIFT                (15U)
#define CRU_CLKSEL20_CON_I2C3_PLL_SEL_MASK                 (0x1U << CRU_CLKSEL20_CON_I2C3_PLL_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL21_CON */
#define CRU_CLKSEL21_CON_TSADC_DIV_CON_SHIFT               (0U)
#define CRU_CLKSEL21_CON_TSADC_DIV_CON_MASK                (0x3FFU << CRU_CLKSEL21_CON_TSADC_DIV_CON_SHIFT)             /* 0x000003FF */
/* CLKSEL22_CON */
#define CRU_CLKSEL22_CON_SARADC_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL22_CON_SARADC_DIV_CON_MASK               (0x3FFU << CRU_CLKSEL22_CON_SARADC_DIV_CON_SHIFT)            /* 0x000003FF */
/* CLKSEL23_CON */
#define CRU_CLKSEL23_CON_ACLK_PERI_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL23_CON_ACLK_PERI_DIV_CON_MASK            (0x1FU << CRU_CLKSEL23_CON_ACLK_PERI_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL23_CON_HCLK_PERI_DIV_CON_SHIFT           (5U)
#define CRU_CLKSEL23_CON_HCLK_PERI_DIV_CON_MASK            (0x1FU << CRU_CLKSEL23_CON_HCLK_PERI_DIV_CON_SHIFT)          /* 0x000003E0 */
#define CRU_CLKSEL23_CON_PCLK_PERI_DIV_CON_SHIFT           (10U)
#define CRU_CLKSEL23_CON_PCLK_PERI_DIV_CON_MASK            (0x1FU << CRU_CLKSEL23_CON_PCLK_PERI_DIV_CON_SHIFT)          /* 0x00007C00 */
#define CRU_CLKSEL23_CON_ACLK_PERI_PLL_SEL_SHIFT           (15U)
#define CRU_CLKSEL23_CON_ACLK_PERI_PLL_SEL_MASK            (0x1U << CRU_CLKSEL23_CON_ACLK_PERI_PLL_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL24_CON */
#define CRU_CLKSEL24_CON_MAC_DIV_CON_SHIFT                 (0U)
#define CRU_CLKSEL24_CON_MAC_DIV_CON_MASK                  (0x1FU << CRU_CLKSEL24_CON_MAC_DIV_CON_SHIFT)                /* 0x0000001F */
#define CRU_CLKSEL24_CON_RMII_EXTCLK_SEL_SHIFT             (8U)
#define CRU_CLKSEL24_CON_RMII_EXTCLK_SEL_MASK              (0x1U << CRU_CLKSEL24_CON_RMII_EXTCLK_SEL_SHIFT)             /* 0x00000100 */
#define CRU_CLKSEL24_CON_MAC_PLL_SEL_SHIFT                 (12U)
#define CRU_CLKSEL24_CON_MAC_PLL_SEL_MASK                  (0x1U << CRU_CLKSEL24_CON_MAC_PLL_SEL_SHIFT)                 /* 0x00001000 */
/* CLKSEL25_CON */
#define CRU_CLKSEL25_CON_CLK_SDMMC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL25_CON_CLK_SDMMC_DIV_CON_MASK            (0xFFU << CRU_CLKSEL25_CON_CLK_SDMMC_DIV_CON_SHIFT)          /* 0x000000FF */
#define CRU_CLKSEL25_CON_CLK_SDMMC_PLL_SEL_SHIFT           (8U)
#define CRU_CLKSEL25_CON_CLK_SDMMC_PLL_SEL_MASK            (0x3U << CRU_CLKSEL25_CON_CLK_SDMMC_PLL_SEL_SHIFT)           /* 0x00000300 */
#define CRU_CLKSEL25_CON_CLK_SDIO_PLL_SEL_SHIFT            (10U)
#define CRU_CLKSEL25_CON_CLK_SDIO_PLL_SEL_MASK             (0x3U << CRU_CLKSEL25_CON_CLK_SDIO_PLL_SEL_SHIFT)            /* 0x00000C00 */
#define CRU_CLKSEL25_CON_CLK_EMMC_PLL_SEL_SHIFT            (12U)
#define CRU_CLKSEL25_CON_CLK_EMMC_PLL_SEL_MASK             (0x3U << CRU_CLKSEL25_CON_CLK_EMMC_PLL_SEL_SHIFT)            /* 0x00003000 */
/* CLKSEL26_CON */
#define CRU_CLKSEL26_CON_CLK_SDIO_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL26_CON_CLK_SDIO_DIV_CON_MASK             (0xFFU << CRU_CLKSEL26_CON_CLK_SDIO_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL26_CON_CLK_EMMC_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL26_CON_CLK_EMMC_DIV_CON_MASK             (0xFFU << CRU_CLKSEL26_CON_CLK_EMMC_DIV_CON_SHIFT)           /* 0x0000FF00 */
/* CLKSEL27_CON */
#define CRU_CLKSEL27_CON_CLK_SFC_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL27_CON_CLK_SFC_DIV_CON_MASK              (0x7FU << CRU_CLKSEL27_CON_CLK_SFC_DIV_CON_SHIFT)            /* 0x0000007F */
#define CRU_CLKSEL27_CON_CLK_SFC_PLL_SEL_SHIFT             (7U)
#define CRU_CLKSEL27_CON_CLK_SFC_PLL_SEL_MASK              (0x1U << CRU_CLKSEL27_CON_CLK_SFC_PLL_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL27_CON_CLK_NANDC_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL27_CON_CLK_NANDC_DIV_CON_MASK            (0x1FU << CRU_CLKSEL27_CON_CLK_NANDC_DIV_CON_SHIFT)          /* 0x00001F00 */
#define CRU_CLKSEL27_CON_CLK_NANDC_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL27_CON_CLK_NANDC_PLL_SEL_MASK            (0x1U << CRU_CLKSEL27_CON_CLK_NANDC_PLL_SEL_SHIFT)           /* 0x00004000 */
/* CLKSEL28_CON */
#define CRU_CLKSEL28_CON_ACLK_VIO0_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL28_CON_ACLK_VIO0_DIV_CON_MASK            (0x1FU << CRU_CLKSEL28_CON_ACLK_VIO0_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL28_CON_ACLK_VIO0_PLL_SEL_SHIFT           (6U)
#define CRU_CLKSEL28_CON_ACLK_VIO0_PLL_SEL_MASK            (0x3U << CRU_CLKSEL28_CON_ACLK_VIO0_PLL_SEL_SHIFT)           /* 0x000000C0 */
#define CRU_CLKSEL28_CON_ACLK_VIO1_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL28_CON_ACLK_VIO1_DIV_CON_MASK            (0x1FU << CRU_CLKSEL28_CON_ACLK_VIO1_DIV_CON_SHIFT)          /* 0x00001F00 */
#define CRU_CLKSEL28_CON_ACLK_VIO1_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL28_CON_ACLK_VIO1_PLL_SEL_MASK            (0x3U << CRU_CLKSEL28_CON_ACLK_VIO1_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL29_CON */
#define CRU_CLKSEL29_CON_HCLK_VIO_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL29_CON_HCLK_VIO_DIV_CON_MASK             (0x1FU << CRU_CLKSEL29_CON_HCLK_VIO_DIV_CON_SHIFT)           /* 0x0000001F */
#define CRU_CLKSEL29_CON_PCLK_VIO_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL29_CON_PCLK_VIO_DIV_CON_MASK             (0x1FU << CRU_CLKSEL29_CON_PCLK_VIO_DIV_CON_SHIFT)           /* 0x00001F00 */
/* CLKSEL30_CON */
#define CRU_CLKSEL30_CON_CLK_ISP_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL30_CON_CLK_ISP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL30_CON_CLK_ISP_DIV_CON_SHIFT)            /* 0x00001F00 */
#define CRU_CLKSEL30_CON_CLK_ISP_PLL_SEL_SHIFT             (14U)
#define CRU_CLKSEL30_CON_CLK_ISP_PLL_SEL_MASK              (0x3U << CRU_CLKSEL30_CON_CLK_ISP_PLL_SEL_SHIFT)             /* 0x0000C000 */
/* CLKSEL31_CON */
#define CRU_CLKSEL31_CON_VIP0_CLK_SEL_SHIFT                (0U)
#define CRU_CLKSEL31_CON_VIP0_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL31_CON_VIP0_CLK_SEL_SHIFT)                /* 0x00000003 */
#define CRU_CLKSEL31_CON_VIP1_CLK_SEL_SHIFT                (2U)
#define CRU_CLKSEL31_CON_VIP1_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL31_CON_VIP1_CLK_SEL_SHIFT)                /* 0x0000000C */
#define CRU_CLKSEL31_CON_VIP2_CLK_SEL_SHIFT                (4U)
#define CRU_CLKSEL31_CON_VIP2_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL31_CON_VIP2_CLK_SEL_SHIFT)                /* 0x00000030 */
#define CRU_CLKSEL31_CON_VIP3_CLK_SEL_SHIFT                (6U)
#define CRU_CLKSEL31_CON_VIP3_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL31_CON_VIP3_CLK_SEL_SHIFT)                /* 0x000000C0 */
#define CRU_CLKSEL31_CON_PCLKIN_INV_SEL_SHIFT              (8U)
#define CRU_CLKSEL31_CON_PCLKIN_INV_SEL_MASK               (0x1U << CRU_CLKSEL31_CON_PCLKIN_INV_SEL_SHIFT)              /* 0x00000100 */
/* CLKSEL32_CON */
#define CRU_CLKSEL32_CON_DCLK_VOP_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL32_CON_DCLK_VOP_DIV_CON_MASK             (0x3FU << CRU_CLKSEL32_CON_DCLK_VOP_DIV_CON_SHIFT)           /* 0x0000003F */
#define CRU_CLKSEL32_CON_DCLK_VOP_PLL_SEL_SHIFT            (6U)
#define CRU_CLKSEL32_CON_DCLK_VOP_PLL_SEL_MASK             (0x1U << CRU_CLKSEL32_CON_DCLK_VOP_PLL_SEL_SHIFT)            /* 0x00000040 */
#define CRU_CLKSEL32_CON_DCLK_VOP_CLK_SEL_SHIFT            (7U)
#define CRU_CLKSEL32_CON_DCLK_VOP_CLK_SEL_MASK             (0x1U << CRU_CLKSEL32_CON_DCLK_VOP_CLK_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL32_CON_HDMIPHY_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL32_CON_HDMIPHY_DIV_CON_MASK              (0x3FU << CRU_CLKSEL32_CON_HDMIPHY_DIV_CON_SHIFT)            /* 0x00003F00 */
#define CRU_CLKSEL32_CON_HDMIPHY_24M_SEL_SHIFT             (15U)
#define CRU_CLKSEL32_CON_HDMIPHY_24M_SEL_MASK              (0x1U << CRU_CLKSEL32_CON_HDMIPHY_24M_SEL_SHIFT)             /* 0x00008000 */
/* CLKSEL33_CON */
#define CRU_CLKSEL33_CON_CLK_RGA_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL33_CON_CLK_RGA_DIV_CON_MASK              (0x1FU << CRU_CLKSEL33_CON_CLK_RGA_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL33_CON_CLK_RGA_PLL_SEL_SHIFT             (6U)
#define CRU_CLKSEL33_CON_CLK_RGA_PLL_SEL_MASK              (0x3U << CRU_CLKSEL33_CON_CLK_RGA_PLL_SEL_SHIFT)             /* 0x000000C0 */
#define CRU_CLKSEL33_CON_CLK_CVBS_HOST_DIV_CON_SHIFT       (8U)
#define CRU_CLKSEL33_CON_CLK_CVBS_HOST_DIV_CON_MASK        (0x1FU << CRU_CLKSEL33_CON_CLK_CVBS_HOST_DIV_CON_SHIFT)      /* 0x00001F00 */
#define CRU_CLKSEL33_CON_CLK_CVBS_HOST_PLL_SEL_SHIFT       (13U)
#define CRU_CLKSEL33_CON_CLK_CVBS_HOST_PLL_SEL_MASK        (0x3U << CRU_CLKSEL33_CON_CLK_CVBS_HOST_PLL_SEL_SHIFT)       /* 0x00006000 */
/* CLKSEL34_CON */
#define CRU_CLKSEL34_CON_HDMI_CEC_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL34_CON_HDMI_CEC_DIV_CON_MASK             (0x3FFFU << CRU_CLKSEL34_CON_HDMI_CEC_DIV_CON_SHIFT)         /* 0x00003FFF */
#define CRU_CLKSEL34_CON_HDMI_CEC_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL34_CON_HDMI_CEC_PLL_SEL_MASK             (0x3U << CRU_CLKSEL34_CON_HDMI_CEC_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL35_CON */
#define CRU_CLKSEL35_CON_ACLK_RKVDEC_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL35_CON_ACLK_RKVDEC_DIV_CON_MASK          (0x1FU << CRU_CLKSEL35_CON_ACLK_RKVDEC_DIV_CON_SHIFT)        /* 0x0000001F */
#define CRU_CLKSEL35_CON_ACLK_RKVDEC_PLL_SEL_SHIFT         (6U)
#define CRU_CLKSEL35_CON_ACLK_RKVDEC_PLL_SEL_MASK          (0x3U << CRU_CLKSEL35_CON_ACLK_RKVDEC_PLL_SEL_SHIFT)         /* 0x000000C0 */
#define CRU_CLKSEL35_CON_HEVC_CABAC_DIV_CON_SHIFT          (8U)
#define CRU_CLKSEL35_CON_HEVC_CABAC_DIV_CON_MASK           (0x1FU << CRU_CLKSEL35_CON_HEVC_CABAC_DIV_CON_SHIFT)         /* 0x00001F00 */
#define CRU_CLKSEL35_CON_HEVC_CABAC_PLL_SEL_SHIFT          (14U)
#define CRU_CLKSEL35_CON_HEVC_CABAC_PLL_SEL_MASK           (0x3U << CRU_CLKSEL35_CON_HEVC_CABAC_PLL_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL36_CON */
#define CRU_CLKSEL36_CON_HEVC_CORE_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL36_CON_HEVC_CORE_DIV_CON_MASK            (0x1FU << CRU_CLKSEL36_CON_HEVC_CORE_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL36_CON_HEVC_CORE_PLL_SEL_SHIFT           (6U)
#define CRU_CLKSEL36_CON_HEVC_CORE_PLL_SEL_MASK            (0x3U << CRU_CLKSEL36_CON_HEVC_CORE_PLL_SEL_SHIFT)           /* 0x000000C0 */
#define CRU_CLKSEL36_CON_ACLK_VPU_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL36_CON_ACLK_VPU_DIV_CON_MASK             (0x1FU << CRU_CLKSEL36_CON_ACLK_VPU_DIV_CON_SHIFT)           /* 0x00001F00 */
#define CRU_CLKSEL36_CON_ACLK_VPU_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL36_CON_ACLK_VPU_PLL_SEL_MASK             (0x3U << CRU_CLKSEL36_CON_ACLK_VPU_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL37_CON */
#define CRU_CLKSEL37_CON_ACLK_RKVENC_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL37_CON_ACLK_RKVENC_DIV_CON_MASK          (0x1FU << CRU_CLKSEL37_CON_ACLK_RKVENC_DIV_CON_SHIFT)        /* 0x0000001F */
#define CRU_CLKSEL37_CON_ACLK_RKVENC_PLL_SEL_SHIFT         (6U)
#define CRU_CLKSEL37_CON_ACLK_RKVENC_PLL_SEL_MASK          (0x3U << CRU_CLKSEL37_CON_ACLK_RKVENC_PLL_SEL_SHIFT)         /* 0x000000C0 */
#define CRU_CLKSEL37_CON_VENC_CORE_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL37_CON_VENC_CORE_DIV_CON_MASK            (0x1FU << CRU_CLKSEL37_CON_VENC_CORE_DIV_CON_SHIFT)          /* 0x00001F00 */
#define CRU_CLKSEL37_CON_VENC_CORE_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL37_CON_VENC_CORE_PLL_SEL_MASK            (0x3U << CRU_CLKSEL37_CON_VENC_CORE_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL38_CON */
#define CRU_CLKSEL38_CON_PCLK_PMU_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL38_CON_PCLK_PMU_DIV_CON_MASK             (0x1FU << CRU_CLKSEL38_CON_PCLK_PMU_DIV_CON_SHIFT)           /* 0x0000001F */
/* CLKSEL39_CON */
#define CRU_CLKSEL39_CON_TEST_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL39_CON_TEST_DIV_CON_MASK                 (0x1FU << CRU_CLKSEL39_CON_TEST_DIV_CON_SHIFT)               /* 0x00001F00 */
/* CLKSEL40_CON */
#define CRU_CLKSEL40_CON_CLK_CIF_OUT_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL40_CON_CLK_CIF_OUT_DIV_CON_MASK          (0x1FU << CRU_CLKSEL40_CON_CLK_CIF_OUT_DIV_CON_SHIFT)        /* 0x0000001F */
#define CRU_CLKSEL40_CON_CLK_CIF_OUT_PLL_SEL_SHIFT         (8U)
#define CRU_CLKSEL40_CON_CLK_CIF_OUT_PLL_SEL_MASK          (0x1U << CRU_CLKSEL40_CON_CLK_CIF_OUT_PLL_SEL_SHIFT)         /* 0x00000100 */
#define CRU_CLKSEL40_CON_CLK_CIF_OUT_SEL_SHIFT             (12U)
#define CRU_CLKSEL40_CON_CLK_CIF_OUT_SEL_MASK              (0x1U << CRU_CLKSEL40_CON_CLK_CIF_OUT_SEL_SHIFT)             /* 0x00001000 */
/* CLKSEL41_CON */
#define CRU_CLKSEL41_CON_CSI_CLK_OUT_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL41_CON_CSI_CLK_OUT_DIV_CON_MASK          (0x1FU << CRU_CLKSEL41_CON_CSI_CLK_OUT_DIV_CON_SHIFT)        /* 0x0000001F */
#define CRU_CLKSEL41_CON_WIFI_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL41_CON_WIFI_DIV_CON_MASK                 (0x3FU << CRU_CLKSEL41_CON_WIFI_DIV_CON_SHIFT)               /* 0x00003F00 */
#define CRU_CLKSEL41_CON_CLK_WIFI_SEL_SHIFT                (15U)
#define CRU_CLKSEL41_CON_CLK_WIFI_SEL_MASK                 (0x1U << CRU_CLKSEL41_CON_CLK_WIFI_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL42_CON */
#define CRU_CLKSEL42_CON_CLK_DSP_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL42_CON_CLK_DSP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL42_CON_CLK_DSP_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL42_CON_CLK_DSP_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL42_CON_CLK_DSP_PLL_SEL_MASK              (0x3U << CRU_CLKSEL42_CON_CLK_DSP_PLL_SEL_SHIFT)             /* 0x00000300 */
/* CLKSEL43_CON */
#define CRU_CLKSEL43_CON_CLK_DSP_PFM_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL43_CON_CLK_DSP_PFM_DIV_CON_MASK          (0x1FU << CRU_CLKSEL43_CON_CLK_DSP_PFM_DIV_CON_SHIFT)        /* 0x0000001F */
#define CRU_CLKSEL43_CON_PCLK_DSP_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL43_CON_PCLK_DSP_DIV_CON_MASK             (0x1FU << CRU_CLKSEL43_CON_PCLK_DSP_DIV_CON_SHIFT)           /* 0x00001F00 */
/* CLKSEL44_CON */
#define CRU_CLKSEL44_CON_DSP_IOP_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL44_CON_DSP_IOP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL44_CON_DSP_IOP_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL44_CON_DSP_EPP_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL44_CON_DSP_EPP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL44_CON_DSP_EPP_DIV_CON_SHIFT)            /* 0x00001F00 */
/* CLKSEL45_CON */
#define CRU_CLKSEL45_CON_DSP_EDP_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL45_CON_DSP_EDP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL45_CON_DSP_EDP_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL45_CON_DSP_EDAP_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL45_CON_DSP_EDAP_DIV_CON_MASK             (0x1FU << CRU_CLKSEL45_CON_DSP_EDAP_DIV_CON_SHIFT)           /* 0x00001F00 */
/* CLKGATE0_CON */
#define CRU_CLKGATE0_CON_CLK_CORE_APLL_EN_SHIFT            (0U)
#define CRU_CLKGATE0_CON_CLK_CORE_APLL_EN_MASK             (0x1U << CRU_CLKGATE0_CON_CLK_CORE_APLL_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE0_CON_CLK_CORE_DPLL_EN_SHIFT            (1U)
#define CRU_CLKGATE0_CON_CLK_CORE_DPLL_EN_MASK             (0x1U << CRU_CLKGATE0_CON_CLK_CORE_DPLL_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE0_CON_CLK_CORE_GPLL_EN_SHIFT            (2U)
#define CRU_CLKGATE0_CON_CLK_CORE_GPLL_EN_MASK             (0x1U << CRU_CLKGATE0_CON_CLK_CORE_GPLL_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE0_CON_CLK_JTAG_EN_SHIFT                 (3U)
#define CRU_CLKGATE0_CON_CLK_JTAG_EN_MASK                  (0x1U << CRU_CLKGATE0_CON_CLK_JTAG_EN_SHIFT)                 /* 0x00000008 */
#define CRU_CLKGATE0_CON_ACLK_CORE_EN_SHIFT                (4U)
#define CRU_CLKGATE0_CON_ACLK_CORE_EN_MASK                 (0x1U << CRU_CLKGATE0_CON_ACLK_CORE_EN_SHIFT)                /* 0x00000010 */
#define CRU_CLKGATE0_CON_PCLK_DBG_EN_SHIFT                 (5U)
#define CRU_CLKGATE0_CON_PCLK_DBG_EN_MASK                  (0x1U << CRU_CLKGATE0_CON_PCLK_DBG_EN_SHIFT)                 /* 0x00000020 */
#define CRU_CLKGATE0_CON_CLK_DDR_APLL_EN_SHIFT             (8U)
#define CRU_CLKGATE0_CON_CLK_DDR_APLL_EN_MASK              (0x1U << CRU_CLKGATE0_CON_CLK_DDR_APLL_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE0_CON_CLK_DDR_DPLL_EN_SHIFT             (9U)
#define CRU_CLKGATE0_CON_CLK_DDR_DPLL_EN_MASK              (0x1U << CRU_CLKGATE0_CON_CLK_DDR_DPLL_EN_SHIFT)             /* 0x00000200 */
#define CRU_CLKGATE0_CON_CLK_DDR_GPLL_EN_SHIFT             (10U)
#define CRU_CLKGATE0_CON_CLK_DDR_GPLL_EN_MASK              (0x1U << CRU_CLKGATE0_CON_CLK_DDR_GPLL_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE0_CON_CLK_DDRMON_EN_SHIFT               (11U)
#define CRU_CLKGATE0_CON_CLK_DDRMON_EN_MASK                (0x1U << CRU_CLKGATE0_CON_CLK_DDRMON_EN_SHIFT)               /* 0x00000800 */
/* CLKGATE1_CON */
#define CRU_CLKGATE1_CON_ACLK_BUS_GPLL_EN_SHIFT            (0U)
#define CRU_CLKGATE1_CON_ACLK_BUS_GPLL_EN_MASK             (0x1U << CRU_CLKGATE1_CON_ACLK_BUS_GPLL_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE1_CON_ACLK_BUS_APLL_EN_SHIFT            (1U)
#define CRU_CLKGATE1_CON_ACLK_BUS_APLL_EN_MASK             (0x1U << CRU_CLKGATE1_CON_ACLK_BUS_APLL_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE1_CON_ACLK_BUS_DPLL_EN_SHIFT            (2U)
#define CRU_CLKGATE1_CON_ACLK_BUS_DPLL_EN_MASK             (0x1U << CRU_CLKGATE1_CON_ACLK_BUS_DPLL_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE1_CON_HCLK_BUS_EN_SHIFT                 (4U)
#define CRU_CLKGATE1_CON_HCLK_BUS_EN_MASK                  (0x1U << CRU_CLKGATE1_CON_HCLK_BUS_EN_SHIFT)                 /* 0x00000010 */
#define CRU_CLKGATE1_CON_PCLK_BUS_SRC_EN_SHIFT             (5U)
#define CRU_CLKGATE1_CON_PCLK_BUS_SRC_EN_MASK              (0x1U << CRU_CLKGATE1_CON_PCLK_BUS_SRC_EN_SHIFT)             /* 0x00000020 */
#define CRU_CLKGATE1_CON_PCLK_BUS_EN_SHIFT                 (6U)
#define CRU_CLKGATE1_CON_PCLK_BUS_EN_MASK                  (0x1U << CRU_CLKGATE1_CON_PCLK_BUS_EN_SHIFT)                 /* 0x00000040 */
#define CRU_CLKGATE1_CON_PCLK_TOP_EN_SHIFT                 (7U)
#define CRU_CLKGATE1_CON_PCLK_TOP_EN_MASK                  (0x1U << CRU_CLKGATE1_CON_PCLK_TOP_EN_SHIFT)                 /* 0x00000080 */
#define CRU_CLKGATE1_CON_PCLK_DDR_EN_SHIFT                 (8U)
#define CRU_CLKGATE1_CON_PCLK_DDR_EN_MASK                  (0x1U << CRU_CLKGATE1_CON_PCLK_DDR_EN_SHIFT)                 /* 0x00000100 */
#define CRU_CLKGATE1_CON_CLK_TIMER0_EN_SHIFT               (9U)
#define CRU_CLKGATE1_CON_CLK_TIMER0_EN_MASK                (0x1U << CRU_CLKGATE1_CON_CLK_TIMER0_EN_SHIFT)               /* 0x00000200 */
#define CRU_CLKGATE1_CON_CLK_TIMER1_EN_SHIFT               (10U)
#define CRU_CLKGATE1_CON_CLK_TIMER1_EN_MASK                (0x1U << CRU_CLKGATE1_CON_CLK_TIMER1_EN_SHIFT)               /* 0x00000400 */
/* CLKGATE2_CON */
#define CRU_CLKGATE2_CON_CLK_I2S0_SRC_EN_SHIFT             (0U)
#define CRU_CLKGATE2_CON_CLK_I2S0_SRC_EN_MASK              (0x1U << CRU_CLKGATE2_CON_CLK_I2S0_SRC_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE2_CON_CLK_I2S0_FRAC_SRC_EN_SHIFT        (1U)
#define CRU_CLKGATE2_CON_CLK_I2S0_FRAC_SRC_EN_MASK         (0x1U << CRU_CLKGATE2_CON_CLK_I2S0_FRAC_SRC_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE2_CON_CLK_I2S0_EN_SHIFT                 (2U)
#define CRU_CLKGATE2_CON_CLK_I2S0_EN_MASK                  (0x1U << CRU_CLKGATE2_CON_CLK_I2S0_EN_SHIFT)                 /* 0x00000004 */
#define CRU_CLKGATE2_CON_CLK_I2S0_OUT_EN_SHIFT             (3U)
#define CRU_CLKGATE2_CON_CLK_I2S0_OUT_EN_MASK              (0x1U << CRU_CLKGATE2_CON_CLK_I2S0_OUT_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE2_CON_CLK_I2S1_SRC_EN_SHIFT             (4U)
#define CRU_CLKGATE2_CON_CLK_I2S1_SRC_EN_MASK              (0x1U << CRU_CLKGATE2_CON_CLK_I2S1_SRC_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE2_CON_CLK_I2S1_FRAC_SRC_EN_SHIFT        (5U)
#define CRU_CLKGATE2_CON_CLK_I2S1_FRAC_SRC_EN_MASK         (0x1U << CRU_CLKGATE2_CON_CLK_I2S1_FRAC_SRC_EN_SHIFT)        /* 0x00000020 */
#define CRU_CLKGATE2_CON_CLK_I2S1_EN_SHIFT                 (6U)
#define CRU_CLKGATE2_CON_CLK_I2S1_EN_MASK                  (0x1U << CRU_CLKGATE2_CON_CLK_I2S1_EN_SHIFT)                 /* 0x00000040 */
#define CRU_CLKGATE2_CON_CLK_I2S2_SRC_EN_SHIFT             (8U)
#define CRU_CLKGATE2_CON_CLK_I2S2_SRC_EN_MASK              (0x1U << CRU_CLKGATE2_CON_CLK_I2S2_SRC_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE2_CON_CLK_I2S2_FRAC_SRC_EN_SHIFT        (9U)
#define CRU_CLKGATE2_CON_CLK_I2S2_FRAC_SRC_EN_MASK         (0x1U << CRU_CLKGATE2_CON_CLK_I2S2_FRAC_SRC_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE2_CON_CLK_I2S2_EN_SHIFT                 (10U)
#define CRU_CLKGATE2_CON_CLK_I2S2_EN_MASK                  (0x1U << CRU_CLKGATE2_CON_CLK_I2S2_EN_SHIFT)                 /* 0x00000400 */
#define CRU_CLKGATE2_CON_CLK_CRYPTO_SRC_EN_SHIFT           (12U)
#define CRU_CLKGATE2_CON_CLK_CRYPTO_SRC_EN_MASK            (0x1U << CRU_CLKGATE2_CON_CLK_CRYPTO_SRC_EN_SHIFT)           /* 0x00001000 */
/* CLKGATE3_CON */
#define CRU_CLKGATE3_CON_CLK_SPI_SRC_EN_SHIFT              (0U)
#define CRU_CLKGATE3_CON_CLK_SPI_SRC_EN_MASK               (0x1U << CRU_CLKGATE3_CON_CLK_SPI_SRC_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE3_CON_CLK_UART0_SRC_EN_SHIFT            (1U)
#define CRU_CLKGATE3_CON_CLK_UART0_SRC_EN_MASK             (0x1U << CRU_CLKGATE3_CON_CLK_UART0_SRC_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE3_CON_CLK_UART0_FRAC_SRC_EN_SHIFT       (2U)
#define CRU_CLKGATE3_CON_CLK_UART0_FRAC_SRC_EN_MASK        (0x1U << CRU_CLKGATE3_CON_CLK_UART0_FRAC_SRC_EN_SHIFT)       /* 0x00000004 */
#define CRU_CLKGATE3_CON_CLK_UART1_SRC_EN_SHIFT            (3U)
#define CRU_CLKGATE3_CON_CLK_UART1_SRC_EN_MASK             (0x1U << CRU_CLKGATE3_CON_CLK_UART1_SRC_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE3_CON_CLK_UART1_FRAC_SRC_EN_SHIFT       (4U)
#define CRU_CLKGATE3_CON_CLK_UART1_FRAC_SRC_EN_MASK        (0x1U << CRU_CLKGATE3_CON_CLK_UART1_FRAC_SRC_EN_SHIFT)       /* 0x00000010 */
#define CRU_CLKGATE3_CON_CLK_UART2_SRC_EN_SHIFT            (5U)
#define CRU_CLKGATE3_CON_CLK_UART2_SRC_EN_MASK             (0x1U << CRU_CLKGATE3_CON_CLK_UART2_SRC_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE3_CON_CLK_UART2_FRAC_SRC_EN_SHIFT       (6U)
#define CRU_CLKGATE3_CON_CLK_UART2_FRAC_SRC_EN_MASK        (0x1U << CRU_CLKGATE3_CON_CLK_UART2_FRAC_SRC_EN_SHIFT)       /* 0x00000040 */
#define CRU_CLKGATE3_CON_CLK_I2C1_SRC_EN_SHIFT             (7U)
#define CRU_CLKGATE3_CON_CLK_I2C1_SRC_EN_MASK              (0x1U << CRU_CLKGATE3_CON_CLK_I2C1_SRC_EN_SHIFT)             /* 0x00000080 */
#define CRU_CLKGATE3_CON_CLK_I2C2_SRC_EN_SHIFT             (8U)
#define CRU_CLKGATE3_CON_CLK_I2C2_SRC_EN_MASK              (0x1U << CRU_CLKGATE3_CON_CLK_I2C2_SRC_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE3_CON_CLK_I2C3_SRC_EN_SHIFT             (9U)
#define CRU_CLKGATE3_CON_CLK_I2C3_SRC_EN_MASK              (0x1U << CRU_CLKGATE3_CON_CLK_I2C3_SRC_EN_SHIFT)             /* 0x00000200 */
#define CRU_CLKGATE3_CON_CLK_PWM1_SRC_EN_SHIFT             (10U)
#define CRU_CLKGATE3_CON_CLK_PWM1_SRC_EN_MASK              (0x1U << CRU_CLKGATE3_CON_CLK_PWM1_SRC_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE3_CON_CLK_TSADC_SRC_EN_SHIFT            (11U)
#define CRU_CLKGATE3_CON_CLK_TSADC_SRC_EN_MASK             (0x1U << CRU_CLKGATE3_CON_CLK_TSADC_SRC_EN_SHIFT)            /* 0x00000800 */
#define CRU_CLKGATE3_CON_CLK_SARADC_SRC_EN_SHIFT           (12U)
#define CRU_CLKGATE3_CON_CLK_SARADC_SRC_EN_MASK            (0x1U << CRU_CLKGATE3_CON_CLK_SARADC_SRC_EN_SHIFT)           /* 0x00001000 */
/* CLKGATE4_CON */
#define CRU_CLKGATE4_CON_ACLK_PERI_DPLL_EN_SHIFT           (1U)
#define CRU_CLKGATE4_CON_ACLK_PERI_DPLL_EN_MASK            (0x1U << CRU_CLKGATE4_CON_ACLK_PERI_DPLL_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE4_CON_ACLK_PERI_GPLL_EN_SHIFT           (2U)
#define CRU_CLKGATE4_CON_ACLK_PERI_GPLL_EN_MASK            (0x1U << CRU_CLKGATE4_CON_ACLK_PERI_GPLL_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE4_CON_HCLK_PERIPH_EN_SHIFT              (4U)
#define CRU_CLKGATE4_CON_HCLK_PERIPH_EN_MASK               (0x1U << CRU_CLKGATE4_CON_HCLK_PERIPH_EN_SHIFT)              /* 0x00000010 */
#define CRU_CLKGATE4_CON_PCLK_PERIPH_EN_SHIFT              (5U)
#define CRU_CLKGATE4_CON_PCLK_PERIPH_EN_MASK               (0x1U << CRU_CLKGATE4_CON_PCLK_PERIPH_EN_SHIFT)              /* 0x00000020 */
#define CRU_CLKGATE4_CON_CLK_MAC_REF_EN_SHIFT              (6U)
#define CRU_CLKGATE4_CON_CLK_MAC_REF_EN_MASK               (0x1U << CRU_CLKGATE4_CON_CLK_MAC_REF_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE4_CON_CLK_MAC_REFOUT_EN_SHIFT           (7U)
#define CRU_CLKGATE4_CON_CLK_MAC_REFOUT_EN_MASK            (0x1U << CRU_CLKGATE4_CON_CLK_MAC_REFOUT_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE4_CON_CLK_MAC_RX_EN_SHIFT               (8U)
#define CRU_CLKGATE4_CON_CLK_MAC_RX_EN_MASK                (0x1U << CRU_CLKGATE4_CON_CLK_MAC_RX_EN_SHIFT)               /* 0x00000100 */
#define CRU_CLKGATE4_CON_CLK_MAC_SRC_EN_SHIFT              (10U)
#define CRU_CLKGATE4_CON_CLK_MAC_SRC_EN_MASK               (0x1U << CRU_CLKGATE4_CON_CLK_MAC_SRC_EN_SHIFT)              /* 0x00000400 */
/* CLKGATE5_CON */
#define CRU_CLKGATE5_CON_CLK_SDMMC_SRC_EN_SHIFT            (0U)
#define CRU_CLKGATE5_CON_CLK_SDMMC_SRC_EN_MASK             (0x1U << CRU_CLKGATE5_CON_CLK_SDMMC_SRC_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE5_CON_CLK_EMMC_SRC_EN_SHIFT             (1U)
#define CRU_CLKGATE5_CON_CLK_EMMC_SRC_EN_MASK              (0x1U << CRU_CLKGATE5_CON_CLK_EMMC_SRC_EN_SHIFT)             /* 0x00000002 */
#define CRU_CLKGATE5_CON_CLK_SDIO_SRC_EN_SHIFT             (2U)
#define CRU_CLKGATE5_CON_CLK_SDIO_SRC_EN_MASK              (0x1U << CRU_CLKGATE5_CON_CLK_SDIO_SRC_EN_SHIFT)             /* 0x00000004 */
#define CRU_CLKGATE5_CON_CLK_NANDC_SRC_EN_SHIFT            (3U)
#define CRU_CLKGATE5_CON_CLK_NANDC_SRC_EN_MASK             (0x1U << CRU_CLKGATE5_CON_CLK_NANDC_SRC_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE5_CON_CLK_SFC_SRC_EN_SHIFT              (4U)
#define CRU_CLKGATE5_CON_CLK_SFC_SRC_EN_MASK               (0x1U << CRU_CLKGATE5_CON_CLK_SFC_SRC_EN_SHIFT)              /* 0x00000010 */
#define CRU_CLKGATE5_CON_CLK_USBPHY_EN_SHIFT               (5U)
#define CRU_CLKGATE5_CON_CLK_USBPHY_EN_MASK                (0x1U << CRU_CLKGATE5_CON_CLK_USBPHY_EN_SHIFT)               /* 0x00000020 */
/* CLKGATE6_CON */
#define CRU_CLKGATE6_CON_ACLK_VIO0_SRC_EN_SHIFT            (0U)
#define CRU_CLKGATE6_CON_ACLK_VIO0_SRC_EN_MASK             (0x1U << CRU_CLKGATE6_CON_ACLK_VIO0_SRC_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE6_CON_ACLK_VIO1_SRC_EN_SHIFT            (1U)
#define CRU_CLKGATE6_CON_ACLK_VIO1_SRC_EN_MASK             (0x1U << CRU_CLKGATE6_CON_ACLK_VIO1_SRC_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE6_CON_CLK_ISP_SRC_EN_SHIFT              (3U)
#define CRU_CLKGATE6_CON_CLK_ISP_SRC_EN_MASK               (0x1U << CRU_CLKGATE6_CON_CLK_ISP_SRC_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE6_CON_DCLK_VOP_DPLL_EN_SHIFT            (4U)
#define CRU_CLKGATE6_CON_DCLK_VOP_DPLL_EN_MASK             (0x1U << CRU_CLKGATE6_CON_DCLK_VOP_DPLL_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE6_CON_DCLK_VOP_GPLL_EN_SHIFT            (5U)
#define CRU_CLKGATE6_CON_DCLK_VOP_GPLL_EN_MASK             (0x1U << CRU_CLKGATE6_CON_DCLK_VOP_GPLL_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE6_CON_CLK_RGA_SRC_EN_SHIFT              (6U)
#define CRU_CLKGATE6_CON_CLK_RGA_SRC_EN_MASK               (0x1U << CRU_CLKGATE6_CON_CLK_RGA_SRC_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE6_CON_CVBS_HOST_SRC_EN_SHIFT            (7U)
#define CRU_CLKGATE6_CON_CVBS_HOST_SRC_EN_MASK             (0x1U << CRU_CLKGATE6_CON_CVBS_HOST_SRC_EN_SHIFT)            /* 0x00000080 */
#define CRU_CLKGATE6_CON_HDMI_SFR_EN_SHIFT                 (8U)
#define CRU_CLKGATE6_CON_HDMI_SFR_EN_MASK                  (0x1U << CRU_CLKGATE6_CON_HDMI_SFR_EN_SHIFT)                 /* 0x00000100 */
#define CRU_CLKGATE6_CON_HDMI_CEC_EN_SHIFT                 (9U)
#define CRU_CLKGATE6_CON_HDMI_CEC_EN_MASK                  (0x1U << CRU_CLKGATE6_CON_HDMI_CEC_EN_SHIFT)                 /* 0x00000200 */
/* CLKGATE7_CON */
#define CRU_CLKGATE7_CON_HCLK_VIO_EN_SHIFT                 (2U)
#define CRU_CLKGATE7_CON_HCLK_VIO_EN_MASK                  (0x1U << CRU_CLKGATE7_CON_HCLK_VIO_EN_SHIFT)                 /* 0x00000004 */
#define CRU_CLKGATE7_CON_PCLK_VIO_EN_SHIFT                 (3U)
#define CRU_CLKGATE7_CON_PCLK_VIO_EN_MASK                  (0x1U << CRU_CLKGATE7_CON_PCLK_VIO_EN_SHIFT)                 /* 0x00000008 */
#define CRU_CLKGATE7_CON_PCLK_ISP_EN_SHIFT                 (6U)
#define CRU_CLKGATE7_CON_PCLK_ISP_EN_MASK                  (0x1U << CRU_CLKGATE7_CON_PCLK_ISP_EN_SHIFT)                 /* 0x00000040 */
#define CRU_CLKGATE7_CON_PCLKIN_CIF1TO4_EN_SHIFT           (8U)
#define CRU_CLKGATE7_CON_PCLKIN_CIF1TO4_EN_MASK            (0x1U << CRU_CLKGATE7_CON_PCLKIN_CIF1TO4_EN_SHIFT)           /* 0x00000100 */
#define CRU_CLKGATE7_CON_PCLKIN_VIP0_EN_SHIFT              (9U)
#define CRU_CLKGATE7_CON_PCLKIN_VIP0_EN_MASK               (0x1U << CRU_CLKGATE7_CON_PCLKIN_VIP0_EN_SHIFT)              /* 0x00000200 */
#define CRU_CLKGATE7_CON_PCLKIN_VIP1_EN_SHIFT              (10U)
#define CRU_CLKGATE7_CON_PCLKIN_VIP1_EN_MASK               (0x1U << CRU_CLKGATE7_CON_PCLKIN_VIP1_EN_SHIFT)              /* 0x00000400 */
#define CRU_CLKGATE7_CON_PCLKIN_VIP2_EN_SHIFT              (11U)
#define CRU_CLKGATE7_CON_PCLKIN_VIP2_EN_MASK               (0x1U << CRU_CLKGATE7_CON_PCLKIN_VIP2_EN_SHIFT)              /* 0x00000800 */
#define CRU_CLKGATE7_CON_PCLKIN_VIP3_EN_SHIFT              (12U)
#define CRU_CLKGATE7_CON_PCLKIN_VIP3_EN_MASK               (0x1U << CRU_CLKGATE7_CON_PCLKIN_VIP3_EN_SHIFT)              /* 0x00001000 */
/* CLKGATE8_CON */
#define CRU_CLKGATE8_CON_ACLK_RKVDEC_SRC_EN_SHIFT          (0U)
#define CRU_CLKGATE8_CON_ACLK_RKVDEC_SRC_EN_MASK           (0x1U << CRU_CLKGATE8_CON_ACLK_RKVDEC_SRC_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE8_CON_CLK_HEVC_CABAC_SRC_EN_SHIFT       (1U)
#define CRU_CLKGATE8_CON_CLK_HEVC_CABAC_SRC_EN_MASK        (0x1U << CRU_CLKGATE8_CON_CLK_HEVC_CABAC_SRC_EN_SHIFT)       /* 0x00000002 */
#define CRU_CLKGATE8_CON_CLK_HEVC_CORE_SRC_EN_SHIFT        (2U)
#define CRU_CLKGATE8_CON_CLK_HEVC_CORE_SRC_EN_MASK         (0x1U << CRU_CLKGATE8_CON_CLK_HEVC_CORE_SRC_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE8_CON_ACLK_VPU_SRC_EN_SHIFT             (3U)
#define CRU_CLKGATE8_CON_ACLK_VPU_SRC_EN_MASK              (0x1U << CRU_CLKGATE8_CON_ACLK_VPU_SRC_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE8_CON_HCLK_RKVDEC_EN_SHIFT              (4U)
#define CRU_CLKGATE8_CON_HCLK_RKVDEC_EN_MASK               (0x1U << CRU_CLKGATE8_CON_HCLK_RKVDEC_EN_SHIFT)              /* 0x00000010 */
#define CRU_CLKGATE8_CON_ACLK_RKVENC_SRC_EN_SHIFT          (8U)
#define CRU_CLKGATE8_CON_ACLK_RKVENC_SRC_EN_MASK           (0x1U << CRU_CLKGATE8_CON_ACLK_RKVENC_SRC_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE8_CON_CLK_VENC_CORE_SRC_EN_SHIFT        (9U)
#define CRU_CLKGATE8_CON_CLK_VENC_CORE_SRC_EN_MASK         (0x1U << CRU_CLKGATE8_CON_CLK_VENC_CORE_SRC_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE8_CON_HCLK_RKVENC_EN_SHIFT              (10U)
#define CRU_CLKGATE8_CON_HCLK_RKVENC_EN_MASK               (0x1U << CRU_CLKGATE8_CON_HCLK_RKVENC_EN_SHIFT)              /* 0x00000400 */
#define CRU_CLKGATE8_CON_PCLK_PMU_EN_SHIFT                 (12U)
#define CRU_CLKGATE8_CON_PCLK_PMU_EN_MASK                  (0x1U << CRU_CLKGATE8_CON_PCLK_PMU_EN_SHIFT)                 /* 0x00001000 */
#define CRU_CLKGATE8_CON_CLK_PVTM_PMU_EN_SHIFT             (13U)
#define CRU_CLKGATE8_CON_CLK_PVTM_PMU_EN_MASK              (0x1U << CRU_CLKGATE8_CON_CLK_PVTM_PMU_EN_SHIFT)             /* 0x00002000 */
#define CRU_CLKGATE8_CON_CLK_I2C0_PMU_SRC_EN_SHIFT         (14U)
#define CRU_CLKGATE8_CON_CLK_I2C0_PMU_SRC_EN_MASK          (0x1U << CRU_CLKGATE8_CON_CLK_I2C0_PMU_SRC_EN_SHIFT)         /* 0x00004000 */
#define CRU_CLKGATE8_CON_CLK_PWM0_PMU_SRC_EN_SHIFT         (15U)
#define CRU_CLKGATE8_CON_CLK_PWM0_PMU_SRC_EN_MASK          (0x1U << CRU_CLKGATE8_CON_CLK_PWM0_PMU_SRC_EN_SHIFT)         /* 0x00008000 */
/* CLKGATE9_CON */
#define CRU_CLKGATE9_CON_CLK_DSP_SRC_EN_SHIFT              (0U)
#define CRU_CLKGATE9_CON_CLK_DSP_SRC_EN_MASK               (0x1U << CRU_CLKGATE9_CON_CLK_DSP_SRC_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE9_CON_CLK_DSP_IOP_EN_SHIFT              (1U)
#define CRU_CLKGATE9_CON_CLK_DSP_IOP_EN_MASK               (0x1U << CRU_CLKGATE9_CON_CLK_DSP_IOP_EN_SHIFT)              /* 0x00000002 */
#define CRU_CLKGATE9_CON_CLK_DSP_EPP_EN_SHIFT              (2U)
#define CRU_CLKGATE9_CON_CLK_DSP_EPP_EN_MASK               (0x1U << CRU_CLKGATE9_CON_CLK_DSP_EPP_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE9_CON_CLK_DSP_EDP_EN_SHIFT              (3U)
#define CRU_CLKGATE9_CON_CLK_DSP_EDP_EN_MASK               (0x1U << CRU_CLKGATE9_CON_CLK_DSP_EDP_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE9_CON_CLK_DSP_EDAP_EN_SHIFT             (4U)
#define CRU_CLKGATE9_CON_CLK_DSP_EDAP_EN_MASK              (0x1U << CRU_CLKGATE9_CON_CLK_DSP_EDAP_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE9_CON_PCLK_DSP_SRC_EN_SHIFT             (5U)
#define CRU_CLKGATE9_CON_PCLK_DSP_SRC_EN_MASK              (0x1U << CRU_CLKGATE9_CON_PCLK_DSP_SRC_EN_SHIFT)             /* 0x00000020 */
#define CRU_CLKGATE9_CON_CLK_DSP_PFM_MON_EN_SHIFT          (6U)
#define CRU_CLKGATE9_CON_CLK_DSP_PFM_MON_EN_MASK           (0x1U << CRU_CLKGATE9_CON_CLK_DSP_PFM_MON_EN_SHIFT)          /* 0x00000040 */
#define CRU_CLKGATE9_CON_CLK_WIFI_SRC_EN_SHIFT             (8U)
#define CRU_CLKGATE9_CON_CLK_WIFI_SRC_EN_MASK              (0x1U << CRU_CLKGATE9_CON_CLK_WIFI_SRC_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE9_CON_TESTCLK_EN_SHIFT                  (9U)
#define CRU_CLKGATE9_CON_TESTCLK_EN_MASK                   (0x1U << CRU_CLKGATE9_CON_TESTCLK_EN_SHIFT)                  /* 0x00000200 */
#define CRU_CLKGATE9_CON_CLK_DSIPHY_24M_EN_SHIFT           (10U)
#define CRU_CLKGATE9_CON_CLK_DSIPHY_24M_EN_MASK            (0x1U << CRU_CLKGATE9_CON_CLK_DSIPHY_24M_EN_SHIFT)           /* 0x00000400 */
#define CRU_CLKGATE9_CON_CLK_CIF_OUT_SRC_EN_SHIFT          (11U)
#define CRU_CLKGATE9_CON_CLK_CIF_OUT_SRC_EN_MASK           (0x1U << CRU_CLKGATE9_CON_CLK_CIF_OUT_SRC_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE9_CON_CLK_MIPI_CSI_OUT_SRC_EN_SHIFT     (12U)
#define CRU_CLKGATE9_CON_CLK_MIPI_CSI_OUT_SRC_EN_MASK      (0x1U << CRU_CLKGATE9_CON_CLK_MIPI_CSI_OUT_SRC_EN_SHIFT)     /* 0x00001000 */
/* CLKGATE10_CON */
#define CRU_CLKGATE10_CON_PCLK_PMU_EN_SHIFT                (0U)
#define CRU_CLKGATE10_CON_PCLK_PMU_EN_MASK                 (0x1U << CRU_CLKGATE10_CON_PCLK_PMU_EN_SHIFT)                /* 0x00000001 */
#define CRU_CLKGATE10_CON_PCLK_INTMEM1_EN_SHIFT            (1U)
#define CRU_CLKGATE10_CON_PCLK_INTMEM1_EN_MASK             (0x1U << CRU_CLKGATE10_CON_PCLK_INTMEM1_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE10_CON_PCLK_GPIO0_EN_SHIFT              (2U)
#define CRU_CLKGATE10_CON_PCLK_GPIO0_EN_MASK               (0x1U << CRU_CLKGATE10_CON_PCLK_GPIO0_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE10_CON_PCLK_PMUGRF_EN_SHIFT             (3U)
#define CRU_CLKGATE10_CON_PCLK_PMUGRF_EN_MASK              (0x1U << CRU_CLKGATE10_CON_PCLK_PMUGRF_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE10_CON_PCLK_PMU_NIU_EN_SHIFT            (4U)
#define CRU_CLKGATE10_CON_PCLK_PMU_NIU_EN_MASK             (0x1U << CRU_CLKGATE10_CON_PCLK_PMU_NIU_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE10_CON_PCLK_I2C_PMU_EN_SHIFT            (5U)
#define CRU_CLKGATE10_CON_PCLK_I2C_PMU_EN_MASK             (0x1U << CRU_CLKGATE10_CON_PCLK_I2C_PMU_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE10_CON_PCLK_PWM_PMU_EN_SHIFT            (6U)
#define CRU_CLKGATE10_CON_PCLK_PWM_PMU_EN_MASK             (0x1U << CRU_CLKGATE10_CON_PCLK_PWM_PMU_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE10_CON_CLK_DDRPHY4X_EN_SHIFT            (9U)
#define CRU_CLKGATE10_CON_CLK_DDRPHY4X_EN_MASK             (0x1U << CRU_CLKGATE10_CON_CLK_DDRPHY4X_EN_SHIFT)            /* 0x00000200 */
/* CLKGATE11_CON */
#define CRU_CLKGATE11_CON_ACLK_CORE_NIU_EN_SHIFT           (0U)
#define CRU_CLKGATE11_CON_ACLK_CORE_NIU_EN_MASK            (0x1U << CRU_CLKGATE11_CON_ACLK_CORE_NIU_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE11_CON_PCLK_CORE_NIU_EN_SHIFT           (1U)
#define CRU_CLKGATE11_CON_PCLK_CORE_NIU_EN_MASK            (0x1U << CRU_CLKGATE11_CON_PCLK_CORE_NIU_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE11_CON_ACLK_DSP_EDP_PERF_EN_SHIFT       (8U)
#define CRU_CLKGATE11_CON_ACLK_DSP_EDP_PERF_EN_MASK        (0x1U << CRU_CLKGATE11_CON_ACLK_DSP_EDP_PERF_EN_SHIFT)       /* 0x00000100 */
/* CLKGATE12_CON */
#define CRU_CLKGATE12_CON_CLK_INTMEM0_EN_SHIFT             (1U)
#define CRU_CLKGATE12_CON_CLK_INTMEM0_EN_MASK              (0x1U << CRU_CLKGATE12_CON_CLK_INTMEM0_EN_SHIFT)             /* 0x00000002 */
#define CRU_CLKGATE12_CON_ACLK_DMAC_BUS_EN_SHIFT           (2U)
#define CRU_CLKGATE12_CON_ACLK_DMAC_BUS_EN_MASK            (0x1U << CRU_CLKGATE12_CON_ACLK_DMAC_BUS_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE12_CON_HCLK_ROM_EN_SHIFT                (3U)
#define CRU_CLKGATE12_CON_HCLK_ROM_EN_MASK                 (0x1U << CRU_CLKGATE12_CON_HCLK_ROM_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE12_CON_PCLK_DDR_UPCTL_EN_SHIFT          (4U)
#define CRU_CLKGATE12_CON_PCLK_DDR_UPCTL_EN_MASK           (0x1U << CRU_CLKGATE12_CON_PCLK_DDR_UPCTL_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE12_CON_CLK_DDR_UPCTL_EN_SHIFT           (5U)
#define CRU_CLKGATE12_CON_CLK_DDR_UPCTL_EN_MASK            (0x1U << CRU_CLKGATE12_CON_CLK_DDR_UPCTL_EN_SHIFT)           /* 0x00000020 */
#define CRU_CLKGATE12_CON_PCLK_DDRMON_EN_SHIFT             (6U)
#define CRU_CLKGATE12_CON_PCLK_DDRMON_EN_MASK              (0x1U << CRU_CLKGATE12_CON_PCLK_DDRMON_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE12_CON_HCLK_I2S0_8CH_EN_SHIFT           (7U)
#define CRU_CLKGATE12_CON_HCLK_I2S0_8CH_EN_MASK            (0x1U << CRU_CLKGATE12_CON_HCLK_I2S0_8CH_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE12_CON_HCLK_I2S1_2CH_EN_SHIFT           (8U)
#define CRU_CLKGATE12_CON_HCLK_I2S1_2CH_EN_MASK            (0x1U << CRU_CLKGATE12_CON_HCLK_I2S1_2CH_EN_SHIFT)           /* 0x00000100 */
#define CRU_CLKGATE12_CON_HCLK_I2S2_2CH_EN_SHIFT           (9U)
#define CRU_CLKGATE12_CON_HCLK_I2S2_2CH_EN_MASK            (0x1U << CRU_CLKGATE12_CON_HCLK_I2S2_2CH_EN_SHIFT)           /* 0x00000200 */
#define CRU_CLKGATE12_CON_MCLK_CRYPTO_EN_SHIFT             (10U)
#define CRU_CLKGATE12_CON_MCLK_CRYPTO_EN_MASK              (0x1U << CRU_CLKGATE12_CON_MCLK_CRYPTO_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE12_CON_SCLK_CRYPTO_EN_SHIFT             (11U)
#define CRU_CLKGATE12_CON_SCLK_CRYPTO_EN_MASK              (0x1U << CRU_CLKGATE12_CON_SCLK_CRYPTO_EN_SHIFT)             /* 0x00000800 */
#define CRU_CLKGATE12_CON_PCLK_EFUSE_0_EN_SHIFT            (12U)
#define CRU_CLKGATE12_CON_PCLK_EFUSE_0_EN_MASK             (0x1U << CRU_CLKGATE12_CON_PCLK_EFUSE_0_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE12_CON_PCLK_EFUSE_1_EN_SHIFT            (13U)
#define CRU_CLKGATE12_CON_PCLK_EFUSE_1_EN_MASK             (0x1U << CRU_CLKGATE12_CON_PCLK_EFUSE_1_EN_SHIFT)            /* 0x00002000 */
/* CLKGATE13_CON */
#define CRU_CLKGATE13_CON_PCLK_I2C1_EN_SHIFT               (0U)
#define CRU_CLKGATE13_CON_PCLK_I2C1_EN_MASK                (0x1U << CRU_CLKGATE13_CON_PCLK_I2C1_EN_SHIFT)               /* 0x00000001 */
#define CRU_CLKGATE13_CON_PCLK_I2C2_EN_SHIFT               (1U)
#define CRU_CLKGATE13_CON_PCLK_I2C2_EN_MASK                (0x1U << CRU_CLKGATE13_CON_PCLK_I2C2_EN_SHIFT)               /* 0x00000002 */
#define CRU_CLKGATE13_CON_PCLK_I2C3_EN_SHIFT               (2U)
#define CRU_CLKGATE13_CON_PCLK_I2C3_EN_MASK                (0x1U << CRU_CLKGATE13_CON_PCLK_I2C3_EN_SHIFT)               /* 0x00000004 */
#define CRU_CLKGATE13_CON_PCLK_WDT_EN_SHIFT                (3U)
#define CRU_CLKGATE13_CON_PCLK_WDT_EN_MASK                 (0x1U << CRU_CLKGATE13_CON_PCLK_WDT_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE13_CON_PCLK_TIMER_EN_SHIFT              (4U)
#define CRU_CLKGATE13_CON_PCLK_TIMER_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_TIMER_EN_SHIFT)              /* 0x00000010 */
#define CRU_CLKGATE13_CON_PCLK_SPI0_EN_SHIFT               (5U)
#define CRU_CLKGATE13_CON_PCLK_SPI0_EN_MASK                (0x1U << CRU_CLKGATE13_CON_PCLK_SPI0_EN_SHIFT)               /* 0x00000020 */
#define CRU_CLKGATE13_CON_PCLK_RK_PWM1_EN_SHIFT            (6U)
#define CRU_CLKGATE13_CON_PCLK_RK_PWM1_EN_MASK             (0x1U << CRU_CLKGATE13_CON_PCLK_RK_PWM1_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE13_CON_PCLK_GPIO1_EN_SHIFT              (7U)
#define CRU_CLKGATE13_CON_PCLK_GPIO1_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_GPIO1_EN_SHIFT)              /* 0x00000080 */
#define CRU_CLKGATE13_CON_PCLK_GPIO2_EN_SHIFT              (8U)
#define CRU_CLKGATE13_CON_PCLK_GPIO2_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_GPIO2_EN_SHIFT)              /* 0x00000100 */
#define CRU_CLKGATE13_CON_PCLK_GPIO3_EN_SHIFT              (9U)
#define CRU_CLKGATE13_CON_PCLK_GPIO3_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_GPIO3_EN_SHIFT)              /* 0x00000200 */
#define CRU_CLKGATE13_CON_PCLK_UART0_EN_SHIFT              (10U)
#define CRU_CLKGATE13_CON_PCLK_UART0_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_UART0_EN_SHIFT)              /* 0x00000400 */
#define CRU_CLKGATE13_CON_PCLK_UART1_EN_SHIFT              (11U)
#define CRU_CLKGATE13_CON_PCLK_UART1_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_UART1_EN_SHIFT)              /* 0x00000800 */
#define CRU_CLKGATE13_CON_PCLK_UART2_EN_SHIFT              (12U)
#define CRU_CLKGATE13_CON_PCLK_UART2_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_UART2_EN_SHIFT)              /* 0x00001000 */
#define CRU_CLKGATE13_CON_PCLK_TSADC_EN_SHIFT              (13U)
#define CRU_CLKGATE13_CON_PCLK_TSADC_EN_MASK               (0x1U << CRU_CLKGATE13_CON_PCLK_TSADC_EN_SHIFT)              /* 0x00002000 */
#define CRU_CLKGATE13_CON_PCLK_SARADC_EN_SHIFT             (14U)
#define CRU_CLKGATE13_CON_PCLK_SARADC_EN_MASK              (0x1U << CRU_CLKGATE13_CON_PCLK_SARADC_EN_SHIFT)             /* 0x00004000 */
/* CLKGATE14_CON */
#define CRU_CLKGATE14_CON_PCLK_GRF_EN_SHIFT                (0U)
#define CRU_CLKGATE14_CON_PCLK_GRF_EN_MASK                 (0x1U << CRU_CLKGATE14_CON_PCLK_GRF_EN_SHIFT)                /* 0x00000001 */
#define CRU_CLKGATE14_CON_ACLK_NIU_EN_SHIFT                (1U)
#define CRU_CLKGATE14_CON_ACLK_NIU_EN_MASK                 (0x1U << CRU_CLKGATE14_CON_ACLK_NIU_EN_SHIFT)                /* 0x00000002 */
#define CRU_CLKGATE14_CON_PCLK_MSCHNIU_EN_SHIFT            (2U)
#define CRU_CLKGATE14_CON_PCLK_MSCHNIU_EN_MASK             (0x1U << CRU_CLKGATE14_CON_PCLK_MSCHNIU_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE14_CON_PCLK_TOP_NIU_EN_SHIFT            (3U)
#define CRU_CLKGATE14_CON_PCLK_TOP_NIU_EN_MASK             (0x1U << CRU_CLKGATE14_CON_PCLK_TOP_NIU_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE14_CON_PCLK_DDRPHY_EN_SHIFT             (4U)
#define CRU_CLKGATE14_CON_PCLK_DDRPHY_EN_MASK              (0x1U << CRU_CLKGATE14_CON_PCLK_DDRPHY_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE14_CON_PCLK_CRU_EN_SHIFT                (5U)
#define CRU_CLKGATE14_CON_PCLK_CRU_EN_MASK                 (0x1U << CRU_CLKGATE14_CON_PCLK_CRU_EN_SHIFT)                /* 0x00000020 */
#define CRU_CLKGATE14_CON_PCLK_ACODECPHY_EN_SHIFT          (6U)
#define CRU_CLKGATE14_CON_PCLK_ACODECPHY_EN_MASK           (0x1U << CRU_CLKGATE14_CON_PCLK_ACODECPHY_EN_SHIFT)          /* 0x00000040 */
#define CRU_CLKGATE14_CON_PCLK_HDMIPHY_EN_SHIFT            (8U)
#define CRU_CLKGATE14_CON_PCLK_HDMIPHY_EN_MASK             (0x1U << CRU_CLKGATE14_CON_PCLK_HDMIPHY_EN_SHIFT)            /* 0x00000100 */
#define CRU_CLKGATE14_CON_PCLK_VDACPHY_EN_SHIFT            (9U)
#define CRU_CLKGATE14_CON_PCLK_VDACPHY_EN_MASK             (0x1U << CRU_CLKGATE14_CON_PCLK_VDACPHY_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE14_CON_PCLK_MIPI_DSI_PHY_EN_SHIFT       (11U)
#define CRU_CLKGATE14_CON_PCLK_MIPI_DSI_PHY_EN_MASK        (0x1U << CRU_CLKGATE14_CON_PCLK_MIPI_DSI_PHY_EN_SHIFT)       /* 0x00000800 */
#define CRU_CLKGATE14_CON_PCLK_MIPI_CSI_PHY_EN_SHIFT       (12U)
#define CRU_CLKGATE14_CON_PCLK_MIPI_CSI_PHY_EN_MASK        (0x1U << CRU_CLKGATE14_CON_PCLK_MIPI_CSI_PHY_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE14_CON_PCLK_USBGRF_EN_SHIFT             (14U)
#define CRU_CLKGATE14_CON_PCLK_USBGRF_EN_MASK              (0x1U << CRU_CLKGATE14_CON_PCLK_USBGRF_EN_SHIFT)             /* 0x00004000 */
/* CLKGATE15_CON */
#define CRU_CLKGATE15_CON_HCLK_SDMMC_EN_SHIFT              (0U)
#define CRU_CLKGATE15_CON_HCLK_SDMMC_EN_MASK               (0x1U << CRU_CLKGATE15_CON_HCLK_SDMMC_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE15_CON_HCLK_SDIO_EN_SHIFT               (1U)
#define CRU_CLKGATE15_CON_HCLK_SDIO_EN_MASK                (0x1U << CRU_CLKGATE15_CON_HCLK_SDIO_EN_SHIFT)               /* 0x00000002 */
#define CRU_CLKGATE15_CON_HCLK_EMMC_EN_SHIFT               (2U)
#define CRU_CLKGATE15_CON_HCLK_EMMC_EN_MASK                (0x1U << CRU_CLKGATE15_CON_HCLK_EMMC_EN_SHIFT)               /* 0x00000004 */
#define CRU_CLKGATE15_CON_HCLK_NANDC_EN_SHIFT              (3U)
#define CRU_CLKGATE15_CON_HCLK_NANDC_EN_MASK               (0x1U << CRU_CLKGATE15_CON_HCLK_NANDC_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE15_CON_ACLK_GMAC_EN_SHIFT               (4U)
#define CRU_CLKGATE15_CON_ACLK_GMAC_EN_MASK                (0x1U << CRU_CLKGATE15_CON_ACLK_GMAC_EN_SHIFT)               /* 0x00000010 */
#define CRU_CLKGATE15_CON_PCLK_GMAC_EN_SHIFT               (5U)
#define CRU_CLKGATE15_CON_PCLK_GMAC_EN_MASK                (0x1U << CRU_CLKGATE15_CON_PCLK_GMAC_EN_SHIFT)               /* 0x00000020 */
#define CRU_CLKGATE15_CON_HCLK_HOST0_EN_SHIFT              (6U)
#define CRU_CLKGATE15_CON_HCLK_HOST0_EN_MASK               (0x1U << CRU_CLKGATE15_CON_HCLK_HOST0_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE15_CON_HCLK_HOST0_ARB_EN_SHIFT          (7U)
#define CRU_CLKGATE15_CON_HCLK_HOST0_ARB_EN_MASK           (0x1U << CRU_CLKGATE15_CON_HCLK_HOST0_ARB_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE15_CON_HCLK_OTG_EN_SHIFT                (8U)
#define CRU_CLKGATE15_CON_HCLK_OTG_EN_MASK                 (0x1U << CRU_CLKGATE15_CON_HCLK_OTG_EN_SHIFT)                /* 0x00000100 */
#define CRU_CLKGATE15_CON_HCLK_OTG_PMU_EN_SHIFT            (9U)
#define CRU_CLKGATE15_CON_HCLK_OTG_PMU_EN_MASK             (0x1U << CRU_CLKGATE15_CON_HCLK_OTG_PMU_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE15_CON_HCLK_SFC_EN_SHIFT                (10U)
#define CRU_CLKGATE15_CON_HCLK_SFC_EN_MASK                 (0x1U << CRU_CLKGATE15_CON_HCLK_SFC_EN_SHIFT)                /* 0x00000400 */
#define CRU_CLKGATE15_CON_ACLK_PERI_NIU_EN_SHIFT           (11U)
#define CRU_CLKGATE15_CON_ACLK_PERI_NIU_EN_MASK            (0x1U << CRU_CLKGATE15_CON_ACLK_PERI_NIU_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE15_CON_HCLK_PERI_NIU_EN_SHIFT           (12U)
#define CRU_CLKGATE15_CON_HCLK_PERI_NIU_EN_MASK            (0x1U << CRU_CLKGATE15_CON_HCLK_PERI_NIU_EN_SHIFT)           /* 0x00001000 */
#define CRU_CLKGATE15_CON_PCLK_PERI_NIU_EN_SHIFT           (13U)
#define CRU_CLKGATE15_CON_PCLK_PERI_NIU_EN_MASK            (0x1U << CRU_CLKGATE15_CON_PCLK_PERI_NIU_EN_SHIFT)           /* 0x00002000 */
/* CLKGATE16_CON */
#define CRU_CLKGATE16_CON_CLK_DSP_SYS_WDOG_EN_SHIFT        (0U)
#define CRU_CLKGATE16_CON_CLK_DSP_SYS_WDOG_EN_MASK         (0x1U << CRU_CLKGATE16_CON_CLK_DSP_SYS_WDOG_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE16_CON_CLK_DSP_EPP_WDOG_EN_SHIFT        (1U)
#define CRU_CLKGATE16_CON_CLK_DSP_EPP_WDOG_EN_MASK         (0x1U << CRU_CLKGATE16_CON_CLK_DSP_EPP_WDOG_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE16_CON_CLK_DSP_EDP_WDOG_EN_SHIFT        (2U)
#define CRU_CLKGATE16_CON_CLK_DSP_EDP_WDOG_EN_MASK         (0x1U << CRU_CLKGATE16_CON_CLK_DSP_EDP_WDOG_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE16_CON_CLK_DSP_IOP_WDOG_EN_SHIFT        (3U)
#define CRU_CLKGATE16_CON_CLK_DSP_IOP_WDOG_EN_MASK         (0x1U << CRU_CLKGATE16_CON_CLK_DSP_IOP_WDOG_EN_SHIFT)        /* 0x00000008 */
#define CRU_CLKGATE16_CON_PCLK_DSP_IOP_NIU_EN_SHIFT        (4U)
#define CRU_CLKGATE16_CON_PCLK_DSP_IOP_NIU_EN_MASK         (0x1U << CRU_CLKGATE16_CON_PCLK_DSP_IOP_NIU_EN_SHIFT)        /* 0x00000010 */
#define CRU_CLKGATE16_CON_ACLK_DSP_EPP_NIU_EN_SHIFT        (5U)
#define CRU_CLKGATE16_CON_ACLK_DSP_EPP_NIU_EN_MASK         (0x1U << CRU_CLKGATE16_CON_ACLK_DSP_EPP_NIU_EN_SHIFT)        /* 0x00000020 */
#define CRU_CLKGATE16_CON_ACLK_DSP_EDP_NIU_EN_SHIFT        (6U)
#define CRU_CLKGATE16_CON_ACLK_DSP_EDP_NIU_EN_MASK         (0x1U << CRU_CLKGATE16_CON_ACLK_DSP_EDP_NIU_EN_SHIFT)        /* 0x00000040 */
#define CRU_CLKGATE16_CON_PCLK_DSP_DBG_NIU_EN_SHIFT        (7U)
#define CRU_CLKGATE16_CON_PCLK_DSP_DBG_NIU_EN_MASK         (0x1U << CRU_CLKGATE16_CON_PCLK_DSP_DBG_NIU_EN_SHIFT)        /* 0x00000080 */
#define CRU_CLKGATE16_CON_PCLK_DSP_CFG_NIU_EN_SHIFT        (8U)
#define CRU_CLKGATE16_CON_PCLK_DSP_CFG_NIU_EN_MASK         (0x1U << CRU_CLKGATE16_CON_PCLK_DSP_CFG_NIU_EN_SHIFT)        /* 0x00000100 */
#define CRU_CLKGATE16_CON_PCLK_DSP_PFM_MON_EN_SHIFT        (9U)
#define CRU_CLKGATE16_CON_PCLK_DSP_PFM_MON_EN_MASK         (0x1U << CRU_CLKGATE16_CON_PCLK_DSP_PFM_MON_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE16_CON_PCLK_INTC_EN_SHIFT               (10U)
#define CRU_CLKGATE16_CON_PCLK_INTC_EN_MASK                (0x1U << CRU_CLKGATE16_CON_PCLK_INTC_EN_SHIFT)               /* 0x00000400 */
#define CRU_CLKGATE16_CON_PCLK_DSP_GRF_EN_SHIFT            (11U)
#define CRU_CLKGATE16_CON_PCLK_DSP_GRF_EN_MASK             (0x1U << CRU_CLKGATE16_CON_PCLK_DSP_GRF_EN_SHIFT)            /* 0x00000800 */
#define CRU_CLKGATE16_CON_PCLK_MAILBOX_EN_SHIFT            (12U)
#define CRU_CLKGATE16_CON_PCLK_MAILBOX_EN_MASK             (0x1U << CRU_CLKGATE16_CON_PCLK_MAILBOX_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE16_CON_CLK_DSP_FREE_EN_SHIFT            (13U)
#define CRU_CLKGATE16_CON_CLK_DSP_FREE_EN_MASK             (0x1U << CRU_CLKGATE16_CON_CLK_DSP_FREE_EN_SHIFT)            /* 0x00002000 */
#define CRU_CLKGATE16_CON_ACLK_DSP_EDAP_NIU_EN_SHIFT       (14U)
#define CRU_CLKGATE16_CON_ACLK_DSP_EDAP_NIU_EN_MASK        (0x1U << CRU_CLKGATE16_CON_ACLK_DSP_EDAP_NIU_EN_SHIFT)       /* 0x00004000 */
#define CRU_CLKGATE16_CON_ACLK_DSP_EPP_PERF_EN_SHIFT       (15U)
#define CRU_CLKGATE16_CON_ACLK_DSP_EPP_PERF_EN_MASK        (0x1U << CRU_CLKGATE16_CON_ACLK_DSP_EPP_PERF_EN_SHIFT)       /* 0x00008000 */
/* CLKGATE17_CON */
#define CRU_CLKGATE17_CON_ACLK_VIO0_EN_SHIFT               (0U)
#define CRU_CLKGATE17_CON_ACLK_VIO0_EN_MASK                (0x1U << CRU_CLKGATE17_CON_ACLK_VIO0_EN_SHIFT)               /* 0x00000001 */
#define CRU_CLKGATE17_CON_ACLK_VIO1_EN_SHIFT               (1U)
#define CRU_CLKGATE17_CON_ACLK_VIO1_EN_MASK                (0x1U << CRU_CLKGATE17_CON_ACLK_VIO1_EN_SHIFT)               /* 0x00000002 */
#define CRU_CLKGATE17_CON_HCLK_VIO_EN_SHIFT                (2U)
#define CRU_CLKGATE17_CON_HCLK_VIO_EN_MASK                 (0x1U << CRU_CLKGATE17_CON_HCLK_VIO_EN_SHIFT)                /* 0x00000004 */
#define CRU_CLKGATE17_CON_PCLK_VIO_EN_SHIFT                (3U)
#define CRU_CLKGATE17_CON_PCLK_VIO_EN_MASK                 (0x1U << CRU_CLKGATE17_CON_PCLK_VIO_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE17_CON_ACLK_CIF1_EN_SHIFT               (6U)
#define CRU_CLKGATE17_CON_ACLK_CIF1_EN_MASK                (0x1U << CRU_CLKGATE17_CON_ACLK_CIF1_EN_SHIFT)               /* 0x00000040 */
#define CRU_CLKGATE17_CON_HCLK_CIF1_EN_SHIFT               (7U)
#define CRU_CLKGATE17_CON_HCLK_CIF1_EN_MASK                (0x1U << CRU_CLKGATE17_CON_HCLK_CIF1_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE17_CON_ACLK_CIF2_EN_SHIFT               (8U)
#define CRU_CLKGATE17_CON_ACLK_CIF2_EN_MASK                (0x1U << CRU_CLKGATE17_CON_ACLK_CIF2_EN_SHIFT)               /* 0x00000100 */
#define CRU_CLKGATE17_CON_HCLK_CIF2_EN_SHIFT               (9U)
#define CRU_CLKGATE17_CON_HCLK_CIF2_EN_MASK                (0x1U << CRU_CLKGATE17_CON_HCLK_CIF2_EN_SHIFT)               /* 0x00000200 */
#define CRU_CLKGATE17_CON_ACLK_CIF3_EN_SHIFT               (10U)
#define CRU_CLKGATE17_CON_ACLK_CIF3_EN_MASK                (0x1U << CRU_CLKGATE17_CON_ACLK_CIF3_EN_SHIFT)               /* 0x00000400 */
#define CRU_CLKGATE17_CON_HCLK_CIF3_EN_SHIFT               (11U)
#define CRU_CLKGATE17_CON_HCLK_CIF3_EN_MASK                (0x1U << CRU_CLKGATE17_CON_HCLK_CIF3_EN_SHIFT)               /* 0x00000800 */
#define CRU_CLKGATE17_CON_HCLK_CVBS_EN_SHIFT               (12U)
#define CRU_CLKGATE17_CON_HCLK_CVBS_EN_MASK                (0x1U << CRU_CLKGATE17_CON_HCLK_CVBS_EN_SHIFT)               /* 0x00001000 */
/* CLKGATE18_CON */
#define CRU_CLKGATE18_CON_ACLK_VOP_EN_SHIFT                (0U)
#define CRU_CLKGATE18_CON_ACLK_VOP_EN_MASK                 (0x1U << CRU_CLKGATE18_CON_ACLK_VOP_EN_SHIFT)                /* 0x00000001 */
#define CRU_CLKGATE18_CON_HCLK_VOP_EN_SHIFT                (1U)
#define CRU_CLKGATE18_CON_HCLK_VOP_EN_MASK                 (0x1U << CRU_CLKGATE18_CON_HCLK_VOP_EN_SHIFT)                /* 0x00000002 */
#define CRU_CLKGATE18_CON_ACLK_IEP_EN_SHIFT                (2U)
#define CRU_CLKGATE18_CON_ACLK_IEP_EN_MASK                 (0x1U << CRU_CLKGATE18_CON_ACLK_IEP_EN_SHIFT)                /* 0x00000004 */
#define CRU_CLKGATE18_CON_HCLK_IEP_EN_SHIFT                (3U)
#define CRU_CLKGATE18_CON_HCLK_IEP_EN_MASK                 (0x1U << CRU_CLKGATE18_CON_HCLK_IEP_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE18_CON_ACLK_RGA_EN_SHIFT                (4U)
#define CRU_CLKGATE18_CON_ACLK_RGA_EN_MASK                 (0x1U << CRU_CLKGATE18_CON_ACLK_RGA_EN_SHIFT)                /* 0x00000010 */
#define CRU_CLKGATE18_CON_HCLK_RGA_EN_SHIFT                (5U)
#define CRU_CLKGATE18_CON_HCLK_RGA_EN_MASK                 (0x1U << CRU_CLKGATE18_CON_HCLK_RGA_EN_SHIFT)                /* 0x00000020 */
#define CRU_CLKGATE18_CON_ACLK_CIF0_EN_SHIFT               (6U)
#define CRU_CLKGATE18_CON_ACLK_CIF0_EN_MASK                (0x1U << CRU_CLKGATE18_CON_ACLK_CIF0_EN_SHIFT)               /* 0x00000040 */
#define CRU_CLKGATE18_CON_HCLK_CIF0_EN_SHIFT               (7U)
#define CRU_CLKGATE18_CON_HCLK_CIF0_EN_MASK                (0x1U << CRU_CLKGATE18_CON_HCLK_CIF0_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE18_CON_PCLK_MIPI_DSI0_EN_SHIFT          (8U)
#define CRU_CLKGATE18_CON_PCLK_MIPI_DSI0_EN_MASK           (0x1U << CRU_CLKGATE18_CON_PCLK_MIPI_DSI0_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE18_CON_PCLK_HDMI_CTRL_EN_SHIFT          (9U)
#define CRU_CLKGATE18_CON_PCLK_HDMI_CTRL_EN_MASK           (0x1U << CRU_CLKGATE18_CON_PCLK_HDMI_CTRL_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE18_CON_PCLKIN_ISP_CTRL_EN_SHIFT         (10U)
#define CRU_CLKGATE18_CON_PCLKIN_ISP_CTRL_EN_MASK          (0x1U << CRU_CLKGATE18_CON_PCLKIN_ISP_CTRL_EN_SHIFT)         /* 0x00000400 */
#define CRU_CLKGATE18_CON_HCLK_ISP_CTRL_EN_SHIFT           (11U)
#define CRU_CLKGATE18_CON_HCLK_ISP_CTRL_EN_MASK            (0x1U << CRU_CLKGATE18_CON_HCLK_ISP_CTRL_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE18_CON_ACLK_ISP_CTRL_EN_SHIFT           (12U)
#define CRU_CLKGATE18_CON_ACLK_ISP_CTRL_EN_MASK            (0x1U << CRU_CLKGATE18_CON_ACLK_ISP_CTRL_EN_SHIFT)           /* 0x00001000 */
/* CLKGATE19_CON */
#define CRU_CLKGATE19_CON_ACLK_RKVDEC_EN_SHIFT             (0U)
#define CRU_CLKGATE19_CON_ACLK_RKVDEC_EN_MASK              (0x1U << CRU_CLKGATE19_CON_ACLK_RKVDEC_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE19_CON_ACLK_VPU_EN_SHIFT                (1U)
#define CRU_CLKGATE19_CON_ACLK_VPU_EN_MASK                 (0x1U << CRU_CLKGATE19_CON_ACLK_VPU_EN_SHIFT)                /* 0x00000002 */
#define CRU_CLKGATE19_CON_HCLK_RKVDEC_EN_SHIFT             (2U)
#define CRU_CLKGATE19_CON_HCLK_RKVDEC_EN_MASK              (0x1U << CRU_CLKGATE19_CON_HCLK_RKVDEC_EN_SHIFT)             /* 0x00000004 */
#define CRU_CLKGATE19_CON_HCLK_VPU_EN_SHIFT                (3U)
#define CRU_CLKGATE19_CON_HCLK_VPU_EN_MASK                 (0x1U << CRU_CLKGATE19_CON_HCLK_VPU_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE19_CON_ACLK_RKVDEC_NIU_EN_SHIFT         (4U)
#define CRU_CLKGATE19_CON_ACLK_RKVDEC_NIU_EN_MASK          (0x1U << CRU_CLKGATE19_CON_ACLK_RKVDEC_NIU_EN_SHIFT)         /* 0x00000010 */
#define CRU_CLKGATE19_CON_HCLK_RKVDEC_NIU_EN_SHIFT         (5U)
#define CRU_CLKGATE19_CON_HCLK_RKVDEC_NIU_EN_MASK          (0x1U << CRU_CLKGATE19_CON_HCLK_RKVDEC_NIU_EN_SHIFT)         /* 0x00000020 */
#define CRU_CLKGATE19_CON_ACLK_VPU_NIU_EN_SHIFT            (6U)
#define CRU_CLKGATE19_CON_ACLK_VPU_NIU_EN_MASK             (0x1U << CRU_CLKGATE19_CON_ACLK_VPU_NIU_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE19_CON_ACLK_RKVENC_EN_SHIFT             (8U)
#define CRU_CLKGATE19_CON_ACLK_RKVENC_EN_MASK              (0x1U << CRU_CLKGATE19_CON_ACLK_RKVENC_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE19_CON_HCLK_RKVENC_EN_SHIFT             (9U)
#define CRU_CLKGATE19_CON_HCLK_RKVENC_EN_MASK              (0x1U << CRU_CLKGATE19_CON_HCLK_RKVENC_EN_SHIFT)             /* 0x00000200 */
#define CRU_CLKGATE19_CON_HCLK_RKVENC_NIU_EN_SHIFT         (10U)
#define CRU_CLKGATE19_CON_HCLK_RKVENC_NIU_EN_MASK          (0x1U << CRU_CLKGATE19_CON_HCLK_RKVENC_NIU_EN_SHIFT)         /* 0x00000400 */
#define CRU_CLKGATE19_CON_ACLK_RKVENC_NIU_EN_SHIFT         (11U)
#define CRU_CLKGATE19_CON_ACLK_RKVENC_NIU_EN_MASK          (0x1U << CRU_CLKGATE19_CON_ACLK_RKVENC_NIU_EN_SHIFT)         /* 0x00000800 */
/* SOFTRST0_CON */
#define CRU_SOFTRST0_CON_COREPO_SRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST0_CON_COREPO_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST0_CON_COREPO_SRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST0_CON_CORE_SRSTN_REQ_SHIFT              (1U)
#define CRU_SOFTRST0_CON_CORE_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST0_CON_CORE_SRSTN_REQ_SHIFT)              /* 0x00000002 */
#define CRU_SOFTRST0_CON_L2_SRSTN_REQ_SHIFT                (2U)
#define CRU_SOFTRST0_CON_L2_SRSTN_REQ_MASK                 (0x1U << CRU_SOFTRST0_CON_L2_SRSTN_REQ_SHIFT)                /* 0x00000004 */
#define CRU_SOFTRST0_CON_CPU_NIU_SRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST0_CON_CPU_NIU_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST0_CON_CPU_NIU_SRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST0_CON_COREPO_SRSTN_REQ_T_SHIFT          (4U)
#define CRU_SOFTRST0_CON_COREPO_SRSTN_REQ_T_MASK           (0x1U << CRU_SOFTRST0_CON_COREPO_SRSTN_REQ_T_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST0_CON_CORE_SRSTN_REQ_T_SHIFT            (5U)
#define CRU_SOFTRST0_CON_CORE_SRSTN_REQ_T_MASK             (0x1U << CRU_SOFTRST0_CON_CORE_SRSTN_REQ_T_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST0_CON_L2_SRSTN_REQ_T_SHIFT              (6U)
#define CRU_SOFTRST0_CON_L2_SRSTN_REQ_T_MASK               (0x1U << CRU_SOFTRST0_CON_L2_SRSTN_REQ_T_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST0_CON_CORE_DBG_SRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST0_CON_CORE_DBG_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST0_CON_CORE_DBG_SRSTN_REQ_SHIFT)          /* 0x00000100 */
#define CRU_SOFTRST0_CON_DBG_PSRSTN_REQ_SHIFT              (9U)
#define CRU_SOFTRST0_CON_DBG_PSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST0_CON_DBG_PSRSTN_REQ_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST0_CON_DAP_PSRSTN_REQ_SHIFT              (10U)
#define CRU_SOFTRST0_CON_DAP_PSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST0_CON_DAP_PSRSTN_REQ_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST0_CON_DBG_NIU_PSRSTN_REQ_SHIFT          (11U)
#define CRU_SOFTRST0_CON_DBG_NIU_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST0_CON_DBG_NIU_PSRSTN_REQ_SHIFT)          /* 0x00000800 */
#define CRU_SOFTRST0_CON_STRC_SYS_ASRSTN_REQ_SHIFT         (15U)
#define CRU_SOFTRST0_CON_STRC_SYS_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST0_CON_STRC_SYS_ASRSTN_REQ_SHIFT)         /* 0x00008000 */
/* SOFTRST1_CON */
#define CRU_SOFTRST1_CON_DDRPHY_SRSTN_CLKDIV_REQ_SHIFT     (0U)
#define CRU_SOFTRST1_CON_DDRPHY_SRSTN_CLKDIV_REQ_MASK      (0x1U << CRU_SOFTRST1_CON_DDRPHY_SRSTN_CLKDIV_REQ_SHIFT)     /* 0x00000001 */
#define CRU_SOFTRST1_CON_DDRPHY_SRSTN_REQ_SHIFT            (1U)
#define CRU_SOFTRST1_CON_DDRPHY_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST1_CON_DDRPHY_SRSTN_REQ_SHIFT)            /* 0x00000002 */
#define CRU_SOFTRST1_CON_DDRPHY_PSRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST1_CON_DDRPHY_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_DDRPHY_PSRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST1_CON_HDMIPHY_PSRSTN_REQ_SHIFT          (3U)
#define CRU_SOFTRST1_CON_HDMIPHY_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST1_CON_HDMIPHY_PSRSTN_REQ_SHIFT)          /* 0x00000008 */
#define CRU_SOFTRST1_CON_VDACPHY_PSRSTN_REQ_SHIFT          (4U)
#define CRU_SOFTRST1_CON_VDACPHY_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST1_CON_VDACPHY_PSRSTN_REQ_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST1_CON_VADCPHY_PSRSTN_REQ_SHIFT          (5U)
#define CRU_SOFTRST1_CON_VADCPHY_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST1_CON_VADCPHY_PSRSTN_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST1_CON_MIPI_CSI_PHY_PSRSTN_REQ_SHIFT     (6U)
#define CRU_SOFTRST1_CON_MIPI_CSI_PHY_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST1_CON_MIPI_CSI_PHY_PSRSTN_REQ_SHIFT)     /* 0x00000040 */
#define CRU_SOFTRST1_CON_MIPI_DSI_PHY_PSRSTN_REQ_SHIFT     (7U)
#define CRU_SOFTRST1_CON_MIPI_DSI_PHY_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST1_CON_MIPI_DSI_PHY_PSRSTN_REQ_SHIFT)     /* 0x00000080 */
#define CRU_SOFTRST1_CON_ACODEC_PSRSTN_REQ_SHIFT           (8U)
#define CRU_SOFTRST1_CON_ACODEC_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_ACODEC_PSRSTN_REQ_SHIFT)           /* 0x00000100 */
#define CRU_SOFTRST1_CON_BUS_NIU_ASRSTN_REQ_SHIFT          (9U)
#define CRU_SOFTRST1_CON_BUS_NIU_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST1_CON_BUS_NIU_ASRSTN_REQ_SHIFT)          /* 0x00000200 */
#define CRU_SOFTRST1_CON_TOP_NIU_PSRSTN_REQ_SHIFT          (10U)
#define CRU_SOFTRST1_CON_TOP_NIU_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST1_CON_TOP_NIU_PSRSTN_REQ_SHIFT)          /* 0x00000400 */
#define CRU_SOFTRST1_CON_INTMEM_ASRSTN_REQ_SHIFT           (11U)
#define CRU_SOFTRST1_CON_INTMEM_ASRSTN_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_INTMEM_ASRSTN_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST1_CON_ROM_HSRSTN_REQ_SHIFT              (12U)
#define CRU_SOFTRST1_CON_ROM_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST1_CON_ROM_HSRSTN_REQ_SHIFT)              /* 0x00001000 */
#define CRU_SOFTRST1_CON_DMAC_ASRSTN_REQ_SHIFT             (13U)
#define CRU_SOFTRST1_CON_DMAC_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST1_CON_DMAC_ASRSTN_REQ_SHIFT)             /* 0x00002000 */
#define CRU_SOFTRST1_CON_MSCH_NIU_SRSTN_REQ_SHIFT          (14U)
#define CRU_SOFTRST1_CON_MSCH_NIU_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST1_CON_MSCH_NIU_SRSTN_REQ_SHIFT)          /* 0x00004000 */
#define CRU_SOFTRST1_CON_MSCH_NIU_PSRSTN_REQ_SHIFT         (15U)
#define CRU_SOFTRST1_CON_MSCH_NIU_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST1_CON_MSCH_NIU_PSRSTN_REQ_SHIFT)         /* 0x00008000 */
/* SOFTRST2_CON */
#define CRU_SOFTRST2_CON_DDRUPCTL_PSRSTN_REQ_SHIFT         (0U)
#define CRU_SOFTRST2_CON_DDRUPCTL_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_DDRUPCTL_PSRSTN_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST2_CON_DDRUPCTL_NSRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST2_CON_DDRUPCTL_NSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_DDRUPCTL_NSRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST2_CON_DDRMON_PSRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST2_CON_DDRMON_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_DDRMON_PSRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST2_CON_I2S0_8CH_HSRSTN_REQ_SHIFT         (3U)
#define CRU_SOFTRST2_CON_I2S0_8CH_HSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_I2S0_8CH_HSRSTN_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST2_CON_I2S0_8CH_MSRSTN_REQ_SHIFT         (4U)
#define CRU_SOFTRST2_CON_I2S0_8CH_MSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_I2S0_8CH_MSRSTN_REQ_SHIFT)         /* 0x00000010 */
#define CRU_SOFTRST2_CON_I2S1_2CH_HSRSTN_REQ_SHIFT         (5U)
#define CRU_SOFTRST2_CON_I2S1_2CH_HSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_I2S1_2CH_HSRSTN_REQ_SHIFT)         /* 0x00000020 */
#define CRU_SOFTRST2_CON_I2S1_2CH_MSRSTN_REQ_SHIFT         (6U)
#define CRU_SOFTRST2_CON_I2S1_2CH_MSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_I2S1_2CH_MSRSTN_REQ_SHIFT)         /* 0x00000040 */
#define CRU_SOFTRST2_CON_I2S2_2CH_HSRSTN_REQ_SHIFT         (7U)
#define CRU_SOFTRST2_CON_I2S2_2CH_HSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_I2S2_2CH_HSRSTN_REQ_SHIFT)         /* 0x00000080 */
#define CRU_SOFTRST2_CON_I2S2_2CH_MSRSTN_REQ_SHIFT         (8U)
#define CRU_SOFTRST2_CON_I2S2_2CH_MSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_I2S2_2CH_MSRSTN_REQ_SHIFT)         /* 0x00000100 */
#define CRU_SOFTRST2_CON_CRYPTO_HSRSTN_REQ_SHIFT           (9U)
#define CRU_SOFTRST2_CON_CRYPTO_HSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_CRYPTO_HSRSTN_REQ_SHIFT)           /* 0x00000200 */
#define CRU_SOFTRST2_CON_CRYPTO_SRSTN_REQ_SHIFT            (10U)
#define CRU_SOFTRST2_CON_CRYPTO_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_CRYPTO_SRSTN_REQ_SHIFT)            /* 0x00000400 */
#define CRU_SOFTRST2_CON_SPI_PSRSTN_REQ_SHIFT              (11U)
#define CRU_SOFTRST2_CON_SPI_PSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST2_CON_SPI_PSRSTN_REQ_SHIFT)              /* 0x00000800 */
#define CRU_SOFTRST2_CON_SPI_SRSTN_REQ_SHIFT               (12U)
#define CRU_SOFTRST2_CON_SPI_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST2_CON_SPI_SRSTN_REQ_SHIFT)               /* 0x00001000 */
#define CRU_SOFTRST2_CON_UART0_PSRSTN_REQ_SHIFT            (13U)
#define CRU_SOFTRST2_CON_UART0_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_UART0_PSRSTN_REQ_SHIFT)            /* 0x00002000 */
#define CRU_SOFTRST2_CON_UART1_PSRSTN_REQ_SHIFT            (14U)
#define CRU_SOFTRST2_CON_UART1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_UART1_PSRSTN_REQ_SHIFT)            /* 0x00004000 */
#define CRU_SOFTRST2_CON_UART2_PSRSTN_REQ_SHIFT            (15U)
#define CRU_SOFTRST2_CON_UART2_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_UART2_PSRSTN_REQ_SHIFT)            /* 0x00008000 */
/* SOFTRST3_CON */
#define CRU_SOFTRST3_CON_UART0_SRSTN_REQ_SHIFT             (0U)
#define CRU_SOFTRST3_CON_UART0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_UART0_SRSTN_REQ_SHIFT)             /* 0x00000001 */
#define CRU_SOFTRST3_CON_UART1_SRSTN_REQ_SHIFT             (1U)
#define CRU_SOFTRST3_CON_UART1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_UART1_SRSTN_REQ_SHIFT)             /* 0x00000002 */
#define CRU_SOFTRST3_CON_UART2_SRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST3_CON_UART2_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_UART2_SRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST3_CON_I2C1_PSRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST3_CON_I2C1_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_I2C1_PSRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST3_CON_I2C2_PSRSTN_REQ_SHIFT             (4U)
#define CRU_SOFTRST3_CON_I2C2_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_I2C2_PSRSTN_REQ_SHIFT)             /* 0x00000010 */
#define CRU_SOFTRST3_CON_I2C3_PSRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST3_CON_I2C3_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_I2C3_PSRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST3_CON_I2C1_SRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST3_CON_I2C1_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST3_CON_I2C1_SRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST3_CON_I2C2_SRSTN_REQ_SHIFT              (7U)
#define CRU_SOFTRST3_CON_I2C2_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST3_CON_I2C2_SRSTN_REQ_SHIFT)              /* 0x00000080 */
#define CRU_SOFTRST3_CON_I2C3_SRSTN_REQ_SHIFT              (8U)
#define CRU_SOFTRST3_CON_I2C3_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST3_CON_I2C3_SRSTN_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST3_CON_PWM1_PSRSTN_REQ_SHIFT             (10U)
#define CRU_SOFTRST3_CON_PWM1_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_PWM1_PSRSTN_REQ_SHIFT)             /* 0x00000400 */
#define CRU_SOFTRST3_CON_PWM1_SRSTN_REQ_SHIFT              (12U)
#define CRU_SOFTRST3_CON_PWM1_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST3_CON_PWM1_SRSTN_REQ_SHIFT)              /* 0x00001000 */
#define CRU_SOFTRST3_CON_WDT_PSRSTN_REQ_SHIFT              (13U)
#define CRU_SOFTRST3_CON_WDT_PSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST3_CON_WDT_PSRSTN_REQ_SHIFT)              /* 0x00002000 */
#define CRU_SOFTRST3_CON_GPIO1_PSRSTN_REQ_SHIFT            (14U)
#define CRU_SOFTRST3_CON_GPIO1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST3_CON_GPIO1_PSRSTN_REQ_SHIFT)            /* 0x00004000 */
#define CRU_SOFTRST3_CON_GPIO2_PSRSTN_REQ_SHIFT            (15U)
#define CRU_SOFTRST3_CON_GPIO2_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST3_CON_GPIO2_PSRSTN_REQ_SHIFT)            /* 0x00008000 */
/* SOFTRST4_CON */
#define CRU_SOFTRST4_CON_GPIO3_PSRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST4_CON_GPIO3_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_GPIO3_PSRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST4_CON_GRF_PSRSTN_REQ_SHIFT              (1U)
#define CRU_SOFTRST4_CON_GRF_PSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST4_CON_GRF_PSRSTN_REQ_SHIFT)              /* 0x00000002 */
#define CRU_SOFTRST4_CON_EFUSE_PSRSTN_REQ_SHIFT            (2U)
#define CRU_SOFTRST4_CON_EFUSE_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_EFUSE_PSRSTN_REQ_SHIFT)            /* 0x00000004 */
#define CRU_SOFTRST4_CON_EFUSE512_PSRSTN_REQ_SHIFT         (3U)
#define CRU_SOFTRST4_CON_EFUSE512_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST4_CON_EFUSE512_PSRSTN_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST4_CON_TIMER0_PSRSTN_REQ_SHIFT           (4U)
#define CRU_SOFTRST4_CON_TIMER0_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_TIMER0_PSRSTN_REQ_SHIFT)           /* 0x00000010 */
#define CRU_SOFTRST4_CON_TIMER0_SRSTN_REQ_SHIFT            (5U)
#define CRU_SOFTRST4_CON_TIMER0_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_TIMER0_SRSTN_REQ_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST4_CON_TIMER1_SRSTN_REQ_SHIFT            (6U)
#define CRU_SOFTRST4_CON_TIMER1_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_TIMER1_SRSTN_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST4_CON_TSADC_PSRSTN_REQ_SHIFT            (7U)
#define CRU_SOFTRST4_CON_TSADC_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_TSADC_PSRSTN_REQ_SHIFT)            /* 0x00000080 */
#define CRU_SOFTRST4_CON_TSADC_SRSTN_REQ_SHIFT             (8U)
#define CRU_SOFTRST4_CON_TSADC_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST4_CON_TSADC_SRSTN_REQ_SHIFT)             /* 0x00000100 */
#define CRU_SOFTRST4_CON_SARADC_PSRSTN_REQ_SHIFT           (9U)
#define CRU_SOFTRST4_CON_SARADC_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_SARADC_PSRSTN_REQ_SHIFT)           /* 0x00000200 */
#define CRU_SOFTRST4_CON_SARADC_SRSTN_REQ_SHIFT            (10U)
#define CRU_SOFTRST4_CON_SARADC_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_SARADC_SRSTN_REQ_SHIFT)            /* 0x00000400 */
#define CRU_SOFTRST4_CON_SYSBUS_HSRSTN_REQ_SHIFT           (11U)
#define CRU_SOFTRST4_CON_SYSBUS_HSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_SYSBUS_HSRSTN_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST4_CON_USBGRF_PSRSTN_REQ_SHIFT           (12U)
#define CRU_SOFTRST4_CON_USBGRF_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_USBGRF_PSRSTN_REQ_SHIFT)           /* 0x00001000 */
/* SOFTRST5_CON */
#define CRU_SOFTRST5_CON_PERIPH_NIU_ASRSTN_REQ_SHIFT       (0U)
#define CRU_SOFTRST5_CON_PERIPH_NIU_ASRSTN_REQ_MASK        (0x1U << CRU_SOFTRST5_CON_PERIPH_NIU_ASRSTN_REQ_SHIFT)       /* 0x00000001 */
#define CRU_SOFTRST5_CON_PERIPH_NIU_HSRSTN_REQ_SHIFT       (1U)
#define CRU_SOFTRST5_CON_PERIPH_NIU_HSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST5_CON_PERIPH_NIU_HSRSTN_REQ_SHIFT)       /* 0x00000002 */
#define CRU_SOFTRST5_CON_PERIPH_NIU_PSRSTN_REQ_SHIFT       (2U)
#define CRU_SOFTRST5_CON_PERIPH_NIU_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST5_CON_PERIPH_NIU_PSRSTN_REQ_SHIFT)       /* 0x00000004 */
#define CRU_SOFTRST5_CON_PERIPH_HSRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST5_CON_PERIPH_HSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST5_CON_PERIPH_HSRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST5_CON_SDMMC_HSRSTN_REQ_SHIFT            (4U)
#define CRU_SOFTRST5_CON_SDMMC_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_SDMMC_HSRSTN_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST5_CON_SDIO_HSRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST5_CON_SDIO_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST5_CON_SDIO_HSRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST5_CON_EMMC_HSRSTN_REQ_SHIFT             (6U)
#define CRU_SOFTRST5_CON_EMMC_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST5_CON_EMMC_HSRSTN_REQ_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST5_CON_NANDC_HSRSTN_REQ_SHIFT            (7U)
#define CRU_SOFTRST5_CON_NANDC_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_NANDC_HSRSTN_REQ_SHIFT)            /* 0x00000080 */
#define CRU_SOFTRST5_CON_NANDC_NSRSTN_REQ_SHIFT            (8U)
#define CRU_SOFTRST5_CON_NANDC_NSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_NANDC_NSRSTN_REQ_SHIFT)            /* 0x00000100 */
#define CRU_SOFTRST5_CON_SFC_HSRSTN_REQ_SHIFT              (9U)
#define CRU_SOFTRST5_CON_SFC_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST5_CON_SFC_HSRSTN_REQ_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST5_CON_SFC_SRSTN_REQ_SHIFT               (10U)
#define CRU_SOFTRST5_CON_SFC_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST5_CON_SFC_SRSTN_REQ_SHIFT)               /* 0x00000400 */
#define CRU_SOFTRST5_CON_GMAC_ASRSTN_REQ_SHIFT             (11U)
#define CRU_SOFTRST5_CON_GMAC_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST5_CON_GMAC_ASRSTN_REQ_SHIFT)             /* 0x00000800 */
#define CRU_SOFTRST5_CON_OTG_HSRSTN_REQ_SHIFT              (12U)
#define CRU_SOFTRST5_CON_OTG_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST5_CON_OTG_HSRSTN_REQ_SHIFT)              /* 0x00001000 */
#define CRU_SOFTRST5_CON_OTG_SRSTN_REQ_SHIFT               (13U)
#define CRU_SOFTRST5_CON_OTG_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST5_CON_OTG_SRSTN_REQ_SHIFT)               /* 0x00002000 */
#define CRU_SOFTRST5_CON_OTG_ADP_SRSTN_REQ_SHIFT           (14U)
#define CRU_SOFTRST5_CON_OTG_ADP_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST5_CON_OTG_ADP_SRSTN_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST5_CON_HOST0_HSRSTN_REQ_SHIFT            (15U)
#define CRU_SOFTRST5_CON_HOST0_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_HOST0_HSRSTN_REQ_SHIFT)            /* 0x00008000 */
/* SOFTRST6_CON */
#define CRU_SOFTRST6_CON_HOST0_AUX_HSRSTN_REQ_SHIFT        (0U)
#define CRU_SOFTRST6_CON_HOST0_AUX_HSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST6_CON_HOST0_AUX_HSRSTN_REQ_SHIFT)        /* 0x00000001 */
#define CRU_SOFTRST6_CON_HOST0_ARB_HSRSTN_REQ_SHIFT        (1U)
#define CRU_SOFTRST6_CON_HOST0_ARB_HSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST6_CON_HOST0_ARB_HSRSTN_REQ_SHIFT)        /* 0x00000002 */
#define CRU_SOFTRST6_CON_HOST0_EHCIPHY_SRSTN_REQ_SHIFT     (2U)
#define CRU_SOFTRST6_CON_HOST0_EHCIPHY_SRSTN_REQ_MASK      (0x1U << CRU_SOFTRST6_CON_HOST0_EHCIPHY_SRSTN_REQ_SHIFT)     /* 0x00000004 */
#define CRU_SOFTRST6_CON_HOST0_UTMI_SRSTN_REQ_SHIFT        (3U)
#define CRU_SOFTRST6_CON_HOST0_UTMI_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST6_CON_HOST0_UTMI_SRSTN_REQ_SHIFT)        /* 0x00000008 */
#define CRU_SOFTRST6_CON_USBPOR_SRSTN_REQ_SHIFT            (4U)
#define CRU_SOFTRST6_CON_USBPOR_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST6_CON_USBPOR_SRSTN_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST6_CON_UTIM0_SRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST6_CON_UTIM0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST6_CON_UTIM0_SRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST6_CON_UTMI1_SRSTN_REQ_SHIFT             (6U)
#define CRU_SOFTRST6_CON_UTMI1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST6_CON_UTMI1_SRSTN_REQ_SHIFT)             /* 0x00000040 */
/* SOFTRST7_CON */
#define CRU_SOFTRST7_CON_VIO0_NIU_ASRSTN_REQ_SHIFT         (0U)
#define CRU_SOFTRST7_CON_VIO0_NIU_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST7_CON_VIO0_NIU_ASRSTN_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST7_CON_VIO1_NIU_ASRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST7_CON_VIO1_NIU_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST7_CON_VIO1_NIU_ASRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST7_CON_VIO_NIU_HSRSTN_REQ_SHIFT          (2U)
#define CRU_SOFTRST7_CON_VIO_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST7_CON_VIO_NIU_HSRSTN_REQ_SHIFT)          /* 0x00000004 */
#define CRU_SOFTRST7_CON_VIO_NIU_PSRSTN_REQ_SHIFT          (3U)
#define CRU_SOFTRST7_CON_VIO_NIU_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST7_CON_VIO_NIU_PSRSTN_REQ_SHIFT)          /* 0x00000008 */
#define CRU_SOFTRST7_CON_VOP_ASRSTN_REQ_SHIFT              (4U)
#define CRU_SOFTRST7_CON_VOP_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_VOP_ASRSTN_REQ_SHIFT)              /* 0x00000010 */
#define CRU_SOFTRST7_CON_VOP_HSRSTN_REQ_SHIFT              (5U)
#define CRU_SOFTRST7_CON_VOP_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_VOP_HSRSTN_REQ_SHIFT)              /* 0x00000020 */
#define CRU_SOFTRST7_CON_VOP_DSRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST7_CON_VOP_DSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_VOP_DSRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST7_CON_IEP_ASRSTN_REQ_SHIFT              (7U)
#define CRU_SOFTRST7_CON_IEP_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_IEP_ASRSTN_REQ_SHIFT)              /* 0x00000080 */
#define CRU_SOFTRST7_CON_IEP_HSRSTN_REQ_SHIFT              (8U)
#define CRU_SOFTRST7_CON_IEP_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_IEP_HSRSTN_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST7_CON_RGA_ASRSTN_REQ_SHIFT              (9U)
#define CRU_SOFTRST7_CON_RGA_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_RGA_ASRSTN_REQ_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST7_CON_RGA_HSRSTN_REQ_SHIFT              (10U)
#define CRU_SOFTRST7_CON_RGA_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_RGA_HSRSTN_REQ_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST7_CON_RGA_SRSTN_REQ_SHIFT               (11U)
#define CRU_SOFTRST7_CON_RGA_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST7_CON_RGA_SRSTN_REQ_SHIFT)               /* 0x00000800 */
#define CRU_SOFTRST7_CON_CVBS_PSRSTN_REQ_SHIFT             (12U)
#define CRU_SOFTRST7_CON_CVBS_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST7_CON_CVBS_PSRSTN_REQ_SHIFT)             /* 0x00001000 */
#define CRU_SOFTRST7_CON_HDMI_PSRSTN_REQ_SHIFT             (13U)
#define CRU_SOFTRST7_CON_HDMI_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST7_CON_HDMI_PSRSTN_REQ_SHIFT)             /* 0x00002000 */
#define CRU_SOFTRST7_CON_HDMI_SRSTN_REQ_SHIFT              (14U)
#define CRU_SOFTRST7_CON_HDMI_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST7_CON_HDMI_SRSTN_REQ_SHIFT)              /* 0x00004000 */
#define CRU_SOFTRST7_CON_MIPI_DSI_PSRSTN_REQ_SHIFT         (15U)
#define CRU_SOFTRST7_CON_MIPI_DSI_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST7_CON_MIPI_DSI_PSRSTN_REQ_SHIFT)         /* 0x00008000 */
/* SOFTRST8_CON */
#define CRU_SOFTRST8_CON_ISP_NIU_ASRSTN_REQ_SHIFT          (0U)
#define CRU_SOFTRST8_CON_ISP_NIU_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST8_CON_ISP_NIU_ASRSTN_REQ_SHIFT)          /* 0x00000001 */
#define CRU_SOFTRST8_CON_ISP_NIU_HSRSTN_REQ_SHIFT          (1U)
#define CRU_SOFTRST8_CON_ISP_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST8_CON_ISP_NIU_HSRSTN_REQ_SHIFT)          /* 0x00000002 */
#define CRU_SOFTRST8_CON_ISP_HSRSTN_REQ_SHIFT              (2U)
#define CRU_SOFTRST8_CON_ISP_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST8_CON_ISP_HSRSTN_REQ_SHIFT)              /* 0x00000004 */
#define CRU_SOFTRST8_CON_ISP_SRSTN_REQ_SHIFT               (3U)
#define CRU_SOFTRST8_CON_ISP_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST8_CON_ISP_SRSTN_REQ_SHIFT)               /* 0x00000008 */
#define CRU_SOFTRST8_CON_VIP0_ASRSTN_REQ_SHIFT             (4U)
#define CRU_SOFTRST8_CON_VIP0_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP0_ASRSTN_REQ_SHIFT)             /* 0x00000010 */
#define CRU_SOFTRST8_CON_VIP0_HSRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST8_CON_VIP0_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP0_HSRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST8_CON_VIP0_PSRSTN_REQ_SHIFT             (6U)
#define CRU_SOFTRST8_CON_VIP0_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP0_PSRSTN_REQ_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST8_CON_VIP1_ASRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST8_CON_VIP1_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP1_ASRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST8_CON_VIP1_HSRSTN_REQ_SHIFT             (8U)
#define CRU_SOFTRST8_CON_VIP1_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP1_HSRSTN_REQ_SHIFT)             /* 0x00000100 */
#define CRU_SOFTRST8_CON_VIP1_PSRSTN_REQ_SHIFT             (9U)
#define CRU_SOFTRST8_CON_VIP1_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP1_PSRSTN_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST8_CON_VIP2_ASRSTN_REQ_SHIFT             (10U)
#define CRU_SOFTRST8_CON_VIP2_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP2_ASRSTN_REQ_SHIFT)             /* 0x00000400 */
#define CRU_SOFTRST8_CON_VIP2_HSRSTN_REQ_SHIFT             (11U)
#define CRU_SOFTRST8_CON_VIP2_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP2_HSRSTN_REQ_SHIFT)             /* 0x00000800 */
#define CRU_SOFTRST8_CON_VIP2_PSRSTN_REQ_SHIFT             (12U)
#define CRU_SOFTRST8_CON_VIP2_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP2_PSRSTN_REQ_SHIFT)             /* 0x00001000 */
#define CRU_SOFTRST8_CON_VIP3_ASRSTN_REQ_SHIFT             (13U)
#define CRU_SOFTRST8_CON_VIP3_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP3_ASRSTN_REQ_SHIFT)             /* 0x00002000 */
#define CRU_SOFTRST8_CON_VIP3_HSRSTN_REQ_SHIFT             (14U)
#define CRU_SOFTRST8_CON_VIP3_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP3_HSRSTN_REQ_SHIFT)             /* 0x00004000 */
#define CRU_SOFTRST8_CON_VIP3_PSRSTN_REQ_SHIFT             (15U)
#define CRU_SOFTRST8_CON_VIP3_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST8_CON_VIP3_PSRSTN_REQ_SHIFT)             /* 0x00008000 */
/* SOFTRST9_CON */
#define CRU_SOFTRST9_CON_CIF1TO4_PSRSTN_REQ_SHIFT          (0U)
#define CRU_SOFTRST9_CON_CIF1TO4_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST9_CON_CIF1TO4_PSRSTN_REQ_SHIFT)          /* 0x00000001 */
#define CRU_SOFTRST9_CON_CVBS_CLK_SRSTN_REQ_SHIFT          (1U)
#define CRU_SOFTRST9_CON_CVBS_CLK_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST9_CON_CVBS_CLK_SRSTN_REQ_SHIFT)          /* 0x00000002 */
#define CRU_SOFTRST9_CON_CVBS_HSRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST9_CON_CVBS_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST9_CON_CVBS_HSRSTN_REQ_SHIFT)             /* 0x00000004 */
/* SOFTRST10_CON */
#define CRU_SOFTRST10_CON_VPU_NIU_ASRSTN_REQ_SHIFT         (0U)
#define CRU_SOFTRST10_CON_VPU_NIU_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST10_CON_VPU_NIU_ASRSTN_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST10_CON_VPU_NIU_HSRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST10_CON_VPU_NIU_HSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST10_CON_VPU_NIU_HSRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST10_CON_VPU_ASRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST10_CON_VPU_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST10_CON_VPU_ASRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST10_CON_VPU_HSRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST10_CON_VPU_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST10_CON_VPU_HSRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST10_CON_RKVDEC_NIU_ASRSTN_REQ_SHIFT      (4U)
#define CRU_SOFTRST10_CON_RKVDEC_NIU_ASRSTN_REQ_MASK       (0x1U << CRU_SOFTRST10_CON_RKVDEC_NIU_ASRSTN_REQ_SHIFT)      /* 0x00000010 */
#define CRU_SOFTRST10_CON_RKVDEC_NIU_HSRSTN_REQ_SHIFT      (5U)
#define CRU_SOFTRST10_CON_RKVDEC_NIU_HSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST10_CON_RKVDEC_NIU_HSRSTN_REQ_SHIFT)      /* 0x00000020 */
#define CRU_SOFTRST10_CON_RKVDEC_ASRSTN_REQ_SHIFT          (6U)
#define CRU_SOFTRST10_CON_RKVDEC_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST10_CON_RKVDEC_ASRSTN_REQ_SHIFT)          /* 0x00000040 */
#define CRU_SOFTRST10_CON_RKVDEC_HSRSTN_REQ_SHIFT          (7U)
#define CRU_SOFTRST10_CON_RKVDEC_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST10_CON_RKVDEC_HSRSTN_REQ_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST10_CON_RKVDEC_CABAC_SRSTN_REQ_SHIFT     (8U)
#define CRU_SOFTRST10_CON_RKVDEC_CABAC_SRSTN_REQ_MASK      (0x1U << CRU_SOFTRST10_CON_RKVDEC_CABAC_SRSTN_REQ_SHIFT)     /* 0x00000100 */
#define CRU_SOFTRST10_CON_RKVDEC_CORE_SRSTN_REQ_SHIFT      (9U)
#define CRU_SOFTRST10_CON_RKVDEC_CORE_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST10_CON_RKVDEC_CORE_SRSTN_REQ_SHIFT)      /* 0x00000200 */
#define CRU_SOFTRST10_CON_RKVENC_NIU_ASRSTN_REQ_SHIFT      (10U)
#define CRU_SOFTRST10_CON_RKVENC_NIU_ASRSTN_REQ_MASK       (0x1U << CRU_SOFTRST10_CON_RKVENC_NIU_ASRSTN_REQ_SHIFT)      /* 0x00000400 */
#define CRU_SOFTRST10_CON_RKVENC_NIU_HSRSTN_REQ_SHIFT      (11U)
#define CRU_SOFTRST10_CON_RKVENC_NIU_HSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST10_CON_RKVENC_NIU_HSRSTN_REQ_SHIFT)      /* 0x00000800 */
#define CRU_SOFTRST10_CON_RKVENC_ASRSTN_REQ_SHIFT          (12U)
#define CRU_SOFTRST10_CON_RKVENC_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST10_CON_RKVENC_ASRSTN_REQ_SHIFT)          /* 0x00001000 */
#define CRU_SOFTRST10_CON_RKVENC_HSRSTN_REQ_SHIFT          (13U)
#define CRU_SOFTRST10_CON_RKVENC_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST10_CON_RKVENC_HSRSTN_REQ_SHIFT)          /* 0x00002000 */
#define CRU_SOFTRST10_CON_RKVENC_CORE_SRSTN_REQ_SHIFT      (14U)
#define CRU_SOFTRST10_CON_RKVENC_CORE_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST10_CON_RKVENC_CORE_SRSTN_REQ_SHIFT)      /* 0x00004000 */
/* SOFTRST11_CON */
#define CRU_SOFTRST11_CON_DSP_CORE_SRSTN_REQ_SHIFT         (0U)
#define CRU_SOFTRST11_CON_DSP_CORE_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST11_CON_DSP_CORE_SRSTN_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST11_CON_DSP_SYS_SRSTN_REQ_SHIFT          (1U)
#define CRU_SOFTRST11_CON_DSP_SYS_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST11_CON_DSP_SYS_SRSTN_REQ_SHIFT)          /* 0x00000002 */
#define CRU_SOFTRST11_CON_DSP_GLOBAL_SRSTN_REQ_SHIFT       (2U)
#define CRU_SOFTRST11_CON_DSP_GLOBAL_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST11_CON_DSP_GLOBAL_SRSTN_REQ_SHIFT)       /* 0x00000004 */
#define CRU_SOFTRST11_CON_DSP_OECM_SRSTN_REQ_SHIFT         (3U)
#define CRU_SOFTRST11_CON_DSP_OECM_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST11_CON_DSP_OECM_SRSTN_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST11_CON_DSP_IOP_NIU_PSRSTN_REQ_SHIFT     (4U)
#define CRU_SOFTRST11_CON_DSP_IOP_NIU_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_IOP_NIU_PSRSTN_REQ_SHIFT)     /* 0x00000010 */
#define CRU_SOFTRST11_CON_DSP_EPP_NIU_ASRSTN_REQ_SHIFT     (5U)
#define CRU_SOFTRST11_CON_DSP_EPP_NIU_ASRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_EPP_NIU_ASRSTN_REQ_SHIFT)     /* 0x00000020 */
#define CRU_SOFTRST11_CON_DSP_EDP_NIU_ASRSTN_REQ_SHIFT     (6U)
#define CRU_SOFTRST11_CON_DSP_EDP_NIU_ASRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_EDP_NIU_ASRSTN_REQ_SHIFT)     /* 0x00000040 */
#define CRU_SOFTRST11_CON_DSP_DBG_NIU_PSRSTN_REQ_SHIFT     (7U)
#define CRU_SOFTRST11_CON_DSP_DBG_NIU_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_DBG_NIU_PSRSTN_REQ_SHIFT)     /* 0x00000080 */
#define CRU_SOFTRST11_CON_DSP_CFG_NIU_PSRSTN_REQ_SHIFT     (8U)
#define CRU_SOFTRST11_CON_DSP_CFG_NIU_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_CFG_NIU_PSRSTN_REQ_SHIFT)     /* 0x00000100 */
#define CRU_SOFTRST11_CON_DSP_GRF_PSRSTN_REQ_SHIFT         (9U)
#define CRU_SOFTRST11_CON_DSP_GRF_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST11_CON_DSP_GRF_PSRSTN_REQ_SHIFT)         /* 0x00000200 */
#define CRU_SOFTRST11_CON_DSP_MAILBOX_PSRSTN_REQ_SHIFT     (10U)
#define CRU_SOFTRST11_CON_DSP_MAILBOX_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_MAILBOX_PSRSTN_REQ_SHIFT)     /* 0x00000400 */
#define CRU_SOFTRST11_CON_DSP_INTC_PSRSTN_REQ_SHIFT        (11U)
#define CRU_SOFTRST11_CON_DSP_INTC_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST11_CON_DSP_INTC_PSRSTN_REQ_SHIFT)        /* 0x00000800 */
#define CRU_SOFTRST11_CON_DSP_PFM_MON_PSRSTN_REQ_SHIFT     (13U)
#define CRU_SOFTRST11_CON_DSP_PFM_MON_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST11_CON_DSP_PFM_MON_PSRSTN_REQ_SHIFT)     /* 0x00002000 */
#define CRU_SOFTRST11_CON_DSP_PFM_MON_SRSTN_REQ_SHIFT      (14U)
#define CRU_SOFTRST11_CON_DSP_PFM_MON_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST11_CON_DSP_PFM_MON_SRSTN_REQ_SHIFT)      /* 0x00004000 */
#define CRU_SOFTRST11_CON_DSP_EDAP_NIU_ASRSTN_REQ_SHIFT    (15U)
#define CRU_SOFTRST11_CON_DSP_EDAP_NIU_ASRSTN_REQ_MASK     (0x1U << CRU_SOFTRST11_CON_DSP_EDAP_NIU_ASRSTN_REQ_SHIFT)    /* 0x00008000 */
/* SOFTRST12_CON */
#define CRU_SOFTRST12_CON_PMU_SRSTN_REQ_SHIFT              (0U)
#define CRU_SOFTRST12_CON_PMU_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST12_CON_PMU_SRSTN_REQ_SHIFT)              /* 0x00000001 */
#define CRU_SOFTRST12_CON_PMU_I2C0_SRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST12_CON_PMU_I2C0_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST12_CON_PMU_I2C0_SRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST12_CON_PMU_I2C0_PSRSTN_REQ_SHIFT        (2U)
#define CRU_SOFTRST12_CON_PMU_I2C0_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST12_CON_PMU_I2C0_PSRSTN_REQ_SHIFT)        /* 0x00000004 */
#define CRU_SOFTRST12_CON_PMU_GPIO0_PSRSTN_REQ_SHIFT       (3U)
#define CRU_SOFTRST12_CON_PMU_GPIO0_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST12_CON_PMU_GPIO0_PSRSTN_REQ_SHIFT)       /* 0x00000008 */
#define CRU_SOFTRST12_CON_PMU_INTMEM_PSRSTN_REQ_SHIFT      (4U)
#define CRU_SOFTRST12_CON_PMU_INTMEM_PSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST12_CON_PMU_INTMEM_PSRSTN_REQ_SHIFT)      /* 0x00000010 */
#define CRU_SOFTRST12_CON_PMU_PWM0_PSRSTN_REQ_SHIFT        (5U)
#define CRU_SOFTRST12_CON_PMU_PWM0_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST12_CON_PMU_PWM0_PSRSTN_REQ_SHIFT)        /* 0x00000020 */
#define CRU_SOFTRST12_CON_PMU_PWM0_SRSTN_REQ_SHIFT         (6U)
#define CRU_SOFTRST12_CON_PMU_PWM0_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST12_CON_PMU_PWM0_SRSTN_REQ_SHIFT)         /* 0x00000040 */
#define CRU_SOFTRST12_CON_PMU_GRF_PSRSTN_REQ_SHIFT         (7U)
#define CRU_SOFTRST12_CON_PMU_GRF_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST12_CON_PMU_GRF_PSRSTN_REQ_SHIFT)         /* 0x00000080 */
#define CRU_SOFTRST12_CON_PMU_NIU_SRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST12_CON_PMU_NIU_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST12_CON_PMU_NIU_SRSTN_REQ_SHIFT)          /* 0x00000100 */
#define CRU_SOFTRST12_CON_PMU_PVTM_SRSTN_REQ_SHIFT         (9U)
#define CRU_SOFTRST12_CON_PMU_PVTM_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST12_CON_PMU_PVTM_SRSTN_REQ_SHIFT)         /* 0x00000200 */
#define CRU_SOFTRST12_CON_DSP_EDP_PERF_ARSTN_REQ_SHIFT     (12U)
#define CRU_SOFTRST12_CON_DSP_EDP_PERF_ARSTN_REQ_MASK      (0x1U << CRU_SOFTRST12_CON_DSP_EDP_PERF_ARSTN_REQ_SHIFT)     /* 0x00001000 */
#define CRU_SOFTRST12_CON_DSP_EPP_PERF_ARSTN_REQ_SHIFT     (13U)
#define CRU_SOFTRST12_CON_DSP_EPP_PERF_ARSTN_REQ_MASK      (0x1U << CRU_SOFTRST12_CON_DSP_EPP_PERF_ARSTN_REQ_SHIFT)     /* 0x00002000 */
/* GLB_SRST_FST_VALUE */
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_SHIFT    (0U)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_MASK     (0xFFFFU << CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_SHIFT) /* 0x0000FFFF */
/* GLB_SRST_SND_VALUE */
#define CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_VALUE_SHIFT    (0U)
#define CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_VALUE_MASK     (0xFFFFU << CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_VALUE_SHIFT) /* 0x0000FFFF */
/* GLB_CNT_TH */
#define CRU_GLB_CNT_TH_GLB_RST_CNT_TH_SHIFT                (0U)
#define CRU_GLB_CNT_TH_GLB_RST_CNT_TH_MASK                 (0x3FFU << CRU_GLB_CNT_TH_GLB_RST_CNT_TH_SHIFT)              /* 0x000003FF */
/* MISC_CON */
#define CRU_MISC_CON_CORE_SRST_WFIEN_SHIFT                 (1U)
#define CRU_MISC_CON_CORE_SRST_WFIEN_MASK                  (0x1U << CRU_MISC_CON_CORE_SRST_WFIEN_SHIFT)                 /* 0x00000002 */
#define CRU_MISC_CON_TESTCLK_SEL_SHIFT                     (8U)
#define CRU_MISC_CON_TESTCLK_SEL_MASK                      (0xFU << CRU_MISC_CON_TESTCLK_SEL_SHIFT)                     /* 0x00000F00 */
#define CRU_MISC_CON_HDMIPHY_24M_SEL_SHIFT                 (13U)
#define CRU_MISC_CON_HDMIPHY_24M_SEL_MASK                  (0x1U << CRU_MISC_CON_HDMIPHY_24M_SEL_SHIFT)                 /* 0x00002000 */
#define CRU_MISC_CON_USB480M_24M_SEL_SHIFT                 (15U)
#define CRU_MISC_CON_USB480M_24M_SEL_MASK                  (0x1U << CRU_MISC_CON_USB480M_24M_SEL_SHIFT)                 /* 0x00008000 */
/* GLB_RST_CON */
#define CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT          (0U)
#define CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_MASK           (0x1U << CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT)          /* 0x00000001 */
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT            (1U)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK             (0x1U << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT)            /* 0x00000002 */
#define CRU_GLB_RST_CON_PMU_GLB_SRST_CTRL_SHIFT            (2U)
#define CRU_GLB_RST_CON_PMU_GLB_SRST_CTRL_MASK             (0x3U << CRU_GLB_RST_CON_PMU_GLB_SRST_CTRL_SHIFT)            /* 0x0000000C */
/* GLB_RST_ST */
#define CRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT                (0U)
#define CRU_GLB_RST_ST_FST_GLB_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT)                /* 0x00000001 */
#define CRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT                (1U)
#define CRU_GLB_RST_ST_SND_GLB_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT)                /* 0x00000002 */
#define CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT            (2U)
#define CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_MASK             (0x1U << CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT)            /* 0x00000004 */
#define CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT            (3U)
#define CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_MASK             (0x1U << CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT)            /* 0x00000008 */
#define CRU_GLB_RST_ST_FST_GLB_TSADC_RST_ST_SHIFT          (4U)
#define CRU_GLB_RST_ST_FST_GLB_TSADC_RST_ST_MASK           (0x1U << CRU_GLB_RST_ST_FST_GLB_TSADC_RST_ST_SHIFT)          /* 0x00000010 */
#define CRU_GLB_RST_ST_SND_GLB_TSADC_RST_ST_SHIFT          (5U)
#define CRU_GLB_RST_ST_SND_GLB_TSADC_RST_ST_MASK           (0x1U << CRU_GLB_RST_ST_SND_GLB_TSADC_RST_ST_SHIFT)          /* 0x00000020 */
/* SDMMC_CON0 */
#define CRU_SDMMC_CON0_INIT_STATE_SHIFT                    (0U)
#define CRU_SDMMC_CON0_INIT_STATE_MASK                     (0x1U << CRU_SDMMC_CON0_INIT_STATE_SHIFT)                    /* 0x00000001 */
#define CRU_SDMMC_CON0_DRV_DEGREE_SHIFT                    (1U)
#define CRU_SDMMC_CON0_DRV_DEGREE_MASK                     (0x3U << CRU_SDMMC_CON0_DRV_DEGREE_SHIFT)                    /* 0x00000006 */
#define CRU_SDMMC_CON0_DRV_DELAYNUM_SHIFT                  (3U)
#define CRU_SDMMC_CON0_DRV_DELAYNUM_MASK                   (0xFFU << CRU_SDMMC_CON0_DRV_DELAYNUM_SHIFT)                 /* 0x000007F8 */
#define CRU_SDMMC_CON0_DRV_SEL_SHIFT                       (11U)
#define CRU_SDMMC_CON0_DRV_SEL_MASK                        (0x1U << CRU_SDMMC_CON0_DRV_SEL_SHIFT)                       /* 0x00000800 */
/* SDMMC_CON1 */
#define CRU_SDMMC_CON1_SAMPLE_DEGREE_SHIFT                 (0U)
#define CRU_SDMMC_CON1_SAMPLE_DEGREE_MASK                  (0x3U << CRU_SDMMC_CON1_SAMPLE_DEGREE_SHIFT)                 /* 0x00000003 */
#define CRU_SDMMC_CON1_SAMPLE_DELAYNUM_SHIFT               (2U)
#define CRU_SDMMC_CON1_SAMPLE_DELAYNUM_MASK                (0xFFU << CRU_SDMMC_CON1_SAMPLE_DELAYNUM_SHIFT)              /* 0x000003FC */
#define CRU_SDMMC_CON1_SAMPLE_SEL_SHIFT                    (10U)
#define CRU_SDMMC_CON1_SAMPLE_SEL_MASK                     (0x1U << CRU_SDMMC_CON1_SAMPLE_SEL_SHIFT)                    /* 0x00000400 */
/* SDIO_CON0 */
#define CRU_SDIO_CON0_INIT_STATE_SHIFT                     (0U)
#define CRU_SDIO_CON0_INIT_STATE_MASK                      (0x1U << CRU_SDIO_CON0_INIT_STATE_SHIFT)                     /* 0x00000001 */
#define CRU_SDIO_CON0_DRV_DEGREE_SHIFT                     (1U)
#define CRU_SDIO_CON0_DRV_DEGREE_MASK                      (0x3U << CRU_SDIO_CON0_DRV_DEGREE_SHIFT)                     /* 0x00000006 */
#define CRU_SDIO_CON0_DRV_DELAYNUM_SHIFT                   (3U)
#define CRU_SDIO_CON0_DRV_DELAYNUM_MASK                    (0xFFU << CRU_SDIO_CON0_DRV_DELAYNUM_SHIFT)                  /* 0x000007F8 */
#define CRU_SDIO_CON0_DRV_SEL_SHIFT                        (11U)
#define CRU_SDIO_CON0_DRV_SEL_MASK                         (0x1U << CRU_SDIO_CON0_DRV_SEL_SHIFT)                        /* 0x00000800 */
/* SDIO_CON1 */
#define CRU_SDIO_CON1_SAMPLE_DEGREE_SHIFT                  (0U)
#define CRU_SDIO_CON1_SAMPLE_DEGREE_MASK                   (0x3U << CRU_SDIO_CON1_SAMPLE_DEGREE_SHIFT)                  /* 0x00000003 */
#define CRU_SDIO_CON1_SAMPLE_DELAYNUM_SHIFT                (2U)
#define CRU_SDIO_CON1_SAMPLE_DELAYNUM_MASK                 (0xFFU << CRU_SDIO_CON1_SAMPLE_DELAYNUM_SHIFT)               /* 0x000003FC */
#define CRU_SDIO_CON1_SAMPLE_SEL_SHIFT                     (10U)
#define CRU_SDIO_CON1_SAMPLE_SEL_MASK                      (0x1U << CRU_SDIO_CON1_SAMPLE_SEL_SHIFT)                     /* 0x00000400 */
/* EMMC_CON0 */
#define CRU_EMMC_CON0_INIT_STATE_SHIFT                     (0U)
#define CRU_EMMC_CON0_INIT_STATE_MASK                      (0x1U << CRU_EMMC_CON0_INIT_STATE_SHIFT)                     /* 0x00000001 */
#define CRU_EMMC_CON0_DRV_DEGREE_SHIFT                     (1U)
#define CRU_EMMC_CON0_DRV_DEGREE_MASK                      (0x3U << CRU_EMMC_CON0_DRV_DEGREE_SHIFT)                     /* 0x00000006 */
#define CRU_EMMC_CON0_DRV_DELAYNUM_SHIFT                   (3U)
#define CRU_EMMC_CON0_DRV_DELAYNUM_MASK                    (0xFFU << CRU_EMMC_CON0_DRV_DELAYNUM_SHIFT)                  /* 0x000007F8 */
#define CRU_EMMC_CON0_DRV_SEL_SHIFT                        (11U)
#define CRU_EMMC_CON0_DRV_SEL_MASK                         (0x1U << CRU_EMMC_CON0_DRV_SEL_SHIFT)                        /* 0x00000800 */
/* EMMC_CON1 */
#define CRU_EMMC_CON1_SAMPLE_DEGREE_SHIFT                  (0U)
#define CRU_EMMC_CON1_SAMPLE_DEGREE_MASK                   (0x3U << CRU_EMMC_CON1_SAMPLE_DEGREE_SHIFT)                  /* 0x00000003 */
#define CRU_EMMC_CON1_SAMPLE_DELAYNUM_SHIFT                (2U)
#define CRU_EMMC_CON1_SAMPLE_DELAYNUM_MASK                 (0xFFU << CRU_EMMC_CON1_SAMPLE_DELAYNUM_SHIFT)               /* 0x000003FC */
#define CRU_EMMC_CON1_SAMPLE_SEL_SHIFT                     (10U)
#define CRU_EMMC_CON1_SAMPLE_SEL_MASK                      (0x1U << CRU_EMMC_CON1_SAMPLE_SEL_SHIFT)                     /* 0x00000400 */
/****************************************DDR_PHY*****************************************/
/* REG0 */
#define DDR_PHY_REG0_FIELD0001_SHIFT                       (0U)
#define DDR_PHY_REG0_FIELD0001_MASK                        (0x3U << DDR_PHY_REG0_FIELD0001_SHIFT)                       /* 0x00000003 */
#define DDR_PHY_REG0_FIELD0002_SHIFT                       (2U)
#define DDR_PHY_REG0_FIELD0002_MASK                        (0x1U << DDR_PHY_REG0_FIELD0002_SHIFT)                       /* 0x00000004 */
#define DDR_PHY_REG0_FIELD0003_SHIFT                       (3U)
#define DDR_PHY_REG0_FIELD0003_MASK                        (0x1U << DDR_PHY_REG0_FIELD0003_SHIFT)                       /* 0x00000008 */
#define DDR_PHY_REG0_FIELD0000_SHIFT                       (4U)
#define DDR_PHY_REG0_FIELD0000_MASK                        (0xFU << DDR_PHY_REG0_FIELD0000_SHIFT)                       /* 0x000000F0 */
/* REG1 */
#define DDR_PHY_REG1_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REG1_FIELD0000_MASK                        (0x3U << DDR_PHY_REG1_FIELD0000_SHIFT)                       /* 0x00000003 */
#define DDR_PHY_REG1_FIELD0001_SHIFT                       (2U)
#define DDR_PHY_REG1_FIELD0001_MASK                        (0x3FU << DDR_PHY_REG1_FIELD0001_SHIFT)                      /* 0x000000FC */
/* REG2 */
#define DDR_PHY_REG2_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REG2_FIELD0000_MASK                        (0x1U << DDR_PHY_REG2_FIELD0000_SHIFT)                       /* 0x00000001 */
#define DDR_PHY_REG2_FIELD0001_SHIFT                       (1U)
#define DDR_PHY_REG2_FIELD0001_MASK                        (0x1U << DDR_PHY_REG2_FIELD0001_SHIFT)                       /* 0x00000002 */
#define DDR_PHY_REG2_FIELD0002_SHIFT                       (2U)
#define DDR_PHY_REG2_FIELD0002_MASK                        (0x1U << DDR_PHY_REG2_FIELD0002_SHIFT)                       /* 0x00000004 */
#define DDR_PHY_REG2_FIELD0003_SHIFT                       (3U)
#define DDR_PHY_REG2_FIELD0003_MASK                        (0x1U << DDR_PHY_REG2_FIELD0003_SHIFT)                       /* 0x00000008 */
#define DDR_PHY_REG2_FIELD0004_SHIFT                       (4U)
#define DDR_PHY_REG2_FIELD0004_MASK                        (0x3U << DDR_PHY_REG2_FIELD0004_SHIFT)                       /* 0x00000030 */
#define DDR_PHY_REG2_FIELD0005_SHIFT                       (6U)
#define DDR_PHY_REG2_FIELD0005_MASK                        (0x3U << DDR_PHY_REG2_FIELD0005_SHIFT)                       /* 0x000000C0 */
/* REG3 */
#define DDR_PHY_REG3_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REG3_FIELD0000_MASK                        (0x7U << DDR_PHY_REG3_FIELD0000_SHIFT)                       /* 0x00000007 */
#define DDR_PHY_REG3_FIELD0001_SHIFT                       (4U)
#define DDR_PHY_REG3_FIELD0001_MASK                        (0x7U << DDR_PHY_REG3_FIELD0001_SHIFT)                       /* 0x00000070 */
/* REG5 */
#define DDR_PHY_REG5_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REG5_FIELD0000_MASK                        (0xFFU << DDR_PHY_REG5_FIELD0000_SHIFT)                      /* 0x000000FF */
/* REG6 */
#define DDR_PHY_REG6_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REG6_FIELD0000_MASK                        (0x3FU << DDR_PHY_REG6_FIELD0000_SHIFT)                      /* 0x0000003F */
#define DDR_PHY_REG6_FIELD0001_SHIFT                       (6U)
#define DDR_PHY_REG6_FIELD0001_MASK                        (0x3U << DDR_PHY_REG6_FIELD0001_SHIFT)                       /* 0x000000C0 */
/* REG9 */
#define DDR_PHY_REG9_FIELD0001_SHIFT                       (6U)
#define DDR_PHY_REG9_FIELD0001_MASK                        (0x1U << DDR_PHY_REG9_FIELD0001_SHIFT)                       /* 0x00000040 */
/* REGB */
#define DDR_PHY_REGB_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REGB_FIELD0000_MASK                        (0xFU << DDR_PHY_REGB_FIELD0000_SHIFT)                       /* 0x0000000F */
#define DDR_PHY_REGB_FIELD0001_SHIFT                       (4U)
#define DDR_PHY_REGB_FIELD0001_MASK                        (0xFU << DDR_PHY_REGB_FIELD0001_SHIFT)                       /* 0x000000F0 */
/* REGC */
#define DDR_PHY_REGC_FIELD0000_SHIFT                       (0U)
#define DDR_PHY_REGC_FIELD0000_MASK                        (0xFU << DDR_PHY_REGC_FIELD0000_SHIFT)                       /* 0x0000000F */
/* REG11 */
#define DDR_PHY_REG11_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG11_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG11_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG12 */
#define DDR_PHY_REG12_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG12_FIELD0000_MASK                       (0x1U << DDR_PHY_REG12_FIELD0000_SHIFT)                      /* 0x00000001 */
#define DDR_PHY_REG12_FIELD0001_SHIFT                      (1U)
#define DDR_PHY_REG12_FIELD0001_MASK                       (0x1U << DDR_PHY_REG12_FIELD0001_SHIFT)                      /* 0x00000002 */
#define DDR_PHY_REG12_FIELD0002_SHIFT                      (3U)
#define DDR_PHY_REG12_FIELD0002_MASK                       (0x1FU << DDR_PHY_REG12_FIELD0002_SHIFT)                     /* 0x000000F8 */
/* REG13 */
#define DDR_PHY_REG13_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG13_FIELD0000_MASK                       (0x7U << DDR_PHY_REG13_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG13_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REG13_FIELD0001_MASK                       (0x1U << DDR_PHY_REG13_FIELD0001_SHIFT)                      /* 0x00000008 */
#define DDR_PHY_REG13_FIELD0002_SHIFT                      (4U)
#define DDR_PHY_REG13_FIELD0002_MASK                       (0x1U << DDR_PHY_REG13_FIELD0002_SHIFT)                      /* 0x00000010 */
/* REG14 */
#define DDR_PHY_REG14_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG14_FIELD0000_MASK                       (0x7U << DDR_PHY_REG14_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG14_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REG14_FIELD0001_MASK                       (0x1U << DDR_PHY_REG14_FIELD0001_SHIFT)                      /* 0x00000008 */
/* REG16 */
#define DDR_PHY_REG16_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG16_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG16_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG17 */
#define DDR_PHY_REG17_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG17_FIELD0000_MASK                       (0x7U << DDR_PHY_REG17_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG17_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REG17_FIELD0001_MASK                       (0x7U << DDR_PHY_REG17_FIELD0001_SHIFT)                      /* 0x00000070 */
/* REG18 */
#define DDR_PHY_REG18_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG18_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG18_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG20 */
#define DDR_PHY_REG20_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG20_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG20_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG21 */
#define DDR_PHY_REG21_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG21_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG21_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG26 */
#define DDR_PHY_REG26_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG26_FIELD0000_MASK                       (0x7U << DDR_PHY_REG26_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG26_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REG26_FIELD0001_MASK                       (0x1U << DDR_PHY_REG26_FIELD0001_SHIFT)                      /* 0x00000008 */
#define DDR_PHY_REG26_FIELD0002_SHIFT                      (4U)
#define DDR_PHY_REG26_FIELD0002_MASK                       (0x1U << DDR_PHY_REG26_FIELD0002_SHIFT)                      /* 0x00000010 */
/* REG27 */
#define DDR_PHY_REG27_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG27_FIELD0000_MASK                       (0x7U << DDR_PHY_REG27_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG27_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REG27_FIELD0001_MASK                       (0x1U << DDR_PHY_REG27_FIELD0001_SHIFT)                      /* 0x00000008 */
/* REG28 */
#define DDR_PHY_REG28_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG28_FIELD0000_MASK                       (0x3U << DDR_PHY_REG28_FIELD0000_SHIFT)                      /* 0x00000003 */
/* REG2B */
#define DDR_PHY_REG2B_FIELD0000_SHIFT                      (2U)
#define DDR_PHY_REG2B_FIELD0000_MASK                       (0x7U << DDR_PHY_REG2B_FIELD0000_SHIFT)                      /* 0x0000001C */
#define DDR_PHY_REG2B_FIELD0001_SHIFT                      (5U)
#define DDR_PHY_REG2B_FIELD0001_MASK                       (0x7U << DDR_PHY_REG2B_FIELD0001_SHIFT)                      /* 0x000000E0 */
/* REG2E */
#define DDR_PHY_REG2E_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG2E_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG2E_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REF2F */
#define DDR_PHY_REF2F_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REF2F_FIELD0000_MASK                       (0x1FU << DDR_PHY_REF2F_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG30 */
#define DDR_PHY_REG30_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG30_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG30_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG31 */
#define DDR_PHY_REG31_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG31_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG31_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG36 */
#define DDR_PHY_REG36_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG36_FIELD0000_MASK                       (0x7U << DDR_PHY_REG36_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG36_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REG36_FIELD0001_MASK                       (0x1U << DDR_PHY_REG36_FIELD0001_SHIFT)                      /* 0x00000008 */
#define DDR_PHY_REG36_FIELD0002_SHIFT                      (4U)
#define DDR_PHY_REG36_FIELD0002_MASK                       (0x1U << DDR_PHY_REG36_FIELD0002_SHIFT)                      /* 0x00000010 */
/* REG37 */
#define DDR_PHY_REG37_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG37_FIELD0000_MASK                       (0x7U << DDR_PHY_REG37_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REG37_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REG37_FIELD0001_MASK                       (0x1U << DDR_PHY_REG37_FIELD0001_SHIFT)                      /* 0x00000008 */
/* REG38 */
#define DDR_PHY_REG38_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG38_FIELD0000_MASK                       (0x3U << DDR_PHY_REG38_FIELD0000_SHIFT)                      /* 0x00000003 */
/* REG3B */
#define DDR_PHY_REG3B_FIELD0000_SHIFT                      (2U)
#define DDR_PHY_REG3B_FIELD0000_MASK                       (0x7U << DDR_PHY_REG3B_FIELD0000_SHIFT)                      /* 0x0000001C */
#define DDR_PHY_REG3B_FIELD0001_SHIFT                      (5U)
#define DDR_PHY_REG3B_FIELD0001_MASK                       (0x7U << DDR_PHY_REG3B_FIELD0001_SHIFT)                      /* 0x000000E0 */
/* REG3E */
#define DDR_PHY_REG3E_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG3E_FIELD0000_MASK                       (0x1FU << DDR_PHY_REG3E_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REF3F */
#define DDR_PHY_REF3F_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REF3F_FIELD0000_MASK                       (0x1FU << DDR_PHY_REF3F_FIELD0000_SHIFT)                     /* 0x0000001F */
/* REG70 */
#define DDR_PHY_REG70_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REG70_FIELD0000_MASK                       (0xFU << DDR_PHY_REG70_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REG70_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REG70_FIELD0001_MASK                       (0xFU << DDR_PHY_REG70_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB0 */
#define DDR_PHY_REGB0_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB0_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB0_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB0_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB0_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB0_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB1 */
#define DDR_PHY_REGB1_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB1_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB1_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB1_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB1_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB1_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB2 */
#define DDR_PHY_REGB2_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB2_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB2_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB2_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB2_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB2_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB3 */
#define DDR_PHY_REGB3_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB3_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB3_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB3_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB3_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB3_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB4 */
#define DDR_PHY_REGB4_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB4_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB4_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB4_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB4_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB4_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB5 */
#define DDR_PHY_REGB5_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB5_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB5_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB5_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB5_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB5_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB6 */
#define DDR_PHY_REGB6_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB6_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB6_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB6_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB6_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB6_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB7 */
#define DDR_PHY_REGB7_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB7_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB7_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB7_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB7_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB7_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB8 */
#define DDR_PHY_REGB8_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB8_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB8_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB8_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB8_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB8_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGB9 */
#define DDR_PHY_REGB9_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGB9_FIELD0000_MASK                       (0xFU << DDR_PHY_REGB9_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGB9_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGB9_FIELD0001_MASK                       (0xFU << DDR_PHY_REGB9_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGBA */
#define DDR_PHY_REGBA_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGBA_FIELD0000_MASK                       (0xFU << DDR_PHY_REGBA_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGBA_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGBA_FIELD0001_MASK                       (0xFU << DDR_PHY_REGBA_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGBB */
#define DDR_PHY_REGBB_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGBB_FIELD0000_MASK                       (0xFU << DDR_PHY_REGBB_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGBB_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGBB_FIELD0001_MASK                       (0xFU << DDR_PHY_REGBB_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGBC */
#define DDR_PHY_REGBC_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGBC_FIELD0000_MASK                       (0xFU << DDR_PHY_REGBC_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGBC_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGBC_FIELD0001_MASK                       (0xFU << DDR_PHY_REGBC_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGBD */
#define DDR_PHY_REGBD_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGBD_FIELD0000_MASK                       (0xFU << DDR_PHY_REGBD_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGBD_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGBD_FIELD0001_MASK                       (0xFU << DDR_PHY_REGBD_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGF0 */
#define DDR_PHY_REGF0_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGF0_FIELD0000_MASK                       (0x1U << DDR_PHY_REGF0_FIELD0000_SHIFT)                      /* 0x00000001 */
#define DDR_PHY_REGF0_FIELD0001_SHIFT                      (1U)
#define DDR_PHY_REGF0_FIELD0001_MASK                       (0x1U << DDR_PHY_REGF0_FIELD0001_SHIFT)                      /* 0x00000002 */
/* REGF1 */
#define DDR_PHY_REGF1_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGF1_FIELD0000_MASK                       (0xFU << DDR_PHY_REGF1_FIELD0000_SHIFT)                      /* 0x0000000F */
#define DDR_PHY_REGF1_FIELD0001_SHIFT                      (4U)
#define DDR_PHY_REGF1_FIELD0001_MASK                       (0xFU << DDR_PHY_REGF1_FIELD0001_SHIFT)                      /* 0x000000F0 */
/* REGFA */
#define DDR_PHY_REGFA_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGFA_FIELD0000_MASK                       (0x1U << DDR_PHY_REGFA_FIELD0000_SHIFT)                      /* 0x00000001 */
#define DDR_PHY_REGFA_FIELD0001_SHIFT                      (1U)
#define DDR_PHY_REGFA_FIELD0001_MASK                       (0x1U << DDR_PHY_REGFA_FIELD0001_SHIFT)                      /* 0x00000002 */
/* REGFB */
#define DDR_PHY_REGFB_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGFB_FIELD0000_MASK                       (0x7U << DDR_PHY_REGFB_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REGFB_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REGFB_FIELD0001_MASK                       (0x3U << DDR_PHY_REGFB_FIELD0001_SHIFT)                      /* 0x00000018 */
#define DDR_PHY_REGFB_FIELD0002_SHIFT                      (5U)
#define DDR_PHY_REGFB_FIELD0002_MASK                       (0x7U << DDR_PHY_REGFB_FIELD0002_SHIFT)                      /* 0x000000E0 */
/* REGFC */
#define DDR_PHY_REGFC_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGFC_FIELD0000_MASK                       (0x7U << DDR_PHY_REGFC_FIELD0000_SHIFT)                      /* 0x00000007 */
#define DDR_PHY_REGFC_FIELD0001_SHIFT                      (3U)
#define DDR_PHY_REGFC_FIELD0001_MASK                       (0x3U << DDR_PHY_REGFC_FIELD0001_SHIFT)                      /* 0x00000018 */
#define DDR_PHY_REGFC_FIELD0002_SHIFT                      (5U)
#define DDR_PHY_REGFC_FIELD0002_MASK                       (0x7U << DDR_PHY_REGFC_FIELD0002_SHIFT)                      /* 0x000000E0 */
/* REGFF */
#define DDR_PHY_REGFF_FIELD0000_SHIFT                      (0U)
#define DDR_PHY_REGFF_FIELD0000_MASK                       (0x1U << DDR_PHY_REGFF_FIELD0000_SHIFT)                      /* 0x00000001 */
#define DDR_PHY_REGFF_FIELD0001_SHIFT                      (1U)
#define DDR_PHY_REGFF_FIELD0001_MASK                       (0x1U << DDR_PHY_REGFF_FIELD0001_SHIFT)                      /* 0x00000002 */
/****************************************DDR_PCTL****************************************/
/* SCFG */
#define DDR_PCTL_SCFG_HW_LOW_POWER_EN_SHIFT                (0U)
#define DDR_PCTL_SCFG_HW_LOW_POWER_EN_MASK                 (0x1U << DDR_PCTL_SCFG_HW_LOW_POWER_EN_SHIFT)                /* 0x00000001 */
#define DDR_PCTL_SCFG_NFIFO_NIF1_DIS_SHIFT                 (6U)
#define DDR_PCTL_SCFG_NFIFO_NIF1_DIS_MASK                  (0x1U << DDR_PCTL_SCFG_NFIFO_NIF1_DIS_SHIFT)                 /* 0x00000040 */
#define DDR_PCTL_SCFG_AC_PDD_EN_SHIFT                      (7U)
#define DDR_PCTL_SCFG_AC_PDD_EN_MASK                       (0x1U << DDR_PCTL_SCFG_AC_PDD_EN_SHIFT)                      /* 0x00000080 */
#define DDR_PCTL_SCFG_BBFLAGS_TIMING_SHIFT                 (8U)
#define DDR_PCTL_SCFG_BBFLAGS_TIMING_MASK                  (0xFU << DDR_PCTL_SCFG_BBFLAGS_TIMING_SHIFT)                 /* 0x00000F00 */
/* SCTL */
#define DDR_PCTL_SCTL_STATE_CMD_SHIFT                      (0U)
#define DDR_PCTL_SCTL_STATE_CMD_MASK                       (0x7U << DDR_PCTL_SCTL_STATE_CMD_SHIFT)                      /* 0x00000007 */
/* STAT */
#define DDR_PCTL_STAT_CTL_STAT_SHIFT                       (0U)
#define DDR_PCTL_STAT_CTL_STAT_MASK                        (0x7U << DDR_PCTL_STAT_CTL_STAT_SHIFT)                       /* 0x00000007 */
#define DDR_PCTL_STAT_LP_TRIG_SHIFT                        (4U)
#define DDR_PCTL_STAT_LP_TRIG_MASK                         (0x7U << DDR_PCTL_STAT_LP_TRIG_SHIFT)                        /* 0x00000070 */
/* INTRSTAT */
#define DDR_PCTL_INTRSTAT_ECC_INTR_SHIFT                   (0U)
#define DDR_PCTL_INTRSTAT_ECC_INTR_MASK                    (0x1U << DDR_PCTL_INTRSTAT_ECC_INTR_SHIFT)                   /* 0x00000001 */
#define DDR_PCTL_INTRSTAT_PARITY_INTR_SHIFT                (1U)
#define DDR_PCTL_INTRSTAT_PARITY_INTR_MASK                 (0x1U << DDR_PCTL_INTRSTAT_PARITY_INTR_SHIFT)                /* 0x00000002 */
/* MCMD */
#define DDR_PCTL_MCMD_CMD_OPCODE_SHIFT                     (0U)
#define DDR_PCTL_MCMD_CMD_OPCODE_MASK                      (0xFU << DDR_PCTL_MCMD_CMD_OPCODE_SHIFT)                     /* 0x0000000F */
#define DDR_PCTL_MCMD_CMD_ADDR_SHIFT                       (4U)
#define DDR_PCTL_MCMD_CMD_ADDR_MASK                        (0x1FFFU << DDR_PCTL_MCMD_CMD_ADDR_SHIFT)                    /* 0x0001FFF0 */
#define DDR_PCTL_MCMD_BANK_ADDR_SHIFT                      (17U)
#define DDR_PCTL_MCMD_BANK_ADDR_MASK                       (0x7U << DDR_PCTL_MCMD_BANK_ADDR_SHIFT)                      /* 0x000E0000 */
#define DDR_PCTL_MCMD_RANK_SEL_SHIFT                       (20U)
#define DDR_PCTL_MCMD_RANK_SEL_MASK                        (0xFU << DDR_PCTL_MCMD_RANK_SEL_SHIFT)                       /* 0x00F00000 */
#define DDR_PCTL_MCMD_CMD_ADD_DEL_SHIFT                    (24U)
#define DDR_PCTL_MCMD_CMD_ADD_DEL_MASK                     (0xFU << DDR_PCTL_MCMD_CMD_ADD_DEL_SHIFT)                    /* 0x0F000000 */
#define DDR_PCTL_MCMD_START_CMD_SHIFT                      (31U)
#define DDR_PCTL_MCMD_START_CMD_MASK                       (0x1U << DDR_PCTL_MCMD_START_CMD_SHIFT)                      /* 0x80000000 */
/* POWCTL */
#define DDR_PCTL_POWCTL_POWER_UP_START_SHIFT               (0U)
#define DDR_PCTL_POWCTL_POWER_UP_START_MASK                (0x1U << DDR_PCTL_POWCTL_POWER_UP_START_SHIFT)               /* 0x00000001 */
/* POWSTAT */
#define DDR_PCTL_POWSTAT_POWER_UP_DONE_SHIFT               (0U)
#define DDR_PCTL_POWSTAT_POWER_UP_DONE_MASK                (0x1U << DDR_PCTL_POWSTAT_POWER_UP_DONE_SHIFT)               /* 0x00000001 */
/* CMDTSTAT */
#define DDR_PCTL_CMDTSTAT_CMD_TSTAT_SHIFT                  (0U)
#define DDR_PCTL_CMDTSTAT_CMD_TSTAT_MASK                   (0x1U << DDR_PCTL_CMDTSTAT_CMD_TSTAT_SHIFT)                  /* 0x00000001 */
/* CMDTSTATEN */
#define DDR_PCTL_CMDTSTATEN_CMD_TSTAT_EN_SHIFT             (0U)
#define DDR_PCTL_CMDTSTATEN_CMD_TSTAT_EN_MASK              (0x1U << DDR_PCTL_CMDTSTATEN_CMD_TSTAT_EN_SHIFT)             /* 0x00000001 */
/* MRRCFG0 */
#define DDR_PCTL_MRRCFG0_MRR_BYTE_SEL_SHIFT                (0U)
#define DDR_PCTL_MRRCFG0_MRR_BYTE_SEL_MASK                 (0xFU << DDR_PCTL_MRRCFG0_MRR_BYTE_SEL_SHIFT)                /* 0x0000000F */
/* MRRSTAT0 */
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT0_SHIFT              (0U)
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT0_MASK               (0xFFU << DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT0_SHIFT)             /* 0x000000FF */
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT1_SHIFT              (8U)
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT1_MASK               (0xFFU << DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT1_SHIFT)             /* 0x0000FF00 */
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT2_SHIFT              (16U)
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT2_MASK               (0xFFU << DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT2_SHIFT)             /* 0x00FF0000 */
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT3_SHIFT              (24U)
#define DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT3_MASK               (0xFFU << DDR_PCTL_MRRSTAT0_MRRSTAT_BEAT3_SHIFT)             /* 0xFF000000 */
/* MRRSTAT1 */
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT4_SHIFT              (0U)
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT4_MASK               (0xFFU << DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT4_SHIFT)             /* 0x000000FF */
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT5_SHIFT              (8U)
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT5_MASK               (0xFFU << DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT5_SHIFT)             /* 0x0000FF00 */
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT6_SHIFT              (16U)
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT6_MASK               (0xFFU << DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT6_SHIFT)             /* 0x00FF0000 */
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT7_SHIFT              (24U)
#define DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT7_MASK               (0xFFU << DDR_PCTL_MRRSTAT1_MRRSTAT_BEAT7_SHIFT)             /* 0xFF000000 */
/* MCFG1 */
#define DDR_PCTL_MCFG1_SR_IDLE_SHIFT                       (0U)
#define DDR_PCTL_MCFG1_SR_IDLE_MASK                        (0xFFU << DDR_PCTL_MCFG1_SR_IDLE_SHIFT)                      /* 0x000000FF */
#define DDR_PCTL_MCFG1_TFAW_CFG_OFFSET_SHIFT               (8U)
#define DDR_PCTL_MCFG1_TFAW_CFG_OFFSET_MASK                (0x7U << DDR_PCTL_MCFG1_TFAW_CFG_OFFSET_SHIFT)               /* 0x00000700 */
#define DDR_PCTL_MCFG1_HW_IDLE_SHIFT                       (16U)
#define DDR_PCTL_MCFG1_HW_IDLE_MASK                        (0xFFU << DDR_PCTL_MCFG1_HW_IDLE_SHIFT)                      /* 0x00FF0000 */
#define DDR_PCTL_MCFG1_ZQ_RESISTOR_SHARED_SHIFT            (24U)
#define DDR_PCTL_MCFG1_ZQ_RESISTOR_SHARED_MASK             (0x1U << DDR_PCTL_MCFG1_ZQ_RESISTOR_SHARED_SHIFT)            /* 0x01000000 */
#define DDR_PCTL_MCFG1_HW_EXIT_IDLE_EN_SHIFT               (31U)
#define DDR_PCTL_MCFG1_HW_EXIT_IDLE_EN_MASK                (0x1U << DDR_PCTL_MCFG1_HW_EXIT_IDLE_EN_SHIFT)               /* 0x80000000 */
/* MCFG */
#define DDR_PCTL_MCFG_MEM_BL_SHIFT                         (0U)
#define DDR_PCTL_MCFG_MEM_BL_MASK                          (0x1U << DDR_PCTL_MCFG_MEM_BL_SHIFT)                         /* 0x00000001 */
#define DDR_PCTL_MCFG_CKE_OR_EN_SHIFT                      (1U)
#define DDR_PCTL_MCFG_CKE_OR_EN_MASK                       (0x1U << DDR_PCTL_MCFG_CKE_OR_EN_SHIFT)                      /* 0x00000002 */
#define DDR_PCTL_MCFG_BL8INT_EN_SHIFT                      (2U)
#define DDR_PCTL_MCFG_BL8INT_EN_MASK                       (0x1U << DDR_PCTL_MCFG_BL8INT_EN_SHIFT)                      /* 0x00000004 */
#define DDR_PCTL_MCFG_TWO_T_EN_SHIFT                       (3U)
#define DDR_PCTL_MCFG_TWO_T_EN_MASK                        (0x1U << DDR_PCTL_MCFG_TWO_T_EN_SHIFT)                       /* 0x00000008 */
#define DDR_PCTL_MCFG_STAGGER_CS_SHIFT                     (4U)
#define DDR_PCTL_MCFG_STAGGER_CS_MASK                      (0x1U << DDR_PCTL_MCFG_STAGGER_CS_SHIFT)                     /* 0x00000010 */
#define DDR_PCTL_MCFG_DDR3_EN_SHIFT                        (5U)
#define DDR_PCTL_MCFG_DDR3_EN_MASK                         (0x1U << DDR_PCTL_MCFG_DDR3_EN_SHIFT)                        /* 0x00000020 */
#define DDR_PCTL_MCFG_LPDDR2_S4_SHIFT                      (6U)
#define DDR_PCTL_MCFG_LPDDR2_S4_MASK                       (0x1U << DDR_PCTL_MCFG_LPDDR2_S4_SHIFT)                      /* 0x00000040 */
#define DDR_PCTL_MCFG_MDDR_LPDDR2_BST_EVEN_SHIFT           (7U)
#define DDR_PCTL_MCFG_MDDR_LPDDR2_BST_EVEN_MASK            (0x1U << DDR_PCTL_MCFG_MDDR_LPDDR2_BST_EVEN_SHIFT)           /* 0x00000080 */
#define DDR_PCTL_MCFG_PD_IDLE_SHIFT                        (8U)
#define DDR_PCTL_MCFG_PD_IDLE_MASK                         (0xFFU << DDR_PCTL_MCFG_PD_IDLE_SHIFT)                       /* 0x0000FF00 */
#define DDR_PCTL_MCFG_PD_TYPE_SHIFT                        (16U)
#define DDR_PCTL_MCFG_PD_TYPE_MASK                         (0x1U << DDR_PCTL_MCFG_PD_TYPE_SHIFT)                        /* 0x00010000 */
#define DDR_PCTL_MCFG_PD_EXIT_MODE_SHIFT                   (17U)
#define DDR_PCTL_MCFG_PD_EXIT_MODE_MASK                    (0x1U << DDR_PCTL_MCFG_PD_EXIT_MODE_SHIFT)                   /* 0x00020000 */
#define DDR_PCTL_MCFG_TFAW_CFG_SHIFT                       (18U)
#define DDR_PCTL_MCFG_TFAW_CFG_MASK                        (0x3U << DDR_PCTL_MCFG_TFAW_CFG_SHIFT)                       /* 0x000C0000 */
#define DDR_PCTL_MCFG_MDDR_LPDDR23_BL_SHIFT                (20U)
#define DDR_PCTL_MCFG_MDDR_LPDDR23_BL_MASK                 (0x3U << DDR_PCTL_MCFG_MDDR_LPDDR23_BL_SHIFT)                /* 0x00300000 */
#define DDR_PCTL_MCFG_MDDR_LPDDR23_EN_SHIFT                (22U)
#define DDR_PCTL_MCFG_MDDR_LPDDR23_EN_MASK                 (0x3U << DDR_PCTL_MCFG_MDDR_LPDDR23_EN_SHIFT)                /* 0x00C00000 */
#define DDR_PCTL_MCFG_MDDR_LPDDR23_CLOCK_STOP_IDLE_SHIFT   (24U)
#define DDR_PCTL_MCFG_MDDR_LPDDR23_CLOCK_STOP_IDLE_MASK    (0xFFU << DDR_PCTL_MCFG_MDDR_LPDDR23_CLOCK_STOP_IDLE_SHIFT)  /* 0xFF000000 */
/* PPCFG */
#define DDR_PCTL_PPCFG_PPMEM_EN_SHIFT                      (0U)
#define DDR_PCTL_PPCFG_PPMEM_EN_MASK                       (0x1U << DDR_PCTL_PPCFG_PPMEM_EN_SHIFT)                      /* 0x00000001 */
#define DDR_PCTL_PPCFG_RPMEM_DIS_SHIFT                     (1U)
#define DDR_PCTL_PPCFG_RPMEM_DIS_MASK                      (0xFFU << DDR_PCTL_PPCFG_RPMEM_DIS_SHIFT)                    /* 0x000001FE */
/* MSTAT */
#define DDR_PCTL_MSTAT_POWER_DOWN_SHIFT                    (0U)
#define DDR_PCTL_MSTAT_POWER_DOWN_MASK                     (0x1U << DDR_PCTL_MSTAT_POWER_DOWN_SHIFT)                    /* 0x00000001 */
#define DDR_PCTL_MSTAT_CLOCK_STOP_SHIFT                    (1U)
#define DDR_PCTL_MSTAT_CLOCK_STOP_MASK                     (0x1U << DDR_PCTL_MSTAT_CLOCK_STOP_SHIFT)                    /* 0x00000002 */
#define DDR_PCTL_MSTAT_SELF_REFRESH_SHIFT                  (2U)
#define DDR_PCTL_MSTAT_SELF_REFRESH_MASK                   (0x1U << DDR_PCTL_MSTAT_SELF_REFRESH_SHIFT)                  /* 0x00000004 */
/* LPDDR2ZQCFG */
#define DDR_PCTL_LPDDR2ZQCFG_ZQCS_MA_SHIFT                 (0U)
#define DDR_PCTL_LPDDR2ZQCFG_ZQCS_MA_MASK                  (0xFFU << DDR_PCTL_LPDDR2ZQCFG_ZQCS_MA_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_LPDDR2ZQCFG_ZQCS_OP_SHIFT                 (8U)
#define DDR_PCTL_LPDDR2ZQCFG_ZQCS_OP_MASK                  (0xFFU << DDR_PCTL_LPDDR2ZQCFG_ZQCS_OP_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_LPDDR2ZQCFG_ZQCL_MA_SHIFT                 (16U)
#define DDR_PCTL_LPDDR2ZQCFG_ZQCL_MA_MASK                  (0xFFU << DDR_PCTL_LPDDR2ZQCFG_ZQCL_MA_SHIFT)                /* 0x00FF0000 */
#define DDR_PCTL_LPDDR2ZQCFG_ZQCL_OP_SHIFT                 (24U)
#define DDR_PCTL_LPDDR2ZQCFG_ZQCL_OP_MASK                  (0xFFU << DDR_PCTL_LPDDR2ZQCFG_ZQCL_OP_SHIFT)                /* 0xFF000000 */
/* DTUPDES */
#define DDR_PCTL_DTUPDES_DTU_ERR_B0_SHIFT                  (0U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B0_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B0_SHIFT)                  /* 0x00000001 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B1_SHIFT                  (1U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B1_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B1_SHIFT)                  /* 0x00000002 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B2_SHIFT                  (2U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B2_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B2_SHIFT)                  /* 0x00000004 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B3_SHIFT                  (3U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B3_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B3_SHIFT)                  /* 0x00000008 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B4_SHIFT                  (4U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B4_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B4_SHIFT)                  /* 0x00000010 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B5_SHIFT                  (5U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B5_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B5_SHIFT)                  /* 0x00000020 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B6_SHIFT                  (6U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B6_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B6_SHIFT)                  /* 0x00000040 */
#define DDR_PCTL_DTUPDES_DTU_ERR_B7_SHIFT                  (7U)
#define DDR_PCTL_DTUPDES_DTU_ERR_B7_MASK                   (0x1U << DDR_PCTL_DTUPDES_DTU_ERR_B7_SHIFT)                  /* 0x00000080 */
#define DDR_PCTL_DTUPDES_DTU_RANDOM_ERROR_SHIFT            (8U)
#define DDR_PCTL_DTUPDES_DTU_RANDOM_ERROR_MASK             (0x1U << DDR_PCTL_DTUPDES_DTU_RANDOM_ERROR_SHIFT)            /* 0x00000100 */
#define DDR_PCTL_DTUPDES_DTU_EAFFL_SHIFT                   (9U)
#define DDR_PCTL_DTUPDES_DTU_EAFFL_MASK                    (0xFU << DDR_PCTL_DTUPDES_DTU_EAFFL_SHIFT)                   /* 0x00001E00 */
#define DDR_PCTL_DTUPDES_DTU_RD_MISSING_SHIFT              (13U)
#define DDR_PCTL_DTUPDES_DTU_RD_MISSING_MASK               (0x1U << DDR_PCTL_DTUPDES_DTU_RD_MISSING_SHIFT)              /* 0x00002000 */
/* DTUNA */
#define DDR_PCTL_DTUNA_DTU_NUM_ADDRESS_SHIFT               (0U)
#define DDR_PCTL_DTUNA_DTU_NUM_ADDRESS_MASK                (0xFFFFFFFFU << DDR_PCTL_DTUNA_DTU_NUM_ADDRESS_SHIFT)        /* 0xFFFFFFFF */
/* DTUNE */
#define DDR_PCTL_DTUNE_DTU_NUM_ERRORS_SHIFT                (0U)
#define DDR_PCTL_DTUNE_DTU_NUM_ERRORS_MASK                 (0xFFFFFFFFU << DDR_PCTL_DTUNE_DTU_NUM_ERRORS_SHIFT)         /* 0xFFFFFFFF */
/* DTUPRD0 */
#define DDR_PCTL_DTUPRD0_DTU_ALLBITS_0_SHIFT               (0U)
#define DDR_PCTL_DTUPRD0_DTU_ALLBITS_0_MASK                (0xFFFFU << DDR_PCTL_DTUPRD0_DTU_ALLBITS_0_SHIFT)            /* 0x0000FFFF */
#define DDR_PCTL_DTUPRD0_DTU_ALLBITS_1_SHIFT               (16U)
#define DDR_PCTL_DTUPRD0_DTU_ALLBITS_1_MASK                (0xFFFFU << DDR_PCTL_DTUPRD0_DTU_ALLBITS_1_SHIFT)            /* 0xFFFF0000 */
/* DTUPRD1 */
#define DDR_PCTL_DTUPRD1_DTU_ALLBITS_2_SHIFT               (0U)
#define DDR_PCTL_DTUPRD1_DTU_ALLBITS_2_MASK                (0xFFFFU << DDR_PCTL_DTUPRD1_DTU_ALLBITS_2_SHIFT)            /* 0x0000FFFF */
#define DDR_PCTL_DTUPRD1_DTU_ALLBITS_3_SHIFT               (16U)
#define DDR_PCTL_DTUPRD1_DTU_ALLBITS_3_MASK                (0xFFFFU << DDR_PCTL_DTUPRD1_DTU_ALLBITS_3_SHIFT)            /* 0xFFFF0000 */
/* DTUPRD2 */
#define DDR_PCTL_DTUPRD2_DTU_ALLBITS_4_SHIFT               (0U)
#define DDR_PCTL_DTUPRD2_DTU_ALLBITS_4_MASK                (0xFFFFU << DDR_PCTL_DTUPRD2_DTU_ALLBITS_4_SHIFT)            /* 0x0000FFFF */
#define DDR_PCTL_DTUPRD2_DTU_ALLBITS_5_SHIFT               (16U)
#define DDR_PCTL_DTUPRD2_DTU_ALLBITS_5_MASK                (0xFFFFU << DDR_PCTL_DTUPRD2_DTU_ALLBITS_5_SHIFT)            /* 0xFFFF0000 */
/* DTUPRD3 */
#define DDR_PCTL_DTUPRD3_DTU_ALLBITS_6_SHIFT               (0U)
#define DDR_PCTL_DTUPRD3_DTU_ALLBITS_6_MASK                (0xFFFFU << DDR_PCTL_DTUPRD3_DTU_ALLBITS_6_SHIFT)            /* 0x0000FFFF */
#define DDR_PCTL_DTUPRD3_DTU_ALLBITS_7_SHIFT               (16U)
#define DDR_PCTL_DTUPRD3_DTU_ALLBITS_7_MASK                (0xFFFFU << DDR_PCTL_DTUPRD3_DTU_ALLBITS_7_SHIFT)            /* 0xFFFF0000 */
/* DTUAWDT */
#define DDR_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_SHIFT           (0U)
#define DDR_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_MASK            (0x3U << DDR_PCTL_DTUAWDT_COLUMN_ADDR_WIDTH_SHIFT)           /* 0x00000003 */
#define DDR_PCTL_DTUAWDT_BANK_ADDR_WIDTH_SHIFT             (3U)
#define DDR_PCTL_DTUAWDT_BANK_ADDR_WIDTH_MASK              (0x3U << DDR_PCTL_DTUAWDT_BANK_ADDR_WIDTH_SHIFT)             /* 0x00000018 */
#define DDR_PCTL_DTUAWDT_ROW_ADDR_WIDTH_SHIFT              (6U)
#define DDR_PCTL_DTUAWDT_ROW_ADDR_WIDTH_MASK               (0x3U << DDR_PCTL_DTUAWDT_ROW_ADDR_WIDTH_SHIFT)              /* 0x000000C0 */
#define DDR_PCTL_DTUAWDT_NUMBER_RANKS_SHIFT                (9U)
#define DDR_PCTL_DTUAWDT_NUMBER_RANKS_MASK                 (0x3U << DDR_PCTL_DTUAWDT_NUMBER_RANKS_SHIFT)                /* 0x00000600 */
/* TOGCNT1U */
#define DDR_PCTL_TOGCNT1U_TOGGLE_COUNTER_1U_SHIFT          (0U)
#define DDR_PCTL_TOGCNT1U_TOGGLE_COUNTER_1U_MASK           (0x3FFU << DDR_PCTL_TOGCNT1U_TOGGLE_COUNTER_1U_SHIFT)        /* 0x000003FF */
/* TINIT */
#define DDR_PCTL_TINIT_T_INIT_SHIFT                        (0U)
#define DDR_PCTL_TINIT_T_INIT_MASK                         (0x1FFU << DDR_PCTL_TINIT_T_INIT_SHIFT)                      /* 0x000001FF */
/* TRSTH */
#define DDR_PCTL_TRSTH_T_RSTH_SHIFT                        (0U)
#define DDR_PCTL_TRSTH_T_RSTH_MASK                         (0x3FFU << DDR_PCTL_TRSTH_T_RSTH_SHIFT)                      /* 0x000003FF */
/* TOGCNT100N */
#define DDR_PCTL_TOGCNT100N_TOGGLE_COUNTER_100N_SHIFT      (0U)
#define DDR_PCTL_TOGCNT100N_TOGGLE_COUNTER_100N_MASK       (0x7FU << DDR_PCTL_TOGCNT100N_TOGGLE_COUNTER_100N_SHIFT)     /* 0x0000007F */
/* TREFI */
#define DDR_PCTL_TREFI_T_REFI_SHIFT                        (0U)
#define DDR_PCTL_TREFI_T_REFI_MASK                         (0x1FFFU << DDR_PCTL_TREFI_T_REFI_SHIFT)                     /* 0x00001FFF */
#define DDR_PCTL_TREFI_NUM_ADD_REF_SHIFT                   (16U)
#define DDR_PCTL_TREFI_NUM_ADD_REF_MASK                    (0x7U << DDR_PCTL_TREFI_NUM_ADD_REF_SHIFT)                   /* 0x00070000 */
#define DDR_PCTL_TREFI_UPD_REF_SHIFT                       (31U)
#define DDR_PCTL_TREFI_UPD_REF_MASK                        (0x1U << DDR_PCTL_TREFI_UPD_REF_SHIFT)                       /* 0x80000000 */
/* TMRD */
#define DDR_PCTL_TMRD_T_MRD_SHIFT                          (0U)
#define DDR_PCTL_TMRD_T_MRD_MASK                           (0x7U << DDR_PCTL_TMRD_T_MRD_SHIFT)                          /* 0x00000007 */
/* TRFC */
#define DDR_PCTL_TRFC_T_RFC_SHIFT                          (0U)
#define DDR_PCTL_TRFC_T_RFC_MASK                           (0x1FFU << DDR_PCTL_TRFC_T_RFC_SHIFT)                        /* 0x000001FF */
/* TRP */
#define DDR_PCTL_TRP_T_RP_SHIFT                            (0U)
#define DDR_PCTL_TRP_T_RP_MASK                             (0x1FU << DDR_PCTL_TRP_T_RP_SHIFT)                           /* 0x0000001F */
#define DDR_PCTL_TRP_PREA_EXTRA_SHIFT                      (16U)
#define DDR_PCTL_TRP_PREA_EXTRA_MASK                       (0x3U << DDR_PCTL_TRP_PREA_EXTRA_SHIFT)                      /* 0x00030000 */
/* TRTW */
#define DDR_PCTL_TRTW_T_RTW_SHIFT                          (0U)
#define DDR_PCTL_TRTW_T_RTW_MASK                           (0xFU << DDR_PCTL_TRTW_T_RTW_SHIFT)                          /* 0x0000000F */
/* TAL */
#define DDR_PCTL_TAL_T_AL_SHIFT                            (0U)
#define DDR_PCTL_TAL_T_AL_MASK                             (0xFU << DDR_PCTL_TAL_T_AL_SHIFT)                            /* 0x0000000F */
/* TCL */
#define DDR_PCTL_TCL_T_CL_SHIFT                            (0U)
#define DDR_PCTL_TCL_T_CL_MASK                             (0xFU << DDR_PCTL_TCL_T_CL_SHIFT)                            /* 0x0000000F */
/* TCWL */
#define DDR_PCTL_TCWL_T_CWL_SHIFT                          (0U)
#define DDR_PCTL_TCWL_T_CWL_MASK                           (0xFU << DDR_PCTL_TCWL_T_CWL_SHIFT)                          /* 0x0000000F */
/* TRAS */
#define DDR_PCTL_TRAS_T_RAS_SHIFT                          (0U)
#define DDR_PCTL_TRAS_T_RAS_MASK                           (0x3FU << DDR_PCTL_TRAS_T_RAS_SHIFT)                         /* 0x0000003F */
/* TRC */
#define DDR_PCTL_TRC_T_RC_SHIFT                            (0U)
#define DDR_PCTL_TRC_T_RC_MASK                             (0x3FU << DDR_PCTL_TRC_T_RC_SHIFT)                           /* 0x0000003F */
/* TRCD */
#define DDR_PCTL_TRCD_T_RCD_SHIFT                          (0U)
#define DDR_PCTL_TRCD_T_RCD_MASK                           (0x1FU << DDR_PCTL_TRCD_T_RCD_SHIFT)                         /* 0x0000001F */
/* TRRD */
#define DDR_PCTL_TRRD_T_RRD_SHIFT                          (0U)
#define DDR_PCTL_TRRD_T_RRD_MASK                           (0xFU << DDR_PCTL_TRRD_T_RRD_SHIFT)                          /* 0x0000000F */
/* TRTP */
#define DDR_PCTL_TRTP_T_RTP_SHIFT                          (0U)
#define DDR_PCTL_TRTP_T_RTP_MASK                           (0xFU << DDR_PCTL_TRTP_T_RTP_SHIFT)                          /* 0x0000000F */
/* TWR */
#define DDR_PCTL_TWR_T_WR_SHIFT                            (0U)
#define DDR_PCTL_TWR_T_WR_MASK                             (0x1FU << DDR_PCTL_TWR_T_WR_SHIFT)                           /* 0x0000001F */
/* TWTR */
#define DDR_PCTL_TWTR_T_WTR_SHIFT                          (0U)
#define DDR_PCTL_TWTR_T_WTR_MASK                           (0xFU << DDR_PCTL_TWTR_T_WTR_SHIFT)                          /* 0x0000000F */
/* TEXSR */
#define DDR_PCTL_TEXSR_T_EXSR_SHIFT                        (0U)
#define DDR_PCTL_TEXSR_T_EXSR_MASK                         (0x3FFU << DDR_PCTL_TEXSR_T_EXSR_SHIFT)                      /* 0x000003FF */
/* TXP */
#define DDR_PCTL_TXP_T_XP_SHIFT                            (0U)
#define DDR_PCTL_TXP_T_XP_MASK                             (0x7U << DDR_PCTL_TXP_T_XP_SHIFT)                            /* 0x00000007 */
/* TXPDLL */
#define DDR_PCTL_TXPDLL_T_XPDLL_SHIFT                      (0U)
#define DDR_PCTL_TXPDLL_T_XPDLL_MASK                       (0x3FU << DDR_PCTL_TXPDLL_T_XPDLL_SHIFT)                     /* 0x0000003F */
/* TZQCS */
#define DDR_PCTL_TZQCS_T_ZQCS_SHIFT                        (0U)
#define DDR_PCTL_TZQCS_T_ZQCS_MASK                         (0x7FU << DDR_PCTL_TZQCS_T_ZQCS_SHIFT)                       /* 0x0000007F */
/* TZQCSI */
#define DDR_PCTL_TZQCSI_T_ZQCSI_SHIFT                      (0U)
#define DDR_PCTL_TZQCSI_T_ZQCSI_MASK                       (0xFFFFFFFFU << DDR_PCTL_TZQCSI_T_ZQCSI_SHIFT)               /* 0xFFFFFFFF */
/* TDQS */
#define DDR_PCTL_TDQS_T_DQS_SHIFT                          (0U)
#define DDR_PCTL_TDQS_T_DQS_MASK                           (0xFU << DDR_PCTL_TDQS_T_DQS_SHIFT)                          /* 0x0000000F */
/* TCKSRE */
#define DDR_PCTL_TCKSRE_T_CKSRE_SHIFT                      (0U)
#define DDR_PCTL_TCKSRE_T_CKSRE_MASK                       (0x1FU << DDR_PCTL_TCKSRE_T_CKSRE_SHIFT)                     /* 0x0000001F */
/* TCKSRX */
#define DDR_PCTL_TCKSRX_T_CKSRX_SHIFT                      (0U)
#define DDR_PCTL_TCKSRX_T_CKSRX_MASK                       (0x1FU << DDR_PCTL_TCKSRX_T_CKSRX_SHIFT)                     /* 0x0000001F */
/* TCKE */
#define DDR_PCTL_TCKE_T_CKE_SHIFT                          (0U)
#define DDR_PCTL_TCKE_T_CKE_MASK                           (0x7U << DDR_PCTL_TCKE_T_CKE_SHIFT)                          /* 0x00000007 */
/* TMOD */
#define DDR_PCTL_TMOD_T_MOD_SHIFT                          (0U)
#define DDR_PCTL_TMOD_T_MOD_MASK                           (0x1FU << DDR_PCTL_TMOD_T_MOD_SHIFT)                         /* 0x0000001F */
/* TRSTL */
#define DDR_PCTL_TRSTL_T_RSTL_SHIFT                        (0U)
#define DDR_PCTL_TRSTL_T_RSTL_MASK                         (0x7FU << DDR_PCTL_TRSTL_T_RSTL_SHIFT)                       /* 0x0000007F */
/* TZQCL */
#define DDR_PCTL_TZQCL_T_ZQCL_SHIFT                        (0U)
#define DDR_PCTL_TZQCL_T_ZQCL_MASK                         (0x3FFU << DDR_PCTL_TZQCL_T_ZQCL_SHIFT)                      /* 0x000003FF */
/* TMRR */
#define DDR_PCTL_TMRR_T_MRR_SHIFT                          (0U)
#define DDR_PCTL_TMRR_T_MRR_MASK                           (0xFFU << DDR_PCTL_TMRR_T_MRR_SHIFT)                         /* 0x000000FF */
/* TCKESR */
#define DDR_PCTL_TCKESR_T_CKESR_SHIFT                      (0U)
#define DDR_PCTL_TCKESR_T_CKESR_MASK                       (0xFU << DDR_PCTL_TCKESR_T_CKESR_SHIFT)                      /* 0x0000000F */
/* TDPD */
#define DDR_PCTL_TDPD_T_DPD_SHIFT                          (0U)
#define DDR_PCTL_TDPD_T_DPD_MASK                           (0x3FFU << DDR_PCTL_TDPD_T_DPD_SHIFT)                        /* 0x000003FF */
/* TREFI_MEM_DDR3 */
#define DDR_PCTL_TREFI_MEM_DDR3_T_REFI_MEM_DDR3_SHIFT      (0U)
#define DDR_PCTL_TREFI_MEM_DDR3_T_REFI_MEM_DDR3_MASK       (0x7FFFU << DDR_PCTL_TREFI_MEM_DDR3_T_REFI_MEM_DDR3_SHIFT)   /* 0x00007FFF */
/* DTUWACTL */
#define DDR_PCTL_DTUWACTL_DTU_WR_COL_SHIFT                 (0U)
#define DDR_PCTL_DTUWACTL_DTU_WR_COL_MASK                  (0x3FFU << DDR_PCTL_DTUWACTL_DTU_WR_COL_SHIFT)               /* 0x000003FF */
#define DDR_PCTL_DTUWACTL_DTU_WR_BANK_SHIFT                (10U)
#define DDR_PCTL_DTUWACTL_DTU_WR_BANK_MASK                 (0x7U << DDR_PCTL_DTUWACTL_DTU_WR_BANK_SHIFT)                /* 0x00001C00 */
#define DDR_PCTL_DTUWACTL_DTU_WR_ROW_SHIFT                 (13U)
#define DDR_PCTL_DTUWACTL_DTU_WR_ROW_MASK                  (0xFFFFU << DDR_PCTL_DTUWACTL_DTU_WR_ROW_SHIFT)              /* 0x1FFFE000 */
#define DDR_PCTL_DTUWACTL_DTU_WR_RANK_SHIFT                (30U)
#define DDR_PCTL_DTUWACTL_DTU_WR_RANK_MASK                 (0x3U << DDR_PCTL_DTUWACTL_DTU_WR_RANK_SHIFT)                /* 0xC0000000 */
/* DTURACTL */
#define DDR_PCTL_DTURACTL_DTU_RD_COL_SHIFT                 (0U)
#define DDR_PCTL_DTURACTL_DTU_RD_COL_MASK                  (0x3FFU << DDR_PCTL_DTURACTL_DTU_RD_COL_SHIFT)               /* 0x000003FF */
#define DDR_PCTL_DTURACTL_DTU_RD_BANK_SHIFT                (10U)
#define DDR_PCTL_DTURACTL_DTU_RD_BANK_MASK                 (0x7U << DDR_PCTL_DTURACTL_DTU_RD_BANK_SHIFT)                /* 0x00001C00 */
#define DDR_PCTL_DTURACTL_DTU_RD_ROW_SHIFT                 (13U)
#define DDR_PCTL_DTURACTL_DTU_RD_ROW_MASK                  (0xFFFFU << DDR_PCTL_DTURACTL_DTU_RD_ROW_SHIFT)              /* 0x1FFFE000 */
#define DDR_PCTL_DTURACTL_DTU_RD_RANK_SHIFT                (30U)
#define DDR_PCTL_DTURACTL_DTU_RD_RANK_MASK                 (0x3U << DDR_PCTL_DTURACTL_DTU_RD_RANK_SHIFT)                /* 0xC0000000 */
/* DTUCFG */
#define DDR_PCTL_DTUCFG_DTU_ENABLE_SHIFT                   (0U)
#define DDR_PCTL_DTUCFG_DTU_ENABLE_MASK                    (0x1U << DDR_PCTL_DTUCFG_DTU_ENABLE_SHIFT)                   /* 0x00000001 */
#define DDR_PCTL_DTUCFG_DTU_NALEN_SHIFT                    (1U)
#define DDR_PCTL_DTUCFG_DTU_NALEN_MASK                     (0x3FU << DDR_PCTL_DTUCFG_DTU_NALEN_SHIFT)                   /* 0x0000007E */
#define DDR_PCTL_DTUCFG_DTU_INCR_COLS_SHIFT                (7U)
#define DDR_PCTL_DTUCFG_DTU_INCR_COLS_MASK                 (0x1U << DDR_PCTL_DTUCFG_DTU_INCR_COLS_SHIFT)                /* 0x00000080 */
#define DDR_PCTL_DTUCFG_DTU_INCR_BANKS_SHIFT               (8U)
#define DDR_PCTL_DTUCFG_DTU_INCR_BANKS_MASK                (0x1U << DDR_PCTL_DTUCFG_DTU_INCR_BANKS_SHIFT)               /* 0x00000100 */
#define DDR_PCTL_DTUCFG_DTU_GENERATE_RANDOM_SHIFT          (9U)
#define DDR_PCTL_DTUCFG_DTU_GENERATE_RANDOM_MASK           (0x1U << DDR_PCTL_DTUCFG_DTU_GENERATE_RANDOM_SHIFT)          /* 0x00000200 */
#define DDR_PCTL_DTUCFG_DTU_TARGET_LANE_SHIFT              (10U)
#define DDR_PCTL_DTUCFG_DTU_TARGET_LANE_MASK               (0xFU << DDR_PCTL_DTUCFG_DTU_TARGET_LANE_SHIFT)              /* 0x00003C00 */
#define DDR_PCTL_DTUCFG_DTU_DATA_MASK_EN_SHIFT             (14U)
#define DDR_PCTL_DTUCFG_DTU_DATA_MASK_EN_MASK              (0x1U << DDR_PCTL_DTUCFG_DTU_DATA_MASK_EN_SHIFT)             /* 0x00004000 */
#define DDR_PCTL_DTUCFG_DTU_WR_MULTI_RD_SHIFT              (15U)
#define DDR_PCTL_DTUCFG_DTU_WR_MULTI_RD_MASK               (0x1U << DDR_PCTL_DTUCFG_DTU_WR_MULTI_RD_SHIFT)              /* 0x00008000 */
#define DDR_PCTL_DTUCFG_DTU_ROW_INCREMENTS_SHIFT           (16U)
#define DDR_PCTL_DTUCFG_DTU_ROW_INCREMENTS_MASK            (0x7FU << DDR_PCTL_DTUCFG_DTU_ROW_INCREMENTS_SHIFT)          /* 0x007F0000 */
/* DTUECTL */
#define DDR_PCTL_DTUECTL_RUN_DTU_SHIFT                     (0U)
#define DDR_PCTL_DTUECTL_RUN_DTU_MASK                      (0x1U << DDR_PCTL_DTUECTL_RUN_DTU_SHIFT)                     /* 0x00000001 */
#define DDR_PCTL_DTUECTL_RUN_ERROR_REPORTS_SHIFT           (1U)
#define DDR_PCTL_DTUECTL_RUN_ERROR_REPORTS_MASK            (0x1U << DDR_PCTL_DTUECTL_RUN_ERROR_REPORTS_SHIFT)           /* 0x00000002 */
#define DDR_PCTL_DTUECTL_WR_MULTI_RD_RST_SHIFT             (2U)
#define DDR_PCTL_DTUECTL_WR_MULTI_RD_RST_MASK              (0x1U << DDR_PCTL_DTUECTL_WR_MULTI_RD_RST_SHIFT)             /* 0x00000004 */
/* DTUWD0 */
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE0_SHIFT                 (0U)
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE0_MASK                  (0xFFU << DDR_PCTL_DTUWD0_DTU_WR_BYTE0_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE1_SHIFT                 (8U)
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE1_MASK                  (0xFFU << DDR_PCTL_DTUWD0_DTU_WR_BYTE1_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE2_SHIFT                 (16U)
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE2_MASK                  (0xFFU << DDR_PCTL_DTUWD0_DTU_WR_BYTE2_SHIFT)                /* 0x00FF0000 */
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE3_SHIFT                 (24U)
#define DDR_PCTL_DTUWD0_DTU_WR_BYTE3_MASK                  (0xFFU << DDR_PCTL_DTUWD0_DTU_WR_BYTE3_SHIFT)                /* 0xFF000000 */
/* DTUWD1 */
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE4_SHIFT                 (0U)
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE4_MASK                  (0xFFU << DDR_PCTL_DTUWD1_DTU_WR_BYTE4_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE5_SHIFT                 (8U)
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE5_MASK                  (0xFFU << DDR_PCTL_DTUWD1_DTU_WR_BYTE5_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE6_SHIFT                 (16U)
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE6_MASK                  (0xFFU << DDR_PCTL_DTUWD1_DTU_WR_BYTE6_SHIFT)                /* 0x00FF0000 */
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE7_SHIFT                 (24U)
#define DDR_PCTL_DTUWD1_DTU_WR_BYTE7_MASK                  (0xFFU << DDR_PCTL_DTUWD1_DTU_WR_BYTE7_SHIFT)                /* 0xFF000000 */
/* DTUWD2 */
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE8_SHIFT                 (0U)
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE8_MASK                  (0xFFU << DDR_PCTL_DTUWD2_DTU_WR_BYTE8_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE9_SHIFT                 (8U)
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE9_MASK                  (0xFFU << DDR_PCTL_DTUWD2_DTU_WR_BYTE9_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE10_SHIFT                (16U)
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE10_MASK                 (0xFFU << DDR_PCTL_DTUWD2_DTU_WR_BYTE10_SHIFT)               /* 0x00FF0000 */
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE11_SHIFT                (24U)
#define DDR_PCTL_DTUWD2_DTU_WR_BYTE11_MASK                 (0xFFU << DDR_PCTL_DTUWD2_DTU_WR_BYTE11_SHIFT)               /* 0xFF000000 */
/* DTUWD3 */
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE12_SHIFT                (0U)
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE12_MASK                 (0xFFU << DDR_PCTL_DTUWD3_DTU_WR_BYTE12_SHIFT)               /* 0x000000FF */
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE13_SHIFT                (8U)
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE13_MASK                 (0xFFU << DDR_PCTL_DTUWD3_DTU_WR_BYTE13_SHIFT)               /* 0x0000FF00 */
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE14_SHIFT                (16U)
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE14_MASK                 (0xFFU << DDR_PCTL_DTUWD3_DTU_WR_BYTE14_SHIFT)               /* 0x00FF0000 */
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE15_SHIFT                (24U)
#define DDR_PCTL_DTUWD3_DTU_WR_BYTE15_MASK                 (0xFFU << DDR_PCTL_DTUWD3_DTU_WR_BYTE15_SHIFT)               /* 0xFF000000 */
/* DTUWDM */
#define DDR_PCTL_DTUWDM_DM_WR_BYTE0_SHIFT                  (0U)
#define DDR_PCTL_DTUWDM_DM_WR_BYTE0_MASK                   (0xFFFFU << DDR_PCTL_DTUWDM_DM_WR_BYTE0_SHIFT)               /* 0x0000FFFF */
/* DTURD0 */
#define DDR_PCTL_DTURD0_DTU_RD_BYTE0_SHIFT                 (0U)
#define DDR_PCTL_DTURD0_DTU_RD_BYTE0_MASK                  (0xFFU << DDR_PCTL_DTURD0_DTU_RD_BYTE0_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_DTURD0_DTU_RD_BYTE1_SHIFT                 (8U)
#define DDR_PCTL_DTURD0_DTU_RD_BYTE1_MASK                  (0xFFU << DDR_PCTL_DTURD0_DTU_RD_BYTE1_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_DTURD0_DTU_RD_BYTE2_SHIFT                 (16U)
#define DDR_PCTL_DTURD0_DTU_RD_BYTE2_MASK                  (0xFFU << DDR_PCTL_DTURD0_DTU_RD_BYTE2_SHIFT)                /* 0x00FF0000 */
#define DDR_PCTL_DTURD0_DTU_RD_BYTE3_SHIFT                 (24U)
#define DDR_PCTL_DTURD0_DTU_RD_BYTE3_MASK                  (0xFFU << DDR_PCTL_DTURD0_DTU_RD_BYTE3_SHIFT)                /* 0xFF000000 */
/* DTURD1 */
#define DDR_PCTL_DTURD1_DTU_RD_BYTE4_SHIFT                 (0U)
#define DDR_PCTL_DTURD1_DTU_RD_BYTE4_MASK                  (0xFFU << DDR_PCTL_DTURD1_DTU_RD_BYTE4_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_DTURD1_DTU_RD_BYTE5_SHIFT                 (8U)
#define DDR_PCTL_DTURD1_DTU_RD_BYTE5_MASK                  (0xFFU << DDR_PCTL_DTURD1_DTU_RD_BYTE5_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_DTURD1_DTU_RD_BYTE6_SHIFT                 (16U)
#define DDR_PCTL_DTURD1_DTU_RD_BYTE6_MASK                  (0xFFU << DDR_PCTL_DTURD1_DTU_RD_BYTE6_SHIFT)                /* 0x00FF0000 */
#define DDR_PCTL_DTURD1_DTU_RD_BYTE7_SHIFT                 (24U)
#define DDR_PCTL_DTURD1_DTU_RD_BYTE7_MASK                  (0xFFU << DDR_PCTL_DTURD1_DTU_RD_BYTE7_SHIFT)                /* 0xFF000000 */
/* DTURD2 */
#define DDR_PCTL_DTURD2_DTU_RD_BYTE8_SHIFT                 (0U)
#define DDR_PCTL_DTURD2_DTU_RD_BYTE8_MASK                  (0xFFU << DDR_PCTL_DTURD2_DTU_RD_BYTE8_SHIFT)                /* 0x000000FF */
#define DDR_PCTL_DTURD2_DTU_RD_BYTE9_SHIFT                 (8U)
#define DDR_PCTL_DTURD2_DTU_RD_BYTE9_MASK                  (0xFFU << DDR_PCTL_DTURD2_DTU_RD_BYTE9_SHIFT)                /* 0x0000FF00 */
#define DDR_PCTL_DTURD2_DTU_RD_BYTE10_SHIFT                (16U)
#define DDR_PCTL_DTURD2_DTU_RD_BYTE10_MASK                 (0xFFU << DDR_PCTL_DTURD2_DTU_RD_BYTE10_SHIFT)               /* 0x00FF0000 */
#define DDR_PCTL_DTURD2_DTU_RD_BYTE11_SHIFT                (24U)
#define DDR_PCTL_DTURD2_DTU_RD_BYTE11_MASK                 (0xFFU << DDR_PCTL_DTURD2_DTU_RD_BYTE11_SHIFT)               /* 0xFF000000 */
/* DTURD3 */
#define DDR_PCTL_DTURD3_DTU_RD_BYTE12_SHIFT                (0U)
#define DDR_PCTL_DTURD3_DTU_RD_BYTE12_MASK                 (0xFFU << DDR_PCTL_DTURD3_DTU_RD_BYTE12_SHIFT)               /* 0x000000FF */
#define DDR_PCTL_DTURD3_DTU_RD_BYTE13_SHIFT                (8U)
#define DDR_PCTL_DTURD3_DTU_RD_BYTE13_MASK                 (0xFFU << DDR_PCTL_DTURD3_DTU_RD_BYTE13_SHIFT)               /* 0x0000FF00 */
#define DDR_PCTL_DTURD3_DTU_RD_BYTE14_SHIFT                (16U)
#define DDR_PCTL_DTURD3_DTU_RD_BYTE14_MASK                 (0xFFU << DDR_PCTL_DTURD3_DTU_RD_BYTE14_SHIFT)               /* 0x00FF0000 */
#define DDR_PCTL_DTURD3_DTU_RD_BYTE15_SHIFT                (24U)
#define DDR_PCTL_DTURD3_DTU_RD_BYTE15_MASK                 (0xFFU << DDR_PCTL_DTURD3_DTU_RD_BYTE15_SHIFT)               /* 0xFF000000 */
/* DTULFSRWD */
#define DDR_PCTL_DTULFSRWD_DTU_LFSR_WSEED_SHIFT            (0U)
#define DDR_PCTL_DTULFSRWD_DTU_LFSR_WSEED_MASK             (0xFFFFFFFFU << DDR_PCTL_DTULFSRWD_DTU_LFSR_WSEED_SHIFT)     /* 0xFFFFFFFF */
/* DTULFSRRD */
#define DDR_PCTL_DTULFSRRD_DTU_LFSR_RSEED_SHIFT            (0U)
#define DDR_PCTL_DTULFSRRD_DTU_LFSR_RSEED_MASK             (0xFFFFFFFFU << DDR_PCTL_DTULFSRRD_DTU_LFSR_RSEED_SHIFT)     /* 0xFFFFFFFF */
/* DTUEAF */
#define DDR_PCTL_DTUEAF_EA_COLUMN_SHIFT                    (0U)
#define DDR_PCTL_DTUEAF_EA_COLUMN_MASK                     (0x3FFU << DDR_PCTL_DTUEAF_EA_COLUMN_SHIFT)                  /* 0x000003FF */
#define DDR_PCTL_DTUEAF_EA_BANK_SHIFT                      (10U)
#define DDR_PCTL_DTUEAF_EA_BANK_MASK                       (0x7U << DDR_PCTL_DTUEAF_EA_BANK_SHIFT)                      /* 0x00001C00 */
#define DDR_PCTL_DTUEAF_EA_ROW_SHIFT                       (13U)
#define DDR_PCTL_DTUEAF_EA_ROW_MASK                        (0xFFFFU << DDR_PCTL_DTUEAF_EA_ROW_SHIFT)                    /* 0x1FFFE000 */
#define DDR_PCTL_DTUEAF_EA_RANK_SHIFT                      (30U)
#define DDR_PCTL_DTUEAF_EA_RANK_MASK                       (0x3U << DDR_PCTL_DTUEAF_EA_RANK_SHIFT)                      /* 0xC0000000 */
/* DFITCTRLDELAY */
#define DDR_PCTL_DFITCTRLDELAY_TCTRL_DELAY_SHIFT           (0U)
#define DDR_PCTL_DFITCTRLDELAY_TCTRL_DELAY_MASK            (0xFU << DDR_PCTL_DFITCTRLDELAY_TCTRL_DELAY_SHIFT)           /* 0x0000000F */
/* DFIODTCFG */
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_READ_NSEL_SHIFT       (0U)
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_READ_NSEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK0_ODT_READ_NSEL_SHIFT)       /* 0x00000001 */
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_READ_SEL_SHIFT        (1U)
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_READ_SEL_MASK         (0x1U << DDR_PCTL_DFIODTCFG_RANK0_ODT_READ_SEL_SHIFT)        /* 0x00000002 */
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_WRITE_NSE_SHIFT       (2U)
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_WRITE_NSE_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK0_ODT_WRITE_NSE_SHIFT)       /* 0x00000004 */
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_WRITE_SEL_SHIFT       (3U)
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_WRITE_SEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK0_ODT_WRITE_SEL_SHIFT)       /* 0x00000008 */
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_DEFAULT_SHIFT         (4U)
#define DDR_PCTL_DFIODTCFG_RANK0_ODT_DEFAULT_MASK          (0x1U << DDR_PCTL_DFIODTCFG_RANK0_ODT_DEFAULT_SHIFT)         /* 0x00000010 */
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_READ_NSEL_SHIFT       (8U)
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_READ_NSEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK1_ODT_READ_NSEL_SHIFT)       /* 0x00000100 */
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_READ_SEL_SHIFT        (9U)
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_READ_SEL_MASK         (0x1U << DDR_PCTL_DFIODTCFG_RANK1_ODT_READ_SEL_SHIFT)        /* 0x00000200 */
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_WRITE_NSE_SHIFT       (10U)
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_WRITE_NSE_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK1_ODT_WRITE_NSE_SHIFT)       /* 0x00000400 */
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_WRITE_SEL_SHIFT       (11U)
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_WRITE_SEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK1_ODT_WRITE_SEL_SHIFT)       /* 0x00000800 */
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_DEFAULT_SHIFT         (12U)
#define DDR_PCTL_DFIODTCFG_RANK1_ODT_DEFAULT_MASK          (0x1U << DDR_PCTL_DFIODTCFG_RANK1_ODT_DEFAULT_SHIFT)         /* 0x00001000 */
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_READ_NSEL_SHIFT       (16U)
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_READ_NSEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK2_ODT_READ_NSEL_SHIFT)       /* 0x00010000 */
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_READ_SEL_SHIFT        (17U)
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_READ_SEL_MASK         (0x1U << DDR_PCTL_DFIODTCFG_RANK2_ODT_READ_SEL_SHIFT)        /* 0x00020000 */
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_WRITE_NSE_SHIFT       (18U)
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_WRITE_NSE_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK2_ODT_WRITE_NSE_SHIFT)       /* 0x00040000 */
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_WRITE_SEL_SHIFT       (19U)
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_WRITE_SEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK2_ODT_WRITE_SEL_SHIFT)       /* 0x00080000 */
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_DEFAULT_SHIFT         (20U)
#define DDR_PCTL_DFIODTCFG_RANK2_ODT_DEFAULT_MASK          (0x1U << DDR_PCTL_DFIODTCFG_RANK2_ODT_DEFAULT_SHIFT)         /* 0x00100000 */
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_READ_NSEL_SHIFT       (24U)
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_READ_NSEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK3_ODT_READ_NSEL_SHIFT)       /* 0x01000000 */
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_READ_SEL_SHIFT        (25U)
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_READ_SEL_MASK         (0x1U << DDR_PCTL_DFIODTCFG_RANK3_ODT_READ_SEL_SHIFT)        /* 0x02000000 */
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_WRITE_NSE_SHIFT       (26U)
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_WRITE_NSE_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK3_ODT_WRITE_NSE_SHIFT)       /* 0x04000000 */
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_WRITE_SEL_SHIFT       (27U)
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_WRITE_SEL_MASK        (0x1U << DDR_PCTL_DFIODTCFG_RANK3_ODT_WRITE_SEL_SHIFT)       /* 0x08000000 */
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_DEFAULT_SHIFT         (28U)
#define DDR_PCTL_DFIODTCFG_RANK3_ODT_DEFAULT_MASK          (0x1U << DDR_PCTL_DFIODTCFG_RANK3_ODT_DEFAULT_SHIFT)         /* 0x10000000 */
/* DFIODTCFG1 */
#define DDR_PCTL_DFIODTCFG1_ODT_LAT_W_SHIFT                (0U)
#define DDR_PCTL_DFIODTCFG1_ODT_LAT_W_MASK                 (0x1FU << DDR_PCTL_DFIODTCFG1_ODT_LAT_W_SHIFT)               /* 0x0000001F */
#define DDR_PCTL_DFIODTCFG1_ODT_LAT_R_SHIFT                (8U)
#define DDR_PCTL_DFIODTCFG1_ODT_LAT_R_MASK                 (0x1FU << DDR_PCTL_DFIODTCFG1_ODT_LAT_R_SHIFT)               /* 0x00001F00 */
#define DDR_PCTL_DFIODTCFG1_ODT_LEN_BL8_W_SHIFT            (16U)
#define DDR_PCTL_DFIODTCFG1_ODT_LEN_BL8_W_MASK             (0x7U << DDR_PCTL_DFIODTCFG1_ODT_LEN_BL8_W_SHIFT)            /* 0x00070000 */
#define DDR_PCTL_DFIODTCFG1_ODT_LEN_BL8_R_SHIFT            (24U)
#define DDR_PCTL_DFIODTCFG1_ODT_LEN_BL8_R_MASK             (0x7U << DDR_PCTL_DFIODTCFG1_ODT_LEN_BL8_R_SHIFT)            /* 0x07000000 */
/* DFIODTRANKMAP */
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP0_SHIFT         (0U)
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP0_MASK          (0xFU << DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP0_SHIFT)         /* 0x0000000F */
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP1_SHIFT         (4U)
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP1_MASK          (0xFU << DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP1_SHIFT)         /* 0x000000F0 */
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP2_SHIFT         (8U)
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP2_MASK          (0xFU << DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP2_SHIFT)         /* 0x00000F00 */
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP3_SHIFT         (12U)
#define DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP3_MASK          (0xFU << DDR_PCTL_DFIODTRANKMAP_ODT_RANK_MAP3_SHIFT)         /* 0x0000F000 */
/* DFITPHYWRDATA */
#define DDR_PCTL_DFITPHYWRDATA_TPHY_WRDATA_SHIFT           (0U)
#define DDR_PCTL_DFITPHYWRDATA_TPHY_WRDATA_MASK            (0x3FU << DDR_PCTL_DFITPHYWRDATA_TPHY_WRDATA_SHIFT)          /* 0x0000003F */
/* DFITPHYWRLAT */
#define DDR_PCTL_DFITPHYWRLAT_TPHY_WRLAT_SHIFT             (0U)
#define DDR_PCTL_DFITPHYWRLAT_TPHY_WRLAT_MASK              (0x3FU << DDR_PCTL_DFITPHYWRLAT_TPHY_WRLAT_SHIFT)            /* 0x0000003F */
/* DFITPHYWRDATALAT */
#define DDR_PCTL_DFITPHYWRDATALAT_TPHY_WRDATA_LAT_SHIFT    (0U)
#define DDR_PCTL_DFITPHYWRDATALAT_TPHY_WRDATA_LAT_MASK     (0x3FU << DDR_PCTL_DFITPHYWRDATALAT_TPHY_WRDATA_LAT_SHIFT)   /* 0x0000003F */
/* DFITRDDATAEN */
#define DDR_PCTL_DFITRDDATAEN_TRDDATA_EN_SHIFT             (0U)
#define DDR_PCTL_DFITRDDATAEN_TRDDATA_EN_MASK              (0x3FU << DDR_PCTL_DFITRDDATAEN_TRDDATA_EN_SHIFT)            /* 0x0000003F */
/* DFITPHYRDLAT */
#define DDR_PCTL_DFITPHYRDLAT_TPHY_RDLAT_SHIFT             (0U)
#define DDR_PCTL_DFITPHYRDLAT_TPHY_RDLAT_MASK              (0x3FU << DDR_PCTL_DFITPHYRDLAT_TPHY_RDLAT_SHIFT)            /* 0x0000003F */
/* DFITPHYUPDTYPE0 */
#define DDR_PCTL_DFITPHYUPDTYPE0_TPHYUPD_TYPE0_SHIFT       (0U)
#define DDR_PCTL_DFITPHYUPDTYPE0_TPHYUPD_TYPE0_MASK        (0xFFFU << DDR_PCTL_DFITPHYUPDTYPE0_TPHYUPD_TYPE0_SHIFT)     /* 0x00000FFF */
/* DFITPHYUPDTYPE1 */
#define DDR_PCTL_DFITPHYUPDTYPE1_TPHYUPD_TYPE1_SHIFT       (0U)
#define DDR_PCTL_DFITPHYUPDTYPE1_TPHYUPD_TYPE1_MASK        (0xFFFU << DDR_PCTL_DFITPHYUPDTYPE1_TPHYUPD_TYPE1_SHIFT)     /* 0x00000FFF */
/* DFITPHYUPDTYPE2 */
#define DDR_PCTL_DFITPHYUPDTYPE2_TPHYUPD_TYPE2_SHIFT       (0U)
#define DDR_PCTL_DFITPHYUPDTYPE2_TPHYUPD_TYPE2_MASK        (0xFFFU << DDR_PCTL_DFITPHYUPDTYPE2_TPHYUPD_TYPE2_SHIFT)     /* 0x00000FFF */
/* DFITPHYUPDTYPE3 */
#define DDR_PCTL_DFITPHYUPDTYPE3_TPHYUPD_TYPE3_SHIFT       (0U)
#define DDR_PCTL_DFITPHYUPDTYPE3_TPHYUPD_TYPE3_MASK        (0xFFFU << DDR_PCTL_DFITPHYUPDTYPE3_TPHYUPD_TYPE3_SHIFT)     /* 0x00000FFF */
/* DFITCTRLUPDMIN */
#define DDR_PCTL_DFITCTRLUPDMIN_TCTRLUPD_MIN_SHIFT         (0U)
#define DDR_PCTL_DFITCTRLUPDMIN_TCTRLUPD_MIN_MASK          (0xFFFFU << DDR_PCTL_DFITCTRLUPDMIN_TCTRLUPD_MIN_SHIFT)      /* 0x0000FFFF */
/* DFITCTRLUPDMAX */
#define DDR_PCTL_DFITCTRLUPDMAX_TCTRLUPD_MAX_SHIFT         (0U)
#define DDR_PCTL_DFITCTRLUPDMAX_TCTRLUPD_MAX_MASK          (0xFFFFU << DDR_PCTL_DFITCTRLUPDMAX_TCTRLUPD_MAX_SHIFT)      /* 0x0000FFFF */
/* DFITCTRLUPDDLY */
#define DDR_PCTL_DFITCTRLUPDDLY_TCTRLUPD_DLY_SHIFT         (0U)
#define DDR_PCTL_DFITCTRLUPDDLY_TCTRLUPD_DLY_MASK          (0xFU << DDR_PCTL_DFITCTRLUPDDLY_TCTRLUPD_DLY_SHIFT)         /* 0x0000000F */
/* DFIUPDCFG */
#define DDR_PCTL_DFIUPDCFG_DFI_CTRLUPD_EN_SHIFT            (0U)
#define DDR_PCTL_DFIUPDCFG_DFI_CTRLUPD_EN_MASK             (0x1U << DDR_PCTL_DFIUPDCFG_DFI_CTRLUPD_EN_SHIFT)            /* 0x00000001 */
#define DDR_PCTL_DFIUPDCFG_DFI_PHYUPD_EN_SHIFT             (1U)
#define DDR_PCTL_DFIUPDCFG_DFI_PHYUPD_EN_MASK              (0x1U << DDR_PCTL_DFIUPDCFG_DFI_PHYUPD_EN_SHIFT)             /* 0x00000002 */
/* DFITREFMSKI */
#define DDR_PCTL_DFITREFMSKI_TREFMSKI_SHIFT                (0U)
#define DDR_PCTL_DFITREFMSKI_TREFMSKI_MASK                 (0xFFU << DDR_PCTL_DFITREFMSKI_TREFMSKI_SHIFT)               /* 0x000000FF */
/* DFITCTRLUPDI */
#define DDR_PCTL_DFITCTRLUPDI_TCTRLUPD_INTERVAL_SHIFT      (0U)
#define DDR_PCTL_DFITCTRLUPDI_TCTRLUPD_INTERVAL_MASK       (0xFFFFFFFFU << DDR_PCTL_DFITCTRLUPDI_TCTRLUPD_INTERVAL_SHIFT) /* 0xFFFFFFFF */
/* DFITRCFG0 */
#define DDR_PCTL_DFITRCFG0_DFI_RDLVL_RANK_SEL_SHIFT        (0U)
#define DDR_PCTL_DFITRCFG0_DFI_RDLVL_RANK_SEL_MASK         (0xFU << DDR_PCTL_DFITRCFG0_DFI_RDLVL_RANK_SEL_SHIFT)        /* 0x0000000F */
#define DDR_PCTL_DFITRCFG0_DFI_RDLVL_EDGE_SHIFT            (4U)
#define DDR_PCTL_DFITRCFG0_DFI_RDLVL_EDGE_MASK             (0x1FFU << DDR_PCTL_DFITRCFG0_DFI_RDLVL_EDGE_SHIFT)          /* 0x00001FF0 */
#define DDR_PCTL_DFITRCFG0_DFI_WRLVL_RANK_SEL_SHIFT        (16U)
#define DDR_PCTL_DFITRCFG0_DFI_WRLVL_RANK_SEL_MASK         (0xFU << DDR_PCTL_DFITRCFG0_DFI_WRLVL_RANK_SEL_SHIFT)        /* 0x000F0000 */
/* DFITRSTAT0 */
#define DDR_PCTL_DFITRSTAT0_DFI_RDLVL_MODE_SHIFT           (0U)
#define DDR_PCTL_DFITRSTAT0_DFI_RDLVL_MODE_MASK            (0x3U << DDR_PCTL_DFITRSTAT0_DFI_RDLVL_MODE_SHIFT)           /* 0x00000003 */
#define DDR_PCTL_DFITRSTAT0_DFI_RDLVL_GATE_MODE_SHIFT      (8U)
#define DDR_PCTL_DFITRSTAT0_DFI_RDLVL_GATE_MODE_MASK       (0x3U << DDR_PCTL_DFITRSTAT0_DFI_RDLVL_GATE_MODE_SHIFT)      /* 0x00000300 */
#define DDR_PCTL_DFITRSTAT0_DFI_WRLVL_MODE_SHIFT           (16U)
#define DDR_PCTL_DFITRSTAT0_DFI_WRLVL_MODE_MASK            (0x3U << DDR_PCTL_DFITRSTAT0_DFI_WRLVL_MODE_SHIFT)           /* 0x00030000 */
/* DFITRWRLVLEN */
#define DDR_PCTL_DFITRWRLVLEN_DFI_WRLVL_EN_SHIFT           (0U)
#define DDR_PCTL_DFITRWRLVLEN_DFI_WRLVL_EN_MASK            (0x1FFU << DDR_PCTL_DFITRWRLVLEN_DFI_WRLVL_EN_SHIFT)         /* 0x000001FF */
/* DFITRRDLVLEN */
#define DDR_PCTL_DFITRRDLVLEN_DFI_RDLVL_EN_SHIFT           (0U)
#define DDR_PCTL_DFITRRDLVLEN_DFI_RDLVL_EN_MASK            (0x1FFU << DDR_PCTL_DFITRRDLVLEN_DFI_RDLVL_EN_SHIFT)         /* 0x000001FF */
/* DFITRRDLVLGATEEN */
#define DDR_PCTL_DFITRRDLVLGATEEN_DFI_RDLVL_GATE_EN_SHIFT  (0U)
#define DDR_PCTL_DFITRRDLVLGATEEN_DFI_RDLVL_GATE_EN_MASK   (0x1FFU << DDR_PCTL_DFITRRDLVLGATEEN_DFI_RDLVL_GATE_EN_SHIFT) /* 0x000001FF */
/* DFISTSTAT0 */
#define DDR_PCTL_DFISTSTAT0_DFI_INIT_COMPLETE_SHIFT        (0U)
#define DDR_PCTL_DFISTSTAT0_DFI_INIT_COMPLETE_MASK         (0x1U << DDR_PCTL_DFISTSTAT0_DFI_INIT_COMPLETE_SHIFT)        /* 0x00000001 */
#define DDR_PCTL_DFISTSTAT0_DFI_INIT_START_SHIFT           (1U)
#define DDR_PCTL_DFISTSTAT0_DFI_INIT_START_MASK            (0x1U << DDR_PCTL_DFISTSTAT0_DFI_INIT_START_SHIFT)           /* 0x00000002 */
#define DDR_PCTL_DFISTSTAT0_DFI_FREQ_RATIO_SHIFT           (4U)
#define DDR_PCTL_DFISTSTAT0_DFI_FREQ_RATIO_MASK            (0x3U << DDR_PCTL_DFISTSTAT0_DFI_FREQ_RATIO_SHIFT)           /* 0x00000030 */
#define DDR_PCTL_DFISTSTAT0_DFI_DATA_BYTE_DISABLE_SHIFT    (16U)
#define DDR_PCTL_DFISTSTAT0_DFI_DATA_BYTE_DISABLE_MASK     (0x1FFU << DDR_PCTL_DFISTSTAT0_DFI_DATA_BYTE_DISABLE_SHIFT)  /* 0x01FF0000 */
/* DFISTCFG0 */
#define DDR_PCTL_DFISTCFG0_DFI_INIT_START_SHIFT            (0U)
#define DDR_PCTL_DFISTCFG0_DFI_INIT_START_MASK             (0x1U << DDR_PCTL_DFISTCFG0_DFI_INIT_START_SHIFT)            /* 0x00000001 */
#define DDR_PCTL_DFISTCFG0_DFI_FREQ_RATIO_EN_SHIFT         (1U)
#define DDR_PCTL_DFISTCFG0_DFI_FREQ_RATIO_EN_MASK          (0x1U << DDR_PCTL_DFISTCFG0_DFI_FREQ_RATIO_EN_SHIFT)         /* 0x00000002 */
#define DDR_PCTL_DFISTCFG0_DFI_DATA_BYTE_DISABLE_EN_SHIFT  (2U)
#define DDR_PCTL_DFISTCFG0_DFI_DATA_BYTE_DISABLE_EN_MASK   (0x1U << DDR_PCTL_DFISTCFG0_DFI_DATA_BYTE_DISABLE_EN_SHIFT)  /* 0x00000004 */
/* DFISTCFG1 */
#define DDR_PCTL_DFISTCFG1_DFI_DRAM_CLK_DISABLE_EN_SHIFT   (0U)
#define DDR_PCTL_DFISTCFG1_DFI_DRAM_CLK_DISABLE_EN_MASK    (0x1U << DDR_PCTL_DFISTCFG1_DFI_DRAM_CLK_DISABLE_EN_SHIFT)   /* 0x00000001 */
#define DDR_PCTL_DFISTCFG1_DFI_DRAM_CLK_DISABLE_EN_DPD_SHIFT (1U)
#define DDR_PCTL_DFISTCFG1_DFI_DRAM_CLK_DISABLE_EN_DPD_MASK (0x1U << DDR_PCTL_DFISTCFG1_DFI_DRAM_CLK_DISABLE_EN_DPD_SHIFT) /* 0x00000002 */
/* DFITDRAMCLKEN */
#define DDR_PCTL_DFITDRAMCLKEN_TDRAM_CLK_ENABLE_SHIFT      (0U)
#define DDR_PCTL_DFITDRAMCLKEN_TDRAM_CLK_ENABLE_MASK       (0xFU << DDR_PCTL_DFITDRAMCLKEN_TDRAM_CLK_ENABLE_SHIFT)      /* 0x0000000F */
/* DFITDRAMCLKDIS */
#define DDR_PCTL_DFITDRAMCLKDIS_TDRAM_CLK_DISABLE_SHIFT    (0U)
#define DDR_PCTL_DFITDRAMCLKDIS_TDRAM_CLK_DISABLE_MASK     (0xFU << DDR_PCTL_DFITDRAMCLKDIS_TDRAM_CLK_DISABLE_SHIFT)    /* 0x0000000F */
/* DFISTCFG2 */
#define DDR_PCTL_DFISTCFG2_PARITY_INTR_EN_SHIFT            (0U)
#define DDR_PCTL_DFISTCFG2_PARITY_INTR_EN_MASK             (0x1U << DDR_PCTL_DFISTCFG2_PARITY_INTR_EN_SHIFT)            /* 0x00000001 */
#define DDR_PCTL_DFISTCFG2_PARITY_EN_SHIFT                 (1U)
#define DDR_PCTL_DFISTCFG2_PARITY_EN_MASK                  (0x1U << DDR_PCTL_DFISTCFG2_PARITY_EN_SHIFT)                 /* 0x00000002 */
/* DFISTPARCLR */
#define DDR_PCTL_DFISTPARCLR_PARITY_INTR_CLR_SHIFT         (0U)
#define DDR_PCTL_DFISTPARCLR_PARITY_INTR_CLR_MASK          (0x1U << DDR_PCTL_DFISTPARCLR_PARITY_INTR_CLR_SHIFT)         /* 0x00000001 */
#define DDR_PCTL_DFISTPARCLR_PARITY_LOG_CLR_SHIFT          (1U)
#define DDR_PCTL_DFISTPARCLR_PARITY_LOG_CLR_MASK           (0x1U << DDR_PCTL_DFISTPARCLR_PARITY_LOG_CLR_SHIFT)          /* 0x00000002 */
/* DFISTPARLOG */
#define DDR_PCTL_DFISTPARLOG_PARITY_ERR_CNT_SHIFT          (0U)
#define DDR_PCTL_DFISTPARLOG_PARITY_ERR_CNT_MASK           (0xFFFFFFFFU << DDR_PCTL_DFISTPARLOG_PARITY_ERR_CNT_SHIFT)   /* 0xFFFFFFFF */
/* DFILPCFG0 */
#define DDR_PCTL_DFILPCFG0_DFI_LP_EN_PD_SHIFT              (0U)
#define DDR_PCTL_DFILPCFG0_DFI_LP_EN_PD_MASK               (0x1U << DDR_PCTL_DFILPCFG0_DFI_LP_EN_PD_SHIFT)              /* 0x00000001 */
#define DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_PD_SHIFT          (4U)
#define DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_PD_MASK           (0xFU << DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_PD_SHIFT)          /* 0x000000F0 */
#define DDR_PCTL_DFILPCFG0_DFI_LP_EN_SR_SHIFT              (8U)
#define DDR_PCTL_DFILPCFG0_DFI_LP_EN_SR_MASK               (0x1U << DDR_PCTL_DFILPCFG0_DFI_LP_EN_SR_SHIFT)              /* 0x00000100 */
#define DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_SR_SHIFT          (12U)
#define DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_SR_MASK           (0xFU << DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_SR_SHIFT)          /* 0x0000F000 */
#define DDR_PCTL_DFILPCFG0_DFI_TLP_RESP_SHIFT              (16U)
#define DDR_PCTL_DFILPCFG0_DFI_TLP_RESP_MASK               (0xFU << DDR_PCTL_DFILPCFG0_DFI_TLP_RESP_SHIFT)              /* 0x000F0000 */
#define DDR_PCTL_DFILPCFG0_DFI_LP_EN_DPD_SHIFT             (24U)
#define DDR_PCTL_DFILPCFG0_DFI_LP_EN_DPD_MASK              (0x1U << DDR_PCTL_DFILPCFG0_DFI_LP_EN_DPD_SHIFT)             /* 0x01000000 */
#define DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_DPD_SHIFT         (28U)
#define DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_DPD_MASK          (0xFU << DDR_PCTL_DFILPCFG0_DFI_LP_WAKEUP_DPD_SHIFT)         /* 0xF0000000 */
/* DFITRWRLVLRESP0 */
#define DDR_PCTL_DFITRWRLVLRESP0_DFI_WRLVL_RESP0_SHIFT     (0U)
#define DDR_PCTL_DFITRWRLVLRESP0_DFI_WRLVL_RESP0_MASK      (0xFFFFFFFFU << DDR_PCTL_DFITRWRLVLRESP0_DFI_WRLVL_RESP0_SHIFT) /* 0xFFFFFFFF */
/* DFITRWRLVLRESP1 */
#define DDR_PCTL_DFITRWRLVLRESP1_DFI_WRLVL_RESP1_SHIFT     (0U)
#define DDR_PCTL_DFITRWRLVLRESP1_DFI_WRLVL_RESP1_MASK      (0xFFFFFFFFU << DDR_PCTL_DFITRWRLVLRESP1_DFI_WRLVL_RESP1_SHIFT) /* 0xFFFFFFFF */
/* DFITRWRLVLRESP2 */
#define DDR_PCTL_DFITRWRLVLRESP2_DFI_WRLVL_RESP2_SHIFT     (0U)
#define DDR_PCTL_DFITRWRLVLRESP2_DFI_WRLVL_RESP2_MASK      (0xFFU << DDR_PCTL_DFITRWRLVLRESP2_DFI_WRLVL_RESP2_SHIFT)    /* 0x000000FF */
/* DFITRRDLVLRESP0 */
#define DDR_PCTL_DFITRRDLVLRESP0_DFI_RDLVL_RESP0_SHIFT     (0U)
#define DDR_PCTL_DFITRRDLVLRESP0_DFI_RDLVL_RESP0_MASK      (0xFFFFFFFFU << DDR_PCTL_DFITRRDLVLRESP0_DFI_RDLVL_RESP0_SHIFT) /* 0xFFFFFFFF */
/* DFITRRDLVLRESP1 */
#define DDR_PCTL_DFITRRDLVLRESP1_DFI_RDLVL_RESP1_SHIFT     (0U)
#define DDR_PCTL_DFITRRDLVLRESP1_DFI_RDLVL_RESP1_MASK      (0xFFFFFFFFU << DDR_PCTL_DFITRRDLVLRESP1_DFI_RDLVL_RESP1_SHIFT) /* 0xFFFFFFFF */
/* DFITRRDLVLRESP2 */
#define DDR_PCTL_DFITRRDLVLRESP2_DFI_RDLVL_RESP2_SHIFT     (0U)
#define DDR_PCTL_DFITRRDLVLRESP2_DFI_RDLVL_RESP2_MASK      (0xFFU << DDR_PCTL_DFITRRDLVLRESP2_DFI_RDLVL_RESP2_SHIFT)    /* 0x000000FF */
/* DFITRWRLVLDELAY0 */
#define DDR_PCTL_DFITRWRLVLDELAY0_DFI_WRLVL_DELAY0_SHIFT   (0U)
#define DDR_PCTL_DFITRWRLVLDELAY0_DFI_WRLVL_DELAY0_MASK    (0xFFFFFFFFU << DDR_PCTL_DFITRWRLVLDELAY0_DFI_WRLVL_DELAY0_SHIFT) /* 0xFFFFFFFF */
/* DFITRWRLVLDELAY1 */
#define DDR_PCTL_DFITRWRLVLDELAY1_DFI_WRLVL_DELAY1_SHIFT   (0U)
#define DDR_PCTL_DFITRWRLVLDELAY1_DFI_WRLVL_DELAY1_MASK    (0xFFFFFFFFU << DDR_PCTL_DFITRWRLVLDELAY1_DFI_WRLVL_DELAY1_SHIFT) /* 0xFFFFFFFF */
/* DFITRWRLVLDELAY2 */
#define DDR_PCTL_DFITRWRLVLDELAY2_DFI_WRLVL_DELAY2_SHIFT   (0U)
#define DDR_PCTL_DFITRWRLVLDELAY2_DFI_WRLVL_DELAY2_MASK    (0xFFU << DDR_PCTL_DFITRWRLVLDELAY2_DFI_WRLVL_DELAY2_SHIFT)  /* 0x000000FF */
/* DFITRRDLVLDELAY0 */
#define DDR_PCTL_DFITRRDLVLDELAY0_DFI_RDLVL_DELAY0_SHIFT   (0U)
#define DDR_PCTL_DFITRRDLVLDELAY0_DFI_RDLVL_DELAY0_MASK    (0xFFFFFFFFU << DDR_PCTL_DFITRRDLVLDELAY0_DFI_RDLVL_DELAY0_SHIFT) /* 0xFFFFFFFF */
/* DFITRRDLVLDELAY1 */
#define DDR_PCTL_DFITRRDLVLDELAY1_DFI_RDLVL_DELAY1_SHIFT   (0U)
#define DDR_PCTL_DFITRRDLVLDELAY1_DFI_RDLVL_DELAY1_MASK    (0xFFFFFFFFU << DDR_PCTL_DFITRRDLVLDELAY1_DFI_RDLVL_DELAY1_SHIFT) /* 0xFFFFFFFF */
/* DFITRRDLVLDELAY2 */
#define DDR_PCTL_DFITRRDLVLDELAY2_DFI_RDLVL_DELAY2_SHIFT   (0U)
#define DDR_PCTL_DFITRRDLVLDELAY2_DFI_RDLVL_DELAY2_MASK    (0xFFU << DDR_PCTL_DFITRRDLVLDELAY2_DFI_RDLVL_DELAY2_SHIFT)  /* 0x000000FF */
/* DFITRRDLVLGATEDELAY0 */
#define DDR_PCTL_DFITRRDLVLGATEDELAY0_DFI_RDLVL_GATE_DELAY0_SHIFT (0U)
#define DDR_PCTL_DFITRRDLVLGATEDELAY0_DFI_RDLVL_GATE_DELAY0_MASK (0xFFFFFFFFU << DDR_PCTL_DFITRRDLVLGATEDELAY0_DFI_RDLVL_GATE_DELAY0_SHIFT) /* 0xFFFFFFFF */
/* DFITRRDLVLGATEDELAY1 */
#define DDR_PCTL_DFITRRDLVLGATEDELAY1_DFI_RDLVL_GATE_DELAY1_SHIFT (0U)
#define DDR_PCTL_DFITRRDLVLGATEDELAY1_DFI_RDLVL_GATE_DELAY1_MASK (0xFFFFFFFFU << DDR_PCTL_DFITRRDLVLGATEDELAY1_DFI_RDLVL_GATE_DELAY1_SHIFT) /* 0xFFFFFFFF */
/* DFITRRDLVLGATEDELAY2 */
#define DDR_PCTL_DFITRRDLVLGATEDELAY2_DFI_RDLVL_GATE_DELAY2_SHIFT (0U)
#define DDR_PCTL_DFITRRDLVLGATEDELAY2_DFI_RDLVL_GATE_DELAY2_MASK (0xFFU << DDR_PCTL_DFITRRDLVLGATEDELAY2_DFI_RDLVL_GATE_DELAY2_SHIFT) /* 0x000000FF */
/* DFITRCMD */
#define DDR_PCTL_DFITRCMD_DFITRCMD_OPCODE_SHIFT            (0U)
#define DDR_PCTL_DFITRCMD_DFITRCMD_OPCODE_MASK             (0x3U << DDR_PCTL_DFITRCMD_DFITRCMD_OPCODE_SHIFT)            /* 0x00000003 */
#define DDR_PCTL_DFITRCMD_DFITRCMD_EN_SHIFT                (4U)
#define DDR_PCTL_DFITRCMD_DFITRCMD_EN_MASK                 (0x1FFU << DDR_PCTL_DFITRCMD_DFITRCMD_EN_SHIFT)              /* 0x00001FF0 */
#define DDR_PCTL_DFITRCMD_DFITRCMD_START_SHIFT             (31U)
#define DDR_PCTL_DFITRCMD_DFITRCMD_START_MASK              (0x1U << DDR_PCTL_DFITRCMD_DFITRCMD_START_SHIFT)             /* 0x80000000 */
/* IPVR */
#define DDR_PCTL_IPVR_IP_VERSION_SHIFT                     (0U)
#define DDR_PCTL_IPVR_IP_VERSION_MASK                      (0xFFFFFFFFU << DDR_PCTL_IPVR_IP_VERSION_SHIFT)              /* 0xFFFFFFFF */
/* IPTR */
#define DDR_PCTL_IPTR_IP_TYPE_SHIFT                        (0U)
#define DDR_PCTL_IPTR_IP_TYPE_MASK                         (0xFFFFFFFFU << DDR_PCTL_IPTR_IP_TYPE_SHIFT)                 /* 0xFFFFFFFF */
/****************************************USB_GRF*****************************************/
/* GRF_USBPHY_REG0 */
#define USB_GRF_GRF_USBPHY_REG0_USBPHY_CTRLREG15_0_SHIFT   (0U)
#define USB_GRF_GRF_USBPHY_REG0_USBPHY_CTRLREG15_0_MASK    (0xFFFFU << USB_GRF_GRF_USBPHY_REG0_USBPHY_CTRLREG15_0_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG1 */
#define USB_GRF_GRF_USBPHY_REG1_USBPHY_CTRLREG31_16_SHIFT  (0U)
#define USB_GRF_GRF_USBPHY_REG1_USBPHY_CTRLREG31_16_MASK   (0xFFFFU << USB_GRF_GRF_USBPHY_REG1_USBPHY_CTRLREG31_16_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG2 */
#define USB_GRF_GRF_USBPHY_REG2_USBPHY_CTRLREG47_32_SHIFT  (0U)
#define USB_GRF_GRF_USBPHY_REG2_USBPHY_CTRLREG47_32_MASK   (0xFFFFU << USB_GRF_GRF_USBPHY_REG2_USBPHY_CTRLREG47_32_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG3 */
#define USB_GRF_GRF_USBPHY_REG3_USBPHY_CTRLREG63_48_SHIFT  (0U)
#define USB_GRF_GRF_USBPHY_REG3_USBPHY_CTRLREG63_48_MASK   (0xFFFFU << USB_GRF_GRF_USBPHY_REG3_USBPHY_CTRLREG63_48_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG4 */
#define USB_GRF_GRF_USBPHY_REG4_USBPHY_CTRLREG79_64_SHIFT  (0U)
#define USB_GRF_GRF_USBPHY_REG4_USBPHY_CTRLREG79_64_MASK   (0xFFFFU << USB_GRF_GRF_USBPHY_REG4_USBPHY_CTRLREG79_64_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG5 */
#define USB_GRF_GRF_USBPHY_REG5_USBPHY_CTRLREG95_80_SHIFT  (0U)
#define USB_GRF_GRF_USBPHY_REG5_USBPHY_CTRLREG95_80_MASK   (0xFFFFU << USB_GRF_GRF_USBPHY_REG5_USBPHY_CTRLREG95_80_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG6 */
#define USB_GRF_GRF_USBPHY_REG6_USBPHY_CTRLREG111_96_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG6_USBPHY_CTRLREG111_96_MASK  (0xFFFFU << USB_GRF_GRF_USBPHY_REG6_USBPHY_CTRLREG111_96_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG7 */
#define USB_GRF_GRF_USBPHY_REG7_USBPHY_CTRLREG127_112_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG7_USBPHY_CTRLREG127_112_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG7_USBPHY_CTRLREG127_112_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG8 */
#define USB_GRF_GRF_USBPHY_REG8_USBPHY_CTRLREG143_128_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG8_USBPHY_CTRLREG143_128_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG8_USBPHY_CTRLREG143_128_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG9 */
#define USB_GRF_GRF_USBPHY_REG9_USBPHY_CTRLREG159_144_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG9_USBPHY_CTRLREG159_144_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG9_USBPHY_CTRLREG159_144_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG10 */
#define USB_GRF_GRF_USBPHY_REG10_USBPHY_CTRLREG175_160_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG10_USBPHY_CTRLREG175_160_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG10_USBPHY_CTRLREG175_160_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG11 */
#define USB_GRF_GRF_USBPHY_REG11_USBPHY_CTRLREG191_176_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG11_USBPHY_CTRLREG191_176_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG11_USBPHY_CTRLREG191_176_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG12 */
#define USB_GRF_GRF_USBPHY_REG12_USBPHY_CTRLREG207_192_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG12_USBPHY_CTRLREG207_192_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG12_USBPHY_CTRLREG207_192_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG13 */
#define USB_GRF_GRF_USBPHY_REG13_USBPHY_CTRLREG223_208_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG13_USBPHY_CTRLREG223_208_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG13_USBPHY_CTRLREG223_208_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG14 */
#define USB_GRF_GRF_USBPHY_REG14_USBPHY_CTRLREG239_224_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG14_USBPHY_CTRLREG239_224_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG14_USBPHY_CTRLREG239_224_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG15 */
#define USB_GRF_GRF_USBPHY_REG15_USBPHY_CTRLREG255_240_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG15_USBPHY_CTRLREG255_240_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG15_USBPHY_CTRLREG255_240_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG16 */
#define USB_GRF_GRF_USBPHY_REG16_USBPHY_CTRLREG271_256_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG16_USBPHY_CTRLREG271_256_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG16_USBPHY_CTRLREG271_256_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG17 */
#define USB_GRF_GRF_USBPHY_REG17_USBPHY_CTRLREG287_272_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG17_USBPHY_CTRLREG287_272_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG17_USBPHY_CTRLREG287_272_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG18 */
#define USB_GRF_GRF_USBPHY_REG18_USBPHY_CTRLREG303_288_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG18_USBPHY_CTRLREG303_288_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG18_USBPHY_CTRLREG303_288_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG19 */
#define USB_GRF_GRF_USBPHY_REG19_USBPHY_CTRLREG319_304_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG19_USBPHY_CTRLREG319_304_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG19_USBPHY_CTRLREG319_304_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG20 */
#define USB_GRF_GRF_USBPHY_REG20_USBPHY_CTRLREG335_320_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG20_USBPHY_CTRLREG335_320_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG20_USBPHY_CTRLREG335_320_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG21 */
#define USB_GRF_GRF_USBPHY_REG21_USBPHY_CTRLREG351_336_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG21_USBPHY_CTRLREG351_336_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG21_USBPHY_CTRLREG351_336_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG22 */
#define USB_GRF_GRF_USBPHY_REG22_USBPHY_CTRLREG367_352_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG22_USBPHY_CTRLREG367_352_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG22_USBPHY_CTRLREG367_352_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_REG23 */
#define USB_GRF_GRF_USBPHY_REG23_USBPHY_CTRLREG383_368_SHIFT (0U)
#define USB_GRF_GRF_USBPHY_REG23_USBPHY_CTRLREG383_368_MASK (0xFFFFU << USB_GRF_GRF_USBPHY_REG23_USBPHY_CTRLREG383_368_SHIFT) /* 0x0000FFFF */
/* GRF_USBPHY_CON0 */
#define USB_GRF_GRF_USBPHY_CON0_UTMI_SEL_SHIFT             (0U)
#define USB_GRF_GRF_USBPHY_CON0_UTMI_SEL_MASK              (0x1U << USB_GRF_GRF_USBPHY_CON0_UTMI_SEL_SHIFT)             /* 0x00000001 */
#define USB_GRF_GRF_USBPHY_CON0_SUSPENDN_SHIFT             (1U)
#define USB_GRF_GRF_USBPHY_CON0_SUSPENDN_MASK              (0x1U << USB_GRF_GRF_USBPHY_CON0_SUSPENDN_SHIFT)             /* 0x00000002 */
#define USB_GRF_GRF_USBPHY_CON0_OPMODE_SHIFT               (2U)
#define USB_GRF_GRF_USBPHY_CON0_OPMODE_MASK                (0x3U << USB_GRF_GRF_USBPHY_CON0_OPMODE_SHIFT)               /* 0x0000000C */
#define USB_GRF_GRF_USBPHY_CON0_XCVRSELECT_SHIFT           (4U)
#define USB_GRF_GRF_USBPHY_CON0_XCVRSELECT_MASK            (0x3U << USB_GRF_GRF_USBPHY_CON0_XCVRSELECT_SHIFT)           /* 0x00000030 */
#define USB_GRF_GRF_USBPHY_CON0_TERMSELECT_SHIFT           (6U)
#define USB_GRF_GRF_USBPHY_CON0_TERMSELECT_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON0_TERMSELECT_SHIFT)           /* 0x00000040 */
#define USB_GRF_GRF_USBPHY_CON0_DPPULLDOWN_SHIFT           (7U)
#define USB_GRF_GRF_USBPHY_CON0_DPPULLDOWN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON0_DPPULLDOWN_SHIFT)           /* 0x00000080 */
#define USB_GRF_GRF_USBPHY_CON0_DMPULLDOWN_SHIFT           (8U)
#define USB_GRF_GRF_USBPHY_CON0_DMPULLDOWN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON0_DMPULLDOWN_SHIFT)           /* 0x00000100 */
#define USB_GRF_GRF_USBPHY_CON0_IDDIG_SEL_SHIFT            (9U)
#define USB_GRF_GRF_USBPHY_CON0_IDDIG_SEL_MASK             (0x1U << USB_GRF_GRF_USBPHY_CON0_IDDIG_SEL_SHIFT)            /* 0x00000200 */
#define USB_GRF_GRF_USBPHY_CON0_IDDIG_SHIFT                (10U)
#define USB_GRF_GRF_USBPHY_CON0_IDDIG_MASK                 (0x1U << USB_GRF_GRF_USBPHY_CON0_IDDIG_SHIFT)                /* 0x00000400 */
/* GRF_USBPHY_CON1 */
#define USB_GRF_GRF_USBPHY_CON1_UTMI_SEL_SHIFT             (0U)
#define USB_GRF_GRF_USBPHY_CON1_UTMI_SEL_MASK              (0x1U << USB_GRF_GRF_USBPHY_CON1_UTMI_SEL_SHIFT)             /* 0x00000001 */
#define USB_GRF_GRF_USBPHY_CON1_SUSPENDN_SHIFT             (1U)
#define USB_GRF_GRF_USBPHY_CON1_SUSPENDN_MASK              (0x1U << USB_GRF_GRF_USBPHY_CON1_SUSPENDN_SHIFT)             /* 0x00000002 */
#define USB_GRF_GRF_USBPHY_CON1_OPMODE_SHIFT               (2U)
#define USB_GRF_GRF_USBPHY_CON1_OPMODE_MASK                (0x3U << USB_GRF_GRF_USBPHY_CON1_OPMODE_SHIFT)               /* 0x0000000C */
#define USB_GRF_GRF_USBPHY_CON1_XCVRSELECT_SHIFT           (4U)
#define USB_GRF_GRF_USBPHY_CON1_XCVRSELECT_MASK            (0x3U << USB_GRF_GRF_USBPHY_CON1_XCVRSELECT_SHIFT)           /* 0x00000030 */
#define USB_GRF_GRF_USBPHY_CON1_TERMSELECT_SHIFT           (6U)
#define USB_GRF_GRF_USBPHY_CON1_TERMSELECT_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON1_TERMSELECT_SHIFT)           /* 0x00000040 */
#define USB_GRF_GRF_USBPHY_CON1_DPPULLDOWN_SHIFT           (7U)
#define USB_GRF_GRF_USBPHY_CON1_DPPULLDOWN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON1_DPPULLDOWN_SHIFT)           /* 0x00000080 */
#define USB_GRF_GRF_USBPHY_CON1_DMPULLDOWN_SHIFT           (8U)
#define USB_GRF_GRF_USBPHY_CON1_DMPULLDOWN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON1_DMPULLDOWN_SHIFT)           /* 0x00000100 */
/* GRF_USBPHY_CON2 */
#define USB_GRF_GRF_USBPHY_CON2_OTG_DISABLE_0_SHIFT        (0U)
#define USB_GRF_GRF_USBPHY_CON2_OTG_DISABLE_0_MASK         (0x1U << USB_GRF_GRF_USBPHY_CON2_OTG_DISABLE_0_SHIFT)        /* 0x00000001 */
#define USB_GRF_GRF_USBPHY_CON2_OTG_DISABLE_1_SHIFT        (1U)
#define USB_GRF_GRF_USBPHY_CON2_OTG_DISABLE_1_MASK         (0x1U << USB_GRF_GRF_USBPHY_CON2_OTG_DISABLE_1_SHIFT)        /* 0x00000002 */
#define USB_GRF_GRF_USBPHY_CON2_BYPASSDMEN_SHIFT           (2U)
#define USB_GRF_GRF_USBPHY_CON2_BYPASSDMEN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON2_BYPASSDMEN_SHIFT)           /* 0x00000004 */
#define USB_GRF_GRF_USBPHY_CON2_BYPASSSEL_SHIFT            (3U)
#define USB_GRF_GRF_USBPHY_CON2_BYPASSSEL_MASK             (0x1U << USB_GRF_GRF_USBPHY_CON2_BYPASSSEL_SHIFT)            /* 0x00000008 */
#define USB_GRF_GRF_USBPHY_CON2_COMMONONN_SHIFT            (4U)
#define USB_GRF_GRF_USBPHY_CON2_COMMONONN_MASK             (0x1U << USB_GRF_GRF_USBPHY_CON2_COMMONONN_SHIFT)            /* 0x00000010 */
#define USB_GRF_GRF_USBPHY_CON2_IDP_SINK_EN_SHIFT          (7U)
#define USB_GRF_GRF_USBPHY_CON2_IDP_SINK_EN_MASK           (0x1U << USB_GRF_GRF_USBPHY_CON2_IDP_SINK_EN_SHIFT)          /* 0x00000080 */
#define USB_GRF_GRF_USBPHY_CON2_IDM_SINK_EN_SHIFT          (8U)
#define USB_GRF_GRF_USBPHY_CON2_IDM_SINK_EN_MASK           (0x1U << USB_GRF_GRF_USBPHY_CON2_IDM_SINK_EN_SHIFT)          /* 0x00000100 */
#define USB_GRF_GRF_USBPHY_CON2_IDP_SRC_EN_SHIFT           (9U)
#define USB_GRF_GRF_USBPHY_CON2_IDP_SRC_EN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON2_IDP_SRC_EN_SHIFT)           /* 0x00000200 */
#define USB_GRF_GRF_USBPHY_CON2_RDM_PDWN_EN_SHIFT          (10U)
#define USB_GRF_GRF_USBPHY_CON2_RDM_PDWN_EN_MASK           (0x1U << USB_GRF_GRF_USBPHY_CON2_RDM_PDWN_EN_SHIFT)          /* 0x00000400 */
#define USB_GRF_GRF_USBPHY_CON2_VDP_SRC_EN_SHIFT           (11U)
#define USB_GRF_GRF_USBPHY_CON2_VDP_SRC_EN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON2_VDP_SRC_EN_SHIFT)           /* 0x00000800 */
#define USB_GRF_GRF_USBPHY_CON2_VDM_SRC_EN_SHIFT           (12U)
#define USB_GRF_GRF_USBPHY_CON2_VDM_SRC_EN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON2_VDM_SRC_EN_SHIFT)           /* 0x00001000 */
/* GRF_USBPHY_CON3 */
#define USB_GRF_GRF_USBPHY_CON3_UTMI_DRVVBUS_SHIFT         (0U)
#define USB_GRF_GRF_USBPHY_CON3_UTMI_DRVVBUS_MASK          (0x1U << USB_GRF_GRF_USBPHY_CON3_UTMI_DRVVBUS_SHIFT)         /* 0x00000001 */
#define USB_GRF_GRF_USBPHY_CON3_UTMI_CHRGVBUS_SHIFT        (1U)
#define USB_GRF_GRF_USBPHY_CON3_UTMI_CHRGVBUS_MASK         (0x1U << USB_GRF_GRF_USBPHY_CON3_UTMI_CHRGVBUS_SHIFT)        /* 0x00000002 */
#define USB_GRF_GRF_USBPHY_CON3_UTMI_DISCHRGVBUS_SHIFT     (2U)
#define USB_GRF_GRF_USBPHY_CON3_UTMI_DISCHRGVBUS_MASK      (0x1U << USB_GRF_GRF_USBPHY_CON3_UTMI_DISCHRGVBUS_SHIFT)     /* 0x00000004 */
#define USB_GRF_GRF_USBPHY_CON3_DPPULLDOWN_SHIFT           (3U)
#define USB_GRF_GRF_USBPHY_CON3_DPPULLDOWN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON3_DPPULLDOWN_SHIFT)           /* 0x00000008 */
#define USB_GRF_GRF_USBPHY_CON3_DMPULLDOWN_SHIFT           (4U)
#define USB_GRF_GRF_USBPHY_CON3_DMPULLDOWN_MASK            (0x1U << USB_GRF_GRF_USBPHY_CON3_DMPULLDOWN_SHIFT)           /* 0x00000010 */
#define USB_GRF_GRF_USBPHY_CON3_IDPULLUP_SHIFT             (5U)
#define USB_GRF_GRF_USBPHY_CON3_IDPULLUP_MASK              (0x1U << USB_GRF_GRF_USBPHY_CON3_IDPULLUP_SHIFT)             /* 0x00000020 */
#define USB_GRF_GRF_USBPHY_CON3_FS_XVER_OWN_SHIFT          (6U)
#define USB_GRF_GRF_USBPHY_CON3_FS_XVER_OWN_MASK           (0x1U << USB_GRF_GRF_USBPHY_CON3_FS_XVER_OWN_SHIFT)          /* 0x00000040 */
#define USB_GRF_GRF_USBPHY_CON3_FS_OE_SHIFT                (7U)
#define USB_GRF_GRF_USBPHY_CON3_FS_OE_MASK                 (0x1U << USB_GRF_GRF_USBPHY_CON3_FS_OE_SHIFT)                /* 0x00000080 */
#define USB_GRF_GRF_USBPHY_CON3_FS_DATA_SHIFT              (8U)
#define USB_GRF_GRF_USBPHY_CON3_FS_DATA_MASK               (0x1U << USB_GRF_GRF_USBPHY_CON3_FS_DATA_SHIFT)              /* 0x00000100 */
#define USB_GRF_GRF_USBPHY_CON3_FS_SE0_SHIFT               (9U)
#define USB_GRF_GRF_USBPHY_CON3_FS_SE0_MASK                (0x1U << USB_GRF_GRF_USBPHY_CON3_FS_SE0_SHIFT)               /* 0x00000200 */
/****************************************DDR_MON*****************************************/
/* IP_VERSION */
#define DDR_MON_IP_VERSION_IP_VERSION_SHIFT                (0U)
#define DDR_MON_IP_VERSION_IP_VERSION_MASK                 (0xFFU << DDR_MON_IP_VERSION_IP_VERSION_SHIFT)               /* 0x000000FF */
/* DFI_CONTROL */
#define DDR_MON_DFI_CONTROL_START_HARDWARE_MODE_SHIFT      (0U)
#define DDR_MON_DFI_CONTROL_START_HARDWARE_MODE_MASK       (0x1U << DDR_MON_DFI_CONTROL_START_HARDWARE_MODE_SHIFT)      /* 0x00000001 */
#define DDR_MON_DFI_CONTROL_START_SOFTWAREMODE_SHIFT       (1U)
#define DDR_MON_DFI_CONTROL_START_SOFTWAREMODE_MASK        (0x1U << DDR_MON_DFI_CONTROL_START_SOFTWAREMODE_SHIFT)       /* 0x00000002 */
#define DDR_MON_DFI_CONTROL_DDR_TYPE_SEL_SHIFT             (2U)
#define DDR_MON_DFI_CONTROL_DDR_TYPE_SEL_MASK              (0x1U << DDR_MON_DFI_CONTROL_DDR_TYPE_SEL_SHIFT)             /* 0x00000004 */
#define DDR_MON_DFI_CONTROL_WORK_MODEL_SHIFT               (3U)
#define DDR_MON_DFI_CONTROL_WORK_MODEL_MASK                (0x1U << DDR_MON_DFI_CONTROL_WORK_MODEL_SHIFT)               /* 0x00000008 */
#define DDR_MON_DFI_CONTROL_RESERVED_SHIFT                 (4U)
#define DDR_MON_DFI_CONTROL_RESERVED_MASK                  (0xFFFU << DDR_MON_DFI_CONTROL_RESERVED_SHIFT)               /* 0x0000FFF0 */
/* INT_STATE */
#define DDR_MON_INT_STATE_BELLOW_INT_SHIFT                 (0U)
#define DDR_MON_INT_STATE_BELLOW_INT_MASK                  (0x1U << DDR_MON_INT_STATE_BELLOW_INT_SHIFT)                 /* 0x00000001 */
#define DDR_MON_INT_STATE_OVER_INT_SHIFT                   (1U)
#define DDR_MON_INT_STATE_OVER_INT_MASK                    (0x1U << DDR_MON_INT_STATE_OVER_INT_SHIFT)                   /* 0x00000002 */
/* INT_MASK */
#define DDR_MON_INT_MASK_BELLOW_INT_MASK_SHIFT             (0U)
#define DDR_MON_INT_MASK_BELLOW_INT_MASK_MASK              (0x1U << DDR_MON_INT_MASK_BELLOW_INT_MASK_SHIFT)             /* 0x00000001 */
#define DDR_MON_INT_MASK_OVER_INT_MASK_SHIFT               (1U)
#define DDR_MON_INT_MASK_OVER_INT_MASK_MASK                (0x1U << DDR_MON_INT_MASK_OVER_INT_MASK_SHIFT)               /* 0x00000002 */
/* TIMER_COUNT */
#define DDR_MON_TIMER_COUNT_TIMER_COUNTER_SHIFT            (0U)
#define DDR_MON_TIMER_COUNT_TIMER_COUNTER_MASK             (0xFFFFFFFFU << DDR_MON_TIMER_COUNT_TIMER_COUNTER_SHIFT)     /* 0xFFFFFFFF */
/* FLOOR_NUMBER */
#define DDR_MON_FLOOR_NUMBER_FLOOR_NUMBER_SHIFT            (0U)
#define DDR_MON_FLOOR_NUMBER_FLOOR_NUMBER_MASK             (0xFFFFFFFFU << DDR_MON_FLOOR_NUMBER_FLOOR_NUMBER_SHIFT)     /* 0xFFFFFFFF */
/* TOP_NUMBER */
#define DDR_MON_TOP_NUMBER_TOP_NUMBER_SHIFT                (0U)
#define DDR_MON_TOP_NUMBER_TOP_NUMBER_MASK                 (0xFFFFFFFFU << DDR_MON_TOP_NUMBER_TOP_NUMBER_SHIFT)         /* 0xFFFFFFFF */
/* DFI_ACT_NUMBER */
#define DDR_MON_DFI_ACT_NUMBER_DFI_ACT_NUMBER_SHIFT        (0U)
#define DDR_MON_DFI_ACT_NUMBER_DFI_ACT_NUMBER_MASK         (0xFFFFFFFFU << DDR_MON_DFI_ACT_NUMBER_DFI_ACT_NUMBER_SHIFT) /* 0xFFFFFFFF */
/* DFI_ACT_WR_NUM */
#define DDR_MON_DFI_ACT_WR_NUM_DFI_ACT_WR_NUM_SHIFT        (0U)
#define DDR_MON_DFI_ACT_WR_NUM_DFI_ACT_WR_NUM_MASK         (0xFFFFFFFFU << DDR_MON_DFI_ACT_WR_NUM_DFI_ACT_WR_NUM_SHIFT) /* 0xFFFFFFFF */
/* DFI_ACT_RD_NUM */
#define DDR_MON_DFI_ACT_RD_NUM_DFI_ACT_RD_NUM_SHIFT        (0U)
#define DDR_MON_DFI_ACT_RD_NUM_DFI_ACT_RD_NUM_MASK         (0xFFFFFFFFU << DDR_MON_DFI_ACT_RD_NUM_DFI_ACT_RD_NUM_SHIFT) /* 0xFFFFFFFF */
/* DFI_COUNT_NUM */
#define DDR_MON_DFI_COUNT_NUM_DFI_COUNT_NUM_SHIFT          (0U)
#define DDR_MON_DFI_COUNT_NUM_DFI_COUNT_NUM_MASK           (0xFFFFFFFFU << DDR_MON_DFI_COUNT_NUM_DFI_COUNT_NUM_SHIFT)   /* 0xFFFFFFFF */
/* DFI_ACT_ACCESS_NUM */
#define DDR_MON_DFI_ACT_ACCESS_NUM_DFI_ACT_ACCESS_NUM_SHIFT (0U)
#define DDR_MON_DFI_ACT_ACCESS_NUM_DFI_ACT_ACCESS_NUM_MASK (0xFFFFFFFFU << DDR_MON_DFI_ACT_ACCESS_NUM_DFI_ACT_ACCESS_NUM_SHIFT) /* 0xFFFFFFFF */
/*****************************************RKVENC*****************************************/
/* VERSION */
#define RKVENC_VERSION_SUB_VER_SHIFT                       (0U)
#define RKVENC_VERSION_SUB_VER_MASK                        (0xFFU << RKVENC_VERSION_SUB_VER_SHIFT)                      /* 0x000000FF */
#define RKVENC_VERSION_H264_ENC_SHIFT                      (8U)
#define RKVENC_VERSION_H264_ENC_MASK                       (0x1U << RKVENC_VERSION_H264_ENC_SHIFT)                      /* 0x00000100 */
#define RKVENC_VERSION_HEVC_ENC_SHIFT                      (9U)
#define RKVENC_VERSION_HEVC_ENC_MASK                       (0x1U << RKVENC_VERSION_HEVC_ENC_SHIFT)                      /* 0x00000200 */
#define RKVENC_VERSION_PIC_SIZE_SHIFT                      (12U)
#define RKVENC_VERSION_PIC_SIZE_MASK                       (0xFU << RKVENC_VERSION_PIC_SIZE_SHIFT)                      /* 0x0000F000 */
#define RKVENC_VERSION_OSD_CAP_SHIFT                       (16U)
#define RKVENC_VERSION_OSD_CAP_MASK                        (0x3U << RKVENC_VERSION_OSD_CAP_SHIFT)                       /* 0x00030000 */
#define RKVENC_VERSION_FILTR_CAP_SHIFT                     (18U)
#define RKVENC_VERSION_FILTR_CAP_MASK                      (0x3U << RKVENC_VERSION_FILTR_CAP_SHIFT)                     /* 0x000C0000 */
#define RKVENC_VERSION_BFRM_CAP_SHIFT                      (20U)
#define RKVENC_VERSION_BFRM_CAP_MASK                       (0x1U << RKVENC_VERSION_BFRM_CAP_SHIFT)                      /* 0x00100000 */
#define RKVENC_VERSION_FBC_CAP_SHIFT                       (21U)
#define RKVENC_VERSION_FBC_CAP_MASK                        (0x1U << RKVENC_VERSION_FBC_CAP_SHIFT)                       /* 0x00200000 */
#define RKVENC_VERSION_RKVENC_VER_SHIFT                    (24U)
#define RKVENC_VERSION_RKVENC_VER_MASK                     (0xFFU << RKVENC_VERSION_RKVENC_VER_SHIFT)                   /* 0xFF000000 */
/* ENC_STRT */
#define RKVENC_ENC_STRT_LKT_NUM_SHIFT                      (0U)
#define RKVENC_ENC_STRT_LKT_NUM_MASK                       (0xFFU << RKVENC_ENC_STRT_LKT_NUM_SHIFT)                     /* 0x000000FF */
#define RKVENC_ENC_STRT_RKVENC_CMD_SHIFT                   (8U)
#define RKVENC_ENC_STRT_RKVENC_CMD_MASK                    (0x3U << RKVENC_ENC_STRT_RKVENC_CMD_SHIFT)                   /* 0x00000300 */
#define RKVENC_ENC_STRT_ENC_CKE_SHIFT                      (16U)
#define RKVENC_ENC_STRT_ENC_CKE_MASK                       (0x1U << RKVENC_ENC_STRT_ENC_CKE_SHIFT)                      /* 0x00010000 */
/* ENC_CLR */
#define RKVENC_ENC_CLR_SAFE_CLR_SHIFT                      (0U)
#define RKVENC_ENC_CLR_SAFE_CLR_MASK                       (0x1U << RKVENC_ENC_CLR_SAFE_CLR_SHIFT)                      /* 0x00000001 */
/* LKT_ADDR */
#define RKVENC_LKT_ADDR_LKTR_ADDR_SHIFT                    (4U)
#define RKVENC_LKT_ADDR_LKTR_ADDR_MASK                     (0xFFFFFFFU << RKVENC_LKT_ADDR_LKTR_ADDR_SHIFT)              /* 0xFFFFFFF0 */
/* INT_EN */
#define RKVENC_INT_EN_FIELD0000_SHIFT                      (0U)
#define RKVENC_INT_EN_FIELD0000_MASK                       (0x1U << RKVENC_INT_EN_FIELD0000_SHIFT)                      /* 0x00000001 */
/* INT_MSK */
#define RKVENC_INT_MSK_FIELD0000_SHIFT                     (0U)
#define RKVENC_INT_MSK_FIELD0000_MASK                      (0x1U << RKVENC_INT_MSK_FIELD0000_SHIFT)                     /* 0x00000001 */
/* INT_CLR */
#define RKVENC_INT_CLR_FIELD0000_SHIFT                     (0U)
#define RKVENC_INT_CLR_FIELD0000_MASK                      (0x1U << RKVENC_INT_CLR_FIELD0000_SHIFT)                     /* 0x00000001 */
/* INT_STUS */
#define RKVENC_INT_STUS_FIELD0000_SHIFT                    (0U)
#define RKVENC_INT_STUS_FIELD0000_MASK                     (0x1U << RKVENC_INT_STUS_FIELD0000_SHIFT)                    /* 0x00000001 */
/* ENC_RSL */
#define RKVENC_ENC_RSL_FIELD0000_SHIFT                     (0U)
#define RKVENC_ENC_RSL_FIELD0000_MASK                      (0x1U << RKVENC_ENC_RSL_FIELD0000_SHIFT)                     /* 0x00000001 */
/* ENC_PIC */
#define RKVENC_ENC_PIC_FIELD0000_SHIFT                     (0U)
#define RKVENC_ENC_PIC_FIELD0000_MASK                      (0x1U << RKVENC_ENC_PIC_FIELD0000_SHIFT)                     /* 0x00000001 */
/* ENC_WDG */
#define RKVENC_ENC_WDG_FIELD0001_SHIFT                     (1U)
#define RKVENC_ENC_WDG_FIELD0001_MASK                      (0x1U << RKVENC_ENC_WDG_FIELD0001_SHIFT)                     /* 0x00000002 */
/* ST_BSL */
#define RKVENC_ST_BSL_BS_LGTH_SHIFT                        (0U)
#define RKVENC_ST_BSL_BS_LGTH_MASK                         (0x7FFFFFFU << RKVENC_ST_BSL_BS_LGTH_SHIFT)                  /* 0x07FFFFFF */
/* ST_SEE_L32 */
#define RKVENC_ST_SEE_L32_SSE_L32_SHIFT                    (0U)
#define RKVENC_ST_SEE_L32_SSE_L32_MASK                     (0xFFFFFFFFU << RKVENC_ST_SEE_L32_SSE_L32_SHIFT)             /* 0xFFFFFFFF */
/* DBG_DMA_CH_ST */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_PPR_SHIFT        (0U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_PPR_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_PPR_SHIFT)        /* 0x00000001 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_PPR_SHIFT        (1U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_PPR_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_PPR_SHIFT)        /* 0x00000002 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_DRDY_PPW_SHIFT        (2U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_DRDY_PPW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_DRDY_PPW_SHIFT)        /* 0x00000004 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_DVLD_PPW_SHIFT        (3U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_DVLD_PPW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_DVLD_PPW_SHIFT)        /* 0x00000008 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_PPW_SHIFT        (4U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_PPW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_PPW_SHIFT)        /* 0x00000010 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_PPW_SHIFT        (5U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_PPW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_PPW_SHIFT)        /* 0x00000020 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_MEIW_SHIFT        (6U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_MEIW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_MEIW_SHIFT)        /* 0x00000040 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_DSPW_SHIFT        (7U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_DSPW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_DSPW_SHIFT)        /* 0x00000080 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RDY_BSW_SHIFT         (8U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RDY_BSW_MASK          (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RDY_BSW_SHIFT)         /* 0x00000100 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_LST_BSW_SHIFT         (9U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_LST_BSW_MASK          (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_LST_BSW_SHIFT)         /* 0x00000200 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_BSW_SHIFT         (10U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_BSW_MASK          (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_BSW_SHIFT)         /* 0x00000400 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_CRPW_SHIFT        (11U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_CRPW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_CRPW_SHIFT)        /* 0x00000800 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_LKTW_SHIFT        (12U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_LKTW_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_LKTW_SHIFT)        /* 0x00001000 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_CFGR_SHIFT        (13U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_CFGR_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_CFGR_SHIFT)        /* 0x00002000 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_DSPR_SHIFT       (14U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_DSPR_MASK        (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_CRDY_DSPR_SHIFT)       /* 0x00004000 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_DSPR_SHIFT       (15U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_DSPR_MASK        (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_CVLD_DSPR_SHIFT)       /* 0x00008000 */
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_LKTR_SHIFT        (16U)
#define RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_LKTR_MASK         (0x1U << RKVENC_DBG_DMA_CH_ST_DBG_DMA_RUN_LKTR_SHIFT)        /* 0x00010000 */
/*****************************************RKVDEC*****************************************/
/* SWREG0_ID */
#define RKVDEC_SWREG0_ID_MINOR_VER_SHIFT                   (0U)
#define RKVDEC_SWREG0_ID_MINOR_VER_MASK                    (0xFFU << RKVDEC_SWREG0_ID_MINOR_VER_SHIFT)                  /* 0x000000FF */
#define RKVDEC_SWREG0_ID_LEVEL_SHIFT                       (8U)
#define RKVDEC_SWREG0_ID_LEVEL_MASK                        (0x1U << RKVDEC_SWREG0_ID_LEVEL_SHIFT)                       /* 0x00000100 */
#define RKVDEC_SWREG0_ID_DEC_SUPPORT_SHIFT                 (9U)
#define RKVDEC_SWREG0_ID_DEC_SUPPORT_MASK                  (0x7U << RKVDEC_SWREG0_ID_DEC_SUPPORT_SHIFT)                 /* 0x00000E00 */
#define RKVDEC_SWREG0_ID_PROFILE_SHIFT                     (12U)
#define RKVDEC_SWREG0_ID_PROFILE_MASK                      (0x1U << RKVDEC_SWREG0_ID_PROFILE_SHIFT)                     /* 0x00001000 */
#define RKVDEC_SWREG0_ID_CODEC_FLAG_SHIFT                  (14U)
#define RKVDEC_SWREG0_ID_CODEC_FLAG_MASK                   (0x1U << RKVDEC_SWREG0_ID_CODEC_FLAG_SHIFT)                  /* 0x00004000 */
#define RKVDEC_SWREG0_ID_PROD_NUM_SHIFT                    (16U)
#define RKVDEC_SWREG0_ID_PROD_NUM_MASK                     (0xFFFFU << RKVDEC_SWREG0_ID_PROD_NUM_SHIFT)                 /* 0xFFFF0000 */
/* SWREG1_INT */
#define RKVDEC_SWREG1_INT_SW_DEC_E_SHIFT                   (0U)
#define RKVDEC_SWREG1_INT_SW_DEC_E_MASK                    (0x1U << RKVDEC_SWREG1_INT_SW_DEC_E_SHIFT)                   /* 0x00000001 */
#define RKVDEC_SWREG1_INT_SW_DEC_CLKGATE_E_SHIFT           (1U)
#define RKVDEC_SWREG1_INT_SW_DEC_CLKGATE_E_MASK            (0x1U << RKVDEC_SWREG1_INT_SW_DEC_CLKGATE_E_SHIFT)           /* 0x00000002 */
#define RKVDEC_SWREG1_INT_SW_DEC_E_STRMD_CLKGATE_DIS_SHIFT (2U)
#define RKVDEC_SWREG1_INT_SW_DEC_E_STRMD_CLKGATE_DIS_MASK  (0x1U << RKVDEC_SWREG1_INT_SW_DEC_E_STRMD_CLKGATE_DIS_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG1_INT_SW_TIMEOUT_MODE_SHIFT            (3U)
#define RKVDEC_SWREG1_INT_SW_TIMEOUT_MODE_MASK             (0x1U << RKVDEC_SWREG1_INT_SW_TIMEOUT_MODE_SHIFT)            /* 0x00000008 */
#define RKVDEC_SWREG1_INT_SW_DEC_IRQ_DIS_SHIFT             (4U)
#define RKVDEC_SWREG1_INT_SW_DEC_IRQ_DIS_MASK              (0x1U << RKVDEC_SWREG1_INT_SW_DEC_IRQ_DIS_SHIFT)             /* 0x00000010 */
#define RKVDEC_SWREG1_INT_SW_DEC_TIMEOUT_E_SHIFT           (5U)
#define RKVDEC_SWREG1_INT_SW_DEC_TIMEOUT_E_MASK            (0x1U << RKVDEC_SWREG1_INT_SW_DEC_TIMEOUT_E_SHIFT)           /* 0x00000020 */
#define RKVDEC_SWREG1_INT_SW_BUF_EMPTY_EN_SHIFT            (6U)
#define RKVDEC_SWREG1_INT_SW_BUF_EMPTY_EN_MASK             (0x1U << RKVDEC_SWREG1_INT_SW_BUF_EMPTY_EN_SHIFT)            /* 0x00000040 */
#define RKVDEC_SWREG1_INT_SW_STMERROR_WAITDECFIFO_EMPTY_SHIFT (7U)
#define RKVDEC_SWREG1_INT_SW_STMERROR_WAITDECFIFO_EMPTY_MASK (0x1U << RKVDEC_SWREG1_INT_SW_STMERROR_WAITDECFIFO_EMPTY_SHIFT) /* 0x00000080 */
#define RKVDEC_SWREG1_INT_SW_DEC_IRQ_SHIFT                 (8U)
#define RKVDEC_SWREG1_INT_SW_DEC_IRQ_MASK                  (0x1U << RKVDEC_SWREG1_INT_SW_DEC_IRQ_SHIFT)                 /* 0x00000100 */
#define RKVDEC_SWREG1_INT_SW_DEC_IRQ_RAW_SHIFT             (9U)
#define RKVDEC_SWREG1_INT_SW_DEC_IRQ_RAW_MASK              (0x1U << RKVDEC_SWREG1_INT_SW_DEC_IRQ_RAW_SHIFT)             /* 0x00000200 */
#define RKVDEC_SWREG1_INT_SW_DEC_E_REWRITE_VALID_SHIFT     (10U)
#define RKVDEC_SWREG1_INT_SW_DEC_E_REWRITE_VALID_MASK      (0x1U << RKVDEC_SWREG1_INT_SW_DEC_E_REWRITE_VALID_SHIFT)     /* 0x00000400 */
#define RKVDEC_SWREG1_INT_SW_DEC_COMMONIRQ_MODE_SHIFT      (11U)
#define RKVDEC_SWREG1_INT_SW_DEC_COMMONIRQ_MODE_MASK       (0x1U << RKVDEC_SWREG1_INT_SW_DEC_COMMONIRQ_MODE_SHIFT)      /* 0x00000800 */
#define RKVDEC_SWREG1_INT_SW_DEC_RDY_STA_SHIFT             (12U)
#define RKVDEC_SWREG1_INT_SW_DEC_RDY_STA_MASK              (0x1U << RKVDEC_SWREG1_INT_SW_DEC_RDY_STA_SHIFT)             /* 0x00001000 */
#define RKVDEC_SWREG1_INT_SW_DEC_BUS_STA_SHIFT             (13U)
#define RKVDEC_SWREG1_INT_SW_DEC_BUS_STA_MASK              (0x1U << RKVDEC_SWREG1_INT_SW_DEC_BUS_STA_SHIFT)             /* 0x00002000 */
#define RKVDEC_SWREG1_INT_SW_DEC_ERROR_STA_SHIFT           (14U)
#define RKVDEC_SWREG1_INT_SW_DEC_ERROR_STA_MASK            (0x1U << RKVDEC_SWREG1_INT_SW_DEC_ERROR_STA_SHIFT)           /* 0x00004000 */
#define RKVDEC_SWREG1_INT_SW_DEC_TIMEOUT_STA_SHIFT         (15U)
#define RKVDEC_SWREG1_INT_SW_DEC_TIMEOUT_STA_MASK          (0x1U << RKVDEC_SWREG1_INT_SW_DEC_TIMEOUT_STA_SHIFT)         /* 0x00008000 */
#define RKVDEC_SWREG1_INT_SW_BUF_EMPTY_STA_SHIFT           (16U)
#define RKVDEC_SWREG1_INT_SW_BUF_EMPTY_STA_MASK            (0x1U << RKVDEC_SWREG1_INT_SW_BUF_EMPTY_STA_SHIFT)           /* 0x00010000 */
#define RKVDEC_SWREG1_INT_SW_COLMV_REF_ERROR_STA_SHIFT     (17U)
#define RKVDEC_SWREG1_INT_SW_COLMV_REF_ERROR_STA_MASK      (0x1U << RKVDEC_SWREG1_INT_SW_COLMV_REF_ERROR_STA_SHIFT)     /* 0x00020000 */
#define RKVDEC_SWREG1_INT_SW_CABU_END_STA_SHIFT            (18U)
#define RKVDEC_SWREG1_INT_SW_CABU_END_STA_MASK             (0x1U << RKVDEC_SWREG1_INT_SW_CABU_END_STA_SHIFT)            /* 0x00040000 */
#define RKVDEC_SWREG1_INT_SW_H264ORVP9_ERROR_MODE_SHIFT    (19U)
#define RKVDEC_SWREG1_INT_SW_H264ORVP9_ERROR_MODE_MASK     (0x1U << RKVDEC_SWREG1_INT_SW_H264ORVP9_ERROR_MODE_SHIFT)    /* 0x00080000 */
#define RKVDEC_SWREG1_INT_SW_SOFTRST_EN_P_SHIFT            (20U)
#define RKVDEC_SWREG1_INT_SW_SOFTRST_EN_P_MASK             (0x1U << RKVDEC_SWREG1_INT_SW_SOFTRST_EN_P_SHIFT)            /* 0x00100000 */
#define RKVDEC_SWREG1_INT_SW_FORCE_SOFTRESET_VALID_SHIFT   (21U)
#define RKVDEC_SWREG1_INT_SW_FORCE_SOFTRESET_VALID_MASK    (0x1U << RKVDEC_SWREG1_INT_SW_FORCE_SOFTRESET_VALID_SHIFT)   /* 0x00200000 */
#define RKVDEC_SWREG1_INT_SW_SOFTRESET_RDY_SHIFT           (22U)
#define RKVDEC_SWREG1_INT_SW_SOFTRESET_RDY_MASK            (0x1U << RKVDEC_SWREG1_INT_SW_SOFTRESET_RDY_SHIFT)           /* 0x00400000 */
/* SWREG2_SYSCTRL */
#define RKVDEC_SWREG2_SYSCTRL_SW_IN_ENDIAN_SHIFT           (0U)
#define RKVDEC_SWREG2_SYSCTRL_SW_IN_ENDIAN_MASK            (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_IN_ENDIAN_SHIFT)           /* 0x00000001 */
#define RKVDEC_SWREG2_SYSCTRL_SW_IN_SWAP32_E_SHIFT         (1U)
#define RKVDEC_SWREG2_SYSCTRL_SW_IN_SWAP32_E_MASK          (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_IN_SWAP32_E_SHIFT)         /* 0x00000002 */
#define RKVDEC_SWREG2_SYSCTRL_SW_IN_SWAP64_E_SHIFT         (2U)
#define RKVDEC_SWREG2_SYSCTRL_SW_IN_SWAP64_E_MASK          (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_IN_SWAP64_E_SHIFT)         /* 0x00000004 */
#define RKVDEC_SWREG2_SYSCTRL_SW_STR_ENDIAN_SHIFT          (3U)
#define RKVDEC_SWREG2_SYSCTRL_SW_STR_ENDIAN_MASK           (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_STR_ENDIAN_SHIFT)          /* 0x00000008 */
#define RKVDEC_SWREG2_SYSCTRL_SW_STR_SWAP32_E_SHIFT        (4U)
#define RKVDEC_SWREG2_SYSCTRL_SW_STR_SWAP32_E_MASK         (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_STR_SWAP32_E_SHIFT)        /* 0x00000010 */
#define RKVDEC_SWREG2_SYSCTRL_SW_STR_SWAP64_E_SHIFT        (5U)
#define RKVDEC_SWREG2_SYSCTRL_SW_STR_SWAP64_E_MASK         (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_STR_SWAP64_E_SHIFT)        /* 0x00000020 */
#define RKVDEC_SWREG2_SYSCTRL_SW_OUT_ENDIAN_SHIFT          (6U)
#define RKVDEC_SWREG2_SYSCTRL_SW_OUT_ENDIAN_MASK           (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_OUT_ENDIAN_SHIFT)          /* 0x00000040 */
#define RKVDEC_SWREG2_SYSCTRL_SW_OUT_SWAP32_E_SHIFT        (7U)
#define RKVDEC_SWREG2_SYSCTRL_SW_OUT_SWAP32_E_MASK         (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_OUT_SWAP32_E_SHIFT)        /* 0x00000080 */
#define RKVDEC_SWREG2_SYSCTRL_SW_OUT_CBCR_SWAP_SHIFT       (8U)
#define RKVDEC_SWREG2_SYSCTRL_SW_OUT_CBCR_SWAP_MASK        (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_OUT_CBCR_SWAP_SHIFT)       /* 0x00000100 */
#define RKVDEC_SWREG2_SYSCTRL_SW_RLC_MODE_DIRECT_WRITE_SHIFT (10U)
#define RKVDEC_SWREG2_SYSCTRL_SW_RLC_MODE_DIRECT_WRITE_MASK (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_RLC_MODE_DIRECT_WRITE_SHIFT) /* 0x00000400 */
#define RKVDEC_SWREG2_SYSCTRL_SW_RLC_MODE_SHIFT            (11U)
#define RKVDEC_SWREG2_SYSCTRL_SW_RLC_MODE_MASK             (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_RLC_MODE_SHIFT)            /* 0x00000800 */
#define RKVDEC_SWREG2_SYSCTRL_SW_STRM_START_BIT_SHIFT      (12U)
#define RKVDEC_SWREG2_SYSCTRL_SW_STRM_START_BIT_MASK       (0x7FU << RKVDEC_SWREG2_SYSCTRL_SW_STRM_START_BIT_SHIFT)     /* 0x0007F000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_DEC_MODE_SHIFT            (20U)
#define RKVDEC_SWREG2_SYSCTRL_SW_DEC_MODE_MASK             (0x3U << RKVDEC_SWREG2_SYSCTRL_SW_DEC_MODE_SHIFT)            /* 0x00300000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_RPS_MODE_SHIFT       (24U)
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_RPS_MODE_MASK        (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_H264_RPS_MODE_SHIFT)       /* 0x01000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_STREAM_MODE_SHIFT    (25U)
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_STREAM_MODE_MASK     (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_H264_STREAM_MODE_SHIFT)    /* 0x02000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_STREAM_LASTPACKET_SHIFT (26U)
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_STREAM_LASTPACKET_MASK (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_H264_STREAM_LASTPACKET_SHIFT) /* 0x04000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_FIRSTSLICE_FLAG_SHIFT (27U)
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_FIRSTSLICE_FLAG_MASK (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_H264_FIRSTSLICE_FLAG_SHIFT) /* 0x08000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_FRAME_ORSLICE_SHIFT  (28U)
#define RKVDEC_SWREG2_SYSCTRL_SW_H264_FRAME_ORSLICE_MASK   (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_H264_FRAME_ORSLICE_SHIFT)  /* 0x10000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_BUSPR_SLOT_DISABLE_SHIFT  (29U)
#define RKVDEC_SWREG2_SYSCTRL_SW_BUSPR_SLOT_DISABLE_MASK   (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_BUSPR_SLOT_DISABLE_SHIFT)  /* 0x20000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_COLMV_MODE_SHIFT          (30U)
#define RKVDEC_SWREG2_SYSCTRL_SW_COLMV_MODE_MASK           (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_COLMV_MODE_SHIFT)          /* 0x40000000 */
#define RKVDEC_SWREG2_SYSCTRL_SW_YCACHERD_PRIOR_SHIFT      (31U)
#define RKVDEC_SWREG2_SYSCTRL_SW_YCACHERD_PRIOR_MASK       (0x1U << RKVDEC_SWREG2_SYSCTRL_SW_YCACHERD_PRIOR_SHIFT)      /* 0x80000000 */
/* SWREG3_PICPAR */
#define RKVDEC_SWREG3_PICPAR_SW_Y_HOR_VIRSTRIDE_SHIFT      (0U)
#define RKVDEC_SWREG3_PICPAR_SW_Y_HOR_VIRSTRIDE_MASK       (0x3FFU << RKVDEC_SWREG3_PICPAR_SW_Y_HOR_VIRSTRIDE_SHIFT)    /* 0x000003FF */
#define RKVDEC_SWREG3_PICPAR_SW_UV_HOR_VIRSTRIDE_HIGHBIT_SHIFT (10U)
#define RKVDEC_SWREG3_PICPAR_SW_UV_HOR_VIRSTRIDE_HIGHBIT_MASK (0x1U << RKVDEC_SWREG3_PICPAR_SW_UV_HOR_VIRSTRIDE_HIGHBIT_SHIFT) /* 0x00000400 */
#define RKVDEC_SWREG3_PICPAR_SW_SLICE_NUM_HIGHBIT_SHIFT    (11U)
#define RKVDEC_SWREG3_PICPAR_SW_SLICE_NUM_HIGHBIT_MASK     (0x1U << RKVDEC_SWREG3_PICPAR_SW_SLICE_NUM_HIGHBIT_SHIFT)    /* 0x00000800 */
#define RKVDEC_SWREG3_PICPAR_SW_UV_HOR_VIRSTRIDE_SHIFT     (12U)
#define RKVDEC_SWREG3_PICPAR_SW_UV_HOR_VIRSTRIDE_MASK      (0x1FFU << RKVDEC_SWREG3_PICPAR_SW_UV_HOR_VIRSTRIDE_SHIFT)   /* 0x001FF000 */
#define RKVDEC_SWREG3_PICPAR_SW_SLICE_NUM_LOWBITS_SHIFT    (21U)
#define RKVDEC_SWREG3_PICPAR_SW_SLICE_NUM_LOWBITS_MASK     (0x7FFU << RKVDEC_SWREG3_PICPAR_SW_SLICE_NUM_LOWBITS_SHIFT)  /* 0xFFE00000 */
/* SWREG4_STRM_RLC_BASE */
#define RKVDEC_SWREG4_STRM_RLC_BASE_SW_STRM_RLC_BASE_SHIFT (4U)
#define RKVDEC_SWREG4_STRM_RLC_BASE_SW_STRM_RLC_BASE_MASK  (0xFFFFFFFU << RKVDEC_SWREG4_STRM_RLC_BASE_SW_STRM_RLC_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG5_STREAM_RLC_LEN */
#define RKVDEC_SWREG5_STREAM_RLC_LEN_SW_STREAM_LEN_SHIFT   (0U)
#define RKVDEC_SWREG5_STREAM_RLC_LEN_SW_STREAM_LEN_MASK    (0x7FFFFFFU << RKVDEC_SWREG5_STREAM_RLC_LEN_SW_STREAM_LEN_SHIFT) /* 0x07FFFFFF */
/* SWREG6_CABACTBL_PROB_BASE */
#define RKVDEC_SWREG6_CABACTBL_PROB_BASE_SW_CABACTBL_BASE_SHIFT (4U)
#define RKVDEC_SWREG6_CABACTBL_PROB_BASE_SW_CABACTBL_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG6_CABACTBL_PROB_BASE_SW_CABACTBL_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG7_DECOUT_BASE */
#define RKVDEC_SWREG7_DECOUT_BASE_SW_DECOUT_BASE_SHIFT     (4U)
#define RKVDEC_SWREG7_DECOUT_BASE_SW_DECOUT_BASE_MASK      (0xFFFFFFFU << RKVDEC_SWREG7_DECOUT_BASE_SW_DECOUT_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG8_Y_VIRSTRIDE */
#define RKVDEC_SWREG8_Y_VIRSTRIDE_SW_Y_VIRSTRIDE_SHIFT     (0U)
#define RKVDEC_SWREG8_Y_VIRSTRIDE_SW_Y_VIRSTRIDE_MASK      (0x1FFFFFU << RKVDEC_SWREG8_Y_VIRSTRIDE_SW_Y_VIRSTRIDE_SHIFT) /* 0x001FFFFF */
/* SWREG9_YUV_VIRSTRIDE */
#define RKVDEC_SWREG9_YUV_VIRSTRIDE_SW_YUV_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG9_YUV_VIRSTRIDE_SW_YUV_VIRSTRIDE_MASK  (0x3FFFFFU << RKVDEC_SWREG9_YUV_VIRSTRIDE_SW_YUV_VIRSTRIDE_SHIFT) /* 0x003FFFFF */
/* SWREG10_HEVC_REFER0_BASE */
#define RKVDEC_SWREG10_HEVC_REFER0_BASE_SW_REF_VALID_0_3_SHIFT (0U)
#define RKVDEC_SWREG10_HEVC_REFER0_BASE_SW_REF_VALID_0_3_MASK (0xFU << RKVDEC_SWREG10_HEVC_REFER0_BASE_SW_REF_VALID_0_3_SHIFT) /* 0x0000000F */
#define RKVDEC_SWREG10_HEVC_REFER0_BASE_SW_REFER0_BASE_SHIFT (4U)
#define RKVDEC_SWREG10_HEVC_REFER0_BASE_SW_REFER0_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG10_HEVC_REFER0_BASE_SW_REFER0_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG10_H264_REFER0_BASE */
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_FIELD_SHIFT (0U)
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_FIELD_MASK (0x1U << RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG10_H264_REFER0_BASE_SW_REF0_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REFER0_BASE_SHIFT (4U)
#define RKVDEC_SWREG10_H264_REFER0_BASE_SW_REFER0_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG10_H264_REFER0_BASE_SW_REFER0_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG10_VP9_CPRHEADER_OFFSET */
#define RKVDEC_SWREG10_VP9_CPRHEADER_OFFSET_SW_VP9_CPRHEADER_OFFSET_SHIFT (0U)
#define RKVDEC_SWREG10_VP9_CPRHEADER_OFFSET_SW_VP9_CPRHEADER_OFFSET_MASK (0xFFFFU << RKVDEC_SWREG10_VP9_CPRHEADER_OFFSET_SW_VP9_CPRHEADER_OFFSET_SHIFT) /* 0x0000FFFF */
/* SWREG11_HEVC_REFER1_BASE */
#define RKVDEC_SWREG11_HEVC_REFER1_BASE_SW_REF_VALID_4_7_SHIFT (0U)
#define RKVDEC_SWREG11_HEVC_REFER1_BASE_SW_REF_VALID_4_7_MASK (0xFU << RKVDEC_SWREG11_HEVC_REFER1_BASE_SW_REF_VALID_4_7_SHIFT) /* 0x0000000F */
#define RKVDEC_SWREG11_HEVC_REFER1_BASE_SW_REFER1_BASE_SHIFT (4U)
#define RKVDEC_SWREG11_HEVC_REFER1_BASE_SW_REFER1_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG11_HEVC_REFER1_BASE_SW_REFER1_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG11_H264_REFER1_BASE */
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_FIELD_SHIFT (0U)
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_FIELD_MASK (0x1U << RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG11_H264_REFER1_BASE_SW_REF1_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REFER1_BASE_SHIFT (4U)
#define RKVDEC_SWREG11_H264_REFER1_BASE_SW_REFER1_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG11_H264_REFER1_BASE_SW_REFER1_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG11_VP9_REFERLAST_BASE */
#define RKVDEC_SWREG11_VP9_REFERLAST_BASE_SW_VP9LAST_BASE_SHIFT (4U)
#define RKVDEC_SWREG11_VP9_REFERLAST_BASE_SW_VP9LAST_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG11_VP9_REFERLAST_BASE_SW_VP9LAST_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG12_HEVC_REFER2_BASE */
#define RKVDEC_SWREG12_HEVC_REFER2_BASE_SW_REF_VALID_8_11_SHIFT (0U)
#define RKVDEC_SWREG12_HEVC_REFER2_BASE_SW_REF_VALID_8_11_MASK (0xFU << RKVDEC_SWREG12_HEVC_REFER2_BASE_SW_REF_VALID_8_11_SHIFT) /* 0x0000000F */
#define RKVDEC_SWREG12_HEVC_REFER2_BASE_SW_REFER2_BASE_SHIFT (4U)
#define RKVDEC_SWREG12_HEVC_REFER2_BASE_SW_REFER2_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG12_HEVC_REFER2_BASE_SW_REFER2_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG12_H264_REFER2_BASE */
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_FIELD_SHIFT (0U)
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_FIELD_MASK (0x1U << RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG12_H264_REFER2_BASE_SW_REF2_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REFER2_BASE_SHIFT (4U)
#define RKVDEC_SWREG12_H264_REFER2_BASE_SW_REFER2_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG12_H264_REFER2_BASE_SW_REFER2_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG12_VP9_REFERGOLDEN_BASE */
#define RKVDEC_SWREG12_VP9_REFERGOLDEN_BASE_SW_VP9GOLDEN_BASE_SHIFT (4U)
#define RKVDEC_SWREG12_VP9_REFERGOLDEN_BASE_SW_VP9GOLDEN_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG12_VP9_REFERGOLDEN_BASE_SW_VP9GOLDEN_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG13_HEVC_REFER3_BASE */
#define RKVDEC_SWREG13_HEVC_REFER3_BASE_SW_REF_VALID_12_14_SHIFT (0U)
#define RKVDEC_SWREG13_HEVC_REFER3_BASE_SW_REF_VALID_12_14_MASK (0x7U << RKVDEC_SWREG13_HEVC_REFER3_BASE_SW_REF_VALID_12_14_SHIFT) /* 0x00000007 */
#define RKVDEC_SWREG13_HEVC_REFER3_BASE_SW_REFER3_BASE_SHIFT (4U)
#define RKVDEC_SWREG13_HEVC_REFER3_BASE_SW_REFER3_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG13_HEVC_REFER3_BASE_SW_REFER3_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG13_H264_REFER3_BASE */
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_FIELD_SHIFT (0U)
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_FIELD_MASK (0x1U << RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG13_H264_REFER3_BASE_SW_REF3_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REFER3_BASE_SHIFT (4U)
#define RKVDEC_SWREG13_H264_REFER3_BASE_SW_REFER3_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG13_H264_REFER3_BASE_SW_REFER3_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG13_VP9_REFERALFTER_BASE */
#define RKVDEC_SWREG13_VP9_REFERALFTER_BASE_SW_VP9ALFTER_BASE_SHIFT (4U)
#define RKVDEC_SWREG13_VP9_REFERALFTER_BASE_SW_VP9ALFTER_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG13_VP9_REFERALFTER_BASE_SW_VP9ALFTER_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG14_HEVC_REFER4_BASE */
#define RKVDEC_SWREG14_HEVC_REFER4_BASE_SW_REFER4_BASE_SHIFT (4U)
#define RKVDEC_SWREG14_HEVC_REFER4_BASE_SW_REFER4_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG14_HEVC_REFER4_BASE_SW_REFER4_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG14_H264_REFER4_BASE */
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_FIELD_SHIFT (0U)
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_FIELD_MASK (0x1U << RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG14_H264_REFER4_BASE_SW_REF4_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REFER4_BASE_SHIFT (4U)
#define RKVDEC_SWREG14_H264_REFER4_BASE_SW_REFER4_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG14_H264_REFER4_BASE_SW_REFER4_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG14_VP9COUNT_BASE */
#define RKVDEC_SWREG14_VP9COUNT_BASE_SW_VP9COUNT_UPDATE_EN_SHIFT (0U)
#define RKVDEC_SWREG14_VP9COUNT_BASE_SW_VP9COUNT_UPDATE_EN_MASK (0x1U << RKVDEC_SWREG14_VP9COUNT_BASE_SW_VP9COUNT_UPDATE_EN_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG14_VP9COUNT_BASE_SW_VP9COUNT_BASE_SHIFT (3U)
#define RKVDEC_SWREG14_VP9COUNT_BASE_SW_VP9COUNT_BASE_MASK (0x1FFFFFFFU << RKVDEC_SWREG14_VP9COUNT_BASE_SW_VP9COUNT_BASE_SHIFT) /* 0xFFFFFFF8 */
/* SWREG15_H264_REFER5_BASE */
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_FIELD_SHIFT (0U)
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_FIELD_MASK (0x1U << RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG15_H264_REFER5_BASE_SW_REF5_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REFER5_BASE_SHIFT (4U)
#define RKVDEC_SWREG15_H264_REFER5_BASE_SW_REFER5_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG15_H264_REFER5_BASE_SW_REFER5_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG15_VP9_SEGIDLAST_BASE */
#define RKVDEC_SWREG15_VP9_SEGIDLAST_BASE_SW_VP9SEGIDLAST_BASE_SHIFT (4U)
#define RKVDEC_SWREG15_VP9_SEGIDLAST_BASE_SW_VP9SEGIDLAST_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG15_VP9_SEGIDLAST_BASE_SW_VP9SEGIDLAST_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG15_HEVC_REFER5_BASE */
#define RKVDEC_SWREG15_HEVC_REFER5_BASE_SW_REFER5_BASE_SHIFT (4U)
#define RKVDEC_SWREG15_HEVC_REFER5_BASE_SW_REFER5_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG15_HEVC_REFER5_BASE_SW_REFER5_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG16_H264_REFER6_BASE */
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_FIELD_SHIFT (0U)
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_FIELD_MASK (0x1U << RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG16_H264_REFER6_BASE_SW_REF6_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REFER6_BASE_SHIFT (4U)
#define RKVDEC_SWREG16_H264_REFER6_BASE_SW_REFER6_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG16_H264_REFER6_BASE_SW_REFER6_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG16_VP9_SEGIDCUR_BASE */
#define RKVDEC_SWREG16_VP9_SEGIDCUR_BASE_SW_VP9SEGIDCUR_BASE_SHIFT (4U)
#define RKVDEC_SWREG16_VP9_SEGIDCUR_BASE_SW_VP9SEGIDCUR_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG16_VP9_SEGIDCUR_BASE_SW_VP9SEGIDCUR_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG16_HEVC_REFER6_BASE */
#define RKVDEC_SWREG16_HEVC_REFER6_BASE_SW_REFER6_BASE_SHIFT (4U)
#define RKVDEC_SWREG16_HEVC_REFER6_BASE_SW_REFER6_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG16_HEVC_REFER6_BASE_SW_REFER6_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG17_H264_REFER7_BASE */
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_FIELD_SHIFT (0U)
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_FIELD_MASK (0x1U << RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG17_H264_REFER7_BASE_SW_REF7_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REFER7_BASE_SHIFT (4U)
#define RKVDEC_SWREG17_H264_REFER7_BASE_SW_REFER7_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG17_H264_REFER7_BASE_SW_REFER7_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG17_VP9_FRAME_SIZE_LAST */
#define RKVDEC_SWREG17_VP9_FRAME_SIZE_LAST_SW_FRAMEWIDTH_LAST_SHIFT (0U)
#define RKVDEC_SWREG17_VP9_FRAME_SIZE_LAST_SW_FRAMEWIDTH_LAST_MASK (0xFFFFU << RKVDEC_SWREG17_VP9_FRAME_SIZE_LAST_SW_FRAMEWIDTH_LAST_SHIFT) /* 0x0000FFFF */
#define RKVDEC_SWREG17_VP9_FRAME_SIZE_LAST_SW_FRAMEHEIGHT_LAST_SHIFT (16U)
#define RKVDEC_SWREG17_VP9_FRAME_SIZE_LAST_SW_FRAMEHEIGHT_LAST_MASK (0xFFFFU << RKVDEC_SWREG17_VP9_FRAME_SIZE_LAST_SW_FRAMEHEIGHT_LAST_SHIFT) /* 0xFFFF0000 */
/* SWREG17_HEVC_REFER7_BASE */
#define RKVDEC_SWREG17_HEVC_REFER7_BASE_SW_REFER7_BASE_SHIFT (4U)
#define RKVDEC_SWREG17_HEVC_REFER7_BASE_SW_REFER7_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG17_HEVC_REFER7_BASE_SW_REFER7_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG18_H264_REFER8_BASE */
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_FIELD_SHIFT (0U)
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_FIELD_MASK (0x1U << RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_TOPFILED_USED_SHIFT (1U)
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_TOPFILED_USED_MASK (0x1U << RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_TOPFILED_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG18_H264_REFER8_BASE_SW_REF8_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REFER8_BASE_SHIFT (4U)
#define RKVDEC_SWREG18_H264_REFER8_BASE_SW_REFER8_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG18_H264_REFER8_BASE_SW_REFER8_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG18_VP9_FRAME_SIZE_GOLDEN */
#define RKVDEC_SWREG18_VP9_FRAME_SIZE_GOLDEN_SW_FRAMEWIDTH_GOLDEN_SHIFT (0U)
#define RKVDEC_SWREG18_VP9_FRAME_SIZE_GOLDEN_SW_FRAMEWIDTH_GOLDEN_MASK (0xFFFFU << RKVDEC_SWREG18_VP9_FRAME_SIZE_GOLDEN_SW_FRAMEWIDTH_GOLDEN_SHIFT) /* 0x0000FFFF */
#define RKVDEC_SWREG18_VP9_FRAME_SIZE_GOLDEN_SW_FRAMEHEIGHT_GOLDEN_SHIFT (16U)
#define RKVDEC_SWREG18_VP9_FRAME_SIZE_GOLDEN_SW_FRAMEHEIGHT_GOLDEN_MASK (0xFFFFU << RKVDEC_SWREG18_VP9_FRAME_SIZE_GOLDEN_SW_FRAMEHEIGHT_GOLDEN_SHIFT) /* 0xFFFF0000 */
/* SWREG18_HEVC_REFER8_BASE */
#define RKVDEC_SWREG18_HEVC_REFER8_BASE_SW_REFER8_BASE_SHIFT (4U)
#define RKVDEC_SWREG18_HEVC_REFER8_BASE_SW_REFER8_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG18_HEVC_REFER8_BASE_SW_REFER8_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG19_H264_REFER9_BASE */
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_FIELD_SHIFT (0U)
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_FIELD_MASK (0x1U << RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG19_H264_REFER9_BASE_SW_REF9_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REFER9_BASE_SHIFT (4U)
#define RKVDEC_SWREG19_H264_REFER9_BASE_SW_REFER9_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG19_H264_REFER9_BASE_SW_REFER9_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG19_VP9_FRAME_SIZE_ALTREF */
#define RKVDEC_SWREG19_VP9_FRAME_SIZE_ALTREF_SW_FRAMEWIDTH_ALFTER_SHIFT (0U)
#define RKVDEC_SWREG19_VP9_FRAME_SIZE_ALTREF_SW_FRAMEWIDTH_ALFTER_MASK (0xFFFFU << RKVDEC_SWREG19_VP9_FRAME_SIZE_ALTREF_SW_FRAMEWIDTH_ALFTER_SHIFT) /* 0x0000FFFF */
#define RKVDEC_SWREG19_VP9_FRAME_SIZE_ALTREF_SW_FRAMEHEIGHT_ALFTER_SHIFT (16U)
#define RKVDEC_SWREG19_VP9_FRAME_SIZE_ALTREF_SW_FRAMEHEIGHT_ALFTER_MASK (0xFFFFU << RKVDEC_SWREG19_VP9_FRAME_SIZE_ALTREF_SW_FRAMEHEIGHT_ALFTER_SHIFT) /* 0xFFFF0000 */
/* SWREG19_HEVC_REFER9_BASE */
#define RKVDEC_SWREG19_HEVC_REFER9_BASE_SW_REFER9_BASE_SHIFT (4U)
#define RKVDEC_SWREG19_HEVC_REFER9_BASE_SW_REFER9_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG19_HEVC_REFER9_BASE_SW_REFER9_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG20_H264_REFER10_BASE */
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_FIELD_SHIFT (0U)
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_FIELD_MASK (0x1U << RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG20_H264_REFER10_BASE_SW_REF10_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REFER10_BASE_SHIFT (4U)
#define RKVDEC_SWREG20_H264_REFER10_BASE_SW_REFER10_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG20_H264_REFER10_BASE_SW_REFER10_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG20_VP9_SEGID_GRP0 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID_ABS_DELTA_SHIFT (0U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID_ABS_DELTA_MASK (0x1U << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID_ABS_DELTA_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_REFERINFO_MASK (0x3U << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG20_VP9_SEGID_GRP0_SW_VP9SEGID0_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG20_HEVC_REFER10_BASE */
#define RKVDEC_SWREG20_HEVC_REFER10_BASE_SW_REFER10_BASE_SHIFT (4U)
#define RKVDEC_SWREG20_HEVC_REFER10_BASE_SW_REFER10_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG20_HEVC_REFER10_BASE_SW_REFER10_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG21_H264_REFER11_BASE */
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_FIELD_SHIFT (0U)
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_FIELD_MASK (0x1U << RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG21_H264_REFER11_BASE_SW_REF11_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REFER11_BASE_SHIFT (4U)
#define RKVDEC_SWREG21_H264_REFER11_BASE_SW_REFER11_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG21_H264_REFER11_BASE_SW_REFER11_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG21_VP9_SEGID_GRP1 */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_REFERINFO_MASK (0x3U << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG21_VP9_SEGID_GRP1_SW_VP9SEGID1_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG21_HEVC_REFER11_BASE */
#define RKVDEC_SWREG21_HEVC_REFER11_BASE_SW_REFER11_BASE_SHIFT (4U)
#define RKVDEC_SWREG21_HEVC_REFER11_BASE_SW_REFER11_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG21_HEVC_REFER11_BASE_SW_REFER11_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG22_H264_REFER12_BASE */
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_FIELD_SHIFT (0U)
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_FIELD_MASK (0x1U << RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG22_H264_REFER12_BASE_SW_REF12_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REFER12_BASE_SHIFT (4U)
#define RKVDEC_SWREG22_H264_REFER12_BASE_SW_REFER12_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG22_H264_REFER12_BASE_SW_REFER12_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG22_VP9_SEGID_GRP2 */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_REFERINFO_MASK (0x3U << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG22_VP9_SEGID_GRP2_SW_VP9SEGID2_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG22_HEVC_REFER12_BASE */
#define RKVDEC_SWREG22_HEVC_REFER12_BASE_SW_REFER12_BASE_SHIFT (4U)
#define RKVDEC_SWREG22_HEVC_REFER12_BASE_SW_REFER12_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG22_HEVC_REFER12_BASE_SW_REFER12_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG23_H264_REFER13_BASE */
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_FIELD_SHIFT (0U)
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_FIELD_MASK (0x1U << RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG23_H264_REFER13_BASE_SW_REF13_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REFER13_BASE_SHIFT (4U)
#define RKVDEC_SWREG23_H264_REFER13_BASE_SW_REFER13_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG23_H264_REFER13_BASE_SW_REFER13_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG23_VP9_SEGID_GRP3 */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_REFERINFO_MASK (0x3U << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG23_VP9_SEGID_GRP3_SW_VP9SEGID3_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG23_HEVC_REFER13_BASE */
#define RKVDEC_SWREG23_HEVC_REFER13_BASE_SW_REFER13_BASE_SHIFT (4U)
#define RKVDEC_SWREG23_HEVC_REFER13_BASE_SW_REFER13_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG23_HEVC_REFER13_BASE_SW_REFER13_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG24_H264_REFER14_BASE */
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_FIELD_SHIFT (0U)
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_FIELD_MASK (0x1U << RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG24_H264_REFER14_BASE_SW_REF14_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REFER14_BASE_SHIFT (4U)
#define RKVDEC_SWREG24_H264_REFER14_BASE_SW_REFER14_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG24_H264_REFER14_BASE_SW_REFER14_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG24_VP9_SEGID_GRP4 */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_REFERINFO_MASK (0x3U << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG24_VP9_SEGID_GRP4_SW_VP9SEGID4_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG24_HEVC_REFER14_BASE */
#define RKVDEC_SWREG24_HEVC_REFER14_BASE_SW_REFER14_BASE_SHIFT (4U)
#define RKVDEC_SWREG24_HEVC_REFER14_BASE_SW_REFER14_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG24_HEVC_REFER14_BASE_SW_REFER14_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG25_VP9_SEGID_GRP5 */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_REFERINFO_MASK (0x3U << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG25_VP9_SEGID_GRP5_SW_VP9SEGID5_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG25_REFER0_POC */
#define RKVDEC_SWREG25_REFER0_POC_SW_REFER0_POC_SHIFT      (0U)
#define RKVDEC_SWREG25_REFER0_POC_SW_REFER0_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG25_REFER0_POC_SW_REFER0_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG26_VP9_SEGID_GRP6 */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_REFERINFO_MASK (0x3U << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG26_VP9_SEGID_GRP6_SW_VP9SEGID6_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG26_REFER1_POC */
#define RKVDEC_SWREG26_REFER1_POC_SW_REFER1_POC_SHIFT      (0U)
#define RKVDEC_SWREG26_REFER1_POC_SW_REFER1_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG26_REFER1_POC_SW_REFER1_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG27_VP9_SEGID_GRP7 */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_QP_DELTA_EN_SHIFT (1U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_QP_DELTA_EN_MASK (0x1U << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_QP_DELTA_EN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_QP_DELTA_SHIFT (2U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_QP_DELTA_MASK (0x1FFU << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_QP_DELTA_SHIFT) /* 0x000007FC */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_LOOPFITLER_VALUE_EN_SHIFT (11U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_LOOPFITLER_VALUE_EN_MASK (0x1U << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_LOOPFITLER_VALUE_EN_SHIFT) /* 0x00000800 */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_LOOPFILTER_VALUE_SHIFT (12U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_LOOPFILTER_VALUE_MASK (0x7FU << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_LOOPFILTER_VALUE_SHIFT) /* 0x0007F000 */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_REFERINFO_EN_SHIFT (19U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_REFERINFO_EN_MASK (0x1U << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_REFERINFO_EN_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_REFERINFO_SHIFT (20U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_REFERINFO_MASK (0x3U << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_REFERINFO_SHIFT) /* 0x00300000 */
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_SKIP_EN_SHIFT (22U)
#define RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_SKIP_EN_MASK (0x1U << RKVDEC_SWREG27_VP9_SEGID_GRP7_SW_VP9SEGID7_FRAME_SKIP_EN_SHIFT) /* 0x00400000 */
/* SWREG27_REFER2_POC */
#define RKVDEC_SWREG27_REFER2_POC_SW_REFER2_POC_SHIFT      (0U)
#define RKVDEC_SWREG27_REFER2_POC_SW_REFER2_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG27_REFER2_POC_SW_REFER2_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG28_VP9_CPRHEADER_CONFIG */
#define RKVDEC_SWREG28_VP9_CPRHEADER_CONFIG_SW_VP9_TX_MODE_SHIFT (0U)
#define RKVDEC_SWREG28_VP9_CPRHEADER_CONFIG_SW_VP9_TX_MODE_MASK (0x7U << RKVDEC_SWREG28_VP9_CPRHEADER_CONFIG_SW_VP9_TX_MODE_SHIFT) /* 0x00000007 */
#define RKVDEC_SWREG28_VP9_CPRHEADER_CONFIG_SW_VP9_FRAME_REFERENCE_MODE_SHIFT (3U)
#define RKVDEC_SWREG28_VP9_CPRHEADER_CONFIG_SW_VP9_FRAME_REFERENCE_MODE_MASK (0x3U << RKVDEC_SWREG28_VP9_CPRHEADER_CONFIG_SW_VP9_FRAME_REFERENCE_MODE_SHIFT) /* 0x00000018 */
/* SWREG28_REFER3_POC */
#define RKVDEC_SWREG28_REFER3_POC_SW_REFER3_POC_SHIFT      (0U)
#define RKVDEC_SWREG28_REFER3_POC_SW_REFER3_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG28_REFER3_POC_SW_REFER3_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG29_VP9_LREF_SCALE */
#define RKVDEC_SWREG29_VP9_LREF_SCALE_SW_VP9_LREF_HOR_SCALE_SHIFT (0U)
#define RKVDEC_SWREG29_VP9_LREF_SCALE_SW_VP9_LREF_HOR_SCALE_MASK (0xFFFFU << RKVDEC_SWREG29_VP9_LREF_SCALE_SW_VP9_LREF_HOR_SCALE_SHIFT) /* 0x0000FFFF */
#define RKVDEC_SWREG29_VP9_LREF_SCALE_SW_VP9_LREF_VER_SCALE_SHIFT (16U)
#define RKVDEC_SWREG29_VP9_LREF_SCALE_SW_VP9_LREF_VER_SCALE_MASK (0xFFFFU << RKVDEC_SWREG29_VP9_LREF_SCALE_SW_VP9_LREF_VER_SCALE_SHIFT) /* 0xFFFF0000 */
/* SWREG29_REFER4_POC */
#define RKVDEC_SWREG29_REFER4_POC_SW_REFER4_POC_SHIFT      (0U)
#define RKVDEC_SWREG29_REFER4_POC_SW_REFER4_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG29_REFER4_POC_SW_REFER4_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG30_VP9_GREF_SCALE */
#define RKVDEC_SWREG30_VP9_GREF_SCALE_SW_VP9_GREF_HOR_SCALE_SHIFT (0U)
#define RKVDEC_SWREG30_VP9_GREF_SCALE_SW_VP9_GREF_HOR_SCALE_MASK (0xFFFFU << RKVDEC_SWREG30_VP9_GREF_SCALE_SW_VP9_GREF_HOR_SCALE_SHIFT) /* 0x0000FFFF */
#define RKVDEC_SWREG30_VP9_GREF_SCALE_SW_VP9_GREF_VER_SCALE_SHIFT (16U)
#define RKVDEC_SWREG30_VP9_GREF_SCALE_SW_VP9_GREF_VER_SCALE_MASK (0xFFFFU << RKVDEC_SWREG30_VP9_GREF_SCALE_SW_VP9_GREF_VER_SCALE_SHIFT) /* 0xFFFF0000 */
/* SWREG30_REFER5_POC */
#define RKVDEC_SWREG30_REFER5_POC_SW_REFER5_POC_SHIFT      (0U)
#define RKVDEC_SWREG30_REFER5_POC_SW_REFER5_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG30_REFER5_POC_SW_REFER5_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG31_VP9_AREF_SCALE */
#define RKVDEC_SWREG31_VP9_AREF_SCALE_SW_VP9_AREF_HOR_SCALE_SHIFT (0U)
#define RKVDEC_SWREG31_VP9_AREF_SCALE_SW_VP9_AREF_HOR_SCALE_MASK (0xFFFFU << RKVDEC_SWREG31_VP9_AREF_SCALE_SW_VP9_AREF_HOR_SCALE_SHIFT) /* 0x0000FFFF */
#define RKVDEC_SWREG31_VP9_AREF_SCALE_SW_VP9_AREF_VER_SCALE_SHIFT (16U)
#define RKVDEC_SWREG31_VP9_AREF_SCALE_SW_VP9_AREF_VER_SCALE_MASK (0xFFFFU << RKVDEC_SWREG31_VP9_AREF_SCALE_SW_VP9_AREF_VER_SCALE_SHIFT) /* 0xFFFF0000 */
/* SWREG31_REFER6_POC */
#define RKVDEC_SWREG31_REFER6_POC_SW_REFER6_POC_SHIFT      (0U)
#define RKVDEC_SWREG31_REFER6_POC_SW_REFER6_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG31_REFER6_POC_SW_REFER6_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG32_VP9_REF_DELTAS_LASTFRAME */
#define RKVDEC_SWREG32_VP9_REF_DELTAS_LASTFRAME_SW_VP9_REF_DELTAS_LASTFRAME_SHIFT (0U)
#define RKVDEC_SWREG32_VP9_REF_DELTAS_LASTFRAME_SW_VP9_REF_DELTAS_LASTFRAME_MASK (0xFFFFFFFU << RKVDEC_SWREG32_VP9_REF_DELTAS_LASTFRAME_SW_VP9_REF_DELTAS_LASTFRAME_SHIFT) /* 0x0FFFFFFF */
/* SWREG32_REFER7_POC */
#define RKVDEC_SWREG32_REFER7_POC_SW_REFER7_POC_SHIFT      (0U)
#define RKVDEC_SWREG32_REFER7_POC_SW_REFER7_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG32_REFER7_POC_SW_REFER7_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG33_VP9_INFO_LASTFRAME */
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_MODE_DELTAS_LASTFRAME_SHIFT (0U)
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_MODE_DELTAS_LASTFRAME_MASK (0x3FFFU << RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_MODE_DELTAS_LASTFRAME_SHIFT) /* 0x00003FFF */
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_SEGMENTATION_ENABLE_LSTFRAME_SHIFT (16U)
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_SEGMENTATION_ENABLE_LSTFRAME_MASK (0x1U << RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_SEGMENTATION_ENABLE_LSTFRAME_SHIFT) /* 0x00010000 */
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_SHOW_FRAME_SHIFT (17U)
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_SHOW_FRAME_MASK (0x1U << RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_SHOW_FRAME_SHIFT) /* 0x00020000 */
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_INTRA_ONLY_SHIFT (18U)
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_INTRA_ONLY_MASK (0x1U << RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_INTRA_ONLY_SHIFT) /* 0x00040000 */
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_WIDHHEIGHT_EQCUR_SHIFT (19U)
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_WIDHHEIGHT_EQCUR_MASK (0x1U << RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_LAST_WIDHHEIGHT_EQCUR_SHIFT) /* 0x00080000 */
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_COLOR_SPACE_LASTKEYFRAME_SHIFT (20U)
#define RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_COLOR_SPACE_LASTKEYFRAME_MASK (0x7U << RKVDEC_SWREG33_VP9_INFO_LASTFRAME_SW_VP9_COLOR_SPACE_LASTKEYFRAME_SHIFT) /* 0x00700000 */
/* SWREG33_REFER8_POC */
#define RKVDEC_SWREG33_REFER8_POC_SW_REFER8_POC_SHIFT      (0U)
#define RKVDEC_SWREG33_REFER8_POC_SW_REFER8_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG33_REFER8_POC_SW_REFER8_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG34_VP9_INTERCMD_BASE */
#define RKVDEC_SWREG34_VP9_INTERCMD_BASE_SW_VP9_INTERCMD_BASE_SHIFT (4U)
#define RKVDEC_SWREG34_VP9_INTERCMD_BASE_SW_VP9_INTERCMD_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG34_VP9_INTERCMD_BASE_SW_VP9_INTERCMD_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG34_REFER9_POC */
#define RKVDEC_SWREG34_REFER9_POC_SW_REFER9_POC_SHIFT      (0U)
#define RKVDEC_SWREG34_REFER9_POC_SW_REFER9_POC_MASK       (0xFFFFFFFFU << RKVDEC_SWREG34_REFER9_POC_SW_REFER9_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG35_VP9_INTERCMD_NUM */
#define RKVDEC_SWREG35_VP9_INTERCMD_NUM_SW_VP9_INTERCMD_NUM_SHIFT (0U)
#define RKVDEC_SWREG35_VP9_INTERCMD_NUM_SW_VP9_INTERCMD_NUM_MASK (0xFFFFFFU << RKVDEC_SWREG35_VP9_INTERCMD_NUM_SW_VP9_INTERCMD_NUM_SHIFT) /* 0x00FFFFFF */
/* SWREG35_REFER10_POC */
#define RKVDEC_SWREG35_REFER10_POC_SW_REFER10_POC_SHIFT    (0U)
#define RKVDEC_SWREG35_REFER10_POC_SW_REFER10_POC_MASK     (0xFFFFFFFFU << RKVDEC_SWREG35_REFER10_POC_SW_REFER10_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG36_VP9_LASTTILE_SIZE */
#define RKVDEC_SWREG36_VP9_LASTTILE_SIZE_SW_VP9_LASTTILE_SIZE_SHIFT (0U)
#define RKVDEC_SWREG36_VP9_LASTTILE_SIZE_SW_VP9_LASTTILE_SIZE_MASK (0xFFFFFFU << RKVDEC_SWREG36_VP9_LASTTILE_SIZE_SW_VP9_LASTTILE_SIZE_SHIFT) /* 0x00FFFFFF */
/* SWREG36_REFER11_POC */
#define RKVDEC_SWREG36_REFER11_POC_SW_REFER11_POC_SHIFT    (0U)
#define RKVDEC_SWREG36_REFER11_POC_SW_REFER11_POC_MASK     (0xFFFFFFFFU << RKVDEC_SWREG36_REFER11_POC_SW_REFER11_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG37_VP9_LASTF_HOR_VIRSTRIDE */
#define RKVDEC_SWREG37_VP9_LASTF_HOR_VIRSTRIDE_SW_VP9_LASTFY_HOR_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG37_VP9_LASTF_HOR_VIRSTRIDE_SW_VP9_LASTFY_HOR_VIRSTRIDE_MASK (0x3FFU << RKVDEC_SWREG37_VP9_LASTF_HOR_VIRSTRIDE_SW_VP9_LASTFY_HOR_VIRSTRIDE_SHIFT) /* 0x000003FF */
#define RKVDEC_SWREG37_VP9_LASTF_HOR_VIRSTRIDE_SW_VP9_LASTFUV_HOR_VIRSTRIDE_SHIFT (16U)
#define RKVDEC_SWREG37_VP9_LASTF_HOR_VIRSTRIDE_SW_VP9_LASTFUV_HOR_VIRSTRIDE_MASK (0x3FFU << RKVDEC_SWREG37_VP9_LASTF_HOR_VIRSTRIDE_SW_VP9_LASTFUV_HOR_VIRSTRIDE_SHIFT) /* 0x03FF0000 */
/* SWREG37_REFER12_POC */
#define RKVDEC_SWREG37_REFER12_POC_SW_REFER12_POC_SHIFT    (0U)
#define RKVDEC_SWREG37_REFER12_POC_SW_REFER12_POC_MASK     (0xFFFFFFFFU << RKVDEC_SWREG37_REFER12_POC_SW_REFER12_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE */
#define RKVDEC_SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE_SW_VP9_GOLDENFY_HOR_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE_SW_VP9_GOLDENFY_HOR_VIRSTRIDE_MASK (0x3FFU << RKVDEC_SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE_SW_VP9_GOLDENFY_HOR_VIRSTRIDE_SHIFT) /* 0x000003FF */
#define RKVDEC_SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE_SW_VP9_GOLDENUV_HOR_VIRSTRIDE_SHIFT (16U)
#define RKVDEC_SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE_SW_VP9_GOLDENUV_HOR_VIRSTRIDE_MASK (0x3FFU << RKVDEC_SWREG38_VP9_GOLDENF_HOR_VIRSTRIDE_SW_VP9_GOLDENUV_HOR_VIRSTRIDE_SHIFT) /* 0x03FF0000 */
/* SWREG38_REFER13_POC */
#define RKVDEC_SWREG38_REFER13_POC_SW_REFER13_POC_SHIFT    (0U)
#define RKVDEC_SWREG38_REFER13_POC_SW_REFER13_POC_MASK     (0xFFFFFFFFU << RKVDEC_SWREG38_REFER13_POC_SW_REFER13_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE */
#define RKVDEC_SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE_SW_VP9_ALTREFFY_HOR_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE_SW_VP9_ALTREFFY_HOR_VIRSTRIDE_MASK (0x3FFU << RKVDEC_SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE_SW_VP9_ALTREFFY_HOR_VIRSTRIDE_SHIFT) /* 0x000003FF */
#define RKVDEC_SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE_SW_VP9_ALTREFFUV_HOR_VIRSTRIDE_SHIFT (16U)
#define RKVDEC_SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE_SW_VP9_ALTREFFUV_HOR_VIRSTRIDE_MASK (0x3FFU << RKVDEC_SWREG39_VP9_ALTREFF_HOR_VIRSTRIDE_SW_VP9_ALTREFFUV_HOR_VIRSTRIDE_SHIFT) /* 0x03FF0000 */
/* SWREG39_REFER14_POC */
#define RKVDEC_SWREG39_REFER14_POC_SW_REFER14_POC_SHIFT    (0U)
#define RKVDEC_SWREG39_REFER14_POC_SW_REFER14_POC_MASK     (0xFFFFFFFFU << RKVDEC_SWREG39_REFER14_POC_SW_REFER14_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG40_CUR_POC */
#define RKVDEC_SWREG40_CUR_POC_SW_CUR_POC_SHIFT            (0U)
#define RKVDEC_SWREG40_CUR_POC_SW_CUR_POC_MASK             (0xFFFFFFFFU << RKVDEC_SWREG40_CUR_POC_SW_CUR_POC_SHIFT)     /* 0xFFFFFFFF */
/* SWREG41_RLCWRITE_BASE */
#define RKVDEC_SWREG41_RLCWRITE_BASE_SW_RLCWRITE_BASE_SHIFT (3U)
#define RKVDEC_SWREG41_RLCWRITE_BASE_SW_RLCWRITE_BASE_MASK (0x1FFFFFFFU << RKVDEC_SWREG41_RLCWRITE_BASE_SW_RLCWRITE_BASE_SHIFT) /* 0xFFFFFFF8 */
/* SWREG42_PPS_BASE */
#define RKVDEC_SWREG42_PPS_BASE_SW_PPS_BASE_SHIFT          (4U)
#define RKVDEC_SWREG42_PPS_BASE_SW_PPS_BASE_MASK           (0xFFFFFFFU << RKVDEC_SWREG42_PPS_BASE_SW_PPS_BASE_SHIFT)    /* 0xFFFFFFF0 */
/* SWREG43_RPS_BASE */
#define RKVDEC_SWREG43_RPS_BASE_SW_RPS_BASE_SHIFT          (4U)
#define RKVDEC_SWREG43_RPS_BASE_SW_RPS_BASE_MASK           (0xFFFFFFFU << RKVDEC_SWREG43_RPS_BASE_SW_RPS_BASE_SHIFT)    /* 0xFFFFFFF0 */
/* SWREG44_STRMD_ERROR_EN */
#define RKVDEC_SWREG44_STRMD_ERROR_EN_SW_STRMD_ERROR_E_SHIFT (0U)
#define RKVDEC_SWREG44_STRMD_ERROR_EN_SW_STRMD_ERROR_E_MASK (0xFFFFFFFFU << RKVDEC_SWREG44_STRMD_ERROR_EN_SW_STRMD_ERROR_E_SHIFT) /* 0xFFFFFFFF */
/* SWREG45_STRMD_ERROR_STATUS */
#define RKVDEC_SWREG45_STRMD_ERROR_STATUS_SW_STRMD_ERROR_STATUS_SHIFT (0U)
#define RKVDEC_SWREG45_STRMD_ERROR_STATUS_SW_STRMD_ERROR_STATUS_MASK (0xFFFFFFFU << RKVDEC_SWREG45_STRMD_ERROR_STATUS_SW_STRMD_ERROR_STATUS_SHIFT) /* 0x0FFFFFFF */
#define RKVDEC_SWREG45_STRMD_ERROR_STATUS_SW_COLMV_ERROR_REF_PICIDX_SHIFT (28U)
#define RKVDEC_SWREG45_STRMD_ERROR_STATUS_SW_COLMV_ERROR_REF_PICIDX_MASK (0xFU << RKVDEC_SWREG45_STRMD_ERROR_STATUS_SW_COLMV_ERROR_REF_PICIDX_SHIFT) /* 0xF0000000 */
/* SWREG45_VP9_ERROR_INFO0 */
#define RKVDEC_SWREG45_VP9_ERROR_INFO0_VP9_ERROR_INFO0_SHIFT (0U)
#define RKVDEC_SWREG45_VP9_ERROR_INFO0_VP9_ERROR_INFO0_MASK (0xFFFFFFFFU << RKVDEC_SWREG45_VP9_ERROR_INFO0_VP9_ERROR_INFO0_SHIFT) /* 0xFFFFFFFF */
/* SWREG46_STRMD_ERROR_CTU */
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STRMD_ERROR_CTU_XOFFSET_SHIFT (0U)
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STRMD_ERROR_CTU_XOFFSET_MASK (0xFFU << RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STRMD_ERROR_CTU_XOFFSET_SHIFT) /* 0x000000FF */
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STRMD_ERROR_CTU_YOFFSET_SHIFT (8U)
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STRMD_ERROR_CTU_YOFFSET_MASK (0xFFU << RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STRMD_ERROR_CTU_YOFFSET_SHIFT) /* 0x0000FF00 */
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STREAMFIFO_SPACE2FULL_SHIFT (16U)
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STREAMFIFO_SPACE2FULL_MASK (0x7FU << RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_STREAMFIFO_SPACE2FULL_SHIFT) /* 0x007F0000 */
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_VP9_ERROR_CTU0_EN_SHIFT (24U)
#define RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_VP9_ERROR_CTU0_EN_MASK (0x1U << RKVDEC_SWREG46_STRMD_ERROR_CTU_SW_VP9_ERROR_CTU0_EN_SHIFT) /* 0x01000000 */
/* SWREG47_SAO_CTU_POSITION */
#define RKVDEC_SWREG47_SAO_CTU_POSITION_SW_SAOWR_XOFFET_SHIFT (0U)
#define RKVDEC_SWREG47_SAO_CTU_POSITION_SW_SAOWR_XOFFET_MASK (0x1FFU << RKVDEC_SWREG47_SAO_CTU_POSITION_SW_SAOWR_XOFFET_SHIFT) /* 0x000001FF */
#define RKVDEC_SWREG47_SAO_CTU_POSITION_SW_SAOWR_YOFFSET_SHIFT (16U)
#define RKVDEC_SWREG47_SAO_CTU_POSITION_SW_SAOWR_YOFFSET_MASK (0x3FFU << RKVDEC_SWREG47_SAO_CTU_POSITION_SW_SAOWR_YOFFSET_SHIFT) /* 0x03FF0000 */
/* SWREG48_H264_REFER15_BASE */
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_FIELD_SHIFT (0U)
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_FIELD_MASK (0x1U << RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_FIELD_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_TOPFIELD_USED_SHIFT (1U)
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_TOPFIELD_USED_MASK (0x1U << RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_TOPFIELD_USED_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_BOTFIELD_USED_SHIFT (2U)
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_BOTFIELD_USED_MASK (0x1U << RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_BOTFIELD_USED_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_COLMV_USE_FLAG_SHIFT (3U)
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_COLMV_USE_FLAG_MASK (0x1U << RKVDEC_SWREG48_H264_REFER15_BASE_SW_REF15_COLMV_USE_FLAG_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REFER15_BASE_SHIFT (4U)
#define RKVDEC_SWREG48_H264_REFER15_BASE_SW_REFER15_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG48_H264_REFER15_BASE_SW_REFER15_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG48_VP9_LAST_YSTRIDE */
#define RKVDEC_SWREG48_VP9_LAST_YSTRIDE_SW_VP9_LASTFY_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG48_VP9_LAST_YSTRIDE_SW_VP9_LASTFY_VIRSTRIDE_MASK (0x1FFFFFU << RKVDEC_SWREG48_VP9_LAST_YSTRIDE_SW_VP9_LASTFY_VIRSTRIDE_SHIFT) /* 0x001FFFFF */
/* SWREG49_H264_REFER15_POC */
#define RKVDEC_SWREG49_H264_REFER15_POC_SW_REFER15_POC_SHIFT (0U)
#define RKVDEC_SWREG49_H264_REFER15_POC_SW_REFER15_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG49_H264_REFER15_POC_SW_REFER15_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG49_VP9_GOLDEN_YSTRIDE */
#define RKVDEC_SWREG49_VP9_GOLDEN_YSTRIDE_SW_VP9_GOLDENY_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG49_VP9_GOLDEN_YSTRIDE_SW_VP9_GOLDENY_VIRSTRIDE_MASK (0x1FFFFFU << RKVDEC_SWREG49_VP9_GOLDEN_YSTRIDE_SW_VP9_GOLDENY_VIRSTRIDE_SHIFT) /* 0x001FFFFF */
/* SWREG50_H264_REFER16_POC */
#define RKVDEC_SWREG50_H264_REFER16_POC_SW_REFER16_POC_SHIFT (0U)
#define RKVDEC_SWREG50_H264_REFER16_POC_SW_REFER16_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG50_H264_REFER16_POC_SW_REFER16_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG50_VP9_ALTREFY_YSTRIDE */
#define RKVDEC_SWREG50_VP9_ALTREFY_YSTRIDE_SW_VP9_ALTREFY_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG50_VP9_ALTREFY_YSTRIDE_SW_VP9_ALTREFY_VIRSTRIDE_MASK (0x1FFFFFU << RKVDEC_SWREG50_VP9_ALTREFY_YSTRIDE_SW_VP9_ALTREFY_VIRSTRIDE_SHIFT) /* 0x001FFFFF */
/* SWREG51_H264_REFER17_POC */
#define RKVDEC_SWREG51_H264_REFER17_POC_SW_REFER17_POC_SHIFT (0U)
#define RKVDEC_SWREG51_H264_REFER17_POC_SW_REFER17_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG51_H264_REFER17_POC_SW_REFER17_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG51_VP9_LASTREF_YUVSTRIDE */
#define RKVDEC_SWREG51_VP9_LASTREF_YUVSTRIDE_SW_VP9_LASTREF_YUV_VIRSTRIDE_SHIFT (0U)
#define RKVDEC_SWREG51_VP9_LASTREF_YUVSTRIDE_SW_VP9_LASTREF_YUV_VIRSTRIDE_MASK (0x3FFFFFU << RKVDEC_SWREG51_VP9_LASTREF_YUVSTRIDE_SW_VP9_LASTREF_YUV_VIRSTRIDE_SHIFT) /* 0x003FFFFF */
/* SWREG52_VP9_REFCOLMV_BASE */
#define RKVDEC_SWREG52_VP9_REFCOLMV_BASE_SW_VP9_REFCOLMV_BASE_SHIFT (4U)
#define RKVDEC_SWREG52_VP9_REFCOLMV_BASE_SW_VP9_REFCOLMV_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG52_VP9_REFCOLMV_BASE_SW_VP9_REFCOLMV_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG52_H264_REFER18_POC */
#define RKVDEC_SWREG52_H264_REFER18_POC_SW_REFER18_POC_SHIFT (0U)
#define RKVDEC_SWREG52_H264_REFER18_POC_SW_REFER18_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG52_H264_REFER18_POC_SW_REFER18_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG53_H264_REFER19_POC */
#define RKVDEC_SWREG53_H264_REFER19_POC_SW_REFER19_POC_SHIFT (0U)
#define RKVDEC_SWREG53_H264_REFER19_POC_SW_REFER19_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG53_H264_REFER19_POC_SW_REFER19_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG54_H264_REFER20_POC */
#define RKVDEC_SWREG54_H264_REFER20_POC_SW_REFER20_POC_SHIFT (0U)
#define RKVDEC_SWREG54_H264_REFER20_POC_SW_REFER20_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG54_H264_REFER20_POC_SW_REFER20_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG55_H264_REFER21_POC */
#define RKVDEC_SWREG55_H264_REFER21_POC_SW_REFER21_POC_SHIFT (0U)
#define RKVDEC_SWREG55_H264_REFER21_POC_SW_REFER21_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG55_H264_REFER21_POC_SW_REFER21_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG56_H264_REFER22_POC */
#define RKVDEC_SWREG56_H264_REFER22_POC_SW_REFER22_POC_SHIFT (0U)
#define RKVDEC_SWREG56_H264_REFER22_POC_SW_REFER22_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG56_H264_REFER22_POC_SW_REFER22_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG57_H264_REFER23_POC */
#define RKVDEC_SWREG57_H264_REFER23_POC_SW_REFER23_POC_SHIFT (0U)
#define RKVDEC_SWREG57_H264_REFER23_POC_SW_REFER23_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG57_H264_REFER23_POC_SW_REFER23_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG58_H264_REFER24_POC */
#define RKVDEC_SWREG58_H264_REFER24_POC_SW_REFER24_POC_SHIFT (0U)
#define RKVDEC_SWREG58_H264_REFER24_POC_SW_REFER24_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG58_H264_REFER24_POC_SW_REFER24_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG59_H264_REFER25_POC */
#define RKVDEC_SWREG59_H264_REFER25_POC_SW_REFER25_POC_SHIFT (0U)
#define RKVDEC_SWREG59_H264_REFER25_POC_SW_REFER25_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG59_H264_REFER25_POC_SW_REFER25_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG60_H264_REFER26_POC */
#define RKVDEC_SWREG60_H264_REFER26_POC_SW_REFER26_POC_SHIFT (0U)
#define RKVDEC_SWREG60_H264_REFER26_POC_SW_REFER26_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG60_H264_REFER26_POC_SW_REFER26_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG61_H264_REFER27_POC */
#define RKVDEC_SWREG61_H264_REFER27_POC_SW_REFER27_POC_SHIFT (0U)
#define RKVDEC_SWREG61_H264_REFER27_POC_SW_REFER27_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG61_H264_REFER27_POC_SW_REFER27_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG62_H264_REFER28_POC */
#define RKVDEC_SWREG62_H264_REFER28_POC_SW_REFER28_POC_SHIFT (0U)
#define RKVDEC_SWREG62_H264_REFER28_POC_SW_REFER28_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG62_H264_REFER28_POC_SW_REFER28_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG63_H264_REFER29_POC */
#define RKVDEC_SWREG63_H264_REFER29_POC_SW_REFER29_POC_SHIFT (0U)
#define RKVDEC_SWREG63_H264_REFER29_POC_SW_REFER29_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG63_H264_REFER29_POC_SW_REFER29_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG64_PERFORMANCE_CYCLE */
#define RKVDEC_SWREG64_PERFORMANCE_CYCLE_SW_PERFORMANCE_CYCLE_SHIFT (0U)
#define RKVDEC_SWREG64_PERFORMANCE_CYCLE_SW_PERFORMANCE_CYCLE_MASK (0xFFFFFFFFU << RKVDEC_SWREG64_PERFORMANCE_CYCLE_SW_PERFORMANCE_CYCLE_SHIFT) /* 0xFFFFFFFF */
/* SWREG65_AXI_DDR_RDATA */
#define RKVDEC_SWREG65_AXI_DDR_RDATA_SW_AXI_DDR_RDATA_SHIFT (0U)
#define RKVDEC_SWREG65_AXI_DDR_RDATA_SW_AXI_DDR_RDATA_MASK (0xFFFFFFFFU << RKVDEC_SWREG65_AXI_DDR_RDATA_SW_AXI_DDR_RDATA_SHIFT) /* 0xFFFFFFFF */
/* SWREG66_AXI_DDR_WDATA */
#define RKVDEC_SWREG66_AXI_DDR_WDATA_SW_AXI_DDR_WDATA_SHIFT (0U)
#define RKVDEC_SWREG66_AXI_DDR_WDATA_SW_AXI_DDR_WDATA_MASK (0xFFFFFFFFU << RKVDEC_SWREG66_AXI_DDR_WDATA_SW_AXI_DDR_WDATA_SHIFT) /* 0xFFFFFFFF */
/* SWREG67_FPGADEBUG_RESET */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_BUSIFD_RESETN_SHIFT (0U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_BUSIFD_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_BUSIFD_RESETN_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_CABAC_RESETN_SHIFT (1U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_CABAC_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_CABAC_RESETN_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_DEC_CTRL_RESETN_SHIFT (2U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_DEC_CTRL_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_DEC_CTRL_RESETN_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_TRANSD_RESETN_SHIFT (3U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_TRANSD_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_TRANSD_RESETN_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_INTRA_RESETN_SHIFT (4U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_INTRA_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_INTRA_RESETN_SHIFT) /* 0x00000010 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_INTER_RESETN_SHIFT (5U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_INTER_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_INTER_RESETN_SHIFT) /* 0x00000020 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_RECON_RESETN_SHIFT (6U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_RECON_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_RECON_RESETN_SHIFT) /* 0x00000040 */
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_FILER_RESETN_SHIFT (7U)
#define RKVDEC_SWREG67_FPGADEBUG_RESET_SW_FILER_RESETN_MASK (0x1U << RKVDEC_SWREG67_FPGADEBUG_RESET_SW_FILER_RESETN_SHIFT) /* 0x00000080 */
/* SWREG68_PERFORMANCE_SEL */
#define RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT0_SEL_SHIFT (0U)
#define RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT0_SEL_MASK  (0x3FU << RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT0_SEL_SHIFT) /* 0x0000003F */
#define RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT1_SEL_SHIFT (8U)
#define RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT1_SEL_MASK  (0x3FU << RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT1_SEL_SHIFT) /* 0x00003F00 */
#define RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT2_SEL_SHIFT (16U)
#define RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT2_SEL_MASK  (0x3FU << RKVDEC_SWREG68_PERFORMANCE_SEL_PERF_CNT2_SEL_SHIFT) /* 0x003F0000 */
/* SWREG69_PERFORMANCE_CNT0 */
#define RKVDEC_SWREG69_PERFORMANCE_CNT0_PERF_CNT0_SHIFT    (0U)
#define RKVDEC_SWREG69_PERFORMANCE_CNT0_PERF_CNT0_MASK     (0xFFFFFFFFU << RKVDEC_SWREG69_PERFORMANCE_CNT0_PERF_CNT0_SHIFT) /* 0xFFFFFFFF */
/* SWREG70_PERFORMANCE_CNT1 */
#define RKVDEC_SWREG70_PERFORMANCE_CNT1_PERF_CNT1_SHIFT    (0U)
#define RKVDEC_SWREG70_PERFORMANCE_CNT1_PERF_CNT1_MASK     (0xFFFFFFFFU << RKVDEC_SWREG70_PERFORMANCE_CNT1_PERF_CNT1_SHIFT) /* 0xFFFFFFFF */
/* SWREG71_PERFORMANCE_CNT2 */
#define RKVDEC_SWREG71_PERFORMANCE_CNT2_PERF_CNT2_SHIFT    (0U)
#define RKVDEC_SWREG71_PERFORMANCE_CNT2_PERF_CNT2_MASK     (0xFFFFFFFFU << RKVDEC_SWREG71_PERFORMANCE_CNT2_PERF_CNT2_SHIFT) /* 0xFFFFFFFF */
/* SWREG72_H264_REFER30_POC */
#define RKVDEC_SWREG72_H264_REFER30_POC_SW_REFER30_POC_SHIFT (0U)
#define RKVDEC_SWREG72_H264_REFER30_POC_SW_REFER30_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG72_H264_REFER30_POC_SW_REFER30_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG73_H264_REFER31_POC */
#define RKVDEC_SWREG73_H264_REFER31_POC_SW_REFER31_POC_SHIFT (0U)
#define RKVDEC_SWREG73_H264_REFER31_POC_SW_REFER31_POC_MASK (0xFFFFFFFFU << RKVDEC_SWREG73_H264_REFER31_POC_SW_REFER31_POC_SHIFT) /* 0xFFFFFFFF */
/* SWREG74_H264_CUR_POC1 */
#define RKVDEC_SWREG74_H264_CUR_POC1_SW_H264_CUR_POC1_SHIFT (0U)
#define RKVDEC_SWREG74_H264_CUR_POC1_SW_H264_CUR_POC1_MASK (0xFFFFFFFFU << RKVDEC_SWREG74_H264_CUR_POC1_SW_H264_CUR_POC1_SHIFT) /* 0xFFFFFFFF */
/* SWREG75_H264_ERRORINFO_BASE */
#define RKVDEC_SWREG75_H264_ERRORINFO_BASE_SW_ERRORINFO_BASE_SHIFT (3U)
#define RKVDEC_SWREG75_H264_ERRORINFO_BASE_SW_ERRORINFO_BASE_MASK (0x1FFFFFFFU << RKVDEC_SWREG75_H264_ERRORINFO_BASE_SW_ERRORINFO_BASE_SHIFT) /* 0xFFFFFFF8 */
/* SWREG75_VP9_ERROR_INFO1 */
#define RKVDEC_SWREG75_VP9_ERROR_INFO1_VP9_ERROR_INFO1_SHIFT (0U)
#define RKVDEC_SWREG75_VP9_ERROR_INFO1_VP9_ERROR_INFO1_MASK (0xFFFFFFFFU << RKVDEC_SWREG75_VP9_ERROR_INFO1_VP9_ERROR_INFO1_SHIFT) /* 0xFFFFFFFF */
/* SWREG76_H264_ERRORINFO_NUM */
#define RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_SLICEDEC_NUM_SHIFT (0U)
#define RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_SLICEDEC_NUM_MASK (0x3FFFU << RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_SLICEDEC_NUM_SHIFT) /* 0x00003FFF */
#define RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_STRMD_DETECT_ERROR_FLAG_SHIFT (15U)
#define RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_STRMD_DETECT_ERROR_FLAG_MASK (0x1U << RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_STRMD_DETECT_ERROR_FLAG_SHIFT) /* 0x00008000 */
#define RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_ERROR_PACKET_NUM_SHIFT (16U)
#define RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_ERROR_PACKET_NUM_MASK (0x3FFFU << RKVDEC_SWREG76_H264_ERRORINFO_NUM_SW_ERROR_PACKET_NUM_SHIFT) /* 0x3FFF0000 */
/* SWREG76_VP9_ERROR_CTU1 */
#define RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_X_SHIFT (0U)
#define RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_X_MASK (0x3FU << RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_X_SHIFT) /* 0x0000003F */
#define RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_Y_SHIFT (8U)
#define RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_Y_MASK (0x3FU << RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_Y_SHIFT) /* 0x00003F00 */
#define RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_EN_SHIFT (15U)
#define RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_EN_MASK (0x1U << RKVDEC_SWREG76_VP9_ERROR_CTU1_VP9_ERROR_CTU1_EN_SHIFT) /* 0x00008000 */
/* SWREG77_H264_ERROR_E */
#define RKVDEC_SWREG77_H264_ERROR_E_SW_H264_ERROR_EN_HIGHBITS_SHIFT (0U)
#define RKVDEC_SWREG77_H264_ERROR_E_SW_H264_ERROR_EN_HIGHBITS_MASK (0xFFFFFFFU << RKVDEC_SWREG77_H264_ERROR_E_SW_H264_ERROR_EN_HIGHBITS_SHIFT) /* 0x0FFFFFFF */
#define RKVDEC_SWREG77_H264_ERROR_E_SW_STRMD_ERROR_EN_SHIFT (28U)
#define RKVDEC_SWREG77_H264_ERROR_E_SW_STRMD_ERROR_EN_MASK (0x3U << RKVDEC_SWREG77_H264_ERROR_E_SW_STRMD_ERROR_EN_SHIFT) /* 0x30000000 */
/* SWREG78_COLMV_CUR_BASE */
#define RKVDEC_SWREG78_COLMV_CUR_BASE_SW_COLMV_CUR_BASE_SHIFT (4U)
#define RKVDEC_SWREG78_COLMV_CUR_BASE_SW_COLMV_CUR_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG78_COLMV_CUR_BASE_SW_COLMV_CUR_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG79_COLMV_REF0_BASE */
#define RKVDEC_SWREG79_COLMV_REF0_BASE_SW_COLMV_REF0_BASE_SHIFT (4U)
#define RKVDEC_SWREG79_COLMV_REF0_BASE_SW_COLMV_REF0_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG79_COLMV_REF0_BASE_SW_COLMV_REF0_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG80_COLMV_REF1_BASE */
#define RKVDEC_SWREG80_COLMV_REF1_BASE_SW_COLMV_REF1_BASE_SHIFT (4U)
#define RKVDEC_SWREG80_COLMV_REF1_BASE_SW_COLMV_REF1_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG80_COLMV_REF1_BASE_SW_COLMV_REF1_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG81_COLMV_REF2_BASE */
#define RKVDEC_SWREG81_COLMV_REF2_BASE_SW_COLMV_REF2_BASE_SHIFT (4U)
#define RKVDEC_SWREG81_COLMV_REF2_BASE_SW_COLMV_REF2_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG81_COLMV_REF2_BASE_SW_COLMV_REF2_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG82_COLMV_REF3_BASE */
#define RKVDEC_SWREG82_COLMV_REF3_BASE_SW_COLMV_REF3_BASE_SHIFT (4U)
#define RKVDEC_SWREG82_COLMV_REF3_BASE_SW_COLMV_REF3_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG82_COLMV_REF3_BASE_SW_COLMV_REF3_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG83_COLMV_REF4_BASE */
#define RKVDEC_SWREG83_COLMV_REF4_BASE_SW_COLMV_REF4_BASE_SHIFT (4U)
#define RKVDEC_SWREG83_COLMV_REF4_BASE_SW_COLMV_REF4_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG83_COLMV_REF4_BASE_SW_COLMV_REF4_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG84_COLMV_REF5_BASE */
#define RKVDEC_SWREG84_COLMV_REF5_BASE_SW_COLMV_REF5_BASE_SHIFT (4U)
#define RKVDEC_SWREG84_COLMV_REF5_BASE_SW_COLMV_REF5_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG84_COLMV_REF5_BASE_SW_COLMV_REF5_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG85_COLMV_REF6_BASE */
#define RKVDEC_SWREG85_COLMV_REF6_BASE_SW_COLMV_REF6_BASE_SHIFT (4U)
#define RKVDEC_SWREG85_COLMV_REF6_BASE_SW_COLMV_REF6_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG85_COLMV_REF6_BASE_SW_COLMV_REF6_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG86_COLMV_REF7_BASE */
#define RKVDEC_SWREG86_COLMV_REF7_BASE_SW_COLMV_REF7_BASE_SHIFT (4U)
#define RKVDEC_SWREG86_COLMV_REF7_BASE_SW_COLMV_REF7_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG86_COLMV_REF7_BASE_SW_COLMV_REF7_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG87_COLMV_REF8_BASE */
#define RKVDEC_SWREG87_COLMV_REF8_BASE_SW_COLMV_REF8_BASE_SHIFT (4U)
#define RKVDEC_SWREG87_COLMV_REF8_BASE_SW_COLMV_REF8_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG87_COLMV_REF8_BASE_SW_COLMV_REF8_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG88_COLMV_REF9_BASE */
#define RKVDEC_SWREG88_COLMV_REF9_BASE_SW_COLMV_REF9_BASE_SHIFT (4U)
#define RKVDEC_SWREG88_COLMV_REF9_BASE_SW_COLMV_REF9_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG88_COLMV_REF9_BASE_SW_COLMV_REF9_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG89_COLMV_REF10_BASE */
#define RKVDEC_SWREG89_COLMV_REF10_BASE_SW_COLMV_REF10_BASE_SHIFT (4U)
#define RKVDEC_SWREG89_COLMV_REF10_BASE_SW_COLMV_REF10_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG89_COLMV_REF10_BASE_SW_COLMV_REF10_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG90_COLMV_REF11_BASE */
#define RKVDEC_SWREG90_COLMV_REF11_BASE_SW_COLMV_REF11_BASE_SHIFT (4U)
#define RKVDEC_SWREG90_COLMV_REF11_BASE_SW_COLMV_REF11_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG90_COLMV_REF11_BASE_SW_COLMV_REF11_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG91_COLMV_REF12_BASE */
#define RKVDEC_SWREG91_COLMV_REF12_BASE_SW_COLMV_REF12_BASE_SHIFT (4U)
#define RKVDEC_SWREG91_COLMV_REF12_BASE_SW_COLMV_REF12_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG91_COLMV_REF12_BASE_SW_COLMV_REF12_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG92_COLMV_REF13_BASE */
#define RKVDEC_SWREG92_COLMV_REF13_BASE_SW_COLMV_REF13_BASE_SHIFT (4U)
#define RKVDEC_SWREG92_COLMV_REF13_BASE_SW_COLMV_REF13_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG92_COLMV_REF13_BASE_SW_COLMV_REF13_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG93_COLMV_REF14_BASE */
#define RKVDEC_SWREG93_COLMV_REF14_BASE_SW_COLMV_REF14_BASE_SHIFT (4U)
#define RKVDEC_SWREG93_COLMV_REF14_BASE_SW_COLMV_REF14_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG93_COLMV_REF14_BASE_SW_COLMV_REF14_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG94_COLMV_REF15_BASE */
#define RKVDEC_SWREG94_COLMV_REF15_BASE_SW_COLMV_REF15_BASE_SHIFT (4U)
#define RKVDEC_SWREG94_COLMV_REF15_BASE_SW_COLMV_REF15_BASE_MASK (0xFFFFFFFU << RKVDEC_SWREG94_COLMV_REF15_BASE_SW_COLMV_REF15_BASE_SHIFT) /* 0xFFFFFFF0 */
/* SWREG95_SCANLIST_ADDR */
#define RKVDEC_SWREG95_SCANLIST_ADDR_SCANLIST_ADDR_VALID_EN_SHIFT (0U)
#define RKVDEC_SWREG95_SCANLIST_ADDR_SCANLIST_ADDR_VALID_EN_MASK (0x1U << RKVDEC_SWREG95_SCANLIST_ADDR_SCANLIST_ADDR_VALID_EN_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG95_SCANLIST_ADDR_SCANLIST_ADDR_SHIFT   (4U)
#define RKVDEC_SWREG95_SCANLIST_ADDR_SCANLIST_ADDR_MASK    (0xFFFFFFFU << RKVDEC_SWREG95_SCANLIST_ADDR_SCANLIST_ADDR_SHIFT) /* 0xFFFFFFF0 */
/* SWREG100_PERF_LATENCY_CTRL0 */
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_WORK_E_SHIFT (0U)
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_WORK_E_MASK (0x1U << RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_WORK_E_SHIFT) /* 0x00000001 */
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_CLR_E_SHIFT (1U)
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_CLR_E_MASK (0x1U << RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_CLR_E_SHIFT) /* 0x00000002 */
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_FRM_TYPE_SHIFT (2U)
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_FRM_TYPE_MASK (0x1U << RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_PERF_FRM_TYPE_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_CNT_TYPE_SHIFT (3U)
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_CNT_TYPE_MASK (0x1U << RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_AXI_CNT_TYPE_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_RD_LATENCY_ID_SHIFT (4U)
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_RD_LATENCY_ID_MASK (0xFU << RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_RD_LATENCY_ID_SHIFT) /* 0x000000F0 */
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_RD_LATENCY_THR_SHIFT (8U)
#define RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_RD_LATENCY_THR_MASK (0xFFFU << RKVDEC_SWREG100_PERF_LATENCY_CTRL0_SW_RD_LATENCY_THR_SHIFT) /* 0x000FFF00 */
/* SWREG101_PERF_LATENCY_CTRL1 */
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_ADDR_ALIGN_TYPE_SHIFT (0U)
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_ADDR_ALIGN_TYPE_MASK (0x3U << RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_ADDR_ALIGN_TYPE_SHIFT) /* 0x00000003 */
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AR_CNT_ID_TYPE_SHIFT (2U)
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AR_CNT_ID_TYPE_MASK (0x1U << RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AR_CNT_ID_TYPE_SHIFT) /* 0x00000004 */
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AW_CNT_ID_TYPE_SHIFT (3U)
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AW_CNT_ID_TYPE_MASK (0x1U << RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AW_CNT_ID_TYPE_SHIFT) /* 0x00000008 */
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AR_COUNT_ID_SHIFT (4U)
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AR_COUNT_ID_MASK (0xFU << RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AR_COUNT_ID_SHIFT) /* 0x000000F0 */
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AW_COUNT_ID_SHIFT (8U)
#define RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AW_COUNT_ID_MASK (0xFU << RKVDEC_SWREG101_PERF_LATENCY_CTRL1_SW_AW_COUNT_ID_SHIFT) /* 0x00000F00 */
/* SWREG102_PERF_RD_MAX_LATENCY_NUM0 */
#define RKVDEC_SWREG102_PERF_RD_MAX_LATENCY_NUM0_RD_MAX_LATENCY_NUM_CH0_SHIFT (0U)
#define RKVDEC_SWREG102_PERF_RD_MAX_LATENCY_NUM0_RD_MAX_LATENCY_NUM_CH0_MASK (0xFFFFU << RKVDEC_SWREG102_PERF_RD_MAX_LATENCY_NUM0_RD_MAX_LATENCY_NUM_CH0_SHIFT) /* 0x0000FFFF */
/* SWREG103_PERF_RD_LATENCY_SAMP_NUM */
#define RKVDEC_SWREG103_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_THR_NUM_CH0_SHIFT (0U)
#define RKVDEC_SWREG103_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_THR_NUM_CH0_MASK (0xFFFFFFFFU << RKVDEC_SWREG103_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_THR_NUM_CH0_SHIFT) /* 0xFFFFFFFF */
/* SWREG104_PERF_RD_LATENCY_ACC_SUM */
#define RKVDEC_SWREG104_PERF_RD_LATENCY_ACC_SUM_RD_LATENCY_ACC_SUM_SHIFT (0U)
#define RKVDEC_SWREG104_PERF_RD_LATENCY_ACC_SUM_RD_LATENCY_ACC_SUM_MASK (0xFFFFFFFFU << RKVDEC_SWREG104_PERF_RD_LATENCY_ACC_SUM_RD_LATENCY_ACC_SUM_SHIFT) /* 0xFFFFFFFF */
/* SWREG105_PERF_RD_AXI_TOTAL_BYTE */
#define RKVDEC_SWREG105_PERF_RD_AXI_TOTAL_BYTE_PERF_RD_AXI_TOTAL_BYTE_SHIFT (0U)
#define RKVDEC_SWREG105_PERF_RD_AXI_TOTAL_BYTE_PERF_RD_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << RKVDEC_SWREG105_PERF_RD_AXI_TOTAL_BYTE_PERF_RD_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* SWREG106_PERF_WR_AXI_TOTAL_BYTE */
#define RKVDEC_SWREG106_PERF_WR_AXI_TOTAL_BYTE_PERF_WR_AXI_TOTAL_BYTE_SHIFT (0U)
#define RKVDEC_SWREG106_PERF_WR_AXI_TOTAL_BYTE_PERF_WR_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << RKVDEC_SWREG106_PERF_WR_AXI_TOTAL_BYTE_PERF_WR_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* SWREG107_PERF_WORKING_CNT */
#define RKVDEC_SWREG107_PERF_WORKING_CNT_PERF_WORKING_CNT_SHIFT (0U)
#define RKVDEC_SWREG107_PERF_WORKING_CNT_PERF_WORKING_CNT_MASK (0xFFFFFFFFU << RKVDEC_SWREG107_PERF_WORKING_CNT_PERF_WORKING_CNT_SHIFT) /* 0xFFFFFFFF */
/******************************************IEP*******************************************/
/* CONFIG0 */
#define IEP_CONFIG0_VOP_PATH_EN_SHIFT                      (0U)
#define IEP_CONFIG0_VOP_PATH_EN_MASK                       (0x1U << IEP_CONFIG0_VOP_PATH_EN_SHIFT)                      /* 0x00000001 */
#define IEP_CONFIG0_DIL_HF_FCT_SHIFT                       (1U)
#define IEP_CONFIG0_DIL_HF_FCT_MASK                        (0x7FU << IEP_CONFIG0_DIL_HF_FCT_SHIFT)                      /* 0x000000FE */
#define IEP_CONFIG0_DIL_MODE_SHIFT                         (8U)
#define IEP_CONFIG0_DIL_MODE_MASK                          (0x7U << IEP_CONFIG0_DIL_MODE_SHIFT)                         /* 0x00000700 */
#define IEP_CONFIG0_DIL_HF_EN_SHIFT                        (11U)
#define IEP_CONFIG0_DIL_HF_EN_MASK                         (0x1U << IEP_CONFIG0_DIL_HF_EN_SHIFT)                        /* 0x00000800 */
#define IEP_CONFIG0_DIL_EI_MODE_SHIFT                      (12U)
#define IEP_CONFIG0_DIL_EI_MODE_MASK                       (0x1U << IEP_CONFIG0_DIL_EI_MODE_SHIFT)                      /* 0x00001000 */
#define IEP_CONFIG0_YUV_DNS_EN_SHIFT                       (13U)
#define IEP_CONFIG0_YUV_DNS_EN_MASK                        (0x1U << IEP_CONFIG0_YUV_DNS_EN_SHIFT)                       /* 0x00002000 */
#define IEP_CONFIG0_YUV_ENH_EN_SHIFT                       (14U)
#define IEP_CONFIG0_YUV_ENH_EN_MASK                        (0x1U << IEP_CONFIG0_YUV_ENH_EN_SHIFT)                       /* 0x00004000 */
#define IEP_CONFIG0_DIL_EI_SMOOTH_SHIFT                    (15U)
#define IEP_CONFIG0_DIL_EI_SMOOTH_MASK                     (0x1U << IEP_CONFIG0_DIL_EI_SMOOTH_SHIFT)                    /* 0x00008000 */
#define IEP_CONFIG0_RGB_COLOR_ENH_EN_SHIFT                 (16U)
#define IEP_CONFIG0_RGB_COLOR_ENH_EN_MASK                  (0x1U << IEP_CONFIG0_RGB_COLOR_ENH_EN_SHIFT)                 /* 0x00010000 */
#define IEP_CONFIG0_RGB_CON_GAM_EN_SHIFT                   (17U)
#define IEP_CONFIG0_RGB_CON_GAM_EN_MASK                    (0x1U << IEP_CONFIG0_RGB_CON_GAM_EN_SHIFT)                   /* 0x00020000 */
#define IEP_CONFIG0_RGB_ENH_SEL_SHIFT                      (18U)
#define IEP_CONFIG0_RGB_ENH_SEL_MASK                       (0x3U << IEP_CONFIG0_RGB_ENH_SEL_SHIFT)                      /* 0x000C0000 */
#define IEP_CONFIG0_RGB_CON_GAM_ORDER_SHIFT                (20U)
#define IEP_CONFIG0_RGB_CON_GAM_ORDER_MASK                 (0x1U << IEP_CONFIG0_RGB_CON_GAM_ORDER_SHIFT)                /* 0x00100000 */
#define IEP_CONFIG0_DIL_EI_RADIUS_SHIFT                    (21U)
#define IEP_CONFIG0_DIL_EI_RADIUS_MASK                     (0x3U << IEP_CONFIG0_DIL_EI_RADIUS_SHIFT)                    /* 0x00600000 */
#define IEP_CONFIG0_DIL_EI_SEL_SHIFT                       (23U)
#define IEP_CONFIG0_DIL_EI_SEL_MASK                        (0x1U << IEP_CONFIG0_DIL_EI_SEL_SHIFT)                       /* 0x00800000 */
/* CONFIG1 */
#define IEP_CONFIG1_SRC_FMT_SHIFT                          (0U)
#define IEP_CONFIG1_SRC_FMT_MASK                           (0x3U << IEP_CONFIG1_SRC_FMT_SHIFT)                          /* 0x00000003 */
#define IEP_CONFIG1_SRC_RGB_SWAP_SHIFT                     (2U)
#define IEP_CONFIG1_SRC_RGB_SWAP_MASK                      (0x3U << IEP_CONFIG1_SRC_RGB_SWAP_SHIFT)                     /* 0x0000000C */
#define IEP_CONFIG1_SRC_YUV_SWAP_SHIFT                     (4U)
#define IEP_CONFIG1_SRC_YUV_SWAP_MASK                      (0x3U << IEP_CONFIG1_SRC_YUV_SWAP_SHIFT)                     /* 0x00000030 */
#define IEP_CONFIG1_DST_FMT_SHIFT                          (8U)
#define IEP_CONFIG1_DST_FMT_MASK                           (0x3U << IEP_CONFIG1_DST_FMT_SHIFT)                          /* 0x00000300 */
#define IEP_CONFIG1_DST_RGB_SWAP_SHIFT                     (10U)
#define IEP_CONFIG1_DST_RGB_SWAP_MASK                      (0x3U << IEP_CONFIG1_DST_RGB_SWAP_SHIFT)                     /* 0x00000C00 */
#define IEP_CONFIG1_DST_YUV_SWAP_SHIFT                     (12U)
#define IEP_CONFIG1_DST_YUV_SWAP_MASK                      (0x3U << IEP_CONFIG1_DST_YUV_SWAP_SHIFT)                     /* 0x00003000 */
#define IEP_CONFIG1_DTHR_UP_EN_SHIFT                       (14U)
#define IEP_CONFIG1_DTHR_UP_EN_MASK                        (0x1U << IEP_CONFIG1_DTHR_UP_EN_SHIFT)                       /* 0x00004000 */
#define IEP_CONFIG1_DTHR_DOWN_EN_SHIFT                     (15U)
#define IEP_CONFIG1_DTHR_DOWN_EN_MASK                      (0x1U << IEP_CONFIG1_DTHR_DOWN_EN_SHIFT)                     /* 0x00008000 */
#define IEP_CONFIG1_YUV2RGB_COE_SEL_SHIFT                  (16U)
#define IEP_CONFIG1_YUV2RGB_COE_SEL_MASK                   (0x3U << IEP_CONFIG1_YUV2RGB_COE_SEL_SHIFT)                  /* 0x00030000 */
#define IEP_CONFIG1_RGB2YUV_COE_SEL_SHIFT                  (18U)
#define IEP_CONFIG1_RGB2YUV_COE_SEL_MASK                   (0x3U << IEP_CONFIG1_RGB2YUV_COE_SEL_SHIFT)                  /* 0x000C0000 */
#define IEP_CONFIG1_YUV_TO_RGB_EN_SHIFT                    (20U)
#define IEP_CONFIG1_YUV_TO_RGB_EN_MASK                     (0x1U << IEP_CONFIG1_YUV_TO_RGB_EN_SHIFT)                    /* 0x00100000 */
#define IEP_CONFIG1_RGB_TO_YUV_EN_SHIFT                    (21U)
#define IEP_CONFIG1_RGB_TO_YUV_EN_MASK                     (0x1U << IEP_CONFIG1_RGB_TO_YUV_EN_SHIFT)                    /* 0x00200000 */
#define IEP_CONFIG1_YUV2RGB_INPUT_CLIP_SHIFT               (22U)
#define IEP_CONFIG1_YUV2RGB_INPUT_CLIP_MASK                (0x1U << IEP_CONFIG1_YUV2RGB_INPUT_CLIP_SHIFT)               /* 0x00400000 */
#define IEP_CONFIG1_RGB2YUV_INPUT_CLIP_SHIFT               (23U)
#define IEP_CONFIG1_RGB2YUV_INPUT_CLIP_MASK                (0x1U << IEP_CONFIG1_RGB2YUV_INPUT_CLIP_SHIFT)               /* 0x00800000 */
#define IEP_CONFIG1_GLB_ALPHA_SHIFT                        (24U)
#define IEP_CONFIG1_GLB_ALPHA_MASK                         (0xFFU << IEP_CONFIG1_GLB_ALPHA_SHIFT)                       /* 0xFF000000 */
/* STATUS */
#define IEP_STATUS_DNS_STS_SHIFT                           (0U)
#define IEP_STATUS_DNS_STS_MASK                            (0x1U << IEP_STATUS_DNS_STS_SHIFT)                           /* 0x00000001 */
#define IEP_STATUS_SCL_STS_SHIFT                           (1U)
#define IEP_STATUS_SCL_STS_MASK                            (0x1U << IEP_STATUS_SCL_STS_SHIFT)                           /* 0x00000002 */
#define IEP_STATUS_DIL_STS_SHIFT                           (2U)
#define IEP_STATUS_DIL_STS_MASK                            (0x1U << IEP_STATUS_DIL_STS_SHIFT)                           /* 0x00000004 */
#define IEP_STATUS_DDE_STS_SHIFT                           (3U)
#define IEP_STATUS_DDE_STS_MASK                            (0x1U << IEP_STATUS_DDE_STS_SHIFT)                           /* 0x00000008 */
#define IEP_STATUS_WYUV_STS_SHIFT                          (4U)
#define IEP_STATUS_WYUV_STS_MASK                           (0x1U << IEP_STATUS_WYUV_STS_SHIFT)                          /* 0x00000010 */
#define IEP_STATUS_RYUV_STS_SHIFT                          (5U)
#define IEP_STATUS_RYUV_STS_MASK                           (0x1U << IEP_STATUS_RYUV_STS_SHIFT)                          /* 0x00000020 */
#define IEP_STATUS_WRGB_STS_SHIFT                          (6U)
#define IEP_STATUS_WRGB_STS_MASK                           (0x1U << IEP_STATUS_WRGB_STS_SHIFT)                          /* 0x00000040 */
#define IEP_STATUS_RRGB_STS_SHIFT                          (7U)
#define IEP_STATUS_RRGB_STS_MASK                           (0x1U << IEP_STATUS_RRGB_STS_SHIFT)                          /* 0x00000080 */
#define IEP_STATUS_VOI_STS_SHIFT                           (8U)
#define IEP_STATUS_VOI_STS_MASK                            (0x1U << IEP_STATUS_VOI_STS_SHIFT)                           /* 0x00000100 */
#define IEP_STATUS_WYUV_IDLE_ACK_SHIFT                     (16U)
#define IEP_STATUS_WYUV_IDLE_ACK_MASK                      (0x1U << IEP_STATUS_WYUV_IDLE_ACK_SHIFT)                     /* 0x00010000 */
#define IEP_STATUS_RYUV_IDLE_ACK_SHIFT                     (17U)
#define IEP_STATUS_RYUV_IDLE_ACK_MASK                      (0x1U << IEP_STATUS_RYUV_IDLE_ACK_SHIFT)                     /* 0x00020000 */
#define IEP_STATUS_WRGB_IDLE_ACK_SHIFT                     (18U)
#define IEP_STATUS_WRGB_IDLE_ACK_MASK                      (0x1U << IEP_STATUS_WRGB_IDLE_ACK_SHIFT)                     /* 0x00040000 */
#define IEP_STATUS_RRGB_IDLE_ACK_SHIFT                     (19U)
#define IEP_STATUS_RRGB_IDLE_ACK_MASK                      (0x1U << IEP_STATUS_RRGB_IDLE_ACK_SHIFT)                     /* 0x00080000 */
/* INT */
#define IEP_INT_FRM_DONE_INT_SHIFT                         (0U)
#define IEP_INT_FRM_DONE_INT_MASK                          (0x1U << IEP_INT_FRM_DONE_INT_SHIFT)                         /* 0x00000001 */
#define IEP_INT_FRM_DONE_INT_EN_SHIFT                      (8U)
#define IEP_INT_FRM_DONE_INT_EN_MASK                       (0x1U << IEP_INT_FRM_DONE_INT_EN_SHIFT)                      /* 0x00000100 */
#define IEP_INT_FRM_DONE_INT_CLR_SHIFT                     (16U)
#define IEP_INT_FRM_DONE_INT_CLR_MASK                      (0x1U << IEP_INT_FRM_DONE_INT_CLR_SHIFT)                     /* 0x00010000 */
/* FRM_START */
#define IEP_FRM_START_FRM_START_SHIFT                      (0U)
#define IEP_FRM_START_FRM_START_MASK                       (0x1U << IEP_FRM_START_FRM_START_SHIFT)                      /* 0x00000001 */
/* CONFIG_DONE */
#define IEP_CONFIG_DONE_CONFIG_DONE_SHIFT                  (0U)
#define IEP_CONFIG_DONE_CONFIG_DONE_MASK                   (0x1U << IEP_CONFIG_DONE_CONFIG_DONE_SHIFT)                  /* 0x00000001 */
/* FRM_CNT */
#define IEP_FRM_CNT_FRM_CNT_SHIFT                          (0U)
#define IEP_FRM_CNT_FRM_CNT_MASK                           (0xFFFFFFFFU << IEP_FRM_CNT_FRM_CNT_SHIFT)                   /* 0xFFFFFFFF */
/* VIR_IMG_WIDTH */
#define IEP_VIR_IMG_WIDTH_SRC_VIR_IMAGE_WIDTH_SHIFT        (0U)
#define IEP_VIR_IMG_WIDTH_SRC_VIR_IMAGE_WIDTH_MASK         (0xFFFFU << IEP_VIR_IMG_WIDTH_SRC_VIR_IMAGE_WIDTH_SHIFT)     /* 0x0000FFFF */
#define IEP_VIR_IMG_WIDTH_DST_VIR_IMAGE_WIDTH_SHIFT        (16U)
#define IEP_VIR_IMG_WIDTH_DST_VIR_IMAGE_WIDTH_MASK         (0xFFFFU << IEP_VIR_IMG_WIDTH_DST_VIR_IMAGE_WIDTH_SHIFT)     /* 0xFFFF0000 */
/* SRC_IMG_SIZE */
#define IEP_SRC_IMG_SIZE_SRC_IMAGE_WIDTH_SHIFT             (0U)
#define IEP_SRC_IMG_SIZE_SRC_IMAGE_WIDTH_MASK              (0x1FFFU << IEP_SRC_IMG_SIZE_SRC_IMAGE_WIDTH_SHIFT)          /* 0x00001FFF */
#define IEP_SRC_IMG_SIZE_SRC_IMAGE_HEIGHT_SHIFT            (16U)
#define IEP_SRC_IMG_SIZE_SRC_IMAGE_HEIGHT_MASK             (0x1FFFU << IEP_SRC_IMG_SIZE_SRC_IMAGE_HEIGHT_SHIFT)         /* 0x1FFF0000 */
/* DST_IMG_SIZE */
#define IEP_DST_IMG_SIZE_DST_IMAGE_WIDTH_SHIFT             (0U)
#define IEP_DST_IMG_SIZE_DST_IMAGE_WIDTH_MASK              (0x1FFFU << IEP_DST_IMG_SIZE_DST_IMAGE_WIDTH_SHIFT)          /* 0x00001FFF */
#define IEP_DST_IMG_SIZE_DST_IMAGE_HEIGHT_SHIFT            (16U)
#define IEP_DST_IMG_SIZE_DST_IMAGE_HEIGHT_MASK             (0x1FFFU << IEP_DST_IMG_SIZE_DST_IMAGE_HEIGHT_SHIFT)         /* 0x1FFF0000 */
/* DST_IMG_WIDTH_TILE0 */
#define IEP_DST_IMG_WIDTH_TILE0_DST_IMAGE_WIDTH_TILE0_SHIFT (0U)
#define IEP_DST_IMG_WIDTH_TILE0_DST_IMAGE_WIDTH_TILE0_MASK (0xFFFU << IEP_DST_IMG_WIDTH_TILE0_DST_IMAGE_WIDTH_TILE0_SHIFT) /* 0x00000FFF */
/* DST_IMG_WIDTH_TILE1 */
#define IEP_DST_IMG_WIDTH_TILE1_DST_IMAGE_WIDTH_TILE1_SHIFT (0U)
#define IEP_DST_IMG_WIDTH_TILE1_DST_IMAGE_WIDTH_TILE1_MASK (0x7FFU << IEP_DST_IMG_WIDTH_TILE1_DST_IMAGE_WIDTH_TILE1_SHIFT) /* 0x000007FF */
/* DST_IMG_WIDTH_TILE2 */
#define IEP_DST_IMG_WIDTH_TILE2_DST_IMAGE_WIDTH_TILE2_SHIFT (0U)
#define IEP_DST_IMG_WIDTH_TILE2_DST_IMAGE_WIDTH_TILE2_MASK (0x7FFU << IEP_DST_IMG_WIDTH_TILE2_DST_IMAGE_WIDTH_TILE2_SHIFT) /* 0x000007FF */
/* DST_IMG_WIDTH_TILE3 */
#define IEP_DST_IMG_WIDTH_TILE3_DST_IMAGE_WIDTH_TILE3_SHIFT (0U)
#define IEP_DST_IMG_WIDTH_TILE3_DST_IMAGE_WIDTH_TILE3_MASK (0x7FFU << IEP_DST_IMG_WIDTH_TILE3_DST_IMAGE_WIDTH_TILE3_SHIFT) /* 0x000007FF */
/* ENH_YUV_CNFG_0 */
#define IEP_ENH_YUV_CNFG_0_BRIGHTNESS_SHIFT                (0U)
#define IEP_ENH_YUV_CNFG_0_BRIGHTNESS_MASK                 (0x3FU << IEP_ENH_YUV_CNFG_0_BRIGHTNESS_SHIFT)               /* 0x0000003F */
#define IEP_ENH_YUV_CNFG_0_CONTRAST_SHIFT                  (8U)
#define IEP_ENH_YUV_CNFG_0_CONTRAST_MASK                   (0xFFU << IEP_ENH_YUV_CNFG_0_CONTRAST_SHIFT)                 /* 0x0000FF00 */
#define IEP_ENH_YUV_CNFG_0_SAT_CON_SHIFT                   (16U)
#define IEP_ENH_YUV_CNFG_0_SAT_CON_MASK                    (0x1FFU << IEP_ENH_YUV_CNFG_0_SAT_CON_SHIFT)                 /* 0x01FF0000 */
/* ENH_YUV_CNFG_1 */
#define IEP_ENH_YUV_CNFG_1_SIN_HUE_SHIFT                   (0U)
#define IEP_ENH_YUV_CNFG_1_SIN_HUE_MASK                    (0xFFU << IEP_ENH_YUV_CNFG_1_SIN_HUE_SHIFT)                  /* 0x000000FF */
#define IEP_ENH_YUV_CNFG_1_COS_HUE_SHIFT                   (8U)
#define IEP_ENH_YUV_CNFG_1_COS_HUE_MASK                    (0xFFU << IEP_ENH_YUV_CNFG_1_COS_HUE_SHIFT)                  /* 0x0000FF00 */
/* ENH_YUV_CNFG_2 */
#define IEP_ENH_YUV_CNFG_2_COLOR_BAR_Y_SHIFT               (0U)
#define IEP_ENH_YUV_CNFG_2_COLOR_BAR_Y_MASK                (0xFFU << IEP_ENH_YUV_CNFG_2_COLOR_BAR_Y_SHIFT)              /* 0x000000FF */
#define IEP_ENH_YUV_CNFG_2_COLOR_BAR_U_SHIFT               (8U)
#define IEP_ENH_YUV_CNFG_2_COLOR_BAR_U_MASK                (0xFFU << IEP_ENH_YUV_CNFG_2_COLOR_BAR_U_SHIFT)              /* 0x0000FF00 */
#define IEP_ENH_YUV_CNFG_2_COLOR_BAR_V_SHIFT               (16U)
#define IEP_ENH_YUV_CNFG_2_COLOR_BAR_V_MASK                (0xFFU << IEP_ENH_YUV_CNFG_2_COLOR_BAR_V_SHIFT)              /* 0x00FF0000 */
#define IEP_ENH_YUV_CNFG_2_VIDEO_MODE_SHIFT                (24U)
#define IEP_ENH_YUV_CNFG_2_VIDEO_MODE_MASK                 (0x3U << IEP_ENH_YUV_CNFG_2_VIDEO_MODE_SHIFT)                /* 0x03000000 */
/* ENH_RGB_CNFG */
#define IEP_ENH_RGB_CNFG_ENH_RADIUS_SHIFT                  (0U)
#define IEP_ENH_RGB_CNFG_ENH_RADIUS_MASK                   (0x3U << IEP_ENH_RGB_CNFG_ENH_RADIUS_SHIFT)                  /* 0x00000003 */
#define IEP_ENH_RGB_CNFG_ENH_ALPHA_SHIFT                   (8U)
#define IEP_ENH_RGB_CNFG_ENH_ALPHA_MASK                    (0x7FU << IEP_ENH_RGB_CNFG_ENH_ALPHA_SHIFT)                  /* 0x00007F00 */
#define IEP_ENH_RGB_CNFG_ENH_THRESHOLD_SHIFT               (16U)
#define IEP_ENH_RGB_CNFG_ENH_THRESHOLD_MASK                (0xFFU << IEP_ENH_RGB_CNFG_ENH_THRESHOLD_SHIFT)              /* 0x00FF0000 */
#define IEP_ENH_RGB_CNFG_CHROMA_TEMP_SEL_SHIFT             (24U)
#define IEP_ENH_RGB_CNFG_CHROMA_TEMP_SEL_MASK              (0x3U << IEP_ENH_RGB_CNFG_CHROMA_TEMP_SEL_SHIFT)             /* 0x03000000 */
#define IEP_ENH_RGB_CNFG_CHROMA_SPAT_SEL_SHIFT             (26U)
#define IEP_ENH_RGB_CNFG_CHROMA_SPAT_SEL_MASK              (0x3U << IEP_ENH_RGB_CNFG_CHROMA_SPAT_SEL_SHIFT)             /* 0x0C000000 */
#define IEP_ENH_RGB_CNFG_LUMA_TEMP_SEL_SHIFT               (28U)
#define IEP_ENH_RGB_CNFG_LUMA_TEMP_SEL_MASK                (0x3U << IEP_ENH_RGB_CNFG_LUMA_TEMP_SEL_SHIFT)               /* 0x30000000 */
#define IEP_ENH_RGB_CNFG_LUMA_SPAT_SEL_SHIFT               (30U)
#define IEP_ENH_RGB_CNFG_LUMA_SPAT_SEL_MASK                (0x3U << IEP_ENH_RGB_CNFG_LUMA_SPAT_SEL_SHIFT)               /* 0xC0000000 */
/* ENH_C_COE */
#define IEP_ENH_C_COE_C_FRAC_COE_SHIFT                     (0U)
#define IEP_ENH_C_COE_C_FRAC_COE_MASK                      (0x1FU << IEP_ENH_C_COE_C_FRAC_COE_SHIFT)                    /* 0x0000001F */
#define IEP_ENH_C_COE_C_INT_COE_SHIFT                      (5U)
#define IEP_ENH_C_COE_C_INT_COE_MASK                       (0x3U << IEP_ENH_C_COE_C_INT_COE_SHIFT)                      /* 0x00000060 */
/* IEP_VERSION_INFO */
/* RAW_CONFIG0 */
#define IEP_RAW_CONFIG0_VOP_PATH_EN_SHIFT                  (0U)
#define IEP_RAW_CONFIG0_VOP_PATH_EN_MASK                   (0x1U << IEP_RAW_CONFIG0_VOP_PATH_EN_SHIFT)                  /* 0x00000001 */
#define IEP_RAW_CONFIG0_DIL_HF_FCT_SHIFT                   (1U)
#define IEP_RAW_CONFIG0_DIL_HF_FCT_MASK                    (0x7FU << IEP_RAW_CONFIG0_DIL_HF_FCT_SHIFT)                  /* 0x000000FE */
#define IEP_RAW_CONFIG0_DIL_MODE_SHIFT                     (8U)
#define IEP_RAW_CONFIG0_DIL_MODE_MASK                      (0x7U << IEP_RAW_CONFIG0_DIL_MODE_SHIFT)                     /* 0x00000700 */
#define IEP_RAW_CONFIG0_DIL_HF_EN_SHIFT                    (11U)
#define IEP_RAW_CONFIG0_DIL_HF_EN_MASK                     (0x1U << IEP_RAW_CONFIG0_DIL_HF_EN_SHIFT)                    /* 0x00000800 */
#define IEP_RAW_CONFIG0_DIL_EI_MODE_SHIFT                  (12U)
#define IEP_RAW_CONFIG0_DIL_EI_MODE_MASK                   (0x1U << IEP_RAW_CONFIG0_DIL_EI_MODE_SHIFT)                  /* 0x00001000 */
#define IEP_RAW_CONFIG0_YUV_DNS_EN_SHIFT                   (13U)
#define IEP_RAW_CONFIG0_YUV_DNS_EN_MASK                    (0x1U << IEP_RAW_CONFIG0_YUV_DNS_EN_SHIFT)                   /* 0x00002000 */
#define IEP_RAW_CONFIG0_YUV_ENH_EN_SHIFT                   (14U)
#define IEP_RAW_CONFIG0_YUV_ENH_EN_MASK                    (0x1U << IEP_RAW_CONFIG0_YUV_ENH_EN_SHIFT)                   /* 0x00004000 */
#define IEP_RAW_CONFIG0_DIL_EI_SMOOTH_SHIFT                (15U)
#define IEP_RAW_CONFIG0_DIL_EI_SMOOTH_MASK                 (0x1U << IEP_RAW_CONFIG0_DIL_EI_SMOOTH_SHIFT)                /* 0x00008000 */
#define IEP_RAW_CONFIG0_RGB_COLOR_ENH_EN_SHIFT             (16U)
#define IEP_RAW_CONFIG0_RGB_COLOR_ENH_EN_MASK              (0x1U << IEP_RAW_CONFIG0_RGB_COLOR_ENH_EN_SHIFT)             /* 0x00010000 */
#define IEP_RAW_CONFIG0_RGB_CON_GAM_EN_SHIFT               (17U)
#define IEP_RAW_CONFIG0_RGB_CON_GAM_EN_MASK                (0x1U << IEP_RAW_CONFIG0_RGB_CON_GAM_EN_SHIFT)               /* 0x00020000 */
#define IEP_RAW_CONFIG0_RGB_ENH_SEL_SHIFT                  (18U)
#define IEP_RAW_CONFIG0_RGB_ENH_SEL_MASK                   (0x3U << IEP_RAW_CONFIG0_RGB_ENH_SEL_SHIFT)                  /* 0x000C0000 */
#define IEP_RAW_CONFIG0_RGB_CON_GAM_ORDER_SHIFT            (20U)
#define IEP_RAW_CONFIG0_RGB_CON_GAM_ORDER_MASK             (0x1U << IEP_RAW_CONFIG0_RGB_CON_GAM_ORDER_SHIFT)            /* 0x00100000 */
#define IEP_RAW_CONFIG0_DIL_EI_RADIUS_SHIFT                (21U)
#define IEP_RAW_CONFIG0_DIL_EI_RADIUS_MASK                 (0x3U << IEP_RAW_CONFIG0_DIL_EI_RADIUS_SHIFT)                /* 0x00600000 */
#define IEP_RAW_CONFIG0_DIL_EI_SEL_SHIFT                   (23U)
#define IEP_RAW_CONFIG0_DIL_EI_SEL_MASK                    (0x1U << IEP_RAW_CONFIG0_DIL_EI_SEL_SHIFT)                   /* 0x00800000 */
/* RAW_CONFIG1 */
#define IEP_RAW_CONFIG1_SRC_FMT_SHIFT                      (0U)
#define IEP_RAW_CONFIG1_SRC_FMT_MASK                       (0x3U << IEP_RAW_CONFIG1_SRC_FMT_SHIFT)                      /* 0x00000003 */
#define IEP_RAW_CONFIG1_SRC_RGB_SWAP_SHIFT                 (2U)
#define IEP_RAW_CONFIG1_SRC_RGB_SWAP_MASK                  (0x3U << IEP_RAW_CONFIG1_SRC_RGB_SWAP_SHIFT)                 /* 0x0000000C */
#define IEP_RAW_CONFIG1_SRC_YUV_SWAP_SHIFT                 (4U)
#define IEP_RAW_CONFIG1_SRC_YUV_SWAP_MASK                  (0x3U << IEP_RAW_CONFIG1_SRC_YUV_SWAP_SHIFT)                 /* 0x00000030 */
#define IEP_RAW_CONFIG1_DST_FMT_SHIFT                      (8U)
#define IEP_RAW_CONFIG1_DST_FMT_MASK                       (0x3U << IEP_RAW_CONFIG1_DST_FMT_SHIFT)                      /* 0x00000300 */
#define IEP_RAW_CONFIG1_DST_RGB_SWAP_SHIFT                 (10U)
#define IEP_RAW_CONFIG1_DST_RGB_SWAP_MASK                  (0x3U << IEP_RAW_CONFIG1_DST_RGB_SWAP_SHIFT)                 /* 0x00000C00 */
#define IEP_RAW_CONFIG1_DST_YUV_SWAP_SHIFT                 (12U)
#define IEP_RAW_CONFIG1_DST_YUV_SWAP_MASK                  (0x3U << IEP_RAW_CONFIG1_DST_YUV_SWAP_SHIFT)                 /* 0x00003000 */
#define IEP_RAW_CONFIG1_DTHR_UP_EN_SHIFT                   (14U)
#define IEP_RAW_CONFIG1_DTHR_UP_EN_MASK                    (0x1U << IEP_RAW_CONFIG1_DTHR_UP_EN_SHIFT)                   /* 0x00004000 */
#define IEP_RAW_CONFIG1_DTHR_DOWN_EN_SHIFT                 (15U)
#define IEP_RAW_CONFIG1_DTHR_DOWN_EN_MASK                  (0x1U << IEP_RAW_CONFIG1_DTHR_DOWN_EN_SHIFT)                 /* 0x00008000 */
#define IEP_RAW_CONFIG1_YUV2RGB_COE_SEL_SHIFT              (16U)
#define IEP_RAW_CONFIG1_YUV2RGB_COE_SEL_MASK               (0x3U << IEP_RAW_CONFIG1_YUV2RGB_COE_SEL_SHIFT)              /* 0x00030000 */
#define IEP_RAW_CONFIG1_RGB2YUV_COE_SEL_SHIFT              (18U)
#define IEP_RAW_CONFIG1_RGB2YUV_COE_SEL_MASK               (0x3U << IEP_RAW_CONFIG1_RGB2YUV_COE_SEL_SHIFT)              /* 0x000C0000 */
#define IEP_RAW_CONFIG1_YUV_TO_RGB_EN_SHIFT                (20U)
#define IEP_RAW_CONFIG1_YUV_TO_RGB_EN_MASK                 (0x1U << IEP_RAW_CONFIG1_YUV_TO_RGB_EN_SHIFT)                /* 0x00100000 */
#define IEP_RAW_CONFIG1_RGB_TO_YUV_EN_SHIFT                (21U)
#define IEP_RAW_CONFIG1_RGB_TO_YUV_EN_MASK                 (0x1U << IEP_RAW_CONFIG1_RGB_TO_YUV_EN_SHIFT)                /* 0x00200000 */
#define IEP_RAW_CONFIG1_YUV2RGB_INPUT_CLIP_SHIFT           (22U)
#define IEP_RAW_CONFIG1_YUV2RGB_INPUT_CLIP_MASK            (0x1U << IEP_RAW_CONFIG1_YUV2RGB_INPUT_CLIP_SHIFT)           /* 0x00400000 */
#define IEP_RAW_CONFIG1_RGB2YUV_INPUT_CLIP_SHIFT           (23U)
#define IEP_RAW_CONFIG1_RGB2YUV_INPUT_CLIP_MASK            (0x1U << IEP_RAW_CONFIG1_RGB2YUV_INPUT_CLIP_SHIFT)           /* 0x00800000 */
#define IEP_RAW_CONFIG1_GLB_ALPHA_SHIFT                    (24U)
#define IEP_RAW_CONFIG1_GLB_ALPHA_MASK                     (0xFFU << IEP_RAW_CONFIG1_GLB_ALPHA_SHIFT)                   /* 0xFF000000 */
/* RAW_VIR_IMG_WIDTH */
#define IEP_RAW_VIR_IMG_WIDTH_SRC_VIR_IMAGE_WIDTH_SHIFT    (0U)
#define IEP_RAW_VIR_IMG_WIDTH_SRC_VIR_IMAGE_WIDTH_MASK     (0xFFFFU << IEP_RAW_VIR_IMG_WIDTH_SRC_VIR_IMAGE_WIDTH_SHIFT) /* 0x0000FFFF */
#define IEP_RAW_VIR_IMG_WIDTH_DST_VIR_IMAGE_WIDTH_SHIFT    (16U)
#define IEP_RAW_VIR_IMG_WIDTH_DST_VIR_IMAGE_WIDTH_MASK     (0xFFFFU << IEP_RAW_VIR_IMG_WIDTH_DST_VIR_IMAGE_WIDTH_SHIFT) /* 0xFFFF0000 */
/* RAW_IMG_SCL_FCT */
#define IEP_RAW_IMG_SCL_FCT_HRZ_SCL_FCT_SHIFT              (0U)
#define IEP_RAW_IMG_SCL_FCT_HRZ_SCL_FCT_MASK               (0xFFFFU << IEP_RAW_IMG_SCL_FCT_HRZ_SCL_FCT_SHIFT)           /* 0x0000FFFF */
#define IEP_RAW_IMG_SCL_FCT_VRT_SCL_FCT_SHIFT              (16U)
#define IEP_RAW_IMG_SCL_FCT_VRT_SCL_FCT_MASK               (0xFFFFU << IEP_RAW_IMG_SCL_FCT_VRT_SCL_FCT_SHIFT)           /* 0xFFFF0000 */
/* RAW_SRC_IMG_SIZE */
#define IEP_RAW_SRC_IMG_SIZE_SRC_IMAGE_WIDTH_SHIFT         (0U)
#define IEP_RAW_SRC_IMG_SIZE_SRC_IMAGE_WIDTH_MASK          (0x1FFFU << IEP_RAW_SRC_IMG_SIZE_SRC_IMAGE_WIDTH_SHIFT)      /* 0x00001FFF */
#define IEP_RAW_SRC_IMG_SIZE_SRC_IMAGE_HEIGHT_SHIFT        (16U)
#define IEP_RAW_SRC_IMG_SIZE_SRC_IMAGE_HEIGHT_MASK         (0x1FFFU << IEP_RAW_SRC_IMG_SIZE_SRC_IMAGE_HEIGHT_SHIFT)     /* 0x1FFF0000 */
/* RAW_DST_IMG_SIZE */
#define IEP_RAW_DST_IMG_SIZE_DST_IMAGE_WIDTH_SHIFT         (0U)
#define IEP_RAW_DST_IMG_SIZE_DST_IMAGE_WIDTH_MASK          (0x1FFFU << IEP_RAW_DST_IMG_SIZE_DST_IMAGE_WIDTH_SHIFT)      /* 0x00001FFF */
#define IEP_RAW_DST_IMG_SIZE_DST_IMAGE_HEIGHT_SHIFT        (16U)
#define IEP_RAW_DST_IMG_SIZE_DST_IMAGE_HEIGHT_MASK         (0x1FFFU << IEP_RAW_DST_IMG_SIZE_DST_IMAGE_HEIGHT_SHIFT)     /* 0x1FFF0000 */
/* RAW_ENH_YUV_CNFG_0 */
#define IEP_RAW_ENH_YUV_CNFG_0_BRIGHTNESS_SHIFT            (0U)
#define IEP_RAW_ENH_YUV_CNFG_0_BRIGHTNESS_MASK             (0x3FU << IEP_RAW_ENH_YUV_CNFG_0_BRIGHTNESS_SHIFT)           /* 0x0000003F */
#define IEP_RAW_ENH_YUV_CNFG_0_CONTRAST_SHIFT              (8U)
#define IEP_RAW_ENH_YUV_CNFG_0_CONTRAST_MASK               (0xFFU << IEP_RAW_ENH_YUV_CNFG_0_CONTRAST_SHIFT)             /* 0x0000FF00 */
#define IEP_RAW_ENH_YUV_CNFG_0_SAT_CON_SHIFT               (16U)
#define IEP_RAW_ENH_YUV_CNFG_0_SAT_CON_MASK                (0x1FFU << IEP_RAW_ENH_YUV_CNFG_0_SAT_CON_SHIFT)             /* 0x01FF0000 */
/* RAW_ENH_YUV_CNFG_1 */
#define IEP_RAW_ENH_YUV_CNFG_1_SIN_HUE_SHIFT               (0U)
#define IEP_RAW_ENH_YUV_CNFG_1_SIN_HUE_MASK                (0xFFU << IEP_RAW_ENH_YUV_CNFG_1_SIN_HUE_SHIFT)              /* 0x000000FF */
#define IEP_RAW_ENH_YUV_CNFG_1_COS_HUE_SHIFT               (8U)
#define IEP_RAW_ENH_YUV_CNFG_1_COS_HUE_MASK                (0xFFU << IEP_RAW_ENH_YUV_CNFG_1_COS_HUE_SHIFT)              /* 0x0000FF00 */
/* RAW_ENH_YUV_CNFG_2 */
#define IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_Y_SHIFT           (0U)
#define IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_Y_MASK            (0xFFU << IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_Y_SHIFT)          /* 0x000000FF */
#define IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_U_SHIFT           (8U)
#define IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_U_MASK            (0xFFU << IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_U_SHIFT)          /* 0x0000FF00 */
#define IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_V_SHIFT           (16U)
#define IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_V_MASK            (0xFFU << IEP_RAW_ENH_YUV_CNFG_2_COLOR_BAR_V_SHIFT)          /* 0x00FF0000 */
#define IEP_RAW_ENH_YUV_CNFG_2_VIDEO_MODE_SHIFT            (24U)
#define IEP_RAW_ENH_YUV_CNFG_2_VIDEO_MODE_MASK             (0x3U << IEP_RAW_ENH_YUV_CNFG_2_VIDEO_MODE_SHIFT)            /* 0x03000000 */
/* RAW_ENH_RGB_CNFG */
#define IEP_RAW_ENH_RGB_CNFG_ENH_RADIUS_SHIFT              (0U)
#define IEP_RAW_ENH_RGB_CNFG_ENH_RADIUS_MASK               (0x3U << IEP_RAW_ENH_RGB_CNFG_ENH_RADIUS_SHIFT)              /* 0x00000003 */
#define IEP_RAW_ENH_RGB_CNFG_ENH_ALPHA_SHIFT               (8U)
#define IEP_RAW_ENH_RGB_CNFG_ENH_ALPHA_MASK                (0x7FU << IEP_RAW_ENH_RGB_CNFG_ENH_ALPHA_SHIFT)              /* 0x00007F00 */
#define IEP_RAW_ENH_RGB_CNFG_ENH_THRESHOLD_SHIFT           (16U)
#define IEP_RAW_ENH_RGB_CNFG_ENH_THRESHOLD_MASK            (0xFFU << IEP_RAW_ENH_RGB_CNFG_ENH_THRESHOLD_SHIFT)          /* 0x00FF0000 */
#define IEP_RAW_ENH_RGB_CNFG_CHROMA_TEMP_SEL_SHIFT         (24U)
#define IEP_RAW_ENH_RGB_CNFG_CHROMA_TEMP_SEL_MASK          (0x3U << IEP_RAW_ENH_RGB_CNFG_CHROMA_TEMP_SEL_SHIFT)         /* 0x03000000 */
#define IEP_RAW_ENH_RGB_CNFG_CHROMA_SPAT_SEL_SHIFT         (26U)
#define IEP_RAW_ENH_RGB_CNFG_CHROMA_SPAT_SEL_MASK          (0x3U << IEP_RAW_ENH_RGB_CNFG_CHROMA_SPAT_SEL_SHIFT)         /* 0x0C000000 */
#define IEP_RAW_ENH_RGB_CNFG_LUMA_TEMP_SEL_SHIFT           (28U)
#define IEP_RAW_ENH_RGB_CNFG_LUMA_TEMP_SEL_MASK            (0x3U << IEP_RAW_ENH_RGB_CNFG_LUMA_TEMP_SEL_SHIFT)           /* 0x30000000 */
#define IEP_RAW_ENH_RGB_CNFG_LUMA_SPAT_SEL_SHIFT           (30U)
#define IEP_RAW_ENH_RGB_CNFG_LUMA_SPAT_SEL_MASK            (0x3U << IEP_RAW_ENH_RGB_CNFG_LUMA_SPAT_SEL_SHIFT)           /* 0xC0000000 */
/* SRC_ADDR_YRGB */
#define IEP_SRC_ADDR_YRGB_SRC_IMAGE_YRGB_MST_SHIFT         (0U)
#define IEP_SRC_ADDR_YRGB_SRC_IMAGE_YRGB_MST_MASK          (0xFFFFFFFFU << IEP_SRC_ADDR_YRGB_SRC_IMAGE_YRGB_MST_SHIFT)  /* 0xFFFFFFFF */
/* SRC_ADDR_CBCR */
#define IEP_SRC_ADDR_CBCR_SRC_IMAGE_CBCR_MST_SHIFT         (0U)
#define IEP_SRC_ADDR_CBCR_SRC_IMAGE_CBCR_MST_MASK          (0xFFFFFFFFU << IEP_SRC_ADDR_CBCR_SRC_IMAGE_CBCR_MST_SHIFT)  /* 0xFFFFFFFF */
/* SRC_ADDR_CR */
#define IEP_SRC_ADDR_CR_SRC_IMAGE_CR_MST_SHIFT             (0U)
#define IEP_SRC_ADDR_CR_SRC_IMAGE_CR_MST_MASK              (0xFFFFFFFFU << IEP_SRC_ADDR_CR_SRC_IMAGE_CR_MST_SHIFT)      /* 0xFFFFFFFF */
/* SRC_ADDR_Y1 */
#define IEP_SRC_ADDR_Y1_SRC_IMAGE_Y_MST_SHIFT              (0U)
#define IEP_SRC_ADDR_Y1_SRC_IMAGE_Y_MST_MASK               (0xFFFFFFFFU << IEP_SRC_ADDR_Y1_SRC_IMAGE_Y_MST_SHIFT)       /* 0xFFFFFFFF */
/* SRC_ADDR_CBCR1 */
#define IEP_SRC_ADDR_CBCR1_SRC_IMAGE_CBCR_MST_SHIFT        (0U)
#define IEP_SRC_ADDR_CBCR1_SRC_IMAGE_CBCR_MST_MASK         (0xFFFFFFFFU << IEP_SRC_ADDR_CBCR1_SRC_IMAGE_CBCR_MST_SHIFT) /* 0xFFFFFFFF */
/* SRC_ADDR_CR1 */
#define IEP_SRC_ADDR_CR1_SRC_IMAGE_CR_MST_SHIFT            (0U)
#define IEP_SRC_ADDR_CR1_SRC_IMAGE_CR_MST_MASK             (0xFFFFFFFFU << IEP_SRC_ADDR_CR1_SRC_IMAGE_CR_MST_SHIFT)     /* 0xFFFFFFFF */
/* SRC_ADDR_Y_ITEMP */
#define IEP_SRC_ADDR_Y_ITEMP_SRC_IMAGE_Y_MST_ITEMP_SHIFT   (0U)
#define IEP_SRC_ADDR_Y_ITEMP_SRC_IMAGE_Y_MST_ITEMP_MASK    (0xFFFFFFFFU << IEP_SRC_ADDR_Y_ITEMP_SRC_IMAGE_Y_MST_ITEMP_SHIFT) /* 0xFFFFFFFF */
/* SRC_ADDR_CBCR_ITEMP */
#define IEP_SRC_ADDR_CBCR_ITEMP_SRC_IMAGE_CBCR_MST_CBCR_ITEMP_SHIFT (0U)
#define IEP_SRC_ADDR_CBCR_ITEMP_SRC_IMAGE_CBCR_MST_CBCR_ITEMP_MASK (0xFFFFFFFFU << IEP_SRC_ADDR_CBCR_ITEMP_SRC_IMAGE_CBCR_MST_CBCR_ITEMP_SHIFT) /* 0xFFFFFFFF */
/* SRC_ADDR_CR_ITEMP */
#define IEP_SRC_ADDR_CR_ITEMP_SRC_IMAGE_CR_MST_CR_ITEMP_SHIFT (0U)
#define IEP_SRC_ADDR_CR_ITEMP_SRC_IMAGE_CR_MST_CR_ITEMP_MASK (0xFFFFFFFFU << IEP_SRC_ADDR_CR_ITEMP_SRC_IMAGE_CR_MST_CR_ITEMP_SHIFT) /* 0xFFFFFFFF */
/* SRC_ADDR_Y_FTEMP */
#define IEP_SRC_ADDR_Y_FTEMP_SRC_IMAGE_Y_MST_FTEMP_SHIFT   (0U)
#define IEP_SRC_ADDR_Y_FTEMP_SRC_IMAGE_Y_MST_FTEMP_MASK    (0xFFFFFFFFU << IEP_SRC_ADDR_Y_FTEMP_SRC_IMAGE_Y_MST_FTEMP_SHIFT) /* 0xFFFFFFFF */
/* SRC_ADDR_CBCR_FTEMP */
#define IEP_SRC_ADDR_CBCR_FTEMP_SRC_IMAGE_CBCR_MST_FTEMP_SHIFT (0U)
#define IEP_SRC_ADDR_CBCR_FTEMP_SRC_IMAGE_CBCR_MST_FTEMP_MASK (0xFFFFFFFFU << IEP_SRC_ADDR_CBCR_FTEMP_SRC_IMAGE_CBCR_MST_FTEMP_SHIFT) /* 0xFFFFFFFF */
/* SRC_ADDR_CR_FTEMP */
#define IEP_SRC_ADDR_CR_FTEMP_SRC_IMAGE_CR_MST_FTEMP_SHIFT (0U)
#define IEP_SRC_ADDR_CR_FTEMP_SRC_IMAGE_CR_MST_FTEMP_MASK  (0xFFFFFFFFU << IEP_SRC_ADDR_CR_FTEMP_SRC_IMAGE_CR_MST_FTEMP_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_YRGB */
#define IEP_DST_ADDR_YRGB_DST_IMAGE_YRGB_MST_SHIFT         (0U)
#define IEP_DST_ADDR_YRGB_DST_IMAGE_YRGB_MST_MASK          (0xFFFFFFFFU << IEP_DST_ADDR_YRGB_DST_IMAGE_YRGB_MST_SHIFT)  /* 0xFFFFFFFF */
/* DST_ADDR_CBCR */
#define IEP_DST_ADDR_CBCR_DST_IMAGE_CBCR_MST_SHIFT         (0U)
#define IEP_DST_ADDR_CBCR_DST_IMAGE_CBCR_MST_MASK          (0xFFFFFFFFU << IEP_DST_ADDR_CBCR_DST_IMAGE_CBCR_MST_SHIFT)  /* 0xFFFFFFFF */
/* DST_ADDR_CR */
#define IEP_DST_ADDR_CR_DST_IMAGE_CR_MST_SHIFT             (0U)
#define IEP_DST_ADDR_CR_DST_IMAGE_CR_MST_MASK              (0xFFFFFFFFU << IEP_DST_ADDR_CR_DST_IMAGE_CR_MST_SHIFT)      /* 0xFFFFFFFF */
/* DST_ADDR_Y1 */
#define IEP_DST_ADDR_Y1_DST_IMAGE_Y_MST_SHIFT              (0U)
#define IEP_DST_ADDR_Y1_DST_IMAGE_Y_MST_MASK               (0xFFFFFFFFU << IEP_DST_ADDR_Y1_DST_IMAGE_Y_MST_SHIFT)       /* 0xFFFFFFFF */
/* DST_ADDR_CBCR1 */
#define IEP_DST_ADDR_CBCR1_DST_IMAGE_CBCR_MST_SHIFT        (0U)
#define IEP_DST_ADDR_CBCR1_DST_IMAGE_CBCR_MST_MASK         (0xFFFFFFFFU << IEP_DST_ADDR_CBCR1_DST_IMAGE_CBCR_MST_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_CR1 */
#define IEP_DST_ADDR_CR1_DST_IMAGE_CR_MST_SHIFT            (0U)
#define IEP_DST_ADDR_CR1_DST_IMAGE_CR_MST_MASK             (0xFFFFFFFFU << IEP_DST_ADDR_CR1_DST_IMAGE_CR_MST_SHIFT)     /* 0xFFFFFFFF */
/* DST_ADDR_Y_ITEMP */
#define IEP_DST_ADDR_Y_ITEMP_DST_IMAGE_Y_MST_ITEMP_SHIFT   (0U)
#define IEP_DST_ADDR_Y_ITEMP_DST_IMAGE_Y_MST_ITEMP_MASK    (0xFFFFFFFFU << IEP_DST_ADDR_Y_ITEMP_DST_IMAGE_Y_MST_ITEMP_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_CBCR_ITEMP */
#define IEP_DST_ADDR_CBCR_ITEMP_DST_IMAGE_CBCR_MST_ITEMP_SHIFT (0U)
#define IEP_DST_ADDR_CBCR_ITEMP_DST_IMAGE_CBCR_MST_ITEMP_MASK (0xFFFFFFFFU << IEP_DST_ADDR_CBCR_ITEMP_DST_IMAGE_CBCR_MST_ITEMP_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_CR_ITEMP */
#define IEP_DST_ADDR_CR_ITEMP_DST_IMAGE_CR_MST_ITEMP_SHIFT (0U)
#define IEP_DST_ADDR_CR_ITEMP_DST_IMAGE_CR_MST_ITEMP_MASK  (0xFFFFFFFFU << IEP_DST_ADDR_CR_ITEMP_DST_IMAGE_CR_MST_ITEMP_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_Y_FTEMP */
#define IEP_DST_ADDR_Y_FTEMP_DST_IMAGE_Y_MST_FTEMP_SHIFT   (0U)
#define IEP_DST_ADDR_Y_FTEMP_DST_IMAGE_Y_MST_FTEMP_MASK    (0xFFFFFFFFU << IEP_DST_ADDR_Y_FTEMP_DST_IMAGE_Y_MST_FTEMP_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_CBCR_FTEMP */
#define IEP_DST_ADDR_CBCR_FTEMP_DST_IMAGE_CBCR_MST_FTEMP_SHIFT (0U)
#define IEP_DST_ADDR_CBCR_FTEMP_DST_IMAGE_CBCR_MST_FTEMP_MASK (0xFFFFFFFFU << IEP_DST_ADDR_CBCR_FTEMP_DST_IMAGE_CBCR_MST_FTEMP_SHIFT) /* 0xFFFFFFFF */
/* DST_ADDR_CR_FTEMP */
#define IEP_DST_ADDR_CR_FTEMP_DST_IMAGE_CR_MST_FTEMP_SHIFT (0U)
#define IEP_DST_ADDR_CR_FTEMP_DST_IMAGE_CR_MST_FTEMP_MASK  (0xFFFFFFFFU << IEP_DST_ADDR_CR_FTEMP_DST_IMAGE_CR_MST_FTEMP_SHIFT) /* 0xFFFFFFFF */
/* DIL_MTN_TAB0 */
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB0_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB0_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB0_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB0_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB0_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB1 */
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB1_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB1_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB1_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB1_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB1_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB2 */
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB2_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB2_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB2_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB2_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB2_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB3 */
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB3_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB3_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB3_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB3_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB3_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB4 */
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB4_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB4_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB4_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB4_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB4_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB5 */
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB5_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB5_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB5_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB5_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB5_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB6 */
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB6_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB6_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB6_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB6_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB6_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* DIL_MTN_TAB7 */
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB0_SHIFT                (0U)
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB0_MASK                 (0x7FU << IEP_DIL_MTN_TAB7_MTN_SUB_TAB0_SHIFT)               /* 0x0000007F */
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB1_SHIFT                (8U)
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB1_MASK                 (0x7FU << IEP_DIL_MTN_TAB7_MTN_SUB_TAB1_SHIFT)               /* 0x00007F00 */
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB2_SHIFT                (16U)
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB2_MASK                 (0x7FU << IEP_DIL_MTN_TAB7_MTN_SUB_TAB2_SHIFT)               /* 0x007F0000 */
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB3_SHIFT                (24U)
#define IEP_DIL_MTN_TAB7_MTN_SUB_TAB3_MASK                 (0x7FU << IEP_DIL_MTN_TAB7_MTN_SUB_TAB3_SHIFT)               /* 0x7F000000 */
/* ENH_CG_TAB */
#define IEP_ENH_CG_TAB_CG_TAB_0_SHIFT                      (0U)
#define IEP_ENH_CG_TAB_CG_TAB_0_MASK                       (0xFFU << IEP_ENH_CG_TAB_CG_TAB_0_SHIFT)                     /* 0x000000FF */
#define IEP_ENH_CG_TAB_CG_TAB_1_SHIFT                      (8U)
#define IEP_ENH_CG_TAB_CG_TAB_1_MASK                       (0xFFU << IEP_ENH_CG_TAB_CG_TAB_1_SHIFT)                     /* 0x0000FF00 */
#define IEP_ENH_CG_TAB_CG_TAB_2_SHIFT                      (16U)
#define IEP_ENH_CG_TAB_CG_TAB_2_MASK                       (0xFFU << IEP_ENH_CG_TAB_CG_TAB_2_SHIFT)                     /* 0x00FF0000 */
#define IEP_ENH_CG_TAB_CG_TAB_3_SHIFT                      (24U)
#define IEP_ENH_CG_TAB_CG_TAB_3_MASK                       (0xFFU << IEP_ENH_CG_TAB_CG_TAB_3_SHIFT)                     /* 0xFF000000 */
/* ENH_DDE_COE0 */
#define IEP_ENH_DDE_COE0_DDE_COE_0_SHIFT                   (0U)
#define IEP_ENH_DDE_COE0_DDE_COE_0_MASK                    (0x3FU << IEP_ENH_DDE_COE0_DDE_COE_0_SHIFT)                  /* 0x0000003F */
#define IEP_ENH_DDE_COE0_DDE_COE_1_SHIFT                   (8U)
#define IEP_ENH_DDE_COE0_DDE_COE_1_MASK                    (0x3FU << IEP_ENH_DDE_COE0_DDE_COE_1_SHIFT)                  /* 0x00003F00 */
#define IEP_ENH_DDE_COE0_DDE_COE_2_SHIFT                   (16U)
#define IEP_ENH_DDE_COE0_DDE_COE_2_MASK                    (0x3FU << IEP_ENH_DDE_COE0_DDE_COE_2_SHIFT)                  /* 0x003F0000 */
#define IEP_ENH_DDE_COE0_DDE_COE_3_SHIFT                   (24U)
#define IEP_ENH_DDE_COE0_DDE_COE_3_MASK                    (0x3FU << IEP_ENH_DDE_COE0_DDE_COE_3_SHIFT)                  /* 0x3F000000 */
/* ENH_DDE_COE1 */
#define IEP_ENH_DDE_COE1_DDE_COE_0_SHIFT                   (0U)
#define IEP_ENH_DDE_COE1_DDE_COE_0_MASK                    (0x3FU << IEP_ENH_DDE_COE1_DDE_COE_0_SHIFT)                  /* 0x0000003F */
#define IEP_ENH_DDE_COE1_DDE_COE_1_SHIFT                   (8U)
#define IEP_ENH_DDE_COE1_DDE_COE_1_MASK                    (0x3FU << IEP_ENH_DDE_COE1_DDE_COE_1_SHIFT)                  /* 0x00003F00 */
#define IEP_ENH_DDE_COE1_DDE_COE_2_SHIFT                   (16U)
#define IEP_ENH_DDE_COE1_DDE_COE_2_MASK                    (0x3FU << IEP_ENH_DDE_COE1_DDE_COE_2_SHIFT)                  /* 0x003F0000 */
#define IEP_ENH_DDE_COE1_DDE_COE_3_SHIFT                   (24U)
#define IEP_ENH_DDE_COE1_DDE_COE_3_MASK                    (0x3FU << IEP_ENH_DDE_COE1_DDE_COE_3_SHIFT)                  /* 0x3F000000 */
/* MMU_DTE_ADDR */
#define IEP_MMU_DTE_ADDR_MMU_DTE_ADDR_SHIFT                (0U)
#define IEP_MMU_DTE_ADDR_MMU_DTE_ADDR_MASK                 (0xFFFFFFFFU << IEP_MMU_DTE_ADDR_MMU_DTE_ADDR_SHIFT)         /* 0xFFFFFFFF */
/* MMU_STATUS */
#define IEP_MMU_STATUS_MMU_PAGING_ENABLED_SHIFT            (0U)
#define IEP_MMU_STATUS_MMU_PAGING_ENABLED_MASK             (0x1U << IEP_MMU_STATUS_MMU_PAGING_ENABLED_SHIFT)            /* 0x00000001 */
#define IEP_MMU_STATUS_MMU_PAGE_FAULT_ACTIVE_SHIFT         (1U)
#define IEP_MMU_STATUS_MMU_PAGE_FAULT_ACTIVE_MASK          (0x1U << IEP_MMU_STATUS_MMU_PAGE_FAULT_ACTIVE_SHIFT)         /* 0x00000002 */
#define IEP_MMU_STATUS_MMU_STALL_ACTIVE_SHIFT              (2U)
#define IEP_MMU_STATUS_MMU_STALL_ACTIVE_MASK               (0x1U << IEP_MMU_STATUS_MMU_STALL_ACTIVE_SHIFT)              /* 0x00000004 */
#define IEP_MMU_STATUS_MMU_IDLE_SHIFT                      (3U)
#define IEP_MMU_STATUS_MMU_IDLE_MASK                       (0x1U << IEP_MMU_STATUS_MMU_IDLE_SHIFT)                      /* 0x00000008 */
#define IEP_MMU_STATUS_MMU_REPLAY_BUFFER_EMPTY_SHIFT       (4U)
#define IEP_MMU_STATUS_MMU_REPLAY_BUFFER_EMPTY_MASK        (0x1U << IEP_MMU_STATUS_MMU_REPLAY_BUFFER_EMPTY_SHIFT)       /* 0x00000010 */
#define IEP_MMU_STATUS_MMU_PAGE_FAULT_IS_WRITE_SHIFT       (5U)
#define IEP_MMU_STATUS_MMU_PAGE_FAULT_IS_WRITE_MASK        (0x1U << IEP_MMU_STATUS_MMU_PAGE_FAULT_IS_WRITE_SHIFT)       /* 0x00000020 */
#define IEP_MMU_STATUS_MMU_PAGE_FAULT_BUS_ID_SHIFT         (6U)
#define IEP_MMU_STATUS_MMU_PAGE_FAULT_BUS_ID_MASK          (0x1FU << IEP_MMU_STATUS_MMU_PAGE_FAULT_BUS_ID_SHIFT)        /* 0x000007C0 */
/* MMU_CMD */
#define IEP_MMU_CMD_MMU_CMD_SHIFT                          (0U)
#define IEP_MMU_CMD_MMU_CMD_MASK                           (0x7U << IEP_MMU_CMD_MMU_CMD_SHIFT)                          /* 0x00000007 */
/* MMU_PAGE_FAULT_ADDR */
#define IEP_MMU_PAGE_FAULT_ADDR_MMU_PAGE_FAULT_ADDR_SHIFT  (0U)
#define IEP_MMU_PAGE_FAULT_ADDR_MMU_PAGE_FAULT_ADDR_MASK   (0xFFFFFFFFU << IEP_MMU_PAGE_FAULT_ADDR_MMU_PAGE_FAULT_ADDR_SHIFT) /* 0xFFFFFFFF */
/* MMU_ZAP_ONE_LINE */
#define IEP_MMU_ZAP_ONE_LINE_MMU_ZAP_ONE_LINE_SHIFT        (0U)
#define IEP_MMU_ZAP_ONE_LINE_MMU_ZAP_ONE_LINE_MASK         (0x1U << IEP_MMU_ZAP_ONE_LINE_MMU_ZAP_ONE_LINE_SHIFT)        /* 0x00000001 */
/* MMU_INT_RAWSTAT */
#define IEP_MMU_INT_RAWSTAT_PAGE_FAULT_SHIFT               (0U)
#define IEP_MMU_INT_RAWSTAT_PAGE_FAULT_MASK                (0x1U << IEP_MMU_INT_RAWSTAT_PAGE_FAULT_SHIFT)               /* 0x00000001 */
#define IEP_MMU_INT_RAWSTAT_READ_BUS_ERROR_SHIFT           (1U)
#define IEP_MMU_INT_RAWSTAT_READ_BUS_ERROR_MASK            (0x1U << IEP_MMU_INT_RAWSTAT_READ_BUS_ERROR_SHIFT)           /* 0x00000002 */
/* MMU_INT_CLEAR */
#define IEP_MMU_INT_CLEAR_PAGE_FAULT_CLEAR_SHIFT           (0U)
#define IEP_MMU_INT_CLEAR_PAGE_FAULT_CLEAR_MASK            (0x1U << IEP_MMU_INT_CLEAR_PAGE_FAULT_CLEAR_SHIFT)           /* 0x00000001 */
#define IEP_MMU_INT_CLEAR_READ_BUS_ERROR_CLEAR_SHIFT       (1U)
#define IEP_MMU_INT_CLEAR_READ_BUS_ERROR_CLEAR_MASK        (0x1U << IEP_MMU_INT_CLEAR_READ_BUS_ERROR_CLEAR_SHIFT)       /* 0x00000002 */
/* MMU_INT_MASK */
#define IEP_MMU_INT_MASK_PAGE_FAULT_INT_EN_SHIFT           (0U)
#define IEP_MMU_INT_MASK_PAGE_FAULT_INT_EN_MASK            (0x1U << IEP_MMU_INT_MASK_PAGE_FAULT_INT_EN_SHIFT)           /* 0x00000001 */
#define IEP_MMU_INT_MASK_READ_BUS_ERROR_INT_EN_SHIFT       (1U)
#define IEP_MMU_INT_MASK_READ_BUS_ERROR_INT_EN_MASK        (0x1U << IEP_MMU_INT_MASK_READ_BUS_ERROR_INT_EN_SHIFT)       /* 0x00000002 */
/* MMU_INT_STATUS */
#define IEP_MMU_INT_STATUS_PAGE_FAULT_SHIFT                (0U)
#define IEP_MMU_INT_STATUS_PAGE_FAULT_MASK                 (0x1U << IEP_MMU_INT_STATUS_PAGE_FAULT_SHIFT)                /* 0x00000001 */
#define IEP_MMU_INT_STATUS_READ_BUS_ERROR_SHIFT            (1U)
#define IEP_MMU_INT_STATUS_READ_BUS_ERROR_MASK             (0x1U << IEP_MMU_INT_STATUS_READ_BUS_ERROR_SHIFT)            /* 0x00000002 */
/* MMU_AUTO_GATING */
#define IEP_MMU_AUTO_GATING_MMU_AUTO_GATING_SHIFT          (0U)
#define IEP_MMU_AUTO_GATING_MMU_AUTO_GATING_MASK           (0x1U << IEP_MMU_AUTO_GATING_MMU_AUTO_GATING_SHIFT)          /* 0x00000001 */
/******************************************RGA*******************************************/
/* SYS_CTRL */
#define RGA_SYS_CTRL_SW_CMD_OP_ST_P_SHIFT                  (0U)
#define RGA_SYS_CTRL_SW_CMD_OP_ST_P_MASK                   (0x1U << RGA_SYS_CTRL_SW_CMD_OP_ST_P_SHIFT)                  /* 0x00000001 */
#define RGA_SYS_CTRL_SW_CMD_MODE_SHIFT                     (1U)
#define RGA_SYS_CTRL_SW_CMD_MODE_MASK                      (0x1U << RGA_SYS_CTRL_SW_CMD_MODE_SHIFT)                     /* 0x00000002 */
#define RGA_SYS_CTRL_SW_AUTO_CKG_SHIFT                     (2U)
#define RGA_SYS_CTRL_SW_AUTO_CKG_MASK                      (0x1U << RGA_SYS_CTRL_SW_AUTO_CKG_SHIFT)                     /* 0x00000004 */
#define RGA_SYS_CTRL_SW_ACLK_SRESET_P_SHIFT                (3U)
#define RGA_SYS_CTRL_SW_ACLK_SRESET_P_MASK                 (0x1U << RGA_SYS_CTRL_SW_ACLK_SRESET_P_SHIFT)                /* 0x00000008 */
#define RGA_SYS_CTRL_SW_CCLK_SRESET_P_SHIFT                (4U)
#define RGA_SYS_CTRL_SW_CCLK_SRESET_P_MASK                 (0x1U << RGA_SYS_CTRL_SW_CCLK_SRESET_P_SHIFT)                /* 0x00000010 */
#define RGA_SYS_CTRL_SW_AUTO_RST_SHIFT                     (5U)
#define RGA_SYS_CTRL_SW_AUTO_RST_MASK                      (0x1U << RGA_SYS_CTRL_SW_AUTO_RST_SHIFT)                     /* 0x00000020 */
#define RGA_SYS_CTRL_SW_RST_PROTECT_E_SHIFT                (6U)
#define RGA_SYS_CTRL_SW_RST_PROTECT_E_MASK                 (0x1U << RGA_SYS_CTRL_SW_RST_PROTECT_E_SHIFT)                /* 0x00000040 */
#define RGA_SYS_CTRL_SW_RST_HANDSAVE_P_SHIFT               (7U)
#define RGA_SYS_CTRL_SW_RST_HANDSAVE_P_MASK                (0x1U << RGA_SYS_CTRL_SW_RST_HANDSAVE_P_SHIFT)               /* 0x00000080 */
/* CMD_CTRL */
#define RGA_CMD_CTRL_SW_CMD_LINE_ST_P_SHIFT                (0U)
#define RGA_CMD_CTRL_SW_CMD_LINE_ST_P_MASK                 (0x1U << RGA_CMD_CTRL_SW_CMD_LINE_ST_P_SHIFT)                /* 0x00000001 */
#define RGA_CMD_CTRL_SW_CMD_INCR_VALID_P_SHIFT             (1U)
#define RGA_CMD_CTRL_SW_CMD_INCR_VALID_P_MASK              (0x1U << RGA_CMD_CTRL_SW_CMD_INCR_VALID_P_SHIFT)             /* 0x00000002 */
#define RGA_CMD_CTRL_SW_CMD_STOP_SHIFT                     (2U)
#define RGA_CMD_CTRL_SW_CMD_STOP_MASK                      (0x1U << RGA_CMD_CTRL_SW_CMD_STOP_SHIFT)                     /* 0x00000004 */
#define RGA_CMD_CTRL_SW_CMD_INCR_NUM_SHIFT                 (3U)
#define RGA_CMD_CTRL_SW_CMD_INCR_NUM_MASK                  (0x3FFU << RGA_CMD_CTRL_SW_CMD_INCR_NUM_SHIFT)               /* 0x00001FF8 */
/* CMD_BASE */
#define RGA_CMD_BASE_SW_CMD_BASE_SHIFT                     (0U)
#define RGA_CMD_BASE_SW_CMD_BASE_MASK                      (0xFFFFFFFFU << RGA_CMD_BASE_SW_CMD_BASE_SHIFT)              /* 0xFFFFFFFF */
/* STATUS1 */
#define RGA_STATUS1_SW_RGA_STA_SHIFT                       (0U)
#define RGA_STATUS1_SW_RGA_STA_MASK                        (0x1U << RGA_STATUS1_SW_RGA_STA_SHIFT)                       /* 0x00000001 */
#define RGA_STATUS1_RESERVED_SHIFT                         (1U)
#define RGA_STATUS1_RESERVED_MASK                          (0x7FU << RGA_STATUS1_RESERVED_SHIFT)                        /* 0x000000FE */
#define RGA_STATUS1_SW_CMD_CUR_NUM_SHIFT                   (8U)
#define RGA_STATUS1_SW_CMD_CUR_NUM_MASK                    (0xFFFU << RGA_STATUS1_SW_CMD_CUR_NUM_SHIFT)                 /* 0x000FFF00 */
#define RGA_STATUS1_SW_CMD_TOTAL_NUM_SHIFT                 (20U)
#define RGA_STATUS1_SW_CMD_TOTAL_NUM_MASK                  (0xFFFU << RGA_STATUS1_SW_CMD_TOTAL_NUM_SHIFT)               /* 0xFFF00000 */
/* INT */
#define RGA_INT_SW_INTR_ERR_SHIFT                          (0U)
#define RGA_INT_SW_INTR_ERR_MASK                           (0x1U << RGA_INT_SW_INTR_ERR_SHIFT)                          /* 0x00000001 */
#define RGA_INT_SW_INTR_MMU_SHIFT                          (1U)
#define RGA_INT_SW_INTR_MMU_MASK                           (0x1U << RGA_INT_SW_INTR_MMU_SHIFT)                          /* 0x00000002 */
#define RGA_INT_SW_INTR_AF_SHIFT                           (2U)
#define RGA_INT_SW_INTR_AF_MASK                            (0x1U << RGA_INT_SW_INTR_AF_SHIFT)                           /* 0x00000004 */
#define RGA_INT_SW_INTR_CF_SHIFT                           (3U)
#define RGA_INT_SW_INTR_CF_MASK                            (0x1U << RGA_INT_SW_INTR_CF_SHIFT)                           /* 0x00000008 */
#define RGA_INT_SW_INTR_ERR_CLR_SHIFT                      (4U)
#define RGA_INT_SW_INTR_ERR_CLR_MASK                       (0x1U << RGA_INT_SW_INTR_ERR_CLR_SHIFT)                      /* 0x00000010 */
#define RGA_INT_SW_INTR_MMU_CLR_SHIFT                      (5U)
#define RGA_INT_SW_INTR_MMU_CLR_MASK                       (0x1U << RGA_INT_SW_INTR_MMU_CLR_SHIFT)                      /* 0x00000020 */
#define RGA_INT_SW_INTR_AF_CLR_SHIFT                       (6U)
#define RGA_INT_SW_INTR_AF_CLR_MASK                        (0x1U << RGA_INT_SW_INTR_AF_CLR_SHIFT)                       /* 0x00000040 */
#define RGA_INT_SW_INTR_CF_CLR_SHIFT                       (7U)
#define RGA_INT_SW_INTR_CF_CLR_MASK                        (0x1U << RGA_INT_SW_INTR_CF_CLR_SHIFT)                       /* 0x00000080 */
#define RGA_INT_SW_INTR_ERR_E_SHIFT                        (8U)
#define RGA_INT_SW_INTR_ERR_E_MASK                         (0x1U << RGA_INT_SW_INTR_ERR_E_SHIFT)                        /* 0x00000100 */
#define RGA_INT_SW_INTR_MMU_E_SHIFT                        (9U)
#define RGA_INT_SW_INTR_MMU_E_MASK                         (0x1U << RGA_INT_SW_INTR_MMU_E_SHIFT)                        /* 0x00000200 */
#define RGA_INT_SW_INTR_AF_E_SHIFT                         (10U)
#define RGA_INT_SW_INTR_AF_E_MASK                          (0x1U << RGA_INT_SW_INTR_AF_E_SHIFT)                         /* 0x00000400 */
/* MMU_CTRL0 */
#define RGA_MMU_CTRL0_SW_MMU_PAGE_SIZE_SHIFT               (0U)
#define RGA_MMU_CTRL0_SW_MMU_PAGE_SIZE_MASK                (0x1U << RGA_MMU_CTRL0_SW_MMU_PAGE_SIZE_SHIFT)               /* 0x00000001 */
#define RGA_MMU_CTRL0_SW_CMD_MMU_EN_SHIFT                  (1U)
#define RGA_MMU_CTRL0_SW_CMD_MMU_EN_MASK                   (0x1U << RGA_MMU_CTRL0_SW_CMD_MMU_EN_SHIFT)                  /* 0x00000002 */
#define RGA_MMU_CTRL0_SW_CMD_MMU_FLUSH_SHIFT               (2U)
#define RGA_MMU_CTRL0_SW_CMD_MMU_FLUSH_MASK                (0x1U << RGA_MMU_CTRL0_SW_CMD_MMU_FLUSH_SHIFT)               /* 0x00000004 */
#define RGA_MMU_CTRL0_SW_SRC_CH_PRIORITY_SHIFT             (3U)
#define RGA_MMU_CTRL0_SW_SRC_CH_PRIORITY_MASK              (0x3U << RGA_MMU_CTRL0_SW_SRC_CH_PRIORITY_SHIFT)             /* 0x00000018 */
#define RGA_MMU_CTRL0_SW_SRC1_CH_PRIORITY_SHIFT            (5U)
#define RGA_MMU_CTRL0_SW_SRC1_CH_PRIORITY_MASK             (0x3U << RGA_MMU_CTRL0_SW_SRC1_CH_PRIORITY_SHIFT)            /* 0x00000060 */
#define RGA_MMU_CTRL0_SW_DST_CH_PRIORITY_SHIFT             (7U)
#define RGA_MMU_CTRL0_SW_DST_CH_PRIORITY_MASK              (0x3U << RGA_MMU_CTRL0_SW_DST_CH_PRIORITY_SHIFT)             /* 0x00000180 */
#define RGA_MMU_CTRL0_SW_ELS_CH_PRIORITY_SHIFT             (9U)
#define RGA_MMU_CTRL0_SW_ELS_CH_PRIORITY_MASK              (0x3U << RGA_MMU_CTRL0_SW_ELS_CH_PRIORITY_SHIFT)             /* 0x00000600 */
#define RGA_MMU_CTRL0_RESERVED_SHIFT                       (11U)
#define RGA_MMU_CTRL0_RESERVED_MASK                        (0x1FFFFFU << RGA_MMU_CTRL0_RESERVED_SHIFT)                  /* 0xFFFFF800 */
/* MMU_CMD_BASE */
#define RGA_MMU_CMD_BASE_SW_MMU_CMD_BASE_SHIFT             (0U)
#define RGA_MMU_CMD_BASE_SW_MMU_CMD_BASE_MASK              (0xFFFFFFFU << RGA_MMU_CMD_BASE_SW_MMU_CMD_BASE_SHIFT)       /* 0x0FFFFFFF */
/* STATUS2 */
#define RGA_STATUS2_RPP_ERROR_SHIFT                        (0U)
#define RGA_STATUS2_RPP_ERROR_MASK                         (0x1U << RGA_STATUS2_RPP_ERROR_SHIFT)                        /* 0x00000001 */
#define RGA_STATUS2_BUS_ERROR_SHIFT                        (1U)
#define RGA_STATUS2_BUS_ERROR_MASK                         (0x1U << RGA_STATUS2_BUS_ERROR_SHIFT)                        /* 0x00000002 */
#define RGA_STATUS2_SRCRPP_OUTBUF_RREADY_SHIFT             (2U)
#define RGA_STATUS2_SRCRPP_OUTBUF_RREADY_MASK              (0xFU << RGA_STATUS2_SRCRPP_OUTBUF_RREADY_SHIFT)             /* 0x0000003C */
#define RGA_STATUS2_DSTRPP_OUTBUF_RREADY_SHIFT             (6U)
#define RGA_STATUS2_DSTRPP_OUTBUF_RREADY_MASK              (0x1FU << RGA_STATUS2_DSTRPP_OUTBUF_RREADY_SHIFT)            /* 0x000007C0 */
#define RGA_STATUS2_RPP_MKRAM_RREADY_SHIFT                 (11U)
#define RGA_STATUS2_RPP_MKRAM_RREADY_MASK                  (0x3U << RGA_STATUS2_RPP_MKRAM_RREADY_SHIFT)                 /* 0x00001800 */
#define RGA_STATUS2_RESERVED_SHIFT                         (13U)
#define RGA_STATUS2_RESERVED_MASK                          (0x7FFFFU << RGA_STATUS2_RESERVED_SHIFT)                     /* 0xFFFFE000 */
/* WORK_CNT */
#define RGA_WORK_CNT_SW_WORK_CNT_SHIFT                     (0U)
#define RGA_WORK_CNT_SW_WORK_CNT_MASK                      (0x7FFFFFFU << RGA_WORK_CNT_SW_WORK_CNT_SHIFT)               /* 0x07FFFFFF */
#define RGA_WORK_CNT_RESERVED_SHIFT                        (27U)
#define RGA_WORK_CNT_RESERVED_MASK                         (0x1FU << RGA_WORK_CNT_RESERVED_SHIFT)                       /* 0xF8000000 */
/* VERSION_INFO */
#define RGA_VERSION_INFO_SVNBUILD_SHIFT                    (0U)
#define RGA_VERSION_INFO_SVNBUILD_MASK                     (0xFFFFFU << RGA_VERSION_INFO_SVNBUILD_SHIFT)                /* 0x000FFFFF */
#define RGA_VERSION_INFO_MINOR_SHIFT                       (20U)
#define RGA_VERSION_INFO_MINOR_MASK                        (0xFU << RGA_VERSION_INFO_MINOR_SHIFT)                       /* 0x00F00000 */
#define RGA_VERSION_INFO_MAJOR_SHIFT                       (24U)
#define RGA_VERSION_INFO_MAJOR_MASK                        (0xFFU << RGA_VERSION_INFO_MAJOR_SHIFT)                      /* 0xFF000000 */
/* PERF_LATENCY_CTRL0 */
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_WORK_E_SHIFT    (0U)
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_WORK_E_MASK     (0x1U << RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_WORK_E_SHIFT)    /* 0x00000001 */
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_CLR_E_SHIFT     (1U)
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_CLR_E_MASK      (0x1U << RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_CLR_E_SHIFT)     /* 0x00000002 */
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_FRM_TYPE_SHIFT  (2U)
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_FRM_TYPE_MASK   (0x1U << RGA_PERF_LATENCY_CTRL0_SW_AXI_PERF_FRM_TYPE_SHIFT)  /* 0x00000004 */
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_CNT_TYPE_SHIFT       (3U)
#define RGA_PERF_LATENCY_CTRL0_SW_AXI_CNT_TYPE_MASK        (0x1U << RGA_PERF_LATENCY_CTRL0_SW_AXI_CNT_TYPE_SHIFT)       /* 0x00000008 */
#define RGA_PERF_LATENCY_CTRL0_SW_RD_LATENCY_ID_SHIFT      (4U)
#define RGA_PERF_LATENCY_CTRL0_SW_RD_LATENCY_ID_MASK       (0xFU << RGA_PERF_LATENCY_CTRL0_SW_RD_LATENCY_ID_SHIFT)      /* 0x000000F0 */
#define RGA_PERF_LATENCY_CTRL0_SW_RD_LATENCY_THR_SHIFT     (8U)
#define RGA_PERF_LATENCY_CTRL0_SW_RD_LATENCY_THR_MASK      (0xFFFU << RGA_PERF_LATENCY_CTRL0_SW_RD_LATENCY_THR_SHIFT)   /* 0x000FFF00 */
/* PERF_LATENCY_CTRL1 */
#define RGA_PERF_LATENCY_CTRL1_SW_ADDR_ALIGN_TYPE_SHIFT    (0U)
#define RGA_PERF_LATENCY_CTRL1_SW_ADDR_ALIGN_TYPE_MASK     (0x3U << RGA_PERF_LATENCY_CTRL1_SW_ADDR_ALIGN_TYPE_SHIFT)    /* 0x00000003 */
#define RGA_PERF_LATENCY_CTRL1_SW_AR_CNT_ID_TYPE_SHIFT     (2U)
#define RGA_PERF_LATENCY_CTRL1_SW_AR_CNT_ID_TYPE_MASK      (0x1U << RGA_PERF_LATENCY_CTRL1_SW_AR_CNT_ID_TYPE_SHIFT)     /* 0x00000004 */
#define RGA_PERF_LATENCY_CTRL1_SW_AW_CNT_ID_TYPE_SHIFT     (3U)
#define RGA_PERF_LATENCY_CTRL1_SW_AW_CNT_ID_TYPE_MASK      (0x1U << RGA_PERF_LATENCY_CTRL1_SW_AW_CNT_ID_TYPE_SHIFT)     /* 0x00000008 */
#define RGA_PERF_LATENCY_CTRL1_SW_AR_COUNT_ID_SHIFT        (4U)
#define RGA_PERF_LATENCY_CTRL1_SW_AR_COUNT_ID_MASK         (0xFU << RGA_PERF_LATENCY_CTRL1_SW_AR_COUNT_ID_SHIFT)        /* 0x000000F0 */
#define RGA_PERF_LATENCY_CTRL1_SW_AW_COUNT_ID_SHIFT        (8U)
#define RGA_PERF_LATENCY_CTRL1_SW_AW_COUNT_ID_MASK         (0xFU << RGA_PERF_LATENCY_CTRL1_SW_AW_COUNT_ID_SHIFT)        /* 0x00000F00 */
/* PERF_RD_MAX_LATENCY_NUM0 */
#define RGA_PERF_RD_MAX_LATENCY_NUM0_RD_MAX_LATENCY_NUM_CH0_SHIFT (0U)
#define RGA_PERF_RD_MAX_LATENCY_NUM0_RD_MAX_LATENCY_NUM_CH0_MASK (0xFFFFU << RGA_PERF_RD_MAX_LATENCY_NUM0_RD_MAX_LATENCY_NUM_CH0_SHIFT) /* 0x0000FFFF */
/* PERF_RD_LATENCY_SAMP_NUM */
#define RGA_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_THR_NUM_CH0_SHIFT (0U)
#define RGA_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_THR_NUM_CH0_MASK (0xFFFFFFFFU << RGA_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_THR_NUM_CH0_SHIFT) /* 0xFFFFFFFF */
/* PERF_RD_LATENCY_ACC_SUM */
#define RGA_PERF_RD_LATENCY_ACC_SUM_RD_LATENCY_ACC_SUM_SHIFT (0U)
#define RGA_PERF_RD_LATENCY_ACC_SUM_RD_LATENCY_ACC_SUM_MASK (0xFFFFFFFFU << RGA_PERF_RD_LATENCY_ACC_SUM_RD_LATENCY_ACC_SUM_SHIFT) /* 0xFFFFFFFF */
/* PERF_RD_AXI_TOTAL_BYTE */
#define RGA_PERF_RD_AXI_TOTAL_BYTE_PERF_RD_AXI_TOTAL_BYTE_SHIFT (0U)
#define RGA_PERF_RD_AXI_TOTAL_BYTE_PERF_RD_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << RGA_PERF_RD_AXI_TOTAL_BYTE_PERF_RD_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* PERF_WR_AXI_TOTAL_BYTE */
#define RGA_PERF_WR_AXI_TOTAL_BYTE_PERF_WR_AXI_TOTAL_BYTE_SHIFT (0U)
#define RGA_PERF_WR_AXI_TOTAL_BYTE_PERF_WR_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << RGA_PERF_WR_AXI_TOTAL_BYTE_PERF_WR_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* PERF_WORKING_CNT */
#define RGA_PERF_WORKING_CNT_PERF_WORKING_CNT_SHIFT        (0U)
#define RGA_PERF_WORKING_CNT_PERF_WORKING_CNT_MASK         (0xFFFFFFFFU << RGA_PERF_WORKING_CNT_PERF_WORKING_CNT_SHIFT) /* 0xFFFFFFFF */
/* MODE_CTRL */
#define RGA_MODE_CTRL_SW_RENDER_MODE_SHIFT                 (0U)
#define RGA_MODE_CTRL_SW_RENDER_MODE_MASK                  (0x7U << RGA_MODE_CTRL_SW_RENDER_MODE_SHIFT)                 /* 0x00000007 */
#define RGA_MODE_CTRL_SW_BB_MODE_SHIFT                     (3U)
#define RGA_MODE_CTRL_SW_BB_MODE_MASK                      (0x1U << RGA_MODE_CTRL_SW_BB_MODE_SHIFT)                     /* 0x00000008 */
#define RGA_MODE_CTRL_SW_CF_ROP4_PAT_SHIFT                 (4U)
#define RGA_MODE_CTRL_SW_CF_ROP4_PAT_MASK                  (0x1U << RGA_MODE_CTRL_SW_CF_ROP4_PAT_SHIFT)                 /* 0x00000010 */
#define RGA_MODE_CTRL_SW_ALPHA_ZERO_KEY_SHIFT              (5U)
#define RGA_MODE_CTRL_SW_ALPHA_ZERO_KEY_MASK               (0x1U << RGA_MODE_CTRL_SW_ALPHA_ZERO_KEY_SHIFT)              /* 0x00000020 */
#define RGA_MODE_CTRL_SW_GRADIENT_SAT_SHIFT                (6U)
#define RGA_MODE_CTRL_SW_GRADIENT_SAT_MASK                 (0x1U << RGA_MODE_CTRL_SW_GRADIENT_SAT_SHIFT)                /* 0x00000040 */
#define RGA_MODE_CTRL_SW_INTR_CF_E_SHIFT                   (7U)
#define RGA_MODE_CTRL_SW_INTR_CF_E_MASK                    (0x1U << RGA_MODE_CTRL_SW_INTR_CF_E_SHIFT)                   /* 0x00000080 */
#define RGA_MODE_CTRL_RESERVED_SHIFT                       (8U)
#define RGA_MODE_CTRL_RESERVED_MASK                        (0xFFFFFFU << RGA_MODE_CTRL_RESERVED_SHIFT)                  /* 0xFFFFFF00 */
/* SRC_INFO */
#define RGA_SRC_INFO_SW_SRC_FMT_SHIFT                      (0U)
#define RGA_SRC_INFO_SW_SRC_FMT_MASK                       (0xFU << RGA_SRC_INFO_SW_SRC_FMT_SHIFT)                      /* 0x0000000F */
#define RGA_SRC_INFO_SW_SRC_RBSWAP_SHIFT                   (4U)
#define RGA_SRC_INFO_SW_SRC_RBSWAP_MASK                    (0x1U << RGA_SRC_INFO_SW_SRC_RBSWAP_SHIFT)                   /* 0x00000010 */
#define RGA_SRC_INFO_SW_SRC_ALPHA_SWAP_SHIFT               (5U)
#define RGA_SRC_INFO_SW_SRC_ALPHA_SWAP_MASK                (0x1U << RGA_SRC_INFO_SW_SRC_ALPHA_SWAP_SHIFT)               /* 0x00000020 */
#define RGA_SRC_INFO_SW_SRC_UVSWAP_SHIFT                   (6U)
#define RGA_SRC_INFO_SW_SRC_UVSWAP_MASK                    (0x1U << RGA_SRC_INFO_SW_SRC_UVSWAP_SHIFT)                   /* 0x00000040 */
#define RGA_SRC_INFO_SW_CP_ENDIAN_SHIFT                    (7U)
#define RGA_SRC_INFO_SW_CP_ENDIAN_MASK                     (0x1U << RGA_SRC_INFO_SW_CP_ENDIAN_SHIFT)                    /* 0x00000080 */
#define RGA_SRC_INFO_SW_SRC_CSC_MODE_SHIFT                 (8U)
#define RGA_SRC_INFO_SW_SRC_CSC_MODE_MASK                  (0x3U << RGA_SRC_INFO_SW_SRC_CSC_MODE_SHIFT)                 /* 0x00000300 */
#define RGA_SRC_INFO_SW_SRC_ROT_MODE_SHIFT                 (10U)
#define RGA_SRC_INFO_SW_SRC_ROT_MODE_MASK                  (0x3U << RGA_SRC_INFO_SW_SRC_ROT_MODE_SHIFT)                 /* 0x00000C00 */
#define RGA_SRC_INFO_SW_SRC_MIR_MODE_SHIFT                 (12U)
#define RGA_SRC_INFO_SW_SRC_MIR_MODE_MASK                  (0x3U << RGA_SRC_INFO_SW_SRC_MIR_MODE_SHIFT)                 /* 0x00003000 */
#define RGA_SRC_INFO_SW_SRC_HSCL_MODE_SHIFT                (14U)
#define RGA_SRC_INFO_SW_SRC_HSCL_MODE_MASK                 (0x3U << RGA_SRC_INFO_SW_SRC_HSCL_MODE_SHIFT)                /* 0x0000C000 */
#define RGA_SRC_INFO_SW_SRC_VSCL_MODE_SHIFT                (16U)
#define RGA_SRC_INFO_SW_SRC_VSCL_MODE_MASK                 (0x3U << RGA_SRC_INFO_SW_SRC_VSCL_MODE_SHIFT)                /* 0x00030000 */
#define RGA_SRC_INFO_SW_SRC_TRANS_MODE_SHIFT               (18U)
#define RGA_SRC_INFO_SW_SRC_TRANS_MODE_MASK                (0x1U << RGA_SRC_INFO_SW_SRC_TRANS_MODE_SHIFT)               /* 0x00040000 */
#define RGA_SRC_INFO_SW_SRC_TRANS_E_SHIFT                  (19U)
#define RGA_SRC_INFO_SW_SRC_TRANS_E_MASK                   (0xFU << RGA_SRC_INFO_SW_SRC_TRANS_E_SHIFT)                  /* 0x00780000 */
#define RGA_SRC_INFO_SW_SRC_DITHER_UP_SHIFT                (23U)
#define RGA_SRC_INFO_SW_SRC_DITHER_UP_MASK                 (0x1U << RGA_SRC_INFO_SW_SRC_DITHER_UP_SHIFT)                /* 0x00800000 */
#define RGA_SRC_INFO_SW_BIC_COE_SEL_SHIFT                  (24U)
#define RGA_SRC_INFO_SW_BIC_COE_SEL_MASK                   (0x3U << RGA_SRC_INFO_SW_BIC_COE_SEL_SHIFT)                  /* 0x03000000 */
#define RGA_SRC_INFO_SW_VSP_MODE_SHIFT                     (26U)
#define RGA_SRC_INFO_SW_VSP_MODE_MASK                      (0x1U << RGA_SRC_INFO_SW_VSP_MODE_SHIFT)                     /* 0x04000000 */
#define RGA_SRC_INFO_SW_SRC_YUV10_E_SHIFT                  (27U)
#define RGA_SRC_INFO_SW_SRC_YUV10_E_MASK                   (0x1U << RGA_SRC_INFO_SW_SRC_YUV10_E_SHIFT)                  /* 0x08000000 */
#define RGA_SRC_INFO_SW_SRC_YUV10_ROUND_E_SHIFT            (28U)
#define RGA_SRC_INFO_SW_SRC_YUV10_ROUND_E_MASK             (0x1U << RGA_SRC_INFO_SW_SRC_YUV10_ROUND_E_SHIFT)            /* 0x10000000 */
/* SRC_BASE0 */
#define RGA_SRC_BASE0_SW_SRC_BASE0_SHIFT                   (0U)
#define RGA_SRC_BASE0_SW_SRC_BASE0_MASK                    (0xFFFFFFFFU << RGA_SRC_BASE0_SW_SRC_BASE0_SHIFT)            /* 0xFFFFFFFF */
/* SRC_BASE1 */
#define RGA_SRC_BASE1_SW_SRC_BASE1_SHIFT                   (0U)
#define RGA_SRC_BASE1_SW_SRC_BASE1_MASK                    (0xFFFFFFFFU << RGA_SRC_BASE1_SW_SRC_BASE1_SHIFT)            /* 0xFFFFFFFF */
/* SRC_BASE2 */
#define RGA_SRC_BASE2_SW_SRC_BASE2_SHIFT                   (0U)
#define RGA_SRC_BASE2_SW_SRC_BASE2_MASK                    (0x1U << RGA_SRC_BASE2_SW_SRC_BASE2_SHIFT)                   /* 0x00000001 */
/* SRC_BASE3 */
#define RGA_SRC_BASE3_SW_SRC_BASE3_SHIFT                   (0U)
#define RGA_SRC_BASE3_SW_SRC_BASE3_MASK                    (0x1U << RGA_SRC_BASE3_SW_SRC_BASE3_SHIFT)                   /* 0x00000001 */
/* SRC_VIR_INFO */
#define RGA_SRC_VIR_INFO_SW_SRC_VIR_STRIDE_SHIFT           (0U)
#define RGA_SRC_VIR_INFO_SW_SRC_VIR_STRIDE_MASK            (0x7FFFU << RGA_SRC_VIR_INFO_SW_SRC_VIR_STRIDE_SHIFT)        /* 0x00007FFF */
#define RGA_SRC_VIR_INFO_RESERVED_SHIFT                    (15U)
#define RGA_SRC_VIR_INFO_RESERVED_MASK                     (0x1U << RGA_SRC_VIR_INFO_RESERVED_SHIFT)                    /* 0x00008000 */
#define RGA_SRC_VIR_INFO_SW_MASK_VIR_STRIDE_SHIFT          (16U)
#define RGA_SRC_VIR_INFO_SW_MASK_VIR_STRIDE_MASK           (0x3FFU << RGA_SRC_VIR_INFO_SW_MASK_VIR_STRIDE_SHIFT)        /* 0x03FF0000 */
#define RGA_SRC_VIR_INFO_FIELD0000_SHIFT                   (26U)
#define RGA_SRC_VIR_INFO_FIELD0000_MASK                    (0x3FU << RGA_SRC_VIR_INFO_FIELD0000_SHIFT)                  /* 0xFC000000 */
/* SRC_ACT_INFO */
#define RGA_SRC_ACT_INFO_SW_SRC_ACT_WIDTH_SHIFT            (0U)
#define RGA_SRC_ACT_INFO_SW_SRC_ACT_WIDTH_MASK             (0x1FFFU << RGA_SRC_ACT_INFO_SW_SRC_ACT_WIDTH_SHIFT)         /* 0x00001FFF */
#define RGA_SRC_ACT_INFO_RESERVED1_SHIFT                   (13U)
#define RGA_SRC_ACT_INFO_RESERVED1_MASK                    (0x7U << RGA_SRC_ACT_INFO_RESERVED1_SHIFT)                   /* 0x0000E000 */
#define RGA_SRC_ACT_INFO_SW_SRC_ACT_HEIGHT_SHIFT           (16U)
#define RGA_SRC_ACT_INFO_SW_SRC_ACT_HEIGHT_MASK            (0x1FFFU << RGA_SRC_ACT_INFO_SW_SRC_ACT_HEIGHT_SHIFT)        /* 0x1FFF0000 */
#define RGA_SRC_ACT_INFO_RESERVED2_SHIFT                   (29U)
#define RGA_SRC_ACT_INFO_RESERVED2_MASK                    (0x7U << RGA_SRC_ACT_INFO_RESERVED2_SHIFT)                   /* 0xE0000000 */
/* SRC_X_FACTOR */
#define RGA_SRC_X_FACTOR_SW_SRC_HSD_FACTOR_SHIFT           (0U)
#define RGA_SRC_X_FACTOR_SW_SRC_HSD_FACTOR_MASK            (0xFFFFU << RGA_SRC_X_FACTOR_SW_SRC_HSD_FACTOR_SHIFT)        /* 0x0000FFFF */
#define RGA_SRC_X_FACTOR_SW_SRC_HSP_FACTOR_SHIFT           (16U)
#define RGA_SRC_X_FACTOR_SW_SRC_HSP_FACTOR_MASK            (0xFFFFU << RGA_SRC_X_FACTOR_SW_SRC_HSP_FACTOR_SHIFT)        /* 0xFFFF0000 */
/* SRC_Y_FACTOR */
#define RGA_SRC_Y_FACTOR_SW_SRC_VSD_FACTOR_SHIFT           (0U)
#define RGA_SRC_Y_FACTOR_SW_SRC_VSD_FACTOR_MASK            (0xFFFFU << RGA_SRC_Y_FACTOR_SW_SRC_VSD_FACTOR_SHIFT)        /* 0x0000FFFF */
#define RGA_SRC_Y_FACTOR_SW_SRC_VSP_FACTOR_SHIFT           (16U)
#define RGA_SRC_Y_FACTOR_SW_SRC_VSP_FACTOR_MASK            (0xFFFFU << RGA_SRC_Y_FACTOR_SW_SRC_VSP_FACTOR_SHIFT)        /* 0xFFFF0000 */
/* SRC_BG_COLOR */
#define RGA_SRC_BG_COLOR_SW_SRC_BG_COLOR_SHIFT             (0U)
#define RGA_SRC_BG_COLOR_SW_SRC_BG_COLOR_MASK              (0xFFFFFFFFU << RGA_SRC_BG_COLOR_SW_SRC_BG_COLOR_SHIFT)      /* 0xFFFFFFFF */
/* SRC_FG_COLOR */
#define RGA_SRC_FG_COLOR_SW_SRC_FG_COLOR_SHIFT             (0U)
#define RGA_SRC_FG_COLOR_SW_SRC_FG_COLOR_MASK              (0xFFFFFFFFU << RGA_SRC_FG_COLOR_SW_SRC_FG_COLOR_SHIFT)      /* 0xFFFFFFFF */
/* SRC_TR_COLOR0 */
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_RMIN_SHIFT          (0U)
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_RMIN_MASK           (0xFFU << RGA_SRC_TR_COLOR0_SW_SRC_TRANS_RMIN_SHIFT)         /* 0x000000FF */
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_GMIN_SHIFT          (8U)
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_GMIN_MASK           (0xFFU << RGA_SRC_TR_COLOR0_SW_SRC_TRANS_GMIN_SHIFT)         /* 0x0000FF00 */
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_BMIN_SHIFT          (16U)
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_BMIN_MASK           (0xFFU << RGA_SRC_TR_COLOR0_SW_SRC_TRANS_BMIN_SHIFT)         /* 0x00FF0000 */
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_AMIN_SHIFT          (24U)
#define RGA_SRC_TR_COLOR0_SW_SRC_TRANS_AMIN_MASK           (0xFFU << RGA_SRC_TR_COLOR0_SW_SRC_TRANS_AMIN_SHIFT)         /* 0xFF000000 */
/* CP_GR_A */
#define RGA_CP_GR_A_SW_GRADIENT_X_A_SHIFT                  (0U)
#define RGA_CP_GR_A_SW_GRADIENT_X_A_MASK                   (0xFFFFU << RGA_CP_GR_A_SW_GRADIENT_X_A_SHIFT)               /* 0x0000FFFF */
#define RGA_CP_GR_A_SW_GRADIENT_Y_A_SHIFT                  (16U)
#define RGA_CP_GR_A_SW_GRADIENT_Y_A_MASK                   (0xFFFFU << RGA_CP_GR_A_SW_GRADIENT_Y_A_SHIFT)               /* 0xFFFF0000 */
/* SRC_TR_COLOR1 */
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_RMAX_SHIFT          (0U)
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_RMAX_MASK           (0xFFU << RGA_SRC_TR_COLOR1_SW_SRC_TRANS_RMAX_SHIFT)         /* 0x000000FF */
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_GMAX_SHIFT          (8U)
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_GMAX_MASK           (0xFFU << RGA_SRC_TR_COLOR1_SW_SRC_TRANS_GMAX_SHIFT)         /* 0x0000FF00 */
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_BMAX_SHIFT          (16U)
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_BMAX_MASK           (0xFFU << RGA_SRC_TR_COLOR1_SW_SRC_TRANS_BMAX_SHIFT)         /* 0x00FF0000 */
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_AMAX_SHIFT          (24U)
#define RGA_SRC_TR_COLOR1_SW_SRC_TRANS_AMAX_MASK           (0xFFU << RGA_SRC_TR_COLOR1_SW_SRC_TRANS_AMAX_SHIFT)         /* 0xFF000000 */
/* CP_GR_B */
#define RGA_CP_GR_B_SW_GRADIENT_X_B_SHIFT                  (0U)
#define RGA_CP_GR_B_SW_GRADIENT_X_B_MASK                   (0xFFFFU << RGA_CP_GR_B_SW_GRADIENT_X_B_SHIFT)               /* 0x0000FFFF */
#define RGA_CP_GR_B_SW_GRADIENT_Y_B_SHIFT                  (16U)
#define RGA_CP_GR_B_SW_GRADIENT_Y_B_MASK                   (0xFFFFU << RGA_CP_GR_B_SW_GRADIENT_Y_B_SHIFT)               /* 0xFFFF0000 */
/* DST_INFO */
#define RGA_DST_INFO_SW_DST_FMT_SHIFT                      (0U)
#define RGA_DST_INFO_SW_DST_FMT_MASK                       (0xFU << RGA_DST_INFO_SW_DST_FMT_SHIFT)                      /* 0x0000000F */
#define RGA_DST_INFO_SW_DST_RBSWAP_SHIFT                   (4U)
#define RGA_DST_INFO_SW_DST_RBSWAP_MASK                    (0x1U << RGA_DST_INFO_SW_DST_RBSWAP_SHIFT)                   /* 0x00000010 */
#define RGA_DST_INFO_SW_DST_ALPHA_SWAP_SHIFT               (5U)
#define RGA_DST_INFO_SW_DST_ALPHA_SWAP_MASK                (0x1U << RGA_DST_INFO_SW_DST_ALPHA_SWAP_SHIFT)               /* 0x00000020 */
#define RGA_DST_INFO_SW_DST_UVSWAP_SHIFT                   (6U)
#define RGA_DST_INFO_SW_DST_UVSWAP_MASK                    (0x1U << RGA_DST_INFO_SW_DST_UVSWAP_SHIFT)                   /* 0x00000040 */
#define RGA_DST_INFO_SW_SRC1_FMT_SHIFT                     (7U)
#define RGA_DST_INFO_SW_SRC1_FMT_MASK                      (0x7U << RGA_DST_INFO_SW_SRC1_FMT_SHIFT)                     /* 0x00000380 */
#define RGA_DST_INFO_SW_SRC1_RBSWAP_SHIFT                  (10U)
#define RGA_DST_INFO_SW_SRC1_RBSWAP_MASK                   (0x1U << RGA_DST_INFO_SW_SRC1_RBSWAP_SHIFT)                  /* 0x00000400 */
#define RGA_DST_INFO_SW_SRC1_ALPHA_SWAP_SHIFT              (11U)
#define RGA_DST_INFO_SW_SRC1_ALPHA_SWAP_MASK               (0x1U << RGA_DST_INFO_SW_SRC1_ALPHA_SWAP_SHIFT)              /* 0x00000800 */
#define RGA_DST_INFO_SW_SRC1_DITHER_UP_SHIFT               (12U)
#define RGA_DST_INFO_SW_SRC1_DITHER_UP_MASK                (0x1U << RGA_DST_INFO_SW_SRC1_DITHER_UP_SHIFT)               /* 0x00001000 */
#define RGA_DST_INFO_SW_DITHER_DOWN_SHIFT                  (13U)
#define RGA_DST_INFO_SW_DITHER_DOWN_MASK                   (0x1U << RGA_DST_INFO_SW_DITHER_DOWN_SHIFT)                  /* 0x00002000 */
#define RGA_DST_INFO_SW_DITHER_MODE_SHIFT                  (14U)
#define RGA_DST_INFO_SW_DITHER_MODE_MASK                   (0x3U << RGA_DST_INFO_SW_DITHER_MODE_SHIFT)                  /* 0x0000C000 */
#define RGA_DST_INFO_SW_DST_CSC_MODE_SHIFT                 (16U)
#define RGA_DST_INFO_SW_DST_CSC_MODE_MASK                  (0x3U << RGA_DST_INFO_SW_DST_CSC_MODE_SHIFT)                 /* 0x00030000 */
#define RGA_DST_INFO_SW_DST_CSC_CLIP_SHIFT                 (18U)
#define RGA_DST_INFO_SW_DST_CSC_CLIP_MASK                  (0x1U << RGA_DST_INFO_SW_DST_CSC_CLIP_SHIFT)                 /* 0x00040000 */
/* DST_BASE0 */
#define RGA_DST_BASE0_SW_DST_BASE0_SHIFT                   (0U)
#define RGA_DST_BASE0_SW_DST_BASE0_MASK                    (0xFFFFFFFFU << RGA_DST_BASE0_SW_DST_BASE0_SHIFT)            /* 0xFFFFFFFF */
/* DST_BASE1 */
#define RGA_DST_BASE1_SW_DST_BASE1_SHIFT                   (0U)
#define RGA_DST_BASE1_SW_DST_BASE1_MASK                    (0xFFFFFFFFU << RGA_DST_BASE1_SW_DST_BASE1_SHIFT)            /* 0xFFFFFFFF */
/* DST_BASE2 */
#define RGA_DST_BASE2_SW_DST_BASE2_SHIFT                   (0U)
#define RGA_DST_BASE2_SW_DST_BASE2_MASK                    (0xFFFFFFFFU << RGA_DST_BASE2_SW_DST_BASE2_SHIFT)            /* 0xFFFFFFFF */
/* DST_VIR_INFO */
#define RGA_DST_VIR_INFO_SW_DST_VIR_STRIDE_SHIFT           (0U)
#define RGA_DST_VIR_INFO_SW_DST_VIR_STRIDE_MASK            (0xFFFU << RGA_DST_VIR_INFO_SW_DST_VIR_STRIDE_SHIFT)         /* 0x00000FFF */
#define RGA_DST_VIR_INFO_RESERVED1_SHIFT                   (12U)
#define RGA_DST_VIR_INFO_RESERVED1_MASK                    (0xFU << RGA_DST_VIR_INFO_RESERVED1_SHIFT)                   /* 0x0000F000 */
#define RGA_DST_VIR_INFO_SW_SRC1_VIR_STRIDE_SHIFT          (16U)
#define RGA_DST_VIR_INFO_SW_SRC1_VIR_STRIDE_MASK           (0xFFFU << RGA_DST_VIR_INFO_SW_SRC1_VIR_STRIDE_SHIFT)        /* 0x0FFF0000 */
#define RGA_DST_VIR_INFO_RESERVED2_SHIFT                   (28U)
#define RGA_DST_VIR_INFO_RESERVED2_MASK                    (0xFU << RGA_DST_VIR_INFO_RESERVED2_SHIFT)                   /* 0xF0000000 */
/* DST_ACT_INFO */
#define RGA_DST_ACT_INFO_SW_DST_ACT_WIDTH_SHIFT            (0U)
#define RGA_DST_ACT_INFO_SW_DST_ACT_WIDTH_MASK             (0xFFFU << RGA_DST_ACT_INFO_SW_DST_ACT_WIDTH_SHIFT)          /* 0x00000FFF */
#define RGA_DST_ACT_INFO_RESERVED1_SHIFT                   (12U)
#define RGA_DST_ACT_INFO_RESERVED1_MASK                    (0xFU << RGA_DST_ACT_INFO_RESERVED1_SHIFT)                   /* 0x0000F000 */
#define RGA_DST_ACT_INFO_SW_DST_ACT_HEIGHT_SHIFT           (16U)
#define RGA_DST_ACT_INFO_SW_DST_ACT_HEIGHT_MASK            (0xFFFU << RGA_DST_ACT_INFO_SW_DST_ACT_HEIGHT_SHIFT)         /* 0x0FFF0000 */
#define RGA_DST_ACT_INFO_RESERVED2_SHIFT                   (28U)
#define RGA_DST_ACT_INFO_RESERVED2_MASK                    (0xFU << RGA_DST_ACT_INFO_RESERVED2_SHIFT)                   /* 0xF0000000 */
/* ALPHA_CTRL0 */
#define RGA_ALPHA_CTRL0_SW_ALPHA_ROP_E_SHIFT               (0U)
#define RGA_ALPHA_CTRL0_SW_ALPHA_ROP_E_MASK                (0x1U << RGA_ALPHA_CTRL0_SW_ALPHA_ROP_E_SHIFT)               /* 0x00000001 */
#define RGA_ALPHA_CTRL0_SW_ALPHA_ROP_SEL_SHIFT             (1U)
#define RGA_ALPHA_CTRL0_SW_ALPHA_ROP_SEL_MASK              (0x1U << RGA_ALPHA_CTRL0_SW_ALPHA_ROP_SEL_SHIFT)             /* 0x00000002 */
#define RGA_ALPHA_CTRL0_SW_ROP_MODE_SHIFT                  (2U)
#define RGA_ALPHA_CTRL0_SW_ROP_MODE_MASK                   (0x3U << RGA_ALPHA_CTRL0_SW_ROP_MODE_SHIFT)                  /* 0x0000000C */
#define RGA_ALPHA_CTRL0_SW_SRC_GLOBAL_ALPHA_SHIFT          (4U)
#define RGA_ALPHA_CTRL0_SW_SRC_GLOBAL_ALPHA_MASK           (0xFFU << RGA_ALPHA_CTRL0_SW_SRC_GLOBAL_ALPHA_SHIFT)         /* 0x00000FF0 */
#define RGA_ALPHA_CTRL0_SW_DST_GLOBAL_ALPHA_SHIFT          (12U)
#define RGA_ALPHA_CTRL0_SW_DST_GLOBAL_ALPHA_MASK           (0xFFU << RGA_ALPHA_CTRL0_SW_DST_GLOBAL_ALPHA_SHIFT)         /* 0x000FF000 */
#define RGA_ALPHA_CTRL0_SW_MASK_ENDIAN_SHIFT               (20U)
#define RGA_ALPHA_CTRL0_SW_MASK_ENDIAN_MASK                (0x1U << RGA_ALPHA_CTRL0_SW_MASK_ENDIAN_SHIFT)               /* 0x00100000 */
#define RGA_ALPHA_CTRL0_RESERVED_SHIFT                     (21U)
#define RGA_ALPHA_CTRL0_RESERVED_MASK                      (0x7FFU << RGA_ALPHA_CTRL0_RESERVED_SHIFT)                   /* 0xFFE00000 */
/* ALPHA_CTRL1 */
#define RGA_ALPHA_CTRL1_SW_DST_COLOR_M0_SHIFT              (0U)
#define RGA_ALPHA_CTRL1_SW_DST_COLOR_M0_MASK               (0x1U << RGA_ALPHA_CTRL1_SW_DST_COLOR_M0_SHIFT)              /* 0x00000001 */
#define RGA_ALPHA_CTRL1_SW_SRC_COLOR_M0_SHIFT              (1U)
#define RGA_ALPHA_CTRL1_SW_SRC_COLOR_M0_MASK               (0x1U << RGA_ALPHA_CTRL1_SW_SRC_COLOR_M0_SHIFT)              /* 0x00000002 */
#define RGA_ALPHA_CTRL1_SW_DST_FACTOR_M0_SHIFT             (2U)
#define RGA_ALPHA_CTRL1_SW_DST_FACTOR_M0_MASK              (0x7U << RGA_ALPHA_CTRL1_SW_DST_FACTOR_M0_SHIFT)             /* 0x0000001C */
#define RGA_ALPHA_CTRL1_SW_SRC_FACTOR_M0_SHIFT             (5U)
#define RGA_ALPHA_CTRL1_SW_SRC_FACTOR_M0_MASK              (0x7U << RGA_ALPHA_CTRL1_SW_SRC_FACTOR_M0_SHIFT)             /* 0x000000E0 */
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_CAL_M0_SHIFT          (8U)
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_CAL_M0_MASK           (0x1U << RGA_ALPHA_CTRL1_SW_DST_ALPHA_CAL_M0_SHIFT)          /* 0x00000100 */
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_CAL_M0_SHIFT          (9U)
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_CAL_M0_MASK           (0x1U << RGA_ALPHA_CTRL1_SW_SRC_ALPHA_CAL_M0_SHIFT)          /* 0x00000200 */
#define RGA_ALPHA_CTRL1_SW_DST_BLEND_M0_SHIFT              (10U)
#define RGA_ALPHA_CTRL1_SW_DST_BLEND_M0_MASK               (0x3U << RGA_ALPHA_CTRL1_SW_DST_BLEND_M0_SHIFT)              /* 0x00000C00 */
#define RGA_ALPHA_CTRL1_SW_SRC_BLEND_M0_SHIFT              (12U)
#define RGA_ALPHA_CTRL1_SW_SRC_BLEND_M0_MASK               (0x3U << RGA_ALPHA_CTRL1_SW_SRC_BLEND_M0_SHIFT)              /* 0x00003000 */
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_M0_SHIFT              (14U)
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_M0_MASK               (0x1U << RGA_ALPHA_CTRL1_SW_DST_ALPHA_M0_SHIFT)              /* 0x00004000 */
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_M0_SHIFT              (15U)
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_M0_MASK               (0x1U << RGA_ALPHA_CTRL1_SW_SRC_ALPHA_M0_SHIFT)              /* 0x00008000 */
#define RGA_ALPHA_CTRL1_SW_DST_FACTOR_M1_SHIFT             (16U)
#define RGA_ALPHA_CTRL1_SW_DST_FACTOR_M1_MASK              (0x7U << RGA_ALPHA_CTRL1_SW_DST_FACTOR_M1_SHIFT)             /* 0x00070000 */
#define RGA_ALPHA_CTRL1_W_SRC_FACTOR_M1_SHIFT              (19U)
#define RGA_ALPHA_CTRL1_W_SRC_FACTOR_M1_MASK               (0x7U << RGA_ALPHA_CTRL1_W_SRC_FACTOR_M1_SHIFT)              /* 0x00380000 */
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_CAL_M1_SHIFT          (22U)
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_CAL_M1_MASK           (0x1U << RGA_ALPHA_CTRL1_SW_DST_ALPHA_CAL_M1_SHIFT)          /* 0x00400000 */
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_CAL_M1_SHIFT          (23U)
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_CAL_M1_MASK           (0x1U << RGA_ALPHA_CTRL1_SW_SRC_ALPHA_CAL_M1_SHIFT)          /* 0x00800000 */
#define RGA_ALPHA_CTRL1_SW_DST_BLEND_M1_SHIFT              (24U)
#define RGA_ALPHA_CTRL1_SW_DST_BLEND_M1_MASK               (0x3U << RGA_ALPHA_CTRL1_SW_DST_BLEND_M1_SHIFT)              /* 0x03000000 */
#define RGA_ALPHA_CTRL1_SW_SRC_BLEND_M1_SHIFT              (26U)
#define RGA_ALPHA_CTRL1_SW_SRC_BLEND_M1_MASK               (0x3U << RGA_ALPHA_CTRL1_SW_SRC_BLEND_M1_SHIFT)              /* 0x0C000000 */
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_M1_SHIFT              (28U)
#define RGA_ALPHA_CTRL1_SW_DST_ALPHA_M1_MASK               (0x1U << RGA_ALPHA_CTRL1_SW_DST_ALPHA_M1_SHIFT)              /* 0x10000000 */
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_M1_SHIFT              (29U)
#define RGA_ALPHA_CTRL1_SW_SRC_ALPHA_M1_MASK               (0x1U << RGA_ALPHA_CTRL1_SW_SRC_ALPHA_M1_SHIFT)              /* 0x20000000 */
#define RGA_ALPHA_CTRL1_RESERVED_SHIFT                     (30U)
#define RGA_ALPHA_CTRL1_RESERVED_MASK                      (0x3U << RGA_ALPHA_CTRL1_RESERVED_SHIFT)                     /* 0xC0000000 */
/* FADING_CTRL */
#define RGA_FADING_CTRL_SW_FADING_OFFSET_R_SHIFT           (0U)
#define RGA_FADING_CTRL_SW_FADING_OFFSET_R_MASK            (0xFFU << RGA_FADING_CTRL_SW_FADING_OFFSET_R_SHIFT)          /* 0x000000FF */
#define RGA_FADING_CTRL_SW_FADING_OFFSET_G_SHIFT           (8U)
#define RGA_FADING_CTRL_SW_FADING_OFFSET_G_MASK            (0xFFU << RGA_FADING_CTRL_SW_FADING_OFFSET_G_SHIFT)          /* 0x0000FF00 */
#define RGA_FADING_CTRL_SW_FADING_OFFSET_B_SHIFT           (16U)
#define RGA_FADING_CTRL_SW_FADING_OFFSET_B_MASK            (0xFFU << RGA_FADING_CTRL_SW_FADING_OFFSET_B_SHIFT)          /* 0x00FF0000 */
#define RGA_FADING_CTRL_SW_FADING_EN_SHIFT                 (24U)
#define RGA_FADING_CTRL_SW_FADING_EN_MASK                  (0x1U << RGA_FADING_CTRL_SW_FADING_EN_SHIFT)                 /* 0x01000000 */
#define RGA_FADING_CTRL_RESERVED_SHIFT                     (25U)
#define RGA_FADING_CTRL_RESERVED_MASK                      (0x7FU << RGA_FADING_CTRL_RESERVED_SHIFT)                    /* 0xFE000000 */
/* PAT_CON */
#define RGA_PAT_CON_SW_PAT_WIDTH_SHIFT                     (0U)
#define RGA_PAT_CON_SW_PAT_WIDTH_MASK                      (0xFFU << RGA_PAT_CON_SW_PAT_WIDTH_SHIFT)                    /* 0x000000FF */
#define RGA_PAT_CON_SW_PAT_HEIGHT_SHIFT                    (8U)
#define RGA_PAT_CON_SW_PAT_HEIGHT_MASK                     (0xFFU << RGA_PAT_CON_SW_PAT_HEIGHT_SHIFT)                   /* 0x0000FF00 */
#define RGA_PAT_CON_SW_PAT_OFFSET_X_SHIFT                  (16U)
#define RGA_PAT_CON_SW_PAT_OFFSET_X_MASK                   (0xFFU << RGA_PAT_CON_SW_PAT_OFFSET_X_SHIFT)                 /* 0x00FF0000 */
#define RGA_PAT_CON_SW_PAT_OFFSET_Y_SHIFT                  (24U)
#define RGA_PAT_CON_SW_PAT_OFFSET_Y_MASK                   (0xFFU << RGA_PAT_CON_SW_PAT_OFFSET_Y_SHIFT)                 /* 0xFF000000 */
/* ROP_CON0 */
#define RGA_ROP_CON0_SW_ROP3_CODE0_SHIFT                   (0U)
#define RGA_ROP_CON0_SW_ROP3_CODE0_MASK                    (0x1FFFFFFU << RGA_ROP_CON0_SW_ROP3_CODE0_SHIFT)             /* 0x01FFFFFF */
#define RGA_ROP_CON0_RESERVED_SHIFT                        (25U)
#define RGA_ROP_CON0_RESERVED_MASK                         (0x7FU << RGA_ROP_CON0_RESERVED_SHIFT)                       /* 0xFE000000 */
/* CP_GR_G */
#define RGA_CP_GR_G_SW_GRADIENT_X_G_SHIFT                  (0U)
#define RGA_CP_GR_G_SW_GRADIENT_X_G_MASK                   (0xFFFFU << RGA_CP_GR_G_SW_GRADIENT_X_G_SHIFT)               /* 0x0000FFFF */
#define RGA_CP_GR_G_SW_GRADIENT_Y_G_SHIFT                  (16U)
#define RGA_CP_GR_G_SW_GRADIENT_Y_G_MASK                   (0xFFFFU << RGA_CP_GR_G_SW_GRADIENT_Y_G_SHIFT)               /* 0xFFFF0000 */
/* ROP_CON1 */
#define RGA_ROP_CON1_SW_ROP3_CODE1_SHIFT                   (0U)
#define RGA_ROP_CON1_SW_ROP3_CODE1_MASK                    (0x1FFFFFFU << RGA_ROP_CON1_SW_ROP3_CODE1_SHIFT)             /* 0x01FFFFFF */
#define RGA_ROP_CON1_RESERVED_SHIFT                        (25U)
#define RGA_ROP_CON1_RESERVED_MASK                         (0x7FU << RGA_ROP_CON1_RESERVED_SHIFT)                       /* 0xFE000000 */
/* CP_GR_R */
#define RGA_CP_GR_R_SW_GRADIENT_X_R_SHIFT                  (0U)
#define RGA_CP_GR_R_SW_GRADIENT_X_R_MASK                   (0xFFFFU << RGA_CP_GR_R_SW_GRADIENT_X_R_SHIFT)               /* 0x0000FFFF */
#define RGA_CP_GR_R_SW_GRADIENT_Y_R_SHIFT                  (16U)
#define RGA_CP_GR_R_SW_GRADIENT_Y_R_MASK                   (0xFFFFU << RGA_CP_GR_R_SW_GRADIENT_Y_R_SHIFT)               /* 0xFFFF0000 */
/* MASK_BASE */
#define RGA_MASK_BASE_SW_MASK_BASE_SHIFT                   (0U)
#define RGA_MASK_BASE_SW_MASK_BASE_MASK                    (0xFFFFFFFFU << RGA_MASK_BASE_SW_MASK_BASE_SHIFT)            /* 0xFFFFFFFF */
/* MMU_CTRL1 */
#define RGA_MMU_CTRL1_SW_SRC_MMU_EN_SHIFT                  (0U)
#define RGA_MMU_CTRL1_SW_SRC_MMU_EN_MASK                   (0x1U << RGA_MMU_CTRL1_SW_SRC_MMU_EN_SHIFT)                  /* 0x00000001 */
#define RGA_MMU_CTRL1_SW_SRC_MMU_FLUSH_SHIFT               (1U)
#define RGA_MMU_CTRL1_SW_SRC_MMU_FLUSH_MASK                (0x1U << RGA_MMU_CTRL1_SW_SRC_MMU_FLUSH_SHIFT)               /* 0x00000002 */
#define RGA_MMU_CTRL1_SW_SRC_MMU_PREFETCH_EN_SHIFT         (2U)
#define RGA_MMU_CTRL1_SW_SRC_MMU_PREFETCH_EN_MASK          (0x1U << RGA_MMU_CTRL1_SW_SRC_MMU_PREFETCH_EN_SHIFT)         /* 0x00000004 */
#define RGA_MMU_CTRL1_SW_SRC_MMU_PREFETCH_DIR_SHIFT        (3U)
#define RGA_MMU_CTRL1_SW_SRC_MMU_PREFETCH_DIR_MASK         (0x1U << RGA_MMU_CTRL1_SW_SRC_MMU_PREFETCH_DIR_SHIFT)        /* 0x00000008 */
#define RGA_MMU_CTRL1_SW_SRC1_MMU_EN_SHIFT                 (4U)
#define RGA_MMU_CTRL1_SW_SRC1_MMU_EN_MASK                  (0x1U << RGA_MMU_CTRL1_SW_SRC1_MMU_EN_SHIFT)                 /* 0x00000010 */
#define RGA_MMU_CTRL1_SW_SRC1_MMU_FLUSH_SHIFT              (5U)
#define RGA_MMU_CTRL1_SW_SRC1_MMU_FLUSH_MASK               (0x1U << RGA_MMU_CTRL1_SW_SRC1_MMU_FLUSH_SHIFT)              /* 0x00000020 */
#define RGA_MMU_CTRL1_SW_SRC1_MMU_PREFETCH_EN_SHIFT        (6U)
#define RGA_MMU_CTRL1_SW_SRC1_MMU_PREFETCH_EN_MASK         (0x1U << RGA_MMU_CTRL1_SW_SRC1_MMU_PREFETCH_EN_SHIFT)        /* 0x00000040 */
#define RGA_MMU_CTRL1_SW_SRC1_MMU_PREFETCH_DIR_SHIFT       (7U)
#define RGA_MMU_CTRL1_SW_SRC1_MMU_PREFETCH_DIR_MASK        (0x1U << RGA_MMU_CTRL1_SW_SRC1_MMU_PREFETCH_DIR_SHIFT)       /* 0x00000080 */
#define RGA_MMU_CTRL1_SW_DST_MMU_EN_SHIFT                  (8U)
#define RGA_MMU_CTRL1_SW_DST_MMU_EN_MASK                   (0x1U << RGA_MMU_CTRL1_SW_DST_MMU_EN_SHIFT)                  /* 0x00000100 */
#define RGA_MMU_CTRL1_SW_DST_MMU_FLUSH_SHIFT               (9U)
#define RGA_MMU_CTRL1_SW_DST_MMU_FLUSH_MASK                (0x1U << RGA_MMU_CTRL1_SW_DST_MMU_FLUSH_SHIFT)               /* 0x00000200 */
#define RGA_MMU_CTRL1_SW_DST_MMU_PREFETCH_EN_SHIFT         (10U)
#define RGA_MMU_CTRL1_SW_DST_MMU_PREFETCH_EN_MASK          (0x1U << RGA_MMU_CTRL1_SW_DST_MMU_PREFETCH_EN_SHIFT)         /* 0x00000400 */
#define RGA_MMU_CTRL1_SW_DST_MMU_PREFETCH_DIR_SHIFT        (11U)
#define RGA_MMU_CTRL1_SW_DST_MMU_PREFETCH_DIR_MASK         (0x1U << RGA_MMU_CTRL1_SW_DST_MMU_PREFETCH_DIR_SHIFT)        /* 0x00000800 */
#define RGA_MMU_CTRL1_SW_ELS_MMU_EN_SHIFT                  (12U)
#define RGA_MMU_CTRL1_SW_ELS_MMU_EN_MASK                   (0x1U << RGA_MMU_CTRL1_SW_ELS_MMU_EN_SHIFT)                  /* 0x00001000 */
#define RGA_MMU_CTRL1_SW_ELS_MMU_FLUSH_SHIFT               (13U)
#define RGA_MMU_CTRL1_SW_ELS_MMU_FLUSH_MASK                (0x1U << RGA_MMU_CTRL1_SW_ELS_MMU_FLUSH_SHIFT)               /* 0x00002000 */
#define RGA_MMU_CTRL1_RESERVED_SHIFT                       (14U)
#define RGA_MMU_CTRL1_RESERVED_MASK                        (0x3FFFFU << RGA_MMU_CTRL1_RESERVED_SHIFT)                   /* 0xFFFFC000 */
/* MMU_SRC_BASE */
#define RGA_MMU_SRC_BASE_SW_MMU_SRC_BASE_SHIFT             (0U)
#define RGA_MMU_SRC_BASE_SW_MMU_SRC_BASE_MASK              (0xFFFFFFFU << RGA_MMU_SRC_BASE_SW_MMU_SRC_BASE_SHIFT)       /* 0x0FFFFFFF */
/* MMU_SRC1_BASE */
#define RGA_MMU_SRC1_BASE_SW_MMU_SRC1_BASE_SHIFT           (0U)
#define RGA_MMU_SRC1_BASE_SW_MMU_SRC1_BASE_MASK            (0xFFFFFFFU << RGA_MMU_SRC1_BASE_SW_MMU_SRC1_BASE_SHIFT)     /* 0x0FFFFFFF */
/* MMU_DST_BASE */
#define RGA_MMU_DST_BASE_SW_MMU_DST_BASE_SHIFT             (0U)
#define RGA_MMU_DST_BASE_SW_MMU_DST_BASE_MASK              (0xFFFFFFFU << RGA_MMU_DST_BASE_SW_MMU_DST_BASE_SHIFT)       /* 0x0FFFFFFF */
/* MMU_ELS_BASE */
#define RGA_MMU_ELS_BASE_SW_MMU_ELS_BASE_SHIFT             (0U)
#define RGA_MMU_ELS_BASE_SW_MMU_ELS_BASE_MASK              (0xFFFFFFFU << RGA_MMU_ELS_BASE_SW_MMU_ELS_BASE_SHIFT)       /* 0x0FFFFFFF */
/*****************************************NANDC******************************************/
/* FMCTL */
#define NANDC_FMCTL_FCS0_SHIFT                             (0U)
#define NANDC_FMCTL_FCS0_MASK                              (0x1U << NANDC_FMCTL_FCS0_SHIFT)                             /* 0x00000001 */
#define NANDC_FMCTL_FCS1_SHIFT                             (1U)
#define NANDC_FMCTL_FCS1_MASK                              (0x1U << NANDC_FMCTL_FCS1_SHIFT)                             /* 0x00000002 */
#define NANDC_FMCTL_FCS2_SHIFT                             (2U)
#define NANDC_FMCTL_FCS2_MASK                              (0x1U << NANDC_FMCTL_FCS2_SHIFT)                             /* 0x00000004 */
#define NANDC_FMCTL_FCS3_SHIFT                             (3U)
#define NANDC_FMCTL_FCS3_MASK                              (0x1U << NANDC_FMCTL_FCS3_SHIFT)                             /* 0x00000008 */
#define NANDC_FMCTL_FCS4_SHIFT                             (4U)
#define NANDC_FMCTL_FCS4_MASK                              (0x1U << NANDC_FMCTL_FCS4_SHIFT)                             /* 0x00000010 */
#define NANDC_FMCTL_FCS5_SHIFT                             (5U)
#define NANDC_FMCTL_FCS5_MASK                              (0x1U << NANDC_FMCTL_FCS5_SHIFT)                             /* 0x00000020 */
#define NANDC_FMCTL_FCS6_SHIFT                             (6U)
#define NANDC_FMCTL_FCS6_MASK                              (0x1U << NANDC_FMCTL_FCS6_SHIFT)                             /* 0x00000040 */
#define NANDC_FMCTL_FCS7_SHIFT                             (7U)
#define NANDC_FMCTL_FCS7_MASK                              (0x1U << NANDC_FMCTL_FCS7_SHIFT)                             /* 0x00000080 */
#define NANDC_FMCTL_WP_SHIFT                               (8U)
#define NANDC_FMCTL_WP_MASK                                (0x1U << NANDC_FMCTL_WP_SHIFT)                               /* 0x00000100 */
#define NANDC_FMCTL_FRDY_SHIFT                             (9U)
#define NANDC_FMCTL_FRDY_MASK                              (0x1U << NANDC_FMCTL_FRDY_SHIFT)                             /* 0x00000200 */
#define NANDC_FMCTL_FIFO_EMPTY_SHIFT                       (10U)
#define NANDC_FMCTL_FIFO_EMPTY_MASK                        (0x1U << NANDC_FMCTL_FIFO_EMPTY_SHIFT)                       /* 0x00000400 */
#define NANDC_FMCTL_DWIDTH_SHIFT                           (12U)
#define NANDC_FMCTL_DWIDTH_MASK                            (0x1U << NANDC_FMCTL_DWIDTH_SHIFT)                           /* 0x00001000 */
#define NANDC_FMCTL_TM_SHIFT                               (13U)
#define NANDC_FMCTL_TM_MASK                                (0x1U << NANDC_FMCTL_TM_SHIFT)                               /* 0x00002000 */
#define NANDC_FMCTL_SYN_CLKEN_SHIFT                        (14U)
#define NANDC_FMCTL_SYN_CLKEN_MASK                         (0x1U << NANDC_FMCTL_SYN_CLKEN_SHIFT)                        /* 0x00004000 */
#define NANDC_FMCTL_SYN_MODE_SHIFT                         (15U)
#define NANDC_FMCTL_SYN_MODE_MASK                          (0x1U << NANDC_FMCTL_SYN_MODE_SHIFT)                         /* 0x00008000 */
#define NANDC_FMCTL_FLASH_ABORT_EN_SHIFT                   (16U)
#define NANDC_FMCTL_FLASH_ABORT_EN_MASK                    (0x1U << NANDC_FMCTL_FLASH_ABORT_EN_SHIFT)                   /* 0x00010000 */
#define NANDC_FMCTL_FLASH_ABORT_STAT_SHIFT                 (17U)
#define NANDC_FMCTL_FLASH_ABORT_STAT_MASK                  (0x1U << NANDC_FMCTL_FLASH_ABORT_STAT_SHIFT)                 /* 0x00020000 */
#define NANDC_FMCTL_READ_DELAY_SHIFT                       (24U)
#define NANDC_FMCTL_READ_DELAY_MASK                        (0x7U << NANDC_FMCTL_READ_DELAY_SHIFT)                       /* 0x07000000 */
/* FMWAIT_ASYN */
#define NANDC_FMWAIT_ASYN_RWCS_SHIFT                       (0U)
#define NANDC_FMWAIT_ASYN_RWCS_MASK                        (0x1FU << NANDC_FMWAIT_ASYN_RWCS_SHIFT)                      /* 0x0000001F */
#define NANDC_FMWAIT_ASYN_RWPW_SHIFT                       (5U)
#define NANDC_FMWAIT_ASYN_RWPW_MASK                        (0x3FU << NANDC_FMWAIT_ASYN_RWPW_SHIFT)                      /* 0x000007E0 */
#define NANDC_FMWAIT_ASYN_HARD_RDY_SHIFT                   (11U)
#define NANDC_FMWAIT_ASYN_HARD_RDY_MASK                    (0x1U << NANDC_FMWAIT_ASYN_HARD_RDY_SHIFT)                   /* 0x00000800 */
#define NANDC_FMWAIT_ASYN_CSRW_SHIFT                       (12U)
#define NANDC_FMWAIT_ASYN_CSRW_MASK                        (0x3FU << NANDC_FMWAIT_ASYN_CSRW_SHIFT)                      /* 0x0003F000 */
#define NANDC_FMWAIT_ASYN_WAIT_FRDY_DLY_SHIFT              (18U)
#define NANDC_FMWAIT_ASYN_WAIT_FRDY_DLY_MASK               (0x1FU << NANDC_FMWAIT_ASYN_WAIT_FRDY_DLY_SHIFT)             /* 0x007C0000 */
#define NANDC_FMWAIT_ASYN_FMW_DLY_SHIFT                    (24U)
#define NANDC_FMWAIT_ASYN_FMW_DLY_MASK                     (0x3FU << NANDC_FMWAIT_ASYN_FMW_DLY_SHIFT)                   /* 0x3F000000 */
#define NANDC_FMWAIT_ASYN_FMW_DLY_EN_SHIFT                 (30U)
#define NANDC_FMWAIT_ASYN_FMW_DLY_EN_MASK                  (0x1U << NANDC_FMWAIT_ASYN_FMW_DLY_EN_SHIFT)                 /* 0x40000000 */
/* FLCTL */
#define NANDC_FLCTL_FLASH_RST_SHIFT                        (0U)
#define NANDC_FLCTL_FLASH_RST_MASK                         (0x1U << NANDC_FLCTL_FLASH_RST_SHIFT)                        /* 0x00000001 */
#define NANDC_FLCTL_FLASH_RDN_SHIFT                        (1U)
#define NANDC_FLCTL_FLASH_RDN_MASK                         (0x1U << NANDC_FLCTL_FLASH_RDN_SHIFT)                        /* 0x00000002 */
#define NANDC_FLCTL_FLASH_ST_SHIFT                         (2U)
#define NANDC_FLCTL_FLASH_ST_MASK                          (0x1U << NANDC_FLCTL_FLASH_ST_SHIFT)                         /* 0x00000004 */
#define NANDC_FLCTL_BYPASS_SHIFT                           (3U)
#define NANDC_FLCTL_BYPASS_MASK                            (0x1U << NANDC_FLCTL_BYPASS_SHIFT)                           /* 0x00000008 */
#define NANDC_FLCTL_ST_ADDR_SHIFT                          (4U)
#define NANDC_FLCTL_ST_ADDR_MASK                           (0x1U << NANDC_FLCTL_ST_ADDR_SHIFT)                          /* 0x00000010 */
#define NANDC_FLCTL_TR_COUNT_SHIFT                         (5U)
#define NANDC_FLCTL_TR_COUNT_MASK                          (0x3U << NANDC_FLCTL_TR_COUNT_SHIFT)                         /* 0x00000060 */
#define NANDC_FLCTL_FLASH_ST_MOD_SHIFT                     (7U)
#define NANDC_FLCTL_FLASH_ST_MOD_MASK                      (0x1U << NANDC_FLCTL_FLASH_ST_MOD_SHIFT)                     /* 0x00000080 */
#define NANDC_FLCTL_COR_ABLE_SHIFT                         (10U)
#define NANDC_FLCTL_COR_ABLE_MASK                          (0x1U << NANDC_FLCTL_COR_ABLE_SHIFT)                         /* 0x00000400 */
#define NANDC_FLCTL_LBA_EN_SHIFT                           (11U)
#define NANDC_FLCTL_LBA_EN_MASK                            (0x1U << NANDC_FLCTL_LBA_EN_SHIFT)                           /* 0x00000800 */
#define NANDC_FLCTL_SPARE_SIZE_SHIFT                       (12U)
#define NANDC_FLCTL_SPARE_SIZE_MASK                        (0x7FU << NANDC_FLCTL_SPARE_SIZE_SHIFT)                      /* 0x0007F000 */
#define NANDC_FLCTL_TR_RDY_SHIFT                           (20U)
#define NANDC_FLCTL_TR_RDY_MASK                            (0x1U << NANDC_FLCTL_TR_RDY_SHIFT)                           /* 0x00100000 */
#define NANDC_FLCTL_PAGE_SIZE_SHIFT                        (21U)
#define NANDC_FLCTL_PAGE_SIZE_MASK                         (0x1U << NANDC_FLCTL_PAGE_SIZE_SHIFT)                        /* 0x00200000 */
#define NANDC_FLCTL_PAGE_NUM_SHIFT                         (22U)
#define NANDC_FLCTL_PAGE_NUM_MASK                          (0x3FU << NANDC_FLCTL_PAGE_NUM_SHIFT)                        /* 0x0FC00000 */
#define NANDC_FLCTL_LOW_POWER_SHIFT                        (28U)
#define NANDC_FLCTL_LOW_POWER_MASK                         (0x1U << NANDC_FLCTL_LOW_POWER_SHIFT)                        /* 0x10000000 */
#define NANDC_FLCTL_ASYNC_TOG_MIX_SHIFT                    (29U)
#define NANDC_FLCTL_ASYNC_TOG_MIX_MASK                     (0x1U << NANDC_FLCTL_ASYNC_TOG_MIX_SHIFT)                    /* 0x20000000 */
#define NANDC_FLCTL_BYPASS_FIFO_MODE_SHIFT                 (30U)
#define NANDC_FLCTL_BYPASS_FIFO_MODE_MASK                  (0x1U << NANDC_FLCTL_BYPASS_FIFO_MODE_SHIFT)                 /* 0x40000000 */
/* BCHCTL */
#define NANDC_BCHCTL_BCHRST_SHIFT                          (0U)
#define NANDC_BCHCTL_BCHRST_MASK                           (0x1U << NANDC_BCHCTL_BCHRST_SHIFT)                          /* 0x00000001 */
#define NANDC_BCHCTL_MODE_ADDRCARE_SHIFT                   (2U)
#define NANDC_BCHCTL_MODE_ADDRCARE_MASK                    (0x1U << NANDC_BCHCTL_MODE_ADDRCARE_SHIFT)                   /* 0x00000004 */
#define NANDC_BCHCTL_BCHEPD_SHIFT                          (3U)
#define NANDC_BCHCTL_BCHEPD_MASK                           (0x1U << NANDC_BCHCTL_BCHEPD_SHIFT)                          /* 0x00000008 */
#define NANDC_BCHCTL_BCHMODE0_SHIFT                        (4U)
#define NANDC_BCHCTL_BCHMODE0_MASK                         (0x1U << NANDC_BCHCTL_BCHMODE0_SHIFT)                        /* 0x00000010 */
#define NANDC_BCHCTL_REGION_SHIFT                          (5U)
#define NANDC_BCHCTL_REGION_MASK                           (0x7U << NANDC_BCHCTL_REGION_SHIFT)                          /* 0x000000E0 */
#define NANDC_BCHCTL_ADDR_SHIFT                            (8U)
#define NANDC_BCHCTL_ADDR_MASK                             (0xFFU << NANDC_BCHCTL_ADDR_SHIFT)                           /* 0x0000FF00 */
#define NANDC_BCHCTL_BCHPAGE_SHIFT                         (16U)
#define NANDC_BCHCTL_BCHPAGE_MASK                          (0x1U << NANDC_BCHCTL_BCHPAGE_SHIFT)                         /* 0x00010000 */
#define NANDC_BCHCTL_BCHMODE1_SHIFT                        (18U)
#define NANDC_BCHCTL_BCHMODE1_MASK                         (0x1U << NANDC_BCHCTL_BCHMODE1_SHIFT)                        /* 0x00040000 */
#define NANDC_BCHCTL_BCHTHRES_SHIFT                        (19U)
#define NANDC_BCHCTL_BCHTHRES_MASK                         (0xFFU << NANDC_BCHCTL_BCHTHRES_SHIFT)                       /* 0x07F80000 */
#define NANDC_BCHCTL_BCH_TODDR_SHIFT                       (28U)
#define NANDC_BCHCTL_BCH_TODDR_MASK                        (0x1U << NANDC_BCHCTL_BCH_TODDR_SHIFT)                       /* 0x10000000 */
/* MTRANS_CFG */
#define NANDC_MTRANS_CFG_AHB_WR_ST_SHIFT                   (0U)
#define NANDC_MTRANS_CFG_AHB_WR_ST_MASK                    (0x1U << NANDC_MTRANS_CFG_AHB_WR_ST_SHIFT)                   /* 0x00000001 */
#define NANDC_MTRANS_CFG_AHB_WR_SHIFT                      (1U)
#define NANDC_MTRANS_CFG_AHB_WR_MASK                       (0x1U << NANDC_MTRANS_CFG_AHB_WR_SHIFT)                      /* 0x00000002 */
#define NANDC_MTRANS_CFG_BUS_MODE_SHIFT                    (2U)
#define NANDC_MTRANS_CFG_BUS_MODE_MASK                     (0x1U << NANDC_MTRANS_CFG_BUS_MODE_SHIFT)                    /* 0x00000004 */
#define NANDC_MTRANS_CFG_HSIZE_SHIFT                       (3U)
#define NANDC_MTRANS_CFG_HSIZE_MASK                        (0x7U << NANDC_MTRANS_CFG_HSIZE_SHIFT)                       /* 0x00000038 */
#define NANDC_MTRANS_CFG_BURST_SHIFT                       (6U)
#define NANDC_MTRANS_CFG_BURST_MASK                        (0x7U << NANDC_MTRANS_CFG_BURST_SHIFT)                       /* 0x000001C0 */
#define NANDC_MTRANS_CFG_INCR_NUM_SHIFT                    (9U)
#define NANDC_MTRANS_CFG_INCR_NUM_MASK                     (0x1FU << NANDC_MTRANS_CFG_INCR_NUM_SHIFT)                   /* 0x00003E00 */
#define NANDC_MTRANS_CFG_FL_PWD_SHIFT                      (14U)
#define NANDC_MTRANS_CFG_FL_PWD_MASK                       (0x1U << NANDC_MTRANS_CFG_FL_PWD_SHIFT)                      /* 0x00004000 */
#define NANDC_MTRANS_CFG_AHB_RST_SHIFT                     (15U)
#define NANDC_MTRANS_CFG_AHB_RST_MASK                      (0x1U << NANDC_MTRANS_CFG_AHB_RST_SHIFT)                     /* 0x00008000 */
/* MTRANS_SADDR0 */
#define NANDC_MTRANS_SADDR0_SADDR0_SHIFT                   (0U)
#define NANDC_MTRANS_SADDR0_SADDR0_MASK                    (0xFFFFFFFFU << NANDC_MTRANS_SADDR0_SADDR0_SHIFT)            /* 0xFFFFFFFF */
/* MTRANS_SADDR1 */
#define NANDC_MTRANS_SADDR1_SADDR1_SHIFT                   (0U)
#define NANDC_MTRANS_SADDR1_SADDR1_MASK                    (0xFFFFFFFFU << NANDC_MTRANS_SADDR1_SADDR1_SHIFT)            /* 0xFFFFFFFF */
/* MTRANS_STAT */
#define NANDC_MTRANS_STAT_BUS_ERR_SHIFT                    (0U)
#define NANDC_MTRANS_STAT_BUS_ERR_MASK                     (0xFFFFU << NANDC_MTRANS_STAT_BUS_ERR_SHIFT)                 /* 0x0000FFFF */
#define NANDC_MTRANS_STAT_MTRANS_CNT_SHIFT                 (16U)
#define NANDC_MTRANS_STAT_MTRANS_CNT_MASK                  (0x3FU << NANDC_MTRANS_STAT_MTRANS_CNT_SHIFT)                /* 0x003F0000 */
/* BCHST0 */
#define NANDC_BCHST0_ERRF0_SHIFT                           (0U)
#define NANDC_BCHST0_ERRF0_MASK                            (0x1U << NANDC_BCHST0_ERRF0_SHIFT)                           /* 0x00000001 */
#define NANDC_BCHST0_DONE0_SHIFT                           (1U)
#define NANDC_BCHST0_DONE0_MASK                            (0x1U << NANDC_BCHST0_DONE0_SHIFT)                           /* 0x00000002 */
#define NANDC_BCHST0_FAIL0_SHIFT                           (2U)
#define NANDC_BCHST0_FAIL0_MASK                            (0x1U << NANDC_BCHST0_FAIL0_SHIFT)                           /* 0x00000004 */
#define NANDC_BCHST0_ERR_TNUM0_L5_SHIFT                    (3U)
#define NANDC_BCHST0_ERR_TNUM0_L5_MASK                     (0x1FU << NANDC_BCHST0_ERR_TNUM0_L5_SHIFT)                   /* 0x000000F8 */
#define NANDC_BCHST0_ERR_HNUM0_L5_SHIFT                    (8U)
#define NANDC_BCHST0_ERR_HNUM0_L5_MASK                     (0x1FU << NANDC_BCHST0_ERR_HNUM0_L5_SHIFT)                   /* 0x00001F00 */
#define NANDC_BCHST0_ERRF1_SHIFT                           (13U)
#define NANDC_BCHST0_ERRF1_MASK                            (0x1U << NANDC_BCHST0_ERRF1_SHIFT)                           /* 0x00002000 */
#define NANDC_BCHST0_DONE1_SHIFT                           (14U)
#define NANDC_BCHST0_DONE1_MASK                            (0x1U << NANDC_BCHST0_DONE1_SHIFT)                           /* 0x00004000 */
#define NANDC_BCHST0_FAIL1_SHIFT                           (15U)
#define NANDC_BCHST0_FAIL1_MASK                            (0x1U << NANDC_BCHST0_FAIL1_SHIFT)                           /* 0x00008000 */
#define NANDC_BCHST0_ERR_TNUM1_L5_SHIFT                    (16U)
#define NANDC_BCHST0_ERR_TNUM1_L5_MASK                     (0x1FU << NANDC_BCHST0_ERR_TNUM1_L5_SHIFT)                   /* 0x001F0000 */
#define NANDC_BCHST0_ERR_HNUM1_L5_SHIFT                    (21U)
#define NANDC_BCHST0_ERR_HNUM1_L5_MASK                     (0x1FU << NANDC_BCHST0_ERR_HNUM1_L5_SHIFT)                   /* 0x03E00000 */
#define NANDC_BCHST0_BCHRDY_SHIFT                          (26U)
#define NANDC_BCHST0_BCHRDY_MASK                           (0x1U << NANDC_BCHST0_BCHRDY_SHIFT)                          /* 0x04000000 */
#define NANDC_BCHST0_ERR_TNUM0_H1_SHIFT                    (27U)
#define NANDC_BCHST0_ERR_TNUM0_H1_MASK                     (0x1U << NANDC_BCHST0_ERR_TNUM0_H1_SHIFT)                    /* 0x08000000 */
#define NANDC_BCHST0_ERR_HNUM0_H1_SHIFT                    (28U)
#define NANDC_BCHST0_ERR_HNUM0_H1_MASK                     (0x1U << NANDC_BCHST0_ERR_HNUM0_H1_SHIFT)                    /* 0x10000000 */
#define NANDC_BCHST0_ERR_TNUM1_H1_SHIFT                    (29U)
#define NANDC_BCHST0_ERR_TNUM1_H1_MASK                     (0x1U << NANDC_BCHST0_ERR_TNUM1_H1_SHIFT)                    /* 0x20000000 */
#define NANDC_BCHST0_ERR_HNUM1_H1_SHIFT                    (30U)
#define NANDC_BCHST0_ERR_HNUM1_H1_MASK                     (0x1U << NANDC_BCHST0_ERR_HNUM1_H1_SHIFT)                    /* 0x40000000 */
/* BCHST1 */
#define NANDC_BCHST1_ERRF2_SHIFT                           (0U)
#define NANDC_BCHST1_ERRF2_MASK                            (0x1U << NANDC_BCHST1_ERRF2_SHIFT)                           /* 0x00000001 */
#define NANDC_BCHST1_DONE2_SHIFT                           (1U)
#define NANDC_BCHST1_DONE2_MASK                            (0x1U << NANDC_BCHST1_DONE2_SHIFT)                           /* 0x00000002 */
#define NANDC_BCHST1_FAIL2_SHIFT                           (2U)
#define NANDC_BCHST1_FAIL2_MASK                            (0x1U << NANDC_BCHST1_FAIL2_SHIFT)                           /* 0x00000004 */
#define NANDC_BCHST1_ERR_TNUM2_L5_SHIFT                    (3U)
#define NANDC_BCHST1_ERR_TNUM2_L5_MASK                     (0x1FU << NANDC_BCHST1_ERR_TNUM2_L5_SHIFT)                   /* 0x000000F8 */
#define NANDC_BCHST1_ERR_HNUM2_L5_SHIFT                    (8U)
#define NANDC_BCHST1_ERR_HNUM2_L5_MASK                     (0x1FU << NANDC_BCHST1_ERR_HNUM2_L5_SHIFT)                   /* 0x00001F00 */
#define NANDC_BCHST1_ERRF3_SHIFT                           (13U)
#define NANDC_BCHST1_ERRF3_MASK                            (0x1U << NANDC_BCHST1_ERRF3_SHIFT)                           /* 0x00002000 */
#define NANDC_BCHST1_DONE3_SHIFT                           (14U)
#define NANDC_BCHST1_DONE3_MASK                            (0x1U << NANDC_BCHST1_DONE3_SHIFT)                           /* 0x00004000 */
#define NANDC_BCHST1_FAIL3_SHIFT                           (15U)
#define NANDC_BCHST1_FAIL3_MASK                            (0x1U << NANDC_BCHST1_FAIL3_SHIFT)                           /* 0x00008000 */
#define NANDC_BCHST1_ERR_TNUM3_L5_SHIFT                    (16U)
#define NANDC_BCHST1_ERR_TNUM3_L5_MASK                     (0x1FU << NANDC_BCHST1_ERR_TNUM3_L5_SHIFT)                   /* 0x001F0000 */
#define NANDC_BCHST1_ERR_HNUM3_L5_SHIFT                    (21U)
#define NANDC_BCHST1_ERR_HNUM3_L5_MASK                     (0x1FU << NANDC_BCHST1_ERR_HNUM3_L5_SHIFT)                   /* 0x03E00000 */
#define NANDC_BCHST1_ERR_TNUM2_H1_SHIFT                    (27U)
#define NANDC_BCHST1_ERR_TNUM2_H1_MASK                     (0x1U << NANDC_BCHST1_ERR_TNUM2_H1_SHIFT)                    /* 0x08000000 */
#define NANDC_BCHST1_ERR_HNUM2_H1_SHIFT                    (28U)
#define NANDC_BCHST1_ERR_HNUM2_H1_MASK                     (0x1U << NANDC_BCHST1_ERR_HNUM2_H1_SHIFT)                    /* 0x10000000 */
#define NANDC_BCHST1_ERR_TNUM3_H1_SHIFT                    (29U)
#define NANDC_BCHST1_ERR_TNUM3_H1_MASK                     (0x1U << NANDC_BCHST1_ERR_TNUM3_H1_SHIFT)                    /* 0x20000000 */
#define NANDC_BCHST1_ERR_HNUM3_H1_SHIFT                    (30U)
#define NANDC_BCHST1_ERR_HNUM3_H1_MASK                     (0x1U << NANDC_BCHST1_ERR_HNUM3_H1_SHIFT)                    /* 0x40000000 */
/* BCHST2 */
#define NANDC_BCHST2_BCHST_CWD4_CWD5_SHIFT                 (0U)
#define NANDC_BCHST2_BCHST_CWD4_CWD5_MASK                  (0xFFFFFFFFU << NANDC_BCHST2_BCHST_CWD4_CWD5_SHIFT)          /* 0xFFFFFFFF */
/* BCHST3 */
#define NANDC_BCHST3_BCHST_CWD6_CWD7_SHIFT                 (0U)
#define NANDC_BCHST3_BCHST_CWD6_CWD7_MASK                  (0xFFFFFFFFU << NANDC_BCHST3_BCHST_CWD6_CWD7_SHIFT)          /* 0xFFFFFFFF */
/* BCHST4 */
#define NANDC_BCHST4_BCHST_CWD8_CWD9_SHIFT                 (0U)
#define NANDC_BCHST4_BCHST_CWD8_CWD9_MASK                  (0xFFFFFFFFU << NANDC_BCHST4_BCHST_CWD8_CWD9_SHIFT)          /* 0xFFFFFFFF */
/* BCHST5 */
#define NANDC_BCHST5_BCHST_CWD10_CWD11_SHIFT               (0U)
#define NANDC_BCHST5_BCHST_CWD10_CWD11_MASK                (0xFFFFFFFFU << NANDC_BCHST5_BCHST_CWD10_CWD11_SHIFT)        /* 0xFFFFFFFF */
/* BCHST6 */
#define NANDC_BCHST6_BCHST_CWD12_CWD13_SHIFT               (0U)
#define NANDC_BCHST6_BCHST_CWD12_CWD13_MASK                (0xFFFFFFFFU << NANDC_BCHST6_BCHST_CWD12_CWD13_SHIFT)        /* 0xFFFFFFFF */
/* BCHST7 */
#define NANDC_BCHST7_BCHST_CWD14_CWD15_SHIFT               (0U)
#define NANDC_BCHST7_BCHST_CWD14_CWD15_MASK                (0xFFFFFFFFU << NANDC_BCHST7_BCHST_CWD14_CWD15_SHIFT)        /* 0xFFFFFFFF */
/* BCHLOC0 */
#define NANDC_BCHLOC0_ERR_LOC0_L5_SHIFT                    (0U)
#define NANDC_BCHLOC0_ERR_LOC0_L5_MASK                     (0x1FU << NANDC_BCHLOC0_ERR_LOC0_L5_SHIFT)                   /* 0x0000001F */
#define NANDC_BCHLOC0_ERR_LOC1_L5_SHIFT                    (5U)
#define NANDC_BCHLOC0_ERR_LOC1_L5_MASK                     (0x1FU << NANDC_BCHLOC0_ERR_LOC1_L5_SHIFT)                   /* 0x000003E0 */
#define NANDC_BCHLOC0_ERR_LOC2_L5_SHIFT                    (10U)
#define NANDC_BCHLOC0_ERR_LOC2_L5_MASK                     (0x1FU << NANDC_BCHLOC0_ERR_LOC2_L5_SHIFT)                   /* 0x00007C00 */
#define NANDC_BCHLOC0_ERR_LOC3_L5_SHIFT                    (15U)
#define NANDC_BCHLOC0_ERR_LOC3_L5_MASK                     (0x1FU << NANDC_BCHLOC0_ERR_LOC3_L5_SHIFT)                   /* 0x000F8000 */
#define NANDC_BCHLOC0_ERR_LOC4_L5_SHIFT                    (20U)
#define NANDC_BCHLOC0_ERR_LOC4_L5_MASK                     (0x1FU << NANDC_BCHLOC0_ERR_LOC4_L5_SHIFT)                   /* 0x01F00000 */
#define NANDC_BCHLOC0_ERR_LOC5_L5_SHIFT                    (25U)
#define NANDC_BCHLOC0_ERR_LOC5_L5_MASK                     (0x1FU << NANDC_BCHLOC0_ERR_LOC5_L5_SHIFT)                   /* 0x3E000000 */
/* BCHLOC1 */
#define NANDC_BCHLOC1_ERR_LOC6_L5_SHIFT                    (0U)
#define NANDC_BCHLOC1_ERR_LOC6_L5_MASK                     (0x1FU << NANDC_BCHLOC1_ERR_LOC6_L5_SHIFT)                   /* 0x0000001F */
#define NANDC_BCHLOC1_ERR_LOC7_L5_SHIFT                    (5U)
#define NANDC_BCHLOC1_ERR_LOC7_L5_MASK                     (0x1FU << NANDC_BCHLOC1_ERR_LOC7_L5_SHIFT)                   /* 0x000003E0 */
#define NANDC_BCHLOC1_ERR_LOC8_L5_SHIFT                    (10U)
#define NANDC_BCHLOC1_ERR_LOC8_L5_MASK                     (0x1FU << NANDC_BCHLOC1_ERR_LOC8_L5_SHIFT)                   /* 0x00007C00 */
#define NANDC_BCHLOC1_ERR_LOC9_L5_SHIFT                    (15U)
#define NANDC_BCHLOC1_ERR_LOC9_L5_MASK                     (0x1FU << NANDC_BCHLOC1_ERR_LOC9_L5_SHIFT)                   /* 0x000F8000 */
#define NANDC_BCHLOC1_ERR_LOC10_L5_SHIFT                   (20U)
#define NANDC_BCHLOC1_ERR_LOC10_L5_MASK                    (0x1FU << NANDC_BCHLOC1_ERR_LOC10_L5_SHIFT)                  /* 0x01F00000 */
#define NANDC_BCHLOC1_ERR_LOC11_L5_SHIFT                   (25U)
#define NANDC_BCHLOC1_ERR_LOC11_L5_MASK                    (0x1FU << NANDC_BCHLOC1_ERR_LOC11_L5_SHIFT)                  /* 0x3E000000 */
/* BCHLOC2 */
#define NANDC_BCHLOC2_ERR_LOC12_L5_SHIFT                   (0U)
#define NANDC_BCHLOC2_ERR_LOC12_L5_MASK                    (0x1FU << NANDC_BCHLOC2_ERR_LOC12_L5_SHIFT)                  /* 0x0000001F */
#define NANDC_BCHLOC2_ERR_LOC13_L5_SHIFT                   (5U)
#define NANDC_BCHLOC2_ERR_LOC13_L5_MASK                    (0x1FU << NANDC_BCHLOC2_ERR_LOC13_L5_SHIFT)                  /* 0x000003E0 */
#define NANDC_BCHLOC2_ERR_LOC14_L5_SHIFT                   (10U)
#define NANDC_BCHLOC2_ERR_LOC14_L5_MASK                    (0x1FU << NANDC_BCHLOC2_ERR_LOC14_L5_SHIFT)                  /* 0x00007C00 */
#define NANDC_BCHLOC2_ERR_LOC15_L5_SHIFT                   (15U)
#define NANDC_BCHLOC2_ERR_LOC15_L5_MASK                    (0x1FU << NANDC_BCHLOC2_ERR_LOC15_L5_SHIFT)                  /* 0x000F8000 */
#define NANDC_BCHLOC2_ERR_LOC16_L5_SHIFT                   (20U)
#define NANDC_BCHLOC2_ERR_LOC16_L5_MASK                    (0x1FU << NANDC_BCHLOC2_ERR_LOC16_L5_SHIFT)                  /* 0x01F00000 */
#define NANDC_BCHLOC2_ERR_LOC17_L5_SHIFT                   (25U)
#define NANDC_BCHLOC2_ERR_LOC17_L5_MASK                    (0x1FU << NANDC_BCHLOC2_ERR_LOC17_L5_SHIFT)                  /* 0x3E000000 */
/* BCHLOC3 */
#define NANDC_BCHLOC3_ERR_LOC18_L5_SHIFT                   (0U)
#define NANDC_BCHLOC3_ERR_LOC18_L5_MASK                    (0x1FU << NANDC_BCHLOC3_ERR_LOC18_L5_SHIFT)                  /* 0x0000001F */
#define NANDC_BCHLOC3_ERR_LOC19_L5_SHIFT                   (5U)
#define NANDC_BCHLOC3_ERR_LOC19_L5_MASK                    (0x1FU << NANDC_BCHLOC3_ERR_LOC19_L5_SHIFT)                  /* 0x000003E0 */
#define NANDC_BCHLOC3_ERR_LOC20_L5_SHIFT                   (10U)
#define NANDC_BCHLOC3_ERR_LOC20_L5_MASK                    (0x1FU << NANDC_BCHLOC3_ERR_LOC20_L5_SHIFT)                  /* 0x00007C00 */
#define NANDC_BCHLOC3_ERR_LOC21_L5_SHIFT                   (15U)
#define NANDC_BCHLOC3_ERR_LOC21_L5_MASK                    (0x1FU << NANDC_BCHLOC3_ERR_LOC21_L5_SHIFT)                  /* 0x000F8000 */
#define NANDC_BCHLOC3_ERR_LOC22_L5_SHIFT                   (20U)
#define NANDC_BCHLOC3_ERR_LOC22_L5_MASK                    (0x1FU << NANDC_BCHLOC3_ERR_LOC22_L5_SHIFT)                  /* 0x01F00000 */
#define NANDC_BCHLOC3_ERR_LOC23_L5_SHIFT                   (25U)
#define NANDC_BCHLOC3_ERR_LOC23_L5_MASK                    (0x1FU << NANDC_BCHLOC3_ERR_LOC23_L5_SHIFT)                  /* 0x3E000000 */
/* BCHLOC4 */
#define NANDC_BCHLOC4_ERR_LOC24_L5_SHIFT                   (0U)
#define NANDC_BCHLOC4_ERR_LOC24_L5_MASK                    (0x1FU << NANDC_BCHLOC4_ERR_LOC24_L5_SHIFT)                  /* 0x0000001F */
#define NANDC_BCHLOC4_ERR_LOC25_L5_SHIFT                   (5U)
#define NANDC_BCHLOC4_ERR_LOC25_L5_MASK                    (0x1FU << NANDC_BCHLOC4_ERR_LOC25_L5_SHIFT)                  /* 0x000003E0 */
#define NANDC_BCHLOC4_ERR_LOC26_L5_SHIFT                   (10U)
#define NANDC_BCHLOC4_ERR_LOC26_L5_MASK                    (0x1FU << NANDC_BCHLOC4_ERR_LOC26_L5_SHIFT)                  /* 0x00007C00 */
#define NANDC_BCHLOC4_ERR_LOC27_L5_SHIFT                   (15U)
#define NANDC_BCHLOC4_ERR_LOC27_L5_MASK                    (0x1FU << NANDC_BCHLOC4_ERR_LOC27_L5_SHIFT)                  /* 0x000F8000 */
#define NANDC_BCHLOC4_ERR_LOC28_L5_SHIFT                   (20U)
#define NANDC_BCHLOC4_ERR_LOC28_L5_MASK                    (0x1FU << NANDC_BCHLOC4_ERR_LOC28_L5_SHIFT)                  /* 0x01F00000 */
#define NANDC_BCHLOC4_ERR_LOC29_L5_SHIFT                   (25U)
#define NANDC_BCHLOC4_ERR_LOC29_L5_MASK                    (0x1FU << NANDC_BCHLOC4_ERR_LOC29_L5_SHIFT)                  /* 0x3E000000 */
/* BCHLOC5 */
#define NANDC_BCHLOC5_ERR_LOC30_L5_SHIFT                   (0U)
#define NANDC_BCHLOC5_ERR_LOC30_L5_MASK                    (0x1FU << NANDC_BCHLOC5_ERR_LOC30_L5_SHIFT)                  /* 0x0000001F */
#define NANDC_BCHLOC5_ERR_LOC31_L5_SHIFT                   (5U)
#define NANDC_BCHLOC5_ERR_LOC31_L5_MASK                    (0x1FU << NANDC_BCHLOC5_ERR_LOC31_L5_SHIFT)                  /* 0x000003E0 */
/* BCHLOC6 */
#define NANDC_BCHLOC6_ERR_LOC0_H1_SHIFT                    (0U)
#define NANDC_BCHLOC6_ERR_LOC0_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC0_H1_SHIFT)                    /* 0x00000001 */
#define NANDC_BCHLOC6_ERR_LOC1_H1_SHIFT                    (1U)
#define NANDC_BCHLOC6_ERR_LOC1_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC1_H1_SHIFT)                    /* 0x00000002 */
#define NANDC_BCHLOC6_ERR_LOC2_H1_SHIFT                    (2U)
#define NANDC_BCHLOC6_ERR_LOC2_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC2_H1_SHIFT)                    /* 0x00000004 */
#define NANDC_BCHLOC6_ERR_LOC3_H1_SHIFT                    (3U)
#define NANDC_BCHLOC6_ERR_LOC3_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC3_H1_SHIFT)                    /* 0x00000008 */
#define NANDC_BCHLOC6_ERR_LOC4_H1_SHIFT                    (4U)
#define NANDC_BCHLOC6_ERR_LOC4_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC4_H1_SHIFT)                    /* 0x00000010 */
#define NANDC_BCHLOC6_ERR_LOC5_H1_SHIFT                    (5U)
#define NANDC_BCHLOC6_ERR_LOC5_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC5_H1_SHIFT)                    /* 0x00000020 */
#define NANDC_BCHLOC6_ERR_LOC6_H1_SHIFT                    (6U)
#define NANDC_BCHLOC6_ERR_LOC6_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC6_H1_SHIFT)                    /* 0x00000040 */
#define NANDC_BCHLOC6_ERR_LOC7_H1_SHIFT                    (7U)
#define NANDC_BCHLOC6_ERR_LOC7_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC7_H1_SHIFT)                    /* 0x00000080 */
#define NANDC_BCHLOC6_ERR_LOC8_H1_SHIFT                    (8U)
#define NANDC_BCHLOC6_ERR_LOC8_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC8_H1_SHIFT)                    /* 0x00000100 */
#define NANDC_BCHLOC6_ERR_LOC9_H1_SHIFT                    (9U)
#define NANDC_BCHLOC6_ERR_LOC9_H1_MASK                     (0x1U << NANDC_BCHLOC6_ERR_LOC9_H1_SHIFT)                    /* 0x00000200 */
#define NANDC_BCHLOC6_ERR_LOC10_H1_SHIFT                   (10U)
#define NANDC_BCHLOC6_ERR_LOC10_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC10_H1_SHIFT)                   /* 0x00000400 */
#define NANDC_BCHLOC6_ERR_LOC11_H1_SHIFT                   (11U)
#define NANDC_BCHLOC6_ERR_LOC11_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC11_H1_SHIFT)                   /* 0x00000800 */
#define NANDC_BCHLOC6_ERR_LOC12_H1_SHIFT                   (12U)
#define NANDC_BCHLOC6_ERR_LOC12_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC12_H1_SHIFT)                   /* 0x00001000 */
#define NANDC_BCHLOC6_ERR_LOC13_H1_SHIFT                   (13U)
#define NANDC_BCHLOC6_ERR_LOC13_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC13_H1_SHIFT)                   /* 0x00002000 */
#define NANDC_BCHLOC6_ERR_LOC14_H1_SHIFT                   (14U)
#define NANDC_BCHLOC6_ERR_LOC14_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC14_H1_SHIFT)                   /* 0x00004000 */
#define NANDC_BCHLOC6_ERR_LOC15_H1_SHIFT                   (15U)
#define NANDC_BCHLOC6_ERR_LOC15_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC15_H1_SHIFT)                   /* 0x00008000 */
#define NANDC_BCHLOC6_ERR_LOC16_H1_SHIFT                   (16U)
#define NANDC_BCHLOC6_ERR_LOC16_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC16_H1_SHIFT)                   /* 0x00010000 */
#define NANDC_BCHLOC6_ERR_LOC17_H1_SHIFT                   (17U)
#define NANDC_BCHLOC6_ERR_LOC17_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC17_H1_SHIFT)                   /* 0x00020000 */
#define NANDC_BCHLOC6_ERR_LOC18_H1_SHIFT                   (18U)
#define NANDC_BCHLOC6_ERR_LOC18_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC18_H1_SHIFT)                   /* 0x00040000 */
#define NANDC_BCHLOC6_ERR_LOC19_H1_SHIFT                   (19U)
#define NANDC_BCHLOC6_ERR_LOC19_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC19_H1_SHIFT)                   /* 0x00080000 */
#define NANDC_BCHLOC6_ERR_LOC20_H1_SHIFT                   (20U)
#define NANDC_BCHLOC6_ERR_LOC20_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC20_H1_SHIFT)                   /* 0x00100000 */
#define NANDC_BCHLOC6_ERR_LOC21_H1_SHIFT                   (21U)
#define NANDC_BCHLOC6_ERR_LOC21_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC21_H1_SHIFT)                   /* 0x00200000 */
#define NANDC_BCHLOC6_ERR_LOC22_H1_SHIFT                   (22U)
#define NANDC_BCHLOC6_ERR_LOC22_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC22_H1_SHIFT)                   /* 0x00400000 */
#define NANDC_BCHLOC6_ERR_LOC23_H1_SHIFT                   (23U)
#define NANDC_BCHLOC6_ERR_LOC23_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC23_H1_SHIFT)                   /* 0x00800000 */
#define NANDC_BCHLOC6_ERR_LOC24_H1_SHIFT                   (24U)
#define NANDC_BCHLOC6_ERR_LOC24_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC24_H1_SHIFT)                   /* 0x01000000 */
#define NANDC_BCHLOC6_ERR_LOC25_H1_SHIFT                   (25U)
#define NANDC_BCHLOC6_ERR_LOC25_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC25_H1_SHIFT)                   /* 0x02000000 */
#define NANDC_BCHLOC6_ERR_LOC26_H1_SHIFT                   (26U)
#define NANDC_BCHLOC6_ERR_LOC26_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC26_H1_SHIFT)                   /* 0x04000000 */
#define NANDC_BCHLOC6_ERR_LOC27_H1_SHIFT                   (27U)
#define NANDC_BCHLOC6_ERR_LOC27_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC27_H1_SHIFT)                   /* 0x08000000 */
#define NANDC_BCHLOC6_ERR_LOC28_H1_SHIFT                   (28U)
#define NANDC_BCHLOC6_ERR_LOC28_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC28_H1_SHIFT)                   /* 0x10000000 */
#define NANDC_BCHLOC6_ERR_LOC29_H1_SHIFT                   (29U)
#define NANDC_BCHLOC6_ERR_LOC29_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC29_H1_SHIFT)                   /* 0x20000000 */
#define NANDC_BCHLOC6_ERR_LOC30_H1_SHIFT                   (30U)
#define NANDC_BCHLOC6_ERR_LOC30_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC30_H1_SHIFT)                   /* 0x40000000 */
#define NANDC_BCHLOC6_ERR_LOC31_H1_SHIFT                   (31U)
#define NANDC_BCHLOC6_ERR_LOC31_H1_MASK                    (0x1U << NANDC_BCHLOC6_ERR_LOC31_H1_SHIFT)                   /* 0x80000000 */
/* BCHDE0_0 */
#define NANDC_BCHDE0_0_ERR_VAL_SHIFT                       (0U)
#define NANDC_BCHDE0_0_ERR_VAL_MASK                        (0xFFU << NANDC_BCHDE0_0_ERR_VAL_SHIFT)                      /* 0x000000FF */
#define NANDC_BCHDE0_0_OFFSET_SHIFT                        (8U)
#define NANDC_BCHDE0_0_OFFSET_MASK                         (0x7FFU << NANDC_BCHDE0_0_OFFSET_SHIFT)                      /* 0x0007FF00 */
/* BCHDE0_1 */
#define NANDC_BCHDE0_1_BCHDE0_1_SHIFT                      (0U)
#define NANDC_BCHDE0_1_BCHDE0_1_MASK                       (0x7FFFFU << NANDC_BCHDE0_1_BCHDE0_1_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_2 */
#define NANDC_BCHDE0_2_BCHDE0_2_SHIFT                      (0U)
#define NANDC_BCHDE0_2_BCHDE0_2_MASK                       (0x7FFFFU << NANDC_BCHDE0_2_BCHDE0_2_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_3 */
#define NANDC_BCHDE0_3_BCHDE0_3_SHIFT                      (0U)
#define NANDC_BCHDE0_3_BCHDE0_3_MASK                       (0x7FFFFU << NANDC_BCHDE0_3_BCHDE0_3_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_4 */
#define NANDC_BCHDE0_4_BCHDE0_4_SHIFT                      (0U)
#define NANDC_BCHDE0_4_BCHDE0_4_MASK                       (0x7FFFFU << NANDC_BCHDE0_4_BCHDE0_4_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_5 */
#define NANDC_BCHDE0_5_BCHDE0_5_SHIFT                      (0U)
#define NANDC_BCHDE0_5_BCHDE0_5_MASK                       (0x7FFFFU << NANDC_BCHDE0_5_BCHDE0_5_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_6 */
#define NANDC_BCHDE0_6_BCHDE0_6_SHIFT                      (0U)
#define NANDC_BCHDE0_6_BCHDE0_6_MASK                       (0x7FFFFU << NANDC_BCHDE0_6_BCHDE0_6_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_7 */
#define NANDC_BCHDE0_7_BCHDE0_7_SHIFT                      (0U)
#define NANDC_BCHDE0_7_BCHDE0_7_MASK                       (0x7FFFFU << NANDC_BCHDE0_7_BCHDE0_7_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_8 */
#define NANDC_BCHDE0_8_BCHDE0_8_SHIFT                      (0U)
#define NANDC_BCHDE0_8_BCHDE0_8_MASK                       (0x7FFFFU << NANDC_BCHDE0_8_BCHDE0_8_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_9 */
#define NANDC_BCHDE0_9_BCHDE0_9_SHIFT                      (0U)
#define NANDC_BCHDE0_9_BCHDE0_9_MASK                       (0x7FFFFU << NANDC_BCHDE0_9_BCHDE0_9_SHIFT)                  /* 0x0007FFFF */
/* BCHDE0_10 */
#define NANDC_BCHDE0_10_BCHDE0_10_SHIFT                    (0U)
#define NANDC_BCHDE0_10_BCHDE0_10_MASK                     (0x7FFFFU << NANDC_BCHDE0_10_BCHDE0_10_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_11 */
#define NANDC_BCHDE0_11_BCHDE0_11_SHIFT                    (0U)
#define NANDC_BCHDE0_11_BCHDE0_11_MASK                     (0x7FFFFU << NANDC_BCHDE0_11_BCHDE0_11_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_12 */
#define NANDC_BCHDE0_12_BCHDE0_12_SHIFT                    (0U)
#define NANDC_BCHDE0_12_BCHDE0_12_MASK                     (0x7FFFFU << NANDC_BCHDE0_12_BCHDE0_12_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_13 */
#define NANDC_BCHDE0_13_BCHDE0_13_SHIFT                    (0U)
#define NANDC_BCHDE0_13_BCHDE0_13_MASK                     (0x7FFFFU << NANDC_BCHDE0_13_BCHDE0_13_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_14 */
#define NANDC_BCHDE0_14_BCHDE0_14_SHIFT                    (0U)
#define NANDC_BCHDE0_14_BCHDE0_14_MASK                     (0x7FFFFU << NANDC_BCHDE0_14_BCHDE0_14_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_15 */
#define NANDC_BCHDE0_15_BCHDE0_15_SHIFT                    (0U)
#define NANDC_BCHDE0_15_BCHDE0_15_MASK                     (0x7FFFFU << NANDC_BCHDE0_15_BCHDE0_15_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_16 */
#define NANDC_BCHDE0_16_BCHDE0_16_SHIFT                    (0U)
#define NANDC_BCHDE0_16_BCHDE0_16_MASK                     (0x7FFFFU << NANDC_BCHDE0_16_BCHDE0_16_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_17 */
#define NANDC_BCHDE0_17_BCHDE0_17_SHIFT                    (0U)
#define NANDC_BCHDE0_17_BCHDE0_17_MASK                     (0x7FFFFU << NANDC_BCHDE0_17_BCHDE0_17_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_18 */
#define NANDC_BCHDE0_18_BCHDE0_18_SHIFT                    (0U)
#define NANDC_BCHDE0_18_BCHDE0_18_MASK                     (0x7FFFFU << NANDC_BCHDE0_18_BCHDE0_18_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_19 */
#define NANDC_BCHDE0_19_BCHDE0_19_SHIFT                    (0U)
#define NANDC_BCHDE0_19_BCHDE0_19_MASK                     (0x7FFFFU << NANDC_BCHDE0_19_BCHDE0_19_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_20 */
#define NANDC_BCHDE0_20_BCHDE0_20_SHIFT                    (0U)
#define NANDC_BCHDE0_20_BCHDE0_20_MASK                     (0x7FFFFU << NANDC_BCHDE0_20_BCHDE0_20_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_21 */
#define NANDC_BCHDE0_21_BCHDE0_21_SHIFT                    (0U)
#define NANDC_BCHDE0_21_BCHDE0_21_MASK                     (0x7FFFFU << NANDC_BCHDE0_21_BCHDE0_21_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_22 */
#define NANDC_BCHDE0_22_BCHDE0_22_SHIFT                    (0U)
#define NANDC_BCHDE0_22_BCHDE0_22_MASK                     (0x7FFFFU << NANDC_BCHDE0_22_BCHDE0_22_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_23 */
#define NANDC_BCHDE0_23_BCHDE0_23_SHIFT                    (0U)
#define NANDC_BCHDE0_23_BCHDE0_23_MASK                     (0x7FFFFU << NANDC_BCHDE0_23_BCHDE0_23_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_0 */
#define NANDC_BCHDE1_0_ERR_VAL_SHIFT                       (0U)
#define NANDC_BCHDE1_0_ERR_VAL_MASK                        (0xFFU << NANDC_BCHDE1_0_ERR_VAL_SHIFT)                      /* 0x000000FF */
#define NANDC_BCHDE1_0_OFFSET_SHIFT                        (8U)
#define NANDC_BCHDE1_0_OFFSET_MASK                         (0x7FFU << NANDC_BCHDE1_0_OFFSET_SHIFT)                      /* 0x0007FF00 */
/* BCHDE1_1 */
#define NANDC_BCHDE1_1_BCHDE1_1_SHIFT                      (0U)
#define NANDC_BCHDE1_1_BCHDE1_1_MASK                       (0x7FFFFU << NANDC_BCHDE1_1_BCHDE1_1_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_2 */
#define NANDC_BCHDE1_2_BCHDE1_2_SHIFT                      (0U)
#define NANDC_BCHDE1_2_BCHDE1_2_MASK                       (0x7FFFFU << NANDC_BCHDE1_2_BCHDE1_2_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_3 */
#define NANDC_BCHDE1_3_BCHDE1_3_SHIFT                      (0U)
#define NANDC_BCHDE1_3_BCHDE1_3_MASK                       (0x7FFFFU << NANDC_BCHDE1_3_BCHDE1_3_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_4 */
#define NANDC_BCHDE1_4_BCHDE1_4_SHIFT                      (0U)
#define NANDC_BCHDE1_4_BCHDE1_4_MASK                       (0x7FFFFU << NANDC_BCHDE1_4_BCHDE1_4_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_5 */
#define NANDC_BCHDE1_5_BCHDE1_5_SHIFT                      (0U)
#define NANDC_BCHDE1_5_BCHDE1_5_MASK                       (0x7FFFFU << NANDC_BCHDE1_5_BCHDE1_5_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_6 */
#define NANDC_BCHDE1_6_BCHDE1_6_SHIFT                      (0U)
#define NANDC_BCHDE1_6_BCHDE1_6_MASK                       (0x7FFFFU << NANDC_BCHDE1_6_BCHDE1_6_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_7 */
#define NANDC_BCHDE1_7_BCHDE1_7_SHIFT                      (0U)
#define NANDC_BCHDE1_7_BCHDE1_7_MASK                       (0x7FFFFU << NANDC_BCHDE1_7_BCHDE1_7_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_8 */
#define NANDC_BCHDE1_8_BCHDE1_8_SHIFT                      (0U)
#define NANDC_BCHDE1_8_BCHDE1_8_MASK                       (0x7FFFFU << NANDC_BCHDE1_8_BCHDE1_8_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_9 */
#define NANDC_BCHDE1_9_BCHDE1_9_SHIFT                      (0U)
#define NANDC_BCHDE1_9_BCHDE1_9_MASK                       (0x7FFFFU << NANDC_BCHDE1_9_BCHDE1_9_SHIFT)                  /* 0x0007FFFF */
/* BCHDE1_10 */
#define NANDC_BCHDE1_10_BCHDE1_10_SHIFT                    (0U)
#define NANDC_BCHDE1_10_BCHDE1_10_MASK                     (0x7FFFFU << NANDC_BCHDE1_10_BCHDE1_10_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_11 */
#define NANDC_BCHDE1_11_BCHDE1_11_SHIFT                    (0U)
#define NANDC_BCHDE1_11_BCHDE1_11_MASK                     (0x7FFFFU << NANDC_BCHDE1_11_BCHDE1_11_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_12 */
#define NANDC_BCHDE1_12_BCHDE1_12_SHIFT                    (0U)
#define NANDC_BCHDE1_12_BCHDE1_12_MASK                     (0x7FFFFU << NANDC_BCHDE1_12_BCHDE1_12_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_13 */
#define NANDC_BCHDE1_13_BCHDE1_13_SHIFT                    (0U)
#define NANDC_BCHDE1_13_BCHDE1_13_MASK                     (0x7FFFFU << NANDC_BCHDE1_13_BCHDE1_13_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_14 */
#define NANDC_BCHDE1_14_BCHDE1_14_SHIFT                    (0U)
#define NANDC_BCHDE1_14_BCHDE1_14_MASK                     (0x7FFFFU << NANDC_BCHDE1_14_BCHDE1_14_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_15 */
#define NANDC_BCHDE1_15_BCHDE1_15_SHIFT                    (0U)
#define NANDC_BCHDE1_15_BCHDE1_15_MASK                     (0x7FFFFU << NANDC_BCHDE1_15_BCHDE1_15_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_16 */
#define NANDC_BCHDE1_16_BCHDE1_16_SHIFT                    (0U)
#define NANDC_BCHDE1_16_BCHDE1_16_MASK                     (0x7FFFFU << NANDC_BCHDE1_16_BCHDE1_16_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_17 */
#define NANDC_BCHDE1_17_BCHDE1_17_SHIFT                    (0U)
#define NANDC_BCHDE1_17_BCHDE1_17_MASK                     (0x7FFFFU << NANDC_BCHDE1_17_BCHDE1_17_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_18 */
#define NANDC_BCHDE1_18_BCHDE1_18_SHIFT                    (0U)
#define NANDC_BCHDE1_18_BCHDE1_18_MASK                     (0x7FFFFU << NANDC_BCHDE1_18_BCHDE1_18_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_19 */
#define NANDC_BCHDE1_19_BCHDE1_19_SHIFT                    (0U)
#define NANDC_BCHDE1_19_BCHDE1_19_MASK                     (0x7FFFFU << NANDC_BCHDE1_19_BCHDE1_19_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_20 */
#define NANDC_BCHDE1_20_BCHDE1_20_SHIFT                    (0U)
#define NANDC_BCHDE1_20_BCHDE1_20_MASK                     (0x7FFFFU << NANDC_BCHDE1_20_BCHDE1_20_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_21 */
#define NANDC_BCHDE1_21_BCHDE1_21_SHIFT                    (0U)
#define NANDC_BCHDE1_21_BCHDE1_21_MASK                     (0x7FFFFU << NANDC_BCHDE1_21_BCHDE1_21_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_22 */
#define NANDC_BCHDE1_22_BCHDE1_22_SHIFT                    (0U)
#define NANDC_BCHDE1_22_BCHDE1_22_MASK                     (0x7FFFFU << NANDC_BCHDE1_22_BCHDE1_22_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_23 */
#define NANDC_BCHDE1_23_BCHDE1_23_SHIFT                    (0U)
#define NANDC_BCHDE1_23_BCHDE1_23_MASK                     (0x7FFFFU << NANDC_BCHDE1_23_BCHDE1_23_SHIFT)                /* 0x0007FFFF */
/* DLL_CTL_REG0 */
#define NANDC_DLL_CTL_REG0_DLL_START_POINT_SHIFT           (0U)
#define NANDC_DLL_CTL_REG0_DLL_START_POINT_MASK            (0xFFU << NANDC_DLL_CTL_REG0_DLL_START_POINT_SHIFT)          /* 0x000000FF */
#define NANDC_DLL_CTL_REG0_DLL_DQS_DLY_SHIFT               (8U)
#define NANDC_DLL_CTL_REG0_DLL_DQS_DLY_MASK                (0xFFU << NANDC_DLL_CTL_REG0_DLL_DQS_DLY_SHIFT)              /* 0x0000FF00 */
#define NANDC_DLL_CTL_REG0_DLL_DQS_DLY_BYPASS_SHIFT        (16U)
#define NANDC_DLL_CTL_REG0_DLL_DQS_DLY_BYPASS_MASK         (0xFFU << NANDC_DLL_CTL_REG0_DLL_DQS_DLY_BYPASS_SHIFT)       /* 0x00FF0000 */
/* DLL_CTL_REG1 */
#define NANDC_DLL_CTL_REG1_DLL_START_SHIFT                 (0U)
#define NANDC_DLL_CTL_REG1_DLL_START_MASK                  (0x1U << NANDC_DLL_CTL_REG1_DLL_START_SHIFT)                 /* 0x00000001 */
#define NANDC_DLL_CTL_REG1_DLL_BYPASS_SHIFT                (1U)
#define NANDC_DLL_CTL_REG1_DLL_BYPASS_MASK                 (0x1U << NANDC_DLL_CTL_REG1_DLL_BYPASS_SHIFT)                /* 0x00000002 */
#define NANDC_DLL_CTL_REG1_DLL_QTREN_SHIFT                 (2U)
#define NANDC_DLL_CTL_REG1_DLL_QTREN_MASK                  (0x3U << NANDC_DLL_CTL_REG1_DLL_QTREN_SHIFT)                 /* 0x0000000C */
#define NANDC_DLL_CTL_REG1_DLL_INCR_SHIFT                  (4U)
#define NANDC_DLL_CTL_REG1_DLL_INCR_MASK                   (0xFFU << NANDC_DLL_CTL_REG1_DLL_INCR_SHIFT)                 /* 0x00000FF0 */
/* DLL_OBS_REG0 */
#define NANDC_DLL_OBS_REG0_DLL_LOCK_SHIFT                  (0U)
#define NANDC_DLL_OBS_REG0_DLL_LOCK_MASK                   (0x1U << NANDC_DLL_OBS_REG0_DLL_LOCK_SHIFT)                  /* 0x00000001 */
#define NANDC_DLL_OBS_REG0_DLL_LOCK_VALUE_SHIFT            (1U)
#define NANDC_DLL_OBS_REG0_DLL_LOCK_VALUE_MASK             (0xFFU << NANDC_DLL_OBS_REG0_DLL_LOCK_VALUE_SHIFT)           /* 0x000001FE */
#define NANDC_DLL_OBS_REG0_DLL_DQS_DELAY_VALUE_SHIFT       (9U)
#define NANDC_DLL_OBS_REG0_DLL_DQS_DELAY_VALUE_MASK        (0xFFU << NANDC_DLL_OBS_REG0_DLL_DQS_DELAY_VALUE_SHIFT)      /* 0x0001FE00 */
/* RANDMZ_CFG */
#define NANDC_RANDMZ_CFG_RANDMZ_SEED_SHIFT                 (0U)
#define NANDC_RANDMZ_CFG_RANDMZ_SEED_MASK                  (0xFFFFFU << NANDC_RANDMZ_CFG_RANDMZ_SEED_SHIFT)             /* 0x000FFFFF */
#define NANDC_RANDMZ_CFG_CWD_OFFSET_SHIFT                  (20U)
#define NANDC_RANDMZ_CFG_CWD_OFFSET_MASK                   (0xFU << NANDC_RANDMZ_CFG_CWD_OFFSET_SHIFT)                  /* 0x00F00000 */
#define NANDC_RANDMZ_CFG_PAGE_OFFSET_SHIFT                 (24U)
#define NANDC_RANDMZ_CFG_PAGE_OFFSET_MASK                  (0x1FU << NANDC_RANDMZ_CFG_PAGE_OFFSET_SHIFT)                /* 0x1F000000 */
#define NANDC_RANDMZ_CFG_RANDMZ_MODE_SHIFT                 (29U)
#define NANDC_RANDMZ_CFG_RANDMZ_MODE_MASK                  (0x3U << NANDC_RANDMZ_CFG_RANDMZ_MODE_SHIFT)                 /* 0x60000000 */
#define NANDC_RANDMZ_CFG_RANDMZ_EN_SHIFT                   (31U)
#define NANDC_RANDMZ_CFG_RANDMZ_EN_MASK                    (0x1U << NANDC_RANDMZ_CFG_RANDMZ_EN_SHIFT)                   /* 0x80000000 */
/* FMWAIT_SYN */
#define NANDC_FMWAIT_SYN_FCLK_SHIFT                        (0U)
#define NANDC_FMWAIT_SYN_FCLK_MASK                         (0x7U << NANDC_FMWAIT_SYN_FCLK_SHIFT)                        /* 0x00000007 */
#define NANDC_FMWAIT_SYN_PRE_SHIFT                         (3U)
#define NANDC_FMWAIT_SYN_PRE_MASK                          (0x3FU << NANDC_FMWAIT_SYN_PRE_SHIFT)                        /* 0x000001F8 */
#define NANDC_FMWAIT_SYN_PST_SHIFT                         (9U)
#define NANDC_FMWAIT_SYN_PST_MASK                          (0x3FU << NANDC_FMWAIT_SYN_PST_SHIFT)                        /* 0x00007E00 */
#define NANDC_FMWAIT_SYN_SSYN_XLE_SEL_SHIFT                (15U)
#define NANDC_FMWAIT_SYN_SSYN_XLE_SEL_MASK                 (0x1U << NANDC_FMWAIT_SYN_SSYN_XLE_SEL_SHIFT)                /* 0x00008000 */
/* MTRANS_STAT2 */
#define NANDC_MTRANS_STAT2_BUS_ERR2_SHIFT                  (0U)
#define NANDC_MTRANS_STAT2_BUS_ERR2_MASK                   (0xFFFFU << NANDC_MTRANS_STAT2_BUS_ERR2_SHIFT)               /* 0x0000FFFF */
/* NANDC_VER */
#define NANDC_NANDC_VER_VERSION_SHIFT                      (0U)
#define NANDC_NANDC_VER_VERSION_MASK                       (0xFFFFFFFFU << NANDC_NANDC_VER_VERSION_SHIFT)               /* 0xFFFFFFFF */
/* LLP_CTL */
#define NANDC_LLP_CTL_LLP_EN_SHIFT                         (0U)
#define NANDC_LLP_CTL_LLP_EN_MASK                          (0x1U << NANDC_LLP_CTL_LLP_EN_SHIFT)                         /* 0x00000001 */
#define NANDC_LLP_CTL_LLP_MODE_SHIFT                       (1U)
#define NANDC_LLP_CTL_LLP_MODE_MASK                        (0x1U << NANDC_LLP_CTL_LLP_MODE_SHIFT)                       /* 0x00000002 */
#define NANDC_LLP_CTL_LLP_RST_SHIFT                        (2U)
#define NANDC_LLP_CTL_LLP_RST_MASK                         (0x1U << NANDC_LLP_CTL_LLP_RST_SHIFT)                        /* 0x00000004 */
#define NANDC_LLP_CTL_LLP_FRDY_SHIFT                       (5U)
#define NANDC_LLP_CTL_LLP_FRDY_MASK                        (0x1U << NANDC_LLP_CTL_LLP_FRDY_SHIFT)                       /* 0x00000020 */
#define NANDC_LLP_CTL_LLP_LOC_SHIFT                        (6U)
#define NANDC_LLP_CTL_LLP_LOC_MASK                         (0x3FFFFFFU << NANDC_LLP_CTL_LLP_LOC_SHIFT)                  /* 0xFFFFFFC0 */
/* LLP_STAT */
#define NANDC_LLP_STAT_LLP_RDY_SHIFT                       (0U)
#define NANDC_LLP_STAT_LLP_RDY_MASK                        (0x1U << NANDC_LLP_STAT_LLP_RDY_SHIFT)                       /* 0x00000001 */
#define NANDC_LLP_STAT_LLP_ERR_SHIFT                       (1U)
#define NANDC_LLP_STAT_LLP_ERR_MASK                        (0x1U << NANDC_LLP_STAT_LLP_ERR_SHIFT)                       /* 0x00000002 */
#define NANDC_LLP_STAT_LLP_STAT_SHIFT                      (6U)
#define NANDC_LLP_STAT_LLP_STAT_MASK                       (0x3FFFFFFU << NANDC_LLP_STAT_LLP_STAT_SHIFT)                /* 0xFFFFFFC0 */
/* INTEN */
#define NANDC_INTEN_DMA_INT_EN_SHIFT                       (0U)
#define NANDC_INTEN_DMA_INT_EN_MASK                        (0x1U << NANDC_INTEN_DMA_INT_EN_SHIFT)                       /* 0x00000001 */
#define NANDC_INTEN_FRDY_INT_EN_SHIFT                      (1U)
#define NANDC_INTEN_FRDY_INT_EN_MASK                       (0x1U << NANDC_INTEN_FRDY_INT_EN_SHIFT)                      /* 0x00000002 */
#define NANDC_INTEN_BCHERR_INT_EN_SHIFT                    (2U)
#define NANDC_INTEN_BCHERR_INT_EN_MASK                     (0x1U << NANDC_INTEN_BCHERR_INT_EN_SHIFT)                    /* 0x00000004 */
#define NANDC_INTEN_BCHFAIL_INT_EN_SHIFT                   (3U)
#define NANDC_INTEN_BCHFAIL_INT_EN_MASK                    (0x1U << NANDC_INTEN_BCHFAIL_INT_EN_SHIFT)                   /* 0x00000008 */
#define NANDC_INTEN_LLP_INT_EN_SHIFT                       (4U)
#define NANDC_INTEN_LLP_INT_EN_MASK                        (0x1U << NANDC_INTEN_LLP_INT_EN_SHIFT)                       /* 0x00000010 */
#define NANDC_INTEN_FLASH_ABORT_INT_EN_SHIFT               (5U)
#define NANDC_INTEN_FLASH_ABORT_INT_EN_MASK                (0x1U << NANDC_INTEN_FLASH_ABORT_INT_EN_SHIFT)               /* 0x00000020 */
#define NANDC_INTEN_MASTER_IDLE_INT_EN_SHIFT               (6U)
#define NANDC_INTEN_MASTER_IDLE_INT_EN_MASK                (0x1U << NANDC_INTEN_MASTER_IDLE_INT_EN_SHIFT)               /* 0x00000040 */
/* INTCLR */
#define NANDC_INTCLR_DMA_INT_CLR_SHIFT                     (0U)
#define NANDC_INTCLR_DMA_INT_CLR_MASK                      (0x1U << NANDC_INTCLR_DMA_INT_CLR_SHIFT)                     /* 0x00000001 */
#define NANDC_INTCLR_FRDY_INT_CLR_SHIFT                    (1U)
#define NANDC_INTCLR_FRDY_INT_CLR_MASK                     (0x1U << NANDC_INTCLR_FRDY_INT_CLR_SHIFT)                    /* 0x00000002 */
#define NANDC_INTCLR_BCHERR_INT_CLR_SHIFT                  (2U)
#define NANDC_INTCLR_BCHERR_INT_CLR_MASK                   (0x1U << NANDC_INTCLR_BCHERR_INT_CLR_SHIFT)                  /* 0x00000004 */
#define NANDC_INTCLR_BCHFAIL_INT_CLR_SHIFT                 (3U)
#define NANDC_INTCLR_BCHFAIL_INT_CLR_MASK                  (0x1U << NANDC_INTCLR_BCHFAIL_INT_CLR_SHIFT)                 /* 0x00000008 */
#define NANDC_INTCLR_LLP_INT_CLR_SHIFT                     (4U)
#define NANDC_INTCLR_LLP_INT_CLR_MASK                      (0x1U << NANDC_INTCLR_LLP_INT_CLR_SHIFT)                     /* 0x00000010 */
#define NANDC_INTCLR_FLASH_ABORT_INT_CLR_SHIFT             (5U)
#define NANDC_INTCLR_FLASH_ABORT_INT_CLR_MASK              (0x1U << NANDC_INTCLR_FLASH_ABORT_INT_CLR_SHIFT)             /* 0x00000020 */
#define NANDC_INTCLR_MASTER_IDLE_INT_CLR_SHIFT             (6U)
#define NANDC_INTCLR_MASTER_IDLE_INT_CLR_MASK              (0x1U << NANDC_INTCLR_MASTER_IDLE_INT_CLR_SHIFT)             /* 0x00000040 */
/* INTST */
#define NANDC_INTST_DMA_INT_STAT_SHIFT                     (0U)
#define NANDC_INTST_DMA_INT_STAT_MASK                      (0x1U << NANDC_INTST_DMA_INT_STAT_SHIFT)                     /* 0x00000001 */
#define NANDC_INTST_FRDY_INT_STAT_SHIFT                    (1U)
#define NANDC_INTST_FRDY_INT_STAT_MASK                     (0x1U << NANDC_INTST_FRDY_INT_STAT_SHIFT)                    /* 0x00000002 */
#define NANDC_INTST_BCHERR_INT_STAT_SHIFT                  (2U)
#define NANDC_INTST_BCHERR_INT_STAT_MASK                   (0x1U << NANDC_INTST_BCHERR_INT_STAT_SHIFT)                  /* 0x00000004 */
#define NANDC_INTST_BCHFAIL_INT_STAT_SHIFT                 (3U)
#define NANDC_INTST_BCHFAIL_INT_STAT_MASK                  (0x1U << NANDC_INTST_BCHFAIL_INT_STAT_SHIFT)                 /* 0x00000008 */
#define NANDC_INTST_LLP_INT_STAT_SHIFT                     (4U)
#define NANDC_INTST_LLP_INT_STAT_MASK                      (0x1U << NANDC_INTST_LLP_INT_STAT_SHIFT)                     /* 0x00000010 */
#define NANDC_INTST_FLASH_ABORT_INT_STAT_SHIFT             (5U)
#define NANDC_INTST_FLASH_ABORT_INT_STAT_MASK              (0x1U << NANDC_INTST_FLASH_ABORT_INT_STAT_SHIFT)             /* 0x00000020 */
#define NANDC_INTST_MASTER_IDLE_INT_STAT_SHIFT             (6U)
#define NANDC_INTST_MASTER_IDLE_INT_STAT_MASK              (0x1U << NANDC_INTST_MASTER_IDLE_INT_STAT_SHIFT)             /* 0x00000040 */
/* SPARE0_0 */
#define NANDC_SPARE0_0_SYSTEM_0_SHIFT                      (0U)
#define NANDC_SPARE0_0_SYSTEM_0_MASK                       (0xFFU << NANDC_SPARE0_0_SYSTEM_0_SHIFT)                     /* 0x000000FF */
#define NANDC_SPARE0_0_SYSTEM_1_SHIFT                      (8U)
#define NANDC_SPARE0_0_SYSTEM_1_MASK                       (0xFFU << NANDC_SPARE0_0_SYSTEM_1_SHIFT)                     /* 0x0000FF00 */
#define NANDC_SPARE0_0_SYSTEM_2_SHIFT                      (16U)
#define NANDC_SPARE0_0_SYSTEM_2_MASK                       (0xFFU << NANDC_SPARE0_0_SYSTEM_2_SHIFT)                     /* 0x00FF0000 */
#define NANDC_SPARE0_0_SYSTEM_3_SHIFT                      (24U)
#define NANDC_SPARE0_0_SYSTEM_3_MASK                       (0xFFU << NANDC_SPARE0_0_SYSTEM_3_SHIFT)                     /* 0xFF000000 */
/* SPARE0_1 */
#define NANDC_SPARE0_1_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_1_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_1_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_1_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_1_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_1_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_1_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_1_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_1_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_1_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_1_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_1_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_2 */
#define NANDC_SPARE0_2_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_2_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_2_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_2_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_2_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_2_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_2_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_2_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_2_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_2_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_2_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_2_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_3 */
#define NANDC_SPARE0_3_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_3_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_3_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_3_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_3_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_3_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_3_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_3_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_3_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_3_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_3_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_3_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_4 */
#define NANDC_SPARE0_4_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_4_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_4_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_4_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_4_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_4_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_4_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_4_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_4_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_4_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_4_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_4_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_5 */
#define NANDC_SPARE0_5_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_5_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_5_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_5_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_5_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_5_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_5_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_5_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_5_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_5_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_5_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_5_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_6 */
#define NANDC_SPARE0_6_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_6_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_6_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_6_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_6_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_6_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_6_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_6_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_6_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_6_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_6_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_6_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_7 */
#define NANDC_SPARE0_7_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_7_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_7_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_7_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_7_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_7_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_7_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_7_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_7_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_7_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_7_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_7_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_8 */
#define NANDC_SPARE0_8_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_8_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_8_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_8_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_8_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_8_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_8_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_8_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_8_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_8_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_8_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_8_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_9 */
#define NANDC_SPARE0_9_BCH0_4X_0_SHIFT                     (0U)
#define NANDC_SPARE0_9_BCH0_4X_0_MASK                      (0xFFU << NANDC_SPARE0_9_BCH0_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE0_9_BCH0_4X_1_SHIFT                     (8U)
#define NANDC_SPARE0_9_BCH0_4X_1_MASK                      (0xFFU << NANDC_SPARE0_9_BCH0_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE0_9_BCH0_4X_2_SHIFT                     (16U)
#define NANDC_SPARE0_9_BCH0_4X_2_MASK                      (0xFFU << NANDC_SPARE0_9_BCH0_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE0_9_BCH0_4X_3_SHIFT                     (24U)
#define NANDC_SPARE0_9_BCH0_4X_3_MASK                      (0xFFU << NANDC_SPARE0_9_BCH0_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE0_10 */
#define NANDC_SPARE0_10_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_10_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_10_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_10_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_10_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_10_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_10_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_10_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_10_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_10_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_10_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_10_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_11 */
#define NANDC_SPARE0_11_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_11_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_11_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_11_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_11_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_11_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_11_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_11_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_11_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_11_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_11_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_11_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_0 */
#define NANDC_SPARE1_0_SYSTEM_0_SHIFT                      (0U)
#define NANDC_SPARE1_0_SYSTEM_0_MASK                       (0xFFU << NANDC_SPARE1_0_SYSTEM_0_SHIFT)                     /* 0x000000FF */
#define NANDC_SPARE1_0_SYSTEM_1_SHIFT                      (8U)
#define NANDC_SPARE1_0_SYSTEM_1_MASK                       (0xFFU << NANDC_SPARE1_0_SYSTEM_1_SHIFT)                     /* 0x0000FF00 */
#define NANDC_SPARE1_0_SYSTEM_2_SHIFT                      (16U)
#define NANDC_SPARE1_0_SYSTEM_2_MASK                       (0xFFU << NANDC_SPARE1_0_SYSTEM_2_SHIFT)                     /* 0x00FF0000 */
#define NANDC_SPARE1_0_SYSTEM_3_SHIFT                      (24U)
#define NANDC_SPARE1_0_SYSTEM_3_MASK                       (0xFFU << NANDC_SPARE1_0_SYSTEM_3_SHIFT)                     /* 0xFF000000 */
/* SPARE1_1 */
#define NANDC_SPARE1_1_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_1_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_1_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_1_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_1_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_1_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_1_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_1_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_1_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_1_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_1_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_1_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_2 */
#define NANDC_SPARE1_2_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_2_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_2_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_2_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_2_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_2_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_2_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_2_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_2_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_2_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_2_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_2_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_3 */
#define NANDC_SPARE1_3_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_3_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_3_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_3_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_3_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_3_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_3_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_3_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_3_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_3_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_3_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_3_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_4 */
#define NANDC_SPARE1_4_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_4_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_4_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_4_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_4_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_4_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_4_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_4_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_4_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_4_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_4_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_4_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_5 */
#define NANDC_SPARE1_5_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_5_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_5_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_5_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_5_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_5_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_5_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_5_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_5_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_5_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_5_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_5_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_6 */
#define NANDC_SPARE1_6_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_6_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_6_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_6_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_6_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_6_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_6_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_6_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_6_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_6_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_6_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_6_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_7 */
#define NANDC_SPARE1_7_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_7_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_7_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_7_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_7_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_7_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_7_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_7_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_7_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_7_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_7_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_7_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_8 */
#define NANDC_SPARE1_8_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_8_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_8_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_8_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_8_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_8_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_8_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_8_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_8_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_8_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_8_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_8_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_9 */
#define NANDC_SPARE1_9_BCH1_4X_0_SHIFT                     (0U)
#define NANDC_SPARE1_9_BCH1_4X_0_MASK                      (0xFFU << NANDC_SPARE1_9_BCH1_4X_0_SHIFT)                    /* 0x000000FF */
#define NANDC_SPARE1_9_BCH1_4X_1_SHIFT                     (8U)
#define NANDC_SPARE1_9_BCH1_4X_1_MASK                      (0xFFU << NANDC_SPARE1_9_BCH1_4X_1_SHIFT)                    /* 0x0000FF00 */
#define NANDC_SPARE1_9_BCH1_4X_2_SHIFT                     (16U)
#define NANDC_SPARE1_9_BCH1_4X_2_MASK                      (0xFFU << NANDC_SPARE1_9_BCH1_4X_2_SHIFT)                    /* 0x00FF0000 */
#define NANDC_SPARE1_9_BCH1_4X_3_SHIFT                     (24U)
#define NANDC_SPARE1_9_BCH1_4X_3_MASK                      (0xFFU << NANDC_SPARE1_9_BCH1_4X_3_SHIFT)                    /* 0xFF000000 */
/* SPARE1_10 */
#define NANDC_SPARE1_10_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_10_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_10_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_10_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_10_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_10_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_10_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_10_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_10_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_10_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_10_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_10_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_11 */
#define NANDC_SPARE1_11_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_11_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_11_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_11_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_11_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_11_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_11_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_11_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_11_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_11_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_11_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_11_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_12 */
#define NANDC_SPARE0_12_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_12_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_12_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_12_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_12_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_12_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_12_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_12_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_12_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_12_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_12_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_12_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_13 */
#define NANDC_SPARE0_13_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_13_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_13_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_13_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_13_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_13_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_13_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_13_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_13_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_13_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_13_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_13_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_14 */
#define NANDC_SPARE0_14_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_14_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_14_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_14_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_14_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_14_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_14_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_14_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_14_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_14_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_14_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_14_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_15 */
#define NANDC_SPARE0_15_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_15_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_15_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_15_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_15_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_15_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_15_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_15_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_15_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_15_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_15_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_15_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_16 */
#define NANDC_SPARE0_16_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_16_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_16_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_16_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_16_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_16_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_16_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_16_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_16_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_16_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_16_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_16_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_17 */
#define NANDC_SPARE0_17_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_17_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_17_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_17_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_17_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_17_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_17_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_17_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_17_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_17_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_17_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_17_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_18 */
#define NANDC_SPARE0_18_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_18_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_18_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_18_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_18_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_18_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_18_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_18_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_18_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_18_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_18_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_18_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_19 */
#define NANDC_SPARE0_19_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_19_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_19_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_19_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_19_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_19_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_19_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_19_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_19_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_19_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_19_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_19_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_20 */
#define NANDC_SPARE0_20_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_20_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_20_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_20_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_20_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_20_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_20_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_20_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_20_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_20_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_20_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_20_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_21 */
#define NANDC_SPARE0_21_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_21_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_21_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_21_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_21_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_21_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_21_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_21_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_21_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_21_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_21_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_21_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_22 */
#define NANDC_SPARE0_22_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_22_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_22_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_22_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_22_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_22_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_22_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_22_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_22_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_22_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_22_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_22_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_23 */
#define NANDC_SPARE0_23_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_23_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_23_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_23_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_23_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_23_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_23_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_23_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_23_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_23_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_23_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_23_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_24 */
#define NANDC_SPARE0_24_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_24_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_24_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_24_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_24_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_24_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_24_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_24_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_24_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_24_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_24_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_24_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_25 */
#define NANDC_SPARE0_25_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_25_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_25_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_25_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_25_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_25_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_25_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_25_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_25_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_25_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_25_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_25_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_26 */
#define NANDC_SPARE0_26_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_26_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_26_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE0_26_BCH0_4X_1_SHIFT                    (8U)
#define NANDC_SPARE0_26_BCH0_4X_1_MASK                     (0xFFU << NANDC_SPARE0_26_BCH0_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE0_26_BCH0_4X_2_SHIFT                    (16U)
#define NANDC_SPARE0_26_BCH0_4X_2_MASK                     (0xFFU << NANDC_SPARE0_26_BCH0_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE0_26_BCH0_4X_3_SHIFT                    (24U)
#define NANDC_SPARE0_26_BCH0_4X_3_MASK                     (0xFFU << NANDC_SPARE0_26_BCH0_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE0_27 */
#define NANDC_SPARE0_27_BCH0_4X_0_SHIFT                    (0U)
#define NANDC_SPARE0_27_BCH0_4X_0_MASK                     (0xFFU << NANDC_SPARE0_27_BCH0_4X_0_SHIFT)                   /* 0x000000FF */
/* SPARE1_12 */
#define NANDC_SPARE1_12_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_12_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_12_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_12_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_12_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_12_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_12_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_12_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_12_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_12_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_12_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_12_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_13 */
#define NANDC_SPARE1_13_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_13_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_13_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_13_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_13_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_13_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_13_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_13_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_13_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_13_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_13_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_13_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_14 */
#define NANDC_SPARE1_14_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_14_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_14_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_14_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_14_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_14_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_14_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_14_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_14_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_14_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_14_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_14_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_15 */
#define NANDC_SPARE1_15_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_15_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_15_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_15_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_15_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_15_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_15_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_15_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_15_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_15_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_15_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_15_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_16 */
#define NANDC_SPARE1_16_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_16_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_16_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_16_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_16_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_16_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_16_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_16_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_16_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_16_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_16_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_16_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_17 */
#define NANDC_SPARE1_17_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_17_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_17_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_17_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_17_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_17_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_17_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_17_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_17_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_17_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_17_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_17_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_18 */
#define NANDC_SPARE1_18_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_18_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_18_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_18_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_18_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_18_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_18_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_18_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_18_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_18_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_18_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_18_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_19 */
#define NANDC_SPARE1_19_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_19_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_19_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_19_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_19_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_19_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_19_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_19_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_19_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_19_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_19_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_19_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_20 */
#define NANDC_SPARE1_20_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_20_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_20_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_20_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_20_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_20_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_20_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_20_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_20_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_20_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_20_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_20_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_21 */
#define NANDC_SPARE1_21_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_21_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_21_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_21_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_21_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_21_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_21_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_21_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_21_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_21_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_21_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_21_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_22 */
#define NANDC_SPARE1_22_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_22_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_22_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_22_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_22_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_22_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_22_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_22_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_22_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_22_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_22_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_22_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_23 */
#define NANDC_SPARE1_23_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_23_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_23_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_23_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_23_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_23_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_23_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_23_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_23_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_23_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_23_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_23_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_24 */
#define NANDC_SPARE1_24_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_24_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_24_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_24_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_24_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_24_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_24_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_24_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_24_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_24_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_24_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_24_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_25 */
#define NANDC_SPARE1_25_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_25_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_25_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_25_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_25_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_25_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_25_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_25_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_25_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_25_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_25_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_25_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_26 */
#define NANDC_SPARE1_26_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_26_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_26_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
#define NANDC_SPARE1_26_BCH1_4X_1_SHIFT                    (8U)
#define NANDC_SPARE1_26_BCH1_4X_1_MASK                     (0xFFU << NANDC_SPARE1_26_BCH1_4X_1_SHIFT)                   /* 0x0000FF00 */
#define NANDC_SPARE1_26_BCH1_4X_2_SHIFT                    (16U)
#define NANDC_SPARE1_26_BCH1_4X_2_MASK                     (0xFFU << NANDC_SPARE1_26_BCH1_4X_2_SHIFT)                   /* 0x00FF0000 */
#define NANDC_SPARE1_26_BCH1_4X_3_SHIFT                    (24U)
#define NANDC_SPARE1_26_BCH1_4X_3_MASK                     (0xFFU << NANDC_SPARE1_26_BCH1_4X_3_SHIFT)                   /* 0xFF000000 */
/* SPARE1_27 */
#define NANDC_SPARE1_27_BCH1_4X_0_SHIFT                    (0U)
#define NANDC_SPARE1_27_BCH1_4X_0_MASK                     (0xFFU << NANDC_SPARE1_27_BCH1_4X_0_SHIFT)                   /* 0x000000FF */
/* BCHDE0_24 */
#define NANDC_BCHDE0_24_BCHDE0_24_SHIFT                    (0U)
#define NANDC_BCHDE0_24_BCHDE0_24_MASK                     (0x7FFFFU << NANDC_BCHDE0_24_BCHDE0_24_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_25 */
#define NANDC_BCHDE0_25_BCHDE0_25_SHIFT                    (0U)
#define NANDC_BCHDE0_25_BCHDE0_25_MASK                     (0x7FFFFU << NANDC_BCHDE0_25_BCHDE0_25_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_26 */
#define NANDC_BCHDE0_26_BCHDE0_26_SHIFT                    (0U)
#define NANDC_BCHDE0_26_BCHDE0_26_MASK                     (0x7FFFFU << NANDC_BCHDE0_26_BCHDE0_26_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_27 */
#define NANDC_BCHDE0_27_BCHDE0_27_SHIFT                    (0U)
#define NANDC_BCHDE0_27_BCHDE0_27_MASK                     (0x7FFFFU << NANDC_BCHDE0_27_BCHDE0_27_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_28 */
#define NANDC_BCHDE0_28_BCHDE0_28_SHIFT                    (0U)
#define NANDC_BCHDE0_28_BCHDE0_28_MASK                     (0x7FFFFU << NANDC_BCHDE0_28_BCHDE0_28_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_29 */
#define NANDC_BCHDE0_29_BCHDE0_29_SHIFT                    (0U)
#define NANDC_BCHDE0_29_BCHDE0_29_MASK                     (0x7FFFFU << NANDC_BCHDE0_29_BCHDE0_29_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_30 */
#define NANDC_BCHDE0_30_BCHDE0_30_SHIFT                    (0U)
#define NANDC_BCHDE0_30_BCHDE0_30_MASK                     (0x7FFFFU << NANDC_BCHDE0_30_BCHDE0_30_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_31 */
#define NANDC_BCHDE0_31_BCHDE0_31_SHIFT                    (0U)
#define NANDC_BCHDE0_31_BCHDE0_31_MASK                     (0x7FFFFU << NANDC_BCHDE0_31_BCHDE0_31_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_32 */
#define NANDC_BCHDE0_32_BCHDE0_3_SHIFT                     (0U)
#define NANDC_BCHDE0_32_BCHDE0_3_MASK                      (0x7FFFFU << NANDC_BCHDE0_32_BCHDE0_3_SHIFT)                 /* 0x0007FFFF */
/* BCHDE0_33 */
#define NANDC_BCHDE0_33_BCHDE0_33_SHIFT                    (0U)
#define NANDC_BCHDE0_33_BCHDE0_33_MASK                     (0x7FFFFU << NANDC_BCHDE0_33_BCHDE0_33_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_34 */
#define NANDC_BCHDE0_34_BCHDE0_34_SHIFT                    (0U)
#define NANDC_BCHDE0_34_BCHDE0_34_MASK                     (0x7FFFFU << NANDC_BCHDE0_34_BCHDE0_34_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_35 */
#define NANDC_BCHDE0_35_BCHDE0_35_SHIFT                    (0U)
#define NANDC_BCHDE0_35_BCHDE0_35_MASK                     (0x7FFFFU << NANDC_BCHDE0_35_BCHDE0_35_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_36 */
#define NANDC_BCHDE0_36_BCHDE0_36_SHIFT                    (0U)
#define NANDC_BCHDE0_36_BCHDE0_36_MASK                     (0x7FFFFU << NANDC_BCHDE0_36_BCHDE0_36_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_37 */
#define NANDC_BCHDE0_37_BCHDE0_37_SHIFT                    (0U)
#define NANDC_BCHDE0_37_BCHDE0_37_MASK                     (0x7FFFFU << NANDC_BCHDE0_37_BCHDE0_37_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_38 */
#define NANDC_BCHDE0_38_BCHDE0_38_SHIFT                    (0U)
#define NANDC_BCHDE0_38_BCHDE0_38_MASK                     (0x7FFFFU << NANDC_BCHDE0_38_BCHDE0_38_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_39 */
#define NANDC_BCHDE0_39_BCHDE0_39_SHIFT                    (0U)
#define NANDC_BCHDE0_39_BCHDE0_39_MASK                     (0x7FFFFU << NANDC_BCHDE0_39_BCHDE0_39_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_40 */
#define NANDC_BCHDE0_40_BCHDE0_40_SHIFT                    (0U)
#define NANDC_BCHDE0_40_BCHDE0_40_MASK                     (0x7FFFFU << NANDC_BCHDE0_40_BCHDE0_40_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_41 */
#define NANDC_BCHDE0_41_BCHDE0_41_SHIFT                    (0U)
#define NANDC_BCHDE0_41_BCHDE0_41_MASK                     (0x7FFFFU << NANDC_BCHDE0_41_BCHDE0_41_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_42 */
#define NANDC_BCHDE0_42_BCHDE0_42_SHIFT                    (0U)
#define NANDC_BCHDE0_42_BCHDE0_42_MASK                     (0x7FFFFU << NANDC_BCHDE0_42_BCHDE0_42_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_43 */
#define NANDC_BCHDE0_43_BCHDE0_43_SHIFT                    (0U)
#define NANDC_BCHDE0_43_BCHDE0_43_MASK                     (0x7FFFFU << NANDC_BCHDE0_43_BCHDE0_43_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_44 */
#define NANDC_BCHDE0_44_BCHDE0_44_SHIFT                    (0U)
#define NANDC_BCHDE0_44_BCHDE0_44_MASK                     (0x7FFFFU << NANDC_BCHDE0_44_BCHDE0_44_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_45 */
#define NANDC_BCHDE0_45_BCHDE0_45_SHIFT                    (0U)
#define NANDC_BCHDE0_45_BCHDE0_45_MASK                     (0x7FFFFU << NANDC_BCHDE0_45_BCHDE0_45_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_46 */
#define NANDC_BCHDE0_46_BCHDE0_46_SHIFT                    (0U)
#define NANDC_BCHDE0_46_BCHDE0_46_MASK                     (0x7FFFFU << NANDC_BCHDE0_46_BCHDE0_46_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_47 */
#define NANDC_BCHDE0_47_BCHDE0_47_SHIFT                    (0U)
#define NANDC_BCHDE0_47_BCHDE0_47_MASK                     (0x7FFFFU << NANDC_BCHDE0_47_BCHDE0_47_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_48 */
#define NANDC_BCHDE0_48_BCHDE0_48_SHIFT                    (0U)
#define NANDC_BCHDE0_48_BCHDE0_48_MASK                     (0x7FFFFU << NANDC_BCHDE0_48_BCHDE0_48_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_49 */
#define NANDC_BCHDE0_49_BCHDE0_49_SHIFT                    (0U)
#define NANDC_BCHDE0_49_BCHDE0_49_MASK                     (0x7FFFFU << NANDC_BCHDE0_49_BCHDE0_49_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_50 */
#define NANDC_BCHDE0_50_BCHDE0_50_SHIFT                    (0U)
#define NANDC_BCHDE0_50_BCHDE0_50_MASK                     (0x7FFFFU << NANDC_BCHDE0_50_BCHDE0_50_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_51 */
#define NANDC_BCHDE0_51_BCHDE0_51_SHIFT                    (0U)
#define NANDC_BCHDE0_51_BCHDE0_51_MASK                     (0x7FFFFU << NANDC_BCHDE0_51_BCHDE0_51_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_52 */
#define NANDC_BCHDE0_52_BCHDE0_52_SHIFT                    (0U)
#define NANDC_BCHDE0_52_BCHDE0_52_MASK                     (0x7FFFFU << NANDC_BCHDE0_52_BCHDE0_52_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_53 */
#define NANDC_BCHDE0_53_BCHDE0_53_SHIFT                    (0U)
#define NANDC_BCHDE0_53_BCHDE0_53_MASK                     (0x7FFFFU << NANDC_BCHDE0_53_BCHDE0_53_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_54 */
#define NANDC_BCHDE0_54_BCHDE0_54_SHIFT                    (0U)
#define NANDC_BCHDE0_54_BCHDE0_54_MASK                     (0x7FFFFU << NANDC_BCHDE0_54_BCHDE0_54_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_55 */
#define NANDC_BCHDE0_55_BCHDE0_55_SHIFT                    (0U)
#define NANDC_BCHDE0_55_BCHDE0_55_MASK                     (0x7FFFFU << NANDC_BCHDE0_55_BCHDE0_55_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_56 */
#define NANDC_BCHDE0_56_BCHDE0_56_SHIFT                    (0U)
#define NANDC_BCHDE0_56_BCHDE0_56_MASK                     (0x7FFFFU << NANDC_BCHDE0_56_BCHDE0_56_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_57 */
#define NANDC_BCHDE0_57_BCHDE0_57_SHIFT                    (0U)
#define NANDC_BCHDE0_57_BCHDE0_57_MASK                     (0x7FFFFU << NANDC_BCHDE0_57_BCHDE0_57_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_58 */
#define NANDC_BCHDE0_58_BCHDE0_58_SHIFT                    (0U)
#define NANDC_BCHDE0_58_BCHDE0_58_MASK                     (0x7FFFFU << NANDC_BCHDE0_58_BCHDE0_58_SHIFT)                /* 0x0007FFFF */
/* BCHDE0_59 */
#define NANDC_BCHDE0_59_BCHDE0_59_SHIFT                    (0U)
#define NANDC_BCHDE0_59_BCHDE0_59_MASK                     (0x7FFFFU << NANDC_BCHDE0_59_BCHDE0_59_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_24 */
#define NANDC_BCHDE1_24_BCHDE1_24_SHIFT                    (0U)
#define NANDC_BCHDE1_24_BCHDE1_24_MASK                     (0x7FFFFU << NANDC_BCHDE1_24_BCHDE1_24_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_25 */
#define NANDC_BCHDE1_25_BCHDE1_25_SHIFT                    (0U)
#define NANDC_BCHDE1_25_BCHDE1_25_MASK                     (0x7FFFFU << NANDC_BCHDE1_25_BCHDE1_25_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_26 */
#define NANDC_BCHDE1_26_BCHDE1_26_SHIFT                    (0U)
#define NANDC_BCHDE1_26_BCHDE1_26_MASK                     (0x7FFFFU << NANDC_BCHDE1_26_BCHDE1_26_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_27 */
#define NANDC_BCHDE1_27_BCHDE1_27_SHIFT                    (0U)
#define NANDC_BCHDE1_27_BCHDE1_27_MASK                     (0x7FFFFU << NANDC_BCHDE1_27_BCHDE1_27_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_28 */
#define NANDC_BCHDE1_28_BCHDE1_28_SHIFT                    (0U)
#define NANDC_BCHDE1_28_BCHDE1_28_MASK                     (0x7FFFFU << NANDC_BCHDE1_28_BCHDE1_28_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_29 */
#define NANDC_BCHDE1_29_BCHDE1_29_SHIFT                    (0U)
#define NANDC_BCHDE1_29_BCHDE1_29_MASK                     (0x7FFFFU << NANDC_BCHDE1_29_BCHDE1_29_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_30 */
#define NANDC_BCHDE1_30_BCHDE1_30_SHIFT                    (0U)
#define NANDC_BCHDE1_30_BCHDE1_30_MASK                     (0x7FFFFU << NANDC_BCHDE1_30_BCHDE1_30_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_31 */
#define NANDC_BCHDE1_31_BCHDE1_31_SHIFT                    (0U)
#define NANDC_BCHDE1_31_BCHDE1_31_MASK                     (0x7FFFFU << NANDC_BCHDE1_31_BCHDE1_31_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_32 */
#define NANDC_BCHDE1_32_BCHDE1_32_SHIFT                    (0U)
#define NANDC_BCHDE1_32_BCHDE1_32_MASK                     (0x7FFFFU << NANDC_BCHDE1_32_BCHDE1_32_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_33 */
#define NANDC_BCHDE1_33_BCHDE1_33_SHIFT                    (0U)
#define NANDC_BCHDE1_33_BCHDE1_33_MASK                     (0x7FFFFU << NANDC_BCHDE1_33_BCHDE1_33_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_34 */
#define NANDC_BCHDE1_34_BCHDE1_34_SHIFT                    (0U)
#define NANDC_BCHDE1_34_BCHDE1_34_MASK                     (0x7FFFFU << NANDC_BCHDE1_34_BCHDE1_34_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_35 */
#define NANDC_BCHDE1_35_BCHDE1_35_SHIFT                    (0U)
#define NANDC_BCHDE1_35_BCHDE1_35_MASK                     (0x7FFFFU << NANDC_BCHDE1_35_BCHDE1_35_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_36 */
#define NANDC_BCHDE1_36_BCHDE1_36_SHIFT                    (0U)
#define NANDC_BCHDE1_36_BCHDE1_36_MASK                     (0x7FFFFU << NANDC_BCHDE1_36_BCHDE1_36_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_37 */
#define NANDC_BCHDE1_37_BCHDE1_37_SHIFT                    (0U)
#define NANDC_BCHDE1_37_BCHDE1_37_MASK                     (0x7FFFFU << NANDC_BCHDE1_37_BCHDE1_37_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_38 */
#define NANDC_BCHDE1_38_BCHDE1_38_SHIFT                    (0U)
#define NANDC_BCHDE1_38_BCHDE1_38_MASK                     (0x7FFFFU << NANDC_BCHDE1_38_BCHDE1_38_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_39 */
#define NANDC_BCHDE1_39_BCHDE1_39_SHIFT                    (0U)
#define NANDC_BCHDE1_39_BCHDE1_39_MASK                     (0x7FFFFU << NANDC_BCHDE1_39_BCHDE1_39_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_40 */
#define NANDC_BCHDE1_40_BCHDE1_40_SHIFT                    (0U)
#define NANDC_BCHDE1_40_BCHDE1_40_MASK                     (0x7FFFFU << NANDC_BCHDE1_40_BCHDE1_40_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_41 */
#define NANDC_BCHDE1_41_BCHDE1_41_SHIFT                    (0U)
#define NANDC_BCHDE1_41_BCHDE1_41_MASK                     (0x7FFFFU << NANDC_BCHDE1_41_BCHDE1_41_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_42 */
#define NANDC_BCHDE1_42_BCHDE1_42_SHIFT                    (0U)
#define NANDC_BCHDE1_42_BCHDE1_42_MASK                     (0x7FFFFU << NANDC_BCHDE1_42_BCHDE1_42_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_43 */
#define NANDC_BCHDE1_43_BCHDE1_43_SHIFT                    (0U)
#define NANDC_BCHDE1_43_BCHDE1_43_MASK                     (0x7FFFFU << NANDC_BCHDE1_43_BCHDE1_43_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_44 */
#define NANDC_BCHDE1_44_BCHDE1_44_SHIFT                    (0U)
#define NANDC_BCHDE1_44_BCHDE1_44_MASK                     (0x7FFFFU << NANDC_BCHDE1_44_BCHDE1_44_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_45 */
#define NANDC_BCHDE1_45_BCHDE1_45_SHIFT                    (0U)
#define NANDC_BCHDE1_45_BCHDE1_45_MASK                     (0x7FFFFU << NANDC_BCHDE1_45_BCHDE1_45_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_46 */
#define NANDC_BCHDE1_46_BCHDE1_46_SHIFT                    (0U)
#define NANDC_BCHDE1_46_BCHDE1_46_MASK                     (0x7FFFFU << NANDC_BCHDE1_46_BCHDE1_46_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_47 */
#define NANDC_BCHDE1_47_BCHDE1_47_SHIFT                    (0U)
#define NANDC_BCHDE1_47_BCHDE1_47_MASK                     (0x7FFFFU << NANDC_BCHDE1_47_BCHDE1_47_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_48 */
#define NANDC_BCHDE1_48_BCHDE1_48_SHIFT                    (0U)
#define NANDC_BCHDE1_48_BCHDE1_48_MASK                     (0x7FFFFU << NANDC_BCHDE1_48_BCHDE1_48_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_49 */
#define NANDC_BCHDE1_49_BCHDE1_49_SHIFT                    (0U)
#define NANDC_BCHDE1_49_BCHDE1_49_MASK                     (0x7FFFFU << NANDC_BCHDE1_49_BCHDE1_49_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_50 */
#define NANDC_BCHDE1_50_BCHDE1_50_SHIFT                    (0U)
#define NANDC_BCHDE1_50_BCHDE1_50_MASK                     (0x7FFFFU << NANDC_BCHDE1_50_BCHDE1_50_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_51 */
#define NANDC_BCHDE1_51_BCHDE1_51_SHIFT                    (0U)
#define NANDC_BCHDE1_51_BCHDE1_51_MASK                     (0x7FFFFU << NANDC_BCHDE1_51_BCHDE1_51_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_52 */
#define NANDC_BCHDE1_52_BCHDE1_52_SHIFT                    (0U)
#define NANDC_BCHDE1_52_BCHDE1_52_MASK                     (0x7FFFFU << NANDC_BCHDE1_52_BCHDE1_52_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_53 */
#define NANDC_BCHDE1_53_BCHDE1_53_SHIFT                    (0U)
#define NANDC_BCHDE1_53_BCHDE1_53_MASK                     (0x7FFFFU << NANDC_BCHDE1_53_BCHDE1_53_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_54 */
#define NANDC_BCHDE1_54_BCHDE1_54_SHIFT                    (0U)
#define NANDC_BCHDE1_54_BCHDE1_54_MASK                     (0x7FFFFU << NANDC_BCHDE1_54_BCHDE1_54_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_55 */
#define NANDC_BCHDE1_55_BCHDE1_55_SHIFT                    (0U)
#define NANDC_BCHDE1_55_BCHDE1_55_MASK                     (0x7FFFFU << NANDC_BCHDE1_55_BCHDE1_55_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_56 */
#define NANDC_BCHDE1_56_BCHDE1_56_SHIFT                    (0U)
#define NANDC_BCHDE1_56_BCHDE1_56_MASK                     (0x7FFFFU << NANDC_BCHDE1_56_BCHDE1_56_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_57 */
#define NANDC_BCHDE1_57_BCHDE1_57_SHIFT                    (0U)
#define NANDC_BCHDE1_57_BCHDE1_57_MASK                     (0x7FFFFU << NANDC_BCHDE1_57_BCHDE1_57_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_58 */
#define NANDC_BCHDE1_58_BCHDE1_58_SHIFT                    (0U)
#define NANDC_BCHDE1_58_BCHDE1_58_MASK                     (0x7FFFFU << NANDC_BCHDE1_58_BCHDE1_58_SHIFT)                /* 0x0007FFFF */
/* BCHDE1_59 */
#define NANDC_BCHDE1_59_BCHDE1_59_SHIFT                    (0U)
#define NANDC_BCHDE1_59_BCHDE1_59_MASK                     (0x7FFFFU << NANDC_BCHDE1_59_BCHDE1_59_SHIFT)                /* 0x0007FFFF */
/* BCHST8 */
#define NANDC_BCHST8_BCHST_CWD16_CWD17_SHIFT               (0U)
#define NANDC_BCHST8_BCHST_CWD16_CWD17_MASK                (0xFFFFFFFFU << NANDC_BCHST8_BCHST_CWD16_CWD17_SHIFT)        /* 0xFFFFFFFF */
/* BCHST9 */
#define NANDC_BCHST9_BCHST_CWD18_CWD19_SHIFT               (0U)
#define NANDC_BCHST9_BCHST_CWD18_CWD19_MASK                (0xFFFFFFFFU << NANDC_BCHST9_BCHST_CWD18_CWD19_SHIFT)        /* 0xFFFFFFFF */
/* BCHST10 */
#define NANDC_BCHST10_BCHST_CWD20_CWD21_SHIFT              (0U)
#define NANDC_BCHST10_BCHST_CWD20_CWD21_MASK               (0xFFFFFFFFU << NANDC_BCHST10_BCHST_CWD20_CWD21_SHIFT)       /* 0xFFFFFFFF */
/* BCHST11 */
#define NANDC_BCHST11_BCHST_CWD22_CWD23_SHIFT              (0U)
#define NANDC_BCHST11_BCHST_CWD22_CWD23_MASK               (0xFFFFFFFFU << NANDC_BCHST11_BCHST_CWD22_CWD23_SHIFT)       /* 0xFFFFFFFF */
/* BCHST12 */
#define NANDC_BCHST12_BCHST_CWD24_CWD25_SHIFT              (0U)
#define NANDC_BCHST12_BCHST_CWD24_CWD25_MASK               (0xFFFFFFFFU << NANDC_BCHST12_BCHST_CWD24_CWD25_SHIFT)       /* 0xFFFFFFFF */
/* BCHST13 */
#define NANDC_BCHST13_BCHST_CWD26_CWD27_SHIFT              (0U)
#define NANDC_BCHST13_BCHST_CWD26_CWD27_MASK               (0xFFFFFFFFU << NANDC_BCHST13_BCHST_CWD26_CWD27_SHIFT)       /* 0xFFFFFFFF */
/* BCHST14 */
#define NANDC_BCHST14_BCHST_CWD28_CWD29_SHIFT              (0U)
#define NANDC_BCHST14_BCHST_CWD28_CWD29_MASK               (0xFFFFFFFFU << NANDC_BCHST14_BCHST_CWD28_CWD29_SHIFT)       /* 0xFFFFFFFF */
/* BCHST15 */
#define NANDC_BCHST15_BCHST_CWD30_CWD31_SHIFT              (0U)
#define NANDC_BCHST15_BCHST_CWD30_CWD31_MASK               (0xFFFFFFFFU << NANDC_BCHST15_BCHST_CWD30_CWD31_SHIFT)       /* 0xFFFFFFFF */
/* RANDMZ_SEED13_0 */
#define NANDC_RANDMZ_SEED13_0_RANDMZ_SEED13_0_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_0_RANDMZ_SEED13_0_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_0_RANDMZ_SEED13_0_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_1 */
#define NANDC_RANDMZ_SEED13_1_RANDMZ_SEED13_1_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_1_RANDMZ_SEED13_1_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_1_RANDMZ_SEED13_1_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_2 */
#define NANDC_RANDMZ_SEED13_2_RANDMZ_SEED13_2_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_2_RANDMZ_SEED13_2_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_2_RANDMZ_SEED13_2_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_3 */
#define NANDC_RANDMZ_SEED13_3_RANDMZ_SEED13_3_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_3_RANDMZ_SEED13_3_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_3_RANDMZ_SEED13_3_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_4 */
#define NANDC_RANDMZ_SEED13_4_RANDMZ_SEED13_4_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_4_RANDMZ_SEED13_4_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_4_RANDMZ_SEED13_4_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_5 */
#define NANDC_RANDMZ_SEED13_5_RANDMZ_SEED13_5_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_5_RANDMZ_SEED13_5_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_5_RANDMZ_SEED13_5_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_6 */
#define NANDC_RANDMZ_SEED13_6_RANDMZ_SEED13_6_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_6_RANDMZ_SEED13_6_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_6_RANDMZ_SEED13_6_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_7 */
#define NANDC_RANDMZ_SEED13_7_RANDMZ_SEED13_7_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_7_RANDMZ_SEED13_7_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_7_RANDMZ_SEED13_7_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_8 */
#define NANDC_RANDMZ_SEED13_8_RANDMZ_SEED13_8_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_8_RANDMZ_SEED13_8_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_8_RANDMZ_SEED13_8_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_9 */
#define NANDC_RANDMZ_SEED13_9_RANDMZ_SEED13_9_SHIFT        (0U)
#define NANDC_RANDMZ_SEED13_9_RANDMZ_SEED13_9_MASK         (0x1FFFU << NANDC_RANDMZ_SEED13_9_RANDMZ_SEED13_9_SHIFT)     /* 0x00001FFF */
/* RANDMZ_SEED13_10 */
#define NANDC_RANDMZ_SEED13_10_RANDMZ_SEED13_10_SHIFT      (0U)
#define NANDC_RANDMZ_SEED13_10_RANDMZ_SEED13_10_MASK       (0x1FFFU << NANDC_RANDMZ_SEED13_10_RANDMZ_SEED13_10_SHIFT)   /* 0x00001FFF */
/* RANDMZ_SEED13_11 */
#define NANDC_RANDMZ_SEED13_11_RANDMZ_SEED13_11_SHIFT      (0U)
#define NANDC_RANDMZ_SEED13_11_RANDMZ_SEED13_11_MASK       (0x1FFFU << NANDC_RANDMZ_SEED13_11_RANDMZ_SEED13_11_SHIFT)   /* 0x00001FFF */
/* RANDMZ_SEED13_12 */
#define NANDC_RANDMZ_SEED13_12_RANDMZ_SEED13_12_SHIFT      (0U)
#define NANDC_RANDMZ_SEED13_12_RANDMZ_SEED13_12_MASK       (0x1FFFU << NANDC_RANDMZ_SEED13_12_RANDMZ_SEED13_12_SHIFT)   /* 0x00001FFF */
/* RANDMZ_SEED13_13 */
#define NANDC_RANDMZ_SEED13_13_RANDMZ_SEED13_13_SHIFT      (0U)
#define NANDC_RANDMZ_SEED13_13_RANDMZ_SEED13_13_MASK       (0x1FFFU << NANDC_RANDMZ_SEED13_13_RANDMZ_SEED13_13_SHIFT)   /* 0x00001FFF */
/* RANDMZ_SEED13_14 */
#define NANDC_RANDMZ_SEED13_14_RANDMZ_SEED13_14_SHIFT      (0U)
#define NANDC_RANDMZ_SEED13_14_RANDMZ_SEED13_14_MASK       (0x1FFFU << NANDC_RANDMZ_SEED13_14_RANDMZ_SEED13_14_SHIFT)   /* 0x00001FFF */
/* RANDMZ_SEED13_15 */
#define NANDC_RANDMZ_SEED13_15_RANDMZ_SEED13_15_SHIFT      (0U)
#define NANDC_RANDMZ_SEED13_15_RANDMZ_SEED13_15_MASK       (0x1FFFU << NANDC_RANDMZ_SEED13_15_RANDMZ_SEED13_15_SHIFT)   /* 0x00001FFF */
/* RANDMZ_SEED17_0 */
#define NANDC_RANDMZ_SEED17_0_RANDMZ_SEED17_0_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_0_RANDMZ_SEED17_0_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_0_RANDMZ_SEED17_0_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_1 */
#define NANDC_RANDMZ_SEED17_1_RANDMZ_SEED17_1_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_1_RANDMZ_SEED17_1_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_1_RANDMZ_SEED17_1_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_2 */
#define NANDC_RANDMZ_SEED17_2_RANDMZ_SEED17_2_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_2_RANDMZ_SEED17_2_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_2_RANDMZ_SEED17_2_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_3 */
#define NANDC_RANDMZ_SEED17_3_RANDMZ_SEED17_3_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_3_RANDMZ_SEED17_3_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_3_RANDMZ_SEED17_3_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_4 */
#define NANDC_RANDMZ_SEED17_4_RANDMZ_SEED17_4_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_4_RANDMZ_SEED17_4_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_4_RANDMZ_SEED17_4_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_5 */
#define NANDC_RANDMZ_SEED17_5_RANDMZ_SEED17_5_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_5_RANDMZ_SEED17_5_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_5_RANDMZ_SEED17_5_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_6 */
#define NANDC_RANDMZ_SEED17_6_RANDMZ_SEED17_6_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_6_RANDMZ_SEED17_6_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_6_RANDMZ_SEED17_6_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_7 */
#define NANDC_RANDMZ_SEED17_7_RANDMZ_SEED17_7_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_7_RANDMZ_SEED17_7_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_7_RANDMZ_SEED17_7_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_8 */
#define NANDC_RANDMZ_SEED17_8_RANDMZ_SEED17_8_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_8_RANDMZ_SEED17_8_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_8_RANDMZ_SEED17_8_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_9 */
#define NANDC_RANDMZ_SEED17_9_RANDMZ_SEED17_9_SHIFT        (0U)
#define NANDC_RANDMZ_SEED17_9_RANDMZ_SEED17_9_MASK         (0x1FFFFU << NANDC_RANDMZ_SEED17_9_RANDMZ_SEED17_9_SHIFT)    /* 0x0001FFFF */
/* RANDMZ_SEED17_10 */
#define NANDC_RANDMZ_SEED17_10_RANDMZ_SEED17_10_SHIFT      (0U)
#define NANDC_RANDMZ_SEED17_10_RANDMZ_SEED17_10_MASK       (0x1FFFFU << NANDC_RANDMZ_SEED17_10_RANDMZ_SEED17_10_SHIFT)  /* 0x0001FFFF */
/* RANDMZ_SEED17_11 */
#define NANDC_RANDMZ_SEED17_11_RANDMZ_SEED17_11_SHIFT      (0U)
#define NANDC_RANDMZ_SEED17_11_RANDMZ_SEED17_11_MASK       (0x1FFFFU << NANDC_RANDMZ_SEED17_11_RANDMZ_SEED17_11_SHIFT)  /* 0x0001FFFF */
/* RANDMZ_SEED17_12 */
#define NANDC_RANDMZ_SEED17_12_RANDMZ_SEED17_12_SHIFT      (0U)
#define NANDC_RANDMZ_SEED17_12_RANDMZ_SEED17_12_MASK       (0x1FFFFU << NANDC_RANDMZ_SEED17_12_RANDMZ_SEED17_12_SHIFT)  /* 0x0001FFFF */
/* RANDMZ_SEED17_13 */
#define NANDC_RANDMZ_SEED17_13_RANDMZ_SEED17_13_SHIFT      (0U)
#define NANDC_RANDMZ_SEED17_13_RANDMZ_SEED17_13_MASK       (0x1FFFFU << NANDC_RANDMZ_SEED17_13_RANDMZ_SEED17_13_SHIFT)  /* 0x0001FFFF */
/* RANDMZ_SEED17_14 */
#define NANDC_RANDMZ_SEED17_14_RANDMZ_SEED17_14_SHIFT      (0U)
#define NANDC_RANDMZ_SEED17_14_RANDMZ_SEED17_14_MASK       (0x1FFFFU << NANDC_RANDMZ_SEED17_14_RANDMZ_SEED17_14_SHIFT)  /* 0x0001FFFF */
/* RANDMZ_SEED17_15 */
#define NANDC_RANDMZ_SEED17_15_RANDMZ_SEED17_15_SHIFT      (0U)
#define NANDC_RANDMZ_SEED17_15_RANDMZ_SEED17_15_MASK       (0x1FFFFU << NANDC_RANDMZ_SEED17_15_RANDMZ_SEED17_15_SHIFT)  /* 0x0001FFFF */
/* RANDMZ_SEED19_0 */
#define NANDC_RANDMZ_SEED19_0_RANDMZ_SEED19_0_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_0_RANDMZ_SEED19_0_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_0_RANDMZ_SEED19_0_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_1 */
#define NANDC_RANDMZ_SEED19_1_RANDMZ_SEED19_1_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_1_RANDMZ_SEED19_1_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_1_RANDMZ_SEED19_1_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_2 */
#define NANDC_RANDMZ_SEED19_2_RANDMZ_SEED19_2_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_2_RANDMZ_SEED19_2_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_2_RANDMZ_SEED19_2_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_3 */
#define NANDC_RANDMZ_SEED19_3_RANDMZ_SEED19_3_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_3_RANDMZ_SEED19_3_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_3_RANDMZ_SEED19_3_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_4 */
#define NANDC_RANDMZ_SEED19_4_RANDMZ_SEED19_4_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_4_RANDMZ_SEED19_4_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_4_RANDMZ_SEED19_4_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_5 */
#define NANDC_RANDMZ_SEED19_5_RANDMZ_SEED19_5_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_5_RANDMZ_SEED19_5_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_5_RANDMZ_SEED19_5_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_6 */
#define NANDC_RANDMZ_SEED19_6_RANDMZ_SEED19_6_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_6_RANDMZ_SEED19_6_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_6_RANDMZ_SEED19_6_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_7 */
#define NANDC_RANDMZ_SEED19_7_RANDMZ_SEED19_7_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_7_RANDMZ_SEED19_7_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_7_RANDMZ_SEED19_7_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_8 */
#define NANDC_RANDMZ_SEED19_8_RANDMZ_SEED19_8_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_8_RANDMZ_SEED19_8_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_8_RANDMZ_SEED19_8_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_9 */
#define NANDC_RANDMZ_SEED19_9_RANDMZ_SEED19_9_SHIFT        (0U)
#define NANDC_RANDMZ_SEED19_9_RANDMZ_SEED19_9_MASK         (0x7FFFFU << NANDC_RANDMZ_SEED19_9_RANDMZ_SEED19_9_SHIFT)    /* 0x0007FFFF */
/* RANDMZ_SEED19_10 */
#define NANDC_RANDMZ_SEED19_10_RANDMZ_SEED19_10_SHIFT      (0U)
#define NANDC_RANDMZ_SEED19_10_RANDMZ_SEED19_10_MASK       (0x7FFFFU << NANDC_RANDMZ_SEED19_10_RANDMZ_SEED19_10_SHIFT)  /* 0x0007FFFF */
/* RANDMZ_SEED19_11 */
#define NANDC_RANDMZ_SEED19_11_RANDMZ_SEED19_11_SHIFT      (0U)
#define NANDC_RANDMZ_SEED19_11_RANDMZ_SEED19_11_MASK       (0x7FFFFU << NANDC_RANDMZ_SEED19_11_RANDMZ_SEED19_11_SHIFT)  /* 0x0007FFFF */
/* RANDMZ_SEED19_12 */
#define NANDC_RANDMZ_SEED19_12_RANDMZ_SEED19_12_SHIFT      (0U)
#define NANDC_RANDMZ_SEED19_12_RANDMZ_SEED19_12_MASK       (0x7FFFFU << NANDC_RANDMZ_SEED19_12_RANDMZ_SEED19_12_SHIFT)  /* 0x0007FFFF */
/* RANDMZ_SEED19_13 */
#define NANDC_RANDMZ_SEED19_13_RANDMZ_SEED19_13_SHIFT      (0U)
#define NANDC_RANDMZ_SEED19_13_RANDMZ_SEED19_13_MASK       (0x7FFFFU << NANDC_RANDMZ_SEED19_13_RANDMZ_SEED19_13_SHIFT)  /* 0x0007FFFF */
/* RANDMZ_SEED19_14 */
#define NANDC_RANDMZ_SEED19_14_RANDMZ_SEED19_14_SHIFT      (0U)
#define NANDC_RANDMZ_SEED19_14_RANDMZ_SEED19_14_MASK       (0x7FFFFU << NANDC_RANDMZ_SEED19_14_RANDMZ_SEED19_14_SHIFT)  /* 0x0007FFFF */
/* RANDMZ_SEED19_15 */
#define NANDC_RANDMZ_SEED19_15_RANDMZ_SEED19_15_SHIFT      (0U)
#define NANDC_RANDMZ_SEED19_15_RANDMZ_SEED19_15_MASK       (0x7FFFFU << NANDC_RANDMZ_SEED19_15_RANDMZ_SEED19_15_SHIFT)  /* 0x0007FFFF */
/* RANDMZ_SEED23_0 */
#define NANDC_RANDMZ_SEED23_0_RANDMZ_SEED23_0_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_0_RANDMZ_SEED23_0_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_0_RANDMZ_SEED23_0_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_1 */
#define NANDC_RANDMZ_SEED23_1_RANDMZ_SEED23_1_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_1_RANDMZ_SEED23_1_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_1_RANDMZ_SEED23_1_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_2 */
#define NANDC_RANDMZ_SEED23_2_RANDMZ_SEED23_2_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_2_RANDMZ_SEED23_2_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_2_RANDMZ_SEED23_2_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_3 */
#define NANDC_RANDMZ_SEED23_3_RANDMZ_SEED23_3_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_3_RANDMZ_SEED23_3_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_3_RANDMZ_SEED23_3_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_4 */
#define NANDC_RANDMZ_SEED23_4_RANDMZ_SEED23_4_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_4_RANDMZ_SEED23_4_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_4_RANDMZ_SEED23_4_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_5 */
#define NANDC_RANDMZ_SEED23_5_RANDMZ_SEED23_5_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_5_RANDMZ_SEED23_5_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_5_RANDMZ_SEED23_5_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_6 */
#define NANDC_RANDMZ_SEED23_6_RANDMZ_SEED23_6_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_6_RANDMZ_SEED23_6_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_6_RANDMZ_SEED23_6_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_7 */
#define NANDC_RANDMZ_SEED23_7_RANDMZ_SEED23_7_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_7_RANDMZ_SEED23_7_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_7_RANDMZ_SEED23_7_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_8 */
#define NANDC_RANDMZ_SEED23_8_RANDMZ_SEED23_8_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_8_RANDMZ_SEED23_8_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_8_RANDMZ_SEED23_8_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_9 */
#define NANDC_RANDMZ_SEED23_9_RANDMZ_SEED23_9_SHIFT        (0U)
#define NANDC_RANDMZ_SEED23_9_RANDMZ_SEED23_9_MASK         (0x7FFFFFU << NANDC_RANDMZ_SEED23_9_RANDMZ_SEED23_9_SHIFT)   /* 0x007FFFFF */
/* RANDMZ_SEED23_10 */
#define NANDC_RANDMZ_SEED23_10_RANDMZ_SEED23_10_SHIFT      (0U)
#define NANDC_RANDMZ_SEED23_10_RANDMZ_SEED23_10_MASK       (0x7FFFFFU << NANDC_RANDMZ_SEED23_10_RANDMZ_SEED23_10_SHIFT) /* 0x007FFFFF */
/* RANDMZ_SEED23_11 */
#define NANDC_RANDMZ_SEED23_11_RANDMZ_SEED23_11_SHIFT      (0U)
#define NANDC_RANDMZ_SEED23_11_RANDMZ_SEED23_11_MASK       (0x7FFFFFU << NANDC_RANDMZ_SEED23_11_RANDMZ_SEED23_11_SHIFT) /* 0x007FFFFF */
/* RANDMZ_SEED23_12 */
#define NANDC_RANDMZ_SEED23_12_RANDMZ_SEED23_12_SHIFT      (0U)
#define NANDC_RANDMZ_SEED23_12_RANDMZ_SEED23_12_MASK       (0x7FFFFFU << NANDC_RANDMZ_SEED23_12_RANDMZ_SEED23_12_SHIFT) /* 0x007FFFFF */
/* RANDMZ_SEED23_13 */
#define NANDC_RANDMZ_SEED23_13_RANDMZ_SEED23_13_SHIFT      (0U)
#define NANDC_RANDMZ_SEED23_13_RANDMZ_SEED23_13_MASK       (0x7FFFFFU << NANDC_RANDMZ_SEED23_13_RANDMZ_SEED23_13_SHIFT) /* 0x007FFFFF */
/* RANDMZ_SEED23_14 */
#define NANDC_RANDMZ_SEED23_14_RANDMZ_SEED23_14_SHIFT      (0U)
#define NANDC_RANDMZ_SEED23_14_RANDMZ_SEED23_14_MASK       (0x7FFFFFU << NANDC_RANDMZ_SEED23_14_RANDMZ_SEED23_14_SHIFT) /* 0x007FFFFF */
/* RANDMZ_SEED23_15 */
#define NANDC_RANDMZ_SEED23_15_RANDMZ_SEED23_15_SHIFT      (0U)
#define NANDC_RANDMZ_SEED23_15_RANDMZ_SEED23_15_MASK       (0x7FFFFFU << NANDC_RANDMZ_SEED23_15_RANDMZ_SEED23_15_SHIFT) /* 0x007FFFFF */
/* FLASH0_DATA */
#define NANDC_FLASH0_DATA_FLASH0_DATA_SHIFT                (0U)
#define NANDC_FLASH0_DATA_FLASH0_DATA_MASK                 (0xFFFFU << NANDC_FLASH0_DATA_FLASH0_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH0_ADDR */
#define NANDC_FLASH0_ADDR_FLASH0_ADDR_SHIFT                (0U)
#define NANDC_FLASH0_ADDR_FLASH0_ADDR_MASK                 (0xFFU << NANDC_FLASH0_ADDR_FLASH0_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH0_CMD */
#define NANDC_FLASH0_CMD_FLASH0_CMD_SHIFT                  (0U)
#define NANDC_FLASH0_CMD_FLASH0_CMD_MASK                   (0xFFU << NANDC_FLASH0_CMD_FLASH0_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH0_DATA_SYN */
#define NANDC_FLASH0_DATA_SYN_FLASH0_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH0_DATA_SYN_FLASH0_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH0_DATA_SYN_FLASH0_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH1_DATA */
#define NANDC_FLASH1_DATA_FLASH1_DATA_SHIFT                (0U)
#define NANDC_FLASH1_DATA_FLASH1_DATA_MASK                 (0xFFFFU << NANDC_FLASH1_DATA_FLASH1_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH1_ADDR */
#define NANDC_FLASH1_ADDR_FLASH1_ADDR_SHIFT                (0U)
#define NANDC_FLASH1_ADDR_FLASH1_ADDR_MASK                 (0xFFU << NANDC_FLASH1_ADDR_FLASH1_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH1_CMD */
#define NANDC_FLASH1_CMD_FLASH1_CMD_SHIFT                  (0U)
#define NANDC_FLASH1_CMD_FLASH1_CMD_MASK                   (0xFFU << NANDC_FLASH1_CMD_FLASH1_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH1_DATA_SYN */
#define NANDC_FLASH1_DATA_SYN_FLASH1_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH1_DATA_SYN_FLASH1_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH1_DATA_SYN_FLASH1_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH2_DATA */
#define NANDC_FLASH2_DATA_FLASH2_DATA_SHIFT                (0U)
#define NANDC_FLASH2_DATA_FLASH2_DATA_MASK                 (0xFFFFU << NANDC_FLASH2_DATA_FLASH2_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH2_ADDR */
#define NANDC_FLASH2_ADDR_FLASH2_ADDR_SHIFT                (0U)
#define NANDC_FLASH2_ADDR_FLASH2_ADDR_MASK                 (0xFFU << NANDC_FLASH2_ADDR_FLASH2_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH2_CMD */
#define NANDC_FLASH2_CMD_FLASH2_CMD_SHIFT                  (0U)
#define NANDC_FLASH2_CMD_FLASH2_CMD_MASK                   (0xFFU << NANDC_FLASH2_CMD_FLASH2_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH2_DATA_SYN */
#define NANDC_FLASH2_DATA_SYN_FLASH2_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH2_DATA_SYN_FLASH2_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH2_DATA_SYN_FLASH2_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH3_DATA */
#define NANDC_FLASH3_DATA_FLASH3_DATA_SHIFT                (0U)
#define NANDC_FLASH3_DATA_FLASH3_DATA_MASK                 (0xFFFFU << NANDC_FLASH3_DATA_FLASH3_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH3_ADDR */
#define NANDC_FLASH3_ADDR_FLASH3_ADDR_SHIFT                (0U)
#define NANDC_FLASH3_ADDR_FLASH3_ADDR_MASK                 (0xFFU << NANDC_FLASH3_ADDR_FLASH3_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH3_CMD */
#define NANDC_FLASH3_CMD_FLASH3_CMD_SHIFT                  (0U)
#define NANDC_FLASH3_CMD_FLASH3_CMD_MASK                   (0xFFU << NANDC_FLASH3_CMD_FLASH3_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH3_DATA_SYN */
#define NANDC_FLASH3_DATA_SYN_FLASH3_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH3_DATA_SYN_FLASH3_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH3_DATA_SYN_FLASH3_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH4_DATA */
#define NANDC_FLASH4_DATA_FLASH4_DATA_SHIFT                (0U)
#define NANDC_FLASH4_DATA_FLASH4_DATA_MASK                 (0xFFFFU << NANDC_FLASH4_DATA_FLASH4_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH4_ADDR */
#define NANDC_FLASH4_ADDR_FLASH4_ADDR_SHIFT                (0U)
#define NANDC_FLASH4_ADDR_FLASH4_ADDR_MASK                 (0xFFU << NANDC_FLASH4_ADDR_FLASH4_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH4_CMD */
#define NANDC_FLASH4_CMD_FLASH4_CMD_SHIFT                  (0U)
#define NANDC_FLASH4_CMD_FLASH4_CMD_MASK                   (0xFFU << NANDC_FLASH4_CMD_FLASH4_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH4_DATA_SYN */
#define NANDC_FLASH4_DATA_SYN_FLASH4_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH4_DATA_SYN_FLASH4_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH4_DATA_SYN_FLASH4_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH5_DATA */
#define NANDC_FLASH5_DATA_FLASH5_DATA_SHIFT                (0U)
#define NANDC_FLASH5_DATA_FLASH5_DATA_MASK                 (0xFFFFU << NANDC_FLASH5_DATA_FLASH5_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH5_ADDR */
#define NANDC_FLASH5_ADDR_FLASH5_ADDR_SHIFT                (0U)
#define NANDC_FLASH5_ADDR_FLASH5_ADDR_MASK                 (0xFFU << NANDC_FLASH5_ADDR_FLASH5_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH5_CMD */
#define NANDC_FLASH5_CMD_FLASH5_CMD_SHIFT                  (0U)
#define NANDC_FLASH5_CMD_FLASH5_CMD_MASK                   (0xFFU << NANDC_FLASH5_CMD_FLASH5_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH5_DATA_SYN */
#define NANDC_FLASH5_DATA_SYN_FLASH5_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH5_DATA_SYN_FLASH5_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH5_DATA_SYN_FLASH5_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH6_DATA */
#define NANDC_FLASH6_DATA_FLASH6_DATA_SHIFT                (0U)
#define NANDC_FLASH6_DATA_FLASH6_DATA_MASK                 (0xFFFFU << NANDC_FLASH6_DATA_FLASH6_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH6_ADDR */
#define NANDC_FLASH6_ADDR_FLASH6_ADDR_SHIFT                (0U)
#define NANDC_FLASH6_ADDR_FLASH6_ADDR_MASK                 (0xFFU << NANDC_FLASH6_ADDR_FLASH6_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH6_CMD */
#define NANDC_FLASH6_CMD_FLASH6_CMD_SHIFT                  (0U)
#define NANDC_FLASH6_CMD_FLASH6_CMD_MASK                   (0xFFU << NANDC_FLASH6_CMD_FLASH6_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH6_DATA_SYN */
#define NANDC_FLASH6_DATA_SYN_FLASH6_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH6_DATA_SYN_FLASH6_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH6_DATA_SYN_FLASH6_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/* FLASH7_DATA */
#define NANDC_FLASH7_DATA_FLASH7_DATA_SHIFT                (0U)
#define NANDC_FLASH7_DATA_FLASH7_DATA_MASK                 (0xFFFFU << NANDC_FLASH7_DATA_FLASH7_DATA_SHIFT)             /* 0x0000FFFF */
/* FLASH7_ADDR */
#define NANDC_FLASH7_ADDR_FLASH7_ADDR_SHIFT                (0U)
#define NANDC_FLASH7_ADDR_FLASH7_ADDR_MASK                 (0xFFU << NANDC_FLASH7_ADDR_FLASH7_ADDR_SHIFT)               /* 0x000000FF */
/* FLASH7_CMD */
#define NANDC_FLASH7_CMD_FLASH7_CMD_SHIFT                  (0U)
#define NANDC_FLASH7_CMD_FLASH7_CMD_MASK                   (0xFFU << NANDC_FLASH7_CMD_FLASH7_CMD_SHIFT)                 /* 0x000000FF */
/* FLASH7_DATA_SYN */
#define NANDC_FLASH7_DATA_SYN_FLASH7_DATA_SYN_SHIFT        (0U)
#define NANDC_FLASH7_DATA_SYN_FLASH7_DATA_SYN_MASK         (0xFFFFU << NANDC_FLASH7_DATA_SYN_FLASH7_DATA_SYN_SHIFT)     /* 0x0000FFFF */
/******************************************SFC*******************************************/
/* CTRL */
#define SFC_CTRL_SPIM_SHIFT                                (0U)
#define SFC_CTRL_SPIM_MASK                                 (0x1U << SFC_CTRL_SPIM_SHIFT)                                /* 0x00000001 */
#define SFC_CTRL_SHIFTPHASE_SHIFT                          (1U)
#define SFC_CTRL_SHIFTPHASE_MASK                           (0x1U << SFC_CTRL_SHIFTPHASE_SHIFT)                          /* 0x00000002 */
#define SFC_CTRL_IDLE_CYCLE_SHIFT                          (4U)
#define SFC_CTRL_IDLE_CYCLE_MASK                           (0xFU << SFC_CTRL_IDLE_CYCLE_SHIFT)                          /* 0x000000F0 */
#define SFC_CTRL_CMDB_SHIFT                                (8U)
#define SFC_CTRL_CMDB_MASK                                 (0x3U << SFC_CTRL_CMDB_SHIFT)                                /* 0x00000300 */
#define SFC_CTRL_ADRB_SHIFT                                (10U)
#define SFC_CTRL_ADRB_MASK                                 (0x3U << SFC_CTRL_ADRB_SHIFT)                                /* 0x00000C00 */
#define SFC_CTRL_DATB_SHIFT                                (12U)
#define SFC_CTRL_DATB_MASK                                 (0x3U << SFC_CTRL_DATB_SHIFT)                                /* 0x00003000 */
/* IMR */
#define SFC_IMR_RXFM_SHIFT                                 (0U)
#define SFC_IMR_RXFM_MASK                                  (0x1U << SFC_IMR_RXFM_SHIFT)                                 /* 0x00000001 */
#define SFC_IMR_RXUM_SHIFT                                 (1U)
#define SFC_IMR_RXUM_MASK                                  (0x1U << SFC_IMR_RXUM_SHIFT)                                 /* 0x00000002 */
#define SFC_IMR_TXOM_SHIFT                                 (2U)
#define SFC_IMR_TXOM_MASK                                  (0x1U << SFC_IMR_TXOM_SHIFT)                                 /* 0x00000004 */
#define SFC_IMR_TXEM_SHIFT                                 (3U)
#define SFC_IMR_TXEM_MASK                                  (0x1U << SFC_IMR_TXEM_SHIFT)                                 /* 0x00000008 */
#define SFC_IMR_TRANSM_SHIFT                               (4U)
#define SFC_IMR_TRANSM_MASK                                (0x1U << SFC_IMR_TRANSM_SHIFT)                               /* 0x00000010 */
#define SFC_IMR_AHBM_SHIFT                                 (5U)
#define SFC_IMR_AHBM_MASK                                  (0x1U << SFC_IMR_AHBM_SHIFT)                                 /* 0x00000020 */
#define SFC_IMR_NSPIM_SHIFT                                (6U)
#define SFC_IMR_NSPIM_MASK                                 (0x1U << SFC_IMR_NSPIM_SHIFT)                                /* 0x00000040 */
#define SFC_IMR_DMAM_SHIFT                                 (7U)
#define SFC_IMR_DMAM_MASK                                  (0x1U << SFC_IMR_DMAM_SHIFT)                                 /* 0x00000080 */
#define SFC_IMR_STPOLLM_SHIFT                              (8U)
#define SFC_IMR_STPOLLM_MASK                               (0x1U << SFC_IMR_STPOLLM_SHIFT)                              /* 0x00000100 */
/* ICLR */
#define SFC_ICLR_RXFC_SHIFT                                (0U)
#define SFC_ICLR_RXFC_MASK                                 (0x1U << SFC_ICLR_RXFC_SHIFT)                                /* 0x00000001 */
#define SFC_ICLR_RXUC_SHIFT                                (1U)
#define SFC_ICLR_RXUC_MASK                                 (0x1U << SFC_ICLR_RXUC_SHIFT)                                /* 0x00000002 */
#define SFC_ICLR_TXOC_SHIFT                                (2U)
#define SFC_ICLR_TXOC_MASK                                 (0x1U << SFC_ICLR_TXOC_SHIFT)                                /* 0x00000004 */
#define SFC_ICLR_TXEC_SHIFT                                (3U)
#define SFC_ICLR_TXEC_MASK                                 (0x1U << SFC_ICLR_TXEC_SHIFT)                                /* 0x00000008 */
#define SFC_ICLR_TRANSC_SHIFT                              (4U)
#define SFC_ICLR_TRANSC_MASK                               (0x1U << SFC_ICLR_TRANSC_SHIFT)                              /* 0x00000010 */
#define SFC_ICLR_AHBC_SHIFT                                (5U)
#define SFC_ICLR_AHBC_MASK                                 (0x1U << SFC_ICLR_AHBC_SHIFT)                                /* 0x00000020 */
#define SFC_ICLR_NSPIC_SHIFT                               (6U)
#define SFC_ICLR_NSPIC_MASK                                (0x1U << SFC_ICLR_NSPIC_SHIFT)                               /* 0x00000040 */
#define SFC_ICLR_DMAC_SHIFT                                (7U)
#define SFC_ICLR_DMAC_MASK                                 (0x1U << SFC_ICLR_DMAC_SHIFT)                                /* 0x00000080 */
#define SFC_ICLR_STPOLLC_SHIFT                             (8U)
#define SFC_ICLR_STPOLLC_MASK                              (0x1U << SFC_ICLR_STPOLLC_SHIFT)                             /* 0x00000100 */
/* FTLR */
#define SFC_FTLR_TXFTLR_SHIFT                              (0U)
#define SFC_FTLR_TXFTLR_MASK                               (0xFFU << SFC_FTLR_TXFTLR_SHIFT)                             /* 0x000000FF */
#define SFC_FTLR_RXFTLR_SHIFT                              (8U)
#define SFC_FTLR_RXFTLR_MASK                               (0xFFU << SFC_FTLR_RXFTLR_SHIFT)                             /* 0x0000FF00 */
/* RCVR */
#define SFC_RCVR_RCVR_SHIFT                                (0U)
#define SFC_RCVR_RCVR_MASK                                 (0x1U << SFC_RCVR_RCVR_SHIFT)                                /* 0x00000001 */
/* AX */
#define SFC_AX_AX_SHIFT                                    (0U)
#define SFC_AX_AX_MASK                                     (0xFFU << SFC_AX_AX_SHIFT)                                   /* 0x000000FF */
/* ABIT */
#define SFC_ABIT_ABIT_SHIFT                                (0U)
#define SFC_ABIT_ABIT_MASK                                 (0x1FU << SFC_ABIT_ABIT_SHIFT)                               /* 0x0000001F */
/* ISR */
#define SFC_ISR_RXFS_SHIFT                                 (0U)
#define SFC_ISR_RXFS_MASK                                  (0x1U << SFC_ISR_RXFS_SHIFT)                                 /* 0x00000001 */
#define SFC_ISR_RXUS_SHIFT                                 (1U)
#define SFC_ISR_RXUS_MASK                                  (0x1U << SFC_ISR_RXUS_SHIFT)                                 /* 0x00000002 */
#define SFC_ISR_TXOS_SHIFT                                 (2U)
#define SFC_ISR_TXOS_MASK                                  (0x1U << SFC_ISR_TXOS_SHIFT)                                 /* 0x00000004 */
#define SFC_ISR_TXES_SHIFT                                 (3U)
#define SFC_ISR_TXES_MASK                                  (0x1U << SFC_ISR_TXES_SHIFT)                                 /* 0x00000008 */
#define SFC_ISR_TRANSS_SHIFT                               (4U)
#define SFC_ISR_TRANSS_MASK                                (0x1U << SFC_ISR_TRANSS_SHIFT)                               /* 0x00000010 */
#define SFC_ISR_AHBS_SHIFT                                 (5U)
#define SFC_ISR_AHBS_MASK                                  (0x1U << SFC_ISR_AHBS_SHIFT)                                 /* 0x00000020 */
#define SFC_ISR_NSPIS_SHIFT                                (6U)
#define SFC_ISR_NSPIS_MASK                                 (0x1U << SFC_ISR_NSPIS_SHIFT)                                /* 0x00000040 */
#define SFC_ISR_DMAS_SHIFT                                 (7U)
#define SFC_ISR_DMAS_MASK                                  (0x1U << SFC_ISR_DMAS_SHIFT)                                 /* 0x00000080 */
#define SFC_ISR_STPOLLS_SHIFT                              (8U)
#define SFC_ISR_STPOLLS_MASK                               (0x1U << SFC_ISR_STPOLLS_SHIFT)                              /* 0x00000100 */
/* FSR */
#define SFC_FSR_TXFS_SHIFT                                 (0U)
#define SFC_FSR_TXFS_MASK                                  (0x1U << SFC_FSR_TXFS_SHIFT)                                 /* 0x00000001 */
#define SFC_FSR_TXES_SHIFT                                 (1U)
#define SFC_FSR_TXES_MASK                                  (0x1U << SFC_FSR_TXES_SHIFT)                                 /* 0x00000002 */
#define SFC_FSR_RXES_SHIFT                                 (2U)
#define SFC_FSR_RXES_MASK                                  (0x1U << SFC_FSR_RXES_SHIFT)                                 /* 0x00000004 */
#define SFC_FSR_RXFS_SHIFT                                 (3U)
#define SFC_FSR_RXFS_MASK                                  (0x1U << SFC_FSR_RXFS_SHIFT)                                 /* 0x00000008 */
#define SFC_FSR_TXWLVL_SHIFT                               (8U)
#define SFC_FSR_TXWLVL_MASK                                (0x1FU << SFC_FSR_TXWLVL_SHIFT)                              /* 0x00001F00 */
#define SFC_FSR_RXWLVL_SHIFT                               (16U)
#define SFC_FSR_RXWLVL_MASK                                (0x1FU << SFC_FSR_RXWLVL_SHIFT)                              /* 0x001F0000 */
/* SR */
#define SFC_SR_SR_SHIFT                                    (0U)
#define SFC_SR_SR_MASK                                     (0x1U << SFC_SR_SR_SHIFT)                                    /* 0x00000001 */
/* VER */
#define SFC_VER_VER_SHIFT                                  (0U)
#define SFC_VER_VER_MASK                                   (0xFFFFU << SFC_VER_VER_SHIFT)                               /* 0x0000FFFF */
/* DMAADDR */
#define SFC_DMAADDR_DMAADDR_SHIFT                          (0U)
#define SFC_DMAADDR_DMAADDR_MASK                           (0xFFFFFFFFU << SFC_DMAADDR_DMAADDR_SHIFT)                   /* 0xFFFFFFFF */
/* DMATR */
#define SFC_DMATR_DMATR_SHIFT                              (0U)
#define SFC_DMATR_DMATR_MASK                               (0x1U << SFC_DMATR_DMATR_SHIFT)                              /* 0x00000001 */
/* CMD */
#define SFC_CMD_CMD_SHIFT                                  (0U)
#define SFC_CMD_CMD_MASK                                   (0xFFU << SFC_CMD_CMD_SHIFT)                                 /* 0x000000FF */
#define SFC_CMD_DUMM_SHIFT                                 (8U)
#define SFC_CMD_DUMM_MASK                                  (0xFU << SFC_CMD_DUMM_SHIFT)                                 /* 0x00000F00 */
#define SFC_CMD_WR_SHIFT                                   (12U)
#define SFC_CMD_WR_MASK                                    (0x1U << SFC_CMD_WR_SHIFT)                                   /* 0x00001000 */
#define SFC_CMD_CONT_SHIFT                                 (13U)
#define SFC_CMD_CONT_MASK                                  (0x1U << SFC_CMD_CONT_SHIFT)                                 /* 0x00002000 */
#define SFC_CMD_ADDRB_SHIFT                                (14U)
#define SFC_CMD_ADDRB_MASK                                 (0x3U << SFC_CMD_ADDRB_SHIFT)                                /* 0x0000C000 */
#define SFC_CMD_TRB_SHIFT                                  (16U)
#define SFC_CMD_TRB_MASK                                   (0x3FFFU << SFC_CMD_TRB_SHIFT)                               /* 0x3FFF0000 */
#define SFC_CMD_CS_SHIFT                                   (30U)
#define SFC_CMD_CS_MASK                                    (0x3U << SFC_CMD_CS_SHIFT)                                   /* 0xC0000000 */
/* ADDR */
#define SFC_ADDR_ADDR_SHIFT                                (0U)
#define SFC_ADDR_ADDR_MASK                                 (0xFFFFFFFFU << SFC_ADDR_ADDR_SHIFT)                         /* 0xFFFFFFFF */
/* DATA */
#define SFC_DATA_DATA_SHIFT                                (0U)
#define SFC_DATA_DATA_MASK                                 (0xFFFFFFFFU << SFC_DATA_DATA_SHIFT)                         /* 0xFFFFFFFF */

#define SFC_CHIP_CNT                                       (1U)
/******************************************GMAC******************************************/
/* MAC_CONF */
#define GMAC_MAC_CONF_RE_SHIFT                             (2U)
#define GMAC_MAC_CONF_RE_MASK                              (0x1U << GMAC_MAC_CONF_RE_SHIFT)                             /* 0x00000004 */
#define GMAC_MAC_CONF_TE_SHIFT                             (3U)
#define GMAC_MAC_CONF_TE_MASK                              (0x1U << GMAC_MAC_CONF_TE_SHIFT)                             /* 0x00000008 */
#define GMAC_MAC_CONF_DC_SHIFT                             (4U)
#define GMAC_MAC_CONF_DC_MASK                              (0x1U << GMAC_MAC_CONF_DC_SHIFT)                             /* 0x00000010 */
#define GMAC_MAC_CONF_BL_SHIFT                             (5U)
#define GMAC_MAC_CONF_BL_MASK                              (0x3U << GMAC_MAC_CONF_BL_SHIFT)                             /* 0x00000060 */
#define GMAC_MAC_CONF_ACS_SHIFT                            (7U)
#define GMAC_MAC_CONF_ACS_MASK                             (0x1U << GMAC_MAC_CONF_ACS_SHIFT)                            /* 0x00000080 */
#define GMAC_MAC_CONF_LUD_SHIFT                            (8U)
#define GMAC_MAC_CONF_LUD_MASK                             (0x1U << GMAC_MAC_CONF_LUD_SHIFT)                            /* 0x00000100 */
#define GMAC_MAC_CONF_DR_SHIFT                             (9U)
#define GMAC_MAC_CONF_DR_MASK                              (0x1U << GMAC_MAC_CONF_DR_SHIFT)                             /* 0x00000200 */
#define GMAC_MAC_CONF_IPC_SHIFT                            (10U)
#define GMAC_MAC_CONF_IPC_MASK                             (0x1U << GMAC_MAC_CONF_IPC_SHIFT)                            /* 0x00000400 */
#define GMAC_MAC_CONF_DM_SHIFT                             (11U)
#define GMAC_MAC_CONF_DM_MASK                              (0x1U << GMAC_MAC_CONF_DM_SHIFT)                             /* 0x00000800 */
#define GMAC_MAC_CONF_LM_SHIFT                             (12U)
#define GMAC_MAC_CONF_LM_MASK                              (0x1U << GMAC_MAC_CONF_LM_SHIFT)                             /* 0x00001000 */
#define GMAC_MAC_CONF_DO_SHIFT                             (13U)
#define GMAC_MAC_CONF_DO_MASK                              (0x1U << GMAC_MAC_CONF_DO_SHIFT)                             /* 0x00002000 */
#define GMAC_MAC_CONF_FES_SHIFT                            (14U)
#define GMAC_MAC_CONF_FES_MASK                             (0x1U << GMAC_MAC_CONF_FES_SHIFT)                            /* 0x00004000 */
#define GMAC_MAC_CONF_PS_SHIFT                             (15U)
#define GMAC_MAC_CONF_PS_MASK                              (0x1U << GMAC_MAC_CONF_PS_SHIFT)                             /* 0x00008000 */
#define GMAC_MAC_CONF_DCRS_SHIFT                           (16U)
#define GMAC_MAC_CONF_DCRS_MASK                            (0x1U << GMAC_MAC_CONF_DCRS_SHIFT)                           /* 0x00010000 */
#define GMAC_MAC_CONF_IFG_SHIFT                            (17U)
#define GMAC_MAC_CONF_IFG_MASK                             (0x7U << GMAC_MAC_CONF_IFG_SHIFT)                            /* 0x000E0000 */
#define GMAC_MAC_CONF_BE_SHIFT                             (21U)
#define GMAC_MAC_CONF_BE_MASK                              (0x1U << GMAC_MAC_CONF_BE_SHIFT)                             /* 0x00200000 */
#define GMAC_MAC_CONF_JD_SHIFT                             (22U)
#define GMAC_MAC_CONF_JD_MASK                              (0x1U << GMAC_MAC_CONF_JD_SHIFT)                             /* 0x00400000 */
#define GMAC_MAC_CONF_WD_SHIFT                             (23U)
#define GMAC_MAC_CONF_WD_MASK                              (0x1U << GMAC_MAC_CONF_WD_SHIFT)                             /* 0x00800000 */
#define GMAC_MAC_CONF_TC_SHIFT                             (24U)
#define GMAC_MAC_CONF_TC_MASK                              (0x1U << GMAC_MAC_CONF_TC_SHIFT)                             /* 0x01000000 */
/* MAC_FRM_FILT */
#define GMAC_MAC_FRM_FILT_PR_SHIFT                         (0U)
#define GMAC_MAC_FRM_FILT_PR_MASK                          (0x1U << GMAC_MAC_FRM_FILT_PR_SHIFT)                         /* 0x00000001 */
#define GMAC_MAC_FRM_FILT_HUC_SHIFT                        (1U)
#define GMAC_MAC_FRM_FILT_HUC_MASK                         (0x1U << GMAC_MAC_FRM_FILT_HUC_SHIFT)                        /* 0x00000002 */
#define GMAC_MAC_FRM_FILT_HMC_SHIFT                        (2U)
#define GMAC_MAC_FRM_FILT_HMC_MASK                         (0x1U << GMAC_MAC_FRM_FILT_HMC_SHIFT)                        /* 0x00000004 */
#define GMAC_MAC_FRM_FILT_DAIF_SHIFT                       (3U)
#define GMAC_MAC_FRM_FILT_DAIF_MASK                        (0x1U << GMAC_MAC_FRM_FILT_DAIF_SHIFT)                       /* 0x00000008 */
#define GMAC_MAC_FRM_FILT_PM_SHIFT                         (4U)
#define GMAC_MAC_FRM_FILT_PM_MASK                          (0x1U << GMAC_MAC_FRM_FILT_PM_SHIFT)                         /* 0x00000010 */
#define GMAC_MAC_FRM_FILT_DBF_SHIFT                        (5U)
#define GMAC_MAC_FRM_FILT_DBF_MASK                         (0x1U << GMAC_MAC_FRM_FILT_DBF_SHIFT)                        /* 0x00000020 */
#define GMAC_MAC_FRM_FILT_PCF_SHIFT                        (6U)
#define GMAC_MAC_FRM_FILT_PCF_MASK                         (0x3U << GMAC_MAC_FRM_FILT_PCF_SHIFT)                        /* 0x000000C0 */
#define GMAC_MAC_FRM_FILT_SAIF_SHIFT                       (8U)
#define GMAC_MAC_FRM_FILT_SAIF_MASK                        (0x1U << GMAC_MAC_FRM_FILT_SAIF_SHIFT)                       /* 0x00000100 */
#define GMAC_MAC_FRM_FILT_SAF_SHIFT                        (9U)
#define GMAC_MAC_FRM_FILT_SAF_MASK                         (0x1U << GMAC_MAC_FRM_FILT_SAF_SHIFT)                        /* 0x00000200 */
#define GMAC_MAC_FRM_FILT_HPF_SHIFT                        (10U)
#define GMAC_MAC_FRM_FILT_HPF_MASK                         (0x1U << GMAC_MAC_FRM_FILT_HPF_SHIFT)                        /* 0x00000400 */
#define GMAC_MAC_FRM_FILT_RA_SHIFT                         (31U)
#define GMAC_MAC_FRM_FILT_RA_MASK                          (0x1U << GMAC_MAC_FRM_FILT_RA_SHIFT)                         /* 0x80000000 */
/* HASH_TAB_HI */
#define GMAC_HASH_TAB_HI_HTH_SHIFT                         (0U)
#define GMAC_HASH_TAB_HI_HTH_MASK                          (0xFFFFFFFFU << GMAC_HASH_TAB_HI_HTH_SHIFT)                  /* 0xFFFFFFFF */
/* HASH_TAB_LO */
#define GMAC_HASH_TAB_LO_HTL_SHIFT                         (0U)
#define GMAC_HASH_TAB_LO_HTL_MASK                          (0xFFFFFFFFU << GMAC_HASH_TAB_LO_HTL_SHIFT)                  /* 0xFFFFFFFF */
/* GMII_ADDR */
#define GMAC_GMII_ADDR_GB_SHIFT                            (0U)
#define GMAC_GMII_ADDR_GB_MASK                             (0x1U << GMAC_GMII_ADDR_GB_SHIFT)                            /* 0x00000001 */
#define GMAC_GMII_ADDR_GW_SHIFT                            (1U)
#define GMAC_GMII_ADDR_GW_MASK                             (0x1U << GMAC_GMII_ADDR_GW_SHIFT)                            /* 0x00000002 */
#define GMAC_GMII_ADDR_CR_SHIFT                            (2U)
#define GMAC_GMII_ADDR_CR_MASK                             (0xFU << GMAC_GMII_ADDR_CR_SHIFT)                            /* 0x0000003C */
#define GMAC_GMII_ADDR_GR_SHIFT                            (6U)
#define GMAC_GMII_ADDR_GR_MASK                             (0x1FU << GMAC_GMII_ADDR_GR_SHIFT)                           /* 0x000007C0 */
#define GMAC_GMII_ADDR_PA_SHIFT                            (11U)
#define GMAC_GMII_ADDR_PA_MASK                             (0x1FU << GMAC_GMII_ADDR_PA_SHIFT)                           /* 0x0000F800 */
/* GMII_DATA */
#define GMAC_GMII_DATA_GD_SHIFT                            (0U)
#define GMAC_GMII_DATA_GD_MASK                             (0xFFFFU << GMAC_GMII_DATA_GD_SHIFT)                         /* 0x0000FFFF */
/* FLOW_CTRL */
#define GMAC_FLOW_CTRL_FCB_BPA_SHIFT                       (0U)
#define GMAC_FLOW_CTRL_FCB_BPA_MASK                        (0x1U << GMAC_FLOW_CTRL_FCB_BPA_SHIFT)                       /* 0x00000001 */
#define GMAC_FLOW_CTRL_TFE_SHIFT                           (1U)
#define GMAC_FLOW_CTRL_TFE_MASK                            (0x1U << GMAC_FLOW_CTRL_TFE_SHIFT)                           /* 0x00000002 */
#define GMAC_FLOW_CTRL_RFE_SHIFT                           (2U)
#define GMAC_FLOW_CTRL_RFE_MASK                            (0x1U << GMAC_FLOW_CTRL_RFE_SHIFT)                           /* 0x00000004 */
#define GMAC_FLOW_CTRL_UP_SHIFT                            (3U)
#define GMAC_FLOW_CTRL_UP_MASK                             (0x1U << GMAC_FLOW_CTRL_UP_SHIFT)                            /* 0x00000008 */
#define GMAC_FLOW_CTRL_PLT_SHIFT                           (4U)
#define GMAC_FLOW_CTRL_PLT_MASK                            (0x3U << GMAC_FLOW_CTRL_PLT_SHIFT)                           /* 0x00000030 */
#define GMAC_FLOW_CTRL_DZPQ_SHIFT                          (7U)
#define GMAC_FLOW_CTRL_DZPQ_MASK                           (0x1U << GMAC_FLOW_CTRL_DZPQ_SHIFT)                          /* 0x00000080 */
#define GMAC_FLOW_CTRL_PT_SHIFT                            (16U)
#define GMAC_FLOW_CTRL_PT_MASK                             (0xFFFFU << GMAC_FLOW_CTRL_PT_SHIFT)                         /* 0xFFFF0000 */
/* VLAN_TAG */
#define GMAC_VLAN_TAG_VL_SHIFT                             (0U)
#define GMAC_VLAN_TAG_VL_MASK                              (0xFFFFU << GMAC_VLAN_TAG_VL_SHIFT)                          /* 0x0000FFFF */
#define GMAC_VLAN_TAG_ETV_SHIFT                            (16U)
#define GMAC_VLAN_TAG_ETV_MASK                             (0x1U << GMAC_VLAN_TAG_ETV_SHIFT)                            /* 0x00010000 */
/* DEBUG */
#define GMAC_DEBUG_RDB_SHIFT                               (0U)
#define GMAC_DEBUG_RDB_MASK                                (0x1U << GMAC_DEBUG_RDB_SHIFT)                               /* 0x00000001 */
#define GMAC_DEBUG_ACT_SHIFT                               (1U)
#define GMAC_DEBUG_ACT_MASK                                (0x3U << GMAC_DEBUG_ACT_SHIFT)                               /* 0x00000006 */
#define GMAC_DEBUG_RFIFOWR_SHIFT                           (4U)
#define GMAC_DEBUG_RFIFOWR_MASK                            (0x1U << GMAC_DEBUG_RFIFOWR_SHIFT)                           /* 0x00000010 */
#define GMAC_DEBUG_RFIFORD_SHIFT                           (5U)
#define GMAC_DEBUG_RFIFORD_MASK                            (0x3U << GMAC_DEBUG_RFIFORD_SHIFT)                           /* 0x00000060 */
#define GMAC_DEBUG_RFIFO_SHIFT                             (8U)
#define GMAC_DEBUG_RFIFO_MASK                              (0x3U << GMAC_DEBUG_RFIFO_SHIFT)                             /* 0x00000300 */
#define GMAC_DEBUG_TACT_SHIFT                              (16U)
#define GMAC_DEBUG_TACT_MASK                               (0x1U << GMAC_DEBUG_TACT_SHIFT)                              /* 0x00010000 */
#define GMAC_DEBUG_TSAT_SHIFT                              (17U)
#define GMAC_DEBUG_TSAT_MASK                               (0x3U << GMAC_DEBUG_TSAT_SHIFT)                              /* 0x00060000 */
#define GMAC_DEBUG_PAUSE_SHIFT                             (19U)
#define GMAC_DEBUG_PAUSE_MASK                              (0x1U << GMAC_DEBUG_PAUSE_SHIFT)                             /* 0x00080000 */
#define GMAC_DEBUG_TFIFOSTA_SHIFT                          (20U)
#define GMAC_DEBUG_TFIFOSTA_MASK                           (0x3U << GMAC_DEBUG_TFIFOSTA_SHIFT)                          /* 0x00300000 */
#define GMAC_DEBUG_TFIFO1_SHIFT                            (22U)
#define GMAC_DEBUG_TFIFO1_MASK                             (0x1U << GMAC_DEBUG_TFIFO1_SHIFT)                            /* 0x00400000 */
#define GMAC_DEBUG_TFIFO2_SHIFT                            (24U)
#define GMAC_DEBUG_TFIFO2_MASK                             (0x1U << GMAC_DEBUG_TFIFO2_SHIFT)                            /* 0x01000000 */
#define GMAC_DEBUG_TFIFO3_SHIFT                            (25U)
#define GMAC_DEBUG_TFIFO3_MASK                             (0x1U << GMAC_DEBUG_TFIFO3_SHIFT)                            /* 0x02000000 */
/* PMT_CTRL_STA */
#define GMAC_PMT_CTRL_STA_PD_SHIFT                         (0U)
#define GMAC_PMT_CTRL_STA_PD_MASK                          (0x1U << GMAC_PMT_CTRL_STA_PD_SHIFT)                         /* 0x00000001 */
#define GMAC_PMT_CTRL_STA_MPE_SHIFT                        (1U)
#define GMAC_PMT_CTRL_STA_MPE_MASK                         (0x1U << GMAC_PMT_CTRL_STA_MPE_SHIFT)                        /* 0x00000002 */
#define GMAC_PMT_CTRL_STA_WFE_SHIFT                        (2U)
#define GMAC_PMT_CTRL_STA_WFE_MASK                         (0x1U << GMAC_PMT_CTRL_STA_WFE_SHIFT)                        /* 0x00000004 */
#define GMAC_PMT_CTRL_STA_MPR_SHIFT                        (5U)
#define GMAC_PMT_CTRL_STA_MPR_MASK                         (0x1U << GMAC_PMT_CTRL_STA_MPR_SHIFT)                        /* 0x00000020 */
#define GMAC_PMT_CTRL_STA_WFR_SHIFT                        (6U)
#define GMAC_PMT_CTRL_STA_WFR_MASK                         (0x1U << GMAC_PMT_CTRL_STA_WFR_SHIFT)                        /* 0x00000040 */
#define GMAC_PMT_CTRL_STA_GU_SHIFT                         (9U)
#define GMAC_PMT_CTRL_STA_GU_MASK                          (0x1U << GMAC_PMT_CTRL_STA_GU_SHIFT)                         /* 0x00000200 */
#define GMAC_PMT_CTRL_STA_WFFRPR_SHIFT                     (31U)
#define GMAC_PMT_CTRL_STA_WFFRPR_MASK                      (0x1U << GMAC_PMT_CTRL_STA_WFFRPR_SHIFT)                     /* 0x80000000 */
/* INT_STATUS */
#define GMAC_INT_STATUS_RIS_SHIFT                          (0U)
#define GMAC_INT_STATUS_RIS_MASK                           (0x1U << GMAC_INT_STATUS_RIS_SHIFT)                          /* 0x00000001 */
#define GMAC_INT_STATUS_PIS_SHIFT                          (3U)
#define GMAC_INT_STATUS_PIS_MASK                           (0x1U << GMAC_INT_STATUS_PIS_SHIFT)                          /* 0x00000008 */
#define GMAC_INT_STATUS_MIS_SHIFT                          (4U)
#define GMAC_INT_STATUS_MIS_MASK                           (0x1U << GMAC_INT_STATUS_MIS_SHIFT)                          /* 0x00000010 */
#define GMAC_INT_STATUS_MRIS_SHIFT                         (5U)
#define GMAC_INT_STATUS_MRIS_MASK                          (0x1U << GMAC_INT_STATUS_MRIS_SHIFT)                         /* 0x00000020 */
#define GMAC_INT_STATUS_MTIS_SHIFT                         (6U)
#define GMAC_INT_STATUS_MTIS_MASK                          (0x1U << GMAC_INT_STATUS_MTIS_SHIFT)                         /* 0x00000040 */
#define GMAC_INT_STATUS_MRCOIS_SHIFT                       (7U)
#define GMAC_INT_STATUS_MRCOIS_MASK                        (0x1U << GMAC_INT_STATUS_MRCOIS_SHIFT)                       /* 0x00000080 */
/* INT_MASK */
#define GMAC_INT_MASK_RIM_SHIFT                            (0U)
#define GMAC_INT_MASK_RIM_MASK                             (0x1U << GMAC_INT_MASK_RIM_SHIFT)                            /* 0x00000001 */
#define GMAC_INT_MASK_PIM_SHIFT                            (3U)
#define GMAC_INT_MASK_PIM_MASK                             (0x1U << GMAC_INT_MASK_PIM_SHIFT)                            /* 0x00000008 */
/* MAC_ADDR0_HI */
#define GMAC_MAC_ADDR0_HI_A47_A32_SHIFT                    (0U)
#define GMAC_MAC_ADDR0_HI_A47_A32_MASK                     (0xFFFFU << GMAC_MAC_ADDR0_HI_A47_A32_SHIFT)                 /* 0x0000FFFF */
/* MAC_ADDR0_LO */
#define GMAC_MAC_ADDR0_LO_A31_A0_SHIFT                     (0U)
#define GMAC_MAC_ADDR0_LO_A31_A0_MASK                      (0xFFFFFFFFU << GMAC_MAC_ADDR0_LO_A31_A0_SHIFT)              /* 0xFFFFFFFF */
/* AN_CTRL */
#define GMAC_AN_CTRL_RAN_SHIFT                             (9U)
#define GMAC_AN_CTRL_RAN_MASK                              (0x1U << GMAC_AN_CTRL_RAN_SHIFT)                             /* 0x00000200 */
#define GMAC_AN_CTRL_ANE_SHIFT                             (12U)
#define GMAC_AN_CTRL_ANE_MASK                              (0x1U << GMAC_AN_CTRL_ANE_SHIFT)                             /* 0x00001000 */
/* AN_STATUS */
#define GMAC_AN_STATUS_LS_SHIFT                            (2U)
#define GMAC_AN_STATUS_LS_MASK                             (0x1U << GMAC_AN_STATUS_LS_SHIFT)                            /* 0x00000004 */
#define GMAC_AN_STATUS_ANA_SHIFT                           (3U)
#define GMAC_AN_STATUS_ANA_MASK                            (0x1U << GMAC_AN_STATUS_ANA_SHIFT)                           /* 0x00000008 */
#define GMAC_AN_STATUS_ANC_SHIFT                           (5U)
#define GMAC_AN_STATUS_ANC_MASK                            (0x1U << GMAC_AN_STATUS_ANC_SHIFT)                           /* 0x00000020 */
/* AN_ADV */
#define GMAC_AN_ADV_FD_SHIFT                               (5U)
#define GMAC_AN_ADV_FD_MASK                                (0x1U << GMAC_AN_ADV_FD_SHIFT)                               /* 0x00000020 */
#define GMAC_AN_ADV_HD_SHIFT                               (6U)
#define GMAC_AN_ADV_HD_MASK                                (0x1U << GMAC_AN_ADV_HD_SHIFT)                               /* 0x00000040 */
#define GMAC_AN_ADV_PSE_SHIFT                              (7U)
#define GMAC_AN_ADV_PSE_MASK                               (0x3U << GMAC_AN_ADV_PSE_SHIFT)                              /* 0x00000180 */
#define GMAC_AN_ADV_RFE_SHIFT                              (12U)
#define GMAC_AN_ADV_RFE_MASK                               (0x3U << GMAC_AN_ADV_RFE_SHIFT)                              /* 0x00003000 */
#define GMAC_AN_ADV_NP_SHIFT                               (15U)
#define GMAC_AN_ADV_NP_MASK                                (0x1U << GMAC_AN_ADV_NP_SHIFT)                               /* 0x00008000 */
/* AN_LINK_PART_AB */
#define GMAC_AN_LINK_PART_AB_FD_SHIFT                      (5U)
#define GMAC_AN_LINK_PART_AB_FD_MASK                       (0x1U << GMAC_AN_LINK_PART_AB_FD_SHIFT)                      /* 0x00000020 */
#define GMAC_AN_LINK_PART_AB_HD_SHIFT                      (6U)
#define GMAC_AN_LINK_PART_AB_HD_MASK                       (0x1U << GMAC_AN_LINK_PART_AB_HD_SHIFT)                      /* 0x00000040 */
#define GMAC_AN_LINK_PART_AB_PSE_SHIFT                     (7U)
#define GMAC_AN_LINK_PART_AB_PSE_MASK                      (0x3U << GMAC_AN_LINK_PART_AB_PSE_SHIFT)                     /* 0x00000180 */
#define GMAC_AN_LINK_PART_AB_RFE_SHIFT                     (12U)
#define GMAC_AN_LINK_PART_AB_RFE_MASK                      (0x3U << GMAC_AN_LINK_PART_AB_RFE_SHIFT)                     /* 0x00003000 */
#define GMAC_AN_LINK_PART_AB_ACK_SHIFT                     (14U)
#define GMAC_AN_LINK_PART_AB_ACK_MASK                      (0x1U << GMAC_AN_LINK_PART_AB_ACK_SHIFT)                     /* 0x00004000 */
#define GMAC_AN_LINK_PART_AB_NP_SHIFT                      (15U)
#define GMAC_AN_LINK_PART_AB_NP_MASK                       (0x1U << GMAC_AN_LINK_PART_AB_NP_SHIFT)                      /* 0x00008000 */
/* AN_EXP */
#define GMAC_AN_EXP_NPR_SHIFT                              (1U)
#define GMAC_AN_EXP_NPR_MASK                               (0x1U << GMAC_AN_EXP_NPR_SHIFT)                              /* 0x00000002 */
#define GMAC_AN_EXP_NPA_SHIFT                              (2U)
#define GMAC_AN_EXP_NPA_MASK                               (0x1U << GMAC_AN_EXP_NPA_SHIFT)                              /* 0x00000004 */
/* INTF_MODE_STA */
#define GMAC_INTF_MODE_STA_LM_SHIFT                        (0U)
#define GMAC_INTF_MODE_STA_LM_MASK                         (0x1U << GMAC_INTF_MODE_STA_LM_SHIFT)                        /* 0x00000001 */
#define GMAC_INTF_MODE_STA_LSD_SHIFT                       (1U)
#define GMAC_INTF_MODE_STA_LSD_MASK                        (0x3U << GMAC_INTF_MODE_STA_LSD_SHIFT)                       /* 0x00000006 */
#define GMAC_INTF_MODE_STA_LST_SHIFT                       (3U)
#define GMAC_INTF_MODE_STA_LST_MASK                        (0x1U << GMAC_INTF_MODE_STA_LST_SHIFT)                       /* 0x00000008 */
/* MMC_CTRL */
#define GMAC_MMC_CTRL_CR_SHIFT                             (0U)
#define GMAC_MMC_CTRL_CR_MASK                              (0x1U << GMAC_MMC_CTRL_CR_SHIFT)                             /* 0x00000001 */
#define GMAC_MMC_CTRL_CSR_SHIFT                            (1U)
#define GMAC_MMC_CTRL_CSR_MASK                             (0x1U << GMAC_MMC_CTRL_CSR_SHIFT)                            /* 0x00000002 */
#define GMAC_MMC_CTRL_ROR_SHIFT                            (2U)
#define GMAC_MMC_CTRL_ROR_MASK                             (0x1U << GMAC_MMC_CTRL_ROR_SHIFT)                            /* 0x00000004 */
#define GMAC_MMC_CTRL_MCF_SHIFT                            (3U)
#define GMAC_MMC_CTRL_MCF_MASK                             (0x1U << GMAC_MMC_CTRL_MCF_SHIFT)                            /* 0x00000008 */
#define GMAC_MMC_CTRL_CP_SHIFT                             (4U)
#define GMAC_MMC_CTRL_CP_MASK                              (0x1U << GMAC_MMC_CTRL_CP_SHIFT)                             /* 0x00000010 */
#define GMAC_MMC_CTRL_FHP_SHIFT                            (5U)
#define GMAC_MMC_CTRL_FHP_MASK                             (0x1U << GMAC_MMC_CTRL_FHP_SHIFT)                            /* 0x00000020 */
/* MMC_RX_INTR */
#define GMAC_MMC_RX_INTR_INT0_SHIFT                        (0U)
#define GMAC_MMC_RX_INTR_INT0_MASK                         (0x1U << GMAC_MMC_RX_INTR_INT0_SHIFT)                        /* 0x00000001 */
#define GMAC_MMC_RX_INTR_INT1_SHIFT                        (1U)
#define GMAC_MMC_RX_INTR_INT1_MASK                         (0x1U << GMAC_MMC_RX_INTR_INT1_SHIFT)                        /* 0x00000002 */
#define GMAC_MMC_RX_INTR_INT2_SHIFT                        (2U)
#define GMAC_MMC_RX_INTR_INT2_MASK                         (0x1U << GMAC_MMC_RX_INTR_INT2_SHIFT)                        /* 0x00000004 */
#define GMAC_MMC_RX_INTR_INT4_SHIFT                        (4U)
#define GMAC_MMC_RX_INTR_INT4_MASK                         (0x1U << GMAC_MMC_RX_INTR_INT4_SHIFT)                        /* 0x00000010 */
#define GMAC_MMC_RX_INTR_INT5_SHIFT                        (5U)
#define GMAC_MMC_RX_INTR_INT5_MASK                         (0x1U << GMAC_MMC_RX_INTR_INT5_SHIFT)                        /* 0x00000020 */
#define GMAC_MMC_RX_INTR_INT18_SHIFT                       (18U)
#define GMAC_MMC_RX_INTR_INT18_MASK                        (0x1U << GMAC_MMC_RX_INTR_INT18_SHIFT)                       /* 0x00040000 */
#define GMAC_MMC_RX_INTR_INT21_SHIFT                       (21U)
#define GMAC_MMC_RX_INTR_INT21_MASK                        (0x1U << GMAC_MMC_RX_INTR_INT21_SHIFT)                       /* 0x00200000 */
/* MMC_TX_INTR */
#define GMAC_MMC_TX_INTR_INT0_SHIFT                        (0U)
#define GMAC_MMC_TX_INTR_INT0_MASK                         (0x1U << GMAC_MMC_TX_INTR_INT0_SHIFT)                        /* 0x00000001 */
#define GMAC_MMC_TX_INTR_INT1_SHIFT                        (1U)
#define GMAC_MMC_TX_INTR_INT1_MASK                         (0x1U << GMAC_MMC_TX_INTR_INT1_SHIFT)                        /* 0x00000002 */
#define GMAC_MMC_TX_INTR_INT13_SHIFT                       (13U)
#define GMAC_MMC_TX_INTR_INT13_MASK                        (0x1U << GMAC_MMC_TX_INTR_INT13_SHIFT)                       /* 0x00002000 */
#define GMAC_MMC_TX_INTR_INT19_SHIFT                       (19U)
#define GMAC_MMC_TX_INTR_INT19_MASK                        (0x1U << GMAC_MMC_TX_INTR_INT19_SHIFT)                       /* 0x00080000 */
#define GMAC_MMC_TX_INTR_INT20_SHIFT                       (20U)
#define GMAC_MMC_TX_INTR_INT20_MASK                        (0x1U << GMAC_MMC_TX_INTR_INT20_SHIFT)                       /* 0x00100000 */
#define GMAC_MMC_TX_INTR_INT21_SHIFT                       (21U)
#define GMAC_MMC_TX_INTR_INT21_MASK                        (0x1U << GMAC_MMC_TX_INTR_INT21_SHIFT)                       /* 0x00200000 */
/* MMC_RX_INT_MSK */
#define GMAC_MMC_RX_INT_MSK_INT0_SHIFT                     (0U)
#define GMAC_MMC_RX_INT_MSK_INT0_MASK                      (0x1U << GMAC_MMC_RX_INT_MSK_INT0_SHIFT)                     /* 0x00000001 */
#define GMAC_MMC_RX_INT_MSK_INT1_SHIFT                     (1U)
#define GMAC_MMC_RX_INT_MSK_INT1_MASK                      (0x1U << GMAC_MMC_RX_INT_MSK_INT1_SHIFT)                     /* 0x00000002 */
#define GMAC_MMC_RX_INT_MSK_INT2_SHIFT                     (2U)
#define GMAC_MMC_RX_INT_MSK_INT2_MASK                      (0x1U << GMAC_MMC_RX_INT_MSK_INT2_SHIFT)                     /* 0x00000004 */
#define GMAC_MMC_RX_INT_MSK_INT4_SHIFT                     (4U)
#define GMAC_MMC_RX_INT_MSK_INT4_MASK                      (0x1U << GMAC_MMC_RX_INT_MSK_INT4_SHIFT)                     /* 0x00000010 */
#define GMAC_MMC_RX_INT_MSK_INT5_SHIFT                     (5U)
#define GMAC_MMC_RX_INT_MSK_INT5_MASK                      (0x1U << GMAC_MMC_RX_INT_MSK_INT5_SHIFT)                     /* 0x00000020 */
#define GMAC_MMC_RX_INT_MSK_INT18_SHIFT                    (18U)
#define GMAC_MMC_RX_INT_MSK_INT18_MASK                     (0x1U << GMAC_MMC_RX_INT_MSK_INT18_SHIFT)                    /* 0x00040000 */
#define GMAC_MMC_RX_INT_MSK_INT21_SHIFT                    (21U)
#define GMAC_MMC_RX_INT_MSK_INT21_MASK                     (0x1U << GMAC_MMC_RX_INT_MSK_INT21_SHIFT)                    /* 0x00200000 */
/* MMC_TX_INT_MSK */
#define GMAC_MMC_TX_INT_MSK_INT0_SHIFT                     (0U)
#define GMAC_MMC_TX_INT_MSK_INT0_MASK                      (0x1U << GMAC_MMC_TX_INT_MSK_INT0_SHIFT)                     /* 0x00000001 */
#define GMAC_MMC_TX_INT_MSK_INT1_SHIFT                     (1U)
#define GMAC_MMC_TX_INT_MSK_INT1_MASK                      (0x1U << GMAC_MMC_TX_INT_MSK_INT1_SHIFT)                     /* 0x00000002 */
#define GMAC_MMC_TX_INT_MSK_INT13_SHIFT                    (13U)
#define GMAC_MMC_TX_INT_MSK_INT13_MASK                     (0x1U << GMAC_MMC_TX_INT_MSK_INT13_SHIFT)                    /* 0x00002000 */
#define GMAC_MMC_TX_INT_MSK_INT19_SHIFT                    (19U)
#define GMAC_MMC_TX_INT_MSK_INT19_MASK                     (0x1U << GMAC_MMC_TX_INT_MSK_INT19_SHIFT)                    /* 0x00080000 */
#define GMAC_MMC_TX_INT_MSK_INT20_SHIFT                    (20U)
#define GMAC_MMC_TX_INT_MSK_INT20_MASK                     (0x1U << GMAC_MMC_TX_INT_MSK_INT20_SHIFT)                    /* 0x00100000 */
#define GMAC_MMC_TX_INT_MSK_INT21_SHIFT                    (21U)
#define GMAC_MMC_TX_INT_MSK_INT21_MASK                     (0x1U << GMAC_MMC_TX_INT_MSK_INT21_SHIFT)                    /* 0x00200000 */
/* MMC_TXOCTETCNT_GB */
#define GMAC_MMC_TXOCTETCNT_GB_TXOCTETCOUNT_GB_SHIFT       (0U)
#define GMAC_MMC_TXOCTETCNT_GB_TXOCTETCOUNT_GB_MASK        (0xFFFFFFFFU << GMAC_MMC_TXOCTETCNT_GB_TXOCTETCOUNT_GB_SHIFT) /* 0xFFFFFFFF */
/* MMC_TXFRMCNT_GB */
#define GMAC_MMC_TXFRMCNT_GB_TXFRAMECOUNT_GB_SHIFT         (0U)
#define GMAC_MMC_TXFRMCNT_GB_TXFRAMECOUNT_GB_MASK          (0xFFFFFFFFU << GMAC_MMC_TXFRMCNT_GB_TXFRAMECOUNT_GB_SHIFT)  /* 0xFFFFFFFF */
/* MMC_TXUNDFLWERR */
#define GMAC_MMC_TXUNDFLWERR_TXUNDERFLOWERROR_SHIFT        (0U)
#define GMAC_MMC_TXUNDFLWERR_TXUNDERFLOWERROR_MASK         (0xFFFFFFFFU << GMAC_MMC_TXUNDFLWERR_TXUNDERFLOWERROR_SHIFT) /* 0xFFFFFFFF */
/* MMC_TXCARERR */
#define GMAC_MMC_TXCARERR_TXCARRIERERROR_SHIFT             (0U)
#define GMAC_MMC_TXCARERR_TXCARRIERERROR_MASK              (0xFFFFFFFFU << GMAC_MMC_TXCARERR_TXCARRIERERROR_SHIFT)      /* 0xFFFFFFFF */
/* MMC_TXOCTETCNT_G */
#define GMAC_MMC_TXOCTETCNT_G_TXOCTETCOUNT_G_SHIFT         (0U)
#define GMAC_MMC_TXOCTETCNT_G_TXOCTETCOUNT_G_MASK          (0xFFFFFFFFU << GMAC_MMC_TXOCTETCNT_G_TXOCTETCOUNT_G_SHIFT)  /* 0xFFFFFFFF */
/* MMC_TXFRMCNT_G */
#define GMAC_MMC_TXFRMCNT_G_TXFRAMECOUNT_G_SHIFT           (0U)
#define GMAC_MMC_TXFRMCNT_G_TXFRAMECOUNT_G_MASK            (0xFFFFFFFFU << GMAC_MMC_TXFRMCNT_G_TXFRAMECOUNT_G_SHIFT)    /* 0xFFFFFFFF */
/* MMC_RXFRMCNT_GB */
#define GMAC_MMC_RXFRMCNT_GB_RXFRAMECOUNT_GB_SHIFT         (0U)
#define GMAC_MMC_RXFRMCNT_GB_RXFRAMECOUNT_GB_MASK          (0xFFFFFFFFU << GMAC_MMC_RXFRMCNT_GB_RXFRAMECOUNT_GB_SHIFT)  /* 0xFFFFFFFF */
/* MMC_RXOCTETCNT_GB */
#define GMAC_MMC_RXOCTETCNT_GB_RXOCTETCOUNT_GB_SHIFT       (0U)
#define GMAC_MMC_RXOCTETCNT_GB_RXOCTETCOUNT_GB_MASK        (0xFFFFFFFFU << GMAC_MMC_RXOCTETCNT_GB_RXOCTETCOUNT_GB_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXOCTETCNT_G */
#define GMAC_MMC_RXOCTETCNT_G_RXOCTETCOUNT_G_SHIFT         (0U)
#define GMAC_MMC_RXOCTETCNT_G_RXOCTETCOUNT_G_MASK          (0xFFFFFFFFU << GMAC_MMC_RXOCTETCNT_G_RXOCTETCOUNT_G_SHIFT)  /* 0xFFFFFFFF */
/* MMC_RXMCFRMCNT_G */
#define GMAC_MMC_RXMCFRMCNT_G_RXMULTICASTFRAMES_G_SHIFT    (0U)
#define GMAC_MMC_RXMCFRMCNT_G_RXMULTICASTFRAMES_G_MASK     (0xFFFFFFFFU << GMAC_MMC_RXMCFRMCNT_G_RXMULTICASTFRAMES_G_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXCRCERR */
#define GMAC_MMC_RXCRCERR_RXCRCERROR_SHIFT                 (0U)
#define GMAC_MMC_RXCRCERR_RXCRCERROR_MASK                  (0xFFFFFFFFU << GMAC_MMC_RXCRCERR_RXCRCERROR_SHIFT)          /* 0xFFFFFFFF */
/* MMC_RXLENERR */
#define GMAC_MMC_RXLENERR_RXLENGTHERROR_SHIFT              (0U)
#define GMAC_MMC_RXLENERR_RXLENGTHERROR_MASK               (0xFFFFFFFFU << GMAC_MMC_RXLENERR_RXLENGTHERROR_SHIFT)       /* 0xFFFFFFFF */
/* MMC_RXFIFOOVRFLW */
#define GMAC_MMC_RXFIFOOVRFLW_RXFIFOOVERFLOW_SHIFT         (0U)
#define GMAC_MMC_RXFIFOOVRFLW_RXFIFOOVERFLOW_MASK          (0xFFFFFFFFU << GMAC_MMC_RXFIFOOVRFLW_RXFIFOOVERFLOW_SHIFT)  /* 0xFFFFFFFF */
/* MMC_IPC_INT_MSK */
#define GMAC_MMC_IPC_INT_MSK_INT0_SHIFT                    (0U)
#define GMAC_MMC_IPC_INT_MSK_INT0_MASK                     (0x1U << GMAC_MMC_IPC_INT_MSK_INT0_SHIFT)                    /* 0x00000001 */
#define GMAC_MMC_IPC_INT_MSK_INT1_SHIFT                    (1U)
#define GMAC_MMC_IPC_INT_MSK_INT1_MASK                     (0x1U << GMAC_MMC_IPC_INT_MSK_INT1_SHIFT)                    /* 0x00000002 */
#define GMAC_MMC_IPC_INT_MSK_INT5_SHIFT                    (5U)
#define GMAC_MMC_IPC_INT_MSK_INT5_MASK                     (0x1U << GMAC_MMC_IPC_INT_MSK_INT5_SHIFT)                    /* 0x00000020 */
#define GMAC_MMC_IPC_INT_MSK_INT6_SHIFT                    (6U)
#define GMAC_MMC_IPC_INT_MSK_INT6_MASK                     (0x1U << GMAC_MMC_IPC_INT_MSK_INT6_SHIFT)                    /* 0x00000040 */
#define GMAC_MMC_IPC_INT_MSK_INT9_SHIFT                    (9U)
#define GMAC_MMC_IPC_INT_MSK_INT9_MASK                     (0x1U << GMAC_MMC_IPC_INT_MSK_INT9_SHIFT)                    /* 0x00000200 */
#define GMAC_MMC_IPC_INT_MSK_INT11_SHIFT                   (11U)
#define GMAC_MMC_IPC_INT_MSK_INT11_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT11_SHIFT)                   /* 0x00000800 */
#define GMAC_MMC_IPC_INT_MSK_INT13_SHIFT                   (13U)
#define GMAC_MMC_IPC_INT_MSK_INT13_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT13_SHIFT)                   /* 0x00002000 */
#define GMAC_MMC_IPC_INT_MSK_INT17_SHIFT                   (17U)
#define GMAC_MMC_IPC_INT_MSK_INT17_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT17_SHIFT)                   /* 0x00020000 */
#define GMAC_MMC_IPC_INT_MSK_INT22_SHIFT                   (22U)
#define GMAC_MMC_IPC_INT_MSK_INT22_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT22_SHIFT)                   /* 0x00400000 */
#define GMAC_MMC_IPC_INT_MSK_INT25_SHIFT                   (25U)
#define GMAC_MMC_IPC_INT_MSK_INT25_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT25_SHIFT)                   /* 0x02000000 */
#define GMAC_MMC_IPC_INT_MSK_INT27_SHIFT                   (27U)
#define GMAC_MMC_IPC_INT_MSK_INT27_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT27_SHIFT)                   /* 0x08000000 */
#define GMAC_MMC_IPC_INT_MSK_INT29_SHIFT                   (29U)
#define GMAC_MMC_IPC_INT_MSK_INT29_MASK                    (0x1U << GMAC_MMC_IPC_INT_MSK_INT29_SHIFT)                   /* 0x20000000 */
/* MMC_IPC_INTR */
#define GMAC_MMC_IPC_INTR_INT0_SHIFT                       (0U)
#define GMAC_MMC_IPC_INTR_INT0_MASK                        (0x1U << GMAC_MMC_IPC_INTR_INT0_SHIFT)                       /* 0x00000001 */
#define GMAC_MMC_IPC_INTR_INT1_SHIFT                       (1U)
#define GMAC_MMC_IPC_INTR_INT1_MASK                        (0x1U << GMAC_MMC_IPC_INTR_INT1_SHIFT)                       /* 0x00000002 */
#define GMAC_MMC_IPC_INTR_INT5_SHIFT                       (5U)
#define GMAC_MMC_IPC_INTR_INT5_MASK                        (0x1U << GMAC_MMC_IPC_INTR_INT5_SHIFT)                       /* 0x00000020 */
#define GMAC_MMC_IPC_INTR_INT6_SHIFT                       (6U)
#define GMAC_MMC_IPC_INTR_INT6_MASK                        (0x1U << GMAC_MMC_IPC_INTR_INT6_SHIFT)                       /* 0x00000040 */
#define GMAC_MMC_IPC_INTR_INT9_SHIFT                       (9U)
#define GMAC_MMC_IPC_INTR_INT9_MASK                        (0x1U << GMAC_MMC_IPC_INTR_INT9_SHIFT)                       /* 0x00000200 */
#define GMAC_MMC_IPC_INTR_INT11_SHIFT                      (11U)
#define GMAC_MMC_IPC_INTR_INT11_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT11_SHIFT)                      /* 0x00000800 */
#define GMAC_MMC_IPC_INTR_INT13_SHIFT                      (13U)
#define GMAC_MMC_IPC_INTR_INT13_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT13_SHIFT)                      /* 0x00002000 */
#define GMAC_MMC_IPC_INTR_INT17_SHIFT                      (17U)
#define GMAC_MMC_IPC_INTR_INT17_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT17_SHIFT)                      /* 0x00020000 */
#define GMAC_MMC_IPC_INTR_INT22_SHIFT                      (22U)
#define GMAC_MMC_IPC_INTR_INT22_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT22_SHIFT)                      /* 0x00400000 */
#define GMAC_MMC_IPC_INTR_INT25_SHIFT                      (25U)
#define GMAC_MMC_IPC_INTR_INT25_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT25_SHIFT)                      /* 0x02000000 */
#define GMAC_MMC_IPC_INTR_INT27_SHIFT                      (27U)
#define GMAC_MMC_IPC_INTR_INT27_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT27_SHIFT)                      /* 0x08000000 */
#define GMAC_MMC_IPC_INTR_INT29_SHIFT                      (29U)
#define GMAC_MMC_IPC_INTR_INT29_MASK                       (0x1U << GMAC_MMC_IPC_INTR_INT29_SHIFT)                      /* 0x20000000 */
/* MMC_RXIPV4GFRM */
#define GMAC_MMC_RXIPV4GFRM_RXIPV4_GD_FRMS_SHIFT           (0U)
#define GMAC_MMC_RXIPV4GFRM_RXIPV4_GD_FRMS_MASK            (0xFFFFFFFFU << GMAC_MMC_RXIPV4GFRM_RXIPV4_GD_FRMS_SHIFT)    /* 0xFFFFFFFF */
/* MMC_RXIPV4HDERRFRM */
#define GMAC_MMC_RXIPV4HDERRFRM_RXIPV4_HDRERR_FRMS_SHIFT   (0U)
#define GMAC_MMC_RXIPV4HDERRFRM_RXIPV4_HDRERR_FRMS_MASK    (0xFFFFFFFFU << GMAC_MMC_RXIPV4HDERRFRM_RXIPV4_HDRERR_FRMS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXIPV6GFRM */
#define GMAC_MMC_RXIPV6GFRM_RXIPV6_GD_FRMS_SHIFT           (0U)
#define GMAC_MMC_RXIPV6GFRM_RXIPV6_GD_FRMS_MASK            (0xFFFFFFFFU << GMAC_MMC_RXIPV6GFRM_RXIPV6_GD_FRMS_SHIFT)    /* 0xFFFFFFFF */
/* MMC_RXIPV6HDERRFRM */
#define GMAC_MMC_RXIPV6HDERRFRM_RXIPV6_HDRERR_FRMS_SHIFT   (0U)
#define GMAC_MMC_RXIPV6HDERRFRM_RXIPV6_HDRERR_FRMS_MASK    (0xFFFFFFFFU << GMAC_MMC_RXIPV6HDERRFRM_RXIPV6_HDRERR_FRMS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXUDPERRFRM */
#define GMAC_MMC_RXUDPERRFRM_RXUDP_ERR_FRMS_SHIFT          (0U)
#define GMAC_MMC_RXUDPERRFRM_RXUDP_ERR_FRMS_MASK           (0xFFFFFFFFU << GMAC_MMC_RXUDPERRFRM_RXUDP_ERR_FRMS_SHIFT)   /* 0xFFFFFFFF */
/* MMC_RXTCPERRFRM */
#define GMAC_MMC_RXTCPERRFRM_RXTCP_ERR_FRMS_SHIFT          (0U)
#define GMAC_MMC_RXTCPERRFRM_RXTCP_ERR_FRMS_MASK           (0xFFFFFFFFU << GMAC_MMC_RXTCPERRFRM_RXTCP_ERR_FRMS_SHIFT)   /* 0xFFFFFFFF */
/* MMC_RXICMPERRFRM */
#define GMAC_MMC_RXICMPERRFRM_RXICMP_ERR_FRMS_SHIFT        (0U)
#define GMAC_MMC_RXICMPERRFRM_RXICMP_ERR_FRMS_MASK         (0xFFFFFFFFU << GMAC_MMC_RXICMPERRFRM_RXICMP_ERR_FRMS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXIPV4HDERROCT */
#define GMAC_MMC_RXIPV4HDERROCT_RXIPV4_HDRERR_OCTETS_SHIFT (0U)
#define GMAC_MMC_RXIPV4HDERROCT_RXIPV4_HDRERR_OCTETS_MASK  (0xFFFFFFFFU << GMAC_MMC_RXIPV4HDERROCT_RXIPV4_HDRERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXIPV6HDERROCT */
#define GMAC_MMC_RXIPV6HDERROCT_RXIPV6_HDRERR_OCTETS_SHIFT (0U)
#define GMAC_MMC_RXIPV6HDERROCT_RXIPV6_HDRERR_OCTETS_MASK  (0xFFFFFFFFU << GMAC_MMC_RXIPV6HDERROCT_RXIPV6_HDRERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXUDPERROCT */
#define GMAC_MMC_RXUDPERROCT_RXUDP_ERR_OCTETS_SHIFT        (0U)
#define GMAC_MMC_RXUDPERROCT_RXUDP_ERR_OCTETS_MASK         (0xFFFFFFFFU << GMAC_MMC_RXUDPERROCT_RXUDP_ERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXTCPERROCT */
#define GMAC_MMC_RXTCPERROCT_RXTCP_ERR_OCTETS_SHIFT        (0U)
#define GMAC_MMC_RXTCPERROCT_RXTCP_ERR_OCTETS_MASK         (0xFFFFFFFFU << GMAC_MMC_RXTCPERROCT_RXTCP_ERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXICMPERROCT */
#define GMAC_MMC_RXICMPERROCT_RXICMP_ERR_OCTETS_SHIFT      (0U)
#define GMAC_MMC_RXICMPERROCT_RXICMP_ERR_OCTETS_MASK       (0xFFFFFFFFU << GMAC_MMC_RXICMPERROCT_RXICMP_ERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* BUS_MODE */
#define GMAC_BUS_MODE_SWR_SHIFT                            (0U)
#define GMAC_BUS_MODE_SWR_MASK                             (0x1U << GMAC_BUS_MODE_SWR_SHIFT)                            /* 0x00000001 */
#define GMAC_BUS_MODE_DSL_SHIFT                            (2U)
#define GMAC_BUS_MODE_DSL_MASK                             (0x1FU << GMAC_BUS_MODE_DSL_SHIFT)                           /* 0x0000007C */
#define GMAC_BUS_MODE_PBL_SHIFT                            (8U)
#define GMAC_BUS_MODE_PBL_MASK                             (0x3FU << GMAC_BUS_MODE_PBL_SHIFT)                           /* 0x00003F00 */
#define GMAC_BUS_MODE_FB_SHIFT                             (16U)
#define GMAC_BUS_MODE_FB_MASK                              (0x1U << GMAC_BUS_MODE_FB_SHIFT)                             /* 0x00010000 */
#define GMAC_BUS_MODE_RPBL_SHIFT                           (17U)
#define GMAC_BUS_MODE_RPBL_MASK                            (0x3FU << GMAC_BUS_MODE_RPBL_SHIFT)                          /* 0x007E0000 */
#define GMAC_BUS_MODE_USP_SHIFT                            (23U)
#define GMAC_BUS_MODE_USP_MASK                             (0x1U << GMAC_BUS_MODE_USP_SHIFT)                            /* 0x00800000 */
#define GMAC_BUS_MODE_PBL_MODE_SHIFT                       (24U)
#define GMAC_BUS_MODE_PBL_MODE_MASK                        (0x1U << GMAC_BUS_MODE_PBL_MODE_SHIFT)                       /* 0x01000000 */
#define GMAC_BUS_MODE_AAL_SHIFT                            (25U)
#define GMAC_BUS_MODE_AAL_MASK                             (0x1U << GMAC_BUS_MODE_AAL_SHIFT)                            /* 0x02000000 */
/* TX_POLL_DEMAND */
#define GMAC_TX_POLL_DEMAND_TPD_SHIFT                      (0U)
#define GMAC_TX_POLL_DEMAND_TPD_MASK                       (0xFFFFFFFFU << GMAC_TX_POLL_DEMAND_TPD_SHIFT)               /* 0xFFFFFFFF */
/* RX_POLL_DEMAND */
#define GMAC_RX_POLL_DEMAND_RPD_SHIFT                      (0U)
#define GMAC_RX_POLL_DEMAND_RPD_MASK                       (0xFFFFFFFFU << GMAC_RX_POLL_DEMAND_RPD_SHIFT)               /* 0xFFFFFFFF */
/* RX_DESC_LIST_ADDR */
#define GMAC_RX_DESC_LIST_ADDR_SRL_SHIFT                   (0U)
#define GMAC_RX_DESC_LIST_ADDR_SRL_MASK                    (0xFFFFFFFFU << GMAC_RX_DESC_LIST_ADDR_SRL_SHIFT)            /* 0xFFFFFFFF */
/* TX_DESC_LIST_ADDR */
#define GMAC_TX_DESC_LIST_ADDR_STL_SHIFT                   (0U)
#define GMAC_TX_DESC_LIST_ADDR_STL_MASK                    (0xFFFFFFFFU << GMAC_TX_DESC_LIST_ADDR_STL_SHIFT)            /* 0xFFFFFFFF */
/* STATUS */
#define GMAC_STATUS_TI_SHIFT                               (0U)
#define GMAC_STATUS_TI_MASK                                (0x1U << GMAC_STATUS_TI_SHIFT)                               /* 0x00000001 */
#define GMAC_STATUS_TPS_SHIFT                              (1U)
#define GMAC_STATUS_TPS_MASK                               (0x1U << GMAC_STATUS_TPS_SHIFT)                              /* 0x00000002 */
#define GMAC_STATUS_TU_SHIFT                               (2U)
#define GMAC_STATUS_TU_MASK                                (0x1U << GMAC_STATUS_TU_SHIFT)                               /* 0x00000004 */
#define GMAC_STATUS_TJT_SHIFT                              (3U)
#define GMAC_STATUS_TJT_MASK                               (0x1U << GMAC_STATUS_TJT_SHIFT)                              /* 0x00000008 */
#define GMAC_STATUS_OVF_SHIFT                              (4U)
#define GMAC_STATUS_OVF_MASK                               (0x1U << GMAC_STATUS_OVF_SHIFT)                              /* 0x00000010 */
#define GMAC_STATUS_UNF_SHIFT                              (5U)
#define GMAC_STATUS_UNF_MASK                               (0x1U << GMAC_STATUS_UNF_SHIFT)                              /* 0x00000020 */
#define GMAC_STATUS_RI_SHIFT                               (6U)
#define GMAC_STATUS_RI_MASK                                (0x1U << GMAC_STATUS_RI_SHIFT)                               /* 0x00000040 */
#define GMAC_STATUS_RU_SHIFT                               (7U)
#define GMAC_STATUS_RU_MASK                                (0x1U << GMAC_STATUS_RU_SHIFT)                               /* 0x00000080 */
#define GMAC_STATUS_RPS_SHIFT                              (8U)
#define GMAC_STATUS_RPS_MASK                               (0x1U << GMAC_STATUS_RPS_SHIFT)                              /* 0x00000100 */
#define GMAC_STATUS_RWT_SHIFT                              (9U)
#define GMAC_STATUS_RWT_MASK                               (0x1U << GMAC_STATUS_RWT_SHIFT)                              /* 0x00000200 */
#define GMAC_STATUS_ETI_SHIFT                              (10U)
#define GMAC_STATUS_ETI_MASK                               (0x1U << GMAC_STATUS_ETI_SHIFT)                              /* 0x00000400 */
#define GMAC_STATUS_FBI_SHIFT                              (13U)
#define GMAC_STATUS_FBI_MASK                               (0x1U << GMAC_STATUS_FBI_SHIFT)                              /* 0x00002000 */
#define GMAC_STATUS_ERI_SHIFT                              (14U)
#define GMAC_STATUS_ERI_MASK                               (0x1U << GMAC_STATUS_ERI_SHIFT)                              /* 0x00004000 */
#define GMAC_STATUS_AIS_SHIFT                              (15U)
#define GMAC_STATUS_AIS_MASK                               (0x1U << GMAC_STATUS_AIS_SHIFT)                              /* 0x00008000 */
#define GMAC_STATUS_NIS_SHIFT                              (16U)
#define GMAC_STATUS_NIS_MASK                               (0x1U << GMAC_STATUS_NIS_SHIFT)                              /* 0x00010000 */
#define GMAC_STATUS_RS_SHIFT                               (17U)
#define GMAC_STATUS_RS_MASK                                (0x7U << GMAC_STATUS_RS_SHIFT)                               /* 0x000E0000 */
#define GMAC_STATUS_TS_SHIFT                               (20U)
#define GMAC_STATUS_TS_MASK                                (0x7U << GMAC_STATUS_TS_SHIFT)                               /* 0x00700000 */
#define GMAC_STATUS_EB_SHIFT                               (23U)
#define GMAC_STATUS_EB_MASK                                (0x7U << GMAC_STATUS_EB_SHIFT)                               /* 0x03800000 */
#define GMAC_STATUS_GLI_SHIFT                              (26U)
#define GMAC_STATUS_GLI_MASK                               (0x1U << GMAC_STATUS_GLI_SHIFT)                              /* 0x04000000 */
#define GMAC_STATUS_GMI_SHIFT                              (27U)
#define GMAC_STATUS_GMI_MASK                               (0x1U << GMAC_STATUS_GMI_SHIFT)                              /* 0x08000000 */
#define GMAC_STATUS_GPI_SHIFT                              (28U)
#define GMAC_STATUS_GPI_MASK                               (0x1U << GMAC_STATUS_GPI_SHIFT)                              /* 0x10000000 */
/* OP_MODE */
#define GMAC_OP_MODE_SR_SHIFT                              (1U)
#define GMAC_OP_MODE_SR_MASK                               (0x1U << GMAC_OP_MODE_SR_SHIFT)                              /* 0x00000002 */
#define GMAC_OP_MODE_OSF_SHIFT                             (2U)
#define GMAC_OP_MODE_OSF_MASK                              (0x1U << GMAC_OP_MODE_OSF_SHIFT)                             /* 0x00000004 */
#define GMAC_OP_MODE_RTC_SHIFT                             (3U)
#define GMAC_OP_MODE_RTC_MASK                              (0x3U << GMAC_OP_MODE_RTC_SHIFT)                             /* 0x00000018 */
#define GMAC_OP_MODE_FUF_SHIFT                             (6U)
#define GMAC_OP_MODE_FUF_MASK                              (0x1U << GMAC_OP_MODE_FUF_SHIFT)                             /* 0x00000040 */
#define GMAC_OP_MODE_FEF_SHIFT                             (7U)
#define GMAC_OP_MODE_FEF_MASK                              (0x1U << GMAC_OP_MODE_FEF_SHIFT)                             /* 0x00000080 */
#define GMAC_OP_MODE_EFC_SHIFT                             (8U)
#define GMAC_OP_MODE_EFC_MASK                              (0x1U << GMAC_OP_MODE_EFC_SHIFT)                             /* 0x00000100 */
#define GMAC_OP_MODE_RFA_SHIFT                             (9U)
#define GMAC_OP_MODE_RFA_MASK                              (0x3U << GMAC_OP_MODE_RFA_SHIFT)                             /* 0x00000600 */
#define GMAC_OP_MODE_RFD_SHIFT                             (11U)
#define GMAC_OP_MODE_RFD_MASK                              (0x3U << GMAC_OP_MODE_RFD_SHIFT)                             /* 0x00001800 */
#define GMAC_OP_MODE_ST_SHIFT                              (13U)
#define GMAC_OP_MODE_ST_MASK                               (0x1U << GMAC_OP_MODE_ST_SHIFT)                              /* 0x00002000 */
#define GMAC_OP_MODE_TTC_SHIFT                             (14U)
#define GMAC_OP_MODE_TTC_MASK                              (0x7U << GMAC_OP_MODE_TTC_SHIFT)                             /* 0x0001C000 */
#define GMAC_OP_MODE_FTF_SHIFT                             (20U)
#define GMAC_OP_MODE_FTF_MASK                              (0x1U << GMAC_OP_MODE_FTF_SHIFT)                             /* 0x00100000 */
#define GMAC_OP_MODE_TSF_SHIFT                             (21U)
#define GMAC_OP_MODE_TSF_MASK                              (0x1U << GMAC_OP_MODE_TSF_SHIFT)                             /* 0x00200000 */
#define GMAC_OP_MODE_DFF_SHIFT                             (24U)
#define GMAC_OP_MODE_DFF_MASK                              (0x1U << GMAC_OP_MODE_DFF_SHIFT)                             /* 0x01000000 */
#define GMAC_OP_MODE_RSF_SHIFT                             (25U)
#define GMAC_OP_MODE_RSF_MASK                              (0x1U << GMAC_OP_MODE_RSF_SHIFT)                             /* 0x02000000 */
#define GMAC_OP_MODE_DT_SHIFT                              (26U)
#define GMAC_OP_MODE_DT_MASK                               (0x1U << GMAC_OP_MODE_DT_SHIFT)                              /* 0x04000000 */
/* INT_ENA */
#define GMAC_INT_ENA_TIE_SHIFT                             (0U)
#define GMAC_INT_ENA_TIE_MASK                              (0x1U << GMAC_INT_ENA_TIE_SHIFT)                             /* 0x00000001 */
#define GMAC_INT_ENA_TSE_SHIFT                             (1U)
#define GMAC_INT_ENA_TSE_MASK                              (0x1U << GMAC_INT_ENA_TSE_SHIFT)                             /* 0x00000002 */
#define GMAC_INT_ENA_TUE_SHIFT                             (2U)
#define GMAC_INT_ENA_TUE_MASK                              (0x1U << GMAC_INT_ENA_TUE_SHIFT)                             /* 0x00000004 */
#define GMAC_INT_ENA_TJE_SHIFT                             (3U)
#define GMAC_INT_ENA_TJE_MASK                              (0x1U << GMAC_INT_ENA_TJE_SHIFT)                             /* 0x00000008 */
#define GMAC_INT_ENA_OVE_SHIFT                             (4U)
#define GMAC_INT_ENA_OVE_MASK                              (0x1U << GMAC_INT_ENA_OVE_SHIFT)                             /* 0x00000010 */
#define GMAC_INT_ENA_UNE_SHIFT                             (5U)
#define GMAC_INT_ENA_UNE_MASK                              (0x1U << GMAC_INT_ENA_UNE_SHIFT)                             /* 0x00000020 */
#define GMAC_INT_ENA_RIE_SHIFT                             (6U)
#define GMAC_INT_ENA_RIE_MASK                              (0x1U << GMAC_INT_ENA_RIE_SHIFT)                             /* 0x00000040 */
#define GMAC_INT_ENA_RUE_SHIFT                             (7U)
#define GMAC_INT_ENA_RUE_MASK                              (0x1U << GMAC_INT_ENA_RUE_SHIFT)                             /* 0x00000080 */
#define GMAC_INT_ENA_RSE_SHIFT                             (8U)
#define GMAC_INT_ENA_RSE_MASK                              (0x1U << GMAC_INT_ENA_RSE_SHIFT)                             /* 0x00000100 */
#define GMAC_INT_ENA_RWE_SHIFT                             (9U)
#define GMAC_INT_ENA_RWE_MASK                              (0x1U << GMAC_INT_ENA_RWE_SHIFT)                             /* 0x00000200 */
#define GMAC_INT_ENA_ETE_SHIFT                             (10U)
#define GMAC_INT_ENA_ETE_MASK                              (0x1U << GMAC_INT_ENA_ETE_SHIFT)                             /* 0x00000400 */
#define GMAC_INT_ENA_FBE_SHIFT                             (13U)
#define GMAC_INT_ENA_FBE_MASK                              (0x1U << GMAC_INT_ENA_FBE_SHIFT)                             /* 0x00002000 */
#define GMAC_INT_ENA_ERE_SHIFT                             (14U)
#define GMAC_INT_ENA_ERE_MASK                              (0x1U << GMAC_INT_ENA_ERE_SHIFT)                             /* 0x00004000 */
#define GMAC_INT_ENA_AIE_SHIFT                             (15U)
#define GMAC_INT_ENA_AIE_MASK                              (0x1U << GMAC_INT_ENA_AIE_SHIFT)                             /* 0x00008000 */
#define GMAC_INT_ENA_NIE_SHIFT                             (16U)
#define GMAC_INT_ENA_NIE_MASK                              (0x1U << GMAC_INT_ENA_NIE_SHIFT)                             /* 0x00010000 */
/* OVERFLOW_CNT */
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_2_SHIFT        (0U)
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_2_MASK         (0xFFFFU << GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_2_SHIFT)     /* 0x0000FFFF */
#define GMAC_OVERFLOW_CNT_MISS_FRAME_OVERFLOW_BIT_SHIFT    (16U)
#define GMAC_OVERFLOW_CNT_MISS_FRAME_OVERFLOW_BIT_MASK     (0x1U << GMAC_OVERFLOW_CNT_MISS_FRAME_OVERFLOW_BIT_SHIFT)    /* 0x00010000 */
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_SHIFT          (17U)
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_MASK           (0x7FFU << GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_SHIFT)        /* 0x0FFE0000 */
#define GMAC_OVERFLOW_CNT_FIFO_OVERFLOW_BIT_SHIFT          (28U)
#define GMAC_OVERFLOW_CNT_FIFO_OVERFLOW_BIT_MASK           (0x1U << GMAC_OVERFLOW_CNT_FIFO_OVERFLOW_BIT_SHIFT)          /* 0x10000000 */
/* REC_INT_WDT_TIMER */
#define GMAC_REC_INT_WDT_TIMER_RIWT_SHIFT                  (0U)
#define GMAC_REC_INT_WDT_TIMER_RIWT_MASK                   (0xFFU << GMAC_REC_INT_WDT_TIMER_RIWT_SHIFT)                 /* 0x000000FF */
/* AXI_BUS_MODE */
#define GMAC_AXI_BUS_MODE_UNDEF_SHIFT                      (0U)
#define GMAC_AXI_BUS_MODE_UNDEF_MASK                       (0x1U << GMAC_AXI_BUS_MODE_UNDEF_SHIFT)                      /* 0x00000001 */
#define GMAC_AXI_BUS_MODE_BLEN4_SHIFT                      (1U)
#define GMAC_AXI_BUS_MODE_BLEN4_MASK                       (0x1U << GMAC_AXI_BUS_MODE_BLEN4_SHIFT)                      /* 0x00000002 */
#define GMAC_AXI_BUS_MODE_BLEN8_SHIFT                      (2U)
#define GMAC_AXI_BUS_MODE_BLEN8_MASK                       (0x1U << GMAC_AXI_BUS_MODE_BLEN8_SHIFT)                      /* 0x00000004 */
#define GMAC_AXI_BUS_MODE_BLEN16_SHIFT                     (3U)
#define GMAC_AXI_BUS_MODE_BLEN16_MASK                      (0x1U << GMAC_AXI_BUS_MODE_BLEN16_SHIFT)                     /* 0x00000008 */
#define GMAC_AXI_BUS_MODE_AXI_AAL_SHIFT                    (12U)
#define GMAC_AXI_BUS_MODE_AXI_AAL_MASK                     (0x1U << GMAC_AXI_BUS_MODE_AXI_AAL_SHIFT)                    /* 0x00001000 */
#define GMAC_AXI_BUS_MODE_RD_OSR_LMT_SHIFT                 (16U)
#define GMAC_AXI_BUS_MODE_RD_OSR_LMT_MASK                  (0x3U << GMAC_AXI_BUS_MODE_RD_OSR_LMT_SHIFT)                 /* 0x00030000 */
#define GMAC_AXI_BUS_MODE_WR_OSR_LMT_SHIFT                 (20U)
#define GMAC_AXI_BUS_MODE_WR_OSR_LMT_MASK                  (0x3U << GMAC_AXI_BUS_MODE_WR_OSR_LMT_SHIFT)                 /* 0x00300000 */
#define GMAC_AXI_BUS_MODE_UNLCK_ON_MGK_RWK_SHIFT           (30U)
#define GMAC_AXI_BUS_MODE_UNLCK_ON_MGK_RWK_MASK            (0x1U << GMAC_AXI_BUS_MODE_UNLCK_ON_MGK_RWK_SHIFT)           /* 0x40000000 */
#define GMAC_AXI_BUS_MODE_EN_LPI_SHIFT                     (31U)
#define GMAC_AXI_BUS_MODE_EN_LPI_MASK                      (0x1U << GMAC_AXI_BUS_MODE_EN_LPI_SHIFT)                     /* 0x80000000 */
/* AXI_STATUS */
#define GMAC_AXI_STATUS_WR_CH_STA_SHIFT                    (0U)
#define GMAC_AXI_STATUS_WR_CH_STA_MASK                     (0x1U << GMAC_AXI_STATUS_WR_CH_STA_SHIFT)                    /* 0x00000001 */
#define GMAC_AXI_STATUS_RD_CH_STA_SHIFT                    (1U)
#define GMAC_AXI_STATUS_RD_CH_STA_MASK                     (0x1U << GMAC_AXI_STATUS_RD_CH_STA_SHIFT)                    /* 0x00000002 */
/* CUR_HOST_TX_DESC */
#define GMAC_CUR_HOST_TX_DESC_HTDAP_SHIFT                  (0U)
#define GMAC_CUR_HOST_TX_DESC_HTDAP_MASK                   (0xFFFFFFFFU << GMAC_CUR_HOST_TX_DESC_HTDAP_SHIFT)           /* 0xFFFFFFFF */
/* CUR_HOST_RX_DESC */
#define GMAC_CUR_HOST_RX_DESC_HRDAP_SHIFT                  (0U)
#define GMAC_CUR_HOST_RX_DESC_HRDAP_MASK                   (0xFFFFFFFFU << GMAC_CUR_HOST_RX_DESC_HRDAP_SHIFT)           /* 0xFFFFFFFF */
/* CUR_HOST_TX_BUF_ADDR */
#define GMAC_CUR_HOST_TX_BUF_ADDR_HTBAP_SHIFT              (0U)
#define GMAC_CUR_HOST_TX_BUF_ADDR_HTBAP_MASK               (0xFFFFFFFFU << GMAC_CUR_HOST_TX_BUF_ADDR_HTBAP_SHIFT)       /* 0xFFFFFFFF */
/* CUR_HOST_RX_BUF_ADDR */
#define GMAC_CUR_HOST_RX_BUF_ADDR_HRBAP_SHIFT              (0U)
#define GMAC_CUR_HOST_RX_BUF_ADDR_HRBAP_MASK               (0xFFFFFFFFU << GMAC_CUR_HOST_RX_BUF_ADDR_HRBAP_SHIFT)       /* 0xFFFFFFFF */
/* HW_FEA_REG */
#define GMAC_HW_FEA_REG_FEATURE0_SHIFT                     (0U)
#define GMAC_HW_FEA_REG_FEATURE0_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE0_SHIFT)                     /* 0x00000001 */
#define GMAC_HW_FEA_REG_FEATURE1_SHIFT                     (1U)
#define GMAC_HW_FEA_REG_FEATURE1_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE1_SHIFT)                     /* 0x00000002 */
#define GMAC_HW_FEA_REG_FEATURE2_SHIFT                     (2U)
#define GMAC_HW_FEA_REG_FEATURE2_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE2_SHIFT)                     /* 0x00000004 */
#define GMAC_HW_FEA_REG_FEATURE4_SHIFT                     (4U)
#define GMAC_HW_FEA_REG_FEATURE4_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE4_SHIFT)                     /* 0x00000010 */
#define GMAC_HW_FEA_REG_FEATURE5_SHIFT                     (5U)
#define GMAC_HW_FEA_REG_FEATURE5_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE5_SHIFT)                     /* 0x00000020 */
#define GMAC_HW_FEA_REG_FEATURE6_SHIFT                     (6U)
#define GMAC_HW_FEA_REG_FEATURE6_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE6_SHIFT)                     /* 0x00000040 */
#define GMAC_HW_FEA_REG_FEATURE8_SHIFT                     (8U)
#define GMAC_HW_FEA_REG_FEATURE8_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE8_SHIFT)                     /* 0x00000100 */
#define GMAC_HW_FEA_REG_FEATURE9_SHIFT                     (9U)
#define GMAC_HW_FEA_REG_FEATURE9_MASK                      (0x1U << GMAC_HW_FEA_REG_FEATURE9_SHIFT)                     /* 0x00000200 */
#define GMAC_HW_FEA_REG_FEATURE10_SHIFT                    (10U)
#define GMAC_HW_FEA_REG_FEATURE10_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE10_SHIFT)                    /* 0x00000400 */
#define GMAC_HW_FEA_REG_FEATURE11_SHIFT                    (11U)
#define GMAC_HW_FEA_REG_FEATURE11_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE11_SHIFT)                    /* 0x00000800 */
#define GMAC_HW_FEA_REG_FEATURE12_SHIFT                    (12U)
#define GMAC_HW_FEA_REG_FEATURE12_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE12_SHIFT)                    /* 0x00001000 */
#define GMAC_HW_FEA_REG_FEATURE13_SHIFT                    (13U)
#define GMAC_HW_FEA_REG_FEATURE13_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE13_SHIFT)                    /* 0x00002000 */
#define GMAC_HW_FEA_REG_FEATURE16_SHIFT                    (16U)
#define GMAC_HW_FEA_REG_FEATURE16_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE16_SHIFT)                    /* 0x00010000 */
#define GMAC_HW_FEA_REG_FEATURE17_SHIFT                    (17U)
#define GMAC_HW_FEA_REG_FEATURE17_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE17_SHIFT)                    /* 0x00020000 */
#define GMAC_HW_FEA_REG_FEATURE18_SHIFT                    (18U)
#define GMAC_HW_FEA_REG_FEATURE18_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE18_SHIFT)                    /* 0x00040000 */
#define GMAC_HW_FEA_REG_FEATURE19_SHIFT                    (19U)
#define GMAC_HW_FEA_REG_FEATURE19_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE19_SHIFT)                    /* 0x00080000 */
#define GMAC_HW_FEA_REG_FEATURE24_SHIFT                    (24U)
#define GMAC_HW_FEA_REG_FEATURE24_MASK                     (0x1U << GMAC_HW_FEA_REG_FEATURE24_SHIFT)                    /* 0x01000000 */
/****************************************CEVA_GRF****************************************/
/* GRF_DSP_CON0 */
#define CEVA_GRF_GRF_DSP_CON0_MCACHE_INVALIDATE_STRAP_SHIFT (0U)
#define CEVA_GRF_GRF_DSP_CON0_MCACHE_INVALIDATE_STRAP_MASK (0x1U << CEVA_GRF_GRF_DSP_CON0_MCACHE_INVALIDATE_STRAP_SHIFT) /* 0x00000001 */
#define CEVA_GRF_GRF_DSP_CON0_EXTERNAL_WAIT_SHIFT          (1U)
#define CEVA_GRF_GRF_DSP_CON0_EXTERNAL_WAIT_MASK           (0x1U << CEVA_GRF_GRF_DSP_CON0_EXTERNAL_WAIT_SHIFT)          /* 0x00000002 */
#define CEVA_GRF_GRF_DSP_CON0_JT_AP_SHIFT                  (2U)
#define CEVA_GRF_GRF_DSP_CON0_JT_AP_MASK                   (0x1U << CEVA_GRF_GRF_DSP_CON0_JT_AP_SHIFT)                  /* 0x00000004 */
#define CEVA_GRF_GRF_DSP_CON0_EXT_PV_SHIFT                 (3U)
#define CEVA_GRF_GRF_DSP_CON0_EXT_PV_MASK                  (0x1U << CEVA_GRF_GRF_DSP_CON0_EXT_PV_SHIFT)                 /* 0x00000008 */
#define CEVA_GRF_GRF_DSP_CON0_EXT_VOM_SHIFT                (4U)
#define CEVA_GRF_GRF_DSP_CON0_EXT_VOM_MASK                 (0x3U << CEVA_GRF_GRF_DSP_CON0_EXT_VOM_SHIFT)                /* 0x00000030 */
#define CEVA_GRF_GRF_DSP_CON0_STOP_SD_SHIFT                (6U)
#define CEVA_GRF_GRF_DSP_CON0_STOP_SD_MASK                 (0x1U << CEVA_GRF_GRF_DSP_CON0_STOP_SD_SHIFT)                /* 0x00000040 */
#define CEVA_GRF_GRF_DSP_CON0_CORE_RCVR_SHIFT              (7U)
#define CEVA_GRF_GRF_DSP_CON0_CORE_RCVR_MASK               (0x1U << CEVA_GRF_GRF_DSP_CON0_CORE_RCVR_SHIFT)              /* 0x00000080 */
#define CEVA_GRF_GRF_DSP_CON0_EXT_BP1_REQ_SHIFT            (8U)
#define CEVA_GRF_GRF_DSP_CON0_EXT_BP1_REQ_MASK             (0x1U << CEVA_GRF_GRF_DSP_CON0_EXT_BP1_REQ_SHIFT)            /* 0x00000100 */
#define CEVA_GRF_GRF_DSP_CON0_EXT_BP2_REQ_SHIFT            (9U)
#define CEVA_GRF_GRF_DSP_CON0_EXT_BP2_REQ_MASK             (0x1U << CEVA_GRF_GRF_DSP_CON0_EXT_BP2_REQ_SHIFT)            /* 0x00000200 */
#define CEVA_GRF_GRF_DSP_CON0_EXT_PMEM_WR_SHIFT            (10U)
#define CEVA_GRF_GRF_DSP_CON0_EXT_PMEM_WR_MASK             (0x1U << CEVA_GRF_GRF_DSP_CON0_EXT_PMEM_WR_SHIFT)            /* 0x00000400 */
#define CEVA_GRF_GRF_DSP_CON0_BS_REG_TDO_SHIFT             (11U)
#define CEVA_GRF_GRF_DSP_CON0_BS_REG_TDO_MASK              (0x1U << CEVA_GRF_GRF_DSP_CON0_BS_REG_TDO_SHIFT)             /* 0x00000800 */
#define CEVA_GRF_GRF_DSP_CON0_CEVA_BOOT_SHIFT              (12U)
#define CEVA_GRF_GRF_DSP_CON0_CEVA_BOOT_MASK               (0x1U << CEVA_GRF_GRF_DSP_CON0_CEVA_BOOT_SHIFT)              /* 0x00001000 */
#define CEVA_GRF_GRF_DSP_CON0_CEVA_GLOBAL_RSTN_REQ_SHIFT   (13U)
#define CEVA_GRF_GRF_DSP_CON0_CEVA_GLOBAL_RSTN_REQ_MASK    (0x1U << CEVA_GRF_GRF_DSP_CON0_CEVA_GLOBAL_RSTN_REQ_SHIFT)   /* 0x00002000 */
/* GRF_DSP_CON1 */
#define CEVA_GRF_GRF_DSP_CON1_EXT_DDMA_DBG_MATCH_ACK_SHIFT (0U)
#define CEVA_GRF_GRF_DSP_CON1_EXT_DDMA_DBG_MATCH_ACK_MASK  (0x1U << CEVA_GRF_GRF_DSP_CON1_EXT_DDMA_DBG_MATCH_ACK_SHIFT) /* 0x00000001 */
#define CEVA_GRF_GRF_DSP_CON1_NEXT_DDMA_SHIFT              (1U)
#define CEVA_GRF_GRF_DSP_CON1_NEXT_DDMA_MASK               (0x1U << CEVA_GRF_GRF_DSP_CON1_NEXT_DDMA_SHIFT)              /* 0x00000002 */
#define CEVA_GRF_GRF_DSP_CON1_ACU_LOCK_SHIFT               (2U)
#define CEVA_GRF_GRF_DSP_CON1_ACU_LOCK_MASK                (0x1U << CEVA_GRF_GRF_DSP_CON1_ACU_LOCK_SHIFT)               /* 0x00000004 */
#define CEVA_GRF_GRF_DSP_CON1_ACU_SLV_ACC_SHIFT            (3U)
#define CEVA_GRF_GRF_DSP_CON1_ACU_SLV_ACC_MASK             (0x1U << CEVA_GRF_GRF_DSP_CON1_ACU_SLV_ACC_SHIFT)            /* 0x00000008 */
#define CEVA_GRF_GRF_DSP_CON1_INT1_EN_SHIFT                (4U)
#define CEVA_GRF_GRF_DSP_CON1_INT1_EN_MASK                 (0x1U << CEVA_GRF_GRF_DSP_CON1_INT1_EN_SHIFT)                /* 0x00000010 */
#define CEVA_GRF_GRF_DSP_CON1_VINT_EN_SHIFT                (5U)
#define CEVA_GRF_GRF_DSP_CON1_VINT_EN_MASK                 (0x1U << CEVA_GRF_GRF_DSP_CON1_VINT_EN_SHIFT)                /* 0x00000020 */
/* GRF_DSP_CON2 */
#define CEVA_GRF_GRF_DSP_CON2_QN_DESC_EN_SHIFT             (0U)
#define CEVA_GRF_GRF_DSP_CON2_QN_DESC_EN_MASK              (0xFFFFU << CEVA_GRF_GRF_DSP_CON2_QN_DESC_EN_SHIFT)          /* 0x0000FFFF */
/* GRF_DSP_CON3 */
#define CEVA_GRF_GRF_DSP_CON3_QMAN_SEMAPHORE_GRANT_SHIFT   (0U)
#define CEVA_GRF_GRF_DSP_CON3_QMAN_SEMAPHORE_GRANT_MASK    (0xFFFFU << CEVA_GRF_GRF_DSP_CON3_QMAN_SEMAPHORE_GRANT_SHIFT) /* 0x0000FFFF */
/* GRF_DSP_CON4 */
#define CEVA_GRF_GRF_DSP_CON4_CEVA_VECTOR_SHIFT            (0U)
#define CEVA_GRF_GRF_DSP_CON4_CEVA_VECTOR_MASK             (0xFFFFU << CEVA_GRF_GRF_DSP_CON4_CEVA_VECTOR_SHIFT)         /* 0x0000FFFF */
/* GRF_DSP_CON5 */
#define CEVA_GRF_GRF_DSP_CON5_CEVA_VECTOR_SHIFT            (0U)
#define CEVA_GRF_GRF_DSP_CON5_CEVA_VECTOR_MASK             (0xFFFFU << CEVA_GRF_GRF_DSP_CON5_CEVA_VECTOR_SHIFT)         /* 0x0000FFFF */
/* GRF_DSP_STAT0 */
#define CEVA_GRF_GRF_DSP_STAT0_CEVAXM4_GPOUT_SHIFT         (0U)
#define CEVA_GRF_GRF_DSP_STAT0_CEVAXM4_GPOUT_MASK          (0xFFFFFFFFU << CEVA_GRF_GRF_DSP_STAT0_CEVAXM4_GPOUT_SHIFT)  /* 0xFFFFFFFF */
/* GRF_DSP_STAT1 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_EOTBIT_SHIFT    (0U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_EOTBIT_MASK     (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_EOTBIT_SHIFT)    /* 0x00000001 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_TRP_SRV_SHIFT   (1U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_TRP_SRV_MASK    (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_TRP_SRV_SHIFT)   /* 0x00000002 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_OM_SHIFT        (2U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_OM_MASK         (0x3U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_SEQ_OM_SHIFT)        /* 0x0000000C */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_DSP_IDLE_SHIFT  (4U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_DSP_IDLE_MASK   (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_DSP_IDLE_SHIFT)  /* 0x00000010 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CORE_IDLE_SHIFT (5U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CORE_IDLE_MASK  (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CORE_IDLE_SHIFT) /* 0x00000020 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CACTIVE_SHIFT   (6U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CACTIVE_MASK    (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CACTIVE_SHIFT)   /* 0x00000040 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_PSHTDOWN_SHIFT  (8U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_PSHTDOWN_MASK   (0xFFU << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_PSHTDOWN_SHIFT) /* 0x0000FF00 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_SYS_PSHTDWN_SHIFT (16U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_SYS_PSHTDWN_MASK (0x3FU << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_SYS_PSHTDWN_SHIFT) /* 0x003F0000 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CSYSACK_SHIFT   (22U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CSYSACK_MASK    (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_PSU_CSYSACK_SHIFT)   /* 0x00400000 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_EPP_APS_SHIFT       (23U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_EPP_APS_MASK        (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_EPP_APS_SHIFT)       /* 0x00800000 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_IOP_APS_SHIFT       (24U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_IOP_APS_MASK        (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_IOP_APS_SHIFT)       /* 0x01000000 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_EDP_APS_SHIFT       (25U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_EDP_APS_MASK        (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_EDP_APS_SHIFT)       /* 0x02000000 */
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_CVERBIT_SHIFT       (26U)
#define CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_CVERBIT_MASK        (0x1U << CEVA_GRF_GRF_DSP_STAT1_CEVAXM4_CVERBIT_SHIFT)       /* 0x04000000 */
/* GRF_DSP_STAT2 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_DDMA_DBG_MATCH_SHIFT (0U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_DDMA_DBG_MATCH_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_DDMA_DBG_MATCH_SHIFT) /* 0x00000001 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_PSU_MAPV_IRQ_SHIFT  (2U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_PSU_MAPV_IRQ_MASK   (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_PSU_MAPV_IRQ_SHIFT)  /* 0x00000004 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_EXT1_ACK_SHIFT  (3U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_EXT1_ACK_MASK   (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_EXT1_ACK_SHIFT)  /* 0x00000008 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_GP_OUT_SHIFT    (4U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_GP_OUT_MASK     (0xFU << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_GP_OUT_SHIFT)    /* 0x000000F0 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_JTAG_STATE_SHIFT (8U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_JTAG_STATE_MASK (0xFU << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_JTAG_STATE_SHIFT) /* 0x00000F00 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_EXT2_ACK_SHIFT  (12U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_EXT2_ACK_MASK   (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_EXT2_ACK_SHIFT)  /* 0x00001000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_CORE_RST_SHIFT  (13U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_CORE_RST_MASK   (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_CORE_RST_SHIFT)  /* 0x00002000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_DEBUG_SHIFT     (14U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_DEBUG_MASK      (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_OCM_DEBUG_SHIFT)     /* 0x00004000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_PSU_CORE_WAIT_SHIFT (15U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_PSU_CORE_WAIT_MASK  (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_PSU_CORE_WAIT_SHIFT) /* 0x00008000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_IOP_WDOG_VIOL_SHIFT (16U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_IOP_WDOG_VIOL_MASK  (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_IOP_WDOG_VIOL_SHIFT) /* 0x00010000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_EPP_WDOG_VIOL_SHIFT (17U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_EPP_WDOG_VIOL_MASK  (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_EPP_WDOG_VIOL_SHIFT) /* 0x00020000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_EDP_WDOG_VIOL_SHIFT (18U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_EDP_WDOG_VIOL_MASK  (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_EDP_WDOG_VIOL_SHIFT) /* 0x00040000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT0_ACK_N_SHIFT (19U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT0_ACK_N_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT0_ACK_N_SHIFT) /* 0x00080000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT1_ACK_N_SHIFT (20U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT1_ACK_N_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT1_ACK_N_SHIFT) /* 0x00100000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT2_ACK_N_SHIFT (21U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT2_ACK_N_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT2_ACK_N_SHIFT) /* 0x00200000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT3_ACK_N_SHIFT (22U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT3_ACK_N_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT3_ACK_N_SHIFT) /* 0x00400000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT4_ACK_N_SHIFT (23U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT4_ACK_N_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_INT4_ACK_N_SHIFT) /* 0x00800000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_BP_ACK_N_SHIFT  (24U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_BP_ACK_N_MASK   (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_BP_ACK_N_SHIFT)  /* 0x01000000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_NMI_ACK_N_SHIFT (25U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_NMI_ACK_N_MASK  (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_NMI_ACK_N_SHIFT) /* 0x02000000 */
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_VINT_ACK_N_SHIFT (26U)
#define CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_VINT_ACK_N_MASK (0x1U << CEVA_GRF_GRF_DSP_STAT2_CEVAXM4_SEQ_VINT_ACK_N_SHIFT) /* 0x04000000 */
/* GRF_DSP_STAT3 */
#define CEVA_GRF_GRF_DSP_STAT3_QMAN_SEMAPHORE_REQ_SHIFT    (0U)
#define CEVA_GRF_GRF_DSP_STAT3_QMAN_SEMAPHORE_REQ_MASK     (0xFFFFU << CEVA_GRF_GRF_DSP_STAT3_QMAN_SEMAPHORE_REQ_SHIFT) /* 0x0000FFFF */
/* GRF_DSP_STAT4 */
#define CEVA_GRF_GRF_DSP_STAT4_CEVAXM4_MCCI_RD_IND_R_SHIFT (0U)
#define CEVA_GRF_GRF_DSP_STAT4_CEVAXM4_MCCI_RD_IND_R_MASK  (0xFFFFFFFFU << CEVA_GRF_GRF_DSP_STAT4_CEVAXM4_MCCI_RD_IND_R_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EDP_PERF_CON0 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_PERF_WORK_CEVA_EDP_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_PERF_WORK_CEVA_EDP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_PERF_WORK_CEVA_EDP_SHIFT) /* 0x00000001 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_PERF_CLR_CEVA_EDP_SHIFT (1U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_PERF_CLR_CEVA_EDP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_PERF_CLR_CEVA_EDP_SHIFT) /* 0x00000002 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_CNT_TYPE_CEVA_EDP_SHIFT (2U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_CNT_TYPE_CEVA_EDP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AXI_CNT_TYPE_CEVA_EDP_SHIFT) /* 0x00000004 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AR_CNT_ID_TYPE_CEVA_EDP_SHIFT (3U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AR_CNT_ID_TYPE_CEVA_EDP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AR_CNT_ID_TYPE_CEVA_EDP_SHIFT) /* 0x00000008 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AW_CNT_ID_TYPE_CEVA_EDP_SHIFT (4U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AW_CNT_ID_TYPE_CEVA_EDP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_AW_CNT_ID_TYPE_CEVA_EDP_SHIFT) /* 0x00000010 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_DW_DDR_ALIGN_TYPE_CEVA_EDP_SHIFT (5U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_DW_DDR_ALIGN_TYPE_CEVA_EDP_MASK (0x3U << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_DW_DDR_ALIGN_TYPE_CEVA_EDP_SHIFT) /* 0x00000060 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_RD_LATENCY_ID_CEVA_EDP_SHIFT (8U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_RD_LATENCY_ID_CEVA_EDP_MASK (0xFU << CEVA_GRF_GRF_CEVA_EDP_PERF_CON0_SW_RD_LATENCY_ID_CEVA_EDP_SHIFT) /* 0x00000F00 */
/* GRF_CEVA_EDP_PERF_CON1 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON1_SW_RD_LATENCY_THR_CEVA_EDP_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON1_SW_RD_LATENCY_THR_CEVA_EDP_MASK (0xFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_CON1_SW_RD_LATENCY_THR_CEVA_EDP_SHIFT) /* 0x00000FFF */
/* GRF_CEVA_EDP_PERF_CON2 */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON2_SW_AR_COUNT_ID_CEVA_EDP_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON2_SW_AR_COUNT_ID_CEVA_EDP_MASK (0xFU << CEVA_GRF_GRF_CEVA_EDP_PERF_CON2_SW_AR_COUNT_ID_CEVA_EDP_SHIFT) /* 0x0000000F */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON2_SW_AW_COUNT_ID_CEVA_EDP_SHIFT (8U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_CON2_SW_AW_COUNT_ID_CEVA_EDP_MASK (0xFU << CEVA_GRF_GRF_CEVA_EDP_PERF_CON2_SW_AW_COUNT_ID_CEVA_EDP_SHIFT) /* 0x00000F00 */
/* GRF_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM_MASK (0xFFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM_CEVA_EDP_PERF_RD_MAX_LATENCY_NUM_SHIFT) /* 0x0000FFFF */
/* GRF_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EDP_PERF_RD_LATENCY_ACC_SUM */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_LATENCY_ACC_SUM_CEVA_EDP_PERF_RD_LATENCY_ACC_NUM_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_LATENCY_ACC_SUM_CEVA_EDP_PERF_RD_LATENCY_ACC_NUM_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_RD_LATENCY_ACC_SUM_CEVA_EDP_PERF_RD_LATENCY_ACC_NUM_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EDP_PERF_RD_AXI_TOTAL_BYTE */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_AXI_TOTAL_BYTE_CEVA_EDP_PERF_RD_AXI_TOTAL_TYPE_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_RD_AXI_TOTAL_BYTE_CEVA_EDP_PERF_RD_AXI_TOTAL_TYPE_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_RD_AXI_TOTAL_BYTE_CEVA_EDP_PERF_RD_AXI_TOTAL_TYPE_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EDP_PERF_WR_AXI_TOTAL_BYTE */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_WR_AXI_TOTAL_BYTE_CEVA_EDP_PERF_WR_AXI_TOTAL_TYPE_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_WR_AXI_TOTAL_BYTE_CEVA_EDP_PERF_WR_AXI_TOTAL_TYPE_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_WR_AXI_TOTAL_BYTE_CEVA_EDP_PERF_WR_AXI_TOTAL_TYPE_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EDP_PERF_WORKING_CNT */
#define CEVA_GRF_GRF_CEVA_EDP_PERF_WORKING_CNT_CEVA_EDP_PERF_WORKING_CNT_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EDP_PERF_WORKING_CNT_CEVA_EDP_PERF_WORKING_CNT_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EDP_PERF_WORKING_CNT_CEVA_EDP_PERF_WORKING_CNT_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EPP_PERF_CON0 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_PERF_WORK_CEVA_EPP_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_PERF_WORK_CEVA_EPP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_PERF_WORK_CEVA_EPP_SHIFT) /* 0x00000001 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_PERF_CLR_CEVA_EPP_SHIFT (1U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_PERF_CLR_CEVA_EPP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_PERF_CLR_CEVA_EPP_SHIFT) /* 0x00000002 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_CNT_TYPE_CEVA_EPP_SHIFT (2U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_CNT_TYPE_CEVA_EPP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AXI_CNT_TYPE_CEVA_EPP_SHIFT) /* 0x00000004 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AR_CNT_ID_TYPE_CEVA_EPP_SHIFT (3U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AR_CNT_ID_TYPE_CEVA_EPP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AR_CNT_ID_TYPE_CEVA_EPP_SHIFT) /* 0x00000008 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AW_CNT_ID_TYPE_CEVA_EPP_SHIFT (4U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AW_CNT_ID_TYPE_CEVA_EPP_MASK (0x1U << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_AW_CNT_ID_TYPE_CEVA_EPP_SHIFT) /* 0x00000010 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_DW_DDR_ALIGN_TYPE_CEVA_EPP_SHIFT (5U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_DW_DDR_ALIGN_TYPE_CEVA_EPP_MASK (0x3U << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_DW_DDR_ALIGN_TYPE_CEVA_EPP_SHIFT) /* 0x00000060 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_RD_LATENCY_ID_CEVA_EPP_SHIFT (8U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_RD_LATENCY_ID_CEVA_EPP_MASK (0xFU << CEVA_GRF_GRF_CEVA_EPP_PERF_CON0_SW_RD_LATENCY_ID_CEVA_EPP_SHIFT) /* 0x00000F00 */
/* GRF_CEVA_EPP_PERF_CON1 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON1_SW_RD_LATENCY_THR_CEVA_EPP_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON1_SW_RD_LATENCY_THR_CEVA_EPP_MASK (0xFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_CON1_SW_RD_LATENCY_THR_CEVA_EPP_SHIFT) /* 0x00000FFF */
/* GRF_CEVA_EPP_PERF_CON2 */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON2_SW_AR_COUNT_ID_CEVA_EPP_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON2_SW_AR_COUNT_ID_CEVA_EPP_MASK (0xFU << CEVA_GRF_GRF_CEVA_EPP_PERF_CON2_SW_AR_COUNT_ID_CEVA_EPP_SHIFT) /* 0x0000000F */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON2_SW_AW_COUNT_ID_CEVA_EPP_SHIFT (8U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_CON2_SW_AW_COUNT_ID_CEVA_EPP_MASK (0xFU << CEVA_GRF_GRF_CEVA_EPP_PERF_CON2_SW_AW_COUNT_ID_CEVA_EPP_SHIFT) /* 0x00000F00 */
/* GRF_CEVA_EPP_PERF_RD_MAX_LATENCY_NUM */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_MAX_LATENCY_NUM_CEVA_EPP_PER_RD_MAX_LATENCY_NUM_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_MAX_LATENCY_NUM_CEVA_EPP_PER_RD_MAX_LATENCY_NUM_MASK (0xFFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_RD_MAX_LATENCY_NUM_CEVA_EPP_PER_RD_MAX_LATENCY_NUM_SHIFT) /* 0x0000FFFF */
/* GRF_CEVA_EPP_PERF_RD_LATENCY_SAMP_NUM */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_LATENCY_SAMP_NUM_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_LATENCY_SAMP_NUM_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_MASK (0xFFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_RD_LATENCY_SAMP_NUM_CEVA_EDP_PERF_RD_LATENCY_SAMP_NUM_SHIFT) /* 0x0000FFFF */
/* GRF_CEVA_EPP_PERF_RD_LATENCY_ACC_SUM */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_LATENCY_ACC_SUM_CEVA_EPP_PERF_RD_LATENCY_ACC_NUM_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_LATENCY_ACC_SUM_CEVA_EPP_PERF_RD_LATENCY_ACC_NUM_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_RD_LATENCY_ACC_SUM_CEVA_EPP_PERF_RD_LATENCY_ACC_NUM_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EPP_PERF_RD_AXI_TOTAL_BYTE */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_AXI_TOTAL_BYTE_CEVA_EPP_PERF_RD_AXI_TOTAL_TYPE_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_RD_AXI_TOTAL_BYTE_CEVA_EPP_PERF_RD_AXI_TOTAL_TYPE_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_RD_AXI_TOTAL_BYTE_CEVA_EPP_PERF_RD_AXI_TOTAL_TYPE_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EPP_PERF_WR_AXI_TOTAL_BYTE */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_WR_AXI_TOTAL_BYTE_CEVA_EPP_PERF_WR_AXI_TOTAL_TYPE_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_WR_AXI_TOTAL_BYTE_CEVA_EPP_PERF_WR_AXI_TOTAL_TYPE_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_WR_AXI_TOTAL_BYTE_CEVA_EPP_PERF_WR_AXI_TOTAL_TYPE_SHIFT) /* 0xFFFFFFFF */
/* GRF_CEVA_EPP_PERF_WORKING_CNT */
#define CEVA_GRF_GRF_CEVA_EPP_PERF_WORKING_CNT_CEVA_EPP_PERF_WORKING_CNT_SHIFT (0U)
#define CEVA_GRF_GRF_CEVA_EPP_PERF_WORKING_CNT_CEVA_EPP_PERF_WORKING_CNT_MASK (0xFFFFFFFFU << CEVA_GRF_GRF_CEVA_EPP_PERF_WORKING_CNT_CEVA_EPP_PERF_WORKING_CNT_SHIFT) /* 0xFFFFFFFF */
/**************************************CEVA_MAILBOX**************************************/
/* A2B_INTEN */
#define CEVA_MAILBOX_A2B_INTEN_INT0_SHIFT                  (0U)
#define CEVA_MAILBOX_A2B_INTEN_INT0_MASK                   (0x1U << CEVA_MAILBOX_A2B_INTEN_INT0_SHIFT)                  /* 0x00000001 */
#define CEVA_MAILBOX_A2B_INTEN_INT1_SHIFT                  (1U)
#define CEVA_MAILBOX_A2B_INTEN_INT1_MASK                   (0x1U << CEVA_MAILBOX_A2B_INTEN_INT1_SHIFT)                  /* 0x00000002 */
#define CEVA_MAILBOX_A2B_INTEN_INT2_SHIFT                  (2U)
#define CEVA_MAILBOX_A2B_INTEN_INT2_MASK                   (0x1U << CEVA_MAILBOX_A2B_INTEN_INT2_SHIFT)                  /* 0x00000004 */
#define CEVA_MAILBOX_A2B_INTEN_INT3_SHIFT                  (3U)
#define CEVA_MAILBOX_A2B_INTEN_INT3_MASK                   (0x1U << CEVA_MAILBOX_A2B_INTEN_INT3_SHIFT)                  /* 0x00000008 */
/* A2B_STATUS */
#define CEVA_MAILBOX_A2B_STATUS_INT0_SHIFT                 (0U)
#define CEVA_MAILBOX_A2B_STATUS_INT0_MASK                  (0x1U << CEVA_MAILBOX_A2B_STATUS_INT0_SHIFT)                 /* 0x00000001 */
#define CEVA_MAILBOX_A2B_STATUS_INT1_SHIFT                 (1U)
#define CEVA_MAILBOX_A2B_STATUS_INT1_MASK                  (0x1U << CEVA_MAILBOX_A2B_STATUS_INT1_SHIFT)                 /* 0x00000002 */
#define CEVA_MAILBOX_A2B_STATUS_INT2_SHIFT                 (2U)
#define CEVA_MAILBOX_A2B_STATUS_INT2_MASK                  (0x1U << CEVA_MAILBOX_A2B_STATUS_INT2_SHIFT)                 /* 0x00000004 */
#define CEVA_MAILBOX_A2B_STATUS_INT3_SHIFT                 (3U)
#define CEVA_MAILBOX_A2B_STATUS_INT3_MASK                  (0x1U << CEVA_MAILBOX_A2B_STATUS_INT3_SHIFT)                 /* 0x00000008 */
/* A2B_CMD_0 */
#define CEVA_MAILBOX_A2B_CMD_0_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_A2B_CMD_0_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_A2B_CMD_0_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* A2B_DAT_0 */
#define CEVA_MAILBOX_A2B_DAT_0_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_A2B_DAT_0_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_A2B_DAT_0_DATA_SHIFT)           /* 0xFFFFFFFF */
/* A2B_CMD_1 */
#define CEVA_MAILBOX_A2B_CMD_1_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_A2B_CMD_1_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_A2B_CMD_1_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* A2B_DAT_1 */
#define CEVA_MAILBOX_A2B_DAT_1_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_A2B_DAT_1_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_A2B_DAT_1_DATA_SHIFT)           /* 0xFFFFFFFF */
/* A2B_CMD_2 */
#define CEVA_MAILBOX_A2B_CMD_2_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_A2B_CMD_2_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_A2B_CMD_2_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* A2B_DAT_2 */
#define CEVA_MAILBOX_A2B_DAT_2_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_A2B_DAT_2_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_A2B_DAT_2_DATA_SHIFT)           /* 0xFFFFFFFF */
/* A2B_CMD_3 */
#define CEVA_MAILBOX_A2B_CMD_3_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_A2B_CMD_3_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_A2B_CMD_3_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* A2B_DAT_3 */
#define CEVA_MAILBOX_A2B_DAT_3_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_A2B_DAT_3_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_A2B_DAT_3_DATA_SHIFT)           /* 0xFFFFFFFF */
/* B2A_INTEN */
#define CEVA_MAILBOX_B2A_INTEN_INT0_SHIFT                  (0U)
#define CEVA_MAILBOX_B2A_INTEN_INT0_MASK                   (0x1U << CEVA_MAILBOX_B2A_INTEN_INT0_SHIFT)                  /* 0x00000001 */
#define CEVA_MAILBOX_B2A_INTEN_INT1_SHIFT                  (1U)
#define CEVA_MAILBOX_B2A_INTEN_INT1_MASK                   (0x1U << CEVA_MAILBOX_B2A_INTEN_INT1_SHIFT)                  /* 0x00000002 */
#define CEVA_MAILBOX_B2A_INTEN_INT2_SHIFT                  (2U)
#define CEVA_MAILBOX_B2A_INTEN_INT2_MASK                   (0x1U << CEVA_MAILBOX_B2A_INTEN_INT2_SHIFT)                  /* 0x00000004 */
#define CEVA_MAILBOX_B2A_INTEN_INT3_SHIFT                  (3U)
#define CEVA_MAILBOX_B2A_INTEN_INT3_MASK                   (0x1U << CEVA_MAILBOX_B2A_INTEN_INT3_SHIFT)                  /* 0x00000008 */
/* B2A_STATUS */
#define CEVA_MAILBOX_B2A_STATUS_INT0_SHIFT                 (0U)
#define CEVA_MAILBOX_B2A_STATUS_INT0_MASK                  (0x1U << CEVA_MAILBOX_B2A_STATUS_INT0_SHIFT)                 /* 0x00000001 */
#define CEVA_MAILBOX_B2A_STATUS_INT1_SHIFT                 (1U)
#define CEVA_MAILBOX_B2A_STATUS_INT1_MASK                  (0x1U << CEVA_MAILBOX_B2A_STATUS_INT1_SHIFT)                 /* 0x00000002 */
#define CEVA_MAILBOX_B2A_STATUS_INT2_SHIFT                 (2U)
#define CEVA_MAILBOX_B2A_STATUS_INT2_MASK                  (0x1U << CEVA_MAILBOX_B2A_STATUS_INT2_SHIFT)                 /* 0x00000004 */
#define CEVA_MAILBOX_B2A_STATUS_INT3_SHIFT                 (3U)
#define CEVA_MAILBOX_B2A_STATUS_INT3_MASK                  (0x1U << CEVA_MAILBOX_B2A_STATUS_INT3_SHIFT)                 /* 0x00000008 */
/* B2A_CMD_0 */
#define CEVA_MAILBOX_B2A_CMD_0_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_B2A_CMD_0_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_B2A_CMD_0_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* B2A_DAT_0 */
#define CEVA_MAILBOX_B2A_DAT_0_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_B2A_DAT_0_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_B2A_DAT_0_DATA_SHIFT)           /* 0xFFFFFFFF */
/* B2A_CMD_1 */
#define CEVA_MAILBOX_B2A_CMD_1_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_B2A_CMD_1_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_B2A_CMD_1_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* B2A_DAT_1 */
#define CEVA_MAILBOX_B2A_DAT_1_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_B2A_DAT_1_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_B2A_DAT_1_DATA_SHIFT)           /* 0xFFFFFFFF */
/* B2A_CMD_2 */
#define CEVA_MAILBOX_B2A_CMD_2_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_B2A_CMD_2_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_B2A_CMD_2_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* B2A_DAT_2 */
#define CEVA_MAILBOX_B2A_DAT_2_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_B2A_DAT_2_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_B2A_DAT_2_DATA_SHIFT)           /* 0xFFFFFFFF */
/* B2A_CMD_3 */
#define CEVA_MAILBOX_B2A_CMD_3_COMMAND_SHIFT               (0U)
#define CEVA_MAILBOX_B2A_CMD_3_COMMAND_MASK                (0xFFFFFFFFU << CEVA_MAILBOX_B2A_CMD_3_COMMAND_SHIFT)        /* 0xFFFFFFFF */
/* B2A_DAT_3 */
#define CEVA_MAILBOX_B2A_DAT_3_DATA_SHIFT                  (0U)
#define CEVA_MAILBOX_B2A_DAT_3_DATA_MASK                   (0xFFFFFFFFU << CEVA_MAILBOX_B2A_DAT_3_DATA_SHIFT)           /* 0xFFFFFFFF */
/* ATOMIC_LOCK_00 */
#define CEVA_MAILBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_01 */
#define CEVA_MAILBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_02 */
#define CEVA_MAILBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_03 */
#define CEVA_MAILBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_04 */
#define CEVA_MAILBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_05 */
#define CEVA_MAILBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_06 */
#define CEVA_MAILBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_07 */
#define CEVA_MAILBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_08 */
#define CEVA_MAILBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_09 */
#define CEVA_MAILBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_10 */
#define CEVA_MAILBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_11 */
#define CEVA_MAILBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_12 */
#define CEVA_MAILBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_13 */
#define CEVA_MAILBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_14 */
#define CEVA_MAILBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_15 */
#define CEVA_MAILBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_16 */
#define CEVA_MAILBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_17 */
#define CEVA_MAILBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_18 */
#define CEVA_MAILBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_19 */
#define CEVA_MAILBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_20 */
#define CEVA_MAILBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_21 */
#define CEVA_MAILBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_22 */
#define CEVA_MAILBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_23 */
#define CEVA_MAILBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_24 */
#define CEVA_MAILBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_25 */
#define CEVA_MAILBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_26 */
#define CEVA_MAILBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_27 */
#define CEVA_MAILBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_28 */
#define CEVA_MAILBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_29 */
#define CEVA_MAILBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_30 */
#define CEVA_MAILBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_SHIFT)   /* 0x00000001 */
/* ATOMIC_LOCK_31 */
#define CEVA_MAILBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_SHIFT   (0U)
#define CEVA_MAILBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_MASK    (0x1U << CEVA_MAILBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_SHIFT)   /* 0x00000001 */
/****************************************CEVA_CNT****************************************/
/* CEVA_PFM_CNT_0 */
#define CEVA_CNT_CEVA_PFM_CNT_0_COUNTER_0_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_0_COUNTER_0_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_0_COUNTER_0_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_1 */
#define CEVA_CNT_CEVA_PFM_CNT_1_COUNTER_1_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_1_COUNTER_1_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_1_COUNTER_1_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_2 */
#define CEVA_CNT_CEVA_PFM_CNT_2_COUNTER_2_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_2_COUNTER_2_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_2_COUNTER_2_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_3 */
#define CEVA_CNT_CEVA_PFM_CNT_3_COUNTER_3_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_3_COUNTER_3_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_3_COUNTER_3_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_4 */
#define CEVA_CNT_CEVA_PFM_CNT_4_COUNTER_4_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_4_COUNTER_4_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_4_COUNTER_4_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_5 */
#define CEVA_CNT_CEVA_PFM_CNT_5_COUNTER_5_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_5_COUNTER_5_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_5_COUNTER_5_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_6 */
#define CEVA_CNT_CEVA_PFM_CNT_6_COUNTER_6_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_6_COUNTER_6_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_6_COUNTER_6_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CNT_7 */
#define CEVA_CNT_CEVA_PFM_CNT_7_COUNTER_7_SHIFT            (0U)
#define CEVA_CNT_CEVA_PFM_CNT_7_COUNTER_7_MASK             (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_CNT_7_COUNTER_7_SHIFT)     /* 0xFFFFFFFF */
/* CEVA_PFM_CTRL_0 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_0_SHIFT        (0U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_0_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_0_SHIFT)        /* 0x00000001 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_0_SHIFT       (1U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_0_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_0_SHIFT)       /* 0x00000002 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_0_SHIFT           (2U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_0_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_0_SHIFT)           /* 0x00000004 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_1_SHIFT        (4U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_1_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_1_SHIFT)        /* 0x00000010 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_1_SHIFT       (5U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_1_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_1_SHIFT)       /* 0x00000020 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_1_SHIFT           (6U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_1_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_1_SHIFT)           /* 0x00000040 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_2_SHIFT        (8U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_2_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_2_SHIFT)        /* 0x00000100 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_2_SHIFT       (9U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_2_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_2_SHIFT)       /* 0x00000200 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_2_SHIFT           (10U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_2_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_2_SHIFT)           /* 0x00000400 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_3_SHIFT        (12U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_3_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_ENABLE_3_SHIFT)        /* 0x00001000 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_3_SHIFT       (13U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_3_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_HW_PAUSE_EN_3_SHIFT)       /* 0x00002000 */
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_3_SHIFT           (14U)
#define CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_3_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_0_CNT_CLR_3_SHIFT)           /* 0x00004000 */
/* CEVA_PFM_CTRL_1 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_4_SHIFT        (0U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_4_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_4_SHIFT)        /* 0x00000001 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_4_SHIFT       (1U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_4_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_4_SHIFT)       /* 0x00000002 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_4_SHIFT           (2U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_4_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_4_SHIFT)           /* 0x00000004 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_5_SHIFT        (4U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_5_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_5_SHIFT)        /* 0x00000010 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_5_SHIFT       (5U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_5_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_5_SHIFT)       /* 0x00000020 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_5_SHIFT           (6U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_5_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_5_SHIFT)           /* 0x00000040 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_6_SHIFT        (8U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_6_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_6_SHIFT)        /* 0x00000100 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_6_SHIFT       (9U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_6_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_6_SHIFT)       /* 0x00000200 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_6_SHIFT           (10U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_6_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_6_SHIFT)           /* 0x00000400 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_7_SHIFT        (12U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_7_MASK         (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_ENABLE_7_SHIFT)        /* 0x00001000 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_7_SHIFT       (13U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_7_MASK        (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_HW_PAUSE_EN_7_SHIFT)       /* 0x00002000 */
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_7_SHIFT           (14U)
#define CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_7_MASK            (0x1U << CEVA_CNT_CEVA_PFM_CTRL_1_CNT_CLR_7_SHIFT)           /* 0x00004000 */
/* CEVA_PFM_VERSION */
#define CEVA_CNT_CEVA_PFM_VERSION_PFM_VERSION_SHIFT        (0U)
#define CEVA_CNT_CEVA_PFM_VERSION_PFM_VERSION_MASK         (0xFFFFFFFFU << CEVA_CNT_CEVA_PFM_VERSION_PFM_VERSION_SHIFT) /* 0xFFFFFFFF */
/*****************************************TIMER******************************************/
/* LOAD_COUNT0 */
#define TIMER_LOAD_COUNT0_TIMER_LOAD_COUNT0_SHIFT          (0U)
#define TIMER_LOAD_COUNT0_TIMER_LOAD_COUNT0_MASK           (0xFFFFFFFFU << TIMER_LOAD_COUNT0_TIMER_LOAD_COUNT0_SHIFT)   /* 0xFFFFFFFF */
/* LOAD_COUNT1 */
#define TIMER_LOAD_COUNT1_TIMER_LOAD_COUNT1_SHIFT          (0U)
#define TIMER_LOAD_COUNT1_TIMER_LOAD_COUNT1_MASK           (0xFFFFFFFFU << TIMER_LOAD_COUNT1_TIMER_LOAD_COUNT1_SHIFT)   /* 0xFFFFFFFF */
/* CURRENT_VALUE0 */
#define TIMER_CURRENT_VALUE0_TIMER_CURRENT_VALUE0_SHIFT    (0U)
#define TIMER_CURRENT_VALUE0_TIMER_CURRENT_VALUE0_MASK     (0xFFFFFFFFU << TIMER_CURRENT_VALUE0_TIMER_CURRENT_VALUE0_SHIFT) /* 0xFFFFFFFF */
/* CURRENT_VALUE1 */
#define TIMER_CURRENT_VALUE1_TIMER_CURRENT_VALUE_SHIFT     (0U)
#define TIMER_CURRENT_VALUE1_TIMER_CURRENT_VALUE_MASK      (0xFFFFFFFFU << TIMER_CURRENT_VALUE1_TIMER_CURRENT_VALUE_SHIFT) /* 0xFFFFFFFF */
/* CONTROLREG */
#define TIMER_CONTROLREG_TIMER_ENABLE_SHIFT                (0U)
#define TIMER_CONTROLREG_TIMER_ENABLE_MASK                 (0x1U << TIMER_CONTROLREG_TIMER_ENABLE_SHIFT)                /* 0x00000001 */
#define TIMER_CONTROLREG_TIMER_MODE_SHIFT                  (1U)
#define TIMER_CONTROLREG_TIMER_MODE_MASK                   (0x1U << TIMER_CONTROLREG_TIMER_MODE_SHIFT)                  /* 0x00000002 */
#define TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT              (2U)
#define TIMER_CONTROLREG_TIMER_INT_MASK_MASK               (0x1U << TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT)              /* 0x00000004 */
/* INTSTATUS */
#define TIMER_INTSTATUS_INTSTATUS_SHIFT                    (0U)
#define TIMER_INTSTATUS_INTSTATUS_MASK                     (0x1U << TIMER_INTSTATUS_INTSTATUS_SHIFT)                    /* 0x00000001 */

#define TIMER_CHAN_CNT   2
/*****************************************CACHE*****************************************/
/* CACHE LINE SIZE */
#define CACHE_LINE_SHIFT                                   (6U)
#define CACHE_LINE_SIZE                                    (0x1U << CACHE_LINE_SHIFT)

/********Name=SOFTRST0_CON,Offset=0x180********/
#define SRST_COREPO_SRSTN    0U
#define SRST_CORE_SRSTN      1U
#define SRST_L2_SRSTN        2U
#define SRST_CPU_NIU_SRSTN   3U
#define SRST_COREPO_T_SRSTN  4U
#define SRST_CORE_T_SRSTN    5U
#define SRST_L2_T_SRSTN      6U
#define SRST_CORE_DBG_SRSTN  8U
#define SRST_DBG_PSRSTN      9U
#define SRST_DAP_PSRSTN      10U
#define SRST_DBG_NIU_PSRSTN  11U
#define SRST_STRC_SYS_ASRSTN 15U
/********Name=SOFTRST1_CON,Offset=0x184********/
#define SRST_DDRPHY_SRSTN_CLKDIV 16U
#define SRST_DDRPHY_SRSTN        17U
#define SRST_DDRPHY_PSRSTN       18U
#define SRST_HDMIPHY_PSRSTN      19U
#define SRST_VDACPHY_PSRSTN      20U
#define SRST_VADCPHY_PSRSTN      21U
#define SRST_MIPI_CSI_PHY_PSRSTN 22U
#define SRST_MIPI_DSI_PHY_PSRSTN 23U
#define SRST_ACODEC_PSRSTN       24U
#define SRST_BUS_NIU_ASRSTN      25U
#define SRST_TOP_NIU_PSRSTN      26U
#define SRST_INTMEM_ASRSTN       27U
#define SRST_ROM_HSRSTN          28U
#define SRST_DMAC_ASRSTN         29U
#define SRST_MSCH_NIU_SRSTN      30U
#define SRST_MSCH_NIU_PSRSTN     31U
/********Name=SOFTRST2_CON,Offset=0x188********/
#define SRST_DDRUPCTL_PSRSTN 32U
#define SRST_DDRUPCTL_NSRSTN 33U
#define SRST_DDRMON_PSRSTN   34U
#define SRST_I2S0_8CH_HSRSTN 35U
#define SRST_I2S0_8CH_MSRSTN 36U
#define SRST_I2S1_2CH_HSRSTN 37U
#define SRST_I2S1_2CH_MSRSTN 38U
#define SRST_I2S2_2CH_HSRSTN 39U
#define SRST_I2S2_2CH_MSRSTN 40U
#define SRST_CRYPTO_HSRSTN   41U
#define SRST_CRYPTO_SRSTN    42U
#define SRST_SPI_PSRSTN      43U
#define SRST_SPI_SRSTN       44U
#define SRST_UART0_PSRSTN    45U
#define SRST_UART1_PSRSTN    46U
#define SRST_UART2_PSRSTN    47U
/********Name=SOFTRST3_CON,Offset=0x18C********/
#define SRST_UART0_SRSTN  48U
#define SRST_UART1_SRSTN  49U
#define SRST_UART2_SRSTN  50U
#define SRST_I2C1_PSRSTN  51U
#define SRST_I2C2_PSRSTN  52U
#define SRST_I2C3_PSRSTN  53U
#define SRST_I2C1_SRSTN   54U
#define SRST_I2C2_SRSTN   55U
#define SRST_I2C3_SRSTN   56U
#define SRST_PWM1_PSRSTN  58U
#define SRST_PWM1_SRSTN   60U
#define SRST_WDT_PSRSTN   61U
#define SRST_GPIO1_PSRSTN 62U
#define SRST_GPIO2_PSRSTN 63U
/********Name=SOFTRST4_CON,Offset=0x190********/
#define SRST_GPIO3_PSRSTN    64U
#define SRST_GRF_PSRSTN      65U
#define SRST_EFUSE_PSRSTN    66U
#define SRST_EFUSE512_PSRSTN 67U
#define SRST_TIMER0_PSRSTN   68U
#define SRST_TIMER0_SRSTN    69U
#define SRST_TIMER1_SRSTN    70U
#define SRST_TSADC_PSRSTN    71U
#define SRST_TSADC_SRSTN     72U
#define SRST_SARADC_PSRSTN   73U
#define SRST_SARADC_SRSTN    74U
#define SRST_SYSBUS_HSRSTN   75U
#define SRST_USBGRF_PSRSTN   76U
/********Name=SOFTRST5_CON,Offset=0x194********/
#define SRST_PERIPH_NIU_ASRSTN 80U
#define SRST_PERIPH_NIU_HSRSTN 81U
#define SRST_PERIPH_NIU_PSRSTN 82U
#define SRST_PERIPH_HSRSTN     83U
#define SRST_SDMMC_HSRSTN      84U
#define SRST_SDIO_HSRSTN       85U
#define SRST_EMMC_HSRSTN       86U
#define SRST_NANDC_HSRSTN      87U
#define SRST_NANDC_NSRSTN      88U
#define SRST_SFC_HSRSTN        89U
#define SRST_SFC_SRSTN         90U
#define SRST_GMAC_ASRSTN       91U
#define SRST_OTG_HSRSTN        92U
#define SRST_OTG_SRSTN         93U
#define SRST_OTG_ADP_SRSTN     94U
#define SRST_HOST0_HSRSTN      95U
/********Name=SOFTRST6_CON,Offset=0x198********/
#define SRST_HOST0_AUX_HSRSTN    96U
#define SRST_HOST0_ARB_HSRSTN    97U
#define SRST_HOST0_EHCIPHY_SRSTN 98U
#define SRST_HOST0_UTMI_SRSTN    99U
#define SRST_USBPOR_SRSTN        100U
#define SRST_UTIM0_SRSTN         101U
#define SRST_UTMI1_SRSTN         102U
/********Name=SOFTRST7_CON,Offset=0x19C********/
#define SRST_VIO0_NIU_ASRSTN 112U
#define SRST_VIO1_NIU_ASRSTN 113U
#define SRST_VIO_NIU_HSRSTN  114U
#define SRST_VIO_NIU_PSRSTN  115U
#define SRST_VOP_ASRSTN      116U
#define SRST_VOP_HSRSTN      117U
#define SRST_VOP_DSRSTN      118U
#define SRST_IEP_ASRSTN      119U
#define SRST_IEP_HSRSTN      120U
#define SRST_RGA_ASRSTN      121U
#define SRST_RGA_HSRSTN      122U
#define SRST_RGA_SRSTN       123U
#define SRST_CVBS_PSRSTN     124U
#define SRST_HDMI_PSRSTN     125U
#define SRST_HDMI_SRSTN      126U
#define SRST_MIPI_DSI_PSRSTN 127U
/********Name=SOFTRST8_CON,Offset=0x1A0********/
#define SRST_ISP_NIU_ASRSTN 128U
#define SRST_ISP_NIU_HSRSTN 129U
#define SRST_ISP_HSRSTN     130U
#define SRST_ISP_SRSTN      131U
#define SRST_VIP0_ASRSTN    132U
#define SRST_VIP0_HSRSTN    133U
#define SRST_VIP0_PSRSTN    134U
#define SRST_VIP1_ASRSTN    135U
#define SRST_VIP1_HSRSTN    136U
#define SRST_VIP1_PSRSTN    137U
#define SRST_VIP2_ASRSTN    138U
#define SRST_VIP2_HSRSTN    139U
#define SRST_VIP2_PSRSTN    140U
#define SRST_VIP3_ASRSTN    141U
#define SRST_VIP3_HSRSTN    142U
#define SRST_VIP3_PSRSTN    143U
/********Name=SOFTRST9_CON,Offset=0x1A4********/
#define SRST_CIF1TO4_PSRSTN 144U
#define SRST_CVBS_CLK_SRSTN 145U
#define SRST_CVBS_HSRSTN    146U
/********Name=SOFTRST10_CON,Offset=0x1A8********/
#define SRST_VPU_NIU_ASRSTN     160U
#define SRST_VPU_NIU_HSRSTN     161U
#define SRST_VPU_ASRSTN         162U
#define SRST_VPU_HSRSTN         163U
#define SRST_RKVDEC_NIU_ASRSTN  164U
#define SRST_RKVDEC_NIU_HSRSTN  165U
#define SRST_RKVDEC_ASRSTN      166U
#define SRST_RKVDEC_HSRSTN      167U
#define SRST_RKVDEC_CABAC_SRSTN 168U
#define SRST_RKVDEC_CORE_SRSTN  169U
#define SRST_RKVENC_NIU_ASRSTN  170U
#define SRST_RKVENC_NIU_HSRSTN  171U
#define SRST_RKVENC_ASRSTN      172U
#define SRST_RKVENC_HSRSTN      173U
#define SRST_RKVENC_CORE_SRSTN  174U
/********Name=SOFTRST11_CON,Offset=0x1AC********/
#define SRST_DSP_CORE_SRSTN      176U
#define SRST_DSP_SYS_SRSTN       177U
#define SRST_DSP_GLOBAL_SRSTN    178U
#define SRST_DSP_OECM_SRSTN      179U
#define SRST_DSP_IOP_NIU_PSRSTN  180U
#define SRST_DSP_EPP_NIU_ASRSTN  181U
#define SRST_DSP_EDP_NIU_ASRSTN  182U
#define SRST_DSP_DBG_NIU_PSRSTN  183U
#define SRST_DSP_CFG_NIU_PSRSTN  184U
#define SRST_DSP_GRF_PSRSTN      185U
#define SRST_DSP_MAILBOX_PSRSTN  186U
#define SRST_DSP_INTC_PSRSTN     187U
#define SRST_DSP_PFM_MON_PSRSTN  189U
#define SRST_DSP_PFM_MON_SRSTN   190U
#define SRST_DSP_EDAP_NIU_ASRSTN 191U
/********Name=SOFTRST12_CON,Offset=0x1B0********/
#define SRST_PMU_SRSTN          192U
#define SRST_PMU_I2C0_SRSTN     193U
#define SRST_PMU_I2C0_PSRSTN    194U
#define SRST_PMU_GPIO0_PSRSTN   195U
#define SRST_PMU_INTMEM_PSRSTN  196U
#define SRST_PMU_PWM0_PSRSTN    197U
#define SRST_PMU_PWM0_SRSTN     198U
#define SRST_PMU_GRF_PSRSTN     199U
#define SRST_PMU_NIU_SRSTN      200U
#define SRST_PMU_PVTM_SRSTN     201U
#define SRST_DSP_EDP_PERF_ARSTN 204U
#define SRST_DSP_EPP_PERF_ARSTN 205U

/********Name=CLKGATE0_CON,Offset=0x120********/
#define CLK_CORE_APLL_GATE 0U
#define CLK_CORE_DPLL_GATE 1U
#define CLK_CORE_GPLL_GATE 2U
#define CLK_JTAG_GATE      3U
#define ACLK_CORE_GATE     4U
#define PCLK_DBG_GATE      5U
#define CLK_DDR_APLL_GATE  8U
#define CLK_DDR_DPLL_GATE  9U
#define CLK_DDR_GPLL_GATE  10U
#define CLK_DDRMON_GATE    11U
/********Name=CLKGATE1_CON,Offset=0x124********/
#define ACLK_BUS_GPLL_GATE 16U
#define ACLK_BUS_APLL_GATE 17U
#define ACLK_BUS_DPLL_GATE 18U
#define HCLK_BUS_GATE      20U
#define PCLK_BUS_SRC_GATE  21U
#define PCLK_BUS_GATE      22U
#define PCLK_TOP_GATE      23U
#define PCLK_DDR_GATE      24U
#define CLK_TIMER0_GATE    25U
#define CLK_TIMER1_GATE    26U
/********Name=CLKGATE2_CON,Offset=0x128********/
#define CLK_I2S0_SRC_GATE      32U
#define CLK_I2S0_FRAC_SRC_GATE 33U
#define CLK_I2S0_GATE          34U
#define CLK_I2S0_OUT_GATE      35U
#define CLK_I2S1_SRC_GATE      36U
#define CLK_I2S1_FRAC_SRC_GATE 37U
#define CLK_I2S1_GATE          38U
#define CLK_I2S2_SRC_GATE      40U
#define CLK_I2S2_FRAC_SRC_GATE 41U
#define CLK_I2S2_GATE          42U
#define CLK_CRYPTO_SRC_GATE    44U
/********Name=CLKGATE3_CON,Offset=0x12C********/
#define CLK_SPI_SRC_GATE        48U
#define CLK_UART0_SRC_GATE      49U
#define CLK_UART0_FRAC_SRC_GATE 50U
#define CLK_UART1_SRC_GATE      51U
#define CLK_UART1_FRAC_SRC_GATE 52U
#define CLK_UART2_SRC_GATE      53U
#define CLK_UART2_FRAC_SRC_GATE 54U
#define CLK_I2C1_SRC_GATE       55U
#define CLK_I2C2_SRC_GATE       56U
#define CLK_I2C3_SRC_GATE       57U
#define CLK_PWM1_SRC_GATE       58U
#define CLK_TSADC_SRC_GATE      59U
#define CLK_SARADC_SRC_GATE     60U
/********Name=CLKGATE4_CON,Offset=0x130********/
#define ACLK_PERI_DPLL_GATE 65U
#define ACLK_PERI_GPLL_GATE 66U
#define HCLK_PERIPH_GATE    68U
#define PCLK_PERIPH_GATE    69U
#define CLK_MAC_REF_GATE    70U
#define CLK_MAC_REFOUT_GATE 71U
#define CLK_MAC_RX_GATE     72U
#define CLK_MAC_SRC_GATE    74U
/********Name=CLKGATE5_CON,Offset=0x134********/
#define CLK_SDMMC_SRC_GATE 80U
#define CLK_EMMC_SRC_GATE  81U
#define CLK_SDIO_SRC_GATE  82U
#define CLK_NANDC_SRC_GATE 83U
#define CLK_SFC_SRC_GATE   84U
#define CLK_USBPHY_GATE    85U
/********Name=CLKGATE6_CON,Offset=0x138********/
#define ACLK_VIO0_SRC_GATE 96U
#define ACLK_VIO1_SRC_GATE 97U
#define CLK_ISP_SRC_GATE   99U
#define DCLK_VOP_DPLL_GATE 100U
#define DCLK_VOP_GPLL_GATE 101U
#define CLK_RGA_SRC_GATE   102U
#define CVBS_HOST_SRC_GATE 103U
#define HDMI_SFR_GATE      104U
#define HDMI_CEC_GATE      105U
/********Name=CLKGATE7_CON,Offset=0x13C********/
#define HCLK_VIO_GATE       114U
#define PCLK_VIO_GATE       115U
#define PCLK_ISP_GATE       118U
#define PCLKIN_CIF1TO4_GATE 120U
#define PCLKIN_VIP0_GATE    121U
#define PCLKIN_VIP1_GATE    122U
#define PCLKIN_VIP2_GATE    123U
#define PCLKIN_VIP3_GATE    124U
/********Name=CLKGATE8_CON,Offset=0x140********/
#define ACLK_RKVDEC_SRC_GATE    128U
#define CLK_HEVC_CABAC_SRC_GATE 129U
#define CLK_HEVC_CORE_SRC_GATE  130U
#define ACLK_VPU_SRC_GATE       131U
#define HCLK_RKVDEC_GATE        132U
#define ACLK_RKVENC_SRC_GATE    136U
#define CLK_VENC_CORE_SRC_GATE  137U
#define HCLK_RKVENC_GATE        138U
#define PCLK_PMU_GATE           140U
#define CLK_PVTM_PMU_GATE       141U
#define CLK_I2C0_PMU_SRC_GATE   142U
#define CLK_PWM0_PMU_SRC_GATE   143U
/********Name=CLKGATE9_CON,Offset=0x144********/
#define CLK_DSP_SRC_GATE          144U
#define CLK_DSP_IOP_GATE          145U
#define CLK_DSP_EPP_GATE          146U
#define CLK_DSP_EDP_GATE          147U
#define CLK_DSP_EDAP_GATE         148U
#define PCLK_DSP_SRC_GATE         149U
#define CLK_DSP_PFM_MON_GATE      150U
#define CLK_WIFI_SRC_GATE         152U
#define TESTCLK_GATE              153U
#define CLK_DSIPHY_24M_GATE       154U
#define CLK_CIF_OUT_SRC_GATE      155U
#define CLK_MIPI_CSI_OUT_SRC_GATE 156U
/********Name=CLKGATE10_CON,Offset=0x148********/
#define PCLK_INTMEM1_GATE 161U
#define PCLK_GPIO0_GATE   162U
#define PCLK_PMUGRF_GATE  163U
#define PCLK_PMU_NIU_GATE 164U
#define PCLK_I2C_PMU_GATE 165U
#define PCLK_PWM_PMU_GATE 166U
#define CLK_DDRPHY4X_GATE 169U
/********Name=CLKGATE11_CON,Offset=0x14C********/
#define ACLK_CORE_NIU_GATE     176U
#define PCLK_CORE_NIU_GATE     177U
#define ACLK_DSP_EDP_PERF_GATE 184U
/********Name=CLKGATE12_CON,Offset=0x150********/
#define CLK_INTMEM0_GATE    193U
#define ACLK_DMAC_BUS_GATE  194U
#define HCLK_ROM_GATE       195U
#define PCLK_DDR_UPCTL_GATE 196U
#define CLK_DDR_UPCTL_GATE  197U
#define PCLK_DDRMON_GATE    198U
#define HCLK_I2S0_8CH_GATE  199U
#define HCLK_I2S1_2CH_GATE  200U
#define HCLK_I2S2_2CH_GATE  201U
#define MCLK_CRYPTO_GATE    202U
#define SCLK_CRYPTO_GATE    203U
#define PCLK_EFUSE_0_GATE   204U
#define PCLK_EFUSE_1_GATE   205U
/********Name=CLKGATE13_CON,Offset=0x154********/
#define PCLK_I2C1_GATE    208U
#define PCLK_I2C2_GATE    209U
#define PCLK_I2C3_GATE    210U
#define PCLK_WDT_GATE     211U
#define PCLK_TIMER_GATE   212U
#define PCLK_SPI0_GATE    213U
#define PCLK_RK_PWM1_GATE 214U
#define PCLK_GPIO1_GATE   215U
#define PCLK_GPIO2_GATE   216U
#define PCLK_GPIO3_GATE   217U
#define PCLK_UART0_GATE   218U
#define PCLK_UART1_GATE   219U
#define PCLK_UART2_GATE   220U
#define PCLK_TSADC_GATE   221U
#define PCLK_SARADC_GATE  222U
/********Name=CLKGATE14_CON,Offset=0x158********/
#define PCLK_GRF_GATE          224U
#define ACLK_NIU_GATE          225U
#define PCLK_MSCHNIU_GATE      226U
#define PCLK_TOP_NIU_GATE      227U
#define PCLK_DDRPHY_GATE       228U
#define PCLK_CRU_GATE          229U
#define PCLK_ACODECPHY_GATE    230U
#define PCLK_HDMIPHY_GATE      232U
#define PCLK_VDACPHY_GATE      233U
#define PCLK_MIPI_DSI_PHY_GATE 235U
#define PCLK_MIPI_CSI_PHY_GATE 236U
#define PCLK_USBGRF_GATE       238U
/********Name=CLKGATE15_CON,Offset=0x15C********/
#define HCLK_SDMMC_GATE     240U
#define HCLK_SDIO_GATE      241U
#define HCLK_EMMC_GATE      242U
#define HCLK_NANDC_GATE     243U
#define ACLK_GMAC_GATE      244U
#define PCLK_GMAC_GATE      245U
#define HCLK_HOST0_GATE     246U
#define HCLK_HOST0_ARB_GATE 247U
#define HCLK_OTG_GATE       248U
#define HCLK_OTG_PMU_GATE   249U
#define HCLK_SFC_GATE       250U
#define ACLK_PERI_NIU_GATE  251U
#define HCLK_PERI_NIU_GATE  252U
#define PCLK_PERI_NIU_GATE  253U
/********Name=CLKGATE16_CON,Offset=0x160********/
#define CLK_DSP_SYS_WDOG_GATE  256U
#define CLK_DSP_EPP_WDOG_GATE  257U
#define CLK_DSP_EDP_WDOG_GATE  258U
#define CLK_DSP_IOP_WDOG_GATE  259U
#define PCLK_DSP_IOP_NIU_GATE  260U
#define ACLK_DSP_EPP_NIU_GATE  261U
#define ACLK_DSP_EDP_NIU_GATE  262U
#define PCLK_DSP_DBG_NIU_GATE  263U
#define PCLK_DSP_CFG_NIU_GATE  264U
#define PCLK_DSP_PFM_MON_GATE  265U
#define PCLK_INTC_GATE         266U
#define PCLK_DSP_GRF_GATE      267U
#define PCLK_MAILBOX_GATE      268U
#define CLK_DSP_FREE_GATE      269U
#define ACLK_DSP_EDAP_NIU_GATE 270U
#define ACLK_DSP_EPP_PERF_GATE 271U
/********Name=CLKGATE17_CON,Offset=0x164********/
#define ACLK_VIO0_GATE 272U
#define ACLK_VIO1_GATE 273U
#define ACLK_CIF1_GATE 278U
#define HCLK_CIF1_GATE 279U
#define ACLK_CIF2_GATE 280U
#define HCLK_CIF2_GATE 281U
#define ACLK_CIF3_GATE 282U
#define HCLK_CIF3_GATE 283U
#define HCLK_CVBS_GATE 284U
/********Name=CLKGATE18_CON,Offset=0x168********/
#define ACLK_VOP_GATE        288U
#define HCLK_VOP_GATE        289U
#define ACLK_IEP_GATE        290U
#define HCLK_IEP_GATE        291U
#define ACLK_RGA_GATE        292U
#define HCLK_RGA_GATE        293U
#define ACLK_CIF0_GATE       294U
#define HCLK_CIF0_GATE       295U
#define PCLK_MIPI_DSI0_GATE  296U
#define PCLK_HDMI_CTRL_GATE  297U
#define PCLKIN_ISP_CTRL_GATE 298U
#define HCLK_ISP_CTRL_GATE   299U
#define ACLK_ISP_CTRL_GATE   300U
/********Name=CLKGATE19_CON,Offset=0x16C********/
#define ACLK_RKVDEC_GATE     304U
#define ACLK_VPU_GATE        305U
#define HCLK_VPU_GATE        307U
#define ACLK_RKVDEC_NIU_GATE 308U
#define HCLK_RKVDEC_NIU_GATE 309U
#define ACLK_VPU_NIU_GATE    310U
#define ACLK_RKVENC_GATE     312U
#define HCLK_RKVENC_NIU_GATE 314U
#define ACLK_RKVENC_NIU_GATE 315U

/********Name=CLKSEL0_CON,Offset=0x60********/
#define CLK_CORE_DIV 0x05000000U
/********Name=CLKSEL1_CON,Offset=0x64********/
#define ACLK_CORE_DIV 0x03000001U
#define PCLK_DBG_DIV  0x04040001U
/********Name=CLKSEL2_CON,Offset=0x68********/
#define ACLK_BUS_DIV 0x05000002U
/********Name=CLKSEL3_CON,Offset=0x6C********/
#define HCLK_BUS_DIV 0x05000003U
#define PCLK_BUS_DIV 0x05080003U
/********Name=CLKSEL4_CON,Offset=0x70********/
#define CLK_DDR_DIV 0x03000004U
/********Name=CLKSEL5_CON,Offset=0x74********/
#define CLK_I2S0_PLL_DIV 0x07000005U
/********Name=CLKSEL6_CON,Offset=0x78********/
#define CLK_I2S1_PLL_DIV 0x07000006U
/********Name=CLKSEL7_CON,Offset=0x7C********/
#define CLK_I2S2_PLL_DIV 0x07000007U
/********Name=CLKSEL8_CON,Offset=0x80********/
#define I2S0_FRAC_DIV 0x20000008U
/********Name=CLKSEL9_CON,Offset=0x84********/
#define I2S1_FRAC_DIV 0x20000009U
/********Name=CLKSEL10_CON,Offset=0x88********/
#define I2S2_FRAC_DIV 0x2000000AU
/********Name=CLKSEL11_CON,Offset=0x8C********/
#define CLK_CRYPTO_DIV 0x0500000BU
#define CLK_SPI_DIV    0x0708000BU
/********Name=CLKSEL12_CON,Offset=0x90********/
#define PWM0_PMU_DIV 0x0700000CU
#define PWM1_DIV     0x0708000CU
/********Name=CLKSEL13_CON,Offset=0x94********/
#define CLK_UART0_DIV 0x0700000DU
/********Name=CLKSEL14_CON,Offset=0x98********/
#define CLK_UART1_DIV 0x0700000EU
/********Name=CLKSEL15_CON,Offset=0x9C********/
#define CLK_UART2_DIV 0x0700000FU
/********Name=CLKSEL16_CON,Offset=0xA0********/
#define UART0_FRAC_DIV 0x20000010U
/********Name=CLKSEL17_CON,Offset=0xA4********/
#define UART1_FRAC_DIV 0x20000011U
/********Name=CLKSEL18_CON,Offset=0xA8********/
#define UART2_FRAC_DIV 0x20000012U
/********Name=CLKSEL19_CON,Offset=0xAC********/
#define I2C0_PMU_DIV 0x07000013U
#define I2C1_DIV     0x07080013U
/********Name=CLKSEL20_CON,Offset=0xB0********/
#define I2C2_DIV 0x07000014U
#define I2C3_DIV 0x07080014U
/********Name=CLKSEL21_CON,Offset=0xB4********/
#define TSADC_DIV 0x0A000015U
/********Name=CLKSEL22_CON,Offset=0xB8********/
#define SARADC_DIV 0x0A000016U
/********Name=CLKSEL23_CON,Offset=0xBC********/
#define ACLK_PERI_DIV 0x05000017U
#define HCLK_PERI_DIV 0x05050017U
#define PCLK_PERI_DIV 0x050A0017U
/********Name=CLKSEL24_CON,Offset=0xC0********/
#define MAC_DIV 0x05000018U
/********Name=CLKSEL25_CON,Offset=0xC4********/
#define CLK_SDMMC_DIV 0x08000019U
/********Name=CLKSEL26_CON,Offset=0xC8********/
#define CLK_SDIO_DIV 0x0800001AU
#define CLK_EMMC_DIV 0x0808001AU
/********Name=CLKSEL27_CON,Offset=0xCC********/
#define CLK_SFC_DIV   0x0700001BU
#define CLK_NANDC_DIV 0x0508001BU
/********Name=CLKSEL28_CON,Offset=0xD0********/
#define ACLK_VIO0_DIV 0x0500001CU
#define ACLK_VIO1_DIV 0x0508001CU
/********Name=CLKSEL29_CON,Offset=0xD4********/
#define HCLK_VIO_DIV 0x0500001DU
#define PCLK_VIO_DIV 0x0508001DU
/********Name=CLKSEL30_CON,Offset=0xD8********/
#define CLK_ISP_DIV 0x0508001EU
/********Name=CLKSEL31_CON,Offset=0xDC********/
/********Name=CLKSEL32_CON,Offset=0xE0********/
#define DCLK_VOP_DIV 0x06000020U
#define HDMIPHY_DIV  0x06080020U
/********Name=CLKSEL33_CON,Offset=0xE4********/
#define CLK_RGA_DIV       0x05000021U
#define CLK_CVBS_HOST_DIV 0x05080021U
/********Name=CLKSEL34_CON,Offset=0xE8********/
#define HDMI_CEC_DIV 0x0E000022U
/********Name=CLKSEL35_CON,Offset=0xEC********/
#define ACLK_RKVDEC_DIV 0x05000023U
#define HEVC_CABAC_DIV  0x05080023U
/********Name=CLKSEL36_CON,Offset=0xF0********/
#define HEVC_CORE_DIV 0x05000024U
#define ACLK_VPU_DIV  0x05080024U
/********Name=CLKSEL37_CON,Offset=0xF4********/
#define ACLK_RKVENC_DIV 0x05000025U
#define VENC_CORE_DIV   0x05080025U
/********Name=CLKSEL38_CON,Offset=0xF8********/
#define PCLK_PMU_DIV 0x05000026U
/********Name=CLKSEL39_CON,Offset=0xFC********/
#define TEST_DIV 0x05080027U
/********Name=CLKSEL40_CON,Offset=0x100********/
#define CLK_CIF_OUT_DIV 0x05000028U
/********Name=CLKSEL41_CON,Offset=0x104********/
#define CSI_CLK_OUT_DIV 0x05000029U
#define WIFI_DIV        0x06080029U
/********Name=CLKSEL42_CON,Offset=0x108********/
#define CLK_DSP_DIV 0x0500002AU
/********Name=CLKSEL43_CON,Offset=0x10C********/
#define CLK_DSP_PFM_DIV 0x0500002BU
#define PCLK_DSP_DIV    0x0508002BU
/********Name=CLKSEL44_CON,Offset=0x110********/
#define DSP_IOP_DIV 0x0500002CU
#define DSP_EPP_DIV 0x0508002CU
/********Name=CLKSEL45_CON,Offset=0x114********/
#define DSP_EDP_DIV  0x0500002DU
#define DSP_EDAP_DIV 0x0508002DU

/********Name=CLKSEL0_CON,Offset=0x60********/
#define CLK_CORE_PLL_SEL          0x02080000U
#define CLK_CORE_PLL_SEL_APLL     0U
#define CLK_CORE_PLL_SEL_GPLL     1U
#define CLK_CORE_PLL_SEL_DPLL     2U
#define CLK_CORE_PLL_SEL_RESERVED 3U
/********Name=CLKSEL1_CON,Offset=0x64********/
/********Name=CLKSEL2_CON,Offset=0x68********/
#define ACLK_BUS_PLL_SEL          0x02080002U
#define ACLK_BUS_PLL_SEL_GPLL     0U
#define ACLK_BUS_PLL_SEL_APLL     1U
#define ACLK_BUS_PLL_SEL_DPLL     2U
#define ACLK_BUS_PLL_SEL_RESERVED 3U
/********Name=CLKSEL3_CON,Offset=0x6C********/
/********Name=CLKSEL4_CON,Offset=0x70********/
#define CLK_DDR_PLL_SEL          0x02080004U
#define CLK_DDR_PLL_SEL_DPLL     0U
#define CLK_DDR_PLL_SEL_APLL     1U
#define CLK_DDR_PLL_SEL_GPLL     2U
#define CLK_DDR_PLL_SEL_RESERVED 3U
/********Name=CLKSEL5_CON,Offset=0x74********/
#define CLK_I2S0_PLL_SEL         0x01080005U
#define CLK_I2S0_PLL_SEL_DPLL    0U
#define CLK_I2S0_PLL_SEL_GPLL    1U
#define CLK_I2S0_SEL             0x020C0005U
#define CLK_I2S0_SEL_DIVOUT      0U
#define CLK_I2S0_SEL_FRAC_DIVOUT 1U
#define I2S0_OUT_SEL             0x010F0005U
#define I2S0_OUT_SEL_CLK_I2S0    0U
#define I2S0_OUT_SEL_12M         1U
/********Name=CLKSEL6_CON,Offset=0x78********/
#define CLK_I2S1_PLL_SEL         0x01080006U
#define CLK_I2S1_PLL_SEL_DPLL    0U
#define CLK_I2S1_PLL_SEL_GPLL    1U
#define CLK_I2S1_SEL             0x020C0006U
#define CLK_I2S1_SEL_DIVOUT      0U
#define CLK_I2S1_SEL_FRAC_DIVOUT 1U
#define CLK_I2S1_SEL_RESERVED    2U
#define CLK_I2S1_SEL_CLKIN       3U
/********Name=CLKSEL7_CON,Offset=0x7C********/
#define CLK_I2S2_PLL_SEL         0x01080007U
#define CLK_I2S2_PLL_SEL_DPLL    0U
#define CLK_I2S2_PLL_SEL_GPLL    1U
#define CLK_I2S2_SEL             0x020C0007U
#define CLK_I2S2_SEL_DIVOUT      0U
#define CLK_I2S2_SEL_FRAC_DIVOUT 1U
#define CLK_I2S2_SEL_RESERVED    2U
#define CLK_I2S2_SEL_CLKIN       3U
/********Name=CLKSEL8_CON,Offset=0x80********/
/********Name=CLKSEL9_CON,Offset=0x84********/
/********Name=CLKSEL10_CON,Offset=0x88********/
/********Name=CLKSEL11_CON,Offset=0x8C********/
#define CLK_CRYPTO_PLL_SEL      0x0107000BU
#define CLK_CRYPTO_PLL_SEL_DPLL 0U
#define CLK_CRYPTO_PLL_SEL_GPLL 1U
#define CLK_SPI_PLL_SEL         0x010F000BU
#define CLK_SPI_PLL_SEL_DPLL    0U
#define CLK_SPI_PLL_SEL_GPLL    1U
/********Name=CLKSEL12_CON,Offset=0x90********/
#define PWM0_PMU_PLL_SEL      0x0107000CU
#define PWM0_PMU_PLL_SEL_DPLL 0U
#define PWM0_PMU_PLL_SEL_GPLL 1U
#define PWM1_PLL_SEL          0x010F000CU
#define PWM1_PLL_SEL_DPLL     0U
#define PWM1_PLL_SEL_GPLL     1U
/********Name=CLKSEL13_CON,Offset=0x94********/
#define CLK_UART0_SEL       0x0208000DU
#define CLK_UART0_SEL_INPUT 2U
#define UART0_PLL_SEL       0x020C000DU
/********Name=CLKSEL14_CON,Offset=0x98********/
#define CLK_UART1_SEL       0x0208000EU
#define CLK_UART1_SEL_INPUT 2U
#define UART1_PLL_SEL       0x020C000EU
/********Name=CLKSEL15_CON,Offset=0x9C********/
#define CLK_UART2_SEL       0x0208000FU
#define CLK_UART2_SEL_INPUT 2U
#define UART2_PLL_SEL       0x020C000FU
/********Name=CLKSEL16_CON,Offset=0xA0********/
/********Name=CLKSEL17_CON,Offset=0xA4********/
/********Name=CLKSEL18_CON,Offset=0xA8********/
/********Name=CLKSEL19_CON,Offset=0xAC********/
#define I2C0_PMU_PLL_SEL      0x01070013U
#define I2C0_PMU_PLL_SEL_DPLL 0U
#define I2C0_PMU_PLL_SEL_GPLL 1U
#define I2C1_PLL_SEL          0x010F0013U
#define I2C1_PLL_SEL_DPLL     0U
#define I2C1_PLL_SEL_GPLL     1U
/********Name=CLKSEL20_CON,Offset=0xB0********/
#define I2C2_PLL_SEL      0x01070014U
#define I2C2_PLL_SEL_DPLL 0U
#define I2C2_PLL_SEL_GPLL 1U
#define I2C3_PLL_SEL      0x010F0014U
#define I2C3_PLL_SEL_DPLL 0U
#define I2C3_PLL_SEL_GPLL 1U
/********Name=CLKSEL21_CON,Offset=0xB4********/
/********Name=CLKSEL22_CON,Offset=0xB8********/
/********Name=CLKSEL23_CON,Offset=0xBC********/
#define ACLK_PERI_PLL_SEL      0x010F0017U
#define ACLK_PERI_PLL_SEL_GPLL 0U
#define ACLK_PERI_PLL_SEL_DPLL 1U
/********Name=CLKSEL24_CON,Offset=0xC0********/
#define RMII_EXTCLK_SEL     0x01080018U
#define RMII_EXTCLK_SEL_IO  0U
#define RMII_EXTCLK_SEL_PLL 1U
#define MAC_PLL_SEL         0x010C0018U
#define MAC_PLL_SEL_APLL    0U
#define MAC_PLL_SEL_GPLL    1U
/********Name=CLKSEL25_CON,Offset=0xC4********/
#define CLK_SDMMC_PLL_SEL             0x02080019U
#define CLK_SDMMC_PLL_SEL_DPLL        0U
#define CLK_SDMMC_PLL_SEL_GPLL        1U
#define CLK_SDMMC_PLL_SEL_XIN_OSC0    2U
#define CLK_SDMMC_PLL_SEL_USBPHY_480M 3U
#define CLK_SDIO_PLL_SEL              0x020A0019U
#define CLK_SDIO_PLL_SEL_DPLL         0U
#define CLK_SDIO_PLL_SEL_GPLL         1U
#define CLK_SDIO_PLL_SEL_XIN_OSC0     2U
#define CLK_SDIO_PLL_SEL_USBPHY_480M  3U
#define CLK_EMMC_PLL_SEL              0x020C0019U
#define CLK_EMMC_PLL_SEL_DPLL         0U
#define CLK_EMMC_PLL_SEL_GPLL         1U
#define CLK_EMMC_PLL_SEL_XIN_OSC0     2U
#define CLK_EMMC_PLL_SEL_USBPHY_480M  3U
/********Name=CLKSEL26_CON,Offset=0xC8********/
/********Name=CLKSEL27_CON,Offset=0xCC********/
#define CLK_SFC_PLL_SEL        0x0107001BU
#define CLK_SFC_PLL_SEL_DPLL   0U
#define CLK_SFC_PLL_SEL_GPLL   1U
#define CLK_NANDC_PLL_SEL      0x010E001BU
#define CLK_NANDC_PLL_SEL_DPLL 0U
#define CLK_NANDC_PLL_SEL_GPLL 1U
/********Name=CLKSEL28_CON,Offset=0xD0********/
#define ACLK_VIO0_PLL_SEL             0x0206001CU
#define ACLK_VIO0_PLL_SEL_DPLL        0U
#define ACLK_VIO0_PLL_SEL_GPLL        1U
#define ACLK_VIO0_PLL_SEL_HDMIPHY_PLL 2U
#define ACLK_VIO0_PLL_SEL_USBPHY_480M 3U
#define ACLK_VIO1_PLL_SEL             0x020E001CU
#define ACLK_VIO1_PLL_SEL_DPLL        0U
#define ACLK_VIO1_PLL_SEL_GPLL        1U
#define ACLK_VIO1_PLL_SEL_HDMIPHY_PLL 2U
#define ACLK_VIO1_PLL_SEL_USBPHY_480M 3U
/********Name=CLKSEL29_CON,Offset=0xD4********/
/********Name=CLKSEL30_CON,Offset=0xD8********/
#define CLK_ISP_PLL_SEL             0x020E001EU
#define CLK_ISP_PLL_SEL_DPLL        0U
#define CLK_ISP_PLL_SEL_GPLL        1U
#define CLK_ISP_PLL_SEL_HDMIPHY_PLL 2U
#define CLK_ISP_PLL_SEL_USBPHY_480M 3U
/********Name=CLKSEL31_CON,Offset=0xDC********/
#define VIP0_CLK_SEL         0x0200001FU
#define VIP0_CLK_SEL_IO      0U
#define VIP0_CLK_SEL_CIF1TO4 1U
#define VIP1_CLK_SEL         0x0202001FU
#define VIP1_CLK_SEL_IO      0U
#define VIP1_CLK_SEL_CIF1TO4 1U
#define VIP2_CLK_SEL         0x0204001FU
#define VIP2_CLK_SEL_IO      0U
#define VIP2_CLK_SEL_CIF1TO4 1U
#define VIP3_CLK_SEL         0x0206001FU
#define VIP3_CLK_SEL_IO      0U
#define VIP3_CLK_SEL_CIF1TO4 1U
#define PCLKIN_INV_SEL       0x0108001FU
/********Name=CLKSEL32_CON,Offset=0xE0********/
#define DCLK_VOP_PLL_SEL           0x01060020U
#define DCLK_VOP_PLL_SEL_GPLL      0U
#define DCLK_VOP_PLL_SEL_DPLL      1U
#define DCLK_VOP_CLK_SEL           0x01070020U
#define HDMIPHY_24M_SEL            0x010F0020U
#define HDMIPHY_24M_SEL_HDMIPHYPLL 0U
#define HDMIPHY_24M_SEL_24M        1U
/********Name=CLKSEL33_CON,Offset=0xE4********/
#define CLK_RGA_PLL_SEL                     0x02060021U
#define CLK_RGA_PLL_SEL_DPLL                0U
#define CLK_RGA_PLL_SEL_GPLL                1U
#define CLK_RGA_PLL_SEL_HDMIPHY_PLL         2U
#define CLK_RGA_PLL_SEL_USBPHY_480M         3U
#define CLK_CVBS_HOST_PLL_SEL               0x020D0021U
#define CLK_CVBS_HOST_PLL_SEL_APLL          0U
#define CLK_CVBS_HOST_PLL_SEL_IO_CVBS_CLKIN 1U
#define CLK_CVBS_HOST_PLL_SEL_HDMIPHY_PLL   2U
#define CLK_CVBS_HOST_PLL_SEL_GPLL          3U
/********Name=CLKSEL34_CON,Offset=0xE8********/
#define HDMI_CEC_PLL_SEL         0x020E0022U
#define HDMI_CEC_PLL_SEL_DPLL    0U
#define HDMI_CEC_PLL_SEL_GPLL    1U
#define HDMI_CEC_PLL_SEL_XIN_24M 2U
/********Name=CLKSEL35_CON,Offset=0xEC********/
#define ACLK_RKVDEC_PLL_SEL             0x02060023U
#define ACLK_RKVDEC_PLL_SEL_DPLL        0U
#define ACLK_RKVDEC_PLL_SEL_GPLL        1U
#define ACLK_RKVDEC_PLL_SEL_HDMIPHY_PLL 2U
#define ACLK_RKVDEC_PLL_SEL_USBPHY_480M 3U
#define HEVC_CABAC_PLL_SEL              0x020E0023U
#define HEVC_CABAC_PLL_SEL_DPLL         0U
#define HEVC_CABAC_PLL_SEL_GPLL         1U
#define HEVC_CABAC_PLL_SEL_HDMIPHY_PLL  2U
#define HEVC_CABAC_PLL_SEL_USBPHY_480M  3U
/********Name=CLKSEL36_CON,Offset=0xF0********/
#define HEVC_CORE_PLL_SEL             0x02060024U
#define HEVC_CORE_PLL_SEL_DPLL        0U
#define HEVC_CORE_PLL_SEL_GPLL        1U
#define HEVC_CORE_PLL_SEL_HDMIPHY_PLL 2U
#define HEVC_CORE_PLL_SEL_USBPHY_480M 3U
#define ACLK_VPU_PLL_SEL              0x020E0024U
#define ACLK_VPU_PLL_SEL_DPLL         0U
#define ACLK_VPU_PLL_SEL_GPLL         1U
#define ACLK_VPU_PLL_SEL_HDMIPHY_PLL  2U
#define ACLK_VPU_PLL_SEL_USBPHY_480M  3U
/********Name=CLKSEL37_CON,Offset=0xF4********/
#define ACLK_RKVENC_PLL_SEL             0x02060025U
#define ACLK_RKVENC_PLL_SEL_DPLL        0U
#define ACLK_RKVENC_PLL_SEL_GPLL        1U
#define ACLK_RKVENC_PLL_SEL_HDMIPHY_PLL 2U
#define ACLK_RKVENC_PLL_SEL_USBPHY_480M 3U
#define VENC_CORE_PLL_SEL               0x020E0025U
#define VENC_CORE_PLL_SEL_DPLL          0U
#define VENC_CORE_PLL_SEL_GPLL          1U
#define VENC_CORE_PLL_SEL_HDMIPHY_PLL   2U
#define VENC_CORE_PLL_SEL_USBPHY_480M   3U
/********Name=CLKSEL38_CON,Offset=0xF8********/
/********Name=CLKSEL39_CON,Offset=0xFC********/
/********Name=CLKSEL40_CON,Offset=0x100********/
#define CLK_CIF_OUT_PLL_SEL            0x01080028U
#define CLK_CIF_OUT_PLL_SEL_HDMIPHYPLL 0U
#define CLK_CIF_OUT_PLL_SEL_GPLL       1U
#define CLK_CIF_OUT_SEL                0x010C0028U
#define CLK_CIF_OUT_SEL_CLK_CIF_SRC    0U
#define CLK_CIF_OUT_SEL_XIN_OSC0       1U
/********Name=CLKSEL41_CON,Offset=0x104********/
#define CLK_WIFI_SEL         0x010F0029U
#define CLK_WIFI_SEL_GPLL    0U
#define CLK_WIFI_SEL_XIN_24M 1U
/********Name=CLKSEL42_CON,Offset=0x108********/
#define CLK_DSP_PLL_SEL             0x0208002AU
#define CLK_DSP_PLL_SEL_DPLL        0U
#define CLK_DSP_PLL_SEL_GPLL        1U
#define CLK_DSP_PLL_SEL_APLL        2U
#define CLK_DSP_PLL_SEL_USBPHY_480M 3U
/********Name=CLKSEL43_CON,Offset=0x10C********/
/********Name=CLKSEL44_CON,Offset=0x110********/
/********Name=CLKSEL45_CON,Offset=0x114********/

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_DPLL,
    PLL_GPLL,
    CLK_CPU         = CLK(CLK_CORE_PLL_SEL, CLK_CORE_DIV),
    PCLK_DBG        = CLK(0U, PCLK_DBG_DIV),
    ACLK_BUS        = CLK(ACLK_BUS_PLL_SEL, ACLK_BUS_DIV),
    HCLK_BUS        = CLK(0U, HCLK_BUS_DIV),
    PCLK_BUS        = CLK(0U, PCLK_BUS_DIV),
    CLK_I2S0_PLL    = CLK(CLK_I2S0_PLL_SEL, CLK_I2S0_PLL_DIV),
    CLK_I2S0_FRAC   = CLK(0U, I2S0_FRAC_DIV),
    CLK_I2S0        = CLK(CLK_I2S0_SEL, 0U),
    CLK_I2S0_OUT    = CLK(I2S0_OUT_SEL, 0U),
    CLK_I2S1_PLL    = CLK(CLK_I2S1_PLL_SEL, CLK_I2S1_PLL_DIV),
    CLK_I2S1_FRAC   = CLK(0U, I2S1_FRAC_DIV),
    CLK_I2S1        = CLK(CLK_I2S1_SEL, 0U),
    CLK_I2S2_PLL    = CLK(CLK_I2S2_PLL_SEL, CLK_I2S2_PLL_DIV),
    CLK_I2S2_FRAC   = CLK(0U, I2S2_FRAC_DIV),
    CLK_I2S2        = CLK(CLK_I2S2_SEL, 0U),
    CLK_CRYPTO      = CLK(CLK_CRYPTO_PLL_SEL, CLK_CRYPTO_DIV),
    CLK_SPI         = CLK(CLK_SPI_PLL_SEL, CLK_SPI_DIV),
    CLK_PWM0_PMU    = CLK(PWM0_PMU_PLL_SEL, PWM0_PMU_DIV),
    CLK_PWM1        = CLK(PWM1_PLL_SEL, PWM1_DIV),
    CLK_UART0_PLL   = CLK(UART0_PLL_SEL, CLK_UART0_DIV),
    CLK_UART0_FRAC  = CLK(0U, UART0_FRAC_DIV),
    CLK_UART0       = CLK(CLK_UART0_SEL, 0U),
    CLK_UART1_PLL   = CLK(UART1_PLL_SEL, CLK_UART1_DIV),
    CLK_UART1_FRAC  = CLK(0U, UART0_FRAC_DIV),
    CLK_UART1       = CLK(CLK_UART1_SEL, 0U),
    CLK_UART2_PLL   = CLK(UART2_PLL_SEL, CLK_UART2_DIV),
    CLK_UART2_FRAC  = CLK(0U, UART2_FRAC_DIV),
    CLK_UART2       = CLK(CLK_UART2_SEL, 0U),
    CLK_I2C0_PMU    = CLK(I2C0_PMU_PLL_SEL, I2C0_PMU_DIV),
    CLK_I2C1        = CLK(I2C1_PLL_SEL, I2C1_DIV),
    CLK_I2C2        = CLK(I2C2_PLL_SEL, I2C2_DIV),
    CLK_I2C3        = CLK(I2C3_PLL_SEL, I2C3_DIV),
    CLK_TSADC       = CLK(0U, TSADC_DIV),
    CLK_SARADC      = CLK(0U, SARADC_DIV),
    ACLK_PERI       = CLK(ACLK_PERI_PLL_SEL, ACLK_PERI_DIV),
    HCLK_PERI       = CLK(0U, HCLK_PERI_DIV),
    PCLK_PERI       = CLK(0U, PCLK_PERI_DIV),
    CLK_MAC         = CLK(MAC_PLL_SEL, MAC_DIV),
    RMII_EXTCLK     = CLK(RMII_EXTCLK_SEL, 0U),
    CLK_SDMMC       = CLK(CLK_SDMMC_PLL_SEL, CLK_SDMMC_DIV),
    CLK_SDIO        = CLK(CLK_SDIO_PLL_SEL, CLK_SDIO_DIV),
    CLK_EMMC        = CLK(CLK_EMMC_PLL_SEL, CLK_EMMC_DIV),
    CLK_SFC         = CLK(CLK_SFC_PLL_SEL, CLK_SFC_DIV),
    CLK_NANDC       = CLK(CLK_NANDC_PLL_SEL, CLK_NANDC_DIV),
    ACLK_VIO0       = CLK(ACLK_VIO0_PLL_SEL, ACLK_VIO0_DIV),
    ACLK_VIO1       = CLK(ACLK_VIO1_PLL_SEL, ACLK_VIO1_DIV),
    HCLK_VIO        = CLK(0U, HCLK_VIO_DIV),
    PCLK_VIO        = CLK(0U, PCLK_VIO_DIV),
    CLK_ISP         = CLK(CLK_ISP_PLL_SEL, CLK_ISP_DIV),
    DCLK_VOP_PLL    = CLK(DCLK_VOP_PLL_SEL, DCLK_VOP_DIV),
    DCLK_VOP        = CLK(DCLK_VOP_CLK_SEL, 0U),
    CLK_RGA         = CLK(CLK_RGA_PLL_SEL, CLK_RGA_DIV),
    CLK_CVBS_HOST   = CLK(CLK_CVBS_HOST_PLL_SEL, CLK_CVBS_HOST_DIV),
    CLK_HDMI_CEC    = CLK(HDMI_CEC_PLL_SEL, HDMI_CEC_DIV),
    ACLK_RKVDEC     = CLK(ACLK_RKVDEC_PLL_SEL, ACLK_RKVDEC_DIV),
    CLK_HEVC_CABAC  = CLK(HEVC_CABAC_PLL_SEL, HEVC_CABAC_DIV),
    CLK_HEVC_CORE   = CLK(HEVC_CORE_PLL_SEL, HEVC_CORE_DIV),
    ACLK_VPU        = CLK(ACLK_VPU_PLL_SEL, ACLK_VPU_DIV),
    ACLK_RKVENC     = CLK(ACLK_RKVENC_PLL_SEL, ACLK_RKVENC_DIV),
    CLK_VENC_CORE   = CLK(VENC_CORE_PLL_SEL, VENC_CORE_DIV),
    CLK_CIF_OUT_PLL = CLK(CLK_CIF_OUT_PLL_SEL, 0U),
    CLK_CIF_OUT_SRC = CLK(CLK_CIF_OUT_SEL, 0U),
    CLK_CIF_OUT     = CLK(0U, CLK_CIF_OUT_DIV),
    CLK_CSI_OUT     = CLK(0U, CSI_CLK_OUT_DIV),
    CLK_WIFI        = CLK(CLK_WIFI_SEL, WIFI_DIV),
    CLK_DSP         = CLK(CLK_DSP_PLL_SEL, CLK_DSP_DIV),
    CLK_DSP_PFM     = CLK(0U, CLK_DSP_PFM_DIV),
    PCLK_DSP        = CLK(0U, PCLK_DSP_DIV),
    DSP_IOP         = CLK(0U, DSP_IOP_DIV),
    DSP_EPP         = CLK(0U, DSP_EPP_DIV),
    DSP_EDP         = CLK(0U, DSP_EDP_DIV),
    DSP_EDAP        = CLK(0U, DSP_EDAP_DIV),
} eCLOCK_Name;
#endif /* __ASSEMBLY__ */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RV1108_H */


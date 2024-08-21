/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __SWALLOW_H
#define __SWALLOW_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* CRU Register Structure Define */
struct CRU_REG {
    __IO uint32_t GPLL_CON[5];                        /* Address Offset: 0x0000 */
         uint32_t RESERVED0014[35];                   /* Address Offset: 0x0014 */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x00A0 */
         uint32_t RESERVED00A4[3];                    /* Address Offset: 0x00A4 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x00B0 */
    __IO uint32_t GLB_RST_ST;                         /* Address Offset: 0x00B4 */
    __IO uint32_t GLB_SRST_FST_VALUE;                 /* Address Offset: 0x00B8 */
    __IO uint32_t GLB_SRST_SND_VALUE;                 /* Address Offset: 0x00BC */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x00C0 */
         uint32_t RESERVED00C4[15];                   /* Address Offset: 0x00C4 */
    __IO uint32_t CRU_CLKSEL_CON[12];                 /* Address Offset: 0x0100 */
         uint32_t RESERVED0130[116];                  /* Address Offset: 0x0130 */
    __IO uint32_t CRU_CLKGATE_CON[7];                 /* Address Offset: 0x0300 */
         uint32_t RESERVED031C[25];                   /* Address Offset: 0x031C */
    __IO uint32_t SSCGTBL0_3;                         /* Address Offset: 0x0380 */
    __IO uint32_t SSCGTBL4_7;                         /* Address Offset: 0x0384 */
    __IO uint32_t SSCGTBL8_11;                        /* Address Offset: 0x0388 */
    __IO uint32_t SSCGTBL12_15;                       /* Address Offset: 0x038C */
    __IO uint32_t SSCGTBL16_19;                       /* Address Offset: 0x0390 */
    __IO uint32_t SSCGTBL20_23;                       /* Address Offset: 0x0394 */
    __IO uint32_t SSCGTBL24_27;                       /* Address Offset: 0x0398 */
    __IO uint32_t SSCGTBL28_31;                       /* Address Offset: 0x039C */
    __IO uint32_t SSCGTBL32_35;                       /* Address Offset: 0x03A0 */
    __IO uint32_t SSCGTBL36_39;                       /* Address Offset: 0x03A4 */
    __IO uint32_t SSCGTBL40_43;                       /* Address Offset: 0x03A8 */
    __IO uint32_t SSCGTBL44_47;                       /* Address Offset: 0x03AC */
    __IO uint32_t SSCGTBL48_51;                       /* Address Offset: 0x03B0 */
    __IO uint32_t SSCGTBL52_55;                       /* Address Offset: 0x03B4 */
    __IO uint32_t SSCGTBL56_59;                       /* Address Offset: 0x03B8 */
    __IO uint32_t SSCGTBL60_63;                       /* Address Offset: 0x03BC */
    __IO uint32_t SSCGTBL64_67;                       /* Address Offset: 0x03C0 */
    __IO uint32_t SSCGTBL68_71;                       /* Address Offset: 0x03C4 */
    __IO uint32_t SSCGTBL72_75;                       /* Address Offset: 0x03C8 */
    __IO uint32_t SSCGTBL76_79;                       /* Address Offset: 0x03CC */
    __IO uint32_t SSCGTBL80_83;                       /* Address Offset: 0x03D0 */
    __IO uint32_t SSCGTBL84_87;                       /* Address Offset: 0x03D4 */
    __IO uint32_t SSCGTBL88_91;                       /* Address Offset: 0x03D8 */
    __IO uint32_t SSCGTBL92_95;                       /* Address Offset: 0x03DC */
    __IO uint32_t SSCGTBL96_99;                       /* Address Offset: 0x03E0 */
    __IO uint32_t SSCGTBL100_103;                     /* Address Offset: 0x03E4 */
    __IO uint32_t SSCGTBL104_107;                     /* Address Offset: 0x03E8 */
    __IO uint32_t SSCGTBL108_111;                     /* Address Offset: 0x03EC */
    __IO uint32_t SSCGTBL112_115;                     /* Address Offset: 0x03F0 */
    __IO uint32_t SSCGTBL116_119;                     /* Address Offset: 0x03F4 */
    __IO uint32_t SSCGTBL120_123;                     /* Address Offset: 0x03F8 */
    __IO uint32_t SSCGTBL124_127;                     /* Address Offset: 0x03FC */
    __IO uint32_t CRU_SOFTRST_CON[7];                 /* Address Offset: 0x0400 */
};
/* GRF Register Structure Define */
struct GRF_REG {
    __IO uint32_t GPIO0A_IOMUX_L;                     /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0A_IOMUX_H;                     /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0B_IOMUX_L;                     /* Address Offset: 0x0008 */
    __IO uint32_t GPIO0B_IOMUX_H;                     /* Address Offset: 0x000C */
    __IO uint32_t GPIO0C_IOMUX_L;                     /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0C_IOMUX_H;                     /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0D_IOMUX_L;                     /* Address Offset: 0x0018 */
    __IO uint32_t GPIO0D_IOMUX_H;                     /* Address Offset: 0x001C */
    __IO uint32_t GPIO1A_IOMUX_L;                     /* Address Offset: 0x0020 */
    __IO uint32_t GPIO1A_IOMUX_H;                     /* Address Offset: 0x0024 */
    __IO uint32_t GPIO1B_IOMUX_L;                     /* Address Offset: 0x0028 */
    __IO uint32_t GPIO1B_IOMUX_H;                     /* Address Offset: 0x002C */
    __IO uint32_t GPIO1C_IOMUX_L;                     /* Address Offset: 0x0030 */
    __IO uint32_t GPIO1C_IOMUX_H;                     /* Address Offset: 0x0034 */
    __IO uint32_t GPIO1D_IOMUX_L;                     /* Address Offset: 0x0038 */
    __IO uint32_t GPIO1D_IOMUX_H;                     /* Address Offset: 0x003C */
         uint32_t RESERVED0040[48];                   /* Address Offset: 0x0040 */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0100 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0104 */
    __IO uint32_t GPIO0C_P;                           /* Address Offset: 0x0108 */
    __IO uint32_t GPIO0D_P;                           /* Address Offset: 0x010C */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0110 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0114 */
    __IO uint32_t GPIO1C_P;                           /* Address Offset: 0x0118 */
    __IO uint32_t GPIO1D_P;                           /* Address Offset: 0x011C */
         uint32_t RESERVED0120[24];                   /* Address Offset: 0x0120 */
    __IO uint32_t GPIO0A_IE;                          /* Address Offset: 0x0180 */
    __IO uint32_t GPIO0B_IE;                          /* Address Offset: 0x0184 */
    __IO uint32_t GPIO0C_IE;                          /* Address Offset: 0x0188 */
         uint32_t RESERVED018C;                       /* Address Offset: 0x018C */
    __IO uint32_t GPIO1A_IE;                          /* Address Offset: 0x0190 */
    __IO uint32_t GPIO1B_IE;                          /* Address Offset: 0x0194 */
    __IO uint32_t GPIO1C_IE;                          /* Address Offset: 0x0198 */
         uint32_t RESERVED019C[25];                   /* Address Offset: 0x019C */
    __IO uint32_t SOC_CON0;                           /* Address Offset: 0x0200 */
    __IO uint32_t SOC_CON1;                           /* Address Offset: 0x0204 */
    __IO uint32_t SOC_CON2;                           /* Address Offset: 0x0208 */
    __IO uint32_t SOC_CON3;                           /* Address Offset: 0x020C */
    __IO uint32_t SOC_CON4;                           /* Address Offset: 0x0210 */
    __IO uint32_t SOC_CON5;                           /* Address Offset: 0x0214 */
         uint32_t RESERVED0218[26];                   /* Address Offset: 0x0218 */
    __IO uint32_t SOC_STATUS0;                        /* Address Offset: 0x0280 */
         uint32_t RESERVED0284[31];                   /* Address Offset: 0x0284 */
    __IO uint32_t MCU_STATUS0;                        /* Address Offset: 0x0300 */
    __I  uint32_t MCU_STATUS1;                        /* Address Offset: 0x0304 */
         uint32_t RESERVED0308[14];                   /* Address Offset: 0x0308 */
    __IO uint32_t UOC_CON0;                           /* Address Offset: 0x0340 */
         uint32_t RESERVED0344[47];                   /* Address Offset: 0x0344 */
    __IO uint32_t FAST_BOOT;                          /* Address Offset: 0x0400 */
    __IO uint32_t FAST_BOOT_ADDR;                     /* Address Offset: 0x0404 */
         uint32_t RESERVED0408[62];                   /* Address Offset: 0x0408 */
    __IO uint32_t MCU_CON0;                           /* Address Offset: 0x0500 */
    __IO uint32_t MCU_CON1;                           /* Address Offset: 0x0504 */
         uint32_t RESERVED0508[190];                  /* Address Offset: 0x0508 */
    __IO uint32_t OS_REG0;                            /* Address Offset: 0x0800 */
    __IO uint32_t OS_REG1;                            /* Address Offset: 0x0804 */
    __IO uint32_t OS_REG2;                            /* Address Offset: 0x0808 */
    __IO uint32_t OS_REG3;                            /* Address Offset: 0x080C */
    __IO uint32_t OS_REG4;                            /* Address Offset: 0x0810 */
    __IO uint32_t OS_REG5;                            /* Address Offset: 0x0814 */
    __IO uint32_t OS_REG6;                            /* Address Offset: 0x0818 */
    __IO uint32_t OS_REG7;                            /* Address Offset: 0x081C */
         uint32_t RESERVED0820[56];                   /* Address Offset: 0x0820 */
    __IO uint32_t SOC_VERSION;                        /* Address Offset: 0x0900 */
};
/* USB_GRF Register Structure Define */
struct USB_GRF_REG {
    __IO uint32_t REG0;                               /* Address Offset: 0x0000 */
    __IO uint32_t REG1;                               /* Address Offset: 0x0004 */
    __IO uint32_t REG2;                               /* Address Offset: 0x0008 */
    __IO uint32_t REG3;                               /* Address Offset: 0x000C */
    __IO uint32_t REG4;                               /* Address Offset: 0x0010 */
    __IO uint32_t REG5;                               /* Address Offset: 0x0014 */
    __IO uint32_t REG6;                               /* Address Offset: 0x0018 */
    __IO uint32_t REG7;                               /* Address Offset: 0x001C */
    __IO uint32_t REG8;                               /* Address Offset: 0x0020 */
    __IO uint32_t REG9;                               /* Address Offset: 0x0024 */
    __IO uint32_t REG10;                              /* Address Offset: 0x0028 */
    __IO uint32_t REG11;                              /* Address Offset: 0x002C */
    __IO uint32_t REG12;                              /* Address Offset: 0x0030 */
    __IO uint32_t REG13;                              /* Address Offset: 0x0034 */
    __IO uint32_t REG14;                              /* Address Offset: 0x0038 */
    __IO uint32_t REG15;                              /* Address Offset: 0x003C */
    __IO uint32_t REG16;                              /* Address Offset: 0x0040 */
    __IO uint32_t REG17;                              /* Address Offset: 0x0044 */
    __IO uint32_t REG18;                              /* Address Offset: 0x0048 */
    __IO uint32_t REG19;                              /* Address Offset: 0x004C */
    __IO uint32_t REG20;                              /* Address Offset: 0x0050 */
    __IO uint32_t REG21;                              /* Address Offset: 0x0054 */
    __IO uint32_t REG22;                              /* Address Offset: 0x0058 */
    __IO uint32_t REG23;                              /* Address Offset: 0x005C */
         uint32_t RESERVED0060[40];                   /* Address Offset: 0x0060 */
    __IO uint32_t CON0;                               /* Address Offset: 0x0100 */
    __IO uint32_t CON1;                               /* Address Offset: 0x0104 */
    __IO uint32_t CON2;                               /* Address Offset: 0x0108 */
    __IO uint32_t CON3;                               /* Address Offset: 0x010C */
    __IO uint32_t USB2_DETECT_IRQ_ENABLE;             /* Address Offset: 0x0110 */
    __I  uint32_t USB2_DETECT_IRQ_STATUS;             /* Address Offset: 0x0114 */
    __O  uint32_t USB2_DETECT_IRQ_STATUS_CLR;         /* Address Offset: 0x0118 */
         uint32_t RESERVED011C;                       /* Address Offset: 0x011C */
    __I  uint32_t STATUS;                             /* Address Offset: 0x0120 */
         uint32_t RESERVED0124[3];                    /* Address Offset: 0x0124 */
    __IO uint32_t USB2_LINESTATE_CON;                 /* Address Offset: 0x0130 */
    __IO uint32_t USB2_DISCONNECT_CON;                /* Address Offset: 0x0134 */
    __IO uint32_t USB2_BVALID_CON;                    /* Address Offset: 0x0138 */
    __IO uint32_t USB2_ID_CON;                        /* Address Offset: 0x013C */
};
/* GPIO Register Structure Define */
struct GPIO_REG {
    __IO uint32_t SWPORT_DR_L;                        /* Address Offset: 0x0000 */
    __IO uint32_t SWPORT_DR_H;                        /* Address Offset: 0x0004 */
    __IO uint32_t SWPORT_DDR_L;                       /* Address Offset: 0x0008 */
    __IO uint32_t SWPORT_DDR_H;                       /* Address Offset: 0x000C */
    __IO uint32_t INT_EN_L;                           /* Address Offset: 0x0010 */
    __IO uint32_t INT_EN_H;                           /* Address Offset: 0x0014 */
    __IO uint32_t INT_MASK_L;                         /* Address Offset: 0x0018 */
    __IO uint32_t INT_MASK_H;                         /* Address Offset: 0x001C */
    __IO uint32_t INT_TYPE_L;                         /* Address Offset: 0x0020 */
    __IO uint32_t INT_TYPE_H;                         /* Address Offset: 0x0024 */
    __IO uint32_t INT_POLARITY_L;                     /* Address Offset: 0x0028 */
    __IO uint32_t INT_POLARITY_H;                     /* Address Offset: 0x002C */
    __IO uint32_t INT_BOTHEDGE_L;                     /* Address Offset: 0x0030 */
    __IO uint32_t INT_BOTHEDGE_H;                     /* Address Offset: 0x0034 */
    __IO uint32_t DEBOUNCE_L;                         /* Address Offset: 0x0038 */
    __IO uint32_t DEBOUNCE_H;                         /* Address Offset: 0x003C */
    __IO uint32_t DBCLK_DIV_EN_L;                     /* Address Offset: 0x0040 */
    __IO uint32_t DBCLK_DIV_EN_H;                     /* Address Offset: 0x0044 */
    __IO uint32_t DBCLK_DIV_CON;                      /* Address Offset: 0x0048 */
         uint32_t RESERVED004C;                       /* Address Offset: 0x004C */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0050 */
         uint32_t RESERVED0054;                       /* Address Offset: 0x0054 */
    __I  uint32_t INT_RAWSTATUS;                      /* Address Offset: 0x0058 */
         uint32_t RESERVED005C;                       /* Address Offset: 0x005C */
    __IO uint32_t PORT_EOI_L;                         /* Address Offset: 0x0060 */
    __IO uint32_t PORT_EOI_H;                         /* Address Offset: 0x0064 */
         uint32_t RESERVED0068[2];                    /* Address Offset: 0x0068 */
    __I  uint32_t EXT_PORT;                           /* Address Offset: 0x0070 */
         uint32_t RESERVED0074;                       /* Address Offset: 0x0074 */
    __I  uint32_t VER_ID;                             /* Address Offset: 0x0078 */
};
/* CACHE Register Structure Define */
struct CACHE_REG {
    __IO uint32_t CACHE_CTRL;                         /* Address Offset: 0x0000 */
    __IO uint32_t CACHE_MAINTAIN[2];                  /* Address Offset: 0x0004 */
    __IO uint32_t STB_TIMEOUT_CTRL;                   /* Address Offset: 0x000C */
         uint32_t RESERVED0010[4];                    /* Address Offset: 0x0010 */
    __IO uint32_t CACHE_INT_EN;                       /* Address Offset: 0x0020 */
    __IO uint32_t CACHE_INT_ST;                       /* Address Offset: 0x0024 */
    __IO uint32_t CACHE_ERR_HADDR;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED002C;                       /* Address Offset: 0x002C */
    __I  uint32_t CACHE_STATUS;                       /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[3];                    /* Address Offset: 0x0034 */
    __I  uint32_t PMU_RD_NUM_CNT;                     /* Address Offset: 0x0040 */
    __I  uint32_t PMU_WR_NUM_CNT;                     /* Address Offset: 0x0044 */
    __I  uint32_t PMU_SRAM_RD_HIT_CNT;                /* Address Offset: 0x0048 */
    __I  uint32_t PMU_HB_RD_HIT_CNT;                  /* Address Offset: 0x004C */
    __IO uint32_t PMU_STB_RD_HIT_CNT;                 /* Address Offset: 0x0050 */
    __I  uint32_t PMU_RD_HIT_CNT;                     /* Address Offset: 0x0054 */
    __I  uint32_t PMU_WR_HIT_CNT;                     /* Address Offset: 0x0058 */
    __I  uint32_t PMU_RD_MISS_PENALTY_CNT;            /* Address Offset: 0x005C */
    __I  uint32_t PMU_WR_MISS_PENALTY_CNT;            /* Address Offset: 0x0060 */
    __I  uint32_t PMU_RD_LAT_CNT;                     /* Address Offset: 0x0064 */
    __I  uint32_t PMU_WR_LAT_CNT;                     /* Address Offset: 0x0068 */
         uint32_t RESERVED006C[33];                   /* Address Offset: 0x006C */
    __IO uint32_t REVISION;                           /* Address Offset: 0x00F0 */

};
/* DMA Register Structure Define */
struct DMA_CHAN_REGS {
    __IO uint32_t SAR;
         uint32_t RESERVED0004;
    __IO uint32_t DAR;
         uint32_t RESERVED000C;
    __IO uint32_t LLP;
         uint32_t RESERVED0014;
    __IO uint32_t CTL_LO;
    __IO uint32_t CTL_HI;
    __IO uint32_t SSTAT;
         uint32_t RESERVED0024;
    __IO uint32_t DSTAT;
         uint32_t RESERVED002C;
    __IO uint32_t SSTATAR;
         uint32_t RESERVED0034;
    __IO uint32_t DSTATAR;
         uint32_t RESERVED003C;
    __IO uint32_t CFG_LO;
    __IO uint32_t CFG_HI;
    __IO uint32_t SGR;
         uint32_t RESERVED004C;
    __IO uint32_t DSR;
         uint32_t RESERVED;
};
struct DMA_IRQ_REGS {
    __IO uint32_t TFR;
         uint32_t RESERVED02C4;
    __IO uint32_t BLOCK;
         uint32_t RESERVED02CC;
    __IO uint32_t SRCTRAN;
         uint32_t RESERVED02D4;
    __IO uint32_t DSTTRAN;
         uint32_t RESERVED02DC;
    __IO uint32_t ERR;
         uint32_t RESERVED;
};
struct DMA_REG {
    struct DMA_CHAN_REGS CHAN[3];                     /* Address Offset: 0x0000 */
         uint32_t RESERVED0108[110];                  /* Address Offset: 0x0108 */
    struct DMA_IRQ_REGS RAW;                          /* Address Offset: 0x02C0 */
    struct DMA_IRQ_REGS STATUS;                       /* Address Offset: 0x02E8 */
    struct DMA_IRQ_REGS MASK;                         /* Address Offset: 0x0310 */
    struct DMA_IRQ_REGS CLEAR;                        /* Address Offset: 0x0338 */
    __I  uint32_t STATUSINT;                          /* Address Offset: 0x0360 */
         uint32_t RESERVED0364[13];                   /* Address Offset: 0x0364 */
    __IO uint32_t DMACFGREG;                          /* Address Offset: 0x0398 */
         uint32_t RESERVED039C;                       /* Address Offset: 0x039C */
    __IO uint32_t CHENREG;                            /* Address Offset: 0x03A0 */
};
/* FSPI Register Structure Define */
struct FSPI_REG {
    __IO uint32_t CTRL0;                              /* Address Offset: 0x0000 */
    __IO uint32_t IMR;                                /* Address Offset: 0x0004 */
    __IO uint32_t ICLR;                               /* Address Offset: 0x0008 */
    __IO uint32_t FTLR;                               /* Address Offset: 0x000C */
    __IO uint32_t RCVR;                               /* Address Offset: 0x0010 */
    __IO uint32_t AX0;                                /* Address Offset: 0x0014 */
    __IO uint32_t ABIT0;                              /* Address Offset: 0x0018 */
    __IO uint32_t ISR;                                /* Address Offset: 0x001C */
    __IO uint32_t FSR;                                /* Address Offset: 0x0020 */
    __I  uint32_t SR;                                 /* Address Offset: 0x0024 */
    __I  uint32_t RISR;                               /* Address Offset: 0x0028 */
    __I  uint32_t VER;                                /* Address Offset: 0x002C */
    __IO uint32_t QOP;                                /* Address Offset: 0x0030 */
    __IO uint32_t EXT_CTRL;                           /* Address Offset: 0x0034 */
         uint32_t RESERVED0038;                       /* Address Offset: 0x0038 */
    __IO uint32_t DLL_CTRL0;                          /* Address Offset: 0x003C */
         uint32_t RESERVED0040;                       /* Address Offset: 0x0040 */
    __IO uint32_t EXT_AX;                             /* Address Offset: 0x0044 */
    __IO uint32_t SCLK_INATM_CNT;                     /* Address Offset: 0x0048 */
         uint32_t RESERVED004C;                       /* Address Offset: 0x004C */
    __O  uint32_t XMMC_WCMD0;                         /* Address Offset: 0x0050 */
    __O  uint32_t XMMC_RCMD0;                         /* Address Offset: 0x0054 */
    __IO uint32_t XMMC_CTRL;                          /* Address Offset: 0x0058 */
    __IO uint32_t MODE;                               /* Address Offset: 0x005C */
    __IO uint32_t DEVRGN;                             /* Address Offset: 0x0060 */
    __IO uint32_t DEVSIZE0;                           /* Address Offset: 0x0064 */
    __IO uint32_t TME0;                               /* Address Offset: 0x0068 */
         uint32_t RESERVED006C;                       /* Address Offset: 0x006C */
    __IO uint32_t XMMC_RX_WTMRK;                      /* Address Offset: 0x0070 */
         uint32_t RESERVED0074[3];                    /* Address Offset: 0x0074 */
    __IO uint32_t DMATR;                              /* Address Offset: 0x0080 */
    __IO uint32_t DMAADDR;                            /* Address Offset: 0x0084 */
    __IO uint32_t LEN_CTRL;                           /* Address Offset: 0x0088 */
    __IO uint32_t LEN_EXT;                            /* Address Offset: 0x008C */
         uint32_t RESERVED0090;                       /* Address Offset: 0x0090 */
    __IO uint32_t XMMCSR;                             /* Address Offset: 0x0094 */
         uint32_t RESERVED0098[26];                   /* Address Offset: 0x0098 */
    __O  uint32_t CMD;                                /* Address Offset: 0x0100 */
    __O  uint32_t ADDR;                               /* Address Offset: 0x0104 */
    __IO uint32_t DATA;                               /* Address Offset: 0x0108 */
         uint32_t RESERVED010C[61];                   /* Address Offset: 0x010C */
    __IO uint32_t CTRL1;                              /* Address Offset: 0x0200 */
         uint32_t RESERVED0204[4];                    /* Address Offset: 0x0204 */
    __IO uint32_t AX1;                                /* Address Offset: 0x0214 */
    __IO uint32_t ABIT1;                              /* Address Offset: 0x0218 */
         uint32_t RESERVED021C[8];                    /* Address Offset: 0x021C */
    __IO uint32_t DLL_CTRL1;                          /* Address Offset: 0x023C */
         uint32_t RESERVED0240[4];                    /* Address Offset: 0x0240 */
    __O  uint32_t XMMC_WCMD1;                         /* Address Offset: 0x0250 */
    __O  uint32_t XMMC_RCMD1;                         /* Address Offset: 0x0254 */
         uint32_t RESERVED0258[3];                    /* Address Offset: 0x0258 */
    __IO uint32_t DEVSIZE1;                           /* Address Offset: 0x0264 */
    __IO uint32_t TME1;                               /* Address Offset: 0x0268 */
};
/* CKCAL Register Structure Define */
struct CKCAL_REG {
    __IO uint32_t CTRL;                               /* Address Offset: 0x0000 */
    __IO uint32_t RESULT_CTRL;                        /* Address Offset: 0x0004 */
    __IO uint32_t STATE;                              /* Address Offset: 0x0008 */
    __IO uint32_t SEL;                                /* Address Offset: 0x000C */
    __IO uint32_t BASE_ADDR;                          /* Address Offset: 0x0010 */
    __IO uint32_t LEN;                                /* Address Offset: 0x0014 */
    __IO uint32_t INI_DATA;                           /* Address Offset: 0x0018 */
    __IO uint32_t BURST;                              /* Address Offset: 0x001C */
    __IO uint32_t RESULT;                             /* Address Offset: 0x0020 */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x0024 */
};
/* JPEG_ENC Register Structure Define */
struct JPEG_ENC_REG {
    __IO uint32_t SWREG_0;                            /* Address Offset: 0x0000 */
    __IO uint32_t SWREG_1;                            /* Address Offset: 0x0004 */
    __IO uint32_t SWREG_2;                            /* Address Offset: 0x0008 */
    __IO uint32_t SWREG_3;                            /* Address Offset: 0x000C */
    __IO uint32_t SWREG_4;                            /* Address Offset: 0x0010 */
    __IO uint32_t SWREG_5;                            /* Address Offset: 0x0014 */
    __IO uint32_t SWREG_6;                            /* Address Offset: 0x0018 */
    __IO uint32_t SWREG_7;                            /* Address Offset: 0x001C */
    __IO uint32_t SWREG_8;                            /* Address Offset: 0x0020 */
    __IO uint32_t SWREG_9;                            /* Address Offset: 0x0024 */
    __IO uint32_t SWREG_10;                           /* Address Offset: 0x0028 */
    __IO uint32_t SWREG_11;                           /* Address Offset: 0x002C */
    __IO uint32_t SWREG_12;                           /* Address Offset: 0x0030 */
    __IO uint32_t SWREG_13;                           /* Address Offset: 0x0034 */
    __IO uint32_t SWREG_14;                           /* Address Offset: 0x0038 */
    __IO uint32_t SWREG_15;                           /* Address Offset: 0x003C */
    __IO uint32_t SWREG_16;                           /* Address Offset: 0x0040 */
    __IO uint32_t SWREG_17;                           /* Address Offset: 0x0044 */
    __IO uint32_t SWREG_18;                           /* Address Offset: 0x0048 */
    __IO uint32_t SWREG_19;                           /* Address Offset: 0x004C */
    __IO uint32_t SWREG_20;                           /* Address Offset: 0x0050 */
    __IO uint32_t SWREG_21;                           /* Address Offset: 0x0054 */
    __IO uint32_t SWREG_22;                           /* Address Offset: 0x0058 */
    __IO uint32_t SWREG_23;                           /* Address Offset: 0x005C */
    __IO uint32_t SWREG_24;                           /* Address Offset: 0x0060 */
    __IO uint32_t SWREG_25;                           /* Address Offset: 0x0064 */
    __IO uint32_t SWREG_26;                           /* Address Offset: 0x0068 */
    __IO uint32_t SWREG_27;                           /* Address Offset: 0x006C */
    __IO uint32_t SWREG_28;                           /* Address Offset: 0x0070 */
    __IO uint32_t SWREG_29;                           /* Address Offset: 0x0074 */
    __IO uint32_t SWREG_30;                           /* Address Offset: 0x0078 */
    __IO uint32_t SWREG_31;                           /* Address Offset: 0x007C */
         uint32_t RESERVED0080[12];                   /* Address Offset: 0x0080 */
    __IO uint32_t SWREG_44;                           /* Address Offset: 0x00B0 */
    __IO uint32_t SWREG_45;                           /* Address Offset: 0x00B4 */
    __IO uint32_t SWREG_46;                           /* Address Offset: 0x00B8 */
    __IO uint32_t SWREG_47;                           /* Address Offset: 0x00BC */
    __IO uint32_t SWREG_48;                           /* Address Offset: 0x00C0 */
    __IO uint32_t SWREG_49;                           /* Address Offset: 0x00C4 */
    __IO uint32_t SWREG_50;                           /* Address Offset: 0x00C8 */
    __IO uint32_t SWREG_51;                           /* Address Offset: 0x00CC */
    __IO uint32_t SWREG_52;                           /* Address Offset: 0x00D0 */
    __IO uint32_t SWREG_53;                           /* Address Offset: 0x00D4 */
    __IO uint32_t SWREG_54;                           /* Address Offset: 0x00D8 */
    __IO uint32_t SWREG_55;                           /* Address Offset: 0x00DC */
    __IO uint32_t SWREG_56;                           /* Address Offset: 0x00E0 */
    __IO uint32_t SWREG_57;                           /* Address Offset: 0x00E4 */
    __IO uint32_t SWREG_58;                           /* Address Offset: 0x00E8 */
    __IO uint32_t SWREG_59;                           /* Address Offset: 0x00EC */
    __IO uint32_t SWREG_60;                           /* Address Offset: 0x00F0 */
    __IO uint32_t SWREG_61;                           /* Address Offset: 0x00F4 */
    __IO uint32_t SWREG_62;                           /* Address Offset: 0x00F8 */
    __IO uint32_t SWREG_63;                           /* Address Offset: 0x00FC */
    __IO uint32_t SWREG_64;                           /* Address Offset: 0x0100 */
    __IO uint32_t SWREG_65_REUSE;                     /* Address Offset: 0x0104 */
    __IO uint32_t SWREG_66_REUSE;                     /* Address Offset: 0x0108 */
    __IO uint32_t SWREG_67_REUSE;                     /* Address Offset: 0x010C */
    __IO uint32_t SWREG_68_REUSE;                     /* Address Offset: 0x0110 */
    __IO uint32_t SWREG_69_REUSE;                     /* Address Offset: 0x0114 */
    __IO uint32_t SWREG_70_REUSE;                     /* Address Offset: 0x0118 */
    __IO uint32_t SWREG_71_REUSE;                     /* Address Offset: 0x011C */
    __IO uint32_t SWREG_72_REUSE;                     /* Address Offset: 0x0120 */
    __IO uint32_t SWREG_73_REUSE;                     /* Address Offset: 0x0124 */
    __IO uint32_t SWREG_74;                           /* Address Offset: 0x0128 */
    __IO uint32_t SWREG_75;                           /* Address Offset: 0x012C */
    __IO uint32_t SWREG_76_REUSE;                     /* Address Offset: 0x0130 */
    __IO uint32_t SWREG_77;                           /* Address Offset: 0x0134 */
    __IO uint32_t SWREG_78;                           /* Address Offset: 0x0138 */
    __IO uint32_t SWREG_79;                           /* Address Offset: 0x013C */
    __IO uint32_t SWREG_80;                           /* Address Offset: 0x0140 */
    __IO uint32_t SWREG_81;                           /* Address Offset: 0x0144 */
    __IO uint32_t SWREG_82;                           /* Address Offset: 0x0148 */
    __IO uint32_t SWREG_83;                           /* Address Offset: 0x014C */
    __IO uint32_t SWREG_84;                           /* Address Offset: 0x0150 */
    __IO uint32_t SWREG_85;                           /* Address Offset: 0x0154 */
    __IO uint32_t SWREG_86;                           /* Address Offset: 0x0158 */
    __IO uint32_t SWREG_87;                           /* Address Offset: 0x015C */
    __IO uint32_t SWREG_88;                           /* Address Offset: 0x0160 */
    __IO uint32_t SWREG_89;                           /* Address Offset: 0x0164 */
    __IO uint32_t SWREG_90;                           /* Address Offset: 0x0168 */
    __IO uint32_t SWREG_91;                           /* Address Offset: 0x016C */
    __IO uint32_t SWREG_92;                           /* Address Offset: 0x0170 */
    __IO uint32_t SWREG_93;                           /* Address Offset: 0x0174 */
    __IO uint32_t SWREG_94;                           /* Address Offset: 0x0178 */
    __IO uint32_t SWREG_95;                           /* Address Offset: 0x017C */
    __IO uint32_t SWREG_96;                           /* Address Offset: 0x0180 */
    __IO uint32_t SWREG_97;                           /* Address Offset: 0x0184 */
    __IO uint32_t SWREG_98;                           /* Address Offset: 0x0188 */
    __IO uint32_t SWREG_99;                           /* Address Offset: 0x018C */
    __IO uint32_t SWREG_100_REUSE;                    /* Address Offset: 0x0190 */
    __I  uint32_t SWREG_101_READ;                     /* Address Offset: 0x0194 */
    __IO uint32_t SWREG_102;                          /* Address Offset: 0x0198 */
    __IO uint32_t SWREG_103;                          /* Address Offset: 0x019C */
    __IO uint32_t SWREG_104;                          /* Address Offset: 0x01A0 */
    __IO uint32_t SWREG_105;                          /* Address Offset: 0x01A4 */
    __IO uint32_t SWREG_106_REUSE;                    /* Address Offset: 0x01A8 */
    __IO uint32_t SWREG_107_REUSE;                    /* Address Offset: 0x01AC */
    __IO uint32_t SWREG_108_REUSE;                    /* Address Offset: 0x01B0 */
    __IO uint32_t SWREG_109;                          /* Address Offset: 0x01B4 */
    __I  uint32_t SWREG_110_READ;                     /* Address Offset: 0x01B8 */
         uint32_t RESERVED01BC[9];                    /* Address Offset: 0x01BC */
    __O  uint32_t SWREG_120_183;                      /* Address Offset: 0x01E0 */
};
/* VICAP_LITE Register Structure Define */
struct VICAP_LITE_REG {
         uint32_t RESERVED0000[32];                   /* Address Offset: 0x0000 */
    __IO uint32_t MIPI_ID0_CTRL0;                     /* Address Offset: 0x0080 */
    __IO uint32_t MIPI_ID0_CTRL1;                     /* Address Offset: 0x0084 */
    __IO uint32_t MIPI_ID1_CTRL0;                     /* Address Offset: 0x0088 */
    __IO uint32_t MIPI_ID1_CTRL1;                     /* Address Offset: 0x008C */
    __IO uint32_t MIPI_ID2_CTRL0;                     /* Address Offset: 0x0090 */
    __IO uint32_t MIPI_ID2_CTRL1;                     /* Address Offset: 0x0094 */
    __IO uint32_t MIPI_ID3_CTRL0;                     /* Address Offset: 0x0098 */
    __IO uint32_t MIPI_ID3_CTRL1;                     /* Address Offset: 0x009C */
    __IO uint32_t MIPI_CTRL;                          /* Address Offset: 0x00A0 */
    __IO uint32_t MIPI_FRAME0_ADDR_Y_ID0;             /* Address Offset: 0x00A4 */
    __IO uint32_t MIPI_FRAME1_ADDR_Y_ID0;             /* Address Offset: 0x00A8 */
    __IO uint32_t MIPI_FRAME0_ADDR_UV_ID0;            /* Address Offset: 0x00AC */
    __IO uint32_t MIPI_FRAME1_ADDR_UV_ID0;            /* Address Offset: 0x00B0 */
    __IO uint32_t MIPI_FRAME0_VLW_Y_ID0;              /* Address Offset: 0x00B4 */
    __IO uint32_t MIPI_FRAME1_VLW_Y_ID0;              /* Address Offset: 0x00B8 */
    __IO uint32_t MIPI_FRAME0_VLW_UV_ID0;             /* Address Offset: 0x00BC */
    __IO uint32_t MIPI_FRAME1_VLW_UV_ID0;             /* Address Offset: 0x00C0 */
    __IO uint32_t MIPI_FRAME0_ADDR_Y_ID1;             /* Address Offset: 0x00C4 */
    __IO uint32_t MIPI_FRAME1_ADDR_Y_ID1;             /* Address Offset: 0x00C8 */
    __IO uint32_t MIPI_FRAME0_ADDR_UV_ID1;            /* Address Offset: 0x00CC */
    __IO uint32_t MIPI_FRAME1_ADDR_UV_ID1;            /* Address Offset: 0x00D0 */
    __IO uint32_t MIPI_FRAME0_VLW_Y_ID1;              /* Address Offset: 0x00D4 */
    __IO uint32_t MIPI_FRAME1_VLW_Y_ID1;              /* Address Offset: 0x00D8 */
    __IO uint32_t MIPI_FRAME0_VLW_UV_ID1;             /* Address Offset: 0x00DC */
    __IO uint32_t MIPI_FRAME1_VLW_UV_ID1;             /* Address Offset: 0x00E0 */
    __IO uint32_t MIPI_FRAME0_ADDR_Y_ID2;             /* Address Offset: 0x00E4 */
    __IO uint32_t MIPI_FRAME1_ADDR_Y_ID2;             /* Address Offset: 0x00E8 */
    __IO uint32_t MIPI_FRAME0_ADDR_UV_ID2;            /* Address Offset: 0x00EC */
    __IO uint32_t MIPI_FRAME1_ADDR_UV_ID2;            /* Address Offset: 0x00F0 */
    __IO uint32_t MIPI_FRAME0_VLW_Y_ID2;              /* Address Offset: 0x00F4 */
    __IO uint32_t MIPI_FRAME1_VLW_Y_ID2;              /* Address Offset: 0x00F8 */
    __IO uint32_t MIPI_FRAME0_VLW_UV_ID2;             /* Address Offset: 0x00FC */
    __IO uint32_t MIPI_FRAME1_VLW_UV_ID2;             /* Address Offset: 0x0100 */
    __IO uint32_t MIPI_FRAME0_ADDR_Y_ID3;             /* Address Offset: 0x0104 */
    __IO uint32_t MIPI_FRAME1_ADDR_Y_ID3;             /* Address Offset: 0x0108 */
    __IO uint32_t MIPI_FRAME0_ADDR_UV_ID3;            /* Address Offset: 0x010C */
    __IO uint32_t MIPI_FRAME1_ADDR_UV_ID3;            /* Address Offset: 0x0110 */
    __IO uint32_t MIPI_FRAME0_VLW_Y_ID3;              /* Address Offset: 0x0114 */
    __IO uint32_t MIPI_FRAME1_VLW_Y_ID3;              /* Address Offset: 0x0118 */
    __IO uint32_t MIPI_FRAME0_VLW_UV_ID3;             /* Address Offset: 0x011C */
    __IO uint32_t MIPI_FRAME1_VLW_UV_ID3;             /* Address Offset: 0x0120 */
    __IO uint32_t MIPI_INTEN;                         /* Address Offset: 0x0124 */
    __IO uint32_t MIPI_INTSTAT;                       /* Address Offset: 0x0128 */
    __IO uint32_t MIPI_LINE_INT_NUM_ID0_1;            /* Address Offset: 0x012C */
    __IO uint32_t MIPI_LINE_INT_NUM_ID2_3;            /* Address Offset: 0x0130 */
    __I  uint32_t MIPI_LINE_CNT_ID0_1;                /* Address Offset: 0x0134 */
    __I  uint32_t MIPI_LINE_CNT_ID2_3;                /* Address Offset: 0x0138 */
    __IO uint32_t MIPI_ID0_CROP_START;                /* Address Offset: 0x013C */
    __IO uint32_t MIPI_ID1_CROP_START;                /* Address Offset: 0x0140 */
    __IO uint32_t MIPI_ID2_CROP_START;                /* Address Offset: 0x0144 */
    __IO uint32_t MIPI_ID3_CROP_START;                /* Address Offset: 0x0148 */
    __I  uint32_t MIPI_FRAME_NUM_VC0;                 /* Address Offset: 0x014C */
    __I  uint32_t MIPI_FRAME_NUM_VC1;                 /* Address Offset: 0x0150 */
    __I  uint32_t MIPI_FRAME_NUM_VC2;                 /* Address Offset: 0x0154 */
    __I  uint32_t MIPI_FRAME_NUM_VC3;                 /* Address Offset: 0x0158 */
};
/* ISP_MAIN_CTRL Register Structure Define */
struct ISP_MAIN_CTRL_REG {
    __IO uint32_t VI_CCL;                             /* Address Offset: 0x0000 */
         uint32_t RESERVED0004;                       /* Address Offset: 0x0004 */
    __I  uint32_t VI_ID;                              /* Address Offset: 0x0008 */
         uint32_t RESERVED000C;                       /* Address Offset: 0x000C */
    __IO uint32_t VI_ICCL;                            /* Address Offset: 0x0010 */
    __IO uint32_t VI_IRCL;                            /* Address Offset: 0x0014 */
    __IO uint32_t VI_DPCL;                            /* Address Offset: 0x0018 */
};
/* ISP_IMG_EFF Register Structure Define */
struct ISP_IMG_EFF_REG {
    __IO uint32_t IMG_EFF_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t IMG_EFF_COLOR_SEL;                  /* Address Offset: 0x0004 */
    __IO uint32_t IMG_EFF_MAT_1;                      /* Address Offset: 0x0008 */
    __IO uint32_t IMG_EFF_MAT_2;                      /* Address Offset: 0x000C */
    __IO uint32_t IMG_EFF_MAT_3;                      /* Address Offset: 0x0010 */
    __IO uint32_t IMG_EFF_MAT_4;                      /* Address Offset: 0x0014 */
    __IO uint32_t IMG_EFF_MAT_5;                      /* Address Offset: 0x0018 */
    __IO uint32_t IMG_EFF_TINT;                       /* Address Offset: 0x001C */
    __I  uint32_t IMG_EFF_CTRL_SHD;                   /* Address Offset: 0x0020 */
    __IO uint32_t IMG_EFF_SHARPEN;                    /* Address Offset: 0x0024 */
};
/* ISP_SUPER_IMP Register Structure Define */
struct ISP_SUPER_IMP_REG {
    __IO uint32_t SUPER_IMP_CTRL;                     /* Address Offset: 0x0000 */
    __IO uint32_t SUPER_IMP_OFFSET_X;                 /* Address Offset: 0x0004 */
    __IO uint32_t SUPER_IMP_OFFSET_Y;                 /* Address Offset: 0x0008 */
    __IO uint32_t SUPER_IMP_COLOR_Y;                  /* Address Offset: 0x000C */
    __IO uint32_t SUPER_IMP_COLOR_CB;                 /* Address Offset: 0x0010 */
    __IO uint32_t SUPER_IMP_COLOR_CR;                 /* Address Offset: 0x0014 */
};
/* ISP Register Structure Define */
struct ISP_REG {
    __IO uint32_t ISP_CTRL;                           /* Address Offset: 0x0000 */
    __IO uint32_t ISP_ACQ_PROP;                       /* Address Offset: 0x0004 */
    __IO uint32_t ISP_ACQ_H_OFFS;                     /* Address Offset: 0x0008 */
    __IO uint32_t ISP_ACQ_V_OFFS;                     /* Address Offset: 0x000C */
    __IO uint32_t ISP_ACQ_H_SIZE;                     /* Address Offset: 0x0010 */
    __IO uint32_t ISP_ACQ_V_SIZE;                     /* Address Offset: 0x0014 */
    __IO uint32_t ISP_ACQ_NR_FRAMES;                  /* Address Offset: 0x0018 */
    __IO uint32_t ISP_GAMMA_DX_LO;                    /* Address Offset: 0x001C */
    __IO uint32_t ISP_GAMMA_DX_HI;                    /* Address Offset: 0x0020 */
    __IO uint32_t ISP_GAMMA_R_Y_0;                    /* Address Offset: 0x0024 */
    __IO uint32_t ISP_GAMMA_R_Y_1;                    /* Address Offset: 0x0028 */
    __IO uint32_t ISP_GAMMA_R_Y_2;                    /* Address Offset: 0x002C */
    __IO uint32_t ISP_GAMMA_R_Y_3;                    /* Address Offset: 0x0030 */
    __IO uint32_t ISP_GAMMA_R_Y_4;                    /* Address Offset: 0x0034 */
    __IO uint32_t ISP_GAMMA_R_Y_5;                    /* Address Offset: 0x0038 */
    __IO uint32_t ISP_GAMMA_R_Y_6;                    /* Address Offset: 0x003C */
    __IO uint32_t ISP_GAMMA_R_Y_7;                    /* Address Offset: 0x0040 */
    __IO uint32_t ISP_GAMMA_R_Y_8;                    /* Address Offset: 0x0044 */
    __IO uint32_t ISP_GAMMA_R_Y_9;                    /* Address Offset: 0x0048 */
    __IO uint32_t ISP_GAMMA_R_Y_10;                   /* Address Offset: 0x004C */
    __IO uint32_t ISP_GAMMA_R_Y_11;                   /* Address Offset: 0x0050 */
    __IO uint32_t ISP_GAMMA_R_Y_12;                   /* Address Offset: 0x0054 */
    __IO uint32_t ISP_GAMMA_R_Y_13;                   /* Address Offset: 0x0058 */
    __IO uint32_t ISP_GAMMA_R_Y_14;                   /* Address Offset: 0x005C */
    __IO uint32_t ISP_GAMMA_R_Y_15;                   /* Address Offset: 0x0060 */
    __IO uint32_t ISP_GAMMA_R_Y_16;                   /* Address Offset: 0x0064 */
    __IO uint32_t ISP_GAMMA_G_Y_0;                    /* Address Offset: 0x0068 */
    __IO uint32_t ISP_GAMMA_G_Y_1;                    /* Address Offset: 0x006C */
    __IO uint32_t ISP_GAMMA_G_Y_2;                    /* Address Offset: 0x0070 */
    __IO uint32_t ISP_GAMMA_G_Y_3;                    /* Address Offset: 0x0074 */
    __IO uint32_t ISP_GAMMA_G_Y_4;                    /* Address Offset: 0x0078 */
    __IO uint32_t ISP_GAMMA_G_Y_5;                    /* Address Offset: 0x007C */
    __IO uint32_t ISP_GAMMA_G_Y_6;                    /* Address Offset: 0x0080 */
    __IO uint32_t ISP_GAMMA_G_Y_7;                    /* Address Offset: 0x0084 */
    __IO uint32_t ISP_GAMMA_G_Y_8;                    /* Address Offset: 0x0088 */
    __IO uint32_t ISP_GAMMA_G_Y_9;                    /* Address Offset: 0x008C */
    __IO uint32_t ISP_GAMMA_G_Y_10;                   /* Address Offset: 0x0090 */
    __IO uint32_t ISP_GAMMA_G_Y_11;                   /* Address Offset: 0x0094 */
    __IO uint32_t ISP_GAMMA_G_Y_12;                   /* Address Offset: 0x0098 */
    __IO uint32_t ISP_GAMMA_G_Y_13;                   /* Address Offset: 0x009C */
    __IO uint32_t ISP_GAMMA_G_Y_14;                   /* Address Offset: 0x00A0 */
    __IO uint32_t ISP_GAMMA_G_Y_15;                   /* Address Offset: 0x00A4 */
    __IO uint32_t ISP_GAMMA_G_Y_16;                   /* Address Offset: 0x00A8 */
    __IO uint32_t ISP_GAMMA_B_Y_0;                    /* Address Offset: 0x00AC */
    __IO uint32_t ISP_GAMMA_B_Y_1;                    /* Address Offset: 0x00B0 */
    __IO uint32_t ISP_GAMMA_B_Y_2;                    /* Address Offset: 0x00B4 */
    __IO uint32_t ISP_GAMMA_B_Y_3;                    /* Address Offset: 0x00B8 */
    __IO uint32_t ISP_GAMMA_B_Y_4;                    /* Address Offset: 0x00BC */
    __IO uint32_t ISP_GAMMA_B_Y_5;                    /* Address Offset: 0x00C0 */
    __IO uint32_t ISP_GAMMA_B_Y_6;                    /* Address Offset: 0x00C4 */
    __IO uint32_t ISP_GAMMA_B_Y_7;                    /* Address Offset: 0x00C8 */
    __IO uint32_t ISP_GAMMA_B_Y_8;                    /* Address Offset: 0x00CC */
    __IO uint32_t ISP_GAMMA_B_Y_9;                    /* Address Offset: 0x00D0 */
    __IO uint32_t ISP_GAMMA_B_Y_10;                   /* Address Offset: 0x00D4 */
    __IO uint32_t ISP_GAMMA_B_Y_11;                   /* Address Offset: 0x00D8 */
    __IO uint32_t ISP_GAMMA_B_Y_12;                   /* Address Offset: 0x00DC */
    __IO uint32_t ISP_GAMMA_B_Y_13;                   /* Address Offset: 0x00E0 */
    __IO uint32_t ISP_GAMMA_B_Y_14;                   /* Address Offset: 0x00E4 */
    __IO uint32_t ISP_GAMMA_B_Y_15;                   /* Address Offset: 0x00E8 */
    __IO uint32_t ISP_GAMMA_B_Y_16;                   /* Address Offset: 0x00EC */
         uint32_t RESERVED00F0[8];                    /* Address Offset: 0x00F0 */
    __IO uint32_t ISP_AWB_PROP;                       /* Address Offset: 0x0110 */
    __IO uint32_t ISP_AWB_H_OFFS;                     /* Address Offset: 0x0114 */
    __IO uint32_t ISP_AWB_V_OFFS;                     /* Address Offset: 0x0118 */
    __IO uint32_t ISP_AWB_H_SIZE;                     /* Address Offset: 0x011C */
    __IO uint32_t ISP_AWB_V_SIZE;                     /* Address Offset: 0x0120 */
    __IO uint32_t ISP_AWB_FRAMES;                     /* Address Offset: 0x0124 */
    __IO uint32_t ISP_AWB_REF;                        /* Address Offset: 0x0128 */
    __IO uint32_t ISP_AWB_THRESH;                     /* Address Offset: 0x012C */
         uint32_t RESERVED0130[2];                    /* Address Offset: 0x0130 */
    __IO uint32_t ISP_AWB_GAIN_G;                     /* Address Offset: 0x0138 */
    __IO uint32_t ISP_AWB_GAIN_RB;                    /* Address Offset: 0x013C */
    __I  uint32_t ISP_AWB_WHITE_CNT;                  /* Address Offset: 0x0140 */
    __I  uint32_t ISP_AWB_MEAN;                       /* Address Offset: 0x0144 */
         uint32_t RESERVED0148[10];                   /* Address Offset: 0x0148 */
    __IO uint32_t ISP_CC_COEFF_0;                     /* Address Offset: 0x0170 */
    __IO uint32_t ISP_CC_COEFF_1;                     /* Address Offset: 0x0174 */
    __IO uint32_t ISP_CC_COEFF_2;                     /* Address Offset: 0x0178 */
    __IO uint32_t ISP_CC_COEFF_3;                     /* Address Offset: 0x017C */
    __IO uint32_t ISP_CC_COEFF_4;                     /* Address Offset: 0x0180 */
    __IO uint32_t ISP_CC_COEFF_5;                     /* Address Offset: 0x0184 */
    __IO uint32_t ISP_CC_COEFF_6;                     /* Address Offset: 0x0188 */
    __IO uint32_t ISP_CC_COEFF_7;                     /* Address Offset: 0x018C */
    __IO uint32_t ISP_CC_COEFF_8;                     /* Address Offset: 0x0190 */
    __IO uint32_t ISP_OUT_H_OFFS;                     /* Address Offset: 0x0194 */
    __IO uint32_t ISP_OUT_V_OFFS;                     /* Address Offset: 0x0198 */
    __IO uint32_t ISP_OUT_H_SIZE;                     /* Address Offset: 0x019C */
    __IO uint32_t ISP_OUT_V_SIZE;                     /* Address Offset: 0x01A0 */
    __IO uint32_t ISP_DEMOSAIC;                       /* Address Offset: 0x01A4 */
    __I  uint32_t ISP_FLAGS_SHD;                      /* Address Offset: 0x01A8 */
    __I  uint32_t ISP_OUT_H_OFFS_SHD;                 /* Address Offset: 0x01AC */
    __I  uint32_t ISP_OUT_V_OFFS_SHD;                 /* Address Offset: 0x01B0 */
    __I  uint32_t ISP_OUT_H_SIZE_SHD;                 /* Address Offset: 0x01B4 */
    __I  uint32_t ISP_OUT_V_SIZE_SHD;                 /* Address Offset: 0x01B8 */
    __IO uint32_t ISP_IMSC;                           /* Address Offset: 0x01BC */
    __I  uint32_t ISP_RIS;                            /* Address Offset: 0x01C0 */
    __I  uint32_t ISP_MIS;                            /* Address Offset: 0x01C4 */
    __O  uint32_t ISP_ICR;                            /* Address Offset: 0x01C8 */
    __O  uint32_t ISP_ISR;                            /* Address Offset: 0x01CC */
    __IO uint32_t ISP_CT_COEFF_0;                     /* Address Offset: 0x01D0 */
    __IO uint32_t ISP_CT_COEFF_1;                     /* Address Offset: 0x01D4 */
    __IO uint32_t ISP_CT_COEFF_2;                     /* Address Offset: 0x01D8 */
    __IO uint32_t ISP_CT_COEFF_3;                     /* Address Offset: 0x01DC */
    __IO uint32_t ISP_CT_COEFF_4;                     /* Address Offset: 0x01E0 */
    __IO uint32_t ISP_CT_COEFF_5;                     /* Address Offset: 0x01E4 */
    __IO uint32_t ISP_CT_COEFF_6;                     /* Address Offset: 0x01E8 */
    __IO uint32_t ISP_CT_COEFF_7;                     /* Address Offset: 0x01EC */
    __IO uint32_t ISP_CT_COEFF_8;                     /* Address Offset: 0x01F0 */
    __IO uint32_t ISP_GAMMA_OUT_MODE;                 /* Address Offset: 0x01F4 */
    __IO uint32_t ISP_GAMMA_OUT_Y_0;                  /* Address Offset: 0x01F8 */
    __IO uint32_t ISP_GAMMA_OUT_Y_1;                  /* Address Offset: 0x01FC */
    __IO uint32_t ISP_GAMMA_OUT_Y_2;                  /* Address Offset: 0x0200 */
    __IO uint32_t ISP_GAMMA_OUT_Y_3;                  /* Address Offset: 0x0204 */
    __IO uint32_t ISP_GAMMA_OUT_Y_4;                  /* Address Offset: 0x0208 */
    __IO uint32_t ISP_GAMMA_OUT_Y_5;                  /* Address Offset: 0x020C */
    __IO uint32_t ISP_GAMMA_OUT_Y_6;                  /* Address Offset: 0x0210 */
    __IO uint32_t ISP_GAMMA_OUT_Y_7;                  /* Address Offset: 0x0214 */
    __IO uint32_t ISP_GAMMA_OUT_Y_8;                  /* Address Offset: 0x0218 */
    __IO uint32_t ISP_GAMMA_OUT_Y_9;                  /* Address Offset: 0x021C */
    __IO uint32_t ISP_GAMMA_OUT_Y_10;                 /* Address Offset: 0x0220 */
    __IO uint32_t ISP_GAMMA_OUT_Y_11;                 /* Address Offset: 0x0224 */
    __IO uint32_t ISP_GAMMA_OUT_Y_12;                 /* Address Offset: 0x0228 */
    __IO uint32_t ISP_GAMMA_OUT_Y_13;                 /* Address Offset: 0x022C */
    __IO uint32_t ISP_GAMMA_OUT_Y_14;                 /* Address Offset: 0x0230 */
    __IO uint32_t ISP_GAMMA_OUT_Y_15;                 /* Address Offset: 0x0234 */
    __IO uint32_t ISP_GAMMA_OUT_Y_16;                 /* Address Offset: 0x0238 */
    __I  uint32_t ISP_ERR;                            /* Address Offset: 0x023C */
    __O  uint32_t ISP_ERR_CLR;                        /* Address Offset: 0x0240 */
    __I  uint32_t ISP_FRAME_COUNT;                    /* Address Offset: 0x0244 */
    __IO uint32_t ISP_CT_OFFSET_R;                    /* Address Offset: 0x0248 */
    __IO uint32_t ISP_CT_OFFSET_G;                    /* Address Offset: 0x024C */
    __IO uint32_t ISP_CT_OFFSET_B;                    /* Address Offset: 0x0250 */
};
/* ISP_CPROC Register Structure Define */
struct ISP_CPROC_REG {
    __IO uint32_t CPROC_CTRL;                         /* Address Offset: 0x0000 */
    __IO uint32_t CPROC_CONTRAST;                     /* Address Offset: 0x0004 */
    __IO uint32_t CPROC_BRIGHTNESS;                   /* Address Offset: 0x0008 */
    __IO uint32_t CPROC_SATURATION;                   /* Address Offset: 0x000C */
    __IO uint32_t CPROC_HUE;                          /* Address Offset: 0x0010 */
};
/* ISP_MAIN_RESIZE Register Structure Define */
struct ISP_MAIN_RESIZE_REG {
    __IO uint32_t MRSZ_CTRL;                          /* Address Offset: 0x0000 */
    __IO uint32_t MRSZ_SCALE_HY;                      /* Address Offset: 0x0004 */
    __IO uint32_t MRSZ_SCALE_HCB;                     /* Address Offset: 0x0008 */
    __IO uint32_t MRSZ_SCALE_HCR;                     /* Address Offset: 0x000C */
    __IO uint32_t MRSZ_SCALE_VY;                      /* Address Offset: 0x0010 */
    __IO uint32_t MRSZ_SCALE_VC;                      /* Address Offset: 0x0014 */
    __IO uint32_t MRSZ_PHASE_HY;                      /* Address Offset: 0x0018 */
    __IO uint32_t MRSZ_PHASE_HC;                      /* Address Offset: 0x001C */
    __IO uint32_t MRSZ_PHASE_VY;                      /* Address Offset: 0x0020 */
    __IO uint32_t MRSZ_PHASE_VC;                      /* Address Offset: 0x0024 */
    __IO uint32_t MRSZ_SCALE_LUT_ADDR;                /* Address Offset: 0x0028 */
    __IO uint32_t MRSZ_SCALE_LUT;                     /* Address Offset: 0x002C */
    __I  uint32_t MRSZ_CTRL_SHD;                      /* Address Offset: 0x0030 */
    __I  uint32_t MRSZ_SCALE_HY_SHD;                  /* Address Offset: 0x0034 */
    __I  uint32_t MRSZ_SCALE_HCB_SHD;                 /* Address Offset: 0x0038 */
    __I  uint32_t MRSZ_SCALE_HCR_SHD;                 /* Address Offset: 0x003C */
    __I  uint32_t MRSZ_SCALE_VY_SHD;                  /* Address Offset: 0x0040 */
    __I  uint32_t MRSZ_SCALE_VC_SHD;                  /* Address Offset: 0x0044 */
    __I  uint32_t MRSZ_PHASE_HY_SHD;                  /* Address Offset: 0x0048 */
    __I  uint32_t MRSZ_PHASE_HC_SHD;                  /* Address Offset: 0x004C */
    __I  uint32_t MRSZ_PHASE_VY_SHD;                  /* Address Offset: 0x0050 */
    __I  uint32_t MRSZ_PHASE_VC_SHD;                  /* Address Offset: 0x0054 */
};
/* ISP_MI Register Structure Define */
struct ISP_MI_REG {
    __IO uint32_t MI_CTRL;                            /* Address Offset: 0x0000 */
    __O  uint32_t MI_INIT;                            /* Address Offset: 0x0004 */
    __IO uint32_t MI_MP_Y_BASE_AD_INIT;               /* Address Offset: 0x0008 */
    __IO uint32_t MI_MP_Y_SIZE_INIT;                  /* Address Offset: 0x000C */
    __IO uint32_t MI_MP_Y_OFFS_CNT_INIT;              /* Address Offset: 0x0010 */
    __I  uint32_t MI_MP_Y_OFFS_CNT_START;             /* Address Offset: 0x0014 */
    __IO uint32_t MI_MP_Y_IRQ_OFFS_INIT;              /* Address Offset: 0x0018 */
    __IO uint32_t MI_MP_CB_BASE_AD_INIT;              /* Address Offset: 0x001C */
    __IO uint32_t MI_MP_CB_SIZE_INIT;                 /* Address Offset: 0x0020 */
    __IO uint32_t MI_MP_CB_OFFS_CNT_INIT;             /* Address Offset: 0x0024 */
    __I  uint32_t MI_MP_CB_OFFS_CNT_START;            /* Address Offset: 0x0028 */
    __IO uint32_t MI_MP_CR_BASE_AD_INIT;              /* Address Offset: 0x002C */
    __IO uint32_t MI_MP_CR_SIZE_INIT;                 /* Address Offset: 0x0030 */
    __IO uint32_t MI_MP_CR_OFFS_CNT_INIT;             /* Address Offset: 0x0034 */
    __I  uint32_t MI_MP_CR_OFFS_CNT_START;            /* Address Offset: 0x0038 */
    __IO uint32_t MI_SP_Y_BASE_AD_INIT;               /* Address Offset: 0x003C */
    __IO uint32_t MI_SP_Y_SIZE_INIT;                  /* Address Offset: 0x0040 */
    __IO uint32_t MI_SP_Y_OFFS_CNT_INIT;              /* Address Offset: 0x0044 */
    __I  uint32_t MI_SP_Y_OFFS_CNT_START;             /* Address Offset: 0x0048 */
    __IO uint32_t MI_SP_Y_LLENGTH;                    /* Address Offset: 0x004C */
    __IO uint32_t MI_SP_CB_BASE_AD_INIT;              /* Address Offset: 0x0050 */
    __IO uint32_t MI_SP_CB_SIZE_INIT;                 /* Address Offset: 0x0054 */
    __IO uint32_t MI_SP_CB_OFFS_CNT_INIT;             /* Address Offset: 0x0058 */
    __I  uint32_t MI_SP_CB_OFFS_CNT_START;            /* Address Offset: 0x005C */
    __IO uint32_t MI_SP_CR_BASE_AD_INIT;              /* Address Offset: 0x0060 */
    __IO uint32_t MI_SP_CR_SIZE_INIT;                 /* Address Offset: 0x0064 */
    __IO uint32_t MI_SP_CR_OFFS_CNT_INIT;             /* Address Offset: 0x0068 */
    __I  uint32_t MI_SP_CR_OFFS_CNT_START;            /* Address Offset: 0x006C */
    __I  uint32_t MI_BYTE_CNT;                        /* Address Offset: 0x0070 */
    __I  uint32_t MI_CTRL_SHD;                        /* Address Offset: 0x0074 */
    __I  uint32_t MI_MP_Y_BASE_AD_SHD;                /* Address Offset: 0x0078 */
    __I  uint32_t MI_MP_Y_SIZE_SHD;                   /* Address Offset: 0x007C */
    __I  uint32_t MI_MP_Y_OFFS_CNT_SHD;               /* Address Offset: 0x0080 */
    __I  uint32_t MI_MP_Y_IRQ_OFFS_SHD;               /* Address Offset: 0x0084 */
    __I  uint32_t MI_MP_CB_BASE_AD_SHD;               /* Address Offset: 0x0088 */
    __I  uint32_t MI_MP_CB_SIZE_SHD;                  /* Address Offset: 0x008C */
    __I  uint32_t MI_MP_CB_OFFS_CNT_SHD;              /* Address Offset: 0x0090 */
    __I  uint32_t MI_MP_CR_BASE_AD_SHD;               /* Address Offset: 0x0094 */
    __I  uint32_t MI_MP_CR_SIZE_SHD;                  /* Address Offset: 0x0098 */
    __I  uint32_t MI_MP_CR_OFFS_CNT_SHD;              /* Address Offset: 0x009C */
    __I  uint32_t MI_SP_Y_BASE_AD_SHD;                /* Address Offset: 0x00A0 */
    __I  uint32_t MI_SP_Y_SIZE_SHD;                   /* Address Offset: 0x00A4 */
    __I  uint32_t MI_SP_Y_OFFS_CNT_SHD;               /* Address Offset: 0x00A8 */
         uint32_t RESERVED00AC;                       /* Address Offset: 0x00AC */
    __I  uint32_t MI_SP_CB_BASE_AD_SHD;               /* Address Offset: 0x00B0 */
    __I  uint32_t MI_SP_CB_SIZE_SHD;                  /* Address Offset: 0x00B4 */
    __I  uint32_t MI_SP_CB_OFFS_CNT_SHD;              /* Address Offset: 0x00B8 */
    __I  uint32_t MI_SP_CR_BASE_AD_SHD;               /* Address Offset: 0x00BC */
    __I  uint32_t MI_SP_CR_SIZE_SHD;                  /* Address Offset: 0x00C0 */
    __I  uint32_t MI_SP_CR_OFFS_CNT_SHD;              /* Address Offset: 0x00C4 */
    __IO uint32_t MI_DMA_Y_PIC_START_AD;              /* Address Offset: 0x00C8 */
    __IO uint32_t MI_DMA_Y_PIC_WIDTH;                 /* Address Offset: 0x00CC */
    __IO uint32_t MI_DMA_Y_LLENGTH;                   /* Address Offset: 0x00D0 */
    __IO uint32_t MI_DMA_Y_PIC_SIZE;                  /* Address Offset: 0x00D4 */
    __IO uint32_t MI_DMA_CB_PIC_START_AD;             /* Address Offset: 0x00D8 */
         uint32_t RESERVED00DC[3];                    /* Address Offset: 0x00DC */
    __IO uint32_t MI_DMA_CR_PIC_START_AD;             /* Address Offset: 0x00E8 */
         uint32_t RESERVED00EC[3];                    /* Address Offset: 0x00EC */
    __IO uint32_t MI_IMSC;                            /* Address Offset: 0x00F8 */
    __I  uint32_t MI_RIS;                             /* Address Offset: 0x00FC */
    __I  uint32_t MI_MIS;                             /* Address Offset: 0x0100 */
    __O  uint32_t MI_ICR;                             /* Address Offset: 0x0104 */
    __O  uint32_t MI_ISR;                             /* Address Offset: 0x0108 */
    __I  uint32_t MI_STATUS;                          /* Address Offset: 0x010C */
    __O  uint32_t MI_STATUS_CLR;                      /* Address Offset: 0x0110 */
    __IO uint32_t MI_SP_Y_PIC_WIDTH;                  /* Address Offset: 0x0114 */
    __IO uint32_t MI_SP_Y_PIC_HEIGHT;                 /* Address Offset: 0x0118 */
    __IO uint32_t MI_SP_Y_PIC_SIZE;                   /* Address Offset: 0x011C */
    __IO uint32_t MI_DMA_CTRL;                        /* Address Offset: 0x0120 */
    __O  uint32_t MI_DMA_START;                       /* Address Offset: 0x0124 */
    __I  uint32_t MI_DMA_STATUS;                      /* Address Offset: 0x0128 */
    __I  uint32_t MI_PIXEL_CNT;                       /* Address Offset: 0x012C */
    __IO uint32_t MI_MP_Y_BASE_AD_INIT2;              /* Address Offset: 0x0130 */
    __IO uint32_t MI_MP_CB_BASE_AD_INIT2;             /* Address Offset: 0x0134 */
    __IO uint32_t MI_MP_CR_BASE_AD_INIT2;             /* Address Offset: 0x0138 */
    __IO uint32_t MI_SP_Y_BASE_AD_INIT2;              /* Address Offset: 0x013C */
    __IO uint32_t MI_SP_CB_BASE_AD_INIT2;             /* Address Offset: 0x0140 */
    __IO uint32_t MI_SP_CR_BASE_AD_INIT2;             /* Address Offset: 0x0144 */
    __IO uint32_t MI_XTD_FORMAT_CTRL;                 /* Address Offset: 0x0148 */
};
/* ISP_MIPI Register Structure Define */
struct ISP_MIPI_REG {
    __IO uint32_t MIPI_CTRL;                          /* Address Offset: 0x0000 */
    __I  uint32_t MIPI_STATUS;                        /* Address Offset: 0x0004 */
    __IO uint32_t MIPI_IMSC;                          /* Address Offset: 0x0008 */
    __I  uint32_t MIPI_RIS;                           /* Address Offset: 0x000C */
    __I  uint32_t MIPI_MIS;                           /* Address Offset: 0x0010 */
    __O  uint32_t MIPI_ICR;                           /* Address Offset: 0x0014 */
    __O  uint32_t MIPI_ISR;                           /* Address Offset: 0x0018 */
    __I  uint32_t MIPI_CUR_DATA_ID;                   /* Address Offset: 0x001C */
    __IO uint32_t MIPI_IMG_DATA_SEL;                  /* Address Offset: 0x0020 */
    __IO uint32_t MIPI_ADD_DATA_SEL_1;                /* Address Offset: 0x0024 */
    __IO uint32_t MIPI_ADD_DATA_SEL_2;                /* Address Offset: 0x0028 */
    __IO uint32_t MIPI_ADD_DATA_SEL_3;                /* Address Offset: 0x002C */
    __IO uint32_t MIPI_ADD_DATA_SEL_4;                /* Address Offset: 0x0030 */
    __I  uint32_t MIPI_ADD_DATA_FIFO;                 /* Address Offset: 0x0034 */
    __IO uint32_t MIPI_ADD_DATA_FILL_LEVEL;           /* Address Offset: 0x0038 */
    __IO uint32_t MIPI_COMPRESSED_MODE;               /* Address Offset: 0x003C */
    __I  uint32_t MIPI_FRAME;                         /* Address Offset: 0x0040 */
    __I  uint32_t MIPI_GEN_SHORT_DT;                  /* Address Offset: 0x0044 */
    __I  uint32_t MIPI_GEN_SHORT_8_9;                 /* Address Offset: 0x0048 */
    __I  uint32_t MIPI_GEN_SHORT_A_B;                 /* Address Offset: 0x004C */
    __I  uint32_t MIPI_GEN_SHORT_C_D;                 /* Address Offset: 0x0050 */
    __I  uint32_t MIPI_GEN_SHORT_E_F;                 /* Address Offset: 0x0054 */
};
/* ISP_AFM Register Structure Define */
struct ISP_AFM_REG {
    __IO uint32_t ISP_AFM_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_AFM_LT_A;                       /* Address Offset: 0x0004 */
    __IO uint32_t ISP_AFM_RB_A;                       /* Address Offset: 0x0008 */
    __IO uint32_t ISP_AFM_LT_B;                       /* Address Offset: 0x000C */
    __IO uint32_t ISP_AFM_RB_B;                       /* Address Offset: 0x0010 */
    __IO uint32_t ISP_AFM_LT_C;                       /* Address Offset: 0x0014 */
    __IO uint32_t ISP_AFM_RB_C;                       /* Address Offset: 0x0018 */
    __IO uint32_t ISP_AFM_THRES;                      /* Address Offset: 0x001C */
    __IO uint32_t ISP_AFM_VAR_SHIFT;                  /* Address Offset: 0x0020 */
    __I  uint32_t ISP_AFM_SUM_A;                      /* Address Offset: 0x0024 */
    __I  uint32_t ISP_AFM_SUM_B;                      /* Address Offset: 0x0028 */
    __I  uint32_t ISP_AFM_SUM_C;                      /* Address Offset: 0x002C */
    __I  uint32_t ISP_AFM_LUM_A;                      /* Address Offset: 0x0030 */
    __I  uint32_t ISP_AFM_LUM_B;                      /* Address Offset: 0x0034 */
    __I  uint32_t ISP_AFM_LUM_C;                      /* Address Offset: 0x0038 */
};
/* ISP_LSC Register Structure Define */
struct ISP_LSC_REG {
    __IO uint32_t ISP_LSC_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_LSC_R_TABLE_ADDR;               /* Address Offset: 0x0004 */
    __IO uint32_t ISP_LSC_GR_TABLE_ADDR;              /* Address Offset: 0x0008 */
    __IO uint32_t ISP_LSC_B_TABLE_ADDR;               /* Address Offset: 0x000C */
    __IO uint32_t ISP_LSC_GB_TABLE_ADDR;              /* Address Offset: 0x0010 */
    __IO uint32_t ISP_LSC_R_TABLE_DATA;               /* Address Offset: 0x0014 */
    __IO uint32_t ISP_LSC_GR_TABLE_DATA;              /* Address Offset: 0x0018 */
    __IO uint32_t ISP_LSC_B_TABLE_DATA;               /* Address Offset: 0x001C */
    __IO uint32_t ISP_LSC_GB_TABLE_DATA;              /* Address Offset: 0x0020 */
    __IO uint32_t ISP_LSC_XGRAD_01;                   /* Address Offset: 0x0024 */
    __IO uint32_t ISP_LSC_XGRAD_23;                   /* Address Offset: 0x0028 */
    __IO uint32_t ISP_LSC_XGRAD_45;                   /* Address Offset: 0x002C */
    __IO uint32_t ISP_LSC_XGRAD_67;                   /* Address Offset: 0x0030 */
    __IO uint32_t ISP_LSC_YGRAD_01;                   /* Address Offset: 0x0034 */
    __IO uint32_t ISP_LSC_YGRAD_23;                   /* Address Offset: 0x0038 */
    __IO uint32_t ISP_LSC_YGRAD_45;                   /* Address Offset: 0x003C */
    __IO uint32_t ISP_LSC_YGRAD_67;                   /* Address Offset: 0x0040 */
    __IO uint32_t ISP_LSC_XSIZE_01;                   /* Address Offset: 0x0044 */
    __IO uint32_t ISP_LSC_XSIZE_23;                   /* Address Offset: 0x0048 */
    __IO uint32_t ISP_LSC_XSIZE_45;                   /* Address Offset: 0x004C */
    __IO uint32_t ISP_LSC_XSIZE_67;                   /* Address Offset: 0x0050 */
    __IO uint32_t ISP_LSC_YSIZE_01;                   /* Address Offset: 0x0054 */
    __IO uint32_t ISP_LSC_YSIZE_23;                   /* Address Offset: 0x0058 */
    __IO uint32_t ISP_LSC_YSIZE_45;                   /* Address Offset: 0x005C */
    __IO uint32_t ISP_LSC_YSIZE_67;                   /* Address Offset: 0x0060 */
    __IO uint32_t ISP_LSC_TABLE_SEL;                  /* Address Offset: 0x0064 */
    __I  uint32_t ISP_LSC_STATUS;                     /* Address Offset: 0x0068 */
};
/* ISP_IS Register Structure Define */
struct ISP_IS_REG {
    __IO uint32_t ISP_IS_CTRL;                        /* Address Offset: 0x0000 */
    __IO uint32_t ISP_IS_RECENTER;                    /* Address Offset: 0x0004 */
    __IO uint32_t ISP_IS_H_OFFS;                      /* Address Offset: 0x0008 */
    __IO uint32_t ISP_IS_V_OFFS;                      /* Address Offset: 0x000C */
    __IO uint32_t ISP_IS_H_SIZE;                      /* Address Offset: 0x0010 */
    __IO uint32_t ISP_IS_V_SIZE;                      /* Address Offset: 0x0014 */
    __IO uint32_t ISP_IS_MAX_DX;                      /* Address Offset: 0x0018 */
    __IO uint32_t ISP_IS_MAX_DY;                      /* Address Offset: 0x001C */
    __IO uint32_t ISP_IS_DISPLACE;                    /* Address Offset: 0x0020 */
    __I  uint32_t ISP_IS_H_OFFS_SHD;                  /* Address Offset: 0x0024 */
    __I  uint32_t ISP_IS_V_OFFS_SHD;                  /* Address Offset: 0x0028 */
    __I  uint32_t ISP_IS_H_SIZE_SHD;                  /* Address Offset: 0x002C */
    __I  uint32_t ISP_IS_V_SIZE_SHD;                  /* Address Offset: 0x0030 */
};
/* ISP_HIST Register Structure Define */
struct ISP_HIST_REG {
    __IO uint32_t ISP_HIST_PROP;                      /* Address Offset: 0x0000 */
    __IO uint32_t ISP_HIST_H_OFFS;                    /* Address Offset: 0x0004 */
    __IO uint32_t ISP_HIST_V_OFFS;                    /* Address Offset: 0x0008 */
    __IO uint32_t ISP_HIST_H_SIZE;                    /* Address Offset: 0x000C */
    __IO uint32_t ISP_HIST_V_SIZE;                    /* Address Offset: 0x0010 */
    __I  uint32_t ISP_HIST_BIN_0;                     /* Address Offset: 0x0014 */
    __I  uint32_t ISP_HIST_BIN_1;                     /* Address Offset: 0x0018 */
    __I  uint32_t ISP_HIST_BIN_2;                     /* Address Offset: 0x001C */
    __I  uint32_t ISP_HIST_BIN_3;                     /* Address Offset: 0x0020 */
    __I  uint32_t ISP_HIST_BIN_4;                     /* Address Offset: 0x0024 */
    __I  uint32_t ISP_HIST_BIN_5;                     /* Address Offset: 0x0028 */
    __I  uint32_t ISP_HIST_BIN_6;                     /* Address Offset: 0x002C */
    __I  uint32_t ISP_HIST_BIN_7;                     /* Address Offset: 0x0030 */
    __I  uint32_t ISP_HIST_BIN_8;                     /* Address Offset: 0x0034 */
    __I  uint32_t ISP_HIST_BIN_9;                     /* Address Offset: 0x0038 */
    __I  uint32_t ISP_HIST_BIN_10;                    /* Address Offset: 0x003C */
    __I  uint32_t ISP_HIST_BIN_11;                    /* Address Offset: 0x0040 */
    __I  uint32_t ISP_HIST_BIN_12;                    /* Address Offset: 0x0044 */
    __I  uint32_t ISP_HIST_BIN_13;                    /* Address Offset: 0x0048 */
    __I  uint32_t ISP_HIST_BIN_14;                    /* Address Offset: 0x004C */
    __I  uint32_t ISP_HIST_BIN_15;                    /* Address Offset: 0x0050 */
    __IO uint32_t ISP_HIST_WEIGHT_00TO30;             /* Address Offset: 0x0054 */
    __IO uint32_t ISP_HIST_WEIGHT_40TO21;             /* Address Offset: 0x0058 */
    __IO uint32_t ISP_HIST_WEIGHT_31TO12;             /* Address Offset: 0x005C */
    __IO uint32_t ISP_HIST_WEIGHT_22TO03;             /* Address Offset: 0x0060 */
    __IO uint32_t ISP_HIST_WEIGHT_13TO43;             /* Address Offset: 0x0064 */
    __IO uint32_t ISP_HIST_WEIGHT_04TO34;             /* Address Offset: 0x0068 */
    __IO uint32_t ISP_HIST_WEIGHT_44;                 /* Address Offset: 0x006C */
};
/* ISP_FILTER Register Structure Define */
struct ISP_FILTER_REG {
    __IO uint32_t ISP_FILT_MODE;                      /* Address Offset: 0x0000 */
         uint32_t RESERVED0004[9];                    /* Address Offset: 0x0004 */
    __IO uint32_t ISP_FILT_THRESH_BL0;                /* Address Offset: 0x0028 */
    __IO uint32_t ISP_FILT_THRESH_BL1;                /* Address Offset: 0x002C */
    __IO uint32_t ISP_FILT_THRESH_SH0;                /* Address Offset: 0x0030 */
    __IO uint32_t ISP_FILT_THRESH_SH1;                /* Address Offset: 0x0034 */
    __IO uint32_t ISP_FILT_LUM_WEIGHT;                /* Address Offset: 0x0038 */
    __IO uint32_t ISP_FILT_FAC_SH1;                   /* Address Offset: 0x003C */
    __IO uint32_t ISP_FILT_FAC_SH0;                   /* Address Offset: 0x0040 */
    __IO uint32_t ISP_FILT_FAC_MID;                   /* Address Offset: 0x0044 */
    __IO uint32_t ISP_FILT_FAC_BL0;                   /* Address Offset: 0x0048 */
    __IO uint32_t ISP_FILT_FAC_BL1;                   /* Address Offset: 0x004C */
};
/* ISP_CAC Register Structure Define */
struct ISP_CAC_REG {
    __IO uint32_t ISP_CAC_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_CAC_COUNT_START;                /* Address Offset: 0x0004 */
    __IO uint32_t ISP_CAC_A;                          /* Address Offset: 0x0008 */
    __IO uint32_t ISP_CAC_B;                          /* Address Offset: 0x000C */
    __IO uint32_t ISP_CAC_C;                          /* Address Offset: 0x0010 */
    __IO uint32_t ISP_CAC_X_NORM;                     /* Address Offset: 0x0014 */
    __IO uint32_t ISP_CAC_Y_NORM;                     /* Address Offset: 0x0018 */
};
/* ISP_EXPOSURE Register Structure Define */
struct ISP_EXPOSURE_REG {
    __IO uint32_t ISP_EXP_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_EXP_H_OFFSET;                   /* Address Offset: 0x0004 */
    __IO uint32_t ISP_EXP_V_OFFSET;                   /* Address Offset: 0x0008 */
    __IO uint32_t ISP_EXP_H_SIZE;                     /* Address Offset: 0x000C */
    __IO uint32_t ISP_EXP_V_SIZE;                     /* Address Offset: 0x0010 */
    __I  uint32_t ISP_EXP_MEAN_00;                    /* Address Offset: 0x0014 */
    __I  uint32_t ISP_EXP_MEAN_10;                    /* Address Offset: 0x0018 */
    __I  uint32_t ISP_EXP_MEAN_20;                    /* Address Offset: 0x001C */
    __I  uint32_t ISP_EXP_MEAN_30;                    /* Address Offset: 0x0020 */
    __I  uint32_t ISP_EXP_MEAN_40;                    /* Address Offset: 0x0024 */
    __I  uint32_t ISP_EXP_MEAN_01;                    /* Address Offset: 0x0028 */
    __I  uint32_t ISP_EXP_MEAN_11;                    /* Address Offset: 0x002C */
    __I  uint32_t ISP_EXP_MEAN_21;                    /* Address Offset: 0x0030 */
    __I  uint32_t ISP_EXP_MEAN_31;                    /* Address Offset: 0x0034 */
    __I  uint32_t ISP_EXP_MEAN_41;                    /* Address Offset: 0x0038 */
    __I  uint32_t ISP_EXP_MEAN_02;                    /* Address Offset: 0x003C */
    __I  uint32_t ISP_EXP_MEAN_12;                    /* Address Offset: 0x0040 */
    __I  uint32_t ISP_EXP_MEAN_22;                    /* Address Offset: 0x0044 */
    __I  uint32_t ISP_EXP_MEAN_32;                    /* Address Offset: 0x0048 */
    __I  uint32_t ISP_EXP_MEAN_42;                    /* Address Offset: 0x004C */
    __I  uint32_t ISP_EXP_MEAN_03;                    /* Address Offset: 0x0050 */
    __I  uint32_t ISP_EXP_MEAN_13;                    /* Address Offset: 0x0054 */
    __I  uint32_t ISP_EXP_MEAN_23;                    /* Address Offset: 0x0058 */
    __I  uint32_t ISP_EXP_MEAN_33;                    /* Address Offset: 0x005C */
    __I  uint32_t ISP_EXP_MEAN_43;                    /* Address Offset: 0x0060 */
    __I  uint32_t ISP_EXP_MEAN_04;                    /* Address Offset: 0x0064 */
    __I  uint32_t ISP_EXP_MEAN_14;                    /* Address Offset: 0x0068 */
    __I  uint32_t ISP_EXP_MEAN_24;                    /* Address Offset: 0x006C */
    __I  uint32_t ISP_EXP_MEAN_34;                    /* Address Offset: 0x0070 */
    __I  uint32_t ISP_EXP_MEAN_44;                    /* Address Offset: 0x0074 */
};
/* ISP_BLS Register Structure Define */
struct ISP_BLS_REG {
    __IO uint32_t ISP_BLS_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_BLS_SAMPLES;                    /* Address Offset: 0x0004 */
    __IO uint32_t ISP_BLS_H1_START;                   /* Address Offset: 0x0008 */
    __IO uint32_t ISP_BLS_H1_STOP;                    /* Address Offset: 0x000C */
    __IO uint32_t ISP_BLS_V1_START;                   /* Address Offset: 0x0010 */
    __IO uint32_t ISP_BLS_V1_STOP;                    /* Address Offset: 0x0014 */
    __IO uint32_t ISP_BLS_H2_START;                   /* Address Offset: 0x0018 */
    __IO uint32_t ISP_BLS_H2_STOP;                    /* Address Offset: 0x001C */
    __IO uint32_t ISP_BLS_V2_START;                   /* Address Offset: 0x0020 */
    __IO uint32_t ISP_BLS_V2_STOP;                    /* Address Offset: 0x0024 */
    __IO uint32_t ISP_BLS_A_FIXED;                    /* Address Offset: 0x0028 */
    __IO uint32_t ISP_BLS_B_FIXED;                    /* Address Offset: 0x002C */
    __IO uint32_t ISP_BLS_C_FIXED;                    /* Address Offset: 0x0030 */
    __IO uint32_t ISP_BLS_D_FIXED;                    /* Address Offset: 0x0034 */
    __I  uint32_t ISP_BLS_A_MEASURED;                 /* Address Offset: 0x0038 */
    __I  uint32_t ISP_BLS_B_MEASURED;                 /* Address Offset: 0x003C */
    __I  uint32_t ISP_BLS_C_MEASURED;                 /* Address Offset: 0x0040 */
    __I  uint32_t ISP_BLS_D_MEASURED;                 /* Address Offset: 0x0044 */
};
/* ISP_DPF Register Structure Define */
struct ISP_DPF_REG {
    __IO uint32_t ISP_DPF_MODE;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_DPF_STRENGTH_R;                 /* Address Offset: 0x0004 */
    __IO uint32_t ISP_DPF_STRENGTH_G;                 /* Address Offset: 0x0008 */
    __IO uint32_t ISP_DPF_STRENGTH_B;                 /* Address Offset: 0x000C */
    __IO uint32_t ISP_DPF_S_WEIGHT_G_1_4;             /* Address Offset: 0x0010 */
    __IO uint32_t ISP_DPF_S_WEIGHT_G_5_6;             /* Address Offset: 0x0014 */
    __IO uint32_t ISP_DPF_S_WEIGHT_RB_1_4;            /* Address Offset: 0x0018 */
    __IO uint32_t ISP_DPF_S_WEIGHT_RB_5_6;            /* Address Offset: 0x001C */
    __IO uint32_t ISP_DPF_NLL_COEFF_0;                /* Address Offset: 0x0020 */
    __IO uint32_t ISP_DPF_NLL_COEFF_1;                /* Address Offset: 0x0024 */
    __IO uint32_t ISP_DPF_NLL_COEFF_2;                /* Address Offset: 0x0028 */
    __IO uint32_t ISP_DPF_NLL_COEFF_3;                /* Address Offset: 0x002C */
    __IO uint32_t ISP_DPF_NLL_COEFF_4;                /* Address Offset: 0x0030 */
    __IO uint32_t ISP_DPF_NLL_COEFF_5;                /* Address Offset: 0x0034 */
    __IO uint32_t ISP_DPF_NLL_COEFF_6;                /* Address Offset: 0x0038 */
    __IO uint32_t ISP_DPF_NLL_COEFF_7;                /* Address Offset: 0x003C */
    __IO uint32_t ISP_DPF_NLL_COEFF_8;                /* Address Offset: 0x0040 */
    __IO uint32_t ISP_DPF_NLL_COEFF_9;                /* Address Offset: 0x0044 */
    __IO uint32_t ISP_DPF_NLL_COEFF_10;               /* Address Offset: 0x0048 */
    __IO uint32_t ISP_DPF_NLL_COEFF_11;               /* Address Offset: 0x004C */
    __IO uint32_t ISP_DPF_NLL_COEFF_12;               /* Address Offset: 0x0050 */
    __IO uint32_t ISP_DPF_NLL_COEFF_13;               /* Address Offset: 0x0054 */
    __IO uint32_t ISP_DPF_NLL_COEFF_14;               /* Address Offset: 0x0058 */
    __IO uint32_t ISP_DPF_NLL_COEFF_15;               /* Address Offset: 0x005C */
    __IO uint32_t ISP_DPF_NLL_COEFF_16;               /* Address Offset: 0x0060 */
    __IO uint32_t ISP_DPF_NF_GAIN_R;                  /* Address Offset: 0x0064 */
    __IO uint32_t ISP_DPF_NF_GAIN_GR;                 /* Address Offset: 0x0068 */
    __IO uint32_t ISP_DPF_NF_GAIN_GB;                 /* Address Offset: 0x006C */
    __IO uint32_t ISP_DPF_NF_GAIN_B;                  /* Address Offset: 0x0070 */
};
/* ISP_DPCC Register Structure Define */
struct ISP_DPCC_REG {
    __IO uint32_t ISP_DPCC_MODE;                      /* Address Offset: 0x0000 */
    __IO uint32_t ISP_DPCC_OUTPUT_MODE;               /* Address Offset: 0x0004 */
    __IO uint32_t ISP_DPCC_SET_USE;                   /* Address Offset: 0x0008 */
    __IO uint32_t ISP_DPCC_METHODS_SET_1;             /* Address Offset: 0x000C */
    __IO uint32_t ISP_DPCC_METHODS_SET_2;             /* Address Offset: 0x0010 */
    __IO uint32_t ISP_DPCC_METHODS_SET_3;             /* Address Offset: 0x0014 */
    __IO uint32_t ISP_DPCC_LINE_THRESH_1;             /* Address Offset: 0x0018 */
    __IO uint32_t ISP_DPCC_LINE_MAD_FAC_1;            /* Address Offset: 0x001C */
    __IO uint32_t ISP_DPCC_PG_FAC_1;                  /* Address Offset: 0x0020 */
    __IO uint32_t ISP_DPCC_RND_THRESH_1;              /* Address Offset: 0x0024 */
    __IO uint32_t ISP_DPCC_RG_FAC_1;                  /* Address Offset: 0x0028 */
    __IO uint32_t ISP_DPCC_LINE_THRESH_2;             /* Address Offset: 0x002C */
    __IO uint32_t ISP_DPCC_LINE_MAD_FAC_2;            /* Address Offset: 0x0030 */
    __IO uint32_t ISP_DPCC_PG_FAC_2;                  /* Address Offset: 0x0034 */
    __IO uint32_t ISP_DPCC_RND_THRESH_2;              /* Address Offset: 0x0038 */
    __IO uint32_t ISP_DPCC_RG_FAC_2;                  /* Address Offset: 0x003C */
    __IO uint32_t ISP_DPCC_LINE_THRESH_3;             /* Address Offset: 0x0040 */
    __IO uint32_t ISP_DPCC_LINE_MAD_FAC_3;            /* Address Offset: 0x0044 */
    __IO uint32_t ISP_DPCC_PG_FAC_3;                  /* Address Offset: 0x0048 */
    __IO uint32_t ISP_DPCC_RND_THRESH_3;              /* Address Offset: 0x004C */
    __IO uint32_t ISP_DPCC_RG_FAC_3;                  /* Address Offset: 0x0050 */
    __IO uint32_t ISP_DPCC_RO_LIMITS;                 /* Address Offset: 0x0054 */
    __IO uint32_t ISP_DPCC_RND_OFFS;                  /* Address Offset: 0x0058 */
    __IO uint32_t ISP_DPCC_BPT_CTRL;                  /* Address Offset: 0x005C */
    __IO uint32_t ISP_DPCC_BPT_NUMBER;                /* Address Offset: 0x0060 */
    __IO uint32_t ISP_DPCC_BPT_ADDR;                  /* Address Offset: 0x0064 */
    __IO uint32_t ISP_DPCC_BPT_DATA;                  /* Address Offset: 0x0068 */
};
/* ISP_WDR Register Structure Define */
struct ISP_WDR_REG {
    __IO uint32_t ISP_WDR_CTRL;                       /* Address Offset: 0x0000 */
    __IO uint32_t ISP_WDR_TONECURVE_1;                /* Address Offset: 0x0004 */
    __IO uint32_t ISP_WDR_TONECURVE_2;                /* Address Offset: 0x0008 */
    __IO uint32_t ISP_WDR_TONECURVE_3;                /* Address Offset: 0x000C */
    __IO uint32_t ISP_WDR_TONECURVE_4;                /* Address Offset: 0x0010 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_0;             /* Address Offset: 0x0014 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_1;             /* Address Offset: 0x0018 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_2;             /* Address Offset: 0x001C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_3;             /* Address Offset: 0x0020 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_4;             /* Address Offset: 0x0024 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_5;             /* Address Offset: 0x0028 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_6;             /* Address Offset: 0x002C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_7;             /* Address Offset: 0x0030 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_8;             /* Address Offset: 0x0034 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_9;             /* Address Offset: 0x0038 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_10;            /* Address Offset: 0x003C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_11;            /* Address Offset: 0x0040 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_12;            /* Address Offset: 0x0044 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_13;            /* Address Offset: 0x0048 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_14;            /* Address Offset: 0x004C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_15;            /* Address Offset: 0x0050 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_16;            /* Address Offset: 0x0054 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_17;            /* Address Offset: 0x0058 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_18;            /* Address Offset: 0x005C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_19;            /* Address Offset: 0x0060 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_20;            /* Address Offset: 0x0064 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_21;            /* Address Offset: 0x0068 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_22;            /* Address Offset: 0x006C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_23;            /* Address Offset: 0x0070 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_24;            /* Address Offset: 0x0074 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_25;            /* Address Offset: 0x0078 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_26;            /* Address Offset: 0x007C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_27;            /* Address Offset: 0x0080 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_28;            /* Address Offset: 0x0084 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_29;            /* Address Offset: 0x0088 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_30;            /* Address Offset: 0x008C */
    __IO uint32_t ISP_WDR_TONECURVE_YM_31;            /* Address Offset: 0x0090 */
    __IO uint32_t ISP_WDR_TONECURVE_YM_32;            /* Address Offset: 0x0094 */
    __IO uint32_t ISP_WDR_OFFSET;                     /* Address Offset: 0x0098 */
    __IO uint32_t ISP_WDR_DELTAMIN;                   /* Address Offset: 0x009C */
    __I  uint32_t ISP_WDR_TONECURVE_1_SHD;            /* Address Offset: 0x00A0 */
    __I  uint32_t ISP_WDR_TONECURVE_2_SHD;            /* Address Offset: 0x00A4 */
    __I  uint32_t ISP_WDR_TONECURVE_3_SHD;            /* Address Offset: 0x00A8 */
    __I  uint32_t ISP_WDR_TONECURVE_4_SHD;            /* Address Offset: 0x00AC */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_0;         /* Address Offset: 0x00B0 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_1;         /* Address Offset: 0x00B4 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_2;         /* Address Offset: 0x00B8 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_3;         /* Address Offset: 0x00BC */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_4;         /* Address Offset: 0x00C0 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_5;         /* Address Offset: 0x00C4 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_6;         /* Address Offset: 0x00C8 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_7;         /* Address Offset: 0x00CC */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_8;         /* Address Offset: 0x00D0 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_9;         /* Address Offset: 0x00D4 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_10;        /* Address Offset: 0x00D8 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_11;        /* Address Offset: 0x00DC */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_12;        /* Address Offset: 0x00E0 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_13;        /* Address Offset: 0x00E4 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_14;        /* Address Offset: 0x00E8 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_15;        /* Address Offset: 0x00EC */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_16;        /* Address Offset: 0x00F0 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_17;        /* Address Offset: 0x00F4 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_18;        /* Address Offset: 0x00F8 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_19;        /* Address Offset: 0x00FC */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_20;        /* Address Offset: 0x0100 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_21;        /* Address Offset: 0x0104 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_22;        /* Address Offset: 0x0108 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_23;        /* Address Offset: 0x010C */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_24;        /* Address Offset: 0x0110 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_25;        /* Address Offset: 0x0114 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_26;        /* Address Offset: 0x0118 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_27;        /* Address Offset: 0x011C */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_28;        /* Address Offset: 0x0120 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_29;        /* Address Offset: 0x0124 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_30;        /* Address Offset: 0x0128 */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_31;        /* Address Offset: 0x012C */
    __I  uint32_t ISP_WDR_TONECURVE_YM_SHD_32;        /* Address Offset: 0x0130 */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
};
/* BUFFER_MANAGER Register Structure Define */
struct BUFFER_MANAGER_REG {
    __IO uint32_t WCH_MBLK_CON0;                      /* Address Offset: 0x0000 */
    __IO uint32_t WCH_MBLK_CON1;                      /* Address Offset: 0x0004 */
    __IO uint32_t WCH_MBLK_CON2;                      /* Address Offset: 0x0008 */
    __IO uint32_t WCH_MBLK_CON3;                      /* Address Offset: 0x000C */
    __IO uint32_t WCH_MBLK_CON4;                      /* Address Offset: 0x0010 */
    __IO uint32_t WCH_MBLK_CON5;                      /* Address Offset: 0x0014 */
    __IO uint32_t WCH_MBLK_CON6;                      /* Address Offset: 0x0018 */
         uint32_t RESERVED001C;                       /* Address Offset: 0x001C */
    __IO uint32_t WCH_MBLK_STATUS0;                   /* Address Offset: 0x0020 */
    __IO uint32_t WCH_MBLK_STATUS1;                   /* Address Offset: 0x0024 */
    __IO uint32_t WCH_MBLK_STATUS2;                   /* Address Offset: 0x0028 */
    __IO uint32_t WCH_MBLK_STATUS3;                   /* Address Offset: 0x002C */
    __IO uint32_t WCH_MBLK_INT_EN;                    /* Address Offset: 0x0030 */
    __IO uint32_t WCH_MBLK_INTSTS;                    /* Address Offset: 0x0034 */
         uint32_t RESERVED0038[114];                  /* Address Offset: 0x0038 */
    __IO uint32_t RCH_MBLK_CON0;                      /* Address Offset: 0x0200 */
    __IO uint32_t RCH_MBLK_CON1;                      /* Address Offset: 0x0204 */
    __IO uint32_t RCH_MBLK_CON2;                      /* Address Offset: 0x0208 */
         uint32_t RESERVED020C[3];                    /* Address Offset: 0x020C */
    __IO uint32_t RCH_MBLK_CON6;                      /* Address Offset: 0x0218 */
         uint32_t RESERVED021C;                       /* Address Offset: 0x021C */
    __IO uint32_t RCH_MBLK_STATUS0;                   /* Address Offset: 0x0220 */
         uint32_t RESERVED0224;                       /* Address Offset: 0x0224 */
    __IO uint32_t RCH_MBLK_STATUS2;                   /* Address Offset: 0x0228 */
    __IO uint32_t RCH_MBLK_STATUS3;                   /* Address Offset: 0x022C */
    __IO uint32_t RCH_MBLK_INT_EN;                    /* Address Offset: 0x0230 */
    __IO uint32_t RCH_MBLK_INTSTS;                    /* Address Offset: 0x0234 */
};
/* WDT Register Structure Define */
struct WDT_REG {
    __IO uint32_t CR;                                 /* Address Offset: 0x0000 */
    __IO uint32_t TORR;                               /* Address Offset: 0x0004 */
    __I  uint32_t CCVR;                               /* Address Offset: 0x0008 */
    __O  uint32_t CRR;                                /* Address Offset: 0x000C */
    __I  uint32_t STAT;                               /* Address Offset: 0x0010 */
    __I  uint32_t EOI;                                /* Address Offset: 0x0014 */
};
/* INTC Register Structure Define */
struct INTC_REG {
    __IO uint32_t IRQ_INTEN_L;                        /* Address Offset: 0x0000 */
    __IO uint32_t IRQ_INTEN_H;                        /* Address Offset: 0x0004 */
    __IO uint32_t IRQ_INTMASK_L;                      /* Address Offset: 0x0008 */
    __IO uint32_t IRQ_INTMASK_H;                      /* Address Offset: 0x000C */
    __IO uint32_t IRQ_INTFORCE_L;                     /* Address Offset: 0x0010 */
    __IO uint32_t IRQ_INTFORCE_H;                     /* Address Offset: 0x0014 */
    __I  uint32_t IRQ_RAWSTATUS_L;                    /* Address Offset: 0x0018 */
    __I  uint32_t IRQ_RAWSTATUS_H;                    /* Address Offset: 0x001C */
    __I  uint32_t IRQ_STATUS_L;                       /* Address Offset: 0x0020 */
    __I  uint32_t IRQ_STATUS_H;                       /* Address Offset: 0x0024 */
    __I  uint32_t IRQ_MASKSTATUS_L;                   /* Address Offset: 0x0028 */
    __I  uint32_t IRQ_MASKSTATUS_H;                   /* Address Offset: 0x002C */
    __I  uint32_t IRQ_FINALSTATUS_L;                  /* Address Offset: 0x0030 */
    __I  uint32_t IRQ_FINALSTATUS_H;                  /* Address Offset: 0x0034 */
         uint32_t RESERVED0038[34];                   /* Address Offset: 0x0038 */
    __IO uint32_t FIQ_INTEN;                          /* Address Offset: 0x00C0 */
    __IO uint32_t FIQ_INTMASK;                        /* Address Offset: 0x00C4 */
    __IO uint32_t FIQ_INTFORCE;                       /* Address Offset: 0x00C8 */
    __I  uint32_t FIQ_RAWSTATUS;                      /* Address Offset: 0x00CC */
    __I  uint32_t FIQ_STATUS;                         /* Address Offset: 0x00D0 */
    __I  uint32_t FIQ_FINALSTATUS;                    /* Address Offset: 0x00D4 */
    __IO uint32_t IRQ_PLEVEL;                         /* Address Offset: 0x00D8 */
         uint32_t RESERVED00DC[3];                    /* Address Offset: 0x00DC */
    __IO uint32_t IRQ_PR_OFFSET[196];                 /* Address Offset: 0x00E8 */
    __I  uint32_t AHB_ICTL_COMP_VERSION;              /* Address Offset: 0x03F8 */
    __I  uint32_t ICTL_COMP_TYPE;                     /* Address Offset: 0x03FC */
};
/* I2C Register Structure Define */
struct I2C_REG {
    __IO uint32_t CON;                                /* Address Offset: 0x0000 */
    __IO uint32_t CLKDIV;                             /* Address Offset: 0x0004 */
    __IO uint32_t MRXADDR;                            /* Address Offset: 0x0008 */
    __IO uint32_t MRXRADDR;                           /* Address Offset: 0x000C */
    __IO uint32_t MTXCNT;                             /* Address Offset: 0x0010 */
    __IO uint32_t MRXCNT;                             /* Address Offset: 0x0014 */
    __IO uint32_t IEN;                                /* Address Offset: 0x0018 */
    __IO uint32_t IPD;                                /* Address Offset: 0x001C */
    __I  uint32_t FCNT;                               /* Address Offset: 0x0020 */
    __IO uint32_t SCL_OE_DB;                          /* Address Offset: 0x0024 */
         uint32_t RESERVED0028[54];                   /* Address Offset: 0x0028 */
    __IO uint32_t TXDATA[8];                          /* Address Offset: 0x0100 */
         uint32_t RESERVED0120[56];                   /* Address Offset: 0x0120 */
    __I  uint32_t RXDATA[8];                          /* Address Offset: 0x0200 */
    __I  uint32_t ST;                                 /* Address Offset: 0x0220 */
    __IO uint32_t DBGCTRL;                            /* Address Offset: 0x0224 */
    __IO uint32_t CON1;                               /* Address Offset: 0x0228 */
};
/* UART Register Structure Define */
struct UART_REG {
    union {
        __I  uint32_t RBR;                                /* Address Offset: 0x0000 */
        __IO uint32_t DLL;                                /* Address Offset: 0x0000 */
        __O  uint32_t THR;                                /* Address Offset: 0x0000 */
    };
    union {
        __IO uint32_t DLH;                                /* Address Offset: 0x0004 */
        __IO uint32_t IER;                                /* Address Offset: 0x0004 */
    };
    union {
        __O  uint32_t FCR;                                /* Address Offset: 0x0008 */
        __I  uint32_t IIR;                                /* Address Offset: 0x0008 */
    };
    __IO uint32_t LCR;                                /* Address Offset: 0x000C */
    __IO uint32_t MCR;                                /* Address Offset: 0x0010 */
    __I  uint32_t LSR;                                /* Address Offset: 0x0014 */
    __I  uint32_t MSR;                                /* Address Offset: 0x0018 */
    __IO uint32_t SCR;                                /* Address Offset: 0x001C */
         uint32_t RESERVED0020[4];                    /* Address Offset: 0x0020 */
    union {
        __I  uint32_t SRBR;                               /* Address Offset: 0x0030 */
        __O  uint32_t STHR;                               /* Address Offset: 0x0030 */
    };
         uint32_t RESERVED0034[15];                   /* Address Offset: 0x0034 */
    __IO uint32_t FAR;                                /* Address Offset: 0x0070 */
    __I  uint32_t TFR;                                /* Address Offset: 0x0074 */
    __O  uint32_t RFW;                                /* Address Offset: 0x0078 */
    __I  uint32_t USR;                                /* Address Offset: 0x007C */
    __I  uint32_t TFL;                                /* Address Offset: 0x0080 */
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
    __IO uint32_t CTR;                                /* Address Offset: 0x00FC */
};
/* PWM Register Structure Define */
struct PWM_CHANNEL {
    __I  uint32_t CNT;
    __IO uint32_t PERIOD_HPR;
    __IO uint32_t DUTY_LPR;
    __IO uint32_t CTRL;
};
struct PWM_REG {
    struct PWM_CHANNEL CHANNELS[4];                   /* Address Offset: 0x0000 */
    __IO uint32_t INTSTS;                             /* Address Offset: 0x0040 */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x0044 */
         uint32_t RESERVED0048[2];                    /* Address Offset: 0x0048 */
    __IO uint32_t FIFO_CTRL;                          /* Address Offset: 0x0050 */
    __IO uint32_t FIFO_INTSTS;                        /* Address Offset: 0x0054 */
    __IO uint32_t FIFO_TOUTTHR;                       /* Address Offset: 0x0058 */
    __IO uint32_t VERSION_ID;                         /* Address Offset: 0x005C */
    __I  uint32_t FIFO;                               /* Address Offset: 0x0060 */
         uint32_t RESERVED0064[7];                    /* Address Offset: 0x0064 */
    __IO uint32_t PWRMATCH_CTRL;                      /* Address Offset: 0x0080 */
    __IO uint32_t PWRMATCH_LPRE;                      /* Address Offset: 0x0084 */
    __IO uint32_t PWRMATCH_HPRE;                      /* Address Offset: 0x0088 */
    __IO uint32_t PWRMATCH_LD;                        /* Address Offset: 0x008C */
    __IO uint32_t PWRMATCH_HD_ZERO;                   /* Address Offset: 0x0090 */
    __IO uint32_t PWRMATCH_HD_ONE;                    /* Address Offset: 0x0094 */
    __IO uint32_t PWRMATCH_VALUE[10];                 /* Address Offset: 0x0098 */
         uint32_t RESERVED00C0[3];                    /* Address Offset: 0x00C0 */
    __I  uint32_t PWM3_PWRCAPTURE_VALUE;              /* Address Offset: 0x00CC */
    __IO uint32_t FILTER_CTRL;                        /* Address Offset: 0x00D0 */
};
/* SARADC Register Structure Define */
struct SARADC_REG {
    __I  uint32_t DATA;                               /* Address Offset: 0x0000 */
    __I  uint32_t STAS;                               /* Address Offset: 0x0004 */
    __IO uint32_t CTRL;                               /* Address Offset: 0x0008 */
    __IO uint32_t DLY_PU_SOC;                         /* Address Offset: 0x000C */
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
    __IO uint32_t ISR;                                /* Address Offset: 0x0030 */
    __IO uint32_t RISR;                               /* Address Offset: 0x0034 */
    __IO uint32_t ICR;                                /* Address Offset: 0x0038 */
    __IO uint32_t DMACR;                              /* Address Offset: 0x003C */
    __IO uint32_t DMATDLR;                            /* Address Offset: 0x0040 */
    __IO uint32_t DMARDLR;                            /* Address Offset: 0x0044 */
         uint32_t RESERVED0048;                       /* Address Offset: 0x0048 */
    __IO uint32_t TIMEOUT;                            /* Address Offset: 0x004C */
    __IO uint32_t BYPASS;                             /* Address Offset: 0x0050 */
         uint32_t RESERVED0054[235];                  /* Address Offset: 0x0054 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0400 */
         uint32_t RESERVED0404[255];                  /* Address Offset: 0x0404 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x0800 */
};
/* CSI2HOST Register Structure Define */
struct CSI2HOST_REG {
    __I  uint32_t VERSION;                            /* Address Offset: 0x0000 */
    __IO uint32_t N_LANES;                            /* Address Offset: 0x0004 */
         uint32_t RESERVED0008[2];                    /* Address Offset: 0x0008 */
    __IO uint32_t CSI2_RESETN;                        /* Address Offset: 0x0010 */
    __IO uint32_t PHY_STATE;                          /* Address Offset: 0x0014 */
    __IO uint32_t DATA_IDS_1;                         /* Address Offset: 0x0018 */
    __IO uint32_t DATA_IDS_2;                         /* Address Offset: 0x001C */
    __IO uint32_t ERR1;                               /* Address Offset: 0x0020 */
    __IO uint32_t ERR2;                               /* Address Offset: 0x0024 */
    __IO uint32_t MSK1;                               /* Address Offset: 0x0028 */
    __IO uint32_t MSK2;                               /* Address Offset: 0x002C */
         uint32_t RESERVED0030[4];                    /* Address Offset: 0x0030 */
    __IO uint32_t CONTROL;                            /* Address Offset: 0x0040 */
};
#endif /*  __ASSEMBLY__  */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define CRU_BASE                       0x40200000U /* CRU base address */
#define GRF_BASE                       0x40240000U /* GRF base address */
#define USB_GRF_BASE                   0x40250000U /* USB_GRF base address */
#define GPIO0_BASE                     0x40260000U /* GPIO0 base address */
#define GPIO1_BASE                     0x40270000U /* GPIO1 base address */
#define CACHE_BASE                     0x40100000U /* CACHE base address */
#define DMA_BASE                       0x40110000U /* DMA base address */
#define FSPI0_BASE                     0x40120000U /* FSPI0 base address */
#define CKCAL_BASE                     0x40130000U /* CKCAL base address */
#define JPEG_ENC0_BASE                 0x40140000U /* JPEG_ENC0 base address */
#define JPEG_ENC1_BASE                 0x40150000U /* JPEG_ENC1 base address */
#define VICAP_LITE_BASE                0x40160000U /* VICAP_LITE base address */
#define ISP_MAIN_CTRL_BASE             0x40170000U /* ISP_MAIN_CTRL base address */
#define ISP_IMG_EFF_BASE               0x40170200U /* ISP_IMG_EFF base address */
#define ISP_SUPER_IMP_BASE             0x40170300U /* ISP_SUPER_IMP base address */
#define ISP_BASE                       0x40170400U /* ISP base address */
#define ISP_CPROC_BASE                 0x40170800U /* ISP_CPROC base address */
#define ISP_MAIN_RESIZE_BASE           0x40170C00U /* ISP_MAIN_RESIZE base address */
#define ISP_MI_BASE                    0x40171400U /* ISP_MI base address */
#define ISP_MIPI_BASE                  0x40171C00U /* ISP_MIPI base address */
#define ISP_AFM_BASE                   0x40172000U /* ISP_AFM base address */
#define ISP_LSC_BASE                   0x40172200U /* ISP_LSC base address */
#define ISP_IS_BASE                    0x40172300U /* ISP_IS base address */
#define ISP_HIST_BASE                  0x40172400U /* ISP_HIST base address */
#define ISP_FILTER_BASE                0x40172500U /* ISP_FILTER base address */
#define ISP_CAC_BASE                   0x40172580U /* ISP_CAC base address */
#define ISP_EXPOSURE_BASE              0x40172600U /* ISP_EXPOSURE base address */
#define ISP_BLS_BASE                   0x40172700U /* ISP_BLS base address */
#define ISP_DPF_BASE                   0x40172800U /* ISP_DPF base address */
#define ISP_DPCC_BASE                  0x40172900U /* ISP_DPCC base address */
#define ISP_WDR_BASE                   0x40172A00U /* ISP_WDR base address */
#define TIMER0_BASE                    0x40000000U /* TIMER0 base address */
#define TIMER1_BASE                    0x40000020U /* TIMER1 base address */
#define TIMER2_BASE                    0x40000040U /* TIMER2 base address */
#define TIMER3_BASE                    0x40000060U /* TIMER3 base address */
#define TIMER4_BASE                    0x40000080U /* TIMER4 base address */
#define TIMER5_BASE                    0x400000A0U /* TIMER5 base address */
#define BUFFER_MANAGER_BASE            0x40010000U /* BUFFER_MANAGER base address */
#define WDT_BASE                       0x40020000U /* WDT base address */
#define INTC0_BASE                     0x40030000U /* INTC0 base address */
#define I2C0_BASE                      0x40040000U /* I2C0 base address */
#define I2C1_BASE                      0x40050000U /* I2C1 base address */
#define UART0_BASE                     0x40070000U /* UART0 base address */
#define PWM0_BASE                      0x40090000U /* PWM0 base address */
#define SARADC_BASE                    0x400B0000U /* SARADC base address */
#define SPI0_BASE                      0x400D0000U /* SPI0 base address */
#define SPI1_BASE                      0x400E0000U /* SPI1 base address */
#define CSI2HOST_BASE                  0x400F0000U /* CSI2HOST base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define CRU                 ((struct CRU_REG *) CRU_BASE)
#define GRF                 ((struct GRF_REG *) GRF_BASE)
#define USB_GRF             ((struct USB_GRF_REG *) USB_GRF_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define ICACHE              ((struct CACHE_REG *) CACHE_BASE)
#define DCACHE              ((struct CACHE_REG *) CACHE_BASE)
#define DMA                 ((struct DMA_REG *) DMA_BASE)
#define FSPI0               ((struct FSPI_REG *) FSPI0_BASE)
#define CKCAL               ((struct CKCAL_REG *) CKCAL_BASE)
#define JPEG_ENC0           ((struct JPEG_ENC_REG *) JPEG_ENC0_BASE)
#define JPEG_ENC1           ((struct JPEG_ENC_REG *) JPEG_ENC1_BASE)
#define VICAP_LITE          ((struct VICAP_LITE_REG *) VICAP_LITE_BASE)
#define ISP_MAIN_CTRL       ((struct ISP_MAIN_CTRL_REG *) ISP_MAIN_CTRL_BASE)
#define ISP_IMG_EFF         ((struct ISP_IMG_EFF_REG *) ISP_IMG_EFF_BASE)
#define ISP_SUPER_IMP       ((struct ISP_SUPER_IMP_REG *) ISP_SUPER_IMP_BASE)
#define ISP                 ((struct ISP_REG *) ISP_BASE)
#define ISP_CPROC           ((struct ISP_CPROC_REG *) ISP_CPROC_BASE)
#define ISP_MAIN_RESIZE     ((struct ISP_MAIN_RESIZE_REG *) ISP_MAIN_RESIZE_BASE)
#define ISP_MI              ((struct ISP_MI_REG *) ISP_MI_BASE)
#define ISP_MIPI            ((struct ISP_MIPI_REG *) ISP_MIPI_BASE)
#define ISP_AFM             ((struct ISP_AFM_REG *) ISP_AFM_BASE)
#define ISP_LSC             ((struct ISP_LSC_REG *) ISP_LSC_BASE)
#define ISP_IS              ((struct ISP_IS_REG *) ISP_IS_BASE)
#define ISP_HIST            ((struct ISP_HIST_REG *) ISP_HIST_BASE)
#define ISP_FILTER          ((struct ISP_FILTER_REG *) ISP_FILTER_BASE)
#define ISP_CAC             ((struct ISP_CAC_REG *) ISP_CAC_BASE)
#define ISP_EXPOSURE        ((struct ISP_EXPOSURE_REG *) ISP_EXPOSURE_BASE)
#define ISP_BLS             ((struct ISP_BLS_REG *) ISP_BLS_BASE)
#define ISP_DPF             ((struct ISP_DPF_REG *) ISP_DPF_BASE)
#define ISP_DPCC            ((struct ISP_DPCC_REG *) ISP_DPCC_BASE)
#define ISP_WDR             ((struct ISP_WDR_REG *) ISP_WDR_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER0_BASE)
#define TIMER1              ((struct TIMER_REG *) TIMER1_BASE)
#define TIMER2              ((struct TIMER_REG *) TIMER2_BASE)
#define TIMER3              ((struct TIMER_REG *) TIMER3_BASE)
#define TIMER4              ((struct TIMER_REG *) TIMER4_BASE)
#define TIMER5              ((struct TIMER_REG *) TIMER5_BASE)
#define BUFFER_MANAGER      ((struct BUFFER_MANAGER_REG *) BUFFER_MANAGER_BASE)
#define WDT                 ((struct WDT_REG *) WDT_BASE)
#define INTC0               ((struct INTC_REG *) INTC0_BASE)
#define I2C0                ((struct I2C_REG *) I2C0_BASE)
#define I2C1                ((struct I2C_REG *) I2C1_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define PWM0                ((struct PWM_REG *) PWM0_BASE)
#define SARADC              ((struct SARADC_REG *) SARADC_BASE)
#define SPI0                ((struct SPI_REG *) SPI0_BASE)
#define SPI1                ((struct SPI_REG *) SPI1_BASE)
#define CSI2HOST            ((struct CSI2HOST_REG *) CSI2HOST_BASE)

#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_GRF_INSTANCE(instance) ((instance) == GRF)
#define IS_USB_GRF_INSTANCE(instance) ((instance) == USB_GRF)
#define IS_CACHE_INSTANCE(instance) ((instance) == CACHE)
#define IS_DMA_INSTANCE(instance) ((instance) == DMA)
#define IS_CKCAL_INSTANCE(instance) ((instance) == CKCAL)
#define IS_VICAP_LITE_INSTANCE(instance) ((instance) == VICAP_LITE)
#define IS_ISP_MAIN_CTRL_INSTANCE(instance) ((instance) == ISP_MAIN_CTRL)
#define IS_ISP_IMG_EFF_INSTANCE(instance) ((instance) == ISP_IMG_EFF)
#define IS_ISP_SUPER_IMP_INSTANCE(instance) ((instance) == ISP_SUPER_IMP)
#define IS_ISP_INSTANCE(instance) ((instance) == ISP)
#define IS_ISP_CPROC_INSTANCE(instance) ((instance) == ISP_CPROC)
#define IS_ISP_MAIN_RESIZE_INSTANCE(instance) ((instance) == ISP_MAIN_RESIZE)
#define IS_ISP_MI_INSTANCE(instance) ((instance) == ISP_MI)
#define IS_ISP_MIPI_INSTANCE(instance) ((instance) == ISP_MIPI)
#define IS_ISP_AFM_INSTANCE(instance) ((instance) == ISP_AFM)
#define IS_ISP_LSC_INSTANCE(instance) ((instance) == ISP_LSC)
#define IS_ISP_IS_INSTANCE(instance) ((instance) == ISP_IS)
#define IS_ISP_HIST_INSTANCE(instance) ((instance) == ISP_HIST)
#define IS_ISP_FILTER_INSTANCE(instance) ((instance) == ISP_FILTER)
#define IS_ISP_CAC_INSTANCE(instance) ((instance) == ISP_CAC)
#define IS_ISP_EXPOSURE_INSTANCE(instance) ((instance) == ISP_EXPOSURE)
#define IS_ISP_BLS_INSTANCE(instance) ((instance) == ISP_BLS)
#define IS_ISP_DPF_INSTANCE(instance) ((instance) == ISP_DPF)
#define IS_ISP_DPCC_INSTANCE(instance) ((instance) == ISP_DPCC)
#define IS_ISP_WDR_INSTANCE(instance) ((instance) == ISP_WDR)
#define IS_BUFFER_MANAGER_INSTANCE(instance) ((instance) == BUFFER_MANAGER)
#define IS_WDT_INSTANCE(instance) ((instance) == WDT)
#define IS_INTC_INSTANCE(instance) ((instance) == INTC0)
#define IS_SARADC_INSTANCE(instance) ((instance) == SARADC)
#define IS_CSI2HOST_INSTANCE(instance) ((instance) == CSI2HOST)
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO0) || ((instance) == GPIO1))
#define IS_FSPI_INSTANCE(instance) ((instance) == FSPI0)
#define IS_JPEG_ENC_INSTANCE(instance) (((instance) == JPEG_ENC0) || ((instance) == JPEG_ENC1))
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || ((instance) == TIMER2) || ((instance) == TIMER3) || ((instance) == TIMER4) || ((instance) == TIMER5))
#define IS_I2C_INSTANCE(instance) (((instance) == I2C0) || ((instance) == I2C1))
#define IS_UART_INSTANCE(instance) ((instance) == UART0)
#define IS_PWM_INSTANCE(instance) ((instance) == PWM0)
#define IS_SPI_INSTANCE(instance) (((instance) == SPI0) || ((instance) == SPI1))
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************CRU*******************************************/
/* GPLL_CON00 */
#define CRU_GPLL_CON00_OFFSET                              (0x0U)
#define CRU_GPLL_CON00_FBDIV_SHIFT                         (0U)
#define CRU_GPLL_CON00_FBDIV_MASK                          (0xFFFU << CRU_GPLL_CON00_FBDIV_SHIFT)                       /* 0x00000FFF */
#define CRU_GPLL_CON00_POSTDIV1_SHIFT                      (12U)
#define CRU_GPLL_CON00_POSTDIV1_MASK                       (0x7U << CRU_GPLL_CON00_POSTDIV1_SHIFT)                      /* 0x00007000 */
#define CRU_GPLL_CON00_BYPASS_SHIFT                        (15U)
#define CRU_GPLL_CON00_BYPASS_MASK                         (0x1U << CRU_GPLL_CON00_BYPASS_SHIFT)                        /* 0x00008000 */
/* GPLL_CON01 */
#define CRU_GPLL_CON01_OFFSET                              (0x4U)
#define CRU_GPLL_CON01_REFDIV_SHIFT                        (0U)
#define CRU_GPLL_CON01_REFDIV_MASK                         (0x3FU << CRU_GPLL_CON01_REFDIV_SHIFT)                       /* 0x0000003F */
#define CRU_GPLL_CON01_POSTDIV2_SHIFT                      (6U)
#define CRU_GPLL_CON01_POSTDIV2_MASK                       (0x7U << CRU_GPLL_CON01_POSTDIV2_SHIFT)                      /* 0x000001C0 */
#define CRU_GPLL_CON01_PLL_LOCK_SHIFT                      (10U)
#define CRU_GPLL_CON01_PLL_LOCK_MASK                       (0x1U << CRU_GPLL_CON01_PLL_LOCK_SHIFT)                      /* 0x00000400 */
#define CRU_GPLL_CON01_DSMPD_SHIFT                         (12U)
#define CRU_GPLL_CON01_DSMPD_MASK                          (0x1U << CRU_GPLL_CON01_DSMPD_SHIFT)                         /* 0x00001000 */
#define CRU_GPLL_CON01_PLLPD_SHIFT                         (13U)
#define CRU_GPLL_CON01_PLLPD_MASK                          (0x1U << CRU_GPLL_CON01_PLLPD_SHIFT)                         /* 0x00002000 */
/* GPLL_CON02 */
#define CRU_GPLL_CON02_OFFSET                              (0x8U)
#define CRU_GPLL_CON02_FRACDIV_SHIFT                       (0U)
#define CRU_GPLL_CON02_FRACDIV_MASK                        (0xFFFFFFU << CRU_GPLL_CON02_FRACDIV_SHIFT)                  /* 0x00FFFFFF */
#define CRU_GPLL_CON02_DACPD_SHIFT                         (24U)
#define CRU_GPLL_CON02_DACPD_MASK                          (0x1U << CRU_GPLL_CON02_DACPD_SHIFT)                         /* 0x01000000 */
#define CRU_GPLL_CON02_FOUTPOSTDIVPD_SHIFT                 (25U)
#define CRU_GPLL_CON02_FOUTPOSTDIVPD_MASK                  (0x1U << CRU_GPLL_CON02_FOUTPOSTDIVPD_SHIFT)                 /* 0x02000000 */
#define CRU_GPLL_CON02_FOUTVCOPD_SHIFT                     (26U)
#define CRU_GPLL_CON02_FOUTVCOPD_MASK                      (0x1U << CRU_GPLL_CON02_FOUTVCOPD_SHIFT)                     /* 0x04000000 */
#define CRU_GPLL_CON02_FOUT4PHASEPD_SHIFT                  (27U)
#define CRU_GPLL_CON02_FOUT4PHASEPD_MASK                   (0x1U << CRU_GPLL_CON02_FOUT4PHASEPD_SHIFT)                  /* 0x08000000 */
/* GPLL_CON03 */
#define CRU_GPLL_CON03_OFFSET                              (0xCU)
#define CRU_GPLL_CON03_SSMOD_BP_SHIFT                      (0U)
#define CRU_GPLL_CON03_SSMOD_BP_MASK                       (0x1U << CRU_GPLL_CON03_SSMOD_BP_SHIFT)                      /* 0x00000001 */
#define CRU_GPLL_CON03_SSMOD_DISABLE_SSCG_SHIFT            (1U)
#define CRU_GPLL_CON03_SSMOD_DISABLE_SSCG_MASK             (0x1U << CRU_GPLL_CON03_SSMOD_DISABLE_SSCG_SHIFT)            /* 0x00000002 */
#define CRU_GPLL_CON03_SSMOD_RESET_SHIFT                   (2U)
#define CRU_GPLL_CON03_SSMOD_RESET_MASK                    (0x1U << CRU_GPLL_CON03_SSMOD_RESET_SHIFT)                   /* 0x00000004 */
#define CRU_GPLL_CON03_SSMOD_DOWNSPREAD_SHIFT              (3U)
#define CRU_GPLL_CON03_SSMOD_DOWNSPREAD_MASK               (0x1U << CRU_GPLL_CON03_SSMOD_DOWNSPREAD_SHIFT)              /* 0x00000008 */
#define CRU_GPLL_CON03_SSMOD_DIVVAL_SHIFT                  (4U)
#define CRU_GPLL_CON03_SSMOD_DIVVAL_MASK                   (0xFU << CRU_GPLL_CON03_SSMOD_DIVVAL_SHIFT)                  /* 0x000000F0 */
#define CRU_GPLL_CON03_SSMOD_SPREAD_SHIFT                  (8U)
#define CRU_GPLL_CON03_SSMOD_SPREAD_MASK                   (0x1FU << CRU_GPLL_CON03_SSMOD_SPREAD_SHIFT)                 /* 0x00001F00 */
/* GPLL_CON04 */
#define CRU_GPLL_CON04_OFFSET                              (0x10U)
#define CRU_GPLL_CON04_SSMOD_SEL_EXT_WAVE_SHIFT            (0U)
#define CRU_GPLL_CON04_SSMOD_SEL_EXT_WAVE_MASK             (0x1U << CRU_GPLL_CON04_SSMOD_SEL_EXT_WAVE_SHIFT)            /* 0x00000001 */
#define CRU_GPLL_CON04_SSMOD_EXT_MAXADDR_SHIFT             (8U)
#define CRU_GPLL_CON04_SSMOD_EXT_MAXADDR_MASK              (0xFFU << CRU_GPLL_CON04_SSMOD_EXT_MAXADDR_SHIFT)            /* 0x0000FF00 */
/* MODE_CON00 */
#define CRU_MODE_CON00_OFFSET                              (0xA0U)
#define CRU_MODE_CON00_CLK_GPLL_MODE_SHIFT                 (0U)
#define CRU_MODE_CON00_CLK_GPLL_MODE_MASK                  (0x3U << CRU_MODE_CON00_CLK_GPLL_MODE_SHIFT)                 /* 0x00000003 */
#define CRU_MODE_CON00_CLK_USBPLL_MODE_SHIFT               (2U)
#define CRU_MODE_CON00_CLK_USBPLL_MODE_MASK                (0x3U << CRU_MODE_CON00_CLK_USBPLL_MODE_SHIFT)               /* 0x0000000C */
/* GLB_CNT_TH */
#define CRU_GLB_CNT_TH_OFFSET                              (0xB0U)
#define CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT (0U)
#define CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_MASK (0xFFFFFFFFU << CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT) /* 0xFFFFFFFF */
/* GLB_RST_ST */
#define CRU_GLB_RST_ST_OFFSET                              (0xB4U)
#define CRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT                (0U)
#define CRU_GLB_RST_ST_FST_GLB_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT)                /* 0x00000001 */
#define CRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT                (1U)
#define CRU_GLB_RST_ST_SND_GLB_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT)                /* 0x00000002 */
#define CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT            (4U)
#define CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_MASK             (0x1U << CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT)            /* 0x00000010 */
#define CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT            (5U)
#define CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_MASK             (0x1U << CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT)            /* 0x00000020 */
#define CRU_GLB_RST_ST_GLB_WDT_RST_ST_SHIFT                (6U)
#define CRU_GLB_RST_ST_GLB_WDT_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_GLB_WDT_RST_ST_SHIFT)                /* 0x00000040 */
/* GLB_SRST_FST_VALUE */
#define CRU_GLB_SRST_FST_VALUE_OFFSET                      (0xB8U)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_SHIFT          (0U)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_MASK           (0xFFFFU << CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_SHIFT)       /* 0x0000FFFF */
/* GLB_SRST_SND_VALUE */
#define CRU_GLB_SRST_SND_VALUE_OFFSET                      (0xBCU)
#define CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_SHIFT          (0U)
#define CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_MASK           (0xFFFFU << CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_SHIFT)       /* 0x0000FFFF */
/* GLB_RST_CON */
#define CRU_GLB_RST_CON_OFFSET                             (0xC0U)
#define CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT             (7U)
#define CRU_GLB_RST_CON_WDT_RESET_EXT_EN_MASK              (0x1U << CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT)             /* 0x00000080 */
#define CRU_GLB_RST_CON_WDT_GLB_SRST_EN_SHIFT              (8U)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_EN_MASK               (0x1U << CRU_GLB_RST_CON_WDT_GLB_SRST_EN_SHIFT)              /* 0x00000100 */
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT            (12U)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK             (0x1U << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT)            /* 0x00001000 */
/* CLKSEL_CON00 */
#define CRU_CLKSEL_CON00_OFFSET                            (0x100U)
#define CRU_CLKSEL_CON00_CLK_32K_SEL_SHIFT                 (6U)
#define CRU_CLKSEL_CON00_CLK_32K_SEL_MASK                  (0x1U << CRU_CLKSEL_CON00_CLK_32K_SEL_SHIFT)                 /* 0x00000040 */
#define CRU_CLKSEL_CON00_CLK_GPLL_MUX_NP5_DIV_SHIFT        (8U)
#define CRU_CLKSEL_CON00_CLK_GPLL_MUX_NP5_DIV_MASK         (0xFU << CRU_CLKSEL_CON00_CLK_GPLL_MUX_NP5_DIV_SHIFT)        /* 0x00000F00 */
/* CLKSEL_CON01 */
#define CRU_CLKSEL_CON01_OFFSET                            (0x104U)
#define CRU_CLKSEL_CON01_CLK_32K_FRAC_DIV_DIV_SHIFT        (0U)
#define CRU_CLKSEL_CON01_CLK_32K_FRAC_DIV_DIV_MASK         (0xFFFFFFFFU << CRU_CLKSEL_CON01_CLK_32K_FRAC_DIV_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON02 */
#define CRU_CLKSEL_CON02_OFFSET                            (0x108U)
#define CRU_CLKSEL_CON02_HCLK_LOGIC_BUS_DIV_SHIFT          (0U)
#define CRU_CLKSEL_CON02_HCLK_LOGIC_BUS_DIV_MASK           (0x1FU << CRU_CLKSEL_CON02_HCLK_LOGIC_BUS_DIV_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL_CON02_HCLK_LOGIC_BUS_SEL_SHIFT          (7U)
#define CRU_CLKSEL_CON02_HCLK_LOGIC_BUS_SEL_MASK           (0x1U << CRU_CLKSEL_CON02_HCLK_LOGIC_BUS_SEL_SHIFT)          /* 0x00000080 */
#define CRU_CLKSEL_CON02_ACLK_JPEG_BUS_DIV_SHIFT           (8U)
#define CRU_CLKSEL_CON02_ACLK_JPEG_BUS_DIV_MASK            (0x1FU << CRU_CLKSEL_CON02_ACLK_JPEG_BUS_DIV_SHIFT)          /* 0x00001F00 */
#define CRU_CLKSEL_CON02_ACLK_JPEG_BUS_SEL_SHIFT           (15U)
#define CRU_CLKSEL_CON02_ACLK_JPEG_BUS_SEL_MASK            (0x1U << CRU_CLKSEL_CON02_ACLK_JPEG_BUS_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL_CON03 */
#define CRU_CLKSEL_CON03_OFFSET                            (0x10CU)
#define CRU_CLKSEL_CON03_PCLK_LOGIC_BUS_DIV_SHIFT          (0U)
#define CRU_CLKSEL_CON03_PCLK_LOGIC_BUS_DIV_MASK           (0x1FU << CRU_CLKSEL_CON03_PCLK_LOGIC_BUS_DIV_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL_CON03_CLK_SARADC_DIV_SHIFT              (8U)
#define CRU_CLKSEL_CON03_CLK_SARADC_DIV_MASK               (0xFFU << CRU_CLKSEL_CON03_CLK_SARADC_DIV_SHIFT)             /* 0x0000FF00 */
/* CLKSEL_CON04 */
#define CRU_CLKSEL_CON04_OFFSET                            (0x110U)
#define CRU_CLKSEL_CON04_CLK_I2C0_DIV_SHIFT                (0U)
#define CRU_CLKSEL_CON04_CLK_I2C0_DIV_MASK                 (0x3FU << CRU_CLKSEL_CON04_CLK_I2C0_DIV_SHIFT)               /* 0x0000003F */
#define CRU_CLKSEL_CON04_CLK_I2C0_SEL_SHIFT                (6U)
#define CRU_CLKSEL_CON04_CLK_I2C0_SEL_MASK                 (0x3U << CRU_CLKSEL_CON04_CLK_I2C0_SEL_SHIFT)                /* 0x000000C0 */
#define CRU_CLKSEL_CON04_CLK_I2C1_DIV_SHIFT                (8U)
#define CRU_CLKSEL_CON04_CLK_I2C1_DIV_MASK                 (0x3FU << CRU_CLKSEL_CON04_CLK_I2C1_DIV_SHIFT)               /* 0x00003F00 */
#define CRU_CLKSEL_CON04_CLK_I2C1_SEL_SHIFT                (14U)
#define CRU_CLKSEL_CON04_CLK_I2C1_SEL_MASK                 (0x3U << CRU_CLKSEL_CON04_CLK_I2C1_SEL_SHIFT)                /* 0x0000C000 */
/* CLKSEL_CON05 */
#define CRU_CLKSEL_CON05_OFFSET                            (0x114U)
#define CRU_CLKSEL_CON05_CLK_SPI0_DIV_SHIFT                (0U)
#define CRU_CLKSEL_CON05_CLK_SPI0_DIV_MASK                 (0x3FU << CRU_CLKSEL_CON05_CLK_SPI0_DIV_SHIFT)               /* 0x0000003F */
#define CRU_CLKSEL_CON05_CLK_SPI0_SEL_SHIFT                (7U)
#define CRU_CLKSEL_CON05_CLK_SPI0_SEL_MASK                 (0x1U << CRU_CLKSEL_CON05_CLK_SPI0_SEL_SHIFT)                /* 0x00000080 */
#define CRU_CLKSEL_CON05_CLK_SPI1_DIV_SHIFT                (8U)
#define CRU_CLKSEL_CON05_CLK_SPI1_DIV_MASK                 (0x3FU << CRU_CLKSEL_CON05_CLK_SPI1_DIV_SHIFT)               /* 0x00003F00 */
#define CRU_CLKSEL_CON05_CLK_SPI1_SEL_SHIFT                (15U)
#define CRU_CLKSEL_CON05_CLK_SPI1_SEL_MASK                 (0x1U << CRU_CLKSEL_CON05_CLK_SPI1_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL_CON06 */
#define CRU_CLKSEL_CON06_OFFSET                            (0x118U)
#define CRU_CLKSEL_CON06_CLK_PWM_DIV_SHIFT                 (0U)
#define CRU_CLKSEL_CON06_CLK_PWM_DIV_MASK                  (0x3FU << CRU_CLKSEL_CON06_CLK_PWM_DIV_SHIFT)                /* 0x0000003F */
#define CRU_CLKSEL_CON06_CLK_PWM_SEL_SHIFT                 (7U)
#define CRU_CLKSEL_CON06_CLK_PWM_SEL_MASK                  (0x1U << CRU_CLKSEL_CON06_CLK_PWM_SEL_SHIFT)                 /* 0x00000080 */
#define CRU_CLKSEL_CON06_CLK_TIMER_DIV_SHIFT               (8U)
#define CRU_CLKSEL_CON06_CLK_TIMER_DIV_MASK                (0x3FU << CRU_CLKSEL_CON06_CLK_TIMER_DIV_SHIFT)              /* 0x00003F00 */
#define CRU_CLKSEL_CON06_CLK_TIMER_SEL_SHIFT               (15U)
#define CRU_CLKSEL_CON06_CLK_TIMER_SEL_MASK                (0x1U << CRU_CLKSEL_CON06_CLK_TIMER_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL_CON07 */
#define CRU_CLKSEL_CON07_OFFSET                            (0x11CU)
#define CRU_CLKSEL_CON07_CLK_XIP_SFC_DIV_SHIFT             (0U)
#define CRU_CLKSEL_CON07_CLK_XIP_SFC_DIV_MASK              (0x3FU << CRU_CLKSEL_CON07_CLK_XIP_SFC_DIV_SHIFT)            /* 0x0000003F */
#define CRU_CLKSEL_CON07_CLK_XIP_SFC_SEL_SHIFT             (6U)
#define CRU_CLKSEL_CON07_CLK_XIP_SFC_SEL_MASK              (0x3U << CRU_CLKSEL_CON07_CLK_XIP_SFC_SEL_SHIFT)             /* 0x000000C0 */
#define CRU_CLKSEL_CON07_CLK_ISP_DIV_SHIFT                 (8U)
#define CRU_CLKSEL_CON07_CLK_ISP_DIV_MASK                  (0x3FU << CRU_CLKSEL_CON07_CLK_ISP_DIV_SHIFT)                /* 0x00003F00 */
/* CLKSEL_CON08 */
#define CRU_CLKSEL_CON08_OFFSET                            (0x120U)
#define CRU_CLKSEL_CON08_DCLK_VIPCAP_DIV_SHIFT             (0U)
#define CRU_CLKSEL_CON08_DCLK_VIPCAP_DIV_MASK              (0x3FU << CRU_CLKSEL_CON08_DCLK_VIPCAP_DIV_SHIFT)            /* 0x0000003F */
#define CRU_CLKSEL_CON08_DCLK_VIPCAP_SEL_SHIFT             (7U)
#define CRU_CLKSEL_CON08_DCLK_VIPCAP_SEL_MASK              (0x1U << CRU_CLKSEL_CON08_DCLK_VIPCAP_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL_CON08_CLK_VIP_OUT_SEL_SHIFT             (14U)
#define CRU_CLKSEL_CON08_CLK_VIP_OUT_SEL_MASK              (0x3U << CRU_CLKSEL_CON08_CLK_VIP_OUT_SEL_SHIFT)             /* 0x0000C000 */
#define CRU_CLKSEL_CON08_CLK_VIP_OUT_DIV_SHIFT             (8U)
#define CRU_CLKSEL_CON08_CLK_VIP_OUT_DIV_MASK              (0x3FU << CRU_CLKSEL_CON08_CLK_VIP_OUT_DIV_SHIFT)            /* 0x00003F00 */
/* CLKSEL_CON09 */
#define CRU_CLKSEL_CON09_OFFSET                            (0x124U)
#define CRU_CLKSEL_CON09_OUTCLOCK_TEST_DIV_SHIFT           (0U)
#define CRU_CLKSEL_CON09_OUTCLOCK_TEST_DIV_MASK            (0xFU << CRU_CLKSEL_CON09_OUTCLOCK_TEST_DIV_SHIFT)           /* 0x0000000F */
#define CRU_CLKSEL_CON09_OUTCLOCK_TEST_SEL_SHIFT           (4U)
#define CRU_CLKSEL_CON09_OUTCLOCK_TEST_SEL_MASK            (0xFU << CRU_CLKSEL_CON09_OUTCLOCK_TEST_SEL_SHIFT)           /* 0x000000F0 */
/* CLKSEL_CON10 */
#define CRU_CLKSEL_CON10_OFFSET                            (0x128U)
#define CRU_CLKSEL_CON10_CLK_UART_DIV_DIV_SHIFT            (0U)
#define CRU_CLKSEL_CON10_CLK_UART_DIV_DIV_MASK             (0x1FU << CRU_CLKSEL_CON10_CLK_UART_DIV_DIV_SHIFT)           /* 0x0000001F */
#define CRU_CLKSEL_CON10_CLK_UART_DIV_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON10_CLK_UART_DIV_SEL_MASK             (0x1U << CRU_CLKSEL_CON10_CLK_UART_DIV_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON10_SCLK_UART_SEL_SHIFT               (15U)
#define CRU_CLKSEL_CON10_SCLK_UART_SEL_MASK                (0x1U << CRU_CLKSEL_CON10_SCLK_UART_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL_CON11 */
#define CRU_CLKSEL_CON11_OFFSET                            (0x12CU)
#define CRU_CLKSEL_CON11_CLK_UART_FRAC_DIV_DIV_SHIFT       (0U)
#define CRU_CLKSEL_CON11_CLK_UART_FRAC_DIV_DIV_MASK        (0xFFFFFFFFU << CRU_CLKSEL_CON11_CLK_UART_FRAC_DIV_DIV_SHIFT) /* 0xFFFFFFFF */
/* GATE_CON00 */
#define CRU_GATE_CON00_OFFSET                              (0x300U)
#define CRU_GATE_CON00_CLK_OSC_EN_SHIFT                    (0U)
#define CRU_GATE_CON00_CLK_OSC_EN_MASK                     (0x1U << CRU_GATE_CON00_CLK_OSC_EN_SHIFT)                    /* 0x00000001 */
#define CRU_GATE_CON00_CLK_32K_FRAC_DIV_EN_SHIFT           (1U)
#define CRU_GATE_CON00_CLK_32K_FRAC_DIV_EN_MASK            (0x1U << CRU_GATE_CON00_CLK_32K_FRAC_DIV_EN_SHIFT)           /* 0x00000002 */
#define CRU_GATE_CON00_CLK_32K_EN_SHIFT                    (2U)
#define CRU_GATE_CON00_CLK_32K_EN_MASK                     (0x1U << CRU_GATE_CON00_CLK_32K_EN_SHIFT)                    /* 0x00000004 */
#define CRU_GATE_CON00_CLK_GPLL_MUX_DIV_NP5_EN_SHIFT       (4U)
#define CRU_GATE_CON00_CLK_GPLL_MUX_DIV_NP5_EN_MASK        (0x1U << CRU_GATE_CON00_CLK_GPLL_MUX_DIV_NP5_EN_SHIFT)       /* 0x00000010 */
/* GATE_CON01 */
#define CRU_GATE_CON01_OFFSET                              (0x304U)
#define CRU_GATE_CON01_HCLK_LOGIC_BUS_PLL_EN_SHIFT         (0U)
#define CRU_GATE_CON01_HCLK_LOGIC_BUS_PLL_EN_MASK          (0x1U << CRU_GATE_CON01_HCLK_LOGIC_BUS_PLL_EN_SHIFT)         /* 0x00000001 */
#define CRU_GATE_CON01_HCLK_LOGIC_BUS_NIU_EN_SHIFT         (1U)
#define CRU_GATE_CON01_HCLK_LOGIC_BUS_NIU_EN_MASK          (0x1U << CRU_GATE_CON01_HCLK_LOGIC_BUS_NIU_EN_SHIFT)         /* 0x00000002 */
#define CRU_GATE_CON01_CLK_MCU_EN_SHIFT                    (2U)
#define CRU_GATE_CON01_CLK_MCU_EN_MASK                     (0x1U << CRU_GATE_CON01_CLK_MCU_EN_SHIFT)                    /* 0x00000004 */
#define CRU_GATE_CON01_HCLK_INTMEM0_LOGIC_EN_SHIFT         (3U)
#define CRU_GATE_CON01_HCLK_INTMEM0_LOGIC_EN_MASK          (0x1U << CRU_GATE_CON01_HCLK_INTMEM0_LOGIC_EN_SHIFT)         /* 0x00000008 */
#define CRU_GATE_CON01_HCLK_INTMEM1_LOGIC_EN_SHIFT         (4U)
#define CRU_GATE_CON01_HCLK_INTMEM1_LOGIC_EN_MASK          (0x1U << CRU_GATE_CON01_HCLK_INTMEM1_LOGIC_EN_SHIFT)         /* 0x00000010 */
#define CRU_GATE_CON01_HCLK_INTMEM2_LOGIC_EN_SHIFT         (5U)
#define CRU_GATE_CON01_HCLK_INTMEM2_LOGIC_EN_MASK          (0x1U << CRU_GATE_CON01_HCLK_INTMEM2_LOGIC_EN_SHIFT)         /* 0x00000020 */
#define CRU_GATE_CON01_HCLK_ROM_EN_SHIFT                   (6U)
#define CRU_GATE_CON01_HCLK_ROM_EN_MASK                    (0x1U << CRU_GATE_CON01_HCLK_ROM_EN_SHIFT)                   /* 0x00000040 */
#define CRU_GATE_CON01_HCLK_SFC_EN_SHIFT                   (7U)
#define CRU_GATE_CON01_HCLK_SFC_EN_MASK                    (0x1U << CRU_GATE_CON01_HCLK_SFC_EN_SHIFT)                   /* 0x00000080 */
#define CRU_GATE_CON01_HCLK_SFC_XIP_EN_SHIFT               (8U)
#define CRU_GATE_CON01_HCLK_SFC_XIP_EN_MASK                (0x1U << CRU_GATE_CON01_HCLK_SFC_XIP_EN_SHIFT)               /* 0x00000100 */
#define CRU_GATE_CON01_HCLK_USB_EN_SHIFT                   (9U)
#define CRU_GATE_CON01_HCLK_USB_EN_MASK                    (0x1U << CRU_GATE_CON01_HCLK_USB_EN_SHIFT)                   /* 0x00000200 */
#define CRU_GATE_CON01_HCLK_USB_PMU_EN_SHIFT               (10U)
#define CRU_GATE_CON01_HCLK_USB_PMU_EN_MASK                (0x1U << CRU_GATE_CON01_HCLK_USB_PMU_EN_SHIFT)               /* 0x00000400 */
#define CRU_GATE_CON01_HCLK_CACHE_EN_SHIFT                 (11U)
#define CRU_GATE_CON01_HCLK_CACHE_EN_MASK                  (0x1U << CRU_GATE_CON01_HCLK_CACHE_EN_SHIFT)                 /* 0x00000800 */
#define CRU_GATE_CON01_ACLK_VIPCAP_EN_SHIFT                (12U)
#define CRU_GATE_CON01_ACLK_VIPCAP_EN_MASK                 (0x1U << CRU_GATE_CON01_ACLK_VIPCAP_EN_SHIFT)                /* 0x00001000 */
#define CRU_GATE_CON01_HCLK_VIPCAP_EN_SHIFT                (13U)
#define CRU_GATE_CON01_HCLK_VIPCAP_EN_MASK                 (0x1U << CRU_GATE_CON01_HCLK_VIPCAP_EN_SHIFT)                /* 0x00002000 */
#define CRU_GATE_CON01_ACLK_ISP_EN_SHIFT                   (14U)
#define CRU_GATE_CON01_ACLK_ISP_EN_MASK                    (0x1U << CRU_GATE_CON01_ACLK_ISP_EN_SHIFT)                   /* 0x00004000 */
#define CRU_GATE_CON01_HCLK_ISP_EN_SHIFT                   (15U)
#define CRU_GATE_CON01_HCLK_ISP_EN_MASK                    (0x1U << CRU_GATE_CON01_HCLK_ISP_EN_SHIFT)                   /* 0x00008000 */
/* GATE_CON02 */
#define CRU_GATE_CON02_OFFSET                              (0x308U)
#define CRU_GATE_CON02_HCLK_JPEG_ENC0_EN_SHIFT             (0U)
#define CRU_GATE_CON02_HCLK_JPEG_ENC0_EN_MASK              (0x1U << CRU_GATE_CON02_HCLK_JPEG_ENC0_EN_SHIFT)             /* 0x00000001 */
#define CRU_GATE_CON02_HCLK_JPEG_ENC1_EN_SHIFT             (1U)
#define CRU_GATE_CON02_HCLK_JPEG_ENC1_EN_MASK              (0x1U << CRU_GATE_CON02_HCLK_JPEG_ENC1_EN_SHIFT)             /* 0x00000002 */
#define CRU_GATE_CON02_ACLK_LOGIC_BUS_NIU_EN_SHIFT         (2U)
#define CRU_GATE_CON02_ACLK_LOGIC_BUS_NIU_EN_MASK          (0x1U << CRU_GATE_CON02_ACLK_LOGIC_BUS_NIU_EN_SHIFT)         /* 0x00000004 */
#define CRU_GATE_CON02_HCLK_DMAC_EN_SHIFT                  (3U)
#define CRU_GATE_CON02_HCLK_DMAC_EN_MASK                   (0x1U << CRU_GATE_CON02_HCLK_DMAC_EN_SHIFT)                  /* 0x00000008 */
#define CRU_GATE_CON02_HCLK_JHASH_M_EN_SHIFT               (4U)
#define CRU_GATE_CON02_HCLK_JHASH_M_EN_MASK                (0x1U << CRU_GATE_CON02_HCLK_JHASH_M_EN_SHIFT)               /* 0x00000010 */
#define CRU_GATE_CON02_HCLK_JHASH_S_EN_SHIFT               (5U)
#define CRU_GATE_CON02_HCLK_JHASH_S_EN_MASK                (0x1U << CRU_GATE_CON02_HCLK_JHASH_S_EN_SHIFT)               /* 0x00000020 */
#define CRU_GATE_CON02_HCLK_HOST_ARB_EN_SHIFT              (6U)
#define CRU_GATE_CON02_HCLK_HOST_ARB_EN_MASK               (0x1U << CRU_GATE_CON02_HCLK_HOST_ARB_EN_SHIFT)              /* 0x00000040 */
#define CRU_GATE_CON02_ACLK_JPEG_BUS_PLL_EN_SHIFT          (8U)
#define CRU_GATE_CON02_ACLK_JPEG_BUS_PLL_EN_MASK           (0x1U << CRU_GATE_CON02_ACLK_JPEG_BUS_PLL_EN_SHIFT)          /* 0x00000100 */
#define CRU_GATE_CON02_ACLK_JPEG_BUS_NIU_EN_SHIFT          (9U)
#define CRU_GATE_CON02_ACLK_JPEG_BUS_NIU_EN_MASK           (0x1U << CRU_GATE_CON02_ACLK_JPEG_BUS_NIU_EN_SHIFT)          /* 0x00000200 */
#define CRU_GATE_CON02_ACLK_JPEG_ENC0_EN_SHIFT             (10U)
#define CRU_GATE_CON02_ACLK_JPEG_ENC0_EN_MASK              (0x1U << CRU_GATE_CON02_ACLK_JPEG_ENC0_EN_SHIFT)             /* 0x00000400 */
#define CRU_GATE_CON02_ACLK_JPEG_ENC1_EN_SHIFT             (11U)
#define CRU_GATE_CON02_ACLK_JPEG_ENC1_EN_MASK              (0x1U << CRU_GATE_CON02_ACLK_JPEG_ENC1_EN_SHIFT)             /* 0x00000800 */
/* GATE_CON03 */
#define CRU_GATE_CON03_OFFSET                              (0x30CU)
#define CRU_GATE_CON03_PCLK_LOGIC_BUS_PLL_EN_SHIFT         (0U)
#define CRU_GATE_CON03_PCLK_LOGIC_BUS_PLL_EN_MASK          (0x1U << CRU_GATE_CON03_PCLK_LOGIC_BUS_PLL_EN_SHIFT)         /* 0x00000001 */
#define CRU_GATE_CON03_PCLK_LOGIC_BUS_NIU_EN_SHIFT         (1U)
#define CRU_GATE_CON03_PCLK_LOGIC_BUS_NIU_EN_MASK          (0x1U << CRU_GATE_CON03_PCLK_LOGIC_BUS_NIU_EN_SHIFT)         /* 0x00000002 */
#define CRU_GATE_CON03_PCLK_UART_EN_SHIFT                  (2U)
#define CRU_GATE_CON03_PCLK_UART_EN_MASK                   (0x1U << CRU_GATE_CON03_PCLK_UART_EN_SHIFT)                  /* 0x00000004 */
#define CRU_GATE_CON03_PCLK_I2C0_EN_SHIFT                  (3U)
#define CRU_GATE_CON03_PCLK_I2C0_EN_MASK                   (0x1U << CRU_GATE_CON03_PCLK_I2C0_EN_SHIFT)                  /* 0x00000008 */
#define CRU_GATE_CON03_PCLK_I2C1_EN_SHIFT                  (4U)
#define CRU_GATE_CON03_PCLK_I2C1_EN_MASK                   (0x1U << CRU_GATE_CON03_PCLK_I2C1_EN_SHIFT)                  /* 0x00000010 */
#define CRU_GATE_CON03_PCLK_PWM_EN_SHIFT                   (5U)
#define CRU_GATE_CON03_PCLK_PWM_EN_MASK                    (0x1U << CRU_GATE_CON03_PCLK_PWM_EN_SHIFT)                   /* 0x00000020 */
#define CRU_GATE_CON03_PCLK_SPI0_EN_SHIFT                  (6U)
#define CRU_GATE_CON03_PCLK_SPI0_EN_MASK                   (0x1U << CRU_GATE_CON03_PCLK_SPI0_EN_SHIFT)                  /* 0x00000040 */
#define CRU_GATE_CON03_PCLK_SPI1_EN_SHIFT                  (7U)
#define CRU_GATE_CON03_PCLK_SPI1_EN_MASK                   (0x1U << CRU_GATE_CON03_PCLK_SPI1_EN_SHIFT)                  /* 0x00000080 */
#define CRU_GATE_CON03_PCLK_SARADC_CONTROL_EN_SHIFT        (8U)
#define CRU_GATE_CON03_PCLK_SARADC_CONTROL_EN_MASK         (0x1U << CRU_GATE_CON03_PCLK_SARADC_CONTROL_EN_SHIFT)        /* 0x00000100 */
#define CRU_GATE_CON03_PCLK_TIMER_EN_SHIFT                 (9U)
#define CRU_GATE_CON03_PCLK_TIMER_EN_MASK                  (0x1U << CRU_GATE_CON03_PCLK_TIMER_EN_SHIFT)                 /* 0x00000200 */
#define CRU_GATE_CON03_PCLK_WDT_EN_SHIFT                   (10U)
#define CRU_GATE_CON03_PCLK_WDT_EN_MASK                    (0x1U << CRU_GATE_CON03_PCLK_WDT_EN_SHIFT)                   /* 0x00000400 */
#define CRU_GATE_CON03_PCLK_CSI2HOST_EN_SHIFT              (11U)
#define CRU_GATE_CON03_PCLK_CSI2HOST_EN_MASK               (0x1U << CRU_GATE_CON03_PCLK_CSI2HOST_EN_SHIFT)              /* 0x00000800 */
/* GATE_CON04 */
#define CRU_GATE_CON04_OFFSET                              (0x310U)
#define CRU_GATE_CON04_PCLK_GPIO0_EN_SHIFT                 (4U)
#define CRU_GATE_CON04_PCLK_GPIO0_EN_MASK                  (0x1U << CRU_GATE_CON04_PCLK_GPIO0_EN_SHIFT)                 /* 0x00000010 */
#define CRU_GATE_CON04_PCLK_GPIO1_EN_SHIFT                 (5U)
#define CRU_GATE_CON04_PCLK_GPIO1_EN_MASK                  (0x1U << CRU_GATE_CON04_PCLK_GPIO1_EN_SHIFT)                 /* 0x00000020 */
#define CRU_GATE_CON04_PCLK_CRU_EN_SHIFT                   (6U)
#define CRU_GATE_CON04_PCLK_CRU_EN_MASK                    (0x1U << CRU_GATE_CON04_PCLK_CRU_EN_SHIFT)                   /* 0x00000040 */
#define CRU_GATE_CON04_PCLK_GRF_EN_SHIFT                   (8U)
#define CRU_GATE_CON04_PCLK_GRF_EN_MASK                    (0x1U << CRU_GATE_CON04_PCLK_GRF_EN_SHIFT)                   /* 0x00000100 */
#define CRU_GATE_CON04_PCLK_USBGRF_EN_SHIFT                (9U)
#define CRU_GATE_CON04_PCLK_USBGRF_EN_MASK                 (0x1U << CRU_GATE_CON04_PCLK_USBGRF_EN_SHIFT)                /* 0x00000200 */
#define CRU_GATE_CON04_DBCLK_GPIO0_EN_SHIFT                (10U)
#define CRU_GATE_CON04_DBCLK_GPIO0_EN_MASK                 (0x1U << CRU_GATE_CON04_DBCLK_GPIO0_EN_SHIFT)                /* 0x00000400 */
#define CRU_GATE_CON04_DBCLK_GPIO1_EN_SHIFT                (11U)
#define CRU_GATE_CON04_DBCLK_GPIO1_EN_MASK                 (0x1U << CRU_GATE_CON04_DBCLK_GPIO1_EN_SHIFT)                /* 0x00000800 */
#define CRU_GATE_CON04_CLK_SARADC_EN_SHIFT                 (12U)
#define CRU_GATE_CON04_CLK_SARADC_EN_MASK                  (0x1U << CRU_GATE_CON04_CLK_SARADC_EN_SHIFT)                 /* 0x00001000 */
#define CRU_GATE_CON04_CLK_MIPIPHY0_CFG_EN_SHIFT           (14U)
#define CRU_GATE_CON04_CLK_MIPIPHY0_CFG_EN_MASK            (0x1U << CRU_GATE_CON04_CLK_MIPIPHY0_CFG_EN_SHIFT)           /* 0x00004000 */
#define CRU_GATE_CON04_CLK_MIPIPHY1_CFG_EN_SHIFT           (15U)
#define CRU_GATE_CON04_CLK_MIPIPHY1_CFG_EN_MASK            (0x1U << CRU_GATE_CON04_CLK_MIPIPHY1_CFG_EN_SHIFT)           /* 0x00008000 */
/* GATE_CON05 */
#define CRU_GATE_CON05_OFFSET                              (0x314U)
#define CRU_GATE_CON05_CLK_I2C0_PLL_EN_SHIFT               (0U)
#define CRU_GATE_CON05_CLK_I2C0_PLL_EN_MASK                (0x1U << CRU_GATE_CON05_CLK_I2C0_PLL_EN_SHIFT)               /* 0x00000001 */
#define CRU_GATE_CON05_CLK_I2C1_PLL_EN_SHIFT               (1U)
#define CRU_GATE_CON05_CLK_I2C1_PLL_EN_MASK                (0x1U << CRU_GATE_CON05_CLK_I2C1_PLL_EN_SHIFT)               /* 0x00000002 */
#define CRU_GATE_CON05_CLK_SPI0_PLL_EN_SHIFT               (2U)
#define CRU_GATE_CON05_CLK_SPI0_PLL_EN_MASK                (0x1U << CRU_GATE_CON05_CLK_SPI0_PLL_EN_SHIFT)               /* 0x00000004 */
#define CRU_GATE_CON05_CLK_SPI1_PLL_EN_SHIFT               (3U)
#define CRU_GATE_CON05_CLK_SPI1_PLL_EN_MASK                (0x1U << CRU_GATE_CON05_CLK_SPI1_PLL_EN_SHIFT)               /* 0x00000008 */
#define CRU_GATE_CON05_CLK_PWM_PLL_EN_SHIFT                (4U)
#define CRU_GATE_CON05_CLK_PWM_PLL_EN_MASK                 (0x1U << CRU_GATE_CON05_CLK_PWM_PLL_EN_SHIFT)                /* 0x00000010 */
#define CRU_GATE_CON05_CLK_TIMER_PLL_EN_SHIFT              (5U)
#define CRU_GATE_CON05_CLK_TIMER_PLL_EN_MASK               (0x1U << CRU_GATE_CON05_CLK_TIMER_PLL_EN_SHIFT)              /* 0x00000020 */
#define CRU_GATE_CON05_CLK_TIMER0_EN_SHIFT                 (6U)
#define CRU_GATE_CON05_CLK_TIMER0_EN_MASK                  (0x1U << CRU_GATE_CON05_CLK_TIMER0_EN_SHIFT)                 /* 0x00000040 */
#define CRU_GATE_CON05_CLK_TIMER1_EN_SHIFT                 (7U)
#define CRU_GATE_CON05_CLK_TIMER1_EN_MASK                  (0x1U << CRU_GATE_CON05_CLK_TIMER1_EN_SHIFT)                 /* 0x00000080 */
#define CRU_GATE_CON05_CLK_TIMER2_EN_SHIFT                 (8U)
#define CRU_GATE_CON05_CLK_TIMER2_EN_MASK                  (0x1U << CRU_GATE_CON05_CLK_TIMER2_EN_SHIFT)                 /* 0x00000100 */
#define CRU_GATE_CON05_CLK_TIMER3_EN_SHIFT                 (9U)
#define CRU_GATE_CON05_CLK_TIMER3_EN_MASK                  (0x1U << CRU_GATE_CON05_CLK_TIMER3_EN_SHIFT)                 /* 0x00000200 */
#define CRU_GATE_CON05_CLK_TIMER4_EN_SHIFT                 (10U)
#define CRU_GATE_CON05_CLK_TIMER4_EN_MASK                  (0x1U << CRU_GATE_CON05_CLK_TIMER4_EN_SHIFT)                 /* 0x00000400 */
#define CRU_GATE_CON05_CLK_TIMER5_EN_SHIFT                 (11U)
#define CRU_GATE_CON05_CLK_TIMER5_EN_MASK                  (0x1U << CRU_GATE_CON05_CLK_TIMER5_EN_SHIFT)                 /* 0x00000800 */
#define CRU_GATE_CON05_CLK_XIP_SFC_DT50_EN_SHIFT           (13U)
#define CRU_GATE_CON05_CLK_XIP_SFC_DT50_EN_MASK            (0x1U << CRU_GATE_CON05_CLK_XIP_SFC_DT50_EN_SHIFT)           /* 0x00002000 */
#define CRU_GATE_CON05_CLK_PWM_CAPTURE_PLL_EN_SHIFT        (14U)
#define CRU_GATE_CON05_CLK_PWM_CAPTURE_PLL_EN_MASK         (0x1U << CRU_GATE_CON05_CLK_PWM_CAPTURE_PLL_EN_SHIFT)        /* 0x00004000 */
#define CRU_GATE_CON05_RTCCLK_MCU_EN_SHIFT                 (15U)
#define CRU_GATE_CON05_RTCCLK_MCU_EN_MASK                  (0x1U << CRU_GATE_CON05_RTCCLK_MCU_EN_SHIFT)                 /* 0x00008000 */
/* GATE_CON06 */
#define CRU_GATE_CON06_OFFSET                              (0x318U)
#define CRU_GATE_CON06_CLK_WDT_PLL_EN_SHIFT                (0U)
#define CRU_GATE_CON06_CLK_WDT_PLL_EN_MASK                 (0x1U << CRU_GATE_CON06_CLK_WDT_PLL_EN_SHIFT)                /* 0x00000001 */
#define CRU_GATE_CON06_CLK_ISP_PLL_EN_SHIFT                (1U)
#define CRU_GATE_CON06_CLK_ISP_PLL_EN_MASK                 (0x1U << CRU_GATE_CON06_CLK_ISP_PLL_EN_SHIFT)                /* 0x00000002 */
#define CRU_GATE_CON06_DCLK_VIPCAP_PLL_EN_SHIFT            (2U)
#define CRU_GATE_CON06_DCLK_VIPCAP_PLL_EN_MASK             (0x1U << CRU_GATE_CON06_DCLK_VIPCAP_PLL_EN_SHIFT)            /* 0x00000004 */
#define CRU_GATE_CON06_CLK_VIP_PLL_EN_SHIFT                (4U)
#define CRU_GATE_CON06_CLK_VIP_PLL_EN_MASK                 (0x1U << CRU_GATE_CON06_CLK_VIP_PLL_EN_SHIFT)                /* 0x00000010 */
#define CRU_GATE_CON06_CLK_USB_ADP_EN_SHIFT                (6U)
#define CRU_GATE_CON06_CLK_USB_ADP_EN_MASK                 (0x1U << CRU_GATE_CON06_CLK_USB_ADP_EN_SHIFT)                /* 0x00000040 */
#define CRU_GATE_CON06_OUTCLOCK_TEST_EN_SHIFT              (7U)
#define CRU_GATE_CON06_OUTCLOCK_TEST_EN_MASK               (0x1U << CRU_GATE_CON06_OUTCLOCK_TEST_EN_SHIFT)              /* 0x00000080 */
#define CRU_GATE_CON06_CLK_UART_PLL_EN_SHIFT               (8U)
#define CRU_GATE_CON06_CLK_UART_PLL_EN_MASK                (0x1U << CRU_GATE_CON06_CLK_UART_PLL_EN_SHIFT)               /* 0x00000100 */
#define CRU_GATE_CON06_CLK_UART_FRAC_EN_SHIFT              (9U)
#define CRU_GATE_CON06_CLK_UART_FRAC_EN_MASK               (0x1U << CRU_GATE_CON06_CLK_UART_FRAC_EN_SHIFT)              /* 0x00000200 */
#define CRU_GATE_CON06_SCLK_UART_EN_SHIFT                  (10U)
#define CRU_GATE_CON06_SCLK_UART_EN_MASK                   (0x1U << CRU_GATE_CON06_SCLK_UART_EN_SHIFT)                  /* 0x00000400 */
/* SSCGTBL0_3 */
#define CRU_SSCGTBL0_3_OFFSET                              (0x380U)
#define CRU_SSCGTBL0_3_SSCGTBL0_3_SHIFT                    (0U)
#define CRU_SSCGTBL0_3_SSCGTBL0_3_MASK                     (0xFFFFFFFFU << CRU_SSCGTBL0_3_SSCGTBL0_3_SHIFT)             /* 0xFFFFFFFF */
/* SSCGTBL4_7 */
#define CRU_SSCGTBL4_7_OFFSET                              (0x384U)
#define CRU_SSCGTBL4_7_SSCGTBL4_7_SHIFT                    (0U)
#define CRU_SSCGTBL4_7_SSCGTBL4_7_MASK                     (0xFFFFFFFFU << CRU_SSCGTBL4_7_SSCGTBL4_7_SHIFT)             /* 0xFFFFFFFF */
/* SSCGTBL8_11 */
#define CRU_SSCGTBL8_11_OFFSET                             (0x388U)
#define CRU_SSCGTBL8_11_SSCGTBL8_11_SHIFT                  (0U)
#define CRU_SSCGTBL8_11_SSCGTBL8_11_MASK                   (0xFFFFFFFFU << CRU_SSCGTBL8_11_SSCGTBL8_11_SHIFT)           /* 0xFFFFFFFF */
/* SSCGTBL12_15 */
#define CRU_SSCGTBL12_15_OFFSET                            (0x38CU)
#define CRU_SSCGTBL12_15_SSCGTBL12_15_SHIFT                (0U)
#define CRU_SSCGTBL12_15_SSCGTBL12_15_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL12_15_SSCGTBL12_15_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL16_19 */
#define CRU_SSCGTBL16_19_OFFSET                            (0x390U)
#define CRU_SSCGTBL16_19_SSCGTBL16_19_SHIFT                (0U)
#define CRU_SSCGTBL16_19_SSCGTBL16_19_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL16_19_SSCGTBL16_19_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL20_23 */
#define CRU_SSCGTBL20_23_OFFSET                            (0x394U)
#define CRU_SSCGTBL20_23_SSCGTBL20_23_SHIFT                (0U)
#define CRU_SSCGTBL20_23_SSCGTBL20_23_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL20_23_SSCGTBL20_23_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL24_27 */
#define CRU_SSCGTBL24_27_OFFSET                            (0x398U)
#define CRU_SSCGTBL24_27_SSCGTBL24_27_SHIFT                (0U)
#define CRU_SSCGTBL24_27_SSCGTBL24_27_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL24_27_SSCGTBL24_27_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL28_31 */
#define CRU_SSCGTBL28_31_OFFSET                            (0x39CU)
#define CRU_SSCGTBL28_31_SSCGTBL28_31_SHIFT                (0U)
#define CRU_SSCGTBL28_31_SSCGTBL28_31_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL28_31_SSCGTBL28_31_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL32_35 */
#define CRU_SSCGTBL32_35_OFFSET                            (0x3A0U)
#define CRU_SSCGTBL32_35_SSCGTBL32_35_SHIFT                (0U)
#define CRU_SSCGTBL32_35_SSCGTBL32_35_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL32_35_SSCGTBL32_35_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL36_39 */
#define CRU_SSCGTBL36_39_OFFSET                            (0x3A4U)
#define CRU_SSCGTBL36_39_SSCGTBL36_39_SHIFT                (0U)
#define CRU_SSCGTBL36_39_SSCGTBL36_39_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL36_39_SSCGTBL36_39_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL40_43 */
#define CRU_SSCGTBL40_43_OFFSET                            (0x3A8U)
#define CRU_SSCGTBL40_43_SSCGTBL40_43_SHIFT                (0U)
#define CRU_SSCGTBL40_43_SSCGTBL40_43_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL40_43_SSCGTBL40_43_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL44_47 */
#define CRU_SSCGTBL44_47_OFFSET                            (0x3ACU)
#define CRU_SSCGTBL44_47_SSCGTBL44_47_SHIFT                (0U)
#define CRU_SSCGTBL44_47_SSCGTBL44_47_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL44_47_SSCGTBL44_47_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL48_51 */
#define CRU_SSCGTBL48_51_OFFSET                            (0x3B0U)
#define CRU_SSCGTBL48_51_SSCGTBL48_51_SHIFT                (0U)
#define CRU_SSCGTBL48_51_SSCGTBL48_51_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL48_51_SSCGTBL48_51_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL52_55 */
#define CRU_SSCGTBL52_55_OFFSET                            (0x3B4U)
#define CRU_SSCGTBL52_55_SSCGTBL52_55_SHIFT                (0U)
#define CRU_SSCGTBL52_55_SSCGTBL52_55_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL52_55_SSCGTBL52_55_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL56_59 */
#define CRU_SSCGTBL56_59_OFFSET                            (0x3B8U)
#define CRU_SSCGTBL56_59_SSCGTBL56_59_SHIFT                (0U)
#define CRU_SSCGTBL56_59_SSCGTBL56_59_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL56_59_SSCGTBL56_59_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL60_63 */
#define CRU_SSCGTBL60_63_OFFSET                            (0x3BCU)
#define CRU_SSCGTBL60_63_SSCGTBL60_63_SHIFT                (0U)
#define CRU_SSCGTBL60_63_SSCGTBL60_63_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL60_63_SSCGTBL60_63_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL64_67 */
#define CRU_SSCGTBL64_67_OFFSET                            (0x3C0U)
#define CRU_SSCGTBL64_67_SSCGTBL64_67_SHIFT                (0U)
#define CRU_SSCGTBL64_67_SSCGTBL64_67_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL64_67_SSCGTBL64_67_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL68_71 */
#define CRU_SSCGTBL68_71_OFFSET                            (0x3C4U)
#define CRU_SSCGTBL68_71_SSCGTBL68_71_SHIFT                (0U)
#define CRU_SSCGTBL68_71_SSCGTBL68_71_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL68_71_SSCGTBL68_71_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL72_75 */
#define CRU_SSCGTBL72_75_OFFSET                            (0x3C8U)
#define CRU_SSCGTBL72_75_SSCGTBL72_75_SHIFT                (0U)
#define CRU_SSCGTBL72_75_SSCGTBL72_75_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL72_75_SSCGTBL72_75_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL76_79 */
#define CRU_SSCGTBL76_79_OFFSET                            (0x3CCU)
#define CRU_SSCGTBL76_79_SSCGTBL76_79_SHIFT                (0U)
#define CRU_SSCGTBL76_79_SSCGTBL76_79_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL76_79_SSCGTBL76_79_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL80_83 */
#define CRU_SSCGTBL80_83_OFFSET                            (0x3D0U)
#define CRU_SSCGTBL80_83_SSCGTBL80_83_SHIFT                (0U)
#define CRU_SSCGTBL80_83_SSCGTBL80_83_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL80_83_SSCGTBL80_83_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL84_87 */
#define CRU_SSCGTBL84_87_OFFSET                            (0x3D4U)
#define CRU_SSCGTBL84_87_SSCGTBL84_87_SHIFT                (0U)
#define CRU_SSCGTBL84_87_SSCGTBL84_87_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL84_87_SSCGTBL84_87_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL88_91 */
#define CRU_SSCGTBL88_91_OFFSET                            (0x3D8U)
#define CRU_SSCGTBL88_91_SSCGTBL88_91_SHIFT                (0U)
#define CRU_SSCGTBL88_91_SSCGTBL88_91_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL88_91_SSCGTBL88_91_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL92_95 */
#define CRU_SSCGTBL92_95_OFFSET                            (0x3DCU)
#define CRU_SSCGTBL92_95_SSCGTBL92_95_SHIFT                (0U)
#define CRU_SSCGTBL92_95_SSCGTBL92_95_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL92_95_SSCGTBL92_95_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL96_99 */
#define CRU_SSCGTBL96_99_OFFSET                            (0x3E0U)
#define CRU_SSCGTBL96_99_SSCGTBL96_99_SHIFT                (0U)
#define CRU_SSCGTBL96_99_SSCGTBL96_99_MASK                 (0xFFFFFFFFU << CRU_SSCGTBL96_99_SSCGTBL96_99_SHIFT)         /* 0xFFFFFFFF */
/* SSCGTBL100_103 */
#define CRU_SSCGTBL100_103_OFFSET                          (0x3E4U)
#define CRU_SSCGTBL100_103_SSCGTBL100_103_SHIFT            (0U)
#define CRU_SSCGTBL100_103_SSCGTBL100_103_MASK             (0xFFFFFFFFU << CRU_SSCGTBL100_103_SSCGTBL100_103_SHIFT)     /* 0xFFFFFFFF */
/* SSCGTBL104_107 */
#define CRU_SSCGTBL104_107_OFFSET                          (0x3E8U)
#define CRU_SSCGTBL104_107_SSCGTBL104_107_SHIFT            (0U)
#define CRU_SSCGTBL104_107_SSCGTBL104_107_MASK             (0xFFFFFFFFU << CRU_SSCGTBL104_107_SSCGTBL104_107_SHIFT)     /* 0xFFFFFFFF */
/* SSCGTBL108_111 */
#define CRU_SSCGTBL108_111_OFFSET                          (0x3ECU)
#define CRU_SSCGTBL108_111_SSCGTBL108_111_SHIFT            (0U)
#define CRU_SSCGTBL108_111_SSCGTBL108_111_MASK             (0xFFFFFFFFU << CRU_SSCGTBL108_111_SSCGTBL108_111_SHIFT)     /* 0xFFFFFFFF */
/* SSCGTBL112_115 */
#define CRU_SSCGTBL112_115_OFFSET                          (0x3F0U)
#define CRU_SSCGTBL112_115_SSCGTBL112_115_SHIFT            (0U)
#define CRU_SSCGTBL112_115_SSCGTBL112_115_MASK             (0xFFFFFFFFU << CRU_SSCGTBL112_115_SSCGTBL112_115_SHIFT)     /* 0xFFFFFFFF */
/* SSCGTBL116_119 */
#define CRU_SSCGTBL116_119_OFFSET                          (0x3F4U)
#define CRU_SSCGTBL116_119_SSCGTBL116_119_SHIFT            (0U)
#define CRU_SSCGTBL116_119_SSCGTBL116_119_MASK             (0xFFFFFFFFU << CRU_SSCGTBL116_119_SSCGTBL116_119_SHIFT)     /* 0xFFFFFFFF */
/* SSCGTBL120_123 */
#define CRU_SSCGTBL120_123_OFFSET                          (0x3F8U)
#define CRU_SSCGTBL120_123_SSCGTBL120_123_SHIFT            (0U)
#define CRU_SSCGTBL120_123_SSCGTBL120_123_MASK             (0xFFFFFFFFU << CRU_SSCGTBL120_123_SSCGTBL120_123_SHIFT)     /* 0xFFFFFFFF */
/* SSCGTBL124_127 */
#define CRU_SSCGTBL124_127_OFFSET                          (0x3FCU)
#define CRU_SSCGTBL124_127_SSCGTBL124_127_SHIFT            (0U)
#define CRU_SSCGTBL124_127_SSCGTBL124_127_MASK             (0xFFFFFFFFU << CRU_SSCGTBL124_127_SSCGTBL124_127_SHIFT)     /* 0xFFFFFFFF */
/* SOFTRST_CON00 */
#define CRU_SOFTRST_CON00_OFFSET                           (0x400U)
#define CRU_SOFTRST_CON00_HRESETN_LOGIC_BUS_AC_SHIFT       (1U)
#define CRU_SOFTRST_CON00_HRESETN_LOGIC_BUS_AC_MASK        (0x1U << CRU_SOFTRST_CON00_HRESETN_LOGIC_BUS_AC_SHIFT)       /* 0x00000002 */
#define CRU_SOFTRST_CON00_RESETN_MCU_AC_SHIFT              (2U)
#define CRU_SOFTRST_CON00_RESETN_MCU_AC_MASK               (0x1U << CRU_SOFTRST_CON00_RESETN_MCU_AC_SHIFT)              /* 0x00000004 */
#define CRU_SOFTRST_CON00_PRESETN_LOGIC_BUS_AC_SHIFT       (3U)
#define CRU_SOFTRST_CON00_PRESETN_LOGIC_BUS_AC_MASK        (0x1U << CRU_SOFTRST_CON00_PRESETN_LOGIC_BUS_AC_SHIFT)       /* 0x00000008 */
/* SOFTRST_CON01 */
#define CRU_SOFTRST_CON01_OFFSET                           (0x404U)
#define CRU_SOFTRST_CON01_HRESETN_LOGIC_BUS_NIU_SHIFT      (1U)
#define CRU_SOFTRST_CON01_HRESETN_LOGIC_BUS_NIU_MASK       (0x1U << CRU_SOFTRST_CON01_HRESETN_LOGIC_BUS_NIU_SHIFT)      /* 0x00000002 */
#define CRU_SOFTRST_CON01_RESETN_MCU_SHIFT                 (2U)
#define CRU_SOFTRST_CON01_RESETN_MCU_MASK                  (0x1U << CRU_SOFTRST_CON01_RESETN_MCU_SHIFT)                 /* 0x00000004 */
#define CRU_SOFTRST_CON01_HRESETN_INTMEM0_SHIFT            (3U)
#define CRU_SOFTRST_CON01_HRESETN_INTMEM0_MASK             (0x1U << CRU_SOFTRST_CON01_HRESETN_INTMEM0_SHIFT)            /* 0x00000008 */
#define CRU_SOFTRST_CON01_HRESETN_INTMEM1_SHIFT            (4U)
#define CRU_SOFTRST_CON01_HRESETN_INTMEM1_MASK             (0x1U << CRU_SOFTRST_CON01_HRESETN_INTMEM1_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST_CON01_HRESETN_INTMEM2_SHIFT            (5U)
#define CRU_SOFTRST_CON01_HRESETN_INTMEM2_MASK             (0x1U << CRU_SOFTRST_CON01_HRESETN_INTMEM2_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST_CON01_HRESETN_ROM_SHIFT                (6U)
#define CRU_SOFTRST_CON01_HRESETN_ROM_MASK                 (0x1U << CRU_SOFTRST_CON01_HRESETN_ROM_SHIFT)                /* 0x00000040 */
#define CRU_SOFTRST_CON01_HRESETN_SFC_SHIFT                (7U)
#define CRU_SOFTRST_CON01_HRESETN_SFC_MASK                 (0x1U << CRU_SOFTRST_CON01_HRESETN_SFC_SHIFT)                /* 0x00000080 */
#define CRU_SOFTRST_CON01_HRESETN_SFC_XIP_SHIFT            (8U)
#define CRU_SOFTRST_CON01_HRESETN_SFC_XIP_MASK             (0x1U << CRU_SOFTRST_CON01_HRESETN_SFC_XIP_SHIFT)            /* 0x00000100 */
#define CRU_SOFTRST_CON01_HRESETN_USB_SHIFT                (9U)
#define CRU_SOFTRST_CON01_HRESETN_USB_MASK                 (0x1U << CRU_SOFTRST_CON01_HRESETN_USB_SHIFT)                /* 0x00000200 */
#define CRU_SOFTRST_CON01_HRESETN_USB_PMU_SHIFT            (10U)
#define CRU_SOFTRST_CON01_HRESETN_USB_PMU_MASK             (0x1U << CRU_SOFTRST_CON01_HRESETN_USB_PMU_SHIFT)            /* 0x00000400 */
#define CRU_SOFTRST_CON01_HRESETN_CACHE_SHIFT              (11U)
#define CRU_SOFTRST_CON01_HRESETN_CACHE_MASK               (0x1U << CRU_SOFTRST_CON01_HRESETN_CACHE_SHIFT)              /* 0x00000800 */
#define CRU_SOFTRST_CON01_ARESETN_VIPCAP_SHIFT             (12U)
#define CRU_SOFTRST_CON01_ARESETN_VIPCAP_MASK              (0x1U << CRU_SOFTRST_CON01_ARESETN_VIPCAP_SHIFT)             /* 0x00001000 */
#define CRU_SOFTRST_CON01_HRESETN_VIPCAP_SHIFT             (13U)
#define CRU_SOFTRST_CON01_HRESETN_VIPCAP_MASK              (0x1U << CRU_SOFTRST_CON01_HRESETN_VIPCAP_SHIFT)             /* 0x00002000 */
#define CRU_SOFTRST_CON01_HRESETN_ISP_SHIFT                (15U)
#define CRU_SOFTRST_CON01_HRESETN_ISP_MASK                 (0x1U << CRU_SOFTRST_CON01_HRESETN_ISP_SHIFT)                /* 0x00008000 */
/* SOFTRST_CON02 */
#define CRU_SOFTRST_CON02_OFFSET                           (0x408U)
#define CRU_SOFTRST_CON02_HRESETN_JPEG_ENC0_SHIFT          (0U)
#define CRU_SOFTRST_CON02_HRESETN_JPEG_ENC0_MASK           (0x1U << CRU_SOFTRST_CON02_HRESETN_JPEG_ENC0_SHIFT)          /* 0x00000001 */
#define CRU_SOFTRST_CON02_HRESETN_JPEG_ENC1_SHIFT          (1U)
#define CRU_SOFTRST_CON02_HRESETN_JPEG_ENC1_MASK           (0x1U << CRU_SOFTRST_CON02_HRESETN_JPEG_ENC1_SHIFT)          /* 0x00000002 */
#define CRU_SOFTRST_CON02_ARESETN_LOGIC_BUS_NIU_SHIFT      (2U)
#define CRU_SOFTRST_CON02_ARESETN_LOGIC_BUS_NIU_MASK       (0x1U << CRU_SOFTRST_CON02_ARESETN_LOGIC_BUS_NIU_SHIFT)      /* 0x00000004 */
#define CRU_SOFTRST_CON02_HRESETN_DMAC_SHIFT               (3U)
#define CRU_SOFTRST_CON02_HRESETN_DMAC_MASK                (0x1U << CRU_SOFTRST_CON02_HRESETN_DMAC_SHIFT)               /* 0x00000008 */
#define CRU_SOFTRST_CON02_HRESETN_JHASH_M_SHIFT            (4U)
#define CRU_SOFTRST_CON02_HRESETN_JHASH_M_MASK             (0x1U << CRU_SOFTRST_CON02_HRESETN_JHASH_M_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST_CON02_HRESETN_JHASH_S_SHIFT            (5U)
#define CRU_SOFTRST_CON02_HRESETN_JHASH_S_MASK             (0x1U << CRU_SOFTRST_CON02_HRESETN_JHASH_S_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST_CON02_HRESETN_HOST_ARB_SHIFT           (6U)
#define CRU_SOFTRST_CON02_HRESETN_HOST_ARB_MASK            (0x1U << CRU_SOFTRST_CON02_HRESETN_HOST_ARB_SHIFT)           /* 0x00000040 */
#define CRU_SOFTRST_CON02_ARESETN_JPEG_BUS_NIU_SHIFT       (9U)
#define CRU_SOFTRST_CON02_ARESETN_JPEG_BUS_NIU_MASK        (0x1U << CRU_SOFTRST_CON02_ARESETN_JPEG_BUS_NIU_SHIFT)       /* 0x00000200 */
#define CRU_SOFTRST_CON02_ARESETN_JPEG_ENC0_SHIFT          (10U)
#define CRU_SOFTRST_CON02_ARESETN_JPEG_ENC0_MASK           (0x1U << CRU_SOFTRST_CON02_ARESETN_JPEG_ENC0_SHIFT)          /* 0x00000400 */
#define CRU_SOFTRST_CON02_ARESETN_JPEG_ENC1_SHIFT          (11U)
#define CRU_SOFTRST_CON02_ARESETN_JPEG_ENC1_MASK           (0x1U << CRU_SOFTRST_CON02_ARESETN_JPEG_ENC1_SHIFT)          /* 0x00000800 */
/* SOFTRST_CON03 */
#define CRU_SOFTRST_CON03_OFFSET                           (0x40CU)
#define CRU_SOFTRST_CON03_PRESETN_LOGIC_BUS_NIU_SHIFT      (1U)
#define CRU_SOFTRST_CON03_PRESETN_LOGIC_BUS_NIU_MASK       (0x1U << CRU_SOFTRST_CON03_PRESETN_LOGIC_BUS_NIU_SHIFT)      /* 0x00000002 */
#define CRU_SOFTRST_CON03_PRESETN_UART_SHIFT               (2U)
#define CRU_SOFTRST_CON03_PRESETN_UART_MASK                (0x1U << CRU_SOFTRST_CON03_PRESETN_UART_SHIFT)               /* 0x00000004 */
#define CRU_SOFTRST_CON03_PRESETN_I2C0_SHIFT               (3U)
#define CRU_SOFTRST_CON03_PRESETN_I2C0_MASK                (0x1U << CRU_SOFTRST_CON03_PRESETN_I2C0_SHIFT)               /* 0x00000008 */
#define CRU_SOFTRST_CON03_PRESETN_I2C1_SHIFT               (4U)
#define CRU_SOFTRST_CON03_PRESETN_I2C1_MASK                (0x1U << CRU_SOFTRST_CON03_PRESETN_I2C1_SHIFT)               /* 0x00000010 */
#define CRU_SOFTRST_CON03_PRESETN_PWM_SHIFT                (5U)
#define CRU_SOFTRST_CON03_PRESETN_PWM_MASK                 (0x1U << CRU_SOFTRST_CON03_PRESETN_PWM_SHIFT)                /* 0x00000020 */
#define CRU_SOFTRST_CON03_PRESETN_SPI0_SHIFT               (6U)
#define CRU_SOFTRST_CON03_PRESETN_SPI0_MASK                (0x1U << CRU_SOFTRST_CON03_PRESETN_SPI0_SHIFT)               /* 0x00000040 */
#define CRU_SOFTRST_CON03_PRESETN_SPI1_SHIFT               (7U)
#define CRU_SOFTRST_CON03_PRESETN_SPI1_MASK                (0x1U << CRU_SOFTRST_CON03_PRESETN_SPI1_SHIFT)               /* 0x00000080 */
#define CRU_SOFTRST_CON03_PRESETN_SARADC_CONTROL_SHIFT     (8U)
#define CRU_SOFTRST_CON03_PRESETN_SARADC_CONTROL_MASK      (0x1U << CRU_SOFTRST_CON03_PRESETN_SARADC_CONTROL_SHIFT)     /* 0x00000100 */
#define CRU_SOFTRST_CON03_PRESETN_TIMER_SHIFT              (9U)
#define CRU_SOFTRST_CON03_PRESETN_TIMER_MASK               (0x1U << CRU_SOFTRST_CON03_PRESETN_TIMER_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST_CON03_PRESETN_WDT_SHIFT                (10U)
#define CRU_SOFTRST_CON03_PRESETN_WDT_MASK                 (0x1U << CRU_SOFTRST_CON03_PRESETN_WDT_SHIFT)                /* 0x00000400 */
#define CRU_SOFTRST_CON03_PRESETN_CSI2HOST_SHIFT           (11U)
#define CRU_SOFTRST_CON03_PRESETN_CSI2HOST_MASK            (0x1U << CRU_SOFTRST_CON03_PRESETN_CSI2HOST_SHIFT)           /* 0x00000800 */
/* SOFTRST_CON04 */
#define CRU_SOFTRST_CON04_OFFSET                           (0x410U)
#define CRU_SOFTRST_CON04_PRESETN_GPIO0_SHIFT              (4U)
#define CRU_SOFTRST_CON04_PRESETN_GPIO0_MASK               (0x1U << CRU_SOFTRST_CON04_PRESETN_GPIO0_SHIFT)              /* 0x00000010 */
#define CRU_SOFTRST_CON04_PRESETN_GPIO1_SHIFT              (5U)
#define CRU_SOFTRST_CON04_PRESETN_GPIO1_MASK               (0x1U << CRU_SOFTRST_CON04_PRESETN_GPIO1_SHIFT)              /* 0x00000020 */
#define CRU_SOFTRST_CON04_PRESETN_CRU_SHIFT                (6U)
#define CRU_SOFTRST_CON04_PRESETN_CRU_MASK                 (0x1U << CRU_SOFTRST_CON04_PRESETN_CRU_SHIFT)                /* 0x00000040 */
#define CRU_SOFTRST_CON04_PRESETN_GRF_REMAP_SHIFT          (7U)
#define CRU_SOFTRST_CON04_PRESETN_GRF_REMAP_MASK           (0x1U << CRU_SOFTRST_CON04_PRESETN_GRF_REMAP_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST_CON04_PRESETN_GRF_SHIFT                (8U)
#define CRU_SOFTRST_CON04_PRESETN_GRF_MASK                 (0x1U << CRU_SOFTRST_CON04_PRESETN_GRF_SHIFT)                /* 0x00000100 */
#define CRU_SOFTRST_CON04_PRESETN_USBGRF_SHIFT             (9U)
#define CRU_SOFTRST_CON04_PRESETN_USBGRF_MASK              (0x1U << CRU_SOFTRST_CON04_PRESETN_USBGRF_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST_CON04_DBRESETN_GPIO0_SHIFT             (10U)
#define CRU_SOFTRST_CON04_DBRESETN_GPIO0_MASK              (0x1U << CRU_SOFTRST_CON04_DBRESETN_GPIO0_SHIFT)             /* 0x00000400 */
#define CRU_SOFTRST_CON04_DBRESETN_GPIO1_SHIFT             (11U)
#define CRU_SOFTRST_CON04_DBRESETN_GPIO1_MASK              (0x1U << CRU_SOFTRST_CON04_DBRESETN_GPIO1_SHIFT)             /* 0x00000800 */
#define CRU_SOFTRST_CON04_RESETN_USBPHY_SHIFT              (13U)
#define CRU_SOFTRST_CON04_RESETN_USBPHY_MASK               (0x1U << CRU_SOFTRST_CON04_RESETN_USBPHY_SHIFT)              /* 0x00002000 */
/* SOFTRST_CON05 */
#define CRU_SOFTRST_CON05_OFFSET                           (0x414U)
#define CRU_SOFTRST_CON05_RESETN_I2C0_SHIFT                (0U)
#define CRU_SOFTRST_CON05_RESETN_I2C0_MASK                 (0x1U << CRU_SOFTRST_CON05_RESETN_I2C0_SHIFT)                /* 0x00000001 */
#define CRU_SOFTRST_CON05_RESETN_I2C1_SHIFT                (1U)
#define CRU_SOFTRST_CON05_RESETN_I2C1_MASK                 (0x1U << CRU_SOFTRST_CON05_RESETN_I2C1_SHIFT)                /* 0x00000002 */
#define CRU_SOFTRST_CON05_RESETN_SPI0_SHIFT                (2U)
#define CRU_SOFTRST_CON05_RESETN_SPI0_MASK                 (0x1U << CRU_SOFTRST_CON05_RESETN_SPI0_SHIFT)                /* 0x00000004 */
#define CRU_SOFTRST_CON05_RESETN_SPI1_SHIFT                (3U)
#define CRU_SOFTRST_CON05_RESETN_SPI1_MASK                 (0x1U << CRU_SOFTRST_CON05_RESETN_SPI1_SHIFT)                /* 0x00000008 */
#define CRU_SOFTRST_CON05_RESETN_PWM_SHIFT                 (4U)
#define CRU_SOFTRST_CON05_RESETN_PWM_MASK                  (0x1U << CRU_SOFTRST_CON05_RESETN_PWM_SHIFT)                 /* 0x00000010 */
#define CRU_SOFTRST_CON05_RESETN_TIMER0_SHIFT              (6U)
#define CRU_SOFTRST_CON05_RESETN_TIMER0_MASK               (0x1U << CRU_SOFTRST_CON05_RESETN_TIMER0_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST_CON05_RESETN_TIMER1_SHIFT              (7U)
#define CRU_SOFTRST_CON05_RESETN_TIMER1_MASK               (0x1U << CRU_SOFTRST_CON05_RESETN_TIMER1_SHIFT)              /* 0x00000080 */
#define CRU_SOFTRST_CON05_RESETN_TIMER2_SHIFT              (8U)
#define CRU_SOFTRST_CON05_RESETN_TIMER2_MASK               (0x1U << CRU_SOFTRST_CON05_RESETN_TIMER2_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST_CON05_RESETN_TIMER3_SHIFT              (9U)
#define CRU_SOFTRST_CON05_RESETN_TIMER3_MASK               (0x1U << CRU_SOFTRST_CON05_RESETN_TIMER3_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST_CON05_RESETN_TIMER4_SHIFT              (10U)
#define CRU_SOFTRST_CON05_RESETN_TIMER4_MASK               (0x1U << CRU_SOFTRST_CON05_RESETN_TIMER4_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST_CON05_RESETN_TIMER5_SHIFT              (11U)
#define CRU_SOFTRST_CON05_RESETN_TIMER5_MASK               (0x1U << CRU_SOFTRST_CON05_RESETN_TIMER5_SHIFT)              /* 0x00000800 */
#define CRU_SOFTRST_CON05_MCU_JTRST_SHIFT                  (12U)
#define CRU_SOFTRST_CON05_MCU_JTRST_MASK                   (0x1U << CRU_SOFTRST_CON05_MCU_JTRST_SHIFT)                  /* 0x00001000 */
#define CRU_SOFTRST_CON05_RESETN_XIP_SFC_SHIFT             (13U)
#define CRU_SOFTRST_CON05_RESETN_XIP_SFC_MASK              (0x1U << CRU_SOFTRST_CON05_RESETN_XIP_SFC_SHIFT)             /* 0x00002000 */
/* SOFTRST_CON06 */
#define CRU_SOFTRST_CON06_OFFSET                           (0x418U)
#define CRU_SOFTRST_CON06_RESETN_ISP_SHIFT                 (1U)
#define CRU_SOFTRST_CON06_RESETN_ISP_MASK                  (0x1U << CRU_SOFTRST_CON06_RESETN_ISP_SHIFT)                 /* 0x00000002 */
#define CRU_SOFTRST_CON06_DRESETN_VIPCAP_SHIFT             (2U)
#define CRU_SOFTRST_CON06_DRESETN_VIPCAP_MASK              (0x1U << CRU_SOFTRST_CON06_DRESETN_VIPCAP_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON06_IRESETN_VIPCAP_SHIFT             (3U)
#define CRU_SOFTRST_CON06_IRESETN_VIPCAP_MASK              (0x1U << CRU_SOFTRST_CON06_IRESETN_VIPCAP_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON06_RESETN_UTMI_SHIFT                (5U)
#define CRU_SOFTRST_CON06_RESETN_UTMI_MASK                 (0x1U << CRU_SOFTRST_CON06_RESETN_UTMI_SHIFT)                /* 0x00000020 */
#define CRU_SOFTRST_CON06_RESETN_USB_ADP_SHIFT             (6U)
#define CRU_SOFTRST_CON06_RESETN_USB_ADP_MASK              (0x1U << CRU_SOFTRST_CON06_RESETN_USB_ADP_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST_CON06_SRESETN_UART_SHIFT               (10U)
#define CRU_SOFTRST_CON06_SRESETN_UART_MASK                (0x1U << CRU_SOFTRST_CON06_SRESETN_UART_SHIFT)               /* 0x00000400 */
/******************************************GRF*******************************************/
/* GPIO0A_IOMUX_L */
#define GRF_GPIO0A_IOMUX_L_OFFSET                          (0x0U)
#define GRF_GPIO0A_IOMUX_L_GPIO0A0_SEL_SHIFT               (0U)
#define GRF_GPIO0A_IOMUX_L_GPIO0A0_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_L_GPIO0A0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO0A_IOMUX_L_GPIO0A1_SEL_SHIFT               (4U)
#define GRF_GPIO0A_IOMUX_L_GPIO0A1_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_L_GPIO0A1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO0A_IOMUX_L_GPIO0A2_SEL_SHIFT               (8U)
#define GRF_GPIO0A_IOMUX_L_GPIO0A2_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_L_GPIO0A2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO0A_IOMUX_L_GPIO0A3_SEL_SHIFT               (12U)
#define GRF_GPIO0A_IOMUX_L_GPIO0A3_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_L_GPIO0A3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO0A_IOMUX_H */
#define GRF_GPIO0A_IOMUX_H_OFFSET                          (0x4U)
#define GRF_GPIO0A_IOMUX_H_GPIO0A4_SEL_SHIFT               (0U)
#define GRF_GPIO0A_IOMUX_H_GPIO0A4_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_H_GPIO0A4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO0A_IOMUX_H_GPIO0A5_SEL_SHIFT               (4U)
#define GRF_GPIO0A_IOMUX_H_GPIO0A5_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_H_GPIO0A5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO0A_IOMUX_H_GPIO0A6_SEL_SHIFT               (8U)
#define GRF_GPIO0A_IOMUX_H_GPIO0A6_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_H_GPIO0A6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO0A_IOMUX_H_GPIO0A7_SEL_SHIFT               (12U)
#define GRF_GPIO0A_IOMUX_H_GPIO0A7_SEL_MASK                (0xFU << GRF_GPIO0A_IOMUX_H_GPIO0A7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO0B_IOMUX_L */
#define GRF_GPIO0B_IOMUX_L_OFFSET                          (0x8U)
#define GRF_GPIO0B_IOMUX_L_GPIO0B0_SEL_SHIFT               (0U)
#define GRF_GPIO0B_IOMUX_L_GPIO0B0_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_L_GPIO0B0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO0B_IOMUX_L_GPIO0B1_SEL_SHIFT               (4U)
#define GRF_GPIO0B_IOMUX_L_GPIO0B1_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_L_GPIO0B1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO0B_IOMUX_L_GPIO0B2_SEL_SHIFT               (8U)
#define GRF_GPIO0B_IOMUX_L_GPIO0B2_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_L_GPIO0B2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO0B_IOMUX_L_GPIO0B3_SEL_SHIFT               (12U)
#define GRF_GPIO0B_IOMUX_L_GPIO0B3_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_L_GPIO0B3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO0B_IOMUX_H */
#define GRF_GPIO0B_IOMUX_H_OFFSET                          (0xCU)
#define GRF_GPIO0B_IOMUX_H_GPIO0B4_SEL_SHIFT               (0U)
#define GRF_GPIO0B_IOMUX_H_GPIO0B4_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_H_GPIO0B4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO0B_IOMUX_H_GPIO0B5_SEL_SHIFT               (4U)
#define GRF_GPIO0B_IOMUX_H_GPIO0B5_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_H_GPIO0B5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO0B_IOMUX_H_GPIO0B6_SEL_SHIFT               (8U)
#define GRF_GPIO0B_IOMUX_H_GPIO0B6_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_H_GPIO0B6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO0B_IOMUX_H_GPIO0B7_SEL_SHIFT               (12U)
#define GRF_GPIO0B_IOMUX_H_GPIO0B7_SEL_MASK                (0xFU << GRF_GPIO0B_IOMUX_H_GPIO0B7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO0C_IOMUX_L */
#define GRF_GPIO0C_IOMUX_L_OFFSET                          (0x10U)
#define GRF_GPIO0C_IOMUX_L_GPIO0C0_SEL_SHIFT               (0U)
#define GRF_GPIO0C_IOMUX_L_GPIO0C0_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_L_GPIO0C0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO0C_IOMUX_L_GPIO0C1_SEL_SHIFT               (4U)
#define GRF_GPIO0C_IOMUX_L_GPIO0C1_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_L_GPIO0C1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO0C_IOMUX_L_GPIO0C2_SEL_SHIFT               (8U)
#define GRF_GPIO0C_IOMUX_L_GPIO0C2_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_L_GPIO0C2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO0C_IOMUX_L_GPIO0C3_SEL_SHIFT               (12U)
#define GRF_GPIO0C_IOMUX_L_GPIO0C3_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_L_GPIO0C3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO0C_IOMUX_H */
#define GRF_GPIO0C_IOMUX_H_OFFSET                          (0x14U)
#define GRF_GPIO0C_IOMUX_H_GPIO0C4_SEL_SHIFT               (0U)
#define GRF_GPIO0C_IOMUX_H_GPIO0C4_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_H_GPIO0C4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO0C_IOMUX_H_GPIO0C5_SEL_SHIFT               (4U)
#define GRF_GPIO0C_IOMUX_H_GPIO0C5_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_H_GPIO0C5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO0C_IOMUX_H_GPIO0C6_SEL_SHIFT               (8U)
#define GRF_GPIO0C_IOMUX_H_GPIO0C6_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_H_GPIO0C6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO0C_IOMUX_H_GPIO0C7_SEL_SHIFT               (12U)
#define GRF_GPIO0C_IOMUX_H_GPIO0C7_SEL_MASK                (0xFU << GRF_GPIO0C_IOMUX_H_GPIO0C7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1A_IOMUX_L */
#define GRF_GPIO1A_IOMUX_L_OFFSET                          (0x20U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_SHIFT               (0U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_SHIFT               (4U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_SHIFT               (8U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_SHIFT               (12U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1A_IOMUX_H */
#define GRF_GPIO1A_IOMUX_H_OFFSET                          (0x24U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_SHIFT               (0U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_SHIFT               (4U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_SHIFT               (8U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_SHIFT               (12U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1B_IOMUX_L */
#define GRF_GPIO1B_IOMUX_L_OFFSET                          (0x28U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_SHIFT               (0U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_SHIFT               (4U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_SHIFT               (8U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_SHIFT               (12U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1B_IOMUX_H */
#define GRF_GPIO1B_IOMUX_H_OFFSET                          (0x2CU)
#define GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_SHIFT               (0U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_SHIFT               (4U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_SHIFT               (8U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_SHIFT               (12U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1C_IOMUX_L */
#define GRF_GPIO1C_IOMUX_L_OFFSET                          (0x30U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C0_SEL_SHIFT               (0U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C0_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1C_IOMUX_L_GPIO1C1_SEL_SHIFT               (4U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C1_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1C_IOMUX_L_GPIO1C2_SEL_SHIFT               (8U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C2_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1C_IOMUX_L_GPIO1C3_SEL_SHIFT               (12U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C3_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1C_IOMUX_H */
#define GRF_GPIO1C_IOMUX_H_OFFSET                          (0x34U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C4_SEL_SHIFT               (0U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C4_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1C_IOMUX_H_GPIO1C5_SEL_SHIFT               (4U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C5_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1C_IOMUX_H_GPIO1C6_SEL_SHIFT               (8U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C6_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1C_IOMUX_H_GPIO1C7_SEL_SHIFT               (12U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C7_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO0A_P */
#define GRF_GPIO0A_P_OFFSET                                (0x100U)
#define GRF_GPIO0A_P_GPIO0A0_P_SHIFT                       (0U)
#define GRF_GPIO0A_P_GPIO0A0_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO0A_P_GPIO0A1_P_SHIFT                       (2U)
#define GRF_GPIO0A_P_GPIO0A1_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO0A_P_GPIO0A2_P_SHIFT                       (4U)
#define GRF_GPIO0A_P_GPIO0A2_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO0A_P_GPIO0A3_P_SHIFT                       (6U)
#define GRF_GPIO0A_P_GPIO0A3_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO0A_P_GPIO0A4_P_SHIFT                       (8U)
#define GRF_GPIO0A_P_GPIO0A4_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO0A_P_GPIO0A5_P_SHIFT                       (10U)
#define GRF_GPIO0A_P_GPIO0A5_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO0A_P_GPIO0A6_P_SHIFT                       (12U)
#define GRF_GPIO0A_P_GPIO0A6_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO0A_P_GPIO0A7_P_SHIFT                       (14U)
#define GRF_GPIO0A_P_GPIO0A7_P_MASK                        (0x3U << GRF_GPIO0A_P_GPIO0A7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO0B_P */
#define GRF_GPIO0B_P_OFFSET                                (0x104U)
#define GRF_GPIO0B_P_GPIO0B0_P_SHIFT                       (0U)
#define GRF_GPIO0B_P_GPIO0B0_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO0B_P_GPIO0B1_P_SHIFT                       (2U)
#define GRF_GPIO0B_P_GPIO0B1_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO0B_P_GPIO0B2_P_SHIFT                       (4U)
#define GRF_GPIO0B_P_GPIO0B2_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO0B_P_GPIO0B3_P_SHIFT                       (6U)
#define GRF_GPIO0B_P_GPIO0B3_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO0B_P_GPIO0B4_P_SHIFT                       (8U)
#define GRF_GPIO0B_P_GPIO0B4_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO0B_P_GPIO0B5_P_SHIFT                       (10U)
#define GRF_GPIO0B_P_GPIO0B5_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO0B_P_GPIO0B6_P_SHIFT                       (12U)
#define GRF_GPIO0B_P_GPIO0B6_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO0B_P_GPIO0B7_P_SHIFT                       (14U)
#define GRF_GPIO0B_P_GPIO0B7_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO0C_P */
#define GRF_GPIO0C_P_OFFSET                                (0x108U)
#define GRF_GPIO0C_P_GPIO0C0_P_SHIFT                       (0U)
#define GRF_GPIO0C_P_GPIO0C0_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO0C_P_GPIO0C1_P_SHIFT                       (2U)
#define GRF_GPIO0C_P_GPIO0C1_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO0C_P_GPIO0C2_P_SHIFT                       (4U)
#define GRF_GPIO0C_P_GPIO0C2_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO0C_P_GPIO0C3_P_SHIFT                       (6U)
#define GRF_GPIO0C_P_GPIO0C3_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO0C_P_GPIO0C4_P_SHIFT                       (8U)
#define GRF_GPIO0C_P_GPIO0C4_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO0C_P_GPIO0C5_P_SHIFT                       (10U)
#define GRF_GPIO0C_P_GPIO0C5_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO0C_P_GPIO0C6_P_SHIFT                       (12U)
#define GRF_GPIO0C_P_GPIO0C6_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO0C_P_GPIO0C7_P_SHIFT                       (14U)
#define GRF_GPIO0C_P_GPIO0C7_P_MASK                        (0x3U << GRF_GPIO0C_P_GPIO0C7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO1A_P */
#define GRF_GPIO1A_P_OFFSET                                (0x110U)
#define GRF_GPIO1A_P_GPIO1A0_P_SHIFT                       (0U)
#define GRF_GPIO1A_P_GPIO1A0_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1A_P_GPIO1A1_P_SHIFT                       (2U)
#define GRF_GPIO1A_P_GPIO1A1_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1A_P_GPIO1A2_P_SHIFT                       (4U)
#define GRF_GPIO1A_P_GPIO1A2_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1A_P_GPIO1A3_P_SHIFT                       (6U)
#define GRF_GPIO1A_P_GPIO1A3_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1A_P_GPIO1A4_P_SHIFT                       (8U)
#define GRF_GPIO1A_P_GPIO1A4_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1A_P_GPIO1A5_P_SHIFT                       (10U)
#define GRF_GPIO1A_P_GPIO1A5_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1A_P_GPIO1A6_P_SHIFT                       (12U)
#define GRF_GPIO1A_P_GPIO1A6_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1A_P_GPIO1A7_P_SHIFT                       (14U)
#define GRF_GPIO1A_P_GPIO1A7_P_MASK                        (0x3U << GRF_GPIO1A_P_GPIO1A7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO1B_P */
#define GRF_GPIO1B_P_OFFSET                                (0x114U)
#define GRF_GPIO1B_P_GPIO1B0_P_SHIFT                       (0U)
#define GRF_GPIO1B_P_GPIO1B0_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1B_P_GPIO1B1_P_SHIFT                       (2U)
#define GRF_GPIO1B_P_GPIO1B1_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1B_P_GPIO1B2_P_SHIFT                       (4U)
#define GRF_GPIO1B_P_GPIO1B2_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1B_P_GPIO1B3_P_SHIFT                       (6U)
#define GRF_GPIO1B_P_GPIO1B3_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1B_P_GPIO1B4_P_SHIFT                       (8U)
#define GRF_GPIO1B_P_GPIO1B4_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1B_P_GPIO1B5_P_SHIFT                       (10U)
#define GRF_GPIO1B_P_GPIO1B5_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1B_P_GPIO1B6_P_SHIFT                       (12U)
#define GRF_GPIO1B_P_GPIO1B6_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1B_P_GPIO1B7_P_SHIFT                       (14U)
#define GRF_GPIO1B_P_GPIO1B7_P_MASK                        (0x3U << GRF_GPIO1B_P_GPIO1B7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO1C_P */
#define GRF_GPIO1C_P_OFFSET                                (0x118U)
#define GRF_GPIO1C_P_GPIO1C0_P_SHIFT                       (0U)
#define GRF_GPIO1C_P_GPIO1C0_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1C_P_GPIO1C1_P_SHIFT                       (2U)
#define GRF_GPIO1C_P_GPIO1C1_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1C_P_GPIO1C2_P_SHIFT                       (4U)
#define GRF_GPIO1C_P_GPIO1C2_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1C_P_GPIO1C3_P_SHIFT                       (6U)
#define GRF_GPIO1C_P_GPIO1C3_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1C_P_GPIO1C4_P_SHIFT                       (8U)
#define GRF_GPIO1C_P_GPIO1C4_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1C_P_GPIO1C5_P_SHIFT                       (10U)
#define GRF_GPIO1C_P_GPIO1C5_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1C_P_GPIO1C6_P_SHIFT                       (12U)
#define GRF_GPIO1C_P_GPIO1C6_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1C_P_GPIO1C7_P_SHIFT                       (14U)
#define GRF_GPIO1C_P_GPIO1C7_P_MASK                        (0x3U << GRF_GPIO1C_P_GPIO1C7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO0A_IE */
#define GRF_GPIO0A_IE_OFFSET                               (0x180U)
#define GRF_GPIO0A_IE_GPIO0A0_IE_SHIFT                     (0U)
#define GRF_GPIO0A_IE_GPIO0A0_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A0_IE_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO0A_IE_GPIO0A1_IE_SHIFT                     (1U)
#define GRF_GPIO0A_IE_GPIO0A1_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A1_IE_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO0A_IE_GPIO0A2_IE_SHIFT                     (2U)
#define GRF_GPIO0A_IE_GPIO0A2_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A2_IE_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO0A_IE_GPIO0A3_IE_SHIFT                     (3U)
#define GRF_GPIO0A_IE_GPIO0A3_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A3_IE_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO0A_IE_GPIO0A4_IE_SHIFT                     (4U)
#define GRF_GPIO0A_IE_GPIO0A4_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A4_IE_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO0A_IE_GPIO0A5_IE_SHIFT                     (5U)
#define GRF_GPIO0A_IE_GPIO0A5_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A5_IE_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO0A_IE_GPIO0A6_IE_SHIFT                     (6U)
#define GRF_GPIO0A_IE_GPIO0A6_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A6_IE_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO0A_IE_GPIO0A7_IE_SHIFT                     (7U)
#define GRF_GPIO0A_IE_GPIO0A7_IE_MASK                      (0x1U << GRF_GPIO0A_IE_GPIO0A7_IE_SHIFT)                     /* 0x00000080 */
/* GPIO0B_IE */
#define GRF_GPIO0B_IE_OFFSET                               (0x184U)
#define GRF_GPIO0B_IE_GPIO0B0_IE_SHIFT                     (0U)
#define GRF_GPIO0B_IE_GPIO0B0_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B0_IE_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO0B_IE_GPIO0B1_IE_SHIFT                     (1U)
#define GRF_GPIO0B_IE_GPIO0B1_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B1_IE_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO0B_IE_GPIO0B2_IE_SHIFT                     (2U)
#define GRF_GPIO0B_IE_GPIO0B2_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B2_IE_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO0B_IE_GPIO0B3_IE_SHIFT                     (3U)
#define GRF_GPIO0B_IE_GPIO0B3_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B3_IE_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO0B_IE_GPIO0B4_IE_SHIFT                     (4U)
#define GRF_GPIO0B_IE_GPIO0B4_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B4_IE_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO0B_IE_GPIO0B5_IE_SHIFT                     (5U)
#define GRF_GPIO0B_IE_GPIO0B5_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B5_IE_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO0B_IE_GPIO0B6_IE_SHIFT                     (6U)
#define GRF_GPIO0B_IE_GPIO0B6_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B6_IE_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO0B_IE_GPIO0B7_IE_SHIFT                     (7U)
#define GRF_GPIO0B_IE_GPIO0B7_IE_MASK                      (0x1U << GRF_GPIO0B_IE_GPIO0B7_IE_SHIFT)                     /* 0x00000080 */
/* GPIO0C_IE */
#define GRF_GPIO0C_IE_OFFSET                               (0x188U)
#define GRF_GPIO0C_IE_GPIO0C0_IE_SHIFT                     (0U)
#define GRF_GPIO0C_IE_GPIO0C0_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C0_IE_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO0C_IE_GPIO0C1_IE_SHIFT                     (1U)
#define GRF_GPIO0C_IE_GPIO0C1_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C1_IE_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO0C_IE_GPIO0C2_IE_SHIFT                     (2U)
#define GRF_GPIO0C_IE_GPIO0C2_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C2_IE_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO0C_IE_GPIO0C3_IE_SHIFT                     (3U)
#define GRF_GPIO0C_IE_GPIO0C3_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C3_IE_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO0C_IE_GPIO0C4_IE_SHIFT                     (4U)
#define GRF_GPIO0C_IE_GPIO0C4_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C4_IE_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO0C_IE_GPIO0C5_IE_SHIFT                     (5U)
#define GRF_GPIO0C_IE_GPIO0C5_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C5_IE_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO0C_IE_GPIO0C6_IE_SHIFT                     (6U)
#define GRF_GPIO0C_IE_GPIO0C6_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C6_IE_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO0C_IE_GPIO0C7_IE_SHIFT                     (7U)
#define GRF_GPIO0C_IE_GPIO0C7_IE_MASK                      (0x1U << GRF_GPIO0C_IE_GPIO0C7_IE_SHIFT)                     /* 0x00000080 */
/* GPIO1A_IE */
#define GRF_GPIO1A_IE_OFFSET                               (0x190U)
#define GRF_GPIO1A_IE_GPIO1A0_IE_SHIFT                     (0U)
#define GRF_GPIO1A_IE_GPIO1A0_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A0_IE_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1A_IE_GPIO1A1_IE_SHIFT                     (1U)
#define GRF_GPIO1A_IE_GPIO1A1_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A1_IE_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1A_IE_GPIO1A2_IE_SHIFT                     (2U)
#define GRF_GPIO1A_IE_GPIO1A2_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A2_IE_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1A_IE_GPIO1A3_IE_SHIFT                     (3U)
#define GRF_GPIO1A_IE_GPIO1A3_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A3_IE_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1A_IE_GPIO1A4_IE_SHIFT                     (4U)
#define GRF_GPIO1A_IE_GPIO1A4_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A4_IE_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1A_IE_GPIO1A5_IE_SHIFT                     (5U)
#define GRF_GPIO1A_IE_GPIO1A5_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A5_IE_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1A_IE_GPIO1A6_IE_SHIFT                     (6U)
#define GRF_GPIO1A_IE_GPIO1A6_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A6_IE_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1A_IE_GPIO1A7_IE_SHIFT                     (7U)
#define GRF_GPIO1A_IE_GPIO1A7_IE_MASK                      (0x1U << GRF_GPIO1A_IE_GPIO1A7_IE_SHIFT)                     /* 0x00000080 */
/* GPIO1B_IE */
#define GRF_GPIO1B_IE_OFFSET                               (0x194U)
#define GRF_GPIO1B_IE_GPIO1B0_IE_SHIFT                     (0U)
#define GRF_GPIO1B_IE_GPIO1B0_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B0_IE_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1B_IE_GPIO1B1_IE_SHIFT                     (1U)
#define GRF_GPIO1B_IE_GPIO1B1_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B1_IE_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1B_IE_GPIO1B2_IE_SHIFT                     (2U)
#define GRF_GPIO1B_IE_GPIO1B2_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B2_IE_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1B_IE_GPIO1B3_IE_SHIFT                     (3U)
#define GRF_GPIO1B_IE_GPIO1B3_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B3_IE_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1B_IE_GPIO1B4_IE_SHIFT                     (4U)
#define GRF_GPIO1B_IE_GPIO1B4_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B4_IE_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1B_IE_GPIO1B5_IE_SHIFT                     (5U)
#define GRF_GPIO1B_IE_GPIO1B5_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B5_IE_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1B_IE_GPIO1B6_IE_SHIFT                     (6U)
#define GRF_GPIO1B_IE_GPIO1B6_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B6_IE_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1B_IE_GPIO1B7_IE_SHIFT                     (7U)
#define GRF_GPIO1B_IE_GPIO1B7_IE_MASK                      (0x1U << GRF_GPIO1B_IE_GPIO1B7_IE_SHIFT)                     /* 0x00000080 */
/* GPIO1C_IE */
#define GRF_GPIO1C_IE_OFFSET                               (0x198U)
#define GRF_GPIO1C_IE_GPIO1C0_IE_SHIFT                     (0U)
#define GRF_GPIO1C_IE_GPIO1C0_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C0_IE_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1C_IE_GPIO1C1_IE_SHIFT                     (1U)
#define GRF_GPIO1C_IE_GPIO1C1_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C1_IE_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1C_IE_GPIO1C2_IE_SHIFT                     (2U)
#define GRF_GPIO1C_IE_GPIO1C2_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C2_IE_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1C_IE_GPIO1C3_IE_SHIFT                     (3U)
#define GRF_GPIO1C_IE_GPIO1C3_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C3_IE_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1C_IE_GPIO1C4_IE_SHIFT                     (4U)
#define GRF_GPIO1C_IE_GPIO1C4_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C4_IE_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1C_IE_GPIO1C5_IE_SHIFT                     (5U)
#define GRF_GPIO1C_IE_GPIO1C5_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C5_IE_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1C_IE_GPIO1C6_IE_SHIFT                     (6U)
#define GRF_GPIO1C_IE_GPIO1C6_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C6_IE_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1C_IE_GPIO1C7_IE_SHIFT                     (7U)
#define GRF_GPIO1C_IE_GPIO1C7_IE_MASK                      (0x1U << GRF_GPIO1C_IE_GPIO1C7_IE_SHIFT)                     /* 0x00000080 */
/* SOC_CON0 */
#define GRF_SOC_CON0_OFFSET                                (0x200U)
#define GRF_SOC_CON0_CACHE_FLUSH_REQ_SHIFT                 (0U)
#define GRF_SOC_CON0_CACHE_FLUSH_REQ_MASK                  (0x1U << GRF_SOC_CON0_CACHE_FLUSH_REQ_SHIFT)                 /* 0x00000001 */
#define GRF_SOC_CON0_BOOTROM_AUTO_EN_SHIFT                 (1U)
#define GRF_SOC_CON0_BOOTROM_AUTO_EN_MASK                  (0x1U << GRF_SOC_CON0_BOOTROM_AUTO_EN_SHIFT)                 /* 0x00000002 */
#define GRF_SOC_CON0_INTMEM0_AUTO_EN_SHIFT                 (2U)
#define GRF_SOC_CON0_INTMEM0_AUTO_EN_MASK                  (0x1U << GRF_SOC_CON0_INTMEM0_AUTO_EN_SHIFT)                 /* 0x00000004 */
#define GRF_SOC_CON0_INTMEM1_AUTO_EN_SHIFT                 (3U)
#define GRF_SOC_CON0_INTMEM1_AUTO_EN_MASK                  (0x1U << GRF_SOC_CON0_INTMEM1_AUTO_EN_SHIFT)                 /* 0x00000008 */
#define GRF_SOC_CON0_INTMEM2_AUTO_EN_SHIFT                 (4U)
#define GRF_SOC_CON0_INTMEM2_AUTO_EN_MASK                  (0x1U << GRF_SOC_CON0_INTMEM2_AUTO_EN_SHIFT)                 /* 0x00000010 */
#define GRF_SOC_CON0_UART_CTS_INVERT_SHIFT                 (5U)
#define GRF_SOC_CON0_UART_CTS_INVERT_MASK                  (0x1U << GRF_SOC_CON0_UART_CTS_INVERT_SHIFT)                 /* 0x00000020 */
#define GRF_SOC_CON0_UART_RTS_INVERT_SHIFT                 (6U)
#define GRF_SOC_CON0_UART_RTS_INVERT_MASK                  (0x1U << GRF_SOC_CON0_UART_RTS_INVERT_SHIFT)                 /* 0x00000040 */
#define GRF_SOC_CON0_DPHY_RX0_CLK_INV_SEL_SHIFT            (7U)
#define GRF_SOC_CON0_DPHY_RX0_CLK_INV_SEL_MASK             (0x1U << GRF_SOC_CON0_DPHY_RX0_CLK_INV_SEL_SHIFT)            /* 0x00000080 */
#define GRF_SOC_CON0_DISABLE_ISP_SHIFT                     (8U)
#define GRF_SOC_CON0_DISABLE_ISP_MASK                      (0x1U << GRF_SOC_CON0_DISABLE_ISP_SHIFT)                     /* 0x00000100 */
#define GRF_SOC_CON0_DPHY_RX1_CLK_INV_SEL_SHIFT            (9U)
#define GRF_SOC_CON0_DPHY_RX1_CLK_INV_SEL_MASK             (0x1U << GRF_SOC_CON0_DPHY_RX1_CLK_INV_SEL_SHIFT)            /* 0x00000200 */
#define GRF_SOC_CON0_GRF_HOST_ARB_PAUSE_SHIFT              (10U)
#define GRF_SOC_CON0_GRF_HOST_ARB_PAUSE_MASK               (0x1U << GRF_SOC_CON0_GRF_HOST_ARB_PAUSE_SHIFT)              /* 0x00000400 */
#define GRF_SOC_CON0_TCM_AUTO_EN_SHIFT                     (11U)
#define GRF_SOC_CON0_TCM_AUTO_EN_MASK                      (0x1U << GRF_SOC_CON0_TCM_AUTO_EN_SHIFT)                     /* 0x00000800 */
#define GRF_SOC_CON0_INTMEM0_SEL_SHIFT                     (12U)
#define GRF_SOC_CON0_INTMEM0_SEL_MASK                      (0x3U << GRF_SOC_CON0_INTMEM0_SEL_SHIFT)                     /* 0x00003000 */
#define GRF_SOC_CON0_WDT_SPEED_UP_SHIFT                    (14U)
#define GRF_SOC_CON0_WDT_SPEED_UP_MASK                     (0x1U << GRF_SOC_CON0_WDT_SPEED_UP_SHIFT)                    /* 0x00004000 */
#define GRF_SOC_CON0_WDT_PAUSE_SHIFT                       (15U)
#define GRF_SOC_CON0_WDT_PAUSE_MASK                        (0x1U << GRF_SOC_CON0_WDT_PAUSE_SHIFT)                       /* 0x00008000 */
/* SOC_CON1 */
#define GRF_SOC_CON1_OFFSET                                (0x204U)
#define GRF_SOC_CON1_UART_SIN_SEL_SHIFT                    (0U)
#define GRF_SOC_CON1_UART_SIN_SEL_MASK                     (0x1U << GRF_SOC_CON1_UART_SIN_SEL_SHIFT)                    /* 0x00000001 */
/* SOC_CON2 */
#define GRF_SOC_CON2_OFFSET                                (0x208U)
#define GRF_SOC_CON2_DPHY_RX0_TESTDIN_SHIFT                (0U)
#define GRF_SOC_CON2_DPHY_RX0_TESTDIN_MASK                 (0xFFU << GRF_SOC_CON2_DPHY_RX0_TESTDIN_SHIFT)               /* 0x000000FF */
#define GRF_SOC_CON2_DPHY_RX0_TESTEN_SHIFT                 (8U)
#define GRF_SOC_CON2_DPHY_RX0_TESTEN_MASK                  (0x1U << GRF_SOC_CON2_DPHY_RX0_TESTEN_SHIFT)                 /* 0x00000100 */
#define GRF_SOC_CON2_DPHY_RX0_TESTCLK_SHIFT                (9U)
#define GRF_SOC_CON2_DPHY_RX0_TESTCLK_MASK                 (0x1U << GRF_SOC_CON2_DPHY_RX0_TESTCLK_SHIFT)                /* 0x00000200 */
#define GRF_SOC_CON2_DPHY_RX0_TESTCLR_SHIFT                (10U)
#define GRF_SOC_CON2_DPHY_RX0_TESTCLR_MASK                 (0x1U << GRF_SOC_CON2_DPHY_RX0_TESTCLR_SHIFT)                /* 0x00000400 */
/* SOC_CON3 */
#define GRF_SOC_CON3_OFFSET                                (0x20CU)
#define GRF_SOC_CON3_DPHY_RX0_ENABLE_SHIFT                 (0U)
#define GRF_SOC_CON3_DPHY_RX0_ENABLE_MASK                  (0x3U << GRF_SOC_CON3_DPHY_RX0_ENABLE_SHIFT)                 /* 0x00000003 */
/* SOC_CON4 */
#define GRF_SOC_CON4_OFFSET                                (0x210U)
#define GRF_SOC_CON4_DPHY_RX1_ERR_CONTROL_SHIFT            (0U)
#define GRF_SOC_CON4_DPHY_RX1_ERR_CONTROL_MASK             (0xFU << GRF_SOC_CON4_DPHY_RX1_ERR_CONTROL_SHIFT)            /* 0x0000000F */
#define GRF_SOC_CON4_DPHY_RX1_ENABLE_SEL_SHIFT             (4U)
#define GRF_SOC_CON4_DPHY_RX1_ENABLE_SEL_MASK              (0x1U << GRF_SOC_CON4_DPHY_RX1_ENABLE_SEL_SHIFT)             /* 0x00000010 */
#define GRF_SOC_CON4_DPHY_RX1_S_ENABLECLK_SHIFT            (5U)
#define GRF_SOC_CON4_DPHY_RX1_S_ENABLECLK_MASK             (0x1U << GRF_SOC_CON4_DPHY_RX1_S_ENABLECLK_SHIFT)            /* 0x00000020 */
#define GRF_SOC_CON4_DPHY_RX1_ENABLE_SHIFT                 (8U)
#define GRF_SOC_CON4_DPHY_RX1_ENABLE_MASK                  (0x3U << GRF_SOC_CON4_DPHY_RX1_ENABLE_SHIFT)                 /* 0x00000180 */
/* SOC_CON5 */
#define GRF_SOC_CON5_OFFSET                                (0x214U)
#define GRF_SOC_CON5_SOC_REMAP_SHIFT                       (0U)
#define GRF_SOC_CON5_SOC_REMAP_MASK                        (0x1U << GRF_SOC_CON5_SOC_REMAP_SHIFT)                       /* 0x00000001 */
/* SOC_STATUS0 */
#define GRF_SOC_STATUS0_OFFSET                             (0x280U)
#define GRF_SOC_STATUS0_GPLL_LOCK_SHIFT                    (0U)
#define GRF_SOC_STATUS0_GPLL_LOCK_MASK                     (0x1U << GRF_SOC_STATUS0_GPLL_LOCK_SHIFT)                    /* 0x00000001 */
#define GRF_SOC_STATUS0_TIMER0_EN_SHIFT                    (1U)
#define GRF_SOC_STATUS0_TIMER0_EN_MASK                     (0x1U << GRF_SOC_STATUS0_TIMER0_EN_SHIFT)                    /* 0x00000002 */
#define GRF_SOC_STATUS0_TIMER1_EN_SHIFT                    (2U)
#define GRF_SOC_STATUS0_TIMER1_EN_MASK                     (0x1U << GRF_SOC_STATUS0_TIMER1_EN_SHIFT)                    /* 0x00000004 */
#define GRF_SOC_STATUS0_TIMER2_EN_SHIFT                    (3U)
#define GRF_SOC_STATUS0_TIMER2_EN_MASK                     (0x1U << GRF_SOC_STATUS0_TIMER2_EN_SHIFT)                    /* 0x00000008 */
#define GRF_SOC_STATUS0_TIMER3_EN_SHIFT                    (4U)
#define GRF_SOC_STATUS0_TIMER3_EN_MASK                     (0x1U << GRF_SOC_STATUS0_TIMER3_EN_SHIFT)                    /* 0x00000010 */
#define GRF_SOC_STATUS0_TIMER4_EN_SHIFT                    (5U)
#define GRF_SOC_STATUS0_TIMER4_EN_MASK                     (0x1U << GRF_SOC_STATUS0_TIMER4_EN_SHIFT)                    /* 0x00000020 */
#define GRF_SOC_STATUS0_TIMER5_EN_SHIFT                    (6U)
#define GRF_SOC_STATUS0_TIMER5_EN_MASK                     (0x1U << GRF_SOC_STATUS0_TIMER5_EN_SHIFT)                    /* 0x00000040 */
#define GRF_SOC_STATUS0_CACHE_FLUSH_ACK_SHIFT              (7U)
#define GRF_SOC_STATUS0_CACHE_FLUSH_ACK_MASK               (0x1U << GRF_SOC_STATUS0_CACHE_FLUSH_ACK_SHIFT)              /* 0x00000080 */
#define GRF_SOC_STATUS0_UTMI_IDDIG_SHIFT                   (8U)
#define GRF_SOC_STATUS0_UTMI_IDDIG_MASK                    (0x1U << GRF_SOC_STATUS0_UTMI_IDDIG_SHIFT)                   /* 0x00000100 */
#define GRF_SOC_STATUS0_UTMI_BVALID_SHIFT                  (9U)
#define GRF_SOC_STATUS0_UTMI_BVALID_MASK                   (0x1U << GRF_SOC_STATUS0_UTMI_BVALID_SHIFT)                  /* 0x00000200 */
#define GRF_SOC_STATUS0_UTMI_LINESTATE_SHIFT               (10U)
#define GRF_SOC_STATUS0_UTMI_LINESTATE_MASK                (0x3U << GRF_SOC_STATUS0_UTMI_LINESTATE_SHIFT)               /* 0x00000C00 */
#define GRF_SOC_STATUS0_UTMI_VBUSVALID_SHIFT               (12U)
#define GRF_SOC_STATUS0_UTMI_VBUSVALID_MASK                (0x1U << GRF_SOC_STATUS0_UTMI_VBUSVALID_SHIFT)               /* 0x00001000 */
#define GRF_SOC_STATUS0_SCR0_SLEEP_SHIFT                   (13U)
#define GRF_SOC_STATUS0_SCR0_SLEEP_MASK                    (0x1U << GRF_SOC_STATUS0_SCR0_SLEEP_SHIFT)                   /* 0x00002000 */
#define GRF_SOC_STATUS0_CACHE_IDLE_SHIFT                   (14U)
#define GRF_SOC_STATUS0_CACHE_IDLE_MASK                    (0x1U << GRF_SOC_STATUS0_CACHE_IDLE_SHIFT)                   /* 0x00004000 */
#define GRF_SOC_STATUS0_SCR0_RST_N_STS_SHIFT               (15U)
#define GRF_SOC_STATUS0_SCR0_RST_N_STS_MASK                (0x1U << GRF_SOC_STATUS0_SCR0_RST_N_STS_SHIFT)               /* 0x00008000 */
#define GRF_SOC_STATUS0_JPEG0_LOW_CLK_FLAG_ACLK_PD_SHIFT   (16U)
#define GRF_SOC_STATUS0_JPEG0_LOW_CLK_FLAG_ACLK_PD_MASK    (0x1U << GRF_SOC_STATUS0_JPEG0_LOW_CLK_FLAG_ACLK_PD_SHIFT)   /* 0x00010000 */
#define GRF_SOC_STATUS0_JPEG1_LOW_CLK_FLAG_ACLK_PD_SHIFT   (17U)
#define GRF_SOC_STATUS0_JPEG1_LOW_CLK_FLAG_ACLK_PD_MASK    (0x1U << GRF_SOC_STATUS0_JPEG1_LOW_CLK_FLAG_ACLK_PD_SHIFT)   /* 0x00020000 */
/* MCU_STATUS0 */
#define GRF_MCU_STATUS0_OFFSET                             (0x300U)
#define GRF_MCU_STATUS0_SCR0_SLEEP_SHIFT                   (0U)
#define GRF_MCU_STATUS0_SCR0_SLEEP_MASK                    (0x1U << GRF_MCU_STATUS0_SCR0_SLEEP_SHIFT)                   /* 0x00000001 */
#define GRF_MCU_STATUS0_SCR0_RST_N_STS_SHIFT               (1U)
#define GRF_MCU_STATUS0_SCR0_RST_N_STS_MASK                (0x1U << GRF_MCU_STATUS0_SCR0_RST_N_STS_SHIFT)               /* 0x00000002 */
/* MCU_STATUS1 */
#define GRF_MCU_STATUS1_OFFSET                             (0x304U)
#define GRF_MCU_STATUS1                                    (0x0U)
#define GRF_MCU_STATUS1_DPHY_RX0_TESTDOUT_SHIFT            (0U)
#define GRF_MCU_STATUS1_DPHY_RX0_TESTDOUT_MASK             (0xFFU << GRF_MCU_STATUS1_DPHY_RX0_TESTDOUT_SHIFT)           /* 0x000000FF */
/* UOC_CON0 */
#define GRF_UOC_CON0_OFFSET                                (0x340U)
#define GRF_UOC_CON0_USB2OTG_SCALEDOWN_MODE_SHIFT          (0U)
#define GRF_UOC_CON0_USB2OTG_SCALEDOWN_MODE_MASK           (0x3U << GRF_UOC_CON0_USB2OTG_SCALEDOWN_MODE_SHIFT)          /* 0x00000003 */
#define GRF_UOC_CON0_USB2OTG_DBNCE_FLTER_BYPASS_SHIFT      (2U)
#define GRF_UOC_CON0_USB2OTG_DBNCE_FLTER_BYPASS_MASK       (0x1U << GRF_UOC_CON0_USB2OTG_DBNCE_FLTER_BYPASS_SHIFT)      /* 0x00000004 */
/* FAST_BOOT */
#define GRF_FAST_BOOT_OFFSET                               (0x400U)
#define GRF_FAST_BOOT_GRF_FAST_BOOT_SHIFT                  (0U)
#define GRF_FAST_BOOT_GRF_FAST_BOOT_MASK                   (0x1U << GRF_FAST_BOOT_GRF_FAST_BOOT_SHIFT)                  /* 0x00000001 */
/* FAST_BOOT_ADDR */
#define GRF_FAST_BOOT_ADDR_OFFSET                          (0x404U)
#define GRF_FAST_BOOT_ADDR_GRF_FAST_BOOT_ADDR_SHIFT        (0U)
#define GRF_FAST_BOOT_ADDR_GRF_FAST_BOOT_ADDR_MASK         (0xFFFFFFFFU << GRF_FAST_BOOT_ADDR_GRF_FAST_BOOT_ADDR_SHIFT) /* 0xFFFFFFFF */
/* MCU_CON0 */
#define GRF_MCU_CON0_OFFSET                                (0x500U)
#define GRF_MCU_CON0_GRF_CON_M0_STALIB_SHIFT               (0U)
#define GRF_MCU_CON0_GRF_CON_M0_STALIB_MASK                (0x3FFFFFFU << GRF_MCU_CON0_GRF_CON_M0_STALIB_SHIFT)         /* 0x03FFFFFF */
/* MCU_CON1 */
#define GRF_MCU_CON1_OFFSET                                (0x504U)
#define GRF_MCU_CON1_GRF_CON_M0_DBGRESTART_SHIFT           (0U)
#define GRF_MCU_CON1_GRF_CON_M0_DBGRESTART_MASK            (0x1U << GRF_MCU_CON1_GRF_CON_M0_DBGRESTART_SHIFT)           /* 0x00000001 */
#define GRF_MCU_CON1_GRF_CON_M0_EDBGRP_SHIFT               (1U)
#define GRF_MCU_CON1_GRF_CON_M0_EDBGRP_MASK                (0x1U << GRF_MCU_CON1_GRF_CON_M0_EDBGRP_SHIFT)               /* 0x00000002 */
#define GRF_MCU_CON1_GRF_CON_M0_NMI_SHIFT                  (2U)
#define GRF_MCU_CON1_GRF_CON_M0_NMI_MASK                   (0x1U << GRF_MCU_CON1_GRF_CON_M0_NMI_SHIFT)                  /* 0x00000004 */
#define GRF_MCU_CON1_GRF_CON_M0_RXEV_SHIFT                 (3U)
#define GRF_MCU_CON1_GRF_CON_M0_RXEV_MASK                  (0x1U << GRF_MCU_CON1_GRF_CON_M0_RXEV_SHIFT)                 /* 0x00000008 */
#define GRF_MCU_CON1_GRF_CON_M0_SLEEPHOLDREQN_SHIFT        (4U)
#define GRF_MCU_CON1_GRF_CON_M0_SLEEPHOLDREQN_MASK         (0x1U << GRF_MCU_CON1_GRF_CON_M0_SLEEPHOLDREQN_SHIFT)        /* 0x00000010 */
#define GRF_MCU_CON1_GRF_CON_M0_WICENREQ_SHIFT             (5U)
#define GRF_MCU_CON1_GRF_CON_M0_WICENREQ_MASK              (0x1U << GRF_MCU_CON1_GRF_CON_M0_WICENREQ_SHIFT)             /* 0x00000020 */
#define GRF_MCU_CON1_GRF_CON_M0_IRQLATENCY_SHIFT           (7U)
#define GRF_MCU_CON1_GRF_CON_M0_IRQLATENCY_MASK            (0xFFU << GRF_MCU_CON1_GRF_CON_M0_IRQLATENCY_SHIFT)          /* 0x00007F80 */
/* OS_REG0 */
#define GRF_OS_REG0_OFFSET                                 (0x800U)
#define GRF_OS_REG0_OS_REGISTER0_SHIFT                     (0U)
#define GRF_OS_REG0_OS_REGISTER0_MASK                      (0xFFFFFFFFU << GRF_OS_REG0_OS_REGISTER0_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG1 */
#define GRF_OS_REG1_OFFSET                                 (0x804U)
#define GRF_OS_REG1_OS_REGISTER1_SHIFT                     (0U)
#define GRF_OS_REG1_OS_REGISTER1_MASK                      (0xFFFFFFFFU << GRF_OS_REG1_OS_REGISTER1_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG2 */
#define GRF_OS_REG2_OFFSET                                 (0x808U)
#define GRF_OS_REG2_OS_REGISTER2_SHIFT                     (0U)
#define GRF_OS_REG2_OS_REGISTER2_MASK                      (0xFFFFFFFFU << GRF_OS_REG2_OS_REGISTER2_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG3 */
#define GRF_OS_REG3_OFFSET                                 (0x80CU)
#define GRF_OS_REG3_OS_REGISTER3_SHIFT                     (0U)
#define GRF_OS_REG3_OS_REGISTER3_MASK                      (0xFFFFFFFFU << GRF_OS_REG3_OS_REGISTER3_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG4 */
#define GRF_OS_REG4_OFFSET                                 (0x810U)
#define GRF_OS_REG4_OS_REGISTER4_SHIFT                     (0U)
#define GRF_OS_REG4_OS_REGISTER4_MASK                      (0xFFFFFFFFU << GRF_OS_REG4_OS_REGISTER4_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG5 */
#define GRF_OS_REG5_OFFSET                                 (0x814U)
#define GRF_OS_REG5_OS_REGISTER5_SHIFT                     (0U)
#define GRF_OS_REG5_OS_REGISTER5_MASK                      (0xFFFFFFFFU << GRF_OS_REG5_OS_REGISTER5_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG6 */
#define GRF_OS_REG6_OFFSET                                 (0x818U)
#define GRF_OS_REG6_OS_REGISTER6_SHIFT                     (0U)
#define GRF_OS_REG6_OS_REGISTER6_MASK                      (0xFFFFFFFFU << GRF_OS_REG6_OS_REGISTER6_SHIFT)              /* 0xFFFFFFFF */
/* OS_REG7 */
#define GRF_OS_REG7_OFFSET                                 (0x81CU)
#define GRF_OS_REG7_OS_REGISTER7_SHIFT                     (0U)
#define GRF_OS_REG7_OS_REGISTER7_MASK                      (0xFFFFFFFFU << GRF_OS_REG7_OS_REGISTER7_SHIFT)              /* 0xFFFFFFFF */
/* SOC_VERSION */
#define GRF_SOC_VERSION_OFFSET                             (0x900U)
#define GRF_SOC_VERSION_SOC_VERSION_SHIFT                  (0U)
#define GRF_SOC_VERSION_SOC_VERSION_MASK                   (0xFFFFFFFFU << GRF_SOC_VERSION_SOC_VERSION_SHIFT)           /* 0xFFFFFFFF */
/****************************************USB_GRF*****************************************/
/* REG0 */
#define USB_GRF_REG0_OFFSET                                (0x0U)
#define USB_GRF_REG0_USBPHY_REG0_SHIFT                     (0U)
#define USB_GRF_REG0_USBPHY_REG0_MASK                      (0xFFFFU << USB_GRF_REG0_USBPHY_REG0_SHIFT)                  /* 0x0000FFFF */
/* REG1 */
#define USB_GRF_REG1_OFFSET                                (0x4U)
#define USB_GRF_REG1_USBPHY_REG1_SHIFT                     (0U)
#define USB_GRF_REG1_USBPHY_REG1_MASK                      (0xFFFFU << USB_GRF_REG1_USBPHY_REG1_SHIFT)                  /* 0x0000FFFF */
/* REG2 */
#define USB_GRF_REG2_OFFSET                                (0x8U)
#define USB_GRF_REG2_USBPHY_REG2_SHIFT                     (0U)
#define USB_GRF_REG2_USBPHY_REG2_MASK                      (0xFFFFU << USB_GRF_REG2_USBPHY_REG2_SHIFT)                  /* 0x0000FFFF */
/* REG3 */
#define USB_GRF_REG3_OFFSET                                (0xCU)
#define USB_GRF_REG3_USBPHY_REG3_SHIFT                     (0U)
#define USB_GRF_REG3_USBPHY_REG3_MASK                      (0xFFFFU << USB_GRF_REG3_USBPHY_REG3_SHIFT)                  /* 0x0000FFFF */
/* REG4 */
#define USB_GRF_REG4_OFFSET                                (0x10U)
#define USB_GRF_REG4_USBPHY_REG4_SHIFT                     (0U)
#define USB_GRF_REG4_USBPHY_REG4_MASK                      (0xFFFFU << USB_GRF_REG4_USBPHY_REG4_SHIFT)                  /* 0x0000FFFF */
/* REG5 */
#define USB_GRF_REG5_OFFSET                                (0x14U)
#define USB_GRF_REG5_USBPHY_REG5_SHIFT                     (0U)
#define USB_GRF_REG5_USBPHY_REG5_MASK                      (0xFFFFU << USB_GRF_REG5_USBPHY_REG5_SHIFT)                  /* 0x0000FFFF */
/* REG6 */
#define USB_GRF_REG6_OFFSET                                (0x18U)
#define USB_GRF_REG6_USBPHY_REG6_SHIFT                     (0U)
#define USB_GRF_REG6_USBPHY_REG6_MASK                      (0xFFFFU << USB_GRF_REG6_USBPHY_REG6_SHIFT)                  /* 0x0000FFFF */
/* REG7 */
#define USB_GRF_REG7_OFFSET                                (0x1CU)
#define USB_GRF_REG7_USBPHY_REG7_SHIFT                     (0U)
#define USB_GRF_REG7_USBPHY_REG7_MASK                      (0xFFFFU << USB_GRF_REG7_USBPHY_REG7_SHIFT)                  /* 0x0000FFFF */
/* REG8 */
#define USB_GRF_REG8_OFFSET                                (0x20U)
#define USB_GRF_REG8_USBPHY_REG8_SHIFT                     (0U)
#define USB_GRF_REG8_USBPHY_REG8_MASK                      (0xFFFFU << USB_GRF_REG8_USBPHY_REG8_SHIFT)                  /* 0x0000FFFF */
/* REG9 */
#define USB_GRF_REG9_OFFSET                                (0x24U)
#define USB_GRF_REG9_USBPHY_REG9_SHIFT                     (0U)
#define USB_GRF_REG9_USBPHY_REG9_MASK                      (0xFFFFU << USB_GRF_REG9_USBPHY_REG9_SHIFT)                  /* 0x0000FFFF */
/* REG10 */
#define USB_GRF_REG10_OFFSET                               (0x28U)
#define USB_GRF_REG10_USBPHY_REG10_SHIFT                   (0U)
#define USB_GRF_REG10_USBPHY_REG10_MASK                    (0xFFFFU << USB_GRF_REG10_USBPHY_REG10_SHIFT)                /* 0x0000FFFF */
/* REG11 */
#define USB_GRF_REG11_OFFSET                               (0x2CU)
#define USB_GRF_REG11_USBPHY_REG11_SHIFT                   (0U)
#define USB_GRF_REG11_USBPHY_REG11_MASK                    (0xFFFFU << USB_GRF_REG11_USBPHY_REG11_SHIFT)                /* 0x0000FFFF */
/* REG12 */
#define USB_GRF_REG12_OFFSET                               (0x30U)
#define USB_GRF_REG12_USBPHY_REG12_SHIFT                   (0U)
#define USB_GRF_REG12_USBPHY_REG12_MASK                    (0xFFFFU << USB_GRF_REG12_USBPHY_REG12_SHIFT)                /* 0x0000FFFF */
/* REG13 */
#define USB_GRF_REG13_OFFSET                               (0x34U)
#define USB_GRF_REG13_USBPHY_REG13_SHIFT                   (0U)
#define USB_GRF_REG13_USBPHY_REG13_MASK                    (0xFFFFU << USB_GRF_REG13_USBPHY_REG13_SHIFT)                /* 0x0000FFFF */
/* REG14 */
#define USB_GRF_REG14_OFFSET                               (0x38U)
#define USB_GRF_REG14_USBPHY_REG14_SHIFT                   (0U)
#define USB_GRF_REG14_USBPHY_REG14_MASK                    (0xFFFFU << USB_GRF_REG14_USBPHY_REG14_SHIFT)                /* 0x0000FFFF */
/* REG15 */
#define USB_GRF_REG15_OFFSET                               (0x3CU)
#define USB_GRF_REG15_USBPHY_REG15_SHIFT                   (0U)
#define USB_GRF_REG15_USBPHY_REG15_MASK                    (0xFFFFU << USB_GRF_REG15_USBPHY_REG15_SHIFT)                /* 0x0000FFFF */
/* REG16 */
#define USB_GRF_REG16_OFFSET                               (0x40U)
#define USB_GRF_REG16_USBPHY_REG16_SHIFT                   (0U)
#define USB_GRF_REG16_USBPHY_REG16_MASK                    (0xFFFFU << USB_GRF_REG16_USBPHY_REG16_SHIFT)                /* 0x0000FFFF */
/* REG17 */
#define USB_GRF_REG17_OFFSET                               (0x44U)
#define USB_GRF_REG17_USBPHY_REG17_SHIFT                   (0U)
#define USB_GRF_REG17_USBPHY_REG17_MASK                    (0xFFFFU << USB_GRF_REG17_USBPHY_REG17_SHIFT)                /* 0x0000FFFF */
/* REG18 */
#define USB_GRF_REG18_OFFSET                               (0x48U)
#define USB_GRF_REG18_USBPHY_REG18_SHIFT                   (0U)
#define USB_GRF_REG18_USBPHY_REG18_MASK                    (0xFFFFU << USB_GRF_REG18_USBPHY_REG18_SHIFT)                /* 0x0000FFFF */
/* REG19 */
#define USB_GRF_REG19_OFFSET                               (0x4CU)
#define USB_GRF_REG19_USBPHY_REG19_SHIFT                   (0U)
#define USB_GRF_REG19_USBPHY_REG19_MASK                    (0xFFFFU << USB_GRF_REG19_USBPHY_REG19_SHIFT)                /* 0x0000FFFF */
/* REG20 */
#define USB_GRF_REG20_OFFSET                               (0x50U)
#define USB_GRF_REG20_USBPHY_REG20_SHIFT                   (0U)
#define USB_GRF_REG20_USBPHY_REG20_MASK                    (0xFFFFU << USB_GRF_REG20_USBPHY_REG20_SHIFT)                /* 0x0000FFFF */
/* REG21 */
#define USB_GRF_REG21_OFFSET                               (0x54U)
#define USB_GRF_REG21_USBPHY_REG21_SHIFT                   (0U)
#define USB_GRF_REG21_USBPHY_REG21_MASK                    (0xFFFFU << USB_GRF_REG21_USBPHY_REG21_SHIFT)                /* 0x0000FFFF */
/* REG22 */
#define USB_GRF_REG22_OFFSET                               (0x58U)
#define USB_GRF_REG22_USBPHY_REG22_SHIFT                   (0U)
#define USB_GRF_REG22_USBPHY_REG22_MASK                    (0xFFFFU << USB_GRF_REG22_USBPHY_REG22_SHIFT)                /* 0x0000FFFF */
/* REG23 */
#define USB_GRF_REG23_OFFSET                               (0x5CU)
#define USB_GRF_REG23_USBPHY_REG23_SHIFT                   (0U)
#define USB_GRF_REG23_USBPHY_REG23_MASK                    (0xFFFFU << USB_GRF_REG23_USBPHY_REG23_SHIFT)                /* 0x0000FFFF */
/* CON0 */
#define USB_GRF_CON0_OFFSET                                (0x100U)
#define USB_GRF_CON0_USBOTG_UTMI_SEL_SHIFT                 (0U)
#define USB_GRF_CON0_USBOTG_UTMI_SEL_MASK                  (0x1U << USB_GRF_CON0_USBOTG_UTMI_SEL_SHIFT)                 /* 0x00000001 */
#define USB_GRF_CON0_USBOTG_UTMI_SUSPEND_N_SHIFT           (1U)
#define USB_GRF_CON0_USBOTG_UTMI_SUSPEND_N_MASK            (0x1U << USB_GRF_CON0_USBOTG_UTMI_SUSPEND_N_SHIFT)           /* 0x00000002 */
#define USB_GRF_CON0_USBOTG_UTMI_OPMODE_SHIFT              (2U)
#define USB_GRF_CON0_USBOTG_UTMI_OPMODE_MASK               (0x3U << USB_GRF_CON0_USBOTG_UTMI_OPMODE_SHIFT)              /* 0x0000000C */
#define USB_GRF_CON0_USBOTG_UTMI_XCVRSELECT_SHIFT          (4U)
#define USB_GRF_CON0_USBOTG_UTMI_XCVRSELECT_MASK           (0x3U << USB_GRF_CON0_USBOTG_UTMI_XCVRSELECT_SHIFT)          /* 0x00000030 */
#define USB_GRF_CON0_USBOTG_UTMI_TERMSELECT_SHIFT          (6U)
#define USB_GRF_CON0_USBOTG_UTMI_TERMSELECT_MASK           (0x1U << USB_GRF_CON0_USBOTG_UTMI_TERMSELECT_SHIFT)          /* 0x00000040 */
#define USB_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_SHIFT          (7U)
#define USB_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_MASK           (0x1U << USB_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_SHIFT)          /* 0x00000080 */
#define USB_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_SHIFT          (8U)
#define USB_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_MASK           (0x1U << USB_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_SHIFT)          /* 0x00000100 */
#define USB_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_SHIFT           (9U)
#define USB_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_MASK            (0x1U << USB_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_SHIFT)           /* 0x00000200 */
#define USB_GRF_CON0_USBOTG_UTMI_IDDIG_SHIFT               (10U)
#define USB_GRF_CON0_USBOTG_UTMI_IDDIG_MASK                (0x1U << USB_GRF_CON0_USBOTG_UTMI_IDDIG_SHIFT)               /* 0x00000400 */
/* CON1 */
#define USB_GRF_CON1_OFFSET                                (0x104U)
#define USB_GRF_CON1_USBHOST_UTMI_SEL_SHIFT                (0U)
#define USB_GRF_CON1_USBHOST_UTMI_SEL_MASK                 (0x1U << USB_GRF_CON1_USBHOST_UTMI_SEL_SHIFT)                /* 0x00000001 */
#define USB_GRF_CON1_USBHOST_UTMI_SUSPEND_N_SHIFT          (1U)
#define USB_GRF_CON1_USBHOST_UTMI_SUSPEND_N_MASK           (0x1U << USB_GRF_CON1_USBHOST_UTMI_SUSPEND_N_SHIFT)          /* 0x00000002 */
#define USB_GRF_CON1_USBHOST_UTMI_OPMODE_SHIFT             (2U)
#define USB_GRF_CON1_USBHOST_UTMI_OPMODE_MASK              (0x3U << USB_GRF_CON1_USBHOST_UTMI_OPMODE_SHIFT)             /* 0x0000000C */
#define USB_GRF_CON1_USBHOST_UTMI_XCVRSELECT_SHIFT         (4U)
#define USB_GRF_CON1_USBHOST_UTMI_XCVRSELECT_MASK          (0x3U << USB_GRF_CON1_USBHOST_UTMI_XCVRSELECT_SHIFT)         /* 0x00000030 */
#define USB_GRF_CON1_USBHOST_UTMI_TERMSELECT_SHIFT         (6U)
#define USB_GRF_CON1_USBHOST_UTMI_TERMSELECT_MASK          (0x1U << USB_GRF_CON1_USBHOST_UTMI_TERMSELECT_SHIFT)         /* 0x00000040 */
#define USB_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_SHIFT         (7U)
#define USB_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_MASK          (0x1U << USB_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_SHIFT)         /* 0x00000080 */
#define USB_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_SHIFT         (8U)
#define USB_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_MASK          (0x1U << USB_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_SHIFT)         /* 0x00000100 */
/* CON2 */
#define USB_GRF_CON2_OFFSET                                (0x108U)
#define USB_GRF_CON2_USBOTG_DISABLE_0_SHIFT                (0U)
#define USB_GRF_CON2_USBOTG_DISABLE_0_MASK                 (0x1U << USB_GRF_CON2_USBOTG_DISABLE_0_SHIFT)                /* 0x00000001 */
#define USB_GRF_CON2_USBOTG_DISABLE_1_SHIFT                (1U)
#define USB_GRF_CON2_USBOTG_DISABLE_1_MASK                 (0x1U << USB_GRF_CON2_USBOTG_DISABLE_1_SHIFT)                /* 0x00000002 */
#define USB_GRF_CON2_BYPASSDMEN_USBOTG_SHIFT               (2U)
#define USB_GRF_CON2_BYPASSDMEN_USBOTG_MASK                (0x1U << USB_GRF_CON2_BYPASSDMEN_USBOTG_SHIFT)               /* 0x00000004 */
#define USB_GRF_CON2_BYPASSSEL_USBOTG_SHIFT                (3U)
#define USB_GRF_CON2_BYPASSSEL_USBOTG_MASK                 (0x1U << USB_GRF_CON2_BYPASSSEL_USBOTG_SHIFT)                /* 0x00000008 */
#define USB_GRF_CON2_USBPHY_COMMONONN_SHIFT                (4U)
#define USB_GRF_CON2_USBPHY_COMMONONN_MASK                 (0x1U << USB_GRF_CON2_USBPHY_COMMONONN_SHIFT)                /* 0x00000010 */
#define USB_GRF_CON2_IDP_SINK_EN_USBOTG_SHIFT              (7U)
#define USB_GRF_CON2_IDP_SINK_EN_USBOTG_MASK               (0x1U << USB_GRF_CON2_IDP_SINK_EN_USBOTG_SHIFT)              /* 0x00000080 */
#define USB_GRF_CON2_IDM_SINK_EN_USBOTG_SHIFT              (8U)
#define USB_GRF_CON2_IDM_SINK_EN_USBOTG_MASK               (0x1U << USB_GRF_CON2_IDM_SINK_EN_USBOTG_SHIFT)              /* 0x00000100 */
#define USB_GRF_CON2_IDP_SRC_EN_USBOTG_SHIFT               (9U)
#define USB_GRF_CON2_IDP_SRC_EN_USBOTG_MASK                (0x1U << USB_GRF_CON2_IDP_SRC_EN_USBOTG_SHIFT)               /* 0x00000200 */
#define USB_GRF_CON2_RDM_PDWN_EN_USBOTG_SHIFT              (10U)
#define USB_GRF_CON2_RDM_PDWN_EN_USBOTG_MASK               (0x1U << USB_GRF_CON2_RDM_PDWN_EN_USBOTG_SHIFT)              /* 0x00000400 */
#define USB_GRF_CON2_VDP_SRC_EN_USBOTG_SHIFT               (11U)
#define USB_GRF_CON2_VDP_SRC_EN_USBOTG_MASK                (0x1U << USB_GRF_CON2_VDP_SRC_EN_USBOTG_SHIFT)               /* 0x00000800 */
#define USB_GRF_CON2_VDM_SRC_EN_USBOTG_SHIFT               (12U)
#define USB_GRF_CON2_VDM_SRC_EN_USBOTG_MASK                (0x1U << USB_GRF_CON2_VDM_SRC_EN_USBOTG_SHIFT)               /* 0x00001000 */
/* CON3 */
#define USB_GRF_CON3_OFFSET                                (0x10CU)
#define USB_GRF_CON3_USBHOST_UTMI_DRVVBUS_SHIFT            (0U)
#define USB_GRF_CON3_USBHOST_UTMI_DRVVBUS_MASK             (0x1U << USB_GRF_CON3_USBHOST_UTMI_DRVVBUS_SHIFT)            /* 0x00000001 */
#define USB_GRF_CON3_USBHOST_UTMI_CHRGVBUS_SHIFT           (1U)
#define USB_GRF_CON3_USBHOST_UTMI_CHRGVBUS_MASK            (0x1U << USB_GRF_CON3_USBHOST_UTMI_CHRGVBUS_SHIFT)           /* 0x00000002 */
#define USB_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_SHIFT        (2U)
#define USB_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_MASK         (0x1U << USB_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_SHIFT)        /* 0x00000004 */
#define USB_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_SHIFT         (3U)
#define USB_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_MASK          (0x1U << USB_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_SHIFT)         /* 0x00000008 */
#define USB_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_SHIFT         (4U)
#define USB_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_MASK          (0x1U << USB_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_SHIFT)         /* 0x00000010 */
#define USB_GRF_CON3_USBHOST_UTMI_IDPULLUP_SHIFT           (5U)
#define USB_GRF_CON3_USBHOST_UTMI_IDPULLUP_MASK            (0x1U << USB_GRF_CON3_USBHOST_UTMI_IDPULLUP_SHIFT)           /* 0x00000020 */
#define USB_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_SHIFT         (6U)
#define USB_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_MASK          (0x1U << USB_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_SHIFT)         /* 0x00000040 */
#define USB_GRF_CON3_USBOTG_UTMI_FS_OE_SHIFT               (7U)
#define USB_GRF_CON3_USBOTG_UTMI_FS_OE_MASK                (0x1U << USB_GRF_CON3_USBOTG_UTMI_FS_OE_SHIFT)               /* 0x00000080 */
#define USB_GRF_CON3_USBOTG_UTMI_FS_DATA_SHIFT             (8U)
#define USB_GRF_CON3_USBOTG_UTMI_FS_DATA_MASK              (0x1U << USB_GRF_CON3_USBOTG_UTMI_FS_DATA_SHIFT)             /* 0x00000100 */
#define USB_GRF_CON3_USBOTG_UTMI_FS_SE0_SHIFT              (9U)
#define USB_GRF_CON3_USBOTG_UTMI_FS_SE0_MASK               (0x1U << USB_GRF_CON3_USBOTG_UTMI_FS_SE0_SHIFT)              /* 0x00000200 */
#define USB_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_SHIFT         (10U)
#define USB_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_MASK          (0x1U << USB_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_SHIFT)         /* 0x00000400 */
#define USB_GRF_CON3_USBOTG_UTMI_DRVVBUS_SHIFT             (11U)
#define USB_GRF_CON3_USBOTG_UTMI_DRVVBUS_MASK              (0x1U << USB_GRF_CON3_USBOTG_UTMI_DRVVBUS_SHIFT)             /* 0x00000800 */
/* USB2_DETECT_IRQ_ENABLE */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OFFSET              (0x110U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_LINESTATE_IRQ_EN_SHIFT (0U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_LINESTATE_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_LINESTATE_IRQ_EN_SHIFT) /* 0x00000001 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_LINESTATE_IRQ_EN_SHIFT (1U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_LINESTATE_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_LINESTATE_IRQ_EN_SHIFT) /* 0x00000002 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_BVALID_POS_IRQ_EN_SHIFT (2U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_BVALID_POS_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_BVALID_POS_IRQ_EN_SHIFT) /* 0x00000004 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_BVALID_NEG_IRQ_EN_SHIFT (3U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_BVALID_NEG_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_BVALID_NEG_IRQ_EN_SHIFT) /* 0x00000008 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_ID_POS_IRQ_EN_SHIFT (4U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_ID_POS_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_ID_POS_IRQ_EN_SHIFT) /* 0x00000010 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_ID_NEG_IRQ_EN_SHIFT (5U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_ID_NEG_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_ID_NEG_IRQ_EN_SHIFT) /* 0x00000020 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_DISCONNECT_POS_IRQ_EN_SHIFT (6U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_DISCONNECT_POS_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_DISCONNECT_POS_IRQ_EN_SHIFT) /* 0x00000040 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_DISCONNECT_NEG_IRQ_EN_SHIFT (7U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_DISCONNECT_NEG_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_OTG0_DISCONNECT_NEG_IRQ_EN_SHIFT) /* 0x00000080 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_DISCONNECT_POS_IRQ_EN_SHIFT (8U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_DISCONNECT_POS_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_DISCONNECT_POS_IRQ_EN_SHIFT) /* 0x00000100 */
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_DISCONNECT_NEG_IRQ_EN_SHIFT (9U)
#define USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_DISCONNECT_NEG_IRQ_EN_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_ENABLE_HOST0_DISCONNECT_NEG_IRQ_EN_SHIFT) /* 0x00000200 */
/* USB2_DETECT_IRQ_STATUS */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OFFSET              (0x114U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS                     (0x0U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_LINESTATE_IRQ_SHIFT (0U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_LINESTATE_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_LINESTATE_IRQ_SHIFT) /* 0x00000001 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_LINESTATE_IRQ_SHIFT (1U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_LINESTATE_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_LINESTATE_IRQ_SHIFT) /* 0x00000002 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_BVALID_POS_IRQ_SHIFT (2U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_BVALID_POS_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_BVALID_POS_IRQ_SHIFT) /* 0x00000004 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_BVALID_NEG_IRQ_SHIFT (3U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_BVALID_NEG_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_BVALID_NEG_IRQ_SHIFT) /* 0x00000008 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_ID_POS_IRQ_SHIFT (4U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_ID_POS_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_ID_POS_IRQ_SHIFT) /* 0x00000010 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_ID_NEG_IRQ_SHIFT (5U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_ID_NEG_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_ID_NEG_IRQ_SHIFT) /* 0x00000020 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_DISCONNECT_POS_IRQ_SHIFT (6U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_DISCONNECT_POS_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_DISCONNECT_POS_IRQ_SHIFT) /* 0x00000040 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_DISCONNECT_NEG_IRQ_SHIFT (7U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_DISCONNECT_NEG_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_OTG0_DISCONNECT_NEG_IRQ_SHIFT) /* 0x00000080 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_DISCONNECT_POS_IRQ_SHIFT (8U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_DISCONNECT_POS_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_DISCONNECT_POS_IRQ_SHIFT) /* 0x00000100 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_DISCONNECT_NEG_IRQ_SHIFT (9U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_DISCONNECT_NEG_IRQ_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_HOST0_DISCONNECT_NEG_IRQ_SHIFT) /* 0x00000200 */
/* USB2_DETECT_IRQ_STATUS_CLR */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OFFSET          (0x118U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_SHIFT (0U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_SHIFT) /* 0x00000001 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_SHIFT (1U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_SHIFT) /* 0x00000002 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_BVALID_POS_IRQ_CLR_SHIFT (2U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_BVALID_POS_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_BVALID_POS_IRQ_CLR_SHIFT) /* 0x00000004 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_BVALID_NEG_IRQ_CLR_SHIFT (3U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_BVALID_NEG_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_BVALID_NEG_IRQ_CLR_SHIFT) /* 0x00000008 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_ID_POS_IRQ_CLR_SHIFT (4U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_ID_POS_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_ID_POS_IRQ_CLR_SHIFT) /* 0x00000010 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_ID_NEG_IRQ_CLR_SHIFT (5U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_ID_NEG_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_ID_NEG_IRQ_CLR_SHIFT) /* 0x00000020 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_DISCONNECT_POS_IRQ_CLR_SHIFT (6U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_DISCONNECT_POS_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_DISCONNECT_POS_IRQ_CLR_SHIFT) /* 0x00000040 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_DISCONNECT_NEG_IRQ_CLR_SHIFT (7U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_DISCONNECT_NEG_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_OTG0_DISCONNECT_NEG_IRQ_CLR_SHIFT) /* 0x00000080 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_DISCONNECT_POS_IRQ_CLR_SHIFT (8U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_DISCONNECT_POS_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_DISCONNECT_POS_IRQ_CLR_SHIFT) /* 0x00000100 */
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_DISCONNECT_NEG_IRQ_CLR_SHIFT (9U)
#define USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_DISCONNECT_NEG_IRQ_CLR_MASK (0x1U << USB_GRF_USB2_DETECT_IRQ_STATUS_CLR_HOST0_DISCONNECT_NEG_IRQ_CLR_SHIFT) /* 0x00000200 */
/* STATUS */
#define USB_GRF_STATUS_OFFSET                              (0x120U)
#define USB_GRF_STATUS                                     (0x0U)
#define USB_GRF_STATUS_USBOTG_UTMI_VPI_SHIFT               (0U)
#define USB_GRF_STATUS_USBOTG_UTMI_VPI_MASK                (0x1U << USB_GRF_STATUS_USBOTG_UTMI_VPI_SHIFT)               /* 0x00000001 */
#define USB_GRF_STATUS_USBOTG_UTMI_VMI_SHIFT               (1U)
#define USB_GRF_STATUS_USBOTG_UTMI_VMI_MASK                (0x1U << USB_GRF_STATUS_USBOTG_UTMI_VMI_SHIFT)               /* 0x00000002 */
#define USB_GRF_STATUS_USBOTG_UTMI_VBUSVALID_SHIFT         (2U)
#define USB_GRF_STATUS_USBOTG_UTMI_VBUSVALID_MASK          (0x1U << USB_GRF_STATUS_USBOTG_UTMI_VBUSVALID_SHIFT)         /* 0x00000004 */
#define USB_GRF_STATUS_USBOTG_UTMI_SESSEND_SHIFT           (3U)
#define USB_GRF_STATUS_USBOTG_UTMI_SESSEND_MASK            (0x1U << USB_GRF_STATUS_USBOTG_UTMI_SESSEND_SHIFT)           /* 0x00000008 */
#define USB_GRF_STATUS_USBOTG_UTMI_LINESTATE_SHIFT         (4U)
#define USB_GRF_STATUS_USBOTG_UTMI_LINESTATE_MASK          (0x3U << USB_GRF_STATUS_USBOTG_UTMI_LINESTATE_SHIFT)         /* 0x00000030 */
#define USB_GRF_STATUS_USBOTG_UTMI_IDDIG_SHIFT             (6U)
#define USB_GRF_STATUS_USBOTG_UTMI_IDDIG_MASK              (0x1U << USB_GRF_STATUS_USBOTG_UTMI_IDDIG_SHIFT)             /* 0x00000040 */
#define USB_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_SHIFT    (7U)
#define USB_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_MASK     (0x1U << USB_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_SHIFT)    /* 0x00000080 */
#define USB_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_SHIFT       (8U)
#define USB_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_MASK        (0x1U << USB_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_SHIFT)       /* 0x00000100 */
#define USB_GRF_STATUS_USBOTG_UTMI_BVALID_SHIFT            (9U)
#define USB_GRF_STATUS_USBOTG_UTMI_BVALID_MASK             (0x1U << USB_GRF_STATUS_USBOTG_UTMI_BVALID_SHIFT)            /* 0x00000200 */
#define USB_GRF_STATUS_USBOTG_UTMI_AVALID_SHIFT            (10U)
#define USB_GRF_STATUS_USBOTG_UTMI_AVALID_MASK             (0x1U << USB_GRF_STATUS_USBOTG_UTMI_AVALID_SHIFT)            /* 0x00000400 */
#define USB_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_SHIFT          (11U)
#define USB_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_MASK           (0x1U << USB_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_SHIFT)          /* 0x00000800 */
#define USB_GRF_STATUS_USBHOST_UTMI_VPI_SHIFT              (12U)
#define USB_GRF_STATUS_USBHOST_UTMI_VPI_MASK               (0x1U << USB_GRF_STATUS_USBHOST_UTMI_VPI_SHIFT)              /* 0x00001000 */
#define USB_GRF_STATUS_USBHOST_UTMI_VMI_SHIFT              (13U)
#define USB_GRF_STATUS_USBHOST_UTMI_VMI_MASK               (0x1U << USB_GRF_STATUS_USBHOST_UTMI_VMI_SHIFT)              /* 0x00002000 */
#define USB_GRF_STATUS_USBHOST_UTMI_VBUSVALID_SHIFT        (14U)
#define USB_GRF_STATUS_USBHOST_UTMI_VBUSVALID_MASK         (0x1U << USB_GRF_STATUS_USBHOST_UTMI_VBUSVALID_SHIFT)        /* 0x00004000 */
#define USB_GRF_STATUS_USBHOST_UTMI_SESSEND_SHIFT          (15U)
#define USB_GRF_STATUS_USBHOST_UTMI_SESSEND_MASK           (0x1U << USB_GRF_STATUS_USBHOST_UTMI_SESSEND_SHIFT)          /* 0x00008000 */
#define USB_GRF_STATUS_USBHOST_UTMI_LINESTATE_SHIFT        (16U)
#define USB_GRF_STATUS_USBHOST_UTMI_LINESTATE_MASK         (0x3U << USB_GRF_STATUS_USBHOST_UTMI_LINESTATE_SHIFT)        /* 0x00030000 */
#define USB_GRF_STATUS_USBHOST_UTMI_IDDIG_O_SHIFT          (18U)
#define USB_GRF_STATUS_USBHOST_UTMI_IDDIG_O_MASK           (0x1U << USB_GRF_STATUS_USBHOST_UTMI_IDDIG_O_SHIFT)          /* 0x00040000 */
#define USB_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_SHIFT   (19U)
#define USB_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_MASK    (0x1U << USB_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_SHIFT)   /* 0x00080000 */
#define USB_GRF_STATUS_USBHOST_UTMI_BVALID_SHIFT           (20U)
#define USB_GRF_STATUS_USBHOST_UTMI_BVALID_MASK            (0x1U << USB_GRF_STATUS_USBHOST_UTMI_BVALID_SHIFT)           /* 0x00100000 */
#define USB_GRF_STATUS_USBHOST_UTMI_AVALID_SHIFT           (21U)
#define USB_GRF_STATUS_USBHOST_UTMI_AVALID_MASK            (0x1U << USB_GRF_STATUS_USBHOST_UTMI_AVALID_SHIFT)           /* 0x00200000 */
#define USB_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_SHIFT         (22U)
#define USB_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_MASK          (0x1U << USB_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_SHIFT)         /* 0x00400000 */
#define USB_GRF_STATUS_GRF_STAT_USBPHY_DCP_DETECTED_SHIFT  (23U)
#define USB_GRF_STATUS_GRF_STAT_USBPHY_DCP_DETECTED_MASK   (0x1U << USB_GRF_STATUS_GRF_STAT_USBPHY_DCP_DETECTED_SHIFT)  /* 0x00800000 */
#define USB_GRF_STATUS_GRF_STAT_USBPHY_CP_DETECTED_SHIFT   (24U)
#define USB_GRF_STATUS_GRF_STAT_USBPHY_CP_DETECTED_MASK    (0x1U << USB_GRF_STATUS_GRF_STAT_USBPHY_CP_DETECTED_SHIFT)   /* 0x01000000 */
#define USB_GRF_STATUS_GRF_STAT_USBPHY_DP_DETECTED_SHIFT   (25U)
#define USB_GRF_STATUS_GRF_STAT_USBPHY_DP_DETECTED_MASK    (0x1U << USB_GRF_STATUS_GRF_STAT_USBPHY_DP_DETECTED_SHIFT)   /* 0x02000000 */
/* USB2_LINESTATE_CON */
#define USB_GRF_USB2_LINESTATE_CON_OFFSET                  (0x130U)
#define USB_GRF_USB2_LINESTATE_CON_LINESTATE_FILTER_CON_SHIFT (0U)
#define USB_GRF_USB2_LINESTATE_CON_LINESTATE_FILTER_CON_MASK (0xFFFFFFFFU << USB_GRF_USB2_LINESTATE_CON_LINESTATE_FILTER_CON_SHIFT) /* 0xFFFFFFFF */
/* USB2_DISCONNECT_CON */
#define USB_GRF_USB2_DISCONNECT_CON_OFFSET                 (0x134U)
#define USB_GRF_USB2_DISCONNECT_CON_DISCONNECT_FILTER_CON_SHIFT (0U)
#define USB_GRF_USB2_DISCONNECT_CON_DISCONNECT_FILTER_CON_MASK (0xFFFFFFFFU << USB_GRF_USB2_DISCONNECT_CON_DISCONNECT_FILTER_CON_SHIFT) /* 0xFFFFFFFF */
/* USB2_BVALID_CON */
#define USB_GRF_USB2_BVALID_CON_OFFSET                     (0x138U)
#define USB_GRF_USB2_BVALID_CON_BVALID_FILTER_CON_SHIFT    (0U)
#define USB_GRF_USB2_BVALID_CON_BVALID_FILTER_CON_MASK     (0xFFFFFFFFU << USB_GRF_USB2_BVALID_CON_BVALID_FILTER_CON_SHIFT) /* 0xFFFFFFFF */
/* USB2_ID_CON */
#define USB_GRF_USB2_ID_CON_OFFSET                         (0x13CU)
#define USB_GRF_USB2_ID_CON_ID_FILTER_CON_SHIFT            (0U)
#define USB_GRF_USB2_ID_CON_ID_FILTER_CON_MASK             (0xFFFFFFFFU << USB_GRF_USB2_ID_CON_ID_FILTER_CON_SHIFT)     /* 0xFFFFFFFF */
/******************************************GPIO******************************************/
/* SWPORT_DR_L */
#define GPIO_SWPORT_DR_L_OFFSET                            (0x0U)
#define GPIO_SWPORT_DR_L_SWPORT_DR_LOW_SHIFT               (0U)
#define GPIO_SWPORT_DR_L_SWPORT_DR_LOW_MASK                (0xFFFFU << GPIO_SWPORT_DR_L_SWPORT_DR_LOW_SHIFT)            /* 0x0000FFFF */
/* SWPORT_DR_H */
#define GPIO_SWPORT_DR_H_OFFSET                            (0x4U)
#define GPIO_SWPORT_DR_H_SWPORT_DR_HIGH_SHIFT              (0U)
#define GPIO_SWPORT_DR_H_SWPORT_DR_HIGH_MASK               (0xFFFFU << GPIO_SWPORT_DR_H_SWPORT_DR_HIGH_SHIFT)           /* 0x0000FFFF */
/* SWPORT_DDR_L */
#define GPIO_SWPORT_DDR_L_OFFSET                           (0x8U)
#define GPIO_SWPORT_DDR_L_SWPORT_DDR_LOW_SHIFT             (0U)
#define GPIO_SWPORT_DDR_L_SWPORT_DDR_LOW_MASK              (0xFFFFU << GPIO_SWPORT_DDR_L_SWPORT_DDR_LOW_SHIFT)          /* 0x0000FFFF */
/* SWPORT_DDR_H */
#define GPIO_SWPORT_DDR_H_OFFSET                           (0xCU)
#define GPIO_SWPORT_DDR_H_SWPORT_DDR_HIGH_SHIFT            (0U)
#define GPIO_SWPORT_DDR_H_SWPORT_DDR_HIGH_MASK             (0xFFFFU << GPIO_SWPORT_DDR_H_SWPORT_DDR_HIGH_SHIFT)         /* 0x0000FFFF */
/* INT_EN_L */
#define GPIO_INT_EN_L_OFFSET                               (0x10U)
#define GPIO_INT_EN_L_INT_EN_LOW_SHIFT                     (0U)
#define GPIO_INT_EN_L_INT_EN_LOW_MASK                      (0xFFFFU << GPIO_INT_EN_L_INT_EN_LOW_SHIFT)                  /* 0x0000FFFF */
/* INT_EN_H */
#define GPIO_INT_EN_H_OFFSET                               (0x14U)
#define GPIO_INT_EN_H_INT_EN_HIGH_SHIFT                    (0U)
#define GPIO_INT_EN_H_INT_EN_HIGH_MASK                     (0xFFFFU << GPIO_INT_EN_H_INT_EN_HIGH_SHIFT)                 /* 0x0000FFFF */
/* INT_MASK_L */
#define GPIO_INT_MASK_L_OFFSET                             (0x18U)
#define GPIO_INT_MASK_L_INT_MASK_LOW_SHIFT                 (0U)
#define GPIO_INT_MASK_L_INT_MASK_LOW_MASK                  (0xFFFFU << GPIO_INT_MASK_L_INT_MASK_LOW_SHIFT)              /* 0x0000FFFF */
/* INT_MASK_H */
#define GPIO_INT_MASK_H_OFFSET                             (0x1CU)
#define GPIO_INT_MASK_H_INT_MASK_HIGH_SHIFT                (0U)
#define GPIO_INT_MASK_H_INT_MASK_HIGH_MASK                 (0xFFFFU << GPIO_INT_MASK_H_INT_MASK_HIGH_SHIFT)             /* 0x0000FFFF */
/* INT_TYPE_L */
#define GPIO_INT_TYPE_L_OFFSET                             (0x20U)
#define GPIO_INT_TYPE_L_INT_TYPE_LOW_SHIFT                 (0U)
#define GPIO_INT_TYPE_L_INT_TYPE_LOW_MASK                  (0xFFFFU << GPIO_INT_TYPE_L_INT_TYPE_LOW_SHIFT)              /* 0x0000FFFF */
/* INT_TYPE_H */
#define GPIO_INT_TYPE_H_OFFSET                             (0x24U)
#define GPIO_INT_TYPE_H_INT_TYPE_HIGH_SHIFT                (0U)
#define GPIO_INT_TYPE_H_INT_TYPE_HIGH_MASK                 (0xFFFFU << GPIO_INT_TYPE_H_INT_TYPE_HIGH_SHIFT)             /* 0x0000FFFF */
/* INT_POLARITY_L */
#define GPIO_INT_POLARITY_L_OFFSET                         (0x28U)
#define GPIO_INT_POLARITY_L_INT_POLARITY_LOW_SHIFT         (0U)
#define GPIO_INT_POLARITY_L_INT_POLARITY_LOW_MASK          (0xFFFFU << GPIO_INT_POLARITY_L_INT_POLARITY_LOW_SHIFT)      /* 0x0000FFFF */
/* INT_POLARITY_H */
#define GPIO_INT_POLARITY_H_OFFSET                         (0x2CU)
#define GPIO_INT_POLARITY_H_INT_POLARITY_HIGH_SHIFT        (0U)
#define GPIO_INT_POLARITY_H_INT_POLARITY_HIGH_MASK         (0xFFFFU << GPIO_INT_POLARITY_H_INT_POLARITY_HIGH_SHIFT)     /* 0x0000FFFF */
/* INT_BOTHEDGE_L */
#define GPIO_INT_BOTHEDGE_L_OFFSET                         (0x30U)
#define GPIO_INT_BOTHEDGE_L_INT_BOTHEDGE_LOW_SHIFT         (0U)
#define GPIO_INT_BOTHEDGE_L_INT_BOTHEDGE_LOW_MASK          (0xFFFFU << GPIO_INT_BOTHEDGE_L_INT_BOTHEDGE_LOW_SHIFT)      /* 0x0000FFFF */
/* INT_BOTHEDGE_H */
#define GPIO_INT_BOTHEDGE_H_OFFSET                         (0x34U)
#define GPIO_INT_BOTHEDGE_H_INT_BOTHEDGE_HIGH_SHIFT        (0U)
#define GPIO_INT_BOTHEDGE_H_INT_BOTHEDGE_HIGH_MASK         (0xFFFFU << GPIO_INT_BOTHEDGE_H_INT_BOTHEDGE_HIGH_SHIFT)     /* 0x0000FFFF */
/* DEBOUNCE_L */
#define GPIO_DEBOUNCE_L_OFFSET                             (0x38U)
#define GPIO_DEBOUNCE_L_DEBOUNCE_LOW_SHIFT                 (0U)
#define GPIO_DEBOUNCE_L_DEBOUNCE_LOW_MASK                  (0xFFFFU << GPIO_DEBOUNCE_L_DEBOUNCE_LOW_SHIFT)              /* 0x0000FFFF */
/* DEBOUNCE_H */
#define GPIO_DEBOUNCE_H_OFFSET                             (0x3CU)
#define GPIO_DEBOUNCE_H_DEBOUNCE_HIGH_SHIFT                (0U)
#define GPIO_DEBOUNCE_H_DEBOUNCE_HIGH_MASK                 (0xFFFFU << GPIO_DEBOUNCE_H_DEBOUNCE_HIGH_SHIFT)             /* 0x0000FFFF */
/* DBCLK_DIV_EN_L */
#define GPIO_DBCLK_DIV_EN_L_OFFSET                         (0x40U)
#define GPIO_DBCLK_DIV_EN_L_DBCLK_DIV_EN_LOW_SHIFT         (0U)
#define GPIO_DBCLK_DIV_EN_L_DBCLK_DIV_EN_LOW_MASK          (0xFFFFU << GPIO_DBCLK_DIV_EN_L_DBCLK_DIV_EN_LOW_SHIFT)      /* 0x0000FFFF */
/* DBCLK_DIV_EN_H */
#define GPIO_DBCLK_DIV_EN_H_OFFSET                         (0x44U)
#define GPIO_DBCLK_DIV_EN_H_DBCLK_DIV_EN_HIGH_SHIFT        (0U)
#define GPIO_DBCLK_DIV_EN_H_DBCLK_DIV_EN_HIGH_MASK         (0xFFFFU << GPIO_DBCLK_DIV_EN_H_DBCLK_DIV_EN_HIGH_SHIFT)     /* 0x0000FFFF */
/* DBCLK_DIV_CON */
#define GPIO_DBCLK_DIV_CON_OFFSET                          (0x48U)
#define GPIO_DBCLK_DIV_CON_DBCLK_DIV_CON_SHIFT             (0U)
#define GPIO_DBCLK_DIV_CON_DBCLK_DIV_CON_MASK              (0xFFFFFFU << GPIO_DBCLK_DIV_CON_DBCLK_DIV_CON_SHIFT)        /* 0x00FFFFFF */
/* INT_STATUS */
#define GPIO_INT_STATUS_OFFSET                             (0x50U)
#define GPIO_INT_STATUS                                    (0x0U)
#define GPIO_INT_STATUS_INT_STATUS_SHIFT                   (0U)
#define GPIO_INT_STATUS_INT_STATUS_MASK                    (0xFFFFFFFFU << GPIO_INT_STATUS_INT_STATUS_SHIFT)            /* 0xFFFFFFFF */
/* INT_RAWSTATUS */
#define GPIO_INT_RAWSTATUS_OFFSET                          (0x58U)
#define GPIO_INT_RAWSTATUS                                 (0x0U)
#define GPIO_INT_RAWSTATUS_INT_RAWSTATUS_SHIFT             (0U)
#define GPIO_INT_RAWSTATUS_INT_RAWSTATUS_MASK              (0xFFFFFFFFU << GPIO_INT_RAWSTATUS_INT_RAWSTATUS_SHIFT)      /* 0xFFFFFFFF */
/* PORT_EOI_L */
#define GPIO_PORT_EOI_L_OFFSET                             (0x60U)
#define GPIO_PORT_EOI_L_PORT_EOI_LOW_SHIFT                 (0U)
#define GPIO_PORT_EOI_L_PORT_EOI_LOW_MASK                  (0xFFFFU << GPIO_PORT_EOI_L_PORT_EOI_LOW_SHIFT)              /* 0x0000FFFF */
/* PORT_EOI_H */
#define GPIO_PORT_EOI_H_OFFSET                             (0x64U)
#define GPIO_PORT_EOI_H_PORT_EOI_HIGH_SHIFT                (0U)
#define GPIO_PORT_EOI_H_PORT_EOI_HIGH_MASK                 (0xFFFFU << GPIO_PORT_EOI_H_PORT_EOI_HIGH_SHIFT)             /* 0x0000FFFF */
/* EXT_PORT */
#define GPIO_EXT_PORT_OFFSET                               (0x70U)
#define GPIO_EXT_PORT                                      (0x0U)
#define GPIO_EXT_PORT_EXT_PORT_SHIFT                       (0U)
#define GPIO_EXT_PORT_EXT_PORT_MASK                        (0xFFFFFFFFU << GPIO_EXT_PORT_EXT_PORT_SHIFT)                /* 0xFFFFFFFF */
/* VER_ID */
#define GPIO_VER_ID_OFFSET                                 (0x78U)
#define GPIO_VER_ID                                        (0x1000C2BU)
#define GPIO_VER_ID_VER_ID_SHIFT                           (0U)
#define GPIO_VER_ID_VER_ID_MASK                            (0xFFFFFFFFU << GPIO_VER_ID_VER_ID_SHIFT)                    /* 0xFFFFFFFF */
/*****************************************CACHE******************************************/
/* CACHE_CTRL */
#define ICACHE_CACHE_CTRL_OFFSET                           (0x0U)
#define ICACHE_CACHE_CTRL_CACHE_EN_SHIFT                   (0U)
#define ICACHE_CACHE_CTRL_CACHE_EN_MASK                    (0x1U << ICACHE_CACHE_CTRL_CACHE_EN_SHIFT)                   /* 0x00000001 */
#define ICACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT                (1U)
#define ICACHE_CACHE_CTRL_CACHE_WT_EN_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT)                /* 0x00000002 */
#define ICACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT                (2U)
#define ICACHE_CACHE_CTRL_CACHE_HB_EN_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT)                /* 0x00000004 */
#define ICACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT               (3U)
#define ICACHE_CACHE_CTRL_CACHE_STB_EN_MASK                (0x1U << ICACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT)               /* 0x00000008 */
#define ICACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT                (4U)
#define ICACHE_CACHE_CTRL_CACHE_FLUSH_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT)                /* 0x00000010 */
#define ICACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT               (5U)
#define ICACHE_CACHE_CTRL_CACHE_PMU_EN_MASK                (0x1U << ICACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT)               /* 0x00000020 */
#define ICACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT               (6U)
#define ICACHE_CACHE_CTRL_CACHE_BYPASS_MASK                (0x1U << ICACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT)               /* 0x00000040 */
#define ICACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT             (7U)
#define ICACHE_CACHE_CTRL_STB_TIMEOUT_EN_MASK              (0x1U << ICACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT)             /* 0x00000080 */
#define ICACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT         (8U)
#define ICACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_MASK          (0x7U << ICACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT)         /* 0x00000700 */
#define ICACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT                (13U)
#define ICACHE_CACHE_CTRL_CACHE_PF_EN_MASK                 (0x1U << ICACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT)                /* 0x00002000 */
/* CACHE_MAINTAIN0 */
#define ICACHE_CACHE_MAINTAIN0_OFFSET                      (0x4U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT         (0U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_VALID_MASK          (0x1U << ICACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT)         /* 0x00000001 */
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT           (1U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_CMD_MASK            (0x3U << ICACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT)           /* 0x00000006 */
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT          (5U)
#define ICACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_MASK           (0x7FFFFFFU << ICACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT)    /* 0xFFFFFFE0 */
/* CACHE_MAINTAIN1 */
#define ICACHE_CACHE_MAINTAIN1_OFFSET                      (0x8U)
#define ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT        (0U)
#define ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_MASK         (0xFFFFU << ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT)     /* 0x0000FFFF */
/* STB_TIMEOUT_CTRL */
#define ICACHE_STB_TIMEOUT_CTRL_OFFSET                     (0xCU)
#define ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT    (0U)
#define ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_MASK     (0x7FFFFU << ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT) /* 0x0007FFFF */
/* CACHE_INT_EN */
#define ICACHE_CACHE_INT_EN_OFFSET                         (0x20U)
#define ICACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT            (0U)
#define ICACHE_CACHE_INT_EN_ERR_RECORD_EN_MASK             (0x1U << ICACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT)            /* 0x00000001 */
/* CACHE_INT_ST */
#define ICACHE_CACHE_INT_ST_OFFSET                         (0x24U)
#define ICACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT         (0U)
#define ICACHE_CACHE_INT_ST_AHB_ERROR_STATUS_MASK          (0x1U << ICACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT)         /* 0x00000001 */
/* CACHE_ERR_HADDR */
#define ICACHE_CACHE_ERR_HADDR_OFFSET                      (0x28U)
#define ICACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT          (0U)
#define ICACHE_CACHE_ERR_HADDR_STATUS_HADDR_MASK           (0x1U << ICACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT)          /* 0x00000001 */
/* CACHE_STATUS */
#define ICACHE_CACHE_STATUS_OFFSET                         (0x30U)
#define ICACHE_CACHE_STATUS                                (0x0U)
#define ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT        (0U)
#define ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK         (0x1U << ICACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT)        /* 0x00000001 */
#define ICACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT             (1U)
#define ICACHE_CACHE_STATUS_CACHE_M_BUSY_MASK              (0x1U << ICACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT)             /* 0x00000002 */
#define ICACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT         (2U)
#define ICACHE_CACHE_STATUS_CACHE_FLUSH_DONE_MASK          (0x1U << ICACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT)         /* 0x00000004 */
/* PMU_RD_NUM_CNT */
#define ICACHE_PMU_RD_NUM_CNT_OFFSET                       (0x40U)
#define ICACHE_PMU_RD_NUM_CNT                              (0x0U)
#define ICACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT         (0U)
#define ICACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_NUM_CNT */
#define ICACHE_PMU_WR_NUM_CNT_OFFSET                       (0x44U)
#define ICACHE_PMU_WR_NUM_CNT                              (0x0U)
#define ICACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_SRAM_RD_HIT_CNT */
#define ICACHE_PMU_SRAM_RD_HIT_CNT_OFFSET                  (0x48U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT                         (0x0U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT (0U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_HB_RD_HIT_CNT */
#define ICACHE_PMU_HB_RD_HIT_CNT_OFFSET                    (0x4CU)
#define ICACHE_PMU_HB_RD_HIT_CNT                           (0x0U)
#define ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT   (0U)
#define ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_MASK    (0xFFFFFFFFU << ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_STB_RD_HIT_CNT */
#define ICACHE_PMU_STB_RD_HIT_CNT_OFFSET                   (0x50U)
#define ICACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT (0U)
#define ICACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_MASK  (0xFFFFFFFFU << ICACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_HIT_CNT */
#define ICACHE_PMU_RD_HIT_CNT_OFFSET                       (0x54U)
#define ICACHE_PMU_RD_HIT_CNT                              (0x0U)
#define ICACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT         (0U)
#define ICACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_HIT_CNT */
#define ICACHE_PMU_WR_HIT_CNT_OFFSET                       (0x58U)
#define ICACHE_PMU_WR_HIT_CNT                              (0x0U)
#define ICACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_RD_MISS_PENALTY_CNT */
#define ICACHE_PMU_RD_MISS_PENALTY_CNT_OFFSET              (0x5CU)
#define ICACHE_PMU_RD_MISS_PENALTY_CNT                     (0x0U)
#define ICACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT (0U)
#define ICACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_WR_MISS_PENALTY_CNT */
#define ICACHE_PMU_WR_MISS_PENALTY_CNT_OFFSET              (0x60U)
#define ICACHE_PMU_WR_MISS_PENALTY_CNT                     (0x0U)
#define ICACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT (0U)
#define ICACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_LAT_CNT */
#define ICACHE_PMU_RD_LAT_CNT_OFFSET                       (0x64U)
#define ICACHE_PMU_RD_LAT_CNT                              (0x0U)
#define ICACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define ICACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_LAT_CNT */
#define ICACHE_PMU_WR_LAT_CNT_OFFSET                       (0x68U)
#define ICACHE_PMU_WR_LAT_CNT                              (0x0U)
#define ICACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* REVISION */
#define ICACHE_REVISION_OFFSET                             (0xF0U)
#define ICACHE_REVISION_REVISION_SHIFT                     (0U)
#define ICACHE_REVISION_REVISION_MASK                      (0xFFFFFFFFU << ICACHE_REVISION_REVISION_SHIFT)              /* 0xFFFFFFFF */
/*****************************************DCACHE*****************************************/
/* CACHE_CTRL */
#define DCACHE_CACHE_CTRL_OFFSET                           (0x0U)
#define DCACHE_CACHE_CTRL_CACHE_EN_SHIFT                   (0U)
#define DCACHE_CACHE_CTRL_CACHE_EN_MASK                    (0x1U << DCACHE_CACHE_CTRL_CACHE_EN_SHIFT)                   /* 0x00000001 */
#define DCACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT                (1U)
#define DCACHE_CACHE_CTRL_CACHE_WT_EN_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_WT_EN_SHIFT)                /* 0x00000002 */
#define DCACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT                (2U)
#define DCACHE_CACHE_CTRL_CACHE_HB_EN_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_HB_EN_SHIFT)                /* 0x00000004 */
#define DCACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT               (3U)
#define DCACHE_CACHE_CTRL_CACHE_STB_EN_MASK                (0x1U << DCACHE_CACHE_CTRL_CACHE_STB_EN_SHIFT)               /* 0x00000008 */
#define DCACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT                (4U)
#define DCACHE_CACHE_CTRL_CACHE_FLUSH_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_FLUSH_SHIFT)                /* 0x00000010 */
#define DCACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT               (5U)
#define DCACHE_CACHE_CTRL_CACHE_PMU_EN_MASK                (0x1U << DCACHE_CACHE_CTRL_CACHE_PMU_EN_SHIFT)               /* 0x00000020 */
#define DCACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT               (6U)
#define DCACHE_CACHE_CTRL_CACHE_BYPASS_MASK                (0x1U << DCACHE_CACHE_CTRL_CACHE_BYPASS_SHIFT)               /* 0x00000040 */
#define DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT             (7U)
#define DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_MASK              (0x1U << DCACHE_CACHE_CTRL_STB_TIMEOUT_EN_SHIFT)             /* 0x00000080 */
#define DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT         (8U)
#define DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_MASK          (0x7U << DCACHE_CACHE_CTRL_CACHE_ENTRY_THRESH_SHIFT)         /* 0x00000700 */
#define DCACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT                (13U)
#define DCACHE_CACHE_CTRL_CACHE_PF_EN_MASK                 (0x1U << DCACHE_CACHE_CTRL_CACHE_PF_EN_SHIFT)                /* 0x00002000 */
/* CACHE_MAINTAIN0 */
#define DCACHE_CACHE_MAINTAIN0_OFFSET                      (0x4U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT         (0U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_VALID_MASK          (0x1U << DCACHE_CACHE_MAINTAIN0_CACHE_M_VALID_SHIFT)         /* 0x00000001 */
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT           (1U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_CMD_MASK            (0x3U << DCACHE_CACHE_MAINTAIN0_CACHE_M_CMD_SHIFT)           /* 0x00000006 */
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT          (5U)
#define DCACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_MASK           (0x7FFFFFFU << DCACHE_CACHE_MAINTAIN0_CACHE_M_ADDR_SHIFT)    /* 0xFFFFFFE0 */
/* CACHE_MAINTAIN1 */
#define DCACHE_CACHE_MAINTAIN1_OFFSET                      (0x8U)
#define DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT        (0U)
#define DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_MASK         (0xFFFFU << DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT)     /* 0x0000FFFF */
/* STB_TIMEOUT_CTRL */
#define DCACHE_STB_TIMEOUT_CTRL_OFFSET                     (0xCU)
#define DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT    (0U)
#define DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_MASK     (0x7FFFFU << DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT) /* 0x0007FFFF */
/* CACHE_INT_EN */
#define DCACHE_CACHE_INT_EN_OFFSET                         (0x20U)
#define DCACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT            (0U)
#define DCACHE_CACHE_INT_EN_ERR_RECORD_EN_MASK             (0x1U << DCACHE_CACHE_INT_EN_ERR_RECORD_EN_SHIFT)            /* 0x00000001 */
/* CACHE_INT_ST */
#define DCACHE_CACHE_INT_ST_OFFSET                         (0x24U)
#define DCACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT         (0U)
#define DCACHE_CACHE_INT_ST_AHB_ERROR_STATUS_MASK          (0x1U << DCACHE_CACHE_INT_ST_AHB_ERROR_STATUS_SHIFT)         /* 0x00000001 */
/* CACHE_ERR_HADDR */
#define DCACHE_CACHE_ERR_HADDR_OFFSET                      (0x28U)
#define DCACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT          (0U)
#define DCACHE_CACHE_ERR_HADDR_STATUS_HADDR_MASK           (0x1U << DCACHE_CACHE_ERR_HADDR_STATUS_HADDR_SHIFT)          /* 0x00000001 */
/* CACHE_STATUS */
#define DCACHE_CACHE_STATUS_OFFSET                         (0x30U)
#define DCACHE_CACHE_STATUS                                (0x0U)
#define DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT        (0U)
#define DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_MASK         (0x1U << DCACHE_CACHE_STATUS_CACHE_INIT_FINISH_SHIFT)        /* 0x00000001 */
#define DCACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT             (1U)
#define DCACHE_CACHE_STATUS_CACHE_M_BUSY_MASK              (0x1U << DCACHE_CACHE_STATUS_CACHE_M_BUSY_SHIFT)             /* 0x00000002 */
#define DCACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT         (2U)
#define DCACHE_CACHE_STATUS_CACHE_FLUSH_DONE_MASK          (0x1U << DCACHE_CACHE_STATUS_CACHE_FLUSH_DONE_SHIFT)         /* 0x00000004 */
/* PMU_RD_NUM_CNT */
#define DCACHE_PMU_RD_NUM_CNT_OFFSET                       (0x40U)
#define DCACHE_PMU_RD_NUM_CNT                              (0x0U)
#define DCACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT         (0U)
#define DCACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_RD_NUM_CNT_PMU_RD_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_NUM_CNT */
#define DCACHE_PMU_WR_NUM_CNT_OFFSET                       (0x44U)
#define DCACHE_PMU_WR_NUM_CNT                              (0x0U)
#define DCACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_NUM_CNT_PMU_WR_NUM_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_SRAM_RD_HIT_CNT */
#define DCACHE_PMU_SRAM_RD_HIT_CNT_OFFSET                  (0x48U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT                         (0x0U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT (0U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_SRAM_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_HB_RD_HIT_CNT */
#define DCACHE_PMU_HB_RD_HIT_CNT_OFFSET                    (0x4CU)
#define DCACHE_PMU_HB_RD_HIT_CNT                           (0x0U)
#define DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT   (0U)
#define DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_MASK    (0xFFFFFFFFU << DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_STB_RD_HIT_CNT */
#define DCACHE_PMU_STB_RD_HIT_CNT_OFFSET                   (0x50U)
#define DCACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT (0U)
#define DCACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_MASK  (0xFFFFFFFFU << DCACHE_PMU_STB_RD_HIT_CNT_PMU_STB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_HIT_CNT */
#define DCACHE_PMU_RD_HIT_CNT_OFFSET                       (0x54U)
#define DCACHE_PMU_RD_HIT_CNT                              (0x0U)
#define DCACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT         (0U)
#define DCACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_RD_HIT_CNT_PMU_RD_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_HIT_CNT */
#define DCACHE_PMU_WR_HIT_CNT_OFFSET                       (0x58U)
#define DCACHE_PMU_WR_HIT_CNT                              (0x0U)
#define DCACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_HIT_CNT_PMU_WR_HIT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_RD_MISS_PENALTY_CNT */
#define DCACHE_PMU_RD_MISS_PENALTY_CNT_OFFSET              (0x5CU)
#define DCACHE_PMU_RD_MISS_PENALTY_CNT                     (0x0U)
#define DCACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT (0U)
#define DCACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_RD_MISS_PENALTY_CNT_PMU_RD_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_WR_MISS_PENALTY_CNT */
#define DCACHE_PMU_WR_MISS_PENALTY_CNT_OFFSET              (0x60U)
#define DCACHE_PMU_WR_MISS_PENALTY_CNT                     (0x0U)
#define DCACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT (0U)
#define DCACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_WR_MISS_PENALTY_CNT_PMU_WR_MISS_PENALTY_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_RD_LAT_CNT */
#define DCACHE_PMU_RD_LAT_CNT_OFFSET                       (0x64U)
#define DCACHE_PMU_RD_LAT_CNT                              (0x0U)
#define DCACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define DCACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_RD_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* PMU_WR_LAT_CNT */
#define DCACHE_PMU_WR_LAT_CNT_OFFSET                       (0x68U)
#define DCACHE_PMU_WR_LAT_CNT                              (0x0U)
#define DCACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_LAT_CNT_PMU_RD_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* REVISION */
#define DCACHE_REVISION_OFFSET                             (0xF0U)
#define DCACHE_REVISION_REVISION_SHIFT                     (0U)
#define DCACHE_REVISION_REVISION_MASK                      (0xFFFFFFFFU << DCACHE_REVISION_REVISION_SHIFT)              /* 0xFFFFFFFF */
/******************************************DMA*******************************************/
/* SAR0 */
#define DMA_SAR0_OFFSET                                    (0x0U)
#define DMA_SAR0_SAR_SHIFT                                 (0U)
#define DMA_SAR0_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR0_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR0 */
#define DMA_DAR0_OFFSET                                    (0x8U)
#define DMA_DAR0_DAR_SHIFT                                 (0U)
#define DMA_DAR0_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR0_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP0 */
#define DMA_LLP0_OFFSET                                    (0x10U)
#define DMA_LLP0_LMS_SHIFT                                 (0U)
#define DMA_LLP0_LMS_MASK                                  (0x3U << DMA_LLP0_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP0_LOC_SHIFT                                 (2U)
#define DMA_LLP0_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP0_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL0 */
#define DMA_CTL0_OFFSET                                    (0x18U)
#define DMA_CTL0_INT_EN_SHIFT                              (0U)
#define DMA_CTL0_INT_EN_MASK                               (0x1U << DMA_CTL0_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL0_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL0_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL0_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL0_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL0_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL0_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL0_DINC_SHIFT                                (7U)
#define DMA_CTL0_DINC_MASK                                 (0x3U << DMA_CTL0_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL0_SINC_SHIFT                                (9U)
#define DMA_CTL0_SINC_MASK                                 (0x3U << DMA_CTL0_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL0_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL0_DEST_MSIZE_MASK                           (0x7U << DMA_CTL0_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL0_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL0_SRC_MSIZE_MASK                            (0x7U << DMA_CTL0_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL0_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL0_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL0_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL0_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL0_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL0_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL0_TT_FC_SHIFT                               (20U)
#define DMA_CTL0_TT_FC_MASK                                (0x7U << DMA_CTL0_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL0_DMS_SHIFT                                 (23U)
#define DMA_CTL0_DMS_MASK                                  (0x3U << DMA_CTL0_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL0_SMS_SHIFT                                 (25U)
#define DMA_CTL0_SMS_MASK                                  (0x3U << DMA_CTL0_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL0_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL0_LLP_DST_EN_MASK                           (0x1U << DMA_CTL0_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL0_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL0_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL0_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL0_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL0_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL0_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL0_DONE_SHIFT                                (44U)
#define DMA_CTL0_DONE_MASK                                 (0x1U << DMA_CTL0_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT0 */
#define DMA_SSTAT0_OFFSET                                  (0x20U)
#define DMA_SSTAT0_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT0_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT0_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT0 */
#define DMA_DSTAT0_OFFSET                                  (0x28U)
#define DMA_DSTAT0_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT0_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT0_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR0 */
#define DMA_SSTATAR0_OFFSET                                (0x30U)
#define DMA_SSTATAR0_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR0_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR0_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR0 */
#define DMA_DSTATAR0_OFFSET                                (0x38U)
#define DMA_DSTATAR0_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR0_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR0_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG0 */
#define DMA_CFG0_OFFSET                                    (0x40U)
#define DMA_CFG0_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG0_CH_PRIOR_MASK                             (0x7U << DMA_CFG0_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG0_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG0_CH_SUSP_MASK                              (0x1U << DMA_CFG0_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG0_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG0_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG0_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG0_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG0_HS_SEL_DST_MASK                           (0x1U << DMA_CFG0_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG0_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG0_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG0_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG0_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG0_LOCK_CH_L_MASK                            (0x3U << DMA_CFG0_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG0_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG0_LOCK_B_L_MASK                             (0x3U << DMA_CFG0_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG0_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG0_LOCK_CH_MASK                              (0x1U << DMA_CFG0_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG0_LOCK_B_SHIFT                              (17U)
#define DMA_CFG0_LOCK_B_MASK                               (0x1U << DMA_CFG0_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG0_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG0_DST_HS_POL_MASK                           (0x1U << DMA_CFG0_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG0_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG0_SRC_HS_POL_MASK                           (0x1U << DMA_CFG0_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG0_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG0_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG0_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG0_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG0_RELOAD_SRC_MASK                           (0x1U << DMA_CFG0_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG0_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG0_RELOAD_DST_MASK                           (0x1U << DMA_CFG0_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG0_FCMODE_SHIFT                              (32U)
#define DMA_CFG0_FCMODE_MASK                               (0x1U << DMA_CFG0_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG0_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG0_FIFO_MODE_MASK                            (0x1U << DMA_CFG0_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG0_PROTCTL_SHIFT                             (34U)
#define DMA_CFG0_PROTCTL_MASK                              (0x7U << DMA_CFG0_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG0_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG0_DS_UPD_EN_MASK                            (0x1U << DMA_CFG0_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG0_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG0_SS_UPD_EN_MASK                            (0x1U << DMA_CFG0_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG0_SRC_PER_SHIFT                             (39U)
#define DMA_CFG0_SRC_PER_MASK                              (0xFU << DMA_CFG0_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG0_DEST_PER_SHIFT                            (43U)
#define DMA_CFG0_DEST_PER_MASK                             (0xFU << DMA_CFG0_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR0 */
#define DMA_SGR0_OFFSET                                    (0x48U)
#define DMA_SGR0_SGI_SHIFT                                 (0U)
#define DMA_SGR0_SGI_MASK                                  (0xFFFFFU << DMA_SGR0_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR0_SGC_SHIFT                                 (20U)
#define DMA_SGR0_SGC_MASK                                  (0xFFFU << DMA_SGR0_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR0 */
#define DMA_DSR0_OFFSET                                    (0x50U)
#define DMA_DSR0_DSI_SHIFT                                 (0U)
#define DMA_DSR0_DSI_MASK                                  (0xFFFFFU << DMA_DSR0_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR0_DSC_SHIFT                                 (20U)
#define DMA_DSR0_DSC_MASK                                  (0xFFFU << DMA_DSR0_DSC_SHIFT)                               /* 0xFFF00000 */
/* SAR1 */
#define DMA_SAR1_OFFSET                                    (0x58U)
#define DMA_SAR1_SAR_SHIFT                                 (0U)
#define DMA_SAR1_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR1_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR1 */
#define DMA_DAR1_OFFSET                                    (0x60U)
#define DMA_DAR1_DAR_SHIFT                                 (0U)
#define DMA_DAR1_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR1_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP1 */
#define DMA_LLP1_OFFSET                                    (0x68U)
#define DMA_LLP1_LMS_SHIFT                                 (0U)
#define DMA_LLP1_LMS_MASK                                  (0x3U << DMA_LLP1_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP1_LOC_SHIFT                                 (2U)
#define DMA_LLP1_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP1_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL1 */
#define DMA_CTL1_OFFSET                                    (0x70U)
#define DMA_CTL1_INT_EN_SHIFT                              (0U)
#define DMA_CTL1_INT_EN_MASK                               (0x1U << DMA_CTL1_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL1_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL1_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL1_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL1_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL1_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL1_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL1_DINC_SHIFT                                (7U)
#define DMA_CTL1_DINC_MASK                                 (0x3U << DMA_CTL1_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL1_SINC_SHIFT                                (9U)
#define DMA_CTL1_SINC_MASK                                 (0x3U << DMA_CTL1_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL1_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL1_DEST_MSIZE_MASK                           (0x7U << DMA_CTL1_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL1_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL1_SRC_MSIZE_MASK                            (0x7U << DMA_CTL1_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL1_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL1_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL1_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL1_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL1_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL1_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL1_TT_FC_SHIFT                               (20U)
#define DMA_CTL1_TT_FC_MASK                                (0x7U << DMA_CTL1_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL1_DMS_SHIFT                                 (23U)
#define DMA_CTL1_DMS_MASK                                  (0x3U << DMA_CTL1_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL1_SMS_SHIFT                                 (25U)
#define DMA_CTL1_SMS_MASK                                  (0x3U << DMA_CTL1_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL1_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL1_LLP_DST_EN_MASK                           (0x1U << DMA_CTL1_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL1_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL1_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL1_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL1_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL1_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL1_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL1_DONE_SHIFT                                (44U)
#define DMA_CTL1_DONE_MASK                                 (0x1U << DMA_CTL1_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT1 */
#define DMA_SSTAT1_OFFSET                                  (0x78U)
#define DMA_SSTAT1_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT1_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT1_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT1 */
#define DMA_DSTAT1_OFFSET                                  (0x80U)
#define DMA_DSTAT1_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT1_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT1_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR1 */
#define DMA_SSTATAR1_OFFSET                                (0x88U)
#define DMA_SSTATAR1_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR1_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR1_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR1 */
#define DMA_DSTATAR1_OFFSET                                (0x90U)
#define DMA_DSTATAR1_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR1_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR1_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG1 */
#define DMA_CFG1_OFFSET                                    (0x98U)
#define DMA_CFG1_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG1_CH_PRIOR_MASK                             (0x7U << DMA_CFG1_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG1_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG1_CH_SUSP_MASK                              (0x1U << DMA_CFG1_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG1_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG1_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG1_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG1_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG1_HS_SEL_DST_MASK                           (0x1U << DMA_CFG1_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG1_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG1_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG1_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG1_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG1_LOCK_CH_L_MASK                            (0x3U << DMA_CFG1_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG1_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG1_LOCK_B_L_MASK                             (0x3U << DMA_CFG1_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG1_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG1_LOCK_CH_MASK                              (0x1U << DMA_CFG1_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG1_LOCK_B_SHIFT                              (17U)
#define DMA_CFG1_LOCK_B_MASK                               (0x1U << DMA_CFG1_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG1_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG1_DST_HS_POL_MASK                           (0x1U << DMA_CFG1_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG1_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG1_SRC_HS_POL_MASK                           (0x1U << DMA_CFG1_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG1_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG1_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG1_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG1_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG1_RELOAD_SRC_MASK                           (0x1U << DMA_CFG1_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG1_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG1_RELOAD_DST_MASK                           (0x1U << DMA_CFG1_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG1_FCMODE_SHIFT                              (32U)
#define DMA_CFG1_FCMODE_MASK                               (0x1U << DMA_CFG1_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG1_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG1_FIFO_MODE_MASK                            (0x1U << DMA_CFG1_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG1_PROTCTL_SHIFT                             (34U)
#define DMA_CFG1_PROTCTL_MASK                              (0x7U << DMA_CFG1_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG1_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG1_DS_UPD_EN_MASK                            (0x1U << DMA_CFG1_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG1_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG1_SS_UPD_EN_MASK                            (0x1U << DMA_CFG1_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG1_SRC_PER_SHIFT                             (39U)
#define DMA_CFG1_SRC_PER_MASK                              (0xFU << DMA_CFG1_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG1_DEST_PER_SHIFT                            (43U)
#define DMA_CFG1_DEST_PER_MASK                             (0xFU << DMA_CFG1_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR1 */
#define DMA_SGR1_OFFSET                                    (0xA0U)
#define DMA_SGR1_SGI_SHIFT                                 (0U)
#define DMA_SGR1_SGI_MASK                                  (0xFFFFFU << DMA_SGR1_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR1_SGC_SHIFT                                 (20U)
#define DMA_SGR1_SGC_MASK                                  (0xFFFU << DMA_SGR1_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR1 */
#define DMA_DSR1_OFFSET                                    (0xA8U)
#define DMA_DSR1_DSI_SHIFT                                 (0U)
#define DMA_DSR1_DSI_MASK                                  (0xFFFFFU << DMA_DSR1_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR1_DSC_SHIFT                                 (20U)
#define DMA_DSR1_DSC_MASK                                  (0xFFFU << DMA_DSR1_DSC_SHIFT)                               /* 0xFFF00000 */
/* SAR2 */
#define DMA_SAR2_OFFSET                                    (0xB0U)
#define DMA_SAR2_SAR_SHIFT                                 (0U)
#define DMA_SAR2_SAR_MASK                                  (0xFFFFFFFFU << DMA_SAR2_SAR_SHIFT)                          /* 0xFFFFFFFF */
/* DAR2 */
#define DMA_DAR2_OFFSET                                    (0xB8U)
#define DMA_DAR2_DAR_SHIFT                                 (0U)
#define DMA_DAR2_DAR_MASK                                  (0xFFFFFFFFU << DMA_DAR2_DAR_SHIFT)                          /* 0xFFFFFFFF */
/* LLP2 */
#define DMA_LLP2_OFFSET                                    (0xC0U)
#define DMA_LLP2_LMS_SHIFT                                 (0U)
#define DMA_LLP2_LMS_MASK                                  (0x3U << DMA_LLP2_LMS_SHIFT)                                 /* 0x00000003 */
#define DMA_LLP2_LOC_SHIFT                                 (2U)
#define DMA_LLP2_LOC_MASK                                  (0x3FFFFFFFU << DMA_LLP2_LOC_SHIFT)                          /* 0xFFFFFFFC */
/* CTL2 */
#define DMA_CTL2_OFFSET                                    (0xC8U)
#define DMA_CTL2_INT_EN_SHIFT                              (0U)
#define DMA_CTL2_INT_EN_MASK                               (0x1U << DMA_CTL2_INT_EN_SHIFT)                              /* 0x0000000000000001 */
#define DMA_CTL2_DST_TR_WIDTH_SHIFT                        (1U)
#define DMA_CTL2_DST_TR_WIDTH_MASK                         (0x7U << DMA_CTL2_DST_TR_WIDTH_SHIFT)                        /* 0x000000000000000E */
#define DMA_CTL2_SRC_TR_WIDTH_SHIFT                        (4U)
#define DMA_CTL2_SRC_TR_WIDTH_MASK                         (0x7U << DMA_CTL2_SRC_TR_WIDTH_SHIFT)                        /* 0x0000000000000070 */
#define DMA_CTL2_DINC_SHIFT                                (7U)
#define DMA_CTL2_DINC_MASK                                 (0x3U << DMA_CTL2_DINC_SHIFT)                                /* 0x0000000000000180 */
#define DMA_CTL2_SINC_SHIFT                                (9U)
#define DMA_CTL2_SINC_MASK                                 (0x3U << DMA_CTL2_SINC_SHIFT)                                /* 0x0000000000000600 */
#define DMA_CTL2_DEST_MSIZE_SHIFT                          (11U)
#define DMA_CTL2_DEST_MSIZE_MASK                           (0x7U << DMA_CTL2_DEST_MSIZE_SHIFT)                          /* 0x0000000000003800 */
#define DMA_CTL2_SRC_MSIZE_SHIFT                           (14U)
#define DMA_CTL2_SRC_MSIZE_MASK                            (0x7U << DMA_CTL2_SRC_MSIZE_SHIFT)                           /* 0x000000000001C000 */
#define DMA_CTL2_SRC_GATHER_EN_SHIFT                       (17U)
#define DMA_CTL2_SRC_GATHER_EN_MASK                        (0x1U << DMA_CTL2_SRC_GATHER_EN_SHIFT)                       /* 0x0000000000020000 */
#define DMA_CTL2_DST_SCATTER_EN_SHIFT                      (18U)
#define DMA_CTL2_DST_SCATTER_EN_MASK                       (0x1U << DMA_CTL2_DST_SCATTER_EN_SHIFT)                      /* 0x0000000000040000 */
#define DMA_CTL2_TT_FC_SHIFT                               (20U)
#define DMA_CTL2_TT_FC_MASK                                (0x7U << DMA_CTL2_TT_FC_SHIFT)                               /* 0x0000000000700000 */
#define DMA_CTL2_DMS_SHIFT                                 (23U)
#define DMA_CTL2_DMS_MASK                                  (0x3U << DMA_CTL2_DMS_SHIFT)                                 /* 0x0000000001800000 */
#define DMA_CTL2_SMS_SHIFT                                 (25U)
#define DMA_CTL2_SMS_MASK                                  (0x3U << DMA_CTL2_SMS_SHIFT)                                 /* 0x0000000006000000 */
#define DMA_CTL2_LLP_DST_EN_SHIFT                          (27U)
#define DMA_CTL2_LLP_DST_EN_MASK                           (0x1U << DMA_CTL2_LLP_DST_EN_SHIFT)                          /* 0x0000000008000000 */
#define DMA_CTL2_LLP_SRC_EN_SHIFT                          (28U)
#define DMA_CTL2_LLP_SRC_EN_MASK                           (0x1U << DMA_CTL2_LLP_SRC_EN_SHIFT)                          /* 0x0000000010000000 */
#define DMA_CTL2_BLOCK_TS_SHIFT                            (32U)
#define DMA_CTL2_BLOCK_TS_MASK                             (0xFFFU << DMA_CTL2_BLOCK_TS_SHIFT)                          /* 0x00000FFF00000000 */
#define DMA_CTL2_DONE_SHIFT                                (44U)
#define DMA_CTL2_DONE_MASK                                 (0x1U << DMA_CTL2_DONE_SHIFT)                                /* 0x0000100000000000 */
/* SSTAT2 */
#define DMA_SSTAT2_OFFSET                                  (0xD0U)
#define DMA_SSTAT2_SSTAT_SHIFT                             (0U)
#define DMA_SSTAT2_SSTAT_MASK                              (0xFFFFFFFFU << DMA_SSTAT2_SSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* DSTAT2 */
#define DMA_DSTAT2_OFFSET                                  (0xD8U)
#define DMA_DSTAT2_DSTAT_SHIFT                             (0U)
#define DMA_DSTAT2_DSTAT_MASK                              (0xFFFFFFFFU << DMA_DSTAT2_DSTAT_SHIFT)                      /* 0xFFFFFFFF */
/* SSTATAR2 */
#define DMA_SSTATAR2_OFFSET                                (0xE0U)
#define DMA_SSTATAR2_SSTATAR_SHIFT                         (0U)
#define DMA_SSTATAR2_SSTATAR_MASK                          (0xFFFFFFFFU << DMA_SSTATAR2_SSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* DSTATAR2 */
#define DMA_DSTATAR2_OFFSET                                (0xE8U)
#define DMA_DSTATAR2_DSTATAR_SHIFT                         (0U)
#define DMA_DSTATAR2_DSTATAR_MASK                          (0xFFFFFFFFU << DMA_DSTATAR2_DSTATAR_SHIFT)                  /* 0xFFFFFFFF */
/* CFG2 */
#define DMA_CFG2_OFFSET                                    (0xF0U)
#define DMA_CFG2_CH_PRIOR_SHIFT                            (5U)
#define DMA_CFG2_CH_PRIOR_MASK                             (0x7U << DMA_CFG2_CH_PRIOR_SHIFT)                            /* 0x00000000000000E0 */
#define DMA_CFG2_CH_SUSP_SHIFT                             (8U)
#define DMA_CFG2_CH_SUSP_MASK                              (0x1U << DMA_CFG2_CH_SUSP_SHIFT)                             /* 0x0000000000000100 */
#define DMA_CFG2_FIFO_EMPTY_SHIFT                          (9U)
#define DMA_CFG2_FIFO_EMPTY_MASK                           (0x1U << DMA_CFG2_FIFO_EMPTY_SHIFT)                          /* 0x0000000000000200 */
#define DMA_CFG2_HS_SEL_DST_SHIFT                          (10U)
#define DMA_CFG2_HS_SEL_DST_MASK                           (0x1U << DMA_CFG2_HS_SEL_DST_SHIFT)                          /* 0x0000000000000400 */
#define DMA_CFG2_HS_SEL_SRC_SHIFT                          (11U)
#define DMA_CFG2_HS_SEL_SRC_MASK                           (0x1U << DMA_CFG2_HS_SEL_SRC_SHIFT)                          /* 0x0000000000000800 */
#define DMA_CFG2_LOCK_CH_L_SHIFT                           (12U)
#define DMA_CFG2_LOCK_CH_L_MASK                            (0x3U << DMA_CFG2_LOCK_CH_L_SHIFT)                           /* 0x0000000000003000 */
#define DMA_CFG2_LOCK_B_L_SHIFT                            (14U)
#define DMA_CFG2_LOCK_B_L_MASK                             (0x3U << DMA_CFG2_LOCK_B_L_SHIFT)                            /* 0x000000000000C000 */
#define DMA_CFG2_LOCK_CH_SHIFT                             (16U)
#define DMA_CFG2_LOCK_CH_MASK                              (0x1U << DMA_CFG2_LOCK_CH_SHIFT)                             /* 0x0000000000010000 */
#define DMA_CFG2_LOCK_B_SHIFT                              (17U)
#define DMA_CFG2_LOCK_B_MASK                               (0x1U << DMA_CFG2_LOCK_B_SHIFT)                              /* 0x0000000000020000 */
#define DMA_CFG2_DST_HS_POL_SHIFT                          (18U)
#define DMA_CFG2_DST_HS_POL_MASK                           (0x1U << DMA_CFG2_DST_HS_POL_SHIFT)                          /* 0x0000000000040000 */
#define DMA_CFG2_SRC_HS_POL_SHIFT                          (19U)
#define DMA_CFG2_SRC_HS_POL_MASK                           (0x1U << DMA_CFG2_SRC_HS_POL_SHIFT)                          /* 0x0000000000080000 */
#define DMA_CFG2_MAX_ABRST_SHIFT                           (20U)
#define DMA_CFG2_MAX_ABRST_MASK                            (0x3FFU << DMA_CFG2_MAX_ABRST_SHIFT)                         /* 0x000000003FF00000 */
#define DMA_CFG2_RELOAD_SRC_SHIFT                          (30U)
#define DMA_CFG2_RELOAD_SRC_MASK                           (0x1U << DMA_CFG2_RELOAD_SRC_SHIFT)                          /* 0x0000000040000000 */
#define DMA_CFG2_RELOAD_DST_SHIFT                          (31U)
#define DMA_CFG2_RELOAD_DST_MASK                           (0x1U << DMA_CFG2_RELOAD_DST_SHIFT)                          /* 0x0000000080000000 */
#define DMA_CFG2_FCMODE_SHIFT                              (32U)
#define DMA_CFG2_FCMODE_MASK                               (0x1U << DMA_CFG2_FCMODE_SHIFT)                              /* 0x0000000100000000 */
#define DMA_CFG2_FIFO_MODE_SHIFT                           (33U)
#define DMA_CFG2_FIFO_MODE_MASK                            (0x1U << DMA_CFG2_FIFO_MODE_SHIFT)                           /* 0x0000000200000000 */
#define DMA_CFG2_PROTCTL_SHIFT                             (34U)
#define DMA_CFG2_PROTCTL_MASK                              (0x7U << DMA_CFG2_PROTCTL_SHIFT)                             /* 0x0000001C00000000 */
#define DMA_CFG2_DS_UPD_EN_SHIFT                           (37U)
#define DMA_CFG2_DS_UPD_EN_MASK                            (0x1U << DMA_CFG2_DS_UPD_EN_SHIFT)                           /* 0x0000002000000000 */
#define DMA_CFG2_SS_UPD_EN_SHIFT                           (38U)
#define DMA_CFG2_SS_UPD_EN_MASK                            (0x1U << DMA_CFG2_SS_UPD_EN_SHIFT)                           /* 0x0000004000000000 */
#define DMA_CFG2_SRC_PER_SHIFT                             (39U)
#define DMA_CFG2_SRC_PER_MASK                              (0xFU << DMA_CFG2_SRC_PER_SHIFT)                             /* 0x0000078000000000 */
#define DMA_CFG2_DEST_PER_SHIFT                            (43U)
#define DMA_CFG2_DEST_PER_MASK                             (0xFU << DMA_CFG2_DEST_PER_SHIFT)                            /* 0x0000780000000000 */
/* SGR2 */
#define DMA_SGR2_OFFSET                                    (0xF8U)
#define DMA_SGR2_SGI_SHIFT                                 (0U)
#define DMA_SGR2_SGI_MASK                                  (0xFFFFFU << DMA_SGR2_SGI_SHIFT)                             /* 0x000FFFFF */
#define DMA_SGR2_SGC_SHIFT                                 (20U)
#define DMA_SGR2_SGC_MASK                                  (0xFFFU << DMA_SGR2_SGC_SHIFT)                               /* 0xFFF00000 */
/* DSR2 */
#define DMA_DSR2_OFFSET                                    (0x100U)
#define DMA_DSR2_DSI_SHIFT                                 (0U)
#define DMA_DSR2_DSI_MASK                                  (0xFFFFFU << DMA_DSR2_DSI_SHIFT)                             /* 0x000FFFFF */
#define DMA_DSR2_DSC_SHIFT                                 (20U)
#define DMA_DSR2_DSC_MASK                                  (0xFFFU << DMA_DSR2_DSC_SHIFT)                               /* 0xFFF00000 */
/* RAWTFR */
#define DMA_RAWTFR_OFFSET                                  (0x2C0U)
#define DMA_RAWTFR                                         (0x0U)
#define DMA_RAWTFR_RAW_SHIFT                               (0U)
#define DMA_RAWTFR_RAW_MASK                                (0x7U << DMA_RAWTFR_RAW_SHIFT)                               /* 0x00000007 */
/* RAWBLOCK */
#define DMA_RAWBLOCK_OFFSET                                (0x2C8U)
#define DMA_RAWBLOCK                                       (0x0U)
#define DMA_RAWBLOCK_RAW_SHIFT                             (0U)
#define DMA_RAWBLOCK_RAW_MASK                              (0x7U << DMA_RAWBLOCK_RAW_SHIFT)                             /* 0x00000007 */
/* RAWSRCTRAN */
#define DMA_RAWSRCTRAN_OFFSET                              (0x2D0U)
#define DMA_RAWSRCTRAN                                     (0x0U)
#define DMA_RAWSRCTRAN_RAW_SHIFT                           (0U)
#define DMA_RAWSRCTRAN_RAW_MASK                            (0x7U << DMA_RAWSRCTRAN_RAW_SHIFT)                           /* 0x00000007 */
/* RAWDSTTRAN */
#define DMA_RAWDSTTRAN_OFFSET                              (0x2D8U)
#define DMA_RAWDSTTRAN                                     (0x0U)
#define DMA_RAWDSTTRAN_RAW_SHIFT                           (0U)
#define DMA_RAWDSTTRAN_RAW_MASK                            (0x7U << DMA_RAWDSTTRAN_RAW_SHIFT)                           /* 0x00000007 */
/* RAWERR */
#define DMA_RAWERR_OFFSET                                  (0x2E0U)
#define DMA_RAWERR                                         (0x0U)
#define DMA_RAWERR_RAW_SHIFT                               (0U)
#define DMA_RAWERR_RAW_MASK                                (0x7U << DMA_RAWERR_RAW_SHIFT)                               /* 0x00000007 */
/* STATUSTFR */
#define DMA_STATUSTFR_OFFSET                               (0x2E8U)
#define DMA_STATUSTFR                                      (0x0U)
#define DMA_STATUSTFR_STATUS_SHIFT                         (0U)
#define DMA_STATUSTFR_STATUS_MASK                          (0x7U << DMA_STATUSTFR_STATUS_SHIFT)                         /* 0x00000007 */
/* STATUSBLOCK */
#define DMA_STATUSBLOCK_OFFSET                             (0x2F0U)
#define DMA_STATUSBLOCK                                    (0x0U)
#define DMA_STATUSBLOCK_STATUS_SHIFT                       (0U)
#define DMA_STATUSBLOCK_STATUS_MASK                        (0x7U << DMA_STATUSBLOCK_STATUS_SHIFT)                       /* 0x00000007 */
/* STATUSSRCTRAN */
#define DMA_STATUSSRCTRAN_OFFSET                           (0x2F8U)
#define DMA_STATUSSRCTRAN                                  (0x0U)
#define DMA_STATUSSRCTRAN_STATUS_SHIFT                     (0U)
#define DMA_STATUSSRCTRAN_STATUS_MASK                      (0x7U << DMA_STATUSSRCTRAN_STATUS_SHIFT)                     /* 0x00000007 */
/* STATUSDSTTRAN */
#define DMA_STATUSDSTTRAN_OFFSET                           (0x300U)
#define DMA_STATUSDSTTRAN                                  (0x0U)
#define DMA_STATUSDSTTRAN_STATUS_SHIFT                     (0U)
#define DMA_STATUSDSTTRAN_STATUS_MASK                      (0x7U << DMA_STATUSDSTTRAN_STATUS_SHIFT)                     /* 0x00000007 */
/* STATUSERR */
#define DMA_STATUSERR_OFFSET                               (0x308U)
#define DMA_STATUSERR                                      (0x0U)
#define DMA_STATUSERR_STATUS_SHIFT                         (0U)
#define DMA_STATUSERR_STATUS_MASK                          (0x7U << DMA_STATUSERR_STATUS_SHIFT)                         /* 0x00000007 */
/* MASKTFR */
#define DMA_MASKTFR_OFFSET                                 (0x310U)
#define DMA_MASKTFR_INT_MASK_SHIFT                         (0U)
#define DMA_MASKTFR_INT_MASK_MASK                          (0x7U << DMA_MASKTFR_INT_MASK_SHIFT)                         /* 0x00000007 */
#define DMA_MASKTFR_INT_MASK_WE_SHIFT                      (8U)
#define DMA_MASKTFR_INT_MASK_WE_MASK                       (0x7U << DMA_MASKTFR_INT_MASK_WE_SHIFT)                      /* 0x00000700 */
/* MASKBLOCK */
#define DMA_MASKBLOCK_OFFSET                               (0x318U)
#define DMA_MASKBLOCK_INT_MASK_SHIFT                       (0U)
#define DMA_MASKBLOCK_INT_MASK_MASK                        (0x7U << DMA_MASKBLOCK_INT_MASK_SHIFT)                       /* 0x00000007 */
#define DMA_MASKBLOCK_INT_MASK_WE_SHIFT                    (8U)
#define DMA_MASKBLOCK_INT_MASK_WE_MASK                     (0x7U << DMA_MASKBLOCK_INT_MASK_WE_SHIFT)                    /* 0x00000700 */
/* MASKSRCTRAN */
#define DMA_MASKSRCTRAN_OFFSET                             (0x320U)
#define DMA_MASKSRCTRAN_INT_MASK_SHIFT                     (0U)
#define DMA_MASKSRCTRAN_INT_MASK_MASK                      (0x7U << DMA_MASKSRCTRAN_INT_MASK_SHIFT)                     /* 0x00000007 */
#define DMA_MASKSRCTRAN_INT_MASK_WE_SHIFT                  (8U)
#define DMA_MASKSRCTRAN_INT_MASK_WE_MASK                   (0x7U << DMA_MASKSRCTRAN_INT_MASK_WE_SHIFT)                  /* 0x00000700 */
/* MASKDSTTRAN */
#define DMA_MASKDSTTRAN_OFFSET                             (0x328U)
#define DMA_MASKDSTTRAN_INT_MASK_SHIFT                     (0U)
#define DMA_MASKDSTTRAN_INT_MASK_MASK                      (0x7U << DMA_MASKDSTTRAN_INT_MASK_SHIFT)                     /* 0x00000007 */
#define DMA_MASKDSTTRAN_INT_MASK_WE_SHIFT                  (8U)
#define DMA_MASKDSTTRAN_INT_MASK_WE_MASK                   (0x7U << DMA_MASKDSTTRAN_INT_MASK_WE_SHIFT)                  /* 0x00000700 */
/* MASKERR */
#define DMA_MASKERR_OFFSET                                 (0x330U)
#define DMA_MASKERR_INT_MASK_SHIFT                         (0U)
#define DMA_MASKERR_INT_MASK_MASK                          (0x7U << DMA_MASKERR_INT_MASK_SHIFT)                         /* 0x00000007 */
#define DMA_MASKERR_INT_MASK_WE_SHIFT                      (8U)
#define DMA_MASKERR_INT_MASK_WE_MASK                       (0x7U << DMA_MASKERR_INT_MASK_WE_SHIFT)                      /* 0x00000700 */
/* CLEARTFR */
#define DMA_CLEARTFR_OFFSET                                (0x338U)
#define DMA_CLEARTFR_CLEAR_SHIFT                           (0U)
#define DMA_CLEARTFR_CLEAR_MASK                            (0x7U << DMA_CLEARTFR_CLEAR_SHIFT)                           /* 0x00000007 */
/* CLEARBLOCK */
#define DMA_CLEARBLOCK_OFFSET                              (0x340U)
#define DMA_CLEARBLOCK_CLEAR_SHIFT                         (0U)
#define DMA_CLEARBLOCK_CLEAR_MASK                          (0x7U << DMA_CLEARBLOCK_CLEAR_SHIFT)                         /* 0x00000007 */
/* CLEARSRCTRAN */
#define DMA_CLEARSRCTRAN_OFFSET                            (0x348U)
#define DMA_CLEARSRCTRAN_CLEAR_SHIFT                       (0U)
#define DMA_CLEARSRCTRAN_CLEAR_MASK                        (0x7U << DMA_CLEARSRCTRAN_CLEAR_SHIFT)                       /* 0x00000007 */
/* CLEARDSTTRAN */
#define DMA_CLEARDSTTRAN_OFFSET                            (0x350U)
#define DMA_CLEARDSTTRAN_CLEAR_SHIFT                       (0U)
#define DMA_CLEARDSTTRAN_CLEAR_MASK                        (0x7U << DMA_CLEARDSTTRAN_CLEAR_SHIFT)                       /* 0x00000007 */
/* CLEARERR */
#define DMA_CLEARERR_OFFSET                                (0x358U)
#define DMA_CLEARERR_CLEAR_SHIFT                           (0U)
#define DMA_CLEARERR_CLEAR_MASK                            (0x7U << DMA_CLEARERR_CLEAR_SHIFT)                           /* 0x00000007 */
/* STATUSINT */
#define DMA_STATUSINT_OFFSET                               (0x360U)
#define DMA_STATUSINT                                      (0x0U)
#define DMA_STATUSINT_TFR_SHIFT                            (0U)
#define DMA_STATUSINT_TFR_MASK                             (0x1U << DMA_STATUSINT_TFR_SHIFT)                            /* 0x00000001 */
#define DMA_STATUSINT_BLOCK_SHIFT                          (1U)
#define DMA_STATUSINT_BLOCK_MASK                           (0x1U << DMA_STATUSINT_BLOCK_SHIFT)                          /* 0x00000002 */
#define DMA_STATUSINT_SRCT_SHIFT                           (2U)
#define DMA_STATUSINT_SRCT_MASK                            (0x1U << DMA_STATUSINT_SRCT_SHIFT)                           /* 0x00000004 */
#define DMA_STATUSINT_DSTT_SHIFT                           (3U)
#define DMA_STATUSINT_DSTT_MASK                            (0x1U << DMA_STATUSINT_DSTT_SHIFT)                           /* 0x00000008 */
#define DMA_STATUSINT_ERR_SHIFT                            (4U)
#define DMA_STATUSINT_ERR_MASK                             (0x1U << DMA_STATUSINT_ERR_SHIFT)                            /* 0x00000010 */
/* DMACFGREG */
#define DMA_DMACFGREG_OFFSET                               (0x398U)
#define DMA_DMACFGREG_DMA_EN_SHIFT                         (0U)
#define DMA_DMACFGREG_DMA_EN_MASK                          (0x1U << DMA_DMACFGREG_DMA_EN_SHIFT)                         /* 0x00000001 */
/* CHENREG */
#define DMA_CHENREG_OFFSET                                 (0x3A0U)
#define DMA_CHENREG_CH_EN_SHIFT                            (0U)
#define DMA_CHENREG_CH_EN_MASK                             (0x7U << DMA_CHENREG_CH_EN_SHIFT)                            /* 0x00000007 */
#define DMA_CHENREG_CH_EN_WE_SHIFT                         (8U)
#define DMA_CHENREG_CH_EN_WE_MASK                          (0x7U << DMA_CHENREG_CH_EN_WE_SHIFT)                         /* 0x00000700 */
/******************************************FSPI******************************************/
/* CTRL0 */
#define FSPI_CTRL0_OFFSET                                  (0x0U)
#define FSPI_CTRL0_SPIM_SHIFT                              (0U)
#define FSPI_CTRL0_SPIM_MASK                               (0x1U << FSPI_CTRL0_SPIM_SHIFT)                              /* 0x00000001 */
#define FSPI_CTRL0_SHIFTPHASE_SHIFT                        (1U)
#define FSPI_CTRL0_SHIFTPHASE_MASK                         (0x1U << FSPI_CTRL0_SHIFTPHASE_SHIFT)                        /* 0x00000002 */
#define FSPI_CTRL0_IDLE_CYCLE_SHIFT                        (4U)
#define FSPI_CTRL0_IDLE_CYCLE_MASK                         (0xFU << FSPI_CTRL0_IDLE_CYCLE_SHIFT)                        /* 0x000000F0 */
#define FSPI_CTRL0_CMDB_SHIFT                              (8U)
#define FSPI_CTRL0_CMDB_MASK                               (0x3U << FSPI_CTRL0_CMDB_SHIFT)                              /* 0x00000300 */
#define FSPI_CTRL0_ADRB_SHIFT                              (10U)
#define FSPI_CTRL0_ADRB_MASK                               (0x3U << FSPI_CTRL0_ADRB_SHIFT)                              /* 0x00000C00 */
#define FSPI_CTRL0_DATB_SHIFT                              (12U)
#define FSPI_CTRL0_DATB_MASK                               (0x3U << FSPI_CTRL0_DATB_SHIFT)                              /* 0x00003000 */
/* IMR */
#define FSPI_IMR_OFFSET                                    (0x4U)
#define FSPI_IMR_RXFM_SHIFT                                (0U)
#define FSPI_IMR_RXFM_MASK                                 (0x1U << FSPI_IMR_RXFM_SHIFT)                                /* 0x00000001 */
#define FSPI_IMR_RXUM_SHIFT                                (1U)
#define FSPI_IMR_RXUM_MASK                                 (0x1U << FSPI_IMR_RXUM_SHIFT)                                /* 0x00000002 */
#define FSPI_IMR_TXOM_SHIFT                                (2U)
#define FSPI_IMR_TXOM_MASK                                 (0x1U << FSPI_IMR_TXOM_SHIFT)                                /* 0x00000004 */
#define FSPI_IMR_TXEM_SHIFT                                (3U)
#define FSPI_IMR_TXEM_MASK                                 (0x1U << FSPI_IMR_TXEM_SHIFT)                                /* 0x00000008 */
#define FSPI_IMR_TRANSM_SHIFT                              (4U)
#define FSPI_IMR_TRANSM_MASK                               (0x1U << FSPI_IMR_TRANSM_SHIFT)                              /* 0x00000010 */
#define FSPI_IMR_AHBM_SHIFT                                (5U)
#define FSPI_IMR_AHBM_MASK                                 (0x1U << FSPI_IMR_AHBM_SHIFT)                                /* 0x00000020 */
#define FSPI_IMR_NSPIM_SHIFT                               (6U)
#define FSPI_IMR_NSPIM_MASK                                (0x1U << FSPI_IMR_NSPIM_SHIFT)                               /* 0x00000040 */
#define FSPI_IMR_DMAM_SHIFT                                (7U)
#define FSPI_IMR_DMAM_MASK                                 (0x1U << FSPI_IMR_DMAM_SHIFT)                                /* 0x00000080 */
/* ICLR */
#define FSPI_ICLR_OFFSET                                   (0x8U)
#define FSPI_ICLR_RXFC_SHIFT                               (0U)
#define FSPI_ICLR_RXFC_MASK                                (0x1U << FSPI_ICLR_RXFC_SHIFT)                               /* 0x00000001 */
#define FSPI_ICLR_RXUC_SHIFT                               (1U)
#define FSPI_ICLR_RXUC_MASK                                (0x1U << FSPI_ICLR_RXUC_SHIFT)                               /* 0x00000002 */
#define FSPI_ICLR_TXOC_SHIFT                               (2U)
#define FSPI_ICLR_TXOC_MASK                                (0x1U << FSPI_ICLR_TXOC_SHIFT)                               /* 0x00000004 */
#define FSPI_ICLR_TXEC_SHIFT                               (3U)
#define FSPI_ICLR_TXEC_MASK                                (0x1U << FSPI_ICLR_TXEC_SHIFT)                               /* 0x00000008 */
#define FSPI_ICLR_TRANSC_SHIFT                             (4U)
#define FSPI_ICLR_TRANSC_MASK                              (0x1U << FSPI_ICLR_TRANSC_SHIFT)                             /* 0x00000010 */
#define FSPI_ICLR_AHBC_SHIFT                               (5U)
#define FSPI_ICLR_AHBC_MASK                                (0x1U << FSPI_ICLR_AHBC_SHIFT)                               /* 0x00000020 */
#define FSPI_ICLR_NSPIC_SHIFT                              (6U)
#define FSPI_ICLR_NSPIC_MASK                               (0x1U << FSPI_ICLR_NSPIC_SHIFT)                              /* 0x00000040 */
#define FSPI_ICLR_DMAC_SHIFT                               (7U)
#define FSPI_ICLR_DMAC_MASK                                (0x1U << FSPI_ICLR_DMAC_SHIFT)                               /* 0x00000080 */
/* FTLR */
#define FSPI_FTLR_OFFSET                                   (0xCU)
#define FSPI_FTLR_TXFTLR_SHIFT                             (0U)
#define FSPI_FTLR_TXFTLR_MASK                              (0xFFU << FSPI_FTLR_TXFTLR_SHIFT)                            /* 0x000000FF */
#define FSPI_FTLR_RXFTLR_SHIFT                             (8U)
#define FSPI_FTLR_RXFTLR_MASK                              (0xFFU << FSPI_FTLR_RXFTLR_SHIFT)                            /* 0x0000FF00 */
/* RCVR */
#define FSPI_RCVR_OFFSET                                   (0x10U)
#define FSPI_RCVR_RCVR_SHIFT                               (0U)
#define FSPI_RCVR_RCVR_MASK                                (0x1U << FSPI_RCVR_RCVR_SHIFT)                               /* 0x00000001 */
/* AX0 */
#define FSPI_AX0_OFFSET                                    (0x14U)
#define FSPI_AX0_AX_SHIFT                                  (0U)
#define FSPI_AX0_AX_MASK                                   (0xFFU << FSPI_AX0_AX_SHIFT)                                 /* 0x000000FF */
/* ABIT0 */
#define FSPI_ABIT0_OFFSET                                  (0x18U)
#define FSPI_ABIT0_ABIT_SHIFT                              (0U)
#define FSPI_ABIT0_ABIT_MASK                               (0x1FU << FSPI_ABIT0_ABIT_SHIFT)                             /* 0x0000001F */
/* ISR */
#define FSPI_ISR_OFFSET                                    (0x1CU)
#define FSPI_ISR_RXFS_SHIFT                                (0U)
#define FSPI_ISR_RXFS_MASK                                 (0x1U << FSPI_ISR_RXFS_SHIFT)                                /* 0x00000001 */
#define FSPI_ISR_RXUS_SHIFT                                (1U)
#define FSPI_ISR_RXUS_MASK                                 (0x1U << FSPI_ISR_RXUS_SHIFT)                                /* 0x00000002 */
#define FSPI_ISR_TXOS_SHIFT                                (2U)
#define FSPI_ISR_TXOS_MASK                                 (0x1U << FSPI_ISR_TXOS_SHIFT)                                /* 0x00000004 */
#define FSPI_ISR_TXES_SHIFT                                (3U)
#define FSPI_ISR_TXES_MASK                                 (0x1U << FSPI_ISR_TXES_SHIFT)                                /* 0x00000008 */
#define FSPI_ISR_TRANSS_SHIFT                              (4U)
#define FSPI_ISR_TRANSS_MASK                               (0x1U << FSPI_ISR_TRANSS_SHIFT)                              /* 0x00000010 */
#define FSPI_ISR_AHBS_SHIFT                                (5U)
#define FSPI_ISR_AHBS_MASK                                 (0x1U << FSPI_ISR_AHBS_SHIFT)                                /* 0x00000020 */
#define FSPI_ISR_NSPIS_SHIFT                               (6U)
#define FSPI_ISR_NSPIS_MASK                                (0x1U << FSPI_ISR_NSPIS_SHIFT)                               /* 0x00000040 */
#define FSPI_ISR_DMAS_SHIFT                                (7U)
#define FSPI_ISR_DMAS_MASK                                 (0x1U << FSPI_ISR_DMAS_SHIFT)                                /* 0x00000080 */
/* FSR */
#define FSPI_FSR_OFFSET                                    (0x20U)
#define FSPI_FSR_TXFS_SHIFT                                (0U)
#define FSPI_FSR_TXFS_MASK                                 (0x1U << FSPI_FSR_TXFS_SHIFT)                                /* 0x00000001 */
#define FSPI_FSR_TXES_SHIFT                                (1U)
#define FSPI_FSR_TXES_MASK                                 (0x1U << FSPI_FSR_TXES_SHIFT)                                /* 0x00000002 */
#define FSPI_FSR_RXES_SHIFT                                (2U)
#define FSPI_FSR_RXES_MASK                                 (0x1U << FSPI_FSR_RXES_SHIFT)                                /* 0x00000004 */
#define FSPI_FSR_RXFS_SHIFT                                (3U)
#define FSPI_FSR_RXFS_MASK                                 (0x1U << FSPI_FSR_RXFS_SHIFT)                                /* 0x00000008 */
#define FSPI_FSR_TXWLVL_SHIFT                              (8U)
#define FSPI_FSR_TXWLVL_MASK                               (0x1FU << FSPI_FSR_TXWLVL_SHIFT)                             /* 0x00001F00 */
#define FSPI_FSR_RXWLVL_SHIFT                              (16U)
#define FSPI_FSR_RXWLVL_MASK                               (0x1FU << FSPI_FSR_RXWLVL_SHIFT)                             /* 0x001F0000 */
/* SR */
#define FSPI_SR_OFFSET                                     (0x24U)
#define FSPI_SR                                            (0x0U)
#define FSPI_SR_SR_SHIFT                                   (0U)
#define FSPI_SR_SR_MASK                                    (0x1U << FSPI_SR_SR_SHIFT)                                   /* 0x00000001 */
/* RISR */
#define FSPI_RISR_OFFSET                                   (0x28U)
#define FSPI_RISR                                          (0x0U)
#define FSPI_RISR_RXFS_SHIFT                               (0U)
#define FSPI_RISR_RXFS_MASK                                (0x1U << FSPI_RISR_RXFS_SHIFT)                               /* 0x00000001 */
#define FSPI_RISR_RXUS_SHIFT                               (1U)
#define FSPI_RISR_RXUS_MASK                                (0x1U << FSPI_RISR_RXUS_SHIFT)                               /* 0x00000002 */
#define FSPI_RISR_TXOS_SHIFT                               (2U)
#define FSPI_RISR_TXOS_MASK                                (0x1U << FSPI_RISR_TXOS_SHIFT)                               /* 0x00000004 */
#define FSPI_RISR_TXES_SHIFT                               (3U)
#define FSPI_RISR_TXES_MASK                                (0x1U << FSPI_RISR_TXES_SHIFT)                               /* 0x00000008 */
#define FSPI_RISR_TRANSS_SHIFT                             (4U)
#define FSPI_RISR_TRANSS_MASK                              (0x1U << FSPI_RISR_TRANSS_SHIFT)                             /* 0x00000010 */
#define FSPI_RISR_AHBS_SHIFT                               (5U)
#define FSPI_RISR_AHBS_MASK                                (0x1U << FSPI_RISR_AHBS_SHIFT)                               /* 0x00000020 */
#define FSPI_RISR_NSPIS_SHIFT                              (6U)
#define FSPI_RISR_NSPIS_MASK                               (0x1U << FSPI_RISR_NSPIS_SHIFT)                              /* 0x00000040 */
#define FSPI_RISR_DMAS_SHIFT                               (7U)
#define FSPI_RISR_DMAS_MASK                                (0x1U << FSPI_RISR_DMAS_SHIFT)                               /* 0x00000080 */
/* VER */
#define FSPI_VER_OFFSET                                    (0x2CU)
#define FSPI_VER                                           (0x5U)
#define FSPI_VER_VER_SHIFT                                 (0U)
#define FSPI_VER_VER_MASK                                  (0xFFFFU << FSPI_VER_VER_SHIFT)                              /* 0x0000FFFF */
/* QOP */
#define FSPI_QOP_OFFSET                                    (0x30U)
#define FSPI_QOP_SO123_SHIFT                               (0U)
#define FSPI_QOP_SO123_MASK                                (0x1U << FSPI_QOP_SO123_SHIFT)                               /* 0x00000001 */
#define FSPI_QOP_SO123BP_SHIFT                             (1U)
#define FSPI_QOP_SO123BP_MASK                              (0x1U << FSPI_QOP_SO123BP_SHIFT)                             /* 0x00000002 */
/* EXT_CTRL */
#define FSPI_EXT_CTRL_OFFSET                               (0x34U)
#define FSPI_EXT_CTRL_CS_DESEL_CTRL_SHIFT                  (0U)
#define FSPI_EXT_CTRL_CS_DESEL_CTRL_MASK                   (0xFU << FSPI_EXT_CTRL_CS_DESEL_CTRL_SHIFT)                  /* 0x0000000F */
#define FSPI_EXT_CTRL_SWITCH_IO_DUMM_CNT_SHIFT             (4U)
#define FSPI_EXT_CTRL_SWITCH_IO_DUMM_CNT_MASK              (0xFU << FSPI_EXT_CTRL_SWITCH_IO_DUMM_CNT_SHIFT)             /* 0x000000F0 */
#define FSPI_EXT_CTRL_SWITCH_IO_O2I_CNT_SHIFT              (8U)
#define FSPI_EXT_CTRL_SWITCH_IO_O2I_CNT_MASK               (0xFU << FSPI_EXT_CTRL_SWITCH_IO_O2I_CNT_SHIFT)              /* 0x00000F00 */
#define FSPI_EXT_CTRL_TRANS_INT_MODE_SHIFT                 (13U)
#define FSPI_EXT_CTRL_TRANS_INT_MODE_MASK                  (0x1U << FSPI_EXT_CTRL_TRANS_INT_MODE_SHIFT)                 /* 0x00002000 */
#define FSPI_EXT_CTRL_SR_GEN_MODE_SHIFT                    (14U)
#define FSPI_EXT_CTRL_SR_GEN_MODE_MASK                     (0x1U << FSPI_EXT_CTRL_SR_GEN_MODE_SHIFT)                    /* 0x00004000 */
/* DLL_CTRL0 */
#define FSPI_DLL_CTRL0_OFFSET                              (0x3CU)
#define FSPI_DLL_CTRL0_SMP_DLL_CFG_SHIFT                   (0U)
#define FSPI_DLL_CTRL0_SMP_DLL_CFG_MASK                    (0x1FFU << FSPI_DLL_CTRL0_SMP_DLL_CFG_SHIFT)                 /* 0x000001FF */
#define FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT                  (15U)
#define FSPI_DLL_CTRL0_SCLK_SMP_SEL_MASK                   (0x1U << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT)                  /* 0x00008000 */
/* EXT_AX */
#define FSPI_EXT_AX_OFFSET                                 (0x44U)
#define FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT                    (0U)
#define FSPI_EXT_AX_AX_CANCEL_PAT_MASK                     (0xFFU << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT)                   /* 0x000000FF */
#define FSPI_EXT_AX_AX_SETUP_PAT_SHIFT                     (8U)
#define FSPI_EXT_AX_AX_SETUP_PAT_MASK                      (0xFFU << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT)                    /* 0x0000FF00 */
/* SCLK_INATM_CNT */
#define FSPI_SCLK_INATM_CNT_OFFSET                         (0x48U)
#define FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_SHIFT           (0U)
#define FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_MASK            (0xFFFFFFFFU << FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_SHIFT)    /* 0xFFFFFFFF */
/* XMMC_WCMD0 */
#define FSPI_XMMC_WCMD0_OFFSET                             (0x50U)
#define FSPI_XMMC_WCMD0_CMD_SHIFT                          (0U)
#define FSPI_XMMC_WCMD0_CMD_MASK                           (0xFFU << FSPI_XMMC_WCMD0_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_WCMD0_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_WCMD0_DUMM_MASK                          (0xFU << FSPI_XMMC_WCMD0_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_WCMD0_CONT_SHIFT                         (13U)
#define FSPI_XMMC_WCMD0_CONT_MASK                          (0x1U << FSPI_XMMC_WCMD0_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_WCMD0_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_WCMD0_ADDRB_MASK                         (0x3U << FSPI_XMMC_WCMD0_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_RCMD0 */
#define FSPI_XMMC_RCMD0_OFFSET                             (0x54U)
#define FSPI_XMMC_RCMD0_CMD_SHIFT                          (0U)
#define FSPI_XMMC_RCMD0_CMD_MASK                           (0xFFU << FSPI_XMMC_RCMD0_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_RCMD0_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_RCMD0_DUMM_MASK                          (0xFU << FSPI_XMMC_RCMD0_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_RCMD0_CONT_SHIFT                         (13U)
#define FSPI_XMMC_RCMD0_CONT_MASK                          (0x1U << FSPI_XMMC_RCMD0_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_RCMD0_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_RCMD0_ADDRB_MASK                         (0x3U << FSPI_XMMC_RCMD0_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_CTRL */
#define FSPI_XMMC_CTRL_OFFSET                              (0x58U)
#define FSPI_XMMC_CTRL_DEV_HWEN_SHIFT                      (5U)
#define FSPI_XMMC_CTRL_DEV_HWEN_MASK                       (0x1U << FSPI_XMMC_CTRL_DEV_HWEN_SHIFT)                      /* 0x00000020 */
#define FSPI_XMMC_CTRL_PFT_EN_SHIFT                        (6U)
#define FSPI_XMMC_CTRL_PFT_EN_MASK                         (0x1U << FSPI_XMMC_CTRL_PFT_EN_SHIFT)                        /* 0x00000040 */
/* MODE */
#define FSPI_MODE_OFFSET                                   (0x5CU)
#define FSPI_MODE_XMMC_MODE_EN_SHIFT                       (0U)
#define FSPI_MODE_XMMC_MODE_EN_MASK                        (0x1U << FSPI_MODE_XMMC_MODE_EN_SHIFT)                       /* 0x00000001 */
/* DEVRGN */
#define FSPI_DEVRGN_OFFSET                                 (0x60U)
#define FSPI_DEVRGN_RSIZE_SHIFT                            (0U)
#define FSPI_DEVRGN_RSIZE_MASK                             (0x1FU << FSPI_DEVRGN_RSIZE_SHIFT)                           /* 0x0000001F */
#define FSPI_DEVRGN_DEC_CTRL_SHIFT                         (8U)
#define FSPI_DEVRGN_DEC_CTRL_MASK                          (0x3U << FSPI_DEVRGN_DEC_CTRL_SHIFT)                         /* 0x00000300 */
/* DEVSIZE0 */
#define FSPI_DEVSIZE0_OFFSET                               (0x64U)
#define FSPI_DEVSIZE0_DSIZE_SHIFT                          (0U)
#define FSPI_DEVSIZE0_DSIZE_MASK                           (0x1FU << FSPI_DEVSIZE0_DSIZE_SHIFT)                         /* 0x0000001F */
/* TME0 */
#define FSPI_TME0_OFFSET                                   (0x68U)
#define FSPI_TME0_SCLK_INATM_EN_SHIFT                      (1U)
#define FSPI_TME0_SCLK_INATM_EN_MASK                       (0x1U << FSPI_TME0_SCLK_INATM_EN_SHIFT)                      /* 0x00000002 */
/* XMMC_RX_WTMRK */
#define FSPI_XMMC_RX_WTMRK_OFFSET                          (0x70U)
#define FSPI_XMMC_RX_WTMRK_RX_FULL_WTMRK_SHIFT             (0U)
#define FSPI_XMMC_RX_WTMRK_RX_FULL_WTMRK_MASK              (0xFFU << FSPI_XMMC_RX_WTMRK_RX_FULL_WTMRK_SHIFT)            /* 0x000000FF */
/* DMATR */
#define FSPI_DMATR_OFFSET                                  (0x80U)
#define FSPI_DMATR_DMATR_SHIFT                             (0U)
#define FSPI_DMATR_DMATR_MASK                              (0x1U << FSPI_DMATR_DMATR_SHIFT)                             /* 0x00000001 */
/* DMAADDR */
#define FSPI_DMAADDR_OFFSET                                (0x84U)
#define FSPI_DMAADDR_DMAADDR_SHIFT                         (0U)
#define FSPI_DMAADDR_DMAADDR_MASK                          (0xFFFFFFFFU << FSPI_DMAADDR_DMAADDR_SHIFT)                  /* 0xFFFFFFFF */
/* LEN_CTRL */
#define FSPI_LEN_CTRL_OFFSET                               (0x88U)
#define FSPI_LEN_CTRL_TRB_SEL_SHIFT                        (0U)
#define FSPI_LEN_CTRL_TRB_SEL_MASK                         (0x1U << FSPI_LEN_CTRL_TRB_SEL_SHIFT)                        /* 0x00000001 */
/* LEN_EXT */
#define FSPI_LEN_EXT_OFFSET                                (0x8CU)
#define FSPI_LEN_EXT_TRB_EXT_SHIFT                         (0U)
#define FSPI_LEN_EXT_TRB_EXT_MASK                          (0xFFFFFFFFU << FSPI_LEN_EXT_TRB_EXT_SHIFT)                  /* 0xFFFFFFFF */
/* XMMCSR */
#define FSPI_XMMCSR_OFFSET                                 (0x94U)
#define FSPI_XMMCSR_SLOPOVER0_SHIFT                        (0U)
#define FSPI_XMMCSR_SLOPOVER0_MASK                         (0x1U << FSPI_XMMCSR_SLOPOVER0_SHIFT)                        /* 0x00000001 */
#define FSPI_XMMCSR_SLOPOVER1_SHIFT                        (1U)
#define FSPI_XMMCSR_SLOPOVER1_MASK                         (0x1U << FSPI_XMMCSR_SLOPOVER1_SHIFT)                        /* 0x00000002 */
/* CMD */
#define FSPI_CMD_OFFSET                                    (0x100U)
#define FSPI_CMD_CMD_SHIFT                                 (0U)
#define FSPI_CMD_CMD_MASK                                  (0xFFU << FSPI_CMD_CMD_SHIFT)                                /* 0x000000FF */
#define FSPI_CMD_DUMM_SHIFT                                (8U)
#define FSPI_CMD_DUMM_MASK                                 (0xFU << FSPI_CMD_DUMM_SHIFT)                                /* 0x00000F00 */
#define FSPI_CMD_WR_SHIFT                                  (12U)
#define FSPI_CMD_WR_MASK                                   (0x1U << FSPI_CMD_WR_SHIFT)                                  /* 0x00001000 */
#define FSPI_CMD_CONT_SHIFT                                (13U)
#define FSPI_CMD_CONT_MASK                                 (0x1U << FSPI_CMD_CONT_SHIFT)                                /* 0x00002000 */
#define FSPI_CMD_ADDRB_SHIFT                               (14U)
#define FSPI_CMD_ADDRB_MASK                                (0x3U << FSPI_CMD_ADDRB_SHIFT)                               /* 0x0000C000 */
#define FSPI_CMD_TRB_SHIFT                                 (16U)
#define FSPI_CMD_TRB_MASK                                  (0x3FFFU << FSPI_CMD_TRB_SHIFT)                              /* 0x3FFF0000 */
#define FSPI_CMD_CS_SHIFT                                  (30U)
#define FSPI_CMD_CS_MASK                                   (0x3U << FSPI_CMD_CS_SHIFT)                                  /* 0xC0000000 */
/* ADDR */
#define FSPI_ADDR_OFFSET                                   (0x104U)
#define FSPI_ADDR_ADDR_SHIFT                               (0U)
#define FSPI_ADDR_ADDR_MASK                                (0xFFFFFFFFU << FSPI_ADDR_ADDR_SHIFT)                        /* 0xFFFFFFFF */
/* DATA */
#define FSPI_DATA_OFFSET                                   (0x108U)
#define FSPI_DATA_DATA_SHIFT                               (0U)
#define FSPI_DATA_DATA_MASK                                (0xFFFFFFFFU << FSPI_DATA_DATA_SHIFT)                        /* 0xFFFFFFFF */
/* CTRL1 */
#define FSPI_CTRL1_OFFSET                                  (0x200U)
#define FSPI_CTRL1_SPIM_SHIFT                              (0U)
#define FSPI_CTRL1_SPIM_MASK                               (0x1U << FSPI_CTRL1_SPIM_SHIFT)                              /* 0x00000001 */
#define FSPI_CTRL1_SHIFTPHASE_SHIFT                        (1U)
#define FSPI_CTRL1_SHIFTPHASE_MASK                         (0x1U << FSPI_CTRL1_SHIFTPHASE_SHIFT)                        /* 0x00000002 */
#define FSPI_CTRL1_IDLE_CYCLE_SHIFT                        (4U)
#define FSPI_CTRL1_IDLE_CYCLE_MASK                         (0xFU << FSPI_CTRL1_IDLE_CYCLE_SHIFT)                        /* 0x000000F0 */
#define FSPI_CTRL1_CMDB_SHIFT                              (8U)
#define FSPI_CTRL1_CMDB_MASK                               (0x3U << FSPI_CTRL1_CMDB_SHIFT)                              /* 0x00000300 */
#define FSPI_CTRL1_ADRB_SHIFT                              (10U)
#define FSPI_CTRL1_ADRB_MASK                               (0x3U << FSPI_CTRL1_ADRB_SHIFT)                              /* 0x00000C00 */
#define FSPI_CTRL1_DATB_SHIFT                              (12U)
#define FSPI_CTRL1_DATB_MASK                               (0x3U << FSPI_CTRL1_DATB_SHIFT)                              /* 0x00003000 */
/* AX1 */
#define FSPI_AX1_OFFSET                                    (0x214U)
#define FSPI_AX1_AX_SHIFT                                  (0U)
#define FSPI_AX1_AX_MASK                                   (0xFFU << FSPI_AX1_AX_SHIFT)                                 /* 0x000000FF */
/* ABIT1 */
#define FSPI_ABIT1_OFFSET                                  (0x218U)
#define FSPI_ABIT1_ABIT_SHIFT                              (0U)
#define FSPI_ABIT1_ABIT_MASK                               (0x1FU << FSPI_ABIT1_ABIT_SHIFT)                             /* 0x0000001F */
/* DLL_CTRL1 */
#define FSPI_DLL_CTRL1_OFFSET                              (0x23CU)
#define FSPI_DLL_CTRL1_SMP_DLL_CFG_SHIFT                   (0U)
#define FSPI_DLL_CTRL1_SMP_DLL_CFG_MASK                    (0x1FFU << FSPI_DLL_CTRL1_SMP_DLL_CFG_SHIFT)                 /* 0x000001FF */
#define FSPI_DLL_CTRL1_SCLK_SMP_SEL_SHIFT                  (15U)
#define FSPI_DLL_CTRL1_SCLK_SMP_SEL_MASK                   (0x1U << FSPI_DLL_CTRL1_SCLK_SMP_SEL_SHIFT)                  /* 0x00008000 */
/* XMMC_WCMD1 */
#define FSPI_XMMC_WCMD1_OFFSET                             (0x250U)
#define FSPI_XMMC_WCMD1_CMD_SHIFT                          (0U)
#define FSPI_XMMC_WCMD1_CMD_MASK                           (0xFFU << FSPI_XMMC_WCMD1_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_WCMD1_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_WCMD1_DUMM_MASK                          (0xFU << FSPI_XMMC_WCMD1_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_WCMD1_CONT_SHIFT                         (13U)
#define FSPI_XMMC_WCMD1_CONT_MASK                          (0x1U << FSPI_XMMC_WCMD1_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_WCMD1_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_WCMD1_ADDRB_MASK                         (0x3U << FSPI_XMMC_WCMD1_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_RCMD1 */
#define FSPI_XMMC_RCMD1_OFFSET                             (0x254U)
#define FSPI_XMMC_RCMD1_CMD_SHIFT                          (0U)
#define FSPI_XMMC_RCMD1_CMD_MASK                           (0xFFU << FSPI_XMMC_RCMD1_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_RCMD1_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_RCMD1_DUMM_MASK                          (0xFU << FSPI_XMMC_RCMD1_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_RCMD1_CONT_SHIFT                         (13U)
#define FSPI_XMMC_RCMD1_CONT_MASK                          (0x1U << FSPI_XMMC_RCMD1_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_RCMD1_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_RCMD1_ADDRB_MASK                         (0x3U << FSPI_XMMC_RCMD1_ADDRB_SHIFT)                        /* 0x0000C000 */
/* DEVSIZE1 */
#define FSPI_DEVSIZE1_OFFSET                               (0x264U)
#define FSPI_DEVSIZE1_DSIZE_SHIFT                          (0U)
#define FSPI_DEVSIZE1_DSIZE_MASK                           (0x1FU << FSPI_DEVSIZE1_DSIZE_SHIFT)                         /* 0x0000001F */
/* TME1 */
#define FSPI_TME1_OFFSET                                   (0x268U)
#define FSPI_TME1_SCLK_INATM_EN_SHIFT                      (1U)
#define FSPI_TME1_SCLK_INATM_EN_MASK                       (0x1U << FSPI_TME1_SCLK_INATM_EN_SHIFT)                      /* 0x00000002 */
/*****************************************CKCAL******************************************/
/* CTRL */
#define CKCAL_CTRL_OFFSET                                  (0x0U)
#define CKCAL_CTRL_START_SHIFT                             (0U)
#define CKCAL_CTRL_START_MASK                              (0x1U << CKCAL_CTRL_START_SHIFT)                             /* 0x00000001 */
/* RESULT_CTRL */
#define CKCAL_RESULT_CTRL_OFFSET                           (0x4U)
#define CKCAL_RESULT_CTRL_RESULT_SEL_SHIFT                 (0U)
#define CKCAL_RESULT_CTRL_RESULT_SEL_MASK                  (0x1U << CKCAL_RESULT_CTRL_RESULT_SEL_SHIFT)                 /* 0x00000001 */
/* STATE */
#define CKCAL_STATE_OFFSET                                 (0x8U)
#define CKCAL_STATE_DONE_SHIFT                             (0U)
#define CKCAL_STATE_DONE_MASK                              (0x1U << CKCAL_STATE_DONE_SHIFT)                             /* 0x00000001 */
#define CKCAL_STATE_ERROR_SHIFT                            (1U)
#define CKCAL_STATE_ERROR_MASK                             (0x1U << CKCAL_STATE_ERROR_SHIFT)                            /* 0x00000002 */
/* SEL */
#define CKCAL_SEL_OFFSET                                   (0xCU)
#define CKCAL_SEL_ALGO_SEL_SHIFT                           (0U)
#define CKCAL_SEL_ALGO_SEL_MASK                            (0x3U << CKCAL_SEL_ALGO_SEL_SHIFT)                           /* 0x00000003 */
/* BASE_ADDR */
#define CKCAL_BASE_ADDR_OFFSET                             (0x10U)
#define CKCAL_BASE_ADDR_BASE_ADDR_SHIFT                    (0U)
#define CKCAL_BASE_ADDR_BASE_ADDR_MASK                     (0xFFFFFFFFU << CKCAL_BASE_ADDR_BASE_ADDR_SHIFT)             /* 0xFFFFFFFF */
/* LEN */
#define CKCAL_LEN_OFFSET                                   (0x14U)
#define CKCAL_LEN_LENGTH_SHIFT                             (0U)
#define CKCAL_LEN_LENGTH_MASK                              (0x3FFFFFFU << CKCAL_LEN_LENGTH_SHIFT)                       /* 0x03FFFFFF */
/* INI_DATA */
#define CKCAL_INI_DATA_OFFSET                              (0x18U)
#define CKCAL_INI_DATA_INITIAL_DATA_SHIFT                  (0U)
#define CKCAL_INI_DATA_INITIAL_DATA_MASK                   (0xFFFFFFFFU << CKCAL_INI_DATA_INITIAL_DATA_SHIFT)           /* 0xFFFFFFFF */
/* BURST */
#define CKCAL_BURST_OFFSET                                 (0x1CU)
#define CKCAL_BURST_BURST_TYPE_SHIFT                       (0U)
#define CKCAL_BURST_BURST_TYPE_MASK                        (0x3U << CKCAL_BURST_BURST_TYPE_SHIFT)                       /* 0x00000003 */
/* RESULT */
#define CKCAL_RESULT_OFFSET                                (0x20U)
#define CKCAL_RESULT_RESULT_SHIFT                          (0U)
#define CKCAL_RESULT_RESULT_MASK                           (0xFFFFFFFFU << CKCAL_RESULT_RESULT_SHIFT)                   /* 0xFFFFFFFF */
/* INT_EN */
#define CKCAL_INT_EN_OFFSET                                (0x24U)
#define CKCAL_INT_EN_DONE_INT_EN_SHIFT                     (0U)
#define CKCAL_INT_EN_DONE_INT_EN_MASK                      (0x1U << CKCAL_INT_EN_DONE_INT_EN_SHIFT)                     /* 0x00000001 */
#define CKCAL_INT_EN_ERROR_INT_EN_SHIFT                    (1U)
#define CKCAL_INT_EN_ERROR_INT_EN_MASK                     (0x1U << CKCAL_INT_EN_ERROR_INT_EN_SHIFT)                    /* 0x00000002 */
/****************************************JPEG_ENC****************************************/
/* SWREG_0 */
#define JPEG_ENC_SWREG_0_OFFSET                            (0x0U)
#define JPEG_ENC_SWREG_0_SW_JPEG_LUMA_QUANT1_SHIFT         (0U)
#define JPEG_ENC_SWREG_0_SW_JPEG_LUMA_QUANT1_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_0_SW_JPEG_LUMA_QUANT1_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_1 */
#define JPEG_ENC_SWREG_1_OFFSET                            (0x4U)
#define JPEG_ENC_SWREG_1_SW_JPEG_LUMA_QUANT2_SHIFT         (0U)
#define JPEG_ENC_SWREG_1_SW_JPEG_LUMA_QUANT2_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_1_SW_JPEG_LUMA_QUANT2_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_2 */
#define JPEG_ENC_SWREG_2_OFFSET                            (0x8U)
#define JPEG_ENC_SWREG_2_SW_JPEG_LUMA_QUANT3_SHIFT         (0U)
#define JPEG_ENC_SWREG_2_SW_JPEG_LUMA_QUANT3_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_2_SW_JPEG_LUMA_QUANT3_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_3 */
#define JPEG_ENC_SWREG_3_OFFSET                            (0xCU)
#define JPEG_ENC_SWREG_3_SW_JPEG_LUMA_QUANT4_SHIFT         (0U)
#define JPEG_ENC_SWREG_3_SW_JPEG_LUMA_QUANT4_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_3_SW_JPEG_LUMA_QUANT4_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_4 */
#define JPEG_ENC_SWREG_4_OFFSET                            (0x10U)
#define JPEG_ENC_SWREG_4_SW_JPEG_LUMA_QUANT5_SHIFT         (0U)
#define JPEG_ENC_SWREG_4_SW_JPEG_LUMA_QUANT5_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_4_SW_JPEG_LUMA_QUANT5_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_5 */
#define JPEG_ENC_SWREG_5_OFFSET                            (0x14U)
#define JPEG_ENC_SWREG_5_SW_JPEG_LUMA_QUANT6_SHIFT         (0U)
#define JPEG_ENC_SWREG_5_SW_JPEG_LUMA_QUANT6_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_5_SW_JPEG_LUMA_QUANT6_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_6 */
#define JPEG_ENC_SWREG_6_OFFSET                            (0x18U)
#define JPEG_ENC_SWREG_6_SW_JPEG_LUMA_QUANT7_SHIFT         (0U)
#define JPEG_ENC_SWREG_6_SW_JPEG_LUMA_QUANT7_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_6_SW_JPEG_LUMA_QUANT7_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_7 */
#define JPEG_ENC_SWREG_7_OFFSET                            (0x1CU)
#define JPEG_ENC_SWREG_7_SW_JPEG_LUMA_QUANT8_SHIFT         (0U)
#define JPEG_ENC_SWREG_7_SW_JPEG_LUMA_QUANT8_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_7_SW_JPEG_LUMA_QUANT8_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_8 */
#define JPEG_ENC_SWREG_8_OFFSET                            (0x20U)
#define JPEG_ENC_SWREG_8_SW_JPEG_LUMA_QUANT9_SHIFT         (0U)
#define JPEG_ENC_SWREG_8_SW_JPEG_LUMA_QUANT9_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_8_SW_JPEG_LUMA_QUANT9_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_9 */
#define JPEG_ENC_SWREG_9_OFFSET                            (0x24U)
#define JPEG_ENC_SWREG_9_SW_JPEG_LUMA_QUANT10_SHIFT        (0U)
#define JPEG_ENC_SWREG_9_SW_JPEG_LUMA_QUANT10_MASK         (0xFFFFFFFFU << JPEG_ENC_SWREG_9_SW_JPEG_LUMA_QUANT10_SHIFT) /* 0xFFFFFFFF */
/* SWREG_10 */
#define JPEG_ENC_SWREG_10_OFFSET                           (0x28U)
#define JPEG_ENC_SWREG_10_SW_JPEG_LUMA_QUANT11_SHIFT       (0U)
#define JPEG_ENC_SWREG_10_SW_JPEG_LUMA_QUANT11_MASK        (0xFFFFFFFFU << JPEG_ENC_SWREG_10_SW_JPEG_LUMA_QUANT11_SHIFT) /* 0xFFFFFFFF */
/* SWREG_11 */
#define JPEG_ENC_SWREG_11_OFFSET                           (0x2CU)
#define JPEG_ENC_SWREG_11_SW_JPEG_LUMA_QUANT12_SHIFT       (0U)
#define JPEG_ENC_SWREG_11_SW_JPEG_LUMA_QUANT12_MASK        (0xFFFFFFFFU << JPEG_ENC_SWREG_11_SW_JPEG_LUMA_QUANT12_SHIFT) /* 0xFFFFFFFF */
/* SWREG_12 */
#define JPEG_ENC_SWREG_12_OFFSET                           (0x30U)
#define JPEG_ENC_SWREG_12_SW_JPEG_LUMA_QUANT13_SHIFT       (0U)
#define JPEG_ENC_SWREG_12_SW_JPEG_LUMA_QUANT13_MASK        (0xFFFFFFFFU << JPEG_ENC_SWREG_12_SW_JPEG_LUMA_QUANT13_SHIFT) /* 0xFFFFFFFF */
/* SWREG_13 */
#define JPEG_ENC_SWREG_13_OFFSET                           (0x34U)
#define JPEG_ENC_SWREG_13_SW_JPEG_LUMA_QUANT14_SHIFT       (0U)
#define JPEG_ENC_SWREG_13_SW_JPEG_LUMA_QUANT14_MASK        (0xFFFFFFFFU << JPEG_ENC_SWREG_13_SW_JPEG_LUMA_QUANT14_SHIFT) /* 0xFFFFFFFF */
/* SWREG_14 */
#define JPEG_ENC_SWREG_14_OFFSET                           (0x38U)
#define JPEG_ENC_SWREG_14_SW_JPEG_LUMA_QUANT15_SHIFT       (0U)
#define JPEG_ENC_SWREG_14_SW_JPEG_LUMA_QUANT15_MASK        (0xFFFFFFFFU << JPEG_ENC_SWREG_14_SW_JPEG_LUMA_QUANT15_SHIFT) /* 0xFFFFFFFF */
/* SWREG_15 */
#define JPEG_ENC_SWREG_15_OFFSET                           (0x3CU)
#define JPEG_ENC_SWREG_15_SW_JPEG_LUMA_QUANT16_SHIFT       (0U)
#define JPEG_ENC_SWREG_15_SW_JPEG_LUMA_QUANT16_MASK        (0xFFU << JPEG_ENC_SWREG_15_SW_JPEG_LUMA_QUANT16_SHIFT)      /* 0x000000FF */
/* SWREG_16 */
#define JPEG_ENC_SWREG_16_OFFSET                           (0x40U)
#define JPEG_ENC_SWREG_16_SW_JPEG_CHROMA_QUANT1_SHIFT      (0U)
#define JPEG_ENC_SWREG_16_SW_JPEG_CHROMA_QUANT1_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_16_SW_JPEG_CHROMA_QUANT1_SHIFT) /* 0xFFFFFFFF */
/* SWREG_17 */
#define JPEG_ENC_SWREG_17_OFFSET                           (0x44U)
#define JPEG_ENC_SWREG_17_SW_JPEG_CHROMA_QUANT2_SHIFT      (0U)
#define JPEG_ENC_SWREG_17_SW_JPEG_CHROMA_QUANT2_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_17_SW_JPEG_CHROMA_QUANT2_SHIFT) /* 0xFFFFFFFF */
/* SWREG_18 */
#define JPEG_ENC_SWREG_18_OFFSET                           (0x48U)
#define JPEG_ENC_SWREG_18_SW_JPEG_CHROMA_QUANT3_SHIFT      (0U)
#define JPEG_ENC_SWREG_18_SW_JPEG_CHROMA_QUANT3_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_18_SW_JPEG_CHROMA_QUANT3_SHIFT) /* 0xFFFFFFFF */
/* SWREG_19 */
#define JPEG_ENC_SWREG_19_OFFSET                           (0x4CU)
#define JPEG_ENC_SWREG_19_SW_JPEG_CHROMA_QUANT4_SHIFT      (0U)
#define JPEG_ENC_SWREG_19_SW_JPEG_CHROMA_QUANT4_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_19_SW_JPEG_CHROMA_QUANT4_SHIFT) /* 0xFFFFFFFF */
/* SWREG_20 */
#define JPEG_ENC_SWREG_20_OFFSET                           (0x50U)
#define JPEG_ENC_SWREG_20_SW_JPEG_CHROMA_QUANT5_SHIFT      (0U)
#define JPEG_ENC_SWREG_20_SW_JPEG_CHROMA_QUANT5_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_20_SW_JPEG_CHROMA_QUANT5_SHIFT) /* 0xFFFFFFFF */
/* SWREG_21 */
#define JPEG_ENC_SWREG_21_OFFSET                           (0x54U)
#define JPEG_ENC_SWREG_21_SW_JPEG_CHROMA_QUANT6_SHIFT      (0U)
#define JPEG_ENC_SWREG_21_SW_JPEG_CHROMA_QUANT6_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_21_SW_JPEG_CHROMA_QUANT6_SHIFT) /* 0xFFFFFFFF */
/* SWREG_22 */
#define JPEG_ENC_SWREG_22_OFFSET                           (0x58U)
#define JPEG_ENC_SWREG_22_SW_JPEG_CHROMA_QUANT7_SHIFT      (0U)
#define JPEG_ENC_SWREG_22_SW_JPEG_CHROMA_QUANT7_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_22_SW_JPEG_CHROMA_QUANT7_SHIFT) /* 0xFFFFFFFF */
/* SWREG_23 */
#define JPEG_ENC_SWREG_23_OFFSET                           (0x5CU)
#define JPEG_ENC_SWREG_23_SW_JPEG_CHROMA_QUANT8_SHIFT      (0U)
#define JPEG_ENC_SWREG_23_SW_JPEG_CHROMA_QUANT8_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_23_SW_JPEG_CHROMA_QUANT8_SHIFT) /* 0xFFFFFFFF */
/* SWREG_24 */
#define JPEG_ENC_SWREG_24_OFFSET                           (0x60U)
#define JPEG_ENC_SWREG_24_SW_JPEG_CHROMA_QUANT9_SHIFT      (0U)
#define JPEG_ENC_SWREG_24_SW_JPEG_CHROMA_QUANT9_MASK       (0xFFFFFFFFU << JPEG_ENC_SWREG_24_SW_JPEG_CHROMA_QUANT9_SHIFT) /* 0xFFFFFFFF */
/* SWREG_25 */
#define JPEG_ENC_SWREG_25_OFFSET                           (0x64U)
#define JPEG_ENC_SWREG_25_SW_JPEG_CHROMA_QUANT10_SHIFT     (0U)
#define JPEG_ENC_SWREG_25_SW_JPEG_CHROMA_QUANT10_MASK      (0xFFFFFFFFU << JPEG_ENC_SWREG_25_SW_JPEG_CHROMA_QUANT10_SHIFT) /* 0xFFFFFFFF */
/* SWREG_26 */
#define JPEG_ENC_SWREG_26_OFFSET                           (0x68U)
#define JPEG_ENC_SWREG_26_SW_JPEG_CHROMA_QUANT11_SHIFT     (0U)
#define JPEG_ENC_SWREG_26_SW_JPEG_CHROMA_QUANT11_MASK      (0x1FFU << JPEG_ENC_SWREG_26_SW_JPEG_CHROMA_QUANT11_SHIFT)   /* 0x000001FF */
/* SWREG_27 */
#define JPEG_ENC_SWREG_27_OFFSET                           (0x6CU)
#define JPEG_ENC_SWREG_27_SW_JPEG_CHROMA_QUANT11_SHIFT     (0U)
#define JPEG_ENC_SWREG_27_SW_JPEG_CHROMA_QUANT11_MASK      (0xFFFFFFFFU << JPEG_ENC_SWREG_27_SW_JPEG_CHROMA_QUANT11_SHIFT) /* 0xFFFFFFFF */
/* SWREG_28 */
#define JPEG_ENC_SWREG_28_OFFSET                           (0x70U)
#define JPEG_ENC_SWREG_28_SW_JPEG_CHROMA_QUANT13_SHIFT     (0U)
#define JPEG_ENC_SWREG_28_SW_JPEG_CHROMA_QUANT13_MASK      (0xFFFU << JPEG_ENC_SWREG_28_SW_JPEG_CHROMA_QUANT13_SHIFT)   /* 0x00000FFF */
/* SWREG_29 */
#define JPEG_ENC_SWREG_29_OFFSET                           (0x74U)
#define JPEG_ENC_SWREG_29_SW_JPEG_CHROMA_QUANT14_SHIFT     (0U)
#define JPEG_ENC_SWREG_29_SW_JPEG_CHROMA_QUANT14_MASK      (0xFFFFFFFFU << JPEG_ENC_SWREG_29_SW_JPEG_CHROMA_QUANT14_SHIFT) /* 0xFFFFFFFF */
/* SWREG_30 */
#define JPEG_ENC_SWREG_30_OFFSET                           (0x78U)
#define JPEG_ENC_SWREG_30_SW_JPEG_CHROMA_QUANT15_SHIFT     (0U)
#define JPEG_ENC_SWREG_30_SW_JPEG_CHROMA_QUANT15_MASK      (0xFFFU << JPEG_ENC_SWREG_30_SW_JPEG_CHROMA_QUANT15_SHIFT)   /* 0x00000FFF */
/* SWREG_31 */
#define JPEG_ENC_SWREG_31_OFFSET                           (0x7CU)
#define JPEG_ENC_SWREG_31_SW_JPEG_CHROMA_QUANT16_SHIFT     (0U)
#define JPEG_ENC_SWREG_31_SW_JPEG_CHROMA_QUANT16_MASK      (0xFFFFFFFFU << JPEG_ENC_SWREG_31_SW_JPEG_CHROMA_QUANT16_SHIFT) /* 0xFFFFFFFF */
/* SWREG_44 */
#define JPEG_ENC_SWREG_44_OFFSET                           (0xB0U)
#define JPEG_ENC_SWREG_44_INTRA_SLICE_BMP0_SHIFT           (0U)
#define JPEG_ENC_SWREG_44_INTRA_SLICE_BMP0_MASK            (0xFFFFFFFFU << JPEG_ENC_SWREG_44_INTRA_SLICE_BMP0_SHIFT)    /* 0xFFFFFFFF */
/* SWREG_45 */
#define JPEG_ENC_SWREG_45_OFFSET                           (0xB4U)
#define JPEG_ENC_SWREG_45_INTRA_SLICE_BMP1_SHIFT           (0U)
#define JPEG_ENC_SWREG_45_INTRA_SLICE_BMP1_MASK            (0xFFFFFFFFU << JPEG_ENC_SWREG_45_INTRA_SLICE_BMP1_SHIFT)    /* 0xFFFFFFFF */
/* SWREG_46 */
#define JPEG_ENC_SWREG_46_OFFSET                           (0xB8U)
#define JPEG_ENC_SWREG_46_INTRA_RIGHT_MB_AREA_SHIFT        (0U)
#define JPEG_ENC_SWREG_46_INTRA_RIGHT_MB_AREA_MASK         (0xFFU << JPEG_ENC_SWREG_46_INTRA_RIGHT_MB_AREA_SHIFT)       /* 0x000000FF */
#define JPEG_ENC_SWREG_46_INTRA_LEFT_MB_AREA_SHIFT         (8U)
#define JPEG_ENC_SWREG_46_INTRA_LEFT_MB_AREA_MASK          (0xFFU << JPEG_ENC_SWREG_46_INTRA_LEFT_MB_AREA_SHIFT)        /* 0x0000FF00 */
#define JPEG_ENC_SWREG_46_INTRA_DOWN_MB_AREA_SHIFT         (16U)
#define JPEG_ENC_SWREG_46_INTRA_DOWN_MB_AREA_MASK          (0xFFU << JPEG_ENC_SWREG_46_INTRA_DOWN_MB_AREA_SHIFT)        /* 0x00FF0000 */
#define JPEG_ENC_SWREG_46_INTRA_UP_MB_AREA_SHIFT           (24U)
#define JPEG_ENC_SWREG_46_INTRA_UP_MB_AREA_MASK            (0xFFU << JPEG_ENC_SWREG_46_INTRA_UP_MB_AREA_SHIFT)          /* 0xFF000000 */
/* SWREG_47 */
#define JPEG_ENC_SWREG_47_OFFSET                           (0xBCU)
#define JPEG_ENC_SWREG_47_CIR_INTRA_MB_ITVL_SHIFT          (0U)
#define JPEG_ENC_SWREG_47_CIR_INTRA_MB_ITVL_MASK           (0xFFFFU << JPEG_ENC_SWREG_47_CIR_INTRA_MB_ITVL_SHIFT)       /* 0x0000FFFF */
#define JPEG_ENC_SWREG_47_CIR_FIRST_INTRA_SHIFT            (16U)
#define JPEG_ENC_SWREG_47_CIR_FIRST_INTRA_MASK             (0xFFFFU << JPEG_ENC_SWREG_47_CIR_FIRST_INTRA_SHIFT)         /* 0xFFFF0000 */
/* SWREG_48 */
#define JPEG_ENC_SWREG_48_OFFSET                           (0xC0U)
#define JPEG_ENC_SWREG_48_LUMA_IN_ST_ADR_SHIFT             (0U)
#define JPEG_ENC_SWREG_48_LUMA_IN_ST_ADR_MASK              (0xFFFFFFFFU << JPEG_ENC_SWREG_48_LUMA_IN_ST_ADR_SHIFT)      /* 0xFFFFFFFF */
/* SWREG_49 */
#define JPEG_ENC_SWREG_49_OFFSET                           (0xC4U)
#define JPEG_ENC_SWREG_49_CB_IN_ST_ADR_SHIFT               (0U)
#define JPEG_ENC_SWREG_49_CB_IN_ST_ADR_MASK                (0xFFFFFFFFU << JPEG_ENC_SWREG_49_CB_IN_ST_ADR_SHIFT)        /* 0xFFFFFFFF */
/* SWREG_50 */
#define JPEG_ENC_SWREG_50_OFFSET                           (0xC8U)
#define JPEG_ENC_SWREG_50_CR_IN_ST_ADR_SHIFT               (0U)
#define JPEG_ENC_SWREG_50_CR_IN_ST_ADR_MASK                (0xFFFFFFFFU << JPEG_ENC_SWREG_50_CR_IN_ST_ADR_SHIFT)        /* 0xFFFFFFFF */
/* SWREG_51 */
#define JPEG_ENC_SWREG_51_OFFSET                           (0xCCU)
#define JPEG_ENC_SWREG_51_STRM_HEADER_LEFT_HBITS_SHIFT     (0U)
#define JPEG_ENC_SWREG_51_STRM_HEADER_LEFT_HBITS_MASK      (0xFFFFFFFFU << JPEG_ENC_SWREG_51_STRM_HEADER_LEFT_HBITS_SHIFT) /* 0xFFFFFFFF */
/* SWREG_52 */
#define JPEG_ENC_SWREG_52_OFFSET                           (0xD0U)
#define JPEG_ENC_SWREG_52_STRM_HEADER_LEFT_LBITS_SHIFT     (0U)
#define JPEG_ENC_SWREG_52_STRM_HEADER_LEFT_LBITS_MASK      (0xFFFFFFFFU << JPEG_ENC_SWREG_52_STRM_HEADER_LEFT_LBITS_SHIFT) /* 0xFFFFFFFF */
/* SWREG_53 */
#define JPEG_ENC_SWREG_53_OFFSET                           (0xD4U)
#define JPEG_ENC_SWREG_53_STRM_BUFSIZE_LMT_SHIFT           (0U)
#define JPEG_ENC_SWREG_53_STRM_BUFSIZE_LMT_MASK            (0xFFFFFFFFU << JPEG_ENC_SWREG_53_STRM_BUFSIZE_LMT_SHIFT)    /* 0xFFFFFFFF */
/* SWREG_54 */
#define JPEG_ENC_SWREG_54_OFFSET                           (0xD8U)
#define JPEG_ENC_SWREG_54_BURST_DISABLE_SHIFT              (0U)
#define JPEG_ENC_SWREG_54_BURST_DISABLE_MASK               (0x1U << JPEG_ENC_SWREG_54_BURST_DISABLE_SHIFT)              /* 0x00000001 */
#define JPEG_ENC_SWREG_54_BURST_DISCARD_SHIFT              (1U)
#define JPEG_ENC_SWREG_54_BURST_DISCARD_MASK               (0x1U << JPEG_ENC_SWREG_54_BURST_DISCARD_SHIFT)              /* 0x00000002 */
#define JPEG_ENC_SWREG_54_BURST_INCR_MOD_SEL_SHIFT         (2U)
#define JPEG_ENC_SWREG_54_BURST_INCR_MOD_SEL_MASK          (0x1U << JPEG_ENC_SWREG_54_BURST_INCR_MOD_SEL_SHIFT)         /* 0x00000004 */
#define JPEG_ENC_SWREG_54_BURST_LEN_SHIFT                  (8U)
#define JPEG_ENC_SWREG_54_BURST_LEN_MASK                   (0x3FU << JPEG_ENC_SWREG_54_BURST_LEN_SHIFT)                 /* 0x00003F00 */
#define JPEG_ENC_SWREG_54_AXI_WR_ID_SHIFT                  (16U)
#define JPEG_ENC_SWREG_54_AXI_WR_ID_MASK                   (0xFFU << JPEG_ENC_SWREG_54_AXI_WR_ID_SHIFT)                 /* 0x00FF0000 */
#define JPEG_ENC_SWREG_54_AXI_RD_ID_SHIFT                  (24U)
#define JPEG_ENC_SWREG_54_AXI_RD_ID_MASK                   (0xFFU << JPEG_ENC_SWREG_54_AXI_RD_ID_SHIFT)                 /* 0xFF000000 */
/* SWREG_55 */
#define JPEG_ENC_SWREG_55_OFFSET                           (0xDCU)
#define JPEG_ENC_SWREG_55_QP_ADJST_SHIFT                   (0U)
#define JPEG_ENC_SWREG_55_QP_ADJST_MASK                    (0xFU << JPEG_ENC_SWREG_55_QP_ADJST_SHIFT)                   /* 0x0000000F */
#define JPEG_ENC_SWREG_55_ROI_DLT_QP2_SHIFT                (8U)
#define JPEG_ENC_SWREG_55_ROI_DLT_QP2_MASK                 (0xFU << JPEG_ENC_SWREG_55_ROI_DLT_QP2_SHIFT)                /* 0x00000F00 */
#define JPEG_ENC_SWREG_55_ROI_DLT_QP1_SHIFT                (12U)
#define JPEG_ENC_SWREG_55_ROI_DLT_QP1_MASK                 (0xFU << JPEG_ENC_SWREG_55_ROI_DLT_QP1_SHIFT)                /* 0x0000F000 */
/* SWREG_56 */
#define JPEG_ENC_SWREG_56_OFFSET                           (0xE0U)
#define JPEG_ENC_SWREG_56_LUMA_REF_ST_ADR_SHIFT            (0U)
#define JPEG_ENC_SWREG_56_LUMA_REF_ST_ADR_MASK             (0xFFFFFFFFU << JPEG_ENC_SWREG_56_LUMA_REF_ST_ADR_SHIFT)     /* 0xFFFFFFFF */
/* SWREG_57 */
#define JPEG_ENC_SWREG_57_OFFSET                           (0xE4U)
#define JPEG_ENC_SWREG_57_CHROMA_REF_ST_ADR_SHIFT          (0U)
#define JPEG_ENC_SWREG_57_CHROMA_REF_ST_ADR_MASK           (0xFFFFFFFFU << JPEG_ENC_SWREG_57_CHROMA_REF_ST_ADR_SHIFT)   /* 0xFFFFFFFF */
/* SWREG_58 */
#define JPEG_ENC_SWREG_58_OFFSET                           (0xE8U)
#define JPEG_ENC_SWREG_58_QP_SUM_DIV2_SHIFT                (11U)
#define JPEG_ENC_SWREG_58_QP_SUM_DIV2_MASK                 (0x1FFFFFU << JPEG_ENC_SWREG_58_QP_SUM_DIV2_SHIFT)           /* 0xFFFFF800 */
/* SWREG_59 */
#define JPEG_ENC_SWREG_59_OFFSET                           (0xECU)
#define JPEG_ENC_SWREG_59_H264_SLICE_NUM_SHIFT             (8U)
#define JPEG_ENC_SWREG_59_H264_SLICE_NUM_MASK              (0x7FU << JPEG_ENC_SWREG_59_H264_SLICE_NUM_SHIFT)            /* 0x00007F00 */
#define JPEG_ENC_SWREG_59_H264_STRM_MOD_SEL_SHIFT          (15U)
#define JPEG_ENC_SWREG_59_H264_STRM_MOD_SEL_MASK           (0x1U << JPEG_ENC_SWREG_59_H264_STRM_MOD_SEL_SHIFT)          /* 0x00008000 */
#define JPEG_ENC_SWREG_59_H264_RES_INTERMOD_4X4_SHIFT      (16U)
#define JPEG_ENC_SWREG_59_H264_RES_INTERMOD_4X4_MASK       (0x1U << JPEG_ENC_SWREG_59_H264_RES_INTERMOD_4X4_SHIFT)      /* 0x00010000 */
#define JPEG_ENC_SWREG_59_H264_TRFMOD_8X8_SHIFT            (17U)
#define JPEG_ENC_SWREG_59_H264_TRFMOD_8X8_MASK             (0x1U << JPEG_ENC_SWREG_59_H264_TRFMOD_8X8_SHIFT)            /* 0x00020000 */
#define JPEG_ENC_SWREG_59_ENTRY_CODE_FMT_SHIFT             (20U)
#define JPEG_ENC_SWREG_59_ENTRY_CODE_FMT_MASK              (0x1U << JPEG_ENC_SWREG_59_ENTRY_CODE_FMT_SHIFT)             /* 0x00100000 */
#define JPEG_ENC_SWREG_59_H264_CABAC_IDC_SHIFT             (21U)
#define JPEG_ENC_SWREG_59_H264_CABAC_IDC_MASK              (0x3U << JPEG_ENC_SWREG_59_H264_CABAC_IDC_SHIFT)             /* 0x00600000 */
#define JPEG_ENC_SWREG_59_DBLKING_FLT_MODE_SHIFT           (24U)
#define JPEG_ENC_SWREG_59_DBLKING_FLT_MODE_MASK            (0x3U << JPEG_ENC_SWREG_59_DBLKING_FLT_MODE_SHIFT)           /* 0x03000000 */
#define JPEG_ENC_SWREG_59_H264_QURT_PIXMV_DIS_SHIFT        (28U)
#define JPEG_ENC_SWREG_59_H264_QURT_PIXMV_DIS_MASK         (0x1U << JPEG_ENC_SWREG_59_H264_QURT_PIXMV_DIS_SHIFT)        /* 0x10000000 */
/* SWREG_60 */
#define JPEG_ENC_SWREG_60_OFFSET                           (0xF0U)
#define JPEG_ENC_SWREG_60_BOT_SPILL_SHIFT                  (0U)
#define JPEG_ENC_SWREG_60_BOT_SPILL_MASK                   (0xFU << JPEG_ENC_SWREG_60_BOT_SPILL_SHIFT)                  /* 0x0000000F */
#define JPEG_ENC_SWREG_60_RIGHT_SPILL_SHIFT                (4U)
#define JPEG_ENC_SWREG_60_RIGHT_SPILL_MASK                 (0x3U << JPEG_ENC_SWREG_60_RIGHT_SPILL_SHIFT)                /* 0x00000030 */
#define JPEG_ENC_SWREG_60_SKIP_MB_MODE_SHIFT               (8U)
#define JPEG_ENC_SWREG_60_SKIP_MB_MODE_MASK                (0xFFU << JPEG_ENC_SWREG_60_SKIP_MB_MODE_SHIFT)              /* 0x0000FF00 */
#define JPEG_ENC_SWREG_60_STRM_ST_OFFSET_SHIFT             (16U)
#define JPEG_ENC_SWREG_60_STRM_ST_OFFSET_MASK              (0x3FU << JPEG_ENC_SWREG_60_STRM_ST_OFFSET_SHIFT)            /* 0x003F0000 */
/* SWREG_61 */
#define JPEG_ENC_SWREG_61_OFFSET                           (0xF4U)
#define JPEG_ENC_SWREG_61_ROW_LEN_IN_LUMA_SHIFT            (0U)
#define JPEG_ENC_SWREG_61_ROW_LEN_IN_LUMA_MASK             (0x3FFFU << JPEG_ENC_SWREG_61_ROW_LEN_IN_LUMA_SHIFT)         /* 0x00003FFF */
#define JPEG_ENC_SWREG_61_OFFSET_IN_LUMA_SHIFT             (16U)
#define JPEG_ENC_SWREG_61_OFFSET_IN_LUMA_MASK              (0x7U << JPEG_ENC_SWREG_61_OFFSET_IN_LUMA_SHIFT)             /* 0x00070000 */
#define JPEG_ENC_SWREG_61_OFFSET_IN_CHROMA_SHIFT           (20U)
#define JPEG_ENC_SWREG_61_OFFSET_IN_CHROMA_MASK            (0x7U << JPEG_ENC_SWREG_61_OFFSET_IN_CHROMA_SHIFT)           /* 0x00700000 */
/* SWREG_62 */
#define JPEG_ENC_SWREG_62_OFFSET                           (0xF8U)
#define JPEG_ENC_SWREG_62_RC_SUM_SHIFT                     (0U)
#define JPEG_ENC_SWREG_62_RC_SUM_MASK                      (0x3FFFFFU << JPEG_ENC_SWREG_62_RC_SUM_SHIFT)                /* 0x003FFFFF */
/* SWREG_63 */
#define JPEG_ENC_SWREG_63_OFFSET                           (0xFCU)
#define JPEG_ENC_SWREG_63_RECON_LUMA_ST_ADR_SHIFT          (0U)
#define JPEG_ENC_SWREG_63_RECON_LUMA_ST_ADR_MASK           (0xFFFFFFFFU << JPEG_ENC_SWREG_63_RECON_LUMA_ST_ADR_SHIFT)   /* 0xFFFFFFFF */
/* SWREG_64 */
#define JPEG_ENC_SWREG_64_OFFSET                           (0x100U)
#define JPEG_ENC_SWREG_64_RECON_CHROMA_ST_ADR_SHIFT        (0U)
#define JPEG_ENC_SWREG_64_RECON_CHROMA_ST_ADR_MASK         (0xFFFFFFFFU << JPEG_ENC_SWREG_64_RECON_CHROMA_ST_ADR_SHIFT) /* 0xFFFFFFFF */
/* SWREG_65_REUSE */
#define JPEG_ENC_SWREG_65_REUSE_OFFSET                     (0x104U)
#define JPEG_ENC_SWREG_65_REUSE_H264_CHKPT_2_SHIFT         (0U)
#define JPEG_ENC_SWREG_65_REUSE_H264_CHKPT_2_MASK          (0xFFFFU << JPEG_ENC_SWREG_65_REUSE_H264_CHKPT_2_SHIFT)      /* 0x0000FFFF */
#define JPEG_ENC_SWREG_65_REUSE_H264_CHKPT_1_SHIFT         (16U)
#define JPEG_ENC_SWREG_65_REUSE_H264_CHKPT_1_MASK          (0xFFFFU << JPEG_ENC_SWREG_65_REUSE_H264_CHKPT_1_SHIFT)      /* 0xFFFF0000 */
/* SWREG_66_REUSE */
#define JPEG_ENC_SWREG_66_REUSE_OFFSET                     (0x108U)
#define JPEG_ENC_SWREG_66_REUSE_H264_CHKPT_4_SHIFT         (0U)
#define JPEG_ENC_SWREG_66_REUSE_H264_CHKPT_4_MASK          (0xFFFFU << JPEG_ENC_SWREG_66_REUSE_H264_CHKPT_4_SHIFT)      /* 0x0000FFFF */
#define JPEG_ENC_SWREG_66_REUSE_H264_CHKPT_3_SHIFT         (16U)
#define JPEG_ENC_SWREG_66_REUSE_H264_CHKPT_3_MASK          (0xFFFFU << JPEG_ENC_SWREG_66_REUSE_H264_CHKPT_3_SHIFT)      /* 0xFFFF0000 */
/* SWREG_67_REUSE */
#define JPEG_ENC_SWREG_67_REUSE_OFFSET                     (0x10CU)
#define JPEG_ENC_SWREG_67_REUSE_H264_CHKPT_6_SHIFT         (0U)
#define JPEG_ENC_SWREG_67_REUSE_H264_CHKPT_6_MASK          (0xFFFFU << JPEG_ENC_SWREG_67_REUSE_H264_CHKPT_6_SHIFT)      /* 0x0000FFFF */
#define JPEG_ENC_SWREG_67_REUSE_H264_CHKPT_5_SHIFT         (16U)
#define JPEG_ENC_SWREG_67_REUSE_H264_CHKPT_5_MASK          (0xFFFFU << JPEG_ENC_SWREG_67_REUSE_H264_CHKPT_5_SHIFT)      /* 0xFFFF0000 */
/* SWREG_68_REUSE */
#define JPEG_ENC_SWREG_68_REUSE_OFFSET                     (0x110U)
#define JPEG_ENC_SWREG_68_REUSE_H264_CHKPT_8_SHIFT         (0U)
#define JPEG_ENC_SWREG_68_REUSE_H264_CHKPT_8_MASK          (0xFFFFU << JPEG_ENC_SWREG_68_REUSE_H264_CHKPT_8_SHIFT)      /* 0x0000FFFF */
#define JPEG_ENC_SWREG_68_REUSE_H264_CHKPT_7_SHIFT         (16U)
#define JPEG_ENC_SWREG_68_REUSE_H264_CHKPT_7_MASK          (0xFFFFU << JPEG_ENC_SWREG_68_REUSE_H264_CHKPT_7_SHIFT)      /* 0xFFFF0000 */
/* SWREG_69_REUSE */
#define JPEG_ENC_SWREG_69_REUSE_OFFSET                     (0x114U)
#define JPEG_ENC_SWREG_69_REUSE_H264_CHKPT_10_SHIFT        (0U)
#define JPEG_ENC_SWREG_69_REUSE_H264_CHKPT_10_MASK         (0xFFFFU << JPEG_ENC_SWREG_69_REUSE_H264_CHKPT_10_SHIFT)     /* 0x0000FFFF */
#define JPEG_ENC_SWREG_69_REUSE_H264_CHKPT_9_SHIFT         (16U)
#define JPEG_ENC_SWREG_69_REUSE_H264_CHKPT_9_MASK          (0xFFFFU << JPEG_ENC_SWREG_69_REUSE_H264_CHKPT_9_SHIFT)      /* 0xFFFF0000 */
/* SWREG_70_REUSE */
#define JPEG_ENC_SWREG_70_REUSE_OFFSET                     (0x118U)
#define JPEG_ENC_SWREG_70_REUSE_H264_ERRCHKPT_2_SHIFT      (0U)
#define JPEG_ENC_SWREG_70_REUSE_H264_ERRCHKPT_2_MASK       (0xFFFFU << JPEG_ENC_SWREG_70_REUSE_H264_ERRCHKPT_2_SHIFT)   /* 0x0000FFFF */
#define JPEG_ENC_SWREG_70_REUSE_H264_ERRCHKPT_1_SHIFT      (16U)
#define JPEG_ENC_SWREG_70_REUSE_H264_ERRCHKPT_1_MASK       (0xFFFFU << JPEG_ENC_SWREG_70_REUSE_H264_ERRCHKPT_1_SHIFT)   /* 0xFFFF0000 */
/* SWREG_71_REUSE */
#define JPEG_ENC_SWREG_71_REUSE_OFFSET                     (0x11CU)
#define JPEG_ENC_SWREG_71_REUSE_H264_ERRCHKPT_4_SHIFT      (0U)
#define JPEG_ENC_SWREG_71_REUSE_H264_ERRCHKPT_4_MASK       (0xFFFFU << JPEG_ENC_SWREG_71_REUSE_H264_ERRCHKPT_4_SHIFT)   /* 0x0000FFFF */
#define JPEG_ENC_SWREG_71_REUSE_H264_ERRCHKPT_3_SHIFT      (16U)
#define JPEG_ENC_SWREG_71_REUSE_H264_ERRCHKPT_3_MASK       (0xFFFFU << JPEG_ENC_SWREG_71_REUSE_H264_ERRCHKPT_3_SHIFT)   /* 0xFFFF0000 */
/* SWREG_72_REUSE */
#define JPEG_ENC_SWREG_72_REUSE_OFFSET                     (0x120U)
#define JPEG_ENC_SWREG_72_REUSE_H264_ERRCHKPT_6_SHIFT      (0U)
#define JPEG_ENC_SWREG_72_REUSE_H264_ERRCHKPT_6_MASK       (0xFFFFU << JPEG_ENC_SWREG_72_REUSE_H264_ERRCHKPT_6_SHIFT)   /* 0x0000FFFF */
#define JPEG_ENC_SWREG_72_REUSE_H264_ERRCHKPT_5_SHIFT      (16U)
#define JPEG_ENC_SWREG_72_REUSE_H264_ERRCHKPT_5_MASK       (0xFFFFU << JPEG_ENC_SWREG_72_REUSE_H264_ERRCHKPT_5_SHIFT)   /* 0xFFFF0000 */
/* SWREG_73_REUSE */
#define JPEG_ENC_SWREG_73_REUSE_OFFSET                     (0x124U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_7_SHIFT              (0U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_7_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_7_SHIFT)              /* 0x0000000F */
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_6_SHIFT              (4U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_6_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_6_SHIFT)              /* 0x000000F0 */
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_5_SHIFT              (8U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_5_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_5_SHIFT)              /* 0x00000F00 */
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_4_SHIFT              (12U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_4_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_4_SHIFT)              /* 0x0000F000 */
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_3_SHIFT              (16U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_3_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_3_SHIFT)              /* 0x000F0000 */
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_2_SHIFT              (20U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_2_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_2_SHIFT)              /* 0x00F00000 */
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_1_SHIFT              (24U)
#define JPEG_ENC_SWREG_73_REUSE_CHKQP_1_MASK               (0xFU << JPEG_ENC_SWREG_73_REUSE_CHKQP_1_SHIFT)              /* 0x0F000000 */
/* SWREG_74 */
#define JPEG_ENC_SWREG_74_OFFSET                           (0x128U)
#define JPEG_ENC_SWREG_74_NAL_MODE_SHIFT                   (0U)
#define JPEG_ENC_SWREG_74_NAL_MODE_MASK                    (0x1U << JPEG_ENC_SWREG_74_NAL_MODE_SHIFT)                   /* 0x00000001 */
#define JPEG_ENC_SWREG_74_IMG_IN_ROT_SHIFT                 (2U)
#define JPEG_ENC_SWREG_74_IMG_IN_ROT_MASK                  (0x3U << JPEG_ENC_SWREG_74_IMG_IN_ROT_SHIFT)                 /* 0x0000000C */
#define JPEG_ENC_SWREG_74_IMG_FMT_IN_SHIFT                 (4U)
#define JPEG_ENC_SWREG_74_IMG_FMT_IN_MASK                  (0xFU << JPEG_ENC_SWREG_74_IMG_FMT_IN_SHIFT)                 /* 0x000000F0 */
#define JPEG_ENC_SWREG_74_ENCODERED_SLICES_SHIFT           (16U)
#define JPEG_ENC_SWREG_74_ENCODERED_SLICES_MASK            (0xFFU << JPEG_ENC_SWREG_74_ENCODERED_SLICES_SHIFT)          /* 0x00FF0000 */
#define JPEG_ENC_SWREG_74_MAD_THSLD_SHIFT                  (24U)
#define JPEG_ENC_SWREG_74_MAD_THSLD_MASK                   (0x3FU << JPEG_ENC_SWREG_74_MAD_THSLD_SHIFT)                 /* 0x3F000000 */
/* SWREG_75 */
#define JPEG_ENC_SWREG_75_OFFSET                           (0x12CU)
#define JPEG_ENC_SWREG_75_INTERMOD_SHIFT                   (0U)
#define JPEG_ENC_SWREG_75_INTERMOD_MASK                    (0xFFFFU << JPEG_ENC_SWREG_75_INTERMOD_SHIFT)                /* 0x0000FFFF */
#define JPEG_ENC_SWREG_75_INTRAMOD_16X16_SHIFT             (16U)
#define JPEG_ENC_SWREG_75_INTRAMOD_16X16_MASK              (0xFFFFU << JPEG_ENC_SWREG_75_INTRAMOD_16X16_SHIFT)          /* 0xFFFF0000 */
/* SWREG_76_REUSE */
#define JPEG_ENC_SWREG_76_REUSE_OFFSET                     (0x130U)
#define JPEG_ENC_SWREG_76_REUSE_CONSTR_INTRA_PRED_SHIFT    (0U)
#define JPEG_ENC_SWREG_76_REUSE_CONSTR_INTRA_PRED_MASK     (0x1U << JPEG_ENC_SWREG_76_REUSE_CONSTR_INTRA_PRED_SHIFT)    /* 0x00000001 */
#define JPEG_ENC_SWREG_76_REUSE_IDR_PICID_SHIFT            (1U)
#define JPEG_ENC_SWREG_76_REUSE_IDR_PICID_MASK             (0xFU << JPEG_ENC_SWREG_76_REUSE_IDR_PICID_SHIFT)            /* 0x0000001E */
#define JPEG_ENC_SWREG_76_REUSE_SW_QPASS_SHIFT             (8U)
#define JPEG_ENC_SWREG_76_REUSE_SW_QPASS_MASK              (0x1U << JPEG_ENC_SWREG_76_REUSE_SW_QPASS_SHIFT)             /* 0x00000100 */
#define JPEG_ENC_SWREG_76_REUSE_QP_OFFSET_CH_SHIFT         (13U)
#define JPEG_ENC_SWREG_76_REUSE_QP_OFFSET_CH_MASK          (0x1FU << JPEG_ENC_SWREG_76_REUSE_QP_OFFSET_CH_SHIFT)        /* 0x0003E000 */
#define JPEG_ENC_SWREG_76_REUSE_SLICE_FLT_BETA_SHIFT       (18U)
#define JPEG_ENC_SWREG_76_REUSE_SLICE_FLT_BETA_MASK        (0xFU << JPEG_ENC_SWREG_76_REUSE_SLICE_FLT_BETA_SHIFT)       /* 0x003C0000 */
#define JPEG_ENC_SWREG_76_REUSE_SLICE_FLT_ALPHA_SHIFT      (22U)
#define JPEG_ENC_SWREG_76_REUSE_SLICE_FLT_ALPHA_MASK       (0xFU << JPEG_ENC_SWREG_76_REUSE_SLICE_FLT_ALPHA_SHIFT)      /* 0x03C00000 */
#define JPEG_ENC_SWREG_76_REUSE_PPS_INIT_QP_SHIFT          (26U)
#define JPEG_ENC_SWREG_76_REUSE_PPS_INIT_QP_MASK           (0x3FU << JPEG_ENC_SWREG_76_REUSE_PPS_INIT_QP_SHIFT)         /* 0xFC000000 */
/* SWREG_77 */
#define JPEG_ENC_SWREG_77_OFFSET                           (0x134U)
#define JPEG_ENC_SWREG_77_OUTPUT_STRM_ST_ADR_SHIFT         (0U)
#define JPEG_ENC_SWREG_77_OUTPUT_STRM_ST_ADR_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_77_OUTPUT_STRM_ST_ADR_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_78 */
#define JPEG_ENC_SWREG_78_OFFSET                           (0x138U)
#define JPEG_ENC_SWREG_78_OUTPUT_CTRL_ST_ADR_SHIFT         (0U)
#define JPEG_ENC_SWREG_78_OUTPUT_CTRL_ST_ADR_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_78_OUTPUT_CTRL_ST_ADR_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_79 */
#define JPEG_ENC_SWREG_79_OFFSET                           (0x13CU)
#define JPEG_ENC_SWREG_79_NEXT_LUMA_ST_ADR_SHIFT           (0U)
#define JPEG_ENC_SWREG_79_NEXT_LUMA_ST_ADR_MASK            (0xFFFFFFFFU << JPEG_ENC_SWREG_79_NEXT_LUMA_ST_ADR_SHIFT)    /* 0xFFFFFFFF */
/* SWREG_80 */
#define JPEG_ENC_SWREG_80_OFFSET                           (0x140U)
#define JPEG_ENC_SWREG_80_MV_OUT_ST_ADR_SHIFT              (0U)
#define JPEG_ENC_SWREG_80_MV_OUT_ST_ADR_MASK               (0xFFFFFFFFU << JPEG_ENC_SWREG_80_MV_OUT_ST_ADR_SHIFT)       /* 0xFFFFFFFF */
/* SWREG_81 */
#define JPEG_ENC_SWREG_81_OFFSET                           (0x144U)
#define JPEG_ENC_SWREG_81_CABAC_TABLE_ST_ADR_SHIFT         (0U)
#define JPEG_ENC_SWREG_81_CABAC_TABLE_ST_ADR_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_81_CABAC_TABLE_ST_ADR_SHIFT)  /* 0xFFFFFFFF */
/* SWREG_82 */
#define JPEG_ENC_SWREG_82_OFFSET                           (0x148U)
#define JPEG_ENC_SWREG_82_FIRST_ROI_RMB_SHIFT              (0U)
#define JPEG_ENC_SWREG_82_FIRST_ROI_RMB_MASK               (0xFFU << JPEG_ENC_SWREG_82_FIRST_ROI_RMB_SHIFT)             /* 0x000000FF */
#define JPEG_ENC_SWREG_82_FIRST_ROI_LMB_SHIFT              (8U)
#define JPEG_ENC_SWREG_82_FIRST_ROI_LMB_MASK               (0xFFU << JPEG_ENC_SWREG_82_FIRST_ROI_LMB_SHIFT)             /* 0x0000FF00 */
#define JPEG_ENC_SWREG_82_FIRST_ROI_BMB_SHIFT              (16U)
#define JPEG_ENC_SWREG_82_FIRST_ROI_BMB_MASK               (0xFFU << JPEG_ENC_SWREG_82_FIRST_ROI_BMB_SHIFT)             /* 0x00FF0000 */
#define JPEG_ENC_SWREG_82_FIRST_ROI_TMB_SHIFT              (24U)
#define JPEG_ENC_SWREG_82_FIRST_ROI_TMB_MASK               (0xFFU << JPEG_ENC_SWREG_82_FIRST_ROI_TMB_SHIFT)             /* 0xFF000000 */
/* SWREG_83 */
#define JPEG_ENC_SWREG_83_OFFSET                           (0x14CU)
#define JPEG_ENC_SWREG_83_SECOND_ROI_TMB_SHIFT             (0U)
#define JPEG_ENC_SWREG_83_SECOND_ROI_TMB_MASK              (0xFFU << JPEG_ENC_SWREG_83_SECOND_ROI_TMB_SHIFT)            /* 0x000000FF */
#define JPEG_ENC_SWREG_83_SECOND_ROI_LMB_SHIFT             (8U)
#define JPEG_ENC_SWREG_83_SECOND_ROI_LMB_MASK              (0xFFU << JPEG_ENC_SWREG_83_SECOND_ROI_LMB_SHIFT)            /* 0x0000FF00 */
#define JPEG_ENC_SWREG_83_SECOND_ROI_BMB_SHIFT             (16U)
#define JPEG_ENC_SWREG_83_SECOND_ROI_BMB_MASK              (0xFFU << JPEG_ENC_SWREG_83_SECOND_ROI_BMB_SHIFT)            /* 0x00FF0000 */
#define JPEG_ENC_SWREG_83_SECOND_ROI_RMB_SHIFT             (24U)
#define JPEG_ENC_SWREG_83_SECOND_ROI_RMB_MASK              (0xFFU << JPEG_ENC_SWREG_83_SECOND_ROI_RMB_SHIFT)            /* 0xFF000000 */
/* SWREG_84 */
#define JPEG_ENC_SWREG_84_OFFSET                           (0x150U)
#define JPEG_ENC_SWREG_84_STAB_MATRIX1_SHIFT               (0U)
#define JPEG_ENC_SWREG_84_STAB_MATRIX1_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_84_STAB_MATRIX1_SHIFT)          /* 0x00FFFFFF */
/* SWREG_85 */
#define JPEG_ENC_SWREG_85_OFFSET                           (0x154U)
#define JPEG_ENC_SWREG_85_STAB_MATRIX2_SHIFT               (0U)
#define JPEG_ENC_SWREG_85_STAB_MATRIX2_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_85_STAB_MATRIX2_SHIFT)          /* 0x00FFFFFF */
/* SWREG_86 */
#define JPEG_ENC_SWREG_86_OFFSET                           (0x158U)
#define JPEG_ENC_SWREG_86_STAB_MATRIX3_SHIFT               (0U)
#define JPEG_ENC_SWREG_86_STAB_MATRIX3_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_86_STAB_MATRIX3_SHIFT)          /* 0x00FFFFFF */
/* SWREG_87 */
#define JPEG_ENC_SWREG_87_OFFSET                           (0x15CU)
#define JPEG_ENC_SWREG_87_STAB_MATRIX4_SHIFT               (0U)
#define JPEG_ENC_SWREG_87_STAB_MATRIX4_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_87_STAB_MATRIX4_SHIFT)          /* 0x00FFFFFF */
/* SWREG_88 */
#define JPEG_ENC_SWREG_88_OFFSET                           (0x160U)
#define JPEG_ENC_SWREG_88_STAB_MATRIX5_SHIFT               (0U)
#define JPEG_ENC_SWREG_88_STAB_MATRIX5_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_88_STAB_MATRIX5_SHIFT)          /* 0x00FFFFFF */
/* SWREG_89 */
#define JPEG_ENC_SWREG_89_OFFSET                           (0x164U)
#define JPEG_ENC_SWREG_89_STAB_MATRIX6_SHIFT               (0U)
#define JPEG_ENC_SWREG_89_STAB_MATRIX6_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_89_STAB_MATRIX6_SHIFT)          /* 0x00FFFFFF */
/* SWREG_90 */
#define JPEG_ENC_SWREG_90_OFFSET                           (0x168U)
#define JPEG_ENC_SWREG_90_STAB_MATRIX7_SHIFT               (0U)
#define JPEG_ENC_SWREG_90_STAB_MATRIX7_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_90_STAB_MATRIX7_SHIFT)          /* 0x00FFFFFF */
/* SWREG_91 */
#define JPEG_ENC_SWREG_91_OFFSET                           (0x16CU)
#define JPEG_ENC_SWREG_91_STAB_MATRIX8_SHIFT               (0U)
#define JPEG_ENC_SWREG_91_STAB_MATRIX8_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_91_STAB_MATRIX8_SHIFT)          /* 0x00FFFFFF */
/* SWREG_92 */
#define JPEG_ENC_SWREG_92_OFFSET                           (0x170U)
#define JPEG_ENC_SWREG_92_STAB_MATRIX9_SHIFT               (0U)
#define JPEG_ENC_SWREG_92_STAB_MATRIX9_MASK                (0xFFFFFFU << JPEG_ENC_SWREG_92_STAB_MATRIX9_SHIFT)          /* 0x00FFFFFF */
#define JPEG_ENC_SWREG_92_STAB_GMV_VRTL_SHIFT              (26U)
#define JPEG_ENC_SWREG_92_STAB_GMV_VRTL_MASK               (0x3FU << JPEG_ENC_SWREG_92_STAB_GMV_VRTL_SHIFT)             /* 0xFC000000 */
/* SWREG_93 */
#define JPEG_ENC_SWREG_93_OFFSET                           (0x174U)
#define JPEG_ENC_SWREG_93_STAB_MOTION_SUM_SHIFT            (0U)
#define JPEG_ENC_SWREG_93_STAB_MOTION_SUM_MASK             (0xFFFFFFFFU << JPEG_ENC_SWREG_93_STAB_MOTION_SUM_SHIFT)     /* 0xFFFFFFFF */
/* SWREG_94 */
#define JPEG_ENC_SWREG_94_OFFSET                           (0x178U)
#define JPEG_ENC_SWREG_94_STAB_HOR_GMV_SHIFT               (0U)
#define JPEG_ENC_SWREG_94_STAB_HOR_GMV_MASK                (0x3FU << JPEG_ENC_SWREG_94_STAB_HOR_GMV_SHIFT)              /* 0x0000003F */
#define JPEG_ENC_SWREG_94_STAB_MOD_SEL_SHIFT               (6U)
#define JPEG_ENC_SWREG_94_STAB_MOD_SEL_MASK                (0x3U << JPEG_ENC_SWREG_94_STAB_MOD_SEL_SHIFT)               /* 0x000000C0 */
#define JPEG_ENC_SWREG_94_STAB_MIN_VALUE_SHIFT             (8U)
#define JPEG_ENC_SWREG_94_STAB_MIN_VALUE_MASK              (0xFFFFFFU << JPEG_ENC_SWREG_94_STAB_MIN_VALUE_SHIFT)        /* 0xFFFFFF00 */
/* SWREG_95 */
#define JPEG_ENC_SWREG_95_OFFSET                           (0x17CU)
#define JPEG_ENC_SWREG_95_RGB2YUV_COE1_SHIFT               (0U)
#define JPEG_ENC_SWREG_95_RGB2YUV_COE1_MASK                (0xFFFFU << JPEG_ENC_SWREG_95_RGB2YUV_COE1_SHIFT)            /* 0x0000FFFF */
#define JPEG_ENC_SWREG_95_RGB2YUV_COE2_SHIFT               (16U)
#define JPEG_ENC_SWREG_95_RGB2YUV_COE2_MASK                (0xFFFFU << JPEG_ENC_SWREG_95_RGB2YUV_COE2_SHIFT)            /* 0xFFFF0000 */
/* SWREG_96 */
#define JPEG_ENC_SWREG_96_OFFSET                           (0x180U)
#define JPEG_ENC_SWREG_96_RGB2YUV_COE3_SHIFT               (0U)
#define JPEG_ENC_SWREG_96_RGB2YUV_COE3_MASK                (0xFFFFU << JPEG_ENC_SWREG_96_RGB2YUV_COE3_SHIFT)            /* 0x0000FFFF */
#define JPEG_ENC_SWREG_96_RGB2YUV_COE5_SHIFT               (16U)
#define JPEG_ENC_SWREG_96_RGB2YUV_COE5_MASK                (0xFFFFU << JPEG_ENC_SWREG_96_RGB2YUV_COE5_SHIFT)            /* 0xFFFF0000 */
/* SWREG_97 */
#define JPEG_ENC_SWREG_97_OFFSET                           (0x184U)
#define JPEG_ENC_SWREG_97_RGB2YUV_COE6_SHIFT               (0U)
#define JPEG_ENC_SWREG_97_RGB2YUV_COE6_MASK                (0xFFFFU << JPEG_ENC_SWREG_97_RGB2YUV_COE6_SHIFT)            /* 0x0000FFFF */
/* SWREG_98 */
#define JPEG_ENC_SWREG_98_OFFSET                           (0x188U)
#define JPEG_ENC_SWREG_98_RCMPT_MASK_POSTITION_SHIFT       (0U)
#define JPEG_ENC_SWREG_98_RCMPT_MASK_POSTITION_MASK        (0x1FU << JPEG_ENC_SWREG_98_RCMPT_MASK_POSTITION_SHIFT)      /* 0x0000001F */
#define JPEG_ENC_SWREG_98_GCMPT_MASK_POSTITION_SHIFT       (8U)
#define JPEG_ENC_SWREG_98_GCMPT_MASK_POSTITION_MASK        (0x1FU << JPEG_ENC_SWREG_98_GCMPT_MASK_POSTITION_SHIFT)      /* 0x00001F00 */
#define JPEG_ENC_SWREG_98_BCMPT_MASK_POSTITION_SHIFT       (16U)
#define JPEG_ENC_SWREG_98_BCMPT_MASK_POSTITION_MASK        (0x1FU << JPEG_ENC_SWREG_98_BCMPT_MASK_POSTITION_SHIFT)      /* 0x001F0000 */
/* SWREG_99 */
#define JPEG_ENC_SWREG_99_OFFSET                           (0x18CU)
#define JPEG_ENC_SWREG_99_MUTIMV_EN_SHIFT                  (0U)
#define JPEG_ENC_SWREG_99_MUTIMV_EN_MASK                   (0x1U << JPEG_ENC_SWREG_99_MUTIMV_EN_SHIFT)                  /* 0x00000001 */
#define JPEG_ENC_SWREG_99_MV_4P_PLY_SHIFT                  (1U)
#define JPEG_ENC_SWREG_99_MV_4P_PLY_MASK                   (0x3FFU << JPEG_ENC_SWREG_99_MV_4P_PLY_SHIFT)                /* 0x000007FE */
#define JPEG_ENC_SWREG_99_MV_1P_4P_PLY_SHIFT               (11U)
#define JPEG_ENC_SWREG_99_MV_1P_4P_PLY_MASK                (0x3FFU << JPEG_ENC_SWREG_99_MV_1P_4P_PLY_SHIFT)             /* 0x001FF800 */
#define JPEG_ENC_SWREG_99_MV_1P_PLY_SHIFT                  (21U)
#define JPEG_ENC_SWREG_99_MV_1P_PLY_MASK                   (0x3FFU << JPEG_ENC_SWREG_99_MV_1P_PLY_SHIFT)                /* 0x7FE00000 */
/* SWREG_100_REUSE */
#define JPEG_ENC_SWREG_100_REUSE_OFFSET                    (0x190U)
#define JPEG_ENC_SWREG_100_REUSE_H264_CHKPT_DISTANCE_SHIFT (0U)
#define JPEG_ENC_SWREG_100_REUSE_H264_CHKPT_DISTANCE_MASK  (0x1FFFU << JPEG_ENC_SWREG_100_REUSE_H264_CHKPT_DISTANCE_SHIFT) /* 0x00001FFF */
#define JPEG_ENC_SWREG_100_REUSE_H264_MIN_QP_SHIFT         (14U)
#define JPEG_ENC_SWREG_100_REUSE_H264_MIN_QP_MASK          (0x3FU << JPEG_ENC_SWREG_100_REUSE_H264_MIN_QP_SHIFT)        /* 0x000FC000 */
#define JPEG_ENC_SWREG_100_REUSE_H264_MAX_QP_SHIFT         (20U)
#define JPEG_ENC_SWREG_100_REUSE_H264_MAX_QP_MASK          (0x3FU << JPEG_ENC_SWREG_100_REUSE_H264_MAX_QP_SHIFT)        /* 0x03F00000 */
#define JPEG_ENC_SWREG_100_REUSE_H264_INIT_LUMA_QP_SHIFT   (26U)
#define JPEG_ENC_SWREG_100_REUSE_H264_INIT_LUMA_QP_MASK    (0x3FU << JPEG_ENC_SWREG_100_REUSE_H264_INIT_LUMA_QP_SHIFT)  /* 0xFC000000 */
/* SWREG_101_READ */
#define JPEG_ENC_SWREG_101_READ_OFFSET                     (0x194U)
#define JPEG_ENC_SWREG_101_READ                            (0x1F522780U)
#define JPEG_ENC_SWREG_101_READ_MAX_VID_WIDTH_SHIFT        (0U)
#define JPEG_ENC_SWREG_101_READ_MAX_VID_WIDTH_MASK         (0xFFFU << JPEG_ENC_SWREG_101_READ_MAX_VID_WIDTH_SHIFT)      /* 0x00000FFF */
#define JPEG_ENC_SWREG_101_READ_HW_CONFIG_SHIFT            (12U)
#define JPEG_ENC_SWREG_101_READ_HW_CONFIG_MASK             (0xFFFFFU << JPEG_ENC_SWREG_101_READ_HW_CONFIG_SHIFT)        /* 0xFFFFF000 */
/* SWREG_102 */
#define JPEG_ENC_SWREG_102_OFFSET                          (0x198U)
#define JPEG_ENC_SWREG_102_MVC_INTER_VIEW_FLAG_SHIFT       (0U)
#define JPEG_ENC_SWREG_102_MVC_INTER_VIEW_FLAG_MASK        (0x1U << JPEG_ENC_SWREG_102_MVC_INTER_VIEW_FLAG_SHIFT)       /* 0x00000001 */
#define JPEG_ENC_SWREG_102_MVC_TEMPORAL_ID_SHIFT           (1U)
#define JPEG_ENC_SWREG_102_MVC_TEMPORAL_ID_MASK            (0x7U << JPEG_ENC_SWREG_102_MVC_TEMPORAL_ID_SHIFT)           /* 0x0000000E */
#define JPEG_ENC_SWREG_102_MVC_PRIORITY_ID_SHIFT           (4U)
#define JPEG_ENC_SWREG_102_MVC_PRIORITY_ID_MASK            (0x7U << JPEG_ENC_SWREG_102_MVC_PRIORITY_ID_SHIFT)           /* 0x00000070 */
#define JPEG_ENC_SWREG_102_MVC_ANCHOR_PIC_FLAG_SHIFT       (7U)
#define JPEG_ENC_SWREG_102_MVC_ANCHOR_PIC_FLAG_MASK        (0x1U << JPEG_ENC_SWREG_102_MVC_ANCHOR_PIC_FLAG_SHIFT)       /* 0x00000080 */
#define JPEG_ENC_SWREG_102_MVC_VIEW_ID_SHIFT               (8U)
#define JPEG_ENC_SWREG_102_MVC_VIEW_ID_MASK                (0x7U << JPEG_ENC_SWREG_102_MVC_VIEW_ID_SHIFT)               /* 0x00000700 */
#define JPEG_ENC_SWREG_102_MV_PLY_4X4_SHIFT                (11U)
#define JPEG_ENC_SWREG_102_MV_PLY_4X4_MASK                 (0x1FFU << JPEG_ENC_SWREG_102_MV_PLY_4X4_SHIFT)              /* 0x000FF800 */
#define JPEG_ENC_SWREG_102_MV_FAVOR_16X16_SHIFT            (20U)
#define JPEG_ENC_SWREG_102_MV_FAVOR_16X16_MASK             (0xFU << JPEG_ENC_SWREG_102_MV_FAVOR_16X16_SHIFT)            /* 0x00F00000 */
/* SWREG_103 */
#define JPEG_ENC_SWREG_103_OFFSET                          (0x19CU)
#define JPEG_ENC_SWREG_103_ENC_EN_SHIFT                    (0U)
#define JPEG_ENC_SWREG_103_ENC_EN_MASK                     (0x1U << JPEG_ENC_SWREG_103_ENC_EN_SHIFT)                    /* 0x00000001 */
#define JPEG_ENC_SWREG_103_ENC_FMT_SHIFT                   (4U)
#define JPEG_ENC_SWREG_103_ENC_FMT_MASK                    (0x3U << JPEG_ENC_SWREG_103_ENC_FMT_SHIFT)                   /* 0x00000030 */
#define JPEG_ENC_SWREG_103_ENC_FRAME_TYPE_SHIFT            (6U)
#define JPEG_ENC_SWREG_103_ENC_FRAME_TYPE_MASK             (0x3U << JPEG_ENC_SWREG_103_ENC_FRAME_TYPE_SHIFT)            /* 0x000000C0 */
#define JPEG_ENC_SWREG_103_ENC_WIDTH_SHIFT                 (8U)
#define JPEG_ENC_SWREG_103_ENC_WIDTH_MASK                  (0x1FFU << JPEG_ENC_SWREG_103_ENC_WIDTH_SHIFT)               /* 0x0001FF00 */
#define JPEG_ENC_SWREG_103_ENC_HEIGHT_SHIFT                (20U)
#define JPEG_ENC_SWREG_103_ENC_HEIGHT_MASK                 (0x1FFU << JPEG_ENC_SWREG_103_ENC_HEIGHT_SHIFT)              /* 0x1FF00000 */
/* SWREG_104 */
#define JPEG_ENC_SWREG_104_OFFSET                          (0x1A0U)
#define JPEG_ENC_SWREG_104_MB_CNT_SHIFT                    (0U)
#define JPEG_ENC_SWREG_104_MB_CNT_MASK                     (0xFFFFU << JPEG_ENC_SWREG_104_MB_CNT_SHIFT)                 /* 0x0000FFFF */
#define JPEG_ENC_SWREG_104_MB_COUNT_OUT_SHIFT              (16U)
#define JPEG_ENC_SWREG_104_MB_COUNT_OUT_MASK               (0xFFFFU << JPEG_ENC_SWREG_104_MB_COUNT_OUT_SHIFT)           /* 0xFFFF0000 */
/* SWREG_105 */
#define JPEG_ENC_SWREG_105_OFFSET                          (0x1A4U)
#define JPEG_ENC_SWREG_105_TEST_LEN_SHIFT                  (0U)
#define JPEG_ENC_SWREG_105_TEST_LEN_MASK                   (0x3FFFFU << JPEG_ENC_SWREG_105_TEST_LEN_SHIFT)              /* 0x0003FFFF */
#define JPEG_ENC_SWREG_105_COHER_TEST_MEM_SHIFT            (18U)
#define JPEG_ENC_SWREG_105_COHER_TEST_MEM_MASK             (0x1U << JPEG_ENC_SWREG_105_COHER_TEST_MEM_SHIFT)            /* 0x00040000 */
#define JPEG_ENC_SWREG_105_COHER_TEST_REG_SHIFT            (19U)
#define JPEG_ENC_SWREG_105_COHER_TEST_REG_MASK             (0x1U << JPEG_ENC_SWREG_105_COHER_TEST_REG_SHIFT)            /* 0x00080000 */
#define JPEG_ENC_SWREG_105_TEST_COUNTER_SHIFT              (20U)
#define JPEG_ENC_SWREG_105_TEST_COUNTER_MASK               (0xFU << JPEG_ENC_SWREG_105_TEST_COUNTER_SHIFT)              /* 0x00F00000 */
#define JPEG_ENC_SWREG_105_TEST_IRQ_SHIFT                  (24U)
#define JPEG_ENC_SWREG_105_TEST_IRQ_MASK                   (0x1U << JPEG_ENC_SWREG_105_TEST_IRQ_SHIFT)                  /* 0x01000000 */
#define JPEG_ENC_SWREG_105_SWAP32_OUT_SHIFT                (26U)
#define JPEG_ENC_SWREG_105_SWAP32_OUT_MASK                 (0x1U << JPEG_ENC_SWREG_105_SWAP32_OUT_SHIFT)                /* 0x04000000 */
#define JPEG_ENC_SWREG_105_SWAP16_OUT_SHIFT                (27U)
#define JPEG_ENC_SWREG_105_SWAP16_OUT_MASK                 (0x1U << JPEG_ENC_SWREG_105_SWAP16_OUT_SHIFT)                /* 0x08000000 */
#define JPEG_ENC_SWREG_105_SWAP8_OUT_SHIFT                 (28U)
#define JPEG_ENC_SWREG_105_SWAP8_OUT_MASK                  (0x1U << JPEG_ENC_SWREG_105_SWAP8_OUT_SHIFT)                 /* 0x10000000 */
#define JPEG_ENC_SWREG_105_SWAP32_IN_SHIFT                 (29U)
#define JPEG_ENC_SWREG_105_SWAP32_IN_MASK                  (0x1U << JPEG_ENC_SWREG_105_SWAP32_IN_SHIFT)                 /* 0x20000000 */
#define JPEG_ENC_SWREG_105_SWAP16_IN_SHIFT                 (30U)
#define JPEG_ENC_SWREG_105_SWAP16_IN_MASK                  (0x1U << JPEG_ENC_SWREG_105_SWAP16_IN_SHIFT)                 /* 0x40000000 */
#define JPEG_ENC_SWREG_105_SWAP8_IN_SHIFT                  (31U)
#define JPEG_ENC_SWREG_105_SWAP8_IN_MASK                   (0x1U << JPEG_ENC_SWREG_105_SWAP8_IN_SHIFT)                  /* 0x80000000 */
/* SWREG_106_REUSE */
#define JPEG_ENC_SWREG_106_REUSE_OFFSET                    (0x1A8U)
#define JPEG_ENC_SWREG_106_REUSE_FRAME_NUM_SHIFT           (0U)
#define JPEG_ENC_SWREG_106_REUSE_FRAME_NUM_MASK            (0xFFFFU << JPEG_ENC_SWREG_106_REUSE_FRAME_NUM_SHIFT)        /* 0x0000FFFF */
#define JPEG_ENC_SWREG_106_REUSE_INTRA_PRED_MODE_SHIFT     (16U)
#define JPEG_ENC_SWREG_106_REUSE_INTRA_PRED_MODE_MASK      (0xFFU << JPEG_ENC_SWREG_106_REUSE_INTRA_PRED_MODE_SHIFT)    /* 0x00FF0000 */
#define JPEG_ENC_SWREG_106_REUSE_PIC_PARA_ID_SHIFT         (24U)
#define JPEG_ENC_SWREG_106_REUSE_PIC_PARA_ID_MASK          (0xFFU << JPEG_ENC_SWREG_106_REUSE_PIC_PARA_ID_SHIFT)        /* 0xFF000000 */
/* SWREG_107_REUSE */
#define JPEG_ENC_SWREG_107_REUSE_OFFSET                    (0x1ACU)
#define JPEG_ENC_SWREG_107_REUSE_MV_PLY_8X4_4X8_SHIFT      (0U)
#define JPEG_ENC_SWREG_107_REUSE_MV_PLY_8X4_4X8_MASK       (0x3FFU << JPEG_ENC_SWREG_107_REUSE_MV_PLY_8X4_4X8_SHIFT)    /* 0x000003FF */
#define JPEG_ENC_SWREG_107_REUSE_MV_PLY_8X8_SHIFT          (10U)
#define JPEG_ENC_SWREG_107_REUSE_MV_PLY_8X8_MASK           (0x3FFU << JPEG_ENC_SWREG_107_REUSE_MV_PLY_8X8_SHIFT)        /* 0x000FFC00 */
#define JPEG_ENC_SWREG_107_REUSE_MV_PLY_16X8_8X16_SHIFT    (20U)
#define JPEG_ENC_SWREG_107_REUSE_MV_PLY_16X8_8X16_MASK     (0x3FFU << JPEG_ENC_SWREG_107_REUSE_MV_PLY_16X8_8X16_SHIFT)  /* 0x3FF00000 */
/* SWREG_108_REUSE */
#define JPEG_ENC_SWREG_108_REUSE_OFFSET                    (0x1B0U)
#define JPEG_ENC_SWREG_108_REUSE_INTRA_SLICE_BMP2_SHIFT    (0U)
#define JPEG_ENC_SWREG_108_REUSE_INTRA_SLICE_BMP2_MASK     (0xFFFFFFFFU << JPEG_ENC_SWREG_108_REUSE_INTRA_SLICE_BMP2_SHIFT) /* 0xFFFFFFFF */
/* SWREG_109 */
#define JPEG_ENC_SWREG_109_OFFSET                          (0x1B4U)
#define JPEG_ENC_SWREG_109_ENC_IRQ_SHIFT                   (0U)
#define JPEG_ENC_SWREG_109_ENC_IRQ_MASK                    (0x1U << JPEG_ENC_SWREG_109_ENC_IRQ_SHIFT)                   /* 0x00000001 */
#define JPEG_ENC_SWREG_109_IRQ_FRAME_RDY_SHIFT             (1U)
#define JPEG_ENC_SWREG_109_IRQ_FRAME_RDY_MASK              (0x1U << JPEG_ENC_SWREG_109_IRQ_FRAME_RDY_SHIFT)             /* 0x00000002 */
#define JPEG_ENC_SWREG_109_IRQ_SLICE_READY_SHIFT           (2U)
#define JPEG_ENC_SWREG_109_IRQ_SLICE_READY_MASK            (0x1U << JPEG_ENC_SWREG_109_IRQ_SLICE_READY_SHIFT)           /* 0x00000004 */
#define JPEG_ENC_SWREG_109_FUSE_INT_SHIFT                  (3U)
#define JPEG_ENC_SWREG_109_FUSE_INT_MASK                   (0x1U << JPEG_ENC_SWREG_109_FUSE_INT_SHIFT)                  /* 0x00000008 */
#define JPEG_ENC_SWREG_109_IRQ_BUS_ERROR_SHIFT             (4U)
#define JPEG_ENC_SWREG_109_IRQ_BUS_ERROR_MASK              (0x1U << JPEG_ENC_SWREG_109_IRQ_BUS_ERROR_SHIFT)             /* 0x00000010 */
#define JPEG_ENC_SWREG_109_IRQ_BUFFER_FULL_SHIFT           (5U)
#define JPEG_ENC_SWREG_109_IRQ_BUFFER_FULL_MASK            (0x1U << JPEG_ENC_SWREG_109_IRQ_BUFFER_FULL_SHIFT)           /* 0x00000020 */
#define JPEG_ENC_SWREG_109_IRQ_TIMEOUT_SHIFT               (6U)
#define JPEG_ENC_SWREG_109_IRQ_TIMEOUT_MASK                (0x1U << JPEG_ENC_SWREG_109_IRQ_TIMEOUT_SHIFT)               /* 0x00000040 */
#define JPEG_ENC_SWREG_109_IRQ_DIS_SHIFT                   (8U)
#define JPEG_ENC_SWREG_109_IRQ_DIS_MASK                    (0x1U << JPEG_ENC_SWREG_109_IRQ_DIS_SHIFT)                   /* 0x00000100 */
#define JPEG_ENC_SWREG_109_IRQ_CLR_SHIFT                   (9U)
#define JPEG_ENC_SWREG_109_IRQ_CLR_MASK                    (0x1U << JPEG_ENC_SWREG_109_IRQ_CLR_SHIFT)                   /* 0x00000200 */
#define JPEG_ENC_SWREG_109_INT_TIMEOUT_EN_SHIFT            (10U)
#define JPEG_ENC_SWREG_109_INT_TIMEOUT_EN_MASK             (0x1U << JPEG_ENC_SWREG_109_INT_TIMEOUT_EN_SHIFT)            /* 0x00000400 */
#define JPEG_ENC_SWREG_109_CLK_GATING_EN_SHIFT             (12U)
#define JPEG_ENC_SWREG_109_CLK_GATING_EN_MASK              (0x1U << JPEG_ENC_SWREG_109_CLK_GATING_EN_SHIFT)             /* 0x00001000 */
#define JPEG_ENC_SWREG_109_SLICE_RDYINT_EN_SHIFT           (16U)
#define JPEG_ENC_SWREG_109_SLICE_RDYINT_EN_MASK            (0x1U << JPEG_ENC_SWREG_109_SLICE_RDYINT_EN_SHIFT)           /* 0x00010000 */
#define JPEG_ENC_SWREG_109_ROCON_WRITE_DIS_SHIFT           (20U)
#define JPEG_ENC_SWREG_109_ROCON_WRITE_DIS_MASK            (0x1U << JPEG_ENC_SWREG_109_ROCON_WRITE_DIS_SHIFT)           /* 0x00100000 */
#define JPEG_ENC_SWREG_109_MV_SAD_WREN_SHIFT               (24U)
#define JPEG_ENC_SWREG_109_MV_SAD_WREN_MASK                (0x1U << JPEG_ENC_SWREG_109_MV_SAD_WREN_SHIFT)               /* 0x01000000 */
#define JPEG_ENC_SWREG_109_INT_NON_SHIFT                   (28U)
#define JPEG_ENC_SWREG_109_INT_NON_MASK                    (0x1U << JPEG_ENC_SWREG_109_INT_NON_SHIFT)                   /* 0x10000000 */
/* SWREG_110_READ */
#define JPEG_ENC_SWREG_110_READ_OFFSET                     (0x1B8U)
#define JPEG_ENC_SWREG_110_READ                            (0x48311220U)
#define JPEG_ENC_SWREG_110_READ_SYNTHESIS_SHIFT            (0U)
#define JPEG_ENC_SWREG_110_READ_SYNTHESIS_MASK             (0xFU << JPEG_ENC_SWREG_110_READ_SYNTHESIS_SHIFT)            /* 0x0000000F */
#define JPEG_ENC_SWREG_110_READ_MINOR_NUM_SHIFT            (4U)
#define JPEG_ENC_SWREG_110_READ_MINOR_NUM_MASK             (0xFFU << JPEG_ENC_SWREG_110_READ_MINOR_NUM_SHIFT)           /* 0x00000FF0 */
#define JPEG_ENC_SWREG_110_READ_MAJOR_NUM_SHIFT            (12U)
#define JPEG_ENC_SWREG_110_READ_MAJOR_NUM_MASK             (0xFU << JPEG_ENC_SWREG_110_READ_MAJOR_NUM_SHIFT)            /* 0x0000F000 */
#define JPEG_ENC_SWREG_110_READ_PROD_ID_SHIFT              (16U)
#define JPEG_ENC_SWREG_110_READ_PROD_ID_MASK               (0xFFFFU << JPEG_ENC_SWREG_110_READ_PROD_ID_SHIFT)           /* 0xFFFF0000 */
/* SWREG_120_183 */
#define JPEG_ENC_SWREG_120_183_OFFSET                      (0x1E0U)
#define JPEG_ENC_SWREG_120_183_DMV_PLY_TABLE_SHIFT         (0U)
#define JPEG_ENC_SWREG_120_183_DMV_PLY_TABLE_MASK          (0xFFFFFFFFU << JPEG_ENC_SWREG_120_183_DMV_PLY_TABLE_SHIFT)  /* 0xFFFFFFFF */
/***************************************VICAP_LITE***************************************/
/* MIPI_ID0_CTRL0 */
#define VICAP_LITE_MIPI_ID0_CTRL0_OFFSET                   (0x80U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_CAP_EN_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_CAP_EN_ID0_MASK  (0x1U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_CAP_EN_ID0_SHIFT) /* 0x00000001 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_WRDDR_TYPE_ID0_SHIFT (1U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_WRDDR_TYPE_ID0_MASK (0x7U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_WRDDR_TYPE_ID0_SHIFT) /* 0x0000000E */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID0_SHIFT (4U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID0_MASK (0x1U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID0_SHIFT) /* 0x00000010 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_CROP_EN_ID0_SHIFT (5U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_CROP_EN_ID0_MASK (0x1U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_CROP_EN_ID0_SHIFT) /* 0x00000020 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_COMPACT_ID0_SHIFT (6U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_COMPACT_ID0_MASK (0x1U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_COMPACT_ID0_SHIFT) /* 0x00000040 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_UV_SWAP_ID0_SHIFT (7U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_UV_SWAP_ID0_MASK (0x1U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_UV_SWAP_ID0_SHIFT) /* 0x00000080 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_VC_ID0_SHIFT     (8U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_VC_ID0_MASK      (0x3U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_VC_ID0_SHIFT)     /* 0x00000300 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_DT_ID0_SHIFT     (10U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_DT_ID0_MASK      (0x3FU << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_DT_ID0_SHIFT)    /* 0x0000FC00 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_YUYV_ORDER_ID0_SHIFT (16U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_YUYV_ORDER_ID0_MASK (0x3U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_YUYV_ORDER_ID0_SHIFT) /* 0x00030000 */
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_ALIGN_ID0_SHIFT  (31U)
#define VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_ALIGN_ID0_MASK   (0x1U << VICAP_LITE_MIPI_ID0_CTRL0_SW_MIPI_ALIGN_ID0_SHIFT)  /* 0x80000000 */
/* MIPI_ID0_CTRL1 */
#define VICAP_LITE_MIPI_ID0_CTRL1_OFFSET                   (0x84U)
#define VICAP_LITE_MIPI_ID0_CTRL1_SW_WIDTH_ID0_SHIFT       (0U)
#define VICAP_LITE_MIPI_ID0_CTRL1_SW_WIDTH_ID0_MASK        (0x3FFFU << VICAP_LITE_MIPI_ID0_CTRL1_SW_WIDTH_ID0_SHIFT)    /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID0_CTRL1_SW_HEIGHT_ID0_SHIFT      (16U)
#define VICAP_LITE_MIPI_ID0_CTRL1_SW_HEIGHT_ID0_MASK       (0x3FFFU << VICAP_LITE_MIPI_ID0_CTRL1_SW_HEIGHT_ID0_SHIFT)   /* 0x3FFF0000 */
/* MIPI_ID1_CTRL0 */
#define VICAP_LITE_MIPI_ID1_CTRL0_OFFSET                   (0x88U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_CAP_EN_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_CAP_EN_ID1_MASK  (0x1U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_CAP_EN_ID1_SHIFT) /* 0x00000001 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_WRDDR_TYPE_ID1_SHIFT (1U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_WRDDR_TYPE_ID1_MASK (0x7U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_WRDDR_TYPE_ID1_SHIFT) /* 0x0000000E */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID1_SHIFT (4U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID1_MASK (0x1U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID1_SHIFT) /* 0x00000010 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_CROP_EN_ID1_SHIFT (5U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_CROP_EN_ID1_MASK (0x1U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_CROP_EN_ID1_SHIFT) /* 0x00000020 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_COMPACT_ID1_SHIFT (6U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_COMPACT_ID1_MASK (0x1U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_COMPACT_ID1_SHIFT) /* 0x00000040 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_UV_SWAP_ID1_SHIFT (7U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_UV_SWAP_ID1_MASK (0x1U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_UV_SWAP_ID1_SHIFT) /* 0x00000080 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_VC_ID1_SHIFT     (8U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_VC_ID1_MASK      (0x3U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_VC_ID1_SHIFT)     /* 0x00000300 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_DT_ID1_SHIFT     (10U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_DT_ID1_MASK      (0x3FU << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_DT_ID1_SHIFT)    /* 0x0000FC00 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_YUYV_ORDER_ID1_SHIFT (16U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_YUYV_ORDER_ID1_MASK (0x3U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_YUYV_ORDER_ID1_SHIFT) /* 0x00030000 */
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_ALIGN_ID1_SHIFT  (31U)
#define VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_ALIGN_ID1_MASK   (0x1U << VICAP_LITE_MIPI_ID1_CTRL0_SW_MIPI_ALIGN_ID1_SHIFT)  /* 0x80000000 */
/* MIPI_ID1_CTRL1 */
#define VICAP_LITE_MIPI_ID1_CTRL1_OFFSET                   (0x8CU)
#define VICAP_LITE_MIPI_ID1_CTRL1_SW_WIDTH_ID1_SHIFT       (0U)
#define VICAP_LITE_MIPI_ID1_CTRL1_SW_WIDTH_ID1_MASK        (0x3FFFU << VICAP_LITE_MIPI_ID1_CTRL1_SW_WIDTH_ID1_SHIFT)    /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID1_CTRL1_SW_HEIGHT_ID1_SHIFT      (16U)
#define VICAP_LITE_MIPI_ID1_CTRL1_SW_HEIGHT_ID1_MASK       (0x3FFFU << VICAP_LITE_MIPI_ID1_CTRL1_SW_HEIGHT_ID1_SHIFT)   /* 0x3FFF0000 */
/* MIPI_ID2_CTRL0 */
#define VICAP_LITE_MIPI_ID2_CTRL0_OFFSET                   (0x90U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_CAP_EN_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_CAP_EN_ID2_MASK  (0x1U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_CAP_EN_ID2_SHIFT) /* 0x00000001 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_WRDDR_TYPE_ID2_SHIFT (1U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_WRDDR_TYPE_ID2_MASK (0x7U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_WRDDR_TYPE_ID2_SHIFT) /* 0x0000000E */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID2_SHIFT (4U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID2_MASK (0x1U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID2_SHIFT) /* 0x00000010 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_CROP_EN_ID2_SHIFT (5U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_CROP_EN_ID2_MASK (0x1U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_CROP_EN_ID2_SHIFT) /* 0x00000020 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_COMPACT_ID2_SHIFT (6U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_COMPACT_ID2_MASK (0x1U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_COMPACT_ID2_SHIFT) /* 0x00000040 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_UV_SWAP_ID2_SHIFT (7U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_UV_SWAP_ID2_MASK (0x1U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_UV_SWAP_ID2_SHIFT) /* 0x00000080 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_VC_ID2_SHIFT     (8U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_VC_ID2_MASK      (0x3U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_VC_ID2_SHIFT)     /* 0x00000300 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_DT_ID2_SHIFT     (10U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_DT_ID2_MASK      (0x3FU << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_DT_ID2_SHIFT)    /* 0x0000FC00 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_YUYV_ORDER_ID2_SHIFT (16U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_YUYV_ORDER_ID2_MASK (0x3U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_YUYV_ORDER_ID2_SHIFT) /* 0x00030000 */
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_ALIGN_ID2_SHIFT  (31U)
#define VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_ALIGN_ID2_MASK   (0x1U << VICAP_LITE_MIPI_ID2_CTRL0_SW_MIPI_ALIGN_ID2_SHIFT)  /* 0x80000000 */
/* MIPI_ID2_CTRL1 */
#define VICAP_LITE_MIPI_ID2_CTRL1_OFFSET                   (0x94U)
#define VICAP_LITE_MIPI_ID2_CTRL1_SW_WIDTH_ID2_SHIFT       (0U)
#define VICAP_LITE_MIPI_ID2_CTRL1_SW_WIDTH_ID2_MASK        (0x3FFFU << VICAP_LITE_MIPI_ID2_CTRL1_SW_WIDTH_ID2_SHIFT)    /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID2_CTRL1_SW_HEIGHT_ID2_SHIFT      (16U)
#define VICAP_LITE_MIPI_ID2_CTRL1_SW_HEIGHT_ID2_MASK       (0x3FFFU << VICAP_LITE_MIPI_ID2_CTRL1_SW_HEIGHT_ID2_SHIFT)   /* 0x3FFF0000 */
/* MIPI_ID3_CTRL0 */
#define VICAP_LITE_MIPI_ID3_CTRL0_OFFSET                   (0x98U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_CAP_EN_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_CAP_EN_ID3_MASK  (0x1U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_CAP_EN_ID3_SHIFT) /* 0x00000001 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_WRDDR_TYPE_ID3_SHIFT (1U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_WRDDR_TYPE_ID3_MASK (0x7U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_WRDDR_TYPE_ID3_SHIFT) /* 0x0000000E */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID3_SHIFT (4U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID3_MASK (0x1U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_COMMAND_MODE_EN_ID3_SHIFT) /* 0x00000010 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_CROP_EN_ID3_SHIFT (5U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_CROP_EN_ID3_MASK (0x1U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_CROP_EN_ID3_SHIFT) /* 0x00000020 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_COMPACT_ID3_SHIFT (6U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_COMPACT_ID3_MASK (0x1U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_COMPACT_ID3_SHIFT) /* 0x00000040 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_UV_SWAP_ID3_SHIFT (7U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_UV_SWAP_ID3_MASK (0x1U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_UV_SWAP_ID3_SHIFT) /* 0x00000080 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_VC_ID3_SHIFT     (8U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_VC_ID3_MASK      (0x3U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_VC_ID3_SHIFT)     /* 0x00000300 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_DT_ID3_SHIFT     (10U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_DT_ID3_MASK      (0x3FU << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_DT_ID3_SHIFT)    /* 0x0000FC00 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_YUYV_ORDER_ID3_SHIFT (16U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_YUYV_ORDER_ID3_MASK (0x3U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_YUYV_ORDER_ID3_SHIFT) /* 0x00030000 */
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_ALIGN_ID3_SHIFT  (31U)
#define VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_ALIGN_ID3_MASK   (0x1U << VICAP_LITE_MIPI_ID3_CTRL0_SW_MIPI_ALIGN_ID3_SHIFT)  /* 0x80000000 */
/* MIPI_ID3_CTRL1 */
#define VICAP_LITE_MIPI_ID3_CTRL1_OFFSET                   (0x9CU)
#define VICAP_LITE_MIPI_ID3_CTRL1_SW_WIDTH_ID3_SHIFT       (0U)
#define VICAP_LITE_MIPI_ID3_CTRL1_SW_WIDTH_ID3_MASK        (0x3FFFU << VICAP_LITE_MIPI_ID3_CTRL1_SW_WIDTH_ID3_SHIFT)    /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID3_CTRL1_SW_HEIGHT_ID3_SHIFT      (16U)
#define VICAP_LITE_MIPI_ID3_CTRL1_SW_HEIGHT_ID3_MASK       (0x3FFFU << VICAP_LITE_MIPI_ID3_CTRL1_SW_HEIGHT_ID3_SHIFT)   /* 0x3FFF0000 */
/* MIPI_CTRL */
#define VICAP_LITE_MIPI_CTRL_OFFSET                        (0xA0U)
#define VICAP_LITE_MIPI_CTRL_SW_WATER_LINE_EN_SHIFT        (0U)
#define VICAP_LITE_MIPI_CTRL_SW_WATER_LINE_EN_MASK         (0x1U << VICAP_LITE_MIPI_CTRL_SW_WATER_LINE_EN_SHIFT)        /* 0x00000001 */
#define VICAP_LITE_MIPI_CTRL_SW_WATER_LINE_SHIFT           (1U)
#define VICAP_LITE_MIPI_CTRL_SW_WATER_LINE_MASK            (0x3U << VICAP_LITE_MIPI_CTRL_SW_WATER_LINE_SHIFT)           /* 0x00000006 */
#define VICAP_LITE_MIPI_CTRL_SW_HURRY_EN_SHIFT             (4U)
#define VICAP_LITE_MIPI_CTRL_SW_HURRY_EN_MASK              (0x1U << VICAP_LITE_MIPI_CTRL_SW_HURRY_EN_SHIFT)             /* 0x00000010 */
#define VICAP_LITE_MIPI_CTRL_SW_HURRY_VALUE_SHIFT          (5U)
#define VICAP_LITE_MIPI_CTRL_SW_HURRY_VALUE_MASK           (0x3U << VICAP_LITE_MIPI_CTRL_SW_HURRY_VALUE_SHIFT)          /* 0x00000060 */
#define VICAP_LITE_MIPI_CTRL_SW_PRESS_EN_SHIFT             (12U)
#define VICAP_LITE_MIPI_CTRL_SW_PRESS_EN_MASK              (0x1U << VICAP_LITE_MIPI_CTRL_SW_PRESS_EN_SHIFT)             /* 0x00001000 */
#define VICAP_LITE_MIPI_CTRL_SW_PRESS_VALUE_SHIFT          (13U)
#define VICAP_LITE_MIPI_CTRL_SW_PRESS_VALUE_MASK           (0x3U << VICAP_LITE_MIPI_CTRL_SW_PRESS_VALUE_SHIFT)          /* 0x00006000 */
#define VICAP_LITE_MIPI_CTRL_SW_DMA_IDLE_SHIFT             (16U)
#define VICAP_LITE_MIPI_CTRL_SW_DMA_IDLE_MASK              (0x1U << VICAP_LITE_MIPI_CTRL_SW_DMA_IDLE_SHIFT)             /* 0x00010000 */
/* MIPI_FRAME0_ADDR_Y_ID0 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID0_OFFSET           (0xA4U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID0_SW_FRAME0_ADDR_Y_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID0_SW_FRAME0_ADDR_Y_ID0_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID0_SW_FRAME0_ADDR_Y_ID0_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_Y_ID0 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID0_OFFSET           (0xA8U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID0_SW_FRAME1_ADDR_Y_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID0_SW_FRAME1_ADDR_Y_ID0_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID0_SW_FRAME1_ADDR_Y_ID0_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_ADDR_UV_ID0 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID0_OFFSET          (0xACU)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID0_SW_FRAME0_ADDR_UV_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID0_SW_FRAME0_ADDR_UV_ID0_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID0_SW_FRAME0_ADDR_UV_ID0_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_UV_ID0 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID0_OFFSET          (0xB0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID0_SW_FRAME1_ADDR_UV_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID0_SW_FRAME1_ADDR_UV_ID0_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID0_SW_FRAME1_ADDR_UV_ID0_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_VLW_Y_ID0 */
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID0_OFFSET            (0xB4U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID0_SW_FRAME0_VLW_Y_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID0_SW_FRAME0_VLW_Y_ID0_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_Y_ID0_SW_FRAME0_VLW_Y_ID0_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_Y_ID0 */
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID0_OFFSET            (0xB8U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID0_SW_FRAME1_VLW_Y_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID0_SW_FRAME1_VLW_Y_ID0_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_Y_ID0_SW_FRAME1_VLW_Y_ID0_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_VLW_UV_ID0 */
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID0_OFFSET           (0xBCU)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID0_SW_FRAME0_VLW_UV_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID0_SW_FRAME0_VLW_UV_ID0_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_UV_ID0_SW_FRAME0_VLW_UV_ID0_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_UV_ID0 */
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID0_OFFSET           (0xC0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID0_SW_FRAME1_VLW_UV_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID0_SW_FRAME1_VLW_UV_ID0_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_UV_ID0_SW_FRAME1_VLW_UV_ID0_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_ADDR_Y_ID1 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID1_OFFSET           (0xC4U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID1_SW_FRAME0_ADDR_Y_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID1_SW_FRAME0_ADDR_Y_ID1_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID1_SW_FRAME0_ADDR_Y_ID1_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_Y_ID1 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID1_OFFSET           (0xC8U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID1_SW_FRAME1_ADDR_Y_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID1_SW_FRAME1_ADDR_Y_ID1_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID1_SW_FRAME1_ADDR_Y_ID1_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_ADDR_UV_ID1 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID1_OFFSET          (0xCCU)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID1_SW_FRAME0_ADDR_UV_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID1_SW_FRAME0_ADDR_UV_ID1_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID1_SW_FRAME0_ADDR_UV_ID1_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_UV_ID1 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID1_OFFSET          (0xD0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID1_SW_FRAME1_ADDR_UV_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID1_SW_FRAME1_ADDR_UV_ID1_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID1_SW_FRAME1_ADDR_UV_ID1_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_VLW_Y_ID1 */
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID1_OFFSET            (0xD4U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID1_SW_FRAME0_VLW_Y_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID1_SW_FRAME0_VLW_Y_ID1_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_Y_ID1_SW_FRAME0_VLW_Y_ID1_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_Y_ID1 */
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID1_OFFSET            (0xD8U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID1_SW_FRAME1_VLW_Y_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID1_SW_FRAME1_VLW_Y_ID1_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_Y_ID1_SW_FRAME1_VLW_Y_ID1_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_VLW_UV_ID1 */
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID1_OFFSET           (0xDCU)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID1_SW_FRAME0_VLW_UV_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID1_SW_FRAME0_VLW_UV_ID1_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_UV_ID1_SW_FRAME0_VLW_UV_ID1_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_UV_ID1 */
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID1_OFFSET           (0xE0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID1_SW_FRAME1_VLW_UV_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID1_SW_FRAME1_VLW_UV_ID1_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_UV_ID1_SW_FRAME1_VLW_UV_ID1_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_ADDR_Y_ID2 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID2_OFFSET           (0xE4U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID2_SW_FRAME0_ADDR_Y_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID2_SW_FRAME0_ADDR_Y_ID2_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID2_SW_FRAME0_ADDR_Y_ID2_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_Y_ID2 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID2_OFFSET           (0xE8U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID2_SW_FRAME1_ADDR_Y_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID2_SW_FRAME1_ADDR_Y_ID2_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID2_SW_FRAME1_ADDR_Y_ID2_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_ADDR_UV_ID2 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID2_OFFSET          (0xECU)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID2_SW_FRAME0_ADDR_UV_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID2_SW_FRAME0_ADDR_UV_ID2_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID2_SW_FRAME0_ADDR_UV_ID2_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_UV_ID2 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID2_OFFSET          (0xF0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID2_SW_FRAME1_ADDR_UV_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID2_SW_FRAME1_ADDR_UV_ID0_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID2_SW_FRAME1_ADDR_UV_ID0_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_VLW_Y_ID2 */
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID2_OFFSET            (0xF4U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID2_SW_FRAME0_VLW_Y_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID2_SW_FRAME0_VLW_Y_ID2_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_Y_ID2_SW_FRAME0_VLW_Y_ID2_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_Y_ID2 */
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID2_OFFSET            (0xF8U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID2_SW_FRAME1_VLW_Y_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID2_SW_FRAME1_VLW_Y_ID2_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_Y_ID2_SW_FRAME1_VLW_Y_ID2_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_VLW_UV_ID2 */
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID2_OFFSET           (0xFCU)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID2_SW_FRAME0_VLW_UV_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID2_SW_FRAME0_VLW_UV_ID2_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_UV_ID2_SW_FRAME0_VLW_UV_ID2_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_UV_ID2 */
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID2_OFFSET           (0x100U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID2_SW_FRAME1_VLW_UV_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID2_SW_FRAME1_VLW_UV_ID2_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_UV_ID2_SW_FRAME1_VLW_UV_ID2_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_ADDR_Y_ID3 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID3_OFFSET           (0x104U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID3_SW_FRAME0_ADDR_Y_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID3_SW_FRAME0_ADDR_Y_ID3_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_Y_ID3_SW_FRAME0_ADDR_Y_ID3_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_Y_ID3 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID3_OFFSET           (0x108U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID3_SW_FRAME1_ADDR_Y_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID3_SW_FRAME1_ADDR_Y_ID3_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_Y_ID3_SW_FRAME1_ADDR_Y_ID3_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_ADDR_UV_ID3 */
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID3_OFFSET          (0x10CU)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID3_SW_FRAME0_ADDR_UV_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID3_SW_FRAME0_ADDR_UV_ID3_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME0_ADDR_UV_ID3_SW_FRAME0_ADDR_UV_ID3_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME1_ADDR_UV_ID3 */
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID3_OFFSET          (0x110U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID3_SW_FRAME1_ADDR_UV_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID3_SW_FRAME1_ADDR_UV_ID3_MASK (0xFFFFFFFFU << VICAP_LITE_MIPI_FRAME1_ADDR_UV_ID3_SW_FRAME1_ADDR_UV_ID3_SHIFT) /* 0xFFFFFFFF */
/* MIPI_FRAME0_VLW_Y_ID3 */
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID3_OFFSET            (0x114U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID3_SW_FRAME0_VLW_Y_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_Y_ID3_SW_FRAME0_VLW_Y_ID3_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_Y_ID3_SW_FRAME0_VLW_Y_ID3_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_Y_ID3 */
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID3_OFFSET            (0x118U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID3_SW_FRAME1_VLW_Y_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_Y_ID3_SW_FRAME1_VLW_Y_ID0_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_Y_ID3_SW_FRAME1_VLW_Y_ID0_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME0_VLW_UV_ID3 */
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID3_OFFSET           (0x11CU)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID3_SW_FRAME0_VLW_UV_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME0_VLW_UV_ID3_SW_FRAME0_VLW_UV_ID3_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME0_VLW_UV_ID3_SW_FRAME0_VLW_UV_ID3_SHIFT) /* 0x000FFFFF */
/* MIPI_FRAME1_VLW_UV_ID3 */
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID3_OFFSET           (0x120U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID3_SW_FRAME1_VLW_UV_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME1_VLW_UV_ID3_SW_FRAME1_VLW_UV_ID3_MASK (0xFFFFFU << VICAP_LITE_MIPI_FRAME1_VLW_UV_ID3_SW_FRAME1_VLW_UV_ID3_SHIFT) /* 0x000FFFFF */
/* MIPI_INTEN */
#define VICAP_LITE_MIPI_INTEN_OFFSET                       (0x124U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID0_INTEN_SHIFT (0U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID0_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_START_ID0_INTEN_SHIFT) /* 0x00000001 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID0_INTEN_SHIFT (1U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID0_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_START_ID0_INTEN_SHIFT) /* 0x00000002 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID1_INTEN_SHIFT (2U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID1_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_START_ID1_INTEN_SHIFT) /* 0x00000004 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID1_INTEN_SHIFT (3U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID1_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_START_ID1_INTEN_SHIFT) /* 0x00000008 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID2_INTEN_SHIFT (4U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID2_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_START_ID2_INTEN_SHIFT) /* 0x00000010 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID2_INTEN_SHIFT (5U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID2_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_START_ID2_INTEN_SHIFT) /* 0x00000020 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID3_INTEN_SHIFT (6U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_START_ID3_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_START_ID3_INTEN_SHIFT) /* 0x00000040 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID3_INTEN_SHIFT (7U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_START_ID3_INTEN_MASK  (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_START_ID3_INTEN_SHIFT) /* 0x00000080 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID0_INTEN_SHIFT (8U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID0_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID0_INTEN_SHIFT) /* 0x00000100 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID0_INTEN_SHIFT (9U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID0_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID0_INTEN_SHIFT) /* 0x00000200 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID1_INTEN_SHIFT (10U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID1_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID1_INTEN_SHIFT) /* 0x00000400 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID1_INTEN_SHIFT (11U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID1_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID1_INTEN_SHIFT) /* 0x00000800 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID2_INTEN_SHIFT (12U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID2_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID2_INTEN_SHIFT) /* 0x00001000 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID2_INTEN_SHIFT (13U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID2_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID2_INTEN_SHIFT) /* 0x00002000 */
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID3_INTEN_SHIFT (14U)
#define VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID3_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME0_DMA_END_ID3_INTEN_SHIFT) /* 0x00004000 */
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID3_INTEN_SHIFT (15U)
#define VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID3_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_FRAME1_DMA_END_ID3_INTEN_SHIFT) /* 0x00008000 */
#define VICAP_LITE_MIPI_INTEN_DMA_Y_FIFO_OVERFLOW_INTEN_SHIFT (16U)
#define VICAP_LITE_MIPI_INTEN_DMA_Y_FIFO_OVERFLOW_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_DMA_Y_FIFO_OVERFLOW_INTEN_SHIFT) /* 0x00010000 */
#define VICAP_LITE_MIPI_INTEN_DMA_UV_FIFO_OVERFLOW_INTEN_SHIFT (17U)
#define VICAP_LITE_MIPI_INTEN_DMA_UV_FIFO_OVERFLOW_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_DMA_UV_FIFO_OVERFLOW_INTEN_SHIFT) /* 0x00020000 */
#define VICAP_LITE_MIPI_INTEN_CONFIG_FIFO_OVERFLOW_INTEN_SHIFT (18U)
#define VICAP_LITE_MIPI_INTEN_CONFIG_FIFO_OVERFLOW_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_CONFIG_FIFO_OVERFLOW_INTEN_SHIFT) /* 0x00040000 */
#define VICAP_LITE_MIPI_INTEN_BANDWIDTH_LACK_INTEN_SHIFT   (19U)
#define VICAP_LITE_MIPI_INTEN_BANDWIDTH_LACK_INTEN_MASK    (0x1U << VICAP_LITE_MIPI_INTEN_BANDWIDTH_LACK_INTEN_SHIFT)   /* 0x00080000 */
#define VICAP_LITE_MIPI_INTEN_CSI2RX_FIFO_OVERFLOW_INTEN_SHIFT (20U)
#define VICAP_LITE_MIPI_INTEN_CSI2RX_FIFO_OVERFLOW_INTEN_MASK (0x1U << VICAP_LITE_MIPI_INTEN_CSI2RX_FIFO_OVERFLOW_INTEN_SHIFT) /* 0x00100000 */
#define VICAP_LITE_MIPI_INTEN_LINE_ID0_INTEN_SHIFT         (21U)
#define VICAP_LITE_MIPI_INTEN_LINE_ID0_INTEN_MASK          (0x1U << VICAP_LITE_MIPI_INTEN_LINE_ID0_INTEN_SHIFT)         /* 0x00200000 */
#define VICAP_LITE_MIPI_INTEN_LINE_ID1_INTEN_SHIFT         (22U)
#define VICAP_LITE_MIPI_INTEN_LINE_ID1_INTEN_MASK          (0x1U << VICAP_LITE_MIPI_INTEN_LINE_ID1_INTEN_SHIFT)         /* 0x00400000 */
#define VICAP_LITE_MIPI_INTEN_LINE_ID2_INTEN_SHIFT         (23U)
#define VICAP_LITE_MIPI_INTEN_LINE_ID2_INTEN_MASK          (0x1U << VICAP_LITE_MIPI_INTEN_LINE_ID2_INTEN_SHIFT)         /* 0x00800000 */
#define VICAP_LITE_MIPI_INTEN_LINE_ID3_INTEN_SHIFT         (24U)
#define VICAP_LITE_MIPI_INTEN_LINE_ID3_INTEN_MASK          (0x1U << VICAP_LITE_MIPI_INTEN_LINE_ID3_INTEN_SHIFT)         /* 0x01000000 */
#define VICAP_LITE_MIPI_INTEN_BUS_ERR_INTEN_SHIFT          (25U)
#define VICAP_LITE_MIPI_INTEN_BUS_ERR_INTEN_MASK           (0x1U << VICAP_LITE_MIPI_INTEN_BUS_ERR_INTEN_SHIFT)          /* 0x02000000 */
/* MIPI_INTSTAT */
#define VICAP_LITE_MIPI_INTSTAT_OFFSET                     (0x128U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID0_INTST_SHIFT (0U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID0_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID0_INTST_SHIFT) /* 0x00000001 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID0_INTST_SHIFT (1U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID0_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID0_INTST_SHIFT) /* 0x00000002 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID1_INTST_SHIFT (2U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID1_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID1_INTST_SHIFT) /* 0x00000004 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID1_INTST_SHIFT (3U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID1_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID1_INTST_SHIFT) /* 0x00000008 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID2_INTST_SHIFT (4U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID2_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID2_INTST_SHIFT) /* 0x00000010 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID2_INTST_SHIFT (5U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID2_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID2_INTST_SHIFT) /* 0x00000020 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID3_INTST_SHIFT (6U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID3_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_START_ID3_INTST_SHIFT) /* 0x00000040 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID3_INTST_SHIFT (7U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID3_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_START_ID3_INTST_SHIFT) /* 0x00000080 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID0_INTST_SHIFT (8U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID0_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID0_INTST_SHIFT) /* 0x00000100 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID0_INTST_SHIFT (9U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID0_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID0_INTST_SHIFT) /* 0x00000200 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID1_INTST_SHIFT (10U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID1_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID1_INTST_SHIFT) /* 0x00000400 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID1_INTST_SHIFT (11U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID1_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID1_INTST_SHIFT) /* 0x00000800 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID2_INTST_SHIFT (12U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID2_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID2_INTST_SHIFT) /* 0x00001000 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID2_INTST_SHIFT (13U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID2_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID2_INTST_SHIFT) /* 0x00002000 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID3_INTST_SHIFT (14U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID3_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME0_DMA_END_ID3_INTST_SHIFT) /* 0x00004000 */
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID3_INTST_SHIFT (15U)
#define VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID3_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_FRAME1_DMA_END_ID3_INTST_SHIFT) /* 0x00008000 */
#define VICAP_LITE_MIPI_INTSTAT_DMA_Y_FIFO_OVERFLOW_INTST_SHIFT (16U)
#define VICAP_LITE_MIPI_INTSTAT_DMA_Y_FIFO_OVERFLOW_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_DMA_Y_FIFO_OVERFLOW_INTST_SHIFT) /* 0x00010000 */
#define VICAP_LITE_MIPI_INTSTAT_DMA_UV_FIFO_OVERFLOW_INTST_SHIFT (17U)
#define VICAP_LITE_MIPI_INTSTAT_DMA_UV_FIFO_OVERFLOW_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_DMA_UV_FIFO_OVERFLOW_INTST_SHIFT) /* 0x00020000 */
#define VICAP_LITE_MIPI_INTSTAT_CONFIG_FIFO_OVERFLOW_INTST_SHIFT (18U)
#define VICAP_LITE_MIPI_INTSTAT_CONFIG_FIFO_OVERFLOW_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_CONFIG_FIFO_OVERFLOW_INTST_SHIFT) /* 0x00040000 */
#define VICAP_LITE_MIPI_INTSTAT_BANDWIDTH_LACK_INTST_SHIFT (19U)
#define VICAP_LITE_MIPI_INTSTAT_BANDWIDTH_LACK_INTST_MASK  (0x1U << VICAP_LITE_MIPI_INTSTAT_BANDWIDTH_LACK_INTST_SHIFT) /* 0x00080000 */
#define VICAP_LITE_MIPI_INTSTAT_CSI2RX_FIFO_OVERFLOW_INTST_SHIFT (20U)
#define VICAP_LITE_MIPI_INTSTAT_CSI2RX_FIFO_OVERFLOW_INTST_MASK (0x1U << VICAP_LITE_MIPI_INTSTAT_CSI2RX_FIFO_OVERFLOW_INTST_SHIFT) /* 0x00100000 */
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID0_INTST_SHIFT       (21U)
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID0_INTST_MASK        (0x1U << VICAP_LITE_MIPI_INTSTAT_LINE_ID0_INTST_SHIFT)       /* 0x00200000 */
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID1_INTST_SHIFT       (22U)
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID1_INTST_MASK        (0x1U << VICAP_LITE_MIPI_INTSTAT_LINE_ID1_INTST_SHIFT)       /* 0x00400000 */
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID2_INTST_SHIFT       (23U)
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID2_INTST_MASK        (0x1U << VICAP_LITE_MIPI_INTSTAT_LINE_ID2_INTST_SHIFT)       /* 0x00800000 */
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID3_INTST_SHIFT       (24U)
#define VICAP_LITE_MIPI_INTSTAT_LINE_ID3_INTST_MASK        (0x1U << VICAP_LITE_MIPI_INTSTAT_LINE_ID3_INTST_SHIFT)       /* 0x01000000 */
#define VICAP_LITE_MIPI_INTSTAT_BUS_ERR_INTST_SHIFT        (25U)
#define VICAP_LITE_MIPI_INTSTAT_BUS_ERR_INTST_MASK         (0x1U << VICAP_LITE_MIPI_INTSTAT_BUS_ERR_INTST_SHIFT)        /* 0x02000000 */
/* MIPI_LINE_INT_NUM_ID0_1 */
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_OFFSET          (0x12CU)
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_LINE_INT_NUM_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_LINE_INT_NUM_ID0_MASK (0x3FFFU << VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_LINE_INT_NUM_ID0_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_LINE_INT_NUM_ID1_SHIFT (16U)
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_LINE_INT_NUM_ID1_MASK (0x3FFFU << VICAP_LITE_MIPI_LINE_INT_NUM_ID0_1_LINE_INT_NUM_ID1_SHIFT) /* 0x3FFF0000 */
/* MIPI_LINE_INT_NUM_ID2_3 */
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_OFFSET          (0x130U)
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_LINE_INT_NUM_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_LINE_INT_NUM_ID2_MASK (0x3FFFU << VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_LINE_INT_NUM_ID2_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_LINE_INT_NUM_ID3_SHIFT (16U)
#define VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_LINE_INT_NUM_ID3_MASK (0x3FFFU << VICAP_LITE_MIPI_LINE_INT_NUM_ID2_3_LINE_INT_NUM_ID3_SHIFT) /* 0x3FFF0000 */
/* MIPI_LINE_CNT_ID0_1 */
#define VICAP_LITE_MIPI_LINE_CNT_ID0_1_OFFSET              (0x134U)
#define VICAP_LITE_MIPI_LINE_CNT_ID0_1                     (0x0U)
#define VICAP_LITE_MIPI_LINE_CNT_ID0_1_LINE_CNT_ID0_SHIFT  (0U)
#define VICAP_LITE_MIPI_LINE_CNT_ID0_1_LINE_CNT_ID0_MASK   (0x3FFFU << VICAP_LITE_MIPI_LINE_CNT_ID0_1_LINE_CNT_ID0_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_LINE_CNT_ID0_1_LINE_CNT_ID1_SHIFT  (16U)
#define VICAP_LITE_MIPI_LINE_CNT_ID0_1_LINE_CNT_ID1_MASK   (0x3FFFU << VICAP_LITE_MIPI_LINE_CNT_ID0_1_LINE_CNT_ID1_SHIFT) /* 0x3FFF0000 */
/* MIPI_LINE_CNT_ID2_3 */
#define VICAP_LITE_MIPI_LINE_CNT_ID2_3_OFFSET              (0x138U)
#define VICAP_LITE_MIPI_LINE_CNT_ID2_3                     (0x0U)
#define VICAP_LITE_MIPI_LINE_CNT_ID2_3_LINE_CNT_ID2_SHIFT  (0U)
#define VICAP_LITE_MIPI_LINE_CNT_ID2_3_LINE_CNT_ID2_MASK   (0x3FFFU << VICAP_LITE_MIPI_LINE_CNT_ID2_3_LINE_CNT_ID2_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_LINE_CNT_ID2_3_LINE_CNT_ID3_SHIFT  (16U)
#define VICAP_LITE_MIPI_LINE_CNT_ID2_3_LINE_CNT_ID3_MASK   (0x3FFFU << VICAP_LITE_MIPI_LINE_CNT_ID2_3_LINE_CNT_ID3_SHIFT) /* 0x3FFF0000 */
/* MIPI_ID0_CROP_START */
#define VICAP_LITE_MIPI_ID0_CROP_START_OFFSET              (0x13CU)
#define VICAP_LITE_MIPI_ID0_CROP_START_SW_START_X_ID0_SHIFT (0U)
#define VICAP_LITE_MIPI_ID0_CROP_START_SW_START_X_ID0_MASK (0x3FFFU << VICAP_LITE_MIPI_ID0_CROP_START_SW_START_X_ID0_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID0_CROP_START_SW_START_Y_ID0_SHIFT (16U)
#define VICAP_LITE_MIPI_ID0_CROP_START_SW_START_Y_ID0_MASK (0x3FFFU << VICAP_LITE_MIPI_ID0_CROP_START_SW_START_Y_ID0_SHIFT) /* 0x3FFF0000 */
/* MIPI_ID1_CROP_START */
#define VICAP_LITE_MIPI_ID1_CROP_START_OFFSET              (0x140U)
#define VICAP_LITE_MIPI_ID1_CROP_START_SW_START_X_ID1_SHIFT (0U)
#define VICAP_LITE_MIPI_ID1_CROP_START_SW_START_X_ID1_MASK (0x3FFFU << VICAP_LITE_MIPI_ID1_CROP_START_SW_START_X_ID1_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID1_CROP_START_SW_START_Y_ID1_SHIFT (16U)
#define VICAP_LITE_MIPI_ID1_CROP_START_SW_START_Y_ID1_MASK (0x3FFFU << VICAP_LITE_MIPI_ID1_CROP_START_SW_START_Y_ID1_SHIFT) /* 0x3FFF0000 */
/* MIPI_ID2_CROP_START */
#define VICAP_LITE_MIPI_ID2_CROP_START_OFFSET              (0x144U)
#define VICAP_LITE_MIPI_ID2_CROP_START_SW_START_X_ID2_SHIFT (0U)
#define VICAP_LITE_MIPI_ID2_CROP_START_SW_START_X_ID2_MASK (0x3FFFU << VICAP_LITE_MIPI_ID2_CROP_START_SW_START_X_ID2_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID2_CROP_START_SW_START_Y_ID2_SHIFT (16U)
#define VICAP_LITE_MIPI_ID2_CROP_START_SW_START_Y_ID2_MASK (0x3FFFU << VICAP_LITE_MIPI_ID2_CROP_START_SW_START_Y_ID2_SHIFT) /* 0x3FFF0000 */
/* MIPI_ID3_CROP_START */
#define VICAP_LITE_MIPI_ID3_CROP_START_OFFSET              (0x148U)
#define VICAP_LITE_MIPI_ID3_CROP_START_SW_START_X_ID3_SHIFT (0U)
#define VICAP_LITE_MIPI_ID3_CROP_START_SW_START_X_ID3_MASK (0x3FFFU << VICAP_LITE_MIPI_ID3_CROP_START_SW_START_X_ID3_SHIFT) /* 0x00003FFF */
#define VICAP_LITE_MIPI_ID3_CROP_START_SW_START_Y_ID3_SHIFT (16U)
#define VICAP_LITE_MIPI_ID3_CROP_START_SW_START_Y_ID3_MASK (0x3FFFU << VICAP_LITE_MIPI_ID3_CROP_START_SW_START_Y_ID3_SHIFT) /* 0x3FFF0000 */
/* MIPI_FRAME_NUM_VC0 */
#define VICAP_LITE_MIPI_FRAME_NUM_VC0_OFFSET               (0x14CU)
#define VICAP_LITE_MIPI_FRAME_NUM_VC0                      (0x0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC0_RO_FRAME_NUM_START_VC0_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC0_RO_FRAME_NUM_START_VC0_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC0_RO_FRAME_NUM_START_VC0_SHIFT) /* 0x0000FFFF */
#define VICAP_LITE_MIPI_FRAME_NUM_VC0_RO_FRAME_NUM_END_VC0_SHIFT (16U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC0_RO_FRAME_NUM_END_VC0_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC0_RO_FRAME_NUM_END_VC0_SHIFT) /* 0xFFFF0000 */
/* MIPI_FRAME_NUM_VC1 */
#define VICAP_LITE_MIPI_FRAME_NUM_VC1_OFFSET               (0x150U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC1                      (0x0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC1_RO_FRAME_NUM_START_VC1_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC1_RO_FRAME_NUM_START_VC1_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC1_RO_FRAME_NUM_START_VC1_SHIFT) /* 0x0000FFFF */
#define VICAP_LITE_MIPI_FRAME_NUM_VC1_RO_FRAME_NUM_END_VC1_SHIFT (16U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC1_RO_FRAME_NUM_END_VC1_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC1_RO_FRAME_NUM_END_VC1_SHIFT) /* 0xFFFF0000 */
/* MIPI_FRAME_NUM_VC2 */
#define VICAP_LITE_MIPI_FRAME_NUM_VC2_OFFSET               (0x154U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC2                      (0x0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC2_RO_FRAME_NUM_START_VC2_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC2_RO_FRAME_NUM_START_VC2_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC2_RO_FRAME_NUM_START_VC2_SHIFT) /* 0x0000FFFF */
#define VICAP_LITE_MIPI_FRAME_NUM_VC2_RO_FRAME_NUM_END_VC2_SHIFT (16U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC2_RO_FRAME_NUM_END_VC2_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC2_RO_FRAME_NUM_END_VC2_SHIFT) /* 0xFFFF0000 */
/* MIPI_FRAME_NUM_VC3 */
#define VICAP_LITE_MIPI_FRAME_NUM_VC3_OFFSET               (0x158U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC3                      (0x0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC3_RO_FRAME_NUM_START_VC3_SHIFT (0U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC3_RO_FRAME_NUM_START_VC3_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC3_RO_FRAME_NUM_START_VC3_SHIFT) /* 0x0000FFFF */
#define VICAP_LITE_MIPI_FRAME_NUM_VC3_RO_FRAME_NUM_END_VC3_SHIFT (16U)
#define VICAP_LITE_MIPI_FRAME_NUM_VC3_RO_FRAME_NUM_END_VC3_MASK (0xFFFFU << VICAP_LITE_MIPI_FRAME_NUM_VC3_RO_FRAME_NUM_END_VC3_SHIFT) /* 0xFFFF0000 */
/*************************************ISP_MAIN_CTRL**************************************/
/* VI_CCL */
#define ISP_MAIN_CTRL_VI_CCL_OFFSET                        (0x0U)
#define ISP_MAIN_CTRL_VI_CCL_VI_CCL_DIS_STATUS_SHIFT       (1U)
#define ISP_MAIN_CTRL_VI_CCL_VI_CCL_DIS_STATUS_MASK        (0x1U << ISP_MAIN_CTRL_VI_CCL_VI_CCL_DIS_STATUS_SHIFT)       /* 0x00000002 */
#define ISP_MAIN_CTRL_VI_CCL_VI_CCL_DIS_SHIFT              (2U)
#define ISP_MAIN_CTRL_VI_CCL_VI_CCL_DIS_MASK               (0x1U << ISP_MAIN_CTRL_VI_CCL_VI_CCL_DIS_SHIFT)              /* 0x00000004 */
/* VI_ID */
#define ISP_MAIN_CTRL_VI_ID_OFFSET                         (0x8U)
#define ISP_MAIN_CTRL_VI_ID                                (0x10853017U)
#define ISP_MAIN_CTRL_VI_ID_REV_ID_SHIFT                   (0U)
#define ISP_MAIN_CTRL_VI_ID_REV_ID_MASK                    (0xFFFFFFFFU << ISP_MAIN_CTRL_VI_ID_REV_ID_SHIFT)            /* 0xFFFFFFFF */
/* VI_ICCL */
#define ISP_MAIN_CTRL_VI_ICCL_OFFSET                       (0x10U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_ISP_CLK_ENABLE_SHIFT      (0U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_ISP_CLK_ENABLE_MASK       (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_ISP_CLK_ENABLE_SHIFT)      /* 0x00000001 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_CP_CLK_ENABLE_SHIFT       (1U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_CP_CLK_ENABLE_MASK        (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_CP_CLK_ENABLE_SHIFT)       /* 0x00000002 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_MRSZ_CLK_ENABLE_SHIFT     (3U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_MRSZ_CLK_ENABLE_MASK      (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_MRSZ_CLK_ENABLE_SHIFT)     /* 0x00000008 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_SRSZ_CLK_ENABLE_SHIFT     (4U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_SRSZ_CLK_ENABLE_MASK      (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_SRSZ_CLK_ENABLE_SHIFT)     /* 0x00000010 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_JPEG_CLK_ENABLE_SHIFT     (5U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_JPEG_CLK_ENABLE_MASK      (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_JPEG_CLK_ENABLE_SHIFT)     /* 0x00000020 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_MI_CLK_ENABLE_SHIFT       (6U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_MI_CLK_ENABLE_MASK        (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_MI_CLK_ENABLE_SHIFT)       /* 0x00000040 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_IE_CLK_ENABLE_SHIFT       (8U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_IE_CLK_ENABLE_MASK        (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_IE_CLK_ENABLE_SHIFT)       /* 0x00000100 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_SIMP_CLK_ENABLE_SHIFT     (9U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_SIMP_CLK_ENABLE_MASK      (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_SIMP_CLK_ENABLE_SHIFT)     /* 0x00000200 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_SMIA_CLK_ENABLE_SHIFT     (10U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_SMIA_CLK_ENABLE_MASK      (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_SMIA_CLK_ENABLE_SHIFT)     /* 0x00000400 */
#define ISP_MAIN_CTRL_VI_ICCL_VI_MIPI_CLK_ENABLE_SHIFT     (11U)
#define ISP_MAIN_CTRL_VI_ICCL_VI_MIPI_CLK_ENABLE_MASK      (0x1U << ISP_MAIN_CTRL_VI_ICCL_VI_MIPI_CLK_ENABLE_SHIFT)     /* 0x00000800 */
/* VI_IRCL */
#define ISP_MAIN_CTRL_VI_IRCL_OFFSET                       (0x14U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_ISP_SOFT_RST_SHIFT        (0U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_ISP_SOFT_RST_MASK         (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_ISP_SOFT_RST_SHIFT)        /* 0x00000001 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_CP_SOFT_RST_SHIFT         (1U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_CP_SOFT_RST_MASK          (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_CP_SOFT_RST_SHIFT)         /* 0x00000002 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_YCS_SOFT_RST_SHIFT        (2U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_YCS_SOFT_RST_MASK         (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_YCS_SOFT_RST_SHIFT)        /* 0x00000004 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_MRSZ_SOFT_RST_SHIFT       (3U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_MRSZ_SOFT_RST_MASK        (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_MRSZ_SOFT_RST_SHIFT)       /* 0x00000008 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_SRSZ_SOFT_RST_SHIFT       (4U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_SRSZ_SOFT_RST_MASK        (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_SRSZ_SOFT_RST_SHIFT)       /* 0x00000010 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_JPEG_SOFT_RST_SHIFT       (5U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_JPEG_SOFT_RST_MASK        (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_JPEG_SOFT_RST_SHIFT)       /* 0x00000020 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_MI_SOFT_RST_SHIFT         (6U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_MI_SOFT_RST_MASK          (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_MI_SOFT_RST_SHIFT)         /* 0x00000040 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_MARVIN_RST_SHIFT          (7U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_MARVIN_RST_MASK           (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_MARVIN_RST_SHIFT)          /* 0x00000080 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_IE_SOFT_RST_SHIFT         (8U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_IE_SOFT_RST_MASK          (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_IE_SOFT_RST_SHIFT)         /* 0x00000100 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_SIMP_SOFT_RST_SHIFT       (9U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_SIMP_SOFT_RST_MASK        (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_SIMP_SOFT_RST_SHIFT)       /* 0x00000200 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_SMIA_SOFT_RST_SHIFT       (10U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_SMIA_SOFT_RST_MASK        (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_SMIA_SOFT_RST_SHIFT)       /* 0x00000400 */
#define ISP_MAIN_CTRL_VI_IRCL_VI_MIPI_SOFT_RST_SHIFT       (11U)
#define ISP_MAIN_CTRL_VI_IRCL_VI_MIPI_SOFT_RST_MASK        (0x1U << ISP_MAIN_CTRL_VI_IRCL_VI_MIPI_SOFT_RST_SHIFT)       /* 0x00000800 */
/* VI_DPCL */
#define ISP_MAIN_CTRL_VI_DPCL_OFFSET                       (0x18U)
#define ISP_MAIN_CTRL_VI_DPCL_VI_MP_MUX_SHIFT              (0U)
#define ISP_MAIN_CTRL_VI_DPCL_VI_MP_MUX_MASK               (0x3U << ISP_MAIN_CTRL_VI_DPCL_VI_MP_MUX_SHIFT)              /* 0x00000003 */
#define ISP_MAIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT           (2U)
#define ISP_MAIN_CTRL_VI_DPCL_VI_CHAN_MODE_MASK            (0x3U << ISP_MAIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT)           /* 0x0000000C */
#define ISP_MAIN_CTRL_VI_DPCL_VI_DMA_SWITCH_SHIFT          (4U)
#define ISP_MAIN_CTRL_VI_DPCL_VI_DMA_SWITCH_MASK           (0x7U << ISP_MAIN_CTRL_VI_DPCL_VI_DMA_SWITCH_SHIFT)          /* 0x00000070 */
#define ISP_MAIN_CTRL_VI_DPCL_IF_SELECT_SHIFT              (8U)
#define ISP_MAIN_CTRL_VI_DPCL_IF_SELECT_MASK               (0x3U << ISP_MAIN_CTRL_VI_DPCL_IF_SELECT_SHIFT)              /* 0x00000300 */
#define ISP_MAIN_CTRL_VI_DPCL_VI_DMA_IEMUX_SHIFT           (10U)
#define ISP_MAIN_CTRL_VI_DPCL_VI_DMA_IEMUX_MASK            (0x1U << ISP_MAIN_CTRL_VI_DPCL_VI_DMA_IEMUX_SHIFT)           /* 0x00000400 */
#define ISP_MAIN_CTRL_VI_DPCL_VI_DMA_SPMUX_SHIFT           (11U)
#define ISP_MAIN_CTRL_VI_DPCL_VI_DMA_SPMUX_MASK            (0x1U << ISP_MAIN_CTRL_VI_DPCL_VI_DMA_SPMUX_SHIFT)           /* 0x00000800 */
/**************************************ISP_IMG_EFF***************************************/
/* IMG_EFF_CTRL */
#define ISP_IMG_EFF_IMG_EFF_CTRL_OFFSET                    (0x0U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_BYPASS_MODE_SHIFT         (0U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_BYPASS_MODE_MASK          (0x1U << ISP_IMG_EFF_IMG_EFF_CTRL_BYPASS_MODE_SHIFT)         /* 0x00000001 */
#define ISP_IMG_EFF_IMG_EFF_CTRL_EFFECT_MODE_SHIFT         (1U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_EFFECT_MODE_MASK          (0x7U << ISP_IMG_EFF_IMG_EFF_CTRL_EFFECT_MODE_SHIFT)         /* 0x0000000E */
#define ISP_IMG_EFF_IMG_EFF_CTRL_CFG_UPD_SHIFT             (4U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_CFG_UPD_MASK              (0x1U << ISP_IMG_EFF_IMG_EFF_CTRL_CFG_UPD_SHIFT)             /* 0x00000010 */
#define ISP_IMG_EFF_IMG_EFF_CTRL_FULL_RANGE_SHIFT          (5U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_FULL_RANGE_MASK           (0x1U << ISP_IMG_EFF_IMG_EFF_CTRL_FULL_RANGE_SHIFT)          /* 0x00000020 */
/* IMG_EFF_COLOR_SEL */
#define ISP_IMG_EFF_IMG_EFF_COLOR_SEL_OFFSET               (0x4U)
#define ISP_IMG_EFF_IMG_EFF_COLOR_SEL_COLOR_SELECTION_SHIFT (0U)
#define ISP_IMG_EFF_IMG_EFF_COLOR_SEL_COLOR_SELECTION_MASK (0x7U << ISP_IMG_EFF_IMG_EFF_COLOR_SEL_COLOR_SELECTION_SHIFT) /* 0x00000007 */
#define ISP_IMG_EFF_IMG_EFF_COLOR_SEL_COLOR_THRESHOLD_SHIFT (8U)
#define ISP_IMG_EFF_IMG_EFF_COLOR_SEL_COLOR_THRESHOLD_MASK (0xFFU << ISP_IMG_EFF_IMG_EFF_COLOR_SEL_COLOR_THRESHOLD_SHIFT) /* 0x0000FF00 */
/* IMG_EFF_MAT_1 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_OFFSET                   (0x8U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_11_SHIFT        (0U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_11_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_11_SHIFT)        /* 0x00000007 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_11_EN_SHIFT     (3U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_11_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_11_EN_SHIFT)     /* 0x00000008 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_12_SHIFT        (4U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_12_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_12_SHIFT)        /* 0x00000070 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_12_EN_SHIFT     (7U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_12_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_12_EN_SHIFT)     /* 0x00000080 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_13_SHIFT        (8U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_13_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_13_SHIFT)        /* 0x00000700 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_13_EN_SHIFT     (11U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_13_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_13_EN_SHIFT)     /* 0x00000800 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_21_SHIFT        (12U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_21_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_21_SHIFT)        /* 0x00007000 */
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_21_EN_SHIFT     (15U)
#define ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_21_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_1_EMB_COEF_21_EN_SHIFT)     /* 0x00008000 */
/* IMG_EFF_MAT_2 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_OFFSET                   (0xCU)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_22_SHIFT        (0U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_22_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_22_SHIFT)        /* 0x00000007 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_22_EN_SHIFT     (3U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_22_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_22_EN_SHIFT)     /* 0x00000008 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_23_SHIFT        (4U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_23_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_23_SHIFT)        /* 0x00000070 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_23_EN_SHIFT     (7U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_23_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_23_EN_SHIFT)     /* 0x00000080 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_31_SHIFT        (8U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_31_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_31_SHIFT)        /* 0x00000700 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_31_EN_SHIFT     (11U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_31_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_31_EN_SHIFT)     /* 0x00000800 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_32_SHIFT        (12U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_32_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_32_SHIFT)        /* 0x00007000 */
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_32_EN_SHIFT     (15U)
#define ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_32_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_2_EMB_COEF_32_EN_SHIFT)     /* 0x00008000 */
/* IMG_EFF_MAT_3 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_OFFSET                   (0x10U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_EMB_COEF_33_SHIFT        (0U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_EMB_COEF_33_MASK         (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_3_EMB_COEF_33_SHIFT)        /* 0x00000007 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_EMB_COEF_33_EN_SHIFT     (3U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_EMB_COEF_33_EN_MASK      (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_3_EMB_COEF_33_EN_SHIFT)     /* 0x00000008 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_11_SHIFT       (4U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_11_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_11_SHIFT)       /* 0x00000070 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_11_EN_SHIFT    (7U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_11_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_11_EN_SHIFT)    /* 0x00000080 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_12_SHIFT       (8U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_12_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_12_SHIFT)       /* 0x00000700 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_12_EN_SHIFT    (11U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_12_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_12_EN_SHIFT)    /* 0x00000800 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_13_SHIFT       (12U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_13_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_13_SHIFT)       /* 0x00007000 */
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_13_EN_SHIFT    (15U)
#define ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_13_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_3_SKET_COEF_13_EN_SHIFT)    /* 0x00008000 */
/* IMG_EFF_MAT_4 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_OFFSET                   (0x14U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_21_SHIFT       (0U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_21_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_21_SHIFT)       /* 0x00000007 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_21_EN_SHIFT    (3U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_21_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_21_EN_SHIFT)    /* 0x00000008 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_22_SHIFT       (4U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_22_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_22_SHIFT)       /* 0x00000070 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_22_EN_SHIFT    (7U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_22_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_22_EN_SHIFT)    /* 0x00000080 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_23_SHIFT       (8U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_23_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_23_SHIFT)       /* 0x00000700 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_23_EN_SHIFT    (11U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_23_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_23_EN_SHIFT)    /* 0x00000800 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_31_SHIFT       (12U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_31_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_31_SHIFT)       /* 0x00007000 */
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_31_EN_SHIFT    (15U)
#define ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_31_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_4_SKET_COEF_31_EN_SHIFT)    /* 0x00008000 */
/* IMG_EFF_MAT_5 */
#define ISP_IMG_EFF_IMG_EFF_MAT_5_OFFSET                   (0x18U)
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_32_SHIFT       (0U)
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_32_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_32_SHIFT)       /* 0x00000007 */
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_32_EN_SHIFT    (3U)
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_32_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_32_EN_SHIFT)    /* 0x00000008 */
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_33_SHIFT       (4U)
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_33_MASK        (0x7U << ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_33_SHIFT)       /* 0x00000070 */
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_33_EN_SHIFT    (7U)
#define ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_33_EN_MASK     (0x1U << ISP_IMG_EFF_IMG_EFF_MAT_5_SKET_COEF_33_EN_SHIFT)    /* 0x00000080 */
/* IMG_EFF_TINT */
#define ISP_IMG_EFF_IMG_EFF_TINT_OFFSET                    (0x1CU)
#define ISP_IMG_EFF_IMG_EFF_TINT_INCR_CB_SHIFT             (0U)
#define ISP_IMG_EFF_IMG_EFF_TINT_INCR_CB_MASK              (0xFFU << ISP_IMG_EFF_IMG_EFF_TINT_INCR_CB_SHIFT)            /* 0x000000FF */
#define ISP_IMG_EFF_IMG_EFF_TINT_INCR_CR_SHIFT             (8U)
#define ISP_IMG_EFF_IMG_EFF_TINT_INCR_CR_MASK              (0xFFU << ISP_IMG_EFF_IMG_EFF_TINT_INCR_CR_SHIFT)            /* 0x0000FF00 */
/* IMG_EFF_CTRL_SHD */
#define ISP_IMG_EFF_IMG_EFF_CTRL_SHD_OFFSET                (0x20U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_SHD                       (0x0U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_SHD_EFFECT_MODE_SHD_SHIFT (1U)
#define ISP_IMG_EFF_IMG_EFF_CTRL_SHD_EFFECT_MODE_SHD_MASK  (0x7U << ISP_IMG_EFF_IMG_EFF_CTRL_SHD_EFFECT_MODE_SHD_SHIFT) /* 0x0000000E */
/* IMG_EFF_SHARPEN */
#define ISP_IMG_EFF_IMG_EFF_SHARPEN_OFFSET                 (0x24U)
#define ISP_IMG_EFF_IMG_EFF_SHARPEN_CORING_THR_SHIFT       (0U)
#define ISP_IMG_EFF_IMG_EFF_SHARPEN_CORING_THR_MASK        (0xFFU << ISP_IMG_EFF_IMG_EFF_SHARPEN_CORING_THR_SHIFT)      /* 0x000000FF */
#define ISP_IMG_EFF_IMG_EFF_SHARPEN_SHARP_FACTOR_SHIFT     (8U)
#define ISP_IMG_EFF_IMG_EFF_SHARPEN_SHARP_FACTOR_MASK      (0x3FU << ISP_IMG_EFF_IMG_EFF_SHARPEN_SHARP_FACTOR_SHIFT)    /* 0x00003F00 */
/*************************************ISP_SUPER_IMP**************************************/
/* SUPER_IMP_CTRL */
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_OFFSET                (0x0U)
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_BYPASS_MODE_SHIFT     (0U)
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_BYPASS_MODE_MASK      (0x1U << ISP_SUPER_IMP_SUPER_IMP_CTRL_BYPASS_MODE_SHIFT)     /* 0x00000001 */
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_REF_IMAGE_SHIFT       (1U)
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_REF_IMAGE_MASK        (0x1U << ISP_SUPER_IMP_SUPER_IMP_CTRL_REF_IMAGE_SHIFT)       /* 0x00000002 */
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_TRANSPARENCY_MODE_SHIFT (2U)
#define ISP_SUPER_IMP_SUPER_IMP_CTRL_TRANSPARENCY_MODE_MASK (0x1U << ISP_SUPER_IMP_SUPER_IMP_CTRL_TRANSPARENCY_MODE_SHIFT) /* 0x00000004 */
/* SUPER_IMP_OFFSET_X */
#define ISP_SUPER_IMP_SUPER_IMP_OFFSET_X_OFFSET            (0x4U)
#define ISP_SUPER_IMP_SUPER_IMP_OFFSET_X_OFFSET_X_SHIFT    (1U)
#define ISP_SUPER_IMP_SUPER_IMP_OFFSET_X_OFFSET_X_MASK     (0x1FFFU << ISP_SUPER_IMP_SUPER_IMP_OFFSET_X_OFFSET_X_SHIFT) /* 0x00003FFE */
/* SUPER_IMP_OFFSET_Y */
#define ISP_SUPER_IMP_SUPER_IMP_OFFSET_Y_OFFSET            (0x8U)
#define ISP_SUPER_IMP_SUPER_IMP_OFFSET_Y_OFFSET_Y_SHIFT    (0U)
#define ISP_SUPER_IMP_SUPER_IMP_OFFSET_Y_OFFSET_Y_MASK     (0x3FFFU << ISP_SUPER_IMP_SUPER_IMP_OFFSET_Y_OFFSET_Y_SHIFT) /* 0x00003FFF */
/* SUPER_IMP_COLOR_Y */
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_Y_OFFSET             (0xCU)
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_Y_Y_COMP_SHIFT       (0U)
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_Y_Y_COMP_MASK        (0xFFU << ISP_SUPER_IMP_SUPER_IMP_COLOR_Y_Y_COMP_SHIFT)      /* 0x000000FF */
/* SUPER_IMP_COLOR_CB */
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_CB_OFFSET            (0x10U)
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_CB_CB_COMP_SHIFT     (0U)
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_CB_CB_COMP_MASK      (0xFFU << ISP_SUPER_IMP_SUPER_IMP_COLOR_CB_CB_COMP_SHIFT)    /* 0x000000FF */
/* SUPER_IMP_COLOR_CR */
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_CR_OFFSET            (0x14U)
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_CR_CR_COMP_SHIFT     (0U)
#define ISP_SUPER_IMP_SUPER_IMP_COLOR_CR_CR_COMP_MASK      (0xFFU << ISP_SUPER_IMP_SUPER_IMP_COLOR_CR_CR_COMP_SHIFT)    /* 0x000000FF */
/******************************************ISP*******************************************/
/* ISP_CTRL */
#define ISP_ISP_CTRL_OFFSET                                (0x0U)
#define ISP_ISP_CTRL_ISP_ENABLE_SHIFT                      (0U)
#define ISP_ISP_CTRL_ISP_ENABLE_MASK                       (0x1U << ISP_ISP_CTRL_ISP_ENABLE_SHIFT)                      /* 0x00000001 */
#define ISP_ISP_CTRL_ISP_MODE_SHIFT                        (1U)
#define ISP_ISP_CTRL_ISP_MODE_MASK                         (0x7U << ISP_ISP_CTRL_ISP_MODE_SHIFT)                        /* 0x0000000E */
#define ISP_ISP_CTRL_ISP_INFORM_ENABLE_SHIFT               (4U)
#define ISP_ISP_CTRL_ISP_INFORM_ENABLE_MASK                (0x1U << ISP_ISP_CTRL_ISP_INFORM_ENABLE_SHIFT)               /* 0x00000010 */
#define ISP_ISP_CTRL_ISP_GAMMA_IN_ENABLE_SHIFT             (6U)
#define ISP_ISP_CTRL_ISP_GAMMA_IN_ENABLE_MASK              (0x1U << ISP_ISP_CTRL_ISP_GAMMA_IN_ENABLE_SHIFT)             /* 0x00000040 */
#define ISP_ISP_CTRL_ISP_AWB_ENABLE_SHIFT                  (7U)
#define ISP_ISP_CTRL_ISP_AWB_ENABLE_MASK                   (0x1U << ISP_ISP_CTRL_ISP_AWB_ENABLE_SHIFT)                  /* 0x00000080 */
#define ISP_ISP_CTRL_ISP_CFG_UPD_PERMANENT_SHIFT           (8U)
#define ISP_ISP_CTRL_ISP_CFG_UPD_PERMANENT_MASK            (0x1U << ISP_ISP_CTRL_ISP_CFG_UPD_PERMANENT_SHIFT)           /* 0x00000100 */
#define ISP_ISP_CTRL_ISP_CFG_UPD_SHIFT                     (9U)
#define ISP_ISP_CTRL_ISP_CFG_UPD_MASK                      (0x1U << ISP_ISP_CTRL_ISP_CFG_UPD_SHIFT)                     /* 0x00000200 */
#define ISP_ISP_CTRL_ISP_GEN_CFG_UPD_SHIFT                 (10U)
#define ISP_ISP_CTRL_ISP_GEN_CFG_UPD_MASK                  (0x1U << ISP_ISP_CTRL_ISP_GEN_CFG_UPD_SHIFT)                 /* 0x00000400 */
#define ISP_ISP_CTRL_ISP_GAMMA_OUT_ENABLE_SHIFT            (11U)
#define ISP_ISP_CTRL_ISP_GAMMA_OUT_ENABLE_MASK             (0x1U << ISP_ISP_CTRL_ISP_GAMMA_OUT_ENABLE_SHIFT)            /* 0x00000800 */
#define ISP_ISP_CTRL_ISP_FLASH_MODE_SHIFT                  (12U)
#define ISP_ISP_CTRL_ISP_FLASH_MODE_MASK                   (0x1U << ISP_ISP_CTRL_ISP_FLASH_MODE_SHIFT)                  /* 0x00001000 */
#define ISP_ISP_CTRL_ISP_CSM_Y_RANGE_SHIFT                 (13U)
#define ISP_ISP_CTRL_ISP_CSM_Y_RANGE_MASK                  (0x1U << ISP_ISP_CTRL_ISP_CSM_Y_RANGE_SHIFT)                 /* 0x00002000 */
#define ISP_ISP_CTRL_ISP_CSM_C_RANGE_SHIFT                 (14U)
#define ISP_ISP_CTRL_ISP_CSM_C_RANGE_MASK                  (0x1U << ISP_ISP_CTRL_ISP_CSM_C_RANGE_SHIFT)                 /* 0x00004000 */
#define ISP_ISP_CTRL_CTRL_RESERVED_2_SHIFT                 (15U)
#define ISP_ISP_CTRL_CTRL_RESERVED_2_MASK                  (0x1U << ISP_ISP_CTRL_CTRL_RESERVED_2_SHIFT)                 /* 0x00008000 */
#define ISP_ISP_CTRL_CTRL_RESERVED_1_SHIFT                 (16U)
#define ISP_ISP_CTRL_CTRL_RESERVED_1_MASK                  (0x1U << ISP_ISP_CTRL_CTRL_RESERVED_1_SHIFT)                 /* 0x00010000 */
/* ISP_ACQ_PROP */
#define ISP_ISP_ACQ_PROP_OFFSET                            (0x4U)
#define ISP_ISP_ACQ_PROP_SAMPLE_EDGE_SHIFT                 (0U)
#define ISP_ISP_ACQ_PROP_SAMPLE_EDGE_MASK                  (0x1U << ISP_ISP_ACQ_PROP_SAMPLE_EDGE_SHIFT)                 /* 0x00000001 */
#define ISP_ISP_ACQ_PROP_HSYNC_POL_SHIFT                   (1U)
#define ISP_ISP_ACQ_PROP_HSYNC_POL_MASK                    (0x1U << ISP_ISP_ACQ_PROP_HSYNC_POL_SHIFT)                   /* 0x00000002 */
#define ISP_ISP_ACQ_PROP_VSYNC_POL_SHIFT                   (2U)
#define ISP_ISP_ACQ_PROP_VSYNC_POL_MASK                    (0x1U << ISP_ISP_ACQ_PROP_VSYNC_POL_SHIFT)                   /* 0x00000004 */
#define ISP_ISP_ACQ_PROP_BAYER_PAT_SHIFT                   (3U)
#define ISP_ISP_ACQ_PROP_BAYER_PAT_MASK                    (0x3U << ISP_ISP_ACQ_PROP_BAYER_PAT_SHIFT)                   /* 0x00000018 */
#define ISP_ISP_ACQ_PROP_CONV_422_SHIFT                    (5U)
#define ISP_ISP_ACQ_PROP_CONV_422_MASK                     (0x3U << ISP_ISP_ACQ_PROP_CONV_422_SHIFT)                    /* 0x00000060 */
#define ISP_ISP_ACQ_PROP_CCIR_SEQ_SHIFT                    (7U)
#define ISP_ISP_ACQ_PROP_CCIR_SEQ_MASK                     (0x3U << ISP_ISP_ACQ_PROP_CCIR_SEQ_SHIFT)                    /* 0x00000180 */
#define ISP_ISP_ACQ_PROP_FIELD_SELECTION_SHIFT             (9U)
#define ISP_ISP_ACQ_PROP_FIELD_SELECTION_MASK              (0x3U << ISP_ISP_ACQ_PROP_FIELD_SELECTION_SHIFT)             /* 0x00000600 */
#define ISP_ISP_ACQ_PROP_FIELD_INV_SHIFT                   (11U)
#define ISP_ISP_ACQ_PROP_FIELD_INV_MASK                    (0x1U << ISP_ISP_ACQ_PROP_FIELD_INV_SHIFT)                   /* 0x00000800 */
#define ISP_ISP_ACQ_PROP_INPUT_SELECTION_SHIFT             (12U)
#define ISP_ISP_ACQ_PROP_INPUT_SELECTION_MASK              (0x7U << ISP_ISP_ACQ_PROP_INPUT_SELECTION_SHIFT)             /* 0x00007000 */
#define ISP_ISP_ACQ_PROP_DMA_RGB_SELECTION_SHIFT           (15U)
#define ISP_ISP_ACQ_PROP_DMA_RGB_SELECTION_MASK            (0x1U << ISP_ISP_ACQ_PROP_DMA_RGB_SELECTION_SHIFT)           /* 0x00008000 */
#define ISP_ISP_ACQ_PROP_DMA_YUV_SELECTION_SHIFT           (16U)
#define ISP_ISP_ACQ_PROP_DMA_YUV_SELECTION_MASK            (0x1U << ISP_ISP_ACQ_PROP_DMA_YUV_SELECTION_SHIFT)           /* 0x00010000 */
/* ISP_ACQ_H_OFFS */
#define ISP_ISP_ACQ_H_OFFS_OFFSET                          (0x8U)
#define ISP_ISP_ACQ_H_OFFS_ACQ_H_OFFS_SHIFT                (0U)
#define ISP_ISP_ACQ_H_OFFS_ACQ_H_OFFS_MASK                 (0x7FFFU << ISP_ISP_ACQ_H_OFFS_ACQ_H_OFFS_SHIFT)             /* 0x00007FFF */
/* ISP_ACQ_V_OFFS */
#define ISP_ISP_ACQ_V_OFFS_OFFSET                          (0xCU)
#define ISP_ISP_ACQ_V_OFFS_ACQ_V_OFFS_SHIFT                (0U)
#define ISP_ISP_ACQ_V_OFFS_ACQ_V_OFFS_MASK                 (0x3FFFU << ISP_ISP_ACQ_V_OFFS_ACQ_V_OFFS_SHIFT)             /* 0x00003FFF */
/* ISP_ACQ_H_SIZE */
#define ISP_ISP_ACQ_H_SIZE_OFFSET                          (0x10U)
#define ISP_ISP_ACQ_H_SIZE_ACQ_H_SIZE_SHIFT                (0U)
#define ISP_ISP_ACQ_H_SIZE_ACQ_H_SIZE_MASK                 (0x7FFFU << ISP_ISP_ACQ_H_SIZE_ACQ_H_SIZE_SHIFT)             /* 0x00007FFF */
/* ISP_ACQ_V_SIZE */
#define ISP_ISP_ACQ_V_SIZE_OFFSET                          (0x14U)
#define ISP_ISP_ACQ_V_SIZE_ACQ_V_SIZE_SHIFT                (0U)
#define ISP_ISP_ACQ_V_SIZE_ACQ_V_SIZE_MASK                 (0x3FFFU << ISP_ISP_ACQ_V_SIZE_ACQ_V_SIZE_SHIFT)             /* 0x00003FFF */
/* ISP_ACQ_NR_FRAMES */
#define ISP_ISP_ACQ_NR_FRAMES_OFFSET                       (0x18U)
#define ISP_ISP_ACQ_NR_FRAMES_ACQ_NR_FRAMES_SHIFT          (0U)
#define ISP_ISP_ACQ_NR_FRAMES_ACQ_NR_FRAMES_MASK           (0x3FFU << ISP_ISP_ACQ_NR_FRAMES_ACQ_NR_FRAMES_SHIFT)        /* 0x000003FF */
/* ISP_GAMMA_DX_LO */
#define ISP_ISP_GAMMA_DX_LO_OFFSET                         (0x1CU)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_1_SHIFT               (0U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_1_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_1_SHIFT)               /* 0x00000007 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_2_SHIFT               (4U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_2_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_2_SHIFT)               /* 0x00000070 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_3_SHIFT               (8U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_3_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_3_SHIFT)               /* 0x00000700 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_4_SHIFT               (12U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_4_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_4_SHIFT)               /* 0x00007000 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_5_SHIFT               (16U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_5_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_5_SHIFT)               /* 0x00070000 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_6_SHIFT               (20U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_6_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_6_SHIFT)               /* 0x00700000 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_7_SHIFT               (24U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_7_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_7_SHIFT)               /* 0x07000000 */
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_8_SHIFT               (28U)
#define ISP_ISP_GAMMA_DX_LO_GAMMA_DX_8_MASK                (0x7U << ISP_ISP_GAMMA_DX_LO_GAMMA_DX_8_SHIFT)               /* 0x70000000 */
/* ISP_GAMMA_DX_HI */
#define ISP_ISP_GAMMA_DX_HI_OFFSET                         (0x20U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_9_SHIFT               (0U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_9_MASK                (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_9_SHIFT)               /* 0x00000007 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_10_SHIFT              (4U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_10_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_10_SHIFT)              /* 0x00000070 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_11_SHIFT              (8U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_11_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_11_SHIFT)              /* 0x00000700 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_12_SHIFT              (12U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_12_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_12_SHIFT)              /* 0x00007000 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_13_SHIFT              (16U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_13_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_13_SHIFT)              /* 0x00070000 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_14_SHIFT              (20U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_14_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_14_SHIFT)              /* 0x00700000 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_15_SHIFT              (24U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_15_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_15_SHIFT)              /* 0x07000000 */
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_16_SHIFT              (28U)
#define ISP_ISP_GAMMA_DX_HI_GAMMA_DX_16_MASK               (0x7U << ISP_ISP_GAMMA_DX_HI_GAMMA_DX_16_SHIFT)              /* 0x70000000 */
/* ISP_GAMMA_R_Y_0 */
#define ISP_ISP_GAMMA_R_Y_0_OFFSET                         (0x24U)
#define ISP_ISP_GAMMA_R_Y_0_GAMMA_R_Y_0_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_0_GAMMA_R_Y_0_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_0_GAMMA_R_Y_0_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_1 */
#define ISP_ISP_GAMMA_R_Y_1_OFFSET                         (0x28U)
#define ISP_ISP_GAMMA_R_Y_1_GAMMA_R_Y_1_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_1_GAMMA_R_Y_1_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_1_GAMMA_R_Y_1_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_2 */
#define ISP_ISP_GAMMA_R_Y_2_OFFSET                         (0x2CU)
#define ISP_ISP_GAMMA_R_Y_2_GAMMA_R_Y_2_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_2_GAMMA_R_Y_2_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_2_GAMMA_R_Y_2_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_3 */
#define ISP_ISP_GAMMA_R_Y_3_OFFSET                         (0x30U)
#define ISP_ISP_GAMMA_R_Y_3_GAMMA_R_Y_3_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_3_GAMMA_R_Y_3_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_3_GAMMA_R_Y_3_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_4 */
#define ISP_ISP_GAMMA_R_Y_4_OFFSET                         (0x34U)
#define ISP_ISP_GAMMA_R_Y_4_GAMMA_R_Y_4_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_4_GAMMA_R_Y_4_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_4_GAMMA_R_Y_4_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_5 */
#define ISP_ISP_GAMMA_R_Y_5_OFFSET                         (0x38U)
#define ISP_ISP_GAMMA_R_Y_5_GAMMA_R_Y_5_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_5_GAMMA_R_Y_5_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_5_GAMMA_R_Y_5_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_6 */
#define ISP_ISP_GAMMA_R_Y_6_OFFSET                         (0x3CU)
#define ISP_ISP_GAMMA_R_Y_6_GAMMA_R_Y_6_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_6_GAMMA_R_Y_6_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_6_GAMMA_R_Y_6_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_7 */
#define ISP_ISP_GAMMA_R_Y_7_OFFSET                         (0x40U)
#define ISP_ISP_GAMMA_R_Y_7_GAMMA_R_Y_7_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_7_GAMMA_R_Y_7_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_7_GAMMA_R_Y_7_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_8 */
#define ISP_ISP_GAMMA_R_Y_8_OFFSET                         (0x44U)
#define ISP_ISP_GAMMA_R_Y_8_GAMMA_R_Y_8_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_8_GAMMA_R_Y_8_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_8_GAMMA_R_Y_8_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_9 */
#define ISP_ISP_GAMMA_R_Y_9_OFFSET                         (0x48U)
#define ISP_ISP_GAMMA_R_Y_9_GAMMA_R_Y_9_SHIFT              (0U)
#define ISP_ISP_GAMMA_R_Y_9_GAMMA_R_Y_9_MASK               (0xFFFU << ISP_ISP_GAMMA_R_Y_9_GAMMA_R_Y_9_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_R_Y_10 */
#define ISP_ISP_GAMMA_R_Y_10_OFFSET                        (0x4CU)
#define ISP_ISP_GAMMA_R_Y_10_GAMMA_R_Y_10_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_10_GAMMA_R_Y_10_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_10_GAMMA_R_Y_10_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_R_Y_11 */
#define ISP_ISP_GAMMA_R_Y_11_OFFSET                        (0x50U)
#define ISP_ISP_GAMMA_R_Y_11_GAMMA_R_Y_11_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_11_GAMMA_R_Y_11_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_11_GAMMA_R_Y_11_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_R_Y_12 */
#define ISP_ISP_GAMMA_R_Y_12_OFFSET                        (0x54U)
#define ISP_ISP_GAMMA_R_Y_12_GAMMA_R_Y_12_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_12_GAMMA_R_Y_12_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_12_GAMMA_R_Y_12_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_R_Y_13 */
#define ISP_ISP_GAMMA_R_Y_13_OFFSET                        (0x58U)
#define ISP_ISP_GAMMA_R_Y_13_GAMMA_R_Y_13_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_13_GAMMA_R_Y_13_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_13_GAMMA_R_Y_13_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_R_Y_14 */
#define ISP_ISP_GAMMA_R_Y_14_OFFSET                        (0x5CU)
#define ISP_ISP_GAMMA_R_Y_14_GAMMA_R_Y_14_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_14_GAMMA_R_Y_14_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_14_GAMMA_R_Y_14_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_R_Y_15 */
#define ISP_ISP_GAMMA_R_Y_15_OFFSET                        (0x60U)
#define ISP_ISP_GAMMA_R_Y_15_GAMMA_R_Y_15_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_15_GAMMA_R_Y_15_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_15_GAMMA_R_Y_15_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_R_Y_16 */
#define ISP_ISP_GAMMA_R_Y_16_OFFSET                        (0x64U)
#define ISP_ISP_GAMMA_R_Y_16_GAMMA_R_Y_16_SHIFT            (0U)
#define ISP_ISP_GAMMA_R_Y_16_GAMMA_R_Y_16_MASK             (0xFFFU << ISP_ISP_GAMMA_R_Y_16_GAMMA_R_Y_16_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_0 */
#define ISP_ISP_GAMMA_G_Y_0_OFFSET                         (0x68U)
#define ISP_ISP_GAMMA_G_Y_0_GAMMA_G_Y_0_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_0_GAMMA_G_Y_0_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_0_GAMMA_G_Y_0_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_1 */
#define ISP_ISP_GAMMA_G_Y_1_OFFSET                         (0x6CU)
#define ISP_ISP_GAMMA_G_Y_1_GAMMA_G_Y_1_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_1_GAMMA_G_Y_1_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_1_GAMMA_G_Y_1_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_2 */
#define ISP_ISP_GAMMA_G_Y_2_OFFSET                         (0x70U)
#define ISP_ISP_GAMMA_G_Y_2_GAMMA_G_Y_2_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_2_GAMMA_G_Y_2_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_2_GAMMA_G_Y_2_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_3 */
#define ISP_ISP_GAMMA_G_Y_3_OFFSET                         (0x74U)
#define ISP_ISP_GAMMA_G_Y_3_GAMMA_G_Y_3_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_3_GAMMA_G_Y_3_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_3_GAMMA_G_Y_3_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_4 */
#define ISP_ISP_GAMMA_G_Y_4_OFFSET                         (0x78U)
#define ISP_ISP_GAMMA_G_Y_4_GAMMA_G_Y_4_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_4_GAMMA_G_Y_4_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_4_GAMMA_G_Y_4_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_5 */
#define ISP_ISP_GAMMA_G_Y_5_OFFSET                         (0x7CU)
#define ISP_ISP_GAMMA_G_Y_5_GAMMA_G_Y_5_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_5_GAMMA_G_Y_5_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_5_GAMMA_G_Y_5_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_6 */
#define ISP_ISP_GAMMA_G_Y_6_OFFSET                         (0x80U)
#define ISP_ISP_GAMMA_G_Y_6_GAMMA_G_Y_6_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_6_GAMMA_G_Y_6_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_6_GAMMA_G_Y_6_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_7 */
#define ISP_ISP_GAMMA_G_Y_7_OFFSET                         (0x84U)
#define ISP_ISP_GAMMA_G_Y_7_GAMMA_G_Y_7_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_7_GAMMA_G_Y_7_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_7_GAMMA_G_Y_7_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_8 */
#define ISP_ISP_GAMMA_G_Y_8_OFFSET                         (0x88U)
#define ISP_ISP_GAMMA_G_Y_8_GAMMA_G_Y_8_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_8_GAMMA_G_Y_8_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_8_GAMMA_G_Y_8_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_9 */
#define ISP_ISP_GAMMA_G_Y_9_OFFSET                         (0x8CU)
#define ISP_ISP_GAMMA_G_Y_9_GAMMA_G_Y_9_SHIFT              (0U)
#define ISP_ISP_GAMMA_G_Y_9_GAMMA_G_Y_9_MASK               (0xFFFU << ISP_ISP_GAMMA_G_Y_9_GAMMA_G_Y_9_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_G_Y_10 */
#define ISP_ISP_GAMMA_G_Y_10_OFFSET                        (0x90U)
#define ISP_ISP_GAMMA_G_Y_10_GAMMA_G_Y_10_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_10_GAMMA_G_Y_10_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_10_GAMMA_G_Y_10_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_11 */
#define ISP_ISP_GAMMA_G_Y_11_OFFSET                        (0x94U)
#define ISP_ISP_GAMMA_G_Y_11_GAMMA_G_Y_11_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_11_GAMMA_G_Y_11_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_11_GAMMA_G_Y_11_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_12 */
#define ISP_ISP_GAMMA_G_Y_12_OFFSET                        (0x98U)
#define ISP_ISP_GAMMA_G_Y_12_GAMMA_G_Y_12_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_12_GAMMA_G_Y_12_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_12_GAMMA_G_Y_12_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_13 */
#define ISP_ISP_GAMMA_G_Y_13_OFFSET                        (0x9CU)
#define ISP_ISP_GAMMA_G_Y_13_GAMMA_G_Y_13_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_13_GAMMA_G_Y_13_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_13_GAMMA_G_Y_13_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_14 */
#define ISP_ISP_GAMMA_G_Y_14_OFFSET                        (0xA0U)
#define ISP_ISP_GAMMA_G_Y_14_GAMMA_G_Y_14_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_14_GAMMA_G_Y_14_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_14_GAMMA_G_Y_14_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_15 */
#define ISP_ISP_GAMMA_G_Y_15_OFFSET                        (0xA4U)
#define ISP_ISP_GAMMA_G_Y_15_GAMMA_G_Y_15_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_15_GAMMA_G_Y_15_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_15_GAMMA_G_Y_15_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_G_Y_16 */
#define ISP_ISP_GAMMA_G_Y_16_OFFSET                        (0xA8U)
#define ISP_ISP_GAMMA_G_Y_16_GAMMA_G_Y_16_SHIFT            (0U)
#define ISP_ISP_GAMMA_G_Y_16_GAMMA_G_Y_16_MASK             (0xFFFU << ISP_ISP_GAMMA_G_Y_16_GAMMA_G_Y_16_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_0 */
#define ISP_ISP_GAMMA_B_Y_0_OFFSET                         (0xACU)
#define ISP_ISP_GAMMA_B_Y_0_GAMMA_B_Y_0_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_0_GAMMA_B_Y_0_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_0_GAMMA_B_Y_0_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_1 */
#define ISP_ISP_GAMMA_B_Y_1_OFFSET                         (0xB0U)
#define ISP_ISP_GAMMA_B_Y_1_GAMMA_B_Y_1_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_1_GAMMA_B_Y_1_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_1_GAMMA_B_Y_1_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_2 */
#define ISP_ISP_GAMMA_B_Y_2_OFFSET                         (0xB4U)
#define ISP_ISP_GAMMA_B_Y_2_GAMMA_B_Y_2_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_2_GAMMA_B_Y_2_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_2_GAMMA_B_Y_2_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_3 */
#define ISP_ISP_GAMMA_B_Y_3_OFFSET                         (0xB8U)
#define ISP_ISP_GAMMA_B_Y_3_GAMMA_B_Y_3_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_3_GAMMA_B_Y_3_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_3_GAMMA_B_Y_3_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_4 */
#define ISP_ISP_GAMMA_B_Y_4_OFFSET                         (0xBCU)
#define ISP_ISP_GAMMA_B_Y_4_GAMMA_B_Y_4_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_4_GAMMA_B_Y_4_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_4_GAMMA_B_Y_4_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_5 */
#define ISP_ISP_GAMMA_B_Y_5_OFFSET                         (0xC0U)
#define ISP_ISP_GAMMA_B_Y_5_GAMMA_B_Y_5_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_5_GAMMA_B_Y_5_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_5_GAMMA_B_Y_5_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_6 */
#define ISP_ISP_GAMMA_B_Y_6_OFFSET                         (0xC4U)
#define ISP_ISP_GAMMA_B_Y_6_GAMMA_B_Y_6_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_6_GAMMA_B_Y_6_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_6_GAMMA_B_Y_6_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_7 */
#define ISP_ISP_GAMMA_B_Y_7_OFFSET                         (0xC8U)
#define ISP_ISP_GAMMA_B_Y_7_GAMMA_B_Y_7_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_7_GAMMA_B_Y_7_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_7_GAMMA_B_Y_7_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_8 */
#define ISP_ISP_GAMMA_B_Y_8_OFFSET                         (0xCCU)
#define ISP_ISP_GAMMA_B_Y_8_GAMMA_B_Y_8_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_8_GAMMA_B_Y_8_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_8_GAMMA_B_Y_8_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_9 */
#define ISP_ISP_GAMMA_B_Y_9_OFFSET                         (0xD0U)
#define ISP_ISP_GAMMA_B_Y_9_GAMMA_B_Y_9_SHIFT              (0U)
#define ISP_ISP_GAMMA_B_Y_9_GAMMA_B_Y_9_MASK               (0xFFFU << ISP_ISP_GAMMA_B_Y_9_GAMMA_B_Y_9_SHIFT)            /* 0x00000FFF */
/* ISP_GAMMA_B_Y_10 */
#define ISP_ISP_GAMMA_B_Y_10_OFFSET                        (0xD4U)
#define ISP_ISP_GAMMA_B_Y_10_GAMMA_B_Y_10_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_10_GAMMA_B_Y_10_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_10_GAMMA_B_Y_10_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_11 */
#define ISP_ISP_GAMMA_B_Y_11_OFFSET                        (0xD8U)
#define ISP_ISP_GAMMA_B_Y_11_GAMMA_B_Y_11_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_11_GAMMA_B_Y_11_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_11_GAMMA_B_Y_11_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_12 */
#define ISP_ISP_GAMMA_B_Y_12_OFFSET                        (0xDCU)
#define ISP_ISP_GAMMA_B_Y_12_GAMMA_B_Y_12_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_12_GAMMA_B_Y_12_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_12_GAMMA_B_Y_12_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_13 */
#define ISP_ISP_GAMMA_B_Y_13_OFFSET                        (0xE0U)
#define ISP_ISP_GAMMA_B_Y_13_GAMMA_B_Y_13_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_13_GAMMA_B_Y_13_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_13_GAMMA_B_Y_13_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_14 */
#define ISP_ISP_GAMMA_B_Y_14_OFFSET                        (0xE4U)
#define ISP_ISP_GAMMA_B_Y_14_GAMMA_B_Y_14_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_14_GAMMA_B_Y_14_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_14_GAMMA_B_Y_14_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_15 */
#define ISP_ISP_GAMMA_B_Y_15_OFFSET                        (0xE8U)
#define ISP_ISP_GAMMA_B_Y_15_GAMMA_B_Y_15_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_15_GAMMA_B_Y_15_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_15_GAMMA_B_Y_15_SHIFT)          /* 0x00000FFF */
/* ISP_GAMMA_B_Y_16 */
#define ISP_ISP_GAMMA_B_Y_16_OFFSET                        (0xECU)
#define ISP_ISP_GAMMA_B_Y_16_GAMMA_B_Y_16_SHIFT            (0U)
#define ISP_ISP_GAMMA_B_Y_16_GAMMA_B_Y_16_MASK             (0xFFFU << ISP_ISP_GAMMA_B_Y_16_GAMMA_B_Y_16_SHIFT)          /* 0x00000FFF */
/* ISP_AWB_PROP */
#define ISP_ISP_AWB_PROP_OFFSET                            (0x110U)
#define ISP_ISP_AWB_PROP_AWB_MODE_SHIFT                    (0U)
#define ISP_ISP_AWB_PROP_AWB_MODE_MASK                     (0x3U << ISP_ISP_AWB_PROP_AWB_MODE_SHIFT)                    /* 0x00000003 */
#define ISP_ISP_AWB_PROP_AWB_MAX_EN_SHIFT                  (2U)
#define ISP_ISP_AWB_PROP_AWB_MAX_EN_MASK                   (0x1U << ISP_ISP_AWB_PROP_AWB_MAX_EN_SHIFT)                  /* 0x00000004 */
#define ISP_ISP_AWB_PROP_AWB_MEAS_MODE_SHIFT               (31U)
#define ISP_ISP_AWB_PROP_AWB_MEAS_MODE_MASK                (0x1U << ISP_ISP_AWB_PROP_AWB_MEAS_MODE_SHIFT)               /* 0x80000000 */
/* ISP_AWB_H_OFFS */
#define ISP_ISP_AWB_H_OFFS_OFFSET                          (0x114U)
#define ISP_ISP_AWB_H_OFFS_AWB_H_OFFS_SHIFT                (0U)
#define ISP_ISP_AWB_H_OFFS_AWB_H_OFFS_MASK                 (0x1FFFU << ISP_ISP_AWB_H_OFFS_AWB_H_OFFS_SHIFT)             /* 0x00001FFF */
/* ISP_AWB_V_OFFS */
#define ISP_ISP_AWB_V_OFFS_OFFSET                          (0x118U)
#define ISP_ISP_AWB_V_OFFS_AWB_V_OFFS_SHIFT                (0U)
#define ISP_ISP_AWB_V_OFFS_AWB_V_OFFS_MASK                 (0x1FFFU << ISP_ISP_AWB_V_OFFS_AWB_V_OFFS_SHIFT)             /* 0x00001FFF */
/* ISP_AWB_H_SIZE */
#define ISP_ISP_AWB_H_SIZE_OFFSET                          (0x11CU)
#define ISP_ISP_AWB_H_SIZE_AWB_H_SIZE_SHIFT                (0U)
#define ISP_ISP_AWB_H_SIZE_AWB_H_SIZE_MASK                 (0x3FFFU << ISP_ISP_AWB_H_SIZE_AWB_H_SIZE_SHIFT)             /* 0x00003FFF */
/* ISP_AWB_V_SIZE */
#define ISP_ISP_AWB_V_SIZE_OFFSET                          (0x120U)
#define ISP_ISP_AWB_V_SIZE_AWB_V_SIZE_SHIFT                (0U)
#define ISP_ISP_AWB_V_SIZE_AWB_V_SIZE_MASK                 (0x3FFFU << ISP_ISP_AWB_V_SIZE_AWB_V_SIZE_SHIFT)             /* 0x00003FFF */
/* ISP_AWB_FRAMES */
#define ISP_ISP_AWB_FRAMES_OFFSET                          (0x124U)
#define ISP_ISP_AWB_FRAMES_AWB_FRAMES_SHIFT                (0U)
#define ISP_ISP_AWB_FRAMES_AWB_FRAMES_MASK                 (0x7U << ISP_ISP_AWB_FRAMES_AWB_FRAMES_SHIFT)                /* 0x00000007 */
#define ISP_ISP_AWB_FRAMES_AWB_REF_CR_SHIFT                (0U)
#define ISP_ISP_AWB_FRAMES_AWB_REF_CR_MASK                 (0x7U << ISP_ISP_AWB_FRAMES_AWB_REF_CR_SHIFT)                /* 0x00000007 */
/* ISP_AWB_REF */
#define ISP_ISP_AWB_REF_OFFSET                             (0x128U)
#define ISP_ISP_AWB_REF_AWB_REF_CB__MAX_B_SHIFT            (0U)
#define ISP_ISP_AWB_REF_AWB_REF_CB__MAX_B_MASK             (0xFFU << ISP_ISP_AWB_REF_AWB_REF_CB__MAX_B_SHIFT)           /* 0x000000FF */
/* ISP_AWB_THRESH */
#define ISP_ISP_AWB_THRESH_OFFSET                          (0x12CU)
#define ISP_ISP_AWB_THRESH_AWB_MIN_C_SHIFT                 (0U)
#define ISP_ISP_AWB_THRESH_AWB_MIN_C_MASK                  (0xFFU << ISP_ISP_AWB_THRESH_AWB_MIN_C_SHIFT)                /* 0x000000FF */
#define ISP_ISP_AWB_THRESH_AWB_MAX_CSUM_SHIFT              (8U)
#define ISP_ISP_AWB_THRESH_AWB_MAX_CSUM_MASK               (0xFFU << ISP_ISP_AWB_THRESH_AWB_MAX_CSUM_SHIFT)             /* 0x0000FF00 */
#define ISP_ISP_AWB_THRESH_AWB_MIN_Y__MAX_G_SHIFT          (16U)
#define ISP_ISP_AWB_THRESH_AWB_MIN_Y__MAX_G_MASK           (0xFFU << ISP_ISP_AWB_THRESH_AWB_MIN_Y__MAX_G_SHIFT)         /* 0x00FF0000 */
#define ISP_ISP_AWB_THRESH_AWB_MAX_Y_SHIFT                 (24U)
#define ISP_ISP_AWB_THRESH_AWB_MAX_Y_MASK                  (0xFFU << ISP_ISP_AWB_THRESH_AWB_MAX_Y_SHIFT)                /* 0xFF000000 */
/* ISP_AWB_GAIN_G */
#define ISP_ISP_AWB_GAIN_G_OFFSET                          (0x138U)
#define ISP_ISP_AWB_GAIN_G_AWB_GAIN_GB_SHIFT               (0U)
#define ISP_ISP_AWB_GAIN_G_AWB_GAIN_GB_MASK                (0x3FFU << ISP_ISP_AWB_GAIN_G_AWB_GAIN_GB_SHIFT)             /* 0x000003FF */
#define ISP_ISP_AWB_GAIN_G_AWB_GAIN_GR_SHIFT               (16U)
#define ISP_ISP_AWB_GAIN_G_AWB_GAIN_GR_MASK                (0x3FFU << ISP_ISP_AWB_GAIN_G_AWB_GAIN_GR_SHIFT)             /* 0x03FF0000 */
/* ISP_AWB_GAIN_RB */
#define ISP_ISP_AWB_GAIN_RB_OFFSET                         (0x13CU)
#define ISP_ISP_AWB_GAIN_RB_AWB_GAIN_B_SHIFT               (0U)
#define ISP_ISP_AWB_GAIN_RB_AWB_GAIN_B_MASK                (0x3FFU << ISP_ISP_AWB_GAIN_RB_AWB_GAIN_B_SHIFT)             /* 0x000003FF */
#define ISP_ISP_AWB_GAIN_RB_AWB_GAIN_R_SHIFT               (16U)
#define ISP_ISP_AWB_GAIN_RB_AWB_GAIN_R_MASK                (0x3FFU << ISP_ISP_AWB_GAIN_RB_AWB_GAIN_R_SHIFT)             /* 0x03FF0000 */
/* ISP_AWB_WHITE_CNT */
#define ISP_ISP_AWB_WHITE_CNT_OFFSET                       (0x140U)
#define ISP_ISP_AWB_WHITE_CNT                              (0x0U)
#define ISP_ISP_AWB_WHITE_CNT_AWB_WHITE_CNT_SHIFT          (0U)
#define ISP_ISP_AWB_WHITE_CNT_AWB_WHITE_CNT_MASK           (0x3FFFFFFU << ISP_ISP_AWB_WHITE_CNT_AWB_WHITE_CNT_SHIFT)    /* 0x03FFFFFF */
/* ISP_AWB_MEAN */
#define ISP_ISP_AWB_MEAN_OFFSET                            (0x144U)
#define ISP_ISP_AWB_MEAN                                   (0x0U)
#define ISP_ISP_AWB_MEAN_AWB_MEAN_CB__B_SHIFT              (8U)
#define ISP_ISP_AWB_MEAN_AWB_MEAN_CB__B_MASK               (0xFFU << ISP_ISP_AWB_MEAN_AWB_MEAN_CB__B_SHIFT)             /* 0x0000FF00 */
#define ISP_ISP_AWB_MEAN_AWB_MEAN_CR_SHIFT                 (8U)
#define ISP_ISP_AWB_MEAN_AWB_MEAN_CR_MASK                  (0xFFU << ISP_ISP_AWB_MEAN_AWB_MEAN_CR_SHIFT)                /* 0x0000FF00 */
#define ISP_ISP_AWB_MEAN_AWB_MEAN_Y__G_SHIFT               (16U)
#define ISP_ISP_AWB_MEAN_AWB_MEAN_Y__G_MASK                (0xFFU << ISP_ISP_AWB_MEAN_AWB_MEAN_Y__G_SHIFT)              /* 0x00FF0000 */
/* ISP_CC_COEFF_0 */
#define ISP_ISP_CC_COEFF_0_OFFSET                          (0x170U)
#define ISP_ISP_CC_COEFF_0_CC_COEFF_0_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_0_CC_COEFF_0_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_0_CC_COEFF_0_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_1 */
#define ISP_ISP_CC_COEFF_1_OFFSET                          (0x174U)
#define ISP_ISP_CC_COEFF_1_CC_COEFF_1_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_1_CC_COEFF_1_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_1_CC_COEFF_1_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_2 */
#define ISP_ISP_CC_COEFF_2_OFFSET                          (0x178U)
#define ISP_ISP_CC_COEFF_2_CC_COEFF_2_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_2_CC_COEFF_2_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_2_CC_COEFF_2_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_3 */
#define ISP_ISP_CC_COEFF_3_OFFSET                          (0x17CU)
#define ISP_ISP_CC_COEFF_3_CC_COEFF_3_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_3_CC_COEFF_3_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_3_CC_COEFF_3_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_4 */
#define ISP_ISP_CC_COEFF_4_OFFSET                          (0x180U)
#define ISP_ISP_CC_COEFF_4_CC_COEFF_4_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_4_CC_COEFF_4_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_4_CC_COEFF_4_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_5 */
#define ISP_ISP_CC_COEFF_5_OFFSET                          (0x184U)
#define ISP_ISP_CC_COEFF_5_CC_COEFF_5_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_5_CC_COEFF_5_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_5_CC_COEFF_5_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_6 */
#define ISP_ISP_CC_COEFF_6_OFFSET                          (0x188U)
#define ISP_ISP_CC_COEFF_6_CC_COEFF_6_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_6_CC_COEFF_6_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_6_CC_COEFF_6_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_7 */
#define ISP_ISP_CC_COEFF_7_OFFSET                          (0x18CU)
#define ISP_ISP_CC_COEFF_7_CC_COEFF_7_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_7_CC_COEFF_7_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_7_CC_COEFF_7_SHIFT)              /* 0x000001FF */
/* ISP_CC_COEFF_8 */
#define ISP_ISP_CC_COEFF_8_OFFSET                          (0x190U)
#define ISP_ISP_CC_COEFF_8_CC_COEFF_8_SHIFT                (0U)
#define ISP_ISP_CC_COEFF_8_CC_COEFF_8_MASK                 (0x1FFU << ISP_ISP_CC_COEFF_8_CC_COEFF_8_SHIFT)              /* 0x000001FF */
/* ISP_OUT_H_OFFS */
#define ISP_ISP_OUT_H_OFFS_OFFSET                          (0x194U)
#define ISP_ISP_OUT_H_OFFS_ISP_OUT_H_OFFS_SHIFT            (0U)
#define ISP_ISP_OUT_H_OFFS_ISP_OUT_H_OFFS_MASK             (0x3FFFU << ISP_ISP_OUT_H_OFFS_ISP_OUT_H_OFFS_SHIFT)         /* 0x00003FFF */
/* ISP_OUT_V_OFFS */
#define ISP_ISP_OUT_V_OFFS_OFFSET                          (0x198U)
#define ISP_ISP_OUT_V_OFFS_ISP_OUT_V_OFFS_SHIFT            (0U)
#define ISP_ISP_OUT_V_OFFS_ISP_OUT_V_OFFS_MASK             (0x3FFFU << ISP_ISP_OUT_V_OFFS_ISP_OUT_V_OFFS_SHIFT)         /* 0x00003FFF */
/* ISP_OUT_H_SIZE */
#define ISP_ISP_OUT_H_SIZE_OFFSET                          (0x19CU)
#define ISP_ISP_OUT_H_SIZE_ISP_OUT_H_SIZE_SHIFT            (0U)
#define ISP_ISP_OUT_H_SIZE_ISP_OUT_H_SIZE_MASK             (0x7FFFU << ISP_ISP_OUT_H_SIZE_ISP_OUT_H_SIZE_SHIFT)         /* 0x00007FFF */
/* ISP_OUT_V_SIZE */
#define ISP_ISP_OUT_V_SIZE_OFFSET                          (0x1A0U)
#define ISP_ISP_OUT_V_SIZE_ISP_OUT_V_SIZE_SHIFT            (0U)
#define ISP_ISP_OUT_V_SIZE_ISP_OUT_V_SIZE_MASK             (0x3FFFU << ISP_ISP_OUT_V_SIZE_ISP_OUT_V_SIZE_SHIFT)         /* 0x00003FFF */
/* ISP_DEMOSAIC */
#define ISP_ISP_DEMOSAIC_OFFSET                            (0x1A4U)
#define ISP_ISP_DEMOSAIC_DEMOSAIC_TH_SHIFT                 (0U)
#define ISP_ISP_DEMOSAIC_DEMOSAIC_TH_MASK                  (0xFFU << ISP_ISP_DEMOSAIC_DEMOSAIC_TH_SHIFT)                /* 0x000000FF */
#define ISP_ISP_DEMOSAIC_DEMOSAIC_BYPASS_SHIFT             (10U)
#define ISP_ISP_DEMOSAIC_DEMOSAIC_BYPASS_MASK              (0x1U << ISP_ISP_DEMOSAIC_DEMOSAIC_BYPASS_SHIFT)             /* 0x00000400 */
/* ISP_FLAGS_SHD */
#define ISP_ISP_FLAGS_SHD_OFFSET                           (0x1A8U)
#define ISP_ISP_FLAGS_SHD                                  (0x0U)
#define ISP_ISP_FLAGS_SHD_ISP_ENABLE_SHD_SHIFT             (0U)
#define ISP_ISP_FLAGS_SHD_ISP_ENABLE_SHD_MASK              (0x1U << ISP_ISP_FLAGS_SHD_ISP_ENABLE_SHD_SHIFT)             /* 0x00000001 */
#define ISP_ISP_FLAGS_SHD_ISP_INFORM_ENABLE_SHD_SHIFT      (1U)
#define ISP_ISP_FLAGS_SHD_ISP_INFORM_ENABLE_SHD_MASK       (0x1U << ISP_ISP_FLAGS_SHD_ISP_INFORM_ENABLE_SHD_SHIFT)      /* 0x00000002 */
#define ISP_ISP_FLAGS_SHD_INFORM_FIELD_SHIFT               (2U)
#define ISP_ISP_FLAGS_SHD_INFORM_FIELD_MASK                (0x1U << ISP_ISP_FLAGS_SHD_INFORM_FIELD_SHIFT)               /* 0x00000004 */
#define ISP_ISP_FLAGS_SHD_S_DATA_SHIFT                     (16U)
#define ISP_ISP_FLAGS_SHD_S_DATA_MASK                      (0xFFFU << ISP_ISP_FLAGS_SHD_S_DATA_SHIFT)                   /* 0x0FFF0000 */
#define ISP_ISP_FLAGS_SHD_S_VSYNC_SHIFT                    (30U)
#define ISP_ISP_FLAGS_SHD_S_VSYNC_MASK                     (0x1U << ISP_ISP_FLAGS_SHD_S_VSYNC_SHIFT)                    /* 0x40000000 */
#define ISP_ISP_FLAGS_SHD_S_HSYNC_SHIFT                    (31U)
#define ISP_ISP_FLAGS_SHD_S_HSYNC_MASK                     (0x1U << ISP_ISP_FLAGS_SHD_S_HSYNC_SHIFT)                    /* 0x80000000 */
/* ISP_OUT_H_OFFS_SHD */
#define ISP_ISP_OUT_H_OFFS_SHD_OFFSET                      (0x1ACU)
#define ISP_ISP_OUT_H_OFFS_SHD                             (0x0U)
#define ISP_ISP_OUT_H_OFFS_SHD_ISP_OUT_H_OFFS_SHD_SHIFT    (0U)
#define ISP_ISP_OUT_H_OFFS_SHD_ISP_OUT_H_OFFS_SHD_MASK     (0x3FFFU << ISP_ISP_OUT_H_OFFS_SHD_ISP_OUT_H_OFFS_SHD_SHIFT) /* 0x00003FFF */
/* ISP_OUT_V_OFFS_SHD */
#define ISP_ISP_OUT_V_OFFS_SHD_OFFSET                      (0x1B0U)
#define ISP_ISP_OUT_V_OFFS_SHD                             (0x0U)
#define ISP_ISP_OUT_V_OFFS_SHD_ISP_OUT_V_OFFS_SHD_SHIFT    (0U)
#define ISP_ISP_OUT_V_OFFS_SHD_ISP_OUT_V_OFFS_SHD_MASK     (0x3FFFU << ISP_ISP_OUT_V_OFFS_SHD_ISP_OUT_V_OFFS_SHD_SHIFT) /* 0x00003FFF */
/* ISP_OUT_H_SIZE_SHD */
#define ISP_ISP_OUT_H_SIZE_SHD_OFFSET                      (0x1B4U)
#define ISP_ISP_OUT_H_SIZE_SHD                             (0x0U)
#define ISP_ISP_OUT_H_SIZE_SHD_ISP_OUT_H_SIZE_SHD_SHIFT    (0U)
#define ISP_ISP_OUT_H_SIZE_SHD_ISP_OUT_H_SIZE_SHD_MASK     (0x7FFFU << ISP_ISP_OUT_H_SIZE_SHD_ISP_OUT_H_SIZE_SHD_SHIFT) /* 0x00007FFF */
/* ISP_OUT_V_SIZE_SHD */
#define ISP_ISP_OUT_V_SIZE_SHD_OFFSET                      (0x1B8U)
#define ISP_ISP_OUT_V_SIZE_SHD                             (0x0U)
#define ISP_ISP_OUT_V_SIZE_SHD_ISP_OUT_V_SIZE_SHD_SHIFT    (0U)
#define ISP_ISP_OUT_V_SIZE_SHD_ISP_OUT_V_SIZE_SHD_MASK     (0x3FFFU << ISP_ISP_OUT_V_SIZE_SHD_ISP_OUT_V_SIZE_SHD_SHIFT) /* 0x00003FFF */
/* ISP_IMSC */
#define ISP_ISP_IMSC_OFFSET                                (0x1BCU)
#define ISP_ISP_IMSC_IMSC_ISP_OFF_SHIFT                    (0U)
#define ISP_ISP_IMSC_IMSC_ISP_OFF_MASK                     (0x1U << ISP_ISP_IMSC_IMSC_ISP_OFF_SHIFT)                    /* 0x00000001 */
#define ISP_ISP_IMSC_IMSC_FRAME_SHIFT                      (1U)
#define ISP_ISP_IMSC_IMSC_FRAME_MASK                       (0x1U << ISP_ISP_IMSC_IMSC_FRAME_SHIFT)                      /* 0x00000002 */
#define ISP_ISP_IMSC_IMSC_DATA_LOSS_SHIFT                  (2U)
#define ISP_ISP_IMSC_IMSC_DATA_LOSS_MASK                   (0x1U << ISP_ISP_IMSC_IMSC_DATA_LOSS_SHIFT)                  /* 0x00000004 */
#define ISP_ISP_IMSC_IMSC_PIC_SIZE_ERR_SHIFT               (3U)
#define ISP_ISP_IMSC_IMSC_PIC_SIZE_ERR_MASK                (0x1U << ISP_ISP_IMSC_IMSC_PIC_SIZE_ERR_SHIFT)               /* 0x00000008 */
#define ISP_ISP_IMSC_IMSC_AWB_DONE_SHIFT                   (4U)
#define ISP_ISP_IMSC_IMSC_AWB_DONE_MASK                    (0x1U << ISP_ISP_IMSC_IMSC_AWB_DONE_SHIFT)                   /* 0x00000010 */
#define ISP_ISP_IMSC_IMSC_FRAME_IN_SHIFT                   (5U)
#define ISP_ISP_IMSC_IMSC_FRAME_IN_MASK                    (0x1U << ISP_ISP_IMSC_IMSC_FRAME_IN_SHIFT)                   /* 0x00000020 */
#define ISP_ISP_IMSC_IMSC_V_START_SHIFT                    (6U)
#define ISP_ISP_IMSC_IMSC_V_START_MASK                     (0x1U << ISP_ISP_IMSC_IMSC_V_START_SHIFT)                    /* 0x00000040 */
#define ISP_ISP_IMSC_IMSC_H_START_SHIFT                    (7U)
#define ISP_ISP_IMSC_IMSC_H_START_MASK                     (0x1U << ISP_ISP_IMSC_IMSC_H_START_SHIFT)                    /* 0x00000080 */
#define ISP_ISP_IMSC_IMSC_FLASH_ON_SHIFT                   (8U)
#define ISP_ISP_IMSC_IMSC_FLASH_ON_MASK                    (0x1U << ISP_ISP_IMSC_IMSC_FLASH_ON_SHIFT)                   /* 0x00000100 */
#define ISP_ISP_IMSC_IMSC_FLASH_OFF_SHIFT                  (9U)
#define ISP_ISP_IMSC_IMSC_FLASH_OFF_MASK                   (0x1U << ISP_ISP_IMSC_IMSC_FLASH_OFF_SHIFT)                  /* 0x00000200 */
#define ISP_ISP_IMSC_IMSC_SHUTTER_ON_SHIFT                 (10U)
#define ISP_ISP_IMSC_IMSC_SHUTTER_ON_MASK                  (0x1U << ISP_ISP_IMSC_IMSC_SHUTTER_ON_SHIFT)                 /* 0x00000400 */
#define ISP_ISP_IMSC_IMSC_SHUTTER_OFF_SHIFT                (11U)
#define ISP_ISP_IMSC_IMSC_SHUTTER_OFF_MASK                 (0x1U << ISP_ISP_IMSC_IMSC_SHUTTER_OFF_SHIFT)                /* 0x00000800 */
#define ISP_ISP_IMSC_IMSC_AFM_SUM_OF_SHIFT                 (12U)
#define ISP_ISP_IMSC_IMSC_AFM_SUM_OF_MASK                  (0x1U << ISP_ISP_IMSC_IMSC_AFM_SUM_OF_SHIFT)                 /* 0x00001000 */
#define ISP_ISP_IMSC_IMSC_AFM_LUM_OF_SHIFT                 (13U)
#define ISP_ISP_IMSC_IMSC_AFM_LUM_OF_MASK                  (0x1U << ISP_ISP_IMSC_IMSC_AFM_LUM_OF_SHIFT)                 /* 0x00002000 */
#define ISP_ISP_IMSC_IMSC_AFM_FIN_SHIFT                    (14U)
#define ISP_ISP_IMSC_IMSC_AFM_FIN_MASK                     (0x1U << ISP_ISP_IMSC_IMSC_AFM_FIN_SHIFT)                    /* 0x00004000 */
#define ISP_ISP_IMSC_IMSC_HIST_MEASURE_RDY_SHIFT           (15U)
#define ISP_ISP_IMSC_IMSC_HIST_MEASURE_RDY_MASK            (0x1U << ISP_ISP_IMSC_IMSC_HIST_MEASURE_RDY_SHIFT)           /* 0x00008000 */
#define ISP_ISP_IMSC_IMSC_RESERVED_1_SHIFT                 (16U)
#define ISP_ISP_IMSC_IMSC_RESERVED_1_MASK                  (0x1U << ISP_ISP_IMSC_IMSC_RESERVED_1_SHIFT)                 /* 0x00010000 */
#define ISP_ISP_IMSC_IMSC_FLASH_CAP_SHIFT                  (17U)
#define ISP_ISP_IMSC_IMSC_FLASH_CAP_MASK                   (0x1U << ISP_ISP_IMSC_IMSC_FLASH_CAP_SHIFT)                  /* 0x00020000 */
#define ISP_ISP_IMSC_IMSC_EXP_END_SHIFT                    (18U)
#define ISP_ISP_IMSC_IMSC_EXP_END_MASK                     (0x1U << ISP_ISP_IMSC_IMSC_EXP_END_SHIFT)                    /* 0x00040000 */
#define ISP_ISP_IMSC_IMSC_VSM_END_SHIFT                    (19U)
#define ISP_ISP_IMSC_IMSC_VSM_END_MASK                     (0x1U << ISP_ISP_IMSC_IMSC_VSM_END_SHIFT)                    /* 0x00080000 */
/* ISP_RIS */
#define ISP_ISP_RIS_OFFSET                                 (0x1C0U)
#define ISP_ISP_RIS                                        (0x0U)
#define ISP_ISP_RIS_RIS_ISP_OFF_SHIFT                      (0U)
#define ISP_ISP_RIS_RIS_ISP_OFF_MASK                       (0x1U << ISP_ISP_RIS_RIS_ISP_OFF_SHIFT)                      /* 0x00000001 */
#define ISP_ISP_RIS_RIS_FRAME_SHIFT                        (1U)
#define ISP_ISP_RIS_RIS_FRAME_MASK                         (0x1U << ISP_ISP_RIS_RIS_FRAME_SHIFT)                        /* 0x00000002 */
#define ISP_ISP_RIS_RIS_DATA_LOSS_SHIFT                    (2U)
#define ISP_ISP_RIS_RIS_DATA_LOSS_MASK                     (0x1U << ISP_ISP_RIS_RIS_DATA_LOSS_SHIFT)                    /* 0x00000004 */
#define ISP_ISP_RIS_RIS_PIC_SIZE_ERR_SHIFT                 (3U)
#define ISP_ISP_RIS_RIS_PIC_SIZE_ERR_MASK                  (0x1U << ISP_ISP_RIS_RIS_PIC_SIZE_ERR_SHIFT)                 /* 0x00000008 */
#define ISP_ISP_RIS_RIS_AWB_DONE_SHIFT                     (4U)
#define ISP_ISP_RIS_RIS_AWB_DONE_MASK                      (0x1U << ISP_ISP_RIS_RIS_AWB_DONE_SHIFT)                     /* 0x00000010 */
#define ISP_ISP_RIS_RIS_FRAME_IN_SHIFT                     (5U)
#define ISP_ISP_RIS_RIS_FRAME_IN_MASK                      (0x1U << ISP_ISP_RIS_RIS_FRAME_IN_SHIFT)                     /* 0x00000020 */
#define ISP_ISP_RIS_RIS_V_START_SHIFT                      (6U)
#define ISP_ISP_RIS_RIS_V_START_MASK                       (0x1U << ISP_ISP_RIS_RIS_V_START_SHIFT)                      /* 0x00000040 */
#define ISP_ISP_RIS_RIS_H_START_SHIFT                      (7U)
#define ISP_ISP_RIS_RIS_H_START_MASK                       (0x1U << ISP_ISP_RIS_RIS_H_START_SHIFT)                      /* 0x00000080 */
#define ISP_ISP_RIS_RIS_FLASH_ON_SHIFT                     (8U)
#define ISP_ISP_RIS_RIS_FLASH_ON_MASK                      (0x1U << ISP_ISP_RIS_RIS_FLASH_ON_SHIFT)                     /* 0x00000100 */
#define ISP_ISP_RIS_RIS_FLASH_OFF_SHIFT                    (9U)
#define ISP_ISP_RIS_RIS_FLASH_OFF_MASK                     (0x1U << ISP_ISP_RIS_RIS_FLASH_OFF_SHIFT)                    /* 0x00000200 */
#define ISP_ISP_RIS_RIS_SHUTTER_ON_SHIFT                   (10U)
#define ISP_ISP_RIS_RIS_SHUTTER_ON_MASK                    (0x1U << ISP_ISP_RIS_RIS_SHUTTER_ON_SHIFT)                   /* 0x00000400 */
#define ISP_ISP_RIS_RIS_SHUTTER_OFF_SHIFT                  (11U)
#define ISP_ISP_RIS_RIS_SHUTTER_OFF_MASK                   (0x1U << ISP_ISP_RIS_RIS_SHUTTER_OFF_SHIFT)                  /* 0x00000800 */
#define ISP_ISP_RIS_RIS_AFM_SUM_OF_SHIFT                   (12U)
#define ISP_ISP_RIS_RIS_AFM_SUM_OF_MASK                    (0x1U << ISP_ISP_RIS_RIS_AFM_SUM_OF_SHIFT)                   /* 0x00001000 */
#define ISP_ISP_RIS_RIS_AFM_LUM_OF_SHIFT                   (13U)
#define ISP_ISP_RIS_RIS_AFM_LUM_OF_MASK                    (0x1U << ISP_ISP_RIS_RIS_AFM_LUM_OF_SHIFT)                   /* 0x00002000 */
#define ISP_ISP_RIS_RIS_AFM_FIN_SHIFT                      (14U)
#define ISP_ISP_RIS_RIS_AFM_FIN_MASK                       (0x1U << ISP_ISP_RIS_RIS_AFM_FIN_SHIFT)                      /* 0x00004000 */
#define ISP_ISP_RIS_RIS_HIST_MEASURE_RDY_SHIFT             (15U)
#define ISP_ISP_RIS_RIS_HIST_MEASURE_RDY_MASK              (0x1U << ISP_ISP_RIS_RIS_HIST_MEASURE_RDY_SHIFT)             /* 0x00008000 */
#define ISP_ISP_RIS_RIS_RESERVED_1_SHIFT                   (16U)
#define ISP_ISP_RIS_RIS_RESERVED_1_MASK                    (0x1U << ISP_ISP_RIS_RIS_RESERVED_1_SHIFT)                   /* 0x00010000 */
#define ISP_ISP_RIS_RIS_FLASH_CAP_SHIFT                    (17U)
#define ISP_ISP_RIS_RIS_FLASH_CAP_MASK                     (0x1U << ISP_ISP_RIS_RIS_FLASH_CAP_SHIFT)                    /* 0x00020000 */
#define ISP_ISP_RIS_RIS_EXP_END_SHIFT                      (18U)
#define ISP_ISP_RIS_RIS_EXP_END_MASK                       (0x1U << ISP_ISP_RIS_RIS_EXP_END_SHIFT)                      /* 0x00040000 */
#define ISP_ISP_RIS_RIS_VSM_END_SHIFT                      (19U)
#define ISP_ISP_RIS_RIS_VSM_END_MASK                       (0x1U << ISP_ISP_RIS_RIS_VSM_END_SHIFT)                      /* 0x00080000 */
/* ISP_MIS */
#define ISP_ISP_MIS_OFFSET                                 (0x1C4U)
#define ISP_ISP_MIS                                        (0x0U)
#define ISP_ISP_MIS_MIS_ISP_OFF_SHIFT                      (0U)
#define ISP_ISP_MIS_MIS_ISP_OFF_MASK                       (0x1U << ISP_ISP_MIS_MIS_ISP_OFF_SHIFT)                      /* 0x00000001 */
#define ISP_ISP_MIS_MIS_FRAME_SHIFT                        (1U)
#define ISP_ISP_MIS_MIS_FRAME_MASK                         (0x1U << ISP_ISP_MIS_MIS_FRAME_SHIFT)                        /* 0x00000002 */
#define ISP_ISP_MIS_MIS_DATA_LOSS_SHIFT                    (2U)
#define ISP_ISP_MIS_MIS_DATA_LOSS_MASK                     (0x1U << ISP_ISP_MIS_MIS_DATA_LOSS_SHIFT)                    /* 0x00000004 */
#define ISP_ISP_MIS_MIS_PIC_SIZE_ERR_SHIFT                 (3U)
#define ISP_ISP_MIS_MIS_PIC_SIZE_ERR_MASK                  (0x1U << ISP_ISP_MIS_MIS_PIC_SIZE_ERR_SHIFT)                 /* 0x00000008 */
#define ISP_ISP_MIS_MIS_AWB_DONE_SHIFT                     (4U)
#define ISP_ISP_MIS_MIS_AWB_DONE_MASK                      (0x1U << ISP_ISP_MIS_MIS_AWB_DONE_SHIFT)                     /* 0x00000010 */
#define ISP_ISP_MIS_MIS_FRAME_IN_SHIFT                     (5U)
#define ISP_ISP_MIS_MIS_FRAME_IN_MASK                      (0x1U << ISP_ISP_MIS_MIS_FRAME_IN_SHIFT)                     /* 0x00000020 */
#define ISP_ISP_MIS_MIS_V_START_SHIFT                      (6U)
#define ISP_ISP_MIS_MIS_V_START_MASK                       (0x1U << ISP_ISP_MIS_MIS_V_START_SHIFT)                      /* 0x00000040 */
#define ISP_ISP_MIS_MIS_H_START_SHIFT                      (7U)
#define ISP_ISP_MIS_MIS_H_START_MASK                       (0x1U << ISP_ISP_MIS_MIS_H_START_SHIFT)                      /* 0x00000080 */
#define ISP_ISP_MIS_MIS_FLASH_ON_SHIFT                     (8U)
#define ISP_ISP_MIS_MIS_FLASH_ON_MASK                      (0x1U << ISP_ISP_MIS_MIS_FLASH_ON_SHIFT)                     /* 0x00000100 */
#define ISP_ISP_MIS_MIS_FLASH_OFF_SHIFT                    (9U)
#define ISP_ISP_MIS_MIS_FLASH_OFF_MASK                     (0x1U << ISP_ISP_MIS_MIS_FLASH_OFF_SHIFT)                    /* 0x00000200 */
#define ISP_ISP_MIS_MIS_SHUTTER_ON_SHIFT                   (10U)
#define ISP_ISP_MIS_MIS_SHUTTER_ON_MASK                    (0x1U << ISP_ISP_MIS_MIS_SHUTTER_ON_SHIFT)                   /* 0x00000400 */
#define ISP_ISP_MIS_MIS_SHUTTER_OFF_SHIFT                  (11U)
#define ISP_ISP_MIS_MIS_SHUTTER_OFF_MASK                   (0x1U << ISP_ISP_MIS_MIS_SHUTTER_OFF_SHIFT)                  /* 0x00000800 */
#define ISP_ISP_MIS_MIS_AFM_SUM_OF_SHIFT                   (12U)
#define ISP_ISP_MIS_MIS_AFM_SUM_OF_MASK                    (0x1U << ISP_ISP_MIS_MIS_AFM_SUM_OF_SHIFT)                   /* 0x00001000 */
#define ISP_ISP_MIS_MIS_AFM_LUM_OF_SHIFT                   (13U)
#define ISP_ISP_MIS_MIS_AFM_LUM_OF_MASK                    (0x1U << ISP_ISP_MIS_MIS_AFM_LUM_OF_SHIFT)                   /* 0x00002000 */
#define ISP_ISP_MIS_MIS_AFM_FIN_SHIFT                      (14U)
#define ISP_ISP_MIS_MIS_AFM_FIN_MASK                       (0x1U << ISP_ISP_MIS_MIS_AFM_FIN_SHIFT)                      /* 0x00004000 */
#define ISP_ISP_MIS_MIS_HIST_MEASURE_RDY_SHIFT             (15U)
#define ISP_ISP_MIS_MIS_HIST_MEASURE_RDY_MASK              (0x1U << ISP_ISP_MIS_MIS_HIST_MEASURE_RDY_SHIFT)             /* 0x00008000 */
#define ISP_ISP_MIS_MIS_RESERVED_1_SHIFT                   (16U)
#define ISP_ISP_MIS_MIS_RESERVED_1_MASK                    (0x1U << ISP_ISP_MIS_MIS_RESERVED_1_SHIFT)                   /* 0x00010000 */
#define ISP_ISP_MIS_MIS_FLASH_CAP_SHIFT                    (17U)
#define ISP_ISP_MIS_MIS_FLASH_CAP_MASK                     (0x1U << ISP_ISP_MIS_MIS_FLASH_CAP_SHIFT)                    /* 0x00020000 */
#define ISP_ISP_MIS_MIS_EXP_END_SHIFT                      (18U)
#define ISP_ISP_MIS_MIS_EXP_END_MASK                       (0x1U << ISP_ISP_MIS_MIS_EXP_END_SHIFT)                      /* 0x00040000 */
#define ISP_ISP_MIS_MIS_VSM_END_SHIFT                      (19U)
#define ISP_ISP_MIS_MIS_VSM_END_MASK                       (0x1U << ISP_ISP_MIS_MIS_VSM_END_SHIFT)                      /* 0x00080000 */
/* ISP_ICR */
#define ISP_ISP_ICR_OFFSET                                 (0x1C8U)
#define ISP_ISP_ICR_ICR_ISP_OFF_SHIFT                      (0U)
#define ISP_ISP_ICR_ICR_ISP_OFF_MASK                       (0x1U << ISP_ISP_ICR_ICR_ISP_OFF_SHIFT)                      /* 0x00000001 */
#define ISP_ISP_ICR_ICR_FRAME_SHIFT                        (1U)
#define ISP_ISP_ICR_ICR_FRAME_MASK                         (0x1U << ISP_ISP_ICR_ICR_FRAME_SHIFT)                        /* 0x00000002 */
#define ISP_ISP_ICR_ICR_DATA_LOSS_SHIFT                    (2U)
#define ISP_ISP_ICR_ICR_DATA_LOSS_MASK                     (0x1U << ISP_ISP_ICR_ICR_DATA_LOSS_SHIFT)                    /* 0x00000004 */
#define ISP_ISP_ICR_ICR_PIC_SIZE_ERR_SHIFT                 (3U)
#define ISP_ISP_ICR_ICR_PIC_SIZE_ERR_MASK                  (0x1U << ISP_ISP_ICR_ICR_PIC_SIZE_ERR_SHIFT)                 /* 0x00000008 */
#define ISP_ISP_ICR_ICR_AWB_DONE_SHIFT                     (4U)
#define ISP_ISP_ICR_ICR_AWB_DONE_MASK                      (0x1U << ISP_ISP_ICR_ICR_AWB_DONE_SHIFT)                     /* 0x00000010 */
#define ISP_ISP_ICR_ICR_FRAME_IN_SHIFT                     (5U)
#define ISP_ISP_ICR_ICR_FRAME_IN_MASK                      (0x1U << ISP_ISP_ICR_ICR_FRAME_IN_SHIFT)                     /* 0x00000020 */
#define ISP_ISP_ICR_ICR_V_START_SHIFT                      (6U)
#define ISP_ISP_ICR_ICR_V_START_MASK                       (0x1U << ISP_ISP_ICR_ICR_V_START_SHIFT)                      /* 0x00000040 */
#define ISP_ISP_ICR_ICR_H_START_SHIFT                      (7U)
#define ISP_ISP_ICR_ICR_H_START_MASK                       (0x1U << ISP_ISP_ICR_ICR_H_START_SHIFT)                      /* 0x00000080 */
#define ISP_ISP_ICR_ICR_FLASH_ON_SHIFT                     (8U)
#define ISP_ISP_ICR_ICR_FLASH_ON_MASK                      (0x1U << ISP_ISP_ICR_ICR_FLASH_ON_SHIFT)                     /* 0x00000100 */
#define ISP_ISP_ICR_ICR_FLASH_OFF_SHIFT                    (9U)
#define ISP_ISP_ICR_ICR_FLASH_OFF_MASK                     (0x1U << ISP_ISP_ICR_ICR_FLASH_OFF_SHIFT)                    /* 0x00000200 */
#define ISP_ISP_ICR_ICR_SHUTTER_ON_SHIFT                   (10U)
#define ISP_ISP_ICR_ICR_SHUTTER_ON_MASK                    (0x1U << ISP_ISP_ICR_ICR_SHUTTER_ON_SHIFT)                   /* 0x00000400 */
#define ISP_ISP_ICR_ICR_SHUTTER_OFF_SHIFT                  (11U)
#define ISP_ISP_ICR_ICR_SHUTTER_OFF_MASK                   (0x1U << ISP_ISP_ICR_ICR_SHUTTER_OFF_SHIFT)                  /* 0x00000800 */
#define ISP_ISP_ICR_ICR_AFM_SUM_OF_SHIFT                   (12U)
#define ISP_ISP_ICR_ICR_AFM_SUM_OF_MASK                    (0x1U << ISP_ISP_ICR_ICR_AFM_SUM_OF_SHIFT)                   /* 0x00001000 */
#define ISP_ISP_ICR_ICR_AFM_LUM_OF_SHIFT                   (13U)
#define ISP_ISP_ICR_ICR_AFM_LUM_OF_MASK                    (0x1U << ISP_ISP_ICR_ICR_AFM_LUM_OF_SHIFT)                   /* 0x00002000 */
#define ISP_ISP_ICR_ICR_AFM_FIN_SHIFT                      (14U)
#define ISP_ISP_ICR_ICR_AFM_FIN_MASK                       (0x1U << ISP_ISP_ICR_ICR_AFM_FIN_SHIFT)                      /* 0x00004000 */
#define ISP_ISP_ICR_ICR_HIST_MEASURE_RDY_SHIFT             (15U)
#define ISP_ISP_ICR_ICR_HIST_MEASURE_RDY_MASK              (0x1U << ISP_ISP_ICR_ICR_HIST_MEASURE_RDY_SHIFT)             /* 0x00008000 */
#define ISP_ISP_ICR_ICR_RESERVED_1_SHIFT                   (16U)
#define ISP_ISP_ICR_ICR_RESERVED_1_MASK                    (0x1U << ISP_ISP_ICR_ICR_RESERVED_1_SHIFT)                   /* 0x00010000 */
#define ISP_ISP_ICR_ICR_FLASH_CAP_SHIFT                    (17U)
#define ISP_ISP_ICR_ICR_FLASH_CAP_MASK                     (0x1U << ISP_ISP_ICR_ICR_FLASH_CAP_SHIFT)                    /* 0x00020000 */
#define ISP_ISP_ICR_ICR_EXP_END_SHIFT                      (18U)
#define ISP_ISP_ICR_ICR_EXP_END_MASK                       (0x1U << ISP_ISP_ICR_ICR_EXP_END_SHIFT)                      /* 0x00040000 */
#define ISP_ISP_ICR_ICR_VSM_END_SHIFT                      (19U)
#define ISP_ISP_ICR_ICR_VSM_END_MASK                       (0x1U << ISP_ISP_ICR_ICR_VSM_END_SHIFT)                      /* 0x00080000 */
/* ISP_ISR */
#define ISP_ISP_ISR_OFFSET                                 (0x1CCU)
#define ISP_ISP_ISR_ISR_ISP_OFF_SHIFT                      (0U)
#define ISP_ISP_ISR_ISR_ISP_OFF_MASK                       (0x1U << ISP_ISP_ISR_ISR_ISP_OFF_SHIFT)                      /* 0x00000001 */
#define ISP_ISP_ISR_ISR_FRAME_SHIFT                        (1U)
#define ISP_ISP_ISR_ISR_FRAME_MASK                         (0x1U << ISP_ISP_ISR_ISR_FRAME_SHIFT)                        /* 0x00000002 */
#define ISP_ISP_ISR_ISR_DATA_LOSS_SHIFT                    (2U)
#define ISP_ISP_ISR_ISR_DATA_LOSS_MASK                     (0x1U << ISP_ISP_ISR_ISR_DATA_LOSS_SHIFT)                    /* 0x00000004 */
#define ISP_ISP_ISR_ISR_PIC_SIZE_ERR_SHIFT                 (3U)
#define ISP_ISP_ISR_ISR_PIC_SIZE_ERR_MASK                  (0x1U << ISP_ISP_ISR_ISR_PIC_SIZE_ERR_SHIFT)                 /* 0x00000008 */
#define ISP_ISP_ISR_ISR_AWB_DONE_SHIFT                     (4U)
#define ISP_ISP_ISR_ISR_AWB_DONE_MASK                      (0x1U << ISP_ISP_ISR_ISR_AWB_DONE_SHIFT)                     /* 0x00000010 */
#define ISP_ISP_ISR_ISR_FRAME_IN_SHIFT                     (5U)
#define ISP_ISP_ISR_ISR_FRAME_IN_MASK                      (0x1U << ISP_ISP_ISR_ISR_FRAME_IN_SHIFT)                     /* 0x00000020 */
#define ISP_ISP_ISR_ISR_V_START_SHIFT                      (6U)
#define ISP_ISP_ISR_ISR_V_START_MASK                       (0x1U << ISP_ISP_ISR_ISR_V_START_SHIFT)                      /* 0x00000040 */
#define ISP_ISP_ISR_ISR_H_START_SHIFT                      (7U)
#define ISP_ISP_ISR_ISR_H_START_MASK                       (0x1U << ISP_ISP_ISR_ISR_H_START_SHIFT)                      /* 0x00000080 */
#define ISP_ISP_ISR_ISR_FLASH_ON_SHIFT                     (8U)
#define ISP_ISP_ISR_ISR_FLASH_ON_MASK                      (0x1U << ISP_ISP_ISR_ISR_FLASH_ON_SHIFT)                     /* 0x00000100 */
#define ISP_ISP_ISR_ISR_FLASH_OFF_SHIFT                    (9U)
#define ISP_ISP_ISR_ISR_FLASH_OFF_MASK                     (0x1U << ISP_ISP_ISR_ISR_FLASH_OFF_SHIFT)                    /* 0x00000200 */
#define ISP_ISP_ISR_ISR_SHUTTER_ON_SHIFT                   (10U)
#define ISP_ISP_ISR_ISR_SHUTTER_ON_MASK                    (0x1U << ISP_ISP_ISR_ISR_SHUTTER_ON_SHIFT)                   /* 0x00000400 */
#define ISP_ISP_ISR_ISR_SHUTTER_OFF_SHIFT                  (11U)
#define ISP_ISP_ISR_ISR_SHUTTER_OFF_MASK                   (0x1U << ISP_ISP_ISR_ISR_SHUTTER_OFF_SHIFT)                  /* 0x00000800 */
#define ISP_ISP_ISR_ISR_AFM_SUM_OF_SHIFT                   (12U)
#define ISP_ISP_ISR_ISR_AFM_SUM_OF_MASK                    (0x1U << ISP_ISP_ISR_ISR_AFM_SUM_OF_SHIFT)                   /* 0x00001000 */
#define ISP_ISP_ISR_ISR_AFM_LUM_OF_SHIFT                   (13U)
#define ISP_ISP_ISR_ISR_AFM_LUM_OF_MASK                    (0x1U << ISP_ISP_ISR_ISR_AFM_LUM_OF_SHIFT)                   /* 0x00002000 */
#define ISP_ISP_ISR_ISR_AFM_FIN_SHIFT                      (14U)
#define ISP_ISP_ISR_ISR_AFM_FIN_MASK                       (0x1U << ISP_ISP_ISR_ISR_AFM_FIN_SHIFT)                      /* 0x00004000 */
#define ISP_ISP_ISR_ISR_HIST_MEASURE_RDY_SHIFT             (15U)
#define ISP_ISP_ISR_ISR_HIST_MEASURE_RDY_MASK              (0x1U << ISP_ISP_ISR_ISR_HIST_MEASURE_RDY_SHIFT)             /* 0x00008000 */
#define ISP_ISP_ISR_ISR_RESERVED_1_SHIFT                   (16U)
#define ISP_ISP_ISR_ISR_RESERVED_1_MASK                    (0x1U << ISP_ISP_ISR_ISR_RESERVED_1_SHIFT)                   /* 0x00010000 */
#define ISP_ISP_ISR_ISR_FLASH_CAP_SHIFT                    (17U)
#define ISP_ISP_ISR_ISR_FLASH_CAP_MASK                     (0x1U << ISP_ISP_ISR_ISR_FLASH_CAP_SHIFT)                    /* 0x00020000 */
#define ISP_ISP_ISR_ISR_EXP_END_SHIFT                      (18U)
#define ISP_ISP_ISR_ISR_EXP_END_MASK                       (0x1U << ISP_ISP_ISR_ISR_EXP_END_SHIFT)                      /* 0x00040000 */
#define ISP_ISP_ISR_ISR_VSM_END_SHIFT                      (19U)
#define ISP_ISP_ISR_ISR_VSM_END_MASK                       (0x1U << ISP_ISP_ISR_ISR_VSM_END_SHIFT)                      /* 0x00080000 */
/* ISP_CT_COEFF_0 */
#define ISP_ISP_CT_COEFF_0_OFFSET                          (0x1D0U)
#define ISP_ISP_CT_COEFF_0_CT_COEFF_0_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_0_CT_COEFF_0_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_0_CT_COEFF_0_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_1 */
#define ISP_ISP_CT_COEFF_1_OFFSET                          (0x1D4U)
#define ISP_ISP_CT_COEFF_1_CT_COEFF_1_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_1_CT_COEFF_1_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_1_CT_COEFF_1_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_2 */
#define ISP_ISP_CT_COEFF_2_OFFSET                          (0x1D8U)
#define ISP_ISP_CT_COEFF_2_CT_COEFF_2_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_2_CT_COEFF_2_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_2_CT_COEFF_2_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_3 */
#define ISP_ISP_CT_COEFF_3_OFFSET                          (0x1DCU)
#define ISP_ISP_CT_COEFF_3_CT_COEFF_3_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_3_CT_COEFF_3_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_3_CT_COEFF_3_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_4 */
#define ISP_ISP_CT_COEFF_4_OFFSET                          (0x1E0U)
#define ISP_ISP_CT_COEFF_4_CT_COEFF_4_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_4_CT_COEFF_4_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_4_CT_COEFF_4_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_5 */
#define ISP_ISP_CT_COEFF_5_OFFSET                          (0x1E4U)
#define ISP_ISP_CT_COEFF_5_CT_COEFF_5_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_5_CT_COEFF_5_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_5_CT_COEFF_5_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_6 */
#define ISP_ISP_CT_COEFF_6_OFFSET                          (0x1E8U)
#define ISP_ISP_CT_COEFF_6_CT_COEFF_6_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_6_CT_COEFF_6_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_6_CT_COEFF_6_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_7 */
#define ISP_ISP_CT_COEFF_7_OFFSET                          (0x1ECU)
#define ISP_ISP_CT_COEFF_7_CT_COEFF_7_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_7_CT_COEFF_7_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_7_CT_COEFF_7_SHIFT)              /* 0x000007FF */
/* ISP_CT_COEFF_8 */
#define ISP_ISP_CT_COEFF_8_OFFSET                          (0x1F0U)
#define ISP_ISP_CT_COEFF_8_CT_COEFF_8_SHIFT                (0U)
#define ISP_ISP_CT_COEFF_8_CT_COEFF_8_MASK                 (0x7FFU << ISP_ISP_CT_COEFF_8_CT_COEFF_8_SHIFT)              /* 0x000007FF */
/* ISP_GAMMA_OUT_MODE */
#define ISP_ISP_GAMMA_OUT_MODE_OFFSET                      (0x1F4U)
#define ISP_ISP_GAMMA_OUT_MODE_EQU_SEGM_SHIFT              (0U)
#define ISP_ISP_GAMMA_OUT_MODE_EQU_SEGM_MASK               (0x1U << ISP_ISP_GAMMA_OUT_MODE_EQU_SEGM_SHIFT)              /* 0x00000001 */
/* ISP_GAMMA_OUT_Y_0 */
#define ISP_ISP_GAMMA_OUT_Y_0_OFFSET                       (0x1F8U)
#define ISP_ISP_GAMMA_OUT_Y_0_ISP_GAMMA_OUT_Y_0_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_0_ISP_GAMMA_OUT_Y_0_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_0_ISP_GAMMA_OUT_Y_0_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_1 */
#define ISP_ISP_GAMMA_OUT_Y_1_OFFSET                       (0x1FCU)
#define ISP_ISP_GAMMA_OUT_Y_1_ISP_GAMMA_OUT_Y_1_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_1_ISP_GAMMA_OUT_Y_1_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_1_ISP_GAMMA_OUT_Y_1_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_2 */
#define ISP_ISP_GAMMA_OUT_Y_2_OFFSET                       (0x200U)
#define ISP_ISP_GAMMA_OUT_Y_2_ISP_GAMMA_OUT_Y_2_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_2_ISP_GAMMA_OUT_Y_2_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_2_ISP_GAMMA_OUT_Y_2_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_3 */
#define ISP_ISP_GAMMA_OUT_Y_3_OFFSET                       (0x204U)
#define ISP_ISP_GAMMA_OUT_Y_3_ISP_GAMMA_OUT_Y_3_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_3_ISP_GAMMA_OUT_Y_3_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_3_ISP_GAMMA_OUT_Y_3_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_4 */
#define ISP_ISP_GAMMA_OUT_Y_4_OFFSET                       (0x208U)
#define ISP_ISP_GAMMA_OUT_Y_4_ISP_GAMMA_OUT_Y_4_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_4_ISP_GAMMA_OUT_Y_4_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_4_ISP_GAMMA_OUT_Y_4_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_5 */
#define ISP_ISP_GAMMA_OUT_Y_5_OFFSET                       (0x20CU)
#define ISP_ISP_GAMMA_OUT_Y_5_ISP_GAMMA_OUT_Y_5_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_5_ISP_GAMMA_OUT_Y_5_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_5_ISP_GAMMA_OUT_Y_5_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_6 */
#define ISP_ISP_GAMMA_OUT_Y_6_OFFSET                       (0x210U)
#define ISP_ISP_GAMMA_OUT_Y_6_ISP_GAMMA_OUT_Y_6_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_6_ISP_GAMMA_OUT_Y_6_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_6_ISP_GAMMA_OUT_Y_6_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_7 */
#define ISP_ISP_GAMMA_OUT_Y_7_OFFSET                       (0x214U)
#define ISP_ISP_GAMMA_OUT_Y_7_ISP_GAMMA_OUT_Y_7_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_7_ISP_GAMMA_OUT_Y_7_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_7_ISP_GAMMA_OUT_Y_7_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_8 */
#define ISP_ISP_GAMMA_OUT_Y_8_OFFSET                       (0x218U)
#define ISP_ISP_GAMMA_OUT_Y_8_ISP_GAMMA_OUT_Y_8_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_8_ISP_GAMMA_OUT_Y_8_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_8_ISP_GAMMA_OUT_Y_8_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_9 */
#define ISP_ISP_GAMMA_OUT_Y_9_OFFSET                       (0x21CU)
#define ISP_ISP_GAMMA_OUT_Y_9_ISP_GAMMA_OUT_Y_9_SHIFT      (0U)
#define ISP_ISP_GAMMA_OUT_Y_9_ISP_GAMMA_OUT_Y_9_MASK       (0x3FFU << ISP_ISP_GAMMA_OUT_Y_9_ISP_GAMMA_OUT_Y_9_SHIFT)    /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_10 */
#define ISP_ISP_GAMMA_OUT_Y_10_OFFSET                      (0x220U)
#define ISP_ISP_GAMMA_OUT_Y_10_ISP_GAMMA_OUT_Y_10_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_10_ISP_GAMMA_OUT_Y_10_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_10_ISP_GAMMA_OUT_Y_10_SHIFT)  /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_11 */
#define ISP_ISP_GAMMA_OUT_Y_11_OFFSET                      (0x224U)
#define ISP_ISP_GAMMA_OUT_Y_11_ISP_GAMMA_OUT_Y_11_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_11_ISP_GAMMA_OUT_Y_11_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_11_ISP_GAMMA_OUT_Y_11_SHIFT)  /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_12 */
#define ISP_ISP_GAMMA_OUT_Y_12_OFFSET                      (0x228U)
#define ISP_ISP_GAMMA_OUT_Y_12_ISP_GAMMA_OUT_Y_12_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_12_ISP_GAMMA_OUT_Y_12_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_12_ISP_GAMMA_OUT_Y_12_SHIFT)  /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_13 */
#define ISP_ISP_GAMMA_OUT_Y_13_OFFSET                      (0x22CU)
#define ISP_ISP_GAMMA_OUT_Y_13_ISP_GAMMA_OUT_Y_13_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_13_ISP_GAMMA_OUT_Y_13_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_13_ISP_GAMMA_OUT_Y_13_SHIFT)  /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_14 */
#define ISP_ISP_GAMMA_OUT_Y_14_OFFSET                      (0x230U)
#define ISP_ISP_GAMMA_OUT_Y_14_ISP_GAMMA_OUT_Y_14_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_14_ISP_GAMMA_OUT_Y_14_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_14_ISP_GAMMA_OUT_Y_14_SHIFT)  /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_15 */
#define ISP_ISP_GAMMA_OUT_Y_15_OFFSET                      (0x234U)
#define ISP_ISP_GAMMA_OUT_Y_15_ISP_GAMMA_OUT_Y_15_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_15_ISP_GAMMA_OUT_Y_15_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_15_ISP_GAMMA_OUT_Y_15_SHIFT)  /* 0x000003FF */
/* ISP_GAMMA_OUT_Y_16 */
#define ISP_ISP_GAMMA_OUT_Y_16_OFFSET                      (0x238U)
#define ISP_ISP_GAMMA_OUT_Y_16_ISP_GAMMA_OUT_Y_16_SHIFT    (0U)
#define ISP_ISP_GAMMA_OUT_Y_16_ISP_GAMMA_OUT_Y_16_MASK     (0x3FFU << ISP_ISP_GAMMA_OUT_Y_16_ISP_GAMMA_OUT_Y_16_SHIFT)  /* 0x000003FF */
/* ISP_ERR */
#define ISP_ISP_ERR_OFFSET                                 (0x23CU)
#define ISP_ISP_ERR                                        (0x0U)
#define ISP_ISP_ERR_INFORM_SIZE_ERR_SHIFT                  (0U)
#define ISP_ISP_ERR_INFORM_SIZE_ERR_MASK                   (0x1U << ISP_ISP_ERR_INFORM_SIZE_ERR_SHIFT)                  /* 0x00000001 */
#define ISP_ISP_ERR_IS_SIZE_ERR_SHIFT                      (1U)
#define ISP_ISP_ERR_IS_SIZE_ERR_MASK                       (0x1U << ISP_ISP_ERR_IS_SIZE_ERR_SHIFT)                      /* 0x00000002 */
#define ISP_ISP_ERR_OUTFORM_SIZE_ERR_SHIFT                 (2U)
#define ISP_ISP_ERR_OUTFORM_SIZE_ERR_MASK                  (0x1U << ISP_ISP_ERR_OUTFORM_SIZE_ERR_SHIFT)                 /* 0x00000004 */
/* ISP_ERR_CLR */
#define ISP_ISP_ERR_CLR_OFFSET                             (0x240U)
#define ISP_ISP_ERR_CLR_INFORM_SIZE_ERR_CLR_SHIFT          (0U)
#define ISP_ISP_ERR_CLR_INFORM_SIZE_ERR_CLR_MASK           (0x1U << ISP_ISP_ERR_CLR_INFORM_SIZE_ERR_CLR_SHIFT)          /* 0x00000001 */
#define ISP_ISP_ERR_CLR_IS_SIZE_ERR_CLR_SHIFT              (1U)
#define ISP_ISP_ERR_CLR_IS_SIZE_ERR_CLR_MASK               (0x1U << ISP_ISP_ERR_CLR_IS_SIZE_ERR_CLR_SHIFT)              /* 0x00000002 */
#define ISP_ISP_ERR_CLR_OUTFORM_SIZE_ERR_CLR_SHIFT         (2U)
#define ISP_ISP_ERR_CLR_OUTFORM_SIZE_ERR_CLR_MASK          (0x1U << ISP_ISP_ERR_CLR_OUTFORM_SIZE_ERR_CLR_SHIFT)         /* 0x00000004 */
/* ISP_FRAME_COUNT */
#define ISP_ISP_FRAME_COUNT_OFFSET                         (0x244U)
#define ISP_ISP_FRAME_COUNT                                (0x0U)
#define ISP_ISP_FRAME_COUNT_FRAME_COUNTER_SHIFT            (0U)
#define ISP_ISP_FRAME_COUNT_FRAME_COUNTER_MASK             (0x3FFU << ISP_ISP_FRAME_COUNT_FRAME_COUNTER_SHIFT)          /* 0x000003FF */
/* ISP_CT_OFFSET_R */
#define ISP_ISP_CT_OFFSET_R_OFFSET                         (0x248U)
#define ISP_ISP_CT_OFFSET_R_CT_OFFSET_R_SHIFT              (0U)
#define ISP_ISP_CT_OFFSET_R_CT_OFFSET_R_MASK               (0xFFFU << ISP_ISP_CT_OFFSET_R_CT_OFFSET_R_SHIFT)            /* 0x00000FFF */
/* ISP_CT_OFFSET_G */
#define ISP_ISP_CT_OFFSET_G_OFFSET                         (0x24CU)
#define ISP_ISP_CT_OFFSET_G_CT_OFFSET_G_SHIFT              (0U)
#define ISP_ISP_CT_OFFSET_G_CT_OFFSET_G_MASK               (0xFFFU << ISP_ISP_CT_OFFSET_G_CT_OFFSET_G_SHIFT)            /* 0x00000FFF */
/* ISP_CT_OFFSET_B */
#define ISP_ISP_CT_OFFSET_B_OFFSET                         (0x250U)
#define ISP_ISP_CT_OFFSET_B_CT_OFFSET_B_SHIFT              (0U)
#define ISP_ISP_CT_OFFSET_B_CT_OFFSET_B_MASK               (0xFFFU << ISP_ISP_CT_OFFSET_B_CT_OFFSET_B_SHIFT)            /* 0x00000FFF */
/***************************************ISP_CPROC****************************************/
/* CPROC_CTRL */
#define ISP_CPROC_CPROC_CTRL_OFFSET                        (0x0U)
#define ISP_CPROC_CPROC_CTRL_CPROC_ENABLE_SHIFT            (0U)
#define ISP_CPROC_CPROC_CTRL_CPROC_ENABLE_MASK             (0x1U << ISP_CPROC_CPROC_CTRL_CPROC_ENABLE_SHIFT)            /* 0x00000001 */
#define ISP_CPROC_CPROC_CTRL_CPROC_Y_OUT_RANGE_SHIFT       (1U)
#define ISP_CPROC_CPROC_CTRL_CPROC_Y_OUT_RANGE_MASK        (0x1U << ISP_CPROC_CPROC_CTRL_CPROC_Y_OUT_RANGE_SHIFT)       /* 0x00000002 */
#define ISP_CPROC_CPROC_CTRL_CPROC_Y_IN_RANGE_SHIFT        (2U)
#define ISP_CPROC_CPROC_CTRL_CPROC_Y_IN_RANGE_MASK         (0x1U << ISP_CPROC_CPROC_CTRL_CPROC_Y_IN_RANGE_SHIFT)        /* 0x00000004 */
#define ISP_CPROC_CPROC_CTRL_CPROC_C_OUT_RANGE_SHIFT       (3U)
#define ISP_CPROC_CPROC_CTRL_CPROC_C_OUT_RANGE_MASK        (0x1U << ISP_CPROC_CPROC_CTRL_CPROC_C_OUT_RANGE_SHIFT)       /* 0x00000008 */
/* CPROC_CONTRAST */
#define ISP_CPROC_CPROC_CONTRAST_OFFSET                    (0x4U)
#define ISP_CPROC_CPROC_CONTRAST_CPROC_CONTRAST_SHIFT      (0U)
#define ISP_CPROC_CPROC_CONTRAST_CPROC_CONTRAST_MASK       (0xFFU << ISP_CPROC_CPROC_CONTRAST_CPROC_CONTRAST_SHIFT)     /* 0x000000FF */
/* CPROC_BRIGHTNESS */
#define ISP_CPROC_CPROC_BRIGHTNESS_OFFSET                  (0x8U)
#define ISP_CPROC_CPROC_BRIGHTNESS_CPROC_BRIGHTNESS_SHIFT  (0U)
#define ISP_CPROC_CPROC_BRIGHTNESS_CPROC_BRIGHTNESS_MASK   (0xFFU << ISP_CPROC_CPROC_BRIGHTNESS_CPROC_BRIGHTNESS_SHIFT) /* 0x000000FF */
/* CPROC_SATURATION */
#define ISP_CPROC_CPROC_SATURATION_OFFSET                  (0xCU)
#define ISP_CPROC_CPROC_SATURATION_CPROC_SATURATION_SHIFT  (0U)
#define ISP_CPROC_CPROC_SATURATION_CPROC_SATURATION_MASK   (0xFFU << ISP_CPROC_CPROC_SATURATION_CPROC_SATURATION_SHIFT) /* 0x000000FF */
/* CPROC_HUE */
#define ISP_CPROC_CPROC_HUE_OFFSET                         (0x10U)
#define ISP_CPROC_CPROC_HUE_CPROC_HUE_SHIFT                (0U)
#define ISP_CPROC_CPROC_HUE_CPROC_HUE_MASK                 (0xFFU << ISP_CPROC_CPROC_HUE_CPROC_HUE_SHIFT)               /* 0x000000FF */
/************************************ISP_MAIN_RESIZE*************************************/
/* MRSZ_CTRL */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_OFFSET                   (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HY_ENABLE_SHIFT    (0U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HY_ENABLE_MASK     (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HY_ENABLE_SHIFT)    /* 0x00000001 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HC_ENABLE_SHIFT    (1U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HC_ENABLE_MASK     (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HC_ENABLE_SHIFT)    /* 0x00000002 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VY_ENABLE_SHIFT    (2U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VY_ENABLE_MASK     (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VY_ENABLE_SHIFT)    /* 0x00000004 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VC_ENABLE_SHIFT    (3U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VC_ENABLE_MASK     (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VC_ENABLE_SHIFT)    /* 0x00000008 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HY_UP_SHIFT        (4U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HY_UP_MASK         (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HY_UP_SHIFT)        /* 0x00000010 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HC_UP_SHIFT        (5U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HC_UP_MASK         (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_HC_UP_SHIFT)        /* 0x00000020 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VY_UP_SHIFT        (6U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VY_UP_MASK         (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VY_UP_SHIFT)        /* 0x00000040 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VC_UP_SHIFT        (7U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VC_UP_MASK         (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SCALE_VC_UP_SHIFT)        /* 0x00000080 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_CFG_UPD_SHIFT            (8U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_CFG_UPD_MASK             (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_CFG_UPD_SHIFT)            /* 0x00000100 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_AUTO_UPD_SHIFT           (9U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_AUTO_UPD_MASK            (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_AUTO_UPD_SHIFT)           /* 0x00000200 */
/* MRSZ_SCALE_HY */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_OFFSET               (0x4U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SCALE_HY_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SCALE_HY_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SCALE_HY_SHIFT)    /* 0x0000FFFF */
/* MRSZ_SCALE_HCB */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_OFFSET              (0x8U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SCALE_HCB_SHIFT     (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SCALE_HCB_MASK      (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SCALE_HCB_SHIFT)  /* 0x0000FFFF */
/* MRSZ_SCALE_HCR */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_OFFSET              (0xCU)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SCALE_HCR_SHIFT     (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SCALE_HCR_MASK      (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SCALE_HCR_SHIFT)  /* 0x0000FFFF */
/* MRSZ_SCALE_VY */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_OFFSET               (0x10U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SCALE_VY_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SCALE_VY_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SCALE_VY_SHIFT)    /* 0x0000FFFF */
/* MRSZ_SCALE_VC */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_OFFSET               (0x14U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SCALE_VC_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SCALE_VC_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SCALE_VC_SHIFT)    /* 0x0000FFFF */
/* MRSZ_PHASE_HY */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_OFFSET               (0x18U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_PHASE_HY_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_PHASE_HY_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_HY_PHASE_HY_SHIFT)    /* 0x0000FFFF */
/* MRSZ_PHASE_HC */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_OFFSET               (0x1CU)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_PHASE_HC_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_PHASE_HC_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_HC_PHASE_HC_SHIFT)    /* 0x0000FFFF */
/* MRSZ_PHASE_VY */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_OFFSET               (0x20U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_PHASE_VY_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_PHASE_VY_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_VY_PHASE_VY_SHIFT)    /* 0x0000FFFF */
/* MRSZ_PHASE_VC */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_OFFSET               (0x24U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_PHASE_VC_SHIFT       (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_PHASE_VC_MASK        (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_VC_PHASE_VC_SHIFT)    /* 0x0000FFFF */
/* MRSZ_SCALE_LUT_ADDR */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_ADDR_OFFSET         (0x28U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_ADDR_SCALE_LUT_ADDR_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_ADDR_SCALE_LUT_ADDR_MASK (0x3FU << ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_ADDR_SCALE_LUT_ADDR_SHIFT) /* 0x0000003F */
/* MRSZ_SCALE_LUT */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_OFFSET              (0x2CU)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_SCALE_LUT_SHIFT     (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_SCALE_LUT_MASK      (0x3FU << ISP_MAIN_RESIZE_MRSZ_SCALE_LUT_SCALE_LUT_SHIFT)    /* 0x0000003F */
/* MRSZ_CTRL_SHD */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_OFFSET               (0x30U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD                      (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HY_ENABLE_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HY_ENABLE_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HY_ENABLE_SHD_SHIFT) /* 0x00000001 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HC_ENABLE_SHD_SHIFT (1U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HC_ENABLE_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HC_ENABLE_SHD_SHIFT) /* 0x00000002 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VY_ENABLE_SHD_SHIFT (2U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VY_ENABLE_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VY_ENABLE_SHD_SHIFT) /* 0x00000004 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VC_ENABLE_SHD_SHIFT (3U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VC_ENABLE_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VC_ENABLE_SHD_SHIFT) /* 0x00000008 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HY_UP_SHD_SHIFT (4U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HY_UP_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HY_UP_SHD_SHIFT) /* 0x00000010 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HC_UP_SHD_SHIFT (5U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HC_UP_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_HC_UP_SHD_SHIFT) /* 0x00000020 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VY_UP_SHD_SHIFT (6U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VY_UP_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VY_UP_SHD_SHIFT) /* 0x00000040 */
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VC_UP_SHD_SHIFT (7U)
#define ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VC_UP_SHD_MASK (0x1U << ISP_MAIN_RESIZE_MRSZ_CTRL_SHD_SCALE_VC_UP_SHD_SHIFT) /* 0x00000080 */
/* MRSZ_SCALE_HY_SHD */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SHD_OFFSET           (0x34U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SHD_SCALE_HY_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SHD_SCALE_HY_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_HY_SHD_SCALE_HY_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_SCALE_HCB_SHD */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SHD_OFFSET          (0x38U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SHD                 (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SHD_SCALE_HCB_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SHD_SCALE_HCB_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_HCB_SHD_SCALE_HCB_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_SCALE_HCR_SHD */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SHD_OFFSET          (0x3CU)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SHD                 (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SHD_SCALE_HCR_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SHD_SCALE_HCR_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_HCR_SHD_SCALE_HCR_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_SCALE_VY_SHD */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SHD_OFFSET           (0x40U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SHD_SCALE_VY_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SHD_SCALE_VY_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_VY_SHD_SCALE_VY_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_SCALE_VC_SHD */
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SHD_OFFSET           (0x44U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SHD_SCALE_VC_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SHD_SCALE_VC_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_SCALE_VC_SHD_SCALE_VC_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_PHASE_HY_SHD */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_SHD_OFFSET           (0x48U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_SHD_PHASE_HY_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HY_SHD_PHASE_HY_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_HY_SHD_PHASE_HY_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_PHASE_HC_SHD */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_SHD_OFFSET           (0x4CU)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_SHD_PHASE_HC_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_HC_SHD_PHASE_HC_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_HC_SHD_PHASE_HC_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_PHASE_VY_SHD */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_SHD_OFFSET           (0x50U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_SHD_PHASE_VY_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VY_SHD_PHASE_VY_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_VY_SHD_PHASE_VY_SHD_SHIFT) /* 0x0000FFFF */
/* MRSZ_PHASE_VC_SHD */
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_SHD_OFFSET           (0x54U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_SHD                  (0x0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_SHD_PHASE_VC_SHD_SHIFT (0U)
#define ISP_MAIN_RESIZE_MRSZ_PHASE_VC_SHD_PHASE_VC_SHD_MASK (0xFFFFU << ISP_MAIN_RESIZE_MRSZ_PHASE_VC_SHD_PHASE_VC_SHD_SHIFT) /* 0x0000FFFF */
/*****************************************ISP_MI*****************************************/
/* MI_CTRL */
#define ISP_MI_MI_CTRL_OFFSET                              (0x0U)
#define ISP_MI_MI_CTRL_PATH_ENABLE_SHIFT                   (0U)
#define ISP_MI_MI_CTRL_PATH_ENABLE_MASK                    (0xFU << ISP_MI_MI_CTRL_PATH_ENABLE_SHIFT)                   /* 0x0000000F */
#define ISP_MI_MI_CTRL_H_FLIP_SHIFT                        (4U)
#define ISP_MI_MI_CTRL_H_FLIP_MASK                         (0x1U << ISP_MI_MI_CTRL_H_FLIP_SHIFT)                        /* 0x00000010 */
#define ISP_MI_MI_CTRL_V_FLIP_SHIFT                        (5U)
#define ISP_MI_MI_CTRL_V_FLIP_MASK                         (0x1U << ISP_MI_MI_CTRL_V_FLIP_SHIFT)                        /* 0x00000020 */
#define ISP_MI_MI_CTRL_ROT_SHIFT                           (6U)
#define ISP_MI_MI_CTRL_ROT_MASK                            (0x1U << ISP_MI_MI_CTRL_ROT_SHIFT)                           /* 0x00000040 */
#define ISP_MI_MI_CTRL_BYTE_SWAP_SHIFT                     (7U)
#define ISP_MI_MI_CTRL_BYTE_SWAP_MASK                      (0x1U << ISP_MI_MI_CTRL_BYTE_SWAP_SHIFT)                     /* 0x00000080 */
#define ISP_MI_MI_CTRL_Y_FULL_RANGE_SHIFT                  (8U)
#define ISP_MI_MI_CTRL_Y_FULL_RANGE_MASK                   (0x1U << ISP_MI_MI_CTRL_Y_FULL_RANGE_SHIFT)                  /* 0x00000100 */
#define ISP_MI_MI_CTRL_CBCR_FULL_RANGE_SHIFT               (9U)
#define ISP_MI_MI_CTRL_CBCR_FULL_RANGE_MASK                (0x1U << ISP_MI_MI_CTRL_CBCR_FULL_RANGE_SHIFT)               /* 0x00000200 */
#define ISP_MI_MI_CTRL_422NONCOSITED_SHIFT                 (10U)
#define ISP_MI_MI_CTRL_422NONCOSITED_MASK                  (0x1U << ISP_MI_MI_CTRL_422NONCOSITED_SHIFT)                 /* 0x00000400 */
#define ISP_MI_MI_CTRL_MP_PINGPONG_ENABLE_SHIFT            (11U)
#define ISP_MI_MI_CTRL_MP_PINGPONG_ENABLE_MASK             (0x1U << ISP_MI_MI_CTRL_MP_PINGPONG_ENABLE_SHIFT)            /* 0x00000800 */
#define ISP_MI_MI_CTRL_SP_PINGPONG_ENABLE_SHIFT            (12U)
#define ISP_MI_MI_CTRL_SP_PINGPONG_ENABLE_MASK             (0x1U << ISP_MI_MI_CTRL_SP_PINGPONG_ENABLE_SHIFT)            /* 0x00001000 */
#define ISP_MI_MI_CTRL_MP_AUTO_UPDATE_SHIFT                (13U)
#define ISP_MI_MI_CTRL_MP_AUTO_UPDATE_MASK                 (0x1U << ISP_MI_MI_CTRL_MP_AUTO_UPDATE_SHIFT)                /* 0x00002000 */
#define ISP_MI_MI_CTRL_SP_AUTO_UPDATE_SHIFT                (14U)
#define ISP_MI_MI_CTRL_SP_AUTO_UPDATE_MASK                 (0x1U << ISP_MI_MI_CTRL_SP_AUTO_UPDATE_SHIFT)                /* 0x00004000 */
#define ISP_MI_MI_CTRL_LAST_PIXEL_SIG_EN_SHIFT             (15U)
#define ISP_MI_MI_CTRL_LAST_PIXEL_SIG_EN_MASK              (0x1U << ISP_MI_MI_CTRL_LAST_PIXEL_SIG_EN_SHIFT)             /* 0x00008000 */
#define ISP_MI_MI_CTRL_BURST_LEN_LUM_SHIFT                 (16U)
#define ISP_MI_MI_CTRL_BURST_LEN_LUM_MASK                  (0x3U << ISP_MI_MI_CTRL_BURST_LEN_LUM_SHIFT)                 /* 0x00030000 */
#define ISP_MI_MI_CTRL_BURST_LEN_CHROM_SHIFT               (18U)
#define ISP_MI_MI_CTRL_BURST_LEN_CHROM_MASK                (0x3U << ISP_MI_MI_CTRL_BURST_LEN_CHROM_SHIFT)               /* 0x000C0000 */
#define ISP_MI_MI_CTRL_INIT_BASE_EN_SHIFT                  (20U)
#define ISP_MI_MI_CTRL_INIT_BASE_EN_MASK                   (0x1U << ISP_MI_MI_CTRL_INIT_BASE_EN_SHIFT)                  /* 0x00100000 */
#define ISP_MI_MI_CTRL_INIT_OFFSET_EN_SHIFT                (21U)
#define ISP_MI_MI_CTRL_INIT_OFFSET_EN_MASK                 (0x1U << ISP_MI_MI_CTRL_INIT_OFFSET_EN_SHIFT)                /* 0x00200000 */
#define ISP_MI_MI_CTRL_MP_WRITE_FORMAT_SHIFT               (22U)
#define ISP_MI_MI_CTRL_MP_WRITE_FORMAT_MASK                (0x3U << ISP_MI_MI_CTRL_MP_WRITE_FORMAT_SHIFT)               /* 0x00C00000 */
#define ISP_MI_MI_CTRL_SP_WRITE_FORMAT_SHIFT               (24U)
#define ISP_MI_MI_CTRL_SP_WRITE_FORMAT_MASK                (0x3U << ISP_MI_MI_CTRL_SP_WRITE_FORMAT_SHIFT)               /* 0x03000000 */
#define ISP_MI_MI_CTRL_SP_INPUT_FORMAT_SHIFT               (26U)
#define ISP_MI_MI_CTRL_SP_INPUT_FORMAT_MASK                (0x3U << ISP_MI_MI_CTRL_SP_INPUT_FORMAT_SHIFT)               /* 0x0C000000 */
#define ISP_MI_MI_CTRL_SP_OUTPUT_FORMAT_SHIFT              (28U)
#define ISP_MI_MI_CTRL_SP_OUTPUT_FORMAT_MASK               (0x7U << ISP_MI_MI_CTRL_SP_OUTPUT_FORMAT_SHIFT)              /* 0x70000000 */
/* MI_INIT */
#define ISP_MI_MI_INIT_OFFSET                              (0x4U)
#define ISP_MI_MI_INIT_MI_SKIP_SHIFT                       (2U)
#define ISP_MI_MI_INIT_MI_SKIP_MASK                        (0x1U << ISP_MI_MI_INIT_MI_SKIP_SHIFT)                       /* 0x00000004 */
#define ISP_MI_MI_INIT_MI_CFG_UPD_SHIFT                    (4U)
#define ISP_MI_MI_INIT_MI_CFG_UPD_MASK                     (0x1U << ISP_MI_MI_INIT_MI_CFG_UPD_SHIFT)                    /* 0x00000010 */
/* MI_MP_Y_BASE_AD_INIT */
#define ISP_MI_MI_MP_Y_BASE_AD_INIT_OFFSET                 (0x8U)
#define ISP_MI_MI_MP_Y_BASE_AD_INIT_MP_Y_BASE_AD_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_Y_BASE_AD_INIT_MP_Y_BASE_AD_INIT_MASK (0x1FFFFFFFU << ISP_MI_MI_MP_Y_BASE_AD_INIT_MP_Y_BASE_AD_INIT_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_Y_SIZE_INIT */
#define ISP_MI_MI_MP_Y_SIZE_INIT_OFFSET                    (0xCU)
#define ISP_MI_MI_MP_Y_SIZE_INIT_MP_Y_SIZE_INIT_SHIFT      (3U)
#define ISP_MI_MI_MP_Y_SIZE_INIT_MP_Y_SIZE_INIT_MASK       (0x3FFFFFFU << ISP_MI_MI_MP_Y_SIZE_INIT_MP_Y_SIZE_INIT_SHIFT) /* 0x1FFFFFF8 */
/* MI_MP_Y_OFFS_CNT_INIT */
#define ISP_MI_MI_MP_Y_OFFS_CNT_INIT_OFFSET                (0x10U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_INIT_MP_Y_OFFS_CNT_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_INIT_MP_Y_OFFS_CNT_INIT_MASK (0x3FFFFFFU << ISP_MI_MI_MP_Y_OFFS_CNT_INIT_MP_Y_OFFS_CNT_INIT_SHIFT) /* 0x1FFFFFF8 */
/* MI_MP_Y_OFFS_CNT_START */
#define ISP_MI_MI_MP_Y_OFFS_CNT_START_OFFSET               (0x14U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_START                      (0x0U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_START_MP_Y_OFFS_CNT_START_SHIFT (3U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_START_MP_Y_OFFS_CNT_START_MASK (0x3FFFFFFU << ISP_MI_MI_MP_Y_OFFS_CNT_START_MP_Y_OFFS_CNT_START_SHIFT) /* 0x1FFFFFF8 */
/* MI_MP_Y_IRQ_OFFS_INIT */
#define ISP_MI_MI_MP_Y_IRQ_OFFS_INIT_OFFSET                (0x18U)
#define ISP_MI_MI_MP_Y_IRQ_OFFS_INIT_MP_Y_IRQ_OFFS_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_Y_IRQ_OFFS_INIT_MP_Y_IRQ_OFFS_INIT_MASK (0x3FFFFFFU << ISP_MI_MI_MP_Y_IRQ_OFFS_INIT_MP_Y_IRQ_OFFS_INIT_SHIFT) /* 0x1FFFFFF8 */
/* MI_MP_CB_BASE_AD_INIT */
#define ISP_MI_MI_MP_CB_BASE_AD_INIT_OFFSET                (0x1CU)
#define ISP_MI_MI_MP_CB_BASE_AD_INIT_MP_CB_BASE_AD_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_CB_BASE_AD_INIT_MP_CB_BASE_AD_INIT_MASK (0x1FFFFFFFU << ISP_MI_MI_MP_CB_BASE_AD_INIT_MP_CB_BASE_AD_INIT_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_CB_SIZE_INIT */
#define ISP_MI_MI_MP_CB_SIZE_INIT_OFFSET                   (0x20U)
#define ISP_MI_MI_MP_CB_SIZE_INIT_MP_CB_SIZE_INIT_SHIFT    (3U)
#define ISP_MI_MI_MP_CB_SIZE_INIT_MP_CB_SIZE_INIT_MASK     (0x1FFFFFFU << ISP_MI_MI_MP_CB_SIZE_INIT_MP_CB_SIZE_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_MP_CB_OFFS_CNT_INIT */
#define ISP_MI_MI_MP_CB_OFFS_CNT_INIT_OFFSET               (0x24U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_INIT_MP_CB_OFFS_CNT_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_INIT_MP_CB_OFFS_CNT_INIT_MASK (0x1FFFFFFU << ISP_MI_MI_MP_CB_OFFS_CNT_INIT_MP_CB_OFFS_CNT_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_MP_CB_OFFS_CNT_START */
#define ISP_MI_MI_MP_CB_OFFS_CNT_START_OFFSET              (0x28U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_START                     (0x0U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_START_MP_CB_OFFS_CNT_START_SHIFT (3U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_START_MP_CB_OFFS_CNT_START_MASK (0x1FFFFFFU << ISP_MI_MI_MP_CB_OFFS_CNT_START_MP_CB_OFFS_CNT_START_SHIFT) /* 0x0FFFFFF8 */
/* MI_MP_CR_BASE_AD_INIT */
#define ISP_MI_MI_MP_CR_BASE_AD_INIT_OFFSET                (0x2CU)
#define ISP_MI_MI_MP_CR_BASE_AD_INIT_MP_CR_BASE_AD_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_CR_BASE_AD_INIT_MP_CR_BASE_AD_INIT_MASK (0x1FFFFFFFU << ISP_MI_MI_MP_CR_BASE_AD_INIT_MP_CR_BASE_AD_INIT_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_CR_SIZE_INIT */
#define ISP_MI_MI_MP_CR_SIZE_INIT_OFFSET                   (0x30U)
#define ISP_MI_MI_MP_CR_SIZE_INIT_MP_CR_SIZE_INIT_SHIFT    (3U)
#define ISP_MI_MI_MP_CR_SIZE_INIT_MP_CR_SIZE_INIT_MASK     (0x1FFFFFFU << ISP_MI_MI_MP_CR_SIZE_INIT_MP_CR_SIZE_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_MP_CR_OFFS_CNT_INIT */
#define ISP_MI_MI_MP_CR_OFFS_CNT_INIT_OFFSET               (0x34U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_INIT_MP_CR_OFFS_CNT_INIT_SHIFT (3U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_INIT_MP_CR_OFFS_CNT_INIT_MASK (0x1FFFFFFU << ISP_MI_MI_MP_CR_OFFS_CNT_INIT_MP_CR_OFFS_CNT_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_MP_CR_OFFS_CNT_START */
#define ISP_MI_MI_MP_CR_OFFS_CNT_START_OFFSET              (0x38U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_START                     (0x0U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_START_MP_CR_OFFS_CNT_START_SHIFT (3U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_START_MP_CR_OFFS_CNT_START_MASK (0x1FFFFFFU << ISP_MI_MI_MP_CR_OFFS_CNT_START_MP_CR_OFFS_CNT_START_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_Y_BASE_AD_INIT */
#define ISP_MI_MI_SP_Y_BASE_AD_INIT_OFFSET                 (0x3CU)
#define ISP_MI_MI_SP_Y_BASE_AD_INIT_SP_Y_BASE_AD_INIT_SHIFT (3U)
#define ISP_MI_MI_SP_Y_BASE_AD_INIT_SP_Y_BASE_AD_INIT_MASK (0x1FFFFFFFU << ISP_MI_MI_SP_Y_BASE_AD_INIT_SP_Y_BASE_AD_INIT_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_Y_SIZE_INIT */
#define ISP_MI_MI_SP_Y_SIZE_INIT_OFFSET                    (0x40U)
#define ISP_MI_MI_SP_Y_SIZE_INIT_SP_Y_SIZE_INIT_SHIFT      (3U)
#define ISP_MI_MI_SP_Y_SIZE_INIT_SP_Y_SIZE_INIT_MASK       (0x3FFFFFFU << ISP_MI_MI_SP_Y_SIZE_INIT_SP_Y_SIZE_INIT_SHIFT) /* 0x1FFFFFF8 */
/* MI_SP_Y_OFFS_CNT_INIT */
#define ISP_MI_MI_SP_Y_OFFS_CNT_INIT_OFFSET                (0x44U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_INIT_SP_Y_OFFS_CNT_INIT_SHIFT (3U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_INIT_SP_Y_OFFS_CNT_INIT_MASK (0x3FFFFFFU << ISP_MI_MI_SP_Y_OFFS_CNT_INIT_SP_Y_OFFS_CNT_INIT_SHIFT) /* 0x1FFFFFF8 */
/* MI_SP_Y_OFFS_CNT_START */
#define ISP_MI_MI_SP_Y_OFFS_CNT_START_OFFSET               (0x48U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_START                      (0x0U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_START_SP_Y_OFFS_CNT_START_SHIFT (3U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_START_SP_Y_OFFS_CNT_START_MASK (0x3FFFFFFU << ISP_MI_MI_SP_Y_OFFS_CNT_START_SP_Y_OFFS_CNT_START_SHIFT) /* 0x1FFFFFF8 */
/* MI_SP_Y_LLENGTH */
#define ISP_MI_MI_SP_Y_LLENGTH_OFFSET                      (0x4CU)
#define ISP_MI_MI_SP_Y_LLENGTH_SP_Y_LLENGTH_SHIFT          (0U)
#define ISP_MI_MI_SP_Y_LLENGTH_SP_Y_LLENGTH_MASK           (0x7FFFU << ISP_MI_MI_SP_Y_LLENGTH_SP_Y_LLENGTH_SHIFT)       /* 0x00007FFF */
/* MI_SP_CB_BASE_AD_INIT */
#define ISP_MI_MI_SP_CB_BASE_AD_INIT_OFFSET                (0x50U)
#define ISP_MI_MI_SP_CB_BASE_AD_INIT_SP_CB_BASE_AD_INIT_SHIFT (3U)
#define ISP_MI_MI_SP_CB_BASE_AD_INIT_SP_CB_BASE_AD_INIT_MASK (0x1FFFFFFFU << ISP_MI_MI_SP_CB_BASE_AD_INIT_SP_CB_BASE_AD_INIT_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_CB_SIZE_INIT */
#define ISP_MI_MI_SP_CB_SIZE_INIT_OFFSET                   (0x54U)
#define ISP_MI_MI_SP_CB_SIZE_INIT_SP_CB_SIZE_INIT_SHIFT    (3U)
#define ISP_MI_MI_SP_CB_SIZE_INIT_SP_CB_SIZE_INIT_MASK     (0x1FFFFFFU << ISP_MI_MI_SP_CB_SIZE_INIT_SP_CB_SIZE_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_CB_OFFS_CNT_INIT */
#define ISP_MI_MI_SP_CB_OFFS_CNT_INIT_OFFSET               (0x58U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_INIT_SP_CB_OFFS_CNT_INIT_SHIFT (3U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_INIT_SP_CB_OFFS_CNT_INIT_MASK (0x1FFFFFFU << ISP_MI_MI_SP_CB_OFFS_CNT_INIT_SP_CB_OFFS_CNT_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_CB_OFFS_CNT_START */
#define ISP_MI_MI_SP_CB_OFFS_CNT_START_OFFSET              (0x5CU)
#define ISP_MI_MI_SP_CB_OFFS_CNT_START                     (0x0U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_START_SP_CB_OFFS_CNT_START_SHIFT (3U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_START_SP_CB_OFFS_CNT_START_MASK (0x1FFFFFFU << ISP_MI_MI_SP_CB_OFFS_CNT_START_SP_CB_OFFS_CNT_START_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_CR_BASE_AD_INIT */
#define ISP_MI_MI_SP_CR_BASE_AD_INIT_OFFSET                (0x60U)
#define ISP_MI_MI_SP_CR_BASE_AD_INIT_SP_CR_BASE_AD_INIT_SHIFT (3U)
#define ISP_MI_MI_SP_CR_BASE_AD_INIT_SP_CR_BASE_AD_INIT_MASK (0x1FFFFFFFU << ISP_MI_MI_SP_CR_BASE_AD_INIT_SP_CR_BASE_AD_INIT_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_CR_SIZE_INIT */
#define ISP_MI_MI_SP_CR_SIZE_INIT_OFFSET                   (0x64U)
#define ISP_MI_MI_SP_CR_SIZE_INIT_SP_CR_SIZE_INIT_SHIFT    (3U)
#define ISP_MI_MI_SP_CR_SIZE_INIT_SP_CR_SIZE_INIT_MASK     (0x1FFFFFFU << ISP_MI_MI_SP_CR_SIZE_INIT_SP_CR_SIZE_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_CR_OFFS_CNT_INIT */
#define ISP_MI_MI_SP_CR_OFFS_CNT_INIT_OFFSET               (0x68U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_INIT_SP_CR_OFFS_CNT_INIT_SHIFT (3U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_INIT_SP_CR_OFFS_CNT_INIT_MASK (0x1FFFFFFU << ISP_MI_MI_SP_CR_OFFS_CNT_INIT_SP_CR_OFFS_CNT_INIT_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_CR_OFFS_CNT_START */
#define ISP_MI_MI_SP_CR_OFFS_CNT_START_OFFSET              (0x6CU)
#define ISP_MI_MI_SP_CR_OFFS_CNT_START                     (0x0U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_START_SP_CR_OFFS_CNT_START_SHIFT (3U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_START_SP_CR_OFFS_CNT_START_MASK (0x1FFFFFFU << ISP_MI_MI_SP_CR_OFFS_CNT_START_SP_CR_OFFS_CNT_START_SHIFT) /* 0x0FFFFFF8 */
/* MI_BYTE_CNT */
#define ISP_MI_MI_BYTE_CNT_OFFSET                          (0x70U)
#define ISP_MI_MI_BYTE_CNT                                 (0x0U)
#define ISP_MI_MI_BYTE_CNT_BYTE_CNT_SHIFT                  (0U)
#define ISP_MI_MI_BYTE_CNT_BYTE_CNT_MASK                   (0xFFFFFFFU << ISP_MI_MI_BYTE_CNT_BYTE_CNT_SHIFT)            /* 0x0FFFFFFF */
/* MI_CTRL_SHD */
#define ISP_MI_MI_CTRL_SHD_OFFSET                          (0x74U)
#define ISP_MI_MI_CTRL_SHD                                 (0x0U)
#define ISP_MI_MI_CTRL_SHD_PATH_ENABLE_IN_SHIFT            (0U)
#define ISP_MI_MI_CTRL_SHD_PATH_ENABLE_IN_MASK             (0xFU << ISP_MI_MI_CTRL_SHD_PATH_ENABLE_IN_SHIFT)            /* 0x0000000F */
#define ISP_MI_MI_CTRL_SHD_PATH_ENABLE_OUT_SHIFT           (16U)
#define ISP_MI_MI_CTRL_SHD_PATH_ENABLE_OUT_MASK            (0xFU << ISP_MI_MI_CTRL_SHD_PATH_ENABLE_OUT_SHIFT)           /* 0x000F0000 */
/* MI_MP_Y_BASE_AD_SHD */
#define ISP_MI_MI_MP_Y_BASE_AD_SHD_OFFSET                  (0x78U)
#define ISP_MI_MI_MP_Y_BASE_AD_SHD                         (0x0U)
#define ISP_MI_MI_MP_Y_BASE_AD_SHD_MP_Y_BASE_AD_SHIFT      (3U)
#define ISP_MI_MI_MP_Y_BASE_AD_SHD_MP_Y_BASE_AD_MASK       (0x1FFFFFFFU << ISP_MI_MI_MP_Y_BASE_AD_SHD_MP_Y_BASE_AD_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_Y_SIZE_SHD */
#define ISP_MI_MI_MP_Y_SIZE_SHD_OFFSET                     (0x7CU)
#define ISP_MI_MI_MP_Y_SIZE_SHD                            (0x0U)
#define ISP_MI_MI_MP_Y_SIZE_SHD_MP_Y_SIZE_SHIFT            (3U)
#define ISP_MI_MI_MP_Y_SIZE_SHD_MP_Y_SIZE_MASK             (0x3FFFFFFU << ISP_MI_MI_MP_Y_SIZE_SHD_MP_Y_SIZE_SHIFT)      /* 0x1FFFFFF8 */
/* MI_MP_Y_OFFS_CNT_SHD */
#define ISP_MI_MI_MP_Y_OFFS_CNT_SHD_OFFSET                 (0x80U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_SHD                        (0x0U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_SHD_MP_Y_OFFS_CNT_SHIFT    (3U)
#define ISP_MI_MI_MP_Y_OFFS_CNT_SHD_MP_Y_OFFS_CNT_MASK     (0x3FFFFFFU << ISP_MI_MI_MP_Y_OFFS_CNT_SHD_MP_Y_OFFS_CNT_SHIFT) /* 0x1FFFFFF8 */
/* MI_MP_Y_IRQ_OFFS_SHD */
#define ISP_MI_MI_MP_Y_IRQ_OFFS_SHD_OFFSET                 (0x84U)
#define ISP_MI_MI_MP_Y_IRQ_OFFS_SHD                        (0x0U)
#define ISP_MI_MI_MP_Y_IRQ_OFFS_SHD_MP_Y_IRQ_OFFS_SHIFT    (3U)
#define ISP_MI_MI_MP_Y_IRQ_OFFS_SHD_MP_Y_IRQ_OFFS_MASK     (0x3FFFFFFU << ISP_MI_MI_MP_Y_IRQ_OFFS_SHD_MP_Y_IRQ_OFFS_SHIFT) /* 0x1FFFFFF8 */
/* MI_MP_CB_BASE_AD_SHD */
#define ISP_MI_MI_MP_CB_BASE_AD_SHD_OFFSET                 (0x88U)
#define ISP_MI_MI_MP_CB_BASE_AD_SHD                        (0x0U)
#define ISP_MI_MI_MP_CB_BASE_AD_SHD_MP_CB_BASE_AD_SHIFT    (3U)
#define ISP_MI_MI_MP_CB_BASE_AD_SHD_MP_CB_BASE_AD_MASK     (0x1FFFFFFFU << ISP_MI_MI_MP_CB_BASE_AD_SHD_MP_CB_BASE_AD_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_CB_SIZE_SHD */
#define ISP_MI_MI_MP_CB_SIZE_SHD_OFFSET                    (0x8CU)
#define ISP_MI_MI_MP_CB_SIZE_SHD                           (0x0U)
#define ISP_MI_MI_MP_CB_SIZE_SHD_MP_CB_SIZE_SHIFT          (3U)
#define ISP_MI_MI_MP_CB_SIZE_SHD_MP_CB_SIZE_MASK           (0x1FFFFFFU << ISP_MI_MI_MP_CB_SIZE_SHD_MP_CB_SIZE_SHIFT)    /* 0x0FFFFFF8 */
/* MI_MP_CB_OFFS_CNT_SHD */
#define ISP_MI_MI_MP_CB_OFFS_CNT_SHD_OFFSET                (0x90U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_SHD                       (0x0U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_SHD_MP_CB_OFFS_CNT_SHIFT  (3U)
#define ISP_MI_MI_MP_CB_OFFS_CNT_SHD_MP_CB_OFFS_CNT_MASK   (0x1FFFFFFU << ISP_MI_MI_MP_CB_OFFS_CNT_SHD_MP_CB_OFFS_CNT_SHIFT) /* 0x0FFFFFF8 */
/* MI_MP_CR_BASE_AD_SHD */
#define ISP_MI_MI_MP_CR_BASE_AD_SHD_OFFSET                 (0x94U)
#define ISP_MI_MI_MP_CR_BASE_AD_SHD                        (0x0U)
#define ISP_MI_MI_MP_CR_BASE_AD_SHD_MP_CR_BASE_AD_SHIFT    (3U)
#define ISP_MI_MI_MP_CR_BASE_AD_SHD_MP_CR_BASE_AD_MASK     (0x1FFFFFFFU << ISP_MI_MI_MP_CR_BASE_AD_SHD_MP_CR_BASE_AD_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_CR_SIZE_SHD */
#define ISP_MI_MI_MP_CR_SIZE_SHD_OFFSET                    (0x98U)
#define ISP_MI_MI_MP_CR_SIZE_SHD                           (0x0U)
#define ISP_MI_MI_MP_CR_SIZE_SHD_MP_CR_SIZE_SHIFT          (3U)
#define ISP_MI_MI_MP_CR_SIZE_SHD_MP_CR_SIZE_MASK           (0x1FFFFFFU << ISP_MI_MI_MP_CR_SIZE_SHD_MP_CR_SIZE_SHIFT)    /* 0x0FFFFFF8 */
/* MI_MP_CR_OFFS_CNT_SHD */
#define ISP_MI_MI_MP_CR_OFFS_CNT_SHD_OFFSET                (0x9CU)
#define ISP_MI_MI_MP_CR_OFFS_CNT_SHD                       (0x0U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_SHD_MP_CR_OFFS_CNT_SHIFT  (3U)
#define ISP_MI_MI_MP_CR_OFFS_CNT_SHD_MP_CR_OFFS_CNT_MASK   (0x1FFFFFFU << ISP_MI_MI_MP_CR_OFFS_CNT_SHD_MP_CR_OFFS_CNT_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_Y_BASE_AD_SHD */
#define ISP_MI_MI_SP_Y_BASE_AD_SHD_OFFSET                  (0xA0U)
#define ISP_MI_MI_SP_Y_BASE_AD_SHD                         (0x0U)
#define ISP_MI_MI_SP_Y_BASE_AD_SHD_SP_Y_BASE_AD_SHIFT      (3U)
#define ISP_MI_MI_SP_Y_BASE_AD_SHD_SP_Y_BASE_AD_MASK       (0x1FFFFFFFU << ISP_MI_MI_SP_Y_BASE_AD_SHD_SP_Y_BASE_AD_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_Y_SIZE_SHD */
#define ISP_MI_MI_SP_Y_SIZE_SHD_OFFSET                     (0xA4U)
#define ISP_MI_MI_SP_Y_SIZE_SHD                            (0x0U)
#define ISP_MI_MI_SP_Y_SIZE_SHD_SP_Y_SIZE_SHIFT            (3U)
#define ISP_MI_MI_SP_Y_SIZE_SHD_SP_Y_SIZE_MASK             (0x3FFFFFFU << ISP_MI_MI_SP_Y_SIZE_SHD_SP_Y_SIZE_SHIFT)      /* 0x1FFFFFF8 */
/* MI_SP_Y_OFFS_CNT_SHD */
#define ISP_MI_MI_SP_Y_OFFS_CNT_SHD_OFFSET                 (0xA8U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_SHD                        (0x0U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_SHD_SP_Y_OFFS_CNT_SHIFT    (3U)
#define ISP_MI_MI_SP_Y_OFFS_CNT_SHD_SP_Y_OFFS_CNT_MASK     (0x3FFFFFFU << ISP_MI_MI_SP_Y_OFFS_CNT_SHD_SP_Y_OFFS_CNT_SHIFT) /* 0x1FFFFFF8 */
/* MI_SP_CB_BASE_AD_SHD */
#define ISP_MI_MI_SP_CB_BASE_AD_SHD_OFFSET                 (0xB0U)
#define ISP_MI_MI_SP_CB_BASE_AD_SHD                        (0x0U)
#define ISP_MI_MI_SP_CB_BASE_AD_SHD_SP_CB_BASE_AD_SHIFT    (3U)
#define ISP_MI_MI_SP_CB_BASE_AD_SHD_SP_CB_BASE_AD_MASK     (0x1FFFFFFFU << ISP_MI_MI_SP_CB_BASE_AD_SHD_SP_CB_BASE_AD_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_CB_SIZE_SHD */
#define ISP_MI_MI_SP_CB_SIZE_SHD_OFFSET                    (0xB4U)
#define ISP_MI_MI_SP_CB_SIZE_SHD                           (0x0U)
#define ISP_MI_MI_SP_CB_SIZE_SHD_SP_CB_SIZE_SHIFT          (3U)
#define ISP_MI_MI_SP_CB_SIZE_SHD_SP_CB_SIZE_MASK           (0x1FFFFFFU << ISP_MI_MI_SP_CB_SIZE_SHD_SP_CB_SIZE_SHIFT)    /* 0x0FFFFFF8 */
/* MI_SP_CB_OFFS_CNT_SHD */
#define ISP_MI_MI_SP_CB_OFFS_CNT_SHD_OFFSET                (0xB8U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_SHD                       (0x0U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_SHD_SP_CB_OFFS_CNT_SHIFT  (3U)
#define ISP_MI_MI_SP_CB_OFFS_CNT_SHD_SP_CB_OFFS_CNT_MASK   (0x1FFFFFFU << ISP_MI_MI_SP_CB_OFFS_CNT_SHD_SP_CB_OFFS_CNT_SHIFT) /* 0x0FFFFFF8 */
/* MI_SP_CR_BASE_AD_SHD */
#define ISP_MI_MI_SP_CR_BASE_AD_SHD_OFFSET                 (0xBCU)
#define ISP_MI_MI_SP_CR_BASE_AD_SHD                        (0x0U)
#define ISP_MI_MI_SP_CR_BASE_AD_SHD_SP_CR_BASE_AD_SHIFT    (3U)
#define ISP_MI_MI_SP_CR_BASE_AD_SHD_SP_CR_BASE_AD_MASK     (0x1FFFFFFFU << ISP_MI_MI_SP_CR_BASE_AD_SHD_SP_CR_BASE_AD_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_CR_SIZE_SHD */
#define ISP_MI_MI_SP_CR_SIZE_SHD_OFFSET                    (0xC0U)
#define ISP_MI_MI_SP_CR_SIZE_SHD                           (0x0U)
#define ISP_MI_MI_SP_CR_SIZE_SHD_SP_CR_SIZE_SHIFT          (3U)
#define ISP_MI_MI_SP_CR_SIZE_SHD_SP_CR_SIZE_MASK           (0x1FFFFFFU << ISP_MI_MI_SP_CR_SIZE_SHD_SP_CR_SIZE_SHIFT)    /* 0x0FFFFFF8 */
/* MI_SP_CR_OFFS_CNT_SHD */
#define ISP_MI_MI_SP_CR_OFFS_CNT_SHD_OFFSET                (0xC4U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_SHD                       (0x0U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_SHD_SP_CR_OFFS_CNT_SHIFT  (3U)
#define ISP_MI_MI_SP_CR_OFFS_CNT_SHD_SP_CR_OFFS_CNT_MASK   (0x1FFFFFFU << ISP_MI_MI_SP_CR_OFFS_CNT_SHD_SP_CR_OFFS_CNT_SHIFT) /* 0x0FFFFFF8 */
/* MI_DMA_Y_PIC_START_AD */
#define ISP_MI_MI_DMA_Y_PIC_START_AD_OFFSET                (0xC8U)
#define ISP_MI_MI_DMA_Y_PIC_START_AD_DMA_Y_PIC_START_AD_SHIFT (0U)
#define ISP_MI_MI_DMA_Y_PIC_START_AD_DMA_Y_PIC_START_AD_MASK (0xFFFFFFFFU << ISP_MI_MI_DMA_Y_PIC_START_AD_DMA_Y_PIC_START_AD_SHIFT) /* 0xFFFFFFFF */
/* MI_DMA_Y_PIC_WIDTH */
#define ISP_MI_MI_DMA_Y_PIC_WIDTH_OFFSET                   (0xCCU)
#define ISP_MI_MI_DMA_Y_PIC_WIDTH_DMA_Y_PIC_WIDTH_SHIFT    (0U)
#define ISP_MI_MI_DMA_Y_PIC_WIDTH_DMA_Y_PIC_WIDTH_MASK     (0x7FFFU << ISP_MI_MI_DMA_Y_PIC_WIDTH_DMA_Y_PIC_WIDTH_SHIFT) /* 0x00007FFF */
/* MI_DMA_Y_LLENGTH */
#define ISP_MI_MI_DMA_Y_LLENGTH_OFFSET                     (0xD0U)
#define ISP_MI_MI_DMA_Y_LLENGTH_DMA_Y_LLENGTH_SHIFT        (0U)
#define ISP_MI_MI_DMA_Y_LLENGTH_DMA_Y_LLENGTH_MASK         (0x7FFFU << ISP_MI_MI_DMA_Y_LLENGTH_DMA_Y_LLENGTH_SHIFT)     /* 0x00007FFF */
/* MI_DMA_Y_PIC_SIZE */
#define ISP_MI_MI_DMA_Y_PIC_SIZE_OFFSET                    (0xD4U)
#define ISP_MI_MI_DMA_Y_PIC_SIZE_DMA_Y_PIC_SIZE_SHIFT      (0U)
#define ISP_MI_MI_DMA_Y_PIC_SIZE_DMA_Y_PIC_SIZE_MASK       (0xFFFFFFFU << ISP_MI_MI_DMA_Y_PIC_SIZE_DMA_Y_PIC_SIZE_SHIFT) /* 0x0FFFFFFF */
/* MI_DMA_CB_PIC_START_AD */
#define ISP_MI_MI_DMA_CB_PIC_START_AD_OFFSET               (0xD8U)
#define ISP_MI_MI_DMA_CB_PIC_START_AD_DMA_CB_PIC_START_AD_SHIFT (0U)
#define ISP_MI_MI_DMA_CB_PIC_START_AD_DMA_CB_PIC_START_AD_MASK (0xFFFFFFFFU << ISP_MI_MI_DMA_CB_PIC_START_AD_DMA_CB_PIC_START_AD_SHIFT) /* 0xFFFFFFFF */
/* MI_DMA_CR_PIC_START_AD */
#define ISP_MI_MI_DMA_CR_PIC_START_AD_OFFSET               (0xE8U)
#define ISP_MI_MI_DMA_CR_PIC_START_AD_DMA_CR_PIC_START_AD_SHIFT (0U)
#define ISP_MI_MI_DMA_CR_PIC_START_AD_DMA_CR_PIC_START_AD_MASK (0xFFFFFFFFU << ISP_MI_MI_DMA_CR_PIC_START_AD_DMA_CR_PIC_START_AD_SHIFT) /* 0xFFFFFFFF */
/* MI_IMSC */
#define ISP_MI_MI_IMSC_OFFSET                              (0xF8U)
#define ISP_MI_MI_IMSC_MP_FRAME_END_SHIFT                  (0U)
#define ISP_MI_MI_IMSC_MP_FRAME_END_MASK                   (0x1U << ISP_MI_MI_IMSC_MP_FRAME_END_SHIFT)                  /* 0x00000001 */
#define ISP_MI_MI_IMSC_SP_FRAME_END_SHIFT                  (1U)
#define ISP_MI_MI_IMSC_SP_FRAME_END_MASK                   (0x1U << ISP_MI_MI_IMSC_SP_FRAME_END_SHIFT)                  /* 0x00000002 */
#define ISP_MI_MI_IMSC_MBLK_LINE_SHIFT                     (2U)
#define ISP_MI_MI_IMSC_MBLK_LINE_MASK                      (0x1U << ISP_MI_MI_IMSC_MBLK_LINE_SHIFT)                     /* 0x00000004 */
#define ISP_MI_MI_IMSC_FILL_MP_Y_SHIFT                     (3U)
#define ISP_MI_MI_IMSC_FILL_MP_Y_MASK                      (0x1U << ISP_MI_MI_IMSC_FILL_MP_Y_SHIFT)                     /* 0x00000008 */
#define ISP_MI_MI_IMSC_WRAP_MP_Y_SHIFT                     (4U)
#define ISP_MI_MI_IMSC_WRAP_MP_Y_MASK                      (0x1U << ISP_MI_MI_IMSC_WRAP_MP_Y_SHIFT)                     /* 0x00000010 */
#define ISP_MI_MI_IMSC_WRAP_MP_CB_SHIFT                    (5U)
#define ISP_MI_MI_IMSC_WRAP_MP_CB_MASK                     (0x1U << ISP_MI_MI_IMSC_WRAP_MP_CB_SHIFT)                    /* 0x00000020 */
#define ISP_MI_MI_IMSC_WRAP_MP_CR_SHIFT                    (6U)
#define ISP_MI_MI_IMSC_WRAP_MP_CR_MASK                     (0x1U << ISP_MI_MI_IMSC_WRAP_MP_CR_SHIFT)                    /* 0x00000040 */
#define ISP_MI_MI_IMSC_WRAP_SP_Y_SHIFT                     (7U)
#define ISP_MI_MI_IMSC_WRAP_SP_Y_MASK                      (0x1U << ISP_MI_MI_IMSC_WRAP_SP_Y_SHIFT)                     /* 0x00000080 */
#define ISP_MI_MI_IMSC_WRAP_SP_CB_SHIFT                    (8U)
#define ISP_MI_MI_IMSC_WRAP_SP_CB_MASK                     (0x1U << ISP_MI_MI_IMSC_WRAP_SP_CB_SHIFT)                    /* 0x00000100 */
#define ISP_MI_MI_IMSC_WRAP_SP_CR_SHIFT                    (9U)
#define ISP_MI_MI_IMSC_WRAP_SP_CR_MASK                     (0x1U << ISP_MI_MI_IMSC_WRAP_SP_CR_SHIFT)                    /* 0x00000200 */
#define ISP_MI_MI_IMSC_DMA_READY_SHIFT                     (11U)
#define ISP_MI_MI_IMSC_DMA_READY_MASK                      (0x1U << ISP_MI_MI_IMSC_DMA_READY_SHIFT)                     /* 0x00000800 */
/* MI_RIS */
#define ISP_MI_MI_RIS_OFFSET                               (0xFCU)
#define ISP_MI_MI_RIS                                      (0x0U)
#define ISP_MI_MI_RIS_MP_FRAME_END_SHIFT                   (0U)
#define ISP_MI_MI_RIS_MP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_RIS_MP_FRAME_END_SHIFT)                   /* 0x00000001 */
#define ISP_MI_MI_RIS_SP_FRAME_END_SHIFT                   (1U)
#define ISP_MI_MI_RIS_SP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_RIS_SP_FRAME_END_SHIFT)                   /* 0x00000002 */
#define ISP_MI_MI_RIS_MBLK_LINE_SHIFT                      (2U)
#define ISP_MI_MI_RIS_MBLK_LINE_MASK                       (0x1U << ISP_MI_MI_RIS_MBLK_LINE_SHIFT)                      /* 0x00000004 */
#define ISP_MI_MI_RIS_FILL_MP_Y_SHIFT                      (3U)
#define ISP_MI_MI_RIS_FILL_MP_Y_MASK                       (0x1U << ISP_MI_MI_RIS_FILL_MP_Y_SHIFT)                      /* 0x00000008 */
#define ISP_MI_MI_RIS_WRAP_MP_Y_SHIFT                      (4U)
#define ISP_MI_MI_RIS_WRAP_MP_Y_MASK                       (0x1U << ISP_MI_MI_RIS_WRAP_MP_Y_SHIFT)                      /* 0x00000010 */
#define ISP_MI_MI_RIS_WRAP_MP_CB_SHIFT                     (5U)
#define ISP_MI_MI_RIS_WRAP_MP_CB_MASK                      (0x1U << ISP_MI_MI_RIS_WRAP_MP_CB_SHIFT)                     /* 0x00000020 */
#define ISP_MI_MI_RIS_WRAP_MP_CR_SHIFT                     (6U)
#define ISP_MI_MI_RIS_WRAP_MP_CR_MASK                      (0x1U << ISP_MI_MI_RIS_WRAP_MP_CR_SHIFT)                     /* 0x00000040 */
#define ISP_MI_MI_RIS_WRAP_SP_Y_SHIFT                      (7U)
#define ISP_MI_MI_RIS_WRAP_SP_Y_MASK                       (0x1U << ISP_MI_MI_RIS_WRAP_SP_Y_SHIFT)                      /* 0x00000080 */
#define ISP_MI_MI_RIS_WRAP_SP_CB_SHIFT                     (8U)
#define ISP_MI_MI_RIS_WRAP_SP_CB_MASK                      (0x1U << ISP_MI_MI_RIS_WRAP_SP_CB_SHIFT)                     /* 0x00000100 */
#define ISP_MI_MI_RIS_WRAP_SP_CR_SHIFT                     (9U)
#define ISP_MI_MI_RIS_WRAP_SP_CR_MASK                      (0x1U << ISP_MI_MI_RIS_WRAP_SP_CR_SHIFT)                     /* 0x00000200 */
#define ISP_MI_MI_RIS_DMA_READY_SHIFT                      (11U)
#define ISP_MI_MI_RIS_DMA_READY_MASK                       (0x1U << ISP_MI_MI_RIS_DMA_READY_SHIFT)                      /* 0x00000800 */
/* MI_MIS */
#define ISP_MI_MI_MIS_OFFSET                               (0x100U)
#define ISP_MI_MI_MIS                                      (0x0U)
#define ISP_MI_MI_MIS_MP_FRAME_END_SHIFT                   (0U)
#define ISP_MI_MI_MIS_MP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_MIS_MP_FRAME_END_SHIFT)                   /* 0x00000001 */
#define ISP_MI_MI_MIS_SP_FRAME_END_SHIFT                   (1U)
#define ISP_MI_MI_MIS_SP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_MIS_SP_FRAME_END_SHIFT)                   /* 0x00000002 */
#define ISP_MI_MI_MIS_MBLK_LINE_SHIFT                      (2U)
#define ISP_MI_MI_MIS_MBLK_LINE_MASK                       (0x1U << ISP_MI_MI_MIS_MBLK_LINE_SHIFT)                      /* 0x00000004 */
#define ISP_MI_MI_MIS_FILL_MP_Y_SHIFT                      (3U)
#define ISP_MI_MI_MIS_FILL_MP_Y_MASK                       (0x1U << ISP_MI_MI_MIS_FILL_MP_Y_SHIFT)                      /* 0x00000008 */
#define ISP_MI_MI_MIS_WRAP_MP_Y_SHIFT                      (4U)
#define ISP_MI_MI_MIS_WRAP_MP_Y_MASK                       (0x1U << ISP_MI_MI_MIS_WRAP_MP_Y_SHIFT)                      /* 0x00000010 */
#define ISP_MI_MI_MIS_WRAP_MP_CB_SHIFT                     (5U)
#define ISP_MI_MI_MIS_WRAP_MP_CB_MASK                      (0x1U << ISP_MI_MI_MIS_WRAP_MP_CB_SHIFT)                     /* 0x00000020 */
#define ISP_MI_MI_MIS_WRAP_MP_CR_SHIFT                     (6U)
#define ISP_MI_MI_MIS_WRAP_MP_CR_MASK                      (0x1U << ISP_MI_MI_MIS_WRAP_MP_CR_SHIFT)                     /* 0x00000040 */
#define ISP_MI_MI_MIS_WRAP_SP_Y_SHIFT                      (7U)
#define ISP_MI_MI_MIS_WRAP_SP_Y_MASK                       (0x1U << ISP_MI_MI_MIS_WRAP_SP_Y_SHIFT)                      /* 0x00000080 */
#define ISP_MI_MI_MIS_WRAP_SP_CB_SHIFT                     (8U)
#define ISP_MI_MI_MIS_WRAP_SP_CB_MASK                      (0x1U << ISP_MI_MI_MIS_WRAP_SP_CB_SHIFT)                     /* 0x00000100 */
#define ISP_MI_MI_MIS_WRAP_SP_CR_SHIFT                     (9U)
#define ISP_MI_MI_MIS_WRAP_SP_CR_MASK                      (0x1U << ISP_MI_MI_MIS_WRAP_SP_CR_SHIFT)                     /* 0x00000200 */
#define ISP_MI_MI_MIS_DMA_READY_SHIFT                      (11U)
#define ISP_MI_MI_MIS_DMA_READY_MASK                       (0x1U << ISP_MI_MI_MIS_DMA_READY_SHIFT)                      /* 0x00000800 */
/* MI_ICR */
#define ISP_MI_MI_ICR_OFFSET                               (0x104U)
#define ISP_MI_MI_ICR_MP_FRAME_END_SHIFT                   (0U)
#define ISP_MI_MI_ICR_MP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_ICR_MP_FRAME_END_SHIFT)                   /* 0x00000001 */
#define ISP_MI_MI_ICR_SP_FRAME_END_SHIFT                   (1U)
#define ISP_MI_MI_ICR_SP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_ICR_SP_FRAME_END_SHIFT)                   /* 0x00000002 */
#define ISP_MI_MI_ICR_MBLK_LINE_SHIFT                      (2U)
#define ISP_MI_MI_ICR_MBLK_LINE_MASK                       (0x1U << ISP_MI_MI_ICR_MBLK_LINE_SHIFT)                      /* 0x00000004 */
#define ISP_MI_MI_ICR_FILL_MP_Y_SHIFT                      (3U)
#define ISP_MI_MI_ICR_FILL_MP_Y_MASK                       (0x1U << ISP_MI_MI_ICR_FILL_MP_Y_SHIFT)                      /* 0x00000008 */
#define ISP_MI_MI_ICR_WRAP_MP_Y_SHIFT                      (4U)
#define ISP_MI_MI_ICR_WRAP_MP_Y_MASK                       (0x1U << ISP_MI_MI_ICR_WRAP_MP_Y_SHIFT)                      /* 0x00000010 */
#define ISP_MI_MI_ICR_WRAP_MP_CB_SHIFT                     (5U)
#define ISP_MI_MI_ICR_WRAP_MP_CB_MASK                      (0x1U << ISP_MI_MI_ICR_WRAP_MP_CB_SHIFT)                     /* 0x00000020 */
#define ISP_MI_MI_ICR_WRAP_MP_CR_SHIFT                     (6U)
#define ISP_MI_MI_ICR_WRAP_MP_CR_MASK                      (0x1U << ISP_MI_MI_ICR_WRAP_MP_CR_SHIFT)                     /* 0x00000040 */
#define ISP_MI_MI_ICR_WRAP_SP_Y_SHIFT                      (7U)
#define ISP_MI_MI_ICR_WRAP_SP_Y_MASK                       (0x1U << ISP_MI_MI_ICR_WRAP_SP_Y_SHIFT)                      /* 0x00000080 */
#define ISP_MI_MI_ICR_WRAP_SP_CB_SHIFT                     (8U)
#define ISP_MI_MI_ICR_WRAP_SP_CB_MASK                      (0x1U << ISP_MI_MI_ICR_WRAP_SP_CB_SHIFT)                     /* 0x00000100 */
#define ISP_MI_MI_ICR_WRAP_SP_CR_SHIFT                     (9U)
#define ISP_MI_MI_ICR_WRAP_SP_CR_MASK                      (0x1U << ISP_MI_MI_ICR_WRAP_SP_CR_SHIFT)                     /* 0x00000200 */
#define ISP_MI_MI_ICR_DMA_READY_SHIFT                      (11U)
#define ISP_MI_MI_ICR_DMA_READY_MASK                       (0x1U << ISP_MI_MI_ICR_DMA_READY_SHIFT)                      /* 0x00000800 */
/* MI_ISR */
#define ISP_MI_MI_ISR_OFFSET                               (0x108U)
#define ISP_MI_MI_ISR_MP_FRAME_END_SHIFT                   (0U)
#define ISP_MI_MI_ISR_MP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_ISR_MP_FRAME_END_SHIFT)                   /* 0x00000001 */
#define ISP_MI_MI_ISR_SP_FRAME_END_SHIFT                   (1U)
#define ISP_MI_MI_ISR_SP_FRAME_END_MASK                    (0x1U << ISP_MI_MI_ISR_SP_FRAME_END_SHIFT)                   /* 0x00000002 */
#define ISP_MI_MI_ISR_MBLK_LINE_SHIFT                      (2U)
#define ISP_MI_MI_ISR_MBLK_LINE_MASK                       (0x1U << ISP_MI_MI_ISR_MBLK_LINE_SHIFT)                      /* 0x00000004 */
#define ISP_MI_MI_ISR_FILL_MP_Y_SHIFT                      (3U)
#define ISP_MI_MI_ISR_FILL_MP_Y_MASK                       (0x1U << ISP_MI_MI_ISR_FILL_MP_Y_SHIFT)                      /* 0x00000008 */
#define ISP_MI_MI_ISR_WRAP_MP_Y_SHIFT                      (4U)
#define ISP_MI_MI_ISR_WRAP_MP_Y_MASK                       (0x1U << ISP_MI_MI_ISR_WRAP_MP_Y_SHIFT)                      /* 0x00000010 */
#define ISP_MI_MI_ISR_WRAP_MP_CB_SHIFT                     (5U)
#define ISP_MI_MI_ISR_WRAP_MP_CB_MASK                      (0x1U << ISP_MI_MI_ISR_WRAP_MP_CB_SHIFT)                     /* 0x00000020 */
#define ISP_MI_MI_ISR_WRAP_MP_CR_SHIFT                     (6U)
#define ISP_MI_MI_ISR_WRAP_MP_CR_MASK                      (0x1U << ISP_MI_MI_ISR_WRAP_MP_CR_SHIFT)                     /* 0x00000040 */
#define ISP_MI_MI_ISR_WRAP_SP_Y_SHIFT                      (7U)
#define ISP_MI_MI_ISR_WRAP_SP_Y_MASK                       (0x1U << ISP_MI_MI_ISR_WRAP_SP_Y_SHIFT)                      /* 0x00000080 */
#define ISP_MI_MI_ISR_WRAP_SP_CB_SHIFT                     (8U)
#define ISP_MI_MI_ISR_WRAP_SP_CB_MASK                      (0x1U << ISP_MI_MI_ISR_WRAP_SP_CB_SHIFT)                     /* 0x00000100 */
#define ISP_MI_MI_ISR_WRAP_SP_CR_SHIFT                     (9U)
#define ISP_MI_MI_ISR_WRAP_SP_CR_MASK                      (0x1U << ISP_MI_MI_ISR_WRAP_SP_CR_SHIFT)                     /* 0x00000200 */
#define ISP_MI_MI_ISR_DMA_READY_SHIFT                      (11U)
#define ISP_MI_MI_ISR_DMA_READY_MASK                       (0x1U << ISP_MI_MI_ISR_DMA_READY_SHIFT)                      /* 0x00000800 */
/* MI_STATUS */
#define ISP_MI_MI_STATUS_OFFSET                            (0x10CU)
#define ISP_MI_MI_STATUS                                   (0x0U)
#define ISP_MI_MI_STATUS_MP_Y_FIFO_FULL_SHIFT              (0U)
#define ISP_MI_MI_STATUS_MP_Y_FIFO_FULL_MASK               (0x1U << ISP_MI_MI_STATUS_MP_Y_FIFO_FULL_SHIFT)              /* 0x00000001 */
#define ISP_MI_MI_STATUS_MP_CB_FIFO_FULL_SHIFT             (1U)
#define ISP_MI_MI_STATUS_MP_CB_FIFO_FULL_MASK              (0x1U << ISP_MI_MI_STATUS_MP_CB_FIFO_FULL_SHIFT)             /* 0x00000002 */
#define ISP_MI_MI_STATUS_MP_CR_FIFO_FULL_SHIFT             (2U)
#define ISP_MI_MI_STATUS_MP_CR_FIFO_FULL_MASK              (0x1U << ISP_MI_MI_STATUS_MP_CR_FIFO_FULL_SHIFT)             /* 0x00000004 */
#define ISP_MI_MI_STATUS_SP_Y_FIFO_FULL_SHIFT              (4U)
#define ISP_MI_MI_STATUS_SP_Y_FIFO_FULL_MASK               (0x1U << ISP_MI_MI_STATUS_SP_Y_FIFO_FULL_SHIFT)              /* 0x00000010 */
#define ISP_MI_MI_STATUS_SP_CB_FIFO_FULL_SHIFT             (5U)
#define ISP_MI_MI_STATUS_SP_CB_FIFO_FULL_MASK              (0x1U << ISP_MI_MI_STATUS_SP_CB_FIFO_FULL_SHIFT)             /* 0x00000020 */
#define ISP_MI_MI_STATUS_SP_CR_FIFO_FULL_SHIFT             (6U)
#define ISP_MI_MI_STATUS_SP_CR_FIFO_FULL_MASK              (0x1U << ISP_MI_MI_STATUS_SP_CR_FIFO_FULL_SHIFT)             /* 0x00000040 */
/* MI_STATUS_CLR */
#define ISP_MI_MI_STATUS_CLR_OFFSET                        (0x110U)
#define ISP_MI_MI_STATUS_CLR_MP_Y_FIFO_FULL_SHIFT          (0U)
#define ISP_MI_MI_STATUS_CLR_MP_Y_FIFO_FULL_MASK           (0x1U << ISP_MI_MI_STATUS_CLR_MP_Y_FIFO_FULL_SHIFT)          /* 0x00000001 */
#define ISP_MI_MI_STATUS_CLR_MP_CB_FIFO_FULL_SHIFT         (1U)
#define ISP_MI_MI_STATUS_CLR_MP_CB_FIFO_FULL_MASK          (0x1U << ISP_MI_MI_STATUS_CLR_MP_CB_FIFO_FULL_SHIFT)         /* 0x00000002 */
#define ISP_MI_MI_STATUS_CLR_MP_CR_FIFO_FULL_SHIFT         (2U)
#define ISP_MI_MI_STATUS_CLR_MP_CR_FIFO_FULL_MASK          (0x1U << ISP_MI_MI_STATUS_CLR_MP_CR_FIFO_FULL_SHIFT)         /* 0x00000004 */
#define ISP_MI_MI_STATUS_CLR_SP_Y_FIFO_FULL_SHIFT          (4U)
#define ISP_MI_MI_STATUS_CLR_SP_Y_FIFO_FULL_MASK           (0x1U << ISP_MI_MI_STATUS_CLR_SP_Y_FIFO_FULL_SHIFT)          /* 0x00000010 */
#define ISP_MI_MI_STATUS_CLR_SP_CB_FIFO_FULL_SHIFT         (5U)
#define ISP_MI_MI_STATUS_CLR_SP_CB_FIFO_FULL_MASK          (0x1U << ISP_MI_MI_STATUS_CLR_SP_CB_FIFO_FULL_SHIFT)         /* 0x00000020 */
#define ISP_MI_MI_STATUS_CLR_SP_CR_FIFO_FULL_SHIFT         (6U)
#define ISP_MI_MI_STATUS_CLR_SP_CR_FIFO_FULL_MASK          (0x1U << ISP_MI_MI_STATUS_CLR_SP_CR_FIFO_FULL_SHIFT)         /* 0x00000040 */
/* MI_SP_Y_PIC_WIDTH */
#define ISP_MI_MI_SP_Y_PIC_WIDTH_OFFSET                    (0x114U)
#define ISP_MI_MI_SP_Y_PIC_WIDTH_SP_Y_PIC_WIDTH_SHIFT      (0U)
#define ISP_MI_MI_SP_Y_PIC_WIDTH_SP_Y_PIC_WIDTH_MASK       (0x7FFFU << ISP_MI_MI_SP_Y_PIC_WIDTH_SP_Y_PIC_WIDTH_SHIFT)   /* 0x00007FFF */
/* MI_SP_Y_PIC_HEIGHT */
#define ISP_MI_MI_SP_Y_PIC_HEIGHT_OFFSET                   (0x118U)
#define ISP_MI_MI_SP_Y_PIC_HEIGHT_SP_Y_PIC_HEIGHT_SHIFT    (0U)
#define ISP_MI_MI_SP_Y_PIC_HEIGHT_SP_Y_PIC_HEIGHT_MASK     (0x7FFFU << ISP_MI_MI_SP_Y_PIC_HEIGHT_SP_Y_PIC_HEIGHT_SHIFT) /* 0x00007FFF */
/* MI_SP_Y_PIC_SIZE */
#define ISP_MI_MI_SP_Y_PIC_SIZE_OFFSET                     (0x11CU)
#define ISP_MI_MI_SP_Y_PIC_SIZE_SP_Y_PIC_SIZE_SHIFT        (0U)
#define ISP_MI_MI_SP_Y_PIC_SIZE_SP_Y_PIC_SIZE_MASK         (0x1FFFFFFU << ISP_MI_MI_SP_Y_PIC_SIZE_SP_Y_PIC_SIZE_SHIFT)  /* 0x01FFFFFF */
/* MI_DMA_CTRL */
#define ISP_MI_MI_DMA_CTRL_OFFSET                          (0x120U)
#define ISP_MI_MI_DMA_CTRL_DMA_BURST_LEN_LUM_SHIFT         (0U)
#define ISP_MI_MI_DMA_CTRL_DMA_BURST_LEN_LUM_MASK          (0x3U << ISP_MI_MI_DMA_CTRL_DMA_BURST_LEN_LUM_SHIFT)         /* 0x00000003 */
#define ISP_MI_MI_DMA_CTRL_DMA_BURST_LEN_CHROM_SHIFT       (2U)
#define ISP_MI_MI_DMA_CTRL_DMA_BURST_LEN_CHROM_MASK        (0x3U << ISP_MI_MI_DMA_CTRL_DMA_BURST_LEN_CHROM_SHIFT)       /* 0x0000000C */
#define ISP_MI_MI_DMA_CTRL_DMA_READ_FORMAT_SHIFT           (4U)
#define ISP_MI_MI_DMA_CTRL_DMA_READ_FORMAT_MASK            (0x3U << ISP_MI_MI_DMA_CTRL_DMA_READ_FORMAT_SHIFT)           /* 0x00000030 */
#define ISP_MI_MI_DMA_CTRL_DMA_INOUT_FORMAT_SHIFT          (6U)
#define ISP_MI_MI_DMA_CTRL_DMA_INOUT_FORMAT_MASK           (0x3U << ISP_MI_MI_DMA_CTRL_DMA_INOUT_FORMAT_SHIFT)          /* 0x000000C0 */
#define ISP_MI_MI_DMA_CTRL_DMA_BYTE_SWAP_SHIFT             (8U)
#define ISP_MI_MI_DMA_CTRL_DMA_BYTE_SWAP_MASK              (0x1U << ISP_MI_MI_DMA_CTRL_DMA_BYTE_SWAP_SHIFT)             /* 0x00000100 */
#define ISP_MI_MI_DMA_CTRL_DMA_CONTINUOUS_EN_SHIFT         (9U)
#define ISP_MI_MI_DMA_CTRL_DMA_CONTINUOUS_EN_MASK          (0x1U << ISP_MI_MI_DMA_CTRL_DMA_CONTINUOUS_EN_SHIFT)         /* 0x00000200 */
#define ISP_MI_MI_DMA_CTRL_DMA_FRAME_END_DISABLE_SHIFT     (10U)
#define ISP_MI_MI_DMA_CTRL_DMA_FRAME_END_DISABLE_MASK      (0x1U << ISP_MI_MI_DMA_CTRL_DMA_FRAME_END_DISABLE_SHIFT)     /* 0x00000400 */
#define ISP_MI_MI_DMA_CTRL_DMA_RGB_FORMAT_SHIFT            (12U)
#define ISP_MI_MI_DMA_CTRL_DMA_RGB_FORMAT_MASK             (0x3U << ISP_MI_MI_DMA_CTRL_DMA_RGB_FORMAT_SHIFT)            /* 0x00003000 */
/* MI_DMA_START */
#define ISP_MI_MI_DMA_START_OFFSET                         (0x124U)
#define ISP_MI_MI_DMA_START_DMA_START_SHIFT                (0U)
#define ISP_MI_MI_DMA_START_DMA_START_MASK                 (0x1U << ISP_MI_MI_DMA_START_DMA_START_SHIFT)                /* 0x00000001 */
/* MI_DMA_STATUS */
#define ISP_MI_MI_DMA_STATUS_OFFSET                        (0x128U)
#define ISP_MI_MI_DMA_STATUS                               (0x0U)
#define ISP_MI_MI_DMA_STATUS_DMA_ACTIVE_SHIFT              (0U)
#define ISP_MI_MI_DMA_STATUS_DMA_ACTIVE_MASK               (0x1U << ISP_MI_MI_DMA_STATUS_DMA_ACTIVE_SHIFT)              /* 0x00000001 */
/* MI_PIXEL_CNT */
#define ISP_MI_MI_PIXEL_CNT_OFFSET                         (0x12CU)
#define ISP_MI_MI_PIXEL_CNT                                (0x0U)
#define ISP_MI_MI_PIXEL_CNT_PIX_CNT_SHIFT                  (0U)
#define ISP_MI_MI_PIXEL_CNT_PIX_CNT_MASK                   (0xFFFFFFFU << ISP_MI_MI_PIXEL_CNT_PIX_CNT_SHIFT)            /* 0x0FFFFFFF */
/* MI_MP_Y_BASE_AD_INIT2 */
#define ISP_MI_MI_MP_Y_BASE_AD_INIT2_OFFSET                (0x130U)
#define ISP_MI_MI_MP_Y_BASE_AD_INIT2_MP_Y_BASE_AD_INIT2_SHIFT (3U)
#define ISP_MI_MI_MP_Y_BASE_AD_INIT2_MP_Y_BASE_AD_INIT2_MASK (0x1FFFFFFFU << ISP_MI_MI_MP_Y_BASE_AD_INIT2_MP_Y_BASE_AD_INIT2_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_CB_BASE_AD_INIT2 */
#define ISP_MI_MI_MP_CB_BASE_AD_INIT2_OFFSET               (0x134U)
#define ISP_MI_MI_MP_CB_BASE_AD_INIT2_MP_CB_BASE_AD_INIT2_SHIFT (3U)
#define ISP_MI_MI_MP_CB_BASE_AD_INIT2_MP_CB_BASE_AD_INIT2_MASK (0x1FFFFFFFU << ISP_MI_MI_MP_CB_BASE_AD_INIT2_MP_CB_BASE_AD_INIT2_SHIFT) /* 0xFFFFFFF8 */
/* MI_MP_CR_BASE_AD_INIT2 */
#define ISP_MI_MI_MP_CR_BASE_AD_INIT2_OFFSET               (0x138U)
#define ISP_MI_MI_MP_CR_BASE_AD_INIT2_MP_CR_BASE_AD_INIT2_SHIFT (3U)
#define ISP_MI_MI_MP_CR_BASE_AD_INIT2_MP_CR_BASE_AD_INIT2_MASK (0x1FFFFFFFU << ISP_MI_MI_MP_CR_BASE_AD_INIT2_MP_CR_BASE_AD_INIT2_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_Y_BASE_AD_INIT2 */
#define ISP_MI_MI_SP_Y_BASE_AD_INIT2_OFFSET                (0x13CU)
#define ISP_MI_MI_SP_Y_BASE_AD_INIT2_SP_Y_BASE_AD_INIT2_SHIFT (3U)
#define ISP_MI_MI_SP_Y_BASE_AD_INIT2_SP_Y_BASE_AD_INIT2_MASK (0x1FFFFFFFU << ISP_MI_MI_SP_Y_BASE_AD_INIT2_SP_Y_BASE_AD_INIT2_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_CB_BASE_AD_INIT2 */
#define ISP_MI_MI_SP_CB_BASE_AD_INIT2_OFFSET               (0x140U)
#define ISP_MI_MI_SP_CB_BASE_AD_INIT2_SP_CB_BASE_AD_INIT2_SHIFT (3U)
#define ISP_MI_MI_SP_CB_BASE_AD_INIT2_SP_CB_BASE_AD_INIT2_MASK (0x1FFFFFFFU << ISP_MI_MI_SP_CB_BASE_AD_INIT2_SP_CB_BASE_AD_INIT2_SHIFT) /* 0xFFFFFFF8 */
/* MI_SP_CR_BASE_AD_INIT2 */
#define ISP_MI_MI_SP_CR_BASE_AD_INIT2_OFFSET               (0x144U)
#define ISP_MI_MI_SP_CR_BASE_AD_INIT2_SP_CR_BASE_AD_INIT2_SHIFT (3U)
#define ISP_MI_MI_SP_CR_BASE_AD_INIT2_SP_CR_BASE_AD_INIT2_MASK (0x1FFFFFFFU << ISP_MI_MI_SP_CR_BASE_AD_INIT2_SP_CR_BASE_AD_INIT2_SHIFT) /* 0xFFFFFFF8 */
/* MI_XTD_FORMAT_CTRL */
#define ISP_MI_MI_XTD_FORMAT_CTRL_OFFSET                   (0x148U)
#define ISP_MI_MI_XTD_FORMAT_CTRL_NV21_MAIN_SHIFT          (0U)
#define ISP_MI_MI_XTD_FORMAT_CTRL_NV21_MAIN_MASK           (0x1U << ISP_MI_MI_XTD_FORMAT_CTRL_NV21_MAIN_SHIFT)          /* 0x00000001 */
#define ISP_MI_MI_XTD_FORMAT_CTRL_NV21_SELF_SHIFT          (1U)
#define ISP_MI_MI_XTD_FORMAT_CTRL_NV21_SELF_MASK           (0x1U << ISP_MI_MI_XTD_FORMAT_CTRL_NV21_SELF_SHIFT)          /* 0x00000002 */
#define ISP_MI_MI_XTD_FORMAT_CTRL_NV21_DMA_READ_SHIFT      (2U)
#define ISP_MI_MI_XTD_FORMAT_CTRL_NV21_DMA_READ_MASK       (0x1U << ISP_MI_MI_XTD_FORMAT_CTRL_NV21_DMA_READ_SHIFT)      /* 0x00000004 */
/****************************************ISP_MIPI****************************************/
/* MIPI_CTRL */
#define ISP_MIPI_MIPI_CTRL_OFFSET                          (0x0U)
#define ISP_MIPI_MIPI_CTRL_OUTPUT_ENA_SHIFT                (0U)
#define ISP_MIPI_MIPI_CTRL_OUTPUT_ENA_MASK                 (0x1U << ISP_MIPI_MIPI_CTRL_OUTPUT_ENA_SHIFT)                /* 0x00000001 */
#define ISP_MIPI_MIPI_CTRL_FLUSH_FIFO_SHIFT                (1U)
#define ISP_MIPI_MIPI_CTRL_FLUSH_FIFO_MASK                 (0x1U << ISP_MIPI_MIPI_CTRL_FLUSH_FIFO_SHIFT)                /* 0x00000002 */
#define ISP_MIPI_MIPI_CTRL_SHUTDOWN_LANE_SHIFT             (8U)
#define ISP_MIPI_MIPI_CTRL_SHUTDOWN_LANE_MASK              (0xFU << ISP_MIPI_MIPI_CTRL_SHUTDOWN_LANE_SHIFT)             /* 0x00000F00 */
#define ISP_MIPI_MIPI_CTRL_NUM_LANES_SHIFT                 (12U)
#define ISP_MIPI_MIPI_CTRL_NUM_LANES_MASK                  (0x3U << ISP_MIPI_MIPI_CTRL_NUM_LANES_SHIFT)                 /* 0x00003000 */
#define ISP_MIPI_MIPI_CTRL_ERR_SOT_HS_SKIP_SHIFT           (16U)
#define ISP_MIPI_MIPI_CTRL_ERR_SOT_HS_SKIP_MASK            (0x1U << ISP_MIPI_MIPI_CTRL_ERR_SOT_HS_SKIP_SHIFT)           /* 0x00010000 */
#define ISP_MIPI_MIPI_CTRL_ERR_SOT_SYNC_HS_SKIP_SHIFT      (17U)
#define ISP_MIPI_MIPI_CTRL_ERR_SOT_SYNC_HS_SKIP_MASK       (0x1U << ISP_MIPI_MIPI_CTRL_ERR_SOT_SYNC_HS_SKIP_SHIFT)      /* 0x00020000 */
#define ISP_MIPI_MIPI_CTRL_S_ENABLE_CLK_SHIFT              (18U)
#define ISP_MIPI_MIPI_CTRL_S_ENABLE_CLK_MASK               (0x1U << ISP_MIPI_MIPI_CTRL_S_ENABLE_CLK_SHIFT)              /* 0x00040000 */
/* MIPI_STATUS */
#define ISP_MIPI_MIPI_STATUS_OFFSET                        (0x4U)
#define ISP_MIPI_MIPI_STATUS                               (0x0U)
#define ISP_MIPI_MIPI_STATUS_ADD_DATA_AVAIL_SHIFT          (0U)
#define ISP_MIPI_MIPI_STATUS_ADD_DATA_AVAIL_MASK           (0x1U << ISP_MIPI_MIPI_STATUS_ADD_DATA_AVAIL_SHIFT)          /* 0x00000001 */
#define ISP_MIPI_MIPI_STATUS_STOPSTATE_SHIFT               (8U)
#define ISP_MIPI_MIPI_STATUS_STOPSTATE_MASK                (0xFU << ISP_MIPI_MIPI_STATUS_STOPSTATE_SHIFT)               /* 0x00000F00 */
#define ISP_MIPI_MIPI_STATUS_S_STOPSTATE_CLK_SHIFT         (12U)
#define ISP_MIPI_MIPI_STATUS_S_STOPSTATE_CLK_MASK          (0x1U << ISP_MIPI_MIPI_STATUS_S_STOPSTATE_CLK_SHIFT)         /* 0x00001000 */
#define ISP_MIPI_MIPI_STATUS_S_ULP_ACTIVE_NOT_CLK_SHIFT    (13U)
#define ISP_MIPI_MIPI_STATUS_S_ULP_ACTIVE_NOT_CLK_MASK     (0x1U << ISP_MIPI_MIPI_STATUS_S_ULP_ACTIVE_NOT_CLK_SHIFT)    /* 0x00002000 */
/* MIPI_IMSC */
#define ISP_MIPI_MIPI_IMSC_OFFSET                          (0x8U)
#define ISP_MIPI_MIPI_IMSC_IMSC_SYNC_FIFO_OVFLW_SHIFT      (0U)
#define ISP_MIPI_MIPI_IMSC_IMSC_SYNC_FIFO_OVFLW_MASK       (0xFU << ISP_MIPI_MIPI_IMSC_IMSC_SYNC_FIFO_OVFLW_SHIFT)      /* 0x0000000F */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_SOT_SHIFT              (4U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_SOT_MASK               (0xFU << ISP_MIPI_MIPI_IMSC_IMSC_ERR_SOT_SHIFT)              /* 0x000000F0 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_SOT_SYNC_SHIFT         (8U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_SOT_SYNC_MASK          (0xFU << ISP_MIPI_MIPI_IMSC_IMSC_ERR_SOT_SYNC_SHIFT)         /* 0x00000F00 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_EOT_SYNC_SHIFT         (12U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_EOT_SYNC_MASK          (0xFU << ISP_MIPI_MIPI_IMSC_IMSC_ERR_EOT_SYNC_SHIFT)         /* 0x0000F000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_CONTROL_SHIFT          (16U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_CONTROL_MASK           (0xFU << ISP_MIPI_MIPI_IMSC_IMSC_ERR_CONTROL_SHIFT)          /* 0x000F0000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_PROTOCOL_SHIFT         (20U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_PROTOCOL_MASK          (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_ERR_PROTOCOL_SHIFT)         /* 0x00100000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_ECC2_SHIFT             (21U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_ECC2_MASK              (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_ERR_ECC2_SHIFT)             /* 0x00200000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_ECC1_SHIFT             (22U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_ECC1_MASK              (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_ERR_ECC1_SHIFT)             /* 0x00400000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_CS_SHIFT               (23U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ERR_CS_MASK                (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_ERR_CS_SHIFT)               /* 0x00800000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_FRAME_END_SHIFT            (24U)
#define ISP_MIPI_MIPI_IMSC_IMSC_FRAME_END_MASK             (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_FRAME_END_SHIFT)            /* 0x01000000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ADD_DATA_OVFLW_SHIFT       (25U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ADD_DATA_OVFLW_MASK        (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_ADD_DATA_OVFLW_SHIFT)       /* 0x02000000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_ADD_DATA_FILL_LEVEL_SHIFT  (26U)
#define ISP_MIPI_MIPI_IMSC_IMSC_ADD_DATA_FILL_LEVEL_MASK   (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_ADD_DATA_FILL_LEVEL_SHIFT)  /* 0x04000000 */
#define ISP_MIPI_MIPI_IMSC_IMSC_GEN_SHORT_PACK_SHIFT       (27U)
#define ISP_MIPI_MIPI_IMSC_IMSC_GEN_SHORT_PACK_MASK        (0x1U << ISP_MIPI_MIPI_IMSC_IMSC_GEN_SHORT_PACK_SHIFT)       /* 0x08000000 */
/* MIPI_RIS */
#define ISP_MIPI_MIPI_RIS_OFFSET                           (0xCU)
#define ISP_MIPI_MIPI_RIS                                  (0x0U)
#define ISP_MIPI_MIPI_RIS_RIS_SYNC_FIFO_OVFLW_SHIFT        (0U)
#define ISP_MIPI_MIPI_RIS_RIS_SYNC_FIFO_OVFLW_MASK         (0xFU << ISP_MIPI_MIPI_RIS_RIS_SYNC_FIFO_OVFLW_SHIFT)        /* 0x0000000F */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_SOT_SHIFT                (4U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_SOT_MASK                 (0xFU << ISP_MIPI_MIPI_RIS_RIS_ERR_SOT_SHIFT)                /* 0x000000F0 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_SOT_SYNC_SHIFT           (8U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_SOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_RIS_RIS_ERR_SOT_SYNC_SHIFT)           /* 0x00000F00 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_EOT_SYNC_SHIFT           (12U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_EOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_RIS_RIS_ERR_EOT_SYNC_SHIFT)           /* 0x0000F000 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_CONTROL_SHIFT            (16U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_CONTROL_MASK             (0xFU << ISP_MIPI_MIPI_RIS_RIS_ERR_CONTROL_SHIFT)            /* 0x000F0000 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_PROTOCOL_SHIFT           (20U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_PROTOCOL_MASK            (0x1U << ISP_MIPI_MIPI_RIS_RIS_ERR_PROTOCOL_SHIFT)           /* 0x00100000 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_ECC2_SHIFT               (21U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_ECC2_MASK                (0x1U << ISP_MIPI_MIPI_RIS_RIS_ERR_ECC2_SHIFT)               /* 0x00200000 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_ECC1_SHIFT               (22U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_ECC1_MASK                (0x1U << ISP_MIPI_MIPI_RIS_RIS_ERR_ECC1_SHIFT)               /* 0x00400000 */
#define ISP_MIPI_MIPI_RIS_RIS_ERR_CS_SHIFT                 (23U)
#define ISP_MIPI_MIPI_RIS_RIS_ERR_CS_MASK                  (0x1U << ISP_MIPI_MIPI_RIS_RIS_ERR_CS_SHIFT)                 /* 0x00800000 */
#define ISP_MIPI_MIPI_RIS_RIS_FRAME_END_SHIFT              (24U)
#define ISP_MIPI_MIPI_RIS_RIS_FRAME_END_MASK               (0x1U << ISP_MIPI_MIPI_RIS_RIS_FRAME_END_SHIFT)              /* 0x01000000 */
#define ISP_MIPI_MIPI_RIS_RIS_ADD_DATA_OVFLW_SHIFT         (25U)
#define ISP_MIPI_MIPI_RIS_RIS_ADD_DATA_OVFLW_MASK          (0x1U << ISP_MIPI_MIPI_RIS_RIS_ADD_DATA_OVFLW_SHIFT)         /* 0x02000000 */
#define ISP_MIPI_MIPI_RIS_RIS_ADD_DATA_FILL_LEVEL_SHIFT    (26U)
#define ISP_MIPI_MIPI_RIS_RIS_ADD_DATA_FILL_LEVEL_MASK     (0x1U << ISP_MIPI_MIPI_RIS_RIS_ADD_DATA_FILL_LEVEL_SHIFT)    /* 0x04000000 */
#define ISP_MIPI_MIPI_RIS_RIS_GEN_SHORT_PACK_SHIFT         (27U)
#define ISP_MIPI_MIPI_RIS_RIS_GEN_SHORT_PACK_MASK          (0x1U << ISP_MIPI_MIPI_RIS_RIS_GEN_SHORT_PACK_SHIFT)         /* 0x08000000 */
/* MIPI_MIS */
#define ISP_MIPI_MIPI_MIS_OFFSET                           (0x10U)
#define ISP_MIPI_MIPI_MIS                                  (0x0U)
#define ISP_MIPI_MIPI_MIS_MIS_SYNC_FIFO_OVFLW_SHIFT        (0U)
#define ISP_MIPI_MIPI_MIS_MIS_SYNC_FIFO_OVFLW_MASK         (0xFU << ISP_MIPI_MIPI_MIS_MIS_SYNC_FIFO_OVFLW_SHIFT)        /* 0x0000000F */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_SOT_SHIFT                (4U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_SOT_MASK                 (0xFU << ISP_MIPI_MIPI_MIS_MIS_ERR_SOT_SHIFT)                /* 0x000000F0 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_SOT_SYNC_SHIFT           (8U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_SOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_MIS_MIS_ERR_SOT_SYNC_SHIFT)           /* 0x00000F00 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_EOT_SYNC_SHIFT           (12U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_EOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_MIS_MIS_ERR_EOT_SYNC_SHIFT)           /* 0x0000F000 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_CONTROL_SHIFT            (16U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_CONTROL_MASK             (0xFU << ISP_MIPI_MIPI_MIS_MIS_ERR_CONTROL_SHIFT)            /* 0x000F0000 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_PROTOCOL_SHIFT           (20U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_PROTOCOL_MASK            (0x1U << ISP_MIPI_MIPI_MIS_MIS_ERR_PROTOCOL_SHIFT)           /* 0x00100000 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_ECC2_SHIFT               (21U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_ECC2_MASK                (0x1U << ISP_MIPI_MIPI_MIS_MIS_ERR_ECC2_SHIFT)               /* 0x00200000 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_ECC1_SHIFT               (22U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_ECC1_MASK                (0x1U << ISP_MIPI_MIPI_MIS_MIS_ERR_ECC1_SHIFT)               /* 0x00400000 */
#define ISP_MIPI_MIPI_MIS_MIS_ERR_CS_SHIFT                 (23U)
#define ISP_MIPI_MIPI_MIS_MIS_ERR_CS_MASK                  (0x1U << ISP_MIPI_MIPI_MIS_MIS_ERR_CS_SHIFT)                 /* 0x00800000 */
#define ISP_MIPI_MIPI_MIS_MIS_FRAME_END_SHIFT              (24U)
#define ISP_MIPI_MIPI_MIS_MIS_FRAME_END_MASK               (0x1U << ISP_MIPI_MIPI_MIS_MIS_FRAME_END_SHIFT)              /* 0x01000000 */
#define ISP_MIPI_MIPI_MIS_MIS_ADD_DATA_OVFLW_SHIFT         (25U)
#define ISP_MIPI_MIPI_MIS_MIS_ADD_DATA_OVFLW_MASK          (0x1U << ISP_MIPI_MIPI_MIS_MIS_ADD_DATA_OVFLW_SHIFT)         /* 0x02000000 */
#define ISP_MIPI_MIPI_MIS_MIS_ADD_DATA_FILL_LEVEL_SHIFT    (26U)
#define ISP_MIPI_MIPI_MIS_MIS_ADD_DATA_FILL_LEVEL_MASK     (0x1U << ISP_MIPI_MIPI_MIS_MIS_ADD_DATA_FILL_LEVEL_SHIFT)    /* 0x04000000 */
#define ISP_MIPI_MIPI_MIS_MIS_GEN_SHORT_PACK_SHIFT         (27U)
#define ISP_MIPI_MIPI_MIS_MIS_GEN_SHORT_PACK_MASK          (0x1U << ISP_MIPI_MIPI_MIS_MIS_GEN_SHORT_PACK_SHIFT)         /* 0x08000000 */
/* MIPI_ICR */
#define ISP_MIPI_MIPI_ICR_OFFSET                           (0x14U)
#define ISP_MIPI_MIPI_ICR_ICR_SYNC_FIFO_OVFLW_SHIFT        (0U)
#define ISP_MIPI_MIPI_ICR_ICR_SYNC_FIFO_OVFLW_MASK         (0xFU << ISP_MIPI_MIPI_ICR_ICR_SYNC_FIFO_OVFLW_SHIFT)        /* 0x0000000F */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_SOT_SHIFT                (4U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_SOT_MASK                 (0xFU << ISP_MIPI_MIPI_ICR_ICR_ERR_SOT_SHIFT)                /* 0x000000F0 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_SOT_SYNC_SHIFT           (8U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_SOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_ICR_ICR_ERR_SOT_SYNC_SHIFT)           /* 0x00000F00 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_EOT_SYNC_SHIFT           (12U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_EOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_ICR_ICR_ERR_EOT_SYNC_SHIFT)           /* 0x0000F000 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_CONTROL_SHIFT            (16U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_CONTROL_MASK             (0xFU << ISP_MIPI_MIPI_ICR_ICR_ERR_CONTROL_SHIFT)            /* 0x000F0000 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_PROTOCOL_SHIFT           (20U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_PROTOCOL_MASK            (0x1U << ISP_MIPI_MIPI_ICR_ICR_ERR_PROTOCOL_SHIFT)           /* 0x00100000 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_ECC2_SHIFT               (21U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_ECC2_MASK                (0x1U << ISP_MIPI_MIPI_ICR_ICR_ERR_ECC2_SHIFT)               /* 0x00200000 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_ECC1_SHIFT               (22U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_ECC1_MASK                (0x1U << ISP_MIPI_MIPI_ICR_ICR_ERR_ECC1_SHIFT)               /* 0x00400000 */
#define ISP_MIPI_MIPI_ICR_ICR_ERR_CS_SHIFT                 (23U)
#define ISP_MIPI_MIPI_ICR_ICR_ERR_CS_MASK                  (0x1U << ISP_MIPI_MIPI_ICR_ICR_ERR_CS_SHIFT)                 /* 0x00800000 */
#define ISP_MIPI_MIPI_ICR_ICR_FRAME_END_SHIFT              (24U)
#define ISP_MIPI_MIPI_ICR_ICR_FRAME_END_MASK               (0x1U << ISP_MIPI_MIPI_ICR_ICR_FRAME_END_SHIFT)              /* 0x01000000 */
#define ISP_MIPI_MIPI_ICR_ICR_ADD_DATA_OVFLW_SHIFT         (25U)
#define ISP_MIPI_MIPI_ICR_ICR_ADD_DATA_OVFLW_MASK          (0x1U << ISP_MIPI_MIPI_ICR_ICR_ADD_DATA_OVFLW_SHIFT)         /* 0x02000000 */
#define ISP_MIPI_MIPI_ICR_ICR_ADD_DATA_FILL_LEVEL_SHIFT    (26U)
#define ISP_MIPI_MIPI_ICR_ICR_ADD_DATA_FILL_LEVEL_MASK     (0x1U << ISP_MIPI_MIPI_ICR_ICR_ADD_DATA_FILL_LEVEL_SHIFT)    /* 0x04000000 */
#define ISP_MIPI_MIPI_ICR_ICR_GEN_SHORT_PACK_SHIFT         (27U)
#define ISP_MIPI_MIPI_ICR_ICR_GEN_SHORT_PACK_MASK          (0x1U << ISP_MIPI_MIPI_ICR_ICR_GEN_SHORT_PACK_SHIFT)         /* 0x08000000 */
/* MIPI_ISR */
#define ISP_MIPI_MIPI_ISR_OFFSET                           (0x18U)
#define ISP_MIPI_MIPI_ISR_ISR_SYNC_FIFO_OVFLW_SHIFT        (0U)
#define ISP_MIPI_MIPI_ISR_ISR_SYNC_FIFO_OVFLW_MASK         (0xFU << ISP_MIPI_MIPI_ISR_ISR_SYNC_FIFO_OVFLW_SHIFT)        /* 0x0000000F */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_SOT_SHIFT                (4U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_SOT_MASK                 (0xFU << ISP_MIPI_MIPI_ISR_ISR_ERR_SOT_SHIFT)                /* 0x000000F0 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_SOT_SYNC_SHIFT           (8U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_SOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_ISR_ISR_ERR_SOT_SYNC_SHIFT)           /* 0x00000F00 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_EOT_SYNC_SHIFT           (12U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_EOT_SYNC_MASK            (0xFU << ISP_MIPI_MIPI_ISR_ISR_ERR_EOT_SYNC_SHIFT)           /* 0x0000F000 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_CONTROL_SHIFT            (16U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_CONTROL_MASK             (0xFU << ISP_MIPI_MIPI_ISR_ISR_ERR_CONTROL_SHIFT)            /* 0x000F0000 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_PROTOCOL_SHIFT           (20U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_PROTOCOL_MASK            (0x1U << ISP_MIPI_MIPI_ISR_ISR_ERR_PROTOCOL_SHIFT)           /* 0x00100000 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_ECC2_SHIFT               (21U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_ECC2_MASK                (0x1U << ISP_MIPI_MIPI_ISR_ISR_ERR_ECC2_SHIFT)               /* 0x00200000 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_ECC1_SHIFT               (22U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_ECC1_MASK                (0x1U << ISP_MIPI_MIPI_ISR_ISR_ERR_ECC1_SHIFT)               /* 0x00400000 */
#define ISP_MIPI_MIPI_ISR_ISR_ERR_CS_SHIFT                 (23U)
#define ISP_MIPI_MIPI_ISR_ISR_ERR_CS_MASK                  (0x1U << ISP_MIPI_MIPI_ISR_ISR_ERR_CS_SHIFT)                 /* 0x00800000 */
#define ISP_MIPI_MIPI_ISR_ISR_FRAME_END_SHIFT              (24U)
#define ISP_MIPI_MIPI_ISR_ISR_FRAME_END_MASK               (0x1U << ISP_MIPI_MIPI_ISR_ISR_FRAME_END_SHIFT)              /* 0x01000000 */
#define ISP_MIPI_MIPI_ISR_ISR_ADD_DATA_OVFLW_SHIFT         (25U)
#define ISP_MIPI_MIPI_ISR_ISR_ADD_DATA_OVFLW_MASK          (0x1U << ISP_MIPI_MIPI_ISR_ISR_ADD_DATA_OVFLW_SHIFT)         /* 0x02000000 */
#define ISP_MIPI_MIPI_ISR_ISR_ADD_DATA_FILL_LEVEL_SHIFT    (26U)
#define ISP_MIPI_MIPI_ISR_ISR_ADD_DATA_FILL_LEVEL_MASK     (0x1U << ISP_MIPI_MIPI_ISR_ISR_ADD_DATA_FILL_LEVEL_SHIFT)    /* 0x04000000 */
#define ISP_MIPI_MIPI_ISR_ISR_GEN_SHORT_PACK_SHIFT         (27U)
#define ISP_MIPI_MIPI_ISR_ISR_GEN_SHORT_PACK_MASK          (0x1U << ISP_MIPI_MIPI_ISR_ISR_GEN_SHORT_PACK_SHIFT)         /* 0x08000000 */
/* MIPI_CUR_DATA_ID */
#define ISP_MIPI_MIPI_CUR_DATA_ID_OFFSET                   (0x1CU)
#define ISP_MIPI_MIPI_CUR_DATA_ID                          (0x0U)
#define ISP_MIPI_MIPI_CUR_DATA_ID_DATA_TYPE_SHIFT          (0U)
#define ISP_MIPI_MIPI_CUR_DATA_ID_DATA_TYPE_MASK           (0x3FU << ISP_MIPI_MIPI_CUR_DATA_ID_DATA_TYPE_SHIFT)         /* 0x0000003F */
#define ISP_MIPI_MIPI_CUR_DATA_ID_VIRTUAL_CHANNEL_SHIFT    (6U)
#define ISP_MIPI_MIPI_CUR_DATA_ID_VIRTUAL_CHANNEL_MASK     (0x3U << ISP_MIPI_MIPI_CUR_DATA_ID_VIRTUAL_CHANNEL_SHIFT)    /* 0x000000C0 */
/* MIPI_IMG_DATA_SEL */
#define ISP_MIPI_MIPI_IMG_DATA_SEL_OFFSET                  (0x20U)
#define ISP_MIPI_MIPI_IMG_DATA_SEL_DATA_TYPE_SEL_SHIFT     (0U)
#define ISP_MIPI_MIPI_IMG_DATA_SEL_DATA_TYPE_SEL_MASK      (0x3FU << ISP_MIPI_MIPI_IMG_DATA_SEL_DATA_TYPE_SEL_SHIFT)    /* 0x0000003F */
#define ISP_MIPI_MIPI_IMG_DATA_SEL_VIRTUAL_CHANNEL_SEL_SHIFT (6U)
#define ISP_MIPI_MIPI_IMG_DATA_SEL_VIRTUAL_CHANNEL_SEL_MASK (0x3U << ISP_MIPI_MIPI_IMG_DATA_SEL_VIRTUAL_CHANNEL_SEL_SHIFT) /* 0x000000C0 */
/* MIPI_ADD_DATA_SEL_1 */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_1_OFFSET                (0x24U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_1_ADD_DATA_TYPE_1_SHIFT (0U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_1_ADD_DATA_TYPE_1_MASK  (0x3FU << ISP_MIPI_MIPI_ADD_DATA_SEL_1_ADD_DATA_TYPE_1_SHIFT) /* 0x0000003F */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_1_ADD_DATA_VC_1_SHIFT   (6U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_1_ADD_DATA_VC_1_MASK    (0x3U << ISP_MIPI_MIPI_ADD_DATA_SEL_1_ADD_DATA_VC_1_SHIFT)   /* 0x000000C0 */
/* MIPI_ADD_DATA_SEL_2 */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_2_OFFSET                (0x28U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_2_ADD_DATA_TYPE_2_SHIFT (0U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_2_ADD_DATA_TYPE_2_MASK  (0x3FU << ISP_MIPI_MIPI_ADD_DATA_SEL_2_ADD_DATA_TYPE_2_SHIFT) /* 0x0000003F */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_2_ADD_DATA_VC_2_SHIFT   (6U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_2_ADD_DATA_VC_2_MASK    (0x3U << ISP_MIPI_MIPI_ADD_DATA_SEL_2_ADD_DATA_VC_2_SHIFT)   /* 0x000000C0 */
/* MIPI_ADD_DATA_SEL_3 */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_3_OFFSET                (0x2CU)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_3_ADD_DATA_TYPE_3_SHIFT (0U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_3_ADD_DATA_TYPE_3_MASK  (0x3FU << ISP_MIPI_MIPI_ADD_DATA_SEL_3_ADD_DATA_TYPE_3_SHIFT) /* 0x0000003F */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_3_ADD_DATA_VC_3_SHIFT   (6U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_3_ADD_DATA_VC_3_MASK    (0x3U << ISP_MIPI_MIPI_ADD_DATA_SEL_3_ADD_DATA_VC_3_SHIFT)   /* 0x000000C0 */
/* MIPI_ADD_DATA_SEL_4 */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_4_OFFSET                (0x30U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_4_ADD_DATA_TYPE_4_SHIFT (0U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_4_ADD_DATA_TYPE_4_MASK  (0x3FU << ISP_MIPI_MIPI_ADD_DATA_SEL_4_ADD_DATA_TYPE_4_SHIFT) /* 0x0000003F */
#define ISP_MIPI_MIPI_ADD_DATA_SEL_4_ADD_DATA_VC_4_SHIFT   (6U)
#define ISP_MIPI_MIPI_ADD_DATA_SEL_4_ADD_DATA_VC_4_MASK    (0x3U << ISP_MIPI_MIPI_ADD_DATA_SEL_4_ADD_DATA_VC_4_SHIFT)   /* 0x000000C0 */
/* MIPI_ADD_DATA_FIFO */
#define ISP_MIPI_MIPI_ADD_DATA_FIFO_OFFSET                 (0x34U)
#define ISP_MIPI_MIPI_ADD_DATA_FIFO                        (0x0U)
#define ISP_MIPI_MIPI_ADD_DATA_FIFO_ADD_DATA_FIFO_SHIFT    (0U)
#define ISP_MIPI_MIPI_ADD_DATA_FIFO_ADD_DATA_FIFO_MASK     (0xFFFFFFFFU << ISP_MIPI_MIPI_ADD_DATA_FIFO_ADD_DATA_FIFO_SHIFT) /* 0xFFFFFFFF */
/* MIPI_ADD_DATA_FILL_LEVEL */
#define ISP_MIPI_MIPI_ADD_DATA_FILL_LEVEL_OFFSET           (0x38U)
#define ISP_MIPI_MIPI_ADD_DATA_FILL_LEVEL_ADD_DATA_FILL_LEVEL_SHIFT (0U)
#define ISP_MIPI_MIPI_ADD_DATA_FILL_LEVEL_ADD_DATA_FILL_LEVEL_MASK (0x1FFFU << ISP_MIPI_MIPI_ADD_DATA_FILL_LEVEL_ADD_DATA_FILL_LEVEL_SHIFT) /* 0x00001FFF */
/* MIPI_COMPRESSED_MODE */
#define ISP_MIPI_MIPI_COMPRESSED_MODE_OFFSET               (0x3CU)
#define ISP_MIPI_MIPI_COMPRESSED_MODE_COMPRESS_EN_SHIFT    (0U)
#define ISP_MIPI_MIPI_COMPRESSED_MODE_COMPRESS_EN_MASK     (0x1U << ISP_MIPI_MIPI_COMPRESSED_MODE_COMPRESS_EN_SHIFT)    /* 0x00000001 */
#define ISP_MIPI_MIPI_COMPRESSED_MODE_COMP_SCHEME_SHIFT    (4U)
#define ISP_MIPI_MIPI_COMPRESSED_MODE_COMP_SCHEME_MASK     (0x7U << ISP_MIPI_MIPI_COMPRESSED_MODE_COMP_SCHEME_SHIFT)    /* 0x00000070 */
#define ISP_MIPI_MIPI_COMPRESSED_MODE_PREDICTOR_SEL_SHIFT  (8U)
#define ISP_MIPI_MIPI_COMPRESSED_MODE_PREDICTOR_SEL_MASK   (0x1U << ISP_MIPI_MIPI_COMPRESSED_MODE_PREDICTOR_SEL_SHIFT)  /* 0x00000100 */
/* MIPI_FRAME */
#define ISP_MIPI_MIPI_FRAME_OFFSET                         (0x40U)
#define ISP_MIPI_MIPI_FRAME                                (0x0U)
#define ISP_MIPI_MIPI_FRAME_FRAME_NUMBER_FS_SHIFT          (0U)
#define ISP_MIPI_MIPI_FRAME_FRAME_NUMBER_FS_MASK           (0xFFFFU << ISP_MIPI_MIPI_FRAME_FRAME_NUMBER_FS_SHIFT)       /* 0x0000FFFF */
#define ISP_MIPI_MIPI_FRAME_FRAME_NUMBER_FE_SHIFT          (16U)
#define ISP_MIPI_MIPI_FRAME_FRAME_NUMBER_FE_MASK           (0xFFFFU << ISP_MIPI_MIPI_FRAME_FRAME_NUMBER_FE_SHIFT)       /* 0xFFFF0000 */
/* MIPI_GEN_SHORT_DT */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_OFFSET                  (0x44U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT                         (0x0U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0X8_SHIFT  (0U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0X8_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0X8_SHIFT)  /* 0x00000001 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0X9_SHIFT  (1U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0X9_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0X9_SHIFT)  /* 0x00000002 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XA_SHIFT  (2U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XA_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XA_SHIFT)  /* 0x00000004 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XB_SHIFT  (3U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XB_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XB_SHIFT)  /* 0x00000008 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XC_SHIFT  (4U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XC_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XC_SHIFT)  /* 0x00000010 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XD_SHIFT  (5U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XD_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XD_SHIFT)  /* 0x00000020 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XE_SHIFT  (6U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XE_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XE_SHIFT)  /* 0x00000040 */
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XF_SHIFT  (7U)
#define ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XF_MASK   (0x1U << ISP_MIPI_MIPI_GEN_SHORT_DT_GEN_SHORT_DT_0XF_SHIFT)  /* 0x00000080 */
/* MIPI_GEN_SHORT_8_9 */
#define ISP_MIPI_MIPI_GEN_SHORT_8_9_OFFSET                 (0x48U)
#define ISP_MIPI_MIPI_GEN_SHORT_8_9                        (0x0U)
#define ISP_MIPI_MIPI_GEN_SHORT_8_9_DATA_FIELD_8_SHIFT     (0U)
#define ISP_MIPI_MIPI_GEN_SHORT_8_9_DATA_FIELD_8_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_8_9_DATA_FIELD_8_SHIFT)  /* 0x0000FFFF */
#define ISP_MIPI_MIPI_GEN_SHORT_8_9_DATA_FIELD_9_SHIFT     (16U)
#define ISP_MIPI_MIPI_GEN_SHORT_8_9_DATA_FIELD_9_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_8_9_DATA_FIELD_9_SHIFT)  /* 0xFFFF0000 */
/* MIPI_GEN_SHORT_A_B */
#define ISP_MIPI_MIPI_GEN_SHORT_A_B_OFFSET                 (0x4CU)
#define ISP_MIPI_MIPI_GEN_SHORT_A_B                        (0x0U)
#define ISP_MIPI_MIPI_GEN_SHORT_A_B_DATA_FIELD_A_SHIFT     (0U)
#define ISP_MIPI_MIPI_GEN_SHORT_A_B_DATA_FIELD_A_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_A_B_DATA_FIELD_A_SHIFT)  /* 0x0000FFFF */
#define ISP_MIPI_MIPI_GEN_SHORT_A_B_DATA_FIELD_B_SHIFT     (16U)
#define ISP_MIPI_MIPI_GEN_SHORT_A_B_DATA_FIELD_B_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_A_B_DATA_FIELD_B_SHIFT)  /* 0xFFFF0000 */
/* MIPI_GEN_SHORT_C_D */
#define ISP_MIPI_MIPI_GEN_SHORT_C_D_OFFSET                 (0x50U)
#define ISP_MIPI_MIPI_GEN_SHORT_C_D                        (0x0U)
#define ISP_MIPI_MIPI_GEN_SHORT_C_D_DATA_FIELD_C_SHIFT     (0U)
#define ISP_MIPI_MIPI_GEN_SHORT_C_D_DATA_FIELD_C_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_C_D_DATA_FIELD_C_SHIFT)  /* 0x0000FFFF */
#define ISP_MIPI_MIPI_GEN_SHORT_C_D_DATA_FIELD_D_SHIFT     (16U)
#define ISP_MIPI_MIPI_GEN_SHORT_C_D_DATA_FIELD_D_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_C_D_DATA_FIELD_D_SHIFT)  /* 0xFFFF0000 */
/* MIPI_GEN_SHORT_E_F */
#define ISP_MIPI_MIPI_GEN_SHORT_E_F_OFFSET                 (0x54U)
#define ISP_MIPI_MIPI_GEN_SHORT_E_F                        (0x0U)
#define ISP_MIPI_MIPI_GEN_SHORT_E_F_DATA_FIELD_E_SHIFT     (0U)
#define ISP_MIPI_MIPI_GEN_SHORT_E_F_DATA_FIELD_E_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_E_F_DATA_FIELD_E_SHIFT)  /* 0x0000FFFF */
#define ISP_MIPI_MIPI_GEN_SHORT_E_F_DATA_FIELD_F_SHIFT     (16U)
#define ISP_MIPI_MIPI_GEN_SHORT_E_F_DATA_FIELD_F_MASK      (0xFFFFU << ISP_MIPI_MIPI_GEN_SHORT_E_F_DATA_FIELD_F_SHIFT)  /* 0xFFFF0000 */
/****************************************ISP_AFM*****************************************/
/* ISP_AFM_CTRL */
#define ISP_AFM_ISP_AFM_CTRL_OFFSET                        (0x0U)
#define ISP_AFM_ISP_AFM_CTRL_AFM_EN_SHIFT                  (0U)
#define ISP_AFM_ISP_AFM_CTRL_AFM_EN_MASK                   (0x1U << ISP_AFM_ISP_AFM_CTRL_AFM_EN_SHIFT)                  /* 0x00000001 */
/* ISP_AFM_LT_A */
#define ISP_AFM_ISP_AFM_LT_A_OFFSET                        (0x4U)
#define ISP_AFM_ISP_AFM_LT_A_A_V_T_SHIFT                   (0U)
#define ISP_AFM_ISP_AFM_LT_A_A_V_T_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_LT_A_A_V_T_SHIFT)                /* 0x00001FFF */
#define ISP_AFM_ISP_AFM_LT_A_A_H_L_SHIFT                   (16U)
#define ISP_AFM_ISP_AFM_LT_A_A_H_L_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_LT_A_A_H_L_SHIFT)                /* 0x1FFF0000 */
/* ISP_AFM_RB_A */
#define ISP_AFM_ISP_AFM_RB_A_OFFSET                        (0x8U)
#define ISP_AFM_ISP_AFM_RB_A_A_V_B_SHIFT                   (0U)
#define ISP_AFM_ISP_AFM_RB_A_A_V_B_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_RB_A_A_V_B_SHIFT)                /* 0x00001FFF */
#define ISP_AFM_ISP_AFM_RB_A_A_H_R_SHIFT                   (16U)
#define ISP_AFM_ISP_AFM_RB_A_A_H_R_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_RB_A_A_H_R_SHIFT)                /* 0x1FFF0000 */
/* ISP_AFM_LT_B */
#define ISP_AFM_ISP_AFM_LT_B_OFFSET                        (0xCU)
#define ISP_AFM_ISP_AFM_LT_B_B_V_T_SHIFT                   (0U)
#define ISP_AFM_ISP_AFM_LT_B_B_V_T_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_LT_B_B_V_T_SHIFT)                /* 0x00001FFF */
#define ISP_AFM_ISP_AFM_LT_B_B_H_L_SHIFT                   (16U)
#define ISP_AFM_ISP_AFM_LT_B_B_H_L_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_LT_B_B_H_L_SHIFT)                /* 0x1FFF0000 */
/* ISP_AFM_RB_B */
#define ISP_AFM_ISP_AFM_RB_B_OFFSET                        (0x10U)
#define ISP_AFM_ISP_AFM_RB_B_B_V_B_SHIFT                   (0U)
#define ISP_AFM_ISP_AFM_RB_B_B_V_B_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_RB_B_B_V_B_SHIFT)                /* 0x00001FFF */
#define ISP_AFM_ISP_AFM_RB_B_B_H_R_SHIFT                   (16U)
#define ISP_AFM_ISP_AFM_RB_B_B_H_R_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_RB_B_B_H_R_SHIFT)                /* 0x1FFF0000 */
/* ISP_AFM_LT_C */
#define ISP_AFM_ISP_AFM_LT_C_OFFSET                        (0x14U)
#define ISP_AFM_ISP_AFM_LT_C_C_V_T_SHIFT                   (0U)
#define ISP_AFM_ISP_AFM_LT_C_C_V_T_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_LT_C_C_V_T_SHIFT)                /* 0x00001FFF */
#define ISP_AFM_ISP_AFM_LT_C_C_H_L_SHIFT                   (16U)
#define ISP_AFM_ISP_AFM_LT_C_C_H_L_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_LT_C_C_H_L_SHIFT)                /* 0x1FFF0000 */
/* ISP_AFM_RB_C */
#define ISP_AFM_ISP_AFM_RB_C_OFFSET                        (0x18U)
#define ISP_AFM_ISP_AFM_RB_C_C_V_B_SHIFT                   (0U)
#define ISP_AFM_ISP_AFM_RB_C_C_V_B_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_RB_C_C_V_B_SHIFT)                /* 0x00001FFF */
#define ISP_AFM_ISP_AFM_RB_C_C_H_R_SHIFT                   (16U)
#define ISP_AFM_ISP_AFM_RB_C_C_H_R_MASK                    (0x1FFFU << ISP_AFM_ISP_AFM_RB_C_C_H_R_SHIFT)                /* 0x1FFF0000 */
/* ISP_AFM_THRES */
#define ISP_AFM_ISP_AFM_THRES_OFFSET                       (0x1CU)
#define ISP_AFM_ISP_AFM_THRES_AFM_THRES_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_THRES_AFM_THRES_MASK               (0xFFFFU << ISP_AFM_ISP_AFM_THRES_AFM_THRES_SHIFT)           /* 0x0000FFFF */
/* ISP_AFM_VAR_SHIFT */
#define ISP_AFM_ISP_AFM_VAR_SHIFT_OFFSET                   (0x20U)
#define ISP_AFM_ISP_AFM_VAR_SHIFT_AFM_VAR_SHIFT_SHIFT      (0U)
#define ISP_AFM_ISP_AFM_VAR_SHIFT_AFM_VAR_SHIFT_MASK       (0x7U << ISP_AFM_ISP_AFM_VAR_SHIFT_AFM_VAR_SHIFT_SHIFT)      /* 0x00000007 */
#define ISP_AFM_ISP_AFM_VAR_SHIFT_LUM_VAR_SHIFT_SHIFT      (16U)
#define ISP_AFM_ISP_AFM_VAR_SHIFT_LUM_VAR_SHIFT_MASK       (0x7U << ISP_AFM_ISP_AFM_VAR_SHIFT_LUM_VAR_SHIFT_SHIFT)      /* 0x00070000 */
/* ISP_AFM_SUM_A */
#define ISP_AFM_ISP_AFM_SUM_A_OFFSET                       (0x24U)
#define ISP_AFM_ISP_AFM_SUM_A                              (0x0U)
#define ISP_AFM_ISP_AFM_SUM_A_AFM_SUM_A_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_SUM_A_AFM_SUM_A_MASK               (0xFFFFFFFFU << ISP_AFM_ISP_AFM_SUM_A_AFM_SUM_A_SHIFT)       /* 0xFFFFFFFF */
/* ISP_AFM_SUM_B */
#define ISP_AFM_ISP_AFM_SUM_B_OFFSET                       (0x28U)
#define ISP_AFM_ISP_AFM_SUM_B                              (0x0U)
#define ISP_AFM_ISP_AFM_SUM_B_AFM_SUM_B_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_SUM_B_AFM_SUM_B_MASK               (0xFFFFFFFFU << ISP_AFM_ISP_AFM_SUM_B_AFM_SUM_B_SHIFT)       /* 0xFFFFFFFF */
/* ISP_AFM_SUM_C */
#define ISP_AFM_ISP_AFM_SUM_C_OFFSET                       (0x2CU)
#define ISP_AFM_ISP_AFM_SUM_C                              (0x0U)
#define ISP_AFM_ISP_AFM_SUM_C_AFM_SUM_C_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_SUM_C_AFM_SUM_C_MASK               (0xFFFFFFFFU << ISP_AFM_ISP_AFM_SUM_C_AFM_SUM_C_SHIFT)       /* 0xFFFFFFFF */
/* ISP_AFM_LUM_A */
#define ISP_AFM_ISP_AFM_LUM_A_OFFSET                       (0x30U)
#define ISP_AFM_ISP_AFM_LUM_A                              (0x0U)
#define ISP_AFM_ISP_AFM_LUM_A_AFM_LUM_A_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_LUM_A_AFM_LUM_A_MASK               (0xFFFFFFU << ISP_AFM_ISP_AFM_LUM_A_AFM_LUM_A_SHIFT)         /* 0x00FFFFFF */
/* ISP_AFM_LUM_B */
#define ISP_AFM_ISP_AFM_LUM_B_OFFSET                       (0x34U)
#define ISP_AFM_ISP_AFM_LUM_B                              (0x0U)
#define ISP_AFM_ISP_AFM_LUM_B_AFM_LUM_B_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_LUM_B_AFM_LUM_B_MASK               (0xFFFFFFU << ISP_AFM_ISP_AFM_LUM_B_AFM_LUM_B_SHIFT)         /* 0x00FFFFFF */
/* ISP_AFM_LUM_C */
#define ISP_AFM_ISP_AFM_LUM_C_OFFSET                       (0x38U)
#define ISP_AFM_ISP_AFM_LUM_C                              (0x0U)
#define ISP_AFM_ISP_AFM_LUM_C_AFM_LUM_C_SHIFT              (0U)
#define ISP_AFM_ISP_AFM_LUM_C_AFM_LUM_C_MASK               (0xFFFFFFU << ISP_AFM_ISP_AFM_LUM_C_AFM_LUM_C_SHIFT)         /* 0x00FFFFFF */
/****************************************ISP_LSC*****************************************/
/* ISP_LSC_CTRL */
#define ISP_LSC_ISP_LSC_CTRL_OFFSET                        (0x0U)
#define ISP_LSC_ISP_LSC_CTRL_LSC_EN_SHIFT                  (0U)
#define ISP_LSC_ISP_LSC_CTRL_LSC_EN_MASK                   (0x1U << ISP_LSC_ISP_LSC_CTRL_LSC_EN_SHIFT)                  /* 0x00000001 */
/* ISP_LSC_R_TABLE_ADDR */
#define ISP_LSC_ISP_LSC_R_TABLE_ADDR_OFFSET                (0x4U)
#define ISP_LSC_ISP_LSC_R_TABLE_ADDR_R_RAM_ADDR_SHIFT      (0U)
#define ISP_LSC_ISP_LSC_R_TABLE_ADDR_R_RAM_ADDR_MASK       (0x1FFU << ISP_LSC_ISP_LSC_R_TABLE_ADDR_R_RAM_ADDR_SHIFT)    /* 0x000001FF */
/* ISP_LSC_GR_TABLE_ADDR */
#define ISP_LSC_ISP_LSC_GR_TABLE_ADDR_OFFSET               (0x8U)
#define ISP_LSC_ISP_LSC_GR_TABLE_ADDR_GR_RAM_ADDR_SHIFT    (0U)
#define ISP_LSC_ISP_LSC_GR_TABLE_ADDR_GR_RAM_ADDR_MASK     (0x1FFU << ISP_LSC_ISP_LSC_GR_TABLE_ADDR_GR_RAM_ADDR_SHIFT)  /* 0x000001FF */
/* ISP_LSC_B_TABLE_ADDR */
#define ISP_LSC_ISP_LSC_B_TABLE_ADDR_OFFSET                (0xCU)
#define ISP_LSC_ISP_LSC_B_TABLE_ADDR_B_RAM_ADDR_SHIFT      (0U)
#define ISP_LSC_ISP_LSC_B_TABLE_ADDR_B_RAM_ADDR_MASK       (0x1FFU << ISP_LSC_ISP_LSC_B_TABLE_ADDR_B_RAM_ADDR_SHIFT)    /* 0x000001FF */
/* ISP_LSC_GB_TABLE_ADDR */
#define ISP_LSC_ISP_LSC_GB_TABLE_ADDR_OFFSET               (0x10U)
#define ISP_LSC_ISP_LSC_GB_TABLE_ADDR_GB_RAM_ADDR_SHIFT    (0U)
#define ISP_LSC_ISP_LSC_GB_TABLE_ADDR_GB_RAM_ADDR_MASK     (0x1FFU << ISP_LSC_ISP_LSC_GB_TABLE_ADDR_GB_RAM_ADDR_SHIFT)  /* 0x000001FF */
/* ISP_LSC_R_TABLE_DATA */
#define ISP_LSC_ISP_LSC_R_TABLE_DATA_OFFSET                (0x14U)
#define ISP_LSC_ISP_LSC_R_TABLE_DATA_R_SAMPLE_0_SHIFT      (0U)
#define ISP_LSC_ISP_LSC_R_TABLE_DATA_R_SAMPLE_0_MASK       (0xFFFU << ISP_LSC_ISP_LSC_R_TABLE_DATA_R_SAMPLE_0_SHIFT)    /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_R_TABLE_DATA_R_SAMPLE_1_SHIFT      (12U)
#define ISP_LSC_ISP_LSC_R_TABLE_DATA_R_SAMPLE_1_MASK       (0xFFFU << ISP_LSC_ISP_LSC_R_TABLE_DATA_R_SAMPLE_1_SHIFT)    /* 0x00FFF000 */
/* ISP_LSC_GR_TABLE_DATA */
#define ISP_LSC_ISP_LSC_GR_TABLE_DATA_OFFSET               (0x18U)
#define ISP_LSC_ISP_LSC_GR_TABLE_DATA_GR_SAMPLE_0_SHIFT    (0U)
#define ISP_LSC_ISP_LSC_GR_TABLE_DATA_GR_SAMPLE_0_MASK     (0xFFFU << ISP_LSC_ISP_LSC_GR_TABLE_DATA_GR_SAMPLE_0_SHIFT)  /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_GR_TABLE_DATA_GR_SAMPLE_1_SHIFT    (12U)
#define ISP_LSC_ISP_LSC_GR_TABLE_DATA_GR_SAMPLE_1_MASK     (0xFFFU << ISP_LSC_ISP_LSC_GR_TABLE_DATA_GR_SAMPLE_1_SHIFT)  /* 0x00FFF000 */
/* ISP_LSC_B_TABLE_DATA */
#define ISP_LSC_ISP_LSC_B_TABLE_DATA_OFFSET                (0x1CU)
#define ISP_LSC_ISP_LSC_B_TABLE_DATA_B_SAMPLE_0_SHIFT      (0U)
#define ISP_LSC_ISP_LSC_B_TABLE_DATA_B_SAMPLE_0_MASK       (0xFFFU << ISP_LSC_ISP_LSC_B_TABLE_DATA_B_SAMPLE_0_SHIFT)    /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_B_TABLE_DATA_B_SAMPLE_1_SHIFT      (12U)
#define ISP_LSC_ISP_LSC_B_TABLE_DATA_B_SAMPLE_1_MASK       (0xFFFU << ISP_LSC_ISP_LSC_B_TABLE_DATA_B_SAMPLE_1_SHIFT)    /* 0x00FFF000 */
/* ISP_LSC_GB_TABLE_DATA */
#define ISP_LSC_ISP_LSC_GB_TABLE_DATA_OFFSET               (0x20U)
#define ISP_LSC_ISP_LSC_GB_TABLE_DATA_GB_SAMPLE_0_SHIFT    (0U)
#define ISP_LSC_ISP_LSC_GB_TABLE_DATA_GB_SAMPLE_0_MASK     (0xFFFU << ISP_LSC_ISP_LSC_GB_TABLE_DATA_GB_SAMPLE_0_SHIFT)  /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_GB_TABLE_DATA_GB_SAMPLE_1_SHIFT    (12U)
#define ISP_LSC_ISP_LSC_GB_TABLE_DATA_GB_SAMPLE_1_MASK     (0xFFFU << ISP_LSC_ISP_LSC_GB_TABLE_DATA_GB_SAMPLE_1_SHIFT)  /* 0x00FFF000 */
/* ISP_LSC_XGRAD_01 */
#define ISP_LSC_ISP_LSC_XGRAD_01_OFFSET                    (0x24U)
#define ISP_LSC_ISP_LSC_XGRAD_01_XGRAD_0_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_XGRAD_01_XGRAD_0_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_01_XGRAD_0_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_XGRAD_01_XGRAD_1_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_XGRAD_01_XGRAD_1_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_01_XGRAD_1_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_XGRAD_23 */
#define ISP_LSC_ISP_LSC_XGRAD_23_OFFSET                    (0x28U)
#define ISP_LSC_ISP_LSC_XGRAD_23_XGRAD_2_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_XGRAD_23_XGRAD_2_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_23_XGRAD_2_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_XGRAD_23_XGRAD_3_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_XGRAD_23_XGRAD_3_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_23_XGRAD_3_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_XGRAD_45 */
#define ISP_LSC_ISP_LSC_XGRAD_45_OFFSET                    (0x2CU)
#define ISP_LSC_ISP_LSC_XGRAD_45_XGRAD_4_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_XGRAD_45_XGRAD_4_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_45_XGRAD_4_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_XGRAD_45_XGRAD_5_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_XGRAD_45_XGRAD_5_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_45_XGRAD_5_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_XGRAD_67 */
#define ISP_LSC_ISP_LSC_XGRAD_67_OFFSET                    (0x30U)
#define ISP_LSC_ISP_LSC_XGRAD_67_XGRAD_6_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_XGRAD_67_XGRAD_6_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_67_XGRAD_6_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_XGRAD_67_XGRAD_7_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_XGRAD_67_XGRAD_7_MASK              (0xFFFU << ISP_LSC_ISP_LSC_XGRAD_67_XGRAD_7_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_YGRAD_01 */
#define ISP_LSC_ISP_LSC_YGRAD_01_OFFSET                    (0x34U)
#define ISP_LSC_ISP_LSC_YGRAD_01_YGRAD_0_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_YGRAD_01_YGRAD_0_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_01_YGRAD_0_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_YGRAD_01_YGRAD_1_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_YGRAD_01_YGRAD_1_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_01_YGRAD_1_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_YGRAD_23 */
#define ISP_LSC_ISP_LSC_YGRAD_23_OFFSET                    (0x38U)
#define ISP_LSC_ISP_LSC_YGRAD_23_YGRAD_2_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_YGRAD_23_YGRAD_2_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_23_YGRAD_2_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_YGRAD_23_YGRAD_3_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_YGRAD_23_YGRAD_3_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_23_YGRAD_3_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_YGRAD_45 */
#define ISP_LSC_ISP_LSC_YGRAD_45_OFFSET                    (0x3CU)
#define ISP_LSC_ISP_LSC_YGRAD_45_YGRAD_4_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_YGRAD_45_YGRAD_4_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_45_YGRAD_4_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_YGRAD_45_YGRAD_5_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_YGRAD_45_YGRAD_5_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_45_YGRAD_5_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_YGRAD_67 */
#define ISP_LSC_ISP_LSC_YGRAD_67_OFFSET                    (0x40U)
#define ISP_LSC_ISP_LSC_YGRAD_67_YGRAD_6_SHIFT             (0U)
#define ISP_LSC_ISP_LSC_YGRAD_67_YGRAD_6_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_67_YGRAD_6_SHIFT)           /* 0x00000FFF */
#define ISP_LSC_ISP_LSC_YGRAD_67_YGRAD_7_SHIFT             (16U)
#define ISP_LSC_ISP_LSC_YGRAD_67_YGRAD_7_MASK              (0xFFFU << ISP_LSC_ISP_LSC_YGRAD_67_YGRAD_7_SHIFT)           /* 0x0FFF0000 */
/* ISP_LSC_XSIZE_01 */
#define ISP_LSC_ISP_LSC_XSIZE_01_OFFSET                    (0x44U)
#define ISP_LSC_ISP_LSC_XSIZE_01_X_SECT_SIZE_0_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_XSIZE_01_X_SECT_SIZE_0_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_01_X_SECT_SIZE_0_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_XSIZE_01_X_SECT_SIZE_1_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_XSIZE_01_X_SECT_SIZE_1_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_01_X_SECT_SIZE_1_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_XSIZE_23 */
#define ISP_LSC_ISP_LSC_XSIZE_23_OFFSET                    (0x48U)
#define ISP_LSC_ISP_LSC_XSIZE_23_X_SECT_SIZE_2_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_XSIZE_23_X_SECT_SIZE_2_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_23_X_SECT_SIZE_2_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_XSIZE_23_X_SECT_SIZE_3_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_XSIZE_23_X_SECT_SIZE_3_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_23_X_SECT_SIZE_3_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_XSIZE_45 */
#define ISP_LSC_ISP_LSC_XSIZE_45_OFFSET                    (0x4CU)
#define ISP_LSC_ISP_LSC_XSIZE_45_X_SECT_SIZE_4_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_XSIZE_45_X_SECT_SIZE_4_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_45_X_SECT_SIZE_4_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_XSIZE_45_X_SECT_SIZE_5_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_XSIZE_45_X_SECT_SIZE_5_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_45_X_SECT_SIZE_5_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_XSIZE_67 */
#define ISP_LSC_ISP_LSC_XSIZE_67_OFFSET                    (0x50U)
#define ISP_LSC_ISP_LSC_XSIZE_67_X_SECT_SIZE_6_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_XSIZE_67_X_SECT_SIZE_6_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_67_X_SECT_SIZE_6_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_XSIZE_67_X_SECT_SIZE_7_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_XSIZE_67_X_SECT_SIZE_7_MASK        (0x3FFU << ISP_LSC_ISP_LSC_XSIZE_67_X_SECT_SIZE_7_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_YSIZE_01 */
#define ISP_LSC_ISP_LSC_YSIZE_01_OFFSET                    (0x54U)
#define ISP_LSC_ISP_LSC_YSIZE_01_Y_SECT_SIZE_0_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_YSIZE_01_Y_SECT_SIZE_0_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_01_Y_SECT_SIZE_0_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_YSIZE_01_Y_SECT_SIZE_1_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_YSIZE_01_Y_SECT_SIZE_1_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_01_Y_SECT_SIZE_1_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_YSIZE_23 */
#define ISP_LSC_ISP_LSC_YSIZE_23_OFFSET                    (0x58U)
#define ISP_LSC_ISP_LSC_YSIZE_23_Y_SECT_SIZE_2_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_YSIZE_23_Y_SECT_SIZE_2_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_23_Y_SECT_SIZE_2_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_YSIZE_23_Y_SECT_SIZE_3_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_YSIZE_23_Y_SECT_SIZE_3_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_23_Y_SECT_SIZE_3_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_YSIZE_45 */
#define ISP_LSC_ISP_LSC_YSIZE_45_OFFSET                    (0x5CU)
#define ISP_LSC_ISP_LSC_YSIZE_45_Y_SECT_SIZE_4_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_YSIZE_45_Y_SECT_SIZE_4_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_45_Y_SECT_SIZE_4_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_YSIZE_45_Y_SECT_SIZE_5_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_YSIZE_45_Y_SECT_SIZE_5_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_45_Y_SECT_SIZE_5_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_YSIZE_67 */
#define ISP_LSC_ISP_LSC_YSIZE_67_OFFSET                    (0x60U)
#define ISP_LSC_ISP_LSC_YSIZE_67_Y_SECT_SIZE_6_SHIFT       (0U)
#define ISP_LSC_ISP_LSC_YSIZE_67_Y_SECT_SIZE_6_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_67_Y_SECT_SIZE_6_SHIFT)     /* 0x000003FF */
#define ISP_LSC_ISP_LSC_YSIZE_67_Y_SECT_SIZE_7_SHIFT       (16U)
#define ISP_LSC_ISP_LSC_YSIZE_67_Y_SECT_SIZE_7_MASK        (0x3FFU << ISP_LSC_ISP_LSC_YSIZE_67_Y_SECT_SIZE_7_SHIFT)     /* 0x03FF0000 */
/* ISP_LSC_TABLE_SEL */
#define ISP_LSC_ISP_LSC_TABLE_SEL_OFFSET                   (0x64U)
#define ISP_LSC_ISP_LSC_TABLE_SEL_TABLE_SEL_SHIFT          (0U)
#define ISP_LSC_ISP_LSC_TABLE_SEL_TABLE_SEL_MASK           (0x1U << ISP_LSC_ISP_LSC_TABLE_SEL_TABLE_SEL_SHIFT)          /* 0x00000001 */
/* ISP_LSC_STATUS */
#define ISP_LSC_ISP_LSC_STATUS_OFFSET                      (0x68U)
#define ISP_LSC_ISP_LSC_STATUS                             (0x0U)
#define ISP_LSC_ISP_LSC_STATUS_LSC_EN_STATUS_SHIFT         (0U)
#define ISP_LSC_ISP_LSC_STATUS_LSC_EN_STATUS_MASK          (0x1U << ISP_LSC_ISP_LSC_STATUS_LSC_EN_STATUS_SHIFT)         /* 0x00000001 */
#define ISP_LSC_ISP_LSC_STATUS_ACTIVE_TABLE_SHIFT          (1U)
#define ISP_LSC_ISP_LSC_STATUS_ACTIVE_TABLE_MASK           (0x1U << ISP_LSC_ISP_LSC_STATUS_ACTIVE_TABLE_SHIFT)          /* 0x00000002 */
/*****************************************ISP_IS*****************************************/
/* ISP_IS_CTRL */
#define ISP_IS_ISP_IS_CTRL_OFFSET                          (0x0U)
#define ISP_IS_ISP_IS_CTRL_IS_EN_SHIFT                     (0U)
#define ISP_IS_ISP_IS_CTRL_IS_EN_MASK                      (0x1U << ISP_IS_ISP_IS_CTRL_IS_EN_SHIFT)                     /* 0x00000001 */
/* ISP_IS_RECENTER */
#define ISP_IS_ISP_IS_RECENTER_OFFSET                      (0x4U)
#define ISP_IS_ISP_IS_RECENTER_IS_RECENTER_SHIFT           (0U)
#define ISP_IS_ISP_IS_RECENTER_IS_RECENTER_MASK            (0x7U << ISP_IS_ISP_IS_RECENTER_IS_RECENTER_SHIFT)           /* 0x00000007 */
/* ISP_IS_H_OFFS */
#define ISP_IS_ISP_IS_H_OFFS_OFFSET                        (0x8U)
#define ISP_IS_ISP_IS_H_OFFS_IS_H_OFFS_SHIFT               (0U)
#define ISP_IS_ISP_IS_H_OFFS_IS_H_OFFS_MASK                (0x3FFFU << ISP_IS_ISP_IS_H_OFFS_IS_H_OFFS_SHIFT)            /* 0x00003FFF */
/* ISP_IS_V_OFFS */
#define ISP_IS_ISP_IS_V_OFFS_OFFSET                        (0xCU)
#define ISP_IS_ISP_IS_V_OFFS_IS_V_OFFS_SHIFT               (0U)
#define ISP_IS_ISP_IS_V_OFFS_IS_V_OFFS_MASK                (0x3FFFU << ISP_IS_ISP_IS_V_OFFS_IS_V_OFFS_SHIFT)            /* 0x00003FFF */
/* ISP_IS_H_SIZE */
#define ISP_IS_ISP_IS_H_SIZE_OFFSET                        (0x10U)
#define ISP_IS_ISP_IS_H_SIZE_IS_H_SIZE_SHIFT               (0U)
#define ISP_IS_ISP_IS_H_SIZE_IS_H_SIZE_MASK                (0x3FFFU << ISP_IS_ISP_IS_H_SIZE_IS_H_SIZE_SHIFT)            /* 0x00003FFF */
/* ISP_IS_V_SIZE */
#define ISP_IS_ISP_IS_V_SIZE_OFFSET                        (0x14U)
#define ISP_IS_ISP_IS_V_SIZE_IS_V_SIZE_SHIFT               (0U)
#define ISP_IS_ISP_IS_V_SIZE_IS_V_SIZE_MASK                (0x3FFFU << ISP_IS_ISP_IS_V_SIZE_IS_V_SIZE_SHIFT)            /* 0x00003FFF */
/* ISP_IS_MAX_DX */
#define ISP_IS_ISP_IS_MAX_DX_OFFSET                        (0x18U)
#define ISP_IS_ISP_IS_MAX_DX_IS_MAX_DX_SHIFT               (0U)
#define ISP_IS_ISP_IS_MAX_DX_IS_MAX_DX_MASK                (0x1FFFU << ISP_IS_ISP_IS_MAX_DX_IS_MAX_DX_SHIFT)            /* 0x00001FFF */
/* ISP_IS_MAX_DY */
#define ISP_IS_ISP_IS_MAX_DY_OFFSET                        (0x1CU)
#define ISP_IS_ISP_IS_MAX_DY_IS_MAX_DY_SHIFT               (0U)
#define ISP_IS_ISP_IS_MAX_DY_IS_MAX_DY_MASK                (0x1FFFU << ISP_IS_ISP_IS_MAX_DY_IS_MAX_DY_SHIFT)            /* 0x00001FFF */
/* ISP_IS_DISPLACE */
#define ISP_IS_ISP_IS_DISPLACE_OFFSET                      (0x20U)
#define ISP_IS_ISP_IS_DISPLACE_DX_SHIFT                    (0U)
#define ISP_IS_ISP_IS_DISPLACE_DX_MASK                     (0x1FFFU << ISP_IS_ISP_IS_DISPLACE_DX_SHIFT)                 /* 0x00001FFF */
#define ISP_IS_ISP_IS_DISPLACE_DY_SHIFT                    (16U)
#define ISP_IS_ISP_IS_DISPLACE_DY_MASK                     (0x1FFFU << ISP_IS_ISP_IS_DISPLACE_DY_SHIFT)                 /* 0x1FFF0000 */
/* ISP_IS_H_OFFS_SHD */
#define ISP_IS_ISP_IS_H_OFFS_SHD_OFFSET                    (0x24U)
#define ISP_IS_ISP_IS_H_OFFS_SHD                           (0x0U)
#define ISP_IS_ISP_IS_H_OFFS_SHD_IS_H_OFFS_SHD_SHIFT       (0U)
#define ISP_IS_ISP_IS_H_OFFS_SHD_IS_H_OFFS_SHD_MASK        (0x3FFFU << ISP_IS_ISP_IS_H_OFFS_SHD_IS_H_OFFS_SHD_SHIFT)    /* 0x00003FFF */
/* ISP_IS_V_OFFS_SHD */
#define ISP_IS_ISP_IS_V_OFFS_SHD_OFFSET                    (0x28U)
#define ISP_IS_ISP_IS_V_OFFS_SHD                           (0x0U)
#define ISP_IS_ISP_IS_V_OFFS_SHD_IS_V_OFFS_SHD_SHIFT       (0U)
#define ISP_IS_ISP_IS_V_OFFS_SHD_IS_V_OFFS_SHD_MASK        (0x3FFFU << ISP_IS_ISP_IS_V_OFFS_SHD_IS_V_OFFS_SHD_SHIFT)    /* 0x00003FFF */
/* ISP_IS_H_SIZE_SHD */
#define ISP_IS_ISP_IS_H_SIZE_SHD_OFFSET                    (0x2CU)
#define ISP_IS_ISP_IS_H_SIZE_SHD                           (0x0U)
#define ISP_IS_ISP_IS_H_SIZE_SHD_ISP_H_SIZE_SHD_SHIFT      (0U)
#define ISP_IS_ISP_IS_H_SIZE_SHD_ISP_H_SIZE_SHD_MASK       (0x3FFFU << ISP_IS_ISP_IS_H_SIZE_SHD_ISP_H_SIZE_SHD_SHIFT)   /* 0x00003FFF */
/* ISP_IS_V_SIZE_SHD */
#define ISP_IS_ISP_IS_V_SIZE_SHD_OFFSET                    (0x30U)
#define ISP_IS_ISP_IS_V_SIZE_SHD                           (0x0U)
#define ISP_IS_ISP_IS_V_SIZE_SHD_ISP_V_SIZE_SHD_SHIFT      (0U)
#define ISP_IS_ISP_IS_V_SIZE_SHD_ISP_V_SIZE_SHD_MASK       (0x3FFFU << ISP_IS_ISP_IS_V_SIZE_SHD_ISP_V_SIZE_SHD_SHIFT)   /* 0x00003FFF */
/****************************************ISP_HIST****************************************/
/* ISP_HIST_PROP */
#define ISP_HIST_ISP_HIST_PROP_OFFSET                      (0x0U)
#define ISP_HIST_ISP_HIST_PROP_HIST_MODE_SHIFT             (0U)
#define ISP_HIST_ISP_HIST_PROP_HIST_MODE_MASK              (0x7U << ISP_HIST_ISP_HIST_PROP_HIST_MODE_SHIFT)             /* 0x00000007 */
#define ISP_HIST_ISP_HIST_PROP_STEPSIZE_SHIFT              (3U)
#define ISP_HIST_ISP_HIST_PROP_STEPSIZE_MASK               (0x7FU << ISP_HIST_ISP_HIST_PROP_STEPSIZE_SHIFT)             /* 0x000003F8 */
/* ISP_HIST_H_OFFS */
#define ISP_HIST_ISP_HIST_H_OFFS_OFFSET                    (0x4U)
#define ISP_HIST_ISP_HIST_H_OFFS_HIST_H_OFFSET_SHIFT       (0U)
#define ISP_HIST_ISP_HIST_H_OFFS_HIST_H_OFFSET_MASK        (0x1FFFU << ISP_HIST_ISP_HIST_H_OFFS_HIST_H_OFFSET_SHIFT)    /* 0x00001FFF */
/* ISP_HIST_V_OFFS */
#define ISP_HIST_ISP_HIST_V_OFFS_OFFSET                    (0x8U)
#define ISP_HIST_ISP_HIST_V_OFFS_HIST_V_OFFSET_SHIFT       (0U)
#define ISP_HIST_ISP_HIST_V_OFFS_HIST_V_OFFSET_MASK        (0x1FFFU << ISP_HIST_ISP_HIST_V_OFFS_HIST_V_OFFSET_SHIFT)    /* 0x00001FFF */
/* ISP_HIST_H_SIZE */
#define ISP_HIST_ISP_HIST_H_SIZE_OFFSET                    (0xCU)
#define ISP_HIST_ISP_HIST_H_SIZE_HIST_H_SIZE_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_H_SIZE_HIST_H_SIZE_MASK          (0x7FFU << ISP_HIST_ISP_HIST_H_SIZE_HIST_H_SIZE_SHIFT)       /* 0x000007FF */
/* ISP_HIST_V_SIZE */
#define ISP_HIST_ISP_HIST_V_SIZE_OFFSET                    (0x10U)
#define ISP_HIST_ISP_HIST_V_SIZE_HIST_V_SIZE_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_V_SIZE_HIST_V_SIZE_MASK          (0x7FFU << ISP_HIST_ISP_HIST_V_SIZE_HIST_V_SIZE_SHIFT)       /* 0x000007FF */
/* ISP_HIST_BIN_0 */
#define ISP_HIST_ISP_HIST_BIN_0_OFFSET                     (0x14U)
#define ISP_HIST_ISP_HIST_BIN_0                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_0_HIST_BIN_0_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_0_HIST_BIN_0_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_0_HIST_BIN_0_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_1 */
#define ISP_HIST_ISP_HIST_BIN_1_OFFSET                     (0x18U)
#define ISP_HIST_ISP_HIST_BIN_1                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_1_HIST_BIN_1_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_1_HIST_BIN_1_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_1_HIST_BIN_1_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_2 */
#define ISP_HIST_ISP_HIST_BIN_2_OFFSET                     (0x1CU)
#define ISP_HIST_ISP_HIST_BIN_2                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_2_HIST_BIN_2_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_2_HIST_BIN_2_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_2_HIST_BIN_2_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_3 */
#define ISP_HIST_ISP_HIST_BIN_3_OFFSET                     (0x20U)
#define ISP_HIST_ISP_HIST_BIN_3                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_3_HIST_BIN_3_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_3_HIST_BIN_3_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_3_HIST_BIN_3_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_4 */
#define ISP_HIST_ISP_HIST_BIN_4_OFFSET                     (0x24U)
#define ISP_HIST_ISP_HIST_BIN_4                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_4_HIST_BIN_4_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_4_HIST_BIN_4_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_4_HIST_BIN_4_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_5 */
#define ISP_HIST_ISP_HIST_BIN_5_OFFSET                     (0x28U)
#define ISP_HIST_ISP_HIST_BIN_5                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_5_HIST_BIN_5_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_5_HIST_BIN_5_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_5_HIST_BIN_5_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_6 */
#define ISP_HIST_ISP_HIST_BIN_6_OFFSET                     (0x2CU)
#define ISP_HIST_ISP_HIST_BIN_6                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_6_HIST_BIN_6_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_6_HIST_BIN_6_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_6_HIST_BIN_6_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_7 */
#define ISP_HIST_ISP_HIST_BIN_7_OFFSET                     (0x30U)
#define ISP_HIST_ISP_HIST_BIN_7                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_7_HIST_BIN_7_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_7_HIST_BIN_7_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_7_HIST_BIN_7_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_8 */
#define ISP_HIST_ISP_HIST_BIN_8_OFFSET                     (0x34U)
#define ISP_HIST_ISP_HIST_BIN_8                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_8_HIST_BIN_8_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_8_HIST_BIN_8_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_8_HIST_BIN_8_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_9 */
#define ISP_HIST_ISP_HIST_BIN_9_OFFSET                     (0x38U)
#define ISP_HIST_ISP_HIST_BIN_9                            (0x0U)
#define ISP_HIST_ISP_HIST_BIN_9_HIST_BIN_9_SHIFT           (0U)
#define ISP_HIST_ISP_HIST_BIN_9_HIST_BIN_9_MASK            (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_9_HIST_BIN_9_SHIFT)       /* 0x000FFFFF */
/* ISP_HIST_BIN_10 */
#define ISP_HIST_ISP_HIST_BIN_10_OFFSET                    (0x3CU)
#define ISP_HIST_ISP_HIST_BIN_10                           (0x0U)
#define ISP_HIST_ISP_HIST_BIN_10_HIST_BIN_10_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_BIN_10_HIST_BIN_10_MASK          (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_10_HIST_BIN_10_SHIFT)     /* 0x000FFFFF */
/* ISP_HIST_BIN_11 */
#define ISP_HIST_ISP_HIST_BIN_11_OFFSET                    (0x40U)
#define ISP_HIST_ISP_HIST_BIN_11                           (0x0U)
#define ISP_HIST_ISP_HIST_BIN_11_HIST_BIN_11_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_BIN_11_HIST_BIN_11_MASK          (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_11_HIST_BIN_11_SHIFT)     /* 0x000FFFFF */
/* ISP_HIST_BIN_12 */
#define ISP_HIST_ISP_HIST_BIN_12_OFFSET                    (0x44U)
#define ISP_HIST_ISP_HIST_BIN_12                           (0x0U)
#define ISP_HIST_ISP_HIST_BIN_12_HIST_BIN_12_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_BIN_12_HIST_BIN_12_MASK          (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_12_HIST_BIN_12_SHIFT)     /* 0x000FFFFF */
/* ISP_HIST_BIN_13 */
#define ISP_HIST_ISP_HIST_BIN_13_OFFSET                    (0x48U)
#define ISP_HIST_ISP_HIST_BIN_13                           (0x0U)
#define ISP_HIST_ISP_HIST_BIN_13_HIST_BIN_13_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_BIN_13_HIST_BIN_13_MASK          (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_13_HIST_BIN_13_SHIFT)     /* 0x000FFFFF */
/* ISP_HIST_BIN_14 */
#define ISP_HIST_ISP_HIST_BIN_14_OFFSET                    (0x4CU)
#define ISP_HIST_ISP_HIST_BIN_14                           (0x0U)
#define ISP_HIST_ISP_HIST_BIN_14_HIST_BIN_14_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_BIN_14_HIST_BIN_14_MASK          (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_14_HIST_BIN_14_SHIFT)     /* 0x000FFFFF */
/* ISP_HIST_BIN_15 */
#define ISP_HIST_ISP_HIST_BIN_15_OFFSET                    (0x50U)
#define ISP_HIST_ISP_HIST_BIN_15                           (0x0U)
#define ISP_HIST_ISP_HIST_BIN_15_HIST_BIN_15_SHIFT         (0U)
#define ISP_HIST_ISP_HIST_BIN_15_HIST_BIN_15_MASK          (0xFFFFFU << ISP_HIST_ISP_HIST_BIN_15_HIST_BIN_15_SHIFT)     /* 0x000FFFFF */
/* ISP_HIST_WEIGHT_00TO30 */
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_OFFSET             (0x54U)
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_00_SHIFT (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_00_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_00_SHIFT) /* 0x0000001F */
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_10_SHIFT (8U)
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_10_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_10_SHIFT) /* 0x00001F00 */
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_20_SHIFT (16U)
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_20_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_20_SHIFT) /* 0x001F0000 */
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_30_SHIFT (24U)
#define ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_30_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_00TO30_HIST_WEIGHT_30_SHIFT) /* 0x1F000000 */
/* ISP_HIST_WEIGHT_40TO21 */
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_OFFSET             (0x58U)
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_40_SHIFT (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_40_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_40_SHIFT) /* 0x0000001F */
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_01_SHIFT (8U)
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_01_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_01_SHIFT) /* 0x00001F00 */
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_11_SHIFT (16U)
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_11_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_11_SHIFT) /* 0x001F0000 */
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_21_SHIFT (24U)
#define ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_21_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_40TO21_HIST_WEIGHT_21_SHIFT) /* 0x1F000000 */
/* ISP_HIST_WEIGHT_31TO12 */
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_OFFSET             (0x5CU)
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_31_SHIFT (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_31_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_31_SHIFT) /* 0x0000001F */
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_41_SHIFT (8U)
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_41_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_41_SHIFT) /* 0x00001F00 */
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_02_SHIFT (16U)
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_02_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_02_SHIFT) /* 0x001F0000 */
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_12_SHIFT (24U)
#define ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_12_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_31TO12_HIST_WEIGHT_12_SHIFT) /* 0x1F000000 */
/* ISP_HIST_WEIGHT_22TO03 */
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_OFFSET             (0x60U)
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_22_SHIFT (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_22_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_22_SHIFT) /* 0x0000001F */
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_32_SHIFT (8U)
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_32_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_32_SHIFT) /* 0x00001F00 */
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_42_SHIFT (16U)
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_42_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_42_SHIFT) /* 0x001F0000 */
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_03_SHIFT (24U)
#define ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_03_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_22TO03_HIST_WEIGHT_03_SHIFT) /* 0x1F000000 */
/* ISP_HIST_WEIGHT_13TO43 */
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_OFFSET             (0x64U)
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_13_SHIFT (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_13_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_13_SHIFT) /* 0x0000001F */
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_23_SHIFT (8U)
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_23_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_23_SHIFT) /* 0x00001F00 */
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_33_SHIFT (16U)
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_33_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_33_SHIFT) /* 0x001F0000 */
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_43_SHIFT (24U)
#define ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_43_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_13TO43_HIST_WEIGHT_43_SHIFT) /* 0x1F000000 */
/* ISP_HIST_WEIGHT_04TO34 */
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_OFFSET             (0x68U)
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_04_SHIFT (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_04_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_04_SHIFT) /* 0x0000001F */
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_14_SHIFT (8U)
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_14_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_14_SHIFT) /* 0x00001F00 */
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_24_SHIFT (16U)
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_24_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_24_SHIFT) /* 0x001F0000 */
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_34_SHIFT (24U)
#define ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_34_MASK (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_04TO34_HIST_WEIGHT_34_SHIFT) /* 0x1F000000 */
/* ISP_HIST_WEIGHT_44 */
#define ISP_HIST_ISP_HIST_WEIGHT_44_OFFSET                 (0x6CU)
#define ISP_HIST_ISP_HIST_WEIGHT_44_HIST_WEIGHT_44_SHIFT   (0U)
#define ISP_HIST_ISP_HIST_WEIGHT_44_HIST_WEIGHT_44_MASK    (0x1FU << ISP_HIST_ISP_HIST_WEIGHT_44_HIST_WEIGHT_44_SHIFT)  /* 0x0000001F */
/***************************************ISP_FILTER***************************************/
/* ISP_FILT_MODE */
#define ISP_FILTER_ISP_FILT_MODE_OFFSET                    (0x0U)
#define ISP_FILTER_ISP_FILT_MODE_FILT_ENABLE_SHIFT         (0U)
#define ISP_FILTER_ISP_FILT_MODE_FILT_ENABLE_MASK          (0x1U << ISP_FILTER_ISP_FILT_MODE_FILT_ENABLE_SHIFT)         /* 0x00000001 */
#define ISP_FILTER_ISP_FILT_MODE_FILT_MODE_SHIFT           (1U)
#define ISP_FILTER_ISP_FILT_MODE_FILT_MODE_MASK            (0x1U << ISP_FILTER_ISP_FILT_MODE_FILT_MODE_SHIFT)           /* 0x00000002 */
#define ISP_FILTER_ISP_FILT_MODE_FILT_CHR_V_MODE_SHIFT     (4U)
#define ISP_FILTER_ISP_FILT_MODE_FILT_CHR_V_MODE_MASK      (0x3U << ISP_FILTER_ISP_FILT_MODE_FILT_CHR_V_MODE_SHIFT)     /* 0x00000030 */
#define ISP_FILTER_ISP_FILT_MODE_FILT_CHR_H_MODE_SHIFT     (6U)
#define ISP_FILTER_ISP_FILT_MODE_FILT_CHR_H_MODE_MASK      (0x3U << ISP_FILTER_ISP_FILT_MODE_FILT_CHR_H_MODE_SHIFT)     /* 0x000000C0 */
#define ISP_FILTER_ISP_FILT_MODE_STAGE1_SELECT_SHIFT       (8U)
#define ISP_FILTER_ISP_FILT_MODE_STAGE1_SELECT_MASK        (0xFU << ISP_FILTER_ISP_FILT_MODE_STAGE1_SELECT_SHIFT)       /* 0x00000F00 */
/* ISP_FILT_THRESH_BL0 */
#define ISP_FILTER_ISP_FILT_THRESH_BL0_OFFSET              (0x28U)
#define ISP_FILTER_ISP_FILT_THRESH_BL0_FILT_THRESH_BL0_SHIFT (0U)
#define ISP_FILTER_ISP_FILT_THRESH_BL0_FILT_THRESH_BL0_MASK (0x3FFU << ISP_FILTER_ISP_FILT_THRESH_BL0_FILT_THRESH_BL0_SHIFT) /* 0x000003FF */
/* ISP_FILT_THRESH_BL1 */
#define ISP_FILTER_ISP_FILT_THRESH_BL1_OFFSET              (0x2CU)
#define ISP_FILTER_ISP_FILT_THRESH_BL1_FILT_THRESH_BL1_SHIFT (0U)
#define ISP_FILTER_ISP_FILT_THRESH_BL1_FILT_THRESH_BL1_MASK (0x3FFU << ISP_FILTER_ISP_FILT_THRESH_BL1_FILT_THRESH_BL1_SHIFT) /* 0x000003FF */
/* ISP_FILT_THRESH_SH0 */
#define ISP_FILTER_ISP_FILT_THRESH_SH0_OFFSET              (0x30U)
#define ISP_FILTER_ISP_FILT_THRESH_SH0_FILT_THRESH_SH0_SHIFT (0U)
#define ISP_FILTER_ISP_FILT_THRESH_SH0_FILT_THRESH_SH0_MASK (0x3FFU << ISP_FILTER_ISP_FILT_THRESH_SH0_FILT_THRESH_SH0_SHIFT) /* 0x000003FF */
/* ISP_FILT_THRESH_SH1 */
#define ISP_FILTER_ISP_FILT_THRESH_SH1_OFFSET              (0x34U)
#define ISP_FILTER_ISP_FILT_THRESH_SH1_FILT_THRESH_SH1_SHIFT (0U)
#define ISP_FILTER_ISP_FILT_THRESH_SH1_FILT_THRESH_SH1_MASK (0x3FFU << ISP_FILTER_ISP_FILT_THRESH_SH1_FILT_THRESH_SH1_SHIFT) /* 0x000003FF */
/* ISP_FILT_LUM_WEIGHT */
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_OFFSET              (0x38U)
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_MIN_SHIFT (0U)
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_MIN_MASK (0xFFU << ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_MIN_SHIFT) /* 0x000000FF */
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_KINK_SHIFT (8U)
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_KINK_MASK (0xFFU << ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_KINK_SHIFT) /* 0x0000FF00 */
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_GAIN_SHIFT (16U)
#define ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_GAIN_MASK (0x7U << ISP_FILTER_ISP_FILT_LUM_WEIGHT_LUM_WEIGHT_GAIN_SHIFT) /* 0x00070000 */
/* ISP_FILT_FAC_SH1 */
#define ISP_FILTER_ISP_FILT_FAC_SH1_OFFSET                 (0x3CU)
#define ISP_FILTER_ISP_FILT_FAC_SH1_FILT_FAC_SH1_SHIFT     (0U)
#define ISP_FILTER_ISP_FILT_FAC_SH1_FILT_FAC_SH1_MASK      (0x3FU << ISP_FILTER_ISP_FILT_FAC_SH1_FILT_FAC_SH1_SHIFT)    /* 0x0000003F */
/* ISP_FILT_FAC_SH0 */
#define ISP_FILTER_ISP_FILT_FAC_SH0_OFFSET                 (0x40U)
#define ISP_FILTER_ISP_FILT_FAC_SH0_FILT_FAC_SH0_SHIFT     (0U)
#define ISP_FILTER_ISP_FILT_FAC_SH0_FILT_FAC_SH0_MASK      (0x3FU << ISP_FILTER_ISP_FILT_FAC_SH0_FILT_FAC_SH0_SHIFT)    /* 0x0000003F */
/* ISP_FILT_FAC_MID */
#define ISP_FILTER_ISP_FILT_FAC_MID_OFFSET                 (0x44U)
#define ISP_FILTER_ISP_FILT_FAC_MID_FILT_FAC_MID_SHIFT     (0U)
#define ISP_FILTER_ISP_FILT_FAC_MID_FILT_FAC_MID_MASK      (0x3FU << ISP_FILTER_ISP_FILT_FAC_MID_FILT_FAC_MID_SHIFT)    /* 0x0000003F */
/* ISP_FILT_FAC_BL0 */
#define ISP_FILTER_ISP_FILT_FAC_BL0_OFFSET                 (0x48U)
#define ISP_FILTER_ISP_FILT_FAC_BL0_FILT_FAC_BL0_SHIFT     (0U)
#define ISP_FILTER_ISP_FILT_FAC_BL0_FILT_FAC_BL0_MASK      (0x3FU << ISP_FILTER_ISP_FILT_FAC_BL0_FILT_FAC_BL0_SHIFT)    /* 0x0000003F */
/* ISP_FILT_FAC_BL1 */
#define ISP_FILTER_ISP_FILT_FAC_BL1_OFFSET                 (0x4CU)
#define ISP_FILTER_ISP_FILT_FAC_BL1_FILT_FAC_BL1_SHIFT     (0U)
#define ISP_FILTER_ISP_FILT_FAC_BL1_FILT_FAC_BL1_MASK      (0x3FU << ISP_FILTER_ISP_FILT_FAC_BL1_FILT_FAC_BL1_SHIFT)    /* 0x0000003F */
/****************************************ISP_CAC*****************************************/
/* ISP_CAC_CTRL */
#define ISP_CAC_ISP_CAC_CTRL_OFFSET                        (0x0U)
#define ISP_CAC_ISP_CAC_CTRL_CAC_EN_SHIFT                  (0U)
#define ISP_CAC_ISP_CAC_CTRL_CAC_EN_MASK                   (0x1U << ISP_CAC_ISP_CAC_CTRL_CAC_EN_SHIFT)                  /* 0x00000001 */
#define ISP_CAC_ISP_CAC_CTRL_V_CLIP_MODE_SHIFT             (1U)
#define ISP_CAC_ISP_CAC_CTRL_V_CLIP_MODE_MASK              (0x3U << ISP_CAC_ISP_CAC_CTRL_V_CLIP_MODE_SHIFT)             /* 0x00000006 */
#define ISP_CAC_ISP_CAC_CTRL_H_CLIP_MODE_SHIFT             (3U)
#define ISP_CAC_ISP_CAC_CTRL_H_CLIP_MODE_MASK              (0x1U << ISP_CAC_ISP_CAC_CTRL_H_CLIP_MODE_SHIFT)             /* 0x00000008 */
/* ISP_CAC_COUNT_START */
#define ISP_CAC_ISP_CAC_COUNT_START_OFFSET                 (0x4U)
#define ISP_CAC_ISP_CAC_COUNT_START_H_COUNT_START_SHIFT    (0U)
#define ISP_CAC_ISP_CAC_COUNT_START_H_COUNT_START_MASK     (0x1FFFU << ISP_CAC_ISP_CAC_COUNT_START_H_COUNT_START_SHIFT) /* 0x00001FFF */
#define ISP_CAC_ISP_CAC_COUNT_START_V_COUNT_START_SHIFT    (16U)
#define ISP_CAC_ISP_CAC_COUNT_START_V_COUNT_START_MASK     (0x1FFFU << ISP_CAC_ISP_CAC_COUNT_START_V_COUNT_START_SHIFT) /* 0x1FFF0000 */
/* ISP_CAC_A */
#define ISP_CAC_ISP_CAC_A_OFFSET                           (0x8U)
#define ISP_CAC_ISP_CAC_A_A_RED_SHIFT                      (0U)
#define ISP_CAC_ISP_CAC_A_A_RED_MASK                       (0x1FFU << ISP_CAC_ISP_CAC_A_A_RED_SHIFT)                    /* 0x000001FF */
#define ISP_CAC_ISP_CAC_A_A_BLUE_SHIFT                     (16U)
#define ISP_CAC_ISP_CAC_A_A_BLUE_MASK                      (0x1FFU << ISP_CAC_ISP_CAC_A_A_BLUE_SHIFT)                   /* 0x01FF0000 */
/* ISP_CAC_B */
#define ISP_CAC_ISP_CAC_B_OFFSET                           (0xCU)
#define ISP_CAC_ISP_CAC_B_B_RED_SHIFT                      (0U)
#define ISP_CAC_ISP_CAC_B_B_RED_MASK                       (0x1FFU << ISP_CAC_ISP_CAC_B_B_RED_SHIFT)                    /* 0x000001FF */
#define ISP_CAC_ISP_CAC_B_B_BLUE_SHIFT                     (16U)
#define ISP_CAC_ISP_CAC_B_B_BLUE_MASK                      (0x1FFU << ISP_CAC_ISP_CAC_B_B_BLUE_SHIFT)                   /* 0x01FF0000 */
/* ISP_CAC_C */
#define ISP_CAC_ISP_CAC_C_OFFSET                           (0x10U)
#define ISP_CAC_ISP_CAC_C_C_RED_SHIFT                      (0U)
#define ISP_CAC_ISP_CAC_C_C_RED_MASK                       (0x1FFU << ISP_CAC_ISP_CAC_C_C_RED_SHIFT)                    /* 0x000001FF */
#define ISP_CAC_ISP_CAC_C_C_BLUE_SHIFT                     (16U)
#define ISP_CAC_ISP_CAC_C_C_BLUE_MASK                      (0x1FFU << ISP_CAC_ISP_CAC_C_C_BLUE_SHIFT)                   /* 0x01FF0000 */
/* ISP_CAC_X_NORM */
#define ISP_CAC_ISP_CAC_X_NORM_OFFSET                      (0x14U)
#define ISP_CAC_ISP_CAC_X_NORM_X_NF_SHIFT                  (0U)
#define ISP_CAC_ISP_CAC_X_NORM_X_NF_MASK                   (0x1FU << ISP_CAC_ISP_CAC_X_NORM_X_NF_SHIFT)                 /* 0x0000001F */
#define ISP_CAC_ISP_CAC_X_NORM_X_NS_SHIFT                  (16U)
#define ISP_CAC_ISP_CAC_X_NORM_X_NS_MASK                   (0xFU << ISP_CAC_ISP_CAC_X_NORM_X_NS_SHIFT)                  /* 0x000F0000 */
/* ISP_CAC_Y_NORM */
#define ISP_CAC_ISP_CAC_Y_NORM_OFFSET                      (0x18U)
#define ISP_CAC_ISP_CAC_Y_NORM_Y_NF_SHIFT                  (0U)
#define ISP_CAC_ISP_CAC_Y_NORM_Y_NF_MASK                   (0x1FU << ISP_CAC_ISP_CAC_Y_NORM_Y_NF_SHIFT)                 /* 0x0000001F */
#define ISP_CAC_ISP_CAC_Y_NORM_Y_NS_SHIFT                  (16U)
#define ISP_CAC_ISP_CAC_Y_NORM_Y_NS_MASK                   (0xFU << ISP_CAC_ISP_CAC_Y_NORM_Y_NS_SHIFT)                  /* 0x000F0000 */
/**************************************ISP_EXPOSURE**************************************/
/* ISP_EXP_CTRL */
#define ISP_EXPOSURE_ISP_EXP_CTRL_OFFSET                   (0x0U)
#define ISP_EXPOSURE_ISP_EXP_CTRL_EXP_START_SHIFT          (0U)
#define ISP_EXPOSURE_ISP_EXP_CTRL_EXP_START_MASK           (0x1U << ISP_EXPOSURE_ISP_EXP_CTRL_EXP_START_SHIFT)          /* 0x00000001 */
#define ISP_EXPOSURE_ISP_EXP_CTRL_AUTOSTOP_SHIFT           (1U)
#define ISP_EXPOSURE_ISP_EXP_CTRL_AUTOSTOP_MASK            (0x1U << ISP_EXPOSURE_ISP_EXP_CTRL_AUTOSTOP_SHIFT)           /* 0x00000002 */
#define ISP_EXPOSURE_ISP_EXP_CTRL_EXP_MEAS_MODE_SHIFT      (31U)
#define ISP_EXPOSURE_ISP_EXP_CTRL_EXP_MEAS_MODE_MASK       (0x1U << ISP_EXPOSURE_ISP_EXP_CTRL_EXP_MEAS_MODE_SHIFT)      /* 0x80000000 */
/* ISP_EXP_H_OFFSET */
#define ISP_EXPOSURE_ISP_EXP_H_OFFSET_OFFSET               (0x4U)
#define ISP_EXPOSURE_ISP_EXP_H_OFFSET_ISP_EXP_H_OFFSET_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_H_OFFSET_ISP_EXP_H_OFFSET_MASK (0x1FFFU << ISP_EXPOSURE_ISP_EXP_H_OFFSET_ISP_EXP_H_OFFSET_SHIFT) /* 0x00001FFF */
/* ISP_EXP_V_OFFSET */
#define ISP_EXPOSURE_ISP_EXP_V_OFFSET_OFFSET               (0x8U)
#define ISP_EXPOSURE_ISP_EXP_V_OFFSET_ISP_EXP_V_OFFSET_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_V_OFFSET_ISP_EXP_V_OFFSET_MASK (0x1FFFU << ISP_EXPOSURE_ISP_EXP_V_OFFSET_ISP_EXP_V_OFFSET_SHIFT) /* 0x00001FFF */
/* ISP_EXP_H_SIZE */
#define ISP_EXPOSURE_ISP_EXP_H_SIZE_OFFSET                 (0xCU)
#define ISP_EXPOSURE_ISP_EXP_H_SIZE_ISP_EXP_H_SIZE_SHIFT   (0U)
#define ISP_EXPOSURE_ISP_EXP_H_SIZE_ISP_EXP_H_SIZE_MASK    (0x7FFU << ISP_EXPOSURE_ISP_EXP_H_SIZE_ISP_EXP_H_SIZE_SHIFT) /* 0x000007FF */
/* ISP_EXP_V_SIZE */
#define ISP_EXPOSURE_ISP_EXP_V_SIZE_OFFSET                 (0x10U)
#define ISP_EXPOSURE_ISP_EXP_V_SIZE_ISP_EXP_V_SIZE_SHIFT   (0U)
#define ISP_EXPOSURE_ISP_EXP_V_SIZE_ISP_EXP_V_SIZE_MASK    (0x7FFU << ISP_EXPOSURE_ISP_EXP_V_SIZE_ISP_EXP_V_SIZE_SHIFT) /* 0x000007FF */
/* ISP_EXP_MEAN_00 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_00_OFFSET                (0x14U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_00                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_00_ISP_EXP_MEAN_00_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_00_ISP_EXP_MEAN_00_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_00_ISP_EXP_MEAN_00_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_10 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_10_OFFSET                (0x18U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_10                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_10_ISP_EXP_MEAN_10_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_10_ISP_EXP_MEAN_10_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_10_ISP_EXP_MEAN_10_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_20 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_20_OFFSET                (0x1CU)
#define ISP_EXPOSURE_ISP_EXP_MEAN_20                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_20_ISP_EXP_MEAN_20_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_20_ISP_EXP_MEAN_20_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_20_ISP_EXP_MEAN_20_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_30 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_30_OFFSET                (0x20U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_30                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_30_ISP_EXP_MEAN_30_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_30_ISP_EXP_MEAN_30_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_30_ISP_EXP_MEAN_30_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_40 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_40_OFFSET                (0x24U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_40                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_40_ISP_EXP_MEAN_40_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_40_ISP_EXP_MEAN_40_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_40_ISP_EXP_MEAN_40_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_01 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_01_OFFSET                (0x28U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_01                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_01_ISP_EXP_MEAN_01_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_01_ISP_EXP_MEAN_01_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_01_ISP_EXP_MEAN_01_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_11 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_11_OFFSET                (0x2CU)
#define ISP_EXPOSURE_ISP_EXP_MEAN_11                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_11_ISP_EXP_MEAN_11_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_11_ISP_EXP_MEAN_11_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_11_ISP_EXP_MEAN_11_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_21 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_21_OFFSET                (0x30U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_21                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_21_ISP_EXP_MEAN_21_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_21_ISP_EXP_MEAN_21_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_21_ISP_EXP_MEAN_21_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_31 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_31_OFFSET                (0x34U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_31                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_31_ISP_EXP_MEAN_31_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_31_ISP_EXP_MEAN_31_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_31_ISP_EXP_MEAN_31_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_41 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_41_OFFSET                (0x38U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_41                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_41_ISP_EXP_MEAN_41_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_41_ISP_EXP_MEAN_41_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_41_ISP_EXP_MEAN_41_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_02 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_02_OFFSET                (0x3CU)
#define ISP_EXPOSURE_ISP_EXP_MEAN_02                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_02_ISP_EXP_MEAN_02_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_02_ISP_EXP_MEAN_02_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_02_ISP_EXP_MEAN_02_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_12 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_12_OFFSET                (0x40U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_12                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_12_ISP_EXP_MEAN_12_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_12_ISP_EXP_MEAN_12_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_12_ISP_EXP_MEAN_12_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_22 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_22_OFFSET                (0x44U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_22                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_22_ISP_EXP_MEAN_22_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_22_ISP_EXP_MEAN_22_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_22_ISP_EXP_MEAN_22_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_32 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_32_OFFSET                (0x48U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_32                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_32_ISP_EXP_MEAN_32_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_32_ISP_EXP_MEAN_32_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_32_ISP_EXP_MEAN_32_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_42 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_42_OFFSET                (0x4CU)
#define ISP_EXPOSURE_ISP_EXP_MEAN_42                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_42_ISP_EXP_MEAN_42_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_42_ISP_EXP_MEAN_42_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_42_ISP_EXP_MEAN_42_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_03 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_03_OFFSET                (0x50U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_03                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_03_ISP_EXP_MEAN_03_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_03_ISP_EXP_MEAN_03_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_03_ISP_EXP_MEAN_03_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_13 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_13_OFFSET                (0x54U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_13                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_13_ISP_EXP_MEAN_13_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_13_ISP_EXP_MEAN_13_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_13_ISP_EXP_MEAN_13_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_23 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_23_OFFSET                (0x58U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_23                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_23_ISP_EXP_MEAN_23_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_23_ISP_EXP_MEAN_23_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_23_ISP_EXP_MEAN_23_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_33 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_33_OFFSET                (0x5CU)
#define ISP_EXPOSURE_ISP_EXP_MEAN_33                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_33_ISP_EXP_MEAN_33_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_33_ISP_EXP_MEAN_33_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_33_ISP_EXP_MEAN_33_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_43 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_43_OFFSET                (0x60U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_43                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_43_ISP_EXP_MEAN_43_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_43_ISP_EXP_MEAN_43_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_43_ISP_EXP_MEAN_43_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_04 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_04_OFFSET                (0x64U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_04                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_04_ISP_EXP_MEAN_04_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_04_ISP_EXP_MEAN_04_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_04_ISP_EXP_MEAN_04_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_14 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_14_OFFSET                (0x68U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_14                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_14_ISP_EXP_MEAN_14_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_14_ISP_EXP_MEAN_14_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_14_ISP_EXP_MEAN_14_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_24 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_24_OFFSET                (0x6CU)
#define ISP_EXPOSURE_ISP_EXP_MEAN_24                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_24_ISP_EXP_MEAN_24_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_24_ISP_EXP_MEAN_24_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_24_ISP_EXP_MEAN_24_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_34 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_34_OFFSET                (0x70U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_34                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_34_ISP_EXP_MEAN_34_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_34_ISP_EXP_MEAN_34_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_34_ISP_EXP_MEAN_34_SHIFT) /* 0x000000FF */
/* ISP_EXP_MEAN_44 */
#define ISP_EXPOSURE_ISP_EXP_MEAN_44_OFFSET                (0x74U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_44                       (0x0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_44_ISP_EXP_MEAN_44_SHIFT (0U)
#define ISP_EXPOSURE_ISP_EXP_MEAN_44_ISP_EXP_MEAN_44_MASK  (0xFFU << ISP_EXPOSURE_ISP_EXP_MEAN_44_ISP_EXP_MEAN_44_SHIFT) /* 0x000000FF */
/****************************************ISP_BLS*****************************************/
/* ISP_BLS_CTRL */
#define ISP_BLS_ISP_BLS_CTRL_OFFSET                        (0x0U)
#define ISP_BLS_ISP_BLS_CTRL_BLS_ENABLE_SHIFT              (0U)
#define ISP_BLS_ISP_BLS_CTRL_BLS_ENABLE_MASK               (0x1U << ISP_BLS_ISP_BLS_CTRL_BLS_ENABLE_SHIFT)              /* 0x00000001 */
#define ISP_BLS_ISP_BLS_CTRL_BLS_MODE_SHIFT                (1U)
#define ISP_BLS_ISP_BLS_CTRL_BLS_MODE_MASK                 (0x1U << ISP_BLS_ISP_BLS_CTRL_BLS_MODE_SHIFT)                /* 0x00000002 */
#define ISP_BLS_ISP_BLS_CTRL_WINDOW_ENABLE_SHIFT           (2U)
#define ISP_BLS_ISP_BLS_CTRL_WINDOW_ENABLE_MASK            (0x3U << ISP_BLS_ISP_BLS_CTRL_WINDOW_ENABLE_SHIFT)           /* 0x0000000C */
/* ISP_BLS_SAMPLES */
#define ISP_BLS_ISP_BLS_SAMPLES_OFFSET                     (0x4U)
#define ISP_BLS_ISP_BLS_SAMPLES_BLS_SAMPLES_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_SAMPLES_BLS_SAMPLES_MASK           (0x1FU << ISP_BLS_ISP_BLS_SAMPLES_BLS_SAMPLES_SHIFT)         /* 0x0000001F */
/* ISP_BLS_H1_START */
#define ISP_BLS_ISP_BLS_H1_START_OFFSET                    (0x8U)
#define ISP_BLS_ISP_BLS_H1_START_BLS_H1_START_SHIFT        (0U)
#define ISP_BLS_ISP_BLS_H1_START_BLS_H1_START_MASK         (0x3FFFU << ISP_BLS_ISP_BLS_H1_START_BLS_H1_START_SHIFT)     /* 0x00003FFF */
/* ISP_BLS_H1_STOP */
#define ISP_BLS_ISP_BLS_H1_STOP_OFFSET                     (0xCU)
#define ISP_BLS_ISP_BLS_H1_STOP_BLS_H1_STOP_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_H1_STOP_BLS_H1_STOP_MASK           (0x3FFFU << ISP_BLS_ISP_BLS_H1_STOP_BLS_H1_STOP_SHIFT)       /* 0x00003FFF */
/* ISP_BLS_V1_START */
#define ISP_BLS_ISP_BLS_V1_START_OFFSET                    (0x10U)
#define ISP_BLS_ISP_BLS_V1_START_BLS_V1_START_SHIFT        (0U)
#define ISP_BLS_ISP_BLS_V1_START_BLS_V1_START_MASK         (0x3FFFU << ISP_BLS_ISP_BLS_V1_START_BLS_V1_START_SHIFT)     /* 0x00003FFF */
/* ISP_BLS_V1_STOP */
#define ISP_BLS_ISP_BLS_V1_STOP_OFFSET                     (0x14U)
#define ISP_BLS_ISP_BLS_V1_STOP_BLS_V1_STOP_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_V1_STOP_BLS_V1_STOP_MASK           (0x3FFFU << ISP_BLS_ISP_BLS_V1_STOP_BLS_V1_STOP_SHIFT)       /* 0x00003FFF */
/* ISP_BLS_H2_START */
#define ISP_BLS_ISP_BLS_H2_START_OFFSET                    (0x18U)
#define ISP_BLS_ISP_BLS_H2_START_BLS_H2_START_SHIFT        (0U)
#define ISP_BLS_ISP_BLS_H2_START_BLS_H2_START_MASK         (0x3FFFU << ISP_BLS_ISP_BLS_H2_START_BLS_H2_START_SHIFT)     /* 0x00003FFF */
/* ISP_BLS_H2_STOP */
#define ISP_BLS_ISP_BLS_H2_STOP_OFFSET                     (0x1CU)
#define ISP_BLS_ISP_BLS_H2_STOP_BLS_H2_STOP_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_H2_STOP_BLS_H2_STOP_MASK           (0x3FFFU << ISP_BLS_ISP_BLS_H2_STOP_BLS_H2_STOP_SHIFT)       /* 0x00003FFF */
/* ISP_BLS_V2_START */
#define ISP_BLS_ISP_BLS_V2_START_OFFSET                    (0x20U)
#define ISP_BLS_ISP_BLS_V2_START_BLS_V2_START_SHIFT        (0U)
#define ISP_BLS_ISP_BLS_V2_START_BLS_V2_START_MASK         (0x3FFFU << ISP_BLS_ISP_BLS_V2_START_BLS_V2_START_SHIFT)     /* 0x00003FFF */
/* ISP_BLS_V2_STOP */
#define ISP_BLS_ISP_BLS_V2_STOP_OFFSET                     (0x24U)
#define ISP_BLS_ISP_BLS_V2_STOP_BLS_V2_STOP_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_V2_STOP_BLS_V2_STOP_MASK           (0x3FFFU << ISP_BLS_ISP_BLS_V2_STOP_BLS_V2_STOP_SHIFT)       /* 0x00003FFF */
/* ISP_BLS_A_FIXED */
#define ISP_BLS_ISP_BLS_A_FIXED_OFFSET                     (0x28U)
#define ISP_BLS_ISP_BLS_A_FIXED_BLS_A_FIXED_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_A_FIXED_BLS_A_FIXED_MASK           (0x1FFFU << ISP_BLS_ISP_BLS_A_FIXED_BLS_A_FIXED_SHIFT)       /* 0x00001FFF */
/* ISP_BLS_B_FIXED */
#define ISP_BLS_ISP_BLS_B_FIXED_OFFSET                     (0x2CU)
#define ISP_BLS_ISP_BLS_B_FIXED_BLS_B_FIXED_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_B_FIXED_BLS_B_FIXED_MASK           (0x1FFFU << ISP_BLS_ISP_BLS_B_FIXED_BLS_B_FIXED_SHIFT)       /* 0x00001FFF */
/* ISP_BLS_C_FIXED */
#define ISP_BLS_ISP_BLS_C_FIXED_OFFSET                     (0x30U)
#define ISP_BLS_ISP_BLS_C_FIXED_BLS_C_FIXED_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_C_FIXED_BLS_C_FIXED_MASK           (0x1FFFU << ISP_BLS_ISP_BLS_C_FIXED_BLS_C_FIXED_SHIFT)       /* 0x00001FFF */
/* ISP_BLS_D_FIXED */
#define ISP_BLS_ISP_BLS_D_FIXED_OFFSET                     (0x34U)
#define ISP_BLS_ISP_BLS_D_FIXED_BLS_D_FIXED_SHIFT          (0U)
#define ISP_BLS_ISP_BLS_D_FIXED_BLS_D_FIXED_MASK           (0x1FFFU << ISP_BLS_ISP_BLS_D_FIXED_BLS_D_FIXED_SHIFT)       /* 0x00001FFF */
/* ISP_BLS_A_MEASURED */
#define ISP_BLS_ISP_BLS_A_MEASURED_OFFSET                  (0x38U)
#define ISP_BLS_ISP_BLS_A_MEASURED                         (0x0U)
#define ISP_BLS_ISP_BLS_A_MEASURED_BLS_A_MEASURED_SHIFT    (0U)
#define ISP_BLS_ISP_BLS_A_MEASURED_BLS_A_MEASURED_MASK     (0xFFFU << ISP_BLS_ISP_BLS_A_MEASURED_BLS_A_MEASURED_SHIFT)  /* 0x00000FFF */
/* ISP_BLS_B_MEASURED */
#define ISP_BLS_ISP_BLS_B_MEASURED_OFFSET                  (0x3CU)
#define ISP_BLS_ISP_BLS_B_MEASURED                         (0x0U)
#define ISP_BLS_ISP_BLS_B_MEASURED_BLS_B_MEASURED_SHIFT    (0U)
#define ISP_BLS_ISP_BLS_B_MEASURED_BLS_B_MEASURED_MASK     (0xFFFU << ISP_BLS_ISP_BLS_B_MEASURED_BLS_B_MEASURED_SHIFT)  /* 0x00000FFF */
/* ISP_BLS_C_MEASURED */
#define ISP_BLS_ISP_BLS_C_MEASURED_OFFSET                  (0x40U)
#define ISP_BLS_ISP_BLS_C_MEASURED                         (0x0U)
#define ISP_BLS_ISP_BLS_C_MEASURED_BLS_C_MEASURED_SHIFT    (0U)
#define ISP_BLS_ISP_BLS_C_MEASURED_BLS_C_MEASURED_MASK     (0xFFFU << ISP_BLS_ISP_BLS_C_MEASURED_BLS_C_MEASURED_SHIFT)  /* 0x00000FFF */
/* ISP_BLS_D_MEASURED */
#define ISP_BLS_ISP_BLS_D_MEASURED_OFFSET                  (0x44U)
#define ISP_BLS_ISP_BLS_D_MEASURED                         (0x0U)
#define ISP_BLS_ISP_BLS_D_MEASURED_BLS_D_MEASURED_SHIFT    (0U)
#define ISP_BLS_ISP_BLS_D_MEASURED_BLS_D_MEASURED_MASK     (0xFFFU << ISP_BLS_ISP_BLS_D_MEASURED_BLS_D_MEASURED_SHIFT)  /* 0x00000FFF */
/****************************************ISP_DPF*****************************************/
/* ISP_DPF_MODE */
#define ISP_DPF_ISP_DPF_MODE_OFFSET                        (0x0U)
#define ISP_DPF_ISP_DPF_MODE_DPF_ENABLE_SHIFT              (0U)
#define ISP_DPF_ISP_DPF_MODE_DPF_ENABLE_MASK               (0x1U << ISP_DPF_ISP_DPF_MODE_DPF_ENABLE_SHIFT)              /* 0x00000001 */
#define ISP_DPF_ISP_DPF_MODE_B_FILTER_OFF_SHIFT            (1U)
#define ISP_DPF_ISP_DPF_MODE_B_FILTER_OFF_MASK             (0x1U << ISP_DPF_ISP_DPF_MODE_B_FILTER_OFF_SHIFT)            /* 0x00000002 */
#define ISP_DPF_ISP_DPF_MODE_GB_FILTER_OFF_SHIFT           (2U)
#define ISP_DPF_ISP_DPF_MODE_GB_FILTER_OFF_MASK            (0x1U << ISP_DPF_ISP_DPF_MODE_GB_FILTER_OFF_SHIFT)           /* 0x00000004 */
#define ISP_DPF_ISP_DPF_MODE_GR_FILTER_OFF_SHIFT           (3U)
#define ISP_DPF_ISP_DPF_MODE_GR_FILTER_OFF_MASK            (0x1U << ISP_DPF_ISP_DPF_MODE_GR_FILTER_OFF_SHIFT)           /* 0x00000008 */
#define ISP_DPF_ISP_DPF_MODE_R_FILTER_OFF_SHIFT            (4U)
#define ISP_DPF_ISP_DPF_MODE_R_FILTER_OFF_MASK             (0x1U << ISP_DPF_ISP_DPF_MODE_R_FILTER_OFF_SHIFT)            /* 0x00000010 */
#define ISP_DPF_ISP_DPF_MODE_RB_FILTER_SIZE_SHIFT          (5U)
#define ISP_DPF_ISP_DPF_MODE_RB_FILTER_SIZE_MASK           (0x1U << ISP_DPF_ISP_DPF_MODE_RB_FILTER_SIZE_SHIFT)          /* 0x00000020 */
#define ISP_DPF_ISP_DPF_MODE_NLL_SEGMENTATION_SHIFT        (6U)
#define ISP_DPF_ISP_DPF_MODE_NLL_SEGMENTATION_MASK         (0x1U << ISP_DPF_ISP_DPF_MODE_NLL_SEGMENTATION_SHIFT)        /* 0x00000040 */
#define ISP_DPF_ISP_DPF_MODE_AWB_GAIN_COMP_SHIFT           (7U)
#define ISP_DPF_ISP_DPF_MODE_AWB_GAIN_COMP_MASK            (0x1U << ISP_DPF_ISP_DPF_MODE_AWB_GAIN_COMP_SHIFT)           /* 0x00000080 */
#define ISP_DPF_ISP_DPF_MODE_LSC_GAIN_COMP_SHIFT           (8U)
#define ISP_DPF_ISP_DPF_MODE_LSC_GAIN_COMP_MASK            (0x1U << ISP_DPF_ISP_DPF_MODE_LSC_GAIN_COMP_SHIFT)           /* 0x00000100 */
#define ISP_DPF_ISP_DPF_MODE_USE_NF_GAIN_SHIFT             (9U)
#define ISP_DPF_ISP_DPF_MODE_USE_NF_GAIN_MASK              (0x1U << ISP_DPF_ISP_DPF_MODE_USE_NF_GAIN_SHIFT)             /* 0x00000200 */
/* ISP_DPF_STRENGTH_R */
#define ISP_DPF_ISP_DPF_STRENGTH_R_OFFSET                  (0x4U)
#define ISP_DPF_ISP_DPF_STRENGTH_R_INV_WEIGHT_R_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_STRENGTH_R_INV_WEIGHT_R_MASK       (0xFFU << ISP_DPF_ISP_DPF_STRENGTH_R_INV_WEIGHT_R_SHIFT)     /* 0x000000FF */
/* ISP_DPF_STRENGTH_G */
#define ISP_DPF_ISP_DPF_STRENGTH_G_OFFSET                  (0x8U)
#define ISP_DPF_ISP_DPF_STRENGTH_G_INV_WEIGHT_G_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_STRENGTH_G_INV_WEIGHT_G_MASK       (0xFFU << ISP_DPF_ISP_DPF_STRENGTH_G_INV_WEIGHT_G_SHIFT)     /* 0x000000FF */
/* ISP_DPF_STRENGTH_B */
#define ISP_DPF_ISP_DPF_STRENGTH_B_OFFSET                  (0xCU)
#define ISP_DPF_ISP_DPF_STRENGTH_B_INV_WEIGHT_B_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_STRENGTH_B_INV_WEIGHT_B_MASK       (0xFFU << ISP_DPF_ISP_DPF_STRENGTH_B_INV_WEIGHT_B_SHIFT)     /* 0x000000FF */
/* ISP_DPF_S_WEIGHT_G_1_4 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_OFFSET              (0x10U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G1_SHIFT   (0U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G1_MASK    (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G1_SHIFT)  /* 0x0000001F */
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G2_SHIFT   (8U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G2_MASK    (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G2_SHIFT)  /* 0x00001F00 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G3_SHIFT   (16U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G3_MASK    (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G3_SHIFT)  /* 0x001F0000 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G4_SHIFT   (24U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G4_MASK    (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_G_1_4_S_WEIGHT_G4_SHIFT)  /* 0x1F000000 */
/* ISP_DPF_S_WEIGHT_G_5_6 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_OFFSET              (0x14U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_S_WEIGHT_G5_SHIFT   (0U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_S_WEIGHT_G5_MASK    (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_S_WEIGHT_G5_SHIFT)  /* 0x0000001F */
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_S_WEIGHT_G6_SHIFT   (8U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_S_WEIGHT_G6_MASK    (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_G_5_6_S_WEIGHT_G6_SHIFT)  /* 0x00001F00 */
/* ISP_DPF_S_WEIGHT_RB_1_4 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_OFFSET             (0x18U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB1_SHIFT (0U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB1_MASK  (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB1_SHIFT) /* 0x0000001F */
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB2_SHIFT (8U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB2_MASK  (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB2_SHIFT) /* 0x00001F00 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB3_SHIFT (16U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB3_MASK  (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB3_SHIFT) /* 0x001F0000 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB4_SHIFT (24U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB4_MASK  (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_RB_1_4_S_WEIGHT_RB4_SHIFT) /* 0x1F000000 */
/* ISP_DPF_S_WEIGHT_RB_5_6 */
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_OFFSET             (0x1CU)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_S_WEIGHT_RB5_SHIFT (0U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_S_WEIGHT_RB5_MASK  (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_S_WEIGHT_RB5_SHIFT) /* 0x0000001F */
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_S_WEIGHT_RB6_SHIFT (8U)
#define ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_S_WEIGHT_RB6_MASK  (0x1FU << ISP_DPF_ISP_DPF_S_WEIGHT_RB_5_6_S_WEIGHT_RB6_SHIFT) /* 0x00001F00 */
/* ISP_DPF_NLL_COEFF_0 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_0_OFFSET                 (0x20U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_0_NLL_COEFF_0_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_0_NLL_COEFF_0_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_0_NLL_COEFF_0_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_1 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_1_OFFSET                 (0x24U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_1_NLL_COEFF_1_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_1_NLL_COEFF_1_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_1_NLL_COEFF_1_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_2 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_2_OFFSET                 (0x28U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_2_NLL_COEFF_2_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_2_NLL_COEFF_2_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_2_NLL_COEFF_2_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_3 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_3_OFFSET                 (0x2CU)
#define ISP_DPF_ISP_DPF_NLL_COEFF_3_NLL_COEFF_3_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_3_NLL_COEFF_3_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_3_NLL_COEFF_3_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_4 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_4_OFFSET                 (0x30U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_4_NLL_COEFF_4_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_4_NLL_COEFF_4_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_4_NLL_COEFF_4_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_5 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_5_OFFSET                 (0x34U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_5_NLL_COEFF_5_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_5_NLL_COEFF_5_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_5_NLL_COEFF_5_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_6 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_6_OFFSET                 (0x38U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_6_NLL_COEFF_6_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_6_NLL_COEFF_6_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_6_NLL_COEFF_6_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_7 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_7_OFFSET                 (0x3CU)
#define ISP_DPF_ISP_DPF_NLL_COEFF_7_NLL_COEFF_7_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_7_NLL_COEFF_7_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_7_NLL_COEFF_7_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_8 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_8_OFFSET                 (0x40U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_8_NLL_COEFF_8_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_8_NLL_COEFF_8_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_8_NLL_COEFF_8_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_9 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_9_OFFSET                 (0x44U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_9_NLL_COEFF_9_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_9_NLL_COEFF_9_MASK       (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_9_NLL_COEFF_9_SHIFT)    /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_10 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_10_OFFSET                (0x48U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_10_NLL_COEFF_10_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_10_NLL_COEFF_10_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_10_NLL_COEFF_10_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_11 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_11_OFFSET                (0x4CU)
#define ISP_DPF_ISP_DPF_NLL_COEFF_11_NLL_COEFF_11_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_11_NLL_COEFF_11_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_11_NLL_COEFF_11_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_12 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_12_OFFSET                (0x50U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_12_NLL_COEFF_12_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_12_NLL_COEFF_12_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_12_NLL_COEFF_12_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_13 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_13_OFFSET                (0x54U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_13_NLL_COEFF_13_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_13_NLL_COEFF_13_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_13_NLL_COEFF_13_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_14 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_14_OFFSET                (0x58U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_14_NLL_COEFF_14_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_14_NLL_COEFF_14_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_14_NLL_COEFF_14_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_15 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_15_OFFSET                (0x5CU)
#define ISP_DPF_ISP_DPF_NLL_COEFF_15_NLL_COEFF_15_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_15_NLL_COEFF_15_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_15_NLL_COEFF_15_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NLL_COEFF_16 */
#define ISP_DPF_ISP_DPF_NLL_COEFF_16_OFFSET                (0x60U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_16_NLL_COEFF_16_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NLL_COEFF_16_NLL_COEFF_16_MASK     (0x3FFU << ISP_DPF_ISP_DPF_NLL_COEFF_16_NLL_COEFF_16_SHIFT)  /* 0x000003FF */
/* ISP_DPF_NF_GAIN_R */
#define ISP_DPF_ISP_DPF_NF_GAIN_R_OFFSET                   (0x64U)
#define ISP_DPF_ISP_DPF_NF_GAIN_R_DPF_NF_GAIN_R_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NF_GAIN_R_DPF_NF_GAIN_R_MASK       (0xFFFU << ISP_DPF_ISP_DPF_NF_GAIN_R_DPF_NF_GAIN_R_SHIFT)    /* 0x00000FFF */
/* ISP_DPF_NF_GAIN_GR */
#define ISP_DPF_ISP_DPF_NF_GAIN_GR_OFFSET                  (0x68U)
#define ISP_DPF_ISP_DPF_NF_GAIN_GR_DPF_NF_GAIN_GR_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NF_GAIN_GR_DPF_NF_GAIN_GR_MASK     (0xFFFU << ISP_DPF_ISP_DPF_NF_GAIN_GR_DPF_NF_GAIN_GR_SHIFT)  /* 0x00000FFF */
/* ISP_DPF_NF_GAIN_GB */
#define ISP_DPF_ISP_DPF_NF_GAIN_GB_OFFSET                  (0x6CU)
#define ISP_DPF_ISP_DPF_NF_GAIN_GB_DPF_NF_GAIN_GB_SHIFT    (0U)
#define ISP_DPF_ISP_DPF_NF_GAIN_GB_DPF_NF_GAIN_GB_MASK     (0xFFFU << ISP_DPF_ISP_DPF_NF_GAIN_GB_DPF_NF_GAIN_GB_SHIFT)  /* 0x00000FFF */
/* ISP_DPF_NF_GAIN_B */
#define ISP_DPF_ISP_DPF_NF_GAIN_B_OFFSET                   (0x70U)
#define ISP_DPF_ISP_DPF_NF_GAIN_B_DPF_NF_GAIN_B_SHIFT      (0U)
#define ISP_DPF_ISP_DPF_NF_GAIN_B_DPF_NF_GAIN_B_MASK       (0xFFFU << ISP_DPF_ISP_DPF_NF_GAIN_B_DPF_NF_GAIN_B_SHIFT)    /* 0x00000FFF */
/****************************************ISP_DPCC****************************************/
/* ISP_DPCC_MODE */
#define ISP_DPCC_ISP_DPCC_MODE_OFFSET                      (0x0U)
#define ISP_DPCC_ISP_DPCC_MODE_ISP_DPCC_ENABLE_SHIFT       (0U)
#define ISP_DPCC_ISP_DPCC_MODE_ISP_DPCC_ENABLE_MASK        (0x1U << ISP_DPCC_ISP_DPCC_MODE_ISP_DPCC_ENABLE_SHIFT)       /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_MODE_GRAYSCALE_MODE_SHIFT        (1U)
#define ISP_DPCC_ISP_DPCC_MODE_GRAYSCALE_MODE_MASK         (0x1U << ISP_DPCC_ISP_DPCC_MODE_GRAYSCALE_MODE_SHIFT)        /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_MODE_STAGE1_ENABLE_SHIFT         (2U)
#define ISP_DPCC_ISP_DPCC_MODE_STAGE1_ENABLE_MASK          (0x1U << ISP_DPCC_ISP_DPCC_MODE_STAGE1_ENABLE_SHIFT)         /* 0x00000004 */
/* ISP_DPCC_OUTPUT_MODE */
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_OFFSET               (0x4U)
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_INCL_GREEN_CENTER_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_INCL_GREEN_CENTER_MASK (0x1U << ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_INCL_GREEN_CENTER_SHIFT) /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_INCL_RB_CENTER_SHIFT (1U)
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_INCL_RB_CENTER_MASK (0x1U << ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_INCL_RB_CENTER_SHIFT) /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_G_3X3_SHIFT   (2U)
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_G_3X3_MASK    (0x1U << ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_G_3X3_SHIFT)   /* 0x00000004 */
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_RB_3X3_SHIFT  (3U)
#define ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_RB_3X3_MASK   (0x1U << ISP_DPCC_ISP_DPCC_OUTPUT_MODE_STAGE1_RB_3X3_SHIFT)  /* 0x00000008 */
/* ISP_DPCC_SET_USE */
#define ISP_DPCC_ISP_DPCC_SET_USE_OFFSET                   (0x8U)
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_1_SHIFT   (0U)
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_1_MASK    (0x1U << ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_1_SHIFT)   /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_2_SHIFT   (1U)
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_2_MASK    (0x1U << ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_2_SHIFT)   /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_3_SHIFT   (2U)
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_3_MASK    (0x1U << ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_SET_3_SHIFT)   /* 0x00000004 */
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_FIX_SET_SHIFT (3U)
#define ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_FIX_SET_MASK  (0x1U << ISP_DPCC_ISP_DPCC_SET_USE_STAGE1_USE_FIX_SET_SHIFT) /* 0x00000008 */
/* ISP_DPCC_METHODS_SET_1 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_OFFSET             (0xCU)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_PG_GREEN1_ENABLE_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_PG_GREEN1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_PG_GREEN1_ENABLE_SHIFT) /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_LC_GREEN1_ENABLE_SHIFT (1U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_LC_GREEN1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_LC_GREEN1_ENABLE_SHIFT) /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RO_GREEN1_ENABLE_SHIFT (2U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RO_GREEN1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_RO_GREEN1_ENABLE_SHIFT) /* 0x00000004 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RND_GREEN1_ENABLE_SHIFT (3U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RND_GREEN1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_RND_GREEN1_ENABLE_SHIFT) /* 0x00000008 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RG_GREEN1_ENABLE_SHIFT (4U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RG_GREEN1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_RG_GREEN1_ENABLE_SHIFT) /* 0x00000010 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_PG_RED_BLUE1_ENABLE_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_PG_RED_BLUE1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_PG_RED_BLUE1_ENABLE_SHIFT) /* 0x00000100 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_LC_RED_BLUE1_ENABLE_SHIFT (9U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_LC_RED_BLUE1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_LC_RED_BLUE1_ENABLE_SHIFT) /* 0x00000200 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RO_RED_BLUE1_ENABLE_SHIFT (10U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RO_RED_BLUE1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_RO_RED_BLUE1_ENABLE_SHIFT) /* 0x00000400 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RND_RED_BLUE1_ENABLE_SHIFT (11U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RND_RED_BLUE1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_RND_RED_BLUE1_ENABLE_SHIFT) /* 0x00000800 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RG_RED_BLUE1_ENABLE_SHIFT (12U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_1_RG_RED_BLUE1_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_1_RG_RED_BLUE1_ENABLE_SHIFT) /* 0x00001000 */
/* ISP_DPCC_METHODS_SET_2 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_OFFSET             (0x10U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_PG_GREEN2_ENABLE_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_PG_GREEN2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_PG_GREEN2_ENABLE_SHIFT) /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_LC_GREEN2_ENABLE_SHIFT (1U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_LC_GREEN2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_LC_GREEN2_ENABLE_SHIFT) /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RO_GREEN2_ENABLE_SHIFT (2U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RO_GREEN2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_RO_GREEN2_ENABLE_SHIFT) /* 0x00000004 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RND_GREEN2_ENABLE_SHIFT (3U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RND_GREEN2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_RND_GREEN2_ENABLE_SHIFT) /* 0x00000008 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RG_GREEN2_ENABLE_SHIFT (4U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RG_GREEN2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_RG_GREEN2_ENABLE_SHIFT) /* 0x00000010 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_PG_RED_BLUE2_ENABLE_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_PG_RED_BLUE2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_PG_RED_BLUE2_ENABLE_SHIFT) /* 0x00000100 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_LC_RED_BLUE2_ENABLE_SHIFT (9U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_LC_RED_BLUE2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_LC_RED_BLUE2_ENABLE_SHIFT) /* 0x00000200 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RO_RED_BLUE2_ENABLE_SHIFT (10U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RO_RED_BLUE2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_RO_RED_BLUE2_ENABLE_SHIFT) /* 0x00000400 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RND_RED_BLUE2_ENABLE_SHIFT (11U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RND_RED_BLUE2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_RND_RED_BLUE2_ENABLE_SHIFT) /* 0x00000800 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RG_RED_BLUE2_ENABLE_SHIFT (12U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_2_RG_RED_BLUE2_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_2_RG_RED_BLUE2_ENABLE_SHIFT) /* 0x00001000 */
/* ISP_DPCC_METHODS_SET_3 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_OFFSET             (0x14U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_PG_GREEN3_ENABLE_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_PG_GREEN3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_PG_GREEN3_ENABLE_SHIFT) /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_LC_GREEN3_ENABLE_SHIFT (1U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_LC_GREEN3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_LC_GREEN3_ENABLE_SHIFT) /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RO_GREEN3_ENABLE_SHIFT (2U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RO_GREEN3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_RO_GREEN3_ENABLE_SHIFT) /* 0x00000004 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RND_GREEN3_ENABLE_SHIFT (3U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RND_GREEN3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_RND_GREEN3_ENABLE_SHIFT) /* 0x00000008 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RG_GREEN3_ENABLE_SHIFT (4U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RG_GREEN3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_RG_GREEN3_ENABLE_SHIFT) /* 0x00000010 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_PG_RED_BLUE3_ENABLE_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_PG_RED_BLUE3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_PG_RED_BLUE3_ENABLE_SHIFT) /* 0x00000100 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_LC_RED_BLUE3_ENABLE_SHIFT (9U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_LC_RED_BLUE3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_LC_RED_BLUE3_ENABLE_SHIFT) /* 0x00000200 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RO_RED_BLUE3_ENABLE_SHIFT (10U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RO_RED_BLUE3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_RO_RED_BLUE3_ENABLE_SHIFT) /* 0x00000400 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RND_RED_BLUE3_ENABLE_SHIFT (11U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RND_RED_BLUE3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_RND_RED_BLUE3_ENABLE_SHIFT) /* 0x00000800 */
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RG_RED_BLUE3_ENABLE_SHIFT (12U)
#define ISP_DPCC_ISP_DPCC_METHODS_SET_3_RG_RED_BLUE3_ENABLE_MASK (0x1U << ISP_DPCC_ISP_DPCC_METHODS_SET_3_RG_RED_BLUE3_ENABLE_SHIFT) /* 0x00001000 */
/* ISP_DPCC_LINE_THRESH_1 */
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_1_OFFSET             (0x18U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_1_LINE_THR_1_G_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_1_LINE_THR_1_G_MASK  (0xFFU << ISP_DPCC_ISP_DPCC_LINE_THRESH_1_LINE_THR_1_G_SHIFT) /* 0x000000FF */
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_1_LINE_THR_1_RB_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_1_LINE_THR_1_RB_MASK (0xFFU << ISP_DPCC_ISP_DPCC_LINE_THRESH_1_LINE_THR_1_RB_SHIFT) /* 0x0000FF00 */
/* ISP_DPCC_LINE_MAD_FAC_1 */
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_OFFSET            (0x1CU)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_LINE_MAD_FAC_1_G_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_LINE_MAD_FAC_1_G_MASK (0x3FU << ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_LINE_MAD_FAC_1_G_SHIFT) /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_LINE_MAD_FAC_1_RB_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_LINE_MAD_FAC_1_RB_MASK (0x3FU << ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_1_LINE_MAD_FAC_1_RB_SHIFT) /* 0x00003F00 */
/* ISP_DPCC_PG_FAC_1 */
#define ISP_DPCC_ISP_DPCC_PG_FAC_1_OFFSET                  (0x20U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_1_PG_FAC_1_G_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_1_PG_FAC_1_G_MASK         (0x3FU << ISP_DPCC_ISP_DPCC_PG_FAC_1_PG_FAC_1_G_SHIFT)       /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_PG_FAC_1_PG_FAC_1_RB_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_1_PG_FAC_1_RB_MASK        (0x3FU << ISP_DPCC_ISP_DPCC_PG_FAC_1_PG_FAC_1_RB_SHIFT)      /* 0x00003F00 */
/* ISP_DPCC_RND_THRESH_1 */
#define ISP_DPCC_ISP_DPCC_RND_THRESH_1_OFFSET              (0x24U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_1_RND_THR_1_G_SHIFT   (0U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_1_RND_THR_1_G_MASK    (0xFFU << ISP_DPCC_ISP_DPCC_RND_THRESH_1_RND_THR_1_G_SHIFT)  /* 0x000000FF */
#define ISP_DPCC_ISP_DPCC_RND_THRESH_1_RND_THR_1_RB_SHIFT  (8U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_1_RND_THR_1_RB_MASK   (0xFFU << ISP_DPCC_ISP_DPCC_RND_THRESH_1_RND_THR_1_RB_SHIFT) /* 0x0000FF00 */
/* ISP_DPCC_RG_FAC_1 */
#define ISP_DPCC_ISP_DPCC_RG_FAC_1_OFFSET                  (0x28U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_1_RG_FAC_1_G_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_1_RG_FAC_1_G_MASK         (0x3FU << ISP_DPCC_ISP_DPCC_RG_FAC_1_RG_FAC_1_G_SHIFT)       /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_RG_FAC_1_RG_FAC_1_RB_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_1_RG_FAC_1_RB_MASK        (0x3FU << ISP_DPCC_ISP_DPCC_RG_FAC_1_RG_FAC_1_RB_SHIFT)      /* 0x00003F00 */
/* ISP_DPCC_LINE_THRESH_2 */
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_2_OFFSET             (0x2CU)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_2_LINE_THR_2_G_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_2_LINE_THR_2_G_MASK  (0xFFU << ISP_DPCC_ISP_DPCC_LINE_THRESH_2_LINE_THR_2_G_SHIFT) /* 0x000000FF */
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_2_LINE_THR_2_RB_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_2_LINE_THR_2_RB_MASK (0xFFU << ISP_DPCC_ISP_DPCC_LINE_THRESH_2_LINE_THR_2_RB_SHIFT) /* 0x0000FF00 */
/* ISP_DPCC_LINE_MAD_FAC_2 */
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_OFFSET            (0x30U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_LINE_MAD_FAC_2_G_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_LINE_MAD_FAC_2_G_MASK (0x3FU << ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_LINE_MAD_FAC_2_G_SHIFT) /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_LINE_MAD_FAC_2_RB_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_LINE_MAD_FAC_2_RB_MASK (0x3FU << ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_2_LINE_MAD_FAC_2_RB_SHIFT) /* 0x00003F00 */
/* ISP_DPCC_PG_FAC_2 */
#define ISP_DPCC_ISP_DPCC_PG_FAC_2_OFFSET                  (0x34U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_2_PG_FAC_2_G_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_2_PG_FAC_2_G_MASK         (0x3FU << ISP_DPCC_ISP_DPCC_PG_FAC_2_PG_FAC_2_G_SHIFT)       /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_PG_FAC_2_PG_FAC_2_RB_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_2_PG_FAC_2_RB_MASK        (0x3FU << ISP_DPCC_ISP_DPCC_PG_FAC_2_PG_FAC_2_RB_SHIFT)      /* 0x00003F00 */
/* ISP_DPCC_RND_THRESH_2 */
#define ISP_DPCC_ISP_DPCC_RND_THRESH_2_OFFSET              (0x38U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_2_RND_THR_2_G_SHIFT   (0U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_2_RND_THR_2_G_MASK    (0xFFU << ISP_DPCC_ISP_DPCC_RND_THRESH_2_RND_THR_2_G_SHIFT)  /* 0x000000FF */
#define ISP_DPCC_ISP_DPCC_RND_THRESH_2_RND_THR_2_RB_SHIFT  (8U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_2_RND_THR_2_RB_MASK   (0xFFU << ISP_DPCC_ISP_DPCC_RND_THRESH_2_RND_THR_2_RB_SHIFT) /* 0x0000FF00 */
/* ISP_DPCC_RG_FAC_2 */
#define ISP_DPCC_ISP_DPCC_RG_FAC_2_OFFSET                  (0x3CU)
#define ISP_DPCC_ISP_DPCC_RG_FAC_2_RG_FAC_2_G_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_2_RG_FAC_2_G_MASK         (0x3FU << ISP_DPCC_ISP_DPCC_RG_FAC_2_RG_FAC_2_G_SHIFT)       /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_RG_FAC_2_RG_FAC_2_RB_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_2_RG_FAC_2_RB_MASK        (0x3FU << ISP_DPCC_ISP_DPCC_RG_FAC_2_RG_FAC_2_RB_SHIFT)      /* 0x00003F00 */
/* ISP_DPCC_LINE_THRESH_3 */
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_3_OFFSET             (0x40U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_3_LINE_THR_3_G_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_3_LINE_THR_3_G_MASK  (0xFFU << ISP_DPCC_ISP_DPCC_LINE_THRESH_3_LINE_THR_3_G_SHIFT) /* 0x000000FF */
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_3_LINE_THR_3_RB_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_LINE_THRESH_3_LINE_THR_3_RB_MASK (0xFFU << ISP_DPCC_ISP_DPCC_LINE_THRESH_3_LINE_THR_3_RB_SHIFT) /* 0x0000FF00 */
/* ISP_DPCC_LINE_MAD_FAC_3 */
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_OFFSET            (0x44U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_LINE_MAD_FAC_3_G_SHIFT (0U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_LINE_MAD_FAC_3_G_MASK (0x3FU << ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_LINE_MAD_FAC_3_G_SHIFT) /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_LINE_MAD_FAC_3_RB_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_LINE_MAD_FAC_3_RB_MASK (0x3FU << ISP_DPCC_ISP_DPCC_LINE_MAD_FAC_3_LINE_MAD_FAC_3_RB_SHIFT) /* 0x00003F00 */
/* ISP_DPCC_PG_FAC_3 */
#define ISP_DPCC_ISP_DPCC_PG_FAC_3_OFFSET                  (0x48U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_3_PG_FAC_3_G_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_3_PG_FAC_3_G_MASK         (0x3FU << ISP_DPCC_ISP_DPCC_PG_FAC_3_PG_FAC_3_G_SHIFT)       /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_PG_FAC_3_PG_FAC_3_RB_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_PG_FAC_3_PG_FAC_3_RB_MASK        (0x3FU << ISP_DPCC_ISP_DPCC_PG_FAC_3_PG_FAC_3_RB_SHIFT)      /* 0x00003F00 */
/* ISP_DPCC_RND_THRESH_3 */
#define ISP_DPCC_ISP_DPCC_RND_THRESH_3_OFFSET              (0x4CU)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_3_RND_THR_3_G_SHIFT   (0U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_3_RND_THR_3_G_MASK    (0xFFU << ISP_DPCC_ISP_DPCC_RND_THRESH_3_RND_THR_3_G_SHIFT)  /* 0x000000FF */
#define ISP_DPCC_ISP_DPCC_RND_THRESH_3_RND_THR_3_RB_SHIFT  (8U)
#define ISP_DPCC_ISP_DPCC_RND_THRESH_3_RND_THR_3_RB_MASK   (0xFFU << ISP_DPCC_ISP_DPCC_RND_THRESH_3_RND_THR_3_RB_SHIFT) /* 0x0000FF00 */
/* ISP_DPCC_RG_FAC_3 */
#define ISP_DPCC_ISP_DPCC_RG_FAC_3_OFFSET                  (0x50U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_3_RG_FAC_3_G_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_3_RG_FAC_3_G_MASK         (0x3FU << ISP_DPCC_ISP_DPCC_RG_FAC_3_RG_FAC_3_G_SHIFT)       /* 0x0000003F */
#define ISP_DPCC_ISP_DPCC_RG_FAC_3_RG_FAC_3_RB_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_RG_FAC_3_RG_FAC_3_RB_MASK        (0x3FU << ISP_DPCC_ISP_DPCC_RG_FAC_3_RG_FAC_3_RB_SHIFT)      /* 0x00003F00 */
/* ISP_DPCC_RO_LIMITS */
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_OFFSET                 (0x54U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_1_G_SHIFT       (0U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_1_G_MASK        (0x3U << ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_1_G_SHIFT)       /* 0x00000003 */
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_1_RB_SHIFT      (2U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_1_RB_MASK       (0x3U << ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_1_RB_SHIFT)      /* 0x0000000C */
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_2_G_SHIFT       (4U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_2_G_MASK        (0x3U << ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_2_G_SHIFT)       /* 0x00000030 */
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_2_RB_SHIFT      (6U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_2_RB_MASK       (0x3U << ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_2_RB_SHIFT)      /* 0x000000C0 */
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_3_G_SHIFT       (8U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_3_G_MASK        (0x3U << ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_3_G_SHIFT)       /* 0x00000300 */
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_3_RB_SHIFT      (10U)
#define ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_3_RB_MASK       (0x3U << ISP_DPCC_ISP_DPCC_RO_LIMITS_RO_LIM_3_RB_SHIFT)      /* 0x00000C00 */
/* ISP_DPCC_RND_OFFS */
#define ISP_DPCC_ISP_DPCC_RND_OFFS_OFFSET                  (0x58U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_1_G_SHIFT      (0U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_1_G_MASK       (0x3U << ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_1_G_SHIFT)      /* 0x00000003 */
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_1_RB_SHIFT     (2U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_1_RB_MASK      (0x3U << ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_1_RB_SHIFT)     /* 0x0000000C */
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_2_G_SHIFT      (4U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_2_G_MASK       (0x3U << ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_2_G_SHIFT)      /* 0x00000030 */
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_2_RB_SHIFT     (6U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_2_RB_MASK      (0x3U << ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_2_RB_SHIFT)     /* 0x000000C0 */
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_3_G_SHIFT      (8U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_3_G_MASK       (0x3U << ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_3_G_SHIFT)      /* 0x00000300 */
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_3_RB_SHIFT     (10U)
#define ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_3_RB_MASK      (0x3U << ISP_DPCC_ISP_DPCC_RND_OFFS_RND_OFFS_3_RB_SHIFT)     /* 0x00000C00 */
/* ISP_DPCC_BPT_CTRL */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_OFFSET                  (0x5CU)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_DET_EN_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_DET_EN_MASK         (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_DET_EN_SHIFT)        /* 0x00000001 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_COR_EN_SHIFT        (1U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_COR_EN_MASK         (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_COR_EN_SHIFT)        /* 0x00000002 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_1_SHIFT     (4U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_1_MASK      (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_1_SHIFT)     /* 0x00000010 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_2_SHIFT     (5U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_2_MASK      (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_2_SHIFT)     /* 0x00000020 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_3_SHIFT     (6U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_3_MASK      (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_SET_3_SHIFT)     /* 0x00000040 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_FIX_SET_SHIFT   (7U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_FIX_SET_MASK    (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_USE_FIX_SET_SHIFT)   /* 0x00000080 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_INCL_GREEN_CENTER_SHIFT (8U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_INCL_GREEN_CENTER_MASK (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_INCL_GREEN_CENTER_SHIFT) /* 0x00000100 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_INCL_RB_CENTER_SHIFT (9U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_INCL_RB_CENTER_MASK (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_INCL_RB_CENTER_SHIFT) /* 0x00000200 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_G_3X3_SHIFT         (10U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_G_3X3_MASK          (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_G_3X3_SHIFT)         /* 0x00000400 */
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_RB_3X3_SHIFT        (11U)
#define ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_RB_3X3_MASK         (0x1U << ISP_DPCC_ISP_DPCC_BPT_CTRL_BPT_RB_3X3_SHIFT)        /* 0x00000800 */
/* ISP_DPCC_BPT_NUMBER */
#define ISP_DPCC_ISP_DPCC_BPT_NUMBER_OFFSET                (0x60U)
#define ISP_DPCC_ISP_DPCC_BPT_NUMBER_BP_NUMBER_SHIFT       (0U)
#define ISP_DPCC_ISP_DPCC_BPT_NUMBER_BP_NUMBER_MASK        (0xFFFU << ISP_DPCC_ISP_DPCC_BPT_NUMBER_BP_NUMBER_SHIFT)     /* 0x00000FFF */
/* ISP_DPCC_BPT_ADDR */
#define ISP_DPCC_ISP_DPCC_BPT_ADDR_OFFSET                  (0x64U)
#define ISP_DPCC_ISP_DPCC_BPT_ADDR_BP_TABLE_ADDR_SHIFT     (0U)
#define ISP_DPCC_ISP_DPCC_BPT_ADDR_BP_TABLE_ADDR_MASK      (0x7FFU << ISP_DPCC_ISP_DPCC_BPT_ADDR_BP_TABLE_ADDR_SHIFT)   /* 0x000007FF */
/* ISP_DPCC_BPT_DATA */
#define ISP_DPCC_ISP_DPCC_BPT_DATA_OFFSET                  (0x68U)
#define ISP_DPCC_ISP_DPCC_BPT_DATA_BPT_H_ADDR_SHIFT        (0U)
#define ISP_DPCC_ISP_DPCC_BPT_DATA_BPT_H_ADDR_MASK         (0x1FFFU << ISP_DPCC_ISP_DPCC_BPT_DATA_BPT_H_ADDR_SHIFT)     /* 0x00001FFF */
#define ISP_DPCC_ISP_DPCC_BPT_DATA_BPT_V_ADDR_SHIFT        (16U)
#define ISP_DPCC_ISP_DPCC_BPT_DATA_BPT_V_ADDR_MASK         (0xFFFU << ISP_DPCC_ISP_DPCC_BPT_DATA_BPT_V_ADDR_SHIFT)      /* 0x0FFF0000 */
/****************************************ISP_WDR*****************************************/
/* ISP_WDR_CTRL */
#define ISP_WDR_ISP_WDR_CTRL_OFFSET                        (0x0U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_ENABLE_SHIFT              (0U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_ENABLE_MASK               (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_ENABLE_SHIFT)              /* 0x00000001 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_COLOR_SPACE_SELECT_SHIFT  (1U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_COLOR_SPACE_SELECT_MASK   (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_COLOR_SPACE_SELECT_SHIFT)  /* 0x00000002 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_CR_MAPPING_DISABLE_SHIFT  (2U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_CR_MAPPING_DISABLE_MASK   (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_CR_MAPPING_DISABLE_SHIFT)  /* 0x00000004 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_USE_IREF_SHIFT            (3U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_USE_IREF_MASK             (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_USE_IREF_SHIFT)            /* 0x00000008 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_USE_Y9_8_SHIFT            (4U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_USE_Y9_8_MASK             (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_USE_Y9_8_SHIFT)            /* 0x00000010 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_USE_RGB7_8_SHIFT          (5U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_USE_RGB7_8_MASK           (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_USE_RGB7_8_SHIFT)          /* 0x00000020 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_DISABLE_TRANSIENT_SHIFT   (6U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_DISABLE_TRANSIENT_MASK    (0x1U << ISP_WDR_ISP_WDR_CTRL_WDR_DISABLE_TRANSIENT_SHIFT)   /* 0x00000040 */
#define ISP_WDR_ISP_WDR_CTRL_WDR_RGB_FACTOR_SHIFT          (8U)
#define ISP_WDR_ISP_WDR_CTRL_WDR_RGB_FACTOR_MASK           (0xFU << ISP_WDR_ISP_WDR_CTRL_WDR_RGB_FACTOR_SHIFT)          /* 0x00000F00 */
/* ISP_WDR_TONECURVE_1 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_OFFSET                 (0x4U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY1_SHIFT          (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY1_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY1_SHIFT)          /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY2_SHIFT          (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY2_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY2_SHIFT)          /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY3_SHIFT          (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY3_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY3_SHIFT)          /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY4_SHIFT          (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY4_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY4_SHIFT)          /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY5_SHIFT          (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY5_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY5_SHIFT)          /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY6_SHIFT          (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY6_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY6_SHIFT)          /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY7_SHIFT          (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY7_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY7_SHIFT)          /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY8_SHIFT          (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY8_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_WDR_DY8_SHIFT)          /* 0x70000000 */
/* ISP_WDR_TONECURVE_2 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_OFFSET                 (0x8U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY9_SHIFT          (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY9_MASK           (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY9_SHIFT)          /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY10_SHIFT         (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY10_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY10_SHIFT)         /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY11_SHIFT         (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY11_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY11_SHIFT)         /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY12_SHIFT         (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY12_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY12_SHIFT)         /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY13_SHIFT         (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY13_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY13_SHIFT)         /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY14_SHIFT         (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY14_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY14_SHIFT)         /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY15_SHIFT         (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY15_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY15_SHIFT)         /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY16_SHIFT         (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY16_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_WDR_DY16_SHIFT)         /* 0x70000000 */
/* ISP_WDR_TONECURVE_3 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_OFFSET                 (0xCU)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY17_SHIFT         (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY17_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY17_SHIFT)         /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY18_SHIFT         (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY18_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY18_SHIFT)         /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY19_SHIFT         (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY19_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY19_SHIFT)         /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY20_SHIFT         (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY20_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY20_SHIFT)         /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY21_SHIFT         (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY21_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY21_SHIFT)         /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY22_SHIFT         (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY22_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY22_SHIFT)         /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY23_SHIFT         (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY23_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY23_SHIFT)         /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY24_SHIFT         (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY24_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_WDR_DY24_SHIFT)         /* 0x70000000 */
/* ISP_WDR_TONECURVE_4 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_OFFSET                 (0x10U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY25_SHIFT         (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY25_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY25_SHIFT)         /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY26_SHIFT         (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY26_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY26_SHIFT)         /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY27_SHIFT         (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY27_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY27_SHIFT)         /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY28_SHIFT         (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY28_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY28_SHIFT)         /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY29_SHIFT         (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY29_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY29_SHIFT)         /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY30_SHIFT         (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY30_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY30_SHIFT)         /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY31_SHIFT         (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY31_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY31_SHIFT)         /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY32_SHIFT         (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY32_MASK          (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_WDR_DY32_SHIFT)         /* 0x70000000 */
/* ISP_WDR_TONECURVE_YM_0 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_0_OFFSET              (0x14U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_0_TONECURVE_YM_0_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_0_TONECURVE_YM_0_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_0_TONECURVE_YM_0_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_1 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_1_OFFSET              (0x18U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_1_TONECURVE_YM_1_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_1_TONECURVE_YM_1_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_1_TONECURVE_YM_1_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_2 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_2_OFFSET              (0x1CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_2_TONECURVE_YM_2_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_2_TONECURVE_YM_2_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_2_TONECURVE_YM_2_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_3 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_3_OFFSET              (0x20U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_3_TONECURVE_YM_3_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_3_TONECURVE_YM_3_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_3_TONECURVE_YM_3_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_4 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_4_OFFSET              (0x24U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_4_TONECURVE_YM_4_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_4_TONECURVE_YM_4_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_4_TONECURVE_YM_4_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_5 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_5_OFFSET              (0x28U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_5_TONECURVE_YM_5_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_5_TONECURVE_YM_5_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_5_TONECURVE_YM_5_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_6 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_6_OFFSET              (0x2CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_6_TONECURVE_YM_6_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_6_TONECURVE_YM_6_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_6_TONECURVE_YM_6_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_7 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_7_OFFSET              (0x30U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_7_TONECURVE_YM_7_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_7_TONECURVE_YM_7_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_7_TONECURVE_YM_7_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_8 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_8_OFFSET              (0x34U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_8_TONECURVE_YM_8_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_8_TONECURVE_YM_8_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_8_TONECURVE_YM_8_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_9 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_9_OFFSET              (0x38U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_9_TONECURVE_YM_9_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_9_TONECURVE_YM_9_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_9_TONECURVE_YM_9_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_10 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_10_OFFSET             (0x3CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_10_TONECURVE_YM_10_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_10_TONECURVE_YM_10_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_10_TONECURVE_YM_10_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_11 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_11_OFFSET             (0x40U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_11_TONECURVE_YM_11_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_11_TONECURVE_YM_11_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_11_TONECURVE_YM_11_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_12 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_12_OFFSET             (0x44U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_12_TONECURVE_YM_12_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_12_TONECURVE_YM_12_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_12_TONECURVE_YM_12_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_13 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_13_OFFSET             (0x48U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_13_TONECURVE_YM_13_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_13_TONECURVE_YM_13_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_13_TONECURVE_YM_13_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_14 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_14_OFFSET             (0x4CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_14_TONECURVE_YM_14_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_14_TONECURVE_YM_14_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_14_TONECURVE_YM_14_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_15 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_15_OFFSET             (0x50U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_15_TONECURVE_YM_15_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_15_TONECURVE_YM_15_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_15_TONECURVE_YM_15_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_16 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_16_OFFSET             (0x54U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_16_TONECURVE_YM_16_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_16_TONECURVE_YM_16_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_16_TONECURVE_YM_16_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_17 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_17_OFFSET             (0x58U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_17_TONECURVE_YM_17_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_17_TONECURVE_YM_17_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_17_TONECURVE_YM_17_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_18 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_18_OFFSET             (0x5CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_18_TONECURVE_YM_18_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_18_TONECURVE_YM_18_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_18_TONECURVE_YM_18_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_19 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_19_OFFSET             (0x60U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_19_TONECURVE_YM_19_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_19_TONECURVE_YM_19_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_19_TONECURVE_YM_19_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_20 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_20_OFFSET             (0x64U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_20_TONECURVE_YM_20_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_20_TONECURVE_YM_20_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_20_TONECURVE_YM_20_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_21 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_21_OFFSET             (0x68U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_21_TONECURVE_YM_21_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_21_TONECURVE_YM_21_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_21_TONECURVE_YM_21_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_22 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_22_OFFSET             (0x6CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_22_TONECURVE_YM_22_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_22_TONECURVE_YM_22_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_22_TONECURVE_YM_22_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_23 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_23_OFFSET             (0x70U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_23_TONECURVE_YM_23_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_23_TONECURVE_YM_23_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_23_TONECURVE_YM_23_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_24 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_24_OFFSET             (0x74U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_24_TONECURVE_YM_24_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_24_TONECURVE_YM_24_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_24_TONECURVE_YM_24_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_25 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_25_OFFSET             (0x78U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_25_TONECURVE_YM_25_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_25_TONECURVE_YM_25_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_25_TONECURVE_YM_25_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_26 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_26_OFFSET             (0x7CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_26_TONECURVE_YM_26_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_26_TONECURVE_YM_26_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_26_TONECURVE_YM_26_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_27 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_27_OFFSET             (0x80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_27_TONECURVE_YM_27_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_27_TONECURVE_YM_27_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_27_TONECURVE_YM_27_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_28 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_28_OFFSET             (0x84U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_28_TONECURVE_YM_28_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_28_TONECURVE_YM_28_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_28_TONECURVE_YM_28_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_29 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_29_OFFSET             (0x88U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_29_TONECURVE_YM_29_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_29_TONECURVE_YM_29_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_29_TONECURVE_YM_29_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_30 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_30_OFFSET             (0x8CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_30_TONECURVE_YM_30_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_30_TONECURVE_YM_30_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_30_TONECURVE_YM_30_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_31 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_31_OFFSET             (0x90U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_31_TONECURVE_YM_31_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_31_TONECURVE_YM_31_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_31_TONECURVE_YM_31_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_32 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_32_OFFSET             (0x94U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_32_TONECURVE_YM_32_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_32_TONECURVE_YM_32_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_32_TONECURVE_YM_32_SHIFT) /* 0x00001FFF */
/* ISP_WDR_OFFSET */
#define ISP_WDR_ISP_WDR_OFFSET_OFFSET                      (0x98U)
#define ISP_WDR_ISP_WDR_OFFSET_RGB_OFFSET_SHIFT            (0U)
#define ISP_WDR_ISP_WDR_OFFSET_RGB_OFFSET_MASK             (0xFFFU << ISP_WDR_ISP_WDR_OFFSET_RGB_OFFSET_SHIFT)          /* 0x00000FFF */
#define ISP_WDR_ISP_WDR_OFFSET_LUM_OFFSET_SHIFT            (16U)
#define ISP_WDR_ISP_WDR_OFFSET_LUM_OFFSET_MASK             (0xFFFU << ISP_WDR_ISP_WDR_OFFSET_LUM_OFFSET_SHIFT)          /* 0x0FFF0000 */
/* ISP_WDR_DELTAMIN */
#define ISP_WDR_ISP_WDR_DELTAMIN_OFFSET                    (0x9CU)
#define ISP_WDR_ISP_WDR_DELTAMIN_DMIN_THRESH_SHIFT         (0U)
#define ISP_WDR_ISP_WDR_DELTAMIN_DMIN_THRESH_MASK          (0xFFFU << ISP_WDR_ISP_WDR_DELTAMIN_DMIN_THRESH_SHIFT)       /* 0x00000FFF */
#define ISP_WDR_ISP_WDR_DELTAMIN_DMIN_STRENGTH_SHIFT       (16U)
#define ISP_WDR_ISP_WDR_DELTAMIN_DMIN_STRENGTH_MASK        (0x1FU << ISP_WDR_ISP_WDR_DELTAMIN_DMIN_STRENGTH_SHIFT)      /* 0x001F0000 */
/* ISP_WDR_TONECURVE_1_SHD */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_OFFSET             (0xA0U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD                    (0x44444444U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY1_SHIFT      (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY1_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY1_SHIFT)      /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY2_SHIFT      (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY2_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY2_SHIFT)      /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY3_SHIFT      (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY3_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY3_SHIFT)      /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY4_SHIFT      (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY4_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY4_SHIFT)      /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY5_SHIFT      (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY5_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY5_SHIFT)      /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY6_SHIFT      (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY6_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY6_SHIFT)      /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY7_SHIFT      (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY7_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY7_SHIFT)      /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY8_SHIFT      (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY8_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_1_SHD_WDR_DY8_SHIFT)      /* 0x70000000 */
/* ISP_WDR_TONECURVE_2_SHD */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_OFFSET             (0xA4U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD                    (0x44444444U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY9_SHIFT      (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY9_MASK       (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY9_SHIFT)      /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY10_SHIFT     (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY10_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY10_SHIFT)     /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY11_SHIFT     (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY11_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY11_SHIFT)     /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY12_SHIFT     (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY12_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY12_SHIFT)     /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY13_SHIFT     (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY13_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY13_SHIFT)     /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY14_SHIFT     (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY14_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY14_SHIFT)     /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY15_SHIFT     (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY15_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY15_SHIFT)     /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY16_SHIFT     (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY16_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_2_SHD_WDR_DY16_SHIFT)     /* 0x70000000 */
/* ISP_WDR_TONECURVE_3_SHD */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_OFFSET             (0xA8U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD                    (0x44444444U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY17_SHIFT     (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY17_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY17_SHIFT)     /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY18_SHIFT     (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY18_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY18_SHIFT)     /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY19_SHIFT     (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY19_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY19_SHIFT)     /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY20_SHIFT     (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY20_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY20_SHIFT)     /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY21_SHIFT     (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY21_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY21_SHIFT)     /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY22_SHIFT     (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY22_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY22_SHIFT)     /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY23_SHIFT     (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY23_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY23_SHIFT)     /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY24_SHIFT     (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY24_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_3_SHD_WDR_DY24_SHIFT)     /* 0x70000000 */
/* ISP_WDR_TONECURVE_4_SHD */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_OFFSET             (0xACU)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD                    (0x44444444U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY25_SHIFT     (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY25_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY25_SHIFT)     /* 0x00000007 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY26_SHIFT     (4U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY26_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY26_SHIFT)     /* 0x00000070 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY27_SHIFT     (8U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY27_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY27_SHIFT)     /* 0x00000700 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY28_SHIFT     (12U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY28_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY28_SHIFT)     /* 0x00007000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY29_SHIFT     (16U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY29_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY29_SHIFT)     /* 0x00070000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY30_SHIFT     (20U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY30_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY30_SHIFT)     /* 0x00700000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY31_SHIFT     (24U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY31_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY31_SHIFT)     /* 0x07000000 */
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY32_SHIFT     (28U)
#define ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY32_MASK      (0x7U << ISP_WDR_ISP_WDR_TONECURVE_4_SHD_WDR_DY32_SHIFT)     /* 0x70000000 */
/* ISP_WDR_TONECURVE_YM_SHD_0 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_0_OFFSET          (0xB0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_0                 (0x0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_0_TONECURVE_YM_0_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_0_TONECURVE_YM_0_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_0_TONECURVE_YM_0_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_1 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_1_OFFSET          (0xB4U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_1                 (0x80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_1_TONECURVE_YM_1_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_1_TONECURVE_YM_1_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_1_TONECURVE_YM_1_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_2 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_2_OFFSET          (0xB8U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_2                 (0x100U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_2_TONECURVE_YM_2_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_2_TONECURVE_YM_2_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_2_TONECURVE_YM_2_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_3 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_3_OFFSET          (0xBCU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_3                 (0x180U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_3_TONECURVE_YM_3_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_3_TONECURVE_YM_3_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_3_TONECURVE_YM_3_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_4 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_4_OFFSET          (0xC0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_4                 (0x200U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_4_TONECURVE_YM_4_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_4_TONECURVE_YM_4_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_4_TONECURVE_YM_4_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_5 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_5_OFFSET          (0xC4U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_5                 (0x280U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_5_TONECURVE_YM_5_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_5_TONECURVE_YM_5_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_5_TONECURVE_YM_5_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_6 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_6_OFFSET          (0xC8U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_6                 (0x300U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_6_TONECURVE_YM_6_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_6_TONECURVE_YM_6_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_6_TONECURVE_YM_6_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_7 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_7_OFFSET          (0xCCU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_7                 (0x380U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_7_TONECURVE_YM_7_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_7_TONECURVE_YM_7_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_7_TONECURVE_YM_7_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_8 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_8_OFFSET          (0xD0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_8                 (0x400U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_8_TONECURVE_YM_8_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_8_TONECURVE_YM_8_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_8_TONECURVE_YM_8_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_9 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_9_OFFSET          (0xD4U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_9                 (0x480U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_9_TONECURVE_YM_9_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_9_TONECURVE_YM_9_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_9_TONECURVE_YM_9_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_10 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_10_OFFSET         (0xD8U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_10                (0x500U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_10_TONECURVE_YM_10_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_10_TONECURVE_YM_10_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_10_TONECURVE_YM_10_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_11 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_11_OFFSET         (0xDCU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_11                (0x580U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_11_TONECURVE_YM_11_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_11_TONECURVE_YM_11_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_11_TONECURVE_YM_11_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_12 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_12_OFFSET         (0xE0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_12                (0x600U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_12_TONECURVE_YM_12_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_12_TONECURVE_YM_12_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_12_TONECURVE_YM_12_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_13 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_13_OFFSET         (0xE4U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_13                (0x680U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_13_TONECURVE_YM_13_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_13_TONECURVE_YM_13_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_13_TONECURVE_YM_13_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_14 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_14_OFFSET         (0xE8U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_14                (0x700U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_14_TONECURVE_YM_14_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_14_TONECURVE_YM_14_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_14_TONECURVE_YM_14_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_15 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_15_OFFSET         (0xECU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_15                (0x780U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_15_TONECURVE_YM_15_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_15_TONECURVE_YM_15_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_15_TONECURVE_YM_15_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_16 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_16_OFFSET         (0xF0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_16                (0x800U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_16_TONECURVE_YM_16_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_16_TONECURVE_YM_16_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_16_TONECURVE_YM_16_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_17 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_17_OFFSET         (0xF4U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_17                (0x880U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_17_TONECURVE_YM_17_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_17_TONECURVE_YM_17_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_17_TONECURVE_YM_17_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_18 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_18_OFFSET         (0xF8U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_18                (0x900U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_18_TONECURVE_YM_18_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_18_TONECURVE_YM_18_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_18_TONECURVE_YM_18_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_19 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_19_OFFSET         (0xFCU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_19                (0x980U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_19_TONECURVE_YM_19_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_19_TONECURVE_YM_19_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_19_TONECURVE_YM_19_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_20 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_20_OFFSET         (0x100U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_20                (0xA00U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_20_TONECURVE_YM_20_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_20_TONECURVE_YM_20_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_20_TONECURVE_YM_20_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_21 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_21_OFFSET         (0x104U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_21                (0xA80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_21_TONECURVE_YM_21_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_21_TONECURVE_YM_21_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_21_TONECURVE_YM_21_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_22 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_22_OFFSET         (0x108U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_22                (0xB00U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_22_TONECURVE_YM_22_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_22_TONECURVE_YM_22_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_22_TONECURVE_YM_22_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_23 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_23_OFFSET         (0x10CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_23                (0xB80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_23_TONECURVE_YM_23_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_23_TONECURVE_YM_23_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_23_TONECURVE_YM_23_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_24 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_24_OFFSET         (0x110U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_24                (0xC00U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_24_TONECURVE_YM_24_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_24_TONECURVE_YM_24_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_24_TONECURVE_YM_24_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_25 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_25_OFFSET         (0x114U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_25                (0xC80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_25_TONECURVE_YM_25_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_25_TONECURVE_YM_25_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_25_TONECURVE_YM_25_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_26 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_26_OFFSET         (0x118U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_26                (0xD00U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_26_TONECURVE_YM_26_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_26_TONECURVE_YM_26_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_26_TONECURVE_YM_26_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_27 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_27_OFFSET         (0x11CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_27                (0xD80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_27_TONECURVE_YM_27_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_27_TONECURVE_YM_27_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_27_TONECURVE_YM_27_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_28 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_28_OFFSET         (0x120U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_28                (0xE00U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_28_TONECURVE_YM_28_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_28_TONECURVE_YM_28_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_28_TONECURVE_YM_28_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_29 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_29_OFFSET         (0x124U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_29                (0xE80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_29_TONECURVE_YM_29_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_29_TONECURVE_YM_29_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_29_TONECURVE_YM_29_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_30 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_30_OFFSET         (0x128U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_30                (0xF00U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_30_TONECURVE_YM_30_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_30_TONECURVE_YM_30_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_30_TONECURVE_YM_30_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_31 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_31_OFFSET         (0x12CU)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_31                (0xF80U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_31_TONECURVE_YM_31_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_31_TONECURVE_YM_31_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_31_TONECURVE_YM_31_SHD_SHIFT) /* 0x00001FFF */
/* ISP_WDR_TONECURVE_YM_SHD_32 */
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_32_OFFSET         (0x130U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_32                (0x1000U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_32_TONECURVE_YM_32_SHD_SHIFT (0U)
#define ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_32_TONECURVE_YM_32_SHD_MASK (0x1FFFU << ISP_WDR_ISP_WDR_TONECURVE_YM_SHD_32_TONECURVE_YM_32_SHD_SHIFT) /* 0x00001FFF */
/*****************************************TIMER******************************************/
/* LOAD_COUNT0 */
#define TIMER_LOAD_COUNT0_OFFSET                           (0x0U)
#define TIMER_LOAD_COUNT0_TIMER_LOAD_COUNT0_SHIFT          (0U)
#define TIMER_LOAD_COUNT0_TIMER_LOAD_COUNT0_MASK           (0xFFFFFFFFU << TIMER_LOAD_COUNT0_TIMER_LOAD_COUNT0_SHIFT)   /* 0xFFFFFFFF */
/* LOAD_COUNT1 */
#define TIMER_LOAD_COUNT1_OFFSET                           (0x4U)
#define TIMER_LOAD_COUNT1_TIMER_LOAD_COUNT1_SHIFT          (0U)
#define TIMER_LOAD_COUNT1_TIMER_LOAD_COUNT1_MASK           (0xFFFFFFFFU << TIMER_LOAD_COUNT1_TIMER_LOAD_COUNT1_SHIFT)   /* 0xFFFFFFFF */
/* CURRENT_VALUE0 */
#define TIMER_CURRENT_VALUE0_OFFSET                        (0x8U)
#define TIMER_CURRENT_VALUE0                               (0x0U)
#define TIMER_CURRENT_VALUE0_TIMER_CURRENT_VALUE0_SHIFT    (0U)
#define TIMER_CURRENT_VALUE0_TIMER_CURRENT_VALUE0_MASK     (0xFFFFFFFFU << TIMER_CURRENT_VALUE0_TIMER_CURRENT_VALUE0_SHIFT) /* 0xFFFFFFFF */
/* CURRENT_VALUE1 */
#define TIMER_CURRENT_VALUE1_OFFSET                        (0xCU)
#define TIMER_CURRENT_VALUE1                               (0x0U)
#define TIMER_CURRENT_VALUE1_TIMER_CURRENT_VALUE_SHIFT     (0U)
#define TIMER_CURRENT_VALUE1_TIMER_CURRENT_VALUE_MASK      (0xFFFFFFFFU << TIMER_CURRENT_VALUE1_TIMER_CURRENT_VALUE_SHIFT) /* 0xFFFFFFFF */
/* CONTROLREG */
#define TIMER_CONTROLREG_OFFSET                            (0x10U)
#define TIMER_CONTROLREG_TIMER_ENABLE_SHIFT                (0U)
#define TIMER_CONTROLREG_TIMER_ENABLE_MASK                 (0x1U << TIMER_CONTROLREG_TIMER_ENABLE_SHIFT)                /* 0x00000001 */
#define TIMER_CONTROLREG_TIMER_MODE_SHIFT                  (1U)
#define TIMER_CONTROLREG_TIMER_MODE_MASK                   (0x1U << TIMER_CONTROLREG_TIMER_MODE_SHIFT)                  /* 0x00000002 */
#define TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT              (2U)
#define TIMER_CONTROLREG_TIMER_INT_MASK_MASK               (0x1U << TIMER_CONTROLREG_TIMER_INT_MASK_SHIFT)              /* 0x00000004 */
/* INTSTATUS */
#define TIMER_INTSTATUS_OFFSET                             (0x18U)
#define TIMER_INTSTATUS_INTSTATUS_SHIFT                    (0U)
#define TIMER_INTSTATUS_INTSTATUS_MASK                     (0x1U << TIMER_INTSTATUS_INTSTATUS_SHIFT)                    /* 0x00000001 */
/*************************************BUFFER_MANAGER*************************************/
/* WCH_MBLK_CON0 */
#define BUFFER_MANAGER_WCH_MBLK_CON0_OFFSET                (0x0U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_ENABLE_SHIFT    (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_ENABLE_MASK     (0x1U << BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_ENABLE_SHIFT)    /* 0x00000001 */
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF0_FULL_HARDCLR_EN_SHIFT (1U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF0_FULL_HARDCLR_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF0_FULL_HARDCLR_EN_SHIFT) /* 0x00000002 */
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF1_FULL_HARDCLR_EN_SHIFT (2U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF1_FULL_HARDCLR_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF1_FULL_HARDCLR_EN_SHIFT) /* 0x00000004 */
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_SHIFT      (4U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_MASK       (0x1FFFFU << BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_SIZE_SHIFT)  /* 0x001FFFF0 */
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_NUM_SHIFT       (24U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_NUM_MASK        (0x7U << BUFFER_MANAGER_WCH_MBLK_CON0_PPBUF_NUM_SHIFT)       /* 0x07000000 */
#define BUFFER_MANAGER_WCH_MBLK_CON0_ADDR_HIGHBIT_SHIFT    (28U)
#define BUFFER_MANAGER_WCH_MBLK_CON0_ADDR_HIGHBIT_MASK     (0xFU << BUFFER_MANAGER_WCH_MBLK_CON0_ADDR_HIGHBIT_SHIFT)    /* 0xF0000000 */
/* WCH_MBLK_CON1 */
#define BUFFER_MANAGER_WCH_MBLK_CON1_OFFSET                (0x4U)
#define BUFFER_MANAGER_WCH_MBLK_CON1_PPBUF0_START_ADDR_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON1_PPBUF0_START_ADDR_MASK (0xFFFFFFFFU << BUFFER_MANAGER_WCH_MBLK_CON1_PPBUF0_START_ADDR_SHIFT) /* 0xFFFFFFFF */
/* WCH_MBLK_CON2 */
#define BUFFER_MANAGER_WCH_MBLK_CON2_OFFSET                (0x8U)
#define BUFFER_MANAGER_WCH_MBLK_CON2_PPBUF1_START_ADDR_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON2_PPBUF1_START_ADDR_MASK (0xFFFFFFFFU << BUFFER_MANAGER_WCH_MBLK_CON2_PPBUF1_START_ADDR_SHIFT) /* 0xFFFFFFFF */
/* WCH_MBLK_CON3 */
#define BUFFER_MANAGER_WCH_MBLK_CON3_OFFSET                (0xCU)
#define BUFFER_MANAGER_WCH_MBLK_CON3_PPBUF2_START_ADDR_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON3_PPBUF2_START_ADDR_MASK (0xFFFFFFFFU << BUFFER_MANAGER_WCH_MBLK_CON3_PPBUF2_START_ADDR_SHIFT) /* 0xFFFFFFFF */
/* WCH_MBLK_CON4 */
#define BUFFER_MANAGER_WCH_MBLK_CON4_OFFSET                (0x10U)
#define BUFFER_MANAGER_WCH_MBLK_CON4_PPBUF3_START_ADDR_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON4_PPBUF3_START_ADDR_MASK (0xFFFFFFFFU << BUFFER_MANAGER_WCH_MBLK_CON4_PPBUF3_START_ADDR_SHIFT) /* 0xFFFFFFFF */
/* WCH_MBLK_CON5 */
#define BUFFER_MANAGER_WCH_MBLK_CON5_OFFSET                (0x14U)
#define BUFFER_MANAGER_WCH_MBLK_CON5_PPBUF_OVERFLOW_MARGIN_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON5_PPBUF_OVERFLOW_MARGIN_MASK (0x3FFU << BUFFER_MANAGER_WCH_MBLK_CON5_PPBUF_OVERFLOW_MARGIN_SHIFT) /* 0x000003FF */
#define BUFFER_MANAGER_WCH_MBLK_CON5_PPBUF_OVERFLOW_MODE_SHIFT (12U)
#define BUFFER_MANAGER_WCH_MBLK_CON5_PPBUF_OVERFLOW_MODE_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_CON5_PPBUF_OVERFLOW_MODE_SHIFT) /* 0x00001000 */
/* WCH_MBLK_CON6 */
#define BUFFER_MANAGER_WCH_MBLK_CON6_OFFSET                (0x18U)
#define BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_DATA_NUM_SHIFT  (0U)
#define BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_DATA_NUM_MASK   (0x1FFFFFFU << BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_DATA_NUM_SHIFT) /* 0x01FFFFFF */
#define BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_PPBUF_SWITCH_MODE_SHIFT (28U)
#define BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_PPBUF_SWITCH_MODE_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_PPBUF_SWITCH_MODE_SHIFT) /* 0x10000000 */
#define BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_END_MODE_SHIFT  (29U)
#define BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_END_MODE_MASK   (0x1U << BUFFER_MANAGER_WCH_MBLK_CON6_FRAME_END_MODE_SHIFT)  /* 0x20000000 */
/* WCH_MBLK_STATUS0 */
#define BUFFER_MANAGER_WCH_MBLK_STATUS0_OFFSET             (0x20U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_DCOUNTER_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_DCOUNTER_MASK (0x1FFFFU << BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_DCOUNTER_SHIFT) /* 0x0001FFFF */
#define BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_INDEX_SHIFT (20U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_INDEX_MASK (0x3U << BUFFER_MANAGER_WCH_MBLK_STATUS0_PPBUF_CUR_INDEX_SHIFT) /* 0x00300000 */
/* WCH_MBLK_STATUS1 */
#define BUFFER_MANAGER_WCH_MBLK_STATUS1_OFFSET             (0x24U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_DCOUNTER_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_DCOUNTER_MASK (0x1FFFFU << BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_DCOUNTER_SHIFT) /* 0x0001FFFF */
#define BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_INDEX_SHIFT (20U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_INDEX_MASK (0x3U << BUFFER_MANAGER_WCH_MBLK_STATUS1_PPBUF_LAST_INDEX_SHIFT) /* 0x00300000 */
/* WCH_MBLK_STATUS2 */
#define BUFFER_MANAGER_WCH_MBLK_STATUS2_OFFSET             (0x28U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS2_FRAME_CUR_COUNTER_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS2_FRAME_CUR_COUNTER_MASK (0x1FFFFFFU << BUFFER_MANAGER_WCH_MBLK_STATUS2_FRAME_CUR_COUNTER_SHIFT) /* 0x01FFFFFF */
/* WCH_MBLK_STATUS3 */
#define BUFFER_MANAGER_WCH_MBLK_STATUS3_OFFSET             (0x2CU)
#define BUFFER_MANAGER_WCH_MBLK_STATUS3_FRAME_END_COUNTER_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_STATUS3_FRAME_END_COUNTER_MASK (0x1FFFFFFU << BUFFER_MANAGER_WCH_MBLK_STATUS3_FRAME_END_COUNTER_SHIFT) /* 0x01FFFFFF */
/* WCH_MBLK_INT_EN */
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_OFFSET              (0x30U)
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF0_FULL_INT_EN_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF0_FULL_INT_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF0_FULL_INT_EN_SHIFT) /* 0x00000001 */
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF1_FULL_INT_EN_SHIFT (1U)
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF1_FULL_INT_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF1_FULL_INT_EN_SHIFT) /* 0x00000002 */
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF2_FULL_INT_EN_SHIFT (2U)
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF2_FULL_INT_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF2_FULL_INT_EN_SHIFT) /* 0x00000004 */
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF3_FULL_INT_EN_SHIFT (3U)
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF3_FULL_INT_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF3_FULL_INT_EN_SHIFT) /* 0x00000008 */
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF_OVERFLOW_INT_EN_SHIFT (16U)
#define BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF_OVERFLOW_INT_EN_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INT_EN_PPBUF_OVERFLOW_INT_EN_SHIFT) /* 0x00010000 */
/* WCH_MBLK_INTSTS */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_OFFSET              (0x34U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_INTSTS_SHIFT (0U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_INTSTS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_INTSTS_SHIFT) /* 0x00000001 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FULL_INTSTS_SHIFT (1U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FULL_INTSTS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FULL_INTSTS_SHIFT) /* 0x00000002 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FULL_INTSTS_SHIFT (2U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FULL_INTSTS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FULL_INTSTS_SHIFT) /* 0x00000004 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FULL_INTSTS_SHIFT (3U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FULL_INTSTS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FULL_INTSTS_SHIFT) /* 0x00000008 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_STATUS_SHIFT (4U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FULL_STATUS_SHIFT) /* 0x00000010 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FULL_STATUS_SHIFT (5U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FULL_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FULL_STATUS_SHIFT) /* 0x00000020 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FULL_STATUS_SHIFT (6U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FULL_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FULL_STATUS_SHIFT) /* 0x00000040 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FULL_STATUS_SHIFT (7U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FULL_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FULL_STATUS_SHIFT) /* 0x00000080 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FIRST_FLAG_STATUS_SHIFT (8U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FIRST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_FIRST_FLAG_STATUS_SHIFT) /* 0x00000100 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FIRST_FLAG_STATUS_SHIFT (9U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FIRST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_FIRST_FLAG_STATUS_SHIFT) /* 0x00000200 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FIRST_FLAG_STATUS_SHIFT (10U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FIRST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_FIRST_FLAG_STATUS_SHIFT) /* 0x00000400 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FIRST_FLAG_STATUS_SHIFT (11U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FIRST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_FIRST_FLAG_STATUS_SHIFT) /* 0x00000800 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_LAST_FLAG_STATUS_SHIFT (12U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_LAST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF0_LAST_FLAG_STATUS_SHIFT) /* 0x00001000 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_LAST_FLAG_STATUS_SHIFT (13U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_LAST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF1_LAST_FLAG_STATUS_SHIFT) /* 0x00002000 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_LAST_FLAG_STATUS_SHIFT (14U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_LAST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF2_LAST_FLAG_STATUS_SHIFT) /* 0x00004000 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_LAST_FLAG_STATUS_SHIFT (15U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_LAST_FLAG_STATUS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF3_LAST_FLAG_STATUS_SHIFT) /* 0x00008000 */
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF_OVERFLOW_INTSTS_SHIFT (16U)
#define BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF_OVERFLOW_INTSTS_MASK (0x1U << BUFFER_MANAGER_WCH_MBLK_INTSTS_PPBUF_OVERFLOW_INTSTS_SHIFT) /* 0x00010000 */
/* RCH_MBLK_CON0 */
#define BUFFER_MANAGER_RCH_MBLK_CON0_OFFSET                (0x200U)
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF_ENABLE_SHIFT    (0U)
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF_ENABLE_MASK     (0x1U << BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF_ENABLE_SHIFT)    /* 0x00000001 */
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF0_EMPTY_HARDCLR_EN_SHIFT (1U)
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF0_EMPTY_HARDCLR_EN_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF0_EMPTY_HARDCLR_EN_SHIFT) /* 0x00000002 */
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF1_EMPTY_HARDCLR_EN_SHIFT (2U)
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF1_EMPTY_HARDCLR_EN_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF1_EMPTY_HARDCLR_EN_SHIFT) /* 0x00000004 */
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF_SIZE_SHIFT      (4U)
#define BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF_SIZE_MASK       (0x1FFFFU << BUFFER_MANAGER_RCH_MBLK_CON0_PPBUF_SIZE_SHIFT)  /* 0x001FFFF0 */
#define BUFFER_MANAGER_RCH_MBLK_CON0_ADDR_HIGHBIT_SHIFT    (28U)
#define BUFFER_MANAGER_RCH_MBLK_CON0_ADDR_HIGHBIT_MASK     (0xFU << BUFFER_MANAGER_RCH_MBLK_CON0_ADDR_HIGHBIT_SHIFT)    /* 0xF0000000 */
/* RCH_MBLK_CON1 */
#define BUFFER_MANAGER_RCH_MBLK_CON1_OFFSET                (0x204U)
#define BUFFER_MANAGER_RCH_MBLK_CON1_PPBUF0_START_ADDR_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_CON1_PPBUF0_START_ADDR_MASK (0xFFFFFFFFU << BUFFER_MANAGER_RCH_MBLK_CON1_PPBUF0_START_ADDR_SHIFT) /* 0xFFFFFFFF */
/* RCH_MBLK_CON2 */
#define BUFFER_MANAGER_RCH_MBLK_CON2_OFFSET                (0x208U)
#define BUFFER_MANAGER_RCH_MBLK_CON2_PPBUF1_START_ADDR_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_CON2_PPBUF1_START_ADDR_MASK (0xFFFFFFFFU << BUFFER_MANAGER_RCH_MBLK_CON2_PPBUF1_START_ADDR_SHIFT) /* 0xFFFFFFFF */
/* RCH_MBLK_CON6 */
#define BUFFER_MANAGER_RCH_MBLK_CON6_OFFSET                (0x218U)
#define BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_DATA_NUM_SHIFT  (0U)
#define BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_DATA_NUM_MASK   (0x1FFFFFFU << BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_DATA_NUM_SHIFT) /* 0x01FFFFFF */
#define BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_PPBUF_SWITCH_MODE_SHIFT (28U)
#define BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_PPBUF_SWITCH_MODE_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_PPBUF_SWITCH_MODE_SHIFT) /* 0x10000000 */
#define BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_END_MODE_SHIFT  (29U)
#define BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_END_MODE_MASK   (0x1U << BUFFER_MANAGER_RCH_MBLK_CON6_FRAME_END_MODE_SHIFT)  /* 0x20000000 */
/* RCH_MBLK_STATUS0 */
#define BUFFER_MANAGER_RCH_MBLK_STATUS0_OFFSET             (0x220U)
#define BUFFER_MANAGER_RCH_MBLK_STATUS0_PPBUF_CUR_DCOUNTER_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_STATUS0_PPBUF_CUR_DCOUNTER_MASK (0x1FFFFU << BUFFER_MANAGER_RCH_MBLK_STATUS0_PPBUF_CUR_DCOUNTER_SHIFT) /* 0x0001FFFF */
#define BUFFER_MANAGER_RCH_MBLK_STATUS0_PPBUF_CUR_INDEX_SHIFT (20U)
#define BUFFER_MANAGER_RCH_MBLK_STATUS0_PPBUF_CUR_INDEX_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_STATUS0_PPBUF_CUR_INDEX_SHIFT) /* 0x00100000 */
/* RCH_MBLK_STATUS2 */
#define BUFFER_MANAGER_RCH_MBLK_STATUS2_OFFSET             (0x228U)
#define BUFFER_MANAGER_RCH_MBLK_STATUS2_FRAME_CUR_DCOUNTER_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_STATUS2_FRAME_CUR_DCOUNTER_MASK (0x1FFFFFFU << BUFFER_MANAGER_RCH_MBLK_STATUS2_FRAME_CUR_DCOUNTER_SHIFT) /* 0x01FFFFFF */
/* RCH_MBLK_STATUS3 */
#define BUFFER_MANAGER_RCH_MBLK_STATUS3_OFFSET             (0x22CU)
#define BUFFER_MANAGER_RCH_MBLK_STATUS3_FRAME_END_DCOUNTER_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_STATUS3_FRAME_END_DCOUNTER_MASK (0x1FFFFFFU << BUFFER_MANAGER_RCH_MBLK_STATUS3_FRAME_END_DCOUNTER_SHIFT) /* 0x01FFFFFF */
/* RCH_MBLK_INT_EN */
#define BUFFER_MANAGER_RCH_MBLK_INT_EN_OFFSET              (0x230U)
#define BUFFER_MANAGER_RCH_MBLK_INT_EN_PPBUF0_EMPTY_INT_EN_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_INT_EN_PPBUF0_EMPTY_INT_EN_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_INT_EN_PPBUF0_EMPTY_INT_EN_SHIFT) /* 0x00000001 */
#define BUFFER_MANAGER_RCH_MBLK_INT_EN_PPBUF1_EMPTY_INT_EN_SHIFT (1U)
#define BUFFER_MANAGER_RCH_MBLK_INT_EN_PPBUF1_EMPTY_INT_EN_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_INT_EN_PPBUF1_EMPTY_INT_EN_SHIFT) /* 0x00000002 */
/* RCH_MBLK_INTSTS */
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_OFFSET              (0x234U)
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF0_EMPTY_INTSTS_SHIFT (0U)
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF0_EMPTY_INTSTS_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF0_EMPTY_INTSTS_SHIFT) /* 0x00000001 */
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF1_EMPTY_INTSTS_SHIFT (1U)
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF1_EMPTY_INTSTS_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF1_EMPTY_INTSTS_SHIFT) /* 0x00000002 */
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF0_EMPTY_STATUS_SHIFT (4U)
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF0_EMPTY_STATUS_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF0_EMPTY_STATUS_SHIFT) /* 0x00000010 */
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF1_EMPTY_STATUS_SHIFT (5U)
#define BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF1_EMPTY_STATUS_MASK (0x1U << BUFFER_MANAGER_RCH_MBLK_INTSTS_PPBUF1_EMPTY_STATUS_SHIFT) /* 0x00000020 */
/******************************************WDT*******************************************/
/* CR */
#define WDT_CR_OFFSET                                      (0x0U)
#define WDT_CR_WDT_EN_SHIFT                                (0U)
#define WDT_CR_WDT_EN_MASK                                 (0x1U << WDT_CR_WDT_EN_SHIFT)                                /* 0x00000001 */
#define WDT_CR_RESP_MODE_SHIFT                             (1U)
#define WDT_CR_RESP_MODE_MASK                              (0x1U << WDT_CR_RESP_MODE_SHIFT)                             /* 0x00000002 */
#define WDT_CR_RST_PLUSE_LENGTH_SHIFT                      (2U)
#define WDT_CR_RST_PLUSE_LENGTH_MASK                       (0x7U << WDT_CR_RST_PLUSE_LENGTH_SHIFT)                      /* 0x0000001C */
/* TORR */
#define WDT_TORR_OFFSET                                    (0x4U)
#define WDT_TORR_TIMEOUT_PERIOD_SHIFT                      (0U)
#define WDT_TORR_TIMEOUT_PERIOD_MASK                       (0xFU << WDT_TORR_TIMEOUT_PERIOD_SHIFT)                      /* 0x0000000F */
/* CCVR */
#define WDT_CCVR_OFFSET                                    (0x8U)
#define WDT_CCVR                                           (0x0U)
#define WDT_CCVR_CUR_CNT_SHIFT                             (0U)
#define WDT_CCVR_CUR_CNT_MASK                              (0xFFFFFFFFU << WDT_CCVR_CUR_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* CRR */
#define WDT_CRR_OFFSET                                     (0xCU)
#define WDT_CRR_CNT_RESTART_SHIFT                          (0U)
#define WDT_CRR_CNT_RESTART_MASK                           (0xFFU << WDT_CRR_CNT_RESTART_SHIFT)                         /* 0x000000FF */
/* STAT */
#define WDT_STAT_OFFSET                                    (0x10U)
#define WDT_STAT                                           (0x0U)
#define WDT_STAT_WDT_STATUS_SHIFT                          (0U)
#define WDT_STAT_WDT_STATUS_MASK                           (0x1U << WDT_STAT_WDT_STATUS_SHIFT)                          /* 0x00000001 */
/* EOI */
#define WDT_EOI_OFFSET                                     (0x14U)
#define WDT_EOI                                            (0x0U)
#define WDT_EOI_WDT_INT_CLR_SHIFT                          (0U)
#define WDT_EOI_WDT_INT_CLR_MASK                           (0x1U << WDT_EOI_WDT_INT_CLR_SHIFT)                          /* 0x00000001 */
/******************************************INTC******************************************/
/* IRQ_INTEN_L */
#define INTC_IRQ_INTEN_L_OFFSET                            (0x0U)
#define INTC_IRQ_INTEN_L_IRQ_INTEN_L_SHIFT                 (0U)
#define INTC_IRQ_INTEN_L_IRQ_INTEN_L_MASK                  (0xFFFFFFFFU << INTC_IRQ_INTEN_L_IRQ_INTEN_L_SHIFT)          /* 0xFFFFFFFF */
/* IRQ_INTEN_H */
#define INTC_IRQ_INTEN_H_OFFSET                            (0x4U)
#define INTC_IRQ_INTEN_H_IRQ_INTEN_H_SHIFT                 (0U)
#define INTC_IRQ_INTEN_H_IRQ_INTEN_H_MASK                  (0xFFFFFFFFU << INTC_IRQ_INTEN_H_IRQ_INTEN_H_SHIFT)          /* 0xFFFFFFFF */
/* IRQ_INTMASK_L */
#define INTC_IRQ_INTMASK_L_OFFSET                          (0x8U)
#define INTC_IRQ_INTMASK_L_IRQ_INTMASK_L_SHIFT             (0U)
#define INTC_IRQ_INTMASK_L_IRQ_INTMASK_L_MASK              (0xFFFFFFFFU << INTC_IRQ_INTMASK_L_IRQ_INTMASK_L_SHIFT)      /* 0xFFFFFFFF */
/* IRQ_INTMASK_H */
#define INTC_IRQ_INTMASK_H_OFFSET                          (0xCU)
#define INTC_IRQ_INTMASK_H_IRQ_INTMASK_H_SHIFT             (0U)
#define INTC_IRQ_INTMASK_H_IRQ_INTMASK_H_MASK              (0xFFFFFFFFU << INTC_IRQ_INTMASK_H_IRQ_INTMASK_H_SHIFT)      /* 0xFFFFFFFF */
/* IRQ_INTFORCE_L */
#define INTC_IRQ_INTFORCE_L_OFFSET                         (0x10U)
#define INTC_IRQ_INTFORCE_L_IRQ_INTFORCE_L_SHIFT           (0U)
#define INTC_IRQ_INTFORCE_L_IRQ_INTFORCE_L_MASK            (0xFFFFFFFFU << INTC_IRQ_INTFORCE_L_IRQ_INTFORCE_L_SHIFT)    /* 0xFFFFFFFF */
/* IRQ_INTFORCE_H */
#define INTC_IRQ_INTFORCE_H_OFFSET                         (0x14U)
#define INTC_IRQ_INTFORCE_H_IRQ_INTFORCE_H_SHIFT           (0U)
#define INTC_IRQ_INTFORCE_H_IRQ_INTFORCE_H_MASK            (0xFFFFFFFFU << INTC_IRQ_INTFORCE_H_IRQ_INTFORCE_H_SHIFT)    /* 0xFFFFFFFF */
/* IRQ_RAWSTATUS_L */
#define INTC_IRQ_RAWSTATUS_L_OFFSET                        (0x18U)
#define INTC_IRQ_RAWSTATUS_L                               (0x0U)
#define INTC_IRQ_RAWSTATUS_L_IRQ_RAWSTATUS_L_SHIFT         (0U)
#define INTC_IRQ_RAWSTATUS_L_IRQ_RAWSTATUS_L_MASK          (0xFFFFFFFFU << INTC_IRQ_RAWSTATUS_L_IRQ_RAWSTATUS_L_SHIFT)  /* 0xFFFFFFFF */
/* IRQ_RAWSTATUS_H */
#define INTC_IRQ_RAWSTATUS_H_OFFSET                        (0x1CU)
#define INTC_IRQ_RAWSTATUS_H                               (0x0U)
#define INTC_IRQ_RAWSTATUS_H_IRQ_RAWSTATUS_H_SHIFT         (0U)
#define INTC_IRQ_RAWSTATUS_H_IRQ_RAWSTATUS_H_MASK          (0xFFFFFFFFU << INTC_IRQ_RAWSTATUS_H_IRQ_RAWSTATUS_H_SHIFT)  /* 0xFFFFFFFF */
/* IRQ_STATUS_L */
#define INTC_IRQ_STATUS_L_OFFSET                           (0x20U)
#define INTC_IRQ_STATUS_L                                  (0x0U)
#define INTC_IRQ_STATUS_L_IRQ_STATUS_L_SHIFT               (0U)
#define INTC_IRQ_STATUS_L_IRQ_STATUS_L_MASK                (0xFFFFFFFFU << INTC_IRQ_STATUS_L_IRQ_STATUS_L_SHIFT)        /* 0xFFFFFFFF */
/* IRQ_STATUS_H */
#define INTC_IRQ_STATUS_H_OFFSET                           (0x24U)
#define INTC_IRQ_STATUS_H                                  (0x0U)
#define INTC_IRQ_STATUS_H_IRQ_STATUS_H_SHIFT               (0U)
#define INTC_IRQ_STATUS_H_IRQ_STATUS_H_MASK                (0xFFFFFFFFU << INTC_IRQ_STATUS_H_IRQ_STATUS_H_SHIFT)        /* 0xFFFFFFFF */
/* IRQ_MASKSTATUS_L */
#define INTC_IRQ_MASKSTATUS_L_OFFSET                       (0x28U)
#define INTC_IRQ_MASKSTATUS_L                              (0x0U)
#define INTC_IRQ_MASKSTATUS_L_IRQ_MASKSTATUS_L_SHIFT       (0U)
#define INTC_IRQ_MASKSTATUS_L_IRQ_MASKSTATUS_L_MASK        (0xFFFFFFFFU << INTC_IRQ_MASKSTATUS_L_IRQ_MASKSTATUS_L_SHIFT) /* 0xFFFFFFFF */
/* IRQ_MASKSTATUS_H */
#define INTC_IRQ_MASKSTATUS_H_OFFSET                       (0x2CU)
#define INTC_IRQ_MASKSTATUS_H                              (0x0U)
#define INTC_IRQ_MASKSTATUS_H_IRQ_MASKSTATUS_H_SHIFT       (0U)
#define INTC_IRQ_MASKSTATUS_H_IRQ_MASKSTATUS_H_MASK        (0xFFFFFFFFU << INTC_IRQ_MASKSTATUS_H_IRQ_MASKSTATUS_H_SHIFT) /* 0xFFFFFFFF */
/* IRQ_FINALSTATUS_L */
#define INTC_IRQ_FINALSTATUS_L_OFFSET                      (0x30U)
#define INTC_IRQ_FINALSTATUS_L                             (0x0U)
#define INTC_IRQ_FINALSTATUS_L_IRQ_FINALSTATUS_L_SHIFT     (0U)
#define INTC_IRQ_FINALSTATUS_L_IRQ_FINALSTATUS_L_MASK      (0xFFFFFFFFU << INTC_IRQ_FINALSTATUS_L_IRQ_FINALSTATUS_L_SHIFT) /* 0xFFFFFFFF */
/* IRQ_FINALSTATUS_H */
#define INTC_IRQ_FINALSTATUS_H_OFFSET                      (0x34U)
#define INTC_IRQ_FINALSTATUS_H                             (0x0U)
#define INTC_IRQ_FINALSTATUS_H_IRQ_FINALSTATUS_H_SHIFT     (0U)
#define INTC_IRQ_FINALSTATUS_H_IRQ_FINALSTATUS_H_MASK      (0xFFFFFFFFU << INTC_IRQ_FINALSTATUS_H_IRQ_FINALSTATUS_H_SHIFT) /* 0xFFFFFFFF */
/* FIQ_INTEN */
#define INTC_FIQ_INTEN_OFFSET                              (0xC0U)
#define INTC_FIQ_INTEN_FIQ_INTEN_SHIFT                     (0U)
#define INTC_FIQ_INTEN_FIQ_INTEN_MASK                      (0x3U << INTC_FIQ_INTEN_FIQ_INTEN_SHIFT)                     /* 0x00000003 */
/* FIQ_INTMASK */
#define INTC_FIQ_INTMASK_OFFSET                            (0xC4U)
#define INTC_FIQ_INTMASK_FIQ_INTMASK_SHIFT                 (0U)
#define INTC_FIQ_INTMASK_FIQ_INTMASK_MASK                  (0x3U << INTC_FIQ_INTMASK_FIQ_INTMASK_SHIFT)                 /* 0x00000003 */
/* FIQ_INTFORCE */
#define INTC_FIQ_INTFORCE_OFFSET                           (0xC8U)
#define INTC_FIQ_INTFORCE_FIQ_INTFORCE_SHIFT               (0U)
#define INTC_FIQ_INTFORCE_FIQ_INTFORCE_MASK                (0x3U << INTC_FIQ_INTFORCE_FIQ_INTFORCE_SHIFT)               /* 0x00000003 */
/* FIQ_RAWSTATUS */
#define INTC_FIQ_RAWSTATUS_OFFSET                          (0xCCU)
#define INTC_FIQ_RAWSTATUS                                 (0x0U)
#define INTC_FIQ_RAWSTATUS_FIQ_RAWSTATUS_SHIFT             (0U)
#define INTC_FIQ_RAWSTATUS_FIQ_RAWSTATUS_MASK              (0x3U << INTC_FIQ_RAWSTATUS_FIQ_RAWSTATUS_SHIFT)             /* 0x00000003 */
/* FIQ_STATUS */
#define INTC_FIQ_STATUS_OFFSET                             (0xD0U)
#define INTC_FIQ_STATUS                                    (0x0U)
#define INTC_FIQ_STATUS_FIQ_STATUS_SHIFT                   (0U)
#define INTC_FIQ_STATUS_FIQ_STATUS_MASK                    (0x3U << INTC_FIQ_STATUS_FIQ_STATUS_SHIFT)                   /* 0x00000003 */
/* FIQ_FINALSTATUS */
#define INTC_FIQ_FINALSTATUS_OFFSET                        (0xD4U)
#define INTC_FIQ_FINALSTATUS                               (0x0U)
#define INTC_FIQ_FINALSTATUS_FIQ_FINALSTATUS_SHIFT         (0U)
#define INTC_FIQ_FINALSTATUS_FIQ_FINALSTATUS_MASK          (0x3U << INTC_FIQ_FINALSTATUS_FIQ_FINALSTATUS_SHIFT)         /* 0x00000003 */
/* IRQ_PLEVEL */
#define INTC_IRQ_PLEVEL_OFFSET                             (0xD8U)
#define INTC_IRQ_PLEVEL_IRQ_PLEVEL_SHIFT                   (0U)
#define INTC_IRQ_PLEVEL_IRQ_PLEVEL_MASK                    (0xFU << INTC_IRQ_PLEVEL_IRQ_PLEVEL_SHIFT)                   /* 0x0000000F */
/* IRQ_PR_OFFSET */
#define INTC_IRQ_PR_OFFSET_OFFSET                          (0xE8U)
#define INTC_IRQ_PR_OFFSET_IRQ_PR_OFFSET_SHIFT             (0U)
#define INTC_IRQ_PR_OFFSET_IRQ_PR_OFFSET_MASK              (0xFU << INTC_IRQ_PR_OFFSET_IRQ_PR_OFFSET_SHIFT)             /* 0x0000000F */
/* AHB_ICTL_COMP_VERSION */
#define INTC_AHB_ICTL_COMP_VERSION_OFFSET                  (0x3F8U)
#define INTC_AHB_ICTL_COMP_VERSION                         (0x3230342AU)
#define INTC_AHB_ICTL_COMP_VERSION_AHB_ICTL_COMP_VERSION_SHIFT (0U)
#define INTC_AHB_ICTL_COMP_VERSION_AHB_ICTL_COMP_VERSION_MASK (0xFFFFFFFFU << INTC_AHB_ICTL_COMP_VERSION_AHB_ICTL_COMP_VERSION_SHIFT) /* 0xFFFFFFFF */
/* ICTL_COMP_TYPE */
#define INTC_ICTL_COMP_TYPE_OFFSET                         (0x3FCU)
#define INTC_ICTL_COMP_TYPE                                (0x44571120U)
#define INTC_ICTL_COMP_TYPE_ICTL_COMP_TYPE_SHIFT           (0U)
#define INTC_ICTL_COMP_TYPE_ICTL_COMP_TYPE_MASK            (0xFFFFFFFFU << INTC_ICTL_COMP_TYPE_ICTL_COMP_TYPE_SHIFT)    /* 0xFFFFFFFF */
/******************************************I2C*******************************************/
/* CON */
#define I2C_CON_OFFSET                                     (0x0U)
#define I2C_CON_I2C_EN_SHIFT                               (0U)
#define I2C_CON_I2C_EN_MASK                                (0x1U << I2C_CON_I2C_EN_SHIFT)                               /* 0x00000001 */
#define I2C_CON_I2C_MODE_SHIFT                             (1U)
#define I2C_CON_I2C_MODE_MASK                              (0x3U << I2C_CON_I2C_MODE_SHIFT)                             /* 0x00000006 */
#define I2C_CON_START_SHIFT                                (3U)
#define I2C_CON_START_MASK                                 (0x1U << I2C_CON_START_SHIFT)                                /* 0x00000008 */
#define I2C_CON_STOP_SHIFT                                 (4U)
#define I2C_CON_STOP_MASK                                  (0x1U << I2C_CON_STOP_SHIFT)                                 /* 0x00000010 */
#define I2C_CON_ACK_SHIFT                                  (5U)
#define I2C_CON_ACK_MASK                                   (0x1U << I2C_CON_ACK_SHIFT)                                  /* 0x00000020 */
#define I2C_CON_ACT2NAK_SHIFT                              (6U)
#define I2C_CON_ACT2NAK_MASK                               (0x1U << I2C_CON_ACT2NAK_SHIFT)                              /* 0x00000040 */
#define I2C_CON_DATA_UPD_ST_SHIFT                          (8U)
#define I2C_CON_DATA_UPD_ST_MASK                           (0x7U << I2C_CON_DATA_UPD_ST_SHIFT)                          /* 0x00000700 */
#define I2C_CON_START_SETUP_SHIFT                          (12U)
#define I2C_CON_START_SETUP_MASK                           (0x3U << I2C_CON_START_SETUP_SHIFT)                          /* 0x00003000 */
#define I2C_CON_STOP_SETUP_SHIFT                           (14U)
#define I2C_CON_STOP_SETUP_MASK                            (0x3U << I2C_CON_STOP_SETUP_SHIFT)                           /* 0x0000C000 */
#define I2C_CON_VERSION_SHIFT                              (16U)
#define I2C_CON_VERSION_MASK                               (0xFFFFU << I2C_CON_VERSION_SHIFT)                           /* 0xFFFF0000 */
/* CLKDIV */
#define I2C_CLKDIV_OFFSET                                  (0x4U)
#define I2C_CLKDIV_CLKDIVL_SHIFT                           (0U)
#define I2C_CLKDIV_CLKDIVL_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVL_SHIFT)                        /* 0x0000FFFF */
#define I2C_CLKDIV_CLKDIVH_SHIFT                           (16U)
#define I2C_CLKDIV_CLKDIVH_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVH_SHIFT)                        /* 0xFFFF0000 */
/* MRXADDR */
#define I2C_MRXADDR_OFFSET                                 (0x8U)
#define I2C_MRXADDR_SADDR_SHIFT                            (0U)
#define I2C_MRXADDR_SADDR_MASK                             (0xFFFFFFU << I2C_MRXADDR_SADDR_SHIFT)                       /* 0x00FFFFFF */
#define I2C_MRXADDR_ADDLVLD_SHIFT                          (24U)
#define I2C_MRXADDR_ADDLVLD_MASK                           (0x1U << I2C_MRXADDR_ADDLVLD_SHIFT)                          /* 0x01000000 */
#define I2C_MRXADDR_ADDMVLD_SHIFT                          (25U)
#define I2C_MRXADDR_ADDMVLD_MASK                           (0x1U << I2C_MRXADDR_ADDMVLD_SHIFT)                          /* 0x02000000 */
#define I2C_MRXADDR_ADDHVLD_SHIFT                          (26U)
#define I2C_MRXADDR_ADDHVLD_MASK                           (0x1U << I2C_MRXADDR_ADDHVLD_SHIFT)                          /* 0x04000000 */
/* MRXRADDR */
#define I2C_MRXRADDR_OFFSET                                (0xCU)
#define I2C_MRXRADDR_SRADDR_SHIFT                          (0U)
#define I2C_MRXRADDR_SRADDR_MASK                           (0xFFFFFFU << I2C_MRXRADDR_SRADDR_SHIFT)                     /* 0x00FFFFFF */
#define I2C_MRXRADDR_SRADDLVLD_SHIFT                       (24U)
#define I2C_MRXRADDR_SRADDLVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDLVLD_SHIFT)                       /* 0x01000000 */
#define I2C_MRXRADDR_SRADDMVLD_SHIFT                       (25U)
#define I2C_MRXRADDR_SRADDMVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDMVLD_SHIFT)                       /* 0x02000000 */
#define I2C_MRXRADDR_SRADDHVLD_SHIFT                       (26U)
#define I2C_MRXRADDR_SRADDHVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDHVLD_SHIFT)                       /* 0x04000000 */
/* MTXCNT */
#define I2C_MTXCNT_OFFSET                                  (0x10U)
#define I2C_MTXCNT_MTXCNT_SHIFT                            (0U)
#define I2C_MTXCNT_MTXCNT_MASK                             (0x3FU << I2C_MTXCNT_MTXCNT_SHIFT)                           /* 0x0000003F */
/* MRXCNT */
#define I2C_MRXCNT_OFFSET                                  (0x14U)
#define I2C_MRXCNT_MRXCNT_SHIFT                            (0U)
#define I2C_MRXCNT_MRXCNT_MASK                             (0x3FU << I2C_MRXCNT_MRXCNT_SHIFT)                           /* 0x0000003F */
/* IEN */
#define I2C_IEN_OFFSET                                     (0x18U)
#define I2C_IEN_BTFIEN_SHIFT                               (0U)
#define I2C_IEN_BTFIEN_MASK                                (0x1U << I2C_IEN_BTFIEN_SHIFT)                               /* 0x00000001 */
#define I2C_IEN_BRFIEN_SHIFT                               (1U)
#define I2C_IEN_BRFIEN_MASK                                (0x1U << I2C_IEN_BRFIEN_SHIFT)                               /* 0x00000002 */
#define I2C_IEN_MBTFIEN_SHIFT                              (2U)
#define I2C_IEN_MBTFIEN_MASK                               (0x1U << I2C_IEN_MBTFIEN_SHIFT)                              /* 0x00000004 */
#define I2C_IEN_MBRFIEN_SHIFT                              (3U)
#define I2C_IEN_MBRFIEN_MASK                               (0x1U << I2C_IEN_MBRFIEN_SHIFT)                              /* 0x00000008 */
#define I2C_IEN_STARTIEN_SHIFT                             (4U)
#define I2C_IEN_STARTIEN_MASK                              (0x1U << I2C_IEN_STARTIEN_SHIFT)                             /* 0x00000010 */
#define I2C_IEN_STOPIEN_SHIFT                              (5U)
#define I2C_IEN_STOPIEN_MASK                               (0x1U << I2C_IEN_STOPIEN_SHIFT)                              /* 0x00000020 */
#define I2C_IEN_NAKRCVIEN_SHIFT                            (6U)
#define I2C_IEN_NAKRCVIEN_MASK                             (0x1U << I2C_IEN_NAKRCVIEN_SHIFT)                            /* 0x00000040 */
#define I2C_IEN_SLAVEHDSCLEN_SHIFT                         (7U)
#define I2C_IEN_SLAVEHDSCLEN_MASK                          (0x1U << I2C_IEN_SLAVEHDSCLEN_SHIFT)                         /* 0x00000080 */
/* IPD */
#define I2C_IPD_OFFSET                                     (0x1CU)
#define I2C_IPD_BTFIPD_SHIFT                               (0U)
#define I2C_IPD_BTFIPD_MASK                                (0x1U << I2C_IPD_BTFIPD_SHIFT)                               /* 0x00000001 */
#define I2C_IPD_BRFIPD_SHIFT                               (1U)
#define I2C_IPD_BRFIPD_MASK                                (0x1U << I2C_IPD_BRFIPD_SHIFT)                               /* 0x00000002 */
#define I2C_IPD_MBTFIPD_SHIFT                              (2U)
#define I2C_IPD_MBTFIPD_MASK                               (0x1U << I2C_IPD_MBTFIPD_SHIFT)                              /* 0x00000004 */
#define I2C_IPD_MBRFIPD_SHIFT                              (3U)
#define I2C_IPD_MBRFIPD_MASK                               (0x1U << I2C_IPD_MBRFIPD_SHIFT)                              /* 0x00000008 */
#define I2C_IPD_STARTIPD_SHIFT                             (4U)
#define I2C_IPD_STARTIPD_MASK                              (0x1U << I2C_IPD_STARTIPD_SHIFT)                             /* 0x00000010 */
#define I2C_IPD_STOPIPD_SHIFT                              (5U)
#define I2C_IPD_STOPIPD_MASK                               (0x1U << I2C_IPD_STOPIPD_SHIFT)                              /* 0x00000020 */
#define I2C_IPD_NAKRCVIPD_SHIFT                            (6U)
#define I2C_IPD_NAKRCVIPD_MASK                             (0x1U << I2C_IPD_NAKRCVIPD_SHIFT)                            /* 0x00000040 */
#define I2C_IPD_SLAVEHDSCLIPD_SHIFT                        (7U)
#define I2C_IPD_SLAVEHDSCLIPD_MASK                         (0x1U << I2C_IPD_SLAVEHDSCLIPD_SHIFT)                        /* 0x00000080 */
/* FCNT */
#define I2C_FCNT_OFFSET                                    (0x20U)
#define I2C_FCNT                                           (0x0U)
#define I2C_FCNT_FCNT_SHIFT                                (0U)
#define I2C_FCNT_FCNT_MASK                                 (0x3FU << I2C_FCNT_FCNT_SHIFT)                               /* 0x0000003F */
/* SCL_OE_DB */
#define I2C_SCL_OE_DB_OFFSET                               (0x24U)
#define I2C_SCL_OE_DB_SCL_OE_DB_SHIFT                      (0U)
#define I2C_SCL_OE_DB_SCL_OE_DB_MASK                       (0xFFU << I2C_SCL_OE_DB_SCL_OE_DB_SHIFT)                     /* 0x000000FF */
/* TXDATA0 */
#define I2C_TXDATA0_OFFSET                                 (0x100U)
#define I2C_TXDATA0_TXDATA0_SHIFT                          (0U)
#define I2C_TXDATA0_TXDATA0_MASK                           (0xFFFFFFFFU << I2C_TXDATA0_TXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA1 */
#define I2C_TXDATA1_OFFSET                                 (0x104U)
#define I2C_TXDATA1_TXDATA1_SHIFT                          (0U)
#define I2C_TXDATA1_TXDATA1_MASK                           (0xFFFFFFFFU << I2C_TXDATA1_TXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA2 */
#define I2C_TXDATA2_OFFSET                                 (0x108U)
#define I2C_TXDATA2_TXDATA2_SHIFT                          (0U)
#define I2C_TXDATA2_TXDATA2_MASK                           (0xFFFFFFFFU << I2C_TXDATA2_TXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA3 */
#define I2C_TXDATA3_OFFSET                                 (0x10CU)
#define I2C_TXDATA3_TXDATA3_SHIFT                          (0U)
#define I2C_TXDATA3_TXDATA3_MASK                           (0xFFFFFFFFU << I2C_TXDATA3_TXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA4 */
#define I2C_TXDATA4_OFFSET                                 (0x110U)
#define I2C_TXDATA4_TXDATA4_SHIFT                          (0U)
#define I2C_TXDATA4_TXDATA4_MASK                           (0xFFFFFFFFU << I2C_TXDATA4_TXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA5 */
#define I2C_TXDATA5_OFFSET                                 (0x114U)
#define I2C_TXDATA5_TXDATA5_SHIFT                          (0U)
#define I2C_TXDATA5_TXDATA5_MASK                           (0xFFFFFFFFU << I2C_TXDATA5_TXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA6 */
#define I2C_TXDATA6_OFFSET                                 (0x118U)
#define I2C_TXDATA6_TXDATA6_SHIFT                          (0U)
#define I2C_TXDATA6_TXDATA6_MASK                           (0xFFFFFFFFU << I2C_TXDATA6_TXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA7 */
#define I2C_TXDATA7_OFFSET                                 (0x11CU)
#define I2C_TXDATA7_TXDATA7_SHIFT                          (0U)
#define I2C_TXDATA7_TXDATA7_MASK                           (0xFFFFFFFFU << I2C_TXDATA7_TXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA0 */
#define I2C_RXDATA0_OFFSET                                 (0x200U)
#define I2C_RXDATA0                                        (0x0U)
#define I2C_RXDATA0_RXDATA0_SHIFT                          (0U)
#define I2C_RXDATA0_RXDATA0_MASK                           (0xFFFFFFFFU << I2C_RXDATA0_RXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA1 */
#define I2C_RXDATA1_OFFSET                                 (0x204U)
#define I2C_RXDATA1                                        (0x0U)
#define I2C_RXDATA1_RXDATA1_SHIFT                          (0U)
#define I2C_RXDATA1_RXDATA1_MASK                           (0xFFFFFFFFU << I2C_RXDATA1_RXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA2 */
#define I2C_RXDATA2_OFFSET                                 (0x208U)
#define I2C_RXDATA2                                        (0x0U)
#define I2C_RXDATA2_RXDATA2_SHIFT                          (0U)
#define I2C_RXDATA2_RXDATA2_MASK                           (0xFFFFFFFFU << I2C_RXDATA2_RXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA3 */
#define I2C_RXDATA3_OFFSET                                 (0x20CU)
#define I2C_RXDATA3                                        (0x0U)
#define I2C_RXDATA3_RXDATA3_SHIFT                          (0U)
#define I2C_RXDATA3_RXDATA3_MASK                           (0xFFFFFFFFU << I2C_RXDATA3_RXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA4 */
#define I2C_RXDATA4_OFFSET                                 (0x210U)
#define I2C_RXDATA4                                        (0x0U)
#define I2C_RXDATA4_RXDATA4_SHIFT                          (0U)
#define I2C_RXDATA4_RXDATA4_MASK                           (0xFFFFFFFFU << I2C_RXDATA4_RXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA5 */
#define I2C_RXDATA5_OFFSET                                 (0x214U)
#define I2C_RXDATA5                                        (0x0U)
#define I2C_RXDATA5_RXDATA5_SHIFT                          (0U)
#define I2C_RXDATA5_RXDATA5_MASK                           (0xFFFFFFFFU << I2C_RXDATA5_RXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA6 */
#define I2C_RXDATA6_OFFSET                                 (0x218U)
#define I2C_RXDATA6                                        (0x0U)
#define I2C_RXDATA6_RXDATA6_SHIFT                          (0U)
#define I2C_RXDATA6_RXDATA6_MASK                           (0xFFFFFFFFU << I2C_RXDATA6_RXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA7 */
#define I2C_RXDATA7_OFFSET                                 (0x21CU)
#define I2C_RXDATA7                                        (0x0U)
#define I2C_RXDATA7_RXDATA7_SHIFT                          (0U)
#define I2C_RXDATA7_RXDATA7_MASK                           (0xFFFFFFFFU << I2C_RXDATA7_RXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* ST */
#define I2C_ST_OFFSET                                      (0x220U)
#define I2C_ST                                             (0x0U)
#define I2C_ST_SDA_ST_SHIFT                                (0U)
#define I2C_ST_SDA_ST_MASK                                 (0x1U << I2C_ST_SDA_ST_SHIFT)                                /* 0x00000001 */
#define I2C_ST_SCL_ST_SHIFT                                (1U)
#define I2C_ST_SCL_ST_MASK                                 (0x1U << I2C_ST_SCL_ST_SHIFT)                                /* 0x00000002 */
/* DBGCTRL */
#define I2C_DBGCTRL_OFFSET                                 (0x224U)
#define I2C_DBGCTRL_FLT_F_SHIFT                            (0U)
#define I2C_DBGCTRL_FLT_F_MASK                             (0xFU << I2C_DBGCTRL_FLT_F_SHIFT)                            /* 0x0000000F */
#define I2C_DBGCTRL_FLT_R_SHIFT                            (4U)
#define I2C_DBGCTRL_FLT_R_MASK                             (0xFU << I2C_DBGCTRL_FLT_R_SHIFT)                            /* 0x000000F0 */
#define I2C_DBGCTRL_SLV_HOLD_SCL_TH_SHIFT                  (8U)
#define I2C_DBGCTRL_SLV_HOLD_SCL_TH_MASK                   (0xFU << I2C_DBGCTRL_SLV_HOLD_SCL_TH_SHIFT)                  /* 0x00000F00 */
#define I2C_DBGCTRL_FLT_EN_SHIFT                           (12U)
#define I2C_DBGCTRL_FLT_EN_MASK                            (0x1U << I2C_DBGCTRL_FLT_EN_SHIFT)                           /* 0x00001000 */
#define I2C_DBGCTRL_NAK_RELEASE_SCL_SHIFT                  (13U)
#define I2C_DBGCTRL_NAK_RELEASE_SCL_MASK                   (0x1U << I2C_DBGCTRL_NAK_RELEASE_SCL_SHIFT)                  /* 0x00002000 */
#define I2C_DBGCTRL_H0_CHECK_SCL_SHIFT                     (14U)
#define I2C_DBGCTRL_H0_CHECK_SCL_MASK                      (0x1U << I2C_DBGCTRL_H0_CHECK_SCL_SHIFT)                     /* 0x00004000 */
/* CON1 */
#define I2C_CON1_OFFSET                                    (0x228U)
#define I2C_CON1_AUTO_STOP_SHIFT                           (0U)
#define I2C_CON1_AUTO_STOP_MASK                            (0x1U << I2C_CON1_AUTO_STOP_SHIFT)                           /* 0x00000001 */
#define I2C_CON1_AUTO_STOP_TX_END_SHIFT                    (1U)
#define I2C_CON1_AUTO_STOP_TX_END_MASK                     (0x1U << I2C_CON1_AUTO_STOP_TX_END_SHIFT)                    /* 0x00000002 */
#define I2C_CON1_AUTO_STOP_NAK_SHIFT                       (2U)
#define I2C_CON1_AUTO_STOP_NAK_MASK                        (0x1U << I2C_CON1_AUTO_STOP_NAK_SHIFT)                       /* 0x00000004 */
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_OFFSET                                    (0x0U)
#define UART_RBR                                           (0x0U)
#define UART_RBR_RBR_SHIFT                                 (0U)
#define UART_RBR_RBR_MASK                                  (0xFFU << UART_RBR_RBR_SHIFT)                                /* 0x000000FF */
/* DLL */
#define UART_DLL_OFFSET                                    (0x0U)
#define UART_DLL_DLL_SHIFT                                 (0U)
#define UART_DLL_DLL_MASK                                  (0xFFU << UART_DLL_DLL_SHIFT)                                /* 0x000000FF */
/* THR */
#define UART_THR_OFFSET                                    (0x0U)
#define UART_THR_THR_SHIFT                                 (0U)
#define UART_THR_THR_MASK                                  (0xFFU << UART_THR_THR_SHIFT)                                /* 0x000000FF */
/* DLH */
#define UART_DLH_OFFSET                                    (0x4U)
#define UART_DLH_DLH_SHIFT                                 (0U)
#define UART_DLH_DLH_MASK                                  (0xFFU << UART_DLH_DLH_SHIFT)                                /* 0x000000FF */
/* IER */
#define UART_IER_OFFSET                                    (0x4U)
#define UART_IER_ERBFI_SHIFT                               (0U)
#define UART_IER_ERBFI_MASK                                (0x1U << UART_IER_ERBFI_SHIFT)                               /* 0x00000001 */
#define UART_IER_ETBEI_SHIFT                               (1U)
#define UART_IER_ETBEI_MASK                                (0x1U << UART_IER_ETBEI_SHIFT)                               /* 0x00000002 */
#define UART_IER_ELSI_SHIFT                                (2U)
#define UART_IER_ELSI_MASK                                 (0x1U << UART_IER_ELSI_SHIFT)                                /* 0x00000004 */
#define UART_IER_EDSSI_SHIFT                               (3U)
#define UART_IER_EDSSI_MASK                                (0x1U << UART_IER_EDSSI_SHIFT)                               /* 0x00000008 */
#define UART_IER_PTIME_SHIFT                               (7U)
#define UART_IER_PTIME_MASK                                (0x1U << UART_IER_PTIME_SHIFT)                               /* 0x00000080 */
/* FCR */
#define UART_FCR_OFFSET                                    (0x8U)
#define UART_FCR_FIFOE_SHIFT                               (0U)
#define UART_FCR_FIFOE_MASK                                (0x1U << UART_FCR_FIFOE_SHIFT)                               /* 0x00000001 */
#define UART_FCR_RFIFOR_SHIFT                              (1U)
#define UART_FCR_RFIFOR_MASK                               (0x1U << UART_FCR_RFIFOR_SHIFT)                              /* 0x00000002 */
#define UART_FCR_XFIFOR_SHIFT                              (2U)
#define UART_FCR_XFIFOR_MASK                               (0x1U << UART_FCR_XFIFOR_SHIFT)                              /* 0x00000004 */
#define UART_FCR_DMAM_SHIFT                                (3U)
#define UART_FCR_DMAM_MASK                                 (0x1U << UART_FCR_DMAM_SHIFT)                                /* 0x00000008 */
#define UART_FCR_TET_SHIFT                                 (4U)
#define UART_FCR_TET_MASK                                  (0x3U << UART_FCR_TET_SHIFT)                                 /* 0x00000030 */
#define UART_FCR_RT_SHIFT                                  (6U)
#define UART_FCR_RT_MASK                                   (0x3U << UART_FCR_RT_SHIFT)                                  /* 0x000000C0 */
/* IIR */
#define UART_IIR_OFFSET                                    (0x8U)
#define UART_IIR                                           (0x1U)
#define UART_IIR_IID_SHIFT                                 (0U)
#define UART_IIR_IID_MASK                                  (0xFU << UART_IIR_IID_SHIFT)                                 /* 0x0000000F */
#define UART_IIR_FIFOSE_SHIFT                              (6U)
#define UART_IIR_FIFOSE_MASK                               (0x3U << UART_IIR_FIFOSE_SHIFT)                              /* 0x000000C0 */
/* LCR */
#define UART_LCR_OFFSET                                    (0xCU)
#define UART_LCR_DLS_SHIFT                                 (0U)
#define UART_LCR_DLS_MASK                                  (0x3U << UART_LCR_DLS_SHIFT)                                 /* 0x00000003 */
#define UART_LCR_STOP_SHIFT                                (2U)
#define UART_LCR_STOP_MASK                                 (0x1U << UART_LCR_STOP_SHIFT)                                /* 0x00000004 */
#define UART_LCR_PEN_SHIFT                                 (3U)
#define UART_LCR_PEN_MASK                                  (0x1U << UART_LCR_PEN_SHIFT)                                 /* 0x00000008 */
#define UART_LCR_EPS_SHIFT                                 (4U)
#define UART_LCR_EPS_MASK                                  (0x1U << UART_LCR_EPS_SHIFT)                                 /* 0x00000010 */
#define UART_LCR_BC_SHIFT                                  (6U)
#define UART_LCR_BC_MASK                                   (0x1U << UART_LCR_BC_SHIFT)                                  /* 0x00000040 */
#define UART_LCR_DLAB_SHIFT                                (7U)
#define UART_LCR_DLAB_MASK                                 (0x1U << UART_LCR_DLAB_SHIFT)                                /* 0x00000080 */
/* MCR */
#define UART_MCR_OFFSET                                    (0x10U)
#define UART_MCR_DTR_SHIFT                                 (0U)
#define UART_MCR_DTR_MASK                                  (0x1U << UART_MCR_DTR_SHIFT)                                 /* 0x00000001 */
#define UART_MCR_RTS_SHIFT                                 (1U)
#define UART_MCR_RTS_MASK                                  (0x1U << UART_MCR_RTS_SHIFT)                                 /* 0x00000002 */
#define UART_MCR_OUT1_SHIFT                                (2U)
#define UART_MCR_OUT1_MASK                                 (0x1U << UART_MCR_OUT1_SHIFT)                                /* 0x00000004 */
#define UART_MCR_OUT2_SHIFT                                (3U)
#define UART_MCR_OUT2_MASK                                 (0x1U << UART_MCR_OUT2_SHIFT)                                /* 0x00000008 */
#define UART_MCR_LB_SHIFT                                  (4U)
#define UART_MCR_LB_MASK                                   (0x1U << UART_MCR_LB_SHIFT)                                  /* 0x00000010 */
#define UART_MCR_AFCE_SHIFT                                (5U)
#define UART_MCR_AFCE_MASK                                 (0x1U << UART_MCR_AFCE_SHIFT)                                /* 0x00000020 */
#define UART_MCR_SIRE_SHIFT                                (6U)
#define UART_MCR_SIRE_MASK                                 (0x1U << UART_MCR_SIRE_SHIFT)                                /* 0x00000040 */
/* LSR */
#define UART_LSR_OFFSET                                    (0x14U)
#define UART_LSR                                           (0x60U)
#define UART_LSR_DR_SHIFT                                  (0U)
#define UART_LSR_DR_MASK                                   (0x1U << UART_LSR_DR_SHIFT)                                  /* 0x00000001 */
#define UART_LSR_OE_SHIFT                                  (1U)
#define UART_LSR_OE_MASK                                   (0x1U << UART_LSR_OE_SHIFT)                                  /* 0x00000002 */
#define UART_LSR_PE_SHIFT                                  (2U)
#define UART_LSR_PE_MASK                                   (0x1U << UART_LSR_PE_SHIFT)                                  /* 0x00000004 */
#define UART_LSR_FE_SHIFT                                  (3U)
#define UART_LSR_FE_MASK                                   (0x1U << UART_LSR_FE_SHIFT)                                  /* 0x00000008 */
#define UART_LSR_BI_SHIFT                                  (4U)
#define UART_LSR_BI_MASK                                   (0x1U << UART_LSR_BI_SHIFT)                                  /* 0x00000010 */
#define UART_LSR_THRE_SHIFT                                (5U)
#define UART_LSR_THRE_MASK                                 (0x1U << UART_LSR_THRE_SHIFT)                                /* 0x00000020 */
#define UART_LSR_TEMT_SHIFT                                (6U)
#define UART_LSR_TEMT_MASK                                 (0x1U << UART_LSR_TEMT_SHIFT)                                /* 0x00000040 */
#define UART_LSR_RFE_SHIFT                                 (7U)
#define UART_LSR_RFE_MASK                                  (0x1U << UART_LSR_RFE_SHIFT)                                 /* 0x00000080 */
/* MSR */
#define UART_MSR_OFFSET                                    (0x18U)
#define UART_MSR                                           (0x0U)
#define UART_MSR_DCTS_SHIFT                                (0U)
#define UART_MSR_DCTS_MASK                                 (0x1U << UART_MSR_DCTS_SHIFT)                                /* 0x00000001 */
#define UART_MSR_DDSR_SHIFT                                (1U)
#define UART_MSR_DDSR_MASK                                 (0x1U << UART_MSR_DDSR_SHIFT)                                /* 0x00000002 */
#define UART_MSR_TERI_SHIFT                                (2U)
#define UART_MSR_TERI_MASK                                 (0x1U << UART_MSR_TERI_SHIFT)                                /* 0x00000004 */
#define UART_MSR_DDCD_SHIFT                                (3U)
#define UART_MSR_DDCD_MASK                                 (0x1U << UART_MSR_DDCD_SHIFT)                                /* 0x00000008 */
#define UART_MSR_CTS_SHIFT                                 (4U)
#define UART_MSR_CTS_MASK                                  (0x1U << UART_MSR_CTS_SHIFT)                                 /* 0x00000010 */
#define UART_MSR_DSR_SHIFT                                 (5U)
#define UART_MSR_DSR_MASK                                  (0x1U << UART_MSR_DSR_SHIFT)                                 /* 0x00000020 */
#define UART_MSR_RI_SHIFT                                  (6U)
#define UART_MSR_RI_MASK                                   (0x1U << UART_MSR_RI_SHIFT)                                  /* 0x00000040 */
#define UART_MSR_DCD_SHIFT                                 (7U)
#define UART_MSR_DCD_MASK                                  (0x1U << UART_MSR_DCD_SHIFT)                                 /* 0x00000080 */
/* SCR */
#define UART_SCR_OFFSET                                    (0x1CU)
#define UART_SCR_SCR_SHIFT                                 (0U)
#define UART_SCR_SCR_MASK                                  (0xFFU << UART_SCR_SCR_SHIFT)                                /* 0x000000FF */
/* SRBR */
#define UART_SRBR_OFFSET                                   (0x30U)
#define UART_SRBR                                          (0x0U)
#define UART_SRBR_SRBR_SHIFT                               (0U)
#define UART_SRBR_SRBR_MASK                                (0xFFU << UART_SRBR_SRBR_SHIFT)                              /* 0x000000FF */
/* STHR */
#define UART_STHR_OFFSET                                   (0x30U)
#define UART_STHR_STHR_SHIFT                               (0U)
#define UART_STHR_STHR_MASK                                (0x1U << UART_STHR_STHR_SHIFT)                               /* 0x00000001 */
/* FAR */
#define UART_FAR_OFFSET                                    (0x70U)
#define UART_FAR_FAR_SHIFT                                 (0U)
#define UART_FAR_FAR_MASK                                  (0x1U << UART_FAR_FAR_SHIFT)                                 /* 0x00000001 */
/* TFR */
#define UART_TFR_OFFSET                                    (0x74U)
#define UART_TFR                                           (0x0U)
#define UART_TFR_TFR_SHIFT                                 (0U)
#define UART_TFR_TFR_MASK                                  (0xFFU << UART_TFR_TFR_SHIFT)                                /* 0x000000FF */
/* RFW */
#define UART_RFW_OFFSET                                    (0x78U)
#define UART_RFW_RFWD_SHIFT                                (0U)
#define UART_RFW_RFWD_MASK                                 (0xFFU << UART_RFW_RFWD_SHIFT)                               /* 0x000000FF */
#define UART_RFW_REPE_SHIFT                                (8U)
#define UART_RFW_REPE_MASK                                 (0x1U << UART_RFW_REPE_SHIFT)                                /* 0x00000100 */
#define UART_RFW_REFE_SHIFT                                (9U)
#define UART_RFW_REFE_MASK                                 (0x1U << UART_RFW_REFE_SHIFT)                                /* 0x00000200 */
/* USR */
#define UART_USR_OFFSET                                    (0x7CU)
#define UART_USR                                           (0x6U)
#define UART_USR_BUSY_SHIFT                                (0U)
#define UART_USR_BUSY_MASK                                 (0x1U << UART_USR_BUSY_SHIFT)                                /* 0x00000001 */
#define UART_USR_TFNF_SHIFT                                (1U)
#define UART_USR_TFNF_MASK                                 (0x1U << UART_USR_TFNF_SHIFT)                                /* 0x00000002 */
#define UART_USR_TFE_SHIFT                                 (2U)
#define UART_USR_TFE_MASK                                  (0x1U << UART_USR_TFE_SHIFT)                                 /* 0x00000004 */
#define UART_USR_RFNE_SHIFT                                (3U)
#define UART_USR_RFNE_MASK                                 (0x1U << UART_USR_RFNE_SHIFT)                                /* 0x00000008 */
#define UART_USR_RFE_SHIFT                                 (4U)
#define UART_USR_RFE_MASK                                  (0x1U << UART_USR_RFE_SHIFT)                                 /* 0x00000010 */
/* TFL */
#define UART_TFL_OFFSET                                    (0x80U)
#define UART_TFL                                           (0x0U)
#define UART_TFL_TFL_SHIFT                                 (0U)
#define UART_TFL_TFL_MASK                                  (0x3FU << UART_TFL_TFL_SHIFT)                                /* 0x0000003F */
/* RFL */
#define UART_RFL_OFFSET                                    (0x84U)
#define UART_RFL                                           (0x0U)
#define UART_RFL_RFL_SHIFT                                 (0U)
#define UART_RFL_RFL_MASK                                  (0x3FU << UART_RFL_RFL_SHIFT)                                /* 0x0000003F */
/* SRR */
#define UART_SRR_OFFSET                                    (0x88U)
#define UART_SRR_UR_SHIFT                                  (0U)
#define UART_SRR_UR_MASK                                   (0x1U << UART_SRR_UR_SHIFT)                                  /* 0x00000001 */
#define UART_SRR_RFR_SHIFT                                 (1U)
#define UART_SRR_RFR_MASK                                  (0x1U << UART_SRR_RFR_SHIFT)                                 /* 0x00000002 */
#define UART_SRR_XFR_SHIFT                                 (2U)
#define UART_SRR_XFR_MASK                                  (0x1U << UART_SRR_XFR_SHIFT)                                 /* 0x00000004 */
/* SRTS */
#define UART_SRTS_OFFSET                                   (0x8CU)
#define UART_SRTS_SRTS_SHIFT                               (0U)
#define UART_SRTS_SRTS_MASK                                (0x1U << UART_SRTS_SRTS_SHIFT)                               /* 0x00000001 */
/* SBCR */
#define UART_SBCR_OFFSET                                   (0x90U)
#define UART_SBCR_SBCR_SHIFT                               (0U)
#define UART_SBCR_SBCR_MASK                                (0x1U << UART_SBCR_SBCR_SHIFT)                               /* 0x00000001 */
/* SDMAM */
#define UART_SDMAM_OFFSET                                  (0x94U)
#define UART_SDMAM_SDMAM_SHIFT                             (0U)
#define UART_SDMAM_SDMAM_MASK                              (0x1U << UART_SDMAM_SDMAM_SHIFT)                             /* 0x00000001 */
/* SFE */
#define UART_SFE_OFFSET                                    (0x98U)
#define UART_SFE_SFE_SHIFT                                 (0U)
#define UART_SFE_SFE_MASK                                  (0x1U << UART_SFE_SFE_SHIFT)                                 /* 0x00000001 */
/* SRT */
#define UART_SRT_OFFSET                                    (0x9CU)
#define UART_SRT_SRT_SHIFT                                 (0U)
#define UART_SRT_SRT_MASK                                  (0x3U << UART_SRT_SRT_SHIFT)                                 /* 0x00000003 */
/* STET */
#define UART_STET_OFFSET                                   (0xA0U)
#define UART_STET_STET_SHIFT                               (0U)
#define UART_STET_STET_MASK                                (0x3U << UART_STET_STET_SHIFT)                               /* 0x00000003 */
/* HTX */
#define UART_HTX_OFFSET                                    (0xA4U)
#define UART_HTX_HTX_SHIFT                                 (0U)
#define UART_HTX_HTX_MASK                                  (0x1U << UART_HTX_HTX_SHIFT)                                 /* 0x00000001 */
/* DMASA */
#define UART_DMASA_OFFSET                                  (0xA8U)
#define UART_DMASA_DMASA_SHIFT                             (0U)
#define UART_DMASA_DMASA_MASK                              (0x1U << UART_DMASA_DMASA_SHIFT)                             /* 0x00000001 */
/* CPR */
#define UART_CPR_OFFSET                                    (0xF4U)
#define UART_CPR                                           (0x0U)
#define UART_CPR_APB_DATA_WIDTH_SHIFT                      (0U)
#define UART_CPR_APB_DATA_WIDTH_MASK                       (0x3U << UART_CPR_APB_DATA_WIDTH_SHIFT)                      /* 0x00000003 */
#define UART_CPR_AFCE_MODE_SHIFT                           (4U)
#define UART_CPR_AFCE_MODE_MASK                            (0x1U << UART_CPR_AFCE_MODE_SHIFT)                           /* 0x00000010 */
#define UART_CPR_THRE_MODE_SHIFT                           (5U)
#define UART_CPR_THRE_MODE_MASK                            (0x1U << UART_CPR_THRE_MODE_SHIFT)                           /* 0x00000020 */
#define UART_CPR_SIR_MODE_SHIFT                            (6U)
#define UART_CPR_SIR_MODE_MASK                             (0x1U << UART_CPR_SIR_MODE_SHIFT)                            /* 0x00000040 */
#define UART_CPR_SIR_LP_MODE_SHIFT                         (7U)
#define UART_CPR_SIR_LP_MODE_MASK                          (0x1U << UART_CPR_SIR_LP_MODE_SHIFT)                         /* 0x00000080 */
#define UART_CPR_NEW_FEAT_SHIFT                            (8U)
#define UART_CPR_NEW_FEAT_MASK                             (0x1U << UART_CPR_NEW_FEAT_SHIFT)                            /* 0x00000100 */
#define UART_CPR_FIFO_ACCESS_SHIFT                         (9U)
#define UART_CPR_FIFO_ACCESS_MASK                          (0x1U << UART_CPR_FIFO_ACCESS_SHIFT)                         /* 0x00000200 */
#define UART_CPR_FIFO_STAT_SHIFT                           (10U)
#define UART_CPR_FIFO_STAT_MASK                            (0x1U << UART_CPR_FIFO_STAT_SHIFT)                           /* 0x00000400 */
#define UART_CPR_SHADOW_SHIFT                              (11U)
#define UART_CPR_SHADOW_MASK                               (0x1U << UART_CPR_SHADOW_SHIFT)                              /* 0x00000800 */
#define UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT             (12U)
#define UART_CPR_UART_ADD_ENCODED_PARAMS_MASK              (0x1U << UART_CPR_UART_ADD_ENCODED_PARAMS_SHIFT)             /* 0x00001000 */
#define UART_CPR_DMA_EXTRA_SHIFT                           (13U)
#define UART_CPR_DMA_EXTRA_MASK                            (0x1U << UART_CPR_DMA_EXTRA_SHIFT)                           /* 0x00002000 */
#define UART_CPR_FIFO_MODE_SHIFT                           (16U)
#define UART_CPR_FIFO_MODE_MASK                            (0xFFU << UART_CPR_FIFO_MODE_SHIFT)                          /* 0x00FF0000 */
/* UCV */
#define UART_UCV_OFFSET                                    (0xF8U)
#define UART_UCV                                           (0x0U)
#define UART_UCV_UCV_SHIFT                                 (0U)
#define UART_UCV_UCV_MASK                                  (0xFFFFFFFFU << UART_UCV_UCV_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_OFFSET                                    (0xFCU)
#define UART_CTR_ID_SHIFT                                  (0U)
#define UART_CTR_ID_MASK                                   (0xFFFFFFFFU << UART_CTR_ID_SHIFT)                           /* 0xFFFFFFFF */
/******************************************PWM*******************************************/
/* PWM0_CNT */
#define PWM_PWM0_CNT_OFFSET                                (0x0U)
#define PWM_PWM0_CNT                                       (0x0U)
#define PWM_PWM0_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM0_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM0_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM0_PERIOD_HPR */
#define PWM_PWM0_PERIOD_HPR_OFFSET                         (0x4U)
#define PWM_PWM0_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM0_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM0_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM0_DUTY_LPR */
#define PWM_PWM0_DUTY_LPR_OFFSET                           (0x8U)
#define PWM_PWM0_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM0_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM0_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM0_CTRL */
#define PWM_PWM0_CTRL_OFFSET                               (0xCU)
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
#define PWM_PWM0_CTRL_CONLOCK_SHIFT                        (6U)
#define PWM_PWM0_CTRL_CONLOCK_MASK                         (0x1U << PWM_PWM0_CTRL_CONLOCK_SHIFT)                        /* 0x00000040 */
#define PWM_PWM0_CTRL_CH_CNT_EN_SHIFT                      (7U)
#define PWM_PWM0_CTRL_CH_CNT_EN_MASK                       (0x1U << PWM_PWM0_CTRL_CH_CNT_EN_SHIFT)                      /* 0x00000080 */
#define PWM_PWM0_CTRL_FORCE_CLK_EN_SHIFT                   (8U)
#define PWM_PWM0_CTRL_FORCE_CLK_EN_MASK                    (0x1U << PWM_PWM0_CTRL_FORCE_CLK_EN_SHIFT)                   /* 0x00000100 */
#define PWM_PWM0_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM0_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM0_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM0_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM0_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM0_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM0_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM0_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM0_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM0_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM0_CTRL_RPT_MASK                             (0xFFU << PWM_PWM0_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* PWM1_CNT */
#define PWM_PWM1_CNT_OFFSET                                (0x10U)
#define PWM_PWM1_CNT                                       (0x0U)
#define PWM_PWM1_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM1_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM1_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM1_PERIOD_HPR */
#define PWM_PWM1_PERIOD_HPR_OFFSET                         (0x14U)
#define PWM_PWM1_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM1_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM1_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM1_DUTY_LPR */
#define PWM_PWM1_DUTY_LPR_OFFSET                           (0x18U)
#define PWM_PWM1_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM1_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM1_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM1_CTRL */
#define PWM_PWM1_CTRL_OFFSET                               (0x1CU)
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
#define PWM_PWM1_CTRL_CONLOCK_SHIFT                        (6U)
#define PWM_PWM1_CTRL_CONLOCK_MASK                         (0x1U << PWM_PWM1_CTRL_CONLOCK_SHIFT)                        /* 0x00000040 */
#define PWM_PWM1_CTRL_CH_CNT_EN_SHIFT                      (7U)
#define PWM_PWM1_CTRL_CH_CNT_EN_MASK                       (0x1U << PWM_PWM1_CTRL_CH_CNT_EN_SHIFT)                      /* 0x00000080 */
#define PWM_PWM1_CTRL_FORCE_CLK_EN_SHIFT                   (8U)
#define PWM_PWM1_CTRL_FORCE_CLK_EN_MASK                    (0x1U << PWM_PWM1_CTRL_FORCE_CLK_EN_SHIFT)                   /* 0x00000100 */
#define PWM_PWM1_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM1_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM1_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM1_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM1_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM1_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM1_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM1_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM1_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM1_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM1_CTRL_RPT_MASK                             (0xFFU << PWM_PWM1_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* PWM2_CNT */
#define PWM_PWM2_CNT_OFFSET                                (0x20U)
#define PWM_PWM2_CNT                                       (0x0U)
#define PWM_PWM2_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM2_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM2_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM2_PERIOD_HPR */
#define PWM_PWM2_PERIOD_HPR_OFFSET                         (0x24U)
#define PWM_PWM2_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM2_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM2_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM2_DUTY_LPR */
#define PWM_PWM2_DUTY_LPR_OFFSET                           (0x28U)
#define PWM_PWM2_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM2_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM2_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM2_CTRL */
#define PWM_PWM2_CTRL_OFFSET                               (0x2CU)
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
#define PWM_PWM2_CTRL_CONLOCK_SHIFT                        (6U)
#define PWM_PWM2_CTRL_CONLOCK_MASK                         (0x1U << PWM_PWM2_CTRL_CONLOCK_SHIFT)                        /* 0x00000040 */
#define PWM_PWM2_CTRL_CH_CNT_EN_SHIFT                      (7U)
#define PWM_PWM2_CTRL_CH_CNT_EN_MASK                       (0x1U << PWM_PWM2_CTRL_CH_CNT_EN_SHIFT)                      /* 0x00000080 */
#define PWM_PWM2_CTRL_FORCE_CLK_EN_SHIFT                   (8U)
#define PWM_PWM2_CTRL_FORCE_CLK_EN_MASK                    (0x1U << PWM_PWM2_CTRL_FORCE_CLK_EN_SHIFT)                   /* 0x00000100 */
#define PWM_PWM2_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM2_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM2_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM2_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM2_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM2_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM2_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM2_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM2_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM2_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM2_CTRL_RPT_MASK                             (0xFFU << PWM_PWM2_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* PWM3_CNT */
#define PWM_PWM3_CNT_OFFSET                                (0x30U)
#define PWM_PWM3_CNT                                       (0x0U)
#define PWM_PWM3_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM3_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM3_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM3_PERIOD_HPR */
#define PWM_PWM3_PERIOD_HPR_OFFSET                         (0x34U)
#define PWM_PWM3_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM3_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM3_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
/* PWM3_DUTY_LPR */
#define PWM_PWM3_DUTY_LPR_OFFSET                           (0x38U)
#define PWM_PWM3_DUTY_LPR_DUTY_LPR_SHIFT                   (0U)
#define PWM_PWM3_DUTY_LPR_DUTY_LPR_MASK                    (0xFFFFFFFFU << PWM_PWM3_DUTY_LPR_DUTY_LPR_SHIFT)            /* 0xFFFFFFFF */
/* PWM3_CTRL */
#define PWM_PWM3_CTRL_OFFSET                               (0x3CU)
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
#define PWM_PWM3_CTRL_CONLOCK_SHIFT                        (6U)
#define PWM_PWM3_CTRL_CONLOCK_MASK                         (0x1U << PWM_PWM3_CTRL_CONLOCK_SHIFT)                        /* 0x00000040 */
#define PWM_PWM3_CTRL_CH_CNT_EN_SHIFT                      (7U)
#define PWM_PWM3_CTRL_CH_CNT_EN_MASK                       (0x1U << PWM_PWM3_CTRL_CH_CNT_EN_SHIFT)                      /* 0x00000080 */
#define PWM_PWM3_CTRL_FORCE_CLK_EN_SHIFT                   (8U)
#define PWM_PWM3_CTRL_FORCE_CLK_EN_MASK                    (0x1U << PWM_PWM3_CTRL_FORCE_CLK_EN_SHIFT)                   /* 0x00000100 */
#define PWM_PWM3_CTRL_CLK_SEL_SHIFT                        (9U)
#define PWM_PWM3_CTRL_CLK_SEL_MASK                         (0x1U << PWM_PWM3_CTRL_CLK_SEL_SHIFT)                        /* 0x00000200 */
#define PWM_PWM3_CTRL_PRESCALE_SHIFT                       (12U)
#define PWM_PWM3_CTRL_PRESCALE_MASK                        (0x7U << PWM_PWM3_CTRL_PRESCALE_SHIFT)                       /* 0x00007000 */
#define PWM_PWM3_CTRL_SCALE_SHIFT                          (16U)
#define PWM_PWM3_CTRL_SCALE_MASK                           (0xFFU << PWM_PWM3_CTRL_SCALE_SHIFT)                         /* 0x00FF0000 */
#define PWM_PWM3_CTRL_RPT_SHIFT                            (24U)
#define PWM_PWM3_CTRL_RPT_MASK                             (0xFFU << PWM_PWM3_CTRL_RPT_SHIFT)                           /* 0xFF000000 */
/* INTSTS */
#define PWM_INTSTS_OFFSET                                  (0x40U)
#define PWM_INTSTS_CH0_INTSTS_SHIFT                        (0U)
#define PWM_INTSTS_CH0_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH0_INTSTS_SHIFT)                        /* 0x00000001 */
#define PWM_INTSTS_CH1_INTSTS_SHIFT                        (1U)
#define PWM_INTSTS_CH1_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH1_INTSTS_SHIFT)                        /* 0x00000002 */
#define PWM_INTSTS_CH2_INTSTS_SHIFT                        (2U)
#define PWM_INTSTS_CH2_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH2_INTSTS_SHIFT)                        /* 0x00000004 */
#define PWM_INTSTS_CH3_INTSTS_SHIFT                        (3U)
#define PWM_INTSTS_CH3_INTSTS_MASK                         (0x1U << PWM_INTSTS_CH3_INTSTS_SHIFT)                        /* 0x00000008 */
#define PWM_INTSTS_CH0_PWR_INTSTS_SHIFT                    (4U)
#define PWM_INTSTS_CH0_PWR_INTSTS_MASK                     (0x1U << PWM_INTSTS_CH0_PWR_INTSTS_SHIFT)                    /* 0x00000010 */
#define PWM_INTSTS_CH1_PWR_INTSTS_SHIFT                    (5U)
#define PWM_INTSTS_CH1_PWR_INTSTS_MASK                     (0x1U << PWM_INTSTS_CH1_PWR_INTSTS_SHIFT)                    /* 0x00000020 */
#define PWM_INTSTS_CH2_PWR_INTSTS_SHIFT                    (6U)
#define PWM_INTSTS_CH2_PWR_INTSTS_MASK                     (0x1U << PWM_INTSTS_CH2_PWR_INTSTS_SHIFT)                    /* 0x00000040 */
#define PWM_INTSTS_CH3_PWR_INTSTS_SHIFT                    (7U)
#define PWM_INTSTS_CH3_PWR_INTSTS_MASK                     (0x1U << PWM_INTSTS_CH3_PWR_INTSTS_SHIFT)                    /* 0x00000080 */
#define PWM_INTSTS_CH0_POL_SHIFT                           (8U)
#define PWM_INTSTS_CH0_POL_MASK                            (0x1U << PWM_INTSTS_CH0_POL_SHIFT)                           /* 0x00000100 */
#define PWM_INTSTS_CH1_POL_SHIFT                           (9U)
#define PWM_INTSTS_CH1_POL_MASK                            (0x1U << PWM_INTSTS_CH1_POL_SHIFT)                           /* 0x00000200 */
#define PWM_INTSTS_CH2_POL_SHIFT                           (10U)
#define PWM_INTSTS_CH2_POL_MASK                            (0x1U << PWM_INTSTS_CH2_POL_SHIFT)                           /* 0x00000400 */
#define PWM_INTSTS_CH3_POL_SHIFT                           (11U)
#define PWM_INTSTS_CH3_POL_MASK                            (0x1U << PWM_INTSTS_CH3_POL_SHIFT)                           /* 0x00000800 */
/* INT_EN */
#define PWM_INT_EN_OFFSET                                  (0x44U)
#define PWM_INT_EN_CH0_INT_EN_SHIFT                        (0U)
#define PWM_INT_EN_CH0_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH0_INT_EN_SHIFT)                        /* 0x00000001 */
#define PWM_INT_EN_CH1_INT_EN_SHIFT                        (1U)
#define PWM_INT_EN_CH1_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH1_INT_EN_SHIFT)                        /* 0x00000002 */
#define PWM_INT_EN_CH2_INT_EN_SHIFT                        (2U)
#define PWM_INT_EN_CH2_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH2_INT_EN_SHIFT)                        /* 0x00000004 */
#define PWM_INT_EN_CH3_INT_EN_SHIFT                        (3U)
#define PWM_INT_EN_CH3_INT_EN_MASK                         (0x1U << PWM_INT_EN_CH3_INT_EN_SHIFT)                        /* 0x00000008 */
#define PWM_INT_EN_CH0_PWR_INT_EN_SHIFT                    (4U)
#define PWM_INT_EN_CH0_PWR_INT_EN_MASK                     (0x1U << PWM_INT_EN_CH0_PWR_INT_EN_SHIFT)                    /* 0x00000010 */
#define PWM_INT_EN_CH1_PWR_INT_EN_SHIFT                    (5U)
#define PWM_INT_EN_CH1_PWR_INT_EN_MASK                     (0x1U << PWM_INT_EN_CH1_PWR_INT_EN_SHIFT)                    /* 0x00000020 */
#define PWM_INT_EN_CH2_PWR_INT_EN_SHIFT                    (6U)
#define PWM_INT_EN_CH2_PWR_INT_EN_MASK                     (0x1U << PWM_INT_EN_CH2_PWR_INT_EN_SHIFT)                    /* 0x00000040 */
#define PWM_INT_EN_CH3_PWR_INT_EN_SHIFT                    (7U)
#define PWM_INT_EN_CH3_PWR_INT_EN_MASK                     (0x1U << PWM_INT_EN_CH3_PWR_INT_EN_SHIFT)                    /* 0x00000080 */
/* FIFO_CTRL */
#define PWM_FIFO_CTRL_OFFSET                               (0x50U)
#define PWM_FIFO_CTRL_FIFO_MODE_SEL_SHIFT                  (0U)
#define PWM_FIFO_CTRL_FIFO_MODE_SEL_MASK                   (0x1U << PWM_FIFO_CTRL_FIFO_MODE_SEL_SHIFT)                  /* 0x00000001 */
#define PWM_FIFO_CTRL_FULL_INT_EN_SHIFT                    (1U)
#define PWM_FIFO_CTRL_FULL_INT_EN_MASK                     (0x1U << PWM_FIFO_CTRL_FULL_INT_EN_SHIFT)                    /* 0x00000002 */
#define PWM_FIFO_CTRL_OVERFLOW_INT_EN_SHIFT                (2U)
#define PWM_FIFO_CTRL_OVERFLOW_INT_EN_MASK                 (0x1U << PWM_FIFO_CTRL_OVERFLOW_INT_EN_SHIFT)                /* 0x00000004 */
#define PWM_FIFO_CTRL_WATERMARK_INT_EN_SHIFT               (3U)
#define PWM_FIFO_CTRL_WATERMARK_INT_EN_MASK                (0x1U << PWM_FIFO_CTRL_WATERMARK_INT_EN_SHIFT)               /* 0x00000008 */
#define PWM_FIFO_CTRL_ALMOST_FULL_WATERMARK_SHIFT          (4U)
#define PWM_FIFO_CTRL_ALMOST_FULL_WATERMARK_MASK           (0x7U << PWM_FIFO_CTRL_ALMOST_FULL_WATERMARK_SHIFT)          /* 0x00000070 */
#define PWM_FIFO_CTRL_DMA_MODE_EN_SHIFT                    (8U)
#define PWM_FIFO_CTRL_DMA_MODE_EN_MASK                     (0x1U << PWM_FIFO_CTRL_DMA_MODE_EN_SHIFT)                    /* 0x00000100 */
#define PWM_FIFO_CTRL_TIMEOUT_EN_SHIFT                     (9U)
#define PWM_FIFO_CTRL_TIMEOUT_EN_MASK                      (0x1U << PWM_FIFO_CTRL_TIMEOUT_EN_SHIFT)                     /* 0x00000200 */
#define PWM_FIFO_CTRL_DMA_CH_SEL_EN_SHIFT                  (10U)
#define PWM_FIFO_CTRL_DMA_CH_SEL_EN_MASK                   (0x1U << PWM_FIFO_CTRL_DMA_CH_SEL_EN_SHIFT)                  /* 0x00000400 */
#define PWM_FIFO_CTRL_DMA_CH_SEL_SHIFT                     (12U)
#define PWM_FIFO_CTRL_DMA_CH_SEL_MASK                      (0x3U << PWM_FIFO_CTRL_DMA_CH_SEL_SHIFT)                     /* 0x00003000 */
/* FIFO_INTSTS */
#define PWM_FIFO_INTSTS_OFFSET                             (0x54U)
#define PWM_FIFO_INTSTS_FIFO_FULL_INTSTS_SHIFT             (0U)
#define PWM_FIFO_INTSTS_FIFO_FULL_INTSTS_MASK              (0x1U << PWM_FIFO_INTSTS_FIFO_FULL_INTSTS_SHIFT)             /* 0x00000001 */
#define PWM_FIFO_INTSTS_FIFO_OVERFLOW_INTSTS_SHIFT         (1U)
#define PWM_FIFO_INTSTS_FIFO_OVERFLOW_INTSTS_MASK          (0x1U << PWM_FIFO_INTSTS_FIFO_OVERFLOW_INTSTS_SHIFT)         /* 0x00000002 */
#define PWM_FIFO_INTSTS_FIFO_WATERMARK_FULL_INTSTS_SHIFT   (2U)
#define PWM_FIFO_INTSTS_FIFO_WATERMARK_FULL_INTSTS_MASK    (0x1U << PWM_FIFO_INTSTS_FIFO_WATERMARK_FULL_INTSTS_SHIFT)   /* 0x00000004 */
#define PWM_FIFO_INTSTS_TIMIEOUT_INTSTS_SHIFT              (3U)
#define PWM_FIFO_INTSTS_TIMIEOUT_INTSTS_MASK               (0x1U << PWM_FIFO_INTSTS_TIMIEOUT_INTSTS_SHIFT)              /* 0x00000008 */
#define PWM_FIFO_INTSTS_FIFO_EMPTY_STATUS_SHIFT            (4U)
#define PWM_FIFO_INTSTS_FIFO_EMPTY_STATUS_MASK             (0x1U << PWM_FIFO_INTSTS_FIFO_EMPTY_STATUS_SHIFT)            /* 0x00000010 */
/* FIFO_TOUTTHR */
#define PWM_FIFO_TOUTTHR_OFFSET                            (0x58U)
#define PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_SHIFT           (0U)
#define PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_MASK            (0xFFFFFU << PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_SHIFT)       /* 0x000FFFFF */
/* VERSION_ID */
#define PWM_VERSION_ID_OFFSET                              (0x5CU)
#define PWM_VERSION_ID_SVN_VERSION_SHIFT                   (0U)
#define PWM_VERSION_ID_SVN_VERSION_MASK                    (0xFFFFU << PWM_VERSION_ID_SVN_VERSION_SHIFT)                /* 0x0000FFFF */
#define PWM_VERSION_ID_MINOR_VERSION_SHIFT                 (16U)
#define PWM_VERSION_ID_MINOR_VERSION_MASK                  (0xFFU << PWM_VERSION_ID_MINOR_VERSION_SHIFT)                /* 0x00FF0000 */
#define PWM_VERSION_ID_MAIN_VERSION_SHIFT                  (24U)
#define PWM_VERSION_ID_MAIN_VERSION_MASK                   (0xFFU << PWM_VERSION_ID_MAIN_VERSION_SHIFT)                 /* 0xFF000000 */
/* FIFO */
#define PWM_FIFO_OFFSET                                    (0x60U)
#define PWM_FIFO                                           (0x0U)
#define PWM_FIFO_CYCLE_CNT_SHIFT                           (0U)
#define PWM_FIFO_CYCLE_CNT_MASK                            (0x7FFFFFFFU << PWM_FIFO_CYCLE_CNT_SHIFT)                    /* 0x7FFFFFFF */
#define PWM_FIFO_POL_SHIFT                                 (31U)
#define PWM_FIFO_POL_MASK                                  (0x1U << PWM_FIFO_POL_SHIFT)                                 /* 0x80000000 */
/* PWRMATCH_CTRL */
#define PWM_PWRMATCH_CTRL_OFFSET                           (0x80U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_SHIFT          (3U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_MASK           (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_SHIFT)          /* 0x00000008 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_SHIFT        (7U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_SHIFT)        /* 0x00000080 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_SHIFT    (11U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_MASK     (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_SHIFT)    /* 0x00000800 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_SHIFT        (15U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_SHIFT)        /* 0x00008000 */
/* PWRMATCH_LPRE */
#define PWM_PWRMATCH_LPRE_OFFSET                           (0x84U)
#define PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT                    (0U)
#define PWM_PWRMATCH_LPRE_CNT_MIN_MASK                     (0xFFFFU << PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT)                 /* 0x0000FFFF */
#define PWM_PWRMATCH_LPRE_CNT_MAX_SHIFT                    (16U)
#define PWM_PWRMATCH_LPRE_CNT_MAX_MASK                     (0xFFFFU << PWM_PWRMATCH_LPRE_CNT_MAX_SHIFT)                 /* 0xFFFF0000 */
/* PWRMATCH_HPRE */
#define PWM_PWRMATCH_HPRE_OFFSET                           (0x88U)
#define PWM_PWRMATCH_HPRE_CNT_MIN_SHIFT                    (0U)
#define PWM_PWRMATCH_HPRE_CNT_MIN_MASK                     (0xFFFFU << PWM_PWRMATCH_HPRE_CNT_MIN_SHIFT)                 /* 0x0000FFFF */
#define PWM_PWRMATCH_HPRE_CNT_MAX_SHIFT                    (16U)
#define PWM_PWRMATCH_HPRE_CNT_MAX_MASK                     (0xFFFFU << PWM_PWRMATCH_HPRE_CNT_MAX_SHIFT)                 /* 0xFFFF0000 */
/* PWRMATCH_LD */
#define PWM_PWRMATCH_LD_OFFSET                             (0x8CU)
#define PWM_PWRMATCH_LD_CNT_MIN_SHIFT                      (0U)
#define PWM_PWRMATCH_LD_CNT_MIN_MASK                       (0xFFFFU << PWM_PWRMATCH_LD_CNT_MIN_SHIFT)                   /* 0x0000FFFF */
#define PWM_PWRMATCH_LD_CNT_MAX_SHIFT                      (16U)
#define PWM_PWRMATCH_LD_CNT_MAX_MASK                       (0xFFFFU << PWM_PWRMATCH_LD_CNT_MAX_SHIFT)                   /* 0xFFFF0000 */
/* PWRMATCH_HD_ZERO */
#define PWM_PWRMATCH_HD_ZERO_OFFSET                        (0x90U)
#define PWM_PWRMATCH_HD_ZERO_CNT_MIN_SHIFT                 (0U)
#define PWM_PWRMATCH_HD_ZERO_CNT_MIN_MASK                  (0xFFFFU << PWM_PWRMATCH_HD_ZERO_CNT_MIN_SHIFT)              /* 0x0000FFFF */
#define PWM_PWRMATCH_HD_ZERO_CNT_MAX_SHIFT                 (16U)
#define PWM_PWRMATCH_HD_ZERO_CNT_MAX_MASK                  (0xFFFFU << PWM_PWRMATCH_HD_ZERO_CNT_MAX_SHIFT)              /* 0xFFFF0000 */
/* PWRMATCH_HD_ONE */
#define PWM_PWRMATCH_HD_ONE_OFFSET                         (0x94U)
#define PWM_PWRMATCH_HD_ONE_CNT_MIN_SHIFT                  (0U)
#define PWM_PWRMATCH_HD_ONE_CNT_MIN_MASK                   (0xFFFFU << PWM_PWRMATCH_HD_ONE_CNT_MIN_SHIFT)               /* 0x0000FFFF */
#define PWM_PWRMATCH_HD_ONE_CNT_MAX_SHIFT                  (16U)
#define PWM_PWRMATCH_HD_ONE_CNT_MAX_MASK                   (0xFFFFU << PWM_PWRMATCH_HD_ONE_CNT_MAX_SHIFT)               /* 0xFFFF0000 */
/* PWRMATCH_VALUE0 */
#define PWM_PWRMATCH_VALUE0_OFFSET                         (0x98U)
#define PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE1 */
#define PWM_PWRMATCH_VALUE1_OFFSET                         (0x9CU)
#define PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE2 */
#define PWM_PWRMATCH_VALUE2_OFFSET                         (0xA0U)
#define PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE3 */
#define PWM_PWRMATCH_VALUE3_OFFSET                         (0xA4U)
#define PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE4 */
#define PWM_PWRMATCH_VALUE4_OFFSET                         (0xA8U)
#define PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE5 */
#define PWM_PWRMATCH_VALUE5_OFFSET                         (0xACU)
#define PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE6 */
#define PWM_PWRMATCH_VALUE6_OFFSET                         (0xB0U)
#define PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE7 */
#define PWM_PWRMATCH_VALUE7_OFFSET                         (0xB4U)
#define PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE8 */
#define PWM_PWRMATCH_VALUE8_OFFSET                         (0xB8U)
#define PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE9 */
#define PWM_PWRMATCH_VALUE9_OFFSET                         (0xBCU)
#define PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWM3_PWRCAPTURE_VALUE */
#define PWM_PWM3_PWRCAPTURE_VALUE_OFFSET                   (0xCCU)
#define PWM_PWM3_PWRCAPTURE_VALUE                          (0x0U)
#define PWM_PWM3_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT (0U)
#define PWM_PWM3_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_MASK (0xFFFFFFFFU << PWM_PWM3_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT) /* 0xFFFFFFFF */
/* FILTER_CTRL */
#define PWM_FILTER_CTRL_OFFSET                             (0xD0U)
#define PWM_FILTER_CTRL_CH0_INPUT_FILTER_ENABLE_SHIFT      (0U)
#define PWM_FILTER_CTRL_CH0_INPUT_FILTER_ENABLE_MASK       (0x1U << PWM_FILTER_CTRL_CH0_INPUT_FILTER_ENABLE_SHIFT)      /* 0x00000001 */
#define PWM_FILTER_CTRL_CH1_INPUT_FILTER_ENABLE_SHIFT      (1U)
#define PWM_FILTER_CTRL_CH1_INPUT_FILTER_ENABLE_MASK       (0x1U << PWM_FILTER_CTRL_CH1_INPUT_FILTER_ENABLE_SHIFT)      /* 0x00000002 */
#define PWM_FILTER_CTRL_CH2_INPUT_FILTER_ENABLE_SHIFT      (2U)
#define PWM_FILTER_CTRL_CH2_INPUT_FILTER_ENABLE_MASK       (0x1U << PWM_FILTER_CTRL_CH2_INPUT_FILTER_ENABLE_SHIFT)      /* 0x00000004 */
#define PWM_FILTER_CTRL_CH3_INPUT_FILTER_ENABLE_SHIFT      (3U)
#define PWM_FILTER_CTRL_CH3_INPUT_FILTER_ENABLE_MASK       (0x1U << PWM_FILTER_CTRL_CH3_INPUT_FILTER_ENABLE_SHIFT)      /* 0x00000008 */
#define PWM_FILTER_CTRL_FILTER_NUMBER_SHIFT                (4U)
#define PWM_FILTER_CTRL_FILTER_NUMBER_MASK                 (0x1FFU << PWM_FILTER_CTRL_FILTER_NUMBER_SHIFT)              /* 0x00001FF0 */
/*****************************************SARADC*****************************************/
/* DATA */
#define SARADC_DATA_OFFSET                                 (0x0U)
#define SARADC_DATA                                        (0x0U)
#define SARADC_DATA_ADC_DATA_SHIFT                         (0U)
#define SARADC_DATA_ADC_DATA_MASK                          (0x3FFU << SARADC_DATA_ADC_DATA_SHIFT)                       /* 0x000003FF */
/* STAS */
#define SARADC_STAS_OFFSET                                 (0x4U)
#define SARADC_STAS                                        (0x0U)
#define SARADC_STAS_ADC_STATUS_SHIFT                       (0U)
#define SARADC_STAS_ADC_STATUS_MASK                        (0x1U << SARADC_STAS_ADC_STATUS_SHIFT)                       /* 0x00000001 */
/* CTRL */
#define SARADC_CTRL_OFFSET                                 (0x8U)
#define SARADC_CTRL_ADC_INPUT_SRC_SEL_SHIFT                (0U)
#define SARADC_CTRL_ADC_INPUT_SRC_SEL_MASK                 (0x7U << SARADC_CTRL_ADC_INPUT_SRC_SEL_SHIFT)                /* 0x00000007 */
#define SARADC_CTRL_ADC_POWER_CTRL_SHIFT                   (3U)
#define SARADC_CTRL_ADC_POWER_CTRL_MASK                    (0x1U << SARADC_CTRL_ADC_POWER_CTRL_SHIFT)                   /* 0x00000008 */
#define SARADC_CTRL_INT_EN_SHIFT                           (5U)
#define SARADC_CTRL_INT_EN_MASK                            (0x1U << SARADC_CTRL_INT_EN_SHIFT)                           /* 0x00000020 */
#define SARADC_CTRL_INT_STATUS_SHIFT                       (6U)
#define SARADC_CTRL_INT_STATUS_MASK                        (0x1U << SARADC_CTRL_INT_STATUS_SHIFT)                       /* 0x00000040 */
/* DLY_PU_SOC */
#define SARADC_DLY_PU_SOC_OFFSET                           (0xCU)
#define SARADC_DLY_PU_SOC_DLY_PU_SOC_SHIFT                 (0U)
#define SARADC_DLY_PU_SOC_DLY_PU_SOC_MASK                  (0x3FU << SARADC_DLY_PU_SOC_DLY_PU_SOC_SHIFT)                /* 0x0000003F */
/******************************************SPI*******************************************/
/* CTRLR0 */
#define SPI_CTRLR0_OFFSET                                  (0x0U)
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
#define SPI_CTRLR0_SOI_SHIFT                               (23U)
#define SPI_CTRLR0_SOI_MASK                                (0x3U << SPI_CTRLR0_SOI_SHIFT)                               /* 0x01800000 */
#define SPI_CTRLR0_LBK_SHIFT                               (25U)
#define SPI_CTRLR0_LBK_MASK                                (0x1U << SPI_CTRLR0_LBK_SHIFT)                               /* 0x02000000 */
/* CTRLR1 */
#define SPI_CTRLR1_OFFSET                                  (0x4U)
#define SPI_CTRLR1_NDM_SHIFT                               (0U)
#define SPI_CTRLR1_NDM_MASK                                (0xFFFFFFFFU << SPI_CTRLR1_NDM_SHIFT)                        /* 0xFFFFFFFF */
/* ENR */
#define SPI_ENR_OFFSET                                     (0x8U)
#define SPI_ENR_ENR_SHIFT                                  (0U)
#define SPI_ENR_ENR_MASK                                   (0x1U << SPI_ENR_ENR_SHIFT)                                  /* 0x00000001 */
/* SER */
#define SPI_SER_OFFSET                                     (0xCU)
#define SPI_SER_SER_SHIFT                                  (0U)
#define SPI_SER_SER_MASK                                   (0x3U << SPI_SER_SER_SHIFT)                                  /* 0x00000003 */
/* BAUDR */
#define SPI_BAUDR_OFFSET                                   (0x10U)
#define SPI_BAUDR_BAUDR_SHIFT                              (0U)
#define SPI_BAUDR_BAUDR_MASK                               (0xFFFFU << SPI_BAUDR_BAUDR_SHIFT)                           /* 0x0000FFFF */
/* TXFTLR */
#define SPI_TXFTLR_OFFSET                                  (0x14U)
#define SPI_TXFTLR_TXFTLR_SHIFT                            (0U)
#define SPI_TXFTLR_TXFTLR_MASK                             (0x3FU << SPI_TXFTLR_TXFTLR_SHIFT)                           /* 0x0000003F */
/* RXFTLR */
#define SPI_RXFTLR_OFFSET                                  (0x18U)
#define SPI_RXFTLR_RXFTLR_SHIFT                            (0U)
#define SPI_RXFTLR_RXFTLR_MASK                             (0x3FU << SPI_RXFTLR_RXFTLR_SHIFT)                           /* 0x0000003F */
/* TXFLR */
#define SPI_TXFLR_OFFSET                                   (0x1CU)
#define SPI_TXFLR                                          (0x0U)
#define SPI_TXFLR_TXFLR_SHIFT                              (0U)
#define SPI_TXFLR_TXFLR_MASK                               (0x7FU << SPI_TXFLR_TXFLR_SHIFT)                             /* 0x0000007F */
/* RXFLR */
#define SPI_RXFLR_OFFSET                                   (0x20U)
#define SPI_RXFLR                                          (0x0U)
#define SPI_RXFLR_RXFLR_SHIFT                              (0U)
#define SPI_RXFLR_RXFLR_MASK                               (0x7FU << SPI_RXFLR_RXFLR_SHIFT)                             /* 0x0000007F */
/* SR */
#define SPI_SR_OFFSET                                      (0x24U)
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
#define SPI_SR_STB_SHIFT                                   (5U)
#define SPI_SR_STB_MASK                                    (0x1U << SPI_SR_STB_SHIFT)                                   /* 0x00000020 */
#define SPI_SR_SSI_SHIFT                                   (6U)
#define SPI_SR_SSI_MASK                                    (0x1U << SPI_SR_SSI_SHIFT)                                   /* 0x00000040 */
/* IPR */
#define SPI_IPR_OFFSET                                     (0x28U)
#define SPI_IPR_IPR_SHIFT                                  (0U)
#define SPI_IPR_IPR_MASK                                   (0x1U << SPI_IPR_IPR_SHIFT)                                  /* 0x00000001 */
/* IMR */
#define SPI_IMR_OFFSET                                     (0x2CU)
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
#define SPI_IMR_TOIM_SHIFT                                 (5U)
#define SPI_IMR_TOIM_MASK                                  (0x1U << SPI_IMR_TOIM_SHIFT)                                 /* 0x00000020 */
#define SPI_IMR_SSPIM_SHIFT                                (6U)
#define SPI_IMR_SSPIM_MASK                                 (0x1U << SPI_IMR_SSPIM_SHIFT)                                /* 0x00000040 */
#define SPI_IMR_TXFIM_SHIFT                                (7U)
#define SPI_IMR_TXFIM_MASK                                 (0x1U << SPI_IMR_TXFIM_SHIFT)                                /* 0x00000080 */
/* ISR */
#define SPI_ISR_OFFSET                                     (0x30U)
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
#define SPI_ISR_TOIS_SHIFT                                 (5U)
#define SPI_ISR_TOIS_MASK                                  (0x1U << SPI_ISR_TOIS_SHIFT)                                 /* 0x00000020 */
#define SPI_ISR_SSPIS_SHIFT                                (6U)
#define SPI_ISR_SSPIS_MASK                                 (0x1U << SPI_ISR_SSPIS_SHIFT)                                /* 0x00000040 */
#define SPI_ISR_TXFIS_SHIFT                                (7U)
#define SPI_ISR_TXFIS_MASK                                 (0x1U << SPI_ISR_TXFIS_SHIFT)                                /* 0x00000080 */
/* RISR */
#define SPI_RISR_OFFSET                                    (0x34U)
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
#define SPI_RISR_TORIS_SHIFT                               (5U)
#define SPI_RISR_TORIS_MASK                                (0x1U << SPI_RISR_TORIS_SHIFT)                               /* 0x00000020 */
#define SPI_RISR_SSPRIS_SHIFT                              (6U)
#define SPI_RISR_SSPRIS_MASK                               (0x1U << SPI_RISR_SSPRIS_SHIFT)                              /* 0x00000040 */
#define SPI_RISR_TXFRIS_SHIFT                              (7U)
#define SPI_RISR_TXFRIS_MASK                               (0x1U << SPI_RISR_TXFRIS_SHIFT)                              /* 0x00000080 */
/* ICR */
#define SPI_ICR_OFFSET                                     (0x38U)
#define SPI_ICR_CCI_SHIFT                                  (0U)
#define SPI_ICR_CCI_MASK                                   (0x1U << SPI_ICR_CCI_SHIFT)                                  /* 0x00000001 */
#define SPI_ICR_CRFUI_SHIFT                                (1U)
#define SPI_ICR_CRFUI_MASK                                 (0x1U << SPI_ICR_CRFUI_SHIFT)                                /* 0x00000002 */
#define SPI_ICR_CRFOI_SHIFT                                (2U)
#define SPI_ICR_CRFOI_MASK                                 (0x1U << SPI_ICR_CRFOI_SHIFT)                                /* 0x00000004 */
#define SPI_ICR_CTFOI_SHIFT                                (3U)
#define SPI_ICR_CTFOI_MASK                                 (0x1U << SPI_ICR_CTFOI_SHIFT)                                /* 0x00000008 */
#define SPI_ICR_CTOI_SHIFT                                 (4U)
#define SPI_ICR_CTOI_MASK                                  (0x1U << SPI_ICR_CTOI_SHIFT)                                 /* 0x00000010 */
#define SPI_ICR_CSSPI_SHIFT                                (5U)
#define SPI_ICR_CSSPI_MASK                                 (0x1U << SPI_ICR_CSSPI_SHIFT)                                /* 0x00000020 */
#define SPI_ICR_CTXFI_SHIFT                                (6U)
#define SPI_ICR_CTXFI_MASK                                 (0x1U << SPI_ICR_CTXFI_SHIFT)                                /* 0x00000040 */
/* DMACR */
#define SPI_DMACR_OFFSET                                   (0x3CU)
#define SPI_DMACR_RDE_SHIFT                                (0U)
#define SPI_DMACR_RDE_MASK                                 (0x1U << SPI_DMACR_RDE_SHIFT)                                /* 0x00000001 */
#define SPI_DMACR_TDE_SHIFT                                (1U)
#define SPI_DMACR_TDE_MASK                                 (0x1U << SPI_DMACR_TDE_SHIFT)                                /* 0x00000002 */
/* DMATDLR */
#define SPI_DMATDLR_OFFSET                                 (0x40U)
#define SPI_DMATDLR_TDL_SHIFT                              (0U)
#define SPI_DMATDLR_TDL_MASK                               (0x3FU << SPI_DMATDLR_TDL_SHIFT)                             /* 0x0000003F */
/* DMARDLR */
#define SPI_DMARDLR_OFFSET                                 (0x44U)
#define SPI_DMARDLR_RDL_SHIFT                              (0U)
#define SPI_DMARDLR_RDL_MASK                               (0x3FU << SPI_DMARDLR_RDL_SHIFT)                             /* 0x0000003F */
/* TIMEOUT */
#define SPI_TIMEOUT_OFFSET                                 (0x4CU)
#define SPI_TIMEOUT_TOV_SHIFT                              (0U)
#define SPI_TIMEOUT_TOV_MASK                               (0xFFFFU << SPI_TIMEOUT_TOV_SHIFT)                           /* 0x0000FFFF */
#define SPI_TIMEOUT_TOE_SHIFT                              (16U)
#define SPI_TIMEOUT_TOE_MASK                               (0x1U << SPI_TIMEOUT_TOE_SHIFT)                              /* 0x00010000 */
/* BYPASS */
#define SPI_BYPASS_OFFSET                                  (0x50U)
#define SPI_BYPASS_BYEN_SHIFT                              (0U)
#define SPI_BYPASS_BYEN_MASK                               (0x1U << SPI_BYPASS_BYEN_SHIFT)                              /* 0x00000001 */
#define SPI_BYPASS_FBM_SHIFT                               (1U)
#define SPI_BYPASS_FBM_MASK                                (0x1U << SPI_BYPASS_FBM_SHIFT)                               /* 0x00000002 */
#define SPI_BYPASS_END_SHIFT                               (2U)
#define SPI_BYPASS_END_MASK                                (0x1U << SPI_BYPASS_END_SHIFT)                               /* 0x00000004 */
#define SPI_BYPASS_RXCP_SHIFT                              (3U)
#define SPI_BYPASS_RXCP_MASK                               (0x1U << SPI_BYPASS_RXCP_SHIFT)                              /* 0x00000008 */
#define SPI_BYPASS_TXCP_SHIFT                              (4U)
#define SPI_BYPASS_TXCP_MASK                               (0x1U << SPI_BYPASS_TXCP_SHIFT)                              /* 0x00000010 */
/* TXDR */
#define SPI_TXDR_OFFSET                                    (0x400U)
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_OFFSET                                    (0x800U)
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/****************************************CSI2HOST****************************************/
/* VERSION */
#define CSI2HOST_VERSION_OFFSET                            (0x0U)
#define CSI2HOST_VERSION                                   (0x0U)
#define CSI2HOST_VERSION_VERSION_SHIFT                     (0U)
#define CSI2HOST_VERSION_VERSION_MASK                      (0x1U << CSI2HOST_VERSION_VERSION_SHIFT)                     /* 0x00000001 */
/* N_LANES */
#define CSI2HOST_N_LANES_OFFSET                            (0x4U)
#define CSI2HOST_N_LANES_N_LANES_SHIFT                     (0U)
#define CSI2HOST_N_LANES_N_LANES_MASK                      (0x3U << CSI2HOST_N_LANES_N_LANES_SHIFT)                     /* 0x00000003 */
/* CSI2_RESETN */
#define CSI2HOST_CSI2_RESETN_OFFSET                        (0x10U)
#define CSI2HOST_CSI2_RESETN_CSI2_RESETN_SHIFT             (0U)
#define CSI2HOST_CSI2_RESETN_CSI2_RESETN_MASK              (0x1U << CSI2HOST_CSI2_RESETN_CSI2_RESETN_SHIFT)             /* 0x00000001 */
/* PHY_STATE */
#define CSI2HOST_PHY_STATE_OFFSET                          (0x14U)
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_0_SHIFT           (0U)
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_0_MASK            (0x1U << CSI2HOST_PHY_STATE_PHY_RXULPSESC_0_SHIFT)           /* 0x00000001 */
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_1_SHIFT           (1U)
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_1_MASK            (0x1U << CSI2HOST_PHY_STATE_PHY_RXULPSESC_1_SHIFT)           /* 0x00000002 */
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_2_SHIFT           (2U)
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_2_MASK            (0x1U << CSI2HOST_PHY_STATE_PHY_RXULPSESC_2_SHIFT)           /* 0x00000004 */
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_3_SHIFT           (3U)
#define CSI2HOST_PHY_STATE_PHY_RXULPSESC_3_MASK            (0x1U << CSI2HOST_PHY_STATE_PHY_RXULPSESC_3_SHIFT)           /* 0x00000008 */
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_0_SHIFT       (4U)
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_0_MASK        (0x1U << CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_0_SHIFT)       /* 0x00000010 */
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_1_SHIFT       (5U)
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_1_MASK        (0x1U << CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_1_SHIFT)       /* 0x00000020 */
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_2_SHIFT       (6U)
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_2_MASK        (0x1U << CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_2_SHIFT)       /* 0x00000040 */
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_3_SHIFT       (7U)
#define CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_3_MASK        (0x1U << CSI2HOST_PHY_STATE_PHY_STOPSTATEDATA_3_SHIFT)       /* 0x00000080 */
#define CSI2HOST_PHY_STATE_PHY_RXCLKACTIVEHS_SHIFT         (8U)
#define CSI2HOST_PHY_STATE_PHY_RXCLKACTIVEHS_MASK          (0x1U << CSI2HOST_PHY_STATE_PHY_RXCLKACTIVEHS_SHIFT)         /* 0x00000100 */
#define CSI2HOST_PHY_STATE_PHY_RXULPSCLKNOT_SHIFT          (9U)
#define CSI2HOST_PHY_STATE_PHY_RXULPSCLKNOT_MASK           (0x1U << CSI2HOST_PHY_STATE_PHY_RXULPSCLKNOT_SHIFT)          /* 0x00000200 */
#define CSI2HOST_PHY_STATE_PHY_STOPSTATECLK_SHIFT          (10U)
#define CSI2HOST_PHY_STATE_PHY_STOPSTATECLK_MASK           (0x1U << CSI2HOST_PHY_STATE_PHY_STOPSTATECLK_SHIFT)          /* 0x00000400 */
#define CSI2HOST_PHY_STATE_BYPASS_2ECC_TST_SHIFT           (11U)
#define CSI2HOST_PHY_STATE_BYPASS_2ECC_TST_MASK            (0x1U << CSI2HOST_PHY_STATE_BYPASS_2ECC_TST_SHIFT)           /* 0x00000800 */
/* DATA_IDS_1 */
#define CSI2HOST_DATA_IDS_1_OFFSET                         (0x18U)
#define CSI2HOST_DATA_IDS_1_DI0_DT_SHIFT                   (0U)
#define CSI2HOST_DATA_IDS_1_DI0_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_1_DI0_DT_SHIFT)                  /* 0x0000003F */
#define CSI2HOST_DATA_IDS_1_DI0_VC_SHIFT                   (6U)
#define CSI2HOST_DATA_IDS_1_DI0_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_1_DI0_VC_SHIFT)                   /* 0x000000C0 */
#define CSI2HOST_DATA_IDS_1_DI1_DT_SHIFT                   (8U)
#define CSI2HOST_DATA_IDS_1_DI1_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_1_DI1_DT_SHIFT)                  /* 0x00003F00 */
#define CSI2HOST_DATA_IDS_1_DI1_VC_SHIFT                   (14U)
#define CSI2HOST_DATA_IDS_1_DI1_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_1_DI1_VC_SHIFT)                   /* 0x0000C000 */
#define CSI2HOST_DATA_IDS_1_DI2_DT_SHIFT                   (16U)
#define CSI2HOST_DATA_IDS_1_DI2_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_1_DI2_DT_SHIFT)                  /* 0x003F0000 */
#define CSI2HOST_DATA_IDS_1_DI2_VC_SHIFT                   (22U)
#define CSI2HOST_DATA_IDS_1_DI2_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_1_DI2_VC_SHIFT)                   /* 0x00C00000 */
#define CSI2HOST_DATA_IDS_1_DI3_DT_SHIFT                   (24U)
#define CSI2HOST_DATA_IDS_1_DI3_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_1_DI3_DT_SHIFT)                  /* 0x3F000000 */
#define CSI2HOST_DATA_IDS_1_DI3_VC_SHIFT                   (30U)
#define CSI2HOST_DATA_IDS_1_DI3_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_1_DI3_VC_SHIFT)                   /* 0xC0000000 */
/* DATA_IDS_2 */
#define CSI2HOST_DATA_IDS_2_OFFSET                         (0x1CU)
#define CSI2HOST_DATA_IDS_2_DI4_DT_SHIFT                   (0U)
#define CSI2HOST_DATA_IDS_2_DI4_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_2_DI4_DT_SHIFT)                  /* 0x0000003F */
#define CSI2HOST_DATA_IDS_2_DI4_VC_SHIFT                   (6U)
#define CSI2HOST_DATA_IDS_2_DI4_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_2_DI4_VC_SHIFT)                   /* 0x000000C0 */
#define CSI2HOST_DATA_IDS_2_DI5_DT_SHIFT                   (8U)
#define CSI2HOST_DATA_IDS_2_DI5_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_2_DI5_DT_SHIFT)                  /* 0x00003F00 */
#define CSI2HOST_DATA_IDS_2_DI5_VC_SHIFT                   (14U)
#define CSI2HOST_DATA_IDS_2_DI5_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_2_DI5_VC_SHIFT)                   /* 0x0000C000 */
#define CSI2HOST_DATA_IDS_2_DI6_DT_SHIFT                   (16U)
#define CSI2HOST_DATA_IDS_2_DI6_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_2_DI6_DT_SHIFT)                  /* 0x003F0000 */
#define CSI2HOST_DATA_IDS_2_DI6_VC_SHIFT                   (22U)
#define CSI2HOST_DATA_IDS_2_DI6_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_2_DI6_VC_SHIFT)                   /* 0x00C00000 */
#define CSI2HOST_DATA_IDS_2_DI7_DT_SHIFT                   (24U)
#define CSI2HOST_DATA_IDS_2_DI7_DT_MASK                    (0x3FU << CSI2HOST_DATA_IDS_2_DI7_DT_SHIFT)                  /* 0x3F000000 */
#define CSI2HOST_DATA_IDS_2_DI7_VC_SHIFT                   (30U)
#define CSI2HOST_DATA_IDS_2_DI7_VC_MASK                    (0x3U << CSI2HOST_DATA_IDS_2_DI7_VC_SHIFT)                   /* 0xC0000000 */
/* ERR1 */
#define CSI2HOST_ERR1_OFFSET                               (0x20U)
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_0_SHIFT             (0U)
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_0_MASK              (0x1U << CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_0_SHIFT)             /* 0x00000001 */
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_1_SHIFT             (1U)
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_1_MASK              (0x1U << CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_1_SHIFT)             /* 0x00000002 */
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_2_SHIFT             (2U)
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_2_MASK              (0x1U << CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_2_SHIFT)             /* 0x00000004 */
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_3_SHIFT             (3U)
#define CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_3_MASK              (0x1U << CSI2HOST_ERR1_PHY_ERRSOTSYNCHS_3_SHIFT)             /* 0x00000008 */
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC0_SHIFT          (4U)
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC0_MASK           (0x1U << CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC0_SHIFT)          /* 0x00000010 */
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC1_SHIFT          (5U)
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC1_MASK           (0x1U << CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC1_SHIFT)          /* 0x00000020 */
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC2_SHIFT          (6U)
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC2_MASK           (0x1U << CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC2_SHIFT)          /* 0x00000040 */
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC3_SHIFT          (7U)
#define CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC3_MASK           (0x1U << CSI2HOST_ERR1_ERR_F_BNDRY_MATCH_VC3_SHIFT)          /* 0x00000080 */
#define CSI2HOST_ERR1_ERR_F_SEQ_VC0_SHIFT                  (8U)
#define CSI2HOST_ERR1_ERR_F_SEQ_VC0_MASK                   (0x1U << CSI2HOST_ERR1_ERR_F_SEQ_VC0_SHIFT)                  /* 0x00000100 */
#define CSI2HOST_ERR1_ERR_F_SEQ_VC1_SHIFT                  (9U)
#define CSI2HOST_ERR1_ERR_F_SEQ_VC1_MASK                   (0x1U << CSI2HOST_ERR1_ERR_F_SEQ_VC1_SHIFT)                  /* 0x00000200 */
#define CSI2HOST_ERR1_ERR_F_SEQ_VC2_SHIFT                  (10U)
#define CSI2HOST_ERR1_ERR_F_SEQ_VC2_MASK                   (0x1U << CSI2HOST_ERR1_ERR_F_SEQ_VC2_SHIFT)                  /* 0x00000400 */
#define CSI2HOST_ERR1_ERR_F_SEQ_VC3_SHIFT                  (11U)
#define CSI2HOST_ERR1_ERR_F_SEQ_VC3_MASK                   (0x1U << CSI2HOST_ERR1_ERR_F_SEQ_VC3_SHIFT)                  /* 0x00000800 */
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC0_SHIFT             (12U)
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC0_MASK              (0x1U << CSI2HOST_ERR1_ERR_FRAME_DATA_VC0_SHIFT)             /* 0x00001000 */
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC1_SHIFT             (13U)
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC1_MASK              (0x1U << CSI2HOST_ERR1_ERR_FRAME_DATA_VC1_SHIFT)             /* 0x00002000 */
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC2_SHIFT             (14U)
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC2_MASK              (0x1U << CSI2HOST_ERR1_ERR_FRAME_DATA_VC2_SHIFT)             /* 0x00004000 */
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC3_SHIFT             (15U)
#define CSI2HOST_ERR1_ERR_FRAME_DATA_VC3_MASK              (0x1U << CSI2HOST_ERR1_ERR_FRAME_DATA_VC3_SHIFT)             /* 0x00008000 */
#define CSI2HOST_ERR1_VC0_ERR_CRC_SHIFT                    (24U)
#define CSI2HOST_ERR1_VC0_ERR_CRC_MASK                     (0x1U << CSI2HOST_ERR1_VC0_ERR_CRC_SHIFT)                    /* 0x01000000 */
#define CSI2HOST_ERR1_VC1_ERR_CRC_SHIFT                    (25U)
#define CSI2HOST_ERR1_VC1_ERR_CRC_MASK                     (0x1U << CSI2HOST_ERR1_VC1_ERR_CRC_SHIFT)                    /* 0x02000000 */
#define CSI2HOST_ERR1_VC2_ERR_CRC_SHIFT                    (26U)
#define CSI2HOST_ERR1_VC2_ERR_CRC_MASK                     (0x1U << CSI2HOST_ERR1_VC2_ERR_CRC_SHIFT)                    /* 0x04000000 */
#define CSI2HOST_ERR1_VC3_ERR_CRC_SHIFT                    (27U)
#define CSI2HOST_ERR1_VC3_ERR_CRC_MASK                     (0x1U << CSI2HOST_ERR1_VC3_ERR_CRC_SHIFT)                    /* 0x08000000 */
#define CSI2HOST_ERR1_ERR_ECC_DOUBLE_SHIFT                 (28U)
#define CSI2HOST_ERR1_ERR_ECC_DOUBLE_MASK                  (0x1U << CSI2HOST_ERR1_ERR_ECC_DOUBLE_SHIFT)                 /* 0x10000000 */
#define CSI2HOST_ERR1_ERR_PH_CRC_LANE0_SHIFT               (29U)
#define CSI2HOST_ERR1_ERR_PH_CRC_LANE0_MASK                (0x1U << CSI2HOST_ERR1_ERR_PH_CRC_LANE0_SHIFT)               /* 0x20000000 */
#define CSI2HOST_ERR1_ERR_PH_CRC_LANE1_SHIFT               (30U)
#define CSI2HOST_ERR1_ERR_PH_CRC_LANE1_MASK                (0x1U << CSI2HOST_ERR1_ERR_PH_CRC_LANE1_SHIFT)               /* 0x40000000 */
#define CSI2HOST_ERR1_ERR_PH_CRC_LANE2_SHIFT               (31U)
#define CSI2HOST_ERR1_ERR_PH_CRC_LANE2_MASK                (0x1U << CSI2HOST_ERR1_ERR_PH_CRC_LANE2_SHIFT)               /* 0x80000000 */
/* ERR2 */
#define CSI2HOST_ERR2_OFFSET                               (0x24U)
#define CSI2HOST_ERR2_PHY_ERRESC_0_SHIFT                   (0U)
#define CSI2HOST_ERR2_PHY_ERRESC_0_MASK                    (0x1U << CSI2HOST_ERR2_PHY_ERRESC_0_SHIFT)                   /* 0x00000001 */
#define CSI2HOST_ERR2_PHY_ERRESC_1_SHIFT                   (1U)
#define CSI2HOST_ERR2_PHY_ERRESC_1_MASK                    (0x1U << CSI2HOST_ERR2_PHY_ERRESC_1_SHIFT)                   /* 0x00000002 */
#define CSI2HOST_ERR2_PHY_ERRESC_2_SHIFT                   (2U)
#define CSI2HOST_ERR2_PHY_ERRESC_2_MASK                    (0x1U << CSI2HOST_ERR2_PHY_ERRESC_2_SHIFT)                   /* 0x00000004 */
#define CSI2HOST_ERR2_PHY_ERRESC_3_SHIFT                   (3U)
#define CSI2HOST_ERR2_PHY_ERRESC_3_MASK                    (0x1U << CSI2HOST_ERR2_PHY_ERRESC_3_SHIFT)                   /* 0x00000008 */
#define CSI2HOST_ERR2_PHY_ERRSOTHS_0_SHIFT                 (4U)
#define CSI2HOST_ERR2_PHY_ERRSOTHS_0_MASK                  (0x1U << CSI2HOST_ERR2_PHY_ERRSOTHS_0_SHIFT)                 /* 0x00000010 */
#define CSI2HOST_ERR2_PHY_ERRSOTHS_1_SHIFT                 (5U)
#define CSI2HOST_ERR2_PHY_ERRSOTHS_1_MASK                  (0x1U << CSI2HOST_ERR2_PHY_ERRSOTHS_1_SHIFT)                 /* 0x00000020 */
#define CSI2HOST_ERR2_PHY_ERRSOTHS_2_SHIFT                 (6U)
#define CSI2HOST_ERR2_PHY_ERRSOTHS_2_MASK                  (0x1U << CSI2HOST_ERR2_PHY_ERRSOTHS_2_SHIFT)                 /* 0x00000040 */
#define CSI2HOST_ERR2_PHY_ERRSOTHS_3_SHIFT                 (7U)
#define CSI2HOST_ERR2_PHY_ERRSOTHS_3_MASK                  (0x1U << CSI2HOST_ERR2_PHY_ERRSOTHS_3_SHIFT)                 /* 0x00000080 */
#define CSI2HOST_ERR2_VC0_ERR_ECC_CORRECTED_SHIFT          (8U)
#define CSI2HOST_ERR2_VC0_ERR_ECC_CORRECTED_MASK           (0x1U << CSI2HOST_ERR2_VC0_ERR_ECC_CORRECTED_SHIFT)          /* 0x00000100 */
#define CSI2HOST_ERR2_VC1_ERR_ECC_CORRECTED_SHIFT          (9U)
#define CSI2HOST_ERR2_VC1_ERR_ECC_CORRECTED_MASK           (0x1U << CSI2HOST_ERR2_VC1_ERR_ECC_CORRECTED_SHIFT)          /* 0x00000200 */
#define CSI2HOST_ERR2_VC2_ERR_ECC_CORRECTED_SHIFT          (10U)
#define CSI2HOST_ERR2_VC2_ERR_ECC_CORRECTED_MASK           (0x1U << CSI2HOST_ERR2_VC2_ERR_ECC_CORRECTED_SHIFT)          /* 0x00000400 */
#define CSI2HOST_ERR2_VC3_ERR_ECC_CORRECTED_SHIFT          (11U)
#define CSI2HOST_ERR2_VC3_ERR_ECC_CORRECTED_MASK           (0x1U << CSI2HOST_ERR2_VC3_ERR_ECC_CORRECTED_SHIFT)          /* 0x00000800 */
#define CSI2HOST_ERR2_ERR_ID_VC0_SHIFT                     (12U)
#define CSI2HOST_ERR2_ERR_ID_VC0_MASK                      (0x1U << CSI2HOST_ERR2_ERR_ID_VC0_SHIFT)                     /* 0x00001000 */
#define CSI2HOST_ERR2_ERR_ID_VC1_SHIFT                     (13U)
#define CSI2HOST_ERR2_ERR_ID_VC1_MASK                      (0x1U << CSI2HOST_ERR2_ERR_ID_VC1_SHIFT)                     /* 0x00002000 */
#define CSI2HOST_ERR2_ERR_ID_VC2_SHIFT                     (14U)
#define CSI2HOST_ERR2_ERR_ID_VC2_MASK                      (0x1U << CSI2HOST_ERR2_ERR_ID_VC2_SHIFT)                     /* 0x00004000 */
#define CSI2HOST_ERR2_ERR_ID_VC3_SHIFT                     (15U)
#define CSI2HOST_ERR2_ERR_ID_VC3_MASK                      (0x1U << CSI2HOST_ERR2_ERR_ID_VC3_SHIFT)                     /* 0x00008000 */
#define CSI2HOST_ERR2_PHY_ERRCODEHS_0_SHIFT                (24U)
#define CSI2HOST_ERR2_PHY_ERRCODEHS_0_MASK                 (0x1U << CSI2HOST_ERR2_PHY_ERRCODEHS_0_SHIFT)                /* 0x01000000 */
#define CSI2HOST_ERR2_PHY_ERRCODEHS_1_SHIFT                (25U)
#define CSI2HOST_ERR2_PHY_ERRCODEHS_1_MASK                 (0x1U << CSI2HOST_ERR2_PHY_ERRCODEHS_1_SHIFT)                /* 0x02000000 */
#define CSI2HOST_ERR2_PHY_ERRCODEHS_2_SHIFT                (26U)
#define CSI2HOST_ERR2_PHY_ERRCODEHS_2_MASK                 (0x1U << CSI2HOST_ERR2_PHY_ERRCODEHS_2_SHIFT)                /* 0x04000000 */
#define CSI2HOST_ERR2_PHY_ERRCODEHS_3_SHIFT                (27U)
#define CSI2HOST_ERR2_PHY_ERRCODEHS_3_MASK                 (0x1U << CSI2HOST_ERR2_PHY_ERRCODEHS_3_SHIFT)                /* 0x08000000 */
/* MSK1 */
#define CSI2HOST_MSK1_OFFSET                               (0x28U)
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_0_SHIFT        (0U)
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_0_MASK         (0x1U << CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_0_SHIFT)        /* 0x00000001 */
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_1_SHIFT        (1U)
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_1_MASK         (0x1U << CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_1_SHIFT)        /* 0x00000002 */
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_2_SHIFT        (2U)
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_2_MASK         (0x1U << CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_2_SHIFT)        /* 0x00000004 */
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_3_SHIFT        (3U)
#define CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_3_MASK         (0x1U << CSI2HOST_MSK1_MASK_PHY_ERRSOTSYNCHS_3_SHIFT)        /* 0x00000008 */
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC0_SHIFT     (4U)
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC0_MASK      (0x1U << CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC0_SHIFT)     /* 0x00000010 */
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC1_SHIFT     (5U)
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC1_MASK      (0x1U << CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC1_SHIFT)     /* 0x00000020 */
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC2_SHIFT     (6U)
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC2_MASK      (0x1U << CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC2_SHIFT)     /* 0x00000040 */
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC3_SHIFT     (7U)
#define CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC3_MASK      (0x1U << CSI2HOST_MSK1_MASK_ERR_F_BNDRY_MATCH_VC3_SHIFT)     /* 0x00000080 */
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC0_SHIFT             (8U)
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC0_MASK              (0x1U << CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC0_SHIFT)             /* 0x00000100 */
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC1_SHIFT             (9U)
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC1_MASK              (0x1U << CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC1_SHIFT)             /* 0x00000200 */
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC2_SHIFT             (10U)
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC2_MASK              (0x1U << CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC2_SHIFT)             /* 0x00000400 */
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC3_SHIFT             (11U)
#define CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC3_MASK              (0x1U << CSI2HOST_MSK1_MASK_ERR_F_SEQ_VC3_SHIFT)             /* 0x00000800 */
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC0_SHIFT        (12U)
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC0_MASK         (0x1U << CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC0_SHIFT)        /* 0x00001000 */
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC1_SHIFT        (13U)
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC1_MASK         (0x1U << CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC1_SHIFT)        /* 0x00002000 */
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC2_SHIFT        (14U)
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC2_MASK         (0x1U << CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC2_SHIFT)        /* 0x00004000 */
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC3_SHIFT        (15U)
#define CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC3_MASK         (0x1U << CSI2HOST_MSK1_MASK_ERR_FRAME_DATA_VC3_SHIFT)        /* 0x00008000 */
#define CSI2HOST_MSK1_MASK_VC0_ERR_CRC_SHIFT               (24U)
#define CSI2HOST_MSK1_MASK_VC0_ERR_CRC_MASK                (0x1U << CSI2HOST_MSK1_MASK_VC0_ERR_CRC_SHIFT)               /* 0x01000000 */
#define CSI2HOST_MSK1_MASK_VC1_ERR_CRC_SHIFT               (25U)
#define CSI2HOST_MSK1_MASK_VC1_ERR_CRC_MASK                (0x1U << CSI2HOST_MSK1_MASK_VC1_ERR_CRC_SHIFT)               /* 0x02000000 */
#define CSI2HOST_MSK1_MASK_VC2_ERR_CRC_SHIFT               (26U)
#define CSI2HOST_MSK1_MASK_VC2_ERR_CRC_MASK                (0x1U << CSI2HOST_MSK1_MASK_VC2_ERR_CRC_SHIFT)               /* 0x04000000 */
#define CSI2HOST_MSK1_MASK_VC3_ERR_CRC_SHIFT               (27U)
#define CSI2HOST_MSK1_MASK_VC3_ERR_CRC_MASK                (0x1U << CSI2HOST_MSK1_MASK_VC3_ERR_CRC_SHIFT)               /* 0x08000000 */
#define CSI2HOST_MSK1_MASK_ERR_ECC_DOUBLE_SHIFT            (28U)
#define CSI2HOST_MSK1_MASK_ERR_ECC_DOUBLE_MASK             (0x1U << CSI2HOST_MSK1_MASK_ERR_ECC_DOUBLE_SHIFT)            /* 0x10000000 */
#define CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE0_SHIFT          (29U)
#define CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE0_MASK           (0x1U << CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE0_SHIFT)          /* 0x20000000 */
#define CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE1_SHIFT          (30U)
#define CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE1_MASK           (0x1U << CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE1_SHIFT)          /* 0x40000000 */
#define CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE2_SHIFT          (31U)
#define CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE2_MASK           (0x1U << CSI2HOST_MSK1_MASK_ERR_PH_CRC_LANE2_SHIFT)          /* 0x80000000 */
/* MSK2 */
#define CSI2HOST_MSK2_OFFSET                               (0x2CU)
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_0_SHIFT              (0U)
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_0_MASK               (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRESC_0_SHIFT)              /* 0x00000001 */
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_1_SHIFT              (1U)
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_1_MASK               (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRESC_1_SHIFT)              /* 0x00000002 */
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_2_SHIFT              (2U)
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_2_MASK               (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRESC_2_SHIFT)              /* 0x00000004 */
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_3_SHIFT              (3U)
#define CSI2HOST_MSK2_MASK_PHY_ERRESC_3_MASK               (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRESC_3_SHIFT)              /* 0x00000008 */
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_0_SHIFT            (4U)
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_0_MASK             (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_0_SHIFT)            /* 0x00000010 */
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_1_SHIFT            (5U)
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_1_MASK             (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_1_SHIFT)            /* 0x00000020 */
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_2_SHIFT            (6U)
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_2_MASK             (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_2_SHIFT)            /* 0x00000040 */
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_3_SHIFT            (7U)
#define CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_3_MASK             (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRSOTHS_3_SHIFT)            /* 0x00000080 */
#define CSI2HOST_MSK2_MASK_VC0_ERR_ECC_CORRECTED_SHIFT     (8U)
#define CSI2HOST_MSK2_MASK_VC0_ERR_ECC_CORRECTED_MASK      (0x1U << CSI2HOST_MSK2_MASK_VC0_ERR_ECC_CORRECTED_SHIFT)     /* 0x00000100 */
#define CSI2HOST_MSK2_MASK_VC1_ERR_ECC_CORRECTED_SHIFT     (9U)
#define CSI2HOST_MSK2_MASK_VC1_ERR_ECC_CORRECTED_MASK      (0x1U << CSI2HOST_MSK2_MASK_VC1_ERR_ECC_CORRECTED_SHIFT)     /* 0x00000200 */
#define CSI2HOST_MSK2_MASK_VC2_ERR_ECC_CORRECTED_SHIFT     (10U)
#define CSI2HOST_MSK2_MASK_VC2_ERR_ECC_CORRECTED_MASK      (0x1U << CSI2HOST_MSK2_MASK_VC2_ERR_ECC_CORRECTED_SHIFT)     /* 0x00000400 */
#define CSI2HOST_MSK2_MASK_VC3_ERR_ECC_CORRECTED_SHIFT     (11U)
#define CSI2HOST_MSK2_MASK_VC3_ERR_ECC_CORRECTED_MASK      (0x1U << CSI2HOST_MSK2_MASK_VC3_ERR_ECC_CORRECTED_SHIFT)     /* 0x00000800 */
#define CSI2HOST_MSK2_MASK_ERR_ID_VC0_SHIFT                (12U)
#define CSI2HOST_MSK2_MASK_ERR_ID_VC0_MASK                 (0x1U << CSI2HOST_MSK2_MASK_ERR_ID_VC0_SHIFT)                /* 0x00001000 */
#define CSI2HOST_MSK2_MASK_ERR_ID_VC1_SHIFT                (13U)
#define CSI2HOST_MSK2_MASK_ERR_ID_VC1_MASK                 (0x1U << CSI2HOST_MSK2_MASK_ERR_ID_VC1_SHIFT)                /* 0x00002000 */
#define CSI2HOST_MSK2_MASK_ERR_ID_VC2_SHIFT                (14U)
#define CSI2HOST_MSK2_MASK_ERR_ID_VC2_MASK                 (0x1U << CSI2HOST_MSK2_MASK_ERR_ID_VC2_SHIFT)                /* 0x00004000 */
#define CSI2HOST_MSK2_MASK_ERR_ID_VC3_SHIFT                (15U)
#define CSI2HOST_MSK2_MASK_ERR_ID_VC3_MASK                 (0x1U << CSI2HOST_MSK2_MASK_ERR_ID_VC3_SHIFT)                /* 0x00008000 */
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_0_SHIFT           (24U)
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_0_MASK            (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_0_SHIFT)           /* 0x01000000 */
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_1_SHIFT           (25U)
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_1_MASK            (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_1_SHIFT)           /* 0x02000000 */
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_2_SHIFT           (26U)
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_2_MASK            (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_2_SHIFT)           /* 0x04000000 */
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_3_SHIFT           (27U)
#define CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_3_MASK            (0x1U << CSI2HOST_MSK2_MASK_PHY_ERRCODEHS_3_SHIFT)           /* 0x08000000 */
/* CONTROL */
#define CSI2HOST_CONTROL_OFFSET                            (0x40U)
#define CSI2HOST_CONTROL_SW_CPHY_EN_SHIFT                  (0U)
#define CSI2HOST_CONTROL_SW_CPHY_EN_MASK                   (0x1U << CSI2HOST_CONTROL_SW_CPHY_EN_SHIFT)                  /* 0x00000001 */
#define CSI2HOST_CONTROL_SW_DSI_EN_SHIFT                   (4U)
#define CSI2HOST_CONTROL_SW_DSI_EN_MASK                    (0x1U << CSI2HOST_CONTROL_SW_DSI_EN_SHIFT)                   /* 0x00000010 */
#define CSI2HOST_CONTROL_SW_DATATYPE_FS_SHIFT              (8U)
#define CSI2HOST_CONTROL_SW_DATATYPE_FS_MASK               (0x3FU << CSI2HOST_CONTROL_SW_DATATYPE_FS_SHIFT)             /* 0x00003F00 */
#define CSI2HOST_CONTROL_SW_DATATYPE_FE_SHIFT              (14U)
#define CSI2HOST_CONTROL_SW_DATATYPE_FE_MASK               (0x3FU << CSI2HOST_CONTROL_SW_DATATYPE_FE_SHIFT)             /* 0x000FC000 */
#define CSI2HOST_CONTROL_SW_DATATYPE_LS_SHIFT              (20U)
#define CSI2HOST_CONTROL_SW_DATATYPE_LS_MASK               (0x3FU << CSI2HOST_CONTROL_SW_DATATYPE_LS_SHIFT)             /* 0x03F00000 */
#define CSI2HOST_CONTROL_SW_DATATYPE_LE_SHIFT              (26U)
#define CSI2HOST_CONTROL_SW_DATATYPE_LE_MASK               (0x3FU << CSI2HOST_CONTROL_SW_DATATYPE_LE_SHIFT)             /* 0xFC000000 */

/********Name=SOFTRST_CON00,Offset=0x400********/
#define SRST_H_MCU_BUS_AC   1U
#define SRST_MCU_AC         2U
#define SRST_P_LOGIC_BUS_AC 3U
/********Name=SOFTRST_CON01,Offset=0x404********/
#define SRST_H_MCU_BUS_NIU 17U
#define SRST_MCU           18U
#define SRST_H_INTMEM0     19U
#define SRST_H_INTMEM1     20U
#define SRST_H_INTMEM2     21U
#define SRST_H_ROM         22U
#define SRST_H_SFC         23U
#define SRST_H_SFC_XIP     24U
#define SRST_H_USB         25U
#define SRST_A_VIPCAP      28U
#define SRST_H_VIPCAP      29U
#define SRST_H_ISP         31U
/********Name=SOFTRST_CON02,Offset=0x408********/
#define SRST_H_JPEG_ENC0     32U
#define SRST_H_JPEG_ENC1     33U
#define SRST_H_LOGIC_BUS_NIU 34U
#define SRST_H_DMAC          35U
#define SRST_H_JHASH_M       36U
#define SRST_H_JHASH_S       37U
#define SRST_H_HOST_ARB      38U
#define SRST_A_JPEG_BUS_NIU  41U
#define SRST_A_JPEG_ENC0     42U
#define SRST_A_JPEG_ENC1     43U
/********Name=SOFTRST_CON03,Offset=0x40C********/
#define SRST_P_LOGIC_BUS_NIU  49U
#define SRST_P_UART           50U
#define SRST_P_I2C0           51U
#define SRST_P_I2C1           52U
#define SRST_P_PWM            53U
#define SRST_P_SPI0           54U
#define SRST_P_SPI1           55U
#define SRST_P_SARADC_CONTROL 56U
#define SRST_P_TIMER          57U
#define SRST_P_WDT            58U
#define SRST_P_CSI2HOST       59U
#define SRST_P_BUFFER_MANAGER 60U
#define SRST_P_INTCTRL        61U
/********Name=SOFTRST_CON04,Offset=0x410********/
#define SRST_P_GPIO0  68U
#define SRST_P_GPIO1  69U
#define SRST_P_GRF    72U
#define SRST_P_USBGRF 73U
#define SRST_DB_GPIO0 74U
#define SRST_DB_GPIO1 75U
#define SRST_USBPHY   77U
/********Name=SOFTRST_CON05,Offset=0x414********/
#define SRST_I2C0      80U
#define SRST_I2C1      81U
#define SRST_SPI0      82U
#define SRST_SPI1      83U
#define SRST_PWM       84U
#define SRST_TIMER0    86U
#define SRST_TIMER1    87U
#define SRST_TIMER2    88U
#define SRST_TIMER3    89U
#define SRST_TIMER4    90U
#define SRST_TIMER5    91U
#define SRST_MCU_JTRST 92U
#define SRST_XIP_SFC   93U
/********Name=SOFTRST_CON06,Offset=0x418********/
#define SRST_ISP      97U
#define SRST_D_VIPCAP 98U
#define SRST_I_VIPCAP 99U
#define SRST_UTMI     101U
#define SRST_USB_ADP  102U
#define SRST_S_UART   106U
/********Name=SOFTRST_CON07,Offset=0x41C********/
#define SRST_WBUFFER_CH0_MANAGE0 112U
#define SRST_WBUFFER_CH0_MANAGE1 113U
#define SRST_WBUFFER_CH1_MANAGE0 114U
#define SRST_WBUFFER_CH2_MANAGE0 115U
#define SRST_WBUFFER_CH2_MANAGE1 116U
#define SRST_WBUFFER_CH3_MANAGE0 117U
#define SRST_RBUFFER_CH0_MANAGE0 118U
#define SRST_RBUFFER_CH0_MANAGE1 119U
#define SRST_RBUFFER_CH1_MANAGE0 120U
#define SRST_RBUFFER_CH1_MANAGE1 121U

/********Name=GATE_CON00,Offset=0x300********/
#define CLK_32K_GATE              1U
#define CLK_GPLL_MUX_DIV_NP5_GATE 4U
#define HCLK_MCU_BUS_PLL_GATE     5U
/********Name=GATE_CON01,Offset=0x304********/
#define HCLK_LOGIC_BUS_GATE     16U
#define HCLK_MCU_BUS_NIU_GATE   17U
#define CLK_MCU_GATE            18U
#define HCLK_INTMEM0_LOGIC_GATE 19U
#define HCLK_INTMEM1_LOGIC_GATE 20U
#define HCLK_INTMEM2_LOGIC_GATE 21U
#define HCLK_ROM_GATE           22U
#define HCLK_SFC_GATE           23U
#define HCLK_SFC_XIP_GATE       24U
#define HCLK_USB_GATE           25U
#define HCLK_USB_PMU_GATE       26U
#define ACLK_VIPCAP_GATE        28U
#define HCLK_VIPCAP_GATE        29U
#define ACLK_ISP_GATE           30U
#define HCLK_ISP_GATE           31U
/********Name=GATE_CON02,Offset=0x308********/
#define HCLK_JPEG_ENC0_GATE     32U
#define HCLK_JPEG_ENC1_GATE     33U
#define HCLK_LOGIC_BUS_NIU_GATE 34U
#define HCLK_DMAC_GATE          35U
#define HCLK_JHASH_M_GATE       36U
#define HCLK_JHASH_S_GATE       37U
#define HCLK_HOST_ARB_GATE      38U
#define ACLK_JPEG_BUS_PLL_GATE  40U
#define ACLK_JPEG_BUS_NIU_GATE  41U
#define ACLK_JPEG_ENC0_GATE     42U
#define ACLK_JPEG_ENC1_GATE     43U
/********Name=GATE_CON03,Offset=0x30C********/
#define PCLK_LOGIC_BUS_PLL_GATE  48U
#define PCLK_LOGIC_BUS_NIU_GATE  49U
#define PCLK_UART_GATE           50U
#define PCLK_I2C0_GATE           51U
#define PCLK_I2C1_GATE           52U
#define PCLK_PWM_GATE            53U
#define PCLK_SPI0_GATE           54U
#define PCLK_SPI1_GATE           55U
#define PCLK_SARADC_CONTROL_GATE 56U
#define PCLK_TIMER_GATE          57U
#define PCLK_WDT_GATE            58U
#define PCLK_CSI2HOST_GATE       59U
#define PCLK_BUFFER_MANAGER_GATE 60U
#define PCLK_INTCTRL_GATE        61U
/********Name=GATE_CON04,Offset=0x310********/
#define PCLK_GPIO0_GATE       68U
#define PCLK_GPIO1_GATE       69U
#define PCLK_CRU_GATE         70U
#define PCLK_GRF_GATE         72U
#define PCLK_USBGRF_GATE      73U
#define DBCLK_GPIO0_GATE      74U
#define DBCLK_GPIO1_GATE      75U
#define CLK_SARADC_GATE       76U
#define CLK_MIPIPHY0_CFG_GATE 78U
#define CLK_MIPIPHY1_CFG_GATE 79U
/********Name=GATE_CON05,Offset=0x314********/
#define CLK_I2C0_PLL_GATE        80U
#define CLK_I2C1_PLL_GATE        81U
#define CLK_SPI0_PLL_GATE        82U
#define CLK_SPI1_PLL_GATE        83U
#define CLK_PWM_PLL_GATE         84U
#define CLK_TIMER_PLL_GATE       85U
#define CLK_TIMER0_GATE          86U
#define CLK_TIMER1_GATE          87U
#define CLK_TIMER2_GATE          88U
#define CLK_TIMER3_GATE          89U
#define CLK_TIMER4_GATE          90U
#define CLK_TIMER5_GATE          91U
#define CLK_XIP_SFC_DT50_GATE    93U
#define CLK_PWM_CAPTURE_PLL_GATE 94U
#define RTCCLK_MCU_GATE          95U
/********Name=GATE_CON06,Offset=0x318********/
#define CLK_WDT_PLL_GATE     96U
#define CLK_ISP_PLL_GATE     97U
#define DCLK_VIPCAP_PLL_GATE 98U
#define CLK_VIP_OUT_PLL_GATE 100U
#define CLK_USB_ADP_GATE     102U
#define OUTCLOCK_TEST_GATE   103U
#define CLK_UART_PLL_GATE    104U
#define CLK_UART_FRAC_GATE   105U
#define SCLK_UART_GATE       106U

/********Name=CLKSEL_CON00,Offset=0x100********/
#define HCLK_MCU_BUS_DIV     0x07000000U
#define CLK_GPLL_MUX_NP5_DIV 0x04080000U
/********Name=CLKSEL_CON01,Offset=0x104********/
#define CLK_32K_FRAC_DIV_DIV 0x20000001U
/********Name=CLKSEL_CON02,Offset=0x108********/
#define HCLK_LOGIC_BUS_DIV 0x04000002U
#define ACLK_JPEG_BUS_DIV  0x05080002U
/********Name=CLKSEL_CON03,Offset=0x10C********/
#define PCLK_LOGIC_BUS_DIV 0x05000003U
#define CLK_SARADC_DIV     0x08080003U
/********Name=CLKSEL_CON04,Offset=0x110********/
#define CLK_I2C0_DIV 0x06000004U
#define CLK_I2C1_DIV 0x06080004U
/********Name=CLKSEL_CON05,Offset=0x114********/
#define CLK_SPI0_DIV 0x06000005U
#define CLK_SPI1_DIV 0x06080005U
/********Name=CLKSEL_CON06,Offset=0x118********/
#define CLK_PWM_DIV   0x06000006U
#define CLK_TIMER_DIV 0x06080006U
/********Name=CLKSEL_CON07,Offset=0x11C********/
#define CLK_XIP_SFC_DIV 0x06000007U
#define CLK_ISP_DIV     0x06080007U
/********Name=CLKSEL_CON08,Offset=0x120********/
#define DCLK_VIPCAP_DIV 0x06000008U
#define CLK_VIP_OUT_DIV 0x06080008U
/********Name=CLKSEL_CON09,Offset=0x124********/
#define OUTCLOCK_TEST_DIV 0x04000009U
/********Name=CLKSEL_CON10,Offset=0x128********/
#define CLK_UART_DIV_DIV 0x0500000AU
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define CLK_UART_FRAC_DIV_DIV 0x2000000BU

/********Name=CLKSEL_CON00,Offset=0x100********/
#define HCLK_MCU_BUS_SEL                  0x020E0000U
#define HCLK_MCU_BUS_SEL_CLK_GPLL_MUX     0U
#define HCLK_MCU_BUS_SEL_CLK_GPLL_MUX_NP5 1U
#define HCLK_MCU_BUS_SEL_CLK_USBPLL_MUX   2U
#define HCLK_MCU_BUS_SEL_RESERVED         3U
/********Name=CLKSEL_CON01,Offset=0x104********/
/********Name=CLKSEL_CON02,Offset=0x108********/
#define ACLK_JPEG_BUS_SEL                  0x010F0002U
#define ACLK_JPEG_BUS_SEL_CLK_GPLL_MUX     0U
#define ACLK_JPEG_BUS_SEL_CLK_GPLL_MUX_NP5 1U
/********Name=CLKSEL_CON03,Offset=0x10C********/
/********Name=CLKSEL_CON04,Offset=0x110********/
#define CLK_I2C0_SEL                  0x02060004U
#define CLK_I2C0_SEL_CLK_GPLL_MUX     0U
#define CLK_I2C0_SEL_CLK_GPLL_MUX_NP5 1U
#define CLK_I2C0_SEL_XIN_OSC0_FUNC    2U
#define CLK_I2C1_SEL                  0x020E0004U
#define CLK_I2C1_SEL_CLK_GPLL_MUX     0U
#define CLK_I2C1_SEL_CLK_GPLL_MUX_NP5 1U
#define CLK_I2C1_SEL_XIN_OSC0_FUNC    2U
/********Name=CLKSEL_CON05,Offset=0x114********/
#define CLK_SPI0_SEL               0x01070005U
#define CLK_SPI0_SEL_CLK_GPLL_MUX  0U
#define CLK_SPI0_SEL_XIN_OSC0_FUNC 1U
#define CLK_SPI1_SEL               0x010F0005U
#define CLK_SPI1_SEL_CLK_GPLL_MUX  0U
#define CLK_SPI1_SEL_XIN_OSC0_FUNC 1U
/********Name=CLKSEL_CON06,Offset=0x118********/
#define CLK_PWM_SEL                 0x01070006U
#define CLK_PWM_SEL_CLK_GPLL_MUX    0U
#define CLK_PWM_SEL_XIN_OSC0_FUNC   1U
#define CLK_TIMER_SEL               0x010F0006U
#define CLK_TIMER_SEL_CLK_GPLL_MUX  0U
#define CLK_TIMER_SEL_XIN_OSC0_FUNC 1U
/********Name=CLKSEL_CON07,Offset=0x11C********/
#define CLK_XIP_SFC_SEL                 0x02060007U
#define CLK_XIP_SFC_SEL_CLK_GPLL_MUX    0U
#define CLK_XIP_SFC_SEL_CLK_USBPLL_MUX  1U
#define CLK_XIP_SFC_SEL_XIN_OSC0_FUNC   2U
#define CLK_ISP_SEL                     0x010F0007U
#define CLK_ISP_SEL_CLK_ISP_DIV_OUT     0U
#define CLK_ISP_SEL_CLK_USBPLL_MUX_DIV2 1U
/********Name=CLKSEL_CON08,Offset=0x120********/
#define CLK_VIP_OUT_SEL                0x020E0008U
#define CLK_VIP_OUT_SEL_CLK_GPLL_MUX   0U
#define CLK_VIP_OUT_SEL_CLK_USBPLL_MUX 1U
#define CLK_VIP_OUT_SEL_XIN_OSC0_FUNC  2U
/********Name=CLKSEL_CON09,Offset=0x124********/
#define OUTCLOCK_TEST_SEL                0x04040009U
#define OUTCLOCK_TEST_SEL_XIN_OSC0_FUNC  0U
#define OUTCLOCK_TEST_SEL_CLK_32K        1U
#define OUTCLOCK_TEST_SEL_CLK_GPLL       2U
#define OUTCLOCK_TEST_SEL_CLK_USBPLL     3U
#define OUTCLOCK_TEST_SEL_USB_PHYCLK     4U
#define OUTCLOCK_TEST_SEL_HCLK_MCU_BUS   5U
#define OUTCLOCK_TEST_SEL_ACLK_JPEG_BUS  6U
#define OUTCLOCK_TEST_SEL_PCLK_LOGIC_BUS 7U
#define OUTCLOCK_TEST_SEL_CLK_I2C0       8U
#define OUTCLOCK_TEST_SEL_CLK_I2C1       9U
#define OUTCLOCK_TEST_SEL_CLK_ISP        10U
#define OUTCLOCK_TEST_SEL_MIPI_PHYCLK0   11U
#define OUTCLOCK_TEST_SEL_MIPI_PHYCLK1   12U
#define OUTCLOCK_TEST_SEL_DCLK_VIPCAP    13U
#define OUTCLOCK_TEST_SEL_CLK_XIP_SFC    14U
#define OUTCLOCK_TEST_SEL_SCLK_UART      15U
/********Name=CLKSEL_CON10,Offset=0x128********/
#define CLK_UART_DIV_SEL                0x0107000AU
#define CLK_UART_DIV_SEL_CLK_GPLL_MUX   0U
#define CLK_UART_DIV_SEL_XIN_OSC0_FUNC  1U
#define SCLK_UART_SEL                   0x010F000AU
#define SCLK_UART_SEL_CLK_UART_DIV      0U
#define SCLK_UART_SEL_CLK_UART_FRAC_DIV 1U
/********Name=CLKSEL_CON11,Offset=0x12C********/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __SWALLOW_H */

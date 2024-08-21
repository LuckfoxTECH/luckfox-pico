/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK3358_H
#define __RK3358_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* PMU Register Structure Define */
struct PMU_REG {
    __IO uint32_t PMU_WAKEUP_CFG0_LO;                 /* Address Offset: 0x0000 */
    __IO uint32_t PMU_WAKEUP_CFG0_HI;                 /* Address Offset: 0x0004 */
    __IO uint32_t PMU_WAKEUP_CFG1_LO;                 /* Address Offset: 0x0008 */
    __IO uint32_t PMU_WAKEUP_CFG1_HI;                 /* Address Offset: 0x000C */
    __IO uint32_t PMU_WAKEUP_CFG2_LO;                 /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t PMU_PWRDN_CON_LO;                   /* Address Offset: 0x0018 */
         uint32_t RESERVED001C;                       /* Address Offset: 0x001C */
    __IO uint32_t PMU_PWRDN_ST;                       /* Address Offset: 0x0020 */
    __IO uint32_t PMU_PWRMODE_CORE_CON_LO;            /* Address Offset: 0x0024 */
    __IO uint32_t PMU_PWRMODE_CORE_CON_HI;            /* Address Offset: 0x0028 */
    __IO uint32_t PMU_PWRMODE_COMMON_CON_LO;          /* Address Offset: 0x002C */
    __IO uint32_t PMU_PWRMODE_COMMON_CON_HI;          /* Address Offset: 0x0030 */
    __IO uint32_t PMU_SFT_CON_LO;                     /* Address Offset: 0x0034 */
         uint32_t RESERVED0038[11];                   /* Address Offset: 0x0038 */
    __IO uint32_t PMU_BUS_IDLE_REQ_LO;                /* Address Offset: 0x0064 */
         uint32_t RESERVED0068;                       /* Address Offset: 0x0068 */
    __IO uint32_t PMU_BUS_IDLE_ST;                    /* Address Offset: 0x006C */
         uint32_t RESERVED0070;                       /* Address Offset: 0x0070 */
    __IO uint32_t PMU_OSC_CNT_LO;                     /* Address Offset: 0x0074 */
    __IO uint32_t PMU_OSC_CNT_HI;                     /* Address Offset: 0x0078 */
    __IO uint32_t PMU_PLLLOCK_CNT_LO;                 /* Address Offset: 0x007C */
    __IO uint32_t PMU_PLLLOCK_CNT_HI;                 /* Address Offset: 0x0080 */
    __IO uint32_t PMU_PLLRST_CNT_LO;                  /* Address Offset: 0x0084 */
    __IO uint32_t PMU_PLLRST_CNT_HI;                  /* Address Offset: 0x0088 */
    __IO uint32_t PMU_STABLE_CNT_LO;                  /* Address Offset: 0x008C */
    __IO uint32_t PMU_STABLE_CNT_HI;                  /* Address Offset: 0x0090 */
         uint32_t RESERVED0094;                       /* Address Offset: 0x0094 */
    __IO uint32_t PMU_WAKEUP_RST_CLR_CNT_HI;          /* Address Offset: 0x0098 */
         uint32_t RESERVED009C;                       /* Address Offset: 0x009C */
    __IO uint32_t PMU_WAKEUP_RST_CLR_CNT_LO;          /* Address Offset: 0x00A0 */
    __I  uint32_t PMU_DDR_SREF_ST;                    /* Address Offset: 0x00A4 */
    __IO uint32_t PMU_SYS_REG0_LO;                    /* Address Offset: 0x00A8 */
    __IO uint32_t PMU_SYS_REG0_HI;                    /* Address Offset: 0x00AC */
    __IO uint32_t PMU_SYS_REG1_LO;                    /* Address Offset: 0x00B0 */
    __IO uint32_t PMU_SYS_REG1_HI;                    /* Address Offset: 0x00B4 */
    __IO uint32_t PMU_SYS_REG2_LO;                    /* Address Offset: 0x00B8 */
    __IO uint32_t PMU_SYS_REG2_HI;                    /* Address Offset: 0x00BC */
    __IO uint32_t PMU_SYS_REG3_LO;                    /* Address Offset: 0x00C0 */
    __IO uint32_t PMU_SYS_REG3_HI;                    /* Address Offset: 0x00C4 */
    __IO uint32_t PMU_SCU_PWRDN_CNT_LO;               /* Address Offset: 0x00C8 */
    __IO uint32_t PMU_SCU_PWRDN_CNT_HI;               /* Address Offset: 0x00CC */
    __IO uint32_t PMU_SCU_PWRUP_CNT_LO;               /* Address Offset: 0x00D0 */
    __IO uint32_t PMU_SCU_PWRUP_CNT_HI;               /* Address Offset: 0x00D4 */
    __IO uint32_t PMU_TIMEOUT_CNT_LO;                 /* Address Offset: 0x00D8 */
    __IO uint32_t PMU_TIMEOUT_CNT_HI;                 /* Address Offset: 0x00DC */
    __IO uint32_t PMU_CPU0APM_CON;                    /* Address Offset: 0x00E0 */
    __IO uint32_t PMU_CPU1APM_CON;                    /* Address Offset: 0x00E4 */
    __IO uint32_t PMU_CPU2APM_CON;                    /* Address Offset: 0x00E8 */
    __IO uint32_t PMU_CPU3APM_CON;                    /* Address Offset: 0x00EC */
};
/* PMU_GRF Register Structure Define */
struct PMU_GRF_REG {
    __IO uint32_t GPIO0A_IOMUX;                       /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0B_IOMUX;                       /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0C_IOMUX;                       /* Address Offset: 0x0008 */
         uint32_t RESERVED000C;                       /* Address Offset: 0x000C */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0C_P;                           /* Address Offset: 0x0018 */
         uint32_t RESERVED001C;                       /* Address Offset: 0x001C */
    __IO uint32_t GPIO0A_E;                           /* Address Offset: 0x0020 */
    __IO uint32_t GPIO0B_E;                           /* Address Offset: 0x0024 */
    __IO uint32_t GPIO0C_E;                           /* Address Offset: 0x0028 */
         uint32_t RESERVED002C;                       /* Address Offset: 0x002C */
    __IO uint32_t GPIO0L_SR;                          /* Address Offset: 0x0030 */
    __IO uint32_t GPIO0H_SR;                          /* Address Offset: 0x0034 */
    __IO uint32_t GPIO0L_SMT;                         /* Address Offset: 0x0038 */
    __IO uint32_t GPIO0H_SMT;                         /* Address Offset: 0x003C */
         uint32_t RESERVED0040[48];                   /* Address Offset: 0x0040 */
    __IO uint32_t SOC_CON[3];                         /* Address Offset: 0x0100 */
    __IO uint32_t FAILSAFE_CON;                       /* Address Offset: 0x010C */
         uint32_t RESERVED0110[28];                   /* Address Offset: 0x0110 */
    __IO uint32_t PVTM_CON[2];                        /* Address Offset: 0x0180 */
         uint32_t RESERVED0188[2];                    /* Address Offset: 0x0188 */
    __IO uint32_t PVTM_STATUS[2];                     /* Address Offset: 0x0190 */
         uint32_t RESERVED0198[26];                   /* Address Offset: 0x0198 */
    __IO uint32_t OS_REG[12];                         /* Address Offset: 0x0200 */
    __I  uint32_t RESET_FUNCTION_STATUS;              /* Address Offset: 0x0230 */
         uint32_t RESERVED0234[83];                   /* Address Offset: 0x0234 */
    __IO uint32_t SIG_DETECT_CON;                     /* Address Offset: 0x0380 */
         uint32_t RESERVED0384[3];                    /* Address Offset: 0x0384 */
    __IO uint32_t SIG_DETECT_STATUS;                  /* Address Offset: 0x0390 */
         uint32_t RESERVED0394[3];                    /* Address Offset: 0x0394 */
    __O  uint32_t SIG_DETECT_STATUS_CLEAR;            /* Address Offset: 0x03A0 */
         uint32_t RESERVED03A4[3];                    /* Address Offset: 0x03A4 */
    __IO uint32_t SDMMC_DET_COUNTER;                  /* Address Offset: 0x03B0 */
};
/* UART Register Structure Define */
struct UART_REG {
    union {
        __IO uint32_t RBR;                                /* Address Offset: 0x0000 */
        __IO uint32_t THR;                                /* Address Offset: 0x0000 */
        __IO uint32_t DLL;                                /* Address Offset: 0x0000 */
    };
    union {
        __IO uint32_t DLH;                                /* Address Offset: 0x0004 */
        __IO uint32_t IER;                                /* Address Offset: 0x0004 */
    };
    union {
        __I  uint32_t IIR;                                /* Address Offset: 0x0008 */
        __O  uint32_t FCR;                                /* Address Offset: 0x0008 */
    };
    __IO uint32_t LCR;                                /* Address Offset: 0x000C */
    __IO uint32_t MCR;                                /* Address Offset: 0x0010 */
    __I  uint32_t LSR;                                /* Address Offset: 0x0014 */
    __I  uint32_t MSR;                                /* Address Offset: 0x0018 */
    __IO uint32_t SCR;                                /* Address Offset: 0x001C */
         uint32_t RESERVED0020[4];                    /* Address Offset: 0x0020 */
    __I  uint32_t SRBR;                               /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[14];                   /* Address Offset: 0x0034 */
    __I  uint32_t STHR;                               /* Address Offset: 0x006C */
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
/* GPIO Register Structure Define */
struct GPIO_REG {
    __IO uint32_t SWPORTA_DR;                         /* Address Offset: 0x0000 */
    __IO uint32_t SWPORTA_DDR;                        /* Address Offset: 0x0004 */
         uint32_t RESERVED0008[10];                   /* Address Offset: 0x0008 */
    __IO uint32_t INTEN;                              /* Address Offset: 0x0030 */
    __IO uint32_t INTMASK;                            /* Address Offset: 0x0034 */
    __IO uint32_t INTTYPE_LEVEL;                      /* Address Offset: 0x0038 */
    __IO uint32_t INT_POLARITY;                       /* Address Offset: 0x003C */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0040 */
    __I  uint32_t INT_RAWSTATUS;                      /* Address Offset: 0x0044 */
    __IO uint32_t DEBOUNCE;                           /* Address Offset: 0x0048 */
    __O  uint32_t PORTA_EOI;                          /* Address Offset: 0x004C */
    __I  uint32_t EXT_PORTA;                          /* Address Offset: 0x0050 */
         uint32_t RESERVED0054[3];                    /* Address Offset: 0x0054 */
    __IO uint32_t LS_SYNC;                            /* Address Offset: 0x0060 */
         uint32_t RESERVED0064;                       /* Address Offset: 0x0064 */
    __IO uint32_t INT_BOTHEDGE;                       /* Address Offset: 0x0068 */
};
/* GRF Register Structure Define */
struct GRF_REG {
    __IO uint32_t GPIO1A_IOMUX_L;                     /* Address Offset: 0x0000 */
    __IO uint32_t GPIO1A_IOMUX_H;                     /* Address Offset: 0x0004 */
    __IO uint32_t GPIO1B_IOMUX_L;                     /* Address Offset: 0x0008 */
    __IO uint32_t GPIO1B_IOMUX_H;                     /* Address Offset: 0x000C */
    __IO uint32_t GPIO1C_IOMUX_L;                     /* Address Offset: 0x0010 */
    __IO uint32_t GPIO1C_IOMUX_H;                     /* Address Offset: 0x0014 */
    __IO uint32_t GPIO1D_IOMUX_L;                     /* Address Offset: 0x0018 */
    __IO uint32_t GPIO1D_IOMUX_H;                     /* Address Offset: 0x001C */
    __IO uint32_t GPIO2A_IOMUX_L;                     /* Address Offset: 0x0020 */
    __IO uint32_t GPIO2A_IOMUX_H;                     /* Address Offset: 0x0024 */
    __IO uint32_t GPIO2B_IOMUX_L;                     /* Address Offset: 0x0028 */
    __IO uint32_t GPIO2B_IOMUX_H;                     /* Address Offset: 0x002C */
    __IO uint32_t GPIO2C_IOMUX_L;                     /* Address Offset: 0x0030 */
    __IO uint32_t GPIO2C_IOMUX_H;                     /* Address Offset: 0x0034 */
         uint32_t RESERVED0038[2];                    /* Address Offset: 0x0038 */
    __IO uint32_t GPIO3A_IOMUX_L;                     /* Address Offset: 0x0040 */
    __IO uint32_t GPIO3A_IOMUX_H;                     /* Address Offset: 0x0044 */
    __IO uint32_t GPIO3B_IOMUX_L;                     /* Address Offset: 0x0048 */
    __IO uint32_t GPIO3B_IOMUX_H;                     /* Address Offset: 0x004C */
    __IO uint32_t GPIO3C_IOMUX_L;                     /* Address Offset: 0x0050 */
    __IO uint32_t GPIO3C_IOMUX_H;                     /* Address Offset: 0x0054 */
    __IO uint32_t GPIO3D_IOMUX_L;                     /* Address Offset: 0x0058 */
    __IO uint32_t GPIO3D_IOMUX_H;                     /* Address Offset: 0x005C */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0060 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0064 */
    __IO uint32_t GPIO1C_P;                           /* Address Offset: 0x0068 */
    __IO uint32_t GPIO1D_P;                           /* Address Offset: 0x006C */
    __IO uint32_t GPIO2A_P;                           /* Address Offset: 0x0070 */
    __IO uint32_t GPIO2B_P;                           /* Address Offset: 0x0074 */
    __IO uint32_t GPIO2C_P;                           /* Address Offset: 0x0078 */
         uint32_t RESERVED007C;                       /* Address Offset: 0x007C */
    __IO uint32_t GPIO3A_P;                           /* Address Offset: 0x0080 */
    __IO uint32_t GPIO3B_P;                           /* Address Offset: 0x0084 */
    __IO uint32_t GPIO3C_P;                           /* Address Offset: 0x0088 */
    __IO uint32_t GPIO3D_P;                           /* Address Offset: 0x008C */
    __IO uint32_t GPIO1A_SR;                          /* Address Offset: 0x0090 */
    __IO uint32_t GPIO1B_SR;                          /* Address Offset: 0x0094 */
    __IO uint32_t GPIO1C_SR;                          /* Address Offset: 0x0098 */
    __IO uint32_t GPIO1D_SR;                          /* Address Offset: 0x009C */
    __IO uint32_t GPIO2A_SR;                          /* Address Offset: 0x00A0 */
    __IO uint32_t GPIO2B_SR;                          /* Address Offset: 0x00A4 */
    __IO uint32_t GPIO2C_SR;                          /* Address Offset: 0x00A8 */
         uint32_t RESERVED00AC;                       /* Address Offset: 0x00AC */
    __IO uint32_t GPIO3A_SR;                          /* Address Offset: 0x00B0 */
    __IO uint32_t GPIO3B_SR;                          /* Address Offset: 0x00B4 */
    __IO uint32_t GPIO3C_SR;                          /* Address Offset: 0x00B8 */
    __IO uint32_t GPIO3D_SR;                          /* Address Offset: 0x00BC */
    __IO uint32_t GPIO1A_SMT;                         /* Address Offset: 0x00C0 */
    __IO uint32_t GPIO1B_SMT;                         /* Address Offset: 0x00C4 */
    __IO uint32_t GPIO1C_SMT;                         /* Address Offset: 0x00C8 */
    __IO uint32_t GPIO1D_SMT;                         /* Address Offset: 0x00CC */
    __IO uint32_t GPIO2A_SMT;                         /* Address Offset: 0x00D0 */
    __IO uint32_t GPIO2B_SMT;                         /* Address Offset: 0x00D4 */
    __IO uint32_t GPIO2C_SMT;                         /* Address Offset: 0x00D8 */
         uint32_t RESERVED00DC;                       /* Address Offset: 0x00DC */
    __IO uint32_t GPIO3A_SMT;                         /* Address Offset: 0x00E0 */
    __IO uint32_t GPIO3B_SMT;                         /* Address Offset: 0x00E4 */
    __IO uint32_t GPIO3C_SMT;                         /* Address Offset: 0x00E8 */
    __IO uint32_t GPIO3D_SMT;                         /* Address Offset: 0x00EC */
    __IO uint32_t GPIO1A_E;                           /* Address Offset: 0x00F0 */
    __IO uint32_t GPIO1B_E;                           /* Address Offset: 0x00F4 */
    __IO uint32_t GPIO1C_E;                           /* Address Offset: 0x00F8 */
    __IO uint32_t GPIO1D_E;                           /* Address Offset: 0x00FC */
    __IO uint32_t GPIO2A_E;                           /* Address Offset: 0x0100 */
    __IO uint32_t GPIO2B_E;                           /* Address Offset: 0x0104 */
    __IO uint32_t GPIO2C_E;                           /* Address Offset: 0x0108 */
         uint32_t RESERVED010C;                       /* Address Offset: 0x010C */
    __IO uint32_t GPIO3A_E;                           /* Address Offset: 0x0110 */
    __IO uint32_t GPIO3B_E;                           /* Address Offset: 0x0114 */
    __IO uint32_t GPIO3C_E;                           /* Address Offset: 0x0118 */
    __IO uint32_t GPIO3D_E;                           /* Address Offset: 0x011C */
         uint32_t RESERVED0120[24];                   /* Address Offset: 0x0120 */
    __IO uint32_t IO_VSEL;                            /* Address Offset: 0x0180 */
    __IO uint32_t IOFUNC_CON[1];                      /* Address Offset: 0x0184 */
         uint32_t RESERVED0188[158];                  /* Address Offset: 0x0188 */
    __IO uint32_t SOC_CON[6];                         /* Address Offset: 0x0400 */
         uint32_t RESERVED0418[6];                    /* Address Offset: 0x0418 */
    __IO uint32_t PD_VI_CON;                          /* Address Offset: 0x0430 */
    __IO uint32_t PD_VO_CON[2];                       /* Address Offset: 0x0434 */
         uint32_t RESERVED043C[17];                   /* Address Offset: 0x043C */
    __I  uint32_t SOC_STATUS[1];                      /* Address Offset: 0x0480 */
         uint32_t RESERVED0484[31];                   /* Address Offset: 0x0484 */
    __IO uint32_t CPU_CON[3];                         /* Address Offset: 0x0500 */
         uint32_t RESERVED050C[5];                    /* Address Offset: 0x050C */
    __I  uint32_t CPU_STATUS[2];                      /* Address Offset: 0x0520 */
         uint32_t RESERVED0528[2];                    /* Address Offset: 0x0528 */
    __IO uint32_t SOC_NOC_CON[2];                     /* Address Offset: 0x0530 */
         uint32_t RESERVED0538[6];                    /* Address Offset: 0x0538 */
    __IO uint32_t DDR_BANKHASH_CTRL;                  /* Address Offset: 0x0550 */
    __IO uint32_t DDR_BANK_MASK[3];                   /* Address Offset: 0x0554 */
         uint32_t RESERVED0560[104];                  /* Address Offset: 0x0560 */
    __IO uint32_t HOST0_CON[2];                       /* Address Offset: 0x0700 */
         uint32_t RESERVED0708[62];                   /* Address Offset: 0x0708 */
    __I  uint32_t CHIP_ID;                            /* Address Offset: 0x0800 */
         uint32_t RESERVED0804[31];                   /* Address Offset: 0x0804 */
    __IO uint32_t OTG_CON[1];                         /* Address Offset: 0x0880 */
         uint32_t RESERVED0884[3];                    /* Address Offset: 0x0884 */
    __I  uint32_t HOST0_STATUS[1];                    /* Address Offset: 0x0890 */
         uint32_t RESERVED0894[28];                   /* Address Offset: 0x0894 */
    __IO uint32_t MAC_CON[1];                         /* Address Offset: 0x0904 */
};
/* CORE_GRF Register Structure Define */
struct CORE_GRF_REG {
    __IO uint32_t CA35_PEFF_CON[9];                   /* Address Offset: 0x0000 */
         uint32_t RESERVED0024[3];                    /* Address Offset: 0x0024 */
    __I  uint32_t A35_PERF_RD_MAX_LATENCY_NUM;        /* Address Offset: 0x0030 */
    __I  uint32_t A35_PERF_RD_LATENCY_SAMP_NUM;       /* Address Offset: 0x0034 */
    __I  uint32_t A35_PERF_RD_LATENCY_ACC_NUM;        /* Address Offset: 0x0038 */
    __I  uint32_t A35_PERF_RD_AXI_TOTAL_BYTE;         /* Address Offset: 0x003C */
    __I  uint32_t A35_PERF_WR_AXI_TOTAL_BYTE;         /* Address Offset: 0x0040 */
    __I  uint32_t A35_PERF_WORKING_CNT;               /* Address Offset: 0x0044 */
    __I  uint32_t A35_PERF_INT_STATUS;                /* Address Offset: 0x0048 */
         uint32_t RESERVED004C[13];                   /* Address Offset: 0x004C */
    __IO uint32_t COREPVTM_CON[2];                    /* Address Offset: 0x0080 */
    __IO uint32_t COREPVTM_STATUS[2];                 /* Address Offset: 0x0088 */
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
         uint32_t RESERVED0048[238];                  /* Address Offset: 0x0048 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0400 */
         uint32_t RESERVED0404[255];                  /* Address Offset: 0x0404 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x0800 */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
};
/* DMA Register Structure Define */
struct DMA_CHANNEL_STATUS {
    __I  uint32_t CSR;
    __I  uint32_t CPC;
};
struct DMA_CHANNEL_CONFIG {
    __I  uint32_t SAR;
    __I  uint32_t DAR;
    __I  uint32_t CCR;
    __I  uint32_t LC0;
    __I  uint32_t LC1;
    __I  uint32_t PADDING[3];
};
struct DMA_REG {
    __I  uint32_t DSR;                                /* Address Offset: 0x0000 */
    __I  uint32_t DPC;                                /* Address Offset: 0x0004 */
         uint32_t RESERVED0008[6];                    /* Address Offset: 0x0008 */
    __IO uint32_t INTEN;                              /* Address Offset: 0x0020 */
    __I  uint32_t EVENT_RIS;                          /* Address Offset: 0x0024 */
    __I  uint32_t INTMIS;                             /* Address Offset: 0x0028 */
    __O  uint32_t INTCLR;                             /* Address Offset: 0x002C */
    __I  uint32_t FSRD;                               /* Address Offset: 0x0030 */
    __I  uint32_t FSRC;                               /* Address Offset: 0x0034 */
    __IO uint32_t FTRD;                               /* Address Offset: 0x0038 */
         uint32_t RESERVED003C;                       /* Address Offset: 0x003C */
    __I  uint32_t FTR[6];                             /* Address Offset: 0x0040 */
         uint32_t RESERVED0058[42];                   /* Address Offset: 0x0058 */
         struct DMA_CHANNEL_STATUS CHAN_STS[6];       /* Address Offset: 0x0100 */
         uint32_t RESERVED0130[180];                  /* Address Offset: 0x0130 */
         struct DMA_CHANNEL_CONFIG CHAN_CFG[6];       /* Address Offset: 0x0400 */
         uint32_t RESERVED04C0[528];                  /* Address Offset: 0x04C0 */
    __I  uint32_t DBGSTATUS;                          /* Address Offset: 0x0D00 */
    __O  uint32_t DBGCMD;                             /* Address Offset: 0x0D04 */
    __O  uint32_t DBGINST[2];                         /* Address Offset: 0x0D08 */
         uint32_t RESERVED0D10[60];                   /* Address Offset: 0x0D10 */
    __I  uint32_t CR[5];                              /* Address Offset: 0x0E00 */
    __I  uint32_t CRDN;                               /* Address Offset: 0x0E14 */
         uint32_t RESERVED0E18[26];                   /* Address Offset: 0x0E18 */
    __IO uint32_t WD;                                 /* Address Offset: 0x0E80 */
};
/* TSADC Register Structure Define */
struct TSADC_REG {
    __IO uint32_t USER_CON;                           /* Address Offset: 0x0000 */
    __IO uint32_t AUTO_CON;                           /* Address Offset: 0x0004 */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x0008 */
    __IO uint32_t INT_PD;                             /* Address Offset: 0x000C */
         uint32_t RESERVED0010[4];                    /* Address Offset: 0x0010 */
    __I  uint32_t DATA[2];                            /* Address Offset: 0x0020 */
         uint32_t RESERVED0028[2];                    /* Address Offset: 0x0028 */
    __IO uint32_t COMP_INT[2];                        /* Address Offset: 0x0030 */
         uint32_t RESERVED0038[2];                    /* Address Offset: 0x0038 */
    __IO uint32_t COMP_SHUT[2];                       /* Address Offset: 0x0040 */
         uint32_t RESERVED0048[6];                    /* Address Offset: 0x0048 */
    __IO uint32_t HIGHT_INT_DEBOUNCE;                 /* Address Offset: 0x0060 */
    __IO uint32_t HIGHT_TSHUT_DEBOUNCE;               /* Address Offset: 0x0064 */
    __IO uint32_t AUTO_PERIOD;                        /* Address Offset: 0x0068 */
    __IO uint32_t AUTO_PERIOD_HT;                     /* Address Offset: 0x006C */
         uint32_t RESERVED0070[4];                    /* Address Offset: 0x0070 */
    __IO uint32_t COMP_LOW_INT[2];                    /* Address Offset: 0x0080 */
};
/* SARADC Register Structure Define */
struct SARADC_REG {
    __I  uint32_t DATA;                               /* Address Offset: 0x0000 */
    __I  uint32_t STAS;                               /* Address Offset: 0x0004 */
    __IO uint32_t CTRL;                               /* Address Offset: 0x0008 */
    __IO uint32_t DLY_PU_SOC;                         /* Address Offset: 0x000C */
};
/* CRU Register Structure Define */
struct CRU_REG {
    __IO uint32_t APLL_CON[5];                        /* Address Offset: 0x0000 */
         uint32_t RESERVED0014[3];                    /* Address Offset: 0x0014 */
    __IO uint32_t DPLL_CON[5];                        /* Address Offset: 0x0020 */
         uint32_t RESERVED0034[3];                    /* Address Offset: 0x0034 */
    __IO uint32_t CPLL_CON[5];                        /* Address Offset: 0x0040 */
         uint32_t RESERVED0054[3];                    /* Address Offset: 0x0054 */
    __IO uint32_t NPLL_CON[5];                        /* Address Offset: 0x0060 */
         uint32_t RESERVED0074[11];                   /* Address Offset: 0x0074 */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x00A0 */
    __IO uint32_t MISC_CON;                           /* Address Offset: 0x00A4 */
         uint32_t RESERVED00A8[2];                    /* Address Offset: 0x00A8 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x00B0 */
    __IO uint32_t GLB_RST_ST;                         /* Address Offset: 0x00B4 */
    __IO uint32_t GLB_SRST_FST_VALUE;                 /* Address Offset: 0x00B8 */
    __IO uint32_t GLB_SRST_SND_VALUE;                 /* Address Offset: 0x00BC */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x00C0 */
         uint32_t RESERVED00C4[7];                    /* Address Offset: 0x00C4 */
    __IO uint32_t HWFFC_CON[1];                       /* Address Offset: 0x00E0 */
         uint32_t RESERVED00E4;                       /* Address Offset: 0x00E4 */
    __IO uint32_t HWFFC_TH;                           /* Address Offset: 0x00E8 */
    __IO uint32_t HWFFC_INTST;                        /* Address Offset: 0x00EC */
    __I  uint32_t APLL_CON0_S;                        /* Address Offset: 0x00F0 */
    __I  uint32_t APLL_CON1_S;                        /* Address Offset: 0x00F4 */
    __I  uint32_t CLKSEL_CON0_S;                      /* Address Offset: 0x00F8 */
         uint32_t RESERVED00FC;                       /* Address Offset: 0x00FC */
    __IO uint32_t CRU_CLKSEL_CON[60];                 /* Address Offset: 0x0100 */
         uint32_t RESERVED01F0[4];                    /* Address Offset: 0x01F0 */
    __IO uint32_t CRU_CLKGATE_CON[18];                /* Address Offset: 0x0200 */
         uint32_t RESERVED0248[14];                   /* Address Offset: 0x0248 */
    __O  uint32_t SSGTBL0[1];                         /* Address Offset: 0x0280 */
    __O  uint32_t SSGTBL4[1];                         /* Address Offset: 0x0284 */
    __O  uint32_t SSGTBL8[1];                         /* Address Offset: 0x0288 */
    __O  uint32_t SSGTBL12[1];                        /* Address Offset: 0x028C */
    __O  uint32_t SSGTBL16[1];                        /* Address Offset: 0x0290 */
    __O  uint32_t SSGTBL20[1];                        /* Address Offset: 0x0294 */
    __O  uint32_t SSGTBL24[1];                        /* Address Offset: 0x0298 */
    __O  uint32_t SSGTBL28[1];                        /* Address Offset: 0x029C */
    __O  uint32_t SSGTBL32[1];                        /* Address Offset: 0x02A0 */
    __O  uint32_t SSGTBL36[1];                        /* Address Offset: 0x02A4 */
    __O  uint32_t SSGTBL40[1];                        /* Address Offset: 0x02A8 */
    __O  uint32_t SSGTBL44[1];                        /* Address Offset: 0x02AC */
    __O  uint32_t SSGTBL48[1];                        /* Address Offset: 0x02B0 */
    __O  uint32_t SSGTBL52[1];                        /* Address Offset: 0x02B4 */
    __O  uint32_t SSGTBL56[1];                        /* Address Offset: 0x02B8 */
    __O  uint32_t SSGTBL60[1];                        /* Address Offset: 0x02BC */
    __O  uint32_t SSGTBL64[1];                        /* Address Offset: 0x02C0 */
    __O  uint32_t SSGTBL68[1];                        /* Address Offset: 0x02C4 */
    __O  uint32_t SSGTBL72[1];                        /* Address Offset: 0x02C8 */
    __O  uint32_t SSGTBL76[1];                        /* Address Offset: 0x02CC */
    __O  uint32_t SSGTBL80[1];                        /* Address Offset: 0x02D0 */
    __O  uint32_t SSGTBL84[1];                        /* Address Offset: 0x02D4 */
    __O  uint32_t SSGTBL88[1];                        /* Address Offset: 0x02D8 */
    __O  uint32_t SSGTBL92[1];                        /* Address Offset: 0x02DC */
    __O  uint32_t SSGTBL96[1];                        /* Address Offset: 0x02E0 */
    __O  uint32_t SSGTBL100[1];                       /* Address Offset: 0x02E4 */
    __O  uint32_t SSGTBL104[1];                       /* Address Offset: 0x02E8 */
    __O  uint32_t SSGTBL108[1];                       /* Address Offset: 0x02EC */
    __O  uint32_t SSGTBL112[1];                       /* Address Offset: 0x02F0 */
    __O  uint32_t SSGTBL116[1];                       /* Address Offset: 0x02F4 */
    __O  uint32_t SSGTBL120[1];                       /* Address Offset: 0x02F8 */
    __O  uint32_t SSGTBL124[1];                       /* Address Offset: 0x02FC */
    __IO uint32_t CRU_SOFTRST_CON[12];                /* Address Offset: 0x0300 */
         uint32_t RESERVED0330[20];                   /* Address Offset: 0x0330 */
    __IO uint32_t SDMMC_CON[2];                       /* Address Offset: 0x0380 */
    __IO uint32_t SDIO_CON[2];                        /* Address Offset: 0x0388 */
    __IO uint32_t EMMC_CON[2];                        /* Address Offset: 0x0390 */
         uint32_t RESERVED0398[26];                   /* Address Offset: 0x0398 */
    __IO uint32_t AUTOCS_CON[8];                      /* Address Offset: 0x0400 */
         uint32_t RESERVED0420[12024];                /* Address Offset: 0x0420 */
    __IO uint32_t GPLL_CON[5];                        /* Address Offset: 0xC000 */
         uint32_t RESERVEDC014[3];                    /* Address Offset: 0xC014 */
    __IO uint32_t PMU_MODE_CON00;                     /* Address Offset: 0xC020 */
         uint32_t RESERVEDC024[7];                    /* Address Offset: 0xC024 */
    __IO uint32_t PMU_CLKSEL_CON[6];                  /* Address Offset: 0xC040 */
         uint32_t RESERVEDC058[10];                   /* Address Offset: 0xC058 */
    __IO uint32_t PMU_CLKGATE_CON[2];                 /* Address Offset: 0xC080 */
         uint32_t RESERVEDC088[14];                   /* Address Offset: 0xC088 */
    __IO uint32_t PMU_AUTOCS_CON[2];                  /* Address Offset: 0xC0C0 */
};
/* USBPHY_GRF Register Structure Define */
struct USBPHY_GRF_REG {
    __IO uint32_t REG[24];                            /* Address Offset: 0x0000 */
         uint32_t RESERVED0060[40];                   /* Address Offset: 0x0060 */
    __IO uint32_t CON[4];                             /* Address Offset: 0x0100 */
    __IO uint32_t INT_MASK;                           /* Address Offset: 0x0110 */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0114 */
    __O  uint32_t INT_STATUS_CLR;                     /* Address Offset: 0x0118 */
         uint32_t RESERVED011C;                       /* Address Offset: 0x011C */
    __I  uint32_t STATUS;                             /* Address Offset: 0x0120 */
         uint32_t RESERVED0124[3];                    /* Address Offset: 0x0124 */
    __IO uint32_t LS_CON;                             /* Address Offset: 0x0130 */
    __IO uint32_t DIS_CON;                            /* Address Offset: 0x0134 */
    __IO uint32_t BVALID_CON;                         /* Address Offset: 0x0138 */
    __IO uint32_t ID_CON;                             /* Address Offset: 0x013C */
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
         uint32_t RESERVED0020;                       /* Address Offset: 0x0020 */
    __IO uint32_t DEBUG;                              /* Address Offset: 0x0024 */
         uint32_t RESERVED0028;                       /* Address Offset: 0x0028 */
    __IO uint32_t PMT_CTRL_STA;                       /* Address Offset: 0x002C */
         uint32_t RESERVED0030[2];                    /* Address Offset: 0x0030 */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0038 */
    __IO uint32_t INT_MASK;                           /* Address Offset: 0x003C */
    __IO uint32_t MAC_ADDR0_HI;                       /* Address Offset: 0x0040 */
    __IO uint32_t MAC_ADDR0_LO;                       /* Address Offset: 0x0044 */
         uint32_t RESERVED0048[30];                   /* Address Offset: 0x0048 */
    __IO uint32_t AN_CTRL;                            /* Address Offset: 0x00C0 */
    __IO uint32_t AN_STATUS;                          /* Address Offset: 0x00C4 */
    __IO uint32_t AN_ADV;                             /* Address Offset: 0x00C8 */
    __I  uint32_t AN_LINK_PART_AB;                    /* Address Offset: 0x00CC */
    __I  uint32_t AN_EXP;                             /* Address Offset: 0x00D0 */
         uint32_t RESERVED00D4;                       /* Address Offset: 0x00D4 */
    __IO uint32_t INTF_MODE_STA;                      /* Address Offset: 0x00D8 */
         uint32_t RESERVED00DC[9];                    /* Address Offset: 0x00DC */
    __IO uint32_t MMC_CTRL;                           /* Address Offset: 0x0100 */
    __IO uint32_t MMC_RX_INTR;                        /* Address Offset: 0x0104 */
    __I  uint32_t MMC_TX_INTR;                        /* Address Offset: 0x0108 */
    __IO uint32_t MMC_RX_INT_MSK;                     /* Address Offset: 0x010C */
    __IO uint32_t MMC_TX_INT_MSK;                     /* Address Offset: 0x0110 */
    __IO uint32_t MMC_TXOCTETCNT_GB;                  /* Address Offset: 0x0114 */
    __IO uint32_t MMC_TXFRMCNT_GB;                    /* Address Offset: 0x0118 */
         uint32_t RESERVED011C[11];                   /* Address Offset: 0x011C */
    __IO uint32_t MMC_TXUNDFLWERR;                    /* Address Offset: 0x0148 */
         uint32_t RESERVED014C[5];                    /* Address Offset: 0x014C */
    __IO uint32_t MMC_TXCARERR;                       /* Address Offset: 0x0160 */
    __IO uint32_t MMC_TXOCTETCNT_G;                   /* Address Offset: 0x0164 */
    __IO uint32_t MMC_TXFRMCNT_G;                     /* Address Offset: 0x0168 */
         uint32_t RESERVED016C[5];                    /* Address Offset: 0x016C */
    __IO uint32_t MMC_RXFRMCNT_GB;                    /* Address Offset: 0x0180 */
    __IO uint32_t MMC_RXOCTETCNT_GB;                  /* Address Offset: 0x0184 */
    __IO uint32_t MMC_RXOCTETCNT_G;                   /* Address Offset: 0x0188 */
         uint32_t RESERVED018C;                       /* Address Offset: 0x018C */
    __IO uint32_t MMC_RXMCFRMCNT_G;                   /* Address Offset: 0x0190 */
    __IO uint32_t MMC_RXCRCERR;                       /* Address Offset: 0x0194 */
         uint32_t RESERVED0198[12];                   /* Address Offset: 0x0198 */
    __IO uint32_t MMC_RXLENERR;                       /* Address Offset: 0x01C8 */
         uint32_t RESERVED01CC[2];                    /* Address Offset: 0x01CC */
    __IO uint32_t MMC_RXFIFOOVRFLW;                   /* Address Offset: 0x01D4 */
         uint32_t RESERVED01D8[10];                   /* Address Offset: 0x01D8 */
    __IO uint32_t MMC_IPC_INT_MSK;                    /* Address Offset: 0x0200 */
         uint32_t RESERVED0204;                       /* Address Offset: 0x0204 */
    __I  uint32_t MMC_IPC_INTR;                       /* Address Offset: 0x0208 */
         uint32_t RESERVED020C;                       /* Address Offset: 0x020C */
    __IO uint32_t MMC_RXIPV4GFRM;                     /* Address Offset: 0x0210 */
    __IO uint32_t MMC_RXIPV4HDERRFRM;                 /* Address Offset: 0x0214 */
         uint32_t RESERVED0218[3];                    /* Address Offset: 0x0218 */
    __IO uint32_t MMC_RXIPV6GFRM;                     /* Address Offset: 0x0224 */
    __IO uint32_t MMC_RXIPV6HDERRFRM;                 /* Address Offset: 0x0228 */
         uint32_t RESERVED022C[2];                    /* Address Offset: 0x022C */
    __IO uint32_t MMC_RXUDPERRFRM;                    /* Address Offset: 0x0234 */
         uint32_t RESERVED0238;                       /* Address Offset: 0x0238 */
    __IO uint32_t MMC_RXTCPERRFRM;                    /* Address Offset: 0x023C */
         uint32_t RESERVED0240;                       /* Address Offset: 0x0240 */
    __IO uint32_t MMC_RXICMPERRFRM;                   /* Address Offset: 0x0244 */
         uint32_t RESERVED0248[3];                    /* Address Offset: 0x0248 */
    __IO uint32_t MMC_RXIPV4HDERROCT;                 /* Address Offset: 0x0254 */
         uint32_t RESERVED0258[4];                    /* Address Offset: 0x0258 */
    __IO uint32_t MMC_RXIPV6HDERROCT;                 /* Address Offset: 0x0268 */
         uint32_t RESERVED026C[2];                    /* Address Offset: 0x026C */
    __IO uint32_t MMC_RXUDPERROCT;                    /* Address Offset: 0x0274 */
         uint32_t RESERVED0278;                       /* Address Offset: 0x0278 */
    __IO uint32_t MMC_RXTCPERROCT;                    /* Address Offset: 0x027C */
         uint32_t RESERVED0280;                       /* Address Offset: 0x0280 */
    __IO uint32_t MMC_RXICMPERROCT;                   /* Address Offset: 0x0284 */
         uint32_t RESERVED0288[862];                  /* Address Offset: 0x0288 */
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
         uint32_t RESERVED1030[6];                    /* Address Offset: 0x1030 */
    __I  uint32_t CUR_HOST_TX_DESC;                   /* Address Offset: 0x1048 */
    __I  uint32_t CUR_HOST_RX_DESC;                   /* Address Offset: 0x104C */
    __I  uint32_t CUR_HOST_TX_BUF_ADDR;               /* Address Offset: 0x1050 */
    __I  uint32_t CUR_HOST_RX_BUF_ADDR;               /* Address Offset: 0x1054 */
};
/* MMC Register Structure Define */
struct MMC_REG {
    __IO uint32_t CTRL;                               /* Address Offset: 0x0000 */
    __IO uint32_t PWREN;                              /* Address Offset: 0x0004 */
    __IO uint32_t CLKDIV;                             /* Address Offset: 0x0008 */
    __IO uint32_t CLKSRC;                             /* Address Offset: 0x000C */
    __IO uint32_t CLKENA;                             /* Address Offset: 0x0010 */
    __IO uint32_t TMOUT;                              /* Address Offset: 0x0014 */
    __IO uint32_t CTYPE;                              /* Address Offset: 0x0018 */
    __IO uint32_t BLKSIZ;                             /* Address Offset: 0x001C */
    __IO uint32_t BYTCNT;                             /* Address Offset: 0x0020 */
    __IO uint32_t INTMASK;                            /* Address Offset: 0x0024 */
    __IO uint32_t CMDARG;                             /* Address Offset: 0x0028 */
    __IO uint32_t CMD;                                /* Address Offset: 0x002C */
    __I  uint32_t RESP[4];                            /* Address Offset: 0x0030 */
    __IO uint32_t MINTSTS;                            /* Address Offset: 0x0040 */
    __IO uint32_t RINTSTS;                            /* Address Offset: 0x0044 */
    __I  uint32_t STATUS;                             /* Address Offset: 0x0048 */
    __IO uint32_t FIFOTH;                             /* Address Offset: 0x004C */
    __I  uint32_t CDETECT;                            /* Address Offset: 0x0050 */
    __IO uint32_t WRTPRT;                             /* Address Offset: 0x0054 */
         uint32_t RESERVED0058;                       /* Address Offset: 0x0058 */
    __I  uint32_t TCBCNT;                             /* Address Offset: 0x005C */
    __I  uint32_t TBBCNT;                             /* Address Offset: 0x0060 */
    __IO uint32_t DEBNCE;                             /* Address Offset: 0x0064 */
    __IO uint32_t USRID;                              /* Address Offset: 0x0068 */
    __I  uint32_t VERID;                              /* Address Offset: 0x006C */
    __I  uint32_t HCON;                               /* Address Offset: 0x0070 */
    __IO uint32_t UHS_REG;                            /* Address Offset: 0x0074 */
    __IO uint32_t RST_n;                              /* Address Offset: 0x0078 */
         uint32_t RESERVED007C;                       /* Address Offset: 0x007C */
    __IO uint32_t BMOD;                               /* Address Offset: 0x0080 */
    __O  uint32_t PLDMND;                             /* Address Offset: 0x0084 */
    __IO uint32_t DBADDR;                             /* Address Offset: 0x0088 */
    __IO uint32_t IDSTS;                              /* Address Offset: 0x008C */
    __IO uint32_t IDINTEN;                            /* Address Offset: 0x0090 */
    __IO uint32_t DSCADDR;                            /* Address Offset: 0x0094 */
    __IO uint32_t BUFADDR;                            /* Address Offset: 0x0098 */
         uint32_t RESERVED009C[25];                   /* Address Offset: 0x009C */
    __IO uint32_t CARDTHRCTL;                         /* Address Offset: 0x0100 */
    __IO uint32_t BACK_END_POWER;                     /* Address Offset: 0x0104 */
    __IO uint32_t UHS_REG_EXT;                        /* Address Offset: 0x0108 */
    __IO uint32_t EMMC_DDR_REG;                       /* Address Offset: 0x010C */
    __IO uint32_t ENABLE_SHIFT;                       /* Address Offset: 0x0110 */
         uint32_t RESERVED0114[59];                   /* Address Offset: 0x0114 */
    __IO uint32_t FIFO_BASE;                          /* Address Offset: 0x0200 */
};
#endif /*  __ASSEMBLY__  */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define PMU_BASE                       0xFF000000U /* PMU base address */
#define PMU_GRF_BASE                   0xFF010000U /* PMU_GRF base address */
#define UART0_BASE                     0xFF030000U /* UART0 base address */
#define GPIO0_BASE                     0xFF040000U /* GPIO0 base address */
#define GRF_BASE                       0xFF140000U /* GRF base address */
#define CORE_GRF_BASE                  0xFF148000U /* CORE_GRF base address */
#define UART1_BASE                     0xFF158000U /* UART1 base address */
#define UART2_BASE                     0xFF160000U /* UART2 base address */
#define UART3_BASE                     0xFF168000U /* UART3 base address */
#define UART4_BASE                     0xFF170000U /* UART4 base address */
#define UART5_BASE                     0xFF178000U /* UART5 base address */
#define I2C0_BASE                      0xFF180000U /* I2C0 base address */
#define I2C1_BASE                      0xFF190000U /* I2C1 base address */
#define I2C2_BASE                      0xFF1A0000U /* I2C2 base address */
#define I2C3_BASE                      0xFF1B0000U /* I2C3 base address */
#define SPI0_BASE                      0xFF1D0000U /* SPI0 base address */
#define SPI1_BASE                      0xFF1D8000U /* SPI1 base address */
#define TIMER0_BASE                    0xFF210000U /* TIMER0 base address */
#define TIMER1_BASE                    0xFF210020U /* TIMER1 base address */
#define TIMER2_BASE                    0xFF210040U /* TIMER2 base address */
#define TIMER3_BASE                    0xFF210060U /* TIMER3 base address */
#define TIMER4_BASE                    0xFF210080U /* TIMER4 base address */
#define TIMER5_BASE                    0xFF2100A0U /* TIMER5 base address */
#define DMA0_BASE                      0xFF240000U /* DMA0 base address */
#define GPIO1_BASE                     0xFF250000U /* GPIO1 base address */
#define GPIO2_BASE                     0xFF260000U /* GPIO2 base address */
#define GPIO3_BASE                     0xFF270000U /* GPIO3 base address */
#define TSADC_BASE                     0xFF280000U /* TSADC base address */
#define SARADC_BASE                    0xFF288000U /* SARADC base address */
#define CRU_BASE                       0xFF2B0000U /* CRU base address */
#define USBPHY_GRF_BASE                0xFF2C0000U /* USBPHY_GRF base address */
#define GMAC_BASE                      0xFF360000U /* GMAC base address */
#define MMC_BASE                       0xFF390000U /* MMC base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define PMU                 ((struct PMU_REG *) PMU_BASE)
#define PMU_GRF             ((struct PMU_GRF_REG *) PMU_GRF_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define GRF                 ((struct GRF_REG *) GRF_BASE)
#define CORE_GRF            ((struct CORE_GRF_REG *) CORE_GRF_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)
#define UART3               ((struct UART_REG *) UART3_BASE)
#define UART4               ((struct UART_REG *) UART4_BASE)
#define UART5               ((struct UART_REG *) UART5_BASE)
#define I2C0                ((struct I2C_REG *) I2C0_BASE)
#define I2C1                ((struct I2C_REG *) I2C1_BASE)
#define I2C2                ((struct I2C_REG *) I2C2_BASE)
#define I2C3                ((struct I2C_REG *) I2C3_BASE)
#define SPI0                ((struct SPI_REG *) SPI0_BASE)
#define SPI1                ((struct SPI_REG *) SPI1_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER0_BASE)
#define TIMER1              ((struct TIMER_REG *) TIMER1_BASE)
#define TIMER2              ((struct TIMER_REG *) TIMER2_BASE)
#define TIMER3              ((struct TIMER_REG *) TIMER3_BASE)
#define TIMER4              ((struct TIMER_REG *) TIMER4_BASE)
#define TIMER5              ((struct TIMER_REG *) TIMER5_BASE)
#define DMA0                ((struct DMA_REG *) DMA0_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define GPIO2               ((struct GPIO_REG *) GPIO2_BASE)
#define GPIO3               ((struct GPIO_REG *) GPIO3_BASE)
#define TSADC               ((struct TSADC_REG *) TSADC_BASE)
#define SARADC              ((struct SARADC_REG *) SARADC_BASE)
#define CRU                 ((struct CRU_REG *) CRU_BASE)
#define USBPHY_GRF          ((struct USBPHY_GRF_REG *) USBPHY_GRF_BASE)
#define GMAC                ((struct GMAC_REG *) GMAC_BASE)
#define MMC                 ((struct MMC_REG *) MMC_BASE)

#define IS_PMU_INSTANCE(instance) ((instance) == PMU)
#define IS_PMU_GRF_INSTANCE(instance) ((instance) == PMU_GRF)
#define IS_GRF_INSTANCE(instance) ((instance) == GRF)
#define IS_CORE_GRF_INSTANCE(instance) ((instance) == CORE_GRF)
#define IS_TSADC_INSTANCE(instance) ((instance) == TSADC)
#define IS_SARADC_INSTANCE(instance) ((instance) == SARADC)
#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_USBPHY_GRF_INSTANCE(instance) ((instance) == USBPHY_GRF)
#define IS_GMAC_INSTANCE(instance) ((instance) == GMAC)
#define IS_MMC_INSTANCE(instance) ((instance) == MMC)
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1) || ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || ((instance) == UART5))
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO0) || ((instance) == GPIO1) || ((instance) == GPIO2) || ((instance) == GPIO3))
#define IS_I2C_INSTANCE(instance) (((instance) == I2C0) || ((instance) == I2C1) || ((instance) == I2C2) || ((instance) == I2C3))
#define IS_SPI_INSTANCE(instance) (((instance) == SPI0) || ((instance) == SPI1))
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || ((instance) == TIMER2) || ((instance) == TIMER3) || ((instance) == TIMER4) || ((instance) == TIMER5))
#define IS_DMA_INSTANCE(instance) ((instance) == DMA0)
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************PMU*******************************************/
/* PMU_WAKEUP_CFG0_LO */
#define PMU_PMU_WAKEUP_CFG0_LO_OFFSET                      (0x0U)
#define PMU_PMU_WAKEUP_CFG0_LO_WAKEUP_GPIO_POS_EN_LO_SHIFT (0U)
#define PMU_PMU_WAKEUP_CFG0_LO_WAKEUP_GPIO_POS_EN_LO_MASK  (0xFFFFU << PMU_PMU_WAKEUP_CFG0_LO_WAKEUP_GPIO_POS_EN_LO_SHIFT) /* 0x0000FFFF */
/* PMU_WAKEUP_CFG0_HI */
#define PMU_PMU_WAKEUP_CFG0_HI_OFFSET                      (0x4U)
#define PMU_PMU_WAKEUP_CFG0_HI_WAKEUP_GPIO_POS_EN_HI_SHIFT (0U)
#define PMU_PMU_WAKEUP_CFG0_HI_WAKEUP_GPIO_POS_EN_HI_MASK  (0xFFFFU << PMU_PMU_WAKEUP_CFG0_HI_WAKEUP_GPIO_POS_EN_HI_SHIFT) /* 0x0000FFFF */
/* PMU_WAKEUP_CFG1_LO */
#define PMU_PMU_WAKEUP_CFG1_LO_OFFSET                      (0x8U)
#define PMU_PMU_WAKEUP_CFG1_LO_WAKEUP_GPIO_NEG_EN_LO_SHIFT (0U)
#define PMU_PMU_WAKEUP_CFG1_LO_WAKEUP_GPIO_NEG_EN_LO_MASK  (0xFFFFU << PMU_PMU_WAKEUP_CFG1_LO_WAKEUP_GPIO_NEG_EN_LO_SHIFT) /* 0x0000FFFF */
/* PMU_WAKEUP_CFG1_HI */
#define PMU_PMU_WAKEUP_CFG1_HI_OFFSET                      (0xCU)
#define PMU_PMU_WAKEUP_CFG1_HI_WAKEUP_GPIO_NEG_EN_HI_SHIFT (0U)
#define PMU_PMU_WAKEUP_CFG1_HI_WAKEUP_GPIO_NEG_EN_HI_MASK  (0xFFFFU << PMU_PMU_WAKEUP_CFG1_HI_WAKEUP_GPIO_NEG_EN_HI_SHIFT) /* 0x0000FFFF */
/* PMU_WAKEUP_CFG2_LO */
#define PMU_PMU_WAKEUP_CFG2_LO_OFFSET                      (0x10U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_INT_CLUSTER_EN_SHIFT (0U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_INT_CLUSTER_EN_MASK  (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_INT_CLUSTER_EN_SHIFT) /* 0x00000001 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_GPIO0_INT_EN_SHIFT   (2U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_GPIO0_INT_EN_MASK    (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_GPIO0_INT_EN_SHIFT)   /* 0x00000004 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SDIO_EN_SHIFT        (3U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SDIO_EN_MASK         (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SDIO_EN_SHIFT)        /* 0x00000008 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SDMMC_EN_SHIFT       (4U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SDMMC_EN_MASK        (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SDMMC_EN_SHIFT)       /* 0x00000010 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_UART0_EN_SHIFT       (5U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_UART0_EN_MASK        (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_UART0_EN_SHIFT)       /* 0x00000020 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_TIMER_EN_SHIFT       (6U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_TIMER_EN_MASK        (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_TIMER_EN_SHIFT)       /* 0x00000040 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_USBDEV_EN_SHIFT      (7U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_USBDEV_EN_MASK       (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_USBDEV_EN_SHIFT)      /* 0x00000080 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SFT_EN_SHIFT         (8U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SFT_EN_MASK          (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_SFT_EN_SHIFT)         /* 0x00000100 */
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_TIMEOUT_EN_SHIFT     (10U)
#define PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_TIMEOUT_EN_MASK      (0x1U << PMU_PMU_WAKEUP_CFG2_LO_WAKEUP_TIMEOUT_EN_SHIFT)     /* 0x00000400 */
/* PMU_PWRDN_CON_LO */
#define PMU_PMU_PWRDN_CON_LO_OFFSET                        (0x18U)
#define PMU_PMU_PWRDN_CON_LO_PD_A35_0_PWRDWN_EN_SHIFT      (0U)
#define PMU_PMU_PWRDN_CON_LO_PD_A35_0_PWRDWN_EN_MASK       (0x1U << PMU_PMU_PWRDN_CON_LO_PD_A35_0_PWRDWN_EN_SHIFT)      /* 0x00000001 */
#define PMU_PMU_PWRDN_CON_LO_PD_A35_1_PWRDWN_EN_SHIFT      (1U)
#define PMU_PMU_PWRDN_CON_LO_PD_A35_1_PWRDWN_EN_MASK       (0x1U << PMU_PMU_PWRDN_CON_LO_PD_A35_1_PWRDWN_EN_SHIFT)      /* 0x00000002 */
#define PMU_PMU_PWRDN_CON_LO_PD_A35_2_PWRDWN_EN_SHIFT      (2U)
#define PMU_PMU_PWRDN_CON_LO_PD_A35_2_PWRDWN_EN_MASK       (0x1U << PMU_PMU_PWRDN_CON_LO_PD_A35_2_PWRDWN_EN_SHIFT)      /* 0x00000004 */
#define PMU_PMU_PWRDN_CON_LO_PD_A35_3_PWRDWN_EN_SHIFT      (3U)
#define PMU_PMU_PWRDN_CON_LO_PD_A35_3_PWRDWN_EN_MASK       (0x1U << PMU_PMU_PWRDN_CON_LO_PD_A35_3_PWRDWN_EN_SHIFT)      /* 0x00000008 */
#define PMU_PMU_PWRDN_CON_LO_PD_SCU_PWRDWN_EN_SHIFT        (4U)
#define PMU_PMU_PWRDN_CON_LO_PD_SCU_PWRDWN_EN_MASK         (0x1U << PMU_PMU_PWRDN_CON_LO_PD_SCU_PWRDWN_EN_SHIFT)        /* 0x00000010 */
#define PMU_PMU_PWRDN_CON_LO_PD_USB_PWRDWN_EN_SHIFT        (5U)
#define PMU_PMU_PWRDN_CON_LO_PD_USB_PWRDWN_EN_MASK         (0x1U << PMU_PMU_PWRDN_CON_LO_PD_USB_PWRDWN_EN_SHIFT)        /* 0x00000020 */
#define PMU_PMU_PWRDN_CON_LO_PD_DDR_PWRDWN_EN_SHIFT        (6U)
#define PMU_PMU_PWRDN_CON_LO_PD_DDR_PWRDWN_EN_MASK         (0x1U << PMU_PMU_PWRDN_CON_LO_PD_DDR_PWRDWN_EN_SHIFT)        /* 0x00000040 */
#define PMU_PMU_PWRDN_CON_LO_PD_NANDC_PWRDWN_EN_SHIFT      (8U)
#define PMU_PMU_PWRDN_CON_LO_PD_NANDC_PWRDWN_EN_MASK       (0x1U << PMU_PMU_PWRDN_CON_LO_PD_NANDC_PWRDWN_EN_SHIFT)      /* 0x00000100 */
#define PMU_PMU_PWRDN_CON_LO_PD_CRYPTO_PWRDWN_EN_SHIFT     (9U)
#define PMU_PMU_PWRDN_CON_LO_PD_CRYPTO_PWRDWN_EN_MASK      (0x1U << PMU_PMU_PWRDN_CON_LO_PD_CRYPTO_PWRDWN_EN_SHIFT)     /* 0x00000200 */
#define PMU_PMU_PWRDN_CON_LO_PD_GMAC_PWRDWN_EN_SHIFT       (10U)
#define PMU_PMU_PWRDN_CON_LO_PD_GMAC_PWRDWN_EN_MASK        (0x1U << PMU_PMU_PWRDN_CON_LO_PD_GMAC_PWRDWN_EN_SHIFT)       /* 0x00000400 */
#define PMU_PMU_PWRDN_CON_LO_PD_MMC_PWRDWN_EN_SHIFT        (11U)
#define PMU_PMU_PWRDN_CON_LO_PD_MMC_PWRDWN_EN_MASK         (0x1U << PMU_PMU_PWRDN_CON_LO_PD_MMC_PWRDWN_EN_SHIFT)        /* 0x00000800 */
#define PMU_PMU_PWRDN_CON_LO_PD_VPU_PWRDWN_EN_SHIFT        (12U)
#define PMU_PMU_PWRDN_CON_LO_PD_VPU_PWRDWN_EN_MASK         (0x1U << PMU_PMU_PWRDN_CON_LO_PD_VPU_PWRDWN_EN_SHIFT)        /* 0x00001000 */
#define PMU_PMU_PWRDN_CON_LO_PD_VO_PWRDWN_EN_SHIFT         (13U)
#define PMU_PMU_PWRDN_CON_LO_PD_VO_PWRDWN_EN_MASK          (0x1U << PMU_PMU_PWRDN_CON_LO_PD_VO_PWRDWN_EN_SHIFT)         /* 0x00002000 */
#define PMU_PMU_PWRDN_CON_LO_PD_VI_PWRDWN_EN_SHIFT         (14U)
#define PMU_PMU_PWRDN_CON_LO_PD_VI_PWRDWN_EN_MASK          (0x1U << PMU_PMU_PWRDN_CON_LO_PD_VI_PWRDWN_EN_SHIFT)         /* 0x00004000 */
#define PMU_PMU_PWRDN_CON_LO_PD_GPU_PWRDWN_EN_SHIFT        (15U)
#define PMU_PMU_PWRDN_CON_LO_PD_GPU_PWRDWN_EN_MASK         (0x1U << PMU_PMU_PWRDN_CON_LO_PD_GPU_PWRDWN_EN_SHIFT)        /* 0x00008000 */
/* PMU_PWRDN_ST */
#define PMU_PMU_PWRDN_ST_OFFSET                            (0x20U)
#define PMU_PMU_PWRDN_ST_PD_A35_0_PWR_STATUS_SHIFT         (0U)
#define PMU_PMU_PWRDN_ST_PD_A35_0_PWR_STATUS_MASK          (0x1U << PMU_PMU_PWRDN_ST_PD_A35_0_PWR_STATUS_SHIFT)         /* 0x00000001 */
#define PMU_PMU_PWRDN_ST_PD_A35_1_PWR_STATUS_SHIFT         (1U)
#define PMU_PMU_PWRDN_ST_PD_A35_1_PWR_STATUS_MASK          (0x1U << PMU_PMU_PWRDN_ST_PD_A35_1_PWR_STATUS_SHIFT)         /* 0x00000002 */
#define PMU_PMU_PWRDN_ST_PD_A35_2_PWR_STATUS_SHIFT         (2U)
#define PMU_PMU_PWRDN_ST_PD_A35_2_PWR_STATUS_MASK          (0x1U << PMU_PMU_PWRDN_ST_PD_A35_2_PWR_STATUS_SHIFT)         /* 0x00000004 */
#define PMU_PMU_PWRDN_ST_PD_A35_3_PWR_STATUS_SHIFT         (3U)
#define PMU_PMU_PWRDN_ST_PD_A35_3_PWR_STATUS_MASK          (0x1U << PMU_PMU_PWRDN_ST_PD_A35_3_PWR_STATUS_SHIFT)         /* 0x00000008 */
#define PMU_PMU_PWRDN_ST_PD_SCU_PWR_STATUS_SHIFT           (4U)
#define PMU_PMU_PWRDN_ST_PD_SCU_PWR_STATUS_MASK            (0x1U << PMU_PMU_PWRDN_ST_PD_SCU_PWR_STATUS_SHIFT)           /* 0x00000010 */
#define PMU_PMU_PWRDN_ST_PD_USB_PWR_STATUS_SHIFT           (5U)
#define PMU_PMU_PWRDN_ST_PD_USB_PWR_STATUS_MASK            (0x1U << PMU_PMU_PWRDN_ST_PD_USB_PWR_STATUS_SHIFT)           /* 0x00000020 */
#define PMU_PMU_PWRDN_ST_PD_DDR_PWR_STATUS_SHIFT           (6U)
#define PMU_PMU_PWRDN_ST_PD_DDR_PWR_STATUS_MASK            (0x1U << PMU_PMU_PWRDN_ST_PD_DDR_PWR_STATUS_SHIFT)           /* 0x00000040 */
#define PMU_PMU_PWRDN_ST_PD_NANDC_PWR_STATUS_SHIFT         (8U)
#define PMU_PMU_PWRDN_ST_PD_NANDC_PWR_STATUS_MASK          (0x1U << PMU_PMU_PWRDN_ST_PD_NANDC_PWR_STATUS_SHIFT)         /* 0x00000100 */
#define PMU_PMU_PWRDN_ST_PD_CRYPTO_PWR_STATUS_SHIFT        (9U)
#define PMU_PMU_PWRDN_ST_PD_CRYPTO_PWR_STATUS_MASK         (0x1U << PMU_PMU_PWRDN_ST_PD_CRYPTO_PWR_STATUS_SHIFT)        /* 0x00000200 */
#define PMU_PMU_PWRDN_ST_PD_GMAC_PWR_STATUS_SHIFT          (10U)
#define PMU_PMU_PWRDN_ST_PD_GMAC_PWR_STATUS_MASK           (0x1U << PMU_PMU_PWRDN_ST_PD_GMAC_PWR_STATUS_SHIFT)          /* 0x00000400 */
#define PMU_PMU_PWRDN_ST_PD_MMC_PWR_STATUS_SHIFT           (11U)
#define PMU_PMU_PWRDN_ST_PD_MMC_PWR_STATUS_MASK            (0x1U << PMU_PMU_PWRDN_ST_PD_MMC_PWR_STATUS_SHIFT)           /* 0x00000800 */
#define PMU_PMU_PWRDN_ST_PD_VPU_PWR_STATUS_SHIFT           (12U)
#define PMU_PMU_PWRDN_ST_PD_VPU_PWR_STATUS_MASK            (0x1U << PMU_PMU_PWRDN_ST_PD_VPU_PWR_STATUS_SHIFT)           /* 0x00001000 */
#define PMU_PMU_PWRDN_ST_PD_VO_PWR_STATUS_SHIFT            (13U)
#define PMU_PMU_PWRDN_ST_PD_VO_PWR_STATUS_MASK             (0x1U << PMU_PMU_PWRDN_ST_PD_VO_PWR_STATUS_SHIFT)            /* 0x00002000 */
#define PMU_PMU_PWRDN_ST_PD_VI_PWR_STATUS_SHIFT            (14U)
#define PMU_PMU_PWRDN_ST_PD_VI_PWR_STATUS_MASK             (0x1U << PMU_PMU_PWRDN_ST_PD_VI_PWR_STATUS_SHIFT)            /* 0x00004000 */
#define PMU_PMU_PWRDN_ST_PD_GPU_PWR_STATUS_SHIFT           (15U)
#define PMU_PMU_PWRDN_ST_PD_GPU_PWR_STATUS_MASK            (0x1U << PMU_PMU_PWRDN_ST_PD_GPU_PWR_STATUS_SHIFT)           /* 0x00008000 */
/* PMU_PWRMODE_CORE_CON_LO */
#define PMU_PMU_PWRMODE_CORE_CON_LO_OFFSET                 (0x24U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_GLOBAL_INT_DISABLE_CFG_SHIFT (0U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_GLOBAL_INT_DISABLE_CFG_MASK (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_GLOBAL_INT_DISABLE_CFG_SHIFT) /* 0x00000001 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLK_CORE_SRC_GATE_EN_SHIFT (1U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLK_CORE_SRC_GATE_EN_MASK (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_CLK_CORE_SRC_GATE_EN_SHIFT) /* 0x00000002 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_CPU0_PD_EN_SHIFT       (3U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_CPU0_PD_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_CPU0_PD_EN_SHIFT)       /* 0x00000008 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLR_CORE_SHIFT         (5U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLR_CORE_MASK          (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_CLR_CORE_SHIFT)         /* 0x00000020 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_SCU_PD_EN_SHIFT        (6U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_SCU_PD_EN_MASK         (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_SCU_PD_EN_SHIFT)        /* 0x00000040 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_L2_IDLE_EN_SHIFT       (8U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_L2_IDLE_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_L2_IDLE_EN_SHIFT)       /* 0x00000100 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_L2_FLUSH_EN_SHIFT      (9U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_L2_FLUSH_EN_MASK       (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_L2_FLUSH_EN_SHIFT)      /* 0x00000200 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLR_BUS2MAIN_SHIFT     (10U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLR_BUS2MAIN_MASK      (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_CLR_BUS2MAIN_SHIFT)     /* 0x00000400 */
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLR_PERI2MSCH_SHIFT    (11U)
#define PMU_PMU_PWRMODE_CORE_CON_LO_CLR_PERI2MSCH_MASK     (0x1U << PMU_PMU_PWRMODE_CORE_CON_LO_CLR_PERI2MSCH_SHIFT)    /* 0x00000800 */
/* PMU_PWRMODE_CORE_CON_HI */
#define PMU_PMU_PWRMODE_CORE_CON_HI_OFFSET                 (0x28U)
#define PMU_PMU_PWRMODE_CORE_CON_HI_APLL_PD_EN_SHIFT       (3U)
#define PMU_PMU_PWRMODE_CORE_CON_HI_APLL_PD_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_HI_APLL_PD_EN_SHIFT)       /* 0x00000008 */
#define PMU_PMU_PWRMODE_CORE_CON_HI_DPLL_PD_EN_SHIFT       (4U)
#define PMU_PMU_PWRMODE_CORE_CON_HI_DPLL_PD_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_HI_DPLL_PD_EN_SHIFT)       /* 0x00000010 */
#define PMU_PMU_PWRMODE_CORE_CON_HI_CPLL_PD_EN_SHIFT       (5U)
#define PMU_PMU_PWRMODE_CORE_CON_HI_CPLL_PD_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_HI_CPLL_PD_EN_SHIFT)       /* 0x00000020 */
#define PMU_PMU_PWRMODE_CORE_CON_HI_GPLL_PD_EN_SHIFT       (6U)
#define PMU_PMU_PWRMODE_CORE_CON_HI_GPLL_PD_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_HI_GPLL_PD_EN_SHIFT)       /* 0x00000040 */
#define PMU_PMU_PWRMODE_CORE_CON_HI_NPLL_PD_EN_SHIFT       (7U)
#define PMU_PMU_PWRMODE_CORE_CON_HI_NPLL_PD_EN_MASK        (0x1U << PMU_PMU_PWRMODE_CORE_CON_HI_NPLL_PD_EN_SHIFT)       /* 0x00000080 */
/* PMU_PWRMODE_COMMON_CON_LO */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_OFFSET               (0x2CU)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_POWER_MODE_EN_SHIFT  (0U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_POWER_MODE_EN_MASK   (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_POWER_MODE_EN_SHIFT)  /* 0x00000001 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_PD_EN_SHIFT      (1U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_PD_EN_MASK       (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_PD_EN_SHIFT)      /* 0x00000002 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_WAKEUP_RESET_EN_SHIFT (3U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_WAKEUP_RESET_EN_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_WAKEUP_RESET_EN_SHIFT) /* 0x00000008 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_PLL_PD_EN_SHIFT      (4U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_PLL_PD_EN_MASK       (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_PLL_PD_EN_SHIFT)      /* 0x00000010 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_PMU_USE_LF_SHIFT     (6U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_PMU_USE_LF_MASK      (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_PMU_USE_LF_SHIFT)     /* 0x00000040 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_ALIVE_USE_LF_SHIFT   (7U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_ALIVE_USE_LF_MASK    (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_ALIVE_USE_LF_SHIFT)   /* 0x00000080 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_OSC_24M_DIS_SHIFT    (8U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_OSC_24M_DIS_MASK     (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_OSC_24M_DIS_SHIFT)    /* 0x00000100 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_INPUT_CLAMP_EN_SHIFT (9U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_INPUT_CLAMP_EN_MASK  (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_INPUT_CLAMP_EN_SHIFT) /* 0x00000200 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_SREF_ENTER_EN_SHIFT  (10U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_SREF_ENTER_EN_MASK   (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_SREF_ENTER_EN_SHIFT)  /* 0x00000400 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDRC_GATING_EN_SHIFT (11U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDRC_GATING_EN_MASK  (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_DDRC_GATING_EN_SHIFT) /* 0x00000800 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_RET_EN_SHIFT     (12U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_RET_EN_MASK      (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_RET_EN_SHIFT)     /* 0x00001000 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_RET_DE_REQ_SHIFT (13U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_RET_DE_REQ_MASK  (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_DDR_RET_DE_REQ_SHIFT) /* 0x00002000 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_CLR_PMU_SHIFT        (14U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_CLR_PMU_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_CLR_PMU_SHIFT)        /* 0x00004000 */
#define PMU_PMU_PWRMODE_COMMON_CON_LO_CLR_PERI_PMU_SHIFT   (15U)
#define PMU_PMU_PWRMODE_COMMON_CON_LO_CLR_PERI_PMU_MASK    (0x1U << PMU_PMU_PWRMODE_COMMON_CON_LO_CLR_PERI_PMU_SHIFT)   /* 0x00008000 */
/* PMU_PWRMODE_COMMON_CON_HI */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_OFFSET               (0x30U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_BUS_SHIFT        (0U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_BUS_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_BUS_SHIFT)        /* 0x00000001 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_MMC_SHIFT        (1U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_MMC_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_MMC_SHIFT)        /* 0x00000002 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_MSCH_SHIFT       (2U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_MSCH_MASK        (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_MSCH_SHIFT)       /* 0x00000004 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_NANDC_SHIFT      (3U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_NANDC_MASK       (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_NANDC_SHIFT)      /* 0x00000008 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_GMAC_SHIFT       (4U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_GMAC_MASK        (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_GMAC_SHIFT)       /* 0x00000010 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VO_SHIFT         (5U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VO_MASK          (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VO_SHIFT)         /* 0x00000020 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VI_SHIFT         (6U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VI_MASK          (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VI_SHIFT)         /* 0x00000040 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_GPU_SHIFT        (7U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_GPU_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_GPU_SHIFT)        /* 0x00000080 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_USB_SHIFT        (8U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_USB_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_USB_SHIFT)        /* 0x00000100 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VPU_SHIFT        (9U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VPU_MASK         (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_VPU_SHIFT)        /* 0x00000200 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_CRYPTO_SHIFT     (10U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_CRYPTO_MASK      (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_CLR_CRYPTO_SHIFT)     /* 0x00000400 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_WAIT_WAKEUP_BEGIN_CFG_SHIFT (11U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_WAIT_WAKEUP_BEGIN_CFG_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_WAIT_WAKEUP_BEGIN_CFG_SHIFT) /* 0x00000800 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_PD_PERI_CLK_SRC_GATE_EN_SHIFT (12U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_PD_PERI_CLK_SRC_GATE_EN_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_PD_PERI_CLK_SRC_GATE_EN_SHIFT) /* 0x00001000 */
#define PMU_PMU_PWRMODE_COMMON_CON_HI_PD_BUS_CLK_SRC_GATE_EN_SHIFT (13U)
#define PMU_PMU_PWRMODE_COMMON_CON_HI_PD_BUS_CLK_SRC_GATE_EN_MASK (0x1U << PMU_PMU_PWRMODE_COMMON_CON_HI_PD_BUS_CLK_SRC_GATE_EN_SHIFT) /* 0x00002000 */
/* PMU_SFT_CON_LO */
#define PMU_PMU_SFT_CON_LO_OFFSET                          (0x34U)
#define PMU_PMU_SFT_CON_LO_WAKEUP_SFT_SHIFT                (0U)
#define PMU_PMU_SFT_CON_LO_WAKEUP_SFT_MASK                 (0x1U << PMU_PMU_SFT_CON_LO_WAKEUP_SFT_SHIFT)                /* 0x00000001 */
#define PMU_PMU_SFT_CON_LO_NPLL_PD_CFG_SHIFT               (1U)
#define PMU_PMU_SFT_CON_LO_NPLL_PD_CFG_MASK                (0x1U << PMU_PMU_SFT_CON_LO_NPLL_PD_CFG_SHIFT)               /* 0x00000002 */
#define PMU_PMU_SFT_CON_LO_APLL_PD_CFG_SHIFT               (2U)
#define PMU_PMU_SFT_CON_LO_APLL_PD_CFG_MASK                (0x1U << PMU_PMU_SFT_CON_LO_APLL_PD_CFG_SHIFT)               /* 0x00000004 */
#define PMU_PMU_SFT_CON_LO_DPLL_PD_CFG_SHIFT               (3U)
#define PMU_PMU_SFT_CON_LO_DPLL_PD_CFG_MASK                (0x1U << PMU_PMU_SFT_CON_LO_DPLL_PD_CFG_SHIFT)               /* 0x00000008 */
#define PMU_PMU_SFT_CON_LO_CPLL_PD_CFG_SHIFT               (4U)
#define PMU_PMU_SFT_CON_LO_CPLL_PD_CFG_MASK                (0x1U << PMU_PMU_SFT_CON_LO_CPLL_PD_CFG_SHIFT)               /* 0x00000010 */
#define PMU_PMU_SFT_CON_LO_GPLL_PD_CFG_SHIFT               (5U)
#define PMU_PMU_SFT_CON_LO_GPLL_PD_CFG_MASK                (0x1U << PMU_PMU_SFT_CON_LO_GPLL_PD_CFG_SHIFT)               /* 0x00000020 */
#define PMU_PMU_SFT_CON_LO_L2FLUSHREQ_CLUSTER_CFG_SHIFT    (6U)
#define PMU_PMU_SFT_CON_LO_L2FLUSHREQ_CLUSTER_CFG_MASK     (0x1U << PMU_PMU_SFT_CON_LO_L2FLUSHREQ_CLUSTER_CFG_SHIFT)    /* 0x00000040 */
#define PMU_PMU_SFT_CON_LO_UPCTL_C_SYSREQ_CFG_SHIFT        (10U)
#define PMU_PMU_SFT_CON_LO_UPCTL_C_SYSREQ_CFG_MASK         (0x1U << PMU_PMU_SFT_CON_LO_UPCTL_C_SYSREQ_CFG_SHIFT)        /* 0x00000400 */
/* PMU_BUS_IDLE_REQ_LO */
#define PMU_PMU_BUS_IDLE_REQ_LO_OFFSET                     (0x64U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_BUS_CFG_SHIFT     (0U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_BUS_CFG_MASK      (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_BUS_CFG_SHIFT)     /* 0x00000001 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_BUS2MAIN_CFG_SHIFT (1U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_BUS2MAIN_CFG_MASK (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_BUS2MAIN_CFG_SHIFT) /* 0x00000002 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_GPU_CFG_SHIFT     (2U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_GPU_CFG_MASK      (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_GPU_CFG_SHIFT)     /* 0x00000004 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_CORE_CFG_SHIFT    (3U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_CORE_CFG_MASK     (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_CORE_CFG_SHIFT)    /* 0x00000008 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_CRYPTO_CFG_SHIFT  (4U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_CRYPTO_CFG_MASK   (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_CRYPTO_CFG_SHIFT)  /* 0x00000010 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_MMC_NAND_CFG_SHIFT (5U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_MMC_NAND_CFG_MASK (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_MMC_NAND_CFG_SHIFT) /* 0x00000020 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_GMAC_CFG_SHIFT    (6U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_GMAC_CFG_MASK     (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_GMAC_CFG_SHIFT)    /* 0x00000040 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VO_CFG_SHIFT      (7U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VO_CFG_MASK       (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VO_CFG_SHIFT)      /* 0x00000080 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VI_CFG_SHIFT      (8U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VI_CFG_MASK       (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VI_CFG_SHIFT)      /* 0x00000100 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_NANDC_CFG_SHIFT   (9U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_NANDC_CFG_MASK    (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_NANDC_CFG_SHIFT)   /* 0x00000200 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_USB_CFG_SHIFT     (10U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_USB_CFG_MASK      (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_USB_CFG_SHIFT)     /* 0x00000400 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_MSCH_CFG_SHIFT    (11U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_MSCH_CFG_MASK     (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_MSCH_CFG_SHIFT)    /* 0x00000800 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PERI_MID_CFG_SHIFT (12U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PERI_MID_CFG_MASK (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PERI_MID_CFG_SHIFT) /* 0x00001000 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PMU_CFG_SHIFT     (13U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PMU_CFG_MASK      (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PMU_CFG_SHIFT)     /* 0x00002000 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VPU_CFG_SHIFT     (14U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VPU_CFG_MASK      (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_VPU_CFG_SHIFT)     /* 0x00004000 */
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PERI2MSCH_CFG_SHIFT (15U)
#define PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PERI2MSCH_CFG_MASK (0x1U << PMU_PMU_BUS_IDLE_REQ_LO_IDLE_REQ_PERI2MSCH_CFG_SHIFT) /* 0x00008000 */
/* PMU_BUS_IDLE_ST */
#define PMU_PMU_BUS_IDLE_ST_OFFSET                         (0x6CU)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS_SHIFT             (0U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS_SHIFT)             /* 0x00000001 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS2MAIN_SHIFT        (1U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS2MAIN_MASK         (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_BUS2MAIN_SHIFT)        /* 0x00000002 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_GPU_SHIFT             (2U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_GPU_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_GPU_SHIFT)             /* 0x00000004 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CORE_SHIFT            (3U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CORE_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CORE_SHIFT)            /* 0x00000008 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CRYPTO_SHIFT          (4U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CRYPTO_MASK           (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_CRYPTO_SHIFT)          /* 0x00000010 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MMC_NAND_SHIFT        (5U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MMC_NAND_MASK         (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MMC_NAND_SHIFT)        /* 0x00000020 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_GMAC_SHIFT            (6U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_GMAC_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_GMAC_SHIFT)            /* 0x00000040 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VO_SHIFT              (7U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VO_MASK               (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VO_SHIFT)              /* 0x00000080 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VI_SHIFT              (8U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VI_MASK               (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VI_SHIFT)              /* 0x00000100 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_SDCARD_SHIFT          (9U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_SDCARD_MASK           (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_SDCARD_SHIFT)          /* 0x00000200 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_USB_SHIFT             (10U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_USB_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_USB_SHIFT)             /* 0x00000400 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MSCH_SHIFT            (11U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MSCH_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_MSCH_SHIFT)            /* 0x00000800 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI_MID_SHIFT        (12U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI_MID_MASK         (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI_MID_SHIFT)        /* 0x00001000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PMU_SHIFT             (13U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PMU_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PMU_SHIFT)             /* 0x00002000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VPU_SHIFT             (14U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VPU_MASK              (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_VPU_SHIFT)             /* 0x00004000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI2MSCH_SHIFT       (15U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI2MSCH_MASK        (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_ACK_PERI2MSCH_SHIFT)       /* 0x00008000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_BUS_SHIFT                 (16U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_BUS_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_BUS_SHIFT)                 /* 0x00010000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_BUS2MAIN_SHIFT            (17U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_BUS2MAIN_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_BUS2MAIN_SHIFT)            /* 0x00020000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_GPU_SHIFT                 (18U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_GPU_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_GPU_SHIFT)                 /* 0x00040000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_CORE_SHIFT                (19U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_CORE_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_CORE_SHIFT)                /* 0x00080000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_CRYPTO_SHIFT              (20U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_CRYPTO_MASK               (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_CRYPTO_SHIFT)              /* 0x00100000 */
#define PMU_PMU_BUS_IDLE_ST_MMC_NAND_IDLE_SHIFT            (21U)
#define PMU_PMU_BUS_IDLE_ST_MMC_NAND_IDLE_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_MMC_NAND_IDLE_SHIFT)            /* 0x00200000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_GMAC_SHIFT                (22U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_GMAC_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_GMAC_SHIFT)                /* 0x00400000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_VO_SHIFT                  (23U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_VO_MASK                   (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_VO_SHIFT)                  /* 0x00800000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_VI_SHIFT                  (24U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_VI_MASK                   (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_VI_SHIFT)                  /* 0x01000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_SDCARD_SHIFT              (25U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_SDCARD_MASK               (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_SDCARD_SHIFT)              /* 0x02000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_USB_SHIFT                 (26U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_USB_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_USB_SHIFT)                 /* 0x04000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_MSCH_SHIFT                (27U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_MSCH_MASK                 (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_MSCH_SHIFT)                /* 0x08000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_PERI_MID_SHIFT            (28U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_PERI_MID_MASK             (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_PERI_MID_SHIFT)            /* 0x10000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_PMU_SHIFT                 (29U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_PMU_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_PMU_SHIFT)                 /* 0x20000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_VPU_SHIFT                 (30U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_VPU_MASK                  (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_VPU_SHIFT)                 /* 0x40000000 */
#define PMU_PMU_BUS_IDLE_ST_IDLE_PERI2MSCH_SHIFT           (31U)
#define PMU_PMU_BUS_IDLE_ST_IDLE_PERI2MSCH_MASK            (0x1U << PMU_PMU_BUS_IDLE_ST_IDLE_PERI2MSCH_SHIFT)           /* 0x80000000 */
/* PMU_OSC_CNT_LO */
#define PMU_PMU_OSC_CNT_LO_OFFSET                          (0x74U)
#define PMU_PMU_OSC_CNT_LO_PMU_OSC_CNT_LO_SHIFT            (0U)
#define PMU_PMU_OSC_CNT_LO_PMU_OSC_CNT_LO_MASK             (0xFFFFU << PMU_PMU_OSC_CNT_LO_PMU_OSC_CNT_LO_SHIFT)         /* 0x0000FFFF */
/* PMU_OSC_CNT_HI */
#define PMU_PMU_OSC_CNT_HI_OFFSET                          (0x78U)
#define PMU_PMU_OSC_CNT_HI_PMU_OSC_CNT_HI_SHIFT            (0U)
#define PMU_PMU_OSC_CNT_HI_PMU_OSC_CNT_HI_MASK             (0xFU << PMU_PMU_OSC_CNT_HI_PMU_OSC_CNT_HI_SHIFT)            /* 0x0000000F */
/* PMU_PLLLOCK_CNT_LO */
#define PMU_PMU_PLLLOCK_CNT_LO_OFFSET                      (0x7CU)
#define PMU_PMU_PLLLOCK_CNT_LO_PMU_PLLLOCK_CNT_LO_SHIFT    (0U)
#define PMU_PMU_PLLLOCK_CNT_LO_PMU_PLLLOCK_CNT_LO_MASK     (0xFFFFU << PMU_PMU_PLLLOCK_CNT_LO_PMU_PLLLOCK_CNT_LO_SHIFT) /* 0x0000FFFF */
/* PMU_PLLLOCK_CNT_HI */
#define PMU_PMU_PLLLOCK_CNT_HI_OFFSET                      (0x80U)
#define PMU_PMU_PLLLOCK_CNT_HI_PMU_PLLLOCK_CNT_HI_SHIFT    (0U)
#define PMU_PMU_PLLLOCK_CNT_HI_PMU_PLLLOCK_CNT_HI_MASK     (0xFU << PMU_PMU_PLLLOCK_CNT_HI_PMU_PLLLOCK_CNT_HI_SHIFT)    /* 0x0000000F */
/* PMU_PLLRST_CNT_LO */
#define PMU_PMU_PLLRST_CNT_LO_OFFSET                       (0x84U)
#define PMU_PMU_PLLRST_CNT_LO_PMU_PLLRST_CNT_LO_SHIFT      (0U)
#define PMU_PMU_PLLRST_CNT_LO_PMU_PLLRST_CNT_LO_MASK       (0xFFFFU << PMU_PMU_PLLRST_CNT_LO_PMU_PLLRST_CNT_LO_SHIFT)   /* 0x0000FFFF */
/* PMU_PLLRST_CNT_HI */
#define PMU_PMU_PLLRST_CNT_HI_OFFSET                       (0x88U)
#define PMU_PMU_PLLRST_CNT_HI_PMU_PLLRST_CNT_HI_SHIFT      (0U)
#define PMU_PMU_PLLRST_CNT_HI_PMU_PLLRST_CNT_HI_MASK       (0xFU << PMU_PMU_PLLRST_CNT_HI_PMU_PLLRST_CNT_HI_SHIFT)      /* 0x0000000F */
/* PMU_STABLE_CNT_LO */
#define PMU_PMU_STABLE_CNT_LO_OFFSET                       (0x8CU)
#define PMU_PMU_STABLE_CNT_LO_PMU_STABLE_CNT_LO_SHIFT      (0U)
#define PMU_PMU_STABLE_CNT_LO_PMU_STABLE_CNT_LO_MASK       (0xFFFFU << PMU_PMU_STABLE_CNT_LO_PMU_STABLE_CNT_LO_SHIFT)   /* 0x0000FFFF */
/* PMU_STABLE_CNT_HI */
#define PMU_PMU_STABLE_CNT_HI_OFFSET                       (0x90U)
#define PMU_PMU_STABLE_CNT_HI_PMU_STABLE_CNT_HI_SHIFT      (0U)
#define PMU_PMU_STABLE_CNT_HI_PMU_STABLE_CNT_HI_MASK       (0xFU << PMU_PMU_STABLE_CNT_HI_PMU_STABLE_CNT_HI_SHIFT)      /* 0x0000000F */
/* PMU_WAKEUP_RST_CLR_CNT_HI */
#define PMU_PMU_WAKEUP_RST_CLR_CNT_HI_OFFSET               (0x98U)
#define PMU_PMU_WAKEUP_RST_CLR_CNT_HI_PMU_WAKEUP_RST_CNT_HI_SHIFT (0U)
#define PMU_PMU_WAKEUP_RST_CLR_CNT_HI_PMU_WAKEUP_RST_CNT_HI_MASK (0xFU << PMU_PMU_WAKEUP_RST_CLR_CNT_HI_PMU_WAKEUP_RST_CNT_HI_SHIFT) /* 0x0000000F */
/* PMU_WAKEUP_RST_CLR_CNT_LO */
#define PMU_PMU_WAKEUP_RST_CLR_CNT_LO_OFFSET               (0xA0U)
#define PMU_PMU_WAKEUP_RST_CLR_CNT_LO_PMU_WAKEUP_RST_CNT_LO_SHIFT (0U)
#define PMU_PMU_WAKEUP_RST_CLR_CNT_LO_PMU_WAKEUP_RST_CNT_LO_MASK (0xFFFFU << PMU_PMU_WAKEUP_RST_CLR_CNT_LO_PMU_WAKEUP_RST_CNT_LO_SHIFT) /* 0x0000FFFF */
/* PMU_DDR_SREF_ST */
#define PMU_PMU_DDR_SREF_ST_OFFSET                         (0xA4U)
#define PMU_PMU_DDR_SREF_ST                                (0x0U)
#define PMU_PMU_DDR_SREF_ST_UPCTL_C_ACTIVE_SHIFT           (0U)
#define PMU_PMU_DDR_SREF_ST_UPCTL_C_ACTIVE_MASK            (0x1U << PMU_PMU_DDR_SREF_ST_UPCTL_C_ACTIVE_SHIFT)           /* 0x00000001 */
#define PMU_PMU_DDR_SREF_ST_UPCTL_C_SYSACK_SHIFT           (1U)
#define PMU_PMU_DDR_SREF_ST_UPCTL_C_SYSACK_MASK            (0x1U << PMU_PMU_DDR_SREF_ST_UPCTL_C_SYSACK_SHIFT)           /* 0x00000002 */
/* PMU_SYS_REG0_LO */
#define PMU_PMU_SYS_REG0_LO_OFFSET                         (0xA8U)
#define PMU_PMU_SYS_REG0_LO_PMU_SYS_REG0_LO_SHIFT          (0U)
#define PMU_PMU_SYS_REG0_LO_PMU_SYS_REG0_LO_MASK           (0xFFFFU << PMU_PMU_SYS_REG0_LO_PMU_SYS_REG0_LO_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG0_HI */
#define PMU_PMU_SYS_REG0_HI_OFFSET                         (0xACU)
#define PMU_PMU_SYS_REG0_HI_PMU_SYS_REG0_HI_SHIFT          (0U)
#define PMU_PMU_SYS_REG0_HI_PMU_SYS_REG0_HI_MASK           (0xFFFFU << PMU_PMU_SYS_REG0_HI_PMU_SYS_REG0_HI_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG1_LO */
#define PMU_PMU_SYS_REG1_LO_OFFSET                         (0xB0U)
#define PMU_PMU_SYS_REG1_LO_PMU_SYS_REG1_LO_SHIFT          (0U)
#define PMU_PMU_SYS_REG1_LO_PMU_SYS_REG1_LO_MASK           (0xFFFFU << PMU_PMU_SYS_REG1_LO_PMU_SYS_REG1_LO_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG1_HI */
#define PMU_PMU_SYS_REG1_HI_OFFSET                         (0xB4U)
#define PMU_PMU_SYS_REG1_HI_PMU_SYS_REG1_HI_SHIFT          (0U)
#define PMU_PMU_SYS_REG1_HI_PMU_SYS_REG1_HI_MASK           (0xFFFFU << PMU_PMU_SYS_REG1_HI_PMU_SYS_REG1_HI_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG2_LO */
#define PMU_PMU_SYS_REG2_LO_OFFSET                         (0xB8U)
#define PMU_PMU_SYS_REG2_LO_PMU_SYS_REG2_LO_SHIFT          (0U)
#define PMU_PMU_SYS_REG2_LO_PMU_SYS_REG2_LO_MASK           (0xFFFFU << PMU_PMU_SYS_REG2_LO_PMU_SYS_REG2_LO_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG2_HI */
#define PMU_PMU_SYS_REG2_HI_OFFSET                         (0xBCU)
#define PMU_PMU_SYS_REG2_HI_PMU_SYS_REG2_HI_SHIFT          (0U)
#define PMU_PMU_SYS_REG2_HI_PMU_SYS_REG2_HI_MASK           (0xFFFFU << PMU_PMU_SYS_REG2_HI_PMU_SYS_REG2_HI_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG3_LO */
#define PMU_PMU_SYS_REG3_LO_OFFSET                         (0xC0U)
#define PMU_PMU_SYS_REG3_LO_PMU_SYS_REG3_LO_SHIFT          (0U)
#define PMU_PMU_SYS_REG3_LO_PMU_SYS_REG3_LO_MASK           (0xFFFFU << PMU_PMU_SYS_REG3_LO_PMU_SYS_REG3_LO_SHIFT)       /* 0x0000FFFF */
/* PMU_SYS_REG3_HI */
#define PMU_PMU_SYS_REG3_HI_OFFSET                         (0xC4U)
#define PMU_PMU_SYS_REG3_HI_PMU_SYS_REG3_HI_SHIFT          (0U)
#define PMU_PMU_SYS_REG3_HI_PMU_SYS_REG3_HI_MASK           (0xFFFFU << PMU_PMU_SYS_REG3_HI_PMU_SYS_REG3_HI_SHIFT)       /* 0x0000FFFF */
/* PMU_SCU_PWRDN_CNT_LO */
#define PMU_PMU_SCU_PWRDN_CNT_LO_OFFSET                    (0xC8U)
#define PMU_PMU_SCU_PWRDN_CNT_LO_PMU_SCU_PWRDN_CNT_LO_SHIFT (0U)
#define PMU_PMU_SCU_PWRDN_CNT_LO_PMU_SCU_PWRDN_CNT_LO_MASK (0xFFFFU << PMU_PMU_SCU_PWRDN_CNT_LO_PMU_SCU_PWRDN_CNT_LO_SHIFT) /* 0x0000FFFF */
/* PMU_SCU_PWRDN_CNT_HI */
#define PMU_PMU_SCU_PWRDN_CNT_HI_OFFSET                    (0xCCU)
#define PMU_PMU_SCU_PWRDN_CNT_HI_PMU_SCU_PWRDN_CNT_HI_SHIFT (0U)
#define PMU_PMU_SCU_PWRDN_CNT_HI_PMU_SCU_PWRDN_CNT_HI_MASK (0xFU << PMU_PMU_SCU_PWRDN_CNT_HI_PMU_SCU_PWRDN_CNT_HI_SHIFT) /* 0x0000000F */
/* PMU_SCU_PWRUP_CNT_LO */
#define PMU_PMU_SCU_PWRUP_CNT_LO_OFFSET                    (0xD0U)
#define PMU_PMU_SCU_PWRUP_CNT_LO_PMU_SCU_PWRDN_CNT_LO_SHIFT (0U)
#define PMU_PMU_SCU_PWRUP_CNT_LO_PMU_SCU_PWRDN_CNT_LO_MASK (0xFFFFU << PMU_PMU_SCU_PWRUP_CNT_LO_PMU_SCU_PWRDN_CNT_LO_SHIFT) /* 0x0000FFFF */
/* PMU_SCU_PWRUP_CNT_HI */
#define PMU_PMU_SCU_PWRUP_CNT_HI_OFFSET                    (0xD4U)
#define PMU_PMU_SCU_PWRUP_CNT_HI_PMU_SCU_PWRDN_CNT_HI_SHIFT (0U)
#define PMU_PMU_SCU_PWRUP_CNT_HI_PMU_SCU_PWRDN_CNT_HI_MASK (0xFU << PMU_PMU_SCU_PWRUP_CNT_HI_PMU_SCU_PWRDN_CNT_HI_SHIFT) /* 0x0000000F */
/* PMU_TIMEOUT_CNT_LO */
#define PMU_PMU_TIMEOUT_CNT_LO_OFFSET                      (0xD8U)
#define PMU_PMU_TIMEOUT_CNT_LO_PMU_TIMEOUT_CNT_LO_SHIFT    (0U)
#define PMU_PMU_TIMEOUT_CNT_LO_PMU_TIMEOUT_CNT_LO_MASK     (0xFFFFU << PMU_PMU_TIMEOUT_CNT_LO_PMU_TIMEOUT_CNT_LO_SHIFT) /* 0x0000FFFF */
/* PMU_TIMEOUT_CNT_HI */
#define PMU_PMU_TIMEOUT_CNT_HI_OFFSET                      (0xDCU)
#define PMU_PMU_TIMEOUT_CNT_HI_PMU_TIMEOUT_CNT_HI_SHIFT    (0U)
#define PMU_PMU_TIMEOUT_CNT_HI_PMU_TIMEOUT_CNT_HI_MASK     (0xFU << PMU_PMU_TIMEOUT_CNT_HI_PMU_TIMEOUT_CNT_HI_SHIFT)    /* 0x0000000F */
/* PMU_CPU0APM_CON */
#define PMU_PMU_CPU0APM_CON_OFFSET                         (0xE0U)
#define PMU_PMU_CPU0APM_CON_CPU0_WFI_PWRDN_EN_SHIFT        (0U)
#define PMU_PMU_CPU0APM_CON_CPU0_WFI_PWRDN_EN_MASK         (0x1U << PMU_PMU_CPU0APM_CON_CPU0_WFI_PWRDN_EN_SHIFT)        /* 0x00000001 */
#define PMU_PMU_CPU0APM_CON_CPU0_INT_WAKEUP_EN_SHIFT       (1U)
#define PMU_PMU_CPU0APM_CON_CPU0_INT_WAKEUP_EN_MASK        (0x1U << PMU_PMU_CPU0APM_CON_CPU0_INT_WAKEUP_EN_SHIFT)       /* 0x00000002 */
#define PMU_PMU_CPU0APM_CON_GLOBAL_INT_DISABLE0_CFG_SHIFT  (2U)
#define PMU_PMU_CPU0APM_CON_GLOBAL_INT_DISABLE0_CFG_MASK   (0x1U << PMU_PMU_CPU0APM_CON_GLOBAL_INT_DISABLE0_CFG_SHIFT)  /* 0x00000004 */
#define PMU_PMU_CPU0APM_CON_CPU0_SFT_WAKEUP_SHIFT          (3U)
#define PMU_PMU_CPU0APM_CON_CPU0_SFT_WAKEUP_MASK           (0x1U << PMU_PMU_CPU0APM_CON_CPU0_SFT_WAKEUP_SHIFT)          /* 0x00000008 */
/* PMU_CPU1APM_CON */
#define PMU_PMU_CPU1APM_CON_OFFSET                         (0xE4U)
#define PMU_PMU_CPU1APM_CON_CPU1_WFI_PWRDN_EN_SHIFT        (0U)
#define PMU_PMU_CPU1APM_CON_CPU1_WFI_PWRDN_EN_MASK         (0x1U << PMU_PMU_CPU1APM_CON_CPU1_WFI_PWRDN_EN_SHIFT)        /* 0x00000001 */
#define PMU_PMU_CPU1APM_CON_CPU1_INT_WAKEUP_EN_SHIFT       (1U)
#define PMU_PMU_CPU1APM_CON_CPU1_INT_WAKEUP_EN_MASK        (0x1U << PMU_PMU_CPU1APM_CON_CPU1_INT_WAKEUP_EN_SHIFT)       /* 0x00000002 */
#define PMU_PMU_CPU1APM_CON_GLOBAL_INT_DISABLE1_CFG_SHIFT  (2U)
#define PMU_PMU_CPU1APM_CON_GLOBAL_INT_DISABLE1_CFG_MASK   (0x1U << PMU_PMU_CPU1APM_CON_GLOBAL_INT_DISABLE1_CFG_SHIFT)  /* 0x00000004 */
#define PMU_PMU_CPU1APM_CON_CPU1_SFT_WAKEUP_SHIFT          (3U)
#define PMU_PMU_CPU1APM_CON_CPU1_SFT_WAKEUP_MASK           (0x1U << PMU_PMU_CPU1APM_CON_CPU1_SFT_WAKEUP_SHIFT)          /* 0x00000008 */
/* PMU_CPU2APM_CON */
#define PMU_PMU_CPU2APM_CON_OFFSET                         (0xE8U)
#define PMU_PMU_CPU2APM_CON_CPU2_WFI_PWRDN_EN_SHIFT        (0U)
#define PMU_PMU_CPU2APM_CON_CPU2_WFI_PWRDN_EN_MASK         (0x1U << PMU_PMU_CPU2APM_CON_CPU2_WFI_PWRDN_EN_SHIFT)        /* 0x00000001 */
#define PMU_PMU_CPU2APM_CON_CPU2_INT_WAKEUP_EN_SHIFT       (1U)
#define PMU_PMU_CPU2APM_CON_CPU2_INT_WAKEUP_EN_MASK        (0x1U << PMU_PMU_CPU2APM_CON_CPU2_INT_WAKEUP_EN_SHIFT)       /* 0x00000002 */
#define PMU_PMU_CPU2APM_CON_GLOBAL_INT_DISABLE2_CFG_SHIFT  (2U)
#define PMU_PMU_CPU2APM_CON_GLOBAL_INT_DISABLE2_CFG_MASK   (0x1U << PMU_PMU_CPU2APM_CON_GLOBAL_INT_DISABLE2_CFG_SHIFT)  /* 0x00000004 */
#define PMU_PMU_CPU2APM_CON_CPU2_SFT_WAKEUP_SHIFT          (3U)
#define PMU_PMU_CPU2APM_CON_CPU2_SFT_WAKEUP_MASK           (0x1U << PMU_PMU_CPU2APM_CON_CPU2_SFT_WAKEUP_SHIFT)          /* 0x00000008 */
/* PMU_CPU3APM_CON */
#define PMU_PMU_CPU3APM_CON_OFFSET                         (0xECU)
#define PMU_PMU_CPU3APM_CON_CPU3_WFI_PWRDN_EN_SHIFT        (0U)
#define PMU_PMU_CPU3APM_CON_CPU3_WFI_PWRDN_EN_MASK         (0x1U << PMU_PMU_CPU3APM_CON_CPU3_WFI_PWRDN_EN_SHIFT)        /* 0x00000001 */
#define PMU_PMU_CPU3APM_CON_CPU3_INT_WAKEUP_EN_SHIFT       (1U)
#define PMU_PMU_CPU3APM_CON_CPU3_INT_WAKEUP_EN_MASK        (0x1U << PMU_PMU_CPU3APM_CON_CPU3_INT_WAKEUP_EN_SHIFT)       /* 0x00000002 */
#define PMU_PMU_CPU3APM_CON_GLOBAL_INT_DISABLE3_CFG_SHIFT  (2U)
#define PMU_PMU_CPU3APM_CON_GLOBAL_INT_DISABLE3_CFG_MASK   (0x1U << PMU_PMU_CPU3APM_CON_GLOBAL_INT_DISABLE3_CFG_SHIFT)  /* 0x00000004 */
#define PMU_PMU_CPU3APM_CON_CPU3_SFT_WAKEUP_SHIFT          (3U)
#define PMU_PMU_CPU3APM_CON_CPU3_SFT_WAKEUP_MASK           (0x1U << PMU_PMU_CPU3APM_CON_CPU3_SFT_WAKEUP_SHIFT)          /* 0x00000008 */
/****************************************PMU_GRF*****************************************/
/* GPIO0A_IOMUX */
#define PMU_GRF_GPIO0A_IOMUX_OFFSET                        (0x0U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A0_SEL_SHIFT             (0U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A0_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A0_SEL_SHIFT)             /* 0x00000003 */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT             (2U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A1_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT)             /* 0x0000000C */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT             (4U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A2_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT)             /* 0x00000030 */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT             (6U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A3_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT)             /* 0x000000C0 */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT             (8U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A4_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT)             /* 0x00000300 */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT             (10U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A5_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT)             /* 0x00000C00 */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT             (12U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A6_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT)             /* 0x00003000 */
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT             (14U)
#define PMU_GRF_GPIO0A_IOMUX_GPIO0A7_SEL_MASK              (0x3U << PMU_GRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT)             /* 0x0000C000 */
/* GPIO0B_IOMUX */
#define PMU_GRF_GPIO0B_IOMUX_OFFSET                        (0x4U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT             (0U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B0_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT)             /* 0x00000003 */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B1_SEL_SHIFT             (2U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B1_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B1_SEL_SHIFT)             /* 0x0000000C */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B2_SEL_SHIFT             (4U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B2_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B2_SEL_SHIFT)             /* 0x00000030 */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B3_SEL_SHIFT             (6U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B3_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B3_SEL_SHIFT)             /* 0x000000C0 */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B4_SEL_SHIFT             (8U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B4_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B4_SEL_SHIFT)             /* 0x00000300 */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B5_SEL_SHIFT             (10U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B5_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B5_SEL_SHIFT)             /* 0x00000C00 */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B6_SEL_SHIFT             (12U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B6_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B6_SEL_SHIFT)             /* 0x00003000 */
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B7_SEL_SHIFT             (14U)
#define PMU_GRF_GPIO0B_IOMUX_GPIO0B7_SEL_MASK              (0x3U << PMU_GRF_GPIO0B_IOMUX_GPIO0B7_SEL_SHIFT)             /* 0x0000C000 */
/* GPIO0C_IOMUX */
#define PMU_GRF_GPIO0C_IOMUX_OFFSET                        (0x8U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C0_SEL_SHIFT             (0U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C0_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C0_SEL_SHIFT)             /* 0x00000003 */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C1_SEL_SHIFT             (2U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C1_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C1_SEL_SHIFT)             /* 0x0000000C */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C2_SEL_SHIFT             (4U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C2_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C2_SEL_SHIFT)             /* 0x00000030 */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C3_SEL_SHIFT             (6U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C3_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C3_SEL_SHIFT)             /* 0x000000C0 */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C4_SEL_SHIFT             (8U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C4_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C4_SEL_SHIFT)             /* 0x00000300 */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C5_SEL_SHIFT             (10U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C5_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C5_SEL_SHIFT)             /* 0x00000C00 */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C6_SEL_SHIFT             (12U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C6_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C6_SEL_SHIFT)             /* 0x00003000 */
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C7_SEL_SHIFT             (14U)
#define PMU_GRF_GPIO0C_IOMUX_GPIO0C7_SEL_MASK              (0x3U << PMU_GRF_GPIO0C_IOMUX_GPIO0C7_SEL_SHIFT)             /* 0x0000C000 */
/* GPIO0A_P */
#define PMU_GRF_GPIO0A_P_OFFSET                            (0x10U)
#define PMU_GRF_GPIO0A_P_GPIO0A0_P_SHIFT                   (0U)
#define PMU_GRF_GPIO0A_P_GPIO0A0_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A0_P_SHIFT)                   /* 0x00000003 */
#define PMU_GRF_GPIO0A_P_GPIO0A1_P_SHIFT                   (2U)
#define PMU_GRF_GPIO0A_P_GPIO0A1_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A1_P_SHIFT)                   /* 0x0000000C */
#define PMU_GRF_GPIO0A_P_GPIO0A2_P_SHIFT                   (4U)
#define PMU_GRF_GPIO0A_P_GPIO0A2_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A2_P_SHIFT)                   /* 0x00000030 */
#define PMU_GRF_GPIO0A_P_GPIO0A3_P_SHIFT                   (6U)
#define PMU_GRF_GPIO0A_P_GPIO0A3_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A3_P_SHIFT)                   /* 0x000000C0 */
#define PMU_GRF_GPIO0A_P_GPIO0A4_P_SHIFT                   (8U)
#define PMU_GRF_GPIO0A_P_GPIO0A4_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A4_P_SHIFT)                   /* 0x00000300 */
#define PMU_GRF_GPIO0A_P_GPIO0A5_P_SHIFT                   (10U)
#define PMU_GRF_GPIO0A_P_GPIO0A5_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A5_P_SHIFT)                   /* 0x00000C00 */
#define PMU_GRF_GPIO0A_P_GPIO0A6_P_SHIFT                   (12U)
#define PMU_GRF_GPIO0A_P_GPIO0A6_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A6_P_SHIFT)                   /* 0x00003000 */
#define PMU_GRF_GPIO0A_P_GPIO0A7_P_SHIFT                   (14U)
#define PMU_GRF_GPIO0A_P_GPIO0A7_P_MASK                    (0x3U << PMU_GRF_GPIO0A_P_GPIO0A7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO0B_P */
#define PMU_GRF_GPIO0B_P_OFFSET                            (0x14U)
#define PMU_GRF_GPIO0B_P_GPIO0B0_P_SHIFT                   (0U)
#define PMU_GRF_GPIO0B_P_GPIO0B0_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B0_P_SHIFT)                   /* 0x00000003 */
#define PMU_GRF_GPIO0B_P_GPIO0B1_P_SHIFT                   (2U)
#define PMU_GRF_GPIO0B_P_GPIO0B1_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B1_P_SHIFT)                   /* 0x0000000C */
#define PMU_GRF_GPIO0B_P_GPIO0B2_P_SHIFT                   (4U)
#define PMU_GRF_GPIO0B_P_GPIO0B2_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B2_P_SHIFT)                   /* 0x00000030 */
#define PMU_GRF_GPIO0B_P_GPIO0B3_P_SHIFT                   (6U)
#define PMU_GRF_GPIO0B_P_GPIO0B3_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B3_P_SHIFT)                   /* 0x000000C0 */
#define PMU_GRF_GPIO0B_P_GPIO0B4_P_SHIFT                   (8U)
#define PMU_GRF_GPIO0B_P_GPIO0B4_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B4_P_SHIFT)                   /* 0x00000300 */
#define PMU_GRF_GPIO0B_P_GPIO0B5_P_SHIFT                   (10U)
#define PMU_GRF_GPIO0B_P_GPIO0B5_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B5_P_SHIFT)                   /* 0x00000C00 */
#define PMU_GRF_GPIO0B_P_GPIO0B6_P_SHIFT                   (12U)
#define PMU_GRF_GPIO0B_P_GPIO0B6_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B6_P_SHIFT)                   /* 0x00003000 */
#define PMU_GRF_GPIO0B_P_GPIO0B7_P_SHIFT                   (14U)
#define PMU_GRF_GPIO0B_P_GPIO0B7_P_MASK                    (0x3U << PMU_GRF_GPIO0B_P_GPIO0B7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO0C_P */
#define PMU_GRF_GPIO0C_P_OFFSET                            (0x18U)
#define PMU_GRF_GPIO0C_P_GPIO0C0_P_SHIFT                   (0U)
#define PMU_GRF_GPIO0C_P_GPIO0C0_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C0_P_SHIFT)                   /* 0x00000003 */
#define PMU_GRF_GPIO0C_P_GPIO0C1_P_SHIFT                   (2U)
#define PMU_GRF_GPIO0C_P_GPIO0C1_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C1_P_SHIFT)                   /* 0x0000000C */
#define PMU_GRF_GPIO0C_P_GPIO0C2_P_SHIFT                   (4U)
#define PMU_GRF_GPIO0C_P_GPIO0C2_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C2_P_SHIFT)                   /* 0x00000030 */
#define PMU_GRF_GPIO0C_P_GPIO0C3_P_SHIFT                   (6U)
#define PMU_GRF_GPIO0C_P_GPIO0C3_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C3_P_SHIFT)                   /* 0x000000C0 */
#define PMU_GRF_GPIO0C_P_GPIO0C4_P_SHIFT                   (8U)
#define PMU_GRF_GPIO0C_P_GPIO0C4_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C4_P_SHIFT)                   /* 0x00000300 */
#define PMU_GRF_GPIO0C_P_GPIO0C5_P_SHIFT                   (10U)
#define PMU_GRF_GPIO0C_P_GPIO0C5_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C5_P_SHIFT)                   /* 0x00000C00 */
#define PMU_GRF_GPIO0C_P_GPIO0C6_P_SHIFT                   (12U)
#define PMU_GRF_GPIO0C_P_GPIO0C6_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C6_P_SHIFT)                   /* 0x00003000 */
#define PMU_GRF_GPIO0C_P_GPIO0C7_P_SHIFT                   (14U)
#define PMU_GRF_GPIO0C_P_GPIO0C7_P_MASK                    (0x3U << PMU_GRF_GPIO0C_P_GPIO0C7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO0A_E */
#define PMU_GRF_GPIO0A_E_OFFSET                            (0x20U)
#define PMU_GRF_GPIO0A_E_GPIO0A0_E_SHIFT                   (0U)
#define PMU_GRF_GPIO0A_E_GPIO0A0_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A0_E_SHIFT)                   /* 0x00000003 */
#define PMU_GRF_GPIO0A_E_GPIO0A1_E_SHIFT                   (2U)
#define PMU_GRF_GPIO0A_E_GPIO0A1_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A1_E_SHIFT)                   /* 0x0000000C */
#define PMU_GRF_GPIO0A_E_GPIO0A2_E_SHIFT                   (4U)
#define PMU_GRF_GPIO0A_E_GPIO0A2_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A2_E_SHIFT)                   /* 0x00000030 */
#define PMU_GRF_GPIO0A_E_GPIO0A3_E_SHIFT                   (6U)
#define PMU_GRF_GPIO0A_E_GPIO0A3_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A3_E_SHIFT)                   /* 0x000000C0 */
#define PMU_GRF_GPIO0A_E_GPIO0A4_E_SHIFT                   (8U)
#define PMU_GRF_GPIO0A_E_GPIO0A4_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A4_E_SHIFT)                   /* 0x00000300 */
#define PMU_GRF_GPIO0A_E_GPIO0A5_E_SHIFT                   (10U)
#define PMU_GRF_GPIO0A_E_GPIO0A5_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A5_E_SHIFT)                   /* 0x00000C00 */
#define PMU_GRF_GPIO0A_E_GPIO0A6_E_SHIFT                   (12U)
#define PMU_GRF_GPIO0A_E_GPIO0A6_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A6_E_SHIFT)                   /* 0x00003000 */
#define PMU_GRF_GPIO0A_E_GPIO0A7_E_SHIFT                   (14U)
#define PMU_GRF_GPIO0A_E_GPIO0A7_E_MASK                    (0x3U << PMU_GRF_GPIO0A_E_GPIO0A7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO0B_E */
#define PMU_GRF_GPIO0B_E_OFFSET                            (0x24U)
#define PMU_GRF_GPIO0B_E_GPIO0B0_E_SHIFT                   (0U)
#define PMU_GRF_GPIO0B_E_GPIO0B0_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B0_E_SHIFT)                   /* 0x00000003 */
#define PMU_GRF_GPIO0B_E_GPIO0B1_E_SHIFT                   (2U)
#define PMU_GRF_GPIO0B_E_GPIO0B1_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B1_E_SHIFT)                   /* 0x0000000C */
#define PMU_GRF_GPIO0B_E_GPIO0B2_E_SHIFT                   (4U)
#define PMU_GRF_GPIO0B_E_GPIO0B2_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B2_E_SHIFT)                   /* 0x00000030 */
#define PMU_GRF_GPIO0B_E_GPIO0B3_E_SHIFT                   (6U)
#define PMU_GRF_GPIO0B_E_GPIO0B3_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B3_E_SHIFT)                   /* 0x000000C0 */
#define PMU_GRF_GPIO0B_E_GPIO0B4_E_SHIFT                   (8U)
#define PMU_GRF_GPIO0B_E_GPIO0B4_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B4_E_SHIFT)                   /* 0x00000300 */
#define PMU_GRF_GPIO0B_E_GPIO0B5_E_SHIFT                   (10U)
#define PMU_GRF_GPIO0B_E_GPIO0B5_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B5_E_SHIFT)                   /* 0x00000C00 */
#define PMU_GRF_GPIO0B_E_GPIO0B6_E_SHIFT                   (12U)
#define PMU_GRF_GPIO0B_E_GPIO0B6_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B6_E_SHIFT)                   /* 0x00003000 */
#define PMU_GRF_GPIO0B_E_GPIO0B7_E_SHIFT                   (14U)
#define PMU_GRF_GPIO0B_E_GPIO0B7_E_MASK                    (0x3U << PMU_GRF_GPIO0B_E_GPIO0B7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO0C_E */
#define PMU_GRF_GPIO0C_E_OFFSET                            (0x28U)
#define PMU_GRF_GPIO0C_E_GPIO0C0_E_SHIFT                   (0U)
#define PMU_GRF_GPIO0C_E_GPIO0C0_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C0_E_SHIFT)                   /* 0x00000003 */
#define PMU_GRF_GPIO0C_E_GPIO0C1_E_SHIFT                   (2U)
#define PMU_GRF_GPIO0C_E_GPIO0C1_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C1_E_SHIFT)                   /* 0x0000000C */
#define PMU_GRF_GPIO0C_E_GPIO0C2_E_SHIFT                   (4U)
#define PMU_GRF_GPIO0C_E_GPIO0C2_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C2_E_SHIFT)                   /* 0x00000030 */
#define PMU_GRF_GPIO0C_E_GPIO0C3_E_SHIFT                   (6U)
#define PMU_GRF_GPIO0C_E_GPIO0C3_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C3_E_SHIFT)                   /* 0x000000C0 */
#define PMU_GRF_GPIO0C_E_GPIO0C4_E_SHIFT                   (8U)
#define PMU_GRF_GPIO0C_E_GPIO0C4_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C4_E_SHIFT)                   /* 0x00000300 */
#define PMU_GRF_GPIO0C_E_GPIO0C5_E_SHIFT                   (10U)
#define PMU_GRF_GPIO0C_E_GPIO0C5_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C5_E_SHIFT)                   /* 0x00000C00 */
#define PMU_GRF_GPIO0C_E_GPIO0C6_E_SHIFT                   (12U)
#define PMU_GRF_GPIO0C_E_GPIO0C6_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C6_E_SHIFT)                   /* 0x00003000 */
#define PMU_GRF_GPIO0C_E_GPIO0C7_E_SHIFT                   (14U)
#define PMU_GRF_GPIO0C_E_GPIO0C7_E_MASK                    (0x3U << PMU_GRF_GPIO0C_E_GPIO0C7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO0L_SR */
#define PMU_GRF_GPIO0L_SR_OFFSET                           (0x30U)
#define PMU_GRF_GPIO0L_SR_GPIO0A0_SR_SHIFT                 (0U)
#define PMU_GRF_GPIO0L_SR_GPIO0A0_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A0_SR_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_GPIO0L_SR_GPIO0A1_SR_SHIFT                 (1U)
#define PMU_GRF_GPIO0L_SR_GPIO0A1_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A1_SR_SHIFT)                 /* 0x00000002 */
#define PMU_GRF_GPIO0L_SR_GPIO0A2_SR_SHIFT                 (2U)
#define PMU_GRF_GPIO0L_SR_GPIO0A2_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A2_SR_SHIFT)                 /* 0x00000004 */
#define PMU_GRF_GPIO0L_SR_GPIO0A3_SR_SHIFT                 (3U)
#define PMU_GRF_GPIO0L_SR_GPIO0A3_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A3_SR_SHIFT)                 /* 0x00000008 */
#define PMU_GRF_GPIO0L_SR_GPIO0A4_SR_SHIFT                 (4U)
#define PMU_GRF_GPIO0L_SR_GPIO0A4_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A4_SR_SHIFT)                 /* 0x00000010 */
#define PMU_GRF_GPIO0L_SR_GPIO0A5_SR_SHIFT                 (5U)
#define PMU_GRF_GPIO0L_SR_GPIO0A5_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A5_SR_SHIFT)                 /* 0x00000020 */
#define PMU_GRF_GPIO0L_SR_GPIO0A6_SR_SHIFT                 (6U)
#define PMU_GRF_GPIO0L_SR_GPIO0A6_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A6_SR_SHIFT)                 /* 0x00000040 */
#define PMU_GRF_GPIO0L_SR_GPIO0A7_SR_SHIFT                 (7U)
#define PMU_GRF_GPIO0L_SR_GPIO0A7_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0A7_SR_SHIFT)                 /* 0x00000080 */
#define PMU_GRF_GPIO0L_SR_GPIO0B0_SR_SHIFT                 (8U)
#define PMU_GRF_GPIO0L_SR_GPIO0B0_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B0_SR_SHIFT)                 /* 0x00000100 */
#define PMU_GRF_GPIO0L_SR_GPIO0B1_SR_SHIFT                 (9U)
#define PMU_GRF_GPIO0L_SR_GPIO0B1_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B1_SR_SHIFT)                 /* 0x00000200 */
#define PMU_GRF_GPIO0L_SR_GPIO0B2_SR_SHIFT                 (10U)
#define PMU_GRF_GPIO0L_SR_GPIO0B2_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B2_SR_SHIFT)                 /* 0x00000400 */
#define PMU_GRF_GPIO0L_SR_GPIO0B3_SR_SHIFT                 (11U)
#define PMU_GRF_GPIO0L_SR_GPIO0B3_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B3_SR_SHIFT)                 /* 0x00000800 */
#define PMU_GRF_GPIO0L_SR_GPIO0B4_SR_SHIFT                 (12U)
#define PMU_GRF_GPIO0L_SR_GPIO0B4_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B4_SR_SHIFT)                 /* 0x00001000 */
#define PMU_GRF_GPIO0L_SR_GPIO0B5_SR_SHIFT                 (13U)
#define PMU_GRF_GPIO0L_SR_GPIO0B5_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B5_SR_SHIFT)                 /* 0x00002000 */
#define PMU_GRF_GPIO0L_SR_GPIO0B6_SR_SHIFT                 (14U)
#define PMU_GRF_GPIO0L_SR_GPIO0B6_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B6_SR_SHIFT)                 /* 0x00004000 */
#define PMU_GRF_GPIO0L_SR_GPIO0B7_SR_SHIFT                 (15U)
#define PMU_GRF_GPIO0L_SR_GPIO0B7_SR_MASK                  (0x1U << PMU_GRF_GPIO0L_SR_GPIO0B7_SR_SHIFT)                 /* 0x00008000 */
/* GPIO0H_SR */
#define PMU_GRF_GPIO0H_SR_OFFSET                           (0x34U)
#define PMU_GRF_GPIO0H_SR_GPIO0C0_SR_SHIFT                 (0U)
#define PMU_GRF_GPIO0H_SR_GPIO0C0_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C0_SR_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_GPIO0H_SR_GPIO0C1_SR_SHIFT                 (1U)
#define PMU_GRF_GPIO0H_SR_GPIO0C1_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C1_SR_SHIFT)                 /* 0x00000002 */
#define PMU_GRF_GPIO0H_SR_GPIO0C2_SR_SHIFT                 (2U)
#define PMU_GRF_GPIO0H_SR_GPIO0C2_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C2_SR_SHIFT)                 /* 0x00000004 */
#define PMU_GRF_GPIO0H_SR_GPIO0C3_SR_SHIFT                 (3U)
#define PMU_GRF_GPIO0H_SR_GPIO0C3_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C3_SR_SHIFT)                 /* 0x00000008 */
#define PMU_GRF_GPIO0H_SR_GPIO0C4_SR_SHIFT                 (4U)
#define PMU_GRF_GPIO0H_SR_GPIO0C4_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C4_SR_SHIFT)                 /* 0x00000010 */
#define PMU_GRF_GPIO0H_SR_GPIO0C5_SR_SHIFT                 (5U)
#define PMU_GRF_GPIO0H_SR_GPIO0C5_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C5_SR_SHIFT)                 /* 0x00000020 */
#define PMU_GRF_GPIO0H_SR_GPIO0C6_SR_SHIFT                 (6U)
#define PMU_GRF_GPIO0H_SR_GPIO0C6_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C6_SR_SHIFT)                 /* 0x00000040 */
#define PMU_GRF_GPIO0H_SR_GPIO0C7_SR_SHIFT                 (7U)
#define PMU_GRF_GPIO0H_SR_GPIO0C7_SR_MASK                  (0x1U << PMU_GRF_GPIO0H_SR_GPIO0C7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO0L_SMT */
#define PMU_GRF_GPIO0L_SMT_OFFSET                          (0x38U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A0_SMT_SHIFT               (0U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A0_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A0_SMT_SHIFT)               /* 0x00000001 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A1_SMT_SHIFT               (1U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A1_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A1_SMT_SHIFT)               /* 0x00000002 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A2_SMT_SHIFT               (2U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A2_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A2_SMT_SHIFT)               /* 0x00000004 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A3_SMT_SHIFT               (3U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A3_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A3_SMT_SHIFT)               /* 0x00000008 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A4_SMT_SHIFT               (4U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A4_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A4_SMT_SHIFT)               /* 0x00000010 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A5_SMT_SHIFT               (5U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A5_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A5_SMT_SHIFT)               /* 0x00000020 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A6_SMT_SHIFT               (6U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A6_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A6_SMT_SHIFT)               /* 0x00000040 */
#define PMU_GRF_GPIO0L_SMT_GPIO0A7_SMT_SHIFT               (7U)
#define PMU_GRF_GPIO0L_SMT_GPIO0A7_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0A7_SMT_SHIFT)               /* 0x00000080 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B0_SMT_SHIFT               (8U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B0_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B0_SMT_SHIFT)               /* 0x00000100 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B1_SMT_SHIFT               (9U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B1_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B1_SMT_SHIFT)               /* 0x00000200 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B2_SMT_SHIFT               (10U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B2_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B2_SMT_SHIFT)               /* 0x00000400 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B3_SMT_SHIFT               (11U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B3_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B3_SMT_SHIFT)               /* 0x00000800 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B4_SMT_SHIFT               (12U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B4_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B4_SMT_SHIFT)               /* 0x00001000 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B5_SMT_SHIFT               (13U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B5_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B5_SMT_SHIFT)               /* 0x00002000 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B6_SMT_SHIFT               (14U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B6_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B6_SMT_SHIFT)               /* 0x00004000 */
#define PMU_GRF_GPIO0L_SMT_GPIO0B7_SMT_SHIFT               (15U)
#define PMU_GRF_GPIO0L_SMT_GPIO0B7_SMT_MASK                (0x1U << PMU_GRF_GPIO0L_SMT_GPIO0B7_SMT_SHIFT)               /* 0x00008000 */
/* GPIO0H_SMT */
#define PMU_GRF_GPIO0H_SMT_OFFSET                          (0x3CU)
#define PMU_GRF_GPIO0H_SMT_GPIO0C0_SMT_SHIFT               (0U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C0_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C0_SMT_SHIFT)               /* 0x00000001 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C1_SMT_SHIFT               (1U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C1_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C1_SMT_SHIFT)               /* 0x00000002 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C2_SMT_SHIFT               (2U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C2_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C2_SMT_SHIFT)               /* 0x00000004 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C3_SMT_SHIFT               (3U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C3_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C3_SMT_SHIFT)               /* 0x00000008 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C4_SMT_SHIFT               (4U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C4_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C4_SMT_SHIFT)               /* 0x00000010 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C5_SMT_SHIFT               (5U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C5_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C5_SMT_SHIFT)               /* 0x00000020 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C6_SMT_SHIFT               (6U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C6_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C6_SMT_SHIFT)               /* 0x00000040 */
#define PMU_GRF_GPIO0H_SMT_GPIO0C7_SMT_SHIFT               (7U)
#define PMU_GRF_GPIO0H_SMT_GPIO0C7_SMT_MASK                (0x1U << PMU_GRF_GPIO0H_SMT_GPIO0C7_SMT_SHIFT)               /* 0x00000080 */
/* SOC_CON0 */
#define PMU_GRF_SOC_CON0_OFFSET                            (0x100U)
#define PMU_GRF_SOC_CON0_CON_32K_IOE_SHIFT                 (0U)
#define PMU_GRF_SOC_CON0_CON_32K_IOE_MASK                  (0x1U << PMU_GRF_SOC_CON0_CON_32K_IOE_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_SOC_CON0_UART0_RTS_SEL_SHIFT               (5U)
#define PMU_GRF_SOC_CON0_UART0_RTS_SEL_MASK                (0x1U << PMU_GRF_SOC_CON0_UART0_RTS_SEL_SHIFT)               /* 0x00000020 */
#define PMU_GRF_SOC_CON0_UART0_CTS_SEL_SHIFT               (6U)
#define PMU_GRF_SOC_CON0_UART0_CTS_SEL_MASK                (0x1U << PMU_GRF_SOC_CON0_UART0_CTS_SEL_SHIFT)               /* 0x00000040 */
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_SEL_SHIFT         (12U)
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_SEL_MASK          (0x1U << PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_SEL_SHIFT)         /* 0x00001000 */
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_SHIFT        (13U)
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_MASK         (0x1U << PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_SHIFT)        /* 0x00002000 */
#define PMU_GRF_SOC_CON0_POC_PMUIO1_SEL18_SHIFT            (14U)
#define PMU_GRF_SOC_CON0_POC_PMUIO1_SEL18_MASK             (0x1U << PMU_GRF_SOC_CON0_POC_PMUIO1_SEL18_SHIFT)            /* 0x00004000 */
#define PMU_GRF_SOC_CON0_POC_PMUIO2_SEL18_SHIFT            (15U)
#define PMU_GRF_SOC_CON0_POC_PMUIO2_SEL18_MASK             (0x1U << PMU_GRF_SOC_CON0_POC_PMUIO2_SEL18_SHIFT)            /* 0x00008000 */
/* SOC_CON1 */
#define PMU_GRF_SOC_CON1_OFFSET                            (0x104U)
#define PMU_GRF_SOC_CON1_RESETN_HOLD_SHIFT                 (0U)
#define PMU_GRF_SOC_CON1_RESETN_HOLD_MASK                  (0xFFFU << PMU_GRF_SOC_CON1_RESETN_HOLD_SHIFT)               /* 0x00000FFF */
#define PMU_GRF_SOC_CON1_HOLD_THE_DDRFAILSAFE_SHIFT        (12U)
#define PMU_GRF_SOC_CON1_HOLD_THE_DDRFAILSAFE_MASK         (0x1U << PMU_GRF_SOC_CON1_HOLD_THE_DDRFAILSAFE_SHIFT)        /* 0x00001000 */
/* SOC_CON2 */
#define PMU_GRF_SOC_CON2_OFFSET                            (0x108U)
#define PMU_GRF_SOC_CON2_NPOR_OUT2CHIP_PULSE_WIDTH_SHIFT   (0U)
#define PMU_GRF_SOC_CON2_NPOR_OUT2CHIP_PULSE_WIDTH_MASK    (0x1FFFU << PMU_GRF_SOC_CON2_NPOR_OUT2CHIP_PULSE_WIDTH_SHIFT) /* 0x00001FFF */
/* FAILSAFE_CON */
#define PMU_GRF_FAILSAFE_CON_OFFSET                        (0x10CU)
#define PMU_GRF_FAILSAFE_CON_TSADC_SHUT_RESET_TRIGGER_EN_SHIFT (0U)
#define PMU_GRF_FAILSAFE_CON_TSADC_SHUT_RESET_TRIGGER_EN_MASK (0x1U << PMU_GRF_FAILSAFE_CON_TSADC_SHUT_RESET_TRIGGER_EN_SHIFT) /* 0x00000001 */
#define PMU_GRF_FAILSAFE_CON_WDT_SHUT_RESET_TRIGGER_EN_SHIFT (1U)
#define PMU_GRF_FAILSAFE_CON_WDT_SHUT_RESET_TRIGGER_EN_MASK (0x1U << PMU_GRF_FAILSAFE_CON_WDT_SHUT_RESET_TRIGGER_EN_SHIFT) /* 0x00000002 */
#define PMU_GRF_FAILSAFE_CON_DDRIO_RET_EN_SHIFT            (2U)
#define PMU_GRF_FAILSAFE_CON_DDRIO_RET_EN_MASK             (0x1U << PMU_GRF_FAILSAFE_CON_DDRIO_RET_EN_SHIFT)            /* 0x00000004 */
#define PMU_GRF_FAILSAFE_CON_SREF_ENTER_EN_SHIFT           (3U)
#define PMU_GRF_FAILSAFE_CON_SREF_ENTER_EN_MASK            (0x1U << PMU_GRF_FAILSAFE_CON_SREF_ENTER_EN_SHIFT)           /* 0x00000008 */
#define PMU_GRF_FAILSAFE_CON_DDRC_GATING_EN_SHIFT          (4U)
#define PMU_GRF_FAILSAFE_CON_DDRC_GATING_EN_MASK           (0x1U << PMU_GRF_FAILSAFE_CON_DDRC_GATING_EN_SHIFT)          /* 0x00000010 */
#define PMU_GRF_FAILSAFE_CON_DDR_IO_RET_DE_REQ_SHIFT       (5U)
#define PMU_GRF_FAILSAFE_CON_DDR_IO_RET_DE_REQ_MASK        (0x1U << PMU_GRF_FAILSAFE_CON_DDR_IO_RET_DE_REQ_SHIFT)       /* 0x00000020 */
#define PMU_GRF_FAILSAFE_CON_DDR_IO_RET_CFG_SHIFT          (6U)
#define PMU_GRF_FAILSAFE_CON_DDR_IO_RET_CFG_MASK           (0x1U << PMU_GRF_FAILSAFE_CON_DDR_IO_RET_CFG_SHIFT)          /* 0x00000040 */
#define PMU_GRF_FAILSAFE_CON_UPCTL_C_SYSREQ_CFG_SHIFT      (8U)
#define PMU_GRF_FAILSAFE_CON_UPCTL_C_SYSREQ_CFG_MASK       (0x1U << PMU_GRF_FAILSAFE_CON_UPCTL_C_SYSREQ_CFG_SHIFT)      /* 0x00000100 */
/* PVTM_CON0 */
#define PMU_GRF_PVTM_CON0_OFFSET                           (0x180U)
#define PMU_GRF_PVTM_CON0_PVTM_PMU_START_SHIFT             (0U)
#define PMU_GRF_PVTM_CON0_PVTM_PMU_START_MASK              (0x1U << PMU_GRF_PVTM_CON0_PVTM_PMU_START_SHIFT)             /* 0x00000001 */
#define PMU_GRF_PVTM_CON0_PVTM_PMU_OSC_EN_SHIFT            (1U)
#define PMU_GRF_PVTM_CON0_PVTM_PMU_OSC_EN_MASK             (0x1U << PMU_GRF_PVTM_CON0_PVTM_PMU_OSC_EN_SHIFT)            /* 0x00000002 */
#define PMU_GRF_PVTM_CON0_PVTM_CLKOUT_DIV_SHIFT            (2U)
#define PMU_GRF_PVTM_CON0_PVTM_CLKOUT_DIV_MASK             (0x3FU << PMU_GRF_PVTM_CON0_PVTM_CLKOUT_DIV_SHIFT)           /* 0x000000FC */
/* PVTM_CON1 */
#define PMU_GRF_PVTM_CON1_OFFSET                           (0x184U)
#define PMU_GRF_PVTM_CON1_PVTM_PMU_CAL_CNT_SHIFT           (0U)
#define PMU_GRF_PVTM_CON1_PVTM_PMU_CAL_CNT_MASK            (0xFFFFFFFFU << PMU_GRF_PVTM_CON1_PVTM_PMU_CAL_CNT_SHIFT)    /* 0xFFFFFFFF */
/* PVTM_STATUS0 */
#define PMU_GRF_PVTM_STATUS0_OFFSET                        (0x190U)
#define PMU_GRF_PVTM_STATUS0_PVTM_PMU_FREQ_DONE_SHIFT      (0U)
#define PMU_GRF_PVTM_STATUS0_PVTM_PMU_FREQ_DONE_MASK       (0x1U << PMU_GRF_PVTM_STATUS0_PVTM_PMU_FREQ_DONE_SHIFT)      /* 0x00000001 */
/* PVTM_STATUS1 */
#define PMU_GRF_PVTM_STATUS1_OFFSET                        (0x194U)
#define PMU_GRF_PVTM_STATUS1_PVTM_PMU_FREQ_CNT_SHIFT       (0U)
#define PMU_GRF_PVTM_STATUS1_PVTM_PMU_FREQ_CNT_MASK        (0xFFFFFFFFU << PMU_GRF_PVTM_STATUS1_PVTM_PMU_FREQ_CNT_SHIFT) /* 0xFFFFFFFF */
/* OS_REG0 */
#define PMU_GRF_OS_REG0_OFFSET                             (0x200U)
#define PMU_GRF_OS_REG0_PMU_OS_REG0_SHIFT                  (0U)
#define PMU_GRF_OS_REG0_PMU_OS_REG0_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG0_PMU_OS_REG0_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG1 */
#define PMU_GRF_OS_REG1_OFFSET                             (0x204U)
#define PMU_GRF_OS_REG1_PMU_OS_REG1_SHIFT                  (0U)
#define PMU_GRF_OS_REG1_PMU_OS_REG1_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG1_PMU_OS_REG1_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG2 */
#define PMU_GRF_OS_REG2_OFFSET                             (0x208U)
#define PMU_GRF_OS_REG2_PMU_OS_REG2_SHIFT                  (0U)
#define PMU_GRF_OS_REG2_PMU_OS_REG2_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG2_PMU_OS_REG2_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG3 */
#define PMU_GRF_OS_REG3_OFFSET                             (0x20CU)
#define PMU_GRF_OS_REG3_PMU_OS_REG3_SHIFT                  (0U)
#define PMU_GRF_OS_REG3_PMU_OS_REG3_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG3_PMU_OS_REG3_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG4 */
#define PMU_GRF_OS_REG4_OFFSET                             (0x210U)
#define PMU_GRF_OS_REG4_PMU_OS_REG4_SHIFT                  (0U)
#define PMU_GRF_OS_REG4_PMU_OS_REG4_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG4_PMU_OS_REG4_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG5 */
#define PMU_GRF_OS_REG5_OFFSET                             (0x214U)
#define PMU_GRF_OS_REG5_PMU_OS_REG5_SHIFT                  (0U)
#define PMU_GRF_OS_REG5_PMU_OS_REG5_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG5_PMU_OS_REG5_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG6 */
#define PMU_GRF_OS_REG6_OFFSET                             (0x218U)
#define PMU_GRF_OS_REG6_PMU_OS_REG6_SHIFT                  (0U)
#define PMU_GRF_OS_REG6_PMU_OS_REG6_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG6_PMU_OS_REG6_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG7 */
#define PMU_GRF_OS_REG7_OFFSET                             (0x21CU)
#define PMU_GRF_OS_REG7_PMU_OS_REG7_SHIFT                  (0U)
#define PMU_GRF_OS_REG7_PMU_OS_REG7_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG7_PMU_OS_REG7_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG8 */
#define PMU_GRF_OS_REG8_OFFSET                             (0x220U)
#define PMU_GRF_OS_REG8_PMU_OS_REG8_SHIFT                  (0U)
#define PMU_GRF_OS_REG8_PMU_OS_REG8_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG8_PMU_OS_REG8_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG9 */
#define PMU_GRF_OS_REG9_OFFSET                             (0x224U)
#define PMU_GRF_OS_REG9_PMU_OS_REG9_SHIFT                  (0U)
#define PMU_GRF_OS_REG9_PMU_OS_REG9_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG9_PMU_OS_REG9_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG10 */
#define PMU_GRF_OS_REG10_OFFSET                            (0x228U)
#define PMU_GRF_OS_REG10_PMU_OS_REG10_SHIFT                (0U)
#define PMU_GRF_OS_REG10_PMU_OS_REG10_MASK                 (0xFFFFFFFFU << PMU_GRF_OS_REG10_PMU_OS_REG10_SHIFT)         /* 0xFFFFFFFF */
/* OS_REG11 */
#define PMU_GRF_OS_REG11_OFFSET                            (0x22CU)
#define PMU_GRF_OS_REG11_PMU_OS_REG11_SHIFT                (0U)
#define PMU_GRF_OS_REG11_PMU_OS_REG11_MASK                 (0xFFFFFFFFU << PMU_GRF_OS_REG11_PMU_OS_REG11_SHIFT)         /* 0xFFFFFFFF */
/* RESET_FUNCTION_STATUS */
#define PMU_GRF_RESET_FUNCTION_STATUS_OFFSET               (0x230U)
#define PMU_GRF_RESET_FUNCTION_STATUS                      (0x0U)
#define PMU_GRF_RESET_FUNCTION_STATUS_ST_RSTFUNC_STATUS_SHIFT (0U)
#define PMU_GRF_RESET_FUNCTION_STATUS_ST_RSTFUNC_STATUS_MASK (0xFFFFFFFFU << PMU_GRF_RESET_FUNCTION_STATUS_ST_RSTFUNC_STATUS_SHIFT) /* 0xFFFFFFFF */
/* SIG_DETECT_CON */
#define PMU_GRF_SIG_DETECT_CON_OFFSET                      (0x380U)
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_POS_IRQ_MSK_SHIFT (0U)
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_POS_IRQ_MSK_MASK (0x1U << PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_POS_IRQ_MSK_SHIFT) /* 0x00000001 */
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_NEG_IRQ_MSK_SHIFT (1U)
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_NEG_IRQ_MSK_MASK (0x1U << PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_NEG_IRQ_MSK_SHIFT) /* 0x00000002 */
/* SIG_DETECT_STATUS */
#define PMU_GRF_SIG_DETECT_STATUS_OFFSET                   (0x390U)
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_POS_IRQ_SHIFT (0U)
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_POS_IRQ_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_POS_IRQ_SHIFT) /* 0x00000001 */
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_NEG_IRQ_SHIFT (1U)
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_NEG_IRQ_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_NEG_IRQ_SHIFT) /* 0x00000002 */
/* SIG_DETECT_STATUS_CLEAR */
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_OFFSET             (0x3A0U)
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_POS_IRQ_CLR_SHIFT (0U)
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_POS_IRQ_CLR_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_POS_IRQ_CLR_SHIFT) /* 0x00000001 */
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_NEG_IRQ_CLR_SHIFT (1U)
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_NEG_IRQ_CLR_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_NEG_IRQ_CLR_SHIFT) /* 0x00000002 */
/* SDMMC_DET_COUNTER */
#define PMU_GRF_SDMMC_DET_COUNTER_OFFSET                   (0x3B0U)
#define PMU_GRF_SDMMC_DET_COUNTER_SDMMC_DETECTN_COUNT_SHIFT (0U)
#define PMU_GRF_SDMMC_DET_COUNTER_SDMMC_DETECTN_COUNT_MASK (0xFFFFFU << PMU_GRF_SDMMC_DET_COUNTER_SDMMC_DETECTN_COUNT_SHIFT) /* 0x000FFFFF */
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_OFFSET                                    (0x0U)
#define UART_RBR_DATA_INPUT_SHIFT                          (0U)
#define UART_RBR_DATA_INPUT_MASK                           (0xFFU << UART_RBR_DATA_INPUT_SHIFT)                         /* 0x000000FF */
/* THR */
#define UART_THR_OFFSET                                    (0x0U)
#define UART_THR_DATA_OUTPUT_SHIFT                         (0U)
#define UART_THR_DATA_OUTPUT_MASK                          (0xFFU << UART_THR_DATA_OUTPUT_SHIFT)                        /* 0x000000FF */
/* DLL */
#define UART_DLL_OFFSET                                    (0x0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT                 (0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_MASK                  (0xFFU << UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT)                /* 0x000000FF */
/* DLH */
#define UART_DLH_OFFSET                                    (0x4U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT                 (0U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_MASK                  (0xFFU << UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT)                /* 0x000000FF */
/* IER */
#define UART_IER_OFFSET                                    (0x4U)
#define UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_SHIFT       (0U)
#define UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_MASK        (0x1U << UART_IER_RECEIVE_DATA_AVAILABLE_INT_EN_SHIFT)       /* 0x00000001 */
#define UART_IER_TRANS_HOLD_EMPTY_INT_EN_SHIFT             (1U)
#define UART_IER_TRANS_HOLD_EMPTY_INT_EN_MASK              (0x1U << UART_IER_TRANS_HOLD_EMPTY_INT_EN_SHIFT)             /* 0x00000002 */
#define UART_IER_RECEIVE_LINE_STATUS_INT_EN_SHIFT          (2U)
#define UART_IER_RECEIVE_LINE_STATUS_INT_EN_MASK           (0x1U << UART_IER_RECEIVE_LINE_STATUS_INT_EN_SHIFT)          /* 0x00000004 */
#define UART_IER_MODEM_STATUS_INT_EN_SHIFT                 (3U)
#define UART_IER_MODEM_STATUS_INT_EN_MASK                  (0x1U << UART_IER_MODEM_STATUS_INT_EN_SHIFT)                 /* 0x00000008 */
#define UART_IER_PROG_THRE_INT_EN_SHIFT                    (7U)
#define UART_IER_PROG_THRE_INT_EN_MASK                     (0x1U << UART_IER_PROG_THRE_INT_EN_SHIFT)                    /* 0x00000080 */
/* IIR */
#define UART_IIR_OFFSET                                    (0x8U)
#define UART_IIR                                           (0x1U)
#define UART_IIR_INT_ID_SHIFT                              (0U)
#define UART_IIR_INT_ID_MASK                               (0xFU << UART_IIR_INT_ID_SHIFT)                              /* 0x0000000F */
#define UART_IIR_FIFOS_EN_SHIFT                            (6U)
#define UART_IIR_FIFOS_EN_MASK                             (0x3U << UART_IIR_FIFOS_EN_SHIFT)                            /* 0x000000C0 */
/* FCR */
#define UART_FCR_OFFSET                                    (0x8U)
#define UART_FCR_FIFO_EN_SHIFT                             (0U)
#define UART_FCR_FIFO_EN_MASK                              (0x1U << UART_FCR_FIFO_EN_SHIFT)                             /* 0x00000001 */
#define UART_FCR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_FCR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_FCR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_FCR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_FCR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_FCR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
#define UART_FCR_DMA_MODE_SHIFT                            (3U)
#define UART_FCR_DMA_MODE_MASK                             (0x1U << UART_FCR_DMA_MODE_SHIFT)                            /* 0x00000008 */
#define UART_FCR_TX_EMPTY_TRIGGER_SHIFT                    (4U)
#define UART_FCR_TX_EMPTY_TRIGGER_MASK                     (0x3U << UART_FCR_TX_EMPTY_TRIGGER_SHIFT)                    /* 0x00000030 */
#define UART_FCR_RCVR_TRIGGER_SHIFT                        (6U)
#define UART_FCR_RCVR_TRIGGER_MASK                         (0x3U << UART_FCR_RCVR_TRIGGER_SHIFT)                        /* 0x000000C0 */
/* LCR */
#define UART_LCR_OFFSET                                    (0xCU)
#define UART_LCR_DATA_LENGTH_SEL_SHIFT                     (0U)
#define UART_LCR_DATA_LENGTH_SEL_MASK                      (0x3U << UART_LCR_DATA_LENGTH_SEL_SHIFT)                     /* 0x00000003 */
#define UART_LCR_STOP_BITS_NUM_SHIFT                       (2U)
#define UART_LCR_STOP_BITS_NUM_MASK                        (0x1U << UART_LCR_STOP_BITS_NUM_SHIFT)                       /* 0x00000004 */
#define UART_LCR_PARITY_EN_SHIFT                           (3U)
#define UART_LCR_PARITY_EN_MASK                            (0x1U << UART_LCR_PARITY_EN_SHIFT)                           /* 0x00000008 */
#define UART_LCR_EVEN_PARITY_SEL_SHIFT                     (4U)
#define UART_LCR_EVEN_PARITY_SEL_MASK                      (0x1U << UART_LCR_EVEN_PARITY_SEL_SHIFT)                     /* 0x00000010 */
#define UART_LCR_BREAK_CTRL_SHIFT                          (6U)
#define UART_LCR_BREAK_CTRL_MASK                           (0x1U << UART_LCR_BREAK_CTRL_SHIFT)                          /* 0x00000040 */
#define UART_LCR_DIV_LAT_ACCESS_SHIFT                      (7U)
#define UART_LCR_DIV_LAT_ACCESS_MASK                       (0x1U << UART_LCR_DIV_LAT_ACCESS_SHIFT)                      /* 0x00000080 */
/* MCR */
#define UART_MCR_OFFSET                                    (0x10U)
#define UART_MCR_DATA_TERMINAL_READY_SHIFT                 (0U)
#define UART_MCR_DATA_TERMINAL_READY_MASK                  (0x1U << UART_MCR_DATA_TERMINAL_READY_SHIFT)                 /* 0x00000001 */
#define UART_MCR_REQ_TO_SEND_SHIFT                         (1U)
#define UART_MCR_REQ_TO_SEND_MASK                          (0x1U << UART_MCR_REQ_TO_SEND_SHIFT)                         /* 0x00000002 */
#define UART_MCR_OUT1_SHIFT                                (2U)
#define UART_MCR_OUT1_MASK                                 (0x1U << UART_MCR_OUT1_SHIFT)                                /* 0x00000004 */
#define UART_MCR_OUT2_SHIFT                                (3U)
#define UART_MCR_OUT2_MASK                                 (0x1U << UART_MCR_OUT2_SHIFT)                                /* 0x00000008 */
#define UART_MCR_LOOPBACK_SHIFT                            (4U)
#define UART_MCR_LOOPBACK_MASK                             (0x1U << UART_MCR_LOOPBACK_SHIFT)                            /* 0x00000010 */
#define UART_MCR_AUTO_FLOW_CTRL_EN_SHIFT                   (5U)
#define UART_MCR_AUTO_FLOW_CTRL_EN_MASK                    (0x1U << UART_MCR_AUTO_FLOW_CTRL_EN_SHIFT)                   /* 0x00000020 */
#define UART_MCR_SIR_MODE_EN_SHIFT                         (6U)
#define UART_MCR_SIR_MODE_EN_MASK                          (0x1U << UART_MCR_SIR_MODE_EN_SHIFT)                         /* 0x00000040 */
/* LSR */
#define UART_LSR_OFFSET                                    (0x14U)
#define UART_LSR                                           (0x60U)
#define UART_LSR_DATA_READY_SHIFT                          (0U)
#define UART_LSR_DATA_READY_MASK                           (0x1U << UART_LSR_DATA_READY_SHIFT)                          /* 0x00000001 */
#define UART_LSR_OVERRUN_ERROR_SHIFT                       (1U)
#define UART_LSR_OVERRUN_ERROR_MASK                        (0x1U << UART_LSR_OVERRUN_ERROR_SHIFT)                       /* 0x00000002 */
#define UART_LSR_PARITY_EROR_SHIFT                         (2U)
#define UART_LSR_PARITY_EROR_MASK                          (0x1U << UART_LSR_PARITY_EROR_SHIFT)                         /* 0x00000004 */
#define UART_LSR_FRAMING_ERROR_SHIFT                       (3U)
#define UART_LSR_FRAMING_ERROR_MASK                        (0x1U << UART_LSR_FRAMING_ERROR_SHIFT)                       /* 0x00000008 */
#define UART_LSR_BREAK_INT_SHIFT                           (4U)
#define UART_LSR_BREAK_INT_MASK                            (0x1U << UART_LSR_BREAK_INT_SHIFT)                           /* 0x00000010 */
#define UART_LSR_TRANS_HOLD_REG_EMPTY_SHIFT                (5U)
#define UART_LSR_TRANS_HOLD_REG_EMPTY_MASK                 (0x1U << UART_LSR_TRANS_HOLD_REG_EMPTY_SHIFT)                /* 0x00000020 */
#define UART_LSR_TRANS_EMPTY_SHIFT                         (6U)
#define UART_LSR_TRANS_EMPTY_MASK                          (0x1U << UART_LSR_TRANS_EMPTY_SHIFT)                         /* 0x00000040 */
#define UART_LSR_RECEIVER_FIFO_ERROR_SHIFT                 (7U)
#define UART_LSR_RECEIVER_FIFO_ERROR_MASK                  (0x1U << UART_LSR_RECEIVER_FIFO_ERROR_SHIFT)                 /* 0x00000080 */
/* MSR */
#define UART_MSR_OFFSET                                    (0x18U)
#define UART_MSR                                           (0x0U)
#define UART_MSR_DELTA_CLEAR_TO_SEND_SHIFT                 (0U)
#define UART_MSR_DELTA_CLEAR_TO_SEND_MASK                  (0x1U << UART_MSR_DELTA_CLEAR_TO_SEND_SHIFT)                 /* 0x00000001 */
#define UART_MSR_DELTA_DATA_SET_READY_SHIFT                (1U)
#define UART_MSR_DELTA_DATA_SET_READY_MASK                 (0x1U << UART_MSR_DELTA_DATA_SET_READY_SHIFT)                /* 0x00000002 */
#define UART_MSR_TRAILING_EDGE_RING_INDICATOR_SHIFT        (2U)
#define UART_MSR_TRAILING_EDGE_RING_INDICATOR_MASK         (0x1U << UART_MSR_TRAILING_EDGE_RING_INDICATOR_SHIFT)        /* 0x00000004 */
#define UART_MSR_DELTA_DATA_CARRIER_DETECT_SHIFT           (3U)
#define UART_MSR_DELTA_DATA_CARRIER_DETECT_MASK            (0x1U << UART_MSR_DELTA_DATA_CARRIER_DETECT_SHIFT)           /* 0x00000008 */
#define UART_MSR_CLEAR_TO_SEND_SHIFT                       (4U)
#define UART_MSR_CLEAR_TO_SEND_MASK                        (0x1U << UART_MSR_CLEAR_TO_SEND_SHIFT)                       /* 0x00000010 */
#define UART_MSR_DATA_SET_READY_SHIFT                      (5U)
#define UART_MSR_DATA_SET_READY_MASK                       (0x1U << UART_MSR_DATA_SET_READY_SHIFT)                      /* 0x00000020 */
#define UART_MSR_RING_INDICATOR_SHIFT                      (6U)
#define UART_MSR_RING_INDICATOR_MASK                       (0x1U << UART_MSR_RING_INDICATOR_SHIFT)                      /* 0x00000040 */
#define UART_MSR_DATA_CARRIOR_DETECT_SHIFT                 (7U)
#define UART_MSR_DATA_CARRIOR_DETECT_MASK                  (0x1U << UART_MSR_DATA_CARRIOR_DETECT_SHIFT)                 /* 0x00000080 */
/* SCR */
#define UART_SCR_OFFSET                                    (0x1CU)
#define UART_SCR_TEMP_STORE_SPACE_SHIFT                    (0U)
#define UART_SCR_TEMP_STORE_SPACE_MASK                     (0xFFU << UART_SCR_TEMP_STORE_SPACE_SHIFT)                   /* 0x000000FF */
/* SRBR */
#define UART_SRBR_OFFSET                                   (0x30U)
#define UART_SRBR                                          (0x0U)
#define UART_SRBR_SHADOW_RBR_SHIFT                         (0U)
#define UART_SRBR_SHADOW_RBR_MASK                          (0xFFU << UART_SRBR_SHADOW_RBR_SHIFT)                        /* 0x000000FF */
/* STHR */
#define UART_STHR_OFFSET                                   (0x6CU)
#define UART_STHR                                          (0x0U)
#define UART_STHR_SHADOW_THR_SHIFT                         (0U)
#define UART_STHR_SHADOW_THR_MASK                          (0xFFU << UART_STHR_SHADOW_THR_SHIFT)                        /* 0x000000FF */
/* FAR */
#define UART_FAR_OFFSET                                    (0x70U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT                 (0U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_MASK                  (0x1U << UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT)                 /* 0x00000001 */
/* TFR */
#define UART_TFR_OFFSET                                    (0x74U)
#define UART_TFR                                           (0x0U)
#define UART_TFR_TRANS_FIFO_READ_SHIFT                     (0U)
#define UART_TFR_TRANS_FIFO_READ_MASK                      (0xFFU << UART_TFR_TRANS_FIFO_READ_SHIFT)                    /* 0x000000FF */
/* RFW */
#define UART_RFW_OFFSET                                    (0x78U)
#define UART_RFW_RECEIVE_FIFO_WRITE_SHIFT                  (0U)
#define UART_RFW_RECEIVE_FIFO_WRITE_MASK                   (0xFFU << UART_RFW_RECEIVE_FIFO_WRITE_SHIFT)                 /* 0x000000FF */
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT           (8U)
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_MASK            (0x1U << UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT)           /* 0x00000100 */
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT          (9U)
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_MASK           (0x1U << UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT)          /* 0x00000200 */
/* USR */
#define UART_USR_OFFSET                                    (0x7CU)
#define UART_USR                                           (0x6U)
#define UART_USR_UART_BUSY_SHIFT                           (0U)
#define UART_USR_UART_BUSY_MASK                            (0x1U << UART_USR_UART_BUSY_SHIFT)                           /* 0x00000001 */
#define UART_USR_TRANS_FIFO_NOT_FULL_SHIFT                 (1U)
#define UART_USR_TRANS_FIFO_NOT_FULL_MASK                  (0x1U << UART_USR_TRANS_FIFO_NOT_FULL_SHIFT)                 /* 0x00000002 */
#define UART_USR_TRASN_FIFO_EMPTY_SHIFT                    (2U)
#define UART_USR_TRASN_FIFO_EMPTY_MASK                     (0x1U << UART_USR_TRASN_FIFO_EMPTY_SHIFT)                    /* 0x00000004 */
#define UART_USR_RECEIVE_FIFO_NOT_EMPTY_SHIFT              (3U)
#define UART_USR_RECEIVE_FIFO_NOT_EMPTY_MASK               (0x1U << UART_USR_RECEIVE_FIFO_NOT_EMPTY_SHIFT)              /* 0x00000008 */
#define UART_USR_RECEIVE_FIFO_FULL_SHIFT                   (4U)
#define UART_USR_RECEIVE_FIFO_FULL_MASK                    (0x1U << UART_USR_RECEIVE_FIFO_FULL_SHIFT)                   /* 0x00000010 */
/* TFL */
#define UART_TFL_OFFSET                                    (0x80U)
#define UART_TFL_TRANS_FIFO_LEVEL_SHIFT                    (0U)
#define UART_TFL_TRANS_FIFO_LEVEL_MASK                     (0x1FU << UART_TFL_TRANS_FIFO_LEVEL_SHIFT)                   /* 0x0000001F */
/* RFL */
#define UART_RFL_OFFSET                                    (0x84U)
#define UART_RFL                                           (0x0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT                  (0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_MASK                   (0x1FU << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)                 /* 0x0000001F */
/* SRR */
#define UART_SRR_OFFSET                                    (0x88U)
#define UART_SRR_UART_RESET_SHIFT                          (0U)
#define UART_SRR_UART_RESET_MASK                           (0x1U << UART_SRR_UART_RESET_SHIFT)                          /* 0x00000001 */
#define UART_SRR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_SRR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_SRR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_SRR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_SRR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_SRR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
/* SRTS */
#define UART_SRTS_OFFSET                                   (0x8CU)
#define UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT                 (0U)
#define UART_SRTS_SHADOW_REQ_TO_SEND_MASK                  (0x1U << UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT)                 /* 0x00000001 */
/* SBCR */
#define UART_SBCR_OFFSET                                   (0x90U)
#define UART_SBCR_SHADOW_BREAK_CTRL_SHIFT                  (0U)
#define UART_SBCR_SHADOW_BREAK_CTRL_MASK                   (0x1U << UART_SBCR_SHADOW_BREAK_CTRL_SHIFT)                  /* 0x00000001 */
/* SDMAM */
#define UART_SDMAM_OFFSET                                  (0x94U)
#define UART_SDMAM_SHADOW_DMA_MODE_SHIFT                   (0U)
#define UART_SDMAM_SHADOW_DMA_MODE_MASK                    (0x1U << UART_SDMAM_SHADOW_DMA_MODE_SHIFT)                   /* 0x00000001 */
/* SFE */
#define UART_SFE_OFFSET                                    (0x98U)
#define UART_SFE_SHADOW_FIFO_EN_SHIFT                      (0U)
#define UART_SFE_SHADOW_FIFO_EN_MASK                       (0x1U << UART_SFE_SHADOW_FIFO_EN_SHIFT)                      /* 0x00000001 */
/* SRT */
#define UART_SRT_OFFSET                                    (0x9CU)
#define UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT                 (0U)
#define UART_SRT_SHADOW_RCVR_TRIGGER_MASK                  (0x3U << UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT)                 /* 0x00000003 */
/* STET */
#define UART_STET_OFFSET                                   (0xA0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT            (0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_MASK             (0x3U << UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT)            /* 0x00000003 */
/* HTX */
#define UART_HTX_OFFSET                                    (0xA4U)
#define UART_HTX_HALT_TX_EN_SHIFT                          (0U)
#define UART_HTX_HALT_TX_EN_MASK                           (0x1U << UART_HTX_HALT_TX_EN_SHIFT)                          /* 0x00000001 */
/* DMASA */
#define UART_DMASA_OFFSET                                  (0xA8U)
#define UART_DMASA_DMA_SOFTWARE_ACK_SHIFT                  (0U)
#define UART_DMASA_DMA_SOFTWARE_ACK_MASK                   (0x1U << UART_DMASA_DMA_SOFTWARE_ACK_SHIFT)                  /* 0x00000001 */
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
#define UART_UCV                                           (0x3330382AU)
#define UART_UCV_VER_SHIFT                                 (0U)
#define UART_UCV_VER_MASK                                  (0xFFFFFFFFU << UART_UCV_VER_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_OFFSET                                    (0xFCU)
#define UART_CTR                                           (0x44570110U)
#define UART_CTR_PERIPHERAL_ID_SHIFT                       (0U)
#define UART_CTR_PERIPHERAL_ID_MASK                        (0xFFFFFFFFU << UART_CTR_PERIPHERAL_ID_SHIFT)                /* 0xFFFFFFFF */
/******************************************GPIO******************************************/
/* SWPORTA_DR */
#define GPIO_SWPORTA_DR_OFFSET                             (0x0U)
#define GPIO_SWPORTA_DR_GPIO_SWPORTA_DR_SHIFT              (0U)
#define GPIO_SWPORTA_DR_GPIO_SWPORTA_DR_MASK               (0xFFFFFFFFU << GPIO_SWPORTA_DR_GPIO_SWPORTA_DR_SHIFT)       /* 0xFFFFFFFF */
/* SWPORTA_DDR */
#define GPIO_SWPORTA_DDR_OFFSET                            (0x4U)
#define GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_SHIFT            (0U)
#define GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_MASK             (0xFFFFFFFFU << GPIO_SWPORTA_DDR_GPIO_SWPORTA_DDR_SHIFT)     /* 0xFFFFFFFF */
/* INTEN */
#define GPIO_INTEN_OFFSET                                  (0x30U)
#define GPIO_INTEN_GPIO_INT_EN_SHIFT                       (0U)
#define GPIO_INTEN_GPIO_INT_EN_MASK                        (0xFFFFFFFFU << GPIO_INTEN_GPIO_INT_EN_SHIFT)                /* 0xFFFFFFFF */
/* INTMASK */
#define GPIO_INTMASK_OFFSET                                (0x34U)
#define GPIO_INTMASK_GPIO_INT_MASK_SHIFT                   (0U)
#define GPIO_INTMASK_GPIO_INT_MASK_MASK                    (0x1U << GPIO_INTMASK_GPIO_INT_MASK_SHIFT)                   /* 0x00000001 */
/* INTTYPE_LEVEL */
#define GPIO_INTTYPE_LEVEL_OFFSET                          (0x38U)
#define GPIO_INTTYPE_LEVEL_GPIO_INTTYPE_LEVEL_SHIFT        (0U)
#define GPIO_INTTYPE_LEVEL_GPIO_INTTYPE_LEVEL_MASK         (0x1U << GPIO_INTTYPE_LEVEL_GPIO_INTTYPE_LEVEL_SHIFT)        /* 0x00000001 */
/* INT_POLARITY */
#define GPIO_INT_POLARITY_OFFSET                           (0x3CU)
#define GPIO_INT_POLARITY_GPIO_INT_POLARITY_SHIFT          (0U)
#define GPIO_INT_POLARITY_GPIO_INT_POLARITY_MASK           (0xFFFFFFFFU << GPIO_INT_POLARITY_GPIO_INT_POLARITY_SHIFT)   /* 0xFFFFFFFF */
/* INT_STATUS */
#define GPIO_INT_STATUS_OFFSET                             (0x40U)
#define GPIO_INT_STATUS                                    (0x0U)
#define GPIO_INT_STATUS_GPIO_INT_STATUS_SHIFT              (0U)
#define GPIO_INT_STATUS_GPIO_INT_STATUS_MASK               (0xFFFFFFFFU << GPIO_INT_STATUS_GPIO_INT_STATUS_SHIFT)       /* 0xFFFFFFFF */
/* INT_RAWSTATUS */
#define GPIO_INT_RAWSTATUS_OFFSET                          (0x44U)
#define GPIO_INT_RAWSTATUS                                 (0x0U)
#define GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_SHIFT        (0U)
#define GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_MASK         (0xFFFFFFFFU << GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_SHIFT) /* 0xFFFFFFFF */
/* DEBOUNCE */
#define GPIO_DEBOUNCE_OFFSET                               (0x48U)
#define GPIO_DEBOUNCE_GPIO_DEBOUNCE_SHIFT                  (0U)
#define GPIO_DEBOUNCE_GPIO_DEBOUNCE_MASK                   (0xFFFFFFFFU << GPIO_DEBOUNCE_GPIO_DEBOUNCE_SHIFT)           /* 0xFFFFFFFF */
/* PORTA_EOI */
#define GPIO_PORTA_EOI_OFFSET                              (0x4CU)
#define GPIO_PORTA_EOI_GPIO_PORTA_EOI_SHIFT                (0U)
#define GPIO_PORTA_EOI_GPIO_PORTA_EOI_MASK                 (0xFFFFFFFFU << GPIO_PORTA_EOI_GPIO_PORTA_EOI_SHIFT)         /* 0xFFFFFFFF */
/* EXT_PORTA */
#define GPIO_EXT_PORTA_OFFSET                              (0x50U)
#define GPIO_EXT_PORTA                                     (0x0U)
#define GPIO_EXT_PORTA_GPIO_EXT_PORTA_SHIFT                (0U)
#define GPIO_EXT_PORTA_GPIO_EXT_PORTA_MASK                 (0xFFFFFFFFU << GPIO_EXT_PORTA_GPIO_EXT_PORTA_SHIFT)         /* 0xFFFFFFFF */
/* LS_SYNC */
#define GPIO_LS_SYNC_OFFSET                                (0x60U)
#define GPIO_LS_SYNC_GPIO_LS_SYNC_SHIFT                    (0U)
#define GPIO_LS_SYNC_GPIO_LS_SYNC_MASK                     (0xFFFFFFFFU << GPIO_LS_SYNC_GPIO_LS_SYNC_SHIFT)             /* 0xFFFFFFFF */
/* INT_BOTHEDGE */
#define GPIO_INT_BOTHEDGE_OFFSET                           (0x68U)
#define GPIO_INT_BOTHEDGE_INTERRUPT_BOTH_EDGE_TYPE_SHIFT   (0U)
#define GPIO_INT_BOTHEDGE_INTERRUPT_BOTH_EDGE_TYPE_MASK    (0xFFFFFFFFU << GPIO_INT_BOTHEDGE_INTERRUPT_BOTH_EDGE_TYPE_SHIFT) /* 0xFFFFFFFF */
/******************************************GRF*******************************************/
/* GPIO1A_IOMUX_L */
#define GRF_GPIO1A_IOMUX_L_OFFSET                          (0x0U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_SHIFT               (0U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_SHIFT               (4U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_SHIFT               (8U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_SHIFT               (12U)
#define GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1A_IOMUX_H */
#define GRF_GPIO1A_IOMUX_H_OFFSET                          (0x4U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_SHIFT               (0U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_SHIFT               (4U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_SHIFT               (8U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_SHIFT               (12U)
#define GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_MASK                (0xFU << GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1B_IOMUX_L */
#define GRF_GPIO1B_IOMUX_L_OFFSET                          (0x8U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_SHIFT               (0U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_SHIFT               (4U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_SHIFT               (8U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_SHIFT               (12U)
#define GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1B_IOMUX_H */
#define GRF_GPIO1B_IOMUX_H_OFFSET                          (0xCU)
#define GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_SHIFT               (0U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_SHIFT               (4U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_SHIFT               (8U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_SHIFT               (12U)
#define GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_MASK                (0xFU << GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1C_IOMUX_L */
#define GRF_GPIO1C_IOMUX_L_OFFSET                          (0x10U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C0_SEL_SHIFT               (0U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C0_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1C_IOMUX_L_GPIO1C1_SEL_SHIFT               (4U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C1_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1C_IOMUX_L_GPIO1C2_SEL_SHIFT               (8U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C2_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1C_IOMUX_L_GPIO1C3_SEL_SHIFT               (12U)
#define GRF_GPIO1C_IOMUX_L_GPIO1C3_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_L_GPIO1C3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1C_IOMUX_H */
#define GRF_GPIO1C_IOMUX_H_OFFSET                          (0x14U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C4_SEL_SHIFT               (0U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C4_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1C_IOMUX_H_GPIO1C5_SEL_SHIFT               (4U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C5_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1C_IOMUX_H_GPIO1C6_SEL_SHIFT               (8U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C6_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1C_IOMUX_H_GPIO1C7_SEL_SHIFT               (12U)
#define GRF_GPIO1C_IOMUX_H_GPIO1C7_SEL_MASK                (0xFU << GRF_GPIO1C_IOMUX_H_GPIO1C7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1D_IOMUX_L */
#define GRF_GPIO1D_IOMUX_L_OFFSET                          (0x18U)
#define GRF_GPIO1D_IOMUX_L_GPIO1D0_SEL_SHIFT               (0U)
#define GRF_GPIO1D_IOMUX_L_GPIO1D0_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_L_GPIO1D0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1D_IOMUX_L_GPIO1D1_SEL_SHIFT               (4U)
#define GRF_GPIO1D_IOMUX_L_GPIO1D1_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_L_GPIO1D1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1D_IOMUX_L_GPIO1D2_SEL_SHIFT               (8U)
#define GRF_GPIO1D_IOMUX_L_GPIO1D2_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_L_GPIO1D2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1D_IOMUX_L_GPIO1D3_SEL_SHIFT               (12U)
#define GRF_GPIO1D_IOMUX_L_GPIO1D3_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_L_GPIO1D3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1D_IOMUX_H */
#define GRF_GPIO1D_IOMUX_H_OFFSET                          (0x1CU)
#define GRF_GPIO1D_IOMUX_H_GPIO1D4_SEL_SHIFT               (0U)
#define GRF_GPIO1D_IOMUX_H_GPIO1D4_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_H_GPIO1D4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO1D_IOMUX_H_GPIO1D5_SEL_SHIFT               (4U)
#define GRF_GPIO1D_IOMUX_H_GPIO1D5_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_H_GPIO1D5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO1D_IOMUX_H_GPIO1D6_SEL_SHIFT               (8U)
#define GRF_GPIO1D_IOMUX_H_GPIO1D6_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_H_GPIO1D6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO1D_IOMUX_H_GPIO1D7_SEL_SHIFT               (12U)
#define GRF_GPIO1D_IOMUX_H_GPIO1D7_SEL_MASK                (0xFU << GRF_GPIO1D_IOMUX_H_GPIO1D7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO2A_IOMUX_L */
#define GRF_GPIO2A_IOMUX_L_OFFSET                          (0x20U)
#define GRF_GPIO2A_IOMUX_L_GPIO2A0_SEL_SHIFT               (0U)
#define GRF_GPIO2A_IOMUX_L_GPIO2A0_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_L_GPIO2A0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO2A_IOMUX_L_GPIO2A1_SEL_SHIFT               (4U)
#define GRF_GPIO2A_IOMUX_L_GPIO2A1_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_L_GPIO2A1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO2A_IOMUX_L_GPIO2A2_SEL_SHIFT               (8U)
#define GRF_GPIO2A_IOMUX_L_GPIO2A2_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_L_GPIO2A2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO2A_IOMUX_L_GPIO2A3_SEL_SHIFT               (12U)
#define GRF_GPIO2A_IOMUX_L_GPIO2A3_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_L_GPIO2A3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO2A_IOMUX_H */
#define GRF_GPIO2A_IOMUX_H_OFFSET                          (0x24U)
#define GRF_GPIO2A_IOMUX_H_GPIO2A4_SEL_SHIFT               (0U)
#define GRF_GPIO2A_IOMUX_H_GPIO2A4_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_H_GPIO2A4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO2A_IOMUX_H_GPIO2A5_SEL_SHIFT               (4U)
#define GRF_GPIO2A_IOMUX_H_GPIO2A5_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_H_GPIO2A5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO2A_IOMUX_H_GPIO2A6_SEL_SHIFT               (8U)
#define GRF_GPIO2A_IOMUX_H_GPIO2A6_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_H_GPIO2A6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO2A_IOMUX_H_GPIO2A7_SEL_SHIFT               (12U)
#define GRF_GPIO2A_IOMUX_H_GPIO2A7_SEL_MASK                (0xFU << GRF_GPIO2A_IOMUX_H_GPIO2A7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO2B_IOMUX_L */
#define GRF_GPIO2B_IOMUX_L_OFFSET                          (0x28U)
#define GRF_GPIO2B_IOMUX_L_GPIO2B0_SEL_SHIFT               (0U)
#define GRF_GPIO2B_IOMUX_L_GPIO2B0_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_L_GPIO2B0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO2B_IOMUX_L_GPIO2B1_SEL_SHIFT               (4U)
#define GRF_GPIO2B_IOMUX_L_GPIO2B1_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_L_GPIO2B1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO2B_IOMUX_L_GPIO2B2_SEL_SHIFT               (8U)
#define GRF_GPIO2B_IOMUX_L_GPIO2B2_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_L_GPIO2B2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO2B_IOMUX_L_GPIO2B3_SEL_SHIFT               (12U)
#define GRF_GPIO2B_IOMUX_L_GPIO2B3_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_L_GPIO2B3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO2B_IOMUX_H */
#define GRF_GPIO2B_IOMUX_H_OFFSET                          (0x2CU)
#define GRF_GPIO2B_IOMUX_H_GPIO2B4_SEL_SHIFT               (0U)
#define GRF_GPIO2B_IOMUX_H_GPIO2B4_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_H_GPIO2B4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO2B_IOMUX_H_GPIO2B5_SEL_SHIFT               (4U)
#define GRF_GPIO2B_IOMUX_H_GPIO2B5_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_H_GPIO2B5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO2B_IOMUX_H_GPIO2B6_SEL_SHIFT               (8U)
#define GRF_GPIO2B_IOMUX_H_GPIO2B6_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_H_GPIO2B6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO2B_IOMUX_H_GPIO2B7_SEL_SHIFT               (12U)
#define GRF_GPIO2B_IOMUX_H_GPIO2B7_SEL_MASK                (0xFU << GRF_GPIO2B_IOMUX_H_GPIO2B7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO2C_IOMUX_L */
#define GRF_GPIO2C_IOMUX_L_OFFSET                          (0x30U)
#define GRF_GPIO2C_IOMUX_L_GPIO2C0_SEL_SHIFT               (0U)
#define GRF_GPIO2C_IOMUX_L_GPIO2C0_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_L_GPIO2C0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO2C_IOMUX_L_GPIO2C1_SEL_SHIFT               (4U)
#define GRF_GPIO2C_IOMUX_L_GPIO2C1_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_L_GPIO2C1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO2C_IOMUX_L_GPIO2C2_SEL_SHIFT               (8U)
#define GRF_GPIO2C_IOMUX_L_GPIO2C2_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_L_GPIO2C2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO2C_IOMUX_L_GPIO2C3_SEL_SHIFT               (12U)
#define GRF_GPIO2C_IOMUX_L_GPIO2C3_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_L_GPIO2C3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO2C_IOMUX_H */
#define GRF_GPIO2C_IOMUX_H_OFFSET                          (0x34U)
#define GRF_GPIO2C_IOMUX_H_GPIO2C4_SEL_SHIFT               (0U)
#define GRF_GPIO2C_IOMUX_H_GPIO2C4_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_H_GPIO2C4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO2C_IOMUX_H_GPIO2C5_SEL_SHIFT               (4U)
#define GRF_GPIO2C_IOMUX_H_GPIO2C5_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_H_GPIO2C5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO2C_IOMUX_H_GPIO2C6_SEL_SHIFT               (8U)
#define GRF_GPIO2C_IOMUX_H_GPIO2C6_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_H_GPIO2C6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO2C_IOMUX_H_GPIO2C7_SEL_SHIFT               (12U)
#define GRF_GPIO2C_IOMUX_H_GPIO2C7_SEL_MASK                (0xFU << GRF_GPIO2C_IOMUX_H_GPIO2C7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3A_IOMUX_L */
#define GRF_GPIO3A_IOMUX_L_OFFSET                          (0x40U)
#define GRF_GPIO3A_IOMUX_L_GPIO3A0_SEL_SHIFT               (0U)
#define GRF_GPIO3A_IOMUX_L_GPIO3A0_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_L_GPIO3A0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3A_IOMUX_L_GPIO3A1_SEL_SHIFT               (4U)
#define GRF_GPIO3A_IOMUX_L_GPIO3A1_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_L_GPIO3A1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3A_IOMUX_L_GPIO3A2_SEL_SHIFT               (8U)
#define GRF_GPIO3A_IOMUX_L_GPIO3A2_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_L_GPIO3A2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3A_IOMUX_L_GPIO3A3_SEL_SHIFT               (12U)
#define GRF_GPIO3A_IOMUX_L_GPIO3A3_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_L_GPIO3A3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3A_IOMUX_H */
#define GRF_GPIO3A_IOMUX_H_OFFSET                          (0x44U)
#define GRF_GPIO3A_IOMUX_H_GPIO3A4_SEL_SHIFT               (0U)
#define GRF_GPIO3A_IOMUX_H_GPIO3A4_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_H_GPIO3A4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3A_IOMUX_H_GPIO3A5_SEL_SHIFT               (4U)
#define GRF_GPIO3A_IOMUX_H_GPIO3A5_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_H_GPIO3A5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3A_IOMUX_H_GPIO3A6_SEL_SHIFT               (8U)
#define GRF_GPIO3A_IOMUX_H_GPIO3A6_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_H_GPIO3A6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3A_IOMUX_H_GPIO3A7_SEL_SHIFT               (12U)
#define GRF_GPIO3A_IOMUX_H_GPIO3A7_SEL_MASK                (0xFU << GRF_GPIO3A_IOMUX_H_GPIO3A7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3B_IOMUX_L */
#define GRF_GPIO3B_IOMUX_L_OFFSET                          (0x48U)
#define GRF_GPIO3B_IOMUX_L_GPIO3B0_SEL_SHIFT               (0U)
#define GRF_GPIO3B_IOMUX_L_GPIO3B0_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_L_GPIO3B0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3B_IOMUX_L_GPIO3B1_SEL_SHIFT               (4U)
#define GRF_GPIO3B_IOMUX_L_GPIO3B1_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_L_GPIO3B1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3B_IOMUX_L_GPIO3B2_SEL_SHIFT               (8U)
#define GRF_GPIO3B_IOMUX_L_GPIO3B2_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_L_GPIO3B2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3B_IOMUX_L_GPIO3B3_SEL_SHIFT               (12U)
#define GRF_GPIO3B_IOMUX_L_GPIO3B3_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_L_GPIO3B3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3B_IOMUX_H */
#define GRF_GPIO3B_IOMUX_H_OFFSET                          (0x4CU)
#define GRF_GPIO3B_IOMUX_H_GPIO3B4_SEL_SHIFT               (0U)
#define GRF_GPIO3B_IOMUX_H_GPIO3B4_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_H_GPIO3B4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3B_IOMUX_H_GPIO3B5_SEL_SHIFT               (4U)
#define GRF_GPIO3B_IOMUX_H_GPIO3B5_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_H_GPIO3B5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3B_IOMUX_H_GPIO3B6_SEL_SHIFT               (8U)
#define GRF_GPIO3B_IOMUX_H_GPIO3B6_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_H_GPIO3B6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3B_IOMUX_H_GPIO3B7_SEL_SHIFT               (12U)
#define GRF_GPIO3B_IOMUX_H_GPIO3B7_SEL_MASK                (0xFU << GRF_GPIO3B_IOMUX_H_GPIO3B7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3C_IOMUX_L */
#define GRF_GPIO3C_IOMUX_L_OFFSET                          (0x50U)
#define GRF_GPIO3C_IOMUX_L_GPIO3C0_SEL_SHIFT               (0U)
#define GRF_GPIO3C_IOMUX_L_GPIO3C0_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_L_GPIO3C0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3C_IOMUX_L_GPIO3C1_SEL_SHIFT               (4U)
#define GRF_GPIO3C_IOMUX_L_GPIO3C1_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_L_GPIO3C1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3C_IOMUX_L_GPIO3C2_SEL_SHIFT               (8U)
#define GRF_GPIO3C_IOMUX_L_GPIO3C2_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_L_GPIO3C2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3C_IOMUX_L_GPIO3C3_SEL_SHIFT               (12U)
#define GRF_GPIO3C_IOMUX_L_GPIO3C3_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_L_GPIO3C3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3C_IOMUX_H */
#define GRF_GPIO3C_IOMUX_H_OFFSET                          (0x54U)
#define GRF_GPIO3C_IOMUX_H_GPIO3C4_SEL_SHIFT               (0U)
#define GRF_GPIO3C_IOMUX_H_GPIO3C4_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_H_GPIO3C4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3C_IOMUX_H_GPIO3C5_SEL_SHIFT               (4U)
#define GRF_GPIO3C_IOMUX_H_GPIO3C5_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_H_GPIO3C5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3C_IOMUX_H_GPIO3C6_SEL_SHIFT               (8U)
#define GRF_GPIO3C_IOMUX_H_GPIO3C6_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_H_GPIO3C6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3C_IOMUX_H_GPIO3C7_SEL_SHIFT               (12U)
#define GRF_GPIO3C_IOMUX_H_GPIO3C7_SEL_MASK                (0xFU << GRF_GPIO3C_IOMUX_H_GPIO3C7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3D_IOMUX_L */
#define GRF_GPIO3D_IOMUX_L_OFFSET                          (0x58U)
#define GRF_GPIO3D_IOMUX_L_GPIO3D0_SEL_SHIFT               (0U)
#define GRF_GPIO3D_IOMUX_L_GPIO3D0_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_L_GPIO3D0_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3D_IOMUX_L_GPIO3D1_SEL_SHIFT               (4U)
#define GRF_GPIO3D_IOMUX_L_GPIO3D1_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_L_GPIO3D1_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3D_IOMUX_L_GPIO3D2_SEL_SHIFT               (8U)
#define GRF_GPIO3D_IOMUX_L_GPIO3D2_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_L_GPIO3D2_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3D_IOMUX_L_GPIO3D3_SEL_SHIFT               (12U)
#define GRF_GPIO3D_IOMUX_L_GPIO3D3_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_L_GPIO3D3_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO3D_IOMUX_H */
#define GRF_GPIO3D_IOMUX_H_OFFSET                          (0x5CU)
#define GRF_GPIO3D_IOMUX_H_GPIO3D4_SEL_SHIFT               (0U)
#define GRF_GPIO3D_IOMUX_H_GPIO3D4_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_H_GPIO3D4_SEL_SHIFT)               /* 0x0000000F */
#define GRF_GPIO3D_IOMUX_H_GPIO3D5_SEL_SHIFT               (4U)
#define GRF_GPIO3D_IOMUX_H_GPIO3D5_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_H_GPIO3D5_SEL_SHIFT)               /* 0x000000F0 */
#define GRF_GPIO3D_IOMUX_H_GPIO3D6_SEL_SHIFT               (8U)
#define GRF_GPIO3D_IOMUX_H_GPIO3D6_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_H_GPIO3D6_SEL_SHIFT)               /* 0x00000F00 */
#define GRF_GPIO3D_IOMUX_H_GPIO3D7_SEL_SHIFT               (12U)
#define GRF_GPIO3D_IOMUX_H_GPIO3D7_SEL_MASK                (0xFU << GRF_GPIO3D_IOMUX_H_GPIO3D7_SEL_SHIFT)               /* 0x0000F000 */
/* GPIO1A_P */
#define GRF_GPIO1A_P_OFFSET                                (0x60U)
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
#define GRF_GPIO1B_P_OFFSET                                (0x64U)
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
#define GRF_GPIO1C_P_OFFSET                                (0x68U)
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
/* GPIO1D_P */
#define GRF_GPIO1D_P_OFFSET                                (0x6CU)
#define GRF_GPIO1D_P_GPIO1D0_P_SHIFT                       (0U)
#define GRF_GPIO1D_P_GPIO1D0_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1D_P_GPIO1D1_P_SHIFT                       (2U)
#define GRF_GPIO1D_P_GPIO1D1_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1D_P_GPIO1D2_P_SHIFT                       (4U)
#define GRF_GPIO1D_P_GPIO1D2_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1D_P_GPIO1D3_P_SHIFT                       (6U)
#define GRF_GPIO1D_P_GPIO1D3_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1D_P_GPIO1D4_P_SHIFT                       (8U)
#define GRF_GPIO1D_P_GPIO1D4_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1D_P_GPIO1D5_P_SHIFT                       (10U)
#define GRF_GPIO1D_P_GPIO1D5_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1D_P_GPIO1D6_P_SHIFT                       (12U)
#define GRF_GPIO1D_P_GPIO1D6_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1D_P_GPIO1D7_P_SHIFT                       (14U)
#define GRF_GPIO1D_P_GPIO1D7_P_MASK                        (0x3U << GRF_GPIO1D_P_GPIO1D7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO2A_P */
#define GRF_GPIO2A_P_OFFSET                                (0x70U)
#define GRF_GPIO2A_P_GPIO2A0_P_SHIFT                       (0U)
#define GRF_GPIO2A_P_GPIO2A0_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2A_P_GPIO2A1_P_SHIFT                       (2U)
#define GRF_GPIO2A_P_GPIO2A1_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2A_P_GPIO2A2_P_SHIFT                       (4U)
#define GRF_GPIO2A_P_GPIO2A2_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2A_P_GPIO2A3_P_SHIFT                       (6U)
#define GRF_GPIO2A_P_GPIO2A3_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO2A_P_GPIO2A4_P_SHIFT                       (8U)
#define GRF_GPIO2A_P_GPIO2A4_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO2A_P_GPIO2A5_P_SHIFT                       (10U)
#define GRF_GPIO2A_P_GPIO2A5_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO2A_P_GPIO2A6_P_SHIFT                       (12U)
#define GRF_GPIO2A_P_GPIO2A6_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO2A_P_GPIO2A7_P_SHIFT                       (14U)
#define GRF_GPIO2A_P_GPIO2A7_P_MASK                        (0x3U << GRF_GPIO2A_P_GPIO2A7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO2B_P */
#define GRF_GPIO2B_P_OFFSET                                (0x74U)
#define GRF_GPIO2B_P_GPIO2B0_P_SHIFT                       (0U)
#define GRF_GPIO2B_P_GPIO2B0_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2B_P_GPIO2B1_P_SHIFT                       (2U)
#define GRF_GPIO2B_P_GPIO2B1_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2B_P_GPIO2B2_P_SHIFT                       (4U)
#define GRF_GPIO2B_P_GPIO2B2_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2B_P_GPIO2B3_P_SHIFT                       (6U)
#define GRF_GPIO2B_P_GPIO2B3_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO2B_P_GPIO2B4_P_SHIFT                       (8U)
#define GRF_GPIO2B_P_GPIO2B4_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO2B_P_GPIO2B5_P_SHIFT                       (10U)
#define GRF_GPIO2B_P_GPIO2B5_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO2B_P_GPIO2B6_P_SHIFT                       (12U)
#define GRF_GPIO2B_P_GPIO2B6_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO2B_P_GPIO2B7_P_SHIFT                       (14U)
#define GRF_GPIO2B_P_GPIO2B7_P_MASK                        (0x3U << GRF_GPIO2B_P_GPIO2B7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO2C_P */
#define GRF_GPIO2C_P_OFFSET                                (0x78U)
#define GRF_GPIO2C_P_GPIO2C0_P_SHIFT                       (0U)
#define GRF_GPIO2C_P_GPIO2C0_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2C_P_GPIO2C1_P_SHIFT                       (2U)
#define GRF_GPIO2C_P_GPIO2C1_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2C_P_GPIO2C2_P_SHIFT                       (4U)
#define GRF_GPIO2C_P_GPIO2C2_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2C_P_GPIO2C3_P_SHIFT                       (6U)
#define GRF_GPIO2C_P_GPIO2C3_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO2C_P_GPIO2C4_P_SHIFT                       (8U)
#define GRF_GPIO2C_P_GPIO2C4_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO2C_P_GPIO2C5_P_SHIFT                       (10U)
#define GRF_GPIO2C_P_GPIO2C5_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO2C_P_GPIO2C6_P_SHIFT                       (12U)
#define GRF_GPIO2C_P_GPIO2C6_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO2C_P_GPIO2C7_P_SHIFT                       (14U)
#define GRF_GPIO2C_P_GPIO2C7_P_MASK                        (0x3U << GRF_GPIO2C_P_GPIO2C7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO3A_P */
#define GRF_GPIO3A_P_OFFSET                                (0x80U)
#define GRF_GPIO3A_P_GPIO3A0_P_SHIFT                       (0U)
#define GRF_GPIO3A_P_GPIO3A0_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3A_P_GPIO3A1_P_SHIFT                       (2U)
#define GRF_GPIO3A_P_GPIO3A1_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3A_P_GPIO3A2_P_SHIFT                       (4U)
#define GRF_GPIO3A_P_GPIO3A2_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3A_P_GPIO3A3_P_SHIFT                       (6U)
#define GRF_GPIO3A_P_GPIO3A3_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3A_P_GPIO3A4_P_SHIFT                       (8U)
#define GRF_GPIO3A_P_GPIO3A4_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3A_P_GPIO3A5_P_SHIFT                       (10U)
#define GRF_GPIO3A_P_GPIO3A5_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3A_P_GPIO3A6_P_SHIFT                       (12U)
#define GRF_GPIO3A_P_GPIO3A6_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3A_P_GPIO3A7_P_SHIFT                       (14U)
#define GRF_GPIO3A_P_GPIO3A7_P_MASK                        (0x3U << GRF_GPIO3A_P_GPIO3A7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO3B_P */
#define GRF_GPIO3B_P_OFFSET                                (0x84U)
#define GRF_GPIO3B_P_GPIO3B0_P_SHIFT                       (0U)
#define GRF_GPIO3B_P_GPIO3B0_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3B_P_GPIO3B1_P_SHIFT                       (2U)
#define GRF_GPIO3B_P_GPIO3B1_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3B_P_GPIO3B2_P_SHIFT                       (4U)
#define GRF_GPIO3B_P_GPIO3B2_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3B_P_GPIO3B3_P_SHIFT                       (6U)
#define GRF_GPIO3B_P_GPIO3B3_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3B_P_GPIO3B4_P_SHIFT                       (8U)
#define GRF_GPIO3B_P_GPIO3B4_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3B_P_GPIO3B5_P_SHIFT                       (10U)
#define GRF_GPIO3B_P_GPIO3B5_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3B_P_GPIO3B6_P_SHIFT                       (12U)
#define GRF_GPIO3B_P_GPIO3B6_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3B_P_GPIO3B7_P_SHIFT                       (14U)
#define GRF_GPIO3B_P_GPIO3B7_P_MASK                        (0x3U << GRF_GPIO3B_P_GPIO3B7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO3C_P */
#define GRF_GPIO3C_P_OFFSET                                (0x88U)
#define GRF_GPIO3C_P_GPIO3C0_P_SHIFT                       (0U)
#define GRF_GPIO3C_P_GPIO3C0_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3C_P_GPIO3C1_P_SHIFT                       (2U)
#define GRF_GPIO3C_P_GPIO3C1_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3C_P_GPIO3C2_P_SHIFT                       (4U)
#define GRF_GPIO3C_P_GPIO3C2_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3C_P_GPIO3C3_P_SHIFT                       (6U)
#define GRF_GPIO3C_P_GPIO3C3_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3C_P_GPIO3C4_P_SHIFT                       (8U)
#define GRF_GPIO3C_P_GPIO3C4_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3C_P_GPIO3C5_P_SHIFT                       (10U)
#define GRF_GPIO3C_P_GPIO3C5_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3C_P_GPIO3C6_P_SHIFT                       (12U)
#define GRF_GPIO3C_P_GPIO3C6_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3C_P_GPIO3C7_P_SHIFT                       (14U)
#define GRF_GPIO3C_P_GPIO3C7_P_MASK                        (0x3U << GRF_GPIO3C_P_GPIO3C7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO3D_P */
#define GRF_GPIO3D_P_OFFSET                                (0x8CU)
#define GRF_GPIO3D_P_GPIO3D0_P_SHIFT                       (0U)
#define GRF_GPIO3D_P_GPIO3D0_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3D_P_GPIO3D1_P_SHIFT                       (2U)
#define GRF_GPIO3D_P_GPIO3D1_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3D_P_GPIO3D2_P_SHIFT                       (4U)
#define GRF_GPIO3D_P_GPIO3D2_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3D_P_GPIO3D3_P_SHIFT                       (6U)
#define GRF_GPIO3D_P_GPIO3D3_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D3_P_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3D_P_GPIO3D4_P_SHIFT                       (8U)
#define GRF_GPIO3D_P_GPIO3D4_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D4_P_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3D_P_GPIO3D5_P_SHIFT                       (10U)
#define GRF_GPIO3D_P_GPIO3D5_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D5_P_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3D_P_GPIO3D6_P_SHIFT                       (12U)
#define GRF_GPIO3D_P_GPIO3D6_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D6_P_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3D_P_GPIO3D7_P_SHIFT                       (14U)
#define GRF_GPIO3D_P_GPIO3D7_P_MASK                        (0x3U << GRF_GPIO3D_P_GPIO3D7_P_SHIFT)                       /* 0x0000C000 */
/* GPIO1A_SR */
#define GRF_GPIO1A_SR_OFFSET                               (0x90U)
#define GRF_GPIO1A_SR_GPIO1A0_SR_SHIFT                     (0U)
#define GRF_GPIO1A_SR_GPIO1A0_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1A_SR_GPIO1A1_SR_SHIFT                     (1U)
#define GRF_GPIO1A_SR_GPIO1A1_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1A_SR_GPIO1A2_SR_SHIFT                     (2U)
#define GRF_GPIO1A_SR_GPIO1A2_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1A_SR_GPIO1A3_SR_SHIFT                     (3U)
#define GRF_GPIO1A_SR_GPIO1A3_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1A_SR_GPIO1A4_SR_SHIFT                     (4U)
#define GRF_GPIO1A_SR_GPIO1A4_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1A_SR_GPIO1A5_SR_SHIFT                     (5U)
#define GRF_GPIO1A_SR_GPIO1A5_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1A_SR_GPIO1A6_SR_SHIFT                     (6U)
#define GRF_GPIO1A_SR_GPIO1A6_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1A_SR_GPIO1A7_SR_SHIFT                     (7U)
#define GRF_GPIO1A_SR_GPIO1A7_SR_MASK                      (0x1U << GRF_GPIO1A_SR_GPIO1A7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO1B_SR */
#define GRF_GPIO1B_SR_OFFSET                               (0x94U)
#define GRF_GPIO1B_SR_GPIO1B0_SR_SHIFT                     (0U)
#define GRF_GPIO1B_SR_GPIO1B0_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1B_SR_GPIO1B1_SR_SHIFT                     (1U)
#define GRF_GPIO1B_SR_GPIO1B1_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1B_SR_GPIO1B2_SR_SHIFT                     (2U)
#define GRF_GPIO1B_SR_GPIO1B2_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1B_SR_GPIO1B3_SR_SHIFT                     (3U)
#define GRF_GPIO1B_SR_GPIO1B3_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1B_SR_GPIO1B4_SR_SHIFT                     (4U)
#define GRF_GPIO1B_SR_GPIO1B4_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1B_SR_GPIO1B5_SR_SHIFT                     (5U)
#define GRF_GPIO1B_SR_GPIO1B5_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1B_SR_GPIO1B6_SR_SHIFT                     (6U)
#define GRF_GPIO1B_SR_GPIO1B6_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1B_SR_GPIO1B7_SR_SHIFT                     (7U)
#define GRF_GPIO1B_SR_GPIO1B7_SR_MASK                      (0x1U << GRF_GPIO1B_SR_GPIO1B7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO1C_SR */
#define GRF_GPIO1C_SR_OFFSET                               (0x98U)
#define GRF_GPIO1C_SR_GPIO1C0_SR_SHIFT                     (0U)
#define GRF_GPIO1C_SR_GPIO1C0_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1C_SR_GPIO1C1_SR_SHIFT                     (1U)
#define GRF_GPIO1C_SR_GPIO1C1_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1C_SR_GPIO1C2_SR_SHIFT                     (2U)
#define GRF_GPIO1C_SR_GPIO1C2_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1C_SR_GPIO1C3_SR_SHIFT                     (3U)
#define GRF_GPIO1C_SR_GPIO1C3_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1C_SR_GPIO1C4_SR_SHIFT                     (4U)
#define GRF_GPIO1C_SR_GPIO1C4_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1C_SR_GPIO1C5_SR_SHIFT                     (5U)
#define GRF_GPIO1C_SR_GPIO1C5_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1C_SR_GPIO1C6_SR_SHIFT                     (6U)
#define GRF_GPIO1C_SR_GPIO1C6_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1C_SR_GPIO1C7_SR_SHIFT                     (7U)
#define GRF_GPIO1C_SR_GPIO1C7_SR_MASK                      (0x1U << GRF_GPIO1C_SR_GPIO1C7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO1D_SR */
#define GRF_GPIO1D_SR_OFFSET                               (0x9CU)
#define GRF_GPIO1D_SR_GPIO1D0_SR_SHIFT                     (0U)
#define GRF_GPIO1D_SR_GPIO1D0_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO1D_SR_GPIO1D1_SR_SHIFT                     (1U)
#define GRF_GPIO1D_SR_GPIO1D1_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO1D_SR_GPIO1D2_SR_SHIFT                     (2U)
#define GRF_GPIO1D_SR_GPIO1D2_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO1D_SR_GPIO1D3_SR_SHIFT                     (3U)
#define GRF_GPIO1D_SR_GPIO1D3_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO1D_SR_GPIO1D4_SR_SHIFT                     (4U)
#define GRF_GPIO1D_SR_GPIO1D4_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO1D_SR_GPIO1D5_SR_SHIFT                     (5U)
#define GRF_GPIO1D_SR_GPIO1D5_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO1D_SR_GPIO1D6_SR_SHIFT                     (6U)
#define GRF_GPIO1D_SR_GPIO1D6_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO1D_SR_GPIO1D7_SR_SHIFT                     (7U)
#define GRF_GPIO1D_SR_GPIO1D7_SR_MASK                      (0x1U << GRF_GPIO1D_SR_GPIO1D7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO2A_SR */
#define GRF_GPIO2A_SR_OFFSET                               (0xA0U)
#define GRF_GPIO2A_SR_GPIO2A0_SR_SHIFT                     (0U)
#define GRF_GPIO2A_SR_GPIO2A0_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO2A_SR_GPIO2A1_SR_SHIFT                     (1U)
#define GRF_GPIO2A_SR_GPIO2A1_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO2A_SR_GPIO2A2_SR_SHIFT                     (2U)
#define GRF_GPIO2A_SR_GPIO2A2_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO2A_SR_GPIO2A3_SR_SHIFT                     (3U)
#define GRF_GPIO2A_SR_GPIO2A3_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO2A_SR_GPIO2A4_SR_SHIFT                     (4U)
#define GRF_GPIO2A_SR_GPIO2A4_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO2A_SR_GPIO2A5_SR_SHIFT                     (5U)
#define GRF_GPIO2A_SR_GPIO2A5_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO2A_SR_GPIO2A6_SR_SHIFT                     (6U)
#define GRF_GPIO2A_SR_GPIO2A6_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO2A_SR_GPIO2A7_SR_SHIFT                     (7U)
#define GRF_GPIO2A_SR_GPIO2A7_SR_MASK                      (0x1U << GRF_GPIO2A_SR_GPIO2A7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO2B_SR */
#define GRF_GPIO2B_SR_OFFSET                               (0xA4U)
#define GRF_GPIO2B_SR_GPIO2B0_SR_SHIFT                     (0U)
#define GRF_GPIO2B_SR_GPIO2B0_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO2B_SR_GPIO2B1_SR_SHIFT                     (1U)
#define GRF_GPIO2B_SR_GPIO2B1_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO2B_SR_GPIO2B2_SR_SHIFT                     (2U)
#define GRF_GPIO2B_SR_GPIO2B2_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO2B_SR_GPIO2B3_SR_SHIFT                     (3U)
#define GRF_GPIO2B_SR_GPIO2B3_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO2B_SR_GPIO2B4_SR_SHIFT                     (4U)
#define GRF_GPIO2B_SR_GPIO2B4_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO2B_SR_GPIO2B5_SR_SHIFT                     (5U)
#define GRF_GPIO2B_SR_GPIO2B5_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO2B_SR_GPIO2B6_SR_SHIFT                     (6U)
#define GRF_GPIO2B_SR_GPIO2B6_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO2B_SR_GPIO2B7_SR_SHIFT                     (7U)
#define GRF_GPIO2B_SR_GPIO2B7_SR_MASK                      (0x1U << GRF_GPIO2B_SR_GPIO2B7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO2C_SR */
#define GRF_GPIO2C_SR_OFFSET                               (0xA8U)
#define GRF_GPIO2C_SR_GPIO2C0_SR_SHIFT                     (0U)
#define GRF_GPIO2C_SR_GPIO2C0_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO2C_SR_GPIO2C1_SR_SHIFT                     (1U)
#define GRF_GPIO2C_SR_GPIO2C1_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO2C_SR_GPIO2C2_SR_SHIFT                     (2U)
#define GRF_GPIO2C_SR_GPIO2C2_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO2C_SR_GPIO2C3_SR_SHIFT                     (3U)
#define GRF_GPIO2C_SR_GPIO2C3_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO2C_SR_GPIO2C4_SR_SHIFT                     (4U)
#define GRF_GPIO2C_SR_GPIO2C4_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO2C_SR_GPIO2C5_SR_SHIFT                     (5U)
#define GRF_GPIO2C_SR_GPIO2C5_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO2C_SR_GPIO2C6_SR_SHIFT                     (6U)
#define GRF_GPIO2C_SR_GPIO2C6_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO2C_SR_GPIO2C7_SR_SHIFT                     (7U)
#define GRF_GPIO2C_SR_GPIO2C7_SR_MASK                      (0x1U << GRF_GPIO2C_SR_GPIO2C7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO3A_SR */
#define GRF_GPIO3A_SR_OFFSET                               (0xB0U)
#define GRF_GPIO3A_SR_GPIO3A0_SR_SHIFT                     (0U)
#define GRF_GPIO3A_SR_GPIO3A0_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO3A_SR_GPIO3A1_SR_SHIFT                     (1U)
#define GRF_GPIO3A_SR_GPIO3A1_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO3A_SR_GPIO3A2_SR_SHIFT                     (2U)
#define GRF_GPIO3A_SR_GPIO3A2_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO3A_SR_GPIO3A3_SR_SHIFT                     (3U)
#define GRF_GPIO3A_SR_GPIO3A3_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO3A_SR_GPIO3A4_SR_SHIFT                     (4U)
#define GRF_GPIO3A_SR_GPIO3A4_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO3A_SR_GPIO3A5_SR_SHIFT                     (5U)
#define GRF_GPIO3A_SR_GPIO3A5_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO3A_SR_GPIO3A6_SR_SHIFT                     (6U)
#define GRF_GPIO3A_SR_GPIO3A6_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO3A_SR_GPIO3A7_SR_SHIFT                     (7U)
#define GRF_GPIO3A_SR_GPIO3A7_SR_MASK                      (0x1U << GRF_GPIO3A_SR_GPIO3A7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO3B_SR */
#define GRF_GPIO3B_SR_OFFSET                               (0xB4U)
#define GRF_GPIO3B_SR_GPIO3B0_SR_SHIFT                     (0U)
#define GRF_GPIO3B_SR_GPIO3B0_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO3B_SR_GPIO3B1_SR_SHIFT                     (1U)
#define GRF_GPIO3B_SR_GPIO3B1_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO3B_SR_GPIO3B2_SR_SHIFT                     (2U)
#define GRF_GPIO3B_SR_GPIO3B2_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO3B_SR_GPIO3B3_SR_SHIFT                     (3U)
#define GRF_GPIO3B_SR_GPIO3B3_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO3B_SR_GPIO3B4_SR_SHIFT                     (4U)
#define GRF_GPIO3B_SR_GPIO3B4_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO3B_SR_GPIO3B5_SR_SHIFT                     (5U)
#define GRF_GPIO3B_SR_GPIO3B5_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO3B_SR_GPIO3B6_SR_SHIFT                     (6U)
#define GRF_GPIO3B_SR_GPIO3B6_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO3B_SR_GPIO3B7_SR_SHIFT                     (7U)
#define GRF_GPIO3B_SR_GPIO3B7_SR_MASK                      (0x1U << GRF_GPIO3B_SR_GPIO3B7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO3C_SR */
#define GRF_GPIO3C_SR_OFFSET                               (0xB8U)
#define GRF_GPIO3C_SR_GPIO3C0_SR_SHIFT                     (0U)
#define GRF_GPIO3C_SR_GPIO3C0_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO3C_SR_GPIO3C1_SR_SHIFT                     (1U)
#define GRF_GPIO3C_SR_GPIO3C1_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO3C_SR_GPIO3C2_SR_SHIFT                     (2U)
#define GRF_GPIO3C_SR_GPIO3C2_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO3C_SR_GPIO3C3_SR_SHIFT                     (3U)
#define GRF_GPIO3C_SR_GPIO3C3_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO3C_SR_GPIO3C4_SR_SHIFT                     (4U)
#define GRF_GPIO3C_SR_GPIO3C4_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO3C_SR_GPIO3C5_SR_SHIFT                     (5U)
#define GRF_GPIO3C_SR_GPIO3C5_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO3C_SR_GPIO3C6_SR_SHIFT                     (6U)
#define GRF_GPIO3C_SR_GPIO3C6_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO3C_SR_GPIO3C7_SR_SHIFT                     (7U)
#define GRF_GPIO3C_SR_GPIO3C7_SR_MASK                      (0x1U << GRF_GPIO3C_SR_GPIO3C7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO3D_SR */
#define GRF_GPIO3D_SR_OFFSET                               (0xBCU)
#define GRF_GPIO3D_SR_GPIO3D0_SR_SHIFT                     (0U)
#define GRF_GPIO3D_SR_GPIO3D0_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D0_SR_SHIFT)                     /* 0x00000001 */
#define GRF_GPIO3D_SR_GPIO3D1_SR_SHIFT                     (1U)
#define GRF_GPIO3D_SR_GPIO3D1_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D1_SR_SHIFT)                     /* 0x00000002 */
#define GRF_GPIO3D_SR_GPIO3D2_SR_SHIFT                     (2U)
#define GRF_GPIO3D_SR_GPIO3D2_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D2_SR_SHIFT)                     /* 0x00000004 */
#define GRF_GPIO3D_SR_GPIO3D3_SR_SHIFT                     (3U)
#define GRF_GPIO3D_SR_GPIO3D3_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D3_SR_SHIFT)                     /* 0x00000008 */
#define GRF_GPIO3D_SR_GPIO3D4_SR_SHIFT                     (4U)
#define GRF_GPIO3D_SR_GPIO3D4_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D4_SR_SHIFT)                     /* 0x00000010 */
#define GRF_GPIO3D_SR_GPIO3D5_SR_SHIFT                     (5U)
#define GRF_GPIO3D_SR_GPIO3D5_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D5_SR_SHIFT)                     /* 0x00000020 */
#define GRF_GPIO3D_SR_GPIO3D6_SR_SHIFT                     (6U)
#define GRF_GPIO3D_SR_GPIO3D6_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D6_SR_SHIFT)                     /* 0x00000040 */
#define GRF_GPIO3D_SR_GPIO3D7_SR_SHIFT                     (7U)
#define GRF_GPIO3D_SR_GPIO3D7_SR_MASK                      (0x1U << GRF_GPIO3D_SR_GPIO3D7_SR_SHIFT)                     /* 0x00000080 */
/* GPIO1A_SMT */
#define GRF_GPIO1A_SMT_OFFSET                              (0xC0U)
#define GRF_GPIO1A_SMT_GPIO1A0_SMT_SHIFT                   (0U)
#define GRF_GPIO1A_SMT_GPIO1A0_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO1A_SMT_GPIO1A1_SMT_SHIFT                   (1U)
#define GRF_GPIO1A_SMT_GPIO1A1_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO1A_SMT_GPIO1A2_SMT_SHIFT                   (2U)
#define GRF_GPIO1A_SMT_GPIO1A2_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO1A_SMT_GPIO1A3_SMT_SHIFT                   (3U)
#define GRF_GPIO1A_SMT_GPIO1A3_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO1A_SMT_GPIO1A4_SMT_SHIFT                   (4U)
#define GRF_GPIO1A_SMT_GPIO1A4_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO1A_SMT_GPIO1A5_SMT_SHIFT                   (5U)
#define GRF_GPIO1A_SMT_GPIO1A5_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO1A_SMT_GPIO1A6_SMT_SHIFT                   (6U)
#define GRF_GPIO1A_SMT_GPIO1A6_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO1A_SMT_GPIO1A7_SMT_SHIFT                   (7U)
#define GRF_GPIO1A_SMT_GPIO1A7_SMT_MASK                    (0x1U << GRF_GPIO1A_SMT_GPIO1A7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO1B_SMT */
#define GRF_GPIO1B_SMT_OFFSET                              (0xC4U)
#define GRF_GPIO1B_SMT_GPIO1B0_SMT_SHIFT                   (0U)
#define GRF_GPIO1B_SMT_GPIO1B0_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO1B_SMT_GPIO1B1_SMT_SHIFT                   (1U)
#define GRF_GPIO1B_SMT_GPIO1B1_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO1B_SMT_GPIO1B2_SMT_SHIFT                   (2U)
#define GRF_GPIO1B_SMT_GPIO1B2_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO1B_SMT_GPIO1B3_SMT_SHIFT                   (3U)
#define GRF_GPIO1B_SMT_GPIO1B3_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO1B_SMT_GPIO1B4_SMT_SHIFT                   (4U)
#define GRF_GPIO1B_SMT_GPIO1B4_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO1B_SMT_GPIO1B5_SMT_SHIFT                   (5U)
#define GRF_GPIO1B_SMT_GPIO1B5_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO1B_SMT_GPIO1B6_SMT_SHIFT                   (6U)
#define GRF_GPIO1B_SMT_GPIO1B6_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO1B_SMT_GPIO1B7_SMT_SHIFT                   (7U)
#define GRF_GPIO1B_SMT_GPIO1B7_SMT_MASK                    (0x1U << GRF_GPIO1B_SMT_GPIO1B7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO1C_SMT */
#define GRF_GPIO1C_SMT_OFFSET                              (0xC8U)
#define GRF_GPIO1C_SMT_GPIO1C0_SMT_SHIFT                   (0U)
#define GRF_GPIO1C_SMT_GPIO1C0_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO1C_SMT_GPIO1C1_SMT_SHIFT                   (1U)
#define GRF_GPIO1C_SMT_GPIO1C1_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO1C_SMT_GPIO1C2_SMT_SHIFT                   (2U)
#define GRF_GPIO1C_SMT_GPIO1C2_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO1C_SMT_GPIO1C3_SMT_SHIFT                   (3U)
#define GRF_GPIO1C_SMT_GPIO1C3_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO1C_SMT_GPIO1C4_SMT_SHIFT                   (4U)
#define GRF_GPIO1C_SMT_GPIO1C4_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO1C_SMT_GPIO1C5_SMT_SHIFT                   (5U)
#define GRF_GPIO1C_SMT_GPIO1C5_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO1C_SMT_GPIO1C6_SMT_SHIFT                   (6U)
#define GRF_GPIO1C_SMT_GPIO1C6_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO1C_SMT_GPIO1C7_SMT_SHIFT                   (7U)
#define GRF_GPIO1C_SMT_GPIO1C7_SMT_MASK                    (0x1U << GRF_GPIO1C_SMT_GPIO1C7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO1D_SMT */
#define GRF_GPIO1D_SMT_OFFSET                              (0xCCU)
#define GRF_GPIO1D_SMT_GPIO1D0_SMT_SHIFT                   (0U)
#define GRF_GPIO1D_SMT_GPIO1D0_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO1D_SMT_GPIO1D1_SMT_SHIFT                   (1U)
#define GRF_GPIO1D_SMT_GPIO1D1_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO1D_SMT_GPIO1D2_SMT_SHIFT                   (2U)
#define GRF_GPIO1D_SMT_GPIO1D2_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO1D_SMT_GPIO1D3_SMT_SHIFT                   (3U)
#define GRF_GPIO1D_SMT_GPIO1D3_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO1D_SMT_GPIO1D4_SMT_SHIFT                   (4U)
#define GRF_GPIO1D_SMT_GPIO1D4_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO1D_SMT_GPIO1D5_SMT_SHIFT                   (5U)
#define GRF_GPIO1D_SMT_GPIO1D5_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO1D_SMT_GPIO1D6_SMT_SHIFT                   (6U)
#define GRF_GPIO1D_SMT_GPIO1D6_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO1D_SMT_GPIO1D7_SMT_SHIFT                   (7U)
#define GRF_GPIO1D_SMT_GPIO1D7_SMT_MASK                    (0x1U << GRF_GPIO1D_SMT_GPIO1D7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO2A_SMT */
#define GRF_GPIO2A_SMT_OFFSET                              (0xD0U)
#define GRF_GPIO2A_SMT_GPIO2A0_SMT_SHIFT                   (0U)
#define GRF_GPIO2A_SMT_GPIO2A0_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO2A_SMT_GPIO2A1_SMT_SHIFT                   (1U)
#define GRF_GPIO2A_SMT_GPIO2A1_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO2A_SMT_GPIO2A2_SMT_SHIFT                   (2U)
#define GRF_GPIO2A_SMT_GPIO2A2_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO2A_SMT_GPIO2A3_SMT_SHIFT                   (3U)
#define GRF_GPIO2A_SMT_GPIO2A3_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO2A_SMT_GPIO2A4_SMT_SHIFT                   (4U)
#define GRF_GPIO2A_SMT_GPIO2A4_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO2A_SMT_GPIO2A5_SMT_SHIFT                   (5U)
#define GRF_GPIO2A_SMT_GPIO2A5_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO2A_SMT_GPIO2A6_SMT_SHIFT                   (6U)
#define GRF_GPIO2A_SMT_GPIO2A6_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO2A_SMT_GPIO2A7_SMT_SHIFT                   (7U)
#define GRF_GPIO2A_SMT_GPIO2A7_SMT_MASK                    (0x1U << GRF_GPIO2A_SMT_GPIO2A7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO2B_SMT */
#define GRF_GPIO2B_SMT_OFFSET                              (0xD4U)
#define GRF_GPIO2B_SMT_GPIO2B0_SMT_SHIFT                   (0U)
#define GRF_GPIO2B_SMT_GPIO2B0_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO2B_SMT_GPIO2B1_SMT_SHIFT                   (1U)
#define GRF_GPIO2B_SMT_GPIO2B1_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO2B_SMT_GPIO2B2_SMT_SHIFT                   (2U)
#define GRF_GPIO2B_SMT_GPIO2B2_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO2B_SMT_GPIO2B3_SMT_SHIFT                   (3U)
#define GRF_GPIO2B_SMT_GPIO2B3_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO2B_SMT_GPIO2B4_SMT_SHIFT                   (4U)
#define GRF_GPIO2B_SMT_GPIO2B4_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO2B_SMT_GPIO2B5_SMT_SHIFT                   (5U)
#define GRF_GPIO2B_SMT_GPIO2B5_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO2B_SMT_GPIO2B6_SMT_SHIFT                   (6U)
#define GRF_GPIO2B_SMT_GPIO2B6_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO2B_SMT_GPIO2B7_SMT_SHIFT                   (7U)
#define GRF_GPIO2B_SMT_GPIO2B7_SMT_MASK                    (0x1U << GRF_GPIO2B_SMT_GPIO2B7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO2C_SMT */
#define GRF_GPIO2C_SMT_OFFSET                              (0xD8U)
#define GRF_GPIO2C_SMT_GPIO2C0_SMT_SHIFT                   (0U)
#define GRF_GPIO2C_SMT_GPIO2C0_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO2C_SMT_GPIO2C1_SMT_SHIFT                   (1U)
#define GRF_GPIO2C_SMT_GPIO2C1_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO2C_SMT_GPIO2C2_SMT_SHIFT                   (2U)
#define GRF_GPIO2C_SMT_GPIO2C2_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO2C_SMT_GPIO2C3_SMT_SHIFT                   (3U)
#define GRF_GPIO2C_SMT_GPIO2C3_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO2C_SMT_GPIO2C4_SMT_SHIFT                   (4U)
#define GRF_GPIO2C_SMT_GPIO2C4_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO2C_SMT_GPIO2C5_SMT_SHIFT                   (5U)
#define GRF_GPIO2C_SMT_GPIO2C5_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO2C_SMT_GPIO2C6_SMT_SHIFT                   (6U)
#define GRF_GPIO2C_SMT_GPIO2C6_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO2C_SMT_GPIO2C7_SMT_SHIFT                   (7U)
#define GRF_GPIO2C_SMT_GPIO2C7_SMT_MASK                    (0x1U << GRF_GPIO2C_SMT_GPIO2C7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO3A_SMT */
#define GRF_GPIO3A_SMT_OFFSET                              (0xE0U)
#define GRF_GPIO3A_SMT_GPIO3A0_SMT_SHIFT                   (0U)
#define GRF_GPIO3A_SMT_GPIO3A0_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO3A_SMT_GPIO3A1_SMT_SHIFT                   (1U)
#define GRF_GPIO3A_SMT_GPIO3A1_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO3A_SMT_GPIO3A2_SMT_SHIFT                   (2U)
#define GRF_GPIO3A_SMT_GPIO3A2_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO3A_SMT_GPIO3A3_SMT_SHIFT                   (3U)
#define GRF_GPIO3A_SMT_GPIO3A3_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO3A_SMT_GPIO3A4_SMT_SHIFT                   (4U)
#define GRF_GPIO3A_SMT_GPIO3A4_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO3A_SMT_GPIO3A5_SMT_SHIFT                   (5U)
#define GRF_GPIO3A_SMT_GPIO3A5_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO3A_SMT_GPIO3A6_SMT_SHIFT                   (6U)
#define GRF_GPIO3A_SMT_GPIO3A6_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO3A_SMT_GPIO3A7_SMT_SHIFT                   (7U)
#define GRF_GPIO3A_SMT_GPIO3A7_SMT_MASK                    (0x1U << GRF_GPIO3A_SMT_GPIO3A7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO3B_SMT */
#define GRF_GPIO3B_SMT_OFFSET                              (0xE4U)
#define GRF_GPIO3B_SMT_GPIO3B0_SMT_SHIFT                   (0U)
#define GRF_GPIO3B_SMT_GPIO3B0_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO3B_SMT_GPIO3B1_SMT_SHIFT                   (1U)
#define GRF_GPIO3B_SMT_GPIO3B1_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO3B_SMT_GPIO3B2_SMT_SHIFT                   (2U)
#define GRF_GPIO3B_SMT_GPIO3B2_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO3B_SMT_GPIO3B3_SMT_SHIFT                   (3U)
#define GRF_GPIO3B_SMT_GPIO3B3_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO3B_SMT_GPIO3B4_SMT_SHIFT                   (4U)
#define GRF_GPIO3B_SMT_GPIO3B4_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO3B_SMT_GPIO3B5_SMT_SHIFT                   (5U)
#define GRF_GPIO3B_SMT_GPIO3B5_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO3B_SMT_GPIO3B6_SMT_SHIFT                   (6U)
#define GRF_GPIO3B_SMT_GPIO3B6_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO3B_SMT_GPIO3B7_SMT_SHIFT                   (7U)
#define GRF_GPIO3B_SMT_GPIO3B7_SMT_MASK                    (0x1U << GRF_GPIO3B_SMT_GPIO3B7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO3C_SMT */
#define GRF_GPIO3C_SMT_OFFSET                              (0xE8U)
#define GRF_GPIO3C_SMT_GPIO3C0_SMT_SHIFT                   (0U)
#define GRF_GPIO3C_SMT_GPIO3C0_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO3C_SMT_GPIO3C1_SMT_SHIFT                   (1U)
#define GRF_GPIO3C_SMT_GPIO3C1_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO3C_SMT_GPIO3C2_SMT_SHIFT                   (2U)
#define GRF_GPIO3C_SMT_GPIO3C2_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO3C_SMT_GPIO3C3_SMT_SHIFT                   (3U)
#define GRF_GPIO3C_SMT_GPIO3C3_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO3C_SMT_GPIO3C4_SMT_SHIFT                   (4U)
#define GRF_GPIO3C_SMT_GPIO3C4_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO3C_SMT_GPIO3C5_SMT_SHIFT                   (5U)
#define GRF_GPIO3C_SMT_GPIO3C5_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO3C_SMT_GPIO3C6_SMT_SHIFT                   (6U)
#define GRF_GPIO3C_SMT_GPIO3C6_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO3C_SMT_GPIO3C7_SMT_SHIFT                   (7U)
#define GRF_GPIO3C_SMT_GPIO3C7_SMT_MASK                    (0x1U << GRF_GPIO3C_SMT_GPIO3C7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO3D_SMT */
#define GRF_GPIO3D_SMT_OFFSET                              (0xECU)
#define GRF_GPIO3D_SMT_GPIO3D0_SMT_SHIFT                   (0U)
#define GRF_GPIO3D_SMT_GPIO3D0_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D0_SMT_SHIFT)                   /* 0x00000001 */
#define GRF_GPIO3D_SMT_GPIO3D1_SMT_SHIFT                   (1U)
#define GRF_GPIO3D_SMT_GPIO3D1_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D1_SMT_SHIFT)                   /* 0x00000002 */
#define GRF_GPIO3D_SMT_GPIO3D2_SMT_SHIFT                   (2U)
#define GRF_GPIO3D_SMT_GPIO3D2_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D2_SMT_SHIFT)                   /* 0x00000004 */
#define GRF_GPIO3D_SMT_GPIO3D3_SMT_SHIFT                   (3U)
#define GRF_GPIO3D_SMT_GPIO3D3_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D3_SMT_SHIFT)                   /* 0x00000008 */
#define GRF_GPIO3D_SMT_GPIO3D4_SMT_SHIFT                   (4U)
#define GRF_GPIO3D_SMT_GPIO3D4_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D4_SMT_SHIFT)                   /* 0x00000010 */
#define GRF_GPIO3D_SMT_GPIO3D5_SMT_SHIFT                   (5U)
#define GRF_GPIO3D_SMT_GPIO3D5_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D5_SMT_SHIFT)                   /* 0x00000020 */
#define GRF_GPIO3D_SMT_GPIO3D6_SMT_SHIFT                   (6U)
#define GRF_GPIO3D_SMT_GPIO3D6_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D6_SMT_SHIFT)                   /* 0x00000040 */
#define GRF_GPIO3D_SMT_GPIO3D7_SMT_SHIFT                   (7U)
#define GRF_GPIO3D_SMT_GPIO3D7_SMT_MASK                    (0x1U << GRF_GPIO3D_SMT_GPIO3D7_SMT_SHIFT)                   /* 0x00000080 */
/* GPIO1A_E */
#define GRF_GPIO1A_E_OFFSET                                (0xF0U)
#define GRF_GPIO1A_E_GPIO1A0_E_SHIFT                       (0U)
#define GRF_GPIO1A_E_GPIO1A0_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1A_E_GPIO1A1_E_SHIFT                       (2U)
#define GRF_GPIO1A_E_GPIO1A1_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1A_E_GPIO1A2_E_SHIFT                       (4U)
#define GRF_GPIO1A_E_GPIO1A2_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1A_E_GPIO1A3_E_SHIFT                       (6U)
#define GRF_GPIO1A_E_GPIO1A3_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1A_E_GPIO1A4_E_SHIFT                       (8U)
#define GRF_GPIO1A_E_GPIO1A4_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1A_E_GPIO1A5_E_SHIFT                       (10U)
#define GRF_GPIO1A_E_GPIO1A5_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1A_E_GPIO1A6_E_SHIFT                       (12U)
#define GRF_GPIO1A_E_GPIO1A6_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1A_E_GPIO1A7_E_SHIFT                       (14U)
#define GRF_GPIO1A_E_GPIO1A7_E_MASK                        (0x3U << GRF_GPIO1A_E_GPIO1A7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO1B_E */
#define GRF_GPIO1B_E_OFFSET                                (0xF4U)
#define GRF_GPIO1B_E_GPIO1B0_E_SHIFT                       (0U)
#define GRF_GPIO1B_E_GPIO1B0_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1B_E_GPIO1B1_E_SHIFT                       (2U)
#define GRF_GPIO1B_E_GPIO1B1_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1B_E_GPIO1B2_E_SHIFT                       (4U)
#define GRF_GPIO1B_E_GPIO1B2_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1B_E_GPIO1B3_E_SHIFT                       (6U)
#define GRF_GPIO1B_E_GPIO1B3_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1B_E_GPIO1B4_E_SHIFT                       (8U)
#define GRF_GPIO1B_E_GPIO1B4_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1B_E_GPIO1B5_E_SHIFT                       (10U)
#define GRF_GPIO1B_E_GPIO1B5_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1B_E_GPIO1B6_E_SHIFT                       (12U)
#define GRF_GPIO1B_E_GPIO1B6_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1B_E_GPIO1B7_E_SHIFT                       (14U)
#define GRF_GPIO1B_E_GPIO1B7_E_MASK                        (0x3U << GRF_GPIO1B_E_GPIO1B7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO1C_E */
#define GRF_GPIO1C_E_OFFSET                                (0xF8U)
#define GRF_GPIO1C_E_GPIO1C0_E_SHIFT                       (0U)
#define GRF_GPIO1C_E_GPIO1C0_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1C_E_GPIO1C1_E_SHIFT                       (2U)
#define GRF_GPIO1C_E_GPIO1C1_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1C_E_GPIO1C2_E_SHIFT                       (4U)
#define GRF_GPIO1C_E_GPIO1C2_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1C_E_GPIO1C3_E_SHIFT                       (6U)
#define GRF_GPIO1C_E_GPIO1C3_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1C_E_GPIO1C4_E_SHIFT                       (8U)
#define GRF_GPIO1C_E_GPIO1C4_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1C_E_GPIO1C5_E_SHIFT                       (10U)
#define GRF_GPIO1C_E_GPIO1C5_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1C_E_GPIO1C6_E_SHIFT                       (12U)
#define GRF_GPIO1C_E_GPIO1C6_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1C_E_GPIO1C7_E_SHIFT                       (14U)
#define GRF_GPIO1C_E_GPIO1C7_E_MASK                        (0x3U << GRF_GPIO1C_E_GPIO1C7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO1D_E */
#define GRF_GPIO1D_E_OFFSET                                (0xFCU)
#define GRF_GPIO1D_E_GPIO1D0_E_SHIFT                       (0U)
#define GRF_GPIO1D_E_GPIO1D0_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO1D_E_GPIO1D1_E_SHIFT                       (2U)
#define GRF_GPIO1D_E_GPIO1D1_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO1D_E_GPIO1D2_E_SHIFT                       (4U)
#define GRF_GPIO1D_E_GPIO1D2_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO1D_E_GPIO1D3_E_SHIFT                       (6U)
#define GRF_GPIO1D_E_GPIO1D3_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO1D_E_GPIO1D4_E_SHIFT                       (8U)
#define GRF_GPIO1D_E_GPIO1D4_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO1D_E_GPIO1D5_E_SHIFT                       (10U)
#define GRF_GPIO1D_E_GPIO1D5_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO1D_E_GPIO1D6_E_SHIFT                       (12U)
#define GRF_GPIO1D_E_GPIO1D6_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO1D_E_GPIO1D7_E_SHIFT                       (14U)
#define GRF_GPIO1D_E_GPIO1D7_E_MASK                        (0x3U << GRF_GPIO1D_E_GPIO1D7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO2A_E */
#define GRF_GPIO2A_E_OFFSET                                (0x100U)
#define GRF_GPIO2A_E_GPIO2A0_E_SHIFT                       (0U)
#define GRF_GPIO2A_E_GPIO2A0_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2A_E_GPIO2A1_E_SHIFT                       (2U)
#define GRF_GPIO2A_E_GPIO2A1_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2A_E_GPIO2A2_E_SHIFT                       (4U)
#define GRF_GPIO2A_E_GPIO2A2_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2A_E_GPIO2A3_E_SHIFT                       (6U)
#define GRF_GPIO2A_E_GPIO2A3_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO2A_E_GPIO2A4_E_SHIFT                       (8U)
#define GRF_GPIO2A_E_GPIO2A4_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO2A_E_GPIO2A5_E_SHIFT                       (10U)
#define GRF_GPIO2A_E_GPIO2A5_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO2A_E_GPIO2A6_E_SHIFT                       (12U)
#define GRF_GPIO2A_E_GPIO2A6_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO2A_E_GPIO2A7_E_SHIFT                       (14U)
#define GRF_GPIO2A_E_GPIO2A7_E_MASK                        (0x3U << GRF_GPIO2A_E_GPIO2A7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO2B_E */
#define GRF_GPIO2B_E_OFFSET                                (0x104U)
#define GRF_GPIO2B_E_GPIO2B0_E_SHIFT                       (0U)
#define GRF_GPIO2B_E_GPIO2B0_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2B_E_GPIO2B1_E_SHIFT                       (2U)
#define GRF_GPIO2B_E_GPIO2B1_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2B_E_GPIO2B2_E_SHIFT                       (4U)
#define GRF_GPIO2B_E_GPIO2B2_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2B_E_GPIO2B3_E_SHIFT                       (6U)
#define GRF_GPIO2B_E_GPIO2B3_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO2B_E_GPIO2B4_E_SHIFT                       (8U)
#define GRF_GPIO2B_E_GPIO2B4_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO2B_E_GPIO2B5_E_SHIFT                       (10U)
#define GRF_GPIO2B_E_GPIO2B5_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO2B_E_GPIO2B6_E_SHIFT                       (12U)
#define GRF_GPIO2B_E_GPIO2B6_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO2B_E_GPIO2B7_E_SHIFT                       (14U)
#define GRF_GPIO2B_E_GPIO2B7_E_MASK                        (0x3U << GRF_GPIO2B_E_GPIO2B7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO2C_E */
#define GRF_GPIO2C_E_OFFSET                                (0x108U)
#define GRF_GPIO2C_E_GPIO2C0_E_SHIFT                       (0U)
#define GRF_GPIO2C_E_GPIO2C0_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2C_E_GPIO2C1_E_SHIFT                       (2U)
#define GRF_GPIO2C_E_GPIO2C1_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2C_E_GPIO2C2_E_SHIFT                       (4U)
#define GRF_GPIO2C_E_GPIO2C2_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2C_E_GPIO2C3_E_SHIFT                       (6U)
#define GRF_GPIO2C_E_GPIO2C3_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO2C_E_GPIO2C4_E_SHIFT                       (8U)
#define GRF_GPIO2C_E_GPIO2C4_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO2C_E_GPIO2C5_E_SHIFT                       (10U)
#define GRF_GPIO2C_E_GPIO2C5_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO2C_E_GPIO2C6_E_SHIFT                       (12U)
#define GRF_GPIO2C_E_GPIO2C6_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO2C_E_GPIO2C7_E_SHIFT                       (14U)
#define GRF_GPIO2C_E_GPIO2C7_E_MASK                        (0x3U << GRF_GPIO2C_E_GPIO2C7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO3A_E */
#define GRF_GPIO3A_E_OFFSET                                (0x110U)
#define GRF_GPIO3A_E_GPIO3A0_E_SHIFT                       (0U)
#define GRF_GPIO3A_E_GPIO3A0_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3A_E_GPIO3A1_E_SHIFT                       (2U)
#define GRF_GPIO3A_E_GPIO3A1_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3A_E_GPIO3A2_E_SHIFT                       (4U)
#define GRF_GPIO3A_E_GPIO3A2_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3A_E_GPIO3A3_E_SHIFT                       (6U)
#define GRF_GPIO3A_E_GPIO3A3_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3A_E_GPIO3A4_E_SHIFT                       (8U)
#define GRF_GPIO3A_E_GPIO3A4_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3A_E_GPIO3A5_E_SHIFT                       (10U)
#define GRF_GPIO3A_E_GPIO3A5_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3A_E_GPIO3A6_E_SHIFT                       (12U)
#define GRF_GPIO3A_E_GPIO3A6_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3A_E_GPIO3A7_E_SHIFT                       (14U)
#define GRF_GPIO3A_E_GPIO3A7_E_MASK                        (0x3U << GRF_GPIO3A_E_GPIO3A7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO3B_E */
#define GRF_GPIO3B_E_OFFSET                                (0x114U)
#define GRF_GPIO3B_E_GPIO3B0_E_SHIFT                       (0U)
#define GRF_GPIO3B_E_GPIO3B0_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3B_E_GPIO3B1_E_SHIFT                       (2U)
#define GRF_GPIO3B_E_GPIO3B1_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3B_E_GPIO3B2_E_SHIFT                       (4U)
#define GRF_GPIO3B_E_GPIO3B2_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3B_E_GPIO3B3_E_SHIFT                       (6U)
#define GRF_GPIO3B_E_GPIO3B3_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3B_E_GPIO3B4_E_SHIFT                       (8U)
#define GRF_GPIO3B_E_GPIO3B4_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3B_E_GPIO3B5_E_SHIFT                       (10U)
#define GRF_GPIO3B_E_GPIO3B5_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3B_E_GPIO3B6_E_SHIFT                       (12U)
#define GRF_GPIO3B_E_GPIO3B6_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3B_E_GPIO3B7_E_SHIFT                       (14U)
#define GRF_GPIO3B_E_GPIO3B7_E_MASK                        (0x3U << GRF_GPIO3B_E_GPIO3B7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO3C_E */
#define GRF_GPIO3C_E_OFFSET                                (0x118U)
#define GRF_GPIO3C_E_GPIO3C0_E_SHIFT                       (0U)
#define GRF_GPIO3C_E_GPIO3C0_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3C_E_GPIO3C1_E_SHIFT                       (2U)
#define GRF_GPIO3C_E_GPIO3C1_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3C_E_GPIO3C2_E_SHIFT                       (4U)
#define GRF_GPIO3C_E_GPIO3C2_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3C_E_GPIO3C3_E_SHIFT                       (6U)
#define GRF_GPIO3C_E_GPIO3C3_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3C_E_GPIO3C4_E_SHIFT                       (8U)
#define GRF_GPIO3C_E_GPIO3C4_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3C_E_GPIO3C5_E_SHIFT                       (10U)
#define GRF_GPIO3C_E_GPIO3C5_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3C_E_GPIO3C6_E_SHIFT                       (12U)
#define GRF_GPIO3C_E_GPIO3C6_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3C_E_GPIO3C7_E_SHIFT                       (14U)
#define GRF_GPIO3C_E_GPIO3C7_E_MASK                        (0x3U << GRF_GPIO3C_E_GPIO3C7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO3D_E */
#define GRF_GPIO3D_E_OFFSET                                (0x11CU)
#define GRF_GPIO3D_E_GPIO3D0_E_SHIFT                       (0U)
#define GRF_GPIO3D_E_GPIO3D0_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO3D_E_GPIO3D1_E_SHIFT                       (2U)
#define GRF_GPIO3D_E_GPIO3D1_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO3D_E_GPIO3D2_E_SHIFT                       (4U)
#define GRF_GPIO3D_E_GPIO3D2_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO3D_E_GPIO3D3_E_SHIFT                       (6U)
#define GRF_GPIO3D_E_GPIO3D3_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO3D_E_GPIO3D4_E_SHIFT                       (8U)
#define GRF_GPIO3D_E_GPIO3D4_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO3D_E_GPIO3D5_E_SHIFT                       (10U)
#define GRF_GPIO3D_E_GPIO3D5_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO3D_E_GPIO3D6_E_SHIFT                       (12U)
#define GRF_GPIO3D_E_GPIO3D6_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO3D_E_GPIO3D7_E_SHIFT                       (14U)
#define GRF_GPIO3D_E_GPIO3D7_E_MASK                        (0x3U << GRF_GPIO3D_E_GPIO3D7_E_SHIFT)                       /* 0x0000C000 */
/* IO_VSEL */
#define GRF_IO_VSEL_OFFSET                                 (0x180U)
#define GRF_IO_VSEL_GRF_VCCIO0_VSEL_SRC_SHIFT              (0U)
#define GRF_IO_VSEL_GRF_VCCIO0_VSEL_SRC_MASK               (0x1U << GRF_IO_VSEL_GRF_VCCIO0_VSEL_SRC_SHIFT)              /* 0x00000001 */
#define GRF_IO_VSEL_GRF_VCCIO0_VSEL_SHIFT                  (1U)
#define GRF_IO_VSEL_GRF_VCCIO0_VSEL_MASK                   (0x1U << GRF_IO_VSEL_GRF_VCCIO0_VSEL_SHIFT)                  /* 0x00000002 */
#define GRF_IO_VSEL_GRF_VCCIO1_VSEL_SHIFT                  (2U)
#define GRF_IO_VSEL_GRF_VCCIO1_VSEL_MASK                   (0x1U << GRF_IO_VSEL_GRF_VCCIO1_VSEL_SHIFT)                  /* 0x00000004 */
#define GRF_IO_VSEL_GRF_VCCIO2_VSEL_SHIFT                  (3U)
#define GRF_IO_VSEL_GRF_VCCIO2_VSEL_MASK                   (0x1U << GRF_IO_VSEL_GRF_VCCIO2_VSEL_SHIFT)                  /* 0x00000008 */
#define GRF_IO_VSEL_GRF_VCCIO3_VSEL_SHIFT                  (4U)
#define GRF_IO_VSEL_GRF_VCCIO3_VSEL_MASK                   (0x1U << GRF_IO_VSEL_GRF_VCCIO3_VSEL_SHIFT)                  /* 0x00000010 */
#define GRF_IO_VSEL_GRF_VCCIO4_VSEL_SHIFT                  (5U)
#define GRF_IO_VSEL_GRF_VCCIO4_VSEL_MASK                   (0x1U << GRF_IO_VSEL_GRF_VCCIO4_VSEL_SHIFT)                  /* 0x00000020 */
#define GRF_IO_VSEL_GRF_VCCIO5_VSEL_SHIFT                  (6U)
#define GRF_IO_VSEL_GRF_VCCIO5_VSEL_MASK                   (0x1U << GRF_IO_VSEL_GRF_VCCIO5_VSEL_SHIFT)                  /* 0x00000040 */
#define GRF_IO_VSEL_GRF_VCCIO_OSCGPI_VSEL_SHIFT            (7U)
#define GRF_IO_VSEL_GRF_VCCIO_OSCGPI_VSEL_MASK             (0x1U << GRF_IO_VSEL_GRF_VCCIO_OSCGPI_VSEL_SHIFT)            /* 0x00000080 */
/* IOFUNC_CON0 */
#define GRF_IOFUNC_CON0_OFFSET                             (0x184U)
#define GRF_IOFUNC_CON0_GRF_CON_I2S1_2CH_LRCK_SHIFT        (0U)
#define GRF_IOFUNC_CON0_GRF_CON_I2S1_2CH_LRCK_MASK         (0x1U << GRF_IOFUNC_CON0_GRF_CON_I2S1_2CH_LRCK_SHIFT)        /* 0x00000001 */
#define GRF_IOFUNC_CON0_GRF_CON_I2S2_2CH_LRCK_SHIFT        (1U)
#define GRF_IOFUNC_CON0_GRF_CON_I2S2_2CH_LRCK_MASK         (0x1U << GRF_IOFUNC_CON0_GRF_CON_I2S2_2CH_LRCK_SHIFT)        /* 0x00000002 */
#define GRF_IOFUNC_CON0_GRF_CON_I2S0_SCLK_SEL_SHIFT        (5U)
#define GRF_IOFUNC_CON0_GRF_CON_I2S0_SCLK_SEL_MASK         (0x1U << GRF_IOFUNC_CON0_GRF_CON_I2S0_SCLK_SEL_SHIFT)        /* 0x00000020 */
#define GRF_IOFUNC_CON0_GRF_CON_PWM0_VOPB_SEL_SHIFT        (6U)
#define GRF_IOFUNC_CON0_GRF_CON_PWM0_VOPB_SEL_MASK         (0x1U << GRF_IOFUNC_CON0_GRF_CON_PWM0_VOPB_SEL_SHIFT)        /* 0x00000040 */
#define GRF_IOFUNC_CON0_GRF_CON_CIF_IOMUX_SEL_SHIFT        (7U)
#define GRF_IOFUNC_CON0_GRF_CON_CIF_IOMUX_SEL_MASK         (0x1U << GRF_IOFUNC_CON0_GRF_CON_CIF_IOMUX_SEL_SHIFT)        /* 0x00000080 */
#define GRF_IOFUNC_CON0_GRF_CON_PDM_IOMUX_SEL_SHIFT        (8U)
#define GRF_IOFUNC_CON0_GRF_CON_PDM_IOMUX_SEL_MASK         (0x1U << GRF_IOFUNC_CON0_GRF_CON_PDM_IOMUX_SEL_SHIFT)        /* 0x00000100 */
#define GRF_IOFUNC_CON0_GRF_CON_UART3_IOMUX_SEL_SHIFT      (9U)
#define GRF_IOFUNC_CON0_GRF_CON_UART3_IOMUX_SEL_MASK       (0x1U << GRF_IOFUNC_CON0_GRF_CON_UART3_IOMUX_SEL_SHIFT)      /* 0x00000200 */
#define GRF_IOFUNC_CON0_GRF_CON_UART2_IOMUX_SEL_SHIFT      (10U)
#define GRF_IOFUNC_CON0_GRF_CON_UART2_IOMUX_SEL_MASK       (0x3U << GRF_IOFUNC_CON0_GRF_CON_UART2_IOMUX_SEL_SHIFT)      /* 0x00000C00 */
#define GRF_IOFUNC_CON0_GRF_CON_I2S0_IOMODE_SEL_SHIFT      (12U)
#define GRF_IOFUNC_CON0_GRF_CON_I2S0_IOMODE_SEL_MASK       (0x3U << GRF_IOFUNC_CON0_GRF_CON_I2S0_IOMODE_SEL_SHIFT)      /* 0x00003000 */
/* SOC_CON0 */
#define GRF_SOC_CON0_OFFSET                                (0x400U)
#define GRF_SOC_CON0_GRF_TSADC_TESTBIT_H_SHIFT             (0U)
#define GRF_SOC_CON0_GRF_TSADC_TESTBIT_H_MASK              (0xFFFFU << GRF_SOC_CON0_GRF_TSADC_TESTBIT_H_SHIFT)          /* 0x0000FFFF */
/* SOC_CON1 */
#define GRF_SOC_CON1_OFFSET                                (0x404U)
#define GRF_SOC_CON1_GRF_TSADC_TESTBIT_L_SHIFT             (0U)
#define GRF_SOC_CON1_GRF_TSADC_TESTBIT_L_MASK              (0xFFFFU << GRF_SOC_CON1_GRF_TSADC_TESTBIT_L_SHIFT)          /* 0x0000FFFF */
/* SOC_CON2 */
#define GRF_SOC_CON2_OFFSET                                (0x408U)
#define GRF_SOC_CON2_GRF_CON_GPLL_CLK_SEL_SHIFT            (0U)
#define GRF_SOC_CON2_GRF_CON_GPLL_CLK_SEL_MASK             (0x1U << GRF_SOC_CON2_GRF_CON_GPLL_CLK_SEL_SHIFT)            /* 0x00000001 */
#define GRF_SOC_CON2_GRF_CON_TSADC_CH_INV_SHIFT            (1U)
#define GRF_SOC_CON2_GRF_CON_TSADC_CH_INV_MASK             (0x1U << GRF_SOC_CON2_GRF_CON_TSADC_CH_INV_SHIFT)            /* 0x00000002 */
#define GRF_SOC_CON2_GRF_CON_UART1_CTS_INV_SHIFT           (2U)
#define GRF_SOC_CON2_GRF_CON_UART1_CTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART1_CTS_INV_SHIFT)           /* 0x00000004 */
#define GRF_SOC_CON2_GRF_CON_UART1_RTS_INV_SHIFT           (3U)
#define GRF_SOC_CON2_GRF_CON_UART1_RTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART1_RTS_INV_SHIFT)           /* 0x00000008 */
#define GRF_SOC_CON2_GRF_CON_UART2_CTS_INV_SHIFT           (4U)
#define GRF_SOC_CON2_GRF_CON_UART2_CTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART2_CTS_INV_SHIFT)           /* 0x00000010 */
#define GRF_SOC_CON2_GRF_CON_UART2_RTS_INV_SHIFT           (5U)
#define GRF_SOC_CON2_GRF_CON_UART2_RTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART2_RTS_INV_SHIFT)           /* 0x00000020 */
#define GRF_SOC_CON2_GRF_CON_UART3_CTS_INV_SHIFT           (6U)
#define GRF_SOC_CON2_GRF_CON_UART3_CTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART3_CTS_INV_SHIFT)           /* 0x00000040 */
#define GRF_SOC_CON2_GRF_CON_UART3_RTS_INV_SHIFT           (7U)
#define GRF_SOC_CON2_GRF_CON_UART3_RTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART3_RTS_INV_SHIFT)           /* 0x00000080 */
#define GRF_SOC_CON2_GRF_CON_UART4_CTS_INV_SHIFT           (8U)
#define GRF_SOC_CON2_GRF_CON_UART4_CTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART4_CTS_INV_SHIFT)           /* 0x00000100 */
#define GRF_SOC_CON2_GRF_CON_UART4_RTS_INV_SHIFT           (9U)
#define GRF_SOC_CON2_GRF_CON_UART4_RTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART4_RTS_INV_SHIFT)           /* 0x00000200 */
#define GRF_SOC_CON2_GRF_CON_UART5_CTS_INV_SHIFT           (10U)
#define GRF_SOC_CON2_GRF_CON_UART5_CTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART5_CTS_INV_SHIFT)           /* 0x00000400 */
#define GRF_SOC_CON2_GRF_CON_UART5_RTS_INV_SHIFT           (11U)
#define GRF_SOC_CON2_GRF_CON_UART5_RTS_INV_MASK            (0x1U << GRF_SOC_CON2_GRF_CON_UART5_RTS_INV_SHIFT)           /* 0x00000800 */
#define GRF_SOC_CON2_GRF_CON_WDTNS_GLB_RESET_EN_SHIFT      (12U)
#define GRF_SOC_CON2_GRF_CON_WDTNS_GLB_RESET_EN_MASK       (0x1U << GRF_SOC_CON2_GRF_CON_WDTNS_GLB_RESET_EN_SHIFT)      /* 0x00001000 */
/* SOC_CON3 */
#define GRF_SOC_CON3_OFFSET                                (0x40CU)
#define GRF_SOC_CON3_GRF_CON_ID_CPU_AR_SHIFT               (0U)
#define GRF_SOC_CON3_GRF_CON_ID_CPU_AR_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_CPU_AR_SHIFT)               /* 0x00000001 */
#define GRF_SOC_CON3_GRF_CON_ID_CPU_AW_SHIFT               (1U)
#define GRF_SOC_CON3_GRF_CON_ID_CPU_AW_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_CPU_AW_SHIFT)               /* 0x00000002 */
#define GRF_SOC_CON3_GRF_CON_ID_CRYPTO_AR_SHIFT            (2U)
#define GRF_SOC_CON3_GRF_CON_ID_CRYPTO_AR_MASK             (0x1U << GRF_SOC_CON3_GRF_CON_ID_CRYPTO_AR_SHIFT)            /* 0x00000004 */
#define GRF_SOC_CON3_GRF_CON_ID_CRYPTO_AW_SHIFT            (3U)
#define GRF_SOC_CON3_GRF_CON_ID_CRYPTO_AW_MASK             (0x1U << GRF_SOC_CON3_GRF_CON_ID_CRYPTO_AW_SHIFT)            /* 0x00000008 */
#define GRF_SOC_CON3_GRF_CON_ID_DCF_AR_SHIFT               (4U)
#define GRF_SOC_CON3_GRF_CON_ID_DCF_AR_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_DCF_AR_SHIFT)               /* 0x00000010 */
#define GRF_SOC_CON3_GRF_CON_ID_DCF_AW_SHIFT               (5U)
#define GRF_SOC_CON3_GRF_CON_ID_DCF_AW_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_DCF_AW_SHIFT)               /* 0x00000020 */
#define GRF_SOC_CON3_GRF_CON_ID_DMA_AR_SHIFT               (6U)
#define GRF_SOC_CON3_GRF_CON_ID_DMA_AR_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_DMA_AR_SHIFT)               /* 0x00000040 */
#define GRF_SOC_CON3_GRF_CON_ID_DMA_AW_SHIFT               (7U)
#define GRF_SOC_CON3_GRF_CON_ID_DMA_AW_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_DMA_AW_SHIFT)               /* 0x00000080 */
#define GRF_SOC_CON3_GRF_CON_ID_GMAC_AR_SHIFT              (8U)
#define GRF_SOC_CON3_GRF_CON_ID_GMAC_AR_MASK               (0x1U << GRF_SOC_CON3_GRF_CON_ID_GMAC_AR_SHIFT)              /* 0x00000100 */
#define GRF_SOC_CON3_GRF_CON_ID_GMAC_AW_SHIFT              (9U)
#define GRF_SOC_CON3_GRF_CON_ID_GMAC_AW_MASK               (0x1U << GRF_SOC_CON3_GRF_CON_ID_GMAC_AW_SHIFT)              /* 0x00000200 */
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AR0_SHIFT              (10U)
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AR0_MASK               (0x1U << GRF_SOC_CON3_GRF_CON_ID_GPU_AR0_SHIFT)              /* 0x00000400 */
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AW0_SHIFT              (11U)
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AW0_MASK               (0x1U << GRF_SOC_CON3_GRF_CON_ID_GPU_AW0_SHIFT)              /* 0x00000800 */
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AR1_SHIFT              (12U)
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AR1_MASK               (0x1U << GRF_SOC_CON3_GRF_CON_ID_GPU_AR1_SHIFT)              /* 0x00001000 */
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AW1_SHIFT              (13U)
#define GRF_SOC_CON3_GRF_CON_ID_GPU_AW1_MASK               (0x1U << GRF_SOC_CON3_GRF_CON_ID_GPU_AW1_SHIFT)              /* 0x00002000 */
#define GRF_SOC_CON3_GRF_CON_ID_CIF_AR_SHIFT               (14U)
#define GRF_SOC_CON3_GRF_CON_ID_CIF_AR_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_CIF_AR_SHIFT)               /* 0x00004000 */
#define GRF_SOC_CON3_GRF_CON_ID_CIF_AW_SHIFT               (15U)
#define GRF_SOC_CON3_GRF_CON_ID_CIF_AW_MASK                (0x1U << GRF_SOC_CON3_GRF_CON_ID_CIF_AW_SHIFT)               /* 0x00008000 */
/* SOC_CON4 */
#define GRF_SOC_CON4_OFFSET                                (0x410U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AR_M2_SHIFT            (0U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AR_M2_MASK             (0x1U << GRF_SOC_CON4_GRF_CON_ID_ISP_AR_M2_SHIFT)            /* 0x00000001 */
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M2_SHIFT            (1U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M2_MASK             (0x1U << GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M2_SHIFT)            /* 0x00000002 */
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AR_M3_SHIFT            (2U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AR_M3_MASK             (0x1U << GRF_SOC_CON4_GRF_CON_ID_ISP_AR_M3_SHIFT)            /* 0x00000004 */
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M3_SHIFT            (3U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M3_MASK             (0x1U << GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M3_SHIFT)            /* 0x00000008 */
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AR_RD_SHIFT            (4U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AR_RD_MASK             (0x1U << GRF_SOC_CON4_GRF_CON_ID_ISP_AR_RD_SHIFT)            /* 0x00000010 */
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M1_SHIFT            (5U)
#define GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M1_MASK             (0x1U << GRF_SOC_CON4_GRF_CON_ID_ISP_AW_M1_SHIFT)            /* 0x00000020 */
#define GRF_SOC_CON4_GRF_CON_ID_RGA_AR_SHIFT               (6U)
#define GRF_SOC_CON4_GRF_CON_ID_RGA_AR_MASK                (0x1U << GRF_SOC_CON4_GRF_CON_ID_RGA_AR_SHIFT)               /* 0x00000040 */
#define GRF_SOC_CON4_GRF_CON_ID_RGA_AW_SHIFT               (7U)
#define GRF_SOC_CON4_GRF_CON_ID_RGA_AW_MASK                (0x1U << GRF_SOC_CON4_GRF_CON_ID_RGA_AW_SHIFT)               /* 0x00000080 */
#define GRF_SOC_CON4_GRF_CON_ID_VOPM_AR_SHIFT              (8U)
#define GRF_SOC_CON4_GRF_CON_ID_VOPM_AR_MASK               (0x1U << GRF_SOC_CON4_GRF_CON_ID_VOPM_AR_SHIFT)              /* 0x00000100 */
#define GRF_SOC_CON4_GRF_CON_ID_VOPM_AW_SHIFT              (9U)
#define GRF_SOC_CON4_GRF_CON_ID_VOPM_AW_MASK               (0x1U << GRF_SOC_CON4_GRF_CON_ID_VOPM_AW_SHIFT)              /* 0x00000200 */
#define GRF_SOC_CON4_GRF_CON_ID_VOPS_AR_SHIFT              (10U)
#define GRF_SOC_CON4_GRF_CON_ID_VOPS_AR_MASK               (0x1U << GRF_SOC_CON4_GRF_CON_ID_VOPS_AR_SHIFT)              /* 0x00000400 */
#define GRF_SOC_CON4_GRF_CON_ID_VOPS_AW_SHIFT              (11U)
#define GRF_SOC_CON4_GRF_CON_ID_VOPS_AW_MASK               (0x1U << GRF_SOC_CON4_GRF_CON_ID_VOPS_AW_SHIFT)              /* 0x00000800 */
#define GRF_SOC_CON4_GRF_CON_ID_VPU_AR_SHIFT               (12U)
#define GRF_SOC_CON4_GRF_CON_ID_VPU_AR_MASK                (0x1U << GRF_SOC_CON4_GRF_CON_ID_VPU_AR_SHIFT)               /* 0x00001000 */
#define GRF_SOC_CON4_GRF_CON_ID_VPU_AW_SHIFT               (13U)
#define GRF_SOC_CON4_GRF_CON_ID_VPU_AW_MASK                (0x1U << GRF_SOC_CON4_GRF_CON_ID_VPU_AW_SHIFT)               /* 0x00002000 */
#define GRF_SOC_CON4_GRF_CON_PMU_PWR_IDLE_REQ_SHIFT        (14U)
#define GRF_SOC_CON4_GRF_CON_PMU_PWR_IDLE_REQ_MASK         (0x1U << GRF_SOC_CON4_GRF_CON_PMU_PWR_IDLE_REQ_SHIFT)        /* 0x00004000 */
#define GRF_SOC_CON4_GRF_CON_HEVC_VCODE_SEL_SHIFT          (15U)
#define GRF_SOC_CON4_GRF_CON_HEVC_VCODE_SEL_MASK           (0x1U << GRF_SOC_CON4_GRF_CON_HEVC_VCODE_SEL_SHIFT)          /* 0x00008000 */
/* SOC_CON5 */
#define GRF_SOC_CON5_OFFSET                                (0x414U)
#define GRF_SOC_CON5_GRF_CON_SDCARD_DECTN_DLY_SHIFT        (0U)
#define GRF_SOC_CON5_GRF_CON_SDCARD_DECTN_DLY_MASK         (0xFFFFFFFFU << GRF_SOC_CON5_GRF_CON_SDCARD_DECTN_DLY_SHIFT) /* 0xFFFFFFFF */
/* PD_VI_CON */
#define GRF_PD_VI_CON_OFFSET                               (0x430U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_0_SHIFT   (0U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_0_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_0_SHIFT)   /* 0x00000001 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_1_SHIFT   (1U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_1_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_1_SHIFT)   /* 0x00000002 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_2_SHIFT   (2U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_2_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_2_SHIFT)   /* 0x00000004 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_3_SHIFT   (3U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_3_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_FORCERXMODE_3_SHIFT)   /* 0x00000008 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_0_SHIFT   (4U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_0_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_0_SHIFT)   /* 0x00000010 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_1_SHIFT   (5U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_1_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_1_SHIFT)   /* 0x00000020 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_2_SHIFT   (6U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_2_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_2_SHIFT)   /* 0x00000040 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_3_SHIFT   (7U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_3_MASK    (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_DATALANE_EN_3_SHIFT)   /* 0x00000080 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_CLKLANE_EN_SHIFT      (8U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_CLKLANE_EN_MASK       (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_CLKLANE_EN_SHIFT)      /* 0x00000100 */
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_CLKINV_SELECTION_SHIFT (9U)
#define GRF_PD_VI_CON_GRF_CON_CSIPHY_CLKINV_SELECTION_MASK (0x1U << GRF_PD_VI_CON_GRF_CON_CSIPHY_CLKINV_SELECTION_SHIFT) /* 0x00000200 */
#define GRF_PD_VI_CON_GRF_CON_CIF_CLK_INV_SEL_SHIFT        (12U)
#define GRF_PD_VI_CON_GRF_CON_CIF_CLK_INV_SEL_MASK         (0x1U << GRF_PD_VI_CON_GRF_CON_CIF_CLK_INV_SEL_SHIFT)        /* 0x00001000 */
#define GRF_PD_VI_CON_GRF_CON_ISP_CIF_IF_DATAWIDTH_SHIFT   (13U)
#define GRF_PD_VI_CON_GRF_CON_ISP_CIF_IF_DATAWIDTH_MASK    (0x3U << GRF_PD_VI_CON_GRF_CON_ISP_CIF_IF_DATAWIDTH_SHIFT)   /* 0x00006000 */
/* PD_VO_CON0 */
#define GRF_PD_VO_CON0_OFFSET                              (0x434U)
#define GRF_PD_VO_CON0_GRF_CON_VOPB_DMA_FINISH_ENABLE_SHIFT (0U)
#define GRF_PD_VO_CON0_GRF_CON_VOPB_DMA_FINISH_ENABLE_MASK (0x1U << GRF_PD_VO_CON0_GRF_CON_VOPB_DMA_FINISH_ENABLE_SHIFT) /* 0x00000001 */
#define GRF_PD_VO_CON0_GRF_CON_VOPL_DMA_FINISH_ENABLE_SHIFT (1U)
#define GRF_PD_VO_CON0_GRF_CON_VOPL_DMA_FINISH_ENABLE_MASK (0x1U << GRF_PD_VO_CON0_GRF_CON_VOPL_DMA_FINISH_ENABLE_SHIFT) /* 0x00000002 */
#define GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPISHUTDN_SHIFT     (2U)
#define GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPISHUTDN_MASK      (0x1U << GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPISHUTDN_SHIFT)     /* 0x00000004 */
#define GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPICOLORM_SHIFT     (3U)
#define GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPICOLORM_MASK      (0x1U << GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPICOLORM_SHIFT)     /* 0x00000008 */
#define GRF_PD_VO_CON0_GRF_CON_LVDS_DCLK_INV_SEL_SHIFT     (5U)
#define GRF_PD_VO_CON0_GRF_CON_LVDS_DCLK_INV_SEL_MASK      (0x1U << GRF_PD_VO_CON0_GRF_CON_LVDS_DCLK_INV_SEL_SHIFT)     /* 0x00000020 */
#define GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPIUPDATECFG_SHIFT  (7U)
#define GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPIUPDATECFG_MASK   (0x1U << GRF_PD_VO_CON0_GRF_CON_DSIHOST_DPIUPDATECFG_SHIFT)  /* 0x00000080 */
#define GRF_PD_VO_CON0_GRF_CON_LVDS_DEN_ONLY_SHIFT         (8U)
#define GRF_PD_VO_CON0_GRF_CON_LVDS_DEN_ONLY_MASK          (0x1U << GRF_PD_VO_CON0_GRF_CON_LVDS_DEN_ONLY_SHIFT)         /* 0x00000100 */
#define GRF_PD_VO_CON0_GRF_CON_LVDS_DEN_ONLY_TIE_VALUE_SHIFT (9U)
#define GRF_PD_VO_CON0_GRF_CON_LVDS_DEN_ONLY_TIE_VALUE_MASK (0x1U << GRF_PD_VO_CON0_GRF_CON_LVDS_DEN_ONLY_TIE_VALUE_SHIFT) /* 0x00000200 */
#define GRF_PD_VO_CON0_GRF_CON_DCF_VOP_STANDBY_SEL_SHIFT   (10U)
#define GRF_PD_VO_CON0_GRF_CON_DCF_VOP_STANDBY_SEL_MASK    (0x3U << GRF_PD_VO_CON0_GRF_CON_DCF_VOP_STANDBY_SEL_SHIFT)   /* 0x00000C00 */
#define GRF_PD_VO_CON0_GRF_CON_VOPM_PRESS_SHIFT            (12U)
#define GRF_PD_VO_CON0_GRF_CON_VOPM_PRESS_MASK             (0x3U << GRF_PD_VO_CON0_GRF_CON_VOPM_PRESS_SHIFT)            /* 0x00003000 */
#define GRF_PD_VO_CON0_GRF_CON_VOPS_PRESS_SHIFT            (14U)
#define GRF_PD_VO_CON0_GRF_CON_VOPS_PRESS_MASK             (0x3U << GRF_PD_VO_CON0_GRF_CON_VOPS_PRESS_SHIFT)            /* 0x0000C000 */
/* PD_VO_CON1 */
#define GRF_PD_VO_CON1_OFFSET                              (0x438U)
#define GRF_PD_VO_CON1_GRF_CON_DSIHOST_VOP_SEL_SHIFT       (0U)
#define GRF_PD_VO_CON1_GRF_CON_DSIHOST_VOP_SEL_MASK        (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIHOST_VOP_SEL_SHIFT)       /* 0x00000001 */
#define GRF_PD_VO_CON1_GRF_CON_LVDS_VOP_SEL_SHIFT          (1U)
#define GRF_PD_VO_CON1_GRF_CON_LVDS_VOP_SEL_MASK           (0x1U << GRF_PD_VO_CON1_GRF_CON_LVDS_VOP_SEL_SHIFT)          /* 0x00000002 */
#define GRF_PD_VO_CON1_GRF_CON_RGB_VOP_SEL_SHIFT           (2U)
#define GRF_PD_VO_CON1_GRF_CON_RGB_VOP_SEL_MASK            (0x1U << GRF_PD_VO_CON1_GRF_CON_RGB_VOP_SEL_SHIFT)           /* 0x00000004 */
#define GRF_PD_VO_CON1_GRF_CON_RGB_BYPASS_SHIFT            (3U)
#define GRF_PD_VO_CON1_GRF_CON_RGB_BYPASS_MASK             (0x1U << GRF_PD_VO_CON1_GRF_CON_RGB_BYPASS_SHIFT)            /* 0x00000008 */
#define GRF_PD_VO_CON1_GRF_CON_LCDC_DCLK_INV_SEL_SHIFT     (4U)
#define GRF_PD_VO_CON1_GRF_CON_LCDC_DCLK_INV_SEL_MASK      (0x1U << GRF_PD_VO_CON1_GRF_CON_LCDC_DCLK_INV_SEL_SHIFT)     /* 0x00000010 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE0_TURNDISABLE_SHIFT (5U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE0_TURNDISABLE_MASK (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE0_TURNDISABLE_SHIFT) /* 0x00000020 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_FORCERXMODE_SHIFT    (6U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_FORCERXMODE_MASK     (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_FORCERXMODE_SHIFT)    /* 0x00000040 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE0_FRCTXSTPM_SHIFT (7U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE0_FRCTXSTPM_MASK (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE0_FRCTXSTPM_SHIFT) /* 0x00000080 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE1_FRCTXSTPM_SHIFT (8U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE1_FRCTXSTPM_MASK (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE1_FRCTXSTPM_SHIFT) /* 0x00000100 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE2_FRCTXSTPM_SHIFT (9U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE2_FRCTXSTPM_MASK (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE2_FRCTXSTPM_SHIFT) /* 0x00000200 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE3_FRCTXSTPM_SHIFT (10U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE3_FRCTXSTPM_MASK (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_LANE3_FRCTXSTPM_SHIFT) /* 0x00000400 */
#define GRF_PD_VO_CON1_GRF_CON_LVDSFORMAT_LVDS_MSBSEL_SHIFT (11U)
#define GRF_PD_VO_CON1_GRF_CON_LVDSFORMAT_LVDS_MSBSEL_MASK (0x1U << GRF_PD_VO_CON1_GRF_CON_LVDSFORMAT_LVDS_MSBSEL_SHIFT) /* 0x00000800 */
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LVDS_MODE_SHIFT      (12U)
#define GRF_PD_VO_CON1_GRF_CON_DSIPHY_LVDS_MODE_MASK       (0x1U << GRF_PD_VO_CON1_GRF_CON_DSIPHY_LVDS_MODE_SHIFT)      /* 0x00001000 */
#define GRF_PD_VO_CON1_GRF_CON_LVDSFORMAT_LVDS_SELECT_SHIFT (13U)
#define GRF_PD_VO_CON1_GRF_CON_LVDSFORMAT_LVDS_SELECT_MASK (0x3U << GRF_PD_VO_CON1_GRF_CON_LVDSFORMAT_LVDS_SELECT_SHIFT) /* 0x00006000 */
/* SOC_STATUS0 */
#define GRF_SOC_STATUS0_OFFSET                             (0x480U)
#define GRF_SOC_STATUS0                                    (0x0U)
#define GRF_SOC_STATUS0_APLL_LOCK_SHIFT                    (0U)
#define GRF_SOC_STATUS0_APLL_LOCK_MASK                     (0x1U << GRF_SOC_STATUS0_APLL_LOCK_SHIFT)                    /* 0x00000001 */
#define GRF_SOC_STATUS0_DPLL_LOCK_SHIFT                    (1U)
#define GRF_SOC_STATUS0_DPLL_LOCK_MASK                     (0x1U << GRF_SOC_STATUS0_DPLL_LOCK_SHIFT)                    /* 0x00000002 */
#define GRF_SOC_STATUS0_CPLL_LOCK_SHIFT                    (2U)
#define GRF_SOC_STATUS0_CPLL_LOCK_MASK                     (0x1U << GRF_SOC_STATUS0_CPLL_LOCK_SHIFT)                    /* 0x00000004 */
#define GRF_SOC_STATUS0_GPLL_LOCK_SHIFT                    (3U)
#define GRF_SOC_STATUS0_GPLL_LOCK_MASK                     (0x1U << GRF_SOC_STATUS0_GPLL_LOCK_SHIFT)                    /* 0x00000008 */
#define GRF_SOC_STATUS0_NPLL_LOCK_SHIFT                    (4U)
#define GRF_SOC_STATUS0_NPLL_LOCK_MASK                     (0x1U << GRF_SOC_STATUS0_NPLL_LOCK_SHIFT)                    /* 0x00000010 */
#define GRF_SOC_STATUS0_OPT_USER_BUSY_S_SHIFT              (6U)
#define GRF_SOC_STATUS0_OPT_USER_BUSY_S_MASK               (0x1U << GRF_SOC_STATUS0_OPT_USER_BUSY_S_SHIFT)              /* 0x00000040 */
#define GRF_SOC_STATUS0_OPT_SBPI_BUSY_S_SHIFT              (7U)
#define GRF_SOC_STATUS0_OPT_SBPI_BUSY_S_MASK               (0x1U << GRF_SOC_STATUS0_OPT_SBPI_BUSY_S_SHIFT)              /* 0x00000080 */
#define GRF_SOC_STATUS0_OPT_USER_BUSY_NS_SHIFT             (8U)
#define GRF_SOC_STATUS0_OPT_USER_BUSY_NS_MASK              (0x1U << GRF_SOC_STATUS0_OPT_USER_BUSY_NS_SHIFT)             /* 0x00000100 */
#define GRF_SOC_STATUS0_OPT_SBPI_BUSY_NS_SHIFT             (9U)
#define GRF_SOC_STATUS0_OPT_SBPI_BUSY_NS_MASK              (0x1U << GRF_SOC_STATUS0_OPT_SBPI_BUSY_NS_SHIFT)             /* 0x00000200 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS0_SHIFT             (11U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS0_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS0_SHIFT)             /* 0x00000800 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS1_SHIFT             (12U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS1_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS1_SHIFT)             /* 0x00001000 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS2_SHIFT             (13U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS2_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS2_SHIFT)             /* 0x00002000 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS3_SHIFT             (14U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS3_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS3_SHIFT)             /* 0x00004000 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS4_SHIFT             (15U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS4_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS4_SHIFT)             /* 0x00008000 */
#define GRF_SOC_STATUS0_TIMER_EN_STATUS5_SHIFT             (16U)
#define GRF_SOC_STATUS0_TIMER_EN_STATUS5_MASK              (0x1U << GRF_SOC_STATUS0_TIMER_EN_STATUS5_SHIFT)             /* 0x00010000 */
#define GRF_SOC_STATUS0_VOPB_DMA_FINISH_SHIFT              (17U)
#define GRF_SOC_STATUS0_VOPB_DMA_FINISH_MASK               (0x1U << GRF_SOC_STATUS0_VOPB_DMA_FINISH_SHIFT)              /* 0x00020000 */
#define GRF_SOC_STATUS0_VOPL_DMA_FINISH_SHIFT              (18U)
#define GRF_SOC_STATUS0_VOPL_DMA_FINISH_MASK               (0x1U << GRF_SOC_STATUS0_VOPL_DMA_FINISH_SHIFT)              /* 0x00040000 */
#define GRF_SOC_STATUS0_PMU_PWR_IDLE_SHIFT                 (19U)
#define GRF_SOC_STATUS0_PMU_PWR_IDLE_MASK                  (0x1U << GRF_SOC_STATUS0_PMU_PWR_IDLE_SHIFT)                 /* 0x00080000 */
#define GRF_SOC_STATUS0_PMU_PWR_IDLE_ACK_SHIFT             (20U)
#define GRF_SOC_STATUS0_PMU_PWR_IDLE_ACK_MASK              (0x1U << GRF_SOC_STATUS0_PMU_PWR_IDLE_ACK_SHIFT)             /* 0x00100000 */
/* CPU_CON0 */
#define GRF_CPU_CON0_OFFSET                                (0x500U)
#define GRF_CPU_CON0_GRF_CON_L1RSTDISABLE_SHIFT            (0U)
#define GRF_CPU_CON0_GRF_CON_L1RSTDISABLE_MASK             (0xFU << GRF_CPU_CON0_GRF_CON_L1RSTDISABLE_SHIFT)            /* 0x0000000F */
#define GRF_CPU_CON0_GRF_CON_L2RSTDISABLE_SHIFT            (4U)
#define GRF_CPU_CON0_GRF_CON_L2RSTDISABLE_MASK             (0x1U << GRF_CPU_CON0_GRF_CON_L2RSTDISABLE_SHIFT)            /* 0x00000010 */
#define GRF_CPU_CON0_GRF_CON_QNAP_DIS_TAGRAM_SHIFT         (5U)
#define GRF_CPU_CON0_GRF_CON_QNAP_DIS_TAGRAM_MASK          (0x1U << GRF_CPU_CON0_GRF_CON_QNAP_DIS_TAGRAM_SHIFT)         /* 0x00000020 */
#define GRF_CPU_CON0_GRF_CON_QNAP_DIS_DATARAM_SHIFT        (6U)
#define GRF_CPU_CON0_GRF_CON_QNAP_DIS_DATARAM_MASK         (0x1U << GRF_CPU_CON0_GRF_CON_QNAP_DIS_DATARAM_SHIFT)        /* 0x00000040 */
#define GRF_CPU_CON0_GRF_CON_CFGEND_SHIFT                  (8U)
#define GRF_CPU_CON0_GRF_CON_CFGEND_MASK                   (0xFU << GRF_CPU_CON0_GRF_CON_CFGEND_SHIFT)                  /* 0x00000F00 */
#define GRF_CPU_CON0_GRF_CON_CFGTE_SHIFT                   (12U)
#define GRF_CPU_CON0_GRF_CON_CFGTE_MASK                    (0xFU << GRF_CPU_CON0_GRF_CON_CFGTE_SHIFT)                   /* 0x0000F000 */
/* CPU_CON1 */
#define GRF_CPU_CON1_OFFSET                                (0x504U)
#define GRF_CPU_CON1_GRF_CON_CLREXMONREQ_SHIFT             (0U)
#define GRF_CPU_CON1_GRF_CON_CLREXMONREQ_MASK              (0x1U << GRF_CPU_CON1_GRF_CON_CLREXMONREQ_SHIFT)             /* 0x00000001 */
#define GRF_CPU_CON1_GRF_CON_CFGSDISABLE_SHIFT             (1U)
#define GRF_CPU_CON1_GRF_CON_CFGSDISABLE_MASK              (0x1U << GRF_CPU_CON1_GRF_CON_CFGSDISABLE_SHIFT)             /* 0x00000002 */
#define GRF_CPU_CON1_GRF_CON_DBGROMADDRV_SHIFT             (2U)
#define GRF_CPU_CON1_GRF_CON_DBGROMADDRV_MASK              (0x1U << GRF_CPU_CON1_GRF_CON_DBGROMADDRV_SHIFT)             /* 0x00000004 */
#define GRF_CPU_CON1_GRF_CON_DBGSELFADDRV_SHIFT            (3U)
#define GRF_CPU_CON1_GRF_CON_DBGSELFADDRV_MASK             (0x1U << GRF_CPU_CON1_GRF_CON_DBGSELFADDRV_SHIFT)            /* 0x00000008 */
#define GRF_CPU_CON1_GRF_CON_EVENTI_SHIFT                  (4U)
#define GRF_CPU_CON1_GRF_CON_EVENTI_MASK                   (0x1U << GRF_CPU_CON1_GRF_CON_EVENTI_SHIFT)                  /* 0x00000010 */
#define GRF_CPU_CON1_GRF_CON_EVENTO_CLEAR_SHIFT            (5U)
#define GRF_CPU_CON1_GRF_CON_EVENTO_CLEAR_MASK             (0x1U << GRF_CPU_CON1_GRF_CON_EVENTO_CLEAR_SHIFT)            /* 0x00000020 */
#define GRF_CPU_CON1_GRF_CON_CPU_EMA_DETECT_EN_SHIFT       (6U)
#define GRF_CPU_CON1_GRF_CON_CPU_EMA_DETECT_EN_MASK        (0x1U << GRF_CPU_CON1_GRF_CON_CPU_EMA_DETECT_EN_SHIFT)       /* 0x00000040 */
#define GRF_CPU_CON1_GRF_FORCE_JTAG_SHIFT                  (7U)
#define GRF_CPU_CON1_GRF_FORCE_JTAG_MASK                   (0x1U << GRF_CPU_CON1_GRF_FORCE_JTAG_SHIFT)                  /* 0x00000080 */
/* CPU_CON2 */
#define GRF_CPU_CON2_OFFSET                                (0x508U)
#define GRF_CPU_CON2_GRF_EMA_L2D_SHIFT                     (0U)
#define GRF_CPU_CON2_GRF_EMA_L2D_MASK                      (0x7U << GRF_CPU_CON2_GRF_EMA_L2D_SHIFT)                     /* 0x00000007 */
#define GRF_CPU_CON2_GRF_EMAW_L2D_SHIFT                    (3U)
#define GRF_CPU_CON2_GRF_EMAW_L2D_MASK                     (0x3U << GRF_CPU_CON2_GRF_EMAW_L2D_SHIFT)                    /* 0x00000018 */
#define GRF_CPU_CON2_GRF_EMA_RA_SHIFT                      (5U)
#define GRF_CPU_CON2_GRF_EMA_RA_MASK                       (0x7U << GRF_CPU_CON2_GRF_EMA_RA_SHIFT)                      /* 0x000000E0 */
#define GRF_CPU_CON2_GRF_EMAW_RA_SHIFT                     (8U)
#define GRF_CPU_CON2_GRF_EMAW_RA_MASK                      (0x3U << GRF_CPU_CON2_GRF_EMAW_RA_SHIFT)                     /* 0x00000300 */
#define GRF_CPU_CON2_GRF_EMAS_RA_SHIFT                     (10U)
#define GRF_CPU_CON2_GRF_EMAS_RA_MASK                      (0x1U << GRF_CPU_CON2_GRF_EMAS_RA_SHIFT)                     /* 0x00000400 */
/* CPU_STATUS0 */
#define GRF_CPU_STATUS0_OFFSET                             (0x520U)
#define GRF_CPU_STATUS0                                    (0x0U)
#define GRF_CPU_STATUS0_GRF_ST_SMPNAMP_SHIFT               (0U)
#define GRF_CPU_STATUS0_GRF_ST_SMPNAMP_MASK                (0xFU << GRF_CPU_STATUS0_GRF_ST_SMPNAMP_SHIFT)               /* 0x0000000F */
#define GRF_CPU_STATUS0_EVENTO_RISING_EDGE_SHIFT           (10U)
#define GRF_CPU_STATUS0_EVENTO_RISING_EDGE_MASK            (0x1U << GRF_CPU_STATUS0_EVENTO_RISING_EDGE_SHIFT)           /* 0x00000400 */
#define GRF_CPU_STATUS0_GRF_ST_JTAGTOP_SHIFT               (11U)
#define GRF_CPU_STATUS0_GRF_ST_JTAGTOP_MASK                (0x1U << GRF_CPU_STATUS0_GRF_ST_JTAGTOP_SHIFT)               /* 0x00000800 */
#define GRF_CPU_STATUS0_GRF_ST_JTAGNSW_SHIFT               (12U)
#define GRF_CPU_STATUS0_GRF_ST_JTAGNSW_MASK                (0x1U << GRF_CPU_STATUS0_GRF_ST_JTAGNSW_SHIFT)               /* 0x00001000 */
#define GRF_CPU_STATUS0_GRF_ST_CLREXMONACK_SHIFT           (13U)
#define GRF_CPU_STATUS0_GRF_ST_CLREXMONACK_MASK            (0x1U << GRF_CPU_STATUS0_GRF_ST_CLREXMONACK_SHIFT)           /* 0x00002000 */
#define GRF_CPU_STATUS0_GRF_ST_L2FLUSHDONE_SHIFT           (14U)
#define GRF_CPU_STATUS0_GRF_ST_L2FLUSHDONE_MASK            (0x1U << GRF_CPU_STATUS0_GRF_ST_L2FLUSHDONE_SHIFT)           /* 0x00004000 */
#define GRF_CPU_STATUS0_GRF_ST_CPU_BOOST_FSM_SHIFT         (15U)
#define GRF_CPU_STATUS0_GRF_ST_CPU_BOOST_FSM_MASK          (0x7FU << GRF_CPU_STATUS0_GRF_ST_CPU_BOOST_FSM_SHIFT)        /* 0x003F8000 */
/* CPU_STATUS1 */
#define GRF_CPU_STATUS1_OFFSET                             (0x524U)
#define GRF_CPU_STATUS1                                    (0x0U)
#define GRF_CPU_STATUS1_GRF_ST_STANDBYWFE_SHIFT            (0U)
#define GRF_CPU_STATUS1_GRF_ST_STANDBYWFE_MASK             (0xFU << GRF_CPU_STATUS1_GRF_ST_STANDBYWFE_SHIFT)            /* 0x0000000F */
#define GRF_CPU_STATUS1_GRF_ST_STANDBYWFI_SHIFT            (4U)
#define GRF_CPU_STATUS1_GRF_ST_STANDBYWFI_MASK             (0xFU << GRF_CPU_STATUS1_GRF_ST_STANDBYWFI_SHIFT)            /* 0x000000F0 */
#define GRF_CPU_STATUS1_GRF_ST_STANDBYWFIL2_SHIFT          (12U)
#define GRF_CPU_STATUS1_GRF_ST_STANDBYWFIL2_MASK           (0x1U << GRF_CPU_STATUS1_GRF_ST_STANDBYWFIL2_SHIFT)          /* 0x00001000 */
/* SOC_NOC_CON0 */
#define GRF_SOC_NOC_CON0_OFFSET                            (0x530U)
#define GRF_SOC_NOC_CON0_DTP_SWITCH26_PWRDISCTARG_PERI2MSCH_SERVICE_TPWRSTALL_SHIFT (0U)
#define GRF_SOC_NOC_CON0_DTP_SWITCH26_PWRDISCTARG_PERI2MSCH_SERVICE_TPWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_SWITCH26_PWRDISCTARG_PERI2MSCH_SERVICE_TPWRSTALL_SHIFT) /* 0x00000001 */
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_DDRC_PWRDISCTARG_DDRC_APB_TPWRSTALL_SHIFT (1U)
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_DDRC_PWRDISCTARG_DDRC_APB_TPWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_BUS_FWD_DDRC_PWRDISCTARG_DDRC_APB_TPWRSTALL_SHIFT) /* 0x00000002 */
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_GPU_PWRDISCTARG_SWITCH46PWRSTALL_SHIFT (2U)
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_GPU_PWRDISCTARG_SWITCH46PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_BUS_FWD_GPU_PWRDISCTARG_SWITCH46PWRSTALL_SHIFT) /* 0x00000004 */
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_PERI_PWRDISCTARG_SWITCH15PWRSTALL_SHIFT (3U)
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_PERI_PWRDISCTARG_SWITCH15PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_BUS_FWD_PERI_PWRDISCTARG_SWITCH15PWRSTALL_SHIFT) /* 0x00000008 */
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_SRVMSCH_PWRDISCTARG_SERVICE_MSCH_TPWRSTALL_SHIFT (4U)
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_SRVMSCH_PWRDISCTARG_SERVICE_MSCH_TPWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_BUS_FWD_SRVMSCH_PWRDISCTARG_SERVICE_MSCH_TPWRSTALL_SHIFT) /* 0x00000010 */
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_VIO_PWRDISCTARG_SWITCH21PWRSTALL_SHIFT (5U)
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_VIO_PWRDISCTARG_SWITCH21PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_BUS_FWD_VIO_PWRDISCTARG_SWITCH21PWRSTALL_SHIFT) /* 0x00000020 */
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_VPU_PWRDISCTARG_SWITCH44PWRSTALL_SHIFT (6U)
#define GRF_SOC_NOC_CON0_DTP_BUS_FWD_VPU_PWRDISCTARG_SWITCH44PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_BUS_FWD_VPU_PWRDISCTARG_SWITCH44PWRSTALL_SHIFT) /* 0x00000040 */
#define GRF_SOC_NOC_CON0_DTP_CPU_FWD_BUS_PWRDISCTARG_SWITCH18PWRSTALL_SHIFT (7U)
#define GRF_SOC_NOC_CON0_DTP_CPU_FWD_BUS_PWRDISCTARG_SWITCH18PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_CPU_FWD_BUS_PWRDISCTARG_SWITCH18PWRSTALL_SHIFT) /* 0x00000080 */
#define GRF_SOC_NOC_CON0_DTP_CPU_REQ_MSCH_PWRDISCTARG_SWITCH28PWRSTALL_SHIFT (8U)
#define GRF_SOC_NOC_CON0_DTP_CPU_REQ_MSCH_PWRDISCTARG_SWITCH28PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_CPU_REQ_MSCH_PWRDISCTARG_SWITCH28PWRSTALL_SHIFT) /* 0x00000100 */
#define GRF_SOC_NOC_CON0_DTP_GPU_REQ_MSCH_PWRDISCTARG_SWITCH29PWRSTALL_SHIFT (9U)
#define GRF_SOC_NOC_CON0_DTP_GPU_REQ_MSCH_PWRDISCTARG_SWITCH29PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_GPU_REQ_MSCH_PWRDISCTARG_SWITCH29PWRSTALL_SHIFT) /* 0x00000200 */
#define GRF_SOC_NOC_CON0_DTP_PERI_REQ_MSCH_PWRDISCTARG_SWITCH8PWRSTALL_SHIFT (10U)
#define GRF_SOC_NOC_CON0_DTP_PERI_REQ_MSCH_PWRDISCTARG_SWITCH8PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_PERI_REQ_MSCH_PWRDISCTARG_SWITCH8PWRSTALL_SHIFT) /* 0x00000400 */
#define GRF_SOC_NOC_CON0_DTP_VI_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_SHIFT (11U)
#define GRF_SOC_NOC_CON0_DTP_VI_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_VI_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_SHIFT) /* 0x00000800 */
#define GRF_SOC_NOC_CON0_DTP_VO_FWD_VI_PWRDISCTARG_SWITCH47PWRSTALL_SHIFT (12U)
#define GRF_SOC_NOC_CON0_DTP_VO_FWD_VI_PWRDISCTARG_SWITCH47PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_VO_FWD_VI_PWRDISCTARG_SWITCH47PWRSTALL_SHIFT) /* 0x00001000 */
#define GRF_SOC_NOC_CON0_DTP_VO_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_SHIFT (13U)
#define GRF_SOC_NOC_CON0_DTP_VO_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_VO_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_SHIFT) /* 0x00002000 */
#define GRF_SOC_NOC_CON0_DTP_VPU_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_SHIFT (14U)
#define GRF_SOC_NOC_CON0_DTP_VPU_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON0_DTP_VPU_REQ_MSCH_PWRDISCTARG_SWITCH1PWRSTALL_SHIFT) /* 0x00004000 */
/* SOC_NOC_CON1 */
#define GRF_SOC_NOC_CON1_OFFSET                            (0x534U)
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_CRYPTO_PWRDISCTARG_SWITCH45PWRSTALL_SHIFT (0U)
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_CRYPTO_PWRDISCTARG_SWITCH45PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_BUS_FWD_CRYPTO_PWRDISCTARG_SWITCH45PWRSTALL_SHIFT) /* 0x00000001 */
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_PERI_PWRDISCTARG_SWITCH16PWRSTALL_SHIFT (1U)
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_PERI_PWRDISCTARG_SWITCH16PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_BUS_FWD_PERI_PWRDISCTARG_SWITCH16PWRSTALL_SHIFT) /* 0x00000002 */
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_PMU_PWRDISCTARG_PMU_APB_TPWRSTALL_SHIFT (2U)
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_PMU_PWRDISCTARG_PMU_APB_TPWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_BUS_FWD_PMU_PWRDISCTARG_PMU_APB_TPWRSTALL_SHIFT) /* 0x00000004 */
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_SDCARD_PWRDISCTARG_SWITCHPWRSTALL_SHIFT (3U)
#define GRF_SOC_NOC_CON1_DTP_BUS_FWD_SDCARD_PWRDISCTARG_SWITCHPWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_BUS_FWD_SDCARD_PWRDISCTARG_SWITCHPWRSTALL_SHIFT) /* 0x00000008 */
#define GRF_SOC_NOC_CON1_DTP_CRYPTO_FWD_BUS_PWRDISCTARG_SWITCH11PWRSTALL_SHIFT (4U)
#define GRF_SOC_NOC_CON1_DTP_CRYPTO_FWD_BUS_PWRDISCTARG_SWITCH11PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_CRYPTO_FWD_BUS_PWRDISCTARG_SWITCH11PWRSTALL_SHIFT) /* 0x00000010 */
#define GRF_SOC_NOC_CON1_DTP_GMAC_FWD_PERI_PWRDISCTARG_SWITCH55PWRSTALL_SHIFT (5U)
#define GRF_SOC_NOC_CON1_DTP_GMAC_FWD_PERI_PWRDISCTARG_SWITCH55PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_GMAC_FWD_PERI_PWRDISCTARG_SWITCH55PWRSTALL_SHIFT) /* 0x00000020 */
#define GRF_SOC_NOC_CON1_DTP_MMC_FWD_PERI_PWRDISCTARG_SWITCH55PWRSTALL_SHIFT (6U)
#define GRF_SOC_NOC_CON1_DTP_MMC_FWD_PERI_PWRDISCTARG_SWITCH55PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_MMC_FWD_PERI_PWRDISCTARG_SWITCH55PWRSTALL_SHIFT) /* 0x00000040 */
#define GRF_SOC_NOC_CON1_DTP_NAND_FWD_PERI_PWRDISCTARG_SWITCH56PWRSTALL_SHIFT (7U)
#define GRF_SOC_NOC_CON1_DTP_NAND_FWD_PERI_PWRDISCTARG_SWITCH56PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_NAND_FWD_PERI_PWRDISCTARG_SWITCH56PWRSTALL_SHIFT) /* 0x00000080 */
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_GMAC_PWRDISCTARG_SWITCH40PWRSTALL_SHIFT (8U)
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_GMAC_PWRDISCTARG_SWITCH40PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_PERI_FWD_GMAC_PWRDISCTARG_SWITCH40PWRSTALL_SHIFT) /* 0x00000100 */
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_MMC_PWRDISCTARG_SWITCH41PWRSTALL_SHIFT (9U)
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_MMC_PWRDISCTARG_SWITCH41PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_PERI_FWD_MMC_PWRDISCTARG_SWITCH41PWRSTALL_SHIFT) /* 0x00000200 */
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_NAND_PWRDISCTARG_SWITCH42PWRSTALL_SHIFT (10U)
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_NAND_PWRDISCTARG_SWITCH42PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_PERI_FWD_NAND_PWRDISCTARG_SWITCH42PWRSTALL_SHIFT) /* 0x00000400 */
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_USB_PWRDISCTARG_SWITCH43PWRSTALL_SHIFT (11U)
#define GRF_SOC_NOC_CON1_DTP_PERI_FWD_USB_PWRDISCTARG_SWITCH43PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_PERI_FWD_USB_PWRDISCTARG_SWITCH43PWRSTALL_SHIFT) /* 0x00000800 */
#define GRF_SOC_NOC_CON1_DTP_SDMMC_FWD_PERI_PWRDISCTARG_SWITCH56PWRSTALL_SHIFT (12U)
#define GRF_SOC_NOC_CON1_DTP_SDMMC_FWD_PERI_PWRDISCTARG_SWITCH56PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_SDMMC_FWD_PERI_PWRDISCTARG_SWITCH56PWRSTALL_SHIFT) /* 0x00001000 */
#define GRF_SOC_NOC_CON1_DTP_USB_FWD_PERI_PWRDISCTARG_SWITCH9PWRSTALL_SHIFT (13U)
#define GRF_SOC_NOC_CON1_DTP_USB_FWD_PERI_PWRDISCTARG_SWITCH9PWRSTALL_MASK (0x1U << GRF_SOC_NOC_CON1_DTP_USB_FWD_PERI_PWRDISCTARG_SWITCH9PWRSTALL_SHIFT) /* 0x00002000 */
#define GRF_SOC_NOC_CON1_MSCH_SPLIT_SIZE_SHIFT             (14U)
#define GRF_SOC_NOC_CON1_MSCH_SPLIT_SIZE_MASK              (0x3U << GRF_SOC_NOC_CON1_MSCH_SPLIT_SIZE_SHIFT)             /* 0x0000C000 */
/* DDR_BANKHASH_CTRL */
#define GRF_DDR_BANKHASH_CTRL_OFFSET                       (0x550U)
#define GRF_DDR_BANKHASH_CTRL_HASH_EN_SHIFT                (0U)
#define GRF_DDR_BANKHASH_CTRL_HASH_EN_MASK                 (0x1U << GRF_DDR_BANKHASH_CTRL_HASH_EN_SHIFT)                /* 0x00000001 */
#define GRF_DDR_BANKHASH_CTRL_MANICURE_MASK_SHIFT          (1U)
#define GRF_DDR_BANKHASH_CTRL_MANICURE_MASK_MASK           (0x7U << GRF_DDR_BANKHASH_CTRL_MANICURE_MASK_SHIFT)          /* 0x0000000E */
#define GRF_DDR_BANKHASH_CTRL_BANK_OFFSET_SHIFT            (4U)
#define GRF_DDR_BANKHASH_CTRL_BANK_OFFSET_MASK             (0x7U << GRF_DDR_BANKHASH_CTRL_BANK_OFFSET_SHIFT)            /* 0x00000070 */
/* DDR_BANK_MASK0 */
#define GRF_DDR_BANK_MASK0_OFFSET                          (0x554U)
#define GRF_DDR_BANK_MASK0_DDR_BANK_MASK0_SHIFT            (0U)
#define GRF_DDR_BANK_MASK0_DDR_BANK_MASK0_MASK             (0xFFFFFFFFU << GRF_DDR_BANK_MASK0_DDR_BANK_MASK0_SHIFT)     /* 0xFFFFFFFF */
/* DDR_BANK_MASK1 */
#define GRF_DDR_BANK_MASK1_OFFSET                          (0x558U)
#define GRF_DDR_BANK_MASK1_DDR_BANK_MASK1_SHIFT            (0U)
#define GRF_DDR_BANK_MASK1_DDR_BANK_MASK1_MASK             (0xFFFFFFFFU << GRF_DDR_BANK_MASK1_DDR_BANK_MASK1_SHIFT)     /* 0xFFFFFFFF */
/* DDR_BANK_MASK2 */
#define GRF_DDR_BANK_MASK2_OFFSET                          (0x55CU)
#define GRF_DDR_BANK_MASK2_DDR_BANK_MASK2_SHIFT            (0U)
#define GRF_DDR_BANK_MASK2_DDR_BANK_MASK2_MASK             (0xFFFFFFFFU << GRF_DDR_BANK_MASK2_DDR_BANK_MASK2_SHIFT)     /* 0xFFFFFFFF */
/* HOST0_CON0 */
#define GRF_HOST0_CON0_OFFSET                              (0x700U)
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_SHIFT       (0U)
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_MASK        (0x3FU << GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_SHIFT)      /* 0x0000003F */
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_COMMON_SHIFT (6U)
#define GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_COMMON_MASK (0x3FU << GRF_HOST0_CON0_GRF_CON_HOST0_FLADJ_VAL_COMMON_SHIFT) /* 0x00000FC0 */
/* HOST0_CON1 */
#define GRF_HOST0_CON1_OFFSET                              (0x704U)
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
#define GRF_HOST0_CON1_GRF_CON_HOST0_ARB_PAUSE_SHIFT       (13U)
#define GRF_HOST0_CON1_GRF_CON_HOST0_ARB_PAUSE_MASK        (0x1U << GRF_HOST0_CON1_GRF_CON_HOST0_ARB_PAUSE_SHIFT)       /* 0x00002000 */
/* CHIP_ID */
#define GRF_CHIP_ID_OFFSET                                 (0x800U)
#define GRF_CHIP_ID                                        (0x3326U)
#define GRF_CHIP_ID_CHIP_IP_SHIFT                          (0U)
#define GRF_CHIP_ID_CHIP_IP_MASK                           (0xFFFFFFFFU << GRF_CHIP_ID_CHIP_IP_SHIFT)                   /* 0xFFFFFFFF */
/* OTG_CON3 */
#define GRF_OTG_CON3_OFFSET                                (0x880U)
#define GRF_OTG_CON3_OTG_SCALEDOWN_MODE_SHIFT              (0U)
#define GRF_OTG_CON3_OTG_SCALEDOWN_MODE_MASK               (0x3U << GRF_OTG_CON3_OTG_SCALEDOWN_MODE_SHIFT)              /* 0x00000003 */
#define GRF_OTG_CON3_OTG_DBNCE_FLTR_BYPASS_SHIFT           (2U)
#define GRF_OTG_CON3_OTG_DBNCE_FLTR_BYPASS_MASK            (0x1U << GRF_OTG_CON3_OTG_DBNCE_FLTR_BYPASS_SHIFT)           /* 0x00000004 */
/* HOST0_STATUS4 */
#define GRF_HOST0_STATUS4_OFFSET                           (0x890U)
#define GRF_HOST0_STATUS4                                  (0x0U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_XFER_CNT_SHIFT (0U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_XFER_CNT_MASK (0x7FFU << GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_XFER_CNT_SHIFT) /* 0x000007FF */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_USBSTS_SHIFT (11U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_USBSTS_MASK  (0x3FU << GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_USBSTS_SHIFT) /* 0x0001F800 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_LPSMC_STATE_SHIFT (17U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_LPSMC_STATE_MASK (0xFU << GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_LPSMC_STATE_SHIFT) /* 0x001E0000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_RMTWKP_SHIFT (21U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_RMTWKP_MASK  (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_RMTWKP_SHIFT) /* 0x00200000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_BUFACC_SHIFT (22U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_BUFACC_MASK  (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_BUFACC_SHIFT) /* 0x00400000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_GLOBALSUSPEND_SHIFT (23U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_GLOBALSUSPEND_MASK (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_GLOBALSUSPEND_SHIFT) /* 0x00800000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_DRWE_SHIFT   (24U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_DRWE_MASK    (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_DRWE_SHIFT)   /* 0x01000000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_RWE_SHIFT    (25U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_RWE_MASK     (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_RWE_SHIFT)    /* 0x02000000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_CCS_SHIFT    (26U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_CCS_MASK     (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_OHCI_CCS_SHIFT)    /* 0x04000000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_XFER_PRDC_SHIFT (27U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_XFER_PRDC_MASK (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_XFER_PRDC_SHIFT) /* 0x08000000 */
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_BUFACC_SHIFT (28U)
#define GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_BUFACC_MASK  (0x1U << GRF_HOST0_STATUS4_GRF_STAT_HOST0_EHCI_BUFACC_SHIFT) /* 0x10000000 */
#define GRF_HOST0_STATUS4_HOST0_EHCI_PME_STATUS_SHIFT      (29U)
#define GRF_HOST0_STATUS4_HOST0_EHCI_PME_STATUS_MASK       (0x1U << GRF_HOST0_STATUS4_HOST0_EHCI_PME_STATUS_SHIFT)      /* 0x20000000 */
#define GRF_HOST0_STATUS4_HOST0_EHCI_POWER_STATE_ACK_SHIFT (30U)
#define GRF_HOST0_STATUS4_HOST0_EHCI_POWER_STATE_ACK_MASK  (0x1U << GRF_HOST0_STATUS4_HOST0_EHCI_POWER_STATE_ACK_SHIFT) /* 0x40000000 */
/* MAC_CON1 */
#define GRF_MAC_CON1_OFFSET                                (0x904U)
#define GRF_MAC_CON1_GMAC2IO_MAC_SPEED_SHIFT               (2U)
#define GRF_MAC_CON1_GMAC2IO_MAC_SPEED_MASK                (0x1U << GRF_MAC_CON1_GMAC2IO_MAC_SPEED_SHIFT)               /* 0x00000004 */
#define GRF_MAC_CON1_GMAC2IO_FLOWCTRL_SHIFT                (3U)
#define GRF_MAC_CON1_GMAC2IO_FLOWCTRL_MASK                 (0x1U << GRF_MAC_CON1_GMAC2IO_FLOWCTRL_SHIFT)                /* 0x00000008 */
#define GRF_MAC_CON1_GMAC2IO_PHY_INTF_SEL_SHIFT            (4U)
#define GRF_MAC_CON1_GMAC2IO_PHY_INTF_SEL_MASK             (0x7U << GRF_MAC_CON1_GMAC2IO_PHY_INTF_SEL_SHIFT)            /* 0x00000070 */
/****************************************CORE_GRF****************************************/
/* CA35_PEFF_CON0 */
#define CORE_GRF_CA35_PEFF_CON0_OFFSET                     (0x0U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_PERF_WORK_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_PERF_WORK_MASK (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_PERF_WORK_SHIFT) /* 0x00000001 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_PERF_CLR_SHIFT (1U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_PERF_CLR_MASK  (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_PERF_CLR_SHIFT) /* 0x00000002 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_CNT_TYPE_SHIFT (2U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_CNT_TYPE_MASK  (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_CNT_TYPE_SHIFT) /* 0x00000004 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_CNT_TYPE_WRAP_SHIFT (3U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_CNT_TYPE_WRAP_MASK (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_AXI_CNT_TYPE_WRAP_SHIFT) /* 0x00000008 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AR_CNT_ID_TYPE_SHIFT (4U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AR_CNT_ID_TYPE_MASK (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_AR_CNT_ID_TYPE_SHIFT) /* 0x00000010 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AW_CNT_ID_TYPE_SHIFT (5U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_AW_CNT_ID_TYPE_MASK (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_AW_CNT_ID_TYPE_SHIFT) /* 0x00000020 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_DDR_ALIGN_TYPE_SHIFT (6U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_DDR_ALIGN_TYPE_MASK (0x3U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_DDR_ALIGN_TYPE_SHIFT) /* 0x000000C0 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_RD_LATENCY_ID_SHIFT (8U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_RD_LATENCY_ID_MASK (0x3FU << CORE_GRF_CA35_PEFF_CON0_CA35_SW_RD_LATENCY_ID_SHIFT) /* 0x00003F00 */
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_RD_LATENCY_ID_RANGE_E_SHIFT (15U)
#define CORE_GRF_CA35_PEFF_CON0_CA35_SW_RD_LATENCY_ID_RANGE_E_MASK (0x1U << CORE_GRF_CA35_PEFF_CON0_CA35_SW_RD_LATENCY_ID_RANGE_E_SHIFT) /* 0x00008000 */
/* CA35_PEFF_CON1 */
#define CORE_GRF_CA35_PEFF_CON1_OFFSET                     (0x4U)
#define CORE_GRF_CA35_PEFF_CON1_CA35_SW_RD_LATENCY_THR_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON1_CA35_SW_RD_LATENCY_THR_MASK (0xFFFU << CORE_GRF_CA35_PEFF_CON1_CA35_SW_RD_LATENCY_THR_SHIFT) /* 0x00000FFF */
/* CA35_PEFF_CON2 */
#define CORE_GRF_CA35_PEFF_CON2_OFFSET                     (0x8U)
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AR_COUNT_ID_SHIFT  (0U)
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AR_COUNT_ID_MASK   (0x3FU << CORE_GRF_CA35_PEFF_CON2_CA35_SW_AR_COUNT_ID_SHIFT) /* 0x0000003F */
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AW_COUNT_ID_SHIFT  (8U)
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AW_COUNT_ID_MASK   (0x1FU << CORE_GRF_CA35_PEFF_CON2_CA35_SW_AW_COUNT_ID_SHIFT) /* 0x00001F00 */
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AXI_PERF_INT_E_SHIFT (14U)
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AXI_PERF_INT_E_MASK (0x1U << CORE_GRF_CA35_PEFF_CON2_CA35_SW_AXI_PERF_INT_E_SHIFT) /* 0x00004000 */
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AXI_PERF_INT_CLR_SHIFT (15U)
#define CORE_GRF_CA35_PEFF_CON2_CA35_SW_AXI_PERF_INT_CLR_MASK (0x1U << CORE_GRF_CA35_PEFF_CON2_CA35_SW_AXI_PERF_INT_CLR_SHIFT) /* 0x00008000 */
/* CA35_PEFF_CON3 */
#define CORE_GRF_CA35_PEFF_CON3_OFFSET                     (0xCU)
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_MSK_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_MSK_MASK (0x3FU << CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_MSK_SHIFT) /* 0x0000003F */
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_TYPE_SHIFT (8U)
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_TYPE_MASK (0x1FU << CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_TYPE_SHIFT) /* 0x00001F00 */
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_BMSK_SHIFT (14U)
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_BMSK_MASK (0x1U << CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_BMSK_SHIFT) /* 0x00004000 */
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_SHIFT    (15U)
#define CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_MASK     (0x1U << CORE_GRF_CA35_PEFF_CON3_CA35_SW_AR_MON_ID_SHIFT)    /* 0x00008000 */
/* CA35_PEFF_CON4 */
#define CORE_GRF_CA35_PEFF_CON4_OFFSET                     (0x10U)
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_MSK_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_MSK_MASK (0x3FU << CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_MSK_SHIFT) /* 0x0000003F */
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_TYPE_SHIFT (8U)
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_TYPE_MASK (0x1FU << CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_TYPE_SHIFT) /* 0x00001F00 */
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_BMSK_SHIFT (14U)
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_BMSK_MASK (0x1U << CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_BMSK_SHIFT) /* 0x00004000 */
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_SHIFT    (15U)
#define CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_MASK     (0x1U << CORE_GRF_CA35_PEFF_CON4_CA35_SW_AW_MON_ID_SHIFT)    /* 0x00008000 */
/* CA35_PEFF_CON5 */
#define CORE_GRF_CA35_PEFF_CON5_OFFSET                     (0x14U)
#define CORE_GRF_CA35_PEFF_CON5_CA35_SW_ARADDR_MON_ST_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON5_CA35_SW_ARADDR_MON_ST_MASK (0xFFFFFFFFU << CORE_GRF_CA35_PEFF_CON5_CA35_SW_ARADDR_MON_ST_SHIFT) /* 0xFFFFFFFF */
/* CA35_PEFF_CON6 */
#define CORE_GRF_CA35_PEFF_CON6_OFFSET                     (0x18U)
#define CORE_GRF_CA35_PEFF_CON6_CA35_SW_ARADDR_MON_END_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON6_CA35_SW_ARADDR_MON_END_MASK (0xFFFFFFFFU << CORE_GRF_CA35_PEFF_CON6_CA35_SW_ARADDR_MON_END_SHIFT) /* 0xFFFFFFFF */
/* CA35_PEFF_CON7 */
#define CORE_GRF_CA35_PEFF_CON7_OFFSET                     (0x1CU)
#define CORE_GRF_CA35_PEFF_CON7_CA35_SW_AWADDR_MON_ST_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON7_CA35_SW_AWADDR_MON_ST_MASK (0xFFFFFFFFU << CORE_GRF_CA35_PEFF_CON7_CA35_SW_AWADDR_MON_ST_SHIFT) /* 0xFFFFFFFF */
/* CA35_PEFF_CON8 */
#define CORE_GRF_CA35_PEFF_CON8_OFFSET                     (0x20U)
#define CORE_GRF_CA35_PEFF_CON8_CA35_SW_AWADDR_MON_END_SHIFT (0U)
#define CORE_GRF_CA35_PEFF_CON8_CA35_SW_AWADDR_MON_END_MASK (0xFFFFFFFFU << CORE_GRF_CA35_PEFF_CON8_CA35_SW_AWADDR_MON_END_SHIFT) /* 0xFFFFFFFF */
/* A35_PERF_RD_MAX_LATENCY_NUM */
#define CORE_GRF_A35_PERF_RD_MAX_LATENCY_NUM_OFFSET        (0x30U)
#define CORE_GRF_A35_PERF_RD_MAX_LATENCY_NUM               (0x0U)
#define CORE_GRF_A35_PERF_RD_MAX_LATENCY_NUM_RD_MAX_LATENCY_R_SHIFT (0U)
#define CORE_GRF_A35_PERF_RD_MAX_LATENCY_NUM_RD_MAX_LATENCY_R_MASK (0x1FFFU << CORE_GRF_A35_PERF_RD_MAX_LATENCY_NUM_RD_MAX_LATENCY_R_SHIFT) /* 0x00001FFF */
/* A35_PERF_RD_LATENCY_SAMP_NUM */
#define CORE_GRF_A35_PERF_RD_LATENCY_SAMP_NUM_OFFSET       (0x34U)
#define CORE_GRF_A35_PERF_RD_LATENCY_SAMP_NUM              (0x0U)
#define CORE_GRF_A35_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_SAMP_R_SHIFT (0U)
#define CORE_GRF_A35_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_SAMP_R_MASK (0x7FFFFFFU << CORE_GRF_A35_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_SAMP_R_SHIFT) /* 0x07FFFFFF */
/* A35_PERF_RD_LATENCY_ACC_NUM */
#define CORE_GRF_A35_PERF_RD_LATENCY_ACC_NUM_OFFSET        (0x38U)
#define CORE_GRF_A35_PERF_RD_LATENCY_ACC_NUM               (0x0U)
#define CORE_GRF_A35_PERF_RD_LATENCY_ACC_NUM_RD_LATENCY_ACC_CNT_R_SHIFT (0U)
#define CORE_GRF_A35_PERF_RD_LATENCY_ACC_NUM_RD_LATENCY_ACC_CNT_R_MASK (0xFFFFFFFFU << CORE_GRF_A35_PERF_RD_LATENCY_ACC_NUM_RD_LATENCY_ACC_CNT_R_SHIFT) /* 0xFFFFFFFF */
/* A35_PERF_RD_AXI_TOTAL_BYTE */
#define CORE_GRF_A35_PERF_RD_AXI_TOTAL_BYTE_OFFSET         (0x3CU)
#define CORE_GRF_A35_PERF_RD_AXI_TOTAL_BYTE                (0x0U)
#define CORE_GRF_A35_PERF_RD_AXI_TOTAL_BYTE_RD_AXI_TOTAL_BYTE_SHIFT (0U)
#define CORE_GRF_A35_PERF_RD_AXI_TOTAL_BYTE_RD_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << CORE_GRF_A35_PERF_RD_AXI_TOTAL_BYTE_RD_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* A35_PERF_WR_AXI_TOTAL_BYTE */
#define CORE_GRF_A35_PERF_WR_AXI_TOTAL_BYTE_OFFSET         (0x40U)
#define CORE_GRF_A35_PERF_WR_AXI_TOTAL_BYTE                (0x0U)
#define CORE_GRF_A35_PERF_WR_AXI_TOTAL_BYTE_WR_AXI_TOTAL_BYTE_SHIFT (0U)
#define CORE_GRF_A35_PERF_WR_AXI_TOTAL_BYTE_WR_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << CORE_GRF_A35_PERF_WR_AXI_TOTAL_BYTE_WR_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* A35_PERF_WORKING_CNT */
#define CORE_GRF_A35_PERF_WORKING_CNT_OFFSET               (0x44U)
#define CORE_GRF_A35_PERF_WORKING_CNT                      (0x0U)
#define CORE_GRF_A35_PERF_WORKING_CNT_WORKING_CNT_R_SHIFT  (0U)
#define CORE_GRF_A35_PERF_WORKING_CNT_WORKING_CNT_R_MASK   (0xFFFFFFFFU << CORE_GRF_A35_PERF_WORKING_CNT_WORKING_CNT_R_SHIFT) /* 0xFFFFFFFF */
/* A35_PERF_INT_STATUS */
#define CORE_GRF_A35_PERF_INT_STATUS_OFFSET                (0x48U)
#define CORE_GRF_A35_PERF_INT_STATUS                       (0x0U)
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AR_MON_AXI_HIT_FLAG_SHIFT (0U)
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AR_MON_AXI_HIT_FLAG_MASK (0x1U << CORE_GRF_A35_PERF_INT_STATUS_A35_AR_MON_AXI_HIT_FLAG_SHIFT) /* 0x00000001 */
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AR_MON_AXI_ID_STATUS_SHIFT (8U)
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AR_MON_AXI_ID_STATUS_MASK (0x7FU << CORE_GRF_A35_PERF_INT_STATUS_A35_AR_MON_AXI_ID_STATUS_SHIFT) /* 0x00007F00 */
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AW_MON_AXI_HIT_FLAG_SHIFT (16U)
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AW_MON_AXI_HIT_FLAG_MASK (0x1U << CORE_GRF_A35_PERF_INT_STATUS_A35_AW_MON_AXI_HIT_FLAG_SHIFT) /* 0x00010000 */
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AW_MON_AXI_ID_STATUS_SHIFT (24U)
#define CORE_GRF_A35_PERF_INT_STATUS_A35_AW_MON_AXI_ID_STATUS_MASK (0x7FU << CORE_GRF_A35_PERF_INT_STATUS_A35_AW_MON_AXI_ID_STATUS_SHIFT) /* 0x7F000000 */
/* COREPVTM_CON0 */
#define CORE_GRF_COREPVTM_CON0_OFFSET                      (0x80U)
#define CORE_GRF_COREPVTM_CON0_COREPVTM_START_SHIFT        (0U)
#define CORE_GRF_COREPVTM_CON0_COREPVTM_START_MASK         (0x1U << CORE_GRF_COREPVTM_CON0_COREPVTM_START_SHIFT)        /* 0x00000001 */
#define CORE_GRF_COREPVTM_CON0_COREPVTM_OSC_EN_SHIFT       (1U)
#define CORE_GRF_COREPVTM_CON0_COREPVTM_OSC_EN_MASK        (0x1U << CORE_GRF_COREPVTM_CON0_COREPVTM_OSC_EN_SHIFT)       /* 0x00000002 */
#define CORE_GRF_COREPVTM_CON0_COREPVTM_OSC_SEL_SHIFT      (2U)
#define CORE_GRF_COREPVTM_CON0_COREPVTM_OSC_SEL_MASK       (0x3U << CORE_GRF_COREPVTM_CON0_COREPVTM_OSC_SEL_SHIFT)      /* 0x0000000C */
/* COREPVTM_CON1 */
#define CORE_GRF_COREPVTM_CON1_OFFSET                      (0x84U)
#define CORE_GRF_COREPVTM_CON1_COREPVTM_CAL_CNT_SHIFT      (0U)
#define CORE_GRF_COREPVTM_CON1_COREPVTM_CAL_CNT_MASK       (0xFFFFFFFFU << CORE_GRF_COREPVTM_CON1_COREPVTM_CAL_CNT_SHIFT) /* 0xFFFFFFFF */
/* COREPVTM_STATUS0 */
#define CORE_GRF_COREPVTM_STATUS0_OFFSET                   (0x88U)
#define CORE_GRF_COREPVTM_STATUS0_COREPVTM_FREQ_DONE_SHIFT (0U)
#define CORE_GRF_COREPVTM_STATUS0_COREPVTM_FREQ_DONE_MASK  (0x1U << CORE_GRF_COREPVTM_STATUS0_COREPVTM_FREQ_DONE_SHIFT) /* 0x00000001 */
/* COREPVTM_STATUS1 */
#define CORE_GRF_COREPVTM_STATUS1_OFFSET                   (0x8CU)
#define CORE_GRF_COREPVTM_STATUS1_COREPVTM_FREQ_CNT_SHIFT  (0U)
#define CORE_GRF_COREPVTM_STATUS1_COREPVTM_FREQ_CNT_MASK   (0xFFFFFFFFU << CORE_GRF_COREPVTM_STATUS1_COREPVTM_FREQ_CNT_SHIFT) /* 0xFFFFFFFF */
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
#define I2C_ST                                             (0x3U)
#define I2C_ST_SDA_ST_SHIFT                                (0U)
#define I2C_ST_SDA_ST_MASK                                 (0x1U << I2C_ST_SDA_ST_SHIFT)                                /* 0x00000001 */
#define I2C_ST_SCL_ST_SHIFT                                (1U)
#define I2C_ST_SCL_ST_MASK                                 (0x1U << I2C_ST_SCL_ST_SHIFT)                                /* 0x00000002 */
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
/* CTRLR1 */
#define SPI_CTRLR1_OFFSET                                  (0x4U)
#define SPI_CTRLR1_NDM_SHIFT                               (0U)
#define SPI_CTRLR1_NDM_MASK                                (0xFFFFU << SPI_CTRLR1_NDM_SHIFT)                            /* 0x0000FFFF */
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
#define SPI_TXFTLR_TXFTLR_MASK                             (0x1FU << SPI_TXFTLR_TXFTLR_SHIFT)                           /* 0x0000001F */
/* RXFTLR */
#define SPI_RXFTLR_OFFSET                                  (0x18U)
#define SPI_RXFTLR_RXFTLR_SHIFT                            (0U)
#define SPI_RXFTLR_RXFTLR_MASK                             (0x1FU << SPI_RXFTLR_RXFTLR_SHIFT)                           /* 0x0000001F */
/* TXFLR */
#define SPI_TXFLR_OFFSET                                   (0x1CU)
#define SPI_TXFLR                                          (0x0U)
#define SPI_TXFLR_TXFLR_SHIFT                              (0U)
#define SPI_TXFLR_TXFLR_MASK                               (0x3FU << SPI_TXFLR_TXFLR_SHIFT)                             /* 0x0000003F */
/* RXFLR */
#define SPI_RXFLR_OFFSET                                   (0x20U)
#define SPI_RXFLR                                          (0x0U)
#define SPI_RXFLR_RXFLR_SHIFT                              (0U)
#define SPI_RXFLR_RXFLR_MASK                               (0x3FU << SPI_RXFLR_RXFLR_SHIFT)                             /* 0x0000003F */
/* SR */
#define SPI_SR_OFFSET                                      (0x24U)
#define SPI_SR                                             (0xCU)
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
/* ISR */
#define SPI_ISR_OFFSET                                     (0x30U)
#define SPI_ISR                                            (0x0U)
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
#define SPI_RISR_OFFSET                                    (0x34U)
#define SPI_RISR                                           (0x1U)
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
#define SPI_ICR_OFFSET                                     (0x38U)
#define SPI_ICR_CCI_SHIFT                                  (0U)
#define SPI_ICR_CCI_MASK                                   (0x1U << SPI_ICR_CCI_SHIFT)                                  /* 0x00000001 */
#define SPI_ICR_CRFUI_SHIFT                                (1U)
#define SPI_ICR_CRFUI_MASK                                 (0x1U << SPI_ICR_CRFUI_SHIFT)                                /* 0x00000002 */
#define SPI_ICR_CRFOI_SHIFT                                (2U)
#define SPI_ICR_CRFOI_MASK                                 (0x1U << SPI_ICR_CRFOI_SHIFT)                                /* 0x00000004 */
#define SPI_ICR_CTFOI_SHIFT                                (3U)
#define SPI_ICR_CTFOI_MASK                                 (0x1U << SPI_ICR_CTFOI_SHIFT)                                /* 0x00000008 */
/* DMACR */
#define SPI_DMACR_OFFSET                                   (0x3CU)
#define SPI_DMACR_RDE_SHIFT                                (0U)
#define SPI_DMACR_RDE_MASK                                 (0x1U << SPI_DMACR_RDE_SHIFT)                                /* 0x00000001 */
#define SPI_DMACR_TDE_SHIFT                                (1U)
#define SPI_DMACR_TDE_MASK                                 (0x1U << SPI_DMACR_TDE_SHIFT)                                /* 0x00000002 */
/* DMATDLR */
#define SPI_DMATDLR_OFFSET                                 (0x40U)
#define SPI_DMATDLR_TDL_SHIFT                              (0U)
#define SPI_DMATDLR_TDL_MASK                               (0x1FU << SPI_DMATDLR_TDL_SHIFT)                             /* 0x0000001F */
/* DMARDLR */
#define SPI_DMARDLR_OFFSET                                 (0x44U)
#define SPI_DMARDLR_RDL_SHIFT                              (0U)
#define SPI_DMARDLR_RDL_MASK                               (0x1FU << SPI_DMARDLR_RDL_SHIFT)                             /* 0x0000001F */
/* TXDR */
#define SPI_TXDR_OFFSET                                    (0x400U)
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_OFFSET                                    (0x800U)
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/*****************************************TIMER******************************************/
/* LOAD_COUNT0 */
#define TIMER_LOAD_COUNT0_OFFSET                           (0x0U)
#define TIMER_LOAD_COUNT0_COUNT0_SHIFT                     (0U)
#define TIMER_LOAD_COUNT0_COUNT0_MASK                      (0xFFFFFFFFU << TIMER_LOAD_COUNT0_COUNT0_SHIFT)              /* 0xFFFFFFFF */
/* LOAD_COUNT1 */
#define TIMER_LOAD_COUNT1_OFFSET                           (0x4U)
#define TIMER_LOAD_COUNT1_COUNT1_SHIFT                     (0U)
#define TIMER_LOAD_COUNT1_COUNT1_MASK                      (0xFFFFFFFFU << TIMER_LOAD_COUNT1_COUNT1_SHIFT)              /* 0xFFFFFFFF */
/* CURRENT_VALUE0 */
#define TIMER_CURRENT_VALUE0_OFFSET                        (0x8U)
#define TIMER_CURRENT_VALUE0                               (0x0U)
#define TIMER_CURRENT_VALUE0_CURRENT_VALUE0_SHIFT          (0U)
#define TIMER_CURRENT_VALUE0_CURRENT_VALUE0_MASK           (0xFFFFFFFFU << TIMER_CURRENT_VALUE0_CURRENT_VALUE0_SHIFT)   /* 0xFFFFFFFF */
/* CURRENT_VALUE1 */
#define TIMER_CURRENT_VALUE1_OFFSET                        (0xCU)
#define TIMER_CURRENT_VALUE1                               (0x0U)
#define TIMER_CURRENT_VALUE1_CURRENT_VALUE1_SHIFT          (0U)
#define TIMER_CURRENT_VALUE1_CURRENT_VALUE1_MASK           (0xFFFFFFFFU << TIMER_CURRENT_VALUE1_CURRENT_VALUE1_SHIFT)   /* 0xFFFFFFFF */
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
#define TIMER_INTSTATUS_INT_PD_SHIFT                       (0U)
#define TIMER_INTSTATUS_INT_PD_MASK                        (0x1U << TIMER_INTSTATUS_INT_PD_SHIFT)                       /* 0x00000001 */
/******************************************DMA*******************************************/
/* DSR */
#define DMA_DSR_OFFSET                                     (0x0U)
#define DMA_DSR                                            (0x0U)
#define DMA_DSR_FIELD0000_SHIFT                            (0U)
#define DMA_DSR_FIELD0000_MASK                             (0xFU << DMA_DSR_FIELD0000_SHIFT)                            /* 0x0000000F */
#define DMA_DSR_FIELD0002_SHIFT                            (4U)
#define DMA_DSR_FIELD0002_MASK                             (0x1FU << DMA_DSR_FIELD0002_SHIFT)                           /* 0x000001F0 */
#define DMA_DSR_FIELD0001_SHIFT                            (9U)
#define DMA_DSR_FIELD0001_MASK                             (0x1U << DMA_DSR_FIELD0001_SHIFT)                            /* 0x00000200 */
/* DPC */
#define DMA_DPC_OFFSET                                     (0x4U)
#define DMA_DPC                                            (0x0U)
#define DMA_DPC_FIELD0000_SHIFT                            (0U)
#define DMA_DPC_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_DPC_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* INTEN */
#define DMA_INTEN_OFFSET                                   (0x20U)
#define DMA_INTEN_FIELD0000_SHIFT                          (0U)
#define DMA_INTEN_FIELD0000_MASK                           (0xFFFFFFFFU << DMA_INTEN_FIELD0000_SHIFT)                   /* 0xFFFFFFFF */
/* EVENT_RIS */
#define DMA_EVENT_RIS_OFFSET                               (0x24U)
#define DMA_EVENT_RIS                                      (0x0U)
#define DMA_EVENT_RIS_FIELD0000_SHIFT                      (0U)
#define DMA_EVENT_RIS_FIELD0000_MASK                       (0xFFFFFFFFU << DMA_EVENT_RIS_FIELD0000_SHIFT)               /* 0xFFFFFFFF */
/* INTMIS */
#define DMA_INTMIS_OFFSET                                  (0x28U)
#define DMA_INTMIS                                         (0x0U)
#define DMA_INTMIS_FIELD0000_SHIFT                         (0U)
#define DMA_INTMIS_FIELD0000_MASK                          (0xFFFFFFFFU << DMA_INTMIS_FIELD0000_SHIFT)                  /* 0xFFFFFFFF */
/* INTCLR */
#define DMA_INTCLR_OFFSET                                  (0x2CU)
#define DMA_INTCLR_FIELD0000_SHIFT                         (0U)
#define DMA_INTCLR_FIELD0000_MASK                          (0xFFFFFFFFU << DMA_INTCLR_FIELD0000_SHIFT)                  /* 0xFFFFFFFF */
/* FSRD */
#define DMA_FSRD_OFFSET                                    (0x30U)
#define DMA_FSRD                                           (0x0U)
#define DMA_FSRD_FIELD0000_SHIFT                           (0U)
#define DMA_FSRD_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_FSRD_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* FSRC */
#define DMA_FSRC_OFFSET                                    (0x34U)
#define DMA_FSRC                                           (0x0U)
#define DMA_FSRC_FIELD0000_SHIFT                           (0U)
#define DMA_FSRC_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_FSRC_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* FTRD */
#define DMA_FTRD_OFFSET                                    (0x38U)
#define DMA_FTRD_FIELD0000_SHIFT                           (0U)
#define DMA_FTRD_FIELD0000_MASK                            (0x1U << DMA_FTRD_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTRD_FIELD0005_SHIFT                           (1U)
#define DMA_FTRD_FIELD0005_MASK                            (0x1U << DMA_FTRD_FIELD0005_SHIFT)                           /* 0x00000002 */
#define DMA_FTRD_FIELD0004_SHIFT                           (4U)
#define DMA_FTRD_FIELD0004_MASK                            (0x1U << DMA_FTRD_FIELD0004_SHIFT)                           /* 0x00000010 */
#define DMA_FTRD_FIELD0003_SHIFT                           (5U)
#define DMA_FTRD_FIELD0003_MASK                            (0x1U << DMA_FTRD_FIELD0003_SHIFT)                           /* 0x00000020 */
#define DMA_FTRD_FIELD0002_SHIFT                           (16U)
#define DMA_FTRD_FIELD0002_MASK                            (0x1U << DMA_FTRD_FIELD0002_SHIFT)                           /* 0x00010000 */
#define DMA_FTRD_FIELD0001_SHIFT                           (30U)
#define DMA_FTRD_FIELD0001_MASK                            (0x1U << DMA_FTRD_FIELD0001_SHIFT)                           /* 0x40000000 */
/* FTR0 */
#define DMA_FTR0_OFFSET                                    (0x40U)
#define DMA_FTR0                                           (0x0U)
#define DMA_FTR0_FIELD0000_SHIFT                           (0U)
#define DMA_FTR0_FIELD0000_MASK                            (0x1U << DMA_FTR0_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTR0_FIELD0011_SHIFT                           (1U)
#define DMA_FTR0_FIELD0011_MASK                            (0x1U << DMA_FTR0_FIELD0011_SHIFT)                           /* 0x00000002 */
#define DMA_FTR0_FIELD0010_SHIFT                           (5U)
#define DMA_FTR0_FIELD0010_MASK                            (0x1U << DMA_FTR0_FIELD0010_SHIFT)                           /* 0x00000020 */
#define DMA_FTR0_FIELD0009_SHIFT                           (6U)
#define DMA_FTR0_FIELD0009_MASK                            (0x1U << DMA_FTR0_FIELD0009_SHIFT)                           /* 0x00000040 */
#define DMA_FTR0_FIELD0008_SHIFT                           (7U)
#define DMA_FTR0_FIELD0008_MASK                            (0x1U << DMA_FTR0_FIELD0008_SHIFT)                           /* 0x00000080 */
#define DMA_FTR0_FIELD0007_SHIFT                           (12U)
#define DMA_FTR0_FIELD0007_MASK                            (0x1U << DMA_FTR0_FIELD0007_SHIFT)                           /* 0x00001000 */
#define DMA_FTR0_FIELD0006_SHIFT                           (13U)
#define DMA_FTR0_FIELD0006_MASK                            (0x1U << DMA_FTR0_FIELD0006_SHIFT)                           /* 0x00002000 */
#define DMA_FTR0_FIELD0005_SHIFT                           (16U)
#define DMA_FTR0_FIELD0005_MASK                            (0x1U << DMA_FTR0_FIELD0005_SHIFT)                           /* 0x00010000 */
#define DMA_FTR0_FIELD0004_SHIFT                           (17U)
#define DMA_FTR0_FIELD0004_MASK                            (0x1U << DMA_FTR0_FIELD0004_SHIFT)                           /* 0x00020000 */
#define DMA_FTR0_FIELD0003_SHIFT                           (18U)
#define DMA_FTR0_FIELD0003_MASK                            (0x1U << DMA_FTR0_FIELD0003_SHIFT)                           /* 0x00040000 */
#define DMA_FTR0_FIELD0002_SHIFT                           (30U)
#define DMA_FTR0_FIELD0002_MASK                            (0x1U << DMA_FTR0_FIELD0002_SHIFT)                           /* 0x40000000 */
#define DMA_FTR0_FIELD0001_SHIFT                           (31U)
#define DMA_FTR0_FIELD0001_MASK                            (0x1U << DMA_FTR0_FIELD0001_SHIFT)                           /* 0x80000000 */
/* FTR1 */
#define DMA_FTR1_OFFSET                                    (0x44U)
#define DMA_FTR1                                           (0x0U)
#define DMA_FTR1_FIELD0000_SHIFT                           (0U)
#define DMA_FTR1_FIELD0000_MASK                            (0x1U << DMA_FTR1_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTR1_FIELD0011_SHIFT                           (1U)
#define DMA_FTR1_FIELD0011_MASK                            (0x1U << DMA_FTR1_FIELD0011_SHIFT)                           /* 0x00000002 */
#define DMA_FTR1_FIELD0010_SHIFT                           (5U)
#define DMA_FTR1_FIELD0010_MASK                            (0x1U << DMA_FTR1_FIELD0010_SHIFT)                           /* 0x00000020 */
#define DMA_FTR1_FIELD0009_SHIFT                           (6U)
#define DMA_FTR1_FIELD0009_MASK                            (0x1U << DMA_FTR1_FIELD0009_SHIFT)                           /* 0x00000040 */
#define DMA_FTR1_FIELD0008_SHIFT                           (7U)
#define DMA_FTR1_FIELD0008_MASK                            (0x1U << DMA_FTR1_FIELD0008_SHIFT)                           /* 0x00000080 */
#define DMA_FTR1_FIELD0007_SHIFT                           (12U)
#define DMA_FTR1_FIELD0007_MASK                            (0x1U << DMA_FTR1_FIELD0007_SHIFT)                           /* 0x00001000 */
#define DMA_FTR1_FIELD0006_SHIFT                           (13U)
#define DMA_FTR1_FIELD0006_MASK                            (0x1U << DMA_FTR1_FIELD0006_SHIFT)                           /* 0x00002000 */
#define DMA_FTR1_FIELD0005_SHIFT                           (16U)
#define DMA_FTR1_FIELD0005_MASK                            (0x1U << DMA_FTR1_FIELD0005_SHIFT)                           /* 0x00010000 */
#define DMA_FTR1_FIELD0004_SHIFT                           (17U)
#define DMA_FTR1_FIELD0004_MASK                            (0x1U << DMA_FTR1_FIELD0004_SHIFT)                           /* 0x00020000 */
#define DMA_FTR1_FIELD0003_SHIFT                           (18U)
#define DMA_FTR1_FIELD0003_MASK                            (0x1U << DMA_FTR1_FIELD0003_SHIFT)                           /* 0x00040000 */
#define DMA_FTR1_FIELD0002_SHIFT                           (30U)
#define DMA_FTR1_FIELD0002_MASK                            (0x1U << DMA_FTR1_FIELD0002_SHIFT)                           /* 0x40000000 */
#define DMA_FTR1_FIELD0001_SHIFT                           (31U)
#define DMA_FTR1_FIELD0001_MASK                            (0x1U << DMA_FTR1_FIELD0001_SHIFT)                           /* 0x80000000 */
/* FTR2 */
#define DMA_FTR2_OFFSET                                    (0x48U)
#define DMA_FTR2                                           (0x0U)
#define DMA_FTR2_FIELD0000_SHIFT                           (0U)
#define DMA_FTR2_FIELD0000_MASK                            (0x1U << DMA_FTR2_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTR2_FIELD0011_SHIFT                           (1U)
#define DMA_FTR2_FIELD0011_MASK                            (0x1U << DMA_FTR2_FIELD0011_SHIFT)                           /* 0x00000002 */
#define DMA_FTR2_FIELD0010_SHIFT                           (5U)
#define DMA_FTR2_FIELD0010_MASK                            (0x1U << DMA_FTR2_FIELD0010_SHIFT)                           /* 0x00000020 */
#define DMA_FTR2_FIELD0009_SHIFT                           (6U)
#define DMA_FTR2_FIELD0009_MASK                            (0x1U << DMA_FTR2_FIELD0009_SHIFT)                           /* 0x00000040 */
#define DMA_FTR2_FIELD0008_SHIFT                           (7U)
#define DMA_FTR2_FIELD0008_MASK                            (0x1U << DMA_FTR2_FIELD0008_SHIFT)                           /* 0x00000080 */
#define DMA_FTR2_FIELD0007_SHIFT                           (12U)
#define DMA_FTR2_FIELD0007_MASK                            (0x1U << DMA_FTR2_FIELD0007_SHIFT)                           /* 0x00001000 */
#define DMA_FTR2_FIELD0006_SHIFT                           (13U)
#define DMA_FTR2_FIELD0006_MASK                            (0x1U << DMA_FTR2_FIELD0006_SHIFT)                           /* 0x00002000 */
#define DMA_FTR2_FIELD0005_SHIFT                           (16U)
#define DMA_FTR2_FIELD0005_MASK                            (0x1U << DMA_FTR2_FIELD0005_SHIFT)                           /* 0x00010000 */
#define DMA_FTR2_FIELD0004_SHIFT                           (17U)
#define DMA_FTR2_FIELD0004_MASK                            (0x1U << DMA_FTR2_FIELD0004_SHIFT)                           /* 0x00020000 */
#define DMA_FTR2_FIELD0003_SHIFT                           (18U)
#define DMA_FTR2_FIELD0003_MASK                            (0x1U << DMA_FTR2_FIELD0003_SHIFT)                           /* 0x00040000 */
#define DMA_FTR2_FIELD0002_SHIFT                           (30U)
#define DMA_FTR2_FIELD0002_MASK                            (0x1U << DMA_FTR2_FIELD0002_SHIFT)                           /* 0x40000000 */
#define DMA_FTR2_FIELD0001_SHIFT                           (31U)
#define DMA_FTR2_FIELD0001_MASK                            (0x1U << DMA_FTR2_FIELD0001_SHIFT)                           /* 0x80000000 */
/* FTR3 */
#define DMA_FTR3_OFFSET                                    (0x4CU)
#define DMA_FTR3                                           (0x0U)
#define DMA_FTR3_FIELD0000_SHIFT                           (0U)
#define DMA_FTR3_FIELD0000_MASK                            (0x1U << DMA_FTR3_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTR3_FIELD0011_SHIFT                           (1U)
#define DMA_FTR3_FIELD0011_MASK                            (0x1U << DMA_FTR3_FIELD0011_SHIFT)                           /* 0x00000002 */
#define DMA_FTR3_FIELD0010_SHIFT                           (5U)
#define DMA_FTR3_FIELD0010_MASK                            (0x1U << DMA_FTR3_FIELD0010_SHIFT)                           /* 0x00000020 */
#define DMA_FTR3_FIELD0009_SHIFT                           (6U)
#define DMA_FTR3_FIELD0009_MASK                            (0x1U << DMA_FTR3_FIELD0009_SHIFT)                           /* 0x00000040 */
#define DMA_FTR3_FIELD0008_SHIFT                           (7U)
#define DMA_FTR3_FIELD0008_MASK                            (0x1U << DMA_FTR3_FIELD0008_SHIFT)                           /* 0x00000080 */
#define DMA_FTR3_FIELD0007_SHIFT                           (12U)
#define DMA_FTR3_FIELD0007_MASK                            (0x1U << DMA_FTR3_FIELD0007_SHIFT)                           /* 0x00001000 */
#define DMA_FTR3_FIELD0006_SHIFT                           (13U)
#define DMA_FTR3_FIELD0006_MASK                            (0x1U << DMA_FTR3_FIELD0006_SHIFT)                           /* 0x00002000 */
#define DMA_FTR3_FIELD0005_SHIFT                           (16U)
#define DMA_FTR3_FIELD0005_MASK                            (0x1U << DMA_FTR3_FIELD0005_SHIFT)                           /* 0x00010000 */
#define DMA_FTR3_FIELD0004_SHIFT                           (17U)
#define DMA_FTR3_FIELD0004_MASK                            (0x1U << DMA_FTR3_FIELD0004_SHIFT)                           /* 0x00020000 */
#define DMA_FTR3_FIELD0003_SHIFT                           (18U)
#define DMA_FTR3_FIELD0003_MASK                            (0x1U << DMA_FTR3_FIELD0003_SHIFT)                           /* 0x00040000 */
#define DMA_FTR3_FIELD0002_SHIFT                           (30U)
#define DMA_FTR3_FIELD0002_MASK                            (0x1U << DMA_FTR3_FIELD0002_SHIFT)                           /* 0x40000000 */
#define DMA_FTR3_FIELD0001_SHIFT                           (31U)
#define DMA_FTR3_FIELD0001_MASK                            (0x1U << DMA_FTR3_FIELD0001_SHIFT)                           /* 0x80000000 */
/* FTR4 */
#define DMA_FTR4_OFFSET                                    (0x50U)
#define DMA_FTR4                                           (0x0U)
#define DMA_FTR4_FIELD0000_SHIFT                           (0U)
#define DMA_FTR4_FIELD0000_MASK                            (0x1U << DMA_FTR4_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTR4_FIELD0011_SHIFT                           (1U)
#define DMA_FTR4_FIELD0011_MASK                            (0x1U << DMA_FTR4_FIELD0011_SHIFT)                           /* 0x00000002 */
#define DMA_FTR4_FIELD0010_SHIFT                           (5U)
#define DMA_FTR4_FIELD0010_MASK                            (0x1U << DMA_FTR4_FIELD0010_SHIFT)                           /* 0x00000020 */
#define DMA_FTR4_FIELD0009_SHIFT                           (6U)
#define DMA_FTR4_FIELD0009_MASK                            (0x1U << DMA_FTR4_FIELD0009_SHIFT)                           /* 0x00000040 */
#define DMA_FTR4_FIELD0008_SHIFT                           (7U)
#define DMA_FTR4_FIELD0008_MASK                            (0x1U << DMA_FTR4_FIELD0008_SHIFT)                           /* 0x00000080 */
#define DMA_FTR4_FIELD0007_SHIFT                           (12U)
#define DMA_FTR4_FIELD0007_MASK                            (0x1U << DMA_FTR4_FIELD0007_SHIFT)                           /* 0x00001000 */
#define DMA_FTR4_FIELD0006_SHIFT                           (13U)
#define DMA_FTR4_FIELD0006_MASK                            (0x1U << DMA_FTR4_FIELD0006_SHIFT)                           /* 0x00002000 */
#define DMA_FTR4_FIELD0005_SHIFT                           (16U)
#define DMA_FTR4_FIELD0005_MASK                            (0x1U << DMA_FTR4_FIELD0005_SHIFT)                           /* 0x00010000 */
#define DMA_FTR4_FIELD0004_SHIFT                           (17U)
#define DMA_FTR4_FIELD0004_MASK                            (0x1U << DMA_FTR4_FIELD0004_SHIFT)                           /* 0x00020000 */
#define DMA_FTR4_FIELD0003_SHIFT                           (18U)
#define DMA_FTR4_FIELD0003_MASK                            (0x1U << DMA_FTR4_FIELD0003_SHIFT)                           /* 0x00040000 */
#define DMA_FTR4_FIELD0002_SHIFT                           (30U)
#define DMA_FTR4_FIELD0002_MASK                            (0x1U << DMA_FTR4_FIELD0002_SHIFT)                           /* 0x40000000 */
#define DMA_FTR4_FIELD0001_SHIFT                           (31U)
#define DMA_FTR4_FIELD0001_MASK                            (0x1U << DMA_FTR4_FIELD0001_SHIFT)                           /* 0x80000000 */
/* FTR5 */
#define DMA_FTR5_OFFSET                                    (0x54U)
#define DMA_FTR5                                           (0x0U)
#define DMA_FTR5_FIELD0000_SHIFT                           (0U)
#define DMA_FTR5_FIELD0000_MASK                            (0x1U << DMA_FTR5_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_FTR5_FIELD0011_SHIFT                           (1U)
#define DMA_FTR5_FIELD0011_MASK                            (0x1U << DMA_FTR5_FIELD0011_SHIFT)                           /* 0x00000002 */
#define DMA_FTR5_FIELD0010_SHIFT                           (5U)
#define DMA_FTR5_FIELD0010_MASK                            (0x1U << DMA_FTR5_FIELD0010_SHIFT)                           /* 0x00000020 */
#define DMA_FTR5_FIELD0009_SHIFT                           (6U)
#define DMA_FTR5_FIELD0009_MASK                            (0x1U << DMA_FTR5_FIELD0009_SHIFT)                           /* 0x00000040 */
#define DMA_FTR5_FIELD0008_SHIFT                           (7U)
#define DMA_FTR5_FIELD0008_MASK                            (0x1U << DMA_FTR5_FIELD0008_SHIFT)                           /* 0x00000080 */
#define DMA_FTR5_FIELD0007_SHIFT                           (12U)
#define DMA_FTR5_FIELD0007_MASK                            (0x1U << DMA_FTR5_FIELD0007_SHIFT)                           /* 0x00001000 */
#define DMA_FTR5_FIELD0006_SHIFT                           (13U)
#define DMA_FTR5_FIELD0006_MASK                            (0x1U << DMA_FTR5_FIELD0006_SHIFT)                           /* 0x00002000 */
#define DMA_FTR5_FIELD0005_SHIFT                           (16U)
#define DMA_FTR5_FIELD0005_MASK                            (0x1U << DMA_FTR5_FIELD0005_SHIFT)                           /* 0x00010000 */
#define DMA_FTR5_FIELD0004_SHIFT                           (17U)
#define DMA_FTR5_FIELD0004_MASK                            (0x1U << DMA_FTR5_FIELD0004_SHIFT)                           /* 0x00020000 */
#define DMA_FTR5_FIELD0003_SHIFT                           (18U)
#define DMA_FTR5_FIELD0003_MASK                            (0x1U << DMA_FTR5_FIELD0003_SHIFT)                           /* 0x00040000 */
#define DMA_FTR5_FIELD0002_SHIFT                           (30U)
#define DMA_FTR5_FIELD0002_MASK                            (0x1U << DMA_FTR5_FIELD0002_SHIFT)                           /* 0x40000000 */
#define DMA_FTR5_FIELD0001_SHIFT                           (31U)
#define DMA_FTR5_FIELD0001_MASK                            (0x1U << DMA_FTR5_FIELD0001_SHIFT)                           /* 0x80000000 */
/* CSR0 */
#define DMA_CSR0_OFFSET                                    (0x100U)
#define DMA_CSR0                                           (0x0U)
#define DMA_CSR0_FIELD0000_SHIFT                           (0U)
#define DMA_CSR0_FIELD0000_MASK                            (0xFU << DMA_CSR0_FIELD0000_SHIFT)                           /* 0x0000000F */
#define DMA_CSR0_FIELD0004_SHIFT                           (4U)
#define DMA_CSR0_FIELD0004_MASK                            (0x1FU << DMA_CSR0_FIELD0004_SHIFT)                          /* 0x000001F0 */
#define DMA_CSR0_FIELD0003_SHIFT                           (14U)
#define DMA_CSR0_FIELD0003_MASK                            (0x1U << DMA_CSR0_FIELD0003_SHIFT)                           /* 0x00004000 */
#define DMA_CSR0_FIELD0002_SHIFT                           (15U)
#define DMA_CSR0_FIELD0002_MASK                            (0x1U << DMA_CSR0_FIELD0002_SHIFT)                           /* 0x00008000 */
#define DMA_CSR0_FIELD0001_SHIFT                           (21U)
#define DMA_CSR0_FIELD0001_MASK                            (0x1U << DMA_CSR0_FIELD0001_SHIFT)                           /* 0x00200000 */
/* CPC0 */
#define DMA_CPC0_OFFSET                                    (0x104U)
#define DMA_CPC0                                           (0x0U)
#define DMA_CPC0_FIELD0000_SHIFT                           (0U)
#define DMA_CPC0_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC0_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR1 */
#define DMA_CSR1_OFFSET                                    (0x108U)
#define DMA_CSR1                                           (0x0U)
#define DMA_CSR1_FIELD0000_SHIFT                           (0U)
#define DMA_CSR1_FIELD0000_MASK                            (0xFU << DMA_CSR1_FIELD0000_SHIFT)                           /* 0x0000000F */
#define DMA_CSR1_FIELD0004_SHIFT                           (4U)
#define DMA_CSR1_FIELD0004_MASK                            (0x1FU << DMA_CSR1_FIELD0004_SHIFT)                          /* 0x000001F0 */
#define DMA_CSR1_FIELD0003_SHIFT                           (14U)
#define DMA_CSR1_FIELD0003_MASK                            (0x1U << DMA_CSR1_FIELD0003_SHIFT)                           /* 0x00004000 */
#define DMA_CSR1_FIELD0002_SHIFT                           (15U)
#define DMA_CSR1_FIELD0002_MASK                            (0x1U << DMA_CSR1_FIELD0002_SHIFT)                           /* 0x00008000 */
#define DMA_CSR1_FIELD0001_SHIFT                           (21U)
#define DMA_CSR1_FIELD0001_MASK                            (0x1U << DMA_CSR1_FIELD0001_SHIFT)                           /* 0x00200000 */
/* CPC1 */
#define DMA_CPC1_OFFSET                                    (0x10CU)
#define DMA_CPC1                                           (0x0U)
#define DMA_CPC1_FIELD0000_SHIFT                           (0U)
#define DMA_CPC1_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC1_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR2 */
#define DMA_CSR2_OFFSET                                    (0x110U)
#define DMA_CSR2                                           (0x0U)
#define DMA_CSR2_FIELD0000_SHIFT                           (0U)
#define DMA_CSR2_FIELD0000_MASK                            (0xFU << DMA_CSR2_FIELD0000_SHIFT)                           /* 0x0000000F */
#define DMA_CSR2_FIELD0004_SHIFT                           (4U)
#define DMA_CSR2_FIELD0004_MASK                            (0x1FU << DMA_CSR2_FIELD0004_SHIFT)                          /* 0x000001F0 */
#define DMA_CSR2_FIELD0003_SHIFT                           (14U)
#define DMA_CSR2_FIELD0003_MASK                            (0x1U << DMA_CSR2_FIELD0003_SHIFT)                           /* 0x00004000 */
#define DMA_CSR2_FIELD0002_SHIFT                           (15U)
#define DMA_CSR2_FIELD0002_MASK                            (0x1U << DMA_CSR2_FIELD0002_SHIFT)                           /* 0x00008000 */
#define DMA_CSR2_FIELD0001_SHIFT                           (21U)
#define DMA_CSR2_FIELD0001_MASK                            (0x1U << DMA_CSR2_FIELD0001_SHIFT)                           /* 0x00200000 */
/* CPC2 */
#define DMA_CPC2_OFFSET                                    (0x114U)
#define DMA_CPC2                                           (0x0U)
#define DMA_CPC2_FIELD0000_SHIFT                           (0U)
#define DMA_CPC2_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC2_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR3 */
#define DMA_CSR3_OFFSET                                    (0x118U)
#define DMA_CSR3                                           (0x0U)
#define DMA_CSR3_FIELD0000_SHIFT                           (0U)
#define DMA_CSR3_FIELD0000_MASK                            (0xFU << DMA_CSR3_FIELD0000_SHIFT)                           /* 0x0000000F */
#define DMA_CSR3_FIELD0004_SHIFT                           (4U)
#define DMA_CSR3_FIELD0004_MASK                            (0x1FU << DMA_CSR3_FIELD0004_SHIFT)                          /* 0x000001F0 */
#define DMA_CSR3_FIELD0003_SHIFT                           (14U)
#define DMA_CSR3_FIELD0003_MASK                            (0x1U << DMA_CSR3_FIELD0003_SHIFT)                           /* 0x00004000 */
#define DMA_CSR3_FIELD0002_SHIFT                           (15U)
#define DMA_CSR3_FIELD0002_MASK                            (0x1U << DMA_CSR3_FIELD0002_SHIFT)                           /* 0x00008000 */
#define DMA_CSR3_FIELD0001_SHIFT                           (21U)
#define DMA_CSR3_FIELD0001_MASK                            (0x1U << DMA_CSR3_FIELD0001_SHIFT)                           /* 0x00200000 */
/* CPC3 */
#define DMA_CPC3_OFFSET                                    (0x11CU)
#define DMA_CPC3                                           (0x0U)
#define DMA_CPC3_FIELD0000_SHIFT                           (0U)
#define DMA_CPC3_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC3_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR4 */
#define DMA_CSR4_OFFSET                                    (0x120U)
#define DMA_CSR4                                           (0x0U)
#define DMA_CSR4_FIELD0000_SHIFT                           (0U)
#define DMA_CSR4_FIELD0000_MASK                            (0xFU << DMA_CSR4_FIELD0000_SHIFT)                           /* 0x0000000F */
#define DMA_CSR4_FIELD0004_SHIFT                           (4U)
#define DMA_CSR4_FIELD0004_MASK                            (0x1FU << DMA_CSR4_FIELD0004_SHIFT)                          /* 0x000001F0 */
#define DMA_CSR4_FIELD0003_SHIFT                           (14U)
#define DMA_CSR4_FIELD0003_MASK                            (0x1U << DMA_CSR4_FIELD0003_SHIFT)                           /* 0x00004000 */
#define DMA_CSR4_FIELD0002_SHIFT                           (15U)
#define DMA_CSR4_FIELD0002_MASK                            (0x1U << DMA_CSR4_FIELD0002_SHIFT)                           /* 0x00008000 */
#define DMA_CSR4_FIELD0001_SHIFT                           (21U)
#define DMA_CSR4_FIELD0001_MASK                            (0x1U << DMA_CSR4_FIELD0001_SHIFT)                           /* 0x00200000 */
/* CPC4 */
#define DMA_CPC4_OFFSET                                    (0x124U)
#define DMA_CPC4                                           (0x0U)
#define DMA_CPC4_FIELD0000_SHIFT                           (0U)
#define DMA_CPC4_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC4_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR5 */
#define DMA_CSR5_OFFSET                                    (0x128U)
#define DMA_CSR5                                           (0x0U)
#define DMA_CSR5_FIELD0000_SHIFT                           (0U)
#define DMA_CSR5_FIELD0000_MASK                            (0xFU << DMA_CSR5_FIELD0000_SHIFT)                           /* 0x0000000F */
#define DMA_CSR5_FIELD0004_SHIFT                           (4U)
#define DMA_CSR5_FIELD0004_MASK                            (0x1FU << DMA_CSR5_FIELD0004_SHIFT)                          /* 0x000001F0 */
#define DMA_CSR5_FIELD0003_SHIFT                           (14U)
#define DMA_CSR5_FIELD0003_MASK                            (0x1U << DMA_CSR5_FIELD0003_SHIFT)                           /* 0x00004000 */
#define DMA_CSR5_FIELD0002_SHIFT                           (15U)
#define DMA_CSR5_FIELD0002_MASK                            (0x1U << DMA_CSR5_FIELD0002_SHIFT)                           /* 0x00008000 */
#define DMA_CSR5_FIELD0001_SHIFT                           (21U)
#define DMA_CSR5_FIELD0001_MASK                            (0x1U << DMA_CSR5_FIELD0001_SHIFT)                           /* 0x00200000 */
/* CPC5 */
#define DMA_CPC5_OFFSET                                    (0x12CU)
#define DMA_CPC5                                           (0x0U)
#define DMA_CPC5_FIELD0000_SHIFT                           (0U)
#define DMA_CPC5_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC5_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* SAR0 */
#define DMA_SAR0_OFFSET                                    (0x400U)
#define DMA_SAR0                                           (0x0U)
#define DMA_SAR0_FIELD0000_SHIFT                           (0U)
#define DMA_SAR0_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR0_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR0 */
#define DMA_DAR0_OFFSET                                    (0x404U)
#define DMA_DAR0                                           (0x0U)
#define DMA_DAR0_FIELD0000_SHIFT                           (0U)
#define DMA_DAR0_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR0_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR0 */
#define DMA_CCR0_OFFSET                                    (0x408U)
#define DMA_CCR0                                           (0x0U)
#define DMA_CCR0_FIELD0000_SHIFT                           (0U)
#define DMA_CCR0_FIELD0000_MASK                            (0x1U << DMA_CCR0_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_CCR0_FIELD0009_SHIFT                           (1U)
#define DMA_CCR0_FIELD0009_MASK                            (0x7U << DMA_CCR0_FIELD0009_SHIFT)                           /* 0x0000000E */
#define DMA_CCR0_FIELD0008_SHIFT                           (4U)
#define DMA_CCR0_FIELD0008_MASK                            (0xFU << DMA_CCR0_FIELD0008_SHIFT)                           /* 0x000000F0 */
#define DMA_CCR0_FIELD0007_SHIFT                           (8U)
#define DMA_CCR0_FIELD0007_MASK                            (0x7U << DMA_CCR0_FIELD0007_SHIFT)                           /* 0x00000700 */
#define DMA_CCR0_FIELD0006_SHIFT                           (11U)
#define DMA_CCR0_FIELD0006_MASK                            (0x7U << DMA_CCR0_FIELD0006_SHIFT)                           /* 0x00003800 */
#define DMA_CCR0_FIELD0005_SHIFT                           (14U)
#define DMA_CCR0_FIELD0005_MASK                            (0x1U << DMA_CCR0_FIELD0005_SHIFT)                           /* 0x00004000 */
#define DMA_CCR0_FIELD0004_SHIFT                           (15U)
#define DMA_CCR0_FIELD0004_MASK                            (0x7U << DMA_CCR0_FIELD0004_SHIFT)                           /* 0x00038000 */
#define DMA_CCR0_FIELD0003_SHIFT                           (18U)
#define DMA_CCR0_FIELD0003_MASK                            (0xFU << DMA_CCR0_FIELD0003_SHIFT)                           /* 0x003C0000 */
#define DMA_CCR0_FIELD0002_SHIFT                           (22U)
#define DMA_CCR0_FIELD0002_MASK                            (0x7U << DMA_CCR0_FIELD0002_SHIFT)                           /* 0x01C00000 */
#define DMA_CCR0_FIELD0001_SHIFT                           (25U)
#define DMA_CCR0_FIELD0001_MASK                            (0x7U << DMA_CCR0_FIELD0001_SHIFT)                           /* 0x0E000000 */
/* LC0_0 */
#define DMA_LC0_0_OFFSET                                   (0x40CU)
#define DMA_LC0_0                                          (0x0U)
#define DMA_LC0_0_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_0_FIELD0000_MASK                           (0xFFU << DMA_LC0_0_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_0 */
#define DMA_LC1_0_OFFSET                                   (0x410U)
#define DMA_LC1_0                                          (0x0U)
#define DMA_LC1_0_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_0_FIELD0000_MASK                           (0xFFU << DMA_LC1_0_FIELD0000_SHIFT)                         /* 0x000000FF */
/* PADDING0_0 */
#define DMA_PADDING0_0_OFFSET                              (0x414U)
/* PADDING0_1 */
#define DMA_PADDING0_1_OFFSET                              (0x418U)
/* PADDING0_2 */
#define DMA_PADDING0_2_OFFSET                              (0x41CU)
/* SAR1 */
#define DMA_SAR1_OFFSET                                    (0x420U)
#define DMA_SAR1                                           (0x0U)
#define DMA_SAR1_FIELD0000_SHIFT                           (0U)
#define DMA_SAR1_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR1_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR1 */
#define DMA_DAR1_OFFSET                                    (0x424U)
#define DMA_DAR1                                           (0x0U)
#define DMA_DAR1_FIELD0000_SHIFT                           (0U)
#define DMA_DAR1_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR1_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR1 */
#define DMA_CCR1_OFFSET                                    (0x428U)
#define DMA_CCR1                                           (0x0U)
#define DMA_CCR1_FIELD0000_SHIFT                           (0U)
#define DMA_CCR1_FIELD0000_MASK                            (0x1U << DMA_CCR1_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_CCR1_FIELD0009_SHIFT                           (1U)
#define DMA_CCR1_FIELD0009_MASK                            (0x7U << DMA_CCR1_FIELD0009_SHIFT)                           /* 0x0000000E */
#define DMA_CCR1_FIELD0008_SHIFT                           (4U)
#define DMA_CCR1_FIELD0008_MASK                            (0xFU << DMA_CCR1_FIELD0008_SHIFT)                           /* 0x000000F0 */
#define DMA_CCR1_FIELD0007_SHIFT                           (8U)
#define DMA_CCR1_FIELD0007_MASK                            (0x7U << DMA_CCR1_FIELD0007_SHIFT)                           /* 0x00000700 */
#define DMA_CCR1_FIELD0006_SHIFT                           (11U)
#define DMA_CCR1_FIELD0006_MASK                            (0x7U << DMA_CCR1_FIELD0006_SHIFT)                           /* 0x00003800 */
#define DMA_CCR1_FIELD0005_SHIFT                           (14U)
#define DMA_CCR1_FIELD0005_MASK                            (0x1U << DMA_CCR1_FIELD0005_SHIFT)                           /* 0x00004000 */
#define DMA_CCR1_FIELD0004_SHIFT                           (15U)
#define DMA_CCR1_FIELD0004_MASK                            (0x7U << DMA_CCR1_FIELD0004_SHIFT)                           /* 0x00038000 */
#define DMA_CCR1_FIELD0003_SHIFT                           (18U)
#define DMA_CCR1_FIELD0003_MASK                            (0xFU << DMA_CCR1_FIELD0003_SHIFT)                           /* 0x003C0000 */
#define DMA_CCR1_FIELD0002_SHIFT                           (22U)
#define DMA_CCR1_FIELD0002_MASK                            (0x7U << DMA_CCR1_FIELD0002_SHIFT)                           /* 0x01C00000 */
#define DMA_CCR1_FIELD0001_SHIFT                           (25U)
#define DMA_CCR1_FIELD0001_MASK                            (0x7U << DMA_CCR1_FIELD0001_SHIFT)                           /* 0x0E000000 */
/* LC0_1 */
#define DMA_LC0_1_OFFSET                                   (0x42CU)
#define DMA_LC0_1                                          (0x0U)
#define DMA_LC0_1_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_1_FIELD0000_MASK                           (0xFFU << DMA_LC0_1_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_1 */
#define DMA_LC1_1_OFFSET                                   (0x430U)
#define DMA_LC1_1                                          (0x0U)
#define DMA_LC1_1_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_1_FIELD0000_MASK                           (0xFFU << DMA_LC1_1_FIELD0000_SHIFT)                         /* 0x000000FF */
/* PADDING1_0 */
#define DMA_PADDING1_0_OFFSET                              (0x434U)
/* PADDING1_1 */
#define DMA_PADDING1_1_OFFSET                              (0x438U)
/* PADDING1_2 */
#define DMA_PADDING1_2_OFFSET                              (0x43CU)
/* SAR2 */
#define DMA_SAR2_OFFSET                                    (0x440U)
#define DMA_SAR2                                           (0x0U)
#define DMA_SAR2_FIELD0000_SHIFT                           (0U)
#define DMA_SAR2_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR2_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR2 */
#define DMA_DAR2_OFFSET                                    (0x444U)
#define DMA_DAR2                                           (0x0U)
#define DMA_DAR2_FIELD0000_SHIFT                           (0U)
#define DMA_DAR2_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR2_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR2 */
#define DMA_CCR2_OFFSET                                    (0x448U)
#define DMA_CCR2                                           (0x0U)
#define DMA_CCR2_FIELD0000_SHIFT                           (0U)
#define DMA_CCR2_FIELD0000_MASK                            (0x1U << DMA_CCR2_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_CCR2_FIELD0009_SHIFT                           (1U)
#define DMA_CCR2_FIELD0009_MASK                            (0x7U << DMA_CCR2_FIELD0009_SHIFT)                           /* 0x0000000E */
#define DMA_CCR2_FIELD0008_SHIFT                           (4U)
#define DMA_CCR2_FIELD0008_MASK                            (0xFU << DMA_CCR2_FIELD0008_SHIFT)                           /* 0x000000F0 */
#define DMA_CCR2_FIELD0007_SHIFT                           (8U)
#define DMA_CCR2_FIELD0007_MASK                            (0x7U << DMA_CCR2_FIELD0007_SHIFT)                           /* 0x00000700 */
#define DMA_CCR2_FIELD0006_SHIFT                           (11U)
#define DMA_CCR2_FIELD0006_MASK                            (0x7U << DMA_CCR2_FIELD0006_SHIFT)                           /* 0x00003800 */
#define DMA_CCR2_FIELD0005_SHIFT                           (14U)
#define DMA_CCR2_FIELD0005_MASK                            (0x1U << DMA_CCR2_FIELD0005_SHIFT)                           /* 0x00004000 */
#define DMA_CCR2_FIELD0004_SHIFT                           (15U)
#define DMA_CCR2_FIELD0004_MASK                            (0x7U << DMA_CCR2_FIELD0004_SHIFT)                           /* 0x00038000 */
#define DMA_CCR2_FIELD0003_SHIFT                           (18U)
#define DMA_CCR2_FIELD0003_MASK                            (0xFU << DMA_CCR2_FIELD0003_SHIFT)                           /* 0x003C0000 */
#define DMA_CCR2_FIELD0002_SHIFT                           (22U)
#define DMA_CCR2_FIELD0002_MASK                            (0x7U << DMA_CCR2_FIELD0002_SHIFT)                           /* 0x01C00000 */
#define DMA_CCR2_FIELD0001_SHIFT                           (25U)
#define DMA_CCR2_FIELD0001_MASK                            (0x7U << DMA_CCR2_FIELD0001_SHIFT)                           /* 0x0E000000 */
/* LC0_2 */
#define DMA_LC0_2_OFFSET                                   (0x44CU)
#define DMA_LC0_2                                          (0x0U)
#define DMA_LC0_2_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_2_FIELD0000_MASK                           (0xFFU << DMA_LC0_2_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_2 */
#define DMA_LC1_2_OFFSET                                   (0x450U)
#define DMA_LC1_2                                          (0x0U)
#define DMA_LC1_2_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_2_FIELD0000_MASK                           (0xFFU << DMA_LC1_2_FIELD0000_SHIFT)                         /* 0x000000FF */
/* PADDING2_0 */
#define DMA_PADDING2_0_OFFSET                              (0x454U)
/* PADDING2_1 */
#define DMA_PADDING2_1_OFFSET                              (0x458U)
/* PADDING2_2 */
#define DMA_PADDING2_2_OFFSET                              (0x45CU)
/* SAR3 */
#define DMA_SAR3_OFFSET                                    (0x460U)
#define DMA_SAR3                                           (0x0U)
#define DMA_SAR3_FIELD0000_SHIFT                           (0U)
#define DMA_SAR3_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR3_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR3 */
#define DMA_DAR3_OFFSET                                    (0x464U)
#define DMA_DAR3                                           (0x0U)
#define DMA_DAR3_FIELD0000_SHIFT                           (0U)
#define DMA_DAR3_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR3_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR3 */
#define DMA_CCR3_OFFSET                                    (0x468U)
#define DMA_CCR3                                           (0x0U)
#define DMA_CCR3_FIELD0000_SHIFT                           (0U)
#define DMA_CCR3_FIELD0000_MASK                            (0x1U << DMA_CCR3_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_CCR3_FIELD0009_SHIFT                           (1U)
#define DMA_CCR3_FIELD0009_MASK                            (0x7U << DMA_CCR3_FIELD0009_SHIFT)                           /* 0x0000000E */
#define DMA_CCR3_FIELD0008_SHIFT                           (4U)
#define DMA_CCR3_FIELD0008_MASK                            (0xFU << DMA_CCR3_FIELD0008_SHIFT)                           /* 0x000000F0 */
#define DMA_CCR3_FIELD0007_SHIFT                           (8U)
#define DMA_CCR3_FIELD0007_MASK                            (0x7U << DMA_CCR3_FIELD0007_SHIFT)                           /* 0x00000700 */
#define DMA_CCR3_FIELD0006_SHIFT                           (11U)
#define DMA_CCR3_FIELD0006_MASK                            (0x7U << DMA_CCR3_FIELD0006_SHIFT)                           /* 0x00003800 */
#define DMA_CCR3_FIELD0005_SHIFT                           (14U)
#define DMA_CCR3_FIELD0005_MASK                            (0x1U << DMA_CCR3_FIELD0005_SHIFT)                           /* 0x00004000 */
#define DMA_CCR3_FIELD0004_SHIFT                           (15U)
#define DMA_CCR3_FIELD0004_MASK                            (0x7U << DMA_CCR3_FIELD0004_SHIFT)                           /* 0x00038000 */
#define DMA_CCR3_FIELD0003_SHIFT                           (18U)
#define DMA_CCR3_FIELD0003_MASK                            (0xFU << DMA_CCR3_FIELD0003_SHIFT)                           /* 0x003C0000 */
#define DMA_CCR3_FIELD0002_SHIFT                           (22U)
#define DMA_CCR3_FIELD0002_MASK                            (0x7U << DMA_CCR3_FIELD0002_SHIFT)                           /* 0x01C00000 */
#define DMA_CCR3_FIELD0001_SHIFT                           (25U)
#define DMA_CCR3_FIELD0001_MASK                            (0x7U << DMA_CCR3_FIELD0001_SHIFT)                           /* 0x0E000000 */
/* LC0_3 */
#define DMA_LC0_3_OFFSET                                   (0x46CU)
#define DMA_LC0_3                                          (0x0U)
#define DMA_LC0_3_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_3_FIELD0000_MASK                           (0xFFU << DMA_LC0_3_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_3 */
#define DMA_LC1_3_OFFSET                                   (0x470U)
#define DMA_LC1_3                                          (0x0U)
#define DMA_LC1_3_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_3_FIELD0000_MASK                           (0xFFU << DMA_LC1_3_FIELD0000_SHIFT)                         /* 0x000000FF */
/* PADDING3_0 */
#define DMA_PADDING3_0_OFFSET                              (0x474U)
/* PADDING3_1 */
#define DMA_PADDING3_1_OFFSET                              (0x478U)
/* PADDING3_2 */
#define DMA_PADDING3_2_OFFSET                              (0x47CU)
/* SAR4 */
#define DMA_SAR4_OFFSET                                    (0x480U)
#define DMA_SAR4                                           (0x0U)
#define DMA_SAR4_FIELD0000_SHIFT                           (0U)
#define DMA_SAR4_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR4_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR4 */
#define DMA_DAR4_OFFSET                                    (0x484U)
#define DMA_DAR4                                           (0x0U)
#define DMA_DAR4_FIELD0000_SHIFT                           (0U)
#define DMA_DAR4_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR4_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR4 */
#define DMA_CCR4_OFFSET                                    (0x488U)
#define DMA_CCR4                                           (0x0U)
#define DMA_CCR4_FIELD0000_SHIFT                           (0U)
#define DMA_CCR4_FIELD0000_MASK                            (0x1U << DMA_CCR4_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_CCR4_FIELD0009_SHIFT                           (1U)
#define DMA_CCR4_FIELD0009_MASK                            (0x7U << DMA_CCR4_FIELD0009_SHIFT)                           /* 0x0000000E */
#define DMA_CCR4_FIELD0008_SHIFT                           (4U)
#define DMA_CCR4_FIELD0008_MASK                            (0xFU << DMA_CCR4_FIELD0008_SHIFT)                           /* 0x000000F0 */
#define DMA_CCR4_FIELD0007_SHIFT                           (8U)
#define DMA_CCR4_FIELD0007_MASK                            (0x7U << DMA_CCR4_FIELD0007_SHIFT)                           /* 0x00000700 */
#define DMA_CCR4_FIELD0006_SHIFT                           (11U)
#define DMA_CCR4_FIELD0006_MASK                            (0x7U << DMA_CCR4_FIELD0006_SHIFT)                           /* 0x00003800 */
#define DMA_CCR4_FIELD0005_SHIFT                           (14U)
#define DMA_CCR4_FIELD0005_MASK                            (0x1U << DMA_CCR4_FIELD0005_SHIFT)                           /* 0x00004000 */
#define DMA_CCR4_FIELD0004_SHIFT                           (15U)
#define DMA_CCR4_FIELD0004_MASK                            (0x7U << DMA_CCR4_FIELD0004_SHIFT)                           /* 0x00038000 */
#define DMA_CCR4_FIELD0003_SHIFT                           (18U)
#define DMA_CCR4_FIELD0003_MASK                            (0xFU << DMA_CCR4_FIELD0003_SHIFT)                           /* 0x003C0000 */
#define DMA_CCR4_FIELD0002_SHIFT                           (22U)
#define DMA_CCR4_FIELD0002_MASK                            (0x7U << DMA_CCR4_FIELD0002_SHIFT)                           /* 0x01C00000 */
#define DMA_CCR4_FIELD0001_SHIFT                           (25U)
#define DMA_CCR4_FIELD0001_MASK                            (0x7U << DMA_CCR4_FIELD0001_SHIFT)                           /* 0x0E000000 */
/* LC0_4 */
#define DMA_LC0_4_OFFSET                                   (0x48CU)
#define DMA_LC0_4                                          (0x0U)
#define DMA_LC0_4_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_4_FIELD0000_MASK                           (0xFFU << DMA_LC0_4_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_4 */
#define DMA_LC1_4_OFFSET                                   (0x490U)
#define DMA_LC1_4                                          (0x0U)
#define DMA_LC1_4_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_4_FIELD0000_MASK                           (0xFFU << DMA_LC1_4_FIELD0000_SHIFT)                         /* 0x000000FF */
/* PADDING4_0 */
#define DMA_PADDING4_0_OFFSET                              (0x494U)
/* PADDING4_1 */
#define DMA_PADDING4_1_OFFSET                              (0x498U)
/* PADDING4_2 */
#define DMA_PADDING4_2_OFFSET                              (0x49CU)
/* SAR5 */
#define DMA_SAR5_OFFSET                                    (0x4A0U)
#define DMA_SAR5                                           (0x0U)
#define DMA_SAR5_FIELD0000_SHIFT                           (0U)
#define DMA_SAR5_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR5_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR5 */
#define DMA_DAR5_OFFSET                                    (0x4A4U)
#define DMA_DAR5                                           (0x0U)
#define DMA_DAR5_FIELD0000_SHIFT                           (0U)
#define DMA_DAR5_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR5_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR5 */
#define DMA_CCR5_OFFSET                                    (0x4A8U)
#define DMA_CCR5                                           (0x0U)
#define DMA_CCR5_FIELD0000_SHIFT                           (0U)
#define DMA_CCR5_FIELD0000_MASK                            (0x1U << DMA_CCR5_FIELD0000_SHIFT)                           /* 0x00000001 */
#define DMA_CCR5_FIELD0009_SHIFT                           (1U)
#define DMA_CCR5_FIELD0009_MASK                            (0x7U << DMA_CCR5_FIELD0009_SHIFT)                           /* 0x0000000E */
#define DMA_CCR5_FIELD0008_SHIFT                           (4U)
#define DMA_CCR5_FIELD0008_MASK                            (0xFU << DMA_CCR5_FIELD0008_SHIFT)                           /* 0x000000F0 */
#define DMA_CCR5_FIELD0007_SHIFT                           (8U)
#define DMA_CCR5_FIELD0007_MASK                            (0x7U << DMA_CCR5_FIELD0007_SHIFT)                           /* 0x00000700 */
#define DMA_CCR5_FIELD0006_SHIFT                           (11U)
#define DMA_CCR5_FIELD0006_MASK                            (0x7U << DMA_CCR5_FIELD0006_SHIFT)                           /* 0x00003800 */
#define DMA_CCR5_FIELD0005_SHIFT                           (14U)
#define DMA_CCR5_FIELD0005_MASK                            (0x1U << DMA_CCR5_FIELD0005_SHIFT)                           /* 0x00004000 */
#define DMA_CCR5_FIELD0004_SHIFT                           (15U)
#define DMA_CCR5_FIELD0004_MASK                            (0x7U << DMA_CCR5_FIELD0004_SHIFT)                           /* 0x00038000 */
#define DMA_CCR5_FIELD0003_SHIFT                           (18U)
#define DMA_CCR5_FIELD0003_MASK                            (0xFU << DMA_CCR5_FIELD0003_SHIFT)                           /* 0x003C0000 */
#define DMA_CCR5_FIELD0002_SHIFT                           (22U)
#define DMA_CCR5_FIELD0002_MASK                            (0x7U << DMA_CCR5_FIELD0002_SHIFT)                           /* 0x01C00000 */
#define DMA_CCR5_FIELD0001_SHIFT                           (25U)
#define DMA_CCR5_FIELD0001_MASK                            (0x7U << DMA_CCR5_FIELD0001_SHIFT)                           /* 0x0E000000 */
/* LC0_5 */
#define DMA_LC0_5_OFFSET                                   (0x4ACU)
#define DMA_LC0_5                                          (0x0U)
#define DMA_LC0_5_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_5_FIELD0000_MASK                           (0xFFU << DMA_LC0_5_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_5 */
#define DMA_LC1_5_OFFSET                                   (0x4B0U)
#define DMA_LC1_5                                          (0x0U)
#define DMA_LC1_5_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_5_FIELD0000_MASK                           (0xFFU << DMA_LC1_5_FIELD0000_SHIFT)                         /* 0x000000FF */
/* PADDING5_0 */
#define DMA_PADDING5_0_OFFSET                              (0x4B4U)
/* PADDING5_1 */
#define DMA_PADDING5_1_OFFSET                              (0x4B8U)
/* PADDING5_2 */
#define DMA_PADDING5_2_OFFSET                              (0x4BCU)
/* DBGSTATUS */
#define DMA_DBGSTATUS_OFFSET                               (0xD00U)
#define DMA_DBGSTATUS                                      (0x0U)
#define DMA_DBGSTATUS_FIELD0000_SHIFT                      (0U)
#define DMA_DBGSTATUS_FIELD0000_MASK                       (0x3U << DMA_DBGSTATUS_FIELD0000_SHIFT)                      /* 0x00000003 */
/* DBGCMD */
#define DMA_DBGCMD_OFFSET                                  (0xD04U)
#define DMA_DBGCMD_FIELD0000_SHIFT                         (0U)
#define DMA_DBGCMD_FIELD0000_MASK                          (0x3U << DMA_DBGCMD_FIELD0000_SHIFT)                         /* 0x00000003 */
/* DBGINST0 */
#define DMA_DBGINST0_OFFSET                                (0xD08U)
#define DMA_DBGINST0_FIELD0000_SHIFT                       (0U)
#define DMA_DBGINST0_FIELD0000_MASK                        (0x1U << DMA_DBGINST0_FIELD0000_SHIFT)                       /* 0x00000001 */
#define DMA_DBGINST0_FIELD0003_SHIFT                       (8U)
#define DMA_DBGINST0_FIELD0003_MASK                        (0x7U << DMA_DBGINST0_FIELD0003_SHIFT)                       /* 0x00000700 */
#define DMA_DBGINST0_FIELD0002_SHIFT                       (16U)
#define DMA_DBGINST0_FIELD0002_MASK                        (0xFFU << DMA_DBGINST0_FIELD0002_SHIFT)                      /* 0x00FF0000 */
#define DMA_DBGINST0_FIELD0001_SHIFT                       (24U)
#define DMA_DBGINST0_FIELD0001_MASK                        (0xFFU << DMA_DBGINST0_FIELD0001_SHIFT)                      /* 0xFF000000 */
/* DBGINST1 */
#define DMA_DBGINST1_OFFSET                                (0xD0CU)
#define DMA_DBGINST1_FIELD0000_SHIFT                       (0U)
#define DMA_DBGINST1_FIELD0000_MASK                        (0xFFU << DMA_DBGINST1_FIELD0000_SHIFT)                      /* 0x000000FF */
#define DMA_DBGINST1_FIELD0003_SHIFT                       (8U)
#define DMA_DBGINST1_FIELD0003_MASK                        (0xFFU << DMA_DBGINST1_FIELD0003_SHIFT)                      /* 0x0000FF00 */
#define DMA_DBGINST1_FIELD0002_SHIFT                       (16U)
#define DMA_DBGINST1_FIELD0002_MASK                        (0xFFU << DMA_DBGINST1_FIELD0002_SHIFT)                      /* 0x00FF0000 */
#define DMA_DBGINST1_FIELD0001_SHIFT                       (24U)
#define DMA_DBGINST1_FIELD0001_MASK                        (0xFFU << DMA_DBGINST1_FIELD0001_SHIFT)                      /* 0xFF000000 */
/* CR0 */
#define DMA_CR0_OFFSET                                     (0xE00U)
#define DMA_CR0                                            (0x47051U)
#define DMA_CR0_FIELD0000_SHIFT                            (0U)
#define DMA_CR0_FIELD0000_MASK                             (0x1U << DMA_CR0_FIELD0000_SHIFT)                            /* 0x00000001 */
#define DMA_CR0_FIELD0005_SHIFT                            (1U)
#define DMA_CR0_FIELD0005_MASK                             (0x1U << DMA_CR0_FIELD0005_SHIFT)                            /* 0x00000002 */
#define DMA_CR0_FIELD0004_SHIFT                            (2U)
#define DMA_CR0_FIELD0004_MASK                             (0x1U << DMA_CR0_FIELD0004_SHIFT)                            /* 0x00000004 */
#define DMA_CR0_FIELD0003_SHIFT                            (4U)
#define DMA_CR0_FIELD0003_MASK                             (0x7U << DMA_CR0_FIELD0003_SHIFT)                            /* 0x00000070 */
#define DMA_CR0_FIELD0002_SHIFT                            (12U)
#define DMA_CR0_FIELD0002_MASK                             (0x1FU << DMA_CR0_FIELD0002_SHIFT)                           /* 0x0001F000 */
#define DMA_CR0_FIELD0001_SHIFT                            (17U)
#define DMA_CR0_FIELD0001_MASK                             (0x1FU << DMA_CR0_FIELD0001_SHIFT)                           /* 0x003E0000 */
/* CR1 */
#define DMA_CR1_OFFSET                                     (0xE04U)
#define DMA_CR1                                            (0x57U)
#define DMA_CR1_FIELD0000_SHIFT                            (0U)
#define DMA_CR1_FIELD0000_MASK                             (0x7U << DMA_CR1_FIELD0000_SHIFT)                            /* 0x00000007 */
#define DMA_CR1_FIELD0001_SHIFT                            (4U)
#define DMA_CR1_FIELD0001_MASK                             (0xFU << DMA_CR1_FIELD0001_SHIFT)                            /* 0x000000F0 */
/* CR2 */
#define DMA_CR2_OFFSET                                     (0xE08U)
#define DMA_CR2                                            (0x0U)
#define DMA_CR2_FIELD0000_SHIFT                            (0U)
#define DMA_CR2_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_CR2_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* CR3 */
#define DMA_CR3_OFFSET                                     (0xE0CU)
#define DMA_CR3                                            (0x0U)
#define DMA_CR3_FIELD0000_SHIFT                            (0U)
#define DMA_CR3_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_CR3_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* CR4 */
#define DMA_CR4_OFFSET                                     (0xE10U)
#define DMA_CR4                                            (0x6U)
#define DMA_CR4_FIELD0000_SHIFT                            (0U)
#define DMA_CR4_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_CR4_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* CRDN */
#define DMA_CRDN_OFFSET                                    (0xE14U)
#define DMA_CRDN                                           (0x2094733U)
#define DMA_CRDN_FIELD0000_SHIFT                           (0U)
#define DMA_CRDN_FIELD0000_MASK                            (0x7U << DMA_CRDN_FIELD0000_SHIFT)                           /* 0x00000007 */
#define DMA_CRDN_FIELD0005_SHIFT                           (4U)
#define DMA_CRDN_FIELD0005_MASK                            (0x7U << DMA_CRDN_FIELD0005_SHIFT)                           /* 0x00000070 */
#define DMA_CRDN_FIELD0004_SHIFT                           (8U)
#define DMA_CRDN_FIELD0004_MASK                            (0xFU << DMA_CRDN_FIELD0004_SHIFT)                           /* 0x00000F00 */
#define DMA_CRDN_FIELD0003_SHIFT                           (12U)
#define DMA_CRDN_FIELD0003_MASK                            (0x7U << DMA_CRDN_FIELD0003_SHIFT)                           /* 0x00007000 */
#define DMA_CRDN_FIELD0002_SHIFT                           (16U)
#define DMA_CRDN_FIELD0002_MASK                            (0xFU << DMA_CRDN_FIELD0002_SHIFT)                           /* 0x000F0000 */
#define DMA_CRDN_FIELD0001_SHIFT                           (20U)
#define DMA_CRDN_FIELD0001_MASK                            (0x3FFU << DMA_CRDN_FIELD0001_SHIFT)                         /* 0x3FF00000 */
/* WD */
#define DMA_WD_OFFSET                                      (0xE80U)
#define DMA_WD_FIELD0000_SHIFT                             (0U)
#define DMA_WD_FIELD0000_MASK                              (0x1U << DMA_WD_FIELD0000_SHIFT)                             /* 0x00000001 */
/*****************************************TSADC******************************************/
/* USER_CON */
#define TSADC_USER_CON_OFFSET                              (0x0U)
#define TSADC_USER_CON_ADC_INPUT_SRC_SEL_SHIFT             (0U)
#define TSADC_USER_CON_ADC_INPUT_SRC_SEL_MASK              (0x7U << TSADC_USER_CON_ADC_INPUT_SRC_SEL_SHIFT)             /* 0x00000007 */
#define TSADC_USER_CON_ADC_POWER_CTRL_SHIFT                (3U)
#define TSADC_USER_CON_ADC_POWER_CTRL_MASK                 (0x1U << TSADC_USER_CON_ADC_POWER_CTRL_SHIFT)                /* 0x00000008 */
#define TSADC_USER_CON_START_MODE_SHIFT                    (4U)
#define TSADC_USER_CON_START_MODE_MASK                     (0x1U << TSADC_USER_CON_START_MODE_SHIFT)                    /* 0x00000010 */
#define TSADC_USER_CON_START_SHIFT                         (5U)
#define TSADC_USER_CON_START_MASK                          (0x1U << TSADC_USER_CON_START_SHIFT)                         /* 0x00000020 */
#define TSADC_USER_CON_INTER_PD_SOC_SHIFT                  (6U)
#define TSADC_USER_CON_INTER_PD_SOC_MASK                   (0x1FFU << TSADC_USER_CON_INTER_PD_SOC_SHIFT)                /* 0x00007FC0 */
#define TSADC_USER_CON_ADC_STATUS_SHIFT                    (15U)
#define TSADC_USER_CON_ADC_STATUS_MASK                     (0x1U << TSADC_USER_CON_ADC_STATUS_SHIFT)                    /* 0x00008000 */
/* AUTO_CON */
#define TSADC_AUTO_CON_OFFSET                              (0x4U)
#define TSADC_AUTO_CON_AUTO_EN_SHIFT                       (0U)
#define TSADC_AUTO_CON_AUTO_EN_MASK                        (0x1U << TSADC_AUTO_CON_AUTO_EN_SHIFT)                       /* 0x00000001 */
#define TSADC_AUTO_CON_TSADC_Q_SEL_SHIFT                   (1U)
#define TSADC_AUTO_CON_TSADC_Q_SEL_MASK                    (0x1U << TSADC_AUTO_CON_TSADC_Q_SEL_SHIFT)                   /* 0x00000002 */
#define TSADC_AUTO_CON_SRC0_EN_SHIFT                       (4U)
#define TSADC_AUTO_CON_SRC0_EN_MASK                        (0x1U << TSADC_AUTO_CON_SRC0_EN_SHIFT)                       /* 0x00000010 */
#define TSADC_AUTO_CON_SRC1_EN_SHIFT                       (5U)
#define TSADC_AUTO_CON_SRC1_EN_MASK                        (0x1U << TSADC_AUTO_CON_SRC1_EN_SHIFT)                       /* 0x00000020 */
#define TSADC_AUTO_CON_TSHUT_PROLARITY_SHIFT               (8U)
#define TSADC_AUTO_CON_TSHUT_PROLARITY_MASK                (0x1U << TSADC_AUTO_CON_TSHUT_PROLARITY_SHIFT)               /* 0x00000100 */
#define TSADC_AUTO_CON_SRC0_LT_EN_SHIFT                    (12U)
#define TSADC_AUTO_CON_SRC0_LT_EN_MASK                     (0x1U << TSADC_AUTO_CON_SRC0_LT_EN_SHIFT)                    /* 0x00001000 */
#define TSADC_AUTO_CON_SRC1_LT_EN_SHIFT                    (13U)
#define TSADC_AUTO_CON_SRC1_LT_EN_MASK                     (0x1U << TSADC_AUTO_CON_SRC1_LT_EN_SHIFT)                    /* 0x00002000 */
#define TSADC_AUTO_CON_AUTO_STATUS_SHIFT                   (16U)
#define TSADC_AUTO_CON_AUTO_STATUS_MASK                    (0x1U << TSADC_AUTO_CON_AUTO_STATUS_SHIFT)                   /* 0x00010000 */
#define TSADC_AUTO_CON_SAMPLE_DLY_SEL_SHIFT                (17U)
#define TSADC_AUTO_CON_SAMPLE_DLY_SEL_MASK                 (0x1U << TSADC_AUTO_CON_SAMPLE_DLY_SEL_SHIFT)                /* 0x00020000 */
#define TSADC_AUTO_CON_LAST_TSHUT_2GPIO_SHIFT              (24U)
#define TSADC_AUTO_CON_LAST_TSHUT_2GPIO_MASK               (0x1U << TSADC_AUTO_CON_LAST_TSHUT_2GPIO_SHIFT)              /* 0x01000000 */
#define TSADC_AUTO_CON_LAST_TSHUT_2CRU_SHIFT               (25U)
#define TSADC_AUTO_CON_LAST_TSHUT_2CRU_MASK                (0x1U << TSADC_AUTO_CON_LAST_TSHUT_2CRU_SHIFT)               /* 0x02000000 */
/* INT_EN */
#define TSADC_INT_EN_OFFSET                                (0x8U)
#define TSADC_INT_EN_HT_INTEN_SRC0_SHIFT                   (0U)
#define TSADC_INT_EN_HT_INTEN_SRC0_MASK                    (0x1U << TSADC_INT_EN_HT_INTEN_SRC0_SHIFT)                   /* 0x00000001 */
#define TSADC_INT_EN_HT_INTEN_SRC1_SHIFT                   (1U)
#define TSADC_INT_EN_HT_INTEN_SRC1_MASK                    (0x1U << TSADC_INT_EN_HT_INTEN_SRC1_SHIFT)                   /* 0x00000002 */
#define TSADC_INT_EN_TSHUT_2GPIO_EN_SRC0_SHIFT             (4U)
#define TSADC_INT_EN_TSHUT_2GPIO_EN_SRC0_MASK              (0x1U << TSADC_INT_EN_TSHUT_2GPIO_EN_SRC0_SHIFT)             /* 0x00000010 */
#define TSADC_INT_EN_TSHUT_2GPIO_EN_SRC1_SHIFT             (5U)
#define TSADC_INT_EN_TSHUT_2GPIO_EN_SRC1_MASK              (0x1U << TSADC_INT_EN_TSHUT_2GPIO_EN_SRC1_SHIFT)             /* 0x00000020 */
#define TSADC_INT_EN_TSHUT_2CRU_EN_SRC0_SHIFT              (8U)
#define TSADC_INT_EN_TSHUT_2CRU_EN_SRC0_MASK               (0x1U << TSADC_INT_EN_TSHUT_2CRU_EN_SRC0_SHIFT)              /* 0x00000100 */
#define TSADC_INT_EN_TSHUT_2CRU_EN_SRC1_SHIFT              (9U)
#define TSADC_INT_EN_TSHUT_2CRU_EN_SRC1_MASK               (0x1U << TSADC_INT_EN_TSHUT_2CRU_EN_SRC1_SHIFT)              /* 0x00000200 */
#define TSADC_INT_EN_LT_INTEN_SRC0_SHIFT                   (12U)
#define TSADC_INT_EN_LT_INTEN_SRC0_MASK                    (0x1U << TSADC_INT_EN_LT_INTEN_SRC0_SHIFT)                   /* 0x00001000 */
#define TSADC_INT_EN_LT_INTEN_SRC1_SHIFT                   (13U)
#define TSADC_INT_EN_LT_INTEN_SRC1_MASK                    (0x1U << TSADC_INT_EN_LT_INTEN_SRC1_SHIFT)                   /* 0x00002000 */
#define TSADC_INT_EN_EOC_INT_EN_SHIFT                      (16U)
#define TSADC_INT_EN_EOC_INT_EN_MASK                       (0x1U << TSADC_INT_EN_EOC_INT_EN_SHIFT)                      /* 0x00010000 */
/* INT_PD */
#define TSADC_INT_PD_OFFSET                                (0xCU)
#define TSADC_INT_PD_HT_IRQ_SRC0_SHIFT                     (0U)
#define TSADC_INT_PD_HT_IRQ_SRC0_MASK                      (0x1U << TSADC_INT_PD_HT_IRQ_SRC0_SHIFT)                     /* 0x00000001 */
#define TSADC_INT_PD_HT_IRQ_SRC1_SHIFT                     (1U)
#define TSADC_INT_PD_HT_IRQ_SRC1_MASK                      (0x1U << TSADC_INT_PD_HT_IRQ_SRC1_SHIFT)                     /* 0x00000002 */
#define TSADC_INT_PD_TSHUT_O_SRC0_SHIFT                    (4U)
#define TSADC_INT_PD_TSHUT_O_SRC0_MASK                     (0x1U << TSADC_INT_PD_TSHUT_O_SRC0_SHIFT)                    /* 0x00000010 */
#define TSADC_INT_PD_TSHUT_O_SRC1_SHIFT                    (5U)
#define TSADC_INT_PD_TSHUT_O_SRC1_MASK                     (0x1U << TSADC_INT_PD_TSHUT_O_SRC1_SHIFT)                    /* 0x00000020 */
#define TSADC_INT_PD_LT_IRQ_SRC0_SHIFT                     (12U)
#define TSADC_INT_PD_LT_IRQ_SRC0_MASK                      (0x1U << TSADC_INT_PD_LT_IRQ_SRC0_SHIFT)                     /* 0x00001000 */
#define TSADC_INT_PD_LT_IRQ_SRC1_SHIFT                     (13U)
#define TSADC_INT_PD_LT_IRQ_SRC1_MASK                      (0x1U << TSADC_INT_PD_LT_IRQ_SRC1_SHIFT)                     /* 0x00002000 */
#define TSADC_INT_PD_EOC_INT_PD_SHIFT                      (16U)
#define TSADC_INT_PD_EOC_INT_PD_MASK                       (0x1U << TSADC_INT_PD_EOC_INT_PD_SHIFT)                      /* 0x00010000 */
/* DATA0 */
#define TSADC_DATA0_OFFSET                                 (0x20U)
#define TSADC_DATA0                                        (0x0U)
#define TSADC_DATA0_ADC_DATA_SHIFT                         (0U)
#define TSADC_DATA0_ADC_DATA_MASK                          (0xFFFU << TSADC_DATA0_ADC_DATA_SHIFT)                       /* 0x00000FFF */
/* DATA1 */
#define TSADC_DATA1_OFFSET                                 (0x24U)
#define TSADC_DATA1                                        (0x0U)
#define TSADC_DATA1_ADC_DATA_SHIFT                         (0U)
#define TSADC_DATA1_ADC_DATA_MASK                          (0xFFFU << TSADC_DATA1_ADC_DATA_SHIFT)                       /* 0x00000FFF */
/* COMP0_INT */
#define TSADC_COMP0_INT_OFFSET                             (0x30U)
#define TSADC_COMP0_INT_TSADC_COMP_SRC0_SHIFT              (0U)
#define TSADC_COMP0_INT_TSADC_COMP_SRC0_MASK               (0xFFFU << TSADC_COMP0_INT_TSADC_COMP_SRC0_SHIFT)            /* 0x00000FFF */
/* COMP1_INT */
#define TSADC_COMP1_INT_OFFSET                             (0x34U)
#define TSADC_COMP1_INT_TSADC_COMP_SRC1_SHIFT              (0U)
#define TSADC_COMP1_INT_TSADC_COMP_SRC1_MASK               (0xFFFU << TSADC_COMP1_INT_TSADC_COMP_SRC1_SHIFT)            /* 0x00000FFF */
/* COMP0_SHUT */
#define TSADC_COMP0_SHUT_OFFSET                            (0x40U)
#define TSADC_COMP0_SHUT_TSADC_COMP_SRC0_SHIFT             (0U)
#define TSADC_COMP0_SHUT_TSADC_COMP_SRC0_MASK              (0xFFFU << TSADC_COMP0_SHUT_TSADC_COMP_SRC0_SHIFT)           /* 0x00000FFF */
/* COMP1_SHUT */
#define TSADC_COMP1_SHUT_OFFSET                            (0x44U)
#define TSADC_COMP1_SHUT_TSADC_COMP_SRC1_SHIFT             (0U)
#define TSADC_COMP1_SHUT_TSADC_COMP_SRC1_MASK              (0xFFFU << TSADC_COMP1_SHUT_TSADC_COMP_SRC1_SHIFT)           /* 0x00000FFF */
/* HIGHT_INT_DEBOUNCE */
#define TSADC_HIGHT_INT_DEBOUNCE_OFFSET                    (0x60U)
#define TSADC_HIGHT_INT_DEBOUNCE_DEBOUNCE_SHIFT            (0U)
#define TSADC_HIGHT_INT_DEBOUNCE_DEBOUNCE_MASK             (0xFFU << TSADC_HIGHT_INT_DEBOUNCE_DEBOUNCE_SHIFT)           /* 0x000000FF */
/* HIGHT_TSHUT_DEBOUNCE */
#define TSADC_HIGHT_TSHUT_DEBOUNCE_OFFSET                  (0x64U)
#define TSADC_HIGHT_TSHUT_DEBOUNCE_DEBOUNCE_SHIFT          (0U)
#define TSADC_HIGHT_TSHUT_DEBOUNCE_DEBOUNCE_MASK           (0xFFU << TSADC_HIGHT_TSHUT_DEBOUNCE_DEBOUNCE_SHIFT)         /* 0x000000FF */
/* AUTO_PERIOD */
#define TSADC_AUTO_PERIOD_OFFSET                           (0x68U)
#define TSADC_AUTO_PERIOD_AUTO_PERIOD_SHIFT                (0U)
#define TSADC_AUTO_PERIOD_AUTO_PERIOD_MASK                 (0xFFFFFFFFU << TSADC_AUTO_PERIOD_AUTO_PERIOD_SHIFT)         /* 0xFFFFFFFF */
/* AUTO_PERIOD_HT */
#define TSADC_AUTO_PERIOD_HT_OFFSET                        (0x6CU)
#define TSADC_AUTO_PERIOD_HT_AUTO_PERIOD_SHIFT             (0U)
#define TSADC_AUTO_PERIOD_HT_AUTO_PERIOD_MASK              (0xFFFFFFFFU << TSADC_AUTO_PERIOD_HT_AUTO_PERIOD_SHIFT)      /* 0xFFFFFFFF */
/* COMP0_LOW_INT */
#define TSADC_COMP0_LOW_INT_OFFSET                         (0x80U)
#define TSADC_COMP0_LOW_INT_TSADC_COMP_SRC0_SHIFT          (0U)
#define TSADC_COMP0_LOW_INT_TSADC_COMP_SRC0_MASK           (0xFFFU << TSADC_COMP0_LOW_INT_TSADC_COMP_SRC0_SHIFT)        /* 0x00000FFF */
/* COMP1_LOW_INT */
#define TSADC_COMP1_LOW_INT_OFFSET                         (0x84U)
#define TSADC_COMP1_LOW_INT_TSADC_COMP_SRC1_SHIFT          (0U)
#define TSADC_COMP1_LOW_INT_TSADC_COMP_SRC1_MASK           (0xFFFU << TSADC_COMP1_LOW_INT_TSADC_COMP_SRC1_SHIFT)        /* 0x00000FFF */
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
/******************************************CRU*******************************************/
/* APLL_CON0 */
#define CRU_APLL_CON0_OFFSET                               (0x0U)
#define CRU_APLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_APLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_APLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_APLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_APLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_APLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_APLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_APLL_CON0_BYPASS_MASK                          (0x1U << CRU_APLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* APLL_CON1 */
#define CRU_APLL_CON1_OFFSET                               (0x4U)
#define CRU_APLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_APLL_CON1_REFDIV_MASK                          (0x3FU << CRU_APLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_APLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_APLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_APLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_APLL_CON1_PLL_LOCK_SHIFT                       (10U)
#define CRU_APLL_CON1_PLL_LOCK_MASK                        (0x1U << CRU_APLL_CON1_PLL_LOCK_SHIFT)                       /* 0x00000400 */
#define CRU_APLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_APLL_CON1_DSMPD_MASK                           (0x1U << CRU_APLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_APLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_APLL_CON1_PLLPD0_MASK                          (0x1U << CRU_APLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_APLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_APLL_CON1_PLLPD1_MASK                          (0x1U << CRU_APLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_APLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_APLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_APLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* APLL_CON2 */
#define CRU_APLL_CON2_OFFSET                               (0x8U)
#define CRU_APLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_APLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_APLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_APLL_CON2_DACPD_SHIFT                          (24U)
#define CRU_APLL_CON2_DACPD_MASK                           (0x1U << CRU_APLL_CON2_DACPD_SHIFT)                          /* 0x01000000 */
#define CRU_APLL_CON2_FOUTPOSTDIVPD_SHIFT                  (25U)
#define CRU_APLL_CON2_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_APLL_CON2_FOUTPOSTDIVPD_SHIFT)                  /* 0x02000000 */
#define CRU_APLL_CON2_FOUTVCOPD_SHIFT                      (26U)
#define CRU_APLL_CON2_FOUTVCOPD_MASK                       (0x1U << CRU_APLL_CON2_FOUTVCOPD_SHIFT)                      /* 0x04000000 */
#define CRU_APLL_CON2_FOUT4PHASEPD_SHIFT                   (27U)
#define CRU_APLL_CON2_FOUT4PHASEPD_MASK                    (0x1U << CRU_APLL_CON2_FOUT4PHASEPD_SHIFT)                   /* 0x08000000 */
/* APLL_CON3 */
#define CRU_APLL_CON3_OFFSET                               (0xCU)
#define CRU_APLL_CON3_SSMOD_BP_SHIFT                       (0U)
#define CRU_APLL_CON3_SSMOD_BP_MASK                        (0x1U << CRU_APLL_CON3_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_APLL_CON3_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_APLL_CON3_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_APLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_APLL_CON3_SSMOD_RESET_SHIFT                    (2U)
#define CRU_APLL_CON3_SSMOD_RESET_MASK                     (0x1U << CRU_APLL_CON3_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_APLL_CON3_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_APLL_CON3_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_APLL_CON3_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_APLL_CON3_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_APLL_CON3_SSMOD_DIVVAL_MASK                    (0xFU << CRU_APLL_CON3_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_APLL_CON3_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_APLL_CON3_SSMOD_SPREAD_MASK                    (0x1FU << CRU_APLL_CON3_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* APLL_CON4 */
#define CRU_APLL_CON4_OFFSET                               (0x10U)
#define CRU_APLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_APLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_APLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_APLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_APLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_APLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* DPLL_CON0 */
#define CRU_DPLL_CON0_OFFSET                               (0x20U)
#define CRU_DPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_DPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_DPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_DPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_DPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_DPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_DPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_DPLL_CON0_BYPASS_MASK                          (0x1U << CRU_DPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* DPLL_CON1 */
#define CRU_DPLL_CON1_OFFSET                               (0x24U)
#define CRU_DPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_DPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_DPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_DPLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_DPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_DPLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_DPLL_CON1_PLL_LOCK_SHIFT                       (10U)
#define CRU_DPLL_CON1_PLL_LOCK_MASK                        (0x1U << CRU_DPLL_CON1_PLL_LOCK_SHIFT)                       /* 0x00000400 */
#define CRU_DPLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_DPLL_CON1_DSMPD_MASK                           (0x1U << CRU_DPLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_DPLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_DPLL_CON1_PLLPD0_MASK                          (0x1U << CRU_DPLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_DPLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_DPLL_CON1_PLLPD1_MASK                          (0x1U << CRU_DPLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_DPLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_DPLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_DPLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* DPLL_CON2 */
#define CRU_DPLL_CON2_OFFSET                               (0x28U)
#define CRU_DPLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_DPLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_DPLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_DPLL_CON2_DACPD_SHIFT                          (24U)
#define CRU_DPLL_CON2_DACPD_MASK                           (0x1U << CRU_DPLL_CON2_DACPD_SHIFT)                          /* 0x01000000 */
#define CRU_DPLL_CON2_FOUTPOSTDIVPD_SHIFT                  (25U)
#define CRU_DPLL_CON2_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_DPLL_CON2_FOUTPOSTDIVPD_SHIFT)                  /* 0x02000000 */
#define CRU_DPLL_CON2_FOUTVCOPD_SHIFT                      (26U)
#define CRU_DPLL_CON2_FOUTVCOPD_MASK                       (0x1U << CRU_DPLL_CON2_FOUTVCOPD_SHIFT)                      /* 0x04000000 */
#define CRU_DPLL_CON2_FOUT4PHASEPD_SHIFT                   (27U)
#define CRU_DPLL_CON2_FOUT4PHASEPD_MASK                    (0x1U << CRU_DPLL_CON2_FOUT4PHASEPD_SHIFT)                   /* 0x08000000 */
/* DPLL_CON3 */
#define CRU_DPLL_CON3_OFFSET                               (0x2CU)
#define CRU_DPLL_CON3_SSMOD_BP_SHIFT                       (0U)
#define CRU_DPLL_CON3_SSMOD_BP_MASK                        (0x1U << CRU_DPLL_CON3_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_DPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_DPLL_CON3_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_DPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_DPLL_CON3_SSMOD_RESET_SHIFT                    (2U)
#define CRU_DPLL_CON3_SSMOD_RESET_MASK                     (0x1U << CRU_DPLL_CON3_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_DPLL_CON3_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_DPLL_CON3_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_DPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_DPLL_CON3_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_DPLL_CON3_SSMOD_DIVVAL_MASK                    (0xFU << CRU_DPLL_CON3_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_DPLL_CON3_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_DPLL_CON3_SSMOD_SPREAD_MASK                    (0x1FU << CRU_DPLL_CON3_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* DPLL_CON4 */
#define CRU_DPLL_CON4_OFFSET                               (0x30U)
#define CRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_DPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_DPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_DPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* CPLL_CON0 */
#define CRU_CPLL_CON0_OFFSET                               (0x40U)
#define CRU_CPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_CPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_CPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_CPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_CPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_CPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_CPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_CPLL_CON0_BYPASS_MASK                          (0x1U << CRU_CPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* CPLL_CON1 */
#define CRU_CPLL_CON1_OFFSET                               (0x44U)
#define CRU_CPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_CPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_CPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_CPLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_CPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_CPLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_CPLL_CON1_PLL_LOCK_SHIFT                       (10U)
#define CRU_CPLL_CON1_PLL_LOCK_MASK                        (0x1U << CRU_CPLL_CON1_PLL_LOCK_SHIFT)                       /* 0x00000400 */
#define CRU_CPLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_CPLL_CON1_DSMPD_MASK                           (0x1U << CRU_CPLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_CPLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_CPLL_CON1_PLLPD0_MASK                          (0x1U << CRU_CPLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_CPLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_CPLL_CON1_PLLPD1_MASK                          (0x1U << CRU_CPLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_CPLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_CPLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_CPLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* CPLL_CON2 */
#define CRU_CPLL_CON2_OFFSET                               (0x48U)
#define CRU_CPLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_CPLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_CPLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_CPLL_CON2_DACPD_SHIFT                          (24U)
#define CRU_CPLL_CON2_DACPD_MASK                           (0x1U << CRU_CPLL_CON2_DACPD_SHIFT)                          /* 0x01000000 */
#define CRU_CPLL_CON2_FOUTPOSTDIVPD_SHIFT                  (25U)
#define CRU_CPLL_CON2_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_CPLL_CON2_FOUTPOSTDIVPD_SHIFT)                  /* 0x02000000 */
#define CRU_CPLL_CON2_FOUTVCOPD_SHIFT                      (26U)
#define CRU_CPLL_CON2_FOUTVCOPD_MASK                       (0x1U << CRU_CPLL_CON2_FOUTVCOPD_SHIFT)                      /* 0x04000000 */
#define CRU_CPLL_CON2_FOUT4PHASEPD_SHIFT                   (27U)
#define CRU_CPLL_CON2_FOUT4PHASEPD_MASK                    (0x1U << CRU_CPLL_CON2_FOUT4PHASEPD_SHIFT)                   /* 0x08000000 */
/* CPLL_CON3 */
#define CRU_CPLL_CON3_OFFSET                               (0x4CU)
#define CRU_CPLL_CON3_SSMOD_BP_SHIFT                       (0U)
#define CRU_CPLL_CON3_SSMOD_BP_MASK                        (0x1U << CRU_CPLL_CON3_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_CPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_CPLL_CON3_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_CPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_CPLL_CON3_SSMOD_RESET_SHIFT                    (2U)
#define CRU_CPLL_CON3_SSMOD_RESET_MASK                     (0x1U << CRU_CPLL_CON3_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_CPLL_CON3_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_CPLL_CON3_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_CPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_CPLL_CON3_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_CPLL_CON3_SSMOD_DIVVAL_MASK                    (0xFU << CRU_CPLL_CON3_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_CPLL_CON3_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_CPLL_CON3_SSMOD_SPREAD_MASK                    (0x1FU << CRU_CPLL_CON3_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* CPLL_CON4 */
#define CRU_CPLL_CON4_OFFSET                               (0x50U)
#define CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* NPLL_CON0 */
#define CRU_NPLL_CON0_OFFSET                               (0x60U)
#define CRU_NPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_NPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_NPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_NPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_NPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_NPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_NPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_NPLL_CON0_BYPASS_MASK                          (0x1U << CRU_NPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* NPLL_CON1 */
#define CRU_NPLL_CON1_OFFSET                               (0x64U)
#define CRU_NPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_NPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_NPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_NPLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_NPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_NPLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_NPLL_CON1_PLL_LOCK_SHIFT                       (10U)
#define CRU_NPLL_CON1_PLL_LOCK_MASK                        (0x1U << CRU_NPLL_CON1_PLL_LOCK_SHIFT)                       /* 0x00000400 */
#define CRU_NPLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_NPLL_CON1_DSMPD_MASK                           (0x1U << CRU_NPLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_NPLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_NPLL_CON1_PLLPD0_MASK                          (0x1U << CRU_NPLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_NPLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_NPLL_CON1_PLLPD1_MASK                          (0x1U << CRU_NPLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_NPLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_NPLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_NPLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* NPLL_CON2 */
#define CRU_NPLL_CON2_OFFSET                               (0x68U)
#define CRU_NPLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_NPLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_NPLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_NPLL_CON2_DACPD_SHIFT                          (24U)
#define CRU_NPLL_CON2_DACPD_MASK                           (0x1U << CRU_NPLL_CON2_DACPD_SHIFT)                          /* 0x01000000 */
#define CRU_NPLL_CON2_FOUTPOSTDIVPD_SHIFT                  (25U)
#define CRU_NPLL_CON2_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_NPLL_CON2_FOUTPOSTDIVPD_SHIFT)                  /* 0x02000000 */
#define CRU_NPLL_CON2_FOUTVCOPD_SHIFT                      (26U)
#define CRU_NPLL_CON2_FOUTVCOPD_MASK                       (0x1U << CRU_NPLL_CON2_FOUTVCOPD_SHIFT)                      /* 0x04000000 */
#define CRU_NPLL_CON2_FOUT4PHASEPD_SHIFT                   (27U)
#define CRU_NPLL_CON2_FOUT4PHASEPD_MASK                    (0x1U << CRU_NPLL_CON2_FOUT4PHASEPD_SHIFT)                   /* 0x08000000 */
/* NPLL_CON3 */
#define CRU_NPLL_CON3_OFFSET                               (0x6CU)
#define CRU_NPLL_CON3_SSMOD_BP_SHIFT                       (0U)
#define CRU_NPLL_CON3_SSMOD_BP_MASK                        (0x1U << CRU_NPLL_CON3_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_NPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_NPLL_CON3_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_NPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_NPLL_CON3_SSMOD_RESET_SHIFT                    (2U)
#define CRU_NPLL_CON3_SSMOD_RESET_MASK                     (0x1U << CRU_NPLL_CON3_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_NPLL_CON3_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_NPLL_CON3_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_NPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_NPLL_CON3_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_NPLL_CON3_SSMOD_DIVVAL_MASK                    (0xFU << CRU_NPLL_CON3_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_NPLL_CON3_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_NPLL_CON3_SSMOD_SPREAD_MASK                    (0x1FU << CRU_NPLL_CON3_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* NPLL_CON4 */
#define CRU_NPLL_CON4_OFFSET                               (0x70U)
#define CRU_NPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_NPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_NPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_NPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_NPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_NPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* MODE_CON00 */
#define CRU_MODE_CON00_OFFSET                              (0xA0U)
#define CRU_MODE_CON00_APLL_WORK_MODE_SHIFT                (0U)
#define CRU_MODE_CON00_APLL_WORK_MODE_MASK                 (0x3U << CRU_MODE_CON00_APLL_WORK_MODE_SHIFT)                /* 0x00000003 */
#define CRU_MODE_CON00_CPLL_WORK_MODE_SHIFT                (2U)
#define CRU_MODE_CON00_CPLL_WORK_MODE_MASK                 (0x3U << CRU_MODE_CON00_CPLL_WORK_MODE_SHIFT)                /* 0x0000000C */
#define CRU_MODE_CON00_DPLL_WORK_MODE_SHIFT                (4U)
#define CRU_MODE_CON00_DPLL_WORK_MODE_MASK                 (0x3U << CRU_MODE_CON00_DPLL_WORK_MODE_SHIFT)                /* 0x00000030 */
#define CRU_MODE_CON00_NPLL_WORK_MODE_SHIFT                (6U)
#define CRU_MODE_CON00_NPLL_WORK_MODE_MASK                 (0x3U << CRU_MODE_CON00_NPLL_WORK_MODE_SHIFT)                /* 0x000000C0 */
#define CRU_MODE_CON00_USBPHY480M_WORK_MODE_SHIFT          (8U)
#define CRU_MODE_CON00_USBPHY480M_WORK_MODE_MASK           (0x3U << CRU_MODE_CON00_USBPHY480M_WORK_MODE_SHIFT)          /* 0x00000300 */
/* MISC_CON */
#define CRU_MISC_CON_OFFSET                                (0xA4U)
#define CRU_MISC_CON_WARMRST_EN_SHIFT                      (0U)
#define CRU_MISC_CON_WARMRST_EN_MASK                       (0x1U << CRU_MISC_CON_WARMRST_EN_SHIFT)                      /* 0x00000001 */
#define CRU_MISC_CON_CORE_SRST_WFIEN_SHIFT                 (1U)
#define CRU_MISC_CON_CORE_SRST_WFIEN_MASK                  (0x1U << CRU_MISC_CON_CORE_SRST_WFIEN_SHIFT)                 /* 0x00000002 */
#define CRU_MISC_CON_CORE_WRST_WFIEN_SHIFT                 (2U)
#define CRU_MISC_CON_CORE_WRST_WFIEN_MASK                  (0x1U << CRU_MISC_CON_CORE_WRST_WFIEN_SHIFT)                 /* 0x00000004 */
#define CRU_MISC_CON_COREPO_SRST_WFIEN_SHIFT               (3U)
#define CRU_MISC_CON_COREPO_SRST_WFIEN_MASK                (0x1U << CRU_MISC_CON_COREPO_SRST_WFIEN_SHIFT)               /* 0x00000008 */
#define CRU_MISC_CON_COREPO_WRST_WFIEN_SHIFT               (4U)
#define CRU_MISC_CON_COREPO_WRST_WFIEN_MASK                (0x1U << CRU_MISC_CON_COREPO_WRST_WFIEN_SHIFT)               /* 0x00000010 */
#define CRU_MISC_CON_CORE_HIGH_FREQ_RST_EN_SHIFT           (12U)
#define CRU_MISC_CON_CORE_HIGH_FREQ_RST_EN_MASK            (0xFU << CRU_MISC_CON_CORE_HIGH_FREQ_RST_EN_SHIFT)           /* 0x0000F000 */
/* GLB_CNT_TH */
#define CRU_GLB_CNT_TH_OFFSET                              (0xB0U)
#define CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT (0U)
#define CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_MASK (0xFFFFU << CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT) /* 0x0000FFFF */
#define CRU_GLB_CNT_TH_PLL_LOCKPERIOD_SHIFT                (16U)
#define CRU_GLB_CNT_TH_PLL_LOCKPERIOD_MASK                 (0xFFFFU << CRU_GLB_CNT_TH_PLL_LOCKPERIOD_SHIFT)             /* 0xFFFF0000 */
/* GLB_RST_ST */
#define CRU_GLB_RST_ST_OFFSET                              (0xB4U)
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
#define CRU_GLB_RST_ST_RESETN_CORE_SRC_ST_SHIFT            (16U)
#define CRU_GLB_RST_ST_RESETN_CORE_SRC_ST_MASK             (0xFU << CRU_GLB_RST_ST_RESETN_CORE_SRC_ST_SHIFT)            /* 0x000F0000 */
#define CRU_GLB_RST_ST_RESETN_COREPO_SRC_ST_SHIFT          (20U)
#define CRU_GLB_RST_ST_RESETN_COREPO_SRC_ST_MASK           (0xFU << CRU_GLB_RST_ST_RESETN_COREPO_SRC_ST_SHIFT)          /* 0x00F00000 */
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
#define CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT          (0U)
#define CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_MASK           (0x1U << CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT)          /* 0x00000001 */
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT            (1U)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK             (0x1U << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT)            /* 0x00000002 */
#define CRU_GLB_RST_CON_PMU_SRST_CTRL_SHIFT                (2U)
#define CRU_GLB_RST_CON_PMU_SRST_CTRL_MASK                 (0x1U << CRU_GLB_RST_CON_PMU_SRST_CTRL_SHIFT)                /* 0x00000004 */
#define CRU_GLB_RST_CON_PMU_SRST_GLB_RST_EN_SHIFT          (3U)
#define CRU_GLB_RST_CON_PMU_SRST_GLB_RST_EN_MASK           (0x1U << CRU_GLB_RST_CON_PMU_SRST_GLB_RST_EN_SHIFT)          /* 0x00000008 */
#define CRU_GLB_RST_CON_PMU_SRST_WDT_EN_SHIFT              (4U)
#define CRU_GLB_RST_CON_PMU_SRST_WDT_EN_MASK               (0x1U << CRU_GLB_RST_CON_PMU_SRST_WDT_EN_SHIFT)              /* 0x00000010 */
#define CRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_SHIFT      (6U)
#define CRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_MASK       (0x1U << CRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_SHIFT)      /* 0x00000040 */
#define CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT             (7U)
#define CRU_GLB_RST_CON_WDT_RESET_EXT_EN_MASK              (0x1U << CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT)             /* 0x00000080 */
/* HWFFC_CON0 */
#define CRU_HWFFC_CON0_OFFSET                              (0xE0U)
#define CRU_HWFFC_CON0_CLK_CORE_DIV_CON_SHIFT              (0U)
#define CRU_HWFFC_CON0_CLK_CORE_DIV_CON_MASK               (0xFU << CRU_HWFFC_CON0_CLK_CORE_DIV_CON_SHIFT)              /* 0x0000000F */
#define CRU_HWFFC_CON0_BACKUPPLL_SEL_SHIFT                 (7U)
#define CRU_HWFFC_CON0_BACKUPPLL_SEL_MASK                  (0x1U << CRU_HWFFC_CON0_BACKUPPLL_SEL_SHIFT)                 /* 0x00000080 */
#define CRU_HWFFC_CON0_HWFFC_REQ_SHIFT                     (8U)
#define CRU_HWFFC_CON0_HWFFC_REQ_MASK                      (0x1U << CRU_HWFFC_CON0_HWFFC_REQ_SHIFT)                     /* 0x00000100 */
#define CRU_HWFFC_CON0_HWFFC_MODE_SHIFT                    (9U)
#define CRU_HWFFC_CON0_HWFFC_MODE_MASK                     (0x1U << CRU_HWFFC_CON0_HWFFC_MODE_SHIFT)                    /* 0x00000200 */
/* HWFFC_TH */
#define CRU_HWFFC_TH_OFFSET                                (0xE8U)
#define CRU_HWFFC_TH_PLL_PD_EXTEND_TH_SHIFT                (0U)
#define CRU_HWFFC_TH_PLL_PD_EXTEND_TH_MASK                 (0xFFU << CRU_HWFFC_TH_PLL_PD_EXTEND_TH_SHIFT)               /* 0x000000FF */
#define CRU_HWFFC_TH_SWITCH_TH_SHIFT                       (8U)
#define CRU_HWFFC_TH_SWITCH_TH_MASK                        (0xFFU << CRU_HWFFC_TH_SWITCH_TH_SHIFT)                      /* 0x0000FF00 */
/* HWFFC_INTST */
#define CRU_HWFFC_INTST_OFFSET                             (0xECU)
#define CRU_HWFFC_INTST_HWFFC_DONE_SHIFT                   (0U)
#define CRU_HWFFC_INTST_HWFFC_DONE_MASK                    (0x1U << CRU_HWFFC_INTST_HWFFC_DONE_SHIFT)                   /* 0x00000001 */
#define CRU_HWFFC_INTST_HWFFC_STATE_SHIFT                  (4U)
#define CRU_HWFFC_INTST_HWFFC_STATE_MASK                   (0x7U << CRU_HWFFC_INTST_HWFFC_STATE_SHIFT)                  /* 0x00000070 */
#define CRU_HWFFC_INTST_HWFFC_DONE_INTEN_SHIFT             (8U)
#define CRU_HWFFC_INTST_HWFFC_DONE_INTEN_MASK              (0x1U << CRU_HWFFC_INTST_HWFFC_DONE_INTEN_SHIFT)             /* 0x00000100 */
/* APLL_CON0_S */
#define CRU_APLL_CON0_S_OFFSET                             (0xF0U)
#define CRU_APLL_CON0_S                                    (0x3064U)
#define CRU_APLL_CON0_S_FBDIV_SHIFT                        (0U)
#define CRU_APLL_CON0_S_FBDIV_MASK                         (0xFFFU << CRU_APLL_CON0_S_FBDIV_SHIFT)                      /* 0x00000FFF */
#define CRU_APLL_CON0_S_POSTDIV1_SHIFT                     (12U)
#define CRU_APLL_CON0_S_POSTDIV1_MASK                      (0x7U << CRU_APLL_CON0_S_POSTDIV1_SHIFT)                     /* 0x00007000 */
#define CRU_APLL_CON0_S_BYPASS_SHIFT                       (15U)
#define CRU_APLL_CON0_S_BYPASS_MASK                        (0x1U << CRU_APLL_CON0_S_BYPASS_SHIFT)                       /* 0x00008000 */
/* APLL_CON1_S */
#define CRU_APLL_CON1_S_OFFSET                             (0xF4U)
#define CRU_APLL_CON1_S                                    (0x1041U)
#define CRU_APLL_CON1_S_REFDIV_SHIFT                       (0U)
#define CRU_APLL_CON1_S_REFDIV_MASK                        (0x3FU << CRU_APLL_CON1_S_REFDIV_SHIFT)                      /* 0x0000003F */
#define CRU_APLL_CON1_S_POSTDIV2_SHIFT                     (6U)
#define CRU_APLL_CON1_S_POSTDIV2_MASK                      (0x7U << CRU_APLL_CON1_S_POSTDIV2_SHIFT)                     /* 0x000001C0 */
#define CRU_APLL_CON1_S_PLL_LOCK_SHIFT                     (10U)
#define CRU_APLL_CON1_S_PLL_LOCK_MASK                      (0x1U << CRU_APLL_CON1_S_PLL_LOCK_SHIFT)                     /* 0x00000400 */
#define CRU_APLL_CON1_S_DSMPD_SHIFT                        (12U)
#define CRU_APLL_CON1_S_DSMPD_MASK                         (0x1U << CRU_APLL_CON1_S_DSMPD_SHIFT)                        /* 0x00001000 */
#define CRU_APLL_CON1_S_PLLPD0_SHIFT                       (13U)
#define CRU_APLL_CON1_S_PLLPD0_MASK                        (0x1U << CRU_APLL_CON1_S_PLLPD0_SHIFT)                       /* 0x00002000 */
#define CRU_APLL_CON1_S_PLLPD1_SHIFT                       (14U)
#define CRU_APLL_CON1_S_PLLPD1_MASK                        (0x1U << CRU_APLL_CON1_S_PLLPD1_SHIFT)                       /* 0x00004000 */
#define CRU_APLL_CON1_S_PLLPDSEL_SHIFT                     (15U)
#define CRU_APLL_CON1_S_PLLPDSEL_MASK                      (0x1U << CRU_APLL_CON1_S_PLLPDSEL_SHIFT)                     /* 0x00008000 */
/* CLKSEL_CON0_S */
#define CRU_CLKSEL_CON0_S_OFFSET                           (0xF8U)
#define CRU_CLKSEL_CON0_S                                  (0x1300U)
#define CRU_CLKSEL_CON0_S_CLK_CORE_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON0_S_CLK_CORE_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON0_S_CLK_CORE_DIV_CON_SHIFT)           /* 0x0000000F */
#define CRU_CLKSEL_CON0_S_CORE_CLK_PLL_SEL_SHIFT           (7U)
#define CRU_CLKSEL_CON0_S_CORE_CLK_PLL_SEL_MASK            (0x1U << CRU_CLKSEL_CON0_S_CORE_CLK_PLL_SEL_SHIFT)           /* 0x00000080 */
#define CRU_CLKSEL_CON0_S_CORE_DBG_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL_CON0_S_CORE_DBG_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON0_S_CORE_DBG_DIV_CON_SHIFT)           /* 0x00000F00 */
#define CRU_CLKSEL_CON0_S_ACLK_CORE_DIV_CON_SHIFT          (12U)
#define CRU_CLKSEL_CON0_S_ACLK_CORE_DIV_CON_MASK           (0x7U << CRU_CLKSEL_CON0_S_ACLK_CORE_DIV_CON_SHIFT)          /* 0x00007000 */
/* CLKSEL_CON0 */
#define CRU_CLKSEL_CON0_OFFSET                             (0x100U)
#define CRU_CLKSEL_CON0_CLK_CORE_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON0_CLK_CORE_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON0_CLK_CORE_DIV_CON_SHIFT)             /* 0x0000000F */
#define CRU_CLKSEL_CON0_CORE_CLK_PLL_SEL_SHIFT             (7U)
#define CRU_CLKSEL_CON0_CORE_CLK_PLL_SEL_MASK              (0x1U << CRU_CLKSEL_CON0_CORE_CLK_PLL_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL_CON0_CORE_DBG_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON0_CORE_DBG_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON0_CORE_DBG_DIV_CON_SHIFT)             /* 0x00000F00 */
#define CRU_CLKSEL_CON0_ACLK_CORE_DIV_CON_SHIFT            (12U)
#define CRU_CLKSEL_CON0_ACLK_CORE_DIV_CON_MASK             (0x7U << CRU_CLKSEL_CON0_ACLK_CORE_DIV_CON_SHIFT)            /* 0x00007000 */
/* CLKSEL_CON1 */
#define CRU_CLKSEL_CON1_OFFSET                             (0x104U)
#define CRU_CLKSEL_CON1_CLK_GPU_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL_CON1_CLK_GPU_DIV_CON_MASK               (0xFU << CRU_CLKSEL_CON1_CLK_GPU_DIV_CON_SHIFT)              /* 0x0000000F */
#define CRU_CLKSEL_CON1_CLK_GPU_PLL_SEL_SHIFT              (6U)
#define CRU_CLKSEL_CON1_CLK_GPU_PLL_SEL_MASK               (0x3U << CRU_CLKSEL_CON1_CLK_GPU_PLL_SEL_SHIFT)              /* 0x000000C0 */
#define CRU_CLKSEL_CON1_CLK_GPU_DIVNP5_CON_SHIFT           (8U)
#define CRU_CLKSEL_CON1_CLK_GPU_DIVNP5_CON_MASK            (0xFU << CRU_CLKSEL_CON1_CLK_GPU_DIVNP5_CON_SHIFT)           /* 0x00000F00 */
#define CRU_CLKSEL_CON1_ACLK_GPU_DIV_CON_SHIFT             (13U)
#define CRU_CLKSEL_CON1_ACLK_GPU_DIV_CON_MASK              (0x3U << CRU_CLKSEL_CON1_ACLK_GPU_DIV_CON_SHIFT)             /* 0x00006000 */
#define CRU_CLKSEL_CON1_CLK_GPU_SEL_SHIFT                  (15U)
#define CRU_CLKSEL_CON1_CLK_GPU_SEL_MASK                   (0x1U << CRU_CLKSEL_CON1_CLK_GPU_SEL_SHIFT)                  /* 0x00008000 */
/* CLKSEL_CON2 */
#define CRU_CLKSEL_CON2_OFFSET                             (0x108U)
#define CRU_CLKSEL_CON2_DDRPHY4X_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON2_DDRPHY4X_DIV_CON_MASK              (0x7U << CRU_CLKSEL_CON2_DDRPHY4X_DIV_CON_SHIFT)             /* 0x00000007 */
#define CRU_CLKSEL_CON2_CLK_DDRSTDBY_SEL_SHIFT             (4U)
#define CRU_CLKSEL_CON2_CLK_DDRSTDBY_SEL_MASK              (0x1U << CRU_CLKSEL_CON2_CLK_DDRSTDBY_SEL_SHIFT)             /* 0x00000010 */
#define CRU_CLKSEL_CON2_DDRPHY4X_PLL_CLK_SEL_SHIFT         (7U)
#define CRU_CLKSEL_CON2_DDRPHY4X_PLL_CLK_SEL_MASK          (0x1U << CRU_CLKSEL_CON2_DDRPHY4X_PLL_CLK_SEL_SHIFT)         /* 0x00000080 */
#define CRU_CLKSEL_CON2_PCLK_DDR_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON2_PCLK_DDR_DIV_CON_MASK              (0x1FU << CRU_CLKSEL_CON2_PCLK_DDR_DIV_CON_SHIFT)            /* 0x00001F00 */
/* CLKSEL_CON3 */
#define CRU_CLKSEL_CON3_OFFSET                             (0x10CU)
#define CRU_CLKSEL_CON3_ACLK_VO_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL_CON3_ACLK_VO_DIV_CON_MASK               (0x1FU << CRU_CLKSEL_CON3_ACLK_VO_DIV_CON_SHIFT)             /* 0x0000001F */
#define CRU_CLKSEL_CON3_ACLK_VO_PLL_SEL_SHIFT              (6U)
#define CRU_CLKSEL_CON3_ACLK_VO_PLL_SEL_MASK               (0x3U << CRU_CLKSEL_CON3_ACLK_VO_PLL_SEL_SHIFT)              /* 0x000000C0 */
#define CRU_CLKSEL_CON3_HCLK_VO_DIV_CON_SHIFT              (8U)
#define CRU_CLKSEL_CON3_HCLK_VO_DIV_CON_MASK               (0xFU << CRU_CLKSEL_CON3_HCLK_VO_DIV_CON_SHIFT)              /* 0x00000F00 */
#define CRU_CLKSEL_CON3_PCLK_VO_DIV_CON_SHIFT              (12U)
#define CRU_CLKSEL_CON3_PCLK_VO_DIV_CON_MASK               (0xFU << CRU_CLKSEL_CON3_PCLK_VO_DIV_CON_SHIFT)              /* 0x0000F000 */
/* CLKSEL_CON4 */
#define CRU_CLKSEL_CON4_OFFSET                             (0x110U)
#define CRU_CLKSEL_CON4_CLK_RGA_CORE_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON4_CLK_RGA_CORE_DIV_CON_MASK          (0x1FU << CRU_CLKSEL_CON4_CLK_RGA_CORE_DIV_CON_SHIFT)        /* 0x0000001F */
#define CRU_CLKSEL_CON4_CLK_RGA_CORE_PLL_SEL_SHIFT         (6U)
#define CRU_CLKSEL_CON4_CLK_RGA_CORE_PLL_SEL_MASK          (0x3U << CRU_CLKSEL_CON4_CLK_RGA_CORE_PLL_SEL_SHIFT)         /* 0x000000C0 */
/* CLKSEL_CON5 */
#define CRU_CLKSEL_CON5_OFFSET                             (0x114U)
#define CRU_CLKSEL_CON5_DCLK_VOPB_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON5_DCLK_VOPB_DIV_CON_MASK             (0xFFU << CRU_CLKSEL_CON5_DCLK_VOPB_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL_CON5_DCLK_VOPB_PLL_SEL_SHIFT            (11U)
#define CRU_CLKSEL_CON5_DCLK_VOPB_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON5_DCLK_VOPB_PLL_SEL_SHIFT)            /* 0x00000800 */
#define CRU_CLKSEL_CON5_DCLK_VOPB_SEL_SHIFT                (14U)
#define CRU_CLKSEL_CON5_DCLK_VOPB_SEL_MASK                 (0x3U << CRU_CLKSEL_CON5_DCLK_VOPB_SEL_SHIFT)                /* 0x0000C000 */
/* CLKSEL_CON6 */
#define CRU_CLKSEL_CON6_OFFSET                             (0x118U)
#define CRU_CLKSEL_CON6_DCLK_VOPB_FRAC_DIV_CON_SHIFT       (0U)
#define CRU_CLKSEL_CON6_DCLK_VOPB_FRAC_DIV_CON_MASK        (0xFFFFFFFFU << CRU_CLKSEL_CON6_DCLK_VOPB_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON7 */
#define CRU_CLKSEL_CON7_OFFSET                             (0x11CU)
#define CRU_CLKSEL_CON7_CLK_PWM_VOPB_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON7_CLK_PWM_VOPB_DIV_CON_MASK          (0x7FU << CRU_CLKSEL_CON7_CLK_PWM_VOPB_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL_CON7_CLK_PWM_VOPB_PLL_SEL_SHIFT         (7U)
#define CRU_CLKSEL_CON7_CLK_PWM_VOPB_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON7_CLK_PWM_VOPB_PLL_SEL_SHIFT)         /* 0x00000080 */
/* CLKSEL_CON8 */
#define CRU_CLKSEL_CON8_OFFSET                             (0x120U)
#define CRU_CLKSEL_CON8_DCLK_VOPL_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON8_DCLK_VOPL_DIV_CON_MASK             (0xFFU << CRU_CLKSEL_CON8_DCLK_VOPL_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL_CON8_DCLK_VOPL_PLL_SEL_SHIFT            (11U)
#define CRU_CLKSEL_CON8_DCLK_VOPL_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON8_DCLK_VOPL_PLL_SEL_SHIFT)            /* 0x00000800 */
#define CRU_CLKSEL_CON8_DCLK_VOPL_SEL_SHIFT                (14U)
#define CRU_CLKSEL_CON8_DCLK_VOPL_SEL_MASK                 (0x3U << CRU_CLKSEL_CON8_DCLK_VOPL_SEL_SHIFT)                /* 0x0000C000 */
/* CLKSEL_CON9 */
#define CRU_CLKSEL_CON9_OFFSET                             (0x124U)
#define CRU_CLKSEL_CON9_DCLK_VOPL_FRAC_DIV_CON_SHIFT       (0U)
#define CRU_CLKSEL_CON9_DCLK_VOPL_FRAC_DIV_CON_MASK        (0xFFFFFFFFU << CRU_CLKSEL_CON9_DCLK_VOPL_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON10 */
#define CRU_CLKSEL_CON10_OFFSET                            (0x128U)
#define CRU_CLKSEL_CON10_ACLK_VPU_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON10_ACLK_VPU_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON10_ACLK_VPU_DIV_CON_SHIFT)           /* 0x0000001F */
#define CRU_CLKSEL_CON10_ACLK_VPU_PLL_SEL_SHIFT            (6U)
#define CRU_CLKSEL_CON10_ACLK_VPU_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON10_ACLK_VPU_PLL_SEL_SHIFT)            /* 0x000000C0 */
#define CRU_CLKSEL_CON10_HCLK_VPU_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON10_HCLK_VPU_DIV_CON_MASK             (0xFU << CRU_CLKSEL_CON10_HCLK_VPU_DIV_CON_SHIFT)            /* 0x00000F00 */
/* CLKSEL_CON11 */
#define CRU_CLKSEL_CON11_OFFSET                            (0x12CU)
#define CRU_CLKSEL_CON11_ACLK_VI_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON11_ACLK_VI_DIV_CON_MASK              (0x1FU << CRU_CLKSEL_CON11_ACLK_VI_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL_CON11_ACLK_VI_PLL_SEL_SHIFT             (6U)
#define CRU_CLKSEL_CON11_ACLK_VI_PLL_SEL_MASK              (0x3U << CRU_CLKSEL_CON11_ACLK_VI_PLL_SEL_SHIFT)             /* 0x000000C0 */
#define CRU_CLKSEL_CON11_HCLK_VI_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON11_HCLK_VI_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON11_HCLK_VI_DIV_CON_SHIFT)             /* 0x00000F00 */
/* CLKSEL_CON12 */
#define CRU_CLKSEL_CON12_OFFSET                            (0x130U)
#define CRU_CLKSEL_CON12_CLK_ISP_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON12_CLK_ISP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL_CON12_CLK_ISP_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL_CON12_CLK_ISP_PLL_SEL_SHIFT             (6U)
#define CRU_CLKSEL_CON12_CLK_ISP_PLL_SEL_MASK              (0x3U << CRU_CLKSEL_CON12_CLK_ISP_PLL_SEL_SHIFT)             /* 0x000000C0 */
#define CRU_CLKSEL_CON12_CLK_GMAC_OUT_DIV_CON_SHIFT        (8U)
#define CRU_CLKSEL_CON12_CLK_GMAC_OUT_DIV_CON_MASK         (0x1FU << CRU_CLKSEL_CON12_CLK_GMAC_OUT_DIV_CON_SHIFT)       /* 0x00001F00 */
#define CRU_CLKSEL_CON12_CLK_GMAC_OUT_PLL_SEL_SHIFT        (14U)
#define CRU_CLKSEL_CON12_CLK_GMAC_OUT_PLL_SEL_MASK         (0x3U << CRU_CLKSEL_CON12_CLK_GMAC_OUT_PLL_SEL_SHIFT)        /* 0x0000C000 */
/* CLKSEL_CON13 */
#define CRU_CLKSEL_CON13_OFFSET                            (0x134U)
#define CRU_CLKSEL_CON13_CLK_CIF_OUT_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON13_CLK_CIF_OUT_DIV_CON_MASK          (0x3FU << CRU_CLKSEL_CON13_CLK_CIF_OUT_DIV_CON_SHIFT)        /* 0x0000003F */
#define CRU_CLKSEL_CON13_CLK_CIF_OUT_PLL_SEL_SHIFT         (6U)
#define CRU_CLKSEL_CON13_CLK_CIF_OUT_PLL_SEL_MASK          (0x3U << CRU_CLKSEL_CON13_CLK_CIF_OUT_PLL_SEL_SHIFT)         /* 0x000000C0 */
#define CRU_CLKSEL_CON13_CLK_VPU_CORE_DIV_CON_SHIFT        (8U)
#define CRU_CLKSEL_CON13_CLK_VPU_CORE_DIV_CON_MASK         (0x1FU << CRU_CLKSEL_CON13_CLK_VPU_CORE_DIV_CON_SHIFT)       /* 0x00001F00 */
#define CRU_CLKSEL_CON13_CLK_VPU_CORE_PLL_CLK_SEL_SHIFT    (14U)
#define CRU_CLKSEL_CON13_CLK_VPU_CORE_PLL_CLK_SEL_MASK     (0x3U << CRU_CLKSEL_CON13_CLK_VPU_CORE_PLL_CLK_SEL_SHIFT)    /* 0x0000C000 */
/* CLKSEL_CON14 */
#define CRU_CLKSEL_CON14_OFFSET                            (0x138U)
#define CRU_CLKSEL_CON14_ACLK_PERI_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON14_ACLK_PERI_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON14_ACLK_PERI_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON14_HCLK_PERI_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL_CON14_HCLK_PERI_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON14_HCLK_PERI_DIV_CON_SHIFT)          /* 0x00001F00 */
#define CRU_CLKSEL_CON14_ACLK_HCLK_PERI_PLL_SEL_SHIFT      (15U)
#define CRU_CLKSEL_CON14_ACLK_HCLK_PERI_PLL_SEL_MASK       (0x1U << CRU_CLKSEL_CON14_ACLK_HCLK_PERI_PLL_SEL_SHIFT)      /* 0x00008000 */
/* CLKSEL_CON15 */
#define CRU_CLKSEL_CON15_OFFSET                            (0x13CU)
#define CRU_CLKSEL_CON15_CLK_NANDC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON15_CLK_NANDC_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON15_CLK_NANDC_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON15_CLK_NANDC_PLL_SHIFT               (6U)
#define CRU_CLKSEL_CON15_CLK_NANDC_PLL_MASK                (0x3U << CRU_CLKSEL_CON15_CLK_NANDC_PLL_SHIFT)               /* 0x000000C0 */
#define CRU_CLKSEL_CON15_CLK_NANDC_DIV50_DIV_CON_SHIFT     (8U)
#define CRU_CLKSEL_CON15_CLK_NANDC_DIV50_DIV_CON_MASK      (0x1FU << CRU_CLKSEL_CON15_CLK_NANDC_DIV50_DIV_CON_SHIFT)    /* 0x00001F00 */
#define CRU_CLKSEL_CON15_CLK_NANDC_SEL_SHIFT               (15U)
#define CRU_CLKSEL_CON15_CLK_NANDC_SEL_MASK                (0x1U << CRU_CLKSEL_CON15_CLK_NANDC_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL_CON16 */
#define CRU_CLKSEL_CON16_OFFSET                            (0x140U)
#define CRU_CLKSEL_CON16_CLK_SDMMC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON16_CLK_SDMMC_DIV_CON_MASK            (0xFFU << CRU_CLKSEL_CON16_CLK_SDMMC_DIV_CON_SHIFT)          /* 0x000000FF */
#define CRU_CLKSEL_CON16_CLK_SDMMC_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON16_CLK_SDMMC_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON16_CLK_SDMMC_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON17 */
#define CRU_CLKSEL_CON17_OFFSET                            (0x144U)
#define CRU_CLKSEL_CON17_CLK_SDMMC_DIV50_DIV_CON_SHIFT     (0U)
#define CRU_CLKSEL_CON17_CLK_SDMMC_DIV50_DIV_CON_MASK      (0xFFU << CRU_CLKSEL_CON17_CLK_SDMMC_DIV50_DIV_CON_SHIFT)    /* 0x000000FF */
#define CRU_CLKSEL_CON17_CLK_SDMMC_SEL_SHIFT               (15U)
#define CRU_CLKSEL_CON17_CLK_SDMMC_SEL_MASK                (0x1U << CRU_CLKSEL_CON17_CLK_SDMMC_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL_CON18 */
#define CRU_CLKSEL_CON18_OFFSET                            (0x148U)
#define CRU_CLKSEL_CON18_CLK_SDIO_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON18_CLK_SDIO_DIV_CON_MASK             (0xFFU << CRU_CLKSEL_CON18_CLK_SDIO_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL_CON18_CLK_SDIO_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON18_CLK_SDIO_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON18_CLK_SDIO_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON19 */
#define CRU_CLKSEL_CON19_OFFSET                            (0x14CU)
#define CRU_CLKSEL_CON19_CLK_SDIO_DIV50_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON19_CLK_SDIO_DIV50_DIV_CON_MASK       (0xFFU << CRU_CLKSEL_CON19_CLK_SDIO_DIV50_DIV_CON_SHIFT)     /* 0x000000FF */
#define CRU_CLKSEL_CON19_CLK_SDIO_SEL_SHIFT                (15U)
#define CRU_CLKSEL_CON19_CLK_SDIO_SEL_MASK                 (0x1U << CRU_CLKSEL_CON19_CLK_SDIO_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL_CON20 */
#define CRU_CLKSEL_CON20_OFFSET                            (0x150U)
#define CRU_CLKSEL_CON20_CLK_EMMC_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON20_CLK_EMMC_DIV_CON_MASK             (0xFFU << CRU_CLKSEL_CON20_CLK_EMMC_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL_CON20_CLK_EMMC_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON20_CLK_EMMC_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON20_CLK_EMMC_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON21 */
#define CRU_CLKSEL_CON21_OFFSET                            (0x154U)
#define CRU_CLKSEL_CON21_CLK_EMMC_DIV50_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON21_CLK_EMMC_DIV50_DIV_CON_MASK       (0xFFU << CRU_CLKSEL_CON21_CLK_EMMC_DIV50_DIV_CON_SHIFT)     /* 0x000000FF */
#define CRU_CLKSEL_CON21_CLK_EMMC_SEL_SHIFT                (15U)
#define CRU_CLKSEL_CON21_CLK_EMMC_SEL_MASK                 (0x1U << CRU_CLKSEL_CON21_CLK_EMMC_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL_CON22 */
#define CRU_CLKSEL_CON22_OFFSET                            (0x158U)
#define CRU_CLKSEL_CON22_CLK_SFC_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON22_CLK_SFC_DIV_CON_MASK              (0x7FU << CRU_CLKSEL_CON22_CLK_SFC_DIV_CON_SHIFT)            /* 0x0000007F */
#define CRU_CLKSEL_CON22_CLK_SFC_PLL_SEL_SHIFT             (7U)
#define CRU_CLKSEL_CON22_CLK_SFC_PLL_SEL_MASK              (0x1U << CRU_CLKSEL_CON22_CLK_SFC_PLL_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL_CON22_CLK_GMAC_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON22_CLK_GMAC_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON22_CLK_GMAC_DIV_CON_SHIFT)           /* 0x00001F00 */
#define CRU_CLKSEL_CON22_CLK_GMAC_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON22_CLK_GMAC_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON22_CLK_GMAC_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON23 */
#define CRU_CLKSEL_CON23_OFFSET                            (0x15CU)
#define CRU_CLKSEL_CON23_PCLK_GMAC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON23_PCLK_GMAC_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON23_PCLK_GMAC_DIV_CON_SHIFT)           /* 0x0000000F */
#define CRU_CLKSEL_CON23_RMII_EXTCLKSRC_SEL_SHIFT          (6U)
#define CRU_CLKSEL_CON23_RMII_EXTCLKSRC_SEL_MASK           (0x1U << CRU_CLKSEL_CON23_RMII_EXTCLKSRC_SEL_SHIFT)          /* 0x00000040 */
#define CRU_CLKSEL_CON23_RMII_CLK_SEL_SHIFT                (7U)
#define CRU_CLKSEL_CON23_RMII_CLK_SEL_MASK                 (0x1U << CRU_CLKSEL_CON23_RMII_CLK_SEL_SHIFT)                /* 0x00000080 */
#define CRU_CLKSEL_CON23_ACLK_BUS_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON23_ACLK_BUS_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON23_ACLK_BUS_DIV_CON_SHIFT)           /* 0x00001F00 */
#define CRU_CLKSEL_CON23_ACLK_HCLK_PCLK_BUS_PLL_SEL_SHIFT  (15U)
#define CRU_CLKSEL_CON23_ACLK_HCLK_PCLK_BUS_PLL_SEL_MASK   (0x1U << CRU_CLKSEL_CON23_ACLK_HCLK_PCLK_BUS_PLL_SEL_SHIFT)  /* 0x00008000 */
/* CLKSEL_CON24 */
#define CRU_CLKSEL_CON24_OFFSET                            (0x160U)
#define CRU_CLKSEL_CON24_HCLK_BUS_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON24_HCLK_BUS_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON24_HCLK_BUS_DIV_CON_SHIFT)           /* 0x0000001F */
#define CRU_CLKSEL_CON24_PCLK_BUS_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON24_PCLK_BUS_DIV_CON_MASK             (0x3U << CRU_CLKSEL_CON24_PCLK_BUS_DIV_CON_SHIFT)            /* 0x00000300 */
/* CLKSEL_CON25 */
#define CRU_CLKSEL_CON25_OFFSET                            (0x164U)
#define CRU_CLKSEL_CON25_CLK_CRYPTO_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON25_CLK_CRYPTO_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON25_CLK_CRYPTO_DIV_CON_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL_CON25_CLK_CRYPTO_PLL_SEL_SHIFT          (6U)
#define CRU_CLKSEL_CON25_CLK_CRYPTO_PLL_SEL_MASK           (0x3U << CRU_CLKSEL_CON25_CLK_CRYPTO_PLL_SEL_SHIFT)          /* 0x000000C0 */
#define CRU_CLKSEL_CON25_CLK_CRYPTO_APK_DIV_CON_SHIFT      (8U)
#define CRU_CLKSEL_CON25_CLK_CRYPTO_APK_DIV_CON_MASK       (0x1FU << CRU_CLKSEL_CON25_CLK_CRYPTO_APK_DIV_CON_SHIFT)     /* 0x00001F00 */
#define CRU_CLKSEL_CON25_CLK_CRYPTO_APK_SEL_SHIFT          (14U)
#define CRU_CLKSEL_CON25_CLK_CRYPTO_APK_SEL_MASK           (0x3U << CRU_CLKSEL_CON25_CLK_CRYPTO_APK_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON26 */
#define CRU_CLKSEL_CON26_OFFSET                            (0x168U)
#define CRU_CLKSEL_CON26_CLK_PDM_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON26_CLK_PDM_DIV_CON_MASK              (0x7FU << CRU_CLKSEL_CON26_CLK_PDM_DIV_CON_SHIFT)            /* 0x0000007F */
#define CRU_CLKSEL_CON26_CLK_PDM_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL_CON26_CLK_PDM_PLL_SEL_MASK              (0x3U << CRU_CLKSEL_CON26_CLK_PDM_PLL_SEL_SHIFT)             /* 0x00000300 */
#define CRU_CLKSEL_CON26_CLK_PDM_SEL_SHIFT                 (15U)
#define CRU_CLKSEL_CON26_CLK_PDM_SEL_MASK                  (0x1U << CRU_CLKSEL_CON26_CLK_PDM_SEL_SHIFT)                 /* 0x00008000 */
/* CLKSEL_CON27 */
#define CRU_CLKSEL_CON27_OFFSET                            (0x16CU)
#define CRU_CLKSEL_CON27_CLK_PDM_FRAC_DIV_CON_SHIFT        (0U)
#define CRU_CLKSEL_CON27_CLK_PDM_FRAC_DIV_CON_MASK         (0xFFFFFFFFU << CRU_CLKSEL_CON27_CLK_PDM_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON28 */
#define CRU_CLKSEL_CON28_OFFSET                            (0x170U)
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_DIV_CON_MASK          (0x7FU << CRU_CLKSEL_CON28_CLK_I2S0_TX_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_PLL_SEL_SHIFT         (8U)
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON28_CLK_I2S0_TX_PLL_SEL_SHIFT)         /* 0x00000100 */
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_SEL_SHIFT             (10U)
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_SEL_MASK              (0x3U << CRU_CLKSEL_CON28_CLK_I2S0_TX_SEL_SHIFT)             /* 0x00000C00 */
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_RX_CLK_SEL_SHIFT      (12U)
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_RX_CLK_SEL_MASK       (0x1U << CRU_CLKSEL_CON28_CLK_I2S0_TX_RX_CLK_SEL_SHIFT)      /* 0x00001000 */
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_OUT_MCLK_SEL_SHIFT    (14U)
#define CRU_CLKSEL_CON28_CLK_I2S0_TX_OUT_MCLK_SEL_MASK     (0x3U << CRU_CLKSEL_CON28_CLK_I2S0_TX_OUT_MCLK_SEL_SHIFT)    /* 0x0000C000 */
/* CLKSEL_CON29 */
#define CRU_CLKSEL_CON29_OFFSET                            (0x174U)
#define CRU_CLKSEL_CON29_CLK_I2S0_TX_FRAC_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON29_CLK_I2S0_TX_FRAC_DIV_CON_MASK     (0xFFFFFFFFU << CRU_CLKSEL_CON29_CLK_I2S0_TX_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON30 */
#define CRU_CLKSEL_CON30_OFFSET                            (0x178U)
#define CRU_CLKSEL_CON30_CLK_I2S1_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON30_CLK_I2S1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON30_CLK_I2S1_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON30_CLK_I2S1_PLL_SEL_SHIFT            (8U)
#define CRU_CLKSEL_CON30_CLK_I2S1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON30_CLK_I2S1_PLL_SEL_SHIFT)            /* 0x00000100 */
#define CRU_CLKSEL_CON30_CLK_I2S1_SEL_SHIFT                (10U)
#define CRU_CLKSEL_CON30_CLK_I2S1_SEL_MASK                 (0x3U << CRU_CLKSEL_CON30_CLK_I2S1_SEL_SHIFT)                /* 0x00000C00 */
#define CRU_CLKSEL_CON30_CLK_I2S1_OUT_MCLK_SEL_SHIFT       (15U)
#define CRU_CLKSEL_CON30_CLK_I2S1_OUT_MCLK_SEL_MASK        (0x1U << CRU_CLKSEL_CON30_CLK_I2S1_OUT_MCLK_SEL_SHIFT)       /* 0x00008000 */
/* CLKSEL_CON31 */
#define CRU_CLKSEL_CON31_OFFSET                            (0x17CU)
#define CRU_CLKSEL_CON31_CLK_I2S1_FRAC_DIV_CON_SHIFT       (0U)
#define CRU_CLKSEL_CON31_CLK_I2S1_FRAC_DIV_CON_MASK        (0xFFFFFFFFU << CRU_CLKSEL_CON31_CLK_I2S1_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON32 */
#define CRU_CLKSEL_CON32_OFFSET                            (0x180U)
#define CRU_CLKSEL_CON32_CLK_I2S2_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON32_CLK_I2S2_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON32_CLK_I2S2_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON32_CLK_I2S2_PLL_SEL_SHIFT            (8U)
#define CRU_CLKSEL_CON32_CLK_I2S2_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON32_CLK_I2S2_PLL_SEL_SHIFT)            /* 0x00000100 */
#define CRU_CLKSEL_CON32_CLK_I2S2_SEL_SHIFT                (10U)
#define CRU_CLKSEL_CON32_CLK_I2S2_SEL_MASK                 (0x3U << CRU_CLKSEL_CON32_CLK_I2S2_SEL_SHIFT)                /* 0x00000C00 */
#define CRU_CLKSEL_CON32_CLK_I2S2_OUT_MCLK_SEL_SHIFT       (15U)
#define CRU_CLKSEL_CON32_CLK_I2S2_OUT_MCLK_SEL_MASK        (0x1U << CRU_CLKSEL_CON32_CLK_I2S2_OUT_MCLK_SEL_SHIFT)       /* 0x00008000 */
/* CLKSEL_CON33 */
#define CRU_CLKSEL_CON33_OFFSET                            (0x184U)
#define CRU_CLKSEL_CON33_CLK_I2S2_FRAC_DIV_CON_SHIFT       (0U)
#define CRU_CLKSEL_CON33_CLK_I2S2_FRAC_DIV_CON_MASK        (0xFFFFFFFFU << CRU_CLKSEL_CON33_CLK_I2S2_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON34 */
#define CRU_CLKSEL_CON34_OFFSET                            (0x188U)
#define CRU_CLKSEL_CON34_CLK_UART1_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON34_CLK_UART1_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON34_CLK_UART1_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON34_CLK_UART1_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON34_CLK_UART1_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON34_CLK_UART1_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON35 */
#define CRU_CLKSEL_CON35_OFFSET                            (0x18CU)
#define CRU_CLKSEL_CON35_CLK_UART1_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON35_CLK_UART1_DIVNP5_DIV_CON_MASK     (0x1FU << CRU_CLKSEL_CON35_CLK_UART1_DIVNP5_DIV_CON_SHIFT)   /* 0x0000001F */
#define CRU_CLKSEL_CON35_CLK_UART1_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON35_CLK_UART1_SEL_MASK                (0x3U << CRU_CLKSEL_CON35_CLK_UART1_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON36 */
#define CRU_CLKSEL_CON36_OFFSET                            (0x190U)
#define CRU_CLKSEL_CON36_CLK_UART1_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON36_CLK_UART1_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON36_CLK_UART1_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON37 */
#define CRU_CLKSEL_CON37_OFFSET                            (0x194U)
#define CRU_CLKSEL_CON37_CLK_UART2_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON37_CLK_UART2_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON37_CLK_UART2_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON37_CLK_UART2_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON37_CLK_UART2_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON37_CLK_UART2_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON38 */
#define CRU_CLKSEL_CON38_OFFSET                            (0x198U)
#define CRU_CLKSEL_CON38_CLK_UART2_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON38_CLK_UART2_DIVNP5_DIV_CON_MASK     (0x1FU << CRU_CLKSEL_CON38_CLK_UART2_DIVNP5_DIV_CON_SHIFT)   /* 0x0000001F */
#define CRU_CLKSEL_CON38_CLK_UART2_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON38_CLK_UART2_SEL_MASK                (0x3U << CRU_CLKSEL_CON38_CLK_UART2_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON39 */
#define CRU_CLKSEL_CON39_OFFSET                            (0x19CU)
#define CRU_CLKSEL_CON39_CLK_UART2_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON39_CLK_UART2_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON39_CLK_UART2_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON40 */
#define CRU_CLKSEL_CON40_OFFSET                            (0x1A0U)
#define CRU_CLKSEL_CON40_CLK_UART3_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON40_CLK_UART3_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON40_CLK_UART3_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON40_CLK_UART3_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON40_CLK_UART3_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON40_CLK_UART3_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON41 */
#define CRU_CLKSEL_CON41_OFFSET                            (0x1A4U)
#define CRU_CLKSEL_CON41_CLK_UART3_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON41_CLK_UART3_DIVNP5_DIV_CON_MASK     (0x1FU << CRU_CLKSEL_CON41_CLK_UART3_DIVNP5_DIV_CON_SHIFT)   /* 0x0000001F */
#define CRU_CLKSEL_CON41_CLK_UART3_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON41_CLK_UART3_SEL_MASK                (0x3U << CRU_CLKSEL_CON41_CLK_UART3_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON42 */
#define CRU_CLKSEL_CON42_OFFSET                            (0x1A8U)
#define CRU_CLKSEL_CON42_CLK_UART3_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON42_CLK_UART3_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON42_CLK_UART3_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON43 */
#define CRU_CLKSEL_CON43_OFFSET                            (0x1ACU)
#define CRU_CLKSEL_CON43_CLK_UART4_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON43_CLK_UART4_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON43_CLK_UART4_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON43_CLK_UART4_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON43_CLK_UART4_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON43_CLK_UART4_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON44 */
#define CRU_CLKSEL_CON44_OFFSET                            (0x1B0U)
#define CRU_CLKSEL_CON44_CLK_UART4_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON44_CLK_UART4_DIVNP5_DIV_CON_MASK     (0x1FU << CRU_CLKSEL_CON44_CLK_UART4_DIVNP5_DIV_CON_SHIFT)   /* 0x0000001F */
#define CRU_CLKSEL_CON44_CLK_UART4_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON44_CLK_UART4_SEL_MASK                (0x3U << CRU_CLKSEL_CON44_CLK_UART4_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON45 */
#define CRU_CLKSEL_CON45_OFFSET                            (0x1B4U)
#define CRU_CLKSEL_CON45_CLK_UART4_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON45_CLK_UART4_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON45_CLK_UART4_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON46 */
#define CRU_CLKSEL_CON46_OFFSET                            (0x1B8U)
#define CRU_CLKSEL_CON46_CLK_UART5_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON46_CLK_UART5_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON46_CLK_UART5_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON46_CLK_UART5_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON46_CLK_UART5_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON46_CLK_UART5_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON47 */
#define CRU_CLKSEL_CON47_OFFSET                            (0x1BCU)
#define CRU_CLKSEL_CON47_CLK_UART5_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON47_CLK_UART5_DIVNP5_DIV_CON_MASK     (0x1FU << CRU_CLKSEL_CON47_CLK_UART5_DIVNP5_DIV_CON_SHIFT)   /* 0x0000001F */
#define CRU_CLKSEL_CON47_CLK_UART5_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON47_CLK_UART5_SEL_MASK                (0x3U << CRU_CLKSEL_CON47_CLK_UART5_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON48 */
#define CRU_CLKSEL_CON48_OFFSET                            (0x1C0U)
#define CRU_CLKSEL_CON48_CLK_UART5_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON48_CLK_UART5_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON48_CLK_UART5_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON49 */
#define CRU_CLKSEL_CON49_OFFSET                            (0x1C4U)
#define CRU_CLKSEL_CON49_CLK_I2C0_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON49_CLK_I2C0_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON49_CLK_I2C0_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON49_CLK_I2C0_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON49_CLK_I2C0_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON49_CLK_I2C0_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON49_CLK_I2C1_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON49_CLK_I2C1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON49_CLK_I2C1_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON49_CLK_I2C1_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON49_CLK_I2C1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON49_CLK_I2C1_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON50 */
#define CRU_CLKSEL_CON50_OFFSET                            (0x1C8U)
#define CRU_CLKSEL_CON50_CLK_I2C2_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON50_CLK_I2C2_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON50_CLK_I2C2_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON50_CLK_I2C2_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON50_CLK_I2C2_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON50_CLK_I2C2_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON50_CLK_I2C3_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON50_CLK_I2C3_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON50_CLK_I2C3_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON50_CLK_I2C3_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON50_CLK_I2C3_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON50_CLK_I2C3_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON52 */
#define CRU_CLKSEL_CON52_OFFSET                            (0x1D0U)
#define CRU_CLKSEL_CON52_CLK_PWM0_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON52_CLK_PWM0_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON52_CLK_PWM0_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON52_CLK_PWM0_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON52_CLK_PWM0_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON52_CLK_PWM0_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON52_CLK_PWM1_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON52_CLK_PWM1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON52_CLK_PWM1_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON52_CLK_PWM1_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON52_CLK_PWM1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON52_CLK_PWM1_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON53 */
#define CRU_CLKSEL_CON53_OFFSET                            (0x1D4U)
#define CRU_CLKSEL_CON53_CLK_SPI0_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON53_CLK_SPI0_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON53_CLK_SPI0_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON53_CLK_SPI0_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON53_CLK_SPI0_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON53_CLK_SPI0_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON53_CLK_SPI1_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON53_CLK_SPI1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON53_CLK_SPI1_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON53_CLK_SPI1_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON53_CLK_SPI1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON53_CLK_SPI1_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON54 */
#define CRU_CLKSEL_CON54_OFFSET                            (0x1D8U)
#define CRU_CLKSEL_CON54_CLK_TSADC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON54_CLK_TSADC_DIV_CON_MASK            (0x7FFU << CRU_CLKSEL_CON54_CLK_TSADC_DIV_CON_SHIFT)         /* 0x000007FF */
/* CLKSEL_CON55 */
#define CRU_CLKSEL_CON55_OFFSET                            (0x1DCU)
#define CRU_CLKSEL_CON55_CLK_SARADC_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON55_CLK_SARADC_DIV_CON_MASK           (0x7FFU << CRU_CLKSEL_CON55_CLK_SARADC_DIV_CON_SHIFT)        /* 0x000007FF */
/* CLKSEL_CON56 */
#define CRU_CLKSEL_CON56_OFFSET                            (0x1E0U)
#define CRU_CLKSEL_CON56_CLK_OTP_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON56_CLK_OTP_DIV_CON_MASK              (0x7U << CRU_CLKSEL_CON56_CLK_OTP_DIV_CON_SHIFT)             /* 0x00000007 */
#define CRU_CLKSEL_CON56_CLK_OTP_USR_DIV_CON_SHIFT         (4U)
#define CRU_CLKSEL_CON56_CLK_OTP_USR_DIV_CON_MASK          (0x3U << CRU_CLKSEL_CON56_CLK_OTP_USR_DIV_CON_SHIFT)         /* 0x00000030 */
/* CLKSEL_CON57 */
#define CRU_CLKSEL_CON57_OFFSET                            (0x1E4U)
#define CRU_CLKSEL_CON57_TESTCLK_SEL_SHIFT                 (0U)
#define CRU_CLKSEL_CON57_TESTCLK_SEL_MASK                  (0x1FU << CRU_CLKSEL_CON57_TESTCLK_SEL_SHIFT)                /* 0x0000001F */
#define CRU_CLKSEL_CON57_TEST_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL_CON57_TEST_DIV_CON_MASK                 (0x1FU << CRU_CLKSEL_CON57_TEST_DIV_CON_SHIFT)               /* 0x00001F00 */
/* CLKSEL_CON58 */
#define CRU_CLKSEL_CON58_OFFSET                            (0x1E8U)
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_DIV_CON_MASK          (0x7FU << CRU_CLKSEL_CON58_CLK_I2S0_RX_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_PLL_SEL_SHIFT         (8U)
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON58_CLK_I2S0_RX_PLL_SEL_SHIFT)         /* 0x00000100 */
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_SEL_SHIFT             (10U)
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_SEL_MASK              (0x3U << CRU_CLKSEL_CON58_CLK_I2S0_RX_SEL_SHIFT)             /* 0x00000C00 */
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_TX_CLK_SEL_SHIFT      (12U)
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_TX_CLK_SEL_MASK       (0x1U << CRU_CLKSEL_CON58_CLK_I2S0_RX_TX_CLK_SEL_SHIFT)      /* 0x00001000 */
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_OUT_MCLK_SEL_SHIFT    (14U)
#define CRU_CLKSEL_CON58_CLK_I2S0_RX_OUT_MCLK_SEL_MASK     (0x3U << CRU_CLKSEL_CON58_CLK_I2S0_RX_OUT_MCLK_SEL_SHIFT)    /* 0x0000C000 */
/* CLKSEL_CON59 */
#define CRU_CLKSEL_CON59_OFFSET                            (0x1ECU)
#define CRU_CLKSEL_CON59_CLK_I2S0_RX_FRAC_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON59_CLK_I2S0_RX_FRAC_DIV_CON_MASK     (0xFFFFFFFFU << CRU_CLKSEL_CON59_CLK_I2S0_RX_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKGATE_CON0 */
#define CRU_CLKGATE_CON0_OFFSET                            (0x200U)
#define CRU_CLKGATE_CON0_CORE_PLL_CLK_EN_SHIFT             (0U)
#define CRU_CLKGATE_CON0_CORE_PLL_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON0_CORE_PLL_CLK_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE_CON0_ACLK_CORE_SRC_CLK_EN_SHIFT        (1U)
#define CRU_CLKGATE_CON0_ACLK_CORE_SRC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON0_ACLK_CORE_SRC_CLK_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE_CON0_PCLK_CORE_DBG_SRC_CLK_EN_SHIFT    (2U)
#define CRU_CLKGATE_CON0_PCLK_CORE_DBG_SRC_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON0_PCLK_CORE_DBG_SRC_CLK_EN_SHIFT)    /* 0x00000004 */
#define CRU_CLKGATE_CON0_CLK_JTAG_CORE_CLK_EN_SHIFT        (3U)
#define CRU_CLKGATE_CON0_CLK_JTAG_CORE_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON0_CLK_JTAG_CORE_CLK_EN_SHIFT)        /* 0x00000008 */
#define CRU_CLKGATE_CON0_ACLK_CORE_NIU_CLK_EN_SHIFT        (4U)
#define CRU_CLKGATE_CON0_ACLK_CORE_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON0_ACLK_CORE_NIU_CLK_EN_SHIFT)        /* 0x00000010 */
#define CRU_CLKGATE_CON0_PCLK_CORE_DBG_NIU_CLK_EN_SHIFT    (5U)
#define CRU_CLKGATE_CON0_PCLK_CORE_DBG_NIU_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON0_PCLK_CORE_DBG_NIU_CLK_EN_SHIFT)    /* 0x00000020 */
#define CRU_CLKGATE_CON0_PCLK_CORE_DBG_DAPLITE_CLK_EN_SHIFT (6U)
#define CRU_CLKGATE_CON0_PCLK_CORE_DBG_DAPLITE_CLK_EN_MASK (0x1U << CRU_CLKGATE_CON0_PCLK_CORE_DBG_DAPLITE_CLK_EN_SHIFT) /* 0x00000040 */
#define CRU_CLKGATE_CON0_DDRPHY_DPLL_CLK_EN_SHIFT          (7U)
#define CRU_CLKGATE_CON0_DDRPHY_DPLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON0_DDRPHY_DPLL_CLK_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE_CON0_GPU_PLL_CLK_EN_SHIFT              (8U)
#define CRU_CLKGATE_CON0_GPU_PLL_CLK_EN_MASK               (0x1U << CRU_CLKGATE_CON0_GPU_PLL_CLK_EN_SHIFT)              /* 0x00000100 */
#define CRU_CLKGATE_CON0_GPU_CLK_NP5_SRC_CLK_EN_SHIFT      (9U)
#define CRU_CLKGATE_CON0_GPU_CLK_NP5_SRC_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON0_GPU_CLK_NP5_SRC_CLK_EN_SHIFT)      /* 0x00000200 */
#define CRU_CLKGATE_CON0_CLK_GPU_CLK_EN_SHIFT              (10U)
#define CRU_CLKGATE_CON0_CLK_GPU_CLK_EN_MASK               (0x1U << CRU_CLKGATE_CON0_CLK_GPU_CLK_EN_SHIFT)              /* 0x00000400 */
#define CRU_CLKGATE_CON0_ACLK_GPU_NIU_CLK_EN_SHIFT         (11U)
#define CRU_CLKGATE_CON0_ACLK_GPU_NIU_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON0_ACLK_GPU_NIU_CLK_EN_SHIFT)         /* 0x00000800 */
#define CRU_CLKGATE_CON0_GPU_CLK_DIV_CLK_EN_SHIFT          (12U)
#define CRU_CLKGATE_CON0_GPU_CLK_DIV_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON0_GPU_CLK_DIV_CLK_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE_CON0_DDRPHY_GPLL_CLK_EN_SHIFT          (13U)
#define CRU_CLKGATE_CON0_DDRPHY_GPLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON0_DDRPHY_GPLL_CLK_EN_SHIFT)          /* 0x00002000 */
#define CRU_CLKGATE_CON0_CLK_DDRPHY4X_CLK_EN_SHIFT         (14U)
#define CRU_CLKGATE_CON0_CLK_DDRPHY4X_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON0_CLK_DDRPHY4X_CLK_EN_SHIFT)         /* 0x00004000 */
#define CRU_CLKGATE_CON0_CLK_DDRMON24M_CLK_EN_SHIFT        (15U)
#define CRU_CLKGATE_CON0_CLK_DDRMON24M_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON0_CLK_DDRMON24M_CLK_EN_SHIFT)        /* 0x00008000 */
/* CLKGATE_CON1 */
#define CRU_CLKGATE_CON1_OFFSET                            (0x204U)
#define CRU_CLKGATE_CON1_CLK_STDBY_SRC_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON1_CLK_STDBY_SRC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON1_CLK_STDBY_SRC_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON1_DDR_PCLK_PLL_CLK_EN_SHIFT         (1U)
#define CRU_CLKGATE_CON1_DDR_PCLK_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON1_DDR_PCLK_PLL_CLK_EN_SHIFT)         /* 0x00000002 */
#define CRU_CLKGATE_CON1_ACLK_AXI_CMD_BUFFER_CLK_EN_SHIFT  (2U)
#define CRU_CLKGATE_CON1_ACLK_AXI_CMD_BUFFER_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON1_ACLK_AXI_CMD_BUFFER_CLK_EN_SHIFT)  /* 0x00000004 */
#define CRU_CLKGATE_CON1_PCLK_AXI_CMD_BUFFER_CLK_EN_SHIFT  (3U)
#define CRU_CLKGATE_CON1_PCLK_AXI_CMD_BUFFER_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON1_PCLK_AXI_CMD_BUFFER_CLK_EN_SHIFT)  /* 0x00000008 */
#define CRU_CLKGATE_CON1_ACLK_UPCTL2_CLK_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON1_ACLK_UPCTL2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON1_ACLK_UPCTL2_CLK_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON1_CLK_DDRC_UPCTL2_CLK_EN_SHIFT      (6U)
#define CRU_CLKGATE_CON1_CLK_DDRC_UPCTL2_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON1_CLK_DDRC_UPCTL2_CLK_EN_SHIFT)      /* 0x00000040 */
#define CRU_CLKGATE_CON1_PCLK_UPCTL2_CLK_EN_SHIFT          (7U)
#define CRU_CLKGATE_CON1_PCLK_UPCTL2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON1_PCLK_UPCTL2_CLK_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE_CON1_CLK_MSCH_CLK_EN_SHIFT             (8U)
#define CRU_CLKGATE_CON1_CLK_MSCH_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON1_CLK_MSCH_CLK_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE_CON1_PCLK_MSCH_CLK_EN_SHIFT            (9U)
#define CRU_CLKGATE_CON1_PCLK_MSCH_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON1_PCLK_MSCH_CLK_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE_CON1_PCLK_DDRMON_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON1_PCLK_DDRMON_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON1_PCLK_DDRMON_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON1_CLK_DDRMON_CLK_EN_SHIFT           (11U)
#define CRU_CLKGATE_CON1_CLK_DDRMON_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON1_CLK_DDRMON_CLK_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE_CON1_PCLK_DDRSTDBY_CLK_EN_SHIFT        (12U)
#define CRU_CLKGATE_CON1_PCLK_DDRSTDBY_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON1_PCLK_DDRSTDBY_CLK_EN_SHIFT)        /* 0x00001000 */
#define CRU_CLKGATE_CON1_CLK_DDRSTANBY_CLK_EN_SHIFT        (13U)
#define CRU_CLKGATE_CON1_CLK_DDRSTANBY_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON1_CLK_DDRSTANBY_CLK_EN_SHIFT)        /* 0x00002000 */
#define CRU_CLKGATE_CON1_PCLK_DDR_GRF_CLK_EN_SHIFT         (14U)
#define CRU_CLKGATE_CON1_PCLK_DDR_GRF_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON1_PCLK_DDR_GRF_CLK_EN_SHIFT)         /* 0x00004000 */
#define CRU_CLKGATE_CON1_ACLK_AXI_SPLIT_CLK_EN_SHIFT       (15U)
#define CRU_CLKGATE_CON1_ACLK_AXI_SPLIT_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON1_ACLK_AXI_SPLIT_CLK_EN_SHIFT)       /* 0x00008000 */
/* CLKGATE_CON2 */
#define CRU_CLKGATE_CON2_OFFSET                            (0x208U)
#define CRU_CLKGATE_CON2_ACLK_VO_PLL_CLK_EN_SHIFT          (0U)
#define CRU_CLKGATE_CON2_ACLK_VO_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON2_ACLK_VO_PLL_CLK_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE_CON2_CLK_RGA_CORE_PLL_CLK_EN_SHIFT     (1U)
#define CRU_CLKGATE_CON2_CLK_RGA_CORE_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON2_CLK_RGA_CORE_PLL_CLK_EN_SHIFT)     /* 0x00000002 */
#define CRU_CLKGATE_CON2_DCLK_VOPB_PLL_CLK_EN_SHIFT        (2U)
#define CRU_CLKGATE_CON2_DCLK_VOPB_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON2_DCLK_VOPB_PLL_CLK_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE_CON2_DCLK_VOPB_FRAC_SRC_CLK_EN_SHIFT   (3U)
#define CRU_CLKGATE_CON2_DCLK_VOPB_FRAC_SRC_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON2_DCLK_VOPB_FRAC_SRC_CLK_EN_SHIFT)   /* 0x00000008 */
#define CRU_CLKGATE_CON2_DCLK_VOPB_CLK_EN_SHIFT            (4U)
#define CRU_CLKGATE_CON2_DCLK_VOPB_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON2_DCLK_VOPB_CLK_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE_CON2_CLK_PWM_VOPB_PLL_CLK_EN_SHIFT     (5U)
#define CRU_CLKGATE_CON2_CLK_PWM_VOPB_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON2_CLK_PWM_VOPB_PLL_CLK_EN_SHIFT)     /* 0x00000020 */
#define CRU_CLKGATE_CON2_DCLK_VOPL_PLL_CLK_EN_SHIFT        (6U)
#define CRU_CLKGATE_CON2_DCLK_VOPL_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON2_DCLK_VOPL_PLL_CLK_EN_SHIFT)        /* 0x00000040 */
#define CRU_CLKGATE_CON2_DCLK_VOPL_FRAC_SRC_CLK_EN_SHIFT   (7U)
#define CRU_CLKGATE_CON2_DCLK_VOPL_FRAC_SRC_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON2_DCLK_VOPL_FRAC_SRC_CLK_EN_SHIFT)   /* 0x00000080 */
#define CRU_CLKGATE_CON2_DCLK_VOPL_CLK_EN_SHIFT            (8U)
#define CRU_CLKGATE_CON2_DCLK_VOPL_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON2_DCLK_VOPL_CLK_EN_SHIFT)            /* 0x00000100 */
#define CRU_CLKGATE_CON2_HCLK_VO_SRC_CLK_EN_SHIFT          (12U)
#define CRU_CLKGATE_CON2_HCLK_VO_SRC_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON2_HCLK_VO_SRC_CLK_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE_CON2_PCLK_VO_SRC_CLK_EN_SHIFT          (13U)
#define CRU_CLKGATE_CON2_PCLK_VO_SRC_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON2_PCLK_VO_SRC_CLK_EN_SHIFT)          /* 0x00002000 */
/* CLKGATE_CON3 */
#define CRU_CLKGATE_CON3_OFFSET                            (0x20CU)
#define CRU_CLKGATE_CON3_ACLK_VO_NIU_CLK_EN_SHIFT          (0U)
#define CRU_CLKGATE_CON3_ACLK_VO_NIU_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON3_ACLK_VO_NIU_CLK_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE_CON3_HCLK_VO_NIU_CLK_EN_SHIFT          (1U)
#define CRU_CLKGATE_CON3_HCLK_VO_NIU_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON3_HCLK_VO_NIU_CLK_EN_SHIFT)          /* 0x00000002 */
#define CRU_CLKGATE_CON3_PCLK_VO_NIU_CLK_EN_SHIFT          (2U)
#define CRU_CLKGATE_CON3_PCLK_VO_NIU_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON3_PCLK_VO_NIU_CLK_EN_SHIFT)          /* 0x00000004 */
#define CRU_CLKGATE_CON3_ACLK_VOPB_CLK_EN_SHIFT            (3U)
#define CRU_CLKGATE_CON3_ACLK_VOPB_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON3_ACLK_VOPB_CLK_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE_CON3_HCLK_VOPB_CLK_EN_SHIFT            (4U)
#define CRU_CLKGATE_CON3_HCLK_VOPB_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON3_HCLK_VOPB_CLK_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE_CON3_ACLK_VOPL_CLK_EN_SHIFT            (5U)
#define CRU_CLKGATE_CON3_ACLK_VOPL_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON3_ACLK_VOPL_CLK_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE_CON3_HCLK_VOPL_CLK_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON3_HCLK_VOPL_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON3_HCLK_VOPL_CLK_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON3_ACLK_RGA_CLK_EN_SHIFT             (7U)
#define CRU_CLKGATE_CON3_ACLK_RGA_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON3_ACLK_RGA_CLK_EN_SHIFT)             /* 0x00000080 */
#define CRU_CLKGATE_CON3_HCLK_RGA_CLK_EN_SHIFT             (8U)
#define CRU_CLKGATE_CON3_HCLK_RGA_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON3_HCLK_RGA_CLK_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE_CON3_PCLK_MIPI_DSI_HOST_CLK_EN_SHIFT   (9U)
#define CRU_CLKGATE_CON3_PCLK_MIPI_DSI_HOST_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON3_PCLK_MIPI_DSI_HOST_CLK_EN_SHIFT)   /* 0x00000200 */
/* CLKGATE_CON4 */
#define CRU_CLKGATE_CON4_OFFSET                            (0x210U)
#define CRU_CLKGATE_CON4_ACLK_VPU_PLL_CLK_EN_SHIFT         (0U)
#define CRU_CLKGATE_CON4_ACLK_VPU_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON4_ACLK_VPU_PLL_CLK_EN_SHIFT)         /* 0x00000001 */
#define CRU_CLKGATE_CON4_CLK_VPU_CORE_PLL_CLK_EN_SHIFT     (1U)
#define CRU_CLKGATE_CON4_CLK_VPU_CORE_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON4_CLK_VPU_CORE_PLL_CLK_EN_SHIFT)     /* 0x00000002 */
#define CRU_CLKGATE_CON4_HCLK_VPU_SRC_CLK_EN_SHIFT         (2U)
#define CRU_CLKGATE_CON4_HCLK_VPU_SRC_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON4_HCLK_VPU_SRC_CLK_EN_SHIFT)         /* 0x00000004 */
#define CRU_CLKGATE_CON4_ACLK_VPU_CLK_EN_SHIFT             (4U)
#define CRU_CLKGATE_CON4_ACLK_VPU_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON4_ACLK_VPU_CLK_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE_CON4_ACLK_VPU_NIU_CLK_EN_SHIFT         (5U)
#define CRU_CLKGATE_CON4_ACLK_VPU_NIU_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON4_ACLK_VPU_NIU_CLK_EN_SHIFT)         /* 0x00000020 */
#define CRU_CLKGATE_CON4_HCLK_VPU_CLK_EN_SHIFT             (6U)
#define CRU_CLKGATE_CON4_HCLK_VPU_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON4_HCLK_VPU_CLK_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE_CON4_HCLK_VPU_NIU_CLK_EN_SHIFT         (7U)
#define CRU_CLKGATE_CON4_HCLK_VPU_NIU_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON4_HCLK_VPU_NIU_CLK_EN_SHIFT)         /* 0x00000080 */
#define CRU_CLKGATE_CON4_ACLK_VI_PLL_CLK_EN_SHIFT          (8U)
#define CRU_CLKGATE_CON4_ACLK_VI_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON4_ACLK_VI_PLL_CLK_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE_CON4_CLK_ISP_PLL_CLK_EN_SHIFT          (9U)
#define CRU_CLKGATE_CON4_CLK_ISP_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON4_CLK_ISP_PLL_CLK_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE_CON4_CLK_CIF_OUT_PLL_CLK_EN_SHIFT      (11U)
#define CRU_CLKGATE_CON4_CLK_CIF_OUT_PLL_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON4_CLK_CIF_OUT_PLL_CLK_EN_SHIFT)      /* 0x00000800 */
#define CRU_CLKGATE_CON4_HCLK_VI_SRC_CLK_EN_SHIFT          (12U)
#define CRU_CLKGATE_CON4_HCLK_VI_SRC_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON4_HCLK_VI_SRC_CLK_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE_CON4_PCLKIN_ISP_CLK_EN_SHIFT           (13U)
#define CRU_CLKGATE_CON4_PCLKIN_ISP_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON4_PCLKIN_ISP_CLK_EN_SHIFT)           /* 0x00002000 */
#define CRU_CLKGATE_CON4_PCLKIN_CIF_CLK_EN_SHIFT           (14U)
#define CRU_CLKGATE_CON4_PCLKIN_CIF_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON4_PCLKIN_CIF_CLK_EN_SHIFT)           /* 0x00004000 */
#define CRU_CLKGATE_CON4_ACLK_VI_NIU_CLK_EN_SHIFT          (15U)
#define CRU_CLKGATE_CON4_ACLK_VI_NIU_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON4_ACLK_VI_NIU_CLK_EN_SHIFT)          /* 0x00008000 */
/* CLKGATE_CON5 */
#define CRU_CLKGATE_CON5_OFFSET                            (0x214U)
#define CRU_CLKGATE_CON5_HCLK_VI_NIU_CLK_EN_SHIFT          (0U)
#define CRU_CLKGATE_CON5_HCLK_VI_NIU_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON5_HCLK_VI_NIU_CLK_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE_CON5_ACLK_CIF_CLK_EN_SHIFT             (1U)
#define CRU_CLKGATE_CON5_ACLK_CIF_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON5_ACLK_CIF_CLK_EN_SHIFT)             /* 0x00000002 */
#define CRU_CLKGATE_CON5_HCLK_CIF_CLK_EN_SHIFT             (2U)
#define CRU_CLKGATE_CON5_HCLK_CIF_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON5_HCLK_CIF_CLK_EN_SHIFT)             /* 0x00000004 */
#define CRU_CLKGATE_CON5_ACLK_ISP_CLK_EN_SHIFT             (3U)
#define CRU_CLKGATE_CON5_ACLK_ISP_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON5_ACLK_ISP_CLK_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE_CON5_HCLK_ISP_CLK_EN_SHIFT             (4U)
#define CRU_CLKGATE_CON5_HCLK_ISP_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON5_HCLK_ISP_CLK_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE_CON5_ACLK_HCLK_PCLK_PERI_PLL_CLK_EN_SHIFT (7U)
#define CRU_CLKGATE_CON5_ACLK_HCLK_PCLK_PERI_PLL_CLK_EN_MASK (0x1U << CRU_CLKGATE_CON5_ACLK_HCLK_PCLK_PERI_PLL_CLK_EN_SHIFT) /* 0x00000080 */
#define CRU_CLKGATE_CON5_ACLK_PERI_CLK_EN_SHIFT            (8U)
#define CRU_CLKGATE_CON5_ACLK_PERI_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON5_ACLK_PERI_CLK_EN_SHIFT)            /* 0x00000100 */
#define CRU_CLKGATE_CON5_ACLK_PERI_NIU_CLK_EN_SHIFT        (9U)
#define CRU_CLKGATE_CON5_ACLK_PERI_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON5_ACLK_PERI_NIU_CLK_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE_CON5_CLK_NANDC_PLL_CLK_EN_SHIFT        (11U)
#define CRU_CLKGATE_CON5_CLK_NANDC_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON5_CLK_NANDC_PLL_CLK_EN_SHIFT)        /* 0x00000800 */
#define CRU_CLKGATE_CON5_CLK_NANDC_DIV50_CLK_EN_SHIFT      (12U)
#define CRU_CLKGATE_CON5_CLK_NANDC_DIV50_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON5_CLK_NANDC_DIV50_CLK_EN_SHIFT)      /* 0x00001000 */
#define CRU_CLKGATE_CON5_CLK_NANDC_CLK_EN_SHIFT            (13U)
#define CRU_CLKGATE_CON5_CLK_NANDC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON5_CLK_NANDC_CLK_EN_SHIFT)            /* 0x00002000 */
#define CRU_CLKGATE_CON5_HCLK_NANDC_CLK_EN_SHIFT           (15U)
#define CRU_CLKGATE_CON5_HCLK_NANDC_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON5_HCLK_NANDC_CLK_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE_CON6 */
#define CRU_CLKGATE_CON6_OFFSET                            (0x218U)
#define CRU_CLKGATE_CON6_HCLK_PDMMC_NAND_CLK_EN_SHIFT      (0U)
#define CRU_CLKGATE_CON6_HCLK_PDMMC_NAND_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON6_HCLK_PDMMC_NAND_CLK_EN_SHIFT)      /* 0x00000001 */
#define CRU_CLKGATE_CON6_CLK_SDIO_PLL_CLK_EN_SHIFT         (1U)
#define CRU_CLKGATE_CON6_CLK_SDIO_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON6_CLK_SDIO_PLL_CLK_EN_SHIFT)         /* 0x00000002 */
#define CRU_CLKGATE_CON6_CLK_SDIO_DIV50_CLK_EN_SHIFT       (2U)
#define CRU_CLKGATE_CON6_CLK_SDIO_DIV50_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON6_CLK_SDIO_DIV50_CLK_EN_SHIFT)       /* 0x00000004 */
#define CRU_CLKGATE_CON6_CLK_SDIO_CLK_EN_SHIFT             (3U)
#define CRU_CLKGATE_CON6_CLK_SDIO_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON6_CLK_SDIO_CLK_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE_CON6_CLK_EMMC_PLL_CLK_EN_SHIFT         (4U)
#define CRU_CLKGATE_CON6_CLK_EMMC_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON6_CLK_EMMC_PLL_CLK_EN_SHIFT)         /* 0x00000010 */
#define CRU_CLKGATE_CON6_CLK_EMMC_DIV50_CLK_EN_SHIFT       (5U)
#define CRU_CLKGATE_CON6_CLK_EMMC_DIV50_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON6_CLK_EMMC_DIV50_CLK_EN_SHIFT)       /* 0x00000020 */
#define CRU_CLKGATE_CON6_CLK_EMMC_CLK_EN_SHIFT             (6U)
#define CRU_CLKGATE_CON6_CLK_EMMC_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON6_CLK_EMMC_CLK_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE_CON6_CLK_SFC_PLL_CLK_EN_SHIFT          (7U)
#define CRU_CLKGATE_CON6_CLK_SFC_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON6_CLK_SFC_PLL_CLK_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE_CON6_HCLK_PDMMC_NAND_NIU_CLK_EN_SHIFT  (8U)
#define CRU_CLKGATE_CON6_HCLK_PDMMC_NAND_NIU_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON6_HCLK_PDMMC_NAND_NIU_CLK_EN_SHIFT)  /* 0x00000100 */
#define CRU_CLKGATE_CON6_HCLK_SDIO_CLK_EN_SHIFT            (9U)
#define CRU_CLKGATE_CON6_HCLK_SDIO_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON6_HCLK_SDIO_CLK_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE_CON6_HCLK_EMMC_CLK_EN_SHIFT            (10U)
#define CRU_CLKGATE_CON6_HCLK_EMMC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON6_HCLK_EMMC_CLK_EN_SHIFT)            /* 0x00000400 */
#define CRU_CLKGATE_CON6_HCLK_SFC_CLK_EN_SHIFT             (11U)
#define CRU_CLKGATE_CON6_HCLK_SFC_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON6_HCLK_SFC_CLK_EN_SHIFT)             /* 0x00000800 */
#define CRU_CLKGATE_CON6_HCLK_PDSDCARD_CLK_EN_SHIFT        (12U)
#define CRU_CLKGATE_CON6_HCLK_PDSDCARD_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON6_HCLK_PDSDCARD_CLK_EN_SHIFT)        /* 0x00001000 */
#define CRU_CLKGATE_CON6_CLK_SDMMC_PLL_CLK_EN_SHIFT        (13U)
#define CRU_CLKGATE_CON6_CLK_SDMMC_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON6_CLK_SDMMC_PLL_CLK_EN_SHIFT)        /* 0x00002000 */
#define CRU_CLKGATE_CON6_CLK_SDMMC_DIV50_CLK_EN_SHIFT      (14U)
#define CRU_CLKGATE_CON6_CLK_SDMMC_DIV50_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON6_CLK_SDMMC_DIV50_CLK_EN_SHIFT)      /* 0x00004000 */
#define CRU_CLKGATE_CON6_CLK_SDMMC_CLK_EN_SHIFT            (15U)
#define CRU_CLKGATE_CON6_CLK_SDMMC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON6_CLK_SDMMC_CLK_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE_CON7 */
#define CRU_CLKGATE_CON7_OFFSET                            (0x21CU)
#define CRU_CLKGATE_CON7_HCLK_PDSDCARD_NIU_CLK_EN_SHIFT    (0U)
#define CRU_CLKGATE_CON7_HCLK_PDSDCARD_NIU_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON7_HCLK_PDSDCARD_NIU_CLK_EN_SHIFT)    /* 0x00000001 */
#define CRU_CLKGATE_CON7_HCLK_SDMMC_CLK_EN_SHIFT           (1U)
#define CRU_CLKGATE_CON7_HCLK_SDMMC_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON7_HCLK_SDMMC_CLK_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE_CON7_HCLK_PDUSB_CLK_EN_SHIFT           (2U)
#define CRU_CLKGATE_CON7_HCLK_PDUSB_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON7_HCLK_PDUSB_CLK_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE_CON7_CLK_OTG_ADP_CLK_EN_SHIFT          (3U)
#define CRU_CLKGATE_CON7_CLK_OTG_ADP_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON7_CLK_OTG_ADP_CLK_EN_SHIFT)          /* 0x00000008 */
#define CRU_CLKGATE_CON7_HCLK_PDUSB_NIU_CLK_EN_SHIFT       (4U)
#define CRU_CLKGATE_CON7_HCLK_PDUSB_NIU_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON7_HCLK_PDUSB_NIU_CLK_EN_SHIFT)       /* 0x00000010 */
#define CRU_CLKGATE_CON7_HCLK_USB2OTG_CLK_EN_SHIFT         (5U)
#define CRU_CLKGATE_CON7_HCLK_USB2OTG_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON7_HCLK_USB2OTG_CLK_EN_SHIFT)         /* 0x00000020 */
#define CRU_CLKGATE_CON7_HCLK_USB2HOST_CLK_EN_SHIFT        (6U)
#define CRU_CLKGATE_CON7_HCLK_USB2HOST_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON7_HCLK_USB2HOST_CLK_EN_SHIFT)        /* 0x00000040 */
#define CRU_CLKGATE_CON7_HCLK_USB2HOST_ARB_CLK_EN_SHIFT    (8U)
#define CRU_CLKGATE_CON7_HCLK_USB2HOST_ARB_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON7_HCLK_USB2HOST_ARB_CLK_EN_SHIFT)    /* 0x00000100 */
#define CRU_CLKGATE_CON7_ACLK_PDGMAC_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON7_ACLK_PDGMAC_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON7_ACLK_PDGMAC_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON7_CLK_GMAC_PLL_CLK_EN_SHIFT         (11U)
#define CRU_CLKGATE_CON7_CLK_GMAC_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON7_CLK_GMAC_PLL_CLK_EN_SHIFT)         /* 0x00000800 */
#define CRU_CLKGATE_CON7_PCLK_GMAC_SRC_CLK_EN_SHIFT        (12U)
#define CRU_CLKGATE_CON7_PCLK_GMAC_SRC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON7_PCLK_GMAC_SRC_CLK_EN_SHIFT)        /* 0x00001000 */
#define CRU_CLKGATE_CON7_CLK_GMAC_TX_RX_CLK_EN_SHIFT       (13U)
#define CRU_CLKGATE_CON7_CLK_GMAC_TX_RX_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON7_CLK_GMAC_TX_RX_CLK_EN_SHIFT)       /* 0x00002000 */
#define CRU_CLKGATE_CON7_CLK_GMAC_REF_CLK_EN_SHIFT         (15U)
#define CRU_CLKGATE_CON7_CLK_GMAC_REF_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON7_CLK_GMAC_REF_CLK_EN_SHIFT)         /* 0x00008000 */
/* CLKGATE_CON8 */
#define CRU_CLKGATE_CON8_OFFSET                            (0x220U)
#define CRU_CLKGATE_CON8_ACLK_GMAC_NIU_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON8_ACLK_GMAC_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON8_ACLK_GMAC_NIU_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON8_PCLK_GMAC_NIU_CLK_EN_SHIFT        (1U)
#define CRU_CLKGATE_CON8_PCLK_GMAC_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON8_PCLK_GMAC_NIU_CLK_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE_CON8_ACLK_GMAC_CLK_EN_SHIFT            (2U)
#define CRU_CLKGATE_CON8_ACLK_GMAC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON8_ACLK_GMAC_CLK_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE_CON8_PCLK_GMAC_CLK_EN_SHIFT            (3U)
#define CRU_CLKGATE_CON8_PCLK_GMAC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON8_PCLK_GMAC_CLK_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE_CON8_CLK_GMAC_OUT_PLL_CLK_EN_SHIFT     (5U)
#define CRU_CLKGATE_CON8_CLK_GMAC_OUT_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON8_CLK_GMAC_OUT_PLL_CLK_EN_SHIFT)     /* 0x00000020 */
#define CRU_CLKGATE_CON8_PD_BUS_PLL_CLK_EN_SHIFT           (6U)
#define CRU_CLKGATE_CON8_PD_BUS_PLL_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON8_PD_BUS_PLL_CLK_EN_SHIFT)           /* 0x00000040 */
#define CRU_CLKGATE_CON8_ACLK_BUS_CLK_EN_SHIFT             (7U)
#define CRU_CLKGATE_CON8_ACLK_BUS_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON8_ACLK_BUS_CLK_EN_SHIFT)             /* 0x00000080 */
#define CRU_CLKGATE_CON8_HCLK_BUS_CLK_EN_SHIFT             (8U)
#define CRU_CLKGATE_CON8_HCLK_BUS_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON8_HCLK_BUS_CLK_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE_CON8_PCLK_BUS_CLK_EN_SHIFT             (9U)
#define CRU_CLKGATE_CON8_PCLK_BUS_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON8_PCLK_BUS_CLK_EN_SHIFT)             /* 0x00000200 */
#define CRU_CLKGATE_CON8_PCLK_TOP_CLK_EN_SHIFT             (10U)
#define CRU_CLKGATE_CON8_PCLK_TOP_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON8_PCLK_TOP_CLK_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE_CON8_ACLK_PDCRYPTO_CLK_EN_SHIFT        (12U)
#define CRU_CLKGATE_CON8_ACLK_PDCRYPTO_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON8_ACLK_PDCRYPTO_CLK_EN_SHIFT)        /* 0x00001000 */
#define CRU_CLKGATE_CON8_HCLK_PDCRYPTO_CLK_EN_SHIFT        (13U)
#define CRU_CLKGATE_CON8_HCLK_PDCRYPTO_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON8_HCLK_PDCRYPTO_CLK_EN_SHIFT)        /* 0x00002000 */
#define CRU_CLKGATE_CON8_CLK_CRYPTO_PLL_CLK_EN_SHIFT       (14U)
#define CRU_CLKGATE_CON8_CLK_CRYPTO_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON8_CLK_CRYPTO_PLL_CLK_EN_SHIFT)       /* 0x00004000 */
#define CRU_CLKGATE_CON8_CLK_CRYPTO_APK_PLL_CLK_EN_SHIFT   (15U)
#define CRU_CLKGATE_CON8_CLK_CRYPTO_APK_PLL_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON8_CLK_CRYPTO_APK_PLL_CLK_EN_SHIFT)   /* 0x00008000 */
/* CLKGATE_CON9 */
#define CRU_CLKGATE_CON9_OFFSET                            (0x224U)
#define CRU_CLKGATE_CON9_ACLK_CRYPTO_NIU_CLK_EN_SHIFT      (2U)
#define CRU_CLKGATE_CON9_ACLK_CRYPTO_NIU_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON9_ACLK_CRYPTO_NIU_CLK_EN_SHIFT)      /* 0x00000004 */
#define CRU_CLKGATE_CON9_HCLK_CRYPTO_NIU_CLK_EN_SHIFT      (3U)
#define CRU_CLKGATE_CON9_HCLK_CRYPTO_NIU_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON9_HCLK_CRYPTO_NIU_CLK_EN_SHIFT)      /* 0x00000008 */
#define CRU_CLKGATE_CON9_ACLK_CRYPTO_CLK_EN_SHIFT          (4U)
#define CRU_CLKGATE_CON9_ACLK_CRYPTO_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON9_ACLK_CRYPTO_CLK_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE_CON9_HCLK_CRYPTO_CLK_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON9_HCLK_CRYPTO_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON9_HCLK_CRYPTO_CLK_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON9_CLK_PDM_PLL_CLK_EN_SHIFT          (9U)
#define CRU_CLKGATE_CON9_CLK_PDM_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON9_CLK_PDM_PLL_CLK_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE_CON9_CLK_PDM_FRAC_SRC_CLK_EN_SHIFT     (10U)
#define CRU_CLKGATE_CON9_CLK_PDM_FRAC_SRC_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON9_CLK_PDM_FRAC_SRC_CLK_EN_SHIFT)     /* 0x00000400 */
#define CRU_CLKGATE_CON9_CLK_PDM_CLK_EN_SHIFT              (11U)
#define CRU_CLKGATE_CON9_CLK_PDM_CLK_EN_MASK               (0x1U << CRU_CLKGATE_CON9_CLK_PDM_CLK_EN_SHIFT)              /* 0x00000800 */
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_PLL_CLK_EN_SHIFT      (12U)
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_PLL_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON9_CLK_I2S0_TX_PLL_CLK_EN_SHIFT)      /* 0x00001000 */
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_FRAC_SRC_CLK_EN_SHIFT (13U)
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_FRAC_SRC_CLK_EN_MASK  (0x1U << CRU_CLKGATE_CON9_CLK_I2S0_TX_FRAC_SRC_CLK_EN_SHIFT) /* 0x00002000 */
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_CLK_EN_SHIFT          (14U)
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON9_CLK_I2S0_TX_CLK_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_OUT_MCLK_EN_SHIFT     (15U)
#define CRU_CLKGATE_CON9_CLK_I2S0_TX_OUT_MCLK_EN_MASK      (0x1U << CRU_CLKGATE_CON9_CLK_I2S0_TX_OUT_MCLK_EN_SHIFT)     /* 0x00008000 */
/* CLKGATE_CON10 */
#define CRU_CLKGATE_CON10_OFFSET                           (0x228U)
#define CRU_CLKGATE_CON10_CLK_I2S1_PLL_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON10_CLK_I2S1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON10_CLK_I2S1_PLL_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON10_CLK_I2S1_FRAC_SRC_CLK_EN_SHIFT   (1U)
#define CRU_CLKGATE_CON10_CLK_I2S1_FRAC_SRC_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON10_CLK_I2S1_FRAC_SRC_CLK_EN_SHIFT)   /* 0x00000002 */
#define CRU_CLKGATE_CON10_CLK_I2S1_CLK_EN_SHIFT            (2U)
#define CRU_CLKGATE_CON10_CLK_I2S1_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON10_CLK_I2S1_CLK_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE_CON10_CLK_I2S1_OUT_MCLK_EN_SHIFT       (3U)
#define CRU_CLKGATE_CON10_CLK_I2S1_OUT_MCLK_EN_MASK        (0x1U << CRU_CLKGATE_CON10_CLK_I2S1_OUT_MCLK_EN_SHIFT)       /* 0x00000008 */
#define CRU_CLKGATE_CON10_CLK_I2S2_PLL_CLK_EN_SHIFT        (4U)
#define CRU_CLKGATE_CON10_CLK_I2S2_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON10_CLK_I2S2_PLL_CLK_EN_SHIFT)        /* 0x00000010 */
#define CRU_CLKGATE_CON10_CLK_I2S2_FRAC_SRC_CLK_EN_SHIFT   (5U)
#define CRU_CLKGATE_CON10_CLK_I2S2_FRAC_SRC_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON10_CLK_I2S2_FRAC_SRC_CLK_EN_SHIFT)   /* 0x00000020 */
#define CRU_CLKGATE_CON10_CLK_I2S2_CLK_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON10_CLK_I2S2_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON10_CLK_I2S2_CLK_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON10_CLK_I2S2_OUT_MCLK_EN_SHIFT       (7U)
#define CRU_CLKGATE_CON10_CLK_I2S2_OUT_MCLK_EN_MASK        (0x1U << CRU_CLKGATE_CON10_CLK_I2S2_OUT_MCLK_EN_SHIFT)       /* 0x00000080 */
#define CRU_CLKGATE_CON10_CLK_I2S0_TX_OUT_MCLK_OE_SHIFT    (8U)
#define CRU_CLKGATE_CON10_CLK_I2S0_TX_OUT_MCLK_OE_MASK     (0x1U << CRU_CLKGATE_CON10_CLK_I2S0_TX_OUT_MCLK_OE_SHIFT)    /* 0x00000100 */
#define CRU_CLKGATE_CON10_CLK_I2S1_OUT_MCLK_OE_SHIFT       (9U)
#define CRU_CLKGATE_CON10_CLK_I2S1_OUT_MCLK_OE_MASK        (0x1U << CRU_CLKGATE_CON10_CLK_I2S1_OUT_MCLK_OE_SHIFT)       /* 0x00000200 */
#define CRU_CLKGATE_CON10_CLK_I2S2_OUT_MCLK_OE_SHIFT       (10U)
#define CRU_CLKGATE_CON10_CLK_I2S2_OUT_MCLK_OE_MASK        (0x1U << CRU_CLKGATE_CON10_CLK_I2S2_OUT_MCLK_OE_SHIFT)       /* 0x00000400 */
#define CRU_CLKGATE_CON10_CLK_I2S0_RX_OUT_MCLK_OE_SHIFT    (11U)
#define CRU_CLKGATE_CON10_CLK_I2S0_RX_OUT_MCLK_OE_MASK     (0x1U << CRU_CLKGATE_CON10_CLK_I2S0_RX_OUT_MCLK_OE_SHIFT)    /* 0x00000800 */
#define CRU_CLKGATE_CON10_CLK_UART1_PLL_CLK_EN_SHIFT       (12U)
#define CRU_CLKGATE_CON10_CLK_UART1_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON10_CLK_UART1_PLL_CLK_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE_CON10_CLK_UART1_DIVNP5_CLK_EN_SHIFT    (13U)
#define CRU_CLKGATE_CON10_CLK_UART1_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON10_CLK_UART1_DIVNP5_CLK_EN_SHIFT)    /* 0x00002000 */
#define CRU_CLKGATE_CON10_CLK_UART1_FRAC_SRC_CLK_EN_SHIFT  (14U)
#define CRU_CLKGATE_CON10_CLK_UART1_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON10_CLK_UART1_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00004000 */
#define CRU_CLKGATE_CON10_CLK_UART1_CLK_EN_SHIFT           (15U)
#define CRU_CLKGATE_CON10_CLK_UART1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON10_CLK_UART1_CLK_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE_CON11 */
#define CRU_CLKGATE_CON11_OFFSET                           (0x22CU)
#define CRU_CLKGATE_CON11_CLK_UART2_PLL_CLK_EN_SHIFT       (0U)
#define CRU_CLKGATE_CON11_CLK_UART2_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON11_CLK_UART2_PLL_CLK_EN_SHIFT)       /* 0x00000001 */
#define CRU_CLKGATE_CON11_CLK_UART2_DIVNP5_CLK_EN_SHIFT    (1U)
#define CRU_CLKGATE_CON11_CLK_UART2_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON11_CLK_UART2_DIVNP5_CLK_EN_SHIFT)    /* 0x00000002 */
#define CRU_CLKGATE_CON11_CLK_UART2_FRAC_SRC_CLK_EN_SHIFT  (2U)
#define CRU_CLKGATE_CON11_CLK_UART2_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_UART2_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000004 */
#define CRU_CLKGATE_CON11_CLK_UART2_CLK_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON11_CLK_UART2_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_CLK_UART2_CLK_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON11_CLK_UART3_PLL_CLK_EN_SHIFT       (4U)
#define CRU_CLKGATE_CON11_CLK_UART3_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON11_CLK_UART3_PLL_CLK_EN_SHIFT)       /* 0x00000010 */
#define CRU_CLKGATE_CON11_CLK_UART3_DIVNP5_CLK_EN_SHIFT    (5U)
#define CRU_CLKGATE_CON11_CLK_UART3_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON11_CLK_UART3_DIVNP5_CLK_EN_SHIFT)    /* 0x00000020 */
#define CRU_CLKGATE_CON11_CLK_UART3_FRAC_SRC_CLK_EN_SHIFT  (6U)
#define CRU_CLKGATE_CON11_CLK_UART3_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_UART3_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000040 */
#define CRU_CLKGATE_CON11_CLK_UART3_CLK_EN_SHIFT           (7U)
#define CRU_CLKGATE_CON11_CLK_UART3_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_CLK_UART3_CLK_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE_CON11_CLK_UART4_PLL_CLK_EN_SHIFT       (8U)
#define CRU_CLKGATE_CON11_CLK_UART4_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON11_CLK_UART4_PLL_CLK_EN_SHIFT)       /* 0x00000100 */
#define CRU_CLKGATE_CON11_CLK_UART4_DIVNP5_CLK_EN_SHIFT    (9U)
#define CRU_CLKGATE_CON11_CLK_UART4_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON11_CLK_UART4_DIVNP5_CLK_EN_SHIFT)    /* 0x00000200 */
#define CRU_CLKGATE_CON11_CLK_UART4_FRAC_SRC_CLK_EN_SHIFT  (10U)
#define CRU_CLKGATE_CON11_CLK_UART4_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_UART4_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000400 */
#define CRU_CLKGATE_CON11_CLK_UART4_CLK_EN_SHIFT           (11U)
#define CRU_CLKGATE_CON11_CLK_UART4_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_CLK_UART4_CLK_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE_CON11_CLK_UART5_PLL_CLK_EN_SHIFT       (12U)
#define CRU_CLKGATE_CON11_CLK_UART5_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON11_CLK_UART5_PLL_CLK_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE_CON11_CLK_UART5_DIVNP5_CLK_EN_SHIFT    (13U)
#define CRU_CLKGATE_CON11_CLK_UART5_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON11_CLK_UART5_DIVNP5_CLK_EN_SHIFT)    /* 0x00002000 */
#define CRU_CLKGATE_CON11_CLK_UART5_FRAC_SRC_CLK_EN_SHIFT  (14U)
#define CRU_CLKGATE_CON11_CLK_UART5_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_UART5_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00004000 */
#define CRU_CLKGATE_CON11_CLK_UART5_CLK_EN_SHIFT           (15U)
#define CRU_CLKGATE_CON11_CLK_UART5_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_CLK_UART5_CLK_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE_CON12 */
#define CRU_CLKGATE_CON12_OFFSET                           (0x230U)
#define CRU_CLKGATE_CON12_CLK_I2C0_PLL_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON12_CLK_I2C0_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_I2C0_PLL_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON12_CLK_I2C1_PLL_CLK_EN_SHIFT        (1U)
#define CRU_CLKGATE_CON12_CLK_I2C1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_I2C1_PLL_CLK_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE_CON12_CLK_I2C2_PLL_CLK_EN_SHIFT        (2U)
#define CRU_CLKGATE_CON12_CLK_I2C2_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_I2C2_PLL_CLK_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE_CON12_CLK_I2C3_PLL_CLK_EN_SHIFT        (3U)
#define CRU_CLKGATE_CON12_CLK_I2C3_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_I2C3_PLL_CLK_EN_SHIFT)        /* 0x00000008 */
#define CRU_CLKGATE_CON12_CLK_PWM0_PLL_CLK_EN_SHIFT        (5U)
#define CRU_CLKGATE_CON12_CLK_PWM0_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_PWM0_PLL_CLK_EN_SHIFT)        /* 0x00000020 */
#define CRU_CLKGATE_CON12_CLK_PWM1_PLL_CLK_EN_SHIFT        (6U)
#define CRU_CLKGATE_CON12_CLK_PWM1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_PWM1_PLL_CLK_EN_SHIFT)        /* 0x00000040 */
#define CRU_CLKGATE_CON12_CLK_SPI0_PLL_CLK_EN_SHIFT        (7U)
#define CRU_CLKGATE_CON12_CLK_SPI0_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_SPI0_PLL_CLK_EN_SHIFT)        /* 0x00000080 */
#define CRU_CLKGATE_CON12_CLK_SPI1_PLL_CLK_EN_SHIFT        (8U)
#define CRU_CLKGATE_CON12_CLK_SPI1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON12_CLK_SPI1_PLL_CLK_EN_SHIFT)        /* 0x00000100 */
#define CRU_CLKGATE_CON12_CLK_TSADC_PLL_CLK_EN_SHIFT       (9U)
#define CRU_CLKGATE_CON12_CLK_TSADC_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON12_CLK_TSADC_PLL_CLK_EN_SHIFT)       /* 0x00000200 */
#define CRU_CLKGATE_CON12_CLK_SARADC_PLL_CLK_EN_SHIFT      (10U)
#define CRU_CLKGATE_CON12_CLK_SARADC_PLL_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON12_CLK_SARADC_PLL_CLK_EN_SHIFT)      /* 0x00000400 */
#define CRU_CLKGATE_CON12_CLK_OTP_PLL_CLK_EN_SHIFT         (11U)
#define CRU_CLKGATE_CON12_CLK_OTP_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON12_CLK_OTP_PLL_CLK_EN_SHIFT)         /* 0x00000800 */
#define CRU_CLKGATE_CON12_CLK_CPU_BOOST_CLK_EN_SHIFT       (12U)
#define CRU_CLKGATE_CON12_CLK_CPU_BOOST_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON12_CLK_CPU_BOOST_CLK_EN_SHIFT)       /* 0x00001000 */
/* CLKGATE_CON13 */
#define CRU_CLKGATE_CON13_OFFSET                           (0x234U)
#define CRU_CLKGATE_CON13_CLK_TIMER0_CLK_EN_SHIFT          (0U)
#define CRU_CLKGATE_CON13_CLK_TIMER0_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON13_CLK_TIMER0_CLK_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE_CON13_CLK_TIMER1_CLK_EN_SHIFT          (1U)
#define CRU_CLKGATE_CON13_CLK_TIMER1_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON13_CLK_TIMER1_CLK_EN_SHIFT)          /* 0x00000002 */
#define CRU_CLKGATE_CON13_CLK_TIMER2_CLK_EN_SHIFT          (2U)
#define CRU_CLKGATE_CON13_CLK_TIMER2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON13_CLK_TIMER2_CLK_EN_SHIFT)          /* 0x00000004 */
#define CRU_CLKGATE_CON13_CLK_TIMER3_CLK_EN_SHIFT          (3U)
#define CRU_CLKGATE_CON13_CLK_TIMER3_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON13_CLK_TIMER3_CLK_EN_SHIFT)          /* 0x00000008 */
#define CRU_CLKGATE_CON13_CLK_TIMER4_CLK_EN_SHIFT          (4U)
#define CRU_CLKGATE_CON13_CLK_TIMER4_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON13_CLK_TIMER4_CLK_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE_CON13_CLK_TIMER5_CLK_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON13_CLK_TIMER5_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON13_CLK_TIMER5_CLK_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON13_CLK_OTP_USR_CLK_EN_SHIFT         (6U)
#define CRU_CLKGATE_CON13_CLK_OTP_USR_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON13_CLK_OTP_USR_CLK_EN_SHIFT)         /* 0x00000040 */
#define CRU_CLKGATE_CON13_ACLK_BUS_NIU_CLK_EN_SHIFT        (8U)
#define CRU_CLKGATE_CON13_ACLK_BUS_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_ACLK_BUS_NIU_CLK_EN_SHIFT)        /* 0x00000100 */
#define CRU_CLKGATE_CON13_HCLK_BUS_NIU_CLK_EN_SHIFT        (9U)
#define CRU_CLKGATE_CON13_HCLK_BUS_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_HCLK_BUS_NIU_CLK_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE_CON13_PCLK_BUS_NIU_CLK_EN_SHIFT        (10U)
#define CRU_CLKGATE_CON13_PCLK_BUS_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_PCLK_BUS_NIU_CLK_EN_SHIFT)        /* 0x00000400 */
#define CRU_CLKGATE_CON13_ACLK_INTMEM_CLK_EN_SHIFT         (11U)
#define CRU_CLKGATE_CON13_ACLK_INTMEM_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON13_ACLK_INTMEM_CLK_EN_SHIFT)         /* 0x00000800 */
#define CRU_CLKGATE_CON13_ACLK_GIC_CLK_EN_SHIFT            (12U)
#define CRU_CLKGATE_CON13_ACLK_GIC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON13_ACLK_GIC_CLK_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE_CON13_HCLK_ROM_CLK_EN_SHIFT            (14U)
#define CRU_CLKGATE_CON13_HCLK_ROM_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON13_HCLK_ROM_CLK_EN_SHIFT)            /* 0x00004000 */
#define CRU_CLKGATE_CON13_ACLK_DFC_CLK_EN_SHIFT            (15U)
#define CRU_CLKGATE_CON13_ACLK_DFC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON13_ACLK_DFC_CLK_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE_CON14 */
#define CRU_CLKGATE_CON14_OFFSET                           (0x238U)
#define CRU_CLKGATE_CON14_PCLK_DCF_CLK_EN_SHIFT            (0U)
#define CRU_CLKGATE_CON14_PCLK_DCF_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON14_PCLK_DCF_CLK_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE_CON14_HCLK_PDM_CLK_EN_SHIFT            (1U)
#define CRU_CLKGATE_CON14_HCLK_PDM_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON14_HCLK_PDM_CLK_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE_CON14_HCLK_I2S0_CLK_EN_SHIFT           (2U)
#define CRU_CLKGATE_CON14_HCLK_I2S0_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_HCLK_I2S0_CLK_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE_CON14_HCLK_I2S1_CLK_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON14_HCLK_I2S1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_HCLK_I2S1_CLK_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON14_HCLK_I2S2_CLK_EN_SHIFT           (4U)
#define CRU_CLKGATE_CON14_HCLK_I2S2_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_HCLK_I2S2_CLK_EN_SHIFT)           /* 0x00000010 */
#define CRU_CLKGATE_CON14_PCLK_UART1_CLK_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON14_PCLK_UART1_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_PCLK_UART1_CLK_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON14_PCLK_UART2_CLK_EN_SHIFT          (6U)
#define CRU_CLKGATE_CON14_PCLK_UART2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_PCLK_UART2_CLK_EN_SHIFT)          /* 0x00000040 */
#define CRU_CLKGATE_CON14_PCLK_UART3_CLK_EN_SHIFT          (7U)
#define CRU_CLKGATE_CON14_PCLK_UART3_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_PCLK_UART3_CLK_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE_CON14_PCLK_UART4_CLK_EN_SHIFT          (8U)
#define CRU_CLKGATE_CON14_PCLK_UART4_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_PCLK_UART4_CLK_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE_CON14_PCLK_UART5_CLK_EN_SHIFT          (9U)
#define CRU_CLKGATE_CON14_PCLK_UART5_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_PCLK_UART5_CLK_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE_CON14_PCLK_I2C0_CLK_EN_SHIFT           (10U)
#define CRU_CLKGATE_CON14_PCLK_I2C0_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_PCLK_I2C0_CLK_EN_SHIFT)           /* 0x00000400 */
#define CRU_CLKGATE_CON14_PCLK_I2C1_CLK_EN_SHIFT           (11U)
#define CRU_CLKGATE_CON14_PCLK_I2C1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_PCLK_I2C1_CLK_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE_CON14_PCLK_I2C2_CLK_EN_SHIFT           (12U)
#define CRU_CLKGATE_CON14_PCLK_I2C2_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_PCLK_I2C2_CLK_EN_SHIFT)           /* 0x00001000 */
#define CRU_CLKGATE_CON14_PCLK_I2C3_CLK_EN_SHIFT           (13U)
#define CRU_CLKGATE_CON14_PCLK_I2C3_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_PCLK_I2C3_CLK_EN_SHIFT)           /* 0x00002000 */
#define CRU_CLKGATE_CON14_PCLK_PWM0_CLK_EN_SHIFT           (15U)
#define CRU_CLKGATE_CON14_PCLK_PWM0_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON14_PCLK_PWM0_CLK_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE_CON15 */
#define CRU_CLKGATE_CON15_OFFSET                           (0x23CU)
#define CRU_CLKGATE_CON15_PCLK_PWM1_CLK_EN_SHIFT           (0U)
#define CRU_CLKGATE_CON15_PCLK_PWM1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON15_PCLK_PWM1_CLK_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE_CON15_PCLK_SPI0_CLK_EN_SHIFT           (1U)
#define CRU_CLKGATE_CON15_PCLK_SPI0_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON15_PCLK_SPI0_CLK_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE_CON15_PCLK_SPI1_CLK_EN_SHIFT           (2U)
#define CRU_CLKGATE_CON15_PCLK_SPI1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON15_PCLK_SPI1_CLK_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE_CON15_PCLK_SARADC_CLK_EN_SHIFT         (3U)
#define CRU_CLKGATE_CON15_PCLK_SARADC_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON15_PCLK_SARADC_CLK_EN_SHIFT)         /* 0x00000008 */
#define CRU_CLKGATE_CON15_PCLK_TSADC_CLK_EN_SHIFT          (4U)
#define CRU_CLKGATE_CON15_PCLK_TSADC_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_TSADC_CLK_EN_SHIFT)          /* 0x00000010 */
#define CRU_CLKGATE_CON15_PCLK_TIMER_CLK_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON15_PCLK_TIMER_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_TIMER_CLK_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON15_PCLK_OTP_NS_CLK_EN_SHIFT         (6U)
#define CRU_CLKGATE_CON15_PCLK_OTP_NS_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON15_PCLK_OTP_NS_CLK_EN_SHIFT)         /* 0x00000040 */
#define CRU_CLKGATE_CON15_PCLK_WDT_NS_EN_SHIFT             (7U)
#define CRU_CLKGATE_CON15_PCLK_WDT_NS_EN_MASK              (0x1U << CRU_CLKGATE_CON15_PCLK_WDT_NS_EN_SHIFT)             /* 0x00000080 */
#define CRU_CLKGATE_CON15_PCLK_GPIO1_CLK_EN_SHIFT          (8U)
#define CRU_CLKGATE_CON15_PCLK_GPIO1_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_GPIO1_CLK_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE_CON15_PCLK_GPIO2_CLK_EN_SHIFT          (9U)
#define CRU_CLKGATE_CON15_PCLK_GPIO2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_GPIO2_CLK_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE_CON15_PCLK_GPIO3_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON15_PCLK_GPIO3_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_GPIO3_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON15_PCLK_BUS_GRF_CLK_EN_SHIFT        (11U)
#define CRU_CLKGATE_CON15_PCLK_BUS_GRF_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON15_PCLK_BUS_GRF_CLK_EN_SHIFT)        /* 0x00000800 */
#define CRU_CLKGATE_CON15_PCLK_BUS_SGRF_CLK_EN_SHIFT       (12U)
#define CRU_CLKGATE_CON15_PCLK_BUS_SGRF_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON15_PCLK_BUS_SGRF_CLK_EN_SHIFT)       /* 0x00001000 */
/* CLKGATE_CON16 */
#define CRU_CLKGATE_CON16_OFFSET                           (0x240U)
#define CRU_CLKGATE_CON16_PCLK_TOP_NIU_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON16_PCLK_TOP_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON16_PCLK_TOP_NIU_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON16_PCLK_TOP_CRU_CLK_EN_SHIFT        (1U)
#define CRU_CLKGATE_CON16_PCLK_TOP_CRU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON16_PCLK_TOP_CRU_CLK_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE_CON16_PCLK_OTP_PHY_CLK_EN_SHIFT        (2U)
#define CRU_CLKGATE_CON16_PCLK_OTP_PHY_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON16_PCLK_OTP_PHY_CLK_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE_CON16_PCLK_DDRPHY_CLK_EN_SHIFT         (3U)
#define CRU_CLKGATE_CON16_PCLK_DDRPHY_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON16_PCLK_DDRPHY_CLK_EN_SHIFT)         /* 0x00000008 */
#define CRU_CLKGATE_CON16_PCLK_MIPIDSIPHY_CLK_EN_SHIFT     (4U)
#define CRU_CLKGATE_CON16_PCLK_MIPIDSIPHY_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON16_PCLK_MIPIDSIPHY_CLK_EN_SHIFT)     /* 0x00000010 */
#define CRU_CLKGATE_CON16_PCLK_MIPICSIPHY_CLK_EN_SHIFT     (5U)
#define CRU_CLKGATE_CON16_PCLK_MIPICSIPHY_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON16_PCLK_MIPICSIPHY_CLK_EN_SHIFT)     /* 0x00000020 */
#define CRU_CLKGATE_CON16_PCLK_USB_GRF_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON16_PCLK_USB_GRF_EN_MASK             (0x1U << CRU_CLKGATE_CON16_PCLK_USB_GRF_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON16_PCLK_CPU_BOOST_CLK_EN_SHIFT      (7U)
#define CRU_CLKGATE_CON16_PCLK_CPU_BOOST_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON16_PCLK_CPU_BOOST_CLK_EN_SHIFT)      /* 0x00000080 */
#define CRU_CLKGATE_CON16_TESTCLK_CLK_EN_SHIFT             (15U)
#define CRU_CLKGATE_CON16_TESTCLK_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON16_TESTCLK_CLK_EN_SHIFT)             /* 0x00008000 */
/* CLKGATE_CON17 */
#define CRU_CLKGATE_CON17_OFFSET                           (0x244U)
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_PLL_CLK_EN_SHIFT     (0U)
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_RX_PLL_CLK_EN_SHIFT)     /* 0x00000001 */
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_DIVFRAC_CLK_EN_SHIFT (1U)
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_DIVFRAC_CLK_EN_MASK  (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_RX_DIVFRAC_CLK_EN_SHIFT) /* 0x00000002 */
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_CLK_EN_SHIFT         (2U)
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_RX_CLK_EN_SHIFT)         /* 0x00000004 */
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_OUT_MCLK_EN_SHIFT    (3U)
#define CRU_CLKGATE_CON17_CLK_I2S0_RX_OUT_MCLK_EN_MASK     (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_RX_OUT_MCLK_EN_SHIFT)    /* 0x00000008 */
#define CRU_CLKGATE_CON17_CLK_CORE_PVTM_CLK_EN_SHIFT       (4U)
#define CRU_CLKGATE_CON17_CLK_CORE_PVTM_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON17_CLK_CORE_PVTM_CLK_EN_SHIFT)       /* 0x00000010 */
#define CRU_CLKGATE_CON17_ACLK_CORE_PERF_CLK_EN_SHIFT      (5U)
#define CRU_CLKGATE_CON17_ACLK_CORE_PERF_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON17_ACLK_CORE_PERF_CLK_EN_SHIFT)      /* 0x00000020 */
#define CRU_CLKGATE_CON17_PCLK_CORE_GRF_CLK_EN_SHIFT       (6U)
#define CRU_CLKGATE_CON17_PCLK_CORE_GRF_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON17_PCLK_CORE_GRF_CLK_EN_SHIFT)       /* 0x00000040 */
#define CRU_CLKGATE_CON17_ACLK_GPU_PERF_CLK_EN_SHIFT       (8U)
#define CRU_CLKGATE_CON17_ACLK_GPU_PERF_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON17_ACLK_GPU_PERF_CLK_EN_SHIFT)       /* 0x00000100 */
#define CRU_CLKGATE_CON17_PCLK_GPU_GRF_CLK_EN_SHIFT        (9U)
#define CRU_CLKGATE_CON17_PCLK_GPU_GRF_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON17_PCLK_GPU_GRF_CLK_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE_CON17_ACLK_GPU_DIV_CLK_EN_SHIFT        (10U)
#define CRU_CLKGATE_CON17_ACLK_GPU_DIV_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON17_ACLK_GPU_DIV_CLK_EN_SHIFT)        /* 0x00000400 */
/* SSGTBL0_3 */
#define CRU_SSGTBL0_3_OFFSET                               (0x280U)
#define CRU_SSGTBL0_3_SSGTBL0_3_SHIFT                      (0U)
#define CRU_SSGTBL0_3_SSGTBL0_3_MASK                       (0xFFFFFFFFU << CRU_SSGTBL0_3_SSGTBL0_3_SHIFT)               /* 0xFFFFFFFF */
/* SSGTBL4_7 */
#define CRU_SSGTBL4_7_OFFSET                               (0x284U)
#define CRU_SSGTBL4_7_SSGTBL4_7_SHIFT                      (0U)
#define CRU_SSGTBL4_7_SSGTBL4_7_MASK                       (0xFFFFFFFFU << CRU_SSGTBL4_7_SSGTBL4_7_SHIFT)               /* 0xFFFFFFFF */
/* SSGTBL8_11 */
#define CRU_SSGTBL8_11_OFFSET                              (0x288U)
#define CRU_SSGTBL8_11_SSGTBL8_11_SHIFT                    (0U)
#define CRU_SSGTBL8_11_SSGTBL8_11_MASK                     (0xFFFFFFFFU << CRU_SSGTBL8_11_SSGTBL8_11_SHIFT)             /* 0xFFFFFFFF */
/* SSGTBL12_15 */
#define CRU_SSGTBL12_15_OFFSET                             (0x28CU)
#define CRU_SSGTBL12_15_SSGTBL12_15_SHIFT                  (0U)
#define CRU_SSGTBL12_15_SSGTBL12_15_MASK                   (0xFFFFFFFFU << CRU_SSGTBL12_15_SSGTBL12_15_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL16_19 */
#define CRU_SSGTBL16_19_OFFSET                             (0x290U)
#define CRU_SSGTBL16_19_SSGTBL16_19_SHIFT                  (0U)
#define CRU_SSGTBL16_19_SSGTBL16_19_MASK                   (0xFFFFFFFFU << CRU_SSGTBL16_19_SSGTBL16_19_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL20_23 */
#define CRU_SSGTBL20_23_OFFSET                             (0x294U)
#define CRU_SSGTBL20_23_SSGTBL20_23_SHIFT                  (0U)
#define CRU_SSGTBL20_23_SSGTBL20_23_MASK                   (0xFFFFFFFFU << CRU_SSGTBL20_23_SSGTBL20_23_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL24_27 */
#define CRU_SSGTBL24_27_OFFSET                             (0x298U)
#define CRU_SSGTBL24_27_SSGTBL24_27_SHIFT                  (0U)
#define CRU_SSGTBL24_27_SSGTBL24_27_MASK                   (0xFFFFFFFFU << CRU_SSGTBL24_27_SSGTBL24_27_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL28_31 */
#define CRU_SSGTBL28_31_OFFSET                             (0x29CU)
#define CRU_SSGTBL28_31_SSGTBL28_31_SHIFT                  (0U)
#define CRU_SSGTBL28_31_SSGTBL28_31_MASK                   (0xFFFFFFFFU << CRU_SSGTBL28_31_SSGTBL28_31_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL32_35 */
#define CRU_SSGTBL32_35_OFFSET                             (0x2A0U)
#define CRU_SSGTBL32_35_SSGTBL32_35_SHIFT                  (0U)
#define CRU_SSGTBL32_35_SSGTBL32_35_MASK                   (0xFFFFFFFFU << CRU_SSGTBL32_35_SSGTBL32_35_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL36_39 */
#define CRU_SSGTBL36_39_OFFSET                             (0x2A4U)
#define CRU_SSGTBL36_39_SSGTBL36_39_SHIFT                  (0U)
#define CRU_SSGTBL36_39_SSGTBL36_39_MASK                   (0xFFFFFFFFU << CRU_SSGTBL36_39_SSGTBL36_39_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL40_43 */
#define CRU_SSGTBL40_43_OFFSET                             (0x2A8U)
#define CRU_SSGTBL40_43_SSGTBL40_43_SHIFT                  (0U)
#define CRU_SSGTBL40_43_SSGTBL40_43_MASK                   (0xFFFFFFFFU << CRU_SSGTBL40_43_SSGTBL40_43_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL44_47 */
#define CRU_SSGTBL44_47_OFFSET                             (0x2ACU)
#define CRU_SSGTBL44_47_SSGTBL44_47_SHIFT                  (0U)
#define CRU_SSGTBL44_47_SSGTBL44_47_MASK                   (0xFFFFFFFFU << CRU_SSGTBL44_47_SSGTBL44_47_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL48_51 */
#define CRU_SSGTBL48_51_OFFSET                             (0x2B0U)
#define CRU_SSGTBL48_51_SSGTBL48_51_SHIFT                  (0U)
#define CRU_SSGTBL48_51_SSGTBL48_51_MASK                   (0xFFFFFFFFU << CRU_SSGTBL48_51_SSGTBL48_51_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL52_55 */
#define CRU_SSGTBL52_55_OFFSET                             (0x2B4U)
#define CRU_SSGTBL52_55_SSGTBL52_55_SHIFT                  (0U)
#define CRU_SSGTBL52_55_SSGTBL52_55_MASK                   (0xFFFFFFFFU << CRU_SSGTBL52_55_SSGTBL52_55_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL56_59 */
#define CRU_SSGTBL56_59_OFFSET                             (0x2B8U)
#define CRU_SSGTBL56_59_SSGTBL56_59_SHIFT                  (0U)
#define CRU_SSGTBL56_59_SSGTBL56_59_MASK                   (0xFFFFFFFFU << CRU_SSGTBL56_59_SSGTBL56_59_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL60_63 */
#define CRU_SSGTBL60_63_OFFSET                             (0x2BCU)
#define CRU_SSGTBL60_63_SSGTBL60_63_SHIFT                  (0U)
#define CRU_SSGTBL60_63_SSGTBL60_63_MASK                   (0xFFFFFFFFU << CRU_SSGTBL60_63_SSGTBL60_63_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL64_67 */
#define CRU_SSGTBL64_67_OFFSET                             (0x2C0U)
#define CRU_SSGTBL64_67_SSGTBL64_67_SHIFT                  (0U)
#define CRU_SSGTBL64_67_SSGTBL64_67_MASK                   (0xFFFFFFFFU << CRU_SSGTBL64_67_SSGTBL64_67_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL68_71 */
#define CRU_SSGTBL68_71_OFFSET                             (0x2C4U)
#define CRU_SSGTBL68_71_SSGTBL68_71_SHIFT                  (0U)
#define CRU_SSGTBL68_71_SSGTBL68_71_MASK                   (0xFFFFFFFFU << CRU_SSGTBL68_71_SSGTBL68_71_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL72_75 */
#define CRU_SSGTBL72_75_OFFSET                             (0x2C8U)
#define CRU_SSGTBL72_75_SSGTBL72_75_SHIFT                  (0U)
#define CRU_SSGTBL72_75_SSGTBL72_75_MASK                   (0xFFFFFFFFU << CRU_SSGTBL72_75_SSGTBL72_75_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL76_79 */
#define CRU_SSGTBL76_79_OFFSET                             (0x2CCU)
#define CRU_SSGTBL76_79_SSGTBL76_79_SHIFT                  (0U)
#define CRU_SSGTBL76_79_SSGTBL76_79_MASK                   (0xFFFFFFFFU << CRU_SSGTBL76_79_SSGTBL76_79_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL80_83 */
#define CRU_SSGTBL80_83_OFFSET                             (0x2D0U)
#define CRU_SSGTBL80_83_SSGTBL80_83_SHIFT                  (0U)
#define CRU_SSGTBL80_83_SSGTBL80_83_MASK                   (0xFFFFFFFFU << CRU_SSGTBL80_83_SSGTBL80_83_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL84_87 */
#define CRU_SSGTBL84_87_OFFSET                             (0x2D4U)
#define CRU_SSGTBL84_87_SSGTBL84_87_SHIFT                  (0U)
#define CRU_SSGTBL84_87_SSGTBL84_87_MASK                   (0xFFFFFFFFU << CRU_SSGTBL84_87_SSGTBL84_87_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL88_91 */
#define CRU_SSGTBL88_91_OFFSET                             (0x2D8U)
#define CRU_SSGTBL88_91_SSGTBL88_91_SHIFT                  (0U)
#define CRU_SSGTBL88_91_SSGTBL88_91_MASK                   (0xFFFFFFFFU << CRU_SSGTBL88_91_SSGTBL88_91_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL92_95 */
#define CRU_SSGTBL92_95_OFFSET                             (0x2DCU)
#define CRU_SSGTBL92_95_SSGTBL92_95_SHIFT                  (0U)
#define CRU_SSGTBL92_95_SSGTBL92_95_MASK                   (0xFFFFFFFFU << CRU_SSGTBL92_95_SSGTBL92_95_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL96_99 */
#define CRU_SSGTBL96_99_OFFSET                             (0x2E0U)
#define CRU_SSGTBL96_99_SSGTBL96_99_SHIFT                  (0U)
#define CRU_SSGTBL96_99_SSGTBL96_99_MASK                   (0xFFFFFFFFU << CRU_SSGTBL96_99_SSGTBL96_99_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL100_103 */
#define CRU_SSGTBL100_103_OFFSET                           (0x2E4U)
#define CRU_SSGTBL100_103_SSGTBL100_103_SHIFT              (0U)
#define CRU_SSGTBL100_103_SSGTBL100_103_MASK               (0xFFFFFFFFU << CRU_SSGTBL100_103_SSGTBL100_103_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL104_107 */
#define CRU_SSGTBL104_107_OFFSET                           (0x2E8U)
#define CRU_SSGTBL104_107_SSGTBL104_107_SHIFT              (0U)
#define CRU_SSGTBL104_107_SSGTBL104_107_MASK               (0xFFFFFFFFU << CRU_SSGTBL104_107_SSGTBL104_107_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL108_111 */
#define CRU_SSGTBL108_111_OFFSET                           (0x2ECU)
#define CRU_SSGTBL108_111_SSGTBL108_111_SHIFT              (0U)
#define CRU_SSGTBL108_111_SSGTBL108_111_MASK               (0xFFFFFFFFU << CRU_SSGTBL108_111_SSGTBL108_111_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL112_115 */
#define CRU_SSGTBL112_115_OFFSET                           (0x2F0U)
#define CRU_SSGTBL112_115_SSGTBL112_115_SHIFT              (0U)
#define CRU_SSGTBL112_115_SSGTBL112_115_MASK               (0xFFFFFFFFU << CRU_SSGTBL112_115_SSGTBL112_115_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL116_119 */
#define CRU_SSGTBL116_119_OFFSET                           (0x2F4U)
#define CRU_SSGTBL116_119_SSGTBL116_119_SHIFT              (0U)
#define CRU_SSGTBL116_119_SSGTBL116_119_MASK               (0xFFFFFFFFU << CRU_SSGTBL116_119_SSGTBL116_119_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL120_123 */
#define CRU_SSGTBL120_123_OFFSET                           (0x2F8U)
#define CRU_SSGTBL120_123_SSGTBL120_123_SHIFT              (0U)
#define CRU_SSGTBL120_123_SSGTBL120_123_MASK               (0xFFFFFFFFU << CRU_SSGTBL120_123_SSGTBL120_123_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL124_127 */
#define CRU_SSGTBL124_127_OFFSET                           (0x2FCU)
#define CRU_SSGTBL124_127_SSGTBL124_127_SHIFT              (0U)
#define CRU_SSGTBL124_127_SSGTBL124_127_MASK               (0xFFFFFFFFU << CRU_SSGTBL124_127_SSGTBL124_127_SHIFT)       /* 0xFFFFFFFF */
/* SOFTRST_CON0 */
#define CRU_SOFTRST_CON0_OFFSET                            (0x300U)
#define CRU_SOFTRST_CON0_COREPO0_SRSTN_REQ_SHIFT           (0U)
#define CRU_SOFTRST_CON0_COREPO0_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON0_COREPO0_SRSTN_REQ_SHIFT)           /* 0x00000001 */
#define CRU_SOFTRST_CON0_COREPO1_SRSTN_REQ_SHIFT           (1U)
#define CRU_SOFTRST_CON0_COREPO1_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON0_COREPO1_SRSTN_REQ_SHIFT)           /* 0x00000002 */
#define CRU_SOFTRST_CON0_COREPO2_SRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON0_COREPO2_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON0_COREPO2_SRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST_CON0_COREPO3_SRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST_CON0_COREPO3_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON0_COREPO3_SRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST_CON0_CORE0_SRSTN_REQ_SHIFT             (4U)
#define CRU_SOFTRST_CON0_CORE0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON0_CORE0_SRSTN_REQ_SHIFT)             /* 0x00000010 */
#define CRU_SOFTRST_CON0_CORE1_SRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST_CON0_CORE1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON0_CORE1_SRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST_CON0_CORE2_SRSTN_REQ_SHIFT             (6U)
#define CRU_SOFTRST_CON0_CORE2_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON0_CORE2_SRSTN_REQ_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST_CON0_CORE3_SRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON0_CORE3_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON0_CORE3_SRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON0_CORE0_DBG_SRSTN_REQ_SHIFT         (8U)
#define CRU_SOFTRST_CON0_CORE0_DBG_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE0_DBG_SRSTN_REQ_SHIFT)         /* 0x00000100 */
#define CRU_SOFTRST_CON0_CORE1_DBG_SRSTN_REQ_SHIFT         (9U)
#define CRU_SOFTRST_CON0_CORE1_DBG_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE1_DBG_SRSTN_REQ_SHIFT)         /* 0x00000200 */
#define CRU_SOFTRST_CON0_CORE2_DBG_SRSTN_REQ_SHIFT         (10U)
#define CRU_SOFTRST_CON0_CORE2_DBG_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE2_DBG_SRSTN_REQ_SHIFT)         /* 0x00000400 */
#define CRU_SOFTRST_CON0_CORE3_DBG_SRSTN_REQ_SHIFT         (11U)
#define CRU_SOFTRST_CON0_CORE3_DBG_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE3_DBG_SRSTN_REQ_SHIFT)         /* 0x00000800 */
#define CRU_SOFTRST_CON0_TOPDBG_SRSTN_REQ_SHIFT            (12U)
#define CRU_SOFTRST_CON0_TOPDBG_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON0_TOPDBG_SRSTN_REQ_SHIFT)            /* 0x00001000 */
#define CRU_SOFTRST_CON0_CORE_NOC_SRSTN_REQ_SHIFT          (13U)
#define CRU_SOFTRST_CON0_CORE_NOC_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON0_CORE_NOC_SRSTN_REQ_SHIFT)          /* 0x00002000 */
#define CRU_SOFTRST_CON0_STRC_SYS_ASRSTN_REQ_SHIFT         (14U)
#define CRU_SOFTRST_CON0_STRC_SYS_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_STRC_SYS_ASRSTN_REQ_SHIFT)         /* 0x00004000 */
#define CRU_SOFTRST_CON0_L2_SRSTN_REQ_SHIFT                (15U)
#define CRU_SOFTRST_CON0_L2_SRSTN_REQ_MASK                 (0x1U << CRU_SOFTRST_CON0_L2_SRSTN_REQ_SHIFT)                /* 0x00008000 */
/* SOFTRST_CON1 */
#define CRU_SOFTRST_CON1_OFFSET                            (0x304U)
#define CRU_SOFTRST_CON1_DAP_SRSTN_REQ_SHIFT               (0U)
#define CRU_SOFTRST_CON1_DAP_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON1_DAP_SRSTN_REQ_SHIFT)               /* 0x00000001 */
#define CRU_SOFTRST_CON1_CORE_PVTM_SRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST_CON1_CORE_PVTM_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON1_CORE_PVTM_SRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST_CON1_GPU_SRSTN_REQ_SHIFT               (2U)
#define CRU_SOFTRST_CON1_GPU_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON1_GPU_SRSTN_REQ_SHIFT)               /* 0x00000004 */
#define CRU_SOFTRST_CON1_GPU_NIU_SRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST_CON1_GPU_NIU_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON1_GPU_NIU_SRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST_CON1_UPCTL2_SRSTN_REQ_SHIFT            (4U)
#define CRU_SOFTRST_CON1_UPCTL2_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON1_UPCTL2_SRSTN_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST_CON1_UPCTL2_ASRSTN_REQ_SHIFT           (5U)
#define CRU_SOFTRST_CON1_UPCTL2_ASRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON1_UPCTL2_ASRSTN_REQ_SHIFT)           /* 0x00000020 */
#define CRU_SOFTRST_CON1_UPCTL2_PRSTN_REQ_SHIFT            (6U)
#define CRU_SOFTRST_CON1_UPCTL2_PRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON1_UPCTL2_PRSTN_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST_CON1_MSCH_SRSTN_REQ_SHIFT              (7U)
#define CRU_SOFTRST_CON1_MSCH_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON1_MSCH_SRSTN_REQ_SHIFT)              /* 0x00000080 */
#define CRU_SOFTRST_CON1_MSCH_PSRSTN_REQ_SHIFT             (8U)
#define CRU_SOFTRST_CON1_MSCH_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON1_MSCH_PSRSTN_REQ_SHIFT)             /* 0x00000100 */
#define CRU_SOFTRST_CON1_DDRMON_PSRSTN_REQ_SHIFT           (9U)
#define CRU_SOFTRST_CON1_DDRMON_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON1_DDRMON_PSRSTN_REQ_SHIFT)           /* 0x00000200 */
#define CRU_SOFTRST_CON1_DDRSTDBY_PSRSTN_REQ_SHIFT         (10U)
#define CRU_SOFTRST_CON1_DDRSTDBY_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON1_DDRSTDBY_PSRSTN_REQ_SHIFT)         /* 0x00000400 */
#define CRU_SOFTRST_CON1_DDRSTDBY_SRSTN_REQ_SHIFT          (11U)
#define CRU_SOFTRST_CON1_DDRSTDBY_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON1_DDRSTDBY_SRSTN_REQ_SHIFT)          /* 0x00000800 */
#define CRU_SOFTRST_CON1_DDRGRF_PSRSTN_REQ_SHIFT           (12U)
#define CRU_SOFTRST_CON1_DDRGRF_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON1_DDRGRF_PSRSTN_REQ_SHIFT)           /* 0x00001000 */
#define CRU_SOFTRST_CON1_AXI_SPLIT_ASRSTN_REQ_SHIFT        (13U)
#define CRU_SOFTRST_CON1_AXI_SPLIT_ASRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON1_AXI_SPLIT_ASRSTN_REQ_SHIFT)        /* 0x00002000 */
#define CRU_SOFTRST_CON1_AXI_CMD_BUFFER_ASRSTN_REQ_SHIFT   (14U)
#define CRU_SOFTRST_CON1_AXI_CMD_BUFFER_ASRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON1_AXI_CMD_BUFFER_ASRSTN_REQ_SHIFT)   /* 0x00004000 */
#define CRU_SOFTRST_CON1_AXI_CMD_BUFFERT_PSRSTN_REQ_SHIFT  (15U)
#define CRU_SOFTRST_CON1_AXI_CMD_BUFFERT_PSRSTN_REQ_MASK   (0x1U << CRU_SOFTRST_CON1_AXI_CMD_BUFFERT_PSRSTN_REQ_SHIFT)  /* 0x00008000 */
/* SOFTRST_CON2 */
#define CRU_SOFTRST_CON2_OFFSET                            (0x308U)
#define CRU_SOFTRST_CON2_DDRPHY_SRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST_CON2_DDRPHY_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON2_DDRPHY_SRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST_CON2_DDRPHYDIV_SRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST_CON2_DDRPHYDIV_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON2_DDRPHYDIV_SRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST_CON2_DDRPHY_PSRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON2_DDRPHY_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON2_DDRPHY_PSRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST_CON2_VPU_ASRSTN_REQ_SHIFT              (4U)
#define CRU_SOFTRST_CON2_VPU_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_VPU_ASRSTN_REQ_SHIFT)              /* 0x00000010 */
#define CRU_SOFTRST_CON2_VPU_NIU_ASRSTN_REQ_SHIFT          (5U)
#define CRU_SOFTRST_CON2_VPU_NIU_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_VPU_NIU_ASRSTN_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST_CON2_VPU_HSRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST_CON2_VPU_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_VPU_HSRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST_CON2_VPU_NIU_HSRSTN_REQ_SHIFT          (7U)
#define CRU_SOFTRST_CON2_VPU_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_VPU_NIU_HSRSTN_REQ_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST_CON2_VI_NIU_ASRSTN_REQ_SHIFT           (8U)
#define CRU_SOFTRST_CON2_VI_NIU_ASRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON2_VI_NIU_ASRSTN_REQ_SHIFT)           /* 0x00000100 */
#define CRU_SOFTRST_CON2_VI_NIU_HSRSTN_REQ_SHIFT           (9U)
#define CRU_SOFTRST_CON2_VI_NIU_HSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON2_VI_NIU_HSRSTN_REQ_SHIFT)           /* 0x00000200 */
#define CRU_SOFTRST_CON2_ISP_HSRSTN_REQ_SHIFT              (10U)
#define CRU_SOFTRST_CON2_ISP_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_ISP_HSRSTN_REQ_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST_CON2_ISP_SRSTN_REQ_SHIFT               (11U)
#define CRU_SOFTRST_CON2_ISP_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON2_ISP_SRSTN_REQ_SHIFT)               /* 0x00000800 */
#define CRU_SOFTRST_CON2_CIF_ASRSTN_REQ_SHIFT              (12U)
#define CRU_SOFTRST_CON2_CIF_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_CIF_ASRSTN_REQ_SHIFT)              /* 0x00001000 */
#define CRU_SOFTRST_CON2_CIF_HSRSTN_REQ_SHIFT              (13U)
#define CRU_SOFTRST_CON2_CIF_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_CIF_HSRSTN_REQ_SHIFT)              /* 0x00002000 */
#define CRU_SOFTRST_CON2_CIF_PCLKIN_SRSTN_REQ_SHIFT        (14U)
#define CRU_SOFTRST_CON2_CIF_PCLKIN_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON2_CIF_PCLKIN_SRSTN_REQ_SHIFT)        /* 0x00004000 */
#define CRU_SOFTRST_CON2_MIPICSIPHY_PSRSTN_REQ_SHIFT       (15U)
#define CRU_SOFTRST_CON2_MIPICSIPHY_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON2_MIPICSIPHY_PSRSTN_REQ_SHIFT)       /* 0x00008000 */
/* SOFTRST_CON3 */
#define CRU_SOFTRST_CON3_OFFSET                            (0x30CU)
#define CRU_SOFTRST_CON3_VO_NIU_ASRSTN_REQ_SHIFT           (0U)
#define CRU_SOFTRST_CON3_VO_NIU_ASRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON3_VO_NIU_ASRSTN_REQ_SHIFT)           /* 0x00000001 */
#define CRU_SOFTRST_CON3_VO_NIU_HSRSTN_REQ_SHIFT           (1U)
#define CRU_SOFTRST_CON3_VO_NIU_HSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON3_VO_NIU_HSRSTN_REQ_SHIFT)           /* 0x00000002 */
#define CRU_SOFTRST_CON3_VO_NIU_PSRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON3_VO_NIU_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON3_VO_NIU_PSRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST_CON3_VOPB_ASRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON3_VOPB_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON3_VOPB_ASRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON3_VOPB_HSRSTN_REQ_SHIFT             (4U)
#define CRU_SOFTRST_CON3_VOPB_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON3_VOPB_HSRSTN_REQ_SHIFT)             /* 0x00000010 */
#define CRU_SOFTRST_CON3_VOPB_SRSTN_REQ_SHIFT              (5U)
#define CRU_SOFTRST_CON3_VOPB_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON3_VOPB_SRSTN_REQ_SHIFT)              /* 0x00000020 */
#define CRU_SOFTRST_CON3_PWM_VOPB_SRSTN_REQ_SHIFT          (6U)
#define CRU_SOFTRST_CON3_PWM_VOPB_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON3_PWM_VOPB_SRSTN_REQ_SHIFT)          /* 0x00000040 */
#define CRU_SOFTRST_CON3_VOPL_ASRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON3_VOPL_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON3_VOPL_ASRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON3_VOPL_HSRSTN_REQ_SHIFT             (8U)
#define CRU_SOFTRST_CON3_VOPL_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON3_VOPL_HSRSTN_REQ_SHIFT)             /* 0x00000100 */
#define CRU_SOFTRST_CON3_VOPL_SRSTN_REQ_SHIFT              (9U)
#define CRU_SOFTRST_CON3_VOPL_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON3_VOPL_SRSTN_REQ_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST_CON3_RGA_ASRSTN_REQ_SHIFT              (10U)
#define CRU_SOFTRST_CON3_RGA_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON3_RGA_ASRSTN_REQ_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST_CON3_RGA_HSRSTN_REQ_SHIFT              (11U)
#define CRU_SOFTRST_CON3_RGA_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON3_RGA_HSRSTN_REQ_SHIFT)              /* 0x00000800 */
#define CRU_SOFTRST_CON3_RGA_SRSTN_REQ_SHIFT               (12U)
#define CRU_SOFTRST_CON3_RGA_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON3_RGA_SRSTN_REQ_SHIFT)               /* 0x00001000 */
#define CRU_SOFTRST_CON3_MIPIDSI_HOST_PSRSTN_REQ_SHIFT     (13U)
#define CRU_SOFTRST_CON3_MIPIDSI_HOST_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON3_MIPIDSI_HOST_PSRSTN_REQ_SHIFT)     /* 0x00002000 */
#define CRU_SOFTRST_CON3_MIPIDSIPHY_PSRSTN_REQ_SHIFT       (14U)
#define CRU_SOFTRST_CON3_MIPIDSIPHY_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON3_MIPIDSIPHY_PSRSTN_REQ_SHIFT)       /* 0x00004000 */
#define CRU_SOFTRST_CON3_VPU_CORE_SRSTN_REQ_SHIFT          (15U)
#define CRU_SOFTRST_CON3_VPU_CORE_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON3_VPU_CORE_SRSTN_REQ_SHIFT)          /* 0x00008000 */
/* SOFTRST_CON4 */
#define CRU_SOFTRST_CON4_OFFSET                            (0x310U)
#define CRU_SOFTRST_CON4_PERI_NIU_ASRSTN_REQ_SHIFT         (0U)
#define CRU_SOFTRST_CON4_PERI_NIU_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON4_PERI_NIU_ASRSTN_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST_CON4_USB_NIU_HSRSTN_REQ_SHIFT          (1U)
#define CRU_SOFTRST_CON4_USB_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON4_USB_NIU_HSRSTN_REQ_SHIFT)          /* 0x00000002 */
#define CRU_SOFTRST_CON4_USB2OTG_HSRSTN_REQ_SHIFT          (2U)
#define CRU_SOFTRST_CON4_USB2OTG_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON4_USB2OTG_HSRSTN_REQ_SHIFT)          /* 0x00000004 */
#define CRU_SOFTRST_CON4_USB2OTG_SRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST_CON4_USB2OTG_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON4_USB2OTG_SRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST_CON4_USB2OTG_ADP_SRSTN_REQ_SHIFT       (4U)
#define CRU_SOFTRST_CON4_USB2OTG_ADP_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON4_USB2OTG_ADP_SRSTN_REQ_SHIFT)       /* 0x00000010 */
#define CRU_SOFTRST_CON4_USB2HOST_HSRSTN_REQ_SHIFT         (5U)
#define CRU_SOFTRST_CON4_USB2HOST_HSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON4_USB2HOST_HSRSTN_REQ_SHIFT)         /* 0x00000020 */
#define CRU_SOFTRST_CON4_USB2HOST_ARB_HSRSTN_REQ_SHIFT     (6U)
#define CRU_SOFTRST_CON4_USB2HOST_ARB_HSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON4_USB2HOST_ARB_HSRSTN_REQ_SHIFT)     /* 0x00000040 */
#define CRU_SOFTRST_CON4_USB2HOST_AUX_HSRSTN_REQ_SHIFT     (7U)
#define CRU_SOFTRST_CON4_USB2HOST_AUX_HSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON4_USB2HOST_AUX_HSRSTN_REQ_SHIFT)     /* 0x00000080 */
#define CRU_SOFTRST_CON4_USB2HOST_EHCI_SRSTN_REQ_SHIFT     (8U)
#define CRU_SOFTRST_CON4_USB2HOST_EHCI_SRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON4_USB2HOST_EHCI_SRSTN_REQ_SHIFT)     /* 0x00000100 */
#define CRU_SOFTRST_CON4_USB2HOST_SRSTN_REQ_SHIFT          (9U)
#define CRU_SOFTRST_CON4_USB2HOST_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON4_USB2HOST_SRSTN_REQ_SHIFT)          /* 0x00000200 */
#define CRU_SOFTRST_CON4_USBPHYPOR_SRSTN_REQ_SHIFT         (10U)
#define CRU_SOFTRST_CON4_USBPHYPOR_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON4_USBPHYPOR_SRSTN_REQ_SHIFT)         /* 0x00000400 */
#define CRU_SOFTRST_CON4_USBPHY_OTG_PORT_SRSTN_REQ_SHIFT   (11U)
#define CRU_SOFTRST_CON4_USBPHY_OTG_PORT_SRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON4_USBPHY_OTG_PORT_SRSTN_REQ_SHIFT)   /* 0x00000800 */
#define CRU_SOFTRST_CON4_USBPHY_HOST_PORT_SRSTN_REQ_SHIFT  (12U)
#define CRU_SOFTRST_CON4_USBPHY_HOST_PORT_SRSTN_REQ_MASK   (0x1U << CRU_SOFTRST_CON4_USBPHY_HOST_PORT_SRSTN_REQ_SHIFT)  /* 0x00001000 */
#define CRU_SOFTRST_CON4_USBPHY_GRF_PSRSTN_REQ_SHIFT       (13U)
#define CRU_SOFTRST_CON4_USBPHY_GRF_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON4_USBPHY_GRF_PSRSTN_REQ_SHIFT)       /* 0x00002000 */
#define CRU_SOFTRST_CON4_CPU_BOOST_PSRSTN_REQ_SHIFT        (14U)
#define CRU_SOFTRST_CON4_CPU_BOOST_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON4_CPU_BOOST_PSRSTN_REQ_SHIFT)        /* 0x00004000 */
#define CRU_SOFTRST_CON4_CPU_BOOST_SRSTN_REQ_SHIFT         (15U)
#define CRU_SOFTRST_CON4_CPU_BOOST_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON4_CPU_BOOST_SRSTN_REQ_SHIFT)         /* 0x00008000 */
/* SOFTRST_CON5 */
#define CRU_SOFTRST_CON5_OFFSET                            (0x314U)
#define CRU_SOFTRST_CON5_PDMMC_NAND_NIU_HSRSTN_REQ_SHIFT   (0U)
#define CRU_SOFTRST_CON5_PDMMC_NAND_NIU_HSRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON5_PDMMC_NAND_NIU_HSRSTN_REQ_SHIFT)   /* 0x00000001 */
#define CRU_SOFTRST_CON5_SDIO_HSRSTN_REQ_SHIFT             (1U)
#define CRU_SOFTRST_CON5_SDIO_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON5_SDIO_HSRSTN_REQ_SHIFT)             /* 0x00000002 */
#define CRU_SOFTRST_CON5_EMMC_HSRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST_CON5_EMMC_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON5_EMMC_HSRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON5_SFC_HSRSTN_REQ_SHIFT              (3U)
#define CRU_SOFTRST_CON5_SFC_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON5_SFC_HSRSTN_REQ_SHIFT)              /* 0x00000008 */
#define CRU_SOFTRST_CON5_SFC_SRSTN_REQ_SHIFT               (4U)
#define CRU_SOFTRST_CON5_SFC_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON5_SFC_SRSTN_REQ_SHIFT)               /* 0x00000010 */
#define CRU_SOFTRST_CON5_PDSDCARD_NIU_HSRSTN_REQ_SHIFT     (5U)
#define CRU_SOFTRST_CON5_PDSDCARD_NIU_HSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON5_PDSDCARD_NIU_HSRSTN_REQ_SHIFT)     /* 0x00000020 */
#define CRU_SOFTRST_CON5_SDMMC_HSRSTN_REQ_SHIFT            (6U)
#define CRU_SOFTRST_CON5_SDMMC_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON5_SDMMC_HSRSTN_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST_CON5_NANDC_HRSTN_REQ_SHIFT             (9U)
#define CRU_SOFTRST_CON5_NANDC_HRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON5_NANDC_HRSTN_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST_CON5_NANDC_SRSTN_REQ_SHIFT             (10U)
#define CRU_SOFTRST_CON5_NANDC_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON5_NANDC_SRSTN_REQ_SHIFT)             /* 0x00000400 */
#define CRU_SOFTRST_CON5_GMAC_NIU_ASRSTN_REQ_SHIFT         (12U)
#define CRU_SOFTRST_CON5_GMAC_NIU_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON5_GMAC_NIU_ASRSTN_REQ_SHIFT)         /* 0x00001000 */
#define CRU_SOFTRST_CON5_GMAC_NIU_PSRSTN_REQ_SHIFT         (13U)
#define CRU_SOFTRST_CON5_GMAC_NIU_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON5_GMAC_NIU_PSRSTN_REQ_SHIFT)         /* 0x00002000 */
#define CRU_SOFTRST_CON5_GMAC_ASRSTN_REQ_SHIFT             (14U)
#define CRU_SOFTRST_CON5_GMAC_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON5_GMAC_ASRSTN_REQ_SHIFT)             /* 0x00004000 */
/* SOFTRST_CON6 */
#define CRU_SOFTRST_CON6_OFFSET                            (0x318U)
#define CRU_SOFTRST_CON6_PMU_NIU_PSRSTN_REQ_SHIFT          (0U)
#define CRU_SOFTRST_CON6_PMU_NIU_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_PMU_NIU_PSRSTN_REQ_SHIFT)          /* 0x00000001 */
#define CRU_SOFTRST_CON6_PMU_SGRF_PSRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST_CON6_PMU_SGRF_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON6_PMU_SGRF_PSRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST_CON6_PMU_GRF_PSRSTN_REQ_SHIFT          (2U)
#define CRU_SOFTRST_CON6_PMU_GRF_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_PMU_GRF_PSRSTN_REQ_SHIFT)          /* 0x00000004 */
#define CRU_SOFTRST_CON6_PMU_PMU_SRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST_CON6_PMU_PMU_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON6_PMU_PMU_SRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST_CON6_PMU_MEM_PSRSTN_REQ_SHIFT          (4U)
#define CRU_SOFTRST_CON6_PMU_MEM_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_PMU_MEM_PSRSTN_REQ_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST_CON6_PMU_GPIO0_PSRSTN_REQ_SHIFT        (5U)
#define CRU_SOFTRST_CON6_PMU_GPIO0_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON6_PMU_GPIO0_PSRSTN_REQ_SHIFT)        /* 0x00000020 */
#define CRU_SOFTRST_CON6_PMU_UART0_PSRSTN_REQ_SHIFT        (6U)
#define CRU_SOFTRST_CON6_PMU_UART0_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON6_PMU_UART0_PSRSTN_REQ_SHIFT)        /* 0x00000040 */
#define CRU_SOFTRST_CON6_PMU_CRU_PSRSTN_REQ_SHIFT          (7U)
#define CRU_SOFTRST_CON6_PMU_CRU_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_PMU_CRU_PSRSTN_REQ_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST_CON6_PMU_PVTM_SRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST_CON6_PMU_PVTM_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_PMU_PVTM_SRSTN_REQ_SHIFT)          /* 0x00000100 */
#define CRU_SOFTRST_CON6_PMU_UART_SRSTN_REQ_SHIFT          (9U)
#define CRU_SOFTRST_CON6_PMU_UART_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_PMU_UART_SRSTN_REQ_SHIFT)          /* 0x00000200 */
#define CRU_SOFTRST_CON6_PMU_NIU_HRSTN_REQ_SHIFT           (10U)
#define CRU_SOFTRST_CON6_PMU_NIU_HRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON6_PMU_NIU_HRSTN_REQ_SHIFT)           /* 0x00000400 */
#define CRU_SOFTRST_CON6_PMU_DDR_FAIL_SAVE_SRSTN_REQ_SHIFT (11U)
#define CRU_SOFTRST_CON6_PMU_DDR_FAIL_SAVE_SRSTN_REQ_MASK  (0x1U << CRU_SOFTRST_CON6_PMU_DDR_FAIL_SAVE_SRSTN_REQ_SHIFT) /* 0x00000800 */
#define CRU_SOFTRST_CON6_CORE_PERF_ASRSTN_REQ_SHIFT        (12U)
#define CRU_SOFTRST_CON6_CORE_PERF_ASRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON6_CORE_PERF_ASRSTN_REQ_SHIFT)        /* 0x00001000 */
#define CRU_SOFTRST_CON6_CORE_GRF_PSRSTN_REQ_SHIFT         (13U)
#define CRU_SOFTRST_CON6_CORE_GRF_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON6_CORE_GRF_PSRSTN_REQ_SHIFT)         /* 0x00002000 */
#define CRU_SOFTRST_CON6_GPU_PERF_ASRSTN_REQ_SHIFT         (14U)
#define CRU_SOFTRST_CON6_GPU_PERF_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON6_GPU_PERF_ASRSTN_REQ_SHIFT)         /* 0x00004000 */
#define CRU_SOFTRST_CON6_GPU_GRF_PSRSTN_REQ_SHIFT          (15U)
#define CRU_SOFTRST_CON6_GPU_GRF_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON6_GPU_GRF_PSRSTN_REQ_SHIFT)          /* 0x00008000 */
/* SOFTRST_CON7 */
#define CRU_SOFTRST_CON7_OFFSET                            (0x31CU)
#define CRU_SOFTRST_CON7_CRYPTO_NIU_ASRSTN_REQ_SHIFT       (0U)
#define CRU_SOFTRST_CON7_CRYPTO_NIU_ASRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON7_CRYPTO_NIU_ASRSTN_REQ_SHIFT)       /* 0x00000001 */
#define CRU_SOFTRST_CON7_CRYPTO_NIU_HSRSTN_REQ_SHIFT       (1U)
#define CRU_SOFTRST_CON7_CRYPTO_NIU_HSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON7_CRYPTO_NIU_HSRSTN_REQ_SHIFT)       /* 0x00000002 */
#define CRU_SOFTRST_CON7_CRYPTO_ASRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON7_CRYPTO_ASRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON7_CRYPTO_ASRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST_CON7_CRYPTO_HSRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST_CON7_CRYPTO_HSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON7_CRYPTO_HSRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST_CON7_CRYPTO_SRSTN_REQ_SHIFT            (4U)
#define CRU_SOFTRST_CON7_CRYPTO_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON7_CRYPTO_SRSTN_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST_CON7_CRYPTO_APK_SRSTN_REQ_SHIFT        (5U)
#define CRU_SOFTRST_CON7_CRYPTO_APK_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON7_CRYPTO_APK_SRSTN_REQ_SHIFT)        /* 0x00000020 */
#define CRU_SOFTRST_CON7_BUS_NIU_HSRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST_CON7_BUS_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON7_BUS_NIU_HSRSTN_REQ_SHIFT)          /* 0x00000100 */
#define CRU_SOFTRST_CON7_BUS_NIU_PSRST_REQ_SHIFT           (9U)
#define CRU_SOFTRST_CON7_BUS_NIU_PSRST_REQ_MASK            (0x1U << CRU_SOFTRST_CON7_BUS_NIU_PSRST_REQ_SHIFT)           /* 0x00000200 */
#define CRU_SOFTRST_CON7_BUS_TOP_NIU_PSRST_REQ_SHIFT       (10U)
#define CRU_SOFTRST_CON7_BUS_TOP_NIU_PSRST_REQ_MASK        (0x1U << CRU_SOFTRST_CON7_BUS_TOP_NIU_PSRST_REQ_SHIFT)       /* 0x00000400 */
#define CRU_SOFTRST_CON7_INTMEM_ASRST_REQ_SHIFT            (11U)
#define CRU_SOFTRST_CON7_INTMEM_ASRST_REQ_MASK             (0x1U << CRU_SOFTRST_CON7_INTMEM_ASRST_REQ_SHIFT)            /* 0x00000800 */
#define CRU_SOFTRST_CON7_GIC_ASRST_REQ_SHIFT               (12U)
#define CRU_SOFTRST_CON7_GIC_ASRST_REQ_MASK                (0x1U << CRU_SOFTRST_CON7_GIC_ASRST_REQ_SHIFT)               /* 0x00001000 */
#define CRU_SOFTRST_CON7_ROM_HSRSTN_REQ_SHIFT              (14U)
#define CRU_SOFTRST_CON7_ROM_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_ROM_HSRSTN_REQ_SHIFT)              /* 0x00004000 */
#define CRU_SOFTRST_CON7_DCF_ASRSTN_REQ_SHIFT              (15U)
#define CRU_SOFTRST_CON7_DCF_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_DCF_ASRSTN_REQ_SHIFT)              /* 0x00008000 */
/* SOFTRST_CON8 */
#define CRU_SOFTRST_CON8_OFFSET                            (0x320U)
#define CRU_SOFTRST_CON8_DCF_PSRSTN_REQ_SHIFT              (0U)
#define CRU_SOFTRST_CON8_DCF_PSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON8_DCF_PSRSTN_REQ_SHIFT)              /* 0x00000001 */
#define CRU_SOFTRST_CON8_PDM_HSRSTN_REQ_SHIFT              (1U)
#define CRU_SOFTRST_CON8_PDM_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON8_PDM_HSRSTN_REQ_SHIFT)              /* 0x00000002 */
#define CRU_SOFTRST_CON8_PDM_SRSTN_REQ_SHIFT               (2U)
#define CRU_SOFTRST_CON8_PDM_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON8_PDM_SRSTN_REQ_SHIFT)               /* 0x00000004 */
#define CRU_SOFTRST_CON8_I2S0_HSRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON8_I2S0_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_I2S0_HSRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON8_I2S0_TX_SRSTN_REQ_SHIFT           (4U)
#define CRU_SOFTRST_CON8_I2S0_TX_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON8_I2S0_TX_SRSTN_REQ_SHIFT)           /* 0x00000010 */
#define CRU_SOFTRST_CON8_I2S1_HSRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST_CON8_I2S1_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_I2S1_HSRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST_CON8_I2S1_SRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST_CON8_I2S1_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON8_I2S1_SRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST_CON8_I2S2_HSRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON8_I2S2_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_I2S2_HSRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON8_I2S2_SRSTN_REQ_SHIFT              (8U)
#define CRU_SOFTRST_CON8_I2S2_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON8_I2S2_SRSTN_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST_CON8_UART1_PSRSTN_REQ_SHIFT            (9U)
#define CRU_SOFTRST_CON8_UART1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON8_UART1_PSRSTN_REQ_SHIFT)            /* 0x00000200 */
#define CRU_SOFTRST_CON8_UART1_SRSTN_REQ_SHIFT             (10U)
#define CRU_SOFTRST_CON8_UART1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_UART1_SRSTN_REQ_SHIFT)             /* 0x00000400 */
#define CRU_SOFTRST_CON8_UART2_PSRSTN_REQ_SHIFT            (11U)
#define CRU_SOFTRST_CON8_UART2_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON8_UART2_PSRSTN_REQ_SHIFT)            /* 0x00000800 */
#define CRU_SOFTRST_CON8_UART2_SRSTN_REQ_SHIFT             (12U)
#define CRU_SOFTRST_CON8_UART2_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_UART2_SRSTN_REQ_SHIFT)             /* 0x00001000 */
#define CRU_SOFTRST_CON8_UART3_PSRSTN_REQ_SHIFT            (13U)
#define CRU_SOFTRST_CON8_UART3_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON8_UART3_PSRSTN_REQ_SHIFT)            /* 0x00002000 */
#define CRU_SOFTRST_CON8_UART3_SRSTN_REQ_SHIFT             (14U)
#define CRU_SOFTRST_CON8_UART3_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_UART3_SRSTN_REQ_SHIFT)             /* 0x00004000 */
#define CRU_SOFTRST_CON8_UART4_PSRSTN_REQ_SHIFT            (15U)
#define CRU_SOFTRST_CON8_UART4_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON8_UART4_PSRSTN_REQ_SHIFT)            /* 0x00008000 */
/* SOFTRST_CON9 */
#define CRU_SOFTRST_CON9_OFFSET                            (0x324U)
#define CRU_SOFTRST_CON9_UART4_SRSTN_REQ_SHIFT             (0U)
#define CRU_SOFTRST_CON9_UART4_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_UART4_SRSTN_REQ_SHIFT)             /* 0x00000001 */
#define CRU_SOFTRST_CON9_UART5_PSRSTN_REQ_SHIFT            (1U)
#define CRU_SOFTRST_CON9_UART5_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON9_UART5_PSRSTN_REQ_SHIFT)            /* 0x00000002 */
#define CRU_SOFTRST_CON9_UART5_SRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST_CON9_UART5_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_UART5_SRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON9_I2C0_PSRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON9_I2C0_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_I2C0_PSRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON9_I2C0_SRSTN_REQ_SHIFT              (4U)
#define CRU_SOFTRST_CON9_I2C0_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON9_I2C0_SRSTN_REQ_SHIFT)              /* 0x00000010 */
#define CRU_SOFTRST_CON9_I2C1_PSRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST_CON9_I2C1_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_I2C1_PSRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST_CON9_I2C1_SRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST_CON9_I2C1_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON9_I2C1_SRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST_CON9_I2C2_PSRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON9_I2C2_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_I2C2_PSRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON9_I2C2_SRSTN_REQ_SHIFT              (8U)
#define CRU_SOFTRST_CON9_I2C2_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON9_I2C2_SRSTN_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST_CON9_I2C3_PSRSTN_REQ_SHIFT             (9U)
#define CRU_SOFTRST_CON9_I2C3_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_I2C3_PSRSTN_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST_CON9_I2C3_SRSTN_REQ_SHIFT              (10U)
#define CRU_SOFTRST_CON9_I2C3_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON9_I2C3_SRSTN_REQ_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST_CON9_PWM0_PSRSTN_REQ_SHIFT             (13U)
#define CRU_SOFTRST_CON9_PWM0_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_PWM0_PSRSTN_REQ_SHIFT)             /* 0x00002000 */
#define CRU_SOFTRST_CON9_PWM0_SRSTN_REQ_SHIFT              (14U)
#define CRU_SOFTRST_CON9_PWM0_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON9_PWM0_SRSTN_REQ_SHIFT)              /* 0x00004000 */
#define CRU_SOFTRST_CON9_PWM1_PSRSTN_REQ_SHIFT             (15U)
#define CRU_SOFTRST_CON9_PWM1_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON9_PWM1_PSRSTN_REQ_SHIFT)             /* 0x00008000 */
/* SOFTRST_CON10 */
#define CRU_SOFTRST_CON10_OFFSET                           (0x328U)
#define CRU_SOFTRST_CON10_PWM1_SRSTN_REQ_SHIFT             (0U)
#define CRU_SOFTRST_CON10_PWM1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON10_PWM1_SRSTN_REQ_SHIFT)             /* 0x00000001 */
#define CRU_SOFTRST_CON10_SPI0_PSRSTN_REQ_SHIFT            (1U)
#define CRU_SOFTRST_CON10_SPI0_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON10_SPI0_PSRSTN_REQ_SHIFT)            /* 0x00000002 */
#define CRU_SOFTRST_CON10_SPI0_SRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST_CON10_SPI0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON10_SPI0_SRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON10_SPI1_PSRSTN_REQ_SHIFT            (3U)
#define CRU_SOFTRST_CON10_SPI1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON10_SPI1_PSRSTN_REQ_SHIFT)            /* 0x00000008 */
#define CRU_SOFTRST_CON10_SPI1_SRSTN_REQ_SHIFT             (4U)
#define CRU_SOFTRST_CON10_SPI1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON10_SPI1_SRSTN_REQ_SHIFT)             /* 0x00000010 */
#define CRU_SOFTRST_CON10_SARADC_PSRSTN_REQ_SHIFT          (5U)
#define CRU_SOFTRST_CON10_SARADC_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON10_SARADC_PSRSTN_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST_CON10_SARADC_SRSTN_REQ_SHIFT           (6U)
#define CRU_SOFTRST_CON10_SARADC_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_SARADC_SRSTN_REQ_SHIFT)           /* 0x00000040 */
#define CRU_SOFTRST_CON10_TSADC_PSRSTN_REQ_SHIFT           (7U)
#define CRU_SOFTRST_CON10_TSADC_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TSADC_PSRSTN_REQ_SHIFT)           /* 0x00000080 */
#define CRU_SOFTRST_CON10_TSADC_SRSTN_REQ_SHIFT            (8U)
#define CRU_SOFTRST_CON10_TSADC_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON10_TSADC_SRSTN_REQ_SHIFT)            /* 0x00000100 */
#define CRU_SOFTRST_CON10_TIMER_PSRSTN_REQ_SHIFT           (9U)
#define CRU_SOFTRST_CON10_TIMER_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER_PSRSTN_REQ_SHIFT)           /* 0x00000200 */
#define CRU_SOFTRST_CON10_TIMER0_SRSTN_REQ_SHIFT           (10U)
#define CRU_SOFTRST_CON10_TIMER0_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER0_SRSTN_REQ_SHIFT)           /* 0x00000400 */
#define CRU_SOFTRST_CON10_TIMER1_SRSTN_REQ_SHIFT           (11U)
#define CRU_SOFTRST_CON10_TIMER1_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER1_SRSTN_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST_CON10_TIMER2_SRSTN_REQ_SHIFT           (12U)
#define CRU_SOFTRST_CON10_TIMER2_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER2_SRSTN_REQ_SHIFT)           /* 0x00001000 */
#define CRU_SOFTRST_CON10_TIMER3_SRSTN_REQ_SHIFT           (13U)
#define CRU_SOFTRST_CON10_TIMER3_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER3_SRSTN_REQ_SHIFT)           /* 0x00002000 */
#define CRU_SOFTRST_CON10_TIMER4_SRSTN_REQ_SHIFT           (14U)
#define CRU_SOFTRST_CON10_TIMER4_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER4_SRSTN_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST_CON10_TIMER5_SRSTN_REQ_SHIFT           (15U)
#define CRU_SOFTRST_CON10_TIMER5_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON10_TIMER5_SRSTN_REQ_SHIFT)           /* 0x00008000 */
/* SOFTRST_CON11 */
#define CRU_SOFTRST_CON11_OFFSET                           (0x32CU)
#define CRU_SOFTRST_CON11_OTP_NS_PSRSTN_REQ_SHIFT          (0U)
#define CRU_SOFTRST_CON11_OTP_NS_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON11_OTP_NS_PSRSTN_REQ_SHIFT)          /* 0x00000001 */
#define CRU_SOFTRST_CON11_OTP_NS_SBPI_SRSTN_REQ_SHIFT      (1U)
#define CRU_SOFTRST_CON11_OTP_NS_SBPI_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON11_OTP_NS_SBPI_SRSTN_REQ_SHIFT)      /* 0x00000002 */
#define CRU_SOFTRST_CON11_OTP_NS_USR_SRSTN_REQ_SHIFT       (2U)
#define CRU_SOFTRST_CON11_OTP_NS_USR_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON11_OTP_NS_USR_SRSTN_REQ_SHIFT)       /* 0x00000004 */
#define CRU_SOFTRST_CON11_OTP_PHY_PSRSTN_REQ_SHIFT         (3U)
#define CRU_SOFTRST_CON11_OTP_PHY_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON11_OTP_PHY_PSRSTN_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST_CON11_OTP_PHY_SRSTN_REQ_SHIFT          (4U)
#define CRU_SOFTRST_CON11_OTP_PHY_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON11_OTP_PHY_SRSTN_REQ_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST_CON11_WDT_NS_PSRSTN_REQ_SHIFT          (5U)
#define CRU_SOFTRST_CON11_WDT_NS_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON11_WDT_NS_PSRSTN_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST_CON11_GPIO1_PSRSTN_REQ_SHIFT           (6U)
#define CRU_SOFTRST_CON11_GPIO1_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON11_GPIO1_PSRSTN_REQ_SHIFT)           /* 0x00000040 */
#define CRU_SOFTRST_CON11_GPIO2_PSRSTN_REQ_SHIFT           (7U)
#define CRU_SOFTRST_CON11_GPIO2_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON11_GPIO2_PSRSTN_REQ_SHIFT)           /* 0x00000080 */
#define CRU_SOFTRST_CON11_GPIO3_PSRSTN_REQ_SHIFT           (8U)
#define CRU_SOFTRST_CON11_GPIO3_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON11_GPIO3_PSRSTN_REQ_SHIFT)           /* 0x00000100 */
#define CRU_SOFTRST_CON11_SGRF_PSRSTN_REQ_SHIFT            (9U)
#define CRU_SOFTRST_CON11_SGRF_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON11_SGRF_PSRSTN_REQ_SHIFT)            /* 0x00000200 */
#define CRU_SOFTRST_CON11_GRF_PSRSTN_REQ_SHIFT             (10U)
#define CRU_SOFTRST_CON11_GRF_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON11_GRF_PSRSTN_REQ_SHIFT)             /* 0x00000400 */
#define CRU_SOFTRST_CON11_I2S0_RX_SRSTN_REQ_SHIFT          (15U)
#define CRU_SOFTRST_CON11_I2S0_RX_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON11_I2S0_RX_SRSTN_REQ_SHIFT)          /* 0x00008000 */
/* SDMMC_CON0 */
#define CRU_SDMMC_CON0_OFFSET                              (0x380U)
#define CRU_SDMMC_CON0_INIT_STATE_SHIFT                    (0U)
#define CRU_SDMMC_CON0_INIT_STATE_MASK                     (0x1U << CRU_SDMMC_CON0_INIT_STATE_SHIFT)                    /* 0x00000001 */
#define CRU_SDMMC_CON0_DRV_DEGREE_SHIFT                    (1U)
#define CRU_SDMMC_CON0_DRV_DEGREE_MASK                     (0x3U << CRU_SDMMC_CON0_DRV_DEGREE_SHIFT)                    /* 0x00000006 */
#define CRU_SDMMC_CON0_DRV_DELAYNUM_SHIFT                  (3U)
#define CRU_SDMMC_CON0_DRV_DELAYNUM_MASK                   (0xFFU << CRU_SDMMC_CON0_DRV_DELAYNUM_SHIFT)                 /* 0x000007F8 */
#define CRU_SDMMC_CON0_DRV_SEL_SHIFT                       (11U)
#define CRU_SDMMC_CON0_DRV_SEL_MASK                        (0x1U << CRU_SDMMC_CON0_DRV_SEL_SHIFT)                       /* 0x00000800 */
/* SDMMC_CON1 */
#define CRU_SDMMC_CON1_OFFSET                              (0x384U)
#define CRU_SDMMC_CON1_SAMPLE_DEGREE_SHIFT                 (0U)
#define CRU_SDMMC_CON1_SAMPLE_DEGREE_MASK                  (0x3U << CRU_SDMMC_CON1_SAMPLE_DEGREE_SHIFT)                 /* 0x00000003 */
#define CRU_SDMMC_CON1_SAMPLE_DELAYNUM_SHIFT               (2U)
#define CRU_SDMMC_CON1_SAMPLE_DELAYNUM_MASK                (0xFFU << CRU_SDMMC_CON1_SAMPLE_DELAYNUM_SHIFT)              /* 0x000003FC */
#define CRU_SDMMC_CON1_SAMPLE_SEL_SHIFT                    (10U)
#define CRU_SDMMC_CON1_SAMPLE_SEL_MASK                     (0x1U << CRU_SDMMC_CON1_SAMPLE_SEL_SHIFT)                    /* 0x00000400 */
/* SDIO_CON0 */
#define CRU_SDIO_CON0_OFFSET                               (0x388U)
#define CRU_SDIO_CON0_INIT_STATE_SHIFT                     (0U)
#define CRU_SDIO_CON0_INIT_STATE_MASK                      (0x1U << CRU_SDIO_CON0_INIT_STATE_SHIFT)                     /* 0x00000001 */
#define CRU_SDIO_CON0_DRV_DEGREE_SHIFT                     (1U)
#define CRU_SDIO_CON0_DRV_DEGREE_MASK                      (0x3U << CRU_SDIO_CON0_DRV_DEGREE_SHIFT)                     /* 0x00000006 */
#define CRU_SDIO_CON0_DRV_DELAYNUM_SHIFT                   (3U)
#define CRU_SDIO_CON0_DRV_DELAYNUM_MASK                    (0xFFU << CRU_SDIO_CON0_DRV_DELAYNUM_SHIFT)                  /* 0x000007F8 */
#define CRU_SDIO_CON0_DRV_SEL_SHIFT                        (11U)
#define CRU_SDIO_CON0_DRV_SEL_MASK                         (0x1U << CRU_SDIO_CON0_DRV_SEL_SHIFT)                        /* 0x00000800 */
/* SDIO_CON1 */
#define CRU_SDIO_CON1_OFFSET                               (0x38CU)
#define CRU_SDIO_CON1_SAMPLE_DEGREE_SHIFT                  (0U)
#define CRU_SDIO_CON1_SAMPLE_DEGREE_MASK                   (0x3U << CRU_SDIO_CON1_SAMPLE_DEGREE_SHIFT)                  /* 0x00000003 */
#define CRU_SDIO_CON1_SAMPLE_DELAYNUM_SHIFT                (2U)
#define CRU_SDIO_CON1_SAMPLE_DELAYNUM_MASK                 (0xFFU << CRU_SDIO_CON1_SAMPLE_DELAYNUM_SHIFT)               /* 0x000003FC */
#define CRU_SDIO_CON1_SAMPLE_SEL_SHIFT                     (10U)
#define CRU_SDIO_CON1_SAMPLE_SEL_MASK                      (0x1U << CRU_SDIO_CON1_SAMPLE_SEL_SHIFT)                     /* 0x00000400 */
/* EMMC_CON0 */
#define CRU_EMMC_CON0_OFFSET                               (0x390U)
#define CRU_EMMC_CON0_INIT_STATE_SHIFT                     (0U)
#define CRU_EMMC_CON0_INIT_STATE_MASK                      (0x1U << CRU_EMMC_CON0_INIT_STATE_SHIFT)                     /* 0x00000001 */
#define CRU_EMMC_CON0_DRV_DEGREE_SHIFT                     (1U)
#define CRU_EMMC_CON0_DRV_DEGREE_MASK                      (0x3U << CRU_EMMC_CON0_DRV_DEGREE_SHIFT)                     /* 0x00000006 */
#define CRU_EMMC_CON0_DRV_DELAYNUM_SHIFT                   (3U)
#define CRU_EMMC_CON0_DRV_DELAYNUM_MASK                    (0xFFU << CRU_EMMC_CON0_DRV_DELAYNUM_SHIFT)                  /* 0x000007F8 */
#define CRU_EMMC_CON0_DRV_SEL_SHIFT                        (11U)
#define CRU_EMMC_CON0_DRV_SEL_MASK                         (0x1U << CRU_EMMC_CON0_DRV_SEL_SHIFT)                        /* 0x00000800 */
/* EMMC_CON1 */
#define CRU_EMMC_CON1_OFFSET                               (0x394U)
#define CRU_EMMC_CON1_SAMPLE_DEGREE_SHIFT                  (0U)
#define CRU_EMMC_CON1_SAMPLE_DEGREE_MASK                   (0x3U << CRU_EMMC_CON1_SAMPLE_DEGREE_SHIFT)                  /* 0x00000003 */
#define CRU_EMMC_CON1_SAMPLE_DELAYNUM_SHIFT                (2U)
#define CRU_EMMC_CON1_SAMPLE_DELAYNUM_MASK                 (0xFFU << CRU_EMMC_CON1_SAMPLE_DELAYNUM_SHIFT)               /* 0x000003FC */
#define CRU_EMMC_CON1_SAMPLE_SEL_SHIFT                     (10U)
#define CRU_EMMC_CON1_SAMPLE_SEL_MASK                      (0x1U << CRU_EMMC_CON1_SAMPLE_SEL_SHIFT)                     /* 0x00000400 */
/* AUTOCS_CON0 */
#define CRU_AUTOCS_CON0_OFFSET                             (0x400U)
#define CRU_AUTOCS_CON0_PDCORE_IDLE_TH_SHIFT               (0U)
#define CRU_AUTOCS_CON0_PDCORE_IDLE_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON0_PDCORE_IDLE_TH_SHIFT)            /* 0x0000FFFF */
/* AUTOCS_CON1 */
#define CRU_AUTOCS_CON1_OFFSET                             (0x404U)
#define CRU_AUTOCS_CON1_PDCORE_SWITCH_EN_SHIFT             (0U)
#define CRU_AUTOCS_CON1_PDCORE_SWITCH_EN_MASK              (0x1U << CRU_AUTOCS_CON1_PDCORE_SWITCH_EN_SHIFT)             /* 0x00000001 */
#define CRU_AUTOCS_CON1_PDCORE_WFIWFE_EN_SHIFT             (1U)
#define CRU_AUTOCS_CON1_PDCORE_WFIWFE_EN_MASK              (0x1U << CRU_AUTOCS_CON1_PDCORE_WFIWFE_EN_SHIFT)             /* 0x00000002 */
#define CRU_AUTOCS_CON1_PDCORE_WFI_EN_SHIFT                (2U)
#define CRU_AUTOCS_CON1_PDCORE_WFI_EN_MASK                 (0x1U << CRU_AUTOCS_CON1_PDCORE_WFI_EN_SHIFT)                /* 0x00000004 */
#define CRU_AUTOCS_CON1_PDCORE_WFIL2_EN_SHIFT              (3U)
#define CRU_AUTOCS_CON1_PDCORE_WFIL2_EN_MASK               (0x1U << CRU_AUTOCS_CON1_PDCORE_WFIL2_EN_SHIFT)              /* 0x00000008 */
#define CRU_AUTOCS_CON1_PDCORE_FIQIRQ_EN_SHIFT             (4U)
#define CRU_AUTOCS_CON1_PDCORE_FIQIRQ_EN_MASK              (0x1U << CRU_AUTOCS_CON1_PDCORE_FIQIRQ_EN_SHIFT)             /* 0x00000010 */
#define CRU_AUTOCS_CON1_PDCORE_VFIQIRQ_EN_SHIFT            (5U)
#define CRU_AUTOCS_CON1_PDCORE_VFIQIRQ_EN_MASK             (0x1U << CRU_AUTOCS_CON1_PDCORE_VFIQIRQ_EN_SHIFT)            /* 0x00000020 */
#define CRU_AUTOCS_CON1_PDCORE_NIU_ACTIVE_EN_SHIFT         (6U)
#define CRU_AUTOCS_CON1_PDCORE_NIU_ACTIVE_EN_MASK          (0x1U << CRU_AUTOCS_CON1_PDCORE_NIU_ACTIVE_EN_SHIFT)         /* 0x00000040 */
#define CRU_AUTOCS_CON1_PDCORE_CLKSEL_OUT_SHIFT            (12U)
#define CRU_AUTOCS_CON1_PDCORE_CLKSEL_OUT_MASK             (0x3U << CRU_AUTOCS_CON1_PDCORE_CLKSEL_OUT_SHIFT)            /* 0x00003000 */
#define CRU_AUTOCS_CON1_PDCORE_CLKSEL_SHIFT                (14U)
#define CRU_AUTOCS_CON1_PDCORE_CLKSEL_MASK                 (0x3U << CRU_AUTOCS_CON1_PDCORE_CLKSEL_SHIFT)                /* 0x0000C000 */
/* AUTOCS_CON2 */
#define CRU_AUTOCS_CON2_OFFSET                             (0x408U)
#define CRU_AUTOCS_CON2_PDBUS_IDLE_TH_SHIFT                (0U)
#define CRU_AUTOCS_CON2_PDBUS_IDLE_TH_MASK                 (0xFFFFU << CRU_AUTOCS_CON2_PDBUS_IDLE_TH_SHIFT)             /* 0x0000FFFF */
/* AUTOCS_CON3 */
#define CRU_AUTOCS_CON3_OFFSET                             (0x40CU)
#define CRU_AUTOCS_CON3_PDBUS_SWITCH_EN_SHIFT              (0U)
#define CRU_AUTOCS_CON3_PDBUS_SWITCH_EN_MASK               (0x1U << CRU_AUTOCS_CON3_PDBUS_SWITCH_EN_SHIFT)              /* 0x00000001 */
#define CRU_AUTOCS_CON3_PDBUS_ACLK_EN_SHIFT                (1U)
#define CRU_AUTOCS_CON3_PDBUS_ACLK_EN_MASK                 (0x1U << CRU_AUTOCS_CON3_PDBUS_ACLK_EN_SHIFT)                /* 0x00000002 */
#define CRU_AUTOCS_CON3_PDBUS_HCLK_EN_SHIFT                (2U)
#define CRU_AUTOCS_CON3_PDBUS_HCLK_EN_MASK                 (0x1U << CRU_AUTOCS_CON3_PDBUS_HCLK_EN_SHIFT)                /* 0x00000004 */
#define CRU_AUTOCS_CON3_PDBUS_PCLK_EN_SHIFT                (3U)
#define CRU_AUTOCS_CON3_PDBUS_PCLK_EN_MASK                 (0x1U << CRU_AUTOCS_CON3_PDBUS_PCLK_EN_SHIFT)                /* 0x00000008 */
#define CRU_AUTOCS_CON3_PDBUS_MASTER_EN_SHIFT              (4U)
#define CRU_AUTOCS_CON3_PDBUS_MASTER_EN_MASK               (0x1U << CRU_AUTOCS_CON3_PDBUS_MASTER_EN_SHIFT)              /* 0x00000010 */
#define CRU_AUTOCS_CON3_PDBUS_SLAVE_EN_SHIFT               (5U)
#define CRU_AUTOCS_CON3_PDBUS_SLAVE_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_SLAVE_EN_SHIFT)               /* 0x00000020 */
#define CRU_AUTOCS_CON3_PDBUS_DCF_M_EN_SHIFT               (6U)
#define CRU_AUTOCS_CON3_PDBUS_DCF_M_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_DCF_M_EN_SHIFT)               /* 0x00000040 */
#define CRU_AUTOCS_CON3_PDBUS_DMAC_M_EN_SHIFT              (7U)
#define CRU_AUTOCS_CON3_PDBUS_DMAC_M_EN_MASK               (0x1U << CRU_AUTOCS_CON3_PDBUS_DMAC_M_EN_SHIFT)              /* 0x00000080 */
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_OUT_SHIFT             (12U)
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_OUT_MASK              (0x3U << CRU_AUTOCS_CON3_PDBUS_CLKSEL_OUT_SHIFT)             /* 0x00003000 */
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_SHIFT                 (14U)
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_MASK                  (0x3U << CRU_AUTOCS_CON3_PDBUS_CLKSEL_SHIFT)                 /* 0x0000C000 */
/* AUTOCS_CON4 */
#define CRU_AUTOCS_CON4_OFFSET                             (0x410U)
#define CRU_AUTOCS_CON4_PDPERI_IDLE_TH_SHIFT               (0U)
#define CRU_AUTOCS_CON4_PDPERI_IDLE_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON4_PDPERI_IDLE_TH_SHIFT)            /* 0x0000FFFF */
/* AUTOCS_CON5 */
#define CRU_AUTOCS_CON5_OFFSET                             (0x414U)
#define CRU_AUTOCS_CON5_PDPERI_SWITCH_EN_SHIFT             (0U)
#define CRU_AUTOCS_CON5_PDPERI_SWITCH_EN_MASK              (0x1U << CRU_AUTOCS_CON5_PDPERI_SWITCH_EN_SHIFT)             /* 0x00000001 */
#define CRU_AUTOCS_CON5_PDBUS_ACLK_EN_SHIFT                (1U)
#define CRU_AUTOCS_CON5_PDBUS_ACLK_EN_MASK                 (0x1U << CRU_AUTOCS_CON5_PDBUS_ACLK_EN_SHIFT)                /* 0x00000002 */
#define CRU_AUTOCS_CON5_PDBUS_HCLK_EN_SHIFT                (2U)
#define CRU_AUTOCS_CON5_PDBUS_HCLK_EN_MASK                 (0x1U << CRU_AUTOCS_CON5_PDBUS_HCLK_EN_SHIFT)                /* 0x00000004 */
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_OUT_SHIFT            (12U)
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_OUT_MASK             (0x3U << CRU_AUTOCS_CON5_PDPERI_CLKSEL_OUT_SHIFT)            /* 0x00003000 */
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_SHIFT                (14U)
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_MASK                 (0x3U << CRU_AUTOCS_CON5_PDPERI_CLKSEL_SHIFT)                /* 0x0000C000 */
/* AUTOCS_CON6 */
#define CRU_AUTOCS_CON6_OFFSET                             (0x418U)
#define CRU_AUTOCS_CON6_PDDDR_IDLE_TH_SHIFT                (0U)
#define CRU_AUTOCS_CON6_PDDDR_IDLE_TH_MASK                 (0xFFFFU << CRU_AUTOCS_CON6_PDDDR_IDLE_TH_SHIFT)             /* 0x0000FFFF */
/* AUTOCS_CON7 */
#define CRU_AUTOCS_CON7_OFFSET                             (0x41CU)
#define CRU_AUTOCS_CON7_PDDDR_SWITCH_EN_SHIFT              (0U)
#define CRU_AUTOCS_CON7_PDDDR_SWITCH_EN_MASK               (0x1U << CRU_AUTOCS_CON7_PDDDR_SWITCH_EN_SHIFT)              /* 0x00000001 */
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_OUT_SHIFT             (12U)
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_OUT_MASK              (0x3U << CRU_AUTOCS_CON7_PDDDR_CLKSEL_OUT_SHIFT)             /* 0x00003000 */
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_SHIFT                 (14U)
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_MASK                  (0x3U << CRU_AUTOCS_CON7_PDDDR_CLKSEL_SHIFT)                 /* 0x0000C000 */
/* GPLL_CON0 */
#define CRU_GPLL_CON0_OFFSET                               (0xC000U)
#define CRU_GPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_GPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_GPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_GPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_GPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_GPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_GPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_GPLL_CON0_BYPASS_MASK                          (0x1U << CRU_GPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* GPLL_CON1 */
#define CRU_GPLL_CON1_OFFSET                               (0xC004U)
#define CRU_GPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_GPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_GPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_GPLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_GPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_GPLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_GPLL_CON1_PLL_LOCK_SHIFT                       (10U)
#define CRU_GPLL_CON1_PLL_LOCK_MASK                        (0x1U << CRU_GPLL_CON1_PLL_LOCK_SHIFT)                       /* 0x00000400 */
#define CRU_GPLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_GPLL_CON1_DSMPD_MASK                           (0x1U << CRU_GPLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_GPLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_GPLL_CON1_PLLPD0_MASK                          (0x1U << CRU_GPLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_GPLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_GPLL_CON1_PLLPD1_MASK                          (0x1U << CRU_GPLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_GPLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_GPLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_GPLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* GPLL_CON2 */
#define CRU_GPLL_CON2_OFFSET                               (0xC008U)
#define CRU_GPLL_CON2_FRACDIV_SHIFT                        (0U)
#define CRU_GPLL_CON2_FRACDIV_MASK                         (0xFFFFFFU << CRU_GPLL_CON2_FRACDIV_SHIFT)                   /* 0x00FFFFFF */
#define CRU_GPLL_CON2_DACPD_SHIFT                          (24U)
#define CRU_GPLL_CON2_DACPD_MASK                           (0x1U << CRU_GPLL_CON2_DACPD_SHIFT)                          /* 0x01000000 */
#define CRU_GPLL_CON2_FOUTPOSTDIVPD_SHIFT                  (25U)
#define CRU_GPLL_CON2_FOUTPOSTDIVPD_MASK                   (0x1U << CRU_GPLL_CON2_FOUTPOSTDIVPD_SHIFT)                  /* 0x02000000 */
#define CRU_GPLL_CON2_FOUTVCOPD_SHIFT                      (26U)
#define CRU_GPLL_CON2_FOUTVCOPD_MASK                       (0x1U << CRU_GPLL_CON2_FOUTVCOPD_SHIFT)                      /* 0x04000000 */
#define CRU_GPLL_CON2_FOUT4PHASEPD_SHIFT                   (27U)
#define CRU_GPLL_CON2_FOUT4PHASEPD_MASK                    (0x1U << CRU_GPLL_CON2_FOUT4PHASEPD_SHIFT)                   /* 0x08000000 */
/* GPLL_CON3 */
#define CRU_GPLL_CON3_OFFSET                               (0xC00CU)
#define CRU_GPLL_CON3_SSMOD_BP_SHIFT                       (0U)
#define CRU_GPLL_CON3_SSMOD_BP_MASK                        (0x1U << CRU_GPLL_CON3_SSMOD_BP_SHIFT)                       /* 0x00000001 */
#define CRU_GPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT             (1U)
#define CRU_GPLL_CON3_SSMOD_DISABLE_SSCG_MASK              (0x1U << CRU_GPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)             /* 0x00000002 */
#define CRU_GPLL_CON3_SSMOD_RESET_SHIFT                    (2U)
#define CRU_GPLL_CON3_SSMOD_RESET_MASK                     (0x1U << CRU_GPLL_CON3_SSMOD_RESET_SHIFT)                    /* 0x00000004 */
#define CRU_GPLL_CON3_SSMOD_DOWNSPREAD_SHIFT               (3U)
#define CRU_GPLL_CON3_SSMOD_DOWNSPREAD_MASK                (0x1U << CRU_GPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)               /* 0x00000008 */
#define CRU_GPLL_CON3_SSMOD_DIVVAL_SHIFT                   (4U)
#define CRU_GPLL_CON3_SSMOD_DIVVAL_MASK                    (0xFU << CRU_GPLL_CON3_SSMOD_DIVVAL_SHIFT)                   /* 0x000000F0 */
#define CRU_GPLL_CON3_SSMOD_SPREAD_SHIFT                   (8U)
#define CRU_GPLL_CON3_SSMOD_SPREAD_MASK                    (0x1FU << CRU_GPLL_CON3_SSMOD_SPREAD_SHIFT)                  /* 0x00001F00 */
/* GPLL_CON4 */
#define CRU_GPLL_CON4_OFFSET                               (0xC010U)
#define CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* PMU_MODE_CON00 */
#define CRU_PMU_MODE_CON00_OFFSET                          (0xC020U)
#define CRU_PMU_MODE_CON00_GPLL_WORK_MODE_SHIFT            (0U)
#define CRU_PMU_MODE_CON00_GPLL_WORK_MODE_MASK             (0x3U << CRU_PMU_MODE_CON00_GPLL_WORK_MODE_SHIFT)            /* 0x00000003 */
/* PMU_CLKSEL_CON0 */
#define CRU_PMU_CLKSEL_CON0_OFFSET                         (0xC040U)
#define CRU_PMU_CLKSEL_CON0_PCLK_PDPMU_DIV_CON_SHIFT       (0U)
#define CRU_PMU_CLKSEL_CON0_PCLK_PDPMU_DIV_CON_MASK        (0x1FU << CRU_PMU_CLKSEL_CON0_PCLK_PDPMU_DIV_CON_SHIFT)      /* 0x0000001F */
#define CRU_PMU_CLKSEL_CON0_XIN_OSC0_FUNC_DIV_CON_SHIFT    (8U)
#define CRU_PMU_CLKSEL_CON0_XIN_OSC0_FUNC_DIV_CON_MASK     (0x1FU << CRU_PMU_CLKSEL_CON0_XIN_OSC0_FUNC_DIV_CON_SHIFT)   /* 0x00001F00 */
#define CRU_PMU_CLKSEL_CON0_CLK_RTC32K_CLK_SEL_SHIFT       (14U)
#define CRU_PMU_CLKSEL_CON0_CLK_RTC32K_CLK_SEL_MASK        (0x3U << CRU_PMU_CLKSEL_CON0_CLK_RTC32K_CLK_SEL_SHIFT)       /* 0x0000C000 */
/* PMU_CLKSEL_CON1 */
#define CRU_PMU_CLKSEL_CON1_OFFSET                         (0xC044U)
#define CRU_PMU_CLKSEL_CON1_CLK_DIV32P768KHZ_DIV_CON_SHIFT (0U)
#define CRU_PMU_CLKSEL_CON1_CLK_DIV32P768KHZ_DIV_CON_MASK  (0xFFFFFFFFU << CRU_PMU_CLKSEL_CON1_CLK_DIV32P768KHZ_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* PMU_CLKSEL_CON2 */
#define CRU_PMU_CLKSEL_CON2_OFFSET                         (0xC048U)
#define CRU_PMU_CLKSEL_CON2_CLK_REF24M_DIV_CON_SHIFT       (0U)
#define CRU_PMU_CLKSEL_CON2_CLK_REF24M_DIV_CON_MASK        (0x3FU << CRU_PMU_CLKSEL_CON2_CLK_REF24M_DIV_CON_SHIFT)      /* 0x0000003F */
#define CRU_PMU_CLKSEL_CON2_USBPHY_REF_SEL_SHIFT           (6U)
#define CRU_PMU_CLKSEL_CON2_USBPHY_REF_SEL_MASK            (0x1U << CRU_PMU_CLKSEL_CON2_USBPHY_REF_SEL_SHIFT)           /* 0x00000040 */
#define CRU_PMU_CLKSEL_CON2_MIPIDSIPHY_REF_SEL_SHIFT       (7U)
#define CRU_PMU_CLKSEL_CON2_MIPIDSIPHY_REF_SEL_MASK        (0x1U << CRU_PMU_CLKSEL_CON2_MIPIDSIPHY_REF_SEL_SHIFT)       /* 0x00000080 */
#define CRU_PMU_CLKSEL_CON2_CLK_WIFI_DIV_CON_SHIFT         (8U)
#define CRU_PMU_CLKSEL_CON2_CLK_WIFI_DIV_CON_MASK          (0x3FU << CRU_PMU_CLKSEL_CON2_CLK_WIFI_DIV_CON_SHIFT)        /* 0x00003F00 */
#define CRU_PMU_CLKSEL_CON2_CLK_WIFI_SEL_SHIFT             (15U)
#define CRU_PMU_CLKSEL_CON2_CLK_WIFI_SEL_MASK              (0x1U << CRU_PMU_CLKSEL_CON2_CLK_WIFI_SEL_SHIFT)             /* 0x00008000 */
/* PMU_CLKSEL_CON3 */
#define CRU_PMU_CLKSEL_CON3_OFFSET                         (0xC04CU)
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_DIV_CON_SHIFT        (0U)
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_DIV_CON_MASK         (0x1FU << CRU_PMU_CLKSEL_CON3_CLK_UART0_DIV_CON_SHIFT)       /* 0x0000001F */
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_PLL_SEL_SHIFT        (14U)
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_PLL_SEL_MASK         (0x3U << CRU_PMU_CLKSEL_CON3_CLK_UART0_PLL_SEL_SHIFT)        /* 0x0000C000 */
/* PMU_CLKSEL_CON4 */
#define CRU_PMU_CLKSEL_CON4_OFFSET                         (0xC050U)
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_DIVNP5_DIV_CON_SHIFT (0U)
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_DIVNP5_DIV_CON_MASK  (0x1FU << CRU_PMU_CLKSEL_CON4_CLK_UART0_DIVNP5_DIV_CON_SHIFT) /* 0x0000001F */
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_SEL_SHIFT            (14U)
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_SEL_MASK             (0x3U << CRU_PMU_CLKSEL_CON4_CLK_UART0_SEL_SHIFT)            /* 0x0000C000 */
/* PMU_CLKSEL_CON5 */
#define CRU_PMU_CLKSEL_CON5_OFFSET                         (0xC054U)
#define CRU_PMU_CLKSEL_CON5_CLK_UART0_FRAC_DIV_CON_SHIFT   (0U)
#define CRU_PMU_CLKSEL_CON5_CLK_UART0_FRAC_DIV_CON_MASK    (0xFFFFFFFFU << CRU_PMU_CLKSEL_CON5_CLK_UART0_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* PMU_CLKGATE_CON0 */
#define CRU_PMU_CLKGATE_CON0_OFFSET                        (0xC080U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PDPMU_PLL_CLK_EN_SHIFT   (0U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PDPMU_PLL_CLK_EN_MASK    (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PDPMU_PLL_CLK_EN_SHIFT)   /* 0x00000001 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_NIU_CLK_EN_SHIFT     (1U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_NIU_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_NIU_CLK_EN_SHIFT)     /* 0x00000002 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_SGRF_CLK_EN_SHIFT    (2U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_SGRF_CLK_EN_MASK     (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_SGRF_CLK_EN_SHIFT)    /* 0x00000004 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_GRF_CLK_EN_SHIFT     (3U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_GRF_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_GRF_CLK_EN_SHIFT)     /* 0x00000008 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_PMU_CLK_EN_SHIFT     (4U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_PMU_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_PMU_CLK_EN_SHIFT)     /* 0x00000010 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_MEM_CLK_EN_SHIFT     (5U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_MEM_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_MEM_CLK_EN_SHIFT)     /* 0x00000020 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_GPIO0_CLK_EN_SHIFT   (6U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_GPIO0_CLK_EN_MASK    (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_GPIO0_CLK_EN_SHIFT)   /* 0x00000040 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_UART0_CLK_EN_SHIFT   (7U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_UART0_CLK_EN_MASK    (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_UART0_CLK_EN_SHIFT)   /* 0x00000080 */
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_CRU_CLK_EN_SHIFT     (8U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_CRU_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_CRU_CLK_EN_SHIFT)     /* 0x00000100 */
#define CRU_PMU_CLKGATE_CON0_XIN_OSC0_FUNC_DIV_SRC_CLK_EN_SHIFT (12U)
#define CRU_PMU_CLKGATE_CON0_XIN_OSC0_FUNC_DIV_SRC_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON0_XIN_OSC0_FUNC_DIV_SRC_CLK_EN_SHIFT) /* 0x00001000 */
#define CRU_PMU_CLKGATE_CON0_CLK_DIV32P768KHZ_SRC_CLK_EN_SHIFT (13U)
#define CRU_PMU_CLKGATE_CON0_CLK_DIV32P768KHZ_SRC_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON0_CLK_DIV32P768KHZ_SRC_CLK_EN_SHIFT) /* 0x00002000 */
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_PLL_CLK_EN_SHIFT     (14U)
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_PLL_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_CLK_WIFI_PLL_CLK_EN_SHIFT)     /* 0x00004000 */
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_CLK_EN_SHIFT         (15U)
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_CLK_EN_MASK          (0x1U << CRU_PMU_CLKGATE_CON0_CLK_WIFI_CLK_EN_SHIFT)         /* 0x00008000 */
/* PMU_CLKGATE_CON1 */
#define CRU_PMU_CLKGATE_CON1_OFFSET                        (0xC084U)
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_PLL_CLK_EN_SHIFT (0U)
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_PLL_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_PLL_CLK_EN_SHIFT) /* 0x00000001 */
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_DIVNP5_CLK_EN_SHIFT (1U)
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_DIVNP5_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_DIVNP5_CLK_EN_SHIFT) /* 0x00000002 */
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_FRAC_CLK_EN_SHIFT (2U)
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_FRAC_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_FRAC_CLK_EN_SHIFT) /* 0x00000004 */
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_CLK_EN_SHIFT    (3U)
#define CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_CLK_EN_MASK     (0x1U << CRU_PMU_CLKGATE_CON1_CLK_UART0_PMU_CLK_EN_SHIFT)    /* 0x00000008 */
#define CRU_PMU_CLKGATE_CON1_CLK_PVTM_PMU_CLK_EN_SHIFT     (4U)
#define CRU_PMU_CLKGATE_CON1_CLK_PVTM_PMU_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON1_CLK_PVTM_PMU_CLK_EN_SHIFT)     /* 0x00000010 */
#define CRU_PMU_CLKGATE_CON1_CLK_REF24M_PLL_CLK_EN_SHIFT   (8U)
#define CRU_PMU_CLKGATE_CON1_CLK_REF24M_PLL_CLK_EN_MASK    (0x1U << CRU_PMU_CLKGATE_CON1_CLK_REF24M_PLL_CLK_EN_SHIFT)   /* 0x00000100 */
#define CRU_PMU_CLKGATE_CON1_USBPHY_REF_CLK_EN_SHIFT       (9U)
#define CRU_PMU_CLKGATE_CON1_USBPHY_REF_CLK_EN_MASK        (0x1U << CRU_PMU_CLKGATE_CON1_USBPHY_REF_CLK_EN_SHIFT)       /* 0x00000200 */
#define CRU_PMU_CLKGATE_CON1_MIPIDSIPHY_REF_CCLK_EN_SHIFT  (10U)
#define CRU_PMU_CLKGATE_CON1_MIPIDSIPHY_REF_CCLK_EN_MASK   (0x1U << CRU_PMU_CLKGATE_CON1_MIPIDSIPHY_REF_CCLK_EN_SHIFT)  /* 0x00000400 */
/* PMU_AUTOCS_CON0 */
#define CRU_PMU_AUTOCS_CON0_OFFSET                         (0xC0C0U)
#define CRU_PMU_AUTOCS_CON0_PDPMU_IDLE_TH_SHIFT            (0U)
#define CRU_PMU_AUTOCS_CON0_PDPMU_IDLE_TH_MASK             (0xFFFFU << CRU_PMU_AUTOCS_CON0_PDPMU_IDLE_TH_SHIFT)         /* 0x0000FFFF */
/* PMU_AUTOCS_CON1 */
#define CRU_PMU_AUTOCS_CON1_OFFSET                         (0xC0C4U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_SWITCH_EN_SHIFT          (0U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_SWITCH_EN_MASK           (0x1U << CRU_PMU_AUTOCS_CON1_PDPMU_SWITCH_EN_SHIFT)          /* 0x00000001 */
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_OUT_SHIFT         (12U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_OUT_MASK          (0x3U << CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_OUT_SHIFT)         /* 0x00003000 */
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_SHIFT             (14U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_MASK              (0x3U << CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_SHIFT)             /* 0x0000C000 */
/***************************************USBPHY_GRF***************************************/
/* REG0 */
#define USBPHY_GRF_REG0_OFFSET                             (0x0U)
#define USBPHY_GRF_REG0_USBPHY_REG0_SHIFT                  (0U)
#define USBPHY_GRF_REG0_USBPHY_REG0_MASK                   (0xFFFFU << USBPHY_GRF_REG0_USBPHY_REG0_SHIFT)               /* 0x0000FFFF */
/* REG1 */
#define USBPHY_GRF_REG1_OFFSET                             (0x4U)
#define USBPHY_GRF_REG1_USBPHY_REG1_SHIFT                  (0U)
#define USBPHY_GRF_REG1_USBPHY_REG1_MASK                   (0xFFFFU << USBPHY_GRF_REG1_USBPHY_REG1_SHIFT)               /* 0x0000FFFF */
/* REG2 */
#define USBPHY_GRF_REG2_OFFSET                             (0x8U)
#define USBPHY_GRF_REG2_USBPHY_REG2_SHIFT                  (0U)
#define USBPHY_GRF_REG2_USBPHY_REG2_MASK                   (0xFFFFU << USBPHY_GRF_REG2_USBPHY_REG2_SHIFT)               /* 0x0000FFFF */
/* REG3 */
#define USBPHY_GRF_REG3_OFFSET                             (0xCU)
#define USBPHY_GRF_REG3_USBPHY_REG3_SHIFT                  (0U)
#define USBPHY_GRF_REG3_USBPHY_REG3_MASK                   (0xFFFFU << USBPHY_GRF_REG3_USBPHY_REG3_SHIFT)               /* 0x0000FFFF */
/* REG4 */
#define USBPHY_GRF_REG4_OFFSET                             (0x10U)
#define USBPHY_GRF_REG4_USBPHY_REG4_SHIFT                  (0U)
#define USBPHY_GRF_REG4_USBPHY_REG4_MASK                   (0xFFFFU << USBPHY_GRF_REG4_USBPHY_REG4_SHIFT)               /* 0x0000FFFF */
/* REG5 */
#define USBPHY_GRF_REG5_OFFSET                             (0x14U)
#define USBPHY_GRF_REG5_USBPHY_REG5_SHIFT                  (0U)
#define USBPHY_GRF_REG5_USBPHY_REG5_MASK                   (0xFFFFU << USBPHY_GRF_REG5_USBPHY_REG5_SHIFT)               /* 0x0000FFFF */
/* REG6 */
#define USBPHY_GRF_REG6_OFFSET                             (0x18U)
#define USBPHY_GRF_REG6_USBPHY_REG6_SHIFT                  (0U)
#define USBPHY_GRF_REG6_USBPHY_REG6_MASK                   (0xFFFFU << USBPHY_GRF_REG6_USBPHY_REG6_SHIFT)               /* 0x0000FFFF */
/* REG7 */
#define USBPHY_GRF_REG7_OFFSET                             (0x1CU)
#define USBPHY_GRF_REG7_USBPHY_REG7_SHIFT                  (0U)
#define USBPHY_GRF_REG7_USBPHY_REG7_MASK                   (0xFFFFU << USBPHY_GRF_REG7_USBPHY_REG7_SHIFT)               /* 0x0000FFFF */
/* REG8 */
#define USBPHY_GRF_REG8_OFFSET                             (0x20U)
#define USBPHY_GRF_REG8_USBPHY_REG8_SHIFT                  (0U)
#define USBPHY_GRF_REG8_USBPHY_REG8_MASK                   (0xFFFFU << USBPHY_GRF_REG8_USBPHY_REG8_SHIFT)               /* 0x0000FFFF */
/* REG9 */
#define USBPHY_GRF_REG9_OFFSET                             (0x24U)
#define USBPHY_GRF_REG9_USBPHY_REG9_SHIFT                  (0U)
#define USBPHY_GRF_REG9_USBPHY_REG9_MASK                   (0xFFFFU << USBPHY_GRF_REG9_USBPHY_REG9_SHIFT)               /* 0x0000FFFF */
/* REG10 */
#define USBPHY_GRF_REG10_OFFSET                            (0x28U)
#define USBPHY_GRF_REG10_USBPHY_REG10_SHIFT                (0U)
#define USBPHY_GRF_REG10_USBPHY_REG10_MASK                 (0xFFFFU << USBPHY_GRF_REG10_USBPHY_REG10_SHIFT)             /* 0x0000FFFF */
/* REG11 */
#define USBPHY_GRF_REG11_OFFSET                            (0x2CU)
#define USBPHY_GRF_REG11_USBPHY_REG11_SHIFT                (0U)
#define USBPHY_GRF_REG11_USBPHY_REG11_MASK                 (0xFFFFU << USBPHY_GRF_REG11_USBPHY_REG11_SHIFT)             /* 0x0000FFFF */
/* REG12 */
#define USBPHY_GRF_REG12_OFFSET                            (0x30U)
#define USBPHY_GRF_REG12_USBPHY_REG12_SHIFT                (0U)
#define USBPHY_GRF_REG12_USBPHY_REG12_MASK                 (0xFFFFU << USBPHY_GRF_REG12_USBPHY_REG12_SHIFT)             /* 0x0000FFFF */
/* REG13 */
#define USBPHY_GRF_REG13_OFFSET                            (0x34U)
#define USBPHY_GRF_REG13_USBPHY_REG13_SHIFT                (0U)
#define USBPHY_GRF_REG13_USBPHY_REG13_MASK                 (0xFFFFU << USBPHY_GRF_REG13_USBPHY_REG13_SHIFT)             /* 0x0000FFFF */
/* REG14 */
#define USBPHY_GRF_REG14_OFFSET                            (0x38U)
#define USBPHY_GRF_REG14_USBPHY_REG14_SHIFT                (0U)
#define USBPHY_GRF_REG14_USBPHY_REG14_MASK                 (0xFFFFU << USBPHY_GRF_REG14_USBPHY_REG14_SHIFT)             /* 0x0000FFFF */
/* REG15 */
#define USBPHY_GRF_REG15_OFFSET                            (0x3CU)
#define USBPHY_GRF_REG15_USBPHY_REG15_SHIFT                (0U)
#define USBPHY_GRF_REG15_USBPHY_REG15_MASK                 (0xFFFFU << USBPHY_GRF_REG15_USBPHY_REG15_SHIFT)             /* 0x0000FFFF */
/* REG16 */
#define USBPHY_GRF_REG16_OFFSET                            (0x40U)
#define USBPHY_GRF_REG16_USBPHY_REG16_SHIFT                (0U)
#define USBPHY_GRF_REG16_USBPHY_REG16_MASK                 (0xFFFFU << USBPHY_GRF_REG16_USBPHY_REG16_SHIFT)             /* 0x0000FFFF */
/* REG17 */
#define USBPHY_GRF_REG17_OFFSET                            (0x44U)
#define USBPHY_GRF_REG17_USBPHY_REG17_SHIFT                (0U)
#define USBPHY_GRF_REG17_USBPHY_REG17_MASK                 (0xFFFFU << USBPHY_GRF_REG17_USBPHY_REG17_SHIFT)             /* 0x0000FFFF */
/* REG18 */
#define USBPHY_GRF_REG18_OFFSET                            (0x48U)
#define USBPHY_GRF_REG18_USBPHY_REG18_SHIFT                (0U)
#define USBPHY_GRF_REG18_USBPHY_REG18_MASK                 (0xFFFFU << USBPHY_GRF_REG18_USBPHY_REG18_SHIFT)             /* 0x0000FFFF */
/* REG19 */
#define USBPHY_GRF_REG19_OFFSET                            (0x4CU)
#define USBPHY_GRF_REG19_USBPHY_REG19_SHIFT                (0U)
#define USBPHY_GRF_REG19_USBPHY_REG19_MASK                 (0xFFFFU << USBPHY_GRF_REG19_USBPHY_REG19_SHIFT)             /* 0x0000FFFF */
/* REG20 */
#define USBPHY_GRF_REG20_OFFSET                            (0x50U)
#define USBPHY_GRF_REG20_USBPHY_REG20_SHIFT                (0U)
#define USBPHY_GRF_REG20_USBPHY_REG20_MASK                 (0xFFFFU << USBPHY_GRF_REG20_USBPHY_REG20_SHIFT)             /* 0x0000FFFF */
/* REG21 */
#define USBPHY_GRF_REG21_OFFSET                            (0x54U)
#define USBPHY_GRF_REG21_USBPHY_REG21_SHIFT                (0U)
#define USBPHY_GRF_REG21_USBPHY_REG21_MASK                 (0xFFFFU << USBPHY_GRF_REG21_USBPHY_REG21_SHIFT)             /* 0x0000FFFF */
/* REG22 */
#define USBPHY_GRF_REG22_OFFSET                            (0x58U)
#define USBPHY_GRF_REG22_USBPHY_REG22_SHIFT                (0U)
#define USBPHY_GRF_REG22_USBPHY_REG22_MASK                 (0xFFFFU << USBPHY_GRF_REG22_USBPHY_REG22_SHIFT)             /* 0x0000FFFF */
/* REG23 */
#define USBPHY_GRF_REG23_OFFSET                            (0x5CU)
#define USBPHY_GRF_REG23_USBPHY_REG23_SHIFT                (0U)
#define USBPHY_GRF_REG23_USBPHY_REG23_MASK                 (0xFFFFU << USBPHY_GRF_REG23_USBPHY_REG23_SHIFT)             /* 0x0000FFFF */
/* CON0 */
#define USBPHY_GRF_CON0_OFFSET                             (0x100U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_SEL_SHIFT              (0U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_SEL_MASK               (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_SEL_SHIFT)              /* 0x00000001 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_SUSPEND_N_SHIFT        (1U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_SUSPEND_N_MASK         (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_SUSPEND_N_SHIFT)        /* 0x00000002 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_OPMODE_SHIFT           (2U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_OPMODE_MASK            (0x3U << USBPHY_GRF_CON0_USBOTG_UTMI_OPMODE_SHIFT)           /* 0x0000000C */
#define USBPHY_GRF_CON0_USBOTG_UTMI_XCVRSELECT_SHIFT       (4U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_XCVRSELECT_MASK        (0x3U << USBPHY_GRF_CON0_USBOTG_UTMI_XCVRSELECT_SHIFT)       /* 0x00000030 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_TERMSELECT_SHIFT       (6U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_TERMSELECT_MASK        (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_TERMSELECT_SHIFT)       /* 0x00000040 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_SHIFT       (7U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_MASK        (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_SHIFT)       /* 0x00000080 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_SHIFT       (8U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_MASK        (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_SHIFT)       /* 0x00000100 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_SHIFT        (9U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_MASK         (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_SHIFT)        /* 0x00000200 */
#define USBPHY_GRF_CON0_USBOTG_UTMI_IDDIG_SHIFT            (10U)
#define USBPHY_GRF_CON0_USBOTG_UTMI_IDDIG_MASK             (0x1U << USBPHY_GRF_CON0_USBOTG_UTMI_IDDIG_SHIFT)            /* 0x00000400 */
/* CON1 */
#define USBPHY_GRF_CON1_OFFSET                             (0x104U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_SEL_SHIFT             (0U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_SEL_MASK              (0x1U << USBPHY_GRF_CON1_USBHOST_UTMI_SEL_SHIFT)             /* 0x00000001 */
#define USBPHY_GRF_CON1_USBHOST_UTMI_SUSPEND_N_SHIFT       (1U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_SUSPEND_N_MASK        (0x1U << USBPHY_GRF_CON1_USBHOST_UTMI_SUSPEND_N_SHIFT)       /* 0x00000002 */
#define USBPHY_GRF_CON1_USBHOST_UTMI_OPMODE_SHIFT          (2U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_OPMODE_MASK           (0x3U << USBPHY_GRF_CON1_USBHOST_UTMI_OPMODE_SHIFT)          /* 0x0000000C */
#define USBPHY_GRF_CON1_USBHOST_UTMI_XCVRSELECT_SHIFT      (4U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_XCVRSELECT_MASK       (0x3U << USBPHY_GRF_CON1_USBHOST_UTMI_XCVRSELECT_SHIFT)      /* 0x00000030 */
#define USBPHY_GRF_CON1_USBHOST_UTMI_TERMSELECT_SHIFT      (6U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_TERMSELECT_MASK       (0x1U << USBPHY_GRF_CON1_USBHOST_UTMI_TERMSELECT_SHIFT)      /* 0x00000040 */
#define USBPHY_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_SHIFT      (7U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_MASK       (0x1U << USBPHY_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_SHIFT)      /* 0x00000080 */
#define USBPHY_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_SHIFT      (8U)
#define USBPHY_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_MASK       (0x1U << USBPHY_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_SHIFT)      /* 0x00000100 */
/* CON2 */
#define USBPHY_GRF_CON2_OFFSET                             (0x108U)
#define USBPHY_GRF_CON2_USBOTG_DISABLE_0_SHIFT             (0U)
#define USBPHY_GRF_CON2_USBOTG_DISABLE_0_MASK              (0x1U << USBPHY_GRF_CON2_USBOTG_DISABLE_0_SHIFT)             /* 0x00000001 */
#define USBPHY_GRF_CON2_USBOTG_DISABLE_1_SHIFT             (1U)
#define USBPHY_GRF_CON2_USBOTG_DISABLE_1_MASK              (0x1U << USBPHY_GRF_CON2_USBOTG_DISABLE_1_SHIFT)             /* 0x00000002 */
#define USBPHY_GRF_CON2_BYPASSDMEN_USBOTG_SHIFT            (2U)
#define USBPHY_GRF_CON2_BYPASSDMEN_USBOTG_MASK             (0x1U << USBPHY_GRF_CON2_BYPASSDMEN_USBOTG_SHIFT)            /* 0x00000004 */
#define USBPHY_GRF_CON2_BYPASSSEL_USBOTG_SHIFT             (3U)
#define USBPHY_GRF_CON2_BYPASSSEL_USBOTG_MASK              (0x1U << USBPHY_GRF_CON2_BYPASSSEL_USBOTG_SHIFT)             /* 0x00000008 */
#define USBPHY_GRF_CON2_USBPHY_COMMONONN_SHIFT             (4U)
#define USBPHY_GRF_CON2_USBPHY_COMMONONN_MASK              (0x1U << USBPHY_GRF_CON2_USBPHY_COMMONONN_SHIFT)             /* 0x00000010 */
#define USBPHY_GRF_CON2_IDP_SINK_EN_USBOTG_SHIFT           (7U)
#define USBPHY_GRF_CON2_IDP_SINK_EN_USBOTG_MASK            (0x1U << USBPHY_GRF_CON2_IDP_SINK_EN_USBOTG_SHIFT)           /* 0x00000080 */
#define USBPHY_GRF_CON2_IDM_SINK_EN_USBOTG_SHIFT           (8U)
#define USBPHY_GRF_CON2_IDM_SINK_EN_USBOTG_MASK            (0x1U << USBPHY_GRF_CON2_IDM_SINK_EN_USBOTG_SHIFT)           /* 0x00000100 */
#define USBPHY_GRF_CON2_IDP_SRC_EN_USBOTG_SHIFT            (9U)
#define USBPHY_GRF_CON2_IDP_SRC_EN_USBOTG_MASK             (0x1U << USBPHY_GRF_CON2_IDP_SRC_EN_USBOTG_SHIFT)            /* 0x00000200 */
#define USBPHY_GRF_CON2_RDM_PDWN_EN_USBOTG_SHIFT           (10U)
#define USBPHY_GRF_CON2_RDM_PDWN_EN_USBOTG_MASK            (0x1U << USBPHY_GRF_CON2_RDM_PDWN_EN_USBOTG_SHIFT)           /* 0x00000400 */
#define USBPHY_GRF_CON2_VDP_SRC_EN_USBOTG_SHIFT            (11U)
#define USBPHY_GRF_CON2_VDP_SRC_EN_USBOTG_MASK             (0x1U << USBPHY_GRF_CON2_VDP_SRC_EN_USBOTG_SHIFT)            /* 0x00000800 */
#define USBPHY_GRF_CON2_VDM_SRC_EN_USBOTG_SHIFT            (12U)
#define USBPHY_GRF_CON2_VDM_SRC_EN_USBOTG_MASK             (0x1U << USBPHY_GRF_CON2_VDM_SRC_EN_USBOTG_SHIFT)            /* 0x00001000 */
/* CON3 */
#define USBPHY_GRF_CON3_OFFSET                             (0x10CU)
#define USBPHY_GRF_CON3_USBHOST_UTMI_DRVVBUS_SHIFT         (0U)
#define USBPHY_GRF_CON3_USBHOST_UTMI_DRVVBUS_MASK          (0x1U << USBPHY_GRF_CON3_USBHOST_UTMI_DRVVBUS_SHIFT)         /* 0x00000001 */
#define USBPHY_GRF_CON3_USBHOST_UTMI_CHRGVBUS_SHIFT        (1U)
#define USBPHY_GRF_CON3_USBHOST_UTMI_CHRGVBUS_MASK         (0x1U << USBPHY_GRF_CON3_USBHOST_UTMI_CHRGVBUS_SHIFT)        /* 0x00000002 */
#define USBPHY_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_SHIFT     (2U)
#define USBPHY_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_MASK      (0x1U << USBPHY_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_SHIFT)     /* 0x00000004 */
#define USBPHY_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_SHIFT      (3U)
#define USBPHY_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_MASK       (0x1U << USBPHY_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_SHIFT)      /* 0x00000008 */
#define USBPHY_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_SHIFT      (4U)
#define USBPHY_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_MASK       (0x1U << USBPHY_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_SHIFT)      /* 0x00000010 */
#define USBPHY_GRF_CON3_USBHOST_UTMI_IDPULLUP_SHIFT        (5U)
#define USBPHY_GRF_CON3_USBHOST_UTMI_IDPULLUP_MASK         (0x1U << USBPHY_GRF_CON3_USBHOST_UTMI_IDPULLUP_SHIFT)        /* 0x00000020 */
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_SHIFT      (6U)
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_MASK       (0x1U << USBPHY_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_SHIFT)      /* 0x00000040 */
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_OE_SHIFT            (7U)
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_OE_MASK             (0x1U << USBPHY_GRF_CON3_USBOTG_UTMI_FS_OE_SHIFT)            /* 0x00000080 */
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_DATA_SHIFT          (8U)
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_DATA_MASK           (0x1U << USBPHY_GRF_CON3_USBOTG_UTMI_FS_DATA_SHIFT)          /* 0x00000100 */
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_SE0_SHIFT           (9U)
#define USBPHY_GRF_CON3_USBOTG_UTMI_FS_SE0_MASK            (0x1U << USBPHY_GRF_CON3_USBOTG_UTMI_FS_SE0_SHIFT)           /* 0x00000200 */
#define USBPHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_SHIFT      (10U)
#define USBPHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_MASK       (0x1U << USBPHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_SHIFT)      /* 0x00000400 */
#define USBPHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SHIFT          (11U)
#define USBPHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_MASK           (0x1U << USBPHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SHIFT)          /* 0x00000800 */
/* INT_MASK */
#define USBPHY_GRF_INT_MASK_OFFSET                         (0x110U)
#define USBPHY_GRF_INT_MASK_OTG0_LINESTATE_IRQ_EN_SHIFT    (0U)
#define USBPHY_GRF_INT_MASK_OTG0_LINESTATE_IRQ_EN_MASK     (0x1U << USBPHY_GRF_INT_MASK_OTG0_LINESTATE_IRQ_EN_SHIFT)    /* 0x00000001 */
#define USBPHY_GRF_INT_MASK_HOST0_LINESTATE_IRQ_EN_SHIFT   (1U)
#define USBPHY_GRF_INT_MASK_HOST0_LINESTATE_IRQ_EN_MASK    (0x1U << USBPHY_GRF_INT_MASK_HOST0_LINESTATE_IRQ_EN_SHIFT)   /* 0x00000002 */
#define USBPHY_GRF_INT_MASK_OTG0_BVALID_IRQ_EN_SHIFT       (2U)
#define USBPHY_GRF_INT_MASK_OTG0_BVALID_IRQ_EN_MASK        (0x3U << USBPHY_GRF_INT_MASK_OTG0_BVALID_IRQ_EN_SHIFT)       /* 0x0000000C */
#define USBPHY_GRF_INT_MASK_OTG0_ID_IRQ_EN_SHIFT           (4U)
#define USBPHY_GRF_INT_MASK_OTG0_ID_IRQ_EN_MASK            (0x3U << USBPHY_GRF_INT_MASK_OTG0_ID_IRQ_EN_SHIFT)           /* 0x00000030 */
#define USBPHY_GRF_INT_MASK_OTG0_DISCONNECT_IRQ_EN_SHIFT   (6U)
#define USBPHY_GRF_INT_MASK_OTG0_DISCONNECT_IRQ_EN_MASK    (0x3U << USBPHY_GRF_INT_MASK_OTG0_DISCONNECT_IRQ_EN_SHIFT)   /* 0x000000C0 */
#define USBPHY_GRF_INT_MASK_HOST0_DISCONNECT_IRQ_EN_SHIFT  (8U)
#define USBPHY_GRF_INT_MASK_HOST0_DISCONNECT_IRQ_EN_MASK   (0x3U << USBPHY_GRF_INT_MASK_HOST0_DISCONNECT_IRQ_EN_SHIFT)  /* 0x00000300 */
/* INT_STATUS */
#define USBPHY_GRF_INT_STATUS_OFFSET                       (0x114U)
#define USBPHY_GRF_INT_STATUS                              (0x0U)
#define USBPHY_GRF_INT_STATUS_OTG0_LINESTATE_IRQ_SHIFT     (0U)
#define USBPHY_GRF_INT_STATUS_OTG0_LINESTATE_IRQ_MASK      (0x1U << USBPHY_GRF_INT_STATUS_OTG0_LINESTATE_IRQ_SHIFT)     /* 0x00000001 */
#define USBPHY_GRF_INT_STATUS_HOST0_LINESTATE_IRQ_SHIFT    (1U)
#define USBPHY_GRF_INT_STATUS_HOST0_LINESTATE_IRQ_MASK     (0x1U << USBPHY_GRF_INT_STATUS_HOST0_LINESTATE_IRQ_SHIFT)    /* 0x00000002 */
#define USBPHY_GRF_INT_STATUS_OTG0_BVALID_IRQ_SHIFT        (2U)
#define USBPHY_GRF_INT_STATUS_OTG0_BVALID_IRQ_MASK         (0x3U << USBPHY_GRF_INT_STATUS_OTG0_BVALID_IRQ_SHIFT)        /* 0x0000000C */
#define USBPHY_GRF_INT_STATUS_OTG0_ID_IRQ_SHIFT            (4U)
#define USBPHY_GRF_INT_STATUS_OTG0_ID_IRQ_MASK             (0x3U << USBPHY_GRF_INT_STATUS_OTG0_ID_IRQ_SHIFT)            /* 0x00000030 */
#define USBPHY_GRF_INT_STATUS_OTG0_DISCONNECT_IRQ_SHIFT    (6U)
#define USBPHY_GRF_INT_STATUS_OTG0_DISCONNECT_IRQ_MASK     (0x3U << USBPHY_GRF_INT_STATUS_OTG0_DISCONNECT_IRQ_SHIFT)    /* 0x000000C0 */
#define USBPHY_GRF_INT_STATUS_HOST0_DISCONNECT_IRQ_SHIFT   (8U)
#define USBPHY_GRF_INT_STATUS_HOST0_DISCONNECT_IRQ_MASK    (0x3U << USBPHY_GRF_INT_STATUS_HOST0_DISCONNECT_IRQ_SHIFT)   /* 0x00000300 */
/* INT_STATUS_CLR */
#define USBPHY_GRF_INT_STATUS_CLR_OFFSET                   (0x118U)
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_SHIFT (0U)
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_MASK (0x1U << USBPHY_GRF_INT_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_SHIFT) /* 0x00000001 */
#define USBPHY_GRF_INT_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_SHIFT (1U)
#define USBPHY_GRF_INT_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_MASK (0x1U << USBPHY_GRF_INT_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_SHIFT) /* 0x00000002 */
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_BVALID_IRQ_CLR_SHIFT (2U)
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_BVALID_IRQ_CLR_MASK (0x3U << USBPHY_GRF_INT_STATUS_CLR_OTG0_BVALID_IRQ_CLR_SHIFT) /* 0x0000000C */
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_ID_IRQ_CLR_SHIFT    (4U)
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_ID_IRQ_CLR_MASK     (0x3U << USBPHY_GRF_INT_STATUS_CLR_OTG0_ID_IRQ_CLR_SHIFT)    /* 0x00000030 */
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_DISCONNECT_IRQ_CLR_SHIFT (6U)
#define USBPHY_GRF_INT_STATUS_CLR_OTG0_DISCONNECT_IRQ_CLR_MASK (0x3U << USBPHY_GRF_INT_STATUS_CLR_OTG0_DISCONNECT_IRQ_CLR_SHIFT) /* 0x000000C0 */
#define USBPHY_GRF_INT_STATUS_CLR_HOST0_DISCONNECT_IRQ_CLR_SHIFT (8U)
#define USBPHY_GRF_INT_STATUS_CLR_HOST0_DISCONNECT_IRQ_CLR_MASK (0x3U << USBPHY_GRF_INT_STATUS_CLR_HOST0_DISCONNECT_IRQ_CLR_SHIFT) /* 0x00000300 */
/* STATUS */
#define USBPHY_GRF_STATUS_OFFSET                           (0x120U)
#define USBPHY_GRF_STATUS                                  (0x0U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_VPI_SHIFT            (0U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_VPI_MASK             (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_VPI_SHIFT)            /* 0x00000001 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_VMI_SHIFT            (1U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_VMI_MASK             (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_VMI_SHIFT)            /* 0x00000002 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_VBUSVALID_SHIFT      (2U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_VBUSVALID_MASK       (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_VBUSVALID_SHIFT)      /* 0x00000004 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_SESSEND_SHIFT        (3U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_SESSEND_MASK         (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_SESSEND_SHIFT)        /* 0x00000008 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_LINESTATE_SHIFT      (4U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_LINESTATE_MASK       (0x3U << USBPHY_GRF_STATUS_USBOTG_UTMI_LINESTATE_SHIFT)      /* 0x00000030 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_IDDIG_SHIFT          (6U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_IDDIG_MASK           (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_IDDIG_SHIFT)          /* 0x00000040 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_SHIFT (7U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_MASK  (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_SHIFT) /* 0x00000080 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_SHIFT    (8U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_MASK     (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_SHIFT)    /* 0x00000100 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_BVALID_SHIFT         (9U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_BVALID_MASK          (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_BVALID_SHIFT)         /* 0x00000200 */
#define USBPHY_GRF_STATUS_USBOTG_UTMI_AVALID_SHIFT         (10U)
#define USBPHY_GRF_STATUS_USBOTG_UTMI_AVALID_MASK          (0x1U << USBPHY_GRF_STATUS_USBOTG_UTMI_AVALID_SHIFT)         /* 0x00000400 */
#define USBPHY_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_SHIFT       (11U)
#define USBPHY_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_MASK        (0x1U << USBPHY_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_SHIFT)       /* 0x00000800 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_VPI_SHIFT           (12U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_VPI_MASK            (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_VPI_SHIFT)           /* 0x00001000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_VMI_SHIFT           (13U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_VMI_MASK            (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_VMI_SHIFT)           /* 0x00002000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_VBUSVALID_SHIFT     (14U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_VBUSVALID_MASK      (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_VBUSVALID_SHIFT)     /* 0x00004000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_SESSEND_SHIFT       (15U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_SESSEND_MASK        (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_SESSEND_SHIFT)       /* 0x00008000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_LINESTATE_SHIFT     (16U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_LINESTATE_MASK      (0x3U << USBPHY_GRF_STATUS_USBHOST_UTMI_LINESTATE_SHIFT)     /* 0x00030000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_IDDIG_O_SHIFT       (18U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_IDDIG_O_MASK        (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_IDDIG_O_SHIFT)       /* 0x00040000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_SHIFT (19U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_MASK (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_SHIFT) /* 0x00080000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_BVALID_SHIFT        (20U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_BVALID_MASK         (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_BVALID_SHIFT)        /* 0x00100000 */
#define USBPHY_GRF_STATUS_USBHOST_UTMI_AVALID_SHIFT        (21U)
#define USBPHY_GRF_STATUS_USBHOST_UTMI_AVALID_MASK         (0x1U << USBPHY_GRF_STATUS_USBHOST_UTMI_AVALID_SHIFT)        /* 0x00200000 */
#define USBPHY_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_SHIFT      (22U)
#define USBPHY_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_MASK       (0x1U << USBPHY_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_SHIFT)      /* 0x00400000 */
#define USBPHY_GRF_STATUS_GRF_STAT_USBPHY_DCP_DETECTED_SHIFT (23U)
#define USBPHY_GRF_STATUS_GRF_STAT_USBPHY_DCP_DETECTED_MASK (0x1U << USBPHY_GRF_STATUS_GRF_STAT_USBPHY_DCP_DETECTED_SHIFT) /* 0x00800000 */
#define USBPHY_GRF_STATUS_GRF_STAT_USBPHY_CP_DETECTED_SHIFT (24U)
#define USBPHY_GRF_STATUS_GRF_STAT_USBPHY_CP_DETECTED_MASK (0x1U << USBPHY_GRF_STATUS_GRF_STAT_USBPHY_CP_DETECTED_SHIFT) /* 0x01000000 */
#define USBPHY_GRF_STATUS_GRF_STAT_USBPHY_DP_DETECTED_SHIFT (25U)
#define USBPHY_GRF_STATUS_GRF_STAT_USBPHY_DP_DETECTED_MASK (0x1U << USBPHY_GRF_STATUS_GRF_STAT_USBPHY_DP_DETECTED_SHIFT) /* 0x02000000 */
/* LS_CON */
#define USBPHY_GRF_LS_CON_OFFSET                           (0x130U)
#define USBPHY_GRF_LS_CON_LINESTATE_FILTER_CON_SHIFT       (0U)
#define USBPHY_GRF_LS_CON_LINESTATE_FILTER_CON_MASK        (0xFFFFFU << USBPHY_GRF_LS_CON_LINESTATE_FILTER_CON_SHIFT)   /* 0x000FFFFF */
/* DIS_CON */
#define USBPHY_GRF_DIS_CON_OFFSET                          (0x134U)
#define USBPHY_GRF_DIS_CON_DISCONNECT_FILTER_CON_SHIFT     (0U)
#define USBPHY_GRF_DIS_CON_DISCONNECT_FILTER_CON_MASK      (0xFFFFFU << USBPHY_GRF_DIS_CON_DISCONNECT_FILTER_CON_SHIFT) /* 0x000FFFFF */
/* BVALID_CON */
#define USBPHY_GRF_BVALID_CON_OFFSET                       (0x138U)
#define USBPHY_GRF_BVALID_CON_BVALID_FILTER_CON_SHIFT      (0U)
#define USBPHY_GRF_BVALID_CON_BVALID_FILTER_CON_MASK       (0xFFFFFU << USBPHY_GRF_BVALID_CON_BVALID_FILTER_CON_SHIFT)  /* 0x000FFFFF */
/* ID_CON */
#define USBPHY_GRF_ID_CON_OFFSET                           (0x13CU)
#define USBPHY_GRF_ID_CON_ID_FILTER_CON_SHIFT              (0U)
#define USBPHY_GRF_ID_CON_ID_FILTER_CON_MASK               (0xFFFFFFFU << USBPHY_GRF_ID_CON_ID_FILTER_CON_SHIFT)        /* 0x0FFFFFFF */
/******************************************GMAC******************************************/
/* MAC_CONF */
#define GMAC_MAC_CONF_OFFSET                               (0x0U)
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
#define GMAC_MAC_FRM_FILT_OFFSET                           (0x4U)
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
#define GMAC_HASH_TAB_HI_OFFSET                            (0x8U)
#define GMAC_HASH_TAB_HI_HTH_SHIFT                         (0U)
#define GMAC_HASH_TAB_HI_HTH_MASK                          (0xFFFFFFFFU << GMAC_HASH_TAB_HI_HTH_SHIFT)                  /* 0xFFFFFFFF */
/* HASH_TAB_LO */
#define GMAC_HASH_TAB_LO_OFFSET                            (0xCU)
#define GMAC_HASH_TAB_LO_HTL_SHIFT                         (0U)
#define GMAC_HASH_TAB_LO_HTL_MASK                          (0xFFFFFFFFU << GMAC_HASH_TAB_LO_HTL_SHIFT)                  /* 0xFFFFFFFF */
/* GMII_ADDR */
#define GMAC_GMII_ADDR_OFFSET                              (0x10U)
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
#define GMAC_GMII_DATA_OFFSET                              (0x14U)
#define GMAC_GMII_DATA_GD_SHIFT                            (0U)
#define GMAC_GMII_DATA_GD_MASK                             (0xFFFFU << GMAC_GMII_DATA_GD_SHIFT)                         /* 0x0000FFFF */
/* FLOW_CTRL */
#define GMAC_FLOW_CTRL_OFFSET                              (0x18U)
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
#define GMAC_VLAN_TAG_OFFSET                               (0x1CU)
#define GMAC_VLAN_TAG_VL_SHIFT                             (0U)
#define GMAC_VLAN_TAG_VL_MASK                              (0xFFFFU << GMAC_VLAN_TAG_VL_SHIFT)                          /* 0x0000FFFF */
#define GMAC_VLAN_TAG_ETV_SHIFT                            (16U)
#define GMAC_VLAN_TAG_ETV_MASK                             (0x1U << GMAC_VLAN_TAG_ETV_SHIFT)                            /* 0x00010000 */
/* DEBUG */
#define GMAC_DEBUG_OFFSET                                  (0x24U)
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
#define GMAC_PMT_CTRL_STA_OFFSET                           (0x2CU)
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
#define GMAC_INT_STATUS_OFFSET                             (0x38U)
#define GMAC_INT_STATUS                                    (0x0U)
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
#define GMAC_INT_MASK_OFFSET                               (0x3CU)
#define GMAC_INT_MASK_RIM_SHIFT                            (0U)
#define GMAC_INT_MASK_RIM_MASK                             (0x1U << GMAC_INT_MASK_RIM_SHIFT)                            /* 0x00000001 */
#define GMAC_INT_MASK_PIM_SHIFT                            (3U)
#define GMAC_INT_MASK_PIM_MASK                             (0x1U << GMAC_INT_MASK_PIM_SHIFT)                            /* 0x00000008 */
/* MAC_ADDR0_HI */
#define GMAC_MAC_ADDR0_HI_OFFSET                           (0x40U)
#define GMAC_MAC_ADDR0_HI_A47_A32_SHIFT                    (0U)
#define GMAC_MAC_ADDR0_HI_A47_A32_MASK                     (0xFFFFU << GMAC_MAC_ADDR0_HI_A47_A32_SHIFT)                 /* 0x0000FFFF */
/* MAC_ADDR0_LO */
#define GMAC_MAC_ADDR0_LO_OFFSET                           (0x44U)
#define GMAC_MAC_ADDR0_LO_A31_A0_SHIFT                     (0U)
#define GMAC_MAC_ADDR0_LO_A31_A0_MASK                      (0xFFFFFFFFU << GMAC_MAC_ADDR0_LO_A31_A0_SHIFT)              /* 0xFFFFFFFF */
/* AN_CTRL */
#define GMAC_AN_CTRL_OFFSET                                (0xC0U)
#define GMAC_AN_CTRL_RAN_SHIFT                             (9U)
#define GMAC_AN_CTRL_RAN_MASK                              (0x1U << GMAC_AN_CTRL_RAN_SHIFT)                             /* 0x00000200 */
#define GMAC_AN_CTRL_ANE_SHIFT                             (12U)
#define GMAC_AN_CTRL_ANE_MASK                              (0x1U << GMAC_AN_CTRL_ANE_SHIFT)                             /* 0x00001000 */
/* AN_STATUS */
#define GMAC_AN_STATUS_OFFSET                              (0xC4U)
#define GMAC_AN_STATUS_LS_SHIFT                            (2U)
#define GMAC_AN_STATUS_LS_MASK                             (0x1U << GMAC_AN_STATUS_LS_SHIFT)                            /* 0x00000004 */
#define GMAC_AN_STATUS_ANA_SHIFT                           (3U)
#define GMAC_AN_STATUS_ANA_MASK                            (0x1U << GMAC_AN_STATUS_ANA_SHIFT)                           /* 0x00000008 */
#define GMAC_AN_STATUS_ANC_SHIFT                           (5U)
#define GMAC_AN_STATUS_ANC_MASK                            (0x1U << GMAC_AN_STATUS_ANC_SHIFT)                           /* 0x00000020 */
/* AN_ADV */
#define GMAC_AN_ADV_OFFSET                                 (0xC8U)
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
#define GMAC_AN_LINK_PART_AB_OFFSET                        (0xCCU)
#define GMAC_AN_LINK_PART_AB                               (0x0U)
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
#define GMAC_AN_EXP_OFFSET                                 (0xD0U)
#define GMAC_AN_EXP                                        (0x0U)
#define GMAC_AN_EXP_NPR_SHIFT                              (1U)
#define GMAC_AN_EXP_NPR_MASK                               (0x1U << GMAC_AN_EXP_NPR_SHIFT)                              /* 0x00000002 */
#define GMAC_AN_EXP_NPA_SHIFT                              (2U)
#define GMAC_AN_EXP_NPA_MASK                               (0x1U << GMAC_AN_EXP_NPA_SHIFT)                              /* 0x00000004 */
/* INTF_MODE_STA */
#define GMAC_INTF_MODE_STA_OFFSET                          (0xD8U)
#define GMAC_INTF_MODE_STA_LM_SHIFT                        (0U)
#define GMAC_INTF_MODE_STA_LM_MASK                         (0x1U << GMAC_INTF_MODE_STA_LM_SHIFT)                        /* 0x00000001 */
#define GMAC_INTF_MODE_STA_LSD_SHIFT                       (1U)
#define GMAC_INTF_MODE_STA_LSD_MASK                        (0x3U << GMAC_INTF_MODE_STA_LSD_SHIFT)                       /* 0x00000006 */
#define GMAC_INTF_MODE_STA_LST_SHIFT                       (3U)
#define GMAC_INTF_MODE_STA_LST_MASK                        (0x1U << GMAC_INTF_MODE_STA_LST_SHIFT)                       /* 0x00000008 */
/* MMC_CTRL */
#define GMAC_MMC_CTRL_OFFSET                               (0x100U)
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
#define GMAC_MMC_RX_INTR_OFFSET                            (0x104U)
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
#define GMAC_MMC_TX_INTR_OFFSET                            (0x108U)
#define GMAC_MMC_TX_INTR                                   (0x0U)
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
#define GMAC_MMC_RX_INT_MSK_OFFSET                         (0x10CU)
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
#define GMAC_MMC_TX_INT_MSK_OFFSET                         (0x110U)
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
#define GMAC_MMC_TXOCTETCNT_GB_OFFSET                      (0x114U)
#define GMAC_MMC_TXOCTETCNT_GB_TXOCTETCOUNT_GB_SHIFT       (0U)
#define GMAC_MMC_TXOCTETCNT_GB_TXOCTETCOUNT_GB_MASK        (0xFFFFFFFFU << GMAC_MMC_TXOCTETCNT_GB_TXOCTETCOUNT_GB_SHIFT) /* 0xFFFFFFFF */
/* MMC_TXFRMCNT_GB */
#define GMAC_MMC_TXFRMCNT_GB_OFFSET                        (0x118U)
#define GMAC_MMC_TXFRMCNT_GB_TXFRAMECOUNT_GB_SHIFT         (0U)
#define GMAC_MMC_TXFRMCNT_GB_TXFRAMECOUNT_GB_MASK          (0xFFFFFFFFU << GMAC_MMC_TXFRMCNT_GB_TXFRAMECOUNT_GB_SHIFT)  /* 0xFFFFFFFF */
/* MMC_TXUNDFLWERR */
#define GMAC_MMC_TXUNDFLWERR_OFFSET                        (0x148U)
#define GMAC_MMC_TXUNDFLWERR_TXUNDERFLOWERROR_SHIFT        (0U)
#define GMAC_MMC_TXUNDFLWERR_TXUNDERFLOWERROR_MASK         (0xFFFFFFFFU << GMAC_MMC_TXUNDFLWERR_TXUNDERFLOWERROR_SHIFT) /* 0xFFFFFFFF */
/* MMC_TXCARERR */
#define GMAC_MMC_TXCARERR_OFFSET                           (0x160U)
#define GMAC_MMC_TXCARERR_TXCARRIERERROR_SHIFT             (0U)
#define GMAC_MMC_TXCARERR_TXCARRIERERROR_MASK              (0xFFFFFFFFU << GMAC_MMC_TXCARERR_TXCARRIERERROR_SHIFT)      /* 0xFFFFFFFF */
/* MMC_TXOCTETCNT_G */
#define GMAC_MMC_TXOCTETCNT_G_OFFSET                       (0x164U)
#define GMAC_MMC_TXOCTETCNT_G_TXOCTETCOUNT_G_SHIFT         (0U)
#define GMAC_MMC_TXOCTETCNT_G_TXOCTETCOUNT_G_MASK          (0xFFFFFFFFU << GMAC_MMC_TXOCTETCNT_G_TXOCTETCOUNT_G_SHIFT)  /* 0xFFFFFFFF */
/* MMC_TXFRMCNT_G */
#define GMAC_MMC_TXFRMCNT_G_OFFSET                         (0x168U)
#define GMAC_MMC_TXFRMCNT_G_TXFRAMECOUNT_G_SHIFT           (0U)
#define GMAC_MMC_TXFRMCNT_G_TXFRAMECOUNT_G_MASK            (0xFFFFFFFFU << GMAC_MMC_TXFRMCNT_G_TXFRAMECOUNT_G_SHIFT)    /* 0xFFFFFFFF */
/* MMC_RXFRMCNT_GB */
#define GMAC_MMC_RXFRMCNT_GB_OFFSET                        (0x180U)
#define GMAC_MMC_RXFRMCNT_GB_RXFRAMECOUNT_GB_SHIFT         (0U)
#define GMAC_MMC_RXFRMCNT_GB_RXFRAMECOUNT_GB_MASK          (0xFFFFFFFFU << GMAC_MMC_RXFRMCNT_GB_RXFRAMECOUNT_GB_SHIFT)  /* 0xFFFFFFFF */
/* MMC_RXOCTETCNT_GB */
#define GMAC_MMC_RXOCTETCNT_GB_OFFSET                      (0x184U)
#define GMAC_MMC_RXOCTETCNT_GB_RXOCTETCOUNT_GB_SHIFT       (0U)
#define GMAC_MMC_RXOCTETCNT_GB_RXOCTETCOUNT_GB_MASK        (0xFFFFFFFFU << GMAC_MMC_RXOCTETCNT_GB_RXOCTETCOUNT_GB_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXOCTETCNT_G */
#define GMAC_MMC_RXOCTETCNT_G_OFFSET                       (0x188U)
#define GMAC_MMC_RXOCTETCNT_G_RXOCTETCOUNT_G_SHIFT         (0U)
#define GMAC_MMC_RXOCTETCNT_G_RXOCTETCOUNT_G_MASK          (0xFFFFFFFFU << GMAC_MMC_RXOCTETCNT_G_RXOCTETCOUNT_G_SHIFT)  /* 0xFFFFFFFF */
/* MMC_RXMCFRMCNT_G */
#define GMAC_MMC_RXMCFRMCNT_G_OFFSET                       (0x190U)
#define GMAC_MMC_RXMCFRMCNT_G_RXMULTICASTFRAMES_G_SHIFT    (0U)
#define GMAC_MMC_RXMCFRMCNT_G_RXMULTICASTFRAMES_G_MASK     (0xFFFFFFFFU << GMAC_MMC_RXMCFRMCNT_G_RXMULTICASTFRAMES_G_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXCRCERR */
#define GMAC_MMC_RXCRCERR_OFFSET                           (0x194U)
#define GMAC_MMC_RXCRCERR_RXCRCERROR_SHIFT                 (0U)
#define GMAC_MMC_RXCRCERR_RXCRCERROR_MASK                  (0xFFFFFFFFU << GMAC_MMC_RXCRCERR_RXCRCERROR_SHIFT)          /* 0xFFFFFFFF */
/* MMC_RXLENERR */
#define GMAC_MMC_RXLENERR_OFFSET                           (0x1C8U)
#define GMAC_MMC_RXLENERR_RXLENGTHERROR_SHIFT              (0U)
#define GMAC_MMC_RXLENERR_RXLENGTHERROR_MASK               (0xFFFFFFFFU << GMAC_MMC_RXLENERR_RXLENGTHERROR_SHIFT)       /* 0xFFFFFFFF */
/* MMC_RXFIFOOVRFLW */
#define GMAC_MMC_RXFIFOOVRFLW_OFFSET                       (0x1D4U)
#define GMAC_MMC_RXFIFOOVRFLW_RXFIFOOVERFLOW_SHIFT         (0U)
#define GMAC_MMC_RXFIFOOVRFLW_RXFIFOOVERFLOW_MASK          (0xFFFFFFFFU << GMAC_MMC_RXFIFOOVRFLW_RXFIFOOVERFLOW_SHIFT)  /* 0xFFFFFFFF */
/* MMC_IPC_INT_MSK */
#define GMAC_MMC_IPC_INT_MSK_OFFSET                        (0x200U)
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
#define GMAC_MMC_IPC_INTR_OFFSET                           (0x208U)
#define GMAC_MMC_IPC_INTR                                  (0x0U)
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
#define GMAC_MMC_RXIPV4GFRM_OFFSET                         (0x210U)
#define GMAC_MMC_RXIPV4GFRM_RXIPV4_GD_FRMS_SHIFT           (0U)
#define GMAC_MMC_RXIPV4GFRM_RXIPV4_GD_FRMS_MASK            (0xFFFFFFFFU << GMAC_MMC_RXIPV4GFRM_RXIPV4_GD_FRMS_SHIFT)    /* 0xFFFFFFFF */
/* MMC_RXIPV4HDERRFRM */
#define GMAC_MMC_RXIPV4HDERRFRM_OFFSET                     (0x214U)
#define GMAC_MMC_RXIPV4HDERRFRM_RXIPV4_HDRERR_FRMS_SHIFT   (0U)
#define GMAC_MMC_RXIPV4HDERRFRM_RXIPV4_HDRERR_FRMS_MASK    (0xFFFFFFFFU << GMAC_MMC_RXIPV4HDERRFRM_RXIPV4_HDRERR_FRMS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXIPV6GFRM */
#define GMAC_MMC_RXIPV6GFRM_OFFSET                         (0x224U)
#define GMAC_MMC_RXIPV6GFRM_RXIPV6_GD_FRMS_SHIFT           (0U)
#define GMAC_MMC_RXIPV6GFRM_RXIPV6_GD_FRMS_MASK            (0xFFFFFFFFU << GMAC_MMC_RXIPV6GFRM_RXIPV6_GD_FRMS_SHIFT)    /* 0xFFFFFFFF */
/* MMC_RXIPV6HDERRFRM */
#define GMAC_MMC_RXIPV6HDERRFRM_OFFSET                     (0x228U)
#define GMAC_MMC_RXIPV6HDERRFRM_RXIPV6_HDRERR_FRMS_SHIFT   (0U)
#define GMAC_MMC_RXIPV6HDERRFRM_RXIPV6_HDRERR_FRMS_MASK    (0xFFFFFFFFU << GMAC_MMC_RXIPV6HDERRFRM_RXIPV6_HDRERR_FRMS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXUDPERRFRM */
#define GMAC_MMC_RXUDPERRFRM_OFFSET                        (0x234U)
#define GMAC_MMC_RXUDPERRFRM_RXUDP_ERR_FRMS_SHIFT          (0U)
#define GMAC_MMC_RXUDPERRFRM_RXUDP_ERR_FRMS_MASK           (0xFFFFFFFFU << GMAC_MMC_RXUDPERRFRM_RXUDP_ERR_FRMS_SHIFT)   /* 0xFFFFFFFF */
/* MMC_RXTCPERRFRM */
#define GMAC_MMC_RXTCPERRFRM_OFFSET                        (0x23CU)
#define GMAC_MMC_RXTCPERRFRM_RXTCP_ERR_FRMS_SHIFT          (0U)
#define GMAC_MMC_RXTCPERRFRM_RXTCP_ERR_FRMS_MASK           (0xFFFFFFFFU << GMAC_MMC_RXTCPERRFRM_RXTCP_ERR_FRMS_SHIFT)   /* 0xFFFFFFFF */
/* MMC_RXICMPERRFRM */
#define GMAC_MMC_RXICMPERRFRM_OFFSET                       (0x244U)
#define GMAC_MMC_RXICMPERRFRM_RXICMP_ERR_FRMS_SHIFT        (0U)
#define GMAC_MMC_RXICMPERRFRM_RXICMP_ERR_FRMS_MASK         (0xFFFFFFFFU << GMAC_MMC_RXICMPERRFRM_RXICMP_ERR_FRMS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXIPV4HDERROCT */
#define GMAC_MMC_RXIPV4HDERROCT_OFFSET                     (0x254U)
#define GMAC_MMC_RXIPV4HDERROCT_RXIPV4_HDRERR_OCTETS_SHIFT (0U)
#define GMAC_MMC_RXIPV4HDERROCT_RXIPV4_HDRERR_OCTETS_MASK  (0xFFFFFFFFU << GMAC_MMC_RXIPV4HDERROCT_RXIPV4_HDRERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXIPV6HDERROCT */
#define GMAC_MMC_RXIPV6HDERROCT_OFFSET                     (0x268U)
#define GMAC_MMC_RXIPV6HDERROCT_RXIPV6_HDRERR_OCTETS_SHIFT (0U)
#define GMAC_MMC_RXIPV6HDERROCT_RXIPV6_HDRERR_OCTETS_MASK  (0xFFFFFFFFU << GMAC_MMC_RXIPV6HDERROCT_RXIPV6_HDRERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXUDPERROCT */
#define GMAC_MMC_RXUDPERROCT_OFFSET                        (0x274U)
#define GMAC_MMC_RXUDPERROCT_RXUDP_ERR_OCTETS_SHIFT        (0U)
#define GMAC_MMC_RXUDPERROCT_RXUDP_ERR_OCTETS_MASK         (0xFFFFFFFFU << GMAC_MMC_RXUDPERROCT_RXUDP_ERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXTCPERROCT */
#define GMAC_MMC_RXTCPERROCT_OFFSET                        (0x27CU)
#define GMAC_MMC_RXTCPERROCT_RXTCP_ERR_OCTETS_SHIFT        (0U)
#define GMAC_MMC_RXTCPERROCT_RXTCP_ERR_OCTETS_MASK         (0xFFFFFFFFU << GMAC_MMC_RXTCPERROCT_RXTCP_ERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* MMC_RXICMPERROCT */
#define GMAC_MMC_RXICMPERROCT_OFFSET                       (0x284U)
#define GMAC_MMC_RXICMPERROCT_RXICMP_ERR_OCTETS_SHIFT      (0U)
#define GMAC_MMC_RXICMPERROCT_RXICMP_ERR_OCTETS_MASK       (0xFFFFFFFFU << GMAC_MMC_RXICMPERROCT_RXICMP_ERR_OCTETS_SHIFT) /* 0xFFFFFFFF */
/* BUS_MODE */
#define GMAC_BUS_MODE_OFFSET                               (0x1000U)
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
#define GMAC_TX_POLL_DEMAND_OFFSET                         (0x1004U)
#define GMAC_TX_POLL_DEMAND                                (0x0U)
#define GMAC_TX_POLL_DEMAND_TPD_SHIFT                      (0U)
#define GMAC_TX_POLL_DEMAND_TPD_MASK                       (0xFFFFFFFFU << GMAC_TX_POLL_DEMAND_TPD_SHIFT)               /* 0xFFFFFFFF */
/* RX_POLL_DEMAND */
#define GMAC_RX_POLL_DEMAND_OFFSET                         (0x1008U)
#define GMAC_RX_POLL_DEMAND                                (0x0U)
#define GMAC_RX_POLL_DEMAND_RPD_SHIFT                      (0U)
#define GMAC_RX_POLL_DEMAND_RPD_MASK                       (0xFFFFFFFFU << GMAC_RX_POLL_DEMAND_RPD_SHIFT)               /* 0xFFFFFFFF */
/* RX_DESC_LIST_ADDR */
#define GMAC_RX_DESC_LIST_ADDR_OFFSET                      (0x100CU)
#define GMAC_RX_DESC_LIST_ADDR_SRL_SHIFT                   (0U)
#define GMAC_RX_DESC_LIST_ADDR_SRL_MASK                    (0xFFFFFFFFU << GMAC_RX_DESC_LIST_ADDR_SRL_SHIFT)            /* 0xFFFFFFFF */
/* TX_DESC_LIST_ADDR */
#define GMAC_TX_DESC_LIST_ADDR_OFFSET                      (0x1010U)
#define GMAC_TX_DESC_LIST_ADDR_STL_SHIFT                   (0U)
#define GMAC_TX_DESC_LIST_ADDR_STL_MASK                    (0xFFFFFFFFU << GMAC_TX_DESC_LIST_ADDR_STL_SHIFT)            /* 0xFFFFFFFF */
/* STATUS */
#define GMAC_STATUS_OFFSET                                 (0x1014U)
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
#define GMAC_OP_MODE_OFFSET                                (0x1018U)
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
#define GMAC_INT_ENA_OFFSET                                (0x101CU)
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
#define GMAC_OVERFLOW_CNT_OFFSET                           (0x1020U)
#define GMAC_OVERFLOW_CNT                                  (0x0U)
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_2_SHIFT        (0U)
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_2_MASK         (0xFFFFU << GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_2_SHIFT)     /* 0x0000FFFF */
#define GMAC_OVERFLOW_CNT_MISS_FRAME_OVERFLOW_BIT_SHIFT    (16U)
#define GMAC_OVERFLOW_CNT_MISS_FRAME_OVERFLOW_BIT_MASK     (0x1U << GMAC_OVERFLOW_CNT_MISS_FRAME_OVERFLOW_BIT_SHIFT)    /* 0x00010000 */
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_SHIFT          (17U)
#define GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_MASK           (0x7FFU << GMAC_OVERFLOW_CNT_FRAME_MISS_NUMBER_SHIFT)        /* 0x0FFE0000 */
#define GMAC_OVERFLOW_CNT_FIFO_OVERFLOW_BIT_SHIFT          (28U)
#define GMAC_OVERFLOW_CNT_FIFO_OVERFLOW_BIT_MASK           (0x1U << GMAC_OVERFLOW_CNT_FIFO_OVERFLOW_BIT_SHIFT)          /* 0x10000000 */
/* REC_INT_WDT_TIMER */
#define GMAC_REC_INT_WDT_TIMER_OFFSET                      (0x1024U)
#define GMAC_REC_INT_WDT_TIMER_RIWT_SHIFT                  (0U)
#define GMAC_REC_INT_WDT_TIMER_RIWT_MASK                   (0xFFU << GMAC_REC_INT_WDT_TIMER_RIWT_SHIFT)                 /* 0x000000FF */
/* AXI_BUS_MODE */
#define GMAC_AXI_BUS_MODE_OFFSET                           (0x1028U)
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
#define GMAC_AXI_STATUS_OFFSET                             (0x102CU)
#define GMAC_AXI_STATUS                                    (0x0U)
#define GMAC_AXI_STATUS_WR_CH_STA_SHIFT                    (0U)
#define GMAC_AXI_STATUS_WR_CH_STA_MASK                     (0x1U << GMAC_AXI_STATUS_WR_CH_STA_SHIFT)                    /* 0x00000001 */
#define GMAC_AXI_STATUS_RD_CH_STA_SHIFT                    (1U)
#define GMAC_AXI_STATUS_RD_CH_STA_MASK                     (0x1U << GMAC_AXI_STATUS_RD_CH_STA_SHIFT)                    /* 0x00000002 */
/* CUR_HOST_TX_DESC */
#define GMAC_CUR_HOST_TX_DESC_OFFSET                       (0x1048U)
#define GMAC_CUR_HOST_TX_DESC                              (0x0U)
#define GMAC_CUR_HOST_TX_DESC_HTDAP_SHIFT                  (0U)
#define GMAC_CUR_HOST_TX_DESC_HTDAP_MASK                   (0xFFFFFFFFU << GMAC_CUR_HOST_TX_DESC_HTDAP_SHIFT)           /* 0xFFFFFFFF */
/* CUR_HOST_RX_DESC */
#define GMAC_CUR_HOST_RX_DESC_OFFSET                       (0x104CU)
#define GMAC_CUR_HOST_RX_DESC                              (0x0U)
#define GMAC_CUR_HOST_RX_DESC_HRDAP_SHIFT                  (0U)
#define GMAC_CUR_HOST_RX_DESC_HRDAP_MASK                   (0xFFFFFFFFU << GMAC_CUR_HOST_RX_DESC_HRDAP_SHIFT)           /* 0xFFFFFFFF */
/* CUR_HOST_TX_BUF_ADDR */
#define GMAC_CUR_HOST_TX_BUF_ADDR_OFFSET                   (0x1050U)
#define GMAC_CUR_HOST_TX_BUF_ADDR                          (0x0U)
#define GMAC_CUR_HOST_TX_BUF_ADDR_HTBAP_SHIFT              (0U)
#define GMAC_CUR_HOST_TX_BUF_ADDR_HTBAP_MASK               (0xFFFFFFFFU << GMAC_CUR_HOST_TX_BUF_ADDR_HTBAP_SHIFT)       /* 0xFFFFFFFF */
/* CUR_HOST_RX_BUF_ADDR */
#define GMAC_CUR_HOST_RX_BUF_ADDR_OFFSET                   (0x1054U)
#define GMAC_CUR_HOST_RX_BUF_ADDR                          (0x0U)
#define GMAC_CUR_HOST_RX_BUF_ADDR_HRBAP_SHIFT              (0U)
#define GMAC_CUR_HOST_RX_BUF_ADDR_HRBAP_MASK               (0xFFFFFFFFU << GMAC_CUR_HOST_RX_BUF_ADDR_HRBAP_SHIFT)       /* 0xFFFFFFFF */
/******************************************MMC*******************************************/
/* CTRL */
#define MMC_CTRL_OFFSET                                    (0x0U)
#define MMC_CTRL_CONTROLLER_RESET_SHIFT                    (0U)
#define MMC_CTRL_CONTROLLER_RESET_MASK                     (0x1U << MMC_CTRL_CONTROLLER_RESET_SHIFT)                    /* 0x00000001 */
#define MMC_CTRL_FIFO_RESET_SHIFT                          (1U)
#define MMC_CTRL_FIFO_RESET_MASK                           (0x1U << MMC_CTRL_FIFO_RESET_SHIFT)                          /* 0x00000002 */
#define MMC_CTRL_DMA_RESET_SHIFT                           (2U)
#define MMC_CTRL_DMA_RESET_MASK                            (0x1U << MMC_CTRL_DMA_RESET_SHIFT)                           /* 0x00000004 */
#define MMC_CTRL_INT_ENABLE_SHIFT                          (4U)
#define MMC_CTRL_INT_ENABLE_MASK                           (0x1U << MMC_CTRL_INT_ENABLE_SHIFT)                          /* 0x00000010 */
#define MMC_CTRL_DMA_ENABLE_SHIFT                          (5U)
#define MMC_CTRL_DMA_ENABLE_MASK                           (0x1U << MMC_CTRL_DMA_ENABLE_SHIFT)                          /* 0x00000020 */
#define MMC_CTRL_READ_WAIT_SHIFT                           (6U)
#define MMC_CTRL_READ_WAIT_MASK                            (0x1U << MMC_CTRL_READ_WAIT_SHIFT)                           /* 0x00000040 */
#define MMC_CTRL_SEND_IRQ_RESPONSE_SHIFT                   (7U)
#define MMC_CTRL_SEND_IRQ_RESPONSE_MASK                    (0x1U << MMC_CTRL_SEND_IRQ_RESPONSE_SHIFT)                   /* 0x00000080 */
#define MMC_CTRL_ABORT_READ_DATA_SHIFT                     (8U)
#define MMC_CTRL_ABORT_READ_DATA_MASK                      (0x1U << MMC_CTRL_ABORT_READ_DATA_SHIFT)                     /* 0x00000100 */
#define MMC_CTRL_SEND_CCSD_SHIFT                           (9U)
#define MMC_CTRL_SEND_CCSD_MASK                            (0x1U << MMC_CTRL_SEND_CCSD_SHIFT)                           /* 0x00000200 */
#define MMC_CTRL_SEND_AUTO_STOP_CCSD_SHIFT                 (10U)
#define MMC_CTRL_SEND_AUTO_STOP_CCSD_MASK                  (0x1U << MMC_CTRL_SEND_AUTO_STOP_CCSD_SHIFT)                 /* 0x00000400 */
#define MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS_SHIFT       (11U)
#define MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS_MASK        (0x1U << MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS_SHIFT)       /* 0x00000800 */
#define MMC_CTRL_CARD_VOLTAGE_A_SHIFT                      (16U)
#define MMC_CTRL_CARD_VOLTAGE_A_MASK                       (0xFU << MMC_CTRL_CARD_VOLTAGE_A_SHIFT)                      /* 0x000F0000 */
#define MMC_CTRL_CARD_VOLTAGE_B_SHIFT                      (20U)
#define MMC_CTRL_CARD_VOLTAGE_B_MASK                       (0xFU << MMC_CTRL_CARD_VOLTAGE_B_SHIFT)                      /* 0x00F00000 */
#define MMC_CTRL_ENABLE_OD_PULLUP_SHIFT                    (24U)
#define MMC_CTRL_ENABLE_OD_PULLUP_MASK                     (0x1U << MMC_CTRL_ENABLE_OD_PULLUP_SHIFT)                    /* 0x01000000 */
#define MMC_CTRL_USE_INTERNAL_DMAC_SHIFT                   (25U)
#define MMC_CTRL_USE_INTERNAL_DMAC_MASK                    (0x1U << MMC_CTRL_USE_INTERNAL_DMAC_SHIFT)                   /* 0x02000000 */
/* PWREN */
#define MMC_PWREN_OFFSET                                   (0x4U)
#define MMC_PWREN_POWER_ENABLE_SHIFT                       (0U)
#define MMC_PWREN_POWER_ENABLE_MASK                        (0x1U << MMC_PWREN_POWER_ENABLE_SHIFT)                       /* 0x00000001 */
/* CLKDIV */
#define MMC_CLKDIV_OFFSET                                  (0x8U)
#define MMC_CLKDIV_CLK_DIVIDER0_SHIFT                      (0U)
#define MMC_CLKDIV_CLK_DIVIDER0_MASK                       (0xFFU << MMC_CLKDIV_CLK_DIVIDER0_SHIFT)                     /* 0x000000FF */
#define MMC_CLKDIV_CLK_DIVIDER1_SHIFT                      (8U)
#define MMC_CLKDIV_CLK_DIVIDER1_MASK                       (0xFFU << MMC_CLKDIV_CLK_DIVIDER1_SHIFT)                     /* 0x0000FF00 */
#define MMC_CLKDIV_CLK_DIVIDER2_SHIFT                      (16U)
#define MMC_CLKDIV_CLK_DIVIDER2_MASK                       (0xFFU << MMC_CLKDIV_CLK_DIVIDER2_SHIFT)                     /* 0x00FF0000 */
#define MMC_CLKDIV_CLK_DIVIDER3_SHIFT                      (24U)
#define MMC_CLKDIV_CLK_DIVIDER3_MASK                       (0xFFU << MMC_CLKDIV_CLK_DIVIDER3_SHIFT)                     /* 0xFF000000 */
/* CLKSRC */
#define MMC_CLKSRC_OFFSET                                  (0xCU)
#define MMC_CLKSRC_CLK_SOURCE_SHIFT                        (0U)
#define MMC_CLKSRC_CLK_SOURCE_MASK                         (0x3U << MMC_CLKSRC_CLK_SOURCE_SHIFT)                        /* 0x00000003 */
/* CLKENA */
#define MMC_CLKENA_OFFSET                                  (0x10U)
#define MMC_CLKENA_CCLK_ENABLE_SHIFT                       (0U)
#define MMC_CLKENA_CCLK_ENABLE_MASK                        (0x1U << MMC_CLKENA_CCLK_ENABLE_SHIFT)                       /* 0x00000001 */
#define MMC_CLKENA_CCLK_LOW_POWER_SHIFT                    (16U)
#define MMC_CLKENA_CCLK_LOW_POWER_MASK                     (0x1U << MMC_CLKENA_CCLK_LOW_POWER_SHIFT)                    /* 0x00010000 */
/* TMOUT */
#define MMC_TMOUT_OFFSET                                   (0x14U)
#define MMC_TMOUT_RESPONSE_TIMEOUT_SHIFT                   (0U)
#define MMC_TMOUT_RESPONSE_TIMEOUT_MASK                    (0xFFU << MMC_TMOUT_RESPONSE_TIMEOUT_SHIFT)                  /* 0x000000FF */
#define MMC_TMOUT_DATA_TIMEOUT_SHIFT                       (8U)
#define MMC_TMOUT_DATA_TIMEOUT_MASK                        (0xFFFFFFU << MMC_TMOUT_DATA_TIMEOUT_SHIFT)                  /* 0xFFFFFF00 */
/* CTYPE */
#define MMC_CTYPE_OFFSET                                   (0x18U)
#define MMC_CTYPE_CARD_WIDTH_SHIFT                         (0U)
#define MMC_CTYPE_CARD_WIDTH_MASK                          (0x1U << MMC_CTYPE_CARD_WIDTH_SHIFT)                         /* 0x00000001 */
#define MMC_CTYPE_CARD_WIDTH_8_SHIFT                       (16U)
#define MMC_CTYPE_CARD_WIDTH_8_MASK                        (0x1U << MMC_CTYPE_CARD_WIDTH_8_SHIFT)                       /* 0x00010000 */
/* BLKSIZ */
#define MMC_BLKSIZ_OFFSET                                  (0x1CU)
#define MMC_BLKSIZ_BLOCK_SIZE_SHIFT                        (0U)
#define MMC_BLKSIZ_BLOCK_SIZE_MASK                         (0xFFFFU << MMC_BLKSIZ_BLOCK_SIZE_SHIFT)                     /* 0x0000FFFF */
/* BYTCNT */
#define MMC_BYTCNT_OFFSET                                  (0x20U)
#define MMC_BYTCNT_BYTE_COUNT_SHIFT                        (0U)
#define MMC_BYTCNT_BYTE_COUNT_MASK                         (0xFFFFFFFFU << MMC_BYTCNT_BYTE_COUNT_SHIFT)                 /* 0xFFFFFFFF */
/* INTMASK */
#define MMC_INTMASK_OFFSET                                 (0x24U)
#define MMC_INTMASK_INT_MASK_SHIFT                         (0U)
#define MMC_INTMASK_INT_MASK_MASK                          (0xFFFFU << MMC_INTMASK_INT_MASK_SHIFT)                      /* 0x0000FFFF */
#define MMC_INTMASK_DATA_NOBUSY_INT_MASK_SHIFT             (16U)
#define MMC_INTMASK_DATA_NOBUSY_INT_MASK_MASK              (0x1U << MMC_INTMASK_DATA_NOBUSY_INT_MASK_SHIFT)             /* 0x00010000 */
#define MMC_INTMASK_SDIO_INT_MASK_SHIFT                    (24U)
#define MMC_INTMASK_SDIO_INT_MASK_MASK                     (0x1U << MMC_INTMASK_SDIO_INT_MASK_SHIFT)                    /* 0x01000000 */
/* CMDARG */
#define MMC_CMDARG_OFFSET                                  (0x28U)
#define MMC_CMDARG_CMD_ARG_SHIFT                           (0U)
#define MMC_CMDARG_CMD_ARG_MASK                            (0xFFFFFFFFU << MMC_CMDARG_CMD_ARG_SHIFT)                    /* 0xFFFFFFFF */
/* CMD */
#define MMC_CMD_OFFSET                                     (0x2CU)
#define MMC_CMD_CMD_INDEX_SHIFT                            (0U)
#define MMC_CMD_CMD_INDEX_MASK                             (0x3FU << MMC_CMD_CMD_INDEX_SHIFT)                           /* 0x0000003F */
#define MMC_CMD_RESPONSE_EXPECT_SHIFT                      (6U)
#define MMC_CMD_RESPONSE_EXPECT_MASK                       (0x1U << MMC_CMD_RESPONSE_EXPECT_SHIFT)                      /* 0x00000040 */
#define MMC_CMD_RESPONSE_LENGTH_SHIFT                      (7U)
#define MMC_CMD_RESPONSE_LENGTH_MASK                       (0x1U << MMC_CMD_RESPONSE_LENGTH_SHIFT)                      /* 0x00000080 */
#define MMC_CMD_CHECK_RESPONSE_CRC_SHIFT                   (8U)
#define MMC_CMD_CHECK_RESPONSE_CRC_MASK                    (0x1U << MMC_CMD_CHECK_RESPONSE_CRC_SHIFT)                   /* 0x00000100 */
#define MMC_CMD_DATA_EXPECTED_SHIFT                        (9U)
#define MMC_CMD_DATA_EXPECTED_MASK                         (0x1U << MMC_CMD_DATA_EXPECTED_SHIFT)                        /* 0x00000200 */
#define MMC_CMD_WR_SHIFT                                   (10U)
#define MMC_CMD_WR_MASK                                    (0x1U << MMC_CMD_WR_SHIFT)                                   /* 0x00000400 */
#define MMC_CMD_TRANSFER_MODE_SHIFT                        (11U)
#define MMC_CMD_TRANSFER_MODE_MASK                         (0x1U << MMC_CMD_TRANSFER_MODE_SHIFT)                        /* 0x00000800 */
#define MMC_CMD_SEND_AUTO_STOP_SHIFT                       (12U)
#define MMC_CMD_SEND_AUTO_STOP_MASK                        (0x1U << MMC_CMD_SEND_AUTO_STOP_SHIFT)                       /* 0x00001000 */
#define MMC_CMD_WAIT_PRVDATA_COMPLETE_SHIFT                (13U)
#define MMC_CMD_WAIT_PRVDATA_COMPLETE_MASK                 (0x1U << MMC_CMD_WAIT_PRVDATA_COMPLETE_SHIFT)                /* 0x00002000 */
#define MMC_CMD_STOP_ABORT_CMD_SHIFT                       (14U)
#define MMC_CMD_STOP_ABORT_CMD_MASK                        (0x1U << MMC_CMD_STOP_ABORT_CMD_SHIFT)                       /* 0x00004000 */
#define MMC_CMD_SEND_INITIALIZATION_SHIFT                  (15U)
#define MMC_CMD_SEND_INITIALIZATION_MASK                   (0x1U << MMC_CMD_SEND_INITIALIZATION_SHIFT)                  /* 0x00008000 */
#define MMC_CMD_CARD_NUMBER_SHIFT                          (16U)
#define MMC_CMD_CARD_NUMBER_MASK                           (0x1FU << MMC_CMD_CARD_NUMBER_SHIFT)                         /* 0x001F0000 */
#define MMC_CMD_UPDATE_CLOCK_REGISTERS_ONLY_SHIFT          (21U)
#define MMC_CMD_UPDATE_CLOCK_REGISTERS_ONLY_MASK           (0x1U << MMC_CMD_UPDATE_CLOCK_REGISTERS_ONLY_SHIFT)          /* 0x00200000 */
#define MMC_CMD_READ_CEATA_DEVICE_SHIFT                    (22U)
#define MMC_CMD_READ_CEATA_DEVICE_MASK                     (0x1U << MMC_CMD_READ_CEATA_DEVICE_SHIFT)                    /* 0x00400000 */
#define MMC_CMD_CCS_EXPECTED_SHIFT                         (23U)
#define MMC_CMD_CCS_EXPECTED_MASK                          (0x1U << MMC_CMD_CCS_EXPECTED_SHIFT)                         /* 0x00800000 */
#define MMC_CMD_ENABLE_BOOT_SHIFT                          (24U)
#define MMC_CMD_ENABLE_BOOT_MASK                           (0x1U << MMC_CMD_ENABLE_BOOT_SHIFT)                          /* 0x01000000 */
#define MMC_CMD_EXPECT_BOOT_ACK_SHIFT                      (25U)
#define MMC_CMD_EXPECT_BOOT_ACK_MASK                       (0x1U << MMC_CMD_EXPECT_BOOT_ACK_SHIFT)                      /* 0x02000000 */
#define MMC_CMD_DISABLE_BOOT_SHIFT                         (26U)
#define MMC_CMD_DISABLE_BOOT_MASK                          (0x1U << MMC_CMD_DISABLE_BOOT_SHIFT)                         /* 0x04000000 */
#define MMC_CMD_BOOT_MODE_SHIFT                            (27U)
#define MMC_CMD_BOOT_MODE_MASK                             (0x1U << MMC_CMD_BOOT_MODE_SHIFT)                            /* 0x08000000 */
#define MMC_CMD_VOLT_SWITCH_SHIFT                          (28U)
#define MMC_CMD_VOLT_SWITCH_MASK                           (0x1U << MMC_CMD_VOLT_SWITCH_SHIFT)                          /* 0x10000000 */
#define MMC_CMD_USE_HOLD_REG_SHIFT                         (29U)
#define MMC_CMD_USE_HOLD_REG_MASK                          (0x1U << MMC_CMD_USE_HOLD_REG_SHIFT)                         /* 0x20000000 */
#define MMC_CMD_START_CMD_SHIFT                            (31U)
#define MMC_CMD_START_CMD_MASK                             (0x1U << MMC_CMD_START_CMD_SHIFT)                            /* 0x80000000 */
/* RESP0 */
#define MMC_RESP0_OFFSET                                   (0x30U)
#define MMC_RESP0                                          (0x0U)
#define MMC_RESP0_RESPONSE0_SHIFT                          (0U)
#define MMC_RESP0_RESPONSE0_MASK                           (0xFFFFFFFFU << MMC_RESP0_RESPONSE0_SHIFT)                   /* 0xFFFFFFFF */
/* RESP1 */
#define MMC_RESP1_OFFSET                                   (0x34U)
#define MMC_RESP1                                          (0x0U)
#define MMC_RESP1_RESPONSE_SHIFT                           (0U)
#define MMC_RESP1_RESPONSE_MASK                            (0xFFFFFFFFU << MMC_RESP1_RESPONSE_SHIFT)                    /* 0xFFFFFFFF */
/* RESP2 */
#define MMC_RESP2_OFFSET                                   (0x38U)
#define MMC_RESP2                                          (0x0U)
#define MMC_RESP2_RESPONSE2_SHIFT                          (0U)
#define MMC_RESP2_RESPONSE2_MASK                           (0xFFFFFFFFU << MMC_RESP2_RESPONSE2_SHIFT)                   /* 0xFFFFFFFF */
/* RESP3 */
#define MMC_RESP3_OFFSET                                   (0x3CU)
#define MMC_RESP3                                          (0x0U)
#define MMC_RESP3_RESPONSE3_SHIFT                          (0U)
#define MMC_RESP3_RESPONSE3_MASK                           (0xFFFFFFFFU << MMC_RESP3_RESPONSE3_SHIFT)                   /* 0xFFFFFFFF */
/* MINTSTS */
#define MMC_MINTSTS_OFFSET                                 (0x40U)
#define MMC_MINTSTS_INT_STATUS_SHIFT                       (0U)
#define MMC_MINTSTS_INT_STATUS_MASK                        (0xFFFFU << MMC_MINTSTS_INT_STATUS_SHIFT)                    /* 0x0000FFFF */
#define MMC_MINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT           (16U)
#define MMC_MINTSTS_DATA_NOBUSY_INT_STATUS_MASK            (0x1U << MMC_MINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT)           /* 0x00010000 */
#define MMC_MINTSTS_SDIO_INTERRUPT_SHIFT                   (24U)
#define MMC_MINTSTS_SDIO_INTERRUPT_MASK                    (0x1U << MMC_MINTSTS_SDIO_INTERRUPT_SHIFT)                   /* 0x01000000 */
/* RINTSTS */
#define MMC_RINTSTS_OFFSET                                 (0x44U)
#define MMC_RINTSTS_INT_STATUS_SHIFT                       (0U)
#define MMC_RINTSTS_INT_STATUS_MASK                        (0xFFFFU << MMC_RINTSTS_INT_STATUS_SHIFT)                    /* 0x0000FFFF */
#define MMC_RINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT           (16U)
#define MMC_RINTSTS_DATA_NOBUSY_INT_STATUS_MASK            (0x1U << MMC_RINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT)           /* 0x00010000 */
#define MMC_RINTSTS_SDIO_INTERRUPT_SHIFT                   (24U)
#define MMC_RINTSTS_SDIO_INTERRUPT_MASK                    (0x1U << MMC_RINTSTS_SDIO_INTERRUPT_SHIFT)                   /* 0x01000000 */
/* STATUS */
#define MMC_STATUS_OFFSET                                  (0x48U)
#define MMC_STATUS                                         (0x406U)
#define MMC_STATUS_FIFO_RX_WATERMARK_SHIFT                 (0U)
#define MMC_STATUS_FIFO_RX_WATERMARK_MASK                  (0x1U << MMC_STATUS_FIFO_RX_WATERMARK_SHIFT)                 /* 0x00000001 */
#define MMC_STATUS_FIFO_TX_WATERMARK_SHIFT                 (1U)
#define MMC_STATUS_FIFO_TX_WATERMARK_MASK                  (0x1U << MMC_STATUS_FIFO_TX_WATERMARK_SHIFT)                 /* 0x00000002 */
#define MMC_STATUS_FIFO_EMPTY_SHIFT                        (2U)
#define MMC_STATUS_FIFO_EMPTY_MASK                         (0x1U << MMC_STATUS_FIFO_EMPTY_SHIFT)                        /* 0x00000004 */
#define MMC_STATUS_FIFO_FULL_SHIFT                         (3U)
#define MMC_STATUS_FIFO_FULL_MASK                          (0x1U << MMC_STATUS_FIFO_FULL_SHIFT)                         /* 0x00000008 */
#define MMC_STATUS_COMMAND_FSM_STATES_SHIFT                (4U)
#define MMC_STATUS_COMMAND_FSM_STATES_MASK                 (0xFU << MMC_STATUS_COMMAND_FSM_STATES_SHIFT)                /* 0x000000F0 */
#define MMC_STATUS_DATA_3_STATUS_SHIFT                     (8U)
#define MMC_STATUS_DATA_3_STATUS_MASK                      (0x1U << MMC_STATUS_DATA_3_STATUS_SHIFT)                     /* 0x00000100 */
#define MMC_STATUS_DATA_BUSY_SHIFT                         (9U)
#define MMC_STATUS_DATA_BUSY_MASK                          (0x1U << MMC_STATUS_DATA_BUSY_SHIFT)                         /* 0x00000200 */
#define MMC_STATUS_DATA_STATE_MC_BUSY_SHIFT                (10U)
#define MMC_STATUS_DATA_STATE_MC_BUSY_MASK                 (0x1U << MMC_STATUS_DATA_STATE_MC_BUSY_SHIFT)                /* 0x00000400 */
#define MMC_STATUS_RESPONSE_INDEX_SHIFT                    (11U)
#define MMC_STATUS_RESPONSE_INDEX_MASK                     (0x3FU << MMC_STATUS_RESPONSE_INDEX_SHIFT)                   /* 0x0001F800 */
#define MMC_STATUS_FIFO_COUNT_SHIFT                        (17U)
#define MMC_STATUS_FIFO_COUNT_MASK                         (0x1FFFU << MMC_STATUS_FIFO_COUNT_SHIFT)                     /* 0x3FFE0000 */
#define MMC_STATUS_DMA_ACK_SHIFT                           (30U)
#define MMC_STATUS_DMA_ACK_MASK                            (0x1U << MMC_STATUS_DMA_ACK_SHIFT)                           /* 0x40000000 */
#define MMC_STATUS_DMA_REQ_SHIFT                           (31U)
#define MMC_STATUS_DMA_REQ_MASK                            (0x1U << MMC_STATUS_DMA_REQ_SHIFT)                           /* 0x80000000 */
/* FIFOTH */
#define MMC_FIFOTH_OFFSET                                  (0x4CU)
#define MMC_FIFOTH_TX_WMARK_SHIFT                          (0U)
#define MMC_FIFOTH_TX_WMARK_MASK                           (0xFFFU << MMC_FIFOTH_TX_WMARK_SHIFT)                        /* 0x00000FFF */
#define MMC_FIFOTH_RX_WMARK_SHIFT                          (16U)
#define MMC_FIFOTH_RX_WMARK_MASK                           (0xFFFU << MMC_FIFOTH_RX_WMARK_SHIFT)                        /* 0x0FFF0000 */
#define MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_SHIFT      (28U)
#define MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_MASK       (0x7U << MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_SHIFT)      /* 0x70000000 */
/* CDETECT */
#define MMC_CDETECT_OFFSET                                 (0x50U)
#define MMC_CDETECT                                        (0x0U)
#define MMC_CDETECT_CARD_DETECT_N_SHIFT                    (0U)
#define MMC_CDETECT_CARD_DETECT_N_MASK                     (0x1U << MMC_CDETECT_CARD_DETECT_N_SHIFT)                    /* 0x00000001 */
/* WRTPRT */
#define MMC_WRTPRT_OFFSET                                  (0x54U)
#define MMC_WRTPRT_WRITE_PROTECT_SHIFT                     (0U)
#define MMC_WRTPRT_WRITE_PROTECT_MASK                      (0x1U << MMC_WRTPRT_WRITE_PROTECT_SHIFT)                     /* 0x00000001 */
/* TCBCNT */
#define MMC_TCBCNT_OFFSET                                  (0x5CU)
#define MMC_TCBCNT                                         (0x0U)
#define MMC_TCBCNT_TRANS_CARD_BYTE_COUNT_SHIFT             (0U)
#define MMC_TCBCNT_TRANS_CARD_BYTE_COUNT_MASK              (0xFFFFFFFFU << MMC_TCBCNT_TRANS_CARD_BYTE_COUNT_SHIFT)      /* 0xFFFFFFFF */
/* TBBCNT */
#define MMC_TBBCNT_OFFSET                                  (0x60U)
#define MMC_TBBCNT                                         (0x0U)
#define MMC_TBBCNT_TRANS_FIFO_BYTE_COUNT_SHIFT             (0U)
#define MMC_TBBCNT_TRANS_FIFO_BYTE_COUNT_MASK              (0xFFFFFFFFU << MMC_TBBCNT_TRANS_FIFO_BYTE_COUNT_SHIFT)      /* 0xFFFFFFFF */
/* DEBNCE */
#define MMC_DEBNCE_OFFSET                                  (0x64U)
#define MMC_DEBNCE_DEBOUNCE_COUNT_SHIFT                    (0U)
#define MMC_DEBNCE_DEBOUNCE_COUNT_MASK                     (0xFFFFFFU << MMC_DEBNCE_DEBOUNCE_COUNT_SHIFT)               /* 0x00FFFFFF */
/* USRID */
#define MMC_USRID_OFFSET                                   (0x68U)
#define MMC_USRID_USRID_SHIFT                              (0U)
#define MMC_USRID_USRID_MASK                               (0xFFFFFFFFU << MMC_USRID_USRID_SHIFT)                       /* 0xFFFFFFFF */
/* VERID */
#define MMC_VERID_OFFSET                                   (0x6CU)
#define MMC_VERID                                          (0x5342270AU)
#define MMC_VERID_VERID_SHIFT                              (0U)
#define MMC_VERID_VERID_MASK                               (0xFFFFFFFFU << MMC_VERID_VERID_SHIFT)                       /* 0xFFFFFFFF */
/* HCON */
#define MMC_HCON_OFFSET                                    (0x70U)
#define MMC_HCON                                           (0x0U)
#define MMC_HCON_HCON_SHIFT                                (0U)
#define MMC_HCON_HCON_MASK                                 (0xFFFFFFFFU << MMC_HCON_HCON_SHIFT)                         /* 0xFFFFFFFF */
/* UHS_REG */
#define MMC_UHS_REG_OFFSET                                 (0x74U)
#define MMC_UHS_REG_VOLT_REG_SHIFT                         (0U)
#define MMC_UHS_REG_VOLT_REG_MASK                          (0x1U << MMC_UHS_REG_VOLT_REG_SHIFT)                         /* 0x00000001 */
#define MMC_UHS_REG_DDR_REG_SHIFT                          (16U)
#define MMC_UHS_REG_DDR_REG_MASK                           (0x1U << MMC_UHS_REG_DDR_REG_SHIFT)                          /* 0x00010000 */
/* RST_N */
#define MMC_RST_N_OFFSET                                   (0x78U)
#define MMC_RST_N_CARD_RESET_SHIFT                         (0U)
#define MMC_RST_N_CARD_RESET_MASK                          (0x1U << MMC_RST_N_CARD_RESET_SHIFT)                         /* 0x00000001 */
/* BMOD */
#define MMC_BMOD_OFFSET                                    (0x80U)
#define MMC_BMOD_SWR_SHIFT                                 (0U)
#define MMC_BMOD_SWR_MASK                                  (0x1U << MMC_BMOD_SWR_SHIFT)                                 /* 0x00000001 */
#define MMC_BMOD_FB_SHIFT                                  (1U)
#define MMC_BMOD_FB_MASK                                   (0x1U << MMC_BMOD_FB_SHIFT)                                  /* 0x00000002 */
#define MMC_BMOD_DSL_SHIFT                                 (2U)
#define MMC_BMOD_DSL_MASK                                  (0x1FU << MMC_BMOD_DSL_SHIFT)                                /* 0x0000007C */
#define MMC_BMOD_DE_SHIFT                                  (7U)
#define MMC_BMOD_DE_MASK                                   (0x1U << MMC_BMOD_DE_SHIFT)                                  /* 0x00000080 */
#define MMC_BMOD_PBL_SHIFT                                 (8U)
#define MMC_BMOD_PBL_MASK                                  (0x7U << MMC_BMOD_PBL_SHIFT)                                 /* 0x00000700 */
/* PLDMND */
#define MMC_PLDMND_OFFSET                                  (0x84U)
#define MMC_PLDMND_PD_SHIFT                                (0U)
#define MMC_PLDMND_PD_MASK                                 (0xFFFFFFFFU << MMC_PLDMND_PD_SHIFT)                         /* 0xFFFFFFFF */
/* DBADDR */
#define MMC_DBADDR_OFFSET                                  (0x88U)
#define MMC_DBADDR_SDL_SHIFT                               (0U)
#define MMC_DBADDR_SDL_MASK                                (0xFFFFFFFFU << MMC_DBADDR_SDL_SHIFT)                        /* 0xFFFFFFFF */
/* IDSTS */
#define MMC_IDSTS_OFFSET                                   (0x8CU)
#define MMC_IDSTS_TI_SHIFT                                 (0U)
#define MMC_IDSTS_TI_MASK                                  (0x1U << MMC_IDSTS_TI_SHIFT)                                 /* 0x00000001 */
#define MMC_IDSTS_RI_SHIFT                                 (1U)
#define MMC_IDSTS_RI_MASK                                  (0x1U << MMC_IDSTS_RI_SHIFT)                                 /* 0x00000002 */
#define MMC_IDSTS_FBE_SHIFT                                (2U)
#define MMC_IDSTS_FBE_MASK                                 (0x1U << MMC_IDSTS_FBE_SHIFT)                                /* 0x00000004 */
#define MMC_IDSTS_DU_SHIFT                                 (4U)
#define MMC_IDSTS_DU_MASK                                  (0x1U << MMC_IDSTS_DU_SHIFT)                                 /* 0x00000010 */
#define MMC_IDSTS_CES_SHIFT                                (5U)
#define MMC_IDSTS_CES_MASK                                 (0x1U << MMC_IDSTS_CES_SHIFT)                                /* 0x00000020 */
#define MMC_IDSTS_NIS_SHIFT                                (8U)
#define MMC_IDSTS_NIS_MASK                                 (0x1U << MMC_IDSTS_NIS_SHIFT)                                /* 0x00000100 */
#define MMC_IDSTS_AIS_SHIFT                                (9U)
#define MMC_IDSTS_AIS_MASK                                 (0x1U << MMC_IDSTS_AIS_SHIFT)                                /* 0x00000200 */
#define MMC_IDSTS_EB_SHIFT                                 (10U)
#define MMC_IDSTS_EB_MASK                                  (0x7U << MMC_IDSTS_EB_SHIFT)                                 /* 0x00001C00 */
#define MMC_IDSTS_FSM_SHIFT                                (13U)
#define MMC_IDSTS_FSM_MASK                                 (0xFU << MMC_IDSTS_FSM_SHIFT)                                /* 0x0001E000 */
/* IDINTEN */
#define MMC_IDINTEN_OFFSET                                 (0x90U)
#define MMC_IDINTEN_TI_SHIFT                               (0U)
#define MMC_IDINTEN_TI_MASK                                (0x1U << MMC_IDINTEN_TI_SHIFT)                               /* 0x00000001 */
#define MMC_IDINTEN_RI_SHIFT                               (1U)
#define MMC_IDINTEN_RI_MASK                                (0x1U << MMC_IDINTEN_RI_SHIFT)                               /* 0x00000002 */
#define MMC_IDINTEN_FBE_SHIFT                              (2U)
#define MMC_IDINTEN_FBE_MASK                               (0x1U << MMC_IDINTEN_FBE_SHIFT)                              /* 0x00000004 */
#define MMC_IDINTEN_DU_SHIFT                               (4U)
#define MMC_IDINTEN_DU_MASK                                (0x1U << MMC_IDINTEN_DU_SHIFT)                               /* 0x00000010 */
#define MMC_IDINTEN_CES_SHIFT                              (5U)
#define MMC_IDINTEN_CES_MASK                               (0x1U << MMC_IDINTEN_CES_SHIFT)                              /* 0x00000020 */
#define MMC_IDINTEN_NI_SHIFT                               (8U)
#define MMC_IDINTEN_NI_MASK                                (0x1U << MMC_IDINTEN_NI_SHIFT)                               /* 0x00000100 */
#define MMC_IDINTEN_AI_SHIFT                               (9U)
#define MMC_IDINTEN_AI_MASK                                (0x1U << MMC_IDINTEN_AI_SHIFT)                               /* 0x00000200 */
/* DSCADDR */
#define MMC_DSCADDR_OFFSET                                 (0x94U)
#define MMC_DSCADDR_HDA_SHIFT                              (0U)
#define MMC_DSCADDR_HDA_MASK                               (0xFFFFFFFFU << MMC_DSCADDR_HDA_SHIFT)                       /* 0xFFFFFFFF */
/* BUFADDR */
#define MMC_BUFADDR_OFFSET                                 (0x98U)
#define MMC_BUFADDR_HBA_SHIFT                              (0U)
#define MMC_BUFADDR_HBA_MASK                               (0xFFFFFFFFU << MMC_BUFADDR_HBA_SHIFT)                       /* 0xFFFFFFFF */
/* CARDTHRCTL */
#define MMC_CARDTHRCTL_OFFSET                              (0x100U)
#define MMC_CARDTHRCTL_CARDRDTHREN_SHIFT                   (0U)
#define MMC_CARDTHRCTL_CARDRDTHREN_MASK                    (0x1U << MMC_CARDTHRCTL_CARDRDTHREN_SHIFT)                   /* 0x00000001 */
#define MMC_CARDTHRCTL_BSYCLRINTEN_SHIFT                   (1U)
#define MMC_CARDTHRCTL_BSYCLRINTEN_MASK                    (0x1U << MMC_CARDTHRCTL_BSYCLRINTEN_SHIFT)                   /* 0x00000002 */
#define MMC_CARDTHRCTL_CARDRDTHRESHOLD_SHIFT               (16U)
#define MMC_CARDTHRCTL_CARDRDTHRESHOLD_MASK                (0xFFFU << MMC_CARDTHRCTL_CARDRDTHRESHOLD_SHIFT)             /* 0x0FFF0000 */
/* BACK_END_POWER */
#define MMC_BACK_END_POWER_OFFSET                          (0x104U)
#define MMC_BACK_END_POWER_BACK_END_POWER_SHIFT            (0U)
#define MMC_BACK_END_POWER_BACK_END_POWER_MASK             (0x1U << MMC_BACK_END_POWER_BACK_END_POWER_SHIFT)            /* 0x00000001 */
/* UHS_REG_EXT */
#define MMC_UHS_REG_EXT_OFFSET                             (0x108U)
#define MMC_UHS_REG_EXT_MMC_VOLT_REG_SHIFT                 (0U)
#define MMC_UHS_REG_EXT_MMC_VOLT_REG_MASK                  (0xFFFFU << MMC_UHS_REG_EXT_MMC_VOLT_REG_SHIFT)              /* 0x0000FFFF */
#define MMC_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL_SHIFT          (16U)
#define MMC_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL_MASK           (0x7FU << MMC_UHS_REG_EXT_CLK_SMPL_PHASE_CTRL_SHIFT)         /* 0x007F0000 */
#define MMC_UHS_REG_EXT_CLK_DRV_PHASE_CTRL_SHIFT           (23U)
#define MMC_UHS_REG_EXT_CLK_DRV_PHASE_CTRL_MASK            (0x7FU << MMC_UHS_REG_EXT_CLK_DRV_PHASE_CTRL_SHIFT)          /* 0x3F800000 */
#define MMC_UHS_REG_EXT_EXT_CLK_MUX_CTRL_SHIFT             (30U)
#define MMC_UHS_REG_EXT_EXT_CLK_MUX_CTRL_MASK              (0x3U << MMC_UHS_REG_EXT_EXT_CLK_MUX_CTRL_SHIFT)             /* 0xC0000000 */
/* EMMC_DDR_REG */
#define MMC_EMMC_DDR_REG_OFFSET                            (0x10CU)
#define MMC_EMMC_DDR_REG_HALF_START_BIT_SHIFT              (0U)
#define MMC_EMMC_DDR_REG_HALF_START_BIT_MASK               (0x1U << MMC_EMMC_DDR_REG_HALF_START_BIT_SHIFT)              /* 0x00000001 */
/* ENABLE_SHIFT */
#define MMC_ENABLE_SHIFT_OFFSET                            (0x110U)
#define MMC_ENABLE_SHIFT_ENABLE_SHIFT_SHIFT                (0U)
#define MMC_ENABLE_SHIFT_ENABLE_SHIFT_MASK                 (0x1U << MMC_ENABLE_SHIFT_ENABLE_SHIFT_SHIFT)                /* 0x00000001 */
/* FIFO_BASE */
#define MMC_FIFO_BASE_OFFSET                               (0x200U)
#define MMC_FIFO_BASE_FIFO_BASE_ADDR_SHIFT                 (0U)
#define MMC_FIFO_BASE_FIFO_BASE_ADDR_MASK                  (0xFFFFFFFFU << MMC_FIFO_BASE_FIFO_BASE_ADDR_SHIFT)          /* 0xFFFFFFFF */

/********Name=SOFTRST_CON0,Offset=0x300********/
#define SRST_COREPO0_SRSTN   0U
#define SRST_COREPO1_SRSTN   1U
#define SRST_COREPO2_SRSTN   2U
#define SRST_COREPO3_SRSTN   3U
#define SRST_CORE0_SRSTN     4U
#define SRST_CORE1_SRSTN     5U
#define SRST_CORE2_SRSTN     6U
#define SRST_CORE3_SRSTN     7U
#define SRST_CORE0_DBG_SRSTN 8U
#define SRST_CORE1_DBG_SRSTN 9U
#define SRST_CORE2_DBG_SRSTN 10U
#define SRST_CORE3_DBG_SRSTN 11U
#define SRST_TOPDBG_SRSTN    12U
#define SRST_CORE_NOC_SRSTN  13U
#define SRST_STRC_SYS_ASRSTN 14U
#define SRST_L2_SRSTN        15U
/********Name=SOFTRST_CON1,Offset=0x304********/
#define SRST_DAP_SRSTN              16U
#define SRST_CORE_PVTM_SRSTN        17U
#define SRST_GPU_SRSTN              18U
#define SRST_GPU_NIU_SRSTN          19U
#define SRST_UPCTL2_SRSTN           20U
#define SRST_UPCTL2_ASRSTN          21U
#define SRST_UPCTL2_PRSTN           22U
#define SRST_MSCH_SRSTN             23U
#define SRST_MSCH_PSRSTN            24U
#define SRST_DDRMON_PSRSTN          25U
#define SRST_DDRSTDBY_PSRSTN        26U
#define SRST_DDRSTDBY_SRSTN         27U
#define SRST_DDRGRF_PSRSTN          28U
#define SRST_AXI_SPLIT_ASRSTN       29U
#define SRST_AXI_CMD_BUFFER_ASRSTN  30U
#define SRST_AXI_CMD_BUFFERT_PSRSTN 31U
/********Name=SOFTRST_CON2,Offset=0x308********/
#define SRST_DDRPHY_SRSTN      32U
#define SRST_DDRPHYDIV_SRSTN   33U
#define SRST_DDRPHY_PSRSTN     34U
#define SRST_VPU_ASRSTN        36U
#define SRST_VPU_NIU_ASRSTN    37U
#define SRST_VPU_HSRSTN        38U
#define SRST_VPU_NIU_HSRSTN    39U
#define SRST_VI_NIU_ASRSTN     40U
#define SRST_VI_NIU_HSRSTN     41U
#define SRST_ISP_HSRSTN        42U
#define SRST_ISP_SRSTN         43U
#define SRST_CIF_ASRSTN        44U
#define SRST_CIF_HSRSTN        45U
#define SRST_CIF_PCLKIN_SRSTN  46U
#define SRST_MIPICSIPHY_PSRSTN 47U
/********Name=SOFTRST_CON3,Offset=0x30C********/
#define SRST_VO_NIU_ASRSTN       48U
#define SRST_VO_NIU_HSRSTN       49U
#define SRST_VO_NIU_PSRSTN       50U
#define SRST_VOPB_ASRSTN         51U
#define SRST_VOPB_HSRSTN         52U
#define SRST_VOPB_SRSTN          53U
#define SRST_PWM_VOPB_SRSTN      54U
#define SRST_VOPL_ASRSTN         55U
#define SRST_VOPL_HSRSTN         56U
#define SRST_VOPL_SRSTN          57U
#define SRST_RGA_ASRSTN          58U
#define SRST_RGA_HSRSTN          59U
#define SRST_RGA_SRSTN           60U
#define SRST_MIPIDSI_HOST_PSRSTN 61U
#define SRST_MIPIDSIPHY_PSRSTN   62U
#define SRST_VPU_CORE_SRSTN      63U
/********Name=SOFTRST_CON4,Offset=0x310********/
#define SRST_PERI_NIU_ASRSTN        64U
#define SRST_USB_NIU_HSRSTN         65U
#define SRST_USB2OTG_HSRSTN         66U
#define SRST_USB2OTG_SRSTN          67U
#define SRST_USB2OTG_ADP_SRSTN      68U
#define SRST_USB2HOST_HSRSTN        69U
#define SRST_USB2HOST_ARB_HSRSTN    70U
#define SRST_USB2HOST_AUX_HSRSTN    71U
#define SRST_USB2HOST_EHCI_SRSTN    72U
#define SRST_USB2HOST_SRSTN         73U
#define SRST_USBPHYPOR_SRSTN        74U
#define SRST_USBPHY_OTG_PORT_SRSTN  75U
#define SRST_USBPHY_HOST_PORT_SRSTN 76U
#define SRST_USBPHY_GRF_PSRSTN      77U
#define SRST_CPU_BOOST_PSRSTN       78U
#define SRST_CPU_BOOST_SRSTN        79U
/********Name=SOFTRST_CON5,Offset=0x314********/
#define SRST_PDMMC_NAND_NIU_HSRSTN 80U
#define SRST_SDIO_HSRSTN           81U
#define SRST_EMMC_HSRSTN           82U
#define SRST_SFC_HSRSTN            83U
#define SRST_SFC_SRSTN             84U
#define SRST_PDSDCARD_NIU_HSRSTN   85U
#define SRST_SDMMC_HSRSTN          86U
#define SRST_NANDC_HRSTN           89U
#define SRST_NANDC_SRSTN           90U
#define SRST_GMAC_NIU_ASRSTN       92U
#define SRST_GMAC_NIU_PSRSTN       93U
#define SRST_GMAC_ASRSTN           94U
/********Name=SOFTRST_CON6,Offset=0x318********/
#define SRST_PMU_NIU_PSRSTN          96U
#define SRST_PMU_SGRF_PSRSTN         97U
#define SRST_PMU_GRF_PSRSTN          98U
#define SRST_PMU_PMU_SRSTN           99U
#define SRST_PMU_MEM_PSRSTN          100U
#define SRST_PMU_GPIO0_PSRSTN        101U
#define SRST_PMU_UART0_PSRSTN        102U
#define SRST_PMU_CRU_PSRSTN          103U
#define SRST_PMU_PVTM_SRSTN          104U
#define SRST_PMU_UART_SRSTN          105U
#define SRST_PMU_NIU_HRSTN           106U
#define SRST_PMU_DDR_FAIL_SAVE_SRSTN 107U
#define SRST_CORE_PERF_ASRSTN        108U
#define SRST_CORE_GRF_PSRSTN         109U
#define SRST_GPU_PERF_ASRSTN         110U
#define SRST_GPU_GRF_PSRSTN          111U
/********Name=SOFTRST_CON7,Offset=0x31C********/
#define SRST_CRYPTO_NIU_ASRSTN 112U
#define SRST_CRYPTO_NIU_HSRSTN 113U
#define SRST_CRYPTO_ASRSTN     114U
#define SRST_CRYPTO_HSRSTN     115U
#define SRST_CRYPTO_SRSTN      116U
#define SRST_CRYPTO_APK_SRSTN  117U
#define SRST_BUS_NIU_HSRSTN    120U
#define SRST_BUS_NIU_PSRST     121U
#define SRST_BUS_TOP_NIU_PSRST 122U
#define SRST_INTMEM_ASRST      123U
#define SRST_GIC_ASRST         124U
#define SRST_ROM_HSRSTN        126U
#define SRST_DCF_ASRSTN        127U
/********Name=SOFTRST_CON8,Offset=0x320********/
#define SRST_I2S0_RX_SRSTN 128U
#define SRST_PDM_HSRSTN    129U
#define SRST_PDM_SRSTN     130U
#define SRST_I2S0_HSRSTN   131U
#define SRST_I2S0_TX_SRSTN 132U
#define SRST_I2S1_HSRSTN   133U
#define SRST_I2S1_SRSTN    134U
#define SRST_I2S2_HSRSTN   135U
#define SRST_I2S2_SRSTN    136U
#define SRST_UART1_PSRSTN  137U
#define SRST_UART1_SRSTN   138U
#define SRST_UART2_PSRSTN  139U
#define SRST_UART2_SRSTN   140U
#define SRST_UART3_PSRSTN  141U
#define SRST_UART3_SRSTN   142U
#define SRST_UART4_PSRSTN  143U
/********Name=SOFTRST_CON9,Offset=0x324********/
#define SRST_UART4_SRSTN  144U
#define SRST_UART5_PSRSTN 145U
#define SRST_UART5_SRSTN  146U
#define SRST_I2C0_PSRSTN  147U
#define SRST_I2C0_SRSTN   148U
#define SRST_I2C1_PSRSTN  149U
#define SRST_I2C1_SRSTN   150U
#define SRST_I2C2_PSRSTN  151U
#define SRST_I2C2_SRSTN   152U
#define SRST_I2C3_PSRSTN  153U
#define SRST_I2C3_SRSTN   154U
#define SRST_PWM0_PSRSTN  157U
#define SRST_PWM0_SRSTN   158U
#define SRST_PWM1_PSRSTN  159U
/********Name=SOFTRST_CON10,Offset=0x328********/
#define SRST_PWM1_SRSTN    160U
#define SRST_SPI0_PSRSTN   161U
#define SRST_SPI0_SRSTN    162U
#define SRST_SPI1_PSRSTN   163U
#define SRST_SPI1_SRSTN    164U
#define SRST_SARADC_PSRSTN 165U
#define SRST_SARADC_SRSTN  166U
#define SRST_TSADC_PSRSTN  167U
#define SRST_TSADC_SRSTN   168U
#define SRST_TIMER_PSRSTN  169U
#define SRST_TIMER0_SRSTN  170U
#define SRST_TIMER1_SRSTN  171U
#define SRST_TIMER2_SRSTN  172U
#define SRST_TIMER3_SRSTN  173U
#define SRST_TIMER4_SRSTN  174U
#define SRST_TIMER5_SRSTN  175U
/********Name=SOFTRST_CON11,Offset=0x32C********/
#define SRST_OTP_NS_PSRSTN     176U
#define SRST_OTP_NS_SBPI_SRSTN 177U
#define SRST_OTP_NS_USR_SRSTN  178U
#define SRST_OTP_PHY_PSRSTN    179U
#define SRST_OTP_PHY_SRSTN     180U
#define SRST_WDT_NS_PSRSTN     181U
#define SRST_GPIO1_PSRSTN      182U
#define SRST_GPIO2_PSRSTN      183U
#define SRST_GPIO3_PSRSTN      184U
#define SRST_SGRF_PSRSTN       185U
#define SRST_GRF_PSRSTN        186U
#define SRST_PVTPLL_CPU_SRSTN  188U
#define SRST_PVTPLL_GPU_SRSTN  189U
#define SRST_DCF_PSRSTN        191U

/********Name=CLKGATE_CON0,Offset=0x200********/
#define CORE_PLL_CLK_GATE              0U
#define ACLK_CORE_SRC_CLK_GATE         1U
#define PCLK_CORE_DBG_SRC_CLK_GATE     2U
#define CLK_JTAG_CORE_CLK_GATE         3U
#define ACLK_CORE_NIU_CLK_GATE         4U
#define PCLK_CORE_DBG_NIU_CLK_GATE     5U
#define PCLK_CORE_DBG_DAPLITE_CLK_GATE 6U
#define DDRPHY_DPLL_CLK_GATE           7U
#define GPU_PLL_CLK_GATE               8U
#define GPU_CLK_NP5_SRC_CLK_GATE       9U
#define CLK_GPU_CLK_GATE               10U
#define ACLK_GPU_NIU_CLK_GATE          11U
#define GPU_CLK_DIV_CLK_GATE           12U
#define DDRPHY_GPLL_CLK_GATE           13U
#define CLK_DDRPHY1X_CLK_GATE          14U
#define CLK_DDRMON24M_CLK_GATE         15U
/********Name=CLKGATE_CON1,Offset=0x204********/
#define CLK_STDBY_SRC_CLK_GATE       16U
#define DDR_PCLK_PLL_CLK_GATE        17U
#define ACLK_AXI_CMD_BUFFER_CLK_GATE 18U
#define PCLK_AXI_CMD_BUFFER_CLK_GATE 19U
#define ACLK_UPCTL2_CLK_GATE         21U
#define CLK_DDRC_UPCTL2_CLK_GATE     22U
#define PCLK_UPCTL2_CLK_GATE         23U
#define CLK_MSCH_CLK_GATE            24U
#define PCLK_MSCH_CLK_GATE           25U
#define PCLK_DDRMON_CLK_GATE         26U
#define CLK_DDRMON_CLK_GATE          27U
#define PCLK_DDRSTDBY_CLK_GATE       28U
#define CLK_DDRSTANBY_CLK_GATE       29U
#define PCLK_DDR_GRF_CLK_GATE        30U
#define ACLK_AXI_SPLIT_CLK_GATE      31U
/********Name=CLKGATE_CON2,Offset=0x208********/
#define ACLK_VO_PLL_CLK_GATE        32U
#define CLK_RGA_CORE_PLL_CLK_GATE   33U
#define DCLK_VOPB_PLL_CLK_GATE      34U
#define DCLK_VOPB_FRAC_SRC_CLK_GATE 35U
#define DCLK_VOPB_CLK_GATE          36U
#define CLK_PWM_VOPB_PLL_CLK_GATE   37U
#define DCLK_VOPL_PLL_CLK_GATE      38U
#define DCLK_VOPL_FRAC_SRC_CLK_GATE 39U
#define DCLK_VOPL_CLK_GATE          40U
#define HCLK_VO_SRC_CLK_GATE        44U
#define PCLK_VO_SRC_CLK_GATE        45U
/********Name=CLKGATE_CON3,Offset=0x20C********/
#define ACLK_VO_NIU_CLK_GATE        48U
#define HCLK_VO_NIU_CLK_GATE        49U
#define PCLK_VO_NIU_CLK_GATE        50U
#define ACLK_VOPB_CLK_GATE          51U
#define HCLK_VOPB_CLK_GATE          52U
#define ACLK_VOPL_CLK_GATE          53U
#define HCLK_VOPL_CLK_GATE          54U
#define ACLK_RGA_CLK_GATE           55U
#define HCLK_RGA_CLK_GATE           56U
#define PCLK_MIPI_DSI_HOST_CLK_GATE 57U
/********Name=CLKGATE_CON4,Offset=0x210********/
#define ACLK_VPU_PLL_CLK_GATE     64U
#define CLK_VPU_CORE_PLL_CLK_GATE 65U
#define HCLK_VPU_SRC_CLK_GATE     66U
#define ACLK_VPU_CLK_GATE         68U
#define ACLK_VPU_NIU_CLK_GATE     69U
#define HCLK_VPU_CLK_GATE         70U
#define HCLK_VPU_NIU_CLK_GATE     71U
#define ACLK_VI_PLL_CLK_GATE      72U
#define CLK_ISP_PLL_CLK_GATE      73U
#define CLK_CIF_OUT_PLL_CLK_GATE  75U
#define HCLK_VI_SRC_CLK_GATE      76U
#define PCLKIN_ISP_CLK_GATE       77U
#define PCLKIN_CIF_CLK_GATE       78U
#define ACLK_VI_NIU_CLK_GATE      79U
/********Name=CLKGATE_CON5,Offset=0x214********/
#define HCLK_VI_NIU_CLK_GATE             80U
#define ACLK_CIF_CLK_GATE                81U
#define HCLK_CIF_CLK_GATE                82U
#define ACLK_ISP_CLK_GATE                83U
#define HCLK_ISP_CLK_GATE                84U
#define ACLK_HCLK_PCLK_PERI_PLL_CLK_GATE 87U
#define ACLK_PERI_CLK_GATE               88U
#define ACLK_PERI_NIU_CLK_GATE           89U
#define CLK_NANDC_PLL_CLK_GATE           91U
#define CLK_NANDC_DIV50_CLK_GATE         92U
#define CLK_NANDC_CLK_GATE               93U
#define HCLK_NANDC_CLK_GATE              95U
/********Name=CLKGATE_CON6,Offset=0x218********/
#define HCLK_PDMMC_NAND_CLK_GATE     96U
#define CLK_SDIO_PLL_CLK_GATE        97U
#define CLK_SDIO_DIV50_CLK_GATE      98U
#define CLK_SDIO_CLK_GATE            99U
#define CLK_EMMC_PLL_CLK_GATE        100U
#define CLK_EMMC_DIV50_CLK_GATE      101U
#define CLK_EMMC_CLK_GATE            102U
#define CLK_SFC_PLL_CLK_GATE         103U
#define HCLK_PDMMC_NAND_NIU_CLK_GATE 104U
#define HCLK_SDIO_CLK_GATE           105U
#define HCLK_EMMC_CLK_GATE           106U
#define HCLK_SFC_CLK_GATE            107U
#define HCLK_PDSDCARD_CLK_GATE       108U
#define CLK_SDMMC_PLL_CLK_GATE       109U
#define CLK_SDMMC_DIV50_CLK_GATE     110U
#define CLK_SDMMC_CLK_GATE           111U
/********Name=CLKGATE_CON7,Offset=0x21C********/
#define HCLK_PDSDCARD_NIU_CLK_GATE 112U
#define HCLK_SDMMC_CLK_GATE        113U
#define HCLK_PDUSB_CLK_GATE        114U
#define CLK_OTG_ADP_CLK_GATE       115U
#define HCLK_PDUSB_NIU_CLK_GATE    116U
#define HCLK_USB2OTG_CLK_GATE      117U
#define HCLK_USB2HOST_CLK_GATE     118U
#define HCLK_USB2HOST_ARB_CLK_GATE 120U
#define ACLK_PDGMAC_CLK_GATE       122U
#define CLK_GMAC_PLL_CLK_GATE      123U
#define PCLK_GMAC_SRC_CLK_GATE     124U
#define CLK_GMAC_TX_RX_CLK_GATE    125U
#define CLK_GMAC_REF_CLK_GATE      127U
/********Name=CLKGATE_CON8,Offset=0x220********/
#define ACLK_GMAC_NIU_CLK_GATE      128U
#define PCLK_GMAC_NIU_CLK_GATE      129U
#define ACLK_GMAC_CLK_GATE          130U
#define PCLK_GMAC_CLK_GATE          131U
#define CLK_GMAC_OUT_PLL_CLK_GATE   133U
#define PD_BUS_PLL_CLK_GATE         134U
#define ACLK_BUS_CLK_GATE           135U
#define HCLK_BUS_CLK_GATE           136U
#define PCLK_BUS_CLK_GATE           137U
#define PCLK_TOP_CLK_GATE           138U
#define ACLK_PDCRYPTO_CLK_GATE      140U
#define HCLK_PDCRYPTO_CLK_GATE      141U
#define CLK_CRYPTO_PLL_CLK_GATE     142U
#define CLK_CRYPTO_APK_PLL_CLK_GATE 143U
/********Name=CLKGATE_CON9,Offset=0x224********/
#define ACLK_CRYPTO_NIU_CLK_GATE      146U
#define HCLK_CRYPTO_NIU_CLK_GATE      147U
#define ACLK_CRYPTO_CLK_GATE          148U
#define HCLK_CRYPTO_CLK_GATE          149U
#define CLK_PDM_PLL_CLK_GATE          153U
#define CLK_PDM_FRAC_SRC_CLK_GATE     154U
#define CLK_PDM_CLK_GATE              155U
#define CLK_I2S0_TX_PLL_CLK_GATE      156U
#define CLK_I2S0_TX_FRAC_SRC_CLK_GATE 157U
#define CLK_I2S0_TX_CLK_GATE          158U
#define CLK_I2S0_TX_OUT_MCLK_GATE     159U
/********Name=CLKGATE_CON10,Offset=0x228********/
#define CLK_I2S1_PLL_CLK_GATE        160U
#define CLK_I2S1_FRAC_SRC_CLK_GATE   161U
#define CLK_I2S1_CLK_GATE            162U
#define CLK_I2S1_OUT_MCLK_GATE       163U
#define CLK_I2S2_PLL_CLK_GATE        164U
#define CLK_I2S2_FRAC_SRC_CLK_GATE   165U
#define CLK_I2S2_CLK_GATE            166U
#define CLK_I2S2_OUT_MCLK_GATE       167U
#define CLK_I2S0_TX_OUT_MCLK_OE_GATE 168U
#define CLK_I2S1_OUT_MCLK_OE_GATE    169U
#define CLK_I2S2_OUT_MCLK_OE_GATE    170U
#define CLK_I2S0_RX_OUT_MCLK_OE_GATE 171U
#define CLK_UART1_PLL_CLK_GATE       172U
#define CLK_UART1_DIVNP5_CLK_GATE    173U
#define CLK_UART1_FRAC_SRC_CLK_GATE  174U
#define CLK_UART1_CLK_GATE           175U
/********Name=CLKGATE_CON11,Offset=0x22C********/
#define CLK_UART2_PLL_CLK_GATE      176U
#define CLK_UART2_DIVNP5_CLK_GATE   177U
#define CLK_UART2_FRAC_SRC_CLK_GATE 178U
#define CLK_UART2_CLK_GATE          179U
#define CLK_UART3_PLL_CLK_GATE      180U
#define CLK_UART3_DIVNP5_CLK_GATE   181U
#define CLK_UART3_FRAC_SRC_CLK_GATE 182U
#define CLK_UART3_CLK_GATE          183U
#define CLK_UART4_PLL_CLK_GATE      184U
#define CLK_UART4_DIVNP5_CLK_GATE   185U
#define CLK_UART4_FRAC_SRC_CLK_GATE 186U
#define CLK_UART4_CLK_GATE          187U
#define CLK_UART5_PLL_CLK_GATE      188U
#define CLK_UART5_DIVNP5_CLK_GATE   189U
#define CLK_UART5_FRAC_SRC_CLK_GATE 190U
#define CLK_UART5_CLK_GATE          191U
/********Name=CLKGATE_CON12,Offset=0x230********/
#define CLK_I2C0_PLL_CLK_GATE   192U
#define CLK_I2C1_PLL_CLK_GATE   193U
#define CLK_I2C2_PLL_CLK_GATE   194U
#define CLK_I2C3_PLL_CLK_GATE   195U
#define CLK_PWM0_PLL_CLK_GATE   197U
#define CLK_PWM1_PLL_CLK_GATE   198U
#define CLK_SPI0_PLL_CLK_GATE   199U
#define CLK_SPI1_PLL_CLK_GATE   200U
#define CLK_TSADC_PLL_CLK_GATE  201U
#define CLK_SARADC_PLL_CLK_GATE 202U
#define CLK_OTP_PLL_CLK_GATE    203U
#define CLK_CPU_BOOST_CLK_GATE  204U
/********Name=CLKGATE_CON13,Offset=0x234********/
#define CLK_TIMER0_CLK_GATE   208U
#define CLK_TIMER1_CLK_GATE   209U
#define CLK_TIMER2_CLK_GATE   210U
#define CLK_TIMER3_CLK_GATE   211U
#define CLK_TIMER4_CLK_GATE   212U
#define CLK_TIMER5_CLK_GATE   213U
#define CLK_OTP_USR_CLK_GATE  214U
#define ACLK_BUS_NIU_CLK_GATE 216U
#define HCLK_BUS_NIU_CLK_GATE 217U
#define PCLK_BUS_NIU_CLK_GATE 218U
#define ACLK_INTMEM_CLK_GATE  219U
#define ACLK_GIC_CLK_GATE     220U
#define HCLK_ROM_CLK_GATE     222U
#define ACLK_DFC_CLK_GATE     223U
/********Name=CLKGATE_CON14,Offset=0x238********/
#define PCLK_DCF_CLK_GATE   224U
#define HCLK_PDM_CLK_GATE   225U
#define HCLK_I2S0_CLK_GATE  226U
#define HCLK_I2S1_CLK_GATE  227U
#define HCLK_I2S2_CLK_GATE  228U
#define PCLK_UART1_CLK_GATE 229U
#define PCLK_UART2_CLK_GATE 230U
#define PCLK_UART3_CLK_GATE 231U
#define PCLK_UART4_CLK_GATE 232U
#define PCLK_UART5_CLK_GATE 233U
#define PCLK_I2C0_CLK_GATE  234U
#define PCLK_I2C1_CLK_GATE  235U
#define PCLK_I2C2_CLK_GATE  236U
#define PCLK_I2C3_CLK_GATE  237U
#define PCLK_PWM0_CLK_GATE  239U
/********Name=CLKGATE_CON15,Offset=0x23C********/
#define PCLK_PWM1_CLK_GATE     240U
#define PCLK_SPI0_CLK_GATE     241U
#define PCLK_SPI1_CLK_GATE     242U
#define PCLK_SARADC_CLK_GATE   243U
#define PCLK_TSADC_CLK_GATE    244U
#define PCLK_TIMER_CLK_GATE    245U
#define PCLK_OTP_NS_CLK_GATE   246U
#define PCLK_WDT_NS_GATE       247U
#define PCLK_GPIO1_CLK_GATE    248U
#define PCLK_GPIO2_CLK_GATE    249U
#define PCLK_GPIO3_CLK_GATE    250U
#define PCLK_BUS_GRF_CLK_GATE  251U
#define PCLK_BUS_SGRF_CLK_GATE 252U
/********Name=CLKGATE_CON16,Offset=0x240********/
#define PCLK_TOP_NIU_CLK_GATE    256U
#define PCLK_TOP_CRU_CLK_GATE    257U
#define PCLK_OTP_PHY_CLK_GATE    258U
#define PCLK_DDRPHY_CLK_GATE     259U
#define PCLK_MIPIDSIPHY_CLK_GATE 260U
#define PCLK_MIPICSIPHY_CLK_GATE 261U
#define PCLK_USB_GRF_GATE        262U
#define PCLK_CPU_BOOST_CLK_GATE  263U
#define TESTCLK_CLK_GATE         271U
/********Name=CLKGATE_CON17,Offset=0x244********/
#define CLK_I2S0_RX_PLL_CLK_GATE     272U
#define CLK_I2S0_RX_DIVFRAC_CLK_GATE 273U
#define CLK_I2S0_RX_CLK_GATE         274U
#define CLK_I2S0_RX_OUT_MCLK_GATE    275U
#define CLK_CORE_PVTM_CLK_GATE       276U
#define ACLK_CORE_PERF_CLK_GATE      277U
#define PCLK_CORE_GRF_CLK_GATE       278U
#define CLK_CORE_APLL_CLK_GATE       279U
#define ACLK_GPU_PERF_CLK_GATE       280U
#define PCLK_GPU_GRF_CLK_GATE        281U
#define ACLK_GPU_DIV_CLK_GATE        282U
#define GPU_PVTPLL_REF_CLK_GATE      283U
#define CPU_PVTPLL_REF_CLK_GATE      284U
/********Name=PMU_CLKGATE_CON0,Offset=0xC080********/
#define PCLK_PDPMU_PLL_CLK_GATE        288U
#define PCLK_PMU_NIU_CLK_GATE          289U
#define PCLK_PMU_SGRF_CLK_GATE         290U
#define PCLK_PMU_GRF_CLK_GATE          291U
#define PCLK_PMU_PMU_CLK_GATE          292U
#define PCLK_PMU_MEM_CLK_GATE          293U
#define PCLK_PMU_GPIO0_CLK_GATE        294U
#define PCLK_PMU_UART0_CLK_GATE        295U
#define PCLK_PMU_CRU_CLK_GATE          296U
#define XIN_OSC0_FUNC_DIV_SRC_CLK_GATE 300U
#define CLK_DIV32P768KHZ_SRC_CLK_GATE  301U
#define CLK_WIFI_PLL_CLK_GATE          302U
#define CLK_WIFI_CLK_GATE              303U
/********Name=PMU_CLKGATE_CON1,Offset=0xC084********/
#define CLK_UART0_PMU_PLL_CLK_GATE    304U
#define CLK_UART0_PMU_DIVNP5_CLK_GATE 305U
#define CLK_UART0_PMU_FRAC_CLK_GATE   306U
#define CLK_UART0_PMU_CLK_GATE        307U
#define CLK_PVTM_PMU_CLK_GATE         308U
#define CLK_REF24M_PLL_CLK_GATE       312U
#define USBPHY_REF_CLK_GATE           313U
#define MIPIDSIPHY_REF_CCLK_GATE      314U

/********Name=CLKSEL_CON0,Offset=0x100********/
#define CLK_CORE_DIV  0x04000000U
#define CORE_DBG_DIV  0x04080000U
#define ACLK_CORE_DIV 0x030C0000U
/********Name=CLKSEL_CON1,Offset=0x104********/
#define CLK_GPU_DIV  0x04000001U
#define ACLK_GPU_DIV 0x020D0001U
/********Name=CLKSEL_CON2,Offset=0x108********/
#define DDRPHY1X_DIV 0x03000002U
#define PCLK_DDR_DIV 0x05080002U
/********Name=CLKSEL_CON3,Offset=0x10C********/
#define ACLK_VO_DIV 0x05000003U
#define HCLK_VO_DIV 0x04080003U
#define PCLK_VO_DIV 0x040C0003U
/********Name=CLKSEL_CON4,Offset=0x110********/
#define CLK_RGA_CORE_DIV 0x05000004U
/********Name=CLKSEL_CON5,Offset=0x114********/
#define DCLK_VOPB_DIV 0x08000005U
/********Name=CLKSEL_CON6,Offset=0x118********/
#define DCLK_VOPB_FRAC_DIV 0x20000006U
/********Name=CLKSEL_CON7,Offset=0x11C********/
#define CLK_PWM_VOPB_DIV 0x07000007U
/********Name=CLKSEL_CON8,Offset=0x120********/
#define DCLK_VOPL_DIV 0x08000008U
/********Name=CLKSEL_CON9,Offset=0x124********/
#define DCLK_VOPL_FRAC_DIV 0x20000009U
/********Name=CLKSEL_CON10,Offset=0x128********/
#define ACLK_VPU_DIV 0x0500000AU
#define HCLK_VPU_DIV 0x0408000AU
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define ACLK_VI_DIV 0x0500000BU
#define HCLK_VI_DIV 0x0408000BU
/********Name=CLKSEL_CON12,Offset=0x130********/
#define CLK_ISP_DIV      0x0500000CU
#define CLK_GMAC_OUT_DIV 0x0508000CU
/********Name=CLKSEL_CON13,Offset=0x134********/
#define CLK_CIF_OUT_DIV  0x0600000DU
#define CLK_VPU_CORE_DIV 0x0508000DU
/********Name=CLKSEL_CON14,Offset=0x138********/
#define ACLK_PERI_DIV 0x0500000EU
#define HCLK_PERI_DIV 0x0508000EU
/********Name=CLKSEL_CON15,Offset=0x13C********/
#define CLK_NANDC_DIV       0x0500000FU
#define CLK_NANDC_DIV50_DIV 0x0508000FU
/********Name=CLKSEL_CON16,Offset=0x140********/
#define CLK_SDMMC_DIV 0x08000010U
/********Name=CLKSEL_CON17,Offset=0x144********/
#define CLK_SDMMC_DIV50_DIV 0x08000011U
/********Name=CLKSEL_CON18,Offset=0x148********/
#define CLK_SDIO_DIV 0x08000012U
/********Name=CLKSEL_CON19,Offset=0x14C********/
#define CLK_SDIO_DIV50_DIV 0x08000013U
/********Name=CLKSEL_CON20,Offset=0x150********/
#define CLK_EMMC_DIV 0x08000014U
/********Name=CLKSEL_CON21,Offset=0x154********/
#define CLK_EMMC_DIV50_DIV 0x08000015U
/********Name=CLKSEL_CON22,Offset=0x158********/
#define CLK_SFC_DIV  0x07000016U
#define CLK_GMAC_DIV 0x05080016U
/********Name=CLKSEL_CON23,Offset=0x15C********/
#define PCLK_GMAC_DIV 0x04000017U
#define ACLK_BUS_DIV  0x05080017U
/********Name=CLKSEL_CON24,Offset=0x160********/
#define HCLK_BUS_DIV 0x05000018U
#define PCLK_BUS_DIV 0x02080018U
/********Name=CLKSEL_CON25,Offset=0x164********/
#define CLK_CRYPTO_DIV     0x05000019U
#define CLK_CRYPTO_APK_DIV 0x05080019U
/********Name=CLKSEL_CON26,Offset=0x168********/
#define CLK_PDM_DIV 0x0700001AU
/********Name=CLKSEL_CON27,Offset=0x16C********/
#define CLK_PDM_FRAC_DIV 0x2000001BU
/********Name=CLKSEL_CON28,Offset=0x170********/
#define CLK_I2S0_TX_DIV 0x0700001CU
/********Name=CLKSEL_CON29,Offset=0x174********/
#define CLK_I2S0_TX_FRAC_DIV 0x2000001DU
/********Name=CLKSEL_CON30,Offset=0x178********/
#define CLK_I2S1_DIV 0x0700001EU
/********Name=CLKSEL_CON31,Offset=0x17C********/
#define CLK_I2S1_FRAC_DIV 0x2000001FU
/********Name=CLKSEL_CON32,Offset=0x180********/
#define CLK_I2S2_DIV 0x07000020U
/********Name=CLKSEL_CON33,Offset=0x184********/
#define CLK_I2S2_FRAC_DIV 0x20000021U
/********Name=CLKSEL_CON34,Offset=0x188********/
#define CLK_UART1_DIV 0x07000022U
/********Name=CLKSEL_CON35,Offset=0x18C********/
#define CLK_UART1_DIVNP5_DIV 0x07000023U
/********Name=CLKSEL_CON36,Offset=0x190********/
#define CLK_UART1_FRAC_DIV 0x20000024U
/********Name=CLKSEL_CON37,Offset=0x194********/
#define CLK_UART2_DIV 0x07000025U
/********Name=CLKSEL_CON38,Offset=0x198********/
#define CLK_UART2_DIVNP5_DIV 0x07000026U
/********Name=CLKSEL_CON39,Offset=0x19C********/
#define CLK_UART2_FRAC_DIV 0x20000027U
/********Name=CLKSEL_CON40,Offset=0x1A0********/
#define CLK_UART3_DIV 0x07000028U
/********Name=CLKSEL_CON41,Offset=0x1A4********/
#define CLK_UART3_DIVNP5_DIV 0x07000029U
/********Name=CLKSEL_CON42,Offset=0x1A8********/
#define CLK_UART3_FRAC_DIV 0x2000002AU
/********Name=CLKSEL_CON43,Offset=0x1AC********/
#define CLK_UART4_DIV 0x0700002BU
/********Name=CLKSEL_CON44,Offset=0x1B0********/
#define CLK_UART4_DIVNP5_DIV 0x0700002CU
/********Name=CLKSEL_CON45,Offset=0x1B4********/
#define CLK_UART4_FRAC_DIV 0x2000002DU
/********Name=CLKSEL_CON46,Offset=0x1B8********/
#define CLK_UART5_DIV 0x0700002EU
/********Name=CLKSEL_CON47,Offset=0x1BC********/
#define CLK_UART5_DIVNP5_DIV 0x0700002FU
/********Name=CLKSEL_CON48,Offset=0x1C0********/
#define CLK_UART5_FRAC_DIV 0x20000030U
/********Name=CLKSEL_CON49,Offset=0x1C4********/
#define CLK_I2C0_DIV 0x07000031U
#define CLK_I2C1_DIV 0x07080031U
/********Name=CLKSEL_CON50,Offset=0x1C8********/
#define CLK_I2C2_DIV 0x07000032U
#define CLK_I2C3_DIV 0x07080032U
/********Name=CLKSEL_CON52,Offset=0x1D0********/
#define CLK_PWM0_DIV 0x07000034U
#define CLK_PWM1_DIV 0x07080034U
/********Name=CLKSEL_CON53,Offset=0x1D4********/
#define CLK_SPI0_DIV 0x07000035U
#define CLK_SPI1_DIV 0x07080035U
/********Name=CLKSEL_CON54,Offset=0x1D8********/
#define CLK_TSADC_DIV 0x0B000036U
/********Name=CLKSEL_CON55,Offset=0x1DC********/
#define CLK_SARADC_DIV 0x0B000037U
/********Name=CLKSEL_CON56,Offset=0x1E0********/
#define CLK_OTP_DIV 0x08000038U
/********Name=CLKSEL_CON57,Offset=0x1E4********/
#define TEST_DIV 0x05080039U
/********Name=CLKSEL_CON58,Offset=0x1E8********/
#define CLK_I2S0_RX_DIV 0x0700003AU
/********Name=CLKSEL_CON59,Offset=0x1EC********/
#define CLK_I2S0_RX_FRAC_DIV 0x2000003BU
/********Name=PMU_CLKSEL_CON0,Offset=0xC040********/
#define PCLK_PDPMU_DIV    0x05000000U
#define XIN_OSC0_FUNC_DIV 0x05080000U
/********Name=PMU_CLKSEL_CON1,Offset=0xC044********/
#define CLK_DIV32P768KHZ_DIV 0x20000001U
/********Name=PMU_CLKSEL_CON2,Offset=0xC048********/
#define CLK_REF24M_DIV 0x06000002U
#define CLK_WIFI_DIV   0x06080002U
/********Name=PMU_CLKSEL_CON3,Offset=0xC04C********/
#define CLK_UART0_DIV 0x05000003U
/********Name=PMU_CLKSEL_CON4,Offset=0xC050********/
#define CLK_UART0_DIVNP5_DIV 0x05000004U
/********Name=PMU_CLKSEL_CON5,Offset=0xC054********/
#define CLK_UART0_FRAC_DIV 0x20000005U

/********Name=CLKSEL_CON0,Offset=0x100********/
#define CORE_CLK_PLL_SEL      0x01070000U
#define CORE_CLK_PLL_SEL_APLL 0U
#define CORE_CLK_PLL_SEL_GPLL 1U
#define CORE_CLK_SEL          0x010F0000U
#define CORE_CLK_SEL_CLK_APLL 0U
/********Name=CLKSEL_CON1,Offset=0x104********/
#define CLK_GPU_PLL_SEL            0x02060001U
#define CLK_GPU_PLL_SEL_GPLL       0U
#define CLK_GPU_PLL_SEL_CPLL       1U
#define CLK_GPU_PLL_SEL_USBPHY480M 2U
#define CLK_GPU_PLL_SEL_NPLL       3U
#define CLK_GPU_SEL                0x010F0001U
#define CLK_GPU_SEL_CLK_GPU_DIV    0U
#define CLK_GPU_SEL_CLK_GPU_NP5    1U
/********Name=CLKSEL_CON2,Offset=0x108********/
#define CLK_DDRSTDBY_SEL          0x01040002U
#define CLK_DDRSTDBY_SEL_CLOCK    0U
#define DDRPHY1X_PLL_CLK_SEL      0x01070002U
#define DDRPHY1X_PLL_CLK_SEL_GPLL 0U
/********Name=CLKSEL_CON3,Offset=0x10C********/
#define ACLK_VO_PLL_SEL      0x02060003U
#define ACLK_VO_PLL_SEL_GPLL 0U
#define ACLK_VO_PLL_SEL_CPLL 1U
#define ACLK_VO_PLL_SEL_NPLL 2U
/********Name=CLKSEL_CON4,Offset=0x110********/
#define CLK_RGA_CORE_PLL_SEL      0x02060004U
#define CLK_RGA_CORE_PLL_SEL_GPLL 0U
#define CLK_RGA_CORE_PLL_SEL_CPLL 1U
#define CLK_RGA_CORE_PLL_SEL_NPLL 2U
/********Name=CLKSEL_CON5,Offset=0x114********/
#define DCLK_VOPB_PLL_SEL                0x010B0005U
#define DCLK_VOPB_PLL_SEL_CPLL           0U
#define DCLK_VOPB_PLL_SEL_NPLL           1U
#define DCLK_VOPB_SEL                    0x020E0005U
#define DCLK_VOPB_SEL_DCLK_VOPB          0U
#define DCLK_VOPB_SEL_DCLK_VOPB_FRAC_OUT 1U
#define DCLK_VOPB_SEL_XIN_OSC0           2U
/********Name=CLKSEL_CON6,Offset=0x118********/
/********Name=CLKSEL_CON7,Offset=0x11C********/
#define CLK_PWM_VOPB_PLL_SEL          0x01070007U
#define CLK_PWM_VOPB_PLL_SEL_GPLL     0U
#define CLK_PWM_VOPB_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON8,Offset=0x120********/
#define DCLK_VOPL_PLL_SEL                0x010B0008U
#define DCLK_VOPL_PLL_SEL_NPLL           0U
#define DCLK_VOPL_PLL_SEL_CPLL           1U
#define DCLK_VOPL_SEL                    0x020E0008U
#define DCLK_VOPL_SEL_DCLK_VOPL          0U
#define DCLK_VOPL_SEL_DCLK_VOPL_FRAC_OUT 1U
#define DCLK_VOPL_SEL_XIN_OSC0           2U
/********Name=CLKSEL_CON9,Offset=0x124********/
/********Name=CLKSEL_CON10,Offset=0x128********/
#define ACLK_VPU_PLL_SEL      0x0206000AU
#define ACLK_VPU_PLL_SEL_GPLL 0U
#define ACLK_VPU_PLL_SEL_CPLL 1U
#define ACLK_VPU_PLL_SEL_NPLL 2U
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define ACLK_VI_PLL_SEL      0x0206000BU
#define ACLK_VI_PLL_SEL_GPLL 0U
#define ACLK_VI_PLL_SEL_CPLL 1U
#define ACLK_VI_PLL_SEL_NPLL 2U
/********Name=CLKSEL_CON12,Offset=0x130********/
#define CLK_ISP_PLL_SEL           0x0206000CU
#define CLK_ISP_PLL_SEL_GPLL      0U
#define CLK_ISP_PLL_SEL_CPLL      1U
#define CLK_ISP_PLL_SEL_NPLL      2U
#define CLK_GMAC_OUT_PLL_SEL      0x020E000CU
#define CLK_GMAC_OUT_PLL_SEL_GPLL 0U
#define CLK_GMAC_OUT_PLL_SEL_CPLL 1U
#define CLK_GMAC_OUT_PLL_SEL_NPLL 2U
/********Name=CLKSEL_CON13,Offset=0x134********/
#define CLK_CIF_OUT_PLL_SEL            0x0206000DU
#define CLK_CIF_OUT_PLL_SEL_XIN_OSC0   0U
#define CLK_CIF_OUT_PLL_SEL_CPLL       1U
#define CLK_CIF_OUT_PLL_SEL_NPLL       2U
#define CLK_CIF_OUT_PLL_SEL_USBPHY480M 3U
#define CLK_VPU_CORE_PLL_CLK_SEL       0x020E000DU
#define CLK_VPU_CORE_PLL_CLK_SEL_GPLL  0U
#define CLK_VPU_CORE_PLL_CLK_SEL_CPLL  1U
#define CLK_VPU_CORE_PLL_CLK_SEL_NPLL  2U
/********Name=CLKSEL_CON14,Offset=0x138********/
#define ACLK_HCLK_PERI_PLL_SEL      0x010F000EU
#define ACLK_HCLK_PERI_PLL_SEL_GPLL 0U
#define ACLK_HCLK_PERI_PLL_SEL_CPLL 1U
/********Name=CLKSEL_CON15,Offset=0x13C********/
#define CLK_NANDC_PLL_SEL             0x0206000FU
#define CLK_NANDC_SEL_CLK_NANDC       0U
#define CLK_NANDC_SEL_CLK_NANDC_DIV50 1U
/********Name=CLKSEL_CON16,Offset=0x140********/
#define CLK_SDMMC_PLL_SEL          0x020E0010U
#define CLK_SDMMC_PLL_SEL_GPLL     0U
#define CLK_SDMMC_PLL_SEL_CPLL     1U
#define CLK_SDMMC_PLL_SEL_NPLL     2U
#define CLK_SDMMC_PLL_SEL_XIN_OSC0 3U
/********Name=CLKSEL_CON17,Offset=0x144********/
#define CLK_SDMMC_SEL                 0x010F0011U
#define CLK_SDMMC_SEL_CLK_SDMMC       0U
#define CLK_SDMMC_SEL_CLK_SDMMC_DIV50 1U
/********Name=CLKSEL_CON18,Offset=0x148********/
#define CLK_SDIO_PLL_SEL          0x020E0012U
#define CLK_SDIO_PLL_SEL_GPLL     0U
#define CLK_SDIO_PLL_SEL_CPLL     1U
#define CLK_SDIO_PLL_SEL_NPLL     2U
#define CLK_SDIO_PLL_SEL_XIN_OSC0 3U
/********Name=CLKSEL_CON19,Offset=0x14C********/
#define CLK_SDIO_SEL                0x010F0013U
#define CLK_SDIO_SEL_CLK_SDIO       0U
#define CLK_SDIO_SEL_CLK_SDIO_DIV50 1U
/********Name=CLKSEL_CON20,Offset=0x150********/
#define CLK_EMMC_PLL_SEL          0x020E0014U
#define CLK_EMMC_PLL_SEL_GPLL     0U
#define CLK_EMMC_PLL_SEL_CPLL     1U
#define CLK_EMMC_PLL_SEL_NPLL     2U
#define CLK_EMMC_PLL_SEL_XIN_OSC0 3U
/********Name=CLKSEL_CON21,Offset=0x154********/
#define CLK_EMMC_SEL                0x010F0015U
#define CLK_EMMC_SEL_CLK_EMMC       0U
#define CLK_EMMC_SEL_CLK_EMMC_DIV50 1U
/********Name=CLKSEL_CON22,Offset=0x158********/
#define CLK_SFC_PLL_SEL       0x01070016U
#define CLK_SFC_PLL_SEL_GPLL  0U
#define CLK_SFC_PLL_SEL_CPLL  1U
#define CLK_GMAC_PLL_SEL      0x020E0016U
#define CLK_GMAC_PLL_SEL_GPLL 0U
#define CLK_GMAC_PLL_SEL_CPLL 1U
#define CLK_GMAC_PLL_SEL_NPLL 2U
/********Name=CLKSEL_CON23,Offset=0x15C********/
#define RMII_EXTCLKSRC_SEL              0x01060017U
#define RMII_CLK_SEL                    0x01070017U
#define RMII_CLK_SEL_10M                0U
#define RMII_CLK_SEL_100M               1U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL      0x010F0017U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL_GPLL 0U
#define ACLK_HCLK_PCLK_BUS_PLL_SEL_CPLL 1U
/********Name=CLKSEL_CON24,Offset=0x160********/
/********Name=CLKSEL_CON25,Offset=0x164********/
#define CLK_CRYPTO_PLL_SEL      0x02060019U
#define CLK_CRYPTO_PLL_SEL_GPLL 0U
#define CLK_CRYPTO_PLL_SEL_CPLL 1U
#define CLK_CRYPTO_PLL_SEL_NPLL 2U
#define CLK_CRYPTO_APK_SEL      0x020E0019U
#define CLK_CRYPTO_APK_SEL_GPLL 0U
#define CLK_CRYPTO_APK_SEL_CPLL 1U
#define CLK_CRYPTO_APK_SEL_NPLL 2U
/********Name=CLKSEL_CON26,Offset=0x168********/
#define CLK_PDM_PLL_SEL              0x0208001AU
#define CLK_PDM_PLL_SEL_GPLL         0U
#define CLK_PDM_PLL_SEL_XIN_OSC0     1U
#define CLK_PDM_PLL_SEL_NPLL         2U
#define CLK_PDM_SEL                  0x010F001AU
#define CLK_PDM_SEL_CLK_PDM          0U
#define CLK_PDM_SEL_CLK_PDM_FRAC_OUT 1U
/********Name=CLKSEL_CON27,Offset=0x16C********/
/********Name=CLKSEL_CON28,Offset=0x170********/
#define CLK_I2S0_TX_PLL_SEL                             0x0108001CU
#define CLK_I2S0_TX_PLL_SEL_GPLL                        0U
#define CLK_I2S0_TX_PLL_SEL_NPLL                        1U
#define CLK_I2S0_TX_SEL                                 0x020A001CU
#define CLK_I2S0_TX_SEL_CLK_I2S0_TX                     0U
#define CLK_I2S0_TX_SEL_CLK_I2S0_TX_FRAC_OUT            1U
#define CLK_I2S0_TX_SEL_MCLK_I2S0_TX_IN                 2U
#define CLK_I2S0_TX_SEL_XIN_OSC0_HALF                   3U
#define CLK_I2S0_TX_RX_CLK_SEL                          0x010C001CU
#define CLK_I2S0_TX_RX_CLK_SEL_CLK_I2S0_TX_CLK          0U
#define CLK_I2S0_TX_RX_CLK_SEL_CLK_I2S0_RX_CLK          1U
#define CLK_I2S0_TX_OUT_MCLK_SEL                        0x020E001CU
#define CLK_I2S0_TX_OUT_MCLK_SEL_CLK_I2S0_TX_RX_CLK_SEL 0U
#define CLK_I2S0_TX_OUT_MCLK_SEL_XIN_OSC0_HALF          1U
#define CLK_I2S0_TX_OUT_MCLK_SEL_CLK_I2S0_RX            2U
/********Name=CLKSEL_CON29,Offset=0x174********/
/********Name=CLKSEL_CON30,Offset=0x178********/
#define CLK_I2S1_PLL_SEL                    0x0108001EU
#define CLK_I2S1_PLL_SEL_GPLL               0U
#define CLK_I2S1_PLL_SEL_NPLL               1U
#define CLK_I2S1_SEL                        0x020A001EU
#define CLK_I2S1_SEL_CLK_I2S1               0U
#define CLK_I2S1_SEL_CLK_I2S1_FRAC_OUT      1U
#define CLK_I2S1_SEL_MCLK_I2S1_IN           2U
#define CLK_I2S1_SEL_XIN_OSC0_HALF          3U
#define CLK_I2S1_OUT_MCLK_SEL               0x010F001EU
#define CLK_I2S1_OUT_MCLK_SEL_CLK_I2S1_SEL  0U
#define CLK_I2S1_OUT_MCLK_SEL_XIN_OSC0_HALF 1U
/********Name=CLKSEL_CON31,Offset=0x17C********/
/********Name=CLKSEL_CON32,Offset=0x180********/
#define CLK_I2S2_PLL_SEL                    0x01080020U
#define CLK_I2S2_PLL_SEL_GPLL               0U
#define CLK_I2S2_PLL_SEL_NPLL               1U
#define CLK_I2S2_SEL                        0x020A0020U
#define CLK_I2S2_SEL_CLK_I2S2               0U
#define CLK_I2S2_SEL_CLK_I2S2_FRAC_OUT      1U
#define CLK_I2S2_SEL_MCLK_I2S2_IN           2U
#define CLK_I2S2_SEL_XIN_OSC0_HALF          3U
#define CLK_I2S2_OUT_MCLK_SEL               0x010F0020U
#define CLK_I2S2_OUT_MCLK_SEL_CLK_I2S2_SEL  0U
#define CLK_I2S2_OUT_MCLK_SEL_XIN_OSC0_HALF 1U
/********Name=CLKSEL_CON33,Offset=0x184********/
/********Name=CLKSEL_CON34,Offset=0x188********/
#define CLK_UART1_PLL_SEL            0x020E0022U
#define CLK_UART1_PLL_SEL_GPLL       0U
#define CLK_UART1_PLL_SEL_XIN_OSC0   1U
#define CLK_UART1_PLL_SEL_USBPHY480M 2U
#define CLK_UART1_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON35,Offset=0x18C********/
#define CLK_UART1_SEL                    0x020E0023U
#define CLK_UART1_SEL_CLK_UART1          0U
#define CLK_UART1_SEL_CLK_UART1_NP5      1U
#define CLK_UART1_SEL_CLK_UART1_FRAC_OUT 2U
/********Name=CLKSEL_CON36,Offset=0x190********/
/********Name=CLKSEL_CON37,Offset=0x194********/
#define CLK_UART2_PLL_SEL            0x020E0025U
#define CLK_UART2_PLL_SEL_GPLL       0U
#define CLK_UART2_PLL_SEL_XIN_OSC0   1U
#define CLK_UART2_PLL_SEL_USBPHY480M 2U
#define CLK_UART2_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON38,Offset=0x198********/
#define CLK_UART2_SEL                    0x020E0026U
#define CLK_UART2_SEL_CLK_UART2          0U
#define CLK_UART2_SEL_CLK_UART2_NP5      1U
#define CLK_UART2_SEL_CLK_UART2_FRAC_OUT 2U
/********Name=CLKSEL_CON39,Offset=0x19C********/
/********Name=CLKSEL_CON40,Offset=0x1A0********/
#define CLK_UART3_PLL_SEL            0x020E0028U
#define CLK_UART3_PLL_SEL_GPLL       0U
#define CLK_UART3_PLL_SEL_XIN_OSC0   1U
#define CLK_UART3_PLL_SEL_USBPHY480M 2U
#define CLK_UART3_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON41,Offset=0x1A4********/
#define CLK_UART3_SEL                    0x020E0029U
#define CLK_UART3_SEL_CLK_UART3          0U
#define CLK_UART3_SEL_CLK_UART3_NP5      1U
#define CLK_UART3_SEL_CLK_UART3_FRAC_OUT 2U
/********Name=CLKSEL_CON42,Offset=0x1A8********/
/********Name=CLKSEL_CON43,Offset=0x1AC********/
#define CLK_UART4_PLL_SEL            0x020E002BU
#define CLK_UART4_PLL_SEL_GPLL       0U
#define CLK_UART4_PLL_SEL_XIN_OSC0   1U
#define CLK_UART4_PLL_SEL_USBPHY480M 2U
#define CLK_UART4_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON44,Offset=0x1B0********/
#define CLK_UART4_SEL                    0x020E002CU
#define CLK_UART4_SEL_CLK_UART4          0U
#define CLK_UART4_SEL_CLK_UART4_NP5      1U
#define CLK_UART4_SEL_CLK_UART4_FRAC_OUT 2U
/********Name=CLKSEL_CON45,Offset=0x1B4********/
/********Name=CLKSEL_CON46,Offset=0x1B8********/
#define CLK_UART5_PLL_SEL            0x020E002EU
#define CLK_UART5_PLL_SEL_GPLL       0U
#define CLK_UART5_PLL_SEL_XIN_OSC0   1U
#define CLK_UART5_PLL_SEL_USBPHY480M 2U
#define CLK_UART5_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON47,Offset=0x1BC********/
#define CLK_UART5_SEL                    0x020E002FU
#define CLK_UART5_SEL_CLK_UART5          0U
#define CLK_UART5_SEL_CLK_UART5_NP5      1U
#define CLK_UART5_SEL_CLK_UART5_FRAC_OUT 2U
/********Name=CLKSEL_CON48,Offset=0x1C0********/
/********Name=CLKSEL_CON49,Offset=0x1C4********/
#define CLK_I2C0_PLL_SEL          0x01070031U
#define CLK_I2C0_PLL_SEL_GPLL     0U
#define CLK_I2C0_PLL_SEL_XIN_OSC0 1U
#define CLK_I2C1_PLL_SEL          0x010F0031U
#define CLK_I2C1_PLL_SEL_GPLL     0U
#define CLK_I2C1_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON50,Offset=0x1C8********/
#define CLK_I2C2_PLL_SEL          0x01070032U
#define CLK_I2C2_PLL_SEL_GPLL     0U
#define CLK_I2C2_PLL_SEL_XIN_OSC0 1U
#define CLK_I2C3_PLL_SEL          0x010F0032U
#define CLK_I2C3_PLL_SEL_GPLL     0U
#define CLK_I2C3_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON52,Offset=0x1D0********/
#define CLK_PWM0_PLL_SEL          0x01070034U
#define CLK_PWM0_PLL_SEL_XIN_OSC0 0U
#define CLK_PWM1_PLL_SEL          0x010F0034U
#define CLK_PWM1_PLL_SEL_GPLL     0U
#define CLK_PWM1_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON53,Offset=0x1D4********/
#define CLK_SPI0_PLL_SEL          0x01070035U
#define CLK_SPI0_PLL_SEL_GPLL     0U
#define CLK_SPI0_PLL_SEL_XIN_OSC0 1U
#define CLK_SPI1_PLL_SEL          0x010F0035U
#define CLK_SPI1_PLL_SEL_GPLL     0U
#define CLK_SPI1_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON54,Offset=0x1D8********/
/********Name=CLKSEL_CON55,Offset=0x1DC********/
/********Name=CLKSEL_CON56,Offset=0x1E0********/
#define CLK_OTP_PLL_CLK_SEL      0x01080038U
#define CLK_OTP_PLL_CLK_SEL_GPLL 0U
#define CLK_CPU_PVTPLL_REF_SEL   0x010A0038U
#define CLK_CPU_PVTPLL_OSC_SEL   0x010B0038U
#define CLK_CPU_PVTPLL_SRC_SEL   0x010C0038U
#define CLK_GPU_PVTPLL_REF_SEL   0x010D0038U
#define CLK_GPU_PVTPLL_OSC_SEL   0x010E0038U
#define CLK_GPU_PVTPLL_SRC_SEL   0x010F0038U
/********Name=CLKSEL_CON57,Offset=0x1E4********/
#define TESTCLK_SEL         0x05000039U
#define TESTCLK_SEL_CLK_OTP 0U
/********Name=CLKSEL_CON58,Offset=0x1E8********/
#define CLK_I2S0_RX_PLL_SEL                             0x0108003AU
#define CLK_I2S0_RX_PLL_SEL_GPLL                        0U
#define CLK_I2S0_RX_PLL_SEL_NPLL                        1U
#define CLK_I2S0_RX_SEL                                 0x020A003AU
#define CLK_I2S0_RX_SEL_CLK_I2S0_RX                     0U
#define CLK_I2S0_RX_SEL_CLK_I2S0_RX_FRAC_OUT            1U
#define CLK_I2S0_RX_SEL_MCLK_I2S0_RX_IN                 2U
#define CLK_I2S0_RX_SEL_XIN_OSC0_HALF                   3U
#define CLK_I2S0_RX_TX_CLK_SEL                          0x010C003AU
#define CLK_I2S0_RX_TX_CLK_SEL_CLK_I2S0_RX_CLK          0U
#define CLK_I2S0_RX_TX_CLK_SEL_CLK_I2S0_TX_CLK          1U
#define CLK_I2S0_RX_OUT_MCLK_SEL                        0x020E003AU
#define CLK_I2S0_RX_OUT_MCLK_SEL_CLK_I2S0_RX_TX_CLK_SEL 0U
#define CLK_I2S0_RX_OUT_MCLK_SEL_XIN_OSC0_HALF          1U
#define CLK_I2S0_RX_OUT_MCLK_SEL_CLK_I2S0_TX            2U
/********Name=CLKSEL_CON59,Offset=0x1EC********/
/********Name=PMU_CLKSEL_CON0,Offset=0xC040********/
#define CLK_RTC32K_CLK_SEL 0x020E0000U

/********Name=PMU_CLKSEL_CON1,Offset=0xC044********/
/********Name=PMU_CLKSEL_CON2,Offset=0xC048********/
#define USBPHY_REF_SEL 0x01060002U

#define MIPIDSIPHY_REF_SEL 0x01070002U

#define CLK_WIFI_SEL 0x010F0002U

/********Name=PMU_CLKSEL_CON3,Offset=0xC04C********/
#define CLK_UART0_PLL_SEL            0x020E0003U
#define CLK_UART0_PLL_SEL_GPLL       0U
#define CLK_UART0_PLL_SEL_XIN_OSC0   1U
#define CLK_UART0_PLL_SEL_USBPHY480M 2U
#define CLK_UART0_PLL_SEL_NPLL       3U
/********Name=PMU_CLKSEL_CON4,Offset=0xC050********/
#define CLK_UART0_SEL                    0x020E0004U
#define CLK_UART0_SEL_CLK_UART0          0U
#define CLK_UART0_SEL_CLK_UART0_NP5      1U
#define CLK_UART0_SEL_CLK_UART0_FRAC_OUT 2U
/********Name=PMU_CLKSEL_CON5,Offset=0xC054********/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK3358_H */

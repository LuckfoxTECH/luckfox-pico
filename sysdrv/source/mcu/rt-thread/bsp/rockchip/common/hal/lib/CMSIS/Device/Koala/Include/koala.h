/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __KOALA_H
#define __KOALA_H
#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* GRF Register Structure Define */
struct GRF_REG {
    __IO uint32_t SOC_CON0;                           /* Address Offset: 0x0000 */
    __IO uint32_t SOC_CON1;                           /* Address Offset: 0x0004 */
    __IO uint32_t SOC_CON2;                           /* Address Offset: 0x0008 */
    __IO uint32_t SOC_CON3;                           /* Address Offset: 0x000C */
    __IO uint32_t SOC_CON4;                           /* Address Offset: 0x0010 */
    __IO uint32_t SOC_CON5;                           /* Address Offset: 0x0014 */
         uint32_t RESERVED0018[2];                    /* Address Offset: 0x0018 */
    __IO uint32_t SOC_CON8;                           /* Address Offset: 0x0020 */
         uint32_t RESERVED0024[6];                    /* Address Offset: 0x0024 */
    __IO uint32_t GPIOMUX_PLUS;                       /* Address Offset: 0x003C */
    __IO uint32_t GPIO0A_IOMUX;                       /* Address Offset: 0x0040 */
    __IO uint32_t GPIO0B_IOMUX;                       /* Address Offset: 0x0044 */
         uint32_t RESERVED0048[2];                    /* Address Offset: 0x0048 */
    __IO uint32_t GPIO1A_IOMUX;                       /* Address Offset: 0x0050 */
    __IO uint32_t GPIO1B_IOMUX;                       /* Address Offset: 0x0054 */
    __IO uint32_t GPIO1C_IOMUX;                       /* Address Offset: 0x0058 */
         uint32_t RESERVED005C;                       /* Address Offset: 0x005C */
    __IO uint32_t GPIO2A_IOMUX;                       /* Address Offset: 0x0060 */
    __IO uint32_t GPIO2B_IOMUX;                       /* Address Offset: 0x0064 */
    __IO uint32_t GPIO2C_IOMUX;                       /* Address Offset: 0x0068 */
    __IO uint32_t GPIO2D_IOMUX;                       /* Address Offset: 0x006C */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0070 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0074 */
         uint32_t RESERVED0078[2];                    /* Address Offset: 0x0078 */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0080 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0084 */
    __IO uint32_t GPIO1C_P;                           /* Address Offset: 0x0088 */
         uint32_t RESERVED008C;                       /* Address Offset: 0x008C */
    __IO uint32_t GPIO2A_P;                           /* Address Offset: 0x0090 */
    __IO uint32_t GPIO2B_P;                           /* Address Offset: 0x0094 */
    __IO uint32_t GPIO2C_P;                           /* Address Offset: 0x0098 */
    __IO uint32_t GPIO2D_P;                           /* Address Offset: 0x009C */
    __IO uint32_t GPIO0A_E;                           /* Address Offset: 0x00A0 */
    __IO uint32_t GPIO0B_E;                           /* Address Offset: 0x00A4 */
         uint32_t RESERVED00A8[2];                    /* Address Offset: 0x00A8 */
    __IO uint32_t GPIO1A_E;                           /* Address Offset: 0x00B0 */
    __IO uint32_t GPIO1B_E;                           /* Address Offset: 0x00B4 */
    __IO uint32_t GPIO1C_E;                           /* Address Offset: 0x00B8 */
         uint32_t RESERVED00BC;                       /* Address Offset: 0x00BC */
    __IO uint32_t GPIO2A_E;                           /* Address Offset: 0x00C0 */
    __IO uint32_t GPIO2B_E;                           /* Address Offset: 0x00C4 */
    __IO uint32_t GPIO2C_E;                           /* Address Offset: 0x00C8 */
    __IO uint32_t GPIO2D_E;                           /* Address Offset: 0x00CC */
    __IO uint32_t GPIO0A_S;                           /* Address Offset: 0x00D0 */
    __IO uint32_t GPIO0B_S;                           /* Address Offset: 0x00D4 */
         uint32_t RESERVED00D8[2];                    /* Address Offset: 0x00D8 */
    __IO uint32_t GPIO1A_S;                           /* Address Offset: 0x00E0 */
    __IO uint32_t GPIO1B_S;                           /* Address Offset: 0x00E4 */
    __IO uint32_t GPIO1C_S;                           /* Address Offset: 0x00E8 */
         uint32_t RESERVED00EC;                       /* Address Offset: 0x00EC */
    __IO uint32_t GPIO2A_S;                           /* Address Offset: 0x00F0 */
    __IO uint32_t GPIO2B_S;                           /* Address Offset: 0x00F4 */
    __IO uint32_t GPIO2C_S;                           /* Address Offset: 0x00F8 */
    __IO uint32_t GPIO2D_S;                           /* Address Offset: 0x00FC */
    __IO uint32_t MCU_STCALIB;                        /* Address Offset: 0x0100 */
    __IO uint32_t MCU_CON;                            /* Address Offset: 0x0104 */
         uint32_t RESERVED0108[14];                   /* Address Offset: 0x0108 */
    __I  uint32_t SOC_STATUS0;                        /* Address Offset: 0x0140 */
    __I  uint32_t MCU_STATUS;                         /* Address Offset: 0x0144 */
    __I  uint32_t BT_DIG;                             /* Address Offset: 0x0148 */
    __I  uint32_t BLE_DIG;                            /* Address Offset: 0x014C */
    __I  uint32_t BTDM_DIG;                           /* Address Offset: 0x0150 */
         uint32_t RESERVED0154[11];                   /* Address Offset: 0x0154 */
    __IO uint32_t DSP_CON0;                           /* Address Offset: 0x0180 */
    __IO uint32_t DSP_CON1;                           /* Address Offset: 0x0184 */
    __IO uint32_t DSP_CON2;                           /* Address Offset: 0x0188 */
         uint32_t RESERVED018C;                       /* Address Offset: 0x018C */
    __I  uint32_t DSP_STAT0;                          /* Address Offset: 0x0190 */
    __I  uint32_t DSP_STAT1;                          /* Address Offset: 0x0194 */
         uint32_t RESERVED0198[2];                    /* Address Offset: 0x0198 */
    __IO uint32_t MEM_CON0;                           /* Address Offset: 0x01A0 */
    __IO uint32_t MEM_CON1;                           /* Address Offset: 0x01A4 */
    __IO uint32_t MEM_CON2;                           /* Address Offset: 0x01A8 */
         uint32_t RESERVED01AC[21];                   /* Address Offset: 0x01AC */
    __IO uint32_t OS_REG0;                            /* Address Offset: 0x0200 */
    __IO uint32_t OS_REG1;                            /* Address Offset: 0x0204 */
    __IO uint32_t OS_REG2;                            /* Address Offset: 0x0208 */
    __IO uint32_t OS_REG3;                            /* Address Offset: 0x020C */
         uint32_t RESERVED0210[28];                   /* Address Offset: 0x0210 */
    __IO uint32_t BBGEN_CON0;                         /* Address Offset: 0x0280 */
    __IO uint32_t BBGEN_CON1;                         /* Address Offset: 0x0284 */
    __IO uint32_t BBGEN_CON2;                         /* Address Offset: 0x0288 */
    __IO uint32_t BBGEN_CON3;                         /* Address Offset: 0x028C */
    __I  uint32_t BBGEN_STATUS;                       /* Address Offset: 0x0290 */
         uint32_t RESERVED0294[11];                   /* Address Offset: 0x0294 */
    __IO uint32_t DLL_CON0;                           /* Address Offset: 0x02C0 */
    __IO uint32_t DLL_CON1;                           /* Address Offset: 0x02C4 */
    __I  uint32_t DLL_STATUS0;                        /* Address Offset: 0x02C8 */
    __I  uint32_t DLL_STATUS1;                        /* Address Offset: 0x02CC */
         uint32_t RESERVED02D0[12];                   /* Address Offset: 0x02D0 */
    __IO uint32_t FAST_BOOT_EN;                       /* Address Offset: 0x0300 */
    __IO uint32_t FAST_BOOT_ADDR;                     /* Address Offset: 0x0304 */
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
         uint32_t RESERVED0[2];                       /* Address Offset: 0x0048 */
    __IO uint32_t FIFO_CTRL;                          /* Address Offset: 0x0050 */
    __IO uint32_t FIFO_INTSTS;                        /* Address Offset: 0x0054 */
    __IO uint32_t FIFO_TOUTTHR;                       /* Address Offset: 0x0058 */
    __IO uint32_t VERSION_ID;                         /* Address Offset: 0x005C */
    __I  uint32_t FIFO;                               /* Address Offset: 0x0060 */
         uint32_t RESERVED1[7];                       /* Address Offset: 0x0064 */
    __IO uint32_t PWRMATCH_CTRL;                      /* Address Offset: 0x0080 */
    __IO uint32_t PWRMATCH_LPRE;                      /* Address Offset: 0x0084 */
    __IO uint32_t PWRMATCH_HPRE;                      /* Address Offset: 0x0088 */
    __IO uint32_t PWRMATCH_LD;                        /* Address Offset: 0x008C */
    __IO uint32_t PWRMATCH_HD_ZERO;                   /* Address Offset: 0x0090 */
    __IO uint32_t PWRMATCH_HD_ONE;                    /* Address Offset: 0x0094 */
    __IO uint32_t PWRMATCH_VALUE[10];                 /* Address Offset: 0x0098 */
         uint32_t RESERVED2[3];                       /* Address Offset: 0x00C0 */
    __I  uint32_t PWM3_PWRCAPTURE_VALUE;              /* Address Offset: 0x00CC */
    __IO uint32_t FILTER_CTRL;                        /* Address Offset: 0x00D0 */
};
/* TIMER Register Structure Define */
#define TIMER_CHAN_CNT   6
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __O  uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
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
/* WDT Register Structure Define */
struct WDT_REG {
    __IO uint32_t CR;                             /* Address Offset: 0x0000 */
    __IO uint32_t TORR;                           /* Address Offset: 0x0004 */
    __I  uint32_t CCVR;                           /* Address Offset: 0x0008 */
    __O  uint32_t CRR;                            /* Address Offset: 0x000C */
    __I  uint32_t STAT;                           /* Address Offset: 0x0010 */
    __I  uint32_t EOI;                            /* Address Offset: 0x0014 */
};
/* MBOX Register Structure Define */
#define MBOX_CNT             1
#define MBOX_CHAN_CNT        4
struct MBOX_CMD_DAT {
    __IO uint32_t CMD;
    __IO uint32_t DATA;
};
struct MBOX_REG {
    __IO uint32_t A2B_INTEN;                          /* Address Offset: 0x0000 */
    __IO uint32_t A2B_STATUS;                         /* Address Offset: 0x0004 */
    struct MBOX_CMD_DAT A2B[MBOX_CHAN_CNT];           /* Address Offset: 0x0008 */
    __IO uint32_t B2A_INTEN;                          /* Address Offset: 0x0028 */
    __IO uint32_t B2A_STATUS;                         /* Address Offset: 0x002C */
    struct MBOX_CMD_DAT B2A[MBOX_CHAN_CNT];           /* Address Offset: 0x0030 */
         uint32_t RESERVED0080[44];                   /* Address Offset: 0x0050 */
    __IO uint32_t ATOMIC_LOCK[32];                    /* Address Offset: 0x0100 */
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
    __IO uint32_t SR;                                 /* Address Offset: 0x0024 */
    __IO uint32_t IPR;                                /* Address Offset: 0x0028 */
    __IO uint32_t IMR;                                /* Address Offset: 0x002C */
    __IO uint32_t ISR;                                /* Address Offset: 0x0030 */
    __IO uint32_t RISR;                               /* Address Offset: 0x0034 */
    __O  uint32_t ICR;                                /* Address Offset: 0x0038 */
    __IO uint32_t DMACR;                              /* Address Offset: 0x003C */
    __IO uint32_t DMATDLR;                            /* Address Offset: 0x0040 */
    __IO uint32_t DMARDLR;                            /* Address Offset: 0x0044 */
         uint32_t RESERVED0072;                       /* Address Offset: 0x0048 */
    __IO uint32_t TIMEOUT;                            /* Address Offset: 0x004C */
    __IO uint32_t BYPASS;                             /* Address Offset: 0x0050 */
         uint32_t RESERVED0084[235];                  /* Address Offset: 0x0054 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0400 */
         uint32_t RESERVED1028[255];                  /* Address Offset: 0x0404 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x0800 */
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
         uint32_t RESERVED0040[54];                   /* Address Offset: 0x0028 */
    __IO uint32_t TXDATA[8];                          /* Address Offset: 0x0100 */
         uint32_t RESERVED0288[56];                   /* Address Offset: 0x0120 */
    __I  uint32_t RXDATA[8];                          /* Address Offset: 0x0200 */
    __I  uint32_t ST;                                 /* Address Offset: 0x0220 */
    __IO uint32_t DBGCTRL;                            /* Address Offset: 0x0224 */
};
/* EFUSE Register Structure Define */
struct EFUSE_REG {
    __IO uint32_t MOD;                                /* Address Offset: 0x0000 */
    __IO uint32_t RD_MASK;                            /* Address Offset: 0x0004 */
    __IO uint32_t PG_MASK;                            /* Address Offset: 0x0008 */
         uint32_t RESERVED0012[2];                    /* Address Offset: 0x000C */
    __IO uint32_t INT_CON;                            /* Address Offset: 0x0014 */
    __O  uint32_t INT_STATUS;                         /* Address Offset: 0x0018 */
    __IO uint32_t USER_CTRL;                          /* Address Offset: 0x001C */
    __I  uint32_t DOUT;                               /* Address Offset: 0x0020 */
    __IO uint32_t AUTO_CTRL;                          /* Address Offset: 0x0024 */
    __IO uint32_t T_CSB_P;                            /* Address Offset: 0x0028 */
    __IO uint32_t T_PGENB_P;                          /* Address Offset: 0x002C */
    __IO uint32_t T_LOAD_P;                           /* Address Offset: 0x0030 */
    __IO uint32_t T_ADDR_P;                           /* Address Offset: 0x0034 */
    __IO uint32_t T_STROBE_P;                         /* Address Offset: 0x0038 */
    __IO uint32_t T_CSB_R;                            /* Address Offset: 0x003C */
    __IO uint32_t T_PGENB_R;                          /* Address Offset: 0x0040 */
    __IO uint32_t T_LOAD_R;                           /* Address Offset: 0x0044 */
    __IO uint32_t T_ADDR_R;                           /* Address Offset: 0x0048 */
    __IO uint32_t T_STROBE_R;                         /* Address Offset: 0x004C */
    __IO uint32_t REVISION;                           /* Address Offset: 0x0050 */
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
         uint32_t PADDING[3];
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
         uint32_t RESERVED0060;                       /* Address Offset: 0x003C */
    __I  uint32_t FTR[6];                             /* Address Offset: 0x0040 */
         uint32_t RESERVED0088[42];                   /* Address Offset: 0x0058 */
    struct DMA_CHANNEL_STATUS CHAN_STS[6];            /* Address Offset: 0x0100 */
         uint32_t RESERVED0304[180];                  /* Address Offset: 0x0130 */
    struct DMA_CHANNEL_CONFIG CHAN_CFG[6];            /* Address Offset: 0x0400 */
         uint32_t RESERVED1216[528];                  /* Address Offset: 0x04C0 */
    __I  uint32_t DBGSTATUS;                          /* Address Offset: 0x0D00 */
    __O  uint32_t DBGCMD;                             /* Address Offset: 0x0D04 */
    __O  uint32_t DBGINST[2];                         /* Address Offset: 0x0D08 */
         uint32_t RESERVED3344[60];                   /* Address Offset: 0x0D10 */
    __I  uint32_t CR[5];                              /* Address Offset: 0x0E00 */
    __I  uint32_t CRDN;                               /* Address Offset: 0x0E14 */
         uint32_t RESERVED3608[26];                   /* Address Offset: 0x0E18 */
    __IO uint32_t WD;                                 /* Address Offset: 0x0E80 */
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
         uint32_t RESERVED0076;                       /* Address Offset: 0x004C */
    __I  uint32_t INT_STATUS;                         /* Address Offset: 0x0050 */
         uint32_t RESERVED0084;                       /* Address Offset: 0x0054 */
    __I  uint32_t INT_RAWSTATUS;                      /* Address Offset: 0x0058 */
         uint32_t RESERVED0092;                       /* Address Offset: 0x005C */
    __O  uint32_t PORT_EOI_L;                         /* Address Offset: 0x0060 */
    __O  uint32_t PORT_EOI_H;                         /* Address Offset: 0x0064 */
         uint32_t RESERVED0104[2];                    /* Address Offset: 0x0068 */
    __I  uint32_t EXT_PORT;                           /* Address Offset: 0x0070 */
         uint32_t RESERVED0116;                       /* Address Offset: 0x0074 */
    __I  uint32_t VER_ID;                             /* Address Offset: 0x0078 */
};
/* FSPI Register Structure Define */
struct FSPI_REG {
    __IO uint32_t CTRL0;                              /* Address Offset: 0x0000 */
    __IO uint32_t IMR;                                /* Address Offset: 0x0004 */
    __O  uint32_t ICLR;                               /* Address Offset: 0x0008 */
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
    __IO uint32_t POLL_CTRL;                          /* Address Offset: 0x0038 */
    __IO uint32_t DLL_CTRL0;                          /* Address Offset: 0x003C */
    __IO uint32_t HRDYMASK;                           /* Address Offset: 0x0040 */
    __IO uint32_t EXT_AX;                             /* Address Offset: 0x0044 */
    __IO uint32_t SCLK_INATM_CNT;                     /* Address Offset: 0x0048 */
    __IO uint32_t AUTO_RF_CNT;                        /* Address Offset: 0x004C */
    __O  uint32_t XMMC_WCMD0;                         /* Address Offset: 0x0050 */
    __O  uint32_t XMMC_RCMD0;                         /* Address Offset: 0x0054 */
    __IO uint32_t XMMC_CTRL;                          /* Address Offset: 0x0058 */
    __IO uint32_t MODE;                               /* Address Offset: 0x005C */
    __IO uint32_t DEVRGN;                             /* Address Offset: 0x0060 */
    __IO uint32_t DEVSIZE0;                           /* Address Offset: 0x0064 */
    __IO uint32_t TME0;                               /* Address Offset: 0x0068 */
    __IO uint32_t POLLDLY_CTRL;                       /* Address Offset: 0x006C */
         uint32_t RESERVED0112[4];                    /* Address Offset: 0x0070 */
    __O  uint32_t DMATR;                              /* Address Offset: 0x0080 */
    __IO uint32_t DMAADDR;                            /* Address Offset: 0x0084 */
         uint32_t RESERVED0136[2];                    /* Address Offset: 0x0088 */
    __I  uint32_t POLL_DATA;                          /* Address Offset: 0x0090 */
    __IO uint32_t XMMCSR;                             /* Address Offset: 0x0094 */
         uint32_t RESERVED0152[26];                   /* Address Offset: 0x0098 */
    __O  uint32_t CMD;                                /* Address Offset: 0x0100 */
    __O  uint32_t ADDR;                               /* Address Offset: 0x0104 */
    __IO uint32_t DATA;                               /* Address Offset: 0x0108 */
         uint32_t RESERVED0268[61];                   /* Address Offset: 0x010C */
    __IO uint32_t CTRL1;                              /* Address Offset: 0x0200 */
         uint32_t RESERVED0516[4];                    /* Address Offset: 0x0204 */
    __IO uint32_t AX1;                                /* Address Offset: 0x0214 */
    __IO uint32_t ABIT1;                              /* Address Offset: 0x0218 */
         uint32_t RESERVED0540[8];                    /* Address Offset: 0x021C */
    __IO uint32_t DLL_CTRL1;                          /* Address Offset: 0x023C */
         uint32_t RESERVED0576[4];                    /* Address Offset: 0x0240 */
    __O  uint32_t XMMC_WCMD1;                         /* Address Offset: 0x0250 */
    __O  uint32_t XMMC_RCMD1;                         /* Address Offset: 0x0254 */
         uint32_t RESERVED0600[3];                    /* Address Offset: 0x0258 */
    __IO uint32_t DEVSIZE1;                           /* Address Offset: 0x0264 */
    __IO uint32_t TME1;                               /* Address Offset: 0x0268 */
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
         uint32_t RESERVED0056[34];                   /* Address Offset: 0x0038 */
    __IO uint32_t FIQ_INTEN;                          /* Address Offset: 0x00C0 */
    __IO uint32_t FIQ_INTMASK;                        /* Address Offset: 0x00C4 */
    __IO uint32_t FIQ_INTFORCE;                       /* Address Offset: 0x00C8 */
    __I  uint32_t FIQ_RAWSTATUS;                      /* Address Offset: 0x00CC */
    __I  uint32_t FIQ_STATUS;                         /* Address Offset: 0x00D0 */
    __I  uint32_t FIQ_FINALSTATUS;                    /* Address Offset: 0x00D4 */
    __IO uint32_t IRQ_PLEVEL;                         /* Address Offset: 0x00D8 */
         uint32_t RESERVED0220[3];                    /* Address Offset: 0x00DC */
    __IO uint32_t IRQ_PR_OFFSET;                      /* Address Offset: 0x00E8 */
         uint32_t RESERVED0236[195];                  /* Address Offset: 0x00EC */
    __I  uint32_t AHB_ICTL_COMP_VERSION;              /* Address Offset: 0x03F8 */
    __I  uint32_t ICTL_COMP_TYPE;                     /* Address Offset: 0x03FC */
};
/* PMU Register Structure Define */
struct PMU_REG {
    __IO uint32_t WAKEUP_CFG[3];                      /* Address Offset: 0x0000 */
    __IO uint32_t PWRDN_CON;                          /* Address Offset: 0x000C */
    __IO uint32_t PMU_NOC_AUTO_ENA;                   /* Address Offset: 0x0010 */
         uint32_t RESERVED0020[3];                    /* Address Offset: 0x0014 */
    __I  uint32_t PWRDN_ST;                           /* Address Offset: 0x0020 */
    __IO uint32_t PLL_CON;                            /* Address Offset: 0x0024 */
    __IO uint32_t PWRMODE_CON;                        /* Address Offset: 0x0028 */
    __IO uint32_t SFT_CON;                            /* Address Offset: 0x002C */
    __IO uint32_t INT_CON;                            /* Address Offset: 0x0030 */
    __IO uint32_t INT_ST;                             /* Address Offset: 0x0034 */
    __IO uint32_t PWRMODE_GPIO_POS_INT_CON;           /* Address Offset: 0x0038 */
    __IO uint32_t PWRMODE_GPIO_NEG_INT_CON;           /* Address Offset: 0x003C */
    __O  uint32_t PWRMODE_GPIO_POS_INT_ST;            /* Address Offset: 0x0040 */
    __O  uint32_t PWRMODE_GPIO_NEG_INT_ST;            /* Address Offset: 0x0044 */
    __IO uint32_t PWRDN_INTEN;                        /* Address Offset: 0x0048 */
    __O  uint32_t PWRDN_STATUS;                       /* Address Offset: 0x004C */
    __O  uint32_t WAKEUP_STATUS;                      /* Address Offset: 0x0050 */
    __IO uint32_t BUS_CLR;                            /* Address Offset: 0x0054 */
    __IO uint32_t BUS_IDLE_REQ;                       /* Address Offset: 0x0058 */
    __I  uint32_t BUS_IDLE_ST;                        /* Address Offset: 0x005C */
    __I  uint32_t BUS_IDLE_ACK;                       /* Address Offset: 0x0060 */
    __I  uint32_t POWER_ST;                           /* Address Offset: 0x0064 */
    __I  uint32_t CORE_PWR_ST;                        /* Address Offset: 0x0068 */
    __IO uint32_t OSC_CNT;                            /* Address Offset: 0x006C */
    __IO uint32_t PLLLOCK_CNT;                        /* Address Offset: 0x0070 */
    __IO uint32_t PWRMODE_TIMEOUT_CNT;                /* Address Offset: 0x0074 */
    __IO uint32_t WAKEUP_RST_CLT_CNT;                 /* Address Offset: 0x0078 */
    __IO uint32_t PMU_STABLE_CNT;                     /* Address Offset: 0x007C */
    __IO uint32_t SYS_REG[4];                         /* Address Offset: 0x0080 */
};
/* I2STDM Register Structure Define */
struct I2STDM_REG {
    __IO uint32_t TXCR;                               /* Address Offset: 0x0000 */
    __IO uint32_t RXCR;                               /* Address Offset: 0x0004 */
    __IO uint32_t CKR;                                /* Address Offset: 0x0008 */
    __IO uint32_t TXFIFOLR;                           /* Address Offset: 0x000C */
    __IO uint32_t DMACR;                              /* Address Offset: 0x0010 */
    __IO uint32_t INTCR;                              /* Address Offset: 0x0014 */
    __IO uint32_t INTSR;                              /* Address Offset: 0x0018 */
    __IO uint32_t XFER;                               /* Address Offset: 0x001C */
    __IO uint32_t CLR;                                /* Address Offset: 0x0020 */
    __IO uint32_t TXDR;                               /* Address Offset: 0x0024 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x0028 */
    __IO uint32_t RXFIFOLR;                           /* Address Offset: 0x002C */
    __IO uint32_t TDM_TXCTRL;                         /* Address Offset: 0x0030 */
    __IO uint32_t TDM_RXCTRL;                         /* Address Offset: 0x0034 */
    __IO uint32_t CLKDIV;                             /* Address Offset: 0x0038 */
    __IO uint32_t VERSION;                            /* Address Offset: 0x003C */
};
/* PDM Register Structure Define */
struct PDM_REG {
    __IO uint32_t SYSCONFIG;                          /* Address Offset: 0x0000 */
    __IO uint32_t CTRL[2];                            /* Address Offset: 0x0004 */
    __IO uint32_t CLK_CTRL;                           /* Address Offset: 0x000C */
    __IO uint32_t HPF_CTRL;                           /* Address Offset: 0x0010 */
    __IO uint32_t FIFO_CTRL;                          /* Address Offset: 0x0014 */
    __IO uint32_t DMA_CTRL;                           /* Address Offset: 0x0018 */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x001C */
    __O  uint32_t INT_CLR;                            /* Address Offset: 0x0020 */
    __I  uint32_t INT_ST;                             /* Address Offset: 0x0024 */
         uint32_t RESERVED0040[2];                    /* Address Offset: 0x0028 */
    __I  uint32_t RXFIFO_DATA_REG;                    /* Address Offset: 0x0030 */
    __I  uint32_t DATA0R_REG;                         /* Address Offset: 0x0034 */
    __I  uint32_t DATA0L_REG;                         /* Address Offset: 0x0038 */
    __I  uint32_t DATA1R_REG;                         /* Address Offset: 0x003C */
    __I  uint32_t DATA1L_REG;                         /* Address Offset: 0x0040 */
    __I  uint32_t DATA2R_REG;                         /* Address Offset: 0x0044 */
    __I  uint32_t DATA2L_REG;                         /* Address Offset: 0x0048 */
    __I  uint32_t DATA3R_REG;                         /* Address Offset: 0x004C */
    __I  uint32_t DATA3L_REG;                         /* Address Offset: 0x0050 */
    __I  uint32_t DATA_VALID;                         /* Address Offset: 0x0054 */
    __I  uint32_t VERSION;                            /* Address Offset: 0x0058 */
         uint32_t RESERVED0092[233];                  /* Address Offset: 0x005C */
    __I  uint32_t INCR_RXDR;                          /* Address Offset: 0x0400 */
};
/* CRU Register Structure Define */
struct CRU_REG {
         uint32_t RESERVED0000[3];                    /* Address Offset: 0x0000 */
    __IO uint32_t MODE_CON;                           /* Address Offset: 0x000C */
         uint32_t RESERVED0016[20];                   /* Address Offset: 0x0010 */
    __IO uint32_t CPLL_CON[5];                        /* Address Offset: 0x0060 */
    __I  uint32_t CPLL_STAT0;                         /* Address Offset: 0x0074 */
         uint32_t RESERVED0120[2];                    /* Address Offset: 0x0078 */
    __IO uint32_t GPLL_CON[5];                        /* Address Offset: 0x0080 */
    __I  uint32_t GPLL_STAT0;                         /* Address Offset: 0x0094 */
         uint32_t RESERVED0152[26];                   /* Address Offset: 0x0098 */
    __IO uint32_t CRU_CLKSEL_CON[18];                 /* Address Offset: 0x0100 */
         uint32_t RESERVED0328[46];                   /* Address Offset: 0x0148 */
    __IO uint32_t CRU_CLKGATE_CON[6];                 /* Address Offset: 0x0200 */
         uint32_t RESERVED0536[58];                   /* Address Offset: 0x0218 */
    __IO uint32_t CRU_SOFTRST_CON[6];                 /* Address Offset: 0x0300 */
         uint32_t RESERVED0792[58];                   /* Address Offset: 0x0318 */
    __IO uint32_t GLB_SRST_FST_VALUE;                 /* Address Offset: 0x0400 */
    __IO uint32_t GLB_SRST_SND_VALUE;                 /* Address Offset: 0x0404 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x0408 */
    __IO uint32_t MISC_CON;                           /* Address Offset: 0x040C */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x0410 */
    __O  uint32_t GLB_RST_ST;                         /* Address Offset: 0x0414 */
};
#endif /* __ASSEMBLY__ */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define DSP_ITCM_BASE       0x30000000U /* DSP itcm base address */
#define DSP_ITCM_END        0x3000ffffU /* DSP itcm end address */
#define DSP_DTCM_BASE       0x30200000U /* DSP dtcm base address */
#define DSP_DTCM_END        0x3027ffffU /* DSP dtcm end address */
#define GRF_BASE            0x44300000U /* GRF base address */
#define TIMER_BASE          0x44000000U /* TIMER base address */
#define UART0_BASE          0x44010000U /* UART0 base address */
#define UART1_BASE          0x44020000U /* UART1 base address */
#define WDT0_BASE           0x44030000U /* WDT0 base address */
#define WDT1_BASE           0x44040000U /* WDT1 base address */
#define MBOX_BASE           0x44050000U /* MBOX base address */
#define SPI1_BASE            0x44060000U /* SPI base address */
#define I2C0_BASE           0x44070000U /* I2C0 base address */
#define I2C1_BASE           0x44080000U /* I2C1 base address */
#define PWM_BASE            0x44090000U /* PWM base address  */
#define EFUSE_BASE          0x440A0000U /* EFUSE base address */
#define DMA_BASE            0x440B0000U /* DMA base address */
#define GPIO1_BASE          0x440C0000U /* GPIO1 base address */
#define GPIO2_BASE          0x440E0000U /* GPIO2 base address */
#define FSPI0_BASE          0x44100000U /* FSPI0 base address */
#define INTC_BASE           0x44110000U /* INTC base address */
#define GPIO0_BASE          0x44320000U /* GPIO0 base address */
#define PMU_BASE            0x44330000U /* PMU base address */
#define I2C2_BASE           0x44350000U /* I2C2 base address */
#define I2STDM0_BASE        0x44420000U /* I2STDM0 base address */
#define PDM0_BASE           0x44440000U /* PDM0 base address */
#define CRU_BASE            0x44500000U /* CRU base address */
#define XIP_MAP0_BASE0      0x50000000U /* FSPI0 map address0 */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define GRF                 ((struct GRF_REG *) GRF_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER_BASE)
#define TIMER1              ((struct TIMER_REG *) (TIMER_BASE + 0x20))
#define TIMER2              ((struct TIMER_REG *) (TIMER_BASE + 0x40))
#define TIMER3              ((struct TIMER_REG *) (TIMER_BASE + 0x60))
#define TIMER4              ((struct TIMER_REG *) (TIMER_BASE + 0x80))
#define UART0               ((struct UART_REG *) UART0_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define WDT0                ((struct WDT_REG *) WDT0_BASE)
#define WDT1                ((struct WDT_REG *) WDT1_BASE)
#define MBOX0               ((struct MBOX_REG *) MBOX_BASE)
#define SPI1                 ((struct SPI_REG *) SPI1_BASE)
#define I2C0                ((struct I2C_REG *) I2C0_BASE)
#define I2C1                ((struct I2C_REG *) I2C1_BASE)
#define PWM		    ((struct PWM_REG *) PWM_BASE)
#define EFUSE               ((struct EFUSE_REG *) EFUSE_BASE)
#define DMA                 ((struct DMA_REG *) DMA_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define GPIO2               ((struct GPIO_REG *) GPIO2_BASE)
#define FSPI0               ((struct FSPI_REG *) FSPI0_BASE)
#define INTC                ((struct INTC_REG *) INTC_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define PMU                 ((struct PMU_REG *) PMU_BASE)
#define I2C2                ((struct I2C_REG *) I2C2_BASE)
#define I2STDM0             ((struct I2STDM_REG *) I2STDM0_BASE)
#define PDM0                ((struct PDM_REG *) PDM0_BASE)
#define CRU                 ((struct CRU_REG *) CRU_BASE)

#define IS_GRF_INSTANCE(instance) ((instance) == GRF)
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || ((instance) == TIMER2) || ((instance) == TIMER3) || ((instance) == TIMER4))
#define IS_MBOX_INSTANCE(instance) ((instance) == MBOX0)
#define IS_SPI_INSTANCE(instance) ((instance) == SPI1)
#define IS_EFUSE_INSTANCE(instance) ((instance) == EFUSE)
#define IS_DMA_INSTANCE(instance) ((instance) == DMA)
#define IS_FSPI_INSTANCE(instance) ((instance) == FSPI0)
#define IS_INTC_INSTANCE(instance) ((instance) == INTC)
#define IS_PMU_INSTANCE(instance) ((instance) == PMU)
#define IS_I2STDM_INSTANCE(instance) ((instance) == I2STDM0)
#define IS_PDM_INSTANCE(instance) ((instance) == PDM0)
#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1))
#define IS_WDT_INSTANCE(instance) (((instance) == WDT0) || ((instance) == WDT1))
#define IS_I2C_INSTANCE(instance) (((instance) == I2C0) || ((instance) == I2C1) || ((instance) == I2C2))
#define IS_PWM_INSTANCE(instance) ((instance) == PWM)
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO1) || ((instance) == GPIO2) || ((instance) == GPIO0))
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/******************************************GRF*******************************************/
/* SOC_CON0 */
#define GRF_SOC_CON0_OFFSET                                (0x0)
#define GRF_SOC_CON0_PMU_RESET_HOLD_NIUP_SHIFT             (0U)
#define GRF_SOC_CON0_PMU_RESET_HOLD_NIUP_MASK              (0x1U << GRF_SOC_CON0_PMU_RESET_HOLD_NIUP_SHIFT)             /* 0x00000001 */
#define GRF_SOC_CON0_PMU_RESET_HOLD_GPIO_SHIFT             (1U)
#define GRF_SOC_CON0_PMU_RESET_HOLD_GPIO_MASK              (0x1U << GRF_SOC_CON0_PMU_RESET_HOLD_GPIO_SHIFT)             /* 0x00000002 */
#define GRF_SOC_CON0_PMU_RESET_HOLD_GRF_SHIFT              (2U)
#define GRF_SOC_CON0_PMU_RESET_HOLD_GRF_MASK               (0x1U << GRF_SOC_CON0_PMU_RESET_HOLD_GRF_SHIFT)              /* 0x00000004 */
#define GRF_SOC_CON0_PD_PMU_RESET_HOLD_I2C_SHIFT           (3U)
#define GRF_SOC_CON0_PD_PMU_RESET_HOLD_I2C_MASK            (0x1U << GRF_SOC_CON0_PD_PMU_RESET_HOLD_I2C_SHIFT)           /* 0x00000008 */
#define GRF_SOC_CON0_REMAP_SHIFT                           (4U)
#define GRF_SOC_CON0_REMAP_MASK                            (0x1U << GRF_SOC_CON0_REMAP_SHIFT)                           /* 0x00000010 */
#define GRF_SOC_CON0_RF_PD_SHIFT                           (8U)
#define GRF_SOC_CON0_RF_PD_MASK                            (0x1U << GRF_SOC_CON0_RF_PD_SHIFT)                           /* 0x00000100 */
#define GRF_SOC_CON0_RF_PD_SEL_SHIFT                       (9U)
#define GRF_SOC_CON0_RF_PD_SEL_MASK                        (0x1U << GRF_SOC_CON0_RF_PD_SEL_SHIFT)                       /* 0x00000200 */
#define GRF_SOC_CON0_REV0_SHIFT                            (10U)
#define GRF_SOC_CON0_REV0_MASK                             (0x1U << GRF_SOC_CON0_REV0_SHIFT)                            /* 0x00000400 */
#define GRF_SOC_CON0_REV1_SHIFT                            (11U)
#define GRF_SOC_CON0_REV1_MASK                             (0x1U << GRF_SOC_CON0_REV1_SHIFT)                            /* 0x00000800 */
#define GRF_SOC_CON0_RF_32K_EN_SHIFT                       (12U)
#define GRF_SOC_CON0_RF_32K_EN_MASK                        (0x1U << GRF_SOC_CON0_RF_32K_EN_SHIFT)                       /* 0x00001000 */
#define GRF_SOC_CON0_RF_32K_SEL_SHIFT                      (13U)
#define GRF_SOC_CON0_RF_32K_SEL_MASK                       (0x1U << GRF_SOC_CON0_RF_32K_SEL_SHIFT)                      /* 0x00002000 */
/* SOC_CON1 */
#define GRF_SOC_CON1_OFFSET                                (0x4)
#define GRF_SOC_CON1_DRTYPE_DMAC_I2S_TX_SHIFT              (0U)
#define GRF_SOC_CON1_DRTYPE_DMAC_I2S_TX_MASK               (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_I2S_TX_SHIFT)              /* 0x00000003 */
#define GRF_SOC_CON1_DRTYPE_DMAC_I2S_RX_SHIFT              (2U)
#define GRF_SOC_CON1_DRTYPE_DMAC_I2S_RX_MASK               (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_I2S_RX_SHIFT)              /* 0x0000000C */
#define GRF_SOC_CON1_DRTYPE_DMAC_UART0_TX_SHIFT            (4U)
#define GRF_SOC_CON1_DRTYPE_DMAC_UART0_TX_MASK             (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_UART0_TX_SHIFT)            /* 0x00000030 */
#define GRF_SOC_CON1_DRTYPE_DMAC_UART0_RX_SHIFT            (6U)
#define GRF_SOC_CON1_DRTYPE_DMAC_UART0_RX_MASK             (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_UART0_RX_SHIFT)            /* 0x000000C0 */
#define GRF_SOC_CON1_DRTYPE_DMAC_UART1_TX_SHIFT            (8U)
#define GRF_SOC_CON1_DRTYPE_DMAC_UART1_TX_MASK             (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_UART1_TX_SHIFT)            /* 0x00000300 */
#define GRF_SOC_CON1_DRTYPE_DMAC_UART1_RX_SHIFT            (10U)
#define GRF_SOC_CON1_DRTYPE_DMAC_UART1_RX_MASK             (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_UART1_RX_SHIFT)            /* 0x00000C00 */
#define GRF_SOC_CON1_DRTYPE_DMAC_SPI_TX_SHIFT              (12U)
#define GRF_SOC_CON1_DRTYPE_DMAC_SPI_TX_MASK               (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_SPI_TX_SHIFT)              /* 0x00003000 */
#define GRF_SOC_CON1_DRTYPE_DMAC_SPI_RX_SHIFT              (14U)
#define GRF_SOC_CON1_DRTYPE_DMAC_SPI_RX_MASK               (0x3U << GRF_SOC_CON1_DRTYPE_DMAC_SPI_RX_SHIFT)              /* 0x0000C000 */
/* SOC_CON2 */
#define GRF_SOC_CON2_OFFSET                                (0x8)
#define GRF_SOC_CON2_DRTYPE_DMAC_PWM_SHIFT                 (0U)
#define GRF_SOC_CON2_DRTYPE_DMAC_PWM_MASK                  (0x3U << GRF_SOC_CON2_DRTYPE_DMAC_PWM_SHIFT)                 /* 0x00000003 */
#define GRF_SOC_CON2_REV0_SHIFT                            (2U)
#define GRF_SOC_CON2_REV0_MASK                             (0x3U << GRF_SOC_CON2_REV0_SHIFT)                            /* 0x0000000C */
#define GRF_SOC_CON2_DRTYPE_DMAC_PDM_SHIFT                 (4U)
#define GRF_SOC_CON2_DRTYPE_DMAC_PDM_MASK                  (0x3U << GRF_SOC_CON2_DRTYPE_DMAC_PDM_SHIFT)                 /* 0x00000030 */
#define GRF_SOC_CON2_GRF_UART0_RTS_SEL_SHIFT               (6U)
#define GRF_SOC_CON2_GRF_UART0_RTS_SEL_MASK                (0x1U << GRF_SOC_CON2_GRF_UART0_RTS_SEL_SHIFT)               /* 0x00000040 */
#define GRF_SOC_CON2_GRF_UART1_RTS_SEL_SHIFT               (7U)
#define GRF_SOC_CON2_GRF_UART1_RTS_SEL_MASK                (0x1U << GRF_SOC_CON2_GRF_UART1_RTS_SEL_SHIFT)               /* 0x00000080 */
#define GRF_SOC_CON2_GRF_UART0_CTS_SEL_SHIFT               (8U)
#define GRF_SOC_CON2_GRF_UART0_CTS_SEL_MASK                (0x1U << GRF_SOC_CON2_GRF_UART0_CTS_SEL_SHIFT)               /* 0x00000100 */
#define GRF_SOC_CON2_GRF_UART1_CTS_SEL_SHIFT               (9U)
#define GRF_SOC_CON2_GRF_UART1_CTS_SEL_MASK                (0x1U << GRF_SOC_CON2_GRF_UART1_CTS_SEL_SHIFT)               /* 0x00000200 */
#define GRF_SOC_CON2_DRTYPE_DMAC_BT_TEST_TX_SHIFT          (10U)
#define GRF_SOC_CON2_DRTYPE_DMAC_BT_TEST_TX_MASK           (0x3U << GRF_SOC_CON2_DRTYPE_DMAC_BT_TEST_TX_SHIFT)          /* 0x00000C00 */
#define GRF_SOC_CON2_DRTYPE_DMAC_BT_TEST_RX_SHIFT          (12U)
#define GRF_SOC_CON2_DRTYPE_DMAC_BT_TEST_RX_MASK           (0x3U << GRF_SOC_CON2_DRTYPE_DMAC_BT_TEST_RX_SHIFT)          /* 0x00003000 */
#define GRF_SOC_CON2_REV1_SHIFT                            (14U)
#define GRF_SOC_CON2_REV1_MASK                             (0x1U << GRF_SOC_CON2_REV1_SHIFT)                            /* 0x00004000 */
#define GRF_SOC_CON2_BBGEN_APB_SEL_SHIFT                   (15U)
#define GRF_SOC_CON2_BBGEN_APB_SEL_MASK                    (0x1U << GRF_SOC_CON2_BBGEN_APB_SEL_SHIFT)                   /* 0x00008000 */
/* SOC_CON3 */
#define GRF_SOC_CON3_OFFSET                                (0xC)
#define GRF_SOC_CON3_DMAC_BOOT_IRQ_NS_SHIFT                (0U)
#define GRF_SOC_CON3_DMAC_BOOT_IRQ_NS_MASK                 (0xFFFU << GRF_SOC_CON3_DMAC_BOOT_IRQ_NS_SHIFT)              /* 0x00000FFF */
#define GRF_SOC_CON3_DMAC_BOOT_FROM_PC_SHIFT               (12U)
#define GRF_SOC_CON3_DMAC_BOOT_FROM_PC_MASK                (0x1U << GRF_SOC_CON3_DMAC_BOOT_FROM_PC_SHIFT)               /* 0x00001000 */
#define GRF_SOC_CON3_DMAC_BOOT_MANAGER_NS_SHIFT            (13U)
#define GRF_SOC_CON3_DMAC_BOOT_MANAGER_NS_MASK             (0x1U << GRF_SOC_CON3_DMAC_BOOT_MANAGER_NS_SHIFT)            /* 0x00002000 */
#define GRF_SOC_CON3_DMA_REQ_MODIFY_DIS_SHIFT              (14U)
#define GRF_SOC_CON3_DMA_REQ_MODIFY_DIS_MASK               (0x1U << GRF_SOC_CON3_DMA_REQ_MODIFY_DIS_SHIFT)              /* 0x00004000 */
/* SOC_CON4 */
#define GRF_SOC_CON4_OFFSET                                (0x10)
#define GRF_SOC_CON4_DMAC_BOOT_PERIPH_NS_SHIFT             (0U)
#define GRF_SOC_CON4_DMAC_BOOT_PERIPH_NS_MASK              (0xFFFU << GRF_SOC_CON4_DMAC_BOOT_PERIPH_NS_SHIFT)           /* 0x00000FFF */
#define GRF_SOC_CON4_DMAC_BOOT_ADDR_SHIFT                  (12U)
#define GRF_SOC_CON4_DMAC_BOOT_ADDR_MASK                   (0xFU << GRF_SOC_CON4_DMAC_BOOT_ADDR_SHIFT)                  /* 0x0000F000 */
/* SOC_CON5 */
#define GRF_SOC_CON5_OFFSET                                (0x14)
#define GRF_SOC_CON5_DMAC_BOOT_ADDR_SHIFT                  (0U)
#define GRF_SOC_CON5_DMAC_BOOT_ADDR_MASK                   (0xFFFFU << GRF_SOC_CON5_DMAC_BOOT_ADDR_SHIFT)               /* 0x0000FFFF */
/* SOC_CON8 */
#define GRF_SOC_CON8_OFFSET                                (0x20)
#define GRF_SOC_CON8_BT_DEBUG_SEL_SHIFT                    (0U)
#define GRF_SOC_CON8_BT_DEBUG_SEL_MASK                     (0x7U << GRF_SOC_CON8_BT_DEBUG_SEL_SHIFT)                    /* 0x00000007 */
#define GRF_SOC_CON8_WAKEUP_REQ_SHIFT                      (6U)
#define GRF_SOC_CON8_WAKEUP_REQ_MASK                       (0x1U << GRF_SOC_CON8_WAKEUP_REQ_SHIFT)                      /* 0x00000040 */
/* GPIOMUX_PLUS */
#define GRF_GPIOMUX_PLUS_OFFSET                            (0x3C)
#define GRF_GPIOMUX_PLUS_GPIO1C0_SEL_2_SHIFT               (0U)
#define GRF_GPIOMUX_PLUS_GPIO1C0_SEL_2_MASK                (0x1U << GRF_GPIOMUX_PLUS_GPIO1C0_SEL_2_SHIFT)               /* 0x00000001 */
#define GRF_GPIOMUX_PLUS_GPIO1C1_SEL_2_SHIFT               (1U)
#define GRF_GPIOMUX_PLUS_GPIO1C1_SEL_2_MASK                (0x1U << GRF_GPIOMUX_PLUS_GPIO1C1_SEL_2_SHIFT)               /* 0x00000002 */
#define GRF_GPIOMUX_PLUS_GPIO1C3_SEL_2_SHIFT               (2U)
#define GRF_GPIOMUX_PLUS_GPIO1C3_SEL_2_MASK                (0x1U << GRF_GPIOMUX_PLUS_GPIO1C3_SEL_2_SHIFT)               /* 0x00000004 */
#define GRF_GPIOMUX_PLUS_GPIO1C5_SEL_2_SHIFT               (3U)
#define GRF_GPIOMUX_PLUS_GPIO1C5_SEL_2_MASK                (0x1U << GRF_GPIOMUX_PLUS_GPIO1C5_SEL_2_SHIFT)               /* 0x00000008 */
#define GRF_GPIOMUX_PLUS_GPIO1C7_SEL_2_SHIFT               (4U)
#define GRF_GPIOMUX_PLUS_GPIO1C7_SEL_2_MASK                (0x1U << GRF_GPIOMUX_PLUS_GPIO1C7_SEL_2_SHIFT)               /* 0x00000010 */
#define GRF_GPIOMUX_PLUS_REV_SHIFT                         (5U)
#define GRF_GPIOMUX_PLUS_REV_MASK                          (0x7U << GRF_GPIOMUX_PLUS_REV_SHIFT)                         /* 0x000000E0 */
/* GPIO0A_IOMUX */
#define GRF_GPIO0A_IOMUX_OFFSET                            (0x40)
#define GRF_GPIO0A_IOMUX_GPIO0A0_SEL_SHIFT                 (0U)
#define GRF_GPIO0A_IOMUX_GPIO0A0_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT                 (2U)
#define GRF_GPIO0A_IOMUX_GPIO0A1_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT                 (4U)
#define GRF_GPIO0A_IOMUX_GPIO0A2_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT                 (6U)
#define GRF_GPIO0A_IOMUX_GPIO0A3_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A3_SEL_SHIFT)                 /* 0x000000C0 */
#define GRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT                 (8U)
#define GRF_GPIO0A_IOMUX_GPIO0A4_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT                 (10U)
#define GRF_GPIO0A_IOMUX_GPIO0A5_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A5_SEL_SHIFT)                 /* 0x00000C00 */
#define GRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT                 (12U)
#define GRF_GPIO0A_IOMUX_GPIO0A6_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT                 (14U)
#define GRF_GPIO0A_IOMUX_GPIO0A7_SEL_MASK                  (0x3U << GRF_GPIO0A_IOMUX_GPIO0A7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO0B_IOMUX */
#define GRF_GPIO0B_IOMUX_OFFSET                            (0x44)
#define GRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT                 (0U)
#define GRF_GPIO0B_IOMUX_GPIO0B0_SEL_MASK                  (0x3U << GRF_GPIO0B_IOMUX_GPIO0B0_SEL_SHIFT)                 /* 0x00000003 */
/* GPIO1A_IOMUX */
#define GRF_GPIO1A_IOMUX_OFFSET                            (0x50)
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
/* GPIO1B_IOMUX */
#define GRF_GPIO1B_IOMUX_OFFSET                            (0x54)
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
#define GRF_GPIO1C_IOMUX_OFFSET                            (0x58)
#define GRF_GPIO1C_IOMUX_GPIO1C0_SEL_10_SHIFT              (0U)
#define GRF_GPIO1C_IOMUX_GPIO1C0_SEL_10_MASK               (0x3U << GRF_GPIO1C_IOMUX_GPIO1C0_SEL_10_SHIFT)              /* 0x00000003 */
#define GRF_GPIO1C_IOMUX_GPIO1C1_SEL_10_SHIFT              (2U)
#define GRF_GPIO1C_IOMUX_GPIO1C1_SEL_10_MASK               (0x3U << GRF_GPIO1C_IOMUX_GPIO1C1_SEL_10_SHIFT)              /* 0x0000000C */
#define GRF_GPIO1C_IOMUX_GPIO1C2_SEL_SHIFT                 (4U)
#define GRF_GPIO1C_IOMUX_GPIO1C2_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO1C_IOMUX_GPIO1C3_SEL_10_SHIFT              (6U)
#define GRF_GPIO1C_IOMUX_GPIO1C3_SEL_10_MASK               (0x3U << GRF_GPIO1C_IOMUX_GPIO1C3_SEL_10_SHIFT)              /* 0x000000C0 */
#define GRF_GPIO1C_IOMUX_GPIO1C4_SEL_SHIFT                 (8U)
#define GRF_GPIO1C_IOMUX_GPIO1C4_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C4_SEL_SHIFT)                 /* 0x00000300 */
#define GRF_GPIO1C_IOMUX_GPIO1C5_SEL_10_SHIFT              (10U)
#define GRF_GPIO1C_IOMUX_GPIO1C5_SEL_10_MASK               (0x3U << GRF_GPIO1C_IOMUX_GPIO1C5_SEL_10_SHIFT)              /* 0x00000C00 */
#define GRF_GPIO1C_IOMUX_GPIO1C6_SEL_SHIFT                 (12U)
#define GRF_GPIO1C_IOMUX_GPIO1C6_SEL_MASK                  (0x3U << GRF_GPIO1C_IOMUX_GPIO1C6_SEL_SHIFT)                 /* 0x00003000 */
#define GRF_GPIO1C_IOMUX_GPIO1C7_SEL_10_SHIFT              (14U)
#define GRF_GPIO1C_IOMUX_GPIO1C7_SEL_10_MASK               (0x3U << GRF_GPIO1C_IOMUX_GPIO1C7_SEL_10_SHIFT)              /* 0x0000C000 */
/* GPIO2A_IOMUX */
#define GRF_GPIO2A_IOMUX_OFFSET                            (0x60)
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
#define GRF_GPIO2B_IOMUX_OFFSET                            (0x64)
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
#define GRF_GPIO2C_IOMUX_OFFSET                            (0x68)
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
#define GRF_GPIO2C_IOMUX_GPIO2C7_SEL_SHIFT                 (14U)
#define GRF_GPIO2C_IOMUX_GPIO2C7_SEL_MASK                  (0x3U << GRF_GPIO2C_IOMUX_GPIO2C7_SEL_SHIFT)                 /* 0x0000C000 */
/* GPIO2D_IOMUX */
#define GRF_GPIO2D_IOMUX_OFFSET                            (0x6C)
#define GRF_GPIO2D_IOMUX_GPIO2D0_SEL_SHIFT                 (0U)
#define GRF_GPIO2D_IOMUX_GPIO2D0_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D0_SEL_SHIFT)                 /* 0x00000003 */
#define GRF_GPIO2D_IOMUX_GPIO2D1_SEL_SHIFT                 (2U)
#define GRF_GPIO2D_IOMUX_GPIO2D1_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D1_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_GPIO2D_IOMUX_GPIO2D2_SEL_SHIFT                 (4U)
#define GRF_GPIO2D_IOMUX_GPIO2D2_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D2_SEL_SHIFT)                 /* 0x00000030 */
#define GRF_GPIO2D_IOMUX_GPIO2D3_SEL_SHIFT                 (6U)
#define GRF_GPIO2D_IOMUX_GPIO2D3_SEL_MASK                  (0x3U << GRF_GPIO2D_IOMUX_GPIO2D3_SEL_SHIFT)                 /* 0x000000C0 */
/* GPIO0A_P */
#define GRF_GPIO0A_P_OFFSET                                (0x70)
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
#define GRF_GPIO0B_P_OFFSET                                (0x74)
#define GRF_GPIO0B_P_GPIO0B0_P_SHIFT                       (0U)
#define GRF_GPIO0B_P_GPIO0B0_P_MASK                        (0x3U << GRF_GPIO0B_P_GPIO0B0_P_SHIFT)                       /* 0x00000003 */
/* GPIO1A_P */
#define GRF_GPIO1A_P_OFFSET                                (0x80)
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
/* GPIO1B_P */
#define GRF_GPIO1B_P_OFFSET                                (0x84)
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
#define GRF_GPIO1C_P_OFFSET                                (0x88)
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
/* GPIO2A_P */
#define GRF_GPIO2A_P_OFFSET                                (0x90)
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
#define GRF_GPIO2B_P_OFFSET                                (0x94)
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
#define GRF_GPIO2C_P_OFFSET                                (0x98)
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
/* GPIO2D_P */
#define GRF_GPIO2D_P_OFFSET                                (0x9C)
#define GRF_GPIO2D_P_GPIO2D0_P_SHIFT                       (0U)
#define GRF_GPIO2D_P_GPIO2D0_P_MASK                        (0x3U << GRF_GPIO2D_P_GPIO2D0_P_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2D_P_GPIO2D1_P_SHIFT                       (2U)
#define GRF_GPIO2D_P_GPIO2D1_P_MASK                        (0x3U << GRF_GPIO2D_P_GPIO2D1_P_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2D_P_GPIO2D2_P_SHIFT                       (4U)
#define GRF_GPIO2D_P_GPIO2D2_P_MASK                        (0x3U << GRF_GPIO2D_P_GPIO2D2_P_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2D_P_GPIO2D3_P_SHIFT                       (6U)
#define GRF_GPIO2D_P_GPIO2D3_P_MASK                        (0x3U << GRF_GPIO2D_P_GPIO2D3_P_SHIFT)                       /* 0x000000C0 */
/* GPIO0A_E */
#define GRF_GPIO0A_E_OFFSET                                (0xA0)
#define GRF_GPIO0A_E_GPIO0A0_E_SHIFT                       (0U)
#define GRF_GPIO0A_E_GPIO0A0_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO0A_E_GPIO0A1_E_SHIFT                       (2U)
#define GRF_GPIO0A_E_GPIO0A1_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO0A_E_GPIO0A2_E_SHIFT                       (4U)
#define GRF_GPIO0A_E_GPIO0A2_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO0A_E_GPIO0A3_E_SHIFT                       (6U)
#define GRF_GPIO0A_E_GPIO0A3_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A3_E_SHIFT)                       /* 0x000000C0 */
#define GRF_GPIO0A_E_GPIO0A4_E_SHIFT                       (8U)
#define GRF_GPIO0A_E_GPIO0A4_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A4_E_SHIFT)                       /* 0x00000300 */
#define GRF_GPIO0A_E_GPIO0A5_E_SHIFT                       (10U)
#define GRF_GPIO0A_E_GPIO0A5_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A5_E_SHIFT)                       /* 0x00000C00 */
#define GRF_GPIO0A_E_GPIO0A6_E_SHIFT                       (12U)
#define GRF_GPIO0A_E_GPIO0A6_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A6_E_SHIFT)                       /* 0x00003000 */
#define GRF_GPIO0A_E_GPIO0A7_E_SHIFT                       (14U)
#define GRF_GPIO0A_E_GPIO0A7_E_MASK                        (0x3U << GRF_GPIO0A_E_GPIO0A7_E_SHIFT)                       /* 0x0000C000 */
/* GPIO0B_E */
#define GRF_GPIO0B_E_OFFSET                                (0xA4)
#define GRF_GPIO0B_E_GPIO0B0_E_SHIFT                       (0U)
#define GRF_GPIO0B_E_GPIO0B0_E_MASK                        (0x3U << GRF_GPIO0B_E_GPIO0B0_E_SHIFT)                       /* 0x00000003 */
/* GPIO1A_E */
#define GRF_GPIO1A_E_OFFSET                                (0xB0)
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
/* GPIO1B_E */
#define GRF_GPIO1B_E_OFFSET                                (0xB4)
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
#define GRF_GPIO1C_E_OFFSET                                (0xB8)
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
/* GPIO2A_E */
#define GRF_GPIO2A_E_OFFSET                                (0xC0)
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
#define GRF_GPIO2B_E_OFFSET                                (0xC4)
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
#define GRF_GPIO2C_E_OFFSET                                (0xC8)
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
/* GPIO2D_E */
#define GRF_GPIO2D_E_OFFSET                                (0xCC)
#define GRF_GPIO2D_E_GPIO2D0_E_SHIFT                       (0U)
#define GRF_GPIO2D_E_GPIO2D0_E_MASK                        (0x3U << GRF_GPIO2D_E_GPIO2D0_E_SHIFT)                       /* 0x00000003 */
#define GRF_GPIO2D_E_GPIO2D1_E_SHIFT                       (2U)
#define GRF_GPIO2D_E_GPIO2D1_E_MASK                        (0x3U << GRF_GPIO2D_E_GPIO2D1_E_SHIFT)                       /* 0x0000000C */
#define GRF_GPIO2D_E_GPIO2D2_E_SHIFT                       (4U)
#define GRF_GPIO2D_E_GPIO2D2_E_MASK                        (0x3U << GRF_GPIO2D_E_GPIO2D2_E_SHIFT)                       /* 0x00000030 */
#define GRF_GPIO2D_E_GPIO2D3_E_SHIFT                       (6U)
#define GRF_GPIO2D_E_GPIO2D3_E_MASK                        (0x3U << GRF_GPIO2D_E_GPIO2D3_E_SHIFT)                       /* 0x000000C0 */
/* GPIO0A_S */
#define GRF_GPIO0A_S_OFFSET                                (0xD0)
#define GRF_GPIO0A_S_GPIO0A_SLW_SHIFT                      (0U)
#define GRF_GPIO0A_S_GPIO0A_SLW_MASK                       (0xFFU << GRF_GPIO0A_S_GPIO0A_SLW_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO0A_S_GPIO0A_SMT_SHIFT                      (8U)
#define GRF_GPIO0A_S_GPIO0A_SMT_MASK                       (0xFFU << GRF_GPIO0A_S_GPIO0A_SMT_SHIFT)                     /* 0x0000FF00 */
/* GPIO0B_S */
#define GRF_GPIO0B_S_OFFSET                                (0xD4)
#define GRF_GPIO0B_S_GPIO0B_SLW_SHIFT                      (0U)
#define GRF_GPIO0B_S_GPIO0B_SLW_MASK                       (0x3U << GRF_GPIO0B_S_GPIO0B_SLW_SHIFT)                      /* 0x00000003 */
#define GRF_GPIO0B_S_GPIO0B_SMT_SHIFT                      (2U)
#define GRF_GPIO0B_S_GPIO0B_SMT_MASK                       (0x3U << GRF_GPIO0B_S_GPIO0B_SMT_SHIFT)                      /* 0x0000000C */
/* GPIO1A_S */
#define GRF_GPIO1A_S_OFFSET                                (0xE0)
#define GRF_GPIO1A_S_GPIO1A_SLW_SHIFT                      (0U)
#define GRF_GPIO1A_S_GPIO1A_SLW_MASK                       (0x1FU << GRF_GPIO1A_S_GPIO1A_SLW_SHIFT)                     /* 0x0000001F */
#define GRF_GPIO1A_S_GPIO1A_SMT_SHIFT                      (5U)
#define GRF_GPIO1A_S_GPIO1A_SMT_MASK                       (0x1FU << GRF_GPIO1A_S_GPIO1A_SMT_SHIFT)                     /* 0x000003E0 */
/* GPIO1B_S */
#define GRF_GPIO1B_S_OFFSET                                (0xE4)
#define GRF_GPIO1B_S_GPIO1B_SLW_SHIFT                      (0U)
#define GRF_GPIO1B_S_GPIO1B_SLW_MASK                       (0xFFU << GRF_GPIO1B_S_GPIO1B_SLW_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO1B_S_GPIO1B_SMT_SHIFT                      (8U)
#define GRF_GPIO1B_S_GPIO1B_SMT_MASK                       (0xFFU << GRF_GPIO1B_S_GPIO1B_SMT_SHIFT)                     /* 0x0000FF00 */
/* GPIO1C_S */
#define GRF_GPIO1C_S_OFFSET                                (0xE8)
#define GRF_GPIO1C_S_GPIO1C_SLW_SHIFT                      (0U)
#define GRF_GPIO1C_S_GPIO1C_SLW_MASK                       (0xFFU << GRF_GPIO1C_S_GPIO1C_SLW_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO1C_S_GPIO1C_SMT_SHIFT                      (8U)
#define GRF_GPIO1C_S_GPIO1C_SMT_MASK                       (0xFFU << GRF_GPIO1C_S_GPIO1C_SMT_SHIFT)                     /* 0x0000FF00 */
/* GPIO2A_S */
#define GRF_GPIO2A_S_OFFSET                                (0xF0)
#define GRF_GPIO2A_S_GPIO2A_SLW_SHIFT                      (0U)
#define GRF_GPIO2A_S_GPIO2A_SLW_MASK                       (0xFFU << GRF_GPIO2A_S_GPIO2A_SLW_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO2A_S_GPIO2A_SMT_SHIFT                      (8U)
#define GRF_GPIO2A_S_GPIO2A_SMT_MASK                       (0xFFU << GRF_GPIO2A_S_GPIO2A_SMT_SHIFT)                     /* 0x0000FF00 */
/* GPIO2B_S */
#define GRF_GPIO2B_S_OFFSET                                (0xF4)
#define GRF_GPIO2B_S_GPIO2B_SLW_SHIFT                      (0U)
#define GRF_GPIO2B_S_GPIO2B_SLW_MASK                       (0xFFU << GRF_GPIO2B_S_GPIO2B_SLW_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO2B_S_GPIO2B_SMT_SHIFT                      (8U)
#define GRF_GPIO2B_S_GPIO2B_SMT_MASK                       (0xFFU << GRF_GPIO2B_S_GPIO2B_SMT_SHIFT)                     /* 0x0000FF00 */
/* GPIO2C_S */
#define GRF_GPIO2C_S_OFFSET                                (0xF8)
#define GRF_GPIO2C_S_GPIO2C_SMT_SHIFT                      (0U)
#define GRF_GPIO2C_S_GPIO2C_SMT_MASK                       (0xFFU << GRF_GPIO2C_S_GPIO2C_SMT_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO2C_S_GPIO2C_SLW_SHIFT                      (8U)
#define GRF_GPIO2C_S_GPIO2C_SLW_MASK                       (0xFFU << GRF_GPIO2C_S_GPIO2C_SLW_SHIFT)                     /* 0x0000FF00 */
/* GPIO2D_S */
#define GRF_GPIO2D_S_OFFSET                                (0xFC)
#define GRF_GPIO2D_S_GPIO2D_SMT_SHIFT                      (0U)
#define GRF_GPIO2D_S_GPIO2D_SMT_MASK                       (0xFFU << GRF_GPIO2D_S_GPIO2D_SMT_SHIFT)                     /* 0x000000FF */
#define GRF_GPIO2D_S_GPIO2D_SLW_SHIFT                      (8U)
#define GRF_GPIO2D_S_GPIO2D_SLW_MASK                       (0xFFU << GRF_GPIO2D_S_GPIO2D_SLW_SHIFT)                     /* 0x0000FF00 */
/* MCU_STCALIB */
#define GRF_MCU_STCALIB_OFFSET                             (0x100)
#define GRF_MCU_STCALIB_MCU_STCALIB_SHIFT                  (0U)
#define GRF_MCU_STCALIB_MCU_STCALIB_MASK                   (0x3FFFFFFU << GRF_MCU_STCALIB_MCU_STCALIB_SHIFT)            /* 0x03FFFFFF */
/* MCU_CON */
#define GRF_MCU_CON_OFFSET                                 (0x104)
#define GRF_MCU_CON_NMI_SHIFT                              (0U)
#define GRF_MCU_CON_NMI_MASK                               (0x1U << GRF_MCU_CON_NMI_SHIFT)                              /* 0x00000001 */
/* SOC_STATUS0 */
#define GRF_SOC_STATUS0_OFFSET                             (0x140)
#define GRF_SOC_STATUS0_BT_CLK_STATUS_SHIFT                (0U)
#define GRF_SOC_STATUS0_BT_CLK_STATUS_MASK                 (0x1U << GRF_SOC_STATUS0_BT_CLK_STATUS_SHIFT)                /* 0x00000001 */
#define GRF_SOC_STATUS0_STATUS_MDM_RXPWR_SHIFT             (1U)
#define GRF_SOC_STATUS0_STATUS_MDM_RXPWR_MASK              (0x3FU << GRF_SOC_STATUS0_STATUS_MDM_RXPWR_SHIFT)            /* 0x0000007E */
/* MCU_STATUS */
#define GRF_MCU_STATUS_OFFSET                              (0x144)
#define GRF_MCU_STATUS_MCU_HALTED_SHIFT                    (0U)
#define GRF_MCU_STATUS_MCU_HALTED_MASK                     (0x1U << GRF_MCU_STATUS_MCU_HALTED_SHIFT)                    /* 0x00000001 */
#define GRF_MCU_STATUS_MCU_LOCKUP_SHIFT                    (1U)
#define GRF_MCU_STATUS_MCU_LOCKUP_MASK                     (0x1U << GRF_MCU_STATUS_MCU_LOCKUP_SHIFT)                    /* 0x00000002 */
#define GRF_MCU_STATUS_MCU_DEEPSLEEPING_SHIFT              (2U)
#define GRF_MCU_STATUS_MCU_DEEPSLEEPING_MASK               (0x1U << GRF_MCU_STATUS_MCU_DEEPSLEEPING_SHIFT)              /* 0x00000004 */
#define GRF_MCU_STATUS_MCU_SLEEPING_SHIFT                  (3U)
#define GRF_MCU_STATUS_MCU_SLEEPING_MASK                   (0x1U << GRF_MCU_STATUS_MCU_SLEEPING_SHIFT)                  /* 0x00000008 */
/* BT_DIG */
#define GRF_BT_DIG_OFFSET                                  (0x148)
#define GRF_BT_DIG_BT_DIG_SHIFT                            (0U)
#define GRF_BT_DIG_BT_DIG_MASK                             (0xFFFFFFFFU << GRF_BT_DIG_BT_DIG_SHIFT)                     /* 0xFFFFFFFF */
/* BLE_DIG */
#define GRF_BLE_DIG_OFFSET                                 (0x14C)
#define GRF_BLE_DIG_BLE_DIG_SHIFT                          (0U)
#define GRF_BLE_DIG_BLE_DIG_MASK                           (0xFFFFFFFFU << GRF_BLE_DIG_BLE_DIG_SHIFT)                   /* 0xFFFFFFFF */
/* BTDM_DIG */
#define GRF_BTDM_DIG_OFFSET                                (0x150)
#define GRF_BTDM_DIG_BTDM_DIG_SHIFT                        (0U)
#define GRF_BTDM_DIG_BTDM_DIG_MASK                         (0xFFFFFFFFU << GRF_BTDM_DIG_BTDM_DIG_SHIFT)                 /* 0xFFFFFFFF */
/* DSP_CON0 */
#define GRF_DSP_CON0_OFFSET                                (0x180)
#define GRF_DSP_CON0_DSPGRF_OCDHALTONRESET_SHIFT           (0U)
#define GRF_DSP_CON0_DSPGRF_OCDHALTONRESET_MASK            (0x1U << GRF_DSP_CON0_DSPGRF_OCDHALTONRESET_SHIFT)           /* 0x00000001 */
#define GRF_DSP_CON0_DSPGRF_BREAKIN_SHIFT                  (1U)
#define GRF_DSP_CON0_DSPGRF_BREAKIN_MASK                   (0x1U << GRF_DSP_CON0_DSPGRF_BREAKIN_SHIFT)                  /* 0x00000002 */
#define GRF_DSP_CON0_DSPGRF_BREAKOUTACK_SHIFT              (2U)
#define GRF_DSP_CON0_DSPGRF_BREAKOUTACK_MASK               (0x1U << GRF_DSP_CON0_DSPGRF_BREAKOUTACK_SHIFT)              /* 0x00000004 */
#define GRF_DSP_CON0_REV_SHIFT                             (3U)
#define GRF_DSP_CON0_REV_MASK                              (0x1U << GRF_DSP_CON0_REV_SHIFT)                             /* 0x00000008 */
#define GRF_DSP_CON0_STATVECTORSEL_SHIFT                   (4U)
#define GRF_DSP_CON0_STATVECTORSEL_MASK                    (0x1U << GRF_DSP_CON0_STATVECTORSEL_SHIFT)                   /* 0x00000010 */
#define GRF_DSP_CON0_RUNSTALL_SHIFT                        (5U)
#define GRF_DSP_CON0_RUNSTALL_MASK                         (0x1U << GRF_DSP_CON0_RUNSTALL_SHIFT)                        /* 0x00000020 */
#define GRF_DSP_CON0_WITRE_ENABLE_SHIFT                    (16U)
#define GRF_DSP_CON0_WITRE_ENABLE_MASK                     (0x3FU << GRF_DSP_CON0_WITRE_ENABLE_SHIFT)                   /* 0x003F0000 */
/* DSP_CON1 */
#define GRF_DSP_CON1_OFFSET                                (0x184)
#define GRF_DSP_CON1_DSPGRF_ALTRESETVEC_SHIFT              (0U)
#define GRF_DSP_CON1_DSPGRF_ALTRESETVEC_MASK               (0xFFFFFFFFU << GRF_DSP_CON1_DSPGRF_ALTRESETVEC_SHIFT)       /* 0xFFFFFFFF */
/* DSP_CON2 */
#define GRF_DSP_CON2_OFFSET                                (0x188)
#define GRF_DSP_CON2_DSPGRF_MEMAUTOGATINGEN_SHIFT          (0U)
#define GRF_DSP_CON2_DSPGRF_MEMAUTOGATINGEN_MASK           (0x7FU << GRF_DSP_CON2_DSPGRF_MEMAUTOGATINGEN_SHIFT)         /* 0x0000007F */
#define GRF_DSP_CON2_ICACHE_MEM_AUTO_GATING_DISABLE_SHIFT  (0U)
#define GRF_DSP_CON2_ICACHE_MEM_AUTO_GATING_DISABLE_MASK   (0x1U << GRF_DSP_CON2_ICACHE_MEM_AUTO_GATING_DISABLE_SHIFT)       /* 0x00000001 */
#define GRF_DSP_CON2_ITAG_MEM_AUTO_GATING_DISABLE_SHIFT    (1U)
#define GRF_DSP_CON2_ITAG_MEM_AUTO_GATING_DISABLE_MASK     (0x1U << GRF_DSP_CON2_ITAG_MEM_AUTO_GATING_DISABLE_SHIFT)         /* 0x00000002 */
#define GRF_DSP_CON2_DCACHE_MEM_AUTO_GATING_DISABLE_SHIFT  (2U)
#define GRF_DSP_CON2_DCACHE_MEM_AUTO_GATING_DISABLE_MASK   (0x1U << GRF_DSP_CON2_DCACHE_MEM_AUTO_GATING_DISABLE_SHIFT)       /* 0x00000004 */
#define GRF_DSP_CON2_DTAG_MEM_AUTO_GATING_DISABLE_SHIFT    (3U)
#define GRF_DSP_CON2_DTAG_MEM_AUTO_GATING_DISABLE_MASK     (0x1U << GRF_DSP_CON2_DTAG_MEM_AUTO_GATING_DISABLE_SHIFT)         /* 0x00000008 */
#define GRF_DSP_CON2_PREFETCH_RAM_AUTO_GATING_DISABLE_SHIFT (4U)
#define GRF_DSP_CON2_PREFETCH_RAM_AUTO_GATING_DISABLE_MASK (0x1U << GRF_DSP_CON2_PREFETCH_RAM_AUTO_GATING_DISABLE_SHIFT)     /* 0x00000010 */
#define GRF_DSP_CON2_DTCM_MEM_AUTO_GATING_DISABLE_SHIFT    (5U)
#define GRF_DSP_CON2_DTCM_MEM_AUTO_GATING_DISABLE_MASK     (0x1U << GRF_DSP_CON2_DTCM_MEM_AUTO_GATING_DISABLE_SHIFT)         /* 0x00000020 */
#define GRF_DSP_CON2_ITCM_MEM_AUTO_GATING_DISABLE_SHIFT    (6U)
#define GRF_DSP_CON2_ITCM_MEM_AUTO_GATING_DISABLE_MASK     (0x1U << GRF_DSP_CON2_ITCM_MEM_AUTO_GATING_DISABLE_SHIFT)         /* 0x00000040 */

/* DSP_STATUS0 */
#define GRF_DSP_STATUS0_OFFSET                             (0x190)
#define GRF_DSP_STATUS0_DSPGRF_XOCDNODE_SHIFT              (0U)
#define GRF_DSP_STATUS0_DSPGRF_XOCDNODE_MASK               (0x1U << GRF_DSP_STATUS0_DSPGRF_XOCDNODE_SHIFT)              /* 0x00000001 */
#define GRF_DSP_STATUS0_DSPGRF_DEBUGMODE_SHIFT             (1U)
#define GRF_DSP_STATUS0_DSPGRF_DEBUGMODE_MASK              (0x1U << GRF_DSP_STATUS0_DSPGRF_DEBUGMODE_SHIFT)             /* 0x00000002 */
#define GRF_DSP_STATUS0_DSPGRF_BREAKINACK_SHIFT            (2U)
#define GRF_DSP_STATUS0_DSPGRF_BREAKINACK_MASK             (0x1U << GRF_DSP_STATUS0_DSPGRF_BREAKINACK_SHIFT)            /* 0x00000004 */
#define GRF_DSP_STATUS0_DSPGRF_BREAKOUT_SHIFT              (3U)
#define GRF_DSP_STATUS0_DSPGRF_BREAKOUT_MASK               (0x1U << GRF_DSP_STATUS0_DSPGRF_BREAKOUT_SHIFT)              /* 0x00000008 */
#define GRF_DSP_STATUS0_DSPGRF_DOUBLEEXCEPTIONERROR_SHIFT  (4U)
#define GRF_DSP_STATUS0_DSPGRF_DOUBLEEXCEPTIONERROR_MASK   (0x1U << GRF_DSP_STATUS0_DSPGRF_DOUBLEEXCEPTIONERROR_SHIFT)  /* 0x00000010 */
#define GRF_DSP_STATUS0_DSPGRF_PFATALERROR_SHIFT           (5U)
#define GRF_DSP_STATUS0_DSPGRF_PFATALERROR_MASK            (0x1U << GRF_DSP_STATUS0_DSPGRF_PFATALERROR_SHIFT)           /* 0x00000020 */
#define GRF_DSP_STATUS0_DSPGRF_PFAULTINFOVALID_SHIFT       (6U)
#define GRF_DSP_STATUS0_DSPGRF_PFAULTINFOVALID_MASK        (0x1U << GRF_DSP_STATUS0_DSPGRF_PFAULTINFOVALID_SHIFT)       /* 0x00000040 */
#define GRF_DSP_STATUS0_DSPGRF_PWAITMODE_SHIFT             (7U)
#define GRF_DSP_STATUS0_DSPGRF_PWAITMODE_MASK              (0x1U << GRF_DSP_STATUS0_DSPGRF_PWAITMODE_SHIFT)             /* 0x00000080 */
#define GRF_DSP_STATUS0_DSPGRF_IRAM0LOADSTORE_SHIFT        (8U)
#define GRF_DSP_STATUS0_DSPGRF_IRAM0LOADSTORE_MASK         (0x1U << GRF_DSP_STATUS0_DSPGRF_IRAM0LOADSTORE_SHIFT)        /* 0x00000100 */
/* DSP_STATUS1 */
#define GRF_DSP_STATUS1_OFFSET                             (0x194)
#define GRF_DSP_STATUS1_DSPGRF_PFAULTINFO_SHIFT            (0U)
#define GRF_DSP_STATUS1_DSPGRF_PFAULTINFO_MASK             (0xFFFFFFFFU << GRF_DSP_STATUS1_DSPGRF_PFAULTINFO_SHIFT)     /* 0xFFFFFFFF */
/* MEM_CON0 */
#define GRF_MEM_CON0_OFFSET                                (0x1A0)
#define GRF_MEM_CON0_ROM_MA_SAWL0_SHIFT                    (0U)
#define GRF_MEM_CON0_ROM_MA_SAWL0_MASK                     (0x1U << GRF_MEM_CON0_ROM_MA_SAWL0_SHIFT)                    /* 0x00000001 */
#define GRF_MEM_CON0_ROM_MA_SAWL1_SHIFT                    (1U)
#define GRF_MEM_CON0_ROM_MA_SAWL1_MASK                     (0x1U << GRF_MEM_CON0_ROM_MA_SAWL1_SHIFT)                    /* 0x00000002 */
#define GRF_MEM_CON0_ROM_MA_WL_SHIFT                       (2U)
#define GRF_MEM_CON0_ROM_MA_WL_MASK                        (0x1U << GRF_MEM_CON0_ROM_MA_WL_SHIFT)                       /* 0x00000004 */
#define GRF_MEM_CON0_SRAM_DEEPSLEEP_SHIFT                  (3U)
#define GRF_MEM_CON0_SRAM_DEEPSLEEP_MASK                   (0x1U << GRF_MEM_CON0_SRAM_DEEPSLEEP_SHIFT)                  /* 0x00000008 */
#define GRF_MEM_CON0_SRAM_POWERGATING_SHIFT                (4U)
#define GRF_MEM_CON0_SRAM_POWERGATING_MASK                 (0x1U << GRF_MEM_CON0_SRAM_POWERGATING_SHIFT)                /* 0x00000010 */
#define GRF_MEM_CON0_SRAM_MA_SAWL0_SHIFT                   (5U)
#define GRF_MEM_CON0_SRAM_MA_SAWL0_MASK                    (0x1U << GRF_MEM_CON0_SRAM_MA_SAWL0_SHIFT)                   /* 0x00000020 */
#define GRF_MEM_CON0_SRAM_MA_SAWL1_SHIFT                   (6U)
#define GRF_MEM_CON0_SRAM_MA_SAWL1_MASK                    (0x1U << GRF_MEM_CON0_SRAM_MA_SAWL1_SHIFT)                   /* 0x00000040 */
#define GRF_MEM_CON0_SRAM_MA_WL0_SHIFT                     (7U)
#define GRF_MEM_CON0_SRAM_MA_WL0_MASK                      (0x1U << GRF_MEM_CON0_SRAM_MA_WL0_SHIFT)                     /* 0x00000080 */
#define GRF_MEM_CON0_SRAM_MA_WL1_SHIFT                     (8U)
#define GRF_MEM_CON0_SRAM_MA_WL1_MASK                      (0x1U << GRF_MEM_CON0_SRAM_MA_WL1_SHIFT)                     /* 0x00000100 */
#define GRF_MEM_CON0_SRAM_MA_WRAS0_SHIFT                   (9U)
#define GRF_MEM_CON0_SRAM_MA_WRAS0_MASK                    (0x1U << GRF_MEM_CON0_SRAM_MA_WRAS0_SHIFT)                   /* 0x00000200 */
#define GRF_MEM_CON0_SRAM_MA_WRAS1_SHIFT                   (10U)
#define GRF_MEM_CON0_SRAM_MA_WRAS1_MASK                    (0x1U << GRF_MEM_CON0_SRAM_MA_WRAS1_SHIFT)                   /* 0x00000400 */
#define GRF_MEM_CON0_SRAM_MA_WRASD_SHIFT                   (11U)
#define GRF_MEM_CON0_SRAM_MA_WRASD_MASK                    (0x1U << GRF_MEM_CON0_SRAM_MA_WRASD_SHIFT)                   /* 0x00000800 */
#define GRF_MEM_CON0_DSP_MEM_EMA_SHIFT                     (12U)
#define GRF_MEM_CON0_DSP_MEM_EMA_MASK                      (0x7U << GRF_MEM_CON0_DSP_MEM_EMA_SHIFT)                     /* 0x00007000 */
#define GRF_MEM_CON0_DSPGRF_HIFIMEM_DEEPSLEEP_SHIFT        (15U)
#define GRF_MEM_CON0_DSPGRF_HIFIMEM_DEEPSLEEP_MASK         (0x1U << GRF_MEM_CON0_DSPGRF_HIFIMEM_DEEPSLEEP_SHIFT)        /* 0x00008000 */
/* MEM_CON1 */
#define GRF_MEM_CON1_OFFSET                                (0x1A4)
#define GRF_MEM_CON1_DSP_MEM_EMAW_SHIFT                    (0U)
#define GRF_MEM_CON1_DSP_MEM_EMAW_MASK                     (0x3U << GRF_MEM_CON1_DSP_MEM_EMAW_SHIFT)                    /* 0x00000003 */
#define GRF_MEM_CON1_DSPGRF_HIFIMEM_POWERGATE_SHIFT        (2U)
#define GRF_MEM_CON1_DSPGRF_HIFIMEM_POWERGATE_MASK         (0x1U << GRF_MEM_CON1_DSPGRF_HIFIMEM_POWERGATE_SHIFT)        /* 0x00000004 */
#define GRF_MEM_CON1_ASP_DEEPSLEEP_SHIFT                   (3U)
#define GRF_MEM_CON1_ASP_DEEPSLEEP_MASK                    (0x1U << GRF_MEM_CON1_ASP_DEEPSLEEP_SHIFT)                   /* 0x00000008 */
#define GRF_MEM_CON1_ASP_POWERGATE_SHIFT                   (4U)
#define GRF_MEM_CON1_ASP_POWERGATE_MASK                    (0x1U << GRF_MEM_CON1_ASP_POWERGATE_SHIFT)                   /* 0x00000010 */
#define GRF_MEM_CON1_ASP_MA_SAWL_SHIFT                     (5U)
#define GRF_MEM_CON1_ASP_MA_SAWL_MASK                      (0x1U << GRF_MEM_CON1_ASP_MA_SAWL_SHIFT)                     /* 0x00000020 */
#define GRF_MEM_CON1_ASP_MA_WL_SHIFT                       (6U)
#define GRF_MEM_CON1_ASP_MA_WL_MASK                        (0x1U << GRF_MEM_CON1_ASP_MA_WL_SHIFT)                       /* 0x00000040 */
#define GRF_MEM_CON1_ASP_MA_WRAS_SHIFT                     (7U)
#define GRF_MEM_CON1_ASP_MA_WRAS_MASK                      (0x1U << GRF_MEM_CON1_ASP_MA_WRAS_SHIFT)                     /* 0x00000080 */
#define GRF_MEM_CON1_ASP_MA_WRASD_SHIFT                    (8U)
#define GRF_MEM_CON1_ASP_MA_WRASD_MASK                     (0x1U << GRF_MEM_CON1_ASP_MA_WRASD_SHIFT)                    /* 0x00000100 */
#define GRF_MEM_CON1_BT_MA_SAWL0_SHIFT                     (9U)
#define GRF_MEM_CON1_BT_MA_SAWL0_MASK                      (0x1U << GRF_MEM_CON1_BT_MA_SAWL0_SHIFT)                     /* 0x00000200 */
#define GRF_MEM_CON1_BT_MA_SAWL1_SHIFT                     (10U)
#define GRF_MEM_CON1_BT_MA_SAWL1_MASK                      (0x1U << GRF_MEM_CON1_BT_MA_SAWL1_SHIFT)                     /* 0x00000400 */
#define GRF_MEM_CON1_BT_MA_WL0_SHIFT                       (11U)
#define GRF_MEM_CON1_BT_MA_WL0_MASK                        (0x1U << GRF_MEM_CON1_BT_MA_WL0_SHIFT)                       /* 0x00000800 */
#define GRF_MEM_CON1_BT_MA_WL1_SHIFT                       (12U)
#define GRF_MEM_CON1_BT_MA_WL1_MASK                        (0x1U << GRF_MEM_CON1_BT_MA_WL1_SHIFT)                       /* 0x00001000 */
#define GRF_MEM_CON1_BT_MA_WRAS0_SHIFT                     (13U)
#define GRF_MEM_CON1_BT_MA_WRAS0_MASK                      (0x1U << GRF_MEM_CON1_BT_MA_WRAS0_SHIFT)                     /* 0x00002000 */
#define GRF_MEM_CON1_BT_MA_WRAS1_SHIFT                     (14U)
#define GRF_MEM_CON1_BT_MA_WRAS1_MASK                      (0x1U << GRF_MEM_CON1_BT_MA_WRAS1_SHIFT)                     /* 0x00004000 */
#define GRF_MEM_CON1_BT_MA_WRASD_SHIFT                     (15U)
#define GRF_MEM_CON1_BT_MA_WRASD_MASK                      (0x1U << GRF_MEM_CON1_BT_MA_WRASD_SHIFT)                     /* 0x00008000 */
/* MEM_CON2 */
#define GRF_MEM_CON2_OFFSET                                (0x1A8)
#define GRF_MEM_CON2_ROM_POWERGATE_SHIFT                   (0U)
#define GRF_MEM_CON2_ROM_POWERGATE_MASK                    (0x1U << GRF_MEM_CON2_ROM_POWERGATE_SHIFT)                   /* 0x00000001 */
#define GRF_MEM_CON2_BT_DEEPSLEEP_SHIFT                    (1U)
#define GRF_MEM_CON2_BT_DEEPSLEEP_MASK                     (0x1U << GRF_MEM_CON2_BT_DEEPSLEEP_SHIFT)                    /* 0x00000002 */
#define GRF_MEM_CON2_BT_POWERGATE_SHIFT                    (2U)
#define GRF_MEM_CON2_BT_POWERGATE_MASK                     (0x1U << GRF_MEM_CON2_BT_POWERGATE_SHIFT)                    /* 0x00000004 */
#define GRF_MEM_CON2_PMUMEM_DEEPSLEEP_SHIFT                (3U)
#define GRF_MEM_CON2_PMUMEM_DEEPSLEEP_MASK                 (0x1U << GRF_MEM_CON2_PMUMEM_DEEPSLEEP_SHIFT)                /* 0x00000008 */
#define GRF_MEM_CON2_PMUMEM_POWERGATE_SHIFT                (4U)
#define GRF_MEM_CON2_PMUMEM_POWERGATE_MASK                 (0x1U << GRF_MEM_CON2_PMUMEM_POWERGATE_SHIFT)                /* 0x00000010 */
#define GRF_MEM_CON2_PMUMEM_MA_SAWL0_SHIFT                 (5U)
#define GRF_MEM_CON2_PMUMEM_MA_SAWL0_MASK                  (0x1U << GRF_MEM_CON2_PMUMEM_MA_SAWL0_SHIFT)                 /* 0x00000020 */
#define GRF_MEM_CON2_PMUMEM_MA_SAWL1_SHIFT                 (6U)
#define GRF_MEM_CON2_PMUMEM_MA_SAWL1_MASK                  (0x1U << GRF_MEM_CON2_PMUMEM_MA_SAWL1_SHIFT)                 /* 0x00000040 */
#define GRF_MEM_CON2_PMUMEM_MA_WL0_SHIFT                   (7U)
#define GRF_MEM_CON2_PMUMEM_MA_WL0_MASK                    (0x1U << GRF_MEM_CON2_PMUMEM_MA_WL0_SHIFT)                   /* 0x00000080 */
#define GRF_MEM_CON2_PMUMEM_WL1_SHIFT                      (8U)
#define GRF_MEM_CON2_PMUMEM_WL1_MASK                       (0x1U << GRF_MEM_CON2_PMUMEM_WL1_SHIFT)                      /* 0x00000100 */
#define GRF_MEM_CON2_PMUMEM_MA_WRAS0_SHIFT                 (9U)
#define GRF_MEM_CON2_PMUMEM_MA_WRAS0_MASK                  (0x1U << GRF_MEM_CON2_PMUMEM_MA_WRAS0_SHIFT)                 /* 0x00000200 */
#define GRF_MEM_CON2_PMUMEM_MA_WRAS1_SHIFT                 (10U)
#define GRF_MEM_CON2_PMUMEM_MA_WRAS1_MASK                  (0x1U << GRF_MEM_CON2_PMUMEM_MA_WRAS1_SHIFT)                 /* 0x00000400 */
#define GRF_MEM_CON2_PMUMEM_MA_WRASD_SHIFT                 (11U)
#define GRF_MEM_CON2_PMUMEM_MA_WRASD_MASK                  (0x1U << GRF_MEM_CON2_PMUMEM_MA_WRASD_SHIFT)                 /* 0x00000800 */
/* OS_REG0 */
#define GRF_OS_REG0_OFFSET                                 (0x200)
#define GRF_OS_REG0_OS_REG0_SHIFT                          (0U)
#define GRF_OS_REG0_OS_REG0_MASK                           (0xFFFFFFFFU << GRF_OS_REG0_OS_REG0_SHIFT)                   /* 0xFFFFFFFF */
/* OS_REG1 */
#define GRF_OS_REG1_OFFSET                                 (0x204)
#define GRF_OS_REG1_OS_REG1_SHIFT                          (0U)
#define GRF_OS_REG1_OS_REG1_MASK                           (0xFFFFFFFFU << GRF_OS_REG1_OS_REG1_SHIFT)                   /* 0xFFFFFFFF */
/* OS_REG2 */
#define GRF_OS_REG2_OFFSET                                 (0x208)
#define GRF_OS_REG2_OS_REG2_SHIFT                          (0U)
#define GRF_OS_REG2_OS_REG2_MASK                           (0xFFFFFFFFU << GRF_OS_REG2_OS_REG2_SHIFT)                   /* 0xFFFFFFFF */
/* OS_REG3 */
#define GRF_OS_REG3_OFFSET                                 (0x20C)
#define GRF_OS_REG3_OS_REG3_SHIFT                          (0U)
#define GRF_OS_REG3_OS_REG3_MASK                           (0xFFFFFFFFU << GRF_OS_REG3_OS_REG3_SHIFT)                   /* 0xFFFFFFFF */
/* BBGEN_CON0 */
#define GRF_BBGEN_CON0_OFFSET                              (0x280)
#define GRF_BBGEN_CON0_I2C_FLT_F_SHIFT                     (0U)
#define GRF_BBGEN_CON0_I2C_FLT_F_MASK                      (0xFU << GRF_BBGEN_CON0_I2C_FLT_F_SHIFT)                     /* 0x0000000F */
#define GRF_BBGEN_CON0_I2C_FLT_R_SHIFT                     (4U)
#define GRF_BBGEN_CON0_I2C_FLT_R_MASK                      (0xFU << GRF_BBGEN_CON0_I2C_FLT_R_SHIFT)                     /* 0x000000F0 */
#define GRF_BBGEN_CON0_I2CSLADR_SHIFT                      (8U)
#define GRF_BBGEN_CON0_I2CSLADR_MASK                       (0x7U << GRF_BBGEN_CON0_I2CSLADR_SHIFT)                      /* 0x00000700 */
#define GRF_BBGEN_CON0_BBGEN_MODE_SHIFT                    (11U)
#define GRF_BBGEN_CON0_BBGEN_MODE_MASK                     (0x3U << GRF_BBGEN_CON0_BBGEN_MODE_SHIFT)                    /* 0x00001800 */
#define GRF_BBGEN_CON0_BBGEN_LUT_SELECT_SHIFT              (13U)
#define GRF_BBGEN_CON0_BBGEN_LUT_SELECT_MASK               (0x3U << GRF_BBGEN_CON0_BBGEN_LUT_SELECT_SHIFT)              /* 0x00006000 */
#define GRF_BBGEN_CON0_BBGEN_START_CTRL_SHIFT              (15U)
#define GRF_BBGEN_CON0_BBGEN_START_CTRL_MASK               (0x1U << GRF_BBGEN_CON0_BBGEN_START_CTRL_SHIFT)              /* 0x00008000 */
/* BBGEN_CON1 */
#define GRF_BBGEN_CON1_OFFSET                              (0x284)
#define GRF_BBGEN_CON1_BBGEN_CORNER_SHIFT                  (0U)
#define GRF_BBGEN_CON1_BBGEN_CORNER_MASK                   (0x3U << GRF_BBGEN_CON1_BBGEN_CORNER_SHIFT)                  /* 0x00000003 */
#define GRF_BBGEN_CON1_BBGEN_CLK_SEL_SHIFT                 (2U)
#define GRF_BBGEN_CON1_BBGEN_CLK_SEL_MASK                  (0x3U << GRF_BBGEN_CON1_BBGEN_CLK_SEL_SHIFT)                 /* 0x0000000C */
#define GRF_BBGEN_CON1_BBGEN_TCK_SHIFT                     (4U)
#define GRF_BBGEN_CON1_BBGEN_TCK_MASK                      (0x1U << GRF_BBGEN_CON1_BBGEN_TCK_SHIFT)                     /* 0x00000010 */
#define GRF_BBGEN_CON1_BBGEN_JTAG_EN_SHIFT                 (5U)
#define GRF_BBGEN_CON1_BBGEN_JTAG_EN_MASK                  (0x1U << GRF_BBGEN_CON1_BBGEN_JTAG_EN_SHIFT)                 /* 0x00000020 */
#define GRF_BBGEN_CON1_BBGEN_TDI_SHIFT                     (6U)
#define GRF_BBGEN_CON1_BBGEN_TDI_MASK                      (0x1U << GRF_BBGEN_CON1_BBGEN_TDI_SHIFT)                     /* 0x00000040 */
#define GRF_BBGEN_CON1_BBGEN_CAPTURE_SHIFT                 (7U)
#define GRF_BBGEN_CON1_BBGEN_CAPTURE_MASK                  (0x1U << GRF_BBGEN_CON1_BBGEN_CAPTURE_SHIFT)                 /* 0x00000080 */
#define GRF_BBGEN_CON1_BBGEN_UPDATE_SHIFT                  (8U)
#define GRF_BBGEN_CON1_BBGEN_UPDATE_MASK                   (0x1U << GRF_BBGEN_CON1_BBGEN_UPDATE_SHIFT)                  /* 0x00000100 */
#define GRF_BBGEN_CON1_BBGEN_SHIFT_SHIFT                   (9U)
#define GRF_BBGEN_CON1_BBGEN_SHIFT_MASK                    (0x1U << GRF_BBGEN_CON1_BBGEN_SHIFT_SHIFT)                   /* 0x00000200 */
#define GRF_BBGEN_CON1_BBGEN_TRSTN_SHIFT                   (10U)
#define GRF_BBGEN_CON1_BBGEN_TRSTN_MASK                    (0x1U << GRF_BBGEN_CON1_BBGEN_TRSTN_SHIFT)                   /* 0x00000400 */
#define GRF_BBGEN_CON1_BBGEN_RO_EN_SHIFT                   (11U)
#define GRF_BBGEN_CON1_BBGEN_RO_EN_MASK                    (0xFU << GRF_BBGEN_CON1_BBGEN_RO_EN_SHIFT)                   /* 0x00007800 */
#define GRF_BBGEN_CON1_BBGEN_RO_BYPASS_SHIFT               (15U)
#define GRF_BBGEN_CON1_BBGEN_RO_BYPASS_MASK                (0x1U << GRF_BBGEN_CON1_BBGEN_RO_BYPASS_SHIFT)               /* 0x00008000 */
/* BBGEN_CON2 */
#define GRF_BBGEN_CON2_OFFSET                              (0x288)
#define GRF_BBGEN_CON2_BBGENN_DATA_SHIFT                   (0U)
#define GRF_BBGEN_CON2_BBGENN_DATA_MASK                    (0x1FU << GRF_BBGEN_CON2_BBGENN_DATA_SHIFT)                  /* 0x0000001F */
#define GRF_BBGEN_CON2_BBGENN_LOAD_SHIFT                   (5U)
#define GRF_BBGEN_CON2_BBGENN_LOAD_MASK                    (0x1U << GRF_BBGEN_CON2_BBGENN_LOAD_SHIFT)                   /* 0x00000020 */
#define GRF_BBGEN_CON2_BBGENP_DATA_SHIFT                   (6U)
#define GRF_BBGEN_CON2_BBGENP_DATA_MASK                    (0x1FU << GRF_BBGEN_CON2_BBGENP_DATA_SHIFT)                  /* 0x000007C0 */
#define GRF_BBGEN_CON2_BBGENP_LOAD_SHIFT                   (11U)
#define GRF_BBGEN_CON2_BBGENP_LOAD_MASK                    (0x1U << GRF_BBGEN_CON2_BBGENP_LOAD_SHIFT)                   /* 0x00000800 */
#define GRF_BBGEN_CON2_BBGEN_SCAN_MODE_SHIFT               (12U)
#define GRF_BBGEN_CON2_BBGEN_SCAN_MODE_MASK                (0x1U << GRF_BBGEN_CON2_BBGEN_SCAN_MODE_SHIFT)               /* 0x00001000 */
#define GRF_BBGEN_CON2_BBGEN_SCAN_CLK_SHIFT                (13U)
#define GRF_BBGEN_CON2_BBGEN_SCAN_CLK_MASK                 (0x1U << GRF_BBGEN_CON2_BBGEN_SCAN_CLK_SHIFT)                /* 0x00002000 */
#define GRF_BBGEN_CON2_BBGEN_SCAN_ENABLE_SHIFT             (14U)
#define GRF_BBGEN_CON2_BBGEN_SCAN_ENABLE_MASK              (0x1U << GRF_BBGEN_CON2_BBGEN_SCAN_ENABLE_SHIFT)             /* 0x00004000 */
#define GRF_BBGEN_CON2_BBGEN_SCAN_RSTN_SHIFT               (15U)
#define GRF_BBGEN_CON2_BBGEN_SCAN_RSTN_MASK                (0x1U << GRF_BBGEN_CON2_BBGEN_SCAN_RSTN_SHIFT)               /* 0x00008000 */
/* BBGEN_CON3 */
#define GRF_BBGEN_CON3_OFFSET                              (0x28C)
#define GRF_BBGEN_CON3_BBGEN_RESERVED_SHIFT                (0U)
#define GRF_BBGEN_CON3_BBGEN_RESERVED_MASK                 (0xFU << GRF_BBGEN_CON3_BBGEN_RESERVED_SHIFT)                /* 0x0000000F */
#define GRF_BBGEN_CON3_BBGEN_INTR_POS_CLR_SHIFT            (4U)
#define GRF_BBGEN_CON3_BBGEN_INTR_POS_CLR_MASK             (0x1U << GRF_BBGEN_CON3_BBGEN_INTR_POS_CLR_SHIFT)            /* 0x00000010 */
#define GRF_BBGEN_CON3_BBGEN_INTR_NEG_CLR_SHIFT            (5U)
#define GRF_BBGEN_CON3_BBGEN_INTR_NEG_CLR_MASK             (0x1U << GRF_BBGEN_CON3_BBGEN_INTR_NEG_CLR_SHIFT)            /* 0x00000020 */
/* BBGEN_STATUS */
#define GRF_BBGEN_STATUS_OFFSET                            (0x290)
#define GRF_BBGEN_STATUS_BBGEN_INTR_NEG_SHIFT              (0U)
#define GRF_BBGEN_STATUS_BBGEN_INTR_NEG_MASK               (0x1U << GRF_BBGEN_STATUS_BBGEN_INTR_NEG_SHIFT)              /* 0x00000001 */
#define GRF_BBGEN_STATUS_BBGEN_INTR_POS_SHIFT              (1U)
#define GRF_BBGEN_STATUS_BBGEN_INTR_POS_MASK               (0x1U << GRF_BBGEN_STATUS_BBGEN_INTR_POS_SHIFT)              /* 0x00000002 */
#define GRF_BBGEN_STATUS_BBGEN_INITREADY_SHIFT             (2U)
#define GRF_BBGEN_STATUS_BBGEN_INITREADY_MASK              (0x1U << GRF_BBGEN_STATUS_BBGEN_INITREADY_SHIFT)             /* 0x00000004 */
#define GRF_BBGEN_STATUS_BBGEN_ROOUT_SHIFT                 (3U)
#define GRF_BBGEN_STATUS_BBGEN_ROOUT_MASK                  (0x1U << GRF_BBGEN_STATUS_BBGEN_ROOUT_SHIFT)                 /* 0x00000008 */
#define GRF_BBGEN_STATUS_GRF_BBGEN_TDO_SHIFT               (4U)
#define GRF_BBGEN_STATUS_GRF_BBGEN_TDO_MASK                (0x1U << GRF_BBGEN_STATUS_GRF_BBGEN_TDO_SHIFT)               /* 0x00000010 */
/* DLL_CON0 */
#define GRF_DLL_CON0_OFFSET                                (0x2C0)
#define GRF_DLL_CON0_PVTM_START_SHIFT                      (0U)
#define GRF_DLL_CON0_PVTM_START_MASK                       (0x1U << GRF_DLL_CON0_PVTM_START_SHIFT)                      /* 0x00000001 */
#define GRF_DLL_CON0_OSC_EN_SHIFT                          (1U)
#define GRF_DLL_CON0_OSC_EN_MASK                           (0x1U << GRF_DLL_CON0_OSC_EN_SHIFT)                          /* 0x00000002 */
#define GRF_DLL_CON0_PVTM_RING_SEL_SHIFT                   (2U)
#define GRF_DLL_CON0_PVTM_RING_SEL_MASK                    (0x7U << GRF_DLL_CON0_PVTM_RING_SEL_SHIFT)                   /* 0x0000001C */
/* DLL_CON1 */
#define GRF_DLL_CON1_OFFSET                                (0x2C4)
#define GRF_DLL_CON1_PVTM_CAL_CNT_SHIFT                    (0U)
#define GRF_DLL_CON1_PVTM_CAL_CNT_MASK                     (0xFFFFFFFFU << GRF_DLL_CON1_PVTM_CAL_CNT_SHIFT)             /* 0xFFFFFFFF */
/* DLL_STATUS0 */
#define GRF_DLL_STATUS0_OFFSET                             (0x2C8)
#define GRF_DLL_STATUS0_PVTM_FREQ_DONE_SHIFT               (0U)
#define GRF_DLL_STATUS0_PVTM_FREQ_DONE_MASK                (0x1U << GRF_DLL_STATUS0_PVTM_FREQ_DONE_SHIFT)               /* 0x00000001 */
/* DLL_STATUS1 */
#define GRF_DLL_STATUS1_OFFSET                             (0x2CC)
#define GRF_DLL_STATUS1_PVTM_FREQ_CNT_SHIFT                (0U)
#define GRF_DLL_STATUS1_PVTM_FREQ_CNT_MASK                 (0xFFFFFFFFU << GRF_DLL_STATUS1_PVTM_FREQ_CNT_SHIFT)         /* 0xFFFFFFFF */
/* FAST_BOOT_EN */
#define GRF_FAST_BOOT_EN_OFFSET                            (0x300)
#define GRF_FAST_BOOT_EN_FAST_BOOT_EN_SHIFT                (0U)
#define GRF_FAST_BOOT_EN_FAST_BOOT_EN_MASK                 (0x1U << GRF_FAST_BOOT_EN_FAST_BOOT_EN_SHIFT)                /* 0x00000001 */
/* FAST_BOOT_ADDR */
#define GRF_FAST_BOOT_ADDR_OFFSET                          (0x304)
#define GRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_SHIFT            (0U)
#define GRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_MASK             (0xFFFFFFFFU << GRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_SHIFT)     /* 0xFFFFFFFF */

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
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_OFFSET                                    (0x0)
#define UART_RBR_RBR_SHIFT                                 (0U)
#define UART_RBR_RBR_MASK                                  (0xFFU << UART_RBR_RBR_SHIFT)                                /* 0x000000FF */
/* DLH */
#define UART_DLH_OFFSET                                    (0x4)
#define UART_DLH_DLH_SHIFT                                 (0U)
#define UART_DLH_DLH_MASK                                  (0xFFU << UART_DLH_DLH_SHIFT)                                /* 0x000000FF */
/* IIR */
#define UART_IIR_OFFSET                                    (0x8)
#define UART_IIR_IID_SHIFT                                 (0U)
#define UART_IIR_IID_MASK                                  (0xFU << UART_IIR_IID_SHIFT)                                 /* 0x0000000F */
#define UART_IIR_FIFOSE_SHIFT                              (6U)
#define UART_IIR_FIFOSE_MASK                               (0x3U << UART_IIR_FIFOSE_SHIFT)                              /* 0x000000C0 */
/* LCR */
#define UART_LCR_OFFSET                                    (0xC)
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
#define UART_MCR_OFFSET                                    (0x10)
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
#define UART_LSR_OFFSET                                    (0x14)
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
#define UART_MSR_OFFSET                                    (0x18)
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
#define UART_SCR_OFFSET                                    (0x1C)
#define UART_SCR_SCR_SHIFT                                 (0U)
#define UART_SCR_SCR_MASK                                  (0xFFU << UART_SCR_SCR_SHIFT)                                /* 0x000000FF */
/* SRBR */
#define UART_SRBR_OFFSET                                   (0x30)
#define UART_SRBR_SRBR_SHIFT                               (0U)
#define UART_SRBR_SRBR_MASK                                (0xFFU << UART_SRBR_SRBR_SHIFT)                              /* 0x000000FF */
/* FAR */
#define UART_FAR_OFFSET                                    (0x70)
#define UART_FAR_FAR_SHIFT                                 (0U)
#define UART_FAR_FAR_MASK                                  (0x1U << UART_FAR_FAR_SHIFT)                                 /* 0x00000001 */
/* TFR */
#define UART_TFR_OFFSET                                    (0x74)
#define UART_TFR_TFR_SHIFT                                 (0U)
#define UART_TFR_TFR_MASK                                  (0xFFU << UART_TFR_TFR_SHIFT)                                /* 0x000000FF */
/* RFW */
#define UART_RFW_OFFSET                                    (0x78)
#define UART_RFW_RFWD_SHIFT                                (0U)
#define UART_RFW_RFWD_MASK                                 (0xFFU << UART_RFW_RFWD_SHIFT)                               /* 0x000000FF */
#define UART_RFW_REPE_SHIFT                                (8U)
#define UART_RFW_REPE_MASK                                 (0x1U << UART_RFW_REPE_SHIFT)                                /* 0x00000100 */
#define UART_RFW_REFE_SHIFT                                (9U)
#define UART_RFW_REFE_MASK                                 (0x1U << UART_RFW_REFE_SHIFT)                                /* 0x00000200 */
/* USR */
#define UART_USR_OFFSET                                    (0x7C)
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
#define UART_TFL_OFFSET                                    (0x80)
#define UART_TFL_TFL_SHIFT                                 (0U)
#define UART_TFL_TFL_MASK                                  (0x3FU << UART_TFL_TFL_SHIFT)                                /* 0x0000003F */
/* RFL */
#define UART_RFL_OFFSET                                    (0x84)
#define UART_RFL_RFL_SHIFT                                 (0U)
#define UART_RFL_RFL_MASK                                  (0x3FU << UART_RFL_RFL_SHIFT)                                /* 0x0000003F */
/* SRR */
#define UART_SRR_OFFSET                                    (0x88)
#define UART_SRR_UR_SHIFT                                  (0U)
#define UART_SRR_UR_MASK                                   (0x1U << UART_SRR_UR_SHIFT)                                  /* 0x00000001 */
#define UART_SRR_RFR_SHIFT                                 (1U)
#define UART_SRR_RFR_MASK                                  (0x1U << UART_SRR_RFR_SHIFT)                                 /* 0x00000002 */
#define UART_SRR_XFR_SHIFT                                 (2U)
#define UART_SRR_XFR_MASK                                  (0x1U << UART_SRR_XFR_SHIFT)                                 /* 0x00000004 */
/* SRTS */
#define UART_SRTS_OFFSET                                   (0x8C)
#define UART_SRTS_SRTS_SHIFT                               (0U)
#define UART_SRTS_SRTS_MASK                                (0x1U << UART_SRTS_SRTS_SHIFT)                               /* 0x00000001 */
/* SBCR */
#define UART_SBCR_OFFSET                                   (0x90)
#define UART_SBCR_SBCR_SHIFT                               (0U)
#define UART_SBCR_SBCR_MASK                                (0x1U << UART_SBCR_SBCR_SHIFT)                               /* 0x00000001 */
/* SDMAM */
#define UART_SDMAM_OFFSET                                  (0x94)
#define UART_SDMAM_SDMAM_SHIFT                             (0U)
#define UART_SDMAM_SDMAM_MASK                              (0x1U << UART_SDMAM_SDMAM_SHIFT)                             /* 0x00000001 */
/* SFE */
#define UART_SFE_OFFSET                                    (0x98)
#define UART_SFE_SFE_SHIFT                                 (0U)
#define UART_SFE_SFE_MASK                                  (0x1U << UART_SFE_SFE_SHIFT)                                 /* 0x00000001 */
/* SRT */
#define UART_SRT_OFFSET                                    (0x9C)
#define UART_SRT_SRT_SHIFT                                 (0U)
#define UART_SRT_SRT_MASK                                  (0x3U << UART_SRT_SRT_SHIFT)                                 /* 0x00000003 */
/* STET */
#define UART_STET_OFFSET                                   (0xA0)
#define UART_STET_STET_SHIFT                               (0U)
#define UART_STET_STET_MASK                                (0x3U << UART_STET_STET_SHIFT)                               /* 0x00000003 */
/* HTX */
#define UART_HTX_OFFSET                                    (0xA4)
#define UART_HTX_HXT_SHIFT                                 (0U)
#define UART_HTX_HXT_MASK                                  (0x1U << UART_HTX_HXT_SHIFT)                                 /* 0x00000001 */
/* DMASA */
#define UART_DMASA_OFFSET                                  (0xA8)
#define UART_DMASA_DMASA_SHIFT                             (0U)
#define UART_DMASA_DMASA_MASK                              (0x1U << UART_DMASA_DMASA_SHIFT)                             /* 0x00000001 */
/* CPR */
#define UART_CPR_OFFSET                                    (0xF4)
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
#define UART_UCV_OFFSET                                    (0xF8)
#define UART_UCV_UCV_SHIFT                                 (0U)
#define UART_UCV_UCV_MASK                                  (0xFFFFFFFFU << UART_UCV_UCV_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_OFFSET                                    (0xFC)
#define UART_CTR_ID_SHIFT                                  (0U)
#define UART_CTR_ID_MASK                                   (0xFFFFFFFFU << UART_CTR_ID_SHIFT)                           /* 0xFFFFFFFF */
/* THR */
#define UART_THR_OFFSET                                    (0x100)
#define UART_THR_THR_SHIFT                                 (0U)
#define UART_THR_THR_MASK                                  (0xFFU << UART_THR_THR_SHIFT)                                /* 0x000000FF */
/* IER */
#define UART_IER_OFFSET                                    (0x104)
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
#define UART_FCR_OFFSET                                    (0x108)
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
/* STHR */
#define UART_STHR_OFFSET                                   (0x130)
#define UART_STHR_STHR_SHIFT                               (0U)
#define UART_STHR_STHR_MASK                                (0x1U << UART_STHR_STHR_SHIFT)                               /* 0x00000001 */
/* DLL */
#define UART_DLL_OFFSET                                    (0x2000)
#define UART_DLL_DLL_SHIFT                                 (0U)
#define UART_DLL_DLL_MASK                                  (0xFFU << UART_DLL_DLL_SHIFT)                                /* 0x000000FF */
/******************************************WDT*******************************************/
#define WDT WDT0
#define PCLK_WDT PCLK_MCU
/* WDT_CR */
#define WDT_CR_OFFSET                                  (0x0)
#define WDT_CR_WDT_EN_SHIFT                            (0U)
#define WDT_CR_WDT_EN_MASK                             (0x1U << WDT_CR_WDT_EN_SHIFT)                            /* 0x00000001 */
#define WDT_CR_RESP_MODE_SHIFT                         (1U)
#define WDT_CR_RESP_MODE_MASK                          (0x1U << WDT_CR_RESP_MODE_SHIFT)                         /* 0x00000002 */
#define WDT_CR_RST_PLUSE_LENGTH_SHIFT                  (2U)
#define WDT_CR_RST_PLUSE_LENGTH_MASK                   (0x3U << WDT_CR_RST_PLUSE_LENGTH_SHIFT)                  /* 0x0000000C */
/* WDT_TORR */
#define WDT_TORR_OFFSET                                (0x4)
#define WDT_TORR_TIMEOUT_PERIOD_SHIFT                  (0U)
#define WDT_TORR_TIMEOUT_PERIOD_MASK                   (0xFU << WDT_TORR_TIMEOUT_PERIOD_SHIFT)                  /* 0x0000000F */
/* WDT_CCVR */
#define WDT_CCVR_OFFSET                                (0x8)
#define WDT_CCVR_CUR_CNT_SHIFT                         (0U)
#define WDT_CCVR_CUR_CNT_MASK                          (0xFFFFFFFFU << WDT_CCVR_CUR_CNT_SHIFT)                  /* 0xFFFFFFFF */
/* WDT_CRR */
#define WDT_CRR_OFFSET                                 (0xC)
#define WDT_CRR_CNT_RESTART_SHIFT                      (0U)
#define WDT_CRR_CNT_RESTART_MASK                       (0xFFU << WDT_CRR_CNT_RESTART_SHIFT)                     /* 0x000000FF */
/* WDT_STAT */
#define WDT_STAT_OFFSET                                (0x10)
#define WDT_STAT_WDT_STATUS_SHIFT                      (0U)
#define WDT_STAT_WDT_STATUS_MASK                       (0x1U << WDT_STAT_WDT_STATUS_SHIFT)                      /* 0x00000001 */
/* WDT_EOI */
#define WDT_EOI_OFFSET                                 (0x14)
#define WDT_EOI_WDT_INT_CLR_SHIFT                      (0U)
#define WDT_EOI_WDT_INT_CLR_MASK                       (0x1U << WDT_EOI_WDT_INT_CLR_SHIFT)                      /* 0x00000001 */
/******************************************MBOX******************************************/
/* A2B_INTEN */
#define MBOX_A2B_INTEN_OFFSET                              (0x0)
#define MBOX_A2B_INTEN_INT0_SHIFT                          (0U)
#define MBOX_A2B_INTEN_INT0_MASK                           (0x1U << MBOX_A2B_INTEN_INT0_SHIFT)                          /* 0x00000001 */
#define MBOX_A2B_INTEN_INT1_SHIFT                          (1U)
#define MBOX_A2B_INTEN_INT1_MASK                           (0x1U << MBOX_A2B_INTEN_INT1_SHIFT)                          /* 0x00000002 */
#define MBOX_A2B_INTEN_INT2_SHIFT                          (2U)
#define MBOX_A2B_INTEN_INT2_MASK                           (0x1U << MBOX_A2B_INTEN_INT2_SHIFT)                          /* 0x00000004 */
#define MBOX_A2B_INTEN_INT3_SHIFT                          (3U)
#define MBOX_A2B_INTEN_INT3_MASK                           (0x1U << MBOX_A2B_INTEN_INT3_SHIFT)                          /* 0x00000008 */
/* A2B_STATUS */
#define MBOX_A2B_STATUS_OFFSET                             (0x4)
#define MBOX_A2B_STATUS_INT0_SHIFT                         (0U)
#define MBOX_A2B_STATUS_INT0_MASK                          (0x1U << MBOX_A2B_STATUS_INT0_SHIFT)                         /* 0x00000001 */
#define MBOX_A2B_STATUS_INT1_SHIFT                         (1U)
#define MBOX_A2B_STATUS_INT1_MASK                          (0x1U << MBOX_A2B_STATUS_INT1_SHIFT)                         /* 0x00000002 */
#define MBOX_A2B_STATUS_INT2_SHIFT                         (2U)
#define MBOX_A2B_STATUS_INT2_MASK                          (0x1U << MBOX_A2B_STATUS_INT2_SHIFT)                         /* 0x00000004 */
#define MBOX_A2B_STATUS_INT3_SHIFT                         (3U)
#define MBOX_A2B_STATUS_INT3_MASK                          (0x1U << MBOX_A2B_STATUS_INT3_SHIFT)                         /* 0x00000008 */
/* A2B_CMD_0 */
#define MBOX_A2B_CMD_0_OFFSET                              (0x8)
#define MBOX_A2B_CMD_0_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_0_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_0_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_0 */
#define MBOX_A2B_DAT_0_OFFSET                              (0xC)
#define MBOX_A2B_DAT_0_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_0_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_0_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_1 */
#define MBOX_A2B_CMD_1_OFFSET                              (0x10)
#define MBOX_A2B_CMD_1_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_1_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_1_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_1 */
#define MBOX_A2B_DAT_1_OFFSET                              (0x14)
#define MBOX_A2B_DAT_1_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_1_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_1_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_2 */
#define MBOX_A2B_CMD_2_OFFSET                              (0x18)
#define MBOX_A2B_CMD_2_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_2_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_2_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_2 */
#define MBOX_A2B_DAT_2_OFFSET                              (0x1C)
#define MBOX_A2B_DAT_2_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_2_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_2_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_3 */
#define MBOX_A2B_CMD_3_OFFSET                              (0x20)
#define MBOX_A2B_CMD_3_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_3_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_3_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* MBOX_CMD_DAT */
#define MBOX_MBOX_CMD_DAT_OFFSET                           (0x24)
#define MBOX_MBOX_CMD_DAT_DATA_SHIFT                       (0U)
#define MBOX_MBOX_CMD_DAT_DATA_MASK                        (0xFFFFFFFFU << MBOX_MBOX_CMD_DAT_DATA_SHIFT)                /* 0xFFFFFFFF */
/* B2A_INTEN */
#define MBOX_B2A_INTEN_OFFSET                              (0x28)
#define MBOX_B2A_INTEN_INT0_SHIFT                          (0U)
#define MBOX_B2A_INTEN_INT0_MASK                           (0x1U << MBOX_B2A_INTEN_INT0_SHIFT)                          /* 0x00000001 */
#define MBOX_B2A_INTEN_INT1_SHIFT                          (1U)
#define MBOX_B2A_INTEN_INT1_MASK                           (0x1U << MBOX_B2A_INTEN_INT1_SHIFT)                          /* 0x00000002 */
#define MBOX_B2A_INTEN_INT2_SHIFT                          (2U)
#define MBOX_B2A_INTEN_INT2_MASK                           (0x1U << MBOX_B2A_INTEN_INT2_SHIFT)                          /* 0x00000004 */
#define MBOX_B2A_INTEN_INT3_SHIFT                          (3U)
#define MBOX_B2A_INTEN_INT3_MASK                           (0x1U << MBOX_B2A_INTEN_INT3_SHIFT)                          /* 0x00000008 */
/* B2A_STATUS */
#define MBOX_B2A_STATUS_OFFSET                             (0x2C)
#define MBOX_B2A_STATUS_INT0_SHIFT                         (0U)
#define MBOX_B2A_STATUS_INT0_MASK                          (0x1U << MBOX_B2A_STATUS_INT0_SHIFT)                         /* 0x00000001 */
#define MBOX_B2A_STATUS_INT1_SHIFT                         (1U)
#define MBOX_B2A_STATUS_INT1_MASK                          (0x1U << MBOX_B2A_STATUS_INT1_SHIFT)                         /* 0x00000002 */
#define MBOX_B2A_STATUS_INT2_SHIFT                         (2U)
#define MBOX_B2A_STATUS_INT2_MASK                          (0x1U << MBOX_B2A_STATUS_INT2_SHIFT)                         /* 0x00000004 */
#define MBOX_B2A_STATUS_INT3_SHIFT                         (3U)
#define MBOX_B2A_STATUS_INT3_MASK                          (0x1U << MBOX_B2A_STATUS_INT3_SHIFT)                         /* 0x00000008 */
/* B2A_CMD_0 */
#define MBOX_B2A_CMD_0_OFFSET                              (0x30)
#define MBOX_B2A_CMD_0_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_0_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_0_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_0 */
#define MBOX_B2A_DAT_0_OFFSET                              (0x34)
#define MBOX_B2A_DAT_0_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_0_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_0_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_1 */
#define MBOX_B2A_CMD_1_OFFSET                              (0x38)
#define MBOX_B2A_CMD_1_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_1_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_1_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_1 */
#define MBOX_B2A_DAT_1_OFFSET                              (0x3C)
#define MBOX_B2A_DAT_1_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_1_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_1_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_2 */
#define MBOX_B2A_CMD_2_OFFSET                              (0x40)
#define MBOX_B2A_CMD_2_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_2_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_2_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_2 */
#define MBOX_B2A_DAT_2_OFFSET                              (0x44)
#define MBOX_B2A_DAT_2_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_2_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_2_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_3 */
#define MBOX_B2A_CMD_3_OFFSET                              (0x48)
#define MBOX_B2A_CMD_3_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_3_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_3_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_3 */
#define MBOX_B2A_DAT_3_OFFSET                              (0x4C)
#define MBOX_B2A_DAT_3_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_3_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_3_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* ATOMIC_LOCK_00 */
#define MBOX_ATOMIC_LOCK_00_OFFSET                         (0x100)
#define MBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_MASK            (0x1U << MBOX_ATOMIC_LOCK_00_ATOMIC_LOCK_00_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_01 */
#define MBOX_ATOMIC_LOCK_01_OFFSET                         (0x104)
#define MBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_MASK            (0x1U << MBOX_ATOMIC_LOCK_01_ATOMIC_LOCK_01_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_02 */
#define MBOX_ATOMIC_LOCK_02_OFFSET                         (0x108)
#define MBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_MASK            (0x1U << MBOX_ATOMIC_LOCK_02_ATOMIC_LOCK_02_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_03 */
#define MBOX_ATOMIC_LOCK_03_OFFSET                         (0x10C)
#define MBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_MASK            (0x1U << MBOX_ATOMIC_LOCK_03_ATOMIC_LOCK_03_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_04 */
#define MBOX_ATOMIC_LOCK_04_OFFSET                         (0x110)
#define MBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_MASK            (0x1U << MBOX_ATOMIC_LOCK_04_ATOMIC_LOCK_04_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_05 */
#define MBOX_ATOMIC_LOCK_05_OFFSET                         (0x114)
#define MBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_MASK            (0x1U << MBOX_ATOMIC_LOCK_05_ATOMIC_LOCK_05_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_06 */
#define MBOX_ATOMIC_LOCK_06_OFFSET                         (0x118)
#define MBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_MASK            (0x1U << MBOX_ATOMIC_LOCK_06_ATOMIC_LOCK_06_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_07 */
#define MBOX_ATOMIC_LOCK_07_OFFSET                         (0x11C)
#define MBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_MASK            (0x1U << MBOX_ATOMIC_LOCK_07_ATOMIC_LOCK_07_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_08 */
#define MBOX_ATOMIC_LOCK_08_OFFSET                         (0x120)
#define MBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_MASK            (0x1U << MBOX_ATOMIC_LOCK_08_ATOMIC_LOCK_08_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_09 */
#define MBOX_ATOMIC_LOCK_09_OFFSET                         (0x124)
#define MBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_MASK            (0x1U << MBOX_ATOMIC_LOCK_09_ATOMIC_LOCK_09_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_10 */
#define MBOX_ATOMIC_LOCK_10_OFFSET                         (0x128)
#define MBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_MASK            (0x1U << MBOX_ATOMIC_LOCK_10_ATOMIC_LOCK_10_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_11 */
#define MBOX_ATOMIC_LOCK_11_OFFSET                         (0x12C)
#define MBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_MASK            (0x1U << MBOX_ATOMIC_LOCK_11_ATOMIC_LOCK_11_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_12 */
#define MBOX_ATOMIC_LOCK_12_OFFSET                         (0x130)
#define MBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_MASK            (0x1U << MBOX_ATOMIC_LOCK_12_ATOMIC_LOCK_12_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_13 */
#define MBOX_ATOMIC_LOCK_13_OFFSET                         (0x134)
#define MBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_MASK            (0x1U << MBOX_ATOMIC_LOCK_13_ATOMIC_LOCK_13_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_14 */
#define MBOX_ATOMIC_LOCK_14_OFFSET                         (0x138)
#define MBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_MASK            (0x1U << MBOX_ATOMIC_LOCK_14_ATOMIC_LOCK_14_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_15 */
#define MBOX_ATOMIC_LOCK_15_OFFSET                         (0x13C)
#define MBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_MASK            (0x1U << MBOX_ATOMIC_LOCK_15_ATOMIC_LOCK_15_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_16 */
#define MBOX_ATOMIC_LOCK_16_OFFSET                         (0x140)
#define MBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_MASK            (0x1U << MBOX_ATOMIC_LOCK_16_ATOMIC_LOCK_16_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_17 */
#define MBOX_ATOMIC_LOCK_17_OFFSET                         (0x144)
#define MBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_MASK            (0x1U << MBOX_ATOMIC_LOCK_17_ATOMIC_LOCK_17_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_18 */
#define MBOX_ATOMIC_LOCK_18_OFFSET                         (0x148)
#define MBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_MASK            (0x1U << MBOX_ATOMIC_LOCK_18_ATOMIC_LOCK_18_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_19 */
#define MBOX_ATOMIC_LOCK_19_OFFSET                         (0x14C)
#define MBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_MASK            (0x1U << MBOX_ATOMIC_LOCK_19_ATOMIC_LOCK_19_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_20 */
#define MBOX_ATOMIC_LOCK_20_OFFSET                         (0x150)
#define MBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_MASK            (0x1U << MBOX_ATOMIC_LOCK_20_ATOMIC_LOCK_20_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_21 */
#define MBOX_ATOMIC_LOCK_21_OFFSET                         (0x154)
#define MBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_MASK            (0x1U << MBOX_ATOMIC_LOCK_21_ATOMIC_LOCK_21_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_22 */
#define MBOX_ATOMIC_LOCK_22_OFFSET                         (0x158)
#define MBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_MASK            (0x1U << MBOX_ATOMIC_LOCK_22_ATOMIC_LOCK_22_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_23 */
#define MBOX_ATOMIC_LOCK_23_OFFSET                         (0x15C)
#define MBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_MASK            (0x1U << MBOX_ATOMIC_LOCK_23_ATOMIC_LOCK_23_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_24 */
#define MBOX_ATOMIC_LOCK_24_OFFSET                         (0x160)
#define MBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_MASK            (0x1U << MBOX_ATOMIC_LOCK_24_ATOMIC_LOCK_24_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_25 */
#define MBOX_ATOMIC_LOCK_25_OFFSET                         (0x164)
#define MBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_MASK            (0x1U << MBOX_ATOMIC_LOCK_25_ATOMIC_LOCK_25_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_26 */
#define MBOX_ATOMIC_LOCK_26_OFFSET                         (0x168)
#define MBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_MASK            (0x1U << MBOX_ATOMIC_LOCK_26_ATOMIC_LOCK_26_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_27 */
#define MBOX_ATOMIC_LOCK_27_OFFSET                         (0x16C)
#define MBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_MASK            (0x1U << MBOX_ATOMIC_LOCK_27_ATOMIC_LOCK_27_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_28 */
#define MBOX_ATOMIC_LOCK_28_OFFSET                         (0x170)
#define MBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_MASK            (0x1U << MBOX_ATOMIC_LOCK_28_ATOMIC_LOCK_28_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_29 */
#define MBOX_ATOMIC_LOCK_29_OFFSET                         (0x174)
#define MBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_MASK            (0x1U << MBOX_ATOMIC_LOCK_29_ATOMIC_LOCK_29_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_30 */
#define MBOX_ATOMIC_LOCK_30_OFFSET                         (0x178)
#define MBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_MASK            (0x1U << MBOX_ATOMIC_LOCK_30_ATOMIC_LOCK_30_SHIFT)           /* 0x00000001 */
/* ATOMIC_LOCK_31 */
#define MBOX_ATOMIC_LOCK_31_OFFSET                         (0x17C)
#define MBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_SHIFT           (0U)
#define MBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_MASK            (0x1U << MBOX_ATOMIC_LOCK_31_ATOMIC_LOCK_31_SHIFT)           /* 0x00000001 */
/******************************************SPI*******************************************/
/* CTRLR0 */
#define SPI_CTRLR0_OFFSET                                  (0x0)
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
#define SPI_CTRLR1_OFFSET                                  (0x4)
#define SPI_CTRLR1_NDM_SHIFT                               (0U)
#define SPI_CTRLR1_NDM_MASK                                (0xFFFFFFFFU << SPI_CTRLR1_NDM_SHIFT)                        /* 0xFFFFFFFF */
/* ENR */
#define SPI_ENR_OFFSET                                     (0x8)
#define SPI_ENR_ENR_SHIFT                                  (0U)
#define SPI_ENR_ENR_MASK                                   (0x1U << SPI_ENR_ENR_SHIFT)                                  /* 0x00000001 */
/* SER */
#define SPI_SER_OFFSET                                     (0xC)
#define SPI_SER_SER_SHIFT                                  (0U)
#define SPI_SER_SER_MASK                                   (0x3U << SPI_SER_SER_SHIFT)                                  /* 0x00000003 */
/* BAUDR */
#define SPI_BAUDR_OFFSET                                   (0x10)
#define SPI_BAUDR_BAUDR_SHIFT                              (0U)
#define SPI_BAUDR_BAUDR_MASK                               (0xFFFFU << SPI_BAUDR_BAUDR_SHIFT)                           /* 0x0000FFFF */
/* TXFTLR */
#define SPI_TXFTLR_OFFSET                                  (0x14)
#define SPI_TXFTLR_TXFTLR_SHIFT                            (0U)
#define SPI_TXFTLR_TXFTLR_MASK                             (0x3FU << SPI_TXFTLR_TXFTLR_SHIFT)                           /* 0x0000003F */
/* RXFTLR */
#define SPI_RXFTLR_OFFSET                                  (0x18)
#define SPI_RXFTLR_RXFTLR_SHIFT                            (0U)
#define SPI_RXFTLR_RXFTLR_MASK                             (0x3FU << SPI_RXFTLR_RXFTLR_SHIFT)                           /* 0x0000003F */
/* TXFLR */
#define SPI_TXFLR_OFFSET                                   (0x1C)
#define SPI_TXFLR_TXFLR_SHIFT                              (0U)
#define SPI_TXFLR_TXFLR_MASK                               (0x7FU << SPI_TXFLR_TXFLR_SHIFT)                             /* 0x0000007F */
/* RXFLR */
#define SPI_RXFLR_OFFSET                                   (0x20)
#define SPI_RXFLR_RXFLR_SHIFT                              (0U)
#define SPI_RXFLR_RXFLR_MASK                               (0x7FU << SPI_RXFLR_RXFLR_SHIFT)                             /* 0x0000007F */
/* SR */
#define SPI_SR_OFFSET                                      (0x24)
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
#define SPI_IPR_OFFSET                                     (0x28)
#define SPI_IPR_IPR_SHIFT                                  (0U)
#define SPI_IPR_IPR_MASK                                   (0x1U << SPI_IPR_IPR_SHIFT)                                  /* 0x00000001 */
/* IMR */
#define SPI_IMR_OFFSET                                     (0x2C)
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
#define SPI_ISR_OFFSET                                     (0x30)
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
#define SPI_RISR_OFFSET                                    (0x34)
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
#define SPI_ICR_OFFSET                                     (0x38)
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
#define SPI_DMACR_OFFSET                                   (0x3C)
#define SPI_DMACR_RDE_SHIFT                                (0U)
#define SPI_DMACR_RDE_MASK                                 (0x1U << SPI_DMACR_RDE_SHIFT)                                /* 0x00000001 */
#define SPI_DMACR_TDE_SHIFT                                (1U)
#define SPI_DMACR_TDE_MASK                                 (0x1U << SPI_DMACR_TDE_SHIFT)                                /* 0x00000002 */
/* DMATDLR */
#define SPI_DMATDLR_OFFSET                                 (0x40)
#define SPI_DMATDLR_TDL_SHIFT                              (0U)
#define SPI_DMATDLR_TDL_MASK                               (0x3FU << SPI_DMATDLR_TDL_SHIFT)                             /* 0x0000003F */
/* DMARDLR */
#define SPI_DMARDLR_OFFSET                                 (0x44)
#define SPI_DMARDLR_RDL_SHIFT                              (0U)
#define SPI_DMARDLR_RDL_MASK                               (0x3FU << SPI_DMARDLR_RDL_SHIFT)                             /* 0x0000003F */
/* TIMEOUT */
#define SPI_TIMEOUT_OFFSET                                 (0x4C)
#define SPI_TIMEOUT_TOV_SHIFT                              (0U)
#define SPI_TIMEOUT_TOV_MASK                               (0xFFFFU << SPI_TIMEOUT_TOV_SHIFT)                           /* 0x0000FFFF */
#define SPI_TIMEOUT_TOE_SHIFT                              (16U)
#define SPI_TIMEOUT_TOE_MASK                               (0x1U << SPI_TIMEOUT_TOE_SHIFT)                              /* 0x00010000 */
/* BYPASS */
#define SPI_BYPASS_OFFSET                                  (0x50)
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
#define SPI_TXDR_OFFSET                                    (0x400)
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_OFFSET                                    (0x800)
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/******************************************I2C*******************************************/
/* CON */
#define I2C_CON_OFFSET                                     (0x0)
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
#define I2C_CLKDIV_OFFSET                                  (0x4)
#define I2C_CLKDIV_CLKDIVL_SHIFT                           (0U)
#define I2C_CLKDIV_CLKDIVL_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVL_SHIFT)                        /* 0x0000FFFF */
#define I2C_CLKDIV_CLKDIVH_SHIFT                           (16U)
#define I2C_CLKDIV_CLKDIVH_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVH_SHIFT)                        /* 0xFFFF0000 */
/* MRXADDR */
#define I2C_MRXADDR_OFFSET                                 (0x8)
#define I2C_MRXADDR_SADDR_SHIFT                            (0U)
#define I2C_MRXADDR_SADDR_MASK                             (0xFFFFFFU << I2C_MRXADDR_SADDR_SHIFT)                       /* 0x00FFFFFF */
#define I2C_MRXADDR_ADDLVLD_SHIFT                          (24U)
#define I2C_MRXADDR_ADDLVLD_MASK                           (0x1U << I2C_MRXADDR_ADDLVLD_SHIFT)                          /* 0x01000000 */
#define I2C_MRXADDR_ADDMVLD_SHIFT                          (25U)
#define I2C_MRXADDR_ADDMVLD_MASK                           (0x1U << I2C_MRXADDR_ADDMVLD_SHIFT)                          /* 0x02000000 */
#define I2C_MRXADDR_ADDHVLD_SHIFT                          (26U)
#define I2C_MRXADDR_ADDHVLD_MASK                           (0x1U << I2C_MRXADDR_ADDHVLD_SHIFT)                          /* 0x04000000 */
/* MRXRADDR */
#define I2C_MRXRADDR_OFFSET                                (0xC)
#define I2C_MRXRADDR_SRADDR_SHIFT                          (0U)
#define I2C_MRXRADDR_SRADDR_MASK                           (0xFFFFFFU << I2C_MRXRADDR_SRADDR_SHIFT)                     /* 0x00FFFFFF */
#define I2C_MRXRADDR_SRADDLVLD_SHIFT                       (24U)
#define I2C_MRXRADDR_SRADDLVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDLVLD_SHIFT)                       /* 0x01000000 */
#define I2C_MRXRADDR_SRADDMVLD_SHIFT                       (25U)
#define I2C_MRXRADDR_SRADDMVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDMVLD_SHIFT)                       /* 0x02000000 */
#define I2C_MRXRADDR_SRADDHVLD_SHIFT                       (26U)
#define I2C_MRXRADDR_SRADDHVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDHVLD_SHIFT)                       /* 0x04000000 */
/* MTXCNT */
#define I2C_MTXCNT_OFFSET                                  (0x10)
#define I2C_MTXCNT_MTXCNT_SHIFT                            (0U)
#define I2C_MTXCNT_MTXCNT_MASK                             (0x3FU << I2C_MTXCNT_MTXCNT_SHIFT)                           /* 0x0000003F */
/* MRXCNT */
#define I2C_MRXCNT_OFFSET                                  (0x14)
#define I2C_MRXCNT_MRXCNT_SHIFT                            (0U)
#define I2C_MRXCNT_MRXCNT_MASK                             (0x3FU << I2C_MRXCNT_MRXCNT_SHIFT)                           /* 0x0000003F */
/* IEN */
#define I2C_IEN_OFFSET                                     (0x18)
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
#define I2C_IPD_OFFSET                                     (0x1C)
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
#define I2C_FCNT_OFFSET                                    (0x20)
#define I2C_FCNT_FCNT_SHIFT                                (0U)
#define I2C_FCNT_FCNT_MASK                                 (0x3FU << I2C_FCNT_FCNT_SHIFT)                               /* 0x0000003F */
/* SCL_OE_DB */
#define I2C_SCL_OE_DB_OFFSET                               (0x24)
#define I2C_SCL_OE_DB_SCL_OE_DB_SHIFT                      (0U)
#define I2C_SCL_OE_DB_SCL_OE_DB_MASK                       (0xFFU << I2C_SCL_OE_DB_SCL_OE_DB_SHIFT)                     /* 0x000000FF */
/* TXDATA0 */
#define I2C_TXDATA0_OFFSET                                 (0x100)
#define I2C_TXDATA0_TXDATA0_SHIFT                          (0U)
#define I2C_TXDATA0_TXDATA0_MASK                           (0xFFFFFFFFU << I2C_TXDATA0_TXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA1 */
#define I2C_TXDATA1_OFFSET                                 (0x104)
#define I2C_TXDATA1_TXDATA1_SHIFT                          (0U)
#define I2C_TXDATA1_TXDATA1_MASK                           (0xFFFFFFFFU << I2C_TXDATA1_TXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA2 */
#define I2C_TXDATA2_OFFSET                                 (0x108)
#define I2C_TXDATA2_TXDATA2_SHIFT                          (0U)
#define I2C_TXDATA2_TXDATA2_MASK                           (0xFFFFFFFFU << I2C_TXDATA2_TXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA3 */
#define I2C_TXDATA3_OFFSET                                 (0x10C)
#define I2C_TXDATA3_TXDATA3_SHIFT                          (0U)
#define I2C_TXDATA3_TXDATA3_MASK                           (0xFFFFFFFFU << I2C_TXDATA3_TXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA4 */
#define I2C_TXDATA4_OFFSET                                 (0x110)
#define I2C_TXDATA4_TXDATA4_SHIFT                          (0U)
#define I2C_TXDATA4_TXDATA4_MASK                           (0xFFFFFFFFU << I2C_TXDATA4_TXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA5 */
#define I2C_TXDATA5_OFFSET                                 (0x114)
#define I2C_TXDATA5_TXDATA5_SHIFT                          (0U)
#define I2C_TXDATA5_TXDATA5_MASK                           (0xFFFFFFFFU << I2C_TXDATA5_TXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA6 */
#define I2C_TXDATA6_OFFSET                                 (0x118)
#define I2C_TXDATA6_TXDATA6_SHIFT                          (0U)
#define I2C_TXDATA6_TXDATA6_MASK                           (0xFFFFFFFFU << I2C_TXDATA6_TXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA7 */
#define I2C_TXDATA7_OFFSET                                 (0x11C)
#define I2C_TXDATA7_TXDATA7_SHIFT                          (0U)
#define I2C_TXDATA7_TXDATA7_MASK                           (0xFFFFFFFFU << I2C_TXDATA7_TXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA0 */
#define I2C_RXDATA0_OFFSET                                 (0x200)
#define I2C_RXDATA0_RXDATA0_SHIFT                          (0U)
#define I2C_RXDATA0_RXDATA0_MASK                           (0xFFFFFFFFU << I2C_RXDATA0_RXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA1 */
#define I2C_RXDATA1_OFFSET                                 (0x204)
#define I2C_RXDATA1_RXDATA1_SHIFT                          (0U)
#define I2C_RXDATA1_RXDATA1_MASK                           (0xFFFFFFFFU << I2C_RXDATA1_RXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA2 */
#define I2C_RXDATA2_OFFSET                                 (0x208)
#define I2C_RXDATA2_RXDATA2_SHIFT                          (0U)
#define I2C_RXDATA2_RXDATA2_MASK                           (0xFFFFFFFFU << I2C_RXDATA2_RXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA3 */
#define I2C_RXDATA3_OFFSET                                 (0x20C)
#define I2C_RXDATA3_RXDATA3_SHIFT                          (0U)
#define I2C_RXDATA3_RXDATA3_MASK                           (0xFFFFFFFFU << I2C_RXDATA3_RXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA4 */
#define I2C_RXDATA4_OFFSET                                 (0x210)
#define I2C_RXDATA4_RXDATA4_SHIFT                          (0U)
#define I2C_RXDATA4_RXDATA4_MASK                           (0xFFFFFFFFU << I2C_RXDATA4_RXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA5 */
#define I2C_RXDATA5_OFFSET                                 (0x214)
#define I2C_RXDATA5_RXDATA5_SHIFT                          (0U)
#define I2C_RXDATA5_RXDATA5_MASK                           (0xFFFFFFFFU << I2C_RXDATA5_RXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA6 */
#define I2C_RXDATA6_OFFSET                                 (0x218)
#define I2C_RXDATA6_RXDATA6_SHIFT                          (0U)
#define I2C_RXDATA6_RXDATA6_MASK                           (0xFFFFFFFFU << I2C_RXDATA6_RXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA7 */
#define I2C_RXDATA7_OFFSET                                 (0x21C)
#define I2C_RXDATA7_RXDATA7_SHIFT                          (0U)
#define I2C_RXDATA7_RXDATA7_MASK                           (0xFFFFFFFFU << I2C_RXDATA7_RXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* ST */
#define I2C_ST_OFFSET                                      (0x220)
#define I2C_ST_SDA_ST_SHIFT                                (0U)
#define I2C_ST_SDA_ST_MASK                                 (0x1U << I2C_ST_SDA_ST_SHIFT)                                /* 0x00000001 */
#define I2C_ST_SCL_ST_SHIFT                                (1U)
#define I2C_ST_SCL_ST_MASK                                 (0x1U << I2C_ST_SCL_ST_SHIFT)                                /* 0x00000002 */
/* DBGCTRL */
#define I2C_DBGCTRL_OFFSET                                 (0x224)
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
/******************************************PWM*******************************************/
/* PWM0_CNT */
#define PWM_PWM0_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM0_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM0_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM0_PERIOD_HPR */
#define PWM_PWM0_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM0_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM0_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
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
#define PWM_PWM1_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM1_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM1_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM1_PERIOD_HPR */
#define PWM_PWM1_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM1_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM1_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
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
#define PWM_PWM2_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM2_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM2_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM2_PERIOD_HPR */
#define PWM_PWM2_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM2_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM2_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
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
#define PWM_PWM3_CNT_CNT_SHIFT                             (0U)
#define PWM_PWM3_CNT_CNT_MASK                              (0xFFFFFFFFU << PWM_PWM3_CNT_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* PWM3_PERIOD_HPR */
#define PWM_PWM3_PERIOD_HPR_PERIOD_HPR_SHIFT               (0U)
#define PWM_PWM3_PERIOD_HPR_PERIOD_HPR_MASK                (0xFFFFFFFFU << PWM_PWM3_PERIOD_HPR_PERIOD_HPR_SHIFT)        /* 0xFFFFFFFF */
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
#define PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_SHIFT           (0U)
#define PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_MASK            (0xFFFFFU << PWM_FIFO_TOUTTHR_TIMEOUT_THRESHOLD_SHIFT)       /* 0x000FFFFF */
/* VERSION_ID */
#define PWM_VERSION_ID_SVN_VERSION_SHIFT                   (0U)
#define PWM_VERSION_ID_SVN_VERSION_MASK                    (0xFFFFU << PWM_VERSION_ID_SVN_VERSION_SHIFT)                /* 0x0000FFFF */
#define PWM_VERSION_ID_MINOR_VERSION_SHIFT                 (16U)
#define PWM_VERSION_ID_MINOR_VERSION_MASK                  (0xFFU << PWM_VERSION_ID_MINOR_VERSION_SHIFT)                /* 0x00FF0000 */
#define PWM_VERSION_ID_MAIN_VERSION_SHIFT                  (24U)
#define PWM_VERSION_ID_MAIN_VERSION_MASK                   (0xFFU << PWM_VERSION_ID_MAIN_VERSION_SHIFT)                 /* 0xFF000000 */
/* FIFO */
#define PWM_FIFO_CYCLE_CNT_SHIFT                           (0U)
#define PWM_FIFO_CYCLE_CNT_MASK                            (0x7FFFFFFFU << PWM_FIFO_CYCLE_CNT_SHIFT)                    /* 0x7FFFFFFF */
#define PWM_FIFO_POL_SHIFT                                 (31U)
#define PWM_FIFO_POL_MASK                                  (0x1U << PWM_FIFO_POL_SHIFT)                                 /* 0x80000000 */
/* PWRMATCH_CTRL */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_SHIFT          (3U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_MASK           (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_SHIFT)          /* 0x00000008 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_SHIFT        (7U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_SHIFT)        /* 0x00000080 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_SHIFT    (11U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_MASK     (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_SHIFT)    /* 0x00000800 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_SHIFT        (15U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_SHIFT)        /* 0x00008000 */
/* PWRMATCH_LPRE */
#define PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT                    (0U)
#define PWM_PWRMATCH_LPRE_CNT_MIN_MASK                     (0xFFFFU << PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT)                 /* 0x0000FFFF */
#define PWM_PWRMATCH_LPRE_CNT_MAX_SHIFT                    (16U)
#define PWM_PWRMATCH_LPRE_CNT_MAX_MASK                     (0xFFFFU << PWM_PWRMATCH_LPRE_CNT_MAX_SHIFT)                 /* 0xFFFF0000 */
/* PWRMATCH_HPRE */
#define PWM_PWRMATCH_HPRE_CNT_MIN_SHIFT                    (0U)
#define PWM_PWRMATCH_HPRE_CNT_MIN_MASK                     (0xFFFFU << PWM_PWRMATCH_HPRE_CNT_MIN_SHIFT)                 /* 0x0000FFFF */
#define PWM_PWRMATCH_HPRE_CNT_MAX_SHIFT                    (16U)
#define PWM_PWRMATCH_HPRE_CNT_MAX_MASK                     (0xFFFFU << PWM_PWRMATCH_HPRE_CNT_MAX_SHIFT)                 /* 0xFFFF0000 */
/* PWRMATCH_LD */
#define PWM_PWRMATCH_LD_CNT_MIN_SHIFT                      (0U)
#define PWM_PWRMATCH_LD_CNT_MIN_MASK                       (0xFFFFU << PWM_PWRMATCH_LD_CNT_MIN_SHIFT)                   /* 0x0000FFFF */
#define PWM_PWRMATCH_LD_CNT_MAX_SHIFT                      (16U)
#define PWM_PWRMATCH_LD_CNT_MAX_MASK                       (0xFFFFU << PWM_PWRMATCH_LD_CNT_MAX_SHIFT)                   /* 0xFFFF0000 */
/* PWRMATCH_HD_ZERO */
#define PWM_PWRMATCH_HD_ZERO_CNT_MIN_SHIFT                 (0U)
#define PWM_PWRMATCH_HD_ZERO_CNT_MIN_MASK                  (0xFFFFU << PWM_PWRMATCH_HD_ZERO_CNT_MIN_SHIFT)              /* 0x0000FFFF */
#define PWM_PWRMATCH_HD_ZERO_CNT_MAX_SHIFT                 (16U)
#define PWM_PWRMATCH_HD_ZERO_CNT_MAX_MASK                  (0xFFFFU << PWM_PWRMATCH_HD_ZERO_CNT_MAX_SHIFT)              /* 0xFFFF0000 */
/* PWRMATCH_HD_ONE */
#define PWM_PWRMATCH_HD_ONE_CNT_MIN_SHIFT                  (0U)
#define PWM_PWRMATCH_HD_ONE_CNT_MIN_MASK                   (0xFFFFU << PWM_PWRMATCH_HD_ONE_CNT_MIN_SHIFT)               /* 0x0000FFFF */
#define PWM_PWRMATCH_HD_ONE_CNT_MAX_SHIFT                  (16U)
#define PWM_PWRMATCH_HD_ONE_CNT_MAX_MASK                   (0xFFFFU << PWM_PWRMATCH_HD_ONE_CNT_MAX_SHIFT)               /* 0xFFFF0000 */
/* PWRMATCH_VALUE0 */
#define PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE1 */
#define PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE2 */
#define PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE3 */
#define PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE4 */
#define PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE5 */
#define PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE6 */
#define PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE7 */
#define PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE8 */
#define PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE9 */
#define PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE_SHIFT       (0U)
#define PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE_MASK        (0xFFFFFFFFU << PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWM3_PWRCAPTURE_VALUE */
#define PWM_PWM3_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT (0U)
#define PWM_PWM3_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_MASK (0xFFFFFFFFU << PWM_PWM3_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT) /* 0xFFFFFFFF */
/* FILTER_CTRL */
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
/*****************************************EFUSE******************************************/
/* MOD */
#define EFUSE_MOD_OFFSET                                   (0x0)
#define EFUSE_MOD_WORK_MOD_SHIFT                           (0U)
#define EFUSE_MOD_WORK_MOD_MASK                            (0x1U << EFUSE_MOD_WORK_MOD_SHIFT)                           /* 0x00000001 */
#define EFUSE_MOD_CSB_POL_SHIFT                            (1U)
#define EFUSE_MOD_CSB_POL_MASK                             (0x1U << EFUSE_MOD_CSB_POL_SHIFT)                            /* 0x00000002 */
#define EFUSE_MOD_PGENB_POL_SHIFT                          (2U)
#define EFUSE_MOD_PGENB_POL_MASK                           (0x1U << EFUSE_MOD_PGENB_POL_SHIFT)                          /* 0x00000004 */
#define EFUSE_MOD_LOAD_POL_SHIFT                           (3U)
#define EFUSE_MOD_LOAD_POL_MASK                            (0x1U << EFUSE_MOD_LOAD_POL_SHIFT)                           /* 0x00000008 */
#define EFUSE_MOD_STROBE_POL_SHIFT                         (4U)
#define EFUSE_MOD_STROBE_POL_MASK                          (0x1U << EFUSE_MOD_STROBE_POL_SHIFT)                         /* 0x00000010 */
#define EFUSE_MOD_PG_EN_USER_SHIFT                         (5U)
#define EFUSE_MOD_PG_EN_USER_MASK                          (0x1U << EFUSE_MOD_PG_EN_USER_SHIFT)                         /* 0x00000020 */
#define EFUSE_MOD_RD_ENB_USER_SHIFT                        (6U)
#define EFUSE_MOD_RD_ENB_USER_MASK                         (0x1U << EFUSE_MOD_RD_ENB_USER_SHIFT)                        /* 0x00000040 */
/* RD_MASK */
#define EFUSE_RD_MASK_OFFSET                               (0x4)
#define EFUSE_RD_MASK_RD_MASK_SHIFT                        (0U)
#define EFUSE_RD_MASK_RD_MASK_MASK                         (0xFFFFFFFFU << EFUSE_RD_MASK_RD_MASK_SHIFT)                 /* 0xFFFFFFFF */
/* PG_MASK */
#define EFUSE_PG_MASK_OFFSET                               (0x8)
#define EFUSE_PG_MASK_PG_MASK_SHIFT                        (0U)
#define EFUSE_PG_MASK_PG_MASK_MASK                         (0xFFFFFFFFU << EFUSE_PG_MASK_PG_MASK_SHIFT)                 /* 0xFFFFFFFF */
/* INT_CON */
#define EFUSE_INT_CON_OFFSET                               (0x14)
#define EFUSE_INT_CON_FINISH_INT_EN_SHIFT                  (0U)
#define EFUSE_INT_CON_FINISH_INT_EN_MASK                   (0x1U << EFUSE_INT_CON_FINISH_INT_EN_SHIFT)                  /* 0x00000001 */
#define EFUSE_INT_CON_AUTO_RD_MASK_INT_EN_SHIFT            (7U)
#define EFUSE_INT_CON_AUTO_RD_MASK_INT_EN_MASK             (0x1U << EFUSE_INT_CON_AUTO_RD_MASK_INT_EN_SHIFT)            /* 0x00000080 */
#define EFUSE_INT_CON_AUTO_PG_MASK_INT_EN_SHIFT            (8U)
#define EFUSE_INT_CON_AUTO_PG_MASK_INT_EN_MASK             (0x1U << EFUSE_INT_CON_AUTO_PG_MASK_INT_EN_SHIFT)            /* 0x00000100 */
#define EFUSE_INT_CON_USER_RD_MASK_INT_EN_SHIFT            (11U)
#define EFUSE_INT_CON_USER_RD_MASK_INT_EN_MASK             (0x1U << EFUSE_INT_CON_USER_RD_MASK_INT_EN_SHIFT)            /* 0x00000800 */
#define EFUSE_INT_CON_USER_PG_MASK_INT_EN_SHIFT            (12U)
#define EFUSE_INT_CON_USER_PG_MASK_INT_EN_MASK             (0x1U << EFUSE_INT_CON_USER_PG_MASK_INT_EN_SHIFT)            /* 0x00001000 */
/* INT_STATUS */
#define EFUSE_INT_STATUS_OFFSET                            (0x18)
#define EFUSE_INT_STATUS_FINISH_INT_SHIFT                  (0U)
#define EFUSE_INT_STATUS_FINISH_INT_MASK                   (0x1U << EFUSE_INT_STATUS_FINISH_INT_SHIFT)                  /* 0x00000001 */
#define EFUSE_INT_STATUS_AUTO_RD_MASK_INT_SHIFT            (7U)
#define EFUSE_INT_STATUS_AUTO_RD_MASK_INT_MASK             (0x1U << EFUSE_INT_STATUS_AUTO_RD_MASK_INT_SHIFT)            /* 0x00000080 */
#define EFUSE_INT_STATUS_AUTO_PG_MASK_INT_SHIFT            (8U)
#define EFUSE_INT_STATUS_AUTO_PG_MASK_INT_MASK             (0x1U << EFUSE_INT_STATUS_AUTO_PG_MASK_INT_SHIFT)            /* 0x00000100 */
#define EFUSE_INT_STATUS_USER_RD_MASK_INT_SHIFT            (11U)
#define EFUSE_INT_STATUS_USER_RD_MASK_INT_MASK             (0x1U << EFUSE_INT_STATUS_USER_RD_MASK_INT_SHIFT)            /* 0x00000800 */
#define EFUSE_INT_STATUS_USER_PG_MASK_INT_SHIFT            (12U)
#define EFUSE_INT_STATUS_USER_PG_MASK_INT_MASK             (0x1U << EFUSE_INT_STATUS_USER_PG_MASK_INT_SHIFT)            /* 0x00001000 */
/* USER_CTRL */
#define EFUSE_USER_CTRL_OFFSET                             (0x1C)
#define EFUSE_USER_CTRL_CSB_USER_SHIFT                     (0U)
#define EFUSE_USER_CTRL_CSB_USER_MASK                      (0x1U << EFUSE_USER_CTRL_CSB_USER_SHIFT)                     /* 0x00000001 */
#define EFUSE_USER_CTRL_STROBE_USER_SHIFT                  (1U)
#define EFUSE_USER_CTRL_STROBE_USER_MASK                   (0x1U << EFUSE_USER_CTRL_STROBE_USER_SHIFT)                  /* 0x00000002 */
#define EFUSE_USER_CTRL_LOAD_USER_SHIFT                    (2U)
#define EFUSE_USER_CTRL_LOAD_USER_MASK                     (0x1U << EFUSE_USER_CTRL_LOAD_USER_SHIFT)                    /* 0x00000004 */
#define EFUSE_USER_CTRL_PGENB_USER_SHIFT                   (3U)
#define EFUSE_USER_CTRL_PGENB_USER_MASK                    (0x1U << EFUSE_USER_CTRL_PGENB_USER_SHIFT)                   /* 0x00000008 */
#define EFUSE_USER_CTRL_MR1_SHIFT                          (4U)
#define EFUSE_USER_CTRL_MR1_MASK                           (0x1U << EFUSE_USER_CTRL_MR1_SHIFT)                          /* 0x00000010 */
#define EFUSE_USER_CTRL_MR0_SHIFT                          (5U)
#define EFUSE_USER_CTRL_MR0_MASK                           (0x1U << EFUSE_USER_CTRL_MR0_SHIFT)                          /* 0x00000020 */
#define EFUSE_USER_CTRL_ADDR_USER_SHIFT                    (16U)
#define EFUSE_USER_CTRL_ADDR_USER_MASK                     (0x3FFU << EFUSE_USER_CTRL_ADDR_USER_SHIFT)                  /* 0x03FF0000 */
/* DOUT */
#define EFUSE_DOUT_OFFSET                                  (0x20)
#define EFUSE_DOUT_DOUT_SHIFT                              (0U)
#define EFUSE_DOUT_DOUT_MASK                               (0xFFFFFFFFU << EFUSE_DOUT_DOUT_SHIFT)                       /* 0xFFFFFFFF */
/* AUTO_CTRL */
#define EFUSE_AUTO_CTRL_OFFSET                             (0x24)
#define EFUSE_AUTO_CTRL_ENB_SHIFT                          (0U)
#define EFUSE_AUTO_CTRL_ENB_MASK                           (0x1U << EFUSE_AUTO_CTRL_ENB_SHIFT)                          /* 0x00000001 */
#define EFUSE_AUTO_CTRL_PG_R_SHIFT                         (1U)
#define EFUSE_AUTO_CTRL_PG_R_MASK                          (0x1U << EFUSE_AUTO_CTRL_PG_R_SHIFT)                         /* 0x00000002 */
#define EFUSE_AUTO_CTRL_ADDR_AUTO_SHIFT                    (16U)
#define EFUSE_AUTO_CTRL_ADDR_AUTO_MASK                     (0x3FFU << EFUSE_AUTO_CTRL_ADDR_AUTO_SHIFT)                  /* 0x03FF0000 */
/* T_CSB_P */
#define EFUSE_T_CSB_P_OFFSET                               (0x28)
#define EFUSE_T_CSB_P_T_CSB_P_L_SHIFT                      (0U)
#define EFUSE_T_CSB_P_T_CSB_P_L_MASK                       (0x7FFU << EFUSE_T_CSB_P_T_CSB_P_L_SHIFT)                    /* 0x000007FF */
#define EFUSE_T_CSB_P_T_CSB_P_S_SHIFT                      (16U)
#define EFUSE_T_CSB_P_T_CSB_P_S_MASK                       (0xFU << EFUSE_T_CSB_P_T_CSB_P_S_SHIFT)                      /* 0x000F0000 */
/* T_PGENB_P */
#define EFUSE_T_PGENB_P_OFFSET                             (0x2C)
#define EFUSE_T_PGENB_P_T_PGENB_P_L_SHIFT                  (0U)
#define EFUSE_T_PGENB_P_T_PGENB_P_L_MASK                   (0x7FFU << EFUSE_T_PGENB_P_T_PGENB_P_L_SHIFT)                /* 0x000007FF */
#define EFUSE_T_PGENB_P_T_PGENB_P_S_SHIFT                  (16U)
#define EFUSE_T_PGENB_P_T_PGENB_P_S_MASK                   (0xFU << EFUSE_T_PGENB_P_T_PGENB_P_S_SHIFT)                  /* 0x000F0000 */
/* T_LOAD_P */
#define EFUSE_T_LOAD_P_OFFSET                              (0x30)
#define EFUSE_T_LOAD_P_T_LOAD_P_L_SHIFT                    (0U)
#define EFUSE_T_LOAD_P_T_LOAD_P_L_MASK                     (0x7FFU << EFUSE_T_LOAD_P_T_LOAD_P_L_SHIFT)                  /* 0x000007FF */
#define EFUSE_T_LOAD_P_T_LOAD_P_S_SHIFT                    (16U)
#define EFUSE_T_LOAD_P_T_LOAD_P_S_MASK                     (0xFU << EFUSE_T_LOAD_P_T_LOAD_P_S_SHIFT)                    /* 0x000F0000 */
/* T_ADDR_P */
#define EFUSE_T_ADDR_P_OFFSET                              (0x34)
#define EFUSE_T_ADDR_P_T_ADDR_P_L_SHIFT                    (0U)
#define EFUSE_T_ADDR_P_T_ADDR_P_L_MASK                     (0x7FFU << EFUSE_T_ADDR_P_T_ADDR_P_L_SHIFT)                  /* 0x000007FF */
#define EFUSE_T_ADDR_P_T_ADDR_P_S_SHIFT                    (16U)
#define EFUSE_T_ADDR_P_T_ADDR_P_S_MASK                     (0xFU << EFUSE_T_ADDR_P_T_ADDR_P_S_SHIFT)                    /* 0x000F0000 */
/* T_STROBE_P */
#define EFUSE_T_STROBE_P_OFFSET                            (0x38)
#define EFUSE_T_STROBE_P_T_STROBE_P_L_SHIFT                (0U)
#define EFUSE_T_STROBE_P_T_STROBE_P_L_MASK                 (0x7FFU << EFUSE_T_STROBE_P_T_STROBE_P_L_SHIFT)              /* 0x000007FF */
#define EFUSE_T_STROBE_P_T_STROBE_P_S_SHIFT                (16U)
#define EFUSE_T_STROBE_P_T_STROBE_P_S_MASK                 (0xFFU << EFUSE_T_STROBE_P_T_STROBE_P_S_SHIFT)               /* 0x00FF0000 */
/* T_CSB_R */
#define EFUSE_T_CSB_R_OFFSET                               (0x3C)
#define EFUSE_T_CSB_R_T_CSB_R_L_SHIFT                      (0U)
#define EFUSE_T_CSB_R_T_CSB_R_L_MASK                       (0x3FFU << EFUSE_T_CSB_R_T_CSB_R_L_SHIFT)                    /* 0x000003FF */
#define EFUSE_T_CSB_R_T_CSB_R_S_SHIFT                      (16U)
#define EFUSE_T_CSB_R_T_CSB_R_S_MASK                       (0xFU << EFUSE_T_CSB_R_T_CSB_R_S_SHIFT)                      /* 0x000F0000 */
/* T_PGENB_R */
#define EFUSE_T_PGENB_R_OFFSET                             (0x40)
#define EFUSE_T_PGENB_R_T_PGENB_R_L_SHIFT                  (0U)
#define EFUSE_T_PGENB_R_T_PGENB_R_L_MASK                   (0x3FFU << EFUSE_T_PGENB_R_T_PGENB_R_L_SHIFT)                /* 0x000003FF */
#define EFUSE_T_PGENB_R_T_PGENB_R_S_SHIFT                  (16U)
#define EFUSE_T_PGENB_R_T_PGENB_R_S_MASK                   (0xFU << EFUSE_T_PGENB_R_T_PGENB_R_S_SHIFT)                  /* 0x000F0000 */
/* T_LOAD_R */
#define EFUSE_T_LOAD_R_OFFSET                              (0x44)
#define EFUSE_T_LOAD_R_T_LOAD_R_L_SHIFT                    (0U)
#define EFUSE_T_LOAD_R_T_LOAD_R_L_MASK                     (0x3FFU << EFUSE_T_LOAD_R_T_LOAD_R_L_SHIFT)                  /* 0x000003FF */
#define EFUSE_T_LOAD_R_T_LOAD_R_S_SHIFT                    (16U)
#define EFUSE_T_LOAD_R_T_LOAD_R_S_MASK                     (0xFU << EFUSE_T_LOAD_R_T_LOAD_R_S_SHIFT)                    /* 0x000F0000 */
/* T_ADDR_R */
#define EFUSE_T_ADDR_R_OFFSET                              (0x48)
#define EFUSE_T_ADDR_R_T_ADDR_R_L_SHIFT                    (0U)
#define EFUSE_T_ADDR_R_T_ADDR_R_L_MASK                     (0x3FFU << EFUSE_T_ADDR_R_T_ADDR_R_L_SHIFT)                  /* 0x000003FF */
#define EFUSE_T_ADDR_R_T_ADDR_R_S_SHIFT                    (16U)
#define EFUSE_T_ADDR_R_T_ADDR_R_S_MASK                     (0xFU << EFUSE_T_ADDR_R_T_ADDR_R_S_SHIFT)                    /* 0x000F0000 */
/* T_STROBE_R */
#define EFUSE_T_STROBE_R_OFFSET                            (0x4C)
#define EFUSE_T_STROBE_R_T_STROBE_R_L_SHIFT                (0U)
#define EFUSE_T_STROBE_R_T_STROBE_R_L_MASK                 (0x3FFU << EFUSE_T_STROBE_R_T_STROBE_R_L_SHIFT)              /* 0x000003FF */
#define EFUSE_T_STROBE_R_T_STROBE_R_S_SHIFT                (16U)
#define EFUSE_T_STROBE_R_T_STROBE_R_S_MASK                 (0xFU << EFUSE_T_STROBE_R_T_STROBE_R_S_SHIFT)                /* 0x000F0000 */
/* REVISION */
#define EFUSE_REVISION_OFFSET                              (0x50)
#define EFUSE_REVISION_REVISION_SHIFT                      (0U)
#define EFUSE_REVISION_REVISION_MASK                       (0xFFU << EFUSE_REVISION_REVISION_SHIFT)                     /* 0x000000FF */
/******************************************DMA*******************************************/
/* DSR */
#define DMA_DSR_OFFSET                                     (0x0)
#define DMA_DSR_FIELD0000_SHIFT                            (0U)
#define DMA_DSR_FIELD0000_MASK                             (0xFU << DMA_DSR_FIELD0000_SHIFT)                            /* 0x0000000F */
#define DMA_DSR_FIELD0002_SHIFT                            (4U)
#define DMA_DSR_FIELD0002_MASK                             (0x1FU << DMA_DSR_FIELD0002_SHIFT)                           /* 0x000001F0 */
#define DMA_DSR_FIELD0001_SHIFT                            (9U)
#define DMA_DSR_FIELD0001_MASK                             (0x1U << DMA_DSR_FIELD0001_SHIFT)                            /* 0x00000200 */
/* DPC */
#define DMA_DPC_OFFSET                                     (0x4)
#define DMA_DPC_FIELD0000_SHIFT                            (0U)
#define DMA_DPC_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_DPC_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* INTEN */
#define DMA_INTEN_OFFSET                                   (0x20)
#define DMA_INTEN_FIELD0000_SHIFT                          (0U)
#define DMA_INTEN_FIELD0000_MASK                           (0xFFFFFFFFU << DMA_INTEN_FIELD0000_SHIFT)                   /* 0xFFFFFFFF */
/* EVENT_RIS */
#define DMA_EVENT_RIS_OFFSET                               (0x24)
#define DMA_EVENT_RIS_FIELD0000_SHIFT                      (0U)
#define DMA_EVENT_RIS_FIELD0000_MASK                       (0xFFFFFFFFU << DMA_EVENT_RIS_FIELD0000_SHIFT)               /* 0xFFFFFFFF */
/* INTMIS */
#define DMA_INTMIS_OFFSET                                  (0x28)
#define DMA_INTMIS_FIELD0000_SHIFT                         (0U)
#define DMA_INTMIS_FIELD0000_MASK                          (0xFFFFFFFFU << DMA_INTMIS_FIELD0000_SHIFT)                  /* 0xFFFFFFFF */
/* INTCLR */
#define DMA_INTCLR_OFFSET                                  (0x2C)
#define DMA_INTCLR_FIELD0000_SHIFT                         (0U)
#define DMA_INTCLR_FIELD0000_MASK                          (0xFFFFFFFFU << DMA_INTCLR_FIELD0000_SHIFT)                  /* 0xFFFFFFFF */
/* FSRD */
#define DMA_FSRD_OFFSET                                    (0x30)
#define DMA_FSRD_FIELD0000_SHIFT                           (0U)
#define DMA_FSRD_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_FSRD_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* FSRC */
#define DMA_FSRC_OFFSET                                    (0x34)
#define DMA_FSRC_FIELD0000_SHIFT                           (0U)
#define DMA_FSRC_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_FSRC_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* FTRD */
#define DMA_FTRD_OFFSET                                    (0x38)
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
#define DMA_FTR0_OFFSET                                    (0x40)
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
#define DMA_FTR1_OFFSET                                    (0x44)
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
#define DMA_FTR2_OFFSET                                    (0x48)
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
#define DMA_FTR3_OFFSET                                    (0x4C)
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
#define DMA_FTR4_OFFSET                                    (0x50)
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
#define DMA_FTR5_OFFSET                                    (0x54)
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
#define DMA_CSR0_OFFSET                                    (0x100)
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
#define DMA_CPC0_OFFSET                                    (0x104)
#define DMA_CPC0_FIELD0000_SHIFT                           (0U)
#define DMA_CPC0_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC0_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR1 */
#define DMA_CSR1_OFFSET                                    (0x108)
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
#define DMA_CPC1_OFFSET                                    (0x10C)
#define DMA_CPC1_FIELD0000_SHIFT                           (0U)
#define DMA_CPC1_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC1_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR2 */
#define DMA_CSR2_OFFSET                                    (0x110)
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
#define DMA_CPC2_OFFSET                                    (0x114)
#define DMA_CPC2_FIELD0000_SHIFT                           (0U)
#define DMA_CPC2_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC2_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR3 */
#define DMA_CSR3_OFFSET                                    (0x118)
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
#define DMA_CPC3_OFFSET                                    (0x11C)
#define DMA_CPC3_FIELD0000_SHIFT                           (0U)
#define DMA_CPC3_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC3_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR4 */
#define DMA_CSR4_OFFSET                                    (0x120)
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
#define DMA_CPC4_OFFSET                                    (0x124)
#define DMA_CPC4_FIELD0000_SHIFT                           (0U)
#define DMA_CPC4_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC4_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CSR5 */
#define DMA_CSR5_OFFSET                                    (0x128)
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
#define DMA_CPC5_OFFSET                                    (0x12C)
#define DMA_CPC5_FIELD0000_SHIFT                           (0U)
#define DMA_CPC5_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_CPC5_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* SAR0 */
#define DMA_SAR0_OFFSET                                    (0x400)
#define DMA_SAR0_FIELD0000_SHIFT                           (0U)
#define DMA_SAR0_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR0_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR0 */
#define DMA_DAR0_OFFSET                                    (0x404)
#define DMA_DAR0_FIELD0000_SHIFT                           (0U)
#define DMA_DAR0_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR0_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR0 */
#define DMA_CCR0_OFFSET                                    (0x408)
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
#define DMA_LC0_0_OFFSET                                   (0x40C)
#define DMA_LC0_0_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_0_FIELD0000_MASK                           (0xFFU << DMA_LC0_0_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_0 */
#define DMA_LC1_0_OFFSET                                   (0x410)
#define DMA_LC1_0_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_0_FIELD0000_MASK                           (0xFFU << DMA_LC1_0_FIELD0000_SHIFT)                         /* 0x000000FF */
/* SAR1 */
#define DMA_SAR1_OFFSET                                    (0x420)
#define DMA_SAR1_FIELD0000_SHIFT                           (0U)
#define DMA_SAR1_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR1_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR1 */
#define DMA_DAR1_OFFSET                                    (0x424)
#define DMA_DAR1_FIELD0000_SHIFT                           (0U)
#define DMA_DAR1_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR1_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR1 */
#define DMA_CCR1_OFFSET                                    (0x428)
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
#define DMA_LC0_1_OFFSET                                   (0x42C)
#define DMA_LC0_1_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_1_FIELD0000_MASK                           (0xFFU << DMA_LC0_1_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_1 */
#define DMA_LC1_1_OFFSET                                   (0x430)
#define DMA_LC1_1_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_1_FIELD0000_MASK                           (0xFFU << DMA_LC1_1_FIELD0000_SHIFT)                         /* 0x000000FF */
/* SAR2 */
#define DMA_SAR2_OFFSET                                    (0x440)
#define DMA_SAR2_FIELD0000_SHIFT                           (0U)
#define DMA_SAR2_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR2_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR2 */
#define DMA_DAR2_OFFSET                                    (0x444)
#define DMA_DAR2_FIELD0000_SHIFT                           (0U)
#define DMA_DAR2_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR2_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR2 */
#define DMA_CCR2_OFFSET                                    (0x448)
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
#define DMA_LC0_2_OFFSET                                   (0x44C)
#define DMA_LC0_2_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_2_FIELD0000_MASK                           (0xFFU << DMA_LC0_2_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_2 */
#define DMA_LC1_2_OFFSET                                   (0x450)
#define DMA_LC1_2_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_2_FIELD0000_MASK                           (0xFFU << DMA_LC1_2_FIELD0000_SHIFT)                         /* 0x000000FF */
/* SAR3 */
#define DMA_SAR3_OFFSET                                    (0x460)
#define DMA_SAR3_FIELD0000_SHIFT                           (0U)
#define DMA_SAR3_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR3_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR3 */
#define DMA_DAR3_OFFSET                                    (0x464)
#define DMA_DAR3_FIELD0000_SHIFT                           (0U)
#define DMA_DAR3_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR3_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR3 */
#define DMA_CCR3_OFFSET                                    (0x468)
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
#define DMA_LC0_3_OFFSET                                   (0x46C)
#define DMA_LC0_3_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_3_FIELD0000_MASK                           (0xFFU << DMA_LC0_3_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_3 */
#define DMA_LC1_3_OFFSET                                   (0x470)
#define DMA_LC1_3_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_3_FIELD0000_MASK                           (0xFFU << DMA_LC1_3_FIELD0000_SHIFT)                         /* 0x000000FF */
/* SAR4 */
#define DMA_SAR4_OFFSET                                    (0x480)
#define DMA_SAR4_FIELD0000_SHIFT                           (0U)
#define DMA_SAR4_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR4_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR4 */
#define DMA_DAR4_OFFSET                                    (0x484)
#define DMA_DAR4_FIELD0000_SHIFT                           (0U)
#define DMA_DAR4_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR4_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR4 */
#define DMA_CCR4_OFFSET                                    (0x488)
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
#define DMA_LC0_4_OFFSET                                   (0x48C)
#define DMA_LC0_4_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_4_FIELD0000_MASK                           (0xFFU << DMA_LC0_4_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_4 */
#define DMA_LC1_4_OFFSET                                   (0x490)
#define DMA_LC1_4_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_4_FIELD0000_MASK                           (0xFFU << DMA_LC1_4_FIELD0000_SHIFT)                         /* 0x000000FF */
/* SAR5 */
#define DMA_SAR5_OFFSET                                    (0x4A0)
#define DMA_SAR5_FIELD0000_SHIFT                           (0U)
#define DMA_SAR5_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_SAR5_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* DAR5 */
#define DMA_DAR5_OFFSET                                    (0x4A4)
#define DMA_DAR5_FIELD0000_SHIFT                           (0U)
#define DMA_DAR5_FIELD0000_MASK                            (0xFFFFFFFFU << DMA_DAR5_FIELD0000_SHIFT)                    /* 0xFFFFFFFF */
/* CCR5 */
#define DMA_CCR5_OFFSET                                    (0x4A8)
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
#define DMA_LC0_5_OFFSET                                   (0x4AC)
#define DMA_LC0_5_FIELD0000_SHIFT                          (0U)
#define DMA_LC0_5_FIELD0000_MASK                           (0xFFU << DMA_LC0_5_FIELD0000_SHIFT)                         /* 0x000000FF */
/* LC1_5 */
#define DMA_LC1_5_OFFSET                                   (0x4B0)
#define DMA_LC1_5_FIELD0000_SHIFT                          (0U)
#define DMA_LC1_5_FIELD0000_MASK                           (0xFFU << DMA_LC1_5_FIELD0000_SHIFT)                         /* 0x000000FF */
/* DBGSTATUS */
#define DMA_DBGSTATUS_OFFSET                               (0xD00)
#define DMA_DBGSTATUS_FIELD0000_SHIFT                      (0U)
#define DMA_DBGSTATUS_FIELD0000_MASK                       (0x3U << DMA_DBGSTATUS_FIELD0000_SHIFT)                      /* 0x00000003 */
/* DBGCMD */
#define DMA_DBGCMD_OFFSET                                  (0xD04)
#define DMA_DBGCMD_FIELD0000_SHIFT                         (0U)
#define DMA_DBGCMD_FIELD0000_MASK                          (0x3U << DMA_DBGCMD_FIELD0000_SHIFT)                         /* 0x00000003 */
/* DBGINST0 */
#define DMA_DBGINST0_OFFSET                                (0xD08)
#define DMA_DBGINST0_FIELD0000_SHIFT                       (0U)
#define DMA_DBGINST0_FIELD0000_MASK                        (0x1U << DMA_DBGINST0_FIELD0000_SHIFT)                       /* 0x00000001 */
#define DMA_DBGINST0_FIELD0003_SHIFT                       (8U)
#define DMA_DBGINST0_FIELD0003_MASK                        (0x7U << DMA_DBGINST0_FIELD0003_SHIFT)                       /* 0x00000700 */
#define DMA_DBGINST0_FIELD0002_SHIFT                       (16U)
#define DMA_DBGINST0_FIELD0002_MASK                        (0xFFU << DMA_DBGINST0_FIELD0002_SHIFT)                      /* 0x00FF0000 */
#define DMA_DBGINST0_FIELD0001_SHIFT                       (24U)
#define DMA_DBGINST0_FIELD0001_MASK                        (0xFFU << DMA_DBGINST0_FIELD0001_SHIFT)                      /* 0xFF000000 */
/* DBGINST1 */
#define DMA_DBGINST1_OFFSET                                (0xD0C)
#define DMA_DBGINST1_FIELD0000_SHIFT                       (0U)
#define DMA_DBGINST1_FIELD0000_MASK                        (0xFFU << DMA_DBGINST1_FIELD0000_SHIFT)                      /* 0x000000FF */
#define DMA_DBGINST1_FIELD0003_SHIFT                       (8U)
#define DMA_DBGINST1_FIELD0003_MASK                        (0xFFU << DMA_DBGINST1_FIELD0003_SHIFT)                      /* 0x0000FF00 */
#define DMA_DBGINST1_FIELD0002_SHIFT                       (16U)
#define DMA_DBGINST1_FIELD0002_MASK                        (0xFFU << DMA_DBGINST1_FIELD0002_SHIFT)                      /* 0x00FF0000 */
#define DMA_DBGINST1_FIELD0001_SHIFT                       (24U)
#define DMA_DBGINST1_FIELD0001_MASK                        (0xFFU << DMA_DBGINST1_FIELD0001_SHIFT)                      /* 0xFF000000 */
/* CR0 */
#define DMA_CR0_OFFSET                                     (0xE00)
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
#define DMA_CR1_OFFSET                                     (0xE04)
#define DMA_CR1_FIELD0000_SHIFT                            (0U)
#define DMA_CR1_FIELD0000_MASK                             (0x7U << DMA_CR1_FIELD0000_SHIFT)                            /* 0x00000007 */
#define DMA_CR1_FIELD0001_SHIFT                            (4U)
#define DMA_CR1_FIELD0001_MASK                             (0xFU << DMA_CR1_FIELD0001_SHIFT)                            /* 0x000000F0 */
/* CR2 */
#define DMA_CR2_OFFSET                                     (0xE08)
#define DMA_CR2_FIELD0000_SHIFT                            (0U)
#define DMA_CR2_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_CR2_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* CR3 */
#define DMA_CR3_OFFSET                                     (0xE0C)
#define DMA_CR3_FIELD0000_SHIFT                            (0U)
#define DMA_CR3_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_CR3_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* CR4 */
#define DMA_CR4_OFFSET                                     (0xE10)
#define DMA_CR4_FIELD0000_SHIFT                            (0U)
#define DMA_CR4_FIELD0000_MASK                             (0xFFFFFFFFU << DMA_CR4_FIELD0000_SHIFT)                     /* 0xFFFFFFFF */
/* CRDN */
#define DMA_CRDN_OFFSET                                    (0xE14)
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
#define DMA_WD_OFFSET                                      (0xE80)
#define DMA_WD_FIELD0000_SHIFT                             (0U)
#define DMA_WD_FIELD0000_MASK                              (0x1U << DMA_WD_FIELD0000_SHIFT)                             /* 0x00000001 */
/******************************************GPIO******************************************/
/* SWPORT_DR_L */
#define GPIO_SWPORT_DR_L_OFFSET                            (0x0)
#define GPIO_SWPORT_DR_L_GPIO_SWPORT_DR_LOW_SHIFT          (0U)
#define GPIO_SWPORT_DR_L_GPIO_SWPORT_DR_LOW_MASK           (0xFFFFU << GPIO_SWPORT_DR_L_GPIO_SWPORT_DR_LOW_SHIFT)       /* 0x0000FFFF */
/* SWPORT_DR_H */
#define GPIO_SWPORT_DR_H_OFFSET                            (0x4)
#define GPIO_SWPORT_DR_H_GPIO_SWPORT_DR_HIGH_SHIFT         (0U)
#define GPIO_SWPORT_DR_H_GPIO_SWPORT_DR_HIGH_MASK          (0xFFFFU << GPIO_SWPORT_DR_H_GPIO_SWPORT_DR_HIGH_SHIFT)      /* 0x0000FFFF */
/* SWPORT_DDR_L */
#define GPIO_SWPORT_DDR_L_OFFSET                           (0x8)
#define GPIO_SWPORT_DDR_L_GPIO_SWPORT_DDR_LOW_SHIFT        (0U)
#define GPIO_SWPORT_DDR_L_GPIO_SWPORT_DDR_LOW_MASK         (0xFFFFU << GPIO_SWPORT_DDR_L_GPIO_SWPORT_DDR_LOW_SHIFT)     /* 0x0000FFFF */
/* SWPORT_DDR_H */
#define GPIO_SWPORT_DDR_H_OFFSET                           (0xC)
#define GPIO_SWPORT_DDR_H_GPIO_SWPORT_DDR_HIGH_SHIFT       (0U)
#define GPIO_SWPORT_DDR_H_GPIO_SWPORT_DDR_HIGH_MASK        (0xFFFFU << GPIO_SWPORT_DDR_H_GPIO_SWPORT_DDR_HIGH_SHIFT)    /* 0x0000FFFF */
/* INT_EN_L */
#define GPIO_INT_EN_L_OFFSET                               (0x10)
#define GPIO_INT_EN_L_GPIO_INT_EN_LOW_SHIFT                (0U)
#define GPIO_INT_EN_L_GPIO_INT_EN_LOW_MASK                 (0xFFFFU << GPIO_INT_EN_L_GPIO_INT_EN_LOW_SHIFT)             /* 0x0000FFFF */
/* INT_EN_H */
#define GPIO_INT_EN_H_OFFSET                               (0x14)
#define GPIO_INT_EN_H_GPIO_INT_EN_HIGH_SHIFT               (0U)
#define GPIO_INT_EN_H_GPIO_INT_EN_HIGH_MASK                (0xFFFFU << GPIO_INT_EN_H_GPIO_INT_EN_HIGH_SHIFT)            /* 0x0000FFFF */
/* INT_MASK_L */
#define GPIO_INT_MASK_L_OFFSET                             (0x18)
#define GPIO_INT_MASK_L_GPIO_INT_MASK_LOW_SHIFT            (0U)
#define GPIO_INT_MASK_L_GPIO_INT_MASK_LOW_MASK             (0xFFFFU << GPIO_INT_MASK_L_GPIO_INT_MASK_LOW_SHIFT)         /* 0x0000FFFF */
/* INT_MASK_H */
#define GPIO_INT_MASK_H_OFFSET                             (0x1C)
#define GPIO_INT_MASK_H_GPIO_INT_MASK_HIGH_SHIFT           (0U)
#define GPIO_INT_MASK_H_GPIO_INT_MASK_HIGH_MASK            (0xFFFFU << GPIO_INT_MASK_H_GPIO_INT_MASK_HIGH_SHIFT)        /* 0x0000FFFF */
/* INT_TYPE_L */
#define GPIO_INT_TYPE_L_OFFSET                             (0x20)
#define GPIO_INT_TYPE_L_GPIO_INT_TYPE_LOW_SHIFT            (0U)
#define GPIO_INT_TYPE_L_GPIO_INT_TYPE_LOW_MASK             (0xFFFFU << GPIO_INT_TYPE_L_GPIO_INT_TYPE_LOW_SHIFT)         /* 0x0000FFFF */
/* INT_TYPE_H */
#define GPIO_INT_TYPE_H_OFFSET                             (0x24)
#define GPIO_INT_TYPE_H_GPIO_INT_TYPE_HIGH_SHIFT           (0U)
#define GPIO_INT_TYPE_H_GPIO_INT_TYPE_HIGH_MASK            (0xFFFFU << GPIO_INT_TYPE_H_GPIO_INT_TYPE_HIGH_SHIFT)        /* 0x0000FFFF */
/* INT_POLARITY_L */
#define GPIO_INT_POLARITY_L_OFFSET                         (0x28)
#define GPIO_INT_POLARITY_L_GPIO_INT_POLARITY_LOW_SHIFT    (0U)
#define GPIO_INT_POLARITY_L_GPIO_INT_POLARITY_LOW_MASK     (0xFFFFU << GPIO_INT_POLARITY_L_GPIO_INT_POLARITY_LOW_SHIFT) /* 0x0000FFFF */
/* INT_POLARITY_H */
#define GPIO_INT_POLARITY_H_OFFSET                         (0x2C)
#define GPIO_INT_POLARITY_H_GPIO_INT_POLARITY_HIGH_SHIFT   (0U)
#define GPIO_INT_POLARITY_H_GPIO_INT_POLARITY_HIGH_MASK    (0xFFFFU << GPIO_INT_POLARITY_H_GPIO_INT_POLARITY_HIGH_SHIFT) /* 0x0000FFFF */
/* INT_BOTHEDGE_L */
#define GPIO_INT_BOTHEDGE_L_OFFSET                         (0x30)
#define GPIO_INT_BOTHEDGE_L_GPIO_INT_BOTHEDGE_LOW_SHIFT    (0U)
#define GPIO_INT_BOTHEDGE_L_GPIO_INT_BOTHEDGE_LOW_MASK     (0xFFFFU << GPIO_INT_BOTHEDGE_L_GPIO_INT_BOTHEDGE_LOW_SHIFT) /* 0x0000FFFF */
/* INT_BOTHEDGE_H */
#define GPIO_INT_BOTHEDGE_H_OFFSET                         (0x34)
#define GPIO_INT_BOTHEDGE_H_GPIO_INT_BOTHEDGE_HIGH_SHIFT   (0U)
#define GPIO_INT_BOTHEDGE_H_GPIO_INT_BOTHEDGE_HIGH_MASK    (0xFFFFU << GPIO_INT_BOTHEDGE_H_GPIO_INT_BOTHEDGE_HIGH_SHIFT) /* 0x0000FFFF */
/* DEBOUNCE_L */
#define GPIO_DEBOUNCE_L_OFFSET                             (0x38)
#define GPIO_DEBOUNCE_L_GPIO_DEBOUNCE_LOW_SHIFT            (0U)
#define GPIO_DEBOUNCE_L_GPIO_DEBOUNCE_LOW_MASK             (0xFFFFU << GPIO_DEBOUNCE_L_GPIO_DEBOUNCE_LOW_SHIFT)         /* 0x0000FFFF */
/* DEBOUNCE_H */
#define GPIO_DEBOUNCE_H_OFFSET                             (0x3C)
#define GPIO_DEBOUNCE_H_GPIO_DEBOUNCE_HIGH_SHIFT           (0U)
#define GPIO_DEBOUNCE_H_GPIO_DEBOUNCE_HIGH_MASK            (0xFFFFU << GPIO_DEBOUNCE_H_GPIO_DEBOUNCE_HIGH_SHIFT)        /* 0x0000FFFF */
/* DBCLK_DIV_EN_L */
#define GPIO_DBCLK_DIV_EN_L_OFFSET                         (0x40)
#define GPIO_DBCLK_DIV_EN_L_GPIO_DBCLK_DIV_EN_LOW_SHIFT    (0U)
#define GPIO_DBCLK_DIV_EN_L_GPIO_DBCLK_DIV_EN_LOW_MASK     (0xFFFFU << GPIO_DBCLK_DIV_EN_L_GPIO_DBCLK_DIV_EN_LOW_SHIFT) /* 0x0000FFFF */
/* DBCLK_DIV_EN_H */
#define GPIO_DBCLK_DIV_EN_H_OFFSET                         (0x44)
#define GPIO_DBCLK_DIV_EN_H_GPIO_DBCLK_DIV_EN_HIGH_SHIFT   (0U)
#define GPIO_DBCLK_DIV_EN_H_GPIO_DBCLK_DIV_EN_HIGH_MASK    (0xFFFFU << GPIO_DBCLK_DIV_EN_H_GPIO_DBCLK_DIV_EN_HIGH_SHIFT) /* 0x0000FFFF */
/* DBCLK_DIV_CON */
#define GPIO_DBCLK_DIV_CON_OFFSET                          (0x48)
#define GPIO_DBCLK_DIV_CON_GPIO_DBCLK_DIV_CON_SHIFT        (0U)
#define GPIO_DBCLK_DIV_CON_GPIO_DBCLK_DIV_CON_MASK         (0xFFFFFFU << GPIO_DBCLK_DIV_CON_GPIO_DBCLK_DIV_CON_SHIFT)   /* 0x00FFFFFF */
/* INT_STATUS */
#define GPIO_INT_STATUS_OFFSET                             (0x50)
#define GPIO_INT_STATUS_GPIO_INT_STATUS_SHIFT              (0U)
#define GPIO_INT_STATUS_GPIO_INT_STATUS_MASK               (0xFFFFFFFFU << GPIO_INT_STATUS_GPIO_INT_STATUS_SHIFT)       /* 0xFFFFFFFF */
/* INT_RAWSTATUS */
#define GPIO_INT_RAWSTATUS_OFFSET                          (0x58)
#define GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_SHIFT        (0U)
#define GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_MASK         (0xFFFFFFFFU << GPIO_INT_RAWSTATUS_GPIO_INT_RAWSTATUS_SHIFT) /* 0xFFFFFFFF */
/* PORT_EOI_L */
#define GPIO_PORT_EOI_L_OFFSET                             (0x60)
#define GPIO_PORT_EOI_L_GPIO_PORT_EOI_LOW_SHIFT            (0U)
#define GPIO_PORT_EOI_L_GPIO_PORT_EOI_LOW_MASK             (0xFFFFU << GPIO_PORT_EOI_L_GPIO_PORT_EOI_LOW_SHIFT)         /* 0x0000FFFF */
/* PORT_EOI_H */
#define GPIO_PORT_EOI_H_OFFSET                             (0x64)
#define GPIO_PORT_EOI_H_GPIO_PORT_EOI_HIGH_SHIFT           (0U)
#define GPIO_PORT_EOI_H_GPIO_PORT_EOI_HIGH_MASK            (0xFFFFU << GPIO_PORT_EOI_H_GPIO_PORT_EOI_HIGH_SHIFT)        /* 0x0000FFFF */
/* EXT_PORT */
#define GPIO_EXT_PORT_OFFSET                               (0x70)
#define GPIO_EXT_PORT_GPIO_EXT_PORT_SHIFT                  (0U)
#define GPIO_EXT_PORT_GPIO_EXT_PORT_MASK                   (0xFFFFFFFFU << GPIO_EXT_PORT_GPIO_EXT_PORT_SHIFT)           /* 0xFFFFFFFF */
/* VER_ID */
#define GPIO_VER_ID                                        (0x01000C2BU)
#define GPIO_VER_ID_OFFSET                                 (0x78)
#define GPIO_VER_ID_GPIO_VER_ID_SHIFT                      (0U)
#define GPIO_VER_ID_GPIO_VER_ID_MASK                       (0xFFFFFFFFU << GPIO_VER_ID_GPIO_VER_ID_SHIFT)               /* 0xFFFFFFFF */
/******************************************FSPI******************************************/
/* CTRL0 */
#define FSPI_CTRL0_OFFSET                                  (0x0)
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
#define FSPI_IMR_OFFSET                                    (0x4)
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
#define FSPI_IMR_STPOLLM_SHIFT                             (8U)
#define FSPI_IMR_STPOLLM_MASK                              (0x1U << FSPI_IMR_STPOLLM_SHIFT)                             /* 0x00000100 */
/* ICLR */
#define FSPI_ICLR_OFFSET                                   (0x8)
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
#define FSPI_ICLR_STPOLLC_SHIFT                            (8U)
#define FSPI_ICLR_STPOLLC_MASK                             (0x1U << FSPI_ICLR_STPOLLC_SHIFT)                            /* 0x00000100 */
/* FTLR */
#define FSPI_FTLR_OFFSET                                   (0xC)
#define FSPI_FTLR_TXFTLR_SHIFT                             (0U)
#define FSPI_FTLR_TXFTLR_MASK                              (0xFFU << FSPI_FTLR_TXFTLR_SHIFT)                            /* 0x000000FF */
#define FSPI_FTLR_RXFTLR_SHIFT                             (8U)
#define FSPI_FTLR_RXFTLR_MASK                              (0xFFU << FSPI_FTLR_RXFTLR_SHIFT)                            /* 0x0000FF00 */
/* RCVR */
#define FSPI_RCVR_OFFSET                                   (0x10)
#define FSPI_RCVR_RCVR_SHIFT                               (0U)
#define FSPI_RCVR_RCVR_MASK                                (0x1U << FSPI_RCVR_RCVR_SHIFT)                               /* 0x00000001 */
/* AX0 */
#define FSPI_AX0_OFFSET                                    (0x14)
#define FSPI_AX0_AX_SHIFT                                  (0U)
#define FSPI_AX0_AX_MASK                                   (0xFFU << FSPI_AX0_AX_SHIFT)                                 /* 0x000000FF */
/* ABIT0 */
#define FSPI_ABIT0_OFFSET                                  (0x18)
#define FSPI_ABIT0_ABIT_SHIFT                              (0U)
#define FSPI_ABIT0_ABIT_MASK                               (0x1FU << FSPI_ABIT0_ABIT_SHIFT)                             /* 0x0000001F */
/* ISR */
#define FSPI_ISR_OFFSET                                    (0x1C)
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
#define FSPI_ISR_STPOLLS_SHIFT                             (8U)
#define FSPI_ISR_STPOLLS_MASK                              (0x1U << FSPI_ISR_STPOLLS_SHIFT)                             /* 0x00000100 */
/* FSR */
#define FSPI_FSR_OFFSET                                    (0x20)
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
#define FSPI_SR_OFFSET                                     (0x24)
#define FSPI_SR_SR_SHIFT                                   (0U)
#define FSPI_SR_SR_MASK                                    (0x1U << FSPI_SR_SR_SHIFT)                                   /* 0x00000001 */
/* RISR */
#define FSPI_RISR_OFFSET                                   (0x28)
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
#define FSPI_RISR_STPOLLS_SHIFT                            (8U)
#define FSPI_RISR_STPOLLS_MASK                             (0x1U << FSPI_RISR_STPOLLS_SHIFT)                            /* 0x00000100 */
/* VER */
#define FSPI_VER_OFFSET                                    (0x2C)
#define FSPI_VER_VER_SHIFT                                 (0U)
#define FSPI_VER_VER_MASK                                  (0xFFFFU << FSPI_VER_VER_SHIFT)                              /* 0x0000FFFF */
/* QOP */
#define FSPI_QOP_OFFSET                                    (0x30)
#define FSPI_QOP_SO123_SHIFT                               (0U)
#define FSPI_QOP_SO123_MASK                                (0x1U << FSPI_QOP_SO123_SHIFT)                               /* 0x00000001 */
#define FSPI_QOP_SO123BP_SHIFT                             (1U)
#define FSPI_QOP_SO123BP_MASK                              (0x1U << FSPI_QOP_SO123BP_SHIFT)                             /* 0x00000002 */
/* EXT_CTRL */
#define FSPI_EXT_CTRL_OFFSET                               (0x34)
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
/* POLL_CTRL */
#define FSPI_POLL_CTRL_OFFSET                              (0x38)
#define FSPI_POLL_CTRL_ST_POLL_EN_SHIFT                    (0U)
#define FSPI_POLL_CTRL_ST_POLL_EN_MASK                     (0x1U << FSPI_POLL_CTRL_ST_POLL_EN_SHIFT)                    /* 0x00000001 */
#define FSPI_POLL_CTRL_POLL_DLY_EN_SHIFT                   (1U)
#define FSPI_POLL_CTRL_POLL_DLY_EN_MASK                    (0x1U << FSPI_POLL_CTRL_POLL_DLY_EN_SHIFT)                   /* 0x00000002 */
#define FSPI_POLL_CTRL_ST_POLL_CMD_PARA_SHIFT              (8U)
#define FSPI_POLL_CTRL_ST_POLL_CMD_PARA_MASK               (0xFFU << FSPI_POLL_CTRL_ST_POLL_CMD_PARA_SHIFT)             /* 0x0000FF00 */
#define FSPI_POLL_CTRL_ST_POLL_EXPECT_DATA_SHIFT           (16U)
#define FSPI_POLL_CTRL_ST_POLL_EXPECT_DATA_MASK            (0xFFU << FSPI_POLL_CTRL_ST_POLL_EXPECT_DATA_SHIFT)          /* 0x00FF0000 */
#define FSPI_POLL_CTRL_ST_POLL_BIT_COMP_EN_SHIFT           (24U)
#define FSPI_POLL_CTRL_ST_POLL_BIT_COMP_EN_MASK            (0xFFU << FSPI_POLL_CTRL_ST_POLL_BIT_COMP_EN_SHIFT)          /* 0xFF000000 */
/* DLL_CTRL0 */
#define FSPI_DLL_CTRL0_OFFSET                              (0x3C)
#define FSPI_DLL_CTRL0_SMP_DLL_CFG_SHIFT                   (0U)
#define FSPI_DLL_CTRL0_SMP_DLL_CFG_MASK                    (0xFFU << FSPI_DLL_CTRL0_SMP_DLL_CFG_SHIFT)                  /* 0x000000FF */
#define FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT                  (8U)
#define FSPI_DLL_CTRL0_SCLK_SMP_SEL_MASK                   (0x1U << FSPI_DLL_CTRL0_SCLK_SMP_SEL_SHIFT)                  /* 0x00000100 */
/* HRDYMASK */
#define FSPI_HRDYMASK_OFFSET                               (0x40)
#define FSPI_HRDYMASK_HRDYMASK_SHIFT                       (0U)
#define FSPI_HRDYMASK_HRDYMASK_MASK                        (0x1U << FSPI_HRDYMASK_HRDYMASK_SHIFT)                       /* 0x00000001 */
/* EXT_AX */
#define FSPI_EXT_AX_OFFSET                                 (0x44)
#define FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT                    (0U)
#define FSPI_EXT_AX_AX_CANCEL_PAT_MASK                     (0xFFU << FSPI_EXT_AX_AX_CANCEL_PAT_SHIFT)                   /* 0x000000FF */
#define FSPI_EXT_AX_AX_SETUP_PAT_SHIFT                     (8U)
#define FSPI_EXT_AX_AX_SETUP_PAT_MASK                      (0xFFU << FSPI_EXT_AX_AX_SETUP_PAT_SHIFT)                    /* 0x0000FF00 */
/* SCLK_INATM_CNT */
#define FSPI_SCLK_INATM_CNT_OFFSET                         (0x48)
#define FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_SHIFT           (0U)
#define FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_MASK            (0xFFFFFFFFU << FSPI_SCLK_INATM_CNT_SCLK_INATM_CNT_SHIFT)    /* 0xFFFFFFFF */
/* AUTO_RF_CNT */
#define FSPI_AUTO_RF_CNT_OFFSET                            (0x4C)
#define FSPI_AUTO_RF_CNT_AUTO_RF_CNT_SHIFT                 (0U)
#define FSPI_AUTO_RF_CNT_AUTO_RF_CNT_MASK                  (0xFFFFFFFFU << FSPI_AUTO_RF_CNT_AUTO_RF_CNT_SHIFT)          /* 0xFFFFFFFF */
/* XMMC_WCMD0 */
#define FSPI_XMMC_WCMD0_OFFSET                             (0x50)
#define FSPI_XMMC_WCMD0_CMD_SHIFT                          (0U)
#define FSPI_XMMC_WCMD0_CMD_MASK                           (0xFFU << FSPI_XMMC_WCMD0_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_WCMD0_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_WCMD0_DUMM_MASK                          (0xFU << FSPI_XMMC_WCMD0_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_WCMD0_CONT_SHIFT                         (13U)
#define FSPI_XMMC_WCMD0_CONT_MASK                          (0x1U << FSPI_XMMC_WCMD0_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_WCMD0_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_WCMD0_ADDRB_MASK                         (0x3U << FSPI_XMMC_WCMD0_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_RCMD0 */
#define FSPI_XMMC_RCMD0_OFFSET                             (0x54)
#define FSPI_XMMC_RCMD0_CMD_SHIFT                          (0U)
#define FSPI_XMMC_RCMD0_CMD_MASK                           (0xFFU << FSPI_XMMC_RCMD0_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_RCMD0_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_RCMD0_DUMM_MASK                          (0xFU << FSPI_XMMC_RCMD0_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_RCMD0_CONT_SHIFT                         (13U)
#define FSPI_XMMC_RCMD0_CONT_MASK                          (0x1U << FSPI_XMMC_RCMD0_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_RCMD0_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_RCMD0_ADDRB_MASK                         (0x3U << FSPI_XMMC_RCMD0_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_CTRL */
#define FSPI_XMMC_CTRL_OFFSET                              (0x58)
#define FSPI_XMMC_CTRL_DEV_HWEN_SHIFT                      (5U)
#define FSPI_XMMC_CTRL_DEV_HWEN_MASK                       (0x1U << FSPI_XMMC_CTRL_DEV_HWEN_SHIFT)                      /* 0x00000020 */
#define FSPI_XMMC_CTRL_PFT_EN_SHIFT                        (6U)
#define FSPI_XMMC_CTRL_PFT_EN_MASK                         (0x1U << FSPI_XMMC_CTRL_PFT_EN_SHIFT)                        /* 0x00000040 */
#define FSPI_XMMC_CTRL_UINCR_PFT_EN_SHIFT                  (7U)
#define FSPI_XMMC_CTRL_UINCR_PFT_EN_MASK                   (0x1U << FSPI_XMMC_CTRL_UINCR_PFT_EN_SHIFT)                  /* 0x00000080 */
#define FSPI_XMMC_CTRL_UINCR_LEN_SHIFT                     (8U)
#define FSPI_XMMC_CTRL_UINCR_LEN_MASK                      (0xFU << FSPI_XMMC_CTRL_UINCR_LEN_SHIFT)                     /* 0x00000F00 */
#define FSPI_XMMC_CTRL_DEV_WRAP_EN_SHIFT                   (12U)
#define FSPI_XMMC_CTRL_DEV_WRAP_EN_MASK                    (0x1U << FSPI_XMMC_CTRL_DEV_WRAP_EN_SHIFT)                   /* 0x00001000 */
#define FSPI_XMMC_CTRL_DEV_INCR_EN_SHIFT                   (13U)
#define FSPI_XMMC_CTRL_DEV_INCR_EN_MASK                    (0x1U << FSPI_XMMC_CTRL_DEV_INCR_EN_SHIFT)                   /* 0x00002000 */
#define FSPI_XMMC_CTRL_DEV_UDFINCR_EN_SHIFT                (14U)
#define FSPI_XMMC_CTRL_DEV_UDFINCR_EN_MASK                 (0x1U << FSPI_XMMC_CTRL_DEV_UDFINCR_EN_SHIFT)                /* 0x00004000 */
/* MODE */
#define FSPI_MODE_OFFSET                                   (0x5C)
#define FSPI_MODE_XMMC_MODE_EN_SHIFT                       (0U)
#define FSPI_MODE_XMMC_MODE_EN_MASK                        (0x1U << FSPI_MODE_XMMC_MODE_EN_SHIFT)                       /* 0x00000001 */
/* DEVRGN */
#define FSPI_DEVRGN_OFFSET                                 (0x60)
#define FSPI_DEVRGN_RSIZE_SHIFT                            (0U)
#define FSPI_DEVRGN_RSIZE_MASK                             (0x1FU << FSPI_DEVRGN_RSIZE_SHIFT)                           /* 0x0000001F */
/* DEVSIZE0 */
#define FSPI_DEVSIZE0_OFFSET                               (0x64)
#define FSPI_DEVSIZE0_DSIZE_SHIFT                          (0U)
#define FSPI_DEVSIZE0_DSIZE_MASK                           (0x1FU << FSPI_DEVSIZE0_DSIZE_SHIFT)                         /* 0x0000001F */
/* TME0 */
#define FSPI_TME0_OFFSET                                   (0x68)
#define FSPI_TME0_AUTO_RF_EN_SHIFT                         (0U)
#define FSPI_TME0_AUTO_RF_EN_MASK                          (0x1U << FSPI_TME0_AUTO_RF_EN_SHIFT)                         /* 0x00000001 */
#define FSPI_TME0_SCLK_INATM_EN_SHIFT                      (1U)
#define FSPI_TME0_SCLK_INATM_EN_MASK                       (0x1U << FSPI_TME0_SCLK_INATM_EN_SHIFT)                      /* 0x00000002 */
/* POLLDLY_CTRL */
#define FSPI_POLLDLY_CTRL_OFFSET                           (0x6C)
#define FSPI_POLLDLY_CTRL_CNT_SHIFT                        (0U)
#define FSPI_POLLDLY_CTRL_CNT_MASK                         (0x7FFFFFFFU << FSPI_POLLDLY_CTRL_CNT_SHIFT)                 /* 0x7FFFFFFF */
#define FSPI_POLLDLY_CTRL_POLLDLY_IP_SHIFT                 (31U)
#define FSPI_POLLDLY_CTRL_POLLDLY_IP_MASK                  (0x1U << FSPI_POLLDLY_CTRL_POLLDLY_IP_SHIFT)                 /* 0x80000000 */
/* DMATR */
#define FSPI_DMATR_OFFSET                                  (0x80)
#define FSPI_DMATR_DMATR_SHIFT                             (0U)
#define FSPI_DMATR_DMATR_MASK                              (0x1U << FSPI_DMATR_DMATR_SHIFT)                             /* 0x00000001 */
/* DMAADDR */
#define FSPI_DMAADDR_OFFSET                                (0x84)
#define FSPI_DMAADDR_DMAADDR_SHIFT                         (0U)
#define FSPI_DMAADDR_DMAADDR_MASK                          (0xFFFFFFFFU << FSPI_DMAADDR_DMAADDR_SHIFT)                  /* 0xFFFFFFFF */
/* POLL_DATA */
#define FSPI_POLL_DATA_OFFSET                              (0x90)
#define FSPI_POLL_DATA_POLL_DATA_SHIFT                     (0U)
#define FSPI_POLL_DATA_POLL_DATA_MASK                      (0xFFU << FSPI_POLL_DATA_POLL_DATA_SHIFT)                    /* 0x000000FF */
#define FSPI_POLL_DATA_POLL_STA_SHIFT                      (8U)
#define FSPI_POLL_DATA_POLL_STA_MASK                       (0x1U << FSPI_POLL_DATA_POLL_STA_SHIFT)                      /* 0x00000100 */
/* XMMCSR */
#define FSPI_XMMCSR_OFFSET                                 (0x94)
#define FSPI_XMMCSR_SLOPOVER0_SHIFT                        (0U)
#define FSPI_XMMCSR_SLOPOVER0_MASK                         (0x1U << FSPI_XMMCSR_SLOPOVER0_SHIFT)                        /* 0x00000001 */
#define FSPI_XMMCSR_SLOPOVER1_SHIFT                        (1U)
#define FSPI_XMMCSR_SLOPOVER1_MASK                         (0x1U << FSPI_XMMCSR_SLOPOVER1_SHIFT)                        /* 0x00000002 */
/* CMD */
#define FSPI_CMD_OFFSET                                    (0x100)
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
#define FSPI_ADDR_OFFSET                                   (0x104)
#define FSPI_ADDR_ADDR_SHIFT                               (0U)
#define FSPI_ADDR_ADDR_MASK                                (0xFFFFFFFFU << FSPI_ADDR_ADDR_SHIFT)                        /* 0xFFFFFFFF */
/* DATA */
#define FSPI_DATA_OFFSET                                   (0x108)
#define FSPI_DATA_DATA_SHIFT                               (0U)
#define FSPI_DATA_DATA_MASK                                (0xFFFFFFFFU << FSPI_DATA_DATA_SHIFT)                        /* 0xFFFFFFFF */
/* CTRL1 */
#define FSPI_CTRL1_OFFSET                                  (0x200)
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
#define FSPI_AX1_OFFSET                                    (0x214)
#define FSPI_AX1_AX_SHIFT                                  (0U)
#define FSPI_AX1_AX_MASK                                   (0xFFU << FSPI_AX1_AX_SHIFT)                                 /* 0x000000FF */
/* ABIT1 */
#define FSPI_ABIT1_OFFSET                                  (0x218)
#define FSPI_ABIT1_ABIT_SHIFT                              (0U)
#define FSPI_ABIT1_ABIT_MASK                               (0x1FU << FSPI_ABIT1_ABIT_SHIFT)                             /* 0x0000001F */
/* DLL_CTRL1 */
#define FSPI_DLL_CTRL1_OFFSET                              (0x23C)
#define FSPI_DLL_CTRL1_SMP_DLL_CFG_SHIFT                   (0U)
#define FSPI_DLL_CTRL1_SMP_DLL_CFG_MASK                    (0xFFU << FSPI_DLL_CTRL1_SMP_DLL_CFG_SHIFT)                  /* 0x000000FF */
#define FSPI_DLL_CTRL1_SCLK_SMP_SEL_SHIFT                  (8U)
#define FSPI_DLL_CTRL1_SCLK_SMP_SEL_MASK                   (0x1U << FSPI_DLL_CTRL1_SCLK_SMP_SEL_SHIFT)                  /* 0x00000100 */
/* XMMC_WCMD1 */
#define FSPI_XMMC_WCMD1_OFFSET                             (0x250)
#define FSPI_XMMC_WCMD1_CMD_SHIFT                          (0U)
#define FSPI_XMMC_WCMD1_CMD_MASK                           (0xFFU << FSPI_XMMC_WCMD1_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_WCMD1_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_WCMD1_DUMM_MASK                          (0xFU << FSPI_XMMC_WCMD1_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_WCMD1_CONT_SHIFT                         (13U)
#define FSPI_XMMC_WCMD1_CONT_MASK                          (0x1U << FSPI_XMMC_WCMD1_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_WCMD1_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_WCMD1_ADDRB_MASK                         (0x3U << FSPI_XMMC_WCMD1_ADDRB_SHIFT)                        /* 0x0000C000 */
/* XMMC_RCMD1 */
#define FSPI_XMMC_RCMD1_OFFSET                             (0x254)
#define FSPI_XMMC_RCMD1_CMD_SHIFT                          (0U)
#define FSPI_XMMC_RCMD1_CMD_MASK                           (0xFFU << FSPI_XMMC_RCMD1_CMD_SHIFT)                         /* 0x000000FF */
#define FSPI_XMMC_RCMD1_DUMM_SHIFT                         (8U)
#define FSPI_XMMC_RCMD1_DUMM_MASK                          (0xFU << FSPI_XMMC_RCMD1_DUMM_SHIFT)                         /* 0x00000F00 */
#define FSPI_XMMC_RCMD1_CONT_SHIFT                         (13U)
#define FSPI_XMMC_RCMD1_CONT_MASK                          (0x1U << FSPI_XMMC_RCMD1_CONT_SHIFT)                         /* 0x00002000 */
#define FSPI_XMMC_RCMD1_ADDRB_SHIFT                        (14U)
#define FSPI_XMMC_RCMD1_ADDRB_MASK                         (0x3U << FSPI_XMMC_RCMD1_ADDRB_SHIFT)                        /* 0x0000C000 */
/* DEVSIZE1 */
#define FSPI_DEVSIZE1_OFFSET                               (0x264)
#define FSPI_DEVSIZE1_DSIZE_SHIFT                          (0U)
#define FSPI_DEVSIZE1_DSIZE_MASK                           (0x1FU << FSPI_DEVSIZE1_DSIZE_SHIFT)                         /* 0x0000001F */
/* TME1 */
#define FSPI_TME1_OFFSET                                   (0x268)
#define FSPI_TME1_AUTO_RF_EN_SHIFT                         (0U)
#define FSPI_TME1_AUTO_RF_EN_MASK                          (0x1U << FSPI_TME1_AUTO_RF_EN_SHIFT)                         /* 0x00000001 */
#define FSPI_TME1_SCLK_INATM_EN_SHIFT                      (1U)
#define FSPI_TME1_SCLK_INATM_EN_MASK                       (0x1U << FSPI_TME1_SCLK_INATM_EN_SHIFT)                      /* 0x00000002 */

#define FSPI_CHIP_CNT                                      (0x2U)
/******************************************INTC******************************************/
/* IRQ_INTEN_L */
#define INTC_IRQ_INTEN_L_OFFSET                            (0x0)
#define INTC_IRQ_INTEN_L_IRQ_INTEN_L_SHIFT                 (0U)
#define INTC_IRQ_INTEN_L_IRQ_INTEN_L_MASK                  (0xFFFFFFFFU << INTC_IRQ_INTEN_L_IRQ_INTEN_L_SHIFT)          /* 0xFFFFFFFF */
/* IRQ_INTEN_H */
#define INTC_IRQ_INTEN_H_OFFSET                            (0x4)
#define INTC_IRQ_INTEN_H_IRQ_INTEN_H_SHIFT                 (0U)
#define INTC_IRQ_INTEN_H_IRQ_INTEN_H_MASK                  (0xFFFFFFFFU << INTC_IRQ_INTEN_H_IRQ_INTEN_H_SHIFT)          /* 0xFFFFFFFF */
/* IRQ_INTMASK_L */
#define INTC_IRQ_INTMASK_L_OFFSET                          (0x8)
#define INTC_IRQ_INTMASK_L_IRQ_INTMASK_L_SHIFT             (0U)
#define INTC_IRQ_INTMASK_L_IRQ_INTMASK_L_MASK              (0xFFFFFFFFU << INTC_IRQ_INTMASK_L_IRQ_INTMASK_L_SHIFT)      /* 0xFFFFFFFF */
/* IRQ_INTMASK_H */
#define INTC_IRQ_INTMASK_H_OFFSET                          (0xC)
#define INTC_IRQ_INTMASK_H_IRQ_INTMASK_H_SHIFT             (0U)
#define INTC_IRQ_INTMASK_H_IRQ_INTMASK_H_MASK              (0xFFFFFFFFU << INTC_IRQ_INTMASK_H_IRQ_INTMASK_H_SHIFT)      /* 0xFFFFFFFF */
/* IRQ_INTFORCE_L */
#define INTC_IRQ_INTFORCE_L_OFFSET                         (0x10)
#define INTC_IRQ_INTFORCE_L_IRQ_INTFORCE_L_SHIFT           (0U)
#define INTC_IRQ_INTFORCE_L_IRQ_INTFORCE_L_MASK            (0xFFFFFFFFU << INTC_IRQ_INTFORCE_L_IRQ_INTFORCE_L_SHIFT)    /* 0xFFFFFFFF */
/* IRQ_INTFORCE_H */
#define INTC_IRQ_INTFORCE_H_OFFSET                         (0x14)
#define INTC_IRQ_INTFORCE_H_IRQ_INTFORCE_H_SHIFT           (0U)
#define INTC_IRQ_INTFORCE_H_IRQ_INTFORCE_H_MASK            (0xFFFFFFFFU << INTC_IRQ_INTFORCE_H_IRQ_INTFORCE_H_SHIFT)    /* 0xFFFFFFFF */
/* IRQ_RAWSTATUS_L */
#define INTC_IRQ_RAWSTATUS_L_OFFSET                        (0x18)
#define INTC_IRQ_RAWSTATUS_L_IRQ_RAWSTATUS_L_SHIFT         (0U)
#define INTC_IRQ_RAWSTATUS_L_IRQ_RAWSTATUS_L_MASK          (0xFFFFFFFFU << INTC_IRQ_RAWSTATUS_L_IRQ_RAWSTATUS_L_SHIFT)  /* 0xFFFFFFFF */
/* IRQ_RAWSTATUS_H */
#define INTC_IRQ_RAWSTATUS_H_OFFSET                        (0x1C)
#define INTC_IRQ_RAWSTATUS_H_IRQ_RAWSTATUS_H_SHIFT         (0U)
#define INTC_IRQ_RAWSTATUS_H_IRQ_RAWSTATUS_H_MASK          (0xFFFFFFFFU << INTC_IRQ_RAWSTATUS_H_IRQ_RAWSTATUS_H_SHIFT)  /* 0xFFFFFFFF */
/* IRQ_STATUS_L */
#define INTC_IRQ_STATUS_L_OFFSET                           (0x20)
#define INTC_IRQ_STATUS_L_IRQ_STATUS_L_SHIFT               (0U)
#define INTC_IRQ_STATUS_L_IRQ_STATUS_L_MASK                (0xFFFFFFFFU << INTC_IRQ_STATUS_L_IRQ_STATUS_L_SHIFT)        /* 0xFFFFFFFF */
/* IRQ_STATUS_H */
#define INTC_IRQ_STATUS_H_OFFSET                           (0x24)
#define INTC_IRQ_STATUS_H_IRQ_STATUS_H_SHIFT               (0U)
#define INTC_IRQ_STATUS_H_IRQ_STATUS_H_MASK                (0xFFFFFFFFU << INTC_IRQ_STATUS_H_IRQ_STATUS_H_SHIFT)        /* 0xFFFFFFFF */
/* IRQ_MASKSTATUS_L */
#define INTC_IRQ_MASKSTATUS_L_OFFSET                       (0x28)
#define INTC_IRQ_MASKSTATUS_L_IRQ_MASKSTATUS_L_SHIFT       (0U)
#define INTC_IRQ_MASKSTATUS_L_IRQ_MASKSTATUS_L_MASK        (0xFFFFFFFFU << INTC_IRQ_MASKSTATUS_L_IRQ_MASKSTATUS_L_SHIFT) /* 0xFFFFFFFF */
/* IRQ_MASKSTATUS_H */
#define INTC_IRQ_MASKSTATUS_H_OFFSET                       (0x2C)
#define INTC_IRQ_MASKSTATUS_H_IRQ_MASKSTATUS_H_SHIFT       (0U)
#define INTC_IRQ_MASKSTATUS_H_IRQ_MASKSTATUS_H_MASK        (0xFFFFFFFFU << INTC_IRQ_MASKSTATUS_H_IRQ_MASKSTATUS_H_SHIFT) /* 0xFFFFFFFF */
/* IRQ_FINALSTATUS_L */
#define INTC_IRQ_FINALSTATUS_L_OFFSET                      (0x30)
#define INTC_IRQ_FINALSTATUS_L_IRQ_FINALSTATUS_L_SHIFT     (0U)
#define INTC_IRQ_FINALSTATUS_L_IRQ_FINALSTATUS_L_MASK      (0xFFFFFFFFU << INTC_IRQ_FINALSTATUS_L_IRQ_FINALSTATUS_L_SHIFT) /* 0xFFFFFFFF */
/* IRQ_FINALSTATUS_H */
#define INTC_IRQ_FINALSTATUS_H_OFFSET                      (0x34)
#define INTC_IRQ_FINALSTATUS_H_IRQ_FINALSTATUS_H_SHIFT     (0U)
#define INTC_IRQ_FINALSTATUS_H_IRQ_FINALSTATUS_H_MASK      (0xFFFFFFFFU << INTC_IRQ_FINALSTATUS_H_IRQ_FINALSTATUS_H_SHIFT) /* 0xFFFFFFFF */
/* FIQ_INTEN */
#define INTC_FIQ_INTEN_OFFSET                              (0xC0)
#define INTC_FIQ_INTEN_FIQ_INTEN_SHIFT                     (0U)
#define INTC_FIQ_INTEN_FIQ_INTEN_MASK                      (0x3U << INTC_FIQ_INTEN_FIQ_INTEN_SHIFT)                     /* 0x00000003 */
/* FIQ_INTMASK */
#define INTC_FIQ_INTMASK_OFFSET                            (0xC4)
#define INTC_FIQ_INTMASK_FIQ_INTMASK_SHIFT                 (0U)
#define INTC_FIQ_INTMASK_FIQ_INTMASK_MASK                  (0x3U << INTC_FIQ_INTMASK_FIQ_INTMASK_SHIFT)                 /* 0x00000003 */
/* FIQ_INTFORCE */
#define INTC_FIQ_INTFORCE_OFFSET                           (0xC8)
#define INTC_FIQ_INTFORCE_FIQ_INTFORCE_SHIFT               (0U)
#define INTC_FIQ_INTFORCE_FIQ_INTFORCE_MASK                (0x3U << INTC_FIQ_INTFORCE_FIQ_INTFORCE_SHIFT)               /* 0x00000003 */
/* FIQ_RAWSTATUS */
#define INTC_FIQ_RAWSTATUS_OFFSET                          (0xCC)
#define INTC_FIQ_RAWSTATUS_FIQ_RAWSTATUS_SHIFT             (0U)
#define INTC_FIQ_RAWSTATUS_FIQ_RAWSTATUS_MASK              (0x3U << INTC_FIQ_RAWSTATUS_FIQ_RAWSTATUS_SHIFT)             /* 0x00000003 */
/* FIQ_STATUS */
#define INTC_FIQ_STATUS_OFFSET                             (0xD0)
#define INTC_FIQ_STATUS_FIQ_STATUS_SHIFT                   (0U)
#define INTC_FIQ_STATUS_FIQ_STATUS_MASK                    (0x3U << INTC_FIQ_STATUS_FIQ_STATUS_SHIFT)                   /* 0x00000003 */
/* FIQ_FINALSTATUS */
#define INTC_FIQ_FINALSTATUS_OFFSET                        (0xD4)
#define INTC_FIQ_FINALSTATUS_FIQ_FINALSTATUS_SHIFT         (0U)
#define INTC_FIQ_FINALSTATUS_FIQ_FINALSTATUS_MASK          (0x3U << INTC_FIQ_FINALSTATUS_FIQ_FINALSTATUS_SHIFT)         /* 0x00000003 */
/* IRQ_PLEVEL */
#define INTC_IRQ_PLEVEL_OFFSET                             (0xD8)
#define INTC_IRQ_PLEVEL_IRQ_PLEVEL_SHIFT                   (0U)
#define INTC_IRQ_PLEVEL_IRQ_PLEVEL_MASK                    (0xFU << INTC_IRQ_PLEVEL_IRQ_PLEVEL_SHIFT)                   /* 0x0000000F */
/* IRQ_PR_OFFSET */
#define INTC_IRQ_PR_OFFSET_OFFSET                          (0xE8)
#define INTC_IRQ_PR_OFFSET_IRQ_PR_OFFSET_SHIFT             (0U)
#define INTC_IRQ_PR_OFFSET_IRQ_PR_OFFSET_MASK              (0xFU << INTC_IRQ_PR_OFFSET_IRQ_PR_OFFSET_SHIFT)             /* 0x0000000F */
/* AHB_ICTL_COMP_VERSION */
#define INTC_AHB_ICTL_COMP_VERSION_OFFSET                  (0x3F8)
#define INTC_AHB_ICTL_COMP_VERSION_AHB_ICTL_COMP_VERSION_SHIFT (0U)
#define INTC_AHB_ICTL_COMP_VERSION_AHB_ICTL_COMP_VERSION_MASK (0xFFFFFFFFU << INTC_AHB_ICTL_COMP_VERSION_AHB_ICTL_COMP_VERSION_SHIFT) /* 0xFFFFFFFF */
/* ICTL_COMP_TYPE */
#define INTC_ICTL_COMP_TYPE_OFFSET                         (0x3FC)
#define INTC_ICTL_COMP_TYPE_ICTL_COMP_TYPE_SHIFT           (0U)
#define INTC_ICTL_COMP_TYPE_ICTL_COMP_TYPE_MASK            (0xFFFFFFFFU << INTC_ICTL_COMP_TYPE_ICTL_COMP_TYPE_SHIFT)    /* 0xFFFFFFFF */
/******************************************PMU*******************************************/
/* WAKEUP_CFG0 */
#define PMU_WAKEUP_CFG0_OFFSET                             (0x0)
#define PMU_WAKEUP_CFG0_GPIO0D_POSEDGE_EN_SHIFT            (0U)
#define PMU_WAKEUP_CFG0_GPIO0D_POSEDGE_EN_MASK             (0x1U << PMU_WAKEUP_CFG0_GPIO0D_POSEDGE_EN_SHIFT)            /* 0x00000001 */
/* WAKEUP_CFG1 */
#define PMU_WAKEUP_CFG1_OFFSET                             (0x4)
#define PMU_WAKEUP_CFG1_GPIO0D_NEGEDGE_EN_SHIFT            (0U)
#define PMU_WAKEUP_CFG1_GPIO0D_NEGEDGE_EN_MASK             (0x1U << PMU_WAKEUP_CFG1_GPIO0D_NEGEDGE_EN_SHIFT)            /* 0x00000001 */
/* WAKEUP_CFG2 */
#define PMU_WAKEUP_CFG2_OFFSET                             (0x8)
#define PMU_WAKEUP_CFG2_PWRMODE_INT_WAKEUP_EN_SHIFT        (0U)
#define PMU_WAKEUP_CFG2_PWRMODE_INT_WAKEUP_EN_MASK         (0x1U << PMU_WAKEUP_CFG2_PWRMODE_INT_WAKEUP_EN_SHIFT)        /* 0x00000001 */
#define PMU_WAKEUP_CFG2_GPIO_INT_EN_SHIFT                  (1U)
#define PMU_WAKEUP_CFG2_GPIO_INT_EN_MASK                   (0x1U << PMU_WAKEUP_CFG2_GPIO_INT_EN_SHIFT)                  /* 0x00000002 */
#define PMU_WAKEUP_CFG2_TIMEOUT_EN_SHIFT                   (2U)
#define PMU_WAKEUP_CFG2_TIMEOUT_EN_MASK                    (0x1U << PMU_WAKEUP_CFG2_TIMEOUT_EN_SHIFT)                   /* 0x00000004 */
#define PMU_WAKEUP_CFG2_BTWAKE_EN_SHIFT                    (3U)
#define PMU_WAKEUP_CFG2_BTWAKE_EN_MASK                     (0x1U << PMU_WAKEUP_CFG2_BTWAKE_EN_SHIFT)                    /* 0x00000008 */
/* PWRDN_CON */
#define PMU_PWRDN_CON_OFFSET                               (0xC)
#define PMU_PWRDN_CON_PD_DSP_PWRDWN_EN_SHIFT               (0U)
#define PMU_PWRDN_CON_PD_DSP_PWRDWN_EN_MASK                (0x1U << PMU_PWRDN_CON_PD_DSP_PWRDWN_EN_SHIFT)               /* 0x00000001 */
#define PMU_PWRDN_CON_PD_MCU_PWRDWN_EN_SHIFT               (1U)
#define PMU_PWRDN_CON_PD_MCU_PWRDWN_EN_MASK                (0x1U << PMU_PWRDN_CON_PD_MCU_PWRDWN_EN_SHIFT)               /* 0x00000002 */
#define PMU_PWRDN_CON_PD_BT_PWRDWN_EN_SHIFT                (2U)
#define PMU_PWRDN_CON_PD_BT_PWRDWN_EN_MASK                 (0x1U << PMU_PWRDN_CON_PD_BT_PWRDWN_EN_SHIFT)                /* 0x00000004 */
#define PMU_PWRDN_CON_LOGIC_POWER_OFF_ENABLE_SHIFT         (3U)
#define PMU_PWRDN_CON_LOGIC_POWER_OFF_ENABLE_MASK          (0x1U << PMU_PWRDN_CON_LOGIC_POWER_OFF_ENABLE_SHIFT)         /* 0x00000008 */
/* PMU_NOC_AUTO_ENA */
#define PMU_PMU_NOC_AUTO_ENA_OFFSET                        (0x10)
#define PMU_PMU_NOC_AUTO_ENA_PMU_NOC_AUTO_ENA_SHIFT        (0U)
#define PMU_PMU_NOC_AUTO_ENA_PMU_NOC_AUTO_ENA_MASK         (0x1U << PMU_PMU_NOC_AUTO_ENA_PMU_NOC_AUTO_ENA_SHIFT)        /* 0x00000001 */
#define PMU_PMU_NOC_AUTO_ENA_BT_NOC_AUTO_ENA_SHIFT         (1U)
#define PMU_PMU_NOC_AUTO_ENA_BT_NOC_AUTO_ENA_MASK          (0x1U << PMU_PMU_NOC_AUTO_ENA_BT_NOC_AUTO_ENA_SHIFT)         /* 0x00000002 */
#define PMU_PMU_NOC_AUTO_ENA_MCU_NOC_AUTO_ENA_SHIFT        (2U)
#define PMU_PMU_NOC_AUTO_ENA_MCU_NOC_AUTO_ENA_MASK         (0x1U << PMU_PMU_NOC_AUTO_ENA_MCU_NOC_AUTO_ENA_SHIFT)        /* 0x00000004 */
#define PMU_PMU_NOC_AUTO_ENA_DSP_NOC_AUTO_ENA_SHIFT        (3U)
#define PMU_PMU_NOC_AUTO_ENA_DSP_NOC_AUTO_ENA_MASK         (0x1U << PMU_PMU_NOC_AUTO_ENA_DSP_NOC_AUTO_ENA_SHIFT)        /* 0x00000008 */
/* PWRDN_ST */
#define PMU_PWRDN_ST_OFFSET                                (0x20)
#define PMU_PWRDN_ST_PD_DSP_PWR_STAT_SHIFT                 (0U)
#define PMU_PWRDN_ST_PD_DSP_PWR_STAT_MASK                  (0x1U << PMU_PWRDN_ST_PD_DSP_PWR_STAT_SHIFT)                 /* 0x00000001 */
#define PMU_PWRDN_ST_PD_MCU_PWR_STAT_SHIFT                 (1U)
#define PMU_PWRDN_ST_PD_MCU_PWR_STAT_MASK                  (0x1U << PMU_PWRDN_ST_PD_MCU_PWR_STAT_SHIFT)                 /* 0x00000002 */
#define PMU_PWRDN_ST_PD_BT_PWR_STAT_SHIFT                  (2U)
#define PMU_PWRDN_ST_PD_BT_PWR_STAT_MASK                   (0x1U << PMU_PWRDN_ST_PD_BT_PWR_STAT_SHIFT)                  /* 0x00000004 */
/* PLL_CON */
#define PMU_PLL_CON_OFFSET                                 (0x24)
#define PMU_PLL_CON_PLL_PD_CFG_SHIFT                       (0U)
#define PMU_PLL_CON_PLL_PD_CFG_MASK                        (0x7U << PMU_PLL_CON_PLL_PD_CFG_SHIFT)                       /* 0x00000007 */
#define PMU_PLL_CON_SFT_PLL_PD_SHIFT                       (3U)
#define PMU_PLL_CON_SFT_PLL_PD_MASK                        (0x7U << PMU_PLL_CON_SFT_PLL_PD_SHIFT)                       /* 0x00000038 */
/* PWRMODE_CON */
#define PMU_PWRMODE_CON_OFFSET                             (0x28)
#define PMU_PWRMODE_CON_POWER_MODE_EN_SHIFT                (0U)
#define PMU_PWRMODE_CON_POWER_MODE_EN_MASK                 (0x1U << PMU_PWRMODE_CON_POWER_MODE_EN_SHIFT)                /* 0x00000001 */
#define PMU_PWRMODE_CON_PMU_USE_LF_SHIFT                   (2U)
#define PMU_PWRMODE_CON_PMU_USE_LF_MASK                    (0x1U << PMU_PWRMODE_CON_PMU_USE_LF_SHIFT)                   /* 0x00000004 */
#define PMU_PWRMODE_CON_PLL_PD_EN_SHIFT                    (3U)
#define PMU_PWRMODE_CON_PLL_PD_EN_MASK                     (0x1U << PMU_PWRMODE_CON_PLL_PD_EN_SHIFT)                    /* 0x00000008 */
#define PMU_PWRMODE_CON_MCU_PD_EN_SHIFT                    (4U)
#define PMU_PWRMODE_CON_MCU_PD_EN_MASK                     (0x1U << PMU_PWRMODE_CON_MCU_PD_EN_SHIFT)                    /* 0x00000010 */
#define PMU_PWRMODE_CON_CLAMP_EN_SHIFT                     (5U)
#define PMU_PWRMODE_CON_CLAMP_EN_MASK                      (0x1U << PMU_PWRMODE_CON_CLAMP_EN_SHIFT)                     /* 0x00000020 */
#define PMU_PWRMODE_CON_WAKE_UP_RESET_EN_SHIFT             (6U)
#define PMU_PWRMODE_CON_WAKE_UP_RESET_EN_MASK              (0x1U << PMU_PWRMODE_CON_WAKE_UP_RESET_EN_SHIFT)             /* 0x00000040 */
#define PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_CFG_SHIFT       (7U)
#define PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_CFG_MASK        (0x1U << PMU_PWRMODE_CON_GLOBAL_INT_DISABLE_CFG_SHIFT)       /* 0x00000080 */
#define PMU_PWRMODE_CON_WAIT_WAKEUP_BEGIN_CFG_SHIFT        (8U)
#define PMU_PWRMODE_CON_WAIT_WAKEUP_BEGIN_CFG_MASK         (0x1U << PMU_PWRMODE_CON_WAIT_WAKEUP_BEGIN_CFG_SHIFT)        /* 0x00000100 */
/* SFT_CON */
#define PMU_SFT_CON_OFFSET                                 (0x2C)
#define PMU_SFT_CON_PMU_LF_ENA_CFG_SHIFT                   (1U)
#define PMU_SFT_CON_PMU_LF_ENA_CFG_MASK                    (0x1U << PMU_SFT_CON_PMU_LF_ENA_CFG_SHIFT)                   /* 0x00000002 */
#define PMU_SFT_CON_INPUT_CLAMP_CFG_SHIFT                  (2U)
#define PMU_SFT_CON_INPUT_CLAMP_CFG_MASK                   (0x1U << PMU_SFT_CON_INPUT_CLAMP_CFG_SHIFT)                  /* 0x00000004 */
#define PMU_SFT_CON_POWEROFF_WAKEUP_RST_CFG_SHIFT          (3U)
#define PMU_SFT_CON_POWEROFF_WAKEUP_RST_CFG_MASK           (0x1U << PMU_SFT_CON_POWEROFF_WAKEUP_RST_CFG_SHIFT)          /* 0x00000008 */
#define PMU_SFT_CON_LOGIC_POWEROFF_CFG_SHIFT               (4U)
#define PMU_SFT_CON_LOGIC_POWEROFF_CFG_MASK                (0x1U << PMU_SFT_CON_LOGIC_POWEROFF_CFG_SHIFT)               /* 0x00000010 */
/* INT_CON */
#define PMU_INT_CON_OFFSET                                 (0x30)
#define PMU_INT_CON_PMU_INT_EN_SHIFT                       (0U)
#define PMU_INT_CON_PMU_INT_EN_MASK                        (0x1U << PMU_INT_CON_PMU_INT_EN_SHIFT)                       /* 0x00000001 */
#define PMU_INT_CON_PWRMODE_WAKEUP_INT_EN_SHIFT            (1U)
#define PMU_INT_CON_PWRMODE_WAKEUP_INT_EN_MASK             (0x1U << PMU_INT_CON_PWRMODE_WAKEUP_INT_EN_SHIFT)            /* 0x00000002 */
#define PMU_INT_CON_RF_WAKEUP_INT_EN_SHIFT                 (2U)
#define PMU_INT_CON_RF_WAKEUP_INT_EN_MASK                  (0x1U << PMU_INT_CON_RF_WAKEUP_INT_EN_SHIFT)                 /* 0x00000004 */
#define PMU_INT_CON_MCU_WAKEUP_INT_EN_SHIFT                (3U)
#define PMU_INT_CON_MCU_WAKEUP_INT_EN_MASK                 (0x1U << PMU_INT_CON_MCU_WAKEUP_INT_EN_SHIFT)                /* 0x00000008 */
#define PMU_INT_CON_TIMEOUT_WAKEUP_EN_SHIFT                (4U)
#define PMU_INT_CON_TIMEOUT_WAKEUP_EN_MASK                 (0x1U << PMU_INT_CON_TIMEOUT_WAKEUP_EN_SHIFT)                /* 0x00000010 */
#define PMU_INT_CON_GPIO_INT_WAKEUP_EN_SHIFT               (5U)
#define PMU_INT_CON_GPIO_INT_WAKEUP_EN_MASK                (0x1U << PMU_INT_CON_GPIO_INT_WAKEUP_EN_SHIFT)               /* 0x00000020 */
/* INT_ST */
#define PMU_INT_ST_OFFSET                                  (0x34)
#define PMU_INT_ST_PWRMODE_WAKEUP_STATUS_SHIFT             (0U)
#define PMU_INT_ST_PWRMODE_WAKEUP_STATUS_MASK              (0x1U << PMU_INT_ST_PWRMODE_WAKEUP_STATUS_SHIFT)             /* 0x00000001 */
#define PMU_INT_ST_PWRMODE_WAKEUP_GPIO_POS_STATUS_SHIFT    (1U)
#define PMU_INT_ST_PWRMODE_WAKEUP_GPIO_POS_STATUS_MASK     (0x1U << PMU_INT_ST_PWRMODE_WAKEUP_GPIO_POS_STATUS_SHIFT)    /* 0x00000002 */
#define PMU_INT_ST_PWRMODE_WAKEUP_GPIO_NEG_STATUS_SHIFT    (2U)
#define PMU_INT_ST_PWRMODE_WAKEUP_GPIO_NEG_STATUS_MASK     (0x1U << PMU_INT_ST_PWRMODE_WAKEUP_GPIO_NEG_STATUS_SHIFT)    /* 0x00000004 */
#define PMU_INT_ST_PWRMODE_WAKEUP_GPIO_INT_STATUS_SHIFT    (3U)
#define PMU_INT_ST_PWRMODE_WAKEUP_GPIO_INT_STATUS_MASK     (0x1U << PMU_INT_ST_PWRMODE_WAKEUP_GPIO_INT_STATUS_SHIFT)    /* 0x00000008 */
#define PMU_INT_ST_TIMEOUT_WAKEUP_STATUS_SHIFT             (4U)
#define PMU_INT_ST_TIMEOUT_WAKEUP_STATUS_MASK              (0x1U << PMU_INT_ST_TIMEOUT_WAKEUP_STATUS_SHIFT)             /* 0x00000010 */
#define PMU_INT_ST_RF_WAKEUP_STATUS_SHIFT                  (5U)
#define PMU_INT_ST_RF_WAKEUP_STATUS_MASK                   (0x1U << PMU_INT_ST_RF_WAKEUP_STATUS_SHIFT)                  /* 0x00000020 */
#define PMU_INT_ST_MCU_INT_WAKEUP_STATUS_SHIFT             (6U)
#define PMU_INT_ST_MCU_INT_WAKEUP_STATUS_MASK              (0x1U << PMU_INT_ST_MCU_INT_WAKEUP_STATUS_SHIFT)             /* 0x00000040 */
/* PWRMODE_GPIO_POS_INT_CON */
#define PMU_PWRMODE_GPIO_POS_INT_CON_OFFSET                (0x38)
#define PMU_PWRMODE_GPIO_POS_INT_CON_PWRMODE_GPIO0_POS_INT_EN_SHIFT (0U)
#define PMU_PWRMODE_GPIO_POS_INT_CON_PWRMODE_GPIO0_POS_INT_EN_MASK (0x1U << PMU_PWRMODE_GPIO_POS_INT_CON_PWRMODE_GPIO0_POS_INT_EN_SHIFT) /* 0x00000001 */
/* PWRMODE_GPIO_NEG_INT_CON */
#define PMU_PWRMODE_GPIO_NEG_INT_CON_OFFSET                (0x3C)
#define PMU_PWRMODE_GPIO_NEG_INT_CON_PWRMODE_GPIO0D_NEG_INT_EN_SHIFT (0U)
#define PMU_PWRMODE_GPIO_NEG_INT_CON_PWRMODE_GPIO0D_NEG_INT_EN_MASK (0x1U << PMU_PWRMODE_GPIO_NEG_INT_CON_PWRMODE_GPIO0D_NEG_INT_EN_SHIFT) /* 0x00000001 */
/* PWRMODE_GPIO_POS_INT_ST */
#define PMU_PWRMODE_GPIO_POS_INT_ST_OFFSET                 (0x40)
#define PMU_PWRMODE_GPIO_POS_INT_ST_PWRMODE_GPIO0_POS_INT_STATUS_SHIFT (0U)
#define PMU_PWRMODE_GPIO_POS_INT_ST_PWRMODE_GPIO0_POS_INT_STATUS_MASK (0xFFU << PMU_PWRMODE_GPIO_POS_INT_ST_PWRMODE_GPIO0_POS_INT_STATUS_SHIFT) /* 0x000000FF */
/* PWRMODE_GPIO_NEG_INT_ST */
#define PMU_PWRMODE_GPIO_NEG_INT_ST_OFFSET                 (0x44)
#define PMU_PWRMODE_GPIO_NEG_INT_ST_PWRMODE_GPIO0_NEG_INT_STATUS_SHIFT (0U)
#define PMU_PWRMODE_GPIO_NEG_INT_ST_PWRMODE_GPIO0_NEG_INT_STATUS_MASK (0x1U << PMU_PWRMODE_GPIO_NEG_INT_ST_PWRMODE_GPIO0_NEG_INT_STATUS_SHIFT) /* 0x00000001 */
/* PWRDN_INTEN */
#define PMU_PWRDN_INTEN_OFFSET                             (0x48)
#define PMU_PWRDN_INTEN_PD_DSP_PWR_SWITCH_INT_EN_SHIFT     (0U)
#define PMU_PWRDN_INTEN_PD_DSP_PWR_SWITCH_INT_EN_MASK      (0x1U << PMU_PWRDN_INTEN_PD_DSP_PWR_SWITCH_INT_EN_SHIFT)     /* 0x00000001 */
#define PMU_PWRDN_INTEN_PD_MCU_PWR_SWITCH_INT_EN_SHIFT     (1U)
#define PMU_PWRDN_INTEN_PD_MCU_PWR_SWITCH_INT_EN_MASK      (0x1U << PMU_PWRDN_INTEN_PD_MCU_PWR_SWITCH_INT_EN_SHIFT)     /* 0x00000002 */
#define PMU_PWRDN_INTEN_PD_BT_PWR_SWITCH_INT_EN_SHIFT      (2U)
#define PMU_PWRDN_INTEN_PD_BT_PWR_SWITCH_INT_EN_MASK       (0x1U << PMU_PWRDN_INTEN_PD_BT_PWR_SWITCH_INT_EN_SHIFT)      /* 0x00000004 */
/* PWRDN_STATUS */
#define PMU_PWRDN_STATUS_OFFSET                            (0x4C)
#define PMU_PWRDN_STATUS_PD_DSP_PD_SWT_SHIFT               (0U)
#define PMU_PWRDN_STATUS_PD_DSP_PD_SWT_MASK                (0x1U << PMU_PWRDN_STATUS_PD_DSP_PD_SWT_SHIFT)               /* 0x00000001 */
#define PMU_PWRDN_STATUS_PD_MCU_PD_SWT_SHIFT               (1U)
#define PMU_PWRDN_STATUS_PD_MCU_PD_SWT_MASK                (0x1U << PMU_PWRDN_STATUS_PD_MCU_PD_SWT_SHIFT)               /* 0x00000002 */
#define PMU_PWRDN_STATUS_PD_BT_PD_SWT_SHIFT                (2U)
#define PMU_PWRDN_STATUS_PD_BT_PD_SWT_MASK                 (0x1U << PMU_PWRDN_STATUS_PD_BT_PD_SWT_SHIFT)                /* 0x00000004 */
/* WAKEUP_STATUS */
#define PMU_WAKEUP_STATUS_OFFSET                           (0x50)
#define PMU_WAKEUP_STATUS_WAKEUP_MCU_INT_STATUS_SHIFT      (0U)
#define PMU_WAKEUP_STATUS_WAKEUP_MCU_INT_STATUS_MASK       (0x1U << PMU_WAKEUP_STATUS_WAKEUP_MCU_INT_STATUS_SHIFT)      /* 0x00000001 */
#define PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_GPIO_INT_STATUS_SHIFT (1U)
#define PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_GPIO_INT_STATUS_MASK (0x1U << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_GPIO_INT_STATUS_SHIFT) /* 0x00000002 */
#define PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMEOUT_STATUS_SHIFT (2U)
#define PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMEOUT_STATUS_MASK (0x1U << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_TIMEOUT_STATUS_SHIFT) /* 0x00000004 */
#define PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_RF_STATUS_SHIFT   (3U)
#define PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_RF_STATUS_MASK    (0x1U << PMU_WAKEUP_STATUS_PWRMODE_WAKEUP_RF_STATUS_SHIFT)   /* 0x00000008 */
/* BUS_CLR */
#define PMU_BUS_CLR_OFFSET                                 (0x54)
#define PMU_BUS_CLR_CLR_DSP_SHIFT                          (0U)
#define PMU_BUS_CLR_CLR_DSP_MASK                           (0x1U << PMU_BUS_CLR_CLR_DSP_SHIFT)                          /* 0x00000001 */
#define PMU_BUS_CLR_CLR_MCU_SHIFT                          (1U)
#define PMU_BUS_CLR_CLR_MCU_MASK                           (0x1U << PMU_BUS_CLR_CLR_MCU_SHIFT)                          /* 0x00000002 */
#define PMU_BUS_CLR_CLR_BT_SHIFT                           (2U)
#define PMU_BUS_CLR_CLR_BT_MASK                            (0x1U << PMU_BUS_CLR_CLR_BT_SHIFT)                           /* 0x00000004 */
#define PMU_BUS_CLR_CLR_PMU_SHIFT                          (3U)
#define PMU_BUS_CLR_CLR_PMU_MASK                           (0x1U << PMU_BUS_CLR_CLR_PMU_SHIFT)                          /* 0x00000008 */
#define PMU_BUS_CLR_CLR_TOP_SHIFT                          (4U)
#define PMU_BUS_CLR_CLR_TOP_MASK                           (0x1U << PMU_BUS_CLR_CLR_TOP_SHIFT)                          /* 0x00000010 */
/* BUS_IDLE_REQ */
#define PMU_BUS_IDLE_REQ_OFFSET                            (0x58)
#define PMU_BUS_IDLE_REQ_IDLE_REQ_DSP_SHIFT                (0U)
#define PMU_BUS_IDLE_REQ_IDLE_REQ_DSP_MASK                 (0x1U << PMU_BUS_IDLE_REQ_IDLE_REQ_DSP_SHIFT)                /* 0x00000001 */
#define PMU_BUS_IDLE_REQ_IDLE_REQ_MCU_SHIFT                (1U)
#define PMU_BUS_IDLE_REQ_IDLE_REQ_MCU_MASK                 (0x1U << PMU_BUS_IDLE_REQ_IDLE_REQ_MCU_SHIFT)                /* 0x00000002 */
#define PMU_BUS_IDLE_REQ_IDLE_REQ_BT_SHIFT                 (2U)
#define PMU_BUS_IDLE_REQ_IDLE_REQ_BT_MASK                  (0x1U << PMU_BUS_IDLE_REQ_IDLE_REQ_BT_SHIFT)                 /* 0x00000004 */
#define PMU_BUS_IDLE_REQ_IDLE_REQ_PMU_SHIFT                (3U)
#define PMU_BUS_IDLE_REQ_IDLE_REQ_PMU_MASK                 (0x1U << PMU_BUS_IDLE_REQ_IDLE_REQ_PMU_SHIFT)                /* 0x00000008 */
#define PMU_BUS_IDLE_REQ_IDLE_REQ_TOP_SHIFT                (4U)
#define PMU_BUS_IDLE_REQ_IDLE_REQ_TOP_MASK                 (0x1U << PMU_BUS_IDLE_REQ_IDLE_REQ_TOP_SHIFT)                /* 0x00000010 */
/* BUS_IDLE_ST */
#define PMU_BUS_IDLE_ST_OFFSET                             (0x5C)
#define PMU_BUS_IDLE_ST_IDLE_DSP_SHIFT                     (0U)
#define PMU_BUS_IDLE_ST_IDLE_DSP_MASK                      (0x1U << PMU_BUS_IDLE_ST_IDLE_DSP_SHIFT)                     /* 0x00000001 */
#define PMU_BUS_IDLE_ST_IDLE_MCU_SHIFT                     (1U)
#define PMU_BUS_IDLE_ST_IDLE_MCU_MASK                      (0x1U << PMU_BUS_IDLE_ST_IDLE_MCU_SHIFT)                     /* 0x00000002 */
#define PMU_BUS_IDLE_ST_IDLE_BT_SHIFT                      (2U)
#define PMU_BUS_IDLE_ST_IDLE_BT_MASK                       (0x1U << PMU_BUS_IDLE_ST_IDLE_BT_SHIFT)                      /* 0x00000004 */
#define PMU_BUS_IDLE_ST_IDLE_PMU_SHIFT                     (3U)
#define PMU_BUS_IDLE_ST_IDLE_PMU_MASK                      (0x1U << PMU_BUS_IDLE_ST_IDLE_PMU_SHIFT)                     /* 0x00000008 */
#define PMU_BUS_IDLE_ST_IDLE_TOP_SHIFT                     (4U)
#define PMU_BUS_IDLE_ST_IDLE_TOP_MASK                      (0x1U << PMU_BUS_IDLE_ST_IDLE_TOP_SHIFT)                     /* 0x00000010 */
/* BUS_IDLE_ACK */
#define PMU_BUS_IDLE_ACK_OFFSET                            (0x60)
#define PMU_BUS_IDLE_ACK_IDLE_ACK_DSP_SHIFT                (0U)
#define PMU_BUS_IDLE_ACK_IDLE_ACK_DSP_MASK                 (0x1U << PMU_BUS_IDLE_ACK_IDLE_ACK_DSP_SHIFT)                /* 0x00000001 */
#define PMU_BUS_IDLE_ACK_IDLE_ACK_MCU_SHIFT                (1U)
#define PMU_BUS_IDLE_ACK_IDLE_ACK_MCU_MASK                 (0x1U << PMU_BUS_IDLE_ACK_IDLE_ACK_MCU_SHIFT)                /* 0x00000002 */
#define PMU_BUS_IDLE_ACK_IDLE_ACK_BT_SHIFT                 (2U)
#define PMU_BUS_IDLE_ACK_IDLE_ACK_BT_MASK                  (0x1U << PMU_BUS_IDLE_ACK_IDLE_ACK_BT_SHIFT)                 /* 0x00000004 */
#define PMU_BUS_IDLE_ACK_IDLE_ACK_PMU_SHIFT                (3U)
#define PMU_BUS_IDLE_ACK_IDLE_ACK_PMU_MASK                 (0x1U << PMU_BUS_IDLE_ACK_IDLE_ACK_PMU_SHIFT)                /* 0x00000008 */
#define PMU_BUS_IDLE_ACK_IDLE_ACK_TOP_SHIFT                (4U)
#define PMU_BUS_IDLE_ACK_IDLE_ACK_TOP_MASK                 (0x1U << PMU_BUS_IDLE_ACK_IDLE_ACK_TOP_SHIFT)                /* 0x00000010 */
/* POWER_ST */
#define PMU_POWER_ST_OFFSET                                (0x64)
#define PMU_POWER_ST_PWRMODE_POWER_STATE_SHIFT             (0U)
#define PMU_POWER_ST_PWRMODE_POWER_STATE_MASK              (0x1FU << PMU_POWER_ST_PWRMODE_POWER_STATE_SHIFT)            /* 0x0000001F */
/* CORE_PWR_ST */
#define PMU_CORE_PWR_ST_OFFSET                             (0x68)
#define PMU_CORE_PWR_ST_STANDBYWFI_CM3_SHIFT               (0U)
#define PMU_CORE_PWR_ST_STANDBYWFI_CM3_MASK                (0x1U << PMU_CORE_PWR_ST_STANDBYWFI_CM3_SHIFT)               /* 0x00000001 */
/* PLLLOCK_CNT */
#define PMU_PLLLOCK_CNT_OFFSET                             (0x70)
#define PMU_PLLLOCK_CNT_PMU_PLLLOCK_CNT_SHIFT              (0U)
#define PMU_PLLLOCK_CNT_PMU_PLLLOCK_CNT_MASK               (0xFFFFFU << PMU_PLLLOCK_CNT_PMU_PLLLOCK_CNT_SHIFT)          /* 0x000FFFFF */
/* PWRMODE_TIMEOUT_CNT */
#define PMU_PWRMODE_TIMEOUT_CNT_OFFSET                     (0x74)
#define PMU_PWRMODE_TIMEOUT_CNT_PWRMODE_TIMEOUT_COUNT_SHIFT (0U)
#define PMU_PWRMODE_TIMEOUT_CNT_PWRMODE_TIMEOUT_COUNT_MASK (0xFFFFFFFFU << PMU_PWRMODE_TIMEOUT_CNT_PWRMODE_TIMEOUT_COUNT_SHIFT) /* 0xFFFFFFFF */
/* WAKEUP_RST_CLT_CNT */
#define PMU_WAKEUP_RST_CLT_CNT_OFFSET                      (0x78)
#define PMU_WAKEUP_RST_CLT_CNT_WAKEUP_RST_CLR_CNT_SHIFT    (0U)
#define PMU_WAKEUP_RST_CLT_CNT_WAKEUP_RST_CLR_CNT_MASK     (0xFFFFFU << PMU_WAKEUP_RST_CLT_CNT_WAKEUP_RST_CLR_CNT_SHIFT) /* 0x000FFFFF */
/* PMU_STABLE_CNT */
#define PMU_PMU_STABLE_CNT_OFFSET                          (0x7C)
#define PMU_PMU_STABLE_CNT_PMU_WAKEUP_COUNT_SHIFT          (0U)
#define PMU_PMU_STABLE_CNT_PMU_WAKEUP_COUNT_MASK           (0xFFFFFU << PMU_PMU_STABLE_CNT_PMU_WAKEUP_COUNT_SHIFT)      /* 0x000FFFFF */
/* SYS_REG0 */
#define PMU_SYS_REG0_OFFSET                                (0x80)
#define PMU_SYS_REG0_PMU_SYS_REG0_SHIFT                    (0U)
#define PMU_SYS_REG0_PMU_SYS_REG0_MASK                     (0xFFFFFFFFU << PMU_SYS_REG0_PMU_SYS_REG0_SHIFT)             /* 0xFFFFFFFF */
/* SYS_REG1 */
#define PMU_SYS_REG1_OFFSET                                (0x84)
#define PMU_SYS_REG1_PMU_SYS_REG1_SHIFT                    (0U)
#define PMU_SYS_REG1_PMU_SYS_REG1_MASK                     (0xFFFFFFFFU << PMU_SYS_REG1_PMU_SYS_REG1_SHIFT)             /* 0xFFFFFFFF */
/* SYS_REG2 */
#define PMU_SYS_REG2_OFFSET                                (0x8C)
#define PMU_SYS_REG2_PMU_SYS_REG2_SHIFT                    (0U)
#define PMU_SYS_REG2_PMU_SYS_REG2_MASK                     (0xFFFFFFFFU << PMU_SYS_REG2_PMU_SYS_REG2_SHIFT)             /* 0xFFFFFFFF */
/* SYS_REG3 */
#define PMU_SYS_REG3_OFFSET                                (0x90)
#define PMU_SYS_REG3_PMU_SYS_REG3_SHIFT                    (0U)
#define PMU_SYS_REG3_PMU_SYS_REG3_MASK                     (0xFFFFFFFFU << PMU_SYS_REG3_PMU_SYS_REG3_SHIFT)             /* 0xFFFFFFFF */
/******************************************I2STDM*******************************************/
/* TXCR */
#define I2STDM_TXCR_OFFSET                                    (0x0)
#define I2STDM_TXCR_VDW_SHIFT                                 (0U)
#define I2STDM_TXCR_VDW_MASK                                  (0x1FU << I2STDM_TXCR_VDW_SHIFT)                                /* 0x0000001F */
#define I2STDM_TXCR_TFS_SHIFT                                 (5U)
#define I2STDM_TXCR_TFS_MASK                                  (0x3U << I2STDM_TXCR_TFS_SHIFT)                                 /* 0x00000060 */
#define I2STDM_TXCR_PBM_SHIFT                                 (7U)
#define I2STDM_TXCR_PBM_MASK                                  (0x3U << I2STDM_TXCR_PBM_SHIFT)                                 /* 0x00000180 */
#define I2STDM_TXCR_IBM_SHIFT                                 (9U)
#define I2STDM_TXCR_IBM_MASK                                  (0x3U << I2STDM_TXCR_IBM_SHIFT)                                 /* 0x00000600 */
#define I2STDM_TXCR_FBM_SHIFT                                 (11U)
#define I2STDM_TXCR_FBM_MASK                                  (0x1U << I2STDM_TXCR_FBM_SHIFT)                                 /* 0x00000800 */
#define I2STDM_TXCR_SJM_SHIFT                                 (12U)
#define I2STDM_TXCR_SJM_MASK                                  (0x1U << I2STDM_TXCR_SJM_SHIFT)                                 /* 0x00001000 */
#define I2STDM_TXCR_HWT_SHIFT                                 (14U)
#define I2STDM_TXCR_HWT_MASK                                  (0x1U << I2STDM_TXCR_HWT_SHIFT)                                 /* 0x00004000 */
#define I2STDM_TXCR_TCSR_SHIFT                                (15U)
#define I2STDM_TXCR_TCSR_MASK                                 (0x3U << I2STDM_TXCR_TCSR_SHIFT)                                /* 0x00018000 */
#define I2STDM_TXCR_RCNT_SHIFT                                (17U)
#define I2STDM_TXCR_RCNT_MASK                                 (0x3FU << I2STDM_TXCR_RCNT_SHIFT)                               /* 0x007E0000 */
#define I2STDM_TXCR_TX_PATH_SELECT0_SHIFT                     (23U)
#define I2STDM_TXCR_TX_PATH_SELECT0_MASK                      (0x3U << I2STDM_TXCR_TX_PATH_SELECT0_SHIFT)                     /* 0x01800000 */
#define I2STDM_TXCR_TX_PATH_SELECT1_SHIFT                     (25U)
#define I2STDM_TXCR_TX_PATH_SELECT1_MASK                      (0x3U << I2STDM_TXCR_TX_PATH_SELECT1_SHIFT)                     /* 0x06000000 */
#define I2STDM_TXCR_TX_PATH_SELECT2_SHIFT                     (27U)
#define I2STDM_TXCR_TX_PATH_SELECT2_MASK                      (0x3U << I2STDM_TXCR_TX_PATH_SELECT2_SHIFT)                     /* 0x18000000 */
#define I2STDM_TXCR_TX_PATH_SELECT3_SHIFT                     (29U)
#define I2STDM_TXCR_TX_PATH_SELECT3_MASK                      (0x3U << I2STDM_TXCR_TX_PATH_SELECT3_SHIFT)                     /* 0x60000000 */
/* RXCR */
#define I2STDM_RXCR_OFFSET                                    (0x4)
#define I2STDM_RXCR_VDW_SHIFT                                 (0U)
#define I2STDM_RXCR_VDW_MASK                                  (0x1FU << I2STDM_RXCR_VDW_SHIFT)                                /* 0x0000001F */
#define I2STDM_RXCR_TFS_SHIFT                                 (5U)
#define I2STDM_RXCR_TFS_MASK                                  (0x3U << I2STDM_RXCR_TFS_SHIFT)                                 /* 0x00000060 */
#define I2STDM_RXCR_PBM_SHIFT                                 (7U)
#define I2STDM_RXCR_PBM_MASK                                  (0x3U << I2STDM_RXCR_PBM_SHIFT)                                 /* 0x00000180 */
#define I2STDM_RXCR_IBM_SHIFT                                 (9U)
#define I2STDM_RXCR_IBM_MASK                                  (0x3U << I2STDM_RXCR_IBM_SHIFT)                                 /* 0x00000600 */
#define I2STDM_RXCR_FBM_SHIFT                                 (11U)
#define I2STDM_RXCR_FBM_MASK                                  (0x1U << I2STDM_RXCR_FBM_SHIFT)                                 /* 0x00000800 */
#define I2STDM_RXCR_SJM_SHIFT                                 (12U)
#define I2STDM_RXCR_SJM_MASK                                  (0x1U << I2STDM_RXCR_SJM_SHIFT)                                 /* 0x00001000 */
#define I2STDM_RXCR_HWT_SHIFT                                 (14U)
#define I2STDM_RXCR_HWT_MASK                                  (0x1U << I2STDM_RXCR_HWT_SHIFT)                                 /* 0x00004000 */
#define I2STDM_RXCR_RCSR_SHIFT                                (15U)
#define I2STDM_RXCR_RCSR_MASK                                 (0x3U << I2STDM_RXCR_RCSR_SHIFT)                                /* 0x00018000 */
#define I2STDM_RXCR_RX_PATH_SELECT0_SHIFT                     (17U)
#define I2STDM_RXCR_RX_PATH_SELECT0_MASK                      (0x3U << I2STDM_RXCR_RX_PATH_SELECT0_SHIFT)                     /* 0x00060000 */
#define I2STDM_RXCR_RX_PATH_SELECT1_SHIFT                     (19U)
#define I2STDM_RXCR_RX_PATH_SELECT1_MASK                      (0x3U << I2STDM_RXCR_RX_PATH_SELECT1_SHIFT)                     /* 0x00180000 */
#define I2STDM_RXCR_RX_PATH_SELECT2_SHIFT                     (21U)
#define I2STDM_RXCR_RX_PATH_SELECT2_MASK                      (0x3U << I2STDM_RXCR_RX_PATH_SELECT2_SHIFT)                     /* 0x00600000 */
#define I2STDM_RXCR_RX_PATH_SELECT3_SHIFT                     (23U)
#define I2STDM_RXCR_RX_PATH_SELECT3_MASK                      (0x3U << I2STDM_RXCR_RX_PATH_SELECT3_SHIFT)                     /* 0x01800000 */
/* CKR */
#define I2STDM_CKR_OFFSET                                     (0x8)
#define I2STDM_CKR_TSD_SHIFT                                  (0U)
#define I2STDM_CKR_TSD_MASK                                   (0xFFU << I2STDM_CKR_TSD_SHIFT)                                 /* 0x000000FF */
#define I2STDM_CKR_RSD_SHIFT                                  (8U)
#define I2STDM_CKR_RSD_MASK                                   (0xFFU << I2STDM_CKR_RSD_SHIFT)                                 /* 0x0000FF00 */
#define I2STDM_CKR_TLP_SHIFT                                  (24U)
#define I2STDM_CKR_TLP_MASK                                   (0x1U << I2STDM_CKR_TLP_SHIFT)                                  /* 0x01000000 */
#define I2STDM_CKR_RLP_SHIFT                                  (25U)
#define I2STDM_CKR_RLP_MASK                                   (0x1U << I2STDM_CKR_RLP_SHIFT)                                  /* 0x02000000 */
#define I2STDM_CKR_CKP_SHIFT                                  (26U)
#define I2STDM_CKR_CKP_MASK                                   (0x1U << I2STDM_CKR_CKP_SHIFT)                                  /* 0x04000000 */
#define I2STDM_CKR_MSS_SHIFT                                  (27U)
#define I2STDM_CKR_MSS_MASK                                   (0x1U << I2STDM_CKR_MSS_SHIFT)                                  /* 0x08000000 */
#define I2STDM_CKR_LRCK_COMMON_SHIFT                          (28U)
#define I2STDM_CKR_LRCK_COMMON_MASK                           (0x3U << I2STDM_CKR_LRCK_COMMON_SHIFT)                          /* 0x30000000 */
/* TXFIFOLR */
#define I2STDM_TXFIFOLR_OFFSET                                (0xC)
#define I2STDM_TXFIFOLR_TFL0_SHIFT                            (0U)
#define I2STDM_TXFIFOLR_TFL0_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL0_SHIFT)                           /* 0x0000003F */
#define I2STDM_TXFIFOLR_TFL1_SHIFT                            (6U)
#define I2STDM_TXFIFOLR_TFL1_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL1_SHIFT)                           /* 0x00000FC0 */
#define I2STDM_TXFIFOLR_TFL2_SHIFT                            (12U)
#define I2STDM_TXFIFOLR_TFL2_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL2_SHIFT)                           /* 0x0003F000 */
#define I2STDM_TXFIFOLR_TFL3_SHIFT                            (18U)
#define I2STDM_TXFIFOLR_TFL3_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL3_SHIFT)                           /* 0x00FC0000 */
/* DMACR */
#define I2STDM_DMACR_OFFSET                                   (0x10)
#define I2STDM_DMACR_TDL_SHIFT                                (0U)
#define I2STDM_DMACR_TDL_MASK                                 (0x1FU << I2STDM_DMACR_TDL_SHIFT)                               /* 0x0000001F */
#define I2STDM_DMACR_TDE_SHIFT                                (8U)
#define I2STDM_DMACR_TDE_MASK                                 (0x1U << I2STDM_DMACR_TDE_SHIFT)                                /* 0x00000100 */
#define I2STDM_DMACR_RDL_SHIFT                                (16U)
#define I2STDM_DMACR_RDL_MASK                                 (0x1FU << I2STDM_DMACR_RDL_SHIFT)                               /* 0x001F0000 */
#define I2STDM_DMACR_RDE_SHIFT                                (24U)
#define I2STDM_DMACR_RDE_MASK                                 (0x1U << I2STDM_DMACR_RDE_SHIFT)                                /* 0x01000000 */
/* INTCR */
#define I2STDM_INTCR_OFFSET                                   (0x14)
#define I2STDM_INTCR_TXEIE_SHIFT                              (0U)
#define I2STDM_INTCR_TXEIE_MASK                               (0x1U << I2STDM_INTCR_TXEIE_SHIFT)                              /* 0x00000001 */
#define I2STDM_INTCR_TXUIE_SHIFT                              (1U)
#define I2STDM_INTCR_TXUIE_MASK                               (0x1U << I2STDM_INTCR_TXUIE_SHIFT)                              /* 0x00000002 */
#define I2STDM_INTCR_TXUIC_SHIFT                              (2U)
#define I2STDM_INTCR_TXUIC_MASK                               (0x1U << I2STDM_INTCR_TXUIC_SHIFT)                              /* 0x00000004 */
#define I2STDM_INTCR_TFT_SHIFT                                (4U)
#define I2STDM_INTCR_TFT_MASK                                 (0x1FU << I2STDM_INTCR_TFT_SHIFT)                               /* 0x000001F0 */
#define I2STDM_INTCR_RXFIE_SHIFT                              (16U)
#define I2STDM_INTCR_RXFIE_MASK                               (0x1U << I2STDM_INTCR_RXFIE_SHIFT)                              /* 0x00010000 */
#define I2STDM_INTCR_RXOIE_SHIFT                              (17U)
#define I2STDM_INTCR_RXOIE_MASK                               (0x1U << I2STDM_INTCR_RXOIE_SHIFT)                              /* 0x00020000 */
#define I2STDM_INTCR_RXOIC_SHIFT                              (18U)
#define I2STDM_INTCR_RXOIC_MASK                               (0x1U << I2STDM_INTCR_RXOIC_SHIFT)                              /* 0x00040000 */
#define I2STDM_INTCR_RFT_SHIFT                                (20U)
#define I2STDM_INTCR_RFT_MASK                                 (0x1FU << I2STDM_INTCR_RFT_SHIFT)                               /* 0x01F00000 */
/* INTSR */
#define I2STDM_INTSR_OFFSET                                   (0x18)
#define I2STDM_INTSR_TXEI_SHIFT                               (0U)
#define I2STDM_INTSR_TXEI_MASK                                (0x1U << I2STDM_INTSR_TXEI_SHIFT)                               /* 0x00000001 */
#define I2STDM_INTSR_TXUI_SHIFT                               (1U)
#define I2STDM_INTSR_TXUI_MASK                                (0x1U << I2STDM_INTSR_TXUI_SHIFT)                               /* 0x00000002 */
#define I2STDM_INTSR_RXFI_SHIFT                               (16U)
#define I2STDM_INTSR_RXFI_MASK                                (0x1U << I2STDM_INTSR_RXFI_SHIFT)                               /* 0x00010000 */
#define I2STDM_INTSR_RXOI_SHIFT                               (17U)
#define I2STDM_INTSR_RXOI_MASK                                (0x1U << I2STDM_INTSR_RXOI_SHIFT)                               /* 0x00020000 */
/* XFER */
#define I2STDM_XFER_OFFSET                                    (0x1C)
#define I2STDM_XFER_TXS_SHIFT                                 (0U)
#define I2STDM_XFER_TXS_MASK                                  (0x1U << I2STDM_XFER_TXS_SHIFT)                                 /* 0x00000001 */
#define I2STDM_XFER_RXS_SHIFT                                 (1U)
#define I2STDM_XFER_RXS_MASK                                  (0x1U << I2STDM_XFER_RXS_SHIFT)                                 /* 0x00000002 */
/* CLR */
#define I2STDM_CLR_OFFSET                                     (0x20)
#define I2STDM_CLR_TXC_SHIFT                                  (0U)
#define I2STDM_CLR_TXC_MASK                                   (0x1U << I2STDM_CLR_TXC_SHIFT)                                  /* 0x00000001 */
#define I2STDM_CLR_RXC_SHIFT                                  (1U)
#define I2STDM_CLR_RXC_MASK                                   (0x1U << I2STDM_CLR_RXC_SHIFT)                                  /* 0x00000002 */
/* TXDR */
#define I2STDM_TXDR_OFFSET                                    (0x24)
#define I2STDM_TXDR_TXDR_SHIFT                                (0U)
#define I2STDM_TXDR_TXDR_MASK                                 (0xFFFFFFFFU << I2STDM_TXDR_TXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXDR */
#define I2STDM_RXDR_OFFSET                                    (0x28)
#define I2STDM_RXDR_RXDR_SHIFT                                (0U)
#define I2STDM_RXDR_RXDR_MASK                                 (0xFFFFFFFFU << I2STDM_RXDR_RXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXFIFOLR */
#define I2STDM_RXFIFOLR_OFFSET                                (0x2C)
#define I2STDM_RXFIFOLR_RFL0_SHIFT                            (0U)
#define I2STDM_RXFIFOLR_RFL0_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL0_SHIFT)                           /* 0x0000003F */
#define I2STDM_RXFIFOLR_RFL1_SHIFT                            (6U)
#define I2STDM_RXFIFOLR_RFL1_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL1_SHIFT)                           /* 0x00000FC0 */
#define I2STDM_RXFIFOLR_RFL2_SHIFT                            (12U)
#define I2STDM_RXFIFOLR_RFL2_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL2_SHIFT)                           /* 0x0003F000 */
#define I2STDM_RXFIFOLR_RFL3_SHIFT                            (18U)
#define I2STDM_RXFIFOLR_RFL3_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL3_SHIFT)                           /* 0x00FC0000 */
/* TDM_TXCTRL */
#define I2STDM_TDM_TXCTRL_OFFSET                              (0x30)
#define I2STDM_TDM_TXCTRL_TDM_TX_FRAME_WIDTH_SHIFT            (0U)
#define I2STDM_TDM_TXCTRL_TDM_TX_FRAME_WIDTH_MASK             (0x1FFU << I2STDM_TDM_TXCTRL_TDM_TX_FRAME_WIDTH_SHIFT)          /* 0x000001FF */
#define I2STDM_TDM_TXCTRL_TDM_TX_SLOT_BIT_WIDTH_SHIFT         (9U)
#define I2STDM_TDM_TXCTRL_TDM_TX_SLOT_BIT_WIDTH_MASK          (0x1FU << I2STDM_TDM_TXCTRL_TDM_TX_SLOT_BIT_WIDTH_SHIFT)        /* 0x00003E00 */
#define I2STDM_TDM_TXCTRL_TDM_TX_SHIFT_CTRL_SHIFT             (14U)
#define I2STDM_TDM_TXCTRL_TDM_TX_SHIFT_CTRL_MASK              (0x7U << I2STDM_TDM_TXCTRL_TDM_TX_SHIFT_CTRL_SHIFT)             /* 0x0001C000 */
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL0_SHIFT       (17U)
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL0_MASK        (0x1U << I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL0_SHIFT)       /* 0x00020000 */
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL1_SHIFT       (18U)
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL1_MASK        (0x7U << I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL1_SHIFT)       /* 0x001C0000 */
/* TDM_RXCTRL */
#define I2STDM_TDM_RXCTRL_OFFSET                              (0x34)
#define I2STDM_TDM_RXCTRL_TDM_RX_FRAME_WIDTH_SHIFT            (0U)
#define I2STDM_TDM_RXCTRL_TDM_RX_FRAME_WIDTH_MASK             (0x1FFU << I2STDM_TDM_RXCTRL_TDM_RX_FRAME_WIDTH_SHIFT)          /* 0x000001FF */
#define I2STDM_TDM_RXCTRL_TDM_RX_SLOT_BIT_WIDTH_SHIFT         (9U)
#define I2STDM_TDM_RXCTRL_TDM_RX_SLOT_BIT_WIDTH_MASK          (0x1FU << I2STDM_TDM_RXCTRL_TDM_RX_SLOT_BIT_WIDTH_SHIFT)        /* 0x00003E00 */
#define I2STDM_TDM_RXCTRL_TDM_RX_SHIFT_CTRL_SHIFT             (14U)
#define I2STDM_TDM_RXCTRL_TDM_RX_SHIFT_CTRL_MASK              (0x7U << I2STDM_TDM_RXCTRL_TDM_RX_SHIFT_CTRL_SHIFT)             /* 0x0001C000 */
#define I2STDM_TDM_RXCTRL_RX_TDM_FSYNC_WIDTH_SEL0_SHIFT       (17U)
#define I2STDM_TDM_RXCTRL_RX_TDM_FSYNC_WIDTH_SEL0_MASK        (0x1U << I2STDM_TDM_RXCTRL_RX_TDM_FSYNC_WIDTH_SEL0_SHIFT)       /* 0x00020000 */
#define I2STDM_TDM_RXCTRL_RX_TDM_FSYNC_WIDTH_SEL1_SHIFT       (18U)
#define I2STDM_TDM_RXCTRL_RX_TDM_FSYNC_WIDTH_SEL1_MASK        (0x7U << I2STDM_TDM_RXCTRL_RX_TDM_FSYNC_WIDTH_SEL1_SHIFT)       /* 0x001C0000 */
/* CLKDIV */
#define I2STDM_CLKDIV_OFFSET                                  (0x38)
#define I2STDM_CLKDIV_TX_MDIV_SHIFT                           (0U)
#define I2STDM_CLKDIV_TX_MDIV_MASK                            (0xFFU << I2STDM_CLKDIV_TX_MDIV_SHIFT)                          /* 0x000000FF */
#define I2STDM_CLKDIV_RX_MDIV_SHIFT                           (8U)
#define I2STDM_CLKDIV_RX_MDIV_MASK                            (0xFFU << I2STDM_CLKDIV_RX_MDIV_SHIFT)                          /* 0x0000FF00 */
/* VERSION */
#define I2STDM_VERSION_OFFSET                                 (0x3C)
#define I2STDM_VERSION_I2S_VERSION_SHIFT                      (0U)
#define I2STDM_VERSION_I2S_VERSION_MASK                       (0xFFFFFFFFU << I2STDM_VERSION_I2S_VERSION_SHIFT)               /* 0xFFFFFFFF */
/******************************************PDM*******************************************/
/* SYSCONFIG */
#define PDM_SYSCONFIG_OFFSET                               (0x0)
#define PDM_SYSCONFIG_RX_CLR_SHIFT                         (0U)
#define PDM_SYSCONFIG_RX_CLR_MASK                          (0x1U << PDM_SYSCONFIG_RX_CLR_SHIFT)                         /* 0x00000001 */
#define PDM_SYSCONFIG_RX_START_SHIFT                       (2U)
#define PDM_SYSCONFIG_RX_START_MASK                        (0x1U << PDM_SYSCONFIG_RX_START_SHIFT)                       /* 0x00000004 */
/* CTRL0 */
#define PDM_CTRL0_OFFSET                                   (0x4)
#define PDM_CTRL0_DATA_VLD_WIDTH_SHIFT                     (0U)
#define PDM_CTRL0_DATA_VLD_WIDTH_MASK                      (0x1FU << PDM_CTRL0_DATA_VLD_WIDTH_SHIFT)                    /* 0x0000001F */
#define PDM_CTRL0_SAMPLE_RATE_SEL_SHIFT                    (5U)
#define PDM_CTRL0_SAMPLE_RATE_SEL_MASK                     (0x7U << PDM_CTRL0_SAMPLE_RATE_SEL_SHIFT)                    /* 0x000000E0 */
#define PDM_CTRL0_INT_DIV_CON_SHIFT                        (8U)
#define PDM_CTRL0_INT_DIV_CON_MASK                         (0xFFU << PDM_CTRL0_INT_DIV_CON_SHIFT)                       /* 0x0000FF00 */
#define PDM_CTRL0_CLK20X_DIV_CON_SHIFT                     (16U)
#define PDM_CTRL0_CLK20X_DIV_CON_MASK                      (0xFFU << PDM_CTRL0_CLK20X_DIV_CON_SHIFT)                    /* 0x00FF0000 */
#define PDM_CTRL0_SIG_SCALE_MODE_SHIFT                     (24U)
#define PDM_CTRL0_SIG_SCALE_MODE_MASK                      (0x1U << PDM_CTRL0_SIG_SCALE_MODE_SHIFT)                     /* 0x01000000 */
#define PDM_CTRL0_FILTER_GATE_EN_SHIFT                     (25U)
#define PDM_CTRL0_FILTER_GATE_EN_MASK                      (0x1U << PDM_CTRL0_FILTER_GATE_EN_SHIFT)                     /* 0x02000000 */
#define PDM_CTRL0_HWT_EN_SHIFT                             (26U)
#define PDM_CTRL0_HWT_EN_MASK                              (0x1U << PDM_CTRL0_HWT_EN_SHIFT)                             /* 0x04000000 */
#define PDM_CTRL0_PATH0_EN_SHIFT                           (27U)
#define PDM_CTRL0_PATH0_EN_MASK                            (0x1U << PDM_CTRL0_PATH0_EN_SHIFT)                           /* 0x08000000 */
#define PDM_CTRL0_PATH1_EN_SHIFT                           (28U)
#define PDM_CTRL0_PATH1_EN_MASK                            (0x1U << PDM_CTRL0_PATH1_EN_SHIFT)                           /* 0x10000000 */
#define PDM_CTRL0_PATH2_EN_SHIFT                           (29U)
#define PDM_CTRL0_PATH2_EN_MASK                            (0x1U << PDM_CTRL0_PATH2_EN_SHIFT)                           /* 0x20000000 */
#define PDM_CTRL0_PATH3_EN_SHIFT                           (30U)
#define PDM_CTRL0_PATH3_EN_MASK                            (0x1U << PDM_CTRL0_PATH3_EN_SHIFT)                           /* 0x40000000 */
#define PDM_CTRL0_SJM_SEL_SHIFT                            (31U)
#define PDM_CTRL0_SJM_SEL_MASK                             (0x1U << PDM_CTRL0_SJM_SEL_SHIFT)                            /* 0x80000000 */
/* CTRL1 */
#define PDM_CTRL1_OFFSET                                   (0x8)
#define PDM_CTRL1_FRAC_DIV_DENOMONATOR_SHIFT               (0U)
#define PDM_CTRL1_FRAC_DIV_DENOMONATOR_MASK                (0xFFFFU << PDM_CTRL1_FRAC_DIV_DENOMONATOR_SHIFT)            /* 0x0000FFFF */
#define PDM_CTRL1_FRAC_DIV_NUMERATOR_SHIFT                 (16U)
#define PDM_CTRL1_FRAC_DIV_NUMERATOR_MASK                  (0xFFFFU << PDM_CTRL1_FRAC_DIV_NUMERATOR_SHIFT)              /* 0xFFFF0000 */
/* CLK_CTRL */
#define PDM_CLK_CTRL_OFFSET                                (0xC)
#define PDM_CLK_CTRL_CIC_DS_RATIO_SHIFT                    (0U)
#define PDM_CLK_CTRL_CIC_DS_RATIO_MASK                     (0x3U << PDM_CLK_CTRL_CIC_DS_RATIO_SHIFT)                    /* 0x00000003 */
#define PDM_CLK_CTRL_FIR_COM_BPS_SHIFT                     (2U)
#define PDM_CLK_CTRL_FIR_COM_BPS_MASK                      (0x1U << PDM_CLK_CTRL_FIR_COM_BPS_SHIFT)                     /* 0x00000004 */
#define PDM_CLK_CTRL_CLK_POLAR_SHIFT                       (3U)
#define PDM_CLK_CTRL_CLK_POLAR_MASK                        (0x1U << PDM_CLK_CTRL_CLK_POLAR_SHIFT)                       /* 0x00000008 */
#define PDM_CLK_CTRL_DIV_TYPE_SEL_SHIFT                    (4U)
#define PDM_CLK_CTRL_DIV_TYPE_SEL_MASK                     (0x1U << PDM_CLK_CTRL_DIV_TYPE_SEL_SHIFT)                    /* 0x00000010 */
#define PDM_CLK_CTRL_PDM_CLK_EN_SHIFT                      (5U)
#define PDM_CLK_CTRL_PDM_CLK_EN_MASK                       (0x1U << PDM_CLK_CTRL_PDM_CLK_EN_SHIFT)                      /* 0x00000020 */
/* HPF_CTRL */
#define PDM_HPF_CTRL_OFFSET                                (0x10)
#define PDM_HPF_CTRL_HPF_CF_SHIFT                          (0U)
#define PDM_HPF_CTRL_HPF_CF_MASK                           (0x3U << PDM_HPF_CTRL_HPF_CF_SHIFT)                          /* 0x00000003 */
#define PDM_HPF_CTRL_HPFRE_SHIFT                           (2U)
#define PDM_HPF_CTRL_HPFRE_MASK                            (0x1U << PDM_HPF_CTRL_HPFRE_SHIFT)                           /* 0x00000004 */
#define PDM_HPF_CTRL_HPFLE_SHIFT                           (3U)
#define PDM_HPF_CTRL_HPFLE_MASK                            (0x1U << PDM_HPF_CTRL_HPFLE_SHIFT)                           /* 0x00000008 */
/* FIFO_CTRL */
#define PDM_FIFO_CTRL_OFFSET                               (0x14)
#define PDM_FIFO_CTRL_RFL_SHIFT                            (0U)
#define PDM_FIFO_CTRL_RFL_MASK                             (0xFFU << PDM_FIFO_CTRL_RFL_SHIFT)                           /* 0x000000FF */
#define PDM_FIFO_CTRL_RFT_SHIFT                            (8U)
#define PDM_FIFO_CTRL_RFT_MASK                             (0x7FU << PDM_FIFO_CTRL_RFT_SHIFT)                           /* 0x00007F00 */
/* DMA_CTRL */
#define PDM_DMA_CTRL_OFFSET                                (0x18)
#define PDM_DMA_CTRL_RDL_SHIFT                             (0U)
#define PDM_DMA_CTRL_RDL_MASK                              (0x7FU << PDM_DMA_CTRL_RDL_SHIFT)                            /* 0x0000007F */
#define PDM_DMA_CTRL_RDE_SHIFT                             (8U)
#define PDM_DMA_CTRL_RDE_MASK                              (0x1U << PDM_DMA_CTRL_RDE_SHIFT)                             /* 0x00000100 */
/* INT_EN */
#define PDM_INT_EN_OFFSET                                  (0x1C)
#define PDM_INT_EN_RXTIE_SHIFT                             (0U)
#define PDM_INT_EN_RXTIE_MASK                              (0x1U << PDM_INT_EN_RXTIE_SHIFT)                             /* 0x00000001 */
#define PDM_INT_EN_RXOIE_SHIFT                             (1U)
#define PDM_INT_EN_RXOIE_MASK                              (0x1U << PDM_INT_EN_RXOIE_SHIFT)                             /* 0x00000002 */
/* INT_CLR */
#define PDM_INT_CLR_OFFSET                                 (0x20)
#define PDM_INT_CLR_RXOIC_SHIFT                            (1U)
#define PDM_INT_CLR_RXOIC_MASK                             (0x1U << PDM_INT_CLR_RXOIC_SHIFT)                            /* 0x00000002 */
/* INT_ST */
#define PDM_INT_ST_OFFSET                                  (0x24)
#define PDM_INT_ST_RXFI_SHIFT                              (0U)
#define PDM_INT_ST_RXFI_MASK                               (0x1U << PDM_INT_ST_RXFI_SHIFT)                              /* 0x00000001 */
#define PDM_INT_ST_RXOI_SHIFT                              (1U)
#define PDM_INT_ST_RXOI_MASK                               (0x1U << PDM_INT_ST_RXOI_SHIFT)                              /* 0x00000002 */
/* RXFIFO_DATA_REG */
#define PDM_RXFIFO_DATA_REG_OFFSET                         (0x30)
#define PDM_RXFIFO_DATA_REG_RXDR_SHIFT                     (0U)
#define PDM_RXFIFO_DATA_REG_RXDR_MASK                      (0xFFFFFFFFU << PDM_RXFIFO_DATA_REG_RXDR_SHIFT)              /* 0xFFFFFFFF */
/* DATA0R_REG */
#define PDM_DATA0R_REG_OFFSET                              (0x34)
#define PDM_DATA0R_REG_DATA0R_SHIFT                        (0U)
#define PDM_DATA0R_REG_DATA0R_MASK                         (0xFFFFFFFFU << PDM_DATA0R_REG_DATA0R_SHIFT)                 /* 0xFFFFFFFF */
/* DATA0L_REG */
#define PDM_DATA0L_REG_OFFSET                              (0x38)
#define PDM_DATA0L_REG_DATA0L_SHIFT                        (0U)
#define PDM_DATA0L_REG_DATA0L_MASK                         (0xFFFFFFFFU << PDM_DATA0L_REG_DATA0L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA1R_REG */
#define PDM_DATA1R_REG_OFFSET                              (0x3C)
#define PDM_DATA1R_REG_DATA1R_SHIFT                        (0U)
#define PDM_DATA1R_REG_DATA1R_MASK                         (0x1U << PDM_DATA1R_REG_DATA1R_SHIFT)                        /* 0x00000001 */
/* DATA1L_REG */
#define PDM_DATA1L_REG_OFFSET                              (0x40)
#define PDM_DATA1L_REG_DATA1L_SHIFT                        (0U)
#define PDM_DATA1L_REG_DATA1L_MASK                         (0xFFFFFFFFU << PDM_DATA1L_REG_DATA1L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA2R_REG */
#define PDM_DATA2R_REG_OFFSET                              (0x44)
#define PDM_DATA2R_REG_DATA2R_SHIFT                        (0U)
#define PDM_DATA2R_REG_DATA2R_MASK                         (0xFFFFFFFFU << PDM_DATA2R_REG_DATA2R_SHIFT)                 /* 0xFFFFFFFF */
/* DATA2L_REG */
#define PDM_DATA2L_REG_OFFSET                              (0x48)
#define PDM_DATA2L_REG_DATA2L_SHIFT                        (0U)
#define PDM_DATA2L_REG_DATA2L_MASK                         (0xFFFFFFFFU << PDM_DATA2L_REG_DATA2L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA3R_REG */
#define PDM_DATA3R_REG_OFFSET                              (0x4C)
#define PDM_DATA3R_REG_DATA3R_SHIFT                        (0U)
#define PDM_DATA3R_REG_DATA3R_MASK                         (0xFFFFFFFFU << PDM_DATA3R_REG_DATA3R_SHIFT)                 /* 0xFFFFFFFF */
/* DATA3L_REG */
#define PDM_DATA3L_REG_OFFSET                              (0x50)
#define PDM_DATA3L_REG_DATA3L_SHIFT                        (0U)
#define PDM_DATA3L_REG_DATA3L_MASK                         (0xFFFFFFFFU << PDM_DATA3L_REG_DATA3L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA_VALID */
#define PDM_DATA_VALID_OFFSET                              (0x54)
#define PDM_DATA_VALID_PATH3_VLD_SHIFT                     (0U)
#define PDM_DATA_VALID_PATH3_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH3_VLD_SHIFT)                     /* 0x00000001 */
#define PDM_DATA_VALID_PATH2_VLD_SHIFT                     (1U)
#define PDM_DATA_VALID_PATH2_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH2_VLD_SHIFT)                     /* 0x00000002 */
#define PDM_DATA_VALID_PATH1_VLD_SHIFT                     (2U)
#define PDM_DATA_VALID_PATH1_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH1_VLD_SHIFT)                     /* 0x00000004 */
#define PDM_DATA_VALID_PATH0_VLD_SHIFT                     (3U)
#define PDM_DATA_VALID_PATH0_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH0_VLD_SHIFT)                     /* 0x00000008 */
/* VERSION */
#define PDM_VERSION_OFFSET                                 (0x58)
#define PDM_VERSION_VERSION_SHIFT                          (0U)
#define PDM_VERSION_VERSION_MASK                           (0xFFFFFFFFU << PDM_VERSION_VERSION_SHIFT)                   /* 0xFFFFFFFF */
/* INCR_RXDR */
#define PDM_INCR_RXDR_OFFSET                               (0x400)
#define PDM_INCR_RXDR_RECEIVE_FIFO_DATA_SHIFT              (0U)
#define PDM_INCR_RXDR_RECEIVE_FIFO_DATA_MASK               (0xFFFFFFFFU << PDM_INCR_RXDR_RECEIVE_FIFO_DATA_SHIFT)       /* 0xFFFFFFFF */
/******************************************CRU*******************************************/
/* MODE_CON */
#define CRU_MODE_CON_OFFSET                                (0xC)
#define CRU_MODE_CON_CPLL_SLOWMODE_SHIFT                   (0U)
#define CRU_MODE_CON_CPLL_SLOWMODE_MASK                    (0x3U << CRU_MODE_CON_CPLL_SLOWMODE_SHIFT)                   /* 0x00000003 */
#define CRU_MODE_CON_GPLL_SLOWMODE_SHIFT                   (2U)
#define CRU_MODE_CON_GPLL_SLOWMODE_MASK                    (0x3U << CRU_MODE_CON_GPLL_SLOWMODE_SHIFT)                   /* 0x0000000C */
/* CPLL_CON0 */
#define CRU_CPLL_CON0_OFFSET                               (0x60)
#define CRU_CPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_CPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_CPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_CPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_CPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_CPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_CPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_CPLL_CON0_BYPASS_MASK                          (0x1U << CRU_CPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* CPLL_CON1 */
#define CRU_CPLL_CON1_OFFSET                               (0x64)
#define CRU_CPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_CPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_CPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_CPLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_CPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_CPLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_CPLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_CPLL_CON1_DSMPD_MASK                           (0x1U << CRU_CPLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_CPLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_CPLL_CON1_PLLPD0_MASK                          (0x1U << CRU_CPLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_CPLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_CPLL_CON1_PLLPD1_MASK                          (0x1U << CRU_CPLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_CPLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_CPLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_CPLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* CPLL_CON2 */
#define CRU_CPLL_CON2_OFFSET                               (0x68)
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
#define CRU_CPLL_CON3_OFFSET                               (0x6C)
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
#define CRU_CPLL_CON4_OFFSET                               (0x70)
#define CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* CPLL_STAT0 */
#define CRU_CPLL_STAT0_OFFSET                              (0x74)
#define CRU_CPLL_STAT0_LOCK_SHIFT                          (0U)
#define CRU_CPLL_STAT0_LOCK_MASK                           (0x1U << CRU_CPLL_STAT0_LOCK_SHIFT)                          /* 0x00000001 */
/* GPLL_CON0 */
#define CRU_GPLL_CON0_OFFSET                               (0x80)
#define CRU_GPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_GPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_GPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_GPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_GPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_GPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_GPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_GPLL_CON0_BYPASS_MASK                          (0x1U << CRU_GPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* GPLL_CON1 */
#define CRU_GPLL_CON1_OFFSET                               (0x84)
#define CRU_GPLL_CON1_REFDIV_SHIFT                         (0U)
#define CRU_GPLL_CON1_REFDIV_MASK                          (0x3FU << CRU_GPLL_CON1_REFDIV_SHIFT)                        /* 0x0000003F */
#define CRU_GPLL_CON1_POSTDIV2_SHIFT                       (6U)
#define CRU_GPLL_CON1_POSTDIV2_MASK                        (0x7U << CRU_GPLL_CON1_POSTDIV2_SHIFT)                       /* 0x000001C0 */
#define CRU_GPLL_CON1_DSMPD_SHIFT                          (12U)
#define CRU_GPLL_CON1_DSMPD_MASK                           (0x1U << CRU_GPLL_CON1_DSMPD_SHIFT)                          /* 0x00001000 */
#define CRU_GPLL_CON1_PLLPD0_SHIFT                         (13U)
#define CRU_GPLL_CON1_PLLPD0_MASK                          (0x1U << CRU_GPLL_CON1_PLLPD0_SHIFT)                         /* 0x00002000 */
#define CRU_GPLL_CON1_PLLPD1_SHIFT                         (14U)
#define CRU_GPLL_CON1_PLLPD1_MASK                          (0x1U << CRU_GPLL_CON1_PLLPD1_SHIFT)                         /* 0x00004000 */
#define CRU_GPLL_CON1_PLLPDSEL_SHIFT                       (15U)
#define CRU_GPLL_CON1_PLLPDSEL_MASK                        (0x1U << CRU_GPLL_CON1_PLLPDSEL_SHIFT)                       /* 0x00008000 */
/* GPLL_CON2 */
#define CRU_GPLL_CON2_OFFSET                               (0x88)
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
#define CRU_GPLL_CON3_OFFSET                               (0x8C)
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
#define CRU_GPLL_CON4_OFFSET                               (0x90)
#define CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* GPLL_STAT0 */
#define CRU_GPLL_STAT0_OFFSET                              (0x94)
#define CRU_GPLL_STAT0_LOCK_SHIFT                          (0U)
#define CRU_GPLL_STAT0_LOCK_MASK                           (0x1U << CRU_GPLL_STAT0_LOCK_SHIFT)                          /* 0x00000001 */
/* CLKSEL0_CON */
#define CRU_CLKSEL0_CON_OFFSET                             (0x100)
#define CRU_CLKSEL0_CON_INTER_32K_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL0_CON_INTER_32K_DIV_CON_MASK             (0xFFFU << CRU_CLKSEL0_CON_INTER_32K_DIV_CON_SHIFT)          /* 0x00000FFF */
#define CRU_CLKSEL0_CON_CLK_32K_SRC_SEL_SHIFT              (12U)
#define CRU_CLKSEL0_CON_CLK_32K_SRC_SEL_MASK               (0x3U << CRU_CLKSEL0_CON_CLK_32K_SRC_SEL_SHIFT)              /* 0x00003000 */
/* CLKSEL1_CON */
#define CRU_CLKSEL1_CON_OFFSET                             (0x104)
#define CRU_CLKSEL1_CON_HCLK_MCU_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL1_CON_HCLK_MCU_DIV_CON_MASK              (0xFU << CRU_CLKSEL1_CON_HCLK_MCU_DIV_CON_SHIFT)             /* 0x0000000F */
#define CRU_CLKSEL1_CON_HCLK_MCU_SRC_SEL_SHIFT             (7U)
#define CRU_CLKSEL1_CON_HCLK_MCU_SRC_SEL_MASK              (0x1U << CRU_CLKSEL1_CON_HCLK_MCU_SRC_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL1_CON_PCLK_MCU_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL1_CON_PCLK_MCU_DIV_CON_MASK              (0x7U << CRU_CLKSEL1_CON_PCLK_MCU_DIV_CON_SHIFT)             /* 0x00000700 */
#define CRU_CLKSEL1_CON_STCLK_M3_DIV_CON_SHIFT             (12U)
#define CRU_CLKSEL1_CON_STCLK_M3_DIV_CON_MASK              (0xFU << CRU_CLKSEL1_CON_STCLK_M3_DIV_CON_SHIFT)             /* 0x0000F000 */
/* CLKSEL2_CON */
#define CRU_CLKSEL2_CON_OFFSET                             (0x108)
#define CRU_CLKSEL2_CON_CLK_TIMER0_SRC_SEL_SHIFT           (0U)
#define CRU_CLKSEL2_CON_CLK_TIMER0_SRC_SEL_MASK            (0x1U << CRU_CLKSEL2_CON_CLK_TIMER0_SRC_SEL_SHIFT)           /* 0x00000001 */
#define CRU_CLKSEL2_CON_CLK_TIMER1_SRC_SEL_SHIFT           (1U)
#define CRU_CLKSEL2_CON_CLK_TIMER1_SRC_SEL_MASK            (0x1U << CRU_CLKSEL2_CON_CLK_TIMER1_SRC_SEL_SHIFT)           /* 0x00000002 */
#define CRU_CLKSEL2_CON_CLK_TIMER2_SRC_SEL_SHIFT           (2U)
#define CRU_CLKSEL2_CON_CLK_TIMER2_SRC_SEL_MASK            (0x1U << CRU_CLKSEL2_CON_CLK_TIMER2_SRC_SEL_SHIFT)           /* 0x00000004 */
#define CRU_CLKSEL2_CON_CLK_TIMER3_SRC_SEL_SHIFT           (3U)
#define CRU_CLKSEL2_CON_CLK_TIMER3_SRC_SEL_MASK            (0x1U << CRU_CLKSEL2_CON_CLK_TIMER3_SRC_SEL_SHIFT)           /* 0x00000008 */
#define CRU_CLKSEL2_CON_CLK_TIMER4_SRC_SEL_SHIFT           (4U)
#define CRU_CLKSEL2_CON_CLK_TIMER4_SRC_SEL_MASK            (0x1U << CRU_CLKSEL2_CON_CLK_TIMER4_SRC_SEL_SHIFT)           /* 0x00000010 */
#define CRU_CLKSEL2_CON_CLK_PVTM_SRC_SEL_SHIFT             (5U)
#define CRU_CLKSEL2_CON_CLK_PVTM_SRC_SEL_MASK              (0x1U << CRU_CLKSEL2_CON_CLK_PVTM_SRC_SEL_SHIFT)             /* 0x00000020 */
#define CRU_CLKSEL2_CON_UART0_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL2_CON_UART0_DIV_CON_MASK                 (0xFU << CRU_CLKSEL2_CON_UART0_DIV_CON_SHIFT)                /* 0x00000F00 */
#define CRU_CLKSEL2_CON_UART0_CLK_SEL_SHIFT                (12U)
#define CRU_CLKSEL2_CON_UART0_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL2_CON_UART0_CLK_SEL_SHIFT)                /* 0x00003000 */
#define CRU_CLKSEL2_CON_UART0_CLK_SRC_SEL_SHIFT            (15U)
#define CRU_CLKSEL2_CON_UART0_CLK_SRC_SEL_MASK             (0x1U << CRU_CLKSEL2_CON_UART0_CLK_SRC_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL3_CON */
#define CRU_CLKSEL3_CON_OFFSET                             (0x10C)
#define CRU_CLKSEL3_CON_UART1_DIV_CON_SHIFT                (0U)
#define CRU_CLKSEL3_CON_UART1_DIV_CON_MASK                 (0xFU << CRU_CLKSEL3_CON_UART1_DIV_CON_SHIFT)                /* 0x0000000F */
#define CRU_CLKSEL3_CON_UART1_CLK_SEL_SHIFT                (4U)
#define CRU_CLKSEL3_CON_UART1_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL3_CON_UART1_CLK_SEL_SHIFT)                /* 0x00000030 */
#define CRU_CLKSEL3_CON_UART1_CLK_SRC_SEL_SHIFT            (7U)
#define CRU_CLKSEL3_CON_UART1_CLK_SRC_SEL_MASK             (0x1U << CRU_CLKSEL3_CON_UART1_CLK_SRC_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL3_CON_CLK_PWM_DIV_CON_SHIFT              (8U)
#define CRU_CLKSEL3_CON_CLK_PWM_DIV_CON_MASK               (0xFU << CRU_CLKSEL3_CON_CLK_PWM_DIV_CON_SHIFT)              /* 0x00000F00 */
/* CLKSEL4_CON */
#define CRU_CLKSEL4_CON_OFFSET                             (0x110)
#define CRU_CLKSEL4_CON_SPI_DIV_CON_SHIFT                  (0U)
#define CRU_CLKSEL4_CON_SPI_DIV_CON_MASK                   (0xFU << CRU_CLKSEL4_CON_SPI_DIV_CON_SHIFT)                  /* 0x0000000F */
#define CRU_CLKSEL4_CON_CLK_I2C0_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL4_CON_CLK_I2C0_DIV_CON_MASK              (0xFU << CRU_CLKSEL4_CON_CLK_I2C0_DIV_CON_SHIFT)             /* 0x00000F00 */
/* CLKSEL5_CON */
#define CRU_CLKSEL5_CON_OFFSET                             (0x114)
#define CRU_CLKSEL5_CON_CLK_BT52M_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL5_CON_CLK_BT52M_DIV_CON_MASK             (0xFU << CRU_CLKSEL5_CON_CLK_BT52M_DIV_CON_SHIFT)            /* 0x0000000F */
#define CRU_CLKSEL5_CON_CLK_BT52M_GATE_BY_BT_ENABLE_SHIFT  (5U)
#define CRU_CLKSEL5_CON_CLK_BT52M_GATE_BY_BT_ENABLE_MASK   (0x1U << CRU_CLKSEL5_CON_CLK_BT52M_GATE_BY_BT_ENABLE_SHIFT)  /* 0x00000020 */
#define CRU_CLKSEL5_CON_CLK_BT52M_SRC_SEL_SHIFT            (6U)
#define CRU_CLKSEL5_CON_CLK_BT52M_SRC_SEL_MASK             (0x3U << CRU_CLKSEL5_CON_CLK_BT52M_SRC_SEL_SHIFT)            /* 0x000000C0 */
#define CRU_CLKSEL5_CON_HCLK_BT_DIV_CON_SHIFT              (8U)
#define CRU_CLKSEL5_CON_HCLK_BT_DIV_CON_MASK               (0xFU << CRU_CLKSEL5_CON_HCLK_BT_DIV_CON_SHIFT)              /* 0x00000F00 */
#define CRU_CLKSEL5_CON_LOW_POWER_SLK_SRC_SEL_SHIFT        (14U)
#define CRU_CLKSEL5_CON_LOW_POWER_SLK_SRC_SEL_MASK         (0x1U << CRU_CLKSEL5_CON_LOW_POWER_SLK_SRC_SEL_SHIFT)        /* 0x00004000 */
#define CRU_CLKSEL5_CON_HCLK_BT_SRC_SEL_SHIFT              (15U)
#define CRU_CLKSEL5_CON_HCLK_BT_SRC_SEL_MASK               (0x1U << CRU_CLKSEL5_CON_HCLK_BT_SRC_SEL_SHIFT)              /* 0x00008000 */
/* CLKSEL6_CON */
#define CRU_CLKSEL6_CON_OFFSET                             (0x118)
#define CRU_CLKSEL6_CON_I2S_TX_DIV_CON_SHIFT               (0U)
#define CRU_CLKSEL6_CON_I2S_TX_DIV_CON_MASK                (0xFU << CRU_CLKSEL6_CON_I2S_TX_DIV_CON_SHIFT)               /* 0x0000000F */
#define CRU_CLKSEL6_CON_I2S_TX_CLK_SEL_SHIFT               (4U)
#define CRU_CLKSEL6_CON_I2S_TX_CLK_SEL_MASK                (0x3U << CRU_CLKSEL6_CON_I2S_TX_CLK_SEL_SHIFT)               /* 0x00000030 */
#define CRU_CLKSEL6_CON_I2S_TX_CLK_SRC_SEL_SHIFT           (7U)
#define CRU_CLKSEL6_CON_I2S_TX_CLK_SRC_SEL_MASK            (0x1U << CRU_CLKSEL6_CON_I2S_TX_CLK_SRC_SEL_SHIFT)           /* 0x00000080 */
#define CRU_CLKSEL6_CON_I2S_RX_DIV_CON_SHIFT               (8U)
#define CRU_CLKSEL6_CON_I2S_RX_DIV_CON_MASK                (0xFU << CRU_CLKSEL6_CON_I2S_RX_DIV_CON_SHIFT)               /* 0x00000F00 */
#define CRU_CLKSEL6_CON_I2S_RX_CLK_SEL_SHIFT               (12U)
#define CRU_CLKSEL6_CON_I2S_RX_CLK_SEL_MASK                (0x3U << CRU_CLKSEL6_CON_I2S_RX_CLK_SEL_SHIFT)               /* 0x00003000 */
#define CRU_CLKSEL6_CON_I2S_RX_CLK_SRC_SEL_SHIFT           (15U)
#define CRU_CLKSEL6_CON_I2S_RX_CLK_SRC_SEL_MASK            (0x1U << CRU_CLKSEL6_CON_I2S_RX_CLK_SRC_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL7_CON */
#define CRU_CLKSEL7_CON_OFFSET                             (0x11C)
#define CRU_CLKSEL7_CON_CLK_PDM_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL7_CON_CLK_PDM_DIV_CON_MASK               (0xFU << CRU_CLKSEL7_CON_CLK_PDM_DIV_CON_SHIFT)              /* 0x0000000F */
#define CRU_CLKSEL7_CON_CLK_PDM_SEL_SHIFT                  (4U)
#define CRU_CLKSEL7_CON_CLK_PDM_SEL_MASK                   (0x3U << CRU_CLKSEL7_CON_CLK_PDM_SEL_SHIFT)                  /* 0x00000030 */
#define CRU_CLKSEL7_CON_CLK_PDM_SRC_SEL_SHIFT              (7U)
#define CRU_CLKSEL7_CON_CLK_PDM_SRC_SEL_MASK               (0x1U << CRU_CLKSEL7_CON_CLK_PDM_SRC_SEL_SHIFT)              /* 0x00000080 */
#define CRU_CLKSEL7_CON_CLK_SFC_DIV_CON_SHIFT              (8U)
#define CRU_CLKSEL7_CON_CLK_SFC_DIV_CON_MASK               (0xFU << CRU_CLKSEL7_CON_CLK_SFC_DIV_CON_SHIFT)              /* 0x00000F00 */
#define CRU_CLKSEL7_CON_CLK_SFC_SRC_SEL_SHIFT              (15U)
#define CRU_CLKSEL7_CON_CLK_SFC_SRC_SEL_MASK               (0x1U << CRU_CLKSEL7_CON_CLK_SFC_SRC_SEL_SHIFT)              /* 0x00008000 */
/* CLKSEL8_CON */
#define CRU_CLKSEL8_CON_OFFSET                             (0x120)
#define CRU_CLKSEL8_CON_CLK_DSP_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL8_CON_CLK_DSP_DIV_CON_MASK               (0xFU << CRU_CLKSEL8_CON_CLK_DSP_DIV_CON_SHIFT)              /* 0x0000000F */
#define CRU_CLKSEL8_CON_CLK_DSP_SRC_SEL_SHIFT              (7U)
#define CRU_CLKSEL8_CON_CLK_DSP_SRC_SEL_MASK               (0x1U << CRU_CLKSEL8_CON_CLK_DSP_SRC_SEL_SHIFT)              /* 0x00000080 */
#define CRU_CLKSEL8_CON_PCLK_PMU_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL8_CON_PCLK_PMU_DIV_CON_MASK              (0xFU << CRU_CLKSEL8_CON_PCLK_PMU_DIV_CON_SHIFT)             /* 0x00000F00 */
/* CLKSEL9_CON */
#define CRU_CLKSEL9_CON_OFFSET                             (0x124)
#define CRU_CLKSEL9_CON_CLK_I2C2_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL9_CON_CLK_I2C2_DIV_CON_MASK              (0xFU << CRU_CLKSEL9_CON_CLK_I2C2_DIV_CON_SHIFT)             /* 0x0000000F */
#define CRU_CLKSEL9_CON_CLK_GPIO0_DB_SEL_SHIFT             (8U)
#define CRU_CLKSEL9_CON_CLK_GPIO0_DB_SEL_MASK              (0x1U << CRU_CLKSEL9_CON_CLK_GPIO0_DB_SEL_SHIFT)             /* 0x00000100 */
#define CRU_CLKSEL9_CON_CLK_GPIO1_DB_SEL_SHIFT             (9U)
#define CRU_CLKSEL9_CON_CLK_GPIO1_DB_SEL_MASK              (0x1U << CRU_CLKSEL9_CON_CLK_GPIO1_DB_SEL_SHIFT)             /* 0x00000200 */
#define CRU_CLKSEL9_CON_CLK_GPIO2_DB_SEL_SHIFT             (10U)
#define CRU_CLKSEL9_CON_CLK_GPIO2_DB_SEL_MASK              (0x1U << CRU_CLKSEL9_CON_CLK_GPIO2_DB_SEL_SHIFT)             /* 0x00000400 */
/* CLKSEL10_CON */
#define CRU_CLKSEL10_CON_OFFSET                            (0x128)
#define CRU_CLKSEL10_CON_UART0_FRAC_DIV_DENOMINATOR_SHIFT  (0U)
#define CRU_CLKSEL10_CON_UART0_FRAC_DIV_DENOMINATOR_MASK   (0xFFFFU << CRU_CLKSEL10_CON_UART0_FRAC_DIV_DENOMINATOR_SHIFT) /* 0x0000FFFF */
#define CRU_CLKSEL10_CON_UART0_FRAC_DIV_NUMERATOR_SHIFT    (16U)
#define CRU_CLKSEL10_CON_UART0_FRAC_DIV_NUMERATOR_MASK     (0xFFFFU << CRU_CLKSEL10_CON_UART0_FRAC_DIV_NUMERATOR_SHIFT) /* 0xFFFF0000 */
/* CLKSEL11_CON */
#define CRU_CLKSEL11_CON_OFFSET                            (0x12C)
#define CRU_CLKSEL11_CON_UART1_FRAC_DIV_DENOMINATOR_SHIFT  (0U)
#define CRU_CLKSEL11_CON_UART1_FRAC_DIV_DENOMINATOR_MASK   (0xFFFFU << CRU_CLKSEL11_CON_UART1_FRAC_DIV_DENOMINATOR_SHIFT) /* 0x0000FFFF */
#define CRU_CLKSEL11_CON_UART1_FRAC_DIV_NUMERATOR_SHIFT    (16U)
#define CRU_CLKSEL11_CON_UART1_FRAC_DIV_NUMERATOR_MASK     (0xFFFFU << CRU_CLKSEL11_CON_UART1_FRAC_DIV_NUMERATOR_SHIFT) /* 0xFFFF0000 */
/* CLKSEL12_CON */
#define CRU_CLKSEL12_CON_OFFSET                            (0x130)
#define CRU_CLKSEL12_CON_I2S_TX_FRAC_DIV_DENOMINATOR_SHIFT (0U)
#define CRU_CLKSEL12_CON_I2S_TX_FRAC_DIV_DENOMINATOR_MASK  (0xFFFFU << CRU_CLKSEL12_CON_I2S_TX_FRAC_DIV_DENOMINATOR_SHIFT) /* 0x0000FFFF */
#define CRU_CLKSEL12_CON_I2S_TX_FRAC_DIV_NUMERATOR_SHIFT   (16U)
#define CRU_CLKSEL12_CON_I2S_TX_FRAC_DIV_NUMERATOR_MASK    (0xFFFFU << CRU_CLKSEL12_CON_I2S_TX_FRAC_DIV_NUMERATOR_SHIFT) /* 0xFFFF0000 */
/* CLKSEL13_CON */
#define CRU_CLKSEL13_CON_OFFSET                            (0x134)
#define CRU_CLKSEL13_CON_PDM_FRAC_DIV_DENOMINATOR_SHIFT    (0U)
#define CRU_CLKSEL13_CON_PDM_FRAC_DIV_DENOMINATOR_MASK     (0xFFFFU << CRU_CLKSEL13_CON_PDM_FRAC_DIV_DENOMINATOR_SHIFT) /* 0x0000FFFF */
#define CRU_CLKSEL13_CON_PDM_FRAC_DIV_NUMERATOR_SHIFT      (16U)
#define CRU_CLKSEL13_CON_PDM_FRAC_DIV_NUMERATOR_MASK       (0xFFFFU << CRU_CLKSEL13_CON_PDM_FRAC_DIV_NUMERATOR_SHIFT)   /* 0xFFFF0000 */
/* CLKSEL14_CON */
#define CRU_CLKSEL14_CON_OFFSET                            (0x138)
#define CRU_CLKSEL14_CON_I2S_RX_FRAC_DIV_DENOMINATOR_SHIFT (0U)
#define CRU_CLKSEL14_CON_I2S_RX_FRAC_DIV_DENOMINATOR_MASK  (0xFFFFU << CRU_CLKSEL14_CON_I2S_RX_FRAC_DIV_DENOMINATOR_SHIFT) /* 0x0000FFFF */
#define CRU_CLKSEL14_CON_I2S_RX_FRAC_DIV_NUMERATOR_SHIFT   (16U)
#define CRU_CLKSEL14_CON_I2S_RX_FRAC_DIV_NUMERATOR_MASK    (0xFFFFU << CRU_CLKSEL14_CON_I2S_RX_FRAC_DIV_NUMERATOR_SHIFT) /* 0xFFFF0000 */
/* CLKSEL15_CON */
#define CRU_CLKSEL15_CON_OFFSET                            (0x13C)
#define CRU_CLKSEL15_CON_CLK_EFUSE_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL15_CON_CLK_EFUSE_DIV_CON_MASK            (0x7FU << CRU_CLKSEL15_CON_CLK_EFUSE_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL15_CON_CLK_EFUSE_SEL_SHIFT               (7U)
#define CRU_CLKSEL15_CON_CLK_EFUSE_SEL_MASK                (0x1U << CRU_CLKSEL15_CON_CLK_EFUSE_SEL_SHIFT)               /* 0x00000080 */
#define CRU_CLKSEL15_CON_CLK_I2C1_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL15_CON_CLK_I2C1_DIV_CON_MASK             (0xFU << CRU_CLKSEL15_CON_CLK_I2C1_DIV_CON_SHIFT)            /* 0x00000F00 */
/* CLKSEL16_CON */
#define CRU_CLKSEL16_CON_OFFSET                            (0x140)
#define CRU_CLKSEL16_CON_CLK_BBGEN_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL16_CON_CLK_BBGEN_DIV_CON_MASK            (0xFU << CRU_CLKSEL16_CON_CLK_BBGEN_DIV_CON_SHIFT)           /* 0x0000000F */
#define CRU_CLKSEL16_CON_CLK_BBGEN_SEL_SHIFT               (7U)
#define CRU_CLKSEL16_CON_CLK_BBGEN_SEL_MASK                (0x1U << CRU_CLKSEL16_CON_CLK_BBGEN_SEL_SHIFT)               /* 0x00000080 */
/* CLKSEL17_CON */
#define CRU_CLKSEL17_CON_OFFSET                            (0x144)
#define CRU_CLKSEL17_CON_CLK32K_FRAC_DIV_DENOMINATOR_SHIFT (0U)
#define CRU_CLKSEL17_CON_CLK32K_FRAC_DIV_DENOMINATOR_MASK  (0xFFFFU << CRU_CLKSEL17_CON_CLK32K_FRAC_DIV_DENOMINATOR_SHIFT) /* 0x0000FFFF */
#define CRU_CLKSEL17_CON_CLK32K_FRAC_DIV_NUMERATOR_SHIFT   (16U)
#define CRU_CLKSEL17_CON_CLK32K_FRAC_DIV_NUMERATOR_MASK    (0xFFFFU << CRU_CLKSEL17_CON_CLK32K_FRAC_DIV_NUMERATOR_SHIFT) /* 0xFFFF0000 */
/* CLKGATE0_CON */
#define CRU_CLKGATE0_CON_OFFSET                            (0x200)
#define CRU_CLKGATE0_CON_CLK_32K_SRC_EN_SHIFT              (0U)
#define CRU_CLKGATE0_CON_CLK_32K_SRC_EN_MASK               (0x1U << CRU_CLKGATE0_CON_CLK_32K_SRC_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE0_CON_HCLK_MCU_SRC_EN_SHIFT             (1U)
#define CRU_CLKGATE0_CON_HCLK_MCU_SRC_EN_MASK              (0x1U << CRU_CLKGATE0_CON_HCLK_MCU_SRC_EN_SHIFT)             /* 0x00000002 */
#define CRU_CLKGATE0_CON_PCLK_MCU_SRC_EN_SHIFT             (2U)
#define CRU_CLKGATE0_CON_PCLK_MCU_SRC_EN_MASK              (0x1U << CRU_CLKGATE0_CON_PCLK_MCU_SRC_EN_SHIFT)             /* 0x00000004 */
#define CRU_CLKGATE0_CON_CLK_SFC_SRC_EN_SHIFT              (3U)
#define CRU_CLKGATE0_CON_CLK_SFC_SRC_EN_MASK               (0x1U << CRU_CLKGATE0_CON_CLK_SFC_SRC_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE0_CON_CLK_TIMER0_EN_SHIFT               (4U)
#define CRU_CLKGATE0_CON_CLK_TIMER0_EN_MASK                (0x1U << CRU_CLKGATE0_CON_CLK_TIMER0_EN_SHIFT)               /* 0x00000010 */
#define CRU_CLKGATE0_CON_CLK_TIMER1_EN_SHIFT               (5U)
#define CRU_CLKGATE0_CON_CLK_TIMER1_EN_MASK                (0x1U << CRU_CLKGATE0_CON_CLK_TIMER1_EN_SHIFT)               /* 0x00000020 */
#define CRU_CLKGATE0_CON_CLK_TIMER2_EN_SHIFT               (6U)
#define CRU_CLKGATE0_CON_CLK_TIMER2_EN_MASK                (0x1U << CRU_CLKGATE0_CON_CLK_TIMER2_EN_SHIFT)               /* 0x00000040 */
#define CRU_CLKGATE0_CON_CLK_TIMER3_EN_SHIFT               (7U)
#define CRU_CLKGATE0_CON_CLK_TIMER3_EN_MASK                (0x1U << CRU_CLKGATE0_CON_CLK_TIMER3_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE0_CON_CLK_TIMER4_EN_SHIFT               (8U)
#define CRU_CLKGATE0_CON_CLK_TIMER4_EN_MASK                (0x1U << CRU_CLKGATE0_CON_CLK_TIMER4_EN_SHIFT)               /* 0x00000100 */
#define CRU_CLKGATE0_CON_CLK_UART0_SRC_EN_SHIFT            (9U)
#define CRU_CLKGATE0_CON_CLK_UART0_SRC_EN_MASK             (0x1U << CRU_CLKGATE0_CON_CLK_UART0_SRC_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE0_CON_CLK_UART0_FRAC_SRC_EN_SHIFT       (10U)
#define CRU_CLKGATE0_CON_CLK_UART0_FRAC_SRC_EN_MASK        (0x1U << CRU_CLKGATE0_CON_CLK_UART0_FRAC_SRC_EN_SHIFT)       /* 0x00000400 */
#define CRU_CLKGATE0_CON_CLK_UART1_SRC_EN_SHIFT            (11U)
#define CRU_CLKGATE0_CON_CLK_UART1_SRC_EN_MASK             (0x1U << CRU_CLKGATE0_CON_CLK_UART1_SRC_EN_SHIFT)            /* 0x00000800 */
#define CRU_CLKGATE0_CON_CLK_UART1_FRAC_SRC_EN_SHIFT       (12U)
#define CRU_CLKGATE0_CON_CLK_UART1_FRAC_SRC_EN_MASK        (0x1U << CRU_CLKGATE0_CON_CLK_UART1_FRAC_SRC_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE0_CON_CLK_PWM_SRC_EN_SHIFT              (13U)
#define CRU_CLKGATE0_CON_CLK_PWM_SRC_EN_MASK               (0x1U << CRU_CLKGATE0_CON_CLK_PWM_SRC_EN_SHIFT)              /* 0x00002000 */
#define CRU_CLKGATE0_CON_CLK_PVTM_EN_SHIFT                 (14U)
#define CRU_CLKGATE0_CON_CLK_PVTM_EN_MASK                  (0x1U << CRU_CLKGATE0_CON_CLK_PVTM_EN_SHIFT)                 /* 0x00004000 */
#define CRU_CLKGATE0_CON_CLK_SPI_SRC_EN_SHIFT              (15U)
#define CRU_CLKGATE0_CON_CLK_SPI_SRC_EN_MASK               (0x1U << CRU_CLKGATE0_CON_CLK_SPI_SRC_EN_SHIFT)              /* 0x00008000 */
/* CLKGATE1_CON */
#define CRU_CLKGATE1_CON_OFFSET                            (0x204)
#define CRU_CLKGATE1_CON_CLK_I2C0_SRC_EN_SHIFT             (0U)
#define CRU_CLKGATE1_CON_CLK_I2C0_SRC_EN_MASK              (0x1U << CRU_CLKGATE1_CON_CLK_I2C0_SRC_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE1_CON_CLK_I2C1_SRC_EN_SHIFT             (1U)
#define CRU_CLKGATE1_CON_CLK_I2C1_SRC_EN_MASK              (0x1U << CRU_CLKGATE1_CON_CLK_I2C1_SRC_EN_SHIFT)             /* 0x00000002 */
#define CRU_CLKGATE1_CON_HCLK_BT_SRC_EN_SHIFT              (2U)
#define CRU_CLKGATE1_CON_HCLK_BT_SRC_EN_MASK               (0x1U << CRU_CLKGATE1_CON_HCLK_BT_SRC_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE1_CON_CLK_I2S_TX_SRC_EN_SHIFT           (4U)
#define CRU_CLKGATE1_CON_CLK_I2S_TX_SRC_EN_MASK            (0x1U << CRU_CLKGATE1_CON_CLK_I2S_TX_SRC_EN_SHIFT)           /* 0x00000010 */
#define CRU_CLKGATE1_CON_CLK_I2S_TX_FRAC_SRC_EN_SHIFT      (5U)
#define CRU_CLKGATE1_CON_CLK_I2S_TX_FRAC_SRC_EN_MASK       (0x1U << CRU_CLKGATE1_CON_CLK_I2S_TX_FRAC_SRC_EN_SHIFT)      /* 0x00000020 */
#define CRU_CLKGATE1_CON_CLK_PDM_SRC_EN_SHIFT              (6U)
#define CRU_CLKGATE1_CON_CLK_PDM_SRC_EN_MASK               (0x1U << CRU_CLKGATE1_CON_CLK_PDM_SRC_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE1_CON_CLK_PDM_FRAC_SRC_EN_SHIFT         (7U)
#define CRU_CLKGATE1_CON_CLK_PDM_FRAC_SRC_EN_MASK          (0x1U << CRU_CLKGATE1_CON_CLK_PDM_FRAC_SRC_EN_SHIFT)         /* 0x00000080 */
#define CRU_CLKGATE1_CON_CLK_DSP_SRC_EN_SHIFT              (10U)
#define CRU_CLKGATE1_CON_CLK_DSP_SRC_EN_MASK               (0x1U << CRU_CLKGATE1_CON_CLK_DSP_SRC_EN_SHIFT)              /* 0x00000400 */
#define CRU_CLKGATE1_CON_PCLK_PMU_SRC_EN_SHIFT             (12U)
#define CRU_CLKGATE1_CON_PCLK_PMU_SRC_EN_MASK              (0x1U << CRU_CLKGATE1_CON_PCLK_PMU_SRC_EN_SHIFT)             /* 0x00001000 */
#define CRU_CLKGATE1_CON_CLK_I2C2_SRC_EN_SHIFT             (13U)
#define CRU_CLKGATE1_CON_CLK_I2C2_SRC_EN_MASK              (0x1U << CRU_CLKGATE1_CON_CLK_I2C2_SRC_EN_SHIFT)             /* 0x00002000 */
#define CRU_CLKGATE1_CON_TESTCLK_EN_SHIFT                  (14U)
#define CRU_CLKGATE1_CON_TESTCLK_EN_MASK                   (0x1U << CRU_CLKGATE1_CON_TESTCLK_EN_SHIFT)                  /* 0x00004000 */
#define CRU_CLKGATE1_CON_CLK_BT52M_SRC_EN_SHIFT            (15U)
#define CRU_CLKGATE1_CON_CLK_BT52M_SRC_EN_MASK             (0x1U << CRU_CLKGATE1_CON_CLK_BT52M_SRC_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE2_CON */
#define CRU_CLKGATE2_CON_OFFSET                            (0x208)
#define CRU_CLKGATE2_CON_HCLK_MCU_NOC_EN_SHIFT             (0U)
#define CRU_CLKGATE2_CON_HCLK_MCU_NOC_EN_MASK              (0x1U << CRU_CLKGATE2_CON_HCLK_MCU_NOC_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE2_CON_FCLK_M3_EN_SHIFT                  (1U)
#define CRU_CLKGATE2_CON_FCLK_M3_EN_MASK                   (0x1U << CRU_CLKGATE2_CON_FCLK_M3_EN_SHIFT)                  /* 0x00000002 */
#define CRU_CLKGATE2_CON_HCLK_M3_EN_SHIFT                  (2U)
#define CRU_CLKGATE2_CON_HCLK_M3_EN_MASK                   (0x1U << CRU_CLKGATE2_CON_HCLK_M3_EN_SHIFT)                  /* 0x00000004 */
#define CRU_CLKGATE2_CON_HCLK_INTMEM0_EN_SHIFT             (3U)
#define CRU_CLKGATE2_CON_HCLK_INTMEM0_EN_MASK              (0x1U << CRU_CLKGATE2_CON_HCLK_INTMEM0_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE2_CON_HCLK_INTMEM1_EN_SHIFT             (4U)
#define CRU_CLKGATE2_CON_HCLK_INTMEM1_EN_MASK              (0x1U << CRU_CLKGATE2_CON_HCLK_INTMEM1_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE2_CON_HCLK_ROM_EN_SHIFT                 (5U)
#define CRU_CLKGATE2_CON_HCLK_ROM_EN_MASK                  (0x1U << CRU_CLKGATE2_CON_HCLK_ROM_EN_SHIFT)                 /* 0x00000020 */
#define CRU_CLKGATE2_CON_HCLK_SFC_EN_SHIFT                 (6U)
#define CRU_CLKGATE2_CON_HCLK_SFC_EN_MASK                  (0x1U << CRU_CLKGATE2_CON_HCLK_SFC_EN_SHIFT)                 /* 0x00000040 */
#define CRU_CLKGATE2_CON_ACLK_DMAC_EN_SHIFT                (7U)
#define CRU_CLKGATE2_CON_ACLK_DMAC_EN_MASK                 (0x1U << CRU_CLKGATE2_CON_ACLK_DMAC_EN_SHIFT)                /* 0x00000080 */
#define CRU_CLKGATE2_CON_PCLK_TIMER_EN_SHIFT               (8U)
#define CRU_CLKGATE2_CON_PCLK_TIMER_EN_MASK                (0x1U << CRU_CLKGATE2_CON_PCLK_TIMER_EN_SHIFT)               /* 0x00000100 */
#define CRU_CLKGATE2_CON_PCLK_WDT0_EN_SHIFT                (9U)
#define CRU_CLKGATE2_CON_PCLK_WDT0_EN_MASK                 (0x1U << CRU_CLKGATE2_CON_PCLK_WDT0_EN_SHIFT)                /* 0x00000200 */
#define CRU_CLKGATE2_CON_PCLK_WDT1_EN_SHIFT                (10U)
#define CRU_CLKGATE2_CON_PCLK_WDT1_EN_MASK                 (0x1U << CRU_CLKGATE2_CON_PCLK_WDT1_EN_SHIFT)                /* 0x00000400 */
#define CRU_CLKGATE2_CON_PCLK_UART0_EN_SHIFT               (11U)
#define CRU_CLKGATE2_CON_PCLK_UART0_EN_MASK                (0x1U << CRU_CLKGATE2_CON_PCLK_UART0_EN_SHIFT)               /* 0x00000800 */
#define CRU_CLKGATE2_CON_PCLK_UART1_EN_SHIFT               (12U)
#define CRU_CLKGATE2_CON_PCLK_UART1_EN_MASK                (0x1U << CRU_CLKGATE2_CON_PCLK_UART1_EN_SHIFT)               /* 0x00001000 */
#define CRU_CLKGATE2_CON_PCLK_PWM_EN_SHIFT                 (13U)
#define CRU_CLKGATE2_CON_PCLK_PWM_EN_MASK                  (0x1U << CRU_CLKGATE2_CON_PCLK_PWM_EN_SHIFT)                 /* 0x00002000 */
#define CRU_CLKGATE2_CON_PCLK_MAILBOX_EN_SHIFT             (14U)
#define CRU_CLKGATE2_CON_PCLK_MAILBOX_EN_MASK              (0x1U << CRU_CLKGATE2_CON_PCLK_MAILBOX_EN_SHIFT)             /* 0x00004000 */
#define CRU_CLKGATE2_CON_HCLK_SFC_XIP_EN_SHIFT             (15U)
#define CRU_CLKGATE2_CON_HCLK_SFC_XIP_EN_MASK              (0x1U << CRU_CLKGATE2_CON_HCLK_SFC_XIP_EN_SHIFT)             /* 0x00008000 */
/* CLKGATE3_CON */
#define CRU_CLKGATE3_CON_OFFSET                            (0x20C)
#define CRU_CLKGATE3_CON_PCLK_SPI_EN_SHIFT                 (1U)
#define CRU_CLKGATE3_CON_PCLK_SPI_EN_MASK                  (0x1U << CRU_CLKGATE3_CON_PCLK_SPI_EN_SHIFT)                 /* 0x00000002 */
#define CRU_CLKGATE3_CON_PCLK_I2C0_EN_SHIFT                (2U)
#define CRU_CLKGATE3_CON_PCLK_I2C0_EN_MASK                 (0x1U << CRU_CLKGATE3_CON_PCLK_I2C0_EN_SHIFT)                /* 0x00000004 */
#define CRU_CLKGATE3_CON_PCLK_I2C1_EN_SHIFT                (3U)
#define CRU_CLKGATE3_CON_PCLK_I2C1_EN_MASK                 (0x1U << CRU_CLKGATE3_CON_PCLK_I2C1_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE3_CON_PCLK_GPIO1_EN_SHIFT               (4U)
#define CRU_CLKGATE3_CON_PCLK_GPIO1_EN_MASK                (0x1U << CRU_CLKGATE3_CON_PCLK_GPIO1_EN_SHIFT)               /* 0x00000010 */
#define CRU_CLKGATE3_CON_HCLK_BT_NOC_EN_SHIFT              (5U)
#define CRU_CLKGATE3_CON_HCLK_BT_NOC_EN_MASK               (0x1U << CRU_CLKGATE3_CON_HCLK_BT_NOC_EN_SHIFT)              /* 0x00000020 */
#define CRU_CLKGATE3_CON_HCLK_BTDM_EN_SHIFT                (6U)
#define CRU_CLKGATE3_CON_HCLK_BTDM_EN_MASK                 (0x1U << CRU_CLKGATE3_CON_HCLK_BTDM_EN_SHIFT)                /* 0x00000040 */
#define CRU_CLKGATE3_CON_HCLK_I2S_EN_SHIFT                 (7U)
#define CRU_CLKGATE3_CON_HCLK_I2S_EN_MASK                  (0x1U << CRU_CLKGATE3_CON_HCLK_I2S_EN_SHIFT)                 /* 0x00000080 */
#define CRU_CLKGATE3_CON_HCLK_PDM_EN_SHIFT                 (8U)
#define CRU_CLKGATE3_CON_HCLK_PDM_EN_MASK                  (0x1U << CRU_CLKGATE3_CON_HCLK_PDM_EN_SHIFT)                 /* 0x00000100 */
#define CRU_CLKGATE3_CON_HCLK_BTSLV_NOC_EN_SHIFT           (9U)
#define CRU_CLKGATE3_CON_HCLK_BTSLV_NOC_EN_MASK            (0x1U << CRU_CLKGATE3_CON_HCLK_BTSLV_NOC_EN_SHIFT)           /* 0x00000200 */
#define CRU_CLKGATE3_CON_ACLK_DSP_NOC_EN_SHIFT             (10U)
#define CRU_CLKGATE3_CON_ACLK_DSP_NOC_EN_MASK              (0x1U << CRU_CLKGATE3_CON_ACLK_DSP_NOC_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE3_CON_CLK_DSP_EN_SHIFT                  (11U)
#define CRU_CLKGATE3_CON_CLK_DSP_EN_MASK                   (0x1U << CRU_CLKGATE3_CON_CLK_DSP_EN_SHIFT)                  /* 0x00000800 */
#define CRU_CLKGATE3_CON_PCLK_EFUSE_EN_SHIFT               (12U)
#define CRU_CLKGATE3_CON_PCLK_EFUSE_EN_MASK                (0x1U << CRU_CLKGATE3_CON_PCLK_EFUSE_EN_SHIFT)               /* 0x00001000 */
#define CRU_CLKGATE3_CON_HCLK_INTC0_EN_SHIFT               (13U)
#define CRU_CLKGATE3_CON_HCLK_INTC0_EN_MASK                (0x1U << CRU_CLKGATE3_CON_HCLK_INTC0_EN_SHIFT)               /* 0x00002000 */
#define CRU_CLKGATE3_CON_HCLK_BT_TEST_EN_SHIFT             (14U)
#define CRU_CLKGATE3_CON_HCLK_BT_TEST_EN_MASK              (0x1U << CRU_CLKGATE3_CON_HCLK_BT_TEST_EN_SHIFT)             /* 0x00004000 */
#define CRU_CLKGATE3_CON_PCLK_GPIO2_EN_SHIFT               (15U)
#define CRU_CLKGATE3_CON_PCLK_GPIO2_EN_MASK                (0x1U << CRU_CLKGATE3_CON_PCLK_GPIO2_EN_SHIFT)               /* 0x00008000 */
/* CLKGATE4_CON */
#define CRU_CLKGATE4_CON_OFFSET                            (0x210)
#define CRU_CLKGATE4_CON_PCLK_PMU_NOC_EN_SHIFT             (0U)
#define CRU_CLKGATE4_CON_PCLK_PMU_NOC_EN_MASK              (0x1U << CRU_CLKGATE4_CON_PCLK_PMU_NOC_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE4_CON_PCLK_PMUIP_EN_SHIFT               (1U)
#define CRU_CLKGATE4_CON_PCLK_PMUIP_EN_MASK                (0x1U << CRU_CLKGATE4_CON_PCLK_PMUIP_EN_SHIFT)               /* 0x00000002 */
#define CRU_CLKGATE4_CON_PCLK_GPIO0_EN_SHIFT               (2U)
#define CRU_CLKGATE4_CON_PCLK_GPIO0_EN_MASK                (0x1U << CRU_CLKGATE4_CON_PCLK_GPIO0_EN_SHIFT)               /* 0x00000004 */
#define CRU_CLKGATE4_CON_PCLK_GRF_EN_SHIFT                 (3U)
#define CRU_CLKGATE4_CON_PCLK_GRF_EN_MASK                  (0x1U << CRU_CLKGATE4_CON_PCLK_GRF_EN_SHIFT)                 /* 0x00000008 */
#define CRU_CLKGATE4_CON_PCLK_I2C2_EN_SHIFT                (6U)
#define CRU_CLKGATE4_CON_PCLK_I2C2_EN_MASK                 (0x1U << CRU_CLKGATE4_CON_PCLK_I2C2_EN_SHIFT)                /* 0x00000040 */
#define CRU_CLKGATE4_CON_PCLK_PMUMEM_EN_SHIFT              (7U)
#define CRU_CLKGATE4_CON_PCLK_PMUMEM_EN_MASK               (0x1U << CRU_CLKGATE4_CON_PCLK_PMUMEM_EN_SHIFT)              /* 0x00000080 */
#define CRU_CLKGATE4_CON_PCLK_BBGENIP_EN_SHIFT             (8U)
#define CRU_CLKGATE4_CON_PCLK_BBGENIP_EN_MASK              (0x1U << CRU_CLKGATE4_CON_PCLK_BBGENIP_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE4_CON_PCLK_I2CS_EN_SHIFT                (9U)
#define CRU_CLKGATE4_CON_PCLK_I2CS_EN_MASK                 (0x1U << CRU_CLKGATE4_CON_PCLK_I2CS_EN_SHIFT)                /* 0x00000200 */
#define CRU_CLKGATE4_CON_RFPLL_CLK_EN_SHIFT                (10U)
#define CRU_CLKGATE4_CON_RFPLL_CLK_EN_MASK                 (0x1U << CRU_CLKGATE4_CON_RFPLL_CLK_EN_SHIFT)                /* 0x00000400 */
#define CRU_CLKGATE4_CON_PCLK_TOPNOC_EN_SHIFT              (11U)
#define CRU_CLKGATE4_CON_PCLK_TOPNOC_EN_MASK               (0x1U << CRU_CLKGATE4_CON_PCLK_TOPNOC_EN_SHIFT)              /* 0x00000800 */
/* CLKGATE5_CON */
#define CRU_CLKGATE5_CON_OFFSET                            (0x214)
#define CRU_CLKGATE5_CON_CLK_I2S_RX_SRC_EN_SHIFT           (0U)
#define CRU_CLKGATE5_CON_CLK_I2S_RX_SRC_EN_MASK            (0x1U << CRU_CLKGATE5_CON_CLK_I2S_RX_SRC_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE5_CON_CLK_I2S_RX_FRAC_SRC_EN_SHIFT      (1U)
#define CRU_CLKGATE5_CON_CLK_I2S_RX_FRAC_SRC_EN_MASK       (0x1U << CRU_CLKGATE5_CON_CLK_I2S_RX_FRAC_SRC_EN_SHIFT)      /* 0x00000002 */
#define CRU_CLKGATE5_CON_CLK_GPIO0_DB_EN_SHIFT             (2U)
#define CRU_CLKGATE5_CON_CLK_GPIO0_DB_EN_MASK              (0x1U << CRU_CLKGATE5_CON_CLK_GPIO0_DB_EN_SHIFT)             /* 0x00000004 */
#define CRU_CLKGATE5_CON_CLK_GPIO1_DB_EN_SHIFT             (3U)
#define CRU_CLKGATE5_CON_CLK_GPIO1_DB_EN_MASK              (0x1U << CRU_CLKGATE5_CON_CLK_GPIO1_DB_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE5_CON_CLK_GPIO2_DB_EN_SHIFT             (4U)
#define CRU_CLKGATE5_CON_CLK_GPIO2_DB_EN_MASK              (0x1U << CRU_CLKGATE5_CON_CLK_GPIO2_DB_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE5_CON_CLK_EFUSE_EN_SHIFT                (5U)
#define CRU_CLKGATE5_CON_CLK_EFUSE_EN_MASK                 (0x1U << CRU_CLKGATE5_CON_CLK_EFUSE_EN_SHIFT)                /* 0x00000020 */
#define CRU_CLKGATE5_CON_CLK_BBGEN_SRC_EN_SHIFT            (6U)
#define CRU_CLKGATE5_CON_CLK_BBGEN_SRC_EN_MASK             (0x1U << CRU_CLKGATE5_CON_CLK_BBGEN_SRC_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE5_CON_CLK_REF_BBGEN_EN_SHIFT            (7U)
#define CRU_CLKGATE5_CON_CLK_REF_BBGEN_EN_MASK             (0x1U << CRU_CLKGATE5_CON_CLK_REF_BBGEN_EN_SHIFT)            /* 0x00000080 */
#define CRU_CLKGATE5_CON_PCLK_BBGEN_SRC_EN_SHIFT           (8U)
#define CRU_CLKGATE5_CON_PCLK_BBGEN_SRC_EN_MASK            (0x1U << CRU_CLKGATE5_CON_PCLK_BBGEN_SRC_EN_SHIFT)           /* 0x00000100 */
#define CRU_CLKGATE5_CON_CLK_LOW_POWER_EN_SHIFT            (9U)
#define CRU_CLKGATE5_CON_CLK_LOW_POWER_EN_MASK             (0x1U << CRU_CLKGATE5_CON_CLK_LOW_POWER_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE5_CON_CLK32K_OSC_FRAC_SRC_EN_SHIFT      (10U)
#define CRU_CLKGATE5_CON_CLK32K_OSC_FRAC_SRC_EN_MASK       (0x1U << CRU_CLKGATE5_CON_CLK32K_OSC_FRAC_SRC_EN_SHIFT)      /* 0x00000400 */
/* SOFTRST0_CON */
#define CRU_SOFTRST0_CON_OFFSET                            (0x300)
#define CRU_SOFTRST0_CON_PORESETN_M3_AUTOCLR_REQ_SHIFT     (0U)
#define CRU_SOFTRST0_CON_PORESETN_M3_AUTOCLR_REQ_MASK      (0x1U << CRU_SOFTRST0_CON_PORESETN_M3_AUTOCLR_REQ_SHIFT)     /* 0x00000001 */
#define CRU_SOFTRST0_CON_SYSRESETN_M3_AUTOCLR_REQ_SHIFT    (1U)
#define CRU_SOFTRST0_CON_SYSRESETN_M3_AUTOCLR_REQ_MASK     (0x1U << CRU_SOFTRST0_CON_SYSRESETN_M3_AUTOCLR_REQ_SHIFT)    /* 0x00000002 */
#define CRU_SOFTRST0_CON_HRESETN_MCU_NOC_AUTOCLR_REQ_SHIFT (2U)
#define CRU_SOFTRST0_CON_HRESETN_MCU_NOC_AUTOCLR_REQ_MASK  (0x1U << CRU_SOFTRST0_CON_HRESETN_MCU_NOC_AUTOCLR_REQ_SHIFT) /* 0x00000004 */
#define CRU_SOFTRST0_CON_PRESETN_TOPNOC_AUTOCLR_REQ_SHIFT  (3U)
#define CRU_SOFTRST0_CON_PRESETN_TOPNOC_AUTOCLR_REQ_MASK   (0x1U << CRU_SOFTRST0_CON_PRESETN_TOPNOC_AUTOCLR_REQ_SHIFT)  /* 0x00000008 */
/* SOFTRST1_CON */
#define CRU_SOFTRST1_CON_OFFSET                            (0x304)
#define CRU_SOFTRST1_CON_PORESETN_M3_REQ_SHIFT             (0U)
#define CRU_SOFTRST1_CON_PORESETN_M3_REQ_MASK              (0x1U << CRU_SOFTRST1_CON_PORESETN_M3_REQ_SHIFT)             /* 0x00000001 */
#define CRU_SOFTRST1_CON_SYSRESETN_M3_REQ_SHIFT            (1U)
#define CRU_SOFTRST1_CON_SYSRESETN_M3_REQ_MASK             (0x1U << CRU_SOFTRST1_CON_SYSRESETN_M3_REQ_SHIFT)            /* 0x00000002 */
#define CRU_SOFTRST1_CON_HRESETN_MCU_NOC_REQ_SHIFT         (2U)
#define CRU_SOFTRST1_CON_HRESETN_MCU_NOC_REQ_MASK          (0x1U << CRU_SOFTRST1_CON_HRESETN_MCU_NOC_REQ_SHIFT)         /* 0x00000004 */
#define CRU_SOFTRST1_CON_HRESETN_INTMEM0_REQ_SHIFT         (3U)
#define CRU_SOFTRST1_CON_HRESETN_INTMEM0_REQ_MASK          (0x1U << CRU_SOFTRST1_CON_HRESETN_INTMEM0_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST1_CON_HRESETN_INTMEM1_REQ_SHIFT         (4U)
#define CRU_SOFTRST1_CON_HRESETN_INTMEM1_REQ_MASK          (0x1U << CRU_SOFTRST1_CON_HRESETN_INTMEM1_REQ_SHIFT)         /* 0x00000010 */
#define CRU_SOFTRST1_CON_HRESETN_ROM_REQ_SHIFT             (5U)
#define CRU_SOFTRST1_CON_HRESETN_ROM_REQ_MASK              (0x1U << CRU_SOFTRST1_CON_HRESETN_ROM_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST1_CON_ARESETN_DMA_REQ_SHIFT             (6U)
#define CRU_SOFTRST1_CON_ARESETN_DMA_REQ_MASK              (0x1U << CRU_SOFTRST1_CON_ARESETN_DMA_REQ_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST1_CON_HRESETN_SFC_REQ_SHIFT             (7U)
#define CRU_SOFTRST1_CON_HRESETN_SFC_REQ_MASK              (0x1U << CRU_SOFTRST1_CON_HRESETN_SFC_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST1_CON_PRESETN_WDT0_REQ_SHIFT            (8U)
#define CRU_SOFTRST1_CON_PRESETN_WDT0_REQ_MASK             (0x1U << CRU_SOFTRST1_CON_PRESETN_WDT0_REQ_SHIFT)            /* 0x00000100 */
#define CRU_SOFTRST1_CON_PRESETN_WDT1_REQ_SHIFT            (9U)
#define CRU_SOFTRST1_CON_PRESETN_WDT1_REQ_MASK             (0x1U << CRU_SOFTRST1_CON_PRESETN_WDT1_REQ_SHIFT)            /* 0x00000200 */
#define CRU_SOFTRST1_CON_PRESETN_UART0_REQ_SHIFT           (10U)
#define CRU_SOFTRST1_CON_PRESETN_UART0_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_PRESETN_UART0_REQ_SHIFT)           /* 0x00000400 */
#define CRU_SOFTRST1_CON_PRESETN_UART1_REQ_SHIFT           (11U)
#define CRU_SOFTRST1_CON_PRESETN_UART1_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_PRESETN_UART1_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST1_CON_PRESETN_PWM_REQ_SHIFT             (12U)
#define CRU_SOFTRST1_CON_PRESETN_PWM_REQ_MASK              (0x1U << CRU_SOFTRST1_CON_PRESETN_PWM_REQ_SHIFT)             /* 0x00001000 */
#define CRU_SOFTRST1_CON_HRESETN_SFC_XIP_REQ_SHIFT         (13U)
#define CRU_SOFTRST1_CON_HRESETN_SFC_XIP_REQ_MASK          (0x1U << CRU_SOFTRST1_CON_HRESETN_SFC_XIP_REQ_SHIFT)         /* 0x00002000 */
#define CRU_SOFTRST1_CON_RESETN_I2S_RX_REQ_SHIFT           (14U)
#define CRU_SOFTRST1_CON_RESETN_I2S_RX_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_RESETN_I2S_RX_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST1_CON_PRESETN_EFUSE_REQ_SHIFT           (15U)
#define CRU_SOFTRST1_CON_PRESETN_EFUSE_REQ_MASK            (0x1U << CRU_SOFTRST1_CON_PRESETN_EFUSE_REQ_SHIFT)           /* 0x00008000 */
/* SOFTRST2_CON */
#define CRU_SOFTRST2_CON_OFFSET                            (0x308)
#define CRU_SOFTRST2_CON_PRESETN_MAILBOX_REQ_SHIFT         (0U)
#define CRU_SOFTRST2_CON_PRESETN_MAILBOX_REQ_MASK          (0x1U << CRU_SOFTRST2_CON_PRESETN_MAILBOX_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST2_CON_PRESETN_SPI_REQ_SHIFT             (2U)
#define CRU_SOFTRST2_CON_PRESETN_SPI_REQ_MASK              (0x1U << CRU_SOFTRST2_CON_PRESETN_SPI_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST2_CON_PRESETN_I2C0_REQ_SHIFT            (3U)
#define CRU_SOFTRST2_CON_PRESETN_I2C0_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_PRESETN_I2C0_REQ_SHIFT)            /* 0x00000008 */
#define CRU_SOFTRST2_CON_PRESETN_I2C1_REQ_SHIFT            (4U)
#define CRU_SOFTRST2_CON_PRESETN_I2C1_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_PRESETN_I2C1_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST2_CON_RESETN_SFC_REQ_SHIFT              (5U)
#define CRU_SOFTRST2_CON_RESETN_SFC_REQ_MASK               (0x1U << CRU_SOFTRST2_CON_RESETN_SFC_REQ_SHIFT)              /* 0x00000020 */
#define CRU_SOFTRST2_CON_RESETN_UART0_REQ_SHIFT            (6U)
#define CRU_SOFTRST2_CON_RESETN_UART0_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_RESETN_UART0_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST2_CON_RESETN_UART1_REQ_SHIFT            (7U)
#define CRU_SOFTRST2_CON_RESETN_UART1_REQ_MASK             (0x1U << CRU_SOFTRST2_CON_RESETN_UART1_REQ_SHIFT)            /* 0x00000080 */
#define CRU_SOFTRST2_CON_RESETN_PWM_REQ_SHIFT              (8U)
#define CRU_SOFTRST2_CON_RESETN_PWM_REQ_MASK               (0x1U << CRU_SOFTRST2_CON_RESETN_PWM_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST2_CON_RESETN_TIMER0_REQ_SHIFT           (10U)
#define CRU_SOFTRST2_CON_RESETN_TIMER0_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_RESETN_TIMER0_REQ_SHIFT)           /* 0x00000400 */
#define CRU_SOFTRST2_CON_RESETN_TIMER1_REQ_SHIFT           (11U)
#define CRU_SOFTRST2_CON_RESETN_TIMER1_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_RESETN_TIMER1_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST2_CON_RESETN_TIMER2_REQ_SHIFT           (12U)
#define CRU_SOFTRST2_CON_RESETN_TIMER2_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_RESETN_TIMER2_REQ_SHIFT)           /* 0x00001000 */
#define CRU_SOFTRST2_CON_RESETN_TIMER3_REQ_SHIFT           (13U)
#define CRU_SOFTRST2_CON_RESETN_TIMER3_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_RESETN_TIMER3_REQ_SHIFT)           /* 0x00002000 */
#define CRU_SOFTRST2_CON_RESETN_TIMER4_REQ_SHIFT           (14U)
#define CRU_SOFTRST2_CON_RESETN_TIMER4_REQ_MASK            (0x1U << CRU_SOFTRST2_CON_RESETN_TIMER4_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST2_CON_RESETN_SPI_REQ_SHIFT              (15U)
#define CRU_SOFTRST2_CON_RESETN_SPI_REQ_MASK               (0x1U << CRU_SOFTRST2_CON_RESETN_SPI_REQ_SHIFT)              /* 0x00008000 */
/* SOFTRST3_CON */
#define CRU_SOFTRST3_CON_OFFSET                            (0x30C)
#define CRU_SOFTRST3_CON_RESETN_I2C0_REQ_SHIFT             (0U)
#define CRU_SOFTRST3_CON_RESETN_I2C0_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_RESETN_I2C0_REQ_SHIFT)             /* 0x00000001 */
#define CRU_SOFTRST3_CON_RESETN_I2C1_REQ_SHIFT             (1U)
#define CRU_SOFTRST3_CON_RESETN_I2C1_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_RESETN_I2C1_REQ_SHIFT)             /* 0x00000002 */
#define CRU_SOFTRST3_CON_TRSTN_M3_JTAG_REQ_SHIFT           (2U)
#define CRU_SOFTRST3_CON_TRSTN_M3_JTAG_REQ_MASK            (0x1U << CRU_SOFTRST3_CON_TRSTN_M3_JTAG_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST3_CON_PRESETN_GPIO1_REQ_SHIFT           (3U)
#define CRU_SOFTRST3_CON_PRESETN_GPIO1_REQ_MASK            (0x1U << CRU_SOFTRST3_CON_PRESETN_GPIO1_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST3_CON_PRESETN_TIMER_REQ_SHIFT           (4U)
#define CRU_SOFTRST3_CON_PRESETN_TIMER_REQ_MASK            (0x1U << CRU_SOFTRST3_CON_PRESETN_TIMER_REQ_SHIFT)           /* 0x00000010 */
#define CRU_SOFTRST3_CON_HRESETN_BT_NOC_REQ_SHIFT          (5U)
#define CRU_SOFTRST3_CON_HRESETN_BT_NOC_REQ_MASK           (0x1U << CRU_SOFTRST3_CON_HRESETN_BT_NOC_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST3_CON_HRESETN_BTDM_REQ_SHIFT            (6U)
#define CRU_SOFTRST3_CON_HRESETN_BTDM_REQ_MASK             (0x1U << CRU_SOFTRST3_CON_HRESETN_BTDM_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST3_CON_HRESETN_I2S_REQ_SHIFT             (7U)
#define CRU_SOFTRST3_CON_HRESETN_I2S_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_HRESETN_I2S_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST3_CON_HRESETN_PDM_REQ_SHIFT             (8U)
#define CRU_SOFTRST3_CON_HRESETN_PDM_REQ_MASK              (0x1U << CRU_SOFTRST3_CON_HRESETN_PDM_REQ_SHIFT)             /* 0x00000100 */
#define CRU_SOFTRST3_CON_RESETN_I2S_TX_REQ_SHIFT           (11U)
#define CRU_SOFTRST3_CON_RESETN_I2S_TX_REQ_MASK            (0x1U << CRU_SOFTRST3_CON_RESETN_I2S_TX_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST3_CON_RESETN_PDM_REQ_SHIFT              (12U)
#define CRU_SOFTRST3_CON_RESETN_PDM_REQ_MASK               (0x1U << CRU_SOFTRST3_CON_RESETN_PDM_REQ_SHIFT)              /* 0x00001000 */
#define CRU_SOFTRST3_CON_RESETN_RF_REQ_SHIFT               (15U)
#define CRU_SOFTRST3_CON_RESETN_RF_REQ_MASK                (0x1U << CRU_SOFTRST3_CON_RESETN_RF_REQ_SHIFT)               /* 0x00008000 */
/* SOFTRST4_CON */
#define CRU_SOFTRST4_CON_OFFSET                            (0x310)
#define CRU_SOFTRST4_CON_ARESETN_DSP_NOC_REQ_SHIFT         (0U)
#define CRU_SOFTRST4_CON_ARESETN_DSP_NOC_REQ_MASK          (0x1U << CRU_SOFTRST4_CON_ARESETN_DSP_NOC_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST4_CON_RESETN_DSP_REQ_SHIFT              (1U)
#define CRU_SOFTRST4_CON_RESETN_DSP_REQ_MASK               (0x1U << CRU_SOFTRST4_CON_RESETN_DSP_REQ_SHIFT)              /* 0x00000002 */
#define CRU_SOFTRST4_CON_RESETN_DSP_EXTCORE_REQ_SHIFT      (2U)
#define CRU_SOFTRST4_CON_RESETN_DSP_EXTCORE_REQ_MASK       (0x1U << CRU_SOFTRST4_CON_RESETN_DSP_EXTCORE_REQ_SHIFT)      /* 0x00000004 */
#define CRU_SOFTRST4_CON_PRESETN_PMU_NOC_REQ_SHIFT         (3U)
#define CRU_SOFTRST4_CON_PRESETN_PMU_NOC_REQ_MASK          (0x1U << CRU_SOFTRST4_CON_PRESETN_PMU_NOC_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST4_CON_PRESETN_GPIO0_REQ_SHIFT           (4U)
#define CRU_SOFTRST4_CON_PRESETN_GPIO0_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_PRESETN_GPIO0_REQ_SHIFT)           /* 0x00000010 */
#define CRU_SOFTRST4_CON_PRESETN_GRF_REQ_SHIFT             (5U)
#define CRU_SOFTRST4_CON_PRESETN_GRF_REQ_MASK              (0x1U << CRU_SOFTRST4_CON_PRESETN_GRF_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST4_CON_PRESETN_I2C2_REQ_SHIFT            (7U)
#define CRU_SOFTRST4_CON_PRESETN_I2C2_REQ_MASK             (0x1U << CRU_SOFTRST4_CON_PRESETN_I2C2_REQ_SHIFT)            /* 0x00000080 */
#define CRU_SOFTRST4_CON_RESETN_I2C2_REQ_SHIFT             (9U)
#define CRU_SOFTRST4_CON_RESETN_I2C2_REQ_MASK              (0x1U << CRU_SOFTRST4_CON_RESETN_I2C2_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST4_CON_RESETN_PMU_REQ_SHIFT              (10U)
#define CRU_SOFTRST4_CON_RESETN_PMU_REQ_MASK               (0x1U << CRU_SOFTRST4_CON_RESETN_PMU_REQ_SHIFT)              /* 0x00000400 */
#define CRU_SOFTRST4_CON_HRESETN_INTC0_REQ_SHIFT           (11U)
#define CRU_SOFTRST4_CON_HRESETN_INTC0_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_HRESETN_INTC0_REQ_SHIFT)           /* 0x00000800 */
#define CRU_SOFTRST4_CON_MDM_NRST_REQ_SHIFT                (12U)
#define CRU_SOFTRST4_CON_MDM_NRST_REQ_MASK                 (0x1U << CRU_SOFTRST4_CON_MDM_NRST_REQ_SHIFT)                /* 0x00001000 */
#define CRU_SOFTRST4_CON_BB_NRST_REQ_SHIFT                 (13U)
#define CRU_SOFTRST4_CON_BB_NRST_REQ_MASK                  (0x1U << CRU_SOFTRST4_CON_BB_NRST_REQ_SHIFT)                 /* 0x00002000 */
#define CRU_SOFTRST4_CON_PRESETN_PMUMEM_REQ_SHIFT          (14U)
#define CRU_SOFTRST4_CON_PRESETN_PMUMEM_REQ_MASK           (0x1U << CRU_SOFTRST4_CON_PRESETN_PMUMEM_REQ_SHIFT)          /* 0x00004000 */
#define CRU_SOFTRST4_CON_PRESETN_GPIO2_REQ_SHIFT           (15U)
#define CRU_SOFTRST4_CON_PRESETN_GPIO2_REQ_MASK            (0x1U << CRU_SOFTRST4_CON_PRESETN_GPIO2_REQ_SHIFT)           /* 0x00008000 */
/* SOFTRST5_CON */
#define CRU_SOFTRST5_CON_OFFSET                            (0x314)
#define CRU_SOFTRST5_CON_HRESETN_BTSLV_NOC_REQ_SHIFT       (0U)
#define CRU_SOFTRST5_CON_HRESETN_BTSLV_NOC_REQ_MASK        (0x1U << CRU_SOFTRST5_CON_HRESETN_BTSLV_NOC_REQ_SHIFT)       /* 0x00000001 */
#define CRU_SOFTRST5_CON_RESETN_GPIO0_DB_REQ_SHIFT         (1U)
#define CRU_SOFTRST5_CON_RESETN_GPIO0_DB_REQ_MASK          (0x1U << CRU_SOFTRST5_CON_RESETN_GPIO0_DB_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST5_CON_RESETN_GPIO1_DB_REQ_SHIFT         (2U)
#define CRU_SOFTRST5_CON_RESETN_GPIO1_DB_REQ_MASK          (0x1U << CRU_SOFTRST5_CON_RESETN_GPIO1_DB_REQ_SHIFT)         /* 0x00000004 */
#define CRU_SOFTRST5_CON_RESETN_GPIO2_DB_REQ_SHIFT         (3U)
#define CRU_SOFTRST5_CON_RESETN_GPIO2_DB_REQ_MASK          (0x1U << CRU_SOFTRST5_CON_RESETN_GPIO2_DB_REQ_SHIFT)         /* 0x00000008 */
#define CRU_SOFTRST5_CON_RESETN_EFUSE_REQ_SHIFT            (4U)
#define CRU_SOFTRST5_CON_RESETN_EFUSE_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_RESETN_EFUSE_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST5_CON_RESETN_BBGEN_REQ_SHIFT            (5U)
#define CRU_SOFTRST5_CON_RESETN_BBGEN_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_RESETN_BBGEN_REQ_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST5_CON_PRESENT_BBGEN_REQ_SHIFT           (6U)
#define CRU_SOFTRST5_CON_PRESENT_BBGEN_REQ_MASK            (0x1U << CRU_SOFTRST5_CON_PRESENT_BBGEN_REQ_SHIFT)           /* 0x00000040 */
#define CRU_SOFTRST5_CON_PRESETN_I2CS_REQ_SHIFT            (7U)
#define CRU_SOFTRST5_CON_PRESETN_I2CS_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_PRESETN_I2CS_REQ_SHIFT)            /* 0x00000080 */
#define CRU_SOFTRST5_CON_RKFE_NRST_REQ_SHIFT               (8U)
#define CRU_SOFTRST5_CON_RKFE_NRST_REQ_MASK                (0x1U << CRU_SOFTRST5_CON_RKFE_NRST_REQ_SHIFT)               /* 0x00000100 */
#define CRU_SOFTRST5_CON_SPI2APB_NRST_REQ_SHIFT            (9U)
#define CRU_SOFTRST5_CON_SPI2APB_NRST_REQ_MASK             (0x1U << CRU_SOFTRST5_CON_SPI2APB_NRST_REQ_SHIFT)            /* 0x00000200 */
#define CRU_SOFTRST5_CON_HRESETN_BT_TEST_REQ_SHIFT         (10U)
#define CRU_SOFTRST5_CON_HRESETN_BT_TEST_REQ_MASK          (0x1U << CRU_SOFTRST5_CON_HRESETN_BT_TEST_REQ_SHIFT)         /* 0x00000400 */
#define CRU_SOFTRST5_CON_RESETN_PVTM_REQ_SHIFT             (11U)
#define CRU_SOFTRST5_CON_RESETN_PVTM_REQ_MASK              (0x1U << CRU_SOFTRST5_CON_RESETN_PVTM_REQ_SHIFT)             /* 0x00000800 */
#define CRU_SOFTRST5_CON_PRESETN_TOPNOC_REQ_SHIFT          (12U)
#define CRU_SOFTRST5_CON_PRESETN_TOPNOC_REQ_MASK           (0x1U << CRU_SOFTRST5_CON_PRESETN_TOPNOC_REQ_SHIFT)          /* 0x00001000 */
/* GLB_SRST_FST_VALUE */
#define CRU_GLB_SRST_FST_VALUE_OFFSET                      (0x400)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_SHIFT    (0U)
#define CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_MASK     (0xFFFFU << CRU_GLB_SRST_FST_VALUE_GLB_SRST_FST_VALUE_SHIFT) /* 0x0000FFFF */
/* GLB_SRST_SND_VALUE */
#define CRU_GLB_SRST_SND_VALUE_OFFSET                      (0x404)
#define CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_VALUE_SHIFT    (0U)
#define CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_VALUE_MASK     (0xFFFFU << CRU_GLB_SRST_SND_VALUE_GLB_SRST_SND_VALUE_SHIFT) /* 0x0000FFFF */
/* GLB_CNT_TH */
#define CRU_GLB_CNT_TH_OFFSET                              (0x408)
#define CRU_GLB_CNT_TH_GLB_RST_CNT_TH_SHIFT                (0U)
#define CRU_GLB_CNT_TH_GLB_RST_CNT_TH_MASK                 (0x3FFU << CRU_GLB_CNT_TH_GLB_RST_CNT_TH_SHIFT)              /* 0x000003FF */
/* MISC_CON */
#define CRU_MISC_CON_OFFSET                                (0x40C)
#define CRU_MISC_CON_TESTCLK_SEL_SHIFT                     (0U)
#define CRU_MISC_CON_TESTCLK_SEL_MASK                      (0xFU << CRU_MISC_CON_TESTCLK_SEL_SHIFT)                     /* 0x0000000F */
#define CRU_MISC_CON_CLK_TEST_DIV_CON_SHIFT                (4U)
#define CRU_MISC_CON_CLK_TEST_DIV_CON_MASK                 (0xFU << CRU_MISC_CON_CLK_TEST_DIV_CON_SHIFT)                /* 0x000000F0 */
#define CRU_MISC_CON_M3_SYSRESET_REQ_EN_SHIFT              (8U)
#define CRU_MISC_CON_M3_SYSRESET_REQ_EN_MASK               (0x1U << CRU_MISC_CON_M3_SYSRESET_REQ_EN_SHIFT)              /* 0x00000100 */
#define CRU_MISC_CON_M3_LOCKUP_RESET_REQ_EN_SHIFT          (9U)
#define CRU_MISC_CON_M3_LOCKUP_RESET_REQ_EN_MASK           (0x1U << CRU_MISC_CON_M3_LOCKUP_RESET_REQ_EN_SHIFT)          /* 0x00000200 */
/* GLB_RST_CON */
#define CRU_GLB_RST_CON_OFFSET                             (0x410)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT            (0U)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK             (0x1U << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT)            /* 0x00000001 */
#define CRU_GLB_RST_CON_PMU_GLB_SRST_CTRL_SHIFT            (1U)
#define CRU_GLB_RST_CON_PMU_GLB_SRST_CTRL_MASK             (0x3U << CRU_GLB_RST_CON_PMU_GLB_SRST_CTRL_SHIFT)            /* 0x00000006 */
#define CRU_GLB_RST_CON_PMU_GLBRST_WDT_CTRL_SHIFT          (3U)
#define CRU_GLB_RST_CON_PMU_GLBRST_WDT_CTRL_MASK           (0x1U << CRU_GLB_RST_CON_PMU_GLBRST_WDT_CTRL_SHIFT)          /* 0x00000008 */
#define CRU_GLB_RST_CON_GPIO_GLB_SRST_CTRL_SHIFT           (4U)
#define CRU_GLB_RST_CON_GPIO_GLB_SRST_CTRL_MASK            (0x1U << CRU_GLB_RST_CON_GPIO_GLB_SRST_CTRL_SHIFT)           /* 0x00000010 */
#define CRU_GLB_RST_CON_GRF_GLB_SRST_CTRL_SHIFT            (5U)
#define CRU_GLB_RST_CON_GRF_GLB_SRST_CTRL_MASK             (0x1U << CRU_GLB_RST_CON_GRF_GLB_SRST_CTRL_SHIFT)            /* 0x00000020 */
/* GLB_RST_ST */
#define CRU_GLB_RST_ST_OFFSET                              (0x414)
#define CRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT                (0U)
#define CRU_GLB_RST_ST_FST_GLB_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT)                /* 0x00000001 */
#define CRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT                (1U)
#define CRU_GLB_RST_ST_SND_GLB_RST_ST_MASK                 (0x1U << CRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT)                /* 0x00000002 */
#define CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT            (2U)
#define CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_MASK             (0x1U << CRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT)            /* 0x00000004 */
#define CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT            (3U)
#define CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_MASK             (0x1U << CRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT)            /* 0x00000008 */

#define PLL_INPUT_OSC_RATE       (26 * MHZ)

#ifndef __ASSEMBLY__
typedef enum PD_Id {
    PD_INVALID          = 0U,
    PD_DSP              = 0x80000000U,
    PD_MCU              = 0x80011111U,
    PD_BT_AUDIO         = 0x80022222U,
} ePD_Id;
#endif

/********Name=SOFTRST0_CON,Offset=0x300********/
#define SRST_M3_AUTOCLR          0U
#define SRST_S_M3_AUTOCLR        1U
#define SRST_H_MCU_NOC_AUTOCLR   2U
#define SRST_P_TOPNOC_AUTOCLR    3U
/********Name=SOFTRST1_CON,Offset=0x304********/
#define SRST_M3                  16U
#define SRST_S_M3                17U
#define SRST_H_MCU_NOC           18U
#define SRST_H_INTMEM0           19U
#define SRST_H_INTMEM1           20U
#define SRST_H_ROM               21U
#define SRST_A_DMA               22U
#define SRST_H_SFC               23U
#define SRST_P_WDT0              24U
#define SRST_P_WDT1              25U
#define SRST_P_UART0             26U
#define SRST_P_UART1             27U
#define SRST_P_PWM               28U
#define SRST_H_SFC_XIP           29U
#define SRST_I2S_RX              30U
#define SRST_P_EFUSE             31U
/********Name=SOFTRST2_CON,Offset=0x308********/
#define SRST_P_MAILBOX           32U
#define SRST_P_SPI               34U
#define SRST_P_I2C0              35U
#define SRST_P_I2C1              36U
#define SRST_SFC                 37U
#define SRST_UART0               38U
#define SRST_UART1               39U
#define SRST_PWM                 40U
#define SRST_TIMER0              42U
#define SRST_TIMER1              43U
#define SRST_TIMER2              44U
#define SRST_TIMER3              45U
#define SRST_TIMER4              46U
#define SRST_SPI                 47U
/********Name=SOFTRST3_CON,Offset=0x30C********/
#define SRST_I2C0                48U
#define SRST_I2C1                49U
#define SRST_TRSTN_M3_JTAG       50U
#define SRST_P_GPIO1             51U
#define SRST_P_TIMER             52U
#define SRST_H_BT_NOC            53U
#define SRST_H_BTDM              54U
#define SRST_H_I2S               55U
#define SRST_H_PDM               56U
#define SRST_I2S_TX              59U
#define SRST_PDM                 60U
#define SRST_RF                  63U
/********Name=SOFTRST4_CON,Offset=0x310********/
#define SRST_A_DSP_NOC           64U
#define SRST_DSP                 65U
#define SRST_DSP_EXTCORE         66U
#define SRST_P_PMU_NOC           67U
#define SRST_P_GPIO0             68U
#define SRST_P_GRF               69U
#define SRST_P_I2C2              71U
#define SRST_I2C2                73U
#define SRST_PMU                 74U
#define SRST_H_INTC0             75U
#define SRST_MDM_NRST            76U
#define SRST_BB_NRST             77U
#define SRST_P_PMUMEM            78U
#define SRST_P_GPIO2             79U
/********Name=SOFTRST5_CON,Offset=0x314********/
#define SRST_H_BTSLV_NOC         80U
#define SRST_GPIO0_DB            81U
#define SRST_GPIO1_DB            82U
#define SRST_GPIO2_DB            83U
#define SRST_EFUSE               84U
#define SRST_BBGEN               85U
#define SRST_P_BBGEN             86U
#define SRST_P_I2CS              87U
#define SRST_RKFE_NRST           88U
#define SRST_SPI2APB_NRST        89U
#define SRST_H_BT_TEST           90U
#define SRST_PVTM                91U
#define SRST_P_TOPNOC            92U

/********Name=CLKGATE0_CON,Offset=0x200********/
#define CLK_32K_SRC_GATE         0U
#define HCLK_MCU_SRC_GATE        1U
#define PCLK_MCU_SRC_GATE        2U
#define CLK_SFC_SRC_GATE         3U
#define CLK_TIMER0_GATE          4U
#define CLK_TIMER1_GATE          5U
#define CLK_TIMER2_GATE          6U
#define CLK_TIMER3_GATE          7U
#define CLK_TIMER4_GATE          8U
#define CLK_UART0_SRC_GATE       9U
#define CLK_UART0_FRAC_SRC_GATE  10U
#define CLK_UART1_SRC_GATE       11U
#define CLK_UART1_FRAC_SRC_GATE  12U
#define CLK_PWM_SRC_GATE         13U
#define CLK_PVTM_GATE            14U
#define CLK_SPI1_GATE            15U
/********Name=CLKGATE1_CON,Offset=0x204********/
#define CLK_I2C0_SRC_GATE        16U
#define CLK_I2C1_SRC_GATE        17U
#define HCLK_BT_SRC_GATE         18U
#define CLK_I2S_TX_SRC_GATE      20U
#define CLK_I2S_TX_FRAC_SRC_GATE 21U
#define CLK_PDM_SRC_GATE         22U
#define CLK_PDM_FRAC_SRC_GATE    23U
#define CLK_DSP_SRC_GATE         26U
#define PCLK_PMU_SRC_GATE        28U
#define CLK_I2C2_SRC_GATE        29U
#define TESTCLK_GATE             30U
#define CLK_BT52M_SRC_GATE       31U
/********Name=CLKGATE2_CON,Offset=0x208********/
#define HCLK_MCU_NOC_GATE        32U
#define FCLK_M3_GATE             33U
#define HCLK_M3_GATE             34U
#define HCLK_INTMEM0_GATE        35U
#define HCLK_INTMEM1_GATE        36U
#define HCLK_ROM_GATE            37U
#define HCLK_SFC_GATE            38U
#define ACLK_DMAC_GATE           39U
#define PCLK_TIMER_GATE          40U
#define PCLK_WDT0_GATE           41U
#define PCLK_WDT1_GATE           42U
#define PCLK_UART0_GATE          43U
#define PCLK_UART1_GATE          44U
#define PCLK_PWM_GATE            45U
#define PCLK_MAILBOX_GATE        46U
#define HCLK_SFC_XIP_GATE        47U
/********Name=CLKGATE3_CON,Offset=0x20C********/
#define PCLK_SPI1_GATE            49U
#define PCLK_I2C0_GATE           50U
#define PCLK_I2C1_GATE           51U
#define PCLK_GPIO1_GATE          52U
#define HCLK_BT_NOC_GATE         53U
#define HCLK_BTDM_GATE           54U
#define HCLK_I2S_GATE            55U
#define HCLK_PDM_GATE            56U
#define HCLK_BTSLV_NOC_GATE      57U
#define ACLK_DSP_NOC_GATE        58U
#define CLK_DSP_GATE             59U
#define PCLK_EFUSE_GATE          60U
#define HCLK_INTC0_GATE          61U
#define HCLK_BT_TEST_GATE        62U
#define PCLK_GPIO2_GATE          63U

#define ACLK_DSP_GATE            ACLK_DSP_NOC_GATE
#define PCLK_DSP_GATE            CLK_DSP_GATE

/********Name=CLKGATE4_CON,Offset=0x210********/
#define PCLK_PMU_NOC_GATE        64U
#define PCLK_PMUIP_GATE          65U
#define PCLK_GPIO0_GATE          66U
#define PCLK_GRF_GATE            67U
#define PCLK_I2C2_GATE           70U
#define PCLK_PMUMEM_GATE         71U
#define PCLK_BBGENIP_GATE        72U
#define PCLK_I2CS_GATE           73U
#define RFPLL_CLK_GATE           74U
#define PCLK_TOPNOC_GATE         75U
/********Name=CLKGATE5_CON,Offset=0x214********/
#define CLK_I2S_RX_SRC_GATE      80U
#define CLK_I2S_RX_FRAC_SRC_GATE 81U
#define CLK_GPIO0_DB_GATE        82U
#define CLK_GPIO1_DB_GATE        83U
#define CLK_GPIO2_DB_GATE        84U
#define CLK_EFUSE_GATE           85U
#define CLK_BBGEN_SRC_GATE       86U
#define CLK_REF_BBGEN_GATE       87U
#define PCLK_BBGEN_SRC_GATE      88U
#define CLK_LOW_POWER_GATE       89U
#define CLK32K_OSC_FRAC_SRC_GATE 90U

/********Name=CLKSEL0_CON,Offset=0x100********/
/********Name=CLKSEL1_CON,Offset=0x104********/
#define HCLK_MCU_DIV                0x04000001U
#define PCLK_MCU_DIV                0x03080001U
#define STCLK_M3_DIV                0x040C0001U
/********Name=CLKSEL2_CON,Offset=0x108********/
#define UART0_DIV                   0x04080002U
/********Name=CLKSEL3_CON,Offset=0x10C********/
#define UART1_DIV                   0x04000003U
#define CLK_PWM_DIV                 0x04080003U
/********Name=CLKSEL4_CON,Offset=0x110********/
#define SPI_DIV                     0x04000004U
#define CLK_I2C0_DIV                0x04080004U
/********Name=CLKSEL5_CON,Offset=0x114********/
#define CLK_BT52M_DIV               0x04000005U
#define HCLK_BT_DIV                 0x04080005U
/********Name=CLKSEL6_CON,Offset=0x118********/
#define I2S_TX_DIV                  0x04000006U
#define I2S_RX_DIV                  0x04080006U
/********Name=CLKSEL7_CON,Offset=0x11C********/
#define CLK_PDM_DIV                 0x04000007U
#define CLK_SFC_DIV                 0x04080007U
/********Name=CLKSEL8_CON,Offset=0x120********/
#define CLK_DSP_DIV                 0x04000008U
#define PCLK_PMU_DIV                0x05080008U
/********Name=CLKSEL9_CON,Offset=0x124********/
#define CLK_I2C2_DIV                0x04000009U
/********Name=CLKSEL10_CON,Offset=0x128********/
#define UART0_FRAC_DIV              0x2000000AU
/********Name=CLKSEL11_CON,Offset=0x12C********/
#define UART1_FRAC_DIV              0x2000000BU
/********Name=CLKSEL12_CON,Offset=0x130********/
#define I2S_TX_FRAC_DIV             0x2000000CU
/********Name=CLKSEL13_CON,Offset=0x134********/
#define PDM_FRAC_DIV                0x2000000DU
/********Name=CLKSEL14_CON,Offset=0x138********/
#define I2S_RX_FRAC_DIV             0x2000000EU
/********Name=CLKSEL15_CON,Offset=0x13C********/
#define CLK_EFUSE_DIV               0x0700000FU
#define CLK_I2C1_DIV                0x0408000FU
/********Name=CLKSEL16_CON,Offset=0x140********/
#define CLK_BBGEN_DIV               0x04000010U
/********Name=CLKSEL17_CON,Offset=0x144********/
#define CLK32K_FRAC_DIV             0x20000011U

/********Name=CLKSEL0_CON,Offset=0x100********/
#define CLK_32K_SRC_SEL             0x020C0000U
#define CLK_32K_SRC_SEL_IO          0U
#define CLK_32K_SRC_SEL_XIN_OSC     1U
#define CLK_32K_SRC_SEL_32K         2U
/********Name=CLKSEL1_CON,Offset=0x104********/
#define HCLK_MCU_SRC_SEL            0x01070001U
#define HCLK_MCU_SRC_SEL_CPLL       0U
#define HCLK_MCU_SRC_SEL_GPLL       1U
/********Name=CLKSEL2_CON,Offset=0x108********/
#define CLK_TIMER0_SRC_SEL          0x01000002U
#define CLK_TIMER0_SRC_SEL_XIN_OSC  0U
#define CLK_TIMER0_SRC_SEL_32KHZ    1U
#define CLK_TIMER1_SRC_SEL          0x01010002U
#define CLK_TIMER1_SRC_SEL_XIN_OSC  0U
#define CLK_TIMER1_SRC_SEL_32KHZ    1U
#define CLK_TIMER2_SRC_SEL          0x01020002U
#define CLK_TIMER2_SRC_SEL_XIN_OSC  0U
#define CLK_TIMER2_SRC_SEL_32KHZ    1U
#define CLK_TIMER3_SRC_SEL          0x01030002U
#define CLK_TIMER3_SRC_SEL_XIN_OSC  0U
#define CLK_TIMER3_SRC_SEL_32KHZ    1U
#define CLK_TIMER4_SRC_SEL          0x01040002U
#define CLK_TIMER4_SRC_SEL_XIN_OSC  0U
#define CLK_TIMER4_SRC_SEL_32KHZ    1U
#define CLK_PVTM_SRC_SEL            0x01050002U
#define CLK_PVTM_SRC_SEL_XIN_OSC    0U
#define CLK_PVTM_SRC_SEL_32KHZ      1U
#define UART0_CLK_SEL               0x020C0002U
#define UART0_CLK_SEL_INTEGRATION   0U
#define UART0_CLK_SEL_FRACTION      1U
#define UART0_CLK_SEL_XIN_OSC       2U
#define UART0_CLK_SRC_SEL           0x010F0002U
#define UART0_CLK_SRC_SEL_CPLL      0U
#define UART0_CLK_SRC_SEL_GPLL      1U
/********Name=CLKSEL3_CON,Offset=0x10C********/
#define UART1_CLK_SEL               0x02040003U
#define UART1_CLK_SEL_INTEGRATION   0U
#define UART1_CLK_SEL_FRACTION      1U
#define UART1_CLK_SEL_XIN_OSC       2U
#define UART1_CLK_SRC_SEL           0x01070003U
#define UART1_CLK_SRC_SEL_CPLL      0U
#define UART1_CLK_SRC_SEL_GPLL      1U
/********Name=CLKSEL4_CON,Offset=0x110********/
/********Name=CLKSEL5_CON,Offset=0x114********/
#define CLK_BT52M_SRC_SEL           0x02060005U
#define CLK_BT52M_SRC_SEL_CPLL      0U
#define CLK_BT52M_SRC_SEL_GPLL      1U
#define CLK_BT52M_SRC_SEL_RFPLL     2U
#define HCLK_BT_SRC_SEL             0x010F0005U
#define HCLK_BT_SRC_SEL_CPLL        0U
#define HCLK_BT_SRC_SEL_GPLL        1U
/********Name=CLKSEL6_CON,Offset=0x118********/
#define I2S_TX_CLK_SEL              0x02040006U
#define I2S_TX_CLK_SEL_INTEGRATION  0U
#define I2S_TX_CLK_SEL_FRACTION     1U
#define I2S_TX_CLK_SEL_IO           2U
#define I2S_TX_CLK_SEL_XIN_OSC      3U
#define I2S_TX_CLK_SRC_SEL          0x01070006U
#define I2S_TX_CLK_SRC_SEL_CPLL     0U
#define I2S_TX_CLK_SRC_SEL_GPLL     1U
#define I2S_RX_CLK_SEL              0x020C0006U
#define I2S_RX_CLK_SEL_INTEGRATION  0U
#define I2S_RX_CLK_SEL_FRACTION     1U
#define I2S_RX_CLK_SEL_IO           2U
#define I2S_RX_CLK_SEL_XIN_OSC      3U
#define I2S_RX_CLK_SRC_SEL          0x010F0006U
#define I2S_RX_CLK_SRC_SEL_CPLL     0U
#define I2S_RX_CLK_SRC_SEL_GPLL     1U
/********Name=CLKSEL7_CON,Offset=0x11C********/
#define CLK_PDM_SEL                 0x02040007U
#define CLK_PDM_SEL_INTEGRATION     0U
#define CLK_PDM_SEL_FRACTION        1U
#define CLK_PDM_SEL_IO              2U
#define CLK_PDM_SEL_XIN_OSC         3U
#define CLK_PDM_SRC_SEL             0x01070007U
#define CLK_PDM_SRC_SEL_CPLL        0U
#define CLK_PDM_SRC_SEL_GPLL        1U
#define CLK_SFC_SRC_SEL             0x010F0007U
#define CLK_SFC_SRC_SEL_CPLL        0U
#define CLK_SFC_SRC_SEL_GPLL        1U
/********Name=CLKSEL8_CON,Offset=0x120********/
#define CLK_DSP_SRC_SEL             0x01070008U
#define CLK_DSP_SRC_SEL_CPLL        0U
#define CLK_DSP_SRC_SEL_GPLL        1U
/********Name=CLKSEL9_CON,Offset=0x124********/
#define CLK_GPIO0_DB_SEL            0x01080009U
#define CLK_GPIO0_DB_SEL_XIN_OSC    0U
#define CLK_GPIO0_DB_SEL_32KHZ      1U
#define CLK_GPIO1_DB_SEL            0x01090009U
#define CLK_GPIO1_DB_SEL_XIN_OSC    0U
#define CLK_GPIO1_DB_SEL_32KHZ      1U
#define CLK_GPIO2_DB_SEL            0x010A0009U
#define CLK_GPIO2_DB_SEL_XIN_OSC    0U
#define CLK_GPIO2_DB_SEL_32KHZ      1U
/********Name=CLKSEL10_CON,Offset=0x128********/
/********Name=CLKSEL11_CON,Offset=0x12C********/
/********Name=CLKSEL12_CON,Offset=0x130********/
/********Name=CLKSEL13_CON,Offset=0x134********/
/********Name=CLKSEL14_CON,Offset=0x138********/
/********Name=CLKSEL15_CON,Offset=0x13C********/
#define CLK_EFUSE_SEL               0x0107000FU
#define CLK_EFUSE_SEL_XIN_OSC       0U
#define CLK_EFUSE_SEL_GPLL          1U
/********Name=CLKSEL16_CON,Offset=0x140********/
#define CLK_BBGEN_SEL               0x01070010U
#define CLK_BBGEN_SEL_XIN_OSC       0U
#define CLK_BBGEN_SEL_GPLL          1U
/********Name=CLKSEL17_CON,Offset=0x144********/

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_CPLL,
    PLL_GPLL,
    CLK_32K              = CLK(CLK_32K_SRC_SEL, 0U),
    HCLK_MCU             = CLK(HCLK_MCU_SRC_SEL, HCLK_MCU_DIV),
    PCLK_MCU             = CLK(0U, PCLK_MCU_DIV),
    STCLK_M3             = CLK(0U, STCLK_M3_DIV),
    CLK_TIMER0           = CLK(CLK_TIMER0_SRC_SEL, 0U),
    CLK_TIMER1           = CLK(CLK_TIMER1_SRC_SEL, 0U),
    CLK_TIMER2           = CLK(CLK_TIMER2_SRC_SEL, 0U),
    CLK_TIMER3           = CLK(CLK_TIMER3_SRC_SEL, 0U),
    CLK_TIMER4           = CLK(CLK_TIMER4_SRC_SEL, 0U),
    CLK_PVTM             = CLK(CLK_PVTM_SRC_SEL, 0U),
    CLK_UART0_SRC        = CLK(UART0_CLK_SRC_SEL, UART0_DIV),
    CLK_UART0_FRAC       = CLK(0U, UART0_FRAC_DIV),
    CLK_UART0            = CLK(UART0_CLK_SEL, 0U),
    CLK_UART1_SRC        = CLK(UART1_CLK_SRC_SEL, UART1_DIV),
    CLK_UART1_FRAC       = CLK(0U, UART1_FRAC_DIV),
    CLK_UART1            = CLK(UART1_CLK_SEL, 0U),
    CLK_PWM              = CLK(0U, CLK_PWM_DIV),
    CLK_SPI1             = CLK(0U, SPI_DIV),
    CLK_I2C0             = CLK(0U, CLK_I2C0_DIV),
    CLK_BT52M            = CLK(CLK_BT52M_SRC_SEL, CLK_BT52M_DIV),
    HCLK_BT              = CLK(HCLK_BT_SRC_SEL, HCLK_BT_DIV),
    CLK_I2S_TX_SRC       = CLK(I2S_TX_CLK_SRC_SEL, I2S_TX_DIV),
    CLK_I2S_TX_FRAC      = CLK(0U, I2S_TX_FRAC_DIV),
    CLK_I2S_TX           = CLK(I2S_TX_CLK_SEL, 0U),
    CLK_I2S_RX_SRC       = CLK(I2S_RX_CLK_SRC_SEL, I2S_RX_DIV),
    CLK_I2S_RX_FRAC      = CLK(0U, I2S_RX_FRAC_DIV),
    CLK_I2S_RX           = CLK(I2S_RX_CLK_SEL, 0U),
    CLK_PDM_SRC          = CLK(CLK_PDM_SRC_SEL, CLK_PDM_DIV),
    CLK_PDM_FRAC         = CLK(0U, PDM_FRAC_DIV),
    CLK_PDM              = CLK(CLK_PDM_SEL, 0U),
    CLK_SFC              = CLK(CLK_SFC_SRC_SEL, CLK_SFC_DIV),
    CLK_DSP              = CLK(CLK_DSP_SRC_SEL, CLK_DSP_DIV),
    PCLK_PMU             = CLK(0U, PCLK_PMU_DIV),
    CLK_I2C2             = CLK(0U, CLK_I2C2_DIV),
    CLK_GPIO0            = CLK(CLK_GPIO0_DB_SEL, 0U),
    CLK_GPIO1            = CLK(CLK_GPIO1_DB_SEL, 0U),
    CLK_GPIO2            = CLK(CLK_GPIO2_DB_SEL, 0U),
    CLK_EFUSE            = CLK(CLK_EFUSE_SEL, CLK_EFUSE_DIV),
    CLK_I2C1             = CLK(0U, CLK_I2C1_DIV),
    CLK_BBGEN            = CLK(CLK_BBGEN_SEL, CLK_BBGEN_DIV),
} eCLOCK_Name;

#define CPU_CLK_ID       HCLK_MCU
#define MEM_CLK_ID       -1
#define DSP_CLK_ID       CLK_DSP

#endif /* __ASSEMBLY__ */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __KOALA_H */

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */


#ifndef __RK3528_H
#define __RK3528_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* GPIO0_IOC Register Structure Define */
struct GPIO0_IOC_REG {
    __IO uint32_t GPIO0A_IOMUX_SEL_L;                 /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0A_IOMUX_SEL_H;                 /* Address Offset: 0x0004 */
         uint32_t RESERVED0008[62];                   /* Address Offset: 0x0008 */
    __IO uint32_t GPIO0A_DS_0;                        /* Address Offset: 0x0100 */
    __IO uint32_t GPIO0A_DS_1;                        /* Address Offset: 0x0104 */
    __IO uint32_t GPIO0A_DS_2;                        /* Address Offset: 0x0108 */
         uint32_t RESERVED010C[61];                   /* Address Offset: 0x010C */
    __IO uint32_t GPIO0A_PULL;                        /* Address Offset: 0x0200 */
         uint32_t RESERVED0204[63];                   /* Address Offset: 0x0204 */
    __IO uint32_t GPIO0A_IE;                          /* Address Offset: 0x0300 */
         uint32_t RESERVED0304[63];                   /* Address Offset: 0x0304 */
    __IO uint32_t GPIO0A_SMT;                         /* Address Offset: 0x0400 */
         uint32_t RESERVED0404[63];                   /* Address Offset: 0x0404 */
    __IO uint32_t GPIO0A_SUS;                         /* Address Offset: 0x0500 */
         uint32_t RESERVED0504[63];                   /* Address Offset: 0x0504 */
    __IO uint32_t GPIO0A_SL;                          /* Address Offset: 0x0600 */
         uint32_t RESERVED0604[63];                   /* Address Offset: 0x0604 */
    __IO uint32_t GPIO0A_OD;                          /* Address Offset: 0x0700 */
    __IO uint32_t IO5V_CTRL;                          /* Address Offset: 0x0704 */
};
/* GPIO4_IOC Register Structure Define */
struct GPIO4_IOC_REG {
         uint32_t RESERVED0000[32];                   /* Address Offset: 0x0000 */
    __IO uint32_t GPIO4A_IOMUX_SEL_L;                 /* Address Offset: 0x0080 */
    __IO uint32_t GPIO4A_IOMUX_SEL_H;                 /* Address Offset: 0x0084 */
    __IO uint32_t GPIO4B_IOMUX_SEL_L;                 /* Address Offset: 0x0088 */
    __IO uint32_t GPIO4B_IOMUX_SEL_H;                 /* Address Offset: 0x008C */
    __IO uint32_t GPIO4C_IOMUX_SEL_L;                 /* Address Offset: 0x0090 */
    __IO uint32_t GPIO4C_IOMUX_SEL_H;                 /* Address Offset: 0x0094 */
    __IO uint32_t GPIO4D_IOMUX_SEL_L;                 /* Address Offset: 0x0098 */
         uint32_t RESERVED009C[73];                   /* Address Offset: 0x009C */
    __IO uint32_t GPIO4A_DS_0;                        /* Address Offset: 0x01C0 */
    __IO uint32_t GPIO4A_DS_1;                        /* Address Offset: 0x01C4 */
    __IO uint32_t GPIO4A_DS_2;                        /* Address Offset: 0x01C8 */
    __IO uint32_t GPIO4A_DS_3;                        /* Address Offset: 0x01CC */
    __IO uint32_t GPIO4B_DS_0;                        /* Address Offset: 0x01D0 */
    __IO uint32_t GPIO4B_DS_1;                        /* Address Offset: 0x01D4 */
    __IO uint32_t GPIO4B_DS_2;                        /* Address Offset: 0x01D8 */
    __IO uint32_t GPIO4B_DS_3;                        /* Address Offset: 0x01DC */
    __IO uint32_t GPIO4C_DS_0;                        /* Address Offset: 0x01E0 */
    __IO uint32_t GPIO4C_DS_1;                        /* Address Offset: 0x01E4 */
    __IO uint32_t GPIO4C_DS_2;                        /* Address Offset: 0x01E8 */
    __IO uint32_t GPIO4C_DS_3;                        /* Address Offset: 0x01EC */
    __IO uint32_t GPIO4D_DS_0;                        /* Address Offset: 0x01F0 */
         uint32_t RESERVED01F4[19];                   /* Address Offset: 0x01F4 */
    __IO uint32_t GPIO4A_PULL;                        /* Address Offset: 0x0240 */
    __IO uint32_t GPIO4B_PULL;                        /* Address Offset: 0x0244 */
    __IO uint32_t GPIO4C_PULL;                        /* Address Offset: 0x0248 */
    __IO uint32_t GPIO4D_PULL;                        /* Address Offset: 0x024C */
         uint32_t RESERVED0250[60];                   /* Address Offset: 0x0250 */
    __IO uint32_t GPIO4A_IE;                          /* Address Offset: 0x0340 */
    __IO uint32_t GPIO4B_IE;                          /* Address Offset: 0x0344 */
    __IO uint32_t GPIO4C_IE;                          /* Address Offset: 0x0348 */
    __IO uint32_t GPIO4D_IE;                          /* Address Offset: 0x034C */
         uint32_t RESERVED0350[60];                   /* Address Offset: 0x0350 */
    __IO uint32_t GPIO4A_SMT;                         /* Address Offset: 0x0440 */
    __IO uint32_t GPIO4B_SMT;                         /* Address Offset: 0x0444 */
    __IO uint32_t GPIO4C_SMT;                         /* Address Offset: 0x0448 */
    __IO uint32_t GPIO4D_SMT;                         /* Address Offset: 0x044C */
         uint32_t RESERVED0450[60];                   /* Address Offset: 0x0450 */
    __IO uint32_t GPIO4A_SUS;                         /* Address Offset: 0x0540 */
    __IO uint32_t GPIO4B_SUS;                         /* Address Offset: 0x0544 */
    __IO uint32_t GPIO4C_SUS;                         /* Address Offset: 0x0548 */
    __IO uint32_t GPIO4D_SUS;                         /* Address Offset: 0x054C */
         uint32_t RESERVED0550[60];                   /* Address Offset: 0x0550 */
    __IO uint32_t GPIO4A_SL;                          /* Address Offset: 0x0640 */
    __IO uint32_t GPIO4B_SL;                          /* Address Offset: 0x0644 */
    __IO uint32_t GPIO4C_SL;                          /* Address Offset: 0x0648 */
    __IO uint32_t GPIO4D_SL;                          /* Address Offset: 0x064C */
         uint32_t RESERVED0650[60];                   /* Address Offset: 0x0650 */
    __IO uint32_t GPIO4A_OD;                          /* Address Offset: 0x0740 */
    __IO uint32_t GPIO4B_OD;                          /* Address Offset: 0x0744 */
    __IO uint32_t GPIO4C_OD;                          /* Address Offset: 0x0748 */
    __IO uint32_t GPIO4D_OD;                          /* Address Offset: 0x074C */
         uint32_t RESERVED0750[64];                   /* Address Offset: 0x0750 */
    __IO uint32_t VCCIO4_POC;                         /* Address Offset: 0x0850 */
};
/* GPIO1_IOC Register Structure Define */
struct GPIO1_IOC_REG {
         uint32_t RESERVED0000[8];                    /* Address Offset: 0x0000 */
    __IO uint32_t GPIO1A_IOMUX_SEL_L;                 /* Address Offset: 0x0020 */
    __IO uint32_t GPIO1A_IOMUX_SEL_H;                 /* Address Offset: 0x0024 */
    __IO uint32_t GPIO1B_IOMUX_SEL_L;                 /* Address Offset: 0x0028 */
    __IO uint32_t GPIO1B_IOMUX_SEL_H;                 /* Address Offset: 0x002C */
    __IO uint32_t GPIO1C_IOMUX_SEL_L;                 /* Address Offset: 0x0030 */
    __IO uint32_t GPIO1C_IOMUX_SEL_H;                 /* Address Offset: 0x0034 */
    __IO uint32_t GPIO1D_IOMUX_SEL_L;                 /* Address Offset: 0x0038 */
    __IO uint32_t GPIO1D_IOMUX_SEL_H;                 /* Address Offset: 0x003C */
         uint32_t RESERVED0040[56];                   /* Address Offset: 0x0040 */
    __IO uint32_t GPIO1A_DS_0;                        /* Address Offset: 0x0120 */
    __IO uint32_t GPIO1A_DS_1;                        /* Address Offset: 0x0124 */
    __IO uint32_t GPIO1A_DS_2;                        /* Address Offset: 0x0128 */
    __IO uint32_t GPIO1A_DS_3;                        /* Address Offset: 0x012C */
    __IO uint32_t GPIO1B_DS_0;                        /* Address Offset: 0x0130 */
    __IO uint32_t GPIO1B_DS_1;                        /* Address Offset: 0x0134 */
    __IO uint32_t GPIO1B_DS_2;                        /* Address Offset: 0x0138 */
    __IO uint32_t GPIO1B_DS_3;                        /* Address Offset: 0x013C */
    __IO uint32_t GPIO1C_DS_0;                        /* Address Offset: 0x0140 */
    __IO uint32_t GPIO1C_DS_1;                        /* Address Offset: 0x0144 */
    __IO uint32_t GPIO1C_DS_2;                        /* Address Offset: 0x0148 */
    __IO uint32_t GPIO1C_DS_3;                        /* Address Offset: 0x014C */
    __IO uint32_t GPIO1D_DS_0;                        /* Address Offset: 0x0150 */
    __IO uint32_t GPIO1D_DS_1;                        /* Address Offset: 0x0154 */
    __IO uint32_t GPIO1D_DS_2;                        /* Address Offset: 0x0158 */
    __IO uint32_t GPIO1D_DS_3;                        /* Address Offset: 0x015C */
         uint32_t RESERVED0160[44];                   /* Address Offset: 0x0160 */
    __IO uint32_t GPIO1A_PULL;                        /* Address Offset: 0x0210 */
    __IO uint32_t GPIO1B_PULL;                        /* Address Offset: 0x0214 */
    __IO uint32_t GPIO1C_PULL;                        /* Address Offset: 0x0218 */
    __IO uint32_t GPIO1D_PULL;                        /* Address Offset: 0x021C */
         uint32_t RESERVED0220[60];                   /* Address Offset: 0x0220 */
    __IO uint32_t GPIO1A_IE;                          /* Address Offset: 0x0310 */
    __IO uint32_t GPIO1B_IE;                          /* Address Offset: 0x0314 */
    __IO uint32_t GPIO1C_IE;                          /* Address Offset: 0x0318 */
    __IO uint32_t GPIO1D_IE;                          /* Address Offset: 0x031C */
         uint32_t RESERVED0320[60];                   /* Address Offset: 0x0320 */
    __IO uint32_t GPIO1A_SMT;                         /* Address Offset: 0x0410 */
    __IO uint32_t GPIO1B_SMT;                         /* Address Offset: 0x0414 */
    __IO uint32_t GPIO1C_SMT;                         /* Address Offset: 0x0418 */
    __IO uint32_t GPIO1D_SMT;                         /* Address Offset: 0x041C */
         uint32_t RESERVED0420[60];                   /* Address Offset: 0x0420 */
    __IO uint32_t GPIO1A_SUS;                         /* Address Offset: 0x0510 */
    __IO uint32_t GPIO1B_SUS;                         /* Address Offset: 0x0514 */
    __IO uint32_t GPIO1C_SUS;                         /* Address Offset: 0x0518 */
    __IO uint32_t GPIO1D_SUS;                         /* Address Offset: 0x051C */
         uint32_t RESERVED0520[60];                   /* Address Offset: 0x0520 */
    __IO uint32_t GPIO1A_SL;                          /* Address Offset: 0x0610 */
    __IO uint32_t GPIO1B_SL;                          /* Address Offset: 0x0614 */
    __IO uint32_t GPIO1C_SL;                          /* Address Offset: 0x0618 */
    __IO uint32_t GPIO1D_SL;                          /* Address Offset: 0x061C */
         uint32_t RESERVED0620[60];                   /* Address Offset: 0x0620 */
    __IO uint32_t GPIO1A_OD;                          /* Address Offset: 0x0710 */
    __IO uint32_t GPIO1B_OD;                          /* Address Offset: 0x0714 */
    __IO uint32_t GPIO1C_OD;                          /* Address Offset: 0x0718 */
    __IO uint32_t GPIO1D_OD;                          /* Address Offset: 0x071C */
         uint32_t RESERVED0720[60];                   /* Address Offset: 0x0720 */
    __IO uint32_t VCCIO0_POC;                         /* Address Offset: 0x0810 */
         uint32_t RESERVED0814[3];                    /* Address Offset: 0x0814 */
    __IO uint32_t VCCIO1_POC;                         /* Address Offset: 0x0820 */
};
/* GPIO3_IOC Register Structure Define */
struct GPIO3_IOC_REG {
         uint32_t RESERVED0000[24];                   /* Address Offset: 0x0000 */
    __IO uint32_t GPIO3A_IOMUX_SEL_L;                 /* Address Offset: 0x0060 */
    __IO uint32_t GPIO3A_IOMUX_SEL_H;                 /* Address Offset: 0x0064 */
    __IO uint32_t GPIO3B_IOMUX_SEL_L;                 /* Address Offset: 0x0068 */
    __IO uint32_t GPIO3B_IOMUX_SEL_H;                 /* Address Offset: 0x006C */
    __IO uint32_t GPIO3C_IOMUX_SEL_L;                 /* Address Offset: 0x0070 */
         uint32_t RESERVED0074[71];                   /* Address Offset: 0x0074 */
    __IO uint32_t GPIO3A_DS_0;                        /* Address Offset: 0x0190 */
    __IO uint32_t GPIO3A_DS_1;                        /* Address Offset: 0x0194 */
    __IO uint32_t GPIO3A_DS_2;                        /* Address Offset: 0x0198 */
    __IO uint32_t GPIO3A_DS_3;                        /* Address Offset: 0x019C */
    __IO uint32_t GPIO3B_DS_0;                        /* Address Offset: 0x01A0 */
    __IO uint32_t GPIO3B_DS_1;                        /* Address Offset: 0x01A4 */
    __IO uint32_t GPIO3B_DS_2;                        /* Address Offset: 0x01A8 */
    __IO uint32_t GPIO3B_DS_3;                        /* Address Offset: 0x01AC */
    __IO uint32_t GPIO3C_DS_0;                        /* Address Offset: 0x01B0 */
    __IO uint32_t GPIO3C_DS_1;                        /* Address Offset: 0x01B4 */
         uint32_t RESERVED01B8[30];                   /* Address Offset: 0x01B8 */
    __IO uint32_t GPIO3A_PULL;                        /* Address Offset: 0x0230 */
    __IO uint32_t GPIO3B_PULL;                        /* Address Offset: 0x0234 */
    __IO uint32_t GPIO3C_PULL;                        /* Address Offset: 0x0238 */
         uint32_t RESERVED023C[61];                   /* Address Offset: 0x023C */
    __IO uint32_t GPIO3A_IE;                          /* Address Offset: 0x0330 */
    __IO uint32_t GPIO3B_IE;                          /* Address Offset: 0x0334 */
    __IO uint32_t GPIO3C_IE;                          /* Address Offset: 0x0338 */
         uint32_t RESERVED033C[61];                   /* Address Offset: 0x033C */
    __IO uint32_t GPIO3A_SMT;                         /* Address Offset: 0x0430 */
    __IO uint32_t GPIO3B_SMT;                         /* Address Offset: 0x0434 */
    __IO uint32_t GPIO3C_SMT;                         /* Address Offset: 0x0438 */
         uint32_t RESERVED043C[61];                   /* Address Offset: 0x043C */
    __IO uint32_t GPIO3A_SUS;                         /* Address Offset: 0x0530 */
    __IO uint32_t GPIO3B_SUS;                         /* Address Offset: 0x0534 */
    __IO uint32_t GPIO3C_SUS;                         /* Address Offset: 0x0538 */
         uint32_t RESERVED053C[61];                   /* Address Offset: 0x053C */
    __IO uint32_t GPIO3A_SL;                          /* Address Offset: 0x0630 */
    __IO uint32_t GPIO3B_SL;                          /* Address Offset: 0x0634 */
    __IO uint32_t GPIO3C_SL;                          /* Address Offset: 0x0638 */
         uint32_t RESERVED063C[61];                   /* Address Offset: 0x063C */
    __IO uint32_t GPIO3A_OD;                          /* Address Offset: 0x0730 */
    __IO uint32_t GPIO3B_OD;                          /* Address Offset: 0x0734 */
    __IO uint32_t GPIO3C_OD;                          /* Address Offset: 0x0738 */
         uint32_t RESERVED073C[65];                   /* Address Offset: 0x073C */
    __IO uint32_t VCCIO3_POC;                         /* Address Offset: 0x0840 */
};
/* GPIO2_IOC Register Structure Define */
struct GPIO2_IOC_REG {
         uint32_t RESERVED0000[16];                   /* Address Offset: 0x0000 */
    __IO uint32_t GPIO2A_IOMUX_SEL_L;                 /* Address Offset: 0x0040 */
    __IO uint32_t GPIO2A_IOMUX_SEL_H;                 /* Address Offset: 0x0044 */
         uint32_t RESERVED0048[70];                   /* Address Offset: 0x0048 */
    __IO uint32_t GPIO2A_DS_0;                        /* Address Offset: 0x0160 */
    __IO uint32_t GPIO2A_DS_1;                        /* Address Offset: 0x0164 */
    __IO uint32_t GPIO2A_DS_2;                        /* Address Offset: 0x0168 */
    __IO uint32_t GPIO2A_DS_3;                        /* Address Offset: 0x016C */
         uint32_t RESERVED0170[44];                   /* Address Offset: 0x0170 */
    __IO uint32_t GPIO2A_PULL;                        /* Address Offset: 0x0220 */
         uint32_t RESERVED0224[63];                   /* Address Offset: 0x0224 */
    __IO uint32_t GPIO2A_IE;                          /* Address Offset: 0x0320 */
         uint32_t RESERVED0324[63];                   /* Address Offset: 0x0324 */
    __IO uint32_t GPIO2A_SMT;                         /* Address Offset: 0x0420 */
         uint32_t RESERVED0424[63];                   /* Address Offset: 0x0424 */
    __IO uint32_t GPIO2A_SUS;                         /* Address Offset: 0x0520 */
         uint32_t RESERVED0524[63];                   /* Address Offset: 0x0524 */
    __IO uint32_t GPIO2A_SL;                          /* Address Offset: 0x0620 */
         uint32_t RESERVED0624[63];                   /* Address Offset: 0x0624 */
    __IO uint32_t GPIO2A_OD;                          /* Address Offset: 0x0720 */
         uint32_t RESERVED0724[67];                   /* Address Offset: 0x0724 */
    __IO uint32_t VCCIO2_POC;                         /* Address Offset: 0x0830 */
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
    __I  uint32_t CTR;                                /* Address Offset: 0x00FC */
};
/* INTMUX Register Structure Define */
struct INTMUX_REG {
    __IO uint32_t INT_ENABLE_GROUP[8];                /* Address Offset: 0x0000 */
         uint32_t RESERVED0020[24];                   /* Address Offset: 0x0020 */
    __I  uint32_t INT_FLAG_GROUP[8];                  /* Address Offset: 0x0080 */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
};
#endif /*  __ASSEMBLY__  */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define GPIO0_IOC_BASE                 0xFF540000U /* GPIO0_IOC base address */
#define GPIO4_IOC_BASE                 0xFF550000U /* GPIO4_IOC base address */
#define GPIO1_IOC_BASE                 0xFF560000U /* GPIO1_IOC base address */
#define GPIO3_IOC_BASE                 0xFF560000U /* GPIO3_IOC base address */
#define GPIO2_IOC_BASE                 0xFF570000U /* GPIO2_IOC base address */
#define WDT_BASE                       0xFF5E0000U /* WDT base address */
#define UART0_BASE                     0xFF9F0000U /* UART0 base address */
#define UART1_BASE                     0xFF9F8000U /* UART1 base address */
#define UART2_BASE                     0xFFA00000U /* UART2 base address */
#define UART3_BASE                     0xFFA08000U /* UART3 base address */
#define UART4_BASE                     0xFFA10000U /* UART4 base address */
#define UART5_BASE                     0xFFA18000U /* UART5 base address */
#define UART6_BASE                     0xFFA20000U /* UART6 base address */
#define UART7_BASE                     0xFFA28000U /* UART7 base address */
#define INTMUX_BASE                    0xFFA40000U /* INTMUX base address */
#define TIMER0_BASE                    0xFFAB0000U /* TIMER0 base address */
#define TIMER1_BASE                    0xFFAB0020U /* TIMER1 base address */
#define TIMER2_BASE                    0xFFAB0040U /* TIMER2 base address */
#define TIMER3_BASE                    0xFFAB0060U /* TIMER3 base address */
#define TIMER4_BASE                    0xFFAB0080U /* TIMER4 base address */
#define TIMER5_BASE                    0xFFAB00A0U /* TIMER5 base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define GPIO0_IOC           ((struct GPIO0_IOC_REG *) GPIO0_IOC_BASE)
#define GPIO4_IOC           ((struct GPIO4_IOC_REG *) GPIO4_IOC_BASE)
#define GPIO1_IOC           ((struct GPIO1_IOC_REG *) GPIO1_IOC_BASE)
#define GPIO3_IOC           ((struct GPIO3_IOC_REG *) GPIO3_IOC_BASE)
#define GPIO2_IOC           ((struct GPIO2_IOC_REG *) GPIO2_IOC_BASE)
#define WDT                 ((struct WDT_REG *) WDT_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)
#define UART3               ((struct UART_REG *) UART3_BASE)
#define UART4               ((struct UART_REG *) UART4_BASE)
#define UART5               ((struct UART_REG *) UART5_BASE)
#define UART6               ((struct UART_REG *) UART6_BASE)
#define UART7               ((struct UART_REG *) UART7_BASE)
#define INTMUX              ((struct INTMUX_REG *) INTMUX_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER0_BASE)
#define TIMER1              ((struct TIMER_REG *) TIMER1_BASE)
#define TIMER2              ((struct TIMER_REG *) TIMER2_BASE)
#define TIMER3              ((struct TIMER_REG *) TIMER3_BASE)
#define TIMER4              ((struct TIMER_REG *) TIMER4_BASE)
#define TIMER5              ((struct TIMER_REG *) TIMER5_BASE)

#define IS_GPIO0_IOC_INSTANCE(instance) ((instance) == GPIO0_IOC)
#define IS_GPIO4_IOC_INSTANCE(instance) ((instance) == GPIO4_IOC)
#define IS_GPIO1_IOC_INSTANCE(instance) ((instance) == GPIO1_IOC)
#define IS_GPIO3_IOC_INSTANCE(instance) ((instance) == GPIO3_IOC)
#define IS_GPIO2_IOC_INSTANCE(instance) ((instance) == GPIO2_IOC)
#define IS_WDT_INSTANCE(instance) ((instance) == WDT)
#define IS_INTMUX_INSTANCE(instance) ((instance) == INTMUX)
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1) || ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || ((instance) == UART5) || ((instance) == UART6) || ((instance) == UART7))
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || ((instance) == TIMER2) || ((instance) == TIMER3) || ((instance) == TIMER4) || ((instance) == TIMER5))
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/***************************************GPIO0_IOC****************************************/
/* GPIO0A_IOMUX_SEL_L */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_OFFSET                (0x0U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_MASK      (0xFU << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_MASK      (0xFU << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_MASK      (0xFU << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_MASK      (0xFU << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO0A_IOMUX_SEL_H */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_OFFSET                (0x4U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_MASK      (0xFU << GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_MASK      (0xFU << GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_SHIFT)     /* 0x000000F0 */
/* GPIO0A_DS_0 */
#define GPIO0_IOC_GPIO0A_DS_0_OFFSET                       (0x100U)
#define GPIO0_IOC_GPIO0A_DS_0_GPIO0A0_DS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0A_DS_0_GPIO0A0_DS_MASK              (0x3FU << GPIO0_IOC_GPIO0A_DS_0_GPIO0A0_DS_SHIFT)            /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS_0_GPIO0A1_DS_SHIFT             (8U)
#define GPIO0_IOC_GPIO0A_DS_0_GPIO0A1_DS_MASK              (0x3FU << GPIO0_IOC_GPIO0A_DS_0_GPIO0A1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO0A_DS_1 */
#define GPIO0_IOC_GPIO0A_DS_1_OFFSET                       (0x104U)
#define GPIO0_IOC_GPIO0A_DS_1_GPIO0A2_DS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0A_DS_1_GPIO0A2_DS_MASK              (0x3FU << GPIO0_IOC_GPIO0A_DS_1_GPIO0A2_DS_SHIFT)            /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS_1_GPIO0A3_DS_SHIFT             (8U)
#define GPIO0_IOC_GPIO0A_DS_1_GPIO0A3_DS_MASK              (0x3FU << GPIO0_IOC_GPIO0A_DS_1_GPIO0A3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO0A_DS_2 */
#define GPIO0_IOC_GPIO0A_DS_2_OFFSET                       (0x108U)
#define GPIO0_IOC_GPIO0A_DS_2_GPIO0A4_DS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0A_DS_2_GPIO0A4_DS_MASK              (0x3FU << GPIO0_IOC_GPIO0A_DS_2_GPIO0A4_DS_SHIFT)            /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS_2_GPIO0A5_DS_SHIFT             (8U)
#define GPIO0_IOC_GPIO0A_DS_2_GPIO0A5_DS_MASK              (0x3FU << GPIO0_IOC_GPIO0A_DS_2_GPIO0A5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO0A_PULL */
#define GPIO0_IOC_GPIO0A_PULL_OFFSET                       (0x200U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A0_PULL_SHIFT           (0U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A0_PULL_MASK            (0x3U << GPIO0_IOC_GPIO0A_PULL_GPIO0A0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A1_PULL_SHIFT           (2U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A1_PULL_MASK            (0x3U << GPIO0_IOC_GPIO0A_PULL_GPIO0A1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A2_PULL_SHIFT           (4U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A2_PULL_MASK            (0x3U << GPIO0_IOC_GPIO0A_PULL_GPIO0A2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A3_PULL_SHIFT           (6U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A3_PULL_MASK            (0x3U << GPIO0_IOC_GPIO0A_PULL_GPIO0A3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A4_PULL_SHIFT           (8U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A4_PULL_MASK            (0x3U << GPIO0_IOC_GPIO0A_PULL_GPIO0A4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A5_PULL_SHIFT           (10U)
#define GPIO0_IOC_GPIO0A_PULL_GPIO0A5_PULL_MASK            (0x3U << GPIO0_IOC_GPIO0A_PULL_GPIO0A5_PULL_SHIFT)           /* 0x00000C00 */
/* GPIO0A_IE */
#define GPIO0_IOC_GPIO0A_IE_OFFSET                         (0x300U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A0_IE_SHIFT               (0U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A0_IE_MASK                (0x1U << GPIO0_IOC_GPIO0A_IE_GPIO0A0_IE_SHIFT)               /* 0x00000001 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A1_IE_SHIFT               (1U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A1_IE_MASK                (0x1U << GPIO0_IOC_GPIO0A_IE_GPIO0A1_IE_SHIFT)               /* 0x00000002 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A2_IE_SHIFT               (2U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A2_IE_MASK                (0x1U << GPIO0_IOC_GPIO0A_IE_GPIO0A2_IE_SHIFT)               /* 0x00000004 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A3_IE_SHIFT               (3U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A3_IE_MASK                (0x1U << GPIO0_IOC_GPIO0A_IE_GPIO0A3_IE_SHIFT)               /* 0x00000008 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A4_IE_SHIFT               (4U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A4_IE_MASK                (0x1U << GPIO0_IOC_GPIO0A_IE_GPIO0A4_IE_SHIFT)               /* 0x00000010 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A5_IE_SHIFT               (5U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A5_IE_MASK                (0x1U << GPIO0_IOC_GPIO0A_IE_GPIO0A5_IE_SHIFT)               /* 0x00000020 */
/* GPIO0A_SMT */
#define GPIO0_IOC_GPIO0A_SMT_OFFSET                        (0x400U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A0_SMT_SHIFT             (0U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A0_SMT_MASK              (0x1U << GPIO0_IOC_GPIO0A_SMT_GPIO0A0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A1_SMT_SHIFT             (1U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A1_SMT_MASK              (0x1U << GPIO0_IOC_GPIO0A_SMT_GPIO0A1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A2_SMT_SHIFT             (2U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A2_SMT_MASK              (0x1U << GPIO0_IOC_GPIO0A_SMT_GPIO0A2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A3_SMT_SHIFT             (3U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A3_SMT_MASK              (0x1U << GPIO0_IOC_GPIO0A_SMT_GPIO0A3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A4_SMT_SHIFT             (4U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A4_SMT_MASK              (0x1U << GPIO0_IOC_GPIO0A_SMT_GPIO0A4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A5_SMT_SHIFT             (5U)
#define GPIO0_IOC_GPIO0A_SMT_GPIO0A5_SMT_MASK              (0x1U << GPIO0_IOC_GPIO0A_SMT_GPIO0A5_SMT_SHIFT)             /* 0x00000020 */
/* GPIO0A_SUS */
#define GPIO0_IOC_GPIO0A_SUS_OFFSET                        (0x500U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A0_SUS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A0_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A1_SUS_SHIFT             (1U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A1_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A2_SUS_SHIFT             (2U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A2_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A3_SUS_SHIFT             (3U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A3_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A4_SUS_SHIFT             (4U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A4_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A5_SUS_SHIFT             (5U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A5_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A5_SUS_SHIFT)             /* 0x00000020 */
/* GPIO0A_SL */
#define GPIO0_IOC_GPIO0A_SL_OFFSET                         (0x600U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A0_SL_SHIFT               (0U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A0_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A0_SL_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0A_SL_GPIO0A1_SL_SHIFT               (2U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A1_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A1_SL_SHIFT)               /* 0x0000000C */
#define GPIO0_IOC_GPIO0A_SL_GPIO0A2_SL_SHIFT               (4U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A2_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A2_SL_SHIFT)               /* 0x00000030 */
#define GPIO0_IOC_GPIO0A_SL_GPIO0A3_SL_SHIFT               (6U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A3_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO0_IOC_GPIO0A_SL_GPIO0A4_SL_SHIFT               (8U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A4_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A4_SL_SHIFT)               /* 0x00000300 */
#define GPIO0_IOC_GPIO0A_SL_GPIO0A5_SL_SHIFT               (10U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A5_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A5_SL_SHIFT)               /* 0x00000C00 */
/* GPIO0A_OD */
#define GPIO0_IOC_GPIO0A_OD_OFFSET                         (0x700U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A0_OD_SHIFT               (0U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A0_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A0_OD_SHIFT)               /* 0x00000001 */
#define GPIO0_IOC_GPIO0A_OD_GPIO0A1_OD_SHIFT               (1U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A1_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A1_OD_SHIFT)               /* 0x00000002 */
#define GPIO0_IOC_GPIO0A_OD_GPIO0A2_OD_SHIFT               (2U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A2_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A2_OD_SHIFT)               /* 0x00000004 */
#define GPIO0_IOC_GPIO0A_OD_GPIO0A3_OD_SHIFT               (3U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A3_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A3_OD_SHIFT)               /* 0x00000008 */
#define GPIO0_IOC_GPIO0A_OD_GPIO0A4_OD_SHIFT               (4U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A4_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A4_OD_SHIFT)               /* 0x00000010 */
#define GPIO0_IOC_GPIO0A_OD_GPIO0A5_OD_SHIFT               (5U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A5_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A5_OD_SHIFT)               /* 0x00000020 */
/* IO5V_CTRL */
#define GPIO0_IOC_IO5V_CTRL_OFFSET                         (0x704U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_HPD_IE_SHIFT              (0U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_HPD_IE_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_HPD_IE_SHIFT)              /* 0x00000001 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_HPD_DS_SHIFT              (1U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_HPD_DS_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_HPD_DS_SHIFT)              /* 0x00000002 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_CEC_PU_SHIFT              (4U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_CEC_PU_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_CEC_PU_SHIFT)              /* 0x00000010 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_CEC_IE_SHIFT              (5U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_CEC_IE_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_CEC_IE_SHIFT)              /* 0x00000020 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_CEC_DS_SHIFT              (6U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_CEC_DS_MASK               (0x3U << GPIO0_IOC_IO5V_CTRL_HDMI_CEC_DS_SHIFT)              /* 0x000000C0 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_SCL_IE_SHIFT              (8U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_SCL_IE_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_SCL_IE_SHIFT)              /* 0x00000100 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_SCL_DS_SHIFT              (9U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_SCL_DS_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_SCL_DS_SHIFT)              /* 0x00000200 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_SCL_M33IIC_SHIFT          (10U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_SCL_M33IIC_MASK           (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_SCL_M33IIC_SHIFT)          /* 0x00000400 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_SDA_IE_SHIFT              (12U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_SDA_IE_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_SDA_IE_SHIFT)              /* 0x00001000 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_SDC_DS_SHIFT              (13U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_SDC_DS_MASK               (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_SDC_DS_SHIFT)              /* 0x00002000 */
#define GPIO0_IOC_IO5V_CTRL_HDMI_SDA_M33IIC_SHIFT          (14U)
#define GPIO0_IOC_IO5V_CTRL_HDMI_SDA_M33IIC_MASK           (0x1U << GPIO0_IOC_IO5V_CTRL_HDMI_SDA_M33IIC_SHIFT)          /* 0x00004000 */
/***************************************GPIO4_IOC****************************************/
/* GPIO4A_IOMUX_SEL_L */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_OFFSET                (0x80U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO4A_IOMUX_SEL_H */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_OFFSET                (0x84U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO4B_IOMUX_SEL_L */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_OFFSET                (0x88U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO4B_IOMUX_SEL_H */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_OFFSET                (0x8CU)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO4C_IOMUX_SEL_L */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_OFFSET                (0x90U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C0_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C0_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C1_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C1_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C2_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C2_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C3_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C3_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_L_GPIO4C3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO4C_IOMUX_SEL_H */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_OFFSET                (0x94U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C4_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C4_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C5_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C5_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C6_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C6_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C7_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C7_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4C_IOMUX_SEL_H_GPIO4C7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO4D_IOMUX_SEL_L */
#define GPIO4_IOC_GPIO4D_IOMUX_SEL_L_OFFSET                (0x98U)
#define GPIO4_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D0_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D0_SEL_MASK      (0xFU << GPIO4_IOC_GPIO4D_IOMUX_SEL_L_GPIO4D0_SEL_SHIFT)     /* 0x0000000F */
/* GPIO4A_DS_0 */
#define GPIO4_IOC_GPIO4A_DS_0_OFFSET                       (0x1C0U)
#define GPIO4_IOC_GPIO4A_DS_0_GPIO4A0_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4A_DS_0_GPIO4A0_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_0_GPIO4A0_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS_0_GPIO4A1_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4A_DS_0_GPIO4A1_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_0_GPIO4A1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4A_DS_1 */
#define GPIO4_IOC_GPIO4A_DS_1_OFFSET                       (0x1C4U)
#define GPIO4_IOC_GPIO4A_DS_1_GPIO4A2_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4A_DS_1_GPIO4A2_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_1_GPIO4A2_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS_1_GPIO4A3_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4A_DS_1_GPIO4A3_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_1_GPIO4A3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4A_DS_2 */
#define GPIO4_IOC_GPIO4A_DS_2_OFFSET                       (0x1C8U)
#define GPIO4_IOC_GPIO4A_DS_2_GPIO4A4_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4A_DS_2_GPIO4A4_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_2_GPIO4A4_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS_2_GPIO4A5_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4A_DS_2_GPIO4A5_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_2_GPIO4A5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4A_DS_3 */
#define GPIO4_IOC_GPIO4A_DS_3_OFFSET                       (0x1CCU)
#define GPIO4_IOC_GPIO4A_DS_3_GPIO4A6_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4A_DS_3_GPIO4A6_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_3_GPIO4A6_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS_3_GPIO4A7_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4A_DS_3_GPIO4A7_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4A_DS_3_GPIO4A7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4B_DS_0 */
#define GPIO4_IOC_GPIO4B_DS_0_OFFSET                       (0x1D0U)
#define GPIO4_IOC_GPIO4B_DS_0_GPIO4B0_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4B_DS_0_GPIO4B0_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_0_GPIO4B0_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS_0_GPIO4B1_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4B_DS_0_GPIO4B1_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_0_GPIO4B1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4B_DS_1 */
#define GPIO4_IOC_GPIO4B_DS_1_OFFSET                       (0x1D4U)
#define GPIO4_IOC_GPIO4B_DS_1_GPIO4B2_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4B_DS_1_GPIO4B2_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_1_GPIO4B2_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS_1_GPIO4B3_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4B_DS_1_GPIO4B3_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_1_GPIO4B3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4B_DS_2 */
#define GPIO4_IOC_GPIO4B_DS_2_OFFSET                       (0x1D8U)
#define GPIO4_IOC_GPIO4B_DS_2_GPIO4B4_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4B_DS_2_GPIO4B4_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_2_GPIO4B4_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS_2_GPIO4B5_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4B_DS_2_GPIO4B5_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_2_GPIO4B5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4B_DS_3 */
#define GPIO4_IOC_GPIO4B_DS_3_OFFSET                       (0x1DCU)
#define GPIO4_IOC_GPIO4B_DS_3_GPIO4B6_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4B_DS_3_GPIO4B6_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_3_GPIO4B6_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS_3_GPIO4B7_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4B_DS_3_GPIO4B7_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4B_DS_3_GPIO4B7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4C_DS_0 */
#define GPIO4_IOC_GPIO4C_DS_0_OFFSET                       (0x1E0U)
#define GPIO4_IOC_GPIO4C_DS_0_GPIO4C0_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4C_DS_0_GPIO4C0_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_0_GPIO4C0_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4C_DS_0_GPIO4C1_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4C_DS_0_GPIO4C1_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_0_GPIO4C1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4C_DS_1 */
#define GPIO4_IOC_GPIO4C_DS_1_OFFSET                       (0x1E4U)
#define GPIO4_IOC_GPIO4C_DS_1_GPIO4C2_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4C_DS_1_GPIO4C2_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_1_GPIO4C2_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4C_DS_1_GPIO4C3_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4C_DS_1_GPIO4C3_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_1_GPIO4C3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4C_DS_2 */
#define GPIO4_IOC_GPIO4C_DS_2_OFFSET                       (0x1E8U)
#define GPIO4_IOC_GPIO4C_DS_2_GPIO4C4_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4C_DS_2_GPIO4C4_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_2_GPIO4C4_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4C_DS_2_GPIO4C5_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4C_DS_2_GPIO4C5_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_2_GPIO4C5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4C_DS_3 */
#define GPIO4_IOC_GPIO4C_DS_3_OFFSET                       (0x1ECU)
#define GPIO4_IOC_GPIO4C_DS_3_GPIO4C6_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4C_DS_3_GPIO4C6_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_3_GPIO4C6_DS_SHIFT)            /* 0x0000003F */
#define GPIO4_IOC_GPIO4C_DS_3_GPIO4C7_DS_SHIFT             (8U)
#define GPIO4_IOC_GPIO4C_DS_3_GPIO4C7_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4C_DS_3_GPIO4C7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO4D_DS_0 */
#define GPIO4_IOC_GPIO4D_DS_0_OFFSET                       (0x1F0U)
#define GPIO4_IOC_GPIO4D_DS_0_GPIO4D0_DS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4D_DS_0_GPIO4D0_DS_MASK              (0x3FU << GPIO4_IOC_GPIO4D_DS_0_GPIO4D0_DS_SHIFT)            /* 0x0000003F */
/* GPIO4A_PULL */
#define GPIO4_IOC_GPIO4A_PULL_OFFSET                       (0x240U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A0_PULL_SHIFT           (0U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A0_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A1_PULL_SHIFT           (2U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A1_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A2_PULL_SHIFT           (4U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A2_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A3_PULL_SHIFT           (6U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A3_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A4_PULL_SHIFT           (8U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A4_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A5_PULL_SHIFT           (10U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A5_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A6_PULL_SHIFT           (12U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A6_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A7_PULL_SHIFT           (14U)
#define GPIO4_IOC_GPIO4A_PULL_GPIO4A7_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4A_PULL_GPIO4A7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO4B_PULL */
#define GPIO4_IOC_GPIO4B_PULL_OFFSET                       (0x244U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B0_PULL_SHIFT           (0U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B0_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B1_PULL_SHIFT           (2U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B1_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B2_PULL_SHIFT           (4U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B2_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B3_PULL_SHIFT           (6U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B3_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B4_PULL_SHIFT           (8U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B4_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B5_PULL_SHIFT           (10U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B5_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B6_PULL_SHIFT           (12U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B6_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B7_PULL_SHIFT           (14U)
#define GPIO4_IOC_GPIO4B_PULL_GPIO4B7_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4B_PULL_GPIO4B7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO4C_PULL */
#define GPIO4_IOC_GPIO4C_PULL_OFFSET                       (0x248U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C0_PULL_SHIFT           (0U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C0_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C1_PULL_SHIFT           (2U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C1_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C2_PULL_SHIFT           (4U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C2_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C3_PULL_SHIFT           (6U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C3_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C4_PULL_SHIFT           (8U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C4_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C6_PULL_SHIFT           (12U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C6_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C5_PULL_SHIFT           (10U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C5_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C7_PULL_SHIFT           (14U)
#define GPIO4_IOC_GPIO4C_PULL_GPIO4C7_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4C_PULL_GPIO4C7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO4D_PULL */
#define GPIO4_IOC_GPIO4D_PULL_OFFSET                       (0x24CU)
#define GPIO4_IOC_GPIO4D_PULL_GPIO4D0_PULL_SHIFT           (0U)
#define GPIO4_IOC_GPIO4D_PULL_GPIO4D0_PULL_MASK            (0x3U << GPIO4_IOC_GPIO4D_PULL_GPIO4D0_PULL_SHIFT)           /* 0x00000003 */
/* GPIO4A_IE */
#define GPIO4_IOC_GPIO4A_IE_OFFSET                         (0x340U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A0_IE_SHIFT               (0U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A0_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A0_IE_SHIFT)               /* 0x00000001 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A1_IE_SHIFT               (1U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A1_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A1_IE_SHIFT)               /* 0x00000002 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A2_IE_SHIFT               (2U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A2_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A2_IE_SHIFT)               /* 0x00000004 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A3_IE_SHIFT               (3U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A3_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A3_IE_SHIFT)               /* 0x00000008 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A4_IE_SHIFT               (4U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A4_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A4_IE_SHIFT)               /* 0x00000010 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A5_IE_SHIFT               (5U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A5_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A5_IE_SHIFT)               /* 0x00000020 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A6_IE_SHIFT               (6U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A6_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A6_IE_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A7_IE_SHIFT               (7U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A7_IE_MASK                (0x1U << GPIO4_IOC_GPIO4A_IE_GPIO4A7_IE_SHIFT)               /* 0x00000080 */
/* GPIO4B_IE */
#define GPIO4_IOC_GPIO4B_IE_OFFSET                         (0x344U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B0_IE_SHIFT               (0U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B0_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B0_IE_SHIFT)               /* 0x00000001 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B1_IE_SHIFT               (1U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B1_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B1_IE_SHIFT)               /* 0x00000002 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B2_IE_SHIFT               (2U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B2_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B2_IE_SHIFT)               /* 0x00000004 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B3_IE_SHIFT               (3U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B3_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B3_IE_SHIFT)               /* 0x00000008 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B4_IE_SHIFT               (4U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B4_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B4_IE_SHIFT)               /* 0x00000010 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B5_IE_SHIFT               (5U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B5_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B5_IE_SHIFT)               /* 0x00000020 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B6_IE_SHIFT               (6U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B6_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B6_IE_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B7_IE_SHIFT               (7U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B7_IE_MASK                (0x1U << GPIO4_IOC_GPIO4B_IE_GPIO4B7_IE_SHIFT)               /* 0x00000080 */
/* GPIO4C_IE */
#define GPIO4_IOC_GPIO4C_IE_OFFSET                         (0x348U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C0_IE_SHIFT               (0U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C0_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C0_IE_SHIFT)               /* 0x00000001 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C1_IE_SHIFT               (1U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C1_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C1_IE_SHIFT)               /* 0x00000002 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C2_IE_SHIFT               (2U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C2_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C2_IE_SHIFT)               /* 0x00000004 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C3_IE_SHIFT               (3U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C3_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C3_IE_SHIFT)               /* 0x00000008 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C4_IE_SHIFT               (4U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C4_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C4_IE_SHIFT)               /* 0x00000010 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C5_IE_SHIFT               (5U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C5_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C5_IE_SHIFT)               /* 0x00000020 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C6_IE_SHIFT               (6U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C6_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C6_IE_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_GPIO4C_IE_GPIO4C7_IE_SHIFT               (7U)
#define GPIO4_IOC_GPIO4C_IE_GPIO4C7_IE_MASK                (0x1U << GPIO4_IOC_GPIO4C_IE_GPIO4C7_IE_SHIFT)               /* 0x00000080 */
/* GPIO4D_IE */
#define GPIO4_IOC_GPIO4D_IE_OFFSET                         (0x34CU)
#define GPIO4_IOC_GPIO4D_IE_GPIO4D0_IE_SHIFT               (0U)
#define GPIO4_IOC_GPIO4D_IE_GPIO4D0_IE_MASK                (0x1U << GPIO4_IOC_GPIO4D_IE_GPIO4D0_IE_SHIFT)               /* 0x00000001 */
/* GPIO4A_SMT */
#define GPIO4_IOC_GPIO4A_SMT_OFFSET                        (0x440U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A0_SMT_SHIFT             (0U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A0_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A1_SMT_SHIFT             (1U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A1_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A2_SMT_SHIFT             (2U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A2_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A3_SMT_SHIFT             (3U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A3_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A4_SMT_SHIFT             (4U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A4_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A5_SMT_SHIFT             (5U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A5_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A6_SMT_SHIFT             (6U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A6_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A7_SMT_SHIFT             (7U)
#define GPIO4_IOC_GPIO4A_SMT_GPIO4A7_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4A_SMT_GPIO4A7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO4B_SMT */
#define GPIO4_IOC_GPIO4B_SMT_OFFSET                        (0x444U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B0_SMT_SHIFT             (0U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B0_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B1_SMT_SHIFT             (1U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B1_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B2_SMT_SHIFT             (2U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B2_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B3_SMT_SHIFT             (3U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B3_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B4_SMT_SHIFT             (4U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B4_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B5_SMT_SHIFT             (5U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B5_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B6_SMT_SHIFT             (6U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B6_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B7_SMT_SHIFT             (7U)
#define GPIO4_IOC_GPIO4B_SMT_GPIO4B7_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4B_SMT_GPIO4B7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO4C_SMT */
#define GPIO4_IOC_GPIO4C_SMT_OFFSET                        (0x448U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C0_SMT_SHIFT             (0U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C0_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C1_SMT_SHIFT             (1U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C1_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C2_SMT_SHIFT             (2U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C2_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C3_SMT_SHIFT             (3U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C3_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C4_SMT_SHIFT             (4U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C4_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C5_SMT_SHIFT             (5U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C5_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C6_SMT_SHIFT             (6U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C6_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C7_SMT_SHIFT             (7U)
#define GPIO4_IOC_GPIO4C_SMT_GPIO4C7_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4C_SMT_GPIO4C7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO4D_SMT */
#define GPIO4_IOC_GPIO4D_SMT_OFFSET                        (0x44CU)
#define GPIO4_IOC_GPIO4D_SMT_GPIO4D0_SMT_SHIFT             (0U)
#define GPIO4_IOC_GPIO4D_SMT_GPIO4D0_SMT_MASK              (0x1U << GPIO4_IOC_GPIO4D_SMT_GPIO4D0_SMT_SHIFT)             /* 0x00000001 */
/* GPIO4A_SUS */
#define GPIO4_IOC_GPIO4A_SUS_OFFSET                        (0x540U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A0_SUS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A0_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A1_SUS_SHIFT             (1U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A1_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A2_SUS_SHIFT             (2U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A2_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A3_SUS_SHIFT             (3U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A3_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A4_SUS_SHIFT             (4U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A4_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A5_SUS_SHIFT             (5U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A5_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A6_SUS_SHIFT             (6U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A6_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A7_SUS_SHIFT             (7U)
#define GPIO4_IOC_GPIO4A_SUS_GPIO4A7_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4A_SUS_GPIO4A7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO4B_SUS */
#define GPIO4_IOC_GPIO4B_SUS_OFFSET                        (0x544U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B0_SUS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B0_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B1_SUS_SHIFT             (1U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B1_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B2_SUS_SHIFT             (2U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B2_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B3_SUS_SHIFT             (3U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B3_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B4_SUS_SHIFT             (4U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B4_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B5_SUS_SHIFT             (5U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B5_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B6_SUS_SHIFT             (6U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B6_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B7_SUS_SHIFT             (7U)
#define GPIO4_IOC_GPIO4B_SUS_GPIO4B7_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4B_SUS_GPIO4B7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO4C_SUS */
#define GPIO4_IOC_GPIO4C_SUS_OFFSET                        (0x548U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C0_SUS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C0_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C1_SUS_SHIFT             (1U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C1_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C2_SUS_SHIFT             (2U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C2_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C3_SUS_SHIFT             (3U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C3_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C4_SUS_SHIFT             (4U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C4_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C5_SUS_SHIFT             (5U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C5_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C6_SUS_SHIFT             (6U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C6_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C7_SUS_SHIFT             (7U)
#define GPIO4_IOC_GPIO4C_SUS_GPIO4C7_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4C_SUS_GPIO4C7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO4D_SUS */
#define GPIO4_IOC_GPIO4D_SUS_OFFSET                        (0x54CU)
#define GPIO4_IOC_GPIO4D_SUS_GPIO4D0_SUS_SHIFT             (0U)
#define GPIO4_IOC_GPIO4D_SUS_GPIO4D0_SUS_MASK              (0x1U << GPIO4_IOC_GPIO4D_SUS_GPIO4D0_SUS_SHIFT)             /* 0x00000001 */
/* GPIO4A_SL */
#define GPIO4_IOC_GPIO4A_SL_OFFSET                         (0x640U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A0_SL_SHIFT               (0U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A0_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A0_SL_SHIFT)               /* 0x00000003 */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A1_SL_SHIFT               (2U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A1_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A1_SL_SHIFT)               /* 0x0000000C */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A2_SL_SHIFT               (4U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A2_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A2_SL_SHIFT)               /* 0x00000030 */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A3_SL_SHIFT               (6U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A3_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A4_SL_SHIFT               (8U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A4_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A4_SL_SHIFT)               /* 0x00000300 */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A5_SL_SHIFT               (10U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A5_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A6_SL_SHIFT               (12U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A6_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A6_SL_SHIFT)               /* 0x00003000 */
#define GPIO4_IOC_GPIO4A_SL_GPIO4A7_SL_SHIFT               (14U)
#define GPIO4_IOC_GPIO4A_SL_GPIO4A7_SL_MASK                (0x3U << GPIO4_IOC_GPIO4A_SL_GPIO4A7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO4B_SL */
#define GPIO4_IOC_GPIO4B_SL_OFFSET                         (0x644U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B0_SL_SHIFT               (0U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B0_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B0_SL_SHIFT)               /* 0x00000003 */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B1_SL_SHIFT               (2U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B1_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B1_SL_SHIFT)               /* 0x0000000C */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B2_SL_SHIFT               (4U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B2_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B2_SL_SHIFT)               /* 0x00000030 */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B3_SL_SHIFT               (6U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B3_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B4_SL_SHIFT               (8U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B4_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B4_SL_SHIFT)               /* 0x00000300 */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B5_SL_SHIFT               (10U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B5_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B6_SL_SHIFT               (12U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B6_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B6_SL_SHIFT)               /* 0x00003000 */
#define GPIO4_IOC_GPIO4B_SL_GPIO4B7_SL_SHIFT               (14U)
#define GPIO4_IOC_GPIO4B_SL_GPIO4B7_SL_MASK                (0x3U << GPIO4_IOC_GPIO4B_SL_GPIO4B7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO4C_SL */
#define GPIO4_IOC_GPIO4C_SL_OFFSET                         (0x648U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C0_SL_SHIFT               (0U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C0_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C0_SL_SHIFT)               /* 0x00000003 */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C1_SL_SHIFT               (2U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C1_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C1_SL_SHIFT)               /* 0x0000000C */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C2_SL_SHIFT               (4U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C2_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C2_SL_SHIFT)               /* 0x00000030 */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C3_SL_SHIFT               (6U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C3_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C4_SL_SHIFT               (8U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C4_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C4_SL_SHIFT)               /* 0x00000300 */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C5_SL_SHIFT               (10U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C5_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C6_SL_SHIFT               (12U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C6_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C6_SL_SHIFT)               /* 0x00003000 */
#define GPIO4_IOC_GPIO4C_SL_GPIO4C7_SL_SHIFT               (14U)
#define GPIO4_IOC_GPIO4C_SL_GPIO4C7_SL_MASK                (0x3U << GPIO4_IOC_GPIO4C_SL_GPIO4C7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO4D_SL */
#define GPIO4_IOC_GPIO4D_SL_OFFSET                         (0x64CU)
#define GPIO4_IOC_GPIO4D_SL_GPIO4D0_SL_SHIFT               (0U)
#define GPIO4_IOC_GPIO4D_SL_GPIO4D0_SL_MASK                (0x3U << GPIO4_IOC_GPIO4D_SL_GPIO4D0_SL_SHIFT)               /* 0x00000003 */
/* GPIO4A_OD */
#define GPIO4_IOC_GPIO4A_OD_OFFSET                         (0x740U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A0_OD_SHIFT               (0U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A0_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A0_OD_SHIFT)               /* 0x00000001 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A1_OD_SHIFT               (1U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A1_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A1_OD_SHIFT)               /* 0x00000002 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A2_OD_SHIFT               (2U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A2_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A2_OD_SHIFT)               /* 0x00000004 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A3_OD_SHIFT               (3U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A3_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A3_OD_SHIFT)               /* 0x00000008 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A4_OD_SHIFT               (4U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A4_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A4_OD_SHIFT)               /* 0x00000010 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A5_OD_SHIFT               (5U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A5_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A5_OD_SHIFT)               /* 0x00000020 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A6_OD_SHIFT               (6U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A6_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A6_OD_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_GPIO4A_OD_GPIO4A7_OD_SHIFT               (7U)
#define GPIO4_IOC_GPIO4A_OD_GPIO4A7_OD_MASK                (0x1U << GPIO4_IOC_GPIO4A_OD_GPIO4A7_OD_SHIFT)               /* 0x00000080 */
/* GPIO4B_OD */
#define GPIO4_IOC_GPIO4B_OD_OFFSET                         (0x744U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B0_OD_SHIFT               (0U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B0_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B0_OD_SHIFT)               /* 0x00000001 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B1_OD_SHIFT               (1U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B1_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B1_OD_SHIFT)               /* 0x00000002 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B2_OD_SHIFT               (2U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B2_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B2_OD_SHIFT)               /* 0x00000004 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B3_OD_SHIFT               (3U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B3_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B3_OD_SHIFT)               /* 0x00000008 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B4_OD_SHIFT               (4U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B4_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B4_OD_SHIFT)               /* 0x00000010 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B5_OD_SHIFT               (5U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B5_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B5_OD_SHIFT)               /* 0x00000020 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B6_OD_SHIFT               (6U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B6_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B6_OD_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_GPIO4B_OD_GPIO4B7_OD_SHIFT               (7U)
#define GPIO4_IOC_GPIO4B_OD_GPIO4B7_OD_MASK                (0x1U << GPIO4_IOC_GPIO4B_OD_GPIO4B7_OD_SHIFT)               /* 0x00000080 */
/* GPIO4C_OD */
#define GPIO4_IOC_GPIO4C_OD_OFFSET                         (0x748U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C0_OD_SHIFT               (0U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C0_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C0_OD_SHIFT)               /* 0x00000001 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C1_OD_SHIFT               (1U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C1_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C1_OD_SHIFT)               /* 0x00000002 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C2_OD_SHIFT               (2U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C2_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C2_OD_SHIFT)               /* 0x00000004 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C3_OD_SHIFT               (3U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C3_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C3_OD_SHIFT)               /* 0x00000008 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C4_OD_SHIFT               (4U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C4_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C4_OD_SHIFT)               /* 0x00000010 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C5_OD_SHIFT               (5U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C5_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C5_OD_SHIFT)               /* 0x00000020 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C6_OD_SHIFT               (6U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C6_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C6_OD_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_GPIO4C_OD_GPIO4C7_OD_SHIFT               (7U)
#define GPIO4_IOC_GPIO4C_OD_GPIO4C7_OD_MASK                (0x1U << GPIO4_IOC_GPIO4C_OD_GPIO4C7_OD_SHIFT)               /* 0x00000080 */
/* GPIO4D_OD */
#define GPIO4_IOC_GPIO4D_OD_OFFSET                         (0x74CU)
#define GPIO4_IOC_GPIO4D_OD_GPIO4D0_OD_SHIFT               (0U)
#define GPIO4_IOC_GPIO4D_OD_GPIO4D0_OD_MASK                (0x1U << GPIO4_IOC_GPIO4D_OD_GPIO4D0_OD_SHIFT)               /* 0x00000001 */
/* VCCIO4_POC */
#define GPIO4_IOC_VCCIO4_POC_OFFSET                        (0x850U)
#define GPIO4_IOC_VCCIO4_POC_RESERVED_SHIFT                (0U)
#define GPIO4_IOC_VCCIO4_POC_RESERVED_MASK                 (0x1FU << GPIO4_IOC_VCCIO4_POC_RESERVED_SHIFT)               /* 0x0000001F */
#define GPIO4_IOC_VCCIO4_POC_FORCE_JTAG_SHIFT              (5U)
#define GPIO4_IOC_VCCIO4_POC_FORCE_JTAG_MASK               (0x1U << GPIO4_IOC_VCCIO4_POC_FORCE_JTAG_SHIFT)              /* 0x00000020 */
#define GPIO4_IOC_VCCIO4_POC_VCCIO4_VD_SHIFT               (8U)
#define GPIO4_IOC_VCCIO4_POC_VCCIO4_VD_MASK                (0x1U << GPIO4_IOC_VCCIO4_POC_VCCIO4_VD_SHIFT)               /* 0x00000100 */
/***************************************GPIO1_IOC****************************************/
/* GPIO1A_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_OFFSET                (0x20U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1A_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_OFFSET                (0x24U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1B_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_OFFSET                (0x28U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1B_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_OFFSET                (0x2CU)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1C_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_OFFSET                (0x30U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1C_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_OFFSET                (0x34U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1D_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_OFFSET                (0x38U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1D_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_OFFSET                (0x3CU)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_MASK      (0xFU << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO1A_DS_0 */
#define GPIO1_IOC_GPIO1A_DS_0_OFFSET                       (0x120U)
#define GPIO1_IOC_GPIO1A_DS_0_GPIO1A0_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1A_DS_0_GPIO1A0_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_0_GPIO1A0_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS_0_GPIO1A1_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1A_DS_0_GPIO1A1_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_0_GPIO1A1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1A_DS_1 */
#define GPIO1_IOC_GPIO1A_DS_1_OFFSET                       (0x124U)
#define GPIO1_IOC_GPIO1A_DS_1_GPIO1A2_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1A_DS_1_GPIO1A2_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_1_GPIO1A2_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS_1_GPIO1A3_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1A_DS_1_GPIO1A3_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_1_GPIO1A3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1A_DS_2 */
#define GPIO1_IOC_GPIO1A_DS_2_OFFSET                       (0x128U)
#define GPIO1_IOC_GPIO1A_DS_2_GPIO1A4_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1A_DS_2_GPIO1A4_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_2_GPIO1A4_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS_2_GPIO1A5_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1A_DS_2_GPIO1A5_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_2_GPIO1A5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1A_DS_3 */
#define GPIO1_IOC_GPIO1A_DS_3_OFFSET                       (0x12CU)
#define GPIO1_IOC_GPIO1A_DS_3_GPIO1A6_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1A_DS_3_GPIO1A6_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_3_GPIO1A6_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS_3_GPIO1A7_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1A_DS_3_GPIO1A7_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1A_DS_3_GPIO1A7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1B_DS_0 */
#define GPIO1_IOC_GPIO1B_DS_0_OFFSET                       (0x130U)
#define GPIO1_IOC_GPIO1B_DS_0_GPIO1B0_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1B_DS_0_GPIO1B0_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_0_GPIO1B0_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS_0_GPIO1B1_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1B_DS_0_GPIO1B1_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_0_GPIO1B1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1B_DS_1 */
#define GPIO1_IOC_GPIO1B_DS_1_OFFSET                       (0x134U)
#define GPIO1_IOC_GPIO1B_DS_1_GPIO1B2_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1B_DS_1_GPIO1B2_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_1_GPIO1B2_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS_1_GPIO1B3_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1B_DS_1_GPIO1B3_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_1_GPIO1B3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1B_DS_2 */
#define GPIO1_IOC_GPIO1B_DS_2_OFFSET                       (0x138U)
#define GPIO1_IOC_GPIO1B_DS_2_GPIO1B4_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1B_DS_2_GPIO1B4_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_2_GPIO1B4_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS_2_GPIO1B5_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1B_DS_2_GPIO1B5_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_2_GPIO1B5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1B_DS_3 */
#define GPIO1_IOC_GPIO1B_DS_3_OFFSET                       (0x13CU)
#define GPIO1_IOC_GPIO1B_DS_3_GPIO1B6_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1B_DS_3_GPIO1B6_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_3_GPIO1B6_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS_3_GPIO1B7_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1B_DS_3_GPIO1B7_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1B_DS_3_GPIO1B7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1C_DS_0 */
#define GPIO1_IOC_GPIO1C_DS_0_OFFSET                       (0x140U)
#define GPIO1_IOC_GPIO1C_DS_0_GPIO1C0_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1C_DS_0_GPIO1C0_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_0_GPIO1C0_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS_0_GPIO1C1_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1C_DS_0_GPIO1C1_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_0_GPIO1C1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1C_DS_1 */
#define GPIO1_IOC_GPIO1C_DS_1_OFFSET                       (0x144U)
#define GPIO1_IOC_GPIO1C_DS_1_GPIO1C2_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1C_DS_1_GPIO1C2_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_1_GPIO1C2_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS_1_GPIO1C3_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1C_DS_1_GPIO1C3_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_1_GPIO1C3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1C_DS_2 */
#define GPIO1_IOC_GPIO1C_DS_2_OFFSET                       (0x148U)
#define GPIO1_IOC_GPIO1C_DS_2_GPIO1C4_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1C_DS_2_GPIO1C4_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_2_GPIO1C4_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS_2_GPIO1C5_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1C_DS_2_GPIO1C5_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_2_GPIO1C5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1C_DS_3 */
#define GPIO1_IOC_GPIO1C_DS_3_OFFSET                       (0x14CU)
#define GPIO1_IOC_GPIO1C_DS_3_GPIO1C6_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1C_DS_3_GPIO1C6_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_3_GPIO1C6_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS_3_GPIO1C7_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1C_DS_3_GPIO1C7_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1C_DS_3_GPIO1C7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1D_DS_0 */
#define GPIO1_IOC_GPIO1D_DS_0_OFFSET                       (0x150U)
#define GPIO1_IOC_GPIO1D_DS_0_GPIO1D0_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1D_DS_0_GPIO1D0_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_0_GPIO1D0_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS_0_GPIO1D1_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1D_DS_0_GPIO1D1_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_0_GPIO1D1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1D_DS_1 */
#define GPIO1_IOC_GPIO1D_DS_1_OFFSET                       (0x154U)
#define GPIO1_IOC_GPIO1D_DS_1_GPIO1D2_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1D_DS_1_GPIO1D2_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_1_GPIO1D2_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS_1_GPIO1D3_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1D_DS_1_GPIO1D3_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_1_GPIO1D3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1D_DS_2 */
#define GPIO1_IOC_GPIO1D_DS_2_OFFSET                       (0x158U)
#define GPIO1_IOC_GPIO1D_DS_2_GPIO1D4_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1D_DS_2_GPIO1D4_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_2_GPIO1D4_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS_2_GPIO1D5_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1D_DS_2_GPIO1D5_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_2_GPIO1D5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1D_DS_3 */
#define GPIO1_IOC_GPIO1D_DS_3_OFFSET                       (0x15CU)
#define GPIO1_IOC_GPIO1D_DS_3_GPIO1D6_DS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1D_DS_3_GPIO1D6_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_3_GPIO1D6_DS_SHIFT)            /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS_3_GPIO1D7_DS_SHIFT             (8U)
#define GPIO1_IOC_GPIO1D_DS_3_GPIO1D7_DS_MASK              (0x3FU << GPIO1_IOC_GPIO1D_DS_3_GPIO1D7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO1A_PULL */
#define GPIO1_IOC_GPIO1A_PULL_OFFSET                       (0x210U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A0_PULL_SHIFT           (0U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A0_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A1_PULL_SHIFT           (2U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A1_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A2_PULL_SHIFT           (4U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A2_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A3_PULL_SHIFT           (6U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A3_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A4_PULL_SHIFT           (8U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A4_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A5_PULL_SHIFT           (10U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A5_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A6_PULL_SHIFT           (12U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A6_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A7_PULL_SHIFT           (14U)
#define GPIO1_IOC_GPIO1A_PULL_GPIO1A7_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1A_PULL_GPIO1A7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO1B_PULL */
#define GPIO1_IOC_GPIO1B_PULL_OFFSET                       (0x214U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B0_PULL_SHIFT           (0U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B0_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B1_PULL_SHIFT           (2U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B1_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B2_PULL_SHIFT           (4U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B2_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B3_PULL_SHIFT           (6U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B3_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B4_PULL_SHIFT           (8U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B4_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B5_PULL_SHIFT           (10U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B5_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B6_PULL_SHIFT           (12U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B6_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B7_PULL_SHIFT           (14U)
#define GPIO1_IOC_GPIO1B_PULL_GPIO1B7_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1B_PULL_GPIO1B7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO1C_PULL */
#define GPIO1_IOC_GPIO1C_PULL_OFFSET                       (0x218U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C0_PULL_SHIFT           (0U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C0_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C1_PULL_SHIFT           (2U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C1_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C2_PULL_SHIFT           (4U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C2_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C3_PULL_SHIFT           (6U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C3_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C4_PULL_SHIFT           (8U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C4_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C5_PULL_SHIFT           (10U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C5_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C6_PULL_SHIFT           (12U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C6_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C7_PULL_SHIFT           (14U)
#define GPIO1_IOC_GPIO1C_PULL_GPIO1C7_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1C_PULL_GPIO1C7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO1D_PULL */
#define GPIO1_IOC_GPIO1D_PULL_OFFSET                       (0x21CU)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D1_PULL_SHIFT           (2U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D1_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D0_PULL_SHIFT           (0U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D0_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D2_PULL_SHIFT           (4U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D2_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D3_PULL_SHIFT           (6U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D3_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D4_PULL_SHIFT           (8U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D4_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D6_PULL_SHIFT           (12U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D6_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D5_PULL_SHIFT           (10U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D5_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D7_PULL_SHIFT           (14U)
#define GPIO1_IOC_GPIO1D_PULL_GPIO1D7_PULL_MASK            (0x3U << GPIO1_IOC_GPIO1D_PULL_GPIO1D7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO1A_IE */
#define GPIO1_IOC_GPIO1A_IE_OFFSET                         (0x310U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A0_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A0_IE_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A1_IE_SHIFT               (1U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A1_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A1_IE_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A2_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A2_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A2_IE_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A3_IE_SHIFT               (3U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A3_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A3_IE_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A4_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A4_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A4_IE_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A5_IE_SHIFT               (5U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A5_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A5_IE_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A6_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A6_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A6_IE_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A7_IE_SHIFT               (7U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A7_IE_MASK                (0x1U << GPIO1_IOC_GPIO1A_IE_GPIO1A7_IE_SHIFT)               /* 0x00000080 */
/* GPIO1B_IE */
#define GPIO1_IOC_GPIO1B_IE_OFFSET                         (0x314U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B0_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B0_IE_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B1_IE_SHIFT               (1U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B1_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B1_IE_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B2_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B2_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B2_IE_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B3_IE_SHIFT               (3U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B3_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B3_IE_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B4_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B4_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B4_IE_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B5_IE_SHIFT               (5U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B5_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B5_IE_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B6_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B6_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B6_IE_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B7_IE_SHIFT               (7U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B7_IE_MASK                (0x1U << GPIO1_IOC_GPIO1B_IE_GPIO1B7_IE_SHIFT)               /* 0x00000080 */
/* GPIO1C_IE */
#define GPIO1_IOC_GPIO1C_IE_OFFSET                         (0x318U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C0_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C0_IE_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C1_IE_SHIFT               (1U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C1_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C1_IE_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C2_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C2_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C2_IE_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C3_IE_SHIFT               (3U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C3_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C3_IE_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C4_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C4_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C4_IE_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C5_IE_SHIFT               (5U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C5_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C5_IE_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C6_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C6_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C6_IE_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C7_IE_SHIFT               (7U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C7_IE_MASK                (0x1U << GPIO1_IOC_GPIO1C_IE_GPIO1C7_IE_SHIFT)               /* 0x00000080 */
/* GPIO1D_IE */
#define GPIO1_IOC_GPIO1D_IE_OFFSET                         (0x31CU)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D0_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D0_IE_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D1_IE_SHIFT               (1U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D1_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D1_IE_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D2_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D2_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D2_IE_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D3_IE_SHIFT               (3U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D3_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D3_IE_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D4_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D4_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D4_IE_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D5_IE_SHIFT               (5U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D5_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D5_IE_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D6_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D6_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D6_IE_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D7_IE_SHIFT               (7U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D7_IE_MASK                (0x1U << GPIO1_IOC_GPIO1D_IE_GPIO1D7_IE_SHIFT)               /* 0x00000080 */
/* GPIO1A_SMT */
#define GPIO1_IOC_GPIO1A_SMT_OFFSET                        (0x410U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A0_SMT_SHIFT             (0U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A0_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A1_SMT_SHIFT             (1U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A1_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A2_SMT_SHIFT             (2U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A2_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A3_SMT_SHIFT             (3U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A3_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A4_SMT_SHIFT             (4U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A4_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A5_SMT_SHIFT             (5U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A5_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A6_SMT_SHIFT             (6U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A6_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A7_SMT_SHIFT             (7U)
#define GPIO1_IOC_GPIO1A_SMT_GPIO1A7_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1A_SMT_GPIO1A7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO1B_SMT */
#define GPIO1_IOC_GPIO1B_SMT_OFFSET                        (0x414U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B0_SMT_SHIFT             (0U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B0_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B1_SMT_SHIFT             (1U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B1_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B2_SMT_SHIFT             (2U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B2_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B3_SMT_SHIFT             (3U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B3_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B4_SMT_SHIFT             (4U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B4_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B5_SMT_SHIFT             (5U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B5_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B6_SMT_SHIFT             (6U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B6_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B7_SMT_SHIFT             (7U)
#define GPIO1_IOC_GPIO1B_SMT_GPIO1B7_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1B_SMT_GPIO1B7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO1C_SMT */
#define GPIO1_IOC_GPIO1C_SMT_OFFSET                        (0x418U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C0_SMT_SHIFT             (0U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C0_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C1_SMT_SHIFT             (1U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C1_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C2_SMT_SHIFT             (2U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C2_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C3_SMT_SHIFT             (3U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C3_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C4_SMT_SHIFT             (4U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C4_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C5_SMT_SHIFT             (5U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C5_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C6_SMT_SHIFT             (6U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C6_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C7_SMT_SHIFT             (7U)
#define GPIO1_IOC_GPIO1C_SMT_GPIO1C7_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1C_SMT_GPIO1C7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO1D_SMT */
#define GPIO1_IOC_GPIO1D_SMT_OFFSET                        (0x41CU)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D0_SMT_SHIFT             (0U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D0_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D1_SMT_SHIFT             (1U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D1_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D2_SMT_SHIFT             (2U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D2_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D3_SMT_SHIFT             (3U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D3_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D4_SMT_SHIFT             (4U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D4_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D5_SMT_SHIFT             (5U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D5_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D6_SMT_SHIFT             (6U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D6_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D7_SMT_SHIFT             (7U)
#define GPIO1_IOC_GPIO1D_SMT_GPIO1D7_SMT_MASK              (0x1U << GPIO1_IOC_GPIO1D_SMT_GPIO1D7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO1A_SUS */
#define GPIO1_IOC_GPIO1A_SUS_OFFSET                        (0x510U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A0_SUS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A0_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A1_SUS_SHIFT             (1U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A1_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A2_SUS_SHIFT             (2U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A2_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A3_SUS_SHIFT             (3U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A3_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A4_SUS_SHIFT             (4U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A4_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A5_SUS_SHIFT             (5U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A5_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A6_SUS_SHIFT             (6U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A6_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A7_SUS_SHIFT             (7U)
#define GPIO1_IOC_GPIO1A_SUS_GPIO1A7_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1A_SUS_GPIO1A7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO1B_SUS */
#define GPIO1_IOC_GPIO1B_SUS_OFFSET                        (0x514U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B0_SUS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B0_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B1_SUS_SHIFT             (1U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B1_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B2_SUS_SHIFT             (2U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B2_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B3_SUS_SHIFT             (3U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B3_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B4_SUS_SHIFT             (4U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B4_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B5_SUS_SHIFT             (5U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B5_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B6_SUS_SHIFT             (6U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B6_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B7_SUS_SHIFT             (7U)
#define GPIO1_IOC_GPIO1B_SUS_GPIO1B7_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1B_SUS_GPIO1B7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO1C_SUS */
#define GPIO1_IOC_GPIO1C_SUS_OFFSET                        (0x518U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C0_SUS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C0_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C1_SUS_SHIFT             (1U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C1_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C2_SUS_SHIFT             (2U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C2_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C3_SUS_SHIFT             (3U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C3_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C4_SUS_SHIFT             (4U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C4_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C5_SUS_SHIFT             (5U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C5_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C6_SUS_SHIFT             (6U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C6_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C7_SUS_SHIFT             (7U)
#define GPIO1_IOC_GPIO1C_SUS_GPIO1C7_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1C_SUS_GPIO1C7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO1D_SUS */
#define GPIO1_IOC_GPIO1D_SUS_OFFSET                        (0x51CU)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D0_SUS_SHIFT             (0U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D0_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D1_SUS_SHIFT             (1U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D1_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D2_SUS_SHIFT             (2U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D2_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D3_SUS_SHIFT             (3U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D3_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D4_SUS_SHIFT             (4U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D4_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D5_SUS_SHIFT             (5U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D5_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D6_SUS_SHIFT             (6U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D6_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D7_SUS_SHIFT             (7U)
#define GPIO1_IOC_GPIO1D_SUS_GPIO1D7_SUS_MASK              (0x1U << GPIO1_IOC_GPIO1D_SUS_GPIO1D7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO1A_SL */
#define GPIO1_IOC_GPIO1A_SL_OFFSET                         (0x610U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A0_SL_SHIFT               (0U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A0_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A0_SL_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A1_SL_SHIFT               (2U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A1_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A1_SL_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A2_SL_SHIFT               (4U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A2_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A2_SL_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A3_SL_SHIFT               (6U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A3_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A4_SL_SHIFT               (8U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A4_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A4_SL_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A5_SL_SHIFT               (10U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A5_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A6_SL_SHIFT               (12U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A6_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A6_SL_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1A_SL_GPIO1A7_SL_SHIFT               (14U)
#define GPIO1_IOC_GPIO1A_SL_GPIO1A7_SL_MASK                (0x3U << GPIO1_IOC_GPIO1A_SL_GPIO1A7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO1B_SL */
#define GPIO1_IOC_GPIO1B_SL_OFFSET                         (0x614U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B0_SL_SHIFT               (0U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B0_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B0_SL_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B1_SL_SHIFT               (2U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B1_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B1_SL_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B2_SL_SHIFT               (4U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B2_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B2_SL_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B3_SL_SHIFT               (6U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B3_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B4_SL_SHIFT               (8U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B4_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B4_SL_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B5_SL_SHIFT               (10U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B5_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B6_SL_SHIFT               (12U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B6_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B6_SL_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1B_SL_GPIO1B7_SL_SHIFT               (14U)
#define GPIO1_IOC_GPIO1B_SL_GPIO1B7_SL_MASK                (0x3U << GPIO1_IOC_GPIO1B_SL_GPIO1B7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO1C_SL */
#define GPIO1_IOC_GPIO1C_SL_OFFSET                         (0x618U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C0_SL_SHIFT               (0U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C0_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C0_SL_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C1_SL_SHIFT               (2U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C1_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C1_SL_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C2_SL_SHIFT               (4U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C2_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C2_SL_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C3_SL_SHIFT               (6U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C3_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C4_SL_SHIFT               (8U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C4_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C4_SL_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C5_SL_SHIFT               (10U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C5_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C6_SL_SHIFT               (12U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C6_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C6_SL_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1C_SL_GPIO1C7_SL_SHIFT               (14U)
#define GPIO1_IOC_GPIO1C_SL_GPIO1C7_SL_MASK                (0x3U << GPIO1_IOC_GPIO1C_SL_GPIO1C7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO1D_SL */
#define GPIO1_IOC_GPIO1D_SL_OFFSET                         (0x61CU)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D0_SL_SHIFT               (0U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D0_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D0_SL_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D1_SL_SHIFT               (2U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D1_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D1_SL_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D2_SL_SHIFT               (4U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D2_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D2_SL_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D3_SL_SHIFT               (6U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D3_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D4_SL_SHIFT               (8U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D4_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D4_SL_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D5_SL_SHIFT               (10U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D5_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D6_SL_SHIFT               (12U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D6_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D6_SL_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1D_SL_GPIO1D7_SL_SHIFT               (14U)
#define GPIO1_IOC_GPIO1D_SL_GPIO1D7_SL_MASK                (0x3U << GPIO1_IOC_GPIO1D_SL_GPIO1D7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO1A_OD */
#define GPIO1_IOC_GPIO1A_OD_OFFSET                         (0x710U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A0_OD_SHIFT               (0U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A0_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A0_OD_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A1_OD_SHIFT               (1U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A1_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A1_OD_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A2_OD_SHIFT               (2U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A2_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A2_OD_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A3_OD_SHIFT               (3U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A3_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A3_OD_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A4_OD_SHIFT               (4U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A4_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A4_OD_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A5_OD_SHIFT               (5U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A5_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A5_OD_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A6_OD_SHIFT               (6U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A6_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A6_OD_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1A_OD_GPIO1A7_OD_SHIFT               (7U)
#define GPIO1_IOC_GPIO1A_OD_GPIO1A7_OD_MASK                (0x1U << GPIO1_IOC_GPIO1A_OD_GPIO1A7_OD_SHIFT)               /* 0x00000080 */
/* GPIO1B_OD */
#define GPIO1_IOC_GPIO1B_OD_OFFSET                         (0x714U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B0_OD_SHIFT               (0U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B0_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B0_OD_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B1_OD_SHIFT               (1U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B1_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B1_OD_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B2_OD_SHIFT               (2U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B2_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B2_OD_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B3_OD_SHIFT               (3U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B3_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B3_OD_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B4_OD_SHIFT               (4U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B4_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B4_OD_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B5_OD_SHIFT               (5U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B5_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B5_OD_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B6_OD_SHIFT               (6U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B6_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B6_OD_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1B_OD_GPIO1B7_OD_SHIFT               (7U)
#define GPIO1_IOC_GPIO1B_OD_GPIO1B7_OD_MASK                (0x1U << GPIO1_IOC_GPIO1B_OD_GPIO1B7_OD_SHIFT)               /* 0x00000080 */
/* GPIO1C_OD */
#define GPIO1_IOC_GPIO1C_OD_OFFSET                         (0x718U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C0_OD_SHIFT               (0U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C0_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C0_OD_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C1_OD_SHIFT               (1U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C1_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C1_OD_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C2_OD_SHIFT               (2U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C2_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C2_OD_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C3_OD_SHIFT               (3U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C3_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C3_OD_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C4_OD_SHIFT               (4U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C4_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C4_OD_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C5_OD_SHIFT               (5U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C5_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C5_OD_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C6_OD_SHIFT               (6U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C6_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C6_OD_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1C_OD_GPIO1C7_OD_SHIFT               (7U)
#define GPIO1_IOC_GPIO1C_OD_GPIO1C7_OD_MASK                (0x1U << GPIO1_IOC_GPIO1C_OD_GPIO1C7_OD_SHIFT)               /* 0x00000080 */
/* GPIO1D_OD */
#define GPIO1_IOC_GPIO1D_OD_OFFSET                         (0x71CU)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D0_OD_SHIFT               (0U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D0_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D0_OD_SHIFT)               /* 0x00000001 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D1_OD_SHIFT               (1U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D1_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D1_OD_SHIFT)               /* 0x00000002 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D2_OD_SHIFT               (2U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D2_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D2_OD_SHIFT)               /* 0x00000004 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D3_OD_SHIFT               (3U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D3_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D3_OD_SHIFT)               /* 0x00000008 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D4_OD_SHIFT               (4U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D4_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D4_OD_SHIFT)               /* 0x00000010 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D5_OD_SHIFT               (5U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D5_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D5_OD_SHIFT)               /* 0x00000020 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D6_OD_SHIFT               (6U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D6_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D6_OD_SHIFT)               /* 0x00000040 */
#define GPIO1_IOC_GPIO1D_OD_GPIO1D7_OD_SHIFT               (7U)
#define GPIO1_IOC_GPIO1D_OD_GPIO1D7_OD_MASK                (0x1U << GPIO1_IOC_GPIO1D_OD_GPIO1D7_OD_SHIFT)               /* 0x00000080 */
/* VCCIO0_POC */
#define GPIO1_IOC_VCCIO0_POC_OFFSET                        (0x810U)
#define GPIO1_IOC_VCCIO0_POC_RESERVED_SHIFT                (0U)
#define GPIO1_IOC_VCCIO0_POC_RESERVED_MASK                 (0xFFU << GPIO1_IOC_VCCIO0_POC_RESERVED_SHIFT)               /* 0x000000FF */
#define GPIO1_IOC_VCCIO0_POC_VCCIO0_VD_SHIFT               (8U)
#define GPIO1_IOC_VCCIO0_POC_VCCIO0_VD_MASK                (0x1U << GPIO1_IOC_VCCIO0_POC_VCCIO0_VD_SHIFT)               /* 0x00000100 */
/* VCCIO1_POC */
#define GPIO1_IOC_VCCIO1_POC_OFFSET                        (0x820U)
#define GPIO1_IOC_VCCIO1_POC_RESERVED_SHIFT                (0U)
#define GPIO1_IOC_VCCIO1_POC_RESERVED_MASK                 (0xFFU << GPIO1_IOC_VCCIO1_POC_RESERVED_SHIFT)               /* 0x000000FF */
#define GPIO1_IOC_VCCIO1_POC_VCCIO1_VD_SHIFT               (8U)
#define GPIO1_IOC_VCCIO1_POC_VCCIO1_VD_MASK                (0x1U << GPIO1_IOC_VCCIO1_POC_VCCIO1_VD_SHIFT)               /* 0x00000100 */
/***************************************GPIO3_IOC****************************************/
/* GPIO3A_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_OFFSET                (0x60U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO3A_IOMUX_SEL_H */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_OFFSET                (0x64U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO3B_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_OFFSET                (0x68U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO3B_IOMUX_SEL_H */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_OFFSET                (0x6CU)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO3C_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_OFFSET                (0x70U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_MASK      (0xFU << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO3A_DS_0 */
#define GPIO3_IOC_GPIO3A_DS_0_OFFSET                       (0x190U)
#define GPIO3_IOC_GPIO3A_DS_0_GPIO3A0_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3A_DS_0_GPIO3A0_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_0_GPIO3A0_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS_0_GPIO3A1_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3A_DS_0_GPIO3A1_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_0_GPIO3A1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3A_DS_1 */
#define GPIO3_IOC_GPIO3A_DS_1_OFFSET                       (0x194U)
#define GPIO3_IOC_GPIO3A_DS_1_GPIO3A2_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3A_DS_1_GPIO3A2_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_1_GPIO3A2_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS_1_GPIO3A3_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3A_DS_1_GPIO3A3_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_1_GPIO3A3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3A_DS_2 */
#define GPIO3_IOC_GPIO3A_DS_2_OFFSET                       (0x198U)
#define GPIO3_IOC_GPIO3A_DS_2_GPIO3A4_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3A_DS_2_GPIO3A4_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_2_GPIO3A4_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS_2_GPIO3A5_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3A_DS_2_GPIO3A5_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_2_GPIO3A5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3A_DS_3 */
#define GPIO3_IOC_GPIO3A_DS_3_OFFSET                       (0x19CU)
#define GPIO3_IOC_GPIO3A_DS_3_GPIO3A6_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3A_DS_3_GPIO3A6_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_3_GPIO3A6_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS_3_GPIO3A7_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3A_DS_3_GPIO3A7_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3A_DS_3_GPIO3A7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3B_DS_0 */
#define GPIO3_IOC_GPIO3B_DS_0_OFFSET                       (0x1A0U)
#define GPIO3_IOC_GPIO3B_DS_0_GPIO3B0_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3B_DS_0_GPIO3B0_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_0_GPIO3B0_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS_0_GPIO3B1_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3B_DS_0_GPIO3B1_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_0_GPIO3B1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3B_DS_1 */
#define GPIO3_IOC_GPIO3B_DS_1_OFFSET                       (0x1A4U)
#define GPIO3_IOC_GPIO3B_DS_1_GPIO3B2_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3B_DS_1_GPIO3B2_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_1_GPIO3B2_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS_1_GPIO3B3_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3B_DS_1_GPIO3B3_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_1_GPIO3B3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3B_DS_2 */
#define GPIO3_IOC_GPIO3B_DS_2_OFFSET                       (0x1A8U)
#define GPIO3_IOC_GPIO3B_DS_2_GPIO3B4_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3B_DS_2_GPIO3B4_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_2_GPIO3B4_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS_2_GPIO3B5_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3B_DS_2_GPIO3B5_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_2_GPIO3B5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3B_DS_3 */
#define GPIO3_IOC_GPIO3B_DS_3_OFFSET                       (0x1ACU)
#define GPIO3_IOC_GPIO3B_DS_3_GPIO3B6_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3B_DS_3_GPIO3B6_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_3_GPIO3B6_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS_3_GPIO3B7_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3B_DS_3_GPIO3B7_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3B_DS_3_GPIO3B7_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3C_DS_0 */
#define GPIO3_IOC_GPIO3C_DS_0_OFFSET                       (0x1B0U)
#define GPIO3_IOC_GPIO3C_DS_0_GPIO3C0_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3C_DS_0_GPIO3C0_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3C_DS_0_GPIO3C0_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3C_DS_0_GPIO3C1_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3C_DS_0_GPIO3C1_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3C_DS_0_GPIO3C1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3C_DS_1 */
#define GPIO3_IOC_GPIO3C_DS_1_OFFSET                       (0x1B4U)
#define GPIO3_IOC_GPIO3C_DS_1_GPIO3C2_DS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3C_DS_1_GPIO3C2_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3C_DS_1_GPIO3C2_DS_SHIFT)            /* 0x0000003F */
#define GPIO3_IOC_GPIO3C_DS_1_GPIO3C3_DS_SHIFT             (8U)
#define GPIO3_IOC_GPIO3C_DS_1_GPIO3C3_DS_MASK              (0x3FU << GPIO3_IOC_GPIO3C_DS_1_GPIO3C3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO3A_PULL */
#define GPIO3_IOC_GPIO3A_PULL_OFFSET                       (0x230U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A0_PULL_SHIFT           (0U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A0_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A1_PULL_SHIFT           (2U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A1_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A2_PULL_SHIFT           (4U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A2_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A3_PULL_SHIFT           (6U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A3_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A4_PULL_SHIFT           (8U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A4_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A5_PULL_SHIFT           (10U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A5_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A6_PULL_SHIFT           (12U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A6_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A7_PULL_SHIFT           (14U)
#define GPIO3_IOC_GPIO3A_PULL_GPIO3A7_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3A_PULL_GPIO3A7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO3B_PULL */
#define GPIO3_IOC_GPIO3B_PULL_OFFSET                       (0x234U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B0_PULL_SHIFT           (0U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B0_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B1_PULL_SHIFT           (2U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B1_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B2_PULL_SHIFT           (4U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B2_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B3_PULL_SHIFT           (6U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B3_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B4_PULL_SHIFT           (8U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B4_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B5_PULL_SHIFT           (10U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B5_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B6_PULL_SHIFT           (12U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B6_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B6_PULL_SHIFT)           /* 0x00003000 */
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B7_PULL_SHIFT           (14U)
#define GPIO3_IOC_GPIO3B_PULL_GPIO3B7_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3B_PULL_GPIO3B7_PULL_SHIFT)           /* 0x0000C000 */
/* GPIO3C_PULL */
#define GPIO3_IOC_GPIO3C_PULL_OFFSET                       (0x238U)
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C0_PULL_SHIFT           (0U)
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C0_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3C_PULL_GPIO3C0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C1_PULL_SHIFT           (2U)
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C1_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3C_PULL_GPIO3C1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C2_PULL_SHIFT           (4U)
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C2_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3C_PULL_GPIO3C2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C3_PULL_SHIFT           (6U)
#define GPIO3_IOC_GPIO3C_PULL_GPIO3C3_PULL_MASK            (0x3U << GPIO3_IOC_GPIO3C_PULL_GPIO3C3_PULL_SHIFT)           /* 0x000000C0 */
/* GPIO3A_IE */
#define GPIO3_IOC_GPIO3A_IE_OFFSET                         (0x330U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A0_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A0_IE_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A1_IE_SHIFT               (1U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A1_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A1_IE_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A2_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A2_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A2_IE_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A3_IE_SHIFT               (3U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A3_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A3_IE_SHIFT)               /* 0x00000008 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A4_IE_SHIFT               (4U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A4_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A4_IE_SHIFT)               /* 0x00000010 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A5_IE_SHIFT               (5U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A5_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A5_IE_SHIFT)               /* 0x00000020 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A6_IE_SHIFT               (6U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A6_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A6_IE_SHIFT)               /* 0x00000040 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A7_IE_SHIFT               (7U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A7_IE_MASK                (0x1U << GPIO3_IOC_GPIO3A_IE_GPIO3A7_IE_SHIFT)               /* 0x00000080 */
/* GPIO3B_IE */
#define GPIO3_IOC_GPIO3B_IE_OFFSET                         (0x334U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B0_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B0_IE_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B1_IE_SHIFT               (1U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B1_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B1_IE_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B2_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B2_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B2_IE_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B3_IE_SHIFT               (3U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B3_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B3_IE_SHIFT)               /* 0x00000008 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B4_IE_SHIFT               (4U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B4_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B4_IE_SHIFT)               /* 0x00000010 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B5_IE_SHIFT               (5U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B5_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B5_IE_SHIFT)               /* 0x00000020 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B6_IE_SHIFT               (6U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B6_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B6_IE_SHIFT)               /* 0x00000040 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B7_IE_SHIFT               (7U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B7_IE_MASK                (0x1U << GPIO3_IOC_GPIO3B_IE_GPIO3B7_IE_SHIFT)               /* 0x00000080 */
/* GPIO3C_IE */
#define GPIO3_IOC_GPIO3C_IE_OFFSET                         (0x338U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C0_IE_MASK                (0x1U << GPIO3_IOC_GPIO3C_IE_GPIO3C0_IE_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C1_IE_SHIFT               (1U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C1_IE_MASK                (0x1U << GPIO3_IOC_GPIO3C_IE_GPIO3C1_IE_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C2_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C2_IE_MASK                (0x1U << GPIO3_IOC_GPIO3C_IE_GPIO3C2_IE_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C3_IE_SHIFT               (3U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C3_IE_MASK                (0x1U << GPIO3_IOC_GPIO3C_IE_GPIO3C3_IE_SHIFT)               /* 0x00000008 */
/* GPIO3A_SMT */
#define GPIO3_IOC_GPIO3A_SMT_OFFSET                        (0x430U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A0_SMT_SHIFT             (0U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A0_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A1_SMT_SHIFT             (1U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A1_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A2_SMT_SHIFT             (2U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A2_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A3_SMT_SHIFT             (3U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A3_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A4_SMT_SHIFT             (4U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A4_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A5_SMT_SHIFT             (5U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A5_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A6_SMT_SHIFT             (6U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A6_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A7_SMT_SHIFT             (7U)
#define GPIO3_IOC_GPIO3A_SMT_GPIO3A7_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3A_SMT_GPIO3A7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO3B_SMT */
#define GPIO3_IOC_GPIO3B_SMT_OFFSET                        (0x434U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B0_SMT_SHIFT             (0U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B0_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B1_SMT_SHIFT             (1U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B1_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B2_SMT_SHIFT             (2U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B2_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B3_SMT_SHIFT             (3U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B3_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B4_SMT_SHIFT             (4U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B4_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B5_SMT_SHIFT             (5U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B5_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B6_SMT_SHIFT             (6U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B6_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B6_SMT_SHIFT)             /* 0x00000040 */
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B7_SMT_SHIFT             (7U)
#define GPIO3_IOC_GPIO3B_SMT_GPIO3B7_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3B_SMT_GPIO3B7_SMT_SHIFT)             /* 0x00000080 */
/* GPIO3C_SMT */
#define GPIO3_IOC_GPIO3C_SMT_OFFSET                        (0x438U)
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C0_SMT_SHIFT             (0U)
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C0_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3C_SMT_GPIO3C0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C1_SMT_SHIFT             (1U)
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C1_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3C_SMT_GPIO3C1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C2_SMT_SHIFT             (2U)
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C2_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3C_SMT_GPIO3C2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C3_SMT_SHIFT             (3U)
#define GPIO3_IOC_GPIO3C_SMT_GPIO3C3_SMT_MASK              (0x1U << GPIO3_IOC_GPIO3C_SMT_GPIO3C3_SMT_SHIFT)             /* 0x00000008 */
/* GPIO3A_SUS */
#define GPIO3_IOC_GPIO3A_SUS_OFFSET                        (0x530U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A0_SUS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A0_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A1_SUS_SHIFT             (1U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A1_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A2_SUS_SHIFT             (2U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A2_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A3_SUS_SHIFT             (3U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A3_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A4_SUS_SHIFT             (4U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A4_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A5_SUS_SHIFT             (5U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A5_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A6_SUS_SHIFT             (6U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A6_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A7_SUS_SHIFT             (7U)
#define GPIO3_IOC_GPIO3A_SUS_GPIO3A7_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3A_SUS_GPIO3A7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO3B_SUS */
#define GPIO3_IOC_GPIO3B_SUS_OFFSET                        (0x534U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B0_SUS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B0_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B1_SUS_SHIFT             (1U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B1_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B2_SUS_SHIFT             (2U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B2_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B3_SUS_SHIFT             (3U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B3_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B4_SUS_SHIFT             (4U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B4_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B5_SUS_SHIFT             (5U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B5_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B6_SUS_SHIFT             (6U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B6_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B7_SUS_SHIFT             (7U)
#define GPIO3_IOC_GPIO3B_SUS_GPIO3B7_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3B_SUS_GPIO3B7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO3C_SUS */
#define GPIO3_IOC_GPIO3C_SUS_OFFSET                        (0x538U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C0_SUS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C0_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C1_SUS_SHIFT             (1U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C1_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C2_SUS_SHIFT             (2U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C2_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C3_SUS_SHIFT             (3U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C3_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C3_SUS_SHIFT)             /* 0x00000008 */
/* GPIO3A_SL */
#define GPIO3_IOC_GPIO3A_SL_OFFSET                         (0x630U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A0_SL_SHIFT               (0U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A0_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A0_SL_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A1_SL_SHIFT               (2U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A1_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A1_SL_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A2_SL_SHIFT               (4U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A2_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A2_SL_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A3_SL_SHIFT               (6U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A3_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A4_SL_SHIFT               (8U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A4_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A4_SL_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A5_SL_SHIFT               (10U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A5_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A6_SL_SHIFT               (12U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A6_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A6_SL_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3A_SL_GPIO3A7_SL_SHIFT               (14U)
#define GPIO3_IOC_GPIO3A_SL_GPIO3A7_SL_MASK                (0x3U << GPIO3_IOC_GPIO3A_SL_GPIO3A7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO3B_SL */
#define GPIO3_IOC_GPIO3B_SL_OFFSET                         (0x634U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B0_SL_SHIFT               (0U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B0_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B0_SL_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B1_SL_SHIFT               (2U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B1_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B1_SL_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B2_SL_SHIFT               (4U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B2_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B2_SL_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B3_SL_SHIFT               (6U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B3_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B4_SL_SHIFT               (8U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B4_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B4_SL_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B5_SL_SHIFT               (10U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B5_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B6_SL_SHIFT               (12U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B6_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B6_SL_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3B_SL_GPIO3B7_SL_SHIFT               (14U)
#define GPIO3_IOC_GPIO3B_SL_GPIO3B7_SL_MASK                (0x3U << GPIO3_IOC_GPIO3B_SL_GPIO3B7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO3C_SL */
#define GPIO3_IOC_GPIO3C_SL_OFFSET                         (0x638U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C0_SL_SHIFT               (0U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C0_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C0_SL_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C1_SL_SHIFT               (2U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C1_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C1_SL_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C2_SL_SHIFT               (4U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C2_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C2_SL_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C3_SL_SHIFT               (6U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C3_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C3_SL_SHIFT)               /* 0x000000C0 */
/* GPIO3A_OD */
#define GPIO3_IOC_GPIO3A_OD_OFFSET                         (0x730U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A0_OD_SHIFT               (0U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A0_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A0_OD_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A1_OD_SHIFT               (1U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A1_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A1_OD_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A2_OD_SHIFT               (2U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A2_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A2_OD_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A3_OD_SHIFT               (3U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A3_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A3_OD_SHIFT)               /* 0x00000008 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A4_OD_SHIFT               (4U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A4_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A4_OD_SHIFT)               /* 0x00000010 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A5_OD_SHIFT               (5U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A5_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A5_OD_SHIFT)               /* 0x00000020 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A6_OD_SHIFT               (6U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A6_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A6_OD_SHIFT)               /* 0x00000040 */
#define GPIO3_IOC_GPIO3A_OD_GPIO3A7_OD_SHIFT               (7U)
#define GPIO3_IOC_GPIO3A_OD_GPIO3A7_OD_MASK                (0x1U << GPIO3_IOC_GPIO3A_OD_GPIO3A7_OD_SHIFT)               /* 0x00000080 */
/* GPIO3B_OD */
#define GPIO3_IOC_GPIO3B_OD_OFFSET                         (0x734U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B0_OD_SHIFT               (0U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B0_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B0_OD_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B1_OD_SHIFT               (1U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B1_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B1_OD_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B2_OD_SHIFT               (2U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B2_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B2_OD_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B3_OD_SHIFT               (3U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B3_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B3_OD_SHIFT)               /* 0x00000008 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B4_OD_SHIFT               (4U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B4_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B4_OD_SHIFT)               /* 0x00000010 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B5_OD_SHIFT               (5U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B5_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B5_OD_SHIFT)               /* 0x00000020 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B6_OD_SHIFT               (6U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B6_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B6_OD_SHIFT)               /* 0x00000040 */
#define GPIO3_IOC_GPIO3B_OD_GPIO3B7_OD_SHIFT               (7U)
#define GPIO3_IOC_GPIO3B_OD_GPIO3B7_OD_MASK                (0x1U << GPIO3_IOC_GPIO3B_OD_GPIO3B7_OD_SHIFT)               /* 0x00000080 */
/* GPIO3C_OD */
#define GPIO3_IOC_GPIO3C_OD_OFFSET                         (0x738U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C0_OD_SHIFT               (0U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C0_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C0_OD_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C1_OD_SHIFT               (1U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C1_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C1_OD_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C2_OD_SHIFT               (2U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C2_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C2_OD_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C3_OD_SHIFT               (3U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C3_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C3_OD_SHIFT)               /* 0x00000008 */
/* VCCIO3_POC */
#define GPIO3_IOC_VCCIO3_POC_OFFSET                        (0x840U)
#define GPIO3_IOC_VCCIO3_POC_RESERVED_SHIFT                (0U)
#define GPIO3_IOC_VCCIO3_POC_RESERVED_MASK                 (0xFFU << GPIO3_IOC_VCCIO3_POC_RESERVED_SHIFT)               /* 0x000000FF */
#define GPIO3_IOC_VCCIO3_POC_VCCIO3_VD_SHIFT               (8U)
#define GPIO3_IOC_VCCIO3_POC_VCCIO3_VD_MASK                (0x1U << GPIO3_IOC_VCCIO3_POC_VCCIO3_VD_SHIFT)               /* 0x00000100 */
/***************************************GPIO2_IOC****************************************/
/* GPIO2A_IOMUX_SEL_L */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_OFFSET                (0x40U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_SHIFT     (0U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_SHIFT)     /* 0x0000000F */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_SHIFT     (4U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A2_SEL_SHIFT     (8U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A2_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A2_SEL_SHIFT)     /* 0x00000F00 */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A3_SEL_SHIFT     (12U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A3_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A3_SEL_SHIFT)     /* 0x0000F000 */
/* GPIO2A_IOMUX_SEL_H */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_OFFSET                (0x44U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A4_SEL_SHIFT     (0U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A4_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A4_SEL_SHIFT)     /* 0x0000000F */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A5_SEL_SHIFT     (4U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A5_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A5_SEL_SHIFT)     /* 0x000000F0 */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A6_SEL_SHIFT     (8U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A6_SEL_MASK      (0xFU << GPIO2_IOC_GPIO2A_IOMUX_SEL_H_GPIO2A6_SEL_SHIFT)     /* 0x00000F00 */
/* GPIO2A_DS_0 */
#define GPIO2_IOC_GPIO2A_DS_0_OFFSET                       (0x160U)
#define GPIO2_IOC_GPIO2A_DS_0_GPIO2A0_DS_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_DS_0_GPIO2A0_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_0_GPIO2A0_DS_SHIFT)            /* 0x0000003F */
#define GPIO2_IOC_GPIO2A_DS_0_GPIO2A1_DS_SHIFT             (8U)
#define GPIO2_IOC_GPIO2A_DS_0_GPIO2A1_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_0_GPIO2A1_DS_SHIFT)            /* 0x00003F00 */
/* GPIO2A_DS_1 */
#define GPIO2_IOC_GPIO2A_DS_1_OFFSET                       (0x164U)
#define GPIO2_IOC_GPIO2A_DS_1_GPIO2A2_DS_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_DS_1_GPIO2A2_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_1_GPIO2A2_DS_SHIFT)            /* 0x0000003F */
#define GPIO2_IOC_GPIO2A_DS_1_GPIO2A3_DS_SHIFT             (8U)
#define GPIO2_IOC_GPIO2A_DS_1_GPIO2A3_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_1_GPIO2A3_DS_SHIFT)            /* 0x00003F00 */
/* GPIO2A_DS_2 */
#define GPIO2_IOC_GPIO2A_DS_2_OFFSET                       (0x168U)
#define GPIO2_IOC_GPIO2A_DS_2_GPIO2A4_DS_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_DS_2_GPIO2A4_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_2_GPIO2A4_DS_SHIFT)            /* 0x0000003F */
#define GPIO2_IOC_GPIO2A_DS_2_GPIO2A5_DS_SHIFT             (8U)
#define GPIO2_IOC_GPIO2A_DS_2_GPIO2A5_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_2_GPIO2A5_DS_SHIFT)            /* 0x00003F00 */
/* GPIO2A_DS_3 */
#define GPIO2_IOC_GPIO2A_DS_3_OFFSET                       (0x16CU)
#define GPIO2_IOC_GPIO2A_DS_3_GPIO2A6_DS_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_DS_3_GPIO2A6_DS_MASK              (0x3FU << GPIO2_IOC_GPIO2A_DS_3_GPIO2A6_DS_SHIFT)            /* 0x0000003F */
/* GPIO2A_PULL */
#define GPIO2_IOC_GPIO2A_PULL_OFFSET                       (0x220U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A0_PULL_SHIFT           (0U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A0_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A0_PULL_SHIFT)           /* 0x00000003 */
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A1_PULL_SHIFT           (2U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A1_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A1_PULL_SHIFT)           /* 0x0000000C */
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A2_PULL_SHIFT           (4U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A2_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A2_PULL_SHIFT)           /* 0x00000030 */
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A3_PULL_SHIFT           (6U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A3_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A3_PULL_SHIFT)           /* 0x000000C0 */
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A4_PULL_SHIFT           (8U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A4_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A4_PULL_SHIFT)           /* 0x00000300 */
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A5_PULL_SHIFT           (10U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A5_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A5_PULL_SHIFT)           /* 0x00000C00 */
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A6_PULL_SHIFT           (12U)
#define GPIO2_IOC_GPIO2A_PULL_GPIO2A6_PULL_MASK            (0x3U << GPIO2_IOC_GPIO2A_PULL_GPIO2A6_PULL_SHIFT)           /* 0x00003000 */
/* GPIO2A_IE */
#define GPIO2_IOC_GPIO2A_IE_OFFSET                         (0x320U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A0_IE_SHIFT               (0U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A0_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A0_IE_SHIFT)               /* 0x00000001 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A1_IE_SHIFT               (1U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A1_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A1_IE_SHIFT)               /* 0x00000002 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A2_IE_SHIFT               (2U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A2_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A2_IE_SHIFT)               /* 0x00000004 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A3_IE_SHIFT               (3U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A3_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A3_IE_SHIFT)               /* 0x00000008 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A4_IE_SHIFT               (4U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A4_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A4_IE_SHIFT)               /* 0x00000010 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A5_IE_SHIFT               (5U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A5_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A5_IE_SHIFT)               /* 0x00000020 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A6_IE_SHIFT               (6U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A6_IE_MASK                (0x1U << GPIO2_IOC_GPIO2A_IE_GPIO2A6_IE_SHIFT)               /* 0x00000040 */
/* GPIO2A_SMT */
#define GPIO2_IOC_GPIO2A_SMT_OFFSET                        (0x420U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A0_SMT_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A0_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A0_SMT_SHIFT)             /* 0x00000001 */
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A1_SMT_SHIFT             (1U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A1_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A1_SMT_SHIFT)             /* 0x00000002 */
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A2_SMT_SHIFT             (2U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A2_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A2_SMT_SHIFT)             /* 0x00000004 */
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A3_SMT_SHIFT             (3U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A3_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A3_SMT_SHIFT)             /* 0x00000008 */
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A4_SMT_SHIFT             (4U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A4_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A4_SMT_SHIFT)             /* 0x00000010 */
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A5_SMT_SHIFT             (5U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A5_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A5_SMT_SHIFT)             /* 0x00000020 */
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A6_SMT_SHIFT             (6U)
#define GPIO2_IOC_GPIO2A_SMT_GPIO2A6_SMT_MASK              (0x1U << GPIO2_IOC_GPIO2A_SMT_GPIO2A6_SMT_SHIFT)             /* 0x00000040 */
/* GPIO2A_SUS */
#define GPIO2_IOC_GPIO2A_SUS_OFFSET                        (0x520U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A0_SUS_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A0_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A1_SUS_SHIFT             (1U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A1_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A2_SUS_SHIFT             (2U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A2_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A3_SUS_SHIFT             (3U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A3_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A4_SUS_SHIFT             (4U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A4_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A5_SUS_SHIFT             (5U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A5_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A6_SUS_SHIFT             (6U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A6_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A6_SUS_SHIFT)             /* 0x00000040 */
/* GPIO2A_SL */
#define GPIO2_IOC_GPIO2A_SL_OFFSET                         (0x620U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A0_SL_SHIFT               (0U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A0_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A0_SL_SHIFT)               /* 0x00000003 */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A1_SL_SHIFT               (2U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A1_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A1_SL_SHIFT)               /* 0x0000000C */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A2_SL_SHIFT               (4U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A2_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A2_SL_SHIFT)               /* 0x00000030 */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A3_SL_SHIFT               (6U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A3_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A4_SL_SHIFT               (8U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A4_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A4_SL_SHIFT)               /* 0x00000300 */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A5_SL_SHIFT               (10U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A5_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A6_SL_SHIFT               (12U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A6_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A6_SL_SHIFT)               /* 0x00003000 */
/* GPIO2A_OD */
#define GPIO2_IOC_GPIO2A_OD_OFFSET                         (0x720U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A0_OD_SHIFT               (0U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A0_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A0_OD_SHIFT)               /* 0x00000001 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A1_OD_SHIFT               (1U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A1_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A1_OD_SHIFT)               /* 0x00000002 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A2_OD_SHIFT               (2U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A2_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A2_OD_SHIFT)               /* 0x00000004 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A3_OD_SHIFT               (3U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A3_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A3_OD_SHIFT)               /* 0x00000008 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A4_OD_SHIFT               (4U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A4_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A4_OD_SHIFT)               /* 0x00000010 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A5_OD_SHIFT               (5U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A5_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A5_OD_SHIFT)               /* 0x00000020 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A6_OD_SHIFT               (6U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A6_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A6_OD_SHIFT)               /* 0x00000040 */
/* VCCIO2_POC */
#define GPIO2_IOC_VCCIO2_POC_OFFSET                        (0x830U)
#define GPIO2_IOC_VCCIO2_POC_RESERVED_SHIFT                (0U)
#define GPIO2_IOC_VCCIO2_POC_RESERVED_MASK                 (0x1FU << GPIO2_IOC_VCCIO2_POC_RESERVED_SHIFT)               /* 0x0000001F */
#define GPIO2_IOC_VCCIO2_POC_FORCE_JTAG_SHIFT              (5U)
#define GPIO2_IOC_VCCIO2_POC_FORCE_JTAG_MASK               (0x1U << GPIO2_IOC_VCCIO2_POC_FORCE_JTAG_SHIFT)              /* 0x00000020 */
#define GPIO2_IOC_VCCIO2_POC_VCCIO2_VD_SHIFT               (8U)
#define GPIO2_IOC_VCCIO2_POC_VCCIO2_VD_MASK                (0x1U << GPIO2_IOC_VCCIO2_POC_VCCIO2_VD_SHIFT)               /* 0x00000100 */
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
#define WDT_CCVR                                           (0xFFFFU)
#define WDT_CCVR_CUR_CNT_SHIFT                             (0U)
#define WDT_CCVR_CUR_CNT_MASK                              (0xFFFFFFFFU << WDT_CCVR_CUR_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* CRR */
#define WDT_CRR_OFFSET                                     (0xCU)
#define WDT_CRR_CNT_RESTART_SHIFT                          (0U)
#define WDT_CRR_CNT_RESTART_MASK                           (0xFFU << WDT_CRR_CNT_RESTART_SHIFT)                         /* 0x000000FF */
/* STAT */
#define WDT_STAT_OFFSET                                    (0x10U)
#define WDT_STAT                                           (0x0U)
#define WDT_STAT_STATUS_SHIFT                              (0U)
#define WDT_STAT_STATUS_MASK                               (0x1U << WDT_STAT_STATUS_SHIFT)                              /* 0x00000001 */
/* EOI */
#define WDT_EOI_OFFSET                                     (0x14U)
#define WDT_EOI                                            (0x0U)
#define WDT_EOI_INT_CLR_SHIFT                              (0U)
#define WDT_EOI_INT_CLR_MASK                               (0x1U << WDT_EOI_INT_CLR_SHIFT)                              /* 0x00000001 */
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_OFFSET                                    (0x0U)
#define UART_RBR                                           (0x0U)
#define UART_RBR_DATA_INPUT_SHIFT                          (0U)
#define UART_RBR_DATA_INPUT_MASK                           (0xFFU << UART_RBR_DATA_INPUT_SHIFT)                         /* 0x000000FF */
/* DLL */
#define UART_DLL_OFFSET                                    (0x0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT                 (0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_MASK                  (0xFFU << UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT)                /* 0x000000FF */
/* THR */
#define UART_THR_OFFSET                                    (0x0U)
#define UART_THR_DATA_OUTPUT_SHIFT                         (0U)
#define UART_THR_DATA_OUTPUT_MASK                          (0xFFU << UART_THR_DATA_OUTPUT_SHIFT)                        /* 0x000000FF */
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
/* IIR */
#define UART_IIR_OFFSET                                    (0x8U)
#define UART_IIR                                           (0x1U)
#define UART_IIR_INT_ID_SHIFT                              (0U)
#define UART_IIR_INT_ID_MASK                               (0xFU << UART_IIR_INT_ID_SHIFT)                              /* 0x0000000F */
#define UART_IIR_FIFOS_EN_SHIFT                            (6U)
#define UART_IIR_FIFOS_EN_MASK                             (0x3U << UART_IIR_FIFOS_EN_SHIFT)                            /* 0x000000C0 */
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
#define UART_LCR_STICK_PARITY_SHIFT                        (5U)
#define UART_LCR_STICK_PARITY_MASK                         (0x1U << UART_LCR_STICK_PARITY_SHIFT)                        /* 0x00000020 */
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
#define UART_LSR_PARITY_ERROR_SHIFT                        (2U)
#define UART_LSR_PARITY_ERROR_MASK                         (0x1U << UART_LSR_PARITY_ERROR_SHIFT)                        /* 0x00000004 */
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
#define UART_STHR_OFFSET                                   (0x30U)
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
#define UART_TFL                                           (0x0U)
#define UART_TFL_TRANS_FIFO_LEVEL_SHIFT                    (0U)
#define UART_TFL_TRANS_FIFO_LEVEL_MASK                     (0x3FU << UART_TFL_TRANS_FIFO_LEVEL_SHIFT)                   /* 0x0000003F */
/* RFL */
#define UART_RFL_OFFSET                                    (0x84U)
#define UART_RFL                                           (0x0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT                  (0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_MASK                   (0x3FU << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)                 /* 0x0000003F */
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
#define UART_CPR                                           (0x43FF2U)
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
#define UART_UCV                                           (0x3430322AU)
#define UART_UCV_VER_SHIFT                                 (0U)
#define UART_UCV_VER_MASK                                  (0xFFFFFFFFU << UART_UCV_VER_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_OFFSET                                    (0xFCU)
#define UART_CTR                                           (0x44570110U)
#define UART_CTR_PERIPHERAL_ID_SHIFT                       (0U)
#define UART_CTR_PERIPHERAL_ID_MASK                        (0xFFFFFFFFU << UART_CTR_PERIPHERAL_ID_SHIFT)                /* 0xFFFFFFFF */
/*****************************************INTMUX*****************************************/
/* INT_ENABLE_GROUP0 */
#define INTMUX_INT_ENABLE_GROUP0_OFFSET                    (0x0U)
#define INTMUX_INT_ENABLE_GROUP0_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP0_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP0_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP1 */
#define INTMUX_INT_ENABLE_GROUP1_OFFSET                    (0x4U)
#define INTMUX_INT_ENABLE_GROUP1_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP1_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP1_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP2 */
#define INTMUX_INT_ENABLE_GROUP2_OFFSET                    (0x8U)
#define INTMUX_INT_ENABLE_GROUP2_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP2_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP2_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP3 */
#define INTMUX_INT_ENABLE_GROUP3_OFFSET                    (0xCU)
#define INTMUX_INT_ENABLE_GROUP3_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP3_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP3_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP4 */
#define INTMUX_INT_ENABLE_GROUP4_OFFSET                    (0x10U)
#define INTMUX_INT_ENABLE_GROUP4_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP4_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP4_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP5 */
#define INTMUX_INT_ENABLE_GROUP5_OFFSET                    (0x14U)
#define INTMUX_INT_ENABLE_GROUP5_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP5_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP5_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP6 */
#define INTMUX_INT_ENABLE_GROUP6_OFFSET                    (0x18U)
#define INTMUX_INT_ENABLE_GROUP6_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP6_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP6_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_ENABLE_GROUP7 */
#define INTMUX_INT_ENABLE_GROUP7_OFFSET                    (0x1CU)
#define INTMUX_INT_ENABLE_GROUP7_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP7_ENABLE_MASK               (0xFFFFFFFFU << INTMUX_INT_ENABLE_GROUP7_ENABLE_SHIFT)       /* 0xFFFFFFFF */
/* INT_FLAG_GROUP0 */
#define INTMUX_INT_FLAG_GROUP0_OFFSET                      (0x80U)
#define INTMUX_INT_FLAG_GROUP0                             (0x0U)
#define INTMUX_INT_FLAG_GROUP0_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP0_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP0_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP1 */
#define INTMUX_INT_FLAG_GROUP1_OFFSET                      (0x84U)
#define INTMUX_INT_FLAG_GROUP1                             (0x0U)
#define INTMUX_INT_FLAG_GROUP1_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP1_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP1_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP2 */
#define INTMUX_INT_FLAG_GROUP2_OFFSET                      (0x88U)
#define INTMUX_INT_FLAG_GROUP2                             (0x0U)
#define INTMUX_INT_FLAG_GROUP2_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP2_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP2_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP3 */
#define INTMUX_INT_FLAG_GROUP3_OFFSET                      (0x8CU)
#define INTMUX_INT_FLAG_GROUP3                             (0x0U)
#define INTMUX_INT_FLAG_GROUP3_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP3_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP3_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP4 */
#define INTMUX_INT_FLAG_GROUP4_OFFSET                      (0x90U)
#define INTMUX_INT_FLAG_GROUP4                             (0x0U)
#define INTMUX_INT_FLAG_GROUP4_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP4_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP4_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP5 */
#define INTMUX_INT_FLAG_GROUP5_OFFSET                      (0x94U)
#define INTMUX_INT_FLAG_GROUP5                             (0x0U)
#define INTMUX_INT_FLAG_GROUP5_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP5_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP5_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP6 */
#define INTMUX_INT_FLAG_GROUP6_OFFSET                      (0x98U)
#define INTMUX_INT_FLAG_GROUP6                             (0x0U)
#define INTMUX_INT_FLAG_GROUP6_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP6_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP6_FLAG_SHIFT)           /* 0xFFFFFFFF */
/* INT_FLAG_GROUP7 */
#define INTMUX_INT_FLAG_GROUP7_OFFSET                      (0x9CU)
#define INTMUX_INT_FLAG_GROUP7                             (0x0U)
#define INTMUX_INT_FLAG_GROUP7_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP7_FLAG_MASK                   (0xFFFFFFFFU << INTMUX_INT_FLAG_GROUP7_FLAG_SHIFT)           /* 0xFFFFFFFF */
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
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK3528_H */

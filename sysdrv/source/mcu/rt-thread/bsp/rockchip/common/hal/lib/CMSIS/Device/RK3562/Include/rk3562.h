/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK3562_H
#define __RK3562_H
#ifdef __cplusplus
  extern "C" {
#endif
/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* GPIO1_IOC Register Structure Define */
struct GPIO1_IOC_REG {
    __IO uint32_t GPIO1A_IOMUX_SEL_L;                 /* Address Offset: 0x0000 */
    __IO uint32_t GPIO1A_IOMUX_SEL_H;                 /* Address Offset: 0x0004 */
    __IO uint32_t GPIO1B_IOMUX_SEL_L;                 /* Address Offset: 0x0008 */
    __IO uint32_t GPIO1B_IOMUX_SEL_H;                 /* Address Offset: 0x000C */
    __IO uint32_t GPIO1C_IOMUX_SEL_L;                 /* Address Offset: 0x0010 */
    __IO uint32_t GPIO1C_IOMUX_SEL_H;                 /* Address Offset: 0x0014 */
    __IO uint32_t GPIO1D_IOMUX_SEL_L;                 /* Address Offset: 0x0018 */
    __IO uint32_t GPIO1D_IOMUX_SEL_H;                 /* Address Offset: 0x001C */
         uint32_t RESERVED0020[24];                   /* Address Offset: 0x0020 */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0080 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0084 */
    __IO uint32_t GPIO1C_P;                           /* Address Offset: 0x0088 */
    __IO uint32_t GPIO1D_P;                           /* Address Offset: 0x008C */
         uint32_t RESERVED0090[12];                   /* Address Offset: 0x0090 */
    __IO uint32_t GPIO1A_IE;                          /* Address Offset: 0x00C0 */
    __IO uint32_t GPIO1B_IE;                          /* Address Offset: 0x00C4 */
    __IO uint32_t GPIO1C_IE;                          /* Address Offset: 0x00C8 */
    __IO uint32_t GPIO1D_IE;                          /* Address Offset: 0x00CC */
         uint32_t RESERVED00D0[12];                   /* Address Offset: 0x00D0 */
    __IO uint32_t GPIO1A_OD;                          /* Address Offset: 0x0100 */
    __IO uint32_t GPIO1B_OD;                          /* Address Offset: 0x0104 */
    __IO uint32_t GPIO1C_OD;                          /* Address Offset: 0x0108 */
    __IO uint32_t GPIO1D_OD;                          /* Address Offset: 0x010C */
         uint32_t RESERVED0110[12];                   /* Address Offset: 0x0110 */
    __IO uint32_t GPIO1A_SUS;                         /* Address Offset: 0x0140 */
    __IO uint32_t GPIO1B_SUS;                         /* Address Offset: 0x0144 */
    __IO uint32_t GPIO1C_SUS;                         /* Address Offset: 0x0148 */
    __IO uint32_t GPIO1D_SUS;                         /* Address Offset: 0x014C */
         uint32_t RESERVED0150[12];                   /* Address Offset: 0x0150 */
    __IO uint32_t GPIO1A_SL;                          /* Address Offset: 0x0180 */
    __IO uint32_t GPIO1B_SL;                          /* Address Offset: 0x0184 */
    __IO uint32_t GPIO1C_SL;                          /* Address Offset: 0x0188 */
    __IO uint32_t GPIO1D_SL;                          /* Address Offset: 0x018C */
         uint32_t RESERVED0190[28];                   /* Address Offset: 0x0190 */
    __IO uint32_t GPIO1A_DS0;                         /* Address Offset: 0x0200 */
    __IO uint32_t GPIO1A_DS1;                         /* Address Offset: 0x0204 */
    __IO uint32_t GPIO1A_DS2;                         /* Address Offset: 0x0208 */
    __IO uint32_t GPIO1A_DS3;                         /* Address Offset: 0x020C */
    __IO uint32_t GPIO1B_DS0;                         /* Address Offset: 0x0210 */
    __IO uint32_t GPIO1B_DS1;                         /* Address Offset: 0x0214 */
    __IO uint32_t GPIO1B_DS2;                         /* Address Offset: 0x0218 */
    __IO uint32_t GPIO1B_DS3;                         /* Address Offset: 0x021C */
    __IO uint32_t GPIO1C_DS0;                         /* Address Offset: 0x0220 */
    __IO uint32_t GPIO1C_DS1;                         /* Address Offset: 0x0224 */
    __IO uint32_t GPIO1C_DS2;                         /* Address Offset: 0x0228 */
    __IO uint32_t GPIO1C_DS3;                         /* Address Offset: 0x022C */
    __IO uint32_t GPIO1D_DS0;                         /* Address Offset: 0x0230 */
    __IO uint32_t GPIO1D_DS1;                         /* Address Offset: 0x0234 */
    __IO uint32_t GPIO1D_DS2;                         /* Address Offset: 0x0238 */
    __IO uint32_t GPIO1D_DS3;                         /* Address Offset: 0x023C */
};
/* GPIO2_IOC Register Structure Define */
struct GPIO2_IOC_REG {
         uint32_t RESERVED0000[8];                    /* Address Offset: 0x0000 */
    __IO uint32_t GPIO2A_IOMUX_SEL_L;                 /* Address Offset: 0x0020 */
         uint32_t RESERVED0024[27];                   /* Address Offset: 0x0024 */
    __IO uint32_t GPIO2A_P;                           /* Address Offset: 0x0090 */
         uint32_t RESERVED0094[15];                   /* Address Offset: 0x0094 */
    __IO uint32_t GPIO2A_IE;                          /* Address Offset: 0x00D0 */
         uint32_t RESERVED00D4[15];                   /* Address Offset: 0x00D4 */
    __IO uint32_t GPIO2A_OD;                          /* Address Offset: 0x0110 */
         uint32_t RESERVED0114[15];                   /* Address Offset: 0x0114 */
    __IO uint32_t GPIO2A_SUS;                         /* Address Offset: 0x0150 */
         uint32_t RESERVED0154[15];                   /* Address Offset: 0x0154 */
    __IO uint32_t GPIO2A_SL;                          /* Address Offset: 0x0190 */
         uint32_t RESERVED0194[43];                   /* Address Offset: 0x0194 */
    __IO uint32_t GPIO2A_DS0;                         /* Address Offset: 0x0240 */
         uint32_t RESERVED0244[47];                   /* Address Offset: 0x0244 */
    __IO uint32_t IO_VSEL0;                           /* Address Offset: 0x0300 */
         uint32_t RESERVED0304[63];                   /* Address Offset: 0x0304 */
    __IO uint32_t MAC1_IO_CON0;                       /* Address Offset: 0x0400 */
    __IO uint32_t MAC1_IO_CON1;                       /* Address Offset: 0x0404 */
         uint32_t RESERVED0408[62];                   /* Address Offset: 0x0408 */
    __IO uint32_t SDCARD0_IO_CON;                     /* Address Offset: 0x0500 */
    __IO uint32_t JTAG_M1_CON;                        /* Address Offset: 0x0504 */
};
/* GPIO3_IOC Register Structure Define */
struct GPIO3_IOC_REG {
         uint32_t RESERVED0000[16];                   /* Address Offset: 0x0000 */
    __IO uint32_t GPIO3A_IOMUX_SEL_L;                 /* Address Offset: 0x0040 */
    __IO uint32_t GPIO3A_IOMUX_SEL_H;                 /* Address Offset: 0x0044 */
    __IO uint32_t GPIO3B_IOMUX_SEL_L;                 /* Address Offset: 0x0048 */
    __IO uint32_t GPIO3B_IOMUX_SEL_H;                 /* Address Offset: 0x004C */
    __IO uint32_t GPIO3C_IOMUX_SEL_L;                 /* Address Offset: 0x0050 */
    __IO uint32_t GPIO3C_IOMUX_SEL_H;                 /* Address Offset: 0x0054 */
    __IO uint32_t GPIO3D_IOMUX_SEL_L;                 /* Address Offset: 0x0058 */
    __IO uint32_t GPIO3D_IOMUX_SEL_H;                 /* Address Offset: 0x005C */
         uint32_t RESERVED0060[16];                   /* Address Offset: 0x0060 */
    __IO uint32_t GPIO3A_P;                           /* Address Offset: 0x00A0 */
    __IO uint32_t GPIO3B_P;                           /* Address Offset: 0x00A4 */
    __IO uint32_t GPIO3C_P;                           /* Address Offset: 0x00A8 */
    __IO uint32_t GPIO3D_P;                           /* Address Offset: 0x00AC */
         uint32_t RESERVED00B0[12];                   /* Address Offset: 0x00B0 */
    __IO uint32_t GPIO3A_IE;                          /* Address Offset: 0x00E0 */
    __IO uint32_t GPIO3B_IE;                          /* Address Offset: 0x00E4 */
    __IO uint32_t GPIO3C_IE;                          /* Address Offset: 0x00E8 */
    __IO uint32_t GPIO3D_IE;                          /* Address Offset: 0x00EC */
         uint32_t RESERVED00F0[12];                   /* Address Offset: 0x00F0 */
    __IO uint32_t GPIO3A_OD;                          /* Address Offset: 0x0120 */
    __IO uint32_t GPIO3B_OD;                          /* Address Offset: 0x0124 */
    __IO uint32_t GPIO3C_OD;                          /* Address Offset: 0x0128 */
    __IO uint32_t GPIO3D_OD;                          /* Address Offset: 0x012C */
         uint32_t RESERVED0130[12];                   /* Address Offset: 0x0130 */
    __IO uint32_t GPIO3A_SUS;                         /* Address Offset: 0x0160 */
    __IO uint32_t GPIO3B_SUS;                         /* Address Offset: 0x0164 */
    __IO uint32_t GPIO3C_SUS;                         /* Address Offset: 0x0168 */
    __IO uint32_t GPIO3D_SUS;                         /* Address Offset: 0x016C */
         uint32_t RESERVED0170[12];                   /* Address Offset: 0x0170 */
    __IO uint32_t GPIO3A_SL;                          /* Address Offset: 0x01A0 */
    __IO uint32_t GPIO3B_SL;                          /* Address Offset: 0x01A4 */
    __IO uint32_t GPIO3C_SL;                          /* Address Offset: 0x01A8 */
    __IO uint32_t GPIO3D_SL;                          /* Address Offset: 0x01AC */
         uint32_t RESERVED01B0[52];                   /* Address Offset: 0x01B0 */
    __IO uint32_t GPIO3A_DS0;                         /* Address Offset: 0x0280 */
    __IO uint32_t GPIO3A_DS1;                         /* Address Offset: 0x0284 */
    __IO uint32_t GPIO3A_DS2;                         /* Address Offset: 0x0288 */
    __IO uint32_t GPIO3A_DS3;                         /* Address Offset: 0x028C */
    __IO uint32_t GPIO3B_DS0;                         /* Address Offset: 0x0290 */
    __IO uint32_t GPIO3B_DS1;                         /* Address Offset: 0x0294 */
    __IO uint32_t GPIO3B_DS2;                         /* Address Offset: 0x0298 */
    __IO uint32_t GPIO3B_DS3;                         /* Address Offset: 0x029C */
    __IO uint32_t GPIO3C_DS0;                         /* Address Offset: 0x02A0 */
    __IO uint32_t GPIO3C_DS1;                         /* Address Offset: 0x02A4 */
    __IO uint32_t GPIO3C_DS2;                         /* Address Offset: 0x02A8 */
    __IO uint32_t GPIO3C_DS3;                         /* Address Offset: 0x02AC */
    __IO uint32_t GPIO3D_DS0;                         /* Address Offset: 0x02B0 */
    __IO uint32_t GPIO3D_DS1;                         /* Address Offset: 0x02B4 */
    __IO uint32_t GPIO3D_DS2;                         /* Address Offset: 0x02B8 */
    __IO uint32_t GPIO3D_DS3;                         /* Address Offset: 0x02BC */
};
/* GPIO4_IOC Register Structure Define */
struct GPIO4_IOC_REG {
         uint32_t RESERVED0000[24];                   /* Address Offset: 0x0000 */
    __IO uint32_t GPIO4A_IOMUX_SEL_L;                 /* Address Offset: 0x0060 */
    __IO uint32_t GPIO4A_IOMUX_SEL_H;                 /* Address Offset: 0x0064 */
    __IO uint32_t GPIO4B_IOMUX_SEL_L;                 /* Address Offset: 0x0068 */
    __IO uint32_t GPIO4B_IOMUX_SEL_H;                 /* Address Offset: 0x006C */
         uint32_t RESERVED0070[16];                   /* Address Offset: 0x0070 */
    __IO uint32_t GPIO4A_P;                           /* Address Offset: 0x00B0 */
    __IO uint32_t GPIO4B_P;                           /* Address Offset: 0x00B4 */
         uint32_t RESERVED00B8[14];                   /* Address Offset: 0x00B8 */
    __IO uint32_t GPIO4A_IE;                          /* Address Offset: 0x00F0 */
    __IO uint32_t GPIO4B_IE;                          /* Address Offset: 0x00F4 */
         uint32_t RESERVED00F8[14];                   /* Address Offset: 0x00F8 */
    __IO uint32_t GPIO4A_OD;                          /* Address Offset: 0x0130 */
    __IO uint32_t GPIO4B_OD;                          /* Address Offset: 0x0134 */
         uint32_t RESERVED0138[14];                   /* Address Offset: 0x0138 */
    __IO uint32_t GPIO4A_SUS;                         /* Address Offset: 0x0170 */
    __IO uint32_t GPIO4B_SUS;                         /* Address Offset: 0x0174 */
         uint32_t RESERVED0178[14];                   /* Address Offset: 0x0178 */
    __IO uint32_t GPIO4A_SL;                          /* Address Offset: 0x01B0 */
    __IO uint32_t GPIO4B_SL;                          /* Address Offset: 0x01B4 */
         uint32_t RESERVED01B8[66];                   /* Address Offset: 0x01B8 */
    __IO uint32_t GPIO4A_DS0;                         /* Address Offset: 0x02C0 */
    __IO uint32_t GPIO4A_DS1;                         /* Address Offset: 0x02C4 */
    __IO uint32_t GPIO4A_DS2;                         /* Address Offset: 0x02C8 */
    __IO uint32_t GPIO4A_DS3;                         /* Address Offset: 0x02CC */
    __IO uint32_t GPIO4B_DS0;                         /* Address Offset: 0x02D0 */
    __IO uint32_t GPIO4B_DS1;                         /* Address Offset: 0x02D4 */
    __IO uint32_t GPIO4B_DS2;                         /* Address Offset: 0x02D8 */
    __IO uint32_t GPIO4B_DS3;                         /* Address Offset: 0x02DC */
         uint32_t RESERVED02E0[8];                    /* Address Offset: 0x02E0 */
    __IO uint32_t IO_VSEL1;                           /* Address Offset: 0x0300 */
         uint32_t RESERVED0304[63];                   /* Address Offset: 0x0304 */
    __IO uint32_t MAC0_IO_CON0;                       /* Address Offset: 0x0400 */
    __IO uint32_t MAC0_IO_CON1;                       /* Address Offset: 0x0404 */
         uint32_t RESERVED0408[62];                   /* Address Offset: 0x0408 */
    __IO uint32_t VO_IO_CON;                          /* Address Offset: 0x0500 */
         uint32_t RESERVED0504[35];                   /* Address Offset: 0x0504 */
    __IO uint32_t SARADC1_CON;                        /* Address Offset: 0x0590 */
};
/* GPIO0_IOC Register Structure Define */
struct GPIO0_IOC_REG {
    __IO uint32_t GPIO0A_IOMUX_SEL_L;                 /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0A_IOMUX_SEL_H;                 /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0B_IOMUX_SEL_L;                 /* Address Offset: 0x0008 */
    __IO uint32_t GPIO0B_IOMUX_SEL_H;                 /* Address Offset: 0x000C */
    __IO uint32_t GPIO0C_IOMUX_SEL_L;                 /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0C_IOMUX_SEL_H;                 /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0D_IOMUX_SEL_L;                 /* Address Offset: 0x0018 */
         uint32_t RESERVED001C;                       /* Address Offset: 0x001C */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0020 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0024 */
    __IO uint32_t GPIO0C_P;                           /* Address Offset: 0x0028 */
    __IO uint32_t GPIO0D_P;                           /* Address Offset: 0x002C */
    __IO uint32_t GPIO0A_IE;                          /* Address Offset: 0x0030 */
    __IO uint32_t GPIO0B_IE;                          /* Address Offset: 0x0034 */
    __IO uint32_t GPIO0C_IE;                          /* Address Offset: 0x0038 */
    __IO uint32_t GPIO0D_IE;                          /* Address Offset: 0x003C */
    __IO uint32_t GPIO0A_OD;                          /* Address Offset: 0x0040 */
    __IO uint32_t GPIO0B_OD;                          /* Address Offset: 0x0044 */
    __IO uint32_t GPIO0C_OD;                          /* Address Offset: 0x0048 */
    __IO uint32_t GPIO0D_OD;                          /* Address Offset: 0x004C */
    __IO uint32_t GPIO0A_SUS;                         /* Address Offset: 0x0050 */
    __IO uint32_t GPIO0B_SUS;                         /* Address Offset: 0x0054 */
    __IO uint32_t GPIO0C_SUS;                         /* Address Offset: 0x0058 */
    __IO uint32_t GPIO0D_SUS;                         /* Address Offset: 0x005C */
    __IO uint32_t GPIO0A_SL;                          /* Address Offset: 0x0060 */
    __IO uint32_t GPIO0B_SL;                          /* Address Offset: 0x0064 */
    __IO uint32_t GPIO0C_SL;                          /* Address Offset: 0x0068 */
    __IO uint32_t GPIO0D_SL;                          /* Address Offset: 0x006C */
    __IO uint32_t GPIO0A_DS0;                         /* Address Offset: 0x0070 */
    __IO uint32_t GPIO0A_DS1;                         /* Address Offset: 0x0074 */
    __IO uint32_t GPIO0A_DS2;                         /* Address Offset: 0x0078 */
    __IO uint32_t GPIO0A_DS3;                         /* Address Offset: 0x007C */
    __IO uint32_t GPIO0B_DS0;                         /* Address Offset: 0x0080 */
    __IO uint32_t GPIO0B_DS1;                         /* Address Offset: 0x0084 */
    __IO uint32_t GPIO0B_DS2;                         /* Address Offset: 0x0088 */
    __IO uint32_t GPIO0B_DS3;                         /* Address Offset: 0x008C */
    __IO uint32_t GPIO0C_DS0;                         /* Address Offset: 0x0090 */
    __IO uint32_t GPIO0C_DS1;                         /* Address Offset: 0x0094 */
    __IO uint32_t GPIO0C_DS2;                         /* Address Offset: 0x0098 */
    __IO uint32_t GPIO0C_DS3;                         /* Address Offset: 0x009C */
    __IO uint32_t GPIO0D_DS0;                         /* Address Offset: 0x00A0 */
         uint32_t RESERVED00A4[23];                   /* Address Offset: 0x00A4 */
    __IO uint32_t JTAG_M0_CON;                        /* Address Offset: 0x0100 */
    __IO uint32_t UART_IO_CON;                        /* Address Offset: 0x0104 */
         uint32_t RESERVED0108[16];                   /* Address Offset: 0x0108 */
    __IO uint32_t IO_VSEL2;                           /* Address Offset: 0x0148 */
    __IO uint32_t XIN_CON;                            /* Address Offset: 0x014C */
};
/* TOPCRU Register Structure Define */
struct TOPCRU_REG {
    __IO uint32_t APLL_CON[3];                        /* Address Offset: 0x0000 */
         uint32_t RESERVED000C[21];                   /* Address Offset: 0x000C */
    __IO uint32_t GPLL_CON[5];                        /* Address Offset: 0x0060 */
         uint32_t RESERVED0074[3];                    /* Address Offset: 0x0074 */
    __IO uint32_t VPLL_CON[5];                        /* Address Offset: 0x0080 */
         uint32_t RESERVED0094[3];                    /* Address Offset: 0x0094 */
    __IO uint32_t HPLL_CON[5];                        /* Address Offset: 0x00A0 */
         uint32_t RESERVED00B4[19];                   /* Address Offset: 0x00B4 */
    __IO uint32_t CLKSEL_CON[48];                     /* Address Offset: 0x0100 */
         uint32_t RESERVED01C0[80];                   /* Address Offset: 0x01C0 */
    __IO uint32_t GATE_CON[28];                       /* Address Offset: 0x0300 */
         uint32_t RESERVED0370[36];                   /* Address Offset: 0x0370 */
    __IO uint32_t SOFTRST_CON[28];                    /* Address Offset: 0x0400 */
         uint32_t RESERVED0470[36];                   /* Address Offset: 0x0470 */
    __IO uint32_t SSGTBL0_3;                          /* Address Offset: 0x0500 */
    __IO uint32_t SSGTBL4_7;                          /* Address Offset: 0x0504 */
    __IO uint32_t SSGTBL8_11;                         /* Address Offset: 0x0508 */
    __IO uint32_t SSGTBL12_15;                        /* Address Offset: 0x050C */
    __IO uint32_t SSGTBL16_19;                        /* Address Offset: 0x0510 */
    __IO uint32_t SSGTBL20_23;                        /* Address Offset: 0x0514 */
    __IO uint32_t SSGTBL24_27;                        /* Address Offset: 0x0518 */
    __IO uint32_t SSGTBL28_31;                        /* Address Offset: 0x051C */
    __IO uint32_t SSGTBL32_35;                        /* Address Offset: 0x0520 */
    __IO uint32_t SSGTBL36_39;                        /* Address Offset: 0x0524 */
    __IO uint32_t SSGTBL40_43;                        /* Address Offset: 0x0528 */
    __IO uint32_t SSGTBL44_47;                        /* Address Offset: 0x052C */
    __IO uint32_t SSGTBL48_51;                        /* Address Offset: 0x0530 */
    __IO uint32_t SSGTBL52_55;                        /* Address Offset: 0x0534 */
    __IO uint32_t SSGTBL56_59;                        /* Address Offset: 0x0538 */
    __IO uint32_t SSGTBL60_63;                        /* Address Offset: 0x053C */
    __IO uint32_t SSGTBL64_67;                        /* Address Offset: 0x0540 */
    __IO uint32_t SSGTBL68_71;                        /* Address Offset: 0x0544 */
    __IO uint32_t SSGTBL72_75;                        /* Address Offset: 0x0548 */
    __IO uint32_t SSGTBL76_79;                        /* Address Offset: 0x054C */
    __IO uint32_t SSGTBL80_83;                        /* Address Offset: 0x0550 */
    __IO uint32_t SSGTBL84_87;                        /* Address Offset: 0x0554 */
    __IO uint32_t SSGTBL88_91;                        /* Address Offset: 0x0558 */
    __IO uint32_t SSGTBL92_95;                        /* Address Offset: 0x055C */
    __IO uint32_t SSGTBL96_99;                        /* Address Offset: 0x0560 */
    __IO uint32_t SSGTBL100_103;                      /* Address Offset: 0x0564 */
    __IO uint32_t SSGTBL104_107;                      /* Address Offset: 0x0568 */
    __IO uint32_t SSGTBL108_111;                      /* Address Offset: 0x056C */
    __IO uint32_t SSGTBL112_115;                      /* Address Offset: 0x0570 */
    __IO uint32_t SSGTBL116_119;                      /* Address Offset: 0x0574 */
    __IO uint32_t SSGTBL120_123;                      /* Address Offset: 0x0578 */
    __IO uint32_t SSGTBL124_127;                      /* Address Offset: 0x057C */
         uint32_t RESERVED0580[32];                   /* Address Offset: 0x0580 */
    __IO uint32_t MODE_CON00;                         /* Address Offset: 0x0600 */
         uint32_t RESERVED0604[3];                    /* Address Offset: 0x0604 */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x0610 */
    __IO uint32_t GLB_SRST_FST;                       /* Address Offset: 0x0614 */
    __IO uint32_t GLB_SRST_SND;                       /* Address Offset: 0x0618 */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x061C */
    __IO uint32_t GLB_RST_ST;                         /* Address Offset: 0x0620 */
    __IO uint32_t SDMMC0_CON[2];                      /* Address Offset: 0x0624 */
    __IO uint32_t SDMMC1_CON[2];                      /* Address Offset: 0x062C */
         uint32_t RESERVED0634[2];                    /* Address Offset: 0x0634 */
    __IO uint32_t EMMC_CON[1];                        /* Address Offset: 0x063C */
         uint32_t RESERVED0640[16];                   /* Address Offset: 0x0640 */
    __IO uint32_t CM0_GATEMASK;                       /* Address Offset: 0x0680 */
    __IO uint32_t MISC_CON0;                          /* Address Offset: 0x0684 */
    __IO uint32_t MISC_CON1;                          /* Address Offset: 0x0688 */
    __IO uint32_t MISC_CON2;                          /* Address Offset: 0x068C */
         uint32_t RESERVED0690;                       /* Address Offset: 0x0690 */
    __I  uint32_t FREQ_CAL;                           /* Address Offset: 0x0694 */
};
/* PMU0CRU Register Structure Define */
struct PMU0CRU_REG {
         uint32_t RESERVED0000[64];                   /* Address Offset: 0x0000 */
    __IO uint32_t CLKSEL_CON[4];                      /* Address Offset: 0x0100 */
         uint32_t RESERVED0110[28];                   /* Address Offset: 0x0110 */
    __IO uint32_t GATE_CON[3];                        /* Address Offset: 0x0180 */
         uint32_t RESERVED018C[29];                   /* Address Offset: 0x018C */
    __IO uint32_t SOFTRST_CON[3];                     /* Address Offset: 0x0200 */
         uint32_t RESERVED020C[33];                   /* Address Offset: 0x020C */
    __IO uint32_t MISC_CON0;                          /* Address Offset: 0x0290 */
    __I  uint32_t GRF_FREQ;                           /* Address Offset: 0x0294 */
    __I  uint32_t IO32K_EXIST;                        /* Address Offset: 0x0298 */
};
/* PMU1CRU Register Structure Define */
struct PMU1CRU_REG {
         uint32_t RESERVED0000[16];                   /* Address Offset: 0x0000 */
    __IO uint32_t CPLL_CON[5];                        /* Address Offset: 0x0040 */
         uint32_t RESERVED0054[43];                   /* Address Offset: 0x0054 */
    __IO uint32_t CLKSEL_CON[7];                      /* Address Offset: 0x0100 */
         uint32_t RESERVED011C[25];                   /* Address Offset: 0x011C */
    __IO uint32_t GATE_CON[4];                        /* Address Offset: 0x0180 */
         uint32_t RESERVED0190[28];                   /* Address Offset: 0x0190 */
    __IO uint32_t SOFTRST_CON[3];                     /* Address Offset: 0x0200 */
         uint32_t RESERVED020C[29];                   /* Address Offset: 0x020C */
    __IO uint32_t SSGTBL0_3;                          /* Address Offset: 0x0280 */
    __IO uint32_t SSGTBL4_7;                          /* Address Offset: 0x0284 */
    __IO uint32_t SSGTBL8_11;                         /* Address Offset: 0x0288 */
    __IO uint32_t SSGTBL12_15;                        /* Address Offset: 0x028C */
    __IO uint32_t SSGTBL16_19;                        /* Address Offset: 0x0290 */
    __IO uint32_t SSGTBL20_23;                        /* Address Offset: 0x0294 */
    __IO uint32_t SSGTBL24_27;                        /* Address Offset: 0x0298 */
    __IO uint32_t SSGTBL28_31;                        /* Address Offset: 0x029C */
    __IO uint32_t SSGTBL32_35;                        /* Address Offset: 0x02A0 */
    __IO uint32_t SSGTBL36_39;                        /* Address Offset: 0x02A4 */
    __IO uint32_t SSGTBL40_43;                        /* Address Offset: 0x02A8 */
    __IO uint32_t SSGTBL44_47;                        /* Address Offset: 0x02AC */
    __IO uint32_t SSGTBL48_51;                        /* Address Offset: 0x02B0 */
    __IO uint32_t SSGTBL52_55;                        /* Address Offset: 0x02B4 */
    __IO uint32_t SSGTBL56_59;                        /* Address Offset: 0x02B8 */
    __IO uint32_t SSGTBL60_63;                        /* Address Offset: 0x02BC */
    __IO uint32_t SSGTBL64_67;                        /* Address Offset: 0x02C0 */
    __IO uint32_t SSGTBL68_71;                        /* Address Offset: 0x02C4 */
    __IO uint32_t SSGTBL72_75;                        /* Address Offset: 0x02C8 */
    __IO uint32_t SSGTBL76_79;                        /* Address Offset: 0x02CC */
    __IO uint32_t SSGTBL80_83;                        /* Address Offset: 0x02D0 */
    __IO uint32_t SSGTBL84_87;                        /* Address Offset: 0x02D4 */
    __IO uint32_t SSGTBL88_91;                        /* Address Offset: 0x02D8 */
    __IO uint32_t SSGTBL92_95;                        /* Address Offset: 0x02DC */
    __IO uint32_t SSGTBL96_99;                        /* Address Offset: 0x02E0 */
    __IO uint32_t SSGTBL100_103;                      /* Address Offset: 0x02E4 */
    __IO uint32_t SSGTBL104_107;                      /* Address Offset: 0x02E8 */
    __IO uint32_t SSGTBL108_111;                      /* Address Offset: 0x02EC */
    __IO uint32_t SSGTBL112_115;                      /* Address Offset: 0x02F0 */
    __IO uint32_t SSGTBL116_119;                      /* Address Offset: 0x02F4 */
    __IO uint32_t SSGTBL120_123;                      /* Address Offset: 0x02F8 */
    __IO uint32_t SSGTBL124_127;                      /* Address Offset: 0x02FC */
         uint32_t RESERVED0300[32];                   /* Address Offset: 0x0300 */
    __IO uint32_t PMU1MODE_CON00;                     /* Address Offset: 0x0380 */
         uint32_t RESERVED0384[39];                   /* Address Offset: 0x0384 */
    __IO uint32_t CM0_GATMASK_CON;                    /* Address Offset: 0x0420 */
         uint32_t RESERVED0424[152];                  /* Address Offset: 0x0424 */
    __IO uint32_t MISC_CON00;                         /* Address Offset: 0x0684 */
};
/* DDRCRU Register Structure Define */
struct DDRCRU_REG {
         uint32_t RESERVED0000[64];                   /* Address Offset: 0x0000 */
    __IO uint32_t CLKSEL_CON[2];                      /* Address Offset: 0x0100 */
         uint32_t RESERVED0108[30];                   /* Address Offset: 0x0108 */
    __IO uint32_t GATE_CON[2];                        /* Address Offset: 0x0180 */
         uint32_t RESERVED0188[30];                   /* Address Offset: 0x0188 */
    __IO uint32_t SOFTRST_CON[2];                     /* Address Offset: 0x0200 */
         uint32_t RESERVED0208[287];                  /* Address Offset: 0x0208 */
    __IO uint32_t MISC_CON0;                          /* Address Offset: 0x0684 */
};
/* SUBDDRCRU Register Structure Define */
struct SUBDDRCRU_REG {
         uint32_t RESERVED0000[8];                    /* Address Offset: 0x0000 */
    __IO uint32_t DPLL_CON[5];                        /* Address Offset: 0x0020 */
         uint32_t RESERVED0034[51];                   /* Address Offset: 0x0034 */
    __IO uint32_t CLKSEL_CON[1];                      /* Address Offset: 0x0100 */
         uint32_t RESERVED0104[31];                   /* Address Offset: 0x0104 */
    __IO uint32_t GATE_CON[1];                        /* Address Offset: 0x0180 */
         uint32_t RESERVED0184[31];                   /* Address Offset: 0x0184 */
    __IO uint32_t SOFTRST_CON[1];                     /* Address Offset: 0x0200 */
         uint32_t RESERVED0204[31];                   /* Address Offset: 0x0204 */
    __IO uint32_t SSGTBL0_3;                          /* Address Offset: 0x0280 */
    __IO uint32_t SSGTBL4_7;                          /* Address Offset: 0x0284 */
    __IO uint32_t SSGTBL8_11;                         /* Address Offset: 0x0288 */
    __IO uint32_t SSGTBL12_15;                        /* Address Offset: 0x028C */
    __IO uint32_t SSGTBL16_19;                        /* Address Offset: 0x0290 */
    __IO uint32_t SSGTBL20_23;                        /* Address Offset: 0x0294 */
    __IO uint32_t SSGTBL24_27;                        /* Address Offset: 0x0298 */
    __IO uint32_t SSGTBL28_31;                        /* Address Offset: 0x029C */
    __IO uint32_t SSGTBL32_35;                        /* Address Offset: 0x02A0 */
    __IO uint32_t SSGTBL36_39;                        /* Address Offset: 0x02A4 */
    __IO uint32_t SSGTBL40_43;                        /* Address Offset: 0x02A8 */
    __IO uint32_t SSGTBL44_47;                        /* Address Offset: 0x02AC */
    __IO uint32_t SSGTBL48_51;                        /* Address Offset: 0x02B0 */
    __IO uint32_t SSGTBL52_55;                        /* Address Offset: 0x02B4 */
    __IO uint32_t SSGTBL56_59;                        /* Address Offset: 0x02B8 */
    __IO uint32_t SSGTBL60_63;                        /* Address Offset: 0x02BC */
    __IO uint32_t SSGTBL64_67;                        /* Address Offset: 0x02C0 */
    __IO uint32_t SSGTBL68_71;                        /* Address Offset: 0x02C4 */
    __IO uint32_t SSGTBL72_75;                        /* Address Offset: 0x02C8 */
    __IO uint32_t SSGTBL76_79;                        /* Address Offset: 0x02CC */
    __IO uint32_t SSGTBL80_83;                        /* Address Offset: 0x02D0 */
    __IO uint32_t SSGTBL84_87;                        /* Address Offset: 0x02D4 */
    __IO uint32_t SSGTBL88_91;                        /* Address Offset: 0x02D8 */
    __IO uint32_t SSGTBL92_95;                        /* Address Offset: 0x02DC */
    __IO uint32_t SSGTBL96_99;                        /* Address Offset: 0x02E0 */
    __IO uint32_t SSGTBL100_103;                      /* Address Offset: 0x02E4 */
    __IO uint32_t SSGTBL104_107;                      /* Address Offset: 0x02E8 */
    __IO uint32_t SSGTBL108_111;                      /* Address Offset: 0x02EC */
    __IO uint32_t SSGTBL112_115;                      /* Address Offset: 0x02F0 */
    __IO uint32_t SSGTBL116_119;                      /* Address Offset: 0x02F4 */
    __IO uint32_t SSGTBL120_123;                      /* Address Offset: 0x02F8 */
    __IO uint32_t SSGTBL124_127;                      /* Address Offset: 0x02FC */
         uint32_t RESERVED0300[32];                   /* Address Offset: 0x0300 */
    __IO uint32_t SUBDDRMODE_CON00;                   /* Address Offset: 0x0380 */
};
/* PERICRU Register Structure Define */
struct PERICRU_REG {
         uint32_t RESERVED0000[64];                   /* Address Offset: 0x0000 */
    __IO uint32_t CLKSEL_CON[48];                     /* Address Offset: 0x0100 */
         uint32_t RESERVED01C0[80];                   /* Address Offset: 0x01C0 */
    __IO uint32_t GATE_CON[18];                       /* Address Offset: 0x0300 */
         uint32_t RESERVED0348[46];                   /* Address Offset: 0x0348 */
    __IO uint32_t SOFTRST_CON[18];                    /* Address Offset: 0x0400 */
         uint32_t RESERVED0448[143];                  /* Address Offset: 0x0448 */
    __IO uint32_t MISC_CON0;                          /* Address Offset: 0x0684 */
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
    __I  uint32_t CTR;                                /* Address Offset: 0x00FC */
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
    __O  uint32_t ICR;                                /* Address Offset: 0x0038 */
    __IO uint32_t DMACR;                              /* Address Offset: 0x003C */
    __IO uint32_t DMATDLR;                            /* Address Offset: 0x0040 */
    __IO uint32_t DMARDLR;                            /* Address Offset: 0x0044 */
    __I  uint32_t VERSION;                            /* Address Offset: 0x0048 */
    __IO uint32_t TIMEOUT;                            /* Address Offset: 0x004C */
    __IO uint32_t BYPASS;                             /* Address Offset: 0x0050 */
         uint32_t RESERVED0054[235];                  /* Address Offset: 0x0054 */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0400 */
         uint32_t RESERVED0404[255];                  /* Address Offset: 0x0404 */
    __I  uint32_t RXDR;                               /* Address Offset: 0x0800 */
};
/* PWM Register Structure Define */
struct PWM_CHANNEL {
    __I  uint32_t CNT;
    __IO uint32_t PERIOD_HPR;
    __IO uint32_t DUTY_LPR;
    __IO uint32_t CTRL;
};
struct PWM_REG {
         struct PWM_CHANNEL CHANNELS[4];              /* Address Offset: 0x0000 */
    __IO uint32_t INTSTS;                             /* Address Offset: 0x0040 */
    __IO uint32_t INT_EN;                             /* Address Offset: 0x0044 */
         uint32_t RESERVED0048[2];                    /* Address Offset: 0x0048 */
    __IO uint32_t FIFO_CTRL;                          /* Address Offset: 0x0050 */
    __IO uint32_t FIFO_INTSTS;                        /* Address Offset: 0x0054 */
    __IO uint32_t FIFO_TOUTTHR;                       /* Address Offset: 0x0058 */
    __I  uint32_t VERSION_ID;                         /* Address Offset: 0x005C */
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
         uint32_t RESERVED00D4[3];                    /* Address Offset: 0x00D4 */
    __IO uint32_t OFFSET[4];                          /* Address Offset: 0x00E0 */
         uint32_t RESERVED00F0[4];                    /* Address Offset: 0x00F0 */
    __IO uint32_t IR_TRANS_CTRL;                      /* Address Offset: 0x0100 */
    __IO uint32_t IR_TRANS_PRE;                       /* Address Offset: 0x0104 */
    __IO uint32_t IR_TRANS_SPRE;                      /* Address Offset: 0x0108 */
    __IO uint32_t IR_TRANS_LD;                        /* Address Offset: 0x010C */
    __IO uint32_t IR_TRANS_HD;                        /* Address Offset: 0x0110 */
    __IO uint32_t IR_TRANS_BURST_FRAME;               /* Address Offset: 0x0114 */
    __IO uint32_t IR_TRANS_DATA_VALUE;                /* Address Offset: 0x0118 */
         uint32_t RESERVED011C;                       /* Address Offset: 0x011C */
    __IO uint32_t CAPTURE_CNT_EN[4];                  /* Address Offset: 0x0120 */
    __IO uint32_t CAPTURE_POS_CNT[4];                 /* Address Offset: 0x0130 */
    __IO uint32_t CAPTURE_NEG_CNT[4];                 /* Address Offset: 0x0140 */
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
         uint32_t RESERVED007C[33];                   /* Address Offset: 0x007C */
    __IO uint32_t GPIO_REG_GROUP_L;                   /* Address Offset: 0x0100 */
    __IO uint32_t GPIO_REG_GROUP_H;                   /* Address Offset: 0x0104 */
    __IO uint32_t GPIO_VIRTUAL_EN;                    /* Address Offset: 0x0108 */
};
/* SARADC Register Structure Define */
struct SARADC_REG {
    __IO uint32_t CONV_CON;                           /* Address Offset: 0x0000 */
    __IO uint32_t T_PD_SOC;                           /* Address Offset: 0x0004 */
    __IO uint32_t T_AS_SOC;                           /* Address Offset: 0x0008 */
    __IO uint32_t T_DAS_SOC;                          /* Address Offset: 0x000C */
    __IO uint32_t T_SEL_SOC;                          /* Address Offset: 0x0010 */
    __IO uint32_t HIGH_COMP[16];                      /* Address Offset: 0x0014 */
    __IO uint32_t LOW_COMP[16];                       /* Address Offset: 0x0054 */
    __IO uint32_t DEBOUNCE;                           /* Address Offset: 0x0094 */
    __IO uint32_t HT_INT_EN;                          /* Address Offset: 0x0098 */
    __IO uint32_t LT_INT_EN;                          /* Address Offset: 0x009C */
         uint32_t RESERVED00A0[24];                   /* Address Offset: 0x00A0 */
    __IO uint32_t MT_INT_EN;                          /* Address Offset: 0x0100 */
    __IO uint32_t END_INT_EN;                         /* Address Offset: 0x0104 */
    __IO uint32_t ST_CON;                             /* Address Offset: 0x0108 */
    __I  uint32_t STATUS;                             /* Address Offset: 0x010C */
    __IO uint32_t END_INT_ST;                         /* Address Offset: 0x0110 */
    __IO uint32_t HT_INT_ST;                          /* Address Offset: 0x0114 */
    __IO uint32_t LT_INT_ST;                          /* Address Offset: 0x0118 */
    __IO uint32_t MT_INT_ST;                          /* Address Offset: 0x011C */
    __I  uint32_t DATA[16];                           /* Address Offset: 0x0120 */
    __IO uint32_t AUTO_CH_EN;                         /* Address Offset: 0x0160 */
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
/* DCACHE Register Structure Define */
struct DCACHE_REG {
    __IO uint32_t CACHE_CTRL;                         /* Address Offset: 0x0000 */
    __IO uint32_t CACHE_MAINTAIN[2];                  /* Address Offset: 0x0004 */
    __IO uint32_t STB_TIMEOUT_CTRL;                   /* Address Offset: 0x000C */
    __IO uint32_t RAM_DEBUG;                          /* Address Offset: 0x0010 */
         uint32_t RESERVED0014[3];                    /* Address Offset: 0x0014 */
    __IO uint32_t CACHE_INT_EN;                       /* Address Offset: 0x0020 */
    __IO uint32_t CACHE_INT_ST;                       /* Address Offset: 0x0024 */
    __I  uint32_t CACHE_ERR_HADDR;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED002C;                       /* Address Offset: 0x002C */
    __I  uint32_t CACHE_STATUS;                       /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[3];                    /* Address Offset: 0x0034 */
    __I  uint32_t PMU_RD_NUM_CNT;                     /* Address Offset: 0x0040 */
    __I  uint32_t PMU_WR_NUM_CNT;                     /* Address Offset: 0x0044 */
    __I  uint32_t PMU_SRAM_RD_HIT_CNT;                /* Address Offset: 0x0048 */
    __I  uint32_t PMU_HB_RD_HIT_CNT;                  /* Address Offset: 0x004C */
    __I  uint32_t PMU_STB_RD_HIT_CNT;                 /* Address Offset: 0x0050 */
    __I  uint32_t PMU_RD_HIT_CNT;                     /* Address Offset: 0x0054 */
    __I  uint32_t PMU_WR_HIT_CNT;                     /* Address Offset: 0x0058 */
    __I  uint32_t PMU_RD_MISS_PENALTY_CNT;            /* Address Offset: 0x005C */
    __I  uint32_t PMU_WR_MISS_PENALTY_CNT;            /* Address Offset: 0x0060 */
    __I  uint32_t PMU_RD_LAT_CNT;                     /* Address Offset: 0x0064 */
    __I  uint32_t PMU_WR_LAT_CNT;                     /* Address Offset: 0x0068 */
         uint32_t RESERVED006C[33];                   /* Address Offset: 0x006C */
    __I  uint32_t REVISION;                           /* Address Offset: 0x00F0 */
};
/* ICACHE Register Structure Define */
struct ICACHE_REG {
    __IO uint32_t CACHE_CTRL;                         /* Address Offset: 0x0000 */
    __IO uint32_t CACHE_MAINTAIN[2];                  /* Address Offset: 0x0004 */
    __IO uint32_t STB_TIMEOUT_CTRL;                   /* Address Offset: 0x000C */
    __IO uint32_t RAM_DEBUG;                          /* Address Offset: 0x0010 */
         uint32_t RESERVED0014[3];                    /* Address Offset: 0x0014 */
    __IO uint32_t CACHE_INT_EN;                       /* Address Offset: 0x0020 */
    __IO uint32_t CACHE_INT_ST;                       /* Address Offset: 0x0024 */
    __I  uint32_t CACHE_ERR_HADDR;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED002C;                       /* Address Offset: 0x002C */
    __I  uint32_t CACHE_STATUS;                       /* Address Offset: 0x0030 */
         uint32_t RESERVED0034[3];                    /* Address Offset: 0x0034 */
    __I  uint32_t PMU_RD_NUM_CNT;                     /* Address Offset: 0x0040 */
    __I  uint32_t PMU_WR_NUM_CNT;                     /* Address Offset: 0x0044 */
    __I  uint32_t PMU_SRAM_RD_HIT_CNT;                /* Address Offset: 0x0048 */
    __I  uint32_t PMU_HB_RD_HIT_CNT;                  /* Address Offset: 0x004C */
    __I  uint32_t PMU_STB_RD_HIT_CNT;                 /* Address Offset: 0x0050 */
    __I  uint32_t PMU_RD_HIT_CNT;                     /* Address Offset: 0x0054 */
    __I  uint32_t PMU_WR_HIT_CNT;                     /* Address Offset: 0x0058 */
    __I  uint32_t PMU_RD_MISS_PENALTY_CNT;            /* Address Offset: 0x005C */
    __I  uint32_t PMU_WR_MISS_PENALTY_CNT;            /* Address Offset: 0x0060 */
    __I  uint32_t PMU_RD_LAT_CNT;                     /* Address Offset: 0x0064 */
    __I  uint32_t PMU_WR_LAT_CNT;                     /* Address Offset: 0x0068 */
         uint32_t RESERVED006C[33];                   /* Address Offset: 0x006C */
    __I  uint32_t REVISION;                           /* Address Offset: 0x00F0 */
};
/* TIMER Register Structure Define */
struct TIMER_REG {
    __IO uint32_t LOAD_COUNT[2];                      /* Address Offset: 0x0000 */
    __I  uint32_t CURRENT_VALUE[2];                   /* Address Offset: 0x0008 */
    __IO uint32_t CONTROLREG;                         /* Address Offset: 0x0010 */
         uint32_t RESERVED0014;                       /* Address Offset: 0x0014 */
    __IO uint32_t INTSTATUS;                          /* Address Offset: 0x0018 */
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
/* MBOX Register Structure Define */
struct MBOX_CMD_DAT {
    __IO uint32_t CMD;
    __IO uint32_t DATA;
};
struct MBOX_REG {
    __IO uint32_t A2B_INTEN;                          /* Address Offset: 0x0000 */
    __IO uint32_t A2B_STATUS;                         /* Address Offset: 0x0004 */
         struct MBOX_CMD_DAT A2B[4];                  /* Address Offset: 0x0008 */
    __IO uint32_t B2A_INTEN;                          /* Address Offset: 0x0028 */
    __IO uint32_t B2A_STATUS;                         /* Address Offset: 0x002C */
         struct MBOX_CMD_DAT B2A[4];                  /* Address Offset: 0x0030 */
};
/* INTMUX Register Structure Define */
struct INTMUX_REG {
    __IO uint32_t INT_ENABLE_GROUP[32];               /* Address Offset: 0x0000 */
    __I  uint32_t INT_FLAG_GROUP[32];                 /* Address Offset: 0x0080 */
};
#endif /*  __ASSEMBLY__  */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define GPIO1_IOC_BASE                 0xFF060000U /* GPIO1_IOC base address */
#define GPIO2_IOC_BASE                 0xFF060000U /* GPIO2_IOC base address */
#define GPIO3_IOC_BASE                 0xFF070000U /* GPIO3_IOC base address */
#define GPIO4_IOC_BASE                 0xFF070000U /* GPIO4_IOC base address */
#define GPIO0_IOC_BASE                 0xFF080000U /* GPIO0_IOC base address */
#define TOPCRU_BASE                    0xFF100000U /* TOPCRU base address */
#define PMU0CRU_BASE                   0xFF110000U /* PMU0CRU base address */
#define PMU1CRU_BASE                   0xFF118000U /* PMU1CRU base address */
#define DDRCRU_BASE                    0xFF120000U /* DDRCRU base address */
#define SUBDDRCRU_BASE                 0xFF128000U /* SUBDDRCRU base address */
#define PERICRU_BASE                   0xFF130000U /* PERICRU base address */
#define I2C0_BASE                      0xFF200000U /* I2C0 base address */
#define UART0_BASE                     0xFF210000U /* UART0 base address */
#define SPI0_BASE                      0xFF220000U /* SPI0 base address */
#define PWM0_BASE                      0xFF230000U /* PWM0 base address */
#define GPIO0_BASE                     0xFF260000U /* GPIO0 base address */
#define GPIO0_EXP_BASE                 0xFF261000U /* GPIO0 EXP base address */
#define GPIO1_BASE                     0xFF620000U /* GPIO1 base address */
#define GPIO1_EXP_BASE                 0xFF621000U /* GPIO1 EXP base address */
#define GPIO2_BASE                     0xFF630000U /* GPIO2 base address */
#define GPIO2_EXP_BASE                 0xFF631000U /* GPIO2 EXP base address */
#define SPI1_BASE                      0xFF640000U /* SPI1 base address */
#define SPI2_BASE                      0xFF650000U /* SPI2 base address */
#define UART1_BASE                     0xFF670000U /* UART1 base address */
#define UART2_BASE                     0xFF680000U /* UART2 base address */
#define UART3_BASE                     0xFF690000U /* UART3 base address */
#define UART4_BASE                     0xFF6A0000U /* UART4 base address */
#define UART5_BASE                     0xFF6B0000U /* UART5 base address */
#define UART6_BASE                     0xFF6C0000U /* UART6 base address */
#define UART7_BASE                     0xFF6D0000U /* UART7 base address */
#define UART8_BASE                     0xFF6E0000U /* UART8 base address */
#define UART9_BASE                     0xFF6F0000U /* UART9 base address */
#define PWM1_BASE                      0xFF700000U /* PWM1 base address */
#define PWM2_BASE                      0xFF710000U /* PWM2 base address */
#define PWM3_BASE                      0xFF720000U /* PWM3 base address */
#define SARADC0_BASE                   0xFF730000U /* SARADC0 base address */
#define FSPI_BASE                      0xFF860000U /* FSPI base address */
#define DCACHE_BASE                    0xFF9F0000U /* DCACHE base address */
#define ICACHE_BASE                    0xFF9F0000U /* ICACHE base address */
#define I2C1_BASE                      0xFFA00000U /* I2C1 base address */
#define I2C2_BASE                      0xFFA10000U /* I2C2 base address */
#define I2C3_BASE                      0xFFA20000U /* I2C3 base address */
#define I2C4_BASE                      0xFFA30000U /* I2C4 base address */
#define I2C5_BASE                      0xFFA40000U /* I2C5 base address */
#define TIMER0_BASE                    0xFFA50000U /* TIMER0 base address */
#define TIMER1_BASE                    0xFFA50020U /* TIMER1 base address */
#define TIMER2_BASE                    0xFFA50040U /* TIMER2 base address */
#define TIMER3_BASE                    0xFFA50060U /* TIMER3 base address */
#define TIMER4_BASE                    0xFFA50080U /* TIMER4 base address */
#define TIMER5_BASE                    0xFFA500A0U /* TIMER5 base address */
#define WDT_BASE                       0xFFA60000U /* WDT base address */
#define SARADC1_BASE                   0xFFAA0000U /* SARADC1 base address */
#define GPIO3_BASE                     0xFFAC0000U /* GPIO3 base address */
#define GPIO3_EXP_BASE                 0xFFAC1000U /* GPIO3 EXP base address */
#define GPIO4_BASE                     0xFFAD0000U /* GPIO4 base address */
#define GPIO4_EXP_BASE                 0xFFAD1000U /* GPIO4 EXP base address */
#define MBOX0_BASE                     0xFFAE0000U /* MBOX0 base address */
#define INTMUX_BASE                    0xFFAF0000U /* INTMUX base address */
/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define GPIO1_IOC           ((struct GPIO1_IOC_REG *) GPIO1_IOC_BASE)
#define GPIO2_IOC           ((struct GPIO2_IOC_REG *) GPIO2_IOC_BASE)
#define GPIO3_IOC           ((struct GPIO3_IOC_REG *) GPIO3_IOC_BASE)
#define GPIO4_IOC           ((struct GPIO4_IOC_REG *) GPIO4_IOC_BASE)
#define GPIO0_IOC           ((struct GPIO0_IOC_REG *) GPIO0_IOC_BASE)
#define TOPCRU              ((struct TOPCRU_REG *) TOPCRU_BASE)
#define PMU0CRU             ((struct PMU0CRU_REG *) PMU0CRU_BASE)
#define PMU1CRU             ((struct PMU1CRU_REG *) PMU1CRU_BASE)
#define DDRCRU              ((struct DDRCRU_REG *) DDRCRU_BASE)
#define SUBDDRCRU           ((struct SUBDDRCRU_REG *) SUBDDRCRU_BASE)
#define PERICRU             ((struct PERICRU_REG *) PERICRU_BASE)
#define I2C0                ((struct I2C_REG *) I2C0_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define SPI0                ((struct SPI_REG *) SPI0_BASE)
#define PWM0                ((struct PWM_REG *) PWM0_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define GPIO0_EXP           ((struct GPIO_REG *) GPIO0_EXP_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define GPIO1_EXP           ((struct GPIO_REG *) GPIO1_EXP_BASE)
#define GPIO2               ((struct GPIO_REG *) GPIO2_BASE)
#define GPIO2_EXP           ((struct GPIO_REG *) GPIO2_EXP_BASE)
#define SPI1                ((struct SPI_REG *) SPI1_BASE)
#define SPI2                ((struct SPI_REG *) SPI2_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)
#define UART3               ((struct UART_REG *) UART3_BASE)
#define UART4               ((struct UART_REG *) UART4_BASE)
#define UART5               ((struct UART_REG *) UART5_BASE)
#define UART6               ((struct UART_REG *) UART6_BASE)
#define UART7               ((struct UART_REG *) UART7_BASE)
#define UART8               ((struct UART_REG *) UART8_BASE)
#define UART9               ((struct UART_REG *) UART9_BASE)
#define PWM1                ((struct PWM_REG *) PWM1_BASE)
#define PWM2                ((struct PWM_REG *) PWM2_BASE)
#define PWM3                ((struct PWM_REG *) PWM3_BASE)
#define SARADC0             ((struct SARADC_REG *) SARADC0_BASE)
#define FSPI                ((struct FSPI_REG *) FSPI_BASE)
#define DCACHE              ((struct DCACHE_REG *) DCACHE_BASE)
#define ICACHE              ((struct ICACHE_REG *) ICACHE_BASE)
#define I2C1                ((struct I2C_REG *) I2C1_BASE)
#define I2C2                ((struct I2C_REG *) I2C2_BASE)
#define I2C3                ((struct I2C_REG *) I2C3_BASE)
#define I2C4                ((struct I2C_REG *) I2C4_BASE)
#define I2C5                ((struct I2C_REG *) I2C5_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER0_BASE)
#define TIMER1              ((struct TIMER_REG *) TIMER1_BASE)
#define TIMER2              ((struct TIMER_REG *) TIMER2_BASE)
#define TIMER3              ((struct TIMER_REG *) TIMER3_BASE)
#define TIMER4              ((struct TIMER_REG *) TIMER4_BASE)
#define TIMER5              ((struct TIMER_REG *) TIMER5_BASE)
#define WDT                 ((struct WDT_REG *) WDT_BASE)
#define SARADC1             ((struct SARADC_REG *) SARADC1_BASE)
#define GPIO3               ((struct GPIO_REG *) GPIO3_BASE)
#define GPIO3_EXP           ((struct GPIO_REG *) GPIO3_EXP_BASE)
#define GPIO4               ((struct GPIO_REG *) GPIO4_BASE)
#define GPIO4_EXP           ((struct GPIO_REG *) GPIO4_EXP_BASE)
#define MBOX0               ((struct MBOX_REG *) MBOX0_BASE)
#define INTMUX              ((struct INTMUX_REG *) INTMUX_BASE)

#define IS_GPIO1_IOC_INSTANCE(instance) ((instance) == GPIO1_IOC)
#define IS_GPIO2_IOC_INSTANCE(instance) ((instance) == GPIO2_IOC)
#define IS_GPIO3_IOC_INSTANCE(instance) ((instance) == GPIO3_IOC)
#define IS_GPIO4_IOC_INSTANCE(instance) ((instance) == GPIO4_IOC)
#define IS_GPIO0_IOC_INSTANCE(instance) ((instance) == GPIO0_IOC)
#define IS_TOPCRU_INSTANCE(instance) ((instance) == TOPCRU)
#define IS_PMU0CRU_INSTANCE(instance) ((instance) == PMU0CRU)
#define IS_PMU1CRU_INSTANCE(instance) ((instance) == PMU1CRU)
#define IS_DDRCRU_INSTANCE(instance) ((instance) == DDRCRU)
#define IS_SUBDDRCRU_INSTANCE(instance) ((instance) == SUBDDRCRU)
#define IS_PERICRU_INSTANCE(instance) ((instance) == PERICRU)
#define IS_FSPI_INSTANCE(instance) ((instance) == FSPI)
#define IS_DCACHE_INSTANCE(instance) ((instance) == DCACHE)
#define IS_ICACHE_INSTANCE(instance) ((instance) == ICACHE)
#define IS_WDT_INSTANCE(instance) ((instance) == WDT)
#define IS_INTMUX_INSTANCE(instance) ((instance) == INTMUX)
#define IS_I2C_INSTANCE(instance) (((instance) == I2C0) || ((instance) == I2C1) || ((instance) == I2C2) || ((instance) == I2C3) || ((instance) == I2C4) || ((instance) == I2C5))
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1) || ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || ((instance) == UART5) || ((instance) == UART6) || ((instance) == UART7) || ((instance) == UART8) || ((instance) == UART9))
#define IS_SPI_INSTANCE(instance) (((instance) == SPI0) || ((instance) == SPI1) || ((instance) == SPI2))
#define IS_PWM_INSTANCE(instance) (((instance) == PWM0) || ((instance) == PWM1) || ((instance) == PWM2) || ((instance) == PWM3))
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO0) || ((instance) == GPIO1) || ((instance) == GPIO2) || ((instance) == GPIO3) || ((instance) == GPIO4) || ((instance) == GPIO0_EXP) || ((instance) == GPIO1_EXP) || ((instance) == GPIO2_EXP) || ((instance) == GPIO3_EXP) || ((instance) == GPIO4_EXP))
#define IS_SARADC_INSTANCE(instance) (((instance) == SARADC0) || ((instance) == SARADC1))
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || ((instance) == TIMER2) || ((instance) == TIMER3) || ((instance) == TIMER4) || ((instance) == TIMER5))
#define IS_MBOX_INSTANCE(instance) ((instance) == MBOX0)
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/***************************************GPIO1_IOC****************************************/
/* GPIO1A_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_OFFSET                (0x0U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_L_GPIO1A3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1A_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_OFFSET                (0x4U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1A_IOMUX_SEL_H_GPIO1A7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1B_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_OFFSET                (0x8U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_L_GPIO1B3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1B_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_OFFSET                (0xCU)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1B_IOMUX_SEL_H_GPIO1B7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1C_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_OFFSET                (0x10U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_L_GPIO1C3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1C_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_OFFSET                (0x14U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1C_IOMUX_SEL_H_GPIO1C7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1D_IOMUX_SEL_L */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_OFFSET                (0x18U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_L_GPIO1D3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1D_IOMUX_SEL_H */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_OFFSET                (0x1CU)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_SHIFT     (0U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_SHIFT     (4U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_SHIFT     (8U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_SHIFT     (12U)
#define GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_MASK      (0x7U << GPIO1_IOC_GPIO1D_IOMUX_SEL_H_GPIO1D7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO1A_P */
#define GPIO1_IOC_GPIO1A_P_OFFSET                          (0x80U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A0_P_SHIFT                 (0U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A0_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A0_P_SHIFT)                 /* 0x00000003 */
#define GPIO1_IOC_GPIO1A_P_GPIO1A1_P_SHIFT                 (2U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A1_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A1_P_SHIFT)                 /* 0x0000000C */
#define GPIO1_IOC_GPIO1A_P_GPIO1A2_P_SHIFT                 (4U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A2_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A2_P_SHIFT)                 /* 0x00000030 */
#define GPIO1_IOC_GPIO1A_P_GPIO1A3_P_SHIFT                 (6U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A3_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO1_IOC_GPIO1A_P_GPIO1A4_P_SHIFT                 (8U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A4_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A4_P_SHIFT)                 /* 0x00000300 */
#define GPIO1_IOC_GPIO1A_P_GPIO1A5_P_SHIFT                 (10U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A5_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO1_IOC_GPIO1A_P_GPIO1A6_P_SHIFT                 (12U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A6_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A6_P_SHIFT)                 /* 0x00003000 */
#define GPIO1_IOC_GPIO1A_P_GPIO1A7_P_SHIFT                 (14U)
#define GPIO1_IOC_GPIO1A_P_GPIO1A7_P_MASK                  (0x3U << GPIO1_IOC_GPIO1A_P_GPIO1A7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO1B_P */
#define GPIO1_IOC_GPIO1B_P_OFFSET                          (0x84U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B0_P_SHIFT                 (0U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B0_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B0_P_SHIFT)                 /* 0x00000003 */
#define GPIO1_IOC_GPIO1B_P_GPIO1B1_P_SHIFT                 (2U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B1_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B1_P_SHIFT)                 /* 0x0000000C */
#define GPIO1_IOC_GPIO1B_P_GPIO1B2_P_SHIFT                 (4U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B2_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B2_P_SHIFT)                 /* 0x00000030 */
#define GPIO1_IOC_GPIO1B_P_GPIO1B3_P_SHIFT                 (6U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B3_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO1_IOC_GPIO1B_P_GPIO1B4_P_SHIFT                 (8U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B4_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B4_P_SHIFT)                 /* 0x00000300 */
#define GPIO1_IOC_GPIO1B_P_GPIO1B5_P_SHIFT                 (10U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B5_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO1_IOC_GPIO1B_P_GPIO1B6_P_SHIFT                 (12U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B6_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B6_P_SHIFT)                 /* 0x00003000 */
#define GPIO1_IOC_GPIO1B_P_GPIO1B7_P_SHIFT                 (14U)
#define GPIO1_IOC_GPIO1B_P_GPIO1B7_P_MASK                  (0x3U << GPIO1_IOC_GPIO1B_P_GPIO1B7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO1C_P */
#define GPIO1_IOC_GPIO1C_P_OFFSET                          (0x88U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C0_P_SHIFT                 (0U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C0_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C0_P_SHIFT)                 /* 0x00000003 */
#define GPIO1_IOC_GPIO1C_P_GPIO1C1_P_SHIFT                 (2U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C1_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C1_P_SHIFT)                 /* 0x0000000C */
#define GPIO1_IOC_GPIO1C_P_GPIO1C2_P_SHIFT                 (4U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C2_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C2_P_SHIFT)                 /* 0x00000030 */
#define GPIO1_IOC_GPIO1C_P_GPIO1C3_P_SHIFT                 (6U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C3_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO1_IOC_GPIO1C_P_GPIO1C4_P_SHIFT                 (8U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C4_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C4_P_SHIFT)                 /* 0x00000300 */
#define GPIO1_IOC_GPIO1C_P_GPIO1C5_P_SHIFT                 (10U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C5_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO1_IOC_GPIO1C_P_GPIO1C6_P_SHIFT                 (12U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C6_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C6_P_SHIFT)                 /* 0x00003000 */
#define GPIO1_IOC_GPIO1C_P_GPIO1C7_P_SHIFT                 (14U)
#define GPIO1_IOC_GPIO1C_P_GPIO1C7_P_MASK                  (0x3U << GPIO1_IOC_GPIO1C_P_GPIO1C7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO1D_P */
#define GPIO1_IOC_GPIO1D_P_OFFSET                          (0x8CU)
#define GPIO1_IOC_GPIO1D_P_GPIO1D0_P_SHIFT                 (0U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D0_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D0_P_SHIFT)                 /* 0x00000003 */
#define GPIO1_IOC_GPIO1D_P_GPIO1D1_P_SHIFT                 (2U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D1_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D1_P_SHIFT)                 /* 0x0000000C */
#define GPIO1_IOC_GPIO1D_P_GPIO1D2_P_SHIFT                 (4U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D2_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D2_P_SHIFT)                 /* 0x00000030 */
#define GPIO1_IOC_GPIO1D_P_GPIO1D3_P_SHIFT                 (6U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D3_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO1_IOC_GPIO1D_P_GPIO1D4_P_SHIFT                 (8U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D4_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D4_P_SHIFT)                 /* 0x00000300 */
#define GPIO1_IOC_GPIO1D_P_GPIO1D5_P_SHIFT                 (10U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D5_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO1_IOC_GPIO1D_P_GPIO1D6_P_SHIFT                 (12U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D6_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D6_P_SHIFT)                 /* 0x00003000 */
#define GPIO1_IOC_GPIO1D_P_GPIO1D7_P_SHIFT                 (14U)
#define GPIO1_IOC_GPIO1D_P_GPIO1D7_P_MASK                  (0x3U << GPIO1_IOC_GPIO1D_P_GPIO1D7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO1A_IE */
#define GPIO1_IOC_GPIO1A_IE_OFFSET                         (0xC0U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A0_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A0_IE_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A1_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A1_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A1_IE_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A2_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A2_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A2_IE_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A3_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A3_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A4_IE_SHIFT               (8U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A4_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A4_IE_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A5_IE_SHIFT               (10U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A5_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A6_IE_SHIFT               (12U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A6_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A6_IE_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1A_IE_GPIO1A7_IE_SHIFT               (14U)
#define GPIO1_IOC_GPIO1A_IE_GPIO1A7_IE_MASK                (0x3U << GPIO1_IOC_GPIO1A_IE_GPIO1A7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO1B_IE */
#define GPIO1_IOC_GPIO1B_IE_OFFSET                         (0xC4U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B0_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B0_IE_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B1_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B1_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B1_IE_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B2_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B2_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B2_IE_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B3_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B3_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B4_IE_SHIFT               (8U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B4_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B4_IE_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B5_IE_SHIFT               (10U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B5_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B6_IE_SHIFT               (12U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B6_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B6_IE_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1B_IE_GPIO1B7_IE_SHIFT               (14U)
#define GPIO1_IOC_GPIO1B_IE_GPIO1B7_IE_MASK                (0x3U << GPIO1_IOC_GPIO1B_IE_GPIO1B7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO1C_IE */
#define GPIO1_IOC_GPIO1C_IE_OFFSET                         (0xC8U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C0_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C0_IE_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C1_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C1_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C1_IE_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C2_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C2_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C2_IE_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C3_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C3_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C4_IE_SHIFT               (8U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C4_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C4_IE_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C5_IE_SHIFT               (10U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C5_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C6_IE_SHIFT               (12U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C6_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C6_IE_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1C_IE_GPIO1C7_IE_SHIFT               (14U)
#define GPIO1_IOC_GPIO1C_IE_GPIO1C7_IE_MASK                (0x3U << GPIO1_IOC_GPIO1C_IE_GPIO1C7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO1D_IE */
#define GPIO1_IOC_GPIO1D_IE_OFFSET                         (0xCCU)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D0_IE_SHIFT               (0U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D0_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D0_IE_SHIFT)               /* 0x00000003 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D1_IE_SHIFT               (2U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D1_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D1_IE_SHIFT)               /* 0x0000000C */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D2_IE_SHIFT               (4U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D2_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D2_IE_SHIFT)               /* 0x00000030 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D3_IE_SHIFT               (6U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D3_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D4_IE_SHIFT               (8U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D4_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D4_IE_SHIFT)               /* 0x00000300 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D5_IE_SHIFT               (10U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D5_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D6_IE_SHIFT               (12U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D6_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D6_IE_SHIFT)               /* 0x00003000 */
#define GPIO1_IOC_GPIO1D_IE_GPIO1D7_IE_SHIFT               (14U)
#define GPIO1_IOC_GPIO1D_IE_GPIO1D7_IE_MASK                (0x3U << GPIO1_IOC_GPIO1D_IE_GPIO1D7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO1A_OD */
#define GPIO1_IOC_GPIO1A_OD_OFFSET                         (0x100U)
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
#define GPIO1_IOC_GPIO1B_OD_OFFSET                         (0x104U)
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
#define GPIO1_IOC_GPIO1C_OD_OFFSET                         (0x108U)
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
#define GPIO1_IOC_GPIO1D_OD_OFFSET                         (0x10CU)
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
/* GPIO1A_SUS */
#define GPIO1_IOC_GPIO1A_SUS_OFFSET                        (0x140U)
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
#define GPIO1_IOC_GPIO1B_SUS_OFFSET                        (0x144U)
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
#define GPIO1_IOC_GPIO1C_SUS_OFFSET                        (0x148U)
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
#define GPIO1_IOC_GPIO1D_SUS_OFFSET                        (0x14CU)
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
#define GPIO1_IOC_GPIO1A_SL_OFFSET                         (0x180U)
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
#define GPIO1_IOC_GPIO1B_SL_OFFSET                         (0x184U)
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
#define GPIO1_IOC_GPIO1C_SL_OFFSET                         (0x188U)
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
#define GPIO1_IOC_GPIO1D_SL_OFFSET                         (0x18CU)
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
/* GPIO1A_DS0 */
#define GPIO1_IOC_GPIO1A_DS0_OFFSET                        (0x200U)
#define GPIO1_IOC_GPIO1A_DS0_GPIO1A0_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1A_DS0_GPIO1A0_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS0_GPIO1A0_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS0_GPIO1A1_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1A_DS0_GPIO1A1_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS0_GPIO1A1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1A_DS1 */
#define GPIO1_IOC_GPIO1A_DS1_OFFSET                        (0x204U)
#define GPIO1_IOC_GPIO1A_DS1_GPIO1A2_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1A_DS1_GPIO1A2_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS1_GPIO1A2_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS1_GPIO1A3_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1A_DS1_GPIO1A3_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS1_GPIO1A3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1A_DS2 */
#define GPIO1_IOC_GPIO1A_DS2_OFFSET                        (0x208U)
#define GPIO1_IOC_GPIO1A_DS2_GPIO1A4_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1A_DS2_GPIO1A4_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS2_GPIO1A4_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS2_GPIO1A5_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1A_DS2_GPIO1A5_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS2_GPIO1A5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1A_DS3 */
#define GPIO1_IOC_GPIO1A_DS3_OFFSET                        (0x20CU)
#define GPIO1_IOC_GPIO1A_DS3_GPIO1A6_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1A_DS3_GPIO1A6_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS3_GPIO1A6_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1A_DS3_GPIO1A7_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1A_DS3_GPIO1A7_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1A_DS3_GPIO1A7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1B_DS0 */
#define GPIO1_IOC_GPIO1B_DS0_OFFSET                        (0x210U)
#define GPIO1_IOC_GPIO1B_DS0_GPIO1B0_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1B_DS0_GPIO1B0_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS0_GPIO1B0_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS0_GPIO1B1_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1B_DS0_GPIO1B1_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS0_GPIO1B1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1B_DS1 */
#define GPIO1_IOC_GPIO1B_DS1_OFFSET                        (0x214U)
#define GPIO1_IOC_GPIO1B_DS1_GPIO1B2_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1B_DS1_GPIO1B2_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS1_GPIO1B2_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS1_GPIO1B3_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1B_DS1_GPIO1B3_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS1_GPIO1B3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1B_DS2 */
#define GPIO1_IOC_GPIO1B_DS2_OFFSET                        (0x218U)
#define GPIO1_IOC_GPIO1B_DS2_GPIO1B4_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1B_DS2_GPIO1B4_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS2_GPIO1B4_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS2_GPIO1B5_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1B_DS2_GPIO1B5_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS2_GPIO1B5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1B_DS3 */
#define GPIO1_IOC_GPIO1B_DS3_OFFSET                        (0x21CU)
#define GPIO1_IOC_GPIO1B_DS3_GPIO1B6_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1B_DS3_GPIO1B6_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS3_GPIO1B6_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1B_DS3_GPIO1B7_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1B_DS3_GPIO1B7_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1B_DS3_GPIO1B7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1C_DS0 */
#define GPIO1_IOC_GPIO1C_DS0_OFFSET                        (0x220U)
#define GPIO1_IOC_GPIO1C_DS0_GPIO1C0_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1C_DS0_GPIO1C0_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS0_GPIO1C0_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS0_GPIO1C1_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1C_DS0_GPIO1C1_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS0_GPIO1C1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1C_DS1 */
#define GPIO1_IOC_GPIO1C_DS1_OFFSET                        (0x224U)
#define GPIO1_IOC_GPIO1C_DS1_GPIO1C2_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1C_DS1_GPIO1C2_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS1_GPIO1C2_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS1_GPIO1C3_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1C_DS1_GPIO1C3_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS1_GPIO1C3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1C_DS2 */
#define GPIO1_IOC_GPIO1C_DS2_OFFSET                        (0x228U)
#define GPIO1_IOC_GPIO1C_DS2_GPIO1C4_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1C_DS2_GPIO1C4_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS2_GPIO1C4_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS2_GPIO1C5_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1C_DS2_GPIO1C5_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS2_GPIO1C5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1C_DS3 */
#define GPIO1_IOC_GPIO1C_DS3_OFFSET                        (0x22CU)
#define GPIO1_IOC_GPIO1C_DS3_GPIO1C6_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1C_DS3_GPIO1C6_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS3_GPIO1C6_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1C_DS3_GPIO1C7_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1C_DS3_GPIO1C7_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1C_DS3_GPIO1C7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1D_DS0 */
#define GPIO1_IOC_GPIO1D_DS0_OFFSET                        (0x230U)
#define GPIO1_IOC_GPIO1D_DS0_GPIO1D0_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1D_DS0_GPIO1D0_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS0_GPIO1D0_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS0_GPIO1D1_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1D_DS0_GPIO1D1_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS0_GPIO1D1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1D_DS1 */
#define GPIO1_IOC_GPIO1D_DS1_OFFSET                        (0x234U)
#define GPIO1_IOC_GPIO1D_DS1_GPIO1D2_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1D_DS1_GPIO1D2_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS1_GPIO1D2_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS1_GPIO1D3_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1D_DS1_GPIO1D3_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS1_GPIO1D3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1D_DS2 */
#define GPIO1_IOC_GPIO1D_DS2_OFFSET                        (0x238U)
#define GPIO1_IOC_GPIO1D_DS2_GPIO1D4_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1D_DS2_GPIO1D4_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS2_GPIO1D4_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS2_GPIO1D5_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1D_DS2_GPIO1D5_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS2_GPIO1D5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO1D_DS3 */
#define GPIO1_IOC_GPIO1D_DS3_OFFSET                        (0x23CU)
#define GPIO1_IOC_GPIO1D_DS3_GPIO1D6_DS_SHIFT              (0U)
#define GPIO1_IOC_GPIO1D_DS3_GPIO1D6_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS3_GPIO1D6_DS_SHIFT)             /* 0x0000003F */
#define GPIO1_IOC_GPIO1D_DS3_GPIO1D7_DS_SHIFT              (8U)
#define GPIO1_IOC_GPIO1D_DS3_GPIO1D7_DS_MASK               (0x3FU << GPIO1_IOC_GPIO1D_DS3_GPIO1D7_DS_SHIFT)             /* 0x00003F00 */
/***************************************GPIO2_IOC****************************************/
/* GPIO2A_IOMUX_SEL_L */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_OFFSET                (0x20U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_SHIFT     (0U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_MASK      (0x7U << GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_SHIFT     (4U)
#define GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_MASK      (0x7U << GPIO2_IOC_GPIO2A_IOMUX_SEL_L_GPIO2A1_SEL_SHIFT)     /* 0x00000070 */
/* GPIO2A_P */
#define GPIO2_IOC_GPIO2A_P_OFFSET                          (0x90U)
#define GPIO2_IOC_GPIO2A_P_GPIO2A0_P_SHIFT                 (0U)
#define GPIO2_IOC_GPIO2A_P_GPIO2A0_P_MASK                  (0x3U << GPIO2_IOC_GPIO2A_P_GPIO2A0_P_SHIFT)                 /* 0x00000003 */
#define GPIO2_IOC_GPIO2A_P_GPIO2A1_P_SHIFT                 (2U)
#define GPIO2_IOC_GPIO2A_P_GPIO2A1_P_MASK                  (0x3U << GPIO2_IOC_GPIO2A_P_GPIO2A1_P_SHIFT)                 /* 0x0000000C */
/* GPIO2A_IE */
#define GPIO2_IOC_GPIO2A_IE_OFFSET                         (0xD0U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A0_IE_SHIFT               (0U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A0_IE_MASK                (0x3U << GPIO2_IOC_GPIO2A_IE_GPIO2A0_IE_SHIFT)               /* 0x00000003 */
#define GPIO2_IOC_GPIO2A_IE_GPIO2A1_IE_SHIFT               (2U)
#define GPIO2_IOC_GPIO2A_IE_GPIO2A1_IE_MASK                (0x3U << GPIO2_IOC_GPIO2A_IE_GPIO2A1_IE_SHIFT)               /* 0x0000000C */
/* GPIO2A_OD */
#define GPIO2_IOC_GPIO2A_OD_OFFSET                         (0x110U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A0_OD_SHIFT               (0U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A0_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A0_OD_SHIFT)               /* 0x00000001 */
#define GPIO2_IOC_GPIO2A_OD_GPIO2A1_OD_SHIFT               (1U)
#define GPIO2_IOC_GPIO2A_OD_GPIO2A1_OD_MASK                (0x1U << GPIO2_IOC_GPIO2A_OD_GPIO2A1_OD_SHIFT)               /* 0x00000002 */
/* GPIO2A_SUS */
#define GPIO2_IOC_GPIO2A_SUS_OFFSET                        (0x150U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A0_SUS_SHIFT             (0U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A0_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A1_SUS_SHIFT             (1U)
#define GPIO2_IOC_GPIO2A_SUS_GPIO2A1_SUS_MASK              (0x1U << GPIO2_IOC_GPIO2A_SUS_GPIO2A1_SUS_SHIFT)             /* 0x00000002 */
/* GPIO2A_SL */
#define GPIO2_IOC_GPIO2A_SL_OFFSET                         (0x190U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A0_SL_SHIFT               (0U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A0_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A0_SL_SHIFT)               /* 0x00000003 */
#define GPIO2_IOC_GPIO2A_SL_GPIO2A1_SL_SHIFT               (2U)
#define GPIO2_IOC_GPIO2A_SL_GPIO2A1_SL_MASK                (0x3U << GPIO2_IOC_GPIO2A_SL_GPIO2A1_SL_SHIFT)               /* 0x0000000C */
/* GPIO2A_DS0 */
#define GPIO2_IOC_GPIO2A_DS0_OFFSET                        (0x240U)
#define GPIO2_IOC_GPIO2A_DS0_GPIO2A0_DS_SHIFT              (0U)
#define GPIO2_IOC_GPIO2A_DS0_GPIO2A0_DS_MASK               (0x3FU << GPIO2_IOC_GPIO2A_DS0_GPIO2A0_DS_SHIFT)             /* 0x0000003F */
#define GPIO2_IOC_GPIO2A_DS0_GPIO2A1_DS_SHIFT              (8U)
#define GPIO2_IOC_GPIO2A_DS0_GPIO2A1_DS_MASK               (0x3FU << GPIO2_IOC_GPIO2A_DS0_GPIO2A1_DS_SHIFT)             /* 0x00003F00 */
/* IO_VSEL0 */
#define GPIO2_IOC_IO_VSEL0_OFFSET                          (0x300U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO234_SEL_SHIFT          (0U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO234_SEL_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO234_SEL_SHIFT)          /* 0x00000001 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_SEL18_SHIFT          (1U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_SEL18_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO2_SEL18_SHIFT)          /* 0x00000002 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_SEL18_SHIFT          (2U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_SEL18_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO3_SEL18_SHIFT)          /* 0x00000004 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_SEL18_SHIFT          (3U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_SEL18_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO4_SEL18_SHIFT)          /* 0x00000008 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_SEL33_SHIFT          (4U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_SEL33_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO2_SEL33_SHIFT)          /* 0x00000010 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_SEL33_SHIFT          (5U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_SEL33_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO3_SEL33_SHIFT)          /* 0x00000020 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_SEL33_SHIFT          (6U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_SEL33_MASK           (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO4_SEL33_SHIFT)          /* 0x00000040 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_IDDQ_SHIFT           (7U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_IDDQ_MASK            (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO2_IDDQ_SHIFT)           /* 0x00000080 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_IDDQ_SHIFT           (8U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_IDDQ_MASK            (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO3_IDDQ_SHIFT)           /* 0x00000100 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_IDDQ_SHIFT           (9U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_IDDQ_MASK            (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO4_IDDQ_SHIFT)           /* 0x00000200 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_VD_SHIFT             (12U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO2_VD_MASK              (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO2_VD_SHIFT)             /* 0x00001000 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_VD_SHIFT             (13U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO3_VD_MASK              (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO3_VD_SHIFT)             /* 0x00002000 */
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_VD_SHIFT             (14U)
#define GPIO2_IOC_IO_VSEL0_POC_VCCIO4_VD_MASK              (0x1U << GPIO2_IOC_IO_VSEL0_POC_VCCIO4_VD_SHIFT)             /* 0x00004000 */
/* MAC1_IO_CON0 */
#define GPIO2_IOC_MAC1_IO_CON0_OFFSET                      (0x400U)
#define GPIO2_IOC_MAC1_IO_CON0_GMAC1_CLK_TX_DL_CFG_SHIFT   (0U)
#define GPIO2_IOC_MAC1_IO_CON0_GMAC1_CLK_TX_DL_CFG_MASK    (0xFFU << GPIO2_IOC_MAC1_IO_CON0_GMAC1_CLK_TX_DL_CFG_SHIFT)  /* 0x000000FF */
#define GPIO2_IOC_MAC1_IO_CON0_GMAC1_CLK_RX_DL_CFG_SHIFT   (8U)
#define GPIO2_IOC_MAC1_IO_CON0_GMAC1_CLK_RX_DL_CFG_MASK    (0xFFU << GPIO2_IOC_MAC1_IO_CON0_GMAC1_CLK_RX_DL_CFG_SHIFT)  /* 0x0000FF00 */
/* MAC1_IO_CON1 */
#define GPIO2_IOC_MAC1_IO_CON1_OFFSET                      (0x404U)
#define GPIO2_IOC_MAC1_IO_CON1_GMAC1_TXCLK_DLY_ENA_SHIFT   (0U)
#define GPIO2_IOC_MAC1_IO_CON1_GMAC1_TXCLK_DLY_ENA_MASK    (0x1U << GPIO2_IOC_MAC1_IO_CON1_GMAC1_TXCLK_DLY_ENA_SHIFT)   /* 0x00000001 */
#define GPIO2_IOC_MAC1_IO_CON1_GMAC1_RXCLK_DLY_ENA_SHIFT   (1U)
#define GPIO2_IOC_MAC1_IO_CON1_GMAC1_RXCLK_DLY_ENA_MASK    (0x1U << GPIO2_IOC_MAC1_IO_CON1_GMAC1_RXCLK_DLY_ENA_SHIFT)   /* 0x00000002 */
/* SDCARD0_IO_CON */
#define GPIO2_IOC_SDCARD0_IO_CON_OFFSET                    (0x500U)
#define GPIO2_IOC_SDCARD0_IO_CON_SDCARD_DECTN_DLY_SHIFT    (0U)
#define GPIO2_IOC_SDCARD0_IO_CON_SDCARD_DECTN_DLY_MASK     (0xFFFFFFFFU << GPIO2_IOC_SDCARD0_IO_CON_SDCARD_DECTN_DLY_SHIFT) /* 0xFFFFFFFF */
/* JTAG_M1_CON */
#define GPIO2_IOC_JTAG_M1_CON_OFFSET                       (0x504U)
#define GPIO2_IOC_JTAG_M1_CON_FORCE_JTAG_SHIFT             (0U)
#define GPIO2_IOC_JTAG_M1_CON_FORCE_JTAG_MASK              (0x1U << GPIO2_IOC_JTAG_M1_CON_FORCE_JTAG_SHIFT)             /* 0x00000001 */
#define GPIO2_IOC_JTAG_M1_CON_JTAG_SEL_M1_SHIFT            (1U)
#define GPIO2_IOC_JTAG_M1_CON_JTAG_SEL_M1_MASK             (0x3U << GPIO2_IOC_JTAG_M1_CON_JTAG_SEL_M1_SHIFT)            /* 0x00000006 */
/***************************************GPIO3_IOC****************************************/
/* GPIO3A_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_OFFSET                (0x40U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_L_GPIO3A3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3A_IOMUX_SEL_H */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_OFFSET                (0x44U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3A_IOMUX_SEL_H_GPIO3A7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3B_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_OFFSET                (0x48U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_L_GPIO3B3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3B_IOMUX_SEL_H */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_OFFSET                (0x4CU)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3B_IOMUX_SEL_H_GPIO3B7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3C_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_OFFSET                (0x50U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_L_GPIO3C3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3C_IOMUX_SEL_H */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_OFFSET                (0x54U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C4_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C4_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C5_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C5_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C6_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C6_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C7_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C7_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3C_IOMUX_SEL_H_GPIO3C7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3D_IOMUX_SEL_L */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_OFFSET                (0x58U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D0_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D0_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D1_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D1_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D2_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D2_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D3_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D3_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_L_GPIO3D3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3D_IOMUX_SEL_H */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_OFFSET                (0x5CU)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D4_SEL_SHIFT     (0U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D4_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D5_SEL_SHIFT     (4U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D5_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D6_SEL_SHIFT     (8U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D6_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D7_SEL_SHIFT     (12U)
#define GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D7_SEL_MASK      (0x7U << GPIO3_IOC_GPIO3D_IOMUX_SEL_H_GPIO3D7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO3A_P */
#define GPIO3_IOC_GPIO3A_P_OFFSET                          (0xA0U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A0_P_SHIFT                 (0U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A0_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A0_P_SHIFT)                 /* 0x00000003 */
#define GPIO3_IOC_GPIO3A_P_GPIO3A1_P_SHIFT                 (2U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A1_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A1_P_SHIFT)                 /* 0x0000000C */
#define GPIO3_IOC_GPIO3A_P_GPIO3A2_P_SHIFT                 (4U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A2_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A2_P_SHIFT)                 /* 0x00000030 */
#define GPIO3_IOC_GPIO3A_P_GPIO3A3_P_SHIFT                 (6U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A3_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO3_IOC_GPIO3A_P_GPIO3A4_P_SHIFT                 (8U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A4_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A4_P_SHIFT)                 /* 0x00000300 */
#define GPIO3_IOC_GPIO3A_P_GPIO3A5_P_SHIFT                 (10U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A5_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO3_IOC_GPIO3A_P_GPIO3A6_P_SHIFT                 (12U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A6_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A6_P_SHIFT)                 /* 0x00003000 */
#define GPIO3_IOC_GPIO3A_P_GPIO3A7_P_SHIFT                 (14U)
#define GPIO3_IOC_GPIO3A_P_GPIO3A7_P_MASK                  (0x3U << GPIO3_IOC_GPIO3A_P_GPIO3A7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO3B_P */
#define GPIO3_IOC_GPIO3B_P_OFFSET                          (0xA4U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B0_P_SHIFT                 (0U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B0_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B0_P_SHIFT)                 /* 0x00000003 */
#define GPIO3_IOC_GPIO3B_P_GPIO3B1_P_SHIFT                 (2U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B1_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B1_P_SHIFT)                 /* 0x0000000C */
#define GPIO3_IOC_GPIO3B_P_GPIO3B2_P_SHIFT                 (4U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B2_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B2_P_SHIFT)                 /* 0x00000030 */
#define GPIO3_IOC_GPIO3B_P_GPIO3B3_P_SHIFT                 (6U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B3_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO3_IOC_GPIO3B_P_GPIO3B4_P_SHIFT                 (8U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B4_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B4_P_SHIFT)                 /* 0x00000300 */
#define GPIO3_IOC_GPIO3B_P_GPIO3B5_P_SHIFT                 (10U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B5_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO3_IOC_GPIO3B_P_GPIO3B6_P_SHIFT                 (12U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B6_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B6_P_SHIFT)                 /* 0x00003000 */
#define GPIO3_IOC_GPIO3B_P_GPIO3B7_P_SHIFT                 (14U)
#define GPIO3_IOC_GPIO3B_P_GPIO3B7_P_MASK                  (0x3U << GPIO3_IOC_GPIO3B_P_GPIO3B7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO3C_P */
#define GPIO3_IOC_GPIO3C_P_OFFSET                          (0xA8U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C0_P_SHIFT                 (0U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C0_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C0_P_SHIFT)                 /* 0x00000003 */
#define GPIO3_IOC_GPIO3C_P_GPIO3C1_P_SHIFT                 (2U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C1_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C1_P_SHIFT)                 /* 0x0000000C */
#define GPIO3_IOC_GPIO3C_P_GPIO3C2_P_SHIFT                 (4U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C2_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C2_P_SHIFT)                 /* 0x00000030 */
#define GPIO3_IOC_GPIO3C_P_GPIO3C3_P_SHIFT                 (6U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C3_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO3_IOC_GPIO3C_P_GPIO3C4_P_SHIFT                 (8U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C4_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C4_P_SHIFT)                 /* 0x00000300 */
#define GPIO3_IOC_GPIO3C_P_GPIO3C5_P_SHIFT                 (10U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C5_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO3_IOC_GPIO3C_P_GPIO3C6_P_SHIFT                 (12U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C6_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C6_P_SHIFT)                 /* 0x00003000 */
#define GPIO3_IOC_GPIO3C_P_GPIO3C7_P_SHIFT                 (14U)
#define GPIO3_IOC_GPIO3C_P_GPIO3C7_P_MASK                  (0x3U << GPIO3_IOC_GPIO3C_P_GPIO3C7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO3D_P */
#define GPIO3_IOC_GPIO3D_P_OFFSET                          (0xACU)
#define GPIO3_IOC_GPIO3D_P_GPIO3D0_P_SHIFT                 (0U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D0_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D0_P_SHIFT)                 /* 0x00000003 */
#define GPIO3_IOC_GPIO3D_P_GPIO3D1_P_SHIFT                 (2U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D1_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D1_P_SHIFT)                 /* 0x0000000C */
#define GPIO3_IOC_GPIO3D_P_GPIO3D2_P_SHIFT                 (4U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D2_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D2_P_SHIFT)                 /* 0x00000030 */
#define GPIO3_IOC_GPIO3D_P_GPIO3D3_P_SHIFT                 (6U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D3_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO3_IOC_GPIO3D_P_GPIO3D4_P_SHIFT                 (8U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D4_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D4_P_SHIFT)                 /* 0x00000300 */
#define GPIO3_IOC_GPIO3D_P_GPIO3D5_P_SHIFT                 (10U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D5_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO3_IOC_GPIO3D_P_GPIO3D6_P_SHIFT                 (12U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D6_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D6_P_SHIFT)                 /* 0x00003000 */
#define GPIO3_IOC_GPIO3D_P_GPIO3D7_P_SHIFT                 (14U)
#define GPIO3_IOC_GPIO3D_P_GPIO3D7_P_MASK                  (0x3U << GPIO3_IOC_GPIO3D_P_GPIO3D7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO3A_IE */
#define GPIO3_IOC_GPIO3A_IE_OFFSET                         (0xE0U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A0_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A0_IE_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A1_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A1_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A1_IE_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A2_IE_SHIFT               (4U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A2_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A2_IE_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A3_IE_SHIFT               (6U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A3_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A4_IE_SHIFT               (8U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A4_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A4_IE_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A5_IE_SHIFT               (10U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A5_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A6_IE_SHIFT               (12U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A6_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A6_IE_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3A_IE_GPIO3A7_IE_SHIFT               (14U)
#define GPIO3_IOC_GPIO3A_IE_GPIO3A7_IE_MASK                (0x3U << GPIO3_IOC_GPIO3A_IE_GPIO3A7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO3B_IE */
#define GPIO3_IOC_GPIO3B_IE_OFFSET                         (0xE4U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B0_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B0_IE_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B1_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B1_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B1_IE_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B2_IE_SHIFT               (4U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B2_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B2_IE_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B3_IE_SHIFT               (6U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B3_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B4_IE_SHIFT               (8U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B4_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B4_IE_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B5_IE_SHIFT               (10U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B5_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B6_IE_SHIFT               (12U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B6_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B6_IE_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3B_IE_GPIO3B7_IE_SHIFT               (14U)
#define GPIO3_IOC_GPIO3B_IE_GPIO3B7_IE_MASK                (0x3U << GPIO3_IOC_GPIO3B_IE_GPIO3B7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO3C_IE */
#define GPIO3_IOC_GPIO3C_IE_OFFSET                         (0xE8U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C0_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C0_IE_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C1_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C1_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C1_IE_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C2_IE_SHIFT               (4U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C2_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C2_IE_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C3_IE_SHIFT               (6U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C3_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C4_IE_SHIFT               (8U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C4_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C4_IE_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C5_IE_SHIFT               (10U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C5_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C6_IE_SHIFT               (12U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C6_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C6_IE_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3C_IE_GPIO3C7_IE_SHIFT               (14U)
#define GPIO3_IOC_GPIO3C_IE_GPIO3C7_IE_MASK                (0x3U << GPIO3_IOC_GPIO3C_IE_GPIO3C7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO3D_IE */
#define GPIO3_IOC_GPIO3D_IE_OFFSET                         (0xECU)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D0_IE_SHIFT               (0U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D0_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D0_IE_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D1_IE_SHIFT               (2U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D1_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D1_IE_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D2_IE_SHIFT               (4U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D2_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D2_IE_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D3_IE_SHIFT               (6U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D3_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D4_IE_SHIFT               (8U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D4_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D4_IE_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D5_IE_SHIFT               (10U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D5_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D6_IE_SHIFT               (12U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D6_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D6_IE_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3D_IE_GPIO3D7_IE_SHIFT               (14U)
#define GPIO3_IOC_GPIO3D_IE_GPIO3D7_IE_MASK                (0x3U << GPIO3_IOC_GPIO3D_IE_GPIO3D7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO3A_OD */
#define GPIO3_IOC_GPIO3A_OD_OFFSET                         (0x120U)
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
#define GPIO3_IOC_GPIO3B_OD_OFFSET                         (0x124U)
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
#define GPIO3_IOC_GPIO3C_OD_OFFSET                         (0x128U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C0_OD_SHIFT               (0U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C0_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C0_OD_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C1_OD_SHIFT               (1U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C1_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C1_OD_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C2_OD_SHIFT               (2U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C2_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C2_OD_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C3_OD_SHIFT               (3U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C3_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C3_OD_SHIFT)               /* 0x00000008 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C4_OD_SHIFT               (4U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C4_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C4_OD_SHIFT)               /* 0x00000010 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C5_OD_SHIFT               (5U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C5_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C5_OD_SHIFT)               /* 0x00000020 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C6_OD_SHIFT               (6U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C6_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C6_OD_SHIFT)               /* 0x00000040 */
#define GPIO3_IOC_GPIO3C_OD_GPIO3C7_OD_SHIFT               (7U)
#define GPIO3_IOC_GPIO3C_OD_GPIO3C7_OD_MASK                (0x1U << GPIO3_IOC_GPIO3C_OD_GPIO3C7_OD_SHIFT)               /* 0x00000080 */
/* GPIO3D_OD */
#define GPIO3_IOC_GPIO3D_OD_OFFSET                         (0x12CU)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D0_OD_SHIFT               (0U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D0_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D0_OD_SHIFT)               /* 0x00000001 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D1_OD_SHIFT               (1U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D1_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D1_OD_SHIFT)               /* 0x00000002 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D2_OD_SHIFT               (2U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D2_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D2_OD_SHIFT)               /* 0x00000004 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D3_OD_SHIFT               (3U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D3_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D3_OD_SHIFT)               /* 0x00000008 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D4_OD_SHIFT               (4U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D4_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D4_OD_SHIFT)               /* 0x00000010 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D5_OD_SHIFT               (5U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D5_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D5_OD_SHIFT)               /* 0x00000020 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D6_OD_SHIFT               (6U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D6_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D6_OD_SHIFT)               /* 0x00000040 */
#define GPIO3_IOC_GPIO3D_OD_GPIO3D7_OD_SHIFT               (7U)
#define GPIO3_IOC_GPIO3D_OD_GPIO3D7_OD_MASK                (0x1U << GPIO3_IOC_GPIO3D_OD_GPIO3D7_OD_SHIFT)               /* 0x00000080 */
/* GPIO3A_SUS */
#define GPIO3_IOC_GPIO3A_SUS_OFFSET                        (0x160U)
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
#define GPIO3_IOC_GPIO3B_SUS_OFFSET                        (0x164U)
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
#define GPIO3_IOC_GPIO3C_SUS_OFFSET                        (0x168U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C0_SUS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C0_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C1_SUS_SHIFT             (1U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C1_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C2_SUS_SHIFT             (2U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C2_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C3_SUS_SHIFT             (3U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C3_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C4_SUS_SHIFT             (4U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C4_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C5_SUS_SHIFT             (5U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C5_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C6_SUS_SHIFT             (6U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C6_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C7_SUS_SHIFT             (7U)
#define GPIO3_IOC_GPIO3C_SUS_GPIO3C7_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3C_SUS_GPIO3C7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO3D_SUS */
#define GPIO3_IOC_GPIO3D_SUS_OFFSET                        (0x16CU)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D0_SUS_SHIFT             (0U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D0_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D1_SUS_SHIFT             (1U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D1_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D2_SUS_SHIFT             (2U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D2_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D3_SUS_SHIFT             (3U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D3_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D4_SUS_SHIFT             (4U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D4_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D5_SUS_SHIFT             (5U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D5_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D6_SUS_SHIFT             (6U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D6_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D7_SUS_SHIFT             (7U)
#define GPIO3_IOC_GPIO3D_SUS_GPIO3D7_SUS_MASK              (0x1U << GPIO3_IOC_GPIO3D_SUS_GPIO3D7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO3A_SL */
#define GPIO3_IOC_GPIO3A_SL_OFFSET                         (0x1A0U)
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
#define GPIO3_IOC_GPIO3B_SL_OFFSET                         (0x1A4U)
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
#define GPIO3_IOC_GPIO3C_SL_OFFSET                         (0x1A8U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C0_SL_SHIFT               (0U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C0_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C0_SL_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C1_SL_SHIFT               (2U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C1_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C1_SL_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C2_SL_SHIFT               (4U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C2_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C2_SL_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C3_SL_SHIFT               (6U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C3_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C4_SL_SHIFT               (8U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C4_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C4_SL_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C5_SL_SHIFT               (10U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C5_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C6_SL_SHIFT               (12U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C6_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C6_SL_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3C_SL_GPIO3C7_SL_SHIFT               (14U)
#define GPIO3_IOC_GPIO3C_SL_GPIO3C7_SL_MASK                (0x3U << GPIO3_IOC_GPIO3C_SL_GPIO3C7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO3D_SL */
#define GPIO3_IOC_GPIO3D_SL_OFFSET                         (0x1ACU)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D0_SL_SHIFT               (0U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D0_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D0_SL_SHIFT)               /* 0x00000003 */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D1_SL_SHIFT               (2U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D1_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D1_SL_SHIFT)               /* 0x0000000C */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D2_SL_SHIFT               (4U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D2_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D2_SL_SHIFT)               /* 0x00000030 */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D3_SL_SHIFT               (6U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D3_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D4_SL_SHIFT               (8U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D4_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D4_SL_SHIFT)               /* 0x00000300 */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D5_SL_SHIFT               (10U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D5_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D6_SL_SHIFT               (12U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D6_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D6_SL_SHIFT)               /* 0x00003000 */
#define GPIO3_IOC_GPIO3D_SL_GPIO3D7_SL_SHIFT               (14U)
#define GPIO3_IOC_GPIO3D_SL_GPIO3D7_SL_MASK                (0x3U << GPIO3_IOC_GPIO3D_SL_GPIO3D7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO3A_DS0 */
#define GPIO3_IOC_GPIO3A_DS0_OFFSET                        (0x280U)
#define GPIO3_IOC_GPIO3A_DS0_GPIO3A0_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3A_DS0_GPIO3A0_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS0_GPIO3A0_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS0_GPIO3A1_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3A_DS0_GPIO3A1_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS0_GPIO3A1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3A_DS1 */
#define GPIO3_IOC_GPIO3A_DS1_OFFSET                        (0x284U)
#define GPIO3_IOC_GPIO3A_DS1_GPIO3A2_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3A_DS1_GPIO3A2_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS1_GPIO3A2_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS1_GPIO3A3_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3A_DS1_GPIO3A3_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS1_GPIO3A3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3A_DS2 */
#define GPIO3_IOC_GPIO3A_DS2_OFFSET                        (0x288U)
#define GPIO3_IOC_GPIO3A_DS2_GPIO3A4_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3A_DS2_GPIO3A4_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS2_GPIO3A4_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS2_GPIO3A5_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3A_DS2_GPIO3A5_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS2_GPIO3A5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3A_DS3 */
#define GPIO3_IOC_GPIO3A_DS3_OFFSET                        (0x28CU)
#define GPIO3_IOC_GPIO3A_DS3_GPIO3A6_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3A_DS3_GPIO3A6_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS3_GPIO3A6_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3A_DS3_GPIO3A7_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3A_DS3_GPIO3A7_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3A_DS3_GPIO3A7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3B_DS0 */
#define GPIO3_IOC_GPIO3B_DS0_OFFSET                        (0x290U)
#define GPIO3_IOC_GPIO3B_DS0_GPIO3B0_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3B_DS0_GPIO3B0_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS0_GPIO3B0_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS0_GPIO3B1_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3B_DS0_GPIO3B1_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS0_GPIO3B1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3B_DS1 */
#define GPIO3_IOC_GPIO3B_DS1_OFFSET                        (0x294U)
#define GPIO3_IOC_GPIO3B_DS1_GPIO3B2_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3B_DS1_GPIO3B2_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS1_GPIO3B2_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS1_GPIO3B3_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3B_DS1_GPIO3B3_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS1_GPIO3B3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3B_DS2 */
#define GPIO3_IOC_GPIO3B_DS2_OFFSET                        (0x298U)
#define GPIO3_IOC_GPIO3B_DS2_GPIO3B4_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3B_DS2_GPIO3B4_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS2_GPIO3B4_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS2_GPIO3B5_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3B_DS2_GPIO3B5_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS2_GPIO3B5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3B_DS3 */
#define GPIO3_IOC_GPIO3B_DS3_OFFSET                        (0x29CU)
#define GPIO3_IOC_GPIO3B_DS3_GPIO3B6_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3B_DS3_GPIO3B6_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS3_GPIO3B6_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3B_DS3_GPIO3B7_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3B_DS3_GPIO3B7_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3B_DS3_GPIO3B7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3C_DS0 */
#define GPIO3_IOC_GPIO3C_DS0_OFFSET                        (0x2A0U)
#define GPIO3_IOC_GPIO3C_DS0_GPIO3C0_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3C_DS0_GPIO3C0_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS0_GPIO3C0_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3C_DS0_GPIO3C1_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3C_DS0_GPIO3C1_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS0_GPIO3C1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3C_DS1 */
#define GPIO3_IOC_GPIO3C_DS1_OFFSET                        (0x2A4U)
#define GPIO3_IOC_GPIO3C_DS1_GPIO3C2_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3C_DS1_GPIO3C2_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS1_GPIO3C2_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3C_DS1_GPIO3C3_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3C_DS1_GPIO3C3_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS1_GPIO3C3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3C_DS2 */
#define GPIO3_IOC_GPIO3C_DS2_OFFSET                        (0x2A8U)
#define GPIO3_IOC_GPIO3C_DS2_GPIO3C4_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3C_DS2_GPIO3C4_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS2_GPIO3C4_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3C_DS2_GPIO3C5_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3C_DS2_GPIO3C5_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS2_GPIO3C5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3C_DS3 */
#define GPIO3_IOC_GPIO3C_DS3_OFFSET                        (0x2ACU)
#define GPIO3_IOC_GPIO3C_DS3_GPIO3C6_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3C_DS3_GPIO3C6_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS3_GPIO3C6_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3C_DS3_GPIO3C7_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3C_DS3_GPIO3C7_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3C_DS3_GPIO3C7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3D_DS0 */
#define GPIO3_IOC_GPIO3D_DS0_OFFSET                        (0x2B0U)
#define GPIO3_IOC_GPIO3D_DS0_GPIO3D0_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3D_DS0_GPIO3D0_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS0_GPIO3D0_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3D_DS0_GPIO3D1_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3D_DS0_GPIO3D1_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS0_GPIO3D1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3D_DS1 */
#define GPIO3_IOC_GPIO3D_DS1_OFFSET                        (0x2B4U)
#define GPIO3_IOC_GPIO3D_DS1_GPIO3D2_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3D_DS1_GPIO3D2_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS1_GPIO3D2_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3D_DS1_GPIO3D3_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3D_DS1_GPIO3D3_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS1_GPIO3D3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3D_DS2 */
#define GPIO3_IOC_GPIO3D_DS2_OFFSET                        (0x2B8U)
#define GPIO3_IOC_GPIO3D_DS2_GPIO3D4_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3D_DS2_GPIO3D4_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS2_GPIO3D4_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3D_DS2_GPIO3D5_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3D_DS2_GPIO3D5_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS2_GPIO3D5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO3D_DS3 */
#define GPIO3_IOC_GPIO3D_DS3_OFFSET                        (0x2BCU)
#define GPIO3_IOC_GPIO3D_DS3_GPIO3D6_DS_SHIFT              (0U)
#define GPIO3_IOC_GPIO3D_DS3_GPIO3D6_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS3_GPIO3D6_DS_SHIFT)             /* 0x0000003F */
#define GPIO3_IOC_GPIO3D_DS3_GPIO3D7_DS_SHIFT              (8U)
#define GPIO3_IOC_GPIO3D_DS3_GPIO3D7_DS_MASK               (0x3FU << GPIO3_IOC_GPIO3D_DS3_GPIO3D7_DS_SHIFT)             /* 0x00003F00 */
/***************************************GPIO4_IOC****************************************/
/* GPIO4A_IOMUX_SEL_L */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_OFFSET                (0x60U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_L_GPIO4A3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO4A_IOMUX_SEL_H */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_OFFSET                (0x64U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4A_IOMUX_SEL_H_GPIO4A7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO4B_IOMUX_SEL_L */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_OFFSET                (0x68U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_L_GPIO4B3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO4B_IOMUX_SEL_H */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_OFFSET                (0x6CU)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_SHIFT     (0U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_SHIFT     (4U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_SHIFT     (8U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_SHIFT     (12U)
#define GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_MASK      (0x7U << GPIO4_IOC_GPIO4B_IOMUX_SEL_H_GPIO4B7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO4A_P */
#define GPIO4_IOC_GPIO4A_P_OFFSET                          (0xB0U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A0_P_SHIFT                 (0U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A0_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A0_P_SHIFT)                 /* 0x00000003 */
#define GPIO4_IOC_GPIO4A_P_GPIO4A1_P_SHIFT                 (2U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A1_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A1_P_SHIFT)                 /* 0x0000000C */
#define GPIO4_IOC_GPIO4A_P_GPIO4A2_P_SHIFT                 (4U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A2_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A2_P_SHIFT)                 /* 0x00000030 */
#define GPIO4_IOC_GPIO4A_P_GPIO4A3_P_SHIFT                 (6U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A3_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO4_IOC_GPIO4A_P_GPIO4A4_P_SHIFT                 (8U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A4_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A4_P_SHIFT)                 /* 0x00000300 */
#define GPIO4_IOC_GPIO4A_P_GPIO4A5_P_SHIFT                 (10U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A5_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO4_IOC_GPIO4A_P_GPIO4A6_P_SHIFT                 (12U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A6_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A6_P_SHIFT)                 /* 0x00003000 */
#define GPIO4_IOC_GPIO4A_P_GPIO4A7_P_SHIFT                 (14U)
#define GPIO4_IOC_GPIO4A_P_GPIO4A7_P_MASK                  (0x3U << GPIO4_IOC_GPIO4A_P_GPIO4A7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO4B_P */
#define GPIO4_IOC_GPIO4B_P_OFFSET                          (0xB4U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B0_P_SHIFT                 (0U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B0_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B0_P_SHIFT)                 /* 0x00000003 */
#define GPIO4_IOC_GPIO4B_P_GPIO4B1_P_SHIFT                 (2U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B1_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B1_P_SHIFT)                 /* 0x0000000C */
#define GPIO4_IOC_GPIO4B_P_GPIO4B2_P_SHIFT                 (4U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B2_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B2_P_SHIFT)                 /* 0x00000030 */
#define GPIO4_IOC_GPIO4B_P_GPIO4B3_P_SHIFT                 (6U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B3_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO4_IOC_GPIO4B_P_GPIO4B4_P_SHIFT                 (8U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B4_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B4_P_SHIFT)                 /* 0x00000300 */
#define GPIO4_IOC_GPIO4B_P_GPIO4B5_P_SHIFT                 (10U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B5_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO4_IOC_GPIO4B_P_GPIO4B6_P_SHIFT                 (12U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B6_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B6_P_SHIFT)                 /* 0x00003000 */
#define GPIO4_IOC_GPIO4B_P_GPIO4B7_P_SHIFT                 (14U)
#define GPIO4_IOC_GPIO4B_P_GPIO4B7_P_MASK                  (0x3U << GPIO4_IOC_GPIO4B_P_GPIO4B7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO4A_IE */
#define GPIO4_IOC_GPIO4A_IE_OFFSET                         (0xF0U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A0_IE_SHIFT               (0U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A0_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A0_IE_SHIFT)               /* 0x00000003 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A1_IE_SHIFT               (2U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A1_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A1_IE_SHIFT)               /* 0x0000000C */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A2_IE_SHIFT               (4U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A2_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A2_IE_SHIFT)               /* 0x00000030 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A3_IE_SHIFT               (6U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A3_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A4_IE_SHIFT               (8U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A4_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A4_IE_SHIFT)               /* 0x00000300 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A5_IE_SHIFT               (10U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A5_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A6_IE_SHIFT               (12U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A6_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A6_IE_SHIFT)               /* 0x00003000 */
#define GPIO4_IOC_GPIO4A_IE_GPIO4A7_IE_SHIFT               (14U)
#define GPIO4_IOC_GPIO4A_IE_GPIO4A7_IE_MASK                (0x3U << GPIO4_IOC_GPIO4A_IE_GPIO4A7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO4B_IE */
#define GPIO4_IOC_GPIO4B_IE_OFFSET                         (0xF4U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B0_IE_SHIFT               (0U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B0_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B0_IE_SHIFT)               /* 0x00000003 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B1_IE_SHIFT               (2U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B1_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B1_IE_SHIFT)               /* 0x0000000C */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B2_IE_SHIFT               (4U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B2_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B2_IE_SHIFT)               /* 0x00000030 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B3_IE_SHIFT               (6U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B3_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B4_IE_SHIFT               (8U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B4_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B4_IE_SHIFT)               /* 0x00000300 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B5_IE_SHIFT               (10U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B5_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B6_IE_SHIFT               (12U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B6_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B6_IE_SHIFT)               /* 0x00003000 */
#define GPIO4_IOC_GPIO4B_IE_GPIO4B7_IE_SHIFT               (14U)
#define GPIO4_IOC_GPIO4B_IE_GPIO4B7_IE_MASK                (0x3U << GPIO4_IOC_GPIO4B_IE_GPIO4B7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO4A_OD */
#define GPIO4_IOC_GPIO4A_OD_OFFSET                         (0x130U)
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
#define GPIO4_IOC_GPIO4B_OD_OFFSET                         (0x134U)
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
/* GPIO4A_SUS */
#define GPIO4_IOC_GPIO4A_SUS_OFFSET                        (0x170U)
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
#define GPIO4_IOC_GPIO4B_SUS_OFFSET                        (0x174U)
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
/* GPIO4A_SL */
#define GPIO4_IOC_GPIO4A_SL_OFFSET                         (0x1B0U)
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
#define GPIO4_IOC_GPIO4B_SL_OFFSET                         (0x1B4U)
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
/* GPIO4A_DS0 */
#define GPIO4_IOC_GPIO4A_DS0_OFFSET                        (0x2C0U)
#define GPIO4_IOC_GPIO4A_DS0_GPIO4A0_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4A_DS0_GPIO4A0_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS0_GPIO4A0_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS0_GPIO4A1_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4A_DS0_GPIO4A1_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS0_GPIO4A1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4A_DS1 */
#define GPIO4_IOC_GPIO4A_DS1_OFFSET                        (0x2C4U)
#define GPIO4_IOC_GPIO4A_DS1_GPIO4A2_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4A_DS1_GPIO4A2_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS1_GPIO4A2_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS1_GPIO4A3_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4A_DS1_GPIO4A3_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS1_GPIO4A3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4A_DS2 */
#define GPIO4_IOC_GPIO4A_DS2_OFFSET                        (0x2C8U)
#define GPIO4_IOC_GPIO4A_DS2_GPIO4A4_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4A_DS2_GPIO4A4_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS2_GPIO4A4_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS2_GPIO4A5_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4A_DS2_GPIO4A5_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS2_GPIO4A5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4A_DS3 */
#define GPIO4_IOC_GPIO4A_DS3_OFFSET                        (0x2CCU)
#define GPIO4_IOC_GPIO4A_DS3_GPIO4A6_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4A_DS3_GPIO4A6_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS3_GPIO4A6_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4A_DS3_GPIO4A7_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4A_DS3_GPIO4A7_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4A_DS3_GPIO4A7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4B_DS0 */
#define GPIO4_IOC_GPIO4B_DS0_OFFSET                        (0x2D0U)
#define GPIO4_IOC_GPIO4B_DS0_GPIO4B0_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4B_DS0_GPIO4B0_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS0_GPIO4B0_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS0_GPIO4B1_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4B_DS0_GPIO4B1_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS0_GPIO4B1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4B_DS1 */
#define GPIO4_IOC_GPIO4B_DS1_OFFSET                        (0x2D4U)
#define GPIO4_IOC_GPIO4B_DS1_GPIO4B2_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4B_DS1_GPIO4B2_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS1_GPIO4B2_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS1_GPIO4B3_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4B_DS1_GPIO4B3_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS1_GPIO4B3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4B_DS2 */
#define GPIO4_IOC_GPIO4B_DS2_OFFSET                        (0x2D8U)
#define GPIO4_IOC_GPIO4B_DS2_GPIO4B4_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4B_DS2_GPIO4B4_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS2_GPIO4B4_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS2_GPIO4B5_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4B_DS2_GPIO4B5_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS2_GPIO4B5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO4B_DS3 */
#define GPIO4_IOC_GPIO4B_DS3_OFFSET                        (0x2DCU)
#define GPIO4_IOC_GPIO4B_DS3_GPIO4B6_DS_SHIFT              (0U)
#define GPIO4_IOC_GPIO4B_DS3_GPIO4B6_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS3_GPIO4B6_DS_SHIFT)             /* 0x0000003F */
#define GPIO4_IOC_GPIO4B_DS3_GPIO4B7_DS_SHIFT              (8U)
#define GPIO4_IOC_GPIO4B_DS3_GPIO4B7_DS_MASK               (0x3FU << GPIO4_IOC_GPIO4B_DS3_GPIO4B7_DS_SHIFT)             /* 0x00003F00 */
/* IO_VSEL1 */
#define GPIO4_IOC_IO_VSEL1_OFFSET                          (0x300U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO156_SEL_SHIFT          (0U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO156_SEL_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO156_SEL_SHIFT)          /* 0x00000001 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_SEL18_SHIFT          (1U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_SEL18_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO1_SEL18_SHIFT)          /* 0x00000002 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_SEL18_SHIFT          (2U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_SEL18_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO5_SEL18_SHIFT)          /* 0x00000004 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_SEL18_SHIFT          (3U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_SEL18_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO6_SEL18_SHIFT)          /* 0x00000008 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_SEL33_SHIFT          (4U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_SEL33_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO1_SEL33_SHIFT)          /* 0x00000010 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_SEL33_SHIFT          (5U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_SEL33_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO5_SEL33_SHIFT)          /* 0x00000020 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_SEL33_SHIFT          (6U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_SEL33_MASK           (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO6_SEL33_SHIFT)          /* 0x00000040 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_IDDQ_SHIFT           (7U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_IDDQ_MASK            (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO1_IDDQ_SHIFT)           /* 0x00000080 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_IDDQ_SHIFT           (8U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_IDDQ_MASK            (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO5_IDDQ_SHIFT)           /* 0x00000100 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_IDDQ_SHIFT           (9U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_IDDQ_MASK            (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO6_IDDQ_SHIFT)           /* 0x00000200 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_VD_SHIFT             (12U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO1_VD_MASK              (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO1_VD_SHIFT)             /* 0x00001000 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_VD_SHIFT             (13U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO5_VD_MASK              (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO5_VD_SHIFT)             /* 0x00002000 */
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_VD_SHIFT             (14U)
#define GPIO4_IOC_IO_VSEL1_POC_VCCIO6_VD_MASK              (0x1U << GPIO4_IOC_IO_VSEL1_POC_VCCIO6_VD_SHIFT)             /* 0x00004000 */
/* MAC0_IO_CON0 */
#define GPIO4_IOC_MAC0_IO_CON0_OFFSET                      (0x400U)
#define GPIO4_IOC_MAC0_IO_CON0_GMAC0_CLK_TX_DL_CFG_SHIFT   (0U)
#define GPIO4_IOC_MAC0_IO_CON0_GMAC0_CLK_TX_DL_CFG_MASK    (0xFFU << GPIO4_IOC_MAC0_IO_CON0_GMAC0_CLK_TX_DL_CFG_SHIFT)  /* 0x000000FF */
#define GPIO4_IOC_MAC0_IO_CON0_GMAC0_CLK_RX_DL_CFG_SHIFT   (8U)
#define GPIO4_IOC_MAC0_IO_CON0_GMAC0_CLK_RX_DL_CFG_MASK    (0xFFU << GPIO4_IOC_MAC0_IO_CON0_GMAC0_CLK_RX_DL_CFG_SHIFT)  /* 0x0000FF00 */
/* MAC0_IO_CON1 */
#define GPIO4_IOC_MAC0_IO_CON1_OFFSET                      (0x404U)
#define GPIO4_IOC_MAC0_IO_CON1_GMAC0_TXCLK_DLY_ENA_SHIFT   (0U)
#define GPIO4_IOC_MAC0_IO_CON1_GMAC0_TXCLK_DLY_ENA_MASK    (0x1U << GPIO4_IOC_MAC0_IO_CON1_GMAC0_TXCLK_DLY_ENA_SHIFT)   /* 0x00000001 */
#define GPIO4_IOC_MAC0_IO_CON1_GMAC0_RXCLK_DLY_ENA_SHIFT   (1U)
#define GPIO4_IOC_MAC0_IO_CON1_GMAC0_RXCLK_DLY_ENA_MASK    (0x1U << GPIO4_IOC_MAC0_IO_CON1_GMAC0_RXCLK_DLY_ENA_SHIFT)   /* 0x00000002 */
/* VO_IO_CON */
#define GPIO4_IOC_VO_IO_CON_OFFSET                         (0x500U)
#define GPIO4_IOC_VO_IO_CON_RGB_DCLK_INV_SEL_SHIFT         (3U)
#define GPIO4_IOC_VO_IO_CON_RGB_DCLK_INV_SEL_MASK          (0x1U << GPIO4_IOC_VO_IO_CON_RGB_DCLK_INV_SEL_SHIFT)         /* 0x00000008 */
#define GPIO4_IOC_VO_IO_CON_RGB_BYPASS_SHIFT               (6U)
#define GPIO4_IOC_VO_IO_CON_RGB_BYPASS_MASK                (0x1U << GPIO4_IOC_VO_IO_CON_RGB_BYPASS_SHIFT)               /* 0x00000040 */
#define GPIO4_IOC_VO_IO_CON_RGB_DCLK_DELAY_NUM_SHIFT       (8U)
#define GPIO4_IOC_VO_IO_CON_RGB_DCLK_DELAY_NUM_MASK        (0xFFU << GPIO4_IOC_VO_IO_CON_RGB_DCLK_DELAY_NUM_SHIFT)      /* 0x0000FF00 */
/* SARADC1_CON */
#define GPIO4_IOC_SARADC1_CON_OFFSET                       (0x590U)
#define GPIO4_IOC_SARADC1_CON_SARADC_ANA_REG_SHIFT         (0U)
#define GPIO4_IOC_SARADC1_CON_SARADC_ANA_REG_MASK          (0xFFFFU << GPIO4_IOC_SARADC1_CON_SARADC_ANA_REG_SHIFT)      /* 0x0000FFFF */
/***************************************GPIO0_IOC****************************************/
/* GPIO0A_IOMUX_SEL_L */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_OFFSET                (0x0U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_L_GPIO0A3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO0A_IOMUX_SEL_H */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_OFFSET                (0x4U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A6_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A6_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A7_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A7_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0A_IOMUX_SEL_H_GPIO0A7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO0B_IOMUX_SEL_L */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_OFFSET                (0x8U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B0_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B0_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B1_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B1_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B2_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B2_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B3_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B3_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_L_GPIO0B3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO0B_IOMUX_SEL_H */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_OFFSET                (0xCU)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B4_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B4_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0B_IOMUX_SEL_H_GPIO0B7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO0C_IOMUX_SEL_L */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_OFFSET                (0x10U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C1_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C1_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C1_SEL_SHIFT)     /* 0x00000070 */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C2_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C2_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C2_SEL_SHIFT)     /* 0x00000700 */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C3_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C3_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_L_GPIO0C3_SEL_SHIFT)     /* 0x00007000 */
/* GPIO0C_IOMUX_SEL_H */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_OFFSET                (0x14U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C4_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C5_SEL_SHIFT)     /* 0x00000070 */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_SHIFT     (8U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C6_SEL_SHIFT)     /* 0x00000700 */
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_SHIFT     (12U)
#define GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0C_IOMUX_SEL_H_GPIO0C7_SEL_SHIFT)     /* 0x00007000 */
/* GPIO0D_IOMUX_SEL_L */
#define GPIO0_IOC_GPIO0D_IOMUX_SEL_L_OFFSET                (0x18U)
#define GPIO0_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_SHIFT     (0U)
#define GPIO0_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D0_SEL_SHIFT)     /* 0x00000007 */
#define GPIO0_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_SHIFT     (4U)
#define GPIO0_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_MASK      (0x7U << GPIO0_IOC_GPIO0D_IOMUX_SEL_L_GPIO0D1_SEL_SHIFT)     /* 0x00000070 */
/* GPIO0A_P */
#define GPIO0_IOC_GPIO0A_P_OFFSET                          (0x20U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A0_P_SHIFT                 (0U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A0_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A0_P_SHIFT)                 /* 0x00000003 */
#define GPIO0_IOC_GPIO0A_P_GPIO0A1_P_SHIFT                 (2U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A1_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A1_P_SHIFT)                 /* 0x0000000C */
#define GPIO0_IOC_GPIO0A_P_GPIO0A2_P_SHIFT                 (4U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A2_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A2_P_SHIFT)                 /* 0x00000030 */
#define GPIO0_IOC_GPIO0A_P_GPIO0A3_P_SHIFT                 (6U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A3_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO0_IOC_GPIO0A_P_GPIO0A4_P_SHIFT                 (8U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A4_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A4_P_SHIFT)                 /* 0x00000300 */
#define GPIO0_IOC_GPIO0A_P_GPIO0A5_P_SHIFT                 (10U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A5_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO0_IOC_GPIO0A_P_GPIO0A6_P_SHIFT                 (12U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A6_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A6_P_SHIFT)                 /* 0x00003000 */
#define GPIO0_IOC_GPIO0A_P_GPIO0A7_P_SHIFT                 (14U)
#define GPIO0_IOC_GPIO0A_P_GPIO0A7_P_MASK                  (0x3U << GPIO0_IOC_GPIO0A_P_GPIO0A7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO0B_P */
#define GPIO0_IOC_GPIO0B_P_OFFSET                          (0x24U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B0_P_SHIFT                 (0U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B0_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B0_P_SHIFT)                 /* 0x00000003 */
#define GPIO0_IOC_GPIO0B_P_GPIO0B1_P_SHIFT                 (2U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B1_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B1_P_SHIFT)                 /* 0x0000000C */
#define GPIO0_IOC_GPIO0B_P_GPIO0B2_P_SHIFT                 (4U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B2_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B2_P_SHIFT)                 /* 0x00000030 */
#define GPIO0_IOC_GPIO0B_P_GPIO0B3_P_SHIFT                 (6U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B3_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO0_IOC_GPIO0B_P_GPIO0B4_P_SHIFT                 (8U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B4_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B4_P_SHIFT)                 /* 0x00000300 */
#define GPIO0_IOC_GPIO0B_P_GPIO0B5_P_SHIFT                 (10U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B5_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO0_IOC_GPIO0B_P_GPIO0B6_P_SHIFT                 (12U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B6_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B6_P_SHIFT)                 /* 0x00003000 */
#define GPIO0_IOC_GPIO0B_P_GPIO0B7_P_SHIFT                 (14U)
#define GPIO0_IOC_GPIO0B_P_GPIO0B7_P_MASK                  (0x3U << GPIO0_IOC_GPIO0B_P_GPIO0B7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO0C_P */
#define GPIO0_IOC_GPIO0C_P_OFFSET                          (0x28U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C0_P_SHIFT                 (0U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C0_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C0_P_SHIFT)                 /* 0x00000003 */
#define GPIO0_IOC_GPIO0C_P_GPIO0C1_P_SHIFT                 (2U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C1_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C1_P_SHIFT)                 /* 0x0000000C */
#define GPIO0_IOC_GPIO0C_P_GPIO0C2_P_SHIFT                 (4U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C2_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C2_P_SHIFT)                 /* 0x00000030 */
#define GPIO0_IOC_GPIO0C_P_GPIO0C3_P_SHIFT                 (6U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C3_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C3_P_SHIFT)                 /* 0x000000C0 */
#define GPIO0_IOC_GPIO0C_P_GPIO0C4_P_SHIFT                 (8U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C4_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C4_P_SHIFT)                 /* 0x00000300 */
#define GPIO0_IOC_GPIO0C_P_GPIO0C5_P_SHIFT                 (10U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C5_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C5_P_SHIFT)                 /* 0x00000C00 */
#define GPIO0_IOC_GPIO0C_P_GPIO0C6_P_SHIFT                 (12U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C6_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C6_P_SHIFT)                 /* 0x00003000 */
#define GPIO0_IOC_GPIO0C_P_GPIO0C7_P_SHIFT                 (14U)
#define GPIO0_IOC_GPIO0C_P_GPIO0C7_P_MASK                  (0x3U << GPIO0_IOC_GPIO0C_P_GPIO0C7_P_SHIFT)                 /* 0x0000C000 */
/* GPIO0D_P */
#define GPIO0_IOC_GPIO0D_P_OFFSET                          (0x2CU)
#define GPIO0_IOC_GPIO0D_P_GPIO0D0_P_SHIFT                 (0U)
#define GPIO0_IOC_GPIO0D_P_GPIO0D0_P_MASK                  (0x3U << GPIO0_IOC_GPIO0D_P_GPIO0D0_P_SHIFT)                 /* 0x00000003 */
#define GPIO0_IOC_GPIO0D_P_GPIO0D1_P_SHIFT                 (2U)
#define GPIO0_IOC_GPIO0D_P_GPIO0D1_P_MASK                  (0x3U << GPIO0_IOC_GPIO0D_P_GPIO0D1_P_SHIFT)                 /* 0x0000000C */
/* GPIO0A_IE */
#define GPIO0_IOC_GPIO0A_IE_OFFSET                         (0x30U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A0_IE_SHIFT               (0U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A0_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A0_IE_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A1_IE_SHIFT               (2U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A1_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A1_IE_SHIFT)               /* 0x0000000C */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A2_IE_SHIFT               (4U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A2_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A2_IE_SHIFT)               /* 0x00000030 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A3_IE_SHIFT               (6U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A3_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A4_IE_SHIFT               (8U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A4_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A4_IE_SHIFT)               /* 0x00000300 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A5_IE_SHIFT               (10U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A5_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A6_IE_SHIFT               (12U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A6_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A6_IE_SHIFT)               /* 0x00003000 */
#define GPIO0_IOC_GPIO0A_IE_GPIO0A7_IE_SHIFT               (14U)
#define GPIO0_IOC_GPIO0A_IE_GPIO0A7_IE_MASK                (0x3U << GPIO0_IOC_GPIO0A_IE_GPIO0A7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO0B_IE */
#define GPIO0_IOC_GPIO0B_IE_OFFSET                         (0x34U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B0_IE_SHIFT               (0U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B0_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B0_IE_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B1_IE_SHIFT               (2U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B1_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B1_IE_SHIFT)               /* 0x0000000C */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B2_IE_SHIFT               (4U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B2_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B2_IE_SHIFT)               /* 0x00000030 */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B3_IE_SHIFT               (6U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B3_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B4_IE_SHIFT               (8U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B4_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B4_IE_SHIFT)               /* 0x00000300 */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B5_IE_SHIFT               (10U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B5_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B6_IE_SHIFT               (12U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B6_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B6_IE_SHIFT)               /* 0x00003000 */
#define GPIO0_IOC_GPIO0B_IE_GPIO0B7_IE_SHIFT               (14U)
#define GPIO0_IOC_GPIO0B_IE_GPIO0B7_IE_MASK                (0x3U << GPIO0_IOC_GPIO0B_IE_GPIO0B7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO0C_IE */
#define GPIO0_IOC_GPIO0C_IE_OFFSET                         (0x38U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C0_IE_SHIFT               (0U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C0_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C0_IE_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C1_IE_SHIFT               (2U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C1_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C1_IE_SHIFT)               /* 0x0000000C */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C2_IE_SHIFT               (4U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C2_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C2_IE_SHIFT)               /* 0x00000030 */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C3_IE_SHIFT               (6U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C3_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C3_IE_SHIFT)               /* 0x000000C0 */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C4_IE_SHIFT               (8U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C4_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C4_IE_SHIFT)               /* 0x00000300 */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C5_IE_SHIFT               (10U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C5_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C5_IE_SHIFT)               /* 0x00000C00 */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C6_IE_SHIFT               (12U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C6_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C6_IE_SHIFT)               /* 0x00003000 */
#define GPIO0_IOC_GPIO0C_IE_GPIO0C7_IE_SHIFT               (14U)
#define GPIO0_IOC_GPIO0C_IE_GPIO0C7_IE_MASK                (0x3U << GPIO0_IOC_GPIO0C_IE_GPIO0C7_IE_SHIFT)               /* 0x0000C000 */
/* GPIO0D_IE */
#define GPIO0_IOC_GPIO0D_IE_OFFSET                         (0x3CU)
#define GPIO0_IOC_GPIO0D_IE_GPIO0D0_IE_SHIFT               (0U)
#define GPIO0_IOC_GPIO0D_IE_GPIO0D0_IE_MASK                (0x3U << GPIO0_IOC_GPIO0D_IE_GPIO0D0_IE_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0D_IE_GPIO0D1_IE_SHIFT               (2U)
#define GPIO0_IOC_GPIO0D_IE_GPIO0D1_IE_MASK                (0x3U << GPIO0_IOC_GPIO0D_IE_GPIO0D1_IE_SHIFT)               /* 0x0000000C */
/* GPIO0A_OD */
#define GPIO0_IOC_GPIO0A_OD_OFFSET                         (0x40U)
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
#define GPIO0_IOC_GPIO0A_OD_GPIO0A6_OD_SHIFT               (6U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A6_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A6_OD_SHIFT)               /* 0x00000040 */
#define GPIO0_IOC_GPIO0A_OD_GPIO0A7_OD_SHIFT               (7U)
#define GPIO0_IOC_GPIO0A_OD_GPIO0A7_OD_MASK                (0x1U << GPIO0_IOC_GPIO0A_OD_GPIO0A7_OD_SHIFT)               /* 0x00000080 */
/* GPIO0B_OD */
#define GPIO0_IOC_GPIO0B_OD_OFFSET                         (0x44U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B0_OD_SHIFT               (0U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B0_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B0_OD_SHIFT)               /* 0x00000001 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B1_OD_SHIFT               (1U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B1_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B1_OD_SHIFT)               /* 0x00000002 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B2_OD_SHIFT               (2U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B2_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B2_OD_SHIFT)               /* 0x00000004 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B3_OD_SHIFT               (3U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B3_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B3_OD_SHIFT)               /* 0x00000008 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B4_OD_SHIFT               (4U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B4_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B4_OD_SHIFT)               /* 0x00000010 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B5_OD_SHIFT               (5U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B5_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B5_OD_SHIFT)               /* 0x00000020 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B6_OD_SHIFT               (6U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B6_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B6_OD_SHIFT)               /* 0x00000040 */
#define GPIO0_IOC_GPIO0B_OD_GPIO0B7_OD_SHIFT               (7U)
#define GPIO0_IOC_GPIO0B_OD_GPIO0B7_OD_MASK                (0x1U << GPIO0_IOC_GPIO0B_OD_GPIO0B7_OD_SHIFT)               /* 0x00000080 */
/* GPIO0C_OD */
#define GPIO0_IOC_GPIO0C_OD_OFFSET                         (0x48U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C0_OD_SHIFT               (0U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C0_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C0_OD_SHIFT)               /* 0x00000001 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C1_OD_SHIFT               (1U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C1_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C1_OD_SHIFT)               /* 0x00000002 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C2_OD_SHIFT               (2U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C2_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C2_OD_SHIFT)               /* 0x00000004 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C3_OD_SHIFT               (3U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C3_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C3_OD_SHIFT)               /* 0x00000008 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C4_OD_SHIFT               (4U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C4_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C4_OD_SHIFT)               /* 0x00000010 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C5_OD_SHIFT               (5U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C5_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C5_OD_SHIFT)               /* 0x00000020 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C6_OD_SHIFT               (6U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C6_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C6_OD_SHIFT)               /* 0x00000040 */
#define GPIO0_IOC_GPIO0C_OD_GPIO0C7_OD_SHIFT               (7U)
#define GPIO0_IOC_GPIO0C_OD_GPIO0C7_OD_MASK                (0x1U << GPIO0_IOC_GPIO0C_OD_GPIO0C7_OD_SHIFT)               /* 0x00000080 */
/* GPIO0D_OD */
#define GPIO0_IOC_GPIO0D_OD_OFFSET                         (0x4CU)
#define GPIO0_IOC_GPIO0D_OD_GPIO0D0_OD_SHIFT               (0U)
#define GPIO0_IOC_GPIO0D_OD_GPIO0D0_OD_MASK                (0x1U << GPIO0_IOC_GPIO0D_OD_GPIO0D0_OD_SHIFT)               /* 0x00000001 */
#define GPIO0_IOC_GPIO0D_OD_GPIO0D1_OD_SHIFT               (1U)
#define GPIO0_IOC_GPIO0D_OD_GPIO0D1_OD_MASK                (0x1U << GPIO0_IOC_GPIO0D_OD_GPIO0D1_OD_SHIFT)               /* 0x00000002 */
/* GPIO0A_SUS */
#define GPIO0_IOC_GPIO0A_SUS_OFFSET                        (0x50U)
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
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A6_SUS_SHIFT             (6U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A6_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A7_SUS_SHIFT             (7U)
#define GPIO0_IOC_GPIO0A_SUS_GPIO0A7_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0A_SUS_GPIO0A7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO0B_SUS */
#define GPIO0_IOC_GPIO0B_SUS_OFFSET                        (0x54U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B0_SUS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B0_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B1_SUS_SHIFT             (1U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B1_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B2_SUS_SHIFT             (2U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B2_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B3_SUS_SHIFT             (3U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B3_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B4_SUS_SHIFT             (4U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B4_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B5_SUS_SHIFT             (5U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B5_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B6_SUS_SHIFT             (6U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B6_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B7_SUS_SHIFT             (7U)
#define GPIO0_IOC_GPIO0B_SUS_GPIO0B7_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0B_SUS_GPIO0B7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO0C_SUS */
#define GPIO0_IOC_GPIO0C_SUS_OFFSET                        (0x58U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C0_SUS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C0_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C1_SUS_SHIFT             (1U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C1_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C1_SUS_SHIFT)             /* 0x00000002 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C2_SUS_SHIFT             (2U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C2_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C2_SUS_SHIFT)             /* 0x00000004 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C3_SUS_SHIFT             (3U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C3_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C3_SUS_SHIFT)             /* 0x00000008 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C4_SUS_SHIFT             (4U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C4_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C4_SUS_SHIFT)             /* 0x00000010 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C5_SUS_SHIFT             (5U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C5_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C5_SUS_SHIFT)             /* 0x00000020 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C6_SUS_SHIFT             (6U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C6_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C6_SUS_SHIFT)             /* 0x00000040 */
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C7_SUS_SHIFT             (7U)
#define GPIO0_IOC_GPIO0C_SUS_GPIO0C7_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0C_SUS_GPIO0C7_SUS_SHIFT)             /* 0x00000080 */
/* GPIO0D_SUS */
#define GPIO0_IOC_GPIO0D_SUS_OFFSET                        (0x5CU)
#define GPIO0_IOC_GPIO0D_SUS_GPIO0D0_SUS_SHIFT             (0U)
#define GPIO0_IOC_GPIO0D_SUS_GPIO0D0_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0D_SUS_GPIO0D0_SUS_SHIFT)             /* 0x00000001 */
#define GPIO0_IOC_GPIO0D_SUS_GPIO0D1_SUS_SHIFT             (1U)
#define GPIO0_IOC_GPIO0D_SUS_GPIO0D1_SUS_MASK              (0x1U << GPIO0_IOC_GPIO0D_SUS_GPIO0D1_SUS_SHIFT)             /* 0x00000002 */
/* GPIO0A_SL */
#define GPIO0_IOC_GPIO0A_SL_OFFSET                         (0x60U)
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
#define GPIO0_IOC_GPIO0A_SL_GPIO0A6_SL_SHIFT               (12U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A6_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A6_SL_SHIFT)               /* 0x00003000 */
#define GPIO0_IOC_GPIO0A_SL_GPIO0A7_SL_SHIFT               (14U)
#define GPIO0_IOC_GPIO0A_SL_GPIO0A7_SL_MASK                (0x3U << GPIO0_IOC_GPIO0A_SL_GPIO0A7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO0B_SL */
#define GPIO0_IOC_GPIO0B_SL_OFFSET                         (0x64U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B0_SL_SHIFT               (0U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B0_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B0_SL_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B1_SL_SHIFT               (2U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B1_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B1_SL_SHIFT)               /* 0x0000000C */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B2_SL_SHIFT               (4U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B2_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B2_SL_SHIFT)               /* 0x00000030 */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B3_SL_SHIFT               (6U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B3_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B4_SL_SHIFT               (8U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B4_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B4_SL_SHIFT)               /* 0x00000300 */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B5_SL_SHIFT               (10U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B5_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B6_SL_SHIFT               (12U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B6_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B6_SL_SHIFT)               /* 0x00003000 */
#define GPIO0_IOC_GPIO0B_SL_GPIO0B7_SL_SHIFT               (14U)
#define GPIO0_IOC_GPIO0B_SL_GPIO0B7_SL_MASK                (0x3U << GPIO0_IOC_GPIO0B_SL_GPIO0B7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO0C_SL */
#define GPIO0_IOC_GPIO0C_SL_OFFSET                         (0x68U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C0_SL_SHIFT               (0U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C0_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C0_SL_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C1_SL_SHIFT               (2U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C1_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C1_SL_SHIFT)               /* 0x0000000C */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C2_SL_SHIFT               (4U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C2_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C2_SL_SHIFT)               /* 0x00000030 */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C3_SL_SHIFT               (6U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C3_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C3_SL_SHIFT)               /* 0x000000C0 */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C4_SL_SHIFT               (8U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C4_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C4_SL_SHIFT)               /* 0x00000300 */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C5_SL_SHIFT               (10U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C5_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C5_SL_SHIFT)               /* 0x00000C00 */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C6_SL_SHIFT               (12U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C6_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C6_SL_SHIFT)               /* 0x00003000 */
#define GPIO0_IOC_GPIO0C_SL_GPIO0C7_SL_SHIFT               (14U)
#define GPIO0_IOC_GPIO0C_SL_GPIO0C7_SL_MASK                (0x3U << GPIO0_IOC_GPIO0C_SL_GPIO0C7_SL_SHIFT)               /* 0x0000C000 */
/* GPIO0D_SL */
#define GPIO0_IOC_GPIO0D_SL_OFFSET                         (0x6CU)
#define GPIO0_IOC_GPIO0D_SL_GPIO0D0_SL_SHIFT               (0U)
#define GPIO0_IOC_GPIO0D_SL_GPIO0D0_SL_MASK                (0x3U << GPIO0_IOC_GPIO0D_SL_GPIO0D0_SL_SHIFT)               /* 0x00000003 */
#define GPIO0_IOC_GPIO0D_SL_GPIO0D1_SL_SHIFT               (2U)
#define GPIO0_IOC_GPIO0D_SL_GPIO0D1_SL_MASK                (0x3U << GPIO0_IOC_GPIO0D_SL_GPIO0D1_SL_SHIFT)               /* 0x0000000C */
/* GPIO0A_DS0 */
#define GPIO0_IOC_GPIO0A_DS0_OFFSET                        (0x70U)
#define GPIO0_IOC_GPIO0A_DS0_GPIO0A0_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0A_DS0_GPIO0A0_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS0_GPIO0A0_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS0_GPIO0A1_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0A_DS0_GPIO0A1_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS0_GPIO0A1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0A_DS1 */
#define GPIO0_IOC_GPIO0A_DS1_OFFSET                        (0x74U)
#define GPIO0_IOC_GPIO0A_DS1_GPIO0A2_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0A_DS1_GPIO0A2_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS1_GPIO0A2_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS1_GPIO0A3_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0A_DS1_GPIO0A3_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS1_GPIO0A3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0A_DS2 */
#define GPIO0_IOC_GPIO0A_DS2_OFFSET                        (0x78U)
#define GPIO0_IOC_GPIO0A_DS2_GPIO0A4_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0A_DS2_GPIO0A4_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS2_GPIO0A4_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS2_GPIO0A5_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0A_DS2_GPIO0A5_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS2_GPIO0A5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0A_DS3 */
#define GPIO0_IOC_GPIO0A_DS3_OFFSET                        (0x7CU)
#define GPIO0_IOC_GPIO0A_DS3_GPIO0A6_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0A_DS3_GPIO0A6_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS3_GPIO0A6_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0A_DS3_GPIO0A7_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0A_DS3_GPIO0A7_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0A_DS3_GPIO0A7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0B_DS0 */
#define GPIO0_IOC_GPIO0B_DS0_OFFSET                        (0x80U)
#define GPIO0_IOC_GPIO0B_DS0_GPIO0B0_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0B_DS0_GPIO0B0_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS0_GPIO0B0_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0B_DS0_GPIO0B1_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0B_DS0_GPIO0B1_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS0_GPIO0B1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0B_DS1 */
#define GPIO0_IOC_GPIO0B_DS1_OFFSET                        (0x84U)
#define GPIO0_IOC_GPIO0B_DS1_GPIO0B2_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0B_DS1_GPIO0B2_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS1_GPIO0B2_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0B_DS1_GPIO0B3_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0B_DS1_GPIO0B3_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS1_GPIO0B3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0B_DS2 */
#define GPIO0_IOC_GPIO0B_DS2_OFFSET                        (0x88U)
#define GPIO0_IOC_GPIO0B_DS2_GPIO0B4_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0B_DS2_GPIO0B4_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS2_GPIO0B4_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0B_DS2_GPIO0B5_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0B_DS2_GPIO0B5_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS2_GPIO0B5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0B_DS3 */
#define GPIO0_IOC_GPIO0B_DS3_OFFSET                        (0x8CU)
#define GPIO0_IOC_GPIO0B_DS3_GPIO0B6_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0B_DS3_GPIO0B6_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS3_GPIO0B6_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0B_DS3_GPIO0B7_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0B_DS3_GPIO0B7_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0B_DS3_GPIO0B7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0C_DS0 */
#define GPIO0_IOC_GPIO0C_DS0_OFFSET                        (0x90U)
#define GPIO0_IOC_GPIO0C_DS0_GPIO0C0_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0C_DS0_GPIO0C0_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS0_GPIO0C0_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0C_DS0_GPIO0C1_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0C_DS0_GPIO0C1_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS0_GPIO0C1_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0C_DS1 */
#define GPIO0_IOC_GPIO0C_DS1_OFFSET                        (0x94U)
#define GPIO0_IOC_GPIO0C_DS1_GPIO0C2_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0C_DS1_GPIO0C2_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS1_GPIO0C2_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0C_DS1_GPIO0C3_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0C_DS1_GPIO0C3_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS1_GPIO0C3_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0C_DS2 */
#define GPIO0_IOC_GPIO0C_DS2_OFFSET                        (0x98U)
#define GPIO0_IOC_GPIO0C_DS2_GPIO0C4_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0C_DS2_GPIO0C4_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS2_GPIO0C4_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0C_DS2_GPIO0C5_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0C_DS2_GPIO0C5_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS2_GPIO0C5_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0C_DS3 */
#define GPIO0_IOC_GPIO0C_DS3_OFFSET                        (0x9CU)
#define GPIO0_IOC_GPIO0C_DS3_GPIO0C6_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0C_DS3_GPIO0C6_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS3_GPIO0C6_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0C_DS3_GPIO0C7_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0C_DS3_GPIO0C7_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0C_DS3_GPIO0C7_DS_SHIFT)             /* 0x00003F00 */
/* GPIO0D_DS0 */
#define GPIO0_IOC_GPIO0D_DS0_OFFSET                        (0xA0U)
#define GPIO0_IOC_GPIO0D_DS0_GPIO0D0_DS_SHIFT              (0U)
#define GPIO0_IOC_GPIO0D_DS0_GPIO0D0_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0D_DS0_GPIO0D0_DS_SHIFT)             /* 0x0000003F */
#define GPIO0_IOC_GPIO0D_DS0_GPIO0D1_DS_SHIFT              (8U)
#define GPIO0_IOC_GPIO0D_DS0_GPIO0D1_DS_MASK               (0x3FU << GPIO0_IOC_GPIO0D_DS0_GPIO0D1_DS_SHIFT)             /* 0x00003F00 */
/* JTAG_M0_CON */
#define GPIO0_IOC_JTAG_M0_CON_OFFSET                       (0x100U)
#define GPIO0_IOC_JTAG_M0_CON_FORCE_JTAG_UART_SHIFT        (0U)
#define GPIO0_IOC_JTAG_M0_CON_FORCE_JTAG_UART_MASK         (0x1U << GPIO0_IOC_JTAG_M0_CON_FORCE_JTAG_UART_SHIFT)        /* 0x00000001 */
#define GPIO0_IOC_JTAG_M0_CON_JTAG_SEL_M0_SHIFT            (1U)
#define GPIO0_IOC_JTAG_M0_CON_JTAG_SEL_M0_MASK             (0x3U << GPIO0_IOC_JTAG_M0_CON_JTAG_SEL_M0_SHIFT)            /* 0x00000006 */
/* UART_IO_CON */
#define GPIO0_IOC_UART_IO_CON_OFFSET                       (0x104U)
#define GPIO0_IOC_UART_IO_CON_UART0RX_LOW_DLY_SHIFT        (0U)
#define GPIO0_IOC_UART_IO_CON_UART0RX_LOW_DLY_MASK         (0xFFFFFFFFU << GPIO0_IOC_UART_IO_CON_UART0RX_LOW_DLY_SHIFT) /* 0xFFFFFFFF */
/* IO_VSEL2 */
#define GPIO0_IOC_IO_VSEL2_OFFSET                          (0x148U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL_SHIFT            (0U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL_MASK             (0x1U << GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL_SHIFT)            /* 0x00000001 */
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL18_SHIFT          (1U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL18_MASK           (0x1U << GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL18_SHIFT)          /* 0x00000002 */
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL33_SHIFT          (2U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL33_MASK           (0x1U << GPIO0_IOC_IO_VSEL2_POC_PMUIO1_SEL33_SHIFT)          /* 0x00000004 */
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_IDDQ_SHIFT           (3U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_IDDQ_MASK            (0x1U << GPIO0_IOC_IO_VSEL2_POC_PMUIO1_IDDQ_SHIFT)           /* 0x00000008 */
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO0_IDDQ_SHIFT           (4U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO0_IDDQ_MASK            (0x1U << GPIO0_IOC_IO_VSEL2_POC_PMUIO0_IDDQ_SHIFT)           /* 0x00000010 */
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_VD_SHIFT             (12U)
#define GPIO0_IOC_IO_VSEL2_POC_PMUIO1_VD_MASK              (0x1U << GPIO0_IOC_IO_VSEL2_POC_PMUIO1_VD_SHIFT)             /* 0x00001000 */
/* XIN_CON */
#define GPIO0_IOC_XIN_CON_OFFSET                           (0x14CU)
#define GPIO0_IOC_XIN_CON_OSC_DS_SHIFT                     (0U)
#define GPIO0_IOC_XIN_CON_OSC_DS_MASK                      (0x7U << GPIO0_IOC_XIN_CON_OSC_DS_SHIFT)                     /* 0x00000007 */
#define GPIO0_IOC_XIN_CON_RESERVED_SHIFT                   (3U)
#define GPIO0_IOC_XIN_CON_RESERVED_MASK                    (0x1U << GPIO0_IOC_XIN_CON_RESERVED_SHIFT)                   /* 0x00000008 */
/*****************************************TOPCRU*****************************************/
/* APLL_CON0 */
#define TOPCRU_APLL_CON0_OFFSET                            (0x0U)
#define TOPCRU_APLL_CON0_FBDIV_SHIFT                       (0U)
#define TOPCRU_APLL_CON0_FBDIV_MASK                        (0xFFFU << TOPCRU_APLL_CON0_FBDIV_SHIFT)                     /* 0x00000FFF */
#define TOPCRU_APLL_CON0_POSTDIV1_SHIFT                    (12U)
#define TOPCRU_APLL_CON0_POSTDIV1_MASK                     (0x7U << TOPCRU_APLL_CON0_POSTDIV1_SHIFT)                    /* 0x00007000 */
#define TOPCRU_APLL_CON0_BYPASS_SHIFT                      (15U)
#define TOPCRU_APLL_CON0_BYPASS_MASK                       (0x1U << TOPCRU_APLL_CON0_BYPASS_SHIFT)                      /* 0x00008000 */
/* APLL_CON1 */
#define TOPCRU_APLL_CON1_OFFSET                            (0x4U)
#define TOPCRU_APLL_CON1_REFDIV_SHIFT                      (0U)
#define TOPCRU_APLL_CON1_REFDIV_MASK                       (0x3FU << TOPCRU_APLL_CON1_REFDIV_SHIFT)                     /* 0x0000003F */
#define TOPCRU_APLL_CON1_POSTDIV2_SHIFT                    (6U)
#define TOPCRU_APLL_CON1_POSTDIV2_MASK                     (0x7U << TOPCRU_APLL_CON1_POSTDIV2_SHIFT)                    /* 0x000001C0 */
#define TOPCRU_APLL_CON1_PLL_LOCK_SHIFT                    (10U)
#define TOPCRU_APLL_CON1_PLL_LOCK_MASK                     (0x1U << TOPCRU_APLL_CON1_PLL_LOCK_SHIFT)                    /* 0x00000400 */
#define TOPCRU_APLL_CON1_DSMPD_SHIFT                       (12U)
#define TOPCRU_APLL_CON1_DSMPD_MASK                        (0x1U << TOPCRU_APLL_CON1_DSMPD_SHIFT)                       /* 0x00001000 */
#define TOPCRU_APLL_CON1_PLLPD0_SHIFT                      (13U)
#define TOPCRU_APLL_CON1_PLLPD0_MASK                       (0x1U << TOPCRU_APLL_CON1_PLLPD0_SHIFT)                      /* 0x00002000 */
#define TOPCRU_APLL_CON1_PLLPD1_SHIFT                      (14U)
#define TOPCRU_APLL_CON1_PLLPD1_MASK                       (0x1U << TOPCRU_APLL_CON1_PLLPD1_SHIFT)                      /* 0x00004000 */
#define TOPCRU_APLL_CON1_PLLPDSEL_SHIFT                    (15U)
#define TOPCRU_APLL_CON1_PLLPDSEL_MASK                     (0x1U << TOPCRU_APLL_CON1_PLLPDSEL_SHIFT)                    /* 0x00008000 */
/* APLL_CON2 */
#define TOPCRU_APLL_CON2_OFFSET                            (0x8U)
#define TOPCRU_APLL_CON2_FOUTPOSTDIVPD_SHIFT               (25U)
#define TOPCRU_APLL_CON2_FOUTPOSTDIVPD_MASK                (0x1U << TOPCRU_APLL_CON2_FOUTPOSTDIVPD_SHIFT)               /* 0x02000000 */
#define TOPCRU_APLL_CON2_FOUTVCOPD_SHIFT                   (26U)
#define TOPCRU_APLL_CON2_FOUTVCOPD_MASK                    (0x1U << TOPCRU_APLL_CON2_FOUTVCOPD_SHIFT)                   /* 0x04000000 */
#define TOPCRU_APLL_CON2_FOUT4PHASEPD_SHIFT                (27U)
#define TOPCRU_APLL_CON2_FOUT4PHASEPD_MASK                 (0x1U << TOPCRU_APLL_CON2_FOUT4PHASEPD_SHIFT)                /* 0x08000000 */
/* GPLL_CON0 */
#define TOPCRU_GPLL_CON0_OFFSET                            (0x60U)
#define TOPCRU_GPLL_CON0_FBDIV_SHIFT                       (0U)
#define TOPCRU_GPLL_CON0_FBDIV_MASK                        (0xFFFU << TOPCRU_GPLL_CON0_FBDIV_SHIFT)                     /* 0x00000FFF */
#define TOPCRU_GPLL_CON0_POSTDIV1_SHIFT                    (12U)
#define TOPCRU_GPLL_CON0_POSTDIV1_MASK                     (0x7U << TOPCRU_GPLL_CON0_POSTDIV1_SHIFT)                    /* 0x00007000 */
#define TOPCRU_GPLL_CON0_BYPASS_SHIFT                      (15U)
#define TOPCRU_GPLL_CON0_BYPASS_MASK                       (0x1U << TOPCRU_GPLL_CON0_BYPASS_SHIFT)                      /* 0x00008000 */
/* GPLL_CON1 */
#define TOPCRU_GPLL_CON1_OFFSET                            (0x64U)
#define TOPCRU_GPLL_CON1_REFDIV_SHIFT                      (0U)
#define TOPCRU_GPLL_CON1_REFDIV_MASK                       (0x3FU << TOPCRU_GPLL_CON1_REFDIV_SHIFT)                     /* 0x0000003F */
#define TOPCRU_GPLL_CON1_POSTDIV2_SHIFT                    (6U)
#define TOPCRU_GPLL_CON1_POSTDIV2_MASK                     (0x7U << TOPCRU_GPLL_CON1_POSTDIV2_SHIFT)                    /* 0x000001C0 */
#define TOPCRU_GPLL_CON1_PLL_LOCK_SHIFT                    (10U)
#define TOPCRU_GPLL_CON1_PLL_LOCK_MASK                     (0x1U << TOPCRU_GPLL_CON1_PLL_LOCK_SHIFT)                    /* 0x00000400 */
#define TOPCRU_GPLL_CON1_DSMPD_SHIFT                       (12U)
#define TOPCRU_GPLL_CON1_DSMPD_MASK                        (0x1U << TOPCRU_GPLL_CON1_DSMPD_SHIFT)                       /* 0x00001000 */
#define TOPCRU_GPLL_CON1_PLLPD0_SHIFT                      (13U)
#define TOPCRU_GPLL_CON1_PLLPD0_MASK                       (0x1U << TOPCRU_GPLL_CON1_PLLPD0_SHIFT)                      /* 0x00002000 */
#define TOPCRU_GPLL_CON1_PLLPD1_SHIFT                      (14U)
#define TOPCRU_GPLL_CON1_PLLPD1_MASK                       (0x1U << TOPCRU_GPLL_CON1_PLLPD1_SHIFT)                      /* 0x00004000 */
#define TOPCRU_GPLL_CON1_PLLPDSEL_SHIFT                    (15U)
#define TOPCRU_GPLL_CON1_PLLPDSEL_MASK                     (0x1U << TOPCRU_GPLL_CON1_PLLPDSEL_SHIFT)                    /* 0x00008000 */
/* GPLL_CON2 */
#define TOPCRU_GPLL_CON2_OFFSET                            (0x68U)
#define TOPCRU_GPLL_CON2_FRACDIV_SHIFT                     (0U)
#define TOPCRU_GPLL_CON2_FRACDIV_MASK                      (0xFFFFFFU << TOPCRU_GPLL_CON2_FRACDIV_SHIFT)                /* 0x00FFFFFF */
#define TOPCRU_GPLL_CON2_DACPD_SHIFT                       (24U)
#define TOPCRU_GPLL_CON2_DACPD_MASK                        (0x1U << TOPCRU_GPLL_CON2_DACPD_SHIFT)                       /* 0x01000000 */
#define TOPCRU_GPLL_CON2_FOUTPOSTDIVPD_SHIFT               (25U)
#define TOPCRU_GPLL_CON2_FOUTPOSTDIVPD_MASK                (0x1U << TOPCRU_GPLL_CON2_FOUTPOSTDIVPD_SHIFT)               /* 0x02000000 */
#define TOPCRU_GPLL_CON2_FOUTVCOPD_SHIFT                   (26U)
#define TOPCRU_GPLL_CON2_FOUTVCOPD_MASK                    (0x1U << TOPCRU_GPLL_CON2_FOUTVCOPD_SHIFT)                   /* 0x04000000 */
#define TOPCRU_GPLL_CON2_FOUT4PHASEPD_SHIFT                (27U)
#define TOPCRU_GPLL_CON2_FOUT4PHASEPD_MASK                 (0x1U << TOPCRU_GPLL_CON2_FOUT4PHASEPD_SHIFT)                /* 0x08000000 */
/* GPLL_CON3 */
#define TOPCRU_GPLL_CON3_OFFSET                            (0x6CU)
#define TOPCRU_GPLL_CON3_SSMOD_BP_SHIFT                    (0U)
#define TOPCRU_GPLL_CON3_SSMOD_BP_MASK                     (0x1U << TOPCRU_GPLL_CON3_SSMOD_BP_SHIFT)                    /* 0x00000001 */
#define TOPCRU_GPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT          (1U)
#define TOPCRU_GPLL_CON3_SSMOD_DISABLE_SSCG_MASK           (0x1U << TOPCRU_GPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)          /* 0x00000002 */
#define TOPCRU_GPLL_CON3_SSMOD_RESET_SHIFT                 (2U)
#define TOPCRU_GPLL_CON3_SSMOD_RESET_MASK                  (0x1U << TOPCRU_GPLL_CON3_SSMOD_RESET_SHIFT)                 /* 0x00000004 */
#define TOPCRU_GPLL_CON3_SSMOD_DOWNSPREAD_SHIFT            (3U)
#define TOPCRU_GPLL_CON3_SSMOD_DOWNSPREAD_MASK             (0x1U << TOPCRU_GPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)            /* 0x00000008 */
#define TOPCRU_GPLL_CON3_SSMOD_DIVVAL_SHIFT                (4U)
#define TOPCRU_GPLL_CON3_SSMOD_DIVVAL_MASK                 (0xFU << TOPCRU_GPLL_CON3_SSMOD_DIVVAL_SHIFT)                /* 0x000000F0 */
#define TOPCRU_GPLL_CON3_SSMOD_SPREAD_SHIFT                (8U)
#define TOPCRU_GPLL_CON3_SSMOD_SPREAD_MASK                 (0x1FU << TOPCRU_GPLL_CON3_SSMOD_SPREAD_SHIFT)               /* 0x00001F00 */
/* GPLL_CON4 */
#define TOPCRU_GPLL_CON4_OFFSET                            (0x70U)
#define TOPCRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT          (0U)
#define TOPCRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK           (0x1U << TOPCRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)          /* 0x00000001 */
#define TOPCRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT           (8U)
#define TOPCRU_GPLL_CON4_SSMOD_EXT_MAXADDR_MASK            (0xFFU << TOPCRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)          /* 0x0000FF00 */
/* VPLL_CON0 */
#define TOPCRU_VPLL_CON0_OFFSET                            (0x80U)
#define TOPCRU_VPLL_CON0_FBDIV_SHIFT                       (0U)
#define TOPCRU_VPLL_CON0_FBDIV_MASK                        (0xFFFU << TOPCRU_VPLL_CON0_FBDIV_SHIFT)                     /* 0x00000FFF */
#define TOPCRU_VPLL_CON0_POSTDIV1_SHIFT                    (12U)
#define TOPCRU_VPLL_CON0_POSTDIV1_MASK                     (0x7U << TOPCRU_VPLL_CON0_POSTDIV1_SHIFT)                    /* 0x00007000 */
#define TOPCRU_VPLL_CON0_BYPASS_SHIFT                      (15U)
#define TOPCRU_VPLL_CON0_BYPASS_MASK                       (0x1U << TOPCRU_VPLL_CON0_BYPASS_SHIFT)                      /* 0x00008000 */
/* VPLL_CON1 */
#define TOPCRU_VPLL_CON1_OFFSET                            (0x84U)
#define TOPCRU_VPLL_CON1_REFDIV_SHIFT                      (0U)
#define TOPCRU_VPLL_CON1_REFDIV_MASK                       (0x3FU << TOPCRU_VPLL_CON1_REFDIV_SHIFT)                     /* 0x0000003F */
#define TOPCRU_VPLL_CON1_POSTDIV2_SHIFT                    (6U)
#define TOPCRU_VPLL_CON1_POSTDIV2_MASK                     (0x7U << TOPCRU_VPLL_CON1_POSTDIV2_SHIFT)                    /* 0x000001C0 */
#define TOPCRU_VPLL_CON1_PLL_LOCK_SHIFT                    (10U)
#define TOPCRU_VPLL_CON1_PLL_LOCK_MASK                     (0x1U << TOPCRU_VPLL_CON1_PLL_LOCK_SHIFT)                    /* 0x00000400 */
#define TOPCRU_VPLL_CON1_DSMPD_SHIFT                       (12U)
#define TOPCRU_VPLL_CON1_DSMPD_MASK                        (0x1U << TOPCRU_VPLL_CON1_DSMPD_SHIFT)                       /* 0x00001000 */
#define TOPCRU_VPLL_CON1_PLLPD0_SHIFT                      (13U)
#define TOPCRU_VPLL_CON1_PLLPD0_MASK                       (0x1U << TOPCRU_VPLL_CON1_PLLPD0_SHIFT)                      /* 0x00002000 */
#define TOPCRU_VPLL_CON1_PLLPD1_SHIFT                      (14U)
#define TOPCRU_VPLL_CON1_PLLPD1_MASK                       (0x1U << TOPCRU_VPLL_CON1_PLLPD1_SHIFT)                      /* 0x00004000 */
#define TOPCRU_VPLL_CON1_PLLPDSEL_SHIFT                    (15U)
#define TOPCRU_VPLL_CON1_PLLPDSEL_MASK                     (0x1U << TOPCRU_VPLL_CON1_PLLPDSEL_SHIFT)                    /* 0x00008000 */
/* VPLL_CON2 */
#define TOPCRU_VPLL_CON2_OFFSET                            (0x88U)
#define TOPCRU_VPLL_CON2_FRACDIV_SHIFT                     (0U)
#define TOPCRU_VPLL_CON2_FRACDIV_MASK                      (0xFFFFFFU << TOPCRU_VPLL_CON2_FRACDIV_SHIFT)                /* 0x00FFFFFF */
#define TOPCRU_VPLL_CON2_DACPD_SHIFT                       (24U)
#define TOPCRU_VPLL_CON2_DACPD_MASK                        (0x1U << TOPCRU_VPLL_CON2_DACPD_SHIFT)                       /* 0x01000000 */
#define TOPCRU_VPLL_CON2_FOUTPOSTDIVPD_SHIFT               (25U)
#define TOPCRU_VPLL_CON2_FOUTPOSTDIVPD_MASK                (0x1U << TOPCRU_VPLL_CON2_FOUTPOSTDIVPD_SHIFT)               /* 0x02000000 */
#define TOPCRU_VPLL_CON2_FOUTVCOPD_SHIFT                   (26U)
#define TOPCRU_VPLL_CON2_FOUTVCOPD_MASK                    (0x1U << TOPCRU_VPLL_CON2_FOUTVCOPD_SHIFT)                   /* 0x04000000 */
#define TOPCRU_VPLL_CON2_FOUT4PHASEPD_SHIFT                (27U)
#define TOPCRU_VPLL_CON2_FOUT4PHASEPD_MASK                 (0x1U << TOPCRU_VPLL_CON2_FOUT4PHASEPD_SHIFT)                /* 0x08000000 */
/* VPLL_CON3 */
#define TOPCRU_VPLL_CON3_OFFSET                            (0x8CU)
#define TOPCRU_VPLL_CON3_SSMOD_BP_SHIFT                    (0U)
#define TOPCRU_VPLL_CON3_SSMOD_BP_MASK                     (0x1U << TOPCRU_VPLL_CON3_SSMOD_BP_SHIFT)                    /* 0x00000001 */
#define TOPCRU_VPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT          (1U)
#define TOPCRU_VPLL_CON3_SSMOD_DISABLE_SSCG_MASK           (0x1U << TOPCRU_VPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)          /* 0x00000002 */
#define TOPCRU_VPLL_CON3_SSMOD_RESET_SHIFT                 (2U)
#define TOPCRU_VPLL_CON3_SSMOD_RESET_MASK                  (0x1U << TOPCRU_VPLL_CON3_SSMOD_RESET_SHIFT)                 /* 0x00000004 */
#define TOPCRU_VPLL_CON3_SSMOD_DOWNSPREAD_SHIFT            (3U)
#define TOPCRU_VPLL_CON3_SSMOD_DOWNSPREAD_MASK             (0x1U << TOPCRU_VPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)            /* 0x00000008 */
#define TOPCRU_VPLL_CON3_SSMOD_DIVVAL_SHIFT                (4U)
#define TOPCRU_VPLL_CON3_SSMOD_DIVVAL_MASK                 (0xFU << TOPCRU_VPLL_CON3_SSMOD_DIVVAL_SHIFT)                /* 0x000000F0 */
#define TOPCRU_VPLL_CON3_SSMOD_SPREAD_SHIFT                (8U)
#define TOPCRU_VPLL_CON3_SSMOD_SPREAD_MASK                 (0x1FU << TOPCRU_VPLL_CON3_SSMOD_SPREAD_SHIFT)               /* 0x00001F00 */
/* VPLL_CON4 */
#define TOPCRU_VPLL_CON4_OFFSET                            (0x90U)
#define TOPCRU_VPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT          (0U)
#define TOPCRU_VPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK           (0x1U << TOPCRU_VPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)          /* 0x00000001 */
#define TOPCRU_VPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT           (8U)
#define TOPCRU_VPLL_CON4_SSMOD_EXT_MAXADDR_MASK            (0xFFU << TOPCRU_VPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)          /* 0x0000FF00 */
/* HPLL_CON0 */
#define TOPCRU_HPLL_CON0_OFFSET                            (0xA0U)
#define TOPCRU_HPLL_CON0_FBDIV_SHIFT                       (0U)
#define TOPCRU_HPLL_CON0_FBDIV_MASK                        (0xFFFU << TOPCRU_HPLL_CON0_FBDIV_SHIFT)                     /* 0x00000FFF */
#define TOPCRU_HPLL_CON0_POSTDIV1_SHIFT                    (12U)
#define TOPCRU_HPLL_CON0_POSTDIV1_MASK                     (0x7U << TOPCRU_HPLL_CON0_POSTDIV1_SHIFT)                    /* 0x00007000 */
#define TOPCRU_HPLL_CON0_BYPASS_SHIFT                      (15U)
#define TOPCRU_HPLL_CON0_BYPASS_MASK                       (0x1U << TOPCRU_HPLL_CON0_BYPASS_SHIFT)                      /* 0x00008000 */
/* HPLL_CON1 */
#define TOPCRU_HPLL_CON1_OFFSET                            (0xA4U)
#define TOPCRU_HPLL_CON1_REFDIV_SHIFT                      (0U)
#define TOPCRU_HPLL_CON1_REFDIV_MASK                       (0x3FU << TOPCRU_HPLL_CON1_REFDIV_SHIFT)                     /* 0x0000003F */
#define TOPCRU_HPLL_CON1_POSTDIV2_SHIFT                    (6U)
#define TOPCRU_HPLL_CON1_POSTDIV2_MASK                     (0x7U << TOPCRU_HPLL_CON1_POSTDIV2_SHIFT)                    /* 0x000001C0 */
#define TOPCRU_HPLL_CON1_PLL_LOCK_SHIFT                    (10U)
#define TOPCRU_HPLL_CON1_PLL_LOCK_MASK                     (0x1U << TOPCRU_HPLL_CON1_PLL_LOCK_SHIFT)                    /* 0x00000400 */
#define TOPCRU_HPLL_CON1_DSMPD_SHIFT                       (12U)
#define TOPCRU_HPLL_CON1_DSMPD_MASK                        (0x1U << TOPCRU_HPLL_CON1_DSMPD_SHIFT)                       /* 0x00001000 */
#define TOPCRU_HPLL_CON1_PLLPD0_SHIFT                      (13U)
#define TOPCRU_HPLL_CON1_PLLPD0_MASK                       (0x1U << TOPCRU_HPLL_CON1_PLLPD0_SHIFT)                      /* 0x00002000 */
#define TOPCRU_HPLL_CON1_PLLPD1_SHIFT                      (14U)
#define TOPCRU_HPLL_CON1_PLLPD1_MASK                       (0x1U << TOPCRU_HPLL_CON1_PLLPD1_SHIFT)                      /* 0x00004000 */
#define TOPCRU_HPLL_CON1_PLLPDSEL_SHIFT                    (15U)
#define TOPCRU_HPLL_CON1_PLLPDSEL_MASK                     (0x1U << TOPCRU_HPLL_CON1_PLLPDSEL_SHIFT)                    /* 0x00008000 */
/* HPLL_CON2 */
#define TOPCRU_HPLL_CON2_OFFSET                            (0xA8U)
#define TOPCRU_HPLL_CON2_FRACDIV_SHIFT                     (0U)
#define TOPCRU_HPLL_CON2_FRACDIV_MASK                      (0xFFFFFFU << TOPCRU_HPLL_CON2_FRACDIV_SHIFT)                /* 0x00FFFFFF */
#define TOPCRU_HPLL_CON2_DACPD_SHIFT                       (24U)
#define TOPCRU_HPLL_CON2_DACPD_MASK                        (0x1U << TOPCRU_HPLL_CON2_DACPD_SHIFT)                       /* 0x01000000 */
#define TOPCRU_HPLL_CON2_FOUTPOSTDIVPD_SHIFT               (25U)
#define TOPCRU_HPLL_CON2_FOUTPOSTDIVPD_MASK                (0x1U << TOPCRU_HPLL_CON2_FOUTPOSTDIVPD_SHIFT)               /* 0x02000000 */
#define TOPCRU_HPLL_CON2_FOUTVCOPD_SHIFT                   (26U)
#define TOPCRU_HPLL_CON2_FOUTVCOPD_MASK                    (0x1U << TOPCRU_HPLL_CON2_FOUTVCOPD_SHIFT)                   /* 0x04000000 */
#define TOPCRU_HPLL_CON2_FOUT4PHASEPD_SHIFT                (27U)
#define TOPCRU_HPLL_CON2_FOUT4PHASEPD_MASK                 (0x1U << TOPCRU_HPLL_CON2_FOUT4PHASEPD_SHIFT)                /* 0x08000000 */
/* HPLL_CON3 */
#define TOPCRU_HPLL_CON3_OFFSET                            (0xACU)
#define TOPCRU_HPLL_CON3_SSMOD_BP_SHIFT                    (0U)
#define TOPCRU_HPLL_CON3_SSMOD_BP_MASK                     (0x1U << TOPCRU_HPLL_CON3_SSMOD_BP_SHIFT)                    /* 0x00000001 */
#define TOPCRU_HPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT          (1U)
#define TOPCRU_HPLL_CON3_SSMOD_DISABLE_SSCG_MASK           (0x1U << TOPCRU_HPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)          /* 0x00000002 */
#define TOPCRU_HPLL_CON3_SSMOD_RESET_SHIFT                 (2U)
#define TOPCRU_HPLL_CON3_SSMOD_RESET_MASK                  (0x1U << TOPCRU_HPLL_CON3_SSMOD_RESET_SHIFT)                 /* 0x00000004 */
#define TOPCRU_HPLL_CON3_SSMOD_DOWNSPREAD_SHIFT            (3U)
#define TOPCRU_HPLL_CON3_SSMOD_DOWNSPREAD_MASK             (0x1U << TOPCRU_HPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)            /* 0x00000008 */
#define TOPCRU_HPLL_CON3_SSMOD_DIVVAL_SHIFT                (4U)
#define TOPCRU_HPLL_CON3_SSMOD_DIVVAL_MASK                 (0xFU << TOPCRU_HPLL_CON3_SSMOD_DIVVAL_SHIFT)                /* 0x000000F0 */
#define TOPCRU_HPLL_CON3_SSMOD_SPREAD_SHIFT                (8U)
#define TOPCRU_HPLL_CON3_SSMOD_SPREAD_MASK                 (0x1FU << TOPCRU_HPLL_CON3_SSMOD_SPREAD_SHIFT)               /* 0x00001F00 */
/* HPLL_CON4 */
#define TOPCRU_HPLL_CON4_OFFSET                            (0xB0U)
#define TOPCRU_HPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT          (0U)
#define TOPCRU_HPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK           (0x1U << TOPCRU_HPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)          /* 0x00000001 */
#define TOPCRU_HPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT           (8U)
#define TOPCRU_HPLL_CON4_SSMOD_EXT_MAXADDR_MASK            (0xFFU << TOPCRU_HPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)          /* 0x0000FF00 */
/* CLKSEL_CON00 */
#define TOPCRU_CLKSEL_CON00_OFFSET                         (0x100U)
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_50M_SRC_DIV_SHIFT   (0U)
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_50M_SRC_DIV_MASK    (0x1FU << TOPCRU_CLKSEL_CON00_CLK_MATRIX_50M_SRC_DIV_SHIFT)  /* 0x0000001F */
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_50M_SRC_SEL_SHIFT   (7U)
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_50M_SRC_SEL_MASK    (0x1U << TOPCRU_CLKSEL_CON00_CLK_MATRIX_50M_SRC_SEL_SHIFT)   /* 0x00000080 */
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_100M_SRC_DIV_SHIFT  (8U)
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_100M_SRC_DIV_MASK   (0xFU << TOPCRU_CLKSEL_CON00_CLK_MATRIX_100M_SRC_DIV_SHIFT)  /* 0x00000F00 */
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_100M_SRC_SEL_SHIFT  (15U)
#define TOPCRU_CLKSEL_CON00_CLK_MATRIX_100M_SRC_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON00_CLK_MATRIX_100M_SRC_SEL_SHIFT)  /* 0x00008000 */
/* CLKSEL_CON01 */
#define TOPCRU_CLKSEL_CON01_OFFSET                         (0x104U)
#define TOPCRU_CLKSEL_CON01_CLK_MATRIX_125M_SRC_DIV_SHIFT  (0U)
#define TOPCRU_CLKSEL_CON01_CLK_MATRIX_125M_SRC_DIV_MASK   (0xFU << TOPCRU_CLKSEL_CON01_CLK_MATRIX_125M_SRC_DIV_SHIFT)  /* 0x0000000F */
#define TOPCRU_CLKSEL_CON01_CLK_MATRIX_125M_SRC_SEL_SHIFT  (7U)
#define TOPCRU_CLKSEL_CON01_CLK_MATRIX_125M_SRC_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON01_CLK_MATRIX_125M_SRC_SEL_SHIFT)  /* 0x00000080 */
/* CLKSEL_CON02 */
#define TOPCRU_CLKSEL_CON02_OFFSET                         (0x108U)
#define TOPCRU_CLKSEL_CON02_CLK_MATRIX_200M_SRC_DIV_SHIFT  (0U)
#define TOPCRU_CLKSEL_CON02_CLK_MATRIX_200M_SRC_DIV_MASK   (0xFU << TOPCRU_CLKSEL_CON02_CLK_MATRIX_200M_SRC_DIV_SHIFT)  /* 0x0000000F */
#define TOPCRU_CLKSEL_CON02_CLK_MATRIX_200M_SRC_SEL_SHIFT  (7U)
#define TOPCRU_CLKSEL_CON02_CLK_MATRIX_200M_SRC_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON02_CLK_MATRIX_200M_SRC_SEL_SHIFT)  /* 0x00000080 */
/* CLKSEL_CON03 */
#define TOPCRU_CLKSEL_CON03_OFFSET                         (0x10CU)
#define TOPCRU_CLKSEL_CON03_CLK_MATRIX_300M_SRC_DIV_SHIFT  (0U)
#define TOPCRU_CLKSEL_CON03_CLK_MATRIX_300M_SRC_DIV_MASK   (0xFU << TOPCRU_CLKSEL_CON03_CLK_MATRIX_300M_SRC_DIV_SHIFT)  /* 0x0000000F */
#define TOPCRU_CLKSEL_CON03_CLK_MATRIX_300M_SRC_SEL_SHIFT  (7U)
#define TOPCRU_CLKSEL_CON03_CLK_MATRIX_300M_SRC_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON03_CLK_MATRIX_300M_SRC_SEL_SHIFT)  /* 0x00000080 */
/* CLKSEL_CON04 */
#define TOPCRU_CLKSEL_CON04_OFFSET                         (0x110U)
#define TOPCRU_CLKSEL_CON04_CLK_TESTOUT_TOP_DIV_SHIFT      (0U)
#define TOPCRU_CLKSEL_CON04_CLK_TESTOUT_TOP_DIV_MASK       (0xFFU << TOPCRU_CLKSEL_CON04_CLK_TESTOUT_TOP_DIV_SHIFT)     /* 0x000000FF */
#define TOPCRU_CLKSEL_CON04_CLK_TESTOUT_TOP_SEL_SHIFT      (8U)
#define TOPCRU_CLKSEL_CON04_CLK_TESTOUT_TOP_SEL_MASK       (0x1FU << TOPCRU_CLKSEL_CON04_CLK_TESTOUT_TOP_SEL_SHIFT)     /* 0x00001F00 */
/* CLKSEL_CON05 */
#define TOPCRU_CLKSEL_CON05_OFFSET                         (0x114U)
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_BIU_DIV_SHIFT         (0U)
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_BIU_DIV_MASK          (0xFU << TOPCRU_CLKSEL_CON05_ACLK_TOP_BIU_DIV_SHIFT)         /* 0x0000000F */
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_BIU_SEL_SHIFT         (7U)
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_BIU_SEL_MASK          (0x1U << TOPCRU_CLKSEL_CON05_ACLK_TOP_BIU_SEL_SHIFT)         /* 0x00000080 */
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_VIO_BIU_DIV_SHIFT     (8U)
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_VIO_BIU_DIV_MASK      (0xFU << TOPCRU_CLKSEL_CON05_ACLK_TOP_VIO_BIU_DIV_SHIFT)     /* 0x00000F00 */
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_VIO_BIU_SEL_SHIFT     (15U)
#define TOPCRU_CLKSEL_CON05_ACLK_TOP_VIO_BIU_SEL_MASK      (0x1U << TOPCRU_CLKSEL_CON05_ACLK_TOP_VIO_BIU_SEL_SHIFT)     /* 0x00008000 */
/* CLKSEL_CON06 */
#define TOPCRU_CLKSEL_CON06_OFFSET                         (0x118U)
#define TOPCRU_CLKSEL_CON06_CLK_24M_SSCSRC_DIV_SHIFT       (0U)
#define TOPCRU_CLKSEL_CON06_CLK_24M_SSCSRC_DIV_MASK        (0x3FU << TOPCRU_CLKSEL_CON06_CLK_24M_SSCSRC_DIV_SHIFT)      /* 0x0000003F */
#define TOPCRU_CLKSEL_CON06_CLK_24M_SSCSRC_SEL_SHIFT       (6U)
#define TOPCRU_CLKSEL_CON06_CLK_24M_SSCSRC_SEL_MASK        (0x3U << TOPCRU_CLKSEL_CON06_CLK_24M_SSCSRC_SEL_SHIFT)       /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON06_CLK_PVTPLL_LOGIC_O_SEL_SHIFT   (8U)
#define TOPCRU_CLKSEL_CON06_CLK_PVTPLL_LOGIC_O_SEL_MASK    (0x1U << TOPCRU_CLKSEL_CON06_CLK_PVTPLL_LOGIC_O_SEL_SHIFT)   /* 0x00000100 */
/* CLKSEL_CON08 */
#define TOPCRU_CLKSEL_CON08_OFFSET                         (0x120U)
#define TOPCRU_CLKSEL_CON08_CLK_CAM0_OUT2IO_DIV_SHIFT      (0U)
#define TOPCRU_CLKSEL_CON08_CLK_CAM0_OUT2IO_DIV_MASK       (0x3FU << TOPCRU_CLKSEL_CON08_CLK_CAM0_OUT2IO_DIV_SHIFT)     /* 0x0000003F */
#define TOPCRU_CLKSEL_CON08_CLK_CAM0_OUT2IO_SEL_SHIFT      (6U)
#define TOPCRU_CLKSEL_CON08_CLK_CAM0_OUT2IO_SEL_MASK       (0x3U << TOPCRU_CLKSEL_CON08_CLK_CAM0_OUT2IO_SEL_SHIFT)      /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON08_CLK_CAM1_OUT2IO_DIV_SHIFT      (8U)
#define TOPCRU_CLKSEL_CON08_CLK_CAM1_OUT2IO_DIV_MASK       (0x3FU << TOPCRU_CLKSEL_CON08_CLK_CAM1_OUT2IO_DIV_SHIFT)     /* 0x00003F00 */
#define TOPCRU_CLKSEL_CON08_CLK_CAM1_OUT2IO_SEL_SHIFT      (14U)
#define TOPCRU_CLKSEL_CON08_CLK_CAM1_OUT2IO_SEL_MASK       (0x3U << TOPCRU_CLKSEL_CON08_CLK_CAM1_OUT2IO_SEL_SHIFT)      /* 0x0000C000 */
/* CLKSEL_CON09 */
#define TOPCRU_CLKSEL_CON09_OFFSET                         (0x124U)
#define TOPCRU_CLKSEL_CON09_CLK_CAM2_OUT2IO_DIV_SHIFT      (0U)
#define TOPCRU_CLKSEL_CON09_CLK_CAM2_OUT2IO_DIV_MASK       (0x3FU << TOPCRU_CLKSEL_CON09_CLK_CAM2_OUT2IO_DIV_SHIFT)     /* 0x0000003F */
#define TOPCRU_CLKSEL_CON09_CLK_CAM2_OUT2IO_SEL_SHIFT      (6U)
#define TOPCRU_CLKSEL_CON09_CLK_CAM2_OUT2IO_SEL_MASK       (0x3U << TOPCRU_CLKSEL_CON09_CLK_CAM2_OUT2IO_SEL_SHIFT)      /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON09_CLK_CAM3_OUT2IO_DIV_SHIFT      (8U)
#define TOPCRU_CLKSEL_CON09_CLK_CAM3_OUT2IO_DIV_MASK       (0x3FU << TOPCRU_CLKSEL_CON09_CLK_CAM3_OUT2IO_DIV_SHIFT)     /* 0x00003F00 */
#define TOPCRU_CLKSEL_CON09_CLK_CAM3_OUT2IO_SEL_SHIFT      (14U)
#define TOPCRU_CLKSEL_CON09_CLK_CAM3_OUT2IO_SEL_MASK       (0x3U << TOPCRU_CLKSEL_CON09_CLK_CAM3_OUT2IO_SEL_SHIFT)      /* 0x0000C000 */
/* CLKSEL_CON10 */
#define TOPCRU_CLKSEL_CON10_OFFSET                         (0x128U)
#define TOPCRU_CLKSEL_CON10_CLK_CORE_LEV1PRE_DIV_SHIFT     (0U)
#define TOPCRU_CLKSEL_CON10_CLK_CORE_LEV1PRE_DIV_MASK      (0x1FU << TOPCRU_CLKSEL_CON10_CLK_CORE_LEV1PRE_DIV_SHIFT)    /* 0x0000001F */
#define TOPCRU_CLKSEL_CON10_CLK_CORE_LEV1PRE_SEL_SHIFT     (7U)
#define TOPCRU_CLKSEL_CON10_CLK_CORE_LEV1PRE_SEL_MASK      (0x1U << TOPCRU_CLKSEL_CON10_CLK_CORE_LEV1PRE_SEL_SHIFT)     /* 0x00000080 */
#define TOPCRU_CLKSEL_CON10_CLK_CORE_PVTPLL_SRC_SEL_SHIFT  (8U)
#define TOPCRU_CLKSEL_CON10_CLK_CORE_PVTPLL_SRC_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON10_CLK_CORE_PVTPLL_SRC_SEL_SHIFT)  /* 0x00000100 */
#define TOPCRU_CLKSEL_CON10_CLK_CORE_PRE_NDFT_SEL_SHIFT    (14U)
#define TOPCRU_CLKSEL_CON10_CLK_CORE_PRE_NDFT_SEL_MASK     (0x1U << TOPCRU_CLKSEL_CON10_CLK_CORE_PRE_NDFT_SEL_SHIFT)    /* 0x00004000 */
#define TOPCRU_CLKSEL_CON10_CLK_CORE_I_SEL_SHIFT           (15U)
#define TOPCRU_CLKSEL_CON10_CLK_CORE_I_SEL_MASK            (0x1U << TOPCRU_CLKSEL_CON10_CLK_CORE_I_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL_CON11 */
#define TOPCRU_CLKSEL_CON11_OFFSET                         (0x12CU)
#define TOPCRU_CLKSEL_CON11_ACLK_CORE_PRE_DIV_SHIFT        (0U)
#define TOPCRU_CLKSEL_CON11_ACLK_CORE_PRE_DIV_MASK         (0x7U << TOPCRU_CLKSEL_CON11_ACLK_CORE_PRE_DIV_SHIFT)        /* 0x00000007 */
#define TOPCRU_CLKSEL_CON11_CLK_SCANHS_ACLKM_CORE_DIV_SHIFT (8U)
#define TOPCRU_CLKSEL_CON11_CLK_SCANHS_ACLKM_CORE_DIV_MASK (0x7U << TOPCRU_CLKSEL_CON11_CLK_SCANHS_ACLKM_CORE_DIV_SHIFT) /* 0x00000700 */
/* CLKSEL_CON12 */
#define TOPCRU_CLKSEL_CON12_OFFSET                         (0x130U)
#define TOPCRU_CLKSEL_CON12_PCLK_DBG_PRE_DIV_SHIFT         (0U)
#define TOPCRU_CLKSEL_CON12_PCLK_DBG_PRE_DIV_MASK          (0xFU << TOPCRU_CLKSEL_CON12_PCLK_DBG_PRE_DIV_SHIFT)         /* 0x0000000F */
#define TOPCRU_CLKSEL_CON12_CLK_SCANHS_PCLK_DBG_DIV_SHIFT  (8U)
#define TOPCRU_CLKSEL_CON12_CLK_SCANHS_PCLK_DBG_DIV_MASK   (0xFU << TOPCRU_CLKSEL_CON12_CLK_SCANHS_PCLK_DBG_DIV_SHIFT)  /* 0x00000F00 */
/* CLKSEL_CON13 */
#define TOPCRU_CLKSEL_CON13_OFFSET                         (0x134U)
#define TOPCRU_CLKSEL_CON13_HCLK_CORE_BIU_DIV_SHIFT        (0U)
#define TOPCRU_CLKSEL_CON13_HCLK_CORE_BIU_DIV_MASK         (0x3FU << TOPCRU_CLKSEL_CON13_HCLK_CORE_BIU_DIV_SHIFT)       /* 0x0000003F */
/* CLKSEL_CON14 */
#define TOPCRU_CLKSEL_CON14_OFFSET                         (0x138U)
#define TOPCRU_CLKSEL_CON14_CLK_TESTOUT_CORE_DIV_SHIFT     (8U)
#define TOPCRU_CLKSEL_CON14_CLK_TESTOUT_CORE_DIV_MASK      (0x7U << TOPCRU_CLKSEL_CON14_CLK_TESTOUT_CORE_DIV_SHIFT)     /* 0x00000700 */
#define TOPCRU_CLKSEL_CON14_CLK_TESTOUT_CORE_SEL_SHIFT     (15U)
#define TOPCRU_CLKSEL_CON14_CLK_TESTOUT_CORE_SEL_MASK      (0x1U << TOPCRU_CLKSEL_CON14_CLK_TESTOUT_CORE_SEL_SHIFT)     /* 0x00008000 */
/* CLKSEL_CON15 */
#define TOPCRU_CLKSEL_CON15_OFFSET                         (0x13CU)
#define TOPCRU_CLKSEL_CON15_CLK_NPU_PRE_DIV_SHIFT          (0U)
#define TOPCRU_CLKSEL_CON15_CLK_NPU_PRE_DIV_MASK           (0xFU << TOPCRU_CLKSEL_CON15_CLK_NPU_PRE_DIV_SHIFT)          /* 0x0000000F */
#define TOPCRU_CLKSEL_CON15_CLK_NPU_PRE_SEL_SHIFT          (7U)
#define TOPCRU_CLKSEL_CON15_CLK_NPU_PRE_SEL_MASK           (0x1U << TOPCRU_CLKSEL_CON15_CLK_NPU_PRE_SEL_SHIFT)          /* 0x00000080 */
#define TOPCRU_CLKSEL_CON15_CLK_NPU_PVTPLL_SRC_SEL_SHIFT   (8U)
#define TOPCRU_CLKSEL_CON15_CLK_NPU_PVTPLL_SRC_SEL_MASK    (0x1U << TOPCRU_CLKSEL_CON15_CLK_NPU_PVTPLL_SRC_SEL_SHIFT)   /* 0x00000100 */
#define TOPCRU_CLKSEL_CON15_CLK_NPU_I_SEL_SHIFT            (15U)
#define TOPCRU_CLKSEL_CON15_CLK_NPU_I_SEL_MASK             (0x1U << TOPCRU_CLKSEL_CON15_CLK_NPU_I_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON16 */
#define TOPCRU_CLKSEL_CON16_OFFSET                         (0x140U)
#define TOPCRU_CLKSEL_CON16_HCLK_NPU_PRE_DIV_SHIFT         (0U)
#define TOPCRU_CLKSEL_CON16_HCLK_NPU_PRE_DIV_MASK          (0xFU << TOPCRU_CLKSEL_CON16_HCLK_NPU_PRE_DIV_SHIFT)         /* 0x0000000F */
/* CLKSEL_CON17 */
#define TOPCRU_CLKSEL_CON17_OFFSET                         (0x144U)
#define TOPCRU_CLKSEL_CON17_CLK_TESTOUT_NPU_DIV_SHIFT      (0U)
#define TOPCRU_CLKSEL_CON17_CLK_TESTOUT_NPU_DIV_MASK       (0x7U << TOPCRU_CLKSEL_CON17_CLK_TESTOUT_NPU_DIV_SHIFT)      /* 0x00000007 */
#define TOPCRU_CLKSEL_CON17_CLK_TESTOUT_NPU_SEL_SHIFT      (7U)
#define TOPCRU_CLKSEL_CON17_CLK_TESTOUT_NPU_SEL_MASK       (0x1U << TOPCRU_CLKSEL_CON17_CLK_TESTOUT_NPU_SEL_SHIFT)      /* 0x00000080 */
/* CLKSEL_CON18 */
#define TOPCRU_CLKSEL_CON18_OFFSET                         (0x148U)
#define TOPCRU_CLKSEL_CON18_CLK_GPU_LEV1PRE_DIV_SHIFT      (0U)
#define TOPCRU_CLKSEL_CON18_CLK_GPU_LEV1PRE_DIV_MASK       (0xFU << TOPCRU_CLKSEL_CON18_CLK_GPU_LEV1PRE_DIV_SHIFT)      /* 0x0000000F */
#define TOPCRU_CLKSEL_CON18_CLK_GPU_LEV1PRE_SEL_SHIFT      (7U)
#define TOPCRU_CLKSEL_CON18_CLK_GPU_LEV1PRE_SEL_MASK       (0x1U << TOPCRU_CLKSEL_CON18_CLK_GPU_LEV1PRE_SEL_SHIFT)      /* 0x00000080 */
#define TOPCRU_CLKSEL_CON18_CLK_GPU_PVTPLL_SRC_SEL_SHIFT   (8U)
#define TOPCRU_CLKSEL_CON18_CLK_GPU_PVTPLL_SRC_SEL_MASK    (0x1U << TOPCRU_CLKSEL_CON18_CLK_GPU_PVTPLL_SRC_SEL_SHIFT)   /* 0x00000100 */
#define TOPCRU_CLKSEL_CON18_CLK_GPU_I_SEL_SHIFT            (15U)
#define TOPCRU_CLKSEL_CON18_CLK_GPU_I_SEL_MASK             (0x1U << TOPCRU_CLKSEL_CON18_CLK_GPU_I_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON19 */
#define TOPCRU_CLKSEL_CON19_OFFSET                         (0x14CU)
#define TOPCRU_CLKSEL_CON19_ACLK_GPU_PRE_DIV_SHIFT         (0U)
#define TOPCRU_CLKSEL_CON19_ACLK_GPU_PRE_DIV_MASK          (0xFU << TOPCRU_CLKSEL_CON19_ACLK_GPU_PRE_DIV_SHIFT)         /* 0x0000000F */
#define TOPCRU_CLKSEL_CON19_CLK_TESTOUT_GPU_DIV_SHIFT      (4U)
#define TOPCRU_CLKSEL_CON19_CLK_TESTOUT_GPU_DIV_MASK       (0x7U << TOPCRU_CLKSEL_CON19_CLK_TESTOUT_GPU_DIV_SHIFT)      /* 0x00000070 */
#define TOPCRU_CLKSEL_CON19_CLK_TESTOUT_GPU_SEL_SHIFT      (7U)
#define TOPCRU_CLKSEL_CON19_CLK_TESTOUT_GPU_SEL_MASK       (0x1U << TOPCRU_CLKSEL_CON19_CLK_TESTOUT_GPU_SEL_SHIFT)      /* 0x00000080 */
#define TOPCRU_CLKSEL_CON19_CLK_GPU_BRG_BIU_SEL_SHIFT      (15U)
#define TOPCRU_CLKSEL_CON19_CLK_GPU_BRG_BIU_SEL_MASK       (0x1U << TOPCRU_CLKSEL_CON19_CLK_GPU_BRG_BIU_SEL_SHIFT)      /* 0x00008000 */
/* CLKSEL_CON20 */
#define TOPCRU_CLKSEL_CON20_OFFSET                         (0x150U)
#define TOPCRU_CLKSEL_CON20_CLK_RKVENC_CORE_DIV_SHIFT      (0U)
#define TOPCRU_CLKSEL_CON20_CLK_RKVENC_CORE_DIV_MASK       (0x1FU << TOPCRU_CLKSEL_CON20_CLK_RKVENC_CORE_DIV_SHIFT)     /* 0x0000001F */
#define TOPCRU_CLKSEL_CON20_CLK_RKVENC_CORE_SEL_SHIFT      (6U)
#define TOPCRU_CLKSEL_CON20_CLK_RKVENC_CORE_SEL_MASK       (0x3U << TOPCRU_CLKSEL_CON20_CLK_RKVENC_CORE_SEL_SHIFT)      /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON20_ACLK_VEPU_PRE_DIV_SHIFT        (8U)
#define TOPCRU_CLKSEL_CON20_ACLK_VEPU_PRE_DIV_MASK         (0x1FU << TOPCRU_CLKSEL_CON20_ACLK_VEPU_PRE_DIV_SHIFT)       /* 0x00001F00 */
#define TOPCRU_CLKSEL_CON20_ACLK_VEPU_PRE_SEL_SHIFT        (14U)
#define TOPCRU_CLKSEL_CON20_ACLK_VEPU_PRE_SEL_MASK         (0x3U << TOPCRU_CLKSEL_CON20_ACLK_VEPU_PRE_SEL_SHIFT)        /* 0x0000C000 */
/* CLKSEL_CON21 */
#define TOPCRU_CLKSEL_CON21_OFFSET                         (0x154U)
#define TOPCRU_CLKSEL_CON21_HCLK_VEPU_PRE_DIV_SHIFT        (0U)
#define TOPCRU_CLKSEL_CON21_HCLK_VEPU_PRE_DIV_MASK         (0xFU << TOPCRU_CLKSEL_CON21_HCLK_VEPU_PRE_DIV_SHIFT)        /* 0x0000000F */
#define TOPCRU_CLKSEL_CON21_CLK_TESTOUT_VEPU_SEL_SHIFT     (15U)
#define TOPCRU_CLKSEL_CON21_CLK_TESTOUT_VEPU_SEL_MASK      (0x1U << TOPCRU_CLKSEL_CON21_CLK_TESTOUT_VEPU_SEL_SHIFT)     /* 0x00008000 */
/* CLKSEL_CON22 */
#define TOPCRU_CLKSEL_CON22_OFFSET                         (0x158U)
#define TOPCRU_CLKSEL_CON22_ACLK_VDPU_PRE_DIV_SHIFT        (0U)
#define TOPCRU_CLKSEL_CON22_ACLK_VDPU_PRE_DIV_MASK         (0x1FU << TOPCRU_CLKSEL_CON22_ACLK_VDPU_PRE_DIV_SHIFT)       /* 0x0000001F */
#define TOPCRU_CLKSEL_CON22_ACLK_VDPU_PRE_SEL_SHIFT        (6U)
#define TOPCRU_CLKSEL_CON22_ACLK_VDPU_PRE_SEL_MASK         (0x3U << TOPCRU_CLKSEL_CON22_ACLK_VDPU_PRE_SEL_SHIFT)        /* 0x000000C0 */
/* CLKSEL_CON23 */
#define TOPCRU_CLKSEL_CON23_OFFSET                         (0x15CU)
#define TOPCRU_CLKSEL_CON23_CLK_RKVDEC_HEVC_CA_DIV_SHIFT   (8U)
#define TOPCRU_CLKSEL_CON23_CLK_RKVDEC_HEVC_CA_DIV_MASK    (0x1FU << TOPCRU_CLKSEL_CON23_CLK_RKVDEC_HEVC_CA_DIV_SHIFT)  /* 0x00001F00 */
#define TOPCRU_CLKSEL_CON23_CLK_RKVDEC_HEVC_CA_SEL_SHIFT   (14U)
#define TOPCRU_CLKSEL_CON23_CLK_RKVDEC_HEVC_CA_SEL_MASK    (0x3U << TOPCRU_CLKSEL_CON23_CLK_RKVDEC_HEVC_CA_SEL_SHIFT)   /* 0x0000C000 */
/* CLKSEL_CON24 */
#define TOPCRU_CLKSEL_CON24_OFFSET                         (0x160U)
#define TOPCRU_CLKSEL_CON24_HCLK_VDPU_PRE_DIV_SHIFT        (0U)
#define TOPCRU_CLKSEL_CON24_HCLK_VDPU_PRE_DIV_MASK         (0xFU << TOPCRU_CLKSEL_CON24_HCLK_VDPU_PRE_DIV_SHIFT)        /* 0x0000000F */
#define TOPCRU_CLKSEL_CON24_CLK_TESTOUT_VDPU_SEL_SHIFT     (15U)
#define TOPCRU_CLKSEL_CON24_CLK_TESTOUT_VDPU_SEL_MASK      (0x1U << TOPCRU_CLKSEL_CON24_CLK_TESTOUT_VDPU_SEL_SHIFT)     /* 0x00008000 */
/* CLKSEL_CON25 */
#define TOPCRU_CLKSEL_CON25_OFFSET                         (0x164U)
#define TOPCRU_CLKSEL_CON25_ACLK_VI_DIV_SHIFT              (0U)
#define TOPCRU_CLKSEL_CON25_ACLK_VI_DIV_MASK               (0xFU << TOPCRU_CLKSEL_CON25_ACLK_VI_DIV_SHIFT)              /* 0x0000000F */
#define TOPCRU_CLKSEL_CON25_ACLK_VI_SEL_SHIFT              (6U)
#define TOPCRU_CLKSEL_CON25_ACLK_VI_SEL_MASK               (0x3U << TOPCRU_CLKSEL_CON25_ACLK_VI_SEL_SHIFT)              /* 0x000000C0 */
/* CLKSEL_CON26 */
#define TOPCRU_CLKSEL_CON26_OFFSET                         (0x168U)
#define TOPCRU_CLKSEL_CON26_HCLK_VI_DIV_SHIFT              (0U)
#define TOPCRU_CLKSEL_CON26_HCLK_VI_DIV_MASK               (0xFU << TOPCRU_CLKSEL_CON26_HCLK_VI_DIV_SHIFT)              /* 0x0000000F */
#define TOPCRU_CLKSEL_CON26_PCLK_VI_DIV_SHIFT              (8U)
#define TOPCRU_CLKSEL_CON26_PCLK_VI_DIV_MASK               (0xFU << TOPCRU_CLKSEL_CON26_PCLK_VI_DIV_SHIFT)              /* 0x00000F00 */
#define TOPCRU_CLKSEL_CON26_CLK_TESTOUT_VIPHY_SEL_SHIFT    (12U)
#define TOPCRU_CLKSEL_CON26_CLK_TESTOUT_VIPHY_SEL_MASK     (0xFU << TOPCRU_CLKSEL_CON26_CLK_TESTOUT_VIPHY_SEL_SHIFT)    /* 0x0000F000 */
/* CLKSEL_CON27 */
#define TOPCRU_CLKSEL_CON27_OFFSET                         (0x16CU)
#define TOPCRU_CLKSEL_CON27_CLK_ISP_DIV_SHIFT              (0U)
#define TOPCRU_CLKSEL_CON27_CLK_ISP_DIV_MASK               (0xFU << TOPCRU_CLKSEL_CON27_CLK_ISP_DIV_SHIFT)              /* 0x0000000F */
#define TOPCRU_CLKSEL_CON27_CLK_ISP_SEL_SHIFT              (6U)
#define TOPCRU_CLKSEL_CON27_CLK_ISP_SEL_MASK               (0x3U << TOPCRU_CLKSEL_CON27_CLK_ISP_SEL_SHIFT)              /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON27_DCLK_VICAP_DIV_SHIFT           (8U)
#define TOPCRU_CLKSEL_CON27_DCLK_VICAP_DIV_MASK            (0xFU << TOPCRU_CLKSEL_CON27_DCLK_VICAP_DIV_SHIFT)           /* 0x00000F00 */
#define TOPCRU_CLKSEL_CON27_DCLK_VICAP_SEL_SHIFT           (14U)
#define TOPCRU_CLKSEL_CON27_DCLK_VICAP_SEL_MASK            (0x3U << TOPCRU_CLKSEL_CON27_DCLK_VICAP_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON28 */
#define TOPCRU_CLKSEL_CON28_OFFSET                         (0x170U)
#define TOPCRU_CLKSEL_CON28_ACLK_VO_PRE_DIV_SHIFT          (0U)
#define TOPCRU_CLKSEL_CON28_ACLK_VO_PRE_DIV_MASK           (0x1FU << TOPCRU_CLKSEL_CON28_ACLK_VO_PRE_DIV_SHIFT)         /* 0x0000001F */
#define TOPCRU_CLKSEL_CON28_ACLK_VO_PRE_SEL_SHIFT          (6U)
#define TOPCRU_CLKSEL_CON28_ACLK_VO_PRE_SEL_MASK           (0x3U << TOPCRU_CLKSEL_CON28_ACLK_VO_PRE_SEL_SHIFT)          /* 0x000000C0 */
/* CLKSEL_CON29 */
#define TOPCRU_CLKSEL_CON29_OFFSET                         (0x174U)
#define TOPCRU_CLKSEL_CON29_HCLK_VO_PRE_DIV_SHIFT          (0U)
#define TOPCRU_CLKSEL_CON29_HCLK_VO_PRE_DIV_MASK           (0x1FU << TOPCRU_CLKSEL_CON29_HCLK_VO_PRE_DIV_SHIFT)         /* 0x0000001F */
/* CLKSEL_CON30 */
#define TOPCRU_CLKSEL_CON30_OFFSET                         (0x178U)
#define TOPCRU_CLKSEL_CON30_DCLK_VOP_DIV_SHIFT             (0U)
#define TOPCRU_CLKSEL_CON30_DCLK_VOP_DIV_MASK              (0xFFU << TOPCRU_CLKSEL_CON30_DCLK_VOP_DIV_SHIFT)            /* 0x000000FF */
#define TOPCRU_CLKSEL_CON30_DCLK_VOP_SEL_SHIFT             (14U)
#define TOPCRU_CLKSEL_CON30_DCLK_VOP_SEL_MASK              (0x3U << TOPCRU_CLKSEL_CON30_DCLK_VOP_SEL_SHIFT)             /* 0x0000C000 */
/* CLKSEL_CON31 */
#define TOPCRU_CLKSEL_CON31_OFFSET                         (0x17CU)
#define TOPCRU_CLKSEL_CON31_DCLK_VOP1_DIV_SHIFT            (0U)
#define TOPCRU_CLKSEL_CON31_DCLK_VOP1_DIV_MASK             (0xFFU << TOPCRU_CLKSEL_CON31_DCLK_VOP1_DIV_SHIFT)           /* 0x000000FF */
#define TOPCRU_CLKSEL_CON31_CLK_TESTOUT_VO_SEL_SHIFT       (8U)
#define TOPCRU_CLKSEL_CON31_CLK_TESTOUT_VO_SEL_MASK        (0x3U << TOPCRU_CLKSEL_CON31_CLK_TESTOUT_VO_SEL_SHIFT)       /* 0x00000300 */
#define TOPCRU_CLKSEL_CON31_DCLK_VOP1_SEL_SHIFT            (14U)
#define TOPCRU_CLKSEL_CON31_DCLK_VOP1_SEL_MASK             (0x3U << TOPCRU_CLKSEL_CON31_DCLK_VOP1_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON32 */
#define TOPCRU_CLKSEL_CON32_OFFSET                         (0x180U)
#define TOPCRU_CLKSEL_CON32_ACLK_RGA_PRE_DIV_SHIFT         (0U)
#define TOPCRU_CLKSEL_CON32_ACLK_RGA_PRE_DIV_MASK          (0xFU << TOPCRU_CLKSEL_CON32_ACLK_RGA_PRE_DIV_SHIFT)         /* 0x0000000F */
#define TOPCRU_CLKSEL_CON32_ACLK_RGA_PRE_SEL_SHIFT         (6U)
#define TOPCRU_CLKSEL_CON32_ACLK_RGA_PRE_SEL_MASK          (0x3U << TOPCRU_CLKSEL_CON32_ACLK_RGA_PRE_SEL_SHIFT)         /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON32_HCLK_RGA_PRE_DIV_SHIFT         (8U)
#define TOPCRU_CLKSEL_CON32_HCLK_RGA_PRE_DIV_MASK          (0x7U << TOPCRU_CLKSEL_CON32_HCLK_RGA_PRE_DIV_SHIFT)         /* 0x00000700 */
/* CLKSEL_CON33 */
#define TOPCRU_CLKSEL_CON33_OFFSET                         (0x184U)
#define TOPCRU_CLKSEL_CON33_CLK_RGA_CORE_DIV_SHIFT         (0U)
#define TOPCRU_CLKSEL_CON33_CLK_RGA_CORE_DIV_MASK          (0xFU << TOPCRU_CLKSEL_CON33_CLK_RGA_CORE_DIV_SHIFT)         /* 0x0000000F */
#define TOPCRU_CLKSEL_CON33_CLK_RGA_CORE_SEL_SHIFT         (6U)
#define TOPCRU_CLKSEL_CON33_CLK_RGA_CORE_SEL_MASK          (0x3U << TOPCRU_CLKSEL_CON33_CLK_RGA_CORE_SEL_SHIFT)         /* 0x000000C0 */
#define TOPCRU_CLKSEL_CON33_CLK_TESTOUT_RGA_SEL_SHIFT      (15U)
#define TOPCRU_CLKSEL_CON33_CLK_TESTOUT_RGA_SEL_MASK       (0x1U << TOPCRU_CLKSEL_CON33_CLK_TESTOUT_RGA_SEL_SHIFT)      /* 0x00008000 */
/* CLKSEL_CON36 */
#define TOPCRU_CLKSEL_CON36_OFFSET                         (0x190U)
#define TOPCRU_CLKSEL_CON36_ACLK_PHP_DIV_SHIFT             (0U)
#define TOPCRU_CLKSEL_CON36_ACLK_PHP_DIV_MASK              (0xFU << TOPCRU_CLKSEL_CON36_ACLK_PHP_DIV_SHIFT)             /* 0x0000000F */
#define TOPCRU_CLKSEL_CON36_ACLK_PHP_SEL_SHIFT             (7U)
#define TOPCRU_CLKSEL_CON36_ACLK_PHP_SEL_MASK              (0x1U << TOPCRU_CLKSEL_CON36_ACLK_PHP_SEL_SHIFT)             /* 0x00000080 */
#define TOPCRU_CLKSEL_CON36_PCLK_PHP_DIV_SHIFT             (8U)
#define TOPCRU_CLKSEL_CON36_PCLK_PHP_DIV_MASK              (0xFU << TOPCRU_CLKSEL_CON36_PCLK_PHP_DIV_SHIFT)             /* 0x00000F00 */
#define TOPCRU_CLKSEL_CON36_CLK_USB3OTG_SUSPEND_SEL_SHIFT  (15U)
#define TOPCRU_CLKSEL_CON36_CLK_USB3OTG_SUSPEND_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON36_CLK_USB3OTG_SUSPEND_SEL_SHIFT)  /* 0x00008000 */
/* CLKSEL_CON37 */
#define TOPCRU_CLKSEL_CON37_OFFSET                         (0x194U)
#define TOPCRU_CLKSEL_CON37_CLK_TESTOUT_PHPPHY_SEL_SHIFT   (6U)
#define TOPCRU_CLKSEL_CON37_CLK_TESTOUT_PHPPHY_SEL_MASK    (0x3U << TOPCRU_CLKSEL_CON37_CLK_TESTOUT_PHPPHY_SEL_SHIFT)   /* 0x000000C0 */
/* CLKSEL_CON40 */
#define TOPCRU_CLKSEL_CON40_OFFSET                         (0x1A0U)
#define TOPCRU_CLKSEL_CON40_ACLK_BUS_DIV_SHIFT             (0U)
#define TOPCRU_CLKSEL_CON40_ACLK_BUS_DIV_MASK              (0x1FU << TOPCRU_CLKSEL_CON40_ACLK_BUS_DIV_SHIFT)            /* 0x0000001F */
#define TOPCRU_CLKSEL_CON40_ACLK_BUS_SEL_SHIFT             (7U)
#define TOPCRU_CLKSEL_CON40_ACLK_BUS_SEL_MASK              (0x1U << TOPCRU_CLKSEL_CON40_ACLK_BUS_SEL_SHIFT)             /* 0x00000080 */
#define TOPCRU_CLKSEL_CON40_HCLK_BUS_DIV_SHIFT             (8U)
#define TOPCRU_CLKSEL_CON40_HCLK_BUS_DIV_MASK              (0x3FU << TOPCRU_CLKSEL_CON40_HCLK_BUS_DIV_SHIFT)            /* 0x00003F00 */
#define TOPCRU_CLKSEL_CON40_HCLK_BUS_SEL_SHIFT             (15U)
#define TOPCRU_CLKSEL_CON40_HCLK_BUS_SEL_MASK              (0x1U << TOPCRU_CLKSEL_CON40_HCLK_BUS_SEL_SHIFT)             /* 0x00008000 */
/* CLKSEL_CON41 */
#define TOPCRU_CLKSEL_CON41_OFFSET                         (0x1A4U)
#define TOPCRU_CLKSEL_CON41_PCLK_BUS_DIV_SHIFT             (0U)
#define TOPCRU_CLKSEL_CON41_PCLK_BUS_DIV_MASK              (0x1FU << TOPCRU_CLKSEL_CON41_PCLK_BUS_DIV_SHIFT)            /* 0x0000001F */
#define TOPCRU_CLKSEL_CON41_PCLK_BUS_SEL_SHIFT             (7U)
#define TOPCRU_CLKSEL_CON41_PCLK_BUS_SEL_MASK              (0x1U << TOPCRU_CLKSEL_CON41_PCLK_BUS_SEL_SHIFT)             /* 0x00000080 */
#define TOPCRU_CLKSEL_CON41_CLK_I2C_SEL_SHIFT              (8U)
#define TOPCRU_CLKSEL_CON41_CLK_I2C_SEL_MASK               (0x3U << TOPCRU_CLKSEL_CON41_CLK_I2C_SEL_SHIFT)              /* 0x00000300 */
#define TOPCRU_CLKSEL_CON41_DCLK_BUS_GPIO_SEL_SHIFT        (15U)
#define TOPCRU_CLKSEL_CON41_DCLK_BUS_GPIO_SEL_MASK         (0x1U << TOPCRU_CLKSEL_CON41_DCLK_BUS_GPIO_SEL_SHIFT)        /* 0x00008000 */
/* CLKSEL_CON43 */
#define TOPCRU_CLKSEL_CON43_OFFSET                         (0x1ACU)
#define TOPCRU_CLKSEL_CON43_CLK_TSADC_DIV_SHIFT            (0U)
#define TOPCRU_CLKSEL_CON43_CLK_TSADC_DIV_MASK             (0x7FFU << TOPCRU_CLKSEL_CON43_CLK_TSADC_DIV_SHIFT)          /* 0x000007FF */
#define TOPCRU_CLKSEL_CON43_CLK_TSADC_TSEN_DIV_SHIFT       (11U)
#define TOPCRU_CLKSEL_CON43_CLK_TSADC_TSEN_DIV_MASK        (0x1FU << TOPCRU_CLKSEL_CON43_CLK_TSADC_TSEN_DIV_SHIFT)      /* 0x0000F800 */
/* CLKSEL_CON44 */
#define TOPCRU_CLKSEL_CON44_OFFSET                         (0x1B0U)
#define TOPCRU_CLKSEL_CON44_CLK_SARADC_TO_VCCIO156_DIV_SHIFT (0U)
#define TOPCRU_CLKSEL_CON44_CLK_SARADC_TO_VCCIO156_DIV_MASK (0xFFFU << TOPCRU_CLKSEL_CON44_CLK_SARADC_TO_VCCIO156_DIV_SHIFT) /* 0x00000FFF */
/* CLKSEL_CON45 */
#define TOPCRU_CLKSEL_CON45_OFFSET                         (0x1B4U)
#define TOPCRU_CLKSEL_CON45_CLK_GMAC_50M_CRU_I_SEL_SHIFT   (7U)
#define TOPCRU_CLKSEL_CON45_CLK_GMAC_50M_CRU_I_SEL_MASK    (0x1U << TOPCRU_CLKSEL_CON45_CLK_GMAC_50M_CRU_I_SEL_SHIFT)   /* 0x00000080 */
#define TOPCRU_CLKSEL_CON45_CLK_GMAC_125M_CRU_I_SEL_SHIFT  (8U)
#define TOPCRU_CLKSEL_CON45_CLK_GMAC_125M_CRU_I_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON45_CLK_GMAC_125M_CRU_I_SEL_SHIFT)  /* 0x00000100 */
/* CLKSEL_CON46 */
#define TOPCRU_CLKSEL_CON46_OFFSET                         (0x1B8U)
#define TOPCRU_CLKSEL_CON46_CLK_GMAC_ETH_OUT2IO_DIV_SHIFT  (0U)
#define TOPCRU_CLKSEL_CON46_CLK_GMAC_ETH_OUT2IO_DIV_MASK   (0x7FU << TOPCRU_CLKSEL_CON46_CLK_GMAC_ETH_OUT2IO_DIV_SHIFT) /* 0x0000007F */
#define TOPCRU_CLKSEL_CON46_CLK_GMAC_ETH_OUT2IO_SEL_SHIFT  (7U)
#define TOPCRU_CLKSEL_CON46_CLK_GMAC_ETH_OUT2IO_SEL_MASK   (0x1U << TOPCRU_CLKSEL_CON46_CLK_GMAC_ETH_OUT2IO_SEL_SHIFT)  /* 0x00000080 */
#define TOPCRU_CLKSEL_CON46_CLK_TESTOUT_BUS_SEL_SHIFT      (13U)
#define TOPCRU_CLKSEL_CON46_CLK_TESTOUT_BUS_SEL_MASK       (0x7U << TOPCRU_CLKSEL_CON46_CLK_TESTOUT_BUS_SEL_SHIFT)      /* 0x0000E000 */
/* CLKSEL_CON47 */
#define TOPCRU_CLKSEL_CON47_OFFSET                         (0x1BCU)
#define TOPCRU_CLKSEL_CON47_CLK_MAC100_50M_MATRIX_SEL_SHIFT (7U)
#define TOPCRU_CLKSEL_CON47_CLK_MAC100_50M_MATRIX_SEL_MASK (0x1U << TOPCRU_CLKSEL_CON47_CLK_MAC100_50M_MATRIX_SEL_SHIFT) /* 0x00000080 */
/* GATE_CON00 */
#define TOPCRU_GATE_CON00_OFFSET                           (0x300U)
#define TOPCRU_GATE_CON00_CLK_MATRIX_50M_SRC_EN_SHIFT      (0U)
#define TOPCRU_GATE_CON00_CLK_MATRIX_50M_SRC_EN_MASK       (0x1U << TOPCRU_GATE_CON00_CLK_MATRIX_50M_SRC_EN_SHIFT)      /* 0x00000001 */
#define TOPCRU_GATE_CON00_CLK_MATRIX_100M_SRC_EN_SHIFT     (1U)
#define TOPCRU_GATE_CON00_CLK_MATRIX_100M_SRC_EN_MASK      (0x1U << TOPCRU_GATE_CON00_CLK_MATRIX_100M_SRC_EN_SHIFT)     /* 0x00000002 */
#define TOPCRU_GATE_CON00_CLK_MATRIX_125M_SRC_EN_SHIFT     (2U)
#define TOPCRU_GATE_CON00_CLK_MATRIX_125M_SRC_EN_MASK      (0x1U << TOPCRU_GATE_CON00_CLK_MATRIX_125M_SRC_EN_SHIFT)     /* 0x00000004 */
#define TOPCRU_GATE_CON00_CLK_MATRIX_200M_SRC_EN_SHIFT     (4U)
#define TOPCRU_GATE_CON00_CLK_MATRIX_200M_SRC_EN_MASK      (0x1U << TOPCRU_GATE_CON00_CLK_MATRIX_200M_SRC_EN_SHIFT)     /* 0x00000010 */
#define TOPCRU_GATE_CON00_CLK_MATRIX_300M_SRC_EN_SHIFT     (6U)
#define TOPCRU_GATE_CON00_CLK_MATRIX_300M_SRC_EN_MASK      (0x1U << TOPCRU_GATE_CON00_CLK_MATRIX_300M_SRC_EN_SHIFT)     /* 0x00000040 */
#define TOPCRU_GATE_CON00_CLK_TESTOUT_TOP_EN_SHIFT         (15U)
#define TOPCRU_GATE_CON00_CLK_TESTOUT_TOP_EN_MASK          (0x1U << TOPCRU_GATE_CON00_CLK_TESTOUT_TOP_EN_SHIFT)         /* 0x00008000 */
/* GATE_CON01 */
#define TOPCRU_GATE_CON01_OFFSET                           (0x304U)
#define TOPCRU_GATE_CON01_ACLK_TOP_BIU_EN_SHIFT            (0U)
#define TOPCRU_GATE_CON01_ACLK_TOP_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON01_ACLK_TOP_BIU_EN_SHIFT)            /* 0x00000001 */
#define TOPCRU_GATE_CON01_ACLK_TOP_VIO_BIU_EN_SHIFT        (1U)
#define TOPCRU_GATE_CON01_ACLK_TOP_VIO_BIU_EN_MASK         (0x1U << TOPCRU_GATE_CON01_ACLK_TOP_VIO_BIU_EN_SHIFT)        /* 0x00000002 */
#define TOPCRU_GATE_CON01_CLK_REF_PVTPLL_LOGIC_EN_SHIFT    (2U)
#define TOPCRU_GATE_CON01_CLK_REF_PVTPLL_LOGIC_EN_MASK     (0x1U << TOPCRU_GATE_CON01_CLK_REF_PVTPLL_LOGIC_EN_SHIFT)    /* 0x00000004 */
#define TOPCRU_GATE_CON01_CLK_PVTPLL_LOGIC_O_EN_SHIFT      (3U)
#define TOPCRU_GATE_CON01_CLK_PVTPLL_LOGIC_O_EN_MASK       (0x1U << TOPCRU_GATE_CON01_CLK_PVTPLL_LOGIC_O_EN_SHIFT)      /* 0x00000008 */
#define TOPCRU_GATE_CON01_CLK_GPLL_MUX_TO_DDR_EN_SHIFT     (6U)
#define TOPCRU_GATE_CON01_CLK_GPLL_MUX_TO_DDR_EN_MASK      (0x1U << TOPCRU_GATE_CON01_CLK_GPLL_MUX_TO_DDR_EN_SHIFT)     /* 0x00000040 */
#define TOPCRU_GATE_CON01_CLK_GPLL_MUX_TO_PERI_EN_SHIFT    (7U)
#define TOPCRU_GATE_CON01_CLK_GPLL_MUX_TO_PERI_EN_MASK     (0x1U << TOPCRU_GATE_CON01_CLK_GPLL_MUX_TO_PERI_EN_SHIFT)    /* 0x00000080 */
#define TOPCRU_GATE_CON01_CLK_HPLL_MUX_TO_PERI_EN_SHIFT    (8U)
#define TOPCRU_GATE_CON01_CLK_HPLL_MUX_TO_PERI_EN_MASK     (0x1U << TOPCRU_GATE_CON01_CLK_HPLL_MUX_TO_PERI_EN_SHIFT)    /* 0x00000100 */
#define TOPCRU_GATE_CON01_CLK_24M_SSCSRC_EN_SHIFT          (9U)
#define TOPCRU_GATE_CON01_CLK_24M_SSCSRC_EN_MASK           (0x1U << TOPCRU_GATE_CON01_CLK_24M_SSCSRC_EN_SHIFT)          /* 0x00000200 */
#define TOPCRU_GATE_CON01_CLK_CAM0_OUT2IO_EN_SHIFT         (12U)
#define TOPCRU_GATE_CON01_CLK_CAM0_OUT2IO_EN_MASK          (0x1U << TOPCRU_GATE_CON01_CLK_CAM0_OUT2IO_EN_SHIFT)         /* 0x00001000 */
#define TOPCRU_GATE_CON01_CLK_CAM1_OUT2IO_EN_SHIFT         (13U)
#define TOPCRU_GATE_CON01_CLK_CAM1_OUT2IO_EN_MASK          (0x1U << TOPCRU_GATE_CON01_CLK_CAM1_OUT2IO_EN_SHIFT)         /* 0x00002000 */
#define TOPCRU_GATE_CON01_CLK_CAM2_OUT2IO_EN_SHIFT         (14U)
#define TOPCRU_GATE_CON01_CLK_CAM2_OUT2IO_EN_MASK          (0x1U << TOPCRU_GATE_CON01_CLK_CAM2_OUT2IO_EN_SHIFT)         /* 0x00004000 */
#define TOPCRU_GATE_CON01_CLK_CAM3_OUT2IO_EN_SHIFT         (15U)
#define TOPCRU_GATE_CON01_CLK_CAM3_OUT2IO_EN_MASK          (0x1U << TOPCRU_GATE_CON01_CLK_CAM3_OUT2IO_EN_SHIFT)         /* 0x00008000 */
/* GATE_CON04 */
#define TOPCRU_GATE_CON04_OFFSET                           (0x310U)
#define TOPCRU_GATE_CON04_CLK_CORE_EN_SHIFT                (0U)
#define TOPCRU_GATE_CON04_CLK_CORE_EN_MASK                 (0x1U << TOPCRU_GATE_CON04_CLK_CORE_EN_SHIFT)                /* 0x00000001 */
#define TOPCRU_GATE_CON04_CLK_CORE_LEV1PRE_EN_SHIFT        (1U)
#define TOPCRU_GATE_CON04_CLK_CORE_LEV1PRE_EN_MASK         (0x1U << TOPCRU_GATE_CON04_CLK_CORE_LEV1PRE_EN_SHIFT)        /* 0x00000002 */
#define TOPCRU_GATE_CON04_CLK_CORE_PRE_EN_SHIFT            (2U)
#define TOPCRU_GATE_CON04_CLK_CORE_PRE_EN_MASK             (0x1U << TOPCRU_GATE_CON04_CLK_CORE_PRE_EN_SHIFT)            /* 0x00000004 */
#define TOPCRU_GATE_CON04_ACLK_CORE_PRE_EN_SHIFT           (3U)
#define TOPCRU_GATE_CON04_ACLK_CORE_PRE_EN_MASK            (0x1U << TOPCRU_GATE_CON04_ACLK_CORE_PRE_EN_SHIFT)           /* 0x00000008 */
#define TOPCRU_GATE_CON04_CLK_SCANHS_ACLKM_CORE_EN_SHIFT   (4U)
#define TOPCRU_GATE_CON04_CLK_SCANHS_ACLKM_CORE_EN_MASK    (0x1U << TOPCRU_GATE_CON04_CLK_SCANHS_ACLKM_CORE_EN_SHIFT)   /* 0x00000010 */
#define TOPCRU_GATE_CON04_PCLK_DBG_PRE_EN_SHIFT            (5U)
#define TOPCRU_GATE_CON04_PCLK_DBG_PRE_EN_MASK             (0x1U << TOPCRU_GATE_CON04_PCLK_DBG_PRE_EN_SHIFT)            /* 0x00000020 */
#define TOPCRU_GATE_CON04_CLK_SCANHS_PCLK_DBG_EN_SHIFT     (6U)
#define TOPCRU_GATE_CON04_CLK_SCANHS_PCLK_DBG_EN_MASK      (0x1U << TOPCRU_GATE_CON04_CLK_SCANHS_PCLK_DBG_EN_SHIFT)     /* 0x00000040 */
#define TOPCRU_GATE_CON04_CLK_REF_PVTPLL_CORE_EN_SHIFT     (13U)
#define TOPCRU_GATE_CON04_CLK_REF_PVTPLL_CORE_EN_MASK      (0x1U << TOPCRU_GATE_CON04_CLK_REF_PVTPLL_CORE_EN_SHIFT)     /* 0x00002000 */
#define TOPCRU_GATE_CON04_CLK_TESTOUT_CORE_EN_SHIFT        (14U)
#define TOPCRU_GATE_CON04_CLK_TESTOUT_CORE_EN_MASK         (0x1U << TOPCRU_GATE_CON04_CLK_TESTOUT_CORE_EN_SHIFT)        /* 0x00004000 */
/* GATE_CON05 */
#define TOPCRU_GATE_CON05_OFFSET                           (0x314U)
#define TOPCRU_GATE_CON05_ACLK_CORE_BIU_EN_SHIFT           (0U)
#define TOPCRU_GATE_CON05_ACLK_CORE_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON05_ACLK_CORE_BIU_EN_SHIFT)           /* 0x00000001 */
#define TOPCRU_GATE_CON05_PCLK_CORE_BIU_EN_SHIFT           (1U)
#define TOPCRU_GATE_CON05_PCLK_CORE_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON05_PCLK_CORE_BIU_EN_SHIFT)           /* 0x00000002 */
#define TOPCRU_GATE_CON05_HCLK_CORE_BIU_EN_SHIFT           (2U)
#define TOPCRU_GATE_CON05_HCLK_CORE_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON05_HCLK_CORE_BIU_EN_SHIFT)           /* 0x00000004 */
#define TOPCRU_GATE_CON05_CLK_APLL_I_SHIFT                 (14U)
#define TOPCRU_GATE_CON05_CLK_APLL_I_MASK                  (0x1U << TOPCRU_GATE_CON05_CLK_APLL_I_SHIFT)                 /* 0x00004000 */
#define TOPCRU_GATE_CON05_CLK_APLL_G_EN_SHIFT              (15U)
#define TOPCRU_GATE_CON05_CLK_APLL_G_EN_MASK               (0x1U << TOPCRU_GATE_CON05_CLK_APLL_G_EN_SHIFT)              /* 0x00008000 */
/* GATE_CON06 */
#define TOPCRU_GATE_CON06_OFFSET                           (0x318U)
#define TOPCRU_GATE_CON06_CLK_NPU_PRE_EN_SHIFT             (0U)
#define TOPCRU_GATE_CON06_CLK_NPU_PRE_EN_MASK              (0x1U << TOPCRU_GATE_CON06_CLK_NPU_PRE_EN_SHIFT)             /* 0x00000001 */
#define TOPCRU_GATE_CON06_HCLK_NPU_PRE_EN_SHIFT            (1U)
#define TOPCRU_GATE_CON06_HCLK_NPU_PRE_EN_MASK             (0x1U << TOPCRU_GATE_CON06_HCLK_NPU_PRE_EN_SHIFT)            /* 0x00000002 */
#define TOPCRU_GATE_CON06_ACLK_NPU_BIU_EN_SHIFT            (2U)
#define TOPCRU_GATE_CON06_ACLK_NPU_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON06_ACLK_NPU_BIU_EN_SHIFT)            /* 0x00000004 */
#define TOPCRU_GATE_CON06_HCLK_NPU_BIU_EN_SHIFT            (3U)
#define TOPCRU_GATE_CON06_HCLK_NPU_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON06_HCLK_NPU_BIU_EN_SHIFT)            /* 0x00000008 */
#define TOPCRU_GATE_CON06_ACLK_RKNN_EN_SHIFT               (4U)
#define TOPCRU_GATE_CON06_ACLK_RKNN_EN_MASK                (0x1U << TOPCRU_GATE_CON06_ACLK_RKNN_EN_SHIFT)               /* 0x00000010 */
#define TOPCRU_GATE_CON06_HCLK_RKNN_EN_SHIFT               (5U)
#define TOPCRU_GATE_CON06_HCLK_RKNN_EN_MASK                (0x1U << TOPCRU_GATE_CON06_HCLK_RKNN_EN_SHIFT)               /* 0x00000020 */
#define TOPCRU_GATE_CON06_CLK_REF_PVTPLL_NPU_EN_SHIFT      (6U)
#define TOPCRU_GATE_CON06_CLK_REF_PVTPLL_NPU_EN_MASK       (0x1U << TOPCRU_GATE_CON06_CLK_REF_PVTPLL_NPU_EN_SHIFT)      /* 0x00000040 */
#define TOPCRU_GATE_CON06_CLK_TESTOUT_NPU_EN_SHIFT         (7U)
#define TOPCRU_GATE_CON06_CLK_TESTOUT_NPU_EN_MASK          (0x1U << TOPCRU_GATE_CON06_CLK_TESTOUT_NPU_EN_SHIFT)         /* 0x00000080 */
/* GATE_CON08 */
#define TOPCRU_GATE_CON08_OFFSET                           (0x320U)
#define TOPCRU_GATE_CON08_CLK_GPU_LEV1PRE_EN_SHIFT         (0U)
#define TOPCRU_GATE_CON08_CLK_GPU_LEV1PRE_EN_MASK          (0x1U << TOPCRU_GATE_CON08_CLK_GPU_LEV1PRE_EN_SHIFT)         /* 0x00000001 */
#define TOPCRU_GATE_CON08_CLK_GPU_PRE_EN_SHIFT             (1U)
#define TOPCRU_GATE_CON08_CLK_GPU_PRE_EN_MASK              (0x1U << TOPCRU_GATE_CON08_CLK_GPU_PRE_EN_SHIFT)             /* 0x00000002 */
#define TOPCRU_GATE_CON08_ACLK_GPU_PRE_EN_SHIFT            (2U)
#define TOPCRU_GATE_CON08_ACLK_GPU_PRE_EN_MASK             (0x1U << TOPCRU_GATE_CON08_ACLK_GPU_PRE_EN_SHIFT)            /* 0x00000004 */
#define TOPCRU_GATE_CON08_ACLK_GPU_BIU_EN_SHIFT            (3U)
#define TOPCRU_GATE_CON08_ACLK_GPU_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON08_ACLK_GPU_BIU_EN_SHIFT)            /* 0x00000008 */
#define TOPCRU_GATE_CON08_CLK_GPU_EN_SHIFT                 (4U)
#define TOPCRU_GATE_CON08_CLK_GPU_EN_MASK                  (0x1U << TOPCRU_GATE_CON08_CLK_GPU_EN_SHIFT)                 /* 0x00000010 */
#define TOPCRU_GATE_CON08_CLK_REF_PVTPLL_GPU_EN_SHIFT      (5U)
#define TOPCRU_GATE_CON08_CLK_REF_PVTPLL_GPU_EN_MASK       (0x1U << TOPCRU_GATE_CON08_CLK_REF_PVTPLL_GPU_EN_SHIFT)      /* 0x00000020 */
#define TOPCRU_GATE_CON08_CLK_TESTOUT_GPU_EN_SHIFT         (6U)
#define TOPCRU_GATE_CON08_CLK_TESTOUT_GPU_EN_MASK          (0x1U << TOPCRU_GATE_CON08_CLK_TESTOUT_GPU_EN_SHIFT)         /* 0x00000040 */
#define TOPCRU_GATE_CON08_CLK_GPU_BRG_BIU_EN_SHIFT         (8U)
#define TOPCRU_GATE_CON08_CLK_GPU_BRG_BIU_EN_MASK          (0x1U << TOPCRU_GATE_CON08_CLK_GPU_BRG_BIU_EN_SHIFT)         /* 0x00000100 */
/* GATE_CON09 */
#define TOPCRU_GATE_CON09_OFFSET                           (0x324U)
#define TOPCRU_GATE_CON09_CLK_RKVENC_CORE_EN_SHIFT         (0U)
#define TOPCRU_GATE_CON09_CLK_RKVENC_CORE_EN_MASK          (0x1U << TOPCRU_GATE_CON09_CLK_RKVENC_CORE_EN_SHIFT)         /* 0x00000001 */
#define TOPCRU_GATE_CON09_ACLK_VEPU_PRE_EN_SHIFT           (1U)
#define TOPCRU_GATE_CON09_ACLK_VEPU_PRE_EN_MASK            (0x1U << TOPCRU_GATE_CON09_ACLK_VEPU_PRE_EN_SHIFT)           /* 0x00000002 */
#define TOPCRU_GATE_CON09_HCLK_VEPU_PRE_EN_SHIFT           (2U)
#define TOPCRU_GATE_CON09_HCLK_VEPU_PRE_EN_MASK            (0x1U << TOPCRU_GATE_CON09_HCLK_VEPU_PRE_EN_SHIFT)           /* 0x00000004 */
#define TOPCRU_GATE_CON09_ACLK_VEPU_BIU_EN_SHIFT           (3U)
#define TOPCRU_GATE_CON09_ACLK_VEPU_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON09_ACLK_VEPU_BIU_EN_SHIFT)           /* 0x00000008 */
#define TOPCRU_GATE_CON09_HCLK_VEPU_BIU_EN_SHIFT           (4U)
#define TOPCRU_GATE_CON09_HCLK_VEPU_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON09_HCLK_VEPU_BIU_EN_SHIFT)           /* 0x00000010 */
#define TOPCRU_GATE_CON09_ACLK_RKVENC_EN_SHIFT             (5U)
#define TOPCRU_GATE_CON09_ACLK_RKVENC_EN_MASK              (0x1U << TOPCRU_GATE_CON09_ACLK_RKVENC_EN_SHIFT)             /* 0x00000020 */
#define TOPCRU_GATE_CON09_HCLK_RKVENC_EN_SHIFT             (6U)
#define TOPCRU_GATE_CON09_HCLK_RKVENC_EN_MASK              (0x1U << TOPCRU_GATE_CON09_HCLK_RKVENC_EN_SHIFT)             /* 0x00000040 */
#define TOPCRU_GATE_CON09_CLK_TESTOUT_VEPU_EN_SHIFT        (7U)
#define TOPCRU_GATE_CON09_CLK_TESTOUT_VEPU_EN_MASK         (0x1U << TOPCRU_GATE_CON09_CLK_TESTOUT_VEPU_EN_SHIFT)        /* 0x00000080 */
/* GATE_CON10 */
#define TOPCRU_GATE_CON10_OFFSET                           (0x328U)
#define TOPCRU_GATE_CON10_ACLK_VDPU_PRE_EN_SHIFT           (0U)
#define TOPCRU_GATE_CON10_ACLK_VDPU_PRE_EN_MASK            (0x1U << TOPCRU_GATE_CON10_ACLK_VDPU_PRE_EN_SHIFT)           /* 0x00000001 */
#define TOPCRU_GATE_CON10_CLK_RKVDEC_HEVC_CA_EN_SHIFT      (3U)
#define TOPCRU_GATE_CON10_CLK_RKVDEC_HEVC_CA_EN_MASK       (0x1U << TOPCRU_GATE_CON10_CLK_RKVDEC_HEVC_CA_EN_SHIFT)      /* 0x00000008 */
#define TOPCRU_GATE_CON10_HCLK_VDPU_PRE_EN_SHIFT           (4U)
#define TOPCRU_GATE_CON10_HCLK_VDPU_PRE_EN_MASK            (0x1U << TOPCRU_GATE_CON10_HCLK_VDPU_PRE_EN_SHIFT)           /* 0x00000010 */
#define TOPCRU_GATE_CON10_ACLK_VDPU_BIU_EN_SHIFT           (5U)
#define TOPCRU_GATE_CON10_ACLK_VDPU_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON10_ACLK_VDPU_BIU_EN_SHIFT)           /* 0x00000020 */
#define TOPCRU_GATE_CON10_HCLK_VDPU_BIU_EN_SHIFT           (6U)
#define TOPCRU_GATE_CON10_HCLK_VDPU_BIU_EN_MASK            (0x1U << TOPCRU_GATE_CON10_HCLK_VDPU_BIU_EN_SHIFT)           /* 0x00000040 */
#define TOPCRU_GATE_CON10_ACLK_RKVDEC_EN_SHIFT             (7U)
#define TOPCRU_GATE_CON10_ACLK_RKVDEC_EN_MASK              (0x1U << TOPCRU_GATE_CON10_ACLK_RKVDEC_EN_SHIFT)             /* 0x00000080 */
#define TOPCRU_GATE_CON10_HCLK_RKVDEC_EN_SHIFT             (8U)
#define TOPCRU_GATE_CON10_HCLK_RKVDEC_EN_MASK              (0x1U << TOPCRU_GATE_CON10_HCLK_RKVDEC_EN_SHIFT)             /* 0x00000100 */
#define TOPCRU_GATE_CON10_CLK_TESTOUT_VDPU_EN_SHIFT        (9U)
#define TOPCRU_GATE_CON10_CLK_TESTOUT_VDPU_EN_MASK         (0x1U << TOPCRU_GATE_CON10_CLK_TESTOUT_VDPU_EN_SHIFT)        /* 0x00000200 */
/* GATE_CON11 */
#define TOPCRU_GATE_CON11_OFFSET                           (0x32CU)
#define TOPCRU_GATE_CON11_ACLK_VI_EN_SHIFT                 (0U)
#define TOPCRU_GATE_CON11_ACLK_VI_EN_MASK                  (0x1U << TOPCRU_GATE_CON11_ACLK_VI_EN_SHIFT)                 /* 0x00000001 */
#define TOPCRU_GATE_CON11_HCLK_VI_EN_SHIFT                 (1U)
#define TOPCRU_GATE_CON11_HCLK_VI_EN_MASK                  (0x1U << TOPCRU_GATE_CON11_HCLK_VI_EN_SHIFT)                 /* 0x00000002 */
#define TOPCRU_GATE_CON11_PCLK_VI_EN_SHIFT                 (2U)
#define TOPCRU_GATE_CON11_PCLK_VI_EN_MASK                  (0x1U << TOPCRU_GATE_CON11_PCLK_VI_EN_SHIFT)                 /* 0x00000004 */
#define TOPCRU_GATE_CON11_ACLK_VI_BIU_EN_SHIFT             (3U)
#define TOPCRU_GATE_CON11_ACLK_VI_BIU_EN_MASK              (0x1U << TOPCRU_GATE_CON11_ACLK_VI_BIU_EN_SHIFT)             /* 0x00000008 */
#define TOPCRU_GATE_CON11_HCLK_VI_BIU_EN_SHIFT             (4U)
#define TOPCRU_GATE_CON11_HCLK_VI_BIU_EN_MASK              (0x1U << TOPCRU_GATE_CON11_HCLK_VI_BIU_EN_SHIFT)             /* 0x00000010 */
#define TOPCRU_GATE_CON11_PCLK_VI_BIU_EN_SHIFT             (5U)
#define TOPCRU_GATE_CON11_PCLK_VI_BIU_EN_MASK              (0x1U << TOPCRU_GATE_CON11_PCLK_VI_BIU_EN_SHIFT)             /* 0x00000020 */
#define TOPCRU_GATE_CON11_ACLK_ISP_EN_SHIFT                (6U)
#define TOPCRU_GATE_CON11_ACLK_ISP_EN_MASK                 (0x1U << TOPCRU_GATE_CON11_ACLK_ISP_EN_SHIFT)                /* 0x00000040 */
#define TOPCRU_GATE_CON11_HCLK_ISP_EN_SHIFT                (7U)
#define TOPCRU_GATE_CON11_HCLK_ISP_EN_MASK                 (0x1U << TOPCRU_GATE_CON11_HCLK_ISP_EN_SHIFT)                /* 0x00000080 */
#define TOPCRU_GATE_CON11_CLK_ISP_EN_SHIFT                 (8U)
#define TOPCRU_GATE_CON11_CLK_ISP_EN_MASK                  (0x1U << TOPCRU_GATE_CON11_CLK_ISP_EN_SHIFT)                 /* 0x00000100 */
#define TOPCRU_GATE_CON11_ACLK_VICAP_EN_SHIFT              (9U)
#define TOPCRU_GATE_CON11_ACLK_VICAP_EN_MASK               (0x1U << TOPCRU_GATE_CON11_ACLK_VICAP_EN_SHIFT)              /* 0x00000200 */
#define TOPCRU_GATE_CON11_HCLK_VICAP_EN_SHIFT              (10U)
#define TOPCRU_GATE_CON11_HCLK_VICAP_EN_MASK               (0x1U << TOPCRU_GATE_CON11_HCLK_VICAP_EN_SHIFT)              /* 0x00000400 */
#define TOPCRU_GATE_CON11_DCLK_VICAP_EN_SHIFT              (11U)
#define TOPCRU_GATE_CON11_DCLK_VICAP_EN_MASK               (0x1U << TOPCRU_GATE_CON11_DCLK_VICAP_EN_SHIFT)              /* 0x00000800 */
#define TOPCRU_GATE_CON11_CSIRX0_CLK_DATA_EN_SHIFT         (12U)
#define TOPCRU_GATE_CON11_CSIRX0_CLK_DATA_EN_MASK          (0x1U << TOPCRU_GATE_CON11_CSIRX0_CLK_DATA_EN_SHIFT)         /* 0x00001000 */
#define TOPCRU_GATE_CON11_CSIRX1_CLK_DATA_EN_SHIFT         (13U)
#define TOPCRU_GATE_CON11_CSIRX1_CLK_DATA_EN_MASK          (0x1U << TOPCRU_GATE_CON11_CSIRX1_CLK_DATA_EN_SHIFT)         /* 0x00002000 */
#define TOPCRU_GATE_CON11_CSIRX2_CLK_DATA_EN_SHIFT         (14U)
#define TOPCRU_GATE_CON11_CSIRX2_CLK_DATA_EN_MASK          (0x1U << TOPCRU_GATE_CON11_CSIRX2_CLK_DATA_EN_SHIFT)         /* 0x00004000 */
#define TOPCRU_GATE_CON11_CSIRX3_CLK_DATA_EN_SHIFT         (15U)
#define TOPCRU_GATE_CON11_CSIRX3_CLK_DATA_EN_MASK          (0x1U << TOPCRU_GATE_CON11_CSIRX3_CLK_DATA_EN_SHIFT)         /* 0x00008000 */
/* GATE_CON12 */
#define TOPCRU_GATE_CON12_OFFSET                           (0x330U)
#define TOPCRU_GATE_CON12_PCLK_CSIHOST0_EN_SHIFT           (0U)
#define TOPCRU_GATE_CON12_PCLK_CSIHOST0_EN_MASK            (0x1U << TOPCRU_GATE_CON12_PCLK_CSIHOST0_EN_SHIFT)           /* 0x00000001 */
#define TOPCRU_GATE_CON12_PCLK_CSIHOST1_EN_SHIFT           (1U)
#define TOPCRU_GATE_CON12_PCLK_CSIHOST1_EN_MASK            (0x1U << TOPCRU_GATE_CON12_PCLK_CSIHOST1_EN_SHIFT)           /* 0x00000002 */
#define TOPCRU_GATE_CON12_PCLK_CSIHOST2_EN_SHIFT           (2U)
#define TOPCRU_GATE_CON12_PCLK_CSIHOST2_EN_MASK            (0x1U << TOPCRU_GATE_CON12_PCLK_CSIHOST2_EN_SHIFT)           /* 0x00000004 */
#define TOPCRU_GATE_CON12_PCLK_CSIHOST3_EN_SHIFT           (3U)
#define TOPCRU_GATE_CON12_PCLK_CSIHOST3_EN_MASK            (0x1U << TOPCRU_GATE_CON12_PCLK_CSIHOST3_EN_SHIFT)           /* 0x00000008 */
#define TOPCRU_GATE_CON12_PCLK_CSIPHY0_EN_SHIFT            (4U)
#define TOPCRU_GATE_CON12_PCLK_CSIPHY0_EN_MASK             (0x1U << TOPCRU_GATE_CON12_PCLK_CSIPHY0_EN_SHIFT)            /* 0x00000010 */
#define TOPCRU_GATE_CON12_PCLK_CSIPHY1_EN_SHIFT            (5U)
#define TOPCRU_GATE_CON12_PCLK_CSIPHY1_EN_MASK             (0x1U << TOPCRU_GATE_CON12_PCLK_CSIPHY1_EN_SHIFT)            /* 0x00000020 */
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST0_DFT_EN_SHIFT (8U)
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST0_DFT_EN_MASK (0x1U << TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST0_DFT_EN_SHIFT) /* 0x00000100 */
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST1_DFT_EN_SHIFT (9U)
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST1_DFT_EN_MASK (0x1U << TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST1_DFT_EN_SHIFT) /* 0x00000200 */
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST2_DFT_EN_SHIFT (10U)
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST2_DFT_EN_MASK (0x1U << TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST2_DFT_EN_SHIFT) /* 0x00000400 */
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST3_DFT_EN_SHIFT (11U)
#define TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST3_DFT_EN_MASK (0x1U << TOPCRU_GATE_CON12_CLK_RXBYTEHS_CSIHOST3_DFT_EN_SHIFT) /* 0x00000800 */
#define TOPCRU_GATE_CON12_CLK_TESTOUT_VIPHY_EN_SHIFT       (15U)
#define TOPCRU_GATE_CON12_CLK_TESTOUT_VIPHY_EN_MASK        (0x1U << TOPCRU_GATE_CON12_CLK_TESTOUT_VIPHY_EN_SHIFT)       /* 0x00008000 */
/* GATE_CON13 */
#define TOPCRU_GATE_CON13_OFFSET                           (0x334U)
#define TOPCRU_GATE_CON13_ACLK_VO_PRE_EN_SHIFT             (0U)
#define TOPCRU_GATE_CON13_ACLK_VO_PRE_EN_MASK              (0x1U << TOPCRU_GATE_CON13_ACLK_VO_PRE_EN_SHIFT)             /* 0x00000001 */
#define TOPCRU_GATE_CON13_HCLK_VO_PRE_EN_SHIFT             (1U)
#define TOPCRU_GATE_CON13_HCLK_VO_PRE_EN_MASK              (0x1U << TOPCRU_GATE_CON13_HCLK_VO_PRE_EN_SHIFT)             /* 0x00000002 */
#define TOPCRU_GATE_CON13_ACLK_VO_BIU_EN_SHIFT             (3U)
#define TOPCRU_GATE_CON13_ACLK_VO_BIU_EN_MASK              (0x1U << TOPCRU_GATE_CON13_ACLK_VO_BIU_EN_SHIFT)             /* 0x00000008 */
#define TOPCRU_GATE_CON13_HCLK_VO_BIU_EN_SHIFT             (4U)
#define TOPCRU_GATE_CON13_HCLK_VO_BIU_EN_MASK              (0x1U << TOPCRU_GATE_CON13_HCLK_VO_BIU_EN_SHIFT)             /* 0x00000010 */
#define TOPCRU_GATE_CON13_ACLK_VOP_EN_SHIFT                (6U)
#define TOPCRU_GATE_CON13_ACLK_VOP_EN_MASK                 (0x1U << TOPCRU_GATE_CON13_ACLK_VOP_EN_SHIFT)                /* 0x00000040 */
#define TOPCRU_GATE_CON13_HCLK_VOP_EN_SHIFT                (7U)
#define TOPCRU_GATE_CON13_HCLK_VOP_EN_MASK                 (0x1U << TOPCRU_GATE_CON13_HCLK_VOP_EN_SHIFT)                /* 0x00000080 */
#define TOPCRU_GATE_CON13_DCLK_VOP_EN_SHIFT                (8U)
#define TOPCRU_GATE_CON13_DCLK_VOP_EN_MASK                 (0x1U << TOPCRU_GATE_CON13_DCLK_VOP_EN_SHIFT)                /* 0x00000100 */
#define TOPCRU_GATE_CON13_DCLK_VOP1_EN_SHIFT               (9U)
#define TOPCRU_GATE_CON13_DCLK_VOP1_EN_MASK                (0x1U << TOPCRU_GATE_CON13_DCLK_VOP1_EN_SHIFT)               /* 0x00000200 */
#define TOPCRU_GATE_CON13_CLK_TESTOUT_VO_EN_SHIFT          (15U)
#define TOPCRU_GATE_CON13_CLK_TESTOUT_VO_EN_MASK           (0x1U << TOPCRU_GATE_CON13_CLK_TESTOUT_VO_EN_SHIFT)          /* 0x00008000 */
/* GATE_CON14 */
#define TOPCRU_GATE_CON14_OFFSET                           (0x338U)
#define TOPCRU_GATE_CON14_ACLK_RGA_PRE_EN_SHIFT            (0U)
#define TOPCRU_GATE_CON14_ACLK_RGA_PRE_EN_MASK             (0x1U << TOPCRU_GATE_CON14_ACLK_RGA_PRE_EN_SHIFT)            /* 0x00000001 */
#define TOPCRU_GATE_CON14_HCLK_RGA_PRE_EN_SHIFT            (1U)
#define TOPCRU_GATE_CON14_HCLK_RGA_PRE_EN_MASK             (0x1U << TOPCRU_GATE_CON14_HCLK_RGA_PRE_EN_SHIFT)            /* 0x00000002 */
#define TOPCRU_GATE_CON14_ACLK_RGA_BIU_EN_SHIFT            (3U)
#define TOPCRU_GATE_CON14_ACLK_RGA_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON14_ACLK_RGA_BIU_EN_SHIFT)            /* 0x00000008 */
#define TOPCRU_GATE_CON14_HCLK_RGA_BIU_EN_SHIFT            (4U)
#define TOPCRU_GATE_CON14_HCLK_RGA_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON14_HCLK_RGA_BIU_EN_SHIFT)            /* 0x00000010 */
#define TOPCRU_GATE_CON14_ACLK_RGA_EN_SHIFT                (6U)
#define TOPCRU_GATE_CON14_ACLK_RGA_EN_MASK                 (0x1U << TOPCRU_GATE_CON14_ACLK_RGA_EN_SHIFT)                /* 0x00000040 */
#define TOPCRU_GATE_CON14_HCLK_RGA_EN_SHIFT                (7U)
#define TOPCRU_GATE_CON14_HCLK_RGA_EN_MASK                 (0x1U << TOPCRU_GATE_CON14_HCLK_RGA_EN_SHIFT)                /* 0x00000080 */
#define TOPCRU_GATE_CON14_CLK_RGA_CORE_EN_SHIFT            (8U)
#define TOPCRU_GATE_CON14_CLK_RGA_CORE_EN_MASK             (0x1U << TOPCRU_GATE_CON14_CLK_RGA_CORE_EN_SHIFT)            /* 0x00000100 */
#define TOPCRU_GATE_CON14_ACLK_JDEC_EN_SHIFT               (9U)
#define TOPCRU_GATE_CON14_ACLK_JDEC_EN_MASK                (0x1U << TOPCRU_GATE_CON14_ACLK_JDEC_EN_SHIFT)               /* 0x00000200 */
#define TOPCRU_GATE_CON14_HCLK_JDEC_EN_SHIFT               (10U)
#define TOPCRU_GATE_CON14_HCLK_JDEC_EN_MASK                (0x1U << TOPCRU_GATE_CON14_HCLK_JDEC_EN_SHIFT)               /* 0x00000400 */
#define TOPCRU_GATE_CON14_CLK_TESTOUT_RGA_EN_SHIFT         (15U)
#define TOPCRU_GATE_CON14_CLK_TESTOUT_RGA_EN_MASK          (0x1U << TOPCRU_GATE_CON14_CLK_TESTOUT_RGA_EN_SHIFT)         /* 0x00008000 */
/* GATE_CON16 */
#define TOPCRU_GATE_CON16_OFFSET                           (0x340U)
#define TOPCRU_GATE_CON16_ACLK_PHP_EN_SHIFT                (0U)
#define TOPCRU_GATE_CON16_ACLK_PHP_EN_MASK                 (0x1U << TOPCRU_GATE_CON16_ACLK_PHP_EN_SHIFT)                /* 0x00000001 */
#define TOPCRU_GATE_CON16_PCLK_PHP_EN_SHIFT                (1U)
#define TOPCRU_GATE_CON16_PCLK_PHP_EN_MASK                 (0x1U << TOPCRU_GATE_CON16_PCLK_PHP_EN_SHIFT)                /* 0x00000002 */
#define TOPCRU_GATE_CON16_PCLK_PHP_BIU_EN_SHIFT            (2U)
#define TOPCRU_GATE_CON16_PCLK_PHP_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON16_PCLK_PHP_BIU_EN_SHIFT)            /* 0x00000004 */
#define TOPCRU_GATE_CON16_ACLK_PHP_BIU_EN_SHIFT            (3U)
#define TOPCRU_GATE_CON16_ACLK_PHP_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON16_ACLK_PHP_BIU_EN_SHIFT)            /* 0x00000008 */
#define TOPCRU_GATE_CON16_ACLK_PCIE20_MST_EN_SHIFT         (4U)
#define TOPCRU_GATE_CON16_ACLK_PCIE20_MST_EN_MASK          (0x1U << TOPCRU_GATE_CON16_ACLK_PCIE20_MST_EN_SHIFT)         /* 0x00000010 */
#define TOPCRU_GATE_CON16_ACLK_PCIE20_SLV_EN_SHIFT         (5U)
#define TOPCRU_GATE_CON16_ACLK_PCIE20_SLV_EN_MASK          (0x1U << TOPCRU_GATE_CON16_ACLK_PCIE20_SLV_EN_SHIFT)         /* 0x00000020 */
#define TOPCRU_GATE_CON16_ACLK_PCIE20_DBI_EN_SHIFT         (6U)
#define TOPCRU_GATE_CON16_ACLK_PCIE20_DBI_EN_MASK          (0x1U << TOPCRU_GATE_CON16_ACLK_PCIE20_DBI_EN_SHIFT)         /* 0x00000040 */
#define TOPCRU_GATE_CON16_PCLK_PCIE20_EN_SHIFT             (7U)
#define TOPCRU_GATE_CON16_PCLK_PCIE20_EN_MASK              (0x1U << TOPCRU_GATE_CON16_PCLK_PCIE20_EN_SHIFT)             /* 0x00000080 */
#define TOPCRU_GATE_CON16_CLK_PCIE20_AUX_EN_SHIFT          (8U)
#define TOPCRU_GATE_CON16_CLK_PCIE20_AUX_EN_MASK           (0x1U << TOPCRU_GATE_CON16_CLK_PCIE20_AUX_EN_SHIFT)          /* 0x00000100 */
#define TOPCRU_GATE_CON16_ACLK_USB3OTG_EN_SHIFT            (10U)
#define TOPCRU_GATE_CON16_ACLK_USB3OTG_EN_MASK             (0x1U << TOPCRU_GATE_CON16_ACLK_USB3OTG_EN_SHIFT)            /* 0x00000400 */
#define TOPCRU_GATE_CON16_CLK_USB3OTG_SUSPEND_EN_SHIFT     (11U)
#define TOPCRU_GATE_CON16_CLK_USB3OTG_SUSPEND_EN_MASK      (0x1U << TOPCRU_GATE_CON16_CLK_USB3OTG_SUSPEND_EN_SHIFT)     /* 0x00000800 */
#define TOPCRU_GATE_CON16_CLK_USB3OTG_REF_EN_SHIFT         (12U)
#define TOPCRU_GATE_CON16_CLK_USB3OTG_REF_EN_MASK          (0x1U << TOPCRU_GATE_CON16_CLK_USB3OTG_REF_EN_SHIFT)         /* 0x00001000 */
/* GATE_CON17 */
#define TOPCRU_GATE_CON17_OFFSET                           (0x344U)
#define TOPCRU_GATE_CON17_CLK_PIPEPHY_EN_SHIFT             (0U)
#define TOPCRU_GATE_CON17_CLK_PIPEPHY_EN_MASK              (0x1U << TOPCRU_GATE_CON17_CLK_PIPEPHY_EN_SHIFT)             /* 0x00000001 */
#define TOPCRU_GATE_CON17_CLK_USB3OTG_PIPE_DFT_EN_SHIFT    (1U)
#define TOPCRU_GATE_CON17_CLK_USB3OTG_PIPE_DFT_EN_MASK     (0x1U << TOPCRU_GATE_CON17_CLK_USB3OTG_PIPE_DFT_EN_SHIFT)    /* 0x00000002 */
#define TOPCRU_GATE_CON17_CLK_USBPHY_USB3OTG_UTMI_DFT_EN_SHIFT (2U)
#define TOPCRU_GATE_CON17_CLK_USBPHY_USB3OTG_UTMI_DFT_EN_MASK (0x1U << TOPCRU_GATE_CON17_CLK_USBPHY_USB3OTG_UTMI_DFT_EN_SHIFT) /* 0x00000004 */
#define TOPCRU_GATE_CON17_CLK_PIPEPHY_REF_FUNC_EN_SHIFT    (3U)
#define TOPCRU_GATE_CON17_CLK_PIPEPHY_REF_FUNC_EN_MASK     (0x1U << TOPCRU_GATE_CON17_CLK_PIPEPHY_REF_FUNC_EN_SHIFT)    /* 0x00000008 */
#define TOPCRU_GATE_CON17_CLK_TESTOUT_PHPPHY_EN_SHIFT      (15U)
#define TOPCRU_GATE_CON17_CLK_TESTOUT_PHPPHY_EN_MASK       (0x1U << TOPCRU_GATE_CON17_CLK_TESTOUT_PHPPHY_EN_SHIFT)      /* 0x00008000 */
/* GATE_CON18 */
#define TOPCRU_GATE_CON18_OFFSET                           (0x348U)
#define TOPCRU_GATE_CON18_ACLK_BUS_EN_SHIFT                (0U)
#define TOPCRU_GATE_CON18_ACLK_BUS_EN_MASK                 (0x1U << TOPCRU_GATE_CON18_ACLK_BUS_EN_SHIFT)                /* 0x00000001 */
#define TOPCRU_GATE_CON18_HCLK_BUS_EN_SHIFT                (1U)
#define TOPCRU_GATE_CON18_HCLK_BUS_EN_MASK                 (0x1U << TOPCRU_GATE_CON18_HCLK_BUS_EN_SHIFT)                /* 0x00000002 */
#define TOPCRU_GATE_CON18_PCLK_BUS_EN_SHIFT                (2U)
#define TOPCRU_GATE_CON18_PCLK_BUS_EN_MASK                 (0x1U << TOPCRU_GATE_CON18_PCLK_BUS_EN_SHIFT)                /* 0x00000004 */
#define TOPCRU_GATE_CON18_ACLK_BUS_BIU_EN_SHIFT            (3U)
#define TOPCRU_GATE_CON18_ACLK_BUS_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON18_ACLK_BUS_BIU_EN_SHIFT)            /* 0x00000008 */
#define TOPCRU_GATE_CON18_HCLK_BUS_BIU_EN_SHIFT            (4U)
#define TOPCRU_GATE_CON18_HCLK_BUS_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON18_HCLK_BUS_BIU_EN_SHIFT)            /* 0x00000010 */
#define TOPCRU_GATE_CON18_PCLK_BUS_BIU_EN_SHIFT            (5U)
#define TOPCRU_GATE_CON18_PCLK_BUS_BIU_EN_MASK             (0x1U << TOPCRU_GATE_CON18_PCLK_BUS_BIU_EN_SHIFT)            /* 0x00000020 */
/* GATE_CON19 */
#define TOPCRU_GATE_CON19_OFFSET                           (0x34CU)
#define TOPCRU_GATE_CON19_PCLK_I2C1_EN_SHIFT               (0U)
#define TOPCRU_GATE_CON19_PCLK_I2C1_EN_MASK                (0x1U << TOPCRU_GATE_CON19_PCLK_I2C1_EN_SHIFT)               /* 0x00000001 */
#define TOPCRU_GATE_CON19_PCLK_I2C2_EN_SHIFT               (1U)
#define TOPCRU_GATE_CON19_PCLK_I2C2_EN_MASK                (0x1U << TOPCRU_GATE_CON19_PCLK_I2C2_EN_SHIFT)               /* 0x00000002 */
#define TOPCRU_GATE_CON19_PCLK_I2C3_EN_SHIFT               (2U)
#define TOPCRU_GATE_CON19_PCLK_I2C3_EN_MASK                (0x1U << TOPCRU_GATE_CON19_PCLK_I2C3_EN_SHIFT)               /* 0x00000004 */
#define TOPCRU_GATE_CON19_PCLK_I2C4_EN_SHIFT               (3U)
#define TOPCRU_GATE_CON19_PCLK_I2C4_EN_MASK                (0x1U << TOPCRU_GATE_CON19_PCLK_I2C4_EN_SHIFT)               /* 0x00000008 */
#define TOPCRU_GATE_CON19_PCLK_I2C5_EN_SHIFT               (4U)
#define TOPCRU_GATE_CON19_PCLK_I2C5_EN_MASK                (0x1U << TOPCRU_GATE_CON19_PCLK_I2C5_EN_SHIFT)               /* 0x00000010 */
#define TOPCRU_GATE_CON19_CLK_I2C_EN_SHIFT                 (5U)
#define TOPCRU_GATE_CON19_CLK_I2C_EN_MASK                  (0x1U << TOPCRU_GATE_CON19_CLK_I2C_EN_SHIFT)                 /* 0x00000020 */
#define TOPCRU_GATE_CON19_CLK_I2C1_EN_SHIFT                (6U)
#define TOPCRU_GATE_CON19_CLK_I2C1_EN_MASK                 (0x1U << TOPCRU_GATE_CON19_CLK_I2C1_EN_SHIFT)                /* 0x00000040 */
#define TOPCRU_GATE_CON19_CLK_I2C2_EN_SHIFT                (7U)
#define TOPCRU_GATE_CON19_CLK_I2C2_EN_MASK                 (0x1U << TOPCRU_GATE_CON19_CLK_I2C2_EN_SHIFT)                /* 0x00000080 */
#define TOPCRU_GATE_CON19_CLK_I2C3_EN_SHIFT                (8U)
#define TOPCRU_GATE_CON19_CLK_I2C3_EN_MASK                 (0x1U << TOPCRU_GATE_CON19_CLK_I2C3_EN_SHIFT)                /* 0x00000100 */
#define TOPCRU_GATE_CON19_CLK_I2C4_EN_SHIFT                (9U)
#define TOPCRU_GATE_CON19_CLK_I2C4_EN_MASK                 (0x1U << TOPCRU_GATE_CON19_CLK_I2C4_EN_SHIFT)                /* 0x00000200 */
#define TOPCRU_GATE_CON19_CLK_I2C5_EN_SHIFT                (10U)
#define TOPCRU_GATE_CON19_CLK_I2C5_EN_MASK                 (0x1U << TOPCRU_GATE_CON19_CLK_I2C5_EN_SHIFT)                /* 0x00000400 */
/* GATE_CON20 */
#define TOPCRU_GATE_CON20_OFFSET                           (0x350U)
#define TOPCRU_GATE_CON20_DCLK_BUS_GPIO_EN_SHIFT           (4U)
#define TOPCRU_GATE_CON20_DCLK_BUS_GPIO_EN_MASK            (0x1U << TOPCRU_GATE_CON20_DCLK_BUS_GPIO_EN_SHIFT)           /* 0x00000010 */
#define TOPCRU_GATE_CON20_DCLK_BUS_GPIO3_EN_SHIFT          (5U)
#define TOPCRU_GATE_CON20_DCLK_BUS_GPIO3_EN_MASK           (0x1U << TOPCRU_GATE_CON20_DCLK_BUS_GPIO3_EN_SHIFT)          /* 0x00000020 */
#define TOPCRU_GATE_CON20_DCLK_BUS_GPIO4_EN_SHIFT          (6U)
#define TOPCRU_GATE_CON20_DCLK_BUS_GPIO4_EN_MASK           (0x1U << TOPCRU_GATE_CON20_DCLK_BUS_GPIO4_EN_SHIFT)          /* 0x00000040 */
/* GATE_CON21 */
#define TOPCRU_GATE_CON21_OFFSET                           (0x354U)
#define TOPCRU_GATE_CON21_PCLK_TIMER_EN_SHIFT              (0U)
#define TOPCRU_GATE_CON21_PCLK_TIMER_EN_MASK               (0x1U << TOPCRU_GATE_CON21_PCLK_TIMER_EN_SHIFT)              /* 0x00000001 */
#define TOPCRU_GATE_CON21_CLK_TIMER0_EN_SHIFT              (1U)
#define TOPCRU_GATE_CON21_CLK_TIMER0_EN_MASK               (0x1U << TOPCRU_GATE_CON21_CLK_TIMER0_EN_SHIFT)              /* 0x00000002 */
#define TOPCRU_GATE_CON21_CLK_TIMER1_EN_SHIFT              (2U)
#define TOPCRU_GATE_CON21_CLK_TIMER1_EN_MASK               (0x1U << TOPCRU_GATE_CON21_CLK_TIMER1_EN_SHIFT)              /* 0x00000004 */
#define TOPCRU_GATE_CON21_CLK_TIMER2_EN_SHIFT              (3U)
#define TOPCRU_GATE_CON21_CLK_TIMER2_EN_MASK               (0x1U << TOPCRU_GATE_CON21_CLK_TIMER2_EN_SHIFT)              /* 0x00000008 */
#define TOPCRU_GATE_CON21_CLK_TIMER3_EN_SHIFT              (4U)
#define TOPCRU_GATE_CON21_CLK_TIMER3_EN_MASK               (0x1U << TOPCRU_GATE_CON21_CLK_TIMER3_EN_SHIFT)              /* 0x00000010 */
#define TOPCRU_GATE_CON21_CLK_TIMER4_EN_SHIFT              (5U)
#define TOPCRU_GATE_CON21_CLK_TIMER4_EN_MASK               (0x1U << TOPCRU_GATE_CON21_CLK_TIMER4_EN_SHIFT)              /* 0x00000020 */
#define TOPCRU_GATE_CON21_CLK_TIMER5_EN_SHIFT              (6U)
#define TOPCRU_GATE_CON21_CLK_TIMER5_EN_MASK               (0x1U << TOPCRU_GATE_CON21_CLK_TIMER5_EN_SHIFT)              /* 0x00000040 */
#define TOPCRU_GATE_CON21_PCLK_STIMER_EN_SHIFT             (7U)
#define TOPCRU_GATE_CON21_PCLK_STIMER_EN_MASK              (0x1U << TOPCRU_GATE_CON21_PCLK_STIMER_EN_SHIFT)             /* 0x00000080 */
#define TOPCRU_GATE_CON21_CLK_STIMER0_EN_SHIFT             (8U)
#define TOPCRU_GATE_CON21_CLK_STIMER0_EN_MASK              (0x1U << TOPCRU_GATE_CON21_CLK_STIMER0_EN_SHIFT)             /* 0x00000100 */
#define TOPCRU_GATE_CON21_CLK_STIMER1_EN_SHIFT             (9U)
#define TOPCRU_GATE_CON21_CLK_STIMER1_EN_MASK              (0x1U << TOPCRU_GATE_CON21_CLK_STIMER1_EN_SHIFT)             /* 0x00000200 */
/* GATE_CON22 */
#define TOPCRU_GATE_CON22_OFFSET                           (0x358U)
#define TOPCRU_GATE_CON22_PCLK_WDTNS_EN_SHIFT              (0U)
#define TOPCRU_GATE_CON22_PCLK_WDTNS_EN_MASK               (0x1U << TOPCRU_GATE_CON22_PCLK_WDTNS_EN_SHIFT)              /* 0x00000001 */
#define TOPCRU_GATE_CON22_CLK_WDTNS_EN_SHIFT               (1U)
#define TOPCRU_GATE_CON22_CLK_WDTNS_EN_MASK                (0x1U << TOPCRU_GATE_CON22_CLK_WDTNS_EN_SHIFT)               /* 0x00000002 */
#define TOPCRU_GATE_CON22_PCLK_GRF_EN_SHIFT                (2U)
#define TOPCRU_GATE_CON22_PCLK_GRF_EN_MASK                 (0x1U << TOPCRU_GATE_CON22_PCLK_GRF_EN_SHIFT)                /* 0x00000004 */
#define TOPCRU_GATE_CON22_PCLK_SGRF_EN_SHIFT               (3U)
#define TOPCRU_GATE_CON22_PCLK_SGRF_EN_MASK                (0x1U << TOPCRU_GATE_CON22_PCLK_SGRF_EN_SHIFT)               /* 0x00000008 */
#define TOPCRU_GATE_CON22_PCLK_MAILBOX_EN_SHIFT            (4U)
#define TOPCRU_GATE_CON22_PCLK_MAILBOX_EN_MASK             (0x1U << TOPCRU_GATE_CON22_PCLK_MAILBOX_EN_SHIFT)            /* 0x00000010 */
#define TOPCRU_GATE_CON22_PCLK_INTC_EN_SHIFT               (5U)
#define TOPCRU_GATE_CON22_PCLK_INTC_EN_MASK                (0x1U << TOPCRU_GATE_CON22_PCLK_INTC_EN_SHIFT)               /* 0x00000020 */
#define TOPCRU_GATE_CON22_ACLK_BUS_GIC400_EN_SHIFT         (6U)
#define TOPCRU_GATE_CON22_ACLK_BUS_GIC400_EN_MASK          (0x1U << TOPCRU_GATE_CON22_ACLK_BUS_GIC400_EN_SHIFT)         /* 0x00000040 */
/* GATE_CON23 */
#define TOPCRU_GATE_CON23_OFFSET                           (0x35CU)
#define TOPCRU_GATE_CON23_ACLK_BUS_SPINLOCK_EN_SHIFT       (0U)
#define TOPCRU_GATE_CON23_ACLK_BUS_SPINLOCK_EN_MASK        (0x1U << TOPCRU_GATE_CON23_ACLK_BUS_SPINLOCK_EN_SHIFT)       /* 0x00000001 */
#define TOPCRU_GATE_CON23_ACLK_DCF_EN_SHIFT                (1U)
#define TOPCRU_GATE_CON23_ACLK_DCF_EN_MASK                 (0x1U << TOPCRU_GATE_CON23_ACLK_DCF_EN_SHIFT)                /* 0x00000002 */
#define TOPCRU_GATE_CON23_PCLK_DCF_EN_SHIFT                (2U)
#define TOPCRU_GATE_CON23_PCLK_DCF_EN_MASK                 (0x1U << TOPCRU_GATE_CON23_PCLK_DCF_EN_SHIFT)                /* 0x00000004 */
#define TOPCRU_GATE_CON23_FCLK_BUS_CM0_CORE_EN_SHIFT       (3U)
#define TOPCRU_GATE_CON23_FCLK_BUS_CM0_CORE_EN_MASK        (0x1U << TOPCRU_GATE_CON23_FCLK_BUS_CM0_CORE_EN_SHIFT)       /* 0x00000008 */
#define TOPCRU_GATE_CON23_CLK_BUS_CM0_RTC_EN_SHIFT         (4U)
#define TOPCRU_GATE_CON23_CLK_BUS_CM0_RTC_EN_MASK          (0x1U << TOPCRU_GATE_CON23_CLK_BUS_CM0_RTC_EN_SHIFT)         /* 0x00000010 */
#define TOPCRU_GATE_CON23_HCLK_ICACHE_EN_SHIFT             (8U)
#define TOPCRU_GATE_CON23_HCLK_ICACHE_EN_MASK              (0x1U << TOPCRU_GATE_CON23_HCLK_ICACHE_EN_SHIFT)             /* 0x00000100 */
#define TOPCRU_GATE_CON23_HCLK_DCACHE_EN_SHIFT             (9U)
#define TOPCRU_GATE_CON23_HCLK_DCACHE_EN_MASK              (0x1U << TOPCRU_GATE_CON23_HCLK_DCACHE_EN_SHIFT)             /* 0x00000200 */
/* GATE_CON24 */
#define TOPCRU_GATE_CON24_OFFSET                           (0x360U)
#define TOPCRU_GATE_CON24_PCLK_TSADC_EN_SHIFT              (0U)
#define TOPCRU_GATE_CON24_PCLK_TSADC_EN_MASK               (0x1U << TOPCRU_GATE_CON24_PCLK_TSADC_EN_SHIFT)              /* 0x00000001 */
#define TOPCRU_GATE_CON24_CLK_TSADC_EN_SHIFT               (1U)
#define TOPCRU_GATE_CON24_CLK_TSADC_EN_MASK                (0x1U << TOPCRU_GATE_CON24_CLK_TSADC_EN_SHIFT)               /* 0x00000002 */
#define TOPCRU_GATE_CON24_CLK_TSADC_TSEN_EN_SHIFT          (3U)
#define TOPCRU_GATE_CON24_CLK_TSADC_TSEN_EN_MASK           (0x1U << TOPCRU_GATE_CON24_CLK_TSADC_TSEN_EN_SHIFT)          /* 0x00000008 */
#define TOPCRU_GATE_CON24_PCLK_DFT2APB_EN_SHIFT            (4U)
#define TOPCRU_GATE_CON24_PCLK_DFT2APB_EN_MASK             (0x1U << TOPCRU_GATE_CON24_PCLK_DFT2APB_EN_SHIFT)            /* 0x00000010 */
#define TOPCRU_GATE_CON24_CLK_SARADC_VCCIO156_EN_SHIFT     (9U)
#define TOPCRU_GATE_CON24_CLK_SARADC_VCCIO156_EN_MASK      (0x1U << TOPCRU_GATE_CON24_CLK_SARADC_VCCIO156_EN_SHIFT)     /* 0x00000200 */
/* GATE_CON25 */
#define TOPCRU_GATE_CON25_OFFSET                           (0x364U)
#define TOPCRU_GATE_CON25_PCLK_GMAC_EN_SHIFT               (0U)
#define TOPCRU_GATE_CON25_PCLK_GMAC_EN_MASK                (0x1U << TOPCRU_GATE_CON25_PCLK_GMAC_EN_SHIFT)               /* 0x00000001 */
#define TOPCRU_GATE_CON25_ACLK_GMAC_EN_SHIFT               (1U)
#define TOPCRU_GATE_CON25_ACLK_GMAC_EN_MASK                (0x1U << TOPCRU_GATE_CON25_ACLK_GMAC_EN_SHIFT)               /* 0x00000002 */
#define TOPCRU_GATE_CON25_CLK_GMAC_125M_CRU_EN_SHIFT       (2U)
#define TOPCRU_GATE_CON25_CLK_GMAC_125M_CRU_EN_MASK        (0x1U << TOPCRU_GATE_CON25_CLK_GMAC_125M_CRU_EN_SHIFT)       /* 0x00000004 */
#define TOPCRU_GATE_CON25_CLK_GMAC_50M_CRU_EN_SHIFT        (3U)
#define TOPCRU_GATE_CON25_CLK_GMAC_50M_CRU_EN_MASK         (0x1U << TOPCRU_GATE_CON25_CLK_GMAC_50M_CRU_EN_SHIFT)        /* 0x00000008 */
#define TOPCRU_GATE_CON25_CLK_GMAC_ETH_OUT2IO_EN_SHIFT     (4U)
#define TOPCRU_GATE_CON25_CLK_GMAC_ETH_OUT2IO_EN_MASK      (0x1U << TOPCRU_GATE_CON25_CLK_GMAC_ETH_OUT2IO_EN_SHIFT)     /* 0x00000010 */
#define TOPCRU_GATE_CON25_PCLK_APB2ASB_VCCIO156_EN_SHIFT   (5U)
#define TOPCRU_GATE_CON25_PCLK_APB2ASB_VCCIO156_EN_MASK    (0x1U << TOPCRU_GATE_CON25_PCLK_APB2ASB_VCCIO156_EN_SHIFT)   /* 0x00000020 */
#define TOPCRU_GATE_CON25_PCLK_TO_VCCIO156_EN_SHIFT        (6U)
#define TOPCRU_GATE_CON25_PCLK_TO_VCCIO156_EN_MASK         (0x1U << TOPCRU_GATE_CON25_PCLK_TO_VCCIO156_EN_SHIFT)        /* 0x00000040 */
#define TOPCRU_GATE_CON25_PCLK_DSIPHY_EN_SHIFT             (8U)
#define TOPCRU_GATE_CON25_PCLK_DSIPHY_EN_MASK              (0x1U << TOPCRU_GATE_CON25_PCLK_DSIPHY_EN_SHIFT)             /* 0x00000100 */
#define TOPCRU_GATE_CON25_PCLK_DSITX_EN_SHIFT              (9U)
#define TOPCRU_GATE_CON25_PCLK_DSITX_EN_MASK               (0x1U << TOPCRU_GATE_CON25_PCLK_DSITX_EN_SHIFT)              /* 0x00000200 */
#define TOPCRU_GATE_CON25_PCLK_CPU_EMA_DET_EN_SHIFT        (10U)
#define TOPCRU_GATE_CON25_PCLK_CPU_EMA_DET_EN_MASK         (0x1U << TOPCRU_GATE_CON25_PCLK_CPU_EMA_DET_EN_SHIFT)        /* 0x00000400 */
#define TOPCRU_GATE_CON25_PCLK_HASH_EN_SHIFT               (11U)
#define TOPCRU_GATE_CON25_PCLK_HASH_EN_MASK                (0x1U << TOPCRU_GATE_CON25_PCLK_HASH_EN_SHIFT)               /* 0x00000800 */
#define TOPCRU_GATE_CON25_CLK_TESTOUT_BUS_EN_SHIFT         (14U)
#define TOPCRU_GATE_CON25_CLK_TESTOUT_BUS_EN_MASK          (0x1U << TOPCRU_GATE_CON25_CLK_TESTOUT_BUS_EN_SHIFT)         /* 0x00004000 */
#define TOPCRU_GATE_CON25_PCLK_TOPCRU_EN_SHIFT             (15U)
#define TOPCRU_GATE_CON25_PCLK_TOPCRU_EN_MASK              (0x1U << TOPCRU_GATE_CON25_PCLK_TOPCRU_EN_SHIFT)             /* 0x00008000 */
/* GATE_CON26 */
#define TOPCRU_GATE_CON26_OFFSET                           (0x368U)
#define TOPCRU_GATE_CON26_PCLK_ASB2APB_VCCIO156_EN_SHIFT   (0U)
#define TOPCRU_GATE_CON26_PCLK_ASB2APB_VCCIO156_EN_MASK    (0x1U << TOPCRU_GATE_CON26_PCLK_ASB2APB_VCCIO156_EN_SHIFT)   /* 0x00000001 */
#define TOPCRU_GATE_CON26_PCLK_IOC_VCCIO156_EN_SHIFT       (1U)
#define TOPCRU_GATE_CON26_PCLK_IOC_VCCIO156_EN_MASK        (0x1U << TOPCRU_GATE_CON26_PCLK_IOC_VCCIO156_EN_SHIFT)       /* 0x00000002 */
#define TOPCRU_GATE_CON26_PCLK_GPIO3_VCCIO156_EN_SHIFT     (2U)
#define TOPCRU_GATE_CON26_PCLK_GPIO3_VCCIO156_EN_MASK      (0x1U << TOPCRU_GATE_CON26_PCLK_GPIO3_VCCIO156_EN_SHIFT)     /* 0x00000004 */
#define TOPCRU_GATE_CON26_PCLK_GPIO4_VCCIO156_EN_SHIFT     (3U)
#define TOPCRU_GATE_CON26_PCLK_GPIO4_VCCIO156_EN_MASK      (0x1U << TOPCRU_GATE_CON26_PCLK_GPIO4_VCCIO156_EN_SHIFT)     /* 0x00000008 */
#define TOPCRU_GATE_CON26_PCLK_SARADC_VCCIO156_EN_SHIFT    (4U)
#define TOPCRU_GATE_CON26_PCLK_SARADC_VCCIO156_EN_MASK     (0x1U << TOPCRU_GATE_CON26_PCLK_SARADC_VCCIO156_EN_SHIFT)    /* 0x00000010 */
/* GATE_CON27 */
#define TOPCRU_GATE_CON27_OFFSET                           (0x36CU)
#define TOPCRU_GATE_CON27_PCLK_MAC100_EN_SHIFT             (0U)
#define TOPCRU_GATE_CON27_PCLK_MAC100_EN_MASK              (0x1U << TOPCRU_GATE_CON27_PCLK_MAC100_EN_SHIFT)             /* 0x00000001 */
#define TOPCRU_GATE_CON27_ACLK_MAC100_EN_SHIFT             (1U)
#define TOPCRU_GATE_CON27_ACLK_MAC100_EN_MASK              (0x1U << TOPCRU_GATE_CON27_ACLK_MAC100_EN_SHIFT)             /* 0x00000002 */
#define TOPCRU_GATE_CON27_CLK_MAC100_50M_CRU_EN_SHIFT      (2U)
#define TOPCRU_GATE_CON27_CLK_MAC100_50M_CRU_EN_MASK       (0x1U << TOPCRU_GATE_CON27_CLK_MAC100_50M_CRU_EN_SHIFT)      /* 0x00000004 */
/* SOFTRST_CON00 */
#define TOPCRU_SOFTRST_CON00_OFFSET                        (0x400U)
/* SOFTRST_CON01 */
#define TOPCRU_SOFTRST_CON01_OFFSET                        (0x404U)
#define TOPCRU_SOFTRST_CON01_ARESETN_TOP_BIU_SHIFT         (0U)
#define TOPCRU_SOFTRST_CON01_ARESETN_TOP_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON01_ARESETN_TOP_BIU_SHIFT)         /* 0x00000001 */
#define TOPCRU_SOFTRST_CON01_ARESETN_TOP_VIO_BIU_SHIFT     (1U)
#define TOPCRU_SOFTRST_CON01_ARESETN_TOP_VIO_BIU_MASK      (0x1U << TOPCRU_SOFTRST_CON01_ARESETN_TOP_VIO_BIU_SHIFT)     /* 0x00000002 */
#define TOPCRU_SOFTRST_CON01_RESETN_REF_PVTPLL_LOGIC_SHIFT (2U)
#define TOPCRU_SOFTRST_CON01_RESETN_REF_PVTPLL_LOGIC_MASK  (0x1U << TOPCRU_SOFTRST_CON01_RESETN_REF_PVTPLL_LOGIC_SHIFT) /* 0x00000004 */
/* SOFTRST_CON03 */
#define TOPCRU_SOFTRST_CON03_OFFSET                        (0x40CU)
#define TOPCRU_SOFTRST_CON03_NCOREPORESET0_SHIFT           (0U)
#define TOPCRU_SOFTRST_CON03_NCOREPORESET0_MASK            (0x1U << TOPCRU_SOFTRST_CON03_NCOREPORESET0_SHIFT)           /* 0x00000001 */
#define TOPCRU_SOFTRST_CON03_NCOREPORESET1_SHIFT           (1U)
#define TOPCRU_SOFTRST_CON03_NCOREPORESET1_MASK            (0x1U << TOPCRU_SOFTRST_CON03_NCOREPORESET1_SHIFT)           /* 0x00000002 */
#define TOPCRU_SOFTRST_CON03_NCOREPORESET2_SHIFT           (2U)
#define TOPCRU_SOFTRST_CON03_NCOREPORESET2_MASK            (0x1U << TOPCRU_SOFTRST_CON03_NCOREPORESET2_SHIFT)           /* 0x00000004 */
#define TOPCRU_SOFTRST_CON03_NCOREPORESET3_SHIFT           (3U)
#define TOPCRU_SOFTRST_CON03_NCOREPORESET3_MASK            (0x1U << TOPCRU_SOFTRST_CON03_NCOREPORESET3_SHIFT)           /* 0x00000008 */
#define TOPCRU_SOFTRST_CON03_NCORESET0_SHIFT               (4U)
#define TOPCRU_SOFTRST_CON03_NCORESET0_MASK                (0x1U << TOPCRU_SOFTRST_CON03_NCORESET0_SHIFT)               /* 0x00000010 */
#define TOPCRU_SOFTRST_CON03_NCORESET1_SHIFT               (5U)
#define TOPCRU_SOFTRST_CON03_NCORESET1_MASK                (0x1U << TOPCRU_SOFTRST_CON03_NCORESET1_SHIFT)               /* 0x00000020 */
#define TOPCRU_SOFTRST_CON03_NCORESET2_SHIFT               (6U)
#define TOPCRU_SOFTRST_CON03_NCORESET2_MASK                (0x1U << TOPCRU_SOFTRST_CON03_NCORESET2_SHIFT)               /* 0x00000040 */
#define TOPCRU_SOFTRST_CON03_NCORESET3_SHIFT               (7U)
#define TOPCRU_SOFTRST_CON03_NCORESET3_MASK                (0x1U << TOPCRU_SOFTRST_CON03_NCORESET3_SHIFT)               /* 0x00000080 */
#define TOPCRU_SOFTRST_CON03_NL2RESET_SHIFT                (8U)
#define TOPCRU_SOFTRST_CON03_NL2RESET_MASK                 (0x1U << TOPCRU_SOFTRST_CON03_NL2RESET_SHIFT)                /* 0x00000100 */
/* SOFTRST_CON04 */
#define TOPCRU_SOFTRST_CON04_OFFSET                        (0x410U)
#define TOPCRU_SOFTRST_CON04_RESETN_DAP_SHIFT              (9U)
#define TOPCRU_SOFTRST_CON04_RESETN_DAP_MASK               (0x1U << TOPCRU_SOFTRST_CON04_RESETN_DAP_SHIFT)              /* 0x00000200 */
#define TOPCRU_SOFTRST_CON04_PRESETN_DBG_DAPLITE_SHIFT     (10U)
#define TOPCRU_SOFTRST_CON04_PRESETN_DBG_DAPLITE_MASK      (0x1U << TOPCRU_SOFTRST_CON04_PRESETN_DBG_DAPLITE_SHIFT)     /* 0x00000400 */
#define TOPCRU_SOFTRST_CON04_RESETN_REF_PVTPLL_CORE_SHIFT  (13U)
#define TOPCRU_SOFTRST_CON04_RESETN_REF_PVTPLL_CORE_MASK   (0x1U << TOPCRU_SOFTRST_CON04_RESETN_REF_PVTPLL_CORE_SHIFT)  /* 0x00002000 */
/* SOFTRST_CON05 */
#define TOPCRU_SOFTRST_CON05_OFFSET                        (0x414U)
#define TOPCRU_SOFTRST_CON05_ARESETN_CORE_BIU_SHIFT        (0U)
#define TOPCRU_SOFTRST_CON05_ARESETN_CORE_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON05_ARESETN_CORE_BIU_SHIFT)        /* 0x00000001 */
#define TOPCRU_SOFTRST_CON05_PRESETN_CORE_BIU_SHIFT        (1U)
#define TOPCRU_SOFTRST_CON05_PRESETN_CORE_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON05_PRESETN_CORE_BIU_SHIFT)        /* 0x00000002 */
#define TOPCRU_SOFTRST_CON05_HRESETN_CORE_BIU_SHIFT        (2U)
#define TOPCRU_SOFTRST_CON05_HRESETN_CORE_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON05_HRESETN_CORE_BIU_SHIFT)        /* 0x00000004 */
/* SOFTRST_CON06 */
#define TOPCRU_SOFTRST_CON06_OFFSET                        (0x418U)
#define TOPCRU_SOFTRST_CON06_ARESETN_NPU_BIU_SHIFT         (2U)
#define TOPCRU_SOFTRST_CON06_ARESETN_NPU_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON06_ARESETN_NPU_BIU_SHIFT)         /* 0x00000004 */
#define TOPCRU_SOFTRST_CON06_HRESETN_NPU_BIU_SHIFT         (3U)
#define TOPCRU_SOFTRST_CON06_HRESETN_NPU_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON06_HRESETN_NPU_BIU_SHIFT)         /* 0x00000008 */
#define TOPCRU_SOFTRST_CON06_ARESETN_RKNN_SHIFT            (4U)
#define TOPCRU_SOFTRST_CON06_ARESETN_RKNN_MASK             (0x1U << TOPCRU_SOFTRST_CON06_ARESETN_RKNN_SHIFT)            /* 0x00000010 */
#define TOPCRU_SOFTRST_CON06_HRESETN_RKNN_SHIFT            (5U)
#define TOPCRU_SOFTRST_CON06_HRESETN_RKNN_MASK             (0x1U << TOPCRU_SOFTRST_CON06_HRESETN_RKNN_SHIFT)            /* 0x00000020 */
#define TOPCRU_SOFTRST_CON06_RESETN_REF_PVTPLL_NPU_SHIFT   (6U)
#define TOPCRU_SOFTRST_CON06_RESETN_REF_PVTPLL_NPU_MASK    (0x1U << TOPCRU_SOFTRST_CON06_RESETN_REF_PVTPLL_NPU_SHIFT)   /* 0x00000040 */
/* SOFTRST_CON08 */
#define TOPCRU_SOFTRST_CON08_OFFSET                        (0x420U)
#define TOPCRU_SOFTRST_CON08_ARESETN_GPU_BIU_SHIFT         (3U)
#define TOPCRU_SOFTRST_CON08_ARESETN_GPU_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON08_ARESETN_GPU_BIU_SHIFT)         /* 0x00000008 */
#define TOPCRU_SOFTRST_CON08_RESETN_GPU_SHIFT              (4U)
#define TOPCRU_SOFTRST_CON08_RESETN_GPU_MASK               (0x1U << TOPCRU_SOFTRST_CON08_RESETN_GPU_SHIFT)              /* 0x00000010 */
#define TOPCRU_SOFTRST_CON08_RESETN_REF_PVTPLL_GPU_SHIFT   (5U)
#define TOPCRU_SOFTRST_CON08_RESETN_REF_PVTPLL_GPU_MASK    (0x1U << TOPCRU_SOFTRST_CON08_RESETN_REF_PVTPLL_GPU_SHIFT)   /* 0x00000020 */
#define TOPCRU_SOFTRST_CON08_RESETN_GPU_BRG_BIU_SHIFT      (8U)
#define TOPCRU_SOFTRST_CON08_RESETN_GPU_BRG_BIU_MASK       (0x1U << TOPCRU_SOFTRST_CON08_RESETN_GPU_BRG_BIU_SHIFT)      /* 0x00000100 */
/* SOFTRST_CON09 */
#define TOPCRU_SOFTRST_CON09_OFFSET                        (0x424U)
#define TOPCRU_SOFTRST_CON09_RESETN_RKVENC_CORE_SHIFT      (0U)
#define TOPCRU_SOFTRST_CON09_RESETN_RKVENC_CORE_MASK       (0x1U << TOPCRU_SOFTRST_CON09_RESETN_RKVENC_CORE_SHIFT)      /* 0x00000001 */
#define TOPCRU_SOFTRST_CON09_ARESETN_VEPU_BIU_SHIFT        (3U)
#define TOPCRU_SOFTRST_CON09_ARESETN_VEPU_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON09_ARESETN_VEPU_BIU_SHIFT)        /* 0x00000008 */
#define TOPCRU_SOFTRST_CON09_HRESETN_VEPU_BIU_SHIFT        (4U)
#define TOPCRU_SOFTRST_CON09_HRESETN_VEPU_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON09_HRESETN_VEPU_BIU_SHIFT)        /* 0x00000010 */
#define TOPCRU_SOFTRST_CON09_ARESETN_RKVENC_SHIFT          (5U)
#define TOPCRU_SOFTRST_CON09_ARESETN_RKVENC_MASK           (0x1U << TOPCRU_SOFTRST_CON09_ARESETN_RKVENC_SHIFT)          /* 0x00000020 */
#define TOPCRU_SOFTRST_CON09_HRESETN_RKVENC_SHIFT          (6U)
#define TOPCRU_SOFTRST_CON09_HRESETN_RKVENC_MASK           (0x1U << TOPCRU_SOFTRST_CON09_HRESETN_RKVENC_SHIFT)          /* 0x00000040 */
/* SOFTRST_CON10 */
#define TOPCRU_SOFTRST_CON10_OFFSET                        (0x428U)
#define TOPCRU_SOFTRST_CON10_RESETN_RKVDEC_HEVC_CA_SHIFT   (2U)
#define TOPCRU_SOFTRST_CON10_RESETN_RKVDEC_HEVC_CA_MASK    (0x1U << TOPCRU_SOFTRST_CON10_RESETN_RKVDEC_HEVC_CA_SHIFT)   /* 0x00000004 */
#define TOPCRU_SOFTRST_CON10_ARESETN_VDPU_BIU_SHIFT        (5U)
#define TOPCRU_SOFTRST_CON10_ARESETN_VDPU_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON10_ARESETN_VDPU_BIU_SHIFT)        /* 0x00000020 */
#define TOPCRU_SOFTRST_CON10_HRESETN_VDPU_BIU_SHIFT        (6U)
#define TOPCRU_SOFTRST_CON10_HRESETN_VDPU_BIU_MASK         (0x1U << TOPCRU_SOFTRST_CON10_HRESETN_VDPU_BIU_SHIFT)        /* 0x00000040 */
#define TOPCRU_SOFTRST_CON10_ARESETN_RKVDEC_SHIFT          (7U)
#define TOPCRU_SOFTRST_CON10_ARESETN_RKVDEC_MASK           (0x1U << TOPCRU_SOFTRST_CON10_ARESETN_RKVDEC_SHIFT)          /* 0x00000080 */
#define TOPCRU_SOFTRST_CON10_HRESETN_RKVDEC_SHIFT          (8U)
#define TOPCRU_SOFTRST_CON10_HRESETN_RKVDEC_MASK           (0x1U << TOPCRU_SOFTRST_CON10_HRESETN_RKVDEC_SHIFT)          /* 0x00000100 */
/* SOFTRST_CON11 */
#define TOPCRU_SOFTRST_CON11_OFFSET                        (0x42CU)
#define TOPCRU_SOFTRST_CON11_ARESETN_VI_BIU_SHIFT          (3U)
#define TOPCRU_SOFTRST_CON11_ARESETN_VI_BIU_MASK           (0x1U << TOPCRU_SOFTRST_CON11_ARESETN_VI_BIU_SHIFT)          /* 0x00000008 */
#define TOPCRU_SOFTRST_CON11_HRESETN_VI_BIU_SHIFT          (4U)
#define TOPCRU_SOFTRST_CON11_HRESETN_VI_BIU_MASK           (0x1U << TOPCRU_SOFTRST_CON11_HRESETN_VI_BIU_SHIFT)          /* 0x00000010 */
#define TOPCRU_SOFTRST_CON11_PRESETN_VI_BIU_SHIFT          (5U)
#define TOPCRU_SOFTRST_CON11_PRESETN_VI_BIU_MASK           (0x1U << TOPCRU_SOFTRST_CON11_PRESETN_VI_BIU_SHIFT)          /* 0x00000020 */
#define TOPCRU_SOFTRST_CON11_RESETN_ISP_SHIFT              (8U)
#define TOPCRU_SOFTRST_CON11_RESETN_ISP_MASK               (0x1U << TOPCRU_SOFTRST_CON11_RESETN_ISP_SHIFT)              /* 0x00000100 */
#define TOPCRU_SOFTRST_CON11_ARESETN_VICAP_SHIFT           (9U)
#define TOPCRU_SOFTRST_CON11_ARESETN_VICAP_MASK            (0x1U << TOPCRU_SOFTRST_CON11_ARESETN_VICAP_SHIFT)           /* 0x00000200 */
#define TOPCRU_SOFTRST_CON11_HRESETN_VICAP_SHIFT           (10U)
#define TOPCRU_SOFTRST_CON11_HRESETN_VICAP_MASK            (0x1U << TOPCRU_SOFTRST_CON11_HRESETN_VICAP_SHIFT)           /* 0x00000400 */
#define TOPCRU_SOFTRST_CON11_DRESETN_VICAP_SHIFT           (11U)
#define TOPCRU_SOFTRST_CON11_DRESETN_VICAP_MASK            (0x1U << TOPCRU_SOFTRST_CON11_DRESETN_VICAP_SHIFT)           /* 0x00000800 */
#define TOPCRU_SOFTRST_CON11_I0RESETN_VICAP_SHIFT          (12U)
#define TOPCRU_SOFTRST_CON11_I0RESETN_VICAP_MASK           (0x1U << TOPCRU_SOFTRST_CON11_I0RESETN_VICAP_SHIFT)          /* 0x00001000 */
#define TOPCRU_SOFTRST_CON11_I1RESETN_VICAP_SHIFT          (13U)
#define TOPCRU_SOFTRST_CON11_I1RESETN_VICAP_MASK           (0x1U << TOPCRU_SOFTRST_CON11_I1RESETN_VICAP_SHIFT)          /* 0x00002000 */
#define TOPCRU_SOFTRST_CON11_I2RESETN_VICAP_SHIFT          (14U)
#define TOPCRU_SOFTRST_CON11_I2RESETN_VICAP_MASK           (0x1U << TOPCRU_SOFTRST_CON11_I2RESETN_VICAP_SHIFT)          /* 0x00004000 */
#define TOPCRU_SOFTRST_CON11_I3RESETN_VICAP_SHIFT          (15U)
#define TOPCRU_SOFTRST_CON11_I3RESETN_VICAP_MASK           (0x1U << TOPCRU_SOFTRST_CON11_I3RESETN_VICAP_SHIFT)          /* 0x00008000 */
/* SOFTRST_CON12 */
#define TOPCRU_SOFTRST_CON12_OFFSET                        (0x430U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST0_SHIFT        (0U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST0_MASK         (0x1U << TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST0_SHIFT)        /* 0x00000001 */
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST1_SHIFT        (1U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST1_MASK         (0x1U << TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST1_SHIFT)        /* 0x00000002 */
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST2_SHIFT        (2U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST2_MASK         (0x1U << TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST2_SHIFT)        /* 0x00000004 */
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST3_SHIFT        (3U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST3_MASK         (0x1U << TOPCRU_SOFTRST_CON12_PRESETN_CSIHOST3_SHIFT)        /* 0x00000008 */
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIPHY0_SHIFT         (4U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIPHY0_MASK          (0x1U << TOPCRU_SOFTRST_CON12_PRESETN_CSIPHY0_SHIFT)         /* 0x00000010 */
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIPHY1_SHIFT         (5U)
#define TOPCRU_SOFTRST_CON12_PRESETN_CSIPHY1_MASK          (0x1U << TOPCRU_SOFTRST_CON12_PRESETN_CSIPHY1_SHIFT)         /* 0x00000020 */
/* SOFTRST_CON13 */
#define TOPCRU_SOFTRST_CON13_OFFSET                        (0x434U)
#define TOPCRU_SOFTRST_CON13_ARESETN_VO_BIU_SHIFT          (3U)
#define TOPCRU_SOFTRST_CON13_ARESETN_VO_BIU_MASK           (0x1U << TOPCRU_SOFTRST_CON13_ARESETN_VO_BIU_SHIFT)          /* 0x00000008 */
#define TOPCRU_SOFTRST_CON13_HRESETN_VO_BIU_SHIFT          (4U)
#define TOPCRU_SOFTRST_CON13_HRESETN_VO_BIU_MASK           (0x1U << TOPCRU_SOFTRST_CON13_HRESETN_VO_BIU_SHIFT)          /* 0x00000010 */
#define TOPCRU_SOFTRST_CON13_ARESETN_VOP_SHIFT             (6U)
#define TOPCRU_SOFTRST_CON13_ARESETN_VOP_MASK              (0x1U << TOPCRU_SOFTRST_CON13_ARESETN_VOP_SHIFT)             /* 0x00000040 */
#define TOPCRU_SOFTRST_CON13_HRESETN_VOP_SHIFT             (7U)
#define TOPCRU_SOFTRST_CON13_HRESETN_VOP_MASK              (0x1U << TOPCRU_SOFTRST_CON13_HRESETN_VOP_SHIFT)             /* 0x00000080 */
#define TOPCRU_SOFTRST_CON13_DRESETN_VOP_SHIFT             (8U)
#define TOPCRU_SOFTRST_CON13_DRESETN_VOP_MASK              (0x1U << TOPCRU_SOFTRST_CON13_DRESETN_VOP_SHIFT)             /* 0x00000100 */
#define TOPCRU_SOFTRST_CON13_DRESETN_VOP1_SHIFT            (9U)
#define TOPCRU_SOFTRST_CON13_DRESETN_VOP1_MASK             (0x1U << TOPCRU_SOFTRST_CON13_DRESETN_VOP1_SHIFT)            /* 0x00000200 */
/* SOFTRST_CON14 */
#define TOPCRU_SOFTRST_CON14_OFFSET                        (0x438U)
#define TOPCRU_SOFTRST_CON14_ARESETN_RGA_BIU_SHIFT         (3U)
#define TOPCRU_SOFTRST_CON14_ARESETN_RGA_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON14_ARESETN_RGA_BIU_SHIFT)         /* 0x00000008 */
#define TOPCRU_SOFTRST_CON14_HRESETN_RGA_BIU_SHIFT         (4U)
#define TOPCRU_SOFTRST_CON14_HRESETN_RGA_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON14_HRESETN_RGA_BIU_SHIFT)         /* 0x00000010 */
#define TOPCRU_SOFTRST_CON14_ARESETN_RGA_SHIFT             (6U)
#define TOPCRU_SOFTRST_CON14_ARESETN_RGA_MASK              (0x1U << TOPCRU_SOFTRST_CON14_ARESETN_RGA_SHIFT)             /* 0x00000040 */
#define TOPCRU_SOFTRST_CON14_HRESETN_RGA_SHIFT             (7U)
#define TOPCRU_SOFTRST_CON14_HRESETN_RGA_MASK              (0x1U << TOPCRU_SOFTRST_CON14_HRESETN_RGA_SHIFT)             /* 0x00000080 */
#define TOPCRU_SOFTRST_CON14_RESETN_RGA_CORE_SHIFT         (8U)
#define TOPCRU_SOFTRST_CON14_RESETN_RGA_CORE_MASK          (0x1U << TOPCRU_SOFTRST_CON14_RESETN_RGA_CORE_SHIFT)         /* 0x00000100 */
#define TOPCRU_SOFTRST_CON14_ARESETN_JDEC_SHIFT            (9U)
#define TOPCRU_SOFTRST_CON14_ARESETN_JDEC_MASK             (0x1U << TOPCRU_SOFTRST_CON14_ARESETN_JDEC_SHIFT)            /* 0x00000200 */
#define TOPCRU_SOFTRST_CON14_HRESETN_JDEC_SHIFT            (10U)
#define TOPCRU_SOFTRST_CON14_HRESETN_JDEC_MASK             (0x1U << TOPCRU_SOFTRST_CON14_HRESETN_JDEC_SHIFT)            /* 0x00000400 */
/* SOFTRST_CON16 */
#define TOPCRU_SOFTRST_CON16_OFFSET                        (0x440U)
#define TOPCRU_SOFTRST_CON16_PRESETN_PHP_BIU_SHIFT         (2U)
#define TOPCRU_SOFTRST_CON16_PRESETN_PHP_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON16_PRESETN_PHP_BIU_SHIFT)         /* 0x00000004 */
#define TOPCRU_SOFTRST_CON16_ARESETN_PHP_BIU_SHIFT         (3U)
#define TOPCRU_SOFTRST_CON16_ARESETN_PHP_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON16_ARESETN_PHP_BIU_SHIFT)         /* 0x00000008 */
#define TOPCRU_SOFTRST_CON16_PRESETN_PCIE20_SHIFT          (7U)
#define TOPCRU_SOFTRST_CON16_PRESETN_PCIE20_MASK           (0x1U << TOPCRU_SOFTRST_CON16_PRESETN_PCIE20_SHIFT)          /* 0x00000080 */
#define TOPCRU_SOFTRST_CON16_RESETN_PCIE20_POWERUP_SHIFT   (8U)
#define TOPCRU_SOFTRST_CON16_RESETN_PCIE20_POWERUP_MASK    (0x1U << TOPCRU_SOFTRST_CON16_RESETN_PCIE20_POWERUP_SHIFT)   /* 0x00000100 */
#define TOPCRU_SOFTRST_CON16_RESETN_USB3OTG_SHIFT          (10U)
#define TOPCRU_SOFTRST_CON16_RESETN_USB3OTG_MASK           (0x1U << TOPCRU_SOFTRST_CON16_RESETN_USB3OTG_SHIFT)          /* 0x00000400 */
/* SOFTRST_CON17 */
#define TOPCRU_SOFTRST_CON17_OFFSET                        (0x444U)
#define TOPCRU_SOFTRST_CON17_RESETN_PIPEPHY_SHIFT          (3U)
#define TOPCRU_SOFTRST_CON17_RESETN_PIPEPHY_MASK           (0x1U << TOPCRU_SOFTRST_CON17_RESETN_PIPEPHY_SHIFT)          /* 0x00000008 */
/* SOFTRST_CON18 */
#define TOPCRU_SOFTRST_CON18_OFFSET                        (0x448U)
#define TOPCRU_SOFTRST_CON18_ARESETN_BUS_BIU_SHIFT         (3U)
#define TOPCRU_SOFTRST_CON18_ARESETN_BUS_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON18_ARESETN_BUS_BIU_SHIFT)         /* 0x00000008 */
#define TOPCRU_SOFTRST_CON18_HRESETN_BUS_BIU_SHIFT         (4U)
#define TOPCRU_SOFTRST_CON18_HRESETN_BUS_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON18_HRESETN_BUS_BIU_SHIFT)         /* 0x00000010 */
#define TOPCRU_SOFTRST_CON18_PRESETN_BUS_BIU_SHIFT         (5U)
#define TOPCRU_SOFTRST_CON18_PRESETN_BUS_BIU_MASK          (0x1U << TOPCRU_SOFTRST_CON18_PRESETN_BUS_BIU_SHIFT)         /* 0x00000020 */
/* SOFTRST_CON19 */
#define TOPCRU_SOFTRST_CON19_OFFSET                        (0x44CU)
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C1_SHIFT            (0U)
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C1_MASK             (0x1U << TOPCRU_SOFTRST_CON19_PRESETN_I2C1_SHIFT)            /* 0x00000001 */
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C2_SHIFT            (1U)
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C2_MASK             (0x1U << TOPCRU_SOFTRST_CON19_PRESETN_I2C2_SHIFT)            /* 0x00000002 */
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C3_SHIFT            (2U)
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C3_MASK             (0x1U << TOPCRU_SOFTRST_CON19_PRESETN_I2C3_SHIFT)            /* 0x00000004 */
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C4_SHIFT            (3U)
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C4_MASK             (0x1U << TOPCRU_SOFTRST_CON19_PRESETN_I2C4_SHIFT)            /* 0x00000008 */
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C5_SHIFT            (4U)
#define TOPCRU_SOFTRST_CON19_PRESETN_I2C5_MASK             (0x1U << TOPCRU_SOFTRST_CON19_PRESETN_I2C5_SHIFT)            /* 0x00000010 */
#define TOPCRU_SOFTRST_CON19_RESETN_I2C1_SHIFT             (6U)
#define TOPCRU_SOFTRST_CON19_RESETN_I2C1_MASK              (0x1U << TOPCRU_SOFTRST_CON19_RESETN_I2C1_SHIFT)             /* 0x00000040 */
#define TOPCRU_SOFTRST_CON19_RESETN_I2C2_SHIFT             (7U)
#define TOPCRU_SOFTRST_CON19_RESETN_I2C2_MASK              (0x1U << TOPCRU_SOFTRST_CON19_RESETN_I2C2_SHIFT)             /* 0x00000080 */
#define TOPCRU_SOFTRST_CON19_RESETN_I2C3_SHIFT             (8U)
#define TOPCRU_SOFTRST_CON19_RESETN_I2C3_MASK              (0x1U << TOPCRU_SOFTRST_CON19_RESETN_I2C3_SHIFT)             /* 0x00000100 */
#define TOPCRU_SOFTRST_CON19_RESETN_I2C4_SHIFT             (9U)
#define TOPCRU_SOFTRST_CON19_RESETN_I2C4_MASK              (0x1U << TOPCRU_SOFTRST_CON19_RESETN_I2C4_SHIFT)             /* 0x00000200 */
#define TOPCRU_SOFTRST_CON19_RESETN_I2C5_SHIFT             (10U)
#define TOPCRU_SOFTRST_CON19_RESETN_I2C5_MASK              (0x1U << TOPCRU_SOFTRST_CON19_RESETN_I2C5_SHIFT)             /* 0x00000400 */
/* SOFTRST_CON20 */
#define TOPCRU_SOFTRST_CON20_OFFSET                        (0x450U)
#define TOPCRU_SOFTRST_CON20_RESETN_BUS_GPIO3_SHIFT        (5U)
#define TOPCRU_SOFTRST_CON20_RESETN_BUS_GPIO3_MASK         (0x1U << TOPCRU_SOFTRST_CON20_RESETN_BUS_GPIO3_SHIFT)        /* 0x00000020 */
#define TOPCRU_SOFTRST_CON20_RESETN_BUS_GPIO4_SHIFT        (6U)
#define TOPCRU_SOFTRST_CON20_RESETN_BUS_GPIO4_MASK         (0x1U << TOPCRU_SOFTRST_CON20_RESETN_BUS_GPIO4_SHIFT)        /* 0x00000040 */
/* SOFTRST_CON21 */
#define TOPCRU_SOFTRST_CON21_OFFSET                        (0x454U)
#define TOPCRU_SOFTRST_CON21_PRESETN_TIMER_SHIFT           (0U)
#define TOPCRU_SOFTRST_CON21_PRESETN_TIMER_MASK            (0x1U << TOPCRU_SOFTRST_CON21_PRESETN_TIMER_SHIFT)           /* 0x00000001 */
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER0_SHIFT           (1U)
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER0_MASK            (0x1U << TOPCRU_SOFTRST_CON21_RESETN_TIMER0_SHIFT)           /* 0x00000002 */
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER1_SHIFT           (2U)
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER1_MASK            (0x1U << TOPCRU_SOFTRST_CON21_RESETN_TIMER1_SHIFT)           /* 0x00000004 */
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER2_SHIFT           (3U)
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER2_MASK            (0x1U << TOPCRU_SOFTRST_CON21_RESETN_TIMER2_SHIFT)           /* 0x00000008 */
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER3_SHIFT           (4U)
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER3_MASK            (0x1U << TOPCRU_SOFTRST_CON21_RESETN_TIMER3_SHIFT)           /* 0x00000010 */
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER4_SHIFT           (5U)
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER4_MASK            (0x1U << TOPCRU_SOFTRST_CON21_RESETN_TIMER4_SHIFT)           /* 0x00000020 */
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER5_SHIFT           (6U)
#define TOPCRU_SOFTRST_CON21_RESETN_TIMER5_MASK            (0x1U << TOPCRU_SOFTRST_CON21_RESETN_TIMER5_SHIFT)           /* 0x00000040 */
#define TOPCRU_SOFTRST_CON21_PRESETN_STIMER_SHIFT          (7U)
#define TOPCRU_SOFTRST_CON21_PRESETN_STIMER_MASK           (0x1U << TOPCRU_SOFTRST_CON21_PRESETN_STIMER_SHIFT)          /* 0x00000080 */
#define TOPCRU_SOFTRST_CON21_RESETN_STIMER0_SHIFT          (8U)
#define TOPCRU_SOFTRST_CON21_RESETN_STIMER0_MASK           (0x1U << TOPCRU_SOFTRST_CON21_RESETN_STIMER0_SHIFT)          /* 0x00000100 */
#define TOPCRU_SOFTRST_CON21_RESETN_STIMER1_SHIFT          (9U)
#define TOPCRU_SOFTRST_CON21_RESETN_STIMER1_MASK           (0x1U << TOPCRU_SOFTRST_CON21_RESETN_STIMER1_SHIFT)          /* 0x00000200 */
/* SOFTRST_CON22 */
#define TOPCRU_SOFTRST_CON22_OFFSET                        (0x458U)
#define TOPCRU_SOFTRST_CON22_PRESETN_WDTNS_SHIFT           (0U)
#define TOPCRU_SOFTRST_CON22_PRESETN_WDTNS_MASK            (0x1U << TOPCRU_SOFTRST_CON22_PRESETN_WDTNS_SHIFT)           /* 0x00000001 */
#define TOPCRU_SOFTRST_CON22_RESETN_WDTNS_SHIFT            (1U)
#define TOPCRU_SOFTRST_CON22_RESETN_WDTNS_MASK             (0x1U << TOPCRU_SOFTRST_CON22_RESETN_WDTNS_SHIFT)            /* 0x00000002 */
#define TOPCRU_SOFTRST_CON22_PRESETN_GRF_SHIFT             (2U)
#define TOPCRU_SOFTRST_CON22_PRESETN_GRF_MASK              (0x1U << TOPCRU_SOFTRST_CON22_PRESETN_GRF_SHIFT)             /* 0x00000004 */
#define TOPCRU_SOFTRST_CON22_PRESETN_SGRF_SHIFT            (3U)
#define TOPCRU_SOFTRST_CON22_PRESETN_SGRF_MASK             (0x1U << TOPCRU_SOFTRST_CON22_PRESETN_SGRF_SHIFT)            /* 0x00000008 */
#define TOPCRU_SOFTRST_CON22_PRESETN_MAILBOX_SHIFT         (4U)
#define TOPCRU_SOFTRST_CON22_PRESETN_MAILBOX_MASK          (0x1U << TOPCRU_SOFTRST_CON22_PRESETN_MAILBOX_SHIFT)         /* 0x00000010 */
#define TOPCRU_SOFTRST_CON22_PRESETN_INTC_SHIFT            (5U)
#define TOPCRU_SOFTRST_CON22_PRESETN_INTC_MASK             (0x1U << TOPCRU_SOFTRST_CON22_PRESETN_INTC_SHIFT)            /* 0x00000020 */
#define TOPCRU_SOFTRST_CON22_ARESETN_BUS_GIC400_SHIFT      (6U)
#define TOPCRU_SOFTRST_CON22_ARESETN_BUS_GIC400_MASK       (0x1U << TOPCRU_SOFTRST_CON22_ARESETN_BUS_GIC400_SHIFT)      /* 0x00000040 */
#define TOPCRU_SOFTRST_CON22_ARESETN_BUS_GIC400_DEBUG_SHIFT (7U)
#define TOPCRU_SOFTRST_CON22_ARESETN_BUS_GIC400_DEBUG_MASK (0x1U << TOPCRU_SOFTRST_CON22_ARESETN_BUS_GIC400_DEBUG_SHIFT) /* 0x00000080 */
/* SOFTRST_CON23 */
#define TOPCRU_SOFTRST_CON23_OFFSET                        (0x45CU)
#define TOPCRU_SOFTRST_CON23_ARESETN_BUS_SPINLOCK_SHIFT    (0U)
#define TOPCRU_SOFTRST_CON23_ARESETN_BUS_SPINLOCK_MASK     (0x1U << TOPCRU_SOFTRST_CON23_ARESETN_BUS_SPINLOCK_SHIFT)    /* 0x00000001 */
#define TOPCRU_SOFTRST_CON23_ARESETN_DCF_SHIFT             (1U)
#define TOPCRU_SOFTRST_CON23_ARESETN_DCF_MASK              (0x1U << TOPCRU_SOFTRST_CON23_ARESETN_DCF_SHIFT)             /* 0x00000002 */
#define TOPCRU_SOFTRST_CON23_PRESETN_DCF_SHIFT             (2U)
#define TOPCRU_SOFTRST_CON23_PRESETN_DCF_MASK              (0x1U << TOPCRU_SOFTRST_CON23_PRESETN_DCF_SHIFT)             /* 0x00000004 */
#define TOPCRU_SOFTRST_CON23_FRESETN_BUS_CM0_CORE_SHIFT    (3U)
#define TOPCRU_SOFTRST_CON23_FRESETN_BUS_CM0_CORE_MASK     (0x1U << TOPCRU_SOFTRST_CON23_FRESETN_BUS_CM0_CORE_SHIFT)    /* 0x00000008 */
#define TOPCRU_SOFTRST_CON23_TRESETN_BUS_CM0_JTAG_SHIFT    (5U)
#define TOPCRU_SOFTRST_CON23_TRESETN_BUS_CM0_JTAG_MASK     (0x1U << TOPCRU_SOFTRST_CON23_TRESETN_BUS_CM0_JTAG_SHIFT)    /* 0x00000020 */
#define TOPCRU_SOFTRST_CON23_HRESETN_ICACHE_SHIFT          (8U)
#define TOPCRU_SOFTRST_CON23_HRESETN_ICACHE_MASK           (0x1U << TOPCRU_SOFTRST_CON23_HRESETN_ICACHE_SHIFT)          /* 0x00000100 */
#define TOPCRU_SOFTRST_CON23_HRESETN_DCACHE_SHIFT          (9U)
#define TOPCRU_SOFTRST_CON23_HRESETN_DCACHE_MASK           (0x1U << TOPCRU_SOFTRST_CON23_HRESETN_DCACHE_SHIFT)          /* 0x00000200 */
/* SOFTRST_CON24 */
#define TOPCRU_SOFTRST_CON24_OFFSET                        (0x460U)
#define TOPCRU_SOFTRST_CON24_PRESETN_TSADC_SHIFT           (0U)
#define TOPCRU_SOFTRST_CON24_PRESETN_TSADC_MASK            (0x1U << TOPCRU_SOFTRST_CON24_PRESETN_TSADC_SHIFT)           /* 0x00000001 */
#define TOPCRU_SOFTRST_CON24_RESETN_TSADC_SHIFT            (1U)
#define TOPCRU_SOFTRST_CON24_RESETN_TSADC_MASK             (0x1U << TOPCRU_SOFTRST_CON24_RESETN_TSADC_SHIFT)            /* 0x00000002 */
#define TOPCRU_SOFTRST_CON24_RESETN_TSADCPHY_SHIFT         (2U)
#define TOPCRU_SOFTRST_CON24_RESETN_TSADCPHY_MASK          (0x1U << TOPCRU_SOFTRST_CON24_RESETN_TSADCPHY_SHIFT)         /* 0x00000004 */
#define TOPCRU_SOFTRST_CON24_PRESETN_DFT2APB_SHIFT         (4U)
#define TOPCRU_SOFTRST_CON24_PRESETN_DFT2APB_MASK          (0x1U << TOPCRU_SOFTRST_CON24_PRESETN_DFT2APB_SHIFT)         /* 0x00000010 */
/* SOFTRST_CON25 */
#define TOPCRU_SOFTRST_CON25_OFFSET                        (0x464U)
#define TOPCRU_SOFTRST_CON25_ARESETN_GMAC_SHIFT            (1U)
#define TOPCRU_SOFTRST_CON25_ARESETN_GMAC_MASK             (0x1U << TOPCRU_SOFTRST_CON25_ARESETN_GMAC_SHIFT)            /* 0x00000002 */
#define TOPCRU_SOFTRST_CON25_PRESETN_APB2ASB_VCCIO156_SHIFT (5U)
#define TOPCRU_SOFTRST_CON25_PRESETN_APB2ASB_VCCIO156_MASK (0x1U << TOPCRU_SOFTRST_CON25_PRESETN_APB2ASB_VCCIO156_SHIFT) /* 0x00000020 */
#define TOPCRU_SOFTRST_CON25_PRESETN_DSIPHY_SHIFT          (8U)
#define TOPCRU_SOFTRST_CON25_PRESETN_DSIPHY_MASK           (0x1U << TOPCRU_SOFTRST_CON25_PRESETN_DSIPHY_SHIFT)          /* 0x00000100 */
#define TOPCRU_SOFTRST_CON25_PRESETN_DSITX_SHIFT           (9U)
#define TOPCRU_SOFTRST_CON25_PRESETN_DSITX_MASK            (0x1U << TOPCRU_SOFTRST_CON25_PRESETN_DSITX_SHIFT)           /* 0x00000200 */
#define TOPCRU_SOFTRST_CON25_PRESETN_CPU_EMA_DET_SHIFT     (10U)
#define TOPCRU_SOFTRST_CON25_PRESETN_CPU_EMA_DET_MASK      (0x1U << TOPCRU_SOFTRST_CON25_PRESETN_CPU_EMA_DET_SHIFT)     /* 0x00000400 */
#define TOPCRU_SOFTRST_CON25_PRESETN_HASH_SHIFT            (11U)
#define TOPCRU_SOFTRST_CON25_PRESETN_HASH_MASK             (0x1U << TOPCRU_SOFTRST_CON25_PRESETN_HASH_SHIFT)            /* 0x00000800 */
#define TOPCRU_SOFTRST_CON25_PRESETN_TOPCRU_SHIFT          (15U)
#define TOPCRU_SOFTRST_CON25_PRESETN_TOPCRU_MASK           (0x1U << TOPCRU_SOFTRST_CON25_PRESETN_TOPCRU_SHIFT)          /* 0x00008000 */
/* SOFTRST_CON26 */
#define TOPCRU_SOFTRST_CON26_OFFSET                        (0x468U)
#define TOPCRU_SOFTRST_CON26_PRESETN_ASB2APB_VCCIO156_SHIFT (0U)
#define TOPCRU_SOFTRST_CON26_PRESETN_ASB2APB_VCCIO156_MASK (0x1U << TOPCRU_SOFTRST_CON26_PRESETN_ASB2APB_VCCIO156_SHIFT) /* 0x00000001 */
#define TOPCRU_SOFTRST_CON26_PRESETN_IOC_VCCIO156_SHIFT    (1U)
#define TOPCRU_SOFTRST_CON26_PRESETN_IOC_VCCIO156_MASK     (0x1U << TOPCRU_SOFTRST_CON26_PRESETN_IOC_VCCIO156_SHIFT)    /* 0x00000002 */
#define TOPCRU_SOFTRST_CON26_PRESETN_GPIO3_VCCIO156_SHIFT  (2U)
#define TOPCRU_SOFTRST_CON26_PRESETN_GPIO3_VCCIO156_MASK   (0x1U << TOPCRU_SOFTRST_CON26_PRESETN_GPIO3_VCCIO156_SHIFT)  /* 0x00000004 */
#define TOPCRU_SOFTRST_CON26_PRESETN_GPIO4_VCCIO156_SHIFT  (3U)
#define TOPCRU_SOFTRST_CON26_PRESETN_GPIO4_VCCIO156_MASK   (0x1U << TOPCRU_SOFTRST_CON26_PRESETN_GPIO4_VCCIO156_SHIFT)  /* 0x00000008 */
#define TOPCRU_SOFTRST_CON26_PRESETN_SARADC_VCCIO156_SHIFT (4U)
#define TOPCRU_SOFTRST_CON26_PRESETN_SARADC_VCCIO156_MASK  (0x1U << TOPCRU_SOFTRST_CON26_PRESETN_SARADC_VCCIO156_SHIFT) /* 0x00000010 */
#define TOPCRU_SOFTRST_CON26_RESETN_SARADC_VCCIO156_SHIFT  (5U)
#define TOPCRU_SOFTRST_CON26_RESETN_SARADC_VCCIO156_MASK   (0x1U << TOPCRU_SOFTRST_CON26_RESETN_SARADC_VCCIO156_SHIFT)  /* 0x00000020 */
#define TOPCRU_SOFTRST_CON26_RESETN_SARADC_VCCIO156_PHY_SHIFT (6U)
#define TOPCRU_SOFTRST_CON26_RESETN_SARADC_VCCIO156_PHY_MASK (0x1U << TOPCRU_SOFTRST_CON26_RESETN_SARADC_VCCIO156_PHY_SHIFT) /* 0x00000040 */
/* SOFTRST_CON27 */
#define TOPCRU_SOFTRST_CON27_OFFSET                        (0x46CU)
#define TOPCRU_SOFTRST_CON27_ARESETN_MAC100_SHIFT          (1U)
#define TOPCRU_SOFTRST_CON27_ARESETN_MAC100_MASK           (0x1U << TOPCRU_SOFTRST_CON27_ARESETN_MAC100_SHIFT)          /* 0x00000002 */
/* SSGTBL0_3 */
#define TOPCRU_SSGTBL0_3_OFFSET                            (0x500U)
#define TOPCRU_SSGTBL0_3_SSGTBL0_3_SHIFT                   (0U)
#define TOPCRU_SSGTBL0_3_SSGTBL0_3_MASK                    (0xFFFFFFFFU << TOPCRU_SSGTBL0_3_SSGTBL0_3_SHIFT)            /* 0xFFFFFFFF */
/* SSGTBL4_7 */
#define TOPCRU_SSGTBL4_7_OFFSET                            (0x504U)
#define TOPCRU_SSGTBL4_7_SSGTBL4_7_SHIFT                   (0U)
#define TOPCRU_SSGTBL4_7_SSGTBL4_7_MASK                    (0xFFFFFFFFU << TOPCRU_SSGTBL4_7_SSGTBL4_7_SHIFT)            /* 0xFFFFFFFF */
/* SSGTBL8_11 */
#define TOPCRU_SSGTBL8_11_OFFSET                           (0x508U)
#define TOPCRU_SSGTBL8_11_SSGTBL8_11_SHIFT                 (0U)
#define TOPCRU_SSGTBL8_11_SSGTBL8_11_MASK                  (0xFFFFFFFFU << TOPCRU_SSGTBL8_11_SSGTBL8_11_SHIFT)          /* 0xFFFFFFFF */
/* SSGTBL12_15 */
#define TOPCRU_SSGTBL12_15_OFFSET                          (0x50CU)
#define TOPCRU_SSGTBL12_15_SSGTBL12_15_SHIFT               (0U)
#define TOPCRU_SSGTBL12_15_SSGTBL12_15_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL12_15_SSGTBL12_15_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL16_19 */
#define TOPCRU_SSGTBL16_19_OFFSET                          (0x510U)
#define TOPCRU_SSGTBL16_19_SSGTBL16_19_SHIFT               (0U)
#define TOPCRU_SSGTBL16_19_SSGTBL16_19_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL16_19_SSGTBL16_19_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL20_23 */
#define TOPCRU_SSGTBL20_23_OFFSET                          (0x514U)
#define TOPCRU_SSGTBL20_23_SSGTBL20_23_SHIFT               (0U)
#define TOPCRU_SSGTBL20_23_SSGTBL20_23_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL20_23_SSGTBL20_23_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL24_27 */
#define TOPCRU_SSGTBL24_27_OFFSET                          (0x518U)
#define TOPCRU_SSGTBL24_27_SSGTBL24_27_SHIFT               (0U)
#define TOPCRU_SSGTBL24_27_SSGTBL24_27_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL24_27_SSGTBL24_27_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL28_31 */
#define TOPCRU_SSGTBL28_31_OFFSET                          (0x51CU)
#define TOPCRU_SSGTBL28_31_SSGTBL28_31_SHIFT               (0U)
#define TOPCRU_SSGTBL28_31_SSGTBL28_31_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL28_31_SSGTBL28_31_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL32_35 */
#define TOPCRU_SSGTBL32_35_OFFSET                          (0x520U)
#define TOPCRU_SSGTBL32_35_SSGTBL32_35_SHIFT               (0U)
#define TOPCRU_SSGTBL32_35_SSGTBL32_35_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL32_35_SSGTBL32_35_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL36_39 */
#define TOPCRU_SSGTBL36_39_OFFSET                          (0x524U)
#define TOPCRU_SSGTBL36_39_SSGTBL36_39_SHIFT               (0U)
#define TOPCRU_SSGTBL36_39_SSGTBL36_39_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL36_39_SSGTBL36_39_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL40_43 */
#define TOPCRU_SSGTBL40_43_OFFSET                          (0x528U)
#define TOPCRU_SSGTBL40_43_SSGTBL40_43_SHIFT               (0U)
#define TOPCRU_SSGTBL40_43_SSGTBL40_43_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL40_43_SSGTBL40_43_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL44_47 */
#define TOPCRU_SSGTBL44_47_OFFSET                          (0x52CU)
#define TOPCRU_SSGTBL44_47_SSGTBL44_47_SHIFT               (0U)
#define TOPCRU_SSGTBL44_47_SSGTBL44_47_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL44_47_SSGTBL44_47_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL48_51 */
#define TOPCRU_SSGTBL48_51_OFFSET                          (0x530U)
#define TOPCRU_SSGTBL48_51_SSGTBL48_51_SHIFT               (0U)
#define TOPCRU_SSGTBL48_51_SSGTBL48_51_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL48_51_SSGTBL48_51_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL52_55 */
#define TOPCRU_SSGTBL52_55_OFFSET                          (0x534U)
#define TOPCRU_SSGTBL52_55_SSGTBL52_55_SHIFT               (0U)
#define TOPCRU_SSGTBL52_55_SSGTBL52_55_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL52_55_SSGTBL52_55_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL56_59 */
#define TOPCRU_SSGTBL56_59_OFFSET                          (0x538U)
#define TOPCRU_SSGTBL56_59_SSGTBL56_59_SHIFT               (0U)
#define TOPCRU_SSGTBL56_59_SSGTBL56_59_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL56_59_SSGTBL56_59_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL60_63 */
#define TOPCRU_SSGTBL60_63_OFFSET                          (0x53CU)
#define TOPCRU_SSGTBL60_63_SSGTBL60_63_SHIFT               (0U)
#define TOPCRU_SSGTBL60_63_SSGTBL60_63_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL60_63_SSGTBL60_63_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL64_67 */
#define TOPCRU_SSGTBL64_67_OFFSET                          (0x540U)
#define TOPCRU_SSGTBL64_67_SSGTBL64_67_SHIFT               (0U)
#define TOPCRU_SSGTBL64_67_SSGTBL64_67_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL64_67_SSGTBL64_67_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL68_71 */
#define TOPCRU_SSGTBL68_71_OFFSET                          (0x544U)
#define TOPCRU_SSGTBL68_71_SSGTBL68_71_SHIFT               (0U)
#define TOPCRU_SSGTBL68_71_SSGTBL68_71_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL68_71_SSGTBL68_71_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL72_75 */
#define TOPCRU_SSGTBL72_75_OFFSET                          (0x548U)
#define TOPCRU_SSGTBL72_75_SSGTBL72_75_SHIFT               (0U)
#define TOPCRU_SSGTBL72_75_SSGTBL72_75_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL72_75_SSGTBL72_75_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL76_79 */
#define TOPCRU_SSGTBL76_79_OFFSET                          (0x54CU)
#define TOPCRU_SSGTBL76_79_SSGTBL76_79_SHIFT               (0U)
#define TOPCRU_SSGTBL76_79_SSGTBL76_79_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL76_79_SSGTBL76_79_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL80_83 */
#define TOPCRU_SSGTBL80_83_OFFSET                          (0x550U)
#define TOPCRU_SSGTBL80_83_SSGTBL80_83_SHIFT               (0U)
#define TOPCRU_SSGTBL80_83_SSGTBL80_83_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL80_83_SSGTBL80_83_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL84_87 */
#define TOPCRU_SSGTBL84_87_OFFSET                          (0x554U)
#define TOPCRU_SSGTBL84_87_SSGTBL84_87_SHIFT               (0U)
#define TOPCRU_SSGTBL84_87_SSGTBL84_87_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL84_87_SSGTBL84_87_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL88_91 */
#define TOPCRU_SSGTBL88_91_OFFSET                          (0x558U)
#define TOPCRU_SSGTBL88_91_SSGTBL88_91_SHIFT               (0U)
#define TOPCRU_SSGTBL88_91_SSGTBL88_91_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL88_91_SSGTBL88_91_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL92_95 */
#define TOPCRU_SSGTBL92_95_OFFSET                          (0x55CU)
#define TOPCRU_SSGTBL92_95_SSGTBL92_95_SHIFT               (0U)
#define TOPCRU_SSGTBL92_95_SSGTBL92_95_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL92_95_SSGTBL92_95_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL96_99 */
#define TOPCRU_SSGTBL96_99_OFFSET                          (0x560U)
#define TOPCRU_SSGTBL96_99_SSGTBL96_99_SHIFT               (0U)
#define TOPCRU_SSGTBL96_99_SSGTBL96_99_MASK                (0xFFFFFFFFU << TOPCRU_SSGTBL96_99_SSGTBL96_99_SHIFT)        /* 0xFFFFFFFF */
/* SSGTBL100_103 */
#define TOPCRU_SSGTBL100_103_OFFSET                        (0x564U)
#define TOPCRU_SSGTBL100_103_SSGTBL100_103_SHIFT           (0U)
#define TOPCRU_SSGTBL100_103_SSGTBL100_103_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL100_103_SSGTBL100_103_SHIFT)    /* 0xFFFFFFFF */
/* SSGTBL104_107 */
#define TOPCRU_SSGTBL104_107_OFFSET                        (0x568U)
#define TOPCRU_SSGTBL104_107_SSGTBL104_107_SHIFT           (0U)
#define TOPCRU_SSGTBL104_107_SSGTBL104_107_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL104_107_SSGTBL104_107_SHIFT)    /* 0xFFFFFFFF */
/* SSGTBL108_111 */
#define TOPCRU_SSGTBL108_111_OFFSET                        (0x56CU)
#define TOPCRU_SSGTBL108_111_SSGTBL108_111_SHIFT           (0U)
#define TOPCRU_SSGTBL108_111_SSGTBL108_111_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL108_111_SSGTBL108_111_SHIFT)    /* 0xFFFFFFFF */
/* SSGTBL112_115 */
#define TOPCRU_SSGTBL112_115_OFFSET                        (0x570U)
#define TOPCRU_SSGTBL112_115_SSGTBL112_115_SHIFT           (0U)
#define TOPCRU_SSGTBL112_115_SSGTBL112_115_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL112_115_SSGTBL112_115_SHIFT)    /* 0xFFFFFFFF */
/* SSGTBL116_119 */
#define TOPCRU_SSGTBL116_119_OFFSET                        (0x574U)
#define TOPCRU_SSGTBL116_119_SSGTBL116_119_SHIFT           (0U)
#define TOPCRU_SSGTBL116_119_SSGTBL116_119_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL116_119_SSGTBL116_119_SHIFT)    /* 0xFFFFFFFF */
/* SSGTBL120_123 */
#define TOPCRU_SSGTBL120_123_OFFSET                        (0x578U)
#define TOPCRU_SSGTBL120_123_SSGTBL120_123_SHIFT           (0U)
#define TOPCRU_SSGTBL120_123_SSGTBL120_123_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL120_123_SSGTBL120_123_SHIFT)    /* 0xFFFFFFFF */
/* SSGTBL124_127 */
#define TOPCRU_SSGTBL124_127_OFFSET                        (0x57CU)
#define TOPCRU_SSGTBL124_127_SSGTBL124_127_SHIFT           (0U)
#define TOPCRU_SSGTBL124_127_SSGTBL124_127_MASK            (0xFFFFFFFFU << TOPCRU_SSGTBL124_127_SSGTBL124_127_SHIFT)    /* 0xFFFFFFFF */
/* MODE_CON00 */
#define TOPCRU_MODE_CON00_OFFSET                           (0x600U)
#define TOPCRU_MODE_CON00_CLK_APLL_MODE_SHIFT              (0U)
#define TOPCRU_MODE_CON00_CLK_APLL_MODE_MASK               (0x3U << TOPCRU_MODE_CON00_CLK_APLL_MODE_SHIFT)              /* 0x00000003 */
#define TOPCRU_MODE_CON00_CLK_GPLL_MODE_SHIFT              (2U)
#define TOPCRU_MODE_CON00_CLK_GPLL_MODE_MASK               (0x3U << TOPCRU_MODE_CON00_CLK_GPLL_MODE_SHIFT)              /* 0x0000000C */
#define TOPCRU_MODE_CON00_CLK_VPLL_MODE_SHIFT              (6U)
#define TOPCRU_MODE_CON00_CLK_VPLL_MODE_MASK               (0x3U << TOPCRU_MODE_CON00_CLK_VPLL_MODE_SHIFT)              /* 0x000000C0 */
#define TOPCRU_MODE_CON00_CLK_HPLL_MODE_SHIFT              (8U)
#define TOPCRU_MODE_CON00_CLK_HPLL_MODE_MASK               (0x3U << TOPCRU_MODE_CON00_CLK_HPLL_MODE_SHIFT)              /* 0x00000300 */
/* GLB_CNT_TH */
#define TOPCRU_GLB_CNT_TH_OFFSET                           (0x610U)
#define TOPCRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT (0U)
#define TOPCRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_MASK (0xFFFFU << TOPCRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT) /* 0x0000FFFF */
#define TOPCRU_GLB_CNT_TH_RESERVED_SHIFT                   (16U)
#define TOPCRU_GLB_CNT_TH_RESERVED_MASK                    (0xFFFFU << TOPCRU_GLB_CNT_TH_RESERVED_SHIFT)                /* 0xFFFF0000 */
/* GLB_SRST_FST */
#define TOPCRU_GLB_SRST_FST_OFFSET                         (0x614U)
#define TOPCRU_GLB_SRST_FST_GLB_SRST_FST_SHIFT             (0U)
#define TOPCRU_GLB_SRST_FST_GLB_SRST_FST_MASK              (0xFFFFU << TOPCRU_GLB_SRST_FST_GLB_SRST_FST_SHIFT)          /* 0x0000FFFF */
/* GLB_SRST_SND */
#define TOPCRU_GLB_SRST_SND_OFFSET                         (0x618U)
#define TOPCRU_GLB_SRST_SND_GLB_SRST_SND_SHIFT             (0U)
#define TOPCRU_GLB_SRST_SND_GLB_SRST_SND_MASK              (0xFFFFU << TOPCRU_GLB_SRST_SND_GLB_SRST_SND_SHIFT)          /* 0x0000FFFF */
/* GLB_RST_CON */
#define TOPCRU_GLB_RST_CON_OFFSET                          (0x61CU)
#define TOPCRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT       (0U)
#define TOPCRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_MASK        (0x1U << TOPCRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT)       /* 0x00000001 */
#define TOPCRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT         (1U)
#define TOPCRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK          (0x1U << TOPCRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT)         /* 0x00000002 */
#define TOPCRU_GLB_RST_CON_PMU_SRST_GLB_CTRL_SHIFT         (2U)
#define TOPCRU_GLB_RST_CON_PMU_SRST_GLB_CTRL_MASK          (0x1U << TOPCRU_GLB_RST_CON_PMU_SRST_GLB_CTRL_SHIFT)         /* 0x00000004 */
#define TOPCRU_GLB_RST_CON_PMU_SRST_GLB_EN_SHIFT           (3U)
#define TOPCRU_GLB_RST_CON_PMU_SRST_GLB_EN_MASK            (0x1U << TOPCRU_GLB_RST_CON_PMU_SRST_GLB_EN_SHIFT)           /* 0x00000008 */
#define TOPCRU_GLB_RST_CON_PMU_SRST_WDT_EN_SHIFT           (4U)
#define TOPCRU_GLB_RST_CON_PMU_SRST_WDT_EN_MASK            (0x1U << TOPCRU_GLB_RST_CON_PMU_SRST_WDT_EN_SHIFT)           /* 0x00000010 */
#define TOPCRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_SHIFT   (6U)
#define TOPCRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_MASK    (0x1U << TOPCRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_SHIFT)   /* 0x00000040 */
#define TOPCRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT          (7U)
#define TOPCRU_GLB_RST_CON_WDT_RESET_EXT_EN_MASK           (0x1U << TOPCRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT)          /* 0x00000080 */
#define TOPCRU_GLB_RST_CON_PMUSGRF_CRC_CHK_GLB_SRST_CTRL_ENABLE_SHIFT (11U)
#define TOPCRU_GLB_RST_CON_PMUSGRF_CRC_CHK_GLB_SRST_CTRL_ENABLE_MASK (0x1U << TOPCRU_GLB_RST_CON_PMUSGRF_CRC_CHK_GLB_SRST_CTRL_ENABLE_SHIFT) /* 0x00000800 */
#define TOPCRU_GLB_RST_CON_PMUSGRF_CRC_CHK_GLB_SRST_CTRL_SHIFT (15U)
#define TOPCRU_GLB_RST_CON_PMUSGRF_CRC_CHK_GLB_SRST_CTRL_MASK (0x1U << TOPCRU_GLB_RST_CON_PMUSGRF_CRC_CHK_GLB_SRST_CTRL_SHIFT) /* 0x00008000 */
/* GLB_RST_ST */
#define TOPCRU_GLB_RST_ST_OFFSET                           (0x620U)
#define TOPCRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT             (0U)
#define TOPCRU_GLB_RST_ST_FST_GLB_RST_ST_MASK              (0x1U << TOPCRU_GLB_RST_ST_FST_GLB_RST_ST_SHIFT)             /* 0x00000001 */
#define TOPCRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT             (1U)
#define TOPCRU_GLB_RST_ST_SND_GLB_RST_ST_MASK              (0x1U << TOPCRU_GLB_RST_ST_SND_GLB_RST_ST_SHIFT)             /* 0x00000002 */
#define TOPCRU_GLB_RST_ST_FST_GLB_TSADC_RST_ST_SHIFT       (2U)
#define TOPCRU_GLB_RST_ST_FST_GLB_TSADC_RST_ST_MASK        (0x1U << TOPCRU_GLB_RST_ST_FST_GLB_TSADC_RST_ST_SHIFT)       /* 0x00000004 */
#define TOPCRU_GLB_RST_ST_SND_GLB_TSADC_RST_ST_SHIFT       (3U)
#define TOPCRU_GLB_RST_ST_SND_GLB_TSADC_RST_ST_MASK        (0x1U << TOPCRU_GLB_RST_ST_SND_GLB_TSADC_RST_ST_SHIFT)       /* 0x00000008 */
#define TOPCRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT         (4U)
#define TOPCRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_MASK          (0x1U << TOPCRU_GLB_RST_ST_FST_GLB_WDT_RST_ST_SHIFT)         /* 0x00000010 */
#define TOPCRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT         (5U)
#define TOPCRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_MASK          (0x1U << TOPCRU_GLB_RST_ST_SND_GLB_WDT_RST_ST_SHIFT)         /* 0x00000020 */
#define TOPCRU_GLB_RST_ST_GLB_PMUSGRF_CRC_RST_ST_SHIFT     (9U)
#define TOPCRU_GLB_RST_ST_GLB_PMUSGRF_CRC_RST_ST_MASK      (0x1U << TOPCRU_GLB_RST_ST_GLB_PMUSGRF_CRC_RST_ST_SHIFT)     /* 0x00000200 */
/* SDMMC0_CON0 */
#define TOPCRU_SDMMC0_CON0_OFFSET                          (0x624U)
#define TOPCRU_SDMMC0_CON0_INIT_STATE_SHIFT                (0U)
#define TOPCRU_SDMMC0_CON0_INIT_STATE_MASK                 (0x1U << TOPCRU_SDMMC0_CON0_INIT_STATE_SHIFT)                /* 0x00000001 */
#define TOPCRU_SDMMC0_CON0_DRV_DEGREE_SHIFT                (1U)
#define TOPCRU_SDMMC0_CON0_DRV_DEGREE_MASK                 (0x3U << TOPCRU_SDMMC0_CON0_DRV_DEGREE_SHIFT)                /* 0x00000006 */
#define TOPCRU_SDMMC0_CON0_DRV_DELAYNUM_SHIFT              (3U)
#define TOPCRU_SDMMC0_CON0_DRV_DELAYNUM_MASK               (0xFFU << TOPCRU_SDMMC0_CON0_DRV_DELAYNUM_SHIFT)             /* 0x000007F8 */
#define TOPCRU_SDMMC0_CON0_DRV_SEL_SHIFT                   (11U)
#define TOPCRU_SDMMC0_CON0_DRV_SEL_MASK                    (0x1U << TOPCRU_SDMMC0_CON0_DRV_SEL_SHIFT)                   /* 0x00000800 */
#define TOPCRU_SDMMC0_CON0_TEST_CLKOUT_0_SEL_SHIFT         (12U)
#define TOPCRU_SDMMC0_CON0_TEST_CLKOUT_0_SEL_MASK          (0x1U << TOPCRU_SDMMC0_CON0_TEST_CLKOUT_0_SEL_SHIFT)         /* 0x00001000 */
#define TOPCRU_SDMMC0_CON0_RAM_CLKGAT_DISABLE_SHIFT        (13U)
#define TOPCRU_SDMMC0_CON0_RAM_CLKGAT_DISABLE_MASK         (0x1U << TOPCRU_SDMMC0_CON0_RAM_CLKGAT_DISABLE_SHIFT)        /* 0x00002000 */
/* SDMMC0_CON1 */
#define TOPCRU_SDMMC0_CON1_OFFSET                          (0x628U)
#define TOPCRU_SDMMC0_CON1_SAMPLE_DEGREE_SHIFT             (1U)
#define TOPCRU_SDMMC0_CON1_SAMPLE_DEGREE_MASK              (0x3U << TOPCRU_SDMMC0_CON1_SAMPLE_DEGREE_SHIFT)             /* 0x00000006 */
#define TOPCRU_SDMMC0_CON1_SAMPLE_DELAYNUM_SHIFT           (3U)
#define TOPCRU_SDMMC0_CON1_SAMPLE_DELAYNUM_MASK            (0xFFU << TOPCRU_SDMMC0_CON1_SAMPLE_DELAYNUM_SHIFT)          /* 0x000007F8 */
#define TOPCRU_SDMMC0_CON1_SAMPLE_SEL_SHIFT                (11U)
#define TOPCRU_SDMMC0_CON1_SAMPLE_SEL_MASK                 (0x1U << TOPCRU_SDMMC0_CON1_SAMPLE_SEL_SHIFT)                /* 0x00000800 */
#define TOPCRU_SDMMC0_CON1_TEST_CLKOUT_1_SEL_SHIFT         (12U)
#define TOPCRU_SDMMC0_CON1_TEST_CLKOUT_1_SEL_MASK          (0x1U << TOPCRU_SDMMC0_CON1_TEST_CLKOUT_1_SEL_SHIFT)         /* 0x00001000 */
/* SDMMC1_CON0 */
#define TOPCRU_SDMMC1_CON0_OFFSET                          (0x62CU)
#define TOPCRU_SDMMC1_CON0_INIT_STATE_SHIFT                (0U)
#define TOPCRU_SDMMC1_CON0_INIT_STATE_MASK                 (0x1U << TOPCRU_SDMMC1_CON0_INIT_STATE_SHIFT)                /* 0x00000001 */
#define TOPCRU_SDMMC1_CON0_DRV_DEGREE_SHIFT                (1U)
#define TOPCRU_SDMMC1_CON0_DRV_DEGREE_MASK                 (0x3U << TOPCRU_SDMMC1_CON0_DRV_DEGREE_SHIFT)                /* 0x00000006 */
#define TOPCRU_SDMMC1_CON0_DRV_DELAYNUM_SHIFT              (3U)
#define TOPCRU_SDMMC1_CON0_DRV_DELAYNUM_MASK               (0xFFU << TOPCRU_SDMMC1_CON0_DRV_DELAYNUM_SHIFT)             /* 0x000007F8 */
#define TOPCRU_SDMMC1_CON0_DRV_SEL_SHIFT                   (11U)
#define TOPCRU_SDMMC1_CON0_DRV_SEL_MASK                    (0x1U << TOPCRU_SDMMC1_CON0_DRV_SEL_SHIFT)                   /* 0x00000800 */
#define TOPCRU_SDMMC1_CON0_TEST_CLKOUT_0_SEL_SHIFT         (12U)
#define TOPCRU_SDMMC1_CON0_TEST_CLKOUT_0_SEL_MASK          (0x1U << TOPCRU_SDMMC1_CON0_TEST_CLKOUT_0_SEL_SHIFT)         /* 0x00001000 */
#define TOPCRU_SDMMC1_CON0_RAM_CLKGAT_DISABLE_SHIFT        (13U)
#define TOPCRU_SDMMC1_CON0_RAM_CLKGAT_DISABLE_MASK         (0x1U << TOPCRU_SDMMC1_CON0_RAM_CLKGAT_DISABLE_SHIFT)        /* 0x00002000 */
/* SDMMC1_CON1 */
#define TOPCRU_SDMMC1_CON1_OFFSET                          (0x630U)
#define TOPCRU_SDMMC1_CON1_SAMPLE_DEGREE_SHIFT             (1U)
#define TOPCRU_SDMMC1_CON1_SAMPLE_DEGREE_MASK              (0x3U << TOPCRU_SDMMC1_CON1_SAMPLE_DEGREE_SHIFT)             /* 0x00000006 */
#define TOPCRU_SDMMC1_CON1_SAMPLE_DELAYNUM_SHIFT           (3U)
#define TOPCRU_SDMMC1_CON1_SAMPLE_DELAYNUM_MASK            (0xFFU << TOPCRU_SDMMC1_CON1_SAMPLE_DELAYNUM_SHIFT)          /* 0x000007F8 */
#define TOPCRU_SDMMC1_CON1_SAMPLE_SEL_SHIFT                (11U)
#define TOPCRU_SDMMC1_CON1_SAMPLE_SEL_MASK                 (0x1U << TOPCRU_SDMMC1_CON1_SAMPLE_SEL_SHIFT)                /* 0x00000800 */
#define TOPCRU_SDMMC1_CON1_TEST_CLKOUT_1_SEL_SHIFT         (12U)
#define TOPCRU_SDMMC1_CON1_TEST_CLKOUT_1_SEL_MASK          (0x1U << TOPCRU_SDMMC1_CON1_TEST_CLKOUT_1_SEL_SHIFT)         /* 0x00001000 */
/* EMMC_CON */
#define TOPCRU_EMMC_CON_OFFSET                             (0x63CU)
#define TOPCRU_EMMC_CON_CLKSTABLE_SHIFT                    (0U)
#define TOPCRU_EMMC_CON_CLKSTABLE_MASK                     (0x1U << TOPCRU_EMMC_CON_CLKSTABLE_SHIFT)                    /* 0x00000001 */
#define TOPCRU_EMMC_CON_CLKBYPASS_SHIFT                    (1U)
#define TOPCRU_EMMC_CON_CLKBYPASS_MASK                     (0x1U << TOPCRU_EMMC_CON_CLKBYPASS_SHIFT)                    /* 0x00000002 */
#define TOPCRU_EMMC_CON_RAM_CLKGAT_DISABLE_SHIFT           (2U)
#define TOPCRU_EMMC_CON_RAM_CLKGAT_DISABLE_MASK            (0x1U << TOPCRU_EMMC_CON_RAM_CLKGAT_DISABLE_SHIFT)           /* 0x00000004 */
/* CM0_GATEMASK */
#define TOPCRU_CM0_GATEMASK_OFFSET                         (0x680U)
#define TOPCRU_CM0_GATEMASK_SGRF_CON_MUC_DCLK_CM0S_EN_SHIFT (0U)
#define TOPCRU_CM0_GATEMASK_SGRF_CON_MUC_DCLK_CM0S_EN_MASK (0x1U << TOPCRU_CM0_GATEMASK_SGRF_CON_MUC_DCLK_CM0S_EN_SHIFT) /* 0x00000001 */
#define TOPCRU_CM0_GATEMASK_SGRF_CON_MCU_HCLK_CM0S_EN_SHIFT (1U)
#define TOPCRU_CM0_GATEMASK_SGRF_CON_MCU_HCLK_CM0S_EN_MASK (0x1U << TOPCRU_CM0_GATEMASK_SGRF_CON_MCU_HCLK_CM0S_EN_SHIFT) /* 0x00000002 */
#define TOPCRU_CM0_GATEMASK_SGRF_CON_MCU_SCLK_CM0S_EN_SHIFT (2U)
#define TOPCRU_CM0_GATEMASK_SGRF_CON_MCU_SCLK_CM0S_EN_MASK (0x1U << TOPCRU_CM0_GATEMASK_SGRF_CON_MCU_SCLK_CM0S_EN_SHIFT) /* 0x00000004 */
/* MISC_CON0 */
#define TOPCRU_MISC_CON0_OFFSET                            (0x684U)
#define TOPCRU_MISC_CON0_ENABLE_CLUSETER_CLK_SRC_GATING_SHIFT (0U)
#define TOPCRU_MISC_CON0_ENABLE_CLUSETER_CLK_SRC_GATING_MASK (0x1U << TOPCRU_MISC_CON0_ENABLE_CLUSETER_CLK_SRC_GATING_SHIFT) /* 0x00000001 */
#define TOPCRU_MISC_CON0_ENABLE_PD_BUS_CLK_SRC_GATING_SHIFT (1U)
#define TOPCRU_MISC_CON0_ENABLE_PD_BUS_CLK_SRC_GATING_MASK (0x1U << TOPCRU_MISC_CON0_ENABLE_PD_BUS_CLK_SRC_GATING_SHIFT) /* 0x00000002 */
#define TOPCRU_MISC_CON0_ENABLE_RGA_IDLE_CLK_CORE_SHIFT    (3U)
#define TOPCRU_MISC_CON0_ENABLE_RGA_IDLE_CLK_CORE_MASK     (0x1U << TOPCRU_MISC_CON0_ENABLE_RGA_IDLE_CLK_CORE_SHIFT)    /* 0x00000008 */
/* MISC_CON1 */
#define TOPCRU_MISC_CON1_OFFSET                            (0x688U)
#define TOPCRU_MISC_CON1_WARMRSTN_EN_SHIFT                 (0U)
#define TOPCRU_MISC_CON1_WARMRSTN_EN_MASK                  (0x1U << TOPCRU_MISC_CON1_WARMRSTN_EN_SHIFT)                 /* 0x00000001 */
#define TOPCRU_MISC_CON1_CORE_SRSTN_WFIEN_SHIFT            (1U)
#define TOPCRU_MISC_CON1_CORE_SRSTN_WFIEN_MASK             (0x1U << TOPCRU_MISC_CON1_CORE_SRSTN_WFIEN_SHIFT)            /* 0x00000002 */
#define TOPCRU_MISC_CON1_CORE_WRST_WFIEN_SHIFT             (2U)
#define TOPCRU_MISC_CON1_CORE_WRST_WFIEN_MASK              (0x1U << TOPCRU_MISC_CON1_CORE_WRST_WFIEN_SHIFT)             /* 0x00000004 */
#define TOPCRU_MISC_CON1_COREPO_SRST_WFIEN_SHIFT           (3U)
#define TOPCRU_MISC_CON1_COREPO_SRST_WFIEN_MASK            (0x1U << TOPCRU_MISC_CON1_COREPO_SRST_WFIEN_SHIFT)           /* 0x00000008 */
#define TOPCRU_MISC_CON1_COREPO_WRST_WFIEN_SHIFT           (4U)
#define TOPCRU_MISC_CON1_COREPO_WRST_WFIEN_MASK            (0x1U << TOPCRU_MISC_CON1_COREPO_WRST_WFIEN_SHIFT)           /* 0x00000010 */
#define TOPCRU_MISC_CON1_CORE1_SRSTN_EN_SHIFT              (8U)
#define TOPCRU_MISC_CON1_CORE1_SRSTN_EN_MASK               (0x1U << TOPCRU_MISC_CON1_CORE1_SRSTN_EN_SHIFT)              /* 0x00000100 */
#define TOPCRU_MISC_CON1_CORE2_SRSTN_EN_SHIFT              (9U)
#define TOPCRU_MISC_CON1_CORE2_SRSTN_EN_MASK               (0x1U << TOPCRU_MISC_CON1_CORE2_SRSTN_EN_SHIFT)              /* 0x00000200 */
#define TOPCRU_MISC_CON1_CORE3_SRSTN_EN_SHIFT              (10U)
#define TOPCRU_MISC_CON1_CORE3_SRSTN_EN_MASK               (0x1U << TOPCRU_MISC_CON1_CORE3_SRSTN_EN_SHIFT)              /* 0x00000400 */
#define TOPCRU_MISC_CON1_COREPO1_SRSTN_EN_SHIFT            (11U)
#define TOPCRU_MISC_CON1_COREPO1_SRSTN_EN_MASK             (0x1U << TOPCRU_MISC_CON1_COREPO1_SRSTN_EN_SHIFT)            /* 0x00000800 */
#define TOPCRU_MISC_CON1_COREPO2_SRSTN_EN_SHIFT            (12U)
#define TOPCRU_MISC_CON1_COREPO2_SRSTN_EN_MASK             (0x1U << TOPCRU_MISC_CON1_COREPO2_SRSTN_EN_SHIFT)            /* 0x00001000 */
#define TOPCRU_MISC_CON1_COREPO3_SRSTN_EN_SHIFT            (13U)
#define TOPCRU_MISC_CON1_COREPO3_SRSTN_EN_MASK             (0x1U << TOPCRU_MISC_CON1_COREPO3_SRSTN_EN_SHIFT)            /* 0x00002000 */
/* MISC_CON2 */
#define TOPCRU_MISC_CON2_OFFSET                            (0x68CU)
#define TOPCRU_MISC_CON2_ENABLE_PD_A53_DWN_CLK_EN_SHIFT    (0U)
#define TOPCRU_MISC_CON2_ENABLE_PD_A53_DWN_CLK_EN_MASK     (0xFU << TOPCRU_MISC_CON2_ENABLE_PD_A53_DWN_CLK_EN_SHIFT)    /* 0x0000000F */
#define TOPCRU_MISC_CON2_ENABLE_PD_CORE_DWN_CLK_EN_SHIFT   (4U)
#define TOPCRU_MISC_CON2_ENABLE_PD_CORE_DWN_CLK_EN_MASK    (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_CORE_DWN_CLK_EN_SHIFT)   /* 0x00000010 */
#define TOPCRU_MISC_CON2_ENABLE_PD_GPU_DWN_CLK_EN_SHIFT    (5U)
#define TOPCRU_MISC_CON2_ENABLE_PD_GPU_DWN_CLK_EN_MASK     (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_GPU_DWN_CLK_EN_SHIFT)    /* 0x00000020 */
#define TOPCRU_MISC_CON2_ENABLE_PD_NPU_DWN_CLK_EN_SHIFT    (6U)
#define TOPCRU_MISC_CON2_ENABLE_PD_NPU_DWN_CLK_EN_MASK     (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_NPU_DWN_CLK_EN_SHIFT)    /* 0x00000040 */
#define TOPCRU_MISC_CON2_ENABLE_PD_PHP_DWN_CLK_EN_SHIFT    (7U)
#define TOPCRU_MISC_CON2_ENABLE_PD_PHP_DWN_CLK_EN_MASK     (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_PHP_DWN_CLK_EN_SHIFT)    /* 0x00000080 */
#define TOPCRU_MISC_CON2_ENABLE_PD_RGA_DWN_CLK_EN_SHIFT    (8U)
#define TOPCRU_MISC_CON2_ENABLE_PD_RGA_DWN_CLK_EN_MASK     (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_RGA_DWN_CLK_EN_SHIFT)    /* 0x00000100 */
#define TOPCRU_MISC_CON2_ENABLE_PD_VDPU_DWN_CLK_EN_SHIFT   (9U)
#define TOPCRU_MISC_CON2_ENABLE_PD_VDPU_DWN_CLK_EN_MASK    (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_VDPU_DWN_CLK_EN_SHIFT)   /* 0x00000200 */
#define TOPCRU_MISC_CON2_ENABLE_PD_VEPU_DWN_CLK_EN_SHIFT   (10U)
#define TOPCRU_MISC_CON2_ENABLE_PD_VEPU_DWN_CLK_EN_MASK    (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_VEPU_DWN_CLK_EN_SHIFT)   /* 0x00000400 */
#define TOPCRU_MISC_CON2_ENABLE_PD_VI_DWN_CLK_EN_SHIFT     (11U)
#define TOPCRU_MISC_CON2_ENABLE_PD_VI_DWN_CLK_EN_MASK      (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_VI_DWN_CLK_EN_SHIFT)     /* 0x00000800 */
#define TOPCRU_MISC_CON2_ENABLE_PD_VO_DWN_CLK_EN_SHIFT     (12U)
#define TOPCRU_MISC_CON2_ENABLE_PD_VO_DWN_CLK_EN_MASK      (0x1U << TOPCRU_MISC_CON2_ENABLE_PD_VO_DWN_CLK_EN_SHIFT)     /* 0x00001000 */
#define TOPCRU_MISC_CON2_ENABLE_REF_CLK_TESTOUT_CLK_SHIFT  (15U)
#define TOPCRU_MISC_CON2_ENABLE_REF_CLK_TESTOUT_CLK_MASK   (0x1U << TOPCRU_MISC_CON2_ENABLE_REF_CLK_TESTOUT_CLK_SHIFT)  /* 0x00008000 */
/* FREQ_CAL */
#define TOPCRU_FREQ_CAL_OFFSET                             (0x694U)
#define TOPCRU_FREQ_CAL                                    (0x0U)
#define TOPCRU_FREQ_CAL_FREQ_CAL_SHIFT                     (0U)
#define TOPCRU_FREQ_CAL_FREQ_CAL_MASK                      (0xFFFFFFFFU << TOPCRU_FREQ_CAL_FREQ_CAL_SHIFT)              /* 0xFFFFFFFF */
/****************************************PMU0CRU*****************************************/
/* CLKSEL_CON00 */
#define PMU0CRU_CLKSEL_CON00_OFFSET                        (0x100U)
#define PMU0CRU_CLKSEL_CON00_XIN_OSC0_DIV_DIV_SHIFT        (0U)
#define PMU0CRU_CLKSEL_CON00_XIN_OSC0_DIV_DIV_MASK         (0xFFFFFFFFU << PMU0CRU_CLKSEL_CON00_XIN_OSC0_DIV_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON01 */
#define PMU0CRU_CLKSEL_CON01_OFFSET                        (0x104U)
#define PMU0CRU_CLKSEL_CON01_CLK_DEEPSLOW_SEL_SHIFT        (0U)
#define PMU0CRU_CLKSEL_CON01_CLK_DEEPSLOW_SEL_MASK         (0x3U << PMU0CRU_CLKSEL_CON01_CLK_DEEPSLOW_SEL_SHIFT)        /* 0x00000003 */
#define PMU0CRU_CLKSEL_CON01_CLK_PDPMU_200M_DIV_DIV_SHIFT  (3U)
#define PMU0CRU_CLKSEL_CON01_CLK_PDPMU_200M_DIV_DIV_MASK   (0x3U << PMU0CRU_CLKSEL_CON01_CLK_PDPMU_200M_DIV_DIV_SHIFT)  /* 0x00000018 */
/* CLKSEL_CON02 */
#define PMU0CRU_CLKSEL_CON02_OFFSET                        (0x108U)
#define PMU0CRU_CLKSEL_CON02_CLK_PIPEPHY_DIV_DIV_SHIFT     (0U)
#define PMU0CRU_CLKSEL_CON02_CLK_PIPEPHY_DIV_DIV_MASK      (0x3FU << PMU0CRU_CLKSEL_CON02_CLK_PIPEPHY_DIV_DIV_SHIFT)    /* 0x0000003F */
#define PMU0CRU_CLKSEL_CON02_CLK_PIPEPHY_REF_SEL_SHIFT     (7U)
#define PMU0CRU_CLKSEL_CON02_CLK_PIPEPHY_REF_SEL_MASK      (0x1U << PMU0CRU_CLKSEL_CON02_CLK_PIPEPHY_REF_SEL_SHIFT)     /* 0x00000080 */
#define PMU0CRU_CLKSEL_CON02_CLK_USB2PHY_REF_SEL_SHIFT     (8U)
#define PMU0CRU_CLKSEL_CON02_CLK_USB2PHY_REF_SEL_MASK      (0x1U << PMU0CRU_CLKSEL_CON02_CLK_USB2PHY_REF_SEL_SHIFT)     /* 0x00000100 */
#define PMU0CRU_CLKSEL_CON02_CLK_MIPIDSIPHY_REF_SEL_SHIFT  (15U)
#define PMU0CRU_CLKSEL_CON02_CLK_MIPIDSIPHY_REF_SEL_MASK   (0x1U << PMU0CRU_CLKSEL_CON02_CLK_MIPIDSIPHY_REF_SEL_SHIFT)  /* 0x00008000 */
/* CLKSEL_CON03 */
#define PMU0CRU_CLKSEL_CON03_OFFSET                        (0x10CU)
#define PMU0CRU_CLKSEL_CON03_CLK_PMU0_I2C0_DIV_SHIFT       (8U)
#define PMU0CRU_CLKSEL_CON03_CLK_PMU0_I2C0_DIV_MASK        (0x1FU << PMU0CRU_CLKSEL_CON03_CLK_PMU0_I2C0_DIV_SHIFT)      /* 0x00001F00 */
#define PMU0CRU_CLKSEL_CON03_CLK_PMU0_I2C0_SEL_SHIFT       (14U)
#define PMU0CRU_CLKSEL_CON03_CLK_PMU0_I2C0_SEL_MASK        (0x3U << PMU0CRU_CLKSEL_CON03_CLK_PMU0_I2C0_SEL_SHIFT)       /* 0x0000C000 */
/* GATE_CON00 */
#define PMU0CRU_GATE_CON00_OFFSET                          (0x180U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_CRU_EN_SHIFT          (0U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_CRU_EN_MASK           (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_CRU_EN_SHIFT)          /* 0x00000001 */
#define PMU0CRU_GATE_CON00_PCLK_PMU0_PMU_EN_SHIFT          (1U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_PMU_EN_MASK           (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_PMU_EN_SHIFT)          /* 0x00000002 */
#define PMU0CRU_GATE_CON00_CLK_PMU0_PMU_EN_SHIFT           (2U)
#define PMU0CRU_GATE_CON00_CLK_PMU0_PMU_EN_MASK            (0x1U << PMU0CRU_GATE_CON00_CLK_PMU0_PMU_EN_SHIFT)           /* 0x00000004 */
#define PMU0CRU_GATE_CON00_PCLK_PMU0_HP_TIMER_EN_SHIFT     (3U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_HP_TIMER_EN_MASK      (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_HP_TIMER_EN_SHIFT)     /* 0x00000008 */
#define PMU0CRU_GATE_CON00_CLK_PMU0_HP_TIMER_EN_SHIFT      (4U)
#define PMU0CRU_GATE_CON00_CLK_PMU0_HP_TIMER_EN_MASK       (0x1U << PMU0CRU_GATE_CON00_CLK_PMU0_HP_TIMER_EN_SHIFT)      /* 0x00000010 */
#define PMU0CRU_GATE_CON00_CLK_PMU0_32K_HP_TIMER_EN_SHIFT  (5U)
#define PMU0CRU_GATE_CON00_CLK_PMU0_32K_HP_TIMER_EN_MASK   (0x1U << PMU0CRU_GATE_CON00_CLK_PMU0_32K_HP_TIMER_EN_SHIFT)  /* 0x00000020 */
#define PMU0CRU_GATE_CON00_PCLK_PMU0_PVTM_EN_SHIFT         (6U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_PVTM_EN_MASK          (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_PVTM_EN_SHIFT)         /* 0x00000040 */
#define PMU0CRU_GATE_CON00_CLK_PMU0_PVTM_EN_SHIFT          (7U)
#define PMU0CRU_GATE_CON00_CLK_PMU0_PVTM_EN_MASK           (0x1U << PMU0CRU_GATE_CON00_CLK_PMU0_PVTM_EN_SHIFT)          /* 0x00000080 */
#define PMU0CRU_GATE_CON00_PCLK_IOC_PMUIO_EN_SHIFT         (8U)
#define PMU0CRU_GATE_CON00_PCLK_IOC_PMUIO_EN_MASK          (0x1U << PMU0CRU_GATE_CON00_PCLK_IOC_PMUIO_EN_SHIFT)         /* 0x00000100 */
#define PMU0CRU_GATE_CON00_PCLK_PMU0_GPIO0_EN_SHIFT        (9U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_GPIO0_EN_MASK         (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_GPIO0_EN_SHIFT)        /* 0x00000200 */
#define PMU0CRU_GATE_CON00_DBCLK_PMU0_GPIO0_EN_SHIFT       (10U)
#define PMU0CRU_GATE_CON00_DBCLK_PMU0_GPIO0_EN_MASK        (0x1U << PMU0CRU_GATE_CON00_DBCLK_PMU0_GPIO0_EN_SHIFT)       /* 0x00000400 */
#define PMU0CRU_GATE_CON00_PCLK_PMU0_GRF_EN_SHIFT          (11U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_GRF_EN_MASK           (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_GRF_EN_SHIFT)          /* 0x00000800 */
#define PMU0CRU_GATE_CON00_PCLK_PMU0_SGRF_EN_SHIFT         (12U)
#define PMU0CRU_GATE_CON00_PCLK_PMU0_SGRF_EN_MASK          (0x1U << PMU0CRU_GATE_CON00_PCLK_PMU0_SGRF_EN_SHIFT)         /* 0x00001000 */
#define PMU0CRU_GATE_CON00_BUSCLK_PDPMU0_EN_SHIFT          (13U)
#define PMU0CRU_GATE_CON00_BUSCLK_PDPMU0_EN_MASK           (0x1U << PMU0CRU_GATE_CON00_BUSCLK_PDPMU0_EN_SHIFT)          /* 0x00002000 */
#define PMU0CRU_GATE_CON00_CLK_PDPMU_200M_DIV_EN_SHIFT     (14U)
#define PMU0CRU_GATE_CON00_CLK_PDPMU_200M_DIV_EN_MASK      (0x1U << PMU0CRU_GATE_CON00_CLK_PDPMU_200M_DIV_EN_SHIFT)     /* 0x00004000 */
#define PMU0CRU_GATE_CON00_XIN_OSC0_DIV_EN_SHIFT           (15U)
#define PMU0CRU_GATE_CON00_XIN_OSC0_DIV_EN_MASK            (0x1U << PMU0CRU_GATE_CON00_XIN_OSC0_DIV_EN_SHIFT)           /* 0x00008000 */
/* GATE_CON01 */
#define PMU0CRU_GATE_CON01_OFFSET                          (0x184U)
#define PMU0CRU_GATE_CON01_CLK_DDR_FAIL_SAFE_EN_SHIFT      (0U)
#define PMU0CRU_GATE_CON01_CLK_DDR_FAIL_SAFE_EN_MASK       (0x1U << PMU0CRU_GATE_CON01_CLK_DDR_FAIL_SAFE_EN_SHIFT)      /* 0x00000001 */
#define PMU0CRU_GATE_CON01_PCLK_PMU0_SCRKEYGEN_EN_SHIFT    (1U)
#define PMU0CRU_GATE_CON01_PCLK_PMU0_SCRKEYGEN_EN_MASK     (0x1U << PMU0CRU_GATE_CON01_PCLK_PMU0_SCRKEYGEN_EN_SHIFT)    /* 0x00000002 */
/* GATE_CON02 */
#define PMU0CRU_GATE_CON02_OFFSET                          (0x188U)
#define PMU0CRU_GATE_CON02_CLK_PIPEPHY_DIV_EN_SHIFT        (0U)
#define PMU0CRU_GATE_CON02_CLK_PIPEPHY_DIV_EN_MASK         (0x1U << PMU0CRU_GATE_CON02_CLK_PIPEPHY_DIV_EN_SHIFT)        /* 0x00000001 */
#define PMU0CRU_GATE_CON02_XIN_OSC0_PIPEPHY_G_EN_SHIFT     (1U)
#define PMU0CRU_GATE_CON02_XIN_OSC0_PIPEPHY_G_EN_MASK      (0x1U << PMU0CRU_GATE_CON02_XIN_OSC0_PIPEPHY_G_EN_SHIFT)     /* 0x00000002 */
#define PMU0CRU_GATE_CON02_CLK_PIPEPHY_REF_EN_SHIFT        (2U)
#define PMU0CRU_GATE_CON02_CLK_PIPEPHY_REF_EN_MASK         (0x1U << PMU0CRU_GATE_CON02_CLK_PIPEPHY_REF_EN_SHIFT)        /* 0x00000004 */
#define PMU0CRU_GATE_CON02_XIN_OSC0_USB2PHY_G_EN_SHIFT     (4U)
#define PMU0CRU_GATE_CON02_XIN_OSC0_USB2PHY_G_EN_MASK      (0x1U << PMU0CRU_GATE_CON02_XIN_OSC0_USB2PHY_G_EN_SHIFT)     /* 0x00000010 */
#define PMU0CRU_GATE_CON02_CLK_USB2PHY_REF_EN_SHIFT        (5U)
#define PMU0CRU_GATE_CON02_CLK_USB2PHY_REF_EN_MASK         (0x1U << PMU0CRU_GATE_CON02_CLK_USB2PHY_REF_EN_SHIFT)        /* 0x00000020 */
#define PMU0CRU_GATE_CON02_XIN_OSC0_MIPIDSIPHY_G_EN_SHIFT  (6U)
#define PMU0CRU_GATE_CON02_XIN_OSC0_MIPIDSIPHY_G_EN_MASK   (0x1U << PMU0CRU_GATE_CON02_XIN_OSC0_MIPIDSIPHY_G_EN_SHIFT)  /* 0x00000040 */
#define PMU0CRU_GATE_CON02_CLK_MIPIDSIPHY_REF_EN_SHIFT     (7U)
#define PMU0CRU_GATE_CON02_CLK_MIPIDSIPHY_REF_EN_MASK      (0x1U << PMU0CRU_GATE_CON02_CLK_MIPIDSIPHY_REF_EN_SHIFT)     /* 0x00000080 */
#define PMU0CRU_GATE_CON02_PCLK_PMU0_I2C0_EN_SHIFT         (8U)
#define PMU0CRU_GATE_CON02_PCLK_PMU0_I2C0_EN_MASK          (0x1U << PMU0CRU_GATE_CON02_PCLK_PMU0_I2C0_EN_SHIFT)         /* 0x00000100 */
#define PMU0CRU_GATE_CON02_CLK_PMU0_I2C0_EN_SHIFT          (9U)
#define PMU0CRU_GATE_CON02_CLK_PMU0_I2C0_EN_MASK           (0x1U << PMU0CRU_GATE_CON02_CLK_PMU0_I2C0_EN_SHIFT)          /* 0x00000200 */
/* SOFTRST_CON00 */
#define PMU0CRU_SOFTRST_CON00_OFFSET                       (0x200U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_CRU_SHIFT       (0U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_CRU_MASK        (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_CRU_SHIFT)       /* 0x00000001 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_PMU_SHIFT       (1U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_PMU_MASK        (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_PMU_SHIFT)       /* 0x00000002 */
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_PMU_SHIFT        (2U)
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_PMU_MASK         (0x1U << PMU0CRU_SOFTRST_CON00_RESETN_PMU0_PMU_SHIFT)        /* 0x00000004 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_HP_TIMER_SHIFT  (3U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_HP_TIMER_MASK   (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_HP_TIMER_SHIFT)  /* 0x00000008 */
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_HP_TIMER_SHIFT   (4U)
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_HP_TIMER_MASK    (0x1U << PMU0CRU_SOFTRST_CON00_RESETN_PMU0_HP_TIMER_SHIFT)   /* 0x00000010 */
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_32K_HP_TIMER_SHIFT (5U)
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_32K_HP_TIMER_MASK (0x1U << PMU0CRU_SOFTRST_CON00_RESETN_PMU0_32K_HP_TIMER_SHIFT) /* 0x00000020 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_PVTM_SHIFT      (6U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_PVTM_MASK       (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_PVTM_SHIFT)      /* 0x00000040 */
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_PVTM_SHIFT       (7U)
#define PMU0CRU_SOFTRST_CON00_RESETN_PMU0_PVTM_MASK        (0x1U << PMU0CRU_SOFTRST_CON00_RESETN_PMU0_PVTM_SHIFT)       /* 0x00000080 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_IOC_PMUIO_SHIFT      (8U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_IOC_PMUIO_MASK       (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_IOC_PMUIO_SHIFT)      /* 0x00000100 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_GPIO0_SHIFT     (9U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_GPIO0_MASK      (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_GPIO0_SHIFT)     /* 0x00000200 */
#define PMU0CRU_SOFTRST_CON00_DBRESETN_PMU0_GPIO0_SHIFT    (10U)
#define PMU0CRU_SOFTRST_CON00_DBRESETN_PMU0_GPIO0_MASK     (0x1U << PMU0CRU_SOFTRST_CON00_DBRESETN_PMU0_GPIO0_SHIFT)    /* 0x00000400 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_GRF_SHIFT       (11U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_GRF_MASK        (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_GRF_SHIFT)       /* 0x00000800 */
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_SGRF_SHIFT      (12U)
#define PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_SGRF_MASK       (0x1U << PMU0CRU_SOFTRST_CON00_PRESETN_PMU0_SGRF_SHIFT)      /* 0x00001000 */
/* SOFTRST_CON01 */
#define PMU0CRU_SOFTRST_CON01_OFFSET                       (0x204U)
#define PMU0CRU_SOFTRST_CON01_RESETN_DDR_FAIL_SAFE_SHIFT   (0U)
#define PMU0CRU_SOFTRST_CON01_RESETN_DDR_FAIL_SAFE_MASK    (0x1U << PMU0CRU_SOFTRST_CON01_RESETN_DDR_FAIL_SAFE_SHIFT)   /* 0x00000001 */
#define PMU0CRU_SOFTRST_CON01_PRESETN_PMU0_SCRKEYGEN_SHIFT (1U)
#define PMU0CRU_SOFTRST_CON01_PRESETN_PMU0_SCRKEYGEN_MASK  (0x1U << PMU0CRU_SOFTRST_CON01_PRESETN_PMU0_SCRKEYGEN_SHIFT) /* 0x00000002 */
/* SOFTRST_CON02 */
#define PMU0CRU_SOFTRST_CON02_OFFSET                       (0x208U)
#define PMU0CRU_SOFTRST_CON02_PRESETN_PMU0_I2C0_SHIFT      (8U)
#define PMU0CRU_SOFTRST_CON02_PRESETN_PMU0_I2C0_MASK       (0x1U << PMU0CRU_SOFTRST_CON02_PRESETN_PMU0_I2C0_SHIFT)      /* 0x00000100 */
#define PMU0CRU_SOFTRST_CON02_RESETN_PMU0_I2C0_SHIFT       (9U)
#define PMU0CRU_SOFTRST_CON02_RESETN_PMU0_I2C0_MASK        (0x1U << PMU0CRU_SOFTRST_CON02_RESETN_PMU0_I2C0_SHIFT)       /* 0x00000200 */
/* MISC_CON0 */
#define PMU0CRU_MISC_CON0_OFFSET                           (0x290U)
#define PMU0CRU_MISC_CON0_GRF_32K_DEVIATION_SHIFT          (0U)
#define PMU0CRU_MISC_CON0_GRF_32K_DEVIATION_MASK           (0xFFU << PMU0CRU_MISC_CON0_GRF_32K_DEVIATION_SHIFT)         /* 0x000000FF */
#define PMU0CRU_MISC_CON0_GRF_XIN_OSC0_GATE_SHIFT          (8U)
#define PMU0CRU_MISC_CON0_GRF_XIN_OSC0_GATE_MASK           (0x1U << PMU0CRU_MISC_CON0_GRF_XIN_OSC0_GATE_SHIFT)          /* 0x00000100 */
#define PMU0CRU_MISC_CON0_GRF_RST_THE_32K_DETECT_SHIFT     (9U)
#define PMU0CRU_MISC_CON0_GRF_RST_THE_32K_DETECT_MASK      (0x1U << PMU0CRU_MISC_CON0_GRF_RST_THE_32K_DETECT_SHIFT)     /* 0x00000200 */
/* GRF_FREQ */
#define PMU0CRU_GRF_FREQ_OFFSET                            (0x294U)
#define PMU0CRU_GRF_FREQ                                   (0x0U)
#define PMU0CRU_GRF_FREQ_GRF_FREQ_VALUE_SHIFT              (0U)
#define PMU0CRU_GRF_FREQ_GRF_FREQ_VALUE_MASK               (0xFFFFFFFFU << PMU0CRU_GRF_FREQ_GRF_FREQ_VALUE_SHIFT)       /* 0xFFFFFFFF */
/* IO32K_EXIST */
#define PMU0CRU_IO32K_EXIST_OFFSET                         (0x298U)
#define PMU0CRU_IO32K_EXIST                                (0x0U)
#define PMU0CRU_IO32K_EXIST_GRF_EXIST_SHIFT                (0U)
#define PMU0CRU_IO32K_EXIST_GRF_EXIST_MASK                 (0x1U << PMU0CRU_IO32K_EXIST_GRF_EXIST_SHIFT)                /* 0x00000001 */
/****************************************PMU1CRU*****************************************/
/* CPLL_CON0 */
#define PMU1CRU_CPLL_CON0_OFFSET                           (0x40U)
#define PMU1CRU_CPLL_CON0_FBDIV_SHIFT                      (0U)
#define PMU1CRU_CPLL_CON0_FBDIV_MASK                       (0xFFFU << PMU1CRU_CPLL_CON0_FBDIV_SHIFT)                    /* 0x00000FFF */
#define PMU1CRU_CPLL_CON0_POSTDIV1_SHIFT                   (12U)
#define PMU1CRU_CPLL_CON0_POSTDIV1_MASK                    (0x7U << PMU1CRU_CPLL_CON0_POSTDIV1_SHIFT)                   /* 0x00007000 */
#define PMU1CRU_CPLL_CON0_BYPASS_SHIFT                     (15U)
#define PMU1CRU_CPLL_CON0_BYPASS_MASK                      (0x1U << PMU1CRU_CPLL_CON0_BYPASS_SHIFT)                     /* 0x00008000 */
/* CPLL_CON1 */
#define PMU1CRU_CPLL_CON1_OFFSET                           (0x44U)
#define PMU1CRU_CPLL_CON1_REFDIV_SHIFT                     (0U)
#define PMU1CRU_CPLL_CON1_REFDIV_MASK                      (0x3FU << PMU1CRU_CPLL_CON1_REFDIV_SHIFT)                    /* 0x0000003F */
#define PMU1CRU_CPLL_CON1_POSTDIV2_SHIFT                   (6U)
#define PMU1CRU_CPLL_CON1_POSTDIV2_MASK                    (0x7U << PMU1CRU_CPLL_CON1_POSTDIV2_SHIFT)                   /* 0x000001C0 */
#define PMU1CRU_CPLL_CON1_PLL_LOCK_SHIFT                   (10U)
#define PMU1CRU_CPLL_CON1_PLL_LOCK_MASK                    (0x1U << PMU1CRU_CPLL_CON1_PLL_LOCK_SHIFT)                   /* 0x00000400 */
#define PMU1CRU_CPLL_CON1_DSMPD_SHIFT                      (12U)
#define PMU1CRU_CPLL_CON1_DSMPD_MASK                       (0x1U << PMU1CRU_CPLL_CON1_DSMPD_SHIFT)                      /* 0x00001000 */
#define PMU1CRU_CPLL_CON1_PLLPD0_SHIFT                     (13U)
#define PMU1CRU_CPLL_CON1_PLLPD0_MASK                      (0x1U << PMU1CRU_CPLL_CON1_PLLPD0_SHIFT)                     /* 0x00002000 */
#define PMU1CRU_CPLL_CON1_PLLPD1_SHIFT                     (14U)
#define PMU1CRU_CPLL_CON1_PLLPD1_MASK                      (0x1U << PMU1CRU_CPLL_CON1_PLLPD1_SHIFT)                     /* 0x00004000 */
#define PMU1CRU_CPLL_CON1_PLLPDSEL_SHIFT                   (15U)
#define PMU1CRU_CPLL_CON1_PLLPDSEL_MASK                    (0x1U << PMU1CRU_CPLL_CON1_PLLPDSEL_SHIFT)                   /* 0x00008000 */
/* CPLL_CON2 */
#define PMU1CRU_CPLL_CON2_OFFSET                           (0x48U)
#define PMU1CRU_CPLL_CON2_FRACDIV_SHIFT                    (0U)
#define PMU1CRU_CPLL_CON2_FRACDIV_MASK                     (0xFFFFFFU << PMU1CRU_CPLL_CON2_FRACDIV_SHIFT)               /* 0x00FFFFFF */
#define PMU1CRU_CPLL_CON2_DACPD_SHIFT                      (24U)
#define PMU1CRU_CPLL_CON2_DACPD_MASK                       (0x1U << PMU1CRU_CPLL_CON2_DACPD_SHIFT)                      /* 0x01000000 */
#define PMU1CRU_CPLL_CON2_FOUTPOSTDIVPD_SHIFT              (25U)
#define PMU1CRU_CPLL_CON2_FOUTPOSTDIVPD_MASK               (0x1U << PMU1CRU_CPLL_CON2_FOUTPOSTDIVPD_SHIFT)              /* 0x02000000 */
#define PMU1CRU_CPLL_CON2_FOUTVCOPD_SHIFT                  (26U)
#define PMU1CRU_CPLL_CON2_FOUTVCOPD_MASK                   (0x1U << PMU1CRU_CPLL_CON2_FOUTVCOPD_SHIFT)                  /* 0x04000000 */
#define PMU1CRU_CPLL_CON2_FOUT4PHASEPD_SHIFT               (27U)
#define PMU1CRU_CPLL_CON2_FOUT4PHASEPD_MASK                (0x1U << PMU1CRU_CPLL_CON2_FOUT4PHASEPD_SHIFT)               /* 0x08000000 */
/* CPLL_CON3 */
#define PMU1CRU_CPLL_CON3_OFFSET                           (0x4CU)
#define PMU1CRU_CPLL_CON3_SSMOD_BP_SHIFT                   (0U)
#define PMU1CRU_CPLL_CON3_SSMOD_BP_MASK                    (0x1U << PMU1CRU_CPLL_CON3_SSMOD_BP_SHIFT)                   /* 0x00000001 */
#define PMU1CRU_CPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT         (1U)
#define PMU1CRU_CPLL_CON3_SSMOD_DISABLE_SSCG_MASK          (0x1U << PMU1CRU_CPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)         /* 0x00000002 */
#define PMU1CRU_CPLL_CON3_SSMOD_RESET_SHIFT                (2U)
#define PMU1CRU_CPLL_CON3_SSMOD_RESET_MASK                 (0x1U << PMU1CRU_CPLL_CON3_SSMOD_RESET_SHIFT)                /* 0x00000004 */
#define PMU1CRU_CPLL_CON3_SSMOD_DOWNSPREAD_SHIFT           (3U)
#define PMU1CRU_CPLL_CON3_SSMOD_DOWNSPREAD_MASK            (0x1U << PMU1CRU_CPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)           /* 0x00000008 */
#define PMU1CRU_CPLL_CON3_SSMOD_DIVVAL_SHIFT               (4U)
#define PMU1CRU_CPLL_CON3_SSMOD_DIVVAL_MASK                (0xFU << PMU1CRU_CPLL_CON3_SSMOD_DIVVAL_SHIFT)               /* 0x000000F0 */
#define PMU1CRU_CPLL_CON3_SSMOD_SPREAD_SHIFT               (8U)
#define PMU1CRU_CPLL_CON3_SSMOD_SPREAD_MASK                (0x1FU << PMU1CRU_CPLL_CON3_SSMOD_SPREAD_SHIFT)              /* 0x00001F00 */
/* CPLL_CON4 */
#define PMU1CRU_CPLL_CON4_OFFSET                           (0x50U)
#define PMU1CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT         (0U)
#define PMU1CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK          (0x1U << PMU1CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)         /* 0x00000001 */
#define PMU1CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT          (8U)
#define PMU1CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_MASK           (0xFFU << PMU1CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)         /* 0x0000FF00 */
/* CLKSEL_CON00 */
#define PMU1CRU_CLKSEL_CON00_OFFSET                        (0x100U)
#define PMU1CRU_CLKSEL_CON00_CLK_200M_PMU_DIV_SHIFT        (0U)
#define PMU1CRU_CLKSEL_CON00_CLK_200M_PMU_DIV_MASK         (0x1FU << PMU1CRU_CLKSEL_CON00_CLK_200M_PMU_DIV_SHIFT)       /* 0x0000001F */
/* CLKSEL_CON02 */
#define PMU1CRU_CLKSEL_CON02_OFFSET                        (0x108U)
#define PMU1CRU_CLKSEL_CON02_CLK_PMU1_UART0_SRC_DIV_SHIFT  (0U)
#define PMU1CRU_CLKSEL_CON02_CLK_PMU1_UART0_SRC_DIV_MASK   (0xFU << PMU1CRU_CLKSEL_CON02_CLK_PMU1_UART0_SRC_DIV_SHIFT)  /* 0x0000000F */
#define PMU1CRU_CLKSEL_CON02_SCLK_PMU1_UART0_SEL_SHIFT     (6U)
#define PMU1CRU_CLKSEL_CON02_SCLK_PMU1_UART0_SEL_MASK      (0x3U << PMU1CRU_CLKSEL_CON02_SCLK_PMU1_UART0_SEL_SHIFT)     /* 0x000000C0 */
/* CLKSEL_CON03 */
#define PMU1CRU_CLKSEL_CON03_OFFSET                        (0x10CU)
#define PMU1CRU_CLKSEL_CON03_CLK_PMU1_UART0_FRAC_DIV_SHIFT (0U)
#define PMU1CRU_CLKSEL_CON03_CLK_PMU1_UART0_FRAC_DIV_MASK  (0xFFFFFFFFU << PMU1CRU_CLKSEL_CON03_CLK_PMU1_UART0_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON04 */
#define PMU1CRU_CLKSEL_CON04_OFFSET                        (0x110U)
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_SPI0_DIV_SHIFT       (0U)
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_SPI0_DIV_MASK        (0x3U << PMU1CRU_CLKSEL_CON04_CLK_PMU1_SPI0_DIV_SHIFT)       /* 0x00000003 */
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_SPI0_SEL_SHIFT       (6U)
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_SPI0_SEL_MASK        (0x3U << PMU1CRU_CLKSEL_CON04_CLK_PMU1_SPI0_SEL_SHIFT)       /* 0x000000C0 */
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_PWM0_DIV_SHIFT       (8U)
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_PWM0_DIV_MASK        (0x3U << PMU1CRU_CLKSEL_CON04_CLK_PMU1_PWM0_DIV_SHIFT)       /* 0x00000300 */
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_PWM0_SEL_SHIFT       (14U)
#define PMU1CRU_CLKSEL_CON04_CLK_PMU1_PWM0_SEL_MASK        (0x3U << PMU1CRU_CLKSEL_CON04_CLK_PMU1_PWM0_SEL_SHIFT)       /* 0x0000C000 */
/* CLKSEL_CON06 */
#define PMU1CRU_CLKSEL_CON06_OFFSET                        (0x118U)
#define PMU1CRU_CLKSEL_CON06_CLK_TESTOUT_PMU1_DIV_SHIFT    (8U)
#define PMU1CRU_CLKSEL_CON06_CLK_TESTOUT_PMU1_DIV_MASK     (0x7U << PMU1CRU_CLKSEL_CON06_CLK_TESTOUT_PMU1_DIV_SHIFT)    /* 0x00000700 */
#define PMU1CRU_CLKSEL_CON06_CLK_TESTOUT_PMU1_SEL_SHIFT    (13U)
#define PMU1CRU_CLKSEL_CON06_CLK_TESTOUT_PMU1_SEL_MASK     (0x7U << PMU1CRU_CLKSEL_CON06_CLK_TESTOUT_PMU1_SEL_SHIFT)    /* 0x0000E000 */
/* GATE_CON00 */
#define PMU1CRU_GATE_CON00_OFFSET                          (0x180U)
#define PMU1CRU_GATE_CON00_PCLK_PMU1_CRU_EN_SHIFT          (0U)
#define PMU1CRU_GATE_CON00_PCLK_PMU1_CRU_EN_MASK           (0x1U << PMU1CRU_GATE_CON00_PCLK_PMU1_CRU_EN_SHIFT)          /* 0x00000001 */
#define PMU1CRU_GATE_CON00_CLK_200M_PMU_EN_SHIFT           (1U)
#define PMU1CRU_GATE_CON00_CLK_200M_PMU_EN_MASK            (0x1U << PMU1CRU_GATE_CON00_CLK_200M_PMU_EN_SHIFT)           /* 0x00000002 */
#define PMU1CRU_GATE_CON00_HCLK_PMU1_MEM_EN_SHIFT          (2U)
#define PMU1CRU_GATE_CON00_HCLK_PMU1_MEM_EN_MASK           (0x1U << PMU1CRU_GATE_CON00_HCLK_PMU1_MEM_EN_SHIFT)          /* 0x00000004 */
#define PMU1CRU_GATE_CON00_HCLK_PMU1_BIU_EN_SHIFT          (3U)
#define PMU1CRU_GATE_CON00_HCLK_PMU1_BIU_EN_MASK           (0x1U << PMU1CRU_GATE_CON00_HCLK_PMU1_BIU_EN_SHIFT)          /* 0x00000008 */
#define PMU1CRU_GATE_CON00_PCLK_PMU1_BIU_EN_SHIFT          (4U)
#define PMU1CRU_GATE_CON00_PCLK_PMU1_BIU_EN_MASK           (0x1U << PMU1CRU_GATE_CON00_PCLK_PMU1_BIU_EN_SHIFT)          /* 0x00000010 */
#define PMU1CRU_GATE_CON00_PCLK_PMU1_UART0_EN_SHIFT        (7U)
#define PMU1CRU_GATE_CON00_PCLK_PMU1_UART0_EN_MASK         (0x1U << PMU1CRU_GATE_CON00_PCLK_PMU1_UART0_EN_SHIFT)        /* 0x00000080 */
#define PMU1CRU_GATE_CON00_CLK_PMU1_UART0_EN_SHIFT         (8U)
#define PMU1CRU_GATE_CON00_CLK_PMU1_UART0_EN_MASK          (0x1U << PMU1CRU_GATE_CON00_CLK_PMU1_UART0_EN_SHIFT)         /* 0x00000100 */
#define PMU1CRU_GATE_CON00_CLK_PMU1_UART0_FRAC_EN_SHIFT    (9U)
#define PMU1CRU_GATE_CON00_CLK_PMU1_UART0_FRAC_EN_MASK     (0x1U << PMU1CRU_GATE_CON00_CLK_PMU1_UART0_FRAC_EN_SHIFT)    /* 0x00000200 */
#define PMU1CRU_GATE_CON00_SCLK_PMU1_UART0_EN_SHIFT        (10U)
#define PMU1CRU_GATE_CON00_SCLK_PMU1_UART0_EN_MASK         (0x1U << PMU1CRU_GATE_CON00_SCLK_PMU1_UART0_EN_SHIFT)        /* 0x00000400 */
#define PMU1CRU_GATE_CON00_PCLK_PDPMU1_TO_PERI_EN_SHIFT    (15U)
#define PMU1CRU_GATE_CON00_PCLK_PDPMU1_TO_PERI_EN_MASK     (0x1U << PMU1CRU_GATE_CON00_PCLK_PDPMU1_TO_PERI_EN_SHIFT)    /* 0x00008000 */
/* GATE_CON01 */
#define PMU1CRU_GATE_CON01_OFFSET                          (0x184U)
#define PMU1CRU_GATE_CON01_PCLK_PMU1_SPI0_EN_SHIFT         (0U)
#define PMU1CRU_GATE_CON01_PCLK_PMU1_SPI0_EN_MASK          (0x1U << PMU1CRU_GATE_CON01_PCLK_PMU1_SPI0_EN_SHIFT)         /* 0x00000001 */
#define PMU1CRU_GATE_CON01_CLK_PMU1_SPI0_EN_SHIFT          (1U)
#define PMU1CRU_GATE_CON01_CLK_PMU1_SPI0_EN_MASK           (0x1U << PMU1CRU_GATE_CON01_CLK_PMU1_SPI0_EN_SHIFT)          /* 0x00000002 */
#define PMU1CRU_GATE_CON01_SCLK_IN_PMU1_SPI0_EN_SHIFT      (2U)
#define PMU1CRU_GATE_CON01_SCLK_IN_PMU1_SPI0_EN_MASK       (0x1U << PMU1CRU_GATE_CON01_SCLK_IN_PMU1_SPI0_EN_SHIFT)      /* 0x00000004 */
#define PMU1CRU_GATE_CON01_PCLK_PMU1_PWM0_EN_SHIFT         (3U)
#define PMU1CRU_GATE_CON01_PCLK_PMU1_PWM0_EN_MASK          (0x1U << PMU1CRU_GATE_CON01_PCLK_PMU1_PWM0_EN_SHIFT)         /* 0x00000008 */
#define PMU1CRU_GATE_CON01_CLK_PMU1_PWM0_EN_SHIFT          (4U)
#define PMU1CRU_GATE_CON01_CLK_PMU1_PWM0_EN_MASK           (0x1U << PMU1CRU_GATE_CON01_CLK_PMU1_PWM0_EN_SHIFT)          /* 0x00000010 */
#define PMU1CRU_GATE_CON01_CLK_CAPTURE_PMU1_PWM0_EN_SHIFT  (5U)
#define PMU1CRU_GATE_CON01_CLK_CAPTURE_PMU1_PWM0_EN_MASK   (0x1U << PMU1CRU_GATE_CON01_CLK_CAPTURE_PMU1_PWM0_EN_SHIFT)  /* 0x00000020 */
#define PMU1CRU_GATE_CON01_CLK_PMU1_WFI_EN_SHIFT           (6U)
#define PMU1CRU_GATE_CON01_CLK_PMU1_WFI_EN_MASK            (0x1U << PMU1CRU_GATE_CON01_CLK_PMU1_WFI_EN_SHIFT)           /* 0x00000040 */
/* GATE_CON02 */
#define PMU1CRU_GATE_CON02_OFFSET                          (0x188U)
#define PMU1CRU_GATE_CON02_FCLK_PMU1_CM0_CORE_EN_SHIFT     (0U)
#define PMU1CRU_GATE_CON02_FCLK_PMU1_CM0_CORE_EN_MASK      (0x1U << PMU1CRU_GATE_CON02_FCLK_PMU1_CM0_CORE_EN_SHIFT)     /* 0x00000001 */
#define PMU1CRU_GATE_CON02_CLK_PMU1_CM0_RTC_EN_SHIFT       (1U)
#define PMU1CRU_GATE_CON02_CLK_PMU1_CM0_RTC_EN_MASK        (0x1U << PMU1CRU_GATE_CON02_CLK_PMU1_CM0_RTC_EN_SHIFT)       /* 0x00000002 */
#define PMU1CRU_GATE_CON02_PCLK_PMU1_WDTNS_EN_SHIFT        (3U)
#define PMU1CRU_GATE_CON02_PCLK_PMU1_WDTNS_EN_MASK         (0x1U << PMU1CRU_GATE_CON02_PCLK_PMU1_WDTNS_EN_SHIFT)        /* 0x00000008 */
#define PMU1CRU_GATE_CON02_CLK_PMU1_WDTNS_EN_SHIFT         (4U)
#define PMU1CRU_GATE_CON02_CLK_PMU1_WDTNS_EN_MASK          (0x1U << PMU1CRU_GATE_CON02_CLK_PMU1_WDTNS_EN_SHIFT)         /* 0x00000010 */
/* GATE_CON03 */
#define PMU1CRU_GATE_CON03_OFFSET                          (0x18CU)
#define PMU1CRU_GATE_CON03_PCLK_PMU1_MAILBOX_EN_SHIFT      (8U)
#define PMU1CRU_GATE_CON03_PCLK_PMU1_MAILBOX_EN_MASK       (0x1U << PMU1CRU_GATE_CON03_PCLK_PMU1_MAILBOX_EN_SHIFT)      /* 0x00000100 */
#define PMU1CRU_GATE_CON03_CLK_TESTOUT_PMU1_EN_SHIFT       (15U)
#define PMU1CRU_GATE_CON03_CLK_TESTOUT_PMU1_EN_MASK        (0x1U << PMU1CRU_GATE_CON03_CLK_TESTOUT_PMU1_EN_SHIFT)       /* 0x00008000 */
/* SOFTRST_CON00 */
#define PMU1CRU_SOFTRST_CON00_OFFSET                       (0x200U)
#define PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_CRU_SHIFT       (0U)
#define PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_CRU_MASK        (0x1U << PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_CRU_SHIFT)       /* 0x00000001 */
#define PMU1CRU_SOFTRST_CON00_HRESETN_PMU1_MEM_SHIFT       (2U)
#define PMU1CRU_SOFTRST_CON00_HRESETN_PMU1_MEM_MASK        (0x1U << PMU1CRU_SOFTRST_CON00_HRESETN_PMU1_MEM_SHIFT)       /* 0x00000004 */
#define PMU1CRU_SOFTRST_CON00_HRESETN_PMU1_BIU_SHIFT       (3U)
#define PMU1CRU_SOFTRST_CON00_HRESETN_PMU1_BIU_MASK        (0x1U << PMU1CRU_SOFTRST_CON00_HRESETN_PMU1_BIU_SHIFT)       /* 0x00000008 */
#define PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_BIU_SHIFT       (4U)
#define PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_BIU_MASK        (0x1U << PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_BIU_SHIFT)       /* 0x00000010 */
#define PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_UART0_SHIFT     (7U)
#define PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_UART0_MASK      (0x1U << PMU1CRU_SOFTRST_CON00_PRESETN_PMU1_UART0_SHIFT)     /* 0x00000080 */
#define PMU1CRU_SOFTRST_CON00_SRESETN_PMU1_UART0_SHIFT     (10U)
#define PMU1CRU_SOFTRST_CON00_SRESETN_PMU1_UART0_MASK      (0x1U << PMU1CRU_SOFTRST_CON00_SRESETN_PMU1_UART0_SHIFT)     /* 0x00000400 */
/* SOFTRST_CON01 */
#define PMU1CRU_SOFTRST_CON01_OFFSET                       (0x204U)
#define PMU1CRU_SOFTRST_CON01_PRESETN_PMU1_SPI0_SHIFT      (0U)
#define PMU1CRU_SOFTRST_CON01_PRESETN_PMU1_SPI0_MASK       (0x1U << PMU1CRU_SOFTRST_CON01_PRESETN_PMU1_SPI0_SHIFT)      /* 0x00000001 */
#define PMU1CRU_SOFTRST_CON01_RESETN_PMU1_SPI0_SHIFT       (1U)
#define PMU1CRU_SOFTRST_CON01_RESETN_PMU1_SPI0_MASK        (0x1U << PMU1CRU_SOFTRST_CON01_RESETN_PMU1_SPI0_SHIFT)       /* 0x00000002 */
#define PMU1CRU_SOFTRST_CON01_PRESETN_PMU1_PWM0_SHIFT      (3U)
#define PMU1CRU_SOFTRST_CON01_PRESETN_PMU1_PWM0_MASK       (0x1U << PMU1CRU_SOFTRST_CON01_PRESETN_PMU1_PWM0_SHIFT)      /* 0x00000008 */
#define PMU1CRU_SOFTRST_CON01_RESETN_PMU1_PWM0_SHIFT       (4U)
#define PMU1CRU_SOFTRST_CON01_RESETN_PMU1_PWM0_MASK        (0x1U << PMU1CRU_SOFTRST_CON01_RESETN_PMU1_PWM0_SHIFT)       /* 0x00000010 */
/* SOFTRST_CON02 */
#define PMU1CRU_SOFTRST_CON02_OFFSET                       (0x208U)
#define PMU1CRU_SOFTRST_CON02_FRESETN_PMU1_CM0_CORE_SHIFT  (0U)
#define PMU1CRU_SOFTRST_CON02_FRESETN_PMU1_CM0_CORE_MASK   (0x1U << PMU1CRU_SOFTRST_CON02_FRESETN_PMU1_CM0_CORE_SHIFT)  /* 0x00000001 */
#define PMU1CRU_SOFTRST_CON02_TRESETN_PMU1_CM0_JTAG_SHIFT  (2U)
#define PMU1CRU_SOFTRST_CON02_TRESETN_PMU1_CM0_JTAG_MASK   (0x1U << PMU1CRU_SOFTRST_CON02_TRESETN_PMU1_CM0_JTAG_SHIFT)  /* 0x00000004 */
#define PMU1CRU_SOFTRST_CON02_PRESETN_PMU1_WDTNS_SHIFT     (3U)
#define PMU1CRU_SOFTRST_CON02_PRESETN_PMU1_WDTNS_MASK      (0x1U << PMU1CRU_SOFTRST_CON02_PRESETN_PMU1_WDTNS_SHIFT)     /* 0x00000008 */
#define PMU1CRU_SOFTRST_CON02_RESETN_PMU1_WDTNS_SHIFT      (4U)
#define PMU1CRU_SOFTRST_CON02_RESETN_PMU1_WDTNS_MASK       (0x1U << PMU1CRU_SOFTRST_CON02_RESETN_PMU1_WDTNS_SHIFT)      /* 0x00000010 */
#define PMU1CRU_SOFTRST_CON02_PRESETN_PMU1_MAILBOX_SHIFT   (8U)
#define PMU1CRU_SOFTRST_CON02_PRESETN_PMU1_MAILBOX_MASK    (0x1U << PMU1CRU_SOFTRST_CON02_PRESETN_PMU1_MAILBOX_SHIFT)   /* 0x00000100 */
/* SSGTBL0_3 */
#define PMU1CRU_SSGTBL0_3_OFFSET                           (0x280U)
#define PMU1CRU_SSGTBL0_3_SSGTBL0_3_SHIFT                  (0U)
#define PMU1CRU_SSGTBL0_3_SSGTBL0_3_MASK                   (0xFFFFFFFFU << PMU1CRU_SSGTBL0_3_SSGTBL0_3_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL4_7 */
#define PMU1CRU_SSGTBL4_7_OFFSET                           (0x284U)
#define PMU1CRU_SSGTBL4_7_SSGTBL4_7_SHIFT                  (0U)
#define PMU1CRU_SSGTBL4_7_SSGTBL4_7_MASK                   (0xFFFFFFFFU << PMU1CRU_SSGTBL4_7_SSGTBL4_7_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL8_11 */
#define PMU1CRU_SSGTBL8_11_OFFSET                          (0x288U)
#define PMU1CRU_SSGTBL8_11_SSGTBL8_11_SHIFT                (0U)
#define PMU1CRU_SSGTBL8_11_SSGTBL8_11_MASK                 (0xFFFFFFFFU << PMU1CRU_SSGTBL8_11_SSGTBL8_11_SHIFT)         /* 0xFFFFFFFF */
/* SSGTBL12_15 */
#define PMU1CRU_SSGTBL12_15_OFFSET                         (0x28CU)
#define PMU1CRU_SSGTBL12_15_SSGTBL12_15_SHIFT              (0U)
#define PMU1CRU_SSGTBL12_15_SSGTBL12_15_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL12_15_SSGTBL12_15_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL16_19 */
#define PMU1CRU_SSGTBL16_19_OFFSET                         (0x290U)
#define PMU1CRU_SSGTBL16_19_SSGTBL16_19_SHIFT              (0U)
#define PMU1CRU_SSGTBL16_19_SSGTBL16_19_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL16_19_SSGTBL16_19_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL20_23 */
#define PMU1CRU_SSGTBL20_23_OFFSET                         (0x294U)
#define PMU1CRU_SSGTBL20_23_SSGTBL20_23_SHIFT              (0U)
#define PMU1CRU_SSGTBL20_23_SSGTBL20_23_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL20_23_SSGTBL20_23_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL24_27 */
#define PMU1CRU_SSGTBL24_27_OFFSET                         (0x298U)
#define PMU1CRU_SSGTBL24_27_SSGTBL24_27_SHIFT              (0U)
#define PMU1CRU_SSGTBL24_27_SSGTBL24_27_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL24_27_SSGTBL24_27_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL28_31 */
#define PMU1CRU_SSGTBL28_31_OFFSET                         (0x29CU)
#define PMU1CRU_SSGTBL28_31_SSGTBL28_31_SHIFT              (0U)
#define PMU1CRU_SSGTBL28_31_SSGTBL28_31_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL28_31_SSGTBL28_31_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL32_35 */
#define PMU1CRU_SSGTBL32_35_OFFSET                         (0x2A0U)
#define PMU1CRU_SSGTBL32_35_SSGTBL32_35_SHIFT              (0U)
#define PMU1CRU_SSGTBL32_35_SSGTBL32_35_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL32_35_SSGTBL32_35_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL36_39 */
#define PMU1CRU_SSGTBL36_39_OFFSET                         (0x2A4U)
#define PMU1CRU_SSGTBL36_39_SSGTBL36_39_SHIFT              (0U)
#define PMU1CRU_SSGTBL36_39_SSGTBL36_39_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL36_39_SSGTBL36_39_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL40_43 */
#define PMU1CRU_SSGTBL40_43_OFFSET                         (0x2A8U)
#define PMU1CRU_SSGTBL40_43_SSGTBL40_43_SHIFT              (0U)
#define PMU1CRU_SSGTBL40_43_SSGTBL40_43_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL40_43_SSGTBL40_43_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL44_47 */
#define PMU1CRU_SSGTBL44_47_OFFSET                         (0x2ACU)
#define PMU1CRU_SSGTBL44_47_SSGTBL44_47_SHIFT              (0U)
#define PMU1CRU_SSGTBL44_47_SSGTBL44_47_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL44_47_SSGTBL44_47_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL48_51 */
#define PMU1CRU_SSGTBL48_51_OFFSET                         (0x2B0U)
#define PMU1CRU_SSGTBL48_51_SSGTBL48_51_SHIFT              (0U)
#define PMU1CRU_SSGTBL48_51_SSGTBL48_51_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL48_51_SSGTBL48_51_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL52_55 */
#define PMU1CRU_SSGTBL52_55_OFFSET                         (0x2B4U)
#define PMU1CRU_SSGTBL52_55_SSGTBL52_55_SHIFT              (0U)
#define PMU1CRU_SSGTBL52_55_SSGTBL52_55_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL52_55_SSGTBL52_55_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL56_59 */
#define PMU1CRU_SSGTBL56_59_OFFSET                         (0x2B8U)
#define PMU1CRU_SSGTBL56_59_SSGTBL56_59_SHIFT              (0U)
#define PMU1CRU_SSGTBL56_59_SSGTBL56_59_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL56_59_SSGTBL56_59_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL60_63 */
#define PMU1CRU_SSGTBL60_63_OFFSET                         (0x2BCU)
#define PMU1CRU_SSGTBL60_63_SSGTBL60_63_SHIFT              (0U)
#define PMU1CRU_SSGTBL60_63_SSGTBL60_63_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL60_63_SSGTBL60_63_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL64_67 */
#define PMU1CRU_SSGTBL64_67_OFFSET                         (0x2C0U)
#define PMU1CRU_SSGTBL64_67_SSGTBL64_67_SHIFT              (0U)
#define PMU1CRU_SSGTBL64_67_SSGTBL64_67_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL64_67_SSGTBL64_67_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL68_71 */
#define PMU1CRU_SSGTBL68_71_OFFSET                         (0x2C4U)
#define PMU1CRU_SSGTBL68_71_SSGTBL68_71_SHIFT              (0U)
#define PMU1CRU_SSGTBL68_71_SSGTBL68_71_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL68_71_SSGTBL68_71_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL72_75 */
#define PMU1CRU_SSGTBL72_75_OFFSET                         (0x2C8U)
#define PMU1CRU_SSGTBL72_75_SSGTBL72_75_SHIFT              (0U)
#define PMU1CRU_SSGTBL72_75_SSGTBL72_75_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL72_75_SSGTBL72_75_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL76_79 */
#define PMU1CRU_SSGTBL76_79_OFFSET                         (0x2CCU)
#define PMU1CRU_SSGTBL76_79_SSGTBL76_79_SHIFT              (0U)
#define PMU1CRU_SSGTBL76_79_SSGTBL76_79_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL76_79_SSGTBL76_79_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL80_83 */
#define PMU1CRU_SSGTBL80_83_OFFSET                         (0x2D0U)
#define PMU1CRU_SSGTBL80_83_SSGTBL80_83_SHIFT              (0U)
#define PMU1CRU_SSGTBL80_83_SSGTBL80_83_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL80_83_SSGTBL80_83_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL84_87 */
#define PMU1CRU_SSGTBL84_87_OFFSET                         (0x2D4U)
#define PMU1CRU_SSGTBL84_87_SSGTBL84_87_SHIFT              (0U)
#define PMU1CRU_SSGTBL84_87_SSGTBL84_87_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL84_87_SSGTBL84_87_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL88_91 */
#define PMU1CRU_SSGTBL88_91_OFFSET                         (0x2D8U)
#define PMU1CRU_SSGTBL88_91_SSGTBL88_91_SHIFT              (0U)
#define PMU1CRU_SSGTBL88_91_SSGTBL88_91_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL88_91_SSGTBL88_91_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL92_95 */
#define PMU1CRU_SSGTBL92_95_OFFSET                         (0x2DCU)
#define PMU1CRU_SSGTBL92_95_SSGTBL92_95_SHIFT              (0U)
#define PMU1CRU_SSGTBL92_95_SSGTBL92_95_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL92_95_SSGTBL92_95_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL96_99 */
#define PMU1CRU_SSGTBL96_99_OFFSET                         (0x2E0U)
#define PMU1CRU_SSGTBL96_99_SSGTBL96_99_SHIFT              (0U)
#define PMU1CRU_SSGTBL96_99_SSGTBL96_99_MASK               (0xFFFFFFFFU << PMU1CRU_SSGTBL96_99_SSGTBL96_99_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL100_103 */
#define PMU1CRU_SSGTBL100_103_OFFSET                       (0x2E4U)
#define PMU1CRU_SSGTBL100_103_SSGTBL100_103_SHIFT          (0U)
#define PMU1CRU_SSGTBL100_103_SSGTBL100_103_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL100_103_SSGTBL100_103_SHIFT)   /* 0xFFFFFFFF */
/* SSGTBL104_107 */
#define PMU1CRU_SSGTBL104_107_OFFSET                       (0x2E8U)
#define PMU1CRU_SSGTBL104_107_SSGTBL104_107_SHIFT          (0U)
#define PMU1CRU_SSGTBL104_107_SSGTBL104_107_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL104_107_SSGTBL104_107_SHIFT)   /* 0xFFFFFFFF */
/* SSGTBL108_111 */
#define PMU1CRU_SSGTBL108_111_OFFSET                       (0x2ECU)
#define PMU1CRU_SSGTBL108_111_SSGTBL108_111_SHIFT          (0U)
#define PMU1CRU_SSGTBL108_111_SSGTBL108_111_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL108_111_SSGTBL108_111_SHIFT)   /* 0xFFFFFFFF */
/* SSGTBL112_115 */
#define PMU1CRU_SSGTBL112_115_OFFSET                       (0x2F0U)
#define PMU1CRU_SSGTBL112_115_SSGTBL112_115_SHIFT          (0U)
#define PMU1CRU_SSGTBL112_115_SSGTBL112_115_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL112_115_SSGTBL112_115_SHIFT)   /* 0xFFFFFFFF */
/* SSGTBL116_119 */
#define PMU1CRU_SSGTBL116_119_OFFSET                       (0x2F4U)
#define PMU1CRU_SSGTBL116_119_SSGTBL116_119_SHIFT          (0U)
#define PMU1CRU_SSGTBL116_119_SSGTBL116_119_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL116_119_SSGTBL116_119_SHIFT)   /* 0xFFFFFFFF */
/* SSGTBL120_123 */
#define PMU1CRU_SSGTBL120_123_OFFSET                       (0x2F8U)
#define PMU1CRU_SSGTBL120_123_SSGTBL120_123_SHIFT          (0U)
#define PMU1CRU_SSGTBL120_123_SSGTBL120_123_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL120_123_SSGTBL120_123_SHIFT)   /* 0xFFFFFFFF */
/* SSGTBL124_127 */
#define PMU1CRU_SSGTBL124_127_OFFSET                       (0x2FCU)
#define PMU1CRU_SSGTBL124_127_SSGTBL124_127_SHIFT          (0U)
#define PMU1CRU_SSGTBL124_127_SSGTBL124_127_MASK           (0xFFFFFFFFU << PMU1CRU_SSGTBL124_127_SSGTBL124_127_SHIFT)   /* 0xFFFFFFFF */
/* PMU1MODE_CON00 */
#define PMU1CRU_PMU1MODE_CON00_OFFSET                      (0x380U)
#define PMU1CRU_PMU1MODE_CON00_CLK_CPLL_MODE_SHIFT         (0U)
#define PMU1CRU_PMU1MODE_CON00_CLK_CPLL_MODE_MASK          (0x3U << PMU1CRU_PMU1MODE_CON00_CLK_CPLL_MODE_SHIFT)         /* 0x00000003 */
/* CM0_GATMASK_CON */
#define PMU1CRU_CM0_GATMASK_CON_OFFSET                     (0x420U)
#define PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_DCLK_CM0S_EN_SHIFT (0U)
#define PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_DCLK_CM0S_EN_MASK (0x3U << PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_DCLK_CM0S_EN_SHIFT) /* 0x00000003 */
#define PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_HCLK_CM0S_EN_SHIFT (2U)
#define PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_HCLK_CM0S_EN_MASK (0x1U << PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_HCLK_CM0S_EN_SHIFT) /* 0x00000004 */
#define PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_SCLK_CM0S_EN_SHIFT (3U)
#define PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_SCLK_CM0S_EN_MASK (0x1U << PMU1CRU_CM0_GATMASK_CON_SGRF_CON_MCU_SCLK_CM0S_EN_SHIFT) /* 0x00000008 */
/* MISC_CON00 */
#define PMU1CRU_MISC_CON00_OFFSET                          (0x684U)
#define PMU1CRU_MISC_CON00_ENABLE_PD_PMU1_CLK_EN_SHIFT     (0U)
#define PMU1CRU_MISC_CON00_ENABLE_PD_PMU1_CLK_EN_MASK      (0x1U << PMU1CRU_MISC_CON00_ENABLE_PD_PMU1_CLK_EN_SHIFT)     /* 0x00000001 */
/*****************************************DDRCRU*****************************************/
/* CLKSEL_CON00 */
#define DDRCRU_CLKSEL_CON00_OFFSET                         (0x100U)
/* CLKSEL_CON01 */
#define DDRCRU_CLKSEL_CON01_OFFSET                         (0x104U)
#define DDRCRU_CLKSEL_CON01_CLK_MSCH_BRG_BIU_DIV_SHIFT     (0U)
#define DDRCRU_CLKSEL_CON01_CLK_MSCH_BRG_BIU_DIV_MASK      (0xFU << DDRCRU_CLKSEL_CON01_CLK_MSCH_BRG_BIU_DIV_SHIFT)     /* 0x0000000F */
#define DDRCRU_CLKSEL_CON01_PCLK_DDR_DIV_SHIFT             (8U)
#define DDRCRU_CLKSEL_CON01_PCLK_DDR_DIV_MASK              (0x1FU << DDRCRU_CLKSEL_CON01_PCLK_DDR_DIV_SHIFT)            /* 0x00001F00 */
/* GATE_CON00 */
#define DDRCRU_GATE_CON00_OFFSET                           (0x180U)
#define DDRCRU_GATE_CON00_CLK_GPLL_MUX_TO_SUBDDR_EN_SHIFT  (0U)
#define DDRCRU_GATE_CON00_CLK_GPLL_MUX_TO_SUBDDR_EN_MASK   (0x1U << DDRCRU_GATE_CON00_CLK_GPLL_MUX_TO_SUBDDR_EN_SHIFT)  /* 0x00000001 */
#define DDRCRU_GATE_CON00_XIN_OSC0_FUNC_TO_SUBDDR_EN_SHIFT (1U)
#define DDRCRU_GATE_CON00_XIN_OSC0_FUNC_TO_SUBDDR_EN_MASK  (0x1U << DDRCRU_GATE_CON00_XIN_OSC0_FUNC_TO_SUBDDR_EN_SHIFT) /* 0x00000002 */
#define DDRCRU_GATE_CON00_CLK_DEEPSLOW_TO_SUBDDR_EN_SHIFT  (2U)
#define DDRCRU_GATE_CON00_CLK_DEEPSLOW_TO_SUBDDR_EN_MASK   (0x1U << DDRCRU_GATE_CON00_CLK_DEEPSLOW_TO_SUBDDR_EN_SHIFT)  /* 0x00000004 */
#define DDRCRU_GATE_CON00_PCLK_DDR_EN_SHIFT                (3U)
#define DDRCRU_GATE_CON00_PCLK_DDR_EN_MASK                 (0x1U << DDRCRU_GATE_CON00_PCLK_DDR_EN_SHIFT)                /* 0x00000008 */
#define DDRCRU_GATE_CON00_CLK_MSCH_BRG_BIU_EN_SHIFT        (4U)
#define DDRCRU_GATE_CON00_CLK_MSCH_BRG_BIU_EN_MASK         (0x1U << DDRCRU_GATE_CON00_CLK_MSCH_BRG_BIU_EN_SHIFT)        /* 0x00000010 */
#define DDRCRU_GATE_CON00_PCLK_MSCH_BIU_EN_SHIFT           (5U)
#define DDRCRU_GATE_CON00_PCLK_MSCH_BIU_EN_MASK            (0x1U << DDRCRU_GATE_CON00_PCLK_MSCH_BIU_EN_SHIFT)           /* 0x00000020 */
#define DDRCRU_GATE_CON00_PCLK_DDR_HWLP_EN_SHIFT           (6U)
#define DDRCRU_GATE_CON00_PCLK_DDR_HWLP_EN_MASK            (0x1U << DDRCRU_GATE_CON00_PCLK_DDR_HWLP_EN_SHIFT)           /* 0x00000040 */
#define DDRCRU_GATE_CON00_PCLK_DDR_PHY_EN_SHIFT            (8U)
#define DDRCRU_GATE_CON00_PCLK_DDR_PHY_EN_MASK             (0x1U << DDRCRU_GATE_CON00_PCLK_DDR_PHY_EN_SHIFT)            /* 0x00000100 */
#define DDRCRU_GATE_CON00_PCLK_DDR_DFICTL_EN_SHIFT         (9U)
#define DDRCRU_GATE_CON00_PCLK_DDR_DFICTL_EN_MASK          (0x1U << DDRCRU_GATE_CON00_PCLK_DDR_DFICTL_EN_SHIFT)         /* 0x00000200 */
#define DDRCRU_GATE_CON00_PCLK_DDR_DMA2DDR_EN_SHIFT        (10U)
#define DDRCRU_GATE_CON00_PCLK_DDR_DMA2DDR_EN_MASK         (0x1U << DDRCRU_GATE_CON00_PCLK_DDR_DMA2DDR_EN_SHIFT)        /* 0x00000400 */
/* GATE_CON01 */
#define DDRCRU_GATE_CON01_OFFSET                           (0x184U)
#define DDRCRU_GATE_CON01_PCLK_DDR_MON_EN_SHIFT            (0U)
#define DDRCRU_GATE_CON01_PCLK_DDR_MON_EN_MASK             (0x1U << DDRCRU_GATE_CON01_PCLK_DDR_MON_EN_SHIFT)            /* 0x00000001 */
#define DDRCRU_GATE_CON01_TMCLK_DDR_MON_EN_SHIFT           (1U)
#define DDRCRU_GATE_CON01_TMCLK_DDR_MON_EN_MASK            (0x1U << DDRCRU_GATE_CON01_TMCLK_DDR_MON_EN_SHIFT)           /* 0x00000002 */
#define DDRCRU_GATE_CON01_PCLK_DDR_GRF_EN_SHIFT            (2U)
#define DDRCRU_GATE_CON01_PCLK_DDR_GRF_EN_MASK             (0x1U << DDRCRU_GATE_CON01_PCLK_DDR_GRF_EN_SHIFT)            /* 0x00000004 */
#define DDRCRU_GATE_CON01_PCLK_DDR_CRU_EN_SHIFT            (3U)
#define DDRCRU_GATE_CON01_PCLK_DDR_CRU_EN_MASK             (0x1U << DDRCRU_GATE_CON01_PCLK_DDR_CRU_EN_SHIFT)            /* 0x00000008 */
#define DDRCRU_GATE_CON01_PCLK_SUBDDR_CRU_EN_SHIFT         (4U)
#define DDRCRU_GATE_CON01_PCLK_SUBDDR_CRU_EN_MASK          (0x1U << DDRCRU_GATE_CON01_PCLK_SUBDDR_CRU_EN_SHIFT)         /* 0x00000010 */
/* SOFTRST_CON00 */
#define DDRCRU_SOFTRST_CON00_OFFSET                        (0x200U)
#define DDRCRU_SOFTRST_CON00_RESETN_MSCH_BRG_BIU_SHIFT     (4U)
#define DDRCRU_SOFTRST_CON00_RESETN_MSCH_BRG_BIU_MASK      (0x1U << DDRCRU_SOFTRST_CON00_RESETN_MSCH_BRG_BIU_SHIFT)     /* 0x00000010 */
#define DDRCRU_SOFTRST_CON00_PRESETN_MSCH_BIU_SHIFT        (5U)
#define DDRCRU_SOFTRST_CON00_PRESETN_MSCH_BIU_MASK         (0x1U << DDRCRU_SOFTRST_CON00_PRESETN_MSCH_BIU_SHIFT)        /* 0x00000020 */
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_HWLP_SHIFT        (6U)
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_HWLP_MASK         (0x1U << DDRCRU_SOFTRST_CON00_PRESETN_DDR_HWLP_SHIFT)        /* 0x00000040 */
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_PHY_SHIFT         (8U)
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_PHY_MASK          (0x1U << DDRCRU_SOFTRST_CON00_PRESETN_DDR_PHY_SHIFT)         /* 0x00000100 */
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_DFICTL_SHIFT      (9U)
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_DFICTL_MASK       (0x1U << DDRCRU_SOFTRST_CON00_PRESETN_DDR_DFICTL_SHIFT)      /* 0x00000200 */
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_DMA2DDR_SHIFT     (10U)
#define DDRCRU_SOFTRST_CON00_PRESETN_DDR_DMA2DDR_MASK      (0x1U << DDRCRU_SOFTRST_CON00_PRESETN_DDR_DMA2DDR_SHIFT)     /* 0x00000400 */
/* SOFTRST_CON01 */
#define DDRCRU_SOFTRST_CON01_OFFSET                        (0x204U)
#define DDRCRU_SOFTRST_CON01_PRESETN_DDR_MON_SHIFT         (0U)
#define DDRCRU_SOFTRST_CON01_PRESETN_DDR_MON_MASK          (0x1U << DDRCRU_SOFTRST_CON01_PRESETN_DDR_MON_SHIFT)         /* 0x00000001 */
#define DDRCRU_SOFTRST_CON01_TMRESETN_DDR_MON_SHIFT        (1U)
#define DDRCRU_SOFTRST_CON01_TMRESETN_DDR_MON_MASK         (0x1U << DDRCRU_SOFTRST_CON01_TMRESETN_DDR_MON_SHIFT)        /* 0x00000002 */
#define DDRCRU_SOFTRST_CON01_PRESETN_DDR_GRF_SHIFT         (2U)
#define DDRCRU_SOFTRST_CON01_PRESETN_DDR_GRF_MASK          (0x1U << DDRCRU_SOFTRST_CON01_PRESETN_DDR_GRF_SHIFT)         /* 0x00000004 */
#define DDRCRU_SOFTRST_CON01_PRESETN_DDR_CRU_SHIFT         (3U)
#define DDRCRU_SOFTRST_CON01_PRESETN_DDR_CRU_MASK          (0x1U << DDRCRU_SOFTRST_CON01_PRESETN_DDR_CRU_SHIFT)         /* 0x00000008 */
#define DDRCRU_SOFTRST_CON01_PRESETN_SUBDDR_CRU_SHIFT      (4U)
#define DDRCRU_SOFTRST_CON01_PRESETN_SUBDDR_CRU_MASK       (0x1U << DDRCRU_SOFTRST_CON01_PRESETN_SUBDDR_CRU_SHIFT)      /* 0x00000010 */
/* MISC_CON0 */
#define DDRCRU_MISC_CON0_OFFSET                            (0x684U)
#define DDRCRU_MISC_CON0_ENABLE_PD_DDR_CLK_EN_SHIFT        (0U)
#define DDRCRU_MISC_CON0_ENABLE_PD_DDR_CLK_EN_MASK         (0x1U << DDRCRU_MISC_CON0_ENABLE_PD_DDR_CLK_EN_SHIFT)        /* 0x00000001 */
/***************************************SUBDDRCRU****************************************/
/* DPLL_CON0 */
#define SUBDDRCRU_DPLL_CON0_OFFSET                         (0x20U)
#define SUBDDRCRU_DPLL_CON0_FBDIV_SHIFT                    (0U)
#define SUBDDRCRU_DPLL_CON0_FBDIV_MASK                     (0xFFFU << SUBDDRCRU_DPLL_CON0_FBDIV_SHIFT)                  /* 0x00000FFF */
#define SUBDDRCRU_DPLL_CON0_POSTDIV1_SHIFT                 (12U)
#define SUBDDRCRU_DPLL_CON0_POSTDIV1_MASK                  (0x7U << SUBDDRCRU_DPLL_CON0_POSTDIV1_SHIFT)                 /* 0x00007000 */
#define SUBDDRCRU_DPLL_CON0_BYPASS_SHIFT                   (15U)
#define SUBDDRCRU_DPLL_CON0_BYPASS_MASK                    (0x1U << SUBDDRCRU_DPLL_CON0_BYPASS_SHIFT)                   /* 0x00008000 */
/* DPLL_CON1 */
#define SUBDDRCRU_DPLL_CON1_OFFSET                         (0x24U)
#define SUBDDRCRU_DPLL_CON1_REFDIV_SHIFT                   (0U)
#define SUBDDRCRU_DPLL_CON1_REFDIV_MASK                    (0x3FU << SUBDDRCRU_DPLL_CON1_REFDIV_SHIFT)                  /* 0x0000003F */
#define SUBDDRCRU_DPLL_CON1_POSTDIV2_SHIFT                 (6U)
#define SUBDDRCRU_DPLL_CON1_POSTDIV2_MASK                  (0x7U << SUBDDRCRU_DPLL_CON1_POSTDIV2_SHIFT)                 /* 0x000001C0 */
#define SUBDDRCRU_DPLL_CON1_PLL_LOCK_SHIFT                 (10U)
#define SUBDDRCRU_DPLL_CON1_PLL_LOCK_MASK                  (0x1U << SUBDDRCRU_DPLL_CON1_PLL_LOCK_SHIFT)                 /* 0x00000400 */
#define SUBDDRCRU_DPLL_CON1_DSMPD_SHIFT                    (12U)
#define SUBDDRCRU_DPLL_CON1_DSMPD_MASK                     (0x1U << SUBDDRCRU_DPLL_CON1_DSMPD_SHIFT)                    /* 0x00001000 */
#define SUBDDRCRU_DPLL_CON1_PLLPD0_SHIFT                   (13U)
#define SUBDDRCRU_DPLL_CON1_PLLPD0_MASK                    (0x1U << SUBDDRCRU_DPLL_CON1_PLLPD0_SHIFT)                   /* 0x00002000 */
#define SUBDDRCRU_DPLL_CON1_PLLPD1_SHIFT                   (14U)
#define SUBDDRCRU_DPLL_CON1_PLLPD1_MASK                    (0x1U << SUBDDRCRU_DPLL_CON1_PLLPD1_SHIFT)                   /* 0x00004000 */
#define SUBDDRCRU_DPLL_CON1_PLLPDSEL_SHIFT                 (15U)
#define SUBDDRCRU_DPLL_CON1_PLLPDSEL_MASK                  (0x1U << SUBDDRCRU_DPLL_CON1_PLLPDSEL_SHIFT)                 /* 0x00008000 */
/* DPLL_CON2 */
#define SUBDDRCRU_DPLL_CON2_OFFSET                         (0x28U)
#define SUBDDRCRU_DPLL_CON2_FRACDIV_SHIFT                  (0U)
#define SUBDDRCRU_DPLL_CON2_FRACDIV_MASK                   (0xFFFFFFU << SUBDDRCRU_DPLL_CON2_FRACDIV_SHIFT)             /* 0x00FFFFFF */
#define SUBDDRCRU_DPLL_CON2_DACPD_SHIFT                    (24U)
#define SUBDDRCRU_DPLL_CON2_DACPD_MASK                     (0x1U << SUBDDRCRU_DPLL_CON2_DACPD_SHIFT)                    /* 0x01000000 */
#define SUBDDRCRU_DPLL_CON2_FOUTPOSTDIVPD_SHIFT            (25U)
#define SUBDDRCRU_DPLL_CON2_FOUTPOSTDIVPD_MASK             (0x1U << SUBDDRCRU_DPLL_CON2_FOUTPOSTDIVPD_SHIFT)            /* 0x02000000 */
#define SUBDDRCRU_DPLL_CON2_FOUTVCOPD_SHIFT                (26U)
#define SUBDDRCRU_DPLL_CON2_FOUTVCOPD_MASK                 (0x1U << SUBDDRCRU_DPLL_CON2_FOUTVCOPD_SHIFT)                /* 0x04000000 */
#define SUBDDRCRU_DPLL_CON2_FOUT4PHASEPD_SHIFT             (27U)
#define SUBDDRCRU_DPLL_CON2_FOUT4PHASEPD_MASK              (0x1U << SUBDDRCRU_DPLL_CON2_FOUT4PHASEPD_SHIFT)             /* 0x08000000 */
/* DPLL_CON3 */
#define SUBDDRCRU_DPLL_CON3_OFFSET                         (0x2CU)
#define SUBDDRCRU_DPLL_CON3_SSMOD_BP_SHIFT                 (0U)
#define SUBDDRCRU_DPLL_CON3_SSMOD_BP_MASK                  (0x1U << SUBDDRCRU_DPLL_CON3_SSMOD_BP_SHIFT)                 /* 0x00000001 */
#define SUBDDRCRU_DPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT       (1U)
#define SUBDDRCRU_DPLL_CON3_SSMOD_DISABLE_SSCG_MASK        (0x1U << SUBDDRCRU_DPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)       /* 0x00000002 */
#define SUBDDRCRU_DPLL_CON3_SSMOD_RESET_SHIFT              (2U)
#define SUBDDRCRU_DPLL_CON3_SSMOD_RESET_MASK               (0x1U << SUBDDRCRU_DPLL_CON3_SSMOD_RESET_SHIFT)              /* 0x00000004 */
#define SUBDDRCRU_DPLL_CON3_SSMOD_DOWNSPREAD_SHIFT         (3U)
#define SUBDDRCRU_DPLL_CON3_SSMOD_DOWNSPREAD_MASK          (0x1U << SUBDDRCRU_DPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)         /* 0x00000008 */
#define SUBDDRCRU_DPLL_CON3_SSMOD_DIVVAL_SHIFT             (4U)
#define SUBDDRCRU_DPLL_CON3_SSMOD_DIVVAL_MASK              (0xFU << SUBDDRCRU_DPLL_CON3_SSMOD_DIVVAL_SHIFT)             /* 0x000000F0 */
#define SUBDDRCRU_DPLL_CON3_SSMOD_SPREAD_SHIFT             (8U)
#define SUBDDRCRU_DPLL_CON3_SSMOD_SPREAD_MASK              (0x1FU << SUBDDRCRU_DPLL_CON3_SSMOD_SPREAD_SHIFT)            /* 0x00001F00 */
/* DPLL_CON4 */
#define SUBDDRCRU_DPLL_CON4_OFFSET                         (0x30U)
#define SUBDDRCRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT       (0U)
#define SUBDDRCRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK        (0x1U << SUBDDRCRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)       /* 0x00000001 */
#define SUBDDRCRU_DPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT        (8U)
#define SUBDDRCRU_DPLL_CON4_SSMOD_EXT_MAXADDR_MASK         (0xFFU << SUBDDRCRU_DPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)       /* 0x0000FF00 */
/* CLKSEL_CON00 */
#define SUBDDRCRU_CLKSEL_CON00_OFFSET                      (0x100U)
#define SUBDDRCRU_CLKSEL_CON00_CLK_DDR_SEL_SHIFT           (7U)
#define SUBDDRCRU_CLKSEL_CON00_CLK_DDR_SEL_MASK            (0x1U << SUBDDRCRU_CLKSEL_CON00_CLK_DDR_SEL_SHIFT)           /* 0x00000080 */
#define SUBDDRCRU_CLKSEL_CON00_CLK_TESTOUT_SUBDDR_DIV_SHIFT (8U)
#define SUBDDRCRU_CLKSEL_CON00_CLK_TESTOUT_SUBDDR_DIV_MASK (0x3U << SUBDDRCRU_CLKSEL_CON00_CLK_TESTOUT_SUBDDR_DIV_SHIFT) /* 0x00000300 */
#define SUBDDRCRU_CLKSEL_CON00_CLK_TESTOUT_SUBDDR_SEL_SHIFT (15U)
#define SUBDDRCRU_CLKSEL_CON00_CLK_TESTOUT_SUBDDR_SEL_MASK (0x1U << SUBDDRCRU_CLKSEL_CON00_CLK_TESTOUT_SUBDDR_SEL_SHIFT) /* 0x00008000 */
/* GATE_CON00 */
#define SUBDDRCRU_GATE_CON00_OFFSET                        (0x180U)
#define SUBDDRCRU_GATE_CON00_CLK_DDR_EN_SHIFT              (0U)
#define SUBDDRCRU_GATE_CON00_CLK_DDR_EN_MASK               (0x1U << SUBDDRCRU_GATE_CON00_CLK_DDR_EN_SHIFT)              /* 0x00000001 */
#define SUBDDRCRU_GATE_CON00_CLK_MSCH_BIU_EN_SHIFT         (1U)
#define SUBDDRCRU_GATE_CON00_CLK_MSCH_BIU_EN_MASK          (0x1U << SUBDDRCRU_GATE_CON00_CLK_MSCH_BIU_EN_SHIFT)         /* 0x00000002 */
#define SUBDDRCRU_GATE_CON00_CLK1X_DDR_PHY_EN_SHIFT        (4U)
#define SUBDDRCRU_GATE_CON00_CLK1X_DDR_PHY_EN_MASK         (0x1U << SUBDDRCRU_GATE_CON00_CLK1X_DDR_PHY_EN_SHIFT)        /* 0x00000010 */
#define SUBDDRCRU_GATE_CON00_CLK_DDR_DFICTL_EN_SHIFT       (5U)
#define SUBDDRCRU_GATE_CON00_CLK_DDR_DFICTL_EN_MASK        (0x1U << SUBDDRCRU_GATE_CON00_CLK_DDR_DFICTL_EN_SHIFT)       /* 0x00000020 */
#define SUBDDRCRU_GATE_CON00_CLK_DDR_SCRAMBLE_EN_SHIFT     (6U)
#define SUBDDRCRU_GATE_CON00_CLK_DDR_SCRAMBLE_EN_MASK      (0x1U << SUBDDRCRU_GATE_CON00_CLK_DDR_SCRAMBLE_EN_SHIFT)     /* 0x00000040 */
#define SUBDDRCRU_GATE_CON00_CLK_DDR_MON_EN_SHIFT          (7U)
#define SUBDDRCRU_GATE_CON00_CLK_DDR_MON_EN_MASK           (0x1U << SUBDDRCRU_GATE_CON00_CLK_DDR_MON_EN_SHIFT)          /* 0x00000080 */
#define SUBDDRCRU_GATE_CON00_ACLK_DDR_SPLIT_EN_SHIFT       (8U)
#define SUBDDRCRU_GATE_CON00_ACLK_DDR_SPLIT_EN_MASK        (0x1U << SUBDDRCRU_GATE_CON00_ACLK_DDR_SPLIT_EN_SHIFT)       /* 0x00000100 */
#define SUBDDRCRU_GATE_CON00_ACLK_DDR_DMA2DDR_EN_SHIFT     (9U)
#define SUBDDRCRU_GATE_CON00_ACLK_DDR_DMA2DDR_EN_MASK      (0x1U << SUBDDRCRU_GATE_CON00_ACLK_DDR_DMA2DDR_EN_SHIFT)     /* 0x00000200 */
#define SUBDDRCRU_GATE_CON00_CLK_TESTOUT_SUBDDR_EN_SHIFT   (15U)
#define SUBDDRCRU_GATE_CON00_CLK_TESTOUT_SUBDDR_EN_MASK    (0x1U << SUBDDRCRU_GATE_CON00_CLK_TESTOUT_SUBDDR_EN_SHIFT)   /* 0x00008000 */
/* SOFTRST_CON00 */
#define SUBDDRCRU_SOFTRST_CON00_OFFSET                     (0x200U)
#define SUBDDRCRU_SOFTRST_CON00_RESETN_MSCH_BIU_SHIFT      (1U)
#define SUBDDRCRU_SOFTRST_CON00_RESETN_MSCH_BIU_MASK       (0x1U << SUBDDRCRU_SOFTRST_CON00_RESETN_MSCH_BIU_SHIFT)      /* 0x00000002 */
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_PHY_SHIFT       (4U)
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_PHY_MASK        (0x1U << SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_PHY_SHIFT)       /* 0x00000010 */
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_DFICTL_SHIFT    (5U)
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_DFICTL_MASK     (0x1U << SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_DFICTL_SHIFT)    /* 0x00000020 */
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_SCRAMBLE_SHIFT  (6U)
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_SCRAMBLE_MASK   (0x1U << SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_SCRAMBLE_SHIFT)  /* 0x00000040 */
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_MON_SHIFT       (7U)
#define SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_MON_MASK        (0x1U << SUBDDRCRU_SOFTRST_CON00_RESETN_DDR_MON_SHIFT)       /* 0x00000080 */
#define SUBDDRCRU_SOFTRST_CON00_ARESETN_DDR_SPLIT_SHIFT    (8U)
#define SUBDDRCRU_SOFTRST_CON00_ARESETN_DDR_SPLIT_MASK     (0x1U << SUBDDRCRU_SOFTRST_CON00_ARESETN_DDR_SPLIT_SHIFT)    /* 0x00000100 */
#define SUBDDRCRU_SOFTRST_CON00_ARESETN_DDR_DMA2DDR_SHIFT  (9U)
#define SUBDDRCRU_SOFTRST_CON00_ARESETN_DDR_DMA2DDR_MASK   (0x1U << SUBDDRCRU_SOFTRST_CON00_ARESETN_DDR_DMA2DDR_SHIFT)  /* 0x00000200 */
/* SSGTBL0_3 */
#define SUBDDRCRU_SSGTBL0_3_OFFSET                         (0x280U)
#define SUBDDRCRU_SSGTBL0_3_SSGTBL0_3_SHIFT                (0U)
#define SUBDDRCRU_SSGTBL0_3_SSGTBL0_3_MASK                 (0xFFFFFFFFU << SUBDDRCRU_SSGTBL0_3_SSGTBL0_3_SHIFT)         /* 0xFFFFFFFF */
/* SSGTBL4_7 */
#define SUBDDRCRU_SSGTBL4_7_OFFSET                         (0x284U)
#define SUBDDRCRU_SSGTBL4_7_SSGTBL4_7_SHIFT                (0U)
#define SUBDDRCRU_SSGTBL4_7_SSGTBL4_7_MASK                 (0xFFFFFFFFU << SUBDDRCRU_SSGTBL4_7_SSGTBL4_7_SHIFT)         /* 0xFFFFFFFF */
/* SSGTBL8_11 */
#define SUBDDRCRU_SSGTBL8_11_OFFSET                        (0x288U)
#define SUBDDRCRU_SSGTBL8_11_SSGTBL8_11_SHIFT              (0U)
#define SUBDDRCRU_SSGTBL8_11_SSGTBL8_11_MASK               (0xFFFFFFFFU << SUBDDRCRU_SSGTBL8_11_SSGTBL8_11_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL12_15 */
#define SUBDDRCRU_SSGTBL12_15_OFFSET                       (0x28CU)
#define SUBDDRCRU_SSGTBL12_15_SSGTBL12_15_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL12_15_SSGTBL12_15_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL12_15_SSGTBL12_15_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL16_19 */
#define SUBDDRCRU_SSGTBL16_19_OFFSET                       (0x290U)
#define SUBDDRCRU_SSGTBL16_19_SSGTBL16_19_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL16_19_SSGTBL16_19_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL16_19_SSGTBL16_19_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL20_23 */
#define SUBDDRCRU_SSGTBL20_23_OFFSET                       (0x294U)
#define SUBDDRCRU_SSGTBL20_23_SSGTBL20_23_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL20_23_SSGTBL20_23_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL20_23_SSGTBL20_23_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL24_27 */
#define SUBDDRCRU_SSGTBL24_27_OFFSET                       (0x298U)
#define SUBDDRCRU_SSGTBL24_27_SSGTBL24_27_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL24_27_SSGTBL24_27_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL24_27_SSGTBL24_27_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL28_31 */
#define SUBDDRCRU_SSGTBL28_31_OFFSET                       (0x29CU)
#define SUBDDRCRU_SSGTBL28_31_SSGTBL28_31_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL28_31_SSGTBL28_31_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL28_31_SSGTBL28_31_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL32_35 */
#define SUBDDRCRU_SSGTBL32_35_OFFSET                       (0x2A0U)
#define SUBDDRCRU_SSGTBL32_35_SSGTBL32_35_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL32_35_SSGTBL32_35_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL32_35_SSGTBL32_35_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL36_39 */
#define SUBDDRCRU_SSGTBL36_39_OFFSET                       (0x2A4U)
#define SUBDDRCRU_SSGTBL36_39_SSGTBL36_39_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL36_39_SSGTBL36_39_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL36_39_SSGTBL36_39_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL40_43 */
#define SUBDDRCRU_SSGTBL40_43_OFFSET                       (0x2A8U)
#define SUBDDRCRU_SSGTBL40_43_SSGTBL40_43_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL40_43_SSGTBL40_43_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL40_43_SSGTBL40_43_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL44_47 */
#define SUBDDRCRU_SSGTBL44_47_OFFSET                       (0x2ACU)
#define SUBDDRCRU_SSGTBL44_47_SSGTBL44_47_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL44_47_SSGTBL44_47_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL44_47_SSGTBL44_47_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL48_51 */
#define SUBDDRCRU_SSGTBL48_51_OFFSET                       (0x2B0U)
#define SUBDDRCRU_SSGTBL48_51_SSGTBL48_51_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL48_51_SSGTBL48_51_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL48_51_SSGTBL48_51_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL52_55 */
#define SUBDDRCRU_SSGTBL52_55_OFFSET                       (0x2B4U)
#define SUBDDRCRU_SSGTBL52_55_SSGTBL52_55_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL52_55_SSGTBL52_55_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL52_55_SSGTBL52_55_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL56_59 */
#define SUBDDRCRU_SSGTBL56_59_OFFSET                       (0x2B8U)
#define SUBDDRCRU_SSGTBL56_59_SSGTBL56_59_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL56_59_SSGTBL56_59_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL56_59_SSGTBL56_59_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL60_63 */
#define SUBDDRCRU_SSGTBL60_63_OFFSET                       (0x2BCU)
#define SUBDDRCRU_SSGTBL60_63_SSGTBL60_63_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL60_63_SSGTBL60_63_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL60_63_SSGTBL60_63_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL64_67 */
#define SUBDDRCRU_SSGTBL64_67_OFFSET                       (0x2C0U)
#define SUBDDRCRU_SSGTBL64_67_SSGTBL64_67_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL64_67_SSGTBL64_67_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL64_67_SSGTBL64_67_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL68_71 */
#define SUBDDRCRU_SSGTBL68_71_OFFSET                       (0x2C4U)
#define SUBDDRCRU_SSGTBL68_71_SSGTBL68_71_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL68_71_SSGTBL68_71_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL68_71_SSGTBL68_71_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL72_75 */
#define SUBDDRCRU_SSGTBL72_75_OFFSET                       (0x2C8U)
#define SUBDDRCRU_SSGTBL72_75_SSGTBL72_75_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL72_75_SSGTBL72_75_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL72_75_SSGTBL72_75_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL76_79 */
#define SUBDDRCRU_SSGTBL76_79_OFFSET                       (0x2CCU)
#define SUBDDRCRU_SSGTBL76_79_SSGTBL76_79_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL76_79_SSGTBL76_79_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL76_79_SSGTBL76_79_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL80_83 */
#define SUBDDRCRU_SSGTBL80_83_OFFSET                       (0x2D0U)
#define SUBDDRCRU_SSGTBL80_83_SSGTBL80_83_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL80_83_SSGTBL80_83_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL80_83_SSGTBL80_83_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL84_87 */
#define SUBDDRCRU_SSGTBL84_87_OFFSET                       (0x2D4U)
#define SUBDDRCRU_SSGTBL84_87_SSGTBL84_87_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL84_87_SSGTBL84_87_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL84_87_SSGTBL84_87_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL88_91 */
#define SUBDDRCRU_SSGTBL88_91_OFFSET                       (0x2D8U)
#define SUBDDRCRU_SSGTBL88_91_SSGTBL88_91_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL88_91_SSGTBL88_91_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL88_91_SSGTBL88_91_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL92_95 */
#define SUBDDRCRU_SSGTBL92_95_OFFSET                       (0x2DCU)
#define SUBDDRCRU_SSGTBL92_95_SSGTBL92_95_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL92_95_SSGTBL92_95_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL92_95_SSGTBL92_95_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL96_99 */
#define SUBDDRCRU_SSGTBL96_99_OFFSET                       (0x2E0U)
#define SUBDDRCRU_SSGTBL96_99_SSGTBL96_99_SHIFT            (0U)
#define SUBDDRCRU_SSGTBL96_99_SSGTBL96_99_MASK             (0xFFFFFFFFU << SUBDDRCRU_SSGTBL96_99_SSGTBL96_99_SHIFT)     /* 0xFFFFFFFF */
/* SSGTBL100_103 */
#define SUBDDRCRU_SSGTBL100_103_OFFSET                     (0x2E4U)
#define SUBDDRCRU_SSGTBL100_103_SSGTBL100_103_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL100_103_SSGTBL100_103_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL100_103_SSGTBL100_103_SHIFT) /* 0xFFFFFFFF */
/* SSGTBL104_107 */
#define SUBDDRCRU_SSGTBL104_107_OFFSET                     (0x2E8U)
#define SUBDDRCRU_SSGTBL104_107_SSGTBL104_107_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL104_107_SSGTBL104_107_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL104_107_SSGTBL104_107_SHIFT) /* 0xFFFFFFFF */
/* SSGTBL108_111 */
#define SUBDDRCRU_SSGTBL108_111_OFFSET                     (0x2ECU)
#define SUBDDRCRU_SSGTBL108_111_SSGTBL108_111_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL108_111_SSGTBL108_111_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL108_111_SSGTBL108_111_SHIFT) /* 0xFFFFFFFF */
/* SSGTBL112_115 */
#define SUBDDRCRU_SSGTBL112_115_OFFSET                     (0x2F0U)
#define SUBDDRCRU_SSGTBL112_115_SSGTBL112_115_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL112_115_SSGTBL112_115_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL112_115_SSGTBL112_115_SHIFT) /* 0xFFFFFFFF */
/* SSGTBL116_119 */
#define SUBDDRCRU_SSGTBL116_119_OFFSET                     (0x2F4U)
#define SUBDDRCRU_SSGTBL116_119_SSGTBL116_119_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL116_119_SSGTBL116_119_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL116_119_SSGTBL116_119_SHIFT) /* 0xFFFFFFFF */
/* SSGTBL120_123 */
#define SUBDDRCRU_SSGTBL120_123_OFFSET                     (0x2F8U)
#define SUBDDRCRU_SSGTBL120_123_SSGTBL120_123_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL120_123_SSGTBL120_123_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL120_123_SSGTBL120_123_SHIFT) /* 0xFFFFFFFF */
/* SSGTBL124_127 */
#define SUBDDRCRU_SSGTBL124_127_OFFSET                     (0x2FCU)
#define SUBDDRCRU_SSGTBL124_127_SSGTBL124_127_SHIFT        (0U)
#define SUBDDRCRU_SSGTBL124_127_SSGTBL124_127_MASK         (0xFFFFFFFFU << SUBDDRCRU_SSGTBL124_127_SSGTBL124_127_SHIFT) /* 0xFFFFFFFF */
/* SUBDDRMODE_CON00 */
#define SUBDDRCRU_SUBDDRMODE_CON00_OFFSET                  (0x380U)
#define SUBDDRCRU_SUBDDRMODE_CON00_CLK_DPLL_MODE_SHIFT     (0U)
#define SUBDDRCRU_SUBDDRMODE_CON00_CLK_DPLL_MODE_MASK      (0x3U << SUBDDRCRU_SUBDDRMODE_CON00_CLK_DPLL_MODE_SHIFT)     /* 0x00000003 */
/****************************************PERICRU*****************************************/
/* CLKSEL_CON00 */
#define PERICRU_CLKSEL_CON00_OFFSET                        (0x100U)
#define PERICRU_CLKSEL_CON00_ACLK_PERI_DIV_SHIFT           (0U)
#define PERICRU_CLKSEL_CON00_ACLK_PERI_DIV_MASK            (0x1FU << PERICRU_CLKSEL_CON00_ACLK_PERI_DIV_SHIFT)          /* 0x0000001F */
#define PERICRU_CLKSEL_CON00_ACLK_PERI_SEL_SHIFT           (7U)
#define PERICRU_CLKSEL_CON00_ACLK_PERI_SEL_MASK            (0x1U << PERICRU_CLKSEL_CON00_ACLK_PERI_SEL_SHIFT)           /* 0x00000080 */
#define PERICRU_CLKSEL_CON00_HCLK_PERI_DIV_SHIFT           (8U)
#define PERICRU_CLKSEL_CON00_HCLK_PERI_DIV_MASK            (0x3FU << PERICRU_CLKSEL_CON00_HCLK_PERI_DIV_SHIFT)          /* 0x00003F00 */
#define PERICRU_CLKSEL_CON00_HCLK_PERI_SEL_SHIFT           (15U)
#define PERICRU_CLKSEL_CON00_HCLK_PERI_SEL_MASK            (0x1U << PERICRU_CLKSEL_CON00_HCLK_PERI_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL_CON01 */
#define PERICRU_CLKSEL_CON01_OFFSET                        (0x104U)
#define PERICRU_CLKSEL_CON01_PCLK_PERI_DIV_SHIFT           (0U)
#define PERICRU_CLKSEL_CON01_PCLK_PERI_DIV_MASK            (0x1FU << PERICRU_CLKSEL_CON01_PCLK_PERI_DIV_SHIFT)          /* 0x0000001F */
#define PERICRU_CLKSEL_CON01_PCLK_PERI_SEL_SHIFT           (7U)
#define PERICRU_CLKSEL_CON01_PCLK_PERI_SEL_MASK            (0x1U << PERICRU_CLKSEL_CON01_PCLK_PERI_SEL_SHIFT)           /* 0x00000080 */
#define PERICRU_CLKSEL_CON01_CLK_SAI0_SRC_DIV_SHIFT        (8U)
#define PERICRU_CLKSEL_CON01_CLK_SAI0_SRC_DIV_MASK         (0x3FU << PERICRU_CLKSEL_CON01_CLK_SAI0_SRC_DIV_SHIFT)       /* 0x00003F00 */
#define PERICRU_CLKSEL_CON01_CLK_SAI0_SRC_SEL_SHIFT        (14U)
#define PERICRU_CLKSEL_CON01_CLK_SAI0_SRC_SEL_MASK         (0x3U << PERICRU_CLKSEL_CON01_CLK_SAI0_SRC_SEL_SHIFT)        /* 0x0000C000 */
/* CLKSEL_CON02 */
#define PERICRU_CLKSEL_CON02_OFFSET                        (0x108U)
#define PERICRU_CLKSEL_CON02_CLK_SAI0_FRAC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON02_CLK_SAI0_FRAC_DIV_MASK        (0xFFFFFFFFU << PERICRU_CLKSEL_CON02_CLK_SAI0_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON03 */
#define PERICRU_CLKSEL_CON03_OFFSET                        (0x10CU)
#define PERICRU_CLKSEL_CON03_MCLK_SAI0_OUT2IO_SEL_SHIFT    (5U)
#define PERICRU_CLKSEL_CON03_MCLK_SAI0_OUT2IO_SEL_MASK     (0x1U << PERICRU_CLKSEL_CON03_MCLK_SAI0_OUT2IO_SEL_SHIFT)    /* 0x00000020 */
#define PERICRU_CLKSEL_CON03_MCLK_SAI0_SEL_SHIFT           (6U)
#define PERICRU_CLKSEL_CON03_MCLK_SAI0_SEL_MASK            (0x3U << PERICRU_CLKSEL_CON03_MCLK_SAI0_SEL_SHIFT)           /* 0x000000C0 */
#define PERICRU_CLKSEL_CON03_CLK_SAI1_SRC_DIV_SHIFT        (8U)
#define PERICRU_CLKSEL_CON03_CLK_SAI1_SRC_DIV_MASK         (0x3FU << PERICRU_CLKSEL_CON03_CLK_SAI1_SRC_DIV_SHIFT)       /* 0x00003F00 */
#define PERICRU_CLKSEL_CON03_CLK_SAI1_SRC_SEL_SHIFT        (14U)
#define PERICRU_CLKSEL_CON03_CLK_SAI1_SRC_SEL_MASK         (0x3U << PERICRU_CLKSEL_CON03_CLK_SAI1_SRC_SEL_SHIFT)        /* 0x0000C000 */
/* CLKSEL_CON04 */
#define PERICRU_CLKSEL_CON04_OFFSET                        (0x110U)
#define PERICRU_CLKSEL_CON04_CLK_SAI1_FRAC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON04_CLK_SAI1_FRAC_DIV_MASK        (0xFFFFFFFFU << PERICRU_CLKSEL_CON04_CLK_SAI1_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON05 */
#define PERICRU_CLKSEL_CON05_OFFSET                        (0x114U)
#define PERICRU_CLKSEL_CON05_MCLK_SAI1_OUT2IO_SEL_SHIFT    (5U)
#define PERICRU_CLKSEL_CON05_MCLK_SAI1_OUT2IO_SEL_MASK     (0x1U << PERICRU_CLKSEL_CON05_MCLK_SAI1_OUT2IO_SEL_SHIFT)    /* 0x00000020 */
#define PERICRU_CLKSEL_CON05_MCLK_SAI1_SEL_SHIFT           (6U)
#define PERICRU_CLKSEL_CON05_MCLK_SAI1_SEL_MASK            (0x3U << PERICRU_CLKSEL_CON05_MCLK_SAI1_SEL_SHIFT)           /* 0x000000C0 */
/* CLKSEL_CON06 */
#define PERICRU_CLKSEL_CON06_OFFSET                        (0x118U)
#define PERICRU_CLKSEL_CON06_CLK_SAI2_SRC_DIV_SHIFT        (8U)
#define PERICRU_CLKSEL_CON06_CLK_SAI2_SRC_DIV_MASK         (0x3FU << PERICRU_CLKSEL_CON06_CLK_SAI2_SRC_DIV_SHIFT)       /* 0x00003F00 */
#define PERICRU_CLKSEL_CON06_CLK_SAI2_SRC_SEL_SHIFT        (14U)
#define PERICRU_CLKSEL_CON06_CLK_SAI2_SRC_SEL_MASK         (0x3U << PERICRU_CLKSEL_CON06_CLK_SAI2_SRC_SEL_SHIFT)        /* 0x0000C000 */
/* CLKSEL_CON07 */
#define PERICRU_CLKSEL_CON07_OFFSET                        (0x11CU)
#define PERICRU_CLKSEL_CON07_CLK_SAI2_FRAC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON07_CLK_SAI2_FRAC_DIV_MASK        (0xFFFFFFFFU << PERICRU_CLKSEL_CON07_CLK_SAI2_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON08 */
#define PERICRU_CLKSEL_CON08_OFFSET                        (0x120U)
#define PERICRU_CLKSEL_CON08_MCLK_SAI2_OUT2IO_SEL_SHIFT    (5U)
#define PERICRU_CLKSEL_CON08_MCLK_SAI2_OUT2IO_SEL_MASK     (0x1U << PERICRU_CLKSEL_CON08_MCLK_SAI2_OUT2IO_SEL_SHIFT)    /* 0x00000020 */
#define PERICRU_CLKSEL_CON08_MCLK_SAI2_SEL_SHIFT           (6U)
#define PERICRU_CLKSEL_CON08_MCLK_SAI2_SEL_MASK            (0x3U << PERICRU_CLKSEL_CON08_MCLK_SAI2_SEL_SHIFT)           /* 0x000000C0 */
/* CLKSEL_CON12 */
#define PERICRU_CLKSEL_CON12_OFFSET                        (0x130U)
#define PERICRU_CLKSEL_CON12_MCLK_PDM_DIV_SHIFT            (0U)
#define PERICRU_CLKSEL_CON12_MCLK_PDM_DIV_MASK             (0x1FU << PERICRU_CLKSEL_CON12_MCLK_PDM_DIV_SHIFT)           /* 0x0000001F */
#define PERICRU_CLKSEL_CON12_MCLK_PDM_SEL_SHIFT            (6U)
#define PERICRU_CLKSEL_CON12_MCLK_PDM_SEL_MASK             (0x3U << PERICRU_CLKSEL_CON12_MCLK_PDM_SEL_SHIFT)            /* 0x000000C0 */
/* CLKSEL_CON13 */
#define PERICRU_CLKSEL_CON13_OFFSET                        (0x134U)
#define PERICRU_CLKSEL_CON13_CLK_SPDIF_SRC_DIV_SHIFT       (8U)
#define PERICRU_CLKSEL_CON13_CLK_SPDIF_SRC_DIV_MASK        (0x3FU << PERICRU_CLKSEL_CON13_CLK_SPDIF_SRC_DIV_SHIFT)      /* 0x00003F00 */
#define PERICRU_CLKSEL_CON13_CLK_SPDIF_SRC_SEL_SHIFT       (14U)
#define PERICRU_CLKSEL_CON13_CLK_SPDIF_SRC_SEL_MASK        (0x3U << PERICRU_CLKSEL_CON13_CLK_SPDIF_SRC_SEL_SHIFT)       /* 0x0000C000 */
/* CLKSEL_CON14 */
#define PERICRU_CLKSEL_CON14_OFFSET                        (0x138U)
#define PERICRU_CLKSEL_CON14_CLK_SPDIF_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON14_CLK_SPDIF_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON14_CLK_SPDIF_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON15 */
#define PERICRU_CLKSEL_CON15_OFFSET                        (0x13CU)
#define PERICRU_CLKSEL_CON15_MCLK_SPDIF_SEL_SHIFT          (6U)
#define PERICRU_CLKSEL_CON15_MCLK_SPDIF_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON15_MCLK_SPDIF_SEL_SHIFT)          /* 0x000000C0 */
/* CLKSEL_CON16 */
#define PERICRU_CLKSEL_CON16_OFFSET                        (0x140U)
#define PERICRU_CLKSEL_CON16_CCLK_SDMMC0_DIV_SHIFT         (0U)
#define PERICRU_CLKSEL_CON16_CCLK_SDMMC0_DIV_MASK          (0xFFU << PERICRU_CLKSEL_CON16_CCLK_SDMMC0_DIV_SHIFT)        /* 0x000000FF */
#define PERICRU_CLKSEL_CON16_CCLK_SDMMC0_SEL_SHIFT         (14U)
#define PERICRU_CLKSEL_CON16_CCLK_SDMMC0_SEL_MASK          (0x3U << PERICRU_CLKSEL_CON16_CCLK_SDMMC0_SEL_SHIFT)         /* 0x0000C000 */
/* CLKSEL_CON17 */
#define PERICRU_CLKSEL_CON17_OFFSET                        (0x144U)
#define PERICRU_CLKSEL_CON17_CCLK_SDMMC1_DIV_SHIFT         (0U)
#define PERICRU_CLKSEL_CON17_CCLK_SDMMC1_DIV_MASK          (0xFFU << PERICRU_CLKSEL_CON17_CCLK_SDMMC1_DIV_SHIFT)        /* 0x000000FF */
#define PERICRU_CLKSEL_CON17_CCLK_SDMMC1_SEL_SHIFT         (14U)
#define PERICRU_CLKSEL_CON17_CCLK_SDMMC1_SEL_MASK          (0x3U << PERICRU_CLKSEL_CON17_CCLK_SDMMC1_SEL_SHIFT)         /* 0x0000C000 */
/* CLKSEL_CON18 */
#define PERICRU_CLKSEL_CON18_OFFSET                        (0x148U)
#define PERICRU_CLKSEL_CON18_CCLK_EMMC_DIV_SHIFT           (0U)
#define PERICRU_CLKSEL_CON18_CCLK_EMMC_DIV_MASK            (0xFFU << PERICRU_CLKSEL_CON18_CCLK_EMMC_DIV_SHIFT)          /* 0x000000FF */
#define PERICRU_CLKSEL_CON18_CCLK_EMMC_SEL_SHIFT           (14U)
#define PERICRU_CLKSEL_CON18_CCLK_EMMC_SEL_MASK            (0x3U << PERICRU_CLKSEL_CON18_CCLK_EMMC_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON19 */
#define PERICRU_CLKSEL_CON19_OFFSET                        (0x14CU)
#define PERICRU_CLKSEL_CON19_BCLK_EMMC_DIV_SHIFT           (8U)
#define PERICRU_CLKSEL_CON19_BCLK_EMMC_DIV_MASK            (0x7FU << PERICRU_CLKSEL_CON19_BCLK_EMMC_DIV_SHIFT)          /* 0x00007F00 */
#define PERICRU_CLKSEL_CON19_BCLK_EMMC_SEL_SHIFT           (15U)
#define PERICRU_CLKSEL_CON19_BCLK_EMMC_SEL_MASK            (0x1U << PERICRU_CLKSEL_CON19_BCLK_EMMC_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL_CON20 */
#define PERICRU_CLKSEL_CON20_OFFSET                        (0x150U)
#define PERICRU_CLKSEL_CON20_SCLK_SFC_DIV_SHIFT            (0U)
#define PERICRU_CLKSEL_CON20_SCLK_SFC_DIV_MASK             (0xFFU << PERICRU_CLKSEL_CON20_SCLK_SFC_DIV_SHIFT)           /* 0x000000FF */
#define PERICRU_CLKSEL_CON20_SCLK_SFC_SEL_SHIFT            (8U)
#define PERICRU_CLKSEL_CON20_SCLK_SFC_SEL_MASK             (0x3U << PERICRU_CLKSEL_CON20_SCLK_SFC_SEL_SHIFT)            /* 0x00000300 */
#define PERICRU_CLKSEL_CON20_CLK_SPI1_SEL_SHIFT            (12U)
#define PERICRU_CLKSEL_CON20_CLK_SPI1_SEL_MASK             (0x3U << PERICRU_CLKSEL_CON20_CLK_SPI1_SEL_SHIFT)            /* 0x00003000 */
#define PERICRU_CLKSEL_CON20_CLK_SPI2_SEL_SHIFT            (14U)
#define PERICRU_CLKSEL_CON20_CLK_SPI2_SEL_MASK             (0x3U << PERICRU_CLKSEL_CON20_CLK_SPI2_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON21 */
#define PERICRU_CLKSEL_CON21_OFFSET                        (0x154U)
#define PERICRU_CLKSEL_CON21_CLK_UART1_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON21_CLK_UART1_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON21_CLK_UART1_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON21_CLK_UART1_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON21_CLK_UART1_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON21_CLK_UART1_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON21_SCLK_UART1_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON21_SCLK_UART1_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON21_SCLK_UART1_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON22 */
#define PERICRU_CLKSEL_CON22_OFFSET                        (0x158U)
#define PERICRU_CLKSEL_CON22_CLK_UART1_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON22_CLK_UART1_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON22_CLK_UART1_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON23 */
#define PERICRU_CLKSEL_CON23_OFFSET                        (0x15CU)
#define PERICRU_CLKSEL_CON23_CLK_UART2_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON23_CLK_UART2_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON23_CLK_UART2_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON23_CLK_UART2_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON23_CLK_UART2_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON23_CLK_UART2_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON23_SCLK_UART2_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON23_SCLK_UART2_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON23_SCLK_UART2_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON24 */
#define PERICRU_CLKSEL_CON24_OFFSET                        (0x160U)
#define PERICRU_CLKSEL_CON24_CLK_UART2_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON24_CLK_UART2_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON24_CLK_UART2_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON25 */
#define PERICRU_CLKSEL_CON25_OFFSET                        (0x164U)
#define PERICRU_CLKSEL_CON25_CLK_UART3_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON25_CLK_UART3_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON25_CLK_UART3_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON25_CLK_UART3_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON25_CLK_UART3_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON25_CLK_UART3_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON25_SCLK_UART3_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON25_SCLK_UART3_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON25_SCLK_UART3_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON26 */
#define PERICRU_CLKSEL_CON26_OFFSET                        (0x168U)
#define PERICRU_CLKSEL_CON26_CLK_UART3_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON26_CLK_UART3_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON26_CLK_UART3_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON27 */
#define PERICRU_CLKSEL_CON27_OFFSET                        (0x16CU)
#define PERICRU_CLKSEL_CON27_CLK_UART4_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON27_CLK_UART4_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON27_CLK_UART4_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON27_CLK_UART4_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON27_CLK_UART4_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON27_CLK_UART4_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON27_SCLK_UART4_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON27_SCLK_UART4_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON27_SCLK_UART4_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON28 */
#define PERICRU_CLKSEL_CON28_OFFSET                        (0x170U)
#define PERICRU_CLKSEL_CON28_CLK_UART4_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON28_CLK_UART4_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON28_CLK_UART4_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON29 */
#define PERICRU_CLKSEL_CON29_OFFSET                        (0x174U)
#define PERICRU_CLKSEL_CON29_CLK_UART5_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON29_CLK_UART5_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON29_CLK_UART5_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON29_CLK_UART5_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON29_CLK_UART5_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON29_CLK_UART5_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON29_SCLK_UART5_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON29_SCLK_UART5_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON29_SCLK_UART5_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON30 */
#define PERICRU_CLKSEL_CON30_OFFSET                        (0x178U)
#define PERICRU_CLKSEL_CON30_CLK_UART5_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON30_CLK_UART5_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON30_CLK_UART5_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON31 */
#define PERICRU_CLKSEL_CON31_OFFSET                        (0x17CU)
#define PERICRU_CLKSEL_CON31_CLK_UART6_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON31_CLK_UART6_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON31_CLK_UART6_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON31_CLK_UART6_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON31_CLK_UART6_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON31_CLK_UART6_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON31_SCLK_UART6_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON31_SCLK_UART6_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON31_SCLK_UART6_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON32 */
#define PERICRU_CLKSEL_CON32_OFFSET                        (0x180U)
#define PERICRU_CLKSEL_CON32_CLK_UART6_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON32_CLK_UART6_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON32_CLK_UART6_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON33 */
#define PERICRU_CLKSEL_CON33_OFFSET                        (0x184U)
#define PERICRU_CLKSEL_CON33_CLK_UART7_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON33_CLK_UART7_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON33_CLK_UART7_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON33_CLK_UART7_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON33_CLK_UART7_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON33_CLK_UART7_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON33_SCLK_UART7_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON33_SCLK_UART7_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON33_SCLK_UART7_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON34 */
#define PERICRU_CLKSEL_CON34_OFFSET                        (0x188U)
#define PERICRU_CLKSEL_CON34_CLK_UART7_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON34_CLK_UART7_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON34_CLK_UART7_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON35 */
#define PERICRU_CLKSEL_CON35_OFFSET                        (0x18CU)
#define PERICRU_CLKSEL_CON35_CLK_UART8_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON35_CLK_UART8_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON35_CLK_UART8_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON35_CLK_UART8_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON35_CLK_UART8_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON35_CLK_UART8_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON35_SCLK_UART8_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON35_SCLK_UART8_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON35_SCLK_UART8_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON36 */
#define PERICRU_CLKSEL_CON36_OFFSET                        (0x190U)
#define PERICRU_CLKSEL_CON36_CLK_UART8_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON36_CLK_UART8_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON36_CLK_UART8_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON37 */
#define PERICRU_CLKSEL_CON37_OFFSET                        (0x194U)
#define PERICRU_CLKSEL_CON37_CLK_UART9_SRC_DIV_SHIFT       (0U)
#define PERICRU_CLKSEL_CON37_CLK_UART9_SRC_DIV_MASK        (0x7FU << PERICRU_CLKSEL_CON37_CLK_UART9_SRC_DIV_SHIFT)      /* 0x0000007F */
#define PERICRU_CLKSEL_CON37_CLK_UART9_SRC_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON37_CLK_UART9_SRC_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON37_CLK_UART9_SRC_SEL_SHIFT)       /* 0x00000100 */
#define PERICRU_CLKSEL_CON37_SCLK_UART9_SEL_SHIFT          (14U)
#define PERICRU_CLKSEL_CON37_SCLK_UART9_SEL_MASK           (0x3U << PERICRU_CLKSEL_CON37_SCLK_UART9_SEL_SHIFT)          /* 0x0000C000 */
/* CLKSEL_CON38 */
#define PERICRU_CLKSEL_CON38_OFFSET                        (0x198U)
#define PERICRU_CLKSEL_CON38_CLK_UART9_FRAC_DIV_SHIFT      (0U)
#define PERICRU_CLKSEL_CON38_CLK_UART9_FRAC_DIV_MASK       (0xFFFFFFFFU << PERICRU_CLKSEL_CON38_CLK_UART9_FRAC_DIV_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON40 */
#define PERICRU_CLKSEL_CON40_OFFSET                        (0x1A0U)
#define PERICRU_CLKSEL_CON40_CLK_PWM1_PERI_SEL_SHIFT       (0U)
#define PERICRU_CLKSEL_CON40_CLK_PWM1_PERI_SEL_MASK        (0x3U << PERICRU_CLKSEL_CON40_CLK_PWM1_PERI_SEL_SHIFT)       /* 0x00000003 */
#define PERICRU_CLKSEL_CON40_CLK_PWM2_PERI_SEL_SHIFT       (6U)
#define PERICRU_CLKSEL_CON40_CLK_PWM2_PERI_SEL_MASK        (0x3U << PERICRU_CLKSEL_CON40_CLK_PWM2_PERI_SEL_SHIFT)       /* 0x000000C0 */
#define PERICRU_CLKSEL_CON40_CLK_PWM3_PERI_SEL_SHIFT       (8U)
#define PERICRU_CLKSEL_CON40_CLK_PWM3_PERI_SEL_MASK        (0x3U << PERICRU_CLKSEL_CON40_CLK_PWM3_PERI_SEL_SHIFT)       /* 0x00000300 */
/* CLKSEL_CON43 */
#define PERICRU_CLKSEL_CON43_OFFSET                        (0x1ACU)
#define PERICRU_CLKSEL_CON43_CLK_CORE_CRYPTO_SEL_SHIFT     (0U)
#define PERICRU_CLKSEL_CON43_CLK_CORE_CRYPTO_SEL_MASK      (0x3U << PERICRU_CLKSEL_CON43_CLK_CORE_CRYPTO_SEL_SHIFT)     /* 0x00000003 */
#define PERICRU_CLKSEL_CON43_CLK_PKA_CRYPTO_SEL_SHIFT      (6U)
#define PERICRU_CLKSEL_CON43_CLK_PKA_CRYPTO_SEL_MASK       (0x3U << PERICRU_CLKSEL_CON43_CLK_PKA_CRYPTO_SEL_SHIFT)      /* 0x000000C0 */
#define PERICRU_CLKSEL_CON43_TCLK_PERI_WDT_SEL_SHIFT       (15U)
#define PERICRU_CLKSEL_CON43_TCLK_PERI_WDT_SEL_MASK        (0x1U << PERICRU_CLKSEL_CON43_TCLK_PERI_WDT_SEL_SHIFT)       /* 0x00008000 */
/* CLKSEL_CON44 */
#define PERICRU_CLKSEL_CON44_OFFSET                        (0x1B0U)
#define PERICRU_CLKSEL_CON44_CLK_USER_OTPC_NS_DIV_SHIFT    (0U)
#define PERICRU_CLKSEL_CON44_CLK_USER_OTPC_NS_DIV_MASK     (0xFFU << PERICRU_CLKSEL_CON44_CLK_USER_OTPC_NS_DIV_SHIFT)   /* 0x000000FF */
#define PERICRU_CLKSEL_CON44_CLK_USER_OTPC_S_DIV_SHIFT     (8U)
#define PERICRU_CLKSEL_CON44_CLK_USER_OTPC_S_DIV_MASK      (0xFFU << PERICRU_CLKSEL_CON44_CLK_USER_OTPC_S_DIV_SHIFT)    /* 0x0000FF00 */
/* CLKSEL_CON46 */
#define PERICRU_CLKSEL_CON46_OFFSET                        (0x1B8U)
#define PERICRU_CLKSEL_CON46_CLK_SARADC_DIV_SHIFT          (0U)
#define PERICRU_CLKSEL_CON46_CLK_SARADC_DIV_MASK           (0xFFFU << PERICRU_CLKSEL_CON46_CLK_SARADC_DIV_SHIFT)        /* 0x00000FFF */
/* CLKSEL_CON47 */
#define PERICRU_CLKSEL_CON47_OFFSET                        (0x1BCU)
#define PERICRU_CLKSEL_CON47_DCLK_PERI_GPIO_SEL_SHIFT      (8U)
#define PERICRU_CLKSEL_CON47_DCLK_PERI_GPIO_SEL_MASK       (0x1U << PERICRU_CLKSEL_CON47_DCLK_PERI_GPIO_SEL_SHIFT)      /* 0x00000100 */
#define PERICRU_CLKSEL_CON47_CLK_TESTOUT_PERI_SEL_SHIFT    (11U)
#define PERICRU_CLKSEL_CON47_CLK_TESTOUT_PERI_SEL_MASK     (0x1FU << PERICRU_CLKSEL_CON47_CLK_TESTOUT_PERI_SEL_SHIFT)   /* 0x0000F800 */
/* GATE_CON00 */
#define PERICRU_GATE_CON00_OFFSET                          (0x300U)
/* GATE_CON01 */
#define PERICRU_GATE_CON01_OFFSET                          (0x304U)
#define PERICRU_GATE_CON01_ACLK_PERI_EN_SHIFT              (0U)
#define PERICRU_GATE_CON01_ACLK_PERI_EN_MASK               (0x1U << PERICRU_GATE_CON01_ACLK_PERI_EN_SHIFT)              /* 0x00000001 */
#define PERICRU_GATE_CON01_HCLK_PERI_EN_SHIFT              (1U)
#define PERICRU_GATE_CON01_HCLK_PERI_EN_MASK               (0x1U << PERICRU_GATE_CON01_HCLK_PERI_EN_SHIFT)              /* 0x00000002 */
#define PERICRU_GATE_CON01_PCLK_PERI_EN_SHIFT              (2U)
#define PERICRU_GATE_CON01_PCLK_PERI_EN_MASK               (0x1U << PERICRU_GATE_CON01_PCLK_PERI_EN_SHIFT)              /* 0x00000004 */
#define PERICRU_GATE_CON01_ACLK_PERI_BIU_EN_SHIFT          (3U)
#define PERICRU_GATE_CON01_ACLK_PERI_BIU_EN_MASK           (0x1U << PERICRU_GATE_CON01_ACLK_PERI_BIU_EN_SHIFT)          /* 0x00000008 */
#define PERICRU_GATE_CON01_HCLK_PERI_BIU_EN_SHIFT          (4U)
#define PERICRU_GATE_CON01_HCLK_PERI_BIU_EN_MASK           (0x1U << PERICRU_GATE_CON01_HCLK_PERI_BIU_EN_SHIFT)          /* 0x00000010 */
#define PERICRU_GATE_CON01_PCLK_PERI_BIU_EN_SHIFT          (5U)
#define PERICRU_GATE_CON01_PCLK_PERI_BIU_EN_MASK           (0x1U << PERICRU_GATE_CON01_PCLK_PERI_BIU_EN_SHIFT)          /* 0x00000020 */
#define PERICRU_GATE_CON01_PCLK_PERICRU_EN_SHIFT           (6U)
#define PERICRU_GATE_CON01_PCLK_PERICRU_EN_MASK            (0x1U << PERICRU_GATE_CON01_PCLK_PERICRU_EN_SHIFT)           /* 0x00000040 */
/* GATE_CON02 */
#define PERICRU_GATE_CON02_OFFSET                          (0x308U)
#define PERICRU_GATE_CON02_HCLK_SAI0_EN_SHIFT              (0U)
#define PERICRU_GATE_CON02_HCLK_SAI0_EN_MASK               (0x1U << PERICRU_GATE_CON02_HCLK_SAI0_EN_SHIFT)              /* 0x00000001 */
#define PERICRU_GATE_CON02_CLK_SAI0_EN_SHIFT               (1U)
#define PERICRU_GATE_CON02_CLK_SAI0_EN_MASK                (0x1U << PERICRU_GATE_CON02_CLK_SAI0_EN_SHIFT)               /* 0x00000002 */
#define PERICRU_GATE_CON02_CLK_SAI0_FRAC_EN_SHIFT          (2U)
#define PERICRU_GATE_CON02_CLK_SAI0_FRAC_EN_MASK           (0x1U << PERICRU_GATE_CON02_CLK_SAI0_FRAC_EN_SHIFT)          /* 0x00000004 */
#define PERICRU_GATE_CON02_MCLK_SAI0_EN_SHIFT              (3U)
#define PERICRU_GATE_CON02_MCLK_SAI0_EN_MASK               (0x1U << PERICRU_GATE_CON02_MCLK_SAI0_EN_SHIFT)              /* 0x00000008 */
#define PERICRU_GATE_CON02_MCLK_SAI0_OUT2IO_EN_SHIFT       (4U)
#define PERICRU_GATE_CON02_MCLK_SAI0_OUT2IO_EN_MASK        (0x1U << PERICRU_GATE_CON02_MCLK_SAI0_OUT2IO_EN_SHIFT)       /* 0x00000010 */
#define PERICRU_GATE_CON02_HCLK_SAI1_EN_SHIFT              (5U)
#define PERICRU_GATE_CON02_HCLK_SAI1_EN_MASK               (0x1U << PERICRU_GATE_CON02_HCLK_SAI1_EN_SHIFT)              /* 0x00000020 */
#define PERICRU_GATE_CON02_CLK_SAI1_EN_SHIFT               (6U)
#define PERICRU_GATE_CON02_CLK_SAI1_EN_MASK                (0x1U << PERICRU_GATE_CON02_CLK_SAI1_EN_SHIFT)               /* 0x00000040 */
#define PERICRU_GATE_CON02_CLK_SAI1_FRAC_EN_SHIFT          (7U)
#define PERICRU_GATE_CON02_CLK_SAI1_FRAC_EN_MASK           (0x1U << PERICRU_GATE_CON02_CLK_SAI1_FRAC_EN_SHIFT)          /* 0x00000080 */
#define PERICRU_GATE_CON02_MCLK_SAI1_EN_SHIFT              (8U)
#define PERICRU_GATE_CON02_MCLK_SAI1_EN_MASK               (0x1U << PERICRU_GATE_CON02_MCLK_SAI1_EN_SHIFT)              /* 0x00000100 */
#define PERICRU_GATE_CON02_MCLK_SAI1_OUT2IO_EN_SHIFT       (9U)
#define PERICRU_GATE_CON02_MCLK_SAI1_OUT2IO_EN_MASK        (0x1U << PERICRU_GATE_CON02_MCLK_SAI1_OUT2IO_EN_SHIFT)       /* 0x00000200 */
#define PERICRU_GATE_CON02_HCLK_SAI2_EN_SHIFT              (10U)
#define PERICRU_GATE_CON02_HCLK_SAI2_EN_MASK               (0x1U << PERICRU_GATE_CON02_HCLK_SAI2_EN_SHIFT)              /* 0x00000400 */
#define PERICRU_GATE_CON02_CLK_SAI2_EN_SHIFT               (11U)
#define PERICRU_GATE_CON02_CLK_SAI2_EN_MASK                (0x1U << PERICRU_GATE_CON02_CLK_SAI2_EN_SHIFT)               /* 0x00000800 */
#define PERICRU_GATE_CON02_CLK_SAI2_FRAC_EN_SHIFT          (12U)
#define PERICRU_GATE_CON02_CLK_SAI2_FRAC_EN_MASK           (0x1U << PERICRU_GATE_CON02_CLK_SAI2_FRAC_EN_SHIFT)          /* 0x00001000 */
#define PERICRU_GATE_CON02_MCLK_SAI2_EN_SHIFT              (13U)
#define PERICRU_GATE_CON02_MCLK_SAI2_EN_MASK               (0x1U << PERICRU_GATE_CON02_MCLK_SAI2_EN_SHIFT)              /* 0x00002000 */
#define PERICRU_GATE_CON02_MCLK_SAI2_OUT2IO_EN_SHIFT       (14U)
#define PERICRU_GATE_CON02_MCLK_SAI2_OUT2IO_EN_MASK        (0x1U << PERICRU_GATE_CON02_MCLK_SAI2_OUT2IO_EN_SHIFT)       /* 0x00004000 */
/* GATE_CON03 */
#define PERICRU_GATE_CON03_OFFSET                          (0x30CU)
#define PERICRU_GATE_CON03_HCLK_DSM_EN_SHIFT               (1U)
#define PERICRU_GATE_CON03_HCLK_DSM_EN_MASK                (0x1U << PERICRU_GATE_CON03_HCLK_DSM_EN_SHIFT)               /* 0x00000002 */
#define PERICRU_GATE_CON03_CLK_DSM_EN_SHIFT                (2U)
#define PERICRU_GATE_CON03_CLK_DSM_EN_MASK                 (0x1U << PERICRU_GATE_CON03_CLK_DSM_EN_SHIFT)                /* 0x00000004 */
#define PERICRU_GATE_CON03_HCLK_PDM_EN_SHIFT               (4U)
#define PERICRU_GATE_CON03_HCLK_PDM_EN_MASK                (0x1U << PERICRU_GATE_CON03_HCLK_PDM_EN_SHIFT)               /* 0x00000010 */
#define PERICRU_GATE_CON03_MCLK_PDM_EN_SHIFT               (5U)
#define PERICRU_GATE_CON03_MCLK_PDM_EN_MASK                (0x1U << PERICRU_GATE_CON03_MCLK_PDM_EN_SHIFT)               /* 0x00000020 */
#define PERICRU_GATE_CON03_HCLK_SPDIF_EN_SHIFT             (8U)
#define PERICRU_GATE_CON03_HCLK_SPDIF_EN_MASK              (0x1U << PERICRU_GATE_CON03_HCLK_SPDIF_EN_SHIFT)             /* 0x00000100 */
#define PERICRU_GATE_CON03_CLK_SPDIF_EN_SHIFT              (9U)
#define PERICRU_GATE_CON03_CLK_SPDIF_EN_MASK               (0x1U << PERICRU_GATE_CON03_CLK_SPDIF_EN_SHIFT)              /* 0x00000200 */
#define PERICRU_GATE_CON03_CLK_SPDIF_FRAC_EN_SHIFT         (10U)
#define PERICRU_GATE_CON03_CLK_SPDIF_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON03_CLK_SPDIF_FRAC_EN_SHIFT)         /* 0x00000400 */
#define PERICRU_GATE_CON03_MCLK_SPDIF_EN_SHIFT             (11U)
#define PERICRU_GATE_CON03_MCLK_SPDIF_EN_MASK              (0x1U << PERICRU_GATE_CON03_MCLK_SPDIF_EN_SHIFT)             /* 0x00000800 */
/* GATE_CON04 */
#define PERICRU_GATE_CON04_OFFSET                          (0x310U)
#define PERICRU_GATE_CON04_HCLK_SDMMC0_EN_SHIFT            (0U)
#define PERICRU_GATE_CON04_HCLK_SDMMC0_EN_MASK             (0x1U << PERICRU_GATE_CON04_HCLK_SDMMC0_EN_SHIFT)            /* 0x00000001 */
#define PERICRU_GATE_CON04_CCLK_SDMMC0_EN_SHIFT            (1U)
#define PERICRU_GATE_CON04_CCLK_SDMMC0_EN_MASK             (0x1U << PERICRU_GATE_CON04_CCLK_SDMMC0_EN_SHIFT)            /* 0x00000002 */
#define PERICRU_GATE_CON04_HCLK_SDMMC1_EN_SHIFT            (2U)
#define PERICRU_GATE_CON04_HCLK_SDMMC1_EN_MASK             (0x1U << PERICRU_GATE_CON04_HCLK_SDMMC1_EN_SHIFT)            /* 0x00000004 */
#define PERICRU_GATE_CON04_CCLK_SDMMC1_EN_SHIFT            (3U)
#define PERICRU_GATE_CON04_CCLK_SDMMC1_EN_MASK             (0x1U << PERICRU_GATE_CON04_CCLK_SDMMC1_EN_SHIFT)            /* 0x00000008 */
#define PERICRU_GATE_CON04_HCLK_EMMC_EN_SHIFT              (8U)
#define PERICRU_GATE_CON04_HCLK_EMMC_EN_MASK               (0x1U << PERICRU_GATE_CON04_HCLK_EMMC_EN_SHIFT)              /* 0x00000100 */
#define PERICRU_GATE_CON04_ACLK_EMMC_EN_SHIFT              (9U)
#define PERICRU_GATE_CON04_ACLK_EMMC_EN_MASK               (0x1U << PERICRU_GATE_CON04_ACLK_EMMC_EN_SHIFT)              /* 0x00000200 */
#define PERICRU_GATE_CON04_CCLK_EMMC_EN_SHIFT              (10U)
#define PERICRU_GATE_CON04_CCLK_EMMC_EN_MASK               (0x1U << PERICRU_GATE_CON04_CCLK_EMMC_EN_SHIFT)              /* 0x00000400 */
#define PERICRU_GATE_CON04_BCLK_EMMC_EN_SHIFT              (11U)
#define PERICRU_GATE_CON04_BCLK_EMMC_EN_MASK               (0x1U << PERICRU_GATE_CON04_BCLK_EMMC_EN_SHIFT)              /* 0x00000800 */
#define PERICRU_GATE_CON04_TMCLK_EMMC_EN_SHIFT             (12U)
#define PERICRU_GATE_CON04_TMCLK_EMMC_EN_MASK              (0x1U << PERICRU_GATE_CON04_TMCLK_EMMC_EN_SHIFT)             /* 0x00001000 */
#define PERICRU_GATE_CON04_SCLK_SFC_EN_SHIFT               (13U)
#define PERICRU_GATE_CON04_SCLK_SFC_EN_MASK                (0x1U << PERICRU_GATE_CON04_SCLK_SFC_EN_SHIFT)               /* 0x00002000 */
#define PERICRU_GATE_CON04_HCLK_SFC_EN_SHIFT               (14U)
#define PERICRU_GATE_CON04_HCLK_SFC_EN_MASK                (0x1U << PERICRU_GATE_CON04_HCLK_SFC_EN_SHIFT)               /* 0x00004000 */
/* GATE_CON05 */
#define PERICRU_GATE_CON05_OFFSET                          (0x314U)
#define PERICRU_GATE_CON05_HCLK_USB2HOST_EN_SHIFT          (0U)
#define PERICRU_GATE_CON05_HCLK_USB2HOST_EN_MASK           (0x1U << PERICRU_GATE_CON05_HCLK_USB2HOST_EN_SHIFT)          /* 0x00000001 */
#define PERICRU_GATE_CON05_HCLK_USB2HOST_ARB_EN_SHIFT      (1U)
#define PERICRU_GATE_CON05_HCLK_USB2HOST_ARB_EN_MASK       (0x1U << PERICRU_GATE_CON05_HCLK_USB2HOST_ARB_EN_SHIFT)      /* 0x00000002 */
/* GATE_CON06 */
#define PERICRU_GATE_CON06_OFFSET                          (0x318U)
#define PERICRU_GATE_CON06_PCLK_SPI1_EN_SHIFT              (0U)
#define PERICRU_GATE_CON06_PCLK_SPI1_EN_MASK               (0x1U << PERICRU_GATE_CON06_PCLK_SPI1_EN_SHIFT)              /* 0x00000001 */
#define PERICRU_GATE_CON06_CLK_SPI1_EN_SHIFT               (1U)
#define PERICRU_GATE_CON06_CLK_SPI1_EN_MASK                (0x1U << PERICRU_GATE_CON06_CLK_SPI1_EN_SHIFT)               /* 0x00000002 */
#define PERICRU_GATE_CON06_SCLK_IN_SPI1_EN_SHIFT           (2U)
#define PERICRU_GATE_CON06_SCLK_IN_SPI1_EN_MASK            (0x1U << PERICRU_GATE_CON06_SCLK_IN_SPI1_EN_SHIFT)           /* 0x00000004 */
#define PERICRU_GATE_CON06_PCLK_SPI2_EN_SHIFT              (3U)
#define PERICRU_GATE_CON06_PCLK_SPI2_EN_MASK               (0x1U << PERICRU_GATE_CON06_PCLK_SPI2_EN_SHIFT)              /* 0x00000008 */
#define PERICRU_GATE_CON06_CLK_SPI2_EN_SHIFT               (4U)
#define PERICRU_GATE_CON06_CLK_SPI2_EN_MASK                (0x1U << PERICRU_GATE_CON06_CLK_SPI2_EN_SHIFT)               /* 0x00000010 */
#define PERICRU_GATE_CON06_SCLK_IN_SPI2_EN_SHIFT           (5U)
#define PERICRU_GATE_CON06_SCLK_IN_SPI2_EN_MASK            (0x1U << PERICRU_GATE_CON06_SCLK_IN_SPI2_EN_SHIFT)           /* 0x00000020 */
/* GATE_CON07 */
#define PERICRU_GATE_CON07_OFFSET                          (0x31CU)
#define PERICRU_GATE_CON07_PCLK_UART1_EN_SHIFT             (0U)
#define PERICRU_GATE_CON07_PCLK_UART1_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART1_EN_SHIFT)             /* 0x00000001 */
#define PERICRU_GATE_CON07_PCLK_UART2_EN_SHIFT             (1U)
#define PERICRU_GATE_CON07_PCLK_UART2_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART2_EN_SHIFT)             /* 0x00000002 */
#define PERICRU_GATE_CON07_PCLK_UART3_EN_SHIFT             (2U)
#define PERICRU_GATE_CON07_PCLK_UART3_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART3_EN_SHIFT)             /* 0x00000004 */
#define PERICRU_GATE_CON07_PCLK_UART4_EN_SHIFT             (3U)
#define PERICRU_GATE_CON07_PCLK_UART4_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART4_EN_SHIFT)             /* 0x00000008 */
#define PERICRU_GATE_CON07_PCLK_UART5_EN_SHIFT             (4U)
#define PERICRU_GATE_CON07_PCLK_UART5_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART5_EN_SHIFT)             /* 0x00000010 */
#define PERICRU_GATE_CON07_PCLK_UART6_EN_SHIFT             (5U)
#define PERICRU_GATE_CON07_PCLK_UART6_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART6_EN_SHIFT)             /* 0x00000020 */
#define PERICRU_GATE_CON07_PCLK_UART7_EN_SHIFT             (6U)
#define PERICRU_GATE_CON07_PCLK_UART7_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART7_EN_SHIFT)             /* 0x00000040 */
#define PERICRU_GATE_CON07_PCLK_UART8_EN_SHIFT             (7U)
#define PERICRU_GATE_CON07_PCLK_UART8_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART8_EN_SHIFT)             /* 0x00000080 */
#define PERICRU_GATE_CON07_PCLK_UART9_EN_SHIFT             (8U)
#define PERICRU_GATE_CON07_PCLK_UART9_EN_MASK              (0x1U << PERICRU_GATE_CON07_PCLK_UART9_EN_SHIFT)             /* 0x00000100 */
#define PERICRU_GATE_CON07_CLK_UART1_EN_SHIFT              (9U)
#define PERICRU_GATE_CON07_CLK_UART1_EN_MASK               (0x1U << PERICRU_GATE_CON07_CLK_UART1_EN_SHIFT)              /* 0x00000200 */
#define PERICRU_GATE_CON07_CLK_UART1_FRAC_EN_SHIFT         (10U)
#define PERICRU_GATE_CON07_CLK_UART1_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON07_CLK_UART1_FRAC_EN_SHIFT)         /* 0x00000400 */
#define PERICRU_GATE_CON07_SCLK_UART1_EN_SHIFT             (11U)
#define PERICRU_GATE_CON07_SCLK_UART1_EN_MASK              (0x1U << PERICRU_GATE_CON07_SCLK_UART1_EN_SHIFT)             /* 0x00000800 */
#define PERICRU_GATE_CON07_CLK_UART2_EN_SHIFT              (12U)
#define PERICRU_GATE_CON07_CLK_UART2_EN_MASK               (0x1U << PERICRU_GATE_CON07_CLK_UART2_EN_SHIFT)              /* 0x00001000 */
#define PERICRU_GATE_CON07_CLK_UART2_FRAC_EN_SHIFT         (13U)
#define PERICRU_GATE_CON07_CLK_UART2_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON07_CLK_UART2_FRAC_EN_SHIFT)         /* 0x00002000 */
#define PERICRU_GATE_CON07_SCLK_UART2_EN_SHIFT             (14U)
#define PERICRU_GATE_CON07_SCLK_UART2_EN_MASK              (0x1U << PERICRU_GATE_CON07_SCLK_UART2_EN_SHIFT)             /* 0x00004000 */
#define PERICRU_GATE_CON07_CLK_UART3_EN_SHIFT              (15U)
#define PERICRU_GATE_CON07_CLK_UART3_EN_MASK               (0x1U << PERICRU_GATE_CON07_CLK_UART3_EN_SHIFT)              /* 0x00008000 */
/* GATE_CON08 */
#define PERICRU_GATE_CON08_OFFSET                          (0x320U)
#define PERICRU_GATE_CON08_CLK_UART3_FRAC_EN_SHIFT         (0U)
#define PERICRU_GATE_CON08_CLK_UART3_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON08_CLK_UART3_FRAC_EN_SHIFT)         /* 0x00000001 */
#define PERICRU_GATE_CON08_SCLK_UART3_EN_SHIFT             (1U)
#define PERICRU_GATE_CON08_SCLK_UART3_EN_MASK              (0x1U << PERICRU_GATE_CON08_SCLK_UART3_EN_SHIFT)             /* 0x00000002 */
#define PERICRU_GATE_CON08_CLK_UART4_EN_SHIFT              (2U)
#define PERICRU_GATE_CON08_CLK_UART4_EN_MASK               (0x1U << PERICRU_GATE_CON08_CLK_UART4_EN_SHIFT)              /* 0x00000004 */
#define PERICRU_GATE_CON08_CLK_UART4_FRAC_EN_SHIFT         (3U)
#define PERICRU_GATE_CON08_CLK_UART4_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON08_CLK_UART4_FRAC_EN_SHIFT)         /* 0x00000008 */
#define PERICRU_GATE_CON08_SCLK_UART4_EN_SHIFT             (4U)
#define PERICRU_GATE_CON08_SCLK_UART4_EN_MASK              (0x1U << PERICRU_GATE_CON08_SCLK_UART4_EN_SHIFT)             /* 0x00000010 */
#define PERICRU_GATE_CON08_CLK_UART5_EN_SHIFT              (5U)
#define PERICRU_GATE_CON08_CLK_UART5_EN_MASK               (0x1U << PERICRU_GATE_CON08_CLK_UART5_EN_SHIFT)              /* 0x00000020 */
#define PERICRU_GATE_CON08_CLK_UART5_FRAC_EN_SHIFT         (6U)
#define PERICRU_GATE_CON08_CLK_UART5_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON08_CLK_UART5_FRAC_EN_SHIFT)         /* 0x00000040 */
#define PERICRU_GATE_CON08_SCLK_UART5_EN_SHIFT             (7U)
#define PERICRU_GATE_CON08_SCLK_UART5_EN_MASK              (0x1U << PERICRU_GATE_CON08_SCLK_UART5_EN_SHIFT)             /* 0x00000080 */
#define PERICRU_GATE_CON08_CLK_UART6_EN_SHIFT              (8U)
#define PERICRU_GATE_CON08_CLK_UART6_EN_MASK               (0x1U << PERICRU_GATE_CON08_CLK_UART6_EN_SHIFT)              /* 0x00000100 */
#define PERICRU_GATE_CON08_CLK_UART6_FRAC_EN_SHIFT         (9U)
#define PERICRU_GATE_CON08_CLK_UART6_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON08_CLK_UART6_FRAC_EN_SHIFT)         /* 0x00000200 */
#define PERICRU_GATE_CON08_SCLK_UART6_EN_SHIFT             (10U)
#define PERICRU_GATE_CON08_SCLK_UART6_EN_MASK              (0x1U << PERICRU_GATE_CON08_SCLK_UART6_EN_SHIFT)             /* 0x00000400 */
#define PERICRU_GATE_CON08_CLK_UART7_EN_SHIFT              (11U)
#define PERICRU_GATE_CON08_CLK_UART7_EN_MASK               (0x1U << PERICRU_GATE_CON08_CLK_UART7_EN_SHIFT)              /* 0x00000800 */
#define PERICRU_GATE_CON08_CLK_UART7_FRAC_EN_SHIFT         (12U)
#define PERICRU_GATE_CON08_CLK_UART7_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON08_CLK_UART7_FRAC_EN_SHIFT)         /* 0x00001000 */
#define PERICRU_GATE_CON08_SCLK_UART7_EN_SHIFT             (13U)
#define PERICRU_GATE_CON08_SCLK_UART7_EN_MASK              (0x1U << PERICRU_GATE_CON08_SCLK_UART7_EN_SHIFT)             /* 0x00002000 */
#define PERICRU_GATE_CON08_CLK_UART8_EN_SHIFT              (14U)
#define PERICRU_GATE_CON08_CLK_UART8_EN_MASK               (0x1U << PERICRU_GATE_CON08_CLK_UART8_EN_SHIFT)              /* 0x00004000 */
#define PERICRU_GATE_CON08_CLK_UART8_FRAC_EN_SHIFT         (15U)
#define PERICRU_GATE_CON08_CLK_UART8_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON08_CLK_UART8_FRAC_EN_SHIFT)         /* 0x00008000 */
/* GATE_CON09 */
#define PERICRU_GATE_CON09_OFFSET                          (0x324U)
#define PERICRU_GATE_CON09_SCLK_UART8_EN_SHIFT             (0U)
#define PERICRU_GATE_CON09_SCLK_UART8_EN_MASK              (0x1U << PERICRU_GATE_CON09_SCLK_UART8_EN_SHIFT)             /* 0x00000001 */
#define PERICRU_GATE_CON09_CLK_UART9_EN_SHIFT              (1U)
#define PERICRU_GATE_CON09_CLK_UART9_EN_MASK               (0x1U << PERICRU_GATE_CON09_CLK_UART9_EN_SHIFT)              /* 0x00000002 */
#define PERICRU_GATE_CON09_CLK_UART9_FRAC_EN_SHIFT         (2U)
#define PERICRU_GATE_CON09_CLK_UART9_FRAC_EN_MASK          (0x1U << PERICRU_GATE_CON09_CLK_UART9_FRAC_EN_SHIFT)         /* 0x00000004 */
#define PERICRU_GATE_CON09_SCLK_UART9_EN_SHIFT             (3U)
#define PERICRU_GATE_CON09_SCLK_UART9_EN_MASK              (0x1U << PERICRU_GATE_CON09_SCLK_UART9_EN_SHIFT)             /* 0x00000008 */
/* GATE_CON10 */
#define PERICRU_GATE_CON10_OFFSET                          (0x328U)
#define PERICRU_GATE_CON10_PCLK_PWM1_PERI_EN_SHIFT         (0U)
#define PERICRU_GATE_CON10_PCLK_PWM1_PERI_EN_MASK          (0x1U << PERICRU_GATE_CON10_PCLK_PWM1_PERI_EN_SHIFT)         /* 0x00000001 */
#define PERICRU_GATE_CON10_CLK_PWM1_PERI_EN_SHIFT          (1U)
#define PERICRU_GATE_CON10_CLK_PWM1_PERI_EN_MASK           (0x1U << PERICRU_GATE_CON10_CLK_PWM1_PERI_EN_SHIFT)          /* 0x00000002 */
#define PERICRU_GATE_CON10_CLK_CAPTURE_PWM1_PERI_EN_SHIFT  (2U)
#define PERICRU_GATE_CON10_CLK_CAPTURE_PWM1_PERI_EN_MASK   (0x1U << PERICRU_GATE_CON10_CLK_CAPTURE_PWM1_PERI_EN_SHIFT)  /* 0x00000004 */
#define PERICRU_GATE_CON10_PCLK_PWM2_PERI_EN_SHIFT         (3U)
#define PERICRU_GATE_CON10_PCLK_PWM2_PERI_EN_MASK          (0x1U << PERICRU_GATE_CON10_PCLK_PWM2_PERI_EN_SHIFT)         /* 0x00000008 */
#define PERICRU_GATE_CON10_CLK_PWM2_PERI_EN_SHIFT          (4U)
#define PERICRU_GATE_CON10_CLK_PWM2_PERI_EN_MASK           (0x1U << PERICRU_GATE_CON10_CLK_PWM2_PERI_EN_SHIFT)          /* 0x00000010 */
#define PERICRU_GATE_CON10_CLK_CAPTURE_PWM2_PERI_EN_SHIFT  (5U)
#define PERICRU_GATE_CON10_CLK_CAPTURE_PWM2_PERI_EN_MASK   (0x1U << PERICRU_GATE_CON10_CLK_CAPTURE_PWM2_PERI_EN_SHIFT)  /* 0x00000020 */
#define PERICRU_GATE_CON10_PCLK_PWM3_PERI_EN_SHIFT         (6U)
#define PERICRU_GATE_CON10_PCLK_PWM3_PERI_EN_MASK          (0x1U << PERICRU_GATE_CON10_PCLK_PWM3_PERI_EN_SHIFT)         /* 0x00000040 */
#define PERICRU_GATE_CON10_CLK_PWM3_PERI_EN_SHIFT          (7U)
#define PERICRU_GATE_CON10_CLK_PWM3_PERI_EN_MASK           (0x1U << PERICRU_GATE_CON10_CLK_PWM3_PERI_EN_SHIFT)          /* 0x00000080 */
#define PERICRU_GATE_CON10_CLK_CAPTURE_PWM3_PERI_EN_SHIFT  (8U)
#define PERICRU_GATE_CON10_CLK_CAPTURE_PWM3_PERI_EN_MASK   (0x1U << PERICRU_GATE_CON10_CLK_CAPTURE_PWM3_PERI_EN_SHIFT)  /* 0x00000100 */
/* GATE_CON12 */
#define PERICRU_GATE_CON12_OFFSET                          (0x330U)
#define PERICRU_GATE_CON12_ACLK_CRYPTO_EN_SHIFT            (0U)
#define PERICRU_GATE_CON12_ACLK_CRYPTO_EN_MASK             (0x1U << PERICRU_GATE_CON12_ACLK_CRYPTO_EN_SHIFT)            /* 0x00000001 */
#define PERICRU_GATE_CON12_HCLK_CRYPTO_EN_SHIFT            (1U)
#define PERICRU_GATE_CON12_HCLK_CRYPTO_EN_MASK             (0x1U << PERICRU_GATE_CON12_HCLK_CRYPTO_EN_SHIFT)            /* 0x00000002 */
#define PERICRU_GATE_CON12_PCLK_CRYPTO_EN_SHIFT            (2U)
#define PERICRU_GATE_CON12_PCLK_CRYPTO_EN_MASK             (0x1U << PERICRU_GATE_CON12_PCLK_CRYPTO_EN_SHIFT)            /* 0x00000004 */
#define PERICRU_GATE_CON12_CLK_CORE_CRYPTO_EN_SHIFT        (3U)
#define PERICRU_GATE_CON12_CLK_CORE_CRYPTO_EN_MASK         (0x1U << PERICRU_GATE_CON12_CLK_CORE_CRYPTO_EN_SHIFT)        /* 0x00000008 */
#define PERICRU_GATE_CON12_CLK_PKA_CRYPTO_EN_SHIFT         (4U)
#define PERICRU_GATE_CON12_CLK_PKA_CRYPTO_EN_MASK          (0x1U << PERICRU_GATE_CON12_CLK_PKA_CRYPTO_EN_SHIFT)         /* 0x00000010 */
#define PERICRU_GATE_CON12_HCLK_KLAD_EN_SHIFT              (5U)
#define PERICRU_GATE_CON12_HCLK_KLAD_EN_MASK               (0x1U << PERICRU_GATE_CON12_HCLK_KLAD_EN_SHIFT)              /* 0x00000020 */
#define PERICRU_GATE_CON12_PCLK_KEY_READER_EN_SHIFT        (6U)
#define PERICRU_GATE_CON12_PCLK_KEY_READER_EN_MASK         (0x1U << PERICRU_GATE_CON12_PCLK_KEY_READER_EN_SHIFT)        /* 0x00000040 */
#define PERICRU_GATE_CON12_HCLK_RK_RNG_NS_EN_SHIFT         (7U)
#define PERICRU_GATE_CON12_HCLK_RK_RNG_NS_EN_MASK          (0x1U << PERICRU_GATE_CON12_HCLK_RK_RNG_NS_EN_SHIFT)         /* 0x00000080 */
#define PERICRU_GATE_CON12_HCLK_RK_RNG_S_EN_SHIFT          (8U)
#define PERICRU_GATE_CON12_HCLK_RK_RNG_S_EN_MASK           (0x1U << PERICRU_GATE_CON12_HCLK_RK_RNG_S_EN_SHIFT)          /* 0x00000100 */
#define PERICRU_GATE_CON12_HCLK_TRNG_NS_EN_SHIFT           (9U)
#define PERICRU_GATE_CON12_HCLK_TRNG_NS_EN_MASK            (0x1U << PERICRU_GATE_CON12_HCLK_TRNG_NS_EN_SHIFT)           /* 0x00000200 */
#define PERICRU_GATE_CON12_HCLK_TRNG_S_EN_SHIFT            (10U)
#define PERICRU_GATE_CON12_HCLK_TRNG_S_EN_MASK             (0x1U << PERICRU_GATE_CON12_HCLK_TRNG_S_EN_SHIFT)            /* 0x00000400 */
#define PERICRU_GATE_CON12_HCLK_CRYPTO_S_EN_SHIFT          (11U)
#define PERICRU_GATE_CON12_HCLK_CRYPTO_S_EN_MASK           (0x1U << PERICRU_GATE_CON12_HCLK_CRYPTO_S_EN_SHIFT)          /* 0x00000800 */
/* GATE_CON13 */
#define PERICRU_GATE_CON13_OFFSET                          (0x334U)
#define PERICRU_GATE_CON13_PCLK_PERI_WDT_EN_SHIFT          (0U)
#define PERICRU_GATE_CON13_PCLK_PERI_WDT_EN_MASK           (0x1U << PERICRU_GATE_CON13_PCLK_PERI_WDT_EN_SHIFT)          /* 0x00000001 */
#define PERICRU_GATE_CON13_TCLK_PERI_WDT_EN_SHIFT          (1U)
#define PERICRU_GATE_CON13_TCLK_PERI_WDT_EN_MASK           (0x1U << PERICRU_GATE_CON13_TCLK_PERI_WDT_EN_SHIFT)          /* 0x00000002 */
#define PERICRU_GATE_CON13_ACLK_SYSMEM_EN_SHIFT            (2U)
#define PERICRU_GATE_CON13_ACLK_SYSMEM_EN_MASK             (0x1U << PERICRU_GATE_CON13_ACLK_SYSMEM_EN_SHIFT)            /* 0x00000004 */
#define PERICRU_GATE_CON13_HCLK_BOOTROM_EN_SHIFT           (3U)
#define PERICRU_GATE_CON13_HCLK_BOOTROM_EN_MASK            (0x1U << PERICRU_GATE_CON13_HCLK_BOOTROM_EN_SHIFT)           /* 0x00000008 */
#define PERICRU_GATE_CON13_PCLK_PERI_GRF_EN_SHIFT          (4U)
#define PERICRU_GATE_CON13_PCLK_PERI_GRF_EN_MASK           (0x1U << PERICRU_GATE_CON13_PCLK_PERI_GRF_EN_SHIFT)          /* 0x00000010 */
#define PERICRU_GATE_CON13_ACLK_DMAC_EN_SHIFT              (5U)
#define PERICRU_GATE_CON13_ACLK_DMAC_EN_MASK               (0x1U << PERICRU_GATE_CON13_ACLK_DMAC_EN_SHIFT)              /* 0x00000020 */
#define PERICRU_GATE_CON13_ACLK_RKDMAC_EN_SHIFT            (6U)
#define PERICRU_GATE_CON13_ACLK_RKDMAC_EN_MASK             (0x1U << PERICRU_GATE_CON13_ACLK_RKDMAC_EN_SHIFT)            /* 0x00000040 */
/* GATE_CON14 */
#define PERICRU_GATE_CON14_OFFSET                          (0x338U)
#define PERICRU_GATE_CON14_PCLK_OTPC_NS_EN_SHIFT           (0U)
#define PERICRU_GATE_CON14_PCLK_OTPC_NS_EN_MASK            (0x1U << PERICRU_GATE_CON14_PCLK_OTPC_NS_EN_SHIFT)           /* 0x00000001 */
#define PERICRU_GATE_CON14_CLK_SBPI_OTPC_NS_EN_SHIFT       (1U)
#define PERICRU_GATE_CON14_CLK_SBPI_OTPC_NS_EN_MASK        (0x1U << PERICRU_GATE_CON14_CLK_SBPI_OTPC_NS_EN_SHIFT)       /* 0x00000002 */
#define PERICRU_GATE_CON14_CLK_USER_OTPC_NS_EN_SHIFT       (2U)
#define PERICRU_GATE_CON14_CLK_USER_OTPC_NS_EN_MASK        (0x1U << PERICRU_GATE_CON14_CLK_USER_OTPC_NS_EN_SHIFT)       /* 0x00000004 */
#define PERICRU_GATE_CON14_PCLK_OTPC_S_EN_SHIFT            (3U)
#define PERICRU_GATE_CON14_PCLK_OTPC_S_EN_MASK             (0x1U << PERICRU_GATE_CON14_PCLK_OTPC_S_EN_SHIFT)            /* 0x00000008 */
#define PERICRU_GATE_CON14_CLK_SBPI_OTPC_S_EN_SHIFT        (4U)
#define PERICRU_GATE_CON14_CLK_SBPI_OTPC_S_EN_MASK         (0x1U << PERICRU_GATE_CON14_CLK_SBPI_OTPC_S_EN_SHIFT)        /* 0x00000010 */
#define PERICRU_GATE_CON14_CLK_USER_OTPC_S_EN_SHIFT        (5U)
#define PERICRU_GATE_CON14_CLK_USER_OTPC_S_EN_MASK         (0x1U << PERICRU_GATE_CON14_CLK_USER_OTPC_S_EN_SHIFT)        /* 0x00000020 */
#define PERICRU_GATE_CON14_CLK_OTPC_ARB_EN_SHIFT           (6U)
#define PERICRU_GATE_CON14_CLK_OTPC_ARB_EN_MASK            (0x1U << PERICRU_GATE_CON14_CLK_OTPC_ARB_EN_SHIFT)           /* 0x00000040 */
#define PERICRU_GATE_CON14_PCLK_OTPPHY_EN_SHIFT            (7U)
#define PERICRU_GATE_CON14_PCLK_OTPPHY_EN_MASK             (0x1U << PERICRU_GATE_CON14_PCLK_OTPPHY_EN_SHIFT)            /* 0x00000080 */
/* GATE_CON15 */
#define PERICRU_GATE_CON15_OFFSET                          (0x33CU)
#define PERICRU_GATE_CON15_PCLK_USB2PHY_EN_SHIFT           (0U)
#define PERICRU_GATE_CON15_PCLK_USB2PHY_EN_MASK            (0x1U << PERICRU_GATE_CON15_PCLK_USB2PHY_EN_SHIFT)           /* 0x00000001 */
#define PERICRU_GATE_CON15_PCLK_PIPEPHY_EN_SHIFT           (7U)
#define PERICRU_GATE_CON15_PCLK_PIPEPHY_EN_MASK            (0x1U << PERICRU_GATE_CON15_PCLK_PIPEPHY_EN_SHIFT)           /* 0x00000080 */
/* GATE_CON16 */
#define PERICRU_GATE_CON16_OFFSET                          (0x340U)
#define PERICRU_GATE_CON16_PCLK_SARADC_EN_SHIFT            (4U)
#define PERICRU_GATE_CON16_PCLK_SARADC_EN_MASK             (0x1U << PERICRU_GATE_CON16_PCLK_SARADC_EN_SHIFT)            /* 0x00000010 */
#define PERICRU_GATE_CON16_CLK_SARADC_EN_SHIFT             (5U)
#define PERICRU_GATE_CON16_CLK_SARADC_EN_MASK              (0x1U << PERICRU_GATE_CON16_CLK_SARADC_EN_SHIFT)             /* 0x00000020 */
#define PERICRU_GATE_CON16_PCLK_IOC_VCCIO234_EN_SHIFT      (12U)
#define PERICRU_GATE_CON16_PCLK_IOC_VCCIO234_EN_MASK       (0x1U << PERICRU_GATE_CON16_PCLK_IOC_VCCIO234_EN_SHIFT)      /* 0x00001000 */
/* GATE_CON17 */
#define PERICRU_GATE_CON17_OFFSET                          (0x344U)
#define PERICRU_GATE_CON17_PCLK_PERI_GPIO1_EN_SHIFT        (0U)
#define PERICRU_GATE_CON17_PCLK_PERI_GPIO1_EN_MASK         (0x1U << PERICRU_GATE_CON17_PCLK_PERI_GPIO1_EN_SHIFT)        /* 0x00000001 */
#define PERICRU_GATE_CON17_PCLK_PERI_GPIO2_EN_SHIFT        (1U)
#define PERICRU_GATE_CON17_PCLK_PERI_GPIO2_EN_MASK         (0x1U << PERICRU_GATE_CON17_PCLK_PERI_GPIO2_EN_SHIFT)        /* 0x00000002 */
#define PERICRU_GATE_CON17_DCLK_PERI_GPIO1_EN_SHIFT        (2U)
#define PERICRU_GATE_CON17_DCLK_PERI_GPIO1_EN_MASK         (0x1U << PERICRU_GATE_CON17_DCLK_PERI_GPIO1_EN_SHIFT)        /* 0x00000004 */
#define PERICRU_GATE_CON17_DCLK_PERI_GPIO2_EN_SHIFT        (3U)
#define PERICRU_GATE_CON17_DCLK_PERI_GPIO2_EN_MASK         (0x1U << PERICRU_GATE_CON17_DCLK_PERI_GPIO2_EN_SHIFT)        /* 0x00000008 */
#define PERICRU_GATE_CON17_DCLK_PERI_GPIO_EN_SHIFT         (4U)
#define PERICRU_GATE_CON17_DCLK_PERI_GPIO_EN_MASK          (0x1U << PERICRU_GATE_CON17_DCLK_PERI_GPIO_EN_SHIFT)         /* 0x00000010 */
#define PERICRU_GATE_CON17_CLK_TESTOUT_PERI_EN_SHIFT       (15U)
#define PERICRU_GATE_CON17_CLK_TESTOUT_PERI_EN_MASK        (0x1U << PERICRU_GATE_CON17_CLK_TESTOUT_PERI_EN_SHIFT)       /* 0x00008000 */
/* SOFTRST_CON00 */
#define PERICRU_SOFTRST_CON00_OFFSET                       (0x400U)
/* SOFTRST_CON01 */
#define PERICRU_SOFTRST_CON01_OFFSET                       (0x404U)
#define PERICRU_SOFTRST_CON01_ARESETN_PERI_BIU_SHIFT       (3U)
#define PERICRU_SOFTRST_CON01_ARESETN_PERI_BIU_MASK        (0x1U << PERICRU_SOFTRST_CON01_ARESETN_PERI_BIU_SHIFT)       /* 0x00000008 */
#define PERICRU_SOFTRST_CON01_HRESETN_PERI_BIU_SHIFT       (4U)
#define PERICRU_SOFTRST_CON01_HRESETN_PERI_BIU_MASK        (0x1U << PERICRU_SOFTRST_CON01_HRESETN_PERI_BIU_SHIFT)       /* 0x00000010 */
#define PERICRU_SOFTRST_CON01_PRESETN_PERI_BIU_SHIFT       (5U)
#define PERICRU_SOFTRST_CON01_PRESETN_PERI_BIU_MASK        (0x1U << PERICRU_SOFTRST_CON01_PRESETN_PERI_BIU_SHIFT)       /* 0x00000020 */
#define PERICRU_SOFTRST_CON01_PRESETN_PERICRU_SHIFT        (6U)
#define PERICRU_SOFTRST_CON01_PRESETN_PERICRU_MASK         (0x1U << PERICRU_SOFTRST_CON01_PRESETN_PERICRU_SHIFT)        /* 0x00000040 */
/* SOFTRST_CON02 */
#define PERICRU_SOFTRST_CON02_OFFSET                       (0x408U)
#define PERICRU_SOFTRST_CON02_HRESETN_SAI0_SHIFT           (0U)
#define PERICRU_SOFTRST_CON02_HRESETN_SAI0_MASK            (0x1U << PERICRU_SOFTRST_CON02_HRESETN_SAI0_SHIFT)           /* 0x00000001 */
#define PERICRU_SOFTRST_CON02_MRESETN_SAI0_SHIFT           (3U)
#define PERICRU_SOFTRST_CON02_MRESETN_SAI0_MASK            (0x1U << PERICRU_SOFTRST_CON02_MRESETN_SAI0_SHIFT)           /* 0x00000008 */
#define PERICRU_SOFTRST_CON02_HRESETN_SAI1_SHIFT           (5U)
#define PERICRU_SOFTRST_CON02_HRESETN_SAI1_MASK            (0x1U << PERICRU_SOFTRST_CON02_HRESETN_SAI1_SHIFT)           /* 0x00000020 */
#define PERICRU_SOFTRST_CON02_MRESETN_SAI1_SHIFT           (8U)
#define PERICRU_SOFTRST_CON02_MRESETN_SAI1_MASK            (0x1U << PERICRU_SOFTRST_CON02_MRESETN_SAI1_SHIFT)           /* 0x00000100 */
#define PERICRU_SOFTRST_CON02_HRESETN_SAI2_SHIFT           (10U)
#define PERICRU_SOFTRST_CON02_HRESETN_SAI2_MASK            (0x1U << PERICRU_SOFTRST_CON02_HRESETN_SAI2_SHIFT)           /* 0x00000400 */
#define PERICRU_SOFTRST_CON02_MRESETN_SAI2_SHIFT           (13U)
#define PERICRU_SOFTRST_CON02_MRESETN_SAI2_MASK            (0x1U << PERICRU_SOFTRST_CON02_MRESETN_SAI2_SHIFT)           /* 0x00002000 */
/* SOFTRST_CON03 */
#define PERICRU_SOFTRST_CON03_OFFSET                       (0x40CU)
#define PERICRU_SOFTRST_CON03_HRESETN_DSM_SHIFT            (1U)
#define PERICRU_SOFTRST_CON03_HRESETN_DSM_MASK             (0x1U << PERICRU_SOFTRST_CON03_HRESETN_DSM_SHIFT)            /* 0x00000002 */
#define PERICRU_SOFTRST_CON03_RESETN_DSM_SHIFT             (2U)
#define PERICRU_SOFTRST_CON03_RESETN_DSM_MASK              (0x1U << PERICRU_SOFTRST_CON03_RESETN_DSM_SHIFT)             /* 0x00000004 */
#define PERICRU_SOFTRST_CON03_HRESETN_PDM_SHIFT            (4U)
#define PERICRU_SOFTRST_CON03_HRESETN_PDM_MASK             (0x1U << PERICRU_SOFTRST_CON03_HRESETN_PDM_SHIFT)            /* 0x00000010 */
#define PERICRU_SOFTRST_CON03_MRESETN_PDM_SHIFT            (5U)
#define PERICRU_SOFTRST_CON03_MRESETN_PDM_MASK             (0x1U << PERICRU_SOFTRST_CON03_MRESETN_PDM_SHIFT)            /* 0x00000020 */
#define PERICRU_SOFTRST_CON03_HRESETN_SPDIF_SHIFT          (8U)
#define PERICRU_SOFTRST_CON03_HRESETN_SPDIF_MASK           (0x1U << PERICRU_SOFTRST_CON03_HRESETN_SPDIF_SHIFT)          /* 0x00000100 */
#define PERICRU_SOFTRST_CON03_MRESETN_SPDIF_SHIFT          (11U)
#define PERICRU_SOFTRST_CON03_MRESETN_SPDIF_MASK           (0x1U << PERICRU_SOFTRST_CON03_MRESETN_SPDIF_SHIFT)          /* 0x00000800 */
/* SOFTRST_CON04 */
#define PERICRU_SOFTRST_CON04_OFFSET                       (0x410U)
#define PERICRU_SOFTRST_CON04_HRESETN_SDMMC0_SHIFT         (0U)
#define PERICRU_SOFTRST_CON04_HRESETN_SDMMC0_MASK          (0x1U << PERICRU_SOFTRST_CON04_HRESETN_SDMMC0_SHIFT)         /* 0x00000001 */
#define PERICRU_SOFTRST_CON04_HRESETN_SDMMC1_SHIFT         (2U)
#define PERICRU_SOFTRST_CON04_HRESETN_SDMMC1_MASK          (0x1U << PERICRU_SOFTRST_CON04_HRESETN_SDMMC1_SHIFT)         /* 0x00000004 */
#define PERICRU_SOFTRST_CON04_HRESETN_EMMC_SHIFT           (8U)
#define PERICRU_SOFTRST_CON04_HRESETN_EMMC_MASK            (0x1U << PERICRU_SOFTRST_CON04_HRESETN_EMMC_SHIFT)           /* 0x00000100 */
#define PERICRU_SOFTRST_CON04_ARESETN_EMMC_SHIFT           (9U)
#define PERICRU_SOFTRST_CON04_ARESETN_EMMC_MASK            (0x1U << PERICRU_SOFTRST_CON04_ARESETN_EMMC_SHIFT)           /* 0x00000200 */
#define PERICRU_SOFTRST_CON04_CRESETN_EMMC_SHIFT           (10U)
#define PERICRU_SOFTRST_CON04_CRESETN_EMMC_MASK            (0x1U << PERICRU_SOFTRST_CON04_CRESETN_EMMC_SHIFT)           /* 0x00000400 */
#define PERICRU_SOFTRST_CON04_BRESETN_EMMC_SHIFT           (11U)
#define PERICRU_SOFTRST_CON04_BRESETN_EMMC_MASK            (0x1U << PERICRU_SOFTRST_CON04_BRESETN_EMMC_SHIFT)           /* 0x00000800 */
#define PERICRU_SOFTRST_CON04_TRESETN_EMMC_SHIFT           (12U)
#define PERICRU_SOFTRST_CON04_TRESETN_EMMC_MASK            (0x1U << PERICRU_SOFTRST_CON04_TRESETN_EMMC_SHIFT)           /* 0x00001000 */
#define PERICRU_SOFTRST_CON04_SRESETN_SFC_SHIFT            (13U)
#define PERICRU_SOFTRST_CON04_SRESETN_SFC_MASK             (0x1U << PERICRU_SOFTRST_CON04_SRESETN_SFC_SHIFT)            /* 0x00002000 */
#define PERICRU_SOFTRST_CON04_HRESETN_SFC_SHIFT            (14U)
#define PERICRU_SOFTRST_CON04_HRESETN_SFC_MASK             (0x1U << PERICRU_SOFTRST_CON04_HRESETN_SFC_SHIFT)            /* 0x00004000 */
/* SOFTRST_CON05 */
#define PERICRU_SOFTRST_CON05_OFFSET                       (0x414U)
#define PERICRU_SOFTRST_CON05_HRESETN_USB2HOST_SHIFT       (0U)
#define PERICRU_SOFTRST_CON05_HRESETN_USB2HOST_MASK        (0x1U << PERICRU_SOFTRST_CON05_HRESETN_USB2HOST_SHIFT)       /* 0x00000001 */
#define PERICRU_SOFTRST_CON05_HRESETN_USB2HOST_ARB_SHIFT   (1U)
#define PERICRU_SOFTRST_CON05_HRESETN_USB2HOST_ARB_MASK    (0x1U << PERICRU_SOFTRST_CON05_HRESETN_USB2HOST_ARB_SHIFT)   /* 0x00000002 */
#define PERICRU_SOFTRST_CON05_RESETN_USB2HOST_UTMI_SHIFT   (2U)
#define PERICRU_SOFTRST_CON05_RESETN_USB2HOST_UTMI_MASK    (0x1U << PERICRU_SOFTRST_CON05_RESETN_USB2HOST_UTMI_SHIFT)   /* 0x00000004 */
/* SOFTRST_CON06 */
#define PERICRU_SOFTRST_CON06_OFFSET                       (0x418U)
#define PERICRU_SOFTRST_CON06_PRESETN_SPI1_SHIFT           (0U)
#define PERICRU_SOFTRST_CON06_PRESETN_SPI1_MASK            (0x1U << PERICRU_SOFTRST_CON06_PRESETN_SPI1_SHIFT)           /* 0x00000001 */
#define PERICRU_SOFTRST_CON06_RESETN_SPI1_SHIFT            (1U)
#define PERICRU_SOFTRST_CON06_RESETN_SPI1_MASK             (0x1U << PERICRU_SOFTRST_CON06_RESETN_SPI1_SHIFT)            /* 0x00000002 */
#define PERICRU_SOFTRST_CON06_PRESETN_SPI2_SHIFT           (3U)
#define PERICRU_SOFTRST_CON06_PRESETN_SPI2_MASK            (0x1U << PERICRU_SOFTRST_CON06_PRESETN_SPI2_SHIFT)           /* 0x00000008 */
#define PERICRU_SOFTRST_CON06_RESETN_SPI2_SHIFT            (4U)
#define PERICRU_SOFTRST_CON06_RESETN_SPI2_MASK             (0x1U << PERICRU_SOFTRST_CON06_RESETN_SPI2_SHIFT)            /* 0x00000010 */
/* SOFTRST_CON07 */
#define PERICRU_SOFTRST_CON07_OFFSET                       (0x41CU)
#define PERICRU_SOFTRST_CON07_PRESETN_UART1_SHIFT          (0U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART1_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART1_SHIFT)          /* 0x00000001 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART2_SHIFT          (1U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART2_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART2_SHIFT)          /* 0x00000002 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART3_SHIFT          (2U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART3_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART3_SHIFT)          /* 0x00000004 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART4_SHIFT          (3U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART4_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART4_SHIFT)          /* 0x00000008 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART5_SHIFT          (4U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART5_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART5_SHIFT)          /* 0x00000010 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART6_SHIFT          (5U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART6_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART6_SHIFT)          /* 0x00000020 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART7_SHIFT          (6U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART7_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART7_SHIFT)          /* 0x00000040 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART8_SHIFT          (7U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART8_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART8_SHIFT)          /* 0x00000080 */
#define PERICRU_SOFTRST_CON07_PRESETN_UART9_SHIFT          (8U)
#define PERICRU_SOFTRST_CON07_PRESETN_UART9_MASK           (0x1U << PERICRU_SOFTRST_CON07_PRESETN_UART9_SHIFT)          /* 0x00000100 */
#define PERICRU_SOFTRST_CON07_SRESETN_UART1_SHIFT          (11U)
#define PERICRU_SOFTRST_CON07_SRESETN_UART1_MASK           (0x1U << PERICRU_SOFTRST_CON07_SRESETN_UART1_SHIFT)          /* 0x00000800 */
#define PERICRU_SOFTRST_CON07_SRESETN_UART2_SHIFT          (14U)
#define PERICRU_SOFTRST_CON07_SRESETN_UART2_MASK           (0x1U << PERICRU_SOFTRST_CON07_SRESETN_UART2_SHIFT)          /* 0x00004000 */
/* SOFTRST_CON08 */
#define PERICRU_SOFTRST_CON08_OFFSET                       (0x420U)
#define PERICRU_SOFTRST_CON08_SRESETN_UART3_SHIFT          (1U)
#define PERICRU_SOFTRST_CON08_SRESETN_UART3_MASK           (0x1U << PERICRU_SOFTRST_CON08_SRESETN_UART3_SHIFT)          /* 0x00000002 */
#define PERICRU_SOFTRST_CON08_SRESETN_UART4_SHIFT          (4U)
#define PERICRU_SOFTRST_CON08_SRESETN_UART4_MASK           (0x1U << PERICRU_SOFTRST_CON08_SRESETN_UART4_SHIFT)          /* 0x00000010 */
#define PERICRU_SOFTRST_CON08_SRESETN_UART5_SHIFT          (7U)
#define PERICRU_SOFTRST_CON08_SRESETN_UART5_MASK           (0x1U << PERICRU_SOFTRST_CON08_SRESETN_UART5_SHIFT)          /* 0x00000080 */
#define PERICRU_SOFTRST_CON08_SRESETN_UART6_SHIFT          (10U)
#define PERICRU_SOFTRST_CON08_SRESETN_UART6_MASK           (0x1U << PERICRU_SOFTRST_CON08_SRESETN_UART6_SHIFT)          /* 0x00000400 */
#define PERICRU_SOFTRST_CON08_SRESETN_UART7_SHIFT          (13U)
#define PERICRU_SOFTRST_CON08_SRESETN_UART7_MASK           (0x1U << PERICRU_SOFTRST_CON08_SRESETN_UART7_SHIFT)          /* 0x00002000 */
/* SOFTRST_CON09 */
#define PERICRU_SOFTRST_CON09_OFFSET                       (0x424U)
#define PERICRU_SOFTRST_CON09_SRESETN_UART8_SHIFT          (0U)
#define PERICRU_SOFTRST_CON09_SRESETN_UART8_MASK           (0x1U << PERICRU_SOFTRST_CON09_SRESETN_UART8_SHIFT)          /* 0x00000001 */
#define PERICRU_SOFTRST_CON09_SRESETN_UART9_SHIFT          (3U)
#define PERICRU_SOFTRST_CON09_SRESETN_UART9_MASK           (0x1U << PERICRU_SOFTRST_CON09_SRESETN_UART9_SHIFT)          /* 0x00000008 */
/* SOFTRST_CON10 */
#define PERICRU_SOFTRST_CON10_OFFSET                       (0x428U)
#define PERICRU_SOFTRST_CON10_PRESETN_PWM1_PERI_SHIFT      (0U)
#define PERICRU_SOFTRST_CON10_PRESETN_PWM1_PERI_MASK       (0x1U << PERICRU_SOFTRST_CON10_PRESETN_PWM1_PERI_SHIFT)      /* 0x00000001 */
#define PERICRU_SOFTRST_CON10_RESETN_PWM1_PERI_SHIFT       (1U)
#define PERICRU_SOFTRST_CON10_RESETN_PWM1_PERI_MASK        (0x1U << PERICRU_SOFTRST_CON10_RESETN_PWM1_PERI_SHIFT)       /* 0x00000002 */
#define PERICRU_SOFTRST_CON10_PRESETN_PWM2_PERI_SHIFT      (3U)
#define PERICRU_SOFTRST_CON10_PRESETN_PWM2_PERI_MASK       (0x1U << PERICRU_SOFTRST_CON10_PRESETN_PWM2_PERI_SHIFT)      /* 0x00000008 */
#define PERICRU_SOFTRST_CON10_RESETN_PWM2_PERI_SHIFT       (4U)
#define PERICRU_SOFTRST_CON10_RESETN_PWM2_PERI_MASK        (0x1U << PERICRU_SOFTRST_CON10_RESETN_PWM2_PERI_SHIFT)       /* 0x00000010 */
#define PERICRU_SOFTRST_CON10_PRESETN_PWM3_PERI_SHIFT      (6U)
#define PERICRU_SOFTRST_CON10_PRESETN_PWM3_PERI_MASK       (0x1U << PERICRU_SOFTRST_CON10_PRESETN_PWM3_PERI_SHIFT)      /* 0x00000040 */
#define PERICRU_SOFTRST_CON10_RESETN_PWM3_PERI_SHIFT       (7U)
#define PERICRU_SOFTRST_CON10_RESETN_PWM3_PERI_MASK        (0x1U << PERICRU_SOFTRST_CON10_RESETN_PWM3_PERI_SHIFT)       /* 0x00000080 */
/* SOFTRST_CON12 */
#define PERICRU_SOFTRST_CON12_OFFSET                       (0x430U)
#define PERICRU_SOFTRST_CON12_ARESETN_CRYPTO_SHIFT         (0U)
#define PERICRU_SOFTRST_CON12_ARESETN_CRYPTO_MASK          (0x1U << PERICRU_SOFTRST_CON12_ARESETN_CRYPTO_SHIFT)         /* 0x00000001 */
#define PERICRU_SOFTRST_CON12_HRESETN_CRYPTO_SHIFT         (1U)
#define PERICRU_SOFTRST_CON12_HRESETN_CRYPTO_MASK          (0x1U << PERICRU_SOFTRST_CON12_HRESETN_CRYPTO_SHIFT)         /* 0x00000002 */
#define PERICRU_SOFTRST_CON12_PRESETN_CRYPTO_SHIFT         (2U)
#define PERICRU_SOFTRST_CON12_PRESETN_CRYPTO_MASK          (0x1U << PERICRU_SOFTRST_CON12_PRESETN_CRYPTO_SHIFT)         /* 0x00000004 */
#define PERICRU_SOFTRST_CON12_RESETN_CORE_CRYPTO_SHIFT     (3U)
#define PERICRU_SOFTRST_CON12_RESETN_CORE_CRYPTO_MASK      (0x1U << PERICRU_SOFTRST_CON12_RESETN_CORE_CRYPTO_SHIFT)     /* 0x00000008 */
#define PERICRU_SOFTRST_CON12_RESETN_PKA_CRYPTO_SHIFT      (4U)
#define PERICRU_SOFTRST_CON12_RESETN_PKA_CRYPTO_MASK       (0x1U << PERICRU_SOFTRST_CON12_RESETN_PKA_CRYPTO_SHIFT)      /* 0x00000010 */
#define PERICRU_SOFTRST_CON12_HRESETN_KLAD_SHIFT           (5U)
#define PERICRU_SOFTRST_CON12_HRESETN_KLAD_MASK            (0x1U << PERICRU_SOFTRST_CON12_HRESETN_KLAD_SHIFT)           /* 0x00000020 */
#define PERICRU_SOFTRST_CON12_PRESETN_KEY_READER_SHIFT     (6U)
#define PERICRU_SOFTRST_CON12_PRESETN_KEY_READER_MASK      (0x1U << PERICRU_SOFTRST_CON12_PRESETN_KEY_READER_SHIFT)     /* 0x00000040 */
#define PERICRU_SOFTRST_CON12_HRESETN_RK_RNG_NS_SHIFT      (7U)
#define PERICRU_SOFTRST_CON12_HRESETN_RK_RNG_NS_MASK       (0x1U << PERICRU_SOFTRST_CON12_HRESETN_RK_RNG_NS_SHIFT)      /* 0x00000080 */
#define PERICRU_SOFTRST_CON12_HRESETN_RK_RNG_S_SHIFT       (8U)
#define PERICRU_SOFTRST_CON12_HRESETN_RK_RNG_S_MASK        (0x1U << PERICRU_SOFTRST_CON12_HRESETN_RK_RNG_S_SHIFT)       /* 0x00000100 */
#define PERICRU_SOFTRST_CON12_HRESETN_TRNG_NS_SHIFT        (9U)
#define PERICRU_SOFTRST_CON12_HRESETN_TRNG_NS_MASK         (0x1U << PERICRU_SOFTRST_CON12_HRESETN_TRNG_NS_SHIFT)        /* 0x00000200 */
#define PERICRU_SOFTRST_CON12_HRESETN_TRNG_S_SHIFT         (10U)
#define PERICRU_SOFTRST_CON12_HRESETN_TRNG_S_MASK          (0x1U << PERICRU_SOFTRST_CON12_HRESETN_TRNG_S_SHIFT)         /* 0x00000400 */
#define PERICRU_SOFTRST_CON12_HRESETN_CRYPTO_S_SHIFT       (11U)
#define PERICRU_SOFTRST_CON12_HRESETN_CRYPTO_S_MASK        (0x1U << PERICRU_SOFTRST_CON12_HRESETN_CRYPTO_S_SHIFT)       /* 0x00000800 */
/* SOFTRST_CON13 */
#define PERICRU_SOFTRST_CON13_OFFSET                       (0x434U)
#define PERICRU_SOFTRST_CON13_PRESETN_PERI_WDT_SHIFT       (0U)
#define PERICRU_SOFTRST_CON13_PRESETN_PERI_WDT_MASK        (0x1U << PERICRU_SOFTRST_CON13_PRESETN_PERI_WDT_SHIFT)       /* 0x00000001 */
#define PERICRU_SOFTRST_CON13_TRESETN_PERI_WDT_SHIFT       (1U)
#define PERICRU_SOFTRST_CON13_TRESETN_PERI_WDT_MASK        (0x1U << PERICRU_SOFTRST_CON13_TRESETN_PERI_WDT_SHIFT)       /* 0x00000002 */
#define PERICRU_SOFTRST_CON13_ARESETN_SYSMEM_SHIFT         (2U)
#define PERICRU_SOFTRST_CON13_ARESETN_SYSMEM_MASK          (0x1U << PERICRU_SOFTRST_CON13_ARESETN_SYSMEM_SHIFT)         /* 0x00000004 */
#define PERICRU_SOFTRST_CON13_HRESETN_BOOTROM_SHIFT        (3U)
#define PERICRU_SOFTRST_CON13_HRESETN_BOOTROM_MASK         (0x1U << PERICRU_SOFTRST_CON13_HRESETN_BOOTROM_SHIFT)        /* 0x00000008 */
#define PERICRU_SOFTRST_CON13_PRESETN_PERI_GRF_SHIFT       (4U)
#define PERICRU_SOFTRST_CON13_PRESETN_PERI_GRF_MASK        (0x1U << PERICRU_SOFTRST_CON13_PRESETN_PERI_GRF_SHIFT)       /* 0x00000010 */
#define PERICRU_SOFTRST_CON13_ARESETN_DMAC_SHIFT           (5U)
#define PERICRU_SOFTRST_CON13_ARESETN_DMAC_MASK            (0x1U << PERICRU_SOFTRST_CON13_ARESETN_DMAC_SHIFT)           /* 0x00000020 */
#define PERICRU_SOFTRST_CON13_ARESETN_RKDMAC_SHIFT         (6U)
#define PERICRU_SOFTRST_CON13_ARESETN_RKDMAC_MASK          (0x1U << PERICRU_SOFTRST_CON13_ARESETN_RKDMAC_SHIFT)         /* 0x00000040 */
/* SOFTRST_CON14 */
#define PERICRU_SOFTRST_CON14_OFFSET                       (0x438U)
#define PERICRU_SOFTRST_CON14_PRESETN_OTPC_NS_SHIFT        (0U)
#define PERICRU_SOFTRST_CON14_PRESETN_OTPC_NS_MASK         (0x1U << PERICRU_SOFTRST_CON14_PRESETN_OTPC_NS_SHIFT)        /* 0x00000001 */
#define PERICRU_SOFTRST_CON14_RESETN_SBPI_OTPC_NS_SHIFT    (1U)
#define PERICRU_SOFTRST_CON14_RESETN_SBPI_OTPC_NS_MASK     (0x1U << PERICRU_SOFTRST_CON14_RESETN_SBPI_OTPC_NS_SHIFT)    /* 0x00000002 */
#define PERICRU_SOFTRST_CON14_RESETN_USER_OTPC_NS_SHIFT    (2U)
#define PERICRU_SOFTRST_CON14_RESETN_USER_OTPC_NS_MASK     (0x1U << PERICRU_SOFTRST_CON14_RESETN_USER_OTPC_NS_SHIFT)    /* 0x00000004 */
#define PERICRU_SOFTRST_CON14_PRESETN_OTPC_S_SHIFT         (3U)
#define PERICRU_SOFTRST_CON14_PRESETN_OTPC_S_MASK          (0x1U << PERICRU_SOFTRST_CON14_PRESETN_OTPC_S_SHIFT)         /* 0x00000008 */
#define PERICRU_SOFTRST_CON14_RESETN_SBPI_OTPC_S_SHIFT     (4U)
#define PERICRU_SOFTRST_CON14_RESETN_SBPI_OTPC_S_MASK      (0x1U << PERICRU_SOFTRST_CON14_RESETN_SBPI_OTPC_S_SHIFT)     /* 0x00000010 */
#define PERICRU_SOFTRST_CON14_RESETN_USER_OTPC_S_SHIFT     (5U)
#define PERICRU_SOFTRST_CON14_RESETN_USER_OTPC_S_MASK      (0x1U << PERICRU_SOFTRST_CON14_RESETN_USER_OTPC_S_SHIFT)     /* 0x00000020 */
#define PERICRU_SOFTRST_CON14_RESETN_OTPC_ARB_SHIFT        (6U)
#define PERICRU_SOFTRST_CON14_RESETN_OTPC_ARB_MASK         (0x1U << PERICRU_SOFTRST_CON14_RESETN_OTPC_ARB_SHIFT)        /* 0x00000040 */
#define PERICRU_SOFTRST_CON14_PRESETN_OTPPHY_SHIFT         (7U)
#define PERICRU_SOFTRST_CON14_PRESETN_OTPPHY_MASK          (0x1U << PERICRU_SOFTRST_CON14_PRESETN_OTPPHY_SHIFT)         /* 0x00000080 */
#define PERICRU_SOFTRST_CON14_RESETN_OTP_NPOR_SHIFT        (8U)
#define PERICRU_SOFTRST_CON14_RESETN_OTP_NPOR_MASK         (0x1U << PERICRU_SOFTRST_CON14_RESETN_OTP_NPOR_SHIFT)        /* 0x00000100 */
/* SOFTRST_CON15 */
#define PERICRU_SOFTRST_CON15_OFFSET                       (0x43CU)
#define PERICRU_SOFTRST_CON15_PRESETN_USB2PHY_SHIFT        (0U)
#define PERICRU_SOFTRST_CON15_PRESETN_USB2PHY_MASK         (0x1U << PERICRU_SOFTRST_CON15_PRESETN_USB2PHY_SHIFT)        /* 0x00000001 */
#define PERICRU_SOFTRST_CON15_RESETN_USB2PHY_POR_SHIFT     (4U)
#define PERICRU_SOFTRST_CON15_RESETN_USB2PHY_POR_MASK      (0x1U << PERICRU_SOFTRST_CON15_RESETN_USB2PHY_POR_SHIFT)     /* 0x00000010 */
#define PERICRU_SOFTRST_CON15_RESETN_USB2PHY_OTG_SHIFT     (5U)
#define PERICRU_SOFTRST_CON15_RESETN_USB2PHY_OTG_MASK      (0x1U << PERICRU_SOFTRST_CON15_RESETN_USB2PHY_OTG_SHIFT)     /* 0x00000020 */
#define PERICRU_SOFTRST_CON15_RESETN_USB2PHY_HOST_SHIFT    (6U)
#define PERICRU_SOFTRST_CON15_RESETN_USB2PHY_HOST_MASK     (0x1U << PERICRU_SOFTRST_CON15_RESETN_USB2PHY_HOST_SHIFT)    /* 0x00000040 */
#define PERICRU_SOFTRST_CON15_PRESETN_PIPEPHY_SHIFT        (7U)
#define PERICRU_SOFTRST_CON15_PRESETN_PIPEPHY_MASK         (0x1U << PERICRU_SOFTRST_CON15_PRESETN_PIPEPHY_SHIFT)        /* 0x00000080 */
/* SOFTRST_CON16 */
#define PERICRU_SOFTRST_CON16_OFFSET                       (0x440U)
#define PERICRU_SOFTRST_CON16_PRESETN_SARADC_SHIFT         (4U)
#define PERICRU_SOFTRST_CON16_PRESETN_SARADC_MASK          (0x1U << PERICRU_SOFTRST_CON16_PRESETN_SARADC_SHIFT)         /* 0x00000010 */
#define PERICRU_SOFTRST_CON16_RESETN_SARADC_SHIFT          (5U)
#define PERICRU_SOFTRST_CON16_RESETN_SARADC_MASK           (0x1U << PERICRU_SOFTRST_CON16_RESETN_SARADC_SHIFT)          /* 0x00000020 */
#define PERICRU_SOFTRST_CON16_RESETN_SARADC_PHY_SHIFT      (6U)
#define PERICRU_SOFTRST_CON16_RESETN_SARADC_PHY_MASK       (0x1U << PERICRU_SOFTRST_CON16_RESETN_SARADC_PHY_SHIFT)      /* 0x00000040 */
#define PERICRU_SOFTRST_CON16_PRESETN_IOC_VCCIO234_SHIFT   (12U)
#define PERICRU_SOFTRST_CON16_PRESETN_IOC_VCCIO234_MASK    (0x1U << PERICRU_SOFTRST_CON16_PRESETN_IOC_VCCIO234_SHIFT)   /* 0x00001000 */
/* SOFTRST_CON17 */
#define PERICRU_SOFTRST_CON17_OFFSET                       (0x444U)
#define PERICRU_SOFTRST_CON17_PRESETN_PERI_GPIO1_SHIFT     (0U)
#define PERICRU_SOFTRST_CON17_PRESETN_PERI_GPIO1_MASK      (0x1U << PERICRU_SOFTRST_CON17_PRESETN_PERI_GPIO1_SHIFT)     /* 0x00000001 */
#define PERICRU_SOFTRST_CON17_PRESETN_PERI_GPIO2_SHIFT     (1U)
#define PERICRU_SOFTRST_CON17_PRESETN_PERI_GPIO2_MASK      (0x1U << PERICRU_SOFTRST_CON17_PRESETN_PERI_GPIO2_SHIFT)     /* 0x00000002 */
#define PERICRU_SOFTRST_CON17_RESETN_PERI_GPIO1_SHIFT      (2U)
#define PERICRU_SOFTRST_CON17_RESETN_PERI_GPIO1_MASK       (0x1U << PERICRU_SOFTRST_CON17_RESETN_PERI_GPIO1_SHIFT)      /* 0x00000004 */
#define PERICRU_SOFTRST_CON17_RESETN_PERI_GPIO2_SHIFT      (3U)
#define PERICRU_SOFTRST_CON17_RESETN_PERI_GPIO2_MASK       (0x1U << PERICRU_SOFTRST_CON17_RESETN_PERI_GPIO2_SHIFT)      /* 0x00000008 */
/* MISC_CON0 */
#define PERICRU_MISC_CON0_OFFSET                           (0x684U)
#define PERICRU_MISC_CON0_ENABLE_RESETN_PHY2HOST_SRC_SHIFT (0U)
#define PERICRU_MISC_CON0_ENABLE_RESETN_PHY2HOST_SRC_MASK  (0x1U << PERICRU_MISC_CON0_ENABLE_RESETN_PHY2HOST_SRC_SHIFT) /* 0x00000001 */
#define PERICRU_MISC_CON0_ENABLE_USB3OTG_USB2PHY_RESET_SHIFT (1U)
#define PERICRU_MISC_CON0_ENABLE_USB3OTG_USB2PHY_RESET_MASK (0x1U << PERICRU_MISC_CON0_ENABLE_USB3OTG_USB2PHY_RESET_SHIFT) /* 0x00000002 */
#define PERICRU_MISC_CON0_ENABLE_PD_PERI_CLK_SRC_GATING_SHIFT (15U)
#define PERICRU_MISC_CON0_ENABLE_PD_PERI_CLK_SRC_GATING_MASK (0x1U << PERICRU_MISC_CON0_ENABLE_PD_PERI_CLK_SRC_GATING_SHIFT) /* 0x00008000 */
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
#define I2C_CON_DATA_UPD_ST_MASK                           (0xFU << I2C_CON_DATA_UPD_ST_SHIFT)                          /* 0x00000F00 */
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
#define SPI_CTRLR0_SM_SHIFT                                (22U)
#define SPI_CTRLR0_SM_MASK                                 (0x1U << SPI_CTRLR0_SM_SHIFT)                                /* 0x00400000 */
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
#define SPI_TXFTLR_XFTLR_SHIFT                             (0U)
#define SPI_TXFTLR_XFTLR_MASK                              (0x3FU << SPI_TXFTLR_XFTLR_SHIFT)                            /* 0x0000003F */
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
#define SPI_SR                                             (0x4CU)
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
/* VERSION */
#define SPI_VERSION_OFFSET                                 (0x48U)
#define SPI_VERSION                                        (0x110002U)
#define SPI_VERSION_VERSION_SHIFT                          (0U)
#define SPI_VERSION_VERSION_MASK                           (0xFFFFFFFFU << SPI_VERSION_VERSION_SHIFT)                   /* 0xFFFFFFFF */
/* TIMEOUT */
#define SPI_TIMEOUT_OFFSET                                 (0x4CU)
#define SPI_TIMEOUT_DTSR_SHIFT                             (18U)
#define SPI_TIMEOUT_DTSR_MASK                              (0x1U << SPI_TIMEOUT_DTSR_SHIFT)                             /* 0x00040000 */
#define SPI_TIMEOUT_TSB_SHIFT                              (0U)
#define SPI_TIMEOUT_TSB_MASK                               (0x1U << SPI_TIMEOUT_TSB_SHIFT)                              /* 0x00000001 */
#define SPI_TIMEOUT_TOE_SHIFT                              (16U)
#define SPI_TIMEOUT_TOE_MASK                               (0x1U << SPI_TIMEOUT_TOE_SHIFT)                              /* 0x00010000 */
#define SPI_TIMEOUT_TOV_SHIFT                              (0U)
#define SPI_TIMEOUT_TOV_MASK                               (0xFFFFU << SPI_TIMEOUT_TOV_SHIFT)                           /* 0x0000FFFF */
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
#define SPI_RXDR                                           (0x0U)
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
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
#define PWM_PWM0_CTRL_CLK_SRC_SEL_SHIFT                    (10U)
#define PWM_PWM0_CTRL_CLK_SRC_SEL_MASK                     (0x1U << PWM_PWM0_CTRL_CLK_SRC_SEL_SHIFT)                    /* 0x00000400 */
#define PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT                  (11U)
#define PWM_PWM0_CTRL_ALIGNED_VLD_N_MASK                   (0x1U << PWM_PWM0_CTRL_ALIGNED_VLD_N_SHIFT)                  /* 0x00000800 */
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
#define PWM_PWM1_CTRL_CLK_SRC_SEL_SHIFT                    (10U)
#define PWM_PWM1_CTRL_CLK_SRC_SEL_MASK                     (0x1U << PWM_PWM1_CTRL_CLK_SRC_SEL_SHIFT)                    /* 0x00000400 */
#define PWM_PWM1_CTRL_ALIGNED_VLD_N_SHIFT                  (11U)
#define PWM_PWM1_CTRL_ALIGNED_VLD_N_MASK                   (0x1U << PWM_PWM1_CTRL_ALIGNED_VLD_N_SHIFT)                  /* 0x00000800 */
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
#define PWM_PWM2_CTRL_CLK_SRC_SEL_SHIFT                    (10U)
#define PWM_PWM2_CTRL_CLK_SRC_SEL_MASK                     (0x1U << PWM_PWM2_CTRL_CLK_SRC_SEL_SHIFT)                    /* 0x00000400 */
#define PWM_PWM2_CTRL_ALIGNED_VLD_N_SHIFT                  (11U)
#define PWM_PWM2_CTRL_ALIGNED_VLD_N_MASK                   (0x1U << PWM_PWM2_CTRL_ALIGNED_VLD_N_SHIFT)                  /* 0x00000800 */
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
#define PWM_PWM3_CTRL_CLK_SRC_SEL_SHIFT                    (10U)
#define PWM_PWM3_CTRL_CLK_SRC_SEL_MASK                     (0x1U << PWM_PWM3_CTRL_CLK_SRC_SEL_SHIFT)                    /* 0x00000400 */
#define PWM_PWM3_CTRL_ALIGNED_VLD_N_SHIFT                  (11U)
#define PWM_PWM3_CTRL_ALIGNED_VLD_N_MASK                   (0x1U << PWM_PWM3_CTRL_ALIGNED_VLD_N_SHIFT)                  /* 0x00000800 */
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
#define PWM_VERSION_ID                                     (0x3120B34U)
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
#define PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE0_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE0_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE0_PWRKEY_MATCH_VALUE0_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE1 */
#define PWM_PWRMATCH_VALUE1_OFFSET                         (0x9CU)
#define PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE1_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE1_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE1_PWRKEY_MATCH_VALUE1_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE2 */
#define PWM_PWRMATCH_VALUE2_OFFSET                         (0xA0U)
#define PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE2_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE2_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE2_PWRKEY_MATCH_VALUE2_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE3 */
#define PWM_PWRMATCH_VALUE3_OFFSET                         (0xA4U)
#define PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE3_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE3_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE3_PWRKEY_MATCH_VALUE3_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE4 */
#define PWM_PWRMATCH_VALUE4_OFFSET                         (0xA8U)
#define PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE4_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE4_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE4_PWRKEY_MATCH_VALUE4_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE5 */
#define PWM_PWRMATCH_VALUE5_OFFSET                         (0xACU)
#define PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE5_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE5_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE5_PWRKEY_MATCH_VALUE5_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE6 */
#define PWM_PWRMATCH_VALUE6_OFFSET                         (0xB0U)
#define PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE6_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE6_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE6_PWRKEY_MATCH_VALUE6_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE7 */
#define PWM_PWRMATCH_VALUE7_OFFSET                         (0xB4U)
#define PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE7_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE7_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE7_PWRKEY_MATCH_VALUE7_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE8 */
#define PWM_PWRMATCH_VALUE8_OFFSET                         (0xB8U)
#define PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE8_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE8_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE8_PWRKEY_MATCH_VALUE8_SHIFT) /* 0xFFFFFFFF */
/* PWRMATCH_VALUE9 */
#define PWM_PWRMATCH_VALUE9_OFFSET                         (0xBCU)
#define PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE9_SHIFT      (0U)
#define PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE9_MASK       (0xFFFFFFFFU << PWM_PWRMATCH_VALUE9_PWRKEY_MATCH_VALUE9_SHIFT) /* 0xFFFFFFFF */
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
#define PWM_FILTER_CTRL_CH0_AND_CH3_SWITCH_EN_SHIFT        (16U)
#define PWM_FILTER_CTRL_CH0_AND_CH3_SWITCH_EN_MASK         (0x1U << PWM_FILTER_CTRL_CH0_AND_CH3_SWITCH_EN_SHIFT)        /* 0x00010000 */
#define PWM_FILTER_CTRL_CH1_AND_CH3_SWITCH_EN_SHIFT        (17U)
#define PWM_FILTER_CTRL_CH1_AND_CH3_SWITCH_EN_MASK         (0x1U << PWM_FILTER_CTRL_CH1_AND_CH3_SWITCH_EN_SHIFT)        /* 0x00020000 */
#define PWM_FILTER_CTRL_CH2_AND_CH3_SWITCH_EN_SHIFT        (18U)
#define PWM_FILTER_CTRL_CH2_AND_CH3_SWITCH_EN_MASK         (0x1U << PWM_FILTER_CTRL_CH2_AND_CH3_SWITCH_EN_SHIFT)        /* 0x00040000 */
#define PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_SHIFT             (20U)
#define PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_MASK              (0x1U << PWM_FILTER_CTRL_PWM0_GLOBAL_LOCK_SHIFT)             /* 0x00100000 */
#define PWM_FILTER_CTRL_PWM1_GLOBAL_LOCK_SHIFT             (21U)
#define PWM_FILTER_CTRL_PWM1_GLOBAL_LOCK_MASK              (0x1U << PWM_FILTER_CTRL_PWM1_GLOBAL_LOCK_SHIFT)             /* 0x00200000 */
#define PWM_FILTER_CTRL_PWM2_GLOBAL_LOCK_SHIFT             (22U)
#define PWM_FILTER_CTRL_PWM2_GLOBAL_LOCK_MASK              (0x1U << PWM_FILTER_CTRL_PWM2_GLOBAL_LOCK_SHIFT)             /* 0x00400000 */
#define PWM_FILTER_CTRL_PWM3_GLOBAL_LOCK_SHIFT             (23U)
#define PWM_FILTER_CTRL_PWM3_GLOBAL_LOCK_MASK              (0x1U << PWM_FILTER_CTRL_PWM3_GLOBAL_LOCK_SHIFT)             /* 0x00800000 */
/* PWM0_OFFSET */
#define PWM_PWM0_OFFSET_OFFSET                             (0xE0U)
#define PWM_PWM0_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT        (0U)
#define PWM_PWM0_OFFSET_CHANNEL_OUTPUT_OFFSET_MASK         (0xFFFFFFFFU << PWM_PWM0_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT) /* 0xFFFFFFFF */
/* PWM1_OFFSET */
#define PWM_PWM1_OFFSET_OFFSET                             (0xE4U)
#define PWM_PWM1_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT        (0U)
#define PWM_PWM1_OFFSET_CHANNEL_OUTPUT_OFFSET_MASK         (0xFFFFFFFFU << PWM_PWM1_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT) /* 0xFFFFFFFF */
/* PWM2_OFFSET */
#define PWM_PWM2_OFFSET_OFFSET                             (0xE8U)
#define PWM_PWM2_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT        (0U)
#define PWM_PWM2_OFFSET_CHANNEL_OUTPUT_OFFSET_MASK         (0xFFFFFFFFU << PWM_PWM2_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT) /* 0xFFFFFFFF */
/* PWM3_OFFSET */
#define PWM_PWM3_OFFSET_OFFSET                             (0xECU)
#define PWM_PWM3_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT        (0U)
#define PWM_PWM3_OFFSET_CHANNEL_OUTPUT_OFFSET_MASK         (0xFFFFFFFFU << PWM_PWM3_OFFSET_CHANNEL_OUTPUT_OFFSET_SHIFT) /* 0xFFFFFFFF */
/* IR_TRANS_CTRL */
#define PWM_IR_TRANS_CTRL_OFFSET                           (0x100U)
#define PWM_IR_TRANS_CTRL_IR_TRANS_OUT_ENABLE_SHIFT        (0U)
#define PWM_IR_TRANS_CTRL_IR_TRANS_OUT_ENABLE_MASK         (0x1U << PWM_IR_TRANS_CTRL_IR_TRANS_OUT_ENABLE_SHIFT)        /* 0x00000001 */
#define PWM_IR_TRANS_CTRL_IR_STATE_IDLE_SHIFT              (1U)
#define PWM_IR_TRANS_CTRL_IR_STATE_IDLE_MASK               (0x1U << PWM_IR_TRANS_CTRL_IR_STATE_IDLE_SHIFT)              /* 0x00000002 */
#define PWM_IR_TRANS_CTRL_IR_TRANS_FORMAT_SHIFT            (4U)
#define PWM_IR_TRANS_CTRL_IR_TRANS_FORMAT_MASK             (0xFU << PWM_IR_TRANS_CTRL_IR_TRANS_FORMAT_SHIFT)            /* 0x000000F0 */
#define PWM_IR_TRANS_CTRL_IR_TRANS_LENGTH_WITHIN_ONE_FRAME_SHIFT (8U)
#define PWM_IR_TRANS_CTRL_IR_TRANS_LENGTH_WITHIN_ONE_FRAME_MASK (0x1FU << PWM_IR_TRANS_CTRL_IR_TRANS_LENGTH_WITHIN_ONE_FRAME_SHIFT) /* 0x00001F00 */
/* IR_TRANS_PRE */
#define PWM_IR_TRANS_PRE_OFFSET                            (0x104U)
#define PWM_IR_TRANS_PRE_IR_TRANS_OUT_LOW_PRELOAD_SHIFT    (0U)
#define PWM_IR_TRANS_PRE_IR_TRANS_OUT_LOW_PRELOAD_MASK     (0xFFFFU << PWM_IR_TRANS_PRE_IR_TRANS_OUT_LOW_PRELOAD_SHIFT) /* 0x0000FFFF */
#define PWM_IR_TRANS_PRE_IR_TRANS_OUT_HIGH_PRELOAD_SHIFT   (16U)
#define PWM_IR_TRANS_PRE_IR_TRANS_OUT_HIGH_PRELOAD_MASK    (0xFFFFU << PWM_IR_TRANS_PRE_IR_TRANS_OUT_HIGH_PRELOAD_SHIFT) /* 0xFFFF0000 */
/* IR_TRANS_SPRE */
#define PWM_IR_TRANS_SPRE_OFFSET                           (0x108U)
#define PWM_IR_TRANS_SPRE_IR_TRANS_OUT_LOW_SIMPLE_PRELOAD_SHIFT (0U)
#define PWM_IR_TRANS_SPRE_IR_TRANS_OUT_LOW_SIMPLE_PRELOAD_MASK (0xFFFFU << PWM_IR_TRANS_SPRE_IR_TRANS_OUT_LOW_SIMPLE_PRELOAD_SHIFT) /* 0x0000FFFF */
/* IR_TRANS_LD */
#define PWM_IR_TRANS_LD_OFFSET                             (0x10CU)
#define PWM_IR_TRANS_LD_IR_TRANS_OUT_DATA_LOW_PERIOD_SHIFT (0U)
#define PWM_IR_TRANS_LD_IR_TRANS_OUT_DATA_LOW_PERIOD_MASK  (0xFFFFU << PWM_IR_TRANS_LD_IR_TRANS_OUT_DATA_LOW_PERIOD_SHIFT) /* 0x0000FFFF */
/* IR_TRANS_HD */
#define PWM_IR_TRANS_HD_OFFSET                             (0x110U)
#define PWM_IR_TRANS_HD_IR_TRANS_OUT_HIGH_PERIOD_FOR_ZERO_SHIFT (0U)
#define PWM_IR_TRANS_HD_IR_TRANS_OUT_HIGH_PERIOD_FOR_ZERO_MASK (0xFFFFU << PWM_IR_TRANS_HD_IR_TRANS_OUT_HIGH_PERIOD_FOR_ZERO_SHIFT) /* 0x0000FFFF */
#define PWM_IR_TRANS_HD_IR_TRANS_OUT_HIGH_PERIOD_FOR_ONE_SHIFT (16U)
#define PWM_IR_TRANS_HD_IR_TRANS_OUT_HIGH_PERIOD_FOR_ONE_MASK (0xFFFFU << PWM_IR_TRANS_HD_IR_TRANS_OUT_HIGH_PERIOD_FOR_ONE_SHIFT) /* 0xFFFF0000 */
/* IR_TRANS_BURST_FRAME */
#define PWM_IR_TRANS_BURST_FRAME_OFFSET                    (0x114U)
#define PWM_IR_TRANS_BURST_FRAME_IR_TRANS_OUT_FRAME_PERIOD_SHIFT (0U)
#define PWM_IR_TRANS_BURST_FRAME_IR_TRANS_OUT_FRAME_PERIOD_MASK (0x3FFFFU << PWM_IR_TRANS_BURST_FRAME_IR_TRANS_OUT_FRAME_PERIOD_SHIFT) /* 0x0003FFFF */
#define PWM_IR_TRANS_BURST_FRAME_IR_TRANS_OUT_BURST_PERIOD_SHIFT (20U)
#define PWM_IR_TRANS_BURST_FRAME_IR_TRANS_OUT_BURST_PERIOD_MASK (0x3FFU << PWM_IR_TRANS_BURST_FRAME_IR_TRANS_OUT_BURST_PERIOD_SHIFT) /* 0x3FF00000 */
/* IR_TRANS_DATA_VALUE */
#define PWM_IR_TRANS_DATA_VALUE_OFFSET                     (0x118U)
#define PWM_IR_TRANS_DATA_VALUE_IR_TRANS_OUT_VALUE_SHIFT   (0U)
#define PWM_IR_TRANS_DATA_VALUE_IR_TRANS_OUT_VALUE_MASK    (0xFFFFFFFFU << PWM_IR_TRANS_DATA_VALUE_IR_TRANS_OUT_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWM0_CAPTURE_CNT_EN */
#define PWM_PWM0_CAPTURE_CNT_EN_OFFSET                     (0x120U)
#define PWM_PWM0_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT       (0U)
#define PWM_PWM0_CAPTURE_CNT_EN_POS_CAPTURE_EN_MASK        (0x1U << PWM_PWM0_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT)       /* 0x00000001 */
#define PWM_PWM0_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT       (1U)
#define PWM_PWM0_CAPTURE_CNT_EN_NEG_CAPTURE_EN_MASK        (0x1U << PWM_PWM0_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT)       /* 0x00000002 */
/* PWM1_CAPTURE_CNT_EN */
#define PWM_PWM1_CAPTURE_CNT_EN_OFFSET                     (0x124U)
#define PWM_PWM1_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT       (0U)
#define PWM_PWM1_CAPTURE_CNT_EN_POS_CAPTURE_EN_MASK        (0x1U << PWM_PWM1_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT)       /* 0x00000001 */
#define PWM_PWM1_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT       (1U)
#define PWM_PWM1_CAPTURE_CNT_EN_NEG_CAPTURE_EN_MASK        (0x1U << PWM_PWM1_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT)       /* 0x00000002 */
/* PWM2_CAPTURE_CNT_EN */
#define PWM_PWM2_CAPTURE_CNT_EN_OFFSET                     (0x128U)
#define PWM_PWM2_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT       (0U)
#define PWM_PWM2_CAPTURE_CNT_EN_POS_CAPTURE_EN_MASK        (0x1U << PWM_PWM2_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT)       /* 0x00000001 */
#define PWM_PWM2_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT       (1U)
#define PWM_PWM2_CAPTURE_CNT_EN_NEG_CAPTURE_EN_MASK        (0x1U << PWM_PWM2_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT)       /* 0x00000002 */
/* PWM3_CAPTURE_CNT_EN */
#define PWM_PWM3_CAPTURE_CNT_EN_OFFSET                     (0x12CU)
#define PWM_PWM3_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT       (0U)
#define PWM_PWM3_CAPTURE_CNT_EN_POS_CAPTURE_EN_MASK        (0x1U << PWM_PWM3_CAPTURE_CNT_EN_POS_CAPTURE_EN_SHIFT)       /* 0x00000001 */
#define PWM_PWM3_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT       (1U)
#define PWM_PWM3_CAPTURE_CNT_EN_NEG_CAPTURE_EN_MASK        (0x1U << PWM_PWM3_CAPTURE_CNT_EN_NEG_CAPTURE_EN_SHIFT)       /* 0x00000002 */
/* PWM0_POS_CNT */
#define PWM_PWM0_POS_CNT_OFFSET                            (0x130U)
#define PWM_PWM0_POS_CNT_POS_CNT_SHIFT                     (0U)
#define PWM_PWM0_POS_CNT_POS_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM0_POS_CNT_POS_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM1_POS_CNT */
#define PWM_PWM1_POS_CNT_OFFSET                            (0x134U)
#define PWM_PWM1_POS_CNT_POS_CNT_SHIFT                     (0U)
#define PWM_PWM1_POS_CNT_POS_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM1_POS_CNT_POS_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM2_POS_CNT */
#define PWM_PWM2_POS_CNT_OFFSET                            (0x138U)
#define PWM_PWM2_POS_CNT_POS_CNT_SHIFT                     (0U)
#define PWM_PWM2_POS_CNT_POS_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM2_POS_CNT_POS_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM3_POS_CNT */
#define PWM_PWM3_POS_CNT_OFFSET                            (0x13CU)
#define PWM_PWM3_POS_CNT_POS_CNT_SHIFT                     (0U)
#define PWM_PWM3_POS_CNT_POS_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM3_POS_CNT_POS_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM0_NEG_CNT */
#define PWM_PWM0_NEG_CNT_OFFSET                            (0x140U)
#define PWM_PWM0_NEG_CNT_NEG_CNT_SHIFT                     (0U)
#define PWM_PWM0_NEG_CNT_NEG_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM0_NEG_CNT_NEG_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM1_NEG_CNT */
#define PWM_PWM1_NEG_CNT_OFFSET                            (0x144U)
#define PWM_PWM1_NEG_CNT_NEG_CNT_SHIFT                     (0U)
#define PWM_PWM1_NEG_CNT_NEG_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM1_NEG_CNT_NEG_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM2_NEG_CNT */
#define PWM_PWM2_NEG_CNT_OFFSET                            (0x148U)
#define PWM_PWM2_NEG_CNT_NEG_CNT_SHIFT                     (0U)
#define PWM_PWM2_NEG_CNT_NEG_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM2_NEG_CNT_NEG_CNT_SHIFT)              /* 0xFFFFFFFF */
/* PWM3_NEG_CNT */
#define PWM_PWM3_NEG_CNT_OFFSET                            (0x14CU)
#define PWM_PWM3_NEG_CNT_NEG_CNT_SHIFT                     (0U)
#define PWM_PWM3_NEG_CNT_NEG_CNT_MASK                      (0xFFFFFFFFU << PWM_PWM3_NEG_CNT_NEG_CNT_SHIFT)              /* 0xFFFFFFFF */
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
#define GPIO_VER_ID                                        (0x101157CU)
#define GPIO_VER_ID_VER_ID_SHIFT                           (0U)
#define GPIO_VER_ID_VER_ID_MASK                            (0xFFFFFFFFU << GPIO_VER_ID_VER_ID_SHIFT)                    /* 0xFFFFFFFF */
/* GPIO_REG_GROUP_L */
#define GPIO_GPIO_REG_GROUP_L_OFFSET                       (0x100U)
#define GPIO_GPIO_REG_GROUP_L_GPIO_REG_GROUP_LOW_SHIFT     (0U)
#define GPIO_GPIO_REG_GROUP_L_GPIO_REG_GROUP_LOW_MASK      (0xFFFFU << GPIO_GPIO_REG_GROUP_L_GPIO_REG_GROUP_LOW_SHIFT)  /* 0x0000FFFF */
/* GPIO_REG_GROUP_H */
#define GPIO_GPIO_REG_GROUP_H_OFFSET                       (0x104U)
#define GPIO_GPIO_REG_GROUP_H_GPIO_REG_GROUP_HIGH_SHIFT    (0U)
#define GPIO_GPIO_REG_GROUP_H_GPIO_REG_GROUP_HIGH_MASK     (0xFFFFU << GPIO_GPIO_REG_GROUP_H_GPIO_REG_GROUP_HIGH_SHIFT) /* 0x0000FFFF */
/* GPIO_VIRTUAL_EN */
#define GPIO_GPIO_VIRTUAL_EN_OFFSET                        (0x108U)
#define GPIO_GPIO_VIRTUAL_EN_GPIO_VIRTUAL_EN_SHIFT         (0U)
#define GPIO_GPIO_VIRTUAL_EN_GPIO_VIRTUAL_EN_MASK          (0x1U << GPIO_GPIO_VIRTUAL_EN_GPIO_VIRTUAL_EN_SHIFT)         /* 0x00000001 */
/*****************************************SARADC*****************************************/
/* CONV_CON */
#define SARADC_CONV_CON_OFFSET                             (0x0U)
#define SARADC_CONV_CON_CHANNEL_SEL_SHIFT                  (0U)
#define SARADC_CONV_CON_CHANNEL_SEL_MASK                   (0xFU << SARADC_CONV_CON_CHANNEL_SEL_SHIFT)                  /* 0x0000000F */
#define SARADC_CONV_CON_START_ADC_SHIFT                    (4U)
#define SARADC_CONV_CON_START_ADC_MASK                     (0x1U << SARADC_CONV_CON_START_ADC_SHIFT)                    /* 0x00000010 */
#define SARADC_CONV_CON_SINGLE_PD_MODE_SHIFT               (5U)
#define SARADC_CONV_CON_SINGLE_PD_MODE_MASK                (0x1U << SARADC_CONV_CON_SINGLE_PD_MODE_SHIFT)               /* 0x00000020 */
#define SARADC_CONV_CON_AUTO_CHANNEL_MODE_SHIFT            (6U)
#define SARADC_CONV_CON_AUTO_CHANNEL_MODE_MASK             (0x1U << SARADC_CONV_CON_AUTO_CHANNEL_MODE_SHIFT)            /* 0x00000040 */
#define SARADC_CONV_CON_END_CONV_SHIFT                     (7U)
#define SARADC_CONV_CON_END_CONV_MASK                      (0x1U << SARADC_CONV_CON_END_CONV_SHIFT)                     /* 0x00000080 */
#define SARADC_CONV_CON_AS_PD_MODE_SHIFT                   (8U)
#define SARADC_CONV_CON_AS_PD_MODE_MASK                    (0x1U << SARADC_CONV_CON_AS_PD_MODE_SHIFT)                   /* 0x00000100 */
#define SARADC_CONV_CON_INT_LOCK_SHIFT                     (9U)
#define SARADC_CONV_CON_INT_LOCK_MASK                      (0x1U << SARADC_CONV_CON_INT_LOCK_SHIFT)                     /* 0x00000200 */
/* T_PD_SOC */
#define SARADC_T_PD_SOC_OFFSET                             (0x4U)
#define SARADC_T_PD_SOC_T_PD_SOC_SHIFT                     (13U)
#define SARADC_T_PD_SOC_T_PD_SOC_MASK                      (0xFFU << SARADC_T_PD_SOC_T_PD_SOC_SHIFT)                    /* 0x001FE000 */
/* T_AS_SOC */
#define SARADC_T_AS_SOC_OFFSET                             (0x8U)
#define SARADC_T_AS_SOC_T_AS_SOC_SHIFT                     (0U)
#define SARADC_T_AS_SOC_T_AS_SOC_MASK                      (0xFFFFFFFFU << SARADC_T_AS_SOC_T_AS_SOC_SHIFT)              /* 0xFFFFFFFF */
/* T_DAS_SOC */
#define SARADC_T_DAS_SOC_OFFSET                            (0xCU)
#define SARADC_T_DAS_SOC_T_DAS_SOC_SHIFT                   (0U)
#define SARADC_T_DAS_SOC_T_DAS_SOC_MASK                    (0xFFFFFFFFU << SARADC_T_DAS_SOC_T_DAS_SOC_SHIFT)            /* 0xFFFFFFFF */
/* T_SEL_SOC */
#define SARADC_T_SEL_SOC_OFFSET                            (0x10U)
#define SARADC_T_SEL_SOC_T_SEL_SOC_SHIFT                   (0U)
#define SARADC_T_SEL_SOC_T_SEL_SOC_MASK                    (0xFFFFU << SARADC_T_SEL_SOC_T_SEL_SOC_SHIFT)                /* 0x0000FFFF */
/* HIGH_COMP0 */
#define SARADC_HIGH_COMP0_OFFSET                           (0x14U)
#define SARADC_HIGH_COMP0_HIGH_COMP0_SHIFT                 (0U)
#define SARADC_HIGH_COMP0_HIGH_COMP0_MASK                  (0xFFFU << SARADC_HIGH_COMP0_HIGH_COMP0_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP1 */
#define SARADC_HIGH_COMP1_OFFSET                           (0x18U)
#define SARADC_HIGH_COMP1_HIGH_COMP1_SHIFT                 (0U)
#define SARADC_HIGH_COMP1_HIGH_COMP1_MASK                  (0xFFFU << SARADC_HIGH_COMP1_HIGH_COMP1_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP2 */
#define SARADC_HIGH_COMP2_OFFSET                           (0x1CU)
#define SARADC_HIGH_COMP2_HIGH_COMP2_SHIFT                 (0U)
#define SARADC_HIGH_COMP2_HIGH_COMP2_MASK                  (0xFFFU << SARADC_HIGH_COMP2_HIGH_COMP2_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP3 */
#define SARADC_HIGH_COMP3_OFFSET                           (0x20U)
#define SARADC_HIGH_COMP3_HIGH_COMP3_SHIFT                 (0U)
#define SARADC_HIGH_COMP3_HIGH_COMP3_MASK                  (0xFFFU << SARADC_HIGH_COMP3_HIGH_COMP3_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP4 */
#define SARADC_HIGH_COMP4_OFFSET                           (0x24U)
#define SARADC_HIGH_COMP4_HIGH_COMP4_SHIFT                 (0U)
#define SARADC_HIGH_COMP4_HIGH_COMP4_MASK                  (0xFFFU << SARADC_HIGH_COMP4_HIGH_COMP4_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP5 */
#define SARADC_HIGH_COMP5_OFFSET                           (0x28U)
#define SARADC_HIGH_COMP5_HIGH_COMP5_SHIFT                 (0U)
#define SARADC_HIGH_COMP5_HIGH_COMP5_MASK                  (0xFFFU << SARADC_HIGH_COMP5_HIGH_COMP5_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP6 */
#define SARADC_HIGH_COMP6_OFFSET                           (0x2CU)
#define SARADC_HIGH_COMP6_HIGH_COMP6_SHIFT                 (0U)
#define SARADC_HIGH_COMP6_HIGH_COMP6_MASK                  (0xFFFU << SARADC_HIGH_COMP6_HIGH_COMP6_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP7 */
#define SARADC_HIGH_COMP7_OFFSET                           (0x30U)
#define SARADC_HIGH_COMP7_HIGH_COMP7_SHIFT                 (0U)
#define SARADC_HIGH_COMP7_HIGH_COMP7_MASK                  (0xFFFU << SARADC_HIGH_COMP7_HIGH_COMP7_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP8 */
#define SARADC_HIGH_COMP8_OFFSET                           (0x34U)
#define SARADC_HIGH_COMP8_HIGH_COMP8_SHIFT                 (0U)
#define SARADC_HIGH_COMP8_HIGH_COMP8_MASK                  (0xFFFU << SARADC_HIGH_COMP8_HIGH_COMP8_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP9 */
#define SARADC_HIGH_COMP9_OFFSET                           (0x38U)
#define SARADC_HIGH_COMP9_HIGH_COMP9_SHIFT                 (0U)
#define SARADC_HIGH_COMP9_HIGH_COMP9_MASK                  (0xFFFU << SARADC_HIGH_COMP9_HIGH_COMP9_SHIFT)               /* 0x00000FFF */
/* HIGH_COMP10 */
#define SARADC_HIGH_COMP10_OFFSET                          (0x3CU)
#define SARADC_HIGH_COMP10_HIGH_COMP10_SHIFT               (0U)
#define SARADC_HIGH_COMP10_HIGH_COMP10_MASK                (0xFFFU << SARADC_HIGH_COMP10_HIGH_COMP10_SHIFT)             /* 0x00000FFF */
/* HIGH_COMP11 */
#define SARADC_HIGH_COMP11_OFFSET                          (0x40U)
#define SARADC_HIGH_COMP11_HIGH_COMP11_SHIFT               (0U)
#define SARADC_HIGH_COMP11_HIGH_COMP11_MASK                (0xFFFU << SARADC_HIGH_COMP11_HIGH_COMP11_SHIFT)             /* 0x00000FFF */
/* HIGH_COMP12 */
#define SARADC_HIGH_COMP12_OFFSET                          (0x44U)
#define SARADC_HIGH_COMP12_HIGH_COMP12_SHIFT               (0U)
#define SARADC_HIGH_COMP12_HIGH_COMP12_MASK                (0xFFFU << SARADC_HIGH_COMP12_HIGH_COMP12_SHIFT)             /* 0x00000FFF */
/* HIGH_COMP13 */
#define SARADC_HIGH_COMP13_OFFSET                          (0x48U)
#define SARADC_HIGH_COMP13_HIGH_COMP13_SHIFT               (0U)
#define SARADC_HIGH_COMP13_HIGH_COMP13_MASK                (0xFFFU << SARADC_HIGH_COMP13_HIGH_COMP13_SHIFT)             /* 0x00000FFF */
/* HIGH_COMP14 */
#define SARADC_HIGH_COMP14_OFFSET                          (0x4CU)
#define SARADC_HIGH_COMP14_HIGH_COMP14_SHIFT               (0U)
#define SARADC_HIGH_COMP14_HIGH_COMP14_MASK                (0xFFFU << SARADC_HIGH_COMP14_HIGH_COMP14_SHIFT)             /* 0x00000FFF */
/* HIGH_COMP15 */
#define SARADC_HIGH_COMP15_OFFSET                          (0x50U)
#define SARADC_HIGH_COMP15_HIGH_COMP15_SHIFT               (0U)
#define SARADC_HIGH_COMP15_HIGH_COMP15_MASK                (0xFFFU << SARADC_HIGH_COMP15_HIGH_COMP15_SHIFT)             /* 0x00000FFF */
/* LOW_COMP0 */
#define SARADC_LOW_COMP0_OFFSET                            (0x54U)
#define SARADC_LOW_COMP0_LOW_COMP0_SHIFT                   (0U)
#define SARADC_LOW_COMP0_LOW_COMP0_MASK                    (0xFFFU << SARADC_LOW_COMP0_LOW_COMP0_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP1 */
#define SARADC_LOW_COMP1_OFFSET                            (0x58U)
#define SARADC_LOW_COMP1_LOW_COMP1_SHIFT                   (0U)
#define SARADC_LOW_COMP1_LOW_COMP1_MASK                    (0xFFFU << SARADC_LOW_COMP1_LOW_COMP1_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP2 */
#define SARADC_LOW_COMP2_OFFSET                            (0x5CU)
#define SARADC_LOW_COMP2_LOW_COMP2_SHIFT                   (0U)
#define SARADC_LOW_COMP2_LOW_COMP2_MASK                    (0xFFFU << SARADC_LOW_COMP2_LOW_COMP2_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP3 */
#define SARADC_LOW_COMP3_OFFSET                            (0x60U)
#define SARADC_LOW_COMP3_LOW_COMP3_SHIFT                   (0U)
#define SARADC_LOW_COMP3_LOW_COMP3_MASK                    (0xFFFU << SARADC_LOW_COMP3_LOW_COMP3_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP4 */
#define SARADC_LOW_COMP4_OFFSET                            (0x64U)
#define SARADC_LOW_COMP4_LOW_COMP4_SHIFT                   (0U)
#define SARADC_LOW_COMP4_LOW_COMP4_MASK                    (0xFFFU << SARADC_LOW_COMP4_LOW_COMP4_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP5 */
#define SARADC_LOW_COMP5_OFFSET                            (0x68U)
#define SARADC_LOW_COMP5_LOW_COMP5_SHIFT                   (0U)
#define SARADC_LOW_COMP5_LOW_COMP5_MASK                    (0xFFFU << SARADC_LOW_COMP5_LOW_COMP5_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP6 */
#define SARADC_LOW_COMP6_OFFSET                            (0x6CU)
#define SARADC_LOW_COMP6_LOW_COMP6_SHIFT                   (0U)
#define SARADC_LOW_COMP6_LOW_COMP6_MASK                    (0xFFFU << SARADC_LOW_COMP6_LOW_COMP6_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP7 */
#define SARADC_LOW_COMP7_OFFSET                            (0x70U)
#define SARADC_LOW_COMP7_LOW_COMP7_SHIFT                   (0U)
#define SARADC_LOW_COMP7_LOW_COMP7_MASK                    (0xFFFU << SARADC_LOW_COMP7_LOW_COMP7_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP8 */
#define SARADC_LOW_COMP8_OFFSET                            (0x74U)
#define SARADC_LOW_COMP8_LOW_COMP8_SHIFT                   (0U)
#define SARADC_LOW_COMP8_LOW_COMP8_MASK                    (0xFFFU << SARADC_LOW_COMP8_LOW_COMP8_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP9 */
#define SARADC_LOW_COMP9_OFFSET                            (0x78U)
#define SARADC_LOW_COMP9_LOW_COMP9_SHIFT                   (0U)
#define SARADC_LOW_COMP9_LOW_COMP9_MASK                    (0xFFFU << SARADC_LOW_COMP9_LOW_COMP9_SHIFT)                 /* 0x00000FFF */
/* LOW_COMP10 */
#define SARADC_LOW_COMP10_OFFSET                           (0x7CU)
#define SARADC_LOW_COMP10_LOW_COMP10_SHIFT                 (0U)
#define SARADC_LOW_COMP10_LOW_COMP10_MASK                  (0x1U << SARADC_LOW_COMP10_LOW_COMP10_SHIFT)                 /* 0x00000001 */
/* LOW_COMP11 */
#define SARADC_LOW_COMP11_OFFSET                           (0x80U)
#define SARADC_LOW_COMP11_LOW_COMP11_SHIFT                 (0U)
#define SARADC_LOW_COMP11_LOW_COMP11_MASK                  (0xFFFU << SARADC_LOW_COMP11_LOW_COMP11_SHIFT)               /* 0x00000FFF */
/* LOW_COMP12 */
#define SARADC_LOW_COMP12_OFFSET                           (0x84U)
#define SARADC_LOW_COMP12_LOW_COMP12_SHIFT                 (0U)
#define SARADC_LOW_COMP12_LOW_COMP12_MASK                  (0xFFFU << SARADC_LOW_COMP12_LOW_COMP12_SHIFT)               /* 0x00000FFF */
/* LOW_COMP13 */
#define SARADC_LOW_COMP13_OFFSET                           (0x88U)
#define SARADC_LOW_COMP13_LOW_COMP13_SHIFT                 (0U)
#define SARADC_LOW_COMP13_LOW_COMP13_MASK                  (0x1U << SARADC_LOW_COMP13_LOW_COMP13_SHIFT)                 /* 0x00000001 */
/* LOW_COMP14 */
#define SARADC_LOW_COMP14_OFFSET                           (0x8CU)
#define SARADC_LOW_COMP14_LOW_COMP14_SHIFT                 (0U)
#define SARADC_LOW_COMP14_LOW_COMP14_MASK                  (0xFFFU << SARADC_LOW_COMP14_LOW_COMP14_SHIFT)               /* 0x00000FFF */
/* LOW_COMP15 */
#define SARADC_LOW_COMP15_OFFSET                           (0x90U)
#define SARADC_LOW_COMP15_LOW_COMP15_SHIFT                 (0U)
#define SARADC_LOW_COMP15_LOW_COMP15_MASK                  (0xFFFU << SARADC_LOW_COMP15_LOW_COMP15_SHIFT)               /* 0x00000FFF */
/* DEBOUNCE */
#define SARADC_DEBOUNCE_OFFSET                             (0x94U)
#define SARADC_DEBOUNCE_DEBOUNCE_SHIFT                     (0U)
#define SARADC_DEBOUNCE_DEBOUNCE_MASK                      (0xFFU << SARADC_DEBOUNCE_DEBOUNCE_SHIFT)                    /* 0x000000FF */
/* HT_INT_EN */
#define SARADC_HT_INT_EN_OFFSET                            (0x98U)
#define SARADC_HT_INT_EN_HT_INT_EN0_SHIFT                  (0U)
#define SARADC_HT_INT_EN_HT_INT_EN0_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN0_SHIFT)                  /* 0x00000001 */
#define SARADC_HT_INT_EN_HT_INT_EN1_SHIFT                  (1U)
#define SARADC_HT_INT_EN_HT_INT_EN1_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN1_SHIFT)                  /* 0x00000002 */
#define SARADC_HT_INT_EN_HT_INT_EN2_SHIFT                  (2U)
#define SARADC_HT_INT_EN_HT_INT_EN2_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN2_SHIFT)                  /* 0x00000004 */
#define SARADC_HT_INT_EN_HT_INT_EN3_SHIFT                  (3U)
#define SARADC_HT_INT_EN_HT_INT_EN3_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN3_SHIFT)                  /* 0x00000008 */
#define SARADC_HT_INT_EN_HT_INT_EN4_SHIFT                  (4U)
#define SARADC_HT_INT_EN_HT_INT_EN4_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN4_SHIFT)                  /* 0x00000010 */
#define SARADC_HT_INT_EN_HT_INT_EN5_SHIFT                  (5U)
#define SARADC_HT_INT_EN_HT_INT_EN5_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN5_SHIFT)                  /* 0x00000020 */
#define SARADC_HT_INT_EN_HT_INT_EN6_SHIFT                  (6U)
#define SARADC_HT_INT_EN_HT_INT_EN6_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN6_SHIFT)                  /* 0x00000040 */
#define SARADC_HT_INT_EN_HT_INT_EN7_SHIFT                  (7U)
#define SARADC_HT_INT_EN_HT_INT_EN7_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN7_SHIFT)                  /* 0x00000080 */
#define SARADC_HT_INT_EN_HT_INT_EN8_SHIFT                  (8U)
#define SARADC_HT_INT_EN_HT_INT_EN8_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN8_SHIFT)                  /* 0x00000100 */
#define SARADC_HT_INT_EN_HT_INT_EN9_SHIFT                  (9U)
#define SARADC_HT_INT_EN_HT_INT_EN9_MASK                   (0x1U << SARADC_HT_INT_EN_HT_INT_EN9_SHIFT)                  /* 0x00000200 */
#define SARADC_HT_INT_EN_HT_INT_EN10_SHIFT                 (10U)
#define SARADC_HT_INT_EN_HT_INT_EN10_MASK                  (0x1U << SARADC_HT_INT_EN_HT_INT_EN10_SHIFT)                 /* 0x00000400 */
#define SARADC_HT_INT_EN_HT_INT_EN11_SHIFT                 (11U)
#define SARADC_HT_INT_EN_HT_INT_EN11_MASK                  (0x1U << SARADC_HT_INT_EN_HT_INT_EN11_SHIFT)                 /* 0x00000800 */
#define SARADC_HT_INT_EN_HT_INT_EN12_SHIFT                 (12U)
#define SARADC_HT_INT_EN_HT_INT_EN12_MASK                  (0x1U << SARADC_HT_INT_EN_HT_INT_EN12_SHIFT)                 /* 0x00001000 */
#define SARADC_HT_INT_EN_HT_INT_EN13_SHIFT                 (13U)
#define SARADC_HT_INT_EN_HT_INT_EN13_MASK                  (0x1U << SARADC_HT_INT_EN_HT_INT_EN13_SHIFT)                 /* 0x00002000 */
#define SARADC_HT_INT_EN_HT_INT_EN14_SHIFT                 (14U)
#define SARADC_HT_INT_EN_HT_INT_EN14_MASK                  (0x1U << SARADC_HT_INT_EN_HT_INT_EN14_SHIFT)                 /* 0x00004000 */
#define SARADC_HT_INT_EN_HT_INT_EN15_SHIFT                 (15U)
#define SARADC_HT_INT_EN_HT_INT_EN15_MASK                  (0x1U << SARADC_HT_INT_EN_HT_INT_EN15_SHIFT)                 /* 0x00008000 */
/* LT_INT_EN */
#define SARADC_LT_INT_EN_OFFSET                            (0x9CU)
#define SARADC_LT_INT_EN_LT_INT_EN0_SHIFT                  (0U)
#define SARADC_LT_INT_EN_LT_INT_EN0_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN0_SHIFT)                  /* 0x00000001 */
#define SARADC_LT_INT_EN_LT_INT_EN1_SHIFT                  (1U)
#define SARADC_LT_INT_EN_LT_INT_EN1_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN1_SHIFT)                  /* 0x00000002 */
#define SARADC_LT_INT_EN_LT_INT_EN2_SHIFT                  (2U)
#define SARADC_LT_INT_EN_LT_INT_EN2_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN2_SHIFT)                  /* 0x00000004 */
#define SARADC_LT_INT_EN_LT_INT_EN3_SHIFT                  (3U)
#define SARADC_LT_INT_EN_LT_INT_EN3_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN3_SHIFT)                  /* 0x00000008 */
#define SARADC_LT_INT_EN_LT_INT_EN4_SHIFT                  (4U)
#define SARADC_LT_INT_EN_LT_INT_EN4_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN4_SHIFT)                  /* 0x00000010 */
#define SARADC_LT_INT_EN_LT_INT_EN5_SHIFT                  (5U)
#define SARADC_LT_INT_EN_LT_INT_EN5_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN5_SHIFT)                  /* 0x00000020 */
#define SARADC_LT_INT_EN_LT_INT_EN6_SHIFT                  (6U)
#define SARADC_LT_INT_EN_LT_INT_EN6_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN6_SHIFT)                  /* 0x00000040 */
#define SARADC_LT_INT_EN_LT_INT_EN7_SHIFT                  (7U)
#define SARADC_LT_INT_EN_LT_INT_EN7_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN7_SHIFT)                  /* 0x00000080 */
#define SARADC_LT_INT_EN_LT_INT_EN8_SHIFT                  (8U)
#define SARADC_LT_INT_EN_LT_INT_EN8_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN8_SHIFT)                  /* 0x00000100 */
#define SARADC_LT_INT_EN_LT_INT_EN9_SHIFT                  (9U)
#define SARADC_LT_INT_EN_LT_INT_EN9_MASK                   (0x1U << SARADC_LT_INT_EN_LT_INT_EN9_SHIFT)                  /* 0x00000200 */
#define SARADC_LT_INT_EN_LT_INT_EN10_SHIFT                 (10U)
#define SARADC_LT_INT_EN_LT_INT_EN10_MASK                  (0x1U << SARADC_LT_INT_EN_LT_INT_EN10_SHIFT)                 /* 0x00000400 */
#define SARADC_LT_INT_EN_LT_INT_EN11_SHIFT                 (11U)
#define SARADC_LT_INT_EN_LT_INT_EN11_MASK                  (0x1U << SARADC_LT_INT_EN_LT_INT_EN11_SHIFT)                 /* 0x00000800 */
#define SARADC_LT_INT_EN_LT_INT_EN12_SHIFT                 (12U)
#define SARADC_LT_INT_EN_LT_INT_EN12_MASK                  (0x1U << SARADC_LT_INT_EN_LT_INT_EN12_SHIFT)                 /* 0x00001000 */
#define SARADC_LT_INT_EN_LT_INT_EN13_SHIFT                 (13U)
#define SARADC_LT_INT_EN_LT_INT_EN13_MASK                  (0x1U << SARADC_LT_INT_EN_LT_INT_EN13_SHIFT)                 /* 0x00002000 */
#define SARADC_LT_INT_EN_LT_INT_EN14_SHIFT                 (14U)
#define SARADC_LT_INT_EN_LT_INT_EN14_MASK                  (0x1U << SARADC_LT_INT_EN_LT_INT_EN14_SHIFT)                 /* 0x00004000 */
#define SARADC_LT_INT_EN_LT_INT_EN15_SHIFT                 (15U)
#define SARADC_LT_INT_EN_LT_INT_EN15_MASK                  (0x1U << SARADC_LT_INT_EN_LT_INT_EN15_SHIFT)                 /* 0x00008000 */
/* MT_INT_EN */
#define SARADC_MT_INT_EN_OFFSET                            (0x100U)
#define SARADC_MT_INT_EN_MT_INT_EN0_SHIFT                  (0U)
#define SARADC_MT_INT_EN_MT_INT_EN0_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN0_SHIFT)                  /* 0x00000001 */
#define SARADC_MT_INT_EN_MT_INT_EN1_SHIFT                  (1U)
#define SARADC_MT_INT_EN_MT_INT_EN1_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN1_SHIFT)                  /* 0x00000002 */
#define SARADC_MT_INT_EN_MT_INT_EN2_SHIFT                  (2U)
#define SARADC_MT_INT_EN_MT_INT_EN2_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN2_SHIFT)                  /* 0x00000004 */
#define SARADC_MT_INT_EN_MT_INT_EN3_SHIFT                  (3U)
#define SARADC_MT_INT_EN_MT_INT_EN3_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN3_SHIFT)                  /* 0x00000008 */
#define SARADC_MT_INT_EN_MT_INT_EN4_SHIFT                  (4U)
#define SARADC_MT_INT_EN_MT_INT_EN4_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN4_SHIFT)                  /* 0x00000010 */
#define SARADC_MT_INT_EN_MT_INT_EN5_SHIFT                  (5U)
#define SARADC_MT_INT_EN_MT_INT_EN5_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN5_SHIFT)                  /* 0x00000020 */
#define SARADC_MT_INT_EN_MT_INT_EN6_SHIFT                  (6U)
#define SARADC_MT_INT_EN_MT_INT_EN6_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN6_SHIFT)                  /* 0x00000040 */
#define SARADC_MT_INT_EN_MT_INT_EN7_SHIFT                  (7U)
#define SARADC_MT_INT_EN_MT_INT_EN7_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN7_SHIFT)                  /* 0x00000080 */
#define SARADC_MT_INT_EN_MT_INT_EN8_SHIFT                  (8U)
#define SARADC_MT_INT_EN_MT_INT_EN8_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN8_SHIFT)                  /* 0x00000100 */
#define SARADC_MT_INT_EN_MT_INT_EN9_SHIFT                  (9U)
#define SARADC_MT_INT_EN_MT_INT_EN9_MASK                   (0x1U << SARADC_MT_INT_EN_MT_INT_EN9_SHIFT)                  /* 0x00000200 */
#define SARADC_MT_INT_EN_MT_INT_EN10_SHIFT                 (10U)
#define SARADC_MT_INT_EN_MT_INT_EN10_MASK                  (0x1U << SARADC_MT_INT_EN_MT_INT_EN10_SHIFT)                 /* 0x00000400 */
#define SARADC_MT_INT_EN_MT_INT_EN11_SHIFT                 (11U)
#define SARADC_MT_INT_EN_MT_INT_EN11_MASK                  (0x1U << SARADC_MT_INT_EN_MT_INT_EN11_SHIFT)                 /* 0x00000800 */
#define SARADC_MT_INT_EN_MT_INT_EN12_SHIFT                 (12U)
#define SARADC_MT_INT_EN_MT_INT_EN12_MASK                  (0x1U << SARADC_MT_INT_EN_MT_INT_EN12_SHIFT)                 /* 0x00001000 */
#define SARADC_MT_INT_EN_MT_INT_EN13_SHIFT                 (13U)
#define SARADC_MT_INT_EN_MT_INT_EN13_MASK                  (0x1U << SARADC_MT_INT_EN_MT_INT_EN13_SHIFT)                 /* 0x00002000 */
#define SARADC_MT_INT_EN_MT_INT_EN14_SHIFT                 (14U)
#define SARADC_MT_INT_EN_MT_INT_EN14_MASK                  (0x1U << SARADC_MT_INT_EN_MT_INT_EN14_SHIFT)                 /* 0x00004000 */
#define SARADC_MT_INT_EN_MT_INT_EN15_SHIFT                 (15U)
#define SARADC_MT_INT_EN_MT_INT_EN15_MASK                  (0x1U << SARADC_MT_INT_EN_MT_INT_EN15_SHIFT)                 /* 0x00008000 */
/* END_INT_EN */
#define SARADC_END_INT_EN_OFFSET                           (0x104U)
#define SARADC_END_INT_EN_END_INT_EN_SHIFT                 (0U)
#define SARADC_END_INT_EN_END_INT_EN_MASK                  (0x1U << SARADC_END_INT_EN_END_INT_EN_SHIFT)                 /* 0x00000001 */
/* ST_CON */
#define SARADC_ST_CON_OFFSET                               (0x108U)
#define SARADC_ST_CON_CCTRL_SHIFT                          (0U)
#define SARADC_ST_CON_CCTRL_MASK                           (0x7U << SARADC_ST_CON_CCTRL_SHIFT)                          /* 0x00000007 */
#define SARADC_ST_CON_ICTRL_SHIFT                          (3U)
#define SARADC_ST_CON_ICTRL_MASK                           (0x7U << SARADC_ST_CON_ICTRL_SHIFT)                          /* 0x00000038 */
/* STATUS */
#define SARADC_STATUS_OFFSET                               (0x10CU)
#define SARADC_STATUS                                      (0x2U)
#define SARADC_STATUS_CONV_ST_SHIFT                        (0U)
#define SARADC_STATUS_CONV_ST_MASK                         (0x1U << SARADC_STATUS_CONV_ST_SHIFT)                        /* 0x00000001 */
#define SARADC_STATUS_PD_SHIFT                             (1U)
#define SARADC_STATUS_PD_MASK                              (0x1U << SARADC_STATUS_PD_SHIFT)                             /* 0x00000002 */
#define SARADC_STATUS_SEL_SHIFT                            (2U)
#define SARADC_STATUS_SEL_MASK                             (0xFU << SARADC_STATUS_SEL_SHIFT)                            /* 0x0000003C */
/* END_INT_ST */
#define SARADC_END_INT_ST_OFFSET                           (0x110U)
#define SARADC_END_INT_ST_END_INT_ST_SHIFT                 (0U)
#define SARADC_END_INT_ST_END_INT_ST_MASK                  (0x1U << SARADC_END_INT_ST_END_INT_ST_SHIFT)                 /* 0x00000001 */
/* HT_INT_ST */
#define SARADC_HT_INT_ST_OFFSET                            (0x114U)
#define SARADC_HT_INT_ST_HT_INT_ST0_SHIFT                  (0U)
#define SARADC_HT_INT_ST_HT_INT_ST0_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST0_SHIFT)                  /* 0x00000001 */
#define SARADC_HT_INT_ST_HT_INT_ST1_SHIFT                  (1U)
#define SARADC_HT_INT_ST_HT_INT_ST1_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST1_SHIFT)                  /* 0x00000002 */
#define SARADC_HT_INT_ST_HT_INT_ST2_SHIFT                  (2U)
#define SARADC_HT_INT_ST_HT_INT_ST2_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST2_SHIFT)                  /* 0x00000004 */
#define SARADC_HT_INT_ST_HT_INT_ST3_SHIFT                  (3U)
#define SARADC_HT_INT_ST_HT_INT_ST3_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST3_SHIFT)                  /* 0x00000008 */
#define SARADC_HT_INT_ST_HT_INT_ST4_SHIFT                  (4U)
#define SARADC_HT_INT_ST_HT_INT_ST4_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST4_SHIFT)                  /* 0x00000010 */
#define SARADC_HT_INT_ST_HT_INT_ST5_SHIFT                  (5U)
#define SARADC_HT_INT_ST_HT_INT_ST5_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST5_SHIFT)                  /* 0x00000020 */
#define SARADC_HT_INT_ST_HT_INT_ST6_SHIFT                  (6U)
#define SARADC_HT_INT_ST_HT_INT_ST6_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST6_SHIFT)                  /* 0x00000040 */
#define SARADC_HT_INT_ST_HT_INT_ST7_SHIFT                  (7U)
#define SARADC_HT_INT_ST_HT_INT_ST7_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST7_SHIFT)                  /* 0x00000080 */
#define SARADC_HT_INT_ST_HT_INT_ST8_SHIFT                  (8U)
#define SARADC_HT_INT_ST_HT_INT_ST8_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST8_SHIFT)                  /* 0x00000100 */
#define SARADC_HT_INT_ST_HT_INT_ST9_SHIFT                  (9U)
#define SARADC_HT_INT_ST_HT_INT_ST9_MASK                   (0x1U << SARADC_HT_INT_ST_HT_INT_ST9_SHIFT)                  /* 0x00000200 */
#define SARADC_HT_INT_ST_HT_INT_ST10_SHIFT                 (10U)
#define SARADC_HT_INT_ST_HT_INT_ST10_MASK                  (0x1U << SARADC_HT_INT_ST_HT_INT_ST10_SHIFT)                 /* 0x00000400 */
#define SARADC_HT_INT_ST_HT_INT_ST11_SHIFT                 (11U)
#define SARADC_HT_INT_ST_HT_INT_ST11_MASK                  (0x1U << SARADC_HT_INT_ST_HT_INT_ST11_SHIFT)                 /* 0x00000800 */
#define SARADC_HT_INT_ST_HT_INT_ST12_SHIFT                 (12U)
#define SARADC_HT_INT_ST_HT_INT_ST12_MASK                  (0x1U << SARADC_HT_INT_ST_HT_INT_ST12_SHIFT)                 /* 0x00001000 */
#define SARADC_HT_INT_ST_HT_INT_ST13_SHIFT                 (13U)
#define SARADC_HT_INT_ST_HT_INT_ST13_MASK                  (0x1U << SARADC_HT_INT_ST_HT_INT_ST13_SHIFT)                 /* 0x00002000 */
#define SARADC_HT_INT_ST_HT_INT_ST14_SHIFT                 (14U)
#define SARADC_HT_INT_ST_HT_INT_ST14_MASK                  (0x1U << SARADC_HT_INT_ST_HT_INT_ST14_SHIFT)                 /* 0x00004000 */
#define SARADC_HT_INT_ST_HT_INT_ST15_SHIFT                 (15U)
#define SARADC_HT_INT_ST_HT_INT_ST15_MASK                  (0x1U << SARADC_HT_INT_ST_HT_INT_ST15_SHIFT)                 /* 0x00008000 */
/* LT_INT_ST */
#define SARADC_LT_INT_ST_OFFSET                            (0x118U)
#define SARADC_LT_INT_ST_LT_INT_ST0_SHIFT                  (0U)
#define SARADC_LT_INT_ST_LT_INT_ST0_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST0_SHIFT)                  /* 0x00000001 */
#define SARADC_LT_INT_ST_LT_INT_ST1_SHIFT                  (1U)
#define SARADC_LT_INT_ST_LT_INT_ST1_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST1_SHIFT)                  /* 0x00000002 */
#define SARADC_LT_INT_ST_LT_INT_ST2_SHIFT                  (2U)
#define SARADC_LT_INT_ST_LT_INT_ST2_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST2_SHIFT)                  /* 0x00000004 */
#define SARADC_LT_INT_ST_LT_INT_ST3_SHIFT                  (3U)
#define SARADC_LT_INT_ST_LT_INT_ST3_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST3_SHIFT)                  /* 0x00000008 */
#define SARADC_LT_INT_ST_LT_INT_ST4_SHIFT                  (4U)
#define SARADC_LT_INT_ST_LT_INT_ST4_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST4_SHIFT)                  /* 0x00000010 */
#define SARADC_LT_INT_ST_LT_INT_ST5_SHIFT                  (5U)
#define SARADC_LT_INT_ST_LT_INT_ST5_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST5_SHIFT)                  /* 0x00000020 */
#define SARADC_LT_INT_ST_LT_INT_ST6_SHIFT                  (6U)
#define SARADC_LT_INT_ST_LT_INT_ST6_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST6_SHIFT)                  /* 0x00000040 */
#define SARADC_LT_INT_ST_LT_INT_ST7_SHIFT                  (7U)
#define SARADC_LT_INT_ST_LT_INT_ST7_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST7_SHIFT)                  /* 0x00000080 */
#define SARADC_LT_INT_ST_LT_INT_ST8_SHIFT                  (8U)
#define SARADC_LT_INT_ST_LT_INT_ST8_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST8_SHIFT)                  /* 0x00000100 */
#define SARADC_LT_INT_ST_LT_INT_ST9_SHIFT                  (9U)
#define SARADC_LT_INT_ST_LT_INT_ST9_MASK                   (0x1U << SARADC_LT_INT_ST_LT_INT_ST9_SHIFT)                  /* 0x00000200 */
#define SARADC_LT_INT_ST_LT_INT_ST10_SHIFT                 (10U)
#define SARADC_LT_INT_ST_LT_INT_ST10_MASK                  (0x1U << SARADC_LT_INT_ST_LT_INT_ST10_SHIFT)                 /* 0x00000400 */
#define SARADC_LT_INT_ST_LT_INT_ST11_SHIFT                 (11U)
#define SARADC_LT_INT_ST_LT_INT_ST11_MASK                  (0x1U << SARADC_LT_INT_ST_LT_INT_ST11_SHIFT)                 /* 0x00000800 */
#define SARADC_LT_INT_ST_LT_INT_ST12_SHIFT                 (12U)
#define SARADC_LT_INT_ST_LT_INT_ST12_MASK                  (0x1U << SARADC_LT_INT_ST_LT_INT_ST12_SHIFT)                 /* 0x00001000 */
#define SARADC_LT_INT_ST_LT_INT_ST13_SHIFT                 (13U)
#define SARADC_LT_INT_ST_LT_INT_ST13_MASK                  (0x1U << SARADC_LT_INT_ST_LT_INT_ST13_SHIFT)                 /* 0x00002000 */
#define SARADC_LT_INT_ST_LT_INT_ST14_SHIFT                 (14U)
#define SARADC_LT_INT_ST_LT_INT_ST14_MASK                  (0x1U << SARADC_LT_INT_ST_LT_INT_ST14_SHIFT)                 /* 0x00004000 */
#define SARADC_LT_INT_ST_LT_INT_ST15_SHIFT                 (15U)
#define SARADC_LT_INT_ST_LT_INT_ST15_MASK                  (0x1U << SARADC_LT_INT_ST_LT_INT_ST15_SHIFT)                 /* 0x00008000 */
/* MT_INT_ST */
#define SARADC_MT_INT_ST_OFFSET                            (0x11CU)
#define SARADC_MT_INT_ST_MT_INT_ST0_SHIFT                  (0U)
#define SARADC_MT_INT_ST_MT_INT_ST0_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST0_SHIFT)                  /* 0x00000001 */
#define SARADC_MT_INT_ST_MT_INT_ST1_SHIFT                  (1U)
#define SARADC_MT_INT_ST_MT_INT_ST1_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST1_SHIFT)                  /* 0x00000002 */
#define SARADC_MT_INT_ST_MT_INT_ST2_SHIFT                  (2U)
#define SARADC_MT_INT_ST_MT_INT_ST2_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST2_SHIFT)                  /* 0x00000004 */
#define SARADC_MT_INT_ST_MT_INT_ST3_SHIFT                  (3U)
#define SARADC_MT_INT_ST_MT_INT_ST3_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST3_SHIFT)                  /* 0x00000008 */
#define SARADC_MT_INT_ST_MT_INT_ST4_SHIFT                  (4U)
#define SARADC_MT_INT_ST_MT_INT_ST4_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST4_SHIFT)                  /* 0x00000010 */
#define SARADC_MT_INT_ST_MT_INT_ST5_SHIFT                  (5U)
#define SARADC_MT_INT_ST_MT_INT_ST5_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST5_SHIFT)                  /* 0x00000020 */
#define SARADC_MT_INT_ST_MT_INT_ST6_SHIFT                  (6U)
#define SARADC_MT_INT_ST_MT_INT_ST6_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST6_SHIFT)                  /* 0x00000040 */
#define SARADC_MT_INT_ST_MT_INT_ST7_SHIFT                  (7U)
#define SARADC_MT_INT_ST_MT_INT_ST7_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST7_SHIFT)                  /* 0x00000080 */
#define SARADC_MT_INT_ST_MT_INT_ST8_SHIFT                  (8U)
#define SARADC_MT_INT_ST_MT_INT_ST8_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST8_SHIFT)                  /* 0x00000100 */
#define SARADC_MT_INT_ST_MT_INT_ST9_SHIFT                  (9U)
#define SARADC_MT_INT_ST_MT_INT_ST9_MASK                   (0x1U << SARADC_MT_INT_ST_MT_INT_ST9_SHIFT)                  /* 0x00000200 */
#define SARADC_MT_INT_ST_MT_INT_ST10_SHIFT                 (10U)
#define SARADC_MT_INT_ST_MT_INT_ST10_MASK                  (0x1U << SARADC_MT_INT_ST_MT_INT_ST10_SHIFT)                 /* 0x00000400 */
#define SARADC_MT_INT_ST_MT_INT_ST11_SHIFT                 (11U)
#define SARADC_MT_INT_ST_MT_INT_ST11_MASK                  (0x1U << SARADC_MT_INT_ST_MT_INT_ST11_SHIFT)                 /* 0x00000800 */
#define SARADC_MT_INT_ST_MT_INT_ST12_SHIFT                 (12U)
#define SARADC_MT_INT_ST_MT_INT_ST12_MASK                  (0x1U << SARADC_MT_INT_ST_MT_INT_ST12_SHIFT)                 /* 0x00001000 */
#define SARADC_MT_INT_ST_MT_INT_ST13_SHIFT                 (13U)
#define SARADC_MT_INT_ST_MT_INT_ST13_MASK                  (0x1U << SARADC_MT_INT_ST_MT_INT_ST13_SHIFT)                 /* 0x00002000 */
#define SARADC_MT_INT_ST_MT_INT_ST14_SHIFT                 (14U)
#define SARADC_MT_INT_ST_MT_INT_ST14_MASK                  (0x1U << SARADC_MT_INT_ST_MT_INT_ST14_SHIFT)                 /* 0x00004000 */
#define SARADC_MT_INT_ST_MT_INT_ST15_SHIFT                 (15U)
#define SARADC_MT_INT_ST_MT_INT_ST15_MASK                  (0x1U << SARADC_MT_INT_ST_MT_INT_ST15_SHIFT)                 /* 0x00008000 */
/* DATA0 */
#define SARADC_DATA0_OFFSET                                (0x120U)
#define SARADC_DATA0                                       (0x0U)
#define SARADC_DATA0_DATA0_SHIFT                           (0U)
#define SARADC_DATA0_DATA0_MASK                            (0xFFFU << SARADC_DATA0_DATA0_SHIFT)                         /* 0x00000FFF */
/* DATA1 */
#define SARADC_DATA1_OFFSET                                (0x124U)
#define SARADC_DATA1                                       (0x0U)
#define SARADC_DATA1_DATA1_SHIFT                           (0U)
#define SARADC_DATA1_DATA1_MASK                            (0xFFFU << SARADC_DATA1_DATA1_SHIFT)                         /* 0x00000FFF */
/* DATA2 */
#define SARADC_DATA2_OFFSET                                (0x128U)
#define SARADC_DATA2                                       (0x0U)
#define SARADC_DATA2_DATA2_SHIFT                           (0U)
#define SARADC_DATA2_DATA2_MASK                            (0xFFFU << SARADC_DATA2_DATA2_SHIFT)                         /* 0x00000FFF */
/* DATA3 */
#define SARADC_DATA3_OFFSET                                (0x12CU)
#define SARADC_DATA3                                       (0x0U)
#define SARADC_DATA3_DATA3_SHIFT                           (0U)
#define SARADC_DATA3_DATA3_MASK                            (0xFFFU << SARADC_DATA3_DATA3_SHIFT)                         /* 0x00000FFF */
/* DATA4 */
#define SARADC_DATA4_OFFSET                                (0x130U)
#define SARADC_DATA4                                       (0x0U)
#define SARADC_DATA4_DATA4_SHIFT                           (0U)
#define SARADC_DATA4_DATA4_MASK                            (0xFFFU << SARADC_DATA4_DATA4_SHIFT)                         /* 0x00000FFF */
/* DATA5 */
#define SARADC_DATA5_OFFSET                                (0x134U)
#define SARADC_DATA5                                       (0x0U)
#define SARADC_DATA5_DATA5_SHIFT                           (0U)
#define SARADC_DATA5_DATA5_MASK                            (0xFFFU << SARADC_DATA5_DATA5_SHIFT)                         /* 0x00000FFF */
/* DATA6 */
#define SARADC_DATA6_OFFSET                                (0x138U)
#define SARADC_DATA6                                       (0x0U)
#define SARADC_DATA6_DATA6_SHIFT                           (0U)
#define SARADC_DATA6_DATA6_MASK                            (0xFFFU << SARADC_DATA6_DATA6_SHIFT)                         /* 0x00000FFF */
/* DATA7 */
#define SARADC_DATA7_OFFSET                                (0x13CU)
#define SARADC_DATA7                                       (0x0U)
#define SARADC_DATA7_DATA7_SHIFT                           (0U)
#define SARADC_DATA7_DATA7_MASK                            (0xFFFU << SARADC_DATA7_DATA7_SHIFT)                         /* 0x00000FFF */
/* DATA8 */
#define SARADC_DATA8_OFFSET                                (0x140U)
#define SARADC_DATA8                                       (0x0U)
#define SARADC_DATA8_DATA8_SHIFT                           (0U)
#define SARADC_DATA8_DATA8_MASK                            (0xFFFU << SARADC_DATA8_DATA8_SHIFT)                         /* 0x00000FFF */
/* DATA9 */
#define SARADC_DATA9_OFFSET                                (0x144U)
#define SARADC_DATA9                                       (0x0U)
#define SARADC_DATA9_DATA9_SHIFT                           (0U)
#define SARADC_DATA9_DATA9_MASK                            (0xFFFU << SARADC_DATA9_DATA9_SHIFT)                         /* 0x00000FFF */
/* DATA10 */
#define SARADC_DATA10_OFFSET                               (0x148U)
#define SARADC_DATA10                                      (0x0U)
#define SARADC_DATA10_DATA10_SHIFT                         (0U)
#define SARADC_DATA10_DATA10_MASK                          (0xFFFU << SARADC_DATA10_DATA10_SHIFT)                       /* 0x00000FFF */
/* DATA11 */
#define SARADC_DATA11_OFFSET                               (0x14CU)
#define SARADC_DATA11                                      (0x0U)
#define SARADC_DATA11_DATA11_SHIFT                         (0U)
#define SARADC_DATA11_DATA11_MASK                          (0xFFFU << SARADC_DATA11_DATA11_SHIFT)                       /* 0x00000FFF */
/* DATA12 */
#define SARADC_DATA12_OFFSET                               (0x150U)
#define SARADC_DATA12                                      (0x0U)
#define SARADC_DATA12_DATA12_SHIFT                         (0U)
#define SARADC_DATA12_DATA12_MASK                          (0xFFFU << SARADC_DATA12_DATA12_SHIFT)                       /* 0x00000FFF */
/* DATA13 */
#define SARADC_DATA13_OFFSET                               (0x154U)
#define SARADC_DATA13                                      (0x0U)
#define SARADC_DATA13_DATA13_SHIFT                         (0U)
#define SARADC_DATA13_DATA13_MASK                          (0xFFFU << SARADC_DATA13_DATA13_SHIFT)                       /* 0x00000FFF */
/* DATA14 */
#define SARADC_DATA14_OFFSET                               (0x158U)
#define SARADC_DATA14                                      (0x0U)
#define SARADC_DATA14_DATA14_SHIFT                         (0U)
#define SARADC_DATA14_DATA14_MASK                          (0xFFFU << SARADC_DATA14_DATA14_SHIFT)                       /* 0x00000FFF */
/* DATA15 */
#define SARADC_DATA15_OFFSET                               (0x15CU)
#define SARADC_DATA15                                      (0x0U)
#define SARADC_DATA15_DATA15_SHIFT                         (0U)
#define SARADC_DATA15_DATA15_MASK                          (0xFFFU << SARADC_DATA15_DATA15_SHIFT)                       /* 0x00000FFF */
/* AUTO_CH_EN */
#define SARADC_AUTO_CH_EN_OFFSET                           (0x160U)
#define SARADC_AUTO_CH_EN_AUTO_CH0_EN_SHIFT                (0U)
#define SARADC_AUTO_CH_EN_AUTO_CH0_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH0_EN_SHIFT)                /* 0x00000001 */
#define SARADC_AUTO_CH_EN_AUTO_CH1_EN_SHIFT                (1U)
#define SARADC_AUTO_CH_EN_AUTO_CH1_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH1_EN_SHIFT)                /* 0x00000002 */
#define SARADC_AUTO_CH_EN_AUTO_CH2_EN_SHIFT                (2U)
#define SARADC_AUTO_CH_EN_AUTO_CH2_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH2_EN_SHIFT)                /* 0x00000004 */
#define SARADC_AUTO_CH_EN_AUTO_CH3_EN_SHIFT                (3U)
#define SARADC_AUTO_CH_EN_AUTO_CH3_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH3_EN_SHIFT)                /* 0x00000008 */
#define SARADC_AUTO_CH_EN_AUTO_CH4_EN_SHIFT                (4U)
#define SARADC_AUTO_CH_EN_AUTO_CH4_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH4_EN_SHIFT)                /* 0x00000010 */
#define SARADC_AUTO_CH_EN_AUTO_CH5_EN_SHIFT                (5U)
#define SARADC_AUTO_CH_EN_AUTO_CH5_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH5_EN_SHIFT)                /* 0x00000020 */
#define SARADC_AUTO_CH_EN_AUTO_CH6_EN_SHIFT                (6U)
#define SARADC_AUTO_CH_EN_AUTO_CH6_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH6_EN_SHIFT)                /* 0x00000040 */
#define SARADC_AUTO_CH_EN_AUTO_CH7_EN_SHIFT                (7U)
#define SARADC_AUTO_CH_EN_AUTO_CH7_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH7_EN_SHIFT)                /* 0x00000080 */
#define SARADC_AUTO_CH_EN_AUTO_CH8_EN_SHIFT                (8U)
#define SARADC_AUTO_CH_EN_AUTO_CH8_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH8_EN_SHIFT)                /* 0x00000100 */
#define SARADC_AUTO_CH_EN_AUTO_CH9_EN_SHIFT                (9U)
#define SARADC_AUTO_CH_EN_AUTO_CH9_EN_MASK                 (0x1U << SARADC_AUTO_CH_EN_AUTO_CH9_EN_SHIFT)                /* 0x00000200 */
#define SARADC_AUTO_CH_EN_AUTO_CH10_EN_SHIFT               (10U)
#define SARADC_AUTO_CH_EN_AUTO_CH10_EN_MASK                (0x1U << SARADC_AUTO_CH_EN_AUTO_CH10_EN_SHIFT)               /* 0x00000400 */
#define SARADC_AUTO_CH_EN_AUTO_CH11_EN_SHIFT               (11U)
#define SARADC_AUTO_CH_EN_AUTO_CH11_EN_MASK                (0x1U << SARADC_AUTO_CH_EN_AUTO_CH11_EN_SHIFT)               /* 0x00000800 */
#define SARADC_AUTO_CH_EN_AUTO_CH12_EN_SHIFT               (12U)
#define SARADC_AUTO_CH_EN_AUTO_CH12_EN_MASK                (0x1U << SARADC_AUTO_CH_EN_AUTO_CH12_EN_SHIFT)               /* 0x00001000 */
#define SARADC_AUTO_CH_EN_AUTO_CH13_EN_SHIFT               (13U)
#define SARADC_AUTO_CH_EN_AUTO_CH13_EN_MASK                (0x1U << SARADC_AUTO_CH_EN_AUTO_CH13_EN_SHIFT)               /* 0x00002000 */
#define SARADC_AUTO_CH_EN_AUTO_CH14_EN_SHIFT               (14U)
#define SARADC_AUTO_CH_EN_AUTO_CH14_EN_MASK                (0x1U << SARADC_AUTO_CH_EN_AUTO_CH14_EN_SHIFT)               /* 0x00004000 */
#define SARADC_AUTO_CH_EN_AUTO_CH15_EN_SHIFT               (15U)
#define SARADC_AUTO_CH_EN_AUTO_CH15_EN_MASK                (0x1U << SARADC_AUTO_CH_EN_AUTO_CH15_EN_SHIFT)               /* 0x00008000 */
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
#define FSPI_VER                                           (0x8U)
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
#define DCACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT             (12U)
#define DCACHE_CACHE_CTRL_CACHE_MPU_MODE_MASK              (0x1U << DCACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT)             /* 0x00001000 */
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
#define DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_MASK         (0x7FFFFFFU << DCACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT)  /* 0x07FFFFFF */
/* STB_TIMEOUT_CTRL */
#define DCACHE_STB_TIMEOUT_CTRL_OFFSET                     (0xCU)
#define DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT    (0U)
#define DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_MASK     (0xFFFFFFFFU << DCACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT) /* 0xFFFFFFFF */
/* RAM_DEBUG */
#define DCACHE_RAM_DEBUG_OFFSET                            (0x10U)
#define DCACHE_RAM_DEBUG_RAM_DEBUG_EN_SHIFT                (0U)
#define DCACHE_RAM_DEBUG_RAM_DEBUG_EN_MASK                 (0x1U << DCACHE_RAM_DEBUG_RAM_DEBUG_EN_SHIFT)                /* 0x00000001 */
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
#define DCACHE_CACHE_ERR_HADDR                             (0x0U)
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
#define DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_RAM_RD_HIT_CNT_SHIFT (0U)
#define DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_RAM_RD_HIT_CNT_MASK (0xFFFFFFFFU << DCACHE_PMU_SRAM_RD_HIT_CNT_PMU_RAM_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_HB_RD_HIT_CNT */
#define DCACHE_PMU_HB_RD_HIT_CNT_OFFSET                    (0x4CU)
#define DCACHE_PMU_HB_RD_HIT_CNT                           (0x0U)
#define DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT   (0U)
#define DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_MASK    (0xFFFFFFFFU << DCACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_STB_RD_HIT_CNT */
#define DCACHE_PMU_STB_RD_HIT_CNT_OFFSET                   (0x50U)
#define DCACHE_PMU_STB_RD_HIT_CNT                          (0x0U)
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
#define DCACHE_PMU_WR_LAT_CNT_PMU_WR_LAT_CNT_SHIFT         (0U)
#define DCACHE_PMU_WR_LAT_CNT_PMU_WR_LAT_CNT_MASK          (0xFFFFFFFFU << DCACHE_PMU_WR_LAT_CNT_PMU_WR_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* REVISION */
#define DCACHE_REVISION_OFFSET                             (0xF0U)
#define DCACHE_REVISION                                    (0x100U)
#define DCACHE_REVISION_REVISION_SHIFT                     (0U)
#define DCACHE_REVISION_REVISION_MASK                      (0xFFFFFFFFU << DCACHE_REVISION_REVISION_SHIFT)              /* 0xFFFFFFFF */
/*****************************************ICACHE*****************************************/
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
#define ICACHE_CACHE_CTRL_STB_ENTRY_THRESH_SHIFT           (8U)
#define ICACHE_CACHE_CTRL_STB_ENTRY_THRESH_MASK            (0x7U << ICACHE_CACHE_CTRL_STB_ENTRY_THRESH_SHIFT)           /* 0x00000700 */
#define ICACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT             (12U)
#define ICACHE_CACHE_CTRL_CACHE_MPU_MODE_MASK              (0x1U << ICACHE_CACHE_CTRL_CACHE_MPU_MODE_SHIFT)             /* 0x00001000 */
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
#define ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_MASK         (0x7FFFFFFU << ICACHE_CACHE_MAINTAIN1_CACHE_M_OFFSET_SHIFT)  /* 0x07FFFFFF */
/* STB_TIMEOUT_CTRL */
#define ICACHE_STB_TIMEOUT_CTRL_OFFSET                     (0xCU)
#define ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT    (0U)
#define ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_MASK     (0xFFFFFFFFU << ICACHE_STB_TIMEOUT_CTRL_STB_TIMEOUT_VALUE_SHIFT) /* 0xFFFFFFFF */
/* RAM_DEBUG */
#define ICACHE_RAM_DEBUG_OFFSET                            (0x10U)
#define ICACHE_RAM_DEBUG_RAM_DEBUG_EN_SHIFT                (0U)
#define ICACHE_RAM_DEBUG_RAM_DEBUG_EN_MASK                 (0x1U << ICACHE_RAM_DEBUG_RAM_DEBUG_EN_SHIFT)                /* 0x00000001 */
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
#define ICACHE_CACHE_ERR_HADDR                             (0x0U)
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
#define ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_RAM_RD_HIT_CNT_SHIFT (0U)
#define ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_RAM_RD_HIT_CNT_MASK (0xFFFFFFFFU << ICACHE_PMU_SRAM_RD_HIT_CNT_PMU_RAM_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_HB_RD_HIT_CNT */
#define ICACHE_PMU_HB_RD_HIT_CNT_OFFSET                    (0x4CU)
#define ICACHE_PMU_HB_RD_HIT_CNT                           (0x0U)
#define ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT   (0U)
#define ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_MASK    (0xFFFFFFFFU << ICACHE_PMU_HB_RD_HIT_CNT_PMU_HB_RD_HIT_CNT_SHIFT) /* 0xFFFFFFFF */
/* PMU_STB_RD_HIT_CNT */
#define ICACHE_PMU_STB_RD_HIT_CNT_OFFSET                   (0x50U)
#define ICACHE_PMU_STB_RD_HIT_CNT                          (0x0U)
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
#define ICACHE_PMU_WR_LAT_CNT_PMU_WR_LAT_CNT_SHIFT         (0U)
#define ICACHE_PMU_WR_LAT_CNT_PMU_WR_LAT_CNT_MASK          (0xFFFFFFFFU << ICACHE_PMU_WR_LAT_CNT_PMU_WR_LAT_CNT_SHIFT)  /* 0xFFFFFFFF */
/* REVISION */
#define ICACHE_REVISION_OFFSET                             (0xF0U)
#define ICACHE_REVISION                                    (0x100U)
#define ICACHE_REVISION_REVISION_SHIFT                     (0U)
#define ICACHE_REVISION_REVISION_MASK                      (0xFFFFFFFFU << ICACHE_REVISION_REVISION_SHIFT)              /* 0xFFFFFFFF */
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
/******************************************MBOX******************************************/
/* A2B_INTEN */
#define MBOX_A2B_INTEN_OFFSET                              (0x0U)
#define MBOX_A2B_INTEN_INT0_SHIFT                          (0U)
#define MBOX_A2B_INTEN_INT0_MASK                           (0x1U << MBOX_A2B_INTEN_INT0_SHIFT)                          /* 0x00000001 */
#define MBOX_A2B_INTEN_INT1_SHIFT                          (1U)
#define MBOX_A2B_INTEN_INT1_MASK                           (0x1U << MBOX_A2B_INTEN_INT1_SHIFT)                          /* 0x00000002 */
#define MBOX_A2B_INTEN_INT2_SHIFT                          (2U)
#define MBOX_A2B_INTEN_INT2_MASK                           (0x1U << MBOX_A2B_INTEN_INT2_SHIFT)                          /* 0x00000004 */
#define MBOX_A2B_INTEN_INT3_SHIFT                          (3U)
#define MBOX_A2B_INTEN_INT3_MASK                           (0x1U << MBOX_A2B_INTEN_INT3_SHIFT)                          /* 0x00000008 */
/* A2B_STATUS */
#define MBOX_A2B_STATUS_OFFSET                             (0x4U)
#define MBOX_A2B_STATUS_INT0_SHIFT                         (0U)
#define MBOX_A2B_STATUS_INT0_MASK                          (0x1U << MBOX_A2B_STATUS_INT0_SHIFT)                         /* 0x00000001 */
#define MBOX_A2B_STATUS_INT1_SHIFT                         (1U)
#define MBOX_A2B_STATUS_INT1_MASK                          (0x1U << MBOX_A2B_STATUS_INT1_SHIFT)                         /* 0x00000002 */
#define MBOX_A2B_STATUS_INT2_SHIFT                         (2U)
#define MBOX_A2B_STATUS_INT2_MASK                          (0x1U << MBOX_A2B_STATUS_INT2_SHIFT)                         /* 0x00000004 */
#define MBOX_A2B_STATUS_INT3_SHIFT                         (3U)
#define MBOX_A2B_STATUS_INT3_MASK                          (0x1U << MBOX_A2B_STATUS_INT3_SHIFT)                         /* 0x00000008 */
/* A2B_CMD_0 */
#define MBOX_A2B_CMD_0_OFFSET                              (0x8U)
#define MBOX_A2B_CMD_0_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_0_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_0_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_0 */
#define MBOX_A2B_DAT_0_OFFSET                              (0xCU)
#define MBOX_A2B_DAT_0_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_0_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_0_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_1 */
#define MBOX_A2B_CMD_1_OFFSET                              (0x10U)
#define MBOX_A2B_CMD_1_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_1_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_1_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_1 */
#define MBOX_A2B_DAT_1_OFFSET                              (0x14U)
#define MBOX_A2B_DAT_1_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_1_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_1_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_2 */
#define MBOX_A2B_CMD_2_OFFSET                              (0x18U)
#define MBOX_A2B_CMD_2_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_2_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_2_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_2 */
#define MBOX_A2B_DAT_2_OFFSET                              (0x1CU)
#define MBOX_A2B_DAT_2_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_2_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_2_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* A2B_CMD_3 */
#define MBOX_A2B_CMD_3_OFFSET                              (0x20U)
#define MBOX_A2B_CMD_3_COMMAND_SHIFT                       (0U)
#define MBOX_A2B_CMD_3_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_A2B_CMD_3_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* A2B_DAT_3 */
#define MBOX_A2B_DAT_3_OFFSET                              (0x24U)
#define MBOX_A2B_DAT_3_DATA_SHIFT                          (0U)
#define MBOX_A2B_DAT_3_DATA_MASK                           (0xFFFFFFFFU << MBOX_A2B_DAT_3_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_INTEN */
#define MBOX_B2A_INTEN_OFFSET                              (0x28U)
#define MBOX_B2A_INTEN_INT0_SHIFT                          (0U)
#define MBOX_B2A_INTEN_INT0_MASK                           (0x1U << MBOX_B2A_INTEN_INT0_SHIFT)                          /* 0x00000001 */
#define MBOX_B2A_INTEN_INT1_SHIFT                          (1U)
#define MBOX_B2A_INTEN_INT1_MASK                           (0x1U << MBOX_B2A_INTEN_INT1_SHIFT)                          /* 0x00000002 */
#define MBOX_B2A_INTEN_INT2_SHIFT                          (2U)
#define MBOX_B2A_INTEN_INT2_MASK                           (0x1U << MBOX_B2A_INTEN_INT2_SHIFT)                          /* 0x00000004 */
#define MBOX_B2A_INTEN_INT3_SHIFT                          (3U)
#define MBOX_B2A_INTEN_INT3_MASK                           (0x1U << MBOX_B2A_INTEN_INT3_SHIFT)                          /* 0x00000008 */
/* B2A_STATUS */
#define MBOX_B2A_STATUS_OFFSET                             (0x2CU)
#define MBOX_B2A_STATUS_INT0_SHIFT                         (0U)
#define MBOX_B2A_STATUS_INT0_MASK                          (0x1U << MBOX_B2A_STATUS_INT0_SHIFT)                         /* 0x00000001 */
#define MBOX_B2A_STATUS_INT1_SHIFT                         (1U)
#define MBOX_B2A_STATUS_INT1_MASK                          (0x1U << MBOX_B2A_STATUS_INT1_SHIFT)                         /* 0x00000002 */
#define MBOX_B2A_STATUS_INT2_SHIFT                         (2U)
#define MBOX_B2A_STATUS_INT2_MASK                          (0x1U << MBOX_B2A_STATUS_INT2_SHIFT)                         /* 0x00000004 */
#define MBOX_B2A_STATUS_INT3_SHIFT                         (3U)
#define MBOX_B2A_STATUS_INT3_MASK                          (0x1U << MBOX_B2A_STATUS_INT3_SHIFT)                         /* 0x00000008 */
/* B2A_CMD_0 */
#define MBOX_B2A_CMD_0_OFFSET                              (0x30U)
#define MBOX_B2A_CMD_0_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_0_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_0_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_0 */
#define MBOX_B2A_DAT_0_OFFSET                              (0x34U)
#define MBOX_B2A_DAT_0_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_0_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_0_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_1 */
#define MBOX_B2A_CMD_1_OFFSET                              (0x38U)
#define MBOX_B2A_CMD_1_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_1_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_1_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_1 */
#define MBOX_B2A_DAT_1_OFFSET                              (0x3CU)
#define MBOX_B2A_DAT_1_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_1_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_1_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_2 */
#define MBOX_B2A_CMD_2_OFFSET                              (0x40U)
#define MBOX_B2A_CMD_2_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_2_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_2_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_2 */
#define MBOX_B2A_DAT_2_OFFSET                              (0x44U)
#define MBOX_B2A_DAT_2_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_2_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_2_DATA_SHIFT)                   /* 0xFFFFFFFF */
/* B2A_CMD_3 */
#define MBOX_B2A_CMD_3_OFFSET                              (0x48U)
#define MBOX_B2A_CMD_3_COMMAND_SHIFT                       (0U)
#define MBOX_B2A_CMD_3_COMMAND_MASK                        (0xFFFFFFFFU << MBOX_B2A_CMD_3_COMMAND_SHIFT)                /* 0xFFFFFFFF */
/* B2A_DAT_3 */
#define MBOX_B2A_DAT_3_OFFSET                              (0x4CU)
#define MBOX_B2A_DAT_3_DATA_SHIFT                          (0U)
#define MBOX_B2A_DAT_3_DATA_MASK                           (0xFFFFFFFFU << MBOX_B2A_DAT_3_DATA_SHIFT)                   /* 0xFFFFFFFF */
/*****************************************INTMUX*****************************************/
/* INT_ENABLE_GROUP0 */
#define INTMUX_INT_ENABLE_GROUP0_OFFSET                    (0x0U)
#define INTMUX_INT_ENABLE_GROUP0_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP0_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP0_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP1 */
#define INTMUX_INT_ENABLE_GROUP1_OFFSET                    (0x4U)
#define INTMUX_INT_ENABLE_GROUP1_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP1_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP1_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP2 */
#define INTMUX_INT_ENABLE_GROUP2_OFFSET                    (0x8U)
#define INTMUX_INT_ENABLE_GROUP2_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP2_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP2_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP3 */
#define INTMUX_INT_ENABLE_GROUP3_OFFSET                    (0xCU)
#define INTMUX_INT_ENABLE_GROUP3_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP3_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP3_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP4 */
#define INTMUX_INT_ENABLE_GROUP4_OFFSET                    (0x10U)
#define INTMUX_INT_ENABLE_GROUP4_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP4_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP4_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP5 */
#define INTMUX_INT_ENABLE_GROUP5_OFFSET                    (0x14U)
#define INTMUX_INT_ENABLE_GROUP5_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP5_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP5_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP6 */
#define INTMUX_INT_ENABLE_GROUP6_OFFSET                    (0x18U)
#define INTMUX_INT_ENABLE_GROUP6_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP6_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP6_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP7 */
#define INTMUX_INT_ENABLE_GROUP7_OFFSET                    (0x1CU)
#define INTMUX_INT_ENABLE_GROUP7_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP7_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP7_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP8 */
#define INTMUX_INT_ENABLE_GROUP8_OFFSET                    (0x20U)
#define INTMUX_INT_ENABLE_GROUP8_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP8_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP8_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP9 */
#define INTMUX_INT_ENABLE_GROUP9_OFFSET                    (0x24U)
#define INTMUX_INT_ENABLE_GROUP9_ENABLE_SHIFT              (0U)
#define INTMUX_INT_ENABLE_GROUP9_ENABLE_MASK               (0xFFU << INTMUX_INT_ENABLE_GROUP9_ENABLE_SHIFT)             /* 0x000000FF */
/* INT_ENABLE_GROUP10 */
#define INTMUX_INT_ENABLE_GROUP10_OFFSET                   (0x28U)
#define INTMUX_INT_ENABLE_GROUP10_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP10_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP10_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP11 */
#define INTMUX_INT_ENABLE_GROUP11_OFFSET                   (0x2CU)
#define INTMUX_INT_ENABLE_GROUP11_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP11_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP11_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP12 */
#define INTMUX_INT_ENABLE_GROUP12_OFFSET                   (0x30U)
#define INTMUX_INT_ENABLE_GROUP12_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP12_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP12_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP13 */
#define INTMUX_INT_ENABLE_GROUP13_OFFSET                   (0x34U)
#define INTMUX_INT_ENABLE_GROUP13_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP13_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP13_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP14 */
#define INTMUX_INT_ENABLE_GROUP14_OFFSET                   (0x38U)
#define INTMUX_INT_ENABLE_GROUP14_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP14_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP14_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP15 */
#define INTMUX_INT_ENABLE_GROUP15_OFFSET                   (0x3CU)
#define INTMUX_INT_ENABLE_GROUP15_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP15_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP15_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP16 */
#define INTMUX_INT_ENABLE_GROUP16_OFFSET                   (0x40U)
#define INTMUX_INT_ENABLE_GROUP16_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP16_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP16_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP17 */
#define INTMUX_INT_ENABLE_GROUP17_OFFSET                   (0x44U)
#define INTMUX_INT_ENABLE_GROUP17_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP17_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP17_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP18 */
#define INTMUX_INT_ENABLE_GROUP18_OFFSET                   (0x48U)
#define INTMUX_INT_ENABLE_GROUP18_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP18_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP18_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP19 */
#define INTMUX_INT_ENABLE_GROUP19_OFFSET                   (0x4CU)
#define INTMUX_INT_ENABLE_GROUP19_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP19_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP19_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP20 */
#define INTMUX_INT_ENABLE_GROUP20_OFFSET                   (0x50U)
#define INTMUX_INT_ENABLE_GROUP20_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP20_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP20_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP21 */
#define INTMUX_INT_ENABLE_GROUP21_OFFSET                   (0x54U)
/* INT_ENABLE_GROUP22 */
#define INTMUX_INT_ENABLE_GROUP22_OFFSET                   (0x58U)
#define INTMUX_INT_ENABLE_GROUP22_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP22_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP22_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP23 */
#define INTMUX_INT_ENABLE_GROUP23_OFFSET                   (0x5CU)
#define INTMUX_INT_ENABLE_GROUP23_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP23_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP23_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP24 */
#define INTMUX_INT_ENABLE_GROUP24_OFFSET                   (0x60U)
#define INTMUX_INT_ENABLE_GROUP24_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP24_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP24_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP25 */
#define INTMUX_INT_ENABLE_GROUP25_OFFSET                   (0x64U)
#define INTMUX_INT_ENABLE_GROUP25_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP25_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP25_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP26 */
#define INTMUX_INT_ENABLE_GROUP26_OFFSET                   (0x68U)
#define INTMUX_INT_ENABLE_GROUP26_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP26_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP26_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP27 */
#define INTMUX_INT_ENABLE_GROUP27_OFFSET                   (0x6CU)
#define INTMUX_INT_ENABLE_GROUP27_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP27_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP27_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP28 */
#define INTMUX_INT_ENABLE_GROUP28_OFFSET                   (0x70U)
#define INTMUX_INT_ENABLE_GROUP28_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP28_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP28_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP29 */
#define INTMUX_INT_ENABLE_GROUP29_OFFSET                   (0x74U)
#define INTMUX_INT_ENABLE_GROUP29_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP29_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP29_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP30 */
#define INTMUX_INT_ENABLE_GROUP30_OFFSET                   (0x78U)
#define INTMUX_INT_ENABLE_GROUP30_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP30_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP30_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_ENABLE_GROUP31 */
#define INTMUX_INT_ENABLE_GROUP31_OFFSET                   (0x7CU)
#define INTMUX_INT_ENABLE_GROUP31_ENABLE_SHIFT             (0U)
#define INTMUX_INT_ENABLE_GROUP31_ENABLE_MASK              (0xFFU << INTMUX_INT_ENABLE_GROUP31_ENABLE_SHIFT)            /* 0x000000FF */
/* INT_FLAG_GROUP0 */
#define INTMUX_INT_FLAG_GROUP0_OFFSET                      (0x80U)
#define INTMUX_INT_FLAG_GROUP0                             (0x0U)
#define INTMUX_INT_FLAG_GROUP0_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP0_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP0_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP1 */
#define INTMUX_INT_FLAG_GROUP1_OFFSET                      (0x84U)
#define INTMUX_INT_FLAG_GROUP1                             (0x0U)
#define INTMUX_INT_FLAG_GROUP1_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP1_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP1_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP2 */
#define INTMUX_INT_FLAG_GROUP2_OFFSET                      (0x88U)
#define INTMUX_INT_FLAG_GROUP2                             (0x0U)
#define INTMUX_INT_FLAG_GROUP2_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP2_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP2_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP3 */
#define INTMUX_INT_FLAG_GROUP3_OFFSET                      (0x8CU)
#define INTMUX_INT_FLAG_GROUP3                             (0x0U)
#define INTMUX_INT_FLAG_GROUP3_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP3_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP3_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP4 */
#define INTMUX_INT_FLAG_GROUP4_OFFSET                      (0x90U)
#define INTMUX_INT_FLAG_GROUP4                             (0x0U)
#define INTMUX_INT_FLAG_GROUP4_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP4_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP4_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP5 */
#define INTMUX_INT_FLAG_GROUP5_OFFSET                      (0x94U)
#define INTMUX_INT_FLAG_GROUP5                             (0x0U)
#define INTMUX_INT_FLAG_GROUP5_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP5_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP5_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP6 */
#define INTMUX_INT_FLAG_GROUP6_OFFSET                      (0x98U)
#define INTMUX_INT_FLAG_GROUP6                             (0x0U)
#define INTMUX_INT_FLAG_GROUP6_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP6_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP6_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP7 */
#define INTMUX_INT_FLAG_GROUP7_OFFSET                      (0x9CU)
#define INTMUX_INT_FLAG_GROUP7                             (0x0U)
#define INTMUX_INT_FLAG_GROUP7_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP7_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP7_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP8 */
#define INTMUX_INT_FLAG_GROUP8_OFFSET                      (0xA0U)
#define INTMUX_INT_FLAG_GROUP8                             (0x0U)
#define INTMUX_INT_FLAG_GROUP8_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP8_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP8_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP9 */
#define INTMUX_INT_FLAG_GROUP9_OFFSET                      (0xA4U)
#define INTMUX_INT_FLAG_GROUP9                             (0x0U)
#define INTMUX_INT_FLAG_GROUP9_FLAG_SHIFT                  (0U)
#define INTMUX_INT_FLAG_GROUP9_FLAG_MASK                   (0xFFU << INTMUX_INT_FLAG_GROUP9_FLAG_SHIFT)                 /* 0x000000FF */
/* INT_FLAG_GROUP10 */
#define INTMUX_INT_FLAG_GROUP10_OFFSET                     (0xA8U)
#define INTMUX_INT_FLAG_GROUP10                            (0x0U)
#define INTMUX_INT_FLAG_GROUP10_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP10_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP10_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP11 */
#define INTMUX_INT_FLAG_GROUP11_OFFSET                     (0xACU)
#define INTMUX_INT_FLAG_GROUP11                            (0x0U)
#define INTMUX_INT_FLAG_GROUP11_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP11_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP11_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP12 */
#define INTMUX_INT_FLAG_GROUP12_OFFSET                     (0xB0U)
#define INTMUX_INT_FLAG_GROUP12                            (0x0U)
#define INTMUX_INT_FLAG_GROUP12_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP12_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP12_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP13 */
#define INTMUX_INT_FLAG_GROUP13_OFFSET                     (0xB4U)
#define INTMUX_INT_FLAG_GROUP13                            (0x0U)
#define INTMUX_INT_FLAG_GROUP13_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP13_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP13_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP14 */
#define INTMUX_INT_FLAG_GROUP14_OFFSET                     (0xB8U)
#define INTMUX_INT_FLAG_GROUP14                            (0x0U)
#define INTMUX_INT_FLAG_GROUP14_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP14_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP14_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP15 */
#define INTMUX_INT_FLAG_GROUP15_OFFSET                     (0xBCU)
#define INTMUX_INT_FLAG_GROUP15                            (0x0U)
#define INTMUX_INT_FLAG_GROUP15_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP15_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP15_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP16 */
#define INTMUX_INT_FLAG_GROUP16_OFFSET                     (0xC0U)
#define INTMUX_INT_FLAG_GROUP16_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP16_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP16_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP17 */
#define INTMUX_INT_FLAG_GROUP17_OFFSET                     (0xC4U)
#define INTMUX_INT_FLAG_GROUP17_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP17_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP17_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP18 */
#define INTMUX_INT_FLAG_GROUP18_OFFSET                     (0xC8U)
#define INTMUX_INT_FLAG_GROUP18_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP18_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP18_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP19 */
#define INTMUX_INT_FLAG_GROUP19_OFFSET                     (0xCCU)
#define INTMUX_INT_FLAG_GROUP19_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP19_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP19_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP20 */
#define INTMUX_INT_FLAG_GROUP20_OFFSET                     (0xD0U)
#define INTMUX_INT_FLAG_GROUP20_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP20_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP20_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP21 */
#define INTMUX_INT_FLAG_GROUP21_OFFSET                     (0xD4U)
#define INTMUX_INT_FLAG_GROUP21_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP21_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP21_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP22 */
#define INTMUX_INT_FLAG_GROUP22_OFFSET                     (0xD8U)
#define INTMUX_INT_FLAG_GROUP22_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP22_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP22_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP23 */
#define INTMUX_INT_FLAG_GROUP23_OFFSET                     (0xDCU)
#define INTMUX_INT_FLAG_GROUP23_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP23_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP23_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP24 */
#define INTMUX_INT_FLAG_GROUP24_OFFSET                     (0xE0U)
#define INTMUX_INT_FLAG_GROUP24_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP24_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP24_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP25 */
#define INTMUX_INT_FLAG_GROUP25_OFFSET                     (0xE4U)
#define INTMUX_INT_FLAG_GROUP25_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP25_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP25_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP26 */
#define INTMUX_INT_FLAG_GROUP26_OFFSET                     (0xE8U)
#define INTMUX_INT_FLAG_GROUP26_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP26_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP26_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP27 */
#define INTMUX_INT_FLAG_GROUP27_OFFSET                     (0xECU)
#define INTMUX_INT_FLAG_GROUP27_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP27_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP27_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP28 */
#define INTMUX_INT_FLAG_GROUP28_OFFSET                     (0xF0U)
#define INTMUX_INT_FLAG_GROUP28_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP28_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP28_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP29 */
#define INTMUX_INT_FLAG_GROUP29_OFFSET                     (0xF4U)
#define INTMUX_INT_FLAG_GROUP29_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP29_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP29_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP30 */
#define INTMUX_INT_FLAG_GROUP30_OFFSET                     (0xF8U)
#define INTMUX_INT_FLAG_GROUP30_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP30_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP30_FLAG_SHIFT)                /* 0x000000FF */
/* INT_FLAG_GROUP31 */
#define INTMUX_INT_FLAG_GROUP31_OFFSET                     (0xFCU)
#define INTMUX_INT_FLAG_GROUP31_FLAG_SHIFT                 (0U)
#define INTMUX_INT_FLAG_GROUP31_FLAG_MASK                  (0xFFU << INTMUX_INT_FLAG_GROUP31_FLAG_SHIFT)                /* 0x000000FF */

// ======================= TOPCRU module definition bank=0 ========================
// TOPCRU_SOFTRST_CON00(Offset:0x400)

// TOPCRU_SOFTRST_CON01(Offset:0x404)
#define SRST_ARESETN_TOP_BIU                     0x00000010
#define SRST_ARESETN_TOP_VIO_BIU                 0x00000011
#define SRST_RESETN_REF_PVTPLL_LOGIC             0x00000012

// TOPCRU_SOFTRST_CON03(Offset:0x40C)
#define SRST_NCOREPORESET0                       0x00000030
#define SRST_NCOREPORESET1                       0x00000031
#define SRST_NCOREPORESET2                       0x00000032
#define SRST_NCOREPORESET3                       0x00000033
#define SRST_NCORESET0                           0x00000034
#define SRST_NCORESET1                           0x00000035
#define SRST_NCORESET2                           0x00000036
#define SRST_NCORESET3                           0x00000037
#define SRST_NL2RESET                            0x00000038

// TOPCRU_SOFTRST_CON04(Offset:0x410)
#define SRST_RESETN_DAP                          0x00000049
#define SRST_PRESETN_DBG_DAPLITE                 0x0000004A
#define SRST_RESETN_REF_PVTPLL_CORE              0x0000004D

// TOPCRU_SOFTRST_CON05(Offset:0x414)
#define SRST_ARESETN_CORE_BIU                    0x00000050
#define SRST_PRESETN_CORE_BIU                    0x00000051
#define SRST_HRESETN_CORE_BIU                    0x00000052

// TOPCRU_SOFTRST_CON06(Offset:0x418)
#define SRST_ARESETN_NPU_BIU                     0x00000062
#define SRST_HRESETN_NPU_BIU                     0x00000063
#define SRST_ARESETN_RKNN                        0x00000064
#define SRST_HRESETN_RKNN                        0x00000065
#define SRST_RESETN_REF_PVTPLL_NPU               0x00000066

// TOPCRU_SOFTRST_CON08(Offset:0x420)
#define SRST_ARESETN_GPU_BIU                     0x00000083
#define SRST_RESETN_GPU                          0x00000084
#define SRST_RESETN_REF_PVTPLL_GPU               0x00000085
#define SRST_RESETN_GPU_BRG_BIU                  0x00000088

// TOPCRU_SOFTRST_CON09(Offset:0x424)
#define SRST_RESETN_RKVENC_CORE                  0x00000090
#define SRST_ARESETN_VEPU_BIU                    0x00000093
#define SRST_HRESETN_VEPU_BIU                    0x00000094
#define SRST_ARESETN_RKVENC                      0x00000095
#define SRST_HRESETN_RKVENC                      0x00000096

// TOPCRU_SOFTRST_CON10(Offset:0x428)
#define SRST_RESETN_RKVDEC_HEVC_CA               0x000000A2
#define SRST_ARESETN_VDPU_BIU                    0x000000A5
#define SRST_HRESETN_VDPU_BIU                    0x000000A6
#define SRST_ARESETN_RKVDEC                      0x000000A7
#define SRST_HRESETN_RKVDEC                      0x000000A8

// TOPCRU_SOFTRST_CON11(Offset:0x42C)
#define SRST_ARESETN_VI_BIU                      0x000000B3
#define SRST_HRESETN_VI_BIU                      0x000000B4
#define SRST_PRESETN_VI_BIU                      0x000000B5
#define SRST_RESETN_ISP                          0x000000B8
#define SRST_ARESETN_VICAP                       0x000000B9
#define SRST_HRESETN_VICAP                       0x000000BA
#define SRST_DRESETN_VICAP                       0x000000BB
#define SRST_I0RESETN_VICAP                      0x000000BC
#define SRST_I1RESETN_VICAP                      0x000000BD
#define SRST_I2RESETN_VICAP                      0x000000BE
#define SRST_I3RESETN_VICAP                      0x000000BF

// TOPCRU_SOFTRST_CON12(Offset:0x430)
#define SRST_PRESETN_CSIHOST0                    0x000000C0
#define SRST_PRESETN_CSIHOST1                    0x000000C1
#define SRST_PRESETN_CSIHOST2                    0x000000C2
#define SRST_PRESETN_CSIHOST3                    0x000000C3
#define SRST_PRESETN_CSIPHY0                     0x000000C4
#define SRST_PRESETN_CSIPHY1                     0x000000C5

// TOPCRU_SOFTRST_CON13(Offset:0x434)
#define SRST_ARESETN_VO_BIU                      0x000000D3
#define SRST_HRESETN_VO_BIU                      0x000000D4
#define SRST_ARESETN_VOP                         0x000000D6
#define SRST_HRESETN_VOP                         0x000000D7
#define SRST_DRESETN_VOP                         0x000000D8
#define SRST_DRESETN_VOP1                        0x000000D9

// TOPCRU_SOFTRST_CON14(Offset:0x438)
#define SRST_ARESETN_RGA_BIU                     0x000000E3
#define SRST_HRESETN_RGA_BIU                     0x000000E4
#define SRST_ARESETN_RGA                         0x000000E6
#define SRST_HRESETN_RGA                         0x000000E7
#define SRST_RESETN_RGA_CORE                     0x000000E8
#define SRST_ARESETN_JDEC                        0x000000E9
#define SRST_HRESETN_JDEC                        0x000000EA

// TOPCRU_SOFTRST_CON16(Offset:0x440)
#define SRST_PRESETN_PHP_BIU                     0x00000102
#define SRST_ARESETN_PHP_BIU                     0x00000103
#define SRST_PRESETN_PCIE20                      0x00000107
#define SRST_RESETN_PCIE20_POWERUP               0x00000108
#define SRST_RESETN_USB3OTG                      0x0000010A

// TOPCRU_SOFTRST_CON17(Offset:0x444)
#define SRST_RESETN_PIPEPHY                      0x00000113

// TOPCRU_SOFTRST_CON18(Offset:0x448)
#define SRST_ARESETN_BUS_BIU                     0x00000123
#define SRST_HRESETN_BUS_BIU                     0x00000124
#define SRST_PRESETN_BUS_BIU                     0x00000125

// TOPCRU_SOFTRST_CON19(Offset:0x44C)
#define SRST_PRESETN_I2C1                        0x00000130
#define SRST_PRESETN_I2C2                        0x00000131
#define SRST_PRESETN_I2C3                        0x00000132
#define SRST_PRESETN_I2C4                        0x00000133
#define SRST_PRESETN_I2C5                        0x00000134
#define SRST_RESETN_I2C1                         0x00000136
#define SRST_RESETN_I2C2                         0x00000137
#define SRST_RESETN_I2C3                         0x00000138
#define SRST_RESETN_I2C4                         0x00000139
#define SRST_RESETN_I2C5                         0x0000013A

// TOPCRU_SOFTRST_CON20(Offset:0x450)
#define SRST_RESETN_BUS_GPIO3                    0x00000145
#define SRST_RESETN_BUS_GPIO4                    0x00000146

// TOPCRU_SOFTRST_CON21(Offset:0x454)
#define SRST_PRESETN_TIMER                       0x00000150
#define SRST_RESETN_TIMER0                       0x00000151
#define SRST_RESETN_TIMER1                       0x00000152
#define SRST_RESETN_TIMER2                       0x00000153
#define SRST_RESETN_TIMER3                       0x00000154
#define SRST_RESETN_TIMER4                       0x00000155
#define SRST_RESETN_TIMER5                       0x00000156
#define SRST_PRESETN_STIMER                      0x00000157
#define SRST_RESETN_STIMER0                      0x00000158
#define SRST_RESETN_STIMER1                      0x00000159

// TOPCRU_SOFTRST_CON22(Offset:0x458)
#define SRST_PRESETN_WDTNS                       0x00000160
#define SRST_RESETN_WDTNS                        0x00000161
#define SRST_PRESETN_GRF                         0x00000162
#define SRST_PRESETN_SGRF                        0x00000163
#define SRST_PRESETN_MAILBOX                     0x00000164
#define SRST_PRESETN_INTC                        0x00000165
#define SRST_ARESETN_BUS_GIC400                  0x00000166
#define SRST_ARESETN_BUS_GIC400_DEBUG            0x00000167

// TOPCRU_SOFTRST_CON23(Offset:0x45C)
#define SRST_ARESETN_BUS_SPINLOCK                0x00000170
#define SRST_ARESETN_DCF                         0x00000171
#define SRST_PRESETN_DCF                         0x00000172
#define SRST_FRESETN_BUS_CM0_CORE                0x00000173
#define SRST_TRESETN_BUS_CM0_JTAG                0x00000175
#define SRST_HRESETN_ICACHE                      0x00000178
#define SRST_HRESETN_DCACHE                      0x00000179

// TOPCRU_SOFTRST_CON24(Offset:0x460)
#define SRST_PRESETN_TSADC                       0x00000180
#define SRST_RESETN_TSADC                        0x00000181
#define SRST_RESETN_TSADCPHY                     0x00000182
#define SRST_PRESETN_DFT2APB                     0x00000184

// TOPCRU_SOFTRST_CON25(Offset:0x464)
#define SRST_ARESETN_GMAC                        0x00000191
#define SRST_PRESETN_APB2ASB_VCCIO156            0x00000195
#define SRST_PRESETN_DSIPHY                      0x00000198
#define SRST_PRESETN_DSITX                       0x00000199
#define SRST_PRESETN_CPU_EMA_DET                 0x0000019A
#define SRST_PRESETN_HASH                        0x0000019B
#define SRST_PRESETN_TOPCRU                      0x0000019F

// TOPCRU_SOFTRST_CON26(Offset:0x468)
#define SRST_PRESETN_ASB2APB_VCCIO156            0x000001A0
#define SRST_PRESETN_IOC_VCCIO156                0x000001A1
#define SRST_PRESETN_GPIO3_VCCIO156              0x000001A2
#define SRST_PRESETN_GPIO4_VCCIO156              0x000001A3
#define SRST_PRESETN_SARADC_VCCIO156             0x000001A4
#define SRST_RESETN_SARADC_VCCIO156              0x000001A5
#define SRST_RESETN_SARADC_VCCIO156_PHY          0x000001A6

// TOPCRU_SOFTRST_CON27(Offset:0x46C)
#define SRST_ARESETN_MAC100                      0x000001B1

// TOPCRU_GATE_CON00(Offset:0x300)
#define CLK_MATRIX_50M_SRC_GATE                  0x00000000
#define CLK_MATRIX_100M_SRC_GATE                 0x00000001
#define CLK_MATRIX_125M_SRC_GATE                 0x00000002
#define CLK_MATRIX_200M_SRC_GATE                 0x00000004
#define CLK_MATRIX_300M_SRC_GATE                 0x00000006
#define CLK_TESTOUT_TOP_GATE                     0x0000000F

// TOPCRU_GATE_CON01(Offset:0x304)
#define ACLK_TOP_BIU_GATE                        0x00000010
#define ACLK_TOP_VIO_BIU_GATE                    0x00000011
#define CLK_REF_PVTPLL_LOGIC_GATE                0x00000012
#define CLK_PVTPLL_LOGIC_O_GATE                  0x00000013
#define CLK_GPLL_MUX_TO_DDR_GATE                 0x00000016
#define CLK_GPLL_MUX_TO_PERI_GATE                0x00000017
#define CLK_HPLL_MUX_TO_PERI_GATE                0x00000018
#define CLK_24M_SSCSRC_GATE                      0x00000019
#define CLK_CAM0_OUT2IO_GATE                     0x0000001C
#define CLK_CAM1_OUT2IO_GATE                     0x0000001D
#define CLK_CAM2_OUT2IO_GATE                     0x0000001E
#define CLK_CAM3_OUT2IO_GATE                     0x0000001F

// TOPCRU_GATE_CON04(Offset:0x310)
#define CLK_CORE_GATE                            0x00000040
#define CLK_CORE_LEV1PRE_GATE                    0x00000041
#define CLK_CORE_PRE_GATE                        0x00000042
#define ACLK_CORE_PRE_GATE                       0x00000043
#define CLK_SCANHS_ACLKM_CORE_GATE               0x00000044
#define PCLK_DBG_PRE_GATE                        0x00000045
#define CLK_SCANHS_PCLK_DBG_GATE                 0x00000046
#define CLK_REF_PVTPLL_CORE_GATE                 0x0000004D
#define CLK_TESTOUT_CORE_GATE                    0x0000004E

// TOPCRU_GATE_CON05(Offset:0x314)
#define ACLK_CORE_BIU_GATE                       0x00000050
#define PCLK_CORE_BIU_GATE                       0x00000051
#define HCLK_CORE_BIU_GATE                       0x00000052
#define CLK_APLL_I_GATE                          0x0000005E
#define CLK_APLL_G_GATE                          0x0000005F

// TOPCRU_GATE_CON06(Offset:0x318)
#define CLK_NPU_PRE_GATE                         0x00000060
#define HCLK_NPU_PRE_GATE                        0x00000061
#define ACLK_NPU_BIU_GATE                        0x00000062
#define HCLK_NPU_BIU_GATE                        0x00000063
#define ACLK_RKNN_GATE                           0x00000064
#define HCLK_RKNN_GATE                           0x00000065
#define CLK_REF_PVTPLL_NPU_GATE                  0x00000066
#define CLK_TESTOUT_NPU_GATE                     0x00000067

// TOPCRU_GATE_CON08(Offset:0x320)
#define CLK_GPU_LEV1PRE_GATE                     0x00000080
#define CLK_GPU_PRE_GATE                         0x00000081
#define ACLK_GPU_PRE_GATE                        0x00000082
#define ACLK_GPU_BIU_GATE                        0x00000083
#define CLK_GPU_GATE                             0x00000084
#define CLK_REF_PVTPLL_GPU_GATE                  0x00000085
#define CLK_TESTOUT_GPU_GATE                     0x00000086
#define CLK_GPU_BRG_BIU_GATE                     0x00000088

// TOPCRU_GATE_CON09(Offset:0x324)
#define CLK_RKVENC_CORE_GATE                     0x00000090
#define ACLK_VEPU_PRE_GATE                       0x00000091
#define HCLK_VEPU_PRE_GATE                       0x00000092
#define ACLK_VEPU_BIU_GATE                       0x00000093
#define HCLK_VEPU_BIU_GATE                       0x00000094
#define ACLK_RKVENC_GATE                         0x00000095
#define HCLK_RKVENC_GATE                         0x00000096
#define CLK_TESTOUT_VEPU_GATE                    0x00000097

// TOPCRU_GATE_CON10(Offset:0x328)
#define ACLK_VDPU_PRE_GATE                       0x000000A0
#define CLK_RKVDEC_HEVC_CA_GATE                  0x000000A3
#define HCLK_VDPU_PRE_GATE                       0x000000A4
#define ACLK_VDPU_BIU_GATE                       0x000000A5
#define HCLK_VDPU_BIU_GATE                       0x000000A6
#define ACLK_RKVDEC_GATE                         0x000000A7
#define HCLK_RKVDEC_GATE                         0x000000A8
#define CLK_TESTOUT_VDPU_GATE                    0x000000A9

// TOPCRU_GATE_CON11(Offset:0x32C)
#define ACLK_VI_GATE                             0x000000B0
#define HCLK_VI_GATE                             0x000000B1
#define PCLK_VI_GATE                             0x000000B2
#define ACLK_VI_BIU_GATE                         0x000000B3
#define HCLK_VI_BIU_GATE                         0x000000B4
#define PCLK_VI_BIU_GATE                         0x000000B5
#define ACLK_ISP_GATE                            0x000000B6
#define HCLK_ISP_GATE                            0x000000B7
#define CLK_ISP_GATE                             0x000000B8
#define ACLK_VICAP_GATE                          0x000000B9
#define HCLK_VICAP_GATE                          0x000000BA
#define DCLK_VICAP_GATE                          0x000000BB
#define CSIRX0_CLK_DATA_GATE                     0x000000BC
#define CSIRX1_CLK_DATA_GATE                     0x000000BD
#define CSIRX2_CLK_DATA_GATE                     0x000000BE
#define CSIRX3_CLK_DATA_GATE                     0x000000BF

// TOPCRU_GATE_CON12(Offset:0x330)
#define PCLK_CSIHOST0_GATE                       0x000000C0
#define PCLK_CSIHOST1_GATE                       0x000000C1
#define PCLK_CSIHOST2_GATE                       0x000000C2
#define PCLK_CSIHOST3_GATE                       0x000000C3
#define PCLK_CSIPHY0_GATE                        0x000000C4
#define PCLK_CSIPHY1_GATE                        0x000000C5
#define CLK_RXBYTEHS_CSIHOST0_DFT_GATE           0x000000C8
#define CLK_RXBYTEHS_CSIHOST1_DFT_GATE           0x000000C9
#define CLK_RXBYTEHS_CSIHOST2_DFT_GATE           0x000000CA
#define CLK_RXBYTEHS_CSIHOST3_DFT_GATE           0x000000CB
#define CLK_TESTOUT_VIPHY_GATE                   0x000000CF

// TOPCRU_GATE_CON13(Offset:0x334)
#define ACLK_VO_PRE_GATE                         0x000000D0
#define HCLK_VO_PRE_GATE                         0x000000D1
#define ACLK_VO_BIU_GATE                         0x000000D3
#define HCLK_VO_BIU_GATE                         0x000000D4
#define ACLK_VOP_GATE                            0x000000D6
#define HCLK_VOP_GATE                            0x000000D7
#define DCLK_VOP_GATE                            0x000000D8
#define DCLK_VOP1_GATE                           0x000000D9
#define CLK_TESTOUT_VO_GATE                      0x000000DF

// TOPCRU_GATE_CON14(Offset:0x338)
#define ACLK_RGA_PRE_GATE                        0x000000E0
#define HCLK_RGA_PRE_GATE                        0x000000E1
#define ACLK_RGA_BIU_GATE                        0x000000E3
#define HCLK_RGA_BIU_GATE                        0x000000E4
#define ACLK_RGA_GATE                            0x000000E6
#define HCLK_RGA_GATE                            0x000000E7
#define CLK_RGA_CORE_GATE                        0x000000E8
#define ACLK_JDEC_GATE                           0x000000E9
#define HCLK_JDEC_GATE                           0x000000EA
#define CLK_TESTOUT_RGA_GATE                     0x000000EF

// TOPCRU_GATE_CON16(Offset:0x340)
#define ACLK_PHP_GATE                            0x00000100
#define PCLK_PHP_GATE                            0x00000101
#define PCLK_PHP_BIU_GATE                        0x00000102
#define ACLK_PHP_BIU_GATE                        0x00000103
#define ACLK_PCIE20_MST_GATE                     0x00000104
#define ACLK_PCIE20_SLV_GATE                     0x00000105
#define ACLK_PCIE20_DBI_GATE                     0x00000106
#define PCLK_PCIE20_GATE                         0x00000107
#define CLK_PCIE20_AUX_GATE                      0x00000108
#define ACLK_USB3OTG_GATE                        0x0000010A
#define CLK_USB3OTG_SUSPEND_GATE                 0x0000010B
#define CLK_USB3OTG_REF_GATE                     0x0000010C

// TOPCRU_GATE_CON17(Offset:0x344)
#define CLK_PIPEPHY_GATE                         0x00000110
#define CLK_USB3OTG_PIPE_DFT_GATE                0x00000111
#define CLK_USBPHY_USB3OTG_UTMI_DFT_GATE         0x00000112
#define CLK_PIPEPHY_REF_FUNC_GATE                0x00000113
#define CLK_TESTOUT_PHPPHY_GATE                  0x0000011F

// TOPCRU_GATE_CON18(Offset:0x348)
#define ACLK_BUS_GATE                            0x00000120
#define HCLK_BUS_GATE                            0x00000121
#define PCLK_BUS_GATE                            0x00000122
#define ACLK_BUS_BIU_GATE                        0x00000123
#define HCLK_BUS_BIU_GATE                        0x00000124
#define PCLK_BUS_BIU_GATE                        0x00000125

// TOPCRU_GATE_CON19(Offset:0x34C)
#define PCLK_I2C1_GATE                           0x00000130
#define PCLK_I2C2_GATE                           0x00000131
#define PCLK_I2C3_GATE                           0x00000132
#define PCLK_I2C4_GATE                           0x00000133
#define PCLK_I2C5_GATE                           0x00000134
#define CLK_I2C_GATE                             0x00000135
#define CLK_I2C1_GATE                            0x00000136
#define CLK_I2C2_GATE                            0x00000137
#define CLK_I2C3_GATE                            0x00000138
#define CLK_I2C4_GATE                            0x00000139
#define CLK_I2C5_GATE                            0x0000013A

// TOPCRU_GATE_CON20(Offset:0x350)
#define DCLK_BUS_GPIO_GATE                       0x00000144
#define DCLK_BUS_GPIO3_GATE                      0x00000145
#define DCLK_BUS_GPIO4_GATE                      0x00000146

// TOPCRU_GATE_CON21(Offset:0x354)
#define PCLK_TIMER_GATE                          0x00000150
#define CLK_TIMER0_GATE                          0x00000151
#define CLK_TIMER1_GATE                          0x00000152
#define CLK_TIMER2_GATE                          0x00000153
#define CLK_TIMER3_GATE                          0x00000154
#define CLK_TIMER4_GATE                          0x00000155
#define CLK_TIMER5_GATE                          0x00000156
#define PCLK_STIMER_GATE                         0x00000157
#define CLK_STIMER0_GATE                         0x00000158
#define CLK_STIMER1_GATE                         0x00000159

// TOPCRU_GATE_CON22(Offset:0x358)
#define PCLK_WDTNS_GATE                          0x00000160
#define CLK_WDTNS_GATE                           0x00000161
#define PCLK_GRF_GATE                            0x00000162
#define PCLK_SGRF_GATE                           0x00000163
#define PCLK_MAILBOX_GATE                        0x00000164
#define PCLK_INTC_GATE                           0x00000165
#define ACLK_BUS_GIC400_GATE                     0x00000166

// TOPCRU_GATE_CON23(Offset:0x35C)
#define ACLK_BUS_SPINLOCK_GATE                   0x00000170
#define ACLK_DCF_GATE                            0x00000171
#define PCLK_DCF_GATE                            0x00000172
#define FCLK_BUS_CM0_CORE_GATE                   0x00000173
#define CLK_BUS_CM0_RTC_GATE                     0x00000174
#define HCLK_ICACHE_GATE                         0x00000178
#define HCLK_DCACHE_GATE                         0x00000179

// TOPCRU_GATE_CON24(Offset:0x360)
#define PCLK_TSADC_GATE                          0x00000180
#define CLK_TSADC_GATE                           0x00000181
#define CLK_TSADC_TSEN_GATE                      0x00000183
#define PCLK_DFT2APB_GATE                        0x00000184
#define CLK_SARADC_VCCIO156_GATE                 0x00000189

// TOPCRU_GATE_CON25(Offset:0x364)
#define PCLK_GMAC_GATE                           0x00000190
#define ACLK_GMAC_GATE                           0x00000191
#define CLK_GMAC_125M_CRU_GATE                   0x00000192
#define CLK_GMAC_50M_CRU_GATE                    0x00000193
#define CLK_GMAC_ETH_OUT2IO_GATE                 0x00000194
#define PCLK_APB2ASB_VCCIO156_GATE               0x00000195
#define PCLK_TO_VCCIO156_GATE                    0x00000196
#define PCLK_DSIPHY_GATE                         0x00000198
#define PCLK_DSITX_GATE                          0x00000199
#define PCLK_CPU_EMA_DET_GATE                    0x0000019A
#define PCLK_HASH_GATE                           0x0000019B
#define CLK_TESTOUT_BUS_GATE                     0x0000019E
#define PCLK_TOPCRU_GATE                         0x0000019F

// TOPCRU_GATE_CON26(Offset:0x368)
#define PCLK_ASB2APB_VCCIO156_GATE               0x000001A0
#define PCLK_IOC_VCCIO156_GATE                   0x000001A1
#define PCLK_GPIO3_VCCIO156_GATE                 0x000001A2
#define PCLK_GPIO4_VCCIO156_GATE                 0x000001A3
#define PCLK_SARADC_VCCIO156_GATE                0x000001A4

// TOPCRU_GATE_CON27(Offset:0x36C)
#define PCLK_MAC100_GATE                         0x000001B0
#define ACLK_MAC100_GATE                         0x000001B1
#define CLK_MAC100_50M_CRU_GATE                  0x000001B2

// TOPCRU_CLKSEL_CON00(Offset:0x100)
#define CLK_MATRIX_50M_SRC_DIV                   0x05000000
#define CLK_MATRIX_100M_SRC_DIV                  0x04080000
#define CLK_MATRIX_50M_SRC_SEL                   0x01070000
#define CLK_MATRIX_50M_SRC_SEL_CLK_GPLL_MUX      0U
#define CLK_MATRIX_50M_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_MATRIX_100M_SRC_SEL                  0x010F0000
#define CLK_MATRIX_100M_SRC_SEL_CLK_GPLL_MUX     0U
#define CLK_MATRIX_100M_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U

// TOPCRU_CLKSEL_CON01(Offset:0x104)
#define CLK_MATRIX_125M_SRC_DIV                  0x04000001
#define CLK_MATRIX_125M_SRC_SEL                  0x01070001
#define CLK_MATRIX_125M_SRC_SEL_CLK_GPLL_MUX     0U
#define CLK_MATRIX_125M_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U

// TOPCRU_CLKSEL_CON02(Offset:0x108)
#define CLK_MATRIX_200M_SRC_DIV                  0x04000002
#define CLK_MATRIX_200M_SRC_SEL                  0x01070002
#define CLK_MATRIX_200M_SRC_SEL_CLK_GPLL_MUX     0U
#define CLK_MATRIX_200M_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U

// TOPCRU_CLKSEL_CON03(Offset:0x10C)
#define CLK_MATRIX_300M_SRC_DIV                  0x04000003
#define CLK_MATRIX_300M_SRC_SEL                  0x01070003
#define CLK_MATRIX_300M_SRC_SEL_CLK_GPLL_MUX     0U
#define CLK_MATRIX_300M_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U

// TOPCRU_CLKSEL_CON04(Offset:0x110)
#define CLK_TESTOUT_TOP_DIV                      0x08000004
#define CLK_TESTOUT_TOP_SEL                      0x05080004

// TOPCRU_CLKSEL_CON05(Offset:0x114)
#define ACLK_TOP_BIU_DIV                         0x04000005
#define ACLK_TOP_VIO_BIU_DIV                     0x04080005
#define ACLK_TOP_BIU_SEL                         0x01070005
#define ACLK_TOP_BIU_SEL_CLK_GPLL_MUX            0U
#define ACLK_TOP_BIU_SEL_CLK_CPLL_MUX_FROM_PMU1  1U
#define ACLK_TOP_VIO_BIU_SEL                     0x010F0005
#define ACLK_TOP_VIO_BIU_SEL_CLK_GPLL_MUX        0U
#define ACLK_TOP_VIO_BIU_SEL_CLK_CPLL_MUX_FROM_PMU1 1U

// TOPCRU_CLKSEL_CON06(Offset:0x118)
#define CLK_24M_SSCSRC_DIV                       0x06000006
#define CLK_24M_SSCSRC_SEL                       0x02060006
#define CLK_24M_SSCSRC_SEL_CLK_VPLL_MUX          0U
#define CLK_24M_SSCSRC_SEL_CLK_HPLL_MUX          1U
#define CLK_24M_SSCSRC_SEL_CLK_GPLL_MUX          2U
#define CLK_24M_SSCSRC_SEL_CLK_CPLL_MUX_FROM_PMU1 3U
#define CLK_PVTPLL_LOGIC_O_SEL                   0x01080006
#define CLK_PVTPLL_LOGIC_O_SEL_CLK_PVTPLL_LOGIC  0U
#define CLK_PVTPLL_LOGIC_O_SEL_XIN_OSC0_FUNC     1U

// TOPCRU_CLKSEL_CON08(Offset:0x120)
#define CLK_CAM0_OUT2IO_DIV                      0x06000008
#define CLK_CAM1_OUT2IO_DIV                      0x06080008
#define CLK_CAM0_OUT2IO_SEL                      0x02060008
#define CLK_CAM0_OUT2IO_SEL_CLK_GPLL_MUX         0U
#define CLK_CAM0_OUT2IO_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_CAM0_OUT2IO_SEL_XIN_OSC0_FUNC        2U
#define CLK_CAM0_OUT2IO_SEL_CLK_APLL_MUX         3U
#define CLK_CAM1_OUT2IO_SEL                      0x020E0008
#define CLK_CAM1_OUT2IO_SEL_CLK_GPLL_MUX         0U
#define CLK_CAM1_OUT2IO_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_CAM1_OUT2IO_SEL_XIN_OSC0_FUNC        2U
#define CLK_CAM1_OUT2IO_SEL_CLK_APLL_MUX         3U

// TOPCRU_CLKSEL_CON09(Offset:0x124)
#define CLK_CAM2_OUT2IO_DIV                      0x06000009
#define CLK_CAM3_OUT2IO_DIV                      0x06080009
#define CLK_CAM2_OUT2IO_SEL                      0x02060009
#define CLK_CAM2_OUT2IO_SEL_CLK_GPLL_MUX         0U
#define CLK_CAM2_OUT2IO_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_CAM2_OUT2IO_SEL_XIN_OSC0_FUNC        2U
#define CLK_CAM2_OUT2IO_SEL_CLK_APLL_MUX         3U
#define CLK_CAM3_OUT2IO_SEL                      0x020E0009
#define CLK_CAM3_OUT2IO_SEL_CLK_GPLL_MUX         0U
#define CLK_CAM3_OUT2IO_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_CAM3_OUT2IO_SEL_XIN_OSC0_FUNC        2U
#define CLK_CAM3_OUT2IO_SEL_CLK_APLL_MUX         3U

// TOPCRU_CLKSEL_CON10(Offset:0x128)
#define CLK_CORE_LEV1PRE_DIV                     0x0500000A
#define CLK_CORE_LEV1PRE_SEL                     0x0107000A
#define CLK_CORE_LEV1PRE_SEL_CLK_APLL_MUX        0U
#define CLK_CORE_LEV1PRE_SEL_CLK_GPLL_MUX        1U
#define CLK_CORE_PVTPLL_SRC_SEL                  0x0108000A
#define CLK_CORE_PVTPLL_SRC_SEL_CLK_DEEPSLOW     0U
#define CLK_CORE_PVTPLL_SRC_SEL_CLK_CORE_PVTPLL  1U
#define CLK_CORE_PRE_NDFT_SEL                    0x010E000A
#define CLK_CORE_PRE_NDFT_SEL_I_CLK_CORE_PRE     0U
#define CLK_CORE_PRE_NDFT_SEL_CLK_APLL_G         1U
#define CLK_CORE_I_SEL                           0x010F000A
#define CLK_CORE_I_SEL_CLK_CORE_PRE              0U
#define CLK_CORE_I_SEL_CLK_CORE_PVTPLL_SRC       1U

// TOPCRU_CLKSEL_CON11(Offset:0x12C)
#define ACLK_CORE_PRE_DIV                        0x0300000B
#define CLK_SCANHS_ACLKM_CORE_DIV                0x0308000B

// TOPCRU_CLKSEL_CON12(Offset:0x130)
#define PCLK_DBG_PRE_DIV                         0x0400000C
#define CLK_SCANHS_PCLK_DBG_DIV                  0x0408000C

// TOPCRU_CLKSEL_CON13(Offset:0x134)
#define HCLK_CORE_BIU_DIV                        0x0600000D

// TOPCRU_CLKSEL_CON14(Offset:0x138)
#define CLK_TESTOUT_CORE_DIV                     0x0308000E
#define CLK_TESTOUT_CORE_SEL                     0x010F000E
#define CLK_TESTOUT_CORE_SEL_TEST_CLK_PVTPLL_CORE 0U
#define CLK_TESTOUT_CORE_SEL_CLK_CORE            1U

// TOPCRU_CLKSEL_CON15(Offset:0x13C)
#define CLK_NPU_PRE_DIV                          0x0400000F
#define CLK_NPU_PRE_SEL                          0x0107000F
#define CLK_NPU_PRE_SEL_CLK_GPLL_MUX             0U
#define CLK_NPU_PRE_SEL_CLK_CPLL_MUX_FROM_PMU1   1U
#define CLK_NPU_PVTPLL_SRC_SEL                   0x0108000F
#define CLK_NPU_PVTPLL_SRC_SEL_CLK_DEEPSLOW      0U
#define CLK_NPU_PVTPLL_SRC_SEL_CLK_NPU_PVTPLL    1U
#define CLK_NPU_I_SEL                            0x010F000F
#define CLK_NPU_I_SEL_CLK_NPU_PRE                0U
#define CLK_NPU_I_SEL_CLK_NPU_PVTPLL_SRC         1U

// TOPCRU_CLKSEL_CON16(Offset:0x140)
#define HCLK_NPU_PRE_DIV                         0x04000010

// TOPCRU_CLKSEL_CON17(Offset:0x144)
#define CLK_TESTOUT_NPU_DIV                      0x03000011
#define CLK_TESTOUT_NPU_SEL                      0x01070011
#define CLK_TESTOUT_NPU_SEL_TEST_CLK_PVTPLL_NPU  0U
#define CLK_TESTOUT_NPU_SEL_CLK_NPU              1U

// TOPCRU_CLKSEL_CON18(Offset:0x148)
#define CLK_GPU_LEV1PRE_DIV                      0x04000012
#define CLK_GPU_LEV1PRE_SEL                      0x01070012
#define CLK_GPU_LEV1PRE_SEL_CLK_GPLL_MUX         0U
#define CLK_GPU_LEV1PRE_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_GPU_PVTPLL_SRC_SEL                   0x01080012
#define CLK_GPU_PVTPLL_SRC_SEL_CLK_DEEPSLOW      0U
#define CLK_GPU_PVTPLL_SRC_SEL_CLK_GPU_PVTPLL    1U
#define CLK_GPU_I_SEL                            0x010F0012
#define CLK_GPU_I_SEL_CLK_GPU_PRE                0U
#define CLK_GPU_I_SEL_CLK_GPU_PVTPLL_SRC         1U

// TOPCRU_CLKSEL_CON19(Offset:0x14C)
#define ACLK_GPU_PRE_DIV                         0x04000013
#define CLK_TESTOUT_GPU_DIV                      0x03040013
#define CLK_TESTOUT_GPU_SEL                      0x01070013
#define CLK_TESTOUT_GPU_SEL_TEST_CLK_PVTPLL_GPU  0U
#define CLK_TESTOUT_GPU_SEL_CLK_GPU              1U
#define CLK_GPU_BRG_BIU_SEL                      0x010F0013
#define CLK_GPU_BRG_BIU_SEL_CLK_MATRIX_200M_SRC  0U
#define CLK_GPU_BRG_BIU_SEL_CLK_MATRIX_100M_SRC  1U

// TOPCRU_CLKSEL_CON20(Offset:0x150)
#define CLK_RKVENC_CORE_DIV                      0x05000014
#define ACLK_VEPU_PRE_DIV                        0x05080014
#define CLK_RKVENC_CORE_SEL                      0x02060014
#define CLK_RKVENC_CORE_SEL_CLK_GPLL_MUX         0U
#define CLK_RKVENC_CORE_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_RKVENC_CORE_SEL_CLK_PVTPLL_LOGIC_O   2U
#define CLK_RKVENC_CORE_SEL_CLK_APLL_MUX         3U
#define ACLK_VEPU_PRE_SEL                        0x020E0014
#define ACLK_VEPU_PRE_SEL_CLK_GPLL_MUX           0U
#define ACLK_VEPU_PRE_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define ACLK_VEPU_PRE_SEL_CLK_PVTPLL_LOGIC_O     2U
#define ACLK_VEPU_PRE_SEL_CLK_APLL_MUX           3U

// TOPCRU_CLKSEL_CON21(Offset:0x154)
#define HCLK_VEPU_PRE_DIV                        0x04000015
#define CLK_TESTOUT_VEPU_SEL                     0x010F0015
#define CLK_TESTOUT_VEPU_SEL_CLK_RKVENC_CORE     0U
#define CLK_TESTOUT_VEPU_SEL_ACLK_VEPU_PRE       1U

// TOPCRU_CLKSEL_CON22(Offset:0x158)
#define ACLK_VDPU_PRE_DIV                        0x05000016
#define ACLK_VDPU_PRE_SEL                        0x02060016
#define ACLK_VDPU_PRE_SEL_CLK_GPLL_MUX           0U
#define ACLK_VDPU_PRE_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define ACLK_VDPU_PRE_SEL_CLK_PVTPLL_LOGIC_O     2U
#define ACLK_VDPU_PRE_SEL_CLK_APLL_MUX           3U

// TOPCRU_CLKSEL_CON23(Offset:0x15C)
#define CLK_RKVDEC_HEVC_CA_DIV                   0x05080017
#define CLK_RKVDEC_HEVC_CA_SEL                   0x020E0017
#define CLK_RKVDEC_HEVC_CA_SEL_CLK_GPLL_MUX      0U
#define CLK_RKVDEC_HEVC_CA_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_RKVDEC_HEVC_CA_SEL_CLK_PVTPLL_LOGIC_O 2U
#define CLK_RKVDEC_HEVC_CA_SEL_CLK_APLL_MUX      3U

// TOPCRU_CLKSEL_CON24(Offset:0x160)
#define HCLK_VDPU_PRE_DIV                        0x04000018
#define CLK_TESTOUT_VDPU_SEL                     0x010F0018
#define CLK_TESTOUT_VDPU_SEL_CLK_RKVDEC_HEVC_CA  0U
#define CLK_TESTOUT_VDPU_SEL_ACLK_VDPU_PRE       1U

// TOPCRU_CLKSEL_CON25(Offset:0x164)
#define ACLK_VI_DIV                              0x04000019
#define ACLK_VI_SEL                              0x02060019
#define ACLK_VI_SEL_CLK_GPLL_MUX                 0U
#define ACLK_VI_SEL_CLK_CPLL_MUX_FROM_PMU1       1U
#define ACLK_VI_SEL_CLK_PVTPLL_LOGIC_O           2U
#define ACLK_VI_SEL_CLK_APLL_MUX                 3U

// TOPCRU_CLKSEL_CON26(Offset:0x168)
#define HCLK_VI_DIV                              0x0400001A
#define PCLK_VI_DIV                              0x0408001A
#define CLK_TESTOUT_VIPHY_SEL                    0x040C001A
#define CLK_TESTOUT_VIPHY_SEL_CLK_ISP            0U
#define CLK_TESTOUT_VIPHY_SEL_DCLK_VICAP         1U
#define CLK_TESTOUT_VIPHY_SEL_CSIRX0_CLK_DATA    2U
#define CLK_TESTOUT_VIPHY_SEL_CSIRX1_CLK_DATA    3U
#define CLK_TESTOUT_VIPHY_SEL_CSIRX2_CLK_DATA    4U
#define CLK_TESTOUT_VIPHY_SEL_CSIRX3_CLK_DATA    5U
#define CLK_TESTOUT_VIPHY_SEL_CSIPHY0_LANEBYRXCLK0 6U
#define CLK_TESTOUT_VIPHY_SEL_CSIPHY0_LANEBYRXCLK1 7U
#define CLK_TESTOUT_VIPHY_SEL_CSIPHY1_LANEBYRXCLK0 8U
#define CLK_TESTOUT_VIPHY_SEL_CSIPHY1_LANEBYRXCLK1 9U

// TOPCRU_CLKSEL_CON27(Offset:0x16C)
#define CLK_ISP_DIV                              0x0400001B
#define DCLK_VICAP_DIV                           0x0408001B
#define CLK_ISP_SEL                              0x0206001B
#define CLK_ISP_SEL_CLK_GPLL_MUX                 0U
#define CLK_ISP_SEL_CLK_CPLL_MUX_FROM_PMU1       1U
#define CLK_ISP_SEL_CLK_PVTPLL_LOGIC_O           2U
#define CLK_ISP_SEL_CLK_APLL_MUX                 3U
#define DCLK_VICAP_SEL                           0x020E001B
#define DCLK_VICAP_SEL_CLK_GPLL_MUX              0U
#define DCLK_VICAP_SEL_CLK_CPLL_MUX_FROM_PMU1    1U
#define DCLK_VICAP_SEL_CLK_PVTPLL_LOGIC_O        2U
#define DCLK_VICAP_SEL_CLK_APLL_MUX              3U

// TOPCRU_CLKSEL_CON28(Offset:0x170)
#define ACLK_VO_PRE_DIV                          0x0500001C
#define ACLK_VO_PRE_SEL                          0x0206001C
#define ACLK_VO_PRE_SEL_CLK_GPLL_MUX             0U
#define ACLK_VO_PRE_SEL_CLK_CPLL_MUX_FROM_PMU1   1U
#define ACLK_VO_PRE_SEL_CLK_VPLL_MUX             2U
#define ACLK_VO_PRE_SEL_CLK_HPLL_MUX             3U

// TOPCRU_CLKSEL_CON29(Offset:0x174)
#define HCLK_VO_PRE_DIV                          0x0500001D

// TOPCRU_CLKSEL_CON30(Offset:0x178)
#define DCLK_VOP_DIV                             0x0800001E
#define DCLK_VOP_SEL                             0x020E001E
#define DCLK_VOP_SEL_CLK_GPLL_MUX                0U
#define DCLK_VOP_SEL_CLK_HPLL_MUX                1U
#define DCLK_VOP_SEL_CLK_VPLL_MUX                2U
#define DCLK_VOP_SEL_CLK_APLL_MUX                3U

// TOPCRU_CLKSEL_CON31(Offset:0x17C)
#define DCLK_VOP1_DIV                            0x0800001F
#define CLK_TESTOUT_VO_SEL                       0x0208001F
#define CLK_TESTOUT_VO_SEL_ACLK_VO_PRE           0U
#define CLK_TESTOUT_VO_SEL_DCLK_VOP              1U
#define CLK_TESTOUT_VO_SEL_DCLK_VOP1             2U
#define DCLK_VOP1_SEL                            0x020E001F
#define DCLK_VOP1_SEL_CLK_GPLL_MUX               0U
#define DCLK_VOP1_SEL_CLK_HPLL_MUX               1U
#define DCLK_VOP1_SEL_CLK_VPLL_MUX               2U
#define DCLK_VOP1_SEL_CLK_APLL_MUX               3U

// TOPCRU_CLKSEL_CON32(Offset:0x180)
#define ACLK_RGA_PRE_DIV                         0x04000020
#define HCLK_RGA_PRE_DIV                         0x03080020
#define ACLK_RGA_PRE_SEL                         0x02060020
#define ACLK_RGA_PRE_SEL_CLK_GPLL_MUX            0U
#define ACLK_RGA_PRE_SEL_CLK_CPLL_MUX_FROM_PMU1  1U
#define ACLK_RGA_PRE_SEL_CLK_PVTPLL_LOGIC_O      2U
#define ACLK_RGA_PRE_SEL_CLK_APLL_MUX            3U

// TOPCRU_CLKSEL_CON33(Offset:0x184)
#define CLK_RGA_CORE_DIV                         0x04000021
#define CLK_RGA_CORE_SEL                         0x02060021
#define CLK_RGA_CORE_SEL_CLK_GPLL_MUX            0U
#define CLK_RGA_CORE_SEL_CLK_CPLL_MUX_FROM_PMU1  1U
#define CLK_RGA_CORE_SEL_CLK_PVTPLL_LOGIC_O      2U
#define CLK_RGA_CORE_SEL_CLK_APLL_MUX            3U
#define CLK_TESTOUT_RGA_SEL                      0x010F0021
#define CLK_TESTOUT_RGA_SEL_ACLK_RGA_PRE         0U
#define CLK_TESTOUT_RGA_SEL_CLK_RGA_CORE         1U

// TOPCRU_CLKSEL_CON36(Offset:0x190)
#define ACLK_PHP_DIV                             0x04000024
#define PCLK_PHP_DIV                             0x04080024
#define ACLK_PHP_SEL                             0x01070024
#define ACLK_PHP_SEL_CLK_GPLL_MUX                0U
#define ACLK_PHP_SEL_CLK_CPLL_MUX_FROM_PMU1      1U
#define CLK_USB3OTG_SUSPEND_SEL                  0x010F0024
#define CLK_USB3OTG_SUSPEND_SEL_XIN_OSC0_FUNC    0U
#define CLK_USB3OTG_SUSPEND_SEL_CLK_DEEPSLOW     1U

// TOPCRU_CLKSEL_CON37(Offset:0x194)
#define CLK_TESTOUT_PHPPHY_SEL                   0x02060025
#define CLK_TESTOUT_PHPPHY_SEL_ACLK_PHP          0U
#define CLK_TESTOUT_PHPPHY_SEL_CLK_PIPEPHY_PIPE  1U
#define CLK_TESTOUT_PHPPHY_SEL_CLK_USBPHY_USB3OTG_UTMI 2U

// TOPCRU_CLKSEL_CON40(Offset:0x1A0)
#define ACLK_BUS_DIV                             0x05000028
#define HCLK_BUS_DIV                             0x06080028
#define ACLK_BUS_SEL                             0x01070028
#define ACLK_BUS_SEL_CLK_GPLL_MUX                0U
#define ACLK_BUS_SEL_CLK_CPLL_MUX_FROM_PMU1      1U
#define HCLK_BUS_SEL                             0x010F0028
#define HCLK_BUS_SEL_CLK_GPLL_MUX                0U
#define HCLK_BUS_SEL_CLK_CPLL_MUX_FROM_PMU1      1U

// TOPCRU_CLKSEL_CON41(Offset:0x1A4)
#define PCLK_BUS_DIV                             0x05000029
#define PCLK_BUS_SEL                             0x01070029
#define PCLK_BUS_SEL_CLK_GPLL_MUX                0U
#define PCLK_BUS_SEL_CLK_CPLL_MUX_FROM_PMU1      1U
#define CLK_I2C_SEL                              0x02080029
#define CLK_I2C_SEL_CLK_MATRIX_200M_SRC          0U
#define CLK_I2C_SEL_CLK_MATRIX_100M_SRC          1U
#define CLK_I2C_SEL_CLK_MATRIX_50M_SRC           2U
#define CLK_I2C_SEL_XIN_OSC0_FUNC                3U
#define DCLK_BUS_GPIO_SEL                        0x010F0029
#define DCLK_BUS_GPIO_SEL_XIN_OSC0_FUNC          0U
#define DCLK_BUS_GPIO_SEL_CLK_DEEPSLOW           1U

// TOPCRU_CLKSEL_CON43(Offset:0x1AC)
#define CLK_TSADC_DIV                            0x0B00002B
#define CLK_TSADC_TSEN_DIV                       0x050B002B

// TOPCRU_CLKSEL_CON44(Offset:0x1B0)
#define CLK_SARADC_TO_VCCIO156_DIV               0x0C00002C

// TOPCRU_CLKSEL_CON45(Offset:0x1B4)
#define CLK_GMAC_50M_CRU_I_SEL                   0x0107002D
#define CLK_GMAC_50M_CRU_I_SEL_CLK_MATRIX_50M_SRC 0U
#define CLK_GMAC_50M_CRU_I_SEL_XIN_OSC0_FUNC     1U
#define CLK_GMAC_125M_CRU_I_SEL                  0x0108002D
#define CLK_GMAC_125M_CRU_I_SEL_CLK_MATRIX_125M_SRC 0U
#define CLK_GMAC_125M_CRU_I_SEL_XIN_OSC0_FUNC    1U

// TOPCRU_CLKSEL_CON46(Offset:0x1B8)
#define CLK_GMAC_ETH_OUT2IO_DIV                  0x0700002E
#define CLK_GMAC_ETH_OUT2IO_SEL                  0x0107002E
#define CLK_GMAC_ETH_OUT2IO_SEL_CLK_GPLL_MUX     0U
#define CLK_GMAC_ETH_OUT2IO_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_TESTOUT_BUS_SEL                      0x030D002E
#define CLK_TESTOUT_BUS_SEL_ACLK_BUS             0U
#define CLK_TESTOUT_BUS_SEL_CLK_DSIPHY_TXBYTEHS  4U
#define CLK_TESTOUT_BUS_SEL_CLK_DSIPHY_RXESC     5U

// TOPCRU_CLKSEL_CON47(Offset:0x1BC)
#define CLK_MAC100_50M_MATRIX_SEL                0x0107002F
#define CLK_MAC100_50M_MATRIX_SEL_CLK_MATRIX_50M_SRC 0U
#define CLK_MAC100_50M_MATRIX_SEL_XIN_OSC0_FUNC  1U

// ======================= PMU0CRU module definition bank=1 =======================
// PMU0CRU_SOFTRST_CON00(Offset:0x200)
#define SRST_PRESETN_PMU0_CRU                    0x00001000
#define SRST_PRESETN_PMU0_PMU                    0x00001001
#define SRST_RESETN_PMU0_PMU                     0x00001002
#define SRST_PRESETN_PMU0_HP_TIMER               0x00001003
#define SRST_RESETN_PMU0_HP_TIMER                0x00001004
#define SRST_RESETN_PMU0_32K_HP_TIMER            0x00001005
#define SRST_PRESETN_PMU0_PVTM                   0x00001006
#define SRST_RESETN_PMU0_PVTM                    0x00001007
#define SRST_PRESETN_IOC_PMUIO                   0x00001008
#define SRST_PRESETN_PMU0_GPIO0                  0x00001009
#define SRST_DBRESETN_PMU0_GPIO0                 0x0000100A
#define SRST_PRESETN_PMU0_GRF                    0x0000100B
#define SRST_PRESETN_PMU0_SGRF                   0x0000100C

// PMU0CRU_SOFTRST_CON01(Offset:0x204)
#define SRST_RESETN_DDR_FAIL_SAFE                0x00001010
#define SRST_PRESETN_PMU0_SCRKEYGEN              0x00001011

// PMU0CRU_SOFTRST_CON02(Offset:0x208)
#define SRST_PRESETN_PMU0_I2C0                   0x00001028
#define SRST_RESETN_PMU0_I2C0                    0x00001029

// PMU0CRU_GATE_CON00(Offset:0x180)
#define PCLK_PMU0_CRU_GATE                       0x00001000
#define PCLK_PMU0_PMU_GATE                       0x00001001
#define CLK_PMU0_PMU_GATE                        0x00001002
#define PCLK_PMU0_HP_TIMER_GATE                  0x00001003
#define CLK_PMU0_HP_TIMER_GATE                   0x00001004
#define CLK_PMU0_32K_HP_TIMER_GATE               0x00001005
#define PCLK_PMU0_PVTM_GATE                      0x00001006
#define CLK_PMU0_PVTM_GATE                       0x00001007
#define PCLK_IOC_PMUIO_GATE                      0x00001008
#define PCLK_PMU0_GPIO0_GATE                     0x00001009
#define DBCLK_PMU0_GPIO0_GATE                    0x0000100A
#define PCLK_PMU0_GRF_GATE                       0x0000100B
#define PCLK_PMU0_SGRF_GATE                      0x0000100C
#define BUSCLK_PDPMU0_GATE                       0x0000100D
#define CLK_PDPMU_200M_DIV_GATE                  0x0000100E
#define XIN_OSC0_DIV_GATE                        0x0000100F

// PMU0CRU_GATE_CON01(Offset:0x184)
#define CLK_DDR_FAIL_SAFE_GATE                   0x00001010
#define PCLK_PMU0_SCRKEYGEN_GATE                 0x00001011

// PMU0CRU_GATE_CON02(Offset:0x188)
#define CLK_PIPEPHY_DIV_GATE                     0x00001020
#define XIN_OSC0_PIPEPHY_G_GATE                  0x00001021
#define CLK_PIPEPHY_REF_GATE                     0x00001022
#define XIN_OSC0_USB2PHY_G_GATE                  0x00001024
#define CLK_USB2PHY_REF_GATE                     0x00001025
#define XIN_OSC0_MIPIDSIPHY_G_GATE               0x00001026
#define CLK_MIPIDSIPHY_REF_GATE                  0x00001027
#define PCLK_PMU0_I2C0_GATE                      0x00001028
#define CLK_PMU0_I2C0_GATE                       0x00001029

// PMU0CRU_CLKSEL_CON00(Offset:0x100)
#define XIN_OSC0_DIV_DIV                         0x20000100

// PMU0CRU_CLKSEL_CON01(Offset:0x104)
#define CLK_PDPMU_200M_DIV_DIV                   0x02030101
#define CLK_DEEPSLOW_SEL                         0x02000101
#define CLK_DEEPSLOW_SEL_XIN_OSC0_DIV            0U
#define CLK_DEEPSLOW_SEL_XIN_OSC1_32K            1U
#define CLK_DEEPSLOW_SEL_CLK_PVTM_32K            2U

// PMU0CRU_CLKSEL_CON02(Offset:0x108)
#define CLK_PIPEPHY_DIV_DIV                      0x06000102
#define CLK_PIPEPHY_REF_SEL                      0x01070102
#define CLK_PIPEPHY_REF_SEL_CLK_PIPEPHY_DIV      0U
#define CLK_PIPEPHY_REF_SEL_XIN_OSC0_PIPEPHY_G   1U
#define CLK_USB2PHY_REF_SEL                      0x01080102
#define CLK_USB2PHY_REF_SEL_XIN_OSC0_USB2PHY_G   0U
#define CLK_USB2PHY_REF_SEL_CLK_24M_SSCSRC       1U
#define CLK_MIPIDSIPHY_REF_SEL                   0x010F0102
#define CLK_MIPIDSIPHY_REF_SEL_XIN_OSC0_MIPIDSIPHY_G 0U
#define CLK_MIPIDSIPHY_REF_SEL_CLK_24M_SSCSRC    1U

// PMU0CRU_CLKSEL_CON03(Offset:0x10C)
#define CLK_PMU0_I2C0_DIV                        0x05080103
#define CLK_PMU0_I2C0_SEL                        0x020E0103
#define CLK_PMU0_I2C0_SEL_CLK_200M_PMU           0U
#define CLK_PMU0_I2C0_SEL_XIN_OSC0_FUNC          1U
#define CLK_PMU0_I2C0_SEL_CLK_DEEPSLOW           2U

// ======================= PMU1CRU module definition bank=2 =======================
// PMU1CRU_SOFTRST_CON00(Offset:0x200)
#define SRST_PRESETN_PMU1_CRU                    0x00002000
#define SRST_HRESETN_PMU1_MEM                    0x00002002
#define SRST_HRESETN_PMU1_BIU                    0x00002003
#define SRST_PRESETN_PMU1_BIU                    0x00002004
#define SRST_PRESETN_PMU1_UART0                  0x00002007
#define SRST_SRESETN_PMU1_UART0                  0x0000200A

// PMU1CRU_SOFTRST_CON01(Offset:0x204)
#define SRST_PRESETN_PMU1_SPI0                   0x00002010
#define SRST_RESETN_PMU1_SPI0                    0x00002011
#define SRST_PRESETN_PMU1_PWM0                   0x00002013
#define SRST_RESETN_PMU1_PWM0                    0x00002014

// PMU1CRU_SOFTRST_CON02(Offset:0x208)
#define SRST_FRESETN_PMU1_CM0_CORE               0x00002020
#define SRST_TRESETN_PMU1_CM0_JTAG               0x00002022
#define SRST_PRESETN_PMU1_WDTNS                  0x00002023
#define SRST_RESETN_PMU1_WDTNS                   0x00002024
#define SRST_PRESETN_PMU1_MAILBOX                0x00002028

// PMU1CRU_GATE_CON00(Offset:0x180)
#define PCLK_PMU1_CRU_GATE                       0x00002000
#define CLK_200M_PMU_GATE                        0x00002001
#define HCLK_PMU1_MEM_GATE                       0x00002002
#define HCLK_PMU1_BIU_GATE                       0x00002003
#define PCLK_PMU1_BIU_GATE                       0x00002004
#define PCLK_PMU1_UART0_GATE                     0x00002007
#define CLK_PMU1_UART0_GATE                      0x00002008
#define CLK_PMU1_UART0_FRAC_GATE                 0x00002009
#define SCLK_PMU1_UART0_GATE                     0x0000200A
#define PCLK_PDPMU1_TO_PERI_GATE                 0x0000200F

// PMU1CRU_GATE_CON01(Offset:0x184)
#define PCLK_PMU1_SPI0_GATE                      0x00002010
#define CLK_PMU1_SPI0_GATE                       0x00002011
#define SCLK_IN_PMU1_SPI0_GATE                   0x00002012
#define PCLK_PMU1_PWM0_GATE                      0x00002013
#define CLK_PMU1_PWM0_GATE                       0x00002014
#define CLK_CAPTURE_PMU1_PWM0_GATE               0x00002015
#define CLK_PMU1_WFI_GATE                        0x00002016

// PMU1CRU_GATE_CON02(Offset:0x188)
#define FCLK_PMU1_CM0_CORE_GATE                  0x00002020
#define CLK_PMU1_CM0_RTC_GATE                    0x00002021
#define PCLK_PMU1_WDTNS_GATE                     0x00002023
#define CLK_PMU1_WDTNS_GATE                      0x00002024

// PMU1CRU_GATE_CON03(Offset:0x18C)
#define PCLK_PMU1_MAILBOX_GATE                   0x00002038
#define CLK_TESTOUT_PMU1_GATE                    0x0000203F

// PMU1CRU_CLKSEL_CON00(Offset:0x100)
#define CLK_200M_PMU_DIV                         0x05000200

// PMU1CRU_CLKSEL_CON02(Offset:0x108)
#define CLK_PMU1_UART0_SRC_DIV                   0x04000202
#define SCLK_PMU1_UART0_SEL                      0x02060202
#define SCLK_PMU1_UART0_SEL_CLK_PMU1_UART0_SRC   0U
#define SCLK_PMU1_UART0_SEL_CLK_PMU1_UART0_FRAC  1U
#define SCLK_PMU1_UART0_SEL_XIN_OSC0_FUNC        2U

// PMU1CRU_CLKSEL_CON03(Offset:0x10C)
#define CLK_PMU1_UART0_FRAC_DIV                  0x20000203

// PMU1CRU_CLKSEL_CON04(Offset:0x110)
#define CLK_PMU1_SPI0_DIV                        0x02000204
#define CLK_PMU1_PWM0_DIV                        0x02080204
#define CLK_PMU1_SPI0_SEL                        0x02060204
#define CLK_PMU1_SPI0_SEL_CLK_200M_PMU           0U
#define CLK_PMU1_SPI0_SEL_XIN_OSC0_FUNC          1U
#define CLK_PMU1_SPI0_SEL_CLK_DEEPSLOW           2U
#define CLK_PMU1_PWM0_SEL                        0x020E0204
#define CLK_PMU1_PWM0_SEL_CLK_200M_PMU           0U
#define CLK_PMU1_PWM0_SEL_XIN_OSC0_FUNC          1U
#define CLK_PMU1_PWM0_SEL_CLK_DEEPSLOW           2U

// PMU1CRU_CLKSEL_CON06(Offset:0x118)
#define CLK_TESTOUT_PMU1_DIV                     0x03080206
#define CLK_TESTOUT_PMU1_SEL                     0x030D0206
#define CLK_TESTOUT_PMU1_SEL_CLK_DEEPSLOW        0U
#define CLK_TESTOUT_PMU1_SEL_CLK_CPLL_MUX_FROM_PMU1 2U
#define CLK_TESTOUT_PMU1_SEL_BUSCLK_PDPMU0       4U

// ======================= DDRCRU module definition bank=3 ========================
// DDRCRU_SOFTRST_CON00(Offset:0x200)
#define SRST_RESETN_MSCH_BRG_BIU                 0x00003004
#define SRST_PRESETN_MSCH_BIU                    0x00003005
#define SRST_PRESETN_DDR_HWLP                    0x00003006
#define SRST_PRESETN_DDR_PHY                     0x00003008
#define SRST_PRESETN_DDR_DFICTL                  0x00003009
#define SRST_PRESETN_DDR_DMA2DDR                 0x0000300A

// DDRCRU_SOFTRST_CON01(Offset:0x204)
#define SRST_PRESETN_DDR_MON                     0x00003010
#define SRST_TMRESETN_DDR_MON                    0x00003011
#define SRST_PRESETN_DDR_GRF                     0x00003012
#define SRST_PRESETN_DDR_CRU                     0x00003013
#define SRST_PRESETN_SUBDDR_CRU                  0x00003014

// DDRCRU_GATE_CON00(Offset:0x180)
#define CLK_GPLL_MUX_TO_SUBDDR_GATE              0x00003000
#define XIN_OSC0_FUNC_TO_SUBDDR_GATE             0x00003001
#define CLK_DEEPSLOW_TO_SUBDDR_GATE              0x00003002
#define PCLK_DDR_GATE                            0x00003003
#define CLK_MSCH_BRG_BIU_GATE                    0x00003004
#define PCLK_MSCH_BIU_GATE                       0x00003005
#define PCLK_DDR_HWLP_GATE                       0x00003006
#define PCLK_DDR_PHY_GATE                        0x00003008
#define PCLK_DDR_DFICTL_GATE                     0x00003009
#define PCLK_DDR_DMA2DDR_GATE                    0x0000300A

// DDRCRU_GATE_CON01(Offset:0x184)
#define PCLK_DDR_MON_GATE                        0x00003010
#define TMCLK_DDR_MON_GATE                       0x00003011
#define PCLK_DDR_GRF_GATE                        0x00003012
#define PCLK_DDR_CRU_GATE                        0x00003013
#define PCLK_SUBDDR_CRU_GATE                     0x00003014

// DDRCRU_CLKSEL_CON00(Offset:0x100)

// DDRCRU_CLKSEL_CON01(Offset:0x104)
#define CLK_MSCH_BRG_BIU_DIV                     0x04000301
#define PCLK_DDR_DIV                             0x05080301

// ====================== SUBDDRCRU module definition bank=4 ======================
// SUBDDRCRU_SOFTRST_CON00(Offset:0x200)
#define SRST_RESETN_MSCH_BIU                     0x00004001
#define SRST_RESETN_DDR_PHY                      0x00004004
#define SRST_RESETN_DDR_DFICTL                   0x00004005
#define SRST_RESETN_DDR_SCRAMBLE                 0x00004006
#define SRST_RESETN_DDR_MON                      0x00004007
#define SRST_ARESETN_DDR_SPLIT                   0x00004008
#define SRST_ARESETN_DDR_DMA2DDR                 0x00004009

// SUBDDRCRU_GATE_CON00(Offset:0x180)
#define CLK_DDR_GATE                             0x00004000
#define CLK_MSCH_BIU_GATE                        0x00004001
#define CLK1X_DDR_PHY_GATE                       0x00004004
#define CLK_DDR_DFICTL_GATE                      0x00004005
#define CLK_DDR_SCRAMBLE_GATE                    0x00004006
#define CLK_DDR_MON_GATE                         0x00004007
#define ACLK_DDR_SPLIT_GATE                      0x00004008
#define ACLK_DDR_DMA2DDR_GATE                    0x00004009
#define CLK_TESTOUT_SUBDDR_GATE                  0x0000400F

// SUBDDRCRU_CLKSEL_CON00(Offset:0x100)
#define CLK_TESTOUT_SUBDDR_DIV                   0x02080400
#define CLK_DDR_SEL                              0x01070400
#define CLK_DDR_SEL_CLK_DPLL_MUX                 0U
#define CLK_DDR_SEL_CLK_GPLL_MUX_TO_SUBDDR       1U
#define CLK_TESTOUT_SUBDDR_SEL                   0x010F0400
#define CLK_TESTOUT_SUBDDR_SEL_CLK_DDR           0U
#define CLK_TESTOUT_SUBDDR_SEL_PCLK_DDR          1U

// ======================= PERICRU module definition bank=5 =======================
// PERICRU_SOFTRST_CON00(Offset:0x400)

// PERICRU_SOFTRST_CON01(Offset:0x404)
#define SRST_ARESETN_PERI_BIU                    0x00005013
#define SRST_HRESETN_PERI_BIU                    0x00005014
#define SRST_PRESETN_PERI_BIU                    0x00005015
#define SRST_PRESETN_PERICRU                     0x00005016

// PERICRU_SOFTRST_CON02(Offset:0x408)
#define SRST_HRESETN_SAI0                        0x00005020
#define SRST_MRESETN_SAI0                        0x00005023
#define SRST_HRESETN_SAI1                        0x00005025
#define SRST_MRESETN_SAI1                        0x00005028
#define SRST_HRESETN_SAI2                        0x0000502A
#define SRST_MRESETN_SAI2                        0x0000502D

// PERICRU_SOFTRST_CON03(Offset:0x40C)
#define SRST_HRESETN_DSM                         0x00005031
#define SRST_RESETN_DSM                          0x00005032
#define SRST_HRESETN_PDM                         0x00005034
#define SRST_MRESETN_PDM                         0x00005035
#define SRST_HRESETN_SPDIF                       0x00005038
#define SRST_MRESETN_SPDIF                       0x0000503B

// PERICRU_SOFTRST_CON04(Offset:0x410)
#define SRST_HRESETN_SDMMC0                      0x00005040
#define SRST_HRESETN_SDMMC1                      0x00005042
#define SRST_HRESETN_EMMC                        0x00005048
#define SRST_ARESETN_EMMC                        0x00005049
#define SRST_CRESETN_EMMC                        0x0000504A
#define SRST_BRESETN_EMMC                        0x0000504B
#define SRST_TRESETN_EMMC                        0x0000504C
#define SRST_SRESETN_SFC                         0x0000504D
#define SRST_HRESETN_SFC                         0x0000504E

// PERICRU_SOFTRST_CON05(Offset:0x414)
#define SRST_HRESETN_USB2HOST                    0x00005050
#define SRST_HRESETN_USB2HOST_ARB                0x00005051
#define SRST_RESETN_USB2HOST_UTMI                0x00005052

// PERICRU_SOFTRST_CON06(Offset:0x418)
#define SRST_PRESETN_SPI1                        0x00005060
#define SRST_RESETN_SPI1                         0x00005061
#define SRST_PRESETN_SPI2                        0x00005063
#define SRST_RESETN_SPI2                         0x00005064

// PERICRU_SOFTRST_CON07(Offset:0x41C)
#define SRST_PRESETN_UART1                       0x00005070
#define SRST_PRESETN_UART2                       0x00005071
#define SRST_PRESETN_UART3                       0x00005072
#define SRST_PRESETN_UART4                       0x00005073
#define SRST_PRESETN_UART5                       0x00005074
#define SRST_PRESETN_UART6                       0x00005075
#define SRST_PRESETN_UART7                       0x00005076
#define SRST_PRESETN_UART8                       0x00005077
#define SRST_PRESETN_UART9                       0x00005078
#define SRST_SRESETN_UART1                       0x0000507B
#define SRST_SRESETN_UART2                       0x0000507E

// PERICRU_SOFTRST_CON08(Offset:0x420)
#define SRST_SRESETN_UART3                       0x00005081
#define SRST_SRESETN_UART4                       0x00005084
#define SRST_SRESETN_UART5                       0x00005087
#define SRST_SRESETN_UART6                       0x0000508A
#define SRST_SRESETN_UART7                       0x0000508D

// PERICRU_SOFTRST_CON09(Offset:0x424)
#define SRST_SRESETN_UART8                       0x00005090
#define SRST_SRESETN_UART9                       0x00005093

// PERICRU_SOFTRST_CON10(Offset:0x428)
#define SRST_PRESETN_PWM1_PERI                   0x000050A0
#define SRST_RESETN_PWM1_PERI                    0x000050A1
#define SRST_PRESETN_PWM2_PERI                   0x000050A3
#define SRST_RESETN_PWM2_PERI                    0x000050A4
#define SRST_PRESETN_PWM3_PERI                   0x000050A6
#define SRST_RESETN_PWM3_PERI                    0x000050A7

// PERICRU_SOFTRST_CON12(Offset:0x430)
#define SRST_ARESETN_CRYPTO                      0x000050C0
#define SRST_HRESETN_CRYPTO                      0x000050C1
#define SRST_PRESETN_CRYPTO                      0x000050C2
#define SRST_RESETN_CORE_CRYPTO                  0x000050C3
#define SRST_RESETN_PKA_CRYPTO                   0x000050C4
#define SRST_HRESETN_KLAD                        0x000050C5
#define SRST_PRESETN_KEY_READER                  0x000050C6
#define SRST_HRESETN_RK_RNG_NS                   0x000050C7
#define SRST_HRESETN_RK_RNG_S                    0x000050C8
#define SRST_HRESETN_TRNG_NS                     0x000050C9
#define SRST_HRESETN_TRNG_S                      0x000050CA
#define SRST_HRESETN_CRYPTO_S                    0x000050CB

// PERICRU_SOFTRST_CON13(Offset:0x434)
#define SRST_PRESETN_PERI_WDT                    0x000050D0
#define SRST_TRESETN_PERI_WDT                    0x000050D1
#define SRST_ARESETN_SYSMEM                      0x000050D2
#define SRST_HRESETN_BOOTROM                     0x000050D3
#define SRST_PRESETN_PERI_GRF                    0x000050D4
#define SRST_ARESETN_DMAC                        0x000050D5
#define SRST_ARESETN_RKDMAC                      0x000050D6

// PERICRU_SOFTRST_CON14(Offset:0x438)
#define SRST_PRESETN_OTPC_NS                     0x000050E0
#define SRST_RESETN_SBPI_OTPC_NS                 0x000050E1
#define SRST_RESETN_USER_OTPC_NS                 0x000050E2
#define SRST_PRESETN_OTPC_S                      0x000050E3
#define SRST_RESETN_SBPI_OTPC_S                  0x000050E4
#define SRST_RESETN_USER_OTPC_S                  0x000050E5
#define SRST_RESETN_OTPC_ARB                     0x000050E6
#define SRST_PRESETN_OTPPHY                      0x000050E7
#define SRST_RESETN_OTP_NPOR                     0x000050E8

// PERICRU_SOFTRST_CON15(Offset:0x43C)
#define SRST_PRESETN_USB2PHY                     0x000050F0
#define SRST_RESETN_USB2PHY_POR                  0x000050F4
#define SRST_RESETN_USB2PHY_OTG                  0x000050F5
#define SRST_RESETN_USB2PHY_HOST                 0x000050F6
#define SRST_PRESETN_PIPEPHY                     0x000050F7

// PERICRU_SOFTRST_CON16(Offset:0x440)
#define SRST_PRESETN_SARADC                      0x00005104
#define SRST_RESETN_SARADC                       0x00005105
#define SRST_RESETN_SARADC_PHY                   0x00005106
#define SRST_PRESETN_IOC_VCCIO234                0x0000510C

// PERICRU_SOFTRST_CON17(Offset:0x444)
#define SRST_PRESETN_PERI_GPIO1                  0x00005110
#define SRST_PRESETN_PERI_GPIO2                  0x00005111
#define SRST_RESETN_PERI_GPIO1                   0x00005112
#define SRST_RESETN_PERI_GPIO2                   0x00005113

// PERICRU_GATE_CON00(Offset:0x300)

// PERICRU_GATE_CON01(Offset:0x304)
#define ACLK_PERI_GATE                           0x00005010
#define HCLK_PERI_GATE                           0x00005011
#define PCLK_PERI_GATE                           0x00005012
#define ACLK_PERI_BIU_GATE                       0x00005013
#define HCLK_PERI_BIU_GATE                       0x00005014
#define PCLK_PERI_BIU_GATE                       0x00005015
#define PCLK_PERICRU_GATE                        0x00005016

// PERICRU_GATE_CON02(Offset:0x308)
#define HCLK_SAI0_GATE                           0x00005020
#define CLK_SAI0_GATE                            0x00005021
#define CLK_SAI0_FRAC_GATE                       0x00005022
#define MCLK_SAI0_GATE                           0x00005023
#define MCLK_SAI0_OUT2IO_GATE                    0x00005024
#define HCLK_SAI1_GATE                           0x00005025
#define CLK_SAI1_GATE                            0x00005026
#define CLK_SAI1_FRAC_GATE                       0x00005027
#define MCLK_SAI1_GATE                           0x00005028
#define MCLK_SAI1_OUT2IO_GATE                    0x00005029
#define HCLK_SAI2_GATE                           0x0000502A
#define CLK_SAI2_GATE                            0x0000502B
#define CLK_SAI2_FRAC_GATE                       0x0000502C
#define MCLK_SAI2_GATE                           0x0000502D
#define MCLK_SAI2_OUT2IO_GATE                    0x0000502E

// PERICRU_GATE_CON03(Offset:0x30C)
#define HCLK_DSM_GATE                            0x00005031
#define CLK_DSM_GATE                             0x00005032
#define HCLK_PDM_GATE                            0x00005034
#define MCLK_PDM_GATE                            0x00005035
#define HCLK_SPDIF_GATE                          0x00005038
#define CLK_SPDIF_GATE                           0x00005039
#define CLK_SPDIF_FRAC_GATE                      0x0000503A
#define MCLK_SPDIF_GATE                          0x0000503B

// PERICRU_GATE_CON04(Offset:0x310)
#define HCLK_SDMMC0_GATE                         0x00005040
#define CCLK_SDMMC0_GATE                         0x00005041
#define HCLK_SDMMC1_GATE                         0x00005042
#define CCLK_SDMMC1_GATE                         0x00005043
#define HCLK_EMMC_GATE                           0x00005048
#define ACLK_EMMC_GATE                           0x00005049
#define CCLK_EMMC_GATE                           0x0000504A
#define BCLK_EMMC_GATE                           0x0000504B
#define TMCLK_EMMC_GATE                          0x0000504C
#define SCLK_SFC_GATE                            0x0000504D
#define HCLK_SFC_GATE                            0x0000504E

// PERICRU_GATE_CON05(Offset:0x314)
#define HCLK_USB2HOST_GATE                       0x00005050
#define HCLK_USB2HOST_ARB_GATE                   0x00005051

// PERICRU_GATE_CON06(Offset:0x318)
#define PCLK_SPI1_GATE                           0x00005060
#define CLK_SPI1_GATE                            0x00005061
#define SCLK_IN_SPI1_GATE                        0x00005062
#define PCLK_SPI2_GATE                           0x00005063
#define CLK_SPI2_GATE                            0x00005064
#define SCLK_IN_SPI2_GATE                        0x00005065

// PERICRU_GATE_CON07(Offset:0x31C)
#define PCLK_UART1_GATE                          0x00005070
#define PCLK_UART2_GATE                          0x00005071
#define PCLK_UART3_GATE                          0x00005072
#define PCLK_UART4_GATE                          0x00005073
#define PCLK_UART5_GATE                          0x00005074
#define PCLK_UART6_GATE                          0x00005075
#define PCLK_UART7_GATE                          0x00005076
#define PCLK_UART8_GATE                          0x00005077
#define PCLK_UART9_GATE                          0x00005078
#define CLK_UART1_GATE                           0x00005079
#define CLK_UART1_FRAC_GATE                      0x0000507A
#define SCLK_UART1_GATE                          0x0000507B
#define CLK_UART2_GATE                           0x0000507C
#define CLK_UART2_FRAC_GATE                      0x0000507D
#define SCLK_UART2_GATE                          0x0000507E
#define CLK_UART3_GATE                           0x0000507F

// PERICRU_GATE_CON08(Offset:0x320)
#define CLK_UART3_FRAC_GATE                      0x00005080
#define SCLK_UART3_GATE                          0x00005081
#define CLK_UART4_GATE                           0x00005082
#define CLK_UART4_FRAC_GATE                      0x00005083
#define SCLK_UART4_GATE                          0x00005084
#define CLK_UART5_GATE                           0x00005085
#define CLK_UART5_FRAC_GATE                      0x00005086
#define SCLK_UART5_GATE                          0x00005087
#define CLK_UART6_GATE                           0x00005088
#define CLK_UART6_FRAC_GATE                      0x00005089
#define SCLK_UART6_GATE                          0x0000508A
#define CLK_UART7_GATE                           0x0000508B
#define CLK_UART7_FRAC_GATE                      0x0000508C
#define SCLK_UART7_GATE                          0x0000508D
#define CLK_UART8_GATE                           0x0000508E
#define CLK_UART8_FRAC_GATE                      0x0000508F

// PERICRU_GATE_CON09(Offset:0x324)
#define SCLK_UART8_GATE                          0x00005090
#define CLK_UART9_GATE                           0x00005091
#define CLK_UART9_FRAC_GATE                      0x00005092
#define SCLK_UART9_GATE                          0x00005093

// PERICRU_GATE_CON10(Offset:0x328)
#define PCLK_PWM1_PERI_GATE                      0x000050A0
#define CLK_PWM1_PERI_GATE                       0x000050A1
#define CLK_CAPTURE_PWM1_PERI_GATE               0x000050A2
#define PCLK_PWM2_PERI_GATE                      0x000050A3
#define CLK_PWM2_PERI_GATE                       0x000050A4
#define CLK_CAPTURE_PWM2_PERI_GATE               0x000050A5
#define PCLK_PWM3_PERI_GATE                      0x000050A6
#define CLK_PWM3_PERI_GATE                       0x000050A7
#define CLK_CAPTURE_PWM3_PERI_GATE               0x000050A8

// PERICRU_GATE_CON12(Offset:0x330)
#define ACLK_CRYPTO_GATE                         0x000050C0
#define HCLK_CRYPTO_GATE                         0x000050C1
#define PCLK_CRYPTO_GATE                         0x000050C2
#define CLK_CORE_CRYPTO_GATE                     0x000050C3
#define CLK_PKA_CRYPTO_GATE                      0x000050C4
#define HCLK_KLAD_GATE                           0x000050C5
#define PCLK_KEY_READER_GATE                     0x000050C6
#define HCLK_RK_RNG_NS_GATE                      0x000050C7
#define HCLK_RK_RNG_S_GATE                       0x000050C8
#define HCLK_TRNG_NS_GATE                        0x000050C9
#define HCLK_TRNG_S_GATE                         0x000050CA
#define HCLK_CRYPTO_S_GATE                       0x000050CB

// PERICRU_GATE_CON13(Offset:0x334)
#define PCLK_PERI_WDT_GATE                       0x000050D0
#define TCLK_PERI_WDT_GATE                       0x000050D1
#define ACLK_SYSMEM_GATE                         0x000050D2
#define HCLK_BOOTROM_GATE                        0x000050D3
#define PCLK_PERI_GRF_GATE                       0x000050D4
#define ACLK_DMAC_GATE                           0x000050D5
#define ACLK_RKDMAC_GATE                         0x000050D6

// PERICRU_GATE_CON14(Offset:0x338)
#define PCLK_OTPC_NS_GATE                        0x000050E0
#define CLK_SBPI_OTPC_NS_GATE                    0x000050E1
#define CLK_USER_OTPC_NS_GATE                    0x000050E2
#define PCLK_OTPC_S_GATE                         0x000050E3
#define CLK_SBPI_OTPC_S_GATE                     0x000050E4
#define CLK_USER_OTPC_S_GATE                     0x000050E5
#define CLK_OTPC_ARB_GATE                        0x000050E6
#define PCLK_OTPPHY_GATE                         0x000050E7

// PERICRU_GATE_CON15(Offset:0x33C)
#define PCLK_USB2PHY_GATE                        0x000050F0
#define PCLK_PIPEPHY_GATE                        0x000050F7

// PERICRU_GATE_CON16(Offset:0x340)
#define PCLK_SARADC_GATE                         0x00005104
#define CLK_SARADC_GATE                          0x00005105
#define PCLK_IOC_VCCIO234_GATE                   0x0000510C

// PERICRU_GATE_CON17(Offset:0x344)
#define PCLK_PERI_GPIO1_GATE                     0x00005110
#define PCLK_PERI_GPIO2_GATE                     0x00005111
#define DCLK_PERI_GPIO1_GATE                     0x00005112
#define DCLK_PERI_GPIO2_GATE                     0x00005113
#define DCLK_PERI_GPIO_GATE                      0x00005114
#define CLK_TESTOUT_PERI_GATE                    0x0000511F

// PERICRU_CLKSEL_CON00(Offset:0x100)
#define ACLK_PERI_DIV                            0x05000500
#define HCLK_PERI_DIV                            0x06080500
#define ACLK_PERI_SEL                            0x01070500
#define ACLK_PERI_SEL_CLK_GPLL_MUX_TO_PERI       0U
#define ACLK_PERI_SEL_CLK_CPLL_MUX_FROM_PMU1     1U
#define HCLK_PERI_SEL                            0x010F0500
#define HCLK_PERI_SEL_CLK_GPLL_MUX_TO_PERI       0U
#define HCLK_PERI_SEL_CLK_CPLL_MUX_FROM_PMU1     1U

// PERICRU_CLKSEL_CON01(Offset:0x104)
#define PCLK_PERI_DIV                            0x05000501
#define CLK_SAI0_SRC_DIV                         0x06080501
#define PCLK_PERI_SEL                            0x01070501
#define PCLK_PERI_SEL_CLK_GPLL_MUX_TO_PERI       0U
#define PCLK_PERI_SEL_CLK_CPLL_MUX_FROM_PMU1     1U
#define CLK_SAI0_SRC_SEL                         0x020E0501
#define CLK_SAI0_SRC_SEL_CLK_GPLL_MUX_TO_PERI    0U
#define CLK_SAI0_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1  1U
#define CLK_SAI0_SRC_SEL_CLK_HPLL_MUX_TO_PERI    2U

// PERICRU_CLKSEL_CON02(Offset:0x108)
#define CLK_SAI0_FRAC_DIV                        0x20000502

// PERICRU_CLKSEL_CON03(Offset:0x10C)
#define CLK_SAI1_SRC_DIV                         0x06080503
#define MCLK_SAI0_OUT2IO_SEL                     0x01050503
#define MCLK_SAI0_OUT2IO_SEL_MCLK_SAI0           0U
#define MCLK_SAI0_OUT2IO_SEL_XIN_OSC0_HALF       1U
#define MCLK_SAI0_SEL                            0x02060503
#define MCLK_SAI0_SEL_CLK_SAI0_SRC               0U
#define MCLK_SAI0_SEL_CLK_SAI0_FRAC              1U
#define MCLK_SAI0_SEL_XIN_OSC0_HALF              2U
#define MCLK_SAI0_SEL_MCLK_SAI0_FROM_IO          3U
#define CLK_SAI1_SRC_SEL                         0x020E0503
#define CLK_SAI1_SRC_SEL_CLK_GPLL_MUX_TO_PERI    0U
#define CLK_SAI1_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1  1U
#define CLK_SAI1_SRC_SEL_CLK_HPLL_MUX_TO_PERI    2U

// PERICRU_CLKSEL_CON04(Offset:0x110)
#define CLK_SAI1_FRAC_DIV                        0x20000504

// PERICRU_CLKSEL_CON05(Offset:0x114)
#define MCLK_SAI1_OUT2IO_SEL                     0x01050505
#define MCLK_SAI1_OUT2IO_SEL_MCLK_SAI1           0U
#define MCLK_SAI1_OUT2IO_SEL_XIN_OSC0_HALF       1U
#define MCLK_SAI1_SEL                            0x02060505
#define MCLK_SAI1_SEL_CLK_SAI1_SRC               0U
#define MCLK_SAI1_SEL_CLK_SAI1_FRAC              1U
#define MCLK_SAI1_SEL_XIN_OSC0_HALF              2U
#define MCLK_SAI1_SEL_MCLK_SAI1_FROM_IO          3U

// PERICRU_CLKSEL_CON06(Offset:0x118)
#define CLK_SAI2_SRC_DIV                         0x06080506
#define CLK_SAI2_SRC_SEL                         0x020E0506
#define CLK_SAI2_SRC_SEL_CLK_GPLL_MUX_TO_PERI    0U
#define CLK_SAI2_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1  1U
#define CLK_SAI2_SRC_SEL_CLK_HPLL_MUX_TO_PERI    2U

// PERICRU_CLKSEL_CON07(Offset:0x11C)
#define CLK_SAI2_FRAC_DIV                        0x20000507

// PERICRU_CLKSEL_CON08(Offset:0x120)
#define MCLK_SAI2_OUT2IO_SEL                     0x01050508
#define MCLK_SAI2_OUT2IO_SEL_MCLK_SAI2           0U
#define MCLK_SAI2_OUT2IO_SEL_XIN_OSC0_HALF       1U
#define MCLK_SAI2_SEL                            0x02060508
#define MCLK_SAI2_SEL_CLK_SAI2_SRC               0U
#define MCLK_SAI2_SEL_CLK_SAI2_FRAC              1U
#define MCLK_SAI2_SEL_XIN_OSC0_HALF              2U
#define MCLK_SAI2_SEL_MCLK_SAI2_FROM_IO          3U

// PERICRU_CLKSEL_CON12(Offset:0x130)
#define MCLK_PDM_DIV                             0x0500050C
#define MCLK_PDM_SEL                             0x0206050C
#define MCLK_PDM_SEL_CLK_GPLL_MUX_TO_PERI        0U
#define MCLK_PDM_SEL_CLK_CPLL_MUX_FROM_PMU1      1U
#define MCLK_PDM_SEL_CLK_HPLL_MUX_TO_PERI        2U
#define MCLK_PDM_SEL_XIN_OSC0_FUNC               3U

// PERICRU_CLKSEL_CON13(Offset:0x134)
#define CLK_SPDIF_SRC_DIV                        0x0608050D
#define CLK_SPDIF_SRC_SEL                        0x020E050D
#define CLK_SPDIF_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_SPDIF_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define CLK_SPDIF_SRC_SEL_CLK_HPLL_MUX_TO_PERI   2U

// PERICRU_CLKSEL_CON14(Offset:0x138)
#define CLK_SPDIF_FRAC_DIV                       0x2000050E

// PERICRU_CLKSEL_CON15(Offset:0x13C)
#define MCLK_SPDIF_SEL                           0x0206050F
#define MCLK_SPDIF_SEL_CLK_SPDIF_SRC             0U
#define MCLK_SPDIF_SEL_CLK_SPDIF_FRAC            1U
#define MCLK_SPDIF_SEL_XIN_OSC0_HALF             2U

// PERICRU_CLKSEL_CON16(Offset:0x140)
#define CCLK_SDMMC0_DIV                          0x08000510
#define CCLK_SDMMC0_SEL                          0x020E0510
#define CCLK_SDMMC0_SEL_CLK_GPLL_MUX_TO_PERI     0U
#define CCLK_SDMMC0_SEL_CLK_CPLL_MUX_FROM_PMU1   1U
#define CCLK_SDMMC0_SEL_XIN_OSC0_FUNC            2U
#define CCLK_SDMMC0_SEL_CLK_HPLL_MUX_TO_PERI     3U

// PERICRU_CLKSEL_CON17(Offset:0x144)
#define CCLK_SDMMC1_DIV                          0x08000511
#define CCLK_SDMMC1_SEL                          0x020E0511
#define CCLK_SDMMC1_SEL_CLK_GPLL_MUX_TO_PERI     0U
#define CCLK_SDMMC1_SEL_CLK_CPLL_MUX_FROM_PMU1   1U
#define CCLK_SDMMC1_SEL_XIN_OSC0_FUNC            2U
#define CCLK_SDMMC1_SEL_CLK_HPLL_MUX_TO_PERI     3U

// PERICRU_CLKSEL_CON18(Offset:0x148)
#define CCLK_EMMC_DIV                            0x08000512
#define CCLK_EMMC_SEL                            0x020E0512
#define CCLK_EMMC_SEL_CLK_GPLL_MUX_TO_PERI       0U
#define CCLK_EMMC_SEL_CLK_CPLL_MUX_FROM_PMU1     1U
#define CCLK_EMMC_SEL_XIN_OSC0_FUNC              2U
#define CCLK_EMMC_SEL_CLK_HPLL_MUX_TO_PERI       3U

// PERICRU_CLKSEL_CON19(Offset:0x14C)
#define BCLK_EMMC_DIV                            0x07080513
#define BCLK_EMMC_SEL                            0x010F0513
#define BCLK_EMMC_SEL_CLK_GPLL_MUX_TO_PERI       0U
#define BCLK_EMMC_SEL_CLK_CPLL_MUX_FROM_PMU1     1U

// PERICRU_CLKSEL_CON20(Offset:0x150)
#define SCLK_SFC_DIV                             0x08000514
#define SCLK_SFC_SEL                             0x02080514
#define SCLK_SFC_SEL_CLK_GPLL_MUX_TO_PERI        0U
#define SCLK_SFC_SEL_CLK_CPLL_MUX_FROM_PMU1      1U
#define SCLK_SFC_SEL_XIN_OSC0_FUNC               2U
#define CLK_SPI1_SEL                             0x020C0514
#define CLK_SPI1_SEL_CLK_MATRIX_200M_SRC         0U
#define CLK_SPI1_SEL_CLK_MATRIX_100M_SRC         1U
#define CLK_SPI1_SEL_CLK_MATRIX_50M_SRC          2U
#define CLK_SPI1_SEL_XIN_OSC0_FUNC               3U
#define CLK_SPI2_SEL                             0x020E0514
#define CLK_SPI2_SEL_CLK_MATRIX_200M_SRC         0U
#define CLK_SPI2_SEL_CLK_MATRIX_100M_SRC         1U
#define CLK_SPI2_SEL_CLK_MATRIX_50M_SRC          2U
#define CLK_SPI2_SEL_XIN_OSC0_FUNC               3U

// PERICRU_CLKSEL_CON21(Offset:0x154)
#define CLK_UART1_SRC_DIV                        0x07000515
#define CLK_UART1_SRC_SEL                        0x01080515
#define CLK_UART1_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART1_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART1_SEL                           0x020E0515
#define SCLK_UART1_SEL_CLK_UART1_SRC             0U
#define SCLK_UART1_SEL_CLK_UART1_FRAC            1U
#define SCLK_UART1_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON22(Offset:0x158)
#define CLK_UART1_FRAC_DIV                       0x20000516

// PERICRU_CLKSEL_CON23(Offset:0x15C)
#define CLK_UART2_SRC_DIV                        0x07000517
#define CLK_UART2_SRC_SEL                        0x01080517
#define CLK_UART2_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART2_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART2_SEL                           0x020E0517
#define SCLK_UART2_SEL_CLK_UART2_SRC             0U
#define SCLK_UART2_SEL_CLK_UART2_FRAC            1U
#define SCLK_UART2_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON24(Offset:0x160)
#define CLK_UART2_FRAC_DIV                       0x20000518

// PERICRU_CLKSEL_CON25(Offset:0x164)
#define CLK_UART3_SRC_DIV                        0x07000519
#define CLK_UART3_SRC_SEL                        0x01080519
#define CLK_UART3_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART3_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART3_SEL                           0x020E0519
#define SCLK_UART3_SEL_CLK_UART3_SRC             0U
#define SCLK_UART3_SEL_CLK_UART3_FRAC            1U
#define SCLK_UART3_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON26(Offset:0x168)
#define CLK_UART3_FRAC_DIV                       0x2000051A

// PERICRU_CLKSEL_CON27(Offset:0x16C)
#define CLK_UART4_SRC_DIV                        0x0700051B
#define CLK_UART4_SRC_SEL                        0x0108051B
#define CLK_UART4_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART4_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART4_SEL                           0x020E051B
#define SCLK_UART4_SEL_CLK_UART4_SRC             0U
#define SCLK_UART4_SEL_CLK_UART4_FRAC            1U
#define SCLK_UART4_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON28(Offset:0x170)
#define CLK_UART4_FRAC_DIV                       0x2000051C

// PERICRU_CLKSEL_CON29(Offset:0x174)
#define CLK_UART5_SRC_DIV                        0x0700051D
#define CLK_UART5_SRC_SEL                        0x0108051D
#define CLK_UART5_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART5_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART5_SEL                           0x020E051D
#define SCLK_UART5_SEL_CLK_UART5_SRC             0U
#define SCLK_UART5_SEL_CLK_UART5_FRAC            1U
#define SCLK_UART5_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON30(Offset:0x178)
#define CLK_UART5_FRAC_DIV                       0x2000051E

// PERICRU_CLKSEL_CON31(Offset:0x17C)
#define CLK_UART6_SRC_DIV                        0x0700051F
#define CLK_UART6_SRC_SEL                        0x0108051F
#define CLK_UART6_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART6_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART6_SEL                           0x020E051F
#define SCLK_UART6_SEL_CLK_UART6_SRC             0U
#define SCLK_UART6_SEL_CLK_UART6_FRAC            1U
#define SCLK_UART6_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON32(Offset:0x180)
#define CLK_UART6_FRAC_DIV                       0x20000520

// PERICRU_CLKSEL_CON33(Offset:0x184)
#define CLK_UART7_SRC_DIV                        0x07000521
#define CLK_UART7_SRC_SEL                        0x01080521
#define CLK_UART7_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART7_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART7_SEL                           0x020E0521
#define SCLK_UART7_SEL_CLK_UART7_SRC             0U
#define SCLK_UART7_SEL_CLK_UART7_FRAC            1U
#define SCLK_UART7_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON34(Offset:0x188)
#define CLK_UART7_FRAC_DIV                       0x20000522

// PERICRU_CLKSEL_CON35(Offset:0x18C)
#define CLK_UART8_SRC_DIV                        0x07000523
#define CLK_UART8_SRC_SEL                        0x01080523
#define CLK_UART8_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART8_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART8_SEL                           0x020E0523
#define SCLK_UART8_SEL_CLK_UART8_SRC             0U
#define SCLK_UART8_SEL_CLK_UART8_FRAC            1U
#define SCLK_UART8_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON36(Offset:0x190)
#define CLK_UART8_FRAC_DIV                       0x20000524

// PERICRU_CLKSEL_CON37(Offset:0x194)
#define CLK_UART9_SRC_DIV                        0x07000525
#define CLK_UART9_SRC_SEL                        0x01080525
#define CLK_UART9_SRC_SEL_CLK_GPLL_MUX_TO_PERI   0U
#define CLK_UART9_SRC_SEL_CLK_CPLL_MUX_FROM_PMU1 1U
#define SCLK_UART9_SEL                           0x020E0525
#define SCLK_UART9_SEL_CLK_UART9_SRC             0U
#define SCLK_UART9_SEL_CLK_UART9_FRAC            1U
#define SCLK_UART9_SEL_XIN_OSC0_FUNC             2U

// PERICRU_CLKSEL_CON38(Offset:0x198)
#define CLK_UART9_FRAC_DIV                       0x20000526

// PERICRU_CLKSEL_CON40(Offset:0x1A0)
#define CLK_PWM1_PERI_SEL                        0x02000528
#define CLK_PWM1_PERI_SEL_CLK_MATRIX_100M_SRC    0U
#define CLK_PWM1_PERI_SEL_CLK_MATRIX_50M_SRC     1U
#define CLK_PWM1_PERI_SEL_XIN_OSC0_FUNC          2U
#define CLK_PWM2_PERI_SEL                        0x02060528
#define CLK_PWM2_PERI_SEL_CLK_MATRIX_100M_SRC    0U
#define CLK_PWM2_PERI_SEL_CLK_MATRIX_50M_SRC     1U
#define CLK_PWM2_PERI_SEL_XIN_OSC0_FUNC          2U
#define CLK_PWM3_PERI_SEL                        0x02080528
#define CLK_PWM3_PERI_SEL_CLK_MATRIX_100M_SRC    0U
#define CLK_PWM3_PERI_SEL_CLK_MATRIX_50M_SRC     1U
#define CLK_PWM3_PERI_SEL_XIN_OSC0_FUNC          2U

// PERICRU_CLKSEL_CON43(Offset:0x1AC)
#define CLK_CORE_CRYPTO_SEL                      0x0200052B
#define CLK_CORE_CRYPTO_SEL_CLK_MATRIX_200M_SRC  0U
#define CLK_CORE_CRYPTO_SEL_CLK_MATRIX_100M_SRC  1U
#define CLK_CORE_CRYPTO_SEL_XIN_OSC0_FUNC        2U
#define CLK_PKA_CRYPTO_SEL                       0x0206052B
#define CLK_PKA_CRYPTO_SEL_CLK_MATRIX_300M_SRC   0U
#define CLK_PKA_CRYPTO_SEL_CLK_MATRIX_200M_SRC   1U
#define CLK_PKA_CRYPTO_SEL_CLK_MATRIX_100M_SRC   2U
#define CLK_PKA_CRYPTO_SEL_XIN_OSC0_FUNC         3U
#define TCLK_PERI_WDT_SEL                        0x010F052B
#define TCLK_PERI_WDT_SEL_XIN_OSC0_FUNC          0U
#define TCLK_PERI_WDT_SEL_CLK_DEEPSLOW           1U

// PERICRU_CLKSEL_CON44(Offset:0x1B0)
#define CLK_USER_OTPC_NS_DIV                     0x0800052C
#define CLK_USER_OTPC_S_DIV                      0x0808052C

// PERICRU_CLKSEL_CON46(Offset:0x1B8)
#define CLK_SARADC_DIV                           0x0C00052E

// PERICRU_CLKSEL_CON47(Offset:0x1BC)
#define DCLK_PERI_GPIO_SEL                       0x0108052F
#define DCLK_PERI_GPIO_SEL_XIN_OSC0_FUNC         0U
#define DCLK_PERI_GPIO_SEL_CLK_DEEPSLOW          1U
#define CLK_TESTOUT_PERI_SEL                     0x050B052F

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK3562_H */

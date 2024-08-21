/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#ifndef __RK1808_H
#define __RK1808_H
#ifdef __cplusplus
  extern "C" {
#endif

/****************************************************************************************/
/*                                                                                      */
/*                               Module Structure Section                               */
/*                                                                                      */
/****************************************************************************************/
#ifndef __ASSEMBLY__
/* BUS_GRF Register Structure Define */
struct BUS_GRF_REG {
    __IO uint32_t GPIO1A_IOMUX_L;                     /* Address Offset: 0x0000 */
    __IO uint32_t GPIO1A_IOMUX_H;                     /* Address Offset: 0x0004 */
    __IO uint32_t GPIO1B_IOMUX_L;                     /* Address Offset: 0x0008 */
    __IO uint32_t GPIO1B_IOMUX_H;                     /* Address Offset: 0x000C */
         uint32_t RESERVED0010[4];                    /* Address Offset: 0x0010 */
    __IO uint32_t GPIO2A_IOMUX_L;                     /* Address Offset: 0x0020 */
    __IO uint32_t GPIO2A_IOMUX_H;                     /* Address Offset: 0x0024 */
    __IO uint32_t GPIO2B_IOMUX_L;                     /* Address Offset: 0x0028 */
    __IO uint32_t GPIO2B_IOMUX_H;                     /* Address Offset: 0x002C */
    __IO uint32_t GPIO2C_IOMUX_L;                     /* Address Offset: 0x0030 */
    __IO uint32_t GPIO2C_IOMUX_H;                     /* Address Offset: 0x0034 */
    __IO uint32_t GPIO2D_IOMUX_L;                     /* Address Offset: 0x0038 */
         uint32_t RESERVED003C;                       /* Address Offset: 0x003C */
    __IO uint32_t GPIO3A_IOMUX_L;                     /* Address Offset: 0x0040 */
    __IO uint32_t GPIO3A_IOMUX_H;                     /* Address Offset: 0x0044 */
    __IO uint32_t GPIO3B_IOMUX_L;                     /* Address Offset: 0x0048 */
    __IO uint32_t GPIO3B_IOMUX_H;                     /* Address Offset: 0x004C */
    __IO uint32_t GPIO3C_IOMUX_L;                     /* Address Offset: 0x0050 */
    __IO uint32_t GPIO3C_IOMUX_H;                     /* Address Offset: 0x0054 */
    __IO uint32_t GPIO3D_IOMUX_L;                     /* Address Offset: 0x0058 */
         uint32_t RESERVED005C;                       /* Address Offset: 0x005C */
    __IO uint32_t GPIO4A_IOMUX_L;                     /* Address Offset: 0x0060 */
    __IO uint32_t GPIO4A_IOMUX_H;                     /* Address Offset: 0x0064 */
    __IO uint32_t GPIO4B_IOMUX_L;                     /* Address Offset: 0x0068 */
    __IO uint32_t GPIO4B_IOMUX_H;                     /* Address Offset: 0x006C */
    __IO uint32_t GPIO4C_IOMUX_L;                     /* Address Offset: 0x0070 */
    __IO uint32_t GPIO4C_IOMUX_H;                     /* Address Offset: 0x0074 */
         uint32_t RESERVED0078[2];                    /* Address Offset: 0x0078 */
    __IO uint32_t GPIO1A_P;                           /* Address Offset: 0x0080 */
    __IO uint32_t GPIO1B_P;                           /* Address Offset: 0x0084 */
         uint32_t RESERVED0088[2];                    /* Address Offset: 0x0088 */
    __IO uint32_t GPIO2A_P;                           /* Address Offset: 0x0090 */
    __IO uint32_t GPIO2B_P;                           /* Address Offset: 0x0094 */
    __IO uint32_t GPIO2C_P;                           /* Address Offset: 0x0098 */
    __IO uint32_t GPIO2D_P;                           /* Address Offset: 0x009C */
    __IO uint32_t GPIO3A_P;                           /* Address Offset: 0x00A0 */
    __IO uint32_t GPIO3B_P;                           /* Address Offset: 0x00A4 */
    __IO uint32_t GPIO3C_P;                           /* Address Offset: 0x00A8 */
    __IO uint32_t GPIO3D_P;                           /* Address Offset: 0x00AC */
    __IO uint32_t GPIO4A_P;                           /* Address Offset: 0x00B0 */
    __IO uint32_t GPIO4B_P;                           /* Address Offset: 0x00B4 */
    __IO uint32_t GPIO4C_P;                           /* Address Offset: 0x00B8 */
         uint32_t RESERVED00BC;                       /* Address Offset: 0x00BC */
    __IO uint32_t GPIO1A_SR;                          /* Address Offset: 0x00C0 */
    __IO uint32_t GPIO1B_SR;                          /* Address Offset: 0x00C4 */
         uint32_t RESERVED00C8[2];                    /* Address Offset: 0x00C8 */
    __IO uint32_t GPIO2A_SR;                          /* Address Offset: 0x00D0 */
    __IO uint32_t GPIO2B_SR;                          /* Address Offset: 0x00D4 */
    __IO uint32_t GPIO2C_SR;                          /* Address Offset: 0x00D8 */
    __IO uint32_t GPIO2D_SR;                          /* Address Offset: 0x00DC */
    __IO uint32_t GPIO3A_SR;                          /* Address Offset: 0x00E0 */
    __IO uint32_t GPIO3B_SR;                          /* Address Offset: 0x00E4 */
    __IO uint32_t GPIO3C_SR;                          /* Address Offset: 0x00E8 */
    __IO uint32_t GPIO3D_SR;                          /* Address Offset: 0x00EC */
    __IO uint32_t GPIO4A_SR;                          /* Address Offset: 0x00F0 */
    __IO uint32_t GPIO4B_SR;                          /* Address Offset: 0x00F4 */
    __IO uint32_t GPIO4C_SR;                          /* Address Offset: 0x00F8 */
         uint32_t RESERVED00FC;                       /* Address Offset: 0x00FC */
    __IO uint32_t GPIO1A_SMT;                         /* Address Offset: 0x0100 */
    __IO uint32_t GPIO1B_SMT;                         /* Address Offset: 0x0104 */
    __IO uint32_t GPIO1C_SMT;                         /* Address Offset: 0x0108 */
    __IO uint32_t GPIO1D_SMT;                         /* Address Offset: 0x010C */
    __IO uint32_t GPIO2A_SMT;                         /* Address Offset: 0x0110 */
    __IO uint32_t GPIO2B_SMT;                         /* Address Offset: 0x0114 */
    __IO uint32_t GPIO2C_SMT;                         /* Address Offset: 0x0118 */
    __IO uint32_t GPIO2D_SMT;                         /* Address Offset: 0x011C */
    __IO uint32_t GPIO3A_SMT;                         /* Address Offset: 0x0120 */
    __IO uint32_t GPIO3B_SMT;                         /* Address Offset: 0x0124 */
    __IO uint32_t GPIO3C_SMT;                         /* Address Offset: 0x0128 */
    __IO uint32_t GPIO3D_SMT;                         /* Address Offset: 0x012C */
    __IO uint32_t GPIO4A_SMT;                         /* Address Offset: 0x0130 */
    __IO uint32_t GPIO4B_SMT;                         /* Address Offset: 0x0134 */
    __IO uint32_t GPIO4C_SMT;                         /* Address Offset: 0x0138 */
         uint32_t RESERVED013C;                       /* Address Offset: 0x013C */
    __IO uint32_t GPIO1A_E;                           /* Address Offset: 0x0140 */
    __IO uint32_t GPIO1B_E;                           /* Address Offset: 0x0144 */
         uint32_t RESERVED0148[2];                    /* Address Offset: 0x0148 */
    __IO uint32_t GPIO2A_E;                           /* Address Offset: 0x0150 */
    __IO uint32_t GPIO2B_E;                           /* Address Offset: 0x0154 */
    __IO uint32_t GPIO2C_E;                           /* Address Offset: 0x0158 */
    __IO uint32_t GPIO2D_E;                           /* Address Offset: 0x015C */
    __IO uint32_t GPIO3A_E;                           /* Address Offset: 0x0160 */
    __IO uint32_t GPIO3B_E;                           /* Address Offset: 0x0164 */
    __IO uint32_t GPIO3C_E;                           /* Address Offset: 0x0168 */
    __IO uint32_t GPIO3D_E;                           /* Address Offset: 0x016C */
    __IO uint32_t GPIO4A_E;                           /* Address Offset: 0x0170 */
    __IO uint32_t GPIO4B_E;                           /* Address Offset: 0x0174 */
    __IO uint32_t GPIO4C_E;                           /* Address Offset: 0x0178 */
         uint32_t RESERVED017C[5];                    /* Address Offset: 0x017C */
    __IO uint32_t IOFUNC_CON0;                        /* Address Offset: 0x0190 */
         uint32_t RESERVED0194[155];                  /* Address Offset: 0x0194 */
    __IO uint32_t SOC_CON0;                           /* Address Offset: 0x0400 */
    __IO uint32_t SOC_CON1;                           /* Address Offset: 0x0404 */
    __IO uint32_t SOC_CON2;                           /* Address Offset: 0x0408 */
    __IO uint32_t SOC_CON3;                           /* Address Offset: 0x040C */
    __IO uint32_t SOC_CON4;                           /* Address Offset: 0x0410 */
    __IO uint32_t SOC_CON5;                           /* Address Offset: 0x0414 */
         uint32_t RESERVED0418[6];                    /* Address Offset: 0x0418 */
    __IO uint32_t VI_CON0;                            /* Address Offset: 0x0430 */
    __IO uint32_t VI_CON1;                            /* Address Offset: 0x0434 */
    __IO uint32_t VI_STATUS;                          /* Address Offset: 0x0438 */
         uint32_t RESERVED043C;                       /* Address Offset: 0x043C */
    __IO uint32_t VO_CON0;                            /* Address Offset: 0x0440 */
    __IO uint32_t VO_CON1;                            /* Address Offset: 0x0444 */
         uint32_t RESERVED0448[14];                   /* Address Offset: 0x0448 */
    __IO uint32_t SOC_STATUS0;                        /* Address Offset: 0x0480 */
         uint32_t RESERVED0484[31];                   /* Address Offset: 0x0484 */
    __IO uint32_t CPU_CON0;                           /* Address Offset: 0x0500 */
    __IO uint32_t CPU_CON1;                           /* Address Offset: 0x0504 */
    __IO uint32_t CPU_CON2;                           /* Address Offset: 0x0508 */
         uint32_t RESERVED050C[5];                    /* Address Offset: 0x050C */
    __IO uint32_t CPU_STATUS0;                        /* Address Offset: 0x0520 */
    __I  uint32_t CPU_STATUS1;                        /* Address Offset: 0x0524 */
         uint32_t RESERVED0528[2];                    /* Address Offset: 0x0528 */
    __IO uint32_t SOC_NOC_CON0;                       /* Address Offset: 0x0530 */
    __IO uint32_t SOC_NOC_CON1;                       /* Address Offset: 0x0534 */
    __IO uint32_t SOC_NOC_CON2;                       /* Address Offset: 0x0538 */
         uint32_t RESERVED053C[49];                   /* Address Offset: 0x053C */
    __IO uint32_t RAM_CON0;                           /* Address Offset: 0x0600 */
    __IO uint32_t RAM_CON1;                           /* Address Offset: 0x0604 */
    __IO uint32_t RAM_CON2;                           /* Address Offset: 0x0608 */
    __IO uint32_t RAM_CON3;                           /* Address Offset: 0x060C */
    __IO uint32_t RAM_CON4;                           /* Address Offset: 0x0610 */
         uint32_t RESERVED0614[91];                   /* Address Offset: 0x0614 */
    __IO uint32_t NPUPVTM_CON0;                       /* Address Offset: 0x0780 */
    __IO uint32_t NPUPVTM_CON1;                       /* Address Offset: 0x0784 */
    __IO uint32_t NPUPVTM_STATUS0;                    /* Address Offset: 0x0788 */
    __IO uint32_t NPUPVTM_STATUS1;                    /* Address Offset: 0x078C */
         uint32_t RESERVED0790[28];                   /* Address Offset: 0x0790 */
    __I  uint32_t CHIP_ID;                            /* Address Offset: 0x0800 */
         uint32_t RESERVED0804[63];                   /* Address Offset: 0x0804 */
    __IO uint32_t MAC_CON0;                           /* Address Offset: 0x0900 */
    __IO uint32_t MAC_CON1;                           /* Address Offset: 0x0904 */
};
/* USB2PHY_GRF Register Structure Define */
struct USB2PHY_GRF_REG {
    __IO uint32_t REG[24];                            /* Address Offset: 0x0000 */
         uint32_t RESERVED0[40];                      /* Address Offset: 0x0060 */
    __IO uint32_t CON[4];                             /* Address Offset: 0x0100 */
    __IO uint32_t INT_MASK;                           /* Address Offset: 0x0110 */
    __IO uint32_t INT_STATUS;                         /* Address Offset: 0x0114 */
    __IO uint32_t INT_STATUS_CLR;                     /* Address Offset: 0x0118 */
         uint32_t RESERVED1;                          /* Address Offset: 0x011C */
    __IO uint32_t STATUS;                             /* Address Offset: 0x0120 */
         uint32_t RESERVED2[3];                       /* Address Offset: 0x0124 */
    __IO uint32_t LS_CON;                             /* Address Offset: 0x0130 */
    __IO uint32_t DIS_CON;                            /* Address Offset: 0x0134 */
    __IO uint32_t BVALID_CON;                         /* Address Offset: 0x0138 */
    __IO uint32_t ID_CON;                             /* Address Offset: 0x013C */
    __IO uint32_t CON4;                               /* Address Offset: 0x0140 */
};
/* USB3PHY_GRF Register Structure Define */
struct USB3PHY_GRF_REG {
    __IO uint32_t PIPE_CON[5];                        /* Address Offset: 0x0000 */
         uint32_t RESERVED0[7];                       /* Address Offset: 0x0014 */
    __IO uint32_t PIPE_STATUS[2];                     /* Address Offset: 0x0030 */
};

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
    __I uint32_t RESP[4];                               /* Address Offset: 0x0030 */
    __IO uint32_t MINTSTS;                            /* Address Offset: 0x0040 */
    __IO uint32_t RINTSTS;                            /* Address Offset: 0x0044 */
    __I uint32_t STATUS;                              /* Address Offset: 0x0048 */
    __IO uint32_t FIFOTH;                             /* Address Offset: 0x004C */
    __I uint32_t CDETECT;                             /* Address Offset: 0x0050 */
    __IO uint32_t WRTPRT;                             /* Address Offset: 0x0054 */
    uint32_t RESERVED0;                               /* Address Offset: 0x0058 */
    __I uint32_t TCBCNT;                              /* Address Offset: 0x005C */
    __I uint32_t TBBCNT;                              /* Address Offset: 0x0060 */
    __IO uint32_t DEBNCE;                             /* Address Offset: 0x0064 */
    __IO uint32_t USRID;                              /* Address Offset: 0x0068 */
    __I uint32_t VERID;                               /* Address Offset: 0x006C */
    __I uint32_t HCON;                                /* Address Offset: 0x0070 */
    __IO uint32_t UHSREG;                             /* Address Offset: 0x0074 */
    __IO uint32_t RSTN;                               /* Address Offset: 0x0078 */
    uint32_t RESERVED1;                               /* Address Offset: 0x007C */
    __IO uint32_t BMOD;                               /* Address Offset: 0x0080 */
    __O uint32_t PLDMND;                              /* Address Offset: 0x0084 */
    __IO uint32_t DBADDR;                             /* Address Offset: 0x0088 */
    __IO uint32_t IDSTS;                              /* Address Offset: 0x008C */
    __IO uint32_t IDINTEN;                            /* Address Offset: 0x0090 */
    __IO uint32_t DSCADDR;                            /* Address Offset: 0x0094 */
    __IO uint32_t BUFADDR;                            /* Address Offset: 0x0098 */
    uint32_t RESERVED2[25];                           /* Address Offset: 0x009C */
    __IO uint32_t CARDTHRCTL;                         /* Address Offset: 0x0100 */
    __IO uint32_t BACKEND_POWER;                      /* Address Offset: 0x0104 */
    uint32_t RESERVED3;                               /* Address Offset: 0x0108 */
    __IO uint32_t EMMCDDR_REG;                        /* Address Offset: 0x010C */
    uint32_t RESERVED4[4];                            /* Address Offset: 0x0110 */
    __IO uint32_t RDYINT_GEN;                         /* Address Offset: 0x0120 */
    uint32_t RESERVED5[55];                           /* Address Offset: 0x0124 */
    __IO uint32_t FIFO_BASE;                          /* Address Offset: 0x0200 */
};

/* PMU_GRF Register Structure Define */
struct PMU_GRF_REG {
    __IO uint32_t GPIO0A_IOMUX;                       /* Address Offset: 0x0000 */
    __IO uint32_t GPIO0B_IOMUX;                       /* Address Offset: 0x0004 */
    __IO uint32_t GPIO0C_IOMUX;                       /* Address Offset: 0x0008 */
         uint32_t RESERVED0;                          /* Address Offset: 0x000C */
    __IO uint32_t GPIO0A_P;                           /* Address Offset: 0x0010 */
    __IO uint32_t GPIO0B_P;                           /* Address Offset: 0x0014 */
    __IO uint32_t GPIO0C_P;                           /* Address Offset: 0x0018 */
         uint32_t RESERVED1;                          /* Address Offset: 0x001C */
    __IO uint32_t GPIO0A_E;                           /* Address Offset: 0x0020 */
    __IO uint32_t GPIO0B_E;                           /* Address Offset: 0x0024 */
    __IO uint32_t GPIO0C_E;                           /* Address Offset: 0x0028 */
         uint32_t RESERVED2;                          /* Address Offset: 0x002C */
    __IO uint32_t GPIO0A_SR;                          /* Address Offset: 0x0030 */
    __IO uint32_t GPIO0B_SR;                          /* Address Offset: 0x0034 */
    __IO uint32_t GPIO0C_SR;                          /* Address Offset: 0x0038 */
         uint32_t RESERVED3;                          /* Address Offset: 0x003C */
    __IO uint32_t GPIO0A_SMT;                         /* Address Offset: 0x0040 */
    __IO uint32_t GPIO0B_SMT;                         /* Address Offset: 0x0044 */
    __IO uint32_t GPIO0C_SMT;                         /* Address Offset: 0x0048 */
         uint32_t RESERVED4[45];                      /* Address Offset: 0x004C */
    __IO uint32_t SOC_CON[5];                         /* Address Offset: 0x0100 */
         uint32_t RESERVED5[3];                       /* Address Offset: 0x0114 */
    __IO uint32_t SOC_STATUS;                         /* Address Offset: 0x0120 */
         uint32_t RESERVED6[7];                       /* Address Offset: 0x0124 */
    __IO uint32_t IO_VSEL[2];                         /* Address Offset: 0x0140 */
    __IO uint32_t IO_VSEL_STATUS;                     /* Address Offset: 0x0148 */
         uint32_t RESERVED7[13];                      /* Address Offset: 0x014C */
    __IO uint32_t PVTM_CON[2];                        /* Address Offset: 0x0180 */
         uint32_t RESERVED8[2];                       /* Address Offset: 0x0188 */
    __IO uint32_t PVTM_STATUS[2];                     /* Address Offset: 0x0190 */
         uint32_t RESERVED9[26];                      /* Address Offset: 0x0198 */
    __IO uint32_t OS_REG[12];                         /* Address Offset: 0x0200 */
    __IO uint32_t RESET_FUNCTION_STATUS;              /* Address Offset: 0x0230 */
    __IO uint32_t RESET_FUNCTION_CLR;                 /* Address Offset: 0x0234 */
         uint32_t RESERVED10[82];                     /* Address Offset: 0x0238 */
    __IO uint32_t SIG_DETECT_CON;                     /* Address Offset: 0x0380 */
         uint32_t RESERVED11[3];                      /* Address Offset: 0x0384 */
    __IO uint32_t SIG_DETECT_STATUS;                  /* Address Offset: 0x0390 */
         uint32_t RESERVED12[3];                      /* Address Offset: 0x0394 */
    __O  uint32_t SIG_DETECT_STATUS_CLEAR;            /* Address Offset: 0x03A0 */
         uint32_t RESERVED13[3];                      /* Address Offset: 0x03A4 */
    __IO uint32_t SDMMC_DET_COUNTER;                  /* Address Offset: 0x03B0 */
};
/* DDR_GRF Register Structure Define */
struct DDR_GRF_REG {
    __IO uint32_t CON[2];                             /* Address Offset: 0x0000 */
    __IO uint32_t SPLIT_CON;                          /* Address Offset: 0x0008 */
         uint32_t RESERVED0[5];                       /* Address Offset: 0x000C */
    __IO uint32_t LP_CON;                             /* Address Offset: 0x0020 */
         uint32_t RESERVED1[55];                      /* Address Offset: 0x0024 */
    __IO uint32_t STATUS[10];                         /* Address Offset: 0x0100 */
};
/* USB_GRF Register Structure Define */
struct USB_GRF_REG {
    __IO uint32_t PCIE_CON0;                          /* Address Offset: 0x0000 */
         uint32_t RESERVED0[3];                       /* Address Offset: 0x0004 */
    __I  uint32_t PCIE_STATUS0;                       /* Address Offset: 0x0010 */
         uint32_t RESERVED1[263];                     /* Address Offset: 0x0014 */
    __IO uint32_t USB3OTG_CON[2];                     /* Address Offset: 0x0430 */
         uint32_t RESERVED2[6];                       /* Address Offset: 0x0438 */
    __IO uint32_t USB3OTG_STATUS[3];                  /* Address Offset: 0x0450 */
         uint32_t RESERVED3[169];                     /* Address Offset: 0x045C */
    __IO uint32_t HOST0_CON[2];                       /* Address Offset: 0x0700 */
         uint32_t RESERVED4[98];                      /* Address Offset: 0x0708 */
    __IO uint32_t HOST0_STATUS;                       /* Address Offset: 0x0890 */
};
/* GRF Register Structure Define */
struct GRF_REG {
    __IO uint32_t CPU_PEFF_CON[9];                    /* Address Offset: 0x0000 */
    __IO uint32_t GIC_BASE_CON;                       /* Address Offset: 0x0024 */
         uint32_t RESERVED0[2];                       /* Address Offset: 0x0028 */
    __IO uint32_t CPU_PERF_RD_MAX_LATENCY_NUM;        /* Address Offset: 0x0030 */
    __I  uint32_t CPU_PERF_RD_LATENCY_SAMP_NUM;       /* Address Offset: 0x0034 */
    __I  uint32_t CPU_PERF_RD_LATENCY_ACC_NUM;        /* Address Offset: 0x0038 */
    __I  uint32_t CPU_PERF_RD_AXI_TOTAL_BYTE;         /* Address Offset: 0x003C */
    __I  uint32_t CPU_PERF_WR_AXI_TOTAL_BYTE;         /* Address Offset: 0x0040 */
    __I  uint32_t CPU_PERF_WORKING_CNT;               /* Address Offset: 0x0044 */
    __I  uint32_t CPU_PERF_INT_STATUS;                /* Address Offset: 0x0048 */
         uint32_t RESERVED1[13];                      /* Address Offset: 0x004C */
    __IO uint32_t COREPVTM_CON[2];                    /* Address Offset: 0x0080 */
    __IO uint32_t COREPVTM_STATUS[2];                 /* Address Offset: 0x0088 */
};
/* SGRF Register Structure Define */
struct SGRF_REG {
    __IO uint32_t SOC_CON[9];                         /* Address Offset: 0x0000 */
         uint32_t RESERVED1[23];                      /* Address Offset: 0x0024 */
    __IO uint32_t DMAC_CON[10];                       /* Address Offset: 0x0080 */
         uint32_t RESERVED2[6];                       /* Address Offset: 0x00A8 */
    __IO uint32_t DDR_BANKHASH_CTRL;                  /* Address Offset: 0x00C0 */
    __IO uint32_t DDR_BANK_MASK[3];                   /* Address Offset: 0x00C4 */
};
/* PMU_SGRF Register Structure Define */
struct PMU_SGRF_REG {
    __IO uint32_t SOC_CON[2];                         /* Address Offset: 0x0000 */
         uint32_t RESERVED0[94];                      /* Address Offset: 0x0008 */
    __IO uint32_t FAST_BOOT_ADDR;                     /* Address Offset: 0x0180 */
};
/* MSCH Register Structure Define */
struct MSCH_REG {
    __I  uint32_t ID_COREID;                          /* Address Offset: 0x0000 */
    __I  uint32_t ID_REVISIONID;                      /* Address Offset: 0x0004 */
    __IO uint32_t DEVICECONF;                         /* Address Offset: 0x0008 */
    __IO uint32_t DEVICESIZE;                         /* Address Offset: 0x000C */
    __IO uint32_t DDRTIMINGA0;                        /* Address Offset: 0x0010 */
    __IO uint32_t DDRTIMINGB0;                        /* Address Offset: 0x0014 */
    __IO uint32_t DDRTIMINGC0;                        /* Address Offset: 0x0018 */
    __IO uint32_t DEVTODEV0;                          /* Address Offset: 0x001C */
         uint32_t RESERVED0[60];                      /* Address Offset: 0x0020 */
    __IO uint32_t DDRMODE;                            /* Address Offset: 0x0110 */
    __IO uint32_t DDR4TIMING;                         /* Address Offset: 0x0114 */
         uint32_t RESERVED1[954];                     /* Address Offset: 0x0118 */
    __IO uint32_t AGINGX0;                            /* Address Offset: 0x1000 */
         uint32_t RESERVED2[15];                      /* Address Offset: 0x1004 */
    __IO uint32_t AGING[3];                           /* Address Offset: 0x1040 */
};
/* CRU Register Structure Define */
struct CRU_REG {
    __IO uint32_t APLL_CON[5];                        /* Address Offset: 0x0000 */
         uint32_t RESERVED0[3];                       /* Address Offset: 0x0014 */
    __IO uint32_t DPLL_CON[5];                        /* Address Offset: 0x0020 */
         uint32_t RESERVED1[3];                       /* Address Offset: 0x0034 */
    __IO uint32_t CPLL_CON[5];                        /* Address Offset: 0x0040 */
         uint32_t RESERVED2[3];                       /* Address Offset: 0x0054 */
    __IO uint32_t GPLL_CON[5];                        /* Address Offset: 0x0060 */
         uint32_t RESERVED3[3];                       /* Address Offset: 0x0074 */
    __IO uint32_t NPLL_CON[5];                        /* Address Offset: 0x0080 */
         uint32_t RESERVED4[3];                       /* Address Offset: 0x0094 */
    __IO uint32_t MODE;                               /* Address Offset: 0x00A0 */
    __IO uint32_t MISC;                               /* Address Offset: 0x00A4 */
    __IO uint32_t MISC1;                              /* Address Offset: 0x00A8 */
         uint32_t RESERVED5;                          /* Address Offset: 0x00AC */
    __IO uint32_t GLB_CNT_TH;                         /* Address Offset: 0x00B0 */
    __IO uint32_t GLB_RST_ST;                         /* Address Offset: 0x00B4 */
    __IO uint32_t GLB_SRST_FST;                       /* Address Offset: 0x00B8 */
    __IO uint32_t GLB_SRST_SND;                       /* Address Offset: 0x00BC */
    __IO uint32_t GLB_RST_CON;                        /* Address Offset: 0x00C0 */
         uint32_t RESERVED6[7];                       /* Address Offset: 0x00C4 */
    __IO uint32_t HWFFC_CON0;                         /* Address Offset: 0x00E0 */
         uint32_t RESERVED7;                          /* Address Offset: 0x00E4 */
    __IO uint32_t HWFFC_TH;                           /* Address Offset: 0x00E8 */
    __IO uint32_t HWFFC_INTST;                        /* Address Offset: 0x00EC */
    __I  uint32_t APLL_CON0_S;                        /* Address Offset: 0x00F0 */
    __I  uint32_t APLL_CON1_S;                        /* Address Offset: 0x00F4 */
    __I  uint32_t CLKSEL_CON0_S;                      /* Address Offset: 0x00F8 */
         uint32_t RESERVED8;                          /* Address Offset: 0x00FC */
    __IO uint32_t CRU_CLKSEL_CON[73];                 /* Address Offset: 0x0100 */
         uint32_t RESERVED9[3];                       /* Address Offset: 0x0224 */
    __IO uint32_t CRU_CLKGATE_CON[20];                /* Address Offset: 0x0230 */
    __O  uint32_t SSGTBL0_3;                          /* Address Offset: 0x0280 */
    __O  uint32_t SSGTBL4_7;                          /* Address Offset: 0x0284 */
    __O  uint32_t SSGTBL8_11;                         /* Address Offset: 0x0288 */
    __O  uint32_t SSGTBL12_15;                        /* Address Offset: 0x028C */
    __O  uint32_t SSGTBL16_19;                        /* Address Offset: 0x0290 */
    __O  uint32_t SSGTBL20_23;                        /* Address Offset: 0x0294 */
    __O  uint32_t SSGTBL24_27;                        /* Address Offset: 0x0298 */
    __O  uint32_t SSGTBL28_31;                        /* Address Offset: 0x029C */
    __O  uint32_t SSGTBL32_35;                        /* Address Offset: 0x02A0 */
    __O  uint32_t SSGTBL36_39;                        /* Address Offset: 0x02A4 */
    __O  uint32_t SSGTBL40_43;                        /* Address Offset: 0x02A8 */
    __O  uint32_t SSGTBL44_47;                        /* Address Offset: 0x02AC */
    __O  uint32_t SSGTBL48_51;                        /* Address Offset: 0x02B0 */
    __O  uint32_t SSGTBL52_55;                        /* Address Offset: 0x02B4 */
    __O  uint32_t SSGTBL56_59;                        /* Address Offset: 0x02B8 */
    __O  uint32_t SSGTBL60_63;                        /* Address Offset: 0x02BC */
    __O  uint32_t SSGTBL64_67;                        /* Address Offset: 0x02C0 */
    __O  uint32_t SSGTBL68_71;                        /* Address Offset: 0x02C4 */
    __O  uint32_t SSGTBL72_75;                        /* Address Offset: 0x02C8 */
    __O  uint32_t SSGTBL76_79;                        /* Address Offset: 0x02CC */
    __O  uint32_t SSGTBL80_83;                        /* Address Offset: 0x02D0 */
    __O  uint32_t SSGTBL84_87;                        /* Address Offset: 0x02D4 */
    __O  uint32_t SSGTBL88_91;                        /* Address Offset: 0x02D8 */
    __O  uint32_t SSGTBL92_95;                        /* Address Offset: 0x02DC */
    __O  uint32_t SSGTBL96_99;                        /* Address Offset: 0x02E0 */
    __O  uint32_t SSGTBL100_103;                      /* Address Offset: 0x02E4 */
    __O  uint32_t SSGTBL104_107;                      /* Address Offset: 0x02E8 */
    __O  uint32_t SSGTBL108_111;                      /* Address Offset: 0x02EC */
    __O  uint32_t SSGTBL112_115;                      /* Address Offset: 0x02F0 */
    __O  uint32_t SSGTBL116_119;                      /* Address Offset: 0x02F4 */
    __O  uint32_t SSGTBL120_123;                      /* Address Offset: 0x02F8 */
    __O  uint32_t SSGTBL124_127;                      /* Address Offset: 0x02FC */
    __IO uint32_t CRU_SOFTRST_CON[16];                /* Address Offset: 0x0300 */
         uint32_t RESERVED10[16];                     /* Address Offset: 0x0340 */
    __IO uint32_t SDMMC_CON[2];                       /* Address Offset: 0x0380 */
    __IO uint32_t SDIO_CON[2];                        /* Address Offset: 0x0388 */
    __IO uint32_t EMMC_CON[2];                        /* Address Offset: 0x0390 */
         uint32_t RESERVED11[26];                     /* Address Offset: 0x0398 */
    __IO uint32_t AUTOCS_CON[10];                     /* Address Offset: 0x0400 */
         uint32_t RESERVED12[3830];                   /* Address Offset: 0x0428 */
    __IO uint32_t PMUPLL_CON[5];                      /* Address Offset: 0x4000 */
         uint32_t RESERVED13[3];                      /* Address Offset: 0x4014 */
    __IO uint32_t PMU_MODE;                           /* Address Offset: 0x4020 */
         uint32_t RESERVED14[7];                      /* Address Offset: 0x4024 */
    __IO uint32_t PMU_CLKSEL_CON[8];                  /* Address Offset: 0x4040 */
         uint32_t RESERVED15[8];                      /* Address Offset: 0x4060 */
    __IO uint32_t PMU_CLKGATE_CON[2];                 /* Address Offset: 0x4080 */
         uint32_t RESERVED16[14];                     /* Address Offset: 0x4088 */
    __IO uint32_t PMU_AUTOCS_CON[2];                  /* Address Offset: 0x40C0 */
};
/* PMU Register Structure Define */
struct PMU_REG {
    __IO uint32_t WAKEUP_CFG0_LO;                        /* Address Offset: 0x0000 */
    __IO uint32_t WAKEUP_CFG0_HI;                        /* Address Offset: 0x0004 */
    __IO uint32_t WAKEUP_CFG1_LO;                        /* Address Offset: 0x0008 */
    __IO uint32_t WAKEUP_CFG1_HI;                        /* Address Offset: 0x000C */
    __IO uint32_t WAKEUP_CFG2_LO;                        /* Address Offset: 0x0010 */
         uint32_t RESERVED0;                             /* Address Offset: 0x0014 */
    __IO uint32_t PWRDN_CON;                             /* Address Offset: 0x0018 */
    __IO uint32_t PWRDN_CON_HI;                          /* Address Offset: 0x001C */
    __IO uint32_t PWRDN_ST;                              /* Address Offset: 0x0020 */
    __IO uint32_t PWRMODE_CORE_CON_LO;                   /* Address Offset: 0x0024 */
    __IO uint32_t PWRMODE_CORE_CON_HI;                   /* Address Offset: 0x0028 */
    __IO uint32_t PWRMODE_CORE_COMMON_CON_LO;            /* Address Offset: 0x002C */
    __IO uint32_t PWRMODE_CORE_COMMON_CON_HI;            /* Address Offset: 0x0030 */
    __IO uint32_t SFT_CON_LO;                            /* Address Offset: 0x0034 */
    __IO uint32_t SFT_CON_HI;                            /* Address Offset: 0x0038 */
         uint32_t RESERVED1[2];                          /* Address Offset: 0x003C */
    __IO uint32_t INT_ST;                                /* Address Offset: 0x0044 */
         uint32_t RESERVED2[4];                          /* Address Offset: 0x0048 */
    __IO uint32_t GPIO_POS_INT_ST;                       /* Address Offset: 0x0058 */
    __IO uint32_t GPIO_NEG_INT_ST;                       /* Address Offset: 0x005C */
    __IO uint32_t CORE_PWR_ST;                           /* Address Offset: 0x0060 */
    __IO uint32_t BUS_IDLE_REQ;                          /* Address Offset: 0x0064 */
    __IO uint32_t BUS_IDLE_REQ_HI;                       /* Address Offset: 0x0068 */
    __IO uint32_t BUS_IDLE_ST;                           /* Address Offset: 0x006C */
    __IO uint32_t BUS_IDLE_ST1;                          /* Address Offset: 0x0070 */
         uint32_t RESERVED3;                             /* Address Offset: 0x0074 */
    __IO uint32_t OSC_CNT_LO;                            /* Address Offset: 0x0078 */
    __IO uint32_t OSC_CNT_HI;                            /* Address Offset: 0x007C */
    __IO uint32_t PLLLOCK_CNT_LO;                        /* Address Offset: 0x0080 */
    __IO uint32_t PLLLOCK_CNT_HI;                        /* Address Offset: 0x0084 */
    __IO uint32_t PLLRST_CNT_LO;                         /* Address Offset: 0x0088 */
    __IO uint32_t PLLRST_CNT_HI;                         /* Address Offset: 0x008C */
    __IO uint32_t STABLE_CNT_LO;                         /* Address Offset: 0x0090 */
    __IO uint32_t STABLE_CNT_HI;                         /* Address Offset: 0x0094 */
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
/* EFUSE_NS Register Structure Define */
struct EFUSE_NS_REG {
    __IO uint32_t MOD;                                /* Address Offset: 0x0000 */
    __IO uint32_t RD_MASK;                            /* Address Offset: 0x0004 */
    __IO uint32_t PG_MASK;                            /* Address Offset: 0x0008 */
         uint32_t RESERVED0[2];                       /* Address Offset: 0x000C */
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
/* EFUSE_S Register Structure Define */
struct EFUSE_S_REG {
    __IO uint32_t MOD;                                /* Address Offset: 0x0000 */
    __IO uint32_t RD_MASK;                            /* Address Offset: 0x0004 */
    __IO uint32_t PG_MASK;                            /* Address Offset: 0x0008 */
         uint32_t RESERVED0[2];                       /* Address Offset: 0x000C */
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
/* SARADC Register Structure Define */
struct SARADC_REG {
    __I  uint32_t DATA;                               /* Address Offset: 0x0000 */
    __I  uint32_t STAS;                               /* Address Offset: 0x0004 */
    __IO uint32_t CTRL;                               /* Address Offset: 0x0008 */
    __IO uint32_t DLY_PU_SOC;                         /* Address Offset: 0x000C */
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
/* DMA Register Structure Define */
struct DMA_REG {
    __I  uint32_t DSR;                                /* Address Offset: 0x0000 */
    __I  uint32_t DPC;                                /* Address Offset: 0x0004 */
         uint32_t RESERVED0[6];                       /* Address Offset: 0x0008 */
    __IO uint32_t INTEN;                              /* Address Offset: 0x0020 */
    __I  uint32_t EVENT_RIS;                          /* Address Offset: 0x0024 */
    __I  uint32_t INTMIS;                             /* Address Offset: 0x0028 */
    __O  uint32_t INTCLR;                             /* Address Offset: 0x002C */
    __I  uint32_t FSRD;                               /* Address Offset: 0x0030 */
    __I  uint32_t FSRC;                               /* Address Offset: 0x0034 */
    __IO uint32_t FTRD;                               /* Address Offset: 0x0038 */
         uint32_t RESERVED1;                          /* Address Offset: 0x003C */
    __IO uint32_t FTR[6];                             /* Address Offset: 0x0040 */
         uint32_t RESERVED2[42];                      /* Address Offset: 0x0058 */
    struct DMA_CHANNEL_STATUS CHAN_STS[6];            /* Address Offset: 0x0100 */
         uint32_t RESERVED3[180];                     /* Address Offset: 0x0130 */
    struct DMA_CHANNEL_CONFIG CHAN_CFG[6];            /* Address Offset: 0x0400 */
         uint32_t RESERVED9[528];                     /* Address Offset: 0x04C0 */
    __I  uint32_t DBGSTATUS;                          /* Address Offset: 0x0D00 */
    __O  uint32_t DBGCMD;                             /* Address Offset: 0x0D04 */
    __IO uint32_t DBGINST[2];                         /* Address Offset: 0x0D08 */
         uint32_t RESERVED10[60];                     /* Address Offset: 0x0D10 */
    __IO uint32_t CR[5];                              /* Address Offset: 0x0E00 */
    __I  uint32_t CRDN;                               /* Address Offset: 0x0E14 */
         uint32_t RESERVED11[26];                     /* Address Offset: 0x0E18 */
    __IO uint32_t WD;                                 /* Address Offset: 0x0E80 */
};

/* DCF Register Structure Define */
struct DCF_REG {
    __IO uint32_t DCF_CTRL;                           /* Address Offset: 0x0000 */
    __IO uint32_t DCF_STATUS;                         /* Address Offset: 0x0004 */
    __IO uint32_t DCF_ADDR;                           /* Address Offset: 0x0008 */
    __O  uint32_t DCF_ISR;                            /* Address Offset: 0x000C */
         uint32_t RESERVED0;                          /* Address Offset: 0x0010 */
    __IO uint32_t DCF_TIMEOUT_CYC;                    /* Address Offset: 0x0014 */
         uint32_t RESERVED1[2];                       /* Address Offset: 0x0018 */
    __IO uint32_t DCF_CURR_R[2];                      /* Address Offset: 0x0020 */
    __I  uint32_t DCF_CMD_COUNTER;                    /* Address Offset: 0x0028 */
         uint32_t RESERVED2;                          /* Address Offset: 0x002C */
    __IO uint32_t DCF_LAST_ADDR[4];                   /* Address Offset: 0x0030 */
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
         uint32_t RESERVED0[54];                      /* Address Offset: 0x0028 */
    __IO uint32_t TXDATA[8];                          /* Address Offset: 0x0100 */
         uint32_t RESERVED1[56];                      /* Address Offset: 0x0120 */
    __IO uint32_t RXDATA[8];                          /* Address Offset: 0x0200 */
    __I  uint32_t ST;                                 /* Address Offset: 0x0220 */
    __IO uint32_t DBGCTRL;                            /* Address Offset: 0x0224 */
};
/* SPI2APB Register Structure Define */
struct SPI2APB_REG {
         uint32_t RESERVED0000[11];                   /* Address Offset: 0x0000 */
    __IO uint32_t IMR;                                /* Address Offset: 0x002C */
    __IO uint32_t ISR;                                /* Address Offset: 0x0030 */
    __IO uint32_t RISR;                               /* Address Offset: 0x0034 */
    __IO uint32_t ICR;                                /* Address Offset: 0x0038 */
         uint32_t RESERVED003C[4];                    /* Address Offset: 0x003C */
    __IO uint32_t WORK_MODE;                          /* Address Offset: 0x004C */
    __IO uint32_t MESSAGE_REG[3];                     /* Address Offset: 0x0050 */
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
    __IO uint32_t SPI2APB;                            /* Address Offset: 0x004C */
    __IO uint32_t QUICK_REG[3];                       /* Address Offset: 0x0050 */
         uint32_t RESERVED005C[233];                  /* Address Offset: 0x005C */
    __O  uint32_t TXDR;                               /* Address Offset: 0x0400 */
         uint32_t RESERVED0404[255];                  /* Address Offset: 0x0404 */
    __IO uint32_t RXDR;                               /* Address Offset: 0x0800 */
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
         uint32_t RESERVED0028[2];                    /* Address Offset: 0x0028 */
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
         uint32_t RESERVED005C[233];                  /* Address Offset: 0x005C */
    __I  uint32_t INCR_RXDR;                          /* Address Offset: 0x0400 */
};
/* VAD Register Structure Define */
struct VAD_REG {
    __IO uint32_t CONTROL;                            /* Address Offset: 0x0000 */
    __IO uint32_t VS_ADDR;                            /* Address Offset: 0x0004 */
    __IO uint32_t ACODEC_BASE_ADDR;                   /* Address Offset: 0x0008 */
    __IO uint32_t OD_ADDR[8];                         /* Address Offset: 0x000C */
    __IO uint32_t D_DATA[8];                          /* Address Offset: 0x002C */
    __IO uint32_t TIMEOUT;                            /* Address Offset: 0x004C */
    __IO uint32_t RAM_START_ADDR;                     /* Address Offset: 0x0050 */
    __IO uint32_t RAM_END_ADDR;                       /* Address Offset: 0x0054 */
    __IO uint32_t RAM_CUR_ADDR;                       /* Address Offset: 0x0058 */
    __IO uint32_t DET_CON[6];                         /* Address Offset: 0x005C */
    __IO uint32_t INT;                                /* Address Offset: 0x0074 */
    __IO uint32_t AUX_CONTROL;                        /* Address Offset: 0x0078 */
    __I  uint32_t SAMPLE_CNT;                         /* Address Offset: 0x007C */
         uint32_t RESERVED0[32];                      /* Address Offset: 0x0080 */
    __IO uint32_t NOISE_DATA;                         /* Address Offset: 0x0100 */
};
/* DFICTRL Register Structure Define */
struct DFICTRL_REG {
    __IO uint32_t START;                              /* Address Offset: 0x0000 */
    __IO uint32_t ADDR;                               /* Address Offset: 0x0004 */
    __IO uint32_t CMD;                                /* Address Offset: 0x0008 */
         uint32_t RESERVED0[5];                       /* Address Offset: 0x000C */
    __IO uint32_t STAT;                               /* Address Offset: 0x0020 */
         uint32_t RESERVED1[7];                       /* Address Offset: 0x0024 */
    __IO uint32_t DAT[8];                             /* Address Offset: 0x0040 */
    __IO uint32_t TIM0;                               /* Address Offset: 0x0060 */
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
    __I  uint32_t RISR;                               /* Address Offset: 0x0028 */
    __IO uint32_t VER;                                /* Address Offset: 0x002C */
    __IO uint32_t QOP;                                /* Address Offset: 0x0030 */
    __IO uint32_t EXT_CTRL;                           /* Address Offset: 0x0034 */
         uint32_t RESERVED0;                          /* Address Offset: 0x0038 */
    __IO uint32_t DLL_CTRL;                           /* Address Offset: 0x003C */
         uint32_t RESERVED1[4];                       /* Address Offset: 0x0040 */
    __IO uint32_t POLL_CTRL;                          /* Address Offset: 0x0050 */
    __O  uint32_t FETCH_CMD;                          /* Address Offset: 0x0054 */
    __IO uint32_t FETCH_CTRL;                         /* Address Offset: 0x0058 */
    __IO uint32_t XIP_MODE;                           /* Address Offset: 0x005C */
    __I  uint32_t POLL_DATA;                          /* Address Offset: 0x0060 */
         uint32_t RESERVED2[7];                       /* Address Offset: 0x0064 */
    __O  uint32_t DMATR;                              /* Address Offset: 0x0080 */
    __IO uint32_t DMAADDR;                            /* Address Offset: 0x0084 */
         uint32_t RESERVED3[30];                      /* Address Offset: 0x0088 */
    __O  uint32_t CMD;                                /* Address Offset: 0x0100 */
    __O  uint32_t ADDR;                               /* Address Offset: 0x0104 */
    __IO uint32_t DATA;                               /* Address Offset: 0x0108 */
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
#endif /* __ASSEMBLY__ */
/****************************************************************************************/
/*                                                                                      */
/*                                Module Address Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Memory Base */
#define BUS_GRF_BASE        0xFE000000U /* BUS_GRF base address */
#define USB2PHY_GRF_BASE    0xFE010000U /* USB2PHY_GRF base address */
#define USB3PHY_GRF_BASE    0xFE018000U /* USB3PHY_GRF base address */
#define PMU_GRF_BASE        0xFE020000U /* PMU_GRF base address */
#define DDR_GRF_BASE        0xFE030000U /* DDR_GRF base address */
#define USB_GRF_BASE        0xFE040000U /* USB_GRF base address */
#define GRF_BASE            0xFE050000U /* GRF base address */
#define SGRF_BASE           0xFE400000U /* SGRF base address */
#define PMU_SGRF_BASE       0xFE410000U /* PMU_SGRF base address */
#define MSCH_BASE           0xFE800000U /* MSCH base address */
#define GICD_BASE           0xFF100000U /* GICD base address */
#define GICR_BASE           0xFF140000U /* GICR base address */
#define GICC_BASE           0xFF300000U /* GICC base address */
#define CRU_BASE            0xFF350000U /* CRU base address */
#define EFUSE_NS_BASE       0xFF3B0000U /* EFUSE_NS base address */
#define EFUSE_S_BASE        0xFF3B8000U /* EFUSE_S base address */
#define SARADC_BASE         0xFF3C0000U /* SARADC base address */
#define PWM0_BASE           0xFF3D0000U /* PWM0 base address */
#define PWM1_BASE           0xFF3D8000U /* PWM1 base address */
#define PMU_BASE            0xFF3E0000U /* PMU base address */
#define I2C0_BASE           0xFF410000U /* I2C0 base address */
#define UART0_BASE          0xFF430000U /* UART0 base address */
#define GPIO0_BASE          0xFF4C0000U /* GPIO0 base address */
#define DMA_BASE            0xFF4E0000U /* DMA base address */
#define I2C1_BASE           0xFF500000U /* I2C1 base address */
#define I2C2_BASE           0xFF504000U /* I2C2 base address */
#define I2C3_BASE           0xFF508000U /* I2C3 base address */
#define I2C4_BASE           0xFF50C000U /* I2C4 base address */
#define I2C5_BASE           0xFF510000U /* I2C5 base address */
#define SPI2APB_BASE        0xFF520000U /* SPI2APB base address */
#define SPI1_BASE           0xFF530000U /* SPI1 base address */
#define UART1_BASE          0xFF540000U /* UART1 base address */
#define UART2_BASE          0xFF550000U /* UART2 base address */
#define UART3_BASE          0xFF560000U /* UART3 base address */
#define UART4_BASE          0xFF570000U /* UART4 base address */
#define SPI2_BASE           0xFF580000U /* SPI2 base address */
#define UART5_BASE          0xFF5A0000U /* UART5 base address */
#define UART6_BASE          0xFF5B0000U /* UART6 base address */
#define UART7_BASE          0xFF5C0000U /* UART7 base address */
#define PWM2_BASE           0xFF5D0000U /* PWM2 base address */
#define DCF_BASE            0xFF640000U /* DCF base address */
#define GPIO1_BASE          0xFF690000U /* GPIO1 base address */
#define GPIO2_BASE          0xFF6A0000U /* GPIO2 base address */
#define GPIO3_BASE          0xFF6B0000U /* GPIO3 base address */
#define GPIO4_BASE          0xFF6C0000U /* GPIO4 base address */
#define TIMER_BASE          0xFF700000U /* TIMER base address */
#define STIMER_BASE         0xFF710000U /* STIMER0-1 base address */
#define WDT_BASE            0xFF720000U /* WDT base address */
#define I2STDM0_BASE        0xFF7E0000U /* I2STDM0 base address */
#define I2S1_BASE           0xFF7F0000U /* I2S1 base address */
#define PDM0_BASE           0xFF800000U /* PDM base address */
#define VAD_BASE            0xFF810000U /* VAD base address */
#define DFICTRL_BASE        0xFF9B8000U /* DFICTRL base address */
#define RGA_BASE            0xFFAF0000U /* RGA base address */
#define SFC_BASE            0xFFC50000U /* SFC base address */
#define SDIO_BASE           0xFFC60000U /* SDIO base address */
#define SDMMC_BASE          0xFFCF0000U /* SDMMC base address */

/****************************************************************************************/
/*                                                                                      */
/*                               Module Variable Section                                */
/*                                                                                      */
/****************************************************************************************/
/* Module Variable Define */

#define BUS_GRF             ((struct BUS_GRF_REG *) BUS_GRF_BASE)
#define USB2PHY_GRF         ((struct USB2PHY_GRF_REG *) USB2PHY_GRF_BASE)
#define USB3PHY_GRF         ((struct USB3PHY_GRF_REG *) USB3PHY_GRF_BASE)
#define PMU_GRF             ((struct PMU_GRF_REG *) PMU_GRF_BASE)
#define DDR_GRF             ((struct DDR_GRF_REG *) DDR_GRF_BASE)
#define USB_GRF             ((struct USB_GRF_REG *) USB_GRF_BASE)
#define GRF                 ((struct GRF_REG *) GRF_BASE)
#define SGRF                ((struct SGRF_REG *) SGRF_BASE)
#define PMU_SGRF            ((struct PMU_SGRF_REG *) PMU_SGRF_BASE)
#define CRU                 ((struct CRU_REG *) CRU_BASE)
#define MSCH                ((struct MSCH_REG *) MSCH_BASE)
#define PMU                 ((struct PMU_REG *) PMU_BASE)
#define EFUSE_NS            ((struct EFUSE_NS_REG *) EFUSE_NS_BASE)
#define EFUSE_S             ((struct EFUSE_S_REG *) EFUSE_S_BASE)
#define SARADC              ((struct SARADC_REG *) SARADC_BASE)
#define DCF                 ((struct DCF_REG *) DCF_BASE)
#define DFICTRL             ((struct DFICTRL_REG *) DFICTRL_BASE)
#define RGA                 ((struct RGA_REG *) RGA_BASE)
#define DMA                 ((struct DMA_REG *) DMA_BASE)
#define I2STDM0             ((struct I2STDM_REG *) I2STDM0_BASE)
#define I2S1                ((struct I2S_REG *) I2S1_BASE)
#define PDM0                ((struct PDM_REG *) PDM0_BASE)
#define VAD                 ((struct VAD_REG *) VAD_BASE)
#define PWM0                ((struct PWM_REG *) PWM0_BASE)
#define PWM1                ((struct PWM_REG *) PWM1_BASE)
#define PWM2                ((struct PWM_REG *) PWM2_BASE)
#define SPI2APB             ((struct SPI2APB_REG *) SPI2APB_BASE)
#define SPI0                (SPI2APB)
#define SPI1                ((struct SPI_REG *) SPI1_BASE)
#define SPI2                ((struct SPI_REG *) SPI2_BASE)
#define UART0               ((struct UART_REG *) UART0_BASE)
#define UART1               ((struct UART_REG *) UART1_BASE)
#define UART2               ((struct UART_REG *) UART2_BASE)
#define UART3               ((struct UART_REG *) UART3_BASE)
#define UART4               ((struct UART_REG *) UART4_BASE)
#define UART5               ((struct UART_REG *) UART5_BASE)
#define UART6               ((struct UART_REG *) UART6_BASE)
#define UART7               ((struct UART_REG *) UART7_BASE)
#define GPIO0               ((struct GPIO_REG *) GPIO0_BASE)
#define GPIO1               ((struct GPIO_REG *) GPIO1_BASE)
#define GPIO2               ((struct GPIO_REG *) GPIO2_BASE)
#define GPIO3               ((struct GPIO_REG *) GPIO3_BASE)
#define GPIO4               ((struct GPIO_REG *) GPIO4_BASE)
#define TIMER0              ((struct TIMER_REG *) TIMER_BASE)
#define TIMER1              ((struct TIMER_REG *) (TIMER_BASE + 0x20))
#define TIMER2              ((struct TIMER_REG *) (TIMER_BASE + 0x40))
#define TIMER3              ((struct TIMER_REG *) (TIMER_BASE + 0x60))
#define TIMER4              ((struct TIMER_REG *) (TIMER_BASE + 0x80))
#define TIMER5              ((struct TIMER_REG *) (TIMER_BASE + 0xA0))
#define SFC                 ((struct SFC_REG *) SFC_BASE)
#define I2C0                ((struct I2C_REG *) I2C0_BASE)
#define I2C1                ((struct I2C_REG *) I2C1_BASE)
#define I2C2                ((struct I2C_REG *) I2C2_BASE)
#define I2C3                ((struct I2C_REG *) I2C3_BASE)
#define I2C4                ((struct I2C_REG *) I2C4_BASE)
#define I2C5                ((struct I2C_REG *) I2C5_BASE)

#define IS_BUS_GRF_INSTANCE(instance) ((instance) == BUS_GRF)
#define IS_USB2PHY_GRF_INSTANCE(instance) ((instance) == USB2PHY_GRF)
#define IS_USB3PHY_GRF_INSTANCE(instance) ((instance) == USB3PHY_GRF)
#define IS_PMU_GRF_INSTANCE(instance) ((instance) == PMU_GRF)
#define IS_DDR_GRF_INSTANCE(instance) ((instance) == DDR_GRF)
#define IS_USB_GRF_INSTANCE(instance) ((instance) == USB_GRF)
#define IS_GRF_INSTANCE(instance) ((instance) == GRF)
#define IS_SGRF_INSTANCE(instance) ((instance) == SGRF)
#define IS_PMU_SGRF_INSTANCE(instance) ((instance) == PMU_SGRF)
#define IS_MSCH_INSTANCE(instance) ((instance) == MSCH)
#define IS_WDT_INSTANCE(instance) ((instance) == WDT)
#define IS_EFUSE_NS_INSTANCE(instance) ((instance) == EFUSE_NS)
#define IS_EFUSE_S_INSTANCE(instance) ((instance) == EFUSE_S)
#define IS_SARADC_INSTANCE(instance) ((instance) == SARADC)
#define IS_DCF_INSTANCE(instance) ((instance) == DCF)
#define IS_DFICTRL_INSTANCE(instance) ((instance) == DFICTRL)
#define IS_RGA_INSTANCE(instance) ((instance) == RGA)
#define IS_PWM_INSTANCE(instance) (((instance) == PWM0) || ((instance) == PWM1) || ((instance) == PWM2))
#define IS_UART_INSTANCE(instance) (((instance) == UART0) || ((instance) == UART1) || ((instance) == UART2) || ((instance) == UART3) || ((instance) == UART4) || ((instance) == UART5) || ((instance) == UART6) || ((instance) == UART7))
#define IS_GPIO_INSTANCE(instance) (((instance) == GPIO0) || ((instance) == GPIO1) || ((instance) == GPIO2) || ((instance) == GPIO3) || ((instance) == GPIO4))
#define IS_TIMER_INSTANCE(instance) (((instance) == TIMER0) || ((instance) == TIMER1) || ((instance) == TIMER2) || ((instance) == TIMER3) || ((instance) == TIMER4) || ((instance) == TIMER5))
#define IS_CRU_INSTANCE(instance) ((instance) == CRU)
#define IS_SPI2APB_INSTANCE(instance) ((instance) == SPI2APB)
#define IS_SPI_INSTANCE(instance) (((instance) == SPI1) || ((instance) == SPI2))
#define IS_I2S_INSTANCE(instance) ((instance) == I2S1)
#define IS_I2STDM_INSTANCE(instance) ((instance) == I2STDM0)
#define IS_DMA_INSTANCE(instance) ((instance) == DMA)
#define IS_VAD_INSTANCE(instance) ((instance) == VAD)
#define IS_PDM_INSTANCE(instance) ((instance) == PDM0)
#define IS_SFC_INSTANCE(instance) ((instance) == SFC)
#define IS_I2C_INSTANCE(instance) (((instance) == I2C0) || ((instance) == I2C1) || ((instance) == I2C2) || ((instance) == I2C3) || ((instance) == I2C4) || ((instance) == I2C5))
/****************************************************************************************/
/*                                                                                      */
/*                               Register Bitmap Section                                */
/*                                                                                      */
/****************************************************************************************/
/****************************************BUS_GRF*****************************************/
/* GPIO1A_IOMUX_L */
#define BUS_GRF_GPIO1A_IOMUX_L_OFFSET                      (0x0)
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_L_GPIO1A0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_L_GPIO1A1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_L_GPIO1A2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_L_GPIO1A3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO1A_IOMUX_H */
#define BUS_GRF_GPIO1A_IOMUX_H_OFFSET                      (0x4)
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_H_GPIO1A4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_H_GPIO1A5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_H_GPIO1A6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_MASK            (0xFU << BUS_GRF_GPIO1A_IOMUX_H_GPIO1A7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO1B_IOMUX_L */
#define BUS_GRF_GPIO1B_IOMUX_L_OFFSET                      (0x8)
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_L_GPIO1B0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_L_GPIO1B1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_L_GPIO1B2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO1B_IOMUX_H */
#define BUS_GRF_GPIO1B_IOMUX_H_OFFSET                      (0xC)
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_H_GPIO1B4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_H_GPIO1B5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_H_GPIO1B6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_MASK            (0xFU << BUS_GRF_GPIO1B_IOMUX_H_GPIO1B7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2A_IOMUX_L */
#define BUS_GRF_GPIO2A_IOMUX_L_OFFSET                      (0x20)
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A0_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_L_GPIO2A0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A1_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_L_GPIO2A1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A2_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_L_GPIO2A2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO2A_IOMUX_L_GPIO2A3_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_L_GPIO2A3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2A_IOMUX_H */
#define BUS_GRF_GPIO2A_IOMUX_H_OFFSET                      (0x24)
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A4_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_H_GPIO2A4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A5_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_H_GPIO2A5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A6_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_H_GPIO2A6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO2A_IOMUX_H_GPIO2A7_SEL_MASK            (0xFU << BUS_GRF_GPIO2A_IOMUX_H_GPIO2A7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2B_IOMUX_L */
#define BUS_GRF_GPIO2B_IOMUX_L_OFFSET                      (0x28)
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B0_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_L_GPIO2B0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B1_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_L_GPIO2B1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B2_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_L_GPIO2B2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO2B_IOMUX_L_GPIO2B3_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_L_GPIO2B3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2B_IOMUX_H */
#define BUS_GRF_GPIO2B_IOMUX_H_OFFSET                      (0x2C)
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B4_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_H_GPIO2B4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B5_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_H_GPIO2B5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B6_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_H_GPIO2B6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO2B_IOMUX_H_GPIO2B7_SEL_MASK            (0xFU << BUS_GRF_GPIO2B_IOMUX_H_GPIO2B7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2C_IOMUX_L */
#define BUS_GRF_GPIO2C_IOMUX_L_OFFSET                      (0x30)
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C0_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_L_GPIO2C0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C1_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_L_GPIO2C1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C2_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_L_GPIO2C2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO2C_IOMUX_L_GPIO2C3_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_L_GPIO2C3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2C_IOMUX_H */
#define BUS_GRF_GPIO2C_IOMUX_H_OFFSET                      (0x34)
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C4_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_H_GPIO2C4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C5_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_H_GPIO2C5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C6_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_H_GPIO2C6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO2C_IOMUX_H_GPIO2C7_SEL_MASK            (0xFU << BUS_GRF_GPIO2C_IOMUX_H_GPIO2C7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO2D_IOMUX_L */
#define BUS_GRF_GPIO2D_IOMUX_L_OFFSET                      (0x38)
#define BUS_GRF_GPIO2D_IOMUX_L_GPIO2D0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO2D_IOMUX_L_GPIO2D0_SEL_MASK            (0xFU << BUS_GRF_GPIO2D_IOMUX_L_GPIO2D0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO2D_IOMUX_L_GPIO2D1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO2D_IOMUX_L_GPIO2D1_SEL_MASK            (0xFU << BUS_GRF_GPIO2D_IOMUX_L_GPIO2D1_SEL_SHIFT)           /* 0x000000F0 */
/* GPIO3A_IOMUX_L */
#define BUS_GRF_GPIO3A_IOMUX_L_OFFSET                      (0x40)
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A0_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_L_GPIO3A0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A1_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_L_GPIO3A1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A2_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_L_GPIO3A2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3A_IOMUX_L_GPIO3A3_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_L_GPIO3A3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO3A_IOMUX_H */
#define BUS_GRF_GPIO3A_IOMUX_H_OFFSET                      (0x44)
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A4_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_H_GPIO3A4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A5_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_H_GPIO3A5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A6_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_H_GPIO3A6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3A_IOMUX_H_GPIO3A7_SEL_MASK            (0xFU << BUS_GRF_GPIO3A_IOMUX_H_GPIO3A7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO3B_IOMUX_L */
#define BUS_GRF_GPIO3B_IOMUX_L_OFFSET                      (0x48)
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B0_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_L_GPIO3B0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B1_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_L_GPIO3B1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B2_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_L_GPIO3B2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3B_IOMUX_L_GPIO3B3_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_L_GPIO3B3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO3B_IOMUX_H */
#define BUS_GRF_GPIO3B_IOMUX_H_OFFSET                      (0x4C)
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B4_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_H_GPIO3B4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B5_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_H_GPIO3B5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B6_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_H_GPIO3B6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3B_IOMUX_H_GPIO3B7_SEL_MASK            (0xFU << BUS_GRF_GPIO3B_IOMUX_H_GPIO3B7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO3C_IOMUX_L */
#define BUS_GRF_GPIO3C_IOMUX_L_OFFSET                      (0x50)
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C0_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_L_GPIO3C0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C1_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_L_GPIO3C1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C2_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_L_GPIO3C2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3C_IOMUX_L_GPIO3C3_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_L_GPIO3C3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO3C_IOMUX_H */
#define BUS_GRF_GPIO3C_IOMUX_H_OFFSET                      (0x54)
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C4_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_H_GPIO3C4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C5_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_H_GPIO3C5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C6_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_H_GPIO3C6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3C_IOMUX_H_GPIO3C7_SEL_MASK            (0xFU << BUS_GRF_GPIO3C_IOMUX_H_GPIO3C7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO3D_IOMUX_L */
#define BUS_GRF_GPIO3D_IOMUX_L_OFFSET                      (0x58)
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D0_SEL_MASK            (0xFU << BUS_GRF_GPIO3D_IOMUX_L_GPIO3D0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D1_SEL_MASK            (0xFU << BUS_GRF_GPIO3D_IOMUX_L_GPIO3D1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D2_SEL_MASK            (0xFU << BUS_GRF_GPIO3D_IOMUX_L_GPIO3D2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO3D_IOMUX_L_GPIO3D3_SEL_MASK            (0xFU << BUS_GRF_GPIO3D_IOMUX_L_GPIO3D3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO4A_IOMUX_L */
#define BUS_GRF_GPIO4A_IOMUX_L_OFFSET                      (0x60)
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A0_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_L_GPIO4A0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A1_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_L_GPIO4A1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A2_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_L_GPIO4A2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO4A_IOMUX_L_GPIO4A3_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_L_GPIO4A3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO4A_IOMUX_H */
#define BUS_GRF_GPIO4A_IOMUX_H_OFFSET                      (0x64)
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A4_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_H_GPIO4A4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A5_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_H_GPIO4A5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A6_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_H_GPIO4A6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO4A_IOMUX_H_GPIO4A7_SEL_MASK            (0xFU << BUS_GRF_GPIO4A_IOMUX_H_GPIO4A7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO4B_IOMUX_L */
#define BUS_GRF_GPIO4B_IOMUX_L_OFFSET                      (0x68)
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B0_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_L_GPIO4B0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B1_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_L_GPIO4B1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B2_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_L_GPIO4B2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO4B_IOMUX_L_GPIO4B3_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_L_GPIO4B3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO4B_IOMUX_H */
#define BUS_GRF_GPIO4B_IOMUX_H_OFFSET                      (0x6C)
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B4_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_H_GPIO4B4_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B5_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B5_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_H_GPIO4B5_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B6_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B6_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_H_GPIO4B6_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B7_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO4B_IOMUX_H_GPIO4B7_SEL_MASK            (0xFU << BUS_GRF_GPIO4B_IOMUX_H_GPIO4B7_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO4C_IOMUX_L */
#define BUS_GRF_GPIO4C_IOMUX_L_OFFSET                      (0x70)
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C0_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C0_SEL_MASK            (0xFU << BUS_GRF_GPIO4C_IOMUX_L_GPIO4C0_SEL_SHIFT)           /* 0x0000000F */
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C1_SEL_SHIFT           (4U)
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C1_SEL_MASK            (0xFU << BUS_GRF_GPIO4C_IOMUX_L_GPIO4C1_SEL_SHIFT)           /* 0x000000F0 */
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C2_SEL_SHIFT           (8U)
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C2_SEL_MASK            (0xFU << BUS_GRF_GPIO4C_IOMUX_L_GPIO4C2_SEL_SHIFT)           /* 0x00000F00 */
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C3_SEL_SHIFT           (12U)
#define BUS_GRF_GPIO4C_IOMUX_L_GPIO4C3_SEL_MASK            (0xFU << BUS_GRF_GPIO4C_IOMUX_L_GPIO4C3_SEL_SHIFT)           /* 0x0000F000 */
/* GPIO4C_IOMUX_H */
#define BUS_GRF_GPIO4C_IOMUX_H_OFFSET                      (0x74)
#define BUS_GRF_GPIO4C_IOMUX_H_GPIO4C4_SEL_SHIFT           (0U)
#define BUS_GRF_GPIO4C_IOMUX_H_GPIO4C4_SEL_MASK            (0xFU << BUS_GRF_GPIO4C_IOMUX_H_GPIO4C4_SEL_SHIFT)           /* 0x0000000F */
/* GPIO1A_P */
#define BUS_GRF_GPIO1A_P_OFFSET                            (0x80)
#define BUS_GRF_GPIO1A_P_GPIO1A0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO1A_P_GPIO1A0_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO1A_P_GPIO1A1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO1A_P_GPIO1A1_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO1A_P_GPIO1A2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO1A_P_GPIO1A2_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO1A_P_GPIO1A3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO1A_P_GPIO1A3_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO1A_P_GPIO1A4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO1A_P_GPIO1A4_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO1A_P_GPIO1A5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO1A_P_GPIO1A5_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO1A_P_GPIO1A6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO1A_P_GPIO1A6_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO1A_P_GPIO1A7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO1A_P_GPIO1A7_P_MASK                    (0x3U << BUS_GRF_GPIO1A_P_GPIO1A7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO1B_P */
#define BUS_GRF_GPIO1B_P_OFFSET                            (0x84)
#define BUS_GRF_GPIO1B_P_GPIO1B0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO1B_P_GPIO1B0_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO1B_P_GPIO1B1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO1B_P_GPIO1B1_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO1B_P_GPIO1B2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO1B_P_GPIO1B2_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO1B_P_GPIO1B3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO1B_P_GPIO1B3_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO1B_P_GPIO1B4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO1B_P_GPIO1B4_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO1B_P_GPIO1B5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO1B_P_GPIO1B5_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO1B_P_GPIO1B6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO1B_P_GPIO1B6_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO1B_P_GPIO1B7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO1B_P_GPIO1B7_P_MASK                    (0x3U << BUS_GRF_GPIO1B_P_GPIO1B7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO2A_P */
#define BUS_GRF_GPIO2A_P_OFFSET                            (0x90)
#define BUS_GRF_GPIO2A_P_GPIO2A0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO2A_P_GPIO2A0_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2A_P_GPIO2A1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO2A_P_GPIO2A1_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2A_P_GPIO2A2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO2A_P_GPIO2A2_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2A_P_GPIO2A3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO2A_P_GPIO2A3_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2A_P_GPIO2A4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO2A_P_GPIO2A4_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2A_P_GPIO2A5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO2A_P_GPIO2A5_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2A_P_GPIO2A6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO2A_P_GPIO2A6_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2A_P_GPIO2A7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO2A_P_GPIO2A7_P_MASK                    (0x3U << BUS_GRF_GPIO2A_P_GPIO2A7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO2B_P */
#define BUS_GRF_GPIO2B_P_OFFSET                            (0x94)
#define BUS_GRF_GPIO2B_P_GPIO2B0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO2B_P_GPIO2B0_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2B_P_GPIO2B1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO2B_P_GPIO2B1_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2B_P_GPIO2B2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO2B_P_GPIO2B2_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2B_P_GPIO2B3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO2B_P_GPIO2B3_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2B_P_GPIO2B4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO2B_P_GPIO2B4_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2B_P_GPIO2B5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO2B_P_GPIO2B5_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2B_P_GPIO2B6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO2B_P_GPIO2B6_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2B_P_GPIO2B7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO2B_P_GPIO2B7_P_MASK                    (0x3U << BUS_GRF_GPIO2B_P_GPIO2B7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO2C_P */
#define BUS_GRF_GPIO2C_P_OFFSET                            (0x98)
#define BUS_GRF_GPIO2C_P_GPIO2C0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO2C_P_GPIO2C0_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2C_P_GPIO2C1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO2C_P_GPIO2C1_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2C_P_GPIO2C2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO2C_P_GPIO2C2_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2C_P_GPIO2C3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO2C_P_GPIO2C3_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2C_P_GPIO2C4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO2C_P_GPIO2C4_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2C_P_GPIO2C5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO2C_P_GPIO2C5_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2C_P_GPIO2C6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO2C_P_GPIO2C6_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2C_P_GPIO2C7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO2C_P_GPIO2C7_P_MASK                    (0x3U << BUS_GRF_GPIO2C_P_GPIO2C7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO2D_P */
#define BUS_GRF_GPIO2D_P_OFFSET                            (0x9C)
#define BUS_GRF_GPIO2D_P_GPIO2D0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO2D_P_GPIO2D0_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2D_P_GPIO2D1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO2D_P_GPIO2D1_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2D_P_GPIO2D2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO2D_P_GPIO2D2_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2D_P_GPIO2D3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO2D_P_GPIO2D3_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2D_P_GPIO2D4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO2D_P_GPIO2D4_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2D_P_GPIO2D5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO2D_P_GPIO2D5_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2D_P_GPIO2D6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO2D_P_GPIO2D6_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2D_P_GPIO2D7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO2D_P_GPIO2D7_P_MASK                    (0x3U << BUS_GRF_GPIO2D_P_GPIO2D7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO3A_P */
#define BUS_GRF_GPIO3A_P_OFFSET                            (0xA0)
#define BUS_GRF_GPIO3A_P_GPIO3A0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO3A_P_GPIO3A0_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3A_P_GPIO3A1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO3A_P_GPIO3A1_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3A_P_GPIO3A2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO3A_P_GPIO3A2_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3A_P_GPIO3A3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO3A_P_GPIO3A3_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3A_P_GPIO3A4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO3A_P_GPIO3A4_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3A_P_GPIO3A5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO3A_P_GPIO3A5_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3A_P_GPIO3A6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO3A_P_GPIO3A6_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3A_P_GPIO3A7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO3A_P_GPIO3A7_P_MASK                    (0x3U << BUS_GRF_GPIO3A_P_GPIO3A7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO3B_P */
#define BUS_GRF_GPIO3B_P_OFFSET                            (0xA4)
#define BUS_GRF_GPIO3B_P_GPIO3B0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO3B_P_GPIO3B0_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3B_P_GPIO3B1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO3B_P_GPIO3B1_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3B_P_GPIO3B2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO3B_P_GPIO3B2_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3B_P_GPIO3B3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO3B_P_GPIO3B3_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3B_P_GPIO3B4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO3B_P_GPIO3B4_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3B_P_GPIO3B5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO3B_P_GPIO3B5_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3B_P_GPIO3B6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO3B_P_GPIO3B6_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3B_P_GPIO3B7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO3B_P_GPIO3B7_P_MASK                    (0x3U << BUS_GRF_GPIO3B_P_GPIO3B7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO3C_P */
#define BUS_GRF_GPIO3C_P_OFFSET                            (0xA8)
#define BUS_GRF_GPIO3C_P_GPIO3C0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO3C_P_GPIO3C0_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3C_P_GPIO3C1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO3C_P_GPIO3C1_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3C_P_GPIO3C2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO3C_P_GPIO3C2_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3C_P_GPIO3C3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO3C_P_GPIO3C3_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3C_P_GPIO3C4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO3C_P_GPIO3C4_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3C_P_GPIO3C5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO3C_P_GPIO3C5_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3C_P_GPIO3C6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO3C_P_GPIO3C6_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3C_P_GPIO3C7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO3C_P_GPIO3C7_P_MASK                    (0x3U << BUS_GRF_GPIO3C_P_GPIO3C7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO3D_P */
#define BUS_GRF_GPIO3D_P_OFFSET                            (0xAC)
#define BUS_GRF_GPIO3D_P_GPIO3D0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO3D_P_GPIO3D0_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3D_P_GPIO3D1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO3D_P_GPIO3D1_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3D_P_GPIO3D2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO3D_P_GPIO3D2_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3D_P_GPIO3D3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO3D_P_GPIO3D3_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3D_P_GPIO3D4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO3D_P_GPIO3D4_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3D_P_GPIO3D5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO3D_P_GPIO3D5_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3D_P_GPIO3D6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO3D_P_GPIO3D6_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3D_P_GPIO3D7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO3D_P_GPIO3D7_P_MASK                    (0x3U << BUS_GRF_GPIO3D_P_GPIO3D7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO4A_P */
#define BUS_GRF_GPIO4A_P_OFFSET                            (0xB0)
#define BUS_GRF_GPIO4A_P_GPIO4A0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO4A_P_GPIO4A0_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO4A_P_GPIO4A1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO4A_P_GPIO4A1_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO4A_P_GPIO4A2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO4A_P_GPIO4A2_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO4A_P_GPIO4A3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO4A_P_GPIO4A3_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO4A_P_GPIO4A4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO4A_P_GPIO4A4_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO4A_P_GPIO4A5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO4A_P_GPIO4A5_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO4A_P_GPIO4A6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO4A_P_GPIO4A6_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO4A_P_GPIO4A7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO4A_P_GPIO4A7_P_MASK                    (0x3U << BUS_GRF_GPIO4A_P_GPIO4A7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO4B_P */
#define BUS_GRF_GPIO4B_P_OFFSET                            (0xB4)
#define BUS_GRF_GPIO4B_P_GPIO4B0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO4B_P_GPIO4B0_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO4B_P_GPIO4B1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO4B_P_GPIO4B1_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO4B_P_GPIO4B2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO4B_P_GPIO4B2_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO4B_P_GPIO4B3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO4B_P_GPIO4B3_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO4B_P_GPIO4B4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO4B_P_GPIO4B4_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO4B_P_GPIO4B5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO4B_P_GPIO4B5_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO4B_P_GPIO4B6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO4B_P_GPIO4B6_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO4B_P_GPIO4B7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO4B_P_GPIO4B7_P_MASK                    (0x3U << BUS_GRF_GPIO4B_P_GPIO4B7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO4C_P */
#define BUS_GRF_GPIO4C_P_OFFSET                            (0xB8)
#define BUS_GRF_GPIO4C_P_GPIO4C0_P_SHIFT                   (0U)
#define BUS_GRF_GPIO4C_P_GPIO4C0_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C0_P_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO4C_P_GPIO4C1_P_SHIFT                   (2U)
#define BUS_GRF_GPIO4C_P_GPIO4C1_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C1_P_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO4C_P_GPIO4C2_P_SHIFT                   (4U)
#define BUS_GRF_GPIO4C_P_GPIO4C2_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C2_P_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO4C_P_GPIO4C3_P_SHIFT                   (6U)
#define BUS_GRF_GPIO4C_P_GPIO4C3_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C3_P_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO4C_P_GPIO4C4_P_SHIFT                   (8U)
#define BUS_GRF_GPIO4C_P_GPIO4C4_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C4_P_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO4C_P_GPIO4C5_P_SHIFT                   (10U)
#define BUS_GRF_GPIO4C_P_GPIO4C5_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C5_P_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO4C_P_GPIO4C6_P_SHIFT                   (12U)
#define BUS_GRF_GPIO4C_P_GPIO4C6_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C6_P_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO4C_P_GPIO4C7_P_SHIFT                   (14U)
#define BUS_GRF_GPIO4C_P_GPIO4C7_P_MASK                    (0x3U << BUS_GRF_GPIO4C_P_GPIO4C7_P_SHIFT)                   /* 0x0000C000 */
/* GPIO1A_SR */
#define BUS_GRF_GPIO1A_SR_OFFSET                           (0xC0)
#define BUS_GRF_GPIO1A_SR_GPIO1A0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO1A_SR_GPIO1A0_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO1A_SR_GPIO1A1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO1A_SR_GPIO1A1_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO1A_SR_GPIO1A2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO1A_SR_GPIO1A2_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO1A_SR_GPIO1A3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO1A_SR_GPIO1A3_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO1A_SR_GPIO1A4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO1A_SR_GPIO1A4_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO1A_SR_GPIO1A5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO1A_SR_GPIO1A5_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO1A_SR_GPIO1A6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO1A_SR_GPIO1A6_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO1A_SR_GPIO1A7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO1A_SR_GPIO1A7_SR_MASK                  (0x1U << BUS_GRF_GPIO1A_SR_GPIO1A7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO1B_SR */
#define BUS_GRF_GPIO1B_SR_OFFSET                           (0xC4)
#define BUS_GRF_GPIO1B_SR_GPIO1B0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO1B_SR_GPIO1B0_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO1B_SR_GPIO1B1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO1B_SR_GPIO1B1_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO1B_SR_GPIO1B2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO1B_SR_GPIO1B2_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO1B_SR_GPIO1B3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO1B_SR_GPIO1B3_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO1B_SR_GPIO1B4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO1B_SR_GPIO1B4_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO1B_SR_GPIO1B5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO1B_SR_GPIO1B5_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO1B_SR_GPIO1B6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO1B_SR_GPIO1B6_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO1B_SR_GPIO1B7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO1B_SR_GPIO1B7_SR_MASK                  (0x1U << BUS_GRF_GPIO1B_SR_GPIO1B7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO2A_SR */
#define BUS_GRF_GPIO2A_SR_OFFSET                           (0xD0)
#define BUS_GRF_GPIO2A_SR_GPIO2A0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO2A_SR_GPIO2A0_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO2A_SR_GPIO2A1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO2A_SR_GPIO2A1_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO2A_SR_GPIO2A2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO2A_SR_GPIO2A2_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO2A_SR_GPIO2A3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO2A_SR_GPIO2A3_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO2A_SR_GPIO2A4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO2A_SR_GPIO2A4_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO2A_SR_GPIO2A5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO2A_SR_GPIO2A5_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO2A_SR_GPIO2A6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO2A_SR_GPIO2A6_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO2A_SR_GPIO2A7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO2A_SR_GPIO2A7_SR_MASK                  (0x1U << BUS_GRF_GPIO2A_SR_GPIO2A7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO2B_SR */
#define BUS_GRF_GPIO2B_SR_OFFSET                           (0xD4)
#define BUS_GRF_GPIO2B_SR_GPIO2B0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO2B_SR_GPIO2B0_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO2B_SR_GPIO2B1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO2B_SR_GPIO2B1_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO2B_SR_GPIO2B2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO2B_SR_GPIO2B2_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO2B_SR_GPIO2B3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO2B_SR_GPIO2B3_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO2B_SR_GPIO2B4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO2B_SR_GPIO2B4_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO2B_SR_GPIO2B5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO2B_SR_GPIO2B5_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO2B_SR_GPIO2B6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO2B_SR_GPIO2B6_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO2B_SR_GPIO2B7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO2B_SR_GPIO2B7_SR_MASK                  (0x1U << BUS_GRF_GPIO2B_SR_GPIO2B7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO2C_SR */
#define BUS_GRF_GPIO2C_SR_OFFSET                           (0xD8)
#define BUS_GRF_GPIO2C_SR_GPIO2C0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO2C_SR_GPIO2C0_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO2C_SR_GPIO2C1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO2C_SR_GPIO2C1_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO2C_SR_GPIO2C2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO2C_SR_GPIO2C2_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO2C_SR_GPIO2C3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO2C_SR_GPIO2C3_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO2C_SR_GPIO2C4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO2C_SR_GPIO2C4_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO2C_SR_GPIO2C5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO2C_SR_GPIO2C5_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO2C_SR_GPIO2C6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO2C_SR_GPIO2C6_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO2C_SR_GPIO2C7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO2C_SR_GPIO2C7_SR_MASK                  (0x1U << BUS_GRF_GPIO2C_SR_GPIO2C7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO2D_SR */
#define BUS_GRF_GPIO2D_SR_OFFSET                           (0xDC)
#define BUS_GRF_GPIO2D_SR_GPIO2D0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO2D_SR_GPIO2D0_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO2D_SR_GPIO2D1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO2D_SR_GPIO2D1_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO2D_SR_GPIO2D2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO2D_SR_GPIO2D2_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO2D_SR_GPIO2D3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO2D_SR_GPIO2D3_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO2D_SR_GPIO2D4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO2D_SR_GPIO2D4_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO2D_SR_GPIO2D5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO2D_SR_GPIO2D5_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO2D_SR_GPIO2D6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO2D_SR_GPIO2D6_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO2D_SR_GPIO2D7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO2D_SR_GPIO2D7_SR_MASK                  (0x1U << BUS_GRF_GPIO2D_SR_GPIO2D7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO3A_SR */
#define BUS_GRF_GPIO3A_SR_OFFSET                           (0xE0)
#define BUS_GRF_GPIO3A_SR_GPIO3A0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO3A_SR_GPIO3A0_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO3A_SR_GPIO3A1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO3A_SR_GPIO3A1_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO3A_SR_GPIO3A2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO3A_SR_GPIO3A2_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO3A_SR_GPIO3A3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO3A_SR_GPIO3A3_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO3A_SR_GPIO3A4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO3A_SR_GPIO3A4_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO3A_SR_GPIO3A5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO3A_SR_GPIO3A5_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO3A_SR_GPIO3A6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO3A_SR_GPIO3A6_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO3A_SR_GPIO3A7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO3A_SR_GPIO3A7_SR_MASK                  (0x1U << BUS_GRF_GPIO3A_SR_GPIO3A7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO3B_SR */
#define BUS_GRF_GPIO3B_SR_OFFSET                           (0xE4)
#define BUS_GRF_GPIO3B_SR_GPIO3B0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO3B_SR_GPIO3B0_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO3B_SR_GPIO3B1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO3B_SR_GPIO3B1_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO3B_SR_GPIO3B2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO3B_SR_GPIO3B2_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO3B_SR_GPIO3B3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO3B_SR_GPIO3B3_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO3B_SR_GPIO3B4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO3B_SR_GPIO3B4_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO3B_SR_GPIO3B5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO3B_SR_GPIO3B5_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO3B_SR_GPIO3B6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO3B_SR_GPIO3B6_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO3B_SR_GPIO3B7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO3B_SR_GPIO3B7_SR_MASK                  (0x1U << BUS_GRF_GPIO3B_SR_GPIO3B7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO3C_SR */
#define BUS_GRF_GPIO3C_SR_OFFSET                           (0xE8)
#define BUS_GRF_GPIO3C_SR_GPIO3C0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO3C_SR_GPIO3C0_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO3C_SR_GPIO3C1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO3C_SR_GPIO3C1_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO3C_SR_GPIO3C2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO3C_SR_GPIO3C2_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO3C_SR_GPIO3C3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO3C_SR_GPIO3C3_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO3C_SR_GPIO3C4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO3C_SR_GPIO3C4_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO3C_SR_GPIO3C5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO3C_SR_GPIO3C5_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO3C_SR_GPIO3C6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO3C_SR_GPIO3C6_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO3C_SR_GPIO3C7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO3C_SR_GPIO3C7_SR_MASK                  (0x1U << BUS_GRF_GPIO3C_SR_GPIO3C7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO3D_SR */
#define BUS_GRF_GPIO3D_SR_OFFSET                           (0xEC)
#define BUS_GRF_GPIO3D_SR_GPIO3D0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO3D_SR_GPIO3D0_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO3D_SR_GPIO3D1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO3D_SR_GPIO3D1_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO3D_SR_GPIO3D2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO3D_SR_GPIO3D2_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO3D_SR_GPIO3D3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO3D_SR_GPIO3D3_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO3D_SR_GPIO3D4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO3D_SR_GPIO3D4_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO3D_SR_GPIO3D5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO3D_SR_GPIO3D5_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO3D_SR_GPIO3D6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO3D_SR_GPIO3D6_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO3D_SR_GPIO3D7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO3D_SR_GPIO3D7_SR_MASK                  (0x1U << BUS_GRF_GPIO3D_SR_GPIO3D7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO4A_SR */
#define BUS_GRF_GPIO4A_SR_OFFSET                           (0xF0)
#define BUS_GRF_GPIO4A_SR_GPIO4A0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO4A_SR_GPIO4A0_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO4A_SR_GPIO4A1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO4A_SR_GPIO4A1_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO4A_SR_GPIO4A2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO4A_SR_GPIO4A2_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO4A_SR_GPIO4A3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO4A_SR_GPIO4A3_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO4A_SR_GPIO4A4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO4A_SR_GPIO4A4_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO4A_SR_GPIO4A5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO4A_SR_GPIO4A5_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO4A_SR_GPIO4A6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO4A_SR_GPIO4A6_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO4A_SR_GPIO4A7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO4A_SR_GPIO4A7_SR_MASK                  (0x1U << BUS_GRF_GPIO4A_SR_GPIO4A7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO4B_SR */
#define BUS_GRF_GPIO4B_SR_OFFSET                           (0xF4)
#define BUS_GRF_GPIO4B_SR_GPIO4B0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO4B_SR_GPIO4B0_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO4B_SR_GPIO4B1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO4B_SR_GPIO4B1_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO4B_SR_GPIO4B2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO4B_SR_GPIO4B2_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO4B_SR_GPIO4B3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO4B_SR_GPIO4B3_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO4B_SR_GPIO4B4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO4B_SR_GPIO4B4_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO4B_SR_GPIO4B5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO4B_SR_GPIO4B5_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO4B_SR_GPIO4B6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO4B_SR_GPIO4B6_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO4B_SR_GPIO4B7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO4B_SR_GPIO4B7_SR_MASK                  (0x1U << BUS_GRF_GPIO4B_SR_GPIO4B7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO4C_SR */
#define BUS_GRF_GPIO4C_SR_OFFSET                           (0xF8)
#define BUS_GRF_GPIO4C_SR_GPIO4C0_SR_SHIFT                 (0U)
#define BUS_GRF_GPIO4C_SR_GPIO4C0_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C0_SR_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_GPIO4C_SR_GPIO4C1_SR_SHIFT                 (1U)
#define BUS_GRF_GPIO4C_SR_GPIO4C1_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C1_SR_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_GPIO4C_SR_GPIO4C2_SR_SHIFT                 (2U)
#define BUS_GRF_GPIO4C_SR_GPIO4C2_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C2_SR_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_GPIO4C_SR_GPIO4C3_SR_SHIFT                 (3U)
#define BUS_GRF_GPIO4C_SR_GPIO4C3_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C3_SR_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_GPIO4C_SR_GPIO4C4_SR_SHIFT                 (4U)
#define BUS_GRF_GPIO4C_SR_GPIO4C4_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C4_SR_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_GPIO4C_SR_GPIO4C5_SR_SHIFT                 (5U)
#define BUS_GRF_GPIO4C_SR_GPIO4C5_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C5_SR_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_GPIO4C_SR_GPIO4C6_SR_SHIFT                 (6U)
#define BUS_GRF_GPIO4C_SR_GPIO4C6_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C6_SR_SHIFT)                 /* 0x00000040 */
#define BUS_GRF_GPIO4C_SR_GPIO4C7_SR_SHIFT                 (7U)
#define BUS_GRF_GPIO4C_SR_GPIO4C7_SR_MASK                  (0x1U << BUS_GRF_GPIO4C_SR_GPIO4C7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO1A_SMT */
#define BUS_GRF_GPIO1A_SMT_OFFSET                          (0x100)
#define BUS_GRF_GPIO1A_SMT_GPIO1A0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A0_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A1_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A2_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A3_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A4_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A5_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A6_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO1A_SMT_GPIO1A7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO1A_SMT_GPIO1A7_SMT_MASK                (0x1U << BUS_GRF_GPIO1A_SMT_GPIO1A7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO1B_SMT */
#define BUS_GRF_GPIO1B_SMT_OFFSET                          (0x104)
#define BUS_GRF_GPIO1B_SMT_GPIO1B0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B0_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B1_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B2_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B3_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B4_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B5_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B6_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO1B_SMT_GPIO1B7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO1B_SMT_GPIO1B7_SMT_MASK                (0x1U << BUS_GRF_GPIO1B_SMT_GPIO1B7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO1C_SMT */
#define BUS_GRF_GPIO1C_SMT_OFFSET                          (0x108)
#define BUS_GRF_GPIO1C_SMT_GPIO1C0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C0_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C1_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C2_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C3_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C4_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C5_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C6_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO1C_SMT_GPIO1C7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO1C_SMT_GPIO1C7_SMT_MASK                (0x1U << BUS_GRF_GPIO1C_SMT_GPIO1C7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO1D_SMT */
#define BUS_GRF_GPIO1D_SMT_OFFSET                          (0x10C)
#define BUS_GRF_GPIO1D_SMT_GPIO1D0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D0_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D1_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D2_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D3_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D4_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D5_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D6_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO1D_SMT_GPIO1D7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO1D_SMT_GPIO1D7_SMT_MASK                (0x1U << BUS_GRF_GPIO1D_SMT_GPIO1D7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO2A_SMT */
#define BUS_GRF_GPIO2A_SMT_OFFSET                          (0x110)
#define BUS_GRF_GPIO2A_SMT_GPIO2A0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A0_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A1_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A2_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A3_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A4_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A5_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A6_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO2A_SMT_GPIO2A7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO2A_SMT_GPIO2A7_SMT_MASK                (0x1U << BUS_GRF_GPIO2A_SMT_GPIO2A7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO2B_SMT */
#define BUS_GRF_GPIO2B_SMT_OFFSET                          (0x114)
#define BUS_GRF_GPIO2B_SMT_GPIO2B0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B0_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B1_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B2_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B3_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B4_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B5_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B6_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO2B_SMT_GPIO2B7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO2B_SMT_GPIO2B7_SMT_MASK                (0x1U << BUS_GRF_GPIO2B_SMT_GPIO2B7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO2C_SMT */
#define BUS_GRF_GPIO2C_SMT_OFFSET                          (0x118)
#define BUS_GRF_GPIO2C_SMT_GPIO2C0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C0_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C1_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C2_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C3_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C4_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C5_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C6_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO2C_SMT_GPIO2C7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO2C_SMT_GPIO2C7_SMT_MASK                (0x1U << BUS_GRF_GPIO2C_SMT_GPIO2C7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO2D_SMT */
#define BUS_GRF_GPIO2D_SMT_OFFSET                          (0x11C)
#define BUS_GRF_GPIO2D_SMT_GPIO2D0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D0_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D1_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D2_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D3_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D4_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D5_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D6_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO2D_SMT_GPIO2D7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO2D_SMT_GPIO2D7_SMT_MASK                (0x1U << BUS_GRF_GPIO2D_SMT_GPIO2D7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO3A_SMT */
#define BUS_GRF_GPIO3A_SMT_OFFSET                          (0x120)
#define BUS_GRF_GPIO3A_SMT_GPIO3A0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A0_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A1_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A2_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A3_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A4_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A5_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A6_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO3A_SMT_GPIO3A7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO3A_SMT_GPIO3A7_SMT_MASK                (0x1U << BUS_GRF_GPIO3A_SMT_GPIO3A7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO3B_SMT */
#define BUS_GRF_GPIO3B_SMT_OFFSET                          (0x124)
#define BUS_GRF_GPIO3B_SMT_GPIO3B0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B0_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B1_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B2_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B3_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B4_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B5_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B6_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO3B_SMT_GPIO3B7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO3B_SMT_GPIO3B7_SMT_MASK                (0x1U << BUS_GRF_GPIO3B_SMT_GPIO3B7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO3C_SMT */
#define BUS_GRF_GPIO3C_SMT_OFFSET                          (0x128)
#define BUS_GRF_GPIO3C_SMT_GPIO3C0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C0_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C1_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C2_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C3_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C4_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C5_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C6_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO3C_SMT_GPIO3C7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO3C_SMT_GPIO3C7_SMT_MASK                (0x1U << BUS_GRF_GPIO3C_SMT_GPIO3C7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO3D_SMT */
#define BUS_GRF_GPIO3D_SMT_OFFSET                          (0x12C)
#define BUS_GRF_GPIO3D_SMT_GPIO3D0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D0_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D1_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D2_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D3_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D4_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D5_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D6_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO3D_SMT_GPIO3D7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO3D_SMT_GPIO3D7_SMT_MASK                (0x1U << BUS_GRF_GPIO3D_SMT_GPIO3D7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO4A_SMT */
#define BUS_GRF_GPIO4A_SMT_OFFSET                          (0x130)
#define BUS_GRF_GPIO4A_SMT_GPIO4A0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A0_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A1_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A2_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A3_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A4_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A5_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A6_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO4A_SMT_GPIO4A7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO4A_SMT_GPIO4A7_SMT_MASK                (0x1U << BUS_GRF_GPIO4A_SMT_GPIO4A7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO4B_SMT */
#define BUS_GRF_GPIO4B_SMT_OFFSET                          (0x134)
#define BUS_GRF_GPIO4B_SMT_GPIO4B0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B0_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B1_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B2_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B3_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B4_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B5_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B6_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO4B_SMT_GPIO4B7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO4B_SMT_GPIO4B7_SMT_MASK                (0x1U << BUS_GRF_GPIO4B_SMT_GPIO4B7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO4C_SMT */
#define BUS_GRF_GPIO4C_SMT_OFFSET                          (0x138)
#define BUS_GRF_GPIO4C_SMT_GPIO4C0_SMT_SHIFT               (0U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C0_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C0_SMT_SHIFT)               /* 0x00000001 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C1_SMT_SHIFT               (1U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C1_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C1_SMT_SHIFT)               /* 0x00000002 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C2_SMT_SHIFT               (2U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C2_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C2_SMT_SHIFT)               /* 0x00000004 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C3_SMT_SHIFT               (3U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C3_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C3_SMT_SHIFT)               /* 0x00000008 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C4_SMT_SHIFT               (4U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C4_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C4_SMT_SHIFT)               /* 0x00000010 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C5_SMT_SHIFT               (5U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C5_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C5_SMT_SHIFT)               /* 0x00000020 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C6_SMT_SHIFT               (6U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C6_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C6_SMT_SHIFT)               /* 0x00000040 */
#define BUS_GRF_GPIO4C_SMT_GPIO4C7_SMT_SHIFT               (7U)
#define BUS_GRF_GPIO4C_SMT_GPIO4C7_SMT_MASK                (0x1U << BUS_GRF_GPIO4C_SMT_GPIO4C7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO1A_E */
#define BUS_GRF_GPIO1A_E_OFFSET                            (0x140)
#define BUS_GRF_GPIO1A_E_GPIO1A0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO1A_E_GPIO1A0_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO1A_E_GPIO1A1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO1A_E_GPIO1A1_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO1A_E_GPIO1A2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO1A_E_GPIO1A2_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO1A_E_GPIO1A3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO1A_E_GPIO1A3_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO1A_E_GPIO1A4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO1A_E_GPIO1A4_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO1A_E_GPIO1A5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO1A_E_GPIO1A5_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO1A_E_GPIO1A6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO1A_E_GPIO1A6_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO1A_E_GPIO1A7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO1A_E_GPIO1A7_E_MASK                    (0x3U << BUS_GRF_GPIO1A_E_GPIO1A7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO1B_E */
#define BUS_GRF_GPIO1B_E_OFFSET                            (0x144)
#define BUS_GRF_GPIO1B_E_GPIO1B0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO1B_E_GPIO1B0_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO1B_E_GPIO1B1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO1B_E_GPIO1B1_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO1B_E_GPIO1B2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO1B_E_GPIO1B2_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO1B_E_GPIO1B3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO1B_E_GPIO1B3_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO1B_E_GPIO1B4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO1B_E_GPIO1B4_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO1B_E_GPIO1B5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO1B_E_GPIO1B5_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO1B_E_GPIO1B6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO1B_E_GPIO1B6_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO1B_E_GPIO1B7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO1B_E_GPIO1B7_E_MASK                    (0x3U << BUS_GRF_GPIO1B_E_GPIO1B7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO2A_E */
#define BUS_GRF_GPIO2A_E_OFFSET                            (0x150)
#define BUS_GRF_GPIO2A_E_GPIO2A0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO2A_E_GPIO2A0_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2A_E_GPIO2A1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO2A_E_GPIO2A1_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2A_E_GPIO2A2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO2A_E_GPIO2A2_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2A_E_GPIO2A3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO2A_E_GPIO2A3_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2A_E_GPIO2A4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO2A_E_GPIO2A4_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2A_E_GPIO2A5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO2A_E_GPIO2A5_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2A_E_GPIO2A6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO2A_E_GPIO2A6_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2A_E_GPIO2A7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO2A_E_GPIO2A7_E_MASK                    (0x3U << BUS_GRF_GPIO2A_E_GPIO2A7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO2B_E */
#define BUS_GRF_GPIO2B_E_OFFSET                            (0x154)
#define BUS_GRF_GPIO2B_E_GPIO2B0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO2B_E_GPIO2B0_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2B_E_GPIO2B1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO2B_E_GPIO2B1_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2B_E_GPIO2B2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO2B_E_GPIO2B2_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2B_E_GPIO2B3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO2B_E_GPIO2B3_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2B_E_GPIO2B4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO2B_E_GPIO2B4_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2B_E_GPIO2B5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO2B_E_GPIO2B5_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2B_E_GPIO2B6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO2B_E_GPIO2B6_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2B_E_GPIO2B7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO2B_E_GPIO2B7_E_MASK                    (0x3U << BUS_GRF_GPIO2B_E_GPIO2B7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO2C_E */
#define BUS_GRF_GPIO2C_E_OFFSET                            (0x158)
#define BUS_GRF_GPIO2C_E_GPIO2C0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO2C_E_GPIO2C0_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2C_E_GPIO2C1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO2C_E_GPIO2C1_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2C_E_GPIO2C2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO2C_E_GPIO2C2_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2C_E_GPIO2C3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO2C_E_GPIO2C3_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2C_E_GPIO2C4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO2C_E_GPIO2C4_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2C_E_GPIO2C5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO2C_E_GPIO2C5_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2C_E_GPIO2C6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO2C_E_GPIO2C6_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2C_E_GPIO2C7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO2C_E_GPIO2C7_E_MASK                    (0x3U << BUS_GRF_GPIO2C_E_GPIO2C7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO2D_E */
#define BUS_GRF_GPIO2D_E_OFFSET                            (0x15C)
#define BUS_GRF_GPIO2D_E_GPIO2D0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO2D_E_GPIO2D0_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO2D_E_GPIO2D1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO2D_E_GPIO2D1_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO2D_E_GPIO2D2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO2D_E_GPIO2D2_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO2D_E_GPIO2D3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO2D_E_GPIO2D3_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO2D_E_GPIO2D4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO2D_E_GPIO2D4_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO2D_E_GPIO2D5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO2D_E_GPIO2D5_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO2D_E_GPIO2D6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO2D_E_GPIO2D6_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO2D_E_GPIO2D7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO2D_E_GPIO2D7_E_MASK                    (0x3U << BUS_GRF_GPIO2D_E_GPIO2D7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO3A_E */
#define BUS_GRF_GPIO3A_E_OFFSET                            (0x160)
#define BUS_GRF_GPIO3A_E_GPIO3A0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO3A_E_GPIO3A0_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3A_E_GPIO3A1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO3A_E_GPIO3A1_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3A_E_GPIO3A2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO3A_E_GPIO3A2_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3A_E_GPIO3A3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO3A_E_GPIO3A3_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3A_E_GPIO3A4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO3A_E_GPIO3A4_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3A_E_GPIO3A5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO3A_E_GPIO3A5_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3A_E_GPIO3A6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO3A_E_GPIO3A6_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3A_E_GPIO3A7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO3A_E_GPIO3A7_E_MASK                    (0x3U << BUS_GRF_GPIO3A_E_GPIO3A7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO3B_E */
#define BUS_GRF_GPIO3B_E_OFFSET                            (0x164)
#define BUS_GRF_GPIO3B_E_GPIO3B0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO3B_E_GPIO3B0_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3B_E_GPIO3B1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO3B_E_GPIO3B1_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3B_E_GPIO3B2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO3B_E_GPIO3B2_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3B_E_GPIO3B3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO3B_E_GPIO3B3_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3B_E_GPIO3B4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO3B_E_GPIO3B4_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3B_E_GPIO3B5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO3B_E_GPIO3B5_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3B_E_GPIO3B6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO3B_E_GPIO3B6_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3B_E_GPIO3B7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO3B_E_GPIO3B7_E_MASK                    (0x3U << BUS_GRF_GPIO3B_E_GPIO3B7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO3C_E */
#define BUS_GRF_GPIO3C_E_OFFSET                            (0x168)
#define BUS_GRF_GPIO3C_E_GPIO3C0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO3C_E_GPIO3C0_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3C_E_GPIO3C1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO3C_E_GPIO3C1_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3C_E_GPIO3C2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO3C_E_GPIO3C2_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3C_E_GPIO3C3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO3C_E_GPIO3C3_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3C_E_GPIO3C4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO3C_E_GPIO3C4_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3C_E_GPIO3C5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO3C_E_GPIO3C5_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3C_E_GPIO3C6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO3C_E_GPIO3C6_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3C_E_GPIO3C7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO3C_E_GPIO3C7_E_MASK                    (0x3U << BUS_GRF_GPIO3C_E_GPIO3C7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO3D_E */
#define BUS_GRF_GPIO3D_E_OFFSET                            (0x16C)
#define BUS_GRF_GPIO3D_E_GPIO3D0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO3D_E_GPIO3D0_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO3D_E_GPIO3D1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO3D_E_GPIO3D1_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO3D_E_GPIO3D2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO3D_E_GPIO3D2_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO3D_E_GPIO3D3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO3D_E_GPIO3D3_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO3D_E_GPIO3D4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO3D_E_GPIO3D4_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO3D_E_GPIO3D5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO3D_E_GPIO3D5_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO3D_E_GPIO3D6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO3D_E_GPIO3D6_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO3D_E_GPIO3D7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO3D_E_GPIO3D7_E_MASK                    (0x3U << BUS_GRF_GPIO3D_E_GPIO3D7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO4A_E */
#define BUS_GRF_GPIO4A_E_OFFSET                            (0x170)
#define BUS_GRF_GPIO4A_E_GPIO4A0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO4A_E_GPIO4A0_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO4A_E_GPIO4A1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO4A_E_GPIO4A1_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO4A_E_GPIO4A2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO4A_E_GPIO4A2_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO4A_E_GPIO4A3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO4A_E_GPIO4A3_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO4A_E_GPIO4A4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO4A_E_GPIO4A4_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO4A_E_GPIO4A5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO4A_E_GPIO4A5_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO4A_E_GPIO4A6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO4A_E_GPIO4A6_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO4A_E_GPIO4A7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO4A_E_GPIO4A7_E_MASK                    (0x3U << BUS_GRF_GPIO4A_E_GPIO4A7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO4B_E */
#define BUS_GRF_GPIO4B_E_OFFSET                            (0x174)
#define BUS_GRF_GPIO4B_E_GPIO4B0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO4B_E_GPIO4B0_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO4B_E_GPIO4B1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO4B_E_GPIO4B1_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO4B_E_GPIO4B2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO4B_E_GPIO4B2_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO4B_E_GPIO4B3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO4B_E_GPIO4B3_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO4B_E_GPIO4B4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO4B_E_GPIO4B4_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO4B_E_GPIO4B5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO4B_E_GPIO4B5_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO4B_E_GPIO4B6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO4B_E_GPIO4B6_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO4B_E_GPIO4B7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO4B_E_GPIO4B7_E_MASK                    (0x3U << BUS_GRF_GPIO4B_E_GPIO4B7_E_SHIFT)                   /* 0x0000C000 */
/* GPIO4C_E */
#define BUS_GRF_GPIO4C_E_OFFSET                            (0x178)
#define BUS_GRF_GPIO4C_E_GPIO4C0_E_SHIFT                   (0U)
#define BUS_GRF_GPIO4C_E_GPIO4C0_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C0_E_SHIFT)                   /* 0x00000003 */
#define BUS_GRF_GPIO4C_E_GPIO4C1_E_SHIFT                   (2U)
#define BUS_GRF_GPIO4C_E_GPIO4C1_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C1_E_SHIFT)                   /* 0x0000000C */
#define BUS_GRF_GPIO4C_E_GPIO4C2_E_SHIFT                   (4U)
#define BUS_GRF_GPIO4C_E_GPIO4C2_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C2_E_SHIFT)                   /* 0x00000030 */
#define BUS_GRF_GPIO4C_E_GPIO4C3_E_SHIFT                   (6U)
#define BUS_GRF_GPIO4C_E_GPIO4C3_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C3_E_SHIFT)                   /* 0x000000C0 */
#define BUS_GRF_GPIO4C_E_GPIO4C4_E_SHIFT                   (8U)
#define BUS_GRF_GPIO4C_E_GPIO4C4_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C4_E_SHIFT)                   /* 0x00000300 */
#define BUS_GRF_GPIO4C_E_GPIO4C5_E_SHIFT                   (10U)
#define BUS_GRF_GPIO4C_E_GPIO4C5_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C5_E_SHIFT)                   /* 0x00000C00 */
#define BUS_GRF_GPIO4C_E_GPIO4C6_E_SHIFT                   (12U)
#define BUS_GRF_GPIO4C_E_GPIO4C6_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C6_E_SHIFT)                   /* 0x00003000 */
#define BUS_GRF_GPIO4C_E_GPIO4C7_E_SHIFT                   (14U)
#define BUS_GRF_GPIO4C_E_GPIO4C7_E_MASK                    (0x3U << BUS_GRF_GPIO4C_E_GPIO4C7_E_SHIFT)                   /* 0x0000C000 */
/* IOFUNC_CON0 */
#define BUS_GRF_IOFUNC_CON0_OFFSET                         (0x190)
#define BUS_GRF_IOFUNC_CON0_I2S0_TX_LRCK_SEL_SHIFT         (0U)
#define BUS_GRF_IOFUNC_CON0_I2S0_TX_LRCK_SEL_MASK          (0x3U << BUS_GRF_IOFUNC_CON0_I2S0_TX_LRCK_SEL_SHIFT)         /* 0x00000003 */
#define BUS_GRF_IOFUNC_CON0_I2S0_MCLK_SEL_SHIFT            (2U)
#define BUS_GRF_IOFUNC_CON0_I2S0_MCLK_SEL_MASK             (0x1U << BUS_GRF_IOFUNC_CON0_I2S0_MCLK_SEL_SHIFT)            /* 0x00000004 */
#define BUS_GRF_IOFUNC_CON0_I2C2_SEL_SHIFT                 (3U)
#define BUS_GRF_IOFUNC_CON0_I2C2_SEL_MASK                  (0x1U << BUS_GRF_IOFUNC_CON0_I2C2_SEL_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_IOFUNC_CON0_SPI2_SEL_SHIFT                 (4U)
#define BUS_GRF_IOFUNC_CON0_SPI2_SEL_MASK                  (0x1U << BUS_GRF_IOFUNC_CON0_SPI2_SEL_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_IOFUNC_CON0_SPI1_SEL_SHIFT                 (5U)
#define BUS_GRF_IOFUNC_CON0_SPI1_SEL_MASK                  (0x1U << BUS_GRF_IOFUNC_CON0_SPI1_SEL_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_IOFUNC_CON0_PCIEPHY_DEBUG_SEL_SHIFT        (6U)
#define BUS_GRF_IOFUNC_CON0_PCIEPHY_DEBUG_SEL_MASK         (0x3U << BUS_GRF_IOFUNC_CON0_PCIEPHY_DEBUG_SEL_SHIFT)        /* 0x000000C0 */
#define BUS_GRF_IOFUNC_CON0_I2S1_2CH_LRCK_SEL_SHIFT        (9U)
#define BUS_GRF_IOFUNC_CON0_I2S1_2CH_LRCK_SEL_MASK         (0x1U << BUS_GRF_IOFUNC_CON0_I2S1_2CH_LRCK_SEL_SHIFT)        /* 0x00000200 */
#define BUS_GRF_IOFUNC_CON0_UART1_SEL_SHIFT                (13U)
#define BUS_GRF_IOFUNC_CON0_UART1_SEL_MASK                 (0x1U << BUS_GRF_IOFUNC_CON0_UART1_SEL_SHIFT)                /* 0x00002000 */
#define BUS_GRF_IOFUNC_CON0_UART2_SEL_SHIFT                (14U)
#define BUS_GRF_IOFUNC_CON0_UART2_SEL_MASK                 (0x3U << BUS_GRF_IOFUNC_CON0_UART2_SEL_SHIFT)                /* 0x0000C000 */
/* SOC_CON0 */
#define BUS_GRF_SOC_CON0_OFFSET                            (0x400)
#define BUS_GRF_SOC_CON0_TSADC_TESTBIT_SHIFT               (0U)
#define BUS_GRF_SOC_CON0_TSADC_TESTBIT_MASK                (0xFFFFU << BUS_GRF_SOC_CON0_TSADC_TESTBIT_SHIFT)            /* 0x0000FFFF */
/* SOC_CON1 */
#define BUS_GRF_SOC_CON1_OFFSET                            (0x404)
#define BUS_GRF_SOC_CON1_TSADC_CHSEL_SHIFT                 (0U)
#define BUS_GRF_SOC_CON1_TSADC_CHSEL_MASK                  (0x3U << BUS_GRF_SOC_CON1_TSADC_CHSEL_SHIFT)                 /* 0x00000003 */
#define BUS_GRF_SOC_CON1_TSADC_TSEN_PD_0_SHIFT             (2U)
#define BUS_GRF_SOC_CON1_TSADC_TSEN_PD_0_MASK              (0x1U << BUS_GRF_SOC_CON1_TSADC_TSEN_PD_0_SHIFT)             /* 0x00000004 */
#define BUS_GRF_SOC_CON1_TSADC_DIG_BYPASS_SHIFT            (3U)
#define BUS_GRF_SOC_CON1_TSADC_DIG_BYPASS_MASK             (0x1U << BUS_GRF_SOC_CON1_TSADC_DIG_BYPASS_SHIFT)            /* 0x00000008 */
#define BUS_GRF_SOC_CON1_TSADC_CLK_SEL_SHIFT               (4U)
#define BUS_GRF_SOC_CON1_TSADC_CLK_SEL_MASK                (0x1U << BUS_GRF_SOC_CON1_TSADC_CLK_SEL_SHIFT)               /* 0x00000010 */
#define BUS_GRF_SOC_CON1_UART6_CTS_INV_SHIFT               (8U)
#define BUS_GRF_SOC_CON1_UART6_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON1_UART6_CTS_INV_SHIFT)               /* 0x00000100 */
#define BUS_GRF_SOC_CON1_UART6_RTS_INV_SHIFT               (9U)
#define BUS_GRF_SOC_CON1_UART6_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON1_UART6_RTS_INV_SHIFT)               /* 0x00000200 */
#define BUS_GRF_SOC_CON1_UART7_CTS_INV_SHIFT               (10U)
#define BUS_GRF_SOC_CON1_UART7_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON1_UART7_CTS_INV_SHIFT)               /* 0x00000400 */
#define BUS_GRF_SOC_CON1_UART7_RTS_INV_SHIFT               (11U)
#define BUS_GRF_SOC_CON1_UART7_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON1_UART7_RTS_INV_SHIFT)               /* 0x00000800 */
/* SOC_CON2 */
#define BUS_GRF_SOC_CON2_OFFSET                            (0x408)
#define BUS_GRF_SOC_CON2_UART1_CTS_INV_SHIFT               (2U)
#define BUS_GRF_SOC_CON2_UART1_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART1_CTS_INV_SHIFT)               /* 0x00000004 */
#define BUS_GRF_SOC_CON2_UART1_RTS_INV_SHIFT               (3U)
#define BUS_GRF_SOC_CON2_UART1_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART1_RTS_INV_SHIFT)               /* 0x00000008 */
#define BUS_GRF_SOC_CON2_UART2_CTS_INV_SHIFT               (4U)
#define BUS_GRF_SOC_CON2_UART2_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART2_CTS_INV_SHIFT)               /* 0x00000010 */
#define BUS_GRF_SOC_CON2_UART2_RTS_INV_SHIFT               (5U)
#define BUS_GRF_SOC_CON2_UART2_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART2_RTS_INV_SHIFT)               /* 0x00000020 */
#define BUS_GRF_SOC_CON2_UART3_CTS_INV_SHIFT               (6U)
#define BUS_GRF_SOC_CON2_UART3_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART3_CTS_INV_SHIFT)               /* 0x00000040 */
#define BUS_GRF_SOC_CON2_UART3_RTS_INV_SHIFT               (7U)
#define BUS_GRF_SOC_CON2_UART3_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART3_RTS_INV_SHIFT)               /* 0x00000080 */
#define BUS_GRF_SOC_CON2_UART4_CTS_INV_SHIFT               (8U)
#define BUS_GRF_SOC_CON2_UART4_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART4_CTS_INV_SHIFT)               /* 0x00000100 */
#define BUS_GRF_SOC_CON2_UART4_RTS_INV_SHIFT               (9U)
#define BUS_GRF_SOC_CON2_UART4_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART4_RTS_INV_SHIFT)               /* 0x00000200 */
#define BUS_GRF_SOC_CON2_UART5_CTS_INV_SHIFT               (10U)
#define BUS_GRF_SOC_CON2_UART5_CTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART5_CTS_INV_SHIFT)               /* 0x00000400 */
#define BUS_GRF_SOC_CON2_UART5_RTS_INV_SHIFT               (11U)
#define BUS_GRF_SOC_CON2_UART5_RTS_INV_MASK                (0x1U << BUS_GRF_SOC_CON2_UART5_RTS_INV_SHIFT)               /* 0x00000800 */
#define BUS_GRF_SOC_CON2_WDTNS_GLB_RESET_EN_SHIFT          (12U)
#define BUS_GRF_SOC_CON2_WDTNS_GLB_RESET_EN_MASK           (0x1U << BUS_GRF_SOC_CON2_WDTNS_GLB_RESET_EN_SHIFT)          /* 0x00001000 */
#define BUS_GRF_SOC_CON2_SYSTEM_MEM_CS_CLK_GATE_EN_SHIFT   (13U)
#define BUS_GRF_SOC_CON2_SYSTEM_MEM_CS_CLK_GATE_EN_MASK    (0x1U << BUS_GRF_SOC_CON2_SYSTEM_MEM_CS_CLK_GATE_EN_SHIFT)   /* 0x00002000 */
#define BUS_GRF_SOC_CON2_NPU_DISABLE_RAMCLK_GATE_SHIFT     (14U)
#define BUS_GRF_SOC_CON2_NPU_DISABLE_RAMCLK_GATE_MASK      (0x1U << BUS_GRF_SOC_CON2_NPU_DISABLE_RAMCLK_GATE_SHIFT)     /* 0x00004000 */
#define BUS_GRF_SOC_CON2_SYSTEM_MEM_INTL_SHIFT             (15U)
#define BUS_GRF_SOC_CON2_SYSTEM_MEM_INTL_MASK              (0x1U << BUS_GRF_SOC_CON2_SYSTEM_MEM_INTL_SHIFT)             /* 0x00008000 */
/* SOC_CON3 */
#define BUS_GRF_SOC_CON3_OFFSET                            (0x40C)
#define BUS_GRF_SOC_CON3_SARADC_ANA_REG_SHIFT              (0U)
#define BUS_GRF_SOC_CON3_SARADC_ANA_REG_MASK               (0xFFFFU << BUS_GRF_SOC_CON3_SARADC_ANA_REG_SHIFT)           /* 0x0000FFFF */
/* SOC_CON4 */
#define BUS_GRF_SOC_CON4_OFFSET                            (0x410)
#define BUS_GRF_SOC_CON4_PMU_PWR_IDLE_REQ_SHIFT            (14U)
#define BUS_GRF_SOC_CON4_PMU_PWR_IDLE_REQ_MASK             (0x1U << BUS_GRF_SOC_CON4_PMU_PWR_IDLE_REQ_SHIFT)            /* 0x00004000 */
#define BUS_GRF_SOC_CON4_GIC_PWR_IDLE_REQ_SHIFT            (15U)
#define BUS_GRF_SOC_CON4_GIC_PWR_IDLE_REQ_MASK             (0x1U << BUS_GRF_SOC_CON4_GIC_PWR_IDLE_REQ_SHIFT)            /* 0x00008000 */
/* SOC_CON5 */
#define BUS_GRF_SOC_CON5_OFFSET                            (0x414)
#define BUS_GRF_SOC_CON5_SDCARD_DECTN_DLY_SHIFT            (0U)
#define BUS_GRF_SOC_CON5_SDCARD_DECTN_DLY_MASK             (0xFFFFFFFFU << BUS_GRF_SOC_CON5_SDCARD_DECTN_DLY_SHIFT)     /* 0xFFFFFFFF */
/* VI_CON0 */
#define BUS_GRF_VI_CON0_OFFSET                             (0x430)
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_0_SHIFT         (0U)
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_0_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_0_SHIFT)         /* 0x00000001 */
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_1_SHIFT         (1U)
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_1_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_1_SHIFT)         /* 0x00000002 */
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_2_SHIFT         (2U)
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_2_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_2_SHIFT)         /* 0x00000004 */
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_3_SHIFT         (3U)
#define BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_3_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_FORCERXMODE_3_SHIFT)         /* 0x00000008 */
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_0_SHIFT         (4U)
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_0_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_0_SHIFT)         /* 0x00000010 */
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_1_SHIFT         (5U)
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_1_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_1_SHIFT)         /* 0x00000020 */
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_2_SHIFT         (6U)
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_2_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_2_SHIFT)         /* 0x00000040 */
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_3_SHIFT         (7U)
#define BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_3_MASK          (0x1U << BUS_GRF_VI_CON0_CSIPHY_DATALANE_EN_3_SHIFT)         /* 0x00000080 */
#define BUS_GRF_VI_CON0_CSIPHY_CLKLANE_EN_SHIFT            (8U)
#define BUS_GRF_VI_CON0_CSIPHY_CLKLANE_EN_MASK             (0x1U << BUS_GRF_VI_CON0_CSIPHY_CLKLANE_EN_SHIFT)            /* 0x00000100 */
#define BUS_GRF_VI_CON0_CSIPHY_CLKINV_SELECTION_SHIFT      (9U)
#define BUS_GRF_VI_CON0_CSIPHY_CLKINV_SELECTION_MASK       (0x1U << BUS_GRF_VI_CON0_CSIPHY_CLKINV_SELECTION_SHIFT)      /* 0x00000200 */
#define BUS_GRF_VI_CON0_DVP_CLK_INV_SEL_SHIFT              (12U)
#define BUS_GRF_VI_CON0_DVP_CLK_INV_SEL_MASK               (0x1U << BUS_GRF_VI_CON0_DVP_CLK_INV_SEL_SHIFT)              /* 0x00001000 */
#define BUS_GRF_VI_CON0_ISP_CIF_IF_DATAWIDTH_SHIFT         (13U)
#define BUS_GRF_VI_CON0_ISP_CIF_IF_DATAWIDTH_MASK          (0x3U << BUS_GRF_VI_CON0_ISP_CIF_IF_DATAWIDTH_SHIFT)         /* 0x00006000 */
/* VI_CON1 */
#define BUS_GRF_VI_CON1_OFFSET                             (0x434)
#define BUS_GRF_VI_CON1_CSIPHY_CLKLANE_NUM_SHIFT           (0U)
#define BUS_GRF_VI_CON1_CSIPHY_CLKLANE_NUM_MASK            (0x7FU << BUS_GRF_VI_CON1_CSIPHY_CLKLANE_NUM_SHIFT)          /* 0x0000007F */
#define BUS_GRF_VI_CON1_CIF_CLK_DELAY_EN_SHIFT             (8U)
#define BUS_GRF_VI_CON1_CIF_CLK_DELAY_EN_MASK              (0x1U << BUS_GRF_VI_CON1_CIF_CLK_DELAY_EN_SHIFT)             /* 0x00000100 */
#define BUS_GRF_VI_CON1_CIF_DATAPATH_SHIFT                 (9U)
#define BUS_GRF_VI_CON1_CIF_DATAPATH_MASK                  (0x1U << BUS_GRF_VI_CON1_CIF_DATAPATH_SHIFT)                 /* 0x00000200 */
#define BUS_GRF_VI_CON1_ISP_MAINPRESS_SHIFT                (10U)
#define BUS_GRF_VI_CON1_ISP_MAINPRESS_MASK                 (0x3U << BUS_GRF_VI_CON1_ISP_MAINPRESS_SHIFT)                /* 0x00000C00 */
#define BUS_GRF_VI_CON1_DVP_MAINPRESS_SHIFT                (13U)
#define BUS_GRF_VI_CON1_DVP_MAINPRESS_MASK                 (0x3U << BUS_GRF_VI_CON1_DVP_MAINPRESS_SHIFT)                /* 0x00006000 */
/* VI_STATUS */
#define BUS_GRF_VI_STATUS_OFFSET                           (0x438)
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_0_SHIFT     (0U)
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_0_MASK      (0x1U << BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_0_SHIFT)     /* 0x00000001 */
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_1_SHIFT     (1U)
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_1_MASK      (0x1U << BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_1_SHIFT)     /* 0x00000002 */
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_2_SHIFT     (2U)
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_2_MASK      (0x1U << BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_2_SHIFT)     /* 0x00000004 */
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_3_SHIFT     (3U)
#define BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_3_MASK      (0x1U << BUS_GRF_VI_STATUS_CSIPHY_ULPSACTIVENOT_3_SHIFT)     /* 0x00000008 */
#define BUS_GRF_VI_STATUS_CSIPHY_DIRECTION_SHIFT           (4U)
#define BUS_GRF_VI_STATUS_CSIPHY_DIRECTION_MASK            (0x1U << BUS_GRF_VI_STATUS_CSIPHY_DIRECTION_SHIFT)           /* 0x00000010 */
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_0_SHIFT       (5U)
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_0_MASK        (0x1U << BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_0_SHIFT)       /* 0x00000020 */
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_1_SHIFT       (6U)
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_1_MASK        (0x1U << BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_1_SHIFT)       /* 0x00000040 */
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_2_SHIFT       (7U)
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_2_MASK        (0x1U << BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_2_SHIFT)       /* 0x00000080 */
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_3_SHIFT       (8U)
#define BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_3_MASK        (0x1U << BUS_GRF_VI_STATUS_CSIPHY_RXSKEWCALHS_3_SHIFT)       /* 0x00000100 */
#define BUS_GRF_VI_STATUS_CSIPHY_ERRCONTENTIONLP0_0_SHIFT  (9U)
#define BUS_GRF_VI_STATUS_CSIPHY_ERRCONTENTIONLP0_0_MASK   (0x1U << BUS_GRF_VI_STATUS_CSIPHY_ERRCONTENTIONLP0_0_SHIFT)  /* 0x00000200 */
#define BUS_GRF_VI_STATUS_CSIPHY_ERRCONTENTIONLP1_0_SHIFT  (10U)
#define BUS_GRF_VI_STATUS_CSIPHY_ERRCONTENTIONLP1_0_MASK   (0x1U << BUS_GRF_VI_STATUS_CSIPHY_ERRCONTENTIONLP1_0_SHIFT)  /* 0x00000400 */
/* VO_CON0 */
#define BUS_GRF_VO_CON0_OFFSET                             (0x440)
#define BUS_GRF_VO_CON0_VOPRAW_DMA_FINISH_ENABLE_SHIFT     (0U)
#define BUS_GRF_VO_CON0_VOPRAW_DMA_FINISH_ENABLE_MASK      (0x1U << BUS_GRF_VO_CON0_VOPRAW_DMA_FINISH_ENABLE_SHIFT)     /* 0x00000001 */
#define BUS_GRF_VO_CON0_VOPLITE_DMA_FINISH_ENABLE_SHIFT    (1U)
#define BUS_GRF_VO_CON0_VOPLITE_DMA_FINISH_ENABLE_MASK     (0x1U << BUS_GRF_VO_CON0_VOPLITE_DMA_FINISH_ENABLE_SHIFT)    /* 0x00000002 */
#define BUS_GRF_VO_CON0_DSIHOST_DPISHUTDN_SHIFT            (2U)
#define BUS_GRF_VO_CON0_DSIHOST_DPISHUTDN_MASK             (0x1U << BUS_GRF_VO_CON0_DSIHOST_DPISHUTDN_SHIFT)            /* 0x00000004 */
#define BUS_GRF_VO_CON0_DSIHOST_DPICOLORM_SHIFT            (3U)
#define BUS_GRF_VO_CON0_DSIHOST_DPICOLORM_MASK             (0x1U << BUS_GRF_VO_CON0_DSIHOST_DPICOLORM_SHIFT)            /* 0x00000008 */
#define BUS_GRF_VO_CON0_DSIHOST_DPIUPDATECFG_SHIFT         (7U)
#define BUS_GRF_VO_CON0_DSIHOST_DPIUPDATECFG_MASK          (0x1U << BUS_GRF_VO_CON0_DSIHOST_DPIUPDATECFG_SHIFT)         /* 0x00000080 */
#define BUS_GRF_VO_CON0_DSIPHY_SELECTION_SHIFT             (8U)
#define BUS_GRF_VO_CON0_DSIPHY_SELECTION_MASK              (0x1U << BUS_GRF_VO_CON0_DSIPHY_SELECTION_SHIFT)             /* 0x00000100 */
#define BUS_GRF_VO_CON0_DCF_VOP_STANDBY_SEL_SHIFT          (10U)
#define BUS_GRF_VO_CON0_DCF_VOP_STANDBY_SEL_MASK           (0x3U << BUS_GRF_VO_CON0_DCF_VOP_STANDBY_SEL_SHIFT)          /* 0x00000C00 */
#define BUS_GRF_VO_CON0_VOPRAW_PRESS_SHIFT                 (12U)
#define BUS_GRF_VO_CON0_VOPRAW_PRESS_MASK                  (0x3U << BUS_GRF_VO_CON0_VOPRAW_PRESS_SHIFT)                 /* 0x00003000 */
#define BUS_GRF_VO_CON0_VOPLITE_PRESS_SHIFT                (14U)
#define BUS_GRF_VO_CON0_VOPLITE_PRESS_MASK                 (0x3U << BUS_GRF_VO_CON0_VOPLITE_PRESS_SHIFT)                /* 0x0000C000 */
/* VO_CON1 */
#define BUS_GRF_VO_CON1_OFFSET                             (0x444)
#define BUS_GRF_VO_CON1_RGB_BYPASS_SHIFT                   (3U)
#define BUS_GRF_VO_CON1_RGB_BYPASS_MASK                    (0x1U << BUS_GRF_VO_CON1_RGB_BYPASS_SHIFT)                   /* 0x00000008 */
#define BUS_GRF_VO_CON1_LCDC_DCLK_INV_SEL_SHIFT            (4U)
#define BUS_GRF_VO_CON1_LCDC_DCLK_INV_SEL_MASK             (0x1U << BUS_GRF_VO_CON1_LCDC_DCLK_INV_SEL_SHIFT)            /* 0x00000010 */
#define BUS_GRF_VO_CON1_DSIPHY_LANE0_TURNDISABLE_SHIFT     (5U)
#define BUS_GRF_VO_CON1_DSIPHY_LANE0_TURNDISABLE_MASK      (0x1U << BUS_GRF_VO_CON1_DSIPHY_LANE0_TURNDISABLE_SHIFT)     /* 0x00000020 */
#define BUS_GRF_VO_CON1_DSIPHY_FORCERXMODE_SHIFT           (6U)
#define BUS_GRF_VO_CON1_DSIPHY_FORCERXMODE_MASK            (0x1U << BUS_GRF_VO_CON1_DSIPHY_FORCERXMODE_SHIFT)           /* 0x00000040 */
#define BUS_GRF_VO_CON1_DSIPHY_LANE0_FRCTXSTPM_SHIFT       (7U)
#define BUS_GRF_VO_CON1_DSIPHY_LANE0_FRCTXSTPM_MASK        (0x1U << BUS_GRF_VO_CON1_DSIPHY_LANE0_FRCTXSTPM_SHIFT)       /* 0x00000080 */
#define BUS_GRF_VO_CON1_DSIPHY_LANE1_FRCTXSTPM_SHIFT       (8U)
#define BUS_GRF_VO_CON1_DSIPHY_LANE1_FRCTXSTPM_MASK        (0x1U << BUS_GRF_VO_CON1_DSIPHY_LANE1_FRCTXSTPM_SHIFT)       /* 0x00000100 */
#define BUS_GRF_VO_CON1_DSIPHY_LANE2_FRCTXSTPM_SHIFT       (9U)
#define BUS_GRF_VO_CON1_DSIPHY_LANE2_FRCTXSTPM_MASK        (0x1U << BUS_GRF_VO_CON1_DSIPHY_LANE2_FRCTXSTPM_SHIFT)       /* 0x00000200 */
#define BUS_GRF_VO_CON1_DSIPHY_LANE3_FRCTXSTPM_SHIFT       (10U)
#define BUS_GRF_VO_CON1_DSIPHY_LANE3_FRCTXSTPM_MASK        (0x1U << BUS_GRF_VO_CON1_DSIPHY_LANE3_FRCTXSTPM_SHIFT)       /* 0x00000400 */
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_CK_SHIFT        (11U)
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_CK_MASK         (0x1U << BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_CK_SHIFT)        /* 0x00000800 */
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_0_SHIFT         (12U)
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_0_MASK          (0x1U << BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_0_SHIFT)         /* 0x00001000 */
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_1_SHIFT         (13U)
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_1_MASK          (0x1U << BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_1_SHIFT)         /* 0x00002000 */
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_2_SHIFT         (14U)
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_2_MASK          (0x1U << BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_2_SHIFT)         /* 0x00004000 */
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_3_SHIFT         (15U)
#define BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_3_MASK          (0x1U << BUS_GRF_VO_CON1_DSIPHY_TXSKEWCALHS_3_SHIFT)         /* 0x00008000 */
/* SOC_STATUS0 */
#define BUS_GRF_SOC_STATUS0_OFFSET                         (0x480)
#define BUS_GRF_SOC_STATUS0_APLL_LOCK_SHIFT                (0U)
#define BUS_GRF_SOC_STATUS0_APLL_LOCK_MASK                 (0x1U << BUS_GRF_SOC_STATUS0_APLL_LOCK_SHIFT)                /* 0x00000001 */
#define BUS_GRF_SOC_STATUS0_DPLL_LOCK_SHIFT                (1U)
#define BUS_GRF_SOC_STATUS0_DPLL_LOCK_MASK                 (0x1U << BUS_GRF_SOC_STATUS0_DPLL_LOCK_SHIFT)                /* 0x00000002 */
#define BUS_GRF_SOC_STATUS0_CPLL_LOCK_SHIFT                (2U)
#define BUS_GRF_SOC_STATUS0_CPLL_LOCK_MASK                 (0x1U << BUS_GRF_SOC_STATUS0_CPLL_LOCK_SHIFT)                /* 0x00000004 */
#define BUS_GRF_SOC_STATUS0_GPLL_LOCK_SHIFT                (3U)
#define BUS_GRF_SOC_STATUS0_GPLL_LOCK_MASK                 (0x1U << BUS_GRF_SOC_STATUS0_GPLL_LOCK_SHIFT)                /* 0x00000008 */
#define BUS_GRF_SOC_STATUS0_PPLL_LOCK_SHIFT                (4U)
#define BUS_GRF_SOC_STATUS0_PPLL_LOCK_MASK                 (0x1U << BUS_GRF_SOC_STATUS0_PPLL_LOCK_SHIFT)                /* 0x00000010 */
#define BUS_GRF_SOC_STATUS0_NPLL_LOCK_SHIFT                (5U)
#define BUS_GRF_SOC_STATUS0_NPLL_LOCK_MASK                 (0x1U << BUS_GRF_SOC_STATUS0_NPLL_LOCK_SHIFT)                /* 0x00000020 */
#define BUS_GRF_SOC_STATUS0_DDR_CMD_PLL_LOCK_SHIFT         (6U)
#define BUS_GRF_SOC_STATUS0_DDR_CMD_PLL_LOCK_MASK          (0x1U << BUS_GRF_SOC_STATUS0_DDR_CMD_PLL_LOCK_SHIFT)         /* 0x00000040 */
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS0_SHIFT         (11U)
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS0_MASK          (0x1U << BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS0_SHIFT)         /* 0x00000800 */
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS1_SHIFT         (12U)
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS1_MASK          (0x1U << BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS1_SHIFT)         /* 0x00001000 */
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS2_SHIFT         (13U)
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS2_MASK          (0x1U << BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS2_SHIFT)         /* 0x00002000 */
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS3_SHIFT         (14U)
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS3_MASK          (0x1U << BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS3_SHIFT)         /* 0x00004000 */
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS4_SHIFT         (15U)
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS4_MASK          (0x1U << BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS4_SHIFT)         /* 0x00008000 */
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS5_SHIFT         (16U)
#define BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS5_MASK          (0x1U << BUS_GRF_SOC_STATUS0_TIMER_EN_STATUS5_SHIFT)         /* 0x00010000 */
#define BUS_GRF_SOC_STATUS0_VOPRAW_DMA_FINISH_SHIFT        (17U)
#define BUS_GRF_SOC_STATUS0_VOPRAW_DMA_FINISH_MASK         (0x1U << BUS_GRF_SOC_STATUS0_VOPRAW_DMA_FINISH_SHIFT)        /* 0x00020000 */
#define BUS_GRF_SOC_STATUS0_VOPLITE_DMA_FINISH_SHIFT       (18U)
#define BUS_GRF_SOC_STATUS0_VOPLITE_DMA_FINISH_MASK        (0x1U << BUS_GRF_SOC_STATUS0_VOPLITE_DMA_FINISH_SHIFT)       /* 0x00040000 */
#define BUS_GRF_SOC_STATUS0_PMU_PWR_IDLE_SHIFT             (19U)
#define BUS_GRF_SOC_STATUS0_PMU_PWR_IDLE_MASK              (0x1U << BUS_GRF_SOC_STATUS0_PMU_PWR_IDLE_SHIFT)             /* 0x00080000 */
#define BUS_GRF_SOC_STATUS0_PMU_PWR_IDLE_ACK_SHIFT         (20U)
#define BUS_GRF_SOC_STATUS0_PMU_PWR_IDLE_ACK_MASK          (0x1U << BUS_GRF_SOC_STATUS0_PMU_PWR_IDLE_ACK_SHIFT)         /* 0x00100000 */
#define BUS_GRF_SOC_STATUS0_GIC_PWR_IDLE_SHIFT             (21U)
#define BUS_GRF_SOC_STATUS0_GIC_PWR_IDLE_MASK              (0x1U << BUS_GRF_SOC_STATUS0_GIC_PWR_IDLE_SHIFT)             /* 0x00200000 */
#define BUS_GRF_SOC_STATUS0_GIC_PWR_IDLE_STATUS_SHIFT      (22U)
#define BUS_GRF_SOC_STATUS0_GIC_PWR_IDLE_STATUS_MASK       (0x1U << BUS_GRF_SOC_STATUS0_GIC_PWR_IDLE_STATUS_SHIFT)      /* 0x00400000 */
#define BUS_GRF_SOC_STATUS0_GPIO1B6_DATAIN_SHIFT           (23U)
#define BUS_GRF_SOC_STATUS0_GPIO1B6_DATAIN_MASK            (0x1U << BUS_GRF_SOC_STATUS0_GPIO1B6_DATAIN_SHIFT)           /* 0x00800000 */
#define BUS_GRF_SOC_STATUS0_NPU_DEBUG_SHIFT                (24U)
#define BUS_GRF_SOC_STATUS0_NPU_DEBUG_MASK                 (0xFFU << BUS_GRF_SOC_STATUS0_NPU_DEBUG_SHIFT)               /* 0xFF000000 */
/* CPU_CON0 */
#define BUS_GRF_CPU_CON0_OFFSET                            (0x500)
#define BUS_GRF_CPU_CON0_L1RSTDISABLE_SHIFT                (0U)
#define BUS_GRF_CPU_CON0_L1RSTDISABLE_MASK                 (0x3U << BUS_GRF_CPU_CON0_L1RSTDISABLE_SHIFT)                /* 0x00000003 */
#define BUS_GRF_CPU_CON0_L2RSTDISABLE_SHIFT                (4U)
#define BUS_GRF_CPU_CON0_L2RSTDISABLE_MASK                 (0x1U << BUS_GRF_CPU_CON0_L2RSTDISABLE_SHIFT)                /* 0x00000010 */
#define BUS_GRF_CPU_CON0_CFGEND_SHIFT                      (8U)
#define BUS_GRF_CPU_CON0_CFGEND_MASK                       (0x3U << BUS_GRF_CPU_CON0_CFGEND_SHIFT)                      /* 0x00000300 */
#define BUS_GRF_CPU_CON0_CFGTE_SHIFT                       (12U)
#define BUS_GRF_CPU_CON0_CFGTE_MASK                        (0x3U << BUS_GRF_CPU_CON0_CFGTE_SHIFT)                       /* 0x00003000 */
/* CPU_CON1 */
#define BUS_GRF_CPU_CON1_OFFSET                            (0x504)
#define BUS_GRF_CPU_CON1_CLREXMONREQ_SHIFT                 (0U)
#define BUS_GRF_CPU_CON1_CLREXMONREQ_MASK                  (0x1U << BUS_GRF_CPU_CON1_CLREXMONREQ_SHIFT)                 /* 0x00000001 */
#define BUS_GRF_CPU_CON1_CFGSDISABLE_SHIFT                 (1U)
#define BUS_GRF_CPU_CON1_CFGSDISABLE_MASK                  (0x1U << BUS_GRF_CPU_CON1_CFGSDISABLE_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_CPU_CON1_DBGROMADDRV_SHIFT                 (2U)
#define BUS_GRF_CPU_CON1_DBGROMADDRV_MASK                  (0x1U << BUS_GRF_CPU_CON1_DBGROMADDRV_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_CPU_CON1_EVENTI_SHIFT                      (4U)
#define BUS_GRF_CPU_CON1_EVENTI_MASK                       (0x1U << BUS_GRF_CPU_CON1_EVENTI_SHIFT)                      /* 0x00000010 */
#define BUS_GRF_CPU_CON1_EVENTO_CLEAR_SHIFT                (5U)
#define BUS_GRF_CPU_CON1_EVENTO_CLEAR_MASK                 (0x1U << BUS_GRF_CPU_CON1_EVENTO_CLEAR_SHIFT)                /* 0x00000020 */
#define BUS_GRF_CPU_CON1_CPU_EMA_DETECT_EN_SHIFT           (6U)
#define BUS_GRF_CPU_CON1_CPU_EMA_DETECT_EN_MASK            (0x1U << BUS_GRF_CPU_CON1_CPU_EMA_DETECT_EN_SHIFT)           /* 0x00000040 */
#define BUS_GRF_CPU_CON1_FORCE_JTAG_SHIFT                  (7U)
#define BUS_GRF_CPU_CON1_FORCE_JTAG_MASK                   (0x1U << BUS_GRF_CPU_CON1_FORCE_JTAG_SHIFT)                  /* 0x00000080 */
#define BUS_GRF_CPU_CON1_GIC500_CPU_ACTIVE_0_SHIFT         (8U)
#define BUS_GRF_CPU_CON1_GIC500_CPU_ACTIVE_0_MASK          (0x3U << BUS_GRF_CPU_CON1_GIC500_CPU_ACTIVE_0_SHIFT)         /* 0x00000300 */
#define BUS_GRF_CPU_CON1_GIC500_AXIM_ERR_ACK_SHIFT         (10U)
#define BUS_GRF_CPU_CON1_GIC500_AXIM_ERR_ACK_MASK          (0x1U << BUS_GRF_CPU_CON1_GIC500_AXIM_ERR_ACK_SHIFT)         /* 0x00000400 */
#define BUS_GRF_CPU_CON1_AWUSER_MODE_SHIFT                 (11U)
#define BUS_GRF_CPU_CON1_AWUSER_MODE_MASK                  (0x1U << BUS_GRF_CPU_CON1_AWUSER_MODE_SHIFT)                 /* 0x00000800 */
/* CPU_CON2 */
#define BUS_GRF_CPU_CON2_OFFSET                            (0x508)
#define BUS_GRF_CPU_CON2_CA35_MA_SAWL_SHIFT                (0U)
#define BUS_GRF_CPU_CON2_CA35_MA_SAWL_MASK                 (0x1U << BUS_GRF_CPU_CON2_CA35_MA_SAWL_SHIFT)                /* 0x00000001 */
#define BUS_GRF_CPU_CON2_CA35_MA_WL_SHIFT                  (1U)
#define BUS_GRF_CPU_CON2_CA35_MA_WL_MASK                   (0x1U << BUS_GRF_CPU_CON2_CA35_MA_WL_SHIFT)                  /* 0x00000002 */
#define BUS_GRF_CPU_CON2_CA35_MA_WRAS_SHIFT                (2U)
#define BUS_GRF_CPU_CON2_CA35_MA_WRAS_MASK                 (0x1U << BUS_GRF_CPU_CON2_CA35_MA_WRAS_SHIFT)                /* 0x00000004 */
#define BUS_GRF_CPU_CON2_CA35_MA_WRASD_SHIFT               (3U)
#define BUS_GRF_CPU_CON2_CA35_MA_WRASD_MASK                (0x1U << BUS_GRF_CPU_CON2_CA35_MA_WRASD_SHIFT)               /* 0x00000008 */
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_SAWL_SHIFT            (4U)
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_SAWL_MASK             (0x1U << BUS_GRF_CPU_CON2_CA35_L2D_MA_SAWL_SHIFT)            /* 0x00000010 */
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_WL_SHIFT              (5U)
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_WL_MASK               (0x1U << BUS_GRF_CPU_CON2_CA35_L2D_MA_WL_SHIFT)              /* 0x00000020 */
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_WRAS_SHIFT            (6U)
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_WRAS_MASK             (0x1U << BUS_GRF_CPU_CON2_CA35_L2D_MA_WRAS_SHIFT)            /* 0x00000040 */
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_WRASD_SHIFT           (7U)
#define BUS_GRF_CPU_CON2_CA35_L2D_MA_WRASD_MASK            (0x1U << BUS_GRF_CPU_CON2_CA35_L2D_MA_WRASD_SHIFT)           /* 0x00000080 */
/* CPU_STATUS0 */
#define BUS_GRF_CPU_STATUS0_OFFSET                         (0x520)
#define BUS_GRF_CPU_STATUS0_SMPNAMP_SHIFT                  (0U)
#define BUS_GRF_CPU_STATUS0_SMPNAMP_MASK                   (0x3U << BUS_GRF_CPU_STATUS0_SMPNAMP_SHIFT)                  /* 0x00000003 */
#define BUS_GRF_CPU_STATUS0_GIC500_ECC_FATAL_SHIFT         (4U)
#define BUS_GRF_CPU_STATUS0_GIC500_ECC_FATAL_MASK          (0x1U << BUS_GRF_CPU_STATUS0_GIC500_ECC_FATAL_SHIFT)         /* 0x00000010 */
#define BUS_GRF_CPU_STATUS0_GIC500_AXIM_ERR_SHIFT          (5U)
#define BUS_GRF_CPU_STATUS0_GIC500_AXIM_ERR_MASK           (0x1U << BUS_GRF_CPU_STATUS0_GIC500_AXIM_ERR_SHIFT)          /* 0x00000020 */
#define BUS_GRF_CPU_STATUS0_EVENTO_RISING_EDGE_SHIFT       (12U)
#define BUS_GRF_CPU_STATUS0_EVENTO_RISING_EDGE_MASK        (0x1U << BUS_GRF_CPU_STATUS0_EVENTO_RISING_EDGE_SHIFT)       /* 0x00001000 */
#define BUS_GRF_CPU_STATUS0_JTAGTOP_SHIFT                  (13U)
#define BUS_GRF_CPU_STATUS0_JTAGTOP_MASK                   (0x1U << BUS_GRF_CPU_STATUS0_JTAGTOP_SHIFT)                  /* 0x00002000 */
#define BUS_GRF_CPU_STATUS0_JTAGNSW_SHIFT                  (14U)
#define BUS_GRF_CPU_STATUS0_JTAGNSW_MASK                   (0x1U << BUS_GRF_CPU_STATUS0_JTAGNSW_SHIFT)                  /* 0x00004000 */
#define BUS_GRF_CPU_STATUS0_CLREXMONACK_SHIFT              (15U)
#define BUS_GRF_CPU_STATUS0_CLREXMONACK_MASK               (0x1U << BUS_GRF_CPU_STATUS0_CLREXMONACK_SHIFT)              /* 0x00008000 */
#define BUS_GRF_CPU_STATUS0_L2FLUSHDONE_SHIFT              (16U)
#define BUS_GRF_CPU_STATUS0_L2FLUSHDONE_MASK               (0x1U << BUS_GRF_CPU_STATUS0_L2FLUSHDONE_SHIFT)              /* 0x00010000 */
/* CPU_STATUS1 */
#define BUS_GRF_CPU_STATUS1_OFFSET                         (0x524)
#define BUS_GRF_CPU_STATUS1_STANDBYWFE_SHIFT               (0U)
#define BUS_GRF_CPU_STATUS1_STANDBYWFE_MASK                (0x3U << BUS_GRF_CPU_STATUS1_STANDBYWFE_SHIFT)               /* 0x00000003 */
#define BUS_GRF_CPU_STATUS1_STANDBYWFI_SHIFT               (4U)
#define BUS_GRF_CPU_STATUS1_STANDBYWFI_MASK                (0x3U << BUS_GRF_CPU_STATUS1_STANDBYWFI_SHIFT)               /* 0x00000030 */
#define BUS_GRF_CPU_STATUS1_STANDBYWFIL2_SHIFT             (12U)
#define BUS_GRF_CPU_STATUS1_STANDBYWFIL2_MASK              (0x1U << BUS_GRF_CPU_STATUS1_STANDBYWFIL2_SHIFT)             /* 0x00001000 */
/* SOC_NOC_CON0 */
#define BUS_GRF_SOC_NOC_CON0_OFFSET                        (0x530)
#define BUS_GRF_SOC_NOC_CON0_AUDIO_FWD_BUS_BUS_STALL_SHIFT (0U)
#define BUS_GRF_SOC_NOC_CON0_AUDIO_FWD_BUS_BUS_STALL_MASK  (0x1U << BUS_GRF_SOC_NOC_CON0_AUDIO_FWD_BUS_BUS_STALL_SHIFT) /* 0x00000001 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_AUDIO_AUDIO_STALL_SHIFT (1U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_AUDIO_AUDIO_STALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_AUDIO_AUDIO_STALL_SHIFT) /* 0x00000002 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_BUS_MST_DM_STALL_SHIFT (2U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_BUS_MST_DM_STALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_BUS_MST_DM_STALL_SHIFT) /* 0x00000004 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_DDRC_DDRC_APB_TPWRSTALL_SHIFT (3U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_DDRC_DDRC_APB_TPWRSTALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_DDRC_DDRC_APB_TPWRSTALL_SHIFT) /* 0x00000008 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_GIC_GIC_STALL_SHIFT   (4U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_GIC_GIC_STALL_MASK    (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_GIC_GIC_STALL_SHIFT)   /* 0x00000010 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_SYSTEM_MEM_SYSTEM_MEM_STALL_SHIFT (5U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_SYSTEM_MEM_SYSTEM_MEM_STALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_SYSTEM_MEM_SYSTEM_MEM_STALL_SHIFT) /* 0x00000020 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_NPU_NPU_STALL_SHIFT   (6U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_NPU_NPU_STALL_MASK    (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_NPU_NPU_STALL_SHIFT)   /* 0x00000040 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_PMU_PM_STALL_SHIFT    (7U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_PMU_PM_STALL_MASK     (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_PMU_PM_STALL_SHIFT)    /* 0x00000080 */
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_SRVMSCH_SWITCH10PWRSTALL_SHIFT (8U)
#define BUS_GRF_SOC_NOC_CON0_BUS_FWD_SRVMSCH_SWITCH10PWRSTALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_FWD_SRVMSCH_SWITCH10PWRSTALL_SHIFT) /* 0x00000100 */
#define BUS_GRF_SOC_NOC_CON0_BUS_MST_FWD_BUS_SWITCH21PWRSTALL_SHIFT (9U)
#define BUS_GRF_SOC_NOC_CON0_BUS_MST_FWD_BUS_SWITCH21PWRSTALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_MST_FWD_BUS_SWITCH21PWRSTALL_SHIFT) /* 0x00000200 */
#define BUS_GRF_SOC_NOC_CON0_BUS_REQ_MSCH_MSCH_STALL_SHIFT (10U)
#define BUS_GRF_SOC_NOC_CON0_BUS_REQ_MSCH_MSCH_STALL_MASK  (0x1U << BUS_GRF_SOC_NOC_CON0_BUS_REQ_MSCH_MSCH_STALL_SHIFT) /* 0x00000400 */
#define BUS_GRF_SOC_NOC_CON0_CPU_FWD_BUS_SWITCH18PWRSTALL_SHIFT (11U)
#define BUS_GRF_SOC_NOC_CON0_CPU_FWD_BUS_SWITCH18PWRSTALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_CPU_FWD_BUS_SWITCH18PWRSTALL_SHIFT) /* 0x00000800 */
#define BUS_GRF_SOC_NOC_CON0_CPU_FWD_GIC_GIC_STALL_SHIFT   (12U)
#define BUS_GRF_SOC_NOC_CON0_CPU_FWD_GIC_GIC_STALL_MASK    (0x1U << BUS_GRF_SOC_NOC_CON0_CPU_FWD_GIC_GIC_STALL_SHIFT)   /* 0x00001000 */
#define BUS_GRF_SOC_NOC_CON0_CPU_REQ_MSCH_MSCH_STALL_SHIFT (13U)
#define BUS_GRF_SOC_NOC_CON0_CPU_REQ_MSCH_MSCH_STALL_MASK  (0x1U << BUS_GRF_SOC_NOC_CON0_CPU_REQ_MSCH_MSCH_STALL_SHIFT) /* 0x00002000 */
#define BUS_GRF_SOC_NOC_CON0_GIC_FWD_BUS_BUS_STALL_SHIFT   (14U)
#define BUS_GRF_SOC_NOC_CON0_GIC_FWD_BUS_BUS_STALL_MASK    (0x1U << BUS_GRF_SOC_NOC_CON0_GIC_FWD_BUS_BUS_STALL_SHIFT)   /* 0x00004000 */
#define BUS_GRF_SOC_NOC_CON0_SYSTEM_MEM_FWD_SYSTEM_MEM0_SYSTEM_MEM_S0_TPWRSTALL_SHIFT (15U)
#define BUS_GRF_SOC_NOC_CON0_SYSTEM_MEM_FWD_SYSTEM_MEM0_SYSTEM_MEM_S0_TPWRSTALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON0_SYSTEM_MEM_FWD_SYSTEM_MEM0_SYSTEM_MEM_S0_TPWRSTALL_SHIFT) /* 0x00008000 */
/* SOC_NOC_CON1 */
#define BUS_GRF_SOC_NOC_CON1_OFFSET                        (0x534)
#define BUS_GRF_SOC_NOC_CON1_NPU_FWD_SYSTEM_MEM_SYSTEM_MEM_STALL_SHIFT (3U)
#define BUS_GRF_SOC_NOC_CON1_NPU_FWD_SYSTEM_MEM_SYSTEM_MEM_STALL_MASK (0x1U << BUS_GRF_SOC_NOC_CON1_NPU_FWD_SYSTEM_MEM_SYSTEM_MEM_STALL_SHIFT) /* 0x00000008 */
#define BUS_GRF_SOC_NOC_CON1_NPU_REQ_MSCH_STALL_SHIFT      (4U)
#define BUS_GRF_SOC_NOC_CON1_NPU_REQ_MSCH_STALL_MASK       (0x1U << BUS_GRF_SOC_NOC_CON1_NPU_REQ_MSCH_STALL_SHIFT)      /* 0x00000010 */
#define BUS_GRF_SOC_NOC_CON1_BUS_FWD_PCIE_STALL_SHIFT      (5U)
#define BUS_GRF_SOC_NOC_CON1_BUS_FWD_PCIE_STALL_MASK       (0x1U << BUS_GRF_SOC_NOC_CON1_BUS_FWD_PCIE_STALL_SHIFT)      /* 0x00000020 */
#define BUS_GRF_SOC_NOC_CON1_BUS_FWD_PHP_STALL_SHIFT       (6U)
#define BUS_GRF_SOC_NOC_CON1_BUS_FWD_PHP_STALL_MASK        (0x1U << BUS_GRF_SOC_NOC_CON1_BUS_FWD_PHP_STALL_SHIFT)       /* 0x00000040 */
#define BUS_GRF_SOC_NOC_CON1_BUS_FWD_VIO_STALL_SHIFT       (7U)
#define BUS_GRF_SOC_NOC_CON1_BUS_FWD_VIO_STALL_MASK        (0x1U << BUS_GRF_SOC_NOC_CON1_BUS_FWD_VIO_STALL_SHIFT)       /* 0x00000080 */
#define BUS_GRF_SOC_NOC_CON1_GMAC_FWD_PHP_MID_STALL_SHIFT  (9U)
#define BUS_GRF_SOC_NOC_CON1_GMAC_FWD_PHP_MID_STALL_MASK   (0x1U << BUS_GRF_SOC_NOC_CON1_GMAC_FWD_PHP_MID_STALL_SHIFT)  /* 0x00000200 */
#define BUS_GRF_SOC_NOC_CON1_MMC_FWD_PHP_MID_STALL_SHIFT   (10U)
#define BUS_GRF_SOC_NOC_CON1_MMC_FWD_PHP_MID_STALL_MASK    (0x1U << BUS_GRF_SOC_NOC_CON1_MMC_FWD_PHP_MID_STALL_SHIFT)   /* 0x00000400 */
#define BUS_GRF_SOC_NOC_CON1_PCIE_FWD_BUS_STALL_SHIFT      (11U)
#define BUS_GRF_SOC_NOC_CON1_PCIE_FWD_BUS_STALL_MASK       (0x1U << BUS_GRF_SOC_NOC_CON1_PCIE_FWD_BUS_STALL_SHIFT)      /* 0x00000800 */
#define BUS_GRF_SOC_NOC_CON1_PHP_FWD_BUS_STALL_SHIFT       (12U)
#define BUS_GRF_SOC_NOC_CON1_PHP_FWD_BUS_STALL_MASK        (0x1U << BUS_GRF_SOC_NOC_CON1_PHP_FWD_BUS_STALL_SHIFT)       /* 0x00001000 */
#define BUS_GRF_SOC_NOC_CON1_PHP_MID_FWD_GMAC_STALL_SHIFT  (13U)
#define BUS_GRF_SOC_NOC_CON1_PHP_MID_FWD_GMAC_STALL_MASK   (0x1U << BUS_GRF_SOC_NOC_CON1_PHP_MID_FWD_GMAC_STALL_SHIFT)  /* 0x00002000 */
#define BUS_GRF_SOC_NOC_CON1_PHP_MID_FWD_MMCSTALL_SHIFT    (14U)
#define BUS_GRF_SOC_NOC_CON1_PHP_MID_FWD_MMCSTALL_MASK     (0x1U << BUS_GRF_SOC_NOC_CON1_PHP_MID_FWD_MMCSTALL_SHIFT)    /* 0x00004000 */
#define BUS_GRF_SOC_NOC_CON1_VIO_FWD_BUS_STALL_SHIFT       (15U)
#define BUS_GRF_SOC_NOC_CON1_VIO_FWD_BUS_STALL_MASK        (0x1U << BUS_GRF_SOC_NOC_CON1_VIO_FWD_BUS_STALL_SHIFT)       /* 0x00008000 */
/* SOC_NOC_CON2 */
#define BUS_GRF_SOC_NOC_CON2_OFFSET                        (0x538)
#define BUS_GRF_SOC_NOC_CON2_VPU_FWD_BUS_BUS_STALL_SHIFT   (0U)
#define BUS_GRF_SOC_NOC_CON2_VPU_FWD_BUS_BUS_STALL_MASK    (0x1U << BUS_GRF_SOC_NOC_CON2_VPU_FWD_BUS_BUS_STALL_SHIFT)   /* 0x00000001 */
#define BUS_GRF_SOC_NOC_CON2_PCIE_FWD_PCIE_USB_STALL_SHIFT (1U)
#define BUS_GRF_SOC_NOC_CON2_PCIE_FWD_PCIE_USB_STALL_MASK  (0x1U << BUS_GRF_SOC_NOC_CON2_PCIE_FWD_PCIE_USB_STALL_SHIFT) /* 0x00000002 */
#define BUS_GRF_SOC_NOC_CON2_PCIE_USB_FWD_PCIE_STALL_SHIFT (2U)
#define BUS_GRF_SOC_NOC_CON2_PCIE_USB_FWD_PCIE_STALL_MASK  (0x1U << BUS_GRF_SOC_NOC_CON2_PCIE_USB_FWD_PCIE_STALL_SHIFT) /* 0x00000004 */
#define BUS_GRF_SOC_NOC_CON2_BUS_FWD_VPU_STALL_SHIFT       (3U)
#define BUS_GRF_SOC_NOC_CON2_BUS_FWD_VPU_STALL_MASK        (0x1U << BUS_GRF_SOC_NOC_CON2_BUS_FWD_VPU_STALL_SHIFT)       /* 0x00000008 */
#define BUS_GRF_SOC_NOC_CON2_MSCH_SPLIT_SIZE_SHIFT         (14U)
#define BUS_GRF_SOC_NOC_CON2_MSCH_SPLIT_SIZE_MASK          (0x3U << BUS_GRF_SOC_NOC_CON2_MSCH_SPLIT_SIZE_SHIFT)         /* 0x0000C000 */
/* RAM_CON0 */
#define BUS_GRF_RAM_CON0_OFFSET                            (0x600)
#define BUS_GRF_RAM_CON0_SYSTEM_MEM_EMAS_SHIFT             (0U)
#define BUS_GRF_RAM_CON0_SYSTEM_MEM_EMAS_MASK              (0x3U << BUS_GRF_RAM_CON0_SYSTEM_MEM_EMAS_SHIFT)             /* 0x00000003 */
#define BUS_GRF_RAM_CON0_SYSTEM_MEM_EMA_SHIFT              (2U)
#define BUS_GRF_RAM_CON0_SYSTEM_MEM_EMA_MASK               (0x3U << BUS_GRF_RAM_CON0_SYSTEM_MEM_EMA_SHIFT)              /* 0x0000000C */
#define BUS_GRF_RAM_CON0_SYSTEM_MEM_EMAW_SHIFT             (4U)
#define BUS_GRF_RAM_CON0_SYSTEM_MEM_EMAW_MASK              (0x7U << BUS_GRF_RAM_CON0_SYSTEM_MEM_EMAW_SHIFT)             /* 0x00000070 */
#define BUS_GRF_RAM_CON0_NPU_MA_SAWL_SHIFT                 (8U)
#define BUS_GRF_RAM_CON0_NPU_MA_SAWL_MASK                  (0x1U << BUS_GRF_RAM_CON0_NPU_MA_SAWL_SHIFT)                 /* 0x00000100 */
#define BUS_GRF_RAM_CON0_NPU_MA_WL_SHIFT                   (9U)
#define BUS_GRF_RAM_CON0_NPU_MA_WL_MASK                    (0x1U << BUS_GRF_RAM_CON0_NPU_MA_WL_SHIFT)                   /* 0x00000200 */
#define BUS_GRF_RAM_CON0_NPU_MA_WRAS_SHIFT                 (10U)
#define BUS_GRF_RAM_CON0_NPU_MA_WRAS_MASK                  (0x1U << BUS_GRF_RAM_CON0_NPU_MA_WRAS_SHIFT)                 /* 0x00000400 */
#define BUS_GRF_RAM_CON0_NPU_MA_WRASD_SHIFT                (11U)
#define BUS_GRF_RAM_CON0_NPU_MA_WRASD_MASK                 (0x1U << BUS_GRF_RAM_CON0_NPU_MA_WRASD_SHIFT)                /* 0x00000800 */
#define BUS_GRF_RAM_CON0_NPU_MA_TPA_SHIFT                  (12U)
#define BUS_GRF_RAM_CON0_NPU_MA_TPA_MASK                   (0x1U << BUS_GRF_RAM_CON0_NPU_MA_TPA_SHIFT)                  /* 0x00001000 */
#define BUS_GRF_RAM_CON0_NPU_MA_TPB_SHIFT                  (13U)
#define BUS_GRF_RAM_CON0_NPU_MA_TPB_MASK                   (0x1U << BUS_GRF_RAM_CON0_NPU_MA_TPB_SHIFT)                  /* 0x00002000 */
/* RAM_CON1 */
#define BUS_GRF_RAM_CON1_OFFSET                            (0x604)
#define BUS_GRF_RAM_CON1_VIO_DP_EMAA_SHIFT                 (0U)
#define BUS_GRF_RAM_CON1_VIO_DP_EMAA_MASK                  (0x7U << BUS_GRF_RAM_CON1_VIO_DP_EMAA_SHIFT)                 /* 0x00000007 */
#define BUS_GRF_RAM_CON1_VIO_DP_EMAB_SHIFT                 (4U)
#define BUS_GRF_RAM_CON1_VIO_DP_EMAB_MASK                  (0x7U << BUS_GRF_RAM_CON1_VIO_DP_EMAB_SHIFT)                 /* 0x00000070 */
#define BUS_GRF_RAM_CON1_VIO_DP_EMASA_SHIFT                (7U)
#define BUS_GRF_RAM_CON1_VIO_DP_EMASA_MASK                 (0x1U << BUS_GRF_RAM_CON1_VIO_DP_EMASA_SHIFT)                /* 0x00000080 */
#define BUS_GRF_RAM_CON1_VIO_SP_EMAA_SHIFT                 (8U)
#define BUS_GRF_RAM_CON1_VIO_SP_EMAA_MASK                  (0x7U << BUS_GRF_RAM_CON1_VIO_SP_EMAA_SHIFT)                 /* 0x00000700 */
#define BUS_GRF_RAM_CON1_VIO_SP_EMAS_SHIFT                 (11U)
#define BUS_GRF_RAM_CON1_VIO_SP_EMAS_MASK                  (0x1U << BUS_GRF_RAM_CON1_VIO_SP_EMAS_SHIFT)                 /* 0x00000800 */
#define BUS_GRF_RAM_CON1_VIO_SP_EMAW_SHIFT                 (12U)
#define BUS_GRF_RAM_CON1_VIO_SP_EMAW_MASK                  (0x3U << BUS_GRF_RAM_CON1_VIO_SP_EMAW_SHIFT)                 /* 0x00003000 */
/* RAM_CON2 */
#define BUS_GRF_RAM_CON2_OFFSET                            (0x608)
#define BUS_GRF_RAM_CON2_GMAC_EMASA_SHIFT                  (0U)
#define BUS_GRF_RAM_CON2_GMAC_EMASA_MASK                   (0x1U << BUS_GRF_RAM_CON2_GMAC_EMASA_SHIFT)                  /* 0x00000001 */
#define BUS_GRF_RAM_CON2_GMAC_EMAA_0_SHIFT                 (1U)
#define BUS_GRF_RAM_CON2_GMAC_EMAA_0_MASK                  (0x1U << BUS_GRF_RAM_CON2_GMAC_EMAA_0_SHIFT)                 /* 0x00000002 */
#define BUS_GRF_RAM_CON2_GMAC_EMAA_1_SHIFT                 (2U)
#define BUS_GRF_RAM_CON2_GMAC_EMAA_1_MASK                  (0x1U << BUS_GRF_RAM_CON2_GMAC_EMAA_1_SHIFT)                 /* 0x00000004 */
#define BUS_GRF_RAM_CON2_GMAC_EMAB_0_SHIFT                 (3U)
#define BUS_GRF_RAM_CON2_GMAC_EMAB_0_MASK                  (0x1U << BUS_GRF_RAM_CON2_GMAC_EMAB_0_SHIFT)                 /* 0x00000008 */
#define BUS_GRF_RAM_CON2_GMAC_EMAB_1_SHIFT                 (4U)
#define BUS_GRF_RAM_CON2_GMAC_EMAB_1_MASK                  (0x1U << BUS_GRF_RAM_CON2_GMAC_EMAB_1_SHIFT)                 /* 0x00000010 */
#define BUS_GRF_RAM_CON2_GMAC_EMAB_2_SHIFT                 (5U)
#define BUS_GRF_RAM_CON2_GMAC_EMAB_2_MASK                  (0x1U << BUS_GRF_RAM_CON2_GMAC_EMAB_2_SHIFT)                 /* 0x00000020 */
#define BUS_GRF_RAM_CON2_GIC500_MA_SAWL_SHIFT              (6U)
#define BUS_GRF_RAM_CON2_GIC500_MA_SAWL_MASK               (0x1U << BUS_GRF_RAM_CON2_GIC500_MA_SAWL_SHIFT)              /* 0x00000040 */
#define BUS_GRF_RAM_CON2_GIC500_MA_WL_SHIFT                (7U)
#define BUS_GRF_RAM_CON2_GIC500_MA_WL_MASK                 (0x1U << BUS_GRF_RAM_CON2_GIC500_MA_WL_SHIFT)                /* 0x00000080 */
#define BUS_GRF_RAM_CON2_GIC500_MA_WRAS_SHIFT              (8U)
#define BUS_GRF_RAM_CON2_GIC500_MA_WRAS_MASK               (0x1U << BUS_GRF_RAM_CON2_GIC500_MA_WRAS_SHIFT)              /* 0x00000100 */
#define BUS_GRF_RAM_CON2_GIC500_MA_WRASD_SHIFT             (9U)
#define BUS_GRF_RAM_CON2_GIC500_MA_WRASD_MASK              (0x1U << BUS_GRF_RAM_CON2_GIC500_MA_WRASD_SHIFT)             /* 0x00000200 */
#define BUS_GRF_RAM_CON2_BUS_EMAS_SHIFT                    (10U)
#define BUS_GRF_RAM_CON2_BUS_EMAS_MASK                     (0x1U << BUS_GRF_RAM_CON2_BUS_EMAS_SHIFT)                    /* 0x00000400 */
#define BUS_GRF_RAM_CON2_BUS_EMA_SHIFT                     (11U)
#define BUS_GRF_RAM_CON2_BUS_EMA_MASK                      (0x7U << BUS_GRF_RAM_CON2_BUS_EMA_SHIFT)                     /* 0x00003800 */
#define BUS_GRF_RAM_CON2_BUS_EMAW_SHIFT                    (14U)
#define BUS_GRF_RAM_CON2_BUS_EMAW_MASK                     (0x3U << BUS_GRF_RAM_CON2_BUS_EMAW_SHIFT)                    /* 0x0000C000 */
/* RAM_CON3 */
#define BUS_GRF_RAM_CON3_OFFSET                            (0x60C)
#define BUS_GRF_RAM_CON3_BUS_EMAA_SHIFT                    (0U)
#define BUS_GRF_RAM_CON3_BUS_EMAA_MASK                     (0x7U << BUS_GRF_RAM_CON3_BUS_EMAA_SHIFT)                    /* 0x00000007 */
#define BUS_GRF_RAM_CON3_BUS_EMASA_SHIFT                   (3U)
#define BUS_GRF_RAM_CON3_BUS_EMASA_MASK                    (0x1U << BUS_GRF_RAM_CON3_BUS_EMASA_SHIFT)                   /* 0x00000008 */
#define BUS_GRF_RAM_CON3_BUS_EMAB_SHIFT                    (4U)
#define BUS_GRF_RAM_CON3_BUS_EMAB_MASK                     (0x7U << BUS_GRF_RAM_CON3_BUS_EMAB_SHIFT)                    /* 0x00000070 */
#define BUS_GRF_RAM_CON3_AUDIO_MA_SAWL_SHIFT               (8U)
#define BUS_GRF_RAM_CON3_AUDIO_MA_SAWL_MASK                (0x1U << BUS_GRF_RAM_CON3_AUDIO_MA_SAWL_SHIFT)               /* 0x00000100 */
#define BUS_GRF_RAM_CON3_AUDIO_MA_WL_SHIFT                 (9U)
#define BUS_GRF_RAM_CON3_AUDIO_MA_WL_MASK                  (0x1U << BUS_GRF_RAM_CON3_AUDIO_MA_WL_SHIFT)                 /* 0x00000200 */
#define BUS_GRF_RAM_CON3_AUDIO_MA_WRAS_SHIFT               (10U)
#define BUS_GRF_RAM_CON3_AUDIO_MA_WRAS_MASK                (0x1U << BUS_GRF_RAM_CON3_AUDIO_MA_WRAS_SHIFT)               /* 0x00000400 */
#define BUS_GRF_RAM_CON3_AUDIO_MA_WRASD_SHIFT              (11U)
#define BUS_GRF_RAM_CON3_AUDIO_MA_WRASD_MASK               (0x1U << BUS_GRF_RAM_CON3_AUDIO_MA_WRASD_SHIFT)              /* 0x00000800 */
/* RAM_CON4 */
#define BUS_GRF_RAM_CON4_OFFSET                            (0x610)
#define BUS_GRF_RAM_CON4_VPU_EMA_SHIFT                     (0U)
#define BUS_GRF_RAM_CON4_VPU_EMA_MASK                      (0x7U << BUS_GRF_RAM_CON4_VPU_EMA_SHIFT)                     /* 0x00000007 */
#define BUS_GRF_RAM_CON4_VPU_EMAA_SHIFT                    (3U)
#define BUS_GRF_RAM_CON4_VPU_EMAA_MASK                     (0x7U << BUS_GRF_RAM_CON4_VPU_EMAA_SHIFT)                    /* 0x00000038 */
#define BUS_GRF_RAM_CON4_VPU_EMAB_SHIFT                    (6U)
#define BUS_GRF_RAM_CON4_VPU_EMAB_MASK                     (0x7U << BUS_GRF_RAM_CON4_VPU_EMAB_SHIFT)                    /* 0x000001C0 */
#define BUS_GRF_RAM_CON4_VPU_EMASA_SHIFT                   (9U)
#define BUS_GRF_RAM_CON4_VPU_EMASA_MASK                    (0x1U << BUS_GRF_RAM_CON4_VPU_EMASA_SHIFT)                   /* 0x00000200 */
#define BUS_GRF_RAM_CON4_VPU_EMAS_SHIFT                    (10U)
#define BUS_GRF_RAM_CON4_VPU_EMAS_MASK                     (0x1U << BUS_GRF_RAM_CON4_VPU_EMAS_SHIFT)                    /* 0x00000400 */
#define BUS_GRF_RAM_CON4_VPU_EMAW_SHIFT                    (11U)
#define BUS_GRF_RAM_CON4_VPU_EMAW_MASK                     (0x7U << BUS_GRF_RAM_CON4_VPU_EMAW_SHIFT)                    /* 0x00003800 */
#define BUS_GRF_RAM_CON4_VPU_EMA_ROM_SHIFT                 (13U)
#define BUS_GRF_RAM_CON4_VPU_EMA_ROM_MASK                  (0x7U << BUS_GRF_RAM_CON4_VPU_EMA_ROM_SHIFT)                 /* 0x0000E000 */
/* NPUPVTM_CON0 */
#define BUS_GRF_NPUPVTM_CON0_OFFSET                        (0x780)
#define BUS_GRF_NPUPVTM_CON0_NPUPVTM_START_SHIFT           (0U)
#define BUS_GRF_NPUPVTM_CON0_NPUPVTM_START_MASK            (0x1U << BUS_GRF_NPUPVTM_CON0_NPUPVTM_START_SHIFT)           /* 0x00000001 */
#define BUS_GRF_NPUPVTM_CON0_NPUPVTM_OSC_EN_SHIFT          (1U)
#define BUS_GRF_NPUPVTM_CON0_NPUPVTM_OSC_EN_MASK           (0x1U << BUS_GRF_NPUPVTM_CON0_NPUPVTM_OSC_EN_SHIFT)          /* 0x00000002 */
#define BUS_GRF_NPUPVTM_CON0_NPUPVTM_OSC_SEL_SHIFT         (2U)
#define BUS_GRF_NPUPVTM_CON0_NPUPVTM_OSC_SEL_MASK          (0x7U << BUS_GRF_NPUPVTM_CON0_NPUPVTM_OSC_SEL_SHIFT)         /* 0x0000001C */
/* NPUPVTM_CON1 */
#define BUS_GRF_NPUPVTM_CON1_OFFSET                        (0x784)
#define BUS_GRF_NPUPVTM_CON1_NPUPVTM_CAL_CNT_SHIFT         (0U)
#define BUS_GRF_NPUPVTM_CON1_NPUPVTM_CAL_CNT_MASK          (0xFFFFFFFFU << BUS_GRF_NPUPVTM_CON1_NPUPVTM_CAL_CNT_SHIFT)  /* 0xFFFFFFFF */
/* NPUPVTM_STATUS0 */
#define BUS_GRF_NPUPVTM_STATUS0_OFFSET                     (0x788)
#define BUS_GRF_NPUPVTM_STATUS0_NPUPVTM_FREQ_DONE_SHIFT    (0U)
#define BUS_GRF_NPUPVTM_STATUS0_NPUPVTM_FREQ_DONE_MASK     (0x1U << BUS_GRF_NPUPVTM_STATUS0_NPUPVTM_FREQ_DONE_SHIFT)    /* 0x00000001 */
/* NPUPVTM_STATUS1 */
#define BUS_GRF_NPUPVTM_STATUS1_OFFSET                     (0x78C)
#define BUS_GRF_NPUPVTM_STATUS1_NPUPVTM_FREQ_CNT_SHIFT     (0U)
#define BUS_GRF_NPUPVTM_STATUS1_NPUPVTM_FREQ_CNT_MASK      (0xFFFFFFFFU << BUS_GRF_NPUPVTM_STATUS1_NPUPVTM_FREQ_CNT_SHIFT) /* 0xFFFFFFFF */
/* CHIP_ID */
#define BUS_GRF_CHIP_ID_OFFSET                             (0x800)
#define BUS_GRF_CHIP_ID_CHIP_ID_SHIFT                      (0U)
#define BUS_GRF_CHIP_ID_CHIP_ID_MASK                       (0xFFFFFFFFU << BUS_GRF_CHIP_ID_CHIP_ID_SHIFT)               /* 0xFFFFFFFF */
/* MAC_CON0 */
#define BUS_GRF_MAC_CON0_OFFSET                            (0x900)
#define BUS_GRF_MAC_CON0_GMAC2IO_CLK_TX_DL_CFG_SHIFT       (0U)
#define BUS_GRF_MAC_CON0_GMAC2IO_CLK_TX_DL_CFG_MASK        (0xFFU << BUS_GRF_MAC_CON0_GMAC2IO_CLK_TX_DL_CFG_SHIFT)      /* 0x000000FF */
#define BUS_GRF_MAC_CON0_GMAC2IO_CLK_RX_DL_CFG_SHIFT       (8U)
#define BUS_GRF_MAC_CON0_GMAC2IO_CLK_RX_DL_CFG_MASK        (0xFFU << BUS_GRF_MAC_CON0_GMAC2IO_CLK_RX_DL_CFG_SHIFT)      /* 0x0000FF00 */
/* MAC_CON1 */
#define BUS_GRF_MAC_CON1_OFFSET                            (0x904)
#define BUS_GRF_MAC_CON1_GMAC2IO_TXCLK_DLY_ENA_SHIFT       (0U)
#define BUS_GRF_MAC_CON1_GMAC2IO_TXCLK_DLY_ENA_MASK        (0x1U << BUS_GRF_MAC_CON1_GMAC2IO_TXCLK_DLY_ENA_SHIFT)       /* 0x00000001 */
#define BUS_GRF_MAC_CON1_GMAC2IO_RXCLK_DLY_ENA_SHIFT       (1U)
#define BUS_GRF_MAC_CON1_GMAC2IO_RXCLK_DLY_ENA_MASK        (0x1U << BUS_GRF_MAC_CON1_GMAC2IO_RXCLK_DLY_ENA_SHIFT)       /* 0x00000002 */
#define BUS_GRF_MAC_CON1_GMAC2IO_MAC_SPEED_SHIFT           (2U)
#define BUS_GRF_MAC_CON1_GMAC2IO_MAC_SPEED_MASK            (0x1U << BUS_GRF_MAC_CON1_GMAC2IO_MAC_SPEED_SHIFT)           /* 0x00000004 */
#define BUS_GRF_MAC_CON1_GMAC2IO_FLOWCTRL_SHIFT            (3U)
#define BUS_GRF_MAC_CON1_GMAC2IO_FLOWCTRL_MASK             (0x1U << BUS_GRF_MAC_CON1_GMAC2IO_FLOWCTRL_SHIFT)            /* 0x00000008 */
#define BUS_GRF_MAC_CON1_GMAC2IO_PHY_INTF_SEL_SHIFT        (4U)
#define BUS_GRF_MAC_CON1_GMAC2IO_PHY_INTF_SEL_MASK         (0x7U << BUS_GRF_MAC_CON1_GMAC2IO_PHY_INTF_SEL_SHIFT)        /* 0x00000070 */

/**************************************USB2PHY_GRF***************************************/
/* REG0 */
#define USB2PHY_GRF_REG0_USBPHY_REG0_SHIFT                 (0U)
#define USB2PHY_GRF_REG0_USBPHY_REG0_MASK                  (0xFFFFU << USB2PHY_GRF_REG0_USBPHY_REG0_SHIFT)              /* 0x0000FFFF */
/* REG1 */
#define USB2PHY_GRF_REG1_USBPHY_REG1_SHIFT                 (0U)
#define USB2PHY_GRF_REG1_USBPHY_REG1_MASK                  (0xFFFFU << USB2PHY_GRF_REG1_USBPHY_REG1_SHIFT)              /* 0x0000FFFF */
/* REG2 */
#define USB2PHY_GRF_REG2_USBPHY_REG2_SHIFT                 (0U)
#define USB2PHY_GRF_REG2_USBPHY_REG2_MASK                  (0xFFFFU << USB2PHY_GRF_REG2_USBPHY_REG2_SHIFT)              /* 0x0000FFFF */
/* REG3 */
#define USB2PHY_GRF_REG3_USBPHY_REG3_SHIFT                 (0U)
#define USB2PHY_GRF_REG3_USBPHY_REG3_MASK                  (0xFFFFU << USB2PHY_GRF_REG3_USBPHY_REG3_SHIFT)              /* 0x0000FFFF */
/* REG4 */
#define USB2PHY_GRF_REG4_USBPHY_REG4_SHIFT                 (0U)
#define USB2PHY_GRF_REG4_USBPHY_REG4_MASK                  (0xFFFFU << USB2PHY_GRF_REG4_USBPHY_REG4_SHIFT)              /* 0x0000FFFF */
/* REG5 */
#define USB2PHY_GRF_REG5_USBPHY_REG5_SHIFT                 (0U)
#define USB2PHY_GRF_REG5_USBPHY_REG5_MASK                  (0xFFFFU << USB2PHY_GRF_REG5_USBPHY_REG5_SHIFT)              /* 0x0000FFFF */
/* REG6 */
#define USB2PHY_GRF_REG6_USBPHY_REG6_SHIFT                 (0U)
#define USB2PHY_GRF_REG6_USBPHY_REG6_MASK                  (0xFFFFU << USB2PHY_GRF_REG6_USBPHY_REG6_SHIFT)              /* 0x0000FFFF */
/* REG7 */
#define USB2PHY_GRF_REG7_USBPHY_REG7_SHIFT                 (0U)
#define USB2PHY_GRF_REG7_USBPHY_REG7_MASK                  (0xFFFFU << USB2PHY_GRF_REG7_USBPHY_REG7_SHIFT)              /* 0x0000FFFF */
/* REG8 */
#define USB2PHY_GRF_REG8_USBPHY_REG8_SHIFT                 (0U)
#define USB2PHY_GRF_REG8_USBPHY_REG8_MASK                  (0xFFFFU << USB2PHY_GRF_REG8_USBPHY_REG8_SHIFT)              /* 0x0000FFFF */
/* REG9 */
#define USB2PHY_GRF_REG9_USBPHY_REG9_SHIFT                 (0U)
#define USB2PHY_GRF_REG9_USBPHY_REG9_MASK                  (0xFFFFU << USB2PHY_GRF_REG9_USBPHY_REG9_SHIFT)              /* 0x0000FFFF */
/* REG10 */
#define USB2PHY_GRF_REG10_USBPHY_REG10_SHIFT               (0U)
#define USB2PHY_GRF_REG10_USBPHY_REG10_MASK                (0xFFFFU << USB2PHY_GRF_REG10_USBPHY_REG10_SHIFT)            /* 0x0000FFFF */
/* REG11 */
#define USB2PHY_GRF_REG11_USBPHY_REG11_SHIFT               (0U)
#define USB2PHY_GRF_REG11_USBPHY_REG11_MASK                (0xFFFFU << USB2PHY_GRF_REG11_USBPHY_REG11_SHIFT)            /* 0x0000FFFF */
/* REG12 */
#define USB2PHY_GRF_REG12_USBPHY_REG12_SHIFT               (0U)
#define USB2PHY_GRF_REG12_USBPHY_REG12_MASK                (0xFFFFU << USB2PHY_GRF_REG12_USBPHY_REG12_SHIFT)            /* 0x0000FFFF */
/* REG13 */
#define USB2PHY_GRF_REG13_USBPHY_REG13_SHIFT               (0U)
#define USB2PHY_GRF_REG13_USBPHY_REG13_MASK                (0xFFFFU << USB2PHY_GRF_REG13_USBPHY_REG13_SHIFT)            /* 0x0000FFFF */
/* REG14 */
#define USB2PHY_GRF_REG14_USBPHY_REG14_SHIFT               (0U)
#define USB2PHY_GRF_REG14_USBPHY_REG14_MASK                (0xFFFFU << USB2PHY_GRF_REG14_USBPHY_REG14_SHIFT)            /* 0x0000FFFF */
/* REG15 */
#define USB2PHY_GRF_REG15_USBPHY_REG15_SHIFT               (0U)
#define USB2PHY_GRF_REG15_USBPHY_REG15_MASK                (0xFFFFU << USB2PHY_GRF_REG15_USBPHY_REG15_SHIFT)            /* 0x0000FFFF */
/* REG16 */
#define USB2PHY_GRF_REG16_USBPHY_REG16_SHIFT               (0U)
#define USB2PHY_GRF_REG16_USBPHY_REG16_MASK                (0xFFFFU << USB2PHY_GRF_REG16_USBPHY_REG16_SHIFT)            /* 0x0000FFFF */
/* REG17 */
#define USB2PHY_GRF_REG17_USBPHY_REG17_SHIFT               (0U)
#define USB2PHY_GRF_REG17_USBPHY_REG17_MASK                (0xFFFFU << USB2PHY_GRF_REG17_USBPHY_REG17_SHIFT)            /* 0x0000FFFF */
/* REG18 */
#define USB2PHY_GRF_REG18_USBPHY_REG18_SHIFT               (0U)
#define USB2PHY_GRF_REG18_USBPHY_REG18_MASK                (0xFFFFU << USB2PHY_GRF_REG18_USBPHY_REG18_SHIFT)            /* 0x0000FFFF */
/* REG19 */
#define USB2PHY_GRF_REG19_USBPHY_REG19_SHIFT               (0U)
#define USB2PHY_GRF_REG19_USBPHY_REG19_MASK                (0xFFFFU << USB2PHY_GRF_REG19_USBPHY_REG19_SHIFT)            /* 0x0000FFFF */
/* REG20 */
#define USB2PHY_GRF_REG20_USBPHY_REG20_SHIFT               (0U)
#define USB2PHY_GRF_REG20_USBPHY_REG20_MASK                (0xFFFFU << USB2PHY_GRF_REG20_USBPHY_REG20_SHIFT)            /* 0x0000FFFF */
/* REG21 */
#define USB2PHY_GRF_REG21_USBPHY_REG21_SHIFT               (0U)
#define USB2PHY_GRF_REG21_USBPHY_REG21_MASK                (0xFFFFU << USB2PHY_GRF_REG21_USBPHY_REG21_SHIFT)            /* 0x0000FFFF */
/* REG22 */
#define USB2PHY_GRF_REG22_USBPHY_REG22_SHIFT               (0U)
#define USB2PHY_GRF_REG22_USBPHY_REG22_MASK                (0xFFFFU << USB2PHY_GRF_REG22_USBPHY_REG22_SHIFT)            /* 0x0000FFFF */
/* REG23 */
#define USB2PHY_GRF_REG23_USBPHY_REG23_SHIFT               (0U)
#define USB2PHY_GRF_REG23_USBPHY_REG23_MASK                (0xFFFFU << USB2PHY_GRF_REG23_USBPHY_REG23_SHIFT)            /* 0x0000FFFF */
/* CON0 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_SEL_SHIFT             (0U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_SEL_MASK              (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_SEL_SHIFT)             /* 0x00000001 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_SUSPEND_N_SHIFT       (1U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_SUSPEND_N_MASK        (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_SUSPEND_N_SHIFT)       /* 0x00000002 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_OPMODE_SHIFT          (2U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_OPMODE_MASK           (0x3U << USB2PHY_GRF_CON0_USBOTG_UTMI_OPMODE_SHIFT)          /* 0x0000000C */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_XCVRSELECT_SHIFT      (4U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_XCVRSELECT_MASK       (0x3U << USB2PHY_GRF_CON0_USBOTG_UTMI_XCVRSELECT_SHIFT)      /* 0x00000030 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_TERMSELECT_SHIFT      (6U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_TERMSELECT_MASK       (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_TERMSELECT_SHIFT)      /* 0x00000040 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_SHIFT      (7U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_MASK       (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_DPPULLDOWN_SHIFT)      /* 0x00000080 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_SHIFT      (8U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_MASK       (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_DMPULLDOWN_SHIFT)      /* 0x00000100 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_SHIFT       (9U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_MASK        (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_IDDIG_SEL_SHIFT)       /* 0x00000200 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_IDDIG_SHIFT           (10U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_IDDIG_MASK            (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_IDDIG_SHIFT)           /* 0x00000400 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_IDPULLUP_SHIFT        (11U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_IDPULLUP_MASK         (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_IDPULLUP_SHIFT)        /* 0x00000800 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_CHRGVBUS_SHIFT        (12U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_CHRGVBUS_MASK         (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_CHRGVBUS_SHIFT)        /* 0x00001000 */
#define USB2PHY_GRF_CON0_USBOTG_UTMI_DISCHRGVBUS_SHIFT     (13U)
#define USB2PHY_GRF_CON0_USBOTG_UTMI_DISCHRGVBUS_MASK      (0x1U << USB2PHY_GRF_CON0_USBOTG_UTMI_DISCHRGVBUS_SHIFT)     /* 0x00002000 */
/* CON1 */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_SEL_SHIFT            (0U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_SEL_MASK             (0x1U << USB2PHY_GRF_CON1_USBHOST_UTMI_SEL_SHIFT)            /* 0x00000001 */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_SUSPEND_N_SHIFT      (1U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_SUSPEND_N_MASK       (0x1U << USB2PHY_GRF_CON1_USBHOST_UTMI_SUSPEND_N_SHIFT)      /* 0x00000002 */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_OPMODE_SHIFT         (2U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_OPMODE_MASK          (0x3U << USB2PHY_GRF_CON1_USBHOST_UTMI_OPMODE_SHIFT)         /* 0x0000000C */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_XCVRSELECT_SHIFT     (4U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_XCVRSELECT_MASK      (0x3U << USB2PHY_GRF_CON1_USBHOST_UTMI_XCVRSELECT_SHIFT)     /* 0x00000030 */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_TERMSELECT_SHIFT     (6U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_TERMSELECT_MASK      (0x1U << USB2PHY_GRF_CON1_USBHOST_UTMI_TERMSELECT_SHIFT)     /* 0x00000040 */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_SHIFT     (7U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_MASK      (0x1U << USB2PHY_GRF_CON1_USBHOST_UTMI_DPPULLDOWN_SHIFT)     /* 0x00000080 */
#define USB2PHY_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_SHIFT     (8U)
#define USB2PHY_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_MASK      (0x1U << USB2PHY_GRF_CON1_USBHOST_UTMI_DMPULLDOWN_SHIFT)     /* 0x00000100 */
/* CON2 */
#define USB2PHY_GRF_CON2_USBOTG_DISABLE_0_SHIFT            (0U)
#define USB2PHY_GRF_CON2_USBOTG_DISABLE_0_MASK             (0x1U << USB2PHY_GRF_CON2_USBOTG_DISABLE_0_SHIFT)            /* 0x00000001 */
#define USB2PHY_GRF_CON2_USBOTG_DISABLE_1_SHIFT            (1U)
#define USB2PHY_GRF_CON2_USBOTG_DISABLE_1_MASK             (0x1U << USB2PHY_GRF_CON2_USBOTG_DISABLE_1_SHIFT)            /* 0x00000002 */
#define USB2PHY_GRF_CON2_BYPASSDMEN_USBOTG_SHIFT           (2U)
#define USB2PHY_GRF_CON2_BYPASSDMEN_USBOTG_MASK            (0x1U << USB2PHY_GRF_CON2_BYPASSDMEN_USBOTG_SHIFT)           /* 0x00000004 */
#define USB2PHY_GRF_CON2_BYPASSSEL_USBOTG_SHIFT            (3U)
#define USB2PHY_GRF_CON2_BYPASSSEL_USBOTG_MASK             (0x1U << USB2PHY_GRF_CON2_BYPASSSEL_USBOTG_SHIFT)            /* 0x00000008 */
#define USB2PHY_GRF_CON2_USBPHY_COMMONONN_SHIFT            (4U)
#define USB2PHY_GRF_CON2_USBPHY_COMMONONN_MASK             (0x1U << USB2PHY_GRF_CON2_USBPHY_COMMONONN_SHIFT)            /* 0x00000010 */
#define USB2PHY_GRF_CON2_IDP_SINK_EN_USBOTG_SHIFT          (7U)
#define USB2PHY_GRF_CON2_IDP_SINK_EN_USBOTG_MASK           (0x1U << USB2PHY_GRF_CON2_IDP_SINK_EN_USBOTG_SHIFT)          /* 0x00000080 */
#define USB2PHY_GRF_CON2_IDM_SINK_EN_USBOTG_SHIFT          (8U)
#define USB2PHY_GRF_CON2_IDM_SINK_EN_USBOTG_MASK           (0x1U << USB2PHY_GRF_CON2_IDM_SINK_EN_USBOTG_SHIFT)          /* 0x00000100 */
#define USB2PHY_GRF_CON2_IDP_SRC_EN_USBOTG_SHIFT           (9U)
#define USB2PHY_GRF_CON2_IDP_SRC_EN_USBOTG_MASK            (0x1U << USB2PHY_GRF_CON2_IDP_SRC_EN_USBOTG_SHIFT)           /* 0x00000200 */
#define USB2PHY_GRF_CON2_RDM_PDWN_EN_USBOTG_SHIFT          (10U)
#define USB2PHY_GRF_CON2_RDM_PDWN_EN_USBOTG_MASK           (0x1U << USB2PHY_GRF_CON2_RDM_PDWN_EN_USBOTG_SHIFT)          /* 0x00000400 */
#define USB2PHY_GRF_CON2_VDP_SRC_EN_USBOTG_SHIFT           (11U)
#define USB2PHY_GRF_CON2_VDP_SRC_EN_USBOTG_MASK            (0x1U << USB2PHY_GRF_CON2_VDP_SRC_EN_USBOTG_SHIFT)           /* 0x00000800 */
#define USB2PHY_GRF_CON2_VDM_SRC_EN_USBOTG_SHIFT           (12U)
#define USB2PHY_GRF_CON2_VDM_SRC_EN_USBOTG_MASK            (0x1U << USB2PHY_GRF_CON2_VDM_SRC_EN_USBOTG_SHIFT)           /* 0x00001000 */
/* CON3 */
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DRVVBUS_SHIFT        (0U)
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DRVVBUS_MASK         (0x1U << USB2PHY_GRF_CON3_USBHOST_UTMI_DRVVBUS_SHIFT)        /* 0x00000001 */
#define USB2PHY_GRF_CON3_USBHOST_UTMI_CHRGVBUS_SHIFT       (1U)
#define USB2PHY_GRF_CON3_USBHOST_UTMI_CHRGVBUS_MASK        (0x1U << USB2PHY_GRF_CON3_USBHOST_UTMI_CHRGVBUS_SHIFT)       /* 0x00000002 */
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_SHIFT    (2U)
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_MASK     (0x1U << USB2PHY_GRF_CON3_USBHOST_UTMI_DISCHRGVBUS_SHIFT)    /* 0x00000004 */
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_SHIFT     (3U)
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_MASK      (0x1U << USB2PHY_GRF_CON3_USBHOST_UTMI_DPPULLDOWN_SHIFT)     /* 0x00000008 */
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_SHIFT     (4U)
#define USB2PHY_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_MASK      (0x1U << USB2PHY_GRF_CON3_USBHOST_UTMI_DMPULLDOWN_SHIFT)     /* 0x00000010 */
#define USB2PHY_GRF_CON3_USBHOST_UTMI_IDPULLUP_SHIFT       (5U)
#define USB2PHY_GRF_CON3_USBHOST_UTMI_IDPULLUP_MASK        (0x1U << USB2PHY_GRF_CON3_USBHOST_UTMI_IDPULLUP_SHIFT)       /* 0x00000020 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_SHIFT     (6U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_MASK      (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_FS_XVER_OWN_SHIFT)     /* 0x00000040 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_OE_SHIFT           (7U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_OE_MASK            (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_FS_OE_SHIFT)           /* 0x00000080 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_DATA_SHIFT         (8U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_DATA_MASK          (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_FS_DATA_SHIFT)         /* 0x00000100 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_SE0_SHIFT          (9U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_FS_SE0_MASK           (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_FS_SE0_SHIFT)          /* 0x00000200 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_SHIFT     (10U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_MASK      (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SEL_SHIFT)     /* 0x00000400 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SHIFT         (11U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_MASK          (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_DRVVBUS_SHIFT)         /* 0x00000800 */
#define USB2PHY_GRF_CON3_USBOTG_UTMI_CHRGVBUS_SHIFT        (13U)
#define USB2PHY_GRF_CON3_USBOTG_UTMI_CHRGVBUS_MASK         (0x1U << USB2PHY_GRF_CON3_USBOTG_UTMI_CHRGVBUS_SHIFT)        /* 0x00002000 */
#define USB2PHY_GRF_CON3_USBPHY_HOSTPORT_WAKEUP_IRQ_EN_SHIFT (15U)
#define USB2PHY_GRF_CON3_USBPHY_HOSTPORT_WAKEUP_IRQ_EN_MASK (0x1U << USB2PHY_GRF_CON3_USBPHY_HOSTPORT_WAKEUP_IRQ_EN_SHIFT) /* 0x00008000 */
/* INT_MASK */
#define USB2PHY_GRF_INT_MASK_OTG0_LINESTATE_IRQ_EN_SHIFT   (0U)
#define USB2PHY_GRF_INT_MASK_OTG0_LINESTATE_IRQ_EN_MASK    (0x1U << USB2PHY_GRF_INT_MASK_OTG0_LINESTATE_IRQ_EN_SHIFT)   /* 0x00000001 */
#define USB2PHY_GRF_INT_MASK_HOST0_LINESTATE_IRQ_EN_SHIFT  (1U)
#define USB2PHY_GRF_INT_MASK_HOST0_LINESTATE_IRQ_EN_MASK   (0x1U << USB2PHY_GRF_INT_MASK_HOST0_LINESTATE_IRQ_EN_SHIFT)  /* 0x00000002 */
#define USB2PHY_GRF_INT_MASK_OTG0_BVALID_IRQ_EN_SHIFT      (2U)
#define USB2PHY_GRF_INT_MASK_OTG0_BVALID_IRQ_EN_MASK       (0x3U << USB2PHY_GRF_INT_MASK_OTG0_BVALID_IRQ_EN_SHIFT)      /* 0x0000000C */
#define USB2PHY_GRF_INT_MASK_OTG0_ID_IRQ_EN_SHIFT          (4U)
#define USB2PHY_GRF_INT_MASK_OTG0_ID_IRQ_EN_MASK           (0x3U << USB2PHY_GRF_INT_MASK_OTG0_ID_IRQ_EN_SHIFT)          /* 0x00000030 */
#define USB2PHY_GRF_INT_MASK_OTG0_DISCONNECT_IRQ_EN_SHIFT  (6U)
#define USB2PHY_GRF_INT_MASK_OTG0_DISCONNECT_IRQ_EN_MASK   (0x3U << USB2PHY_GRF_INT_MASK_OTG0_DISCONNECT_IRQ_EN_SHIFT)  /* 0x000000C0 */
#define USB2PHY_GRF_INT_MASK_HOST0_DISCONNECT_IRQ_EN_SHIFT (8U)
#define USB2PHY_GRF_INT_MASK_HOST0_DISCONNECT_IRQ_EN_MASK  (0x3U << USB2PHY_GRF_INT_MASK_HOST0_DISCONNECT_IRQ_EN_SHIFT) /* 0x00000300 */
/* INT_STATUS */
#define USB2PHY_GRF_INT_STATUS_OTG0_LINESTATE_IRQ_SHIFT    (0U)
#define USB2PHY_GRF_INT_STATUS_OTG0_LINESTATE_IRQ_MASK     (0x1U << USB2PHY_GRF_INT_STATUS_OTG0_LINESTATE_IRQ_SHIFT)    /* 0x00000001 */
#define USB2PHY_GRF_INT_STATUS_HOST0_LINESTATE_IRQ_SHIFT   (1U)
#define USB2PHY_GRF_INT_STATUS_HOST0_LINESTATE_IRQ_MASK    (0x1U << USB2PHY_GRF_INT_STATUS_HOST0_LINESTATE_IRQ_SHIFT)   /* 0x00000002 */
#define USB2PHY_GRF_INT_STATUS_OTG0_BVALID_IRQ_SHIFT       (2U)
#define USB2PHY_GRF_INT_STATUS_OTG0_BVALID_IRQ_MASK        (0x3U << USB2PHY_GRF_INT_STATUS_OTG0_BVALID_IRQ_SHIFT)       /* 0x0000000C */
#define USB2PHY_GRF_INT_STATUS_OTG0_ID_IRQ_SHIFT           (4U)
#define USB2PHY_GRF_INT_STATUS_OTG0_ID_IRQ_MASK            (0x3U << USB2PHY_GRF_INT_STATUS_OTG0_ID_IRQ_SHIFT)           /* 0x00000030 */
#define USB2PHY_GRF_INT_STATUS_OTG0_DISCONNECT_IRQ_SHIFT   (6U)
#define USB2PHY_GRF_INT_STATUS_OTG0_DISCONNECT_IRQ_MASK    (0x3U << USB2PHY_GRF_INT_STATUS_OTG0_DISCONNECT_IRQ_SHIFT)   /* 0x000000C0 */
#define USB2PHY_GRF_INT_STATUS_HOST0_DISCONNECT_IRQ_SHIFT  (8U)
#define USB2PHY_GRF_INT_STATUS_HOST0_DISCONNECT_IRQ_MASK   (0x3U << USB2PHY_GRF_INT_STATUS_HOST0_DISCONNECT_IRQ_SHIFT)  /* 0x00000300 */
/* INT_STATUS_CLR */
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_SHIFT (0U)
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_MASK (0x1U << USB2PHY_GRF_INT_STATUS_CLR_OTG0_LINESTATE_IRQ_CLR_SHIFT) /* 0x00000001 */
#define USB2PHY_GRF_INT_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_SHIFT (1U)
#define USB2PHY_GRF_INT_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_MASK (0x1U << USB2PHY_GRF_INT_STATUS_CLR_HOST0_LINESTATE_IRQ_CLR_SHIFT) /* 0x00000002 */
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_BVALID_IRQ_CLR_SHIFT (2U)
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_BVALID_IRQ_CLR_MASK (0x3U << USB2PHY_GRF_INT_STATUS_CLR_OTG0_BVALID_IRQ_CLR_SHIFT) /* 0x0000000C */
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_ID_IRQ_CLR_SHIFT   (4U)
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_ID_IRQ_CLR_MASK    (0x3U << USB2PHY_GRF_INT_STATUS_CLR_OTG0_ID_IRQ_CLR_SHIFT)   /* 0x00000030 */
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_DISCONNECT_IRQ_CLR_SHIFT (6U)
#define USB2PHY_GRF_INT_STATUS_CLR_OTG0_DISCONNECT_IRQ_CLR_MASK (0x3U << USB2PHY_GRF_INT_STATUS_CLR_OTG0_DISCONNECT_IRQ_CLR_SHIFT) /* 0x000000C0 */
#define USB2PHY_GRF_INT_STATUS_CLR_HOST0_DISCONNECT_IRQ_CLR_SHIFT (8U)
#define USB2PHY_GRF_INT_STATUS_CLR_HOST0_DISCONNECT_IRQ_CLR_MASK (0x3U << USB2PHY_GRF_INT_STATUS_CLR_HOST0_DISCONNECT_IRQ_CLR_SHIFT) /* 0x00000300 */
/* STATUS */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_VPI_SHIFT           (0U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_VPI_MASK            (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_VPI_SHIFT)           /* 0x00000001 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_VMI_SHIFT           (1U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_VMI_MASK            (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_VMI_SHIFT)           /* 0x00000002 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_VBUSVALID_SHIFT     (2U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_VBUSVALID_MASK      (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_VBUSVALID_SHIFT)     /* 0x00000004 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_SESSEND_SHIFT       (3U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_SESSEND_MASK        (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_SESSEND_SHIFT)       /* 0x00000008 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_LINESTATE_SHIFT     (4U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_LINESTATE_MASK      (0x3U << USB2PHY_GRF_STATUS_USBOTG_UTMI_LINESTATE_SHIFT)     /* 0x00000030 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_IDDIG_SHIFT         (6U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_IDDIG_MASK          (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_IDDIG_SHIFT)         /* 0x00000040 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_SHIFT (7U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_MASK (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_HOSTDISCONNECT_SHIFT) /* 0x00000080 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_SHIFT   (8U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_MASK    (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_FS_XVER_OWN_SHIFT)   /* 0x00000100 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_BVALID_SHIFT        (9U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_BVALID_MASK         (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_BVALID_SHIFT)        /* 0x00000200 */
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_AVALID_SHIFT        (10U)
#define USB2PHY_GRF_STATUS_USBOTG_UTMI_AVALID_MASK         (0x1U << USB2PHY_GRF_STATUS_USBOTG_UTMI_AVALID_SHIFT)        /* 0x00000400 */
#define USB2PHY_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_SHIFT      (11U)
#define USB2PHY_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_MASK       (0x1U << USB2PHY_GRF_STATUS_USBOTG_PHY_LS_FS_RCV_SHIFT)      /* 0x00000800 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_VPI_SHIFT          (12U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_VPI_MASK           (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_VPI_SHIFT)          /* 0x00001000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_VMI_SHIFT          (13U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_VMI_MASK           (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_VMI_SHIFT)          /* 0x00002000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_VBUSVALID_SHIFT    (14U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_VBUSVALID_MASK     (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_VBUSVALID_SHIFT)    /* 0x00004000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_SESSEND_SHIFT      (15U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_SESSEND_MASK       (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_SESSEND_SHIFT)      /* 0x00008000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_LINESTATE_SHIFT    (16U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_LINESTATE_MASK     (0x3U << USB2PHY_GRF_STATUS_USBHOST_UTMI_LINESTATE_SHIFT)    /* 0x00030000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_IDDIG_O_SHIFT      (18U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_IDDIG_O_MASK       (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_IDDIG_O_SHIFT)      /* 0x00040000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_SHIFT (19U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_MASK (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_HOSTDISCONNECT_SHIFT) /* 0x00080000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_BVALID_SHIFT       (20U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_BVALID_MASK        (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_BVALID_SHIFT)       /* 0x00100000 */
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_AVALID_SHIFT       (21U)
#define USB2PHY_GRF_STATUS_USBHOST_UTMI_AVALID_MASK        (0x1U << USB2PHY_GRF_STATUS_USBHOST_UTMI_AVALID_SHIFT)       /* 0x00200000 */
#define USB2PHY_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_SHIFT     (22U)
#define USB2PHY_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_MASK      (0x1U << USB2PHY_GRF_STATUS_USBHOST_PHY_LS_FS_RCV_SHIFT)     /* 0x00400000 */
#define USB2PHY_GRF_STATUS_USBPHY_DCP_DETECTED_SHIFT       (23U)
#define USB2PHY_GRF_STATUS_USBPHY_DCP_DETECTED_MASK        (0x1U << USB2PHY_GRF_STATUS_USBPHY_DCP_DETECTED_SHIFT)       /* 0x00800000 */
#define USB2PHY_GRF_STATUS_USBPHY_CP_DETECTED_SHIFT        (24U)
#define USB2PHY_GRF_STATUS_USBPHY_CP_DETECTED_MASK         (0x1U << USB2PHY_GRF_STATUS_USBPHY_CP_DETECTED_SHIFT)        /* 0x01000000 */
#define USB2PHY_GRF_STATUS_USBPHY_DP_DETECTED_SHIFT        (25U)
#define USB2PHY_GRF_STATUS_USBPHY_DP_DETECTED_MASK         (0x1U << USB2PHY_GRF_STATUS_USBPHY_DP_DETECTED_SHIFT)        /* 0x02000000 */
/* LS_CON */
#define USB2PHY_GRF_LS_CON_LINESTATE_FILTER_CON_SHIFT      (0U)
#define USB2PHY_GRF_LS_CON_LINESTATE_FILTER_CON_MASK       (0xFFFFFU << USB2PHY_GRF_LS_CON_LINESTATE_FILTER_CON_SHIFT)  /* 0x000FFFFF */
/* DIS_CON */
#define USB2PHY_GRF_DIS_CON_DISCONNECT_FILTER_CON_SHIFT    (0U)
#define USB2PHY_GRF_DIS_CON_DISCONNECT_FILTER_CON_MASK     (0xFFFFFU << USB2PHY_GRF_DIS_CON_DISCONNECT_FILTER_CON_SHIFT) /* 0x000FFFFF */
/* BVALID_CON */
#define USB2PHY_GRF_BVALID_CON_BVALID_FILTER_CON_SHIFT     (0U)
#define USB2PHY_GRF_BVALID_CON_BVALID_FILTER_CON_MASK      (0xFFFFFU << USB2PHY_GRF_BVALID_CON_BVALID_FILTER_CON_SHIFT) /* 0x000FFFFF */
/* ID_CON */
#define USB2PHY_GRF_ID_CON_ID_FILTER_CON_SHIFT             (0U)
#define USB2PHY_GRF_ID_CON_ID_FILTER_CON_MASK              (0xFFFFFFFU << USB2PHY_GRF_ID_CON_ID_FILTER_CON_SHIFT)       /* 0x0FFFFFFF */
/* CON4 */
#define USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_SEL0_SHIFT    (4U)
#define USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_SEL0_MASK     (0x1U << USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_SEL0_SHIFT)    /* 0x00000010 */
#define USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_SEL1_SHIFT    (5U)
#define USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_SEL1_MASK     (0x1U << USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_SEL1_SHIFT)    /* 0x00000020 */
#define USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_N_SHIFT       (6U)
#define USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_N_MASK        (0x1U << USB2PHY_GRF_CON4_USBOTG_UTMI_SUSPEND_N_SHIFT)       /* 0x00000040 */
/**************************************USB3PHY_GRF***************************************/
/* PIPE_CON0 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_DATABUSWIDTH_SHIFT  (0U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_DATABUSWIDTH_MASK   (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_DATABUSWIDTH_SHIFT)  /* 0x00000001 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_PHYMODE_SHIFT       (1U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_PHYMODE_MASK        (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_PHYMODE_SHIFT)       /* 0x00000002 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_RATE_SHIFT          (2U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_RATE_MASK           (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_RATE_SHIFT)          /* 0x00000004 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_MAC_PCLKREQ_N_SHIFT (3U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_MAC_PCLKREQ_N_MASK  (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_MAC_PCLKREQ_N_SHIFT) /* 0x00000008 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_POWERDOWN_SHIFT  (6U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_POWERDOWN_MASK   (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_POWERDOWN_SHIFT)  /* 0x00000040 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_RXTERMINATION_SHIFT (7U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_RXTERMINATION_MASK (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_RXTERMINATION_SHIFT) /* 0x00000080 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXDECTRX_LOOPBACK_SHIFT (8U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXDECTRX_LOOPBACK_MASK (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXDECTRX_LOOPBACK_SHIFT) /* 0x00000100 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXELECIDLE_SHIFT (9U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXELECIDLE_MASK  (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXELECIDLE_SHIFT) /* 0x00000200 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXONESZEROS_SHIFT (10U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXONESZEROS_MASK (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L0_TXONESZEROS_SHIFT) /* 0x00000400 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_POWERDOWN_SHIFT  (11U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_POWERDOWN_MASK   (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_POWERDOWN_SHIFT)  /* 0x00000800 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_RXTERMINATION_SHIFT (12U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_RXTERMINATION_MASK (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_RXTERMINATION_SHIFT) /* 0x00001000 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXDECTRX_LOOPBACK_SHIFT (13U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXDECTRX_LOOPBACK_MASK (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXDECTRX_LOOPBACK_SHIFT) /* 0x00002000 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXELECIDLE_SHIFT (14U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXELECIDLE_MASK  (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXELECIDLE_SHIFT) /* 0x00004000 */
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXONESZEROS_SHIFT (15U)
#define USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXONESZEROS_MASK (0x1U << USB3PHY_GRF_PIPE_CON0_SEL_PIPE_L1_TXONESZEROS_SHIFT) /* 0x00008000 */
/* PIPE_CON1 */
#define USB3PHY_GRF_PIPE_CON1_PIPE_DATABUSWIDTH_SHIFT      (0U)
#define USB3PHY_GRF_PIPE_CON1_PIPE_DATABUSWIDTH_MASK       (0x3U << USB3PHY_GRF_PIPE_CON1_PIPE_DATABUSWIDTH_SHIFT)      /* 0x00000003 */
#define USB3PHY_GRF_PIPE_CON1_PIPE_PHYMODE_SHIFT           (2U)
#define USB3PHY_GRF_PIPE_CON1_PIPE_PHYMODE_MASK            (0x3U << USB3PHY_GRF_PIPE_CON1_PIPE_PHYMODE_SHIFT)           /* 0x0000000C */
#define USB3PHY_GRF_PIPE_CON1_PIPE_RATE_SHIFT              (4U)
#define USB3PHY_GRF_PIPE_CON1_PIPE_RATE_MASK               (0x3U << USB3PHY_GRF_PIPE_CON1_PIPE_RATE_SHIFT)              /* 0x00000030 */
#define USB3PHY_GRF_PIPE_CON1_PIPE_MAC_PCLKREQ_N_SHIFT     (6U)
#define USB3PHY_GRF_PIPE_CON1_PIPE_MAC_PCLKREQ_N_MASK      (0x3U << USB3PHY_GRF_PIPE_CON1_PIPE_MAC_PCLKREQ_N_SHIFT)     /* 0x000000C0 */
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_RXELECIDLE_DISABLE_L0_SHIFT (8U)
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_RXELECIDLE_DISABLE_L0_MASK (0x1U << USB3PHY_GRF_PIPE_CON1_SEL_PIPE_RXELECIDLE_DISABLE_L0_SHIFT) /* 0x00000100 */
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMMONMODE_DISABLE_L0_SHIFT (9U)
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMMONMODE_DISABLE_L0_MASK (0x1U << USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMMONMODE_DISABLE_L0_SHIFT) /* 0x00000200 */
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_RXELECIDLE_DISABLE_L1_SHIFT (10U)
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_RXELECIDLE_DISABLE_L1_MASK (0x1U << USB3PHY_GRF_PIPE_CON1_SEL_PIPE_RXELECIDLE_DISABLE_L1_SHIFT) /* 0x00000400 */
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMMONMODE_DISABLE_L1_SHIFT (11U)
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMMONMODE_DISABLE_L1_MASK (0x1U << USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMMONMODE_DISABLE_L1_SHIFT) /* 0x00000800 */
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMPLIANCE_L0_SHIFT (12U)
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMPLIANCE_L0_MASK (0x1U << USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMPLIANCE_L0_SHIFT) /* 0x00001000 */
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMPLIANCE_L1_SHIFT (13U)
#define USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMPLIANCE_L1_MASK (0x1U << USB3PHY_GRF_PIPE_CON1_SEL_PIPE_TXCOMPLIANCE_L1_SHIFT) /* 0x00002000 */
#define USB3PHY_GRF_PIPE_CON1_PIPE_TXCOMPLIANCE_L0_SHIFT   (14U)
#define USB3PHY_GRF_PIPE_CON1_PIPE_TXCOMPLIANCE_L0_MASK    (0x1U << USB3PHY_GRF_PIPE_CON1_PIPE_TXCOMPLIANCE_L0_SHIFT)   /* 0x00004000 */
#define USB3PHY_GRF_PIPE_CON1_PIPE_TXCOMPLIANCE_L1_SHIFT   (15U)
#define USB3PHY_GRF_PIPE_CON1_PIPE_TXCOMPLIANCE_L1_MASK    (0x1U << USB3PHY_GRF_PIPE_CON1_PIPE_TXCOMPLIANCE_L1_SHIFT)   /* 0x00008000 */
/* PIPE_CON2 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_POWERDOWN_SHIFT      (0U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_POWERDOWN_MASK       (0x3U << USB3PHY_GRF_PIPE_CON2_PIPE_L0_POWERDOWN_SHIFT)      /* 0x00000003 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_RXTERMINATION_SHIFT  (2U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_RXTERMINATION_MASK   (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L0_RXTERMINATION_SHIFT)  /* 0x00000004 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXDECTRX_LOOPBACK_SHIFT (3U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXDECTRX_LOOPBACK_MASK (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXDECTRX_LOOPBACK_SHIFT) /* 0x00000008 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXELECIDLE_SHIFT     (4U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXELECIDLE_MASK      (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXELECIDLE_SHIFT)     /* 0x00000010 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXONESZEROS_SHIFT    (5U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXONESZEROS_MASK     (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L0_TXONESZEROS_SHIFT)    /* 0x00000020 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_RXELECIDLE_DISABLE_L0_SHIFT (6U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_RXELECIDLE_DISABLE_L0_MASK (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_RXELECIDLE_DISABLE_L0_SHIFT) /* 0x00000040 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_TXCOMMONMODE_DISABLE_L0_SHIFT (7U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_TXCOMMONMODE_DISABLE_L0_MASK (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_TXCOMMONMODE_DISABLE_L0_SHIFT) /* 0x00000080 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_POWERDOWN_SHIFT      (8U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_POWERDOWN_MASK       (0x3U << USB3PHY_GRF_PIPE_CON2_PIPE_L1_POWERDOWN_SHIFT)      /* 0x00000300 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_RXTERMINATION_SHIFT  (10U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_RXTERMINATION_MASK   (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L1_RXTERMINATION_SHIFT)  /* 0x00000400 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXDECTRX_LOOPBACK_SHIFT (11U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXDECTRX_LOOPBACK_MASK (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXDECTRX_LOOPBACK_SHIFT) /* 0x00000800 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXELECIDLE_SHIFT     (12U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXELECIDLE_MASK      (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXELECIDLE_SHIFT)     /* 0x00001000 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXONESZEROS_SHIFT    (13U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXONESZEROS_MASK     (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_L1_TXONESZEROS_SHIFT)    /* 0x00002000 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_RXELECIDLE_DISABLE_L1_SHIFT (14U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_RXELECIDLE_DISABLE_L1_MASK (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_RXELECIDLE_DISABLE_L1_SHIFT) /* 0x00004000 */
#define USB3PHY_GRF_PIPE_CON2_PIPE_TXCOMMONMODE_DISABLE_L1_SHIFT (15U)
#define USB3PHY_GRF_PIPE_CON2_PIPE_TXCOMMONMODE_DISABLE_L1_MASK (0x1U << USB3PHY_GRF_PIPE_CON2_PIPE_TXCOMMONMODE_DISABLE_L1_SHIFT) /* 0x00008000 */
/* PIPE_CON3 */
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXSWING_L0_SHIFT        (1U)
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXSWING_L0_MASK         (0x1U << USB3PHY_GRF_PIPE_CON3_PIPE_TXSWING_L0_SHIFT)        /* 0x00000002 */
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXDEEMPH_L0_SHIFT       (2U)
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXDEEMPH_L0_MASK        (0x3U << USB3PHY_GRF_PIPE_CON3_PIPE_TXDEEMPH_L0_SHIFT)       /* 0x0000000C */
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXMARGIN_L0_SHIFT       (4U)
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXMARGIN_L0_MASK        (0x7U << USB3PHY_GRF_PIPE_CON3_PIPE_TXMARGIN_L0_SHIFT)       /* 0x00000070 */
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXSWING_L1_SHIFT        (7U)
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXSWING_L1_MASK         (0x1U << USB3PHY_GRF_PIPE_CON3_PIPE_TXSWING_L1_SHIFT)        /* 0x00000080 */
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXDEEMPH_L1_SHIFT       (8U)
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXDEEMPH_L1_MASK        (0x3U << USB3PHY_GRF_PIPE_CON3_PIPE_TXDEEMPH_L1_SHIFT)       /* 0x00000300 */
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXMARGIN_L1_SHIFT       (10U)
#define USB3PHY_GRF_PIPE_CON3_PIPE_TXMARGIN_L1_MASK        (0x7U << USB3PHY_GRF_PIPE_CON3_PIPE_TXMARGIN_L1_SHIFT)       /* 0x00001C00 */
/* PIPE_CON4 */
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXSWING_L0_SHIFT    (0U)
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXSWING_L0_MASK     (0x1U << USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXSWING_L0_SHIFT)    /* 0x00000001 */
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXDEEMPH_L0_SHIFT   (1U)
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXDEEMPH_L0_MASK    (0x1U << USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXDEEMPH_L0_SHIFT)   /* 0x00000002 */
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXMARGIN_L0_SHIFT   (2U)
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXMARGIN_L0_MASK    (0x1U << USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXMARGIN_L0_SHIFT)   /* 0x00000004 */
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXSWING_L1_SHIFT    (3U)
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXSWING_L1_MASK     (0x1U << USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXSWING_L1_SHIFT)    /* 0x00000008 */
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXDEEMPH_L1_SHIFT   (4U)
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXDEEMPH_L1_MASK    (0x1U << USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXDEEMPH_L1_SHIFT)   /* 0x00000010 */
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXMARGIN_L1_SHIFT   (5U)
#define USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXMARGIN_L1_MASK    (0x1U << USB3PHY_GRF_PIPE_CON4_SEL_PIPE_TXMARGIN_L1_SHIFT)   /* 0x00000020 */
/* PIPE_STATUS0 */
#define USB3PHY_GRF_PIPE_STATUS0_PCIE_PHY_OBS_SHIFT        (0U)
#define USB3PHY_GRF_PIPE_STATUS0_PCIE_PHY_OBS_MASK         (0xFFFFU << USB3PHY_GRF_PIPE_STATUS0_PCIE_PHY_OBS_SHIFT)     /* 0x0000FFFF */
/* PIPE_STATUS1 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXSTATUS_O_L1_SHIFT  (0U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXSTATUS_O_L1_MASK   (0x7U << USB3PHY_GRF_PIPE_STATUS1_PIPE_RXSTATUS_O_L1_SHIFT)  /* 0x00000007 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXSTATUS_O_L0_SHIFT  (3U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXSTATUS_O_L0_MASK   (0x7U << USB3PHY_GRF_PIPE_STATUS1_PIPE_RXSTATUS_O_L0_SHIFT)  /* 0x00000038 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_PHYSTATUS_O_L1_SHIFT (6U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_PHYSTATUS_O_L1_MASK  (0x1U << USB3PHY_GRF_PIPE_STATUS1_PIPE_PHYSTATUS_O_L1_SHIFT) /* 0x00000040 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_PHYSTATUS_O_L0_SHIFT (7U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_PHYSTATUS_O_L0_MASK  (0x1U << USB3PHY_GRF_PIPE_STATUS1_PIPE_PHYSTATUS_O_L0_SHIFT) /* 0x00000080 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXELECIDLE_O_L1_SHIFT (8U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXELECIDLE_O_L1_MASK (0x1U << USB3PHY_GRF_PIPE_STATUS1_PIPE_RXELECIDLE_O_L1_SHIFT) /* 0x00000100 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXELECIDLE_O_L0_SHIFT (9U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_RXELECIDLE_O_L0_MASK (0x1U << USB3PHY_GRF_PIPE_STATUS1_PIPE_RXELECIDLE_O_L0_SHIFT) /* 0x00000200 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_POWER_PRESETN_SHIFT  (10U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_POWER_PRESETN_MASK   (0x1U << USB3PHY_GRF_PIPE_STATUS1_PIPE_POWER_PRESETN_SHIFT)  /* 0x00000400 */
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_CLKREQ_N_SHIFT       (11U)
#define USB3PHY_GRF_PIPE_STATUS1_PIPE_CLKREQ_N_MASK        (0x1U << USB3PHY_GRF_PIPE_STATUS1_PIPE_CLKREQ_N_SHIFT)       /* 0x00000800 */
#define USB3PHY_GRF_PIPE_STATUS1_RX_CDR_LOCK_L1_SHIFT      (12U)
#define USB3PHY_GRF_PIPE_STATUS1_RX_CDR_LOCK_L1_MASK       (0x1U << USB3PHY_GRF_PIPE_STATUS1_RX_CDR_LOCK_L1_SHIFT)      /* 0x00001000 */
#define USB3PHY_GRF_PIPE_STATUS1_RX_CDR_LOCK_L0_SHIFT      (13U)
#define USB3PHY_GRF_PIPE_STATUS1_RX_CDR_LOCK_L0_MASK       (0x1U << USB3PHY_GRF_PIPE_STATUS1_RX_CDR_LOCK_L0_SHIFT)      /* 0x00002000 */
#define USB3PHY_GRF_PIPE_STATUS1_TX_PLL_LOCK_SHIFT         (14U)
#define USB3PHY_GRF_PIPE_STATUS1_TX_PLL_LOCK_MASK          (0x1U << USB3PHY_GRF_PIPE_STATUS1_TX_PLL_LOCK_SHIFT)         /* 0x00004000 */
/****************************************PMU_GRF*****************************************/
/* GPIO0A_IOMUX */
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
/* GPIO0A_SR */
#define PMU_GRF_GPIO0A_SR_GPIO0A0_SR_SHIFT                 (0U)
#define PMU_GRF_GPIO0A_SR_GPIO0A0_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A0_SR_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_GPIO0A_SR_GPIO0A1_SR_SHIFT                 (1U)
#define PMU_GRF_GPIO0A_SR_GPIO0A1_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A1_SR_SHIFT)                 /* 0x00000002 */
#define PMU_GRF_GPIO0A_SR_GPIO0A2_SR_SHIFT                 (2U)
#define PMU_GRF_GPIO0A_SR_GPIO0A2_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A2_SR_SHIFT)                 /* 0x00000004 */
#define PMU_GRF_GPIO0A_SR_GPIO0A3_SR_SHIFT                 (3U)
#define PMU_GRF_GPIO0A_SR_GPIO0A3_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A3_SR_SHIFT)                 /* 0x00000008 */
#define PMU_GRF_GPIO0A_SR_GPIO0A4_SR_SHIFT                 (4U)
#define PMU_GRF_GPIO0A_SR_GPIO0A4_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A4_SR_SHIFT)                 /* 0x00000010 */
#define PMU_GRF_GPIO0A_SR_GPIO0A5_SR_SHIFT                 (5U)
#define PMU_GRF_GPIO0A_SR_GPIO0A5_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A5_SR_SHIFT)                 /* 0x00000020 */
#define PMU_GRF_GPIO0A_SR_GPIO0A6_SR_SHIFT                 (6U)
#define PMU_GRF_GPIO0A_SR_GPIO0A6_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A6_SR_SHIFT)                 /* 0x00000040 */
#define PMU_GRF_GPIO0A_SR_GPIO0A7_SR_SHIFT                 (7U)
#define PMU_GRF_GPIO0A_SR_GPIO0A7_SR_MASK                  (0x1U << PMU_GRF_GPIO0A_SR_GPIO0A7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO0B_SR */
#define PMU_GRF_GPIO0B_SR_GPIO0B0_SR_SHIFT                 (0U)
#define PMU_GRF_GPIO0B_SR_GPIO0B0_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B0_SR_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_GPIO0B_SR_GPIO0B1_SR_SHIFT                 (1U)
#define PMU_GRF_GPIO0B_SR_GPIO0B1_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B1_SR_SHIFT)                 /* 0x00000002 */
#define PMU_GRF_GPIO0B_SR_GPIO0B2_SR_SHIFT                 (2U)
#define PMU_GRF_GPIO0B_SR_GPIO0B2_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B2_SR_SHIFT)                 /* 0x00000004 */
#define PMU_GRF_GPIO0B_SR_GPIO0B3_SR_SHIFT                 (3U)
#define PMU_GRF_GPIO0B_SR_GPIO0B3_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B3_SR_SHIFT)                 /* 0x00000008 */
#define PMU_GRF_GPIO0B_SR_GPIO0B4_SR_SHIFT                 (4U)
#define PMU_GRF_GPIO0B_SR_GPIO0B4_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B4_SR_SHIFT)                 /* 0x00000010 */
#define PMU_GRF_GPIO0B_SR_GPIO0B5_SR_SHIFT                 (5U)
#define PMU_GRF_GPIO0B_SR_GPIO0B5_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B5_SR_SHIFT)                 /* 0x00000020 */
#define PMU_GRF_GPIO0B_SR_GPIO0B6_SR_SHIFT                 (6U)
#define PMU_GRF_GPIO0B_SR_GPIO0B6_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B6_SR_SHIFT)                 /* 0x00000040 */
#define PMU_GRF_GPIO0B_SR_GPIO0B7_SR_SHIFT                 (7U)
#define PMU_GRF_GPIO0B_SR_GPIO0B7_SR_MASK                  (0x1U << PMU_GRF_GPIO0B_SR_GPIO0B7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO0C_SR */
#define PMU_GRF_GPIO0C_SR_GPIO0C0_SR_SHIFT                 (0U)
#define PMU_GRF_GPIO0C_SR_GPIO0C0_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C0_SR_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_GPIO0C_SR_GPIO0C1_SR_SHIFT                 (1U)
#define PMU_GRF_GPIO0C_SR_GPIO0C1_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C1_SR_SHIFT)                 /* 0x00000002 */
#define PMU_GRF_GPIO0C_SR_GPIO0C2_SR_SHIFT                 (2U)
#define PMU_GRF_GPIO0C_SR_GPIO0C2_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C2_SR_SHIFT)                 /* 0x00000004 */
#define PMU_GRF_GPIO0C_SR_GPIO0C3_SR_SHIFT                 (3U)
#define PMU_GRF_GPIO0C_SR_GPIO0C3_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C3_SR_SHIFT)                 /* 0x00000008 */
#define PMU_GRF_GPIO0C_SR_GPIO0C4_SR_SHIFT                 (4U)
#define PMU_GRF_GPIO0C_SR_GPIO0C4_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C4_SR_SHIFT)                 /* 0x00000010 */
#define PMU_GRF_GPIO0C_SR_GPIO0C5_SR_SHIFT                 (5U)
#define PMU_GRF_GPIO0C_SR_GPIO0C5_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C5_SR_SHIFT)                 /* 0x00000020 */
#define PMU_GRF_GPIO0C_SR_GPIO0C6_SR_SHIFT                 (6U)
#define PMU_GRF_GPIO0C_SR_GPIO0C6_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C6_SR_SHIFT)                 /* 0x00000040 */
#define PMU_GRF_GPIO0C_SR_GPIO0C7_SR_SHIFT                 (7U)
#define PMU_GRF_GPIO0C_SR_GPIO0C7_SR_MASK                  (0x1U << PMU_GRF_GPIO0C_SR_GPIO0C7_SR_SHIFT)                 /* 0x00000080 */
/* GPIO0A_SMT */
#define PMU_GRF_GPIO0A_SMT_GPIO0A0_SMT_SHIFT               (0U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A0_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A0_SMT_SHIFT)               /* 0x00000001 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A1_SMT_SHIFT               (1U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A1_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A1_SMT_SHIFT)               /* 0x00000002 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A2_SMT_SHIFT               (2U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A2_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A2_SMT_SHIFT)               /* 0x00000004 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A3_SMT_SHIFT               (3U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A3_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A3_SMT_SHIFT)               /* 0x00000008 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A4_SMT_SHIFT               (4U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A4_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A4_SMT_SHIFT)               /* 0x00000010 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A5_SMT_SHIFT               (5U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A5_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A5_SMT_SHIFT)               /* 0x00000020 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A6_SMT_SHIFT               (6U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A6_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A6_SMT_SHIFT)               /* 0x00000040 */
#define PMU_GRF_GPIO0A_SMT_GPIO0A7_SMT_SHIFT               (7U)
#define PMU_GRF_GPIO0A_SMT_GPIO0A7_SMT_MASK                (0x1U << PMU_GRF_GPIO0A_SMT_GPIO0A7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO0B_SMT */
#define PMU_GRF_GPIO0B_SMT_GPIO0B0_SMT_SHIFT               (0U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B0_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B0_SMT_SHIFT)               /* 0x00000001 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B1_SMT_SHIFT               (1U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B1_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B1_SMT_SHIFT)               /* 0x00000002 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B2_SMT_SHIFT               (2U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B2_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B2_SMT_SHIFT)               /* 0x00000004 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B3_SMT_SHIFT               (3U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B3_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B3_SMT_SHIFT)               /* 0x00000008 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B4_SMT_SHIFT               (4U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B4_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B4_SMT_SHIFT)               /* 0x00000010 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B5_SMT_SHIFT               (5U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B5_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B5_SMT_SHIFT)               /* 0x00000020 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B6_SMT_SHIFT               (6U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B6_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B6_SMT_SHIFT)               /* 0x00000040 */
#define PMU_GRF_GPIO0B_SMT_GPIO0B7_SMT_SHIFT               (7U)
#define PMU_GRF_GPIO0B_SMT_GPIO0B7_SMT_MASK                (0x1U << PMU_GRF_GPIO0B_SMT_GPIO0B7_SMT_SHIFT)               /* 0x00000080 */
/* GPIO0C_SMT */
#define PMU_GRF_GPIO0C_SMT_GPIO0C0_SMT_SHIFT               (0U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C0_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C0_SMT_SHIFT)               /* 0x00000001 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C1_SMT_SHIFT               (1U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C1_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C1_SMT_SHIFT)               /* 0x00000002 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C2_SMT_SHIFT               (2U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C2_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C2_SMT_SHIFT)               /* 0x00000004 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C3_SMT_SHIFT               (3U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C3_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C3_SMT_SHIFT)               /* 0x00000008 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C4_SMT_SHIFT               (4U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C4_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C4_SMT_SHIFT)               /* 0x00000010 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C5_SMT_SHIFT               (5U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C5_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C5_SMT_SHIFT)               /* 0x00000020 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C6_SMT_SHIFT               (6U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C6_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C6_SMT_SHIFT)               /* 0x00000040 */
#define PMU_GRF_GPIO0C_SMT_GPIO0C7_SMT_SHIFT               (7U)
#define PMU_GRF_GPIO0C_SMT_GPIO0C7_SMT_MASK                (0x1U << PMU_GRF_GPIO0C_SMT_GPIO0C7_SMT_SHIFT)               /* 0x00000080 */
/* SOC_CON0 */
#define PMU_GRF_SOC_CON0_CON_32K_IOE_SHIFT                 (0U)
#define PMU_GRF_SOC_CON0_CON_32K_IOE_MASK                  (0x1U << PMU_GRF_SOC_CON0_CON_32K_IOE_SHIFT)                 /* 0x00000001 */
#define PMU_GRF_SOC_CON0_OSC_GM_SEL_SHIFT                  (1U)
#define PMU_GRF_SOC_CON0_OSC_GM_SEL_MASK                   (0xFU << PMU_GRF_SOC_CON0_OSC_GM_SEL_SHIFT)                  /* 0x0000001E */
#define PMU_GRF_SOC_CON0_UART0_RTS_SEL_SHIFT               (5U)
#define PMU_GRF_SOC_CON0_UART0_RTS_SEL_MASK                (0x1U << PMU_GRF_SOC_CON0_UART0_RTS_SEL_SHIFT)               /* 0x00000020 */
#define PMU_GRF_SOC_CON0_UART0_CTS_SEL_SHIFT               (6U)
#define PMU_GRF_SOC_CON0_UART0_CTS_SEL_MASK                (0x1U << PMU_GRF_SOC_CON0_UART0_CTS_SEL_SHIFT)               /* 0x00000040 */
#define PMU_GRF_SOC_CON0_GRF_CON_PMIC_SLEEP_SEL_SHIFT      (7U)
#define PMU_GRF_SOC_CON0_GRF_CON_PMIC_SLEEP_SEL_MASK       (0x1U << PMU_GRF_SOC_CON0_GRF_CON_PMIC_SLEEP_SEL_SHIFT)      /* 0x00000080 */
#define PMU_GRF_SOC_CON0_GRF_CON_PCIE_RSTN_SEL_SHIFT       (8U)
#define PMU_GRF_SOC_CON0_GRF_CON_PCIE_RSTN_SEL_MASK        (0x1U << PMU_GRF_SOC_CON0_GRF_CON_PCIE_RSTN_SEL_SHIFT)       /* 0x00000100 */
#define PMU_GRF_SOC_CON0_GRF_PCI_WAKE_SEL_SHIFT            (9U)
#define PMU_GRF_SOC_CON0_GRF_PCI_WAKE_SEL_MASK             (0x1U << PMU_GRF_SOC_CON0_GRF_PCI_WAKE_SEL_SHIFT)            /* 0x00000200 */
#define PMU_GRF_SOC_CON0_GRF_CON_PCIE_REQN_SEL_SHIFT       (10U)
#define PMU_GRF_SOC_CON0_GRF_CON_PCIE_REQN_SEL_MASK        (0x1U << PMU_GRF_SOC_CON0_GRF_CON_PCIE_REQN_SEL_SHIFT)       /* 0x00000400 */
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_SEL_SHIFT         (12U)
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_SEL_MASK          (0x1U << PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_SEL_SHIFT)         /* 0x00001000 */
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_SHIFT        (13U)
#define PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_MASK         (0x1U << PMU_GRF_SOC_CON0_DDRPHY_BUFFEREN_CORE_SHIFT)        /* 0x00002000 */
/* SOC_CON1 */
#define PMU_GRF_SOC_CON1_RESETN_HOLD_SHIFT                 (0U)
#define PMU_GRF_SOC_CON1_RESETN_HOLD_MASK                  (0x7FFFU << PMU_GRF_SOC_CON1_RESETN_HOLD_SHIFT)              /* 0x00007FFF */
#define PMU_GRF_SOC_CON1_HOLD_THE_DDRFAILSAFE_SHIFT        (15U)
#define PMU_GRF_SOC_CON1_HOLD_THE_DDRFAILSAFE_MASK         (0x1U << PMU_GRF_SOC_CON1_HOLD_THE_DDRFAILSAFE_SHIFT)        /* 0x00008000 */
/* SOC_CON2 */
#define PMU_GRF_SOC_CON2_NPOR_OUT2CHIP_PULSE_WIDTH_SHIFT   (0U)
#define PMU_GRF_SOC_CON2_NPOR_OUT2CHIP_PULSE_WIDTH_MASK    (0x1FFFU << PMU_GRF_SOC_CON2_NPOR_OUT2CHIP_PULSE_WIDTH_SHIFT) /* 0x00001FFF */
/* SOC_CON3 */
#define PMU_GRF_SOC_CON3_TSADC_SHUT_RESET_TRIGGER_EN_SHIFT (0U)
#define PMU_GRF_SOC_CON3_TSADC_SHUT_RESET_TRIGGER_EN_MASK  (0x1U << PMU_GRF_SOC_CON3_TSADC_SHUT_RESET_TRIGGER_EN_SHIFT) /* 0x00000001 */
#define PMU_GRF_SOC_CON3_WDT_SHUT_RESET_TRIGGER_EN_SHIFT   (1U)
#define PMU_GRF_SOC_CON3_WDT_SHUT_RESET_TRIGGER_EN_MASK    (0x1U << PMU_GRF_SOC_CON3_WDT_SHUT_RESET_TRIGGER_EN_SHIFT)   /* 0x00000002 */
#define PMU_GRF_SOC_CON3_DDRIO_RET_EN_SHIFT                (2U)
#define PMU_GRF_SOC_CON3_DDRIO_RET_EN_MASK                 (0x1U << PMU_GRF_SOC_CON3_DDRIO_RET_EN_SHIFT)                /* 0x00000004 */
#define PMU_GRF_SOC_CON3_SREF_ENTER_EN_SHIFT               (3U)
#define PMU_GRF_SOC_CON3_SREF_ENTER_EN_MASK                (0x1U << PMU_GRF_SOC_CON3_SREF_ENTER_EN_SHIFT)               /* 0x00000008 */
#define PMU_GRF_SOC_CON3_DDRC_GATING_EN_SHIFT              (4U)
#define PMU_GRF_SOC_CON3_DDRC_GATING_EN_MASK               (0x1U << PMU_GRF_SOC_CON3_DDRC_GATING_EN_SHIFT)              /* 0x00000010 */
#define PMU_GRF_SOC_CON3_DDR_IO_RET_DE_REQ_SHIFT           (5U)
#define PMU_GRF_SOC_CON3_DDR_IO_RET_DE_REQ_MASK            (0x1U << PMU_GRF_SOC_CON3_DDR_IO_RET_DE_REQ_SHIFT)           /* 0x00000020 */
#define PMU_GRF_SOC_CON3_DDR_IO_RET_CFG_SHIFT              (6U)
#define PMU_GRF_SOC_CON3_DDR_IO_RET_CFG_MASK               (0x1U << PMU_GRF_SOC_CON3_DDR_IO_RET_CFG_SHIFT)              /* 0x00000040 */
#define PMU_GRF_SOC_CON3_DDR_IO_RET_OEN_CFG_SHIFT          (7U)
#define PMU_GRF_SOC_CON3_DDR_IO_RET_OEN_CFG_MASK           (0x1U << PMU_GRF_SOC_CON3_DDR_IO_RET_OEN_CFG_SHIFT)          /* 0x00000080 */
#define PMU_GRF_SOC_CON3_UPCTL_C_SYSREQ_CFG_SHIFT          (8U)
#define PMU_GRF_SOC_CON3_UPCTL_C_SYSREQ_CFG_MASK           (0x1U << PMU_GRF_SOC_CON3_UPCTL_C_SYSREQ_CFG_SHIFT)          /* 0x00000100 */
/* SOC_CON4 */
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_SAWL_SHIFT            (0U)
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_SAWL_MASK             (0x3U << PMU_GRF_SOC_CON4_PMU_SRAM_MA_SAWL_SHIFT)            /* 0x00000003 */
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_WL_SHIFT              (2U)
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_WL_MASK               (0x3U << PMU_GRF_SOC_CON4_PMU_SRAM_MA_WL_SHIFT)              /* 0x0000000C */
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_WRAS_SHIFT            (4U)
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_WRAS_MASK             (0x3U << PMU_GRF_SOC_CON4_PMU_SRAM_MA_WRAS_SHIFT)            /* 0x00000030 */
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_WRASD_SHIFT           (6U)
#define PMU_GRF_SOC_CON4_PMU_SRAM_MA_WRASD_MASK            (0x1U << PMU_GRF_SOC_CON4_PMU_SRAM_MA_WRASD_SHIFT)           /* 0x00000040 */
/* SOC_STATUS */
#define PMU_GRF_SOC_STATUS_GRF_PMUVCCIO2_VOLTAGE_SHIFT     (0U)
#define PMU_GRF_SOC_STATUS_GRF_PMUVCCIO2_VOLTAGE_MASK      (0x1U << PMU_GRF_SOC_STATUS_GRF_PMUVCCIO2_VOLTAGE_SHIFT)     /* 0x00000001 */
/* IO_VSEL0 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO0_VSEL_SHIFT             (0U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO0_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO0_VSEL_SHIFT)             /* 0x00000001 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO1_VSEL_SHIFT             (1U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO1_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO1_VSEL_SHIFT)             /* 0x00000002 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO2_VSEL_SHIFT             (2U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO2_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO2_VSEL_SHIFT)             /* 0x00000004 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO3_VSEL_SHIFT             (3U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO3_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO3_VSEL_SHIFT)             /* 0x00000008 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO4_VSEL_SHIFT             (4U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO4_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO4_VSEL_SHIFT)             /* 0x00000010 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO5_VSEL_SHIFT             (5U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO5_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO5_VSEL_SHIFT)             /* 0x00000020 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO6_VSEL_SHIFT             (6U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO6_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO6_VSEL_SHIFT)             /* 0x00000040 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO7_VSEL_SHIFT             (7U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO7_VSEL_MASK              (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO7_VSEL_SHIFT)             /* 0x00000080 */
#define PMU_GRF_IO_VSEL0_GRF_VCCIO0_VSEL_SRC_SHIFT         (8U)
#define PMU_GRF_IO_VSEL0_GRF_VCCIO0_VSEL_SRC_MASK          (0x1U << PMU_GRF_IO_VSEL0_GRF_VCCIO0_VSEL_SRC_SHIFT)         /* 0x00000100 */
/* IO_VSEL1 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO0_VSEL_AUTO_SHIFT        (0U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO0_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO0_VSEL_AUTO_SHIFT)        /* 0x00000001 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO1_VSEL_AUTO_SHIFT        (1U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO1_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO1_VSEL_AUTO_SHIFT)        /* 0x00000002 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO2_VSEL_AUTO_SHIFT        (2U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO2_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO2_VSEL_AUTO_SHIFT)        /* 0x00000004 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO3_VSEL_AUTO_SHIFT        (3U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO3_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO3_VSEL_AUTO_SHIFT)        /* 0x00000008 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO4_VSEL_AUTO_SHIFT        (4U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO4_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO4_VSEL_AUTO_SHIFT)        /* 0x00000010 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO5_VSEL_AUTO_SHIFT        (5U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO5_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO5_VSEL_AUTO_SHIFT)        /* 0x00000020 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO6_VSEL_AUTO_SHIFT        (6U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO6_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO6_VSEL_AUTO_SHIFT)        /* 0x00000040 */
#define PMU_GRF_IO_VSEL1_GRF_VCCIO7_VSEL_AUTO_SHIFT        (7U)
#define PMU_GRF_IO_VSEL1_GRF_VCCIO7_VSEL_AUTO_MASK         (0x1U << PMU_GRF_IO_VSEL1_GRF_VCCIO7_VSEL_AUTO_SHIFT)        /* 0x00000080 */
/* IO_VSEL_STATUS */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO0_VOLTAGE_SHIFT    (0U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO0_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO0_VOLTAGE_SHIFT)    /* 0x00000001 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO1_VOLTAGE_SHIFT    (1U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO1_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO1_VOLTAGE_SHIFT)    /* 0x00000002 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO2_VOLTAGE0_SHIFT   (2U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO2_VOLTAGE0_MASK    (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO2_VOLTAGE0_SHIFT)   /* 0x00000004 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO3_VOLTAGE_SHIFT    (3U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO3_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO3_VOLTAGE_SHIFT)    /* 0x00000008 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO4_VOLTAGE_SHIFT    (4U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO4_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO4_VOLTAGE_SHIFT)    /* 0x00000010 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO5_VOLTAGE_SHIFT    (5U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO5_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO5_VOLTAGE_SHIFT)    /* 0x00000020 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO6_VOLTAGE_SHIFT    (6U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO6_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO6_VOLTAGE_SHIFT)    /* 0x00000040 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO7_VOLTAGE_SHIFT    (7U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO7_VOLTAGE_MASK     (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO7_VOLTAGE_SHIFT)    /* 0x00000080 */
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO2_VOLTAGE1_SHIFT   (8U)
#define PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO2_VOLTAGE1_MASK    (0x1U << PMU_GRF_IO_VSEL_STATUS_GRF_VCCIO2_VOLTAGE1_SHIFT)   /* 0x00000100 */
/* PVTM_CON0 */
#define PMU_GRF_PVTM_CON0_PVTM_PMU_START_SHIFT             (0U)
#define PMU_GRF_PVTM_CON0_PVTM_PMU_START_MASK              (0x1U << PMU_GRF_PVTM_CON0_PVTM_PMU_START_SHIFT)             /* 0x00000001 */
#define PMU_GRF_PVTM_CON0_PVTM_PMU_OSC_EN_SHIFT            (1U)
#define PMU_GRF_PVTM_CON0_PVTM_PMU_OSC_EN_MASK             (0x1U << PMU_GRF_PVTM_CON0_PVTM_PMU_OSC_EN_SHIFT)            /* 0x00000002 */
#define PMU_GRF_PVTM_CON0_PVTM_CLKOUT_DIV_SHIFT            (2U)
#define PMU_GRF_PVTM_CON0_PVTM_CLKOUT_DIV_MASK             (0x3FU << PMU_GRF_PVTM_CON0_PVTM_CLKOUT_DIV_SHIFT)           /* 0x000000FC */
/* PVTM_CON1 */
#define PMU_GRF_PVTM_CON1_PVTM_PMU_CAL_CNT_SHIFT           (0U)
#define PMU_GRF_PVTM_CON1_PVTM_PMU_CAL_CNT_MASK            (0xFFFFFFFFU << PMU_GRF_PVTM_CON1_PVTM_PMU_CAL_CNT_SHIFT)    /* 0xFFFFFFFF */
/* PVTM_STATUS0 */
#define PMU_GRF_PVTM_STATUS0_PVTM_PMU_FREQ_DONE_SHIFT      (0U)
#define PMU_GRF_PVTM_STATUS0_PVTM_PMU_FREQ_DONE_MASK       (0x1U << PMU_GRF_PVTM_STATUS0_PVTM_PMU_FREQ_DONE_SHIFT)      /* 0x00000001 */
/* PVTM_STATUS1 */
#define PMU_GRF_PVTM_STATUS1_PVTM_PMU_FREQ_CNT_SHIFT       (0U)
#define PMU_GRF_PVTM_STATUS1_PVTM_PMU_FREQ_CNT_MASK        (0xFFFFFFFFU << PMU_GRF_PVTM_STATUS1_PVTM_PMU_FREQ_CNT_SHIFT) /* 0xFFFFFFFF */
/* OS_REG0 */
#define PMU_GRF_OS_REG0_PMU_OS_REG0_SHIFT                  (0U)
#define PMU_GRF_OS_REG0_PMU_OS_REG0_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG0_PMU_OS_REG0_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG1 */
#define PMU_GRF_OS_REG1_PMU_OS_REG1_SHIFT                  (0U)
#define PMU_GRF_OS_REG1_PMU_OS_REG1_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG1_PMU_OS_REG1_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG2 */
#define PMU_GRF_OS_REG2_PMU_OS_REG2_SHIFT                  (0U)
#define PMU_GRF_OS_REG2_PMU_OS_REG2_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG2_PMU_OS_REG2_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG3 */
#define PMU_GRF_OS_REG3_PMU_OS_REG3_SHIFT                  (0U)
#define PMU_GRF_OS_REG3_PMU_OS_REG3_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG3_PMU_OS_REG3_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG4 */
#define PMU_GRF_OS_REG4_PMU_OS_REG4_SHIFT                  (0U)
#define PMU_GRF_OS_REG4_PMU_OS_REG4_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG4_PMU_OS_REG4_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG5 */
#define PMU_GRF_OS_REG5_PMU_OS_REG5_SHIFT                  (0U)
#define PMU_GRF_OS_REG5_PMU_OS_REG5_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG5_PMU_OS_REG5_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG6 */
#define PMU_GRF_OS_REG6_PMU_OS_REG6_SHIFT                  (0U)
#define PMU_GRF_OS_REG6_PMU_OS_REG6_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG6_PMU_OS_REG6_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG7 */
#define PMU_GRF_OS_REG7_PMU_OS_REG7_SHIFT                  (0U)
#define PMU_GRF_OS_REG7_PMU_OS_REG7_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG7_PMU_OS_REG7_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG8 */
#define PMU_GRF_OS_REG8_PMU_OS_REG8_SHIFT                  (0U)
#define PMU_GRF_OS_REG8_PMU_OS_REG8_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG8_PMU_OS_REG8_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG9 */
#define PMU_GRF_OS_REG9_PMU_OS_REG9_SHIFT                  (0U)
#define PMU_GRF_OS_REG9_PMU_OS_REG9_MASK                   (0xFFFFFFFFU << PMU_GRF_OS_REG9_PMU_OS_REG9_SHIFT)           /* 0xFFFFFFFF */
/* OS_REG10 */
#define PMU_GRF_OS_REG10_PMU_OS_REG10_SHIFT                (0U)
#define PMU_GRF_OS_REG10_PMU_OS_REG10_MASK                 (0xFFFFFFFFU << PMU_GRF_OS_REG10_PMU_OS_REG10_SHIFT)         /* 0xFFFFFFFF */
/* OS_REG11 */
#define PMU_GRF_OS_REG11_PMU_OS_REG11_SHIFT                (0U)
#define PMU_GRF_OS_REG11_PMU_OS_REG11_MASK                 (0xFFFFFFFFU << PMU_GRF_OS_REG11_PMU_OS_REG11_SHIFT)         /* 0xFFFFFFFF */
/* RESET_FUNCTION_STATUS */
#define PMU_GRF_RESET_FUNCTION_STATUS_FIRST_RESET_SRC_SHIFT (0U)
#define PMU_GRF_RESET_FUNCTION_STATUS_FIRST_RESET_SRC_MASK (0x1U << PMU_GRF_RESET_FUNCTION_STATUS_FIRST_RESET_SRC_SHIFT) /* 0x00000001 */
#define PMU_GRF_RESET_FUNCTION_STATUS_WDT_RESET_SRC_SHIFT  (1U)
#define PMU_GRF_RESET_FUNCTION_STATUS_WDT_RESET_SRC_MASK   (0x1U << PMU_GRF_RESET_FUNCTION_STATUS_WDT_RESET_SRC_SHIFT)  /* 0x00000002 */
#define PMU_GRF_RESET_FUNCTION_STATUS_TSADC_SHUT_RESET_SRC_SHIFT (2U)
#define PMU_GRF_RESET_FUNCTION_STATUS_TSADC_SHUT_RESET_SRC_MASK (0x1U << PMU_GRF_RESET_FUNCTION_STATUS_TSADC_SHUT_RESET_SRC_SHIFT) /* 0x00000004 */
#define PMU_GRF_RESET_FUNCTION_STATUS_DDR_FAIL_SAFE_SRC_SHIFT (3U)
#define PMU_GRF_RESET_FUNCTION_STATUS_DDR_FAIL_SAFE_SRC_MASK (0x1U << PMU_GRF_RESET_FUNCTION_STATUS_DDR_FAIL_SAFE_SRC_SHIFT) /* 0x00000008 */
/* RESET_FUNCTION_CLR */
#define PMU_GRF_RESET_FUNCTION_CLR_FIRST_RESET_SRC_CLR_SHIFT (0U)
#define PMU_GRF_RESET_FUNCTION_CLR_FIRST_RESET_SRC_CLR_MASK (0x1U << PMU_GRF_RESET_FUNCTION_CLR_FIRST_RESET_SRC_CLR_SHIFT) /* 0x00000001 */
#define PMU_GRF_RESET_FUNCTION_CLR_WDT_RESET_SRC_CLR_SHIFT (1U)
#define PMU_GRF_RESET_FUNCTION_CLR_WDT_RESET_SRC_CLR_MASK  (0x1U << PMU_GRF_RESET_FUNCTION_CLR_WDT_RESET_SRC_CLR_SHIFT) /* 0x00000002 */
#define PMU_GRF_RESET_FUNCTION_CLR_TSADC_SHUT_RESET_SRC_CLR_SHIFT (2U)
#define PMU_GRF_RESET_FUNCTION_CLR_TSADC_SHUT_RESET_SRC_CLR_MASK (0x1U << PMU_GRF_RESET_FUNCTION_CLR_TSADC_SHUT_RESET_SRC_CLR_SHIFT) /* 0x00000004 */
#define PMU_GRF_RESET_FUNCTION_CLR_DDR_FAIL_SAFE_SRC_CLR_SHIFT (3U)
#define PMU_GRF_RESET_FUNCTION_CLR_DDR_FAIL_SAFE_SRC_CLR_MASK (0x1U << PMU_GRF_RESET_FUNCTION_CLR_DDR_FAIL_SAFE_SRC_CLR_SHIFT) /* 0x00000008 */
/* SIG_DETECT_CON */
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_POS_IRQ_MSK_SHIFT (0U)
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_POS_IRQ_MSK_MASK (0x1U << PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_POS_IRQ_MSK_SHIFT) /* 0x00000001 */
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_NEG_IRQ_MSK_SHIFT (1U)
#define PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_NEG_IRQ_MSK_MASK (0x1U << PMU_GRF_SIG_DETECT_CON_SDMMC_DETECTN_NEG_IRQ_MSK_SHIFT) /* 0x00000002 */
/* SIG_DETECT_STATUS */
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_POS_IRQ_SHIFT (0U)
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_POS_IRQ_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_POS_IRQ_SHIFT) /* 0x00000001 */
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_NEG_IRQ_SHIFT (1U)
#define PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_NEG_IRQ_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_SDMMC_DETECTN_NEG_IRQ_SHIFT) /* 0x00000002 */
/* SIG_DETECT_STATUS_CLEAR */
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_POS_IRQ_CLR_SHIFT (0U)
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_POS_IRQ_CLR_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_POS_IRQ_CLR_SHIFT) /* 0x00000001 */
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_NEG_IRQ_CLR_SHIFT (1U)
#define PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_NEG_IRQ_CLR_MASK (0x1U << PMU_GRF_SIG_DETECT_STATUS_CLEAR_SDMMC_DETECTN_NEG_IRQ_CLR_SHIFT) /* 0x00000002 */
/* SDMMC_DET_COUNTER */
#define PMU_GRF_SDMMC_DET_COUNTER_SDMMC_DETECTN_COUNT_SHIFT (0U)
#define PMU_GRF_SDMMC_DET_COUNTER_SDMMC_DETECTN_COUNT_MASK (0xFFFFFU << PMU_GRF_SDMMC_DET_COUNTER_SDMMC_DETECTN_COUNT_SHIFT) /* 0x000FFFFF */
/****************************************DDR_GRF*****************************************/
/* CON0 */
#define DDR_GRF_CON0_UPCTL_SLVERR_ENABLE_SHIFT             (0U)
#define DDR_GRF_CON0_UPCTL_SLVERR_ENABLE_MASK              (0x1U << DDR_GRF_CON0_UPCTL_SLVERR_ENABLE_SHIFT)             /* 0x00000001 */
#define DDR_GRF_CON0_DFI_INIT_START_SEL_SHIFT              (1U)
#define DDR_GRF_CON0_DFI_INIT_START_SEL_MASK               (0x1U << DDR_GRF_CON0_DFI_INIT_START_SEL_SHIFT)              /* 0x00000002 */
#define DDR_GRF_CON0_DFI_INIT_START_SHIFT                  (2U)
#define DDR_GRF_CON0_DFI_INIT_START_MASK                   (0x1U << DDR_GRF_CON0_DFI_INIT_START_SHIFT)                  /* 0x00000004 */
#define DDR_GRF_CON0_CSYSREQ_ACLK_SHIFT                    (3U)
#define DDR_GRF_CON0_CSYSREQ_ACLK_MASK                     (0x1U << DDR_GRF_CON0_CSYSREQ_ACLK_SHIFT)                    /* 0x00000008 */
#define DDR_GRF_CON0_CSYSREQ_UPCTL_PMU_SHIFT               (4U)
#define DDR_GRF_CON0_CSYSREQ_UPCTL_PMU_MASK                (0x1U << DDR_GRF_CON0_CSYSREQ_UPCTL_PMU_SHIFT)               /* 0x00000010 */
#define DDR_GRF_CON0_CSYSREQ_UPCTL_DDRSTDBY_SHIFT          (5U)
#define DDR_GRF_CON0_CSYSREQ_UPCTL_DDRSTDBY_MASK           (0x1U << DDR_GRF_CON0_CSYSREQ_UPCTL_DDRSTDBY_SHIFT)          /* 0x00000020 */
#define DDR_GRF_CON0_PA_RMASK_SHIFT                        (8U)
#define DDR_GRF_CON0_PA_RMASK_MASK                         (0x3U << DDR_GRF_CON0_PA_RMASK_SHIFT)                        /* 0x00000300 */
#define DDR_GRF_CON0_PA_WMASK_SHIFT                        (10U)
#define DDR_GRF_CON0_PA_WMASK_MASK                         (0x1U << DDR_GRF_CON0_PA_WMASK_SHIFT)                        /* 0x00000400 */
#define DDR_GRF_CON0_ARURGENT_SHIFT                        (11U)
#define DDR_GRF_CON0_ARURGENT_MASK                         (0x1U << DDR_GRF_CON0_ARURGENT_SHIFT)                        /* 0x00000800 */
#define DDR_GRF_CON0_ARPOISON_SHIFT                        (12U)
#define DDR_GRF_CON0_ARPOISON_MASK                         (0x1U << DDR_GRF_CON0_ARPOISON_SHIFT)                        /* 0x00001000 */
#define DDR_GRF_CON0_AWURGENT_SHIFT                        (13U)
#define DDR_GRF_CON0_AWURGENT_MASK                         (0x1U << DDR_GRF_CON0_AWURGENT_SHIFT)                        /* 0x00002000 */
#define DDR_GRF_CON0_AWPOISON_SHIFT                        (14U)
#define DDR_GRF_CON0_AWPOISON_MASK                         (0x1U << DDR_GRF_CON0_AWPOISON_SHIFT)                        /* 0x00004000 */
/* CON1 */
#define DDR_GRF_CON1_UPCTL_AXI_CG_EN_SHIFT                 (0U)
#define DDR_GRF_CON1_UPCTL_AXI_CG_EN_MASK                  (0x1U << DDR_GRF_CON1_UPCTL_AXI_CG_EN_SHIFT)                 /* 0x00000001 */
#define DDR_GRF_CON1_UPCTL_APB_CG_EN_SHIFT                 (1U)
#define DDR_GRF_CON1_UPCTL_APB_CG_EN_MASK                  (0x1U << DDR_GRF_CON1_UPCTL_APB_CG_EN_SHIFT)                 /* 0x00000002 */
#define DDR_GRF_CON1_UPCTL_CORE_CG_EN_SHIFT                (2U)
#define DDR_GRF_CON1_UPCTL_CORE_CG_EN_MASK                 (0x1U << DDR_GRF_CON1_UPCTL_CORE_CG_EN_SHIFT)                /* 0x00000004 */
#define DDR_GRF_CON1_SELFREF_TYPE2_EN_SHIFT                (3U)
#define DDR_GRF_CON1_SELFREF_TYPE2_EN_MASK                 (0x1U << DDR_GRF_CON1_SELFREF_TYPE2_EN_SHIFT)                /* 0x00000008 */
#define DDR_GRF_CON1_UPCTL_SYSCREQ_CG_EN_SHIFT             (4U)
#define DDR_GRF_CON1_UPCTL_SYSCREQ_CG_EN_MASK              (0x1U << DDR_GRF_CON1_UPCTL_SYSCREQ_CG_EN_SHIFT)             /* 0x00000010 */
#define DDR_GRF_CON1_CON_UPCTL2_PDSRLP_CG_EN_SHIFT         (5U)
#define DDR_GRF_CON1_CON_UPCTL2_PDSRLP_CG_EN_MASK          (0x1U << DDR_GRF_CON1_CON_UPCTL2_PDSRLP_CG_EN_SHIFT)         /* 0x00000020 */
#define DDR_GRF_CON1_AUTO_SR_DLY_SHIFT                     (8U)
#define DDR_GRF_CON1_AUTO_SR_DLY_MASK                      (0xFU << DDR_GRF_CON1_AUTO_SR_DLY_SHIFT)                     /* 0x00000F00 */
#define DDR_GRF_CON1_CON_PLLPD_DQCMD_SHIFT                 (12U)
#define DDR_GRF_CON1_CON_PLLPD_DQCMD_MASK                  (0x1U << DDR_GRF_CON1_CON_PLLPD_DQCMD_SHIFT)                 /* 0x00001000 */
#define DDR_GRF_CON1_CON_CLKOUTEN_DQCMD_SHIFT              (13U)
#define DDR_GRF_CON1_CON_CLKOUTEN_DQCMD_MASK               (0x1U << DDR_GRF_CON1_CON_CLKOUTEN_DQCMD_SHIFT)              /* 0x00002000 */
#define DDR_GRF_CON1_CON_PLLPD_DQCMD_SEL_SHIFT             (14U)
#define DDR_GRF_CON1_CON_PLLPD_DQCMD_SEL_MASK              (0x1U << DDR_GRF_CON1_CON_PLLPD_DQCMD_SEL_SHIFT)             /* 0x00004000 */
#define DDR_GRF_CON1_CON_CLKOUTEN_DQCMD_SEL_SHIFT          (15U)
#define DDR_GRF_CON1_CON_CLKOUTEN_DQCMD_SEL_MASK           (0x1U << DDR_GRF_CON1_CON_CLKOUTEN_DQCMD_SEL_SHIFT)          /* 0x00008000 */
/* SPLIT_CON */
#define DDR_GRF_SPLIT_CON_SPADDR_SHIFT                     (0U)
#define DDR_GRF_SPLIT_CON_SPADDR_MASK                      (0xFFU << DDR_GRF_SPLIT_CON_SPADDR_SHIFT)                    /* 0x000000FF */
#define DDR_GRF_SPLIT_CON_BYPASS_SHIFT                     (8U)
#define DDR_GRF_SPLIT_CON_BYPASS_MASK                      (0x1U << DDR_GRF_SPLIT_CON_BYPASS_SHIFT)                     /* 0x00000100 */
#define DDR_GRF_SPLIT_CON_SPMODE_SHIFT                     (9U)
#define DDR_GRF_SPLIT_CON_SPMODE_MASK                      (0x3U << DDR_GRF_SPLIT_CON_SPMODE_SHIFT)                     /* 0x00000600 */
/* LP_CON */
#define DDR_GRF_LP_CON_DDR23_MODE_SHIFT                    (0U)
#define DDR_GRF_LP_CON_DDR23_MODE_MASK                     (0x1U << DDR_GRF_LP_CON_DDR23_MODE_SHIFT)                    /* 0x00000001 */
#define DDR_GRF_LP_CON_DDR4_MODE_SHIFT                     (1U)
#define DDR_GRF_LP_CON_DDR4_MODE_MASK                      (0x1U << DDR_GRF_LP_CON_DDR4_MODE_SHIFT)                     /* 0x00000002 */
#define DDR_GRF_LP_CON_LP23_MODE_SHIFT                     (2U)
#define DDR_GRF_LP_CON_LP23_MODE_MASK                      (0x1U << DDR_GRF_LP_CON_LP23_MODE_SHIFT)                     /* 0x00000004 */
#define DDR_GRF_LP_CON_LPCKDIS_INI_SHIFT                   (8U)
#define DDR_GRF_LP_CON_LPCKDIS_INI_MASK                    (0x1U << DDR_GRF_LP_CON_LPCKDIS_INI_SHIFT)                   /* 0x00000100 */
#define DDR_GRF_LP_CON_LPCKDIS_EN_SHIFT                    (9U)
#define DDR_GRF_LP_CON_LPCKDIS_EN_MASK                     (0x1U << DDR_GRF_LP_CON_LPCKDIS_EN_SHIFT)                    /* 0x00000200 */
#define DDR_GRF_LP_CON_PD_CTL_EN_SHIFT                     (12U)
#define DDR_GRF_LP_CON_PD_CTL_EN_MASK                      (0x1U << DDR_GRF_LP_CON_PD_CTL_EN_SHIFT)                     /* 0x00001000 */
#define DDR_GRF_LP_CON_SR_CTL_EN_SHIFT                     (13U)
#define DDR_GRF_LP_CON_SR_CTL_EN_MASK                      (0x1U << DDR_GRF_LP_CON_SR_CTL_EN_SHIFT)                     /* 0x00002000 */
/* STATUS0 */
#define DDR_GRF_STATUS0_MRR_DATA0_SHIFT                    (0U)
#define DDR_GRF_STATUS0_MRR_DATA0_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS0_MRR_DATA0_SHIFT)             /* 0xFFFFFFFF */
/* STATUS1 */
#define DDR_GRF_STATUS1_MRR_DATA0_SHIFT                    (0U)
#define DDR_GRF_STATUS1_MRR_DATA0_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS1_MRR_DATA0_SHIFT)             /* 0xFFFFFFFF */
/* STATUS2 */
#define DDR_GRF_STATUS2_MRR_DATA0_SHIFT                    (0U)
#define DDR_GRF_STATUS2_MRR_DATA0_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS2_MRR_DATA0_SHIFT)             /* 0xFFFFFFFF */
/* STATUS3 */
#define DDR_GRF_STATUS3_MRR_DATA0_SHIFT                    (0U)
#define DDR_GRF_STATUS3_MRR_DATA0_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS3_MRR_DATA0_SHIFT)             /* 0xFFFFFFFF */
/* STATUS4 */
#define DDR_GRF_STATUS4_MRR_DATA1_SHIFT                    (0U)
#define DDR_GRF_STATUS4_MRR_DATA1_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS4_MRR_DATA1_SHIFT)             /* 0xFFFFFFFF */
/* STATUS5 */
#define DDR_GRF_STATUS5_MRR_DATA1_SHIFT                    (0U)
#define DDR_GRF_STATUS5_MRR_DATA1_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS5_MRR_DATA1_SHIFT)             /* 0xFFFFFFFF */
/* STATUS6 */
#define DDR_GRF_STATUS6_MRR_DATA1_SHIFT                    (0U)
#define DDR_GRF_STATUS6_MRR_DATA1_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS6_MRR_DATA1_SHIFT)             /* 0xFFFFFFFF */
/* STATUS7 */
#define DDR_GRF_STATUS7_MRR_DATA1_SHIFT                    (0U)
#define DDR_GRF_STATUS7_MRR_DATA1_MASK                     (0xFFFFFFFFU << DDR_GRF_STATUS7_MRR_DATA1_SHIFT)             /* 0xFFFFFFFF */
/* STATUS8 */
#define DDR_GRF_STATUS8_CSYSREQ_DDRC_SHIFT                 (0U)
#define DDR_GRF_STATUS8_CSYSREQ_DDRC_MASK                  (0x1U << DDR_GRF_STATUS8_CSYSREQ_DDRC_SHIFT)                 /* 0x00000001 */
#define DDR_GRF_STATUS8_CSYSACK_DDRC_SHIFT                 (1U)
#define DDR_GRF_STATUS8_CSYSACK_DDRC_MASK                  (0x1U << DDR_GRF_STATUS8_CSYSACK_DDRC_SHIFT)                 /* 0x00000002 */
#define DDR_GRF_STATUS8_CACTIVE_DDRC_SHIFT                 (2U)
#define DDR_GRF_STATUS8_CACTIVE_DDRC_MASK                  (0x1U << DDR_GRF_STATUS8_CACTIVE_DDRC_SHIFT)                 /* 0x00000004 */
#define DDR_GRF_STATUS8_CON_CSYSREQ_ACLK_SHIFT             (3U)
#define DDR_GRF_STATUS8_CON_CSYSREQ_ACLK_MASK              (0x1U << DDR_GRF_STATUS8_CON_CSYSREQ_ACLK_SHIFT)             /* 0x00000008 */
#define DDR_GRF_STATUS8_CSYSACLK_ACLK_SHIFT                (4U)
#define DDR_GRF_STATUS8_CSYSACLK_ACLK_MASK                 (0x1U << DDR_GRF_STATUS8_CSYSACLK_ACLK_SHIFT)                /* 0x00000010 */
#define DDR_GRF_STATUS8_CACTIVE_ACLK_SHIFT                 (5U)
#define DDR_GRF_STATUS8_CACTIVE_ACLK_MASK                  (0x1U << DDR_GRF_STATUS8_CACTIVE_ACLK_SHIFT)                 /* 0x00000020 */
#define DDR_GRF_STATUS8_DDRC_REG_SELFREF_TYPE_SHIFT        (6U)
#define DDR_GRF_STATUS8_DDRC_REG_SELFREF_TYPE_MASK         (0x3U << DDR_GRF_STATUS8_DDRC_REG_SELFREF_TYPE_SHIFT)        /* 0x000000C0 */
#define DDR_GRF_STATUS8_DFI_SCRAMBLE_KEY_READY_SHIFT       (8U)
#define DDR_GRF_STATUS8_DFI_SCRAMBLE_KEY_READY_MASK        (0x1U << DDR_GRF_STATUS8_DFI_SCRAMBLE_KEY_READY_SHIFT)       /* 0x00000100 */
#define DDR_GRF_STATUS8_DFI_SCRAMBLE_WRITE_OF_SHIFT        (9U)
#define DDR_GRF_STATUS8_DFI_SCRAMBLE_WRITE_OF_MASK         (0x1U << DDR_GRF_STATUS8_DFI_SCRAMBLE_WRITE_OF_SHIFT)        /* 0x00000200 */
#define DDR_GRF_STATUS8_DFI_SCRAMBLE_READ_OF_SHIFT         (10U)
#define DDR_GRF_STATUS8_DFI_SCRAMBLE_READ_OF_MASK          (0x1U << DDR_GRF_STATUS8_DFI_SCRAMBLE_READ_OF_SHIFT)         /* 0x00000400 */
#define DDR_GRF_STATUS8_PERI_PORT_PROBE_MAINTRACEALARM_SHIFT (11U)
#define DDR_GRF_STATUS8_PERI_PORT_PROBE_MAINTRACEALARM_MASK (0x1U << DDR_GRF_STATUS8_PERI_PORT_PROBE_MAINTRACEALARM_SHIFT) /* 0x00000800 */
#define DDR_GRF_STATUS8_PERI_PORT_PROBE_MAINSTATALARM_SHIFT (12U)
#define DDR_GRF_STATUS8_PERI_PORT_PROBE_MAINSTATALARM_MASK (0x1U << DDR_GRF_STATUS8_PERI_PORT_PROBE_MAINSTATALARM_SHIFT) /* 0x00001000 */
#define DDR_GRF_STATUS8_MMIP_PORT_PROBE_MAINTRACEALARM_SHIFT (13U)
#define DDR_GRF_STATUS8_MMIP_PORT_PROBE_MAINTRACEALARM_MASK (0x1U << DDR_GRF_STATUS8_MMIP_PORT_PROBE_MAINTRACEALARM_SHIFT) /* 0x00002000 */
#define DDR_GRF_STATUS8_MMIP_PORT_PROBE_MAINSTATALARM_SHIFT (14U)
#define DDR_GRF_STATUS8_MMIP_PORT_PROBE_MAINSTATALARM_MASK (0x1U << DDR_GRF_STATUS8_MMIP_PORT_PROBE_MAINSTATALARM_SHIFT) /* 0x00004000 */
#define DDR_GRF_STATUS8_GPU_PORT_PROBE_MAINTRACEALARM_SHIFT (15U)
#define DDR_GRF_STATUS8_GPU_PORT_PROBE_MAINTRACEALARM_MASK (0x1U << DDR_GRF_STATUS8_GPU_PORT_PROBE_MAINTRACEALARM_SHIFT) /* 0x00008000 */
#define DDR_GRF_STATUS8_GPU_PORT_PROBE_MAINSTATALARM_SHIFT (16U)
#define DDR_GRF_STATUS8_GPU_PORT_PROBE_MAINSTATALARM_MASK  (0x1U << DDR_GRF_STATUS8_GPU_PORT_PROBE_MAINSTATALARM_SHIFT) /* 0x00010000 */
#define DDR_GRF_STATUS8_CPU_PORT_PROBE_MAINTRACEALARM_SHIFT (17U)
#define DDR_GRF_STATUS8_CPU_PORT_PROBE_MAINTRACEALARM_MASK (0x1U << DDR_GRF_STATUS8_CPU_PORT_PROBE_MAINTRACEALARM_SHIFT) /* 0x00020000 */
#define DDR_GRF_STATUS8_CPU_PORT_PROBE_MAINSTATALARM_SHIFT (18U)
#define DDR_GRF_STATUS8_CPU_PORT_PROBE_MAINSTATALARM_MASK  (0x1U << DDR_GRF_STATUS8_CPU_PORT_PROBE_MAINSTATALARM_SHIFT) /* 0x00040000 */
/* STATUS9 */
#define DDR_GRF_STATUS9_LPR_CREDIT_CNT_SHIFT               (0U)
#define DDR_GRF_STATUS9_LPR_CREDIT_CNT_MASK                (0x7FU << DDR_GRF_STATUS9_LPR_CREDIT_CNT_SHIFT)              /* 0x0000007F */
#define DDR_GRF_STATUS9_HPR_CREDIT_CNT_SHIFT               (8U)
#define DDR_GRF_STATUS9_HPR_CREDIT_CNT_MASK                (0x7FU << DDR_GRF_STATUS9_HPR_CREDIT_CNT_SHIFT)              /* 0x00007F00 */
#define DDR_GRF_STATUS9_WR_CREDIT_CNT_SHIFT                (16U)
#define DDR_GRF_STATUS9_WR_CREDIT_CNT_MASK                 (0x7FU << DDR_GRF_STATUS9_WR_CREDIT_CNT_SHIFT)               /* 0x007F0000 */
#define DDR_GRF_STATUS9_HIF_REFRESH_REQ_BANK_SHIFT         (24U)
#define DDR_GRF_STATUS9_HIF_REFRESH_REQ_BANK_MASK          (0x3FU << DDR_GRF_STATUS9_HIF_REFRESH_REQ_BANK_SHIFT)        /* 0x3F000000 */
#define DDR_GRF_STATUS9_DFI_LP_CK_DISABLE_SHIFT            (31U)
#define DDR_GRF_STATUS9_DFI_LP_CK_DISABLE_MASK             (0x1U << DDR_GRF_STATUS9_DFI_LP_CK_DISABLE_SHIFT)            /* 0x80000000 */
/****************************************USB_GRF*****************************************/
/* PCIE_CON0 */
#define USB_GRF_PCIE_CON0_PCIE_PWR_IDLE_REQ_SHIFT          (1U)
#define USB_GRF_PCIE_CON0_PCIE_PWR_IDLE_REQ_MASK           (0x1U << USB_GRF_PCIE_CON0_PCIE_PWR_IDLE_REQ_SHIFT)          /* 0x00000002 */
#define USB_GRF_PCIE_CON0_PCIE_LINK_RST_GRT_SHIFT          (2U)
#define USB_GRF_PCIE_CON0_PCIE_LINK_RST_GRT_MASK           (0x1U << USB_GRF_PCIE_CON0_PCIE_LINK_RST_GRT_SHIFT)          /* 0x00000004 */
/* PCIE_STATUS0 */
#define USB_GRF_PCIE_STATUS0_PCIE_PWR_IDLE_ACK_SHIFT       (0U)
#define USB_GRF_PCIE_STATUS0_PCIE_PWR_IDLE_ACK_MASK        (0x1U << USB_GRF_PCIE_STATUS0_PCIE_PWR_IDLE_ACK_SHIFT)       /* 0x00000001 */
#define USB_GRF_PCIE_STATUS0_PCIE_PWR_IDLE_SHIFT           (1U)
#define USB_GRF_PCIE_STATUS0_PCIE_PWR_IDLE_MASK            (0x1U << USB_GRF_PCIE_STATUS0_PCIE_PWR_IDLE_SHIFT)           /* 0x00000002 */
/* USB3OTG_CON0 */
#define USB_GRF_USB3OTG_CON0_USB3OTG_BUS_FILTER_BYPASS_SHIFT (0U)
#define USB_GRF_USB3OTG_CON0_USB3OTG_BUS_FILTER_BYPASS_MASK (0xFU << USB_GRF_USB3OTG_CON0_USB3OTG_BUS_FILTER_BYPASS_SHIFT) /* 0x0000000F */
#define USB_GRF_USB3OTG_CON0_USB3OTG_HUB_PORT_OVERCURRENT_SHIFT (4U)
#define USB_GRF_USB3OTG_CON0_USB3OTG_HUB_PORT_OVERCURRENT_MASK (0x3U << USB_GRF_USB3OTG_CON0_USB3OTG_HUB_PORT_OVERCURRENT_SHIFT) /* 0x00000030 */
#define USB_GRF_USB3OTG_CON0_USB3OTG_HUB_PORT_PERM_ATTACH_SHIFT (6U)
#define USB_GRF_USB3OTG_CON0_USB3OTG_HUB_PORT_PERM_ATTACH_MASK (0x3U << USB_GRF_USB3OTG_CON0_USB3OTG_HUB_PORT_PERM_ATTACH_SHIFT) /* 0x000000C0 */
#define USB_GRF_USB3OTG_CON0_USB3OTG_FLADJ_30MHZ_REG_SHIFT (8U)
#define USB_GRF_USB3OTG_CON0_USB3OTG_FLADJ_30MHZ_REG_MASK  (0x3FU << USB_GRF_USB3OTG_CON0_USB3OTG_FLADJ_30MHZ_REG_SHIFT) /* 0x00003F00 */
#define USB_GRF_USB3OTG_CON0_USB3OTG_HOST_PORT_POWER_CONTROL_PRESENT_SHIFT (14U)
#define USB_GRF_USB3OTG_CON0_USB3OTG_HOST_PORT_POWER_CONTROL_PRESENT_MASK (0x1U << USB_GRF_USB3OTG_CON0_USB3OTG_HOST_PORT_POWER_CONTROL_PRESENT_SHIFT) /* 0x00004000 */
#define USB_GRF_USB3OTG_CON0_USB3OTG_HOST_U2_PORT_DISABLE_SHIFT (15U)
#define USB_GRF_USB3OTG_CON0_USB3OTG_HOST_U2_PORT_DISABLE_MASK (0x1U << USB_GRF_USB3OTG_CON0_USB3OTG_HOST_U2_PORT_DISABLE_SHIFT) /* 0x00008000 */
/* USB3OTG_CON1 */
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_U3_PORT_DISABLE_SHIFT (0U)
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_U3_PORT_DISABLE_MASK (0x1U << USB_GRF_USB3OTG_CON1_USB3OTG_HOST_U3_PORT_DISABLE_SHIFT) /* 0x00000001 */
#define USB_GRF_USB3OTG_CON1_USB3OTG_PME_EN_SHIFT          (1U)
#define USB_GRF_USB3OTG_CON1_USB3OTG_PME_EN_MASK           (0x1U << USB_GRF_USB3OTG_CON1_USB3OTG_PME_EN_SHIFT)          /* 0x00000002 */
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_LEGACY_SMI_PCI_CMD_SHIFT (4U)
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_LEGACY_SMI_PCI_CMD_MASK (0x1U << USB_GRF_USB3OTG_CON1_USB3OTG_HOST_LEGACY_SMI_PCI_CMD_SHIFT) /* 0x00000010 */
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_LEGACY_SMI_BAR_SHIFT (5U)
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_LEGACY_SMI_BAR_MASK (0x1U << USB_GRF_USB3OTG_CON1_USB3OTG_HOST_LEGACY_SMI_BAR_SHIFT) /* 0x00000020 */
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_NUM_U2_PORT_SHIFT (8U)
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_NUM_U2_PORT_MASK (0xFU << USB_GRF_USB3OTG_CON1_USB3OTG_HOST_NUM_U2_PORT_SHIFT) /* 0x00000F00 */
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_NUM_U3_PORT_SHIFT (12U)
#define USB_GRF_USB3OTG_CON1_USB3OTG_HOST_NUM_U3_PORT_MASK (0xFU << USB_GRF_USB3OTG_CON1_USB3OTG_HOST_NUM_U3_PORT_SHIFT) /* 0x0000F000 */
/* USB3OTG_STATUS0 */
#define USB_GRF_USB3OTG_STATUS0_USB3OTG_LOGIC_ANALYZER_TRACE_SHIFT (0U)
#define USB_GRF_USB3OTG_STATUS0_USB3OTG_LOGIC_ANALYZER_TRACE_MASK (0xFFFFFFFFU << USB_GRF_USB3OTG_STATUS0_USB3OTG_LOGIC_ANALYZER_TRACE_SHIFT) /* 0xFFFFFFFF */
/* USB3OTG_STATUS1 */
#define USB_GRF_USB3OTG_STATUS1_USB3OTG_LOGIC_ANALYZER_TRACE_SHIFT (0U)
#define USB_GRF_USB3OTG_STATUS1_USB3OTG_LOGIC_ANALYZER_TRACE_MASK (0xFFFFFFFFU << USB_GRF_USB3OTG_STATUS1_USB3OTG_LOGIC_ANALYZER_TRACE_SHIFT) /* 0xFFFFFFFF */
/* USB3OTG_STATUS2 */
#define USB_GRF_USB3OTG_STATUS2_USB3OTG_HOST_CURRENT_BELT_SHIFT (0U)
#define USB_GRF_USB3OTG_STATUS2_USB3OTG_HOST_CURRENT_BELT_MASK (0xFFFU << USB_GRF_USB3OTG_STATUS2_USB3OTG_HOST_CURRENT_BELT_SHIFT) /* 0x00000FFF */
/* HOST0_CON0 */
#define USB_GRF_HOST0_CON0_HOST0_FLADJ_VAL_SHIFT           (0U)
#define USB_GRF_HOST0_CON0_HOST0_FLADJ_VAL_MASK            (0x3FU << USB_GRF_HOST0_CON0_HOST0_FLADJ_VAL_SHIFT)          /* 0x0000003F */
#define USB_GRF_HOST0_CON0_HOST0_FLADJ_VAL_COMMON_SHIFT    (6U)
#define USB_GRF_HOST0_CON0_HOST0_FLADJ_VAL_COMMON_MASK     (0x3FU << USB_GRF_HOST0_CON0_HOST0_FLADJ_VAL_COMMON_SHIFT)   /* 0x00000FC0 */
/* HOST0_CON1 */
#define USB_GRF_HOST0_CON1_HOST0_APP_START_CLK_SHIFT       (0U)
#define USB_GRF_HOST0_CON1_HOST0_APP_START_CLK_MASK        (0x1U << USB_GRF_HOST0_CON1_HOST0_APP_START_CLK_SHIFT)       /* 0x00000001 */
#define USB_GRF_HOST0_CON1_HOST0_HUBSETUP_MIN_SHIFT        (1U)
#define USB_GRF_HOST0_CON1_HOST0_HUBSETUP_MIN_MASK         (0x1U << USB_GRF_HOST0_CON1_HOST0_HUBSETUP_MIN_SHIFT)        /* 0x00000002 */
#define USB_GRF_HOST0_CON1_HOST0_INCR16_EN_SHIFT           (2U)
#define USB_GRF_HOST0_CON1_HOST0_INCR16_EN_MASK            (0x1U << USB_GRF_HOST0_CON1_HOST0_INCR16_EN_SHIFT)           /* 0x00000004 */
#define USB_GRF_HOST0_CON1_HOST0_INCR4_EN_SHIFT            (3U)
#define USB_GRF_HOST0_CON1_HOST0_INCR4_EN_MASK             (0x1U << USB_GRF_HOST0_CON1_HOST0_INCR4_EN_SHIFT)            /* 0x00000008 */
#define USB_GRF_HOST0_CON1_HOST0_INCR8_EN_SHIFT            (4U)
#define USB_GRF_HOST0_CON1_HOST0_INCR8_EN_MASK             (0x1U << USB_GRF_HOST0_CON1_HOST0_INCR8_EN_SHIFT)            /* 0x00000010 */
#define USB_GRF_HOST0_CON1_HOST0_INCRX_EN_SHIFT            (5U)
#define USB_GRF_HOST0_CON1_HOST0_INCRX_EN_MASK             (0x1U << USB_GRF_HOST0_CON1_HOST0_INCRX_EN_SHIFT)            /* 0x00000020 */
#define USB_GRF_HOST0_CON1_HOST0_SIM_MODE_SHIFT            (6U)
#define USB_GRF_HOST0_CON1_HOST0_SIM_MODE_MASK             (0x1U << USB_GRF_HOST0_CON1_HOST0_SIM_MODE_SHIFT)            /* 0x00000040 */
#define USB_GRF_HOST0_CON1_HOST0_WORD_IF_SHIFT             (7U)
#define USB_GRF_HOST0_CON1_HOST0_WORD_IF_MASK              (0x1U << USB_GRF_HOST0_CON1_HOST0_WORD_IF_SHIFT)             /* 0x00000080 */
#define USB_GRF_HOST0_CON1_HOST0_AUTOPPD_ON_OVERCUR_EN_SHIFT (8U)
#define USB_GRF_HOST0_CON1_HOST0_AUTOPPD_ON_OVERCUR_EN_MASK (0x1U << USB_GRF_HOST0_CON1_HOST0_AUTOPPD_ON_OVERCUR_EN_SHIFT) /* 0x00000100 */
#define USB_GRF_HOST0_CON1_HOST0_APP_PRT_OVRCUR_SHIFT      (9U)
#define USB_GRF_HOST0_CON1_HOST0_APP_PRT_OVRCUR_MASK       (0x1U << USB_GRF_HOST0_CON1_HOST0_APP_PRT_OVRCUR_SHIFT)      /* 0x00000200 */
#define USB_GRF_HOST0_CON1_HOST0_OHCI_CLKCKTRST_SHIFT      (10U)
#define USB_GRF_HOST0_CON1_HOST0_OHCI_CLKCKTRST_MASK       (0x1U << USB_GRF_HOST0_CON1_HOST0_OHCI_CLKCKTRST_SHIFT)      /* 0x00000400 */
#define USB_GRF_HOST0_CON1_HOST0_OHCI_CNTSEL_SHIFT         (11U)
#define USB_GRF_HOST0_CON1_HOST0_OHCI_CNTSEL_MASK          (0x1U << USB_GRF_HOST0_CON1_HOST0_OHCI_CNTSEL_SHIFT)         /* 0x00000800 */
#define USB_GRF_HOST0_CON1_HOST0_OHCI_SUSP_LGCY_SHIFT      (12U)
#define USB_GRF_HOST0_CON1_HOST0_OHCI_SUSP_LGCY_MASK       (0x1U << USB_GRF_HOST0_CON1_HOST0_OHCI_SUSP_LGCY_SHIFT)      /* 0x00001000 */
#define USB_GRF_HOST0_CON1_HOST0_ARB_PAUSE_SHIFT           (13U)
#define USB_GRF_HOST0_CON1_HOST0_ARB_PAUSE_MASK            (0x1U << USB_GRF_HOST0_CON1_HOST0_ARB_PAUSE_SHIFT)           /* 0x00002000 */
/* HOST0_STATUS */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_XFER_CNT_SHIFT     (0U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_XFER_CNT_MASK      (0x7FFU << USB_GRF_HOST0_STATUS_HOST0_EHCI_XFER_CNT_SHIFT)   /* 0x000007FF */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_USBSTS_SHIFT       (11U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_USBSTS_MASK        (0x3FU << USB_GRF_HOST0_STATUS_HOST0_EHCI_USBSTS_SHIFT)      /* 0x0001F800 */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_LPSMC_STATE_SHIFT  (17U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_LPSMC_STATE_MASK   (0xFU << USB_GRF_HOST0_STATUS_HOST0_EHCI_LPSMC_STATE_SHIFT)  /* 0x001E0000 */
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_RMTWKP_SHIFT       (21U)
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_RMTWKP_MASK        (0x1U << USB_GRF_HOST0_STATUS_HOST0_OHCI_RMTWKP_SHIFT)       /* 0x00200000 */
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_BUFACC_SHIFT       (22U)
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_BUFACC_MASK        (0x1U << USB_GRF_HOST0_STATUS_HOST0_OHCI_BUFACC_SHIFT)       /* 0x00400000 */
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_GLOBALSUSPEND_SHIFT (23U)
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_GLOBALSUSPEND_MASK (0x1U << USB_GRF_HOST0_STATUS_HOST0_OHCI_GLOBALSUSPEND_SHIFT) /* 0x00800000 */
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_DRWE_SHIFT         (24U)
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_DRWE_MASK          (0x1U << USB_GRF_HOST0_STATUS_HOST0_OHCI_DRWE_SHIFT)         /* 0x01000000 */
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_RWE_SHIFT          (25U)
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_RWE_MASK           (0x1U << USB_GRF_HOST0_STATUS_HOST0_OHCI_RWE_SHIFT)          /* 0x02000000 */
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_CCS_SHIFT          (26U)
#define USB_GRF_HOST0_STATUS_HOST0_OHCI_CCS_MASK           (0x1U << USB_GRF_HOST0_STATUS_HOST0_OHCI_CCS_SHIFT)          /* 0x04000000 */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_XFER_PRDC_SHIFT    (27U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_XFER_PRDC_MASK     (0x1U << USB_GRF_HOST0_STATUS_HOST0_EHCI_XFER_PRDC_SHIFT)    /* 0x08000000 */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_BUFACC_SHIFT       (28U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_BUFACC_MASK        (0x1U << USB_GRF_HOST0_STATUS_HOST0_EHCI_BUFACC_SHIFT)       /* 0x10000000 */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_PME_STATUS_SHIFT   (29U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_PME_STATUS_MASK    (0x1U << USB_GRF_HOST0_STATUS_HOST0_EHCI_PME_STATUS_SHIFT)   /* 0x20000000 */
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_POWER_STATE_ACK_SHIFT (30U)
#define USB_GRF_HOST0_STATUS_HOST0_EHCI_POWER_STATE_ACK_MASK (0x1U << USB_GRF_HOST0_STATUS_HOST0_EHCI_POWER_STATE_ACK_SHIFT) /* 0x40000000 */
/******************************************GRF*******************************************/
/* CPU_PEFF_CON0 */
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_PERF_WORK_SHIFT       (0U)
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_PERF_WORK_MASK        (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_AXI_PERF_WORK_SHIFT)       /* 0x00000001 */
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_PERF_CLR_SHIFT        (1U)
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_PERF_CLR_MASK         (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_AXI_PERF_CLR_SHIFT)        /* 0x00000002 */
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_CNT_TYPE_SHIFT        (2U)
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_CNT_TYPE_MASK         (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_AXI_CNT_TYPE_SHIFT)        /* 0x00000004 */
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_CNT_TYPE_WRAP_SHIFT   (3U)
#define GRF_CPU_PEFF_CON0_CPU_SW_AXI_CNT_TYPE_WRAP_MASK    (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_AXI_CNT_TYPE_WRAP_SHIFT)   /* 0x00000008 */
#define GRF_CPU_PEFF_CON0_CPU_SW_AR_CNT_ID_TYPE_SHIFT      (4U)
#define GRF_CPU_PEFF_CON0_CPU_SW_AR_CNT_ID_TYPE_MASK       (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_AR_CNT_ID_TYPE_SHIFT)      /* 0x00000010 */
#define GRF_CPU_PEFF_CON0_CPU_SW_AW_CNT_ID_TYPE_SHIFT      (5U)
#define GRF_CPU_PEFF_CON0_CPU_SW_AW_CNT_ID_TYPE_MASK       (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_AW_CNT_ID_TYPE_SHIFT)      /* 0x00000020 */
#define GRF_CPU_PEFF_CON0_CPU_SW_DDR_ALIGN_TYPE_SHIFT      (6U)
#define GRF_CPU_PEFF_CON0_CPU_SW_DDR_ALIGN_TYPE_MASK       (0x3U << GRF_CPU_PEFF_CON0_CPU_SW_DDR_ALIGN_TYPE_SHIFT)      /* 0x000000C0 */
#define GRF_CPU_PEFF_CON0_CPU_SW_RD_LATENCY_ID_SHIFT       (8U)
#define GRF_CPU_PEFF_CON0_CPU_SW_RD_LATENCY_ID_MASK        (0x3FU << GRF_CPU_PEFF_CON0_CPU_SW_RD_LATENCY_ID_SHIFT)      /* 0x00003F00 */
#define GRF_CPU_PEFF_CON0_CPU_SW_RD_LATENCY_ID_RANGE_E_SHIFT (15U)
#define GRF_CPU_PEFF_CON0_CPU_SW_RD_LATENCY_ID_RANGE_E_MASK (0x1U << GRF_CPU_PEFF_CON0_CPU_SW_RD_LATENCY_ID_RANGE_E_SHIFT) /* 0x00008000 */
/* CPU_PEFF_CON1 */
#define GRF_CPU_PEFF_CON1_CPU_SW_RD_LATENCY_THR_SHIFT      (0U)
#define GRF_CPU_PEFF_CON1_CPU_SW_RD_LATENCY_THR_MASK       (0xFFFU << GRF_CPU_PEFF_CON1_CPU_SW_RD_LATENCY_THR_SHIFT)    /* 0x00000FFF */
/* CPU_PEFF_CON2 */
#define GRF_CPU_PEFF_CON2_CPU_SW_AR_COUNT_ID_SHIFT         (0U)
#define GRF_CPU_PEFF_CON2_CPU_SW_AR_COUNT_ID_MASK          (0x3FU << GRF_CPU_PEFF_CON2_CPU_SW_AR_COUNT_ID_SHIFT)        /* 0x0000003F */
#define GRF_CPU_PEFF_CON2_CPU_SW_AW_COUNT_ID_SHIFT         (8U)
#define GRF_CPU_PEFF_CON2_CPU_SW_AW_COUNT_ID_MASK          (0x1FU << GRF_CPU_PEFF_CON2_CPU_SW_AW_COUNT_ID_SHIFT)        /* 0x00001F00 */
#define GRF_CPU_PEFF_CON2_CPU_SW_AXI_PERF_INT_E_SHIFT      (14U)
#define GRF_CPU_PEFF_CON2_CPU_SW_AXI_PERF_INT_E_MASK       (0x1U << GRF_CPU_PEFF_CON2_CPU_SW_AXI_PERF_INT_E_SHIFT)      /* 0x00004000 */
#define GRF_CPU_PEFF_CON2_CPU_SW_AXI_PERF_INT_CLR_SHIFT    (15U)
#define GRF_CPU_PEFF_CON2_CPU_SW_AXI_PERF_INT_CLR_MASK     (0x1U << GRF_CPU_PEFF_CON2_CPU_SW_AXI_PERF_INT_CLR_SHIFT)    /* 0x00008000 */
/* CPU_PEFF_CON3 */
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_MSK_SHIFT       (0U)
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_MSK_MASK        (0x1U << GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_MSK_SHIFT)       /* 0x00000001 */
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_TYPE_SHIFT      (1U)
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_TYPE_MASK       (0x1U << GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_TYPE_SHIFT)      /* 0x00000002 */
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_BMSK_SHIFT      (4U)
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_BMSK_MASK       (0x3FU << GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_BMSK_SHIFT)     /* 0x000003F0 */
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_SHIFT           (10U)
#define GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_MASK            (0x3FU << GRF_CPU_PEFF_CON3_CPU_SW_AR_MON_ID_SHIFT)          /* 0x0000FC00 */
/* CPU_PEFF_CON4 */
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_MSK_SHIFT       (0U)
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_MSK_MASK        (0x1U << GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_MSK_SHIFT)       /* 0x00000001 */
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_TYPE_SHIFT      (1U)
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_TYPE_MASK       (0x1U << GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_TYPE_SHIFT)      /* 0x00000002 */
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_BMSK_SHIFT      (2U)
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_BMSK_MASK       (0x1FU << GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_BMSK_SHIFT)     /* 0x0000007C */
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_SHIFT           (7U)
#define GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_MASK            (0x1FU << GRF_CPU_PEFF_CON4_CPU_SW_AW_MON_ID_SHIFT)          /* 0x00000F80 */
/* CPU_PEFF_CON5 */
#define GRF_CPU_PEFF_CON5_CPU_SW_ARADDR_MON_ST_SHIFT       (0U)
#define GRF_CPU_PEFF_CON5_CPU_SW_ARADDR_MON_ST_MASK        (0xFFFFFFFFU << GRF_CPU_PEFF_CON5_CPU_SW_ARADDR_MON_ST_SHIFT) /* 0xFFFFFFFF */
/* CPU_PEFF_CON6 */
#define GRF_CPU_PEFF_CON6_CPU_SW_ARADDR_MON_END_SHIFT      (0U)
#define GRF_CPU_PEFF_CON6_CPU_SW_ARADDR_MON_END_MASK       (0xFFFFFFFFU << GRF_CPU_PEFF_CON6_CPU_SW_ARADDR_MON_END_SHIFT) /* 0xFFFFFFFF */
/* CPU_PEFF_CON7 */
#define GRF_CPU_PEFF_CON7_CPU_SW_AWADDR_MON_ST_SHIFT       (0U)
#define GRF_CPU_PEFF_CON7_CPU_SW_AWADDR_MON_ST_MASK        (0xFFFFFFFFU << GRF_CPU_PEFF_CON7_CPU_SW_AWADDR_MON_ST_SHIFT) /* 0xFFFFFFFF */
/* CPU_PEFF_CON8 */
#define GRF_CPU_PEFF_CON8_CPU_SW_AWADDR_MON_END_SHIFT      (0U)
#define GRF_CPU_PEFF_CON8_CPU_SW_AWADDR_MON_END_MASK       (0xFFFFFFFFU << GRF_CPU_PEFF_CON8_CPU_SW_AWADDR_MON_END_SHIFT) /* 0xFFFFFFFF */
/* GIC_BASE_CON */
#define GRF_GIC_BASE_CON_GIC_BASE_ADDR_SHIFT               (0U)
#define GRF_GIC_BASE_CON_GIC_BASE_ADDR_MASK                (0xFFFFU << GRF_GIC_BASE_CON_GIC_BASE_ADDR_SHIFT)            /* 0x0000FFFF */
/* CPU_PERF_RD_MAX_LATENCY_NUM */
#define GRF_CPU_PERF_RD_MAX_LATENCY_NUM_RD_MAX_LATENCY_R_SHIFT (0U)
#define GRF_CPU_PERF_RD_MAX_LATENCY_NUM_RD_MAX_LATENCY_R_MASK (0x1FFFU << GRF_CPU_PERF_RD_MAX_LATENCY_NUM_RD_MAX_LATENCY_R_SHIFT) /* 0x00001FFF */
/* CPU_PERF_RD_LATENCY_SAMP_NUM */
#define GRF_CPU_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_SAMP_R_SHIFT (0U)
#define GRF_CPU_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_SAMP_R_MASK (0x7FFFFFFU << GRF_CPU_PERF_RD_LATENCY_SAMP_NUM_RD_LATENCY_SAMP_R_SHIFT) /* 0x07FFFFFF */
/* CPU_PERF_RD_LATENCY_ACC_NUM */
#define GRF_CPU_PERF_RD_LATENCY_ACC_NUM_RD_LATENCY_ACC_CNT_R_SHIFT (0U)
#define GRF_CPU_PERF_RD_LATENCY_ACC_NUM_RD_LATENCY_ACC_CNT_R_MASK (0xFFFFFFFFU << GRF_CPU_PERF_RD_LATENCY_ACC_NUM_RD_LATENCY_ACC_CNT_R_SHIFT) /* 0xFFFFFFFF */
/* CPU_PERF_RD_AXI_TOTAL_BYTE */
#define GRF_CPU_PERF_RD_AXI_TOTAL_BYTE_RD_AXI_TOTAL_BYTE_SHIFT (0U)
#define GRF_CPU_PERF_RD_AXI_TOTAL_BYTE_RD_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << GRF_CPU_PERF_RD_AXI_TOTAL_BYTE_RD_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* CPU_PERF_WR_AXI_TOTAL_BYTE */
#define GRF_CPU_PERF_WR_AXI_TOTAL_BYTE_WR_AXI_TOTAL_BYTE_SHIFT (0U)
#define GRF_CPU_PERF_WR_AXI_TOTAL_BYTE_WR_AXI_TOTAL_BYTE_MASK (0xFFFFFFFFU << GRF_CPU_PERF_WR_AXI_TOTAL_BYTE_WR_AXI_TOTAL_BYTE_SHIFT) /* 0xFFFFFFFF */
/* CPU_PERF_WORKING_CNT */
#define GRF_CPU_PERF_WORKING_CNT_WORKING_CNT_R_SHIFT       (0U)
#define GRF_CPU_PERF_WORKING_CNT_WORKING_CNT_R_MASK        (0xFFFFFFFFU << GRF_CPU_PERF_WORKING_CNT_WORKING_CNT_R_SHIFT) /* 0xFFFFFFFF */
/* CPU_PERF_INT_STATUS */
#define GRF_CPU_PERF_INT_STATUS_A35_AR_MON_AXI_HIT_FLAG_SHIFT (0U)
#define GRF_CPU_PERF_INT_STATUS_A35_AR_MON_AXI_HIT_FLAG_MASK (0x1U << GRF_CPU_PERF_INT_STATUS_A35_AR_MON_AXI_HIT_FLAG_SHIFT) /* 0x00000001 */
#define GRF_CPU_PERF_INT_STATUS_A35_AR_MON_AXI_ID_STATUS_SHIFT (8U)
#define GRF_CPU_PERF_INT_STATUS_A35_AR_MON_AXI_ID_STATUS_MASK (0x7FU << GRF_CPU_PERF_INT_STATUS_A35_AR_MON_AXI_ID_STATUS_SHIFT) /* 0x00007F00 */
#define GRF_CPU_PERF_INT_STATUS_A35_AW_MON_AXI_HIT_FLAG_SHIFT (16U)
#define GRF_CPU_PERF_INT_STATUS_A35_AW_MON_AXI_HIT_FLAG_MASK (0x1U << GRF_CPU_PERF_INT_STATUS_A35_AW_MON_AXI_HIT_FLAG_SHIFT) /* 0x00010000 */
#define GRF_CPU_PERF_INT_STATUS_A35_AW_MON_AXI_ID_STATUS_SHIFT (24U)
#define GRF_CPU_PERF_INT_STATUS_A35_AW_MON_AXI_ID_STATUS_MASK (0x7FU << GRF_CPU_PERF_INT_STATUS_A35_AW_MON_AXI_ID_STATUS_SHIFT) /* 0x7F000000 */
/* COREPVTM_CON0 */
#define GRF_COREPVTM_CON0_COREPVTM_START_SHIFT             (0U)
#define GRF_COREPVTM_CON0_COREPVTM_START_MASK              (0x1U << GRF_COREPVTM_CON0_COREPVTM_START_SHIFT)             /* 0x00000001 */
#define GRF_COREPVTM_CON0_COREPVTM_OSC_EN_SHIFT            (1U)
#define GRF_COREPVTM_CON0_COREPVTM_OSC_EN_MASK             (0x1U << GRF_COREPVTM_CON0_COREPVTM_OSC_EN_SHIFT)            /* 0x00000002 */
#define GRF_COREPVTM_CON0_COREPVTM_OSC_SEL_SHIFT           (2U)
#define GRF_COREPVTM_CON0_COREPVTM_OSC_SEL_MASK            (0x7U << GRF_COREPVTM_CON0_COREPVTM_OSC_SEL_SHIFT)           /* 0x0000001C */
/* COREPVTM_CON1 */
#define GRF_COREPVTM_CON1_COREPVTM_CAL_CNT_SHIFT           (0U)
#define GRF_COREPVTM_CON1_COREPVTM_CAL_CNT_MASK            (0xFFFFFFFFU << GRF_COREPVTM_CON1_COREPVTM_CAL_CNT_SHIFT)    /* 0xFFFFFFFF */
/* COREPVTM_STATUS0 */
#define GRF_COREPVTM_STATUS0_COREPVTM_FREQ_DONE_SHIFT      (0U)
#define GRF_COREPVTM_STATUS0_COREPVTM_FREQ_DONE_MASK       (0x1U << GRF_COREPVTM_STATUS0_COREPVTM_FREQ_DONE_SHIFT)      /* 0x00000001 */
/* COREPVTM_STATUS1 */
#define GRF_COREPVTM_STATUS1_COREPVTM_FREQ_CNT_SHIFT       (0U)
#define GRF_COREPVTM_STATUS1_COREPVTM_FREQ_CNT_MASK        (0xFFFFFFFFU << GRF_COREPVTM_STATUS1_COREPVTM_FREQ_CNT_SHIFT) /* 0xFFFFFFFF */
/******************************************SGRF******************************************/
/* SOC_CON2 */
#define SGRF_SOC_CON2_ACLK_DMAC_EN_SHIFT                   (0U)
#define SGRF_SOC_CON2_ACLK_DMAC_EN_MASK                    (0x1U << SGRF_SOC_CON2_ACLK_DMAC_EN_SHIFT)                   /* 0x00000001 */
#define SGRF_SOC_CON2_PCLK_STIMER0_EN_SHIFT                (1U)
#define SGRF_SOC_CON2_PCLK_STIMER0_EN_MASK                 (0x1U << SGRF_SOC_CON2_PCLK_STIMER0_EN_SHIFT)                /* 0x00000002 */
#define SGRF_SOC_CON2_PCLK_WDT_S_EN_SHIFT                  (2U)
#define SGRF_SOC_CON2_PCLK_WDT_S_EN_MASK                   (0x1U << SGRF_SOC_CON2_PCLK_WDT_S_EN_SHIFT)                  /* 0x00000004 */
#define SGRF_SOC_CON2_ARESETN_DMAC_REQ_SHIFT               (3U)
#define SGRF_SOC_CON2_ARESETN_DMAC_REQ_MASK                (0x1U << SGRF_SOC_CON2_ARESETN_DMAC_REQ_SHIFT)               /* 0x00000008 */
#define SGRF_SOC_CON2_PRESETN_STIMER0_REQ_SHIFT            (4U)
#define SGRF_SOC_CON2_PRESETN_STIMER0_REQ_MASK             (0x1U << SGRF_SOC_CON2_PRESETN_STIMER0_REQ_SHIFT)            /* 0x00000010 */
#define SGRF_SOC_CON2_RESETN_STIMER0_SRC_SHIFT             (5U)
#define SGRF_SOC_CON2_RESETN_STIMER0_SRC_MASK              (0x1U << SGRF_SOC_CON2_RESETN_STIMER0_SRC_SHIFT)             /* 0x00000020 */
#define SGRF_SOC_CON2_RESETN_STIMER1_SRC_SHIFT             (6U)
#define SGRF_SOC_CON2_RESETN_STIMER1_SRC_MASK              (0x1U << SGRF_SOC_CON2_RESETN_STIMER1_SRC_SHIFT)             /* 0x00000040 */
#define SGRF_SOC_CON2_PRESETN_WDT_S_SRC_SHIFT              (7U)
#define SGRF_SOC_CON2_PRESETN_WDT_S_SRC_MASK               (0x1U << SGRF_SOC_CON2_PRESETN_WDT_S_SRC_SHIFT)              /* 0x00000080 */
#define SGRF_SOC_CON2_PRESETN_EFUSES_SRC_SHIFT             (8U)
#define SGRF_SOC_CON2_PRESETN_EFUSES_SRC_MASK              (0x1U << SGRF_SOC_CON2_PRESETN_EFUSES_SRC_SHIFT)             /* 0x00000100 */
#define SGRF_SOC_CON2_RESETN_EFUSES_SRC_SHIFT              (9U)
#define SGRF_SOC_CON2_RESETN_EFUSES_SRC_MASK               (0x1U << SGRF_SOC_CON2_RESETN_EFUSES_SRC_SHIFT)              /* 0x00000200 */
#define SGRF_SOC_CON2_SGRF_CLK_STIMER0_EN_SHIFT            (10U)
#define SGRF_SOC_CON2_SGRF_CLK_STIMER0_EN_MASK             (0x1U << SGRF_SOC_CON2_SGRF_CLK_STIMER0_EN_SHIFT)            /* 0x00000400 */
#define SGRF_SOC_CON2_SGRF_CLK_STIMER1_EN_SHIFT            (11U)
#define SGRF_SOC_CON2_SGRF_CLK_STIMER1_EN_MASK             (0x1U << SGRF_SOC_CON2_SGRF_CLK_STIMER1_EN_SHIFT)            /* 0x00000800 */
#define SGRF_SOC_CON2_EFUSE_SECURITY_SELECTION_SHIFT       (12U)
#define SGRF_SOC_CON2_EFUSE_SECURITY_SELECTION_MASK        (0x1U << SGRF_SOC_CON2_EFUSE_SECURITY_SELECTION_SHIFT)       /* 0x00001000 */
#define SGRF_SOC_CON2_PCLK_EFUSES_EN_SHIFT                 (13U)
#define SGRF_SOC_CON2_PCLK_EFUSES_EN_MASK                  (0x1U << SGRF_SOC_CON2_PCLK_EFUSES_EN_SHIFT)                 /* 0x00002000 */
#define SGRF_SOC_CON2_WDTS_GLB_RESET_EN_SHIFT              (14U)
#define SGRF_SOC_CON2_WDTS_GLB_RESET_EN_MASK               (0x1U << SGRF_SOC_CON2_WDTS_GLB_RESET_EN_SHIFT)              /* 0x00004000 */
#define SGRF_SOC_CON2_MSCH_AXI_BYPASS_ALL_SHIFT            (15U)
#define SGRF_SOC_CON2_MSCH_AXI_BYPASS_ALL_MASK             (0x1U << SGRF_SOC_CON2_MSCH_AXI_BYPASS_ALL_SHIFT)            /* 0x00008000 */
/* SOC_CON3 */
#define SGRF_SOC_CON3_CRYPTOM_ARPROT_SECURE_CTRL_SHIFT     (0U)
#define SGRF_SOC_CON3_CRYPTOM_ARPROT_SECURE_CTRL_MASK      (0x1U << SGRF_SOC_CON3_CRYPTOM_ARPROT_SECURE_CTRL_SHIFT)     /* 0x00000001 */
#define SGRF_SOC_CON3_CRYPTOM_AWPROT_SECURE_CTRL_SHIFT     (1U)
#define SGRF_SOC_CON3_CRYPTOM_AWPROT_SECURE_CTRL_MASK      (0x1U << SGRF_SOC_CON3_CRYPTOM_AWPROT_SECURE_CTRL_SHIFT)     /* 0x00000002 */
#define SGRF_SOC_CON3_DCFM_ARPROT_SECURE_CTRL_SHIFT        (2U)
#define SGRF_SOC_CON3_DCFM_ARPROT_SECURE_CTRL_MASK         (0x1U << SGRF_SOC_CON3_DCFM_ARPROT_SECURE_CTRL_SHIFT)        /* 0x00000004 */
#define SGRF_SOC_CON3_DCFM_AWPROT_SECURE_CTRL_SHIFT        (3U)
#define SGRF_SOC_CON3_DCFM_AWPROT_SECURE_CTRL_MASK         (0x1U << SGRF_SOC_CON3_DCFM_AWPROT_SECURE_CTRL_SHIFT)        /* 0x00000008 */
#define SGRF_SOC_CON3_PCIEM_ARPROT_SECURE_CTRL_SHIFT       (4U)
#define SGRF_SOC_CON3_PCIEM_ARPROT_SECURE_CTRL_MASK        (0x1U << SGRF_SOC_CON3_PCIEM_ARPROT_SECURE_CTRL_SHIFT)       /* 0x00000010 */
#define SGRF_SOC_CON3_PCIEM_AWPROT_SECURE_CTRL_SHIFT       (5U)
#define SGRF_SOC_CON3_PCIEM_AWPROT_SECURE_CTRL_MASK        (0x1U << SGRF_SOC_CON3_PCIEM_AWPROT_SECURE_CTRL_SHIFT)       /* 0x00000020 */
#define SGRF_SOC_CON3_USB3M_ARPROT_SECURE_CTRL_SHIFT       (6U)
#define SGRF_SOC_CON3_USB3M_ARPROT_SECURE_CTRL_MASK        (0x1U << SGRF_SOC_CON3_USB3M_ARPROT_SECURE_CTRL_SHIFT)       /* 0x00000040 */
#define SGRF_SOC_CON3_USB3M_AWPROT_SECURE_CTRL_SHIFT       (7U)
#define SGRF_SOC_CON3_USB3M_AWPROT_SECURE_CTRL_MASK        (0x1U << SGRF_SOC_CON3_USB3M_AWPROT_SECURE_CTRL_SHIFT)       /* 0x00000080 */
/* SOC_CON4 */
#define SGRF_SOC_CON4_PCIE_CTRL_SECURITY_CTRL_SHIFT        (0U)
#define SGRF_SOC_CON4_PCIE_CTRL_SECURITY_CTRL_MASK         (0x1U << SGRF_SOC_CON4_PCIE_CTRL_SECURITY_CTRL_SHIFT)        /* 0x00000001 */
#define SGRF_SOC_CON4_USB3_SECURITY_CTRL_SHIFT             (1U)
#define SGRF_SOC_CON4_USB3_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON4_USB3_SECURITY_CTRL_SHIFT)             /* 0x00000002 */
#define SGRF_SOC_CON4_GRF_SECURITY_CTRL_SHIFT              (2U)
#define SGRF_SOC_CON4_GRF_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON4_GRF_SECURITY_CTRL_SHIFT)              /* 0x00000004 */
#define SGRF_SOC_CON4_NOC_SRV_SECURITY_CTRL_SHIFT          (3U)
#define SGRF_SOC_CON4_NOC_SRV_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON4_NOC_SRV_SECURITY_CTRL_SHIFT)          /* 0x00000008 */
#define SGRF_SOC_CON4_DEBUG_SECURITY_CTRL_SHIFT            (4U)
#define SGRF_SOC_CON4_DEBUG_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON4_DEBUG_SECURITY_CTRL_SHIFT)            /* 0x00000010 */
#define SGRF_SOC_CON4_GIC_SECURITY_CTRL_SHIFT              (5U)
#define SGRF_SOC_CON4_GIC_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON4_GIC_SECURITY_CTRL_SHIFT)              /* 0x00000020 */
#define SGRF_SOC_CON4_DDR_PHY_SECURITY_CTRL_SHIFT          (6U)
#define SGRF_SOC_CON4_DDR_PHY_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON4_DDR_PHY_SECURITY_CTRL_SHIFT)          /* 0x00000040 */
#define SGRF_SOC_CON4_CRU_SECURITY_CTRL_SHIFT              (7U)
#define SGRF_SOC_CON4_CRU_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON4_CRU_SECURITY_CTRL_SHIFT)              /* 0x00000080 */
#define SGRF_SOC_CON4_CSI_PHY_SECURITY_CTRL_SHIFT          (8U)
#define SGRF_SOC_CON4_CSI_PHY_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON4_CSI_PHY_SECURITY_CTRL_SHIFT)          /* 0x00000100 */
#define SGRF_SOC_CON4_DSI_PHY_SECURITY_CTRL_SHIFT          (9U)
#define SGRF_SOC_CON4_DSI_PHY_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON4_DSI_PHY_SECURITY_CTRL_SHIFT)          /* 0x00000200 */
#define SGRF_SOC_CON4_DFICTRL_SECURITY_CTRL_SHIFT          (10U)
#define SGRF_SOC_CON4_DFICTRL_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON4_DFICTRL_SECURITY_CTRL_SHIFT)          /* 0x00000400 */
#define SGRF_SOC_CON4_USB3_PHY_SECURITY_CTRL_SHIFT         (11U)
#define SGRF_SOC_CON4_USB3_PHY_SECURITY_CTRL_MASK          (0x1U << SGRF_SOC_CON4_USB3_PHY_SECURITY_CTRL_SHIFT)         /* 0x00000800 */
#define SGRF_SOC_CON4_TSADC_SECURITY_CTRL_SHIFT            (12U)
#define SGRF_SOC_CON4_TSADC_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON4_TSADC_SECURITY_CTRL_SHIFT)            /* 0x00001000 */
#define SGRF_SOC_CON4_NSEFUSE_SECURITY_CTRL_SHIFT          (13U)
#define SGRF_SOC_CON4_NSEFUSE_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON4_NSEFUSE_SECURITY_CTRL_SHIFT)          /* 0x00002000 */
#define SGRF_SOC_CON4_SARADC_SECURITY_CTRL_SHIFT           (14U)
#define SGRF_SOC_CON4_SARADC_SECURITY_CTRL_MASK            (0x1U << SGRF_SOC_CON4_SARADC_SECURITY_CTRL_SHIFT)           /* 0x00004000 */
#define SGRF_SOC_CON4_I2C0_SECURITY_CTRL_SHIFT             (15U)
#define SGRF_SOC_CON4_I2C0_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON4_I2C0_SECURITY_CTRL_SHIFT)             /* 0x00008000 */
/* SOC_CON5 */
#define SGRF_SOC_CON5_I2C1_SECURITY_CTRL_SHIFT             (0U)
#define SGRF_SOC_CON5_I2C1_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_I2C1_SECURITY_CTRL_SHIFT)             /* 0x00000001 */
#define SGRF_SOC_CON5_I2C2_SECURITY_CTRL_SHIFT             (1U)
#define SGRF_SOC_CON5_I2C2_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_I2C2_SECURITY_CTRL_SHIFT)             /* 0x00000002 */
#define SGRF_SOC_CON5_I2C3_SECURITY_CTRL_SHIFT             (2U)
#define SGRF_SOC_CON5_I2C3_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_I2C3_SECURITY_CTRL_SHIFT)             /* 0x00000004 */
#define SGRF_SOC_CON5_I2C4_SECURITY_CTRL_SHIFT             (3U)
#define SGRF_SOC_CON5_I2C4_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_I2C4_SECURITY_CTRL_SHIFT)             /* 0x00000008 */
#define SGRF_SOC_CON5_I2C5_SECURITY_CTRL_SHIFT             (4U)
#define SGRF_SOC_CON5_I2C5_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_I2C5_SECURITY_CTRL_SHIFT)             /* 0x00000010 */
#define SGRF_SOC_CON5_SPI0_SECURITY_CTRL_SHIFT             (5U)
#define SGRF_SOC_CON5_SPI0_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_SPI0_SECURITY_CTRL_SHIFT)             /* 0x00000020 */
#define SGRF_SOC_CON5_SPI1_SECURITY_CTRL_SHIFT             (6U)
#define SGRF_SOC_CON5_SPI1_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_SPI1_SECURITY_CTRL_SHIFT)             /* 0x00000040 */
#define SGRF_SOC_CON5_SPI2_SECURITY_CTRL_SHIFT             (7U)
#define SGRF_SOC_CON5_SPI2_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON5_SPI2_SECURITY_CTRL_SHIFT)             /* 0x00000080 */
#define SGRF_SOC_CON5_UART0_SECURITY_CTRL_SHIFT            (8U)
#define SGRF_SOC_CON5_UART0_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART0_SECURITY_CTRL_SHIFT)            /* 0x00000100 */
#define SGRF_SOC_CON5_UART1_SECURITY_CTRL_SHIFT            (9U)
#define SGRF_SOC_CON5_UART1_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART1_SECURITY_CTRL_SHIFT)            /* 0x00000200 */
#define SGRF_SOC_CON5_UART2_SECURITY_CTRL_SHIFT            (10U)
#define SGRF_SOC_CON5_UART2_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART2_SECURITY_CTRL_SHIFT)            /* 0x00000400 */
#define SGRF_SOC_CON5_UART3_SECURITY_CTRL_SHIFT            (11U)
#define SGRF_SOC_CON5_UART3_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART3_SECURITY_CTRL_SHIFT)            /* 0x00000800 */
#define SGRF_SOC_CON5_UART4_SECURITY_CTRL_SHIFT            (12U)
#define SGRF_SOC_CON5_UART4_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART4_SECURITY_CTRL_SHIFT)            /* 0x00001000 */
#define SGRF_SOC_CON5_UART5_SECURITY_CTRL_SHIFT            (13U)
#define SGRF_SOC_CON5_UART5_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART5_SECURITY_CTRL_SHIFT)            /* 0x00002000 */
#define SGRF_SOC_CON5_UART6_SECURITY_CTRL_SHIFT            (14U)
#define SGRF_SOC_CON5_UART6_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART6_SECURITY_CTRL_SHIFT)            /* 0x00004000 */
#define SGRF_SOC_CON5_UART7_SECURITY_CTRL_SHIFT            (15U)
#define SGRF_SOC_CON5_UART7_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON5_UART7_SECURITY_CTRL_SHIFT)            /* 0x00008000 */
/* SOC_CON6 */
#define SGRF_SOC_CON6_CRYPTO_SECURITY_CTRL_SHIFT           (0U)
#define SGRF_SOC_CON6_CRYPTO_SECURITY_CTRL_MASK            (0x1U << SGRF_SOC_CON6_CRYPTO_SECURITY_CTRL_SHIFT)           /* 0x00000001 */
#define SGRF_SOC_CON6_DCF_SECURITY_CTRL_SHIFT              (1U)
#define SGRF_SOC_CON6_DCF_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON6_DCF_SECURITY_CTRL_SHIFT)              /* 0x00000002 */
#define SGRF_SOC_CON6_GPIO1_SECURITY_CTRL_SHIFT            (2U)
#define SGRF_SOC_CON6_GPIO1_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON6_GPIO1_SECURITY_CTRL_SHIFT)            /* 0x00000004 */
#define SGRF_SOC_CON6_GPIO2_SECURITY_CTRL_SHIFT            (3U)
#define SGRF_SOC_CON6_GPIO2_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON6_GPIO2_SECURITY_CTRL_SHIFT)            /* 0x00000008 */
#define SGRF_SOC_CON6_GPIO3_SECURITY_CTRL_SHIFT            (4U)
#define SGRF_SOC_CON6_GPIO3_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON6_GPIO3_SECURITY_CTRL_SHIFT)            /* 0x00000010 */
#define SGRF_SOC_CON6_NSTIMER_SECURITY_CTRL_SHIFT          (5U)
#define SGRF_SOC_CON6_NSTIMER_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON6_NSTIMER_SECURITY_CTRL_SHIFT)          /* 0x00000020 */
#define SGRF_SOC_CON6_PWM0_SECURITY_CTRL_SHIFT             (6U)
#define SGRF_SOC_CON6_PWM0_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON6_PWM0_SECURITY_CTRL_SHIFT)             /* 0x00000040 */
#define SGRF_SOC_CON6_PWM1_SECURITY_CTRL_SHIFT             (7U)
#define SGRF_SOC_CON6_PWM1_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON6_PWM1_SECURITY_CTRL_SHIFT)             /* 0x00000080 */
#define SGRF_SOC_CON6_PWM2_SECURITY_CTRL_SHIFT             (8U)
#define SGRF_SOC_CON6_PWM2_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON6_PWM2_SECURITY_CTRL_SHIFT)             /* 0x00000100 */
#define SGRF_SOC_CON6_NSWDT_SECURITY_CTRL_SHIFT            (9U)
#define SGRF_SOC_CON6_NSWDT_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON6_NSWDT_SECURITY_CTRL_SHIFT)            /* 0x00000200 */
#define SGRF_SOC_CON6_I2S0_SECURITY_CTRL_SHIFT             (10U)
#define SGRF_SOC_CON6_I2S0_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON6_I2S0_SECURITY_CTRL_SHIFT)             /* 0x00000400 */
#define SGRF_SOC_CON6_I2S1_SECURITY_CTRL_SHIFT             (11U)
#define SGRF_SOC_CON6_I2S1_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON6_I2S1_SECURITY_CTRL_SHIFT)             /* 0x00000800 */
#define SGRF_SOC_CON6_PDM_SECURITY_CTRL_SHIFT              (12U)
#define SGRF_SOC_CON6_PDM_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON6_PDM_SECURITY_CTRL_SHIFT)              /* 0x00001000 */
#define SGRF_SOC_CON6_VAD_SECURITY_CTRL_SHIFT              (13U)
#define SGRF_SOC_CON6_VAD_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON6_VAD_SECURITY_CTRL_SHIFT)              /* 0x00002000 */
#define SGRF_SOC_CON6_DDR_STDBY_SECURITY_CTRL_SHIFT        (14U)
#define SGRF_SOC_CON6_DDR_STDBY_SECURITY_CTRL_MASK         (0x1U << SGRF_SOC_CON6_DDR_STDBY_SECURITY_CTRL_SHIFT)        /* 0x00004000 */
#define SGRF_SOC_CON6_DDR_MON_SECURITY_CTRL_SHIFT          (15U)
#define SGRF_SOC_CON6_DDR_MON_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON6_DDR_MON_SECURITY_CTRL_SHIFT)          /* 0x00008000 */
/* SOC_CON7 */
#define SGRF_SOC_CON7_UPCTL_SECURITY_CTRL_SHIFT            (0U)
#define SGRF_SOC_CON7_UPCTL_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON7_UPCTL_SECURITY_CTRL_SHIFT)            /* 0x00000001 */
#define SGRF_SOC_CON7_CIF_SECURITY_CTRL_SHIFT              (1U)
#define SGRF_SOC_CON7_CIF_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON7_CIF_SECURITY_CTRL_SHIFT)              /* 0x00000002 */
#define SGRF_SOC_CON7_RGA_SECURITY_CTRL_SHIFT              (2U)
#define SGRF_SOC_CON7_RGA_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON7_RGA_SECURITY_CTRL_SHIFT)              /* 0x00000004 */
#define SGRF_SOC_CON7_VOP_LITE_SECURITY_CTRL_SHIFT         (3U)
#define SGRF_SOC_CON7_VOP_LITE_SECURITY_CTRL_MASK          (0x1U << SGRF_SOC_CON7_VOP_LITE_SECURITY_CTRL_SHIFT)         /* 0x00000008 */
#define SGRF_SOC_CON7_CSI_RX_SECURITY_CTRL_SHIFT           (4U)
#define SGRF_SOC_CON7_CSI_RX_SECURITY_CTRL_MASK            (0x1U << SGRF_SOC_CON7_CSI_RX_SECURITY_CTRL_SHIFT)           /* 0x00000010 */
#define SGRF_SOC_CON7_CSI_TX_SECURITY_CTRL_SHIFT           (5U)
#define SGRF_SOC_CON7_CSI_TX_SECURITY_CTRL_MASK            (0x1U << SGRF_SOC_CON7_CSI_TX_SECURITY_CTRL_SHIFT)           /* 0x00000020 */
#define SGRF_SOC_CON7_DSI_SECURITY_CTRL_SHIFT              (6U)
#define SGRF_SOC_CON7_DSI_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON7_DSI_SECURITY_CTRL_SHIFT)              /* 0x00000040 */
#define SGRF_SOC_CON7_VOP_RAW_SECURITY_CTRL_SHIFT          (7U)
#define SGRF_SOC_CON7_VOP_RAW_SECURITY_CTRL_MASK           (0x1U << SGRF_SOC_CON7_VOP_RAW_SECURITY_CTRL_SHIFT)          /* 0x00000080 */
#define SGRF_SOC_CON7_DLNN_SECURITY_CTRL_SHIFT             (8U)
#define SGRF_SOC_CON7_DLNN_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON7_DLNN_SECURITY_CTRL_SHIFT)             /* 0x00000100 */
#define SGRF_SOC_CON7_SFC_SECURITY_CTRL_SHIFT              (9U)
#define SGRF_SOC_CON7_SFC_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON7_SFC_SECURITY_CTRL_SHIFT)              /* 0x00000200 */
#define SGRF_SOC_CON7_SDIO_SECURITY_CTRL_SHIFT             (10U)
#define SGRF_SOC_CON7_SDIO_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON7_SDIO_SECURITY_CTRL_SHIFT)             /* 0x00000400 */
#define SGRF_SOC_CON7_SDMMC_SECURITY_CTRL_SHIFT            (11U)
#define SGRF_SOC_CON7_SDMMC_SECURITY_CTRL_MASK             (0x1U << SGRF_SOC_CON7_SDMMC_SECURITY_CTRL_SHIFT)            /* 0x00000800 */
#define SGRF_SOC_CON7_EMMC_SECURITY_CTRL_SHIFT             (12U)
#define SGRF_SOC_CON7_EMMC_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON7_EMMC_SECURITY_CTRL_SHIFT)             /* 0x00001000 */
#define SGRF_SOC_CON7_USB2_HOST_SECURITY_CTRL_SHIFT        (13U)
#define SGRF_SOC_CON7_USB2_HOST_SECURITY_CTRL_MASK         (0x1U << SGRF_SOC_CON7_USB2_HOST_SECURITY_CTRL_SHIFT)        /* 0x00002000 */
#define SGRF_SOC_CON7_GMAC_SECURITY_CTRL_SHIFT             (14U)
#define SGRF_SOC_CON7_GMAC_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON7_GMAC_SECURITY_CTRL_SHIFT)             /* 0x00004000 */
#define SGRF_SOC_CON7_VPU_SECURITY_CTRL_SHIFT              (15U)
#define SGRF_SOC_CON7_VPU_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON7_VPU_SECURITY_CTRL_SHIFT)              /* 0x00008000 */
/* SOC_CON8 */
#define SGRF_SOC_CON8_ISP_SECURITY_CTRL_SHIFT              (0U)
#define SGRF_SOC_CON8_ISP_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON8_ISP_SECURITY_CTRL_SHIFT)              /* 0x00000001 */
#define SGRF_SOC_CON8_GPIO_SECURITY_CTRL_SHIFT             (1U)
#define SGRF_SOC_CON8_GPIO_SECURITY_CTRL_MASK              (0x1U << SGRF_SOC_CON8_GPIO_SECURITY_CTRL_SHIFT)             /* 0x00000002 */
#define SGRF_SOC_CON8_DMA_NS_SECURITY_CTRL_SHIFT           (2U)
#define SGRF_SOC_CON8_DMA_NS_SECURITY_CTRL_MASK            (0x1U << SGRF_SOC_CON8_DMA_NS_SECURITY_CTRL_SHIFT)           /* 0x00000004 */
#define SGRF_SOC_CON8_PMU_SECURITY_CTRL_SHIFT              (3U)
#define SGRF_SOC_CON8_PMU_SECURITY_CTRL_MASK               (0x1U << SGRF_SOC_CON8_PMU_SECURITY_CTRL_SHIFT)              /* 0x00000008 */
#define SGRF_SOC_CON8_SPINLOCK_SECURITY_CTRL_SHIFT         (4U)
#define SGRF_SOC_CON8_SPINLOCK_SECURITY_CTRL_MASK          (0x1U << SGRF_SOC_CON8_SPINLOCK_SECURITY_CTRL_SHIFT)         /* 0x00000010 */
/* DMAC_CON0 */
#define SGRF_DMAC_CON0_DMA_IRQ_BOOT_NONSEC_SHIFT           (0U)
#define SGRF_DMAC_CON0_DMA_IRQ_BOOT_NONSEC_MASK            (0xFFFFU << SGRF_DMAC_CON0_DMA_IRQ_BOOT_NONSEC_SHIFT)        /* 0x0000FFFF */
/* DMAC_CON1 */
#define SGRF_DMAC_CON1_DMA_PERI_CH_NONSEC_15_0_SHIFT       (0U)
#define SGRF_DMAC_CON1_DMA_PERI_CH_NONSEC_15_0_MASK        (0xFFFFU << SGRF_DMAC_CON1_DMA_PERI_CH_NONSEC_15_0_SHIFT)    /* 0x0000FFFF */
/* DMAC_CON2 */
#define SGRF_DMAC_CON2_DMA_PERI_CH_NONSEC_31_16_SHIFT      (0U)
#define SGRF_DMAC_CON2_DMA_PERI_CH_NONSEC_31_16_MASK       (0xFU << SGRF_DMAC_CON2_DMA_PERI_CH_NONSEC_31_16_SHIFT)      /* 0x0000000F */
/* DMAC_CON3 */
#define SGRF_DMAC_CON3_DMA_MANAGER_BOOT_NONSEC_SHIFT       (0U)
#define SGRF_DMAC_CON3_DMA_MANAGER_BOOT_NONSEC_MASK        (0x1U << SGRF_DMAC_CON3_DMA_MANAGER_BOOT_NONSEC_SHIFT)       /* 0x00000001 */
/* DMAC_CON4 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_0_TX_SHIFT            (0U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_0_TX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_0_TX_SHIFT)            /* 0x00000003 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_0_RX_SHIFT            (2U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_0_RX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_0_RX_SHIFT)            /* 0x0000000C */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_1_TX_SHIFT            (4U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_1_TX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_1_TX_SHIFT)            /* 0x00000030 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_1_RX_SHIFT            (6U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_1_RX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_1_RX_SHIFT)            /* 0x000000C0 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_2_TX_SHIFT            (8U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_2_TX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_2_TX_SHIFT)            /* 0x00000300 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_2_RX_SHIFT            (10U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_2_RX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_2_RX_SHIFT)            /* 0x00000C00 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_3_TX_SHIFT            (12U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_3_TX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_3_TX_SHIFT)            /* 0x00003000 */
#define SGRF_DMAC_CON4_GRF_TYPE_UART_3_RX_SHIFT            (14U)
#define SGRF_DMAC_CON4_GRF_TYPE_UART_3_RX_MASK             (0x3U << SGRF_DMAC_CON4_GRF_TYPE_UART_3_RX_SHIFT)            /* 0x0000C000 */
/* DMAC_CON5 */
#define SGRF_DMAC_CON5_GRF_TYPE_UART_4_TX_SHIFT            (0U)
#define SGRF_DMAC_CON5_GRF_TYPE_UART_4_TX_MASK             (0x3U << SGRF_DMAC_CON5_GRF_TYPE_UART_4_TX_SHIFT)            /* 0x00000003 */
#define SGRF_DMAC_CON5_GRF_TYPE_UART_4_RX_SHIFT            (2U)
#define SGRF_DMAC_CON5_GRF_TYPE_UART_4_RX_MASK             (0x3U << SGRF_DMAC_CON5_GRF_TYPE_UART_4_RX_SHIFT)            /* 0x0000000C */
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_0_TX_SHIFT             (4U)
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_0_TX_MASK              (0x3U << SGRF_DMAC_CON5_GRF_TYPE_SPI_0_TX_SHIFT)             /* 0x00000030 */
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_0_RX_SHIFT             (6U)
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_0_RX_MASK              (0x3U << SGRF_DMAC_CON5_GRF_TYPE_SPI_0_RX_SHIFT)             /* 0x000000C0 */
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_1_TX_SHIFT             (8U)
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_1_TX_MASK              (0x3U << SGRF_DMAC_CON5_GRF_TYPE_SPI_1_TX_SHIFT)             /* 0x00000300 */
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_1_RX_SHIFT             (10U)
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_1_RX_MASK              (0x3U << SGRF_DMAC_CON5_GRF_TYPE_SPI_1_RX_SHIFT)             /* 0x00000C00 */
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_2_TX_SHIFT             (12U)
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_2_TX_MASK              (0x3U << SGRF_DMAC_CON5_GRF_TYPE_SPI_2_TX_SHIFT)             /* 0x00003000 */
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_2_RX_SHIFT             (14U)
#define SGRF_DMAC_CON5_GRF_TYPE_SPI_2_RX_MASK              (0x3U << SGRF_DMAC_CON5_GRF_TYPE_SPI_2_RX_SHIFT)             /* 0x0000C000 */
/* DMAC_CON6 */
#define SGRF_DMAC_CON6_GRF_TYPE_I2S0_8CH_TX_SHIFT          (0U)
#define SGRF_DMAC_CON6_GRF_TYPE_I2S0_8CH_TX_MASK           (0x3U << SGRF_DMAC_CON6_GRF_TYPE_I2S0_8CH_TX_SHIFT)          /* 0x00000003 */
#define SGRF_DMAC_CON6_GRF_TYPE_I2S0_8CH_RX_SHIFT          (2U)
#define SGRF_DMAC_CON6_GRF_TYPE_I2S0_8CH_RX_MASK           (0x3U << SGRF_DMAC_CON6_GRF_TYPE_I2S0_8CH_RX_SHIFT)          /* 0x0000000C */
#define SGRF_DMAC_CON6_GRF_TYPE_I2S1_2CH_TX_SHIFT          (4U)
#define SGRF_DMAC_CON6_GRF_TYPE_I2S1_2CH_TX_MASK           (0x3U << SGRF_DMAC_CON6_GRF_TYPE_I2S1_2CH_TX_SHIFT)          /* 0x00000030 */
#define SGRF_DMAC_CON6_GRF_TYPE_I2S1_2CH_RX_SHIFT          (6U)
#define SGRF_DMAC_CON6_GRF_TYPE_I2S1_2CH_RX_MASK           (0x3U << SGRF_DMAC_CON6_GRF_TYPE_I2S1_2CH_RX_SHIFT)          /* 0x000000C0 */
#define SGRF_DMAC_CON6_GRF_TYPE_PWM_0_TX_SHIFT             (10U)
#define SGRF_DMAC_CON6_GRF_TYPE_PWM_0_TX_MASK              (0x3U << SGRF_DMAC_CON6_GRF_TYPE_PWM_0_TX_SHIFT)             /* 0x00000C00 */
#define SGRF_DMAC_CON6_GRF_TYPE_PWM_1_TX_SHIFT             (12U)
#define SGRF_DMAC_CON6_GRF_TYPE_PWM_1_TX_MASK              (0x3U << SGRF_DMAC_CON6_GRF_TYPE_PWM_1_TX_SHIFT)             /* 0x00003000 */
#define SGRF_DMAC_CON6_GRF_TYPE_PWM_2_TX_SHIFT             (14U)
#define SGRF_DMAC_CON6_GRF_TYPE_PWM_2_TX_MASK              (0x3U << SGRF_DMAC_CON6_GRF_TYPE_PWM_2_TX_SHIFT)             /* 0x0000C000 */
/* DMAC_CON7 */
#define SGRF_DMAC_CON7_GRF_TYPE_PDM_RX_SHIFT               (0U)
#define SGRF_DMAC_CON7_GRF_TYPE_PDM_RX_MASK                (0x3U << SGRF_DMAC_CON7_GRF_TYPE_PDM_RX_SHIFT)               /* 0x00000003 */
#define SGRF_DMAC_CON7_GRF_TYPE_UART_5_TX_SHIFT            (2U)
#define SGRF_DMAC_CON7_GRF_TYPE_UART_5_TX_MASK             (0x3U << SGRF_DMAC_CON7_GRF_TYPE_UART_5_TX_SHIFT)            /* 0x0000000C */
#define SGRF_DMAC_CON7_GRF_TYPE_UART_5_RX_SHIFT            (4U)
#define SGRF_DMAC_CON7_GRF_TYPE_UART_5_RX_MASK             (0x3U << SGRF_DMAC_CON7_GRF_TYPE_UART_5_RX_SHIFT)            /* 0x00000030 */
#define SGRF_DMAC_CON7_GRF_TYPE_UART_6_TX_SHIFT            (6U)
#define SGRF_DMAC_CON7_GRF_TYPE_UART_6_TX_MASK             (0x3U << SGRF_DMAC_CON7_GRF_TYPE_UART_6_TX_SHIFT)            /* 0x000000C0 */
#define SGRF_DMAC_CON7_GRF_TYPE_UART_6_RX_SHIFT            (8U)
#define SGRF_DMAC_CON7_GRF_TYPE_UART_6_RX_MASK             (0x3U << SGRF_DMAC_CON7_GRF_TYPE_UART_6_RX_SHIFT)            /* 0x00000300 */
#define SGRF_DMAC_CON7_GRF_TYPE_UART_7_TX_SHIFT            (10U)
#define SGRF_DMAC_CON7_GRF_TYPE_UART_7_TX_MASK             (0x3U << SGRF_DMAC_CON7_GRF_TYPE_UART_7_TX_SHIFT)            /* 0x00000C00 */
#define SGRF_DMAC_CON7_GRF_TYPE_UART_7_RX_SHIFT            (12U)
#define SGRF_DMAC_CON7_GRF_TYPE_UART_7_RX_MASK             (0x3U << SGRF_DMAC_CON7_GRF_TYPE_UART_7_RX_SHIFT)            /* 0x00003000 */
/* DMAC_CON8 */
#define SGRF_DMAC_CON8_GRF_DMA_REQ_MODIFY_DIS_SHIFT        (0U)
#define SGRF_DMAC_CON8_GRF_DMA_REQ_MODIFY_DIS_MASK         (0xFFFFU << SGRF_DMAC_CON8_GRF_DMA_REQ_MODIFY_DIS_SHIFT)     /* 0x0000FFFF */
/* DMAC_CON9 */
#define SGRF_DMAC_CON9_GRF_DMA_REQ_MODIFY_DIS_SHIFT        (0U)
#define SGRF_DMAC_CON9_GRF_DMA_REQ_MODIFY_DIS_MASK         (0xFFFFU << SGRF_DMAC_CON9_GRF_DMA_REQ_MODIFY_DIS_SHIFT)     /* 0x0000FFFF */
/* DDR_BANKHASH_CTRL */
#define SGRF_DDR_BANKHASH_CTRL_HASH_EN_SHIFT               (0U)
#define SGRF_DDR_BANKHASH_CTRL_HASH_EN_MASK                (0x1U << SGRF_DDR_BANKHASH_CTRL_HASH_EN_SHIFT)               /* 0x00000001 */
#define SGRF_DDR_BANKHASH_CTRL_MANICURE_MASK_SHIFT         (1U)
#define SGRF_DDR_BANKHASH_CTRL_MANICURE_MASK_MASK          (0x7U << SGRF_DDR_BANKHASH_CTRL_MANICURE_MASK_SHIFT)         /* 0x0000000E */
#define SGRF_DDR_BANKHASH_CTRL_BANK_OFFSET_SHIFT           (4U)
#define SGRF_DDR_BANKHASH_CTRL_BANK_OFFSET_MASK            (0x7U << SGRF_DDR_BANKHASH_CTRL_BANK_OFFSET_SHIFT)           /* 0x00000070 */
/* DDR_BANK_MASK0 */
#define SGRF_DDR_BANK_MASK0_DDR_BANK_MASK0_SHIFT           (0U)
#define SGRF_DDR_BANK_MASK0_DDR_BANK_MASK0_MASK            (0xFFFFFFFFU << SGRF_DDR_BANK_MASK0_DDR_BANK_MASK0_SHIFT)    /* 0xFFFFFFFF */
/* DDR_BANK_MASK1 */
#define SGRF_DDR_BANK_MASK1_DDR_BANK_MASK1_SHIFT           (0U)
#define SGRF_DDR_BANK_MASK1_DDR_BANK_MASK1_MASK            (0xFFFFFFFFU << SGRF_DDR_BANK_MASK1_DDR_BANK_MASK1_SHIFT)    /* 0xFFFFFFFF */
/* DDR_BANK_MASK2 */
#define SGRF_DDR_BANK_MASK2_DDR_BANK_MASK2_SHIFT           (0U)
#define SGRF_DDR_BANK_MASK2_DDR_BANK_MASK2_MASK            (0xFFFFFFFFU << SGRF_DDR_BANK_MASK2_DDR_BANK_MASK2_SHIFT)    /* 0xFFFFFFFF */
/****************************************PMU_SGRF****************************************/
/* SOC_CON0 */
#define PMU_SGRF_SOC_CON0_DBGEN_SHIFT                      (0U)
#define PMU_SGRF_SOC_CON0_DBGEN_MASK                       (0x1U << PMU_SGRF_SOC_CON0_DBGEN_SHIFT)                      /* 0x00000001 */
#define PMU_SGRF_SOC_CON0_SPIDEN_SHIFT                     (1U)
#define PMU_SGRF_SOC_CON0_SPIDEN_MASK                      (0x1U << PMU_SGRF_SOC_CON0_SPIDEN_SHIFT)                     /* 0x00000002 */
#define PMU_SGRF_SOC_CON0_NIDEN_SHIFT                      (2U)
#define PMU_SGRF_SOC_CON0_NIDEN_MASK                       (0x1U << PMU_SGRF_SOC_CON0_NIDEN_SHIFT)                      /* 0x00000004 */
#define PMU_SGRF_SOC_CON0_SPNIDEN_SHIFT                    (3U)
#define PMU_SGRF_SOC_CON0_SPNIDEN_MASK                     (0x1U << PMU_SGRF_SOC_CON0_SPNIDEN_SHIFT)                    /* 0x00000008 */
#define PMU_SGRF_SOC_CON0_DAPDEVICEEN_SHIFT                (4U)
#define PMU_SGRF_SOC_CON0_DAPDEVICEEN_MASK                 (0x1U << PMU_SGRF_SOC_CON0_DAPDEVICEEN_SHIFT)                /* 0x00000010 */
#define PMU_SGRF_SOC_CON0_AA64NAA32_SHIFT                  (5U)
#define PMU_SGRF_SOC_CON0_AA64NAA32_MASK                   (0x1U << PMU_SGRF_SOC_CON0_AA64NAA32_SHIFT)                  /* 0x00000020 */
#define PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_MODE_SHIFT      (6U)
#define PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_MODE_MASK       (0x1U << PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_MODE_SHIFT)      /* 0x00000040 */
#define PMU_SGRF_SOC_CON0_CRYPTODISABLE_SHIFT              (7U)
#define PMU_SGRF_SOC_CON0_CRYPTODISABLE_MASK               (0x1U << PMU_SGRF_SOC_CON0_CRYPTODISABLE_SHIFT)              /* 0x00000080 */
#define PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_EN_SHIFT        (8U)
#define PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_EN_MASK         (0x1U << PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_EN_SHIFT)        /* 0x00000100 */
#define PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_LOAD_SHIFT      (9U)
#define PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_LOAD_MASK       (0x1U << PMU_SGRF_SOC_CON0_DDR_DFI_SCRAMBLE_LOAD_SHIFT)      /* 0x00000200 */
#define PMU_SGRF_SOC_CON0_RESETOUT_FIRST_RESET_DIS_SHIFT   (10U)
#define PMU_SGRF_SOC_CON0_RESETOUT_FIRST_RESET_DIS_MASK    (0x1U << PMU_SGRF_SOC_CON0_RESETOUT_FIRST_RESET_DIS_SHIFT)   /* 0x00000400 */
#define PMU_SGRF_SOC_CON0_RESETOUT_TSADC_SHUT_DIS_SHIFT    (11U)
#define PMU_SGRF_SOC_CON0_RESETOUT_TSADC_SHUT_DIS_MASK     (0x1U << PMU_SGRF_SOC_CON0_RESETOUT_TSADC_SHUT_DIS_SHIFT)    /* 0x00000800 */
#define PMU_SGRF_SOC_CON0_RESETOUT_WDT_DISABLE_SHIFT       (12U)
#define PMU_SGRF_SOC_CON0_RESETOUT_WDT_DISABLE_MASK        (0x1U << PMU_SGRF_SOC_CON0_RESETOUT_WDT_DISABLE_SHIFT)       /* 0x00001000 */
#define PMU_SGRF_SOC_CON0_DBG_LOCK_SHIFT                   (15U)
#define PMU_SGRF_SOC_CON0_DBG_LOCK_MASK                    (0x1U << PMU_SGRF_SOC_CON0_DBG_LOCK_SHIFT)                   /* 0x00008000 */
/* SOC_CON1 */
#define PMU_SGRF_SOC_CON1_IMEM0_TZMA_R0SIZE_SHIFT          (0U)
#define PMU_SGRF_SOC_CON1_IMEM0_TZMA_R0SIZE_MASK           (0x3FFU << PMU_SGRF_SOC_CON1_IMEM0_TZMA_R0SIZE_SHIFT)        /* 0x000003FF */
#define PMU_SGRF_SOC_CON1_REMAP_SHIFT                      (10U)
#define PMU_SGRF_SOC_CON1_REMAP_MASK                       (0x3U << PMU_SGRF_SOC_CON1_REMAP_SHIFT)                      /* 0x00000C00 */
#define PMU_SGRF_SOC_CON1_SPIM_NON_DBG_SHIFT               (12U)
#define PMU_SGRF_SOC_CON1_SPIM_NON_DBG_MASK                (0x1U << PMU_SGRF_SOC_CON1_SPIM_NON_DBG_SHIFT)               /* 0x00001000 */
#define PMU_SGRF_SOC_CON1_SGRF_EFUSE_GPIO_EN_SHIFT         (13U)
#define PMU_SGRF_SOC_CON1_SGRF_EFUSE_GPIO_EN_MASK          (0x1U << PMU_SGRF_SOC_CON1_SGRF_EFUSE_GPIO_EN_SHIFT)         /* 0x00002000 */
#define PMU_SGRF_SOC_CON1_REMAP_LOCK_SHIFT                 (15U)
#define PMU_SGRF_SOC_CON1_REMAP_LOCK_MASK                  (0x1U << PMU_SGRF_SOC_CON1_REMAP_LOCK_SHIFT)                 /* 0x00008000 */
/* FAST_BOOT_ADDR */
#define PMU_SGRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_SHIFT       (0U)
#define PMU_SGRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_MASK        (0xFFFFFFFFU << PMU_SGRF_FAST_BOOT_ADDR_FAST_BOOT_ADDR_SHIFT) /* 0xFFFFFFFF */
/******************************************MSCH******************************************/
/* ID_COREID */
#define MSCH_ID_COREID_CORETYPEID_SHIFT                    (0U)
#define MSCH_ID_COREID_CORETYPEID_MASK                     (0xFFU << MSCH_ID_COREID_CORETYPEID_SHIFT)                   /* 0x000000FF */
#define MSCH_ID_COREID_CORECHECKSUM_SHIFT                  (8U)
#define MSCH_ID_COREID_CORECHECKSUM_MASK                   (0xFFFFFFU << MSCH_ID_COREID_CORECHECKSUM_SHIFT)             /* 0xFFFFFF00 */
/* ID_REVISIONID */
#define MSCH_ID_REVISIONID_REVISIONID_SHIFT                (0U)
#define MSCH_ID_REVISIONID_REVISIONID_MASK                 (0xFFFFFFFFU << MSCH_ID_REVISIONID_REVISIONID_SHIFT)         /* 0xFFFFFFFF */
/* DEVICECONF */
#define MSCH_DEVICECONF_RANK0_SHIFT                        (0U)
#define MSCH_DEVICECONF_RANK0_MASK                         (0x3FU << MSCH_DEVICECONF_RANK0_SHIFT)                       /* 0x0000003F */
#define MSCH_DEVICECONF_RANK1_SHIFT                        (8U)
#define MSCH_DEVICECONF_RANK1_MASK                         (0x3FU << MSCH_DEVICECONF_RANK1_SHIFT)                       /* 0x00003F00 */
/* DEVICESIZE */
#define MSCH_DEVICESIZE_RANK0_SHIFT                        (0U)
#define MSCH_DEVICESIZE_RANK0_MASK                         (0x7FU << MSCH_DEVICESIZE_RANK0_SHIFT)                       /* 0x0000007F */
#define MSCH_DEVICESIZE_RANK1_SHIFT                        (8U)
#define MSCH_DEVICESIZE_RANK1_MASK                         (0x7FU << MSCH_DEVICESIZE_RANK1_SHIFT)                       /* 0x00007F00 */
/* DDRTIMINGA0 */
#define MSCH_DDRTIMINGA0_ACTTOACT_SHIFT                    (0U)
#define MSCH_DDRTIMINGA0_ACTTOACT_MASK                     (0x3FU << MSCH_DDRTIMINGA0_ACTTOACT_SHIFT)                   /* 0x0000003F */
#define MSCH_DDRTIMINGA0_RDTOMISS_SHIFT                    (8U)
#define MSCH_DDRTIMINGA0_RDTOMISS_MASK                     (0x3FU << MSCH_DDRTIMINGA0_RDTOMISS_SHIFT)                   /* 0x00003F00 */
#define MSCH_DDRTIMINGA0_WRTOMISS_SHIFT                    (16U)
#define MSCH_DDRTIMINGA0_WRTOMISS_MASK                     (0x3FU << MSCH_DDRTIMINGA0_WRTOMISS_SHIFT)                   /* 0x003F0000 */
#define MSCH_DDRTIMINGA0_READLATENCY_SHIFT                 (24U)
#define MSCH_DDRTIMINGA0_READLATENCY_MASK                  (0xFFU << MSCH_DDRTIMINGA0_READLATENCY_SHIFT)                /* 0xFF000000 */
/* DDRTIMINGB0 */
#define MSCH_DDRTIMINGB0_RDTOWR_SHIFT                      (0U)
#define MSCH_DDRTIMINGB0_RDTOWR_MASK                       (0x1FU << MSCH_DDRTIMINGB0_RDTOWR_SHIFT)                     /* 0x0000001F */
#define MSCH_DDRTIMINGB0_WRTORD_SHIFT                      (8U)
#define MSCH_DDRTIMINGB0_WRTORD_MASK                       (0x1FU << MSCH_DDRTIMINGB0_WRTORD_SHIFT)                     /* 0x00001F00 */
#define MSCH_DDRTIMINGB0_RRD_SHIFT                         (16U)
#define MSCH_DDRTIMINGB0_RRD_MASK                          (0xFU << MSCH_DDRTIMINGB0_RRD_SHIFT)                         /* 0x000F0000 */
#define MSCH_DDRTIMINGB0_FAW_SHIFT                         (24U)
#define MSCH_DDRTIMINGB0_FAW_MASK                          (0x3FU << MSCH_DDRTIMINGB0_FAW_SHIFT)                        /* 0x3F000000 */
/* DDRTIMINGC0 */
#define MSCH_DDRTIMINGC0_BURSTPENALTY_SHIFT                (0U)
#define MSCH_DDRTIMINGC0_BURSTPENALTY_MASK                 (0xFU << MSCH_DDRTIMINGC0_BURSTPENALTY_SHIFT)                /* 0x0000000F */
#define MSCH_DDRTIMINGC0_WRTOMWR_SHIFT                     (8U)
#define MSCH_DDRTIMINGC0_WRTOMWR_MASK                      (0x3FU << MSCH_DDRTIMINGC0_WRTOMWR_SHIFT)                    /* 0x00003F00 */
/* DEVTODEV0 */
#define MSCH_DEVTODEV0_BUSRDTORD_SHIFT                     (0U)
#define MSCH_DEVTODEV0_BUSRDTORD_MASK                      (0x7U << MSCH_DEVTODEV0_BUSRDTORD_SHIFT)                     /* 0x00000007 */
#define MSCH_DEVTODEV0_BUSRDTOWR_SHIFT                     (4U)
#define MSCH_DEVTODEV0_BUSRDTOWR_MASK                      (0xFU << MSCH_DEVTODEV0_BUSRDTOWR_SHIFT)                     /* 0x000000F0 */
#define MSCH_DEVTODEV0_BUSWRTORD_SHIFT                     (8U)
#define MSCH_DEVTODEV0_BUSWRTORD_MASK                      (0xFU << MSCH_DEVTODEV0_BUSWRTORD_SHIFT)                     /* 0x00000F00 */
#define MSCH_DEVTODEV0_BUSWRTOWR_SHIFT                     (12U)
#define MSCH_DEVTODEV0_BUSWRTOWR_MASK                      (0x7U << MSCH_DEVTODEV0_BUSWRTOWR_SHIFT)                     /* 0x00007000 */
/* DDRMODE */
#define MSCH_DDRMODE_AUTOPRECHARGE_SHIFT                   (0U)
#define MSCH_DDRMODE_AUTOPRECHARGE_MASK                    (0x1U << MSCH_DDRMODE_AUTOPRECHARGE_SHIFT)                   /* 0x00000001 */
#define MSCH_DDRMODE_BYPASSFILTERING_SHIFT                 (1U)
#define MSCH_DDRMODE_BYPASSFILTERING_MASK                  (0x1U << MSCH_DDRMODE_BYPASSFILTERING_SHIFT)                 /* 0x00000002 */
#define MSCH_DDRMODE_FAWBANK_SHIFT                         (2U)
#define MSCH_DDRMODE_FAWBANK_MASK                          (0x1U << MSCH_DDRMODE_FAWBANK_SHIFT)                         /* 0x00000004 */
#define MSCH_DDRMODE_BURSTSIZE_SHIFT                       (3U)
#define MSCH_DDRMODE_BURSTSIZE_MASK                        (0x3U << MSCH_DDRMODE_BURSTSIZE_SHIFT)                       /* 0x00000018 */
#define MSCH_DDRMODE_MWRSIZE_SHIFT                         (5U)
#define MSCH_DDRMODE_MWRSIZE_MASK                          (0x3U << MSCH_DDRMODE_MWRSIZE_SHIFT)                         /* 0x00000060 */
#define MSCH_DDRMODE_FORCEORDER_SHIFT                      (8U)
#define MSCH_DDRMODE_FORCEORDER_MASK                       (0xFFU << MSCH_DDRMODE_FORCEORDER_SHIFT)                     /* 0x0000FF00 */
#define MSCH_DDRMODE_FORCEORDERSTATE_SHIFT                 (16U)
#define MSCH_DDRMODE_FORCEORDERSTATE_MASK                  (0xFFU << MSCH_DDRMODE_FORCEORDERSTATE_SHIFT)                /* 0x00FF0000 */
/* DDR4TIMING */
#define MSCH_DDR4TIMING_CCDL_SHIFT                         (0U)
#define MSCH_DDR4TIMING_CCDL_MASK                          (0x7U << MSCH_DDR4TIMING_CCDL_SHIFT)                         /* 0x00000007 */
#define MSCH_DDR4TIMING_WRTORDL_SHIFT                      (3U)
#define MSCH_DDR4TIMING_WRTORDL_MASK                       (0x1FU << MSCH_DDR4TIMING_WRTORDL_SHIFT)                     /* 0x000000F8 */
#define MSCH_DDR4TIMING_RRDL_SHIFT                         (8U)
#define MSCH_DDR4TIMING_RRDL_MASK                          (0xFU << MSCH_DDR4TIMING_RRDL_SHIFT)                         /* 0x00000F00 */
/* AGINGX0 */
#define MSCH_AGINGX0_AGINGX0_SHIFT                         (0U)
#define MSCH_AGINGX0_AGINGX0_MASK                          (0x7U << MSCH_AGINGX0_AGINGX0_SHIFT)                         /* 0x00000007 */
/* AGING0 */
#define MSCH_AGING0_AGING0_SHIFT                           (0U)
#define MSCH_AGING0_AGING0_MASK                            (0xFFU << MSCH_AGING0_AGING0_SHIFT)                          /* 0x000000FF */
/* AGING1 */
#define MSCH_AGING1_AGING1_SHIFT                           (0U)
#define MSCH_AGING1_AGING1_MASK                            (0xFFU << MSCH_AGING1_AGING1_SHIFT)                          /* 0x000000FF */
/* AGING2 */
#define MSCH_AGING2_AGING2_SHIFT                           (0U)
#define MSCH_AGING2_AGING2_MASK                            (0xFFU << MSCH_AGING2_AGING2_SHIFT)                          /* 0x000000FF */
/******************************************CRU*******************************************/
/* APLL_CON0 */
#define CRU_APLL_CON0_OFFSET                               (0x0)
#define CRU_APLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_APLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_APLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_APLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_APLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_APLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_APLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_APLL_CON0_BYPASS_MASK                          (0x1U << CRU_APLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* APLL_CON1 */
#define CRU_APLL_CON1_OFFSET                               (0x4)
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
#define CRU_APLL_CON2_OFFSET                               (0x8)
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
#define CRU_APLL_CON3_OFFSET                               (0xC)
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
#define CRU_APLL_CON4_OFFSET                               (0x10)
#define CRU_APLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_APLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_APLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_APLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_APLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_APLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* DPLL_CON0 */
#define CRU_DPLL_CON0_OFFSET                               (0x20)
#define CRU_DPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_DPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_DPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_DPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_DPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_DPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_DPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_DPLL_CON0_BYPASS_MASK                          (0x1U << CRU_DPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* DPLL_CON1 */
#define CRU_DPLL_CON1_OFFSET                               (0x24)
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
#define CRU_DPLL_CON2_OFFSET                               (0x28)
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
#define CRU_DPLL_CON3_OFFSET                               (0x2C)
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
#define CRU_DPLL_CON4_OFFSET                               (0x30)
#define CRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_DPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_DPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_DPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_DPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* CPLL_CON0 */
#define CRU_CPLL_CON0_OFFSET                               (0x40)
#define CRU_CPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_CPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_CPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_CPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_CPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_CPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_CPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_CPLL_CON0_BYPASS_MASK                          (0x1U << CRU_CPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* CPLL_CON1 */
#define CRU_CPLL_CON1_OFFSET                               (0x44)
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
#define CRU_CPLL_CON2_OFFSET                               (0x48)
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
#define CRU_CPLL_CON3_OFFSET                               (0x4C)
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
#define CRU_CPLL_CON4_OFFSET                               (0x50)
#define CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_CPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_CPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* GPLL_CON0 */
#define CRU_GPLL_CON0_OFFSET                               (0x60)
#define CRU_GPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_GPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_GPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_GPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_GPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_GPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_GPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_GPLL_CON0_BYPASS_MASK                          (0x1U << CRU_GPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* GPLL_CON1 */
#define CRU_GPLL_CON1_OFFSET                               (0x64)
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
#define CRU_GPLL_CON2_OFFSET                               (0x68)
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
#define CRU_GPLL_CON3_OFFSET                               (0x6C)
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
#define CRU_GPLL_CON4_OFFSET                               (0x70)
#define CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_GPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_GPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* NPLL_CON0 */
#define CRU_NPLL_CON0_OFFSET                               (0x80)
#define CRU_NPLL_CON0_FBDIV_SHIFT                          (0U)
#define CRU_NPLL_CON0_FBDIV_MASK                           (0xFFFU << CRU_NPLL_CON0_FBDIV_SHIFT)                        /* 0x00000FFF */
#define CRU_NPLL_CON0_POSTDIV1_SHIFT                       (12U)
#define CRU_NPLL_CON0_POSTDIV1_MASK                        (0x7U << CRU_NPLL_CON0_POSTDIV1_SHIFT)                       /* 0x00007000 */
#define CRU_NPLL_CON0_BYPASS_SHIFT                         (15U)
#define CRU_NPLL_CON0_BYPASS_MASK                          (0x1U << CRU_NPLL_CON0_BYPASS_SHIFT)                         /* 0x00008000 */
/* NPLL_CON1 */
#define CRU_NPLL_CON1_OFFSET                               (0x84)
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
#define CRU_NPLL_CON2_OFFSET                               (0x88)
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
#define CRU_NPLL_CON3_OFFSET                               (0x8C)
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
#define CRU_NPLL_CON4_OFFSET                               (0x90)
#define CRU_NPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT             (0U)
#define CRU_NPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK              (0x1U << CRU_NPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)             /* 0x00000001 */
#define CRU_NPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT              (8U)
#define CRU_NPLL_CON4_SSMOD_EXT_MAXADDR_MASK               (0xFFU << CRU_NPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)             /* 0x0000FF00 */
/* MODE */
#define CRU_MODE_OFFSET                                    (0xA0)
#define CRU_MODE_APLL_WORK_MODE_SHIFT                      (0U)
#define CRU_MODE_APLL_WORK_MODE_MASK                       (0x3U << CRU_MODE_APLL_WORK_MODE_SHIFT)                      /* 0x00000003 */
#define CRU_MODE_DPLL_WORK_MODE_SHIFT                      (2U)
#define CRU_MODE_DPLL_WORK_MODE_MASK                       (0x3U << CRU_MODE_DPLL_WORK_MODE_SHIFT)                      /* 0x0000000C */
#define CRU_MODE_CPLL_WORK_MODE_SHIFT                      (4U)
#define CRU_MODE_CPLL_WORK_MODE_MASK                       (0x3U << CRU_MODE_CPLL_WORK_MODE_SHIFT)                      /* 0x00000030 */
#define CRU_MODE_GPLL_WORK_MODE_SHIFT                      (6U)
#define CRU_MODE_GPLL_WORK_MODE_MASK                       (0x3U << CRU_MODE_GPLL_WORK_MODE_SHIFT)                      /* 0x000000C0 */
#define CRU_MODE_NPLL_WORK_MODE_SHIFT                      (8U)
#define CRU_MODE_NPLL_WORK_MODE_MASK                       (0x3U << CRU_MODE_NPLL_WORK_MODE_SHIFT)                      /* 0x00000300 */
#define CRU_MODE_USBPHY480M_WORK_MODE_SHIFT                (10U)
#define CRU_MODE_USBPHY480M_WORK_MODE_MASK                 (0x3U << CRU_MODE_USBPHY480M_WORK_MODE_SHIFT)                /* 0x00000C00 */
/* MISC */
#define CRU_MISC_OFFSET                                    (0xA4)
#define CRU_MISC_WARMRST_EN_SHIFT                          (0U)
#define CRU_MISC_WARMRST_EN_MASK                           (0x1U << CRU_MISC_WARMRST_EN_SHIFT)                          /* 0x00000001 */
#define CRU_MISC_CORE_SRST_WFIEN_SHIFT                     (1U)
#define CRU_MISC_CORE_SRST_WFIEN_MASK                      (0x1U << CRU_MISC_CORE_SRST_WFIEN_SHIFT)                     /* 0x00000002 */
#define CRU_MISC_CORE_WRST_WFIEN_SHIFT                     (2U)
#define CRU_MISC_CORE_WRST_WFIEN_MASK                      (0x1U << CRU_MISC_CORE_WRST_WFIEN_SHIFT)                     /* 0x00000004 */
#define CRU_MISC_COREPO_SRST_WFIEN_SHIFT                   (3U)
#define CRU_MISC_COREPO_SRST_WFIEN_MASK                    (0x1U << CRU_MISC_COREPO_SRST_WFIEN_SHIFT)                   /* 0x00000008 */
#define CRU_MISC_COREPO_WRST_WFIEN_SHIFT                   (4U)
#define CRU_MISC_COREPO_WRST_WFIEN_MASK                    (0x1U << CRU_MISC_COREPO_WRST_WFIEN_SHIFT)                   /* 0x00000010 */
#define CRU_MISC_CORE_HIGH_FREQ_RST_EN_SHIFT               (12U)
#define CRU_MISC_CORE_HIGH_FREQ_RST_EN_MASK                (0x3U << CRU_MISC_CORE_HIGH_FREQ_RST_EN_SHIFT)               /* 0x00003000 */
/* MISC1 */
#define CRU_MISC1_OFFSET                                   (0xA8)
#define CRU_MISC1_PD_CORE_DWN_CLK_EN_DIS_SHIFT             (0U)
#define CRU_MISC1_PD_CORE_DWN_CLK_EN_DIS_MASK              (0x1U << CRU_MISC1_PD_CORE_DWN_CLK_EN_DIS_SHIFT)             /* 0x00000001 */
#define CRU_MISC1_PD_PCIE_DWN_CLK_EN_DIS_SHIFT             (2U)
#define CRU_MISC1_PD_PCIE_DWN_CLK_EN_DIS_MASK              (0x1U << CRU_MISC1_PD_PCIE_DWN_CLK_EN_DIS_SHIFT)             /* 0x00000004 */
#define CRU_MISC1_PD_VIO_DWN_CLK_EN_DIS_SHIFT              (3U)
#define CRU_MISC1_PD_VIO_DWN_CLK_EN_DIS_MASK               (0x1U << CRU_MISC1_PD_VIO_DWN_CLK_EN_DIS_SHIFT)              /* 0x00000008 */
#define CRU_MISC1_PD_NPU_DWN_CLK_EN_DIS_SHIFT              (4U)
#define CRU_MISC1_PD_NPU_DWN_CLK_EN_DIS_MASK               (0x1U << CRU_MISC1_PD_NPU_DWN_CLK_EN_DIS_SHIFT)              /* 0x00000010 */
#define CRU_MISC1_PD_DDR_DWN_CLK_EN_DIS_SHIFT              (6U)
#define CRU_MISC1_PD_DDR_DWN_CLK_EN_DIS_MASK               (0x1U << CRU_MISC1_PD_DDR_DWN_CLK_EN_DIS_SHIFT)              /* 0x00000040 */
#define CRU_MISC1_PD_VPU_DWN_CLK_EN_DIS_SHIFT              (10U)
#define CRU_MISC1_PD_VPU_DWN_CLK_EN_DIS_MASK               (0x1U << CRU_MISC1_PD_VPU_DWN_CLK_EN_DIS_SHIFT)              /* 0x00000400 */
#define CRU_MISC1_CLK_I2S0_TX_OUT_MCLK_OEN_SHIFT           (12U)
#define CRU_MISC1_CLK_I2S0_TX_OUT_MCLK_OEN_MASK            (0x1U << CRU_MISC1_CLK_I2S0_TX_OUT_MCLK_OEN_SHIFT)           /* 0x00001000 */
#define CRU_MISC1_CLK_I2S0_RX_OUT_MCLK_OEN_SHIFT           (13U)
#define CRU_MISC1_CLK_I2S0_RX_OUT_MCLK_OEN_MASK            (0x1U << CRU_MISC1_CLK_I2S0_RX_OUT_MCLK_OEN_SHIFT)           /* 0x00002000 */
#define CRU_MISC1_CLK_I2S1_OUT_MCLK_OEN_SHIFT              (14U)
#define CRU_MISC1_CLK_I2S1_OUT_MCLK_OEN_MASK               (0x1U << CRU_MISC1_CLK_I2S1_OUT_MCLK_OEN_SHIFT)              /* 0x00004000 */
/* GLB_CNT_TH */
#define CRU_GLB_CNT_TH_OFFSET                              (0xB0)
#define CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT (0U)
#define CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_MASK (0xFFFFU << CRU_GLB_CNT_TH_GLOBAL_RESET_COUNTER_THRESHOLD_SHIFT) /* 0x0000FFFF */
#define CRU_GLB_CNT_TH_PLL_LOCKPERIOD_SHIFT                (16U)
#define CRU_GLB_CNT_TH_PLL_LOCKPERIOD_MASK                 (0xFFFFU << CRU_GLB_CNT_TH_PLL_LOCKPERIOD_SHIFT)             /* 0xFFFF0000 */
/* GLB_RST_ST */
#define CRU_GLB_RST_ST_OFFSET                              (0xB4)
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
#define CRU_GLB_RST_ST_RESETN_CORE0_SRC_ST_SHIFT           (16U)
#define CRU_GLB_RST_ST_RESETN_CORE0_SRC_ST_MASK            (0x1U << CRU_GLB_RST_ST_RESETN_CORE0_SRC_ST_SHIFT)           /* 0x00010000 */
#define CRU_GLB_RST_ST_RESETN_CORE1_SRC_ST_SHIFT           (17U)
#define CRU_GLB_RST_ST_RESETN_CORE1_SRC_ST_MASK            (0x1U << CRU_GLB_RST_ST_RESETN_CORE1_SRC_ST_SHIFT)           /* 0x00020000 */
#define CRU_GLB_RST_ST_RESETN_COREPO0_SRC_ST_SHIFT         (18U)
#define CRU_GLB_RST_ST_RESETN_COREPO0_SRC_ST_MASK          (0x1U << CRU_GLB_RST_ST_RESETN_COREPO0_SRC_ST_SHIFT)         /* 0x00040000 */
#define CRU_GLB_RST_ST_RESETN_COREPO1_SRC_ST_SHIFT         (19U)
#define CRU_GLB_RST_ST_RESETN_COREPO1_SRC_ST_MASK          (0x1U << CRU_GLB_RST_ST_RESETN_COREPO1_SRC_ST_SHIFT)         /* 0x00080000 */
/* GLB_SRST_FST */
#define CRU_GLB_SRST_FST_OFFSET                            (0xB8)
#define CRU_GLB_SRST_FST_GLB_SRST_FST_SHIFT                (0U)
#define CRU_GLB_SRST_FST_GLB_SRST_FST_MASK                 (0xFFFFU << CRU_GLB_SRST_FST_GLB_SRST_FST_SHIFT)             /* 0x0000FFFF */
/* GLB_SRST_SND */
#define CRU_GLB_SRST_SND_OFFSET                            (0xBC)
#define CRU_GLB_SRST_SND_GLB_SRST_SND_SHIFT                (0U)
#define CRU_GLB_SRST_SND_GLB_SRST_SND_MASK                 (0xFFFFU << CRU_GLB_SRST_SND_GLB_SRST_SND_SHIFT)             /* 0x0000FFFF */
/* GLB_RST_CON */
#define CRU_GLB_RST_CON_OFFSET                             (0xC0)
#define CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT          (0U)
#define CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_MASK           (0x1U << CRU_GLB_RST_CON_TSADC_GLB_SRST_CTRL_SHIFT)          /* 0x00000001 */
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT            (1U)
#define CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_MASK             (0x1U << CRU_GLB_RST_CON_WDT_GLB_SRST_CTRL_SHIFT)            /* 0x00000002 */
#define CRU_GLB_RST_CON_PMU_SRST_WDT_EN_SHIFT              (4U)
#define CRU_GLB_RST_CON_PMU_SRST_WDT_EN_MASK               (0x1U << CRU_GLB_RST_CON_PMU_SRST_WDT_EN_SHIFT)              /* 0x00000010 */
#define CRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_SHIFT      (6U)
#define CRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_MASK       (0x1U << CRU_GLB_RST_CON_TSADC_SHUT_RESET_EXT_EN_SHIFT)      /* 0x00000040 */
#define CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT             (7U)
#define CRU_GLB_RST_CON_WDT_RESET_EXT_EN_MASK              (0x1U << CRU_GLB_RST_CON_WDT_RESET_EXT_EN_SHIFT)             /* 0x00000080 */
/* HWFFC_CON0 */
#define CRU_HWFFC_CON0_OFFSET                              (0xE0)
#define CRU_HWFFC_CON0_CLK_CORE_DIV_CON_SHIFT              (0U)
#define CRU_HWFFC_CON0_CLK_CORE_DIV_CON_MASK               (0xFU << CRU_HWFFC_CON0_CLK_CORE_DIV_CON_SHIFT)              /* 0x0000000F */
#define CRU_HWFFC_CON0_BACKUPPLL_SEL_SHIFT                 (7U)
#define CRU_HWFFC_CON0_BACKUPPLL_SEL_MASK                  (0x1U << CRU_HWFFC_CON0_BACKUPPLL_SEL_SHIFT)                 /* 0x00000080 */
#define CRU_HWFFC_CON0_HWFFC_REQ_SHIFT                     (8U)
#define CRU_HWFFC_CON0_HWFFC_REQ_MASK                      (0x1U << CRU_HWFFC_CON0_HWFFC_REQ_SHIFT)                     /* 0x00000100 */
#define CRU_HWFFC_CON0_HWFFC_MODE_SHIFT                    (9U)
#define CRU_HWFFC_CON0_HWFFC_MODE_MASK                     (0x1U << CRU_HWFFC_CON0_HWFFC_MODE_SHIFT)                    /* 0x00000200 */
/* HWFFC_TH */
#define CRU_HWFFC_TH_OFFSET                                (0xE8)
#define CRU_HWFFC_TH_PLL_PD_EXTEND_TH_SHIFT                (0U)
#define CRU_HWFFC_TH_PLL_PD_EXTEND_TH_MASK                 (0xFFU << CRU_HWFFC_TH_PLL_PD_EXTEND_TH_SHIFT)               /* 0x000000FF */
#define CRU_HWFFC_TH_SWITCH_TH_SHIFT                       (8U)
#define CRU_HWFFC_TH_SWITCH_TH_MASK                        (0xFFU << CRU_HWFFC_TH_SWITCH_TH_SHIFT)                      /* 0x0000FF00 */
/* HWFFC_INTST */
#define CRU_HWFFC_INTST_OFFSET                             (0xEC)
#define CRU_HWFFC_INTST_HWFFC_DONE_SHIFT                   (0U)
#define CRU_HWFFC_INTST_HWFFC_DONE_MASK                    (0x1U << CRU_HWFFC_INTST_HWFFC_DONE_SHIFT)                   /* 0x00000001 */
/* APLL_CON0_S */
#define CRU_APLL_CON0_S_OFFSET                             (0xF0)
#define CRU_APLL_CON0_S_FBDIV_SHIFT                        (0U)
#define CRU_APLL_CON0_S_FBDIV_MASK                         (0xFFFU << CRU_APLL_CON0_S_FBDIV_SHIFT)                      /* 0x00000FFF */
#define CRU_APLL_CON0_S_POSTDIV1_SHIFT                     (12U)
#define CRU_APLL_CON0_S_POSTDIV1_MASK                      (0x7U << CRU_APLL_CON0_S_POSTDIV1_SHIFT)                     /* 0x00007000 */
#define CRU_APLL_CON0_S_BYPASS_SHIFT                       (15U)
#define CRU_APLL_CON0_S_BYPASS_MASK                        (0x1U << CRU_APLL_CON0_S_BYPASS_SHIFT)                       /* 0x00008000 */
/* APLL_CON1_S */
#define CRU_APLL_CON1_S_OFFSET                             (0xF4)
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
#define CRU_CLKSEL_CON0_S_OFFSET                           (0xF8)
#define CRU_CLKSEL_CON0_S_CLK_CORE_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON0_S_CLK_CORE_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON0_S_CLK_CORE_DIV_CON_SHIFT)           /* 0x0000000F */
#define CRU_CLKSEL_CON0_S_CORE_CLK_PLL_SEL_SHIFT           (6U)
#define CRU_CLKSEL_CON0_S_CORE_CLK_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON0_S_CORE_CLK_PLL_SEL_SHIFT)           /* 0x000000C0 */
#define CRU_CLKSEL_CON0_S_CORE_DBG_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL_CON0_S_CORE_DBG_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON0_S_CORE_DBG_DIV_CON_SHIFT)           /* 0x00000F00 */
#define CRU_CLKSEL_CON0_S_ACLK_CORE_DIV_CON_SHIFT          (12U)
#define CRU_CLKSEL_CON0_S_ACLK_CORE_DIV_CON_MASK           (0x7U << CRU_CLKSEL_CON0_S_ACLK_CORE_DIV_CON_SHIFT)          /* 0x00007000 */
/* CLKSEL_CON0 */
#define CRU_CLKSEL_CON0_OFFSET                             (0x100)
#define CRU_CLKSEL_CON0_CLK_CORE_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON0_CLK_CORE_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON0_CLK_CORE_DIV_CON_SHIFT)             /* 0x0000000F */
#define CRU_CLKSEL_CON0_CORE_CLK_PLL_SEL_SHIFT             (6U)
#define CRU_CLKSEL_CON0_CORE_CLK_PLL_SEL_MASK              (0x3U << CRU_CLKSEL_CON0_CORE_CLK_PLL_SEL_SHIFT)             /* 0x000000C0 */
#define CRU_CLKSEL_CON0_CORE_DBG_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON0_CORE_DBG_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON0_CORE_DBG_DIV_CON_SHIFT)             /* 0x00000F00 */
#define CRU_CLKSEL_CON0_ACLK_CORE_DIV_CON_SHIFT            (12U)
#define CRU_CLKSEL_CON0_ACLK_CORE_DIV_CON_MASK             (0x7U << CRU_CLKSEL_CON0_ACLK_CORE_DIV_CON_SHIFT)            /* 0x00007000 */
/* CLKSEL_CON1 */
#define CRU_CLKSEL_CON1_OFFSET                             (0x104)
#define CRU_CLKSEL_CON1_CLK_NPU_DIV_CON_SHIFT              (0U)
#define CRU_CLKSEL_CON1_CLK_NPU_DIV_CON_MASK               (0xFU << CRU_CLKSEL_CON1_CLK_NPU_DIV_CON_SHIFT)              /* 0x0000000F */
#define CRU_CLKSEL_CON1_CLK_NPU_NP5_DIV_CON_SHIFT          (4U)
#define CRU_CLKSEL_CON1_CLK_NPU_NP5_DIV_CON_MASK           (0xFU << CRU_CLKSEL_CON1_CLK_NPU_NP5_DIV_CON_SHIFT)          /* 0x000000F0 */
#define CRU_CLKSEL_CON1_NPU_PLL_CLK_SEL_SHIFT              (8U)
#define CRU_CLKSEL_CON1_NPU_PLL_CLK_SEL_MASK               (0x3U << CRU_CLKSEL_CON1_NPU_PLL_CLK_SEL_SHIFT)              /* 0x00000300 */
#define CRU_CLKSEL_CON1_NPU_NP5_PLL_CLK_SEL_SHIFT          (10U)
#define CRU_CLKSEL_CON1_NPU_NP5_PLL_CLK_SEL_MASK           (0x3U << CRU_CLKSEL_CON1_NPU_NP5_PLL_CLK_SEL_SHIFT)          /* 0x00000C00 */
#define CRU_CLKSEL_CON1_NPU_SRC_CLK_SEL_SHIFT              (15U)
#define CRU_CLKSEL_CON1_NPU_SRC_CLK_SEL_MASK               (0x1U << CRU_CLKSEL_CON1_NPU_SRC_CLK_SEL_SHIFT)              /* 0x00008000 */
/* CLKSEL_CON2 */
#define CRU_CLKSEL_CON2_OFFSET                             (0x108)
#define CRU_CLKSEL_CON2_ACLK_NPU_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON2_ACLK_NPU_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON2_ACLK_NPU_DIV_CON_SHIFT)             /* 0x0000000F */
#define CRU_CLKSEL_CON2_ACLK_NPU2MEM_DIV_CON_SHIFT         (4U)
#define CRU_CLKSEL_CON2_ACLK_NPU2MEM_DIV_CON_MASK          (0xFU << CRU_CLKSEL_CON2_ACLK_NPU2MEM_DIV_CON_SHIFT)         /* 0x000000F0 */
#define CRU_CLKSEL_CON2_HCLK_NPU_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON2_HCLK_NPU_DIV_CON_MASK              (0xFU << CRU_CLKSEL_CON2_HCLK_NPU_DIV_CON_SHIFT)             /* 0x00000F00 */
#define CRU_CLKSEL_CON2_NPU_ACLK_PLL_SEL_SHIFT             (14U)
#define CRU_CLKSEL_CON2_NPU_ACLK_PLL_SEL_MASK              (0x1U << CRU_CLKSEL_CON2_NPU_ACLK_PLL_SEL_SHIFT)             /* 0x00004000 */
#define CRU_CLKSEL_CON2_NPU_HCLK_PLL_SEL_SHIFT             (15U)
#define CRU_CLKSEL_CON2_NPU_HCLK_PLL_SEL_MASK              (0x1U << CRU_CLKSEL_CON2_NPU_HCLK_PLL_SEL_SHIFT)             /* 0x00008000 */
/* CLKSEL_CON3 */
#define CRU_CLKSEL_CON3_OFFSET                             (0x10C)
#define CRU_CLKSEL_CON3_DDRPHY1X_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON3_DDRPHY1X_DIV_CON_MASK              (0x1FU << CRU_CLKSEL_CON3_DDRPHY1X_DIV_CON_SHIFT)            /* 0x0000001F */
#define CRU_CLKSEL_CON3_DDRPHY1X_PLL_CLK_SEL_SHIFT         (7U)
#define CRU_CLKSEL_CON3_DDRPHY1X_PLL_CLK_SEL_MASK          (0x1U << CRU_CLKSEL_CON3_DDRPHY1X_PLL_CLK_SEL_SHIFT)         /* 0x00000080 */
#define CRU_CLKSEL_CON3_PCLK_DDR_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON3_PCLK_DDR_DIV_CON_MASK              (0x1FU << CRU_CLKSEL_CON3_PCLK_DDR_DIV_CON_SHIFT)            /* 0x00001F00 */
/* CLKSEL_CON4 */
#define CRU_CLKSEL_CON4_OFFSET                             (0x110)
#define CRU_CLKSEL_CON4_HSCLK_VIO_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON4_HSCLK_VIO_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON4_HSCLK_VIO_DIV_CON_SHIFT)           /* 0x0000001F */
#define CRU_CLKSEL_CON4_HSCLK_VIO_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON4_HSCLK_VIO_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON4_HSCLK_VIO_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON4_LSCLK_VIO_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON4_LSCLK_VIO_DIV_CON_MASK             (0xFU << CRU_CLKSEL_CON4_LSCLK_VIO_DIV_CON_SHIFT)            /* 0x00000F00 */
/* CLKSEL_CON5 */
#define CRU_CLKSEL_CON5_OFFSET                             (0x114)
#define CRU_CLKSEL_CON5_DCLK_VOPRAW_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON5_DCLK_VOPRAW_DIV_CON_MASK           (0xFFU << CRU_CLKSEL_CON5_DCLK_VOPRAW_DIV_CON_SHIFT)         /* 0x000000FF */
#define CRU_CLKSEL_CON5_DCLK_VOPRAW_PLL_SEL_SHIFT          (10U)
#define CRU_CLKSEL_CON5_DCLK_VOPRAW_PLL_SEL_MASK           (0x3U << CRU_CLKSEL_CON5_DCLK_VOPRAW_PLL_SEL_SHIFT)          /* 0x00000C00 */
#define CRU_CLKSEL_CON5_DCLK_VOPRAW_SEL_SHIFT              (14U)
#define CRU_CLKSEL_CON5_DCLK_VOPRAW_SEL_MASK               (0x3U << CRU_CLKSEL_CON5_DCLK_VOPRAW_SEL_SHIFT)              /* 0x0000C000 */
/* CLKSEL_CON6 */
#define CRU_CLKSEL_CON6_OFFSET                             (0x118)
#define CRU_CLKSEL_CON6_DCLK_VOPRAW_FRAC_DIV_CON_SHIFT     (0U)
#define CRU_CLKSEL_CON6_DCLK_VOPRAW_FRAC_DIV_CON_MASK      (0xFFFFFFFFU << CRU_CLKSEL_CON6_DCLK_VOPRAW_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON7 */
#define CRU_CLKSEL_CON7_OFFSET                             (0x11C)
#define CRU_CLKSEL_CON7_DCLK_VOPLITE_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON7_DCLK_VOPLITE_DIV_CON_MASK          (0xFFU << CRU_CLKSEL_CON7_DCLK_VOPLITE_DIV_CON_SHIFT)        /* 0x000000FF */
#define CRU_CLKSEL_CON7_DCLK_VOPLITE_PLL_SEL_SHIFT         (10U)
#define CRU_CLKSEL_CON7_DCLK_VOPLITE_PLL_SEL_MASK          (0x3U << CRU_CLKSEL_CON7_DCLK_VOPLITE_PLL_SEL_SHIFT)         /* 0x00000C00 */
#define CRU_CLKSEL_CON7_DCLK_VOPLITE_SEL_SHIFT             (14U)
#define CRU_CLKSEL_CON7_DCLK_VOPLITE_SEL_MASK              (0x3U << CRU_CLKSEL_CON7_DCLK_VOPLITE_SEL_SHIFT)             /* 0x0000C000 */
/* CLKSEL_CON8 */
#define CRU_CLKSEL_CON8_OFFSET                             (0x120)
#define CRU_CLKSEL_CON8_DCLK_VOPLITE_FRAC_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON8_DCLK_VOPLITE_FRAC_DIV_CON_MASK     (0xFFFFFFFFU << CRU_CLKSEL_CON8_DCLK_VOPLITE_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON9 */
#define CRU_CLKSEL_CON9_OFFSET                             (0x124)
#define CRU_CLKSEL_CON9_CLK_TXESC_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON9_CLK_TXESC_DIV_CON_MASK             (0xFFFU << CRU_CLKSEL_CON9_CLK_TXESC_DIV_CON_SHIFT)          /* 0x00000FFF */
/* CLKSEL_CON10 */
#define CRU_CLKSEL_CON10_OFFSET                            (0x128)
#define CRU_CLKSEL_CON10_CLK_RGA_CORE_DIV_CON_SHIFT        (0U)
#define CRU_CLKSEL_CON10_CLK_RGA_CORE_DIV_CON_MASK         (0x1FU << CRU_CLKSEL_CON10_CLK_RGA_CORE_DIV_CON_SHIFT)       /* 0x0000001F */
#define CRU_CLKSEL_CON10_CLK_RGA_CORE_PLL_SEL_SHIFT        (6U)
#define CRU_CLKSEL_CON10_CLK_RGA_CORE_PLL_SEL_MASK         (0x3U << CRU_CLKSEL_CON10_CLK_RGA_CORE_PLL_SEL_SHIFT)        /* 0x000000C0 */
#define CRU_CLKSEL_CON10_CLK_ISP_DIV_CON_SHIFT             (8U)
#define CRU_CLKSEL_CON10_CLK_ISP_DIV_CON_MASK              (0x1FU << CRU_CLKSEL_CON10_CLK_ISP_DIV_CON_SHIFT)            /* 0x00001F00 */
#define CRU_CLKSEL_CON10_CLK_ISP_PLL_SEL_SHIFT             (14U)
#define CRU_CLKSEL_CON10_CLK_ISP_PLL_SEL_MASK              (0x3U << CRU_CLKSEL_CON10_CLK_ISP_PLL_SEL_SHIFT)             /* 0x0000C000 */
/* CLKSEL_CON11 */
#define CRU_CLKSEL_CON11_OFFSET                            (0x12C)
#define CRU_CLKSEL_CON11_CLK_CIF_OUT_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON11_CLK_CIF_OUT_DIV_CON_MASK          (0x3FU << CRU_CLKSEL_CON11_CLK_CIF_OUT_DIV_CON_SHIFT)        /* 0x0000003F */
#define CRU_CLKSEL_CON11_CLK_CIF_OUT_PLL_SEL_SHIFT         (6U)
#define CRU_CLKSEL_CON11_CLK_CIF_OUT_PLL_SEL_MASK          (0x3U << CRU_CLKSEL_CON11_CLK_CIF_OUT_PLL_SEL_SHIFT)         /* 0x000000C0 */
#define CRU_CLKSEL_CON11_DCLK_CIF_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON11_DCLK_CIF_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON11_DCLK_CIF_DIV_CON_SHIFT)           /* 0x00001F00 */
#define CRU_CLKSEL_CON11_DCLK_CIF_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON11_DCLK_CIF_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON11_DCLK_CIF_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON12 */
#define CRU_CLKSEL_CON12_OFFSET                            (0x130)
#define CRU_CLKSEL_CON12_HSCLK_PCIE_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON12_HSCLK_PCIE_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON12_HSCLK_PCIE_DIV_CON_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL_CON12_LSCLK_PCIE_DIV_CON_SHIFT          (8U)
#define CRU_CLKSEL_CON12_LSCLK_PCIE_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON12_LSCLK_PCIE_DIV_CON_SHIFT)         /* 0x00001F00 */
#define CRU_CLKSEL_CON12_PDCLK_PCIE_PLL_SEL_SHIFT          (15U)
#define CRU_CLKSEL_CON12_PDCLK_PCIE_PLL_SEL_MASK           (0x1U << CRU_CLKSEL_CON12_PDCLK_PCIE_PLL_SEL_SHIFT)          /* 0x00008000 */
/* CLKSEL_CON13 */
#define CRU_CLKSEL_CON13_OFFSET                            (0x134)
#define CRU_CLKSEL_CON13_CLK_USB3_OTG_SUSPEND_DIV_CON_SHIFT (0U)
#define CRU_CLKSEL_CON13_CLK_USB3_OTG_SUSPEND_DIV_CON_MASK (0x3FFU << CRU_CLKSEL_CON13_CLK_USB3_OTG_SUSPEND_DIV_CON_SHIFT) /* 0x000003FF */
#define CRU_CLKSEL_CON13_USB3_OTG_SUSPEND_SRC_SEL_SHIFT    (12U)
#define CRU_CLKSEL_CON13_USB3_OTG_SUSPEND_SRC_SEL_MASK     (0x1U << CRU_CLKSEL_CON13_USB3_OTG_SUSPEND_SRC_SEL_SHIFT)    /* 0x00001000 */
/* CLKSEL_CON14 */
#define CRU_CLKSEL_CON14_OFFSET                            (0x138)
#define CRU_CLKSEL_CON14_CLK_PCIE_AUX_DIV_CON_SHIFT        (0U)
#define CRU_CLKSEL_CON14_CLK_PCIE_AUX_DIV_CON_MASK         (0x7FU << CRU_CLKSEL_CON14_CLK_PCIE_AUX_DIV_CON_SHIFT)       /* 0x0000007F */
#define CRU_CLKSEL_CON14_CLK_PCIE_AUX_PLL_SEL_SHIFT        (8U)
#define CRU_CLKSEL_CON14_CLK_PCIE_AUX_PLL_SEL_MASK         (0x3U << CRU_CLKSEL_CON14_CLK_PCIE_AUX_PLL_SEL_SHIFT)        /* 0x00000300 */
#define CRU_CLKSEL_CON14_CLK_PCIE_AUX_SRC_SEL_SHIFT        (12U)
#define CRU_CLKSEL_CON14_CLK_PCIE_AUX_SRC_SEL_MASK         (0x1U << CRU_CLKSEL_CON14_CLK_PCIE_AUX_SRC_SEL_SHIFT)        /* 0x00001000 */
/* CLKSEL_CON15 */
#define CRU_CLKSEL_CON15_OFFSET                            (0x13C)
#define CRU_CLKSEL_CON15_ACLK_PCIE_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON15_ACLK_PCIE_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON15_ACLK_PCIE_DIV_CON_SHIFT)           /* 0x0000000F */
#define CRU_CLKSEL_CON15_PCLK_PCIE_DIV_CON_SHIFT           (4U)
#define CRU_CLKSEL_CON15_PCLK_PCIE_DIV_CON_MASK            (0xFU << CRU_CLKSEL_CON15_PCLK_PCIE_DIV_CON_SHIFT)           /* 0x000000F0 */
#define CRU_CLKSEL_CON15_ACLK_PCIE_PLL_SEL_SHIFT           (8U)
#define CRU_CLKSEL_CON15_ACLK_PCIE_PLL_SEL_MASK            (0x1U << CRU_CLKSEL_CON15_ACLK_PCIE_PLL_SEL_SHIFT)           /* 0x00000100 */
#define CRU_CLKSEL_CON15_GIC_PLL_CLK_SEL_SHIFT             (11U)
#define CRU_CLKSEL_CON15_GIC_PLL_CLK_SEL_MASK              (0x1U << CRU_CLKSEL_CON15_GIC_PLL_CLK_SEL_SHIFT)             /* 0x00000800 */
#define CRU_CLKSEL_CON15_ACLK_GIC_DIV_CON_SHIFT            (12U)
#define CRU_CLKSEL_CON15_ACLK_GIC_DIV_CON_MASK             (0xFU << CRU_CLKSEL_CON15_ACLK_GIC_DIV_CON_SHIFT)            /* 0x0000F000 */
/* CLKSEL_CON16 */
#define CRU_CLKSEL_CON16_OFFSET                            (0x140)
#define CRU_CLKSEL_CON16_ACLK_VPU_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON16_ACLK_VPU_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON16_ACLK_VPU_DIV_CON_SHIFT)           /* 0x0000001F */
#define CRU_CLKSEL_CON16_ACLK_VPU_PLL_CLK_SEL_SHIFT        (7U)
#define CRU_CLKSEL_CON16_ACLK_VPU_PLL_CLK_SEL_MASK         (0x1U << CRU_CLKSEL_CON16_ACLK_VPU_PLL_CLK_SEL_SHIFT)        /* 0x00000080 */
#define CRU_CLKSEL_CON16_PCLK_VPU_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON16_PCLK_VPU_DIV_CON_MASK             (0xFU << CRU_CLKSEL_CON16_PCLK_VPU_DIV_CON_SHIFT)            /* 0x00000F00 */
/* CLKSEL_CON17 */
#define CRU_CLKSEL_CON17_OFFSET                            (0x144)
#define CRU_CLKSEL_CON17_ACLK_IMEM_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON17_ACLK_IMEM_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON17_ACLK_IMEM_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON17_ACLK_IMEM_PLL_CLK_SEL_SHIFT       (7U)
#define CRU_CLKSEL_CON17_ACLK_IMEM_PLL_CLK_SEL_MASK        (0x1U << CRU_CLKSEL_CON17_ACLK_IMEM_PLL_CLK_SEL_SHIFT)       /* 0x00000080 */
#define CRU_CLKSEL_CON17_HSCLK_IMEM_DIV_CON_SHIFT          (8U)
#define CRU_CLKSEL_CON17_HSCLK_IMEM_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON17_HSCLK_IMEM_DIV_CON_SHIFT)         /* 0x00001F00 */
#define CRU_CLKSEL_CON17_HSCLK_IMEM_PLL_CLK_SEL_SHIFT      (15U)
#define CRU_CLKSEL_CON17_HSCLK_IMEM_PLL_CLK_SEL_MASK       (0x1U << CRU_CLKSEL_CON17_HSCLK_IMEM_PLL_CLK_SEL_SHIFT)      /* 0x00008000 */
/* CLKSEL_CON18 */
#define CRU_CLKSEL_CON18_OFFSET                            (0x148)
#define CRU_CLKSEL_CON18_MSCLK_CORE_NIU_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON18_MSCLK_CORE_NIU_DIV_CON_MASK       (0x1FU << CRU_CLKSEL_CON18_MSCLK_CORE_NIU_DIV_CON_SHIFT)     /* 0x0000001F */
#define CRU_CLKSEL_CON18_CLK_GMAC_OUT_DIV_CON_SHIFT        (8U)
#define CRU_CLKSEL_CON18_CLK_GMAC_OUT_DIV_CON_MASK         (0x1FU << CRU_CLKSEL_CON18_CLK_GMAC_OUT_DIV_CON_SHIFT)       /* 0x00001F00 */
#define CRU_CLKSEL_CON18_CLK_GMAC_OUT_PLL_SEL_SHIFT        (14U)
#define CRU_CLKSEL_CON18_CLK_GMAC_OUT_PLL_SEL_MASK         (0x3U << CRU_CLKSEL_CON18_CLK_GMAC_OUT_PLL_SEL_SHIFT)        /* 0x0000C000 */
/* CLKSEL_CON19 */
#define CRU_CLKSEL_CON19_OFFSET                            (0x14C)
#define CRU_CLKSEL_CON19_MSCLK_PERI_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON19_MSCLK_PERI_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON19_MSCLK_PERI_DIV_CON_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL_CON19_LSCLK_PERI_DIV_CON_SHIFT          (8U)
#define CRU_CLKSEL_CON19_LSCLK_PERI_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON19_LSCLK_PERI_DIV_CON_SHIFT)         /* 0x00001F00 */
#define CRU_CLKSEL_CON19_PD_PERI_PLL_SEL_SHIFT             (15U)
#define CRU_CLKSEL_CON19_PD_PERI_PLL_SEL_MASK              (0x1U << CRU_CLKSEL_CON19_PD_PERI_PLL_SEL_SHIFT)             /* 0x00008000 */
/* CLKSEL_CON20 */
#define CRU_CLKSEL_CON20_OFFSET                            (0x150)
#define CRU_CLKSEL_CON20_CLK_SDMMC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON20_CLK_SDMMC_DIV_CON_MASK            (0xFFU << CRU_CLKSEL_CON20_CLK_SDMMC_DIV_CON_SHIFT)          /* 0x000000FF */
#define CRU_CLKSEL_CON20_CLK_SDMMC_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON20_CLK_SDMMC_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON20_CLK_SDMMC_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON21 */
#define CRU_CLKSEL_CON21_OFFSET                            (0x154)
#define CRU_CLKSEL_CON21_CLK_SDMMC_DIV50_DIV_CON_SHIFT     (0U)
#define CRU_CLKSEL_CON21_CLK_SDMMC_DIV50_DIV_CON_MASK      (0xFFU << CRU_CLKSEL_CON21_CLK_SDMMC_DIV50_DIV_CON_SHIFT)    /* 0x000000FF */
#define CRU_CLKSEL_CON21_CLK_SDMMC_SEL_SHIFT               (15U)
#define CRU_CLKSEL_CON21_CLK_SDMMC_SEL_MASK                (0x1U << CRU_CLKSEL_CON21_CLK_SDMMC_SEL_SHIFT)               /* 0x00008000 */
/* CLKSEL_CON22 */
#define CRU_CLKSEL_CON22_OFFSET                            (0x158)
#define CRU_CLKSEL_CON22_CLK_SDIO_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON22_CLK_SDIO_DIV_CON_MASK             (0xFFU << CRU_CLKSEL_CON22_CLK_SDIO_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL_CON22_CLK_SDIO_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON22_CLK_SDIO_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON22_CLK_SDIO_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON23 */
#define CRU_CLKSEL_CON23_OFFSET                            (0x15C)
#define CRU_CLKSEL_CON23_CLK_SDIO_DIV50_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON23_CLK_SDIO_DIV50_DIV_CON_MASK       (0xFFU << CRU_CLKSEL_CON23_CLK_SDIO_DIV50_DIV_CON_SHIFT)     /* 0x000000FF */
#define CRU_CLKSEL_CON23_CLK_SDIO_SEL_SHIFT                (15U)
#define CRU_CLKSEL_CON23_CLK_SDIO_SEL_MASK                 (0x1U << CRU_CLKSEL_CON23_CLK_SDIO_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL_CON24 */
#define CRU_CLKSEL_CON24_OFFSET                            (0x160)
#define CRU_CLKSEL_CON24_CLK_EMMC_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON24_CLK_EMMC_DIV_CON_MASK             (0xFFU << CRU_CLKSEL_CON24_CLK_EMMC_DIV_CON_SHIFT)           /* 0x000000FF */
#define CRU_CLKSEL_CON24_CLK_EMMC_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON24_CLK_EMMC_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON24_CLK_EMMC_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON25 */
#define CRU_CLKSEL_CON25_OFFSET                            (0x164)
#define CRU_CLKSEL_CON25_CLK_EMMC_DIV50_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON25_CLK_EMMC_DIV50_DIV_CON_MASK       (0xFFU << CRU_CLKSEL_CON25_CLK_EMMC_DIV50_DIV_CON_SHIFT)     /* 0x000000FF */
#define CRU_CLKSEL_CON25_CLK_EMMC_SEL_SHIFT                (15U)
#define CRU_CLKSEL_CON25_CLK_EMMC_SEL_MASK                 (0x1U << CRU_CLKSEL_CON25_CLK_EMMC_SEL_SHIFT)                /* 0x00008000 */
/* CLKSEL_CON26 */
#define CRU_CLKSEL_CON26_OFFSET                            (0x168)
#define CRU_CLKSEL_CON26_CLK_SFC_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON26_CLK_SFC_DIV_CON_MASK              (0x7FU << CRU_CLKSEL_CON26_CLK_SFC_DIV_CON_SHIFT)            /* 0x0000007F */
#define CRU_CLKSEL_CON26_CLK_SFC_PLL_SEL_SHIFT             (7U)
#define CRU_CLKSEL_CON26_CLK_SFC_PLL_SEL_MASK              (0x1U << CRU_CLKSEL_CON26_CLK_SFC_PLL_SEL_SHIFT)             /* 0x00000080 */
#define CRU_CLKSEL_CON26_CLK_GMAC_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON26_CLK_GMAC_DIV_CON_MASK             (0x1FU << CRU_CLKSEL_CON26_CLK_GMAC_DIV_CON_SHIFT)           /* 0x00001F00 */
#define CRU_CLKSEL_CON26_CLK_GMAC_PLL_SEL_SHIFT            (14U)
#define CRU_CLKSEL_CON26_CLK_GMAC_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON26_CLK_GMAC_PLL_SEL_SHIFT)            /* 0x0000C000 */
/* CLKSEL_CON27 */
#define CRU_CLKSEL_CON27_OFFSET                            (0x16C)
#define CRU_CLKSEL_CON27_RMII_EXTCLKSRC_SEL_SHIFT          (0U)
#define CRU_CLKSEL_CON27_RMII_EXTCLKSRC_SEL_MASK           (0x1U << CRU_CLKSEL_CON27_RMII_EXTCLKSRC_SEL_SHIFT)          /* 0x00000001 */
#define CRU_CLKSEL_CON27_RMII_CLK_SEL_SHIFT                (1U)
#define CRU_CLKSEL_CON27_RMII_CLK_SEL_MASK                 (0x1U << CRU_CLKSEL_CON27_RMII_CLK_SEL_SHIFT)                /* 0x00000002 */
#define CRU_CLKSEL_CON27_GMAC_CLK_SEL_SHIFT                (2U)
#define CRU_CLKSEL_CON27_GMAC_CLK_SEL_MASK                 (0x3U << CRU_CLKSEL_CON27_GMAC_CLK_SEL_SHIFT)                /* 0x0000000C */
#define CRU_CLKSEL_CON27_RMII_MODE_SHIFT                   (4U)
#define CRU_CLKSEL_CON27_RMII_MODE_MASK                    (0x1U << CRU_CLKSEL_CON27_RMII_MODE_SHIFT)                   /* 0x00000010 */
#define CRU_CLKSEL_CON27_HSCLK_BUS_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL_CON27_HSCLK_BUS_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON27_HSCLK_BUS_DIV_CON_SHIFT)          /* 0x00001F00 */
#define CRU_CLKSEL_CON27_PDBUS_CLK_PLL_SEL_SHIFT           (15U)
#define CRU_CLKSEL_CON27_PDBUS_CLK_PLL_SEL_MASK            (0x1U << CRU_CLKSEL_CON27_PDBUS_CLK_PLL_SEL_SHIFT)           /* 0x00008000 */
/* CLKSEL_CON28 */
#define CRU_CLKSEL_CON28_OFFSET                            (0x170)
#define CRU_CLKSEL_CON28_MSCLK_BUS_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON28_MSCLK_BUS_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON28_MSCLK_BUS_DIV_CON_SHIFT)          /* 0x0000001F */
#define CRU_CLKSEL_CON28_LSCLK_BUS_DIV_CON_SHIFT           (8U)
#define CRU_CLKSEL_CON28_LSCLK_BUS_DIV_CON_MASK            (0x1FU << CRU_CLKSEL_CON28_LSCLK_BUS_DIV_CON_SHIFT)          /* 0x00001F00 */
/* CLKSEL_CON29 */
#define CRU_CLKSEL_CON29_OFFSET                            (0x174)
#define CRU_CLKSEL_CON29_CLK_CRYPTO_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON29_CLK_CRYPTO_DIV_CON_MASK           (0x1FU << CRU_CLKSEL_CON29_CLK_CRYPTO_DIV_CON_SHIFT)         /* 0x0000001F */
#define CRU_CLKSEL_CON29_CLK_CRYPTO_PLL_SEL_SHIFT          (7U)
#define CRU_CLKSEL_CON29_CLK_CRYPTO_PLL_SEL_MASK           (0x1U << CRU_CLKSEL_CON29_CLK_CRYPTO_PLL_SEL_SHIFT)          /* 0x00000080 */
#define CRU_CLKSEL_CON29_CLK_CRYPTO_APK_DIV_CON_SHIFT      (8U)
#define CRU_CLKSEL_CON29_CLK_CRYPTO_APK_DIV_CON_MASK       (0x1FU << CRU_CLKSEL_CON29_CLK_CRYPTO_APK_DIV_CON_SHIFT)     /* 0x00001F00 */
#define CRU_CLKSEL_CON29_CLK_CRYPTO_APK_SEL_SHIFT          (15U)
#define CRU_CLKSEL_CON29_CLK_CRYPTO_APK_SEL_MASK           (0x1U << CRU_CLKSEL_CON29_CLK_CRYPTO_APK_SEL_SHIFT)          /* 0x00008000 */
/* CLKSEL_CON30 */
#define CRU_CLKSEL_CON30_OFFSET                            (0x178)
#define CRU_CLKSEL_CON30_CLK_PDM_DIV_CON_SHIFT             (0U)
#define CRU_CLKSEL_CON30_CLK_PDM_DIV_CON_MASK              (0x7FU << CRU_CLKSEL_CON30_CLK_PDM_DIV_CON_SHIFT)            /* 0x0000007F */
#define CRU_CLKSEL_CON30_CLK_PDM_PLL_SEL_SHIFT             (8U)
#define CRU_CLKSEL_CON30_CLK_PDM_PLL_SEL_MASK              (0x3U << CRU_CLKSEL_CON30_CLK_PDM_PLL_SEL_SHIFT)             /* 0x00000300 */
#define CRU_CLKSEL_CON30_CLK_PDM_SEL_SHIFT                 (15U)
#define CRU_CLKSEL_CON30_CLK_PDM_SEL_MASK                  (0x1U << CRU_CLKSEL_CON30_CLK_PDM_SEL_SHIFT)                 /* 0x00008000 */
/* CLKSEL_CON31 */
#define CRU_CLKSEL_CON31_OFFSET                            (0x17C)
#define CRU_CLKSEL_CON31_CLK_PDM_FRAC_DIV_CON_SHIFT        (0U)
#define CRU_CLKSEL_CON31_CLK_PDM_FRAC_DIV_CON_MASK         (0xFFFFFFFFU << CRU_CLKSEL_CON31_CLK_PDM_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON32 */
#define CRU_CLKSEL_CON32_OFFSET                            (0x180)
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_DIV_CON_MASK          (0x7FU << CRU_CLKSEL_CON32_CLK_I2S0_TX_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_PLL_SEL_SHIFT         (8U)
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_PLL_SEL_MASK          (0x3U << CRU_CLKSEL_CON32_CLK_I2S0_TX_PLL_SEL_SHIFT)         /* 0x00000300 */
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_SEL_SHIFT             (10U)
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_SEL_MASK              (0x3U << CRU_CLKSEL_CON32_CLK_I2S0_TX_SEL_SHIFT)             /* 0x00000C00 */
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_RX_CLK_SEL_SHIFT      (12U)
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_RX_CLK_SEL_MASK       (0x1U << CRU_CLKSEL_CON32_CLK_I2S0_TX_RX_CLK_SEL_SHIFT)      /* 0x00001000 */
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_OUT_MCLK_SEL_SHIFT    (14U)
#define CRU_CLKSEL_CON32_CLK_I2S0_TX_OUT_MCLK_SEL_MASK     (0x3U << CRU_CLKSEL_CON32_CLK_I2S0_TX_OUT_MCLK_SEL_SHIFT)    /* 0x0000C000 */
/* CLKSEL_CON33 */
#define CRU_CLKSEL_CON33_OFFSET                            (0x184)
#define CRU_CLKSEL_CON33_CLK_I2S0_TX_FRAC_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON33_CLK_I2S0_TX_FRAC_DIV_CON_MASK     (0xFFFFFFFFU << CRU_CLKSEL_CON33_CLK_I2S0_TX_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON34 */
#define CRU_CLKSEL_CON34_OFFSET                            (0x188)
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_DIV_CON_MASK          (0x7FU << CRU_CLKSEL_CON34_CLK_I2S0_RX_DIV_CON_SHIFT)        /* 0x0000007F */
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_PLL_SEL_SHIFT         (8U)
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_PLL_SEL_MASK          (0x3U << CRU_CLKSEL_CON34_CLK_I2S0_RX_PLL_SEL_SHIFT)         /* 0x00000300 */
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_SEL_SHIFT             (10U)
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_SEL_MASK              (0x3U << CRU_CLKSEL_CON34_CLK_I2S0_RX_SEL_SHIFT)             /* 0x00000C00 */
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_TX_CLK_SEL_SHIFT      (12U)
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_TX_CLK_SEL_MASK       (0x1U << CRU_CLKSEL_CON34_CLK_I2S0_RX_TX_CLK_SEL_SHIFT)      /* 0x00001000 */
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_OUT_MCLK_SEL_SHIFT    (14U)
#define CRU_CLKSEL_CON34_CLK_I2S0_RX_OUT_MCLK_SEL_MASK     (0x3U << CRU_CLKSEL_CON34_CLK_I2S0_RX_OUT_MCLK_SEL_SHIFT)    /* 0x0000C000 */
/* CLKSEL_CON35 */
#define CRU_CLKSEL_CON35_OFFSET                            (0x18C)
#define CRU_CLKSEL_CON35_CLK_I2S0_RX_FRAC_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON35_CLK_I2S0_RX_FRAC_DIV_CON_MASK     (0xFFFFFFFFU << CRU_CLKSEL_CON35_CLK_I2S0_RX_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON36 */
#define CRU_CLKSEL_CON36_OFFSET                            (0x190)
#define CRU_CLKSEL_CON36_CLK_I2S1_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON36_CLK_I2S1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON36_CLK_I2S1_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON36_CLK_I2S1_PLL_SEL_SHIFT            (8U)
#define CRU_CLKSEL_CON36_CLK_I2S1_PLL_SEL_MASK             (0x3U << CRU_CLKSEL_CON36_CLK_I2S1_PLL_SEL_SHIFT)            /* 0x00000300 */
#define CRU_CLKSEL_CON36_CLK_I2S1_SEL_SHIFT                (10U)
#define CRU_CLKSEL_CON36_CLK_I2S1_SEL_MASK                 (0x3U << CRU_CLKSEL_CON36_CLK_I2S1_SEL_SHIFT)                /* 0x00000C00 */
#define CRU_CLKSEL_CON36_CLK_I2S1_OUT_MCLK_SEL_SHIFT       (15U)
#define CRU_CLKSEL_CON36_CLK_I2S1_OUT_MCLK_SEL_MASK        (0x1U << CRU_CLKSEL_CON36_CLK_I2S1_OUT_MCLK_SEL_SHIFT)       /* 0x00008000 */
/* CLKSEL_CON37 */
#define CRU_CLKSEL_CON37_OFFSET                            (0x194)
#define CRU_CLKSEL_CON37_CLK_I2S1_FRAC_DIV_CON_SHIFT       (0U)
#define CRU_CLKSEL_CON37_CLK_I2S1_FRAC_DIV_CON_MASK        (0xFFFFFFFFU << CRU_CLKSEL_CON37_CLK_I2S1_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON38 */
#define CRU_CLKSEL_CON38_OFFSET                            (0x198)
#define CRU_CLKSEL_CON38_CLK_UART1_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON38_CLK_UART1_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON38_CLK_UART1_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON38_CLK_UART1_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON38_CLK_UART1_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON38_CLK_UART1_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON39 */
#define CRU_CLKSEL_CON39_OFFSET                            (0x19C)
#define CRU_CLKSEL_CON39_CLK_UART1_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON39_CLK_UART1_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON39_CLK_UART1_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON39_CLK_UART1_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON39_CLK_UART1_SEL_MASK                (0x3U << CRU_CLKSEL_CON39_CLK_UART1_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON40 */
#define CRU_CLKSEL_CON40_OFFSET                            (0x1A0)
#define CRU_CLKSEL_CON40_CLK_UART1_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON40_CLK_UART1_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON40_CLK_UART1_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON41 */
#define CRU_CLKSEL_CON41_OFFSET                            (0x1A4)
#define CRU_CLKSEL_CON41_CLK_UART2_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON41_CLK_UART2_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON41_CLK_UART2_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON41_CLK_UART2_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON41_CLK_UART2_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON41_CLK_UART2_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON42 */
#define CRU_CLKSEL_CON42_OFFSET                            (0x1A8)
#define CRU_CLKSEL_CON42_CLK_UART2_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON42_CLK_UART2_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON42_CLK_UART2_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON42_CLK_UART2_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON42_CLK_UART2_SEL_MASK                (0x3U << CRU_CLKSEL_CON42_CLK_UART2_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON43 */
#define CRU_CLKSEL_CON43_OFFSET                            (0x1AC)
#define CRU_CLKSEL_CON43_CLK_UART2_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON43_CLK_UART2_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON43_CLK_UART2_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON44 */
#define CRU_CLKSEL_CON44_OFFSET                            (0x1B0)
#define CRU_CLKSEL_CON44_CLK_UART3_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON44_CLK_UART3_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON44_CLK_UART3_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON44_CLK_UART3_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON44_CLK_UART3_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON44_CLK_UART3_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON45 */
#define CRU_CLKSEL_CON45_OFFSET                            (0x1B4)
#define CRU_CLKSEL_CON45_CLK_UART3_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON45_CLK_UART3_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON45_CLK_UART3_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON45_CLK_UART3_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON45_CLK_UART3_SEL_MASK                (0x3U << CRU_CLKSEL_CON45_CLK_UART3_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON46 */
#define CRU_CLKSEL_CON46_OFFSET                            (0x1B8)
#define CRU_CLKSEL_CON46_CLK_UART3_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON46_CLK_UART3_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON46_CLK_UART3_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON47 */
#define CRU_CLKSEL_CON47_OFFSET                            (0x1BC)
#define CRU_CLKSEL_CON47_CLK_UART4_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON47_CLK_UART4_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON47_CLK_UART4_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON47_CLK_UART4_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON47_CLK_UART4_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON47_CLK_UART4_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON48 */
#define CRU_CLKSEL_CON48_OFFSET                            (0x1C0)
#define CRU_CLKSEL_CON48_CLK_UART4_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON48_CLK_UART4_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON48_CLK_UART4_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON48_CLK_UART4_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON48_CLK_UART4_SEL_MASK                (0x3U << CRU_CLKSEL_CON48_CLK_UART4_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON49 */
#define CRU_CLKSEL_CON49_OFFSET                            (0x1C4)
#define CRU_CLKSEL_CON49_CLK_UART4_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON49_CLK_UART4_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON49_CLK_UART4_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON50 */
#define CRU_CLKSEL_CON50_OFFSET                            (0x1C8)
#define CRU_CLKSEL_CON50_CLK_UART5_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON50_CLK_UART5_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON50_CLK_UART5_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON50_CLK_UART5_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON50_CLK_UART5_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON50_CLK_UART5_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON51 */
#define CRU_CLKSEL_CON51_OFFSET                            (0x1CC)
#define CRU_CLKSEL_CON51_CLK_UART5_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON51_CLK_UART5_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON51_CLK_UART5_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON51_CLK_UART5_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON51_CLK_UART5_SEL_MASK                (0x3U << CRU_CLKSEL_CON51_CLK_UART5_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON52 */
#define CRU_CLKSEL_CON52_OFFSET                            (0x1D0)
#define CRU_CLKSEL_CON52_CLK_UART5_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON52_CLK_UART5_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON52_CLK_UART5_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON53 */
#define CRU_CLKSEL_CON53_OFFSET                            (0x1D4)
#define CRU_CLKSEL_CON53_CLK_UART6_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON53_CLK_UART6_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON53_CLK_UART6_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON53_CLK_UART6_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON53_CLK_UART6_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON53_CLK_UART6_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON54 */
#define CRU_CLKSEL_CON54_OFFSET                            (0x1D8)
#define CRU_CLKSEL_CON54_CLK_UART6_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON54_CLK_UART6_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON54_CLK_UART6_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON54_CLK_UART6_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON54_CLK_UART6_SEL_MASK                (0x3U << CRU_CLKSEL_CON54_CLK_UART6_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON55 */
#define CRU_CLKSEL_CON55_OFFSET                            (0x1DC)
#define CRU_CLKSEL_CON55_CLK_UART6_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON55_CLK_UART6_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON55_CLK_UART6_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON56 */
#define CRU_CLKSEL_CON56_OFFSET                            (0x1E0)
#define CRU_CLKSEL_CON56_CLK_UART7_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON56_CLK_UART7_DIV_CON_MASK            (0x7FU << CRU_CLKSEL_CON56_CLK_UART7_DIV_CON_SHIFT)          /* 0x0000007F */
#define CRU_CLKSEL_CON56_CLK_UART7_PLL_SEL_SHIFT           (14U)
#define CRU_CLKSEL_CON56_CLK_UART7_PLL_SEL_MASK            (0x3U << CRU_CLKSEL_CON56_CLK_UART7_PLL_SEL_SHIFT)           /* 0x0000C000 */
/* CLKSEL_CON57 */
#define CRU_CLKSEL_CON57_OFFSET                            (0x1E4)
#define CRU_CLKSEL_CON57_CLK_UART7_DIVNP5_DIV_CON_SHIFT    (0U)
#define CRU_CLKSEL_CON57_CLK_UART7_DIVNP5_DIV_CON_MASK     (0x7FU << CRU_CLKSEL_CON57_CLK_UART7_DIVNP5_DIV_CON_SHIFT)   /* 0x0000007F */
#define CRU_CLKSEL_CON57_CLK_UART7_SEL_SHIFT               (14U)
#define CRU_CLKSEL_CON57_CLK_UART7_SEL_MASK                (0x3U << CRU_CLKSEL_CON57_CLK_UART7_SEL_SHIFT)               /* 0x0000C000 */
/* CLKSEL_CON58 */
#define CRU_CLKSEL_CON58_OFFSET                            (0x1E8)
#define CRU_CLKSEL_CON58_CLK_UART7_FRAC_DIV_CON_SHIFT      (0U)
#define CRU_CLKSEL_CON58_CLK_UART7_FRAC_DIV_CON_MASK       (0xFFFFFFFFU << CRU_CLKSEL_CON58_CLK_UART7_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* CLKSEL_CON59 */
#define CRU_CLKSEL_CON59_OFFSET                            (0x1EC)
#define CRU_CLKSEL_CON59_CLK_I2C1_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON59_CLK_I2C1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON59_CLK_I2C1_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON59_CLK_I2C1_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON59_CLK_I2C1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON59_CLK_I2C1_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON59_CLK_I2C2_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON59_CLK_I2C2_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON59_CLK_I2C2_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON59_CLK_I2C2_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON59_CLK_I2C2_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON59_CLK_I2C2_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON60 */
#define CRU_CLKSEL_CON60_OFFSET                            (0x1F0)
#define CRU_CLKSEL_CON60_CLK_I2C3_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON60_CLK_I2C3_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON60_CLK_I2C3_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON60_CLK_I2C3_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON60_CLK_I2C3_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON60_CLK_I2C3_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON60_CLK_SPI0_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON60_CLK_SPI0_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON60_CLK_SPI0_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON60_CLK_SPI0_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON60_CLK_SPI0_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON60_CLK_SPI0_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON61 */
#define CRU_CLKSEL_CON61_OFFSET                            (0x1F4)
#define CRU_CLKSEL_CON61_CLK_SPI1_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON61_CLK_SPI1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON61_CLK_SPI1_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON61_CLK_SPI1_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON61_CLK_SPI1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON61_CLK_SPI1_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON61_CLK_SPI2_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON61_CLK_SPI2_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON61_CLK_SPI2_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON61_CLK_SPI2_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON61_CLK_SPI2_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON61_CLK_SPI2_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON62 */
#define CRU_CLKSEL_CON62_OFFSET                            (0x1F8)
#define CRU_CLKSEL_CON62_CLK_TSADC_DIV_CON_SHIFT           (0U)
#define CRU_CLKSEL_CON62_CLK_TSADC_DIV_CON_MASK            (0x7FFU << CRU_CLKSEL_CON62_CLK_TSADC_DIV_CON_SHIFT)         /* 0x000007FF */
/* CLKSEL_CON63 */
#define CRU_CLKSEL_CON63_OFFSET                            (0x1FC)
#define CRU_CLKSEL_CON63_CLK_SARADC_DIV_CON_SHIFT          (0U)
#define CRU_CLKSEL_CON63_CLK_SARADC_DIV_CON_MASK           (0x7FFU << CRU_CLKSEL_CON63_CLK_SARADC_DIV_CON_SHIFT)        /* 0x000007FF */
/* CLKSEL_CON64 */
#define CRU_CLKSEL_CON64_OFFSET                            (0x200)
#define CRU_CLKSEL_CON64_CLK_EFUSE_S_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON64_CLK_EFUSE_S_DIV_CON_MASK          (0x3FU << CRU_CLKSEL_CON64_CLK_EFUSE_S_DIV_CON_SHIFT)        /* 0x0000003F */
#define CRU_CLKSEL_CON64_CLK_EFUSE_S_PLL_CLK_SEL_SHIFT     (6U)
#define CRU_CLKSEL_CON64_CLK_EFUSE_S_PLL_CLK_SEL_MASK      (0x3U << CRU_CLKSEL_CON64_CLK_EFUSE_S_PLL_CLK_SEL_SHIFT)     /* 0x000000C0 */
#define CRU_CLKSEL_CON64_CLK_EFUSE_NS_DIV_CON_SHIFT        (8U)
#define CRU_CLKSEL_CON64_CLK_EFUSE_NS_DIV_CON_MASK         (0x3FU << CRU_CLKSEL_CON64_CLK_EFUSE_NS_DIV_CON_SHIFT)       /* 0x00003F00 */
#define CRU_CLKSEL_CON64_CLK_EFUSE_NS_PLL_CLK_SEL_SHIFT    (14U)
#define CRU_CLKSEL_CON64_CLK_EFUSE_NS_PLL_CLK_SEL_MASK     (0x3U << CRU_CLKSEL_CON64_CLK_EFUSE_NS_PLL_CLK_SEL_SHIFT)    /* 0x0000C000 */
/* CLKSEL_CON65 */
#define CRU_CLKSEL_CON65_OFFSET                            (0x204)
#define CRU_CLKSEL_CON65_DBCLK_GPIO1_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON65_DBCLK_GPIO1_DIV_CON_MASK          (0x7FFU << CRU_CLKSEL_CON65_DBCLK_GPIO1_DIV_CON_SHIFT)       /* 0x000007FF */
#define CRU_CLKSEL_CON65_DBCLK_GPIO1_PLL_SEL_SHIFT         (15U)
#define CRU_CLKSEL_CON65_DBCLK_GPIO1_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON65_DBCLK_GPIO1_PLL_SEL_SHIFT)         /* 0x00008000 */
/* CLKSEL_CON66 */
#define CRU_CLKSEL_CON66_OFFSET                            (0x208)
#define CRU_CLKSEL_CON66_DBCLK_GPIO2_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON66_DBCLK_GPIO2_DIV_CON_MASK          (0x7FFU << CRU_CLKSEL_CON66_DBCLK_GPIO2_DIV_CON_SHIFT)       /* 0x000007FF */
#define CRU_CLKSEL_CON66_DBCLK_GPIO2_PLL_SEL_SHIFT         (15U)
#define CRU_CLKSEL_CON66_DBCLK_GPIO2_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON66_DBCLK_GPIO2_PLL_SEL_SHIFT)         /* 0x00008000 */
/* CLKSEL_CON67 */
#define CRU_CLKSEL_CON67_OFFSET                            (0x20C)
#define CRU_CLKSEL_CON67_DBCLK_GPIO3_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON67_DBCLK_GPIO3_DIV_CON_MASK          (0x7FFU << CRU_CLKSEL_CON67_DBCLK_GPIO3_DIV_CON_SHIFT)       /* 0x000007FF */
#define CRU_CLKSEL_CON67_DBCLK_GPIO3_PLL_SEL_SHIFT         (15U)
#define CRU_CLKSEL_CON67_DBCLK_GPIO3_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON67_DBCLK_GPIO3_PLL_SEL_SHIFT)         /* 0x00008000 */
/* CLKSEL_CON68 */
#define CRU_CLKSEL_CON68_OFFSET                            (0x210)
#define CRU_CLKSEL_CON68_DBCLK_GPIO4_DIV_CON_SHIFT         (0U)
#define CRU_CLKSEL_CON68_DBCLK_GPIO4_DIV_CON_MASK          (0x7FFU << CRU_CLKSEL_CON68_DBCLK_GPIO4_DIV_CON_SHIFT)       /* 0x000007FF */
#define CRU_CLKSEL_CON68_DBCLK_GPIO4_PLL_SEL_SHIFT         (15U)
#define CRU_CLKSEL_CON68_DBCLK_GPIO4_PLL_SEL_MASK          (0x1U << CRU_CLKSEL_CON68_DBCLK_GPIO4_PLL_SEL_SHIFT)         /* 0x00008000 */
/* CLKSEL_CON69 */
#define CRU_CLKSEL_CON69_OFFSET                            (0x214)
#define CRU_CLKSEL_CON69_CLK_PWM0_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON69_CLK_PWM0_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON69_CLK_PWM0_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON69_CLK_PWM0_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON69_CLK_PWM0_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON69_CLK_PWM0_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON69_CLK_PWM1_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON69_CLK_PWM1_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON69_CLK_PWM1_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON69_CLK_PWM1_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON69_CLK_PWM1_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON69_CLK_PWM1_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON70 */
#define CRU_CLKSEL_CON70_OFFSET                            (0x218)
#define CRU_CLKSEL_CON70_CLK_PWM2_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON70_CLK_PWM2_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON70_CLK_PWM2_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON70_CLK_PWM2_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON70_CLK_PWM2_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON70_CLK_PWM2_PLL_SEL_SHIFT)            /* 0x00000080 */
/* CLKSEL_CON71 */
#define CRU_CLKSEL_CON71_OFFSET                            (0x21C)
#define CRU_CLKSEL_CON71_CLK_I2C4_DIV_CON_SHIFT            (0U)
#define CRU_CLKSEL_CON71_CLK_I2C4_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON71_CLK_I2C4_DIV_CON_SHIFT)           /* 0x0000007F */
#define CRU_CLKSEL_CON71_CLK_I2C4_PLL_SEL_SHIFT            (7U)
#define CRU_CLKSEL_CON71_CLK_I2C4_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON71_CLK_I2C4_PLL_SEL_SHIFT)            /* 0x00000080 */
#define CRU_CLKSEL_CON71_CLK_I2C5_DIV_CON_SHIFT            (8U)
#define CRU_CLKSEL_CON71_CLK_I2C5_DIV_CON_MASK             (0x7FU << CRU_CLKSEL_CON71_CLK_I2C5_DIV_CON_SHIFT)           /* 0x00007F00 */
#define CRU_CLKSEL_CON71_CLK_I2C5_PLL_SEL_SHIFT            (15U)
#define CRU_CLKSEL_CON71_CLK_I2C5_PLL_SEL_MASK             (0x1U << CRU_CLKSEL_CON71_CLK_I2C5_PLL_SEL_SHIFT)            /* 0x00008000 */
/* CLKSEL_CON72 */
#define CRU_CLKSEL_CON72_OFFSET                            (0x220)
#define CRU_CLKSEL_CON72_TESTCLK_SEL_SHIFT                 (0U)
#define CRU_CLKSEL_CON72_TESTCLK_SEL_MASK                  (0x1FU << CRU_CLKSEL_CON72_TESTCLK_SEL_SHIFT)                /* 0x0000001F */
#define CRU_CLKSEL_CON72_TEST_DIV_CON_SHIFT                (8U)
#define CRU_CLKSEL_CON72_TEST_DIV_CON_MASK                 (0x1FU << CRU_CLKSEL_CON72_TEST_DIV_CON_SHIFT)               /* 0x00001F00 */
/* CLKGATE_CON0 */
#define CRU_CLKGATE_CON0_OFFSET                            (0x230)
#define CRU_CLKGATE_CON0_CORE_PLL_CLK_EN_SHIFT             (0U)
#define CRU_CLKGATE_CON0_CORE_PLL_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON0_CORE_PLL_CLK_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE_CON0_MSCLK_CORE_NIU_CLK_EN_SHIFT       (1U)
#define CRU_CLKGATE_CON0_MSCLK_CORE_NIU_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON0_MSCLK_CORE_NIU_CLK_EN_SHIFT)       /* 0x00000002 */
#define CRU_CLKGATE_CON0_ACLK_CORE_EN_SHIFT                (2U)
#define CRU_CLKGATE_CON0_ACLK_CORE_EN_MASK                 (0x1U << CRU_CLKGATE_CON0_ACLK_CORE_EN_SHIFT)                /* 0x00000004 */
#define CRU_CLKGATE_CON0_PCLK_CORE_EN_SHIFT                (3U)
#define CRU_CLKGATE_CON0_PCLK_CORE_EN_MASK                 (0x1U << CRU_CLKGATE_CON0_PCLK_CORE_EN_SHIFT)                /* 0x00000008 */
#define CRU_CLKGATE_CON0_CLK_JTAG_EN_SHIFT                 (4U)
#define CRU_CLKGATE_CON0_CLK_JTAG_EN_MASK                  (0x1U << CRU_CLKGATE_CON0_CLK_JTAG_EN_SHIFT)                 /* 0x00000010 */
#define CRU_CLKGATE_CON0_CLK_PVTM_CORE_EN_SHIFT            (5U)
#define CRU_CLKGATE_CON0_CLK_PVTM_CORE_EN_MASK             (0x1U << CRU_CLKGATE_CON0_CLK_PVTM_CORE_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE_CON0_ACLK_CORE_NIU_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON0_ACLK_CORE_NIU_EN_MASK             (0x1U << CRU_CLKGATE_CON0_ACLK_CORE_NIU_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON0_PCLK_DBG_NIU_EN_SHIFT             (7U)
#define CRU_CLKGATE_CON0_PCLK_DBG_NIU_EN_MASK              (0x1U << CRU_CLKGATE_CON0_PCLK_DBG_NIU_EN_SHIFT)             /* 0x00000080 */
#define CRU_CLKGATE_CON0_PCLK_DBG_DAPLITE_EN_SHIFT         (8U)
#define CRU_CLKGATE_CON0_PCLK_DBG_DAPLITE_EN_MASK          (0x1U << CRU_CLKGATE_CON0_PCLK_DBG_DAPLITE_EN_SHIFT)         /* 0x00000100 */
#define CRU_CLKGATE_CON0_ACLK_ADB400_GIC2CORE_EN_SHIFT     (9U)
#define CRU_CLKGATE_CON0_ACLK_ADB400_GIC2CORE_EN_MASK      (0x1U << CRU_CLKGATE_CON0_ACLK_ADB400_GIC2CORE_EN_SHIFT)     /* 0x00000200 */
#define CRU_CLKGATE_CON0_ACLK_ADB400_CORE2GIC_EN_SHIFT     (10U)
#define CRU_CLKGATE_CON0_ACLK_ADB400_CORE2GIC_EN_MASK      (0x1U << CRU_CLKGATE_CON0_ACLK_ADB400_CORE2GIC_EN_SHIFT)     /* 0x00000400 */
#define CRU_CLKGATE_CON0_ACLK_CORE_PRF_EN_SHIFT            (11U)
#define CRU_CLKGATE_CON0_ACLK_CORE_PRF_EN_MASK             (0x1U << CRU_CLKGATE_CON0_ACLK_CORE_PRF_EN_SHIFT)            /* 0x00000800 */
#define CRU_CLKGATE_CON0_PCLK_CORE_GRF_EN_SHIFT            (12U)
#define CRU_CLKGATE_CON0_PCLK_CORE_GRF_EN_MASK             (0x1U << CRU_CLKGATE_CON0_PCLK_CORE_GRF_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE_CON0_CLK_PVTM_NPU_EN_SHIFT             (15U)
#define CRU_CLKGATE_CON0_CLK_PVTM_NPU_EN_MASK              (0x1U << CRU_CLKGATE_CON0_CLK_PVTM_NPU_EN_SHIFT)             /* 0x00008000 */
/* CLKGATE_CON1 */
#define CRU_CLKGATE_CON1_OFFSET                            (0x234)
#define CRU_CLKGATE_CON1_GIC_PLL_CLK_EN_SHIFT              (0U)
#define CRU_CLKGATE_CON1_GIC_PLL_CLK_EN_MASK               (0x1U << CRU_CLKGATE_CON1_GIC_PLL_CLK_EN_SHIFT)              /* 0x00000001 */
#define CRU_CLKGATE_CON1_ACLK_GIC_NIU_EN_SHIFT             (1U)
#define CRU_CLKGATE_CON1_ACLK_GIC_NIU_EN_MASK              (0x1U << CRU_CLKGATE_CON1_ACLK_GIC_NIU_EN_SHIFT)             /* 0x00000002 */
#define CRU_CLKGATE_CON1_ACLK_GIC500_EN_SHIFT              (2U)
#define CRU_CLKGATE_CON1_ACLK_GIC500_EN_MASK               (0x1U << CRU_CLKGATE_CON1_ACLK_GIC500_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE_CON1_ACLK_ADB400_CORE2GIC_EN_SHIFT     (3U)
#define CRU_CLKGATE_CON1_ACLK_ADB400_CORE2GIC_EN_MASK      (0x1U << CRU_CLKGATE_CON1_ACLK_ADB400_CORE2GIC_EN_SHIFT)     /* 0x00000008 */
#define CRU_CLKGATE_CON1_ACLK_ADB400_GIC2CORE_EN_SHIFT     (4U)
#define CRU_CLKGATE_CON1_ACLK_ADB400_GIC2CORE_EN_MASK      (0x1U << CRU_CLKGATE_CON1_ACLK_ADB400_GIC2CORE_EN_SHIFT)     /* 0x00000010 */
#define CRU_CLKGATE_CON1_ACLK_SPINLOCK_EN_SHIFT            (5U)
#define CRU_CLKGATE_CON1_ACLK_SPINLOCK_EN_MASK             (0x1U << CRU_CLKGATE_CON1_ACLK_SPINLOCK_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE_CON1_NPU_PLL_CLK_EN_SHIFT              (6U)
#define CRU_CLKGATE_CON1_NPU_PLL_CLK_EN_MASK               (0x1U << CRU_CLKGATE_CON1_NPU_PLL_CLK_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE_CON1_NPU_NP5_PLL_CLK_EN_SHIFT          (7U)
#define CRU_CLKGATE_CON1_NPU_NP5_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON1_NPU_NP5_PLL_CLK_EN_SHIFT)          /* 0x00000080 */
#define CRU_CLKGATE_CON1_NPU_PLL_ACLK_EN_SHIFT             (8U)
#define CRU_CLKGATE_CON1_NPU_PLL_ACLK_EN_MASK              (0x1U << CRU_CLKGATE_CON1_NPU_PLL_ACLK_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE_CON1_NPU_PLL_HCLK_EN_SHIFT             (9U)
#define CRU_CLKGATE_CON1_NPU_PLL_HCLK_EN_MASK              (0x1U << CRU_CLKGATE_CON1_NPU_PLL_HCLK_EN_SHIFT)             /* 0x00000200 */
#define CRU_CLKGATE_CON1_CLK_NPU_CORE_EN_SHIFT             (10U)
#define CRU_CLKGATE_CON1_CLK_NPU_CORE_EN_MASK              (0x1U << CRU_CLKGATE_CON1_CLK_NPU_CORE_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE_CON1_ACLK_NPU_EN_SHIFT                 (11U)
#define CRU_CLKGATE_CON1_ACLK_NPU_EN_MASK                  (0x1U << CRU_CLKGATE_CON1_ACLK_NPU_EN_SHIFT)                 /* 0x00000800 */
#define CRU_CLKGATE_CON1_HCLK_NPU_EN_SHIFT                 (12U)
#define CRU_CLKGATE_CON1_HCLK_NPU_EN_MASK                  (0x1U << CRU_CLKGATE_CON1_HCLK_NPU_EN_SHIFT)                 /* 0x00001000 */
#define CRU_CLKGATE_CON1_ACLK_NPU_NIU_EN_SHIFT             (13U)
#define CRU_CLKGATE_CON1_ACLK_NPU_NIU_EN_MASK              (0x1U << CRU_CLKGATE_CON1_ACLK_NPU_NIU_EN_SHIFT)             /* 0x00002000 */
#define CRU_CLKGATE_CON1_HCLK_NPU_NIU_EN_SHIFT             (14U)
#define CRU_CLKGATE_CON1_HCLK_NPU_NIU_EN_MASK              (0x1U << CRU_CLKGATE_CON1_HCLK_NPU_NIU_EN_SHIFT)             /* 0x00004000 */
#define CRU_CLKGATE_CON1_ACLK_NPU2MEM_EN_SHIFT             (15U)
#define CRU_CLKGATE_CON1_ACLK_NPU2MEM_EN_MASK              (0x1U << CRU_CLKGATE_CON1_ACLK_NPU2MEM_EN_SHIFT)             /* 0x00008000 */
/* CLKGATE_CON2 */
#define CRU_CLKGATE_CON2_OFFSET                            (0x238)
#define CRU_CLKGATE_CON2_CLK_DDRMON24_EN_SHIFT             (0U)
#define CRU_CLKGATE_CON2_CLK_DDRMON24_EN_MASK              (0x1U << CRU_CLKGATE_CON2_CLK_DDRMON24_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE_CON2_PCLK_DDR_PLL_CLK_EN_SHIFT         (1U)
#define CRU_CLKGATE_CON2_PCLK_DDR_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON2_PCLK_DDR_PLL_CLK_EN_SHIFT)         /* 0x00000002 */
#define CRU_CLKGATE_CON2_PCLK_DDRDFI_CTL_EN_SHIFT          (2U)
#define CRU_CLKGATE_CON2_PCLK_DDRDFI_CTL_EN_MASK           (0x1U << CRU_CLKGATE_CON2_PCLK_DDRDFI_CTL_EN_SHIFT)          /* 0x00000004 */
#define CRU_CLKGATE_CON2_CLK_DDRDFI_CTL_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON2_CLK_DDRDFI_CTL_EN_MASK            (0x1U << CRU_CLKGATE_CON2_CLK_DDRDFI_CTL_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON2_ACLK_DDR_UPCTL_EN_SHIFT           (5U)
#define CRU_CLKGATE_CON2_ACLK_DDR_UPCTL_EN_MASK            (0x1U << CRU_CLKGATE_CON2_ACLK_DDR_UPCTL_EN_SHIFT)           /* 0x00000020 */
#define CRU_CLKGATE_CON2_CLK_DDR_UPCTL_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON2_CLK_DDR_UPCTL_EN_MASK             (0x1U << CRU_CLKGATE_CON2_CLK_DDR_UPCTL_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON2_PCLK_DDR_UPCTL_EN_SHIFT           (7U)
#define CRU_CLKGATE_CON2_PCLK_DDR_UPCTL_EN_MASK            (0x1U << CRU_CLKGATE_CON2_PCLK_DDR_UPCTL_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE_CON2_CLK_DDR_MSCH_EN_SHIFT             (8U)
#define CRU_CLKGATE_CON2_CLK_DDR_MSCH_EN_MASK              (0x1U << CRU_CLKGATE_CON2_CLK_DDR_MSCH_EN_SHIFT)             /* 0x00000100 */
#define CRU_CLKGATE_CON2_PCLK_DDR_MSCH_EN_SHIFT            (9U)
#define CRU_CLKGATE_CON2_PCLK_DDR_MSCH_EN_MASK             (0x1U << CRU_CLKGATE_CON2_PCLK_DDR_MSCH_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE_CON2_PCLK_DDR_MON_EN_SHIFT             (10U)
#define CRU_CLKGATE_CON2_PCLK_DDR_MON_EN_MASK              (0x1U << CRU_CLKGATE_CON2_PCLK_DDR_MON_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE_CON2_CLK_DDRC_MON_EN_SHIFT             (11U)
#define CRU_CLKGATE_CON2_CLK_DDRC_MON_EN_MASK              (0x1U << CRU_CLKGATE_CON2_CLK_DDRC_MON_EN_SHIFT)             /* 0x00000800 */
#define CRU_CLKGATE_CON2_PCLK_DDRSTDBY_EN_SHIFT            (12U)
#define CRU_CLKGATE_CON2_PCLK_DDRSTDBY_EN_MASK             (0x1U << CRU_CLKGATE_CON2_PCLK_DDRSTDBY_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE_CON2_CLK_DDRSTDBY_EN_SHIFT             (13U)
#define CRU_CLKGATE_CON2_CLK_DDRSTDBY_EN_MASK              (0x1U << CRU_CLKGATE_CON2_CLK_DDRSTDBY_EN_SHIFT)             /* 0x00002000 */
#define CRU_CLKGATE_CON2_PCLK_DDRGRF_EN_SHIFT              (14U)
#define CRU_CLKGATE_CON2_PCLK_DDRGRF_EN_MASK               (0x1U << CRU_CLKGATE_CON2_PCLK_DDRGRF_EN_SHIFT)              /* 0x00004000 */
#define CRU_CLKGATE_CON2_ACLK_AXI_SPLIT_EN_SHIFT           (15U)
#define CRU_CLKGATE_CON2_ACLK_AXI_SPLIT_EN_MASK            (0x1U << CRU_CLKGATE_CON2_ACLK_AXI_SPLIT_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE_CON3 */
#define CRU_CLKGATE_CON3_OFFSET                            (0x23C)
#define CRU_CLKGATE_CON3_HSCLK_VIO_PLL_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON3_HSCLK_VIO_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON3_HSCLK_VIO_PLL_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON3_DCLKVOPRAW_PLL_CLK_EN_SHIFT       (1U)
#define CRU_CLKGATE_CON3_DCLKVOPRAW_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON3_DCLKVOPRAW_PLL_CLK_EN_SHIFT)       /* 0x00000002 */
#define CRU_CLKGATE_CON3_DCLKVOPRAW_FRACDIV_CLK_EN_SHIFT   (2U)
#define CRU_CLKGATE_CON3_DCLKVOPRAW_FRACDIV_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON3_DCLKVOPRAW_FRACDIV_CLK_EN_SHIFT)   /* 0x00000004 */
#define CRU_CLKGATE_CON3_DCLKVOPRAW_CLK_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON3_DCLKVOPRAW_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON3_DCLKVOPRAW_CLK_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON3_DCLKVOPLITE_PLL_CLK_EN_SHIFT      (4U)
#define CRU_CLKGATE_CON3_DCLKVOPLITE_PLL_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON3_DCLKVOPLITE_PLL_CLK_EN_SHIFT)      /* 0x00000010 */
#define CRU_CLKGATE_CON3_DCLKVOPLITE_FRACDIV_CLK_EN_SHIFT  (5U)
#define CRU_CLKGATE_CON3_DCLKVOPLITE_FRACDIV_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON3_DCLKVOPLITE_FRACDIV_CLK_EN_SHIFT)  /* 0x00000020 */
#define CRU_CLKGATE_CON3_DCLKVOPLITE_CLK_EN_SHIFT          (6U)
#define CRU_CLKGATE_CON3_DCLKVOPLITE_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON3_DCLKVOPLITE_CLK_EN_SHIFT)          /* 0x00000040 */
#define CRU_CLKGATE_CON3_CLK_TXESC_PLL_EN_SHIFT            (7U)
#define CRU_CLKGATE_CON3_CLK_TXESC_PLL_EN_MASK             (0x1U << CRU_CLKGATE_CON3_CLK_TXESC_PLL_EN_SHIFT)            /* 0x00000080 */
#define CRU_CLKGATE_CON3_CLK_RGA_PLL_CLK_EN_SHIFT          (8U)
#define CRU_CLKGATE_CON3_CLK_RGA_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON3_CLK_RGA_PLL_CLK_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE_CON3_CLK_CIFOUT_PLL_CLK_EN_SHIFT       (9U)
#define CRU_CLKGATE_CON3_CLK_CIFOUT_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON3_CLK_CIFOUT_PLL_CLK_EN_SHIFT)       /* 0x00000200 */
#define CRU_CLKGATE_CON3_CLK_ISP_PLL_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON3_CLK_ISP_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON3_CLK_ISP_PLL_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON3_DCLK_CIF_PLL_CLK_EN_SHIFT         (11U)
#define CRU_CLKGATE_CON3_DCLK_CIF_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON3_DCLK_CIF_PLL_CLK_EN_SHIFT)         /* 0x00000800 */
#define CRU_CLKGATE_CON3_LSCLK_VIO_SRC_EN_SHIFT            (12U)
#define CRU_CLKGATE_CON3_LSCLK_VIO_SRC_EN_MASK             (0x1U << CRU_CLKGATE_CON3_LSCLK_VIO_SRC_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE_CON3_PCLKIN_CIF_EN_SHIFT               (13U)
#define CRU_CLKGATE_CON3_PCLKIN_CIF_EN_MASK                (0x1U << CRU_CLKGATE_CON3_PCLKIN_CIF_EN_SHIFT)               /* 0x00002000 */
/* CLKGATE_CON4 */
#define CRU_CLKGATE_CON4_OFFSET                            (0x240)
#define CRU_CLKGATE_CON4_HSCLK_VIO_NIU_EN_SHIFT            (0U)
#define CRU_CLKGATE_CON4_HSCLK_VIO_NIU_EN_MASK             (0x1U << CRU_CLKGATE_CON4_HSCLK_VIO_NIU_EN_SHIFT)            /* 0x00000001 */
#define CRU_CLKGATE_CON4_LSCLK_VIO_NIU_EN_SHIFT            (1U)
#define CRU_CLKGATE_CON4_LSCLK_VIO_NIU_EN_MASK             (0x1U << CRU_CLKGATE_CON4_LSCLK_VIO_NIU_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE_CON4_ACLK_VOPRAW_EN_SHIFT              (2U)
#define CRU_CLKGATE_CON4_ACLK_VOPRAW_EN_MASK               (0x1U << CRU_CLKGATE_CON4_ACLK_VOPRAW_EN_SHIFT)              /* 0x00000004 */
#define CRU_CLKGATE_CON4_HCLK_VOPRAW_EN_SHIFT              (3U)
#define CRU_CLKGATE_CON4_HCLK_VOPRAW_EN_MASK               (0x1U << CRU_CLKGATE_CON4_HCLK_VOPRAW_EN_SHIFT)              /* 0x00000008 */
#define CRU_CLKGATE_CON4_ACLK_VOPLITE_EN_SHIFT             (4U)
#define CRU_CLKGATE_CON4_ACLK_VOPLITE_EN_MASK              (0x1U << CRU_CLKGATE_CON4_ACLK_VOPLITE_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE_CON4_HCLK_VOPLITE_EN_SHIFT             (5U)
#define CRU_CLKGATE_CON4_HCLK_VOPLITE_EN_MASK              (0x1U << CRU_CLKGATE_CON4_HCLK_VOPLITE_EN_SHIFT)             /* 0x00000020 */
#define CRU_CLKGATE_CON4_PCLK_MIPI_DSI_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON4_PCLK_MIPI_DSI_EN_MASK             (0x1U << CRU_CLKGATE_CON4_PCLK_MIPI_DSI_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON4_PCLK_CSITX_EN_SHIFT               (7U)
#define CRU_CLKGATE_CON4_PCLK_CSITX_EN_MASK                (0x1U << CRU_CLKGATE_CON4_PCLK_CSITX_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE_CON4_ACLK_RGA_EN_SHIFT                 (8U)
#define CRU_CLKGATE_CON4_ACLK_RGA_EN_MASK                  (0x1U << CRU_CLKGATE_CON4_ACLK_RGA_EN_SHIFT)                 /* 0x00000100 */
#define CRU_CLKGATE_CON4_HCLK_RGA_EN_SHIFT                 (9U)
#define CRU_CLKGATE_CON4_HCLK_RGA_EN_MASK                  (0x1U << CRU_CLKGATE_CON4_HCLK_RGA_EN_SHIFT)                 /* 0x00000200 */
#define CRU_CLKGATE_CON4_ACLK_CIF_EN_SHIFT                 (10U)
#define CRU_CLKGATE_CON4_ACLK_CIF_EN_MASK                  (0x1U << CRU_CLKGATE_CON4_ACLK_CIF_EN_SHIFT)                 /* 0x00000400 */
#define CRU_CLKGATE_CON4_HCLK_CIF_EN_SHIFT                 (11U)
#define CRU_CLKGATE_CON4_HCLK_CIF_EN_MASK                  (0x1U << CRU_CLKGATE_CON4_HCLK_CIF_EN_SHIFT)                 /* 0x00000800 */
#define CRU_CLKGATE_CON4_PCLK_CSI2X_EN_SHIFT               (12U)
#define CRU_CLKGATE_CON4_PCLK_CSI2X_EN_MASK                (0x1U << CRU_CLKGATE_CON4_PCLK_CSI2X_EN_SHIFT)               /* 0x00001000 */
#define CRU_CLKGATE_CON4_ACLK_ISP_EN_SHIFT                 (13U)
#define CRU_CLKGATE_CON4_ACLK_ISP_EN_MASK                  (0x1U << CRU_CLKGATE_CON4_ACLK_ISP_EN_SHIFT)                 /* 0x00002000 */
#define CRU_CLKGATE_CON4_HCLK_ISP_EN_SHIFT                 (14U)
#define CRU_CLKGATE_CON4_HCLK_ISP_EN_MASK                  (0x1U << CRU_CLKGATE_CON4_HCLK_ISP_EN_SHIFT)                 /* 0x00004000 */
/* CLKGATE_CON5 */
#define CRU_CLKGATE_CON5_OFFSET                            (0x244)
#define CRU_CLKGATE_CON5_PDCLK_PCIE_PLL_CLK_EN_SHIFT       (0U)
#define CRU_CLKGATE_CON5_PDCLK_PCIE_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON5_PDCLK_PCIE_PLL_CLK_EN_SHIFT)       /* 0x00000001 */
#define CRU_CLKGATE_CON5_CLK_USB3OTG_REF_CLK_EN_SHIFT      (1U)
#define CRU_CLKGATE_CON5_CLK_USB3OTG_REF_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON5_CLK_USB3OTG_REF_CLK_EN_SHIFT)      /* 0x00000002 */
#define CRU_CLKGATE_CON5_CLK_USB3OTG_SUSPEND_CLK_EN_SHIFT  (2U)
#define CRU_CLKGATE_CON5_CLK_USB3OTG_SUSPEND_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON5_CLK_USB3OTG_SUSPEND_CLK_EN_SHIFT)  /* 0x00000004 */
#define CRU_CLKGATE_CON5_CLK_PCIE_AUX_PLL_CLK_EN_SHIFT     (3U)
#define CRU_CLKGATE_CON5_CLK_PCIE_AUX_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON5_CLK_PCIE_AUX_PLL_CLK_EN_SHIFT)     /* 0x00000008 */
#define CRU_CLKGATE_CON5_CLK_PCIE_AUX_SRC_CLK_EN_SHIFT     (4U)
#define CRU_CLKGATE_CON5_CLK_PCIE_AUX_SRC_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON5_CLK_PCIE_AUX_SRC_CLK_EN_SHIFT)     /* 0x00000010 */
#define CRU_CLKGATE_CON5_ACLK_PCIE_PLL_CLK_EN_SHIFT        (5U)
#define CRU_CLKGATE_CON5_ACLK_PCIE_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON5_ACLK_PCIE_PLL_CLK_EN_SHIFT)        /* 0x00000020 */
#define CRU_CLKGATE_CON5_PCLK_PCIE_SRC_CLK_EN_SHIFT        (6U)
#define CRU_CLKGATE_CON5_PCLK_PCIE_SRC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON5_PCLK_PCIE_SRC_CLK_EN_SHIFT)        /* 0x00000040 */
/* CLKGATE_CON6 */
#define CRU_CLKGATE_CON6_OFFSET                            (0x248)
#define CRU_CLKGATE_CON6_HSCLK_PCIE_NIU_EN_SHIFT           (0U)
#define CRU_CLKGATE_CON6_HSCLK_PCIE_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON6_HSCLK_PCIE_NIU_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE_CON6_LSCLK_PCIE_NIU_EN_SHIFT           (1U)
#define CRU_CLKGATE_CON6_LSCLK_PCIE_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON6_LSCLK_PCIE_NIU_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE_CON6_ACLK_PCIE_MST_EN_SHIFT            (2U)
#define CRU_CLKGATE_CON6_ACLK_PCIE_MST_EN_MASK             (0x1U << CRU_CLKGATE_CON6_ACLK_PCIE_MST_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE_CON6_ACLK_PCIE_SLV_EN_SHIFT            (3U)
#define CRU_CLKGATE_CON6_ACLK_PCIE_SLV_EN_MASK             (0x1U << CRU_CLKGATE_CON6_ACLK_PCIE_SLV_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE_CON6_ACLK_PCIE_DBI_EN_SHIFT            (4U)
#define CRU_CLKGATE_CON6_ACLK_PCIE_DBI_EN_MASK             (0x1U << CRU_CLKGATE_CON6_ACLK_PCIE_DBI_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE_CON6_PCLK_PCIE_GRF_EN_SHIFT            (5U)
#define CRU_CLKGATE_CON6_PCLK_PCIE_GRF_EN_MASK             (0x1U << CRU_CLKGATE_CON6_PCLK_PCIE_GRF_EN_SHIFT)            /* 0x00000020 */
#define CRU_CLKGATE_CON6_ACLK_USB3OTG_EN_SHIFT             (6U)
#define CRU_CLKGATE_CON6_ACLK_USB3OTG_EN_MASK              (0x1U << CRU_CLKGATE_CON6_ACLK_USB3OTG_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE_CON6_HCLK_USB2HOST_EN_SHIFT            (7U)
#define CRU_CLKGATE_CON6_HCLK_USB2HOST_EN_MASK             (0x1U << CRU_CLKGATE_CON6_HCLK_USB2HOST_EN_SHIFT)            /* 0x00000080 */
#define CRU_CLKGATE_CON6_HCLK_USB2HOST_ARB_EN_SHIFT        (8U)
#define CRU_CLKGATE_CON6_HCLK_USB2HOST_ARB_EN_MASK         (0x1U << CRU_CLKGATE_CON6_HCLK_USB2HOST_ARB_EN_SHIFT)        /* 0x00000100 */
#define CRU_CLKGATE_CON6_PCLK_PCIE_EN_SHIFT                (9U)
#define CRU_CLKGATE_CON6_PCLK_PCIE_EN_MASK                 (0x1U << CRU_CLKGATE_CON6_PCLK_PCIE_EN_SHIFT)                /* 0x00000200 */
#define CRU_CLKGATE_CON6_ACLK_PCIE_NIU_EN_SHIFT            (10U)
#define CRU_CLKGATE_CON6_ACLK_PCIE_NIU_EN_MASK             (0x1U << CRU_CLKGATE_CON6_ACLK_PCIE_NIU_EN_SHIFT)            /* 0x00000400 */
#define CRU_CLKGATE_CON6_PCLK_PCIE_NIU_EN_SHIFT            (11U)
#define CRU_CLKGATE_CON6_PCLK_PCIE_NIU_EN_MASK             (0x1U << CRU_CLKGATE_CON6_PCLK_PCIE_NIU_EN_SHIFT)            /* 0x00000800 */
/* CLKGATE_CON7 */
#define CRU_CLKGATE_CON7_OFFSET                            (0x24C)
#define CRU_CLKGATE_CON7_IMEM_PLL_CLK_EN_SHIFT             (0U)
#define CRU_CLKGATE_CON7_IMEM_PLL_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON7_IMEM_PLL_CLK_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE_CON7_IMEM0_SRC_CLK_EN_SHIFT            (1U)
#define CRU_CLKGATE_CON7_IMEM0_SRC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON7_IMEM0_SRC_CLK_EN_SHIFT)            /* 0x00000002 */
#define CRU_CLKGATE_CON7_IMEM1_SRC_CLK_EN_SHIFT            (2U)
#define CRU_CLKGATE_CON7_IMEM1_SRC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON7_IMEM1_SRC_CLK_EN_SHIFT)            /* 0x00000004 */
#define CRU_CLKGATE_CON7_IMEM2_SRC_CLK_EN_SHIFT            (3U)
#define CRU_CLKGATE_CON7_IMEM2_SRC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON7_IMEM2_SRC_CLK_EN_SHIFT)            /* 0x00000008 */
#define CRU_CLKGATE_CON7_IMEM3_SRC_CLK_EN_SHIFT            (4U)
#define CRU_CLKGATE_CON7_IMEM3_SRC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON7_IMEM3_SRC_CLK_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE_CON7_IMEM_HSCLK_PLL_CLK_EN_SHIFT       (5U)
#define CRU_CLKGATE_CON7_IMEM_HSCLK_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON7_IMEM_HSCLK_PLL_CLK_EN_SHIFT)       /* 0x00000020 */
#define CRU_CLKGATE_CON7_ACLK_IMEM0_EN_SHIFT               (6U)
#define CRU_CLKGATE_CON7_ACLK_IMEM0_EN_MASK                (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM0_EN_SHIFT)               /* 0x00000040 */
#define CRU_CLKGATE_CON7_ACLK_IMEM1_EN_SHIFT               (7U)
#define CRU_CLKGATE_CON7_ACLK_IMEM1_EN_MASK                (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM1_EN_SHIFT)               /* 0x00000080 */
#define CRU_CLKGATE_CON7_ACLK_IMEM2_EN_SHIFT               (8U)
#define CRU_CLKGATE_CON7_ACLK_IMEM2_EN_MASK                (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM2_EN_SHIFT)               /* 0x00000100 */
#define CRU_CLKGATE_CON7_ACLK_IMEM3_EN_SHIFT               (9U)
#define CRU_CLKGATE_CON7_ACLK_IMEM3_EN_MASK                (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM3_EN_SHIFT)               /* 0x00000200 */
#define CRU_CLKGATE_CON7_ACLK_IMEM0_NIU_EN_SHIFT           (10U)
#define CRU_CLKGATE_CON7_ACLK_IMEM0_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM0_NIU_EN_SHIFT)           /* 0x00000400 */
#define CRU_CLKGATE_CON7_ACLK_IMEM1_NIU_EN_SHIFT           (11U)
#define CRU_CLKGATE_CON7_ACLK_IMEM1_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM1_NIU_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE_CON7_ACLK_IMEM2_NIU_EN_SHIFT           (12U)
#define CRU_CLKGATE_CON7_ACLK_IMEM2_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM2_NIU_EN_SHIFT)           /* 0x00001000 */
#define CRU_CLKGATE_CON7_ACLK_IMEM3_NIU_EN_SHIFT           (13U)
#define CRU_CLKGATE_CON7_ACLK_IMEM3_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON7_ACLK_IMEM3_NIU_EN_SHIFT)           /* 0x00002000 */
/* CLKGATE_CON8 */
#define CRU_CLKGATE_CON8_OFFSET                            (0x250)
#define CRU_CLKGATE_CON8_PD_PERI_PLL_CLK_EN_SHIFT          (0U)
#define CRU_CLKGATE_CON8_PD_PERI_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON8_PD_PERI_PLL_CLK_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE_CON8_MSCLK_PERI_CLK_EN_SHIFT           (1U)
#define CRU_CLKGATE_CON8_MSCLK_PERI_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON8_MSCLK_PERI_CLK_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE_CON8_LSCLK_PERI_CLK_EN_SHIFT           (2U)
#define CRU_CLKGATE_CON8_LSCLK_PERI_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON8_LSCLK_PERI_CLK_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE_CON8_MSCLK_PERI_NIU_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON8_MSCLK_PERI_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON8_MSCLK_PERI_NIU_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON8_LSCLK_PERI_NIU_EN_SHIFT           (4U)
#define CRU_CLKGATE_CON8_LSCLK_PERI_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON8_LSCLK_PERI_NIU_EN_SHIFT)           /* 0x00000010 */
#define CRU_CLKGATE_CON8_DDRPHY_DPLL_CLK_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON8_DDRPHY_DPLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON8_DDRPHY_DPLL_CLK_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON8_DDRPHY_GPLL_CLK_EN_SHIFT          (6U)
#define CRU_CLKGATE_CON8_DDRPHY_GPLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON8_DDRPHY_GPLL_CLK_EN_SHIFT)          /* 0x00000040 */
#define CRU_CLKGATE_CON8_DDRPHY1X_CLK_EN_SHIFT             (7U)
#define CRU_CLKGATE_CON8_DDRPHY1X_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON8_DDRPHY1X_CLK_EN_SHIFT)             /* 0x00000080 */
#define CRU_CLKGATE_CON8_ACLK_VPU_PLL_CLK_EN_SHIFT         (8U)
#define CRU_CLKGATE_CON8_ACLK_VPU_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON8_ACLK_VPU_PLL_CLK_EN_SHIFT)         /* 0x00000100 */
#define CRU_CLKGATE_CON8_HCLK_VPU_SRC_EN_SHIFT             (9U)
#define CRU_CLKGATE_CON8_HCLK_VPU_SRC_EN_MASK              (0x1U << CRU_CLKGATE_CON8_HCLK_VPU_SRC_EN_SHIFT)             /* 0x00000200 */
#define CRU_CLKGATE_CON8_ACLK_NPU_NIU_EN_SHIFT             (10U)
#define CRU_CLKGATE_CON8_ACLK_NPU_NIU_EN_MASK              (0x1U << CRU_CLKGATE_CON8_ACLK_NPU_NIU_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE_CON8_HCLK_VPU_NIU_EN_SHIFT             (11U)
#define CRU_CLKGATE_CON8_HCLK_VPU_NIU_EN_MASK              (0x1U << CRU_CLKGATE_CON8_HCLK_VPU_NIU_EN_SHIFT)             /* 0x00000800 */
#define CRU_CLKGATE_CON8_ACLK_VPU_EN_SHIFT                 (12U)
#define CRU_CLKGATE_CON8_ACLK_VPU_EN_MASK                  (0x1U << CRU_CLKGATE_CON8_ACLK_VPU_EN_SHIFT)                 /* 0x00001000 */
#define CRU_CLKGATE_CON8_HCLK_VPU_EN_SHIFT                 (13U)
#define CRU_CLKGATE_CON8_HCLK_VPU_EN_MASK                  (0x1U << CRU_CLKGATE_CON8_HCLK_VPU_EN_SHIFT)                 /* 0x00002000 */
/* CLKGATE_CON9 */
#define CRU_CLKGATE_CON9_OFFSET                            (0x254)
#define CRU_CLKGATE_CON9_HCLK_MMC_SFC_CLK_EN_SHIFT         (0U)
#define CRU_CLKGATE_CON9_HCLK_MMC_SFC_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON9_HCLK_MMC_SFC_CLK_EN_SHIFT)         /* 0x00000001 */
#define CRU_CLKGATE_CON9_CLK_SDIO_PLL_CLK_EN_SHIFT         (1U)
#define CRU_CLKGATE_CON9_CLK_SDIO_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON9_CLK_SDIO_PLL_CLK_EN_SHIFT)         /* 0x00000002 */
#define CRU_CLKGATE_CON9_CLK_SDIO_DIV50_CLK_EN_SHIFT       (2U)
#define CRU_CLKGATE_CON9_CLK_SDIO_DIV50_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON9_CLK_SDIO_DIV50_CLK_EN_SHIFT)       /* 0x00000004 */
#define CRU_CLKGATE_CON9_CLK_SDIO_CLK_EN_SHIFT             (3U)
#define CRU_CLKGATE_CON9_CLK_SDIO_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON9_CLK_SDIO_CLK_EN_SHIFT)             /* 0x00000008 */
#define CRU_CLKGATE_CON9_CLK_EMMC_PLL_CLK_EN_SHIFT         (4U)
#define CRU_CLKGATE_CON9_CLK_EMMC_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON9_CLK_EMMC_PLL_CLK_EN_SHIFT)         /* 0x00000010 */
#define CRU_CLKGATE_CON9_CLK_EMMC_DIV50_CLK_EN_SHIFT       (5U)
#define CRU_CLKGATE_CON9_CLK_EMMC_DIV50_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON9_CLK_EMMC_DIV50_CLK_EN_SHIFT)       /* 0x00000020 */
#define CRU_CLKGATE_CON9_CLK_EMMC_CLK_EN_SHIFT             (6U)
#define CRU_CLKGATE_CON9_CLK_EMMC_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON9_CLK_EMMC_CLK_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE_CON9_CLK_SDMMC_PLL_CLK_EN_SHIFT        (7U)
#define CRU_CLKGATE_CON9_CLK_SDMMC_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON9_CLK_SDMMC_PLL_CLK_EN_SHIFT)        /* 0x00000080 */
#define CRU_CLKGATE_CON9_CLK_SDMMC_DIV50_CLK_EN_SHIFT      (8U)
#define CRU_CLKGATE_CON9_CLK_SDMMC_DIV50_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON9_CLK_SDMMC_DIV50_CLK_EN_SHIFT)      /* 0x00000100 */
#define CRU_CLKGATE_CON9_CLK_SDMMC_CLK_EN_SHIFT            (9U)
#define CRU_CLKGATE_CON9_CLK_SDMMC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON9_CLK_SDMMC_CLK_EN_SHIFT)            /* 0x00000200 */
#define CRU_CLKGATE_CON9_CLK_SFC_PLL_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON9_CLK_SFC_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON9_CLK_SFC_PLL_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON9_HCLK_MMC_SFC_NIU_EN_SHIFT         (11U)
#define CRU_CLKGATE_CON9_HCLK_MMC_SFC_NIU_EN_MASK          (0x1U << CRU_CLKGATE_CON9_HCLK_MMC_SFC_NIU_EN_SHIFT)         /* 0x00000800 */
#define CRU_CLKGATE_CON9_HCLK_EMMC_CLK_EN_SHIFT            (12U)
#define CRU_CLKGATE_CON9_HCLK_EMMC_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON9_HCLK_EMMC_CLK_EN_SHIFT)            /* 0x00001000 */
#define CRU_CLKGATE_CON9_HCLK_SFC_CLK_EN_SHIFT             (13U)
#define CRU_CLKGATE_CON9_HCLK_SFC_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON9_HCLK_SFC_CLK_EN_SHIFT)             /* 0x00002000 */
/* CLKGATE_CON10 */
#define CRU_CLKGATE_CON10_OFFSET                           (0x258)
#define CRU_CLKGATE_CON10_ACLK_SD_GMAC_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON10_ACLK_SD_GMAC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON10_ACLK_SD_GMAC_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON10_HCLK_SD_GMAC_CLK_EN_SHIFT        (1U)
#define CRU_CLKGATE_CON10_HCLK_SD_GMAC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON10_HCLK_SD_GMAC_CLK_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE_CON10_PCLK_SD_GMAC_CLK_EN_SHIFT        (2U)
#define CRU_CLKGATE_CON10_PCLK_SD_GMAC_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON10_PCLK_SD_GMAC_CLK_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE_CON10_CLK_GMAC_PLL_CLK_EN_SHIFT        (3U)
#define CRU_CLKGATE_CON10_CLK_GMAC_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON10_CLK_GMAC_PLL_CLK_EN_SHIFT)        /* 0x00000008 */
#define CRU_CLKGATE_CON10_CLK_MAC_REF_EN_SHIFT             (4U)
#define CRU_CLKGATE_CON10_CLK_MAC_REF_EN_MASK              (0x1U << CRU_CLKGATE_CON10_CLK_MAC_REF_EN_SHIFT)             /* 0x00000010 */
#define CRU_CLKGATE_CON10_CLK_MAC_REFOUT_EN_SHIFT          (5U)
#define CRU_CLKGATE_CON10_CLK_MAC_REFOUT_EN_MASK           (0x1U << CRU_CLKGATE_CON10_CLK_MAC_REFOUT_EN_SHIFT)          /* 0x00000020 */
#define CRU_CLKGATE_CON10_CLK_MAC_RX_EN_SHIFT              (6U)
#define CRU_CLKGATE_CON10_CLK_MAC_RX_EN_MASK               (0x1U << CRU_CLKGATE_CON10_CLK_MAC_RX_EN_SHIFT)              /* 0x00000040 */
#define CRU_CLKGATE_CON10_CLK_MAC_TX_EN_SHIFT              (7U)
#define CRU_CLKGATE_CON10_CLK_MAC_TX_EN_MASK               (0x1U << CRU_CLKGATE_CON10_CLK_MAC_TX_EN_SHIFT)              /* 0x00000080 */
#define CRU_CLKGATE_CON10_ACLK_GMAC_NIU_EN_SHIFT           (8U)
#define CRU_CLKGATE_CON10_ACLK_GMAC_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON10_ACLK_GMAC_NIU_EN_SHIFT)           /* 0x00000100 */
#define CRU_CLKGATE_CON10_HCLK_GMAC_NIU_EN_SHIFT           (9U)
#define CRU_CLKGATE_CON10_HCLK_GMAC_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON10_HCLK_GMAC_NIU_EN_SHIFT)           /* 0x00000200 */
#define CRU_CLKGATE_CON10_PCLK_GMAC_NIU_EN_SHIFT           (10U)
#define CRU_CLKGATE_CON10_PCLK_GMAC_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON10_PCLK_GMAC_NIU_EN_SHIFT)           /* 0x00000400 */
#define CRU_CLKGATE_CON10_ACLK_GMAC_EN_SHIFT               (11U)
#define CRU_CLKGATE_CON10_ACLK_GMAC_EN_MASK                (0x1U << CRU_CLKGATE_CON10_ACLK_GMAC_EN_SHIFT)               /* 0x00000800 */
#define CRU_CLKGATE_CON10_PCLK_GMAC_EN_SHIFT               (12U)
#define CRU_CLKGATE_CON10_PCLK_GMAC_EN_MASK                (0x1U << CRU_CLKGATE_CON10_PCLK_GMAC_EN_SHIFT)               /* 0x00001000 */
#define CRU_CLKGATE_CON10_HCLK_SDIO_CLK_EN_SHIFT           (13U)
#define CRU_CLKGATE_CON10_HCLK_SDIO_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON10_HCLK_SDIO_CLK_EN_SHIFT)           /* 0x00002000 */
#define CRU_CLKGATE_CON10_HCLK_SDMMC_CLK_EN_SHIFT          (14U)
#define CRU_CLKGATE_CON10_HCLK_SDMMC_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON10_HCLK_SDMMC_CLK_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE_CON10_CLK_GMAC_OUT_PLL_CLK_EN_SHIFT    (15U)
#define CRU_CLKGATE_CON10_CLK_GMAC_OUT_PLL_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON10_CLK_GMAC_OUT_PLL_CLK_EN_SHIFT)    /* 0x00008000 */
/* CLKGATE_CON11 */
#define CRU_CLKGATE_CON11_OFFSET                           (0x25C)
#define CRU_CLKGATE_CON11_PD_BUS_PLL_CLK_EN_SHIFT          (0U)
#define CRU_CLKGATE_CON11_PD_BUS_PLL_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON11_PD_BUS_PLL_CLK_EN_SHIFT)          /* 0x00000001 */
#define CRU_CLKGATE_CON11_HSCLK_BUS_CLK_EN_SHIFT           (1U)
#define CRU_CLKGATE_CON11_HSCLK_BUS_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_HSCLK_BUS_CLK_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE_CON11_MSCLK_BUS_CLK_EN_SHIFT           (2U)
#define CRU_CLKGATE_CON11_MSCLK_BUS_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_MSCLK_BUS_CLK_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE_CON11_LSCLK_BUS_CLK_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON11_LSCLK_BUS_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_LSCLK_BUS_CLK_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON11_PCLK_TOP_CLK_EN_SHIFT            (4U)
#define CRU_CLKGATE_CON11_PCLK_TOP_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON11_PCLK_TOP_CLK_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE_CON11_CLK_CRYPTO_PLL_CLK_EN_SHIFT      (5U)
#define CRU_CLKGATE_CON11_CLK_CRYPTO_PLL_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON11_CLK_CRYPTO_PLL_CLK_EN_SHIFT)      /* 0x00000020 */
#define CRU_CLKGATE_CON11_CLK_CRYPTO_APK_PLL_CLK_EN_SHIFT  (6U)
#define CRU_CLKGATE_CON11_CLK_CRYPTO_APK_PLL_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_CRYPTO_APK_PLL_CLK_EN_SHIFT)  /* 0x00000040 */
#define CRU_CLKGATE_CON11_CLK_UART1_PLL_CLK_EN_SHIFT       (8U)
#define CRU_CLKGATE_CON11_CLK_UART1_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON11_CLK_UART1_PLL_CLK_EN_SHIFT)       /* 0x00000100 */
#define CRU_CLKGATE_CON11_CLK_UART1_DIVNP5_CLK_EN_SHIFT    (9U)
#define CRU_CLKGATE_CON11_CLK_UART1_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON11_CLK_UART1_DIVNP5_CLK_EN_SHIFT)    /* 0x00000200 */
#define CRU_CLKGATE_CON11_CLK_UART1_FRAC_SRC_CLK_EN_SHIFT  (10U)
#define CRU_CLKGATE_CON11_CLK_UART1_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_UART1_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000400 */
#define CRU_CLKGATE_CON11_CLK_UART1_CLK_EN_SHIFT           (11U)
#define CRU_CLKGATE_CON11_CLK_UART1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON11_CLK_UART1_CLK_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE_CON11_CLK_UART2_PLL_CLK_EN_SHIFT       (12U)
#define CRU_CLKGATE_CON11_CLK_UART2_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON11_CLK_UART2_PLL_CLK_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE_CON11_CLK_UART2_DIVNP5_CLK_EN_SHIFT    (13U)
#define CRU_CLKGATE_CON11_CLK_UART2_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON11_CLK_UART2_DIVNP5_CLK_EN_SHIFT)    /* 0x00002000 */
#define CRU_CLKGATE_CON11_CLK_UART2_FRAC_SRC_CLK_EN_SHIFT  (14U)
#define CRU_CLKGATE_CON11_CLK_UART2_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON11_CLK_UART2_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00004000 */
#define CRU_CLKGATE_CON11_CLK_UAR2_CLK_EN_SHIFT            (15U)
#define CRU_CLKGATE_CON11_CLK_UAR2_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON11_CLK_UAR2_CLK_EN_SHIFT)            /* 0x00008000 */
/* CLKGATE_CON12 */
#define CRU_CLKGATE_CON12_OFFSET                           (0x260)
#define CRU_CLKGATE_CON12_CLK_UART3_PLL_CLK_EN_SHIFT       (0U)
#define CRU_CLKGATE_CON12_CLK_UART3_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON12_CLK_UART3_PLL_CLK_EN_SHIFT)       /* 0x00000001 */
#define CRU_CLKGATE_CON12_CLK_UART3_DIVNP5_CLK_EN_SHIFT    (1U)
#define CRU_CLKGATE_CON12_CLK_UART3_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON12_CLK_UART3_DIVNP5_CLK_EN_SHIFT)    /* 0x00000002 */
#define CRU_CLKGATE_CON12_CLK_UART3_FRAC_SRC_CLK_EN_SHIFT  (2U)
#define CRU_CLKGATE_CON12_CLK_UART3_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON12_CLK_UART3_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000004 */
#define CRU_CLKGATE_CON12_CLK_UART3_CLK_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON12_CLK_UART3_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON12_CLK_UART3_CLK_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON12_CLK_UART4_PLL_CLK_EN_SHIFT       (4U)
#define CRU_CLKGATE_CON12_CLK_UART4_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON12_CLK_UART4_PLL_CLK_EN_SHIFT)       /* 0x00000010 */
#define CRU_CLKGATE_CON12_CLK_UART4_DIVNP5_CLK_EN_SHIFT    (5U)
#define CRU_CLKGATE_CON12_CLK_UART4_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON12_CLK_UART4_DIVNP5_CLK_EN_SHIFT)    /* 0x00000020 */
#define CRU_CLKGATE_CON12_CLK_UART4_FRAC_SRC_CLK_EN_SHIFT  (6U)
#define CRU_CLKGATE_CON12_CLK_UART4_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON12_CLK_UART4_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000040 */
#define CRU_CLKGATE_CON12_CLK_UART4_CLK_EN_SHIFT           (7U)
#define CRU_CLKGATE_CON12_CLK_UART4_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON12_CLK_UART4_CLK_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE_CON12_CLK_UART5_PLL_CLK_EN_SHIFT       (8U)
#define CRU_CLKGATE_CON12_CLK_UART5_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON12_CLK_UART5_PLL_CLK_EN_SHIFT)       /* 0x00000100 */
#define CRU_CLKGATE_CON12_CLK_UART5_DIVNP5_CLK_EN_SHIFT    (9U)
#define CRU_CLKGATE_CON12_CLK_UART5_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON12_CLK_UART5_DIVNP5_CLK_EN_SHIFT)    /* 0x00000200 */
#define CRU_CLKGATE_CON12_CLK_UART5_FRAC_SRC_CLK_EN_SHIFT  (10U)
#define CRU_CLKGATE_CON12_CLK_UART5_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON12_CLK_UART5_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000400 */
#define CRU_CLKGATE_CON12_CLK_UART5_CLK_EN_SHIFT           (11U)
#define CRU_CLKGATE_CON12_CLK_UART5_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON12_CLK_UART5_CLK_EN_SHIFT)           /* 0x00000800 */
#define CRU_CLKGATE_CON12_CLK_UART6_PLL_CLK_EN_SHIFT       (12U)
#define CRU_CLKGATE_CON12_CLK_UART6_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON12_CLK_UART6_PLL_CLK_EN_SHIFT)       /* 0x00001000 */
#define CRU_CLKGATE_CON12_CLK_UART6_DIVNP5_CLK_EN_SHIFT    (13U)
#define CRU_CLKGATE_CON12_CLK_UART6_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON12_CLK_UART6_DIVNP5_CLK_EN_SHIFT)    /* 0x00002000 */
#define CRU_CLKGATE_CON12_CLK_UART6_FRAC_SRC_CLK_EN_SHIFT  (14U)
#define CRU_CLKGATE_CON12_CLK_UART6_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON12_CLK_UART6_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00004000 */
#define CRU_CLKGATE_CON12_CLK_UART6_CLK_EN_SHIFT           (15U)
#define CRU_CLKGATE_CON12_CLK_UART6_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON12_CLK_UART6_CLK_EN_SHIFT)           /* 0x00008000 */
/* CLKGATE_CON13 */
#define CRU_CLKGATE_CON13_OFFSET                           (0x264)
#define CRU_CLKGATE_CON13_CLK_UART7_PLL_CLK_EN_SHIFT       (0U)
#define CRU_CLKGATE_CON13_CLK_UART7_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON13_CLK_UART7_PLL_CLK_EN_SHIFT)       /* 0x00000001 */
#define CRU_CLKGATE_CON13_CLK_UART7_DIVNP5_CLK_EN_SHIFT    (1U)
#define CRU_CLKGATE_CON13_CLK_UART7_DIVNP5_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON13_CLK_UART7_DIVNP5_CLK_EN_SHIFT)    /* 0x00000002 */
#define CRU_CLKGATE_CON13_CLK_UART7_FRAC_SRC_CLK_EN_SHIFT  (2U)
#define CRU_CLKGATE_CON13_CLK_UART7_FRAC_SRC_CLK_EN_MASK   (0x1U << CRU_CLKGATE_CON13_CLK_UART7_FRAC_SRC_CLK_EN_SHIFT)  /* 0x00000004 */
#define CRU_CLKGATE_CON13_CLK_UART7_CLK_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON13_CLK_UART7_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON13_CLK_UART7_CLK_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON13_CLK_I2C1_PLL_CLK_EN_SHIFT        (4U)
#define CRU_CLKGATE_CON13_CLK_I2C1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_I2C1_PLL_CLK_EN_SHIFT)        /* 0x00000010 */
#define CRU_CLKGATE_CON13_CLK_I2C2_PLL_CLK_EN_SHIFT        (5U)
#define CRU_CLKGATE_CON13_CLK_I2C2_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_I2C2_PLL_CLK_EN_SHIFT)        /* 0x00000020 */
#define CRU_CLKGATE_CON13_CLK_I2C3_PLL_CLK_EN_SHIFT        (6U)
#define CRU_CLKGATE_CON13_CLK_I2C3_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_I2C3_PLL_CLK_EN_SHIFT)        /* 0x00000040 */
#define CRU_CLKGATE_CON13_CLK_SPI0_PLL_CLK_EN_SHIFT        (7U)
#define CRU_CLKGATE_CON13_CLK_SPI0_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_SPI0_PLL_CLK_EN_SHIFT)        /* 0x00000080 */
#define CRU_CLKGATE_CON13_CLK_SPI1_PLL_CLK_EN_SHIFT        (8U)
#define CRU_CLKGATE_CON13_CLK_SPI1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_SPI1_PLL_CLK_EN_SHIFT)        /* 0x00000100 */
#define CRU_CLKGATE_CON13_CLK_SPI2_PLL_CLK_EN_SHIFT        (9U)
#define CRU_CLKGATE_CON13_CLK_SPI2_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_SPI2_PLL_CLK_EN_SHIFT)        /* 0x00000200 */
#define CRU_CLKGATE_CON13_CLK_PWM0_PLL_CLK_EN_SHIFT        (10U)
#define CRU_CLKGATE_CON13_CLK_PWM0_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_PWM0_PLL_CLK_EN_SHIFT)        /* 0x00000400 */
#define CRU_CLKGATE_CON13_CLK_PWM1_PLL_CLK_EN_SHIFT        (11U)
#define CRU_CLKGATE_CON13_CLK_PWM1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_PWM1_PLL_CLK_EN_SHIFT)        /* 0x00000800 */
#define CRU_CLKGATE_CON13_CLK_PWM2_PLL_CLK_EN_SHIFT        (12U)
#define CRU_CLKGATE_CON13_CLK_PWM2_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON13_CLK_PWM2_PLL_CLK_EN_SHIFT)        /* 0x00001000 */
#define CRU_CLKGATE_CON13_CLK_TSADC_PLL_CLK_EN_SHIFT       (13U)
#define CRU_CLKGATE_CON13_CLK_TSADC_PLL_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON13_CLK_TSADC_PLL_CLK_EN_SHIFT)       /* 0x00002000 */
#define CRU_CLKGATE_CON13_CLK_SARADC_PLL_CLK_EN_SHIFT      (14U)
#define CRU_CLKGATE_CON13_CLK_SARADC_PLL_CLK_EN_MASK       (0x1U << CRU_CLKGATE_CON13_CLK_SARADC_PLL_CLK_EN_SHIFT)      /* 0x00004000 */
/* CLKGATE_CON14 */
#define CRU_CLKGATE_CON14_OFFSET                           (0x268)
#define CRU_CLKGATE_CON14_CLK_EFUSE_S_PLL_CLK_EN_SHIFT     (0U)
#define CRU_CLKGATE_CON14_CLK_EFUSE_S_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON14_CLK_EFUSE_S_PLL_CLK_EN_SHIFT)     /* 0x00000001 */
#define CRU_CLKGATE_CON14_CLK_EFUSE_NS_PLL_CLK_EN_SHIFT    (1U)
#define CRU_CLKGATE_CON14_CLK_EFUSE_NS_PLL_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON14_CLK_EFUSE_NS_PLL_CLK_EN_SHIFT)    /* 0x00000002 */
#define CRU_CLKGATE_CON14_DBCLK_GPIO1_PLL_CLK_EN_SHIFT     (2U)
#define CRU_CLKGATE_CON14_DBCLK_GPIO1_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON14_DBCLK_GPIO1_PLL_CLK_EN_SHIFT)     /* 0x00000004 */
#define CRU_CLKGATE_CON14_DBCLK_GPIO2_PLL_CLK_EN_SHIFT     (3U)
#define CRU_CLKGATE_CON14_DBCLK_GPIO2_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON14_DBCLK_GPIO2_PLL_CLK_EN_SHIFT)     /* 0x00000008 */
#define CRU_CLKGATE_CON14_DBCLK_GPIO3_PLL_CLK_EN_SHIFT     (4U)
#define CRU_CLKGATE_CON14_DBCLK_GPIO3_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON14_DBCLK_GPIO3_PLL_CLK_EN_SHIFT)     /* 0x00000010 */
#define CRU_CLKGATE_CON14_DBCLK_GPIO4_PLL_CLK_EN_SHIFT     (5U)
#define CRU_CLKGATE_CON14_DBCLK_GPIO4_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON14_DBCLK_GPIO4_PLL_CLK_EN_SHIFT)     /* 0x00000020 */
#define CRU_CLKGATE_CON14_CLK_I2C4_PLL_CLK_EN_SHIFT        (6U)
#define CRU_CLKGATE_CON14_CLK_I2C4_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON14_CLK_I2C4_PLL_CLK_EN_SHIFT)        /* 0x00000040 */
#define CRU_CLKGATE_CON14_CLK_I2C5_PLL_CLK_EN_SHIFT        (7U)
#define CRU_CLKGATE_CON14_CLK_I2C5_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON14_CLK_I2C5_PLL_CLK_EN_SHIFT)        /* 0x00000080 */
#define CRU_CLKGATE_CON14_CLK_TIMER0_CLK_EN_SHIFT          (8U)
#define CRU_CLKGATE_CON14_CLK_TIMER0_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_CLK_TIMER0_CLK_EN_SHIFT)          /* 0x00000100 */
#define CRU_CLKGATE_CON14_CLK_TIMER1_CLK_EN_SHIFT          (9U)
#define CRU_CLKGATE_CON14_CLK_TIMER1_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_CLK_TIMER1_CLK_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE_CON14_CLK_TIMER2_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON14_CLK_TIMER2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_CLK_TIMER2_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON14_CLK_TIMER3_CLK_EN_SHIFT          (11U)
#define CRU_CLKGATE_CON14_CLK_TIMER3_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_CLK_TIMER3_CLK_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE_CON14_CLK_TIMER4_CLK_EN_SHIFT          (12U)
#define CRU_CLKGATE_CON14_CLK_TIMER4_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_CLK_TIMER4_CLK_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE_CON14_CLK_TIMER5_CLK_EN_SHIFT          (13U)
#define CRU_CLKGATE_CON14_CLK_TIMER5_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON14_CLK_TIMER5_CLK_EN_SHIFT)          /* 0x00002000 */
/* CLKGATE_CON15 */
#define CRU_CLKGATE_CON15_OFFSET                           (0x26C)
#define CRU_CLKGATE_CON15_HSCLK_BUS_NIU_CLK_EN_SHIFT       (0U)
#define CRU_CLKGATE_CON15_HSCLK_BUS_NIU_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON15_HSCLK_BUS_NIU_CLK_EN_SHIFT)       /* 0x00000001 */
#define CRU_CLKGATE_CON15_MSCLK_BUS_NIU_CLK_EN_SHIFT       (1U)
#define CRU_CLKGATE_CON15_MSCLK_BUS_NIU_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON15_MSCLK_BUS_NIU_CLK_EN_SHIFT)       /* 0x00000002 */
#define CRU_CLKGATE_CON15_MSCLK_BUS_SUB_NIU_CLK_EN_SHIFT   (2U)
#define CRU_CLKGATE_CON15_MSCLK_BUS_SUB_NIU_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON15_MSCLK_BUS_SUB_NIU_CLK_EN_SHIFT)   /* 0x00000004 */
#define CRU_CLKGATE_CON15_LSCLK_BUS_NIU_EN_SHIFT           (3U)
#define CRU_CLKGATE_CON15_LSCLK_BUS_NIU_EN_MASK            (0x1U << CRU_CLKGATE_CON15_LSCLK_BUS_NIU_EN_SHIFT)           /* 0x00000008 */
#define CRU_CLKGATE_CON15_HCLK_ROM_CLK_EN_SHIFT            (4U)
#define CRU_CLKGATE_CON15_HCLK_ROM_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON15_HCLK_ROM_CLK_EN_SHIFT)            /* 0x00000010 */
#define CRU_CLKGATE_CON15_ACLK_CRYPTO_EN_SHIFT             (5U)
#define CRU_CLKGATE_CON15_ACLK_CRYPTO_EN_MASK              (0x1U << CRU_CLKGATE_CON15_ACLK_CRYPTO_EN_SHIFT)             /* 0x00000020 */
#define CRU_CLKGATE_CON15_HCLK_CRYPTO_EN_SHIFT             (6U)
#define CRU_CLKGATE_CON15_HCLK_CRYPTO_EN_MASK              (0x1U << CRU_CLKGATE_CON15_HCLK_CRYPTO_EN_SHIFT)             /* 0x00000040 */
#define CRU_CLKGATE_CON15_ACLK_DCF_CLK_EN_SHIFT            (7U)
#define CRU_CLKGATE_CON15_ACLK_DCF_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON15_ACLK_DCF_CLK_EN_SHIFT)            /* 0x00000080 */
#define CRU_CLKGATE_CON15_PCLK_DCF_CLK_EN_SHIFT            (8U)
#define CRU_CLKGATE_CON15_PCLK_DCF_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON15_PCLK_DCF_CLK_EN_SHIFT)            /* 0x00000100 */
#define CRU_CLKGATE_CON15_PCLK_UART1_CLK_EN_SHIFT          (9U)
#define CRU_CLKGATE_CON15_PCLK_UART1_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART1_CLK_EN_SHIFT)          /* 0x00000200 */
#define CRU_CLKGATE_CON15_PCLK_UART2_CLK_EN_SHIFT          (10U)
#define CRU_CLKGATE_CON15_PCLK_UART2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART2_CLK_EN_SHIFT)          /* 0x00000400 */
#define CRU_CLKGATE_CON15_PCLK_UART3_CLK_EN_SHIFT          (11U)
#define CRU_CLKGATE_CON15_PCLK_UART3_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART3_CLK_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE_CON15_PCLK_UART4_CLK_EN_SHIFT          (12U)
#define CRU_CLKGATE_CON15_PCLK_UART4_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART4_CLK_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE_CON15_PCLK_UART5_CLK_EN_SHIFT          (13U)
#define CRU_CLKGATE_CON15_PCLK_UART5_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART5_CLK_EN_SHIFT)          /* 0x00002000 */
#define CRU_CLKGATE_CON15_PCLK_UART6_CLK_EN_SHIFT          (14U)
#define CRU_CLKGATE_CON15_PCLK_UART6_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART6_CLK_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE_CON15_PCLK_UART7_CLK_EN_SHIFT          (15U)
#define CRU_CLKGATE_CON15_PCLK_UART7_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON15_PCLK_UART7_CLK_EN_SHIFT)          /* 0x00008000 */
/* CLKGATE_CON16 */
#define CRU_CLKGATE_CON16_OFFSET                           (0x270)
#define CRU_CLKGATE_CON16_PCLK_I2C1_CLK_EN_SHIFT           (0U)
#define CRU_CLKGATE_CON16_PCLK_I2C1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON16_PCLK_I2C1_CLK_EN_SHIFT)           /* 0x00000001 */
#define CRU_CLKGATE_CON16_PCLK_I2C2_CLK_EN_SHIFT           (1U)
#define CRU_CLKGATE_CON16_PCLK_I2C2_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON16_PCLK_I2C2_CLK_EN_SHIFT)           /* 0x00000002 */
#define CRU_CLKGATE_CON16_PCLK_I2C3_CLK_EN_SHIFT           (2U)
#define CRU_CLKGATE_CON16_PCLK_I2C3_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON16_PCLK_I2C3_CLK_EN_SHIFT)           /* 0x00000004 */
#define CRU_CLKGATE_CON16_PCLK_SPI0_EN_SHIFT               (3U)
#define CRU_CLKGATE_CON16_PCLK_SPI0_EN_MASK                (0x1U << CRU_CLKGATE_CON16_PCLK_SPI0_EN_SHIFT)               /* 0x00000008 */
#define CRU_CLKGATE_CON16_PCLK_SPI1_EN_SHIFT               (4U)
#define CRU_CLKGATE_CON16_PCLK_SPI1_EN_MASK                (0x1U << CRU_CLKGATE_CON16_PCLK_SPI1_EN_SHIFT)               /* 0x00000010 */
#define CRU_CLKGATE_CON16_PCLK_SPI2_EN_SHIFT               (5U)
#define CRU_CLKGATE_CON16_PCLK_SPI2_EN_MASK                (0x1U << CRU_CLKGATE_CON16_PCLK_SPI2_EN_SHIFT)               /* 0x00000020 */
#define CRU_CLKGATE_CON16_PCLK_PWM0_CLK_EN_SHIFT           (6U)
#define CRU_CLKGATE_CON16_PCLK_PWM0_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON16_PCLK_PWM0_CLK_EN_SHIFT)           /* 0x00000040 */
#define CRU_CLKGATE_CON16_PCLK_PWM1_CLK_EN_SHIFT           (7U)
#define CRU_CLKGATE_CON16_PCLK_PWM1_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON16_PCLK_PWM1_CLK_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE_CON16_PCLK_PWM2_CLK_EN_SHIFT           (8U)
#define CRU_CLKGATE_CON16_PCLK_PWM2_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON16_PCLK_PWM2_CLK_EN_SHIFT)           /* 0x00000100 */
#define CRU_CLKGATE_CON16_PCLK_TSADC_EN_SHIFT              (9U)
#define CRU_CLKGATE_CON16_PCLK_TSADC_EN_MASK               (0x1U << CRU_CLKGATE_CON16_PCLK_TSADC_EN_SHIFT)              /* 0x00000200 */
#define CRU_CLKGATE_CON16_PCLK_SARADC_EN_SHIFT             (10U)
#define CRU_CLKGATE_CON16_PCLK_SARADC_EN_MASK              (0x1U << CRU_CLKGATE_CON16_PCLK_SARADC_EN_SHIFT)             /* 0x00000400 */
#define CRU_CLKGATE_CON16_PCLK_EFUSE_NS_CLK_EN_SHIFT       (11U)
#define CRU_CLKGATE_CON16_PCLK_EFUSE_NS_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON16_PCLK_EFUSE_NS_CLK_EN_SHIFT)       /* 0x00000800 */
#define CRU_CLKGATE_CON16_PCLK_GPIO1_CLK_EN_SHIFT          (12U)
#define CRU_CLKGATE_CON16_PCLK_GPIO1_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON16_PCLK_GPIO1_CLK_EN_SHIFT)          /* 0x00001000 */
#define CRU_CLKGATE_CON16_PCLK_GPIO2_CLK_EN_SHIFT          (13U)
#define CRU_CLKGATE_CON16_PCLK_GPIO2_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON16_PCLK_GPIO2_CLK_EN_SHIFT)          /* 0x00002000 */
#define CRU_CLKGATE_CON16_PCLK_GPIO3_CLK_EN_SHIFT          (14U)
#define CRU_CLKGATE_CON16_PCLK_GPIO3_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON16_PCLK_GPIO3_CLK_EN_SHIFT)          /* 0x00004000 */
#define CRU_CLKGATE_CON16_PCLK_GPIO4_CLK_EN_SHIFT          (15U)
#define CRU_CLKGATE_CON16_PCLK_GPIO4_CLK_EN_MASK           (0x1U << CRU_CLKGATE_CON16_PCLK_GPIO4_CLK_EN_SHIFT)          /* 0x00008000 */
/* CLKGATE_CON17 */
#define CRU_CLKGATE_CON17_OFFSET                           (0x274)
#define CRU_CLKGATE_CON17_PCLK_TIMER0_EN_SHIFT             (0U)
#define CRU_CLKGATE_CON17_PCLK_TIMER0_EN_MASK              (0x1U << CRU_CLKGATE_CON17_PCLK_TIMER0_EN_SHIFT)             /* 0x00000001 */
#define CRU_CLKGATE_CON17_PCLK_WDT_NS_CLK_EN_SHIFT         (1U)
#define CRU_CLKGATE_CON17_PCLK_WDT_NS_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON17_PCLK_WDT_NS_CLK_EN_SHIFT)         /* 0x00000002 */
#define CRU_CLKGATE_CON17_PCLK_BUS_GRF_CLK_EN_SHIFT        (2U)
#define CRU_CLKGATE_CON17_PCLK_BUS_GRF_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON17_PCLK_BUS_GRF_CLK_EN_SHIFT)        /* 0x00000004 */
#define CRU_CLKGATE_CON17_PCLK_BUS_SGRF_CLK_EN_SHIFT       (3U)
#define CRU_CLKGATE_CON17_PCLK_BUS_SGRF_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON17_PCLK_BUS_SGRF_CLK_EN_SHIFT)       /* 0x00000008 */
#define CRU_CLKGATE_CON17_PCLK_I2C4_CLK_EN_SHIFT           (4U)
#define CRU_CLKGATE_CON17_PCLK_I2C4_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON17_PCLK_I2C4_CLK_EN_SHIFT)           /* 0x00000010 */
#define CRU_CLKGATE_CON17_PCLK_I2C5_CLK_EN_SHIFT           (5U)
#define CRU_CLKGATE_CON17_PCLK_I2C5_CLK_EN_MASK            (0x1U << CRU_CLKGATE_CON17_PCLK_I2C5_CLK_EN_SHIFT)           /* 0x00000020 */
#define CRU_CLKGATE_CON17_HCLK_PD_AUDIO_CLK_EN_SHIFT       (8U)
#define CRU_CLKGATE_CON17_HCLK_PD_AUDIO_CLK_EN_MASK        (0x1U << CRU_CLKGATE_CON17_HCLK_PD_AUDIO_CLK_EN_SHIFT)       /* 0x00000100 */
#define CRU_CLKGATE_CON17_CLK_PDM_PLL_CLK_EN_SHIFT         (9U)
#define CRU_CLKGATE_CON17_CLK_PDM_PLL_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON17_CLK_PDM_PLL_CLK_EN_SHIFT)         /* 0x00000200 */
#define CRU_CLKGATE_CON17_CLK_PDM_FRAC_SRC_CLK_EN_SHIFT    (10U)
#define CRU_CLKGATE_CON17_CLK_PDM_FRAC_SRC_CLK_EN_MASK     (0x1U << CRU_CLKGATE_CON17_CLK_PDM_FRAC_SRC_CLK_EN_SHIFT)    /* 0x00000400 */
#define CRU_CLKGATE_CON17_CLK_PDM_CLK_EN_SHIFT             (11U)
#define CRU_CLKGATE_CON17_CLK_PDM_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON17_CLK_PDM_CLK_EN_SHIFT)             /* 0x00000800 */
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_PLL_CLK_EN_SHIFT     (12U)
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_TX_PLL_CLK_EN_SHIFT)     /* 0x00001000 */
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_FRAC_SRC_CLK_EN_SHIFT (13U)
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_FRAC_SRC_CLK_EN_MASK (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_TX_FRAC_SRC_CLK_EN_SHIFT) /* 0x00002000 */
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_CLK_EN_SHIFT         (14U)
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_TX_CLK_EN_SHIFT)         /* 0x00004000 */
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_OUT_MCLK_EN_SHIFT    (15U)
#define CRU_CLKGATE_CON17_CLK_I2S0_TX_OUT_MCLK_EN_MASK     (0x1U << CRU_CLKGATE_CON17_CLK_I2S0_TX_OUT_MCLK_EN_SHIFT)    /* 0x00008000 */
/* CLKGATE_CON18 */
#define CRU_CLKGATE_CON18_OFFSET                           (0x278)
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_PLL_CLK_EN_SHIFT     (0U)
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_PLL_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON18_CLK_I2S0_RX_PLL_CLK_EN_SHIFT)     /* 0x00000001 */
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_FRAC_SRC_CLK_EN_SHIFT (1U)
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_FRAC_SRC_CLK_EN_MASK (0x1U << CRU_CLKGATE_CON18_CLK_I2S0_RX_FRAC_SRC_CLK_EN_SHIFT) /* 0x00000002 */
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_CLK_EN_SHIFT         (2U)
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON18_CLK_I2S0_RX_CLK_EN_SHIFT)         /* 0x00000004 */
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_OUT_MCLK_EN_SHIFT    (3U)
#define CRU_CLKGATE_CON18_CLK_I2S0_RX_OUT_MCLK_EN_MASK     (0x1U << CRU_CLKGATE_CON18_CLK_I2S0_RX_OUT_MCLK_EN_SHIFT)    /* 0x00000008 */
#define CRU_CLKGATE_CON18_CLK_I2S1_PLL_CLK_EN_SHIFT        (4U)
#define CRU_CLKGATE_CON18_CLK_I2S1_PLL_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON18_CLK_I2S1_PLL_CLK_EN_SHIFT)        /* 0x00000010 */
#define CRU_CLKGATE_CON18_CLK_I2S1_FRAC_SRC_CLK_EN_SHIFT   (5U)
#define CRU_CLKGATE_CON18_CLK_I2S1_FRAC_SRC_CLK_EN_MASK    (0x1U << CRU_CLKGATE_CON18_CLK_I2S1_FRAC_SRC_CLK_EN_SHIFT)   /* 0x00000020 */
#define CRU_CLKGATE_CON18_CLK_I2S1_CLK_EN_SHIFT            (6U)
#define CRU_CLKGATE_CON18_CLK_I2S1_CLK_EN_MASK             (0x1U << CRU_CLKGATE_CON18_CLK_I2S1_CLK_EN_SHIFT)            /* 0x00000040 */
#define CRU_CLKGATE_CON18_CLK_I2S1_OUT_MCLK_EN_SHIFT       (7U)
#define CRU_CLKGATE_CON18_CLK_I2S1_OUT_MCLK_EN_MASK        (0x1U << CRU_CLKGATE_CON18_CLK_I2S1_OUT_MCLK_EN_SHIFT)       /* 0x00000080 */
#define CRU_CLKGATE_CON18_HCLK_AUDIO_NIU_EN_SHIFT          (11U)
#define CRU_CLKGATE_CON18_HCLK_AUDIO_NIU_EN_MASK           (0x1U << CRU_CLKGATE_CON18_HCLK_AUDIO_NIU_EN_SHIFT)          /* 0x00000800 */
#define CRU_CLKGATE_CON18_HCLK_VAD_EN_SHIFT                (12U)
#define CRU_CLKGATE_CON18_HCLK_VAD_EN_MASK                 (0x1U << CRU_CLKGATE_CON18_HCLK_VAD_EN_SHIFT)                /* 0x00001000 */
#define CRU_CLKGATE_CON18_HCLK_PDM_EN_SHIFT                (13U)
#define CRU_CLKGATE_CON18_HCLK_PDM_EN_MASK                 (0x1U << CRU_CLKGATE_CON18_HCLK_PDM_EN_SHIFT)                /* 0x00002000 */
#define CRU_CLKGATE_CON18_HCLK_I2S0_EN_SHIFT               (14U)
#define CRU_CLKGATE_CON18_HCLK_I2S0_EN_MASK                (0x1U << CRU_CLKGATE_CON18_HCLK_I2S0_EN_SHIFT)               /* 0x00004000 */
#define CRU_CLKGATE_CON18_HCLK_I2S1_EN_SHIFT               (15U)
#define CRU_CLKGATE_CON18_HCLK_I2S1_EN_MASK                (0x1U << CRU_CLKGATE_CON18_HCLK_I2S1_EN_SHIFT)               /* 0x00008000 */
/* CLKGATE_CON19 */
#define CRU_CLKGATE_CON19_OFFSET                           (0x27C)
#define CRU_CLKGATE_CON19_PCLK_TOP_NIU_CLK_EN_SHIFT        (0U)
#define CRU_CLKGATE_CON19_PCLK_TOP_NIU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON19_PCLK_TOP_NIU_CLK_EN_SHIFT)        /* 0x00000001 */
#define CRU_CLKGATE_CON19_PCLK_TOP_CRU_CLK_EN_SHIFT        (1U)
#define CRU_CLKGATE_CON19_PCLK_TOP_CRU_CLK_EN_MASK         (0x1U << CRU_CLKGATE_CON19_PCLK_TOP_CRU_CLK_EN_SHIFT)        /* 0x00000002 */
#define CRU_CLKGATE_CON19_PCLK_DDRPHY_CLK_EN_SHIFT         (2U)
#define CRU_CLKGATE_CON19_PCLK_DDRPHY_CLK_EN_MASK          (0x1U << CRU_CLKGATE_CON19_PCLK_DDRPHY_CLK_EN_SHIFT)         /* 0x00000004 */
#define CRU_CLKGATE_CON19_PCLK_MIPIDSIPHY_CLK_EN_SHIFT     (3U)
#define CRU_CLKGATE_CON19_PCLK_MIPIDSIPHY_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON19_PCLK_MIPIDSIPHY_CLK_EN_SHIFT)     /* 0x00000008 */
#define CRU_CLKGATE_CON19_PCLK_MIPICSIPHY_CLK_EN_SHIFT     (4U)
#define CRU_CLKGATE_CON19_PCLK_MIPICSIPHY_CLK_EN_MASK      (0x1U << CRU_CLKGATE_CON19_PCLK_MIPICSIPHY_CLK_EN_SHIFT)     /* 0x00000010 */
#define CRU_CLKGATE_CON19_PCLK_USB3PHY_PIPE_EN_SHIFT       (6U)
#define CRU_CLKGATE_CON19_PCLK_USB3PHY_PIPE_EN_MASK        (0x1U << CRU_CLKGATE_CON19_PCLK_USB3PHY_PIPE_EN_SHIFT)       /* 0x00000040 */
#define CRU_CLKGATE_CON19_PCLK_USB3_GRF_EN_SHIFT           (7U)
#define CRU_CLKGATE_CON19_PCLK_USB3_GRF_EN_MASK            (0x1U << CRU_CLKGATE_CON19_PCLK_USB3_GRF_EN_SHIFT)           /* 0x00000080 */
#define CRU_CLKGATE_CON19_PCLK_USB_GRF_EN_SHIFT            (8U)
#define CRU_CLKGATE_CON19_PCLK_USB_GRF_EN_MASK             (0x1U << CRU_CLKGATE_CON19_PCLK_USB_GRF_EN_SHIFT)            /* 0x00000100 */
#define CRU_CLKGATE_CON19_TESTCLK_CLK_EN_SHIFT             (15U)
#define CRU_CLKGATE_CON19_TESTCLK_CLK_EN_MASK              (0x1U << CRU_CLKGATE_CON19_TESTCLK_CLK_EN_SHIFT)             /* 0x00008000 */
/* SSGTBL0_3 */
#define CRU_SSGTBL0_3_OFFSET                               (0x280)
#define CRU_SSGTBL0_3_SSGTBL0_3_SHIFT                      (0U)
#define CRU_SSGTBL0_3_SSGTBL0_3_MASK                       (0xFFFFFFFFU << CRU_SSGTBL0_3_SSGTBL0_3_SHIFT)               /* 0xFFFFFFFF */
/* SSGTBL4_7 */
#define CRU_SSGTBL4_7_OFFSET                               (0x284)
#define CRU_SSGTBL4_7_SSGTBL4_7_SHIFT                      (0U)
#define CRU_SSGTBL4_7_SSGTBL4_7_MASK                       (0xFFFFFFFFU << CRU_SSGTBL4_7_SSGTBL4_7_SHIFT)               /* 0xFFFFFFFF */
/* SSGTBL8_11 */
#define CRU_SSGTBL8_11_OFFSET                              (0x288)
#define CRU_SSGTBL8_11_SSGTBL8_11_SHIFT                    (0U)
#define CRU_SSGTBL8_11_SSGTBL8_11_MASK                     (0xFFFFFFFFU << CRU_SSGTBL8_11_SSGTBL8_11_SHIFT)             /* 0xFFFFFFFF */
/* SSGTBL12_15 */
#define CRU_SSGTBL12_15_OFFSET                             (0x28C)
#define CRU_SSGTBL12_15_SSGTBL12_15_SHIFT                  (0U)
#define CRU_SSGTBL12_15_SSGTBL12_15_MASK                   (0xFFFFFFFFU << CRU_SSGTBL12_15_SSGTBL12_15_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL16_19 */
#define CRU_SSGTBL16_19_OFFSET                             (0x290)
#define CRU_SSGTBL16_19_SSGTBL16_19_SHIFT                  (0U)
#define CRU_SSGTBL16_19_SSGTBL16_19_MASK                   (0xFFFFFFFFU << CRU_SSGTBL16_19_SSGTBL16_19_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL20_23 */
#define CRU_SSGTBL20_23_OFFSET                             (0x294)
#define CRU_SSGTBL20_23_SSGTBL20_23_SHIFT                  (0U)
#define CRU_SSGTBL20_23_SSGTBL20_23_MASK                   (0xFFFFFFFFU << CRU_SSGTBL20_23_SSGTBL20_23_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL24_27 */
#define CRU_SSGTBL24_27_OFFSET                             (0x298)
#define CRU_SSGTBL24_27_SSGTBL24_27_SHIFT                  (0U)
#define CRU_SSGTBL24_27_SSGTBL24_27_MASK                   (0xFFFFFFFFU << CRU_SSGTBL24_27_SSGTBL24_27_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL28_31 */
#define CRU_SSGTBL28_31_OFFSET                             (0x29C)
#define CRU_SSGTBL28_31_SSGTBL28_31_SHIFT                  (0U)
#define CRU_SSGTBL28_31_SSGTBL28_31_MASK                   (0xFFFFFFFFU << CRU_SSGTBL28_31_SSGTBL28_31_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL32_35 */
#define CRU_SSGTBL32_35_OFFSET                             (0x2A0)
#define CRU_SSGTBL32_35_SSGTBL32_35_SHIFT                  (0U)
#define CRU_SSGTBL32_35_SSGTBL32_35_MASK                   (0xFFFFFFFFU << CRU_SSGTBL32_35_SSGTBL32_35_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL36_39 */
#define CRU_SSGTBL36_39_OFFSET                             (0x2A4)
#define CRU_SSGTBL36_39_SSGTBL36_39_SHIFT                  (0U)
#define CRU_SSGTBL36_39_SSGTBL36_39_MASK                   (0xFFFFFFFFU << CRU_SSGTBL36_39_SSGTBL36_39_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL40_43 */
#define CRU_SSGTBL40_43_OFFSET                             (0x2A8)
#define CRU_SSGTBL40_43_SSGTBL40_43_SHIFT                  (0U)
#define CRU_SSGTBL40_43_SSGTBL40_43_MASK                   (0xFFFFFFFFU << CRU_SSGTBL40_43_SSGTBL40_43_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL44_47 */
#define CRU_SSGTBL44_47_OFFSET                             (0x2AC)
#define CRU_SSGTBL44_47_SSGTBL44_47_SHIFT                  (0U)
#define CRU_SSGTBL44_47_SSGTBL44_47_MASK                   (0xFFFFFFFFU << CRU_SSGTBL44_47_SSGTBL44_47_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL48_51 */
#define CRU_SSGTBL48_51_OFFSET                             (0x2B0)
#define CRU_SSGTBL48_51_SSGTBL48_51_SHIFT                  (0U)
#define CRU_SSGTBL48_51_SSGTBL48_51_MASK                   (0xFFFFFFFFU << CRU_SSGTBL48_51_SSGTBL48_51_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL52_55 */
#define CRU_SSGTBL52_55_OFFSET                             (0x2B4)
#define CRU_SSGTBL52_55_SSGTBL52_55_SHIFT                  (0U)
#define CRU_SSGTBL52_55_SSGTBL52_55_MASK                   (0xFFFFFFFFU << CRU_SSGTBL52_55_SSGTBL52_55_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL56_59 */
#define CRU_SSGTBL56_59_OFFSET                             (0x2B8)
#define CRU_SSGTBL56_59_SSGTBL56_59_SHIFT                  (0U)
#define CRU_SSGTBL56_59_SSGTBL56_59_MASK                   (0xFFFFFFFFU << CRU_SSGTBL56_59_SSGTBL56_59_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL60_63 */
#define CRU_SSGTBL60_63_OFFSET                             (0x2BC)
#define CRU_SSGTBL60_63_SSGTBL60_63_SHIFT                  (0U)
#define CRU_SSGTBL60_63_SSGTBL60_63_MASK                   (0xFFFFFFFFU << CRU_SSGTBL60_63_SSGTBL60_63_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL64_67 */
#define CRU_SSGTBL64_67_OFFSET                             (0x2C0)
#define CRU_SSGTBL64_67_SSGTBL64_67_SHIFT                  (0U)
#define CRU_SSGTBL64_67_SSGTBL64_67_MASK                   (0xFFFFFFFFU << CRU_SSGTBL64_67_SSGTBL64_67_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL68_71 */
#define CRU_SSGTBL68_71_OFFSET                             (0x2C4)
#define CRU_SSGTBL68_71_SSGTBL68_71_SHIFT                  (0U)
#define CRU_SSGTBL68_71_SSGTBL68_71_MASK                   (0xFFFFFFFFU << CRU_SSGTBL68_71_SSGTBL68_71_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL72_75 */
#define CRU_SSGTBL72_75_OFFSET                             (0x2C8)
#define CRU_SSGTBL72_75_SSGTBL72_75_SHIFT                  (0U)
#define CRU_SSGTBL72_75_SSGTBL72_75_MASK                   (0xFFFFFFFFU << CRU_SSGTBL72_75_SSGTBL72_75_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL76_79 */
#define CRU_SSGTBL76_79_OFFSET                             (0x2CC)
#define CRU_SSGTBL76_79_SSGTBL76_79_SHIFT                  (0U)
#define CRU_SSGTBL76_79_SSGTBL76_79_MASK                   (0xFFFFFFFFU << CRU_SSGTBL76_79_SSGTBL76_79_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL80_83 */
#define CRU_SSGTBL80_83_OFFSET                             (0x2D0)
#define CRU_SSGTBL80_83_SSGTBL80_83_SHIFT                  (0U)
#define CRU_SSGTBL80_83_SSGTBL80_83_MASK                   (0xFFFFFFFFU << CRU_SSGTBL80_83_SSGTBL80_83_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL84_87 */
#define CRU_SSGTBL84_87_OFFSET                             (0x2D4)
#define CRU_SSGTBL84_87_SSGTBL84_87_SHIFT                  (0U)
#define CRU_SSGTBL84_87_SSGTBL84_87_MASK                   (0xFFFFFFFFU << CRU_SSGTBL84_87_SSGTBL84_87_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL88_91 */
#define CRU_SSGTBL88_91_OFFSET                             (0x2D8)
#define CRU_SSGTBL88_91_SSGTBL88_91_SHIFT                  (0U)
#define CRU_SSGTBL88_91_SSGTBL88_91_MASK                   (0xFFFFFFFFU << CRU_SSGTBL88_91_SSGTBL88_91_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL92_95 */
#define CRU_SSGTBL92_95_OFFSET                             (0x2DC)
#define CRU_SSGTBL92_95_SSGTBL92_95_SHIFT                  (0U)
#define CRU_SSGTBL92_95_SSGTBL92_95_MASK                   (0xFFFFFFFFU << CRU_SSGTBL92_95_SSGTBL92_95_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL96_99 */
#define CRU_SSGTBL96_99_OFFSET                             (0x2E0)
#define CRU_SSGTBL96_99_SSGTBL96_99_SHIFT                  (0U)
#define CRU_SSGTBL96_99_SSGTBL96_99_MASK                   (0xFFFFFFFFU << CRU_SSGTBL96_99_SSGTBL96_99_SHIFT)           /* 0xFFFFFFFF */
/* SSGTBL100_103 */
#define CRU_SSGTBL100_103_OFFSET                           (0x2E4)
#define CRU_SSGTBL100_103_SSGTBL100_103_SHIFT              (0U)
#define CRU_SSGTBL100_103_SSGTBL100_103_MASK               (0xFFFFFFFFU << CRU_SSGTBL100_103_SSGTBL100_103_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL104_107 */
#define CRU_SSGTBL104_107_OFFSET                           (0x2E8)
#define CRU_SSGTBL104_107_SSGTBL104_107_SHIFT              (0U)
#define CRU_SSGTBL104_107_SSGTBL104_107_MASK               (0xFFFFFFFFU << CRU_SSGTBL104_107_SSGTBL104_107_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL108_111 */
#define CRU_SSGTBL108_111_OFFSET                           (0x2EC)
#define CRU_SSGTBL108_111_SSGTBL108_111_SHIFT              (0U)
#define CRU_SSGTBL108_111_SSGTBL108_111_MASK               (0xFFFFFFFFU << CRU_SSGTBL108_111_SSGTBL108_111_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL112_115 */
#define CRU_SSGTBL112_115_OFFSET                           (0x2F0)
#define CRU_SSGTBL112_115_SSGTBL112_115_SHIFT              (0U)
#define CRU_SSGTBL112_115_SSGTBL112_115_MASK               (0xFFFFFFFFU << CRU_SSGTBL112_115_SSGTBL112_115_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL116_119 */
#define CRU_SSGTBL116_119_OFFSET                           (0x2F4)
#define CRU_SSGTBL116_119_SSGTBL116_119_SHIFT              (0U)
#define CRU_SSGTBL116_119_SSGTBL116_119_MASK               (0xFFFFFFFFU << CRU_SSGTBL116_119_SSGTBL116_119_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL120_123 */
#define CRU_SSGTBL120_123_OFFSET                           (0x2F8)
#define CRU_SSGTBL120_123_SSGTBL120_123_SHIFT              (0U)
#define CRU_SSGTBL120_123_SSGTBL120_123_MASK               (0xFFFFFFFFU << CRU_SSGTBL120_123_SSGTBL120_123_SHIFT)       /* 0xFFFFFFFF */
/* SSGTBL124_127 */
#define CRU_SSGTBL124_127_OFFSET                           (0x2FC)
#define CRU_SSGTBL124_127_SSGTBL124_127_SHIFT              (0U)
#define CRU_SSGTBL124_127_SSGTBL124_127_MASK               (0xFFFFFFFFU << CRU_SSGTBL124_127_SSGTBL124_127_SHIFT)       /* 0xFFFFFFFF */
/* SOFTRST_CON0 */
#define CRU_SOFTRST_CON0_OFFSET                            (0x300)
#define CRU_SOFTRST_CON0_COREPO0_SRSTN_REQ_SHIFT           (0U)
#define CRU_SOFTRST_CON0_COREPO0_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON0_COREPO0_SRSTN_REQ_SHIFT)           /* 0x00000001 */
#define CRU_SOFTRST_CON0_COREPO1_SRSTN_REQ_SHIFT           (1U)
#define CRU_SOFTRST_CON0_COREPO1_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON0_COREPO1_SRSTN_REQ_SHIFT)           /* 0x00000002 */
#define CRU_SOFTRST_CON0_CORE0_SRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST_CON0_CORE0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON0_CORE0_SRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON0_CORE1_SRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON0_CORE1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON0_CORE1_SRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON0_CORE0_DBG_SRSTN_REQ_SHIFT         (4U)
#define CRU_SOFTRST_CON0_CORE0_DBG_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE0_DBG_SRSTN_REQ_SHIFT)         /* 0x00000010 */
#define CRU_SOFTRST_CON0_CORE1_DBG_SRSTN_REQ_SHIFT         (5U)
#define CRU_SOFTRST_CON0_CORE1_DBG_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE1_DBG_SRSTN_REQ_SHIFT)         /* 0x00000020 */
#define CRU_SOFTRST_CON0_TOPDBG_SRSTN_REQ_SHIFT            (6U)
#define CRU_SOFTRST_CON0_TOPDBG_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON0_TOPDBG_SRSTN_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST_CON0_CORE_NOC_SRSTN_REQ_SHIFT          (7U)
#define CRU_SOFTRST_CON0_CORE_NOC_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON0_CORE_NOC_SRSTN_REQ_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST_CON0_STRC_SYS_ASRSTN_REQ_SHIFT         (8U)
#define CRU_SOFTRST_CON0_STRC_SYS_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_STRC_SYS_ASRSTN_REQ_SHIFT)         /* 0x00000100 */
#define CRU_SOFTRST_CON0_L2_SRSTN_REQ_SHIFT                (9U)
#define CRU_SOFTRST_CON0_L2_SRSTN_REQ_MASK                 (0x1U << CRU_SOFTRST_CON0_L2_SRSTN_REQ_SHIFT)                /* 0x00000200 */
#define CRU_SOFTRST_CON0_DAP_SRSTN_REQ_SHIFT               (10U)
#define CRU_SOFTRST_CON0_DAP_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON0_DAP_SRSTN_REQ_SHIFT)               /* 0x00000400 */
#define CRU_SOFTRST_CON0_CORE_MSNIU_SRSTN_REQ_SHIFT        (11U)
#define CRU_SOFTRST_CON0_CORE_MSNIU_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON0_CORE_MSNIU_SRSTN_REQ_SHIFT)        /* 0x00000800 */
#define CRU_SOFTRST_CON0_PDCORE_ADB400_GIC2CORE_ASRSTN_REQ_SHIFT (12U)
#define CRU_SOFTRST_CON0_PDCORE_ADB400_GIC2CORE_ASRSTN_REQ_MASK (0x1U << CRU_SOFTRST_CON0_PDCORE_ADB400_GIC2CORE_ASRSTN_REQ_SHIFT) /* 0x00001000 */
#define CRU_SOFTRST_CON0_PDCORE_ADB400_CORE2GIC_ASRSTN_REQ_SHIFT (13U)
#define CRU_SOFTRST_CON0_PDCORE_ADB400_CORE2GIC_ASRSTN_REQ_MASK (0x1U << CRU_SOFTRST_CON0_PDCORE_ADB400_CORE2GIC_ASRSTN_REQ_SHIFT) /* 0x00002000 */
#define CRU_SOFTRST_CON0_CORE_PRF_ASRSTN_REQ_SHIFT         (14U)
#define CRU_SOFTRST_CON0_CORE_PRF_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE_PRF_ASRSTN_REQ_SHIFT)         /* 0x00004000 */
#define CRU_SOFTRST_CON0_CORE_GRF_PSRSTN_REQ_SHIFT         (15U)
#define CRU_SOFTRST_CON0_CORE_GRF_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON0_CORE_GRF_PSRSTN_REQ_SHIFT)         /* 0x00008000 */
/* SOFTRST_CON1 */
#define CRU_SOFTRST_CON1_OFFSET                            (0x304)
#define CRU_SOFTRST_CON1_DDRPHY_SRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST_CON1_DDRPHY_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON1_DDRPHY_SRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST_CON1_DDRPHY_PSRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON1_DDRPHY_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON1_DDRPHY_PSRSTN_REQ_SHIFT)           /* 0x00000004 */
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
#define CRU_SOFTRST_CON1_DDRDFI_CTL_SRSTN_REQ_SHIFT        (14U)
#define CRU_SOFTRST_CON1_DDRDFI_CTL_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON1_DDRDFI_CTL_SRSTN_REQ_SHIFT)        /* 0x00004000 */
#define CRU_SOFTRST_CON1_DDRDFI_CTL_PSRSTN_REQ_SHIFT       (15U)
#define CRU_SOFTRST_CON1_DDRDFI_CTL_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON1_DDRDFI_CTL_PSRSTN_REQ_SHIFT)       /* 0x00008000 */
/* SOFTRST_CON2 */
#define CRU_SOFTRST_CON2_OFFSET                            (0x308)
#define CRU_SOFTRST_CON2_GIC500_NIU_ASRSTN_REQ_SHIFT       (0U)
#define CRU_SOFTRST_CON2_GIC500_NIU_ASRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON2_GIC500_NIU_ASRSTN_REQ_SHIFT)       /* 0x00000001 */
#define CRU_SOFTRST_CON2_GIC500_ASRSTN_REQ_SHIFT           (1U)
#define CRU_SOFTRST_CON2_GIC500_ASRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON2_GIC500_ASRSTN_REQ_SHIFT)           /* 0x00000002 */
#define CRU_SOFTRST_CON2_PDGIC_ADB400_CORE2GIC_ASRSTN_REQ_SHIFT (2U)
#define CRU_SOFTRST_CON2_PDGIC_ADB400_CORE2GIC_ASRSTN_REQ_MASK (0x1U << CRU_SOFTRST_CON2_PDGIC_ADB400_CORE2GIC_ASRSTN_REQ_SHIFT) /* 0x00000004 */
#define CRU_SOFTRST_CON2_PDGIC_ADB400_GIC2CORE_ASRSTN_REQ_SHIFT (3U)
#define CRU_SOFTRST_CON2_PDGIC_ADB400_GIC2CORE_ASRSTN_REQ_MASK (0x1U << CRU_SOFTRST_CON2_PDGIC_ADB400_GIC2CORE_ASRSTN_REQ_SHIFT) /* 0x00000008 */
#define CRU_SOFTRST_CON2_NPU_CORE_SRSTN_REQ_SHIFT          (4U)
#define CRU_SOFTRST_CON2_NPU_CORE_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_NPU_CORE_SRSTN_REQ_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST_CON2_NPU_ASRSTN_REQ_SHIFT              (5U)
#define CRU_SOFTRST_CON2_NPU_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_NPU_ASRSTN_REQ_SHIFT)              /* 0x00000020 */
#define CRU_SOFTRST_CON2_NPU_HSRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST_CON2_NPU_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON2_NPU_HSRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST_CON2_NPU_NIU_ASRSTN_REQ_SHIFT          (7U)
#define CRU_SOFTRST_CON2_NPU_NIU_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_NPU_NIU_ASRSTN_REQ_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST_CON2_NPU_NIU_HSRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST_CON2_NPU_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_NPU_NIU_HSRSTN_REQ_SHIFT)          /* 0x00000100 */
#define CRU_SOFTRST_CON2_NPU2MEM_ASRSTN_REQ_SHIFT          (9U)
#define CRU_SOFTRST_CON2_NPU2MEM_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_NPU2MEM_ASRSTN_REQ_SHIFT)          /* 0x00000200 */
#define CRU_SOFTRST_CON2_NPU_PVTM_SRSTN_REQ_SHIFT          (10U)
#define CRU_SOFTRST_CON2_NPU_PVTM_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON2_NPU_PVTM_SRSTN_REQ_SHIFT)          /* 0x00000400 */
#define CRU_SOFTRST_CON2_CORE_PVTM_SRSTN_REQ_SHIFT         (11U)
#define CRU_SOFTRST_CON2_CORE_PVTM_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON2_CORE_PVTM_SRSTN_REQ_SHIFT)         /* 0x00000800 */
#define CRU_SOFTRST_CON2_PDGIC_SPINLOCK_ASRSTN_REQ_SHIFT   (15U)
#define CRU_SOFTRST_CON2_PDGIC_SPINLOCK_ASRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON2_PDGIC_SPINLOCK_ASRSTN_REQ_SHIFT)   /* 0x00008000 */
/* SOFTRST_CON3 */
#define CRU_SOFTRST_CON3_OFFSET                            (0x30C)
#define CRU_SOFTRST_CON3_PCIE_NIU_HSSRSTN_REQ_SHIFT        (0U)
#define CRU_SOFTRST_CON3_PCIE_NIU_HSSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON3_PCIE_NIU_HSSRSTN_REQ_SHIFT)        /* 0x00000001 */
#define CRU_SOFTRST_CON3_PCIE_NIU_LSSRSTN_REQ_SHIFT        (1U)
#define CRU_SOFTRST_CON3_PCIE_NIU_LSSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON3_PCIE_NIU_LSSRSTN_REQ_SHIFT)        /* 0x00000002 */
#define CRU_SOFTRST_CON3_PCIEGRF_PSRSTN_REQ_SHIFT          (2U)
#define CRU_SOFTRST_CON3_PCIEGRF_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON3_PCIEGRF_PSRSTN_REQ_SHIFT)          /* 0x00000004 */
#define CRU_SOFTRST_CON3_PCIECTL_PSRSTN_REQ_SHIFT          (3U)
#define CRU_SOFTRST_CON3_PCIECTL_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON3_PCIECTL_PSRSTN_REQ_SHIFT)          /* 0x00000008 */
#define CRU_SOFTRST_CON3_PCIECTL_POWERUP_SRSTN_REQ_SHIFT   (4U)
#define CRU_SOFTRST_CON3_PCIECTL_POWERUP_SRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON3_PCIECTL_POWERUP_SRSTN_REQ_SHIFT)   /* 0x00000010 */
#define CRU_SOFTRST_CON3_PCIECTL_MST_ASRSTN_REQ_SHIFT      (5U)
#define CRU_SOFTRST_CON3_PCIECTL_MST_ASRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON3_PCIECTL_MST_ASRSTN_REQ_SHIFT)      /* 0x00000020 */
#define CRU_SOFTRST_CON3_PCIECTL_SLV_ASRSTN_REQ_SHIFT      (6U)
#define CRU_SOFTRST_CON3_PCIECTL_SLV_ASRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON3_PCIECTL_SLV_ASRSTN_REQ_SHIFT)      /* 0x00000040 */
#define CRU_SOFTRST_CON3_PCIECTL_DBI_ASRSTN_REQ_SHIFT      (7U)
#define CRU_SOFTRST_CON3_PCIECTL_DBI_ASRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON3_PCIECTL_DBI_ASRSTN_REQ_SHIFT)      /* 0x00000080 */
#define CRU_SOFTRST_CON3_PCIECTL_BUTTON_SRSTN_REQ_SHIFT    (8U)
#define CRU_SOFTRST_CON3_PCIECTL_BUTTON_SRSTN_REQ_MASK     (0x1U << CRU_SOFTRST_CON3_PCIECTL_BUTTON_SRSTN_REQ_SHIFT)    /* 0x00000100 */
#define CRU_SOFTRST_CON3_PCIECTL_PE_SRSTN_REQ_SHIFT        (9U)
#define CRU_SOFTRST_CON3_PCIECTL_PE_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON3_PCIECTL_PE_SRSTN_REQ_SHIFT)        /* 0x00000200 */
#define CRU_SOFTRST_CON3_PCIECTL_CORE_SRSTN_REQ_SHIFT      (10U)
#define CRU_SOFTRST_CON3_PCIECTL_CORE_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON3_PCIECTL_CORE_SRSTN_REQ_SHIFT)      /* 0x00000400 */
#define CRU_SOFTRST_CON3_PCIECTL_NSTICKY_SRSTN_REQ_SHIFT   (11U)
#define CRU_SOFTRST_CON3_PCIECTL_NSTICKY_SRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON3_PCIECTL_NSTICKY_SRSTN_REQ_SHIFT)   /* 0x00000800 */
#define CRU_SOFTRST_CON3_PCIECTL_STICKY_SRSTN_REQ_SHIFT    (12U)
#define CRU_SOFTRST_CON3_PCIECTL_STICKY_SRSTN_REQ_MASK     (0x1U << CRU_SOFTRST_CON3_PCIECTL_STICKY_SRSTN_REQ_SHIFT)    /* 0x00001000 */
#define CRU_SOFTRST_CON3_PCIECTL_PWR_SRSTN_REQ_SHIFT       (13U)
#define CRU_SOFTRST_CON3_PCIECTL_PWR_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON3_PCIECTL_PWR_SRSTN_REQ_SHIFT)       /* 0x00002000 */
#define CRU_SOFTRST_CON3_PCIE_NIU_ASRSTN_REQ_SHIFT         (14U)
#define CRU_SOFTRST_CON3_PCIE_NIU_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON3_PCIE_NIU_ASRSTN_REQ_SHIFT)         /* 0x00004000 */
#define CRU_SOFTRST_CON3_PCIE_NIU_PSRSTN_REQ_SHIFT         (15U)
#define CRU_SOFTRST_CON3_PCIE_NIU_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON3_PCIE_NIU_PSRSTN_REQ_SHIFT)         /* 0x00008000 */
/* SOFTRST_CON4 */
#define CRU_SOFTRST_CON4_OFFSET                            (0x310)
#define CRU_SOFTRST_CON4_PCIEPHY_POR_SRSTN_REQ_SHIFT       (0U)
#define CRU_SOFTRST_CON4_PCIEPHY_POR_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON4_PCIEPHY_POR_SRSTN_REQ_SHIFT)       /* 0x00000001 */
#define CRU_SOFTRST_CON4_PCIEPHY_PSRSTN_REQ_SHIFT          (1U)
#define CRU_SOFTRST_CON4_PCIEPHY_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON4_PCIEPHY_PSRSTN_REQ_SHIFT)          /* 0x00000002 */
#define CRU_SOFTRST_CON4_PCIEPHY_PIPE_SRSTN_REQ_SHIFT      (2U)
#define CRU_SOFTRST_CON4_PCIEPHY_PIPE_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON4_PCIEPHY_PIPE_SRSTN_REQ_SHIFT)      /* 0x00000004 */
#define CRU_SOFTRST_CON4_USBPHY_POR_SRSTN_REQ_SHIFT        (3U)
#define CRU_SOFTRST_CON4_USBPHY_POR_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON4_USBPHY_POR_SRSTN_REQ_SHIFT)        /* 0x00000008 */
#define CRU_SOFTRST_CON4_USBPHY_OTG_PORT_SRSTN_REQ_SHIFT   (4U)
#define CRU_SOFTRST_CON4_USBPHY_OTG_PORT_SRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON4_USBPHY_OTG_PORT_SRSTN_REQ_SHIFT)   /* 0x00000010 */
#define CRU_SOFTRST_CON4_USBPHY_HOST_PORT_SRSTN_REQ_SHIFT  (5U)
#define CRU_SOFTRST_CON4_USBPHY_HOST_PORT_SRSTN_REQ_MASK   (0x1U << CRU_SOFTRST_CON4_USBPHY_HOST_PORT_SRSTN_REQ_SHIFT)  /* 0x00000020 */
#define CRU_SOFTRST_CON4_USB3PHY_GRF_PSRSTN_REQ_SHIFT      (6U)
#define CRU_SOFTRST_CON4_USB3PHY_GRF_PSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON4_USB3PHY_GRF_PSRSTN_REQ_SHIFT)      /* 0x00000040 */
#define CRU_SOFTRST_CON4_USB2PHY_GRF_PSRSTN_REQ_SHIFT      (7U)
#define CRU_SOFTRST_CON4_USB2PHY_GRF_PSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON4_USB2PHY_GRF_PSRSTN_REQ_SHIFT)      /* 0x00000080 */
#define CRU_SOFTRST_CON4_USB3_OTG_ASRSTN_REQ_SHIFT         (8U)
#define CRU_SOFTRST_CON4_USB3_OTG_ASRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON4_USB3_OTG_ASRSTN_REQ_SHIFT)         /* 0x00000100 */
#define CRU_SOFTRST_CON4_USB2HOST_HSRSTN_REQ_SHIFT         (9U)
#define CRU_SOFTRST_CON4_USB2HOST_HSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON4_USB2HOST_HSRSTN_REQ_SHIFT)         /* 0x00000200 */
#define CRU_SOFTRST_CON4_USB2HOST_ARB_HSRSTN_REQ_SHIFT     (10U)
#define CRU_SOFTRST_CON4_USB2HOST_ARB_HSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON4_USB2HOST_ARB_HSRSTN_REQ_SHIFT)     /* 0x00000400 */
#define CRU_SOFTRST_CON4_USB2HOST_UTMI_SRSTN_REQ_SHIFT     (11U)
#define CRU_SOFTRST_CON4_USB2HOST_UTMI_SRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON4_USB2HOST_UTMI_SRSTN_REQ_SHIFT)     /* 0x00000800 */
/* SOFTRST_CON5 */
#define CRU_SOFTRST_CON5_OFFSET                            (0x314)
#define CRU_SOFTRST_CON5_IMEM0_ASRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST_CON5_IMEM0_ASRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON5_IMEM0_ASRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST_CON5_IMEM1_ASRSTN_REQ_SHIFT            (1U)
#define CRU_SOFTRST_CON5_IMEM1_ASRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON5_IMEM1_ASRSTN_REQ_SHIFT)            /* 0x00000002 */
#define CRU_SOFTRST_CON5_IMEM2_ASRSTN_REQ_SHIFT            (2U)
#define CRU_SOFTRST_CON5_IMEM2_ASRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON5_IMEM2_ASRSTN_REQ_SHIFT)            /* 0x00000004 */
#define CRU_SOFTRST_CON5_IMEM3_ASRSTN_REQ_SHIFT            (3U)
#define CRU_SOFTRST_CON5_IMEM3_ASRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON5_IMEM3_ASRSTN_REQ_SHIFT)            /* 0x00000008 */
#define CRU_SOFTRST_CON5_IMEM0_NIU_ASRSTN_REQ_SHIFT        (4U)
#define CRU_SOFTRST_CON5_IMEM0_NIU_ASRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON5_IMEM0_NIU_ASRSTN_REQ_SHIFT)        /* 0x00000010 */
#define CRU_SOFTRST_CON5_IMEM1_NIU_ASRSTN_REQ_SHIFT        (5U)
#define CRU_SOFTRST_CON5_IMEM1_NIU_ASRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON5_IMEM1_NIU_ASRSTN_REQ_SHIFT)        /* 0x00000020 */
#define CRU_SOFTRST_CON5_IMEM2_NIU_ASRSTN_REQ_SHIFT        (6U)
#define CRU_SOFTRST_CON5_IMEM2_NIU_ASRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON5_IMEM2_NIU_ASRSTN_REQ_SHIFT)        /* 0x00000040 */
#define CRU_SOFTRST_CON5_IMEM3_NIU_ASRSTN_REQ_SHIFT        (7U)
#define CRU_SOFTRST_CON5_IMEM3_NIU_ASRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON5_IMEM3_NIU_ASRSTN_REQ_SHIFT)        /* 0x00000080 */
#define CRU_SOFTRST_CON5_IMEM_NIU_HSSRSTN_REQ_SHIFT        (8U)
#define CRU_SOFTRST_CON5_IMEM_NIU_HSSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON5_IMEM_NIU_HSSRSTN_REQ_SHIFT)        /* 0x00000100 */
#define CRU_SOFTRST_CON5_VPU_NIU_ASRSTN_REQ_SHIFT          (12U)
#define CRU_SOFTRST_CON5_VPU_NIU_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON5_VPU_NIU_ASRSTN_REQ_SHIFT)          /* 0x00001000 */
#define CRU_SOFTRST_CON5_VPU_NIU_HSRSTN_REQ_SHIFT          (13U)
#define CRU_SOFTRST_CON5_VPU_NIU_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON5_VPU_NIU_HSRSTN_REQ_SHIFT)          /* 0x00002000 */
#define CRU_SOFTRST_CON5_VPU_ASRSTN_REQ_SHIFT              (14U)
#define CRU_SOFTRST_CON5_VPU_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON5_VPU_ASRSTN_REQ_SHIFT)              /* 0x00004000 */
#define CRU_SOFTRST_CON5_VPU_HSRSTN_REQ_SHIFT              (15U)
#define CRU_SOFTRST_CON5_VPU_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON5_VPU_HSRSTN_REQ_SHIFT)              /* 0x00008000 */
/* SOFTRST_CON6 */
#define CRU_SOFTRST_CON6_OFFSET                            (0x318)
#define CRU_SOFTRST_CON6_VIO_NIU_HSSRSTN_REQ_SHIFT         (0U)
#define CRU_SOFTRST_CON6_VIO_NIU_HSSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON6_VIO_NIU_HSSRSTN_REQ_SHIFT)         /* 0x00000001 */
#define CRU_SOFTRST_CON6_VIO_NIU_LSSRSTN_REQ_SHIFT         (1U)
#define CRU_SOFTRST_CON6_VIO_NIU_LSSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON6_VIO_NIU_LSSRSTN_REQ_SHIFT)         /* 0x00000002 */
#define CRU_SOFTRST_CON6_VOPB_ASRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST_CON6_VOPB_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON6_VOPB_ASRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON6_VOPB_HSRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON6_VOPB_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON6_VOPB_HSRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON6_VOPB_DRSTN_REQ_SHIFT              (4U)
#define CRU_SOFTRST_CON6_VOPB_DRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON6_VOPB_DRSTN_REQ_SHIFT)              /* 0x00000010 */
#define CRU_SOFTRST_CON6_VOPL_ASRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST_CON6_VOPL_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON6_VOPL_ASRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST_CON6_VOPL_HSRSTN_REQ_SHIFT             (6U)
#define CRU_SOFTRST_CON6_VOPL_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON6_VOPL_HSRSTN_REQ_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST_CON6_VOPL_DRSTN_REQ_SHIFT              (7U)
#define CRU_SOFTRST_CON6_VOPL_DRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON6_VOPL_DRSTN_REQ_SHIFT)              /* 0x00000080 */
#define CRU_SOFTRST_CON6_MIPIDSI_HOST_PSRSTN_REQ_SHIFT     (8U)
#define CRU_SOFTRST_CON6_MIPIDSI_HOST_PSRSTN_REQ_MASK      (0x1U << CRU_SOFTRST_CON6_MIPIDSI_HOST_PSRSTN_REQ_SHIFT)     /* 0x00000100 */
#define CRU_SOFTRST_CON6_CSITX_PSRSTN_REQ_SHIFT            (9U)
#define CRU_SOFTRST_CON6_CSITX_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON6_CSITX_PSRSTN_REQ_SHIFT)            /* 0x00000200 */
#define CRU_SOFTRST_CON6_CSITX_TXBYTEHS_SRSTN_REQ_SHIFT    (10U)
#define CRU_SOFTRST_CON6_CSITX_TXBYTEHS_SRSTN_REQ_MASK     (0x1U << CRU_SOFTRST_CON6_CSITX_TXBYTEHS_SRSTN_REQ_SHIFT)    /* 0x00000400 */
#define CRU_SOFTRST_CON6_CSITX_TXESC_SRSTN_REQ_SHIFT       (11U)
#define CRU_SOFTRST_CON6_CSITX_TXESC_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON6_CSITX_TXESC_SRSTN_REQ_SHIFT)       /* 0x00000800 */
#define CRU_SOFTRST_CON6_CSITX_CAM_SRSTN_REQ_SHIFT         (12U)
#define CRU_SOFTRST_CON6_CSITX_CAM_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON6_CSITX_CAM_SRSTN_REQ_SHIFT)         /* 0x00001000 */
#define CRU_SOFTRST_CON6_CSITX_ISRSTN_REQ_SHIFT            (13U)
#define CRU_SOFTRST_CON6_CSITX_ISRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON6_CSITX_ISRSTN_REQ_SHIFT)            /* 0x00002000 */
/* SOFTRST_CON7 */
#define CRU_SOFTRST_CON7_OFFSET                            (0x31C)
#define CRU_SOFTRST_CON7_RGA_ASRSTN_REQ_SHIFT              (0U)
#define CRU_SOFTRST_CON7_RGA_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_RGA_ASRSTN_REQ_SHIFT)              /* 0x00000001 */
#define CRU_SOFTRST_CON7_RGA_HSRSTN_REQ_SHIFT              (1U)
#define CRU_SOFTRST_CON7_RGA_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_RGA_HSRSTN_REQ_SHIFT)              /* 0x00000002 */
#define CRU_SOFTRST_CON7_RGA_SRSTN_REQ_SHIFT               (2U)
#define CRU_SOFTRST_CON7_RGA_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON7_RGA_SRSTN_REQ_SHIFT)               /* 0x00000004 */
#define CRU_SOFTRST_CON7_CSIRX_PSRSTN_REQ_SHIFT            (3U)
#define CRU_SOFTRST_CON7_CSIRX_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON7_CSIRX_PSRSTN_REQ_SHIFT)            /* 0x00000008 */
#define CRU_SOFTRST_CON7_CIF_ASRSTN_REQ_SHIFT              (4U)
#define CRU_SOFTRST_CON7_CIF_ASRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_CIF_ASRSTN_REQ_SHIFT)              /* 0x00000010 */
#define CRU_SOFTRST_CON7_CIF_HSRSTN_REQ_SHIFT              (5U)
#define CRU_SOFTRST_CON7_CIF_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_CIF_HSRSTN_REQ_SHIFT)              /* 0x00000020 */
#define CRU_SOFTRST_CON7_CIF_ISRSTN_REQ_SHIFT              (6U)
#define CRU_SOFTRST_CON7_CIF_ISRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_CIF_ISRSTN_REQ_SHIFT)              /* 0x00000040 */
#define CRU_SOFTRST_CON7_CIF_PCLKIN_SRSTN_REQ_SHIFT        (7U)
#define CRU_SOFTRST_CON7_CIF_PCLKIN_SRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON7_CIF_PCLKIN_SRSTN_REQ_SHIFT)        /* 0x00000080 */
#define CRU_SOFTRST_CON7_CIF_DSRSTN_REQ_SHIFT              (8U)
#define CRU_SOFTRST_CON7_CIF_DSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_CIF_DSRSTN_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST_CON7_ISP_HSRSTN_REQ_SHIFT              (9U)
#define CRU_SOFTRST_CON7_ISP_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON7_ISP_HSRSTN_REQ_SHIFT)              /* 0x00000200 */
#define CRU_SOFTRST_CON7_ISP_SRSTN_REQ_SHIFT               (10U)
#define CRU_SOFTRST_CON7_ISP_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON7_ISP_SRSTN_REQ_SHIFT)               /* 0x00000400 */
#define CRU_SOFTRST_CON7_MIPICSIPHY_PSRSTN_REQ_SHIFT       (12U)
#define CRU_SOFTRST_CON7_MIPICSIPHY_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON7_MIPICSIPHY_PSRSTN_REQ_SHIFT)       /* 0x00001000 */
#define CRU_SOFTRST_CON7_MIPIDSIPHY_PSRSTN_REQ_SHIFT       (13U)
#define CRU_SOFTRST_CON7_MIPIDSIPHY_PSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON7_MIPIDSIPHY_PSRSTN_REQ_SHIFT)       /* 0x00002000 */
/* SOFTRST_CON8 */
#define CRU_SOFTRST_CON8_OFFSET                            (0x320)
#define CRU_SOFTRST_CON8_PDPERI_NIU_MSSRSTN_REQ_SHIFT      (0U)
#define CRU_SOFTRST_CON8_PDPERI_NIU_MSSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON8_PDPERI_NIU_MSSRSTN_REQ_SHIFT)      /* 0x00000001 */
#define CRU_SOFTRST_CON8_PDPERI_NIU_LSSRSTN_REQ_SHIFT      (1U)
#define CRU_SOFTRST_CON8_PDPERI_NIU_LSSRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON8_PDPERI_NIU_LSSRSTN_REQ_SHIFT)      /* 0x00000002 */
#define CRU_SOFTRST_CON8_PD_MMC_SFC_NIU_HSRSTN_REQ_SHIFT   (4U)
#define CRU_SOFTRST_CON8_PD_MMC_SFC_NIU_HSRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON8_PD_MMC_SFC_NIU_HSRSTN_REQ_SHIFT)   /* 0x00000010 */
#define CRU_SOFTRST_CON8_SDMMC_HSRSTN_REQ_SHIFT            (5U)
#define CRU_SOFTRST_CON8_SDMMC_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON8_SDMMC_HSRSTN_REQ_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST_CON8_SDIO_HSRSTN_REQ_SHIFT             (6U)
#define CRU_SOFTRST_CON8_SDIO_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_SDIO_HSRSTN_REQ_SHIFT)             /* 0x00000040 */
#define CRU_SOFTRST_CON8_EMMC_HSRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON8_EMMC_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_EMMC_HSRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON8_SFC_HSRSTN_REQ_SHIFT              (8U)
#define CRU_SOFTRST_CON8_SFC_HSRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON8_SFC_HSRSTN_REQ_SHIFT)              /* 0x00000100 */
#define CRU_SOFTRST_CON8_SFC_SRSTN_REQ_SHIFT               (9U)
#define CRU_SOFTRST_CON8_SFC_SRSTN_REQ_MASK                (0x1U << CRU_SOFTRST_CON8_SFC_SRSTN_REQ_SHIFT)               /* 0x00000200 */
#define CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_ASRSTN_REQ_SHIFT   (12U)
#define CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_ASRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_ASRSTN_REQ_SHIFT)   /* 0x00001000 */
#define CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_HSRSTN_REQ_SHIFT   (13U)
#define CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_HSRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_HSRSTN_REQ_SHIFT)   /* 0x00002000 */
#define CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_PSRSTN_REQ_SHIFT   (14U)
#define CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_PSRSTN_REQ_MASK    (0x1U << CRU_SOFTRST_CON8_PD_SD_GMAC_NIU_PSRSTN_REQ_SHIFT)   /* 0x00004000 */
#define CRU_SOFTRST_CON8_GMAC_ASRSTN_REQ_SHIFT             (15U)
#define CRU_SOFTRST_CON8_GMAC_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON8_GMAC_ASRSTN_REQ_SHIFT)             /* 0x00008000 */
/* SOFTRST_CON9 */
#define CRU_SOFTRST_CON9_OFFSET                            (0x324)
#define CRU_SOFTRST_CON9_PMU_NIU_PSRSTN_REQ_SHIFT          (0U)
#define CRU_SOFTRST_CON9_PMU_NIU_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON9_PMU_NIU_PSRSTN_REQ_SHIFT)          /* 0x00000001 */
#define CRU_SOFTRST_CON9_PMU_GRF_PSRSTN_REQ_SHIFT          (2U)
#define CRU_SOFTRST_CON9_PMU_GRF_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON9_PMU_GRF_PSRSTN_REQ_SHIFT)          /* 0x00000004 */
#define CRU_SOFTRST_CON9_PMU_PMU_SRSTN_REQ_SHIFT           (3U)
#define CRU_SOFTRST_CON9_PMU_PMU_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON9_PMU_PMU_SRSTN_REQ_SHIFT)           /* 0x00000008 */
#define CRU_SOFTRST_CON9_PMU_MEM_PSRSTN_REQ_SHIFT          (4U)
#define CRU_SOFTRST_CON9_PMU_MEM_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON9_PMU_MEM_PSRSTN_REQ_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST_CON9_PMU_GPIO0_PSRSTN_REQ_SHIFT        (5U)
#define CRU_SOFTRST_CON9_PMU_GPIO0_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON9_PMU_GPIO0_PSRSTN_REQ_SHIFT)        /* 0x00000020 */
#define CRU_SOFTRST_CON9_PMU_UART0_PSRSTN_REQ_SHIFT        (6U)
#define CRU_SOFTRST_CON9_PMU_UART0_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON9_PMU_UART0_PSRSTN_REQ_SHIFT)        /* 0x00000040 */
#define CRU_SOFTRST_CON9_PMU_PVTM_SRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST_CON9_PMU_PVTM_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON9_PMU_PVTM_SRSTN_REQ_SHIFT)          /* 0x00000100 */
#define CRU_SOFTRST_CON9_PMU_UART0_SRSTN_REQ_SHIFT         (9U)
#define CRU_SOFTRST_CON9_PMU_UART0_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON9_PMU_UART0_SRSTN_REQ_SHIFT)         /* 0x00000200 */
#define CRU_SOFTRST_CON9_PMU_DDR_FAIL_SAVE_SRSTN_REQ_SHIFT (11U)
#define CRU_SOFTRST_CON9_PMU_DDR_FAIL_SAVE_SRSTN_REQ_MASK  (0x1U << CRU_SOFTRST_CON9_PMU_DDR_FAIL_SAVE_SRSTN_REQ_SHIFT) /* 0x00000800 */
#define CRU_SOFTRST_CON9_PMU_I2C0_PSRSTN_REQ_SHIFT         (12U)
#define CRU_SOFTRST_CON9_PMU_I2C0_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON9_PMU_I2C0_PSRSTN_REQ_SHIFT)         /* 0x00001000 */
#define CRU_SOFTRST_CON9_PMU_I2C0_SRSTN_REQ_SHIFT          (13U)
#define CRU_SOFTRST_CON9_PMU_I2C0_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON9_PMU_I2C0_SRSTN_REQ_SHIFT)          /* 0x00002000 */
#define CRU_SOFTRST_CON9_PMU_GPIO0_DBSRSTN_REQ_SHIFT       (14U)
#define CRU_SOFTRST_CON9_PMU_GPIO0_DBSRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON9_PMU_GPIO0_DBSRSTN_REQ_SHIFT)       /* 0x00004000 */
/* SOFTRST_CON10 */
#define CRU_SOFTRST_CON10_OFFSET                           (0x328)
#define CRU_SOFTRST_CON10_PD_AUDIO_NIU_HSRSTN_REQ_SHIFT    (0U)
#define CRU_SOFTRST_CON10_PD_AUDIO_NIU_HSRSTN_REQ_MASK     (0x1U << CRU_SOFTRST_CON10_PD_AUDIO_NIU_HSRSTN_REQ_SHIFT)    /* 0x00000001 */
#define CRU_SOFTRST_CON10_VAD_HSRSTN_REQ_SHIFT             (1U)
#define CRU_SOFTRST_CON10_VAD_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON10_VAD_HSRSTN_REQ_SHIFT)             /* 0x00000002 */
#define CRU_SOFTRST_CON10_PDM_HSRSTN_REQ_SHIFT             (2U)
#define CRU_SOFTRST_CON10_PDM_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON10_PDM_HSRSTN_REQ_SHIFT)             /* 0x00000004 */
#define CRU_SOFTRST_CON10_PDM_SRSTN_REQ_SHIFT              (3U)
#define CRU_SOFTRST_CON10_PDM_SRSTN_REQ_MASK               (0x1U << CRU_SOFTRST_CON10_PDM_SRSTN_REQ_SHIFT)              /* 0x00000008 */
#define CRU_SOFTRST_CON10_I2S0_HSRSTN_REQ_SHIFT            (4U)
#define CRU_SOFTRST_CON10_I2S0_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON10_I2S0_HSRSTN_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST_CON10_I2S0_TX_SRSTN_REQ_SHIFT          (5U)
#define CRU_SOFTRST_CON10_I2S0_TX_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON10_I2S0_TX_SRSTN_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST_CON10_I2S1_HSRSTN_REQ_SHIFT            (6U)
#define CRU_SOFTRST_CON10_I2S1_HSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON10_I2S1_HSRSTN_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST_CON10_I2S1_SRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON10_I2S1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON10_I2S1_SRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON10_I2S0_RX_SRSTN_REQ_SHIFT          (8U)
#define CRU_SOFTRST_CON10_I2S0_RX_SRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON10_I2S0_RX_SRSTN_REQ_SHIFT)          /* 0x00000100 */
/* SOFTRST_CON11 */
#define CRU_SOFTRST_CON11_OFFSET                           (0x32C)
#define CRU_SOFTRST_CON11_BUS_NIU_MSSRSTN_REQ_SHIFT        (0U)
#define CRU_SOFTRST_CON11_BUS_NIU_MSSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON11_BUS_NIU_MSSRSTN_REQ_SHIFT)        /* 0x00000001 */
#define CRU_SOFTRST_CON11_BUS_NIU_LSSRSTN_REQ_SHIFT        (1U)
#define CRU_SOFTRST_CON11_BUS_NIU_LSSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON11_BUS_NIU_LSSRSTN_REQ_SHIFT)        /* 0x00000002 */
#define CRU_SOFTRST_CON11_TOP_NIU_PSRSTN_REQ_SHIFT         (2U)
#define CRU_SOFTRST_CON11_TOP_NIU_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON11_TOP_NIU_PSRSTN_REQ_SHIFT)         /* 0x00000004 */
#define CRU_SOFTRST_CON11_ROM_HSRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON11_ROM_HSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON11_ROM_HSRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON11_CRYPTO_ASRSTN_REQ_SHIFT          (4U)
#define CRU_SOFTRST_CON11_CRYPTO_ASRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON11_CRYPTO_ASRSTN_REQ_SHIFT)          /* 0x00000010 */
#define CRU_SOFTRST_CON11_CRYPTO_HSRSTN_REQ_SHIFT          (5U)
#define CRU_SOFTRST_CON11_CRYPTO_HSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON11_CRYPTO_HSRSTN_REQ_SHIFT)          /* 0x00000020 */
#define CRU_SOFTRST_CON11_CRYPTO_CORE_SRSTN_REQ_SHIFT      (6U)
#define CRU_SOFTRST_CON11_CRYPTO_CORE_SRSTN_REQ_MASK       (0x1U << CRU_SOFTRST_CON11_CRYPTO_CORE_SRSTN_REQ_SHIFT)      /* 0x00000040 */
#define CRU_SOFTRST_CON11_CRYPTO_APK_SRSTN_REQ_SHIFT       (7U)
#define CRU_SOFTRST_CON11_CRYPTO_APK_SRSTN_REQ_MASK        (0x1U << CRU_SOFTRST_CON11_CRYPTO_APK_SRSTN_REQ_SHIFT)       /* 0x00000080 */
#define CRU_SOFTRST_CON11_DCF_ASRSTN_REQ_SHIFT             (8U)
#define CRU_SOFTRST_CON11_DCF_ASRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON11_DCF_ASRSTN_REQ_SHIFT)             /* 0x00000100 */
#define CRU_SOFTRST_CON11_DCF_PSRSTN_REQ_SHIFT             (9U)
#define CRU_SOFTRST_CON11_DCF_PSRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON11_DCF_PSRSTN_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST_CON11_UART1_PSRSTN_REQ_SHIFT           (10U)
#define CRU_SOFTRST_CON11_UART1_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON11_UART1_PSRSTN_REQ_SHIFT)           /* 0x00000400 */
#define CRU_SOFTRST_CON11_UART1_SRSTN_REQ_SHIFT            (11U)
#define CRU_SOFTRST_CON11_UART1_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON11_UART1_SRSTN_REQ_SHIFT)            /* 0x00000800 */
#define CRU_SOFTRST_CON11_UART2_PSRSTN_REQ_SHIFT           (12U)
#define CRU_SOFTRST_CON11_UART2_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON11_UART2_PSRSTN_REQ_SHIFT)           /* 0x00001000 */
#define CRU_SOFTRST_CON11_UART2_SRSTN_REQ_SHIFT            (13U)
#define CRU_SOFTRST_CON11_UART2_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON11_UART2_SRSTN_REQ_SHIFT)            /* 0x00002000 */
#define CRU_SOFTRST_CON11_UART3_PSRSTN_REQ_SHIFT           (14U)
#define CRU_SOFTRST_CON11_UART3_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON11_UART3_PSRSTN_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST_CON11_UART3_SRSTN_REQ_SHIFT            (15U)
#define CRU_SOFTRST_CON11_UART3_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON11_UART3_SRSTN_REQ_SHIFT)            /* 0x00008000 */
/* SOFTRST_CON12 */
#define CRU_SOFTRST_CON12_OFFSET                           (0x330)
#define CRU_SOFTRST_CON12_UART4_PSRSTN_REQ_SHIFT           (0U)
#define CRU_SOFTRST_CON12_UART4_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON12_UART4_PSRSTN_REQ_SHIFT)           /* 0x00000001 */
#define CRU_SOFTRST_CON12_UART4_SRSTN_REQ_SHIFT            (1U)
#define CRU_SOFTRST_CON12_UART4_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_UART4_SRSTN_REQ_SHIFT)            /* 0x00000002 */
#define CRU_SOFTRST_CON12_UART5_PSRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON12_UART5_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON12_UART5_PSRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST_CON12_UART5_SRSTN_REQ_SHIFT            (3U)
#define CRU_SOFTRST_CON12_UART5_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_UART5_SRSTN_REQ_SHIFT)            /* 0x00000008 */
#define CRU_SOFTRST_CON12_UART6_PSRSTN_REQ_SHIFT           (4U)
#define CRU_SOFTRST_CON12_UART6_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON12_UART6_PSRSTN_REQ_SHIFT)           /* 0x00000010 */
#define CRU_SOFTRST_CON12_UART6_SRSTN_REQ_SHIFT            (5U)
#define CRU_SOFTRST_CON12_UART6_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_UART6_SRSTN_REQ_SHIFT)            /* 0x00000020 */
#define CRU_SOFTRST_CON12_UART7_PSRSTN_REQ_SHIFT           (6U)
#define CRU_SOFTRST_CON12_UART7_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON12_UART7_PSRSTN_REQ_SHIFT)           /* 0x00000040 */
#define CRU_SOFTRST_CON12_UART7_SRSTN_REQ_SHIFT            (7U)
#define CRU_SOFTRST_CON12_UART7_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_UART7_SRSTN_REQ_SHIFT)            /* 0x00000080 */
#define CRU_SOFTRST_CON12_I2C1_PSRSTN_REQ_SHIFT            (8U)
#define CRU_SOFTRST_CON12_I2C1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_I2C1_PSRSTN_REQ_SHIFT)            /* 0x00000100 */
#define CRU_SOFTRST_CON12_I2C1_SRSTN_REQ_SHIFT             (9U)
#define CRU_SOFTRST_CON12_I2C1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON12_I2C1_SRSTN_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST_CON12_I2C2_PSRSTN_REQ_SHIFT            (10U)
#define CRU_SOFTRST_CON12_I2C2_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_I2C2_PSRSTN_REQ_SHIFT)            /* 0x00000400 */
#define CRU_SOFTRST_CON12_I2C2_SRSTN_REQ_SHIFT             (11U)
#define CRU_SOFTRST_CON12_I2C2_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON12_I2C2_SRSTN_REQ_SHIFT)             /* 0x00000800 */
#define CRU_SOFTRST_CON12_I2C3_PSRSTN_REQ_SHIFT            (12U)
#define CRU_SOFTRST_CON12_I2C3_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_I2C3_PSRSTN_REQ_SHIFT)            /* 0x00001000 */
#define CRU_SOFTRST_CON12_I2C3_SRSTN_REQ_SHIFT             (13U)
#define CRU_SOFTRST_CON12_I2C3_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON12_I2C3_SRSTN_REQ_SHIFT)             /* 0x00002000 */
#define CRU_SOFTRST_CON12_PWM0_PSRSTN_REQ_SHIFT            (14U)
#define CRU_SOFTRST_CON12_PWM0_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON12_PWM0_PSRSTN_REQ_SHIFT)            /* 0x00004000 */
#define CRU_SOFTRST_CON12_PWM0_SRSTN_REQ_SHIFT             (15U)
#define CRU_SOFTRST_CON12_PWM0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON12_PWM0_SRSTN_REQ_SHIFT)             /* 0x00008000 */
/* SOFTRST_CON13 */
#define CRU_SOFTRST_CON13_OFFSET                           (0x334)
#define CRU_SOFTRST_CON13_PWM1_PSRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST_CON13_PWM1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON13_PWM1_PSRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST_CON13_PWM1_SRSTN_REQ_SHIFT             (1U)
#define CRU_SOFTRST_CON13_PWM1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON13_PWM1_SRSTN_REQ_SHIFT)             /* 0x00000002 */
#define CRU_SOFTRST_CON13_PWM2_PSRSTN_REQ_SHIFT            (2U)
#define CRU_SOFTRST_CON13_PWM2_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON13_PWM2_PSRSTN_REQ_SHIFT)            /* 0x00000004 */
#define CRU_SOFTRST_CON13_PWM2_SRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON13_PWM2_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON13_PWM2_SRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON13_SPI0_PSRSTN_REQ_SHIFT            (4U)
#define CRU_SOFTRST_CON13_SPI0_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON13_SPI0_PSRSTN_REQ_SHIFT)            /* 0x00000010 */
#define CRU_SOFTRST_CON13_SPI0_SRSTN_REQ_SHIFT             (5U)
#define CRU_SOFTRST_CON13_SPI0_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON13_SPI0_SRSTN_REQ_SHIFT)             /* 0x00000020 */
#define CRU_SOFTRST_CON13_SPI1_PSRSTN_REQ_SHIFT            (6U)
#define CRU_SOFTRST_CON13_SPI1_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON13_SPI1_PSRSTN_REQ_SHIFT)            /* 0x00000040 */
#define CRU_SOFTRST_CON13_SPI1_SRSTN_REQ_SHIFT             (7U)
#define CRU_SOFTRST_CON13_SPI1_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON13_SPI1_SRSTN_REQ_SHIFT)             /* 0x00000080 */
#define CRU_SOFTRST_CON13_SPI2_PSRSTN_REQ_SHIFT            (8U)
#define CRU_SOFTRST_CON13_SPI2_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON13_SPI2_PSRSTN_REQ_SHIFT)            /* 0x00000100 */
#define CRU_SOFTRST_CON13_SPI2_SRSTN_REQ_SHIFT             (9U)
#define CRU_SOFTRST_CON13_SPI2_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON13_SPI2_SRSTN_REQ_SHIFT)             /* 0x00000200 */
#define CRU_SOFTRST_CON13_BUS_GRF_PSRSTN_REQ_SHIFT         (11U)
#define CRU_SOFTRST_CON13_BUS_GRF_PSRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON13_BUS_GRF_PSRSTN_REQ_SHIFT)         /* 0x00000800 */
#define CRU_SOFTRST_CON13_TIMER_PSRSTN_REQ_SHIFT           (12U)
#define CRU_SOFTRST_CON13_TIMER_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON13_TIMER_PSRSTN_REQ_SHIFT)           /* 0x00001000 */
#define CRU_SOFTRST_CON13_TIMER0_SRSTN_REQ_SHIFT           (13U)
#define CRU_SOFTRST_CON13_TIMER0_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON13_TIMER0_SRSTN_REQ_SHIFT)           /* 0x00002000 */
#define CRU_SOFTRST_CON13_TIMER1_SRSTN_REQ_SHIFT           (14U)
#define CRU_SOFTRST_CON13_TIMER1_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON13_TIMER1_SRSTN_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST_CON13_TIMER2_SRSTN_REQ_SHIFT           (15U)
#define CRU_SOFTRST_CON13_TIMER2_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON13_TIMER2_SRSTN_REQ_SHIFT)           /* 0x00008000 */
/* SOFTRST_CON14 */
#define CRU_SOFTRST_CON14_OFFSET                           (0x338)
#define CRU_SOFTRST_CON14_TIMER3_SRSTN_REQ_SHIFT           (0U)
#define CRU_SOFTRST_CON14_TIMER3_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_TIMER3_SRSTN_REQ_SHIFT)           /* 0x00000001 */
#define CRU_SOFTRST_CON14_TIMER4_SRSTN_REQ_SHIFT           (1U)
#define CRU_SOFTRST_CON14_TIMER4_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_TIMER4_SRSTN_REQ_SHIFT)           /* 0x00000002 */
#define CRU_SOFTRST_CON14_TIMER5_SRSTN_REQ_SHIFT           (2U)
#define CRU_SOFTRST_CON14_TIMER5_SRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_TIMER5_SRSTN_REQ_SHIFT)           /* 0x00000004 */
#define CRU_SOFTRST_CON14_WDT_NS_PSRSTN_REQ_SHIFT          (3U)
#define CRU_SOFTRST_CON14_WDT_NS_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON14_WDT_NS_PSRSTN_REQ_SHIFT)          /* 0x00000008 */
#define CRU_SOFTRST_CON14_EFUSE_NS_PSRSTN_REQ_SHIFT        (4U)
#define CRU_SOFTRST_CON14_EFUSE_NS_PSRSTN_REQ_MASK         (0x1U << CRU_SOFTRST_CON14_EFUSE_NS_PSRSTN_REQ_SHIFT)        /* 0x00000010 */
#define CRU_SOFTRST_CON14_EFUSE_NS_SRSTN_REQ_SHIFT         (5U)
#define CRU_SOFTRST_CON14_EFUSE_NS_SRSTN_REQ_MASK          (0x1U << CRU_SOFTRST_CON14_EFUSE_NS_SRSTN_REQ_SHIFT)         /* 0x00000020 */
#define CRU_SOFTRST_CON14_GPIO1_PSRSTN_REQ_SHIFT           (6U)
#define CRU_SOFTRST_CON14_GPIO1_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_GPIO1_PSRSTN_REQ_SHIFT)           /* 0x00000040 */
#define CRU_SOFTRST_CON14_GPIO1_DBSRSTN_REQ_SHIFT          (7U)
#define CRU_SOFTRST_CON14_GPIO1_DBSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON14_GPIO1_DBSRSTN_REQ_SHIFT)          /* 0x00000080 */
#define CRU_SOFTRST_CON14_GPIO2_PSRSTN_REQ_SHIFT           (8U)
#define CRU_SOFTRST_CON14_GPIO2_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_GPIO2_PSRSTN_REQ_SHIFT)           /* 0x00000100 */
#define CRU_SOFTRST_CON14_GPIO2_DBSRSTN_REQ_SHIFT          (9U)
#define CRU_SOFTRST_CON14_GPIO2_DBSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON14_GPIO2_DBSRSTN_REQ_SHIFT)          /* 0x00000200 */
#define CRU_SOFTRST_CON14_GPIO3_PSRSTN_REQ_SHIFT           (10U)
#define CRU_SOFTRST_CON14_GPIO3_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_GPIO3_PSRSTN_REQ_SHIFT)           /* 0x00000400 */
#define CRU_SOFTRST_CON14_GPIO3_DBSRSTN_REQ_SHIFT          (11U)
#define CRU_SOFTRST_CON14_GPIO3_DBSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON14_GPIO3_DBSRSTN_REQ_SHIFT)          /* 0x00000800 */
#define CRU_SOFTRST_CON14_GPIO4_PSRSTN_REQ_SHIFT           (12U)
#define CRU_SOFTRST_CON14_GPIO4_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON14_GPIO4_PSRSTN_REQ_SHIFT)           /* 0x00001000 */
#define CRU_SOFTRST_CON14_GPIO4_DBSRSTN_REQ_SHIFT          (13U)
#define CRU_SOFTRST_CON14_GPIO4_DBSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON14_GPIO4_DBSRSTN_REQ_SHIFT)          /* 0x00002000 */
#define CRU_SOFTRST_CON14_BUS_SUB_NIU_MSSRSTN_REQ_SHIFT    (14U)
#define CRU_SOFTRST_CON14_BUS_SUB_NIU_MSSRSTN_REQ_MASK     (0x1U << CRU_SOFTRST_CON14_BUS_SUB_NIU_MSSRSTN_REQ_SHIFT)    /* 0x00004000 */
/* SOFTRST_CON15 */
#define CRU_SOFTRST_CON15_OFFSET                           (0x33C)
#define CRU_SOFTRST_CON15_I2C4_PSRSTN_REQ_SHIFT            (0U)
#define CRU_SOFTRST_CON15_I2C4_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON15_I2C4_PSRSTN_REQ_SHIFT)            /* 0x00000001 */
#define CRU_SOFTRST_CON15_I2C4_SRSTN_REQ_SHIFT             (1U)
#define CRU_SOFTRST_CON15_I2C4_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON15_I2C4_SRSTN_REQ_SHIFT)             /* 0x00000002 */
#define CRU_SOFTRST_CON15_I2C5_PSRSTN_REQ_SHIFT            (2U)
#define CRU_SOFTRST_CON15_I2C5_PSRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON15_I2C5_PSRSTN_REQ_SHIFT)            /* 0x00000004 */
#define CRU_SOFTRST_CON15_I2C5_SRSTN_REQ_SHIFT             (3U)
#define CRU_SOFTRST_CON15_I2C5_SRSTN_REQ_MASK              (0x1U << CRU_SOFTRST_CON15_I2C5_SRSTN_REQ_SHIFT)             /* 0x00000008 */
#define CRU_SOFTRST_CON15_SARADC_PSRSTN_REQ_SHIFT          (13U)
#define CRU_SOFTRST_CON15_SARADC_PSRSTN_REQ_MASK           (0x1U << CRU_SOFTRST_CON15_SARADC_PSRSTN_REQ_SHIFT)          /* 0x00002000 */
#define CRU_SOFTRST_CON15_TSADC_PSRSTN_REQ_SHIFT           (14U)
#define CRU_SOFTRST_CON15_TSADC_PSRSTN_REQ_MASK            (0x1U << CRU_SOFTRST_CON15_TSADC_PSRSTN_REQ_SHIFT)           /* 0x00004000 */
#define CRU_SOFTRST_CON15_TSADC_SRSTN_REQ_SHIFT            (15U)
#define CRU_SOFTRST_CON15_TSADC_SRSTN_REQ_MASK             (0x1U << CRU_SOFTRST_CON15_TSADC_SRSTN_REQ_SHIFT)            /* 0x00008000 */
/* SDMMC_CON0 */
#define CRU_SDMMC_CON0_OFFSET                              (0x380)
#define CRU_SDMMC_CON0_INIT_STATE_SHIFT                    (0U)
#define CRU_SDMMC_CON0_INIT_STATE_MASK                     (0x1U << CRU_SDMMC_CON0_INIT_STATE_SHIFT)                    /* 0x00000001 */
#define CRU_SDMMC_CON0_DRV_DEGREE_SHIFT                    (1U)
#define CRU_SDMMC_CON0_DRV_DEGREE_MASK                     (0x3U << CRU_SDMMC_CON0_DRV_DEGREE_SHIFT)                    /* 0x00000006 */
#define CRU_SDMMC_CON0_DRV_DELAYNUM_SHIFT                  (3U)
#define CRU_SDMMC_CON0_DRV_DELAYNUM_MASK                   (0xFFU << CRU_SDMMC_CON0_DRV_DELAYNUM_SHIFT)                 /* 0x000007F8 */
#define CRU_SDMMC_CON0_DRV_SEL_SHIFT                       (11U)
#define CRU_SDMMC_CON0_DRV_SEL_MASK                        (0x1U << CRU_SDMMC_CON0_DRV_SEL_SHIFT)                       /* 0x00000800 */
/* SDMMC_CON1 */
#define CRU_SDMMC_CON1_OFFSET                              (0x384)
#define CRU_SDMMC_CON1_SAMPLE_DEGREE_SHIFT                 (1U)
#define CRU_SDMMC_CON1_SAMPLE_DEGREE_MASK                  (0x3U << CRU_SDMMC_CON1_SAMPLE_DEGREE_SHIFT)                 /* 0x00000006 */
#define CRU_SDMMC_CON1_SAMPLE_DELAYNUM_SHIFT               (3U)
#define CRU_SDMMC_CON1_SAMPLE_DELAYNUM_MASK                (0xFFU << CRU_SDMMC_CON1_SAMPLE_DELAYNUM_SHIFT)              /* 0x000007F8 */
#define CRU_SDMMC_CON1_SAMPLE_SEL_SHIFT                    (11U)
#define CRU_SDMMC_CON1_SAMPLE_SEL_MASK                     (0x1U << CRU_SDMMC_CON1_SAMPLE_SEL_SHIFT)                    /* 0x00000800 */
/* SDIO_CON0 */
#define CRU_SDIO_CON0_OFFSET                               (0x388)
#define CRU_SDIO_CON0_INIT_STATE_SHIFT                     (0U)
#define CRU_SDIO_CON0_INIT_STATE_MASK                      (0x1U << CRU_SDIO_CON0_INIT_STATE_SHIFT)                     /* 0x00000001 */
#define CRU_SDIO_CON0_DRV_DEGREE_SHIFT                     (1U)
#define CRU_SDIO_CON0_DRV_DEGREE_MASK                      (0x3U << CRU_SDIO_CON0_DRV_DEGREE_SHIFT)                     /* 0x00000006 */
#define CRU_SDIO_CON0_DRV_DELAYNUM_SHIFT                   (3U)
#define CRU_SDIO_CON0_DRV_DELAYNUM_MASK                    (0xFFU << CRU_SDIO_CON0_DRV_DELAYNUM_SHIFT)                  /* 0x000007F8 */
#define CRU_SDIO_CON0_DRV_SEL_SHIFT                        (11U)
#define CRU_SDIO_CON0_DRV_SEL_MASK                         (0x1U << CRU_SDIO_CON0_DRV_SEL_SHIFT)                        /* 0x00000800 */
/* SDIO_CON1 */
#define CRU_SDIO_CON1_OFFSET                               (0x38C)
#define CRU_SDIO_CON1_SAMPLE_DEGREE_SHIFT                  (1U)
#define CRU_SDIO_CON1_SAMPLE_DEGREE_MASK                   (0x3U << CRU_SDIO_CON1_SAMPLE_DEGREE_SHIFT)                  /* 0x00000006 */
#define CRU_SDIO_CON1_SAMPLE_DELAYNUM_SHIFT                (3U)
#define CRU_SDIO_CON1_SAMPLE_DELAYNUM_MASK                 (0xFFU << CRU_SDIO_CON1_SAMPLE_DELAYNUM_SHIFT)               /* 0x000007F8 */
#define CRU_SDIO_CON1_SAMPLE_SEL_SHIFT                     (11U)
#define CRU_SDIO_CON1_SAMPLE_SEL_MASK                      (0x1U << CRU_SDIO_CON1_SAMPLE_SEL_SHIFT)                     /* 0x00000800 */
/* EMMC_CON0 */
#define CRU_EMMC_CON0_OFFSET                               (0x390)
#define CRU_EMMC_CON0_INIT_STATE_SHIFT                     (0U)
#define CRU_EMMC_CON0_INIT_STATE_MASK                      (0x1U << CRU_EMMC_CON0_INIT_STATE_SHIFT)                     /* 0x00000001 */
#define CRU_EMMC_CON0_DRV_DEGREE_SHIFT                     (1U)
#define CRU_EMMC_CON0_DRV_DEGREE_MASK                      (0x3U << CRU_EMMC_CON0_DRV_DEGREE_SHIFT)                     /* 0x00000006 */
#define CRU_EMMC_CON0_DRV_DELAYNUM_SHIFT                   (3U)
#define CRU_EMMC_CON0_DRV_DELAYNUM_MASK                    (0xFFU << CRU_EMMC_CON0_DRV_DELAYNUM_SHIFT)                  /* 0x000007F8 */
#define CRU_EMMC_CON0_DRV_SEL_SHIFT                        (11U)
#define CRU_EMMC_CON0_DRV_SEL_MASK                         (0x1U << CRU_EMMC_CON0_DRV_SEL_SHIFT)                        /* 0x00000800 */
/* EMMC_CON1 */
#define CRU_EMMC_CON1_OFFSET                               (0x394)
#define CRU_EMMC_CON1_SAMPLE_DEGREE_SHIFT                  (1U)
#define CRU_EMMC_CON1_SAMPLE_DEGREE_MASK                   (0x3U << CRU_EMMC_CON1_SAMPLE_DEGREE_SHIFT)                  /* 0x00000006 */
#define CRU_EMMC_CON1_SAMPLE_DELAYNUM_SHIFT                (3U)
#define CRU_EMMC_CON1_SAMPLE_DELAYNUM_MASK                 (0xFFU << CRU_EMMC_CON1_SAMPLE_DELAYNUM_SHIFT)               /* 0x000007F8 */
#define CRU_EMMC_CON1_SAMPLE_SEL_SHIFT                     (11U)
#define CRU_EMMC_CON1_SAMPLE_SEL_MASK                      (0x1U << CRU_EMMC_CON1_SAMPLE_SEL_SHIFT)                     /* 0x00000800 */
/* AUTOCS_CON0 */
#define CRU_AUTOCS_CON0_OFFSET                             (0x400)
#define CRU_AUTOCS_CON0_PDCORE_IDLE_TH_SHIFT               (0U)
#define CRU_AUTOCS_CON0_PDCORE_IDLE_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON0_PDCORE_IDLE_TH_SHIFT)            /* 0x0000FFFF */
#define CRU_AUTOCS_CON0_PDCORE_WAIT_TH_SHIFT               (16U)
#define CRU_AUTOCS_CON0_PDCORE_WAIT_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON0_PDCORE_WAIT_TH_SHIFT)            /* 0xFFFF0000 */
/* AUTOCS_CON1 */
#define CRU_AUTOCS_CON1_OFFSET                             (0x404)
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
#define CRU_AUTOCS_CON2_OFFSET                             (0x408)
#define CRU_AUTOCS_CON2_PDBUS_IDLE_TH_SHIFT                (0U)
#define CRU_AUTOCS_CON2_PDBUS_IDLE_TH_MASK                 (0xFFFFU << CRU_AUTOCS_CON2_PDBUS_IDLE_TH_SHIFT)             /* 0x0000FFFF */
#define CRU_AUTOCS_CON2_PDBUS_WAIT_TH_SHIFT                (16U)
#define CRU_AUTOCS_CON2_PDBUS_WAIT_TH_MASK                 (0xFFFFU << CRU_AUTOCS_CON2_PDBUS_WAIT_TH_SHIFT)             /* 0xFFFF0000 */
/* AUTOCS_CON3 */
#define CRU_AUTOCS_CON3_OFFSET                             (0x40C)
#define CRU_AUTOCS_CON3_PDBUS_SWITCH_EN_SHIFT              (0U)
#define CRU_AUTOCS_CON3_PDBUS_SWITCH_EN_MASK               (0x1U << CRU_AUTOCS_CON3_PDBUS_SWITCH_EN_SHIFT)              /* 0x00000001 */
#define CRU_AUTOCS_CON3_PDBUS_HSCLK_EN_SHIFT               (1U)
#define CRU_AUTOCS_CON3_PDBUS_HSCLK_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_HSCLK_EN_SHIFT)               /* 0x00000002 */
#define CRU_AUTOCS_CON3_PDBUS_MSCLK_EN_SHIFT               (2U)
#define CRU_AUTOCS_CON3_PDBUS_MSCLK_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_MSCLK_EN_SHIFT)               /* 0x00000004 */
#define CRU_AUTOCS_CON3_PDBUS_LSCLK_EN_SHIFT               (3U)
#define CRU_AUTOCS_CON3_PDBUS_LSCLK_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_LSCLK_EN_SHIFT)               /* 0x00000008 */
#define CRU_AUTOCS_CON3_PDBUS_MASTER_EN_SHIFT              (4U)
#define CRU_AUTOCS_CON3_PDBUS_MASTER_EN_MASK               (0x1U << CRU_AUTOCS_CON3_PDBUS_MASTER_EN_SHIFT)              /* 0x00000010 */
#define CRU_AUTOCS_CON3_PDBUS_SLAVE_EN_SHIFT               (5U)
#define CRU_AUTOCS_CON3_PDBUS_SLAVE_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_SLAVE_EN_SHIFT)               /* 0x00000020 */
#define CRU_AUTOCS_CON3_PDBUS_DCF_M_EN_SHIFT               (6U)
#define CRU_AUTOCS_CON3_PDBUS_DCF_M_EN_MASK                (0x1U << CRU_AUTOCS_CON3_PDBUS_DCF_M_EN_SHIFT)               /* 0x00000040 */
#define CRU_AUTOCS_CON3_PDBUS_DMAC_M_EN_SHIFT              (7U)
#define CRU_AUTOCS_CON3_PDBUS_DMAC_M_EN_MASK               (0x1U << CRU_AUTOCS_CON3_PDBUS_DMAC_M_EN_SHIFT)              /* 0x00000080 */
#define CRU_AUTOCS_CON3_PDBUS_CRYPTO_M_EN_SHIFT            (8U)
#define CRU_AUTOCS_CON3_PDBUS_CRYPTO_M_EN_MASK             (0x1U << CRU_AUTOCS_CON3_PDBUS_CRYPTO_M_EN_SHIFT)            /* 0x00000100 */
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_OUT_SHIFT             (12U)
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_OUT_MASK              (0x3U << CRU_AUTOCS_CON3_PDBUS_CLKSEL_OUT_SHIFT)             /* 0x00003000 */
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_SHIFT                 (14U)
#define CRU_AUTOCS_CON3_PDBUS_CLKSEL_MASK                  (0x3U << CRU_AUTOCS_CON3_PDBUS_CLKSEL_SHIFT)                 /* 0x0000C000 */
/* AUTOCS_CON4 */
#define CRU_AUTOCS_CON4_OFFSET                             (0x410)
#define CRU_AUTOCS_CON4_PDPERI_IDLE_TH_SHIFT               (0U)
#define CRU_AUTOCS_CON4_PDPERI_IDLE_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON4_PDPERI_IDLE_TH_SHIFT)            /* 0x0000FFFF */
#define CRU_AUTOCS_CON4_PDPERI_WAIT_TH_SHIFT               (16U)
#define CRU_AUTOCS_CON4_PDPERI_WAIT_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON4_PDPERI_WAIT_TH_SHIFT)            /* 0xFFFF0000 */
/* AUTOCS_CON5 */
#define CRU_AUTOCS_CON5_OFFSET                             (0x414)
#define CRU_AUTOCS_CON5_PDPERI_SWITCH_EN_SHIFT             (0U)
#define CRU_AUTOCS_CON5_PDPERI_SWITCH_EN_MASK              (0x1U << CRU_AUTOCS_CON5_PDPERI_SWITCH_EN_SHIFT)             /* 0x00000001 */
#define CRU_AUTOCS_CON5_PDPERI_ACLK_EN_SHIFT               (1U)
#define CRU_AUTOCS_CON5_PDPERI_ACLK_EN_MASK                (0x1U << CRU_AUTOCS_CON5_PDPERI_ACLK_EN_SHIFT)               /* 0x00000002 */
#define CRU_AUTOCS_CON5_PDPERI_HCLK_EN_SHIFT               (2U)
#define CRU_AUTOCS_CON5_PDPERI_HCLK_EN_MASK                (0x1U << CRU_AUTOCS_CON5_PDPERI_HCLK_EN_SHIFT)               /* 0x00000004 */
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_OUT_SHIFT            (12U)
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_OUT_MASK             (0x3U << CRU_AUTOCS_CON5_PDPERI_CLKSEL_OUT_SHIFT)            /* 0x00003000 */
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_SHIFT                (14U)
#define CRU_AUTOCS_CON5_PDPERI_CLKSEL_MASK                 (0x3U << CRU_AUTOCS_CON5_PDPERI_CLKSEL_SHIFT)                /* 0x0000C000 */
/* AUTOCS_CON6 */
#define CRU_AUTOCS_CON6_OFFSET                             (0x418)
#define CRU_AUTOCS_CON6_PDDDR_IDLE_TH_SHIFT                (0U)
#define CRU_AUTOCS_CON6_PDDDR_IDLE_TH_MASK                 (0xFFFFU << CRU_AUTOCS_CON6_PDDDR_IDLE_TH_SHIFT)             /* 0x0000FFFF */
#define CRU_AUTOCS_CON6_PDDDR_WAIT_TH_SHIFT                (16U)
#define CRU_AUTOCS_CON6_PDDDR_WAIT_TH_MASK                 (0xFFFFU << CRU_AUTOCS_CON6_PDDDR_WAIT_TH_SHIFT)             /* 0xFFFF0000 */
/* AUTOCS_CON7 */
#define CRU_AUTOCS_CON7_OFFSET                             (0x41C)
#define CRU_AUTOCS_CON7_PDDDR_SWITCH_EN_SHIFT              (0U)
#define CRU_AUTOCS_CON7_PDDDR_SWITCH_EN_MASK               (0x1U << CRU_AUTOCS_CON7_PDDDR_SWITCH_EN_SHIFT)              /* 0x00000001 */
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_OUT_SHIFT             (12U)
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_OUT_MASK              (0x3U << CRU_AUTOCS_CON7_PDDDR_CLKSEL_OUT_SHIFT)             /* 0x00003000 */
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_SHIFT                 (14U)
#define CRU_AUTOCS_CON7_PDDDR_CLKSEL_MASK                  (0x3U << CRU_AUTOCS_CON7_PDDDR_CLKSEL_SHIFT)                 /* 0x0000C000 */
/* AUTOCS_CON8 */
#define CRU_AUTOCS_CON8_OFFSET                             (0x420)
#define CRU_AUTOCS_CON8_PDIMEM_IDLE_TH_SHIFT               (0U)
#define CRU_AUTOCS_CON8_PDIMEM_IDLE_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON8_PDIMEM_IDLE_TH_SHIFT)            /* 0x0000FFFF */
#define CRU_AUTOCS_CON8_PDIMEM_WAIT_TH_SHIFT               (16U)
#define CRU_AUTOCS_CON8_PDIMEM_WAIT_TH_MASK                (0xFFFFU << CRU_AUTOCS_CON8_PDIMEM_WAIT_TH_SHIFT)            /* 0xFFFF0000 */
/* AUTOCS_CON9 */
#define CRU_AUTOCS_CON9_OFFSET                             (0x424)
#define CRU_AUTOCS_CON9_PDIMEM_SWITCH_EN_SHIFT             (0U)
#define CRU_AUTOCS_CON9_PDIMEM_SWITCH_EN_MASK              (0x1U << CRU_AUTOCS_CON9_PDIMEM_SWITCH_EN_SHIFT)             /* 0x00000001 */
#define CRU_AUTOCS_CON9_PDIMEM_CLKSEL_OUT_SHIFT            (12U)
#define CRU_AUTOCS_CON9_PDIMEM_CLKSEL_OUT_MASK             (0x3U << CRU_AUTOCS_CON9_PDIMEM_CLKSEL_OUT_SHIFT)            /* 0x00003000 */
#define CRU_AUTOCS_CON9_PDIMEM_CLKSEL_SHIFT                (14U)
#define CRU_AUTOCS_CON9_PDIMEM_CLKSEL_MASK                 (0x3U << CRU_AUTOCS_CON9_PDIMEM_CLKSEL_SHIFT)                /* 0x0000C000 */
/* PMUPLL_CON0 */
#define CRU_PMUPLL_CON0_OFFSET                             (0x4000)
#define CRU_PMUPLL_CON0_FBDIV_SHIFT                        (0U)
#define CRU_PMUPLL_CON0_FBDIV_MASK                         (0xFFFU << CRU_PMUPLL_CON0_FBDIV_SHIFT)                      /* 0x00000FFF */
#define CRU_PMUPLL_CON0_POSTDIV1_SHIFT                     (12U)
#define CRU_PMUPLL_CON0_POSTDIV1_MASK                      (0x7U << CRU_PMUPLL_CON0_POSTDIV1_SHIFT)                     /* 0x00007000 */
#define CRU_PMUPLL_CON0_BYPASS_SHIFT                       (15U)
#define CRU_PMUPLL_CON0_BYPASS_MASK                        (0x1U << CRU_PMUPLL_CON0_BYPASS_SHIFT)                       /* 0x00008000 */
/* PMUPLL_CON1 */
#define CRU_PMUPLL_CON1_OFFSET                             (0x4004)
#define CRU_PMUPLL_CON1_REFDIV_SHIFT                       (0U)
#define CRU_PMUPLL_CON1_REFDIV_MASK                        (0x3FU << CRU_PMUPLL_CON1_REFDIV_SHIFT)                      /* 0x0000003F */
#define CRU_PMUPLL_CON1_POSTDIV2_SHIFT                     (6U)
#define CRU_PMUPLL_CON1_POSTDIV2_MASK                      (0x7U << CRU_PMUPLL_CON1_POSTDIV2_SHIFT)                     /* 0x000001C0 */
#define CRU_PMUPLL_CON1_PLL_LOCK_SHIFT                     (10U)
#define CRU_PMUPLL_CON1_PLL_LOCK_MASK                      (0x1U << CRU_PMUPLL_CON1_PLL_LOCK_SHIFT)                     /* 0x00000400 */
#define CRU_PMUPLL_CON1_DSMPD_SHIFT                        (12U)
#define CRU_PMUPLL_CON1_DSMPD_MASK                         (0x1U << CRU_PMUPLL_CON1_DSMPD_SHIFT)                        /* 0x00001000 */
#define CRU_PMUPLL_CON1_PLLPD0_SHIFT                       (13U)
#define CRU_PMUPLL_CON1_PLLPD0_MASK                        (0x1U << CRU_PMUPLL_CON1_PLLPD0_SHIFT)                       /* 0x00002000 */
#define CRU_PMUPLL_CON1_PLLPD1_SHIFT                       (14U)
#define CRU_PMUPLL_CON1_PLLPD1_MASK                        (0x1U << CRU_PMUPLL_CON1_PLLPD1_SHIFT)                       /* 0x00004000 */
#define CRU_PMUPLL_CON1_PLLPDSEL_SHIFT                     (15U)
#define CRU_PMUPLL_CON1_PLLPDSEL_MASK                      (0x1U << CRU_PMUPLL_CON1_PLLPDSEL_SHIFT)                     /* 0x00008000 */
/* PMUPLL_CON2 */
#define CRU_PMUPLL_CON2_OFFSET                             (0x4008)
#define CRU_PMUPLL_CON2_FRACDIV_SHIFT                      (0U)
#define CRU_PMUPLL_CON2_FRACDIV_MASK                       (0xFFFFFFU << CRU_PMUPLL_CON2_FRACDIV_SHIFT)                 /* 0x00FFFFFF */
#define CRU_PMUPLL_CON2_DACPD_SHIFT                        (24U)
#define CRU_PMUPLL_CON2_DACPD_MASK                         (0x1U << CRU_PMUPLL_CON2_DACPD_SHIFT)                        /* 0x01000000 */
#define CRU_PMUPLL_CON2_FOUTPOSTDIVPD_SHIFT                (25U)
#define CRU_PMUPLL_CON2_FOUTPOSTDIVPD_MASK                 (0x1U << CRU_PMUPLL_CON2_FOUTPOSTDIVPD_SHIFT)                /* 0x02000000 */
#define CRU_PMUPLL_CON2_FOUTVCOPD_SHIFT                    (26U)
#define CRU_PMUPLL_CON2_FOUTVCOPD_MASK                     (0x1U << CRU_PMUPLL_CON2_FOUTVCOPD_SHIFT)                    /* 0x04000000 */
#define CRU_PMUPLL_CON2_FOUT4PHASEPD_SHIFT                 (27U)
#define CRU_PMUPLL_CON2_FOUT4PHASEPD_MASK                  (0x1U << CRU_PMUPLL_CON2_FOUT4PHASEPD_SHIFT)                 /* 0x08000000 */
/* PMUPLL_CON3 */
#define CRU_PMUPLL_CON3_OFFSET                             (0x400C)
#define CRU_PMUPLL_CON3_SSMOD_BP_SHIFT                     (0U)
#define CRU_PMUPLL_CON3_SSMOD_BP_MASK                      (0x1U << CRU_PMUPLL_CON3_SSMOD_BP_SHIFT)                     /* 0x00000001 */
#define CRU_PMUPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT           (1U)
#define CRU_PMUPLL_CON3_SSMOD_DISABLE_SSCG_MASK            (0x1U << CRU_PMUPLL_CON3_SSMOD_DISABLE_SSCG_SHIFT)           /* 0x00000002 */
#define CRU_PMUPLL_CON3_SSMOD_RESET_SHIFT                  (2U)
#define CRU_PMUPLL_CON3_SSMOD_RESET_MASK                   (0x1U << CRU_PMUPLL_CON3_SSMOD_RESET_SHIFT)                  /* 0x00000004 */
#define CRU_PMUPLL_CON3_SSMOD_DOWNSPREAD_SHIFT             (3U)
#define CRU_PMUPLL_CON3_SSMOD_DOWNSPREAD_MASK              (0x1U << CRU_PMUPLL_CON3_SSMOD_DOWNSPREAD_SHIFT)             /* 0x00000008 */
#define CRU_PMUPLL_CON3_SSMOD_DIVVAL_SHIFT                 (4U)
#define CRU_PMUPLL_CON3_SSMOD_DIVVAL_MASK                  (0xFU << CRU_PMUPLL_CON3_SSMOD_DIVVAL_SHIFT)                 /* 0x000000F0 */
#define CRU_PMUPLL_CON3_SSMOD_SPREAD_SHIFT                 (8U)
#define CRU_PMUPLL_CON3_SSMOD_SPREAD_MASK                  (0x1FU << CRU_PMUPLL_CON3_SSMOD_SPREAD_SHIFT)                /* 0x00001F00 */
/* PMUPLL_CON4 */
#define CRU_PMUPLL_CON4_OFFSET                             (0x4010)
#define CRU_PMUPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT           (0U)
#define CRU_PMUPLL_CON4_SSMOD_SEL_EXT_WAVE_MASK            (0x1U << CRU_PMUPLL_CON4_SSMOD_SEL_EXT_WAVE_SHIFT)           /* 0x00000001 */
#define CRU_PMUPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT            (8U)
#define CRU_PMUPLL_CON4_SSMOD_EXT_MAXADDR_MASK             (0xFFU << CRU_PMUPLL_CON4_SSMOD_EXT_MAXADDR_SHIFT)           /* 0x0000FF00 */
/* PMU_MODE */
#define CRU_PMU_MODE_OFFSET                                (0x4020)
#define CRU_PMU_MODE_PMUPLL_WORK_MODE_SHIFT                (0U)
#define CRU_PMU_MODE_PMUPLL_WORK_MODE_MASK                 (0x3U << CRU_PMU_MODE_PMUPLL_WORK_MODE_SHIFT)                /* 0x00000003 */
/* PMU_CLKSEL_CON0 */
#define CRU_PMU_CLKSEL_CON0_OFFSET                         (0x4040)
#define CRU_PMU_CLKSEL_CON0_PCLK_PDPMU_DIV_CON_SHIFT       (0U)
#define CRU_PMU_CLKSEL_CON0_PCLK_PDPMU_DIV_CON_MASK        (0x1FU << CRU_PMU_CLKSEL_CON0_PCLK_PDPMU_DIV_CON_SHIFT)      /* 0x0000001F */
#define CRU_PMU_CLKSEL_CON0_XIN_OSC0_FUNC_DIV_CON_SHIFT    (8U)
#define CRU_PMU_CLKSEL_CON0_XIN_OSC0_FUNC_DIV_CON_MASK     (0x1FU << CRU_PMU_CLKSEL_CON0_XIN_OSC0_FUNC_DIV_CON_SHIFT)   /* 0x00001F00 */
#define CRU_PMU_CLKSEL_CON0_CLK_RTC32K_CLK_SEL_SHIFT       (14U)
#define CRU_PMU_CLKSEL_CON0_CLK_RTC32K_CLK_SEL_MASK        (0x3U << CRU_PMU_CLKSEL_CON0_CLK_RTC32K_CLK_SEL_SHIFT)       /* 0x0000C000 */
/* PMU_CLKSEL_CON1 */
#define CRU_PMU_CLKSEL_CON1_OFFSET                         (0x4044)
#define CRU_PMU_CLKSEL_CON1_CLK_DIV32P768KHZ_DIV_CON_SHIFT (0U)
#define CRU_PMU_CLKSEL_CON1_CLK_DIV32P768KHZ_DIV_CON_MASK  (0xFFFFFFFFU << CRU_PMU_CLKSEL_CON1_CLK_DIV32P768KHZ_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* PMU_CLKSEL_CON2 */
#define CRU_PMU_CLKSEL_CON2_OFFSET                         (0x4048)
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
#define CRU_PMU_CLKSEL_CON3_OFFSET                         (0x404C)
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_DIV_CON_SHIFT        (0U)
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_DIV_CON_MASK         (0x7FU << CRU_PMU_CLKSEL_CON3_CLK_UART0_DIV_CON_SHIFT)       /* 0x0000007F */
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_PLL_SEL_SHIFT        (14U)
#define CRU_PMU_CLKSEL_CON3_CLK_UART0_PLL_SEL_MASK         (0x3U << CRU_PMU_CLKSEL_CON3_CLK_UART0_PLL_SEL_SHIFT)        /* 0x0000C000 */
/* PMU_CLKSEL_CON4 */
#define CRU_PMU_CLKSEL_CON4_OFFSET                         (0x4050)
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_DIVNP5_DIV_CON_SHIFT (0U)
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_DIVNP5_DIV_CON_MASK  (0x7FU << CRU_PMU_CLKSEL_CON4_CLK_UART0_DIVNP5_DIV_CON_SHIFT) /* 0x0000007F */
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_SEL_SHIFT            (14U)
#define CRU_PMU_CLKSEL_CON4_CLK_UART0_SEL_MASK             (0x3U << CRU_PMU_CLKSEL_CON4_CLK_UART0_SEL_SHIFT)            /* 0x0000C000 */
/* PMU_CLKSEL_CON5 */
#define CRU_PMU_CLKSEL_CON5_OFFSET                         (0x4054)
#define CRU_PMU_CLKSEL_CON5_CLK_UART0_FRAC_DIV_CON_SHIFT   (0U)
#define CRU_PMU_CLKSEL_CON5_CLK_UART0_FRAC_DIV_CON_MASK    (0xFFFFFFFFU << CRU_PMU_CLKSEL_CON5_CLK_UART0_FRAC_DIV_CON_SHIFT) /* 0xFFFFFFFF */
/* PMU_CLKSEL_CON6 */
#define CRU_PMU_CLKSEL_CON6_OFFSET                         (0x4058)
#define CRU_PMU_CLKSEL_CON6_DBCLK_GPIO0_DIV_CON_SHIFT      (0U)
#define CRU_PMU_CLKSEL_CON6_DBCLK_GPIO0_DIV_CON_MASK       (0x7FFU << CRU_PMU_CLKSEL_CON6_DBCLK_GPIO0_DIV_CON_SHIFT)    /* 0x000007FF */
#define CRU_PMU_CLKSEL_CON6_DBCLK_GPIO0_PLL_SEL_SHIFT      (15U)
#define CRU_PMU_CLKSEL_CON6_DBCLK_GPIO0_PLL_SEL_MASK       (0x1U << CRU_PMU_CLKSEL_CON6_DBCLK_GPIO0_PLL_SEL_SHIFT)      /* 0x00008000 */
/* PMU_CLKSEL_CON7 */
#define CRU_PMU_CLKSEL_CON7_OFFSET                         (0x405C)
#define CRU_PMU_CLKSEL_CON7_CLK_PCIEPHY_REF_DIV_CON_SHIFT  (0U)
#define CRU_PMU_CLKSEL_CON7_CLK_PCIEPHY_REF_DIV_CON_MASK   (0x3U << CRU_PMU_CLKSEL_CON7_CLK_PCIEPHY_REF_DIV_CON_SHIFT)  /* 0x00000003 */
#define CRU_PMU_CLKSEL_CON7_CLK_PCIEPHY_REF_CLK_SEL_SHIFT  (4U)
#define CRU_PMU_CLKSEL_CON7_CLK_PCIEPHY_REF_CLK_SEL_MASK   (0x1U << CRU_PMU_CLKSEL_CON7_CLK_PCIEPHY_REF_CLK_SEL_SHIFT)  /* 0x00000010 */
#define CRU_PMU_CLKSEL_CON7_CLK_I2C0_DIV_CON_SHIFT         (8U)
#define CRU_PMU_CLKSEL_CON7_CLK_I2C0_DIV_CON_MASK          (0x7FU << CRU_PMU_CLKSEL_CON7_CLK_I2C0_DIV_CON_SHIFT)        /* 0x00007F00 */
#define CRU_PMU_CLKSEL_CON7_CLK_I2C0_PLL_SEL_SHIFT         (15U)
#define CRU_PMU_CLKSEL_CON7_CLK_I2C0_PLL_SEL_MASK          (0x1U << CRU_PMU_CLKSEL_CON7_CLK_I2C0_PLL_SEL_SHIFT)         /* 0x00008000 */
/* PMU_CLKGATE_CON0 */
#define CRU_PMU_CLKGATE_CON0_OFFSET                        (0x4080)
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
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_I2C0_CLK_EN_SHIFT    (9U)
#define CRU_PMU_CLKGATE_CON0_PCLK_PMU_I2C0_CLK_EN_MASK     (0x1U << CRU_PMU_CLKGATE_CON0_PCLK_PMU_I2C0_CLK_EN_SHIFT)    /* 0x00000200 */
#define CRU_PMU_CLKGATE_CON0_XIN_OSC0_FUNC_DIV_SRC_CLK_EN_SHIFT (12U)
#define CRU_PMU_CLKGATE_CON0_XIN_OSC0_FUNC_DIV_SRC_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON0_XIN_OSC0_FUNC_DIV_SRC_CLK_EN_SHIFT) /* 0x00001000 */
#define CRU_PMU_CLKGATE_CON0_CLK_DIV32P768KHZ_SRC_CLK_EN_SHIFT (13U)
#define CRU_PMU_CLKGATE_CON0_CLK_DIV32P768KHZ_SRC_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON0_CLK_DIV32P768KHZ_SRC_CLK_EN_SHIFT) /* 0x00002000 */
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_PLL_CLK_EN_SHIFT     (14U)
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_PLL_CLK_EN_MASK      (0x1U << CRU_PMU_CLKGATE_CON0_CLK_WIFI_PLL_CLK_EN_SHIFT)     /* 0x00004000 */
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_CLK_EN_SHIFT         (15U)
#define CRU_PMU_CLKGATE_CON0_CLK_WIFI_CLK_EN_MASK          (0x1U << CRU_PMU_CLKGATE_CON0_CLK_WIFI_CLK_EN_SHIFT)         /* 0x00008000 */
/* PMU_CLKGATE_CON1 */
#define CRU_PMU_CLKGATE_CON1_OFFSET                        (0x4084)
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
#define CRU_PMU_CLKGATE_CON1_CLK_I2C0_PMU_PLL_CLK_EN_SHIFT (5U)
#define CRU_PMU_CLKGATE_CON1_CLK_I2C0_PMU_PLL_CLK_EN_MASK  (0x1U << CRU_PMU_CLKGATE_CON1_CLK_I2C0_PMU_PLL_CLK_EN_SHIFT) /* 0x00000020 */
#define CRU_PMU_CLKGATE_CON1_DBCLK_GPIO0_PMU_PLL_CLK_EN_SHIFT (6U)
#define CRU_PMU_CLKGATE_CON1_DBCLK_GPIO0_PMU_PLL_CLK_EN_MASK (0x1U << CRU_PMU_CLKGATE_CON1_DBCLK_GPIO0_PMU_PLL_CLK_EN_SHIFT) /* 0x00000040 */
#define CRU_PMU_CLKGATE_CON1_CLK_REF24M_PLL_CLK_EN_SHIFT   (8U)
#define CRU_PMU_CLKGATE_CON1_CLK_REF24M_PLL_CLK_EN_MASK    (0x1U << CRU_PMU_CLKGATE_CON1_CLK_REF24M_PLL_CLK_EN_SHIFT)   /* 0x00000100 */
#define CRU_PMU_CLKGATE_CON1_USBPHY_REF_CLK_EN_SHIFT       (9U)
#define CRU_PMU_CLKGATE_CON1_USBPHY_REF_CLK_EN_MASK        (0x1U << CRU_PMU_CLKGATE_CON1_USBPHY_REF_CLK_EN_SHIFT)       /* 0x00000200 */
#define CRU_PMU_CLKGATE_CON1_MIPIDSIPHY_REF_CCLK_EN_SHIFT  (10U)
#define CRU_PMU_CLKGATE_CON1_MIPIDSIPHY_REF_CCLK_EN_MASK   (0x1U << CRU_PMU_CLKGATE_CON1_MIPIDSIPHY_REF_CCLK_EN_SHIFT)  /* 0x00000400 */
#define CRU_PMU_CLKGATE_CON1_CLK_PCIEPHY_REF_DIV_CON_SHIFT (11U)
#define CRU_PMU_CLKGATE_CON1_CLK_PCIEPHY_REF_DIV_CON_MASK  (0x1U << CRU_PMU_CLKGATE_CON1_CLK_PCIEPHY_REF_DIV_CON_SHIFT) /* 0x00000800 */
#define CRU_PMU_CLKGATE_CON1_CLK_PCIEPHY_REF_CLK_EN_SHIFT  (12U)
#define CRU_PMU_CLKGATE_CON1_CLK_PCIEPHY_REF_CLK_EN_MASK   (0x1U << CRU_PMU_CLKGATE_CON1_CLK_PCIEPHY_REF_CLK_EN_SHIFT)  /* 0x00001000 */
/* PMU_AUTOCS_CON0 */
#define CRU_PMU_AUTOCS_CON0_OFFSET                         (0x40C0)
#define CRU_PMU_AUTOCS_CON0_PDPMU_IDLE_TH_SHIFT            (0U)
#define CRU_PMU_AUTOCS_CON0_PDPMU_IDLE_TH_MASK             (0xFFFFU << CRU_PMU_AUTOCS_CON0_PDPMU_IDLE_TH_SHIFT)         /* 0x0000FFFF */
#define CRU_PMU_AUTOCS_CON0_PDPMU_WAIT_TH_SHIFT            (16U)
#define CRU_PMU_AUTOCS_CON0_PDPMU_WAIT_TH_MASK             (0xFFFFU << CRU_PMU_AUTOCS_CON0_PDPMU_WAIT_TH_SHIFT)         /* 0xFFFF0000 */
/* PMU_AUTOCS_CON1 */
#define CRU_PMU_AUTOCS_CON1_OFFSET                         (0x40C4)
#define CRU_PMU_AUTOCS_CON1_PDPMU_SWITCH_EN_SHIFT          (0U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_SWITCH_EN_MASK           (0x1U << CRU_PMU_AUTOCS_CON1_PDPMU_SWITCH_EN_SHIFT)          /* 0x00000001 */
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_OUT_SHIFT         (12U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_OUT_MASK          (0x3U << CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_OUT_SHIFT)         /* 0x00003000 */
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_SHIFT             (14U)
#define CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_MASK              (0x3U << CRU_PMU_AUTOCS_CON1_PDPMU_CLKSEL_SHIFT)             /* 0x0000C000 */
/****************************************EFUSE_NS****************************************/
/* MOD */
#define EFUSE_NS_MOD_WORK_MOD_SHIFT                        (0U)
#define EFUSE_NS_MOD_WORK_MOD_MASK                         (0x1U << EFUSE_NS_MOD_WORK_MOD_SHIFT)                        /* 0x00000001 */
#define EFUSE_NS_MOD_CSB_POL_SHIFT                         (1U)
#define EFUSE_NS_MOD_CSB_POL_MASK                          (0x1U << EFUSE_NS_MOD_CSB_POL_SHIFT)                         /* 0x00000002 */
#define EFUSE_NS_MOD_PGENB_POL_SHIFT                       (2U)
#define EFUSE_NS_MOD_PGENB_POL_MASK                        (0x1U << EFUSE_NS_MOD_PGENB_POL_SHIFT)                       /* 0x00000004 */
#define EFUSE_NS_MOD_LOAD_POL_SHIFT                        (3U)
#define EFUSE_NS_MOD_LOAD_POL_MASK                         (0x1U << EFUSE_NS_MOD_LOAD_POL_SHIFT)                        /* 0x00000008 */
#define EFUSE_NS_MOD_STROBE_POL_SHIFT                      (4U)
#define EFUSE_NS_MOD_STROBE_POL_MASK                       (0x1U << EFUSE_NS_MOD_STROBE_POL_SHIFT)                      /* 0x00000010 */
#define EFUSE_NS_MOD_PG_EN_USER_SHIFT                      (5U)
#define EFUSE_NS_MOD_PG_EN_USER_MASK                       (0x1U << EFUSE_NS_MOD_PG_EN_USER_SHIFT)                      /* 0x00000020 */
#define EFUSE_NS_MOD_RD_ENB_USER_SHIFT                     (6U)
#define EFUSE_NS_MOD_RD_ENB_USER_MASK                      (0x1U << EFUSE_NS_MOD_RD_ENB_USER_SHIFT)                     /* 0x00000040 */
/* RD_MASK */
#define EFUSE_NS_RD_MASK_RD_MASK_SHIFT                     (0U)
#define EFUSE_NS_RD_MASK_RD_MASK_MASK                      (0xFFFFFFFFU << EFUSE_NS_RD_MASK_RD_MASK_SHIFT)              /* 0xFFFFFFFF */
/* PG_MASK */
#define EFUSE_NS_PG_MASK_PG_MASK_SHIFT                     (0U)
#define EFUSE_NS_PG_MASK_PG_MASK_MASK                      (0xFFFFFFFFU << EFUSE_NS_PG_MASK_PG_MASK_SHIFT)              /* 0xFFFFFFFF */
/* INT_CON */
#define EFUSE_NS_INT_CON_FINISH_INT_EN_SHIFT               (0U)
#define EFUSE_NS_INT_CON_FINISH_INT_EN_MASK                (0x1U << EFUSE_NS_INT_CON_FINISH_INT_EN_SHIFT)               /* 0x00000001 */
#define EFUSE_NS_INT_CON_AUTO_RD_MASK_INT_EN_SHIFT         (7U)
#define EFUSE_NS_INT_CON_AUTO_RD_MASK_INT_EN_MASK          (0x1U << EFUSE_NS_INT_CON_AUTO_RD_MASK_INT_EN_SHIFT)         /* 0x00000080 */
#define EFUSE_NS_INT_CON_AUTO_PG_MASK_INT_EN_SHIFT         (8U)
#define EFUSE_NS_INT_CON_AUTO_PG_MASK_INT_EN_MASK          (0x1U << EFUSE_NS_INT_CON_AUTO_PG_MASK_INT_EN_SHIFT)         /* 0x00000100 */
#define EFUSE_NS_INT_CON_USER_RD_MASK_INT_EN_SHIFT         (11U)
#define EFUSE_NS_INT_CON_USER_RD_MASK_INT_EN_MASK          (0x1U << EFUSE_NS_INT_CON_USER_RD_MASK_INT_EN_SHIFT)         /* 0x00000800 */
#define EFUSE_NS_INT_CON_USER_PG_MASK_INT_EN_SHIFT         (12U)
#define EFUSE_NS_INT_CON_USER_PG_MASK_INT_EN_MASK          (0x1U << EFUSE_NS_INT_CON_USER_PG_MASK_INT_EN_SHIFT)         /* 0x00001000 */
/* INT_STATUS */
#define EFUSE_NS_INT_STATUS_FINISH_INT_SHIFT               (0U)
#define EFUSE_NS_INT_STATUS_FINISH_INT_MASK                (0x1U << EFUSE_NS_INT_STATUS_FINISH_INT_SHIFT)               /* 0x00000001 */
#define EFUSE_NS_INT_STATUS_AUTO_RD_MASK_INT_SHIFT         (7U)
#define EFUSE_NS_INT_STATUS_AUTO_RD_MASK_INT_MASK          (0x1U << EFUSE_NS_INT_STATUS_AUTO_RD_MASK_INT_SHIFT)         /* 0x00000080 */
#define EFUSE_NS_INT_STATUS_AUTO_PG_MASK_INT_SHIFT         (8U)
#define EFUSE_NS_INT_STATUS_AUTO_PG_MASK_INT_MASK          (0x1U << EFUSE_NS_INT_STATUS_AUTO_PG_MASK_INT_SHIFT)         /* 0x00000100 */
#define EFUSE_NS_INT_STATUS_USER_RD_MASK_INT_SHIFT         (11U)
#define EFUSE_NS_INT_STATUS_USER_RD_MASK_INT_MASK          (0x1U << EFUSE_NS_INT_STATUS_USER_RD_MASK_INT_SHIFT)         /* 0x00000800 */
#define EFUSE_NS_INT_STATUS_USER_PG_MASK_INT_SHIFT         (12U)
#define EFUSE_NS_INT_STATUS_USER_PG_MASK_INT_MASK          (0x1U << EFUSE_NS_INT_STATUS_USER_PG_MASK_INT_SHIFT)         /* 0x00001000 */
/* USER_CTRL */
#define EFUSE_NS_USER_CTRL_CSB_USER_SHIFT                  (0U)
#define EFUSE_NS_USER_CTRL_CSB_USER_MASK                   (0x1U << EFUSE_NS_USER_CTRL_CSB_USER_SHIFT)                  /* 0x00000001 */
#define EFUSE_NS_USER_CTRL_STROBE_USER_SHIFT               (1U)
#define EFUSE_NS_USER_CTRL_STROBE_USER_MASK                (0x1U << EFUSE_NS_USER_CTRL_STROBE_USER_SHIFT)               /* 0x00000002 */
#define EFUSE_NS_USER_CTRL_LOAD_USER_SHIFT                 (2U)
#define EFUSE_NS_USER_CTRL_LOAD_USER_MASK                  (0x1U << EFUSE_NS_USER_CTRL_LOAD_USER_SHIFT)                 /* 0x00000004 */
#define EFUSE_NS_USER_CTRL_PGENB_USER_SHIFT                (3U)
#define EFUSE_NS_USER_CTRL_PGENB_USER_MASK                 (0x1U << EFUSE_NS_USER_CTRL_PGENB_USER_SHIFT)                /* 0x00000008 */
#define EFUSE_NS_USER_CTRL_MR1_SHIFT                       (4U)
#define EFUSE_NS_USER_CTRL_MR1_MASK                        (0x1U << EFUSE_NS_USER_CTRL_MR1_SHIFT)                       /* 0x00000010 */
#define EFUSE_NS_USER_CTRL_MR0_SHIFT                       (5U)
#define EFUSE_NS_USER_CTRL_MR0_MASK                        (0x1U << EFUSE_NS_USER_CTRL_MR0_SHIFT)                       /* 0x00000020 */
#define EFUSE_NS_USER_CTRL_ADDR_USER_SHIFT                 (16U)
#define EFUSE_NS_USER_CTRL_ADDR_USER_MASK                  (0x3FFU << EFUSE_NS_USER_CTRL_ADDR_USER_SHIFT)               /* 0x03FF0000 */
/* DOUT */
#define EFUSE_NS_DOUT_DOUT_SHIFT                           (0U)
#define EFUSE_NS_DOUT_DOUT_MASK                            (0xFFFFFFFFU << EFUSE_NS_DOUT_DOUT_SHIFT)                    /* 0xFFFFFFFF */
/* AUTO_CTRL */
#define EFUSE_NS_AUTO_CTRL_ENB_SHIFT                       (0U)
#define EFUSE_NS_AUTO_CTRL_ENB_MASK                        (0x1U << EFUSE_NS_AUTO_CTRL_ENB_SHIFT)                       /* 0x00000001 */
#define EFUSE_NS_AUTO_CTRL_PG_R_SHIFT                      (1U)
#define EFUSE_NS_AUTO_CTRL_PG_R_MASK                       (0x1U << EFUSE_NS_AUTO_CTRL_PG_R_SHIFT)                      /* 0x00000002 */
#define EFUSE_NS_AUTO_CTRL_ADDR_AUTO_SHIFT                 (16U)
#define EFUSE_NS_AUTO_CTRL_ADDR_AUTO_MASK                  (0x3FFU << EFUSE_NS_AUTO_CTRL_ADDR_AUTO_SHIFT)               /* 0x03FF0000 */
/* T_CSB_P */
#define EFUSE_NS_T_CSB_P_T_CSB_P_L_SHIFT                   (0U)
#define EFUSE_NS_T_CSB_P_T_CSB_P_L_MASK                    (0x7FFU << EFUSE_NS_T_CSB_P_T_CSB_P_L_SHIFT)                 /* 0x000007FF */
#define EFUSE_NS_T_CSB_P_T_CSB_P_S_SHIFT                   (16U)
#define EFUSE_NS_T_CSB_P_T_CSB_P_S_MASK                    (0xFU << EFUSE_NS_T_CSB_P_T_CSB_P_S_SHIFT)                   /* 0x000F0000 */
/* T_PGENB_P */
#define EFUSE_NS_T_PGENB_P_T_PGENB_P_L_SHIFT               (0U)
#define EFUSE_NS_T_PGENB_P_T_PGENB_P_L_MASK                (0x7FFU << EFUSE_NS_T_PGENB_P_T_PGENB_P_L_SHIFT)             /* 0x000007FF */
#define EFUSE_NS_T_PGENB_P_T_PGENB_P_S_SHIFT               (16U)
#define EFUSE_NS_T_PGENB_P_T_PGENB_P_S_MASK                (0xFU << EFUSE_NS_T_PGENB_P_T_PGENB_P_S_SHIFT)               /* 0x000F0000 */
/* T_LOAD_P */
#define EFUSE_NS_T_LOAD_P_T_LOAD_P_L_SHIFT                 (0U)
#define EFUSE_NS_T_LOAD_P_T_LOAD_P_L_MASK                  (0x7FFU << EFUSE_NS_T_LOAD_P_T_LOAD_P_L_SHIFT)               /* 0x000007FF */
#define EFUSE_NS_T_LOAD_P_T_LOAD_P_S_SHIFT                 (16U)
#define EFUSE_NS_T_LOAD_P_T_LOAD_P_S_MASK                  (0xFU << EFUSE_NS_T_LOAD_P_T_LOAD_P_S_SHIFT)                 /* 0x000F0000 */
/* T_ADDR_P */
#define EFUSE_NS_T_ADDR_P_T_ADDR_P_L_SHIFT                 (0U)
#define EFUSE_NS_T_ADDR_P_T_ADDR_P_L_MASK                  (0x7FFU << EFUSE_NS_T_ADDR_P_T_ADDR_P_L_SHIFT)               /* 0x000007FF */
#define EFUSE_NS_T_ADDR_P_T_ADDR_P_S_SHIFT                 (16U)
#define EFUSE_NS_T_ADDR_P_T_ADDR_P_S_MASK                  (0xFU << EFUSE_NS_T_ADDR_P_T_ADDR_P_S_SHIFT)                 /* 0x000F0000 */
/* T_STROBE_P */
#define EFUSE_NS_T_STROBE_P_T_STROBE_P_L_SHIFT             (0U)
#define EFUSE_NS_T_STROBE_P_T_STROBE_P_L_MASK              (0x7FFU << EFUSE_NS_T_STROBE_P_T_STROBE_P_L_SHIFT)           /* 0x000007FF */
#define EFUSE_NS_T_STROBE_P_T_STROBE_P_S_SHIFT             (16U)
#define EFUSE_NS_T_STROBE_P_T_STROBE_P_S_MASK              (0xFFU << EFUSE_NS_T_STROBE_P_T_STROBE_P_S_SHIFT)            /* 0x00FF0000 */
/* T_CSB_R */
#define EFUSE_NS_T_CSB_R_T_CSB_R_L_SHIFT                   (0U)
#define EFUSE_NS_T_CSB_R_T_CSB_R_L_MASK                    (0x3FFU << EFUSE_NS_T_CSB_R_T_CSB_R_L_SHIFT)                 /* 0x000003FF */
#define EFUSE_NS_T_CSB_R_T_CSB_R_S_SHIFT                   (16U)
#define EFUSE_NS_T_CSB_R_T_CSB_R_S_MASK                    (0xFU << EFUSE_NS_T_CSB_R_T_CSB_R_S_SHIFT)                   /* 0x000F0000 */
/* T_PGENB_R */
#define EFUSE_NS_T_PGENB_R_T_PGENB_R_L_SHIFT               (0U)
#define EFUSE_NS_T_PGENB_R_T_PGENB_R_L_MASK                (0x3FFU << EFUSE_NS_T_PGENB_R_T_PGENB_R_L_SHIFT)             /* 0x000003FF */
#define EFUSE_NS_T_PGENB_R_T_PGENB_R_S_SHIFT               (16U)
#define EFUSE_NS_T_PGENB_R_T_PGENB_R_S_MASK                (0xFU << EFUSE_NS_T_PGENB_R_T_PGENB_R_S_SHIFT)               /* 0x000F0000 */
/* T_LOAD_R */
#define EFUSE_NS_T_LOAD_R_T_LOAD_R_L_SHIFT                 (0U)
#define EFUSE_NS_T_LOAD_R_T_LOAD_R_L_MASK                  (0x3FFU << EFUSE_NS_T_LOAD_R_T_LOAD_R_L_SHIFT)               /* 0x000003FF */
#define EFUSE_NS_T_LOAD_R_T_LOAD_R_S_SHIFT                 (16U)
#define EFUSE_NS_T_LOAD_R_T_LOAD_R_S_MASK                  (0xFU << EFUSE_NS_T_LOAD_R_T_LOAD_R_S_SHIFT)                 /* 0x000F0000 */
/* T_ADDR_R */
#define EFUSE_NS_T_ADDR_R_T_ADDR_R_L_SHIFT                 (0U)
#define EFUSE_NS_T_ADDR_R_T_ADDR_R_L_MASK                  (0x3FFU << EFUSE_NS_T_ADDR_R_T_ADDR_R_L_SHIFT)               /* 0x000003FF */
#define EFUSE_NS_T_ADDR_R_T_ADDR_R_S_SHIFT                 (16U)
#define EFUSE_NS_T_ADDR_R_T_ADDR_R_S_MASK                  (0xFU << EFUSE_NS_T_ADDR_R_T_ADDR_R_S_SHIFT)                 /* 0x000F0000 */
/* T_STROBE_R */
#define EFUSE_NS_T_STROBE_R_T_STROBE_R_L_SHIFT             (0U)
#define EFUSE_NS_T_STROBE_R_T_STROBE_R_L_MASK              (0x3FFU << EFUSE_NS_T_STROBE_R_T_STROBE_R_L_SHIFT)           /* 0x000003FF */
#define EFUSE_NS_T_STROBE_R_T_STROBE_R_S_SHIFT             (16U)
#define EFUSE_NS_T_STROBE_R_T_STROBE_R_S_MASK              (0xFU << EFUSE_NS_T_STROBE_R_T_STROBE_R_S_SHIFT)             /* 0x000F0000 */
/* REVISION */
#define EFUSE_NS_REVISION_REVISION_SHIFT                   (0U)
#define EFUSE_NS_REVISION_REVISION_MASK                    (0xFFU << EFUSE_NS_REVISION_REVISION_SHIFT)                  /* 0x000000FF */
/****************************************EFUSE_S*****************************************/
/* MOD */
#define EFUSE_S_MOD_WORK_MOD_SHIFT                         (0U)
#define EFUSE_S_MOD_WORK_MOD_MASK                          (0x1U << EFUSE_S_MOD_WORK_MOD_SHIFT)                         /* 0x00000001 */
#define EFUSE_S_MOD_CSB_POL_SHIFT                          (1U)
#define EFUSE_S_MOD_CSB_POL_MASK                           (0x1U << EFUSE_S_MOD_CSB_POL_SHIFT)                          /* 0x00000002 */
#define EFUSE_S_MOD_PGENB_POL_SHIFT                        (2U)
#define EFUSE_S_MOD_PGENB_POL_MASK                         (0x1U << EFUSE_S_MOD_PGENB_POL_SHIFT)                        /* 0x00000004 */
#define EFUSE_S_MOD_LOAD_POL_SHIFT                         (3U)
#define EFUSE_S_MOD_LOAD_POL_MASK                          (0x1U << EFUSE_S_MOD_LOAD_POL_SHIFT)                         /* 0x00000008 */
#define EFUSE_S_MOD_STROBE_POL_SHIFT                       (4U)
#define EFUSE_S_MOD_STROBE_POL_MASK                        (0x1U << EFUSE_S_MOD_STROBE_POL_SHIFT)                       /* 0x00000010 */
#define EFUSE_S_MOD_PG_EN_USER_SHIFT                       (5U)
#define EFUSE_S_MOD_PG_EN_USER_MASK                        (0x1U << EFUSE_S_MOD_PG_EN_USER_SHIFT)                       /* 0x00000020 */
#define EFUSE_S_MOD_RD_ENB_USER_SHIFT                      (6U)
#define EFUSE_S_MOD_RD_ENB_USER_MASK                       (0x1U << EFUSE_S_MOD_RD_ENB_USER_SHIFT)                      /* 0x00000040 */
/* RD_MASK */
#define EFUSE_S_RD_MASK_RD_MASK_SHIFT                      (0U)
#define EFUSE_S_RD_MASK_RD_MASK_MASK                       (0xFFFFFFFFU << EFUSE_S_RD_MASK_RD_MASK_SHIFT)               /* 0xFFFFFFFF */
/* PG_MASK */
#define EFUSE_S_PG_MASK_PG_MASK_SHIFT                      (0U)
#define EFUSE_S_PG_MASK_PG_MASK_MASK                       (0xFFFFFFFFU << EFUSE_S_PG_MASK_PG_MASK_SHIFT)               /* 0xFFFFFFFF */
/* INT_CON */
#define EFUSE_S_INT_CON_FINISH_INT_EN_SHIFT                (0U)
#define EFUSE_S_INT_CON_FINISH_INT_EN_MASK                 (0x1U << EFUSE_S_INT_CON_FINISH_INT_EN_SHIFT)                /* 0x00000001 */
#define EFUSE_S_INT_CON_AUTO_RD_MASK_INT_EN_SHIFT          (7U)
#define EFUSE_S_INT_CON_AUTO_RD_MASK_INT_EN_MASK           (0x1U << EFUSE_S_INT_CON_AUTO_RD_MASK_INT_EN_SHIFT)          /* 0x00000080 */
#define EFUSE_S_INT_CON_AUTO_PG_MASK_INT_EN_SHIFT          (8U)
#define EFUSE_S_INT_CON_AUTO_PG_MASK_INT_EN_MASK           (0x1U << EFUSE_S_INT_CON_AUTO_PG_MASK_INT_EN_SHIFT)          /* 0x00000100 */
#define EFUSE_S_INT_CON_USER_RD_MASK_INT_EN_SHIFT          (11U)
#define EFUSE_S_INT_CON_USER_RD_MASK_INT_EN_MASK           (0x1U << EFUSE_S_INT_CON_USER_RD_MASK_INT_EN_SHIFT)          /* 0x00000800 */
#define EFUSE_S_INT_CON_USER_PG_MASK_INT_EN_SHIFT          (12U)
#define EFUSE_S_INT_CON_USER_PG_MASK_INT_EN_MASK           (0x1U << EFUSE_S_INT_CON_USER_PG_MASK_INT_EN_SHIFT)          /* 0x00001000 */
/* INT_STATUS */
#define EFUSE_S_INT_STATUS_FINISH_INT_SHIFT                (0U)
#define EFUSE_S_INT_STATUS_FINISH_INT_MASK                 (0x1U << EFUSE_S_INT_STATUS_FINISH_INT_SHIFT)                /* 0x00000001 */
#define EFUSE_S_INT_STATUS_AUTO_RD_MASK_INT_SHIFT          (7U)
#define EFUSE_S_INT_STATUS_AUTO_RD_MASK_INT_MASK           (0x1U << EFUSE_S_INT_STATUS_AUTO_RD_MASK_INT_SHIFT)          /* 0x00000080 */
#define EFUSE_S_INT_STATUS_AUTO_PG_MASK_INT_SHIFT          (8U)
#define EFUSE_S_INT_STATUS_AUTO_PG_MASK_INT_MASK           (0x1U << EFUSE_S_INT_STATUS_AUTO_PG_MASK_INT_SHIFT)          /* 0x00000100 */
#define EFUSE_S_INT_STATUS_USER_RD_MASK_INT_SHIFT          (11U)
#define EFUSE_S_INT_STATUS_USER_RD_MASK_INT_MASK           (0x1U << EFUSE_S_INT_STATUS_USER_RD_MASK_INT_SHIFT)          /* 0x00000800 */
#define EFUSE_S_INT_STATUS_USER_PG_MASK_INT_SHIFT          (12U)
#define EFUSE_S_INT_STATUS_USER_PG_MASK_INT_MASK           (0x1U << EFUSE_S_INT_STATUS_USER_PG_MASK_INT_SHIFT)          /* 0x00001000 */
/* USER_CTRL */
#define EFUSE_S_USER_CTRL_CSB_USER_SHIFT                   (0U)
#define EFUSE_S_USER_CTRL_CSB_USER_MASK                    (0x1U << EFUSE_S_USER_CTRL_CSB_USER_SHIFT)                   /* 0x00000001 */
#define EFUSE_S_USER_CTRL_STROBE_USER_SHIFT                (1U)
#define EFUSE_S_USER_CTRL_STROBE_USER_MASK                 (0x1U << EFUSE_S_USER_CTRL_STROBE_USER_SHIFT)                /* 0x00000002 */
#define EFUSE_S_USER_CTRL_LOAD_USER_SHIFT                  (2U)
#define EFUSE_S_USER_CTRL_LOAD_USER_MASK                   (0x1U << EFUSE_S_USER_CTRL_LOAD_USER_SHIFT)                  /* 0x00000004 */
#define EFUSE_S_USER_CTRL_PGENB_USER_SHIFT                 (3U)
#define EFUSE_S_USER_CTRL_PGENB_USER_MASK                  (0x1U << EFUSE_S_USER_CTRL_PGENB_USER_SHIFT)                 /* 0x00000008 */
#define EFUSE_S_USER_CTRL_MR1_SHIFT                        (4U)
#define EFUSE_S_USER_CTRL_MR1_MASK                         (0x1U << EFUSE_S_USER_CTRL_MR1_SHIFT)                        /* 0x00000010 */
#define EFUSE_S_USER_CTRL_MR0_SHIFT                        (5U)
#define EFUSE_S_USER_CTRL_MR0_MASK                         (0x1U << EFUSE_S_USER_CTRL_MR0_SHIFT)                        /* 0x00000020 */
#define EFUSE_S_USER_CTRL_ADDR_USER_SHIFT                  (16U)
#define EFUSE_S_USER_CTRL_ADDR_USER_MASK                   (0x3FFU << EFUSE_S_USER_CTRL_ADDR_USER_SHIFT)                /* 0x03FF0000 */
/* DOUT */
#define EFUSE_S_DOUT_DOUT_SHIFT                            (0U)
#define EFUSE_S_DOUT_DOUT_MASK                             (0xFFFFFFFFU << EFUSE_S_DOUT_DOUT_SHIFT)                     /* 0xFFFFFFFF */
/* AUTO_CTRL */
#define EFUSE_S_AUTO_CTRL_ENB_SHIFT                        (0U)
#define EFUSE_S_AUTO_CTRL_ENB_MASK                         (0x1U << EFUSE_S_AUTO_CTRL_ENB_SHIFT)                        /* 0x00000001 */
#define EFUSE_S_AUTO_CTRL_PG_R_SHIFT                       (1U)
#define EFUSE_S_AUTO_CTRL_PG_R_MASK                        (0x1U << EFUSE_S_AUTO_CTRL_PG_R_SHIFT)                       /* 0x00000002 */
#define EFUSE_S_AUTO_CTRL_ADDR_AUTO_SHIFT                  (16U)
#define EFUSE_S_AUTO_CTRL_ADDR_AUTO_MASK                   (0x3FFU << EFUSE_S_AUTO_CTRL_ADDR_AUTO_SHIFT)                /* 0x03FF0000 */
/* T_CSB_P */
#define EFUSE_S_T_CSB_P_T_CSB_P_L_SHIFT                    (0U)
#define EFUSE_S_T_CSB_P_T_CSB_P_L_MASK                     (0x7FFU << EFUSE_S_T_CSB_P_T_CSB_P_L_SHIFT)                  /* 0x000007FF */
#define EFUSE_S_T_CSB_P_T_CSB_P_S_SHIFT                    (16U)
#define EFUSE_S_T_CSB_P_T_CSB_P_S_MASK                     (0xFU << EFUSE_S_T_CSB_P_T_CSB_P_S_SHIFT)                    /* 0x000F0000 */
/* T_PGENB_P */
#define EFUSE_S_T_PGENB_P_T_PGENB_P_L_SHIFT                (0U)
#define EFUSE_S_T_PGENB_P_T_PGENB_P_L_MASK                 (0x7FFU << EFUSE_S_T_PGENB_P_T_PGENB_P_L_SHIFT)              /* 0x000007FF */
#define EFUSE_S_T_PGENB_P_T_PGENB_P_S_SHIFT                (16U)
#define EFUSE_S_T_PGENB_P_T_PGENB_P_S_MASK                 (0xFU << EFUSE_S_T_PGENB_P_T_PGENB_P_S_SHIFT)                /* 0x000F0000 */
/* T_LOAD_P */
#define EFUSE_S_T_LOAD_P_T_LOAD_P_L_SHIFT                  (0U)
#define EFUSE_S_T_LOAD_P_T_LOAD_P_L_MASK                   (0x7FFU << EFUSE_S_T_LOAD_P_T_LOAD_P_L_SHIFT)                /* 0x000007FF */
#define EFUSE_S_T_LOAD_P_T_LOAD_P_S_SHIFT                  (16U)
#define EFUSE_S_T_LOAD_P_T_LOAD_P_S_MASK                   (0xFU << EFUSE_S_T_LOAD_P_T_LOAD_P_S_SHIFT)                  /* 0x000F0000 */
/* T_ADDR_P */
#define EFUSE_S_T_ADDR_P_T_ADDR_P_L_SHIFT                  (0U)
#define EFUSE_S_T_ADDR_P_T_ADDR_P_L_MASK                   (0x7FFU << EFUSE_S_T_ADDR_P_T_ADDR_P_L_SHIFT)                /* 0x000007FF */
#define EFUSE_S_T_ADDR_P_T_ADDR_P_S_SHIFT                  (16U)
#define EFUSE_S_T_ADDR_P_T_ADDR_P_S_MASK                   (0xFU << EFUSE_S_T_ADDR_P_T_ADDR_P_S_SHIFT)                  /* 0x000F0000 */
/* T_STROBE_P */
#define EFUSE_S_T_STROBE_P_T_STROBE_P_L_SHIFT              (0U)
#define EFUSE_S_T_STROBE_P_T_STROBE_P_L_MASK               (0x7FFU << EFUSE_S_T_STROBE_P_T_STROBE_P_L_SHIFT)            /* 0x000007FF */
#define EFUSE_S_T_STROBE_P_T_STROBE_P_S_SHIFT              (16U)
#define EFUSE_S_T_STROBE_P_T_STROBE_P_S_MASK               (0xFFU << EFUSE_S_T_STROBE_P_T_STROBE_P_S_SHIFT)             /* 0x00FF0000 */
/* T_CSB_R */
#define EFUSE_S_T_CSB_R_T_CSB_R_L_SHIFT                    (0U)
#define EFUSE_S_T_CSB_R_T_CSB_R_L_MASK                     (0x3FFU << EFUSE_S_T_CSB_R_T_CSB_R_L_SHIFT)                  /* 0x000003FF */
#define EFUSE_S_T_CSB_R_T_CSB_R_S_SHIFT                    (16U)
#define EFUSE_S_T_CSB_R_T_CSB_R_S_MASK                     (0xFU << EFUSE_S_T_CSB_R_T_CSB_R_S_SHIFT)                    /* 0x000F0000 */
/* T_PGENB_R */
#define EFUSE_S_T_PGENB_R_T_PGENB_R_L_SHIFT                (0U)
#define EFUSE_S_T_PGENB_R_T_PGENB_R_L_MASK                 (0x3FFU << EFUSE_S_T_PGENB_R_T_PGENB_R_L_SHIFT)              /* 0x000003FF */
#define EFUSE_S_T_PGENB_R_T_PGENB_R_S_SHIFT                (16U)
#define EFUSE_S_T_PGENB_R_T_PGENB_R_S_MASK                 (0xFU << EFUSE_S_T_PGENB_R_T_PGENB_R_S_SHIFT)                /* 0x000F0000 */
/* T_LOAD_R */
#define EFUSE_S_T_LOAD_R_T_LOAD_R_L_SHIFT                  (0U)
#define EFUSE_S_T_LOAD_R_T_LOAD_R_L_MASK                   (0x3FFU << EFUSE_S_T_LOAD_R_T_LOAD_R_L_SHIFT)                /* 0x000003FF */
#define EFUSE_S_T_LOAD_R_T_LOAD_R_S_SHIFT                  (16U)
#define EFUSE_S_T_LOAD_R_T_LOAD_R_S_MASK                   (0xFU << EFUSE_S_T_LOAD_R_T_LOAD_R_S_SHIFT)                  /* 0x000F0000 */
/* T_ADDR_R */
#define EFUSE_S_T_ADDR_R_T_ADDR_R_L_SHIFT                  (0U)
#define EFUSE_S_T_ADDR_R_T_ADDR_R_L_MASK                   (0x3FFU << EFUSE_S_T_ADDR_R_T_ADDR_R_L_SHIFT)                /* 0x000003FF */
#define EFUSE_S_T_ADDR_R_T_ADDR_R_S_SHIFT                  (16U)
#define EFUSE_S_T_ADDR_R_T_ADDR_R_S_MASK                   (0xFU << EFUSE_S_T_ADDR_R_T_ADDR_R_S_SHIFT)                  /* 0x000F0000 */
/* T_STROBE_R */
#define EFUSE_S_T_STROBE_R_T_STROBE_R_L_SHIFT              (0U)
#define EFUSE_S_T_STROBE_R_T_STROBE_R_L_MASK               (0x3FFU << EFUSE_S_T_STROBE_R_T_STROBE_R_L_SHIFT)            /* 0x000003FF */
#define EFUSE_S_T_STROBE_R_T_STROBE_R_S_SHIFT              (16U)
#define EFUSE_S_T_STROBE_R_T_STROBE_R_S_MASK               (0xFU << EFUSE_S_T_STROBE_R_T_STROBE_R_S_SHIFT)              /* 0x000F0000 */
/* REVISION */
#define EFUSE_S_REVISION_REVISION_SHIFT                    (0U)
#define EFUSE_S_REVISION_REVISION_MASK                     (0xFFU << EFUSE_S_REVISION_REVISION_SHIFT)                   /* 0x000000FF */
/*****************************************SARADC*****************************************/
/* DATA */
#define SARADC_DATA_ADC_DATA_SHIFT                         (0U)
#define SARADC_DATA_ADC_DATA_MASK                          (0x3FFU << SARADC_DATA_ADC_DATA_SHIFT)                       /* 0x000003FF */
/* STAS */
#define SARADC_STAS_ADC_STATUS_SHIFT                       (0U)
#define SARADC_STAS_ADC_STATUS_MASK                        (0x1U << SARADC_STAS_ADC_STATUS_SHIFT)                       /* 0x00000001 */
/* CTRL */
#define SARADC_CTRL_ADC_INPUT_SRC_SEL_SHIFT                (0U)
#define SARADC_CTRL_ADC_INPUT_SRC_SEL_MASK                 (0x7U << SARADC_CTRL_ADC_INPUT_SRC_SEL_SHIFT)                /* 0x00000007 */
#define SARADC_CTRL_ADC_POWER_CTRL_SHIFT                   (3U)
#define SARADC_CTRL_ADC_POWER_CTRL_MASK                    (0x1U << SARADC_CTRL_ADC_POWER_CTRL_SHIFT)                   /* 0x00000008 */
#define SARADC_CTRL_INT_EN_SHIFT                           (5U)
#define SARADC_CTRL_INT_EN_MASK                            (0x1U << SARADC_CTRL_INT_EN_SHIFT)                           /* 0x00000020 */
#define SARADC_CTRL_INT_STATUS_SHIFT                       (6U)
#define SARADC_CTRL_INT_STATUS_MASK                        (0x1U << SARADC_CTRL_INT_STATUS_SHIFT)                       /* 0x00000040 */
/* DLY_PU_SOC */
#define SARADC_DLY_PU_SOC_DLY_PU_SOC_SHIFT                 (0U)
#define SARADC_DLY_PU_SOC_DLY_PU_SOC_MASK                  (0x3FU << SARADC_DLY_PU_SOC_DLY_PU_SOC_SHIFT)                /* 0x0000003F */
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
/******************************************DCF*******************************************/
/* DCF_CTRL */
#define DCF_DCF_CTRL_START_SHIFT                           (0U)
#define DCF_DCF_CTRL_START_MASK                            (0x1U << DCF_DCF_CTRL_START_SHIFT)                           /* 0x00000001 */
#define DCF_DCF_CTRL_TIMEOUT_EN_SHIFT                      (1U)
#define DCF_DCF_CTRL_TIMEOUT_EN_MASK                       (0x1U << DCF_DCF_CTRL_TIMEOUT_EN_SHIFT)                      /* 0x00000002 */
#define DCF_DCF_CTRL_VOP_HW_EN_SHIFT                       (2U)
#define DCF_DCF_CTRL_VOP_HW_EN_MASK                        (0x1U << DCF_DCF_CTRL_VOP_HW_EN_SHIFT)                       /* 0x00000004 */
/* DCF_STATUS */
#define DCF_DCF_STATUS_DCF_IDLE_ST_SHIFT                   (0U)
#define DCF_DCF_STATUS_DCF_IDLE_ST_MASK                    (0x1U << DCF_DCF_STATUS_DCF_IDLE_ST_SHIFT)                   /* 0x00000001 */
#define DCF_DCF_STATUS_DCF_LEVEL_TRIGGER_ST_SHIFT          (1U)
#define DCF_DCF_STATUS_DCF_LEVEL_TRIGGER_ST_MASK           (0x1U << DCF_DCF_STATUS_DCF_LEVEL_TRIGGER_ST_SHIFT)          /* 0x00000002 */
#define DCF_DCF_STATUS_DCF_EDGE_TRIGGER_ST_SHIFT           (2U)
#define DCF_DCF_STATUS_DCF_EDGE_TRIGGER_ST_MASK            (0x1U << DCF_DCF_STATUS_DCF_EDGE_TRIGGER_ST_SHIFT)           /* 0x00000004 */
#define DCF_DCF_STATUS_DCF_TIMEOUT_ST_SHIFT                (3U)
#define DCF_DCF_STATUS_DCF_TIMEOUT_ST_MASK                 (0x1U << DCF_DCF_STATUS_DCF_TIMEOUT_ST_SHIFT)                /* 0x00000008 */
#define DCF_DCF_STATUS_INSTR_ERROR_ST_SHIFT                (4U)
#define DCF_DCF_STATUS_INSTR_ERROR_ST_MASK                 (0x1U << DCF_DCF_STATUS_INSTR_ERROR_ST_SHIFT)                /* 0x00000010 */
#define DCF_DCF_STATUS_DMA_ERROR_ST_SHIFT                  (5U)
#define DCF_DCF_STATUS_DMA_ERROR_ST_MASK                   (0x1U << DCF_DCF_STATUS_DMA_ERROR_ST_SHIFT)                  /* 0x00000020 */
#define DCF_DCF_STATUS_INSTR_DONE_ST_SHIFT                 (6U)
#define DCF_DCF_STATUS_INSTR_DONE_ST_MASK                  (0x1U << DCF_DCF_STATUS_INSTR_DONE_ST_SHIFT)                 /* 0x00000040 */
#define DCF_DCF_STATUS_DMA_DONE_ST_SHIFT                   (7U)
#define DCF_DCF_STATUS_DMA_DONE_ST_MASK                    (0x1U << DCF_DCF_STATUS_DMA_DONE_ST_SHIFT)                   /* 0x00000080 */
/* DCF_ADDR */
#define DCF_DCF_ADDR_ADDR_SHIFT                            (0U)
#define DCF_DCF_ADDR_ADDR_MASK                             (0xFFFFFFFFU << DCF_DCF_ADDR_ADDR_SHIFT)                     /* 0xFFFFFFFF */
/* DCF_ISR */
#define DCF_DCF_ISR_DCF_DONE_SHIFT                         (0U)
#define DCF_DCF_ISR_DCF_DONE_MASK                          (0x1U << DCF_DCF_ISR_DCF_DONE_SHIFT)                         /* 0x00000001 */
#define DCF_DCF_ISR_DCF_ERROR_SHIFT                        (1U)
#define DCF_DCF_ISR_DCF_ERROR_MASK                         (0x1U << DCF_DCF_ISR_DCF_ERROR_SHIFT)                        /* 0x00000002 */
/* DCF_TIMEOUT_CYC */
#define DCF_DCF_TIMEOUT_CYC_TIMEOUT_SHIFT                  (0U)
#define DCF_DCF_TIMEOUT_CYC_TIMEOUT_MASK                   (0xFFFFFFFFU << DCF_DCF_TIMEOUT_CYC_TIMEOUT_SHIFT)           /* 0xFFFFFFFF */
/* DCF_CURR_R0 */
#define DCF_DCF_CURR_R0_CURR_R0_SHIFT                      (0U)
#define DCF_DCF_CURR_R0_CURR_R0_MASK                       (0xFFFFFFFFU << DCF_DCF_CURR_R0_CURR_R0_SHIFT)               /* 0xFFFFFFFF */
/* DCF_CURR_R1 */
#define DCF_DCF_CURR_R1_CURR_R1_SHIFT                      (0U)
#define DCF_DCF_CURR_R1_CURR_R1_MASK                       (0xFFFFFFFFU << DCF_DCF_CURR_R1_CURR_R1_SHIFT)               /* 0xFFFFFFFF */
/* DCF_CMD_COUNTER */
#define DCF_DCF_CMD_COUNTER_CMD_COUNTER_SHIFT              (0U)
#define DCF_DCF_CMD_COUNTER_CMD_COUNTER_MASK               (0xFFFFFFFFU << DCF_DCF_CMD_COUNTER_CMD_COUNTER_SHIFT)       /* 0xFFFFFFFF */
/* DCF_LAST_ADDR1 */
#define DCF_DCF_LAST_ADDR1_LAST_ADDR1_SHIFT                (0U)
#define DCF_DCF_LAST_ADDR1_LAST_ADDR1_MASK                 (0xFFFFFFFFU << DCF_DCF_LAST_ADDR1_LAST_ADDR1_SHIFT)         /* 0xFFFFFFFF */
/* DCF_LAST_ADDR2 */
#define DCF_DCF_LAST_ADDR2_LAST_ADDR2_SHIFT                (0U)
#define DCF_DCF_LAST_ADDR2_LAST_ADDR2_MASK                 (0xFFFFFFFFU << DCF_DCF_LAST_ADDR2_LAST_ADDR2_SHIFT)         /* 0xFFFFFFFF */
/* DCF_LAST_ADDR3 */
#define DCF_DCF_LAST_ADDR3_LAST_ADDR3_SHIFT                (0U)
#define DCF_DCF_LAST_ADDR3_LAST_ADDR3_MASK                 (0xFFFFFFFFU << DCF_DCF_LAST_ADDR3_LAST_ADDR3_SHIFT)         /* 0xFFFFFFFF */
/* DCF_LAST_ADDR4 */
#define DCF_DCF_LAST_ADDR4_LAST_ADDR4_SHIFT                (0U)
#define DCF_DCF_LAST_ADDR4_LAST_ADDR4_MASK                 (0xFFFFFFFFU << DCF_DCF_LAST_ADDR4_LAST_ADDR4_SHIFT)         /* 0xFFFFFFFF */
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
/* FIFO */
#define PWM_FIFO_CYCLE_CNT_SHIFT                           (0U)
#define PWM_FIFO_CYCLE_CNT_MASK                            (0x7FFFFFFFU << PWM_FIFO_CYCLE_CNT_SHIFT)                    /* 0x7FFFFFFF */
#define PWM_FIFO_POL_SHIFT                                 (31U)
#define PWM_FIFO_POL_MASK                                  (0x1U << PWM_FIFO_POL_SHIFT)                                 /* 0x80000000 */
/* PWRMATCH_CTRL */
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_ENABLE_SHIFT          (0U)
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_ENABLE_MASK           (0x1U << PWM_PWRMATCH_CTRL_CH0_PWRKEY_ENABLE_SHIFT)          /* 0x00000001 */
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_ENABLE_SHIFT          (1U)
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_ENABLE_MASK           (0x1U << PWM_PWRMATCH_CTRL_CH1_PWRKEY_ENABLE_SHIFT)          /* 0x00000002 */
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_ENABLE_SHIFT          (2U)
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_ENABLE_MASK           (0x1U << PWM_PWRMATCH_CTRL_CH2_PWRKEY_ENABLE_SHIFT)          /* 0x00000004 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_SHIFT          (3U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_MASK           (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_ENABLE_SHIFT)          /* 0x00000008 */
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_POLARITY_SHIFT        (4U)
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_POLARITY_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH0_PWRKEY_POLARITY_SHIFT)        /* 0x00000010 */
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_POLARITY_SHIFT        (5U)
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_POLARITY_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH1_PWRKEY_POLARITY_SHIFT)        /* 0x00000020 */
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_POLARITY_SHIFT        (6U)
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_POLARITY_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH2_PWRKEY_POLARITY_SHIFT)        /* 0x00000040 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_SHIFT        (7U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_POLARITY_SHIFT)        /* 0x00000080 */
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_CAPTURE_CTRL_SHIFT    (8U)
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_CAPTURE_CTRL_MASK     (0x1U << PWM_PWRMATCH_CTRL_CH0_PWRKEY_CAPTURE_CTRL_SHIFT)    /* 0x00000100 */
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_CAPTURE_CTRL_SHIFT    (9U)
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_CAPTURE_CTRL_MASK     (0x1U << PWM_PWRMATCH_CTRL_CH1_PWRKEY_CAPTURE_CTRL_SHIFT)    /* 0x00000200 */
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_CAPTURE_CTRL_SHIFT    (10U)
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_CAPTURE_CTRL_MASK     (0x1U << PWM_PWRMATCH_CTRL_CH2_PWRKEY_CAPTURE_CTRL_SHIFT)    /* 0x00000400 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_SHIFT    (11U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_MASK     (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_CAPTURE_CTRL_SHIFT)    /* 0x00000800 */
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_INT_CTRL_SHIFT        (12U)
#define PWM_PWRMATCH_CTRL_CH0_PWRKEY_INT_CTRL_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH0_PWRKEY_INT_CTRL_SHIFT)        /* 0x00001000 */
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_INT_CTRL_SHIFT        (13U)
#define PWM_PWRMATCH_CTRL_CH1_PWRKEY_INT_CTRL_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH1_PWRKEY_INT_CTRL_SHIFT)        /* 0x00002000 */
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_INT_CTRL_SHIFT        (14U)
#define PWM_PWRMATCH_CTRL_CH2_PWRKEY_INT_CTRL_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH2_PWRKEY_INT_CTRL_SHIFT)        /* 0x00004000 */
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_SHIFT        (15U)
#define PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_MASK         (0x1U << PWM_PWRMATCH_CTRL_CH3_PWRKEY_INT_CTRL_SHIFT)        /* 0x00008000 */
/* PWRMATCH_LPRE */
#define PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT                    (0U)
#define PWM_PWRMATCH_LPRE_CNT_MIN_MASK                     (0xFFFFU << PWM_PWRMATCH_LPRE_CNT_MIN_SHIFT)                 /* 0x0000FFFF */
#define PWM_PWRMATCH_LPRE_CNT_MAX_SHIFT                    (16U)
#define PWM_PWRMATCH_LPRE_CNT_MAX_MASK                     (0xFFFFU << PWM_PWRMATCH_LPRE_CNT_MAX_SHIFT)                 /* 0xFFFF0000 */
/* PWM_PWRMATCH_HPRE */
#define PWM_PWM_PWRMATCH_HPRE_CNT_MIN_SHIFT                (0U)
#define PWM_PWM_PWRMATCH_HPRE_CNT_MIN_MASK                 (0xFFFFU << PWM_PWM_PWRMATCH_HPRE_CNT_MIN_SHIFT)             /* 0x0000FFFF */
#define PWM_PWM_PWRMATCH_HPRE_CNT_MAX_SHIFT                (16U)
#define PWM_PWM_PWRMATCH_HPRE_CNT_MAX_MASK                 (0xFFFFU << PWM_PWM_PWRMATCH_HPRE_CNT_MAX_SHIFT)             /* 0xFFFF0000 */
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
/* PWM0_PWRCAPTURE_VALUE */
#define PWM_PWM0_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT (0U)
#define PWM_PWM0_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_MASK (0xFFFFFFFFU << PWM_PWM0_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWM1_PWRCAPTURE_VALUE */
#define PWM_PWM1_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT (0U)
#define PWM_PWM1_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_MASK (0xFFFFFFFFU << PWM_PWM1_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT) /* 0xFFFFFFFF */
/* PWM2_PWRCAPTURE_VALUE */
#define PWM_PWM2_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT (0U)
#define PWM_PWM2_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_MASK (0xFFFFFFFFU << PWM_PWM2_PWRCAPTURE_VALUE_PWRKEY_CAPTURE_VALUE_SHIFT) /* 0xFFFFFFFF */
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
/******************************************UART******************************************/
/* RBR */
#define UART_RBR_DATA_INPUT_SHIFT                          (0U)
#define UART_RBR_DATA_INPUT_MASK                           (0xFFU << UART_RBR_DATA_INPUT_SHIFT)                         /* 0x000000FF */
/* THR */
#define UART_THR_DATA_OUTPUT_SHIFT                         (0U)
#define UART_THR_DATA_OUTPUT_MASK                          (0xFFU << UART_THR_DATA_OUTPUT_SHIFT)                        /* 0x000000FF */
/* DLL */
#define UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT                 (0U)
#define UART_DLL_BAUD_RATE_DIVISOR_L_MASK                  (0xFFU << UART_DLL_BAUD_RATE_DIVISOR_L_SHIFT)                /* 0x000000FF */
/* DLH */
#define UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT                 (0U)
#define UART_DLH_BAUD_RATE_DIVISOR_H_MASK                  (0xFFU << UART_DLH_BAUD_RATE_DIVISOR_H_SHIFT)                /* 0x000000FF */
/* IER */
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
#define UART_IIR_INT_ID_SHIFT                              (0U)
#define UART_IIR_INT_ID_MASK                               (0xFU << UART_IIR_INT_ID_SHIFT)                              /* 0x0000000F */
#define UART_IIR_FIFOS_EN_SHIFT                            (6U)
#define UART_IIR_FIFOS_EN_MASK                             (0x3U << UART_IIR_FIFOS_EN_SHIFT)                            /* 0x000000C0 */
/* FCR */
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
#define UART_SCR_TEMP_STORE_SPACE_SHIFT                    (0U)
#define UART_SCR_TEMP_STORE_SPACE_MASK                     (0xFFU << UART_SCR_TEMP_STORE_SPACE_SHIFT)                   /* 0x000000FF */
/* SRBR */
#define UART_SRBR_SHADOW_RBR_SHIFT                         (0U)
#define UART_SRBR_SHADOW_RBR_MASK                          (0xFFU << UART_SRBR_SHADOW_RBR_SHIFT)                        /* 0x000000FF */
/* STHR */
#define UART_STHR_SHADOW_THR_SHIFT                         (0U)
#define UART_STHR_SHADOW_THR_MASK                          (0xFFU << UART_STHR_SHADOW_THR_SHIFT)                        /* 0x000000FF */
/* FAR */
#define UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT                 (0U)
#define UART_FAR_FIFO_ACCESS_TEST_EN_MASK                  (0x1U << UART_FAR_FIFO_ACCESS_TEST_EN_SHIFT)                 /* 0x00000001 */
/* TFR */
#define UART_TFR_TRANS_FIFO_READ_SHIFT                     (0U)
#define UART_TFR_TRANS_FIFO_READ_MASK                      (0xFFU << UART_TFR_TRANS_FIFO_READ_SHIFT)                    /* 0x000000FF */
/* RFW */
#define UART_RFW_RECEIVE_FIFO_WRITE_SHIFT                  (0U)
#define UART_RFW_RECEIVE_FIFO_WRITE_MASK                   (0xFFU << UART_RFW_RECEIVE_FIFO_WRITE_SHIFT)                 /* 0x000000FF */
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT           (8U)
#define UART_RFW_RECEIVE_FIFO_PARITY_ERROR_MASK            (0x1U << UART_RFW_RECEIVE_FIFO_PARITY_ERROR_SHIFT)           /* 0x00000100 */
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT          (9U)
#define UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_MASK           (0x1U << UART_RFW_RECEIVE_FIFO_FRAMING_ERROR_SHIFT)          /* 0x00000200 */
/* USR */
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
#define UART_TFL_TRANS_FIFO_LEVEL_SHIFT                    (0U)
#define UART_TFL_TRANS_FIFO_LEVEL_MASK                     (0x1FU << UART_TFL_TRANS_FIFO_LEVEL_SHIFT)                   /* 0x0000001F */
/* RFL */
#define UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT                  (0U)
#define UART_RFL_RECEIVE_FIFO_LEVEL_MASK                   (0x1FU << UART_RFL_RECEIVE_FIFO_LEVEL_SHIFT)                 /* 0x0000001F */
/* SRR */
#define UART_SRR_UART_RESET_SHIFT                          (0U)
#define UART_SRR_UART_RESET_MASK                           (0x1U << UART_SRR_UART_RESET_SHIFT)                          /* 0x00000001 */
#define UART_SRR_RCVR_FIFO_RESET_SHIFT                     (1U)
#define UART_SRR_RCVR_FIFO_RESET_MASK                      (0x1U << UART_SRR_RCVR_FIFO_RESET_SHIFT)                     /* 0x00000002 */
#define UART_SRR_XMIT_FIFO_RESET_SHIFT                     (2U)
#define UART_SRR_XMIT_FIFO_RESET_MASK                      (0x1U << UART_SRR_XMIT_FIFO_RESET_SHIFT)                     /* 0x00000004 */
/* SRTS */
#define UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT                 (0U)
#define UART_SRTS_SHADOW_REQ_TO_SEND_MASK                  (0x1U << UART_SRTS_SHADOW_REQ_TO_SEND_SHIFT)                 /* 0x00000001 */
/* SBCR */
#define UART_SBCR_SHADOW_BREAK_CTRL_SHIFT                  (0U)
#define UART_SBCR_SHADOW_BREAK_CTRL_MASK                   (0x1U << UART_SBCR_SHADOW_BREAK_CTRL_SHIFT)                  /* 0x00000001 */
/* SDMAM */
#define UART_SDMAM_SHADOW_DMA_MODE_SHIFT                   (0U)
#define UART_SDMAM_SHADOW_DMA_MODE_MASK                    (0x1U << UART_SDMAM_SHADOW_DMA_MODE_SHIFT)                   /* 0x00000001 */
/* SFE */
#define UART_SFE_SHADOW_FIFO_EN_SHIFT                      (0U)
#define UART_SFE_SHADOW_FIFO_EN_MASK                       (0x1U << UART_SFE_SHADOW_FIFO_EN_SHIFT)                      /* 0x00000001 */
/* SRT */
#define UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT                 (0U)
#define UART_SRT_SHADOW_RCVR_TRIGGER_MASK                  (0x3U << UART_SRT_SHADOW_RCVR_TRIGGER_SHIFT)                 /* 0x00000003 */
/* STET */
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT            (0U)
#define UART_STET_SHADOW_TX_EMPTY_TRIGGER_MASK             (0x3U << UART_STET_SHADOW_TX_EMPTY_TRIGGER_SHIFT)            /* 0x00000003 */
/* HTX */
#define UART_HTX_HALT_TX_EN_SHIFT                          (0U)
#define UART_HTX_HALT_TX_EN_MASK                           (0x1U << UART_HTX_HALT_TX_EN_SHIFT)                          /* 0x00000001 */
/* DMASA */
#define UART_DMASA_DMA_SOFTWARE_ACK_SHIFT                  (0U)
#define UART_DMASA_DMA_SOFTWARE_ACK_MASK                   (0x1U << UART_DMASA_DMA_SOFTWARE_ACK_SHIFT)                  /* 0x00000001 */
/* CPR */
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
#define UART_UCV_VER_SHIFT                                 (0U)
#define UART_UCV_VER_MASK                                  (0xFFFFFFFFU << UART_UCV_VER_SHIFT)                          /* 0xFFFFFFFF */
/* CTR */
#define UART_CTR_PERIPHERAL_ID_SHIFT                       (0U)
#define UART_CTR_PERIPHERAL_ID_MASK                        (0xFFFFFFFFU << UART_CTR_PERIPHERAL_ID_SHIFT)                /* 0xFFFFFFFF */
/******************************************MMC*******************************************/
/* CTRL */
#define MMC_CTRL_CONTROLLER_RESET_SHIFT              (0U)
#define MMC_CTRL_CONTROLLER_RESET_MASK               (0x1U << MMC_CTRL_CONTROLLER_RESET_SHIFT)                    /* 0x00000001 */
#define MMC_CTRL_FIFO_RESET_SHIFT                    (1U)
#define MMC_CTRL_FIFO_RESET_MASK                     (0x1U << MMC_CTRL_FIFO_RESET_SHIFT)                          /* 0x00000002 */
#define MMC_CTRL_DMA_RESET_SHIFT                     (2U)
#define MMC_CTRL_DMA_RESET_MASK                      (0x1U << MMC_CTRL_DMA_RESET_SHIFT)                           /* 0x00000004 */
#define MMC_CTRL_INT_ENABLE_SHIFT                    (4U)
#define MMC_CTRL_INT_ENABLE_MASK                     (0x1U << MMC_CTRL_INT_ENABLE_SHIFT)                          /* 0x00000010 */
#define MMC_CTRL_DMA_ENABLE_SHIFT                    (5U)
#define MMC_CTRL_DMA_ENABLE_MASK                     (0x1U << MMC_CTRL_DMA_ENABLE_SHIFT)                          /* 0x00000020 */
#define MMC_CTRL_READ_WAIT_SHIFT                     (6U)
#define MMC_CTRL_READ_WAIT_MASK                      (0x1U << MMC_CTRL_READ_WAIT_SHIFT)                           /* 0x00000040 */
#define MMC_CTRL_SEND_IRQ_RESPONSE_SHIFT             (7U)
#define MMC_CTRL_SEND_IRQ_RESPONSE_MASK              (0x1U << MMC_CTRL_SEND_IRQ_RESPONSE_SHIFT)                   /* 0x00000080 */
#define MMC_CTRL_ABORT_READ_DATA_SHIFT               (8U)
#define MMC_CTRL_ABORT_READ_DATA_MASK                (0x1U << MMC_CTRL_ABORT_READ_DATA_SHIFT)                     /* 0x00000100 */
#define MMC_CTRL_SEND_CCSD_SHIFT                     (9U)
#define MMC_CTRL_SEND_CCSD_MASK                      (0x1U << MMC_CTRL_SEND_CCSD_SHIFT)                           /* 0x00000200 */
#define MMC_CTRL_SEND_AUTO_STOP_CCSD_SHIFT           (10U)
#define MMC_CTRL_SEND_AUTO_STOP_CCSD_MASK            (0x1U << MMC_CTRL_SEND_AUTO_STOP_CCSD_SHIFT)                 /* 0x00000400 */
#define MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS_SHIFT (11U)
#define MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS_MASK  (0x1U << MMC_CTRL_CEATA_DEVICE_INTERRUPT_STATUS_SHIFT)       /* 0x00000800 */
#define MMC_CTRL_USE_INTERNAL_DMAC_SHIFT             (25U)
#define MMC_CTRL_USE_INTERNAL_DMAC_MASK              (0x1U << MMC_CTRL_USE_INTERNAL_DMAC_SHIFT)                   /* 0x02000000 */
/* PWREN */
#define MMC_PWREN_POWER_ENABLE_SHIFT (0U)
#define MMC_PWREN_POWER_ENABLE_MASK  (0x1U << MMC_PWREN_POWER_ENABLE_SHIFT)                       /* 0x00000001 */
/* CLKDIV */
#define MMC_CLKDIV_CLK_DIVIDER0_SHIFT (0U)
#define MMC_CLKDIV_CLK_DIVIDER0_MASK  (0xFFU << MMC_CLKDIV_CLK_DIVIDER0_SHIFT)                     /* 0x000000FF */
/* CLKSRC */
#define MMC_CLKSRC_CLK_SOURCE_SHIFT (0U)
#define MMC_CLKSRC_CLK_SOURCE_MASK  (0x3U << MMC_CLKSRC_CLK_SOURCE_SHIFT)                        /* 0x00000003 */
/* CLKENA */
#define MMC_CLKENA_CCLK_ENABLE_SHIFT    (0U)
#define MMC_CLKENA_CCLK_ENABLE_MASK     (0x1U << MMC_CLKENA_CCLK_ENABLE_SHIFT)                       /* 0x00000001 */
#define MMC_CLKENA_CCLK_LOW_POWER_SHIFT (16U)
#define MMC_CLKENA_CCLK_LOW_POWER_MASK  (0x1U << MMC_CLKENA_CCLK_LOW_POWER_SHIFT)                    /* 0x00010000 */
/* TMOUT */
#define MMC_TMOUT_RESPONSE_TIMEOUT_SHIFT (0U)
#define MMC_TMOUT_RESPONSE_TIMEOUT_MASK  (0xFFU << MMC_TMOUT_RESPONSE_TIMEOUT_SHIFT)                  /* 0x000000FF */
#define MMC_TMOUT_DATA_TIMEOUT_SHIFT     (8U)
#define MMC_TMOUT_DATA_TIMEOUT_MASK      (0xFFFFFFU << MMC_TMOUT_DATA_TIMEOUT_SHIFT)                  /* 0xFFFFFF00 */
/* CTYPE */
#define MMC_CTYPE_CARD_WIDTH_SHIFT   (0U)
#define MMC_CTYPE_CARD_WIDTH_MASK    (0x1U << MMC_CTYPE_CARD_WIDTH_SHIFT)                         /* 0x00000001 */
#define MMC_CTYPE_CARD_WIDTH_8_SHIFT (16U)
#define MMC_CTYPE_CARD_WIDTH_8_MASK  (0x1U << MMC_CTYPE_CARD_WIDTH_8_SHIFT)                       /* 0x00010000 */
/* BLKSIZ */
#define MMC_BLKSIZ_BLOCK_SIZE_SHIFT (0U)
#define MMC_BLKSIZ_BLOCK_SIZE_MASK  (0xFFFFU << MMC_BLKSIZ_BLOCK_SIZE_SHIFT)                     /* 0x0000FFFF */
/* BYTCNT */
#define MMC_BYTCNT_BYTE_COUNT_SHIFT (0U)
#define MMC_BYTCNT_BYTE_COUNT_MASK  (0xFFFFFFFFU << MMC_BYTCNT_BYTE_COUNT_SHIFT)                 /* 0xFFFFFFFF */
/* INTMASK */
#define MMC_INTMASK_INT_MASK_SHIFT             (0U)
#define MMC_INTMASK_INT_MASK_MASK              (0xFFFFU << MMC_INTMASK_INT_MASK_SHIFT)                      /* 0x0000FFFF */
#define MMC_INTMASK_DATA_NOBUSY_INT_MASK_SHIFT (16U)
#define MMC_INTMASK_DATA_NOBUSY_INT_MASK_MASK  (0x1U << MMC_INTMASK_DATA_NOBUSY_INT_MASK_SHIFT)             /* 0x00010000 */
#define MMC_INTMASK_SDIO_INT_MASK_SHIFT        (24U)
#define MMC_INTMASK_SDIO_INT_MASK_MASK         (0x1U << MMC_INTMASK_SDIO_INT_MASK_SHIFT)                    /* 0x01000000 */
/* CMDARG */
#define MMC_CMDARG_CMD_ARG_SHIFT (0U)
#define MMC_CMDARG_CMD_ARG_MASK  (0xFFFFFFFFU << MMC_CMDARG_CMD_ARG_SHIFT)                    /* 0xFFFFFFFF */
/* CMD */
#define MMC_CMD_CMD_INDEX_SHIFT              (0U)
#define MMC_CMD_CMD_INDEX_MASK               (0x3FU << MMC_CMD_CMD_INDEX_SHIFT)                           /* 0x0000003F */
#define MMC_CMD_RESPONSE_EXPECT_SHIFT        (6U)
#define MMC_CMD_RESPONSE_EXPECT_MASK         (0x1U << MMC_CMD_RESPONSE_EXPECT_SHIFT)                      /* 0x00000040 */
#define MMC_CMD_RESPONSE_LENGTH_SHIFT        (7U)
#define MMC_CMD_RESPONSE_LENGTH_MASK         (0x1U << MMC_CMD_RESPONSE_LENGTH_SHIFT)                      /* 0x00000080 */
#define MMC_CMD_CHECK_RESPONSE_CRC_SHIFT     (8U)
#define MMC_CMD_CHECK_RESPONSE_CRC_MASK      (0x1U << MMC_CMD_CHECK_RESPONSE_CRC_SHIFT)                   /* 0x00000100 */
#define MMC_CMD_DATA_EXPECTED_SHIFT          (9U)
#define MMC_CMD_DATA_EXPECTED_MASK           (0x1U << MMC_CMD_DATA_EXPECTED_SHIFT)                        /* 0x00000200 */
#define MMC_CMD_WR_SHIFT                     (10U)
#define MMC_CMD_WR_MASK                      (0x1U << MMC_CMD_WR_SHIFT)                                   /* 0x00000400 */
#define MMC_CMD_TRANSFER_MODE_SHIFT          (11U)
#define MMC_CMD_TRANSFER_MODE_MASK           (0x1U << MMC_CMD_TRANSFER_MODE_SHIFT)                        /* 0x00000800 */
#define MMC_CMD_SEND_AUTO_STOP_SHIFT         (12U)
#define MMC_CMD_SEND_AUTO_STOP_MASK          (0x1U << MMC_CMD_SEND_AUTO_STOP_SHIFT)                       /* 0x00001000 */
#define MMC_CMD_WAIT_PRVDATA_COMPLETE_SHIFT  (13U)
#define MMC_CMD_WAIT_PRVDATA_COMPLETE_MASK   (0x1U << MMC_CMD_WAIT_PRVDATA_COMPLETE_SHIFT)                /* 0x00002000 */
#define MMC_CMD_STOP_ABORT_CMD_SHIFT         (14U)
#define MMC_CMD_STOP_ABORT_CMD_MASK          (0x1U << MMC_CMD_STOP_ABORT_CMD_SHIFT)                       /* 0x00004000 */
#define MMC_CMD_SEND_INITIALIZATION_SHIFT    (15U)
#define MMC_CMD_SEND_INITIALIZATION_MASK     (0x1U << MMC_CMD_SEND_INITIALIZATION_SHIFT)                  /* 0x00008000 */
#define MMC_CMD_UPDATE_CLOCK_REGS_ONLY_SHIFT (21U)
#define MMC_CMD_UPDATE_CLOCK_REGS_ONLY_MASK  (0x1U << MMC_CMD_UPDATE_CLOCK_REGS_ONLY_SHIFT)               /* 0x00200000 */
#define MMC_CMD_READ_CEATA_DEVICE_SHIFT      (22U)
#define MMC_CMD_READ_CEATA_DEVICE_MASK       (0x1U << MMC_CMD_READ_CEATA_DEVICE_SHIFT)                    /* 0x00400000 */
#define MMC_CMD_CCS_EXPECTED_SHIFT           (23U)
#define MMC_CMD_CCS_EXPECTED_MASK            (0x1U << MMC_CMD_CCS_EXPECTED_SHIFT)                         /* 0x00800000 */
#define MMC_CMD_ENABLE_BOOT_SHIFT            (24U)
#define MMC_CMD_ENABLE_BOOT_MASK             (0x1U << MMC_CMD_ENABLE_BOOT_SHIFT)                          /* 0x01000000 */
#define MMC_CMD_EXPECT_BOOT_ACK_SHIFT        (25U)
#define MMC_CMD_EXPECT_BOOT_ACK_MASK         (0x1U << MMC_CMD_EXPECT_BOOT_ACK_SHIFT)                      /* 0x02000000 */
#define MMC_CMD_DISABLE_BOOT_SHIFT           (26U)
#define MMC_CMD_DISABLE_BOOT_MASK            (0x1U << MMC_CMD_DISABLE_BOOT_SHIFT)                         /* 0x04000000 */
#define MMC_CMD_BOOT_MODE_SHIFT              (27U)
#define MMC_CMD_BOOT_MODE_MASK               (0x1U << MMC_CMD_BOOT_MODE_SHIFT)                            /* 0x08000000 */
#define MMC_CMD_VOLT_SWITCH_SHIFT            (28U)
#define MMC_CMD_VOLT_SWITCH_MASK             (0x1U << MMC_CMD_VOLT_SWITCH_SHIFT)                          /* 0x10000000 */
#define MMC_CMD_USE_HOLD_REG_SHIFT           (29U)
#define MMC_CMD_USE_HOLD_REG_MASK            (0x1U << MMC_CMD_USE_HOLD_REG_SHIFT)                         /* 0x20000000 */
#define MMC_CMD_START_CMD_SHIFT              (31U)
#define MMC_CMD_START_CMD_MASK               (0x1U << MMC_CMD_START_CMD_SHIFT)                            /* 0x80000000 */
/* RESP0 */
#define MMC_RESP0_RESPONSE0_SHIFT (0U)
#define MMC_RESP0_RESPONSE0_MASK  (0xFFFFFFFFU << MMC_RESP0_RESPONSE0_SHIFT)                   /* 0xFFFFFFFF */
/* RESP1 */
#define MMC_RESP1_RESPONSE_SHIFT (0U)
#define MMC_RESP1_RESPONSE_MASK  (0xFFFFFFFFU << MMC_RESP1_RESPONSE_SHIFT)                    /* 0xFFFFFFFF */
/* RESP2 */
#define MMC_RESP2_RESPONSE2_SHIFT (0U)
#define MMC_RESP2_RESPONSE2_MASK  (0xFFFFFFFFU << MMC_RESP2_RESPONSE2_SHIFT)                   /* 0xFFFFFFFF */
/* RESP3 */
#define MMC_RESP3_RESPONSE3_SHIFT (0U)
#define MMC_RESP3_RESPONSE3_MASK  (0xFFFFFFFFU << MMC_RESP3_RESPONSE3_SHIFT)                   /* 0xFFFFFFFF */
/* MINTSTS */
#define MMC_MINTSTS_INT_STATUS_SHIFT             (0U)
#define MMC_MINTSTS_INT_STATUS_MASK              (0xFFFFU << MMC_MINTSTS_INT_STATUS_SHIFT)                    /* 0x0000FFFF */
#define MMC_MINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT (16U)
#define MMC_MINTSTS_DATA_NOBUSY_INT_STATUS_MASK  (0x1U << MMC_MINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT)           /* 0x00010000 */
#define MMC_MINTSTS_SDIO_INTERRUPT_SHIFT         (24U)
#define MMC_MINTSTS_SDIO_INTERRUPT_MASK          (0x1U << MMC_MINTSTS_SDIO_INTERRUPT_SHIFT)                   /* 0x01000000 */
/* RINTSTS */
#define MMC_RINTSTS_INT_STATUS_SHIFT             (0U)
#define MMC_RINTSTS_INT_STATUS_MASK              (0xFFFFU << MMC_RINTSTS_INT_STATUS_SHIFT)                    /* 0x0000FFFF */
#define MMC_RINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT (16U)
#define MMC_RINTSTS_DATA_NOBUSY_INT_STATUS_MASK  (0x1U << MMC_RINTSTS_DATA_NOBUSY_INT_STATUS_SHIFT)           /* 0x00010000 */
#define MMC_RINTSTS_SDIO_INTERRUPT_SHIFT         (24U)
#define MMC_RINTSTS_SDIO_INTERRUPT_MASK          (0x1U << MMC_RINTSTS_SDIO_INTERRUPT_SHIFT)                   /* 0x01000000 */
/* STATUS */
#define MMC_STATUS_FIFO_RX_WATERMARK_SHIFT  (0U)
#define MMC_STATUS_FIFO_RX_WATERMARK_MASK   (0x1U << MMC_STATUS_FIFO_RX_WATERMARK_SHIFT)                 /* 0x00000001 */
#define MMC_STATUS_FIFO_TX_WATERMARK_SHIFT  (1U)
#define MMC_STATUS_FIFO_TX_WATERMARK_MASK   (0x1U << MMC_STATUS_FIFO_TX_WATERMARK_SHIFT)                 /* 0x00000002 */
#define MMC_STATUS_FIFO_EMPTY_SHIFT         (2U)
#define MMC_STATUS_FIFO_EMPTY_MASK          (0x1U << MMC_STATUS_FIFO_EMPTY_SHIFT)                        /* 0x00000004 */
#define MMC_STATUS_FIFO_FULL_SHIFT          (3U)
#define MMC_STATUS_FIFO_FULL_MASK           (0x1U << MMC_STATUS_FIFO_FULL_SHIFT)                         /* 0x00000008 */
#define MMC_STATUS_COMMAND_FSM_STATES_SHIFT (4U)
#define MMC_STATUS_COMMAND_FSM_STATES_MASK  (0xFU << MMC_STATUS_COMMAND_FSM_STATES_SHIFT)                /* 0x000000F0 */
#define MMC_STATUS_DATA_3_STATUS_SHIFT      (8U)
#define MMC_STATUS_DATA_3_STATUS_MASK       (0x1U << MMC_STATUS_DATA_3_STATUS_SHIFT)                     /* 0x00000100 */
#define MMC_STATUS_DATA_BUSY_SHIFT          (9U)
#define MMC_STATUS_DATA_BUSY_MASK           (0x1U << MMC_STATUS_DATA_BUSY_SHIFT)                         /* 0x00000200 */
#define MMC_STATUS_DATA_STATE_MC_BUSY_SHIFT (10U)
#define MMC_STATUS_DATA_STATE_MC_BUSY_MASK  (0x1U << MMC_STATUS_DATA_STATE_MC_BUSY_SHIFT)                /* 0x00000400 */
#define MMC_STATUS_RESPONSE_INDEX_SHIFT     (11U)
#define MMC_STATUS_RESPONSE_INDEX_MASK      (0x3FU << MMC_STATUS_RESPONSE_INDEX_SHIFT)                   /* 0x0001F800 */
#define MMC_STATUS_FIFO_COUNT_SHIFT         (17U)
#define MMC_STATUS_FIFO_COUNT_MASK          (0x1FFFU << MMC_STATUS_FIFO_COUNT_SHIFT)                     /* 0x3FFE0000 */
#define MMC_STATUS_DMA_ACK_SHIFT            (30U)
#define MMC_STATUS_DMA_ACK_MASK             (0x1U << MMC_STATUS_DMA_ACK_SHIFT)                           /* 0x40000000 */
#define MMC_STATUS_DMA_REQ_SHIFT            (31U)
#define MMC_STATUS_DMA_REQ_MASK             (0x1U << MMC_STATUS_DMA_REQ_SHIFT)                           /* 0x80000000 */
/* FIFOTH */
#define MMC_FIFOTH_TX_WMARK_SHIFT                     (0U)
#define MMC_FIFOTH_TX_WMARK_MASK                      (0xFFFU << MMC_FIFOTH_TX_WMARK_SHIFT)                        /* 0x00000FFF */
#define MMC_FIFOTH_RX_WMARK_SHIFT                     (16U)
#define MMC_FIFOTH_RX_WMARK_MASK                      (0xFFFU << MMC_FIFOTH_RX_WMARK_SHIFT)                        /* 0x0FFF0000 */
#define MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_SHIFT (28U)
#define MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_MASK  (0x7U << MMC_FIFOTH_DMA_MUTIPLE_TRANSACTION_SIZE_SHIFT)      /* 0x70000000 */
/* CDETECT */
#define MMC_CDETECT_CARD_DETECT_N_SHIFT (0U)
#define MMC_CDETECT_CARD_DETECT_N_MASK  (0x1U << MMC_CDETECT_CARD_DETECT_N_SHIFT)                    /* 0x00000001 */
/* WRTPRT */
#define MMC_WRTPRT_WRITE_PROTECT_SHIFT (0U)
#define MMC_WRTPRT_WRITE_PROTECT_MASK  (0x1U << MMC_WRTPRT_WRITE_PROTECT_SHIFT)                     /* 0x00000001 */
/* TCBCNT */
#define MMC_TCBCNT_TRANS_CARD_BYTE_COUNT_SHIFT (0U)
#define MMC_TCBCNT_TRANS_CARD_BYTE_COUNT_MASK  (0xFFFFFFFFU << MMC_TCBCNT_TRANS_CARD_BYTE_COUNT_SHIFT)      /* 0xFFFFFFFF */
/* TBBCNT */
#define MMC_TBBCNT_TRANS_FIFO_BYTE_COUNT_SHIFT (0U)
#define MMC_TBBCNT_TRANS_FIFO_BYTE_COUNT_MASK  (0xFFFFFFFFU << MMC_TBBCNT_TRANS_FIFO_BYTE_COUNT_SHIFT)      /* 0xFFFFFFFF */
/* DEBNCE */
#define MMC_DEBNCE_DEBOUNCE_COUNT_SHIFT (0U)
#define MMC_DEBNCE_DEBOUNCE_COUNT_MASK  (0xFFFFFFU << MMC_DEBNCE_DEBOUNCE_COUNT_SHIFT)               /* 0x00FFFFFF */
/* USRID */
#define MMC_USRID_USRID_SHIFT (0U)
#define MMC_USRID_USRID_MASK  (0xFFFFFFFFU << MMC_USRID_USRID_SHIFT)                       /* 0xFFFFFFFF */
/* VERID */
#define MMC_VERID_VERID_SHIFT (0U)
#define MMC_VERID_VERID_MASK  (0xFFFFFFFFU << MMC_VERID_VERID_SHIFT)                       /* 0xFFFFFFFF */
/* HCON */
#define MMC_HCON_CARD_TYPE_SHIFT          (0U)
#define MMC_HCON_CARD_TYPE_MASK           (0x1U << MMC_HCON_CARD_TYPE_SHIFT)                           /* 0x00000001 */
#define MMC_HCON_CARD_NUM_SHIFT           (1U)
#define MMC_HCON_CARD_NUM_MASK            (0x1FU << MMC_HCON_CARD_NUM_SHIFT)                           /* 0x0000003E */
#define MMC_HCON_H_BUS_TYPE_SHIFT         (6U)
#define MMC_HCON_H_BUS_TYPE_MASK          (0x1U << MMC_HCON_H_BUS_TYPE_SHIFT)                          /* 0x00000040 */
#define MMC_HCON_H_DATA_WIDTH_SHIFT       (7U)
#define MMC_HCON_H_DATA_WIDTH_MASK        (0x7U << MMC_HCON_H_DATA_WIDTH_SHIFT)                        /* 0x00000380 */
#define MMC_HCON_H_ADDR_WIDTH_SHIFT       (10U)
#define MMC_HCON_H_ADDR_WIDTH_MASK        (0x3FU << MMC_HCON_H_ADDR_WIDTH_SHIFT)                       /* 0x0000FC00 */
#define MMC_HCON_DMA_INTERFACE_SHIFT      (16U)
#define MMC_HCON_DMA_INTERFACE_MASK       (0x3U << MMC_HCON_DMA_INTERFACE_SHIFT)                       /* 0x00030000 */
#define MMC_HCON_GE_DMA_DATA_WIDTH_SHIFT  (18U)
#define MMC_HCON_GE_DMA_DATA_WIDTH_MASK   (0x7U << MMC_HCON_GE_DMA_DATA_WIDTH_SHIFT)                   /* 0x001C0000 */
#define MMC_HCON_FIFO_RAM_INSIDE_SHIFT    (21U)
#define MMC_HCON_FIFO_RAM_INSIDE_MASK     (0x1U << MMC_HCON_FIFO_RAM_INSIDE_SHIFT)                     /* 0x00200000 */
#define MMC_HCON_IMPL_HOLD_REG_SHIFT      (22U)
#define MMC_HCON_IMPL_HOLD_REG_MASK       (0x1U << MMC_HCON_IMPL_HOLD_REG_SHIFT)                       /* 0x00400000 */
#define MMC_HCON_SET_CLK_FALSE_PATH_SHIFT (23U)
#define MMC_HCON_SET_CLK_FALSE_PATH_MASK  (0x1U << MMC_HCON_SET_CLK_FALSE_PATH_SHIFT)                  /* 0x00800000 */
#define MMC_HCON_NUM_CLK_DIV_SHIFT        (24U)
#define MMC_HCON_NUM_CLK_DIV_MASK         (0x3U << MMC_HCON_NUM_CLK_DIV_SHIFT)                         /* 0x03000000 */
#define MMC_HCON_AREA_OPTIMIZED_SHIFT     (26U)
#define MMC_HCON_AREA_OPTIMIZED_MASK      (0x1U << MMC_HCON_AREA_OPTIMIZED_SHIFT)                      /* 0x04000000 */
/* UHSREG */
#define MMC_UHSREG_DDR_REG_SHIFT (16U)
#define MMC_UHSREG_DDR_REG_MASK  (0x1U << MMC_UHSREG_DDR_REG_SHIFT)                           /* 0x00010000 */
/* RSTN */
#define MMC_RSTN_CARD_RESET_SHIFT (0U)
#define MMC_RSTN_CARD_RESET_MASK  (0x1U << MMC_RSTN_CARD_RESET_SHIFT)                          /* 0x00000001 */
/* BMOD */
#define MMC_BMOD_SWR_SHIFT (0U)
#define MMC_BMOD_SWR_MASK  (0x1U << MMC_BMOD_SWR_SHIFT)                                 /* 0x00000001 */
#define MMC_BMOD_FB_SHIFT  (1U)
#define MMC_BMOD_FB_MASK   (0x1U << MMC_BMOD_FB_SHIFT)                                  /* 0x00000002 */
#define MMC_BMOD_DSL_SHIFT (2U)
#define MMC_BMOD_DSL_MASK  (0x1FU << MMC_BMOD_DSL_SHIFT)                                /* 0x0000007C */
#define MMC_BMOD_DE_SHIFT  (7U)
#define MMC_BMOD_DE_MASK   (0x1U << MMC_BMOD_DE_SHIFT)                                  /* 0x00000080 */
#define MMC_BMOD_PBL_SHIFT (8U)
#define MMC_BMOD_PBL_MASK  (0x7U << MMC_BMOD_PBL_SHIFT)                                 /* 0x00000700 */
/* PLDMND */
#define MMC_PLDMND_PD_SHIFT (0U)
#define MMC_PLDMND_PD_MASK  (0xFFFFFFFFU << MMC_PLDMND_PD_SHIFT)                         /* 0xFFFFFFFF */
/* DBADDR */
#define MMC_DBADDR_SBL_SHIFT (0U)
#define MMC_DBADDR_SBL_MASK  (0xFFFFFFFFU << MMC_DBADDR_SBL_SHIFT)                        /* 0xFFFFFFFF */
/* IDSTS */
#define MMC_IDSTS_TI_SHIFT  (0U)
#define MMC_IDSTS_TI_MASK   (0x1U << MMC_IDSTS_TI_SHIFT)                                 /* 0x00000001 */
#define MMC_IDSTS_RI_SHIFT  (1U)
#define MMC_IDSTS_RI_MASK   (0x1U << MMC_IDSTS_RI_SHIFT)                                 /* 0x00000002 */
#define MMC_IDSTS_FBE_SHIFT (2U)
#define MMC_IDSTS_FBE_MASK  (0x1U << MMC_IDSTS_FBE_SHIFT)                                /* 0x00000004 */
#define MMC_IDSTS_DUI_SHIFT (4U)
#define MMC_IDSTS_DUI_MASK  (0x1U << MMC_IDSTS_DUI_SHIFT)                                /* 0x00000010 */
#define MMC_IDSTS_CES_SHIFT (5U)
#define MMC_IDSTS_CES_MASK  (0x1U << MMC_IDSTS_CES_SHIFT)                                /* 0x00000020 */
#define MMC_IDSTS_NIS_SHIFT (8U)
#define MMC_IDSTS_NIS_MASK  (0x1U << MMC_IDSTS_NIS_SHIFT)                                /* 0x00000100 */
#define MMC_IDSTS_AIS_SHIFT (9U)
#define MMC_IDSTS_AIS_MASK  (0x1U << MMC_IDSTS_AIS_SHIFT)                                /* 0x00000200 */
#define MMC_IDSTS_EB_SHIFT  (10U)
#define MMC_IDSTS_EB_MASK   (0x7U << MMC_IDSTS_EB_SHIFT)                                 /* 0x00001C00 */
#define MMC_IDSTS_FSM_SHIFT (13U)
#define MMC_IDSTS_FSM_MASK  (0xFU << MMC_IDSTS_FSM_SHIFT)                                /* 0x0001E000 */
/* IDINTEN */
#define MMC_IDINTEN_TI_SHIFT  (0U)
#define MMC_IDINTEN_TI_MASK   (0x1U << MMC_IDINTEN_TI_SHIFT)                               /* 0x00000001 */
#define MMC_IDINTEN_RI_SHIFT  (1U)
#define MMC_IDINTEN_RI_MASK   (0x1U << MMC_IDINTEN_RI_SHIFT)                               /* 0x00000002 */
#define MMC_IDINTEN_FBE_SHIFT (2U)
#define MMC_IDINTEN_FBE_MASK  (0x1U << MMC_IDINTEN_FBE_SHIFT)                              /* 0x00000004 */
#define MMC_IDINTEN_DU_SHIFT  (4U)
#define MMC_IDINTEN_DU_MASK   (0x1U << MMC_IDINTEN_DU_SHIFT)                               /* 0x00000010 */
#define MMC_IDINTEN_CES_SHIFT (5U)
#define MMC_IDINTEN_CES_MASK  (0x1U << MMC_IDINTEN_CES_SHIFT)                              /* 0x00000020 */
#define MMC_IDINTEN_NI_SHIFT  (8U)
#define MMC_IDINTEN_NI_MASK   (0x1U << MMC_IDINTEN_NI_SHIFT)                               /* 0x00000100 */
#define MMC_IDINTEN_AI_SHIFT  (9U)
#define MMC_IDINTEN_AI_MASK   (0x1U << MMC_IDINTEN_AI_SHIFT)                               /* 0x00000200 */
/* DSCADDR */
#define MMC_DSCADDR_HDA_SHIFT (0U)
#define MMC_DSCADDR_HDA_MASK  (0xFFFFFFFFU << MMC_DSCADDR_HDA_SHIFT)                       /* 0xFFFFFFFF */
/* BUFADDR */
#define MMC_BUFADDR_HBA_SHIFT (0U)
#define MMC_BUFADDR_HBA_MASK  (0xFFFFFFFFU << MMC_BUFADDR_HBA_SHIFT)                       /* 0xFFFFFFFF */
/* CARDTHRCTL */
#define MMC_CARDTHRCTL_CARD_RD_THRES_EN_SHIFT (0U)
#define MMC_CARDTHRCTL_CARD_RD_THRES_EN_MASK  (0x1U << MMC_CARDTHRCTL_CARD_RD_THRES_EN_SHIFT)              /* 0x00000001 */
#define MMC_CARDTHRCTL_BUSY_CLR_INT_EN_SHIFT  (1U)
#define MMC_CARDTHRCTL_BUSY_CLR_INT_EN_MASK   (0x1U << MMC_CARDTHRCTL_BUSY_CLR_INT_EN_SHIFT)               /* 0x00000002 */
#define MMC_CARDTHRCTL_CARD_RD_THRES_SHIFT    (16U)
#define MMC_CARDTHRCTL_CARD_RD_THRES_MASK     (0xFFFU << MMC_CARDTHRCTL_CARD_RD_THRES_SHIFT)               /* 0x0FFF0000 */
/* BACKEND_POWER */
#define MMC_BACKEND_POWER_BACK_END_POWER_SHIFT (0U)
#define MMC_BACKEND_POWER_BACK_END_POWER_MASK  (0x1U << MMC_BACKEND_POWER_BACK_END_POWER_SHIFT)             /* 0x00000001 */
/* EMMCDDR_REG */
#define MMC_EMMCDDR_REG_HALF_START_BIT_SHIFT (0U)
#define MMC_EMMCDDR_REG_HALF_START_BIT_MASK  (0x1U << MMC_EMMCDDR_REG_HALF_START_BIT_SHIFT)               /* 0x00000001 */
/* RDYINT_GEN */
#define MMC_RDYINT_GEN_RDYINT_GEN_MAXVAL_SHIFT  (0U)
#define MMC_RDYINT_GEN_RDYINT_GEN_MAXVAL_MASK   (0xFFU << MMC_RDYINT_GEN_RDYINT_GEN_MAXVAL_SHIFT)            /* 0x000000FF */
#define MMC_RDYINT_GEN_RDYINT_GEN_WORKING_SHIFT (8U)
#define MMC_RDYINT_GEN_RDYINT_GEN_WORKING_MASK  (0x1U << MMC_RDYINT_GEN_RDYINT_GEN_WORKING_SHIFT)            /* 0x00000100 */
#define MMC_RDYINT_GEN_RDYINT_CNT_STATUS_SHIFT  (16U)
#define MMC_RDYINT_GEN_RDYINT_CNT_STATUS_MASK   (0xFFU << MMC_RDYINT_GEN_RDYINT_CNT_STATUS_SHIFT)            /* 0x00FF0000 */
#define MMC_RDYINT_GEN_RDYINT_CNT_FINISH_SHIFT  (24U)
#define MMC_RDYINT_GEN_RDYINT_CNT_FINISH_MASK   (0x1U << MMC_RDYINT_GEN_RDYINT_CNT_FINISH_SHIFT)             /* 0x01000000 */
/* FIFO_BASE */
#define MMC_FIFO_BASE_FIFO_BASE_ADDR_SHIFT (0U)
#define MMC_FIFO_BASE_FIFO_BASE_ADDR_MASK  (0xFFFFFFFFU << MMC_FIFO_BASE_FIFO_BASE_ADDR_SHIFT)          /* 0xFFFFFFFF */

/******************************************I2C*******************************************/
/* CON */
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
#define I2C_CLKDIV_CLKDIVL_SHIFT                           (0U)
#define I2C_CLKDIV_CLKDIVL_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVL_SHIFT)                        /* 0x0000FFFF */
#define I2C_CLKDIV_CLKDIVH_SHIFT                           (16U)
#define I2C_CLKDIV_CLKDIVH_MASK                            (0xFFFFU << I2C_CLKDIV_CLKDIVH_SHIFT)                        /* 0xFFFF0000 */
/* MRXADDR */
#define I2C_MRXADDR_SADDR_SHIFT                            (0U)
#define I2C_MRXADDR_SADDR_MASK                             (0xFFFFFFU << I2C_MRXADDR_SADDR_SHIFT)                       /* 0x00FFFFFF */
#define I2C_MRXADDR_ADDLVLD_SHIFT                          (24U)
#define I2C_MRXADDR_ADDLVLD_MASK                           (0x1U << I2C_MRXADDR_ADDLVLD_SHIFT)                          /* 0x01000000 */
#define I2C_MRXADDR_ADDMVLD_SHIFT                          (25U)
#define I2C_MRXADDR_ADDMVLD_MASK                           (0x1U << I2C_MRXADDR_ADDMVLD_SHIFT)                          /* 0x02000000 */
#define I2C_MRXADDR_ADDHVLD_SHIFT                          (26U)
#define I2C_MRXADDR_ADDHVLD_MASK                           (0x1U << I2C_MRXADDR_ADDHVLD_SHIFT)                          /* 0x04000000 */
/* MRXRADDR */
#define I2C_MRXRADDR_SRADDR_SHIFT                          (0U)
#define I2C_MRXRADDR_SRADDR_MASK                           (0xFFFFFFU << I2C_MRXRADDR_SRADDR_SHIFT)                     /* 0x00FFFFFF */
#define I2C_MRXRADDR_SRADDLVLD_SHIFT                       (24U)
#define I2C_MRXRADDR_SRADDLVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDLVLD_SHIFT)                       /* 0x01000000 */
#define I2C_MRXRADDR_SRADDMVLD_SHIFT                       (25U)
#define I2C_MRXRADDR_SRADDMVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDMVLD_SHIFT)                       /* 0x02000000 */
#define I2C_MRXRADDR_SRADDHVLD_SHIFT                       (26U)
#define I2C_MRXRADDR_SRADDHVLD_MASK                        (0x1U << I2C_MRXRADDR_SRADDHVLD_SHIFT)                       /* 0x04000000 */
/* MTXCNT */
#define I2C_MTXCNT_MTXCNT_SHIFT                            (0U)
#define I2C_MTXCNT_MTXCNT_MASK                             (0x3FU << I2C_MTXCNT_MTXCNT_SHIFT)                           /* 0x0000003F */
/* MRXCNT */
#define I2C_MRXCNT_MRXCNT_SHIFT                            (0U)
#define I2C_MRXCNT_MRXCNT_MASK                             (0x3FU << I2C_MRXCNT_MRXCNT_SHIFT)                           /* 0x0000003F */
/* IEN */
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
#define I2C_FCNT_FCNT_SHIFT                                (0U)
#define I2C_FCNT_FCNT_MASK                                 (0x3FU << I2C_FCNT_FCNT_SHIFT)                               /* 0x0000003F */
/* SCL_OE_DB */
#define I2C_SCL_OE_DB_SCL_OE_DB_SHIFT                      (0U)
#define I2C_SCL_OE_DB_SCL_OE_DB_MASK                       (0xFFU << I2C_SCL_OE_DB_SCL_OE_DB_SHIFT)                     /* 0x000000FF */
/* TXDATA0 */
#define I2C_TXDATA0_TXDATA0_SHIFT                          (0U)
#define I2C_TXDATA0_TXDATA0_MASK                           (0xFFFFFFFFU << I2C_TXDATA0_TXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA1 */
#define I2C_TXDATA1_TXDATA1_SHIFT                          (0U)
#define I2C_TXDATA1_TXDATA1_MASK                           (0xFFFFFFFFU << I2C_TXDATA1_TXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA2 */
#define I2C_TXDATA2_TXDATA2_SHIFT                          (0U)
#define I2C_TXDATA2_TXDATA2_MASK                           (0xFFFFFFFFU << I2C_TXDATA2_TXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA3 */
#define I2C_TXDATA3_TXDATA3_SHIFT                          (0U)
#define I2C_TXDATA3_TXDATA3_MASK                           (0xFFFFFFFFU << I2C_TXDATA3_TXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA4 */
#define I2C_TXDATA4_TXDATA4_SHIFT                          (0U)
#define I2C_TXDATA4_TXDATA4_MASK                           (0xFFFFFFFFU << I2C_TXDATA4_TXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA5 */
#define I2C_TXDATA5_TXDATA5_SHIFT                          (0U)
#define I2C_TXDATA5_TXDATA5_MASK                           (0xFFFFFFFFU << I2C_TXDATA5_TXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA6 */
#define I2C_TXDATA6_TXDATA6_SHIFT                          (0U)
#define I2C_TXDATA6_TXDATA6_MASK                           (0xFFFFFFFFU << I2C_TXDATA6_TXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* TXDATA7 */
#define I2C_TXDATA7_TXDATA7_SHIFT                          (0U)
#define I2C_TXDATA7_TXDATA7_MASK                           (0xFFFFFFFFU << I2C_TXDATA7_TXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA0 */
#define I2C_RXDATA0_RXDATA0_SHIFT                          (0U)
#define I2C_RXDATA0_RXDATA0_MASK                           (0xFFFFFFFFU << I2C_RXDATA0_RXDATA0_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA1 */
#define I2C_RXDATA1_RXDATA1_SHIFT                          (0U)
#define I2C_RXDATA1_RXDATA1_MASK                           (0xFFFFFFFFU << I2C_RXDATA1_RXDATA1_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA2 */
#define I2C_RXDATA2_RXDATA2_SHIFT                          (0U)
#define I2C_RXDATA2_RXDATA2_MASK                           (0xFFFFFFFFU << I2C_RXDATA2_RXDATA2_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA3 */
#define I2C_RXDATA3_RXDATA3_SHIFT                          (0U)
#define I2C_RXDATA3_RXDATA3_MASK                           (0xFFFFFFFFU << I2C_RXDATA3_RXDATA3_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA4 */
#define I2C_RXDATA4_RXDATA4_SHIFT                          (0U)
#define I2C_RXDATA4_RXDATA4_MASK                           (0xFFFFFFFFU << I2C_RXDATA4_RXDATA4_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA5 */
#define I2C_RXDATA5_RXDATA5_SHIFT                          (0U)
#define I2C_RXDATA5_RXDATA5_MASK                           (0xFFFFFFFFU << I2C_RXDATA5_RXDATA5_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA6 */
#define I2C_RXDATA6_RXDATA6_SHIFT                          (0U)
#define I2C_RXDATA6_RXDATA6_MASK                           (0xFFFFFFFFU << I2C_RXDATA6_RXDATA6_SHIFT)                   /* 0xFFFFFFFF */
/* RXDATA7 */
#define I2C_RXDATA7_RXDATA7_SHIFT                          (0U)
#define I2C_RXDATA7_RXDATA7_MASK                           (0xFFFFFFFFU << I2C_RXDATA7_RXDATA7_SHIFT)                   /* 0xFFFFFFFF */
/* ST */
#define I2C_ST_SDA_ST_SHIFT                                (0U)
#define I2C_ST_SDA_ST_MASK                                 (0x1U << I2C_ST_SDA_ST_SHIFT)                                /* 0x00000001 */
#define I2C_ST_SCL_ST_SHIFT                                (1U)
#define I2C_ST_SCL_ST_MASK                                 (0x1U << I2C_ST_SCL_ST_SHIFT)                                /* 0x00000002 */
/* DBGCTRL */
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
/****************************************SPI2APB*****************************************/
/* IMR */
#define SPI2APB_IMR_TFEIM_SHIFT                            (0U)
#define SPI2APB_IMR_TFEIM_MASK                             (0x1U << SPI2APB_IMR_TFEIM_SHIFT)                            /* 0x00000001 */
#define SPI2APB_IMR_TFOIM_SHIFT                            (1U)
#define SPI2APB_IMR_TFOIM_MASK                             (0x1U << SPI2APB_IMR_TFOIM_SHIFT)                            /* 0x00000002 */
#define SPI2APB_IMR_RFUIM_SHIFT                            (2U)
#define SPI2APB_IMR_RFUIM_MASK                             (0x1U << SPI2APB_IMR_RFUIM_SHIFT)                            /* 0x00000004 */
#define SPI2APB_IMR_RFOIM_SHIFT                            (3U)
#define SPI2APB_IMR_RFOIM_MASK                             (0x1U << SPI2APB_IMR_RFOIM_SHIFT)                            /* 0x00000008 */
#define SPI2APB_IMR_RFFIM_SHIFT                            (4U)
#define SPI2APB_IMR_RFFIM_MASK                             (0x1U << SPI2APB_IMR_RFFIM_SHIFT)                            /* 0x00000010 */
#define SPI2APB_IMR_QWIM_SHIFT                             (5U)
#define SPI2APB_IMR_QWIM_MASK                              (0x1U << SPI2APB_IMR_QWIM_SHIFT)                             /* 0x00000020 */
/* ISR */
#define SPI2APB_ISR_TFEIS_SHIFT                            (0U)
#define SPI2APB_ISR_TFEIS_MASK                             (0x1U << SPI2APB_ISR_TFEIS_SHIFT)                            /* 0x00000001 */
#define SPI2APB_ISR_TFOIS_SHIFT                            (1U)
#define SPI2APB_ISR_TFOIS_MASK                             (0x1U << SPI2APB_ISR_TFOIS_SHIFT)                            /* 0x00000002 */
#define SPI2APB_ISR_RFUIS_SHIFT                            (2U)
#define SPI2APB_ISR_RFUIS_MASK                             (0x1U << SPI2APB_ISR_RFUIS_SHIFT)                            /* 0x00000004 */
#define SPI2APB_ISR_RFOIS_SHIFT                            (3U)
#define SPI2APB_ISR_RFOIS_MASK                             (0x1U << SPI2APB_ISR_RFOIS_SHIFT)                            /* 0x00000008 */
#define SPI2APB_ISR_RFFIS_SHIFT                            (4U)
#define SPI2APB_ISR_RFFIS_MASK                             (0x1U << SPI2APB_ISR_RFFIS_SHIFT)                            /* 0x00000010 */
#define SPI2APB_ISR_QWIS_SHIFT                             (5U)
#define SPI2APB_ISR_QWIS_MASK                              (0x1U << SPI2APB_ISR_QWIS_SHIFT)                             /* 0x00000020 */
/* RISR */
#define SPI2APB_RISR_TFERIS_SHIFT                          (0U)
#define SPI2APB_RISR_TFERIS_MASK                           (0x1U << SPI2APB_RISR_TFERIS_SHIFT)                          /* 0x00000001 */
#define SPI2APB_RISR_TFORIS_SHIFT                          (1U)
#define SPI2APB_RISR_TFORIS_MASK                           (0x1U << SPI2APB_RISR_TFORIS_SHIFT)                          /* 0x00000002 */
#define SPI2APB_RISR_RFURIS_SHIFT                          (2U)
#define SPI2APB_RISR_RFURIS_MASK                           (0x1U << SPI2APB_RISR_RFURIS_SHIFT)                          /* 0x00000004 */
#define SPI2APB_RISR_RFORIS_SHIFT                          (3U)
#define SPI2APB_RISR_RFORIS_MASK                           (0x1U << SPI2APB_RISR_RFORIS_SHIFT)                          /* 0x00000008 */
#define SPI2APB_RISR_RFFRIS_SHIFT                          (4U)
#define SPI2APB_RISR_RFFRIS_MASK                           (0x1U << SPI2APB_RISR_RFFRIS_SHIFT)                          /* 0x00000010 */
#define SPI2APB_RISR_QWRIS_SHIFT                           (5U)
#define SPI2APB_RISR_QWRIS_MASK                            (0x1U << SPI2APB_RISR_QWRIS_SHIFT)                           /* 0x00000020 */
/* ICR */
#define SPI2APB_ICR_CCI_SHIFT                              (0U)
#define SPI2APB_ICR_CCI_MASK                               (0x1U << SPI2APB_ICR_CCI_SHIFT)                              /* 0x00000001 */
#define SPI2APB_ICR_CRFUI_SHIFT                            (1U)
#define SPI2APB_ICR_CRFUI_MASK                             (0x1U << SPI2APB_ICR_CRFUI_SHIFT)                            /* 0x00000002 */
#define SPI2APB_ICR_CRFOI_SHIFT                            (2U)
#define SPI2APB_ICR_CRFOI_MASK                             (0x1U << SPI2APB_ICR_CRFOI_SHIFT)                            /* 0x00000004 */
#define SPI2APB_ICR_CTFOI_SHIFT                            (3U)
#define SPI2APB_ICR_CTFOI_MASK                             (0x1U << SPI2APB_ICR_CTFOI_SHIFT)                            /* 0x00000008 */
#define SPI2APB_ICR_CQWI_SHIFT                             (4U)
#define SPI2APB_ICR_CQWI_MASK                              (0x1U << SPI2APB_ICR_CQWI_SHIFT)                             /* 0x00000010 */
/* WORK_MODE */
#define SPI2APB_WORK_MODE_SPI2APB_SHIFT                    (0U)
#define SPI2APB_WORK_MODE_SPI2APB_MASK                     (0x1U << SPI2APB_WORK_MODE_SPI2APB_SHIFT)                    /* 0x00000001 */
/* MESSAGE_REG0 */
#define SPI2APB_MESSAGE_REG0_QUICK_REG0_SHIFT              (0U)
#define SPI2APB_MESSAGE_REG0_QUICK_REG0_MASK               (0xFFFFFFFFU << SPI2APB_MESSAGE_REG0_QUICK_REG0_SHIFT)       /* 0xFFFFFFFF */
/* MESSAGE_REG1 */
#define SPI2APB_MESSAGE_REG1_QUICK_REG1_SHIFT              (0U)
#define SPI2APB_MESSAGE_REG1_QUICK_REG1_MASK               (0xFFFFFFFFU << SPI2APB_MESSAGE_REG1_QUICK_REG1_SHIFT)       /* 0xFFFFFFFF */
/* MESSAGE_REG2 */
#define SPI2APB_MESSAGE_REG2_QUICK_REG2_SHIFT              (0U)
#define SPI2APB_MESSAGE_REG2_QUICK_REG2_MASK               (0xFFFFFFFFU << SPI2APB_MESSAGE_REG2_QUICK_REG2_SHIFT)       /* 0xFFFFFFFF */
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
#define SPI_CTRLR0_TXD_DRIVE_SHIFT                         (22U)
#define SPI_CTRLR0_TXD_DRIVE_MASK                          (0x1U << SPI_CTRLR0_TXD_DRIVE_SHIFT)                         /* 0x00400000 */
#define SPI_CTRLR0_TXD_DELAY_SHIFT                         (23U)
#define SPI_CTRLR0_TXD_DELAY_MASK                          (0x3U << SPI_CTRLR0_TXD_DELAY_SHIFT)                         /* 0x01800000 */
#define SPI_CTRLR0_SCLK_IN_MASK_SHIFT                      (25U)
#define SPI_CTRLR0_SCLK_IN_MASK_MASK                       (0x1U << SPI_CTRLR0_SCLK_IN_MASK_SHIFT)                      /* 0x02000000 */
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
#define SPI_IMR_QWIM_SHIFT                                 (5U)
#define SPI_IMR_QWIM_MASK                                  (0x1U << SPI_IMR_QWIM_SHIFT)                                 /* 0x00000020 */
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
#define SPI_ISR_QWIS_SHIFT                                 (5U)
#define SPI_ISR_QWIS_MASK                                  (0x1U << SPI_ISR_QWIS_SHIFT)                                 /* 0x00000020 */
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
#define SPI_RISR_QWRIS_SHIFT                               (5U)
#define SPI_RISR_QWRIS_MASK                                (0x1U << SPI_RISR_QWRIS_SHIFT)                               /* 0x00000020 */
/* ICR */
#define SPI_ICR_CCI_SHIFT                                  (0U)
#define SPI_ICR_CCI_MASK                                   (0x1U << SPI_ICR_CCI_SHIFT)                                  /* 0x00000001 */
#define SPI_ICR_CRFUI_SHIFT                                (1U)
#define SPI_ICR_CRFUI_MASK                                 (0x1U << SPI_ICR_CRFUI_SHIFT)                                /* 0x00000002 */
#define SPI_ICR_CRFOI_SHIFT                                (2U)
#define SPI_ICR_CRFOI_MASK                                 (0x1U << SPI_ICR_CRFOI_SHIFT)                                /* 0x00000004 */
#define SPI_ICR_CTFOI_SHIFT                                (3U)
#define SPI_ICR_CTFOI_MASK                                 (0x1U << SPI_ICR_CTFOI_SHIFT)                                /* 0x00000008 */
#define SPI_ICR_CQWI_SHIFT                                 (4U)
#define SPI_ICR_CQWI_MASK                                  (0x1U << SPI_ICR_CQWI_SHIFT)                                 /* 0x00000010 */
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
/* SPI2APB */
#define SPI_SPI2APB_SPI2APB_SHIFT                          (0U)
#define SPI_SPI2APB_SPI2APB_MASK                           (0x1U << SPI_SPI2APB_SPI2APB_SHIFT)                          /* 0x00000001 */
/* QUICK_REG0 */
#define SPI_QUICK_REG0_QUICK_REG0_SHIFT                    (0U)
#define SPI_QUICK_REG0_QUICK_REG0_MASK                     (0xFFFFFFFFU << SPI_QUICK_REG0_QUICK_REG0_SHIFT)             /* 0xFFFFFFFF */
/* QUICK_REG1 */
#define SPI_QUICK_REG1_QUICK_REG1_SHIFT                    (0U)
#define SPI_QUICK_REG1_QUICK_REG1_MASK                     (0xFFFFFFFFU << SPI_QUICK_REG1_QUICK_REG1_SHIFT)             /* 0xFFFFFFFF */
/* QUICK_REG2 */
#define SPI_QUICK_REG2_QUICK_REG2_SHIFT                    (0U)
#define SPI_QUICK_REG2_QUICK_REG2_MASK                     (0xFFFFFFFFU << SPI_QUICK_REG2_QUICK_REG2_SHIFT)             /* 0xFFFFFFFF */
/* TXDR */
#define SPI_TXDR_TXDR_SHIFT                                (0U)
#define SPI_TXDR_TXDR_MASK                                 (0xFFFFU << SPI_TXDR_TXDR_SHIFT)                             /* 0x0000FFFF */
/* RXDR */
#define SPI_RXDR_RXDR_SHIFT                                (0U)
#define SPI_RXDR_RXDR_MASK                                 (0xFFFFU << SPI_RXDR_RXDR_SHIFT)                             /* 0x0000FFFF */
/******************************************WDT*******************************************/
/* CR */
#define WDT_CR_WDT_EN_SHIFT                                (0U)
#define WDT_CR_WDT_EN_MASK                                 (0x1U << WDT_CR_WDT_EN_SHIFT)                                /* 0x00000001 */
#define WDT_CR_RESP_MODE_SHIFT                             (1U)
#define WDT_CR_RESP_MODE_MASK                              (0x1U << WDT_CR_RESP_MODE_SHIFT)                             /* 0x00000002 */
#define WDT_CR_RST_PLUSE_LENTH_SHIFT                       (2U)
#define WDT_CR_RST_PLUSE_LENTH_MASK                        (0x7U << WDT_CR_RST_PLUSE_LENTH_SHIFT)                       /* 0x0000001C */
/* TORR */
#define WDT_TORR_TIMEOUT_PERIOD_SHIFT                      (0U)
#define WDT_TORR_TIMEOUT_PERIOD_MASK                       (0xFU << WDT_TORR_TIMEOUT_PERIOD_SHIFT)                      /* 0x0000000F */
/* CCVR */
#define WDT_CCVR_CUR_CNT_SHIFT                             (0U)
#define WDT_CCVR_CUR_CNT_MASK                              (0xFFFFFFFFU << WDT_CCVR_CUR_CNT_SHIFT)                      /* 0xFFFFFFFF */
/* CRR */
#define WDT_CRR_CNT_RESTART_SHIFT                          (0U)
#define WDT_CRR_CNT_RESTART_MASK                           (0xFFU << WDT_CRR_CNT_RESTART_SHIFT)                         /* 0x000000FF */
/* STAT */
#define WDT_STAT_WDT_STATUS_SHIFT                          (0U)
#define WDT_STAT_WDT_STATUS_MASK                           (0x1U << WDT_STAT_WDT_STATUS_SHIFT)                          /* 0x00000001 */
/* EOI */
#define WDT_EOI_WDT_INT_CLR_SHIFT                          (0U)
#define WDT_EOI_WDT_INT_CLR_MASK                           (0x1U << WDT_EOI_WDT_INT_CLR_SHIFT)                          /* 0x00000001 */
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
/******************************************I2STDM*******************************************/
/* TXCR */
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
#define I2STDM_TXFIFOLR_TFL0_SHIFT                            (0U)
#define I2STDM_TXFIFOLR_TFL0_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL0_SHIFT)                           /* 0x0000003F */
#define I2STDM_TXFIFOLR_TFL1_SHIFT                            (6U)
#define I2STDM_TXFIFOLR_TFL1_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL1_SHIFT)                           /* 0x00000FC0 */
#define I2STDM_TXFIFOLR_TFL2_SHIFT                            (12U)
#define I2STDM_TXFIFOLR_TFL2_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL2_SHIFT)                           /* 0x0003F000 */
#define I2STDM_TXFIFOLR_TFL3_SHIFT                            (18U)
#define I2STDM_TXFIFOLR_TFL3_MASK                             (0x3FU << I2STDM_TXFIFOLR_TFL3_SHIFT)                           /* 0x00FC0000 */
/* DMACR */
#define I2STDM_DMACR_TDL_SHIFT                                (0U)
#define I2STDM_DMACR_TDL_MASK                                 (0x1FU << I2STDM_DMACR_TDL_SHIFT)                               /* 0x0000001F */
#define I2STDM_DMACR_TDE_SHIFT                                (8U)
#define I2STDM_DMACR_TDE_MASK                                 (0x1U << I2STDM_DMACR_TDE_SHIFT)                                /* 0x00000100 */
#define I2STDM_DMACR_RDL_SHIFT                                (16U)
#define I2STDM_DMACR_RDL_MASK                                 (0x1FU << I2STDM_DMACR_RDL_SHIFT)                               /* 0x001F0000 */
#define I2STDM_DMACR_RDE_SHIFT                                (24U)
#define I2STDM_DMACR_RDE_MASK                                 (0x1U << I2STDM_DMACR_RDE_SHIFT)                                /* 0x01000000 */
/* INTCR */
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
#define I2STDM_INTSR_TXEI_SHIFT                               (0U)
#define I2STDM_INTSR_TXEI_MASK                                (0x1U << I2STDM_INTSR_TXEI_SHIFT)                               /* 0x00000001 */
#define I2STDM_INTSR_TXUI_SHIFT                               (1U)
#define I2STDM_INTSR_TXUI_MASK                                (0x1U << I2STDM_INTSR_TXUI_SHIFT)                               /* 0x00000002 */
#define I2STDM_INTSR_RXFI_SHIFT                               (16U)
#define I2STDM_INTSR_RXFI_MASK                                (0x1U << I2STDM_INTSR_RXFI_SHIFT)                               /* 0x00010000 */
#define I2STDM_INTSR_RXOI_SHIFT                               (17U)
#define I2STDM_INTSR_RXOI_MASK                                (0x1U << I2STDM_INTSR_RXOI_SHIFT)                               /* 0x00020000 */
/* XFER */
#define I2STDM_XFER_TXS_SHIFT                                 (0U)
#define I2STDM_XFER_TXS_MASK                                  (0x1U << I2STDM_XFER_TXS_SHIFT)                                 /* 0x00000001 */
#define I2STDM_XFER_RXS_SHIFT                                 (1U)
#define I2STDM_XFER_RXS_MASK                                  (0x1U << I2STDM_XFER_RXS_SHIFT)                                 /* 0x00000002 */
/* CLR */
#define I2STDM_CLR_TXC_SHIFT                                  (0U)
#define I2STDM_CLR_TXC_MASK                                   (0x1U << I2STDM_CLR_TXC_SHIFT)                                  /* 0x00000001 */
#define I2STDM_CLR_RXC_SHIFT                                  (1U)
#define I2STDM_CLR_RXC_MASK                                   (0x1U << I2STDM_CLR_RXC_SHIFT)                                  /* 0x00000002 */
/* TXDR */
#define I2STDM_TXDR_TXDR_SHIFT                                (0U)
#define I2STDM_TXDR_TXDR_MASK                                 (0xFFFFFFFFU << I2STDM_TXDR_TXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXDR */
#define I2STDM_RXDR_RXDR_SHIFT                                (0U)
#define I2STDM_RXDR_RXDR_MASK                                 (0xFFFFFFFFU << I2STDM_RXDR_RXDR_SHIFT)                         /* 0xFFFFFFFF */
/* RXFIFOLR */
#define I2STDM_RXFIFOLR_RFL0_SHIFT                            (0U)
#define I2STDM_RXFIFOLR_RFL0_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL0_SHIFT)                           /* 0x0000003F */
#define I2STDM_RXFIFOLR_RFL1_SHIFT                            (6U)
#define I2STDM_RXFIFOLR_RFL1_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL1_SHIFT)                           /* 0x00000FC0 */
#define I2STDM_RXFIFOLR_RFL2_SHIFT                            (12U)
#define I2STDM_RXFIFOLR_RFL2_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL2_SHIFT)                           /* 0x0003F000 */
#define I2STDM_RXFIFOLR_RFL3_SHIFT                            (18U)
#define I2STDM_RXFIFOLR_RFL3_MASK                             (0x3FU << I2STDM_RXFIFOLR_RFL3_SHIFT)                           /* 0x00FC0000 */
/* TDM_TXCTRL */
#define I2STDM_TDM_TXCTRL_TDM_TX_FRAME_WIDTH_SHIFT            (0U)
#define I2STDM_TDM_TXCTRL_TDM_TX_FRAME_WIDTH_MASK             (0x1FFU << I2STDM_TDM_TXCTRL_TDM_TX_FRAME_WIDTH_SHIFT)          /* 0x000001FF */
#define I2STDM_TDM_TXCTRL_TDM_TX_SLTO_BIT_SHIFT               (9U)
#define I2STDM_TDM_TXCTRL_TDM_TX_SLTO_BIT_MASK                (0x1FU << I2STDM_TDM_TXCTRL_TDM_TX_SLTO_BIT_SHIFT)              /* 0x00003E00 */
#define I2STDM_TDM_TXCTRL_TDM_TX_SHIFT_CTRL_SHIFT             (14U)
#define I2STDM_TDM_TXCTRL_TDM_TX_SHIFT_CTRL_MASK              (0x7U << I2STDM_TDM_TXCTRL_TDM_TX_SHIFT_CTRL_SHIFT)             /* 0x0001C000 */
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL0_SHIFT       (17U)
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL0_MASK        (0x1U << I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL0_SHIFT)       /* 0x00020000 */
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL1_SHIFT       (18U)
#define I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL1_MASK        (0x7U << I2STDM_TDM_TXCTRL_TX_TDM_FSYNC_WIDTH_SEL1_SHIFT)       /* 0x001C0000 */
/* TDM_RXCTRL */
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
#define I2STDM_CLKDIV_TX_MDIV_SHIFT                           (0U)
#define I2STDM_CLKDIV_TX_MDIV_MASK                            (0xFFU << I2STDM_CLKDIV_TX_MDIV_SHIFT)                          /* 0x000000FF */
#define I2STDM_CLKDIV_RX_MDIV_SHIFT                           (8U)
#define I2STDM_CLKDIV_RX_MDIV_MASK                            (0xFFU << I2STDM_CLKDIV_RX_MDIV_SHIFT)                          /* 0x0000FF00 */
/* VERSION */
#define I2STDM_VERSION_I2S0_VERSION_SHIFT                      (0U)
#define I2STDM_VERSION_I2S0_VERSION_MASK                       (0xFFFFFFFFU << I2STDM_VERSION_I2S0_VERSION_SHIFT)               /* 0xFFFFFFFF */
/******************************************PDM*******************************************/
/* SYSCONFIG */
#define PDM_SYSCONFIG_RX_CLR_SHIFT                         (0U)
#define PDM_SYSCONFIG_RX_CLR_MASK                          (0x1U << PDM_SYSCONFIG_RX_CLR_SHIFT)                         /* 0x00000001 */
#define PDM_SYSCONFIG_RX_START_SHIFT                       (2U)
#define PDM_SYSCONFIG_RX_START_MASK                        (0x1U << PDM_SYSCONFIG_RX_START_SHIFT)                       /* 0x00000004 */
/* CTRL0 */
#define PDM_CTRL0_DATA_VLD_WIDTH_SHIFT                     (0U)
#define PDM_CTRL0_DATA_VLD_WIDTH_MASK                      (0x1FU << PDM_CTRL0_DATA_VLD_WIDTH_SHIFT)                    /* 0x0000001F */
#define PDM_CTRL0_INT_DIV_CON_SHIFT                        (8U)
#define PDM_CTRL0_INT_DIV_CON_MASK                         (0xFFU << PDM_CTRL0_INT_DIV_CON_SHIFT)                       /* 0x0000FF00 */
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
#define PDM_CTRL1_FRAC_DIV_DENOMONATOR_SHIFT               (0U)
#define PDM_CTRL1_FRAC_DIV_DENOMONATOR_MASK                (0xFFFFU << PDM_CTRL1_FRAC_DIV_DENOMONATOR_SHIFT)            /* 0x0000FFFF */
#define PDM_CTRL1_FRAC_DIV_NUMERATOR_SHIFT                 (16U)
#define PDM_CTRL1_FRAC_DIV_NUMERATOR_MASK                  (0xFFFFU << PDM_CTRL1_FRAC_DIV_NUMERATOR_SHIFT)              /* 0xFFFF0000 */
/* CLK_CTRL */
#define PDM_CLK_CTRL_PDM_DS_RATIO_SHIFT                    (0U)
#define PDM_CLK_CTRL_PDM_DS_RATIO_MASK                     (0x7U << PDM_CLK_CTRL_PDM_DS_RATIO_SHIFT)                    /* 0x00000007 */
#define PDM_CLK_CTRL_CLK_POLAR_SHIFT                       (3U)
#define PDM_CLK_CTRL_CLK_POLAR_MASK                        (0x1U << PDM_CLK_CTRL_CLK_POLAR_SHIFT)                       /* 0x00000008 */
#define PDM_CLK_CTRL_DIV_TYPE_SEL_SHIFT                    (4U)
#define PDM_CLK_CTRL_DIV_TYPE_SEL_MASK                     (0x1U << PDM_CLK_CTRL_DIV_TYPE_SEL_SHIFT)                    /* 0x00000010 */
#define PDM_CLK_CTRL_PDM_CLK_EN_SHIFT                      (5U)
#define PDM_CLK_CTRL_PDM_CLK_EN_MASK                       (0x1U << PDM_CLK_CTRL_PDM_CLK_EN_SHIFT)                      /* 0x00000020 */
#define PDM_CLK_CTRL_DIV_RATIO_SEL_SHIFT                   (6U)
#define PDM_CLK_CTRL_DIV_RATIO_SEL_MASK                    (0x1U << PDM_CLK_CTRL_DIV_RATIO_SEL_SHIFT)                   /* 0x00000040 */
/* HPF_CTRL */
#define PDM_HPF_CTRL_HPF_CF_SHIFT                          (0U)
#define PDM_HPF_CTRL_HPF_CF_MASK                           (0x3U << PDM_HPF_CTRL_HPF_CF_SHIFT)                          /* 0x00000003 */
#define PDM_HPF_CTRL_HPFRE_SHIFT                           (2U)
#define PDM_HPF_CTRL_HPFRE_MASK                            (0x1U << PDM_HPF_CTRL_HPFRE_SHIFT)                           /* 0x00000004 */
#define PDM_HPF_CTRL_HPFLE_SHIFT                           (3U)
#define PDM_HPF_CTRL_HPFLE_MASK                            (0x1U << PDM_HPF_CTRL_HPFLE_SHIFT)                           /* 0x00000008 */
/* FIFO_CTRL */
#define PDM_FIFO_CTRL_RFL_SHIFT                            (0U)
#define PDM_FIFO_CTRL_RFL_MASK                             (0xFFU << PDM_FIFO_CTRL_RFL_SHIFT)                           /* 0x000000FF */
#define PDM_FIFO_CTRL_RFT_SHIFT                            (8U)
#define PDM_FIFO_CTRL_RFT_MASK                             (0x7FU << PDM_FIFO_CTRL_RFT_SHIFT)                           /* 0x00007F00 */
/* DMA_CTRL */
#define PDM_DMA_CTRL_RDL_SHIFT                             (0U)
#define PDM_DMA_CTRL_RDL_MASK                              (0x7FU << PDM_DMA_CTRL_RDL_SHIFT)                            /* 0x0000007F */
#define PDM_DMA_CTRL_RDE_SHIFT                             (8U)
#define PDM_DMA_CTRL_RDE_MASK                              (0x1U << PDM_DMA_CTRL_RDE_SHIFT)                             /* 0x00000100 */
/* INT_EN */
#define PDM_INT_EN_RXTIE_SHIFT                             (0U)
#define PDM_INT_EN_RXTIE_MASK                              (0x1U << PDM_INT_EN_RXTIE_SHIFT)                             /* 0x00000001 */
#define PDM_INT_EN_RXOIE_SHIFT                             (1U)
#define PDM_INT_EN_RXOIE_MASK                              (0x1U << PDM_INT_EN_RXOIE_SHIFT)                             /* 0x00000002 */
/* INT_CLR */
#define PDM_INT_CLR_RXOIC_SHIFT                            (1U)
#define PDM_INT_CLR_RXOIC_MASK                             (0x1U << PDM_INT_CLR_RXOIC_SHIFT)                            /* 0x00000002 */
/* INT_ST */
#define PDM_INT_ST_RXFI_SHIFT                              (0U)
#define PDM_INT_ST_RXFI_MASK                               (0x1U << PDM_INT_ST_RXFI_SHIFT)                              /* 0x00000001 */
#define PDM_INT_ST_RXOI_SHIFT                              (1U)
#define PDM_INT_ST_RXOI_MASK                               (0x1U << PDM_INT_ST_RXOI_SHIFT)                              /* 0x00000002 */
/* RXFIFO_DATA_REG */
#define PDM_RXFIFO_DATA_REG_RXDR_SHIFT                     (0U)
#define PDM_RXFIFO_DATA_REG_RXDR_MASK                      (0xFFFFFFFFU << PDM_RXFIFO_DATA_REG_RXDR_SHIFT)              /* 0xFFFFFFFF */
/* DATA0R_REG */
#define PDM_DATA0R_REG_DATA0R_SHIFT                        (0U)
#define PDM_DATA0R_REG_DATA0R_MASK                         (0xFFFFFFFFU << PDM_DATA0R_REG_DATA0R_SHIFT)                 /* 0xFFFFFFFF */
/* DATA0L_REG */
#define PDM_DATA0L_REG_DATA0L_SHIFT                        (0U)
#define PDM_DATA0L_REG_DATA0L_MASK                         (0xFFFFFFFFU << PDM_DATA0L_REG_DATA0L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA1R_REG */
#define PDM_DATA1R_REG_DATA1R_SHIFT                        (0U)
#define PDM_DATA1R_REG_DATA1R_MASK                         (0x1U << PDM_DATA1R_REG_DATA1R_SHIFT)                        /* 0x00000001 */
/* DATA1L_REG */
#define PDM_DATA1L_REG_DATA1L_SHIFT                        (0U)
#define PDM_DATA1L_REG_DATA1L_MASK                         (0xFFFFFFFFU << PDM_DATA1L_REG_DATA1L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA2R_REG */
#define PDM_DATA2R_REG_DATA2R_SHIFT                        (0U)
#define PDM_DATA2R_REG_DATA2R_MASK                         (0xFFFFFFFFU << PDM_DATA2R_REG_DATA2R_SHIFT)                 /* 0xFFFFFFFF */
/* DATA2L_REG */
#define PDM_DATA2L_REG_DATA2L_SHIFT                        (0U)
#define PDM_DATA2L_REG_DATA2L_MASK                         (0xFFFFFFFFU << PDM_DATA2L_REG_DATA2L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA3R_REG */
#define PDM_DATA3R_REG_DATA3R_SHIFT                        (0U)
#define PDM_DATA3R_REG_DATA3R_MASK                         (0xFFFFFFFFU << PDM_DATA3R_REG_DATA3R_SHIFT)                 /* 0xFFFFFFFF */
/* DATA3L_REG */
#define PDM_DATA3L_REG_DATA3L_SHIFT                        (0U)
#define PDM_DATA3L_REG_DATA3L_MASK                         (0xFFFFFFFFU << PDM_DATA3L_REG_DATA3L_SHIFT)                 /* 0xFFFFFFFF */
/* DATA_VALID */
#define PDM_DATA_VALID_PATH3_VLD_SHIFT                     (0U)
#define PDM_DATA_VALID_PATH3_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH3_VLD_SHIFT)                     /* 0x00000001 */
#define PDM_DATA_VALID_PATH2_VLD_SHIFT                     (1U)
#define PDM_DATA_VALID_PATH2_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH2_VLD_SHIFT)                     /* 0x00000002 */
#define PDM_DATA_VALID_PATH1_VLD_SHIFT                     (2U)
#define PDM_DATA_VALID_PATH1_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH1_VLD_SHIFT)                     /* 0x00000004 */
#define PDM_DATA_VALID_PATH0_VLD_SHIFT                     (3U)
#define PDM_DATA_VALID_PATH0_VLD_MASK                      (0x1U << PDM_DATA_VALID_PATH0_VLD_SHIFT)                     /* 0x00000008 */
/* VERSION */
#define PDM_VERSION_VERSION_SHIFT                          (0U)
#define PDM_VERSION_VERSION_MASK                           (0xFFFFFFFFU << PDM_VERSION_VERSION_SHIFT)                   /* 0xFFFFFFFF */
/* INCR_RXDR */
#define PDM_INCR_RXDR_RECEIVE_FIFO_DATA_SHIFT              (0U)
#define PDM_INCR_RXDR_RECEIVE_FIFO_DATA_MASK               (0xFFFFFFFFU << PDM_INCR_RXDR_RECEIVE_FIFO_DATA_SHIFT)       /* 0xFFFFFFFF */
/******************************************VAD*******************************************/
/* CONTROL */
#define VAD_CONTROL_VAD_EN_SHIFT                           (0U)
#define VAD_CONTROL_VAD_EN_MASK                            (0x1U << VAD_CONTROL_VAD_EN_SHIFT)                           /* 0x00000001 */
#define VAD_CONTROL_SOURCE_SELECT_SHIFT                    (1U)
#define VAD_CONTROL_SOURCE_SELECT_MASK                     (0x7U << VAD_CONTROL_SOURCE_SELECT_SHIFT)                    /* 0x0000000E */
#define VAD_CONTROL_SOURCE_BURST_SHIFT                     (4U)
#define VAD_CONTROL_SOURCE_BURST_MASK                      (0x7U << VAD_CONTROL_SOURCE_BURST_SHIFT)                     /* 0x00000070 */
#define VAD_CONTROL_SOURCE_BURST_NUM_SHIFT                 (7U)
#define VAD_CONTROL_SOURCE_BURST_NUM_MASK                  (0x7U << VAD_CONTROL_SOURCE_BURST_NUM_SHIFT)                 /* 0x00000380 */
#define VAD_CONTROL_INCR_LENGTH_SHIFT                      (10U)
#define VAD_CONTROL_INCR_LENGTH_MASK                       (0xFU << VAD_CONTROL_INCR_LENGTH_SHIFT)                      /* 0x00003C00 */
#define VAD_CONTROL_SOURCE_FIXADDR_EN_SHIFT                (14U)
#define VAD_CONTROL_SOURCE_FIXADDR_EN_MASK                 (0x1U << VAD_CONTROL_SOURCE_FIXADDR_EN_SHIFT)                /* 0x00004000 */
#define VAD_CONTROL_ACODEC_CFG_REG_NUM_SHIFT               (15U)
#define VAD_CONTROL_ACODEC_CFG_REG_NUM_MASK                (0x1FU << VAD_CONTROL_ACODEC_CFG_REG_NUM_SHIFT)              /* 0x000F8000 */
#define VAD_CONTROL_VAD_MODE_SHIFT                         (20U)
#define VAD_CONTROL_VAD_MODE_MASK                          (0x3U << VAD_CONTROL_VAD_MODE_SHIFT)                         /* 0x00300000 */
#define VAD_CONTROL_CONFIG_AFTER_DET_EN_SHIFT              (22U)
#define VAD_CONTROL_CONFIG_AFTER_DET_EN_MASK               (0x1U << VAD_CONTROL_CONFIG_AFTER_DET_EN_SHIFT)              /* 0x00400000 */
#define VAD_CONTROL_VOICE_CHANNEL_NUM_SHIFT                (23U)
#define VAD_CONTROL_VOICE_CHANNEL_NUM_MASK                 (0x7U << VAD_CONTROL_VOICE_CHANNEL_NUM_SHIFT)                /* 0x03800000 */
#define VAD_CONTROL_VOICE_CHANNEL_BITWIDTH_SHIFT           (26U)
#define VAD_CONTROL_VOICE_CHANNEL_BITWIDTH_MASK            (0x1U << VAD_CONTROL_VOICE_CHANNEL_BITWIDTH_SHIFT)           /* 0x04000000 */
#define VAD_CONTROL_VOICE_24BIT_ALIGN_MODE_SHIFT           (27U)
#define VAD_CONTROL_VOICE_24BIT_ALIGN_MODE_MASK            (0x1U << VAD_CONTROL_VOICE_24BIT_ALIGN_MODE_SHIFT)           /* 0x08000000 */
#define VAD_CONTROL_VOICE_24BIT_SAT_SHIFT                  (28U)
#define VAD_CONTROL_VOICE_24BIT_SAT_MASK                   (0x1U << VAD_CONTROL_VOICE_24BIT_SAT_SHIFT)                  /* 0x10000000 */
#define VAD_CONTROL_VAD_DET_CHANNEL_SHIFT                  (29U)
#define VAD_CONTROL_VAD_DET_CHANNEL_MASK                   (0x7U << VAD_CONTROL_VAD_DET_CHANNEL_SHIFT)                  /* 0xE0000000 */
/* VS_ADDR */
#define VAD_VS_ADDR_VS_ADDR_SHIFT                          (0U)
#define VAD_VS_ADDR_VS_ADDR_MASK                           (0xFFFFFFFFU << VAD_VS_ADDR_VS_ADDR_SHIFT)                   /* 0xFFFFFFFF */
/* ACODEC_BASE_ADDR */
#define VAD_ACODEC_BASE_ADDR_ACODEC_BASE_ADDR_SHIFT        (0U)
#define VAD_ACODEC_BASE_ADDR_ACODEC_BASE_ADDR_MASK         (0xFFFFFFFFU << VAD_ACODEC_BASE_ADDR_ACODEC_BASE_ADDR_SHIFT) /* 0xFFFFFFFF */
/* OD_ADDR0 */
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_0_SHIFT            (0U)
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_0_MASK             (0xFFU << VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_0_SHIFT)           /* 0x000000FF */
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_1_SHIFT            (8U)
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_1_MASK             (0xFFU << VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_1_SHIFT)           /* 0x0000FF00 */
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_2_SHIFT            (16U)
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_2_MASK             (0xFFU << VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_2_SHIFT)           /* 0x00FF0000 */
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_3_SHIFT            (24U)
#define VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_3_MASK             (0xFFU << VAD_OD_ADDR0_OFFSET_ACODEC_ADDR_3_SHIFT)           /* 0xFF000000 */
/* OD_ADDR1 */
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_4_SHIFT            (0U)
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_4_MASK             (0xFFU << VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_4_SHIFT)           /* 0x000000FF */
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_5_SHIFT            (8U)
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_5_MASK             (0xFFU << VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_5_SHIFT)           /* 0x0000FF00 */
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_6_SHIFT            (16U)
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_6_MASK             (0xFFU << VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_6_SHIFT)           /* 0x00FF0000 */
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_7_SHIFT            (24U)
#define VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_7_MASK             (0xFFU << VAD_OD_ADDR1_OFFSET_ACODEC_ADDR_7_SHIFT)           /* 0xFF000000 */
/* OD_ADDR2 */
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_8_SHIFT            (0U)
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_8_MASK             (0xFFU << VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_8_SHIFT)           /* 0x000000FF */
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_9_SHIFT            (8U)
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_9_MASK             (0xFFU << VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_9_SHIFT)           /* 0x0000FF00 */
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_10_SHIFT           (16U)
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_10_MASK            (0xFFU << VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_10_SHIFT)          /* 0x00FF0000 */
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_11_SHIFT           (24U)
#define VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_11_MASK            (0xFFU << VAD_OD_ADDR2_OFFSET_ACODEC_ADDR_11_SHIFT)          /* 0xFF000000 */
/* OD_ADDR3 */
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_12_SHIFT           (0U)
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_12_MASK            (0xFFU << VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_12_SHIFT)          /* 0x000000FF */
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_13_SHIFT           (8U)
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_13_MASK            (0xFFU << VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_13_SHIFT)          /* 0x0000FF00 */
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_14_SHIFT           (16U)
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_14_MASK            (0xFFU << VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_14_SHIFT)          /* 0x00FF0000 */
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_15_SHIFT           (24U)
#define VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_15_MASK            (0xFFU << VAD_OD_ADDR3_OFFSET_ACODEC_ADDR_15_SHIFT)          /* 0xFF000000 */
/* OD_ADDR4 */
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_16_SHIFT           (0U)
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_16_MASK            (0xFFU << VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_16_SHIFT)          /* 0x000000FF */
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_17_SHIFT           (8U)
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_17_MASK            (0xFFU << VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_17_SHIFT)          /* 0x0000FF00 */
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_18_SHIFT           (16U)
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_18_MASK            (0xFFU << VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_18_SHIFT)          /* 0x00FF0000 */
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_19_SHIFT           (24U)
#define VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_19_MASK            (0xFFU << VAD_OD_ADDR4_OFFSET_ACODEC_ADDR_19_SHIFT)          /* 0xFF000000 */
/* OD_ADDR5 */
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_20_SHIFT           (0U)
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_20_MASK            (0xFFU << VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_20_SHIFT)          /* 0x000000FF */
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_21_SHIFT           (8U)
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_21_MASK            (0xFFU << VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_21_SHIFT)          /* 0x0000FF00 */
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_22_SHIFT           (16U)
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_22_MASK            (0xFFU << VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_22_SHIFT)          /* 0x00FF0000 */
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_23_SHIFT           (24U)
#define VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_23_MASK            (0xFFU << VAD_OD_ADDR5_OFFSET_ACODEC_ADDR_23_SHIFT)          /* 0xFF000000 */
/* OD_ADDR6 */
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_24_SHIFT           (0U)
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_24_MASK            (0xFFU << VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_24_SHIFT)          /* 0x000000FF */
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_25_SHIFT           (8U)
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_25_MASK            (0xFFU << VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_25_SHIFT)          /* 0x0000FF00 */
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_26_SHIFT           (16U)
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_26_MASK            (0xFFU << VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_26_SHIFT)          /* 0x00FF0000 */
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_27_SHIFT           (24U)
#define VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_27_MASK            (0xFFU << VAD_OD_ADDR6_OFFSET_ACODEC_ADDR_27_SHIFT)          /* 0xFF000000 */
/* OD_ADDR7 */
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_28_SHIFT           (0U)
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_28_MASK            (0xFFU << VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_28_SHIFT)          /* 0x000000FF */
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_29_SHIFT           (8U)
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_29_MASK            (0xFFU << VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_29_SHIFT)          /* 0x0000FF00 */
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_30_SHIFT           (16U)
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_30_MASK            (0xFFU << VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_30_SHIFT)          /* 0x00FF0000 */
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_31_SHIFT           (24U)
#define VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_31_MASK            (0xFFU << VAD_OD_ADDR7_OFFSET_ACODEC_ADDR_31_SHIFT)          /* 0xFF000000 */
/* D_DATA0 */
#define VAD_D_DATA0_ACODEC_DATA_0_SHIFT                    (0U)
#define VAD_D_DATA0_ACODEC_DATA_0_MASK                     (0xFFU << VAD_D_DATA0_ACODEC_DATA_0_SHIFT)                   /* 0x000000FF */
#define VAD_D_DATA0_ACODEC_DATA_1_SHIFT                    (8U)
#define VAD_D_DATA0_ACODEC_DATA_1_MASK                     (0xFFU << VAD_D_DATA0_ACODEC_DATA_1_SHIFT)                   /* 0x0000FF00 */
#define VAD_D_DATA0_ACODEC_DATA_2_SHIFT                    (16U)
#define VAD_D_DATA0_ACODEC_DATA_2_MASK                     (0xFFU << VAD_D_DATA0_ACODEC_DATA_2_SHIFT)                   /* 0x00FF0000 */
#define VAD_D_DATA0_ACODEC_DATA_3_SHIFT                    (24U)
#define VAD_D_DATA0_ACODEC_DATA_3_MASK                     (0xFFU << VAD_D_DATA0_ACODEC_DATA_3_SHIFT)                   /* 0xFF000000 */
/* D_DATA1 */
#define VAD_D_DATA1_ACODEC_DATA_4_SHIFT                    (0U)
#define VAD_D_DATA1_ACODEC_DATA_4_MASK                     (0xFFU << VAD_D_DATA1_ACODEC_DATA_4_SHIFT)                   /* 0x000000FF */
#define VAD_D_DATA1_ACODEC_DATA_5_SHIFT                    (8U)
#define VAD_D_DATA1_ACODEC_DATA_5_MASK                     (0xFFU << VAD_D_DATA1_ACODEC_DATA_5_SHIFT)                   /* 0x0000FF00 */
#define VAD_D_DATA1_ACODEC_DATA_6_SHIFT                    (16U)
#define VAD_D_DATA1_ACODEC_DATA_6_MASK                     (0xFFU << VAD_D_DATA1_ACODEC_DATA_6_SHIFT)                   /* 0x00FF0000 */
#define VAD_D_DATA1_ACODEC_DATA_7_SHIFT                    (24U)
#define VAD_D_DATA1_ACODEC_DATA_7_MASK                     (0xFFU << VAD_D_DATA1_ACODEC_DATA_7_SHIFT)                   /* 0xFF000000 */
/* D_DATA2 */
#define VAD_D_DATA2_ACODEC_DATA_8_SHIFT                    (0U)
#define VAD_D_DATA2_ACODEC_DATA_8_MASK                     (0xFFU << VAD_D_DATA2_ACODEC_DATA_8_SHIFT)                   /* 0x000000FF */
#define VAD_D_DATA2_ACODEC_DATA_9_SHIFT                    (8U)
#define VAD_D_DATA2_ACODEC_DATA_9_MASK                     (0xFFU << VAD_D_DATA2_ACODEC_DATA_9_SHIFT)                   /* 0x0000FF00 */
#define VAD_D_DATA2_ACODEC_DATA_10_SHIFT                   (16U)
#define VAD_D_DATA2_ACODEC_DATA_10_MASK                    (0xFFU << VAD_D_DATA2_ACODEC_DATA_10_SHIFT)                  /* 0x00FF0000 */
#define VAD_D_DATA2_ACODEC_DATA_11_SHIFT                   (24U)
#define VAD_D_DATA2_ACODEC_DATA_11_MASK                    (0xFFU << VAD_D_DATA2_ACODEC_DATA_11_SHIFT)                  /* 0xFF000000 */
/* D_DATA3 */
#define VAD_D_DATA3_ACODEC_DATA_12_SHIFT                   (0U)
#define VAD_D_DATA3_ACODEC_DATA_12_MASK                    (0xFFU << VAD_D_DATA3_ACODEC_DATA_12_SHIFT)                  /* 0x000000FF */
#define VAD_D_DATA3_ACODEC_DATA_13_SHIFT                   (8U)
#define VAD_D_DATA3_ACODEC_DATA_13_MASK                    (0xFFU << VAD_D_DATA3_ACODEC_DATA_13_SHIFT)                  /* 0x0000FF00 */
#define VAD_D_DATA3_ACODEC_DATA_14_SHIFT                   (16U)
#define VAD_D_DATA3_ACODEC_DATA_14_MASK                    (0xFFU << VAD_D_DATA3_ACODEC_DATA_14_SHIFT)                  /* 0x00FF0000 */
#define VAD_D_DATA3_ACODEC_DATA_15_SHIFT                   (24U)
#define VAD_D_DATA3_ACODEC_DATA_15_MASK                    (0xFFU << VAD_D_DATA3_ACODEC_DATA_15_SHIFT)                  /* 0xFF000000 */
/* D_DATA4 */
#define VAD_D_DATA4_ACODEC_DATA_16_SHIFT                   (0U)
#define VAD_D_DATA4_ACODEC_DATA_16_MASK                    (0xFFU << VAD_D_DATA4_ACODEC_DATA_16_SHIFT)                  /* 0x000000FF */
#define VAD_D_DATA4_ACODEC_DATA_17_SHIFT                   (8U)
#define VAD_D_DATA4_ACODEC_DATA_17_MASK                    (0xFFU << VAD_D_DATA4_ACODEC_DATA_17_SHIFT)                  /* 0x0000FF00 */
#define VAD_D_DATA4_ACODEC_DATA_18_SHIFT                   (16U)
#define VAD_D_DATA4_ACODEC_DATA_18_MASK                    (0xFFU << VAD_D_DATA4_ACODEC_DATA_18_SHIFT)                  /* 0x00FF0000 */
#define VAD_D_DATA4_ACODEC_DATA_19_SHIFT                   (24U)
#define VAD_D_DATA4_ACODEC_DATA_19_MASK                    (0xFFU << VAD_D_DATA4_ACODEC_DATA_19_SHIFT)                  /* 0xFF000000 */
/* D_DATA5 */
#define VAD_D_DATA5_ACODEC_DATA_20_SHIFT                   (0U)
#define VAD_D_DATA5_ACODEC_DATA_20_MASK                    (0xFFU << VAD_D_DATA5_ACODEC_DATA_20_SHIFT)                  /* 0x000000FF */
#define VAD_D_DATA5_ACODEC_DATA_21_SHIFT                   (8U)
#define VAD_D_DATA5_ACODEC_DATA_21_MASK                    (0xFFU << VAD_D_DATA5_ACODEC_DATA_21_SHIFT)                  /* 0x0000FF00 */
#define VAD_D_DATA5_ACODEC_DATA_22_SHIFT                   (16U)
#define VAD_D_DATA5_ACODEC_DATA_22_MASK                    (0xFFU << VAD_D_DATA5_ACODEC_DATA_22_SHIFT)                  /* 0x00FF0000 */
#define VAD_D_DATA5_ACODEC_DATA_23_SHIFT                   (24U)
#define VAD_D_DATA5_ACODEC_DATA_23_MASK                    (0xFFU << VAD_D_DATA5_ACODEC_DATA_23_SHIFT)                  /* 0xFF000000 */
/* D_DATA6 */
#define VAD_D_DATA6_ACODEC_DATA_24_SHIFT                   (0U)
#define VAD_D_DATA6_ACODEC_DATA_24_MASK                    (0xFFU << VAD_D_DATA6_ACODEC_DATA_24_SHIFT)                  /* 0x000000FF */
#define VAD_D_DATA6_ACODEC_DATA_25_SHIFT                   (8U)
#define VAD_D_DATA6_ACODEC_DATA_25_MASK                    (0xFFU << VAD_D_DATA6_ACODEC_DATA_25_SHIFT)                  /* 0x0000FF00 */
#define VAD_D_DATA6_ACODEC_DATA_26_SHIFT                   (16U)
#define VAD_D_DATA6_ACODEC_DATA_26_MASK                    (0xFFU << VAD_D_DATA6_ACODEC_DATA_26_SHIFT)                  /* 0x00FF0000 */
#define VAD_D_DATA6_ACODEC_DATA_27_SHIFT                   (24U)
#define VAD_D_DATA6_ACODEC_DATA_27_MASK                    (0xFFU << VAD_D_DATA6_ACODEC_DATA_27_SHIFT)                  /* 0xFF000000 */
/* D_DATA7 */
#define VAD_D_DATA7_ACODEC_DATA_28_SHIFT                   (0U)
#define VAD_D_DATA7_ACODEC_DATA_28_MASK                    (0xFFU << VAD_D_DATA7_ACODEC_DATA_28_SHIFT)                  /* 0x000000FF */
#define VAD_D_DATA7_ACODEC_DATA_29_SHIFT                   (8U)
#define VAD_D_DATA7_ACODEC_DATA_29_MASK                    (0xFFU << VAD_D_DATA7_ACODEC_DATA_29_SHIFT)                  /* 0x0000FF00 */
#define VAD_D_DATA7_ACODEC_DATA_30_SHIFT                   (16U)
#define VAD_D_DATA7_ACODEC_DATA_30_MASK                    (0xFFU << VAD_D_DATA7_ACODEC_DATA_30_SHIFT)                  /* 0x00FF0000 */
#define VAD_D_DATA7_ACODEC_DATA_31_SHIFT                   (24U)
#define VAD_D_DATA7_ACODEC_DATA_31_MASK                    (0xFFU << VAD_D_DATA7_ACODEC_DATA_31_SHIFT)                  /* 0xFF000000 */
/* TIMEOUT */
#define VAD_TIMEOUT_IDLE_TIMEOUT_THD_SHIFT                 (0U)
#define VAD_TIMEOUT_IDLE_TIMEOUT_THD_MASK                  (0xFFFFFU << VAD_TIMEOUT_IDLE_TIMEOUT_THD_SHIFT)             /* 0x000FFFFF */
#define VAD_TIMEOUT_WORK_TIMEOUT_THD_SHIFT                 (20U)
#define VAD_TIMEOUT_WORK_TIMEOUT_THD_MASK                  (0x3FFU << VAD_TIMEOUT_WORK_TIMEOUT_THD_SHIFT)               /* 0x3FF00000 */
#define VAD_TIMEOUT_IDLE_TIMEOUT_EN_SHIFT                  (30U)
#define VAD_TIMEOUT_IDLE_TIMEOUT_EN_MASK                   (0x1U << VAD_TIMEOUT_IDLE_TIMEOUT_EN_SHIFT)                  /* 0x40000000 */
#define VAD_TIMEOUT_WORK_TIMEOUT_EN_SHIFT                  (31U)
#define VAD_TIMEOUT_WORK_TIMEOUT_EN_MASK                   (0x1U << VAD_TIMEOUT_WORK_TIMEOUT_EN_SHIFT)                  /* 0x80000000 */
/* RAM_START_ADDR */
#define VAD_RAM_START_ADDR_RAM_BEGIN_ADDR_SHIFT            (0U)
#define VAD_RAM_START_ADDR_RAM_BEGIN_ADDR_MASK             (0xFFFFFFFFU << VAD_RAM_START_ADDR_RAM_BEGIN_ADDR_SHIFT)     /* 0xFFFFFFFF */
/* RAM_END_ADDR */
#define VAD_RAM_END_ADDR_RAM_END_ADDR_SHIFT                (0U)
#define VAD_RAM_END_ADDR_RAM_END_ADDR_MASK                 (0xFFFFFFFFU << VAD_RAM_END_ADDR_RAM_END_ADDR_SHIFT)         /* 0xFFFFFFFF */
/* RAM_CUR_ADDR */
#define VAD_RAM_CUR_ADDR_RAM_CUR_ADDR_SHIFT                (0U)
#define VAD_RAM_CUR_ADDR_RAM_CUR_ADDR_MASK                 (0xFFFFFFFFU << VAD_RAM_CUR_ADDR_RAM_CUR_ADDR_SHIFT)         /* 0xFFFFFFFF */
/* DET_CON0 */
#define VAD_DET_CON0_GAIN_SHIFT                            (0U)
#define VAD_DET_CON0_GAIN_MASK                             (0xFFFU << VAD_DET_CON0_GAIN_SHIFT)                          /* 0x00000FFF */
#define VAD_DET_CON0_NOISE_LEVEL_SHIFT                     (12U)
#define VAD_DET_CON0_NOISE_LEVEL_MASK                      (0x7U << VAD_DET_CON0_NOISE_LEVEL_SHIFT)                     /* 0x00007000 */
#define VAD_DET_CON0_VAD_CON_THD_SHIFT                     (16U)
#define VAD_DET_CON0_VAD_CON_THD_MASK                      (0xFFU << VAD_DET_CON0_VAD_CON_THD_SHIFT)                    /* 0x00FF0000 */
#define VAD_DET_CON0_DIS_VAD_CON_THD_SHIFT                 (24U)
#define VAD_DET_CON0_DIS_VAD_CON_THD_MASK                  (0xFU << VAD_DET_CON0_DIS_VAD_CON_THD_SHIFT)                 /* 0x0F000000 */
#define VAD_DET_CON0_VAD_THD_MODE_SHIFT                    (28U)
#define VAD_DET_CON0_VAD_THD_MODE_MASK                     (0x3U << VAD_DET_CON0_VAD_THD_MODE_SHIFT)                    /* 0x30000000 */
/* DET_CON1 */
#define VAD_DET_CON1_SOUND_THD_SHIFT                       (0U)
#define VAD_DET_CON1_SOUND_THD_MASK                        (0xFFFFU << VAD_DET_CON1_SOUND_THD_SHIFT)                    /* 0x0000FFFF */
#define VAD_DET_CON1_NOISE_SAMPLE_NUM_SHIFT                (16U)
#define VAD_DET_CON1_NOISE_SAMPLE_NUM_MASK                 (0x3FFU << VAD_DET_CON1_NOISE_SAMPLE_NUM_SHIFT)              /* 0x03FF0000 */
#define VAD_DET_CON1_CLEAN_IIR_EN_SHIFT                    (26U)
#define VAD_DET_CON1_CLEAN_IIR_EN_MASK                     (0x1U << VAD_DET_CON1_CLEAN_IIR_EN_SHIFT)                    /* 0x04000000 */
#define VAD_DET_CON1_FORCE_NOISE_CLK_EN_SHIFT              (28U)
#define VAD_DET_CON1_FORCE_NOISE_CLK_EN_MASK               (0x1U << VAD_DET_CON1_FORCE_NOISE_CLK_EN_SHIFT)              /* 0x10000000 */
#define VAD_DET_CON1_CLEAN_NOISE_AT_BEGIN_SHIFT            (29U)
#define VAD_DET_CON1_CLEAN_NOISE_AT_BEGIN_MASK             (0x1U << VAD_DET_CON1_CLEAN_NOISE_AT_BEGIN_SHIFT)            /* 0x20000000 */
#define VAD_DET_CON1_MIN_NOISE_FIND_MODE_SHIFT             (30U)
#define VAD_DET_CON1_MIN_NOISE_FIND_MODE_MASK              (0x1U << VAD_DET_CON1_MIN_NOISE_FIND_MODE_SHIFT)             /* 0x40000000 */
/* DET_CON2 */
#define VAD_DET_CON2_NOISE_FRM_NUM_SHIFT                   (0U)
#define VAD_DET_CON2_NOISE_FRM_NUM_MASK                    (0x7FU << VAD_DET_CON2_NOISE_FRM_NUM_SHIFT)                  /* 0x0000007F */
#define VAD_DET_CON2_NOISE_ALPHA_SHIFT                     (8U)
#define VAD_DET_CON2_NOISE_ALPHA_MASK                      (0xFFU << VAD_DET_CON2_NOISE_ALPHA_SHIFT)                    /* 0x0000FF00 */
#define VAD_DET_CON2_IIR_ANUM_0_SHIFT                      (16U)
#define VAD_DET_CON2_IIR_ANUM_0_MASK                       (0xFFFFU << VAD_DET_CON2_IIR_ANUM_0_SHIFT)                   /* 0xFFFF0000 */
/* DET_CON3 */
#define VAD_DET_CON3_IIR_ANUM_1_SHIFT                      (0U)
#define VAD_DET_CON3_IIR_ANUM_1_MASK                       (0xFFFFU << VAD_DET_CON3_IIR_ANUM_1_SHIFT)                   /* 0x0000FFFF */
#define VAD_DET_CON3_IIR_ANUM_2_SHIFT                      (16U)
#define VAD_DET_CON3_IIR_ANUM_2_MASK                       (0xFFFFU << VAD_DET_CON3_IIR_ANUM_2_SHIFT)                   /* 0xFFFF0000 */
/* DET_CON4 */
#define VAD_DET_CON4_IIR_ADEN_1_SHIFT                      (0U)
#define VAD_DET_CON4_IIR_ADEN_1_MASK                       (0xFFFFU << VAD_DET_CON4_IIR_ADEN_1_SHIFT)                   /* 0x0000FFFF */
#define VAD_DET_CON4_IIR_ADEN_2_SHIFT                      (16U)
#define VAD_DET_CON4_IIR_ADEN_2_MASK                       (0xFFFFU << VAD_DET_CON4_IIR_ADEN_2_SHIFT)                   /* 0xFFFF0000 */
/* DET_CON5 */
#define VAD_DET_CON5_NOISE_ABS_SHIFT                       (0U)
#define VAD_DET_CON5_NOISE_ABS_MASK                        (0xFFFFU << VAD_DET_CON5_NOISE_ABS_SHIFT)                    /* 0x0000FFFF */
#define VAD_DET_CON5_IIR_RESULT_SHIFT                      (16U)
#define VAD_DET_CON5_IIR_RESULT_MASK                       (0xFFFFU << VAD_DET_CON5_IIR_RESULT_SHIFT)                   /* 0xFFFF0000 */
/* INT */
#define VAD_INT_VAD_DET_INT_EN_SHIFT                       (0U)
#define VAD_INT_VAD_DET_INT_EN_MASK                        (0x1U << VAD_INT_VAD_DET_INT_EN_SHIFT)                       /* 0x00000001 */
#define VAD_INT_ERROR_INT_EN_SHIFT                         (1U)
#define VAD_INT_ERROR_INT_EN_MASK                          (0x1U << VAD_INT_ERROR_INT_EN_SHIFT)                         /* 0x00000002 */
#define VAD_INT_IDLE_TIMEOUT_INT_EN_SHIFT                  (2U)
#define VAD_INT_IDLE_TIMEOUT_INT_EN_MASK                   (0x1U << VAD_INT_IDLE_TIMEOUT_INT_EN_SHIFT)                  /* 0x00000004 */
#define VAD_INT_WORK_TIMEOUT_INT_EN_SHIFT                  (3U)
#define VAD_INT_WORK_TIMEOUT_INT_EN_MASK                   (0x1U << VAD_INT_WORK_TIMEOUT_INT_EN_SHIFT)                  /* 0x00000008 */
#define VAD_INT_VAD_DET_INT_SHIFT                          (4U)
#define VAD_INT_VAD_DET_INT_MASK                           (0x1U << VAD_INT_VAD_DET_INT_SHIFT)                          /* 0x00000010 */
#define VAD_INT_ERROR_INT_SHIFT                            (5U)
#define VAD_INT_ERROR_INT_MASK                             (0x1U << VAD_INT_ERROR_INT_SHIFT)                            /* 0x00000020 */
#define VAD_INT_IDLE_TIMEOUT_INT_SHIFT                     (6U)
#define VAD_INT_IDLE_TIMEOUT_INT_MASK                      (0x1U << VAD_INT_IDLE_TIMEOUT_INT_SHIFT)                     /* 0x00000040 */
#define VAD_INT_WORK_TIMEOUT_INT_SHIFT                     (7U)
#define VAD_INT_WORK_TIMEOUT_INT_MASK                      (0x1U << VAD_INT_WORK_TIMEOUT_INT_SHIFT)                     /* 0x00000080 */
#define VAD_INT_RAMP_LOOP_FLAG_SHIFT                       (8U)
#define VAD_INT_RAMP_LOOP_FLAG_MASK                        (0x1U << VAD_INT_RAMP_LOOP_FLAG_SHIFT)                       /* 0x00000100 */
#define VAD_INT_VAD_IDLE_SHIFT                             (9U)
#define VAD_INT_VAD_IDLE_MASK                              (0x1U << VAD_INT_VAD_IDLE_SHIFT)                             /* 0x00000200 */
#define VAD_INT_VAD_DATA_TRANS_INT_EN_SHIFT                (10U)
#define VAD_INT_VAD_DATA_TRANS_INT_EN_MASK                 (0x1U << VAD_INT_VAD_DATA_TRANS_INT_EN_SHIFT)                /* 0x00000400 */
#define VAD_INT_VAD_DATA_TRANS_INT_SHIFT                   (11U)
#define VAD_INT_VAD_DATA_TRANS_INT_MASK                    (0x1U << VAD_INT_VAD_DATA_TRANS_INT_SHIFT)                   /* 0x00000800 */
/* AUX_CONTROL */
#define VAD_AUX_CONTROL_BUS_WRITE_EN_SHIFT                 (0U)
#define VAD_AUX_CONTROL_BUS_WRITE_EN_MASK                  (0x1U << VAD_AUX_CONTROL_BUS_WRITE_EN_SHIFT)                 /* 0x00000001 */
#define VAD_AUX_CONTROL_DIS_RAM_ITF_SHIFT                  (1U)
#define VAD_AUX_CONTROL_DIS_RAM_ITF_MASK                   (0x1U << VAD_AUX_CONTROL_DIS_RAM_ITF_SHIFT)                  /* 0x00000002 */
#define VAD_AUX_CONTROL_DATA_TRANS_TRIG_INT_EN_SHIFT       (2U)
#define VAD_AUX_CONTROL_DATA_TRANS_TRIG_INT_EN_MASK        (0x1U << VAD_AUX_CONTROL_DATA_TRANS_TRIG_INT_EN_SHIFT)       /* 0x00000004 */
#define VAD_AUX_CONTROL_DATA_TRANS_KBYTE_THD_SHIFT         (4U)
#define VAD_AUX_CONTROL_DATA_TRANS_KBYTE_THD_MASK          (0xFFU << VAD_AUX_CONTROL_DATA_TRANS_KBYTE_THD_SHIFT)        /* 0x00000FF0 */
#define VAD_AUX_CONTROL_INT_TRIG_VALID_THD_SHIFT           (16U)
#define VAD_AUX_CONTROL_INT_TRIG_VALID_THD_MASK            (0xFFFU << VAD_AUX_CONTROL_INT_TRIG_VALID_THD_SHIFT)         /* 0x0FFF0000 */
#define VAD_AUX_CONTROL_INT_TRIG_CTRL_EN_SHIFT             (28U)
#define VAD_AUX_CONTROL_INT_TRIG_CTRL_EN_MASK              (0x1U << VAD_AUX_CONTROL_INT_TRIG_CTRL_EN_SHIFT)             /* 0x10000000 */
#define VAD_AUX_CONTROL_SAMPLE_CNT_EN_SHIFT                (29U)
#define VAD_AUX_CONTROL_SAMPLE_CNT_EN_MASK                 (0x1U << VAD_AUX_CONTROL_SAMPLE_CNT_EN_SHIFT)                /* 0x20000000 */
/* SAMPLE_CNT */
#define VAD_SAMPLE_CNT_SAMPLE_CNT_SHIFT                    (0U)
#define VAD_SAMPLE_CNT_SAMPLE_CNT_MASK                     (0xFFFFFFFFU << VAD_SAMPLE_CNT_SAMPLE_CNT_SHIFT)             /* 0xFFFFFFFF */
/* NOISE_DATA */
#define VAD_NOISE_DATA_NOISE_DATA_SHIFT                    (0U)
#define VAD_NOISE_DATA_NOISE_DATA_MASK                     (0xFFFFU << VAD_NOISE_DATA_NOISE_DATA_SHIFT)                 /* 0x0000FFFF */
/****************************************DFICTRL*****************************************/
/* START */
#define DFICTRL_START_START_SHIFT                          (0U)
#define DFICTRL_START_START_MASK                           (0x1U << DFICTRL_START_START_SHIFT)                          /* 0x00000001 */
#define DFICTRL_START_DFI_SEL_SHIFT                        (1U)
#define DFICTRL_START_DFI_SEL_MASK                         (0x1U << DFICTRL_START_DFI_SEL_SHIFT)                        /* 0x00000002 */
#define DFICTRL_START_IS_DDR4_SHIFT                        (2U)
#define DFICTRL_START_IS_DDR4_MASK                         (0x1U << DFICTRL_START_IS_DDR4_SHIFT)                        /* 0x00000004 */
/* ADDR */
#define DFICTRL_ADDR_DFI_ADDR_SHIFT                        (0U)
#define DFICTRL_ADDR_DFI_ADDR_MASK                         (0xFFFFFU << DFICTRL_ADDR_DFI_ADDR_SHIFT)                    /* 0x000FFFFF */
#define DFICTRL_ADDR_DFI_BANK_SHIFT                        (20U)
#define DFICTRL_ADDR_DFI_BANK_MASK                         (0x7U << DFICTRL_ADDR_DFI_BANK_SHIFT)                        /* 0x00700000 */
#define DFICTRL_ADDR_DFI_WEN_N_A14_SHIFT                   (23U)
#define DFICTRL_ADDR_DFI_WEN_N_A14_MASK                    (0x1U << DFICTRL_ADDR_DFI_WEN_N_A14_SHIFT)                   /* 0x00800000 */
#define DFICTRL_ADDR_DFI_BG_SHIFT                          (24U)
#define DFICTRL_ADDR_DFI_BG_MASK                           (0x3U << DFICTRL_ADDR_DFI_BG_SHIFT)                          /* 0x03000000 */
#define DFICTRL_ADDR_DFI_CAS_N_A15_SHIFT                   (26U)
#define DFICTRL_ADDR_DFI_CAS_N_A15_MASK                    (0x1U << DFICTRL_ADDR_DFI_CAS_N_A15_SHIFT)                   /* 0x04000000 */
#define DFICTRL_ADDR_DFI_RAS_N_A16_SHIFT                   (27U)
#define DFICTRL_ADDR_DFI_RAS_N_A16_MASK                    (0x1U << DFICTRL_ADDR_DFI_RAS_N_A16_SHIFT)                   /* 0x08000000 */
#define DFICTRL_ADDR_DFI_LP_WAKEUP_SHIFT                   (28U)
#define DFICTRL_ADDR_DFI_LP_WAKEUP_MASK                    (0xFU << DFICTRL_ADDR_DFI_LP_WAKEUP_SHIFT)                   /* 0xF0000000 */
/* CMD */
#define DFICTRL_CMD_DFI_CS_N_SHIFT                         (0U)
#define DFICTRL_CMD_DFI_CS_N_MASK                          (0xFU << DFICTRL_CMD_DFI_CS_N_SHIFT)                         /* 0x0000000F */
#define DFICTRL_CMD_DFI_CAS_N_SHIFT                        (4U)
#define DFICTRL_CMD_DFI_CAS_N_MASK                         (0x3U << DFICTRL_CMD_DFI_CAS_N_SHIFT)                        /* 0x00000030 */
#define DFICTRL_CMD_DFI_RAS_N_SHIFT                        (6U)
#define DFICTRL_CMD_DFI_RAS_N_MASK                         (0x3U << DFICTRL_CMD_DFI_RAS_N_SHIFT)                        /* 0x000000C0 */
#define DFICTRL_CMD_DFI_WE_N_SHIFT                         (8U)
#define DFICTRL_CMD_DFI_WE_N_MASK                          (0x3U << DFICTRL_CMD_DFI_WE_N_SHIFT)                         /* 0x00000300 */
#define DFICTRL_CMD_DFI_ACT_N_SHIFT                        (10U)
#define DFICTRL_CMD_DFI_ACT_N_MASK                         (0x3U << DFICTRL_CMD_DFI_ACT_N_SHIFT)                        /* 0x00000C00 */
#define DFICTRL_CMD_DFI_CKE_SHIFT                          (16U)
#define DFICTRL_CMD_DFI_CKE_MASK                           (0x1U << DFICTRL_CMD_DFI_CKE_SHIFT)                          /* 0x00010000 */
#define DFICTRL_CMD_DFI_RESET_N_SHIFT                      (17U)
#define DFICTRL_CMD_DFI_RESET_N_MASK                       (0x1U << DFICTRL_CMD_DFI_RESET_N_SHIFT)                      /* 0x00020000 */
#define DFICTRL_CMD_DFI_DRAM_CLK_DIS_SHIFT                 (18U)
#define DFICTRL_CMD_DFI_DRAM_CLK_DIS_MASK                  (0x1U << DFICTRL_CMD_DFI_DRAM_CLK_DIS_SHIFT)                 /* 0x00040000 */
#define DFICTRL_CMD_DFI_LP_CK_DIS_SHIFT                    (19U)
#define DFICTRL_CMD_DFI_LP_CK_DIS_MASK                     (0x1U << DFICTRL_CMD_DFI_LP_CK_DIS_SHIFT)                    /* 0x00080000 */
#define DFICTRL_CMD_DFI_INIT_START_SHIFT                   (20U)
#define DFICTRL_CMD_DFI_INIT_START_MASK                    (0x1U << DFICTRL_CMD_DFI_INIT_START_SHIFT)                   /* 0x00100000 */
#define DFICTRL_CMD_DFI_LP_REQ_SHIFT                       (21U)
#define DFICTRL_CMD_DFI_LP_REQ_MASK                        (0x1U << DFICTRL_CMD_DFI_LP_REQ_SHIFT)                       /* 0x00200000 */
#define DFICTRL_CMD_WR_EN_SHIFT                            (30U)
#define DFICTRL_CMD_WR_EN_MASK                             (0x1U << DFICTRL_CMD_WR_EN_SHIFT)                            /* 0x40000000 */
#define DFICTRL_CMD_RD_EN_SHIFT                            (31U)
#define DFICTRL_CMD_RD_EN_MASK                             (0x1U << DFICTRL_CMD_RD_EN_SHIFT)                            /* 0x80000000 */
/* STAT */
#define DFICTRL_STAT_DFI_INIT_COMPLETE_SHIFT               (0U)
#define DFICTRL_STAT_DFI_INIT_COMPLETE_MASK                (0x1U << DFICTRL_STAT_DFI_INIT_COMPLETE_SHIFT)               /* 0x00000001 */
#define DFICTRL_STAT_DFI_INIT_START_SHIFT                  (1U)
#define DFICTRL_STAT_DFI_INIT_START_MASK                   (0x1U << DFICTRL_STAT_DFI_INIT_START_SHIFT)                  /* 0x00000002 */
#define DFICTRL_STAT_DFI_LP_ACK_SHIFT                      (2U)
#define DFICTRL_STAT_DFI_LP_ACK_MASK                       (0x1U << DFICTRL_STAT_DFI_LP_ACK_SHIFT)                      /* 0x00000004 */
#define DFICTRL_STAT_DFI_LP_REQ_SHIFT                      (3U)
#define DFICTRL_STAT_DFI_LP_REQ_MASK                       (0x1U << DFICTRL_STAT_DFI_LP_REQ_SHIFT)                      /* 0x00000008 */
#define DFICTRL_STAT_DFI_SEL_SHIFT                         (4U)
#define DFICTRL_STAT_DFI_SEL_MASK                          (0x1U << DFICTRL_STAT_DFI_SEL_SHIFT)                         /* 0x00000010 */
/* DAT0 */
#define DFICTRL_DAT0_DAT0_SHIFT                            (0U)
#define DFICTRL_DAT0_DAT0_MASK                             (0xFFFFFFFFU << DFICTRL_DAT0_DAT0_SHIFT)                     /* 0xFFFFFFFF */
/* DAT1 */
#define DFICTRL_DAT1_DAT1_SHIFT                            (0U)
#define DFICTRL_DAT1_DAT1_MASK                             (0xFFFFFFFFU << DFICTRL_DAT1_DAT1_SHIFT)                     /* 0xFFFFFFFF */
/* DAT2 */
#define DFICTRL_DAT2_DAT2_SHIFT                            (0U)
#define DFICTRL_DAT2_DAT2_MASK                             (0xFFFFFFFFU << DFICTRL_DAT2_DAT2_SHIFT)                     /* 0xFFFFFFFF */
/* DAT3 */
#define DFICTRL_DAT3_DAT3_SHIFT                            (0U)
#define DFICTRL_DAT3_DAT3_MASK                             (0xFFFFFFFFU << DFICTRL_DAT3_DAT3_SHIFT)                     /* 0xFFFFFFFF */
/* DAT4 */
#define DFICTRL_DAT4_DAT4_SHIFT                            (0U)
#define DFICTRL_DAT4_DAT4_MASK                             (0xFFFFFFFFU << DFICTRL_DAT4_DAT4_SHIFT)                     /* 0xFFFFFFFF */
/* DAT5 */
#define DFICTRL_DAT5_DAT5_SHIFT                            (0U)
#define DFICTRL_DAT5_DAT5_MASK                             (0xFFFFFFFFU << DFICTRL_DAT5_DAT5_SHIFT)                     /* 0xFFFFFFFF */
/* DAT6 */
#define DFICTRL_DAT6_DAT6_SHIFT                            (0U)
#define DFICTRL_DAT6_DAT6_MASK                             (0xFFFFFFFFU << DFICTRL_DAT6_DAT6_SHIFT)                     /* 0xFFFFFFFF */
/* DAT7 */
#define DFICTRL_DAT7_DAT7_SHIFT                            (0U)
#define DFICTRL_DAT7_DAT7_MASK                             (0xFFFFFFFFU << DFICTRL_DAT7_DAT7_SHIFT)                     /* 0xFFFFFFFF */
/* TIM0 */
#define DFICTRL_TIM0_TPHY_WRLAT_SHIFT                      (0U)
#define DFICTRL_TIM0_TPHY_WRLAT_MASK                       (0xFU << DFICTRL_TIM0_TPHY_WRLAT_SHIFT)                      /* 0x0000000F */
#define DFICTRL_TIM0_TRDDATA_EN_SHIFT                      (4U)
#define DFICTRL_TIM0_TRDDATA_EN_MASK                       (0xFU << DFICTRL_TIM0_TRDDATA_EN_SHIFT)                      /* 0x000000F0 */
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
#define RGA_DST_INFO_SW_SRC1_CSC_MODE_SHIFT                (19U)
#define RGA_DST_INFO_SW_SRC1_CSC_MODE_MASK                 (0x3U << RGA_DST_INFO_SW_SRC1_CSC_MODE_SHIFT)                /* 0x00180000 */
#define RGA_DST_INFO_SW_SRC1_CSC_CLIP_SHIFT                (21U)
#define RGA_DST_INFO_SW_SRC1_CSC_CLIP_MASK                 (0x1U << RGA_DST_INFO_SW_SRC1_CSC_CLIP_SHIFT)                /* 0x00200000 */
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
/* RISR */
#define SFC_RISR_RXFS_SHIFT                                (0U)
#define SFC_RISR_RXFS_MASK                                 (0x1U << SFC_RISR_RXFS_SHIFT)                                /* 0x00000001 */
#define SFC_RISR_RXUS_SHIFT                                (1U)
#define SFC_RISR_RXUS_MASK                                 (0x1U << SFC_RISR_RXUS_SHIFT)                                /* 0x00000002 */
#define SFC_RISR_TXOS_SHIFT                                (2U)
#define SFC_RISR_TXOS_MASK                                 (0x1U << SFC_RISR_TXOS_SHIFT)                                /* 0x00000004 */
#define SFC_RISR_TXES_SHIFT                                (3U)
#define SFC_RISR_TXES_MASK                                 (0x1U << SFC_RISR_TXES_SHIFT)                                /* 0x00000008 */
#define SFC_RISR_TRANSS_SHIFT                              (4U)
#define SFC_RISR_TRANSS_MASK                               (0x1U << SFC_RISR_TRANSS_SHIFT)                              /* 0x00000010 */
#define SFC_RISR_AHBS_SHIFT                                (5U)
#define SFC_RISR_AHBS_MASK                                 (0x1U << SFC_RISR_AHBS_SHIFT)                                /* 0x00000020 */
#define SFC_RISR_NSPIS_SHIFT                               (6U)
#define SFC_RISR_NSPIS_MASK                                (0x1U << SFC_RISR_NSPIS_SHIFT)                               /* 0x00000040 */
#define SFC_RISR_DMAS_SHIFT                                (7U)
#define SFC_RISR_DMAS_MASK                                 (0x1U << SFC_RISR_DMAS_SHIFT)                                /* 0x00000080 */
#define SFC_RISR_STPOLLS_SHIFT                             (8U)
#define SFC_RISR_STPOLLS_MASK                              (0x1U << SFC_RISR_STPOLLS_SHIFT)                             /* 0x00000100 */
/* VER */
#define SFC_VER_VER_SHIFT                                  (0U)
#define SFC_VER_VER_MASK                                   (0xFFFFU << SFC_VER_VER_SHIFT)                               /* 0x0000FFFF */
/* DMATR */
#define SFC_DMATR_DMATR_SHIFT                              (0U)
#define SFC_DMATR_DMATR_MASK                               (0x1U << SFC_DMATR_DMATR_SHIFT)                              /* 0x00000001 */
/* DMAADDR */
#define SFC_DMAADDR_DMAADDR_SHIFT                          (0U)
#define SFC_DMAADDR_DMAADDR_MASK                           (0xFFFFFFFFU << SFC_DMAADDR_DMAADDR_SHIFT)                   /* 0xFFFFFFFF */
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

/*****************************************CACHE*****************************************/
/* CACHE LINE SIZE */
#define CACHE_LINE_SHIFT                                   (6U)
#define CACHE_LINE_SIZE                                    (0x1U << CACHE_LINE_SHIFT)

#ifndef __ASSEMBLY__
typedef enum PD_Id {
    PD_INVALID = 0U,
    VD_NPU     = 0x800222FFU,
    PD_PCIE    = 0x80044499U,
    PD_VPU     = 0x800777DDU,
    PD_VIO     = 0x800888EEU,
} ePD_Id;
#endif

/********Name=SOFTRST_CON0,Offset=0x300********/
#define SRST_COREPO0_SRSTN                 0U
#define SRST_COREPO1_SRSTN                 1U
#define SRST_CORE0_SRSTN                   2U
#define SRST_CORE1_SRSTN                   3U
#define SRST_CORE0_DBG_SRSTN               4U
#define SRST_CORE1_DBG_SRSTN               5U
#define SRST_TOPDBG_SRSTN                  6U
#define SRST_CORE_NOC_SRSTN                7U
#define SRST_STRC_SYS_ASRSTN               8U
#define SRST_L2_SRSTN                      9U
#define SRST_DAP_SRSTN                     10U
#define SRST_CORE_MSNIU_SRSTN              11U
#define SRST_PDCORE_ADB400_GIC2CORE_ASRSTN 12U
#define SRST_PDCORE_ADB400_CORE2GIC_ASRSTN 13U
#define SRST_CORE_PRF_ASRSTN               14U
#define SRST_CORE_GRF_PSRSTN               15U
/********Name=SOFTRST_CON1,Offset=0x304********/
#define SRST_DDRPHY_SRSTN      16U
#define SRST_DDRPHY_PSRSTN     18U
#define SRST_UPCTL2_SRSTN      20U
#define SRST_UPCTL2_ASRSTN     21U
#define SRST_UPCTL2_PRSTN      22U
#define SRST_MSCH_SRSTN        23U
#define SRST_MSCH_PSRSTN       24U
#define SRST_DDRMON_PSRSTN     25U
#define SRST_DDRSTDBY_PSRSTN   26U
#define SRST_DDRSTDBY_SRSTN    27U
#define SRST_DDRGRF_PSRSTN     28U
#define SRST_AXI_SPLIT_ASRSTN  29U
#define SRST_DDRDFI_CTL_SRSTN  30U
#define SRST_DDRDFI_CTL_PSRSTN 31U
/********Name=SOFTRST_CON2,Offset=0x308********/
#define SRST_GIC500_NIU_ASRSTN            32U
#define SRST_GIC500_ASRSTN                33U
#define SRST_PDGIC_ADB400_CORE2GIC_ASRSTN 34U
#define SRST_PDGIC_ADB400_GIC2CORE_ASRSTN 35U
#define SRST_NPU_CORE_SRSTN               36U
#define SRST_NPU_ASRSTN                   37U
#define SRST_NPU_HSRSTN                   38U
#define SRST_NPU_NIU_ASRSTN               39U
#define SRST_NPU_NIU_HSRSTN               40U
#define SRST_NPU2MEM_ASRSTN               41U
#define SRST_NPU_PVTM_SRSTN               42U
#define SRST_CORE_PVTM_SRSTN              43U
#define SRST_PDGIC_SPINLOCK_ASRSTN        47U
/********Name=SOFTRST_CON3,Offset=0x30C********/
#define SRST_PCIE_NIU_HSSRSTN      48U
#define SRST_PCIE_NIU_LSSRSTN      49U
#define SRST_PCIEGRF_PSRSTN        50U
#define SRST_PCIECTL_PSRSTN        51U
#define SRST_PCIECTL_POWERUP_SRSTN 52U
#define SRST_PCIECTL_MST_ASRSTN    53U
#define SRST_PCIECTL_SLV_ASRSTN    54U
#define SRST_PCIECTL_DBI_ASRSTN    55U
#define SRST_PCIECTL_BUTTON_SRSTN  56U
#define SRST_PCIECTL_PE_SRSTN      57U
#define SRST_PCIECTL_CORE_SRSTN    58U
#define SRST_PCIECTL_NSTICKY_SRSTN 59U
#define SRST_PCIECTL_STICKY_SRSTN  60U
#define SRST_PCIECTL_PWR_SRSTN     61U
#define SRST_PCIE_NIU_ASRSTN       62U
#define SRST_PCIE_NIU_PSRSTN       63U
/********Name=SOFTRST_CON4,Offset=0x310********/
#define SRST_PCIEPHY_POR_SRSTN      64U
#define SRST_PCIEPHY_PSRSTN         65U
#define SRST_PCIEPHY_PIPE_SRSTN     66U
#define SRST_USBPHY_POR_SRSTN       67U
#define SRST_USBPHY_OTG_PORT_SRSTN  68U
#define SRST_USBPHY_HOST_PORT_SRSTN 69U
#define SRST_USB3PHY_GRF_PSRSTN     70U
#define SRST_USB2PHY_GRF_PSRSTN     71U
#define SRST_USB3_OTG_ASRSTN        72U
#define SRST_USB2HOST_HSRSTN        73U
#define SRST_USB2HOST_ARB_HSRSTN    74U
#define SRST_USB2HOST_UTMI_SRSTN    75U
/********Name=SOFTRST_CON5,Offset=0x314********/
#define SRST_IMEM0_ASRSTN     80U
#define SRST_IMEM1_ASRSTN     81U
#define SRST_IMEM2_ASRSTN     82U
#define SRST_IMEM3_ASRSTN     83U
#define SRST_IMEM0_NIU_ASRSTN 84U
#define SRST_IMEM1_NIU_ASRSTN 85U
#define SRST_IMEM2_NIU_ASRSTN 86U
#define SRST_IMEM3_NIU_ASRSTN 87U
#define SRST_IMEM_NIU_HSSRSTN 88U
#define SRST_VPU_NIU_ASRSTN   92U
#define SRST_VPU_NIU_HSRSTN   93U
#define SRST_VPU_ASRSTN       94U
#define SRST_VPU_HSRSTN       95U
/********Name=SOFTRST_CON6,Offset=0x318********/
#define SRST_VIO_NIU_HSSRSTN      96U
#define SRST_VIO_NIU_LSSRSTN      97U
#define SRST_VOPB_ASRSTN          98U
#define SRST_VOPB_HSRSTN          99U
#define SRST_VOPB_DRSTN           100U
#define SRST_VOPL_ASRSTN          101U
#define SRST_VOPL_HSRSTN          102U
#define SRST_VOPL_DRSTN           103U
#define SRST_MIPIDSI_HOST_PSRSTN  104U
#define SRST_CSITX_PSRSTN         105U
#define SRST_CSITX_TXBYTEHS_SRSTN 106U
#define SRST_CSITX_TXESC_SRSTN    107U
#define SRST_CSITX_CAM_SRSTN      108U
#define SRST_CSITX_ISRSTN         109U
/********Name=SOFTRST_CON7,Offset=0x31C********/
#define SRST_RGA_ASRSTN        112U
#define SRST_RGA_HSRSTN        113U
#define SRST_RGA_SRSTN         114U
#define SRST_CSIRX_PSRSTN      115U
#define SRST_CIF_ASRSTN        116U
#define SRST_CIF_HSRSTN        117U
#define SRST_CIF_ISRSTN        118U
#define SRST_CIF_PCLKIN_SRSTN  119U
#define SRST_CIF_DSRSTN        120U
#define SRST_ISP_HSRSTN        121U
#define SRST_ISP_SRSTN         122U
#define SRST_MIPICSIPHY_PSRSTN 124U
#define SRST_MIPIDSIPHY_PSRSTN 125U
/********Name=SOFTRST_CON8,Offset=0x320********/
#define SRST_PDPERI_NIU_MSSRSTN    128U
#define SRST_PDPERI_NIU_LSSRSTN    129U
#define SRST_PD_MMC_SFC_NIU_HSRSTN 132U
#define SRST_SDMMC_HSRSTN          133U
#define SRST_SDIO_HSRSTN           134U
#define SRST_EMMC_HSRSTN           135U
#define SRST_SFC_HSRSTN            136U
#define SRST_SFC_SRSTN             137U
#define SRST_PD_SD_GMAC_NIU_ASRSTN 140U
#define SRST_PD_SD_GMAC_NIU_HSRSTN 141U
#define SRST_PD_SD_GMAC_NIU_PSRSTN 142U
#define SRST_GMAC_ASRSTN           143U
/********Name=SOFTRST_CON9,Offset=0x324********/
#define SRST_PMU_NIU_PSRSTN          144U
#define SRST_PMU_SGRF_PSRSTN         145U
#define SRST_PMU_GRF_PSRSTN          146U
#define SRST_PMU_PMU_SRSTN           147U
#define SRST_PMU_MEM_PSRSTN          148U
#define SRST_PMU_GPIO0_PSRSTN        149U
#define SRST_PMU_UART0_PSRSTN        150U
#define SRST_PMU_CRU_PSRSTN          151U
#define SRST_PMU_PVTM_SRSTN          152U
#define SRST_PMU_UART0_SRSTN         153U
#define SRST_PMU_NIU_HRSTN           154U
#define SRST_PMU_DDR_FAIL_SAVE_SRSTN 155U
#define SRST_PMU_I2C0_PSRSTN         156U
#define SRST_PMU_I2C0_SRSTN          157U
#define SRST_PMU_GPIO0_DBSRSTN       158U
/********Name=SOFTRST_CON10,Offset=0x328********/
#define SRST_PD_AUDIO_NIU_HSRSTN 160U
#define SRST_VAD_HSRSTN          161U
#define SRST_PDM_HSRSTN          162U
#define SRST_PDM_SRSTN           163U
#define SRST_I2S0_HSRSTN         164U
#define SRST_I2S0_TX_SRSTN       165U
#define SRST_I2S1_HSRSTN         166U
#define SRST_I2S1_SRSTN          167U
#define SRST_I2S0_RX_SRSTN       168U
/********Name=SOFTRST_CON11,Offset=0x32C********/
#define SRST_BUS_NIU_MSSRSTN   176U
#define SRST_BUS_NIU_LSSRSTN   177U
#define SRST_TOP_NIU_PSRSTN    178U
#define SRST_ROM_HSRSTN        179U
#define SRST_CRYPTO_ASRSTN     180U
#define SRST_CRYPTO_HSRSTN     181U
#define SRST_CRYPTO_CORE_SRSTN 182U
#define SRST_CRYPTO_APK_SRSTN  183U
#define SRST_DCF_ASRSTN        184U
#define SRST_DCF_PSRSTN        185U
#define SRST_UART1_PSRSTN      186U
#define SRST_UART1_SRSTN       187U
#define SRST_UART2_PSRSTN      188U
#define SRST_UART2_SRSTN       189U
#define SRST_UART3_PSRSTN      190U
#define SRST_UART3_SRSTN       191U
/********Name=SOFTRST_CON12,Offset=0x330********/
#define SRST_UART4_PSRSTN 192U
#define SRST_UART4_SRSTN  193U
#define SRST_UART5_PSRSTN 194U
#define SRST_UART5_SRSTN  195U
#define SRST_UART6_PSRSTN 196U
#define SRST_UART6_SRSTN  197U
#define SRST_UART7_PSRSTN 198U
#define SRST_UART7_SRSTN  199U
#define SRST_I2C1_PSRSTN  200U
#define SRST_I2C1_SRSTN   201U
#define SRST_I2C2_PSRSTN  202U
#define SRST_I2C2_SRSTN   203U
#define SRST_I2C3_PSRSTN  204U
#define SRST_I2C3_SRSTN   205U
#define SRST_PWM0_PSRSTN  206U
#define SRST_PWM0_SRSTN   207U
/********Name=SOFTRST_CON13,Offset=0x334********/
#define SRST_PWM1_PSRSTN     208U
#define SRST_PWM1_SRSTN      209U
#define SRST_PWM2_PSRSTN     210U
#define SRST_PWM2_SRSTN      211U
#define SRST_SPI0_PSRSTN     212U
#define SRST_SPI0_SRSTN      213U
#define SRST_SPI1_PSRSTN     214U
#define SRST_SPI1_SRSTN      215U
#define SRST_SPI2_PSRSTN     216U
#define SRST_SPI2_SRSTN      217U
#define SRST_BUS_SGRF_PSRSTN 218U
#define SRST_BUS_GRF_PSRSTN  219U
#define SRST_TIMER_PSRSTN    220U
#define SRST_TIMER0_SRSTN    221U
#define SRST_TIMER1_SRSTN    222U
#define SRST_TIMER2_SRSTN    223U
/********Name=SOFTRST_CON14,Offset=0x338********/
#define SRST_TIMER3_SRSTN        224U
#define SRST_TIMER4_SRSTN        225U
#define SRST_TIMER5_SRSTN        226U
#define SRST_WDT_NS_PSRSTN       227U
#define SRST_EFUSE_NS_PSRSTN     228U
#define SRST_EFUSE_NS_SRSTN      229U
#define SRST_GPIO1_PSRSTN        230U
#define SRST_GPIO1_DBSRSTN       231U
#define SRST_GPIO2_PSRSTN        232U
#define SRST_GPIO2_DBSRSTN       233U
#define SRST_GPIO3_PSRSTN        234U
#define SRST_GPIO3_DBSRSTN       235U
#define SRST_GPIO4_PSRSTN        236U
#define SRST_GPIO4_DBSRSTN       237U
#define SRST_BUS_SUB_NIU_MSSRSTN 238U
/********Name=SOFTRST_CON15,Offset=0x33C********/
#define SRST_I2C4_PSRSTN   240U
#define SRST_I2C4_SRSTN    241U
#define SRST_I2C5_PSRSTN   242U
#define SRST_I2C5_SRSTN    243U
#define SRST_SARADC_SRSTN  252U
#define SRST_SARADC_PSRSTN 253U
#define SRST_TSADC_PSRSTN  254U
#define SRST_TSADC_SRSTN   255U

/********Name=CLKGATE_CON0,Offset=0x230********/
#define CORE_PLL_GATE             0U
#define MSCLK_CORE_NIU_GATE       1U
#define ACLK_CORE_GATE            2U
#define PCLK_CORE_GATE            3U
#define CLK_JTAG_GATE             4U
#define CLK_PVTM_CORE_GATE        5U
#define ACLK_CORE_NIU_GATE        6U
#define PCLK_DBG_NIU_GATE         7U
#define PCLK_DBG_DAPLITE_GATE     8U
#define ACLK_ADB400_GIC2CORE_GATE 9U
#define ACLK_ADB400_CORE2GIC_GATE 10U
#define ACLK_CORE_PRF_GATE        11U
#define PCLK_CORE_GRF_GATE        12U
#define CLK_PVTM_NPU_GATE         15U
/********Name=CLKGATE_CON1,Offset=0x234********/
#define GIC_PLL_GATE       16U
#define ACLK_GIC_NIU_GATE  17U
#define ACLK_GIC500_GATE   18U
#define ACLK_SPINLOCK_GATE 21U
#define NPU_PLL_GATE       22U
#define NPU_NP5_PLL_GATE   23U
#define NPU_PLL_ACLK_GATE  24U
#define NPU_PLL_HCLK_GATE  25U
#define CLK_NPU_CORE_GATE  26U
#define ACLK_NPU_GATE      27U
#define HCLK_NPU_GATE      28U
#define ACLK_NPU_NIU_GATE  29U
#define HCLK_NPU_NIU_GATE  30U
#define ACLK_NPU2MEM_GATE  31U
/********Name=CLKGATE_CON2,Offset=0x238********/
#define CLK_DDRMON24_GATE    32U
#define PCLK_DDR_PLL_GATE    33U
#define PCLK_DDRDFI_CTL_GATE 34U
#define CLK_DDRDFI_CTL_GATE  35U
#define ACLK_DDR_UPCTL_GATE  37U
#define CLK_DDR_UPCTL_GATE   38U
#define PCLK_DDR_UPCTL_GATE  39U
#define CLK_DDR_MSCH_GATE    40U
#define PCLK_DDR_MSCH_GATE   41U
#define PCLK_DDR_MON_GATE    42U
#define CLK_DDRC_MON_GATE    43U
#define PCLK_DDRSTDBY_GATE   44U
#define CLK_DDRSTDBY_GATE    45U
#define PCLK_DDRGRF_GATE     46U
#define ACLK_AXI_SPLIT_GATE  47U
/********Name=CLKGATE_CON3,Offset=0x23C********/
#define HSCLK_VIO_PLL_GATE       48U
#define DCLKVOPRAW_PLL_GATE      49U
#define DCLKVOPRAW_FRACDIV_GATE  50U
#define DCLKVOPRAW_GATE          51U
#define DCLKVOPLITE_PLL_GATE     52U
#define DCLKVOPLITE_FRACDIV_GATE 53U
#define DCLKVOPLITE_GATE         54U
#define CLK_TXESC_PLL_GATE       55U
#define CLK_RGA_PLL_GATE         56U
#define CLK_CIFOUT_PLL_GATE      57U
#define CLK_ISP_PLL_GATE         58U
#define DCLK_CIF_PLL_GATE        59U
#define LSCLK_VIO_SRC_GATE       60U
#define PCLKIN_CIF_GATE          61U
/********Name=CLKGATE_CON4,Offset=0x240********/
#define HSCLK_VIO_NIU_GATE 64U
#define LSCLK_VIO_NIU_GATE 65U
#define ACLK_VOPRAW_GATE   66U
#define HCLK_VOPRAW_GATE   67U
#define ACLK_VOPLITE_GATE  68U
#define HCLK_VOPLITE_GATE  69U
#define PCLK_MIPI_DSI_GATE 70U
#define PCLK_CSITX_GATE    71U
#define ACLK_RGA_GATE      72U
#define HCLK_RGA_GATE      73U
#define ACLK_CIF_GATE      74U
#define HCLK_CIF_GATE      75U
#define PCLK_CSI2X_GATE    76U
#define ACLK_ISP_GATE      77U
#define HCLK_ISP_GATE      78U
/********Name=CLKGATE_CON5,Offset=0x244********/
#define PDCLK_PCIE_PLL_GATE      80U
#define CLK_USB3OTG_REF_GATE     81U
#define CLK_USB3OTG_SUSPEND_GATE 82U
#define CLK_PCIE_AUX_PLL_GATE    83U
#define CLK_PCIE_AUX_SRC_GATE    84U
#define ACLK_PCIE_PLL_GATE       85U
#define PCLK_PCIE_SRC_GATE       86U
/********Name=CLKGATE_CON6,Offset=0x248********/
#define HSCLK_PCIE_NIU_GATE    96U
#define LSCLK_PCIE_NIU_GATE    97U
#define ACLK_PCIE_MST_GATE     98U
#define ACLK_PCIE_SLV_GATE     99U
#define ACLK_PCIE_DBI_GATE     100U
#define PCLK_PCIE_GRF_GATE     101U
#define ACLK_USB3OTG_GATE      102U
#define HCLK_USB2HOST_GATE     103U
#define HCLK_USB2HOST_ARB_GATE 104U
#define PCLK_PCIE_GATE         105U
#define ACLK_PCIE_NIU_GATE     106U
#define PCLK_PCIE_NIU_GATE     107U
/********Name=CLKGATE_CON7,Offset=0x24C********/
#define IMEM_PLL_GATE       112U
#define IMEM0_SRC_GATE      113U
#define IMEM1_SRC_GATE      114U
#define IMEM2_SRC_GATE      115U
#define IMEM3_SRC_GATE      116U
#define IMEM_HSCLK_PLL_GATE 117U
#define ACLK_IMEM0_GATE     118U
#define ACLK_IMEM1_GATE     119U
#define ACLK_IMEM2_GATE     120U
#define ACLK_IMEM3_GATE     121U
#define ACLK_IMEM0_NIU_GATE 122U
#define ACLK_IMEM1_NIU_GATE 123U
#define ACLK_IMEM2_NIU_GATE 124U
#define ACLK_IMEM3_NIU_GATE 125U
/********Name=CLKGATE_CON8,Offset=0x250********/
#define PD_PERI_PLL_GATE    128U
#define MSCLK_PERI_GATE     129U
#define LSCLK_PERI_GATE     130U
#define MSCLK_PERI_NIU_GATE 131U
#define LSCLK_PERI_NIU_GATE 132U
#define DDRPHY_DPLL_GATE    133U
#define DDRPHY_GPLL_GATE    134U
#define DDRPHY1X_GATE       135U
#define ACLK_VPU_PLL_GATE   136U
#define HCLK_VPU_SRC_GATE   137U
#define HCLK_VPU_NIU_GATE   139U
#define ACLK_VPU_GATE       140U
#define HCLK_VPU_GATE       141U
/********Name=CLKGATE_CON9,Offset=0x254********/
#define HCLK_MMC_SFC_GATE     144U
#define CLK_SDIO_PLL_GATE     145U
#define CLK_SDIO_DIV50_GATE   146U
#define CLK_SDIO_GATE         147U
#define CLK_EMMC_PLL_GATE     148U
#define CLK_EMMC_DIV50_GATE   149U
#define CLK_EMMC_GATE         150U
#define CLK_SDMMC_PLL_GATE    151U
#define CLK_SDMMC_DIV50_GATE  152U
#define CLK_SDMMC_GATE        153U
#define CLK_SFC_PLL_GATE      154U
#define HCLK_MMC_SFC_NIU_GATE 155U
#define HCLK_EMMC_GATE        156U
#define HCLK_SFC_GATE         157U
/********Name=CLKGATE_CON10,Offset=0x258********/
#define ACLK_SD_GMAC_GATE     160U
#define HCLK_SD_GMAC_GATE     161U
#define PCLK_SD_GMAC_GATE     162U
#define CLK_GMAC_PLL_GATE     163U
#define CLK_MAC_REF_GATE      164U
#define CLK_MAC_REFOUT_GATE   165U
#define CLK_MAC_RX_GATE       166U
#define CLK_MAC_TX_GATE       167U
#define ACLK_GMAC_NIU_GATE    168U
#define HCLK_GMAC_NIU_GATE    169U
#define PCLK_GMAC_NIU_GATE    170U
#define ACLK_GMAC_GATE        171U
#define PCLK_GMAC_GATE        172U
#define HCLK_SDIO_GATE        173U
#define HCLK_SDMMC_GATE       174U
#define CLK_GMAC_OUT_PLL_GATE 175U
/********Name=CLKGATE_CON11,Offset=0x25C********/
#define PD_BUS_PLL_GATE         176U
#define HSCLK_BUS_GATE          177U
#define MSCLK_BUS_GATE          178U
#define LSCLK_BUS_GATE          179U
#define PCLK_TOP_GATE           180U
#define CLK_CRYPTO_PLL_GATE     181U
#define CLK_CRYPTO_APK_PLL_GATE 182U
#define CLK_UART1_PLL_GATE      184U
#define CLK_UART1_DIVNP5_GATE   185U
#define CLK_UART1_FRAC_SRC_GATE 186U
#define CLK_UART1_GATE          187U
#define CLK_UART2_PLL_GATE      188U
#define CLK_UART2_DIVNP5_GATE   189U
#define CLK_UART2_FRAC_SRC_GATE 190U
#define CLK_UART2_GATE          191U
/********Name=CLKGATE_CON12,Offset=0x260********/
#define CLK_UART3_PLL_GATE      192U
#define CLK_UART3_DIVNP5_GATE   193U
#define CLK_UART3_FRAC_SRC_GATE 194U
#define CLK_UART3_GATE          195U
#define CLK_UART4_PLL_GATE      196U
#define CLK_UART4_DIVNP5_GATE   197U
#define CLK_UART4_FRAC_SRC_GATE 198U
#define CLK_UART4_GATE          199U
#define CLK_UART5_PLL_GATE      200U
#define CLK_UART5_DIVNP5_GATE   201U
#define CLK_UART5_FRAC_SRC_GATE 202U
#define CLK_UART5_GATE          203U
#define CLK_UART6_PLL_GATE      204U
#define CLK_UART6_DIVNP5_GATE   205U
#define CLK_UART6_FRAC_SRC_GATE 206U
#define CLK_UART6_GATE          207U
/********Name=CLKGATE_CON13,Offset=0x264********/
#define CLK_UART7_PLL_GATE      208U
#define CLK_UART7_DIVNP5_GATE   209U
#define CLK_UART7_FRAC_SRC_GATE 210U
#define CLK_UART7_GATE          211U
#define CLK_I2C1_PLL_GATE       212U
#define CLK_I2C2_PLL_GATE       213U
#define CLK_I2C3_PLL_GATE       214U
#define CLK_SPI0_PLL_GATE       215U
#define CLK_SPI1_PLL_GATE       216U
#define CLK_SPI2_PLL_GATE       217U
#define CLK_PWM0_PLL_GATE       218U
#define CLK_PWM1_PLL_GATE       219U
#define CLK_PWM2_PLL_GATE       220U
#define CLK_TSADC_PLL_GATE      221U
#define CLK_SARADC_PLL_GATE     222U
/********Name=CLKGATE_CON14,Offset=0x268********/
#define CLK_EFUSE_S_PLL_GATE  224U
#define CLK_EFUSE_NS_PLL_GATE 225U
#define DBCLK_GPIO1_PLL_GATE  226U
#define DBCLK_GPIO2_PLL_GATE  227U
#define DBCLK_GPIO3_PLL_GATE  228U
#define DBCLK_GPIO4_PLL_GATE  229U
#define CLK_I2C4_PLL_GATE     230U
#define CLK_I2C5_PLL_GATE     231U
#define CLK_TIMER0_GATE       232U
#define CLK_TIMER1_GATE       233U
#define CLK_TIMER2_GATE       234U
#define CLK_TIMER3_GATE       235U
#define CLK_TIMER4_GATE       236U
#define CLK_TIMER5_GATE       237U
/********Name=CLKGATE_CON15,Offset=0x26C********/
#define HSCLK_BUS_NIU_GATE     240U
#define MSCLK_BUS_NIU_GATE     241U
#define MSCLK_BUS_SUB_NIU_GATE 242U
#define LSCLK_BUS_NIU_GATE     243U
#define HCLK_ROM_GATE          244U
#define ACLK_CRYPTO_GATE       245U
#define HCLK_CRYPTO_GATE       246U
#define ACLK_DCF_GATE          247U
#define PCLK_DCF_GATE          248U
#define PCLK_UART1_GATE        249U
#define PCLK_UART2_GATE        250U
#define PCLK_UART3_GATE        251U
#define PCLK_UART4_GATE        252U
#define PCLK_UART5_GATE        253U
#define PCLK_UART6_GATE        254U
#define PCLK_UART7_GATE        255U
/********Name=CLKGATE_CON16,Offset=0x270********/
#define PCLK_I2C1_GATE     256U
#define PCLK_I2C2_GATE     257U
#define PCLK_I2C3_GATE     258U
#define PCLK_SPI0_GATE     259U
#define PCLK_SPI1_GATE     260U
#define PCLK_SPI2_GATE     261U
#define PCLK_PWM0_GATE     262U
#define PCLK_PWM1_GATE     263U
#define PCLK_PWM2_GATE     264U
#define PCLK_TSADC_GATE    265U
#define PCLK_SARADC_GATE   266U
#define PCLK_EFUSE_NS_GATE 267U
#define PCLK_GPIO1_GATE    268U
#define PCLK_GPIO2_GATE    269U
#define PCLK_GPIO3_GATE    270U
#define PCLK_GPIO4_GATE    271U
/********Name=CLKGATE_CON17,Offset=0x274********/
#define PCLK_TIMER0_GATE          272U
#define PCLK_WDT_NS_GATE          273U
#define PCLK_BUS_GRF_GATE         274U
#define PCLK_BUS_SGRF_GATE        275U
#define PCLK_I2C4_GATE            276U
#define PCLK_I2C5_GATE            277U
#define HCLK_PD_AUDIO_GATE        280U
#define CLK_PDM_PLL_GATE          281U
#define CLK_PDM_FRAC_SRC_GATE     282U
#define CLK_PDM_GATE              283U
#define CLK_I2S0_TX_PLL_GATE      284U
#define CLK_I2S0_TX_FRAC_SRC_GATE 285U
#define CLK_I2S0_TX_GATE          286U
#define CLK_I2S0_TX_OUT_MCLK_GATE 287U
/********Name=CLKGATE_CON18,Offset=0x278********/
#define CLK_I2S0_RX_PLL_GATE      288U
#define CLK_I2S0_RX_FRAC_SRC_GATE 289U
#define CLK_I2S0_RX_GATE          290U
#define CLK_I2S0_RX_OUT_MCLK_GATE 291U
#define CLK_I2S1_PLL_GATE         292U
#define CLK_I2S1_FRAC_SRC_GATE    293U
#define CLK_I2S1_GATE             294U
#define CLK_I2S1_OUT_MCLK_GATE    295U
#define HCLK_AUDIO_NIU_GATE       299U
#define HCLK_VAD_GATE             300U
#define HCLK_PDM_GATE             301U
#define HCLK_I2S0_GATE            302U
#define HCLK_I2S1_GATE            303U
/********Name=CLKGATE_CON19,Offset=0x27C********/
#define PCLK_TOP_NIU_GATE      304U
#define PCLK_TOP_CRU_GATE      305U
#define PCLK_DDRPHY_GATE       306U
#define PCLK_MIPIDSIPHY_GATE   307U
#define PCLK_MIPICSIPHY_GATE   308U
#define PCLK_USB3PHY_PIPE_GATE 310U
#define PCLK_USB3_GRF_GATE     311U
#define PCLK_USB_GRF_GATE      312U
#define TESTCLK_GATE           319U
/********Name=PMU_CLKGATE_CON0,Offset=0x4080********/
#define PCLK_PDPMU_PLL_GATE        320U
#define PCLK_PMU_NIU_GATE          321U
#define PCLK_PMU_SGRF_GATE         322U
#define PCLK_PMU_GRF_GATE          323U
#define PCLK_PMU_PMU_GATE          324U
#define PCLK_PMU_MEM_GATE          325U
#define PCLK_PMU_GPIO0_GATE        326U
#define PCLK_PMU_UART0_GATE        327U
#define PCLK_PMU_CRU_GATE          328U
#define PCLK_PMU_I2C0_GATE         329U
#define XIN_OSC0_FUNC_DIV_SRC_GATE 332U
#define CLK_DIV32P768KHZ_SRC_GATE  333U
#define CLK_WIFI_PLL_GATE          334U
#define CLK_WIFI_GATE              335U
/********Name=PMU_CLKGATE_CON1,Offset=0x4084********/
#define CLK_UART0_PMU_PLL_GATE       336U
#define CLK_UART0_PMU_DIVNP5_GATE    337U
#define CLK_UART0_PMU_FRAC_GATE      338U
#define CLK_UART0_PMU_GATE           339U
#define CLK_PVTM_PMU_GATE            340U
#define CLK_I2C0_PMU_PLL_GATE        341U
#define DBCLK_GPIO0_PMU_PLL_GATE     342U
#define CLK_REF24M_PLL_GATE          344U
#define USBPHY_REF_GATE              345U
#define MIPIDSIPHY_REF_CCLK_GATE     346U
#define CLK_PCIEPHY_REF_DIV_CON_GATE 347U
#define CLK_PCIEPHY_REF_GATE         348U

/********Name=CLKSEL_CON0_S,Offset=0xF8********/
/********Name=CLKSEL_CON0,Offset=0x100********/
#define CLK_CORE_DIV  0x04000000U
#define CORE_DBG_DIV  0x04080000U
#define ACLK_CORE_DIV 0x030C0000U
/********Name=CLKSEL_CON1,Offset=0x104********/
#define CLK_NPU_DIV     0x04000001U
#define CLK_NPU_NP5_DIV 0x04040001U
/********Name=CLKSEL_CON2,Offset=0x108********/
#define ACLK_NPU_DIV     0x04000002U
#define ACLK_NPU2MEM_DIV 0x04040002U
#define HCLK_NPU_DIV     0x04080002U
/********Name=CLKSEL_CON3,Offset=0x10C********/
#define DDRPHY1X_DIV 0x05000003U
#define PCLK_DDR_DIV 0x05080003U
/********Name=CLKSEL_CON4,Offset=0x110********/
#define HSCLK_VIO_DIV 0x05000004U
#define LSCLK_VIO_DIV 0x04080004U
/********Name=CLKSEL_CON5,Offset=0x114********/
#define DCLK_VOPRAW_DIV 0x08000005U
/********Name=CLKSEL_CON6,Offset=0x118********/
#define DCLK_VOPRAW_FRAC_DIV 0x20000006U
/********Name=CLKSEL_CON7,Offset=0x11C********/
#define DCLK_VOPLITE_DIV 0x08000007U
/********Name=CLKSEL_CON8,Offset=0x120********/
#define DCLK_VOPLITE_FRAC_DIV 0x20000008U
/********Name=CLKSEL_CON9,Offset=0x124********/
#define CLK_TXESC_DIV 0x0C000009U
/********Name=CLKSEL_CON10,Offset=0x128********/
#define CLK_RGA_CORE_DIV 0x0500000AU
#define CLK_ISP_DIV      0x0508000AU
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define CLK_CIF_OUT_DIV 0x0600000BU
#define DCLK_CIF_DIV    0x0508000BU
/********Name=CLKSEL_CON12,Offset=0x130********/
#define HSCLK_PCIE_DIV 0x0500000CU
#define LSCLK_PCIE_DIV 0x0508000CU
/********Name=CLKSEL_CON13,Offset=0x134********/
#define CLK_USB3_OTG_SUSPEND_DIV 0x0A00000DU
/********Name=CLKSEL_CON14,Offset=0x138********/
#define CLK_PCIE_AUX_DIV 0x0700000EU
/********Name=CLKSEL_CON15,Offset=0x13C********/
#define ACLK_PCIE_DIV 0x0400000FU
#define PCLK_PCIE_DIV 0x0404000FU
#define ACLK_GIC_DIV  0x040C000FU
/********Name=CLKSEL_CON16,Offset=0x140********/
#define ACLK_VPU_DIV 0x05000010U
#define PCLK_VPU_DIV 0x04080010U
/********Name=CLKSEL_CON17,Offset=0x144********/
#define ACLK_IMEM_DIV  0x05000011U
#define HSCLK_IMEM_DIV 0x05080011U
/********Name=CLKSEL_CON18,Offset=0x148********/
#define MSCLK_CORE_NIU_DIV 0x05000012U
#define CLK_GMAC_OUT_DIV   0x05080012U
/********Name=CLKSEL_CON19,Offset=0x14C********/
#define MSCLK_PERI_DIV 0x05000013U
#define LSCLK_PERI_DIV 0x05080013U
/********Name=CLKSEL_CON20,Offset=0x150********/
#define CLK_SDMMC_DIV 0x08000014U
/********Name=CLKSEL_CON21,Offset=0x154********/
#define CLK_SDMMC_DT50_DIV 0x08000015U
/********Name=CLKSEL_CON22,Offset=0x158********/
#define CLK_SDIO_DIV 0x08000016U
/********Name=CLKSEL_CON23,Offset=0x15C********/
#define CLK_SDIO_DT50_DIV 0x08000017U
/********Name=CLKSEL_CON24,Offset=0x160********/
#define CLK_EMMC_DIV 0x08000018U
/********Name=CLKSEL_CON25,Offset=0x164********/
#define CLK_EMMC_DT50_DIV 0x08000019U
/********Name=CLKSEL_CON26,Offset=0x168********/
#define CLK_SFC_DIV  0x0700001AU
#define CLK_GMAC_DIV 0x0508001AU
/********Name=CLKSEL_CON27,Offset=0x16C********/
#define HSCLK_BUS_DIV 0x0508001BU
/********Name=CLKSEL_CON28,Offset=0x170********/
#define MSCLK_BUS_DIV 0x0500001CU
#define LSCLK_BUS_DIV 0x0508001CU
/********Name=CLKSEL_CON29,Offset=0x174********/
#define CLK_CRYPTO_DIV     0x0500001DU
#define CLK_CRYPTO_APK_DIV 0x0508001DU
/********Name=CLKSEL_CON30,Offset=0x178********/
#define CLK_PDM_DIV 0x0700001EU
/********Name=CLKSEL_CON31,Offset=0x17C********/
#define CLK_PDM_FRAC_DIV 0x2000001FU
/********Name=CLKSEL_CON32,Offset=0x180********/
#define CLK_I2S0_TX_DIV 0x07000020U
/********Name=CLKSEL_CON33,Offset=0x184********/
#define CLK_I2S0_TX_FRAC_DIV 0x20000021U
/********Name=CLKSEL_CON34,Offset=0x188********/
#define CLK_I2S0_RX_DIV 0x07000022U
/********Name=CLKSEL_CON35,Offset=0x18C********/
#define CLK_I2S0_RX_FRAC_DIV 0x20000023U
/********Name=CLKSEL_CON36,Offset=0x190********/
#define CLK_I2S1_DIV 0x07000024U
/********Name=CLKSEL_CON37,Offset=0x194********/
#define CLK_I2S1_FRAC_DIV 0x20000025U
/********Name=CLKSEL_CON38,Offset=0x198********/
#define CLK_UART1_DIV 0x07000026U
/********Name=CLKSEL_CON39,Offset=0x19C********/
#define CLK_UART1_NP5_DIV 0x07000027U
/********Name=CLKSEL_CON40,Offset=0x1A0********/
#define CLK_UART1_FRAC_DIV 0x20000028U
/********Name=CLKSEL_CON41,Offset=0x1A4********/
#define CLK_UART2_DIV 0x07000029U
/********Name=CLKSEL_CON42,Offset=0x1A8********/
#define CLK_UART2_NP5_DIV 0x0700002AU
/********Name=CLKSEL_CON43,Offset=0x1AC********/
#define CLK_UART2_FRAC_DIV 0x2000002BU
/********Name=CLKSEL_CON44,Offset=0x1B0********/
#define CLK_UART3_DIV 0x0700002CU
/********Name=CLKSEL_CON45,Offset=0x1B4********/
#define CLK_UART3_NP5_DIV 0x0700002DU
/********Name=CLKSEL_CON46,Offset=0x1B8********/
#define CLK_UART3_FRAC_DIV 0x2000002EU
/********Name=CLKSEL_CON47,Offset=0x1BC********/
#define CLK_UART4_DIV 0x0700002FU
/********Name=CLKSEL_CON48,Offset=0x1C0********/
#define CLK_UART4_NP5_DIV 0x07000030U
/********Name=CLKSEL_CON49,Offset=0x1C4********/
#define CLK_UART4_FRAC_DIV 0x20000031U
/********Name=CLKSEL_CON50,Offset=0x1C8********/
#define CLK_UART5_DIV 0x07000032U
/********Name=CLKSEL_CON51,Offset=0x1CC********/
#define CLK_UART5_NP5_DIV 0x07000033U
/********Name=CLKSEL_CON52,Offset=0x1D0********/
#define CLK_UART5_FRAC_DIV 0x20000034U
/********Name=CLKSEL_CON53,Offset=0x1D4********/
#define CLK_UART6_DIV 0x07000035U
/********Name=CLKSEL_CON54,Offset=0x1D8********/
#define CLK_UART6_NP5_DIV 0x07000036U
/********Name=CLKSEL_CON55,Offset=0x1DC********/
#define CLK_UART6_FRAC_DIV 0x20000037U
/********Name=CLKSEL_CON56,Offset=0x1E0********/
#define CLK_UART7_DIV 0x07000038U
/********Name=CLKSEL_CON57,Offset=0x1E4********/
#define CLK_UART7_NP5_DIV 0x07000039U
/********Name=CLKSEL_CON58,Offset=0x1E8********/
#define CLK_UART7_FRAC_DIV 0x2000003AU
/********Name=CLKSEL_CON59,Offset=0x1EC********/
#define CLK_I2C1_DIV 0x0700003BU
#define CLK_I2C2_DIV 0x0708003BU
/********Name=CLKSEL_CON60,Offset=0x1F0********/
#define CLK_I2C3_DIV 0x0700003CU
#define CLK_SPI0_DIV 0x0708003CU
/********Name=CLKSEL_CON61,Offset=0x1F4********/
#define CLK_SPI1_DIV 0x0700003DU
#define CLK_SPI2_DIV 0x0708003DU
/********Name=CLKSEL_CON62,Offset=0x1F8********/
#define CLK_TSADC_DIV 0x0B00003EU
/********Name=CLKSEL_CON63,Offset=0x1FC********/
#define CLK_SARADC_DIV 0x0B00003FU
/********Name=CLKSEL_CON64,Offset=0x200********/
#define CLK_EFUSE_S_DIV  0x06000040U
#define CLK_EFUSE_NS_DIV 0x06080040U
/********Name=CLKSEL_CON65,Offset=0x204********/
#define DBCLK_GPIO1_DIV 0x0B000041U
/********Name=CLKSEL_CON66,Offset=0x208********/
#define DBCLK_GPIO2_DIV 0x0B000042U
/********Name=CLKSEL_CON67,Offset=0x20C********/
#define DBCLK_GPIO3_DIV 0x0B000043U
/********Name=CLKSEL_CON68,Offset=0x210********/
#define DBCLK_GPIO4_DIV 0x0B000044U
/********Name=CLKSEL_CON69,Offset=0x214********/
#define CLK_PWM0_DIV 0x07000045U
#define CLK_PWM1_DIV 0x07080045U
/********Name=CLKSEL_CON70,Offset=0x218********/
#define CLK_PWM2_DIV 0x07000046U
/********Name=CLKSEL_CON71,Offset=0x21C********/
#define CLK_I2C4_DIV 0x07000047U
#define CLK_I2C5_DIV 0x07080047U
/********Name=CLKSEL_CON72,Offset=0x220********/
#define TEST_DIV 0x05080048U
/********Name=PMU_CLKSEL_CON0,Offset=0x4040********/
#define PCLK_PDPMU_DIV    0x05000000U
#define XIN_OSC0_FUNC_DIV 0x05080000U
/********Name=PMU_CLKSEL_CON1,Offset=0x4044********/
#define CLK_RTC32K_FRAC_DIV 0x20000001U
/********Name=PMU_CLKSEL_CON2,Offset=0x4048********/
#define CLK_REF24M_DIV 0x06000002U
#define CLK_WIFI_DIV   0x06080002U
/********Name=PMU_CLKSEL_CON3,Offset=0x404C********/
#define CLK_UART0_DIV 0x07000003U
/********Name=PMU_CLKSEL_CON4,Offset=0x4050********/
#define CLK_UART0_NP5_DIV 0x07000004U
/********Name=PMU_CLKSEL_CON5,Offset=0x4054********/
#define CLK_UART0_FRAC_DIV 0x20000005U
/********Name=PMU_CLKSEL_CON6,Offset=0x4058********/
#define DBCLK_GPIO0_DIV 0x0B000006U
/********Name=PMU_CLKSEL_CON7,Offset=0x405C********/
#define CLK_PCIEPHY_REF_DIV 0x02000007U
#define CLK_I2C0_DIV        0x07080007U

/********Name=CLKSEL_CON0_S,Offset=0xF8********/
/********Name=CLKSEL_CON0,Offset=0x100********/
#define CORE_CLK_PLL_SEL      0x02060000U
#define CORE_CLK_PLL_SEL_APLL 0U
#define CORE_CLK_PLL_SEL_CPLL 1U
#define CORE_CLK_PLL_SEL_GPLL 2U
/********Name=CLKSEL_CON1,Offset=0x104********/
#define NPU_PLL_CLK_SEL             0x02080001U
#define NPU_PLL_CLK_SEL_GPLL        0U
#define NPU_PLL_CLK_SEL_CPLL        1U
#define NPU_PLL_CLK_SEL_APLL        2U
#define NPU_NP5_PLL_CLK_SEL         0x020A0001U
#define NPU_NP5_PLL_CLK_SEL_GPLL    0U
#define NPU_NP5_PLL_CLK_SEL_CPLL    1U
#define NPU_NP5_PLL_CLK_SEL_APLL    2U
#define NPU_SRC_CLK_SEL             0x010F0001U
#define NPU_SRC_CLK_SEL_CLK_NPU_DIV 0U
#define NPU_SRC_CLK_SEL_CLK_NPU_NP5 1U
/********Name=CLKSEL_CON2,Offset=0x108********/
#define NPU_ACLK_PLL_SEL      0x010E0002U
#define NPU_ACLK_PLL_SEL_GPLL 0U
#define NPU_ACLK_PLL_SEL_CPLL 1U
#define NPU_HCLK_PLL_SEL      0x010F0002U
#define NPU_HCLK_PLL_SEL_GPLL 0U
#define NPU_HCLK_PLL_SEL_CPLL 1U
/********Name=CLKSEL_CON3,Offset=0x10C********/
#define DDRPHY1X_PLL_CLK_SEL      0x01070003U
#define DDRPHY1X_PLL_CLK_SEL_DPLL 0U
#define DDRPHY1X_PLL_CLK_SEL_GPLL 1U
/********Name=CLKSEL_CON4,Offset=0x110********/
#define HSCLK_VIO_PLL_SEL      0x01070004U
#define HSCLK_VIO_PLL_SEL_GPLL 0U
#define HSCLK_VIO_PLL_SEL_CPLL 1U
/********Name=CLKSEL_CON5,Offset=0x114********/
#define DCLK_VOPRAW_PLL_SEL                  0x020A0005U
#define DCLK_VOPRAW_PLL_SEL_CPLL             0U
#define DCLK_VOPRAW_PLL_SEL_GPLL             1U
#define DCLK_VOPRAW_PLL_SEL_NPLL             2U
#define DCLK_VOPRAW_SEL                      0x020E0005U
#define DCLK_VOPRAW_SEL_DCLK_VOPRAW          0U
#define DCLK_VOPRAW_SEL_DCLK_VOPRAW_FRAC_OUT 1U
#define DCLK_VOPRAW_SEL_XIN_OSC0             2U
/********Name=CLKSEL_CON6,Offset=0x118********/
/********Name=CLKSEL_CON7,Offset=0x11C********/
#define DCLK_VOPLITE_PLL_SEL                   0x020A0007U
#define DCLK_VOPLITE_PLL_SEL_CPLL              0U
#define DCLK_VOPLITE_PLL_SEL_GPLL              1U
#define DCLK_VOPLITE_PLL_SEL_NPLL              2U
#define DCLK_VOPLITE_SEL                       0x020E0007U
#define DCLK_VOPLITE_SEL_DCLK_VOPLITE          0U
#define DCLK_VOPLITE_SEL_DCLK_VOPLITE_FRAC_OUT 1U
#define DCLK_VOPLITE_SEL_XIN_OSC0              2U
/********Name=CLKSEL_CON8,Offset=0x120********/
/********Name=CLKSEL_CON9,Offset=0x124********/
/********Name=CLKSEL_CON10,Offset=0x128********/
#define CLK_RGA_CORE_PLL_SEL      0x0206000AU
#define CLK_RGA_CORE_PLL_SEL_GPLL 0U
#define CLK_RGA_CORE_PLL_SEL_CPLL 1U
#define CLK_RGA_CORE_PLL_SEL_NPLL 2U
#define CLK_ISP_PLL_SEL           0x020E000AU
#define CLK_ISP_PLL_SEL_GPLL      0U
#define CLK_ISP_PLL_SEL_CPLL      1U
#define CLK_ISP_PLL_SEL_NPLL      2U
/********Name=CLKSEL_CON11,Offset=0x12C********/
#define CLK_CIF_OUT_PLL_SEL            0x0206000BU
#define CLK_CIF_OUT_PLL_SEL_XIN_OSC0   0U
#define CLK_CIF_OUT_PLL_SEL_NPLL       1U
#define CLK_CIF_OUT_PLL_SEL_GPLL       2U
#define CLK_CIF_OUT_PLL_SEL_USBPHY480M 3U
#define DCLK_CIF_PLL_SEL               0x020E000BU
#define DCLK_CIF_PLL_SEL_CPLL          0U
#define DCLK_CIF_PLL_SEL_GPLL          1U
#define DCLK_CIF_PLL_SEL_NPLL          2U
/********Name=CLKSEL_CON12,Offset=0x130********/
#define PDCLK_PCIE_PLL_SEL      0x010F000CU
#define PDCLK_PCIE_PLL_SEL_GPLL 0U
#define PDCLK_PCIE_PLL_SEL_CPLL 1U
/********Name=CLKSEL_CON13,Offset=0x134********/
#define USB3_OTG_SUSPEND_SRC_SEL          0x010C000DU
#define USB3_OTG_SUSPEND_SRC_SEL_RTC_32K  0U
#define USB3_OTG_SUSPEND_SRC_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON14,Offset=0x138********/
#define CLK_PCIE_AUX_PLL_SEL                  0x0208000EU
#define CLK_PCIE_AUX_PLL_SEL_CPLL             0U
#define CLK_PCIE_AUX_PLL_SEL_GPLL             1U
#define CLK_PCIE_AUX_PLL_SEL_NPLL             2U
#define CLK_PCIE_AUX_SRC_SEL                  0x010C000EU
#define CLK_PCIE_AUX_SRC_SEL_XIN_OSC0         0U
#define CLK_PCIE_AUX_SRC_SEL_CLK_PCIE_AUX_SRC 1U
/********Name=CLKSEL_CON15,Offset=0x13C********/
#define ACLK_PCIE_PLL_SEL      0x0108000FU
#define ACLK_PCIE_PLL_SEL_GPLL 0U
#define ACLK_PCIE_PLL_SEL_CPLL 1U
#define GIC_PLL_CLK_SEL        0x010B000FU
#define GIC_PLL_CLK_SEL_GPLL   0U
#define GIC_PLL_CLK_SEL_CPLL   1U
/********Name=CLKSEL_CON16,Offset=0x140********/
#define ACLK_VPU_PLL_CLK_SEL      0x01070010U
#define ACLK_VPU_PLL_CLK_SEL_GPLL 0U
#define ACLK_VPU_PLL_CLK_SEL_CPLL 1U
/********Name=CLKSEL_CON17,Offset=0x144********/
#define ACLK_IMEM_PLL_CLK_SEL       0x01070011U
#define ACLK_IMEM_PLL_CLK_SEL_GPLL  0U
#define ACLK_IMEM_PLL_CLK_SEL_CPLL  1U
#define HSCLK_IMEM_PLL_CLK_SEL      0x010F0011U
#define HSCLK_IMEM_PLL_CLK_SEL_GPLL 0U
#define HSCLK_IMEM_PLL_CLK_SEL_CPLL 1U
/********Name=CLKSEL_CON18,Offset=0x148********/
#define CLK_GMAC_OUT_PLL_SEL        0x020E0012U
#define CLK_GMAC_OUT_PLL_SEL_CPLL   0U
#define CLK_GMAC_OUT_PLL_SEL_NPLL   1U
#define CLK_GMAC_OUT_PLL_SEL_PMUPLL 2U
/********Name=CLKSEL_CON19,Offset=0x14C********/
#define PD_PERI_PLL_SEL      0x010F0013U
#define PD_PERI_PLL_SEL_GPLL 0U
#define PD_PERI_PLL_SEL_CPLL 1U
/********Name=CLKSEL_CON20,Offset=0x150********/
#define CLK_SDMMC_PLL_SEL          0x020E0014U
#define CLK_SDMMC_PLL_SEL_GPLL     0U
#define CLK_SDMMC_PLL_SEL_CPLL     1U
#define CLK_SDMMC_PLL_SEL_NPLL     2U
#define CLK_SDMMC_PLL_SEL_XIN_OSC0 3U
/********Name=CLKSEL_CON21,Offset=0x154********/
#define CLK_SDMMC_SEL                 0x010F0015U
#define CLK_SDMMC_SEL_CLK_SDMMC       0U
#define CLK_SDMMC_SEL_CLK_SDMMC_DIV50 1U
/********Name=CLKSEL_CON22,Offset=0x158********/
#define CLK_SDIO_PLL_SEL          0x020E0016U
#define CLK_SDIO_PLL_SEL_GPLL     0U
#define CLK_SDIO_PLL_SEL_CPLL     1U
#define CLK_SDIO_PLL_SEL_NPLL     2U
#define CLK_SDIO_PLL_SEL_XIN_OSC0 3U
/********Name=CLKSEL_CON23,Offset=0x15C********/
#define CLK_SDIO_SEL                0x010F0017U
#define CLK_SDIO_SEL_CLK_SDIO       0U
#define CLK_SDIO_SEL_CLK_SDIO_DIV50 1U
/********Name=CLKSEL_CON24,Offset=0x160********/
#define CLK_EMMC_PLL_SEL          0x020E0018U
#define CLK_EMMC_PLL_SEL_GPLL     0U
#define CLK_EMMC_PLL_SEL_CPLL     1U
#define CLK_EMMC_PLL_SEL_NPLL     2U
#define CLK_EMMC_PLL_SEL_XIN_OSC0 3U
/********Name=CLKSEL_CON25,Offset=0x164********/
#define CLK_EMMC_SEL                0x010F0019U
#define CLK_EMMC_SEL_CLK_EMMC       0U
#define CLK_EMMC_SEL_CLK_EMMC_DIV50 1U
/********Name=CLKSEL_CON26,Offset=0x168********/
#define CLK_SFC_PLL_SEL         0x0107001AU
#define CLK_SFC_PLL_SEL_GPLL    0U
#define CLK_SFC_PLL_SEL_CPLL    1U
#define CLK_GMAC_PLL_SEL        0x020E001AU
#define CLK_GMAC_PLL_SEL_CPLL   0U
#define CLK_GMAC_PLL_SEL_NPLL   1U
#define CLK_GMAC_PLL_SEL_PMUPLL 2U
/********Name=CLKSEL_CON27,Offset=0x16C********/
#define RMII_EXTCLKSRC_SEL          0x0100001BU
#define RMII_INTCLKSRC_SEL_CLK_GMAC 0U
#define RMII_EXTCLKSRC_SEL_CLK_GMAC 1U
#define RMII_CLK_SEL                0x0101001BU
#define RMII_CLK_SEL_2_5M           0U
#define RMII_CLK_SEL_25M            1U
#define GMAC_CLK_SEL                0x0202001BU
#define GMAC_CLK_SEL_25M            0U
#define GMAC_CLK_SEL_2_5M           1U
#define GMAC_CLK_SEL_125M           2U
#define PDBUS_CLK_PLL_SEL           0x010F001BU
#define PDBUS_CLK_PLL_SEL_GPLL      0U
#define PDBUS_CLK_PLL_SEL_CPLL      1U
/********Name=CLKSEL_CON28,Offset=0x170********/
/********Name=CLKSEL_CON29,Offset=0x174********/
#define CLK_CRYPTO_PLL_SEL      0x0107001DU
#define CLK_CRYPTO_PLL_SEL_GPLL 0U
#define CLK_CRYPTO_PLL_SEL_CPLL 1U
#define CLK_CRYPTO_APK_SEL      0x010F001DU
#define CLK_CRYPTO_APK_SEL_GPLL 0U
#define CLK_CRYPTO_APK_SEL_CPLL 1U
/********Name=CLKSEL_CON30,Offset=0x178********/
#define CLK_PDM_PLL_SEL              0x0208001EU
#define CLK_PDM_PLL_SEL_GPLL         0U
#define CLK_PDM_PLL_SEL_XIN_OSC0     1U
#define CLK_PDM_PLL_SEL_CPLL         2U
#define CLK_PDM_PLL_SEL_NPLL         3U
#define CLK_PDM_SEL                  0x010F001EU
#define CLK_PDM_SEL_CLK_PDM          0U
#define CLK_PDM_SEL_CLK_PDM_FRAC_OUT 1U
/********Name=CLKSEL_CON31,Offset=0x17C********/
/********Name=CLKSEL_CON32,Offset=0x180********/
#define CLK_I2S0_TX_PLL_SEL                             0x02080020U
#define CLK_I2S0_TX_PLL_SEL_GPLL                        0U
#define CLK_I2S0_TX_PLL_SEL_CPLL                        1U
#define CLK_I2S0_TX_PLL_SEL_NPLL                        2U
#define CLK_I2S0_TX_SEL                                 0x020A0020U
#define CLK_I2S0_TX_SEL_CLK_I2S0_TX                     0U
#define CLK_I2S0_TX_SEL_CLK_I2S0_TX_FRAC_OUT            1U
#define CLK_I2S0_TX_SEL_MCLK_I2S0_TX_IN                 2U
#define CLK_I2S0_TX_SEL_XIN_OSC0_HALF                   3U
#define CLK_I2S0_TX_RX_CLK_SEL                          0x010C0020U
#define CLK_I2S0_TX_RX_CLK_SEL_CLK_I2S0_TX_CLK          0U
#define CLK_I2S0_TX_RX_CLK_SEL_CLK_I2S0_RX_CLK          1U
#define CLK_I2S0_TX_OUT_MCLK_SEL                        0x020E0020U
#define CLK_I2S0_TX_OUT_MCLK_SEL_CLK_I2S0_TX_RX_CLK_SEL 0U
#define CLK_I2S0_TX_OUT_MCLK_SEL_XIN_OSC0_HALF          1U
#define CLK_I2S0_TX_OUT_MCLK_SEL_CLK_I2S0_RX            2U
/********Name=CLKSEL_CON33,Offset=0x184********/
/********Name=CLKSEL_CON34,Offset=0x188********/
#define CLK_I2S0_RX_PLL_SEL                             0x02080022U
#define CLK_I2S0_RX_PLL_SEL_GPLL                        0U
#define CLK_I2S0_RX_PLL_SEL_CPLL                        1U
#define CLK_I2S0_RX_PLL_SEL_NPLL                        2U
#define CLK_I2S0_RX_SEL                                 0x020A0022U
#define CLK_I2S0_RX_SEL_CLK_I2S0_RX                     0U
#define CLK_I2S0_RX_SEL_CLK_I2S0_RX_FRAC_OUT            1U
#define CLK_I2S0_RX_SEL_MCLK_I2S0_RX_IN                 2U
#define CLK_I2S0_RX_SEL_XIN_OSC0_HALF                   3U
#define CLK_I2S0_RX_TX_CLK_SEL                          0x010C0022U
#define CLK_I2S0_RX_TX_CLK_SEL_CLK_I2S0_RX_CLK          0U
#define CLK_I2S0_RX_TX_CLK_SEL_CLK_I2S0_TX_CLK          1U
#define CLK_I2S0_RX_OUT_MCLK_SEL                        0x020E0022U
#define CLK_I2S0_RX_OUT_MCLK_SEL_CLK_I2S0_RX_TX_CLK_SEL 0U
#define CLK_I2S0_RX_OUT_MCLK_SEL_XIN_OSC0_HALF          1U
#define CLK_I2S0_RX_OUT_MCLK_SEL_CLK_I2S0_TX            2U
/********Name=CLKSEL_CON35,Offset=0x18C********/
/********Name=CLKSEL_CON36,Offset=0x190********/
#define CLK_I2S1_PLL_SEL                    0x02080024U
#define CLK_I2S1_PLL_SEL_GPLL               0U
#define CLK_I2S1_PLL_SEL_CPLL               1U
#define CLK_I2S1_PLL_SEL_NPLL               2U
#define CLK_I2S1_SEL                        0x020A0024U
#define CLK_I2S1_SEL_CLK_I2S1               0U
#define CLK_I2S1_SEL_CLK_I2S1_FRAC_OUT      1U
#define CLK_I2S1_SEL_MCLK_I2S1_IN           2U
#define CLK_I2S1_SEL_XIN_OSC0_HALF          3U
#define CLK_I2S1_OUT_MCLK_SEL               0x010F0024U
#define CLK_I2S1_OUT_MCLK_SEL_CLK_I2S1_SEL  0U
#define CLK_I2S1_OUT_MCLK_SEL_XIN_OSC0_HALF 1U
/********Name=CLKSEL_CON37,Offset=0x194********/
/********Name=CLKSEL_CON38,Offset=0x198********/
#define CLK_UART1_PLL_SEL            0x020E0026U
#define CLK_UART1_PLL_SEL_GPLL       0U
#define CLK_UART1_PLL_SEL_USBPHY480M 1U
#define CLK_UART1_PLL_SEL_CPLL       2U
#define CLK_UART1_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON39,Offset=0x19C********/
#define CLK_UART1_SEL                    0x020E0027U
#define CLK_UART1_SEL_CLK_UART1          0U
#define CLK_UART1_SEL_CLK_UART1_NP5      1U
#define CLK_UART1_SEL_CLK_UART1_FRAC_OUT 2U
#define CLK_UART1_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON40,Offset=0x1A0********/
/********Name=CLKSEL_CON41,Offset=0x1A4********/
#define CLK_UART2_PLL_SEL            0x020E0029U
#define CLK_UART2_PLL_SEL_GPLL       0U
#define CLK_UART2_PLL_SEL_USBPHY480M 1U
#define CLK_UART2_PLL_SEL_CPLL       2U
#define CLK_UART2_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON42,Offset=0x1A8********/
#define CLK_UART2_SEL                    0x020E002AU
#define CLK_UART2_SEL_CLK_UART2          0U
#define CLK_UART2_SEL_CLK_UART2_NP5      1U
#define CLK_UART2_SEL_CLK_UART2_FRAC_OUT 2U
#define CLK_UART2_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON43,Offset=0x1AC********/
/********Name=CLKSEL_CON44,Offset=0x1B0********/
#define CLK_UART3_PLL_SEL            0x020E002CU
#define CLK_UART3_PLL_SEL_GPLL       0U
#define CLK_UART3_PLL_SEL_USBPHY480M 1U
#define CLK_UART3_PLL_SEL_CPLL       2U
#define CLK_UART3_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON45,Offset=0x1B4********/
#define CLK_UART3_SEL                    0x020E002DU
#define CLK_UART3_SEL_CLK_UART3          0U
#define CLK_UART3_SEL_CLK_UART3_NP5      1U
#define CLK_UART3_SEL_CLK_UART3_FRAC_OUT 2U
#define CLK_UART3_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON46,Offset=0x1B8********/
/********Name=CLKSEL_CON47,Offset=0x1BC********/
#define CLK_UART4_PLL_SEL            0x020E002FU
#define CLK_UART4_PLL_SEL_GPLL       0U
#define CLK_UART4_PLL_SEL_USBPHY480M 1U
#define CLK_UART4_PLL_SEL_CPLL       2U
#define CLK_UART4_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON48,Offset=0x1C0********/
#define CLK_UART4_SEL                    0x020E0030U
#define CLK_UART4_SEL_CLK_UART4          0U
#define CLK_UART4_SEL_CLK_UART4_NP5      1U
#define CLK_UART4_SEL_CLK_UART4_FRAC_OUT 2U
#define CLK_UART4_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON49,Offset=0x1C4********/
/********Name=CLKSEL_CON50,Offset=0x1C8********/
#define CLK_UART5_PLL_SEL            0x020E0032U
#define CLK_UART5_PLL_SEL_GPLL       0U
#define CLK_UART5_PLL_SEL_USBPHY480M 1U
#define CLK_UART5_PLL_SEL_CPLL       2U
#define CLK_UART5_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON51,Offset=0x1CC********/
#define CLK_UART5_SEL                    0x020E0033U
#define CLK_UART5_SEL_CLK_UART5          0U
#define CLK_UART5_SEL_CLK_UART5_NP5      1U
#define CLK_UART5_SEL_CLK_UART5_FRAC_OUT 2U
#define CLK_UART5_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON52,Offset=0x1D0********/
/********Name=CLKSEL_CON53,Offset=0x1D4********/
#define CLK_UART6_PLL_SEL            0x020E0035U
#define CLK_UART6_PLL_SEL_GPLL       0U
#define CLK_UART6_PLL_SEL_USBPHY480M 1U
#define CLK_UART6_PLL_SEL_CPLL       2U
#define CLK_UART6_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON54,Offset=0x1D8********/
#define CLK_UART6_SEL                    0x020E0036U
#define CLK_UART6_SEL_CLK_UART6          0U
#define CLK_UART6_SEL_CLK_UART6_NP5      1U
#define CLK_UART6_SEL_CLK_UART6_FRAC_OUT 2U
#define CLK_UART6_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON55,Offset=0x1DC********/
/********Name=CLKSEL_CON56,Offset=0x1E0********/
#define CLK_UART7_PLL_SEL            0x020E0038U
#define CLK_UART7_PLL_SEL_GPLL       0U
#define CLK_UART7_PLL_SEL_USBPHY480M 1U
#define CLK_UART7_PLL_SEL_CPLL       2U
#define CLK_UART7_PLL_SEL_NPLL       3U
/********Name=CLKSEL_CON57,Offset=0x1E4********/
#define CLK_UART7_SEL                    0x020E0039U
#define CLK_UART7_SEL_CLK_UART7          0U
#define CLK_UART7_SEL_CLK_UART7_NP5      1U
#define CLK_UART7_SEL_CLK_UART7_FRAC_OUT 2U
#define CLK_UART7_SEL_XIN_OSC0           3U
/********Name=CLKSEL_CON58,Offset=0x1E8********/
/********Name=CLKSEL_CON59,Offset=0x1EC********/
#define CLK_I2C1_PLL_SEL          0x0107003BU
#define CLK_I2C1_PLL_SEL_GPLL     0U
#define CLK_I2C1_PLL_SEL_XIN_OSC0 1U
#define CLK_I2C2_PLL_SEL          0x010F003BU
#define CLK_I2C2_PLL_SEL_GPLL     0U
#define CLK_I2C2_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON60,Offset=0x1F0********/
#define CLK_I2C3_PLL_SEL          0x0107003CU
#define CLK_I2C3_PLL_SEL_GPLL     0U
#define CLK_I2C3_PLL_SEL_XIN_OSC0 1U
#define CLK_SPI0_PLL_SEL          0x010F003CU
#define CLK_SPI0_PLL_SEL_GPLL     0U
#define CLK_SPI0_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON61,Offset=0x1F4********/
#define CLK_SPI1_PLL_SEL          0x0107003DU
#define CLK_SPI1_PLL_SEL_GPLL     0U
#define CLK_SPI1_PLL_SEL_XIN_OSC0 1U
#define CLK_SPI2_PLL_SEL          0x010F003DU
#define CLK_SPI2_PLL_SEL_GPLL     0U
#define CLK_SPI2_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON62,Offset=0x1F8********/
/********Name=CLKSEL_CON63,Offset=0x1FC********/
/********Name=CLKSEL_CON64,Offset=0x200********/
#define CLK_EFUSE_S_PLL_CLK_SEL           0x02060040U
#define CLK_EFUSE_S_PLL_CLK_SEL_GPLL      0U
#define CLK_EFUSE_S_PLL_CLK_SEL_CPLL      1U
#define CLK_EFUSE_S_PLL_CLK_SEL_XIN_OSC0  2U
#define CLK_EFUSE_NS_PLL_CLK_SEL          0x020E0040U
#define CLK_EFUSE_NS_PLL_CLK_SEL_GPLL     0U
#define CLK_EFUSE_NS_PLL_CLK_SEL_CPLL     1U
#define CLK_EFUSE_NS_PLL_CLK_SEL_XIN_OSC0 2U
/********Name=CLKSEL_CON65,Offset=0x204********/
#define DBCLK_GPIO1_PLL_SEL          0x010F0041U
#define DBCLK_GPIO1_PLL_SEL_XIN_OSC0 0U
#define DBCLK_GPIO1_PLL_SEL_CLK_RTC  1U
/********Name=CLKSEL_CON66,Offset=0x208********/
#define DBCLK_GPIO2_PLL_SEL          0x010F0042U
#define DBCLK_GPIO2_PLL_SEL_XIN_OSC0 0U
#define DBCLK_GPIO2_PLL_SEL_CLK_RTC  1U
/********Name=CLKSEL_CON67,Offset=0x20C********/
#define DBCLK_GPIO3_PLL_SEL          0x010F0043U
#define DBCLK_GPIO3_PLL_SEL_XIN_OSC0 0U
#define DBCLK_GPIO3_PLL_SEL_CLK_RTC  1U
/********Name=CLKSEL_CON68,Offset=0x210********/
#define DBCLK_GPIO4_PLL_SEL          0x010F0044U
#define DBCLK_GPIO4_PLL_SEL_XIN_OSC0 0U
#define DBCLK_GPIO4_PLL_SEL_CLK_RTC  1U
/********Name=CLKSEL_CON69,Offset=0x214********/
#define CLK_PWM0_PLL_SEL          0x01070045U
#define CLK_PWM0_PLL_SEL_GPLL     0U
#define CLK_PWM0_PLL_SEL_XIN_OSC0 1U
#define CLK_PWM1_PLL_SEL          0x010F0045U
#define CLK_PWM1_PLL_SEL_GPLL     0U
#define CLK_PWM1_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON70,Offset=0x218********/
#define CLK_PWM2_PLL_SEL          0x01070046U
#define CLK_PWM2_PLL_SEL_GPLL     0U
#define CLK_PWM2_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON71,Offset=0x21C********/
#define CLK_I2C4_PLL_SEL          0x01070047U
#define CLK_I2C4_PLL_SEL_GPLL     0U
#define CLK_I2C4_PLL_SEL_XIN_OSC0 1U
#define CLK_I2C5_PLL_SEL          0x010F0047U
#define CLK_I2C5_PLL_SEL_GPLL     0U
#define CLK_I2C5_PLL_SEL_XIN_OSC0 1U
/********Name=CLKSEL_CON72,Offset=0x220********/
#define TESTCLK_SEL                 0x05000048U
#define TESTCLK_SEL_CLK_24M         0U
#define TESTCLK_SEL_CLK_RTC         1U
#define TESTCLK_SEL_CLK_CORE        2U
#define TESTCLK_SEL_ACLK_GIC        3U
#define TESTCLK_SEL_CLK_NPU         4U
#define TESTCLK_SEL_ACLK_NPU        5U
#define TESTCLK_SEL_DDRPHY1X        6U
#define TESTCLK_SEL_CLK_ISP         7U
#define TESTCLK_SEL_HSCLK_PCIE      8U
#define TESTCLK_SEL_CLK_PCIE_PIPE   9U
#define TESTCLK_SEL_HSCLK_IMEM_NIU  10U
#define TESTCLK_SEL_ACLK_IMEM       11U
#define TESTCLK_SEL_HSCLK_VIO       12U
#define TESTCLK_SEL_CLK_RGA_CORE    13U
#define TESTCLK_SEL_DCLK_VOPRAW     14U
#define TESTCLK_SEL_DCLK_VOPLITE    15U
#define TESTCLK_SEL_MSCLK_PERI      16U
#define TESTCLK_SEL_CLK_SPI0        17U
#define TESTCLK_SEL_CLK_GMAC        18U
#define TESTCLK_SEL_CLK_SDMMC       19U
#define TESTCLK_SEL_CLK_SDIO        20U
#define TESTCLK_SEL_CLK_EMMC        21U
#define TESTCLK_SEL_HSCLK_BUS       22U
#define TESTCLK_SEL_CLK_CRYPTO_APK  23U
#define TESTCLK_SEL_CLK_UART1       24U
#define TESTCLK_SEL_DCLK_CIF        25U
#define TESTCLK_SEL_HSCLK_VPU       26U
#define TESTCLK_SEL_CLK_CRYPTO_CORE 27U
#define TESTCLK_SEL_CLK_I2S0_RX     28U
#define TESTCLK_SEL_CLK_I2S1        29U
#define TESTCLK_SEL_CLK_PDM         30U
#define TESTCLK_SEL_CLK_I2S0_TX     31U
/********Name=PMU_CLKSEL_CON0,Offset=0x4040********/
#define CLK_RTC32K_CLK_SEL                  0x020E0000U
#define CLK_RTC32K_CLK_SEL_IO_CLK_RTC_32K   0U
#define CLK_RTC32K_CLK_SEL_PVTM_CLK_RTC_32K 1U
#define CLK_RTC32K_CLK_SEL_FRAC_CLK_RTC_32K 2U
/********Name=PMU_CLKSEL_CON1,Offset=0x4044********/
/********Name=PMU_CLKSEL_CON2,Offset=0x4048********/
#define USBPHY_REF_SEL             0x01060002U
#define USBPHY_REF_SEL_XIN_OSC     0U
#define USBPHY_REF_SEL_CLOCK       1U
#define MIPIDSIPHY_REF_SEL         0x01070002U
#define MIPIDSIPHY_REF_SEL_XIN_OSC 0U
#define MIPIDSIPHY_REF_SEL_CLOCK   1U
#define CLK_WIFI_SEL               0x010F0002U
#define CLK_WIFI_SEL_XIN_OSC       0U
#define CLK_WIFI_SEL_CLK_WIFI_OUT  1U
/********Name=PMU_CLKSEL_CON3,Offset=0x404C********/
#define CLK_UART0_PLL_SEL            0x020E0003U
#define CLK_UART0_PLL_SEL_GPLL       0U
#define CLK_UART0_PLL_SEL_USBPHY480M 1U
#define CLK_UART0_PLL_SEL_CPLL       2U
#define CLK_UART0_PLL_SEL_PMUPLL     3U
/********Name=PMU_CLKSEL_CON4,Offset=0x4050********/
#define CLK_UART0_SEL                    0x020E0004U
#define CLK_UART0_SEL_CLK_UART0          0U
#define CLK_UART0_SEL_CLK_UART0_NP5      1U
#define CLK_UART0_SEL_CLK_UART0_FRAC_OUT 2U
#define CLK_UART0_SEL_XIN_OSC0           3U
/********Name=PMU_CLKSEL_CON5,Offset=0x4054********/
/********Name=PMU_CLKSEL_CON6,Offset=0x4058********/
#define DBCLK_GPIO0_PLL_SEL          0x010F0006U
#define DBCLK_GPIO0_PLL_SEL_XIN_OSC0 0U
#define DBCLK_GPIO0_PLL_SEL_CLK_RTC  1U
/********Name=PMU_CLKSEL_CON7,Offset=0x405C********/
#define CLK_PCIEPHY_REF_CLK_SEL                     0x01040007U
#define CLK_PCIEPHY_REF_CLK_SEL_XIN_OSC0            0U
#define CLK_PCIEPHY_REF_CLK_SEL_CLK_PCIEPHY_REF_SRC 1U
#define CLK_I2C0_PLL_SEL                            0x010F0007U
#define CLK_I2C0_PLL_SEL_PMUPLL                     0U
#define CLK_I2C0_PLL_SEL_XIN_OSC0                   1U

#define CLK(mux, div) \
    (((mux) & 0x0F0F00FFU) | (((div) & 0xFFU) << 8) | (((div) & 0x0F0F0000U) << 4))

#ifndef __ASSEMBLY__
typedef enum CLOCK_Name {
    CLK_INVALID = 0U,
    PLL_APLL,
    PLL_DPLL,
    PLL_CPLL,
    PLL_GPLL,
    PLL_NPLL,
    PLL_PPLL,
    CLK_CPU              = CLK(CORE_CLK_PLL_SEL, CLK_CORE_DIV),
    PCLK_DBG             = CLK(0U, CORE_DBG_DIV),
    CLK_NPU_D            = CLK(NPU_PLL_CLK_SEL, CLK_NPU_DIV),
    CLK_NPU_NP5          = CLK(NPU_NP5_PLL_CLK_SEL, CLK_NPU_NP5_DIV),
    CLK_NPU              = CLK(NPU_SRC_CLK_SEL, 0U),
    ACLK_NPU             = CLK(NPU_ACLK_PLL_SEL, ACLK_NPU_DIV),
    ACLK_NPU2MEM         = CLK(0U, ACLK_NPU2MEM_DIV),
    HCLK_NPU             = CLK(NPU_HCLK_PLL_SEL, HCLK_NPU_DIV),
    CLK_DDRPHY1X_        = CLK(DDRPHY1X_PLL_CLK_SEL, DDRPHY1X_DIV),
    PCLK_DDR             = CLK(0U, PCLK_DDR_DIV),
    HSCLK_VIO            = CLK(HSCLK_VIO_PLL_SEL, HSCLK_VIO_DIV),
    LSCLK_VIO            = CLK(0U, LSCLK_VIO_DIV),
    DCLK_VOPRAW_PLL      = CLK(DCLK_VOPRAW_PLL_SEL, DCLK_VOPRAW_DIV),
    DCLK_VOPRAW_FRAC     = CLK(0U, DCLK_VOPRAW_FRAC_DIV),
    DCLK_VOPRAW          = CLK(DCLK_VOPRAW_SEL, 0U),
    DCLK_VOPLITE_PLL     = CLK(DCLK_VOPLITE_PLL_SEL, DCLK_VOPLITE_DIV),
    DCLK_VOPLITE_FRAC    = CLK(0U, DCLK_VOPLITE_FRAC_DIV),
    DCLK_VOPLITE         = CLK(DCLK_VOPLITE_SEL, 0U),
    CLK_TXESC            = CLK(0U, CLK_TXESC_DIV),
    CLK_RGA_CORE         = CLK(CLK_RGA_CORE_PLL_SEL, CLK_RGA_CORE_DIV),
    CLK_ISP              = CLK(CLK_ISP_DIV, CLK_ISP_DIV),
    CLK_CIF_OUT          = CLK(CLK_CIF_OUT_PLL_SEL, CLK_CIF_OUT_DIV),
    DCLK_CIF             = CLK(DCLK_CIF_PLL_SEL, DCLK_CIF_DIV),
    HSCLK_PCIE           = CLK(PDCLK_PCIE_PLL_SEL, HSCLK_PCIE_DIV),
    LSCLK_PCIE           = CLK(PDCLK_PCIE_PLL_SEL, LSCLK_PCIE_DIV),
    CLK_USB3_OTG_SUSPEND = CLK(USB3_OTG_SUSPEND_SRC_SEL, CLK_USB3_OTG_SUSPEND_DIV),
    CLK_PCIE_AUX_PLL     = CLK(CLK_PCIE_AUX_PLL_SEL, CLK_PCIE_AUX_DIV),
    CLK_PCIE_AUX         = CLK(CLK_PCIE_AUX_SRC_SEL, 0U),
    ACLK_PCIE            = CLK(ACLK_PCIE_PLL_SEL, ACLK_PCIE_DIV),
    PCLK_PCIE            = CLK(0U, PCLK_PCIE_DIV),
    ACLK_GIC             = CLK(GIC_PLL_CLK_SEL, ACLK_GIC_DIV),
    ACLK_VPU             = CLK(ACLK_VPU_PLL_CLK_SEL, ACLK_VPU_DIV),
    PCLK_VPU             = CLK(0U, PCLK_VPU_DIV),
    ACLK_IMEM            = CLK(ACLK_IMEM_PLL_CLK_SEL, ACLK_IMEM_DIV),
    HSCLK_IMEM           = CLK(HSCLK_IMEM_PLL_CLK_SEL, HSCLK_IMEM_DIV),
    CLK_GMAC_OUT         = CLK(CLK_GMAC_OUT_PLL_SEL, CLK_GMAC_OUT_DIV),
    MSCLK_PERI           = CLK(PD_PERI_PLL_SEL, MSCLK_PERI_DIV),
    LSCLK_PERI           = CLK(PD_PERI_PLL_SEL, LSCLK_PERI_DIV),
    CLK_SDMMC_PLL        = CLK(CLK_SDMMC_PLL_SEL, CLK_SDMMC_DT50_DIV),
    CLK_SDMMC            = CLK(CLK_SDMMC_SEL, 0U),
    CLK_SDIO_PLL         = CLK(CLK_SDIO_PLL_SEL, CLK_SDIO_DT50_DIV),
    CLK_SDIO             = CLK(CLK_SDIO_SEL, 0U),
    CLK_EMMC_PLL         = CLK(CLK_EMMC_PLL_SEL, CLK_EMMC_DT50_DIV),
    CLK_EMMC             = CLK(CLK_EMMC_SEL, 0U),
    CLK_SFC              = CLK(CLK_SFC_PLL_SEL, CLK_SFC_DIV),
    CLK_GMAC_PLL         = CLK(CLK_GMAC_PLL_SEL, CLK_GMAC_DIV),
    CLK_GMAC             = CLK(RMII_EXTCLKSRC_SEL, 0U),
    CLK_GMAC_RMII_SPEED  = CLK(RMII_CLK_SEL, 0U),
    CLK_GMAC_RGMII_SPEED = CLK(GMAC_CLK_SEL, 0U),
    HSCLK_BUS            = CLK(PDBUS_CLK_PLL_SEL, HSCLK_BUS_DIV),
    MSCLK_BUS            = CLK(PDBUS_CLK_PLL_SEL, MSCLK_BUS_DIV),
    LSCLK_BUS            = CLK(PDBUS_CLK_PLL_SEL, LSCLK_BUS_DIV),
    CLK_CRYPTO           = CLK(CLK_CRYPTO_PLL_SEL, CLK_CRYPTO_DIV),
    CLK_CRYPTO_APK       = CLK(CLK_CRYPTO_APK_SEL, CLK_CRYPTO_APK_DIV),
    CLK_PDM_PLL          = CLK(CLK_PDM_PLL_SEL, CLK_PDM_DIV),
    CLK_PDM_FRAC         = CLK(0U, CLK_PDM_FRAC_DIV),
    CLK_PDM              = CLK(CLK_PDM_SEL, 0U),
    CLK_I2S0_TX_PLL      = CLK(CLK_I2S0_TX_PLL_SEL, CLK_I2S0_TX_DIV),
    CLK_I2S0_TX_FRAC     = CLK(0U, CLK_I2S0_TX_FRAC_DIV),
    CLK_I2S0_TX          = CLK(CLK_I2S0_TX_SEL, 0U),
    CLK_I2S0_RX_PLL      = CLK(CLK_I2S0_RX_PLL_SEL, CLK_I2S0_RX_DIV),
    CLK_I2S0_RX_FRAC     = CLK(0U, CLK_I2S0_RX_FRAC_DIV),
    CLK_I2S0_RX          = CLK(CLK_I2S0_RX_SEL, 0U),
    CLK_I2S1_PLL         = CLK(CLK_I2S1_PLL_SEL, CLK_I2S1_DIV),
    CLK_I2S1_FRAC        = CLK(0U, CLK_I2S1_FRAC_DIV),
    CLK_I2S1             = CLK(CLK_I2S1_SEL, 0U),
    CLK_UART1_PLL        = CLK(CLK_UART1_PLL_SEL, CLK_UART1_DIV),
    CLK_UART1_FRAC       = CLK(0U, CLK_UART1_FRAC_DIV),
    CLK_UART1            = CLK(CLK_UART1_SEL, 0U),
    CLK_UART2_PLL        = CLK(CLK_UART2_PLL_SEL, CLK_UART2_DIV),
    CLK_UART2_FRAC       = CLK(0U, CLK_UART2_FRAC_DIV),
    CLK_UART2            = CLK(CLK_UART2_SEL, 0U),
    CLK_UART3_PLL        = CLK(CLK_UART3_PLL_SEL, CLK_UART3_DIV),
    CLK_UART3_FRAC       = CLK(0U, CLK_UART3_FRAC_DIV),
    CLK_UART3            = CLK(CLK_UART3_SEL, 0U),
    CLK_UART4_PLL        = CLK(CLK_UART4_PLL_SEL, CLK_UART4_DIV),
    CLK_UART4_FRAC       = CLK(0U, CLK_UART4_FRAC_DIV),
    CLK_UART4            = CLK(CLK_UART4_SEL, 0U),
    CLK_UART5_PLL        = CLK(CLK_UART5_PLL_SEL, CLK_UART5_DIV),
    CLK_UART5_FRAC       = CLK(0U, CLK_UART5_FRAC_DIV),
    CLK_UART5            = CLK(CLK_UART5_SEL, 0U),
    CLK_UART6_PLL        = CLK(CLK_UART6_PLL_SEL, CLK_UART6_DIV),
    CLK_UART6_FRAC       = CLK(0U, CLK_UART6_FRAC_DIV),
    CLK_UART6            = CLK(CLK_UART6_SEL, 0U),
    CLK_UART7_PLL        = CLK(CLK_UART7_PLL_SEL, CLK_UART7_DIV),
    CLK_UART7_FRAC       = CLK(0U, CLK_UART7_FRAC_DIV),
    CLK_UART7            = CLK(CLK_UART7_SEL, 0U),
    CLK_I2C1             = CLK(CLK_I2C1_PLL_SEL, CLK_I2C1_DIV),
    CLK_I2C2             = CLK(CLK_I2C2_PLL_SEL, CLK_I2C2_DIV),
    CLK_I2C3             = CLK(CLK_I2C3_PLL_SEL, CLK_I2C3_DIV),
    CLK_I2C4             = CLK(CLK_I2C4_PLL_SEL, CLK_I2C4_DIV),
    CLK_I2C5             = CLK(CLK_I2C5_PLL_SEL, CLK_I2C5_DIV),
    CLK_SPI0             = CLK(CLK_SPI0_PLL_SEL, CLK_SPI0_DIV),
    CLK_SPI1             = CLK(CLK_SPI1_PLL_SEL, CLK_SPI1_DIV),
    CLK_SPI2             = CLK(CLK_SPI2_PLL_SEL, CLK_SPI2_DIV),
    CLK_EFUSE_S          = CLK(CLK_EFUSE_S_PLL_CLK_SEL, CLK_EFUSE_S_DIV),
    CLK_EFUSE_NS         = CLK(CLK_EFUSE_NS_PLL_CLK_SEL, CLK_EFUSE_NS_DIV),
    CLK_TSADC            = CLK(0U, CLK_TSADC_DIV),
    CLK_SARADC           = CLK(0U, CLK_SARADC_DIV),
    DBCLK_GPIO1          = CLK(DBCLK_GPIO1_PLL_SEL, DBCLK_GPIO1_DIV),
    DBCLK_GPIO2          = CLK(DBCLK_GPIO2_PLL_SEL, DBCLK_GPIO2_DIV),
    DBCLK_GPIO3          = CLK(DBCLK_GPIO3_PLL_SEL, DBCLK_GPIO3_DIV),
    DBCLK_GPIO4          = CLK(DBCLK_GPIO4_PLL_SEL, DBCLK_GPIO4_DIV),
    CLK_PWM0             = CLK(CLK_PWM0_PLL_SEL, CLK_PWM0_DIV),
    CLK_PWM1             = CLK(CLK_PWM1_PLL_SEL, CLK_PWM1_DIV),
    CLK_PWM2             = CLK(CLK_PWM2_PLL_SEL, CLK_PWM2_DIV),
    PCLK_PDPMU           = CLK(0U, PCLK_PDPMU_DIV),
    XIN_OSC0_FUNC        = CLK(0U, XIN_OSC0_FUNC_DIV),
    CLK_RTC32K_CLK       = CLK(CLK_RTC32K_CLK_SEL, 0U),
    CLK_RTC32K_FRAC      = CLK(0U, CLK_RTC32K_FRAC_DIV),
    CLK_REF24M           = CLK(0U, CLK_REF24M_DIV),
    CLK_WIFI_PLL         = CLK(0U, CLK_WIFI_DIV),
    CLK_WIFI             = CLK(CLK_WIFI_SEL, 0U),
    CLK_PCIEPHY_REF_CLK  = CLK(CLK_PCIEPHY_REF_CLK_SEL, CLK_PCIEPHY_REF_DIV),
    CLK_I2C0             = CLK(CLK_I2C0_PLL_SEL, CLK_I2C0_DIV),
    DBCLK_GPIO0          = CLK(DBCLK_GPIO0_PLL_SEL, DBCLK_GPIO0_DIV),
    CLK_UART0_PLL        = CLK(CLK_UART0_PLL_SEL, CLK_UART0_DIV),
    CLK_UART0_FRAC       = CLK(0U, CLK_UART0_FRAC_DIV),
    CLK_UART0            = CLK(CLK_UART0_SEL, 0U),
} eCLOCK_Name;
#endif /* __ASSEMBLY__ */

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __RK1808_H */

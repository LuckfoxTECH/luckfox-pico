/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_GRF_RK3576_H
#define _ASM_ARCH_GRF_RK3576_H

#include <common.h>

/* bigcore_grf register structure define */
struct rk3576_bigcore_grf_reg {
     uint32_t reserved0000[11];                   /* address offset: 0x0000 */
     uint32_t cpu_status[1];                      /* address offset: 0x002c */
     uint32_t reserved0030;                       /* address offset: 0x0030 */
     uint32_t cpu_con[2];                         /* address offset: 0x0034 */
     uint32_t cpu_mem_cfg_hdsprf;                 /* address offset: 0x003c */
     uint32_t reserved0040;                       /* address offset: 0x0040 */
     uint32_t cpu_mem_cfg_hssprf;                 /* address offset: 0x0044 */
};

check_member(rk3576_bigcore_grf_reg, cpu_mem_cfg_hssprf, 0x0044);

/* cci_grf register structure define */
struct rk3576_cci_grf_reg {
     uint32_t cci_con[5];                         /* address offset: 0x0000 */
     uint32_t reserved0014[8];                    /* address offset: 0x0014 */
     uint32_t cci_status[5];                      /* address offset: 0x0034 */
     uint32_t reserved0048[3];                    /* address offset: 0x0048 */
     uint32_t cci_mem_cfg_hdsprf;                 /* address offset: 0x0054 */
};

check_member(rk3576_cci_grf_reg, cci_mem_cfg_hdsprf, 0x0054);

/* center_grf register structure define */
struct rk3576_center_grf_reg {
     uint32_t soc_con[6];                         /* address offset: 0x0000 */
};

check_member(rk3576_center_grf_reg, soc_con, 0x0000);

/* combo_pipe_phy_grf register structure define */
struct rk3576_combo_pipe_phy_grf_reg {
     uint32_t pipe_con[5];                        /* address offset: 0x0000 */
     uint32_t reserved0014[8];                    /* address offset: 0x0014 */
     uint32_t pipe_status[1];                     /* address offset: 0x0034 */
     uint32_t reserved0038[18];                   /* address offset: 0x0038 */
     uint32_t lfps_det_con;                       /* address offset: 0x0080 */
     uint32_t reserved0084[7];                    /* address offset: 0x0084 */
     uint32_t phy_int_en;                         /* address offset: 0x00a0 */
     uint32_t phy_int_status;                     /* address offset: 0x00a4 */
};

check_member(rk3576_combo_pipe_phy_grf_reg, phy_int_status, 0x00a4);

/* csidphy_grf register structure define */
struct rk3576_csidphy_grf_reg {
     uint32_t con[1];                             /* address offset: 0x0000 */
     uint32_t reserved0004[31];                   /* address offset: 0x0004 */
     uint32_t status[1];                          /* address offset: 0x0080 */
};

check_member(rk3576_csidphy_grf_reg, status, 0x0080);

/* dcphy_grf register structure define */
struct rk3576_dcphy_grf_reg {
     uint32_t con[2];                             /* address offset: 0x0000 */
     uint32_t reserved0008[30];                   /* address offset: 0x0008 */
     uint32_t status[3];                          /* address offset: 0x0080 */
};

check_member(rk3576_dcphy_grf_reg, status, 0x0080);

/* ddr_grf register structure define */
struct rk3576_ddr_grf_reg {
     uint32_t cha_con[20];                        /* address offset: 0x0000 */
     uint32_t reserved0050[44];                   /* address offset: 0x0050 */
     uint32_t chb_con[20];                        /* address offset: 0x0100 */
     uint32_t reserved0150[44];                   /* address offset: 0x0150 */
     uint32_t cha_status[12];                     /* address offset: 0x0200 */
     uint32_t reserved0230[52];                   /* address offset: 0x0230 */
     uint32_t chb_status[12];                     /* address offset: 0x0300 */
     uint32_t reserved0330[128];                  /* address offset: 0x0330 */
     uint32_t cha_phy_con[1];                     /* address offset: 0x0530 */
     uint32_t chb_phy_con[1];                     /* address offset: 0x0534 */
     uint32_t cha_phy_status[1];                  /* address offset: 0x0538 */
     uint32_t chb_phy_status[1];                  /* address offset: 0x053c */
     uint32_t common_con[6];                      /* address offset: 0x0540 */
     uint32_t reserved0558[10];                   /* address offset: 0x0558 */
     uint32_t status[1];                          /* address offset: 0x0580 */
};

check_member(rk3576_ddr_grf_reg, status, 0x0580);

/* gpu_grf register structure define */
struct rk3576_gpu_grf_reg {
     uint32_t reserved0000[8];                    /* address offset: 0x0000 */
     uint32_t gpu_con[1];                         /* address offset: 0x0020 */
     uint32_t reserved0024[6];                    /* address offset: 0x0024 */
     uint32_t gpu_mem_cfg_hdsprf;                 /* address offset: 0x003c */
     uint32_t gpu_mem_cfg_hsspra;                 /* address offset: 0x0040 */
     uint32_t reserved0044;                       /* address offset: 0x0044 */
     uint32_t gpu_mem_cfg_uhdpdprf_low;           /* address offset: 0x0048 */
};

check_member(rk3576_gpu_grf_reg, gpu_mem_cfg_uhdpdprf_low, 0x0048);

/* usb2phy_grf register structure define */
struct rk3576_usb2phy_grf_reg {
     uint32_t con[6];                             /* address offset: 0x0000 */
     uint32_t reserved0018[2];                    /* address offset: 0x0018 */
     uint32_t ls_con;                             /* address offset: 0x0020 */
     uint32_t dis_con;                            /* address offset: 0x0024 */
     uint32_t bvalid_con;                         /* address offset: 0x0028 */
     uint32_t id_con;                             /* address offset: 0x002c */
     uint32_t vbusvalid_con;                      /* address offset: 0x0030 */
     uint32_t reserved0034[3];                    /* address offset: 0x0034 */
     uint32_t dbg_con[1];                         /* address offset: 0x0040 */
     uint32_t linest_timeout;                     /* address offset: 0x0044 */
     uint32_t linest_deb;                         /* address offset: 0x0048 */
     uint32_t rx_timeout;                         /* address offset: 0x004c */
     uint32_t seq_limt;                           /* address offset: 0x0050 */
     uint32_t linest_cnt_st;                      /* address offset: 0x0054 */
     uint32_t dbg_st;                             /* address offset: 0x0058 */
     uint32_t rx_cnt_st;                          /* address offset: 0x005c */
     uint32_t reserved0060[8];                    /* address offset: 0x0060 */
     uint32_t st[1];                              /* address offset: 0x0080 */
     uint32_t reserved0084[15];                   /* address offset: 0x0084 */
     uint32_t int_en;                             /* address offset: 0x00c0 */
     uint32_t int_st;                             /* address offset: 0x00c4 */
     uint32_t int_st_clr;                         /* address offset: 0x00c8 */
     uint32_t reserved00cc;                       /* address offset: 0x00cc */
     uint32_t detclk_sel;                         /* address offset: 0x00d0 */
};

check_member(rk3576_usb2phy_grf_reg, detclk_sel, 0x00d0);

/* hdptxphy_grf register structure define */
struct rk3576_hdptxphy_grf_reg {
     uint32_t con[2];                             /* address offset: 0x0000 */
     uint32_t reserved0008[30];                   /* address offset: 0x0008 */
     uint32_t status[1];                          /* address offset: 0x0080 */
};

check_member(rk3576_hdptxphy_grf_reg, status, 0x0080);

/* litcore_grf register structure define */
struct rk3576_litcore_grf_reg {
     uint32_t reserved0000[11];                   /* address offset: 0x0000 */
     uint32_t cpu_status[1];                      /* address offset: 0x002c */
     uint32_t reserved0030;                       /* address offset: 0x0030 */
     uint32_t cpu_con[2];                         /* address offset: 0x0034 */
     uint32_t cpu_mem_cfg_hdsprf;                 /* address offset: 0x003c */
     uint32_t reserved0040;                       /* address offset: 0x0040 */
     uint32_t cpu_mem_cfg_hssprf;                 /* address offset: 0x0044 */
};

check_member(rk3576_litcore_grf_reg, cpu_mem_cfg_hssprf, 0x0044);

/* mphy_grf register structure define */
struct rk3576_mphy_grf_reg {
     uint32_t con[2];                             /* address offset: 0x0000 */
     uint32_t status[1];                          /* address offset: 0x0008 */
};

check_member(rk3576_mphy_grf_reg, status, 0x0008);

/* npu_grf register structure define */
struct rk3576_npu_grf_reg {
     uint32_t reserved0000[2];                    /* address offset: 0x0000 */
     uint32_t mem_con[3];                         /* address offset: 0x0008 */
     uint32_t memgate_con[2];                     /* address offset: 0x0014 */
     uint32_t rknnst;                             /* address offset: 0x001c */
     uint32_t nsp_slv_addr;                       /* address offset: 0x0020 */
     uint32_t reserved0024;                       /* address offset: 0x0024 */
     uint32_t nputop_con;                         /* address offset: 0x0028 */
     uint32_t stcalib;                            /* address offset: 0x002c */
     uint32_t start_addr;                         /* address offset: 0x0030 */
     uint32_t end_addr;                           /* address offset: 0x0034 */
     uint32_t nputop_st;                          /* address offset: 0x0038 */
     uint32_t reserved003c[7];                    /* address offset: 0x003c */
     uint32_t cache_maintain;                     /* address offset: 0x0058 */
     uint32_t rv_base_addr;                       /* address offset: 0x005c */
     uint32_t reserved0060[3];                    /* address offset: 0x0060 */
     uint32_t urgent_con[4];                      /* address offset: 0x006c */
};

check_member(rk3576_npu_grf_reg, urgent_con, 0x006c);

/* php_grf register structure define */
struct rk3576_php_grf_reg {
     uint32_t mmubp_st;                           /* address offset: 0x0000 */
     uint32_t mmubp_con[1];                       /* address offset: 0x0004 */
     uint32_t mmu0_con;                           /* address offset: 0x0008 */
     uint32_t mmu1_con;                           /* address offset: 0x000c */
     uint32_t mem_con[3];                         /* address offset: 0x0010 */
     uint32_t sata0_con;                          /* address offset: 0x001c */
     uint32_t sata1_con;                          /* address offset: 0x0020 */
     uint32_t usb3otg1_status_lat[2];             /* address offset: 0x0024 */
     uint32_t usb3otg1_status_cb;                 /* address offset: 0x002c */
     uint32_t usb3otg1_status;                    /* address offset: 0x0030 */
     uint32_t usb3otg1_con[2];                    /* address offset: 0x0034 */
     uint32_t reserved003c[3];                    /* address offset: 0x003c */
     uint32_t pciepipe_con[1];                    /* address offset: 0x0048 */
     uint32_t reserved004c[2];                    /* address offset: 0x004c */
     uint32_t pcie_clkreq_st;                     /* address offset: 0x0054 */
     uint32_t reserved0058;                       /* address offset: 0x0058 */
     uint32_t mmu0_st[5];                         /* address offset: 0x005c */
     uint32_t mmu1_st[5];                         /* address offset: 0x0070 */
};

check_member(rk3576_php_grf_reg, mmu1_st, 0x0070);

/* pmu0_grf register structure define */
struct rk3576_pmu0_grf_reg {
     uint32_t soc_con[7];                         /* address offset: 0x0000 */
     uint32_t reserved001c;                       /* address offset: 0x001c */
     uint32_t io_ret_con[2];                      /* address offset: 0x0020 */
     uint32_t reserved0028[2];                    /* address offset: 0x0028 */
     uint32_t mem_con;                            /* address offset: 0x0030 */
     uint32_t reserved0034[3];                    /* address offset: 0x0034 */
     uint32_t os_reg[8];                          /* address offset: 0x0040 */
};

check_member(rk3576_pmu0_grf_reg, os_reg, 0x0040);

/* pmu0_sgrf register structure define */
struct rk3576_pmu0_sgrf_reg {
     uint32_t soc_con[3];                         /* address offset: 0x0000 */
     uint32_t reserved000c[13];                   /* address offset: 0x000c */
     uint32_t dcie_con[8];                        /* address offset: 0x0040 */
     uint32_t dcie_wlock;                         /* address offset: 0x0060 */
};

check_member(rk3576_pmu0_sgrf_reg, dcie_wlock, 0x0060);

/* pmu1_grf register structure define */
struct rk3576_pmu1_grf_reg {
     uint32_t soc_con[8];                         /* address offset: 0x0000 */
     uint32_t reserved0020[12];                   /* address offset: 0x0020 */
     uint32_t biu_con;                            /* address offset: 0x0050 */
     uint32_t biu_status;                         /* address offset: 0x0054 */
     uint32_t reserved0058[2];                    /* address offset: 0x0058 */
     uint32_t soc_status;                         /* address offset: 0x0060 */
     uint32_t reserved0064[7];                    /* address offset: 0x0064 */
     uint32_t mem_con[2];                         /* address offset: 0x0080 */
     uint32_t reserved0088[30];                   /* address offset: 0x0088 */
     uint32_t func_rst_status;                    /* address offset: 0x0100 */
    __o  uint32_t func_rst_clr;                       /* address offset: 0x0104 */
     uint32_t reserved0108[2];                    /* address offset: 0x0108 */
     uint32_t sd_detect_con;                      /* address offset: 0x0110 */
     uint32_t sd_detect_sts;                      /* address offset: 0x0114 */
    __o  uint32_t sd_detect_clr;                      /* address offset: 0x0118 */
     uint32_t sd_detect_cnt;                      /* address offset: 0x011c */
     uint32_t reserved0120[56];                   /* address offset: 0x0120 */
     uint32_t os_reg[16];                         /* address offset: 0x0200 */
};

check_member(rk3576_pmu1_grf_reg, os_reg, 0x0200);

/* pmu1_sgrf register structure define */
struct rk3576_pmu1_sgrf_reg {
     uint32_t soc_con[18];                        /* address offset: 0x0000 */
};

check_member(rk3576_pmu1_sgrf_reg, soc_con, 0x0000);

/* sdgmac_grf register structure define */
struct rk3576_sdgmac_grf_reg {
     uint32_t mem_con[5];                         /* address offset: 0x0000 */
     uint32_t reserved0014[2];                    /* address offset: 0x0014 */
     uint32_t gmac_st[1];                         /* address offset: 0x001c */
     uint32_t gmac0_con;                          /* address offset: 0x0020 */
     uint32_t gmac1_con;                          /* address offset: 0x0024 */
     uint32_t gmac0_tp[2];                        /* address offset: 0x0028 */
     uint32_t gmac1_tp[2];                        /* address offset: 0x0030 */
     uint32_t gmac0_cmd;                          /* address offset: 0x0038 */
     uint32_t gmac1_cmd;                          /* address offset: 0x003c */
     uint32_t reserved0040[2];                    /* address offset: 0x0040 */
     uint32_t mem_gate_con;                       /* address offset: 0x0048 */
};

check_member(rk3576_sdgmac_grf_reg, mem_gate_con, 0x0048);

/* sys_grf register structure define */
struct rk3576_sys_grf_reg {
     uint32_t soc_con[13];                        /* address offset: 0x0000 */
     uint32_t reserved0034[3];                    /* address offset: 0x0034 */
     uint32_t biu_con[6];                         /* address offset: 0x0040 */
     uint32_t reserved0058[2];                    /* address offset: 0x0058 */
     uint32_t biu_status[8];                      /* address offset: 0x0060 */
     uint32_t mem_con[19];                        /* address offset: 0x0080 */
     uint32_t reserved00cc[29];                   /* address offset: 0x00cc */
     uint32_t soc_status[2];                      /* address offset: 0x0140 */
     uint32_t memfault_status[2];                 /* address offset: 0x0148 */
     uint32_t reserved0150[12];                   /* address offset: 0x0150 */
     uint32_t soc_code;                           /* address offset: 0x0180 */
     uint32_t reserved0184[3];                    /* address offset: 0x0184 */
     uint32_t soc_version;                        /* address offset: 0x0190 */
     uint32_t reserved0194[3];                    /* address offset: 0x0194 */
     uint32_t chip_id;                            /* address offset: 0x01a0 */
     uint32_t reserved01a4[3];                    /* address offset: 0x01a4 */
     uint32_t chip_version;                       /* address offset: 0x01b0 */
};

check_member(rk3576_sys_grf_reg, chip_version, 0x01b0);

/* sys_sgrf register structure define */
struct rk3576_sys_sgrf_reg {
     uint32_t ddr_bank_hash_ctrl;                 /* address offset: 0x0000 */
     uint32_t ddr_bank_mask[4];                   /* address offset: 0x0004 */
     uint32_t ddr_rank_mask[1];                   /* address offset: 0x0014 */
     uint32_t reserved0018[2];                    /* address offset: 0x0018 */
     uint32_t soc_con[21];                        /* address offset: 0x0020 */
     uint32_t reserved0074[3];                    /* address offset: 0x0074 */
     uint32_t dmac0_con[10];                      /* address offset: 0x0080 */
     uint32_t reserved00a8[22];                   /* address offset: 0x00a8 */
     uint32_t dmac1_con[10];                      /* address offset: 0x0100 */
     uint32_t reserved0128[22];                   /* address offset: 0x0128 */
     uint32_t dmac2_con[10];                      /* address offset: 0x0180 */
     uint32_t reserved01a8[22];                   /* address offset: 0x01a8 */
     uint32_t key_con[2];                         /* address offset: 0x0200 */
     uint32_t key_wlock;                          /* address offset: 0x0208 */
     uint32_t reserved020c[13];                   /* address offset: 0x020c */
     uint32_t soc_status;                         /* address offset: 0x0240 */
     uint32_t reserved0244[47];                   /* address offset: 0x0244 */
     uint32_t ip_info_con;                        /* address offset: 0x0300 */
};

check_member(rk3576_sys_sgrf_reg, ip_info_con, 0x0300);

/* ufs_grf register structure define */
struct rk3576_ufs_grf_reg {
     uint32_t clk_ctrl;                           /* address offset: 0x0000 */
     uint32_t uic_src_sel;                        /* address offset: 0x0004 */
     uint32_t ufs_state_ie;                       /* address offset: 0x0008 */
     uint32_t ufs_state_is;                       /* address offset: 0x000c */
     uint32_t ufs_state;                          /* address offset: 0x0010 */
     uint32_t reserved0014[13];                   /* address offset: 0x0014 */
};

check_member(rk3576_ufs_grf_reg, reserved0014, 0x0014);

/* usbdpphy_grf register structure define */
struct rk3576_usbdpphy_grf_reg {
     uint32_t reserved0000;                       /* address offset: 0x0000 */
     uint32_t con[3];                             /* address offset: 0x0004 */
     uint32_t reserved0010[29];                   /* address offset: 0x0010 */
     uint32_t status[1];                          /* address offset: 0x0084 */
     uint32_t reserved0088[14];                   /* address offset: 0x0088 */
     uint32_t lfps_det_con;                       /* address offset: 0x00c0 */
     uint32_t int_en;                             /* address offset: 0x00c4 */
     uint32_t int_status;                         /* address offset: 0x00c8 */
};

check_member(rk3576_usbdpphy_grf_reg, int_status, 0x00c8);

/* usb_grf register structure define */
struct rk3576_usb_grf_reg {
     uint32_t mmubp_st;                           /* address offset: 0x0000 */
     uint32_t mmubp_con;                          /* address offset: 0x0004 */
     uint32_t mmu2_con;                           /* address offset: 0x0008 */
     uint32_t mem_con0;                           /* address offset: 0x000c */
     uint32_t mem_con1;                           /* address offset: 0x0010 */
     uint32_t reserved0014[2];                    /* address offset: 0x0014 */
     uint32_t usb3otg0_status_lat[2];             /* address offset: 0x001c */
     uint32_t usb3otg0_status_cb;                 /* address offset: 0x0024 */
     uint32_t usb3otg0_status;                    /* address offset: 0x0028 */
     uint32_t usb3otg0_con[2];                    /* address offset: 0x002c */
     uint32_t reserved0034[4];                    /* address offset: 0x0034 */
     uint32_t mmu2_st[5];                         /* address offset: 0x0044 */
     uint32_t mem_con[1];                         /* address offset: 0x0058 */
};

check_member(rk3576_usb_grf_reg, mem_con, 0x0058);

/* vo0_grf register structure define */
struct rk3576_vo0_grf_reg {
     uint32_t soc_con[34];                        /* address offset: 0x0000 */
     uint32_t reserved0088[14];                   /* address offset: 0x0088 */
     uint32_t soc_st[6];                          /* address offset: 0x00c0 */
     uint32_t reserved00d8[6];                    /* address offset: 0x00d8 */
     uint32_t hdcp0_rng_con[1];                   /* address offset: 0x00f0 */
     uint32_t reserved00f4;                       /* address offset: 0x00f4 */
     uint32_t hdcp0_rng_st[1];                    /* address offset: 0x00f8 */
};

check_member(rk3576_vo0_grf_reg, hdcp0_rng_st, 0x00f8);

/* vo1_grf register structure define */
struct rk3576_vo1_grf_reg {
     uint32_t soc_con[35];                        /* address offset: 0x0000 */
     uint32_t reserved008c[13];                   /* address offset: 0x008c */
     uint32_t soc_st[3];                          /* address offset: 0x00c0 */
     uint32_t reserved00cc[9];                    /* address offset: 0x00cc */
     uint32_t hdcp1_rng_con[1];                   /* address offset: 0x00f0 */
     uint32_t reserved00f4;                       /* address offset: 0x00f4 */
     uint32_t hdcp1_rng_st[1];                    /* address offset: 0x00f8 */
};

check_member(rk3576_vo1_grf_reg, hdcp1_rng_st, 0x00f8);

/* vop_grf register structure define */
struct rk3576_vop_grf_reg {
     uint32_t reserved0000;                       /* address offset: 0x0000 */
     uint32_t soc_con[3];                         /* address offset: 0x0004 */
};

check_member(rk3576_vop_grf_reg, soc_con, 0x0004);

#endif /*  _ASM_ARCH_GRF_RK3576_H  */

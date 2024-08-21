/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_IOC_RK3576_H
#define _ASM_ARCH_IOC_RK3576_H

#include <common.h>

/* pmu0_ioc register structure define */
struct rk3576_pmu0_ioc_reg {
     uint32_t gpio0a_iomux_sel_l;                 /* address offset: 0x0000 */
     uint32_t gpio0a_iomux_sel_h;                 /* address offset: 0x0004 */
     uint32_t gpio0b_iomux_sel_l;                 /* address offset: 0x0008 */
     uint32_t reserved000c;                       /* address offset: 0x000c */
     uint32_t gpio0a_ds_l;                        /* address offset: 0x0010 */
     uint32_t gpio0a_ds_h;                        /* address offset: 0x0014 */
     uint32_t gpio0b_ds_l;                        /* address offset: 0x0018 */
     uint32_t reserved001c;                       /* address offset: 0x001c */
     uint32_t gpio0a_pull;                        /* address offset: 0x0020 */
     uint32_t gpio0b_pull_l;                      /* address offset: 0x0024 */
     uint32_t gpio0a_ie;                          /* address offset: 0x0028 */
     uint32_t gpio0b_ie_l;                        /* address offset: 0x002c */
     uint32_t gpio0a_smt;                         /* address offset: 0x0030 */
     uint32_t gpio0b_smt_l;                       /* address offset: 0x0034 */
     uint32_t gpio0a_pdis;                        /* address offset: 0x0038 */
     uint32_t gpio0b_pdis_l;                      /* address offset: 0x003c */
     uint32_t osc_con;                            /* address offset: 0x0040 */
};

check_member(rk3576_pmu0_ioc_reg, osc_con, 0x0040);

/* pmu1_ioc register structure define */
struct rk3576_pmu1_ioc_reg {
     uint32_t gpio0b_iomux_sel_h;                 /* address offset: 0x0000 */
     uint32_t gpio0c_iomux_sel_l;                 /* address offset: 0x0004 */
     uint32_t gpio0c_iomux_sel_h;                 /* address offset: 0x0008 */
     uint32_t gpio0d_iomux_sel_l;                 /* address offset: 0x000c */
     uint32_t gpio0d_iomux_sel_h;                 /* address offset: 0x0010 */
     uint32_t gpio0b_ds_h;                        /* address offset: 0x0014 */
     uint32_t gpio0c_ds_l;                        /* address offset: 0x0018 */
     uint32_t gpio0c_ds_h;                        /* address offset: 0x001c */
     uint32_t gpio0d_ds_l;                        /* address offset: 0x0020 */
     uint32_t gpio0d_ds_h;                        /* address offset: 0x0024 */
     uint32_t gpio0b_pull_h;                      /* address offset: 0x0028 */
     uint32_t gpio0c_pull;                        /* address offset: 0x002c */
     uint32_t gpio0d_pull;                        /* address offset: 0x0030 */
     uint32_t gpio0b_ie_h;                        /* address offset: 0x0034 */
     uint32_t gpio0c_ie;                          /* address offset: 0x0038 */
     uint32_t gpio0d_ie;                          /* address offset: 0x003c */
     uint32_t gpio0b_smt_h;                       /* address offset: 0x0040 */
     uint32_t gpio0c_smt;                         /* address offset: 0x0044 */
     uint32_t gpio0d_smt;                         /* address offset: 0x0048 */
     uint32_t gpio0b_pdis_h;                      /* address offset: 0x004c */
     uint32_t gpio0c_pdis;                        /* address offset: 0x0050 */
     uint32_t gpio0d_pdis;                        /* address offset: 0x0054 */
};

check_member(rk3576_pmu1_ioc_reg, gpio0d_pdis, 0x0054);

/* top_ioc register structure define */
struct rk3576_top_ioc_reg {
     uint32_t reserved0000[2];                    /* address offset: 0x0000 */
     uint32_t gpio0b_iomux_sel_l;                 /* address offset: 0x0008 */
     uint32_t gpio0b_iomux_sel_h;                 /* address offset: 0x000c */
     uint32_t gpio0c_iomux_sel_l;                 /* address offset: 0x0010 */
     uint32_t gpio0c_iomux_sel_h;                 /* address offset: 0x0014 */
     uint32_t gpio0d_iomux_sel_l;                 /* address offset: 0x0018 */
     uint32_t gpio0d_iomux_sel_h;                 /* address offset: 0x001c */
     uint32_t gpio1a_iomux_sel_l;                 /* address offset: 0x0020 */
     uint32_t gpio1a_iomux_sel_h;                 /* address offset: 0x0024 */
     uint32_t gpio1b_iomux_sel_l;                 /* address offset: 0x0028 */
     uint32_t gpio1b_iomux_sel_h;                 /* address offset: 0x002c */
     uint32_t gpio1c_iomux_sel_l;                 /* address offset: 0x0030 */
     uint32_t gpio1c_iomux_sel_h;                 /* address offset: 0x0034 */
     uint32_t gpio1d_iomux_sel_l;                 /* address offset: 0x0038 */
     uint32_t gpio1d_iomux_sel_h;                 /* address offset: 0x003c */
     uint32_t gpio2a_iomux_sel_l;                 /* address offset: 0x0040 */
     uint32_t gpio2a_iomux_sel_h;                 /* address offset: 0x0044 */
     uint32_t gpio2b_iomux_sel_l;                 /* address offset: 0x0048 */
     uint32_t gpio2b_iomux_sel_h;                 /* address offset: 0x004c */
     uint32_t gpio2c_iomux_sel_l;                 /* address offset: 0x0050 */
     uint32_t gpio2c_iomux_sel_h;                 /* address offset: 0x0054 */
     uint32_t gpio2d_iomux_sel_l;                 /* address offset: 0x0058 */
     uint32_t gpio2d_iomux_sel_h;                 /* address offset: 0x005c */
     uint32_t gpio3a_iomux_sel_l;                 /* address offset: 0x0060 */
     uint32_t gpio3a_iomux_sel_h;                 /* address offset: 0x0064 */
     uint32_t gpio3b_iomux_sel_l;                 /* address offset: 0x0068 */
     uint32_t gpio3b_iomux_sel_h;                 /* address offset: 0x006c */
     uint32_t gpio3c_iomux_sel_l;                 /* address offset: 0x0070 */
     uint32_t gpio3c_iomux_sel_h;                 /* address offset: 0x0074 */
     uint32_t gpio3d_iomux_sel_l;                 /* address offset: 0x0078 */
     uint32_t gpio3d_iomux_sel_h;                 /* address offset: 0x007c */
     uint32_t gpio4a_iomux_sel_l;                 /* address offset: 0x0080 */
     uint32_t gpio4a_iomux_sel_h;                 /* address offset: 0x0084 */
     uint32_t gpio4b_iomux_sel_l;                 /* address offset: 0x0088 */
     uint32_t gpio4b_iomux_sel_h;                 /* address offset: 0x008c */
     uint32_t reserved0090[24];                   /* address offset: 0x0090 */
     uint32_t ioc_misc_con;                       /* address offset: 0x00f0 */
     uint32_t sdmmc_detn_flt;                     /* address offset: 0x00f4 */
};

check_member(rk3576_top_ioc_reg, sdmmc_detn_flt, 0x00f4);

/* vccio_ioc register structure define */
struct rk3576_vccio_ioc_reg {
     uint32_t reserved0000[8];                    /* address offset: 0x0000 */
     uint32_t gpio1a_ds_l;                        /* address offset: 0x0020 */
     uint32_t gpio1a_ds_h;                        /* address offset: 0x0024 */
     uint32_t gpio1b_ds_l;                        /* address offset: 0x0028 */
     uint32_t gpio1b_ds_h;                        /* address offset: 0x002c */
     uint32_t gpio1c_ds_l;                        /* address offset: 0x0030 */
     uint32_t gpio1c_ds_h;                        /* address offset: 0x0034 */
     uint32_t gpio1d_ds_l;                        /* address offset: 0x0038 */
     uint32_t gpio1d_ds_h;                        /* address offset: 0x003c */
     uint32_t gpio2a_ds_l;                        /* address offset: 0x0040 */
     uint32_t gpio2a_ds_h;                        /* address offset: 0x0044 */
     uint32_t gpio2b_ds_l;                        /* address offset: 0x0048 */
     uint32_t gpio2b_ds_h;                        /* address offset: 0x004c */
     uint32_t gpio2c_ds_l;                        /* address offset: 0x0050 */
     uint32_t gpio2c_ds_h;                        /* address offset: 0x0054 */
     uint32_t gpio2d_ds_l;                        /* address offset: 0x0058 */
     uint32_t gpio2d_ds_h;                        /* address offset: 0x005c */
     uint32_t gpio3a_ds_l;                        /* address offset: 0x0060 */
     uint32_t gpio3a_ds_h;                        /* address offset: 0x0064 */
     uint32_t gpio3b_ds_l;                        /* address offset: 0x0068 */
     uint32_t gpio3b_ds_h;                        /* address offset: 0x006c */
     uint32_t gpio3c_ds_l;                        /* address offset: 0x0070 */
     uint32_t gpio3c_ds_h;                        /* address offset: 0x0074 */
     uint32_t gpio3d_ds_l;                        /* address offset: 0x0078 */
     uint32_t gpio3d_ds_h;                        /* address offset: 0x007c */
     uint32_t gpio4a_ds_l;                        /* address offset: 0x0080 */
     uint32_t gpio4a_ds_h;                        /* address offset: 0x0084 */
     uint32_t gpio4b_ds_l;                        /* address offset: 0x0088 */
     uint32_t gpio4b_ds_h;                        /* address offset: 0x008c */
     uint32_t reserved0090[32];                   /* address offset: 0x0090 */
     uint32_t gpio1a_pull;                        /* address offset: 0x0110 */
     uint32_t gpio1b_pull;                        /* address offset: 0x0114 */
     uint32_t gpio1c_pull;                        /* address offset: 0x0118 */
     uint32_t gpio1d_pull;                        /* address offset: 0x011c */
     uint32_t gpio2a_pull;                        /* address offset: 0x0120 */
     uint32_t gpio2b_pull;                        /* address offset: 0x0124 */
     uint32_t gpio2c_pull;                        /* address offset: 0x0128 */
     uint32_t gpio2d_pull;                        /* address offset: 0x012c */
     uint32_t gpio3a_pull;                        /* address offset: 0x0130 */
     uint32_t gpio3b_pull;                        /* address offset: 0x0134 */
     uint32_t gpio3c_pull;                        /* address offset: 0x0138 */
     uint32_t gpio3d_pull;                        /* address offset: 0x013c */
     uint32_t gpio4a_pull;                        /* address offset: 0x0140 */
     uint32_t gpio4b_pull;                        /* address offset: 0x0144 */
     uint32_t reserved0148[14];                   /* address offset: 0x0148 */
     uint32_t gpio1a_ie;                          /* address offset: 0x0180 */
     uint32_t gpio1b_ie;                          /* address offset: 0x0184 */
     uint32_t gpio1c_ie;                          /* address offset: 0x0188 */
     uint32_t gpio1d_ie;                          /* address offset: 0x018c */
     uint32_t gpio2a_ie;                          /* address offset: 0x0190 */
     uint32_t gpio2b_ie;                          /* address offset: 0x0194 */
     uint32_t gpio2c_ie;                          /* address offset: 0x0198 */
     uint32_t gpio2d_ie;                          /* address offset: 0x019c */
     uint32_t gpio3a_ie;                          /* address offset: 0x01a0 */
     uint32_t gpio3b_ie;                          /* address offset: 0x01a4 */
     uint32_t gpio3c_ie;                          /* address offset: 0x01a8 */
     uint32_t gpio3d_ie;                          /* address offset: 0x01ac */
     uint32_t gpio4a_ie;                          /* address offset: 0x01b0 */
     uint32_t gpio4b_ie;                          /* address offset: 0x01b4 */
     uint32_t reserved01b8[22];                   /* address offset: 0x01b8 */
     uint32_t gpio1a_smt;                         /* address offset: 0x0210 */
     uint32_t gpio1b_smt;                         /* address offset: 0x0214 */
     uint32_t gpio1c_smt;                         /* address offset: 0x0218 */
     uint32_t gpio1d_smt;                         /* address offset: 0x021c */
     uint32_t gpio2a_smt;                         /* address offset: 0x0220 */
     uint32_t gpio2b_smt;                         /* address offset: 0x0224 */
     uint32_t gpio2c_smt;                         /* address offset: 0x0228 */
     uint32_t gpio2d_smt;                         /* address offset: 0x022c */
     uint32_t gpio3a_smt;                         /* address offset: 0x0230 */
     uint32_t gpio3b_smt;                         /* address offset: 0x0234 */
     uint32_t gpio3c_smt;                         /* address offset: 0x0238 */
     uint32_t gpio3d_smt;                         /* address offset: 0x023c */
     uint32_t gpio4a_smt;                         /* address offset: 0x0240 */
     uint32_t gpio4b_smt;                         /* address offset: 0x0244 */
     uint32_t reserved0248[14];                   /* address offset: 0x0248 */
     uint32_t gpio1a_pdis;                        /* address offset: 0x0280 */
     uint32_t gpio1b_pdis;                        /* address offset: 0x0284 */
     uint32_t gpio1c_pdis;                        /* address offset: 0x0288 */
     uint32_t gpio1d_pdis;                        /* address offset: 0x028c */
     uint32_t gpio2a_pdis;                        /* address offset: 0x0290 */
     uint32_t gpio2b_pdis;                        /* address offset: 0x0294 */
     uint32_t gpio2c_pdis;                        /* address offset: 0x0298 */
     uint32_t gpio2d_pdis;                        /* address offset: 0x029c */
     uint32_t gpio3a_pdis;                        /* address offset: 0x02a0 */
     uint32_t gpio3b_pdis;                        /* address offset: 0x02a4 */
     uint32_t gpio3c_pdis;                        /* address offset: 0x02a8 */
     uint32_t gpio3d_pdis;                        /* address offset: 0x02ac */
     uint32_t gpio4a_pdis;                        /* address offset: 0x02b0 */
     uint32_t gpio4b_pdis;                        /* address offset: 0x02b4 */
     uint32_t reserved02b8[82];                   /* address offset: 0x02b8 */
     uint32_t misc_con[9];                        /* address offset: 0x0400 */
};

check_member(rk3576_vccio_ioc_reg, misc_con, 0x0400);

/* vccio6_ioc register structure define */
struct rk3576_vccio6_ioc_reg {
     uint32_t reserved0000[36];                   /* address offset: 0x0000 */
     uint32_t gpio4c_ds_l;                        /* address offset: 0x0090 */
     uint32_t gpio4c_ds_h;                        /* address offset: 0x0094 */
     uint32_t reserved0098[44];                   /* address offset: 0x0098 */
     uint32_t gpio4c_pull;                        /* address offset: 0x0148 */
     uint32_t reserved014c[27];                   /* address offset: 0x014c */
     uint32_t gpio4c_ie;                          /* address offset: 0x01b8 */
     uint32_t reserved01bc[35];                   /* address offset: 0x01bc */
     uint32_t gpio4c_smt;                         /* address offset: 0x0248 */
     uint32_t reserved024c[27];                   /* address offset: 0x024c */
     uint32_t gpio4c_pdis;                        /* address offset: 0x02b8 */
     uint32_t reserved02bc[53];                   /* address offset: 0x02bc */
     uint32_t gpio4c_iomux_sel_l;                 /* address offset: 0x0390 */
     uint32_t gpio4c_iomux_sel_h;                 /* address offset: 0x0394 */
     uint32_t reserved0398[26];                   /* address offset: 0x0398 */
     uint32_t misc_con[2];                        /* address offset: 0x0400 */
     uint32_t reserved0408[14];                   /* address offset: 0x0408 */
     uint32_t hdmitx_hpd_status;                  /* address offset: 0x0440 */
};

check_member(rk3576_vccio6_ioc_reg, hdmitx_hpd_status, 0x0440);

/* vccio7_ioc register structure define */
struct rk3576_vccio7_ioc_reg {
     uint32_t reserved0000[38];                   /* address offset: 0x0000 */
     uint32_t gpio4d_ds_l;                        /* address offset: 0x0098 */
     uint32_t reserved009c[44];                   /* address offset: 0x009c */
     uint32_t gpio4d_pull;                        /* address offset: 0x014c */
     uint32_t reserved0150[27];                   /* address offset: 0x0150 */
     uint32_t gpio4d_ie;                          /* address offset: 0x01bc */
     uint32_t reserved01c0[35];                   /* address offset: 0x01c0 */
     uint32_t gpio4d_smt;                         /* address offset: 0x024c */
     uint32_t reserved0250[27];                   /* address offset: 0x0250 */
     uint32_t gpio4d_pdis;                        /* address offset: 0x02bc */
     uint32_t reserved02c0[54];                   /* address offset: 0x02c0 */
     uint32_t gpio4d_iomux_sel_l;                 /* address offset: 0x0398 */
     uint32_t reserved039c[25];                   /* address offset: 0x039c */
     uint32_t xin_ufs_con;                        /* address offset: 0x0400 */
};

check_member(rk3576_vccio7_ioc_reg, xin_ufs_con, 0x0400);

#endif /* _ASM_ARCH_IOC_RK3576_H */

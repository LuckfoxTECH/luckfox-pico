/*
 * (C) Copyright 2021 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_IOC_RK3528_H
#define _ASM_ARCH_IOC_RK3528_H

#include <common.h>

struct rk3528_gpio0_ioc {
	uint32_t gpio0a_iomux_sel_l;                 /* Address Offset: 0x0000 */
	uint32_t gpio0a_iomux_sel_h;                 /* Address Offset: 0x0004 */
	uint32_t reserved0008[62];                   /* Address Offset: 0x0008 */
	uint32_t gpio0a_ds[3];                       /* Address Offset: 0x0100 */
	uint32_t reserved010c[61];                   /* Address Offset: 0x010C */
	uint32_t gpio0a_pull;                        /* Address Offset: 0x0200 */
	uint32_t reserved0204[63];                   /* Address Offset: 0x0204 */
	uint32_t gpio0a_ie;                          /* Address Offset: 0x0300 */
	uint32_t reserved0304[63];                   /* Address Offset: 0x0304 */
	uint32_t gpio0a_smt;                         /* Address Offset: 0x0400 */
	uint32_t reserved0404[63];                   /* Address Offset: 0x0404 */
	uint32_t gpio0a_sus;                         /* Address Offset: 0x0500 */
	uint32_t reserved0504[63];                   /* Address Offset: 0x0504 */
	uint32_t gpio0a_sl;                          /* Address Offset: 0x0600 */
	uint32_t reserved0604[63];                   /* Address Offset: 0x0604 */
	uint32_t gpio0a_od;                          /* Address Offset: 0x0700 */
	uint32_t vcc5vio_ctrl;                          /* Address Offset: 0x0704 */
};
check_member(rk3528_gpio0_ioc, vcc5vio_ctrl, 0x0704);

struct rk3528_gpio1_ioc {
	uint32_t reserved0000[8];                    /* Address Offset: 0x0000 */
	uint32_t gpio1a_iomux_sel_l;                 /* Address Offset: 0x0020 */
	uint32_t gpio1a_iomux_sel_h;                 /* Address Offset: 0x0024 */
	uint32_t gpio1b_iomux_sel_l;                 /* Address Offset: 0x0028 */
	uint32_t gpio1b_iomux_sel_h;                 /* Address Offset: 0x002C */
	uint32_t gpio1c_iomux_sel_l;                 /* Address Offset: 0x0030 */
	uint32_t gpio1c_iomux_sel_h;                 /* Address Offset: 0x0034 */
	uint32_t gpio1d_iomux_sel_l;                 /* Address Offset: 0x0038 */
	uint32_t gpio1d_iomux_sel_h;                 /* Address Offset: 0x003C */
	uint32_t reserved0040[56];                   /* Address Offset: 0x0040 */
	uint32_t gpio1a_ds[4];                       /* Address Offset: 0x0120 */
	uint32_t gpio1b_ds[4];                       /* Address Offset: 0x0130 */
	uint32_t gpio1c_ds[4];                       /* Address Offset: 0x0140 */
	uint32_t gpio1d_ds[4];                       /* Address Offset: 0x0150 */
	uint32_t reserved0160[44];                   /* Address Offset: 0x0160 */
	uint32_t gpio1a_pull;                        /* Address Offset: 0x0210 */
	uint32_t gpio1b_pull;                        /* Address Offset: 0x0214 */
	uint32_t gpio1c_pull;                        /* Address Offset: 0x0218 */
	uint32_t gpio1d_pull;                        /* Address Offset: 0x021C */
	uint32_t reserved0220[60];                   /* Address Offset: 0x0220 */
	uint32_t gpio1a_ie;                          /* Address Offset: 0x0310 */
	uint32_t gpio1b_ie;                          /* Address Offset: 0x0314 */
	uint32_t gpio1c_ie;                          /* Address Offset: 0x0318 */
	uint32_t gpio1d_ie;                          /* Address Offset: 0x031C */
	uint32_t reserved0320[60];                   /* Address Offset: 0x0320 */
	uint32_t gpio1a_smt;                         /* Address Offset: 0x0410 */
	uint32_t gpio1b_smt;                         /* Address Offset: 0x0414 */
	uint32_t gpio1c_smt;                         /* Address Offset: 0x0418 */
	uint32_t gpio1d_smt;                         /* Address Offset: 0x041C */
	uint32_t reserved0420[60];                   /* Address Offset: 0x0420 */
	uint32_t gpio1a_sus;                         /* Address Offset: 0x0510 */
	uint32_t gpio1b_sus;                         /* Address Offset: 0x0514 */
	uint32_t gpio1c_sus;                         /* Address Offset: 0x0518 */
	uint32_t gpio1d_sus;                         /* Address Offset: 0x051C */
	uint32_t reserved0520[60];                   /* Address Offset: 0x0520 */
	uint32_t gpio1a_sl;                          /* Address Offset: 0x0610 */
	uint32_t gpio1b_sl;                          /* Address Offset: 0x0614 */
	uint32_t gpio1c_sl;                          /* Address Offset: 0x0618 */
	uint32_t gpio1d_sl;                          /* Address Offset: 0x061C */
	uint32_t reserved0620[60];                   /* Address Offset: 0x0620 */
	uint32_t gpio1a_od;                          /* Address Offset: 0x0710 */
	uint32_t gpio1b_od;                          /* Address Offset: 0x0714 */
	uint32_t gpio1c_od;                          /* Address Offset: 0x0718 */
	uint32_t gpio1d_od;                          /* Address Offset: 0x071C */
	uint32_t reserved0720[60];                   /* Address Offset: 0x0720 */
	uint32_t vccio0_poc;                         /* Address Offset: 0x0810 */
	uint32_t reserved0814[3];                    /* Address Offset: 0x0814 */
	uint32_t vccio1_poc;                         /* Address Offset: 0x0820 */
};
check_member(rk3528_gpio1_ioc, vccio1_poc, 0x0820);

struct rk3528_gpio2_ioc {
	uint32_t reserved0000[16];                   /* Address Offset: 0x0000 */
	uint32_t gpio2a_iomux_sel_l;                 /* Address Offset: 0x0040 */
	uint32_t gpio2a_iomux_sel_h;                 /* Address Offset: 0x0044 */
	uint32_t reserved0048[70];                   /* Address Offset: 0x0048 */
	uint32_t gpio2a_ds[4];                       /* Address Offset: 0x0160 */
	uint32_t reserved0170[44];                   /* Address Offset: 0x0170 */
	uint32_t gpio2a_pull;                        /* Address Offset: 0x0220 */
	uint32_t reserved0224[63];                   /* Address Offset: 0x0224 */
	uint32_t gpio2a_ie;                          /* Address Offset: 0x0320 */
	uint32_t reserved0324[63];                   /* Address Offset: 0x0324 */
	uint32_t gpio2a_smt;                         /* Address Offset: 0x0420 */
	uint32_t reserved0424[63];                   /* Address Offset: 0x0424 */
	uint32_t gpio2a_sus;                         /* Address Offset: 0x0520 */
	uint32_t reserved0524[63];                   /* Address Offset: 0x0524 */
	uint32_t gpio2a_sl;                          /* Address Offset: 0x0620 */
	uint32_t reserved0624[63];                   /* Address Offset: 0x0624 */
	uint32_t gpio2a_od;                          /* Address Offset: 0x0720 */
	uint32_t reserved0724[67];                   /* Address Offset: 0x0724 */
	uint32_t vccio2_poc;                         /* Address Offset: 0x0830 */
};
check_member(rk3528_gpio2_ioc, vccio2_poc, 0x0830);
	
struct rk3528_gpio3_ioc {
	uint32_t reserved0000[24];                   /* Address Offset: 0x0000 */
	uint32_t gpio3a_iomux_sel_l;                 /* Address Offset: 0x0060 */
	uint32_t gpio3a_iomux_sel_h;                 /* Address Offset: 0x0064 */
	uint32_t gpio3b_iomux_sel_l;                 /* Address Offset: 0x0068 */
	uint32_t gpio3b_iomux_sel_h;                 /* Address Offset: 0x006C */
	uint32_t gpio3c_iomux_sel_l;                 /* Address Offset: 0x0070 */
	uint32_t reserved0074[71];                   /* Address Offset: 0x0074 */
	uint32_t gpio3a_ds[4];                       /* Address Offset: 0x0190 */
	uint32_t gpio3b_ds[4];                       /* Address Offset: 0x01A0 */
	uint32_t gpio3c_ds[2];                       /* Address Offset: 0x01B0 */
	uint32_t reserved01b8[30];                   /* Address Offset: 0x01B8 */
	uint32_t gpio3a_pull;                        /* Address Offset: 0x0230 */
	uint32_t gpio3b_pull;                        /* Address Offset: 0x0234 */
	uint32_t gpio3c_pull;                        /* Address Offset: 0x0238 */
	uint32_t reserved023c[61];                   /* Address Offset: 0x023C */
	uint32_t gpio3a_ie;                          /* Address Offset: 0x0330 */
	uint32_t gpio3b_ie;                          /* Address Offset: 0x0334 */
	uint32_t gpio3c_ie;                          /* Address Offset: 0x0338 */
	uint32_t reserved033c[61];                   /* Address Offset: 0x033C */
	uint32_t gpio3a_smt;                         /* Address Offset: 0x0430 */
	uint32_t gpio3b_smt;                         /* Address Offset: 0x0434 */
	uint32_t gpio3c_smt;                         /* Address Offset: 0x0438 */
	uint32_t reserved043c[61];                   /* Address Offset: 0x043C */
	uint32_t gpio3a_sus;                         /* Address Offset: 0x0530 */
	uint32_t gpio3b_sus;                         /* Address Offset: 0x0534 */
	uint32_t gpio3c_sus;                         /* Address Offset: 0x0538 */
	uint32_t reserved053c[61];                   /* Address Offset: 0x053C */
	uint32_t gpio3a_sl;                          /* Address Offset: 0x0630 */
	uint32_t gpio3b_sl;                          /* Address Offset: 0x0634 */
	uint32_t gpio3c_sl;                          /* Address Offset: 0x0638 */
	uint32_t reserved063c[61];                   /* Address Offset: 0x063C */
	uint32_t gpio3a_od;                          /* Address Offset: 0x0730 */
	uint32_t gpio3b_od;                          /* Address Offset: 0x0734 */
	uint32_t gpio3c_od;                          /* Address Offset: 0x0738 */
	uint32_t reserved073c[65];                   /* Address Offset: 0x073C */
	uint32_t vccio3_poc;                         /* Address Offset: 0x0840 */
};
check_member(rk3528_gpio3_ioc, vccio3_poc, 0x0840);

struct rk3528_gpio4_ioc {
	uint32_t reserved0000[32];                   /* Address Offset: 0x0000 */
	uint32_t gpio4a_iomux_sel_l;                 /* Address Offset: 0x0080 */
	uint32_t gpio4a_iomux_sel_h;                 /* Address Offset: 0x0084 */
	uint32_t gpio4b_iomux_sel_l;                 /* Address Offset: 0x0088 */
	uint32_t gpio4b_iomux_sel_h;                 /* Address Offset: 0x008C */
	uint32_t gpio4c_iomux_sel_l;                 /* Address Offset: 0x0090 */
	uint32_t gpio4c_iomux_sel_h;                 /* Address Offset: 0x0094 */
	uint32_t gpio4d_iomux_sel_l;                 /* Address Offset: 0x0098 */
	uint32_t reserved009c[73];                   /* Address Offset: 0x009C */
	uint32_t gpio4a_ds[4];                       /* Address Offset: 0x01C0 */
	uint32_t gpio4b_ds[4];                       /* Address Offset: 0x01D0 */
	uint32_t gpio4c_ds[4];                       /* Address Offset: 0x01E0 */
	uint32_t gpio4d_ds[1];                       /* Address Offset: 0x01F0 */
	uint32_t reserved01f4[19];                   /* Address Offset: 0x01F4 */
	uint32_t gpio4a_pull;                        /* Address Offset: 0x0240 */
	uint32_t gpio4b_pull;                        /* Address Offset: 0x0244 */
	uint32_t gpio4c_pull;                        /* Address Offset: 0x0248 */
	uint32_t gpio4d_pull;                        /* Address Offset: 0x024C */
	uint32_t reserved0250[60];                   /* Address Offset: 0x0250 */
	uint32_t gpio4a_ie;                          /* Address Offset: 0x0340 */
	uint32_t gpio4b_ie;                          /* Address Offset: 0x0344 */
	uint32_t gpio4c_ie;                          /* Address Offset: 0x0348 */
	uint32_t gpio4d_ie;                          /* Address Offset: 0x034C */
	uint32_t reserved0350[60];                   /* Address Offset: 0x0350 */
	uint32_t gpio4a_smt;                         /* Address Offset: 0x0440 */
	uint32_t gpio4b_smt;                         /* Address Offset: 0x0444 */
	uint32_t gpio4c_smt;                         /* Address Offset: 0x0448 */
	uint32_t gpio4d_smt;                         /* Address Offset: 0x044C */
	uint32_t reserved0450[60];                   /* Address Offset: 0x0450 */
	uint32_t gpio4a_sus;                         /* Address Offset: 0x0540 */
	uint32_t gpio4b_sus;                         /* Address Offset: 0x0544 */
	uint32_t gpio4c_sus;                         /* Address Offset: 0x0548 */
	uint32_t gpio4d_sus;                         /* Address Offset: 0x054C */
	uint32_t reserved0550[60];                   /* Address Offset: 0x0550 */
	uint32_t gpio4a_sl;                          /* Address Offset: 0x0640 */
	uint32_t gpio4b_sl;                          /* Address Offset: 0x0644 */
	uint32_t gpio4c_sl;                          /* Address Offset: 0x0648 */
	uint32_t gpio4d_sl;                          /* Address Offset: 0x064C */
	uint32_t reserved0650[60];                   /* Address Offset: 0x0650 */
	uint32_t gpio4a_od;                          /* Address Offset: 0x0740 */
	uint32_t gpio4b_od;                          /* Address Offset: 0x0744 */
	uint32_t gpio4c_od;                          /* Address Offset: 0x0748 */
	uint32_t gpio4d_od;                          /* Address Offset: 0x074C */
	uint32_t reserved0750[64];                   /* Address Offset: 0x0750 */
	uint32_t vccio4_poc;                         /* Address Offset: 0x0850 */
};
check_member(rk3528_gpio4_ioc, vccio4_poc, 0x0850);
#endif


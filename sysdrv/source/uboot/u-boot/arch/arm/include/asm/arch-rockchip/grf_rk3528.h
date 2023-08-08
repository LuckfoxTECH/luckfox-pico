/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_GRF_RK3528_H
#define _ASM_ARCH_GRF_RK3528_H

#include <common.h>

struct rk3528_grf {
	uint32_t reserved0[0x40018 / 4];

	/* vpugrf*/
	uint32_t gmac1_con0;			     /* Address Offset: 0x40018 */
	uint32_t gmac1_con1;			     /* Address Offset: 0x4001c */
	uint32_t reserved1[(0x60018 - 0x4001c) / 4 - 1];
	
	/* vogrf */
	uint32_t gmac0_con;                          /* Address Offset: 0x60018 */
	uint32_t macphy_con0;                        /* Address Offset: 0x6001c */
	uint32_t macphy_con1;                        /* Address Offset: 0x60020 */
	uint32_t sdmmc_con0;                         /* Address Offset: 0x60024 */
	uint32_t sdmmc_con1;                         /* Address Offset: 0x60028 */
	uint32_t reserved2[(0x70000 - 0x60028) / 4 - 1];

	/* pmugrf */
	uint32_t soc_con[8];                         /* Address Offset: 0x70000 */
	uint32_t soc_status;                         /* Address Offset: 0x70020 */
	uint32_t reserved3[3];                       /* Address Offset: 0x70024 */
	uint32_t pmuio_vsel;                         /* Address Offset: 0x70030 */
	uint32_t reserved4[3];                       /* Address Offset: 0x70034 */
	uint32_t mem_con;                            /* Address Offset: 0x70040 */
	uint32_t reserved5[47];                      /* Address Offset: 0x70044 */
	uint32_t rstfunc_status;                     /* Address Offset: 0x70100 */
	uint32_t rstfunc_clr;                        /* Address Offset: 0x70104 */
	uint32_t reserved6[62];                      /* Address Offset: 0x70108 */
	uint32_t os_reg0;                            /* Address Offset: 0x70200 */
	uint32_t os_reg1;                            /* Address Offset: 0x70204 */
	uint32_t os_reg2;                            /* Address Offset: 0x70208 */
	uint32_t os_reg3;                            /* Address Offset: 0x7020C */
	uint32_t os_reg4;                            /* Address Offset: 0x70210 */
	uint32_t os_reg5;                            /* Address Offset: 0x70214 */
	uint32_t os_reg6;                            /* Address Offset: 0x70218 */
	uint32_t os_reg7;                            /* Address Offset: 0x7021C */
	uint32_t os_reg8;                            /* Address Offset: 0x70220 */
	uint32_t os_reg9;                            /* Address Offset: 0x70224 */
	uint32_t os_reg10;                           /* Address Offset: 0x70228 */
	uint32_t os_reg11;                           /* Address Offset: 0x7022C */
	uint32_t os_reg12;                           /* Address Offset: 0x70230 */
	uint32_t os_reg13;                           /* Address Offset: 0x70234 */
	uint32_t os_reg14;                           /* Address Offset: 0x70238 */
	uint32_t os_reg15;                           /* Address Offset: 0x7023C */
	uint32_t os_reg16;                           /* Address Offset: 0x70240 */
	uint32_t os_reg17;                           /* Address Offset: 0x70244 */
	uint32_t os_reg18;                           /* Address Offset: 0x70248 */
	uint32_t os_reg19;                           /* Address Offset: 0x7024C */
	uint32_t os_reg20;                           /* Address Offset: 0x70250 */
	uint32_t os_reg21;                           /* Address Offset: 0x70254 */
	uint32_t os_reg22;                           /* Address Offset: 0x70258 */
	uint32_t os_reg23;                           /* Address Offset: 0x7025C */
	uint32_t reserved7[(0x80000 - 0x7025C) / 4 - 1];

	uint32_t grf_sys_con[2];                     /* Address Offset: 0x80000 */
	uint32_t reserved8[2];                       /* Address Offset: 0x80008 */
	uint32_t grf_sys_status;                     /* Address Offset: 0x80010 */
	uint32_t reserved9[3];                       /* Address Offset: 0x80014 */
	uint32_t grf_biu_con[2];                     /* Address Offset: 0x80020 */
	uint32_t reserved10[2];                      /* Address Offset: 0x80028 */
	uint32_t grf_biu_status[3];                  /* Address Offset: 0x80030 */
	uint32_t reserved11[17];                     /* Address Offset: 0x8003C */
	uint32_t grf_sys_mem_con[5];                 /* Address Offset: 0x80080 */
	uint32_t reserved12[59];                     /* Address Offset: 0x80094 */
	uint32_t grf_soc_code;                       /* Address Offset: 0x80180 */
	uint32_t reserved13[3];                      /* Address Offset: 0x80184 */
	uint32_t grf_soc_version;                    /* Address Offset: 0x80190 */
	uint32_t reserved14[3];                      /* Address Offset: 0x80194 */
	uint32_t grf_chip_id;                        /* Address Offset: 0x801A0 */
	uint32_t reserved15[3];                      /* Address Offset: 0x801A4 */
	uint32_t grf_chip_version;                   /* Address Offset: 0x801B0 */
	uint32_t reserved16[(0x10000 - 0x81b0) / 4 - 1];

};

check_member(rk3528_grf, sdmmc_con1, 0x60028);
check_member(rk3528_grf, os_reg23, 0x7025C);
check_member(rk3528_grf, grf_chip_version, 0x801B0);

#endif

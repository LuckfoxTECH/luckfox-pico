/*
 * (C) Copyright 2022 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef _ASM_ARCH_GRF_RK3562_H
#define _ASM_ARCH_GRF_RK3562_H

#include <common.h>

struct rk3562_pmu_grf {
        /* pmugrf */
        uint32_t reserved1[(0x0100 - 0x0000) / 4];        /* address offset: 0x0000 */
        uint32_t soc_con[13];                             /* address offset: 0x0100 */
        uint32_t soc_status[1];                           /* address offset: 0x0134 */
        uint32_t reserved2[(0x0180 - 0x0134) / 4 - 1];    /* address offset: 0x0138 */
        uint32_t pvtm_con[1];                             /* address offset: 0x0180 */
        uint32_t reserved3[(0x0200 - 0x0180) / 4 - 1];    /* address offset: 0x0184 */
        uint32_t os_reg[12];                              /* address offset: 0x0200 */
        uint32_t reset_function_status;                   /* address offset: 0x0230 */
        uint32_t reset_function_clr;                      /* address offset: 0x0234 */
        uint32_t reserved4[(0x0380 - 0x0234) / 4 - 1];    /* address offset: 0x0238 */
        uint32_t sig_detect_con;                          /* address offset: 0x0380 */
        uint32_t reserved5[(0x0390 - 0x0380) / 4 - 1];    /* address offset: 0x0384 */
        uint32_t sig_detect_status;                       /* address offset: 0x0390 */
        uint32_t reserved6[(0x03a0 - 0x0390) / 4 - 1];    /* address offset: 0x0394 */
        uint32_t sig_detect_status_clear;                 /* address offset: 0x03a0 */
        uint32_t reserved7[(0x03b0 - 0x03a0) / 4 - 1];    /* address offset: 0x03a4 */
        uint32_t sdmmc_det_counter;                       /* address offset: 0x03b0 */
};

check_member(rk3562_pmu_grf, sdmmc_det_counter, 0x03b0);

struct rk3562_grf {
        /* sysgrf */
        uint32_t reserved1[(0x0400 - 0x0000) / 4];        /* address offset: 0x0000 */
        uint32_t soc_con[7];                              /* address offset: 0x0400 */
        uint32_t reserved2[(0x0430 - 0x0400) / 4 - 7];    /* address offset: 0x041c */
        uint32_t soc_status[3];                           /* address offset: 0x0430 */
        uint32_t reserved3;                               /* address offset: 0x043c */
        uint32_t biu_con[2];                              /* address offset: 0x0440 */
        uint32_t reserved4[(0x0460 - 0x0440) / 4 - 2];    /* address offset: 0x0448 */
        uint32_t ram_con;                                 /* address offset: 0x0460 */
        uint32_t core_ram_con;                            /* address offset: 0x0464 */
        uint32_t reserved5[(0x0500 - 0x0464) / 4 - 1];    /* address offset: 0x0468 */
        uint32_t cpu_con[2];                              /* address offset: 0x0500 */
        uint32_t reserved6[(0x0510 - 0x0500) / 4 - 2];    /* address offset: 0x0508 */
        uint32_t cpu_status[2];                           /* address offset: 0x0510 */
        uint32_t reserved7[(0x0520 - 0x0510) / 4 - 2];    /* address offset: 0x0518 */
        uint32_t vi_con[2];                               /* address offset: 0x0520 */
        uint32_t reserved8[(0x0530 - 0x0520) / 4 - 2];    /* address offset: 0x0528 */
        uint32_t vi_status[1];                            /* address offset: 0x0530 */
        uint32_t reserved9[(0x0570 - 0x0530) / 4 - 1];    /* address offset: 0x0534 */
        uint32_t gpu_con[2];                              /* address offset: 0x0570 */
        uint32_t reserved10[(0x0580 - 0x0570) / 4 - 2];   /* address offset: 0x0578 */
        uint32_t tsadc_con;                               /* address offset: 0x0580 */
        uint32_t reserved11[(0x05d0 - 0x0580) / 4 - 1];   /* address offset: 0x0584 */
        uint32_t vo_con[2];                               /* address offset: 0x05d0 */
        uint32_t reserved12[(0x0600 - 0x05d0) / 4 - 2];   /* address offset: 0x05d8 */
        uint32_t top_pvtpll_con[4];                       /* address offset: 0x0600 */
        uint32_t top_pvtpll_status[2];                    /* address offset: 0x0610 */
        uint32_t reserved13[(0x0620 - 0x0610) / 4 - 2];   /* address offset: 0x0618 */
        uint32_t cpu_pvtpll_con[4];                       /* address offset: 0x0620 */
        uint32_t cpu_pvtpll_status[2];                    /* address offset: 0x0630 */
        uint32_t reserved14[(0x0640 - 0x0630) / 4 - 2];   /* address offset: 0x0638 */
        uint32_t gpu_pvtpll_con[4];                       /* address offset: 0x0640 */
        uint32_t gpu_pvtpll_status[2];                    /* address offset: 0x0650 */
        uint32_t reserved15[(0x0660 - 0x0650) / 4 - 2];   /* address offset: 0x0658 */
        uint32_t npu_pvtpll_con[4];                       /* address offset: 0x0660 */
        uint32_t npu_pvtpll_status[2];                    /* address offset: 0x0670 */
        uint32_t reserved16[(0x0800 - 0x0670) / 4 - 2];   /* address offset: 0x0678 */
        uint32_t chip_id;                                 /* address offset: 0x0800 */
};
check_member(rk3562_grf, chip_id, 0x0800);

#endif

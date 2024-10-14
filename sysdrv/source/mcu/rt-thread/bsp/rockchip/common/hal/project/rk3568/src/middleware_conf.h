/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#ifndef __MIDDLEWARE_CONF_H__
#define __MIDDLEWARE_CONF_H__

#define MASTER_ID   ((uint32_t)1)
#define REMOTE_ID_2 ((uint32_t)2)
#define REMOTE_ID_3 ((uint32_t)3)
#define REMOTE_ID_0 ((uint32_t)0)

/* RPMSG endpoint addr covert */
#define EPT_M2R_ADDR(addr) (addr + VRING_SIZE)   // master to remote covert
#define EPT_R2M_ADDR(addr) (addr - VRING_SIZE)   // remote to master covert

/* RPMSG ID Define */
/* RPMSG master(cpu1) to remote(cpu2) endpoint index define */
#define EPT_M1R2_INIT 0UL

/* RPMSG master(cpu1) to remote(cpu3) endpoint index define */
#define EPT_M1R3_INIT 0UL

/* RPMSG master(cpu1) to remote(cpu0) endpoint index define */
#define EPT_M1R0_INIT 0UL

/* RPMSG API Functions */
void rpmsg_init(void);
#ifdef PRIMARY_CPU
struct rpmsg_lite_instance *rpmsg_master_get_instance(uint32_t master_id, uint32_t remote_id);
#else
struct rpmsg_lite_instance *rpmsg_remote_get_instance(uint32_t master_id, uint32_t remote_id);
#endif

#endif

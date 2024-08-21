/*
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef RPMSG_PLATFORM_H_
#define RPMSG_PLATFORM_H_

#include <stdint.h>

/* RPMSG MU channel index */
#define RPMSG_MU_CHANNEL (1)

/*
 * Linux requires the ALIGN to 0x1000(4KB) instead of 0x80
 */
#ifndef VRING_ALIGN
#define VRING_ALIGN (0x1000U)
#endif

/* contains pool of descriptors and two circular buffers */
#ifndef VRING_SIZE
#define VRING_SIZE (0x8000UL)
#endif

/* size of shared memory + 2*VRING size */
#define RL_VRING_OVERHEAD (2UL * VRING_SIZE)

/* VQ_ID in 8QM is defined as follows:
 *   com_id:   [3:3] communication ID, used to identify the MU instance.
 *   vring_id: [2:1] vring ID, used to identify the vring.
 *   q_id:     [0:0] queue ID, used to identify the tvq or rvq.
 *   com_id + vring_id = link_id
 */

#define RL_GET_VQ_ID(link_id, queue_id) (((queue_id)&0x1U) | (((link_id) << 1U) & 0xFFFFFFFEU))
#define RL_GET_LINK_ID(vq_id)           ((vq_id) >> 1U)
#define RL_GET_COM_ID(vq_id)            ((vq_id) >> 3U)
#define RL_GET_Q_ID(vq_id)              ((vq_id)&0x1U)

#define RL_GEN_LINK_ID(com_id, vring_id) (((com_id) << 2U) | (vring_id))
#define RL_GEN_MU_MSG(vq_id)             (uint32_t)(((vq_id)&0x7U) << 16U) /* com_id is discarded in msg */

#define RL_PLATFORM_IMX8QM_M4_A_COM_ID  (0U)
#define RL_PLATFORM_IMX8QM_M4_M4_COM_ID (1U)

#define RL_PLATFORM_IMX8QM_M4_SRTM_VRING_ID (0U)
#define RL_PLATFORM_IMX8QM_M4_USER_VRING_ID (1U)

#define RL_PLATFORM_HIGHEST_LINK_ID RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_M4_COM_ID, RL_PLATFORM_IMX8QM_M4_USER_VRING_ID)

#define RL_PLATFORM_IMX8QM_M4_A_SRTM_LINK_ID \
    RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_A_COM_ID, RL_PLATFORM_IMX8QM_M4_SRTM_VRING_ID)
#define RL_PLATFORM_IMX8QM_M4_A_USER_LINK_ID \
    RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_A_COM_ID, RL_PLATFORM_IMX8QM_M4_USER_VRING_ID)
#define RL_PLATFORM_IMX8QM_M4_M4_SRTM_LINK_ID \
    RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_M4_COM_ID, RL_PLATFORM_IMX8QM_M4_SRTM_VRING_ID)
#define RL_PLATFORM_IMX8QM_M4_M4_USER_LINK_ID \
    RL_GEN_LINK_ID(RL_PLATFORM_IMX8QM_M4_M4_COM_ID, RL_PLATFORM_IMX8QM_M4_USER_VRING_ID)

/* platform interrupt related functions */
int32_t platform_init_interrupt(uint32_t vector_id, void *isr_data);
int32_t platform_deinit_interrupt(uint32_t vector_id);
int32_t platform_interrupt_enable(uint32_t vector_id);
int32_t platform_interrupt_disable(uint32_t vector_id);
int32_t platform_in_isr(void);
void platform_notify(uint32_t vector_id);

/* platform low-level time-delay (busy loop) */
void platform_time_delay(uint32_t num_msec);

/* platform memory functions */
void platform_map_mem_region(uint32_t vrt_addr, uint32_t phy_addr, uint32_t size, uint32_t flags);
void platform_cache_all_flush_invalidate(void);
void platform_cache_disable(void);
uint32_t platform_vatopa(void *addr);
void *platform_patova(uint32_t addr);

/* platform init/deinit */
int32_t platform_init(void);
int32_t platform_deinit(void);

#if defined(MIMX8QM_CM4_CORE0)
int32_t LSIO_MU5_INT_B_IRQHandler(void);
int32_t LSIO_MU7_INT_A_IRQHandler(void);
#elif defined(MIMX8QM_CM4_CORE1)
int32_t LSIO_MU6_INT_B_IRQHandler(void);
int32_t LSIO_MU7_INT_B_IRQHandler(void);
#endif

#endif /* RPMSG_PLATFORM_H_ */

/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#ifndef RPMSG_PLATFORM_H_
#define RPMSG_PLATFORM_H_

#include <stdint.h>

/*
 * Linux requires the ALIGN to 0x1000(4KB)
 */
#ifndef VRING_ALIGN
#define VRING_ALIGN (0x1000U)
#endif

/* contains pool of descriptors and two circular buffers */
#ifndef VRING_SIZE
#define VRING_SIZE (0x8000UL)
#endif

#ifndef RL_NS_NAME_SIZE
#define RL_NS_NAME_SIZE (32)
#endif

/* size of shared memory + 2*VRING size */
#define RL_VRING_OVERHEAD (2UL * VRING_SIZE)

#define RL_GET_VQ_ID(link_id, queue_id) (((queue_id)&0x1U) | (((link_id) << 1U) & 0xFFFFFFFEU))
#define RL_GET_LINK_ID(id)              (((id)&0xFFFFFFFEU) >> 1U)
#define RL_GET_Q_ID(id)                 ((id)&0x1U)
/* link_id: 4 bit master cpu_id and 4 bit remote_id */
#define RL_GET_R_CPU_ID(id)             (RL_GET_LINK_ID(id) & 0xFU)
#define RL_GET_M_CPU_ID(id)             ((RL_GET_LINK_ID(id) & 0xF0U) >> 4U)
#define RL_PLATFORM_SET_LINK_ID(_M, _R) (((_M << 4U) & 0xF0U) | (_R & 0xFU))

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

#endif /* RPMSG_PLATFORM_H_ */

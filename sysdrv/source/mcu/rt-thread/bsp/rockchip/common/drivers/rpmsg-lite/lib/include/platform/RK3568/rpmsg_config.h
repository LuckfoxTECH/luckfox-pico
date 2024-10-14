/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#ifndef RPMSG_CONFIG_H_
#define RPMSG_CONFIG_H_

#define RL_BUFFER_PAYLOAD_SIZE (496U)
#define RL_BUFFER_COUNT (64U)
/* endpoint size is formed by payload and struct rpmsg_std_hdr */
#define RL_EPT_SIZE (RL_BUFFER_PAYLOAD_SIZE + 16UL)

#define RL_MAX_INSTANCE_NUM (12U)
#define RL_PLATFORM_HIGHEST_LINK_ID     (0xFFU)

/* TODO: Select the IPI method according to the actual scene */
//#define RL_PLATFORM_USING_SOFTIRQ
#define RL_PLATFORM_USING_MBOX

#ifdef RL_PLATFORM_USING_SOFTIRQ
#define RL_PLATFORM_IRQ_BASE            RPMSG_01_IRQn
#endif

#ifdef RL_PLATFORM_USING_MBOX
#define RL_PLATFORM_B2A_IRQ_BASE        MBOX0_CH0_B2A_IRQn
#define RL_PLATFORM_A2B_IRQ_BASE        MBOX0_CH0_A2B_IRQn
#define RL_PLATFORM_M_IRQ(n)            MBOX0_CH##n##_B2A_IRQn
#define RL_PLATFORM_R_IRQ(n)            MBOX0_CH##n##_A2B_IRQn
#define RL_RPMSG_MAGIC                  (0x524D5347U)
#endif

/*
 * env bm/os isr count
 * 4bit for master and 4bit for remote. each link_id has 2 virtqueue.
 * TODO: Modify here to reduce memory usage
 */
#define ISR_COUNT (0x1E0U)

#endif /* RPMSG_CONFIG_H_ */

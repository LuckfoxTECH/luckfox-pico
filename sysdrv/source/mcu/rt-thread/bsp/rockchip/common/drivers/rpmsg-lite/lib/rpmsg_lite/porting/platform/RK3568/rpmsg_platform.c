/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */
#include <stdio.h>
#include <string.h>

#include "rpmsg_platform.h"
#include "rpmsg_env.h"

#include <rthw.h>
#include <rtthread.h>
#include "interrupt.h"
#include "hal_base.h"

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
#error "This RPMsg-Lite port requires RL_USE_ENVIRONMENT_CONTEXT set to 0"
#endif

static int32_t isr_counter = 0;
static int32_t disable_counter = 0;
static int32_t first_notify = 0;
static void *platform_lock;
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
static LOCK_STATIC_CONTEXT platform_lock_static_ctxt;
#endif

#ifdef RL_PLATFORM_USING_SOFTIRQ
/* RK3568 uses 12 consecutive interrupts, refer to soc.h */
static inline uint32_t rl_set_irq_mr(uint32_t vector_id)
{
    uint32_t mr_irqn = RL_PLATFORM_IRQ_BASE;
    uint32_t m_cpuid = RL_GET_M_CPU_ID(vector_id);
    uint32_t r_cpuid = RL_GET_R_CPU_ID(vector_id);

    if (m_cpuid < r_cpuid)
        mr_irqn += 3U * m_cpuid + r_cpuid - 1U;
    else if (m_cpuid > r_cpuid)
        mr_irqn += 3U * m_cpuid + r_cpuid;
    else
        rt_kprintf("rpmsg: set irq mr error!\n");

    return mr_irqn;
}

static inline uint32_t rl_set_irq_rm(uint32_t vector_id)
{
    uint32_t rm_irqn = RL_PLATFORM_IRQ_BASE;
    uint32_t m_cpuid = RL_GET_M_CPU_ID(vector_id);
    uint32_t r_cpuid = RL_GET_R_CPU_ID(vector_id);

    if (m_cpuid < r_cpuid)
        rm_irqn += 3U * r_cpuid + m_cpuid;
    else if (m_cpuid > r_cpuid)
        rm_irqn += 3U * r_cpuid + m_cpuid - 1U;
    else
        rt_kprintf("rpmsg: set irq rm error!\n");

    return rm_irqn;
}

static inline uint32_t rl_get_irq_rm(int irqn)
{
    uint32_t link_id, m_cpuid, r_cpuid;

    r_cpuid = (uint32_t)((irqn - RL_PLATFORM_IRQ_BASE) / 3);
    m_cpuid = (uint32_t)((irqn - RL_PLATFORM_IRQ_BASE) % 3) + 1U;
    if (m_cpuid <= r_cpuid)
        m_cpuid -= 1U;

    link_id = RL_PLATFORM_SET_LINK_ID(m_cpuid, r_cpuid);

    return link_id;
}

static void rpmsg_master_isr(int irqn, void *param)
{
    uint32_t link_id;

    link_id = rl_get_irq_rm(irqn);
    env_isr(RL_GET_VQ_ID(link_id, 0));
    rt_hw_interrupt_ack(irqn);
}

static void rpmsg_remote_isr(int irqn, void *param)
{
    uint32_t link_id = (uint32_t)param;

    if (first_notify == 0)
    {
        env_isr(RL_GET_VQ_ID(link_id, 0));
        first_notify++;
    }
    else
    {
        env_isr(RL_GET_VQ_ID(link_id, 1));
    }
    rt_hw_interrupt_ack(irqn);
}
#endif

#ifdef RL_PLATFORM_USING_MBOX
/* master core uses RL_MBOX_A2B and remote core uses RL_MBOX_B2A */
#define RL_MBOX_B2A 0
#define RL_MBOX_A2B 1

static struct MBOX_REG *rl_pMBox = MBOX0;
static int32_t register_count = 0;

static void rpmsg_mbox_isr(int irqn, void *param)
{
    HAL_MBOX_IrqHandler(irqn, rl_pMBox);
    rt_hw_interrupt_ack(irqn);
}

static void rpmsg_master_cb(struct MBOX_CMD_DAT *msg, void *args)
{
    uint32_t link_id;
    struct MBOX_CMD_DAT rx_msg = *msg;

    if (rx_msg.DATA != RL_RPMSG_MAGIC)
        rt_kprintf("rpmsg master: mailbox data error!\n");
    link_id = rx_msg.CMD & 0xFFU;
    env_isr(RL_GET_VQ_ID(link_id, 0));
}

static void rpmsg_remote_cb(struct MBOX_CMD_DAT *msg, void *args)
{
    uint32_t link_id;
    struct MBOX_CMD_DAT rx_msg = *msg;

    if (rx_msg.DATA != RL_RPMSG_MAGIC)
        rt_kprintf("rpmsg master: mailbox data error!\n");
    link_id = rx_msg.CMD & 0xFFU;

    if (first_notify == 0)
    {
        env_isr(RL_GET_VQ_ID(link_id, 0));
        first_notify++;
    }
    else
    {
        env_isr(RL_GET_VQ_ID(link_id, 1));
    }
}

static inline uint32_t rl_mbox_m_irq(uint32_t cpu_id)
{
    uint32_t irqn;

    irqn = cpu_id + RL_PLATFORM_B2A_IRQ_BASE;

    return irqn;
}

static inline uint32_t rl_mbox_r_irq(uint32_t cpu_id)
{
    uint32_t irqn;

    irqn = cpu_id + RL_PLATFORM_A2B_IRQ_BASE;

    return irqn;
}

static struct MBOX_CLIENT mbox_clm[MBOX_CHAN_CNT] =
{
    { "mbox-clm0", RL_PLATFORM_M_IRQ(0), rpmsg_master_cb, (void *)MBOX_CH_0 },
    { "mbox-clm1", RL_PLATFORM_M_IRQ(1), rpmsg_master_cb, (void *)MBOX_CH_1 },
    { "mbox-clm2", RL_PLATFORM_M_IRQ(2), rpmsg_master_cb, (void *)MBOX_CH_2 },
    { "mbox-clm3", RL_PLATFORM_M_IRQ(3), rpmsg_master_cb, (void *)MBOX_CH_3 },
};

static struct MBOX_CLIENT mbox_clr[MBOX_CHAN_CNT] =
{
    { "mbox-clr0", RL_PLATFORM_R_IRQ(0), rpmsg_remote_cb, (void *)MBOX_CH_0 },
    { "mbox-clr1", RL_PLATFORM_R_IRQ(1), rpmsg_remote_cb, (void *)MBOX_CH_1 },
    { "mbox-clr2", RL_PLATFORM_R_IRQ(2), rpmsg_remote_cb, (void *)MBOX_CH_2 },
    { "mbox-clr3", RL_PLATFORM_R_IRQ(3), rpmsg_remote_cb, (void *)MBOX_CH_3 },
};
#endif

static void platform_global_isr_disable(void)
{
    __asm volatile("cpsid i");
}

static void platform_global_isr_enable(void)
{
    __asm volatile("cpsie i");
}

int32_t platform_init_interrupt(uint32_t vector_id, void *isr_data)
{
    uint32_t cpu_id, link_id;
#ifdef RL_PLATFORM_USING_MBOX
    int ret = 0;
    struct MBOX_CLIENT *mbox_cl[MBOX_CHAN_CNT];
#endif

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    link_id = RL_GET_LINK_ID(vector_id);
    /* Register ISR to environment layer */
    env_register_isr(vector_id, isr_data);

    env_lock_mutex(platform_lock);

    RL_ASSERT(0 <= isr_counter);
    if (isr_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
#ifdef RL_PLATFORM_USING_SOFTIRQ
        if (cpu_id == RL_GET_M_CPU_ID(vector_id))
        {
            rt_hw_interrupt_set_route(rl_set_irq_rm(vector_id), CPU_GET_AFFINITY(cpu_id, 0));
            rt_hw_interrupt_install(rl_set_irq_rm(vector_id), rpmsg_master_isr, RT_NULL, "rpmsg-lite");
        }
        else
        {
            rt_hw_interrupt_set_route(rl_set_irq_mr(vector_id), CPU_GET_AFFINITY(cpu_id, 0));
            rt_hw_interrupt_install(rl_set_irq_mr(vector_id), rpmsg_remote_isr, (void *)link_id, "rpmsg-lite");
        }
#endif
#ifdef RL_PLATFORM_USING_MBOX
        if (cpu_id == RL_GET_M_CPU_ID(vector_id))
        {
            rt_hw_interrupt_set_route(rl_mbox_m_irq(RL_GET_R_CPU_ID(vector_id)), CPU_GET_AFFINITY(cpu_id, 0));
            rt_hw_interrupt_install(rl_mbox_m_irq(RL_GET_R_CPU_ID(vector_id)), rpmsg_mbox_isr, RT_NULL, "rpmsg-lite");
        }
        else
        {
            rt_hw_interrupt_set_route(rl_mbox_r_irq(cpu_id), CPU_GET_AFFINITY(cpu_id, 0));
            rt_hw_interrupt_install(rl_mbox_r_irq(cpu_id), rpmsg_mbox_isr, RT_NULL, "rpmsg-lite");
        }
        if (register_count % 2 == 0)
        {
            if (cpu_id == RL_GET_M_CPU_ID(vector_id))
            {
                HAL_MBOX_Init(rl_pMBox, RL_MBOX_A2B);
                mbox_cl[RL_GET_R_CPU_ID(vector_id)] = &mbox_clm[RL_GET_R_CPU_ID(vector_id)];
                ret = HAL_MBOX_RegisterClient(rl_pMBox, RL_GET_R_CPU_ID(vector_id), mbox_cl[RL_GET_R_CPU_ID(vector_id)]);
                if (ret)
                {
                    rt_kprintf("mbox master client register failed, ret=%d\n", ret);
                }
            }
            else
            {
                HAL_MBOX_Init(rl_pMBox, RL_MBOX_B2A);
                mbox_cl[cpu_id] = &mbox_clr[cpu_id];
                ret = HAL_MBOX_RegisterClient(rl_pMBox, cpu_id, mbox_cl[cpu_id]);
                if (ret)
                {
                    rt_kprintf("mbox remote client register failed, ret=%d\n", ret);
                }
            }
        }
        register_count++;
#endif
    }
    isr_counter++;

    env_unlock_mutex(platform_lock);

    return 0;
}

int32_t platform_deinit_interrupt(uint32_t vector_id)
{
    env_lock_mutex(platform_lock);

    RL_ASSERT(0 < isr_counter);
    isr_counter--;
    if (isr_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
    }

    /* Unregister ISR from environment layer */
    env_unregister_isr(vector_id);

    env_unlock_mutex(platform_lock);

    return 0;
}

void platform_notify(uint32_t vector_id)
{
    uint32_t cpu_id;
#ifdef RL_PLATFORM_USING_MBOX
    uint32_t link_id;
    struct MBOX_CMD_DAT tx_msg;

    link_id = RL_GET_LINK_ID(vector_id);
    tx_msg.CMD = link_id & 0xFFU;
    tx_msg.DATA = RL_RPMSG_MAGIC;
#endif

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    env_lock_mutex(platform_lock);

#ifdef RL_PLATFORM_USING_SOFTIRQ
    if (cpu_id == RL_GET_M_CPU_ID(vector_id))
        rt_hw_interrupt_set_pending(rl_set_irq_mr(vector_id));
    else
        rt_hw_interrupt_set_pending(rl_set_irq_rm(vector_id));
#endif
#ifdef RL_PLATFORM_USING_MBOX
    HAL_MBOX_SendMsg(rl_pMBox, RL_GET_R_CPU_ID(vector_id), &tx_msg);
#endif

    env_unlock_mutex(platform_lock);
}

/**
 * platform_time_delay
 *
 * @param num_msec Delay time in ms.
 *
 */
void platform_time_delay(uint32_t num_msec)
{
    rt_thread_mdelay(num_msec);
}

/**
 * platform_in_isr
 *
 * Return whether CPU is processing IRQ
 *
 * @return True for IRQ, false otherwise.
 *
 */
int32_t platform_in_isr(void)
{
    return ((__get_mode() != 0x10) ? 1 : 0);
}

/**
 * platform_interrupt_enable
 *
 * Enable peripheral-related interrupt
 *
 * @param vector_id Virtual vector ID that needs to be converted to IRQ number
 *
 * @return vector_id Return value is never checked.
 *
 */
int32_t platform_interrupt_enable(uint32_t vector_id)
{
    uint32_t cpu_id;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    RL_ASSERT(0 < disable_counter);

    platform_global_isr_disable();
    disable_counter--;
    if (disable_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
#ifdef RL_PLATFORM_USING_SOFTIRQ
        if (cpu_id == RL_GET_M_CPU_ID(vector_id))
            rt_hw_interrupt_umask(rl_set_irq_rm(vector_id));
        else
            rt_hw_interrupt_umask(rl_set_irq_mr(vector_id));
#endif
#ifdef RL_PLATFORM_USING_MBOX
        if (cpu_id == RL_GET_M_CPU_ID(vector_id))
            rt_hw_interrupt_umask(rl_mbox_m_irq(RL_GET_R_CPU_ID(vector_id)));
        else
            rt_hw_interrupt_umask(rl_mbox_r_irq(cpu_id));
#endif
    }
    platform_global_isr_enable();
    return ((int32_t)vector_id);
}

/**
 * platform_interrupt_disable
 *
 * Disable peripheral-related interrupt.
 *
 * @param vector_id Virtual vector ID that needs to be converted to IRQ number
 *
 * @return vector_id Return value is never checked.
 *
 */
int32_t platform_interrupt_disable(uint32_t vector_id)
{
    uint32_t cpu_id;

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    RL_ASSERT(0 <= disable_counter);

    platform_global_isr_disable();
    if (disable_counter < 2 * RL_MAX_INSTANCE_NUM)
    {
#ifdef RL_PLATFORM_USING_SOFTIRQ
        if (cpu_id == RL_GET_M_CPU_ID(vector_id))
            rt_hw_interrupt_mask(rl_set_irq_rm(vector_id));
        else
            rt_hw_interrupt_mask(rl_set_irq_mr(vector_id));
#endif
#ifdef RL_PLATFORM_USING_MBOX
        if (cpu_id == RL_GET_M_CPU_ID(vector_id))
            rt_hw_interrupt_mask(rl_mbox_m_irq(RL_GET_R_CPU_ID(vector_id)));
        else
            rt_hw_interrupt_mask(rl_mbox_r_irq(cpu_id));
#endif
    }
    disable_counter++;
    platform_global_isr_enable();
    return ((int32_t)vector_id);
}

/**
 * platform_map_mem_region
 *
 * Dummy implementation
 *
 */
void platform_map_mem_region(uint32_t vrt_addr, uint32_t phy_addr, uint32_t size, uint32_t flags)
{
}

/**
 * platform_cache_all_flush_invalidate
 *
 * Dummy implementation
 *
 */
void platform_cache_all_flush_invalidate(void)
{
}

/**
 * platform_cache_disable
 *
 * Dummy implementation
 *
 */
void platform_cache_disable(void)
{
}

/**
 * platform_vatopa
 *
 * Dummy implementation
 *
 */
uint32_t platform_vatopa(void *addr)
{
    return ((uint32_t)(char *)addr);
}

/**
 * platform_patova
 *
 * Dummy implementation
 *
 */
void *platform_patova(uint32_t addr)
{
    return ((void *)(char *)addr);
}

/**
 * platform_init
 *
 * platform/environment init
 */
int32_t platform_init(void)
{
    /* Create lock used in multi-instanced RPMsg */
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    if (0 != env_create_mutex(&platform_lock, 1, &platform_lock_static_ctxt))
#else
    if (0 != env_create_mutex(&platform_lock, 1))
#endif
    {
        return -1;
    }

    return 0;
}

/**
 * platform_deinit
 *
 * platform/environment deinit process
 */
int32_t platform_deinit(void)
{
    /* Delete lock used in multi-instanced RPMsg */
    env_delete_mutex(platform_lock);
    platform_lock = ((void *)0);
    return 0;
}

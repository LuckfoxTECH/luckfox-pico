/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdio.h>
#include <string.h>

#include "rpmsg_platform.h"
#include "rpmsg_env.h"

#include "fsl_device_registers.h"
#include "fsl_mu.h"

#if defined(RL_USE_MCMGR_IPC_ISR_HANDLER) && (RL_USE_MCMGR_IPC_ISR_HANDLER == 1)
#include "mcmgr.h"
#endif

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
#error "This RPMsg-Lite port requires RL_USE_ENVIRONMENT_CONTEXT set to 0"
#endif

static int32_t isr_counter     = 0;
static int32_t disable_counter = 0;
static void *platform_lock;
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
static LOCK_STATIC_CONTEXT platform_lock_static_ctxt;
#endif

#if defined(RL_USE_MCMGR_IPC_ISR_HANDLER) && (RL_USE_MCMGR_IPC_ISR_HANDLER == 1)
static void mcmgr_event_handler(uint16_t vring_idx, void *context)
{
    env_isr((uint32_t)vring_idx);
}
#else
static void mu_isr(MU_Type *base)
{
    uint32_t flags;
    flags = MU_GetStatusFlags(base);
    if (((uint32_t)kMU_GenInt0Flag & flags) != 0UL)
    {
        MU_ClearStatusFlags(base, (uint32_t)kMU_GenInt0Flag);
        env_isr(0);
    }
    if (((uint32_t)kMU_GenInt1Flag & flags) != 0UL)
    {
        MU_ClearStatusFlags(base, (uint32_t)kMU_GenInt1Flag);
        env_isr(1);
    }
}

#if defined(FSL_FEATURE_MU_SIDE_A)
int32_t MUA_IRQHandler(void)
{
    mu_isr(MUA);
    /* ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
     * exception return operation might vector to incorrect interrupt.
     * For Cortex-M7, if core speed much faster than peripheral register write speed,
     * the peripheral interrupt flags may be still set after exiting ISR, this results to
     * the same error similar with errata 83869 */
#if (defined __CORTEX_M) && ((__CORTEX_M == 4U) || (__CORTEX_M == 7U))
    __DSB();
#endif
    return 0;
}
#elif defined(FSL_FEATURE_MU_SIDE_B)
int32_t MUB_IRQHandler(void)
{
    mu_isr(MUB);
    /* ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
     * exception return operation might vector to incorrect interrupt.
     * For Cortex-M7, if core speed much faster than peripheral register write speed,
     * the peripheral interrupt flags may be still set after exiting ISR, this results to
     * the same error similar with errata 83869 */
#if (defined __CORTEX_M) && ((__CORTEX_M == 4U) || (__CORTEX_M == 7U))
    __DSB();
#endif
    return 0;
}
#endif
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
    /* Register ISR to environment layer */
    env_register_isr(vector_id, isr_data);

    env_lock_mutex(platform_lock);

    RL_ASSERT(0 <= isr_counter);
    if (isr_counter < 2)
    {
#if defined(FSL_FEATURE_MU_SIDE_A)
        MU_EnableInterrupts(MUA, 1UL << (31UL - vector_id));
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_EnableInterrupts(MUB, 1UL << (31UL - vector_id));
#endif
    }
    isr_counter++;

    env_unlock_mutex(platform_lock);

    return 0;
}

int32_t platform_deinit_interrupt(uint32_t vector_id)
{
    /* Prepare the MU Hardware */
    env_lock_mutex(platform_lock);

    RL_ASSERT(0 < isr_counter);
    isr_counter--;
    if (isr_counter < 2)
    {
#if defined(FSL_FEATURE_MU_SIDE_A)
        MU_DisableInterrupts(MUA, 1UL << (31UL - vector_id));
#elif defined(FSL_FEATURE_MU_SIDE_B)
        MU_DisableInterrupts(MUB, 1UL << (31UL - vector_id));
#endif
    }

    /* Unregister ISR from environment layer */
    env_unregister_isr(vector_id);

    env_unlock_mutex(platform_lock);

    return 0;
}

void platform_notify(uint32_t vector_id)
{
    env_lock_mutex(platform_lock);
#if defined(RL_USE_MCMGR_IPC_ISR_HANDLER) && (RL_USE_MCMGR_IPC_ISR_HANDLER == 1)
    (void)MCMGR_TriggerEventForce(kMCMGR_RemoteRPMsgEvent, (uint16_t)RL_GET_Q_ID(vector_id));
#else
/* Write directly into the MU Control Register to trigger General Purpose Interrupt Request (GIR).
   No need to wait until the previous interrupt is processed because the same value
   of the virtqueue ID is used for GIR mask when triggering the ISR for the receiver side.
   The whole queue of received buffers for associated virtqueue is then handled in the ISR
   on the receiver side. */
#if defined(FSL_FEATURE_MU_SIDE_A)
    (void)MU_TriggerInterrupts(MUA, 1UL << (19UL - RL_GET_Q_ID(vector_id)));
#elif defined(FSL_FEATURE_MU_SIDE_B)
    (void)MU_TriggerInterrupts(MUB, 1UL << (19UL - RL_GET_Q_ID(vector_id)));
#endif
#endif
    env_unlock_mutex(platform_lock);
}

/**
 * platform_time_delay
 *
 * @param num_msec Delay time in ms.
 *
 * This is not an accurate delay, it ensures at least num_msec passed when return.
 */
void platform_time_delay(uint32_t num_msec)
{
    uint32_t loop;

    /* Recalculate the CPU frequency - not implemented in system_MIMXRT1176_cm[4,7].c */
    /* SystemCoreClockUpdate(); */

    /* Calculate the CPU loops to delay, each loop has 3 cycles */
    loop = SystemCoreClock / 3U / 1000U * num_msec;

    /* There's some difference among toolchains, 3 or 4 cycles each loop */
    while (loop > 0U)
    {
        __NOP();
        loop--;
    }
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
    return (((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0UL) ? 1 : 0);
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
    RL_ASSERT(0 < disable_counter);

    platform_global_isr_disable();
    disable_counter--;

    if (disable_counter == 0)
    {
#if defined(FSL_FEATURE_MU_SIDE_A)
        NVIC_EnableIRQ(MUA_IRQn);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        NVIC_EnableIRQ(MUB_IRQn);
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
    RL_ASSERT(0 <= disable_counter);

    platform_global_isr_disable();
    /* virtqueues use the same NVIC vector
       if counter is set - the interrupts are disabled */
    if (disable_counter == 0)
    {
#if defined(FSL_FEATURE_MU_SIDE_A)
        NVIC_DisableIRQ(MUA_IRQn);
        NVIC_SetPriority(MUA_IRQn, 2);
#elif defined(FSL_FEATURE_MU_SIDE_B)
        NVIC_DisableIRQ(MUB_IRQn);
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
    /* The MU peripheral driver is not initialized here because it covers also
    the secondary core booting controls and it needs to be initialized earlier
    in the application code */

#if defined(RL_USE_MCMGR_IPC_ISR_HANDLER) && (RL_USE_MCMGR_IPC_ISR_HANDLER == 1)
    mcmgr_status_t retval = kStatus_MCMGR_Error;
    retval                = MCMGR_RegisterEvent(kMCMGR_RemoteRPMsgEvent, mcmgr_event_handler, ((void *)0));
    if (kStatus_MCMGR_Success != retval)
    {
        return -1;
    }
#endif

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

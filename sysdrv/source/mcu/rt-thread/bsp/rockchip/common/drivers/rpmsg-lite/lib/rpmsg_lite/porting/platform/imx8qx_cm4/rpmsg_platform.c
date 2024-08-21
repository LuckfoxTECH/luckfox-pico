/*
 * Copyright 2017-2021 NXP
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

#include "fsl_irqsteer.h"

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
#error "This RPMsg-Lite port requires RL_USE_ENVIRONMENT_CONTEXT set to 0"
#endif

#define APP_MU_IRQ_PRIORITY (3U)

#define APP_M4_MU           LSIO__MU5_B
#define APP_M4_MU_IRQn      LSIO_MU5_INT_B_IRQn
#define APP_M4_MU_NVIC_IRQn IRQSTEER_3_IRQn

/* NVIC IRQn that correspond to the LSIO MU IRQn is obtained with the following
 * calculation:
 *
 * NVIC_IRQn = IRQSTEER_0_IRQn + (LSIO_MU_IRQn - FSL_FEATURE_IRQSTEER_IRQ_START_INDEX) / 64
 *
 * LSIO_MU_IRQn min = LSIO_MU0_INT_IRQn = 259
 * LSIO_MU_IRQn max = LSIO_MU13_INT_B_IRQn = 291
 *
 * With all the LSIO MUs, the NVIC_IRQn = 35, that corresponds to IRQSTEER_3_IRQn
 */

static int32_t isr_counter     = 0;
static int32_t disable_counter = 0;
static void *platform_lock;
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
static LOCK_STATIC_CONTEXT platform_lock_static_ctxt;
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

    /* Prepare the MU Hardware, enable channel 1 interrupt */
    env_lock_mutex(platform_lock);

    RL_ASSERT(0 <= isr_counter);
    if (isr_counter == 0)
    {
        MU_EnableInterrupts(APP_M4_MU, (1UL << 27U) >> RPMSG_MU_CHANNEL);
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
    if (isr_counter == 0)
    {
        MU_DisableInterrupts(APP_M4_MU, (1UL << 27U) >> RPMSG_MU_CHANNEL);
    }

    /* Unregister ISR from environment layer */
    env_unregister_isr(vector_id);

    env_unlock_mutex(platform_lock);

    return 0;
}

void platform_notify(uint32_t vector_id)
{
    /* As Linux suggests, use MU->Data Channel 1 as communication channel */
    uint32_t msg = (uint32_t)(vector_id << 16);

    env_lock_mutex(platform_lock);
    MU_SendMsg(APP_M4_MU, RPMSG_MU_CHANNEL, msg);
    env_unlock_mutex(platform_lock);
}

/*
 * MU Interrrupt RPMsg handler
 */
int32_t LSIO_MU5_INT_B_IRQHandler(void)
{
    uint32_t channel;

    if ((((1UL << 27U) >> RPMSG_MU_CHANNEL) & MU_GetStatusFlags(APP_M4_MU)) != 0UL)
    {
        channel = MU_ReceiveMsgNonBlocking(APP_M4_MU, RPMSG_MU_CHANNEL); // Read message from RX register.
        env_isr((uint32_t)(channel >> 16));
    }

    return 0;
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

    /* Recalculate the CPU frequency */
    SystemCoreClockUpdate();

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
        NVIC_EnableIRQ(APP_M4_MU_NVIC_IRQn);
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
        NVIC_DisableIRQ(APP_M4_MU_NVIC_IRQn);
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
    /*
     * Prepare for the MU Interrupt
     *  MU must be initialized before rpmsg init is called
     */
    MU_Init(APP_M4_MU);
    NVIC_SetPriority(APP_M4_MU_NVIC_IRQn, APP_MU_IRQ_PRIORITY);
    IRQSTEER_EnableInterrupt(IRQSTEER, APP_M4_MU_IRQn);

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
    MU_Deinit(APP_M4_MU);
    IRQSTEER_DisableInterrupt(IRQSTEER, APP_M4_MU_IRQn);

    /* Delete lock used in multi-instanced RPMsg */
    env_delete_mutex(platform_lock);
    platform_lock = ((void *)0);
    return 0;
}

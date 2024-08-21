/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-2-1       Tony.xie     first implementation
 *
 */

#include <rthw.h>
#include <rtthread.h>
#include "drv_pm.h"

#ifdef RT_USING_PM

#ifdef RT_USING_UART
#include "drv_uart.h"
#endif
#ifdef RT_USING_COMMON_TEST_CPU_USAGE
#include "../tests/pm/pm_test.h"
#endif
#ifdef RT_USING_SNOR
#include <drv_snor.h>
#endif
#if defined(RT_USING_QPIPSRAM)
#include <drv_psram.h>
#endif

struct pm_mode_ctrl
{
    struct pm_mode_dvfs *pm_dvfs;
    void (*pm_func)(uint32_t new_mode);
    uint8_t dvfs_en;
    uint8_t mode;
    uint8_t has_enter_sleep;
};

static struct pm_mode_ctrl pm_ctrl;

#ifdef RT_USING_PM_TICK_DELAY
static uint64_t pm_ref_timer_count;
#endif

void pm_runtime_request(ePM_RUNTIME_ID runTimeId)
{
#ifdef RT_USING_PM_RUNTIME
    rt_base_t level;
    level = rt_hw_interrupt_disable();
    HAL_PM_RuntimeRequest(runTimeId);
    rt_hw_interrupt_enable(level);
#endif
}

void pm_runtime_release(ePM_RUNTIME_ID runTimeId)
{
#ifdef RT_USING_PM_RUNTIME
    rt_base_t level;
    level = rt_hw_interrupt_disable();
    HAL_PM_RuntimeRelease(runTimeId);
    rt_hw_interrupt_enable(level);
#endif
}

rt_uint64_t rk_pm_tick_to_timer(rt_uint32_t tick)
{
    return HAL_DivU64(((rt_uint64_t)tick * PLL_INPUT_OSC_RATE), RT_TICK_PER_SECOND);
}

rt_uint32_t rk_pm_timer_to_tick(rt_uint64_t timer_count)
{
    return (rt_uint32_t)HAL_DivU64((rt_uint64_t)(timer_count * RT_TICK_PER_SECOND), PLL_INPUT_OSC_RATE);
}

#ifdef RT_USING_PM
#ifdef RT_USING_PM_DVFS
static void pm_mode_list_dvfs(struct rt_pm *pm, uint32_t run_flag)
{
    struct pm_mode_dvfs *pm_dvfs =  pm_ctrl.pm_dvfs;
    uint8_t tbl_idx;
    int ret;

    if (pm->current_mode == pm_ctrl.mode || !pm_ctrl.dvfs_en)
        return;

    DVFS_INFO("%s: mode(%x,%x)\n", __func__, pm_ctrl.mode,
              pm->current_mode);

    while (pm_dvfs->clk_id)
    {
        if (run_flag)
            tbl_idx =  pm_dvfs->run_tbl_idx[pm->current_mode];
        else
            tbl_idx =  pm_dvfs->sleep_tbl_idx;

        DVFS_INFO("%s-%d: tbl_idx(%d) clk_req_id:%x\n", __func__,
                  pm_dvfs->ctrl.cur_tbl_idx, tbl_idx, pm_dvfs->clk_req_id);

        if (tbl_idx != pm_dvfs->ctrl.cur_tbl_idx)
        {
            ret = dvfs_set_rate_by_idx(pm_dvfs->dvfs, tbl_idx, pm_dvfs->clk_req_id);
            if (!ret)
                pm_dvfs->ctrl.cur_tbl_idx = tbl_idx;
            else
                rt_kprintf("rk_dvfs_req_set_rate_by_idx err!(%d)\n", pm_dvfs->clk_id);
        }
        pm_dvfs++;
    }

}
#else
static void pm_mode_list_dvfs(struct rt_pm *pm, uint32_t run_flag)
{
}
#endif
static void rk_pm_enter(struct rt_pm *pm)
{
    struct PM_SUSPEND_INFO suspendInfo = {0};
    int channel;

    if (pm->current_mode >= PM_RUN_MODE_HIGH &&
            pm->current_mode <= PM_RUN_MODE_LOW)
    {
        pm_mode_list_dvfs(pm, 1);
        if (pm_ctrl.pm_func)
            pm_ctrl.pm_func(pm->current_mode);
    }
    else if (pm->current_mode >= PM_SLEEP_MODE_SLEEP &&
             pm->current_mode <= PM_SLEEP_MODE_TIMER)
    {
        pm_mode_list_dvfs(pm, 0);
        if (pm_ctrl.pm_func)
            pm_ctrl.pm_func(pm->current_mode);
        pm_ctrl.has_enter_sleep = 1;

#ifdef RT_USING_UART
        channel = rt_hw_console_channel();
        if (channel >= 0)
        {
            suspendInfo.flag.uartChannel = channel;
            suspendInfo.flag.uartValid = 1;
        }
#endif

#if defined(RT_USING_QPIPSRAM)
        rk_psram_suspend();
#endif

#ifdef RT_USING_SNOR
        rk_snor_suspend();
#endif
        HAL_SYS_Suspend(&suspendInfo);
#ifdef RT_USING_SNOR
        rk_snor_resume();
#endif

#if defined(RT_USING_QPIPSRAM)
        rk_psram_resume();
#endif

#ifdef RT_USING_PM_TICK_DELAY
        pm_ref_timer_count_reinit();
#endif
    }
    else if (pm->current_mode == PM_SLEEP_MODE_SHUTDOWN)
    {
        /* need be implemented */
    }
    pm_ctrl.mode = pm->current_mode;
}

static void rk_pm_exit(struct rt_pm *pm)
{
    /* need be implemented */
    return;
}

static void rk_pm_frequency_change(struct rt_pm *pm, rt_uint32_t frequency)
{
    return;
}


#if defined(RT_USING_PM_TIMER) || defined(RT_USING_PM_TICK_DELAY)
#ifdef PM_TIMER
static void rk_pm_timer_handler(int vector, void *param)
{
    HAL_TIMER_ClrInt(PM_TIMER);
    HAL_TIMER_Stop_IT(PM_TIMER);

    PM_TIMER_INFO("%s:%ums\n", __func__,
                  (rt_uint32_t)HAL_DivU64(HAL_GetSysTimerCount() * 1000,
                                          PLL_INPUT_OSC_RATE));
    PM_TIMER_INFO("%s: tick %u\n", __func__, rt_tick_get());
}
#endif
#endif

#ifdef RT_USING_PM_TIMER
static void rk_pm_timer_start(struct rt_pm *pm, rt_uint32_t tick)
{
    bool need_timeout;

    HAL_TIMER_Stop_IT(TICK_TIMER);
    HAL_TIMER_ClrInt(TICK_TIMER);
    HAL_NVIC_ClearPendingIRQ(TICK_IRQn);

    PM_TIMER_INFO("%s: %ums, tick: %u timeout: 0x%x\n", __func__,
                  (rt_uint32_t)HAL_DivU64(HAL_GetSysTimerCount() *
                                          PM_MS_PER_SEC, PLL_INPUT_OSC_RATE),
                  rt_tick_get(), tick);

    if (tick != RT_TICK_MAX)
    {
        tick = rk_pm_tick_to_timer(tick);
#ifdef PM_TIMER
        HAL_TIMER_SetCount(PM_TIMER, tick);
        HAL_TIMER_Start_IT(PM_TIMER);
        need_timeout = false;
#else
        need_timeout = true;
#endif
    }
    else
    {
        tick = 0;
        need_timeout = false;
    }

    HAL_PM_TimerStart(tick, need_timeout);
}

static void rk_pm_timer_stop(struct rt_pm *pm)
{
    HAL_TIMER_Start_IT(TICK_TIMER);

    HAL_TIMER_Stop_IT(PM_TIMER);
    HAL_TIMER_ClrInt(PM_TIMER);
    HAL_NVIC_ClearPendingIRQ(PM_TIMER_IRQn);

    HAL_PM_TimerStop();
}

rt_tick_t rk_pm_timer_get_tick(struct rt_pm *pm)
{
    uint64_t count = HAL_PM_GetTimerCount();

    PM_TIMER_INFO("%s: %ums, tick: %u\n", __func__,
                  (rt_uint32_t)HAL_DivU64(count * PM_MS_PER_SEC,
                                          PLL_INPUT_OSC_RATE),
                  rk_pm_timer_to_tick(count));

    return rk_pm_timer_to_tick(count);
}
#else
static void rk_pm_timer_start(struct rt_pm *pm, rt_uint32_t timeout)
{
    return;
}

static void rk_pm_timer_stop(struct rt_pm *pm)
{
    return;
}

rt_tick_t rk_pm_timer_get_tick(struct rt_pm *pm)
{
    return 0;
}
#endif

#ifdef RT_USING_PM_TICK_DELAY
RT_WEAK void pm_ref_timer_count_reinit(void)
{
    return;
}

void pm_ref_timer_set_count(uint64_t count)
{
    pm_ref_timer_count = count;
}

uint64_t pm_ref_timer_get_count(void)
{
    return pm_ref_timer_count;
}
#endif

static struct rt_pm_ops rk_pm_ops =
{
    .enter = rk_pm_enter,
    .exit = rk_pm_exit,
#if PM_RUN_MODE_COUNT > 1
    .frequency_change = rk_pm_frequency_change,
#endif
    .timer_start = rk_pm_timer_start,
    .timer_stop = rk_pm_timer_stop,
    .timer_get_tick = rk_pm_timer_get_tick,
};
#endif

static void rt_pm_task_idle_prepare(void)
{
    pm_ctrl.has_enter_sleep = 0;
}

#ifdef RT_USING_PM_RUNTIME
RT_WEAK void rt_pm_runtime_enter(void)
{
    uint32_t idle_tick;
    ePM_RUNTIME_idleMode idle_mode;

    idle_tick = rt_timer_next_timeout_tick() - rt_tick_get();
    if (idle_tick > PM_MS_TO_SYS_TICK(20))
    {
        idle_mode = PM_RUNTIME_IDLE_DEEP;
    }
#ifdef RT_USING_PM_IDLE_NORMAL
    else if (idle_tick > PM_MS_TO_SYS_TICK(3))
    {
        idle_mode = PM_RUNTIME_IDLE_NORMAL;
    }
#endif
    else
    {
        idle_mode = PM_RUNTIME_IDLE_ONLY;
    }

    HAL_PM_RuntimeEnter(idle_mode);
}
#endif

static void rt_pm_task_idle_wfi(void)
{
    rt_ubase_t level;
#ifdef RT_USING_COMMON_TEST_CPU_USAGE
    uint64_t timer_count;
#endif

    if (pm_ctrl.has_enter_sleep)
    {
        pm_ctrl.has_enter_sleep = 0;
        return;
    }

#if defined(ARCH_ARM_CORTEX_M)
    asm volatile("MRS     %0, PRIMASK\n"
                 "CPSID   I"
                 : "=r"(level) : : "memory", "cc");
#else
    level = rt_hw_interrupt_disable();
#endif

#ifdef RT_USING_COMMON_TEST_CPU_USAGE
    timer_count = HAL_GetSysTimerCount();
#endif

#ifdef RT_USING_PM_RUNTIME
    rt_pm_runtime_enter();
#else
    asm volatile("dsb");
    asm volatile("wfi");
#endif

#ifdef RT_USING_COMMON_TEST_CPU_USAGE
    rkpm_cpu_idle_time_update(HAL_GetSysTimerCount() - timer_count);
#endif

#if defined(ARCH_ARM_CORTEX_M)
    asm volatile("MSR     PRIMASK, %0"
                 : : "r"(level) : "memory", "cc");
#else
    rt_hw_interrupt_enable(level);
#endif
}

void rkpm_register_dvfs_info(struct pm_mode_dvfs *dvfs, void (*pm_func)(uint32_t))
{
    RT_ASSERT(dvfs);
    pm_ctrl.pm_dvfs = dvfs;
    pm_ctrl.pm_func = pm_func;
    pm_ctrl.has_enter_sleep = 0;
}

#ifdef RT_USING_PM_DVFS
static int rkpm_dvfs_init(void)
{
    struct pm_mode_dvfs *pm_dvfs = pm_ctrl.pm_dvfs;

    regulator_req_init();
    clk_req_init();
    dvfs_init();

    while (pm_dvfs->clk_id)
    {
        pm_dvfs->dvfs = dvfs_get_by_clk(pm_dvfs->clk_id, &pm_dvfs->clk_req_id);
        RT_ASSERT(pm_dvfs->dvfs);
        pm_dvfs->ctrl.cur_tbl_idx = TBL_IDX_INV;
        DVFS_INFO("%s: dvfs-0x%p, clk_req_id: %x\n", __func__,
                  pm_dvfs->dvfs, pm_dvfs->clk_req_id);
        pm_dvfs++;
    }
    pm_ctrl.mode = PM_SLEEP_MODE_SHUTDOWN;
    pm_ctrl.dvfs_en = 1;

    return RT_EOK;
}
#endif

int rk_rt_pm_init(void)
{
    rt_err_t ret;

    rt_thread_idle_sethook(rt_pm_task_idle_prepare);

#ifdef RT_USING_PM
    rt_system_pm_init(&rk_pm_ops, 1 << PM_SLEEP_MODE_SLEEP, NULL);
#endif

    ret = rt_thread_idle_sethook(rt_pm_task_idle_wfi);
    if (ret)
        rt_kprintf("get rt_thread_idle_sethook error  for rkpm!\n");

#if defined(RT_USING_PM_TIMER) || defined(RT_USING_PM_TICK_DELAY)
    rt_hw_interrupt_install(PM_TIMER_IRQn, rk_pm_timer_handler, RT_NULL, "pm_tick");
    rt_hw_interrupt_umask(PM_TIMER_IRQn);
    HAL_TIMER_Init(PM_TIMER, TIMER_USER_DEFINED);
#endif

    return ret;
}

#ifdef RT_USING_PM_DVFS
INIT_COMPONENT_EXPORT(rkpm_dvfs_init);
#endif

#else /* !RT_USING_PM */

void rkpm_register_dvfs_info(struct pm_mode_dvfs *dvfs, void (*pm_func)(uint32_t))
{
}

void pm_runtime_request(ePM_RUNTIME_ID runTimeId)
{
}

void pm_runtime_release(ePM_RUNTIME_ID runTimeId)
{
}

void rt_pm_runtime_enter(void)
{
}

int rk_rt_pm_init(void)
{
    return RT_EOK;
}

#endif

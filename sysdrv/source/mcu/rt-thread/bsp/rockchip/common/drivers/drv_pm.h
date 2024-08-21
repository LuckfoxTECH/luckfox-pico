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

#ifndef _RK_PM_H_
#define _RK_PM_H_

#include <drivers/pm.h>
#include "drv_regulator.h"
#include "drv_dvfs.h"
#include "hal_base.h"

#define PM_MS_PER_SEC           (1000)
#define PM_RUN_TBL_IDX_MAX      (PM_RUN_MODE_COUNT)
#define TBL_IDX_INV             (0xF)

#define PM_MS_TO_SYS_TICK(ms)   ((RT_TICK_PER_SECOND * (ms))/PM_MS_PER_SEC)
#define PM_SYS_TICK_TO_MS(tick) ((PM_MS_PER_SEC * (tick))/RT_TICK_PER_SECOND)

#define PM_TIMER_TO_MS(timer_count) (rt_uint32_t)HAL_DivU64((timer_count) * \
                                                            PM_MS_PER_SEC,\
                                                            PLL_INPUT_OSC_RATE)
#define PM_MS_TO_TIMER(ms)          (rt_uint64_t)HAL_DivU64((ms) * \
                                                            PLL_INPUT_OSC_RATE,\
                                                            PM_MS_PER_SEC)

#if 0
#define PM_TIMER_INFO(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define PM_TIMER_INFO(fmt, ...)
#endif

#define TICK_DEALY_DBG 1
#if TICK_DEALY_DBG
#define PM_TICKDLY_INFO(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define PM_TICKDLY_INFO(fmt, ...)
#endif

struct pm_mode_dvfs
{
#ifdef HAL_CRU_MODULE_ENABLED
    eCLOCK_Name clk_id;
#endif
    struct rk_dvfs_desc *dvfs;
    uint8_t clk_req_id;
    struct
    {
        uint32_t cur_tbl_idx : 4;
    } ctrl;
    uint8_t run_tbl_idx[PM_RUN_TBL_IDX_MAX];
    uint8_t sleep_tbl_idx;
};

void rkpm_register_dvfs_info(struct pm_mode_dvfs *dvfs, void (*pm_func)(uint32_t));
void pm_runtime_request(ePM_RUNTIME_ID runTimeId);
void pm_runtime_release(ePM_RUNTIME_ID runTimeId);
void rt_pm_runtime_enter(void);

rt_uint64_t rk_pm_tick_to_timer(rt_uint32_t tick);
rt_uint32_t rk_pm_timer_to_tick(rt_uint64_t timer_count);

int rk_rt_pm_init(void);

#ifdef RT_USING_PM_TICK_DELAY
void pm_ref_timer_count_reinit(void);
void pm_ref_timer_set_count(uint64_t count);
rt_uint64_t pm_ref_timer_get_count(void);
#endif

#endif

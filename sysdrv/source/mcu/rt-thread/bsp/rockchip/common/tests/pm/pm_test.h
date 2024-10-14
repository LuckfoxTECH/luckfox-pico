/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    pm_test.h
  * @version V0.1
  * @brief   pm test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-01     Tony.Xie        first implementation
  *
  ******************************************************************************
  */

#ifndef _PM_TEST_H_
#define _PM_TEST_H_

#if 0
#define PM_TEST_INFO(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define PM_TEST_INFO(fmt, ...)
#endif

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

#define MEM_TEST_1K (1024)
#define MEM_TEST_1M ((MEM_TEST_1K) * (MEM_TEST_1K))

void rk_test_mem(void *addr, unsigned long b_size);
void test_cpu_func_calc_pi(int *mem_result, char *mem_pi_calc, long *pi_mem_f);

#ifdef RT_USING_COMMON_TEST_CPU_USAGE
void rkpm_cpu_idle_time_update(uint64_t idle_time);
#endif

#ifdef RT_USING_COREMARK
extern void coremark_main(void);
#endif

#endif

/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dvfs_scan.c
  * @version V0.1
  * @brief   dvfs scan
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-01     Tony.Xie        first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>
#include <stdbool.h>
#include "hal_base.h"
#include "drv_clock.h"
#include "drv_regulator.h"
#include "drv_dvfs.h"
#include "pm_test.h"

#ifdef RT_USING_COMMON_TEST_PM

#define CPU_TEST_PRT_LOOPS (15)

struct cpu_mem_test_ctrl
{
    unsigned long mem_size;
    char *mem_base;

    int *pi_mem_result;
    char *pi_mem_pi_calc;
    long *pi_mem_f;

    unsigned long loops_cnt;
    unsigned long test_loop;

    int test_begin;
};

static struct rt_thread cpu_mem_test_rt;
static rt_uint8_t cpu_mem_test_rt_stack[1024 * 2];
static struct cpu_mem_test_ctrl cpu_test;

void cpu_mem_loop_test(void *parameter)
{

    cpu_test.pi_mem_result =  malloc(10000 * sizeof(int));
    cpu_test.pi_mem_pi_calc = malloc(1000 * sizeof(char));
    cpu_test.pi_mem_f = malloc(2801 * sizeof(long));

    for (; cpu_test.mem_size > 0;)
    {
        cpu_test.mem_base = malloc(cpu_test.mem_size);
        if (!cpu_test.mem_base)
            cpu_test.mem_size >>= 2;
        else break;
    }

    rt_kprintf("%s: malloc mem_size %d\n", __func__, cpu_test.mem_size);

    while (1)
    {
        if (cpu_test.test_loop > cpu_test.loops_cnt || !cpu_test.test_begin)
        {
            rt_kprintf("**** %s: stop, loop-%d\n", __func__, cpu_test.test_loop);
            return;
        }

        if (cpu_test.test_loop % CPU_TEST_PRT_LOOPS == 0)
            rt_kprintf("**** %s test loop-%d\n", __func__, cpu_test.test_loop);

        cpu_test.test_loop ++;
        if (cpu_test.test_begin)
            test_cpu_func_calc_pi(cpu_test.pi_mem_result,
                                  cpu_test.pi_mem_pi_calc,
                                  cpu_test.pi_mem_f);
        if (cpu_test.test_begin)
            rk_test_mem(cpu_test.mem_base, cpu_test.mem_size);

#ifdef RT_USING_COREMARK
        if (cpu_test.test_begin)
            coremark_main();
#endif
    }

    if (cpu_test.pi_mem_result)
    {
        free(cpu_test.pi_mem_result);
        cpu_test.pi_mem_result = NULL;
    }

    if (cpu_test.pi_mem_f)
    {
        free(cpu_test.pi_mem_f);
        cpu_test.pi_mem_f = NULL;
    }

    if (cpu_test.pi_mem_pi_calc)
    {
        free(cpu_test.pi_mem_pi_calc);
        cpu_test.pi_mem_pi_calc = NULL;
    }

    if (cpu_test.mem_base)
    {
        free(cpu_test.mem_base);
        cpu_test.mem_base = NULL;
    }
}

static void cmd_cpu_mem_test_show_usage()
{
    rt_kprintf("Usage: \n");
    rt_kprintf("    cpu_mem_test -s                start cpu_mem_test case\n");
    rt_kprintf("    cpu_mem_test -s <mem_size(Kbyte)> <loop count>\n");
    rt_kprintf("    cpu_mem_test -p                stop cpu_mem_test case\n");
}

static void cmd_cpu_mem_test(int argc, char **argv)
{
    if (argc <= 1 || (strcmp(argv[1], "--help") == 0))
    {
        cmd_cpu_mem_test_show_usage();
        return;
    }

    if (strcmp(argv[1], "-p") == 0)
    {
        cpu_test.test_begin = 0;
        return;
    }

    if (strcmp(argv[1], "-s") != 0)
    {
        return;
    }

    if (argc > 2)
        cpu_test.mem_size = atoi(argv[2]);

    if (argc > 3)
        cpu_test.loops_cnt = atoi(argv[3]);

    if (!cpu_test.mem_size)
        cpu_test.mem_size = MEM_TEST_1M * 2;
    else
        cpu_test.mem_size *= MEM_TEST_1K;

    if (!cpu_test.loops_cnt)
        cpu_test.loops_cnt = 0xffffffff;

    rt_kprintf("%s: req mem_size=%d, loops_cnt=%d\n", __func__, cpu_test.mem_size, cpu_test.loops_cnt);

    if (!cpu_test.test_begin)
    {
        cpu_test.test_begin = 1;
        rt_thread_init(&cpu_mem_test_rt,
                       "cpu_mem_test_rt",
                       cpu_mem_loop_test,
                       RT_NULL,
                       &cpu_mem_test_rt_stack[0],
                       sizeof(cpu_mem_test_rt_stack),
                       20,
                       32);
        rt_thread_startup(&cpu_mem_test_rt);
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(cmd_cpu_mem_test, cpu_mem_test, cpu and mem test cmd);
#endif

#ifdef RK_PM_USING_DVFS
enum
{
    TEST_STEP_UP = 0,
    TEST_STEP_DN,
    TEST_STEP_UP_ITVL,
    TEST_STEP_DN_ITVL,
    TEST_STEP_MIN_MAX,
    TEST_STEP_END,
};

enum
{
    DVFS_TEST_CPU = 0,
    DVFS_TEST_MEM,
    DVFS_TEST_DSP,
    DVFS_TEST_MAX,
};

#define DVFS_LIST_STACK_SIZE (1024*2)

struct dvfs_scan_test_st
{
    uint8_t start;
    uint8_t clk_req_id;
    struct rk_dvfs_desc *dvfs;
    uint32_t loops;
    uint32_t cur_type;
    uint32_t tbl_cur_idx;
    uint32_t tbl_itvl_idx;
    uint32_t tbl_max_min_idx;
    uint32_t tbl_idx_cnt;
    uint32_t clk_id;
    rt_sem_t sem;
    rt_timer_t timer;
};

static struct rt_thread dvfs_scan_rt[DVFS_TEST_MAX];
static rt_uint8_t dvfs_scan_stack[DVFS_TEST_MAX][DVFS_LIST_STACK_SIZE];
static struct dvfs_scan_test_st dvfs_st_array[DVFS_TEST_MAX];
static uint32_t dvfs_test_clk_ids[DVFS_TEST_MAX] =
{
    [DVFS_TEST_CPU] = CPU_CLK_ID,
    [DVFS_TEST_MEM] = MEM_CLK_ID,
    [DVFS_TEST_DSP] = DSP_CLK_ID,
};

static void dvfs_scan_test(struct dvfs_scan_test_st *dvfs_st)
{
    uint32_t test_idx;
    uint32_t idx_max = dvfs_st->tbl_idx_cnt - 1;

    rt_kprintf("**** %s: clk-%x, test loops-%d ****\n",
               __func__, dvfs_st->clk_id, dvfs_st->loops);

    if (dvfs_st->cur_type == TEST_STEP_UP)
    {
        test_idx = dvfs_st->tbl_cur_idx;
    }
    else if (dvfs_st->cur_type == TEST_STEP_DN)
    {
        test_idx = idx_max - dvfs_st->tbl_cur_idx;
    }
    else if (dvfs_st->cur_type == TEST_STEP_UP_ITVL)
    {
        test_idx = dvfs_st->tbl_itvl_idx;
    }
    else if (dvfs_st->cur_type == TEST_STEP_DN_ITVL)
    {
        test_idx = idx_max - dvfs_st->tbl_itvl_idx;
    }
    else if (dvfs_st->cur_type == TEST_STEP_MIN_MAX)
    {
        if (!dvfs_st->tbl_max_min_idx)
            test_idx = idx_max;
        else
            test_idx = 0;
    }
    else
    {
        goto __exit;
    }

    rt_kprintf("    %s: cur_type-%d, test_idx-%d****\n",
               __func__, dvfs_st->cur_type, test_idx);

    dvfs_set_rate_by_idx(dvfs_st->dvfs,  test_idx, dvfs_st->clk_req_id);

__exit:
    dvfs_st->tbl_itvl_idx += 2;
    if (dvfs_st->tbl_itvl_idx > idx_max)
        dvfs_st->tbl_itvl_idx = 1;

    if (!dvfs_st->tbl_max_min_idx)
        dvfs_st->tbl_max_min_idx = idx_max;
    else
        dvfs_st->tbl_max_min_idx = 0;

    dvfs_st->loops ++;

    dvfs_st->tbl_cur_idx = dvfs_st->loops % dvfs_st->tbl_idx_cnt;

    if (!dvfs_st->tbl_cur_idx)
    {
        dvfs_st->tbl_itvl_idx = 0;
        dvfs_st->tbl_max_min_idx = 0;
        dvfs_st->cur_type++;
        dvfs_st->cur_type = dvfs_st->cur_type % TEST_STEP_END;
    }
}

void dvfs_scan_test_handler(void *parameter)
{
    static rt_err_t result;
    struct dvfs_scan_test_st *dvfs_st = (struct dvfs_scan_test_st *)parameter;

    while (1)
    {
        if (!dvfs_st->start)
        {
            rt_kprintf("%s: stop!\n", __func__);
            break;
        }
        result = rt_sem_take(dvfs_st->sem, RT_WAITING_FOREVER);
        if (result != RT_EOK)
        {
            rt_kprintf("take a dynamic semaphore, failed.\n");
            rt_sem_delete(dvfs_st->sem);
            return;
        }
        dvfs_scan_test(dvfs_st);
    }
}

static void dvfs_scan_timer_handler(void *parameter)
{
    struct dvfs_scan_test_st *dvfs_st = (struct dvfs_scan_test_st *)parameter;

    rt_sem_release(dvfs_st->sem);

    if (!dvfs_st->start)
    {
        rt_kprintf("%s: stop!\n", __func__);
        rt_timer_stop(dvfs_st->timer);
    }
}

static void  dvfs_scan_cmd(int argc, char **argv)
{
    uint32_t clk_id;
    struct dvfs_scan_test_st *dvfs_st;
    uint32_t test_id;
    uint8_t test_start;

    if (argc < 1)
        return;

    if (argc <= 1 || strcmp(argv[1], "--help") == 0)
    {
        rt_kprintf("Usage: \n");
        rt_kprintf("    dvfs_scan -s <clk name>        start dvfs_scan case\n");
        rt_kprintf("    dvfs_scan -p <clk name>        stop dvfs_scan case\n");
        return;
    }

    if (argc < 2)
        return;

    if (strcmp(argv[1], "-p") == 0)
        test_start = 0;
    else if (strcmp(argv[1], "-s") == 0)
        test_start = 1;
    else return;

    if (argc < 3)
        return;

    if (strcmp(argv[2], "cpu") == 0)
    {
        test_id = DVFS_TEST_CPU;
    }
    else if (strcmp(argv[2], "mem") == 0)
    {
        test_id = DVFS_TEST_MEM;
    }
    else if (strcmp(argv[2], "dsp") == 0)
    {
        test_id = DVFS_TEST_DSP;
    }
    else return;

    dvfs_st = &dvfs_st_array[test_id];

    if (!test_start)
    {
        dvfs_st->start = 0;
        return;
    }
    else if (dvfs_st->start)
    {
        return;
    }

    dvfs_st->start = 1;

    clk_id = dvfs_test_clk_ids[test_id];
    RT_ASSERT(clk_id);

    dvfs_st->dvfs = dvfs_get_by_clk(clk_id, &dvfs_st->clk_req_id);
    if (!dvfs_st->dvfs)
    {
        rt_kprintf("** %s: %s error\n", __func__, argv[1]);
        goto  __exit;
    }
    else
    {
        dvfs_set_rate_by_idx(dvfs_st->dvfs,  0, dvfs_st->clk_req_id);
    }

    rt_kprintf("****%s: dvfs clk %s: 0x%x begin\n", __func__, argv[1], clk_id);

    dvfs_st->loops = 0;
    dvfs_st->cur_type = 0;
    dvfs_st->tbl_cur_idx = 0;
    dvfs_st->tbl_itvl_idx = 0;
    dvfs_st->tbl_max_min_idx = 0;
    dvfs_st->tbl_idx_cnt = dvfs_st->dvfs->tbl_cnt;
    dvfs_st->clk_id = clk_id;
    dvfs_st->sem = rt_sem_create("dvfs_sem", 0, RT_IPC_FLAG_FIFO);


    rt_thread_init(&dvfs_scan_rt[test_id],
                   "cpu_mem_test_rt",
                   dvfs_scan_test_handler,
                   dvfs_st,
                   &dvfs_scan_stack[test_id][0],
                   DVFS_LIST_STACK_SIZE,
                   20,
                   32);
    /* startup */
    rt_thread_startup(&dvfs_scan_rt[test_id]);

    if (!dvfs_st->timer)
        dvfs_st->timer = rt_timer_create("dvfs",
                                         dvfs_scan_timer_handler,
                                         dvfs_st,
                                         (RT_TICK_PER_SECOND * 4),
                                         RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
    rt_timer_start(dvfs_st->timer);
__exit:
    return;
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(dvfs_scan_cmd, dvfs_scan, dvfs scan cmd);
#endif
#endif
#endif

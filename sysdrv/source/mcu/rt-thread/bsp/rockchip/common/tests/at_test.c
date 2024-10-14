/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-25     Steven Liu   the first version
 */

#include <rtthread.h>

#ifdef RT_USING_AUTO_TEST
#ifdef RT_USING_FINSH
#include <finsh.h>

#define AT_TEST_PRIORITY   25
#define AT_TEST_TICK       5
#define AT_TEST_STACK_SIZE 0x800

static rt_thread_t at_test_thread;
static rt_uint8_t at_test_prefix[64];
static const char *at_test_current;

static void at_test_entry(void *parameter)
{
    rt_uint8_t pass_count = 0;
    rt_uint8_t fail_count = 0;
    struct finsh_syscall *index;

    rt_kprintf("AutoTest Start!\n");
    for (index = _syscall_table_begin; index < _syscall_table_end; FINSH_NEXT_SYSCALL(index))
    {
        if (rt_strstr(index->name, (const char *)at_test_prefix) == index->name)
        {
            long test_return;

            at_test_current = index->name + 4;
            rt_kprintf("START - %s>\n", at_test_current);
            test_return = index->func();
            if (test_return == RT_EOK)
            {
                rt_kprintf(" PASS - %s\n", at_test_current);
                pass_count++;
            }
            else
            {
                rt_kprintf(" FAIL - %s\n", at_test_current);
                fail_count++;
            }
        }
    }
    rt_kprintf("AutoTest completed %d pass %d fail.\n", pass_count, fail_count);
}

static int at_test_list(void)
{
    struct finsh_syscall *index;

    for (index = _syscall_table_begin; index < _syscall_table_end; FINSH_NEXT_SYSCALL(index))
    {
        if (rt_strstr(index->name, "_at_") == index->name)
        {
#ifdef FINSH_USING_DESCRIPTION
            rt_kprintf("%-16s -- %s\n", index->name + 4, index->desc);
#else
            rt_kprintf("%s\n", index->name + 4);
#endif
        }
    }

    return RT_EOK;
}
INIT_APP_EXPORT(at_test_list);

static int at_test_start(void)
{
    rt_err_t ret = RT_EOK;
    const char *at_case = "all";

    rt_memset(at_test_prefix, 0, sizeof(at_test_prefix));
    if (rt_strncmp(at_case, "all", sizeof(at_case)) == 0)
        rt_snprintf((char *)at_test_prefix, sizeof(at_test_prefix), "_at_");
    else
        rt_snprintf((char *)at_test_prefix, sizeof(at_test_prefix), "_at_%s", at_case);

    at_test_thread = rt_thread_create("at_test", at_test_entry, RT_NULL, AT_TEST_STACK_SIZE,
                                      AT_TEST_PRIORITY, AT_TEST_TICK);
    if (at_test_thread != RT_NULL)
        rt_thread_startup(at_test_thread);
    else
        ret = RT_ERROR;

    return ret;
}
INIT_APP_EXPORT(at_test_start);

#endif /* RT_USING_FINSH */
#endif /* RT_USING_AUTO_TEST */

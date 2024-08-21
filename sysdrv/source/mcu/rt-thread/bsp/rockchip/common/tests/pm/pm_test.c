/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    pm_test.c
  * @author  Chen Liang
  * @version V0.1
  * @date    18-Sep-2019
  * @brief   pm test for pisces
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

#ifdef RT_USING_COMMON_TEST_PM
static void clk_test_show_usage()
{
    rt_kprintf("Usage: \n");
    rt_kprintf("    clk -w <id|name> <rate_hz>    set clk rate\n");
    rt_kprintf("    clk -r <id|name>              get clk rate\n");
    rt_kprintf("    clk -e <id>                   enable clk\n");
    rt_kprintf("    clk -d <id>                   disable clk\n");
    rt_kprintf("    clk_dump                      print clk id\n");
}

void clk_test(int argc, char **argv)
{
    char *cmd;
    uint32_t id = 0, rate;

    if ((argc > 4) || (argc <= 1))
        goto out;

    if (!strcmp(argv[2], "core"))
    {
        id = CPU_CLK_ID;
    }
    else if (!strcmp(argv[2], "shrmem"))
    {
        id = MEM_CLK_ID;
    }
    else if (!strcmp(argv[2], "dsp"))
    {
        id = DSP_CLK_ID;
    }
    else
    {
        id = strtol(argv[2], NULL, 16);
    }

    cmd = argv[1];
    if (!strcmp(cmd, "-w"))
    {
        if (argc != 4)
            goto out;

        rate = strtol(argv[3], NULL, 10);
        clk_set_rate(id, rate);
        rt_kprintf("clk_get_rate: id[%x], rate[%d], readback[%d]\n",
                   id, rate, clk_get_rate(id));
    }
    else if (!strcmp(cmd, "-r"))
    {
        rt_kprintf("clk_get_rate: id[%x], rate[%d]\n", id, clk_get_rate(id));
    }
    else if (!strcmp(cmd, "-e"))
    {
        id = strtol(argv[2], NULL, 10);
        clk_enable_by_id(id);
    }
    else if (!strcmp(cmd, "-d"))
    {
        id = strtol(argv[2], NULL, 10);
        clk_disable_by_id(id);
    }
    else
    {
        goto out;
    }

    return;
out:
    clk_test_show_usage();
}

struct regulator_help
{
    const char *name;
    uint8_t id;
};

#define HELP_DATA(_id) {.name = #_id, .id = _id}
static struct regulator_help regulator_help_str[] =
{
    HELP_DATA(PWR_ID_CORE),
    HELP_DATA(PWR_ID_LOG),
    HELP_DATA(PWR_ID_DSP_CORE),
    HELP_DATA(PWR_ID_VCC_MIPI),
    HELP_DATA(PWR_ID_VCC_AUDIO),
    HELP_DATA(PWR_ID_DSP_VCC_MIPI),
    HELP_DATA(PWR_ID_TOP)
};

static void regulator_test_show_usage()
{
    int i;

    rt_kprintf("Usage: \n");
    rt_kprintf("    reg -w <id> <volt_uv>    set regulator voltage\n");
    rt_kprintf("    reg -r <id>              get regulator voltage\n");
    rt_kprintf("    reg -ws <id> <volt_uv>   set regulator suspend voltage\n");
    rt_kprintf("    reg -rs <id>             get regulator suspend voltage\n");
    rt_kprintf("    reg -rr <id>             get real regulator voltage\n");
    rt_kprintf("    reg -e <id>              enable regulator\n");
    rt_kprintf("    reg -d <id>              disable regulator\n");

    rt_kprintf("    regulator id list:\n");
    for (i = 0; i < HAL_ARRAY_SIZE(regulator_help_str); i++)
    {
        rt_kprintf("        id: %d, name: %s\n",
                   regulator_help_str[i].id,
                   regulator_help_str[i].name);
    }
}

static void regulator_test(int argc, char **argv)
{
    char *cmd;
    uint32_t id = 0, volt;
    struct regulator_desc *desc;

    if ((argc > 4) || (argc <= 1))
        goto out;

    id = strtol(argv[2], NULL, 10);
    if (id >= PWR_ID_MAX || !id)
        goto out;

    desc = regulator_get_desc_by_pwrid(id);
    if (!desc)
    {
        rt_kprintf("no regulator found for id[%d]\n", id);
        goto out;
    }

    cmd = argv[1];
    if (!strcmp(cmd, "-w"))
    {
        if (argc != 4)
            goto out;

        volt = strtol(argv[3], NULL, 10);
        regulator_set_voltage(desc, volt);
        rt_kprintf("regulator_set_voltage: id[%d], volt[%d], readback[%d]\n",
                   id, volt, regulator_get_voltage(desc));
    }
    else if (!strcmp(cmd, "-r"))
    {
        rt_kprintf("regulator_get_voltage: id[%d], volt[%d]\n",
                   id, regulator_get_voltage(desc));
    }
    else if (strcmp(cmd, "-ws") == 0 && argc >= 4)
    {
        if (argc != 4)
            goto out;

        volt = strtol(argv[3], NULL, 10);
        regulator_set_suspend_voltage(desc, volt);
        rt_kprintf("regulator_set_suspend_voltage: id[%d], volt[%d], readback[%d]\n",
                   id, volt, regulator_get_suspend_voltage(desc));
    }
    else if (strcmp(cmd, "-rs") == 0)
    {
        rt_kprintf("regulator_get_suspend_voltage: id[%d], volt[%d]\n",
                   id, regulator_get_suspend_voltage(desc));
    }
    else if (strcmp(cmd, "-rr") == 0)
    {
        rt_kprintf("regulator_get_real_voltage: id[%d], volt[%d]\n",
                   id, regulator_get_real_voltage(desc));
    }
    else if (strcmp(cmd, "-e") == 0)
    {
        rt_kprintf("regulator_enable: id[%d], ret(%d)\n",
                   id, regulator_enable(desc));
    }
    else if (strcmp(cmd, "-d") == 0)
    {
        rt_kprintf("regulator_disable: id[%d], ret(%d)\n",
                   id, regulator_disable(desc));
    }
    else
    {
        goto out;
    }

    return;
out:
    regulator_test_show_usage();
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(clk_test, clk, clk test cmd);
MSH_CMD_EXPORT_ALIAS(regulator_test, reg, regulator test cmd);
#endif

#ifdef RK_PM_USING_DVFS
struct dvfs_clk_help
{
    const char *name;
    uint32_t id;
};

static struct dvfs_clk_help dvfs_clk_info[] =
{
    {"core", CPU_CLK_ID},
    {"shrmem", MEM_CLK_ID},
    {"dsp", DSP_CLK_ID},
};

static void dvfs_clk_test_show_usage()
{
    rt_kprintf("Usage: \n");
    rt_kprintf("    dvfs_clk -w <name> <rate_hz>       set clk rate and volt\n");
    rt_kprintf("    dvfs_clk -r <name>                 get clk rate and volt\n");
    rt_kprintf("    dvfs_clk --dump                    get dvfs info\n");
}

static void dvfs_clk_dump(struct rk_dvfs_desc *desc)
{
    int i;

    RT_ASSERT(desc);
    RT_ASSERT(desc->table);

    rt_kprintf("dvfs clk info (clk_id = 0x%x, pwr_id =%d):\n",
               desc->clk_id, desc->pwr_id);
    for (i = 0; i < desc->tbl_cnt; i++)
    {
        rt_kprintf("    idx = %d, freq =%d, volt = %d\n",
                   i, desc->table[i].freq, desc->table[i].volt);
    }
}

static void dvfs_clks_list(void)
{
    struct rk_dvfs_desc *desc = dvfs_get_desc();
    uint32_t cnt = dvfs_get_desc_cnt();
    int i;

    rt_kprintf("dvfs clk name and id list\n");

    for (i = 0; i < HAL_ARRAY_SIZE(dvfs_clk_info); i++)
    {
        rt_kprintf("    id: 0x%x, name: %s\n",
                   dvfs_clk_info[i].id, dvfs_clk_info[i].name);
    }

    for (i = 0; i < cnt; i++)
    {
        dvfs_clk_dump(desc);
        desc ++;
    }
}

static void dvfs_clk_test(int argc, char **argv)
{
    uint32_t clk_id, clk_rate;
    uint32_t cmd_w;
    uint8_t clk_req_id;
    struct rk_dvfs_desc *dvfs_desc;

    if (argc < 2)
    {
        dvfs_clk_test_show_usage();
        return;
    }

    if (strcmp(argv[1], "--dump") == 0)
    {
        dvfs_clks_list();
        return;
    }

    if (strcmp(argv[1], "-w") == 0)
    {
        cmd_w = 1;
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        cmd_w = 0;
    }
    else return;

    if (argc < 3)
        return;

    if (strcmp(argv[2], "core") == 0)
        clk_id = CPU_CLK_ID;
    else if (strcmp(argv[2], "mem") == 0)
        clk_id = MEM_CLK_ID;
    else if (strcmp(argv[2], "dsp") == 0)
        clk_id = DSP_CLK_ID;
    else return;

    dvfs_desc = dvfs_get_by_clk(clk_id, &clk_req_id);

    if (cmd_w)
    {
        if (argc < 4)
            return;
        clk_rate = atoi(argv[3]);
        dvfs_set_rate(dvfs_desc,  clk_req_id, clk_rate);
        rt_kprintf("%s:clk_id-0x%x set rate-%d chk-%d\n", __func__, clk_id, clk_rate, clk_get_rate(clk_id));
    }
    else
    {
        rt_kprintf("%s:clk_id-0x%x get rate-%d\n", __func__, clk_id, clk_get_rate(clk_id));
    }

}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT_ALIAS(dvfs_clk_test, dvfs_clk, dvfs_clk test cmd);
#endif

#endif
#endif

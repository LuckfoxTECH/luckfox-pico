/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_coredump.c
  * @author
  * @version V0.1
  * @date    09-April-2020
  * @brief   coredump driver
  *
  ******************************************************************************
  */

#include <rtthread.h>

#ifdef RT_USING_COREDUMP

#include "drv_coredump.h"

static coredump_ops_t *g_ops;
static coredump_ops_t *coredump_ops[CD_NUMS];

unsigned int cd_regs[REGS_NUM] =
{
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0xdeadbeef,
    0,
};

void register_coredump_ops(coredump_dst_t dst, coredump_ops_t *ops)
{
    if (dst < CD_NUMS)
        coredump_ops[dst] = ops;
    else
        rt_kprintf("%s fail dst:%d\n", __func__, dst);
}

static int coredump_enter(void)
{
    return g_ops->cd_enter();
}

static int coredump_data_save(coredump_area_t *area)
{
    int ret = 0;
    char *buf = (char *)area;
    unsigned int size = 12;

    ret = g_ops->cd_data_save(buf, size);
    if (ret < size)
        return -RT_ERROR;

    buf = (char *)area->start_addr;
    size = area->end_addr - area->start_addr;

    ret = g_ops->cd_data_save(buf, size);
    if (ret < size)
        return -RT_ERROR;
    return RT_EOK;
}

static int coredump_exit(void)
{
    return g_ops->cd_exit();
}

static coredump_ops_t *coredump_get_ops(enum coredump_type cd_type)
{
    if (cd_type == CD_ASSERT)
        g_ops = coredump_ops[CD_SDCARD];

    if (!g_ops)
        g_ops = coredump_ops[CD_FLASH];

    return g_ops;
}

int rt_coredump_init(void)
{
    coredump_area_t *area = &cd_area[0];

    area->magic_id = COREDUMP_MAGIC_ID;
    area->start_addr = (uint32_t)cd_regs;
    area->end_addr = area->start_addr + REGS_NUM * 4;
    area->cache = 0;
    area->prepare = NULL;

    return RT_EOK;
}

static int coredump_init(void)
{
    return rt_coredump_init();
}
INIT_DEVICE_EXPORT(coredump_init);

static int coredump_handler(enum coredump_type cd_type)
{
    int i = 0, ret = 0;
    coredump_area_t *cda;

    coredump_get_ops(cd_type);

    if (!g_ops)
        return -RT_ERROR;

    if (cd_area[0].magic_id != COREDUMP_MAGIC_ID)
        rt_coredump_init();

    ret = coredump_enter();
    if (ret < 0)
        return -RT_ERROR;

    i = 0;
    while (&cd_area[i])
    {
        cda = &cd_area[i];

        if (cda->magic_id != COREDUMP_MAGIC_ID)
            break;
        if (cda->prepare)
            cda->prepare(&cd_area[i]);

        ret = coredump_data_save(cda);

        if (ret < 0)
            break;
        i++;
    }

    coredump_exit();
    return RT_EOK;
}

int rt_coredump_handler(enum coredump_type cd_type)
{
    return coredump_handler(cd_type);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
#ifdef FINSH_USING_MSH

static int rt_coredump_test(int argc, char **argv)
{
    rt_kprintf("%s:%d\n", __func__, __LINE__);

    RT_ASSERT(0);
    return 0;
}

MSH_CMD_EXPORT_ALIAS(rt_coredump_test, coredump, e.g: coredump);
#endif /* FINSH_USING_MSH */
#endif /* RT_USING_FINSH */

#endif /* end of RT_USING_COREDUMP */
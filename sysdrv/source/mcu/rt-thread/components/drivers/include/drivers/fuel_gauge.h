/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-12-26     Joseph Chen     first implementation
 */

#ifndef  __FUEL_GAUGE_H__
#define  __FUEL_GAUGE_H__

#include <rtthread.h>

#define RT_DEVICE_CTRL_FG_GET       (1) /* Get fuel gauge status */

struct rt_fg_status
{
    rt_int8_t  soc; /* state of percent */
    rt_int32_t fcc; /* full charge capacity */
    rt_int32_t current; /* battery current */
    rt_int32_t voltage; /* battery voltage */
    rt_int8_t  present; /* battery existance */
};

enum
{
    POWER_SUPPLY_STATUS_UNKNOWN = 0,
    POWER_SUPPLY_STATUS_CHARGING,
    POWER_SUPPLY_STATUS_DISCHARGING,
    POWER_SUPPLY_STATUS_NOT_CHARGING,
    POWER_SUPPLY_STATUS_FULL,
};

struct rt_fg_ops;
struct rt_fg_device
{
    struct rt_device parent;
    const struct rt_fg_ops *ops;
};
typedef struct rt_fg_device rt_fg_t;

struct rt_fg_ops
{
    rt_err_t (*init)(rt_fg_t *fg);
    rt_err_t (*control)(rt_fg_t *fg, int cmd, void *arg);
};

rt_err_t rt_fg_register(rt_fg_t *fg, const char *name, rt_uint32_t flag);

#endif /* __FUEL_GAUGE_H__ */

/*
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author          Notes
 * 2019-12-26     Joseph Chen     first implementation
 */

#ifndef  __CHARGER_H__
#define  __CHARGER_H__

#include <rtthread.h>

#define RT_DEVICE_CTRL_CHAGER_LIMIT_GET     (1) /* Get min, max */
#define RT_DEVICE_CTRL_CHAGER_STATUS_GET    (2) /* Get current configure */
#define RT_DEVICE_CTRL_CHAGER_BATVOL_SET    (3) /* Set battery charge voltage */
#define RT_DEVICE_CTRL_CHAGER_BATCUR_SET    (4) /* Set battery charge current */
#define RT_DEVICE_CTRL_CHAGER_FNSCUR_SET    (5) /* Set battery finish current */
#define RT_DEVICE_CTRL_CHAGER_SRCCUR_SET    (6) /* Set adapter input source current */

struct rt_charger_limit
{
    rt_int32_t bat_vmin; /* battery charge voltage min, max */
    rt_int32_t bat_vmax;
    rt_int32_t bat_cmin; /* battery charge current min, max */
    rt_int32_t bat_cmax;
    rt_int32_t src_cmin; /* adapter input source current min, max */
    rt_int32_t src_cmax;
    rt_int32_t fns_cmin;
    rt_int32_t fns_cmax;
};

struct rt_charger_status
{
    rt_int32_t bat_vol;
    rt_int32_t bat_cur;
    rt_int32_t src_cur;
    rt_int32_t fns_cur;
};

struct rt_charger_ops;
struct rt_charger_device
{
    struct rt_device parent;
    const struct rt_charger_ops *ops;
};
typedef struct rt_charger_device rt_charger_t;

struct rt_charger_ops
{
    rt_err_t (*init)(rt_charger_t *charger);
    rt_err_t (*control)(rt_charger_t *charger, int cmd, void *arg);
};

rt_err_t rt_charger_register(rt_charger_t *charger,
                             const char *name, rt_uint32_t flag);

#endif /* __CHARGER_H__ */

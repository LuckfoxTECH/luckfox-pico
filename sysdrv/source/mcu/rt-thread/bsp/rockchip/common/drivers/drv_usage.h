/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-02-22     Cliff Chen   first implementation
 */

#ifndef __DRV_USAGE_H__
#define __DRV_USAGE_H__

void rt_cpu_usage_init(void);
uint32_t rt_cpu_usage_get(void);

#endif /* __DRV_USAGE_H__ */
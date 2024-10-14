/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp3.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef __ISP_V3_H__
#define __ISP_V3_H__

/* Includes ------------------------------------------------------------------*/
#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>

#include "common.h"
#include "rtconfig.h"

#if defined(RT_USING_ISP3)

#define ISP_WORK_QUEUE_ENABLE 0

/* Exported macro ------------------------------------------------------------*/
#define RK_DEVICE_CTRL_DEVICE_INIT                            (0)
#define RK_DEVICE_CTRL_ISP_SET_WORKMODE                       (1)
#define RK_DEVICE_CTRL_ISP_SET_FMT                            (2)
#define RK_DEVICE_CTRL_ISP_CROP_IMAGE                         (3)
#define RK_DEVICE_CTRL_ISP_REQBUF                             (4)
#define RK_DEVICE_CTRL_ISP_QUERYBUF                           (5)
#define RK_DEVICE_CTRL_ISP_QBUF                               (6)
#define RK_DEVICE_CTRL_ISP_DQBUF                              (7)
#define RK_DEVICE_CTRL_ISP_STREAM_ON                          (8)
#define RK_DEVICE_CTRL_ISP_STREAM_OFF                         (9)
#define RK_DEVICE_CTRL_ISP_GET_SUBDEV                         (10)
#define RK_DEVICE_CTRL_ISP_SET_BLOCK_NUM                      (11)
#define RK_DEVICE_CTRL_ISP_GET_INPUT_FORMAT                   (12)
#define RK_DEVICE_CTRL_ISP_SET_PARAMS                     (13)
#define RK_DEVICE_CTRL_ISP_GET_STATS                          (14)

/* Exported types ------------------------------------------------------------*/
struct rk_isp_ops;
struct rk_isp_device
{
    struct rt_device parent;
    const struct rk_isp_ops *ops;
};

struct rk_isp_ops
{
    rt_err_t (*init)(struct rk_isp_device *dev);
    rt_err_t (*open)(struct rk_isp_device *dev, uint16_t oflag);
    rt_err_t (*close)(struct rk_isp_device *dev);
    rt_err_t (*control)(struct rk_isp_device *dev, int cmd, void *arg);
    rt_err_t (*rx_indicate)(struct rk_isp_device *dev, rt_size_t size);
};

#if ISP_WORK_QUEUE_ENABLE
struct rk_isp_work
{
    struct rt_work work;
    void (*fun)(void *parameter);
    void *parameter;
};
void rk_isp_workqueue_fun(struct rt_work *work, void *work_data);
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

rt_err_t rk_isp_controller_register(struct rk_isp_device *isp,
                                    const char *name,
                                    void *data);

#endif

#endif


/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vicap.h
  * @version V0.0.1
  * @brief   video capture device abstract for rk2108
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */
#ifndef __VICAP_H__
#define __VICAP_H__

/* Includes ------------------------------------------------------------------*/
#if defined(__RT_THREAD__)
#include "adapter/adapter.h"
#elif defined(__RK_OS__)
#include "driver/adapter.h"
#endif

#if defined(VICAP_MODULE_COMPILED)
#define VICAP_WORK_QUEUE_ENABLE 0

/* Exported macro ------------------------------------------------------------*/
#define RK_DEVICE_CTRL_DEVICE_INIT                              (0)
#define RK_DEVICE_CTRL_VICAP_SET_WORKMODE                       (1)
#define RK_DEVICE_CTRL_VICAP_SET_FMT                            (2)
#define RK_DEVICE_CTRL_VICAP_CROP_IMAGE                         (3)
#define RK_DEVICE_CTRL_VICAP_REQBUF                             (4)
#define RK_DEVICE_CTRL_VICAP_QUERYBUF                           (5)
#define RK_DEVICE_CTRL_VICAP_QBUF                               (6)
#define RK_DEVICE_CTRL_VICAP_DQBUF                              (7)
#define RK_DEVICE_CTRL_VICAP_STREAM_ON                          (8)
#define RK_DEVICE_CTRL_VICAP_STREAM_OFF                         (9)
#define RK_DEVICE_CTRL_VICAP_GET_SUBDEV                         (10)
#define RK_DEVICE_CTRL_VICAP_SET_BLOCK_NUM                      (11)
#define RK_DEVICE_CTRL_VICAP_GET_INPUT_FORMAT                   (12)

/* Exported types ------------------------------------------------------------*/
struct rk_vicap_ops;
struct rk_vicap_device
{
    rk_device parent;
    const struct rk_vicap_ops *ops;
};

struct rk_vicap_ops
{
    ret_err_t (*init)(struct rk_vicap_device *dev);
    ret_err_t (*open)(struct rk_vicap_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_vicap_device *dev);
    ret_err_t (*control)(struct rk_vicap_device *dev, dt_cmd_t cmd, void *arg);
    ret_err_t (*rx_indicate)(struct rk_vicap_device *dev, ret_size_t size);
};

#if VICAP_WORK_QUEUE_ENABLE
struct rk_vicap_work
{
    struct rt_work work;
    void (*fun)(void *parameter);
    void *parameter;
};
void rk_vicap_workqueue_fun(struct rt_work *work, void *work_data);
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
#if defined(__RK_OS__)
void *rk_rkos_vicap_create(uint8_t dev_id, void *arg);
ret_err_t rk_rkos_vicap_delete(uint8_t dev_id, void *arg);
#endif

ret_err_t rk_vicap_controller_register(struct rk_vicap_device *vicap,
                                       const char *name,
                                       void *data);

#endif

#endif

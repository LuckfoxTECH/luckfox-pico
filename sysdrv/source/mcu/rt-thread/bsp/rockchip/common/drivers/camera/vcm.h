/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vcm.h
  * @author  ISP_TEAM
  * @version V0.0.1
  * @date
  * @brief   image signal processing (ISP) abstract device
  *
  ******************************************************************************
  */
#ifndef __VCM_H__
#define __VCM_H__

/* Includes ------------------------------------------------------------------*/
#include <rthw.h>
#include <rtthread.h>
#include "rtconfig.h"

#if defined(RT_USING_VCM)

/* Exported macro ------------------------------------------------------------*/
#define RK_VCM_DEVICE_CID_INIT                                       0
#define RK_GET_VCM_FOCUS_ABSOLUTE                                    1
#define RK_SET_VCM_FOCUS_ABSOLUTE                                    2
#define RK_GET_VCM_MOVE_RES                                          3

#define GT9760S_DRIVER_NAME                                        "vcm-0"
#define I2C_BUS_NAME                                                "i2c0"

#ifndef bool
#define bool unsigned int
#define true (1)
#define false (0)
#endif


/* Exported types ------------------------------------------------------------*/

struct rk_vcm_ops;
struct rk_vcm_dev
{
    struct rt_device parent;
    const struct rk_vcm_ops *ops;
};

struct rk_vcm_ops
{
    rt_err_t (*init)(struct rk_vcm_dev *dev);
    rt_err_t (*open)(struct rk_vcm_dev *dev, unsigned short oflag);
    rt_err_t (*close)(struct rk_vcm_dev *dev);
    rt_err_t (*control)(struct rk_vcm_dev *dev, int cmd, void *arg);
    rt_err_t (*rx_indicate)(struct rk_vcm_dev *dev, rt_size_t size);
};

struct vcm_move_res
{
    uint32_t start_move_tv;
    uint32_t end_move_tv;
};

/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

rt_err_t rk_vcm_controller_register(struct rk_vcm_dev *vcm,
                                    const char *name,
                                    void *data);

#endif

#endif


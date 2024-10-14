/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    adapter.h
  * @version V0.0.1
  * @brief   the module is to bridge the interface for different os.
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-22     ISP Team      first implementation
  *
  ******************************************************************************
  */
#ifndef __DRV_CSI2HOST_H__
#define __DRV_CSI2HOST_H__

#if defined(__RT_THREAD__)

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include <rtconfig.h>
#include "swallow.h"

/* Exported types ------------------------------------------------------------*/

struct rk_csi2host_ops;
struct rk_csi2host_device
{
    struct rt_device dev;
    struct rk_csi2host_ops *ops;
    struct clk_gate *clkgate;
    struct clk_gate *mipi_clkgate;

    /* struct *bq_reg; */
};

struct rk_csi2host_ops
{
    rt_err_t (*init)(struct rk_csi2host_device *dev);
    rt_err_t (*open)(struct rk_csi2host_device *dev, uint16_t oflag);
    rt_err_t (*close)(struct rk_csi2host_device *dev);
    rt_err_t (*control)(struct rk_csi2host_device *dev, int cmd, void *arg);
};



#endif /* */
#endif /* __DRV_BUFFER_QUEUE_H__ */


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
#ifndef __DRV_MIPI_DPHY_H__
#define __DRV_MIPI_DPHY_H__

#if defined(__RT_THREAD__)

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include <rtconfig.h>

/* Compiler Related Definitions */
#define rk_inline rt_inline

/* redefine macro function */
#define RK_ALIGN(size, align)              RT_ALIGN(size, align)
#define rk_container_of(ptr, type, member) rt_container_of(ptr, type, member)

/* redefine system variables */
typedef rt_mutex_t rk_mutex_t;
typedef rt_sem_t rk_semaphore_t;
typedef struct rt_device rk_device;
typedef struct rt_i2c_bus_device rk_i2c_bus_device;
typedef struct clk_gate rk_clk_gate;

/* define private system variables */
typedef rt_list_t rk_list_node;
typedef rt_list_t rk_queue_list;

#define rk_list_first_entry(ptr, type, member)    rt_list_first_entry(ptr, type, member)
#define rk_list_for_each_entry(pos, head, member) rt_list_for_each_entry(pos, head, member)
#define rk_kprintf                                rt_kprintf

/* redefine the basic data type */
typedef rt_err_t ret_err_t;
typedef rt_size_t ret_size_t;
typedef rt_base_t dt_base_t;
typedef rt_ubase_t dt_ubase_t;
typedef rt_tick_t rk_tick_t;
typedef int dt_cmd_t;

#define RK_NULL          RT_NULL
#define MACRO_ASSERT(EX) RT_ASSERT(EX)

/* Exported types ------------------------------------------------------------*/
struct cam_csi_phy
{
    unsigned int grf_base;
    unsigned int csi_base;
    unsigned int data_en_bit;        // data lane enable bit;
    unsigned int bit_rate;           // Mbps/lane
    unsigned int phy_index;          // phy0,phy1
};

struct rk_mipi_dphy_ops;
struct rk_mipi_dphy_device
{
    rk_device dev;
    const struct rk_mipi_dphy_ops *ops;
    /* struct HAL_VIP_REG *vipReg; */
    struct cam_csi_phy csi_phy[2];
    struct clk_gate *clkgate;
};

struct rk_mipi_dphy_ops
{
    ret_err_t (*init)(struct rk_mipi_dphy_device *dev);
    ret_err_t (*open)(struct rk_mipi_dphy_device *dev, uint16_t oflag);
    ret_err_t (*close)(struct rk_mipi_dphy_device *dev);
    ret_err_t (*control)(struct rk_mipi_dphy_device *dev, dt_cmd_t cmd, void *arg);
};

#endif /* */
#endif /* __MIPI_DPHY_H__ */

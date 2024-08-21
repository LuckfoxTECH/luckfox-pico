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
#ifndef __DRV_VICAP_LITE_H__
#define __DRV_VICAP_LITE_H__

#if defined(__RT_THREAD__)

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"
#include "drv_clock.h"
#include "swallow.h"

#ifdef RT_USING_VICAP_LITE
#define VICAP_LITE_MODULE_COMPILED
#else
#undef VICAP_LITE_MODULE_COMPILED
#endif

enum vicap_lite_mipi_id
{
    VICAP_LITE_MIPI_ID0,
    VICAP_LITE_MIPI_ID1,
    VICAP_LITE_MIPI_ID2,
    VICAP_LITE_MIPI_ID3,
};

enum vicap_lite_cmd
{
    VICAP_LITE_ENABLE,
    VICAP_LITE_DISABLE
};

struct rk_vicap_lite_cfg
{
    enum vicap_lite_mipi_id mipi_id;
    unsigned short width;
    unsigned short height;
    unsigned short vc;                  /* virtual channel */
    unsigned short dt;                  /* data type */
    unsigned int addr_y[2];
    unsigned int addr_uv[2];
    unsigned int stride_y;              /* should align to double word (8byte) */
    unsigned int stride_uv;             /* should align to double word (8byte) */
};

struct vicap_lite_clock_info
{
    struct clk_gate *vicap_lite_dclk;
    struct clk_gate *vicap_lite_hclk;
    struct clk_gate *vicap_lite_aclk;
};


/* Exported types ------------------------------------------------------------*/
struct rk_vicap_lite_ops;
struct rk_vicap_lite_device
{
    struct rt_device dev;
    const struct rk_vicap_lite_ops *ops;
    /* struct HAL_VIP_REG *vipReg; */
    struct VICAP_LITE_REG *p_reg;
    struct CRU_REG *cru_reg;
    struct vicap_lite_clock_info clk;

    int irq;
    rt_isr_handler_t vicap_lite_isr;
};

struct rk_vicap_lite_ops
{
    rt_err_t (*init)(struct rk_vicap_lite_device *dev);
    rt_err_t (*open)(struct rk_vicap_lite_device *dev, uint16_t oflag);
    rt_err_t (*close)(struct rk_vicap_lite_device *dev);
    rt_err_t (*control)(struct rk_vicap_lite_device *dev, int cmd, void *arg);
    rt_err_t (*rx_indicate)(struct rk_vicap_lite_device *dev, rt_size_t size);
};



#endif /* */
#endif /* __VICAP_LITE_H__ */


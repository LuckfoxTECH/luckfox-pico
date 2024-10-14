/**
  * Copyright (c) 2018 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    dsp.h
  * @author  Huaping Liao
  * @version V0.1
  * @date    20-Mar-2019
  * @brief   general dsp framework driver for pisces
  *
  ******************************************************************************
  */

#ifndef __DSP_H__
#define __DSP_H__

/* Includes ------------------------------------------------------------------*/

#include <rtthread.h>

/* Exported macro ------------------------------------------------------------*/

#define RT_DSP_CTL_ON                    (1)
#define RT_DSP_CTL_OFF                   (2)
#define RT_DSP_CTL_SUSPEND               (3)
#define RT_DSP_CTL_RESUME                (4)
#define RT_DSP_CTL_SHUTDOWN              (5)
#define RT_DSP_CTL_WAKEUP                (6)
#define RT_DSP_CTL_CONTROL               (7)

/* Exported types ------------------------------------------------------------*/

struct rt_dsp_ops;
struct rt_dsp_device
{
    struct rt_device parent;
    const struct rt_dsp_ops *ops;
};

struct rt_dsp_ops
{
    rt_err_t (*open)(struct rt_dsp_device *dsp, rt_uint16_t oflag);
    rt_err_t (*close)(struct rt_dsp_device *dsp);
    rt_err_t (*on)(struct rt_dsp_device *dsp);
    rt_err_t (*off)(struct rt_dsp_device *dsp);
    rt_err_t (*suspend)(struct rt_dsp_device *dsp);
    rt_err_t (*resume)(struct rt_dsp_device *dsp);
    rt_err_t (*shutdown)(struct rt_dsp_device *dsp);
    rt_err_t (*wakeup)(struct rt_dsp_device *dsp);
    rt_err_t (*control)(struct rt_dsp_device *dsp, int cmd, void *arg);
};

typedef void (*dsp_callback)(void *cparam);

struct rt_dsp_transfer
{
    struct rt_dsp_device *dsp_dev; /* dsp dev */
    dsp_callback callback; /* dsp xfer complete callback func */
    void *cparam; /* callback param */
};

/* Exported constants --------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

rt_err_t rt_hw_dsp_register(struct rt_dsp_device *dsp,
                            const char *name,
                            rt_uint32_t flag,
                            void *data);
#endif

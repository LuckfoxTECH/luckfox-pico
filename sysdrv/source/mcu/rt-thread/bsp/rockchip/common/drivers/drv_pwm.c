/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pwm.c
  * @author  David Wu
  * @version V0.1
  * @date    26-Apri-2019
  * @brief   PWM Driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup PWM
 *  @{
 */

/** @defgroup PWM_How_To_Use How To Use
 *  @{

  See more information, click [here](https://www.rt-thread.org/document/site/programming-manual/device/pwm/pwm/)

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#if defined(RT_USING_PWM)

#include "hal_bsp.h"
#include "drv_clock.h"
#include "hal_base.h"

#define RK_PWM_DEBUG 0
#if RK_PWM_DEBUG
#define rk_pwm_dbg(dev, fmt, ...) \
do { \
    rt_kprintf("%s:", ((struct rt_device *)dev)->parent.name); \
    rt_kprintf(fmt, ##__VA_ARGS__); \
} while(0)
#else
#define rk_pwm_dbg(dev, fmt, ...) \
do { \
} while(0)
#endif

/* PWM controller driver's private data. */
struct rockchip_pwm
{
    struct rt_device_pwm dev;
    const char *name;
    /* Hal */
    struct PWM_HANDLE instance;
    const struct HAL_PWM_DEV *hal_dev;
    struct clk_gate *clk_gate;
    struct clk_gate *pclk_gate;
};

#define DEFINE_ROCKCHIP_PWM(ID)      \
static struct rockchip_pwm pwm##ID = \
{                                    \
    .hal_dev = &g_pwm##ID##Dev,      \
    .name = "pwm"#ID,                \
};

#ifdef RT_USING_PWM0
DEFINE_ROCKCHIP_PWM(0)
#endif

#ifdef RT_USING_PWM1
DEFINE_ROCKCHIP_PWM(1)
#endif

#ifdef RT_USING_PWM2
DEFINE_ROCKCHIP_PWM(2)
#endif

static struct rockchip_pwm *const rockchip_pwm_table[] =
{
#ifdef RT_USING_PWM0
    &pwm0,
#endif
#ifdef RT_USING_PWM1
    &pwm1,
#endif
#ifdef RT_USING_PWM2
    &pwm2,
#endif
    RT_NULL
};

static rt_err_t rockchip_pwm_oneshot(struct rockchip_pwm *pwm, rt_uint32_t channel,
                                     struct rt_pwm_oneshot *config)
{
    struct PWM_HANDLE *pPWM = &pwm->instance;

    return HAL_PWM_SetOneshot(pPWM, channel, config->count);
}

static rt_err_t rockchip_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *config = (struct rt_pwm_configuration *)arg;
    struct rockchip_pwm *pwm = (struct rockchip_pwm *)device->parent.user_data;
    struct PWM_HANDLE *pPWM = &pwm->instance;
    struct HAL_PWM_CONFIG hal_config;
    rt_err_t ret = RT_EOK;

    rk_pwm_dbg(device, "cmd: %d\n", cmd);

    clk_enable(pwm->pclk_gate);

    switch (cmd)
    {
    case PWM_CMD_ENABLE:
        clk_enable(pwm->clk_gate);
        ret = HAL_PWM_Enable(pPWM, config->channel, HAL_PWM_CONTINUOUS);
        break;
    case PWM_CMD_DISABLE:
        clk_disable(pwm->clk_gate);
        ret = HAL_PWM_Disable(pPWM, config->channel);
        break;
    case PWM_CMD_SET_ONESHOT:
        ret = rockchip_pwm_oneshot(pwm, config->channel, (struct rt_pwm_oneshot *)arg);
        if (ret)
            break;
    case PWM_CMD_SET:
        hal_config.channel = config->channel;
        hal_config.periodNS = config->period;
        hal_config.dutyNS = config->pulse;
        hal_config.polarity = config->polarity;
        ret = HAL_PWM_SetConfig(pPWM, config->channel, &hal_config);
        break;
    default:
        break;
    }

    clk_disable(pwm->pclk_gate);
    return ret;
}

static struct rt_pwm_ops rockchip_pwm_ops =
{
    rockchip_pwm_control
};

int rockchip_rt_hw_pwm_init(void)
{
    struct rockchip_pwm *const *pwm;
    rt_uint32_t freq;
    const struct HAL_PWM_DEV *hal_dev;

    for (pwm = rockchip_pwm_table; *pwm != RT_NULL; pwm++)
    {
        hal_dev = (*pwm)->hal_dev;
        (*pwm)->clk_gate = get_clk_gate_from_id(hal_dev->clkGateID);
        (*pwm)->pclk_gate = get_clk_gate_from_id(hal_dev->pclkGateID);

        RT_ASSERT((*pwm)->clk_gate != RT_NULL);
        RT_ASSERT((*pwm)->pclk_gate != RT_NULL);

        clk_enable((*pwm)->pclk_gate);
        /* Get clock rate. */
        freq = clk_get_rate(hal_dev->clkID);
        HAL_PWM_Init(&(*pwm)->instance, hal_dev->pReg, freq);

        clk_disable((*pwm)->pclk_gate);
        /* register pwm */
        rt_device_pwm_register(&(*pwm)->dev, (*pwm)->name, &rockchip_pwm_ops, *pwm);
    }

    return 0;
}

INIT_DEVICE_EXPORT(rockchip_rt_hw_pwm_init);

#endif

/** @} */  // PWM

/** @} */  // RKBSP_Driver_Reference

/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pwm_regulator.h
  * @version V0.1
  * @brief   pwm interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-12-14     shengfei Xu      first implementation
  * 2020-12-18     Elaine.Zhang    second implementation
  *
  ******************************************************************************
  */

#ifndef __DRV_PWM_REGULATOR_H__
#define __DRV_PWM_REGULATOR_H__

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include "hal_base.h"
#include "hal_def.h"

struct pwr_pwm_info_desc
{
    char *name;
    uint32_t chanel;
    bool invert;
};

struct pwr_pwm_desc
{
    struct PWR_CTRL_INFO info;
    uint16_t flag;
    uint16_t pwrId;
    uint32_t period;
    uint32_t minVolt;
    uint32_t maxVlot;
    uint32_t voltage;
    struct pwr_pwm_info_desc *pwm;
};

#ifdef RT_USING_PWM_REGULATOR

rt_err_t pwm_regulator_set_voltage(struct pwr_pwm_desc *desc,
                                   rt_uint32_t voltUv);
rt_err_t pwm_regulator_get_voltage(struct pwr_pwm_desc *desc);
rt_err_t pwm_regulator_check_desc_by_pwrId(struct pwr_pwm_desc *pdesc,
        ePWR_ID pwrId);
#endif
#endif

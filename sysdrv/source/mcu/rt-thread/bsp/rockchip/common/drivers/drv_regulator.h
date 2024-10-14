/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_regulator.h
  * @version V0.1
  * @brief   regulator interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-01     Tony.Xie        first implementation
  * 2019-07-11     Elaine.Zhang    second implementation
  *
  ******************************************************************************
  */

#ifndef _DRV_REGULATOR_H_
#define _DRV_REGULATOR_H_

#include "hal_base.h"
#include "drv_pmic.h"
#include "drv_pwm_regulator.h"

typedef rt_mutex_t regulator_mutex_t;

#define REGULATOR_FLG_INTREG    (1 << 0)
#define REGULATOR_FLG_I2C8      (REGULATOR_FLG_INTREG << 1)
#define REGULATOR_FLG_I2C16     (REGULATOR_FLG_I2C8 << 1)
#define REGULATOR_FLG_PWM       (REGULATOR_FLG_I2C16 << 1)
#define REGULATOR_FLG_LOCK      (REGULATOR_FLG_PWM << 1)

#define REGULATOR_INIT(NAME, ID, VOLTUV, EN, SSPDVOL, SSPDEN)        \
    { .name = NAME, .pwrId = ID, .init_voltage = VOLTUV, .enable = EN, .suspend_voltage = SSPDVOL, .suspend_enable = SSPDEN,}

union U_PWR_REG_DESC
{
    struct PWR_INTREG_DESC intreg_desc;
    struct pwr_i2cbus_desc i2c_desc;
    struct pwr_pwm_desc pwm_desc;
};

struct regulator_desc
{
    uint8_t flag;
    union U_PWR_REG_DESC desc;
    regulator_mutex_t lock;
    uint16_t setup_fixed; /* uS, is a fixed val for delay */
    uint16_t setup_step; /* mV/uS */
    uint32_t currentVolt;
};

struct regulator_init
{
    const char *name;
    ePWR_ID pwrId;
    uint32_t init_voltage;
    uint32_t suspend_voltage;
    uint8_t enable : 4;
    uint8_t suspend_enable : 4;
};

#ifdef HAL_PWR_MODULE_ENABLED
struct regulator_desc *regulator_get_desc_by_pwrid(ePWR_ID pwrId);
rt_err_t regulator_set_voltage(struct regulator_desc *desc, int volt);
uint32_t regulator_get_voltage(struct regulator_desc *desc);
rt_err_t regulator_set_suspend_voltage(struct regulator_desc *desc, int volt);
uint32_t regulator_get_suspend_voltage(struct regulator_desc *desc);
uint32_t regulator_get_real_voltage(struct regulator_desc *desc);
rt_err_t regulator_enable(struct regulator_desc *desc);
rt_err_t regulator_disable(struct regulator_desc *desc);
void regulator_desc_init(struct regulator_desc *descs);
#endif
#endif


/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pwm_regulator.c
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

/** @addtogroup RKBSP_Driver_Reference
*  @{
*/

/** @addtogroup Pwm regulator
 *  @{
 */

/** @defgroup Pmic_How_To_Use How To Use
 *  @{

The pwm regulator driver use to configure dcdc voltage, it can be used in the following three scenarios:

- **Configure voltage**:
    - The device set voltage by pwm_regulator_set_voltage(struct pwr_pwm_desc *desc, rt_uint32_t voltUv);
    - The device get voltage by pwm_regulator_get_voltage(struct pwr_pwm_desc *desc);

 @} */

#include <rtdevice.h>
#include <rtthread.h>
#ifdef RT_USING_PWM_REGULATOR
#include <stdbool.h>

#include "board.h"
#include "drv_regulator.h"
#include "drv_pwm_regulator.h"

/********************* Private Function Definition ***************************/
/** @defgroup Pwm_Regulator_Private_Function Private Function
 *  @{
 */

/**
 * @brief  pwm regulator setting voltage.
 * @param desc: the power regulator description pointer.
 * @param voltUv: the volt value to be config(uV)
 * @retval -RT_ERROR: pwm set failed
 * @retval RT_EOK: pwm set successfully
 */
rt_err_t pwm_regulator_set_voltage(struct pwr_pwm_desc *desc,
                                   rt_uint32_t voltUv)
{
    struct pwr_pwm_info_desc *pwm_info = desc->pwm;
    struct rt_device_pwm *pwm_dev;
    uint32_t minVolt = desc->minVolt;
    uint32_t maxVlot = desc->maxVlot;
    rt_uint32_t period, pulse;
    rt_uint32_t ret;

    pwm_dev = (struct rt_device_pwm *)rt_device_find(pwm_info->name);
    if (pwm_dev == RT_NULL)
    {
        rt_kprintf("%s failed! can't find %s device!\n", __func__, pwm_info->name);
        return -RT_EIO;
    }

    period = desc->period;
    pulse = (voltUv - minVolt) * period / (maxVlot - minVolt);
    if (pwm_info->invert)
        pulse = period - pulse;

    ret = rt_pwm_set(pwm_dev, pwm_info->chanel, period, pulse);
    if (ret < 0)
    {
        rt_kprintf("%s pwm set failed! set voltage fail\n", __func__);
        return ret;
    }
    rt_pwm_enable(pwm_dev, pwm_info->chanel);
    if (ret < 0)
    {
        rt_kprintf("%s pwm enable failed! set voltage fail\n", __func__);
        return ret;
    }
    desc->voltage = voltUv;

    return ret;
}

/**
 * @brief  pwm regulator getting voltage.
 * @param desc: the power regulator description pointer.
 * @retval desc->voltage: the last voltage successfully set(UV)
 */
rt_err_t pwm_regulator_get_voltage(struct pwr_pwm_desc *desc)
{
    return desc->voltage;
}

/**
 * @brief  get pwm regulator output status.
 * @param pdesc: the power regulator description pointer.
 * @param pwrId: the id of the ePWR_ID type
 * @retval RT_EOK:  the ID matches the desc
 * @retval -RT_EINVAL: struct desc is invalid argument
 */
rt_err_t pwm_regulator_check_desc_by_pwrId(struct pwr_pwm_desc *pdesc,
        ePWR_ID pwrId)
{
    if (pdesc->pwrId == pwrId)
        return RT_EOK;
    else
        return -RT_EINVAL;
}

/** @} */

#endif

/** @} */
/** @} */

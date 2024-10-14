/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_regulator.c
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

/** @addtogroup RKBSP_Driver_Reference
*  @{
*/

/** @addtogroup Regulator
 *  @{
 */

/** @defgroup Regulator_How_To_Use How To Use
 *  @{

The Pmic driver use to configure dcdc or ldo voltage, enable/disable voltage output,
 it can be used in the following three scenarios:

- **Configure voltage**:
    - The device get desc pointer by struct regulator_desc *regulator_get_desc_by_pwrid(ePWR_ID pwr_id);
    - The device set voltage by int regulator_set_voltage(struct regulator_desc *descs, int volt)
    - The device get voltage by regulator_get_voltage(struct regulator_desc *descs);

- **Configure suspend voltage**:
    - The device get desc pointer by struct regulator_desc *regulator_get_desc_by_pwrid(ePWR_ID pwr_id);
    - The device set voltage by int regulator_set_suspend_voltage(struct regulator_desc *descs, int volt)
    - The device get voltage by regulator_get_suspend_voltage(struct regulator_desc *descs);

- **Enable/disable voltage output**:
    - The device get desc pointer by struct regulator_desc *regulator_get_desc_by_pwrid(ePWR_ID pwr_id);
    - The device Enable output regulator_enable(struct regulator_desc *descs);
    - The device Disable output regulator_disable(struct regulator_desc *descs);

 @} */

#include <rthw.h>
#include <rtthread.h>
#include "drv_regulator.h"
#include "drivers/i2c.h"
#include "drv_pwm_regulator.h"

#ifdef HAL_PWR_MODULE_ENABLED

#define REGULATOR_CHK_INTREG(desc)   ((desc)->flag & REGULATOR_FLG_INTREG)
#define REGULATOR_CHK_I2C8(desc)   ((desc)->flag & REGULATOR_FLG_I2C8)
#define REGULATOR_CHK_PWM(desc) ((desc)->flag & REGULATOR_FLG_PWM)
#define REGULATOR_CHK_LOCK(desc)   ((desc)->flag & REGULATOR_FLG_LOCK)

static struct
{
    struct regulator_desc *descs;
}  pwr_descs;

/********************* Private Function Definition ***************************/
/** @defgroup Regulator_Private_Function Private Function
 *  @{
 */

/**
 * @brief Create a new regulator mutex
 * @param descs: the power regulator description pointer.
 * @param num: count num.
 * @retval RT_EOK:  mutex create successfully
 * @retval RT_ERROR: mutex create failed
 */
static rt_err_t regulator_mutex_new(struct regulator_desc *descs, int num)
{
    char lockName[RT_NAME_MAX];
    regulator_mutex_t tmpmutex;

    if (REGULATOR_CHK_LOCK(descs))
    {
        rt_snprintf(lockName, RT_NAME_MAX, "%s%d", "lock", num);
        tmpmutex = rt_mutex_create(lockName, RT_IPC_FLAG_FIFO);
        if (tmpmutex == RT_NULL)
        {
            return -RT_ERROR;
        }
        else
        {
            descs->lock = tmpmutex;
            return RT_EOK;
        }
    }
    return RT_EOK;
}

/**
 * @brief regulator mutex take.
 * @param descs: the power regulator description pointer.
 * @retval RT_EOK:  mutex take successfully
 * @retval RT_ERROR: mutex take failed
 */
static rt_err_t regulator_mutex_take(struct regulator_desc *descs)
{
    int ret = 0;

    if (REGULATOR_CHK_LOCK(descs))
        ret = rt_mutex_take(descs->lock, RT_WAITING_FOREVER);

    return ret;
}

/**
 * @brief regulator mutex release.
 * @param descs: the power regulator description pointer.
 * @retval RT_EOK:  mutex release successfully
 * @retval RT_ERROR: mutex release failed
 */
static rt_err_t regulator_mutex_release(struct regulator_desc *descs)
{
    int ret = 0;

    if (REGULATOR_CHK_LOCK(descs))
        ret = rt_mutex_release(descs->lock);

    return ret;
}

static int __regulator_get_voltage(struct regulator_desc *descs)
{
    int ret = RT_ERROR;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
        ret = HAL_PWR_GetVoltage(&descs->desc.intreg_desc);
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_get_voltage(&descs->desc.i2c_desc);
#endif
#ifdef RT_USING_PWM_REGULATOR
    if (REGULATOR_CHK_PWM(descs))
        ret = pwm_regulator_get_voltage(&descs->desc.pwm_desc);
#endif

    descs->currentVolt = ret;
    regulator_mutex_release(descs);
    return ret;
}

static rt_err_t __regulator_set_voltage(struct regulator_desc *descs, int volt)
{
#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
    {
        HAL_Status ret;
        ret = HAL_PWR_SetVoltage(&descs->desc.intreg_desc, volt);
        if (ret == HAL_OK)
            return RT_EOK;
        else if (ret == HAL_INVAL)
            return -RT_EINVAL;
        else
            return -RT_ERROR;
    }
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
    {
        return pmic_set_voltage(&descs->desc.i2c_desc, volt);
    }
#endif

#ifdef RT_USING_PWM_REGULATOR
    if (REGULATOR_CHK_PWM(descs))
        return pwm_regulator_set_voltage(&descs->desc.pwm_desc, volt);
#endif

    return 0;
}

static rt_err_t __regulator_set_suspend_voltage(struct regulator_desc *descs, int volt)
{
#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
    {
        HAL_Status ret;
        ret = HAL_PWR_SetVoltageSuspend(&descs->desc.intreg_desc, volt);
        if (ret == HAL_OK)
            return RT_EOK;
        else if (ret == HAL_INVAL)
            return -RT_EINVAL;
        else
            return -RT_ERROR;
    }
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        return pmic_set_suspend_voltage(&descs->desc.i2c_desc, volt);
#endif
    return 0;
}

static rt_err_t __regulator_round_voltage(struct regulator_desc *descs, int volt)
{
#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
    {
        return HAL_PWR_RoundVoltage(&descs->desc.intreg_desc, volt);
    }
#endif

    return volt;
}

static void regulator_set_voltage_delay(struct regulator_desc *descs, uint32_t old_volt, uint32_t new_volt)
{
    uint16_t setup_time = 0;

    if (old_volt > new_volt)
        return;

    if (descs->setup_step)
    {
        setup_time = (new_volt - old_volt) / descs->setup_step;
    }
    else if (descs->setup_fixed)
    {
        setup_time = descs->setup_fixed;
    }
    else
    {
        return;
    }

    HAL_DelayUs(setup_time);
}

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup Regulator_Public_Functions Public Functions
 *  @{
 */

/**
* @brief This function is for initializing a base address of
*  the regulator_desc array in system.
* @param descs: a base address of the regulator_desc array.
*/
void regulator_desc_init(struct regulator_desc *descs)
{
    RT_ASSERT(descs != RT_NULL);
    pwr_descs.descs = descs;
}

/**
 * @brief get a power regulator description by power id.
 * @param  pwr_id: a regulator channel id.
 * @return a pointer for struct regulator_desc.
 */
struct regulator_desc *regulator_get_desc_by_pwrid(ePWR_ID pwr_id)
{
    struct regulator_desc *descs = pwr_descs.descs;

    RT_ASSERT(pwr_descs.descs != RT_NULL);

    while (descs->flag)
    {
#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
        if (REGULATOR_CHK_INTREG(descs))
        {
            if (HAL_PWR_CheckDescByPwrId(&descs->desc.intreg_desc, pwr_id))
                return descs;
        }
#endif
#ifdef HAL_PWR_I2C8_MODULE_ENABLED
        if (REGULATOR_CHK_I2C8(descs))
        {
            if (!pmic_check_desc_by_pwrId(&descs->desc.i2c_desc, pwr_id))
                return descs;
        }
#endif
#ifdef RT_USING_PWM_REGULATOR
        if (REGULATOR_CHK_PWM(descs))
        {
            if (!pwm_regulator_check_desc_by_pwrId(&descs->desc.pwm_desc, pwr_id))
                return descs;
        }
#endif
        descs++;
    }

    return NULL;
}

/**
 * @brief regulator getting voltage.
 * @param descs: the power regulator description pointer.
 * @return the voltage value (UV)
 */
uint32_t regulator_get_voltage(struct regulator_desc *descs)
{
    uint32_t ret = 0;

    if (descs->currentVolt)
        ret = descs->currentVolt;
    else
        ret = __regulator_get_voltage(descs);
    return ret;
}

/**
 * @brief regulator setting voltage.
 * @param descs: the power regulator description pointer.
 * @param volt: the volt value to be config(uV)
 * @retval -RT_ERROR: set voltage failed
 * @retval RT_EOK: set voltage successfully
 */
rt_err_t regulator_set_voltage(struct regulator_desc *descs, int volt)
{
    int ret;
    uint32_t old_volt = regulator_get_voltage(descs);
    uint32_t new_volt;

    if (old_volt == volt)
        return RT_EOK;

    regulator_mutex_take(descs);

    ret = __regulator_set_voltage(descs, volt);

    if (ret == RT_EOK)
    {
        descs->currentVolt = volt;
    }
    else if (ret == -RT_EINVAL)
    {
        new_volt = __regulator_round_voltage(descs, volt);
        if (new_volt)
        {
            ret = __regulator_set_voltage(descs, new_volt);
            if (ret == RT_EOK)
                descs->currentVolt = new_volt;
        }
    }

    regulator_mutex_release(descs);

    if (ret == RT_EOK)
        regulator_set_voltage_delay(descs, old_volt, descs->currentVolt);
    return ret;
}

/**
 * @brief regulator getting suspend voltage.
 * @param descs: the power regulator description pointer.
 * @return the voltage value (UV)
 */
uint32_t regulator_get_suspend_voltage(struct regulator_desc *descs)
{
    uint32_t ret = 0;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
        ret = HAL_PWR_GetVoltageSuspend(&descs->desc.intreg_desc);
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_get_suspend_voltage(&descs->desc.i2c_desc);
#endif
    regulator_mutex_release(descs);
    return ret;
}

/**
 * @brief regulator setting suspend voltage.
 * @param descs: the power regulator description pointer.
 * @param volt: the volt value to be config(uV)
 * @retval -RT_ERROR: set suspend voltage failed
 * @retval RT_EOK: set suspend voltage successfully
 */
rt_err_t regulator_set_suspend_voltage(struct regulator_desc *descs, int volt)
{
    int ret = 0;
    uint32_t new_volt, old_volt = regulator_get_suspend_voltage(descs);

    if (old_volt == volt)
        return ret;

    regulator_mutex_take(descs);

    ret = __regulator_set_suspend_voltage(descs, volt);

    if (ret == -RT_EINVAL)
    {
        new_volt = __regulator_round_voltage(descs, volt);
        if (new_volt)
        {
            ret = __regulator_set_suspend_voltage(descs, new_volt);
        }
    }

    regulator_mutex_release(descs);

    return ret;
}

/**
 * @brief regulator getting real voltage.
 * @param descs: the power regulator description pointer.
 * @return the voltage value (UV)
 */
uint32_t regulator_get_real_voltage(struct regulator_desc *descs)
{
    uint32_t ret = 0;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
        ret = HAL_PWR_GetVoltageReal(&descs->desc.intreg_desc);
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_get_voltage(&descs->desc.i2c_desc);
#endif
    regulator_mutex_release(descs);

    return ret;
}

/**
 * @brief regulator enable output.
 * @param descs: the power regulator description pointer.
 * @return the voltage value (UV)
 * @retval -RT_ERROR: enable output failed
 * @retval RT_EOK: enable output successfully
 */
rt_err_t regulator_enable(struct regulator_desc *descs)
{
    int ret = 0;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
        ret = HAL_PWR_Enable(&descs->desc.intreg_desc);
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_set_enable(&descs->desc.i2c_desc, 1);
#endif
    regulator_mutex_release(descs);

    if (ret < 0)
        return -RT_ERROR;
    else
        return RT_EOK;
}

/**
 * @brief regulator dsiable output.
 * @param descs: the power regulator description pointer.
 * @return the voltage value (UV)
 * @retval -RT_ERROR: disable output failed
 * @retval RT_EOK: disable output successfully
 */
rt_err_t regulator_disable(struct regulator_desc *descs)
{
    int ret = 0;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_INTBUS_MODULE_ENABLED
    if (REGULATOR_CHK_INTREG(descs))
        ret = HAL_PWR_Disable(&descs->desc.intreg_desc);
#endif

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_set_enable(&descs->desc.i2c_desc, 0);
#endif
    regulator_mutex_release(descs);

    if (ret < 0)
        return -RT_ERROR;
    else
        return RT_EOK;
}



/**
 * @brief regulator suspend enable output.
 * @param descs: the power regulator description pointer.
 * @retval -RT_ERROR: enable output failed
 * @retval RT_EOK: enable output successfully
 */
rt_err_t regulator_suspend_enable(struct regulator_desc *descs)
{
    int ret = 0;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_set_suspend_enable(&descs->desc.i2c_desc, 1);
#endif
    regulator_mutex_release(descs);

    if (ret < 0)
        return -RT_ERROR;
    else
        return RT_EOK;
}

/**
 * @brief regulator suspend dsiable output.
 * @param descs: the power regulator description pointer.
 * @retval -RT_ERROR: disable output failed
 * @retval RT_EOK: disable output successfully
 */
rt_err_t regulator_suspend_disable(struct regulator_desc *descs)
{
    int ret = 0;

    regulator_mutex_take(descs);

#ifdef HAL_PWR_I2C8_MODULE_ENABLED
    if (REGULATOR_CHK_I2C8(descs))
        ret = pmic_set_suspend_enable(&descs->desc.i2c_desc, 0);
#endif
    regulator_mutex_release(descs);

    if (ret < 0)
        return -RT_ERROR;
    else
        return RT_EOK;
}

/**
 * @brief  regulator init.
 * @return RT_EOK
 */
int regulator_init(void)
{
    int i = 0;
    struct regulator_desc *descs = pwr_descs.descs;

    while (descs->flag)
    {
#ifdef HAL_PWR_I2C8_MODULE_ENABLED
        if (REGULATOR_CHK_I2C8(descs) && (descs->desc.i2c_desc.i2c->bus == RT_NULL))
        {
            descs->desc.i2c_desc.i2c->bus = (struct rt_i2c_bus_device *)rt_i2c_bus_device_find(descs->desc.i2c_desc.i2c->name);
        }
#endif
        regulator_mutex_new(descs, i);
        descs++;
        i++;
    }
    return RT_EOK;
}
INIT_PREV_EXPORT(regulator_init);

RT_WEAK const struct regulator_init regulator_inits[] = {};
#ifdef RT_USING_PMIC
RT_WEAK const ePMIC_SleepPinFunc g_pmic_sleep_pin_func = 0;
RT_WEAK const ePMIC_SleepPinPolarity g_pmic_sleep_pin_polarity = 0;
#endif

/**
 * @brief  regulator init volatge.
 * @return 0
 */
int regulator_set_auto_by_default(void)
{
    const struct regulator_init *inits = regulator_inits;
    struct regulator_desc *desc;
#ifdef RT_USING_PMIC
    int pin_set = 0;
#endif

    if (inits == NULL)
        return 0;

    while (inits->pwrId)
    {
        desc = regulator_get_desc_by_pwrid(inits->pwrId);
        if (desc)
        {
            if (inits->init_voltage)
            {
                regulator_set_voltage(desc, inits->init_voltage);
            }
            if (!inits->enable)
            {
                regulator_disable(desc);
            }
            else
            {
                regulator_enable(desc);
            }
            if (inits->suspend_voltage)
            {
                regulator_set_suspend_voltage(desc, inits->init_voltage);
            }
            if (!inits->suspend_enable)
            {
                regulator_suspend_disable(desc);
#ifdef RT_USING_PMIC
                if (REGULATOR_CHK_I2C8(desc) && !pin_set)
                {
                    pin_set = 1;
                    pmic_sleep_pin_func(&desc->desc.i2c_desc, g_pmic_sleep_pin_polarity, g_pmic_sleep_pin_func);
                }
#endif
            }
            else
            {
                regulator_suspend_enable(desc);
            }
        }
        inits++;
    }

    return 0;
}
INIT_DEVICE_EXPORT(regulator_set_auto_by_default);

#if defined(RT_USING_REGULATOR_DUMP) && defined(RT_USING_FINSH)
/**
 * @brief  regulator dump voltage, used for debug.
 */
static void regulator_dump(void)
{
    const struct regulator_init *inits = regulator_inits;
    struct regulator_desc *desc;

    if (inits == NULL)
        return;

    while (inits->pwrId)
    {
        desc = regulator_get_desc_by_pwrid(inits->pwrId);
        if (desc)
        {
            rt_kprintf("%s: %s = %d\n", __func__, inits->name, regulator_get_voltage(desc));
        }
        inits++;
    }
}

#include <finsh.h>
MSH_CMD_EXPORT(regulator_dump, regulator drive dump. e.g: regulator_dump);
#endif

/** @} */

#endif

/** @} */
/** @} */

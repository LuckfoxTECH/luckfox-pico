/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pmic.c
  * @version V0.1
  * @brief   pmic interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-11     Elaine.Zhang      first implementation
  * 2019-12-26     Joseph Chen       export register access API
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
*  @{
*/

/** @addtogroup Pmic
 *  @{
 */

/** @defgroup Pmic_How_To_Use How To Use
 *  @{

The Pmic driver use to configure dcdc or ldo voltage, enable/disable voltage output,
 it can be used in the following three scenarios:

- **Access pmic register**:
    - Read register by pmic_read(struct rt_i2c_bus_device *pmic_i2c_bus, rt_uint8_t addr, rt_uint16_t reg);
    - Write register by pmic_write(struct rt_i2c_bus_device *pmic_i2c_bus, rt_uint8_t addr, rt_uint16_t reg, rt_uint8_t data);
    - Update register fields by pmic_update_bits(struct rt_i2c_bus_device *pmic_i2c_bus, rt_uint8_t addr, rt_uint16_t reg, rt_uint8_t mask, rt_uint8_t data);

- **Configure voltage**:
    - The device set voltage by pmic_set_voltage(struct pwr_i2cbus_desc *desc, rt_uint32_t voltUv);
    - The device get voltage by pmic_get_voltage(struct pwr_i2cbus_desc *desc);

- **Configure suspend voltage**:
    - The device set voltage by pmic_set_suspend_voltage(struct pwr_i2cbus_desc *desc, rt_uint32_t voltUv);
    - The device get voltage by pmic_get_suspend_voltage(struct pwr_i2cbus_desc *desc);

- **Enable/disable voltage output**:
    - The device Enable/disable output by pmic_set_enable(struct pwr_i2cbus_desc *desc, rt_uint32_t enable);
    - The device get the output status by pmic_is_enabled(struct pwr_i2cbus_desc *desc);

 @} */

#include <rtdevice.h>
#include <rtthread.h>
#ifdef RT_USING_PMIC

#include "drv_pmic.h"
#include "drivers/i2c.h"
#ifdef RT_USING_PMIC_RK816
#include "rk816_interrupt.h"
#endif

#define LNR_VOLT_BG(volt, min, step) (((volt - min) + (step -1)) / (step))

#ifndef PMIC_PLUG_IN_HOOK_LIST_SIZE
#define PMIC_PLUG_IN_HOOK_LIST_SIZE          2
#endif
#ifndef PMIC_PLUG_OUT_HOOK_LIST_SIZE
#define PMIC_PLUG_OUT_HOOK_LIST_SIZE          2
#endif
#ifndef PMIC_POWERON_RISE_HOOK_LIST_SIZE
#define PMIC_POWERON_RISE_HOOK_LIST_SIZE          4
#endif
#ifndef PMIC_POWERON_FALL_HOOK_LIST_SIZE
#define PMIC_POWERON_FALL_HOOK_LIST_SIZE          4
#endif
#ifndef PMIC_POWERON_LONG_HOOK_LIST_SIZE
#define PMIC_POWERON_LONG_HOOK_LIST_SIZE          4
#endif

void (*pmic_plug_in_hook_list[PMIC_PLUG_IN_HOOK_LIST_SIZE])();
void (*pmic_plug_out_hook_list[PMIC_PLUG_OUT_HOOK_LIST_SIZE])();
void (*pmic_poweron_rise_hook_list[PMIC_POWERON_RISE_HOOK_LIST_SIZE])();
void (*pmic_poweron_fall_hook_list[PMIC_POWERON_FALL_HOOK_LIST_SIZE])();
void (*pmic_poweron_long_hook_list[PMIC_POWERON_LONG_HOOK_LIST_SIZE])();

RT_WEAK struct pwr_i2c_desc pmic_pwr_i2c_desc = {};

/********************* Private Function Definition ***************************/
/** @defgroup Pmic_Private_Function Private Function
 *  @{
 */

/**
 * @brief  pmic i2c read.
 * @param pmic_i2c_bus: the i2c bus device pointer.
 * @retval RT_EOK: rt_i2c_transfer successfully
 * @retval RT_ERROR: rt_i2c_transfer failed
 */
static rt_err_t pmic_i2c_read(struct rt_i2c_bus_device *pmic_i2c_bus,
                              rt_uint8_t addr, rt_uint16_t reg, rt_uint8_t *data)
{
    struct rt_i2c_msg msg[2];
    rt_uint8_t send_buf[2];

    send_buf[0] = (reg & 0xFF);

    msg[0].addr  = addr;
    msg[0].flags = RT_I2C_WR;
    msg[0].len   = 1;
    msg[0].buf   = send_buf;

    msg[1].addr  = addr;
    msg[1].flags = RT_I2C_RD;
    msg[1].len   = 1;
    msg[1].buf   = data;

    if (rt_i2c_transfer(pmic_i2c_bus, msg, 2) == 2)
        return RT_EOK;
    else
        return -RT_ERROR;
}

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup Pmic_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  pmic read.
 * @param pmic_i2c_bus: the i2c bus device pointer.
 * @param addr: the pmic i2c address.
 * @param reg: the pmic register .
 * @retval data[0]: i2c read regiter data
 * @retval RT_ERROR: rt_i2c_transfer failed
 */
rt_uint32_t pmic_read(struct rt_i2c_bus_device *pmic_i2c_bus,
                      rt_uint8_t addr, rt_uint16_t reg)
{
    rt_uint8_t data[1] = {0x00};

    if (pmic_i2c_read(pmic_i2c_bus, addr, reg, data) == RT_EOK)
        return data[0];
    else
        return -RT_ERROR;
}

/**
 * @brief  pmic write.
 * @param pmic_i2c_bus: the i2c bus device pointer.
 * @param addr: the pmic i2c address.
 * @param reg: the pmic register .
 * @param data: set data.
 * @retval RT_EOK: rt_i2c_transfer successfully
 * @retval RT_ERROR: rt_i2c_transfer failed
 */
rt_err_t pmic_write(struct rt_i2c_bus_device *pmic_i2c_bus,
                    rt_uint8_t addr, rt_uint16_t reg, rt_uint8_t data)
{
    struct rt_i2c_msg msg;
    rt_uint8_t send_buf[2];

    send_buf[0] = (reg & 0xFF);
    send_buf[1] = data;

    msg.addr = addr;
    msg.flags = RT_I2C_WR;
    msg.len = 2;
    msg.buf = send_buf;

    if (rt_i2c_transfer(pmic_i2c_bus, &msg, 1) == 1)
        return RT_EOK;
    else
        return -RT_ERROR;
}

/**
 * @brief  pmic update bits.
 * @param pmic_i2c_bus: the i2c bus device pointer.
 * @param addr: the pmic i2c address.
 * @param reg: the pmic register.
 * @param mask: data mask.
 * @param data: set data.
 * @retval RT_EOK: rt_i2c_transfer successfully
 * @retval -RT_ERROR: rt_i2c_transfer failed
 */
rt_err_t pmic_update_bits(struct rt_i2c_bus_device *pmic_i2c_bus,
                          rt_uint8_t addr, rt_uint16_t reg,
                          rt_uint8_t mask, rt_uint8_t data)
{
    rt_uint32_t orig, tmp;

    orig = pmic_read(pmic_i2c_bus, addr, reg);
    if (orig < 0)
        return orig;

    tmp = orig & ~mask;
    tmp |= (data & mask);

    return pmic_write(pmic_i2c_bus, addr, reg, tmp);
}

/**
 * @brief  pmic getting voltage.
 * @param desc: the power regulator description pointer.
 * @retval 0: rt_i2c_transfer failed
 * @retval volt: the voltage value (UV)
 */
rt_uint32_t pmic_get_voltage(struct pwr_i2cbus_desc *desc)
{
    rt_uint32_t val;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return 0;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_VOLT_RUN]);
    if (val < 0)
        return 0;

    val = val & desc->voltMask;

    return desc->minVolt + desc->volt_list.stepVolt * val;
}

/**
 * @brief  pmic setting voltage.
 * @param desc: the power regulator description pointer.
 * @param voltUv: the volt value to be config(uV)
 * @retval -RT_ERROR: rt_i2c_transfer failed
 * @retval RT_EOK: rt_i2c_transfer successfully
 */
rt_err_t pmic_set_voltage(struct pwr_i2cbus_desc *desc,
                          rt_uint32_t voltUv)
{
    rt_uint32_t volt, val;
    rt_uint32_t ret;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return -RT_ERROR;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_VOLT_RUN]);
    if (val < 0)
        return val;

    volt = LNR_VOLT_BG(voltUv, desc->minVolt, desc->volt_list.stepVolt);
    volt |= val & (~desc->voltMask);

    ret = pmic_write(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_VOLT_RUN], volt);

#ifdef RT_USING_PMIC_RK816
    if (ret < 0)
        return ret;

    ret = pmic_update_bits(desc->i2c->bus, desc->i2c->addr, RK816_DCDC_EN_REG2,
                           RK816_BUCK_DVS_CONFIRM, RK816_BUCK_DVS_CONFIRM);
#endif

    return ret;
}

/**
 * @brief  pmic getting suspend voltage.
 * @param desc: the power regulator description pointer.
 * @retval 0: rt_i2c_transfer failed
 * @retval volt: the voltage value (UV)
 */
rt_uint32_t pmic_get_suspend_voltage(struct pwr_i2cbus_desc *desc)
{
    rt_uint32_t val;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return 0;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_VOLT_SSPD]);
    if (val < 0)
        return 0;

    HAL_ASSERT(val < desc->voltCnt);
    val = val & desc->voltMask;

    return desc->minVolt + desc->volt_list.stepVolt * val;
}

/**
 * @brief  pmic setting suspend voltage.
 * @param desc: the power regulator description pointer.
 * @param voltUv: the volt value to be config(uV)
 * @retval -RT_ERROR: rt_i2c_transfer failed
 * @retval RT_EOK: rt_i2c_transfer successfully
 */
rt_err_t pmic_set_suspend_voltage(struct pwr_i2cbus_desc *desc,
                                  rt_uint32_t voltUv)
{
    rt_uint32_t volt, val;
    rt_uint32_t ret;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return -RT_ERROR;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_VOLT_SSPD]);
    if (val < 0)
        return val;

    volt = LNR_VOLT_BG(voltUv, desc->minVolt, desc->volt_list.stepVolt);
    volt |= val & (~desc->voltMask);

    ret = pmic_write(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_VOLT_SSPD], volt);

    return ret;
}

/**
 * @brief  pmic enable/disable output.
 * @param desc: the power regulator description pointer.
 * @param enable: enable or disable
 * @retval -RT_ERROR: rt_i2c_transfer failed
 * @retval RT_EOK: rt_i2c_transfer successfully
 */
rt_err_t pmic_set_enable(struct pwr_i2cbus_desc *desc, rt_uint32_t enable)
{
    rt_uint32_t val, en = desc->shift[PWR_CTRL_PWR_EN], mask = 0;
    rt_uint32_t ret = RT_EOK;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return -RT_ERROR;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_PWR_EN]);
    if (val < 0)
        return val;

    if (desc->flag & PWR_FLG_ENMASK)
        mask = en << 4;

    if (enable)
        val = val | mask | en;
    else
        val = (val | mask) & (~en);

    ret = pmic_write(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_PWR_EN], val);

    return ret;
}

/**
 * @brief  pmic enable/disable output.
 * @param desc: the power regulator description pointer.
 * @param enable: enable or disable
 * @retval -RT_ERROR: rt_i2c_transfer failed
 * @retval RT_EOK: rt_i2c_transfer successfully
 */
rt_err_t pmic_set_suspend_enable(struct pwr_i2cbus_desc *desc, rt_uint32_t enable)
{
    rt_uint32_t val, en = desc->shift[PWR_CTRL_PWR_SSPD];
    rt_uint32_t ret = RT_EOK;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return -RT_ERROR;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_PWR_SSPD]);
    if (val < 0)
        return val;

    if (enable)
        val |= en;
    else
        val &= ~en;

    ret = pmic_write(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_PWR_SSPD], val);

    return ret;
}

/**
 * @brief  get pmic output status.
 * @param desc: the power regulator description pointer.
 * @retval -RT_ERROR: rt_i2c_transfer failed
 * @retval 0: output is disabled
 * @retval 1: output is enabled
 */
rt_uint32_t pmic_is_enabled(struct pwr_i2cbus_desc *desc)
{
    rt_uint32_t val;

    if (desc->i2c->bus == RT_NULL)
    {
        rt_kprintf("%s: pwr_id =%d, Did not find %s device....\n", __func__, desc->info.pwrId, desc->i2c->name);
        return 0;
    }

    val = pmic_read(desc->i2c->bus, desc->i2c->addr, desc->reg[PWR_CTRL_PWR_EN]);
    if (val < 0)
        return val;

    if (val & desc->shift[PWR_CTRL_PWR_EN])
        return 1;
    else
        return 0;
}

/**
 * @brief  get pmic output status.
 * @param pdesc: the power regulator description pointer.
 * @param pwrId: the id of the ePWR_ID type
 * @retval RT_EOK:  the ID matches the desc
 * @retval -RT_EINVAL: struct desc is invalid argument
 */
rt_err_t pmic_check_desc_by_pwrId(struct pwr_i2cbus_desc *pdesc,
                                  ePWR_ID pwrId)
{
    if (pdesc->info.pwrId == pwrId)
        return RT_EOK;
    else
        return -RT_EINVAL;
}

/**
 * @brief  pmic sleep pin func setting.
 * @param desc: specify the i2c bus
 * @param polarity: the pmic sleep pin polarity.
 * @param func: the pmic sleep pin func.
 * @retval -RT_ERROR: rt_i2c_transfer failed
 * @retval RT_EOK: rt_i2c_transfer successfully
 * @retval -RT_EINVAL: polarity or func is invalid
 */
rt_err_t pmic_sleep_pin_func(struct pwr_i2cbus_desc *desc, ePMIC_SleepPinPolarity polarity, ePMIC_SleepPinFunc func)
{
    __unused rt_uint32_t val = 0, mask = 0, reg = 0;
    rt_uint32_t ret = RT_EOK;

    if (polarity > PMIC_SLEEP_ACTIVE_HIGH)
        return -RT_EINVAL;

    switch (func)
    {
    case PMIC_SLEEP_NO_EFFECT:
        return ret;
    case PMIC_SLEEP_SLEEP_FUNC:
    case PMIC_SLEEP_SHUTDOWN_FUNC:
    case PMIC_SLEEP_RESTART_FUNC:
#if defined(RT_USING_PMIC_RK817)
        mask = 0x38;
        reg = 0xf4;
        val = pmic_read(desc->i2c->bus, desc->i2c->addr, reg) & (~mask);
        if (val < 0)
            return val;
        ret = pmic_write(desc->i2c->bus, desc->i2c->addr, reg, val | (polarity << 5) | (func << 3));
#endif
        break;
    default:

        return -RT_EINVAL;
    }
    return ret;
}

/**
 * @brief  get pmic struct pwr_i2c_desc.
 * @retval NULL: invalid pmic struct pwr_i2c_desc when failed
 * @retval pmic struct pwr_i2c_desc when successful
 */
struct pwr_i2c_desc *pmic_get_i2c_desc(void)
{
    struct pwr_i2c_desc *i2c_desc = &pmic_pwr_i2c_desc;

    RT_ASSERT(i2c_desc->name != NULL);

    if (i2c_desc->bus == RT_NULL)
    {
        i2c_desc->bus = rt_i2c_bus_device_find(i2c_desc->name);
        if (i2c_desc->bus == RT_NULL)
        {
            rt_kprintf("PMIC: No %s\n", i2c_desc->name);
            return NULL;
        }
    }

    return i2c_desc;
}

/**
 * @brief  pmic power off.
 */
void pmic_power_off(void)
{
#ifdef RT_USING_PMIC_RK816
    struct pwr_i2c_desc *i2c_desc = pmic_get_i2c_desc();

    RT_ASSERT(i2c_desc != NULL);

    pmic_update_bits(i2c_desc->bus, i2c_desc->addr,
                     RK816_DEV_CTRL_REG,
                     RK816_DEV_OFF, RK816_DEV_OFF);
#endif
}

rt_err_t pmic_set_primary_hook(void (*hook)(void))
{
    rt_err_t ret = -RT_ERROR;
#ifdef RT_USING_PMIC_RK816
    ret = rk816_isr_sethook(hook);
#endif
    return ret;
}

rt_err_t pmic_plug_in_sethook(void (*hook)(void))
{
    rt_size_t i;
    rt_base_t level;
    rt_err_t ret = -RT_EFULL;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    for (i = 0; i < PMIC_PLUG_IN_HOOK_LIST_SIZE; i++)
    {
        if (pmic_plug_in_hook_list[i] == RT_NULL)
        {
            pmic_plug_in_hook_list[i] = hook;
            ret = RT_EOK;
            break;
        }
    }
    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return ret;
}

rt_err_t pmic_plug_out_sethook(void (*hook)(void))
{
    rt_size_t i;
    rt_base_t level;
    rt_err_t ret = -RT_EFULL;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    for (i = 0; i < PMIC_PLUG_OUT_HOOK_LIST_SIZE; i++)
    {
        if (pmic_plug_out_hook_list[i] == RT_NULL)
        {
            pmic_plug_out_hook_list[i] = hook;
            ret = RT_EOK;
            break;
        }
    }
    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return ret;
}

rt_err_t pmic_poweron_rise_sethook(void (*hook)(void))
{
    rt_size_t i;
    rt_base_t level;
    rt_err_t ret = -RT_EFULL;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    for (i = 0; i < PMIC_POWERON_RISE_HOOK_LIST_SIZE; i++)
    {
        if (pmic_poweron_rise_hook_list[i] == RT_NULL)
        {
            pmic_poweron_rise_hook_list[i] = hook;
            ret = RT_EOK;
            break;
        }
    }
    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return ret;
}

rt_err_t pmic_poweron_fall_sethook(void (*hook)(void))
{
    rt_size_t i;
    rt_base_t level;
    rt_err_t ret = -RT_EFULL;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    for (i = 0; i < PMIC_POWERON_FALL_HOOK_LIST_SIZE; i++)
    {
        if (pmic_poweron_fall_hook_list[i] == RT_NULL)
        {
            pmic_poweron_fall_hook_list[i] = hook;
            ret = RT_EOK;
            break;
        }
    }
    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return ret;
}

rt_err_t pmic_poweron_long_sethook(void (*hook)(void))
{
    rt_size_t i;
    rt_base_t level;
    rt_err_t ret = -RT_EFULL;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    for (i = 0; i < PMIC_POWERON_LONG_HOOK_LIST_SIZE; i++)
    {
        if (pmic_poweron_long_hook_list[i] == RT_NULL)
        {
            pmic_poweron_long_hook_list[i] = hook;
            ret = RT_EOK;
            break;
        }
    }
    /* enable interrupt */
    rt_hw_interrupt_enable(level);

    return ret;
}

void plug_in_isr(void *args)
{
    rt_size_t i;

    for (i = 0; i < PMIC_PLUG_IN_HOOK_LIST_SIZE; i++)
    {
        if (pmic_plug_in_hook_list[i] != RT_NULL)
        {
            pmic_plug_in_hook_list[i]();
        }
    }
}

void plug_out_isr(void *args)
{
    rt_size_t i;

    for (i = 0; i < PMIC_PLUG_OUT_HOOK_LIST_SIZE; i++)
    {
        if (pmic_plug_out_hook_list[i] != RT_NULL)
        {
            pmic_plug_out_hook_list[i]();
        }
    }
}

void poweron_rise_isr(void *args)
{
    rt_size_t i;

    for (i = 0; i < PMIC_POWERON_RISE_HOOK_LIST_SIZE; i++)
    {
        if (pmic_poweron_rise_hook_list[i] != RT_NULL)
        {
            pmic_poweron_rise_hook_list[i]();
        }
    }
}

void poweron_fall_isr(void *args)
{
    rt_size_t i;

    for (i = 0; i < PMIC_POWERON_FALL_HOOK_LIST_SIZE; i++)
    {
        if (pmic_poweron_fall_hook_list[i] != RT_NULL)
        {
            pmic_poweron_fall_hook_list[i]();
        }
    }
}

void poweron_long_isr(void *args)
{
    rt_size_t i;

    for (i = 0; i < PMIC_POWERON_LONG_HOOK_LIST_SIZE; i++)
    {
        if (pmic_poweron_long_hook_list[i] != RT_NULL)
        {
            pmic_poweron_long_hook_list[i]();
        }
    }
}
/** @} */

#endif

/** @} */
/** @} */

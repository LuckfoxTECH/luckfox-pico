/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_pmic.h
  * @version V0.1
  * @brief   pmic interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-07-11     Elaine.Zhang      first implementation
  *
  ******************************************************************************
  */

#ifndef __DRV_PMIC_H__
#define __DRV_PMIC_H__

/*******************************************************************************
 * Included Files
 ******************************************************************************/
#include "hal_base.h"
#include "hal_def.h"

extern struct pwr_i2c_desc pmic_pwr_i2c_desc;

/* support 7-bit slave address and 8-bit date transmission */
struct pwr_i2c_desc
{
    struct rt_i2c_bus_device *bus;
    char *name;
    rt_uint8_t addr;
};

struct pwr_i2cbus_desc
{
    struct PWR_CTRL_INFO info;
    uint16_t flag;
    uint8_t voltMask;
    uint8_t voltCnt;
    struct pwr_i2c_desc *i2c;
    uint8_t shift[PWR_CTRL_MAX];
    uint8_t reg[PWR_CTRL_MAX];
    uint32_t minVolt;
    union  U_PWR_VOLT_LIST volt_list;
};

#define PWR_DESC_I2C_SHIFT_RUN(_reg, sft)      \
    .reg[PWR_CTRL_VOLT_RUN] = (_reg), \
    .shift[PWR_CTRL_VOLT_RUN] = (sft)

#define PWR_DESC_I2C_SHIFT_SSPD(_reg, sft)      \
    .reg[PWR_CTRL_VOLT_SSPD] = (_reg), \
    .shift[PWR_CTRL_VOLT_SSPD] = (sft)

#define PWR_DESC_I2C_SHIFT_EN(_reg, sft)     \
    .reg[PWR_CTRL_PWR_EN] = (_reg), \
    .shift[PWR_CTRL_PWR_EN] = (sft)

#define PWR_DESC_I2C_SHIFT_SSPD_EN(_reg, sft)     \
    .reg[PWR_CTRL_PWR_SSPD] = (_reg), \
    .shift[PWR_CTRL_PWR_SSPD] = (sft)

#define PWR_DESC_I2C_SHIFT_ST(_reg, sft)      \
    .reg[PWR_CTRL_VOLT_ST] = (_reg), \
    .shift[PWR_CTRL_VOLT_ST] = (sft)

#define SYR827_I2C_ADDR    0x40            /* 7-bit I2C address                  */
#define SYR828_I2C_ADDR    0x41            /* 7-bit I2C address                  */
#define TCS4525_I2C_ADDR   0x1c            /* 7-bit I2C address                  */
#define RK805_I2C_ADDR     0x18            /* 7-bit I2C address                  */
#define RK808_I2C_ADDR     0x1b            /* 7-bit I2C address                  */
#define RK809_I2C_ADDR     0x20            /* 7-bit I2C address                  */
#define RK816_I2C_ADDR     0x1a            /* 7-bit I2C address                  */

/*******************************************************************************
 * Pre-processor Definitions
 ******************************************************************************/
#define RK817_BUCK_CONFIG_REG(idx)  (0xba + (idx) * 3)
#define RK817_BUCK_ON_VSEL_REG(idx) (0xbb + (idx) * 3)
#define RK817_BUCK_SLP_VSEL_REG(idx)    (0xbc + (idx) * 3)
#define RK817_LDO_ON_VSEL_REG(idx)  (0xcc + (idx) * 2)
#define RK817_LDO_SLP_VSEL_REG(idx) (0xcd + (idx) * 2)
#define RK817_POWER_EN_REG(i)       (0xb1 + (i))
#define RK817_POWER_SLP_EN_REG(i)   (0xb5 + (i))

typedef enum
{
    RK817_ID_DCDC1 = 0,
    RK817_ID_DCDC2,
    RK817_ID_DCDC3,
    RK817_ID_DCDC4,
    RK817_ID_LDO1,
    RK817_ID_LDO2,
    RK817_ID_LDO3,
    RK817_ID_LDO4,
    RK817_ID_LDO5,
    RK817_ID_LDO6,
    RK817_ID_LDO7,
    RK817_ID_LDO8,
    RK817_ID_LDO9,
    RK817_NUM_REGULATORS
} ePMIC_RK817Id;

#define RK817_BUCK(ID, PWR_ID)                                           \
    {                                                                              \
        .info = {                                                                  \
            .pwrId = PWR_ID,                                                       \
        },                                                                         \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),             \
        PWR_DESC_I2C_SHIFT_RUN(RK817_BUCK_ON_VSEL_REG(ID), 0),                    \
        PWR_DESC_I2C_SHIFT_SSPD(RK817_BUCK_SLP_VSEL_REG(ID), 0),                  \
        PWR_DESC_I2C_SHIFT_EN(RK817_POWER_EN_REG(0), 1 << ID),                    \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK817_POWER_SLP_EN_REG(0), 1 << ID),           \
        .voltMask = 0x7f,                                                          \
        PWR_DESC_LINEAR_VOLT(500000, 1700000, 12500)                               \
    }

#define RK817_LDO(ID, PWR_ID)                                            \
    {                                                                              \
        .info = {                                                                  \
            .pwrId = PWR_ID,                                                       \
        },                                                                         \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),             \
        PWR_DESC_I2C_SHIFT_RUN(RK817_LDO_ON_VSEL_REG(ID - RK817_ID_LDO1), 0),     \
        PWR_DESC_I2C_SHIFT_SSPD(RK817_LDO_SLP_VSEL_REG(ID - RK817_ID_LDO1), 0),   \
        PWR_DESC_I2C_SHIFT_EN(RK817_POWER_EN_REG(1), 1 << (ID - RK817_ID_LDO1)),  \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK817_POWER_SLP_EN_REG(1),                     \
                                                      1 << (ID - RK817_ID_LDO1)),  \
        .voltMask = 0x7f,                                                          \
        PWR_DESC_LINEAR_VOLT(600000, 3400000, 25000)                               \
    }

#define RK817_LDO9(ID, PWR_ID)                                           \
    {                                                                              \
        .info = {                                                                  \
            .pwrId = PWR_ID,                                                       \
        },                                                                         \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),             \
        PWR_DESC_I2C_SHIFT_RUN(RK817_LDO_ON_VSEL_REG(ID - RK817_ID_LDO1), 0),     \
        PWR_DESC_I2C_SHIFT_SSPD(RK817_LDO_SLP_VSEL_REG(ID - RK817_ID_LDO1), 0),   \
        PWR_DESC_I2C_SHIFT_EN(RK817_POWER_EN_REG(3), 1 << 0),                     \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK817_POWER_SLP_EN_REG(0), 1 << 4),            \
        .voltMask = 0x7f,                                                          \
        PWR_DESC_LINEAR_VOLT(600000, 3400000, 25000)                               \
    }

/* RK816 definition */
#define RK816_BUCK1_2_ON_VSEL_REG(idx)  (0x2f + (idx) * 4)
#define RK816_BUCK1_2_SLP_VSEL_REG(idx) (0x30 + (idx) * 4)
#define RK816_BUCK4_ON_VSEL_REG         (0x38)
#define RK816_BUCK4_SLP_VSEL_REG        (0x39)
#define RK816_LDO_ON_VSEL_REG(idx)      (0x3b + (idx) * 2)
#define RK816_LDO_SLP_VSEL_REG(idx)     (0x3c + (idx) * 2)

#define RK816_DCDC_EN_REG1              0x23
#define RK816_DCDC_EN_REG2              0x24
#define RK816_SLP_DCDC_EN_REG           0x25
#define RK816_SLP_LDO_EN_REG            0x26
#define RK816_LDO_EN_REG1               0x27
#define RK816_LDO_EN_REG2               0x28
#define RK816_DEV_CTRL_REG              0x4b
#define RK816_DEV_OFF                   (1 << 0)
#define RK816_BUCK_DVS_CONFIRM          (1 << 7)

#define ENABLE_MASK(id)                 (BIT(id) | BIT(4 + (id)))

typedef enum
{
    RK816_ID_DCDC1,
    RK816_ID_DCDC2,
    RK816_ID_DCDC3,
    RK816_ID_DCDC4,
    RK816_ID_LDO1,
    RK816_ID_LDO2,
    RK816_ID_LDO3,
    RK816_ID_LDO4,
    RK816_ID_LDO5,
    RK816_ID_LDO6,
    RK816_NUM_REGULATORS
} ePMIC_RK816Id;

/* BUCK1_2: 0.7125v - 1.45v */
#define RK816_BUCK1_2(ID, PWR_ID)                                                 \
    {                                                                             \
        .info = {                                                                 \
            .pwrId = PWR_ID,                                                      \
        },                                                                        \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),            \
        PWR_DESC_I2C_SHIFT_RUN(RK816_BUCK1_2_ON_VSEL_REG(ID), 0),                 \
        PWR_DESC_I2C_SHIFT_SSPD(RK816_BUCK1_2_SLP_VSEL_REG(ID), 0),               \
        PWR_DESC_I2C_SHIFT_EN(RK816_DCDC_EN_REG1, 1 << ID),                       \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK816_SLP_DCDC_EN_REG, 1 << ID),               \
        .voltMask = 0x3f,                                                         \
        PWR_DESC_LINEAR_VOLT(712500, 1450000, 12500)                              \
    }

/* BUCK4: 0.8v - 3.5v */
#define RK816_BUCK4(ID, PWR_ID)                                                   \
    {                                                                             \
        .info = {                                                                 \
            .pwrId = PWR_ID,                                                      \
        },                                                                        \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),            \
        PWR_DESC_I2C_SHIFT_RUN(RK816_BUCK4_ON_VSEL_REG, 0),                       \
        PWR_DESC_I2C_SHIFT_SSPD(RK816_BUCK4_SLP_VSEL_REG, 0),                     \
        PWR_DESC_I2C_SHIFT_EN(RK816_DCDC_EN_REG1, 1 << ID),                       \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK816_SLP_DCDC_EN_REG, 1 << ID),               \
        .voltMask = 0x1f,                                                         \
        PWR_DESC_LINEAR_VOLT(800000, 3500000, 100000)                             \
    }

/* LDO1~4: 0.8v - 3.4v */
#define RK816_LDO1_4(ID, PWR_ID)                                                  \
    {                                                                             \
        .info = {                                                                 \
            .pwrId = PWR_ID,                                                      \
        },                                                                        \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),            \
        PWR_DESC_I2C_SHIFT_RUN(RK816_LDO_ON_VSEL_REG(ID - RK816_ID_LDO1), 0),     \
        PWR_DESC_I2C_SHIFT_SSPD(RK816_LDO_SLP_VSEL_REG(ID - RK816_ID_LDO1), 0),   \
        PWR_DESC_I2C_SHIFT_EN(RK816_LDO_EN_REG1, 1 << (ID - RK816_ID_LDO1)),      \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK816_SLP_LDO_EN_REG, 1 << (ID - RK816_ID_LDO1)),  \
        .voltMask = 0x1f,                                                         \
        PWR_DESC_LINEAR_VOLT(800000, 3400000, 100000)                             \
    }

/* LDO5~6: 0.8v - 3.4v */
#define RK816_LDO5_6(ID, PWR_ID)                                                  \
    {                                                                             \
        .info = {                                                                 \
            .pwrId = PWR_ID,                                                      \
        },                                                                        \
        .flag = DESC_FLAG_LINEAR(PWR_CTRL_VOLT_SSPD | PWR_FLG_ENMASK),            \
        PWR_DESC_I2C_SHIFT_RUN(RK816_LDO_ON_VSEL_REG(ID - RK816_ID_LDO1), 0),     \
        PWR_DESC_I2C_SHIFT_SSPD(RK816_LDO_SLP_VSEL_REG(ID - RK816_ID_LDO1), 0),   \
        PWR_DESC_I2C_SHIFT_EN(RK816_LDO_EN_REG2, 1 << (ID - RK816_ID_LDO5)),      \
        PWR_DESC_I2C_SHIFT_SSPD_EN(RK816_SLP_LDO_EN_REG, 1 << (ID - RK816_ID_LDO5)),  \
        .voltMask = 0x1f,                                                         \
        PWR_DESC_LINEAR_VOLT(800000, 3400000, 100000)                             \
    }

/*******************************************************************************
 * Public Types
 ******************************************************************************/
typedef enum
{
    PMIC_SLEEP_ACTIVE_LOW = 0U,
    PMIC_SLEEP_ACTIVE_HIGH,
} ePMIC_SleepPinPolarity;

typedef enum
{
    PMIC_SLEEP_NO_EFFECT = 0U,
    PMIC_SLEEP_SLEEP_FUNC,
    PMIC_SLEEP_SHUTDOWN_FUNC,
    PMIC_SLEEP_RESTART_FUNC,
} ePMIC_SleepPinFunc;

#ifdef RT_USING_PMIC
extern const ePMIC_SleepPinFunc g_pmic_sleep_pin_func;
extern const ePMIC_SleepPinPolarity g_pmic_sleep_pin_polarity;
#endif

/*******************************************************************************
 * Public Data
 ******************************************************************************/

/*******************************************************************************
 * Inline Functions
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/
#if defined(RT_USING_PMIC)
rt_uint32_t pmic_read(struct rt_i2c_bus_device *pmic_i2c_bus,
                      rt_uint8_t addr, rt_uint16_t reg);
rt_err_t pmic_write(struct rt_i2c_bus_device *pmic_i2c_bus,
                    rt_uint8_t addr, rt_uint16_t reg, rt_uint8_t data);
rt_err_t pmic_update_bits(struct rt_i2c_bus_device *pmic_i2c_bus,
                          rt_uint8_t addr, rt_uint16_t reg,
                          rt_uint8_t mask, rt_uint8_t data);
rt_uint32_t pmic_get_voltage(struct pwr_i2cbus_desc *desc);
rt_err_t pmic_set_voltage(struct pwr_i2cbus_desc *desc,
                          rt_uint32_t voltUv);
rt_uint32_t pmic_get_suspend_voltage(struct pwr_i2cbus_desc *desc);
rt_err_t pmic_set_suspend_voltage(struct pwr_i2cbus_desc *desc,
                                  rt_uint32_t voltUv);
rt_err_t pmic_set_enable(struct pwr_i2cbus_desc *desc, rt_uint32_t enable);
rt_err_t pmic_set_suspend_enable(struct pwr_i2cbus_desc *desc, rt_uint32_t enable);
rt_uint32_t pmic_is_enabled(struct pwr_i2cbus_desc *desc);
int pmic_desc_init(struct pwr_i2cbus_desc *descs, uint32_t cnt);
void pmic_desc_deinit(void);
rt_err_t pmic_check_desc_by_pwrId(struct pwr_i2cbus_desc *pdesc, ePWR_ID pwrId);
rt_err_t pmic_sleep_pin_func(struct pwr_i2cbus_desc *desc, ePMIC_SleepPinPolarity polarity, ePMIC_SleepPinFunc func);
struct pwr_i2c_desc *pmic_get_i2c_desc(void);
void pmic_power_off(void);
rt_err_t pmic_set_primary_hook(void (*hook)(void));
rt_err_t pmic_plug_in_sethook(void (*hook)(void));
rt_err_t pmic_plug_out_sethook(void (*hook)(void));
rt_err_t pmic_poweron_rise_sethook(void (*hook)(void));
rt_err_t pmic_poweron_fall_sethook(void (*hook)(void));
rt_err_t pmic_poweron_long_sethook(void (*hook)(void));
void plug_in_isr(void *args);
void plug_out_isr(void *args);
void poweron_rise_isr(void *args);
void poweron_fall_isr(void *args);
void poweron_long_isr(void *args);
#endif

#endif /* __DRV_PMIC_H__ */

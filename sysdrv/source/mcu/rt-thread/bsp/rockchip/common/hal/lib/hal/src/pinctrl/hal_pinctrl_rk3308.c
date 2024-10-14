/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_PINCTRL_MODULE_ENABLED) && defined(SOC_RK3308)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup PINCTRL
 *  @{
 */

/** @defgroup PINCTRL_How_To_Use How To Use
 *  @{

 The pinctrl setting registers actually is bus grf registers, which include
 iomux, drive strength, pull mode, slew rate and schmitt trigger.

 The pinctrl driver provides APIs:
   - HAL_PINCTRL_SetIOMUX() to set pin iomux
   - HAL_PINCTRL_SetParam() to set pin iomux/pull/drive strength/slew rate/schmitt trigger

 Example:

     HAL_PINCTRL_SetIOMUX(GPIO_BANK4,
                          GPIO_PIN_B0 | // UART4_RX
                          GPIO_PIN_B1,  // UART4_TX
                          PIN_CONFIG_MUX_FUNC1);

     HAL_PINCTRL_SetParam(GPIO_BANK4,
                          GPIO_PIN_B0 | // UART4_RX
                          GPIO_PIN_B1,  // UART4_TX
                          PIN_CONFIG_MUX_FUNC1 |
                          PIN_CONFIG_PUL_UP |
                          PIN_CONFIG_DRV_LEVEL2);

 TODO:If you configure the iomux of the following modules, you need to configure
 the group select register additionally:
 pdm: m0/m1/m2
 i2s1: m0/m1
 uart2: m0/m1  uart3: m0/m1
 mac: m0/m1
 can: m0/m1/m2
 owire: m0/m1/m2
 spi1: m0/m1
 i2c3: sel-m0/m1  sel-plus-m0/m1/m2

 @} */

/** @defgroup PINCTRL_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define _TO_MASK(w)          ((1U << (w)) - 1U)
#define _TO_OFFSET(gp, w)    ((gp) * (w))
#define RK_GEN_VAL(gp, v, w) ((_TO_MASK(w) << (_TO_OFFSET(gp, w) + 16)) | (((v) & _TO_MASK(w)) << _TO_OFFSET(gp, w)))

/*
 * Use HAL_DBG("pinctrl: write val = 0x%lx to register %p\n", VAL, &REG);
 * and HAL_DBG("pinctrl: readback register %p = 0x%lx\n", &REG, REG);
 * for debug
 */
#define _PINCTRL_WRITE(REG, VAL) \
{                                \
    REG = VAL;                   \
}

#define IOMUX_2_BIT_PER_PIN                         (2)
#define IOMUX_8_PIN_PER_REG                         (8)
#define IOMUX_0(__B, __G)                           (GRF->GPIO##__B##__G##_IOMUX)
#define SET_IOMUX_0(_B, _G, _GP, _V, _W)            _PINCTRL_WRITE(IOMUX_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_IOMUX_0(B, G, BP, V)                 SET_IOMUX_0(B, G, BP % IOMUX_8_PIN_PER_REG, V, IOMUX_2_BIT_PER_PIN)
#define SET_IOMUX_2_BIT(BANK, GROUP, BANK_PIN, VAL) RK_SET_IOMUX_0(BANK, GROUP, BANK_PIN, VAL)

#define PINCTRL_SET_IOMUX_2_BIT(bank, bank_pin, val) \
{                                                    \
    if (bank_pin < 8) {                              \
        SET_IOMUX_2_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                      \
        SET_IOMUX_2_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                      \
        SET_IOMUX_2_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                      \
        SET_IOMUX_2_BIT(bank, D, bank_pin, val);     \
    } else {                                         \
        HAL_DBG("unknown pin %d\n", bank_pin);       \
    }                                                \
}

#define IOMUX_4_BIT_PER_PIN              (4)
#define IOMUX_4_PIN_PER_REG              (4)
#define IOMUX_L(__B, __G)                (GRF->GPIO##__B##__G##_IOMUX_L)
#define IOMUX_H(__B, __G)                (GRF->GPIO##__B##__G##_IOMUX_H)
#define SET_IOMUX_L(_B, _G, _GP, _V, _W) _PINCTRL_WRITE(IOMUX_L(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_H(_B, _G, _GP, _V, _W) _PINCTRL_WRITE(IOMUX_H(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_IOMUX_L(B, G, BP, V)      SET_IOMUX_L(B, G, BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_H(B, G, BP, V)      SET_IOMUX_H(B, G, BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define SET_IOMUX_4_BIT(BANK, GROUP, BANK_PIN, VAL) \
{                                                   \
    if ((BANK_PIN % 8) < 4) {                       \
        RK_SET_IOMUX_L(BANK, GROUP, BANK_PIN, VAL); \
    } else {                                        \
        RK_SET_IOMUX_H(BANK, GROUP, BANK_PIN, VAL); \
    }                                               \
}

#define PINCTRL_SET_IOMUX_4_BIT(bank, bank_pin, val) \
{                                                    \
    if (bank_pin < 8) {                              \
        SET_IOMUX_4_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                      \
        SET_IOMUX_4_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                      \
        SET_IOMUX_4_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                      \
        SET_IOMUX_4_BIT(bank, D, bank_pin, val);     \
    } else {                                         \
        HAL_DBG("unknown pin %d\n", bank_pin);       \
    }                                                \
}

#define PULL_2_BIT_PER_PIN                         (2)
#define PULL_8_PIN_PER_REG                         (8)
#define PULL_0(__B, __G)                           (GRF->GPIO##__B##__G##_P)
#define SET_PULL_0(_B, _G, _GP, _V, _W)            _PINCTRL_WRITE(PULL_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_PULL_0(B, G, BP, V)                 SET_PULL_0(B, G, BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define SET_PULL_2_BIT(BANK, GROUP, BANK_PIN, VAL) RK_SET_PULL_0(BANK, GROUP, BANK_PIN, VAL)

#define PINCTRL_SET_PULL_2_BIT(bank, bank_pin, val) \
{                                                   \
    if (bank_pin < 8) {                             \
        SET_PULL_2_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                     \
        SET_PULL_2_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                     \
        SET_PULL_2_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                     \
        SET_PULL_2_BIT(bank, D, bank_pin, val);     \
    } else {                                        \
        HAL_DBG("unknown pin %d\n", bank_pin);      \
    }                                               \
}

#define DRV_2_BIT_PER_PIN                         (2)
#define DRV_8_PIN_PER_REG                         (8)
#define DRV_0(__B, __G)                           (GRF->GPIO##__B##__G##_E)
#define SET_DRV_0(_B, _G, _GP, _V, _W)            _PINCTRL_WRITE(DRV_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_DRV_0(B, G, BP, V)                 SET_DRV_0(B, G, BP % DRV_8_PIN_PER_REG, V, DRV_2_BIT_PER_PIN)
#define SET_DRV_2_BIT(BANK, GROUP, BANK_PIN, VAL) RK_SET_DRV_0(BANK, GROUP, BANK_PIN, VAL)

#define PINCTRL_SET_DRV_2_BIT(bank, bank_pin, val) \
{                                                  \
    if (bank_pin < 8) {                            \
        SET_DRV_2_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                    \
        SET_DRV_2_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                    \
        SET_DRV_2_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                    \
        SET_DRV_2_BIT(bank, D, bank_pin, val);     \
    } else {                                       \
        HAL_DBG("unknown pin %d\n", bank_pin);     \
    }                                              \
}

#define SR_1_BIT_PER_PIN                         (1)
#define SR_8_PIN_PER_REG                         (8)
#define SR_0(__B, __G)                           (GRF->GPIO##__B##__G##_SR)
#define SET_SR_0(_B, _G, _GP, _V, _W)            _PINCTRL_WRITE(SR_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_SR_0(B, G, BP, V)                 SET_SR_0(B, G, BP % SR_8_PIN_PER_REG, V, SR_1_BIT_PER_PIN)
#define SET_SR_1_BIT(BANK, GROUP, BANK_PIN, VAL) RK_SET_SR_0(BANK, GROUP, BANK_PIN, VAL)

#define PINCTRL_SET_SR_1_BIT(bank, bank_pin, val) \
{                                                 \
    if (bank_pin < 8) {                           \
        SET_SR_1_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                   \
        SET_SR_1_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                   \
        SET_SR_1_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                   \
        SET_SR_1_BIT(bank, D, bank_pin, val);     \
    } else {                                      \
        HAL_DBG("unknown pin %d\n", bank_pin);    \
    }                                             \
}

#define SMT_1_BIT_PER_PIN                         (1)
#define SMT_8_PIN_PER_REG                         (8)
#define SMT_0(__B, __G)                           (GRF->GPIO##__B##__G##_SMT)
#define SET_SMT_0(_B, _G, _GP, _V, _W)            _PINCTRL_WRITE(SMT_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_SMT_0(B, G, BP, V)                 SET_SMT_0(B, G, BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define SET_SMT_1_BIT(BANK, GROUP, BANK_PIN, VAL) RK_SET_SMT_0(BANK, GROUP, BANK_PIN, VAL)

#define PINCTRL_SET_SMT_1_BIT(bank, bank_pin, val) \
{                                                  \
    if (bank_pin < 8) {                            \
        SET_SMT_1_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                    \
        SET_SMT_1_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                    \
        SET_SMT_1_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                    \
        SET_SMT_1_BIT(bank, D, bank_pin, val);     \
    } else {                                       \
        HAL_DBG("unknown pin %d\n", bank_pin);     \
    }                                              \
}

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/**
 * @brief  Private function to set pin iomux.
 * @param  bank: pin bank.
 * @param  pin: bank pin number 0~31.
 * @param  val: value to write.
 * @return HAL_Status.
 */
static HAL_Status PINCTRL_SetIOMUX(eGPIO_bankId bank, uint8_t pin, uint32_t val)
{
    switch (bank) {
    case 0:
        if (pin < 8) {
            SET_IOMUX_2_BIT(0, A, pin, val);
        } else if (pin < 16) {
            SET_IOMUX_2_BIT(0, B, pin, val);
        } else {
            SET_IOMUX_2_BIT(0, C, pin, val);
        }
        break;
    case 1:
        if (pin < 8) {
            SET_IOMUX_2_BIT(1, A, pin, val);
        } else if (pin < 14) {
            GRF->GPIO1B_IOMUX_L = RK_GEN_VAL(pin % 8, val, 2);
        } else if (pin < 15) {
            GRF->GPIO1B_IOMUX_L = RK_GEN_VAL(3, val, 4);
        } else if (pin < 16) {
            GRF->GPIO1B_IOMUX_H = RK_GEN_VAL(0, val, 4);
        } else if (pin < 18) {
            GRF->GPIO1C_IOMUX_L = RK_GEN_VAL(pin % 8, val, 2);
        } else if (pin < 21) {
            GRF->GPIO1C_IOMUX_L = RK_GEN_VAL((pin - 1) % 4, val, 4);
        } else if (pin < 24) {
            GRF->GPIO1C_IOMUX_H = RK_GEN_VAL((pin - 1) % 4, val, 4);
        } else {
            SET_IOMUX_2_BIT(1, D, pin, val);
        }
        break;
    case 2:
        if (pin < 2) {
            SET_IOMUX_2_BIT(2, A, pin, val);
        } else if (pin < 4) {
            if (val < 4) {
                SET_IOMUX_2_BIT(2, A, pin, val);
            } else {
                GRF->SOC_CON13 = RK_GEN_VAL((pin - 2), (val + 4), 4);
            }
        } else if (pin < 8) {
            SET_IOMUX_2_BIT(2, A, pin, val);
        } else if (pin < 16) {
            SET_IOMUX_2_BIT(2, B, pin, val);
        } else {
            if (val < 4) {
                SET_IOMUX_2_BIT(2, C, pin, val);
            } else {
                GRF->SOC_CON15 = RK_GEN_VAL(2, (val + 4), 4);
            }
        }
        break;
    case 3:
        if (pin < 8) {
            SET_IOMUX_2_BIT(3, A, pin, val);
        } else if (pin < 10) {
            SET_IOMUX_2_BIT(3, B, pin, val);
        } else if (pin < 12) {
            if (val < 4) {
                SET_IOMUX_2_BIT(3, B, pin, val);
            } else {
                GRF->SOC_CON15 = RK_GEN_VAL((pin - 10), (val + 4), 4);
            }
        } else {
            GRF->GPIO3B_IOMUX = RK_GEN_VAL((pin - 10), val, 4);
        }
        break;
    case 4:
        PINCTRL_SET_IOMUX_2_BIT(4, pin, val);
        break;
    default:
        HAL_DBG("unknown gpio%d\n", bank);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  Private function to set pin pull.
 * @param  bank: pin bank.
 * @param  pin: bank pin number 0~31.
 * @param  val: value to write.
 * @return HAL_Status.
 */
static HAL_Status PINCTRL_SetPULL(eGPIO_bankId bank, uint8_t pin, uint32_t val)
{
    switch (bank) {
    case 0:
        if (pin < 8) {
            SET_PULL_2_BIT(0, A, pin, val);
        } else if (pin < 16) {
            SET_PULL_2_BIT(0, B, pin, val);
        } else {
            SET_PULL_2_BIT(0, C, pin, val);
        }
        break;
    case 1:
        PINCTRL_SET_PULL_2_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_PULL_2_BIT(2, A, pin, val);
        } else if (pin < 16) {
            SET_PULL_2_BIT(2, B, pin, val);
        } else {
            SET_PULL_2_BIT(2, C, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_PULL_2_BIT(3, A, pin, val);
        } else {
            SET_PULL_2_BIT(3, B, pin, val);
        }
        break;
    case 4:
        PINCTRL_SET_PULL_2_BIT(4, pin, val);
        break;
    default:
        HAL_DBG("unknown gpio%d\n", bank);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  Private function to set pin drive strength.
 * @param  bank: pin bank.
 * @param  pin: bank pin number 0~31.
 * @param  val: value to write.
 * @return HAL_Status.
 */
static HAL_Status PINCTRL_SetDRV(eGPIO_bankId bank, uint8_t pin, uint32_t val)
{
    switch (bank) {
    case 0:
        if (pin < 8) {
            SET_DRV_2_BIT(0, A, pin, val);
        } else if (pin < 16) {
            SET_DRV_2_BIT(0, B, pin, val);
        } else {
            SET_DRV_2_BIT(0, C, pin, val);
        }
        break;
    case 1:
        PINCTRL_SET_DRV_2_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_DRV_2_BIT(2, A, pin, val);
        } else if (pin < 16) {
            SET_DRV_2_BIT(2, B, pin, val);
        } else {
            SET_DRV_2_BIT(2, C, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_DRV_2_BIT(3, A, pin, val);
        } else {
            SET_DRV_2_BIT(3, B, pin, val);
        }
        break;
    case 4:
        PINCTRL_SET_DRV_2_BIT(4, pin, val);
        break;
    default:
        HAL_DBG("unknown gpio%d\n", bank);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  Private function to set pin slew rate.
 * @param  bank: pin bank.
 * @param  pin: bank pin number 0~31.
 * @param  val: value to write.
 * @return HAL_Status.
 */
static HAL_Status PINCTRL_SetSR(eGPIO_bankId bank, uint8_t pin, uint32_t val)
{
    switch (bank) {
    case 0:
        if (pin < 8) {
            SET_SR_1_BIT(0, A, pin, val);
        } else if (pin < 16) {
            SET_SR_1_BIT(0, B, pin, val);
        } else {
            SET_SR_1_BIT(0, C, pin, val);
        }
        break;
    case 1:
        PINCTRL_SET_SR_1_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_SR_1_BIT(2, A, pin, val);
        } else if (pin < 16) {
            SET_SR_1_BIT(2, B, pin, val);
        } else {
            SET_SR_1_BIT(2, C, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_SR_1_BIT(3, A, pin, val);
        } else {
            SET_SR_1_BIT(3, B, pin, val);
        }
        break;
    case 4:
        PINCTRL_SET_SR_1_BIT(4, pin, val);
        break;
    default:
        HAL_DBG("unknown gpio%d\n", bank);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  Private function to set pin schmitt trigger.
 * @param  bank: pin bank.
 * @param  pin: bank pin number 0~31.
 * @param  val: value to write.
 * @return HAL_Status.
 */
static HAL_Status PINCTRL_SetSMT(eGPIO_bankId bank, uint8_t pin, uint32_t val)
{
    switch (bank) {
    case 0:
        if (pin < 8) {
            SET_SMT_1_BIT(0, A, pin, val);
        } else if (pin < 16) {
            SET_SMT_1_BIT(0, B, pin, val);
        } else {
            SET_SMT_1_BIT(0, C, pin, val);
        }
        break;
    case 1:
        PINCTRL_SET_SMT_1_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_SMT_1_BIT(2, A, pin, val);
        } else if (pin < 16) {
            SET_SMT_1_BIT(2, B, pin, val);
        } else {
            SET_SMT_1_BIT(2, C, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_SMT_1_BIT(3, A, pin, val);
        } else {
            SET_SMT_1_BIT(3, B, pin, val);
        }
        break;
    case 4:
        PINCTRL_SET_SMT_1_BIT(4, pin, val);
        break;
    default:
        HAL_DBG("unknown gpio%d\n", bank);
        break;
    }

    return HAL_OK;
}

/**
 * @brief  Private function to set pin parameter.
 * @param  bank: pin bank.
 * @param  pin: bank pin number 0~31.
 * @param  param: multi parameters defined in @ref ePINCTRL_configParam,
 * @return HAL_Status.
 */
static HAL_Status PINCTRL_SetPinParam(eGPIO_bankId bank, uint8_t pin, uint32_t param)
{
    HAL_Status rc = HAL_OK;

    if (param & FLAG_MUX) {
        rc |= PINCTRL_SetIOMUX(bank, pin, (uint8_t)((param & MASK_MUX) >> SHIFT_MUX));
    }

    if (param & FLAG_PUL) {
        rc |= PINCTRL_SetPULL(bank, pin, (uint8_t)((param & MASK_PUL) >> SHIFT_PUL));
    }

    if (param & FLAG_DRV) {
        rc |= PINCTRL_SetDRV(bank, pin, (uint8_t)((param & MASK_DRV) >> SHIFT_DRV));
    }

    if (param & FLAG_SRT) {
        rc |= PINCTRL_SetSR(bank, pin, (uint8_t)((param & MASK_SRT) >> SHIFT_SRT));
    }

    if (param & FLAG_SMT) {
        rc |= PINCTRL_SetSMT(bank, pin, (uint8_t)((param & MASK_SMT) >> SHIFT_SMT));
    }

    return rc;
}
/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup PINCTRL_Exported_Functions_Group1 Suspend and Resume Functions

 This section provides functions allowing to suspend and resume the module:

 *  @{
 */

/** @} */

/** @defgroup PINCTRL_Exported_Functions_Group2 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */
HAL_Status HAL_PINCTRL_Init(void)
{
    return HAL_OK;
}

HAL_Status HAL_PINCTRL_DeInit(void)
{
    return HAL_OK;
}
/** @} */

/** @defgroup PINCTRL_Exported_Functions_Group3 IO Functions

 *  @{
 */

/**
 * @brief  Public function to set pin parameter for multi pins.
 * @param  bank: pin bank.
 * @param  mPins: multi pins defined in @ref ePINCTRL_GPIO_PINS.
 * @param  param: multi parameters defined in @ref ePINCTRL_configParam,
 * @return HAL_Status.
 */
HAL_Status HAL_PINCTRL_SetParam(eGPIO_bankId bank, uint32_t mPins, ePINCTRL_configParam param)
{
    uint8_t pin;
    HAL_Status rc;

    HAL_ASSERT(bank < GPIO_BANK_NUM);

    if (!(param & (FLAG_MUX | FLAG_PUL | FLAG_DRV | FLAG_SRT | FLAG_SMT))) {
        HAL_DBG("pinctrl: no parameter!\n");

        return HAL_ERROR;
    }

    for (pin = 0; pin < 32; pin++) {
        if (mPins & (1 << pin)) {
            rc = PINCTRL_SetPinParam(bank, pin, param);
            if (rc) {
                return rc;
            }
        }
    }

    return HAL_OK;
}

/**
 * @brief  Public function to set pin iomux for multi pins.
 * @param  bank: pin bank.
 * @param  mPins: multi pins defined in @ref ePINCTRL_GPIO_PINS.
 * @param  param: multi parameters defined in @ref ePINCTRL_configParam,
 * @return HAL_Status.
 */
HAL_Status HAL_PINCTRL_SetIOMUX(eGPIO_bankId bank, uint32_t mPins, ePINCTRL_configParam param)
{
    return HAL_PINCTRL_SetParam(bank, mPins, param);
}
/** @} */

/** @} */

/** @} */

#endif /* HAL_PINCTRL_MODULE_ENABLED */

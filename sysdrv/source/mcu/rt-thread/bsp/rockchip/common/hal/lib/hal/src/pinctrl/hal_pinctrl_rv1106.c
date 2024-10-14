/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_PINCTRL_MODULE_ENABLED) && defined(SOC_RV1106)

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

     HAL_PINCTRL_SetIOMUX(GPIO_BANK0,
                          GPIO_PIN_A0 | // UART0_RX_M0
                          GPIO_PIN_A1,  // UART0_TX_M0
                          PIN_CONFIG_MUX_FUNC1);

     HAL_PINCTRL_SetParam(GPIO_BANK0,
                          GPIO_PIN_A0 | // UART0_RX_M0
                          GPIO_PIN_A1,  // UART0_TX_M0
                          PIN_CONFIG_MUX_FUNC1 |
                          PIN_CONFIG_PUL_UP |
                          PIN_CONFIG_DRV_LEVEL2);
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

#define IOMUX_4_BIT_PER_PIN              (4)
#define IOMUX_4_PIN_PER_REG              (4)
#define IOMUX_L(__B, __G)                (GPIO##__B##_IOC->GPIO##__B##__G##_IOMUX_SEL_L)
#define IOMUX_H(__B, __G)                (GPIO##__B##_IOC->GPIO##__B##__G##_IOMUX_SEL_H)
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
#define PULL_0(__B, __G)                           (GPIO##__B##_IOC->GPIO##__B##__G##_P)
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

#define DRV_8_BIT_PER_PIN              (8)
#define DRV_2_PIN_PER_REG              (2)
#define DRV_0(__B, __G)                (GPIO##__B##_IOC->GPIO##__B##__G##_DS0)
#define DRV_1(__B, __G)                (GPIO##__B##_IOC->GPIO##__B##__G##_DS1)
#define DRV_2(__B, __G)                (GPIO##__B##_IOC->GPIO##__B##__G##_DS2)
#define DRV_3(__B, __G)                (GPIO##__B##_IOC->GPIO##__B##__G##_DS3)
#define SET_DRV_0(_B, _G, _GP, _V, _W) _PINCTRL_WRITE(DRV_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1(_B, _G, _GP, _V, _W) _PINCTRL_WRITE(DRV_1(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2(_B, _G, _GP, _V, _W) _PINCTRL_WRITE(DRV_2(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3(_B, _G, _GP, _V, _W) _PINCTRL_WRITE(DRV_3(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_DRV_0(B, G, BP, V)      SET_DRV_0(B, G, BP % DRV_2_PIN_PER_REG, V, DRV_8_BIT_PER_PIN)
#define RK_SET_DRV_1(B, G, BP, V)      SET_DRV_1(B, G, BP % DRV_2_PIN_PER_REG, V, DRV_8_BIT_PER_PIN)
#define RK_SET_DRV_2(B, G, BP, V)      SET_DRV_2(B, G, BP % DRV_2_PIN_PER_REG, V, DRV_8_BIT_PER_PIN)
#define RK_SET_DRV_3(B, G, BP, V)      SET_DRV_3(B, G, BP % DRV_2_PIN_PER_REG, V, DRV_8_BIT_PER_PIN)
#define SET_DRV_8_BIT_1(BANK, GROUP, BANK_PIN, VAL) \
{                                                   \
    RK_SET_DRV_0(BANK, GROUP, BANK_PIN, VAL);       \
}

#define SET_DRV_8_BIT_2(BANK, GROUP, BANK_PIN, VAL) \
{                                                   \
    if ((BANK_PIN % 8) < 2) {                       \
        RK_SET_DRV_0(BANK, GROUP, BANK_PIN, VAL);   \
    } else {                                        \
        RK_SET_DRV_1(BANK, GROUP, BANK_PIN, VAL);   \
    }                                               \
}

#define SET_DRV_8_BIT_3(BANK, GROUP, BANK_PIN, VAL) \
{                                                   \
    if ((BANK_PIN % 8) < 2) {                       \
        RK_SET_DRV_0(BANK, GROUP, BANK_PIN, VAL);   \
    } else if ((BANK_PIN % 8) < 4) {                \
        RK_SET_DRV_1(BANK, GROUP, BANK_PIN, VAL);   \
    } else {                                        \
        RK_SET_DRV_2(BANK, GROUP, BANK_PIN, VAL);   \
    }                                               \
}

#define SET_DRV_8_BIT_4(BANK, GROUP, BANK_PIN, VAL) \
{                                                   \
    if ((BANK_PIN % 8) < 2) {                       \
        RK_SET_DRV_0(BANK, GROUP, BANK_PIN, VAL);   \
    } else if ((BANK_PIN % 8) < 4) {                \
        RK_SET_DRV_1(BANK, GROUP, BANK_PIN, VAL);   \
    } else if ((BANK_PIN % 8) < 6) {                \
        RK_SET_DRV_2(BANK, GROUP, BANK_PIN, VAL);   \
    } else {                                        \
        RK_SET_DRV_3(BANK, GROUP, BANK_PIN, VAL);   \
    }                                               \
}

#define PINCTRL_SET_DRV_8_BIT(bank, bank_pin, val) \
{                                                  \
    if (bank_pin < 8) {                            \
        SET_DRV_8_BIT_4(bank, A, bank_pin, val);   \
    } else if (bank_pin < 16) {                    \
        SET_DRV_8_BIT_4(bank, B, bank_pin, val);   \
    } else if (bank_pin < 24) {                    \
        SET_DRV_8_BIT_4(bank, C, bank_pin, val);   \
    } else if (bank_pin < 32) {                    \
        SET_DRV_8_BIT_4(bank, D, bank_pin, val);   \
    } else {                                       \
        HAL_DBG("unknown pin %d\n", bank_pin);     \
    }                                              \
}

#define SR_2_BIT_PER_PIN                         (2)
#define SR_8_PIN_PER_REG                         (8)
#define SR_0(__B, __G)                           (GPIO##__B##_IOC->GPIO##__B##__G##_SL)
#define SET_SR_0(_B, _G, _GP, _V, _W)            _PINCTRL_WRITE(SR_0(_B, _G), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_SR_0(B, G, BP, V)                 SET_SR_0(B, G, BP % SR_8_PIN_PER_REG, V, SR_2_BIT_PER_PIN)
#define SET_SR_2_BIT(BANK, GROUP, BANK_PIN, VAL) RK_SET_SR_0(BANK, GROUP, BANK_PIN, VAL)

#define PINCTRL_SET_SR_2_BIT(bank, bank_pin, val) \
{                                                 \
    if (bank_pin < 8) {                           \
        SET_SR_2_BIT(bank, A, bank_pin, val);     \
    } else if (bank_pin < 16) {                   \
        SET_SR_2_BIT(bank, B, bank_pin, val);     \
    } else if (bank_pin < 24) {                   \
        SET_SR_2_BIT(bank, C, bank_pin, val);     \
    } else if (bank_pin < 32) {                   \
        SET_SR_2_BIT(bank, D, bank_pin, val);     \
    } else {                                      \
        HAL_DBG("unknown pin %d\n", bank_pin);    \
    }                                             \
}

#define SMT_1_BIT_PER_PIN                         (1)
#define SMT_8_PIN_PER_REG                         (8)
#define SMT_0(__B, __G)                           (GPIO##__B##_IOC->GPIO##__B##__G##_IE_SMT)
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
        SET_IOMUX_4_BIT(0, A, pin, val);
        break;
    case 1:
        if (pin < 8) {
            SET_IOMUX_4_BIT(1, A, pin, val);
        } else if (pin < 16) {
            RK_SET_IOMUX_L(1, B, pin, val);
        } else if (pin < 24) {
            SET_IOMUX_4_BIT(1, C, pin, val);
        } else {
            RK_SET_IOMUX_L(1, D, pin, val);
        }
        break;
    case 2:
        if (pin < 8) {
            SET_IOMUX_4_BIT(2, A, pin, val);
        } else {
            RK_SET_IOMUX_L(2, B, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_IOMUX_4_BIT(3, A, pin, val);
        } else if (pin < 16) {
            SET_IOMUX_4_BIT(3, B, pin, val);
        } else if (pin < 24) {
            SET_IOMUX_4_BIT(3, C, pin, val);
        } else {
            RK_SET_IOMUX_L(3, D, pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            SET_IOMUX_4_BIT(4, A, pin, val);
        } else if (pin < 16) {
            RK_SET_IOMUX_L(4, B, pin, val);
        } else {
            RK_SET_IOMUX_L(4, C, pin, val);
        }
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
        SET_PULL_2_BIT(0, A, pin, val);
        break;
    case 1:
        PINCTRL_SET_PULL_2_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_PULL_2_BIT(2, A, pin, val);
        } else {
            SET_PULL_2_BIT(2, B, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_PULL_2_BIT(3, A, pin, val);
        } else if (pin < 24) {
            SET_PULL_2_BIT(3, C, pin, val);
        } else {
            SET_PULL_2_BIT(3, D, pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            SET_PULL_2_BIT(4, A, pin, val);
        } else {
            SET_PULL_2_BIT(4, B, pin, val);
        }
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
        SET_DRV_8_BIT_4(0, A, pin, val);
        break;
    case 1:
        if (pin < 8) {
            SET_DRV_8_BIT_3(1, A, pin, val);
        } else if (pin < 16) {
            SET_DRV_8_BIT_2(1, B, pin, val);
        } else if (pin < 24) {
            SET_DRV_8_BIT_4(1, C, pin, val);
        } else {
            SET_DRV_8_BIT_2(1, D, pin, val);
        }
        break;
    case 2:
        if (pin < 8) {
            SET_DRV_8_BIT_4(2, A, pin, val);
        } else {
            SET_DRV_8_BIT_1(2, B, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_DRV_8_BIT_4(3, A, pin, val);
        } else if (pin < 24) {
            if ((pin % 8) < 6) {
                RK_SET_DRV_2(3, C, pin, val);
            } else {
                RK_SET_DRV_3(3, C, pin, val);
            }
        } else {
            SET_DRV_8_BIT_2(3, D, pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            SET_DRV_8_BIT_4(4, A, pin, val);
        } else {
            SET_DRV_8_BIT_1(4, B, pin, val);
        }
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
        SET_SR_2_BIT(0, A, pin, val);
        break;
    case 1:
        PINCTRL_SET_SR_2_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_SR_2_BIT(2, A, pin, val);
        } else {
            SET_SR_2_BIT(2, B, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_SR_2_BIT(3, A, pin, val);
        } else if (pin < 24) {
            SET_SR_2_BIT(3, C, pin, val);
        } else {
            SET_SR_2_BIT(3, D, pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            SET_SR_2_BIT(4, A, pin, val);
        } else {
            SET_SR_2_BIT(4, B, pin, val);
        }
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
        SET_SMT_1_BIT(0, A, pin, val);
        break;
    case 1:
        PINCTRL_SET_SMT_1_BIT(1, pin, val);
        break;
    case 2:
        if (pin < 8) {
            SET_SMT_1_BIT(2, A, pin, val);
        } else {
            SET_SMT_1_BIT(2, B, pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            SET_SMT_1_BIT(3, A, pin, val);
        } else if (pin < 24) {
            SET_SMT_1_BIT(3, C, pin, val);
        } else {
            SET_SMT_1_BIT(3, D, pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            SET_SMT_1_BIT(4, A, pin, val);
        } else {
            SET_SMT_1_BIT(4, B, pin, val);
        }
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

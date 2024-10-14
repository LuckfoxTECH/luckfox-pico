/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(HAL_PINCTRL_MODULE_ENABLED) && defined(SOC_RK3588)

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
   - HAL_PINCTRL_SetParam() to set pin iomux/pull/drive strength/schmitt trigger

 Example:

     HAL_PINCTRL_SetIOMUX(GPIO_BANK1,
                          GPIO_PIN_D2 | // UART4_TX_M0
                          GPIO_PIN_D3,  // UART4_RX_M0
                          PIN_CONFIG_MUX_FUNC10);

     HAL_PINCTRL_SetParam(GPIO_BANK1,
                          GPIO_PIN_D2 | // UART4_TX_M0
                          GPIO_PIN_D3,  // UART4_RX_M0
                          PIN_CONFIG_MUX_FUNC10 |
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
#define SET_IOMUX_0AL(_GP, _V, _W)       _PINCTRL_WRITE((PMU1_IOC->GPIO0A_IOMUX_SEL_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0AH(_GP, _V, _W)       _PINCTRL_WRITE((PMU1_IOC->GPIO0A_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0BL(_GP, _V, _W)       _PINCTRL_WRITE((PMU1_IOC->GPIO0B_IOMUX_SEL_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0BH(_GP, _V, _W)       _PINCTRL_WRITE((PMU2_IOC->GPIO0B_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0CL(_GP, _V, _W)       _PINCTRL_WRITE((PMU2_IOC->GPIO0C_IOMUX_SEL_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0CH(_GP, _V, _W)       _PINCTRL_WRITE((PMU2_IOC->GPIO0C_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0DL(_GP, _V, _W)       _PINCTRL_WRITE((PMU2_IOC->GPIO0D_IOMUX_SEL_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0DH(_GP, _V, _W)       _PINCTRL_WRITE((PMU2_IOC->GPIO0D_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0BH_P(_GP, _V, _W)     _PINCTRL_WRITE((BUS_IOC->GPIO0B_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0CL_P(_GP, _V, _W)     _PINCTRL_WRITE((BUS_IOC->GPIO0C_IOMUX_SEL_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0CH_P(_GP, _V, _W)     _PINCTRL_WRITE((BUS_IOC->GPIO0C_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0DL_P(_GP, _V, _W)     _PINCTRL_WRITE((BUS_IOC->GPIO0D_IOMUX_SEL_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_IOMUX_0DH_P(_GP, _V, _W)     _PINCTRL_WRITE((BUS_IOC->GPIO0D_IOMUX_SEL_H), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_IOMUX_0AL(BP, V)          SET_IOMUX_0AL(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0AH(BP, V)          SET_IOMUX_0AH(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0BL(BP, V)          SET_IOMUX_0BL(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0BH(BP, V)          SET_IOMUX_0BH(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0CL(BP, V)          SET_IOMUX_0CL(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0CH(BP, V)          SET_IOMUX_0CH(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0DL(BP, V)          SET_IOMUX_0DL(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0DH(BP, V)          SET_IOMUX_0DH(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0BH_P(BP, V)        SET_IOMUX_0BH_P(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0CL_P(BP, V)        SET_IOMUX_0CL_P(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0CH_P(BP, V)        SET_IOMUX_0CH_P(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0DL_P(BP, V)        SET_IOMUX_0DL_P(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define RK_SET_IOMUX_0DH_P(BP, V)        SET_IOMUX_0DH_P(BP % IOMUX_4_PIN_PER_REG, V, IOMUX_4_BIT_PER_PIN)
#define IOMUX_L(__B, __G)                (BUS_IOC->GPIO##__B##__G##_IOMUX_SEL_L)
#define IOMUX_H(__B, __G)                (BUS_IOC->GPIO##__B##__G##_IOMUX_SEL_H)
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
    }                                                \
}

#define PULL_2_BIT_PER_PIN         (2)
#define PULL_8_PIN_PER_REG         (8)
#define SET_PULL_0A(_GP, _V, _W)   _PINCTRL_WRITE(PMU1_IOC->GPIO0A_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_0B(_GP, _V, _W)   _PINCTRL_WRITE(PMU1_IOC->GPIO0B_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_0B_P(_GP, _V, _W) _PINCTRL_WRITE(PMU2_IOC->GPIO0B_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_0C(_GP, _V, _W)   _PINCTRL_WRITE(PMU2_IOC->GPIO0C_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_0D(_GP, _V, _W)   _PINCTRL_WRITE(PMU2_IOC->GPIO0D_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_1A(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1A_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_1B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1B_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_1C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1C_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_1D(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1D_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_2A(_GP, _V, _W)   _PINCTRL_WRITE(EMMC_IOC->GPIO2A_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_2A_P(_GP, _V, _W) _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO2A_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_2B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO2B_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_2C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO2C_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_2D(_GP, _V, _W)   _PINCTRL_WRITE(EMMC_IOC->GPIO2D_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_3A(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3A_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_3B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3B_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_3C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3C_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_3D(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3D_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_4A(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO6_IOC->GPIO4A_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_4B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO6_IOC->GPIO4B_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_4C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO6_IOC->GPIO4C_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_4C_P(_GP, _V, _W) _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO4C_P, RK_GEN_VAL(_GP, _V, _W))
#define SET_PULL_4D(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO2_IOC->GPIO4D_P, RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_PULL_0A(BP, V)      SET_PULL_0A(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_0B(BP, V)      SET_PULL_0B(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_0B_P(BP, V)    SET_PULL_0B_P(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_0C(BP, V)      SET_PULL_0C(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_0D(BP, V)      SET_PULL_0D(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_1A(BP, V)      SET_PULL_1A(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_1B(BP, V)      SET_PULL_1B(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_1C(BP, V)      SET_PULL_1C(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_1D(BP, V)      SET_PULL_1D(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_2A(BP, V)      SET_PULL_2A(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_2A_P(BP, V)    SET_PULL_2A_P(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_2B(BP, V)      SET_PULL_2B(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_2C(BP, V)      SET_PULL_2C(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_2D(BP, V)      SET_PULL_2D(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_3A(BP, V)      SET_PULL_3A(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_3B(BP, V)      SET_PULL_3B(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_3C(BP, V)      SET_PULL_3C(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_3D(BP, V)      SET_PULL_3D(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_4A(BP, V)      SET_PULL_4A(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_4B(BP, V)      SET_PULL_4B(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_4C(BP, V)      SET_PULL_4C(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_4C_P(BP, V)    SET_PULL_4C_P(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)
#define RK_SET_PULL_4D(BP, V)      SET_PULL_4D(BP % PULL_8_PIN_PER_REG, V, PULL_2_BIT_PER_PIN)

#define DRV_4_BIT_PER_PIN        (4)
#define DRV_4_PIN_PER_REG        (4)
#define SET_DRV_0AL(_GP, _V, _W) _PINCTRL_WRITE((PMU1_IOC->GPIO0A_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0AH(_GP, _V, _W) _PINCTRL_WRITE((PMU1_IOC->GPIO0A_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0BL(_GP, _V, _W) _PINCTRL_WRITE((PMU1_IOC->GPIO0B_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0BH(_GP, _V, _W) _PINCTRL_WRITE((PMU2_IOC->GPIO0B_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0CL(_GP, _V, _W) _PINCTRL_WRITE((PMU2_IOC->GPIO0C_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0CH(_GP, _V, _W) _PINCTRL_WRITE((PMU2_IOC->GPIO0C_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0DL(_GP, _V, _W) _PINCTRL_WRITE((PMU2_IOC->GPIO0D_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_0DH(_GP, _V, _W) _PINCTRL_WRITE((PMU2_IOC->GPIO0D_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1AL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1A_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1AH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1A_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1BL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1B_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1BH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1B_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1CL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1C_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1CH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1C_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1DL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1D_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_1DH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO1_4_IOC->GPIO1D_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2AL(_GP, _V, _W) _PINCTRL_WRITE((EMMC_IOC->GPIO2A_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2AH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO2A_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2BL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO2B_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2BH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO2B_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2CL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO2C_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2CH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO2C_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2DL(_GP, _V, _W) _PINCTRL_WRITE((EMMC_IOC->GPIO2D_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_2DH(_GP, _V, _W) _PINCTRL_WRITE((EMMC_IOC->GPIO2D_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3AL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3A_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3AH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3A_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3BL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3B_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3BH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3B_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3CL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3C_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3CH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3C_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3DL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3D_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_3DH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO3_5_IOC->GPIO3D_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4AL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO6_IOC->GPIO4A_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4AH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO6_IOC->GPIO4A_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4BL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO6_IOC->GPIO4B_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4BH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO6_IOC->GPIO4B_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4CL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO6_IOC->GPIO4C_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4DL(_GP, _V, _W) _PINCTRL_WRITE((VCCIO2_IOC->GPIO4D_DS_L), RK_GEN_VAL(_GP, _V, _W))
#define SET_DRV_4DH(_GP, _V, _W) _PINCTRL_WRITE((VCCIO2_IOC->GPIO4D_DS_H), RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_DRV_0AL(BP, V)    SET_DRV_0AL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0AH(BP, V)    SET_DRV_0AH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0BL(BP, V)    SET_DRV_0BL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0BH(BP, V)    SET_DRV_0BH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0CL(BP, V)    SET_DRV_0CL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0CH(BP, V)    SET_DRV_0CH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0DL(BP, V)    SET_DRV_0DL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_0DH(BP, V)    SET_DRV_0DH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1AL(BP, V)    SET_DRV_1AL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1AH(BP, V)    SET_DRV_1AH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1BL(BP, V)    SET_DRV_1BL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1BH(BP, V)    SET_DRV_1BH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1CL(BP, V)    SET_DRV_1CL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1CH(BP, V)    SET_DRV_1CH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1DL(BP, V)    SET_DRV_1DL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_1DH(BP, V)    SET_DRV_1DH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2AL(BP, V)    SET_DRV_2AL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2AH(BP, V)    SET_DRV_2AH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2BL(BP, V)    SET_DRV_2BL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2BH(BP, V)    SET_DRV_2BH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2CL(BP, V)    SET_DRV_2CL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2CH(BP, V)    SET_DRV_2CH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2DL(BP, V)    SET_DRV_2DL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_2DH(BP, V)    SET_DRV_2DH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3AL(BP, V)    SET_DRV_3AL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3AH(BP, V)    SET_DRV_3AH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3BL(BP, V)    SET_DRV_3BL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3BH(BP, V)    SET_DRV_3BH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3CL(BP, V)    SET_DRV_3CL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3CH(BP, V)    SET_DRV_3CH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3DL(BP, V)    SET_DRV_3DL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_3DH(BP, V)    SET_DRV_3DH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4AL(BP, V)    SET_DRV_3AL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4AH(BP, V)    SET_DRV_3AH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4BL(BP, V)    SET_DRV_3BL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4BH(BP, V)    SET_DRV_3BH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4CL(BP, V)    SET_DRV_3CL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4CH(BP, V)    SET_DRV_3CH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4DL(BP, V)    SET_DRV_3DL(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)
#define RK_SET_DRV_4DH(BP, V)    SET_DRV_3DH(BP % DRV_4_PIN_PER_REG, V, DRV_4_BIT_PER_PIN)

#define SMT_1_BIT_PER_PIN         (1)
#define SMT_8_PIN_PER_REG         (8)
#define SET_SMT_0A(_GP, _V, _W)   _PINCTRL_WRITE(PMU1_IOC->GPIO0A_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_0B(_GP, _V, _W)   _PINCTRL_WRITE(PMU1_IOC->GPIO0B_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_0B_P(_GP, _V, _W) _PINCTRL_WRITE(PMU2_IOC->GPIO0B_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_0C(_GP, _V, _W)   _PINCTRL_WRITE(PMU2_IOC->GPIO0C_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_0D(_GP, _V, _W)   _PINCTRL_WRITE(PMU2_IOC->GPIO0D_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_1A(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1A_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_1B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1B_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_1C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1C_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_1D(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO1_4_IOC->GPIO1D_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_2A(_GP, _V, _W)   _PINCTRL_WRITE(EMMC_IOC->GPIO2A_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_2A_P(_GP, _V, _W) _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO2A_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_2B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO2B_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_2C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO2C_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_2D(_GP, _V, _W)   _PINCTRL_WRITE(EMMC_IOC->GPIO2D_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_3A(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3A_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_3B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3B_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_3C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3C_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_3D(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO3D_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_4A(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO6_IOC->GPIO4A_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_4B(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO6_IOC->GPIO4B_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_4C(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO6_IOC->GPIO4C_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_4C_P(_GP, _V, _W) _PINCTRL_WRITE(VCCIO3_5_IOC->GPIO4C_SMT, RK_GEN_VAL(_GP, _V, _W))
#define SET_SMT_4D(_GP, _V, _W)   _PINCTRL_WRITE(VCCIO2_IOC->GPIO4D_SMT, RK_GEN_VAL(_GP, _V, _W))
#define RK_SET_SMT_0A(BP, V)      SET_SMT_0A(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_0B(BP, V)      SET_SMT_0B(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_0B_P(BP, V)    SET_SMT_0B_P(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_0C(BP, V)      SET_SMT_0C(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_0D(BP, V)      SET_SMT_0D(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_1A(BP, V)      SET_SMT_1A(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_1B(BP, V)      SET_SMT_1B(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_1C(BP, V)      SET_SMT_1C(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_1D(BP, V)      SET_SMT_1D(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_2A(BP, V)      SET_SMT_2A(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_2A_P(BP, V)    SET_SMT_2A_P(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_2B(BP, V)      SET_SMT_2B(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_2C(BP, V)      SET_SMT_2C(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_2D(BP, V)      SET_SMT_2D(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_3A(BP, V)      SET_SMT_3A(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_3B(BP, V)      SET_SMT_3B(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_3C(BP, V)      SET_SMT_3C(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_3D(BP, V)      SET_SMT_3D(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_4A(BP, V)      SET_SMT_4A(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_4B(BP, V)      SET_SMT_4B(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_4C(BP, V)      SET_SMT_4C(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_4C_P(BP, V)    SET_SMT_4C_P(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)
#define RK_SET_SMT_4D(BP, V)      SET_SMT_4D(BP % SMT_8_PIN_PER_REG, V, SMT_1_BIT_PER_PIN)

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
        if (pin < 4) {
            RK_SET_IOMUX_0AL(pin, val);
        } else if (pin < 8) {
            RK_SET_IOMUX_0AH(pin, val);
        } else if (pin < 12) {
            RK_SET_IOMUX_0BL(pin, val);
        } else if (pin < 16) {
            if (val < 8) {
                RK_SET_IOMUX_0BH(pin, val);
            } else {
                RK_SET_IOMUX_0BH(pin, 8);
                RK_SET_IOMUX_0BH_P(pin, val);
            }
        } else if (pin < 20) {
            if (val < 8) {
                RK_SET_IOMUX_0CL(pin, val);
            } else {
                RK_SET_IOMUX_0CL(pin, 8);
                RK_SET_IOMUX_0CL_P(pin, val);
            }
        } else if (pin < 24) {
            if (val < 8) {
                RK_SET_IOMUX_0CH(pin, val);
            } else {
                RK_SET_IOMUX_0CH(pin, 8);
                RK_SET_IOMUX_0CH_P(pin, val);
            }
        } else if (pin < 28) {
            if (val < 8) {
                RK_SET_IOMUX_0DL(pin, val);
            } else {
                RK_SET_IOMUX_0DL(pin, 8);
                RK_SET_IOMUX_0DL_P(pin, val);
            }
        } else {
            if (val < 8) {
                RK_SET_IOMUX_0DH(pin, val);
            } else {
                RK_SET_IOMUX_0DH(pin, 8);
                RK_SET_IOMUX_0DH_P(pin, val);
            }
        }
        break;
    case 1:
        PINCTRL_SET_IOMUX_4_BIT(1, pin, val);
        break;
    case 2:
        PINCTRL_SET_IOMUX_4_BIT(2, pin, val);
        break;
    case 3:
        PINCTRL_SET_IOMUX_4_BIT(3, pin, val);
        break;
    case 4:
        PINCTRL_SET_IOMUX_4_BIT(4, pin, val);
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
            RK_SET_PULL_0A(pin, val);
        } else if (pin < 12) {
            RK_SET_PULL_0B(pin, val);
        } else if (pin < 16) {
            RK_SET_PULL_0B_P(pin, val);
        } else if (pin < 24) {
            RK_SET_PULL_0C(pin, val);
        } else {
            RK_SET_PULL_0D(pin, val);
        }
        break;
    case 1:
        if (pin < 8) {
            RK_SET_PULL_1A(pin, val);
        } else if (pin < 16) {
            RK_SET_PULL_1B(pin, val);
        } else if (pin < 24) {
            RK_SET_PULL_1C(pin, val);
        } else {
            RK_SET_PULL_1D(pin, val);
        }
        break;
    case 2:
        if (pin < 4) {
            RK_SET_PULL_2A(pin, val);
        } else if (pin < 8) {
            RK_SET_PULL_2A_P(pin, val);
        } else if (pin < 16) {
            RK_SET_PULL_2B(pin, val);
        } else if (pin < 24) {
            RK_SET_PULL_2C(pin, val);
        } else {
            RK_SET_PULL_2D(pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            RK_SET_PULL_3A(pin, val);
        } else if (pin < 16) {
            RK_SET_PULL_3B(pin, val);
        } else if (pin < 24) {
            RK_SET_PULL_3C(pin, val);
        } else {
            RK_SET_PULL_3D(pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            RK_SET_PULL_4A(pin, val);
        } else if (pin < 16) {
            RK_SET_PULL_4B(pin, val);
        } else if (pin < 18) {
            RK_SET_PULL_4C(pin, val);
        } else if (pin < 24) {
            RK_SET_PULL_4C_P(pin, val);
        } else {
            RK_SET_PULL_4D(pin, val);
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
        if (pin < 4) {
            RK_SET_DRV_0AL(pin, val);
        } else if (pin < 8) {
            RK_SET_DRV_0AH(pin, val);
        } else if (pin < 12) {
            RK_SET_DRV_0BL(pin, val);
        } else if (pin < 16) {
            RK_SET_DRV_0BH(pin, val);
        } else if (pin < 20) {
            RK_SET_DRV_0CL(pin, val);
        } else if (pin < 24) {
            RK_SET_DRV_0CH(pin, val);
        } else if (pin < 28) {
            RK_SET_DRV_0DL(pin, val);
        } else {
            RK_SET_DRV_0DH(pin, val);
        }
        break;
    case 1:
        if (pin < 4) {
            RK_SET_DRV_1AL(pin, val);
        } else if (pin < 8) {
            RK_SET_DRV_1AH(pin, val);
        } else if (pin < 12) {
            RK_SET_DRV_1BL(pin, val);
        } else if (pin < 16) {
            RK_SET_DRV_1BH(pin, val);
        } else if (pin < 20) {
            RK_SET_DRV_1CL(pin, val);
        } else if (pin < 24) {
            RK_SET_DRV_1CH(pin, val);
        } else if (pin < 28) {
            RK_SET_DRV_1DL(pin, val);
        } else {
            RK_SET_DRV_1DH(pin, val);
        }
        break;
    case 2:
        if (pin < 4) {
            RK_SET_DRV_2AL(pin, val);
        } else if (pin < 8) {
            RK_SET_DRV_2AH(pin, val);
        } else if (pin < 12) {
            RK_SET_DRV_2BL(pin, val);
        } else if (pin < 16) {
            RK_SET_DRV_2BH(pin, val);
        } else if (pin < 20) {
            RK_SET_DRV_2CL(pin, val);
        } else if (pin < 24) {
            RK_SET_DRV_2CH(pin, val);
        } else if (pin < 28) {
            RK_SET_DRV_2DL(pin, val);
        } else {
            RK_SET_DRV_2DH(pin, val);
        }
        break;
    case 3:
        if (pin < 4) {
            RK_SET_DRV_3AL(pin, val);
        } else if (pin < 8) {
            RK_SET_DRV_3AH(pin, val);
        } else if (pin < 12) {
            RK_SET_DRV_3BL(pin, val);
        } else if (pin < 16) {
            RK_SET_DRV_3BH(pin, val);
        } else if (pin < 20) {
            RK_SET_DRV_3CL(pin, val);
        } else if (pin < 24) {
            RK_SET_DRV_3CH(pin, val);
        } else if (pin < 28) {
            RK_SET_DRV_3DL(pin, val);
        } else {
            RK_SET_DRV_3DH(pin, val);
        }
        break;
    case 4:
        if (pin < 4) {
            RK_SET_DRV_4AL(pin, val);
        } else if (pin < 8) {
            RK_SET_DRV_4AH(pin, val);
        } else if (pin < 12) {
            RK_SET_DRV_4BL(pin, val);
        } else if (pin < 16) {
            RK_SET_DRV_4BH(pin, val);
        } else if (pin < 20) {
            RK_SET_DRV_4CL(pin, val);
        } else if (pin < 24) {
        } else if (pin < 28) {
            RK_SET_DRV_4DL(pin, val);
        } else {
            RK_SET_DRV_4DH(pin, val);
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
        if (pin < 8) {
            RK_SET_SMT_0A(pin, val);
        } else if (pin < 12) {
            RK_SET_SMT_0B(pin, val);
        } else if (pin < 16) {
            RK_SET_SMT_0B_P(pin, val);
        } else if (pin < 24) {
            RK_SET_SMT_0C(pin, val);
        } else {
            RK_SET_SMT_0D(pin, val);
        }
        break;
    case 1:
        if (pin < 8) {
            RK_SET_SMT_1A(pin, val);
        } else if (pin < 16) {
            RK_SET_SMT_1B(pin, val);
        } else if (pin < 24) {
            RK_SET_SMT_1C(pin, val);
        } else {
            RK_SET_SMT_1D(pin, val);
        }
        break;
    case 2:
        if (pin < 4) {
            RK_SET_SMT_2A(pin, val);
        } else if (pin < 8) {
            RK_SET_SMT_2A_P(pin, val);
        } else if (pin < 16) {
            RK_SET_SMT_2B(pin, val);
        } else if (pin < 24) {
            RK_SET_SMT_2C(pin, val);
        } else {
            RK_SET_SMT_2D(pin, val);
        }
        break;
    case 3:
        if (pin < 8) {
            RK_SET_SMT_3A(pin, val);
        } else if (pin < 16) {
            RK_SET_SMT_3B(pin, val);
        } else if (pin < 24) {
            RK_SET_SMT_3C(pin, val);
        } else {
            RK_SET_SMT_3D(pin, val);
        }
        break;
    case 4:
        if (pin < 8) {
            RK_SET_SMT_4A(pin, val);
        } else if (pin < 16) {
            RK_SET_SMT_4B(pin, val);
        } else if (pin < 18) {
            RK_SET_SMT_4C(pin, val);
        } else if (pin < 24) {
            RK_SET_SMT_4C_P(pin, val);
        } else {
            RK_SET_SMT_4D(pin, val);
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

    if (!(param & (FLAG_MUX | FLAG_PUL | FLAG_DRV | FLAG_SMT))) {
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

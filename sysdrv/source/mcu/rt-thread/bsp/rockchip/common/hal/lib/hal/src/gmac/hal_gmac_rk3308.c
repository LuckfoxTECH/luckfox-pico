/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RK3308) && defined(HAL_GMAC1000_MODULE_ENABLED)

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup GMAC
 *  @{
 */

/** @defgroup GMAC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

#define HIWORD_UPDATE(val, mask, shift) \
        ((val) << (shift) | (mask) << ((shift) + 16))

#define GRF_BIT(nr)     (1 << (nr) | 1 << (nr+16))
#define GRF_CLR_BIT(nr) (1 << (nr+16))

#define DELAY_ENABLE(soc, tx, rx)                                          \
    (((tx) ? soc##_GMAC_TXCLK_DLY_ENABLE : soc##_GMAC_TXCLK_DLY_DISABLE) | \
     ((rx) ? soc##_GMAC_RXCLK_DLY_ENABLE : soc##_GMAC_RXCLK_DLY_DISABLE))

/* Rk3308_GRF_MAC_CON1 */
#define RK3308_MAC_PHY_INTF_SEL_RMII (GRF_CLR_BIT(2) | GRF_CLR_BIT(3) | \
                    GRF_BIT(4))
#define RK3308_MAC_SPEED_10M         GRF_CLR_BIT(0)
#define RK3308_MAC_SPEED_100M        GRF_BIT(0)

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/** @} */
/********************* Public Function Definition ****************************/

/** @defgroup GMAC_Exported_Functions_Group5 Other Functions
 *  @{
 */

/**
 * @brief  Set RGMII Mode.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  txDelay: RGMII tx delayline
 * @param  rxDelay: RGMII rx delayline
 */
void HAL_GMAC_SetToRGMII(struct GMAC_HANDLE *pGMAC,
                         int32_t txDelay, int32_t rxDelay)
{
}

/**
 * @brief  Set RMII Mode.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 */
void HAL_GMAC_SetToRMII(struct GMAC_HANDLE *pGMAC)
{
    int32_t ret;

    WRITE_REG(GRF->MAC_CON0, RK3308_MAC_PHY_INTF_SEL_RMII | RK3308_MAC_SPEED_100M);
    ret = HAL_CRU_ClkSetFreq(CLK_MAC, 50000000);
    if (ret) {
        HAL_DBG_ERR("%s: set clk_mac rate 50M failed %ld\n",
                    __func__, ret);
    }
}

/**
 * @brief  Set RMII speed.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  speed: RMII speed 10/100
 */
void HAL_GMAC_SetRGMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed)
{
}

/**
 * @brief  Set external clock source select.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  extClk: 0: select clk_mac as the clock of mac
 *                 1: select external phy clock as the clock of mac
 */
void HAL_GMAC_SetExtclkSrc(struct GMAC_HANDLE *pGMAC, bool extClk)
{
    uint32_t clksel = 0;

    if (extClk) {
        clksel = 1;
    }

    WRITE_REG_MASK_WE(CRU->CRU_CLKSEL_CON[43],
                      CRU_CLKSEL_CON43_RMII_EXTCLKSRC_SEL_MASK,
                      clksel << CRU_CLKSEL_CON43_RMII_EXTCLKSRC_SEL_SHIFT);
}

/**
 * @brief  Set RMII speed.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  speed: RMII speed 10/100
 */
void HAL_GMAC_SetRMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed)
{
    uint32_t clksel = 0;

    switch (speed) {
    case 10:
        WRITE_REG(GRF->MAC_CON0, RK3308_MAC_PHY_INTF_SEL_RMII | RK3308_MAC_SPEED_10M);
        clksel = 0;
        break;
    case 100:
        WRITE_REG(GRF->MAC_CON0, RK3308_MAC_PHY_INTF_SEL_RMII | RK3308_MAC_SPEED_100M);
        clksel = 1;
        break;
    default:
        HAL_DBG_ERR("unknown speed value for GMAC speed=%ld", speed);

        return;
    }

    WRITE_REG_MASK_WE(CRU->CRU_CLKSEL_CON[43],
                      CRU_CLKSEL_CON43_RMII_CLK_SEL_SHIFT,
                      clksel << CRU_CLKSEL_CON43_RMII_CLK_SEL_SHIFT);
}

/** @} */

/** @} */

/** @} */

#endif /* SOC_RK3308 && HAL_GMAC_MODULE_ENABLED */

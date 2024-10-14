/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RK3358) && defined(HAL_GMAC1000_MODULE_ENABLED)

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

/* PX30_GRF_GMAC_CON1 */
#define PX30_GMAC_PHY_INTF_SEL_RMII (GRF_CLR_BIT(4) | GRF_CLR_BIT(5) | \
                                       GRF_BIT(6))
#define PX30_GMAC_SPEED_10M         GRF_CLR_BIT(2)
#define PX30_GMAC_SPEED_100M        GRF_BIT(2)

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
    WRITE_REG(GRF->MAC_CON[0], PX30_GMAC_PHY_INTF_SEL_RMII | PX30_GMAC_SPEED_100M);
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

    WRITE_REG_MASK_WE(CRU->CRU_CLKSEL_CON[23],
                      CRU_CLKSEL_CON23_RMII_EXTCLKSRC_SEL_MASK,
                      clksel << CRU_CLKSEL_CON23_RMII_EXTCLKSRC_SEL_SHIFT);
}

/**
 * @brief  Set RMII speed.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  speed: RMII speed 10/100
 */
void HAL_GMAC_SetRMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed)
{
    uint32_t rate;
    int32_t ret;

    switch (speed) {
    case 10:
        WRITE_REG(GRF->MAC_CON[0], PX30_GMAC_SPEED_10M);
        rate = 2500000;
        break;
    case 100:
        WRITE_REG(GRF->MAC_CON[0], PX30_GMAC_SPEED_100M);
        rate = 25000000;
        break;
    default:
        HAL_DBG_ERR("unknown speed value for GMAC speed=%ld", speed);

        return;
    }

    ret = HAL_CRU_ClkSetFreq(RMII_CLK_SEL, rate);
    if (ret) {
        HAL_DBG_ERR("%s: set clk_mac_speed rate %ld failed %ld\n",
                    __func__, rate, ret);
    }
}

/** @} */

/** @} */

/** @} */

#endif /* SOC_RK3568 && HAL_GMAC_MODULE_ENABLED */

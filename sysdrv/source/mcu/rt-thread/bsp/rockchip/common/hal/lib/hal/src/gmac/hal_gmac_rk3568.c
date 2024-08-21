/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#if defined(SOC_RK3568) && defined(HAL_GMAC_MODULE_ENABLED)

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

#define RK3568_GRF_GMAC0_CON0 0X0380
#define RK3568_GRF_GMAC0_CON1 0X0384
#define RK3568_GRF_GMAC1_CON0 0X0388
#define RK3568_GRF_GMAC1_CON1 0X038c

/* RK3568_GRF_GMAC0_CON1 && RK3568_GRF_GMAC1_CON1 */
#define RK3568_GMAC_GMII_MODE GRF_BIT(7)
#define RK3568_GMAC_PHY_INTF_SEL_RGMII \
        (GRF_BIT(4) | GRF_CLR_BIT(5) | GRF_CLR_BIT(6))
#define RK3568_GMAC_PHY_INTF_SEL_RMII \
        (GRF_CLR_BIT(4) | GRF_CLR_BIT(5) | GRF_BIT(6))
#define RK3568_GMAC_FLOW_CTRL         GRF_BIT(3)
#define RK3568_GMAC_FLOW_CTRL_CLR     GRF_CLR_BIT(3)
#define RK3568_GMAC_RXCLK_DLY_ENABLE  GRF_BIT(1)
#define RK3568_GMAC_RXCLK_DLY_DISABLE GRF_CLR_BIT(1)
#define RK3568_GMAC_TXCLK_DLY_ENABLE  GRF_BIT(0)
#define RK3568_GMAC_TXCLK_DLY_DISABLE GRF_CLR_BIT(0)

/* RK3568_GRF_GMAC0_CON0 && RK3568_GRF_GMAC1_CON0 */
#define RK3568_GMAC_CLK_RX_DL_CFG(val) HIWORD_UPDATE(val, 0x7F, 8)
#define RK3568_GMAC_CLK_TX_DL_CFG(val) HIWORD_UPDATE(val, 0x7F, 0)

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
    uint32_t *con0, *con1;

    con0 = (uint32_t *)((pGMAC->pReg == GMAC1) ? &(GRF->MAC1_CON0) :
                                                 &(GRF->MAC0_CON0));
    con1 = (uint32_t *)((pGMAC->pReg == GMAC1) ? &(GRF->MAC1_CON1) :
                                                 &(GRF->MAC0_CON1));

    WRITE_REG(*con1,
              RK3568_GMAC_PHY_INTF_SEL_RGMII |
              RK3568_GMAC_RXCLK_DLY_ENABLE |
              RK3568_GMAC_TXCLK_DLY_ENABLE);

    WRITE_REG(*con0,
              RK3568_GMAC_CLK_RX_DL_CFG(rxDelay) |
              RK3568_GMAC_CLK_TX_DL_CFG(txDelay));
}

/**
 * @brief  Set RMII Mode.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 */
void HAL_GMAC_SetToRMII(struct GMAC_HANDLE *pGMAC)
{
    uint32_t *con1, *cruCon, val;

    con1 = (uint32_t *)((pGMAC->pReg == GMAC1) ? &(GRF->MAC1_CON1) :
                                                 &(GRF->MAC0_CON1));

    WRITE_REG(*con1, RK3568_GMAC_PHY_INTF_SEL_RMII);

    cruCon = (uint32_t *)((pGMAC->pReg == GMAC1) ? &(CRU->CRU_CLKSEL_CON[33]) :
                                                   &(CRU->CRU_CLKSEL_CON[31]));
    /* RMII mode */
    val = HIWORD_UPDATE(0x1, 0x3, 0);
    /* clock from io if it was */
    /* val |= HIWORD_UPDATE(0x1, 0x1, 2);  */
    /* ref clock sel 50M */
    val |= HIWORD_UPDATE(0x1, 0x3, 8);
    /* clock speed 25M */
    val |= HIWORD_UPDATE(0x1, 0x1, 3);
    WRITE_REG(*cruCon, val);
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
    uint32_t *cruCon, val;
    uint32_t clksel = 0;

    cruCon = (uint32_t *)((pGMAC->pReg == GMAC1) ? &(CRU->CRU_CLKSEL_CON[33]) :
                                                   &(CRU->CRU_CLKSEL_CON[31]));

    if (extClk) {
        clksel = 1;
    }

    val = HIWORD_UPDATE(clksel, 0x1, 2);
    WRITE_REG(*cruCon, val);
}

/**
 * @brief  Set RGMII speed.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  speed: RGMII speed 10/100/1000
 */
void HAL_GMAC_SetRGMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed)
{
    eCLOCK_Name clkID;
    uint32_t rate;
    int32_t ret;

    switch (speed) {
    case 10:
        rate = 2500000;
        break;
    case 100:
        rate = 25000000;
        break;
    case 1000:
        rate = 125000000;
        break;
    default:
        HAL_DBG_ERR("unknown speed value for GMAC speed=%ld", speed);

        return;
    }

    if (pGMAC->phyStatus.interface == PHY_INTERFACE_MODE_RMII) {
        clkID = (pGMAC->pReg == GMAC1) ? SCLK_GMAC1_RMII_SPEED :
                                         SCLK_GMAC0_RMII_SPEED;
    } else {
        clkID = (pGMAC->pReg == GMAC1) ? SCLK_GMAC1_RGMII_SPEED :
                                         SCLK_GMAC0_RGMII_SPEED;
    }

    ret = HAL_CRU_ClkSetFreq(clkID, rate);
    if (ret) {
        HAL_DBG_ERR("%s: set clk_mac_speed rate %ld failed %ld\n",
                    __func__, rate, ret);
    }
}

/**
 * @brief  Set RGMII speed.
 * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
 *                the information for GMAC module.
 * @param  speed: RGMII speed 10/100
 */
void HAL_GMAC_SetRMIISpeed(struct GMAC_HANDLE *pGMAC, int32_t speed)
{
    HAL_GMAC_SetRGMIISpeed(pGMAC, speed);
}

/** @} */

/** @} */

/** @} */

#endif /* SOC_RK3568 && HAL_GMAC_MODULE_ENABLED */

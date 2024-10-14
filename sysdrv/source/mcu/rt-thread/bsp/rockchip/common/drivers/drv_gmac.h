/**
  * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gmac.h
  * @author  David Wu
  * @date    17-Jun-2021
  * @brief   gmac eth driver
  *
  ******************************************************************************
  */

#ifndef _DRV_GMAC_H_
#define _DRV_GMAC_H_

/*******************************************************************************
 * Included Files
 ******************************************************************************/

#include <rtdevice.h>

#ifdef RT_USING_GMAC

#include "hal_base.h"

/*******************************************************************************
 * Pre-processor Definitions
 ******************************************************************************/
enum phy_mode
{
    MII_MODE,
    RMII_MODE,
    RGMII_MODE,
    NONE_MODE,  /* Must be last */
};

/*******************************************************************************
 * Public Types
 ******************************************************************************/

struct GMAC_REG;

/* GMAC consumer config data. */
struct rockchip_eth_config
{
    struct GMAC_REG *id;
    enum phy_mode mode;
    uint32_t speed;
    uint32_t max_speed;
    uint16_t phy_addr;

    /* phy reset gpio */
    struct GPIO_REG *reset_gpio_bank;
    ePINCTRL_GPIO_PINS reset_gpio_num;
    uint32_t reset_delay_ms[3];

    int32_t tx_delay;
    int32_t rx_delay;
};

/*******************************************************************************
 * Public Data
 ******************************************************************************/

extern const struct rockchip_eth_config rockchip_eth_config_table[];

/*******************************************************************************
 * Inline Functions
 ******************************************************************************/

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

#endif
#endif /* _DRV_GMAC_H_ */

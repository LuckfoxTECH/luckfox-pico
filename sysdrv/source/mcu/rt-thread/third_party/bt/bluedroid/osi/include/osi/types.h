// Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef __TYPES_H___
#define __TYPES_H___

#include <rthw.h>
#include <board.h>
#include "rtthread.h"


#define OSI_EOK             RT_EOK
#define OSI_ERROR           (-RT_ERROR)

#define OSI_MAX_TIMEOUT     0xffffffffUL

#define OSI_MS_TO_TICK      (1000/RT_TICK_PER_SECOND)

typedef int32_t osi_err_t;

/**
 * @brief Modules for modem sleep
 */
typedef enum
{
    MODEM_BLE_MODULE,              //!< BLE controller used
    MODEM_CLASSIC_BT_MODULE,       //!< Classic BT controller used
    MODEM_WIFI_STATION_MODULE,     //!< Wi-Fi Station used
    MODEM_WIFI_SOFTAP_MODULE,      //!< Wi-Fi SoftAP used
    MODEM_WIFI_SNIFFER_MODULE,     //!< Wi-Fi Sniffer used
    MODEM_WIFI_NULL_MODULE,        //!< Wi-Fi Null mode used
    MODEM_USER_MODULE,             //!< User used
    MODEM_MODULE_COUNT             //!< Number of items
} modem_sleep_module_t;


typedef enum
{
    PERIPH_LEDC_MODULE = 0,
    PERIPH_UART0_MODULE,
    PERIPH_UART1_MODULE,
    PERIPH_UART2_MODULE,
    PERIPH_I2C0_MODULE,
    PERIPH_I2C1_MODULE,
    PERIPH_I2S0_MODULE,
    PERIPH_I2S1_MODULE,
    PERIPH_TIMG0_MODULE,
    PERIPH_TIMG1_MODULE,
    PERIPH_PWM0_MODULE,
    PERIPH_PWM1_MODULE,
    PERIPH_PWM2_MODULE,
    PERIPH_PWM3_MODULE,
    PERIPH_UHCI0_MODULE,
    PERIPH_UHCI1_MODULE,
    PERIPH_RMT_MODULE,
    PERIPH_PCNT_MODULE,
    PERIPH_SPI_MODULE,
    PERIPH_HSPI_MODULE,
    PERIPH_VSPI_MODULE,
    PERIPH_SPI_DMA_MODULE,
    PERIPH_SDMMC_MODULE,
    PERIPH_SDIO_SLAVE_MODULE,
    PERIPH_CAN_MODULE,
    PERIPH_EMAC_MODULE,
    PERIPH_RNG_MODULE,
    PERIPH_WIFI_MODULE,
    PERIPH_BT_MODULE,
    PERIPH_WIFI_BT_COMMON_MODULE,
    PERIPH_BT_BASEBAND_MODULE,
    PERIPH_BT_LC_MODULE,
    PERIPH_AES_MODULE,
    PERIPH_SHA_MODULE,
    PERIPH_RSA_MODULE,
} periph_module_t;

#define RTC_CLK_CAL_FRACT  19

/**
 * @brief Modules needing to call phy_rf_init
 */
typedef enum
{
    PHY_BT_MODULE,          //!< Bluetooth used
    PHY_WIFI_MODULE,        //!< Wi-Fi used
    PHY_MODEM_MODULE,       //!< Modem sleep used
    PHY_MODULE_COUNT        //!< Number of items
} phy_rf_module_t;

#define OSI_NULL  RT_NULL


#define RK_TASK_PRIO_MAX (RT_THREAD_PRIORITY_MAX)
#define RK_TASK_PRIO_MIN (0)


#define osi_inline  rt_inline

#endif //__TYPES_H___


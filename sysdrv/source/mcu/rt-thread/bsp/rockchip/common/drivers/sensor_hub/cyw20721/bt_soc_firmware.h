/*******************************************************************************
* \file cybtsoc_bt_firmware.h
*
* \brief
* This file provides functions for bt firmware download
*
********************************************************************************
* \copyright
* Copyright 2018-2020 Cypress Semiconductor Corporation
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*******************************************************************************/

#ifndef CYBTSOC_BT_FIRMWARE_H_
#define CYBTSOC_BT_FIRMWARE_H_

#include <rtdevice.h>
#include <rthw.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <rtthread.h>

static inline int cy_rtos_delay_milliseconds(uint32_t timeout)
{
    return rt_thread_mdelay(timeout);
}

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               extern Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
cybtsoc_result_t cybtsoc_bt_issue_reset(void);
cybtsoc_result_t cybtsoc_bt_firmware_dowload(const uint8_t *ram_image, uint32_t ram_size, const char *ram_version,
        const uint8_t *sflash_image, uint32_t sflash_size, const char *sflash_version);
cybtsoc_result_t cybtsoc_bt_start_firmware_dowload(uint32_t rate);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CYBTSOC_BT_FIRMWARE_H_ */
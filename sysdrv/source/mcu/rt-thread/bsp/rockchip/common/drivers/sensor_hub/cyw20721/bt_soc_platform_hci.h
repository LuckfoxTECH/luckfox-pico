/*******************************************************************************
* \file cybtsoc_platform_hci.h
*
* \brief
* Provides API to access BT HCI transport.
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

#ifndef CYBTSOC_PLATFORM_HCI_H
#define CYBTSOC_PLATFORM_HCI_H

#include "bt_soc_result.h"
#include <stdbool.h>
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

/******************************************************************************
 *                                Constants
 ******************************************************************************/
#define HCI_UART_TYPE_HEADER_SIZE  (1)         /**< HCI UART header size */
#define HCI_UART_DEFAULT_BAUDRATE  (115200)    /**< HCI UART default controller
                                               *    baudrate
                                               *
                                               */
/*****************************************************************************
 *                           Type Definitions
 *****************************************************************************/
/**
 * HCI packet type
 */
typedef enum
{
    HCI_PACKET_TYPE_IGNORE   = 0x00,
    HCI_PACKET_TYPE_COMMAND  = 0x01,
    HCI_PACKET_TYPE_EVENT    = 0x04,
    HCI_PACKET_TYPE_WICED_PKT = 0x19,    /* wiced hci protocol */
    HCI_PACKET_TYPE_LOOPBACK = 0xFF
} hci_packet_type_t;

/**
 * HCI Event packet header
 */
typedef struct
{
    uint8_t           event_code;
    uint8_t           content_length;
} hci_event_packet_header_t;

/**
 * Wiced HCI packet header
 */
typedef struct
{
    uint16_t           opcode;
    uint16_t           content_length;
} wiced_hci_packet_header_t;


typedef enum
{
    HCI_CMD_OPCODE_RESET               = 0x0C03,
    HCI_CMD_OPCODE_UPDATE_BAUDRATE     = 0xFC18,
    HCI_CMD_OPCODE_DOWNLOAD_MINIDRIVER = 0xFC2E,
    HCI_CMD_OPCODE_WRITE_RAM           = 0xFC4C,
    HCI_CMD_OPCODE_READ_RAM            = 0xFC4D,
    HCI_CMD_OPCODE_LAUNCH_RAM          = 0xFC4E,
    HCI_CMD_OPCODE_CRC_CHECK           = 0xFCCC,
    HCI_CMD_OPCODE_CHIP_ERASE          = 0xFFCE,

} hci_command_opcode_t;

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 *                           Function Declarations
 ****************************************************************************/
void cyw_wakeup_ctrl(int en);
rt_bool_t wiced_hci_send_command(uint16_t command, char *payload, uint32_t len);

/**
 * Initialize and open HCI transport.
 *
 * @return the status of open operation
 */
cybtsoc_result_t cybtsoc_platform_hci_open(void);

/**
 * Set the new baudrate of HCI UART trasnport.
 *
 * @param[in] req_size: the requested size of memory
 *
 * @returns the status of set baudrate operation
 */
cybtsoc_result_t cybtsoc_platform_hci_uart_set_baudrate(uint32_t baudrate);

/**
 * Write data to HCI transport.
 *
 * Note: This fuction shall be returned only when all data was written done.
 *
 * @param[in] p_data: the pointer of the data to be written
 * @param[in] lenght: the length of the data to be written
 *
 * @returns the status of write operation
 */
cybtsoc_result_t cybtsoc_platform_hci_write(uint8_t *data, uint32_t length);



/**
 * Read data from HCI transpot.
 *
 * @param[in] p_data: the pointer of received buffer
 * @param[in/out] p_length: The pointer of requested/actual length.
 *                          The request read length shall be specified
 *                          through this parameter. The actual read length
 *                          shall be provided in the same parameter,
 *                          along with function return.
 * @param[in] timeout_ms: the timeout (millisecond) to wait to
 *                        receive data from HCI transport
 *
 * @returns the status of read operation
 */
cybtsoc_result_t cybtsoc_platform_hci_read(uint8_t *data, uint32_t *length, uint32_t timeout_ms);


/**
 * Close HCI transport.
 *
 * @returns the status of close operation
 */
cybtsoc_result_t cybtsoc_platform_hci_close(void);

cybtsoc_result_t cyw_wait_replay(uint16_t cmd, void *buffer);

void cyw_wait_cnt_increase(void);
void cyw_wait_cnt_decrease(void);
uint32_t cyw_wait_cnt_get(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
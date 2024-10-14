/*******************************************************************************
* \file cybtsoc_result.h
*
* \brief
* Defines BT basic function result. Based on this information it's easy to know
* whether the result is succeed or error with detail description.
*
********************************************************************************
* \copyright
* Copyright 2018-2019 Cypress Semiconductor Corporation
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

#ifndef CYBTSOC_RESULT_H
#define CYBTSOC_RESULT_H

/*****************************************************************************
 *                           Type Definitions
 ****************************************************************************/
/** BT SOC result code */
typedef enum
{
    CYBTSOC_SUCCESS = 0,
    CYBTSOC_ERR_BADARG = 0xB1,
    CYBTSOC_ERR_OUT_OF_MEMORY,
    CYBTSOC_ERR_TIMEOUT,
    CYBTSOC_ERR_HCI_INIT_FAILED,
    CYBTSOC_ERR_HCI_UNSUPPORTED_IF,
    CYBTSOC_ERR_HCI_UNSUPPORTED_BAUDRATE,
    CYBTSOC_ERR_HCI_NOT_INITIALIZE,
    CYBTSOC_ERR_HCI_WRITE_FAILED,
    CYBTSOC_ERR_HCI_READ_FAILED,
    CYBTSOC_ERR_HCI_GET_TX_MUTEX_FAILED,
    CYBTSOC_ERR_HCI_GET_RX_MUTEX_FAILED,
    CYBTSOC_ERR_HCI_SET_BAUDRATE_FAILED,
    CYBTSOC_ERR_HCI_SET_FLOW_CTRL_FAILED,
    CYBTSOC_ERR_INIT_MEMPOOL_FAILED,
    CYBTSOC_ERR_INIT_QUEUE_FAILED,
    CYBTSOC_ERR_INIT_SEM_FAILED,
    CYBTSOC_ERR_CREATE_TASK_FAILED,
    CYBTSOC_ERR_SEND_QUEUE_FAILED,
    CYBTSOC_ERR_SET_SEM_FAILED,
    CYBTSOC_ERR_MEMPOOL_NOT_INITIALIZE,
    CYBTSOC_ERR_QUEUE_ALMOST_FULL,
    CYBTSOC_ERR_QUEUE_FULL,
    CYBTSOC_ERR_GENERIC,
    CYBTSOC_ERR_PLAT_NOT_INITIALIZE,
    CYBTSOC_ERR_PLAT_IO_NC,
    CYBTSOC_ERR_BT_FMDL_UNKNOWN_PACKET,
    CYBTSOC_ERR_BT_FMDL_CRC_FAIL,
    CYBTSOC_ERR_BT_CMD_RSP_MISMATCH,
    CYBTSOC_ERR_BT_CMD_RSP_UNKNOWN,
    CYBTSOC_ERR_BT_CMD_RSP_BREAK,
    CYBTSOC_ERR_BT_CMD_RSP_FAILED,


} cybtsoc_result_t;
#endif

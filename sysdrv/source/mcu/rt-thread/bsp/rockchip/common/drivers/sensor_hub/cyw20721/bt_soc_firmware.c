/*******************************************************************************
* \file cybtsoc_bt_firmware.c
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
#include <stdio.h>
#include <string.h>

#include "bt_soc_platform_hci.h"
#include "bt_soc_firmware.h"
#include "bt_soc_uart.h"
#include "bt_soc_platform_task.h"
#include "hci_control_api.h"

/* use crc function in wiced first, may use PSOC6 crypto lib future. */
#include "crc.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define DEFAULT_READ_TIMEOUT (100)

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
 *              extern Variable Definitions
 ******************************************************/
extern const char cyw_btsoc_minidriver_version[];
extern const uint8_t cyw_btsoc_minidriver_format;
extern const uint8_t cyw_btsoc_minidriver_buf[];
extern const int cyw_btsoc_minidriver_config_length;

extern const char cyw_btsoc_version[];
extern const uint8_t cyw_btsoc_format;
extern const uint8_t cyw_btsoc_fw_config_buf[];
extern const int cyw_btsoc_fw_config_length;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static struct rt_semaphore rx_sem;

static rt_err_t uart_rx_ind(rt_device_t dev, rt_size_t size)
{
    if (size > 0)
    {
        rt_sem_release(&rx_sem);
    }
    return RT_EOK;
}

static char uart_sample_get_char(uint32_t timeout_ms)
{
    char ch = 0;
    uint32_t size = 1;

    while (rk_uart_read(&ch, &size, 0) != 0)
    {
        rt_sem_control(&rx_sem, RT_IPC_CMD_RESET, RT_NULL);
        if (rt_sem_take(&rx_sem, timeout_ms) != RT_EOK)
            break;
    }

    return ch;
}

static rt_err_t uart_read(uint8_t *p_data, uint32_t *length, uint32_t timeout_ms)
{
    uint32_t len = *length;

    while (len--)
        *p_data++ = uart_sample_get_char(timeout_ms);

    return 0;
}

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
static cybtsoc_result_t bt_hci_vendor_spec_write(hci_packet_type_t type, hci_command_opcode_t opcode, uint8_t *p_data, uint8_t len)
{
    cybtsoc_result_t result = CYBTSOC_SUCCESS;
    char hci_data[256];
    uint32_t hci_len = len + 4;

    if (len + 4 >= 256)
    {
        return CYBTSOC_ERR_BADARG;
    }

    hci_data[0] = type;
    hci_data[1] = opcode & 0xff;
    hci_data[2] = (opcode >> 8) & 0xff;
    hci_data[3] = len;

    if ((p_data != NULL) && (len != 0))
    {
        memcpy(&hci_data[4], p_data, len);
    }


    result = rk_uart_write(hci_data, hci_len);

    return result;
}

static cybtsoc_result_t bt_hci_vendor_spec_rsp_check(hci_command_opcode_t opcode, uint8_t *p_para_data, uint8_t para_len, uint32_t timeout_ms)
{
    cybtsoc_result_t result = CYBTSOC_SUCCESS;

    uint32_t read_len = para_len + 7; /*  */
    uint8_t event_header[7] = {0x04, 0x0E, 0xff, 0xff, 0xff, 0xff, 0x00};

    //uint8_t *hci_event_data=cybtsoc_platform_malloc(64);
    uint8_t hci_event_data[64] = {0};
    if (hci_event_data == NULL)
    {
        return CYBTSOC_ERR_INIT_MEMPOOL_FAILED;
    }

    event_header[2] = 4 + para_len;
    event_header[3] = HCI_PACKET_TYPE_COMMAND;
    event_header[4] = opcode & 0xff;
    event_header[5] = (opcode >> 8) & 0xff;

    result = uart_read(hci_event_data, &read_len, timeout_ms);
    //result = cybtsoc_platform_hci_uart_read(hci_event_data, &read_len, timeout_ms);
    /* only debug printf
    printf("Hci Rsp of 0x%04X cmd, %d byte, err 0x%02X: \r\n",opcode,read_len,result);
    for(uint32_t i=0;i<read_len;i++)
    {
       printf("%0.2x ",hci_event_data[i]);
    }
    printf("\r\n");
    */

    if (result != CYBTSOC_SUCCESS)
    {
        goto read_return_fail;
    }

    if (memcmp(hci_event_data, event_header, 7) != 0)
    {
        result =  CYBTSOC_ERR_BT_CMD_RSP_MISMATCH;

        goto read_return_fail;
    }

    if ((p_para_data != NULL) && (para_len != 0))
    {
        memcpy(p_para_data, &hci_event_data[7], para_len);
    }

read_return_fail :

    //cybtsoc_platform_free(hci_event_data);

    return result;
}

static cybtsoc_result_t cybtsoc_bt_host_update_baudrate(uint32_t newBaudRate)
{
    cybtsoc_result_t result = CYBTSOC_SUCCESS;

    uint8_t update_baudrate_cmd[ 10 ];

    /* format of transmit bytes for update baud rate command
     * 0x01, 0x18, 0xFC, 0x06, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff
     */

    update_baudrate_cmd[ 0 ] = 0x00; /* Encoded baud rate ; disable : 0 , enable : 1 */
    update_baudrate_cmd[ 1 ] = 0x00; /* use Encoded form  ; disable : 0 , enable : 1 */
    /* issue BT hci update baudrate */
    update_baudrate_cmd[ 2 ] = (newBaudRate) & 0xff;
    update_baudrate_cmd[ 3 ] = (newBaudRate >> 8) & 0xff;
    update_baudrate_cmd[ 4 ] = (newBaudRate >> 16) & 0xff;
    update_baudrate_cmd[ 5 ] = (newBaudRate >> 24) & 0xff;

    bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, HCI_CMD_OPCODE_UPDATE_BAUDRATE, update_baudrate_cmd, 6);
    //cy_rtos_delay_milliseconds( 5 );
    if (bt_hci_vendor_spec_rsp_check(HCI_CMD_OPCODE_UPDATE_BAUDRATE, NULL, 0, 1000) != CYBTSOC_SUCCESS)
    {
        printf("hci update baudrate failed\n");
        return CYBTSOC_ERR_GENERIC;
    }
    cy_rtos_delay_milliseconds(20);

    /* Update host uart baudrate*/
    result = rk_uart_set_baudrate(newBaudRate);
    if (result != CYBTSOC_SUCCESS)
    {
        printf("bt_host_update_baudrate Fail!%d\n", result);
    }
    return result;
}

static cybtsoc_result_t cybtsoc_bt_host_verify_flash_crc(uint32_t addr, uint32_t size, uint32_t *returnCRC)
{
    cybtsoc_result_t result = CYBTSOC_SUCCESS;

    /*CRC validation command is an example:
        01 CC FC 08 xx xx xx xx yy yy yy yy
        the xx xx xx xx bytes specify the 32-bit value of the mapped address for the serial flash
        offset and the yy yy yy yy bytes specify the 32-bit value of the number of bytes to be read
    */

    uint8_t wdata[ 8 ];
    uint8_t rdata[ 4 ] = {0};

    wdata[ 0 ] = (addr) & 0xff;
    wdata[ 1 ] = (addr >> 8) & 0xff;
    wdata[ 2 ] = (addr >> 16) & 0xff;
    wdata[ 3 ] = (addr >> 24) & 0xff;
    wdata[ 4 ] = (size) & 0xff;
    wdata[ 5 ] = (size >> 8) & 0xff;
    wdata[ 6 ] = (size >> 16) & 0xff;
    wdata[ 7 ] = (size >> 24) & 0xff;

    result = bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, HCI_CMD_OPCODE_CRC_CHECK, wdata, 8);
    //cy_rtos_delay_milliseconds( 5 );
    if (bt_hci_vendor_spec_rsp_check(HCI_CMD_OPCODE_CRC_CHECK, rdata, 4, 1000) != CYBTSOC_SUCCESS)
    {
        printf("hci verify CRC failed\n");
        return CYBTSOC_ERR_GENERIC;
    }

    *returnCRC = *(uint32_t *)rdata;

    return result;
}

cybtsoc_result_t cybtsoc_bt_issue_reset(void)
{

    // uint8_t hci_reset_cmd[] = { 0x1, 0x03, 0x0c, 0x00}; /* reset code as per spec */

    cybtsoc_result_t result = bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, HCI_CMD_OPCODE_RESET, NULL, 0);
    //cy_rtos_delay_milliseconds( 5 );
    if (bt_hci_vendor_spec_rsp_check(HCI_CMD_OPCODE_RESET, NULL, 0, 1000) != CYBTSOC_SUCCESS)
    {
        printf("hci reset failed\n");
        return CYBTSOC_ERR_GENERIC;
    }

    return result;
}

cybtsoc_result_t cybtsoc_bt_firmware_dowload(const uint8_t *ram_image, uint32_t ram_size, const char *ram_version,
        const uint8_t *sflash_image, uint32_t sflash_size, const char *sflash_version)
{
    cybtsoc_result_t result = CYBTSOC_SUCCESS;
    //cybtsoc_platform_config_t * p_platform_cfg = cybtsoc_platform_get_config();
    uint32_t remain_length;
    uint8_t *data;

    /* pre-config Recovery  */

    result = cybtsoc_bt_issue_reset() ;
    if (result != CYBTSOC_SUCCESS)
    {
        return result;
    }

    if (ram_version != NULL)
    {
        printf("minidriver ver: %s \r\n", ram_version);
    }

    if (sflash_version != NULL)
    {
        printf("BT SoC fw ver: %s \r\n", sflash_version);
    }

    /* download minidrive */
    bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, HCI_CMD_OPCODE_DOWNLOAD_MINIDRIVER, NULL, 0);
    //cy_rtos_delay_milliseconds( 1 );
    if (bt_hci_vendor_spec_rsp_check(HCI_CMD_OPCODE_DOWNLOAD_MINIDRIVER, NULL, 0, 300) != CYBTSOC_SUCCESS)
    {
        printf("hci download minidrive failed\n");
        return CYBTSOC_ERR_GENERIC;
    }

    if (ram_image != NULL)
    {

        remain_length = ram_size;
        data = (uint8_t *)ram_image;
        while (remain_length)
        {
            uint32_t data_length = data[ 2 ] + 3; /* content of data length + 2 bytes of opcode and 1 byte of data length */
            hci_command_opcode_t command_opcode = *(hci_command_opcode_t *) data;
            //uint32_t bytes_read;
            uint32_t addr = *(uint32_t *) &data[3];


            bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, command_opcode, &data[3], data[2]);
            //cy_rtos_delay_milliseconds( 1 );
            if (bt_hci_vendor_spec_rsp_check(command_opcode, NULL, 0, 300) != CYBTSOC_SUCCESS)
            {
                printf("hci write/launch ram cmd failed\n");
                return CYBTSOC_ERR_GENERIC;
            }

            switch (command_opcode)
            {
            case HCI_CMD_OPCODE_WRITE_RAM:
                /* Update remaining length and data pointer */
                printf("Writ RAM to 0x%08lX: %d byte \r\n", addr, data[ 2 ] - 4);
                data += data_length;
                remain_length -= data_length;

                break;

            case HCI_CMD_OPCODE_LAUNCH_RAM:
                /* All responses have been read. Now let's flush residual data if any and reset remaining length */
                printf("Launch RAM at 0x%08lX \r\n", addr);

                //bytes_read = 1;
                remain_length = 0;

                break;

            default:
                return CYBTSOC_ERR_BT_FMDL_UNKNOWN_PACKET;
            }

        }

        printf("minidriver img size: %ld byte \r\n", ram_size);
        printf("download minidriver done!\r\n");

    }

    if (sflash_image != NULL)
    {
        /* update baudrate to "baudrate for firmware downloading" */
        uint32_t baudrate = BAUD_RATE_3000000;//p_platform_cfg->hci_config.hci_uart.baud_rate_for_fw_download;


        result = cybtsoc_bt_host_update_baudrate(baudrate);
        if (result != CYBTSOC_SUCCESS)
        {
            return result;
        }

        /* chip erase */
        /* A special value of EF EE BE FC (0xFCBEEEEF) is used to signal “use the lowest valid non-
            volatile memory range”. Otherwise, the device to be erased is determined from the value when compared to valid
            ranges, where 0x500000 would be the start of on-chip flash when supported and 0xFF000000 would be the start of offchip
            flash.
         */
        uint32_t erase_addr = 0xFCBEEEEF;
        bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, HCI_CMD_OPCODE_CHIP_ERASE, (uint8_t *)&erase_addr, 4);
        //cy_rtos_delay_milliseconds( 1 );
        if (bt_hci_vendor_spec_rsp_check(HCI_CMD_OPCODE_CHIP_ERASE, NULL, 0, 1000) != CYBTSOC_SUCCESS)
        {
            printf("hci chip erase failed\n");
            return CYBTSOC_ERR_GENERIC;
        }

        printf("Downloading application...img size: %ld byte\r\n", sflash_size);

        remain_length = sflash_size;
        data = (uint8_t *)sflash_image;
        uint32_t block_start_addr = *(uint32_t *) &data[3];
        uint32_t block_write_lenght = 0;
        uint32_t fw_crc = CRC32_INIT_VALUE;

        while (remain_length)
        {
            uint32_t data_length = data[ 2 ] + 3; /* content of data length + 2 bytes of opcode and 1 byte of data length */
            hci_command_opcode_t command_opcode = *(hci_command_opcode_t *) data;
            //uint32_t bytes_read;
            uint32_t addr = *(uint32_t *) &data[3];
            uint32_t rawdata_len = data[ 2 ] - 4;


            bt_hci_vendor_spec_write(HCI_PACKET_TYPE_COMMAND, command_opcode, &data[3], data[2]);
            //cy_rtos_delay_milliseconds( 1 );
            if (bt_hci_vendor_spec_rsp_check(command_opcode, NULL, 0, 300) != CYBTSOC_SUCCESS)
            {
                printf("hci write/launch ram cmd failed\n");
                return CYBTSOC_ERR_GENERIC;
            }


            switch (command_opcode)
            {
            case HCI_CMD_OPCODE_WRITE_RAM:
            {

                fw_crc = crc32(&data[7], rawdata_len, fw_crc);

                /* Update remaining length and data pointer */
                printf("Writ RAM to 0x%08lX: %ld byte \r\n", addr, rawdata_len);
                block_write_lenght +=  rawdata_len;
                data += data_length;
                remain_length -= data_length;

                hci_command_opcode_t next_opcode = *(hci_command_opcode_t *) data;
                uint32_t next_addr = *(uint32_t *) &data[3] ;
                /*check next cmd block*/
                if (next_opcode == HCI_CMD_OPCODE_LAUNCH_RAM || /*next cmd is launch ram in image*/
                        (next_opcode == HCI_CMD_OPCODE_WRITE_RAM && (next_addr > (addr + rawdata_len)))     /* next write ram skip new block address*/
                   )
                {
                    /* Verify CRC */
                    uint32_t reCrc = 0;
                    uint32_t expected_crc = ~fw_crc;

                    result = cybtsoc_bt_host_verify_flash_crc(block_start_addr, block_write_lenght, &reCrc);
                    if (result != CYBTSOC_SUCCESS)
                    {
                        return CYBTSOC_ERR_BT_FMDL_CRC_FAIL;
                    }
                    else
                    {
                        printf("VerifyCRC Checking %ld bytes starting at 0x%08lX\r\n", block_write_lenght, block_start_addr);

                        if (reCrc != expected_crc)
                        {
                            printf("Bad CRC value\r\n");
                            return CYBTSOC_ERR_BT_FMDL_CRC_FAIL;
                        }
                        printf("Good CRC value 0x%08lX\r\n", reCrc);

                    }

                    block_start_addr = next_addr;
                    block_write_lenght = 0;
                    fw_crc = CRC32_INIT_VALUE; //calc for another start addr.

                    cy_rtos_delay_milliseconds(2);

                }

            }
            break;

            case HCI_CMD_OPCODE_LAUNCH_RAM:
            {
                /* All responses have been read. Now let's flush residual data if any and reset remaining length */
                printf("Launch RAM at 0x%08lX \r\n", addr);
                //bytes_read = 1;
                remain_length = 0;
            }
            break;

            default:
                return CYBTSOC_ERR_BT_FMDL_UNKNOWN_PACKET;
            }

        }
    }
    else
    {
        result = CYBTSOC_ERR_BADARG;
    }


    return result;
}

cybtsoc_result_t cybtsoc_bt_start_firmware_dowload(uint32_t rate)
{
    cybtsoc_result_t result = CYBTSOC_SUCCESS;
    //cybtsoc_platform_main_cb_t * p_main_cb = cybtsoc_platform_get_main_cb();

    // uint32_t intstat = cybtsoc_platform_critical_section_enter();
    // p_main_cb->hci_status = CYBTSOC_HCI_FW_DOWNLOAD;
    // cybtsoc_platform_critical_section_exit(intstat);

    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    cybtsoc_hci_task_irq_sem_mask();

    if (rate == BAUD_RATE_115200)
        rk_uart_set_baudrate(rate);
    rk_uart_register_rx_ready_cb(uart_rx_ind);

    result = cybtsoc_bt_firmware_dowload(cyw_btsoc_minidriver_buf, cyw_btsoc_minidriver_config_length, cyw_btsoc_minidriver_version,
                                         cyw_btsoc_fw_config_buf, cyw_btsoc_fw_config_length, cyw_btsoc_version);


    // intstat = cybtsoc_platform_critical_section_enter();

    // p_main_cb->hci_status  = CYBTSOC_HCI_WAIT_EVT;
    // cybtsoc_platform_critical_section_exit(intstat);
    cybtsoc_hci_task_irq_sem_unmask();

    //rk_uart_close();

    return result;
}
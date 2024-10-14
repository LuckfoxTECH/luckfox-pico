/*
 * Copyright 2016-2020, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software"), is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

#include <stdio.h>
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "bt_soc_platform_hci.h"
#include "bt_soc_platform_task.h"
#include "bt_soc_uart.h"
#include "hci_control_api.h"
#include "../sensor_hub.h"

#define CY_RTOS_NEVER_TIMEOUT 200
#define WICED_HCI_HEADER 5

static uint32_t cyw_msg_wait_cnt = 0;

void cyw_wait_cnt_increase(void)
{
    cyw_msg_wait_cnt++;
}

void cyw_wait_cnt_decrease(void)
{
    if (cyw_msg_wait_cnt > 0)
        cyw_msg_wait_cnt--;
}

uint32_t cyw_wait_cnt_get(void)
{
    return cyw_msg_wait_cnt;
}

// Send WICED HCI command
rt_bool_t wiced_hci_send_command(uint16_t command, char *payload, uint32_t len)
{
    char    *data;
    uint32_t totalLength = 0;
    uint16_t    header  = 0;

    data = rt_malloc(WICED_HCI_HEADER + len);
    rt_memset(data, 0, sizeof(data));

    rt_mq_control(cybtsoc_bt_evt_task_queue, RT_IPC_CMD_RESET, RT_NULL);
    // If command header byte is not set, set it now
    if (command)
    {
        data[header++] = HCI_WICED_PKT;
        data[header++] = command & 0xff;
        data[header++] = (command >> 8) & 0xff;
        data[header++] = len & 0xff;
        data[header++] = (len >> 8) & 0xff;
    }

    if (len)
        rt_memcpy(&data[header], payload, len);

    totalLength = header + len;

    if (totalLength == 0)
        return false;

    rk_uart_write(data,  totalLength);

    rt_free(data);

    return true;
}

void cyw_wakeup_ctrl(int en)
{
    rt_pin_mode(RT_HW_SENSOR_HUB_PWR_PIN, PIN_MODE_OUTPUT);
    if (en)
        rt_pin_write(RT_HW_SENSOR_HUB_PWR_PIN, PIN_HIGH);
    else
        rt_pin_write(RT_HW_SENSOR_HUB_PWR_PIN, PIN_LOW);
}

cybtsoc_result_t cybtsoc_send_msg_to_bt_evt_task(void *p_bt_msg)
{
    cybtsoc_result_t api_result;

    if (p_bt_msg == NULL)
    {
        return CYBTSOC_ERR_BADARG;
    }

    if (cyw_wait_cnt_get() == 0)
    {
        /* Nobody is waiting */
        return CYBTSOC_ERR_SEND_QUEUE_FAILED;
    }

    api_result = rt_mq_send(cybtsoc_bt_evt_task_queue, (void *)&p_bt_msg, sizeof(WICED_PKT_MSG_HDR *));

    if (api_result != CYBTSOC_SUCCESS)
    {
        return CYBTSOC_ERR_SEND_QUEUE_FAILED;
    }

    return CYBTSOC_SUCCESS;
}

cybtsoc_result_t cyw_wait_replay(uint16_t cmd, void *buffer)
{
    cybtsoc_result_t api_result;
    WICED_PKT_MSG_HDR *p_bt_msg;
    uint16_t opcode;
    uint16_t length;
    uint8_t *p;

    api_result = rt_mq_recv(cybtsoc_bt_evt_task_queue,
                            (void *)&p_bt_msg,
                            sizeof(WICED_PKT_MSG_HDR *),
                            CY_RTOS_NEVER_TIMEOUT);

    if (CYBTSOC_SUCCESS != api_result || NULL == p_bt_msg)
    {
        printf("bt_evt_task(): queue error (0x%x), msg = 0x%p",
               api_result,
               p_bt_msg);
        return api_result;
    }

    /* msg queue handle */
    opcode = p_bt_msg->event;
    length = p_bt_msg->length;
    p = (uint8_t *)(p_bt_msg + 1);

    if (opcode == cmd)
    {
        rt_memcpy(buffer, p, length);
        while (length--)
            printf("%02X ", *p++);
    }

    printf("\n");
    cybtsoc_platform_task_mempool_free((void *) p_bt_msg);

    return 0;
}

static rt_err_t sensor_hub_control(rt_device_t dev, int cmd, void *args)
{
    if (args == NULL)
    {
        rt_kprintf("%s no args\n", __func__, args);
        return -1;
    }

    cyw_wakeup_ctrl(1);
    switch (cmd)
    {
    case HCI_CONTROL_MISC_COMMAND_GET_VERSION:
    case HCI_CONTROL_MISC_COMMAND_GET_RTC_TIME:
    case HCI_CONTROL_MISC_COMMAND_GET_SYS_TICK:
    case HCI_CONTROL_MISC_COMMAND_GET_EVENT:
        cyw_wait_cnt_increase();
        wiced_hci_send_command(cmd, 0, 0);
        cyw_wait_replay(cmd, args);
        cyw_wait_cnt_decrease();
        break;

    case HCI_CONTROL_MISC_COMMAND_SET_RTC_TIME:
        cyw_wait_cnt_increase();
        wiced_hci_send_command(cmd, args, 12);
        cyw_wait_replay(cmd, args);
        cyw_wait_cnt_decrease();
        break;

    case HCI_CONTROL_MISC_COMMAND_SET_TIMER:
        cyw_wait_cnt_increase();
        wiced_hci_send_command(cmd, args, 4);
        cyw_wait_replay(cmd, args);
        cyw_wait_cnt_decrease();
        break;

    default:
        rt_kprintf("Unsupport sensor control cmd: 0x%lx\n", cmd);
        break;
    }

    cyw_wakeup_ctrl(0);
    return 0;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops sensor_hub_ops =
{
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    sensor_hub_control,
};
#endif

int rt_sensor_hub_init(void)
{
    struct rt_device *rt_bl;

    rt_bl = rt_malloc(sizeof(struct rt_device));
    if (!rt_bl)
    {
        rt_kprintf("alloc backlight rt_device failed\n");
        return -RT_ENOMEM;
    }

    /* init rt_device structure */
    rt_bl->type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    rt_bl->ops = &sensor_hub_ops;
#else
    rt_bl->init = NULL;
    rt_bl->open = NULL;
    rt_bl->close = NULL;
    rt_bl->read = NULL;
    rt_bl->write = NULL;
    rt_bl->control = sensor_hub_control;
#endif

    cyw_wakeup_ctrl(0);
    cybtsoc_platform_task_init("uart2");

    /* register rt_bl device to RT-Thread */
    rt_device_register(rt_bl, "sensor_hub", RT_DEVICE_FLAG_RDWR);

    return 0;
}

INIT_DEVICE_EXPORT(rt_sensor_hub_init);

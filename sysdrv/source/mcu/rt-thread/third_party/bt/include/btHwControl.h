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

#ifndef _BT_HW_CONTROL_H_

#define _BT_HW_CONTROL_H_
#include "bt_hw.h"
//#include "hci/hci_hal.h"

typedef enum
{
    POWER_STATUS_ON = 1,
    POWER_STATUS_SLEEP,
    POWER_STATUS_OFF
} POWER_STATUS_t;


typedef enum
{
    POWER_WILL_SLEEP = 1,
    POWER_WILL_WAKE_UP_CFM,
    POWER_WILL_WAKE_UP_IND,
} POWER_NOTIFICATION_t;


typedef enum
{
    UART_ABILITY_RECV_ENABLE = 1,
    UART_ABILITY_RECV_DISABLE
} UART_ABILITY_t;

typedef struct
{
    int (*init)(void);
    int (*on)(void *config);              // <-- turn BT module on and configure
    int (*off)(void);             // <-- turn BT module off
    int (*sleep_enable)(void *config);           // <-- put BT module to sleep    - only to be called after ON
    int (*sleep_disable)(void *config);           // <-- put BT module to sleep    - only to be called after ON
    int (*wake)(void *config);            // <-- wake BT module from sleep - only to be called after SLEEP

    /** support for UART baud rate changes - cmd has to be stored in hci_cmd_buffer
     * @return have command
     */
    int (*set_baudrate)(uint32 baudrate);

    /** support custom init sequences after RESET command - cmd has to be stored in hci_cmd_buffer
      * @return have command
      */

    int (*get_power_status)(void);
    int (*vendor_uart_recv)(uint8 *data , uint32 len);          //use for vendor data recv
    void (*power_event_notifications)(int(*func)(POWER_NOTIFICATION_t event));
    int (*set_uart_recv_ablity)(UART_ABILITY_t Ability);
    int (*set_mac_addr)(struct bd_addr *bdaddr);
    void (*hw_error)(void);
    bool (*host_check_send_available)(void);
    int (*write)(char *data, uint32 len);        /*TX  MUST*/
    int (*read)(char *data, uint32 len);        /*RX  MUST*/
} bt_hw_control_t;
// Gets the correct hw control implementation, as compiled for.
const bt_hw_control_t *bt_hw_control_get_interface(void);

typedef struct _power_on_config
{
    uint8 enable_pin_code;
    uint8 pin_code[8];
    uint8 enable_dev_mac;
    struct bd_addr bdaddr;
    uint8 enable_dev_name;
    uint8 dev_name[32];
    uint32 bt_init_flag;

} POWER_ON_CONFIG;


#endif

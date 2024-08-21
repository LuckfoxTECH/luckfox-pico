/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * BLE Vendor Specific Device
 *
 * Features demonstrated
 *  - GATT database and Device configuration initialization
 *  - Registration with LE stack for various events
 *  - Sending data to the client
 *  - Processing write requests from the client
 *
 * To demonstrate the app, work through the following steps.
 * 1. Plug the WICED eval board into your computer
 * 2. Build and download the application (to the WICED board)
 * 3. On application start the device acts as a GATT server and advertises itself as Hello Sensor
 * 4. Connect to GATT server using one of the LE clients (LEExplorer(android)) or (BLE Utility(Apple Store))
 * 5. Once connected the client can read Hello sensor characteristics
 * 6. Write the hello sensor characteristic configuration value from client
 * 7. on the console terminal you can see the value entered in the client
 * 8. To recieve notification the user has to register for notification after LE connection on the client
 * 9. To test notifications user can enter the command "notify" on the console
 * 10. Each time when user enters the notify command a new value of notification can be seen on the LE client.
 *
 * ENABLE_APP_POWERSAVE is defined to enable power-save related changes in the applications
 * For accurate current measurement, turn off LEDs on the board.
 *
 */

#include <string.h>
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "ble_hello_sensor.h"
#include "wiced.h"
#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "command_console.h"
#include "wwd_debug.h"
#include "wiced_bt_dev.h"
#include "wiced_low_power.h"
#include "hal_base.h"

/******************************************************************************
 *                                Constants
******************************************************************************/

#define HELLO_SENSOR_CONSOLE_COMMAND_HISTORY_LENGTH  (10)
#define MAX_HELLO_SENSOR_COMMAND_LENGTH              (85)
#define MAX_ASCII_VALUE                              0x7e
#define ROLL_OVER_ASCII_VALUE                        0x2f
#define HELLO_SENSOR_CONSOLE_COMMANDS \
    { (char*) "notify",    hello_sensor_send_notification,    0, NULL, NULL, (char *)"", (char *)"Send Notification" }, \
    { (char*) "indicate",  hello_sensor_send_indication,      0, NULL, NULL, (char *)"", (char *)"Send Indication"   }, \
    { (char*) "set_adv_tx_power",  hello_sensor_set_adv_tx_power,      0, NULL, NULL, (char *)"", (char *)"Set ADV TX power power_val"   }, \
    { (char*) "read_adv_tx_power",  hello_sensor_read_adv_tx_power,      0, NULL, NULL, (char *)"", (char *)"Read ADV TX power"   }, \
    { (char*) "read_rssi",          hello_sensor_read_rssi,      0, NULL, NULL, (char *)"", (char *)"Read RSSI value"   }, \

/******************************************************************************
 *                           Function Prototypes
 ******************************************************************************/
static void                     hello_sensor_application_init(void);
static wiced_bt_gatt_status_t   hello_sensor_gatts_connection_status_handler(wiced_bt_gatt_connection_status_t *p_status);
static wiced_bt_gatt_status_t   hello_sensor_gatts_connection_up(wiced_bt_gatt_connection_status_t *p_status);
static wiced_bt_gatt_status_t   hello_sensor_gatts_connection_down(wiced_bt_gatt_connection_status_t *p_status);
wiced_result_t           hello_sensor_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
static wiced_bt_gatt_status_t   hello_sensor_gatts_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data);
static wiced_bt_gatt_status_t   hello_sensor_gatt_server_read_request_handler(uint16_t conn_id, wiced_bt_gatt_read_t *p_read_data);
static wiced_bt_gatt_status_t   hello_sensor_gatt_server_write_request_handler(uint16_t conn_id, wiced_bt_gatt_write_t *p_data);
static void                     hello_sensor_set_advertisement_data(void);
static void                     hello_sensor_send_message(void);
static int                      hello_sensor_send_notification(int argc, char *argv[]);
static int                      hello_sensor_send_indication(int argc, char *argv[]);
static wiced_bt_gatt_status_t   hello_sensor_gatt_server_write_and_execute_request_handler(uint16_t conn_id, wiced_bt_gatt_exec_flag_t exec_flag);
static int                      hello_sensor_set_adv_tx_power(int argc, char *argv[]);
static int                      hello_sensor_read_adv_tx_power(int argc, char *argv[]);
static int                      hello_sensor_read_rssi(int argc, char *argv[]);
static void                     hello_sensor_tx_power_callback(wiced_bt_tx_power_result_t *p_tx_power);
static void                     hello_sensor_read_rssi_callback(wiced_bt_dev_rssi_result_t *p_rssi);
extern void                   BTM_BleWriteDefaultDataLength(void);
extern wiced_bt_cfg_settings_t wiced_bt_audio_cfg_settings;
extern BOOLEAN wiced_bt_l2cap_update_ble_conn_params(BD_ADDR rem_bda, UINT16 min_int, UINT16 max_int, UINT16 latency, UINT16 timeout);

/******************************************************************************
 *                                Structures
 ******************************************************************************/

typedef struct
{
    BD_ADDR         remote_addr;                            /* remote peer device address */
    uint32_t        timer_count;                            /* timer count */
    uint32_t        fine_timer_count;                       /* fine timer count */
    uint16_t        conn_id;                                /* connection ID referenced by the stack */
    uint16_t        peer_mtu;                               /* peer MTU */
    uint8_t         flag_indication_sent;                   /* indicates waiting for confirmation */
    uint8_t         flag_stay_connected;                    /* stay connected or disconnect after all messages are sent */
    uint8_t         battery_level;                          /* dummy battery level */
} hello_sensor_state_t;

typedef PACKED struct
{
    BD_ADDR         bdaddr;                                 /* BD address of the bonded host */
    uint16_t        characteristic_client_configuration;    /* Current value of the client configuration descriptor */
    uint8_t         number_of_blinks;                       /* Sensor config, number of times to blink the LEd when button is pushed. */
} host_info_t;

typedef struct
{
    uint16_t        handle;
    uint16_t        attr_len;
    void           *p_attr;
} attribute_t;

/*
const command_t hello_sensor_console_command_table[] =
{
    HELLO_SENSOR_CONSOLE_COMMANDS
    CMD_TABLE_END
};*/

/******************************************************************************
 *                                Variables Definitions
 ******************************************************************************/

static uint8_t hello_sensor_device_name[]          = "Hello";
static uint8_t hello_sensor_appearance_name[2]     = { BIT16_TO_8(APPEARANCE_GENERIC_TAG) };
static char    hello_sensor_char_notify_value[244]    = { 'H', 'E', 'L', 'L', 'O', '0' };
static char    hello_sensor_char_mfr_name_value[]  = { 'C', 'Y', 'P', 'R', 'E', 'S', 'S', 0, };
static char    hello_sensor_char_model_num_value[] = { '1', '2', '3', '4',   0,   0,   0,   0 };
static uint8_t hello_sensor_char_system_id_value[] = { 0xbb, 0xb8, 0xa1, 0x80, 0x5f, 0x9f, 0x91, 0x71 };
static char    hello_sensor_char_indicate_value[]  = { 'I', 'N', 'D', 'I', 'C', 'A', 'T', 'E', '0' };

static hello_sensor_state_t     hello_sensor_state;
static host_info_t              hello_sensor_hostinfo;
static wiced_bool_t             is_connected = WICED_FALSE;
static uint8_t                  hello_sensor_write;
static char                     hello_sensor_command_buffer[MAX_HELLO_SENSOR_COMMAND_LENGTH];
static char                     hello_sensor_command_history_buffer[MAX_HELLO_SENSOR_COMMAND_LENGTH * HELLO_SENSOR_CONSOLE_COMMAND_HISTORY_LENGTH];

attribute_t gatt_user_attributes[] =
{
    { HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL,       sizeof(hello_sensor_device_name),         hello_sensor_device_name },
    { HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL, sizeof(hello_sensor_appearance_name),       hello_sensor_appearance_name },
    { HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL,             sizeof(hello_sensor_char_notify_value),     hello_sensor_char_notify_value },
    { HANDLE_HSENS_SERVICE_CHAR_CFG_DESC,               2, (void *) &hello_sensor_hostinfo.characteristic_client_configuration },
    { HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL,              1,                                          &hello_sensor_hostinfo.number_of_blinks },
    { HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL,  sizeof(hello_sensor_char_mfr_name_value),   hello_sensor_char_mfr_name_value },
    { HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL, sizeof(hello_sensor_char_model_num_value),  hello_sensor_char_model_num_value },
    { HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL, sizeof(hello_sensor_char_system_id_value),  hello_sensor_char_system_id_value },
    { HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL_VAL,      1,                                          &hello_sensor_state.battery_level },
};

/******************************************************************************
 *                                GATT DATABASE
 ******************************************************************************/
/*
 * This is the GATT database for the Hello Sensor application.  It defines
 * services, characteristics and descriptors supported by the sensor.  Each
 * attribute in the database has a handle, (characteristic has two, one for
 * characteristic itself, another for the value).  The handles are used by
 * the peer to access attributes, and can be used locally by application for
 * example to retrieve data written by the peer.  Definition of characteristics
 * and descriptors has GATT Properties (read, write, notify...) but also has
 * permissions which identify if and how peer is allowed to read or write
 * into it.  All handles do not need to be sequential, but need to be in
 * ascending order.
 */
const uint8_t hello_sensor_gatt_database[] =
{
    /* Declare mandatory GATT service */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_GATT_SERVICE, UUID_SERVCLASS_GATT_SERVER),

    /* Declare mandatory GAP service. Device Name and Appearance are mandatory
     * characteristics of GAP service                                        */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_GAP_SERVICE, UUID_SERVCLASS_GAP_SERVER),

    /* Declare mandatory GAP service characteristic: Dev Name */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME, HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL,
                          GATT_UUID_GAP_DEVICE_NAME, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),

    /* Declare mandatory GAP service characteristic: Appearance */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE, HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,
                          GATT_UUID_GAP_ICON, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),

    /* Declare proprietary Hello Service with 128 byte UUID */
    PRIMARY_SERVICE_UUID128(HANDLE_HSENS_SERVICE, UUID_HELLO_SERVICE),

    /* Declare characteristic used to notify/indicate change */
    CHARACTERISTIC_UUID128(HANDLE_HSENS_SERVICE_CHAR_NOTIFY, HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL,
                           UUID_HELLO_CHARACTERISTIC_NOTIFY, LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_NOTIFY | LEGATTDB_CHAR_PROP_INDICATE, LEGATTDB_PERM_READABLE),

    /* Declare client characteristic configuration descriptor
     * Value of the descriptor can be modified by the client
     * Value modified shall be retained during connection and across connection
     * for bonded devices.  Setting value to 1 tells this application to send notification
     * when value of the characteristic changes.  Value 2 is to allow indications. */
    CHAR_DESCRIPTOR_UUID16_WRITABLE(HANDLE_HSENS_SERVICE_CHAR_CFG_DESC, GATT_UUID_CHAR_CLIENT_CONFIG,
                                    LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_REQ),

    /* Declare characteristic Hello Configuration */
    CHARACTERISTIC_UUID128_WRITABLE(HANDLE_HSENS_SERVICE_CHAR_BLINK, HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL,
                                    UUID_HELLO_CHARACTERISTIC_CONFIG, LEGATTDB_CHAR_PROP_READ | LEGATTDB_CHAR_PROP_WRITE,
                                    LEGATTDB_PERM_READABLE | LEGATTDB_PERM_WRITE_CMD | LEGATTDB_PERM_WRITE_REQ),

    /* Declare Device info service */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE, UUID_SERVCLASS_DEVICE_INFO),

    /* Handle 0x4e: characteristic Manufacturer Name, handle 0x4f characteristic value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME, HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL,
                          GATT_UUID_MANU_NAME, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),

    /* Handle 0x50: characteristic Model Number, handle 0x51 characteristic value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM, HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL,
                          GATT_UUID_MODEL_NUMBER_STR, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),

    /* Handle 0x52: characteristic System ID, handle 0x53 characteristic value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID, HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL,
                          GATT_UUID_SYSTEM_ID, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),

    /* Declare Battery service */
    PRIMARY_SERVICE_UUID16(HANDLE_HSENS_BATTERY_SERVICE, UUID_SERVCLASS_BATTERY),

    /* Handle 0x62: characteristic Battery Level, handle 0x63 characteristic value */
    CHARACTERISTIC_UUID16(HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL, HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL_VAL,
                          GATT_UUID_BATTERY_LEVEL, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE),
};
/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/

#ifdef ENABLE_APP_POWERSAVE
void app_powersave_sleep(void)
{
    /* For 4390x platforms(Embedded Mode) wiced_bt_dev_allow_host_sleep() is dummy.
     * TODO: Need to implement the logic to find if 4390x can enter deep_sleep */
    if (wiced_bt_dev_allow_host_sleep() == WICED_TRUE)
    {
        platform_mcu_powersave_enable();
        WPRINT_BT_APP_INFO(("App going to sleep...\n"));
    }
}

/* Ideally, this should be called as an application-hook when platform MCU wakes up and will complete
 * the app sleep-wake cycle. For now, do nothing.
 */
void app_powersave_resume(void)
{
    WPRINT_BT_APP_INFO(("App woke up....\n"));
    /* TO-DO */
}
#endif

#if 0
/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
void application_start(void)
{
    wiced_core_init();
    //WPRINT_BT_APP_INFO( ( "Hello Sensor Start\n" ) );
    rt_kprintf("Hello Sensor Start\n");

    /* Register call back and configuration with stack */
    wiced_bt_stack_init(hello_sensor_management_callback,
                        &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}
#endif

/* To take the notify command from the command line
 * To view the notify messages, notify option should be
 * checked in the LE Explorer and indicate option should
 * be unchecked.
 */
static int hello_sensor_send_notification(int argc, char *argv[])
{
    if (is_connected)
    {
        WPRINT_BT_APP_INFO(("Received command: %s\n", argv[0]));
        /* Rolling Over the ASCII value back to '0' */
        if (hello_sensor_char_notify_value[5] >= MAX_ASCII_VALUE)
        {
            hello_sensor_char_notify_value[5] = ROLL_OVER_ASCII_VALUE;
        }
        /*incrementing the value field , to ensure that notification values are changing*/
        hello_sensor_char_notify_value[5]++;
        hello_sensor_send_message();
        return ERR_CMD_OK;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Cannot receive, hello sensor not connected\n"));
        return ERR_CMD_OK;
    }
}

/* To take indication command from the command line.
 * To view the indication messages, Indicate option should
 * be checked in the LE Explorer and notify option should be
 * unchecked.
 */

static int hello_sensor_send_indication(int argc, char *argv[])
{
    if (is_connected)
    {
        WPRINT_BT_APP_INFO(("Received command: %s\n", argv[0]));
        hello_sensor_state.flag_indication_sent = FALSE;
        /* Rolling Over the ASCII value back to '0' */
        if (hello_sensor_char_indicate_value[8] >= MAX_ASCII_VALUE)
        {
            hello_sensor_char_indicate_value[8] = ROLL_OVER_ASCII_VALUE;
        }
        hello_sensor_char_indicate_value[8]++;
        hello_sensor_send_message();
        return ERR_CMD_OK;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Cannot receive, hello sensor not connected\n"));
        return ERR_CMD_OK;
    }
}

/* To take adv_power from the command line and set the adv_tx_power */
static int hello_sensor_set_adv_tx_power(int argc, char *argv[])
{
    wiced_result_t result;
    int power_val;
    WPRINT_BT_APP_INFO(("hello_sensor_set_adv_tx_power\n"));
    if (!is_connected)
    {
        power_val = atoi(argv[1]);
        WPRINT_BT_APP_INFO(("\nReceived input power_val: %d\n", power_val));
        if (power_val >= -28 && power_val <= 4)
        {
            // stop adv
            result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, 0, NULL);
            //set tx power
            result = wiced_bt_ble_set_adv_tx_power(power_val);
            //start adv
            result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
            WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements %d\n", result));
        }
        else
        {
            WPRINT_APP_INFO(("TX power can be set only between range -28db to +4db \n"));
        }
        return ERR_CMD_OK;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Cannot change ADV tx power, hello sensor already connected\n"));
        return ERR_CMD_OK;
    }
}

/* To read the adv_tx_power from the command line */
static int hello_sensor_read_adv_tx_power(int argc, char *argv[])
{
    if (!is_connected)
    {
        //read adv tx power
        wiced_bt_ble_read_adv_tx_power((wiced_bt_dev_cmpl_cback_t *)hello_sensor_tx_power_callback);
        return ERR_CMD_OK;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Error in reading ADV TX power\n"));
        return ERR_CMD_OK;
    }
}

static int hello_sensor_read_rssi(int argc, char *argv[])
{
    if (is_connected)
    {
        wiced_bt_dev_read_rssi(hello_sensor_state.remote_addr, BT_TRANSPORT_LE, (wiced_bt_dev_cmpl_cback_t *)hello_sensor_read_rssi_callback);
        return ERR_CMD_OK;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Error in reading RSSI value\n"));
        return ERR_UNKNOWN;
    }
}

static void hello_sensor_tx_power_callback(wiced_bt_tx_power_result_t *p_tx_power)
{
    if ((p_tx_power->status == WICED_BT_SUCCESS) && (p_tx_power->hci_status == HCI_SUCCESS))
    {
        WPRINT_BT_APP_INFO(("ADV TX power: %i\n", p_tx_power->tx_power));
    }
    else
    {
        WPRINT_BT_APP_INFO(("Unable to read ADV TX power.(btm_status=0x%x, hci_status=0x%x)\n", p_tx_power->status, p_tx_power->hci_status));
    }
}

static void hello_sensor_read_rssi_callback(wiced_bt_dev_rssi_result_t *p_rssi)
{
    if ((p_rssi->status == WICED_BT_SUCCESS) && (p_rssi->hci_status == HCI_SUCCESS))
    {
        WPRINT_BT_APP_INFO(("RSSI value: %d\n", p_rssi->rssi));
    }
    else
    {
        WPRINT_BT_APP_INFO(("Unable to read RSSI value.(btm_status=0x%x, hci_status=0x%x)\n", p_rssi->status, p_rssi->hci_status));
    }
}

/*
 * This function is executed in the BTM_ENABLED_EVT management callback.
 */
static void hello_sensor_application_init(void)
{
    wiced_bt_gatt_status_t gatt_status;
    wiced_result_t         result;

    WPRINT_BT_APP_INFO(("hello_sensor_application_init\n"));

    /* Register with stack to receive GATT callback */
    gatt_status = wiced_bt_gatt_register(hello_sensor_gatts_callback);

    WPRINT_BT_APP_INFO(("\n wiced_bt_gatt_register: %d\n", gatt_status));

    /*  Tell stack to use our GATT database */
    gatt_status =  wiced_bt_gatt_db_init(hello_sensor_gatt_database, sizeof(hello_sensor_gatt_database));

    WPRINT_BT_APP_INFO(("wiced_bt_gatt_db_init %d\n", gatt_status));

    /* Set the advertising parameters and make the device discoverable */
    hello_sensor_set_advertisement_data();

    result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);

    WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements %d\n", result));

    /*
     * Set flag_stay_connected to remain connected after all messages are sent
     * Reset flag to 0, to disconnect
     */
    hello_sensor_state.flag_stay_connected = 1;

    /* app_powersave_sleep() needs to be called here only in case of Embedded mode.
     * In case of host mode this will be called when BTM_LPM_STATE_LOW_POWER event is received from stack */
#ifdef USE_WICED_HCI
    app_powersave_sleep();
#endif
}

/*
 * Setup advertisement data with 16 byte UUID and device name
 */
void hello_sensor_set_advertisement_data(void)
{
    wiced_result_t              result;
    wiced_bt_ble_advert_elem_t  adv_elem[3];
    uint8_t ble_advertisement_flag_value        = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;
    uint8_t num_elem                            = 0;
    uint8_t hello_service_uuid[LEN_UUID_128]    = { UUID_HELLO_SERVICE };

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len          = 1;
    adv_elem[num_elem].p_data       = &ble_advertisement_flag_value;
    num_elem ++;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE;
    adv_elem[num_elem].len          = LEN_UUID_128;
    adv_elem[num_elem].p_data       = hello_service_uuid;
    num_elem ++;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len          = strlen((const char *)wiced_bt_audio_cfg_settings.device_name);
    WPRINT_BT_APP_INFO(("wiced_bt_audio_cfg_settings.device_name:%s\n", wiced_bt_audio_cfg_settings.device_name));
    adv_elem[num_elem].p_data       = (uint8_t *)wiced_bt_audio_cfg_settings.device_name;

    num_elem++;

    result = wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);

    WPRINT_BT_APP_INFO(("wiced_bt_ble_set_advertisement_data %d\n", result));
}

/*
 * This function is invoked when advertisements stop.  If we are configured to stay connected,
 * disconnection was caused by the peer, start low advertisements, so that peer can connect
 * when it wakes up
 */
void hello_sensor_advertisement_stopped(void)
{
    wiced_result_t result;

    if (hello_sensor_state.flag_stay_connected && !hello_sensor_state.conn_id)
    {
        result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL);
        WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements: %d\n", result));
    }
    else
    {
        WPRINT_BT_APP_INFO(("ADV stop\n"));
    }
}

BD_ADDR rs_addr = {0xc0, 0x43, 0x43, 0x80, 0x0, 0x0};
/*
 * hello_sensor bt/ble link management callback
 */
wiced_result_t hello_sensor_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t                   result = WICED_BT_SUCCESS;
    wiced_bt_dev_ble_pairing_info_t *p_info;
    wiced_bt_ble_advert_mode_t      *p_mode;
    //wiced_bool_t                     is_warmboot = WICED_DEEP_SLEEP_IS_WARMBOOT( );
    wiced_bool_t                     is_warmboot = WICED_FALSE;

    WPRINT_BT_APP_INFO(("hello_sensor_management_callback: %d\n", event));

    switch (event)
    {
    /* Bluetooth  stack enabled */
    case BTM_ENABLED_EVT:

        if (is_warmboot == WICED_FALSE)
        {
            wiced_bt_ble_setRandomAddr(rs_addr);
            BTM_BleWriteDefaultDataLength();
            hello_sensor_application_init();
        }
        else
        {
            wiced_bt_gatt_register(hello_sensor_gatts_callback);
        }

#ifdef ENABLE_APP_POWERSAVE
        /* Break early as initializing command console and hence STDIO_UART increases the power numbers */
        break;
#endif
        result = command_console_init(STDIO_UART, sizeof(hello_sensor_command_buffer), hello_sensor_command_buffer,
                                      HELLO_SENSOR_CONSOLE_COMMAND_HISTORY_LENGTH, hello_sensor_command_history_buffer, " ");
        if (result != WICED_SUCCESS)
        {
            WPRINT_BT_APP_INFO(("Error starting the command console\r\n"));
        }
        //console_add_cmd_table( hello_sensor_console_command_table );
        break;

    case BTM_DISABLED_EVT:
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        p_event_data->pairing_io_capabilities_ble_request.local_io_cap  = BTM_IO_CAPABILITIES_NONE;
        p_event_data->pairing_io_capabilities_ble_request.oob_data      = BTM_OOB_NONE;
#if 0
        p_event_data->pairing_io_capabilities_ble_request.auth_req      = BTM_LE_AUTH_REQ_BOND | BTM_LE_AUTH_REQ_MITM;
#else
        p_event_data->pairing_io_capabilities_ble_request.auth_req      = BTM_LE_AUTH_REQ_NO_BOND;
#endif
        p_event_data->pairing_io_capabilities_ble_request.max_key_size  = 0x10;
        p_event_data->pairing_io_capabilities_ble_request.init_keys     = BTM_LE_KEY_PENC | BTM_LE_KEY_PID;
        p_event_data->pairing_io_capabilities_ble_request.resp_keys     = BTM_LE_KEY_PENC | BTM_LE_KEY_PID;
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        p_info = &p_event_data->pairing_complete.pairing_complete_info.ble;
        WPRINT_BT_APP_INFO(("Pairing Complete: %d", p_info->reason));
        break;

    case BTM_SECURITY_REQUEST_EVT:
        wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr, WICED_BT_SUCCESS);
        break;

    case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
        p_mode = &p_event_data->ble_advert_state_changed;
        WPRINT_BT_APP_INFO(("Advertisement State Change: %d\n", *p_mode));
        if (*p_mode == BTM_BLE_ADVERT_OFF)
        {
            hello_sensor_advertisement_stopped();
        }
        break;

    case BTM_LPM_STATE_LOW_POWER:

#ifdef ENABLE_APP_POWERSAVE
        app_powersave_sleep();
#endif
        break;

    default:
        break;
    }

    return result;
}

/*
 * Check if client has registered for notification/indication
 * and send message if appropriate
 */
static void hello_sensor_send_message(void)
{
    WPRINT_BT_APP_INFO(("%s: Client's Characteristic configuration:%d\n", __func__, hello_sensor_hostinfo.characteristic_client_configuration));

    /* If client has not registered for indication or notification, no action */
    if (hello_sensor_hostinfo.characteristic_client_configuration == 0)
    {
        return;
    }
    else if (hello_sensor_hostinfo.characteristic_client_configuration & GATT_CLIENT_CONFIG_NOTIFICATION)
    {
        uint8_t *p_attr = (uint8_t *)hello_sensor_char_notify_value;
        wiced_bt_gatt_send_notification(hello_sensor_state.conn_id, HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL, sizeof(hello_sensor_char_notify_value), p_attr);
    }
    else
    {
        if (!hello_sensor_state.flag_indication_sent)
        {
            WPRINT_BT_APP_INFO((" Indication message\n"));
            uint8_t *p_attr = (uint8_t *)hello_sensor_char_indicate_value;
            hello_sensor_state.flag_indication_sent = TRUE;

            wiced_bt_gatt_send_indication(hello_sensor_state.conn_id, HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL, sizeof(hello_sensor_char_indicate_value), p_attr);
        }
    }
}

/*
 * Find attribute description by handle
 */
static attribute_t *hello_sensor_get_attribute(uint16_t handle)
{
    int i;
    for (i = 0; i <  sizeof(gatt_user_attributes) / sizeof(gatt_user_attributes[0]); i++)
    {
        if (gatt_user_attributes[i].handle == handle)
        {
            return (&gatt_user_attributes[i]);
        }
    }
    WPRINT_BT_APP_INFO(("attribute not found:%x\n", handle));
    return NULL;
}

/*
 * Process Read request or command from peer device
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_read_request_handler(uint16_t conn_id, wiced_bt_gatt_read_t *p_read_data)
{
    attribute_t *puAttribute;
    int          attr_len_to_copy;

    if ((puAttribute = hello_sensor_get_attribute(p_read_data->handle)) == NULL)
    {
        WPRINT_BT_APP_INFO(("read_hndlr attr not found hdl:%x\n", p_read_data->handle));
        return WICED_BT_GATT_INVALID_HANDLE;
    }

    /* Dummy battery value read increment */
    if (p_read_data->handle == HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL_VAL)
    {
        if (hello_sensor_state.battery_level++ > 99)
        {
            hello_sensor_state.battery_level = 0;
        }
    }

    if (p_read_data->handle == HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL)
    {
        puAttribute->p_attr = &hello_sensor_write;
        puAttribute->attr_len = sizeof(hello_sensor_write);
    }
    attr_len_to_copy = puAttribute->attr_len;

    WPRINT_BT_APP_INFO(("read_hndlr conn_id:%d hdl:%x offset:%d len:%d\n", conn_id, p_read_data->handle, p_read_data->offset, attr_len_to_copy
                       ));

    if (p_read_data->offset >= puAttribute->attr_len)
    {
        attr_len_to_copy = 0;
    }

    if (attr_len_to_copy != 0)
    {
        uint8_t *from;
        int      to_copy = attr_len_to_copy - p_read_data->offset;


        if (to_copy > *p_read_data->p_val_len)
        {
            to_copy = *p_read_data->p_val_len;
        }

        from = ((uint8_t *)puAttribute->p_attr) + p_read_data->offset;
        *p_read_data->p_val_len = to_copy;

        memcpy(p_read_data->p_val, from, to_copy);
    }

    return WICED_BT_GATT_SUCCESS;
}

/*
 * Process write request or write command from peer device
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_write_request_handler(uint16_t conn_id, wiced_bt_gatt_write_t *p_data)
{
    wiced_bt_gatt_status_t result    = WICED_BT_GATT_SUCCESS;
    uint8_t                *p_attr   = p_data->p_val;
    uint8_t attribute_value = *(uint8_t *) p_data->p_val;

    WPRINT_BT_APP_INFO(("write_handler: conn_id:%d hdl:0x%x prep:%d offset:%d len:%d\n ", conn_id, p_data->handle, p_data->is_prep, p_data->offset, p_data->val_len
                       ));

    switch (p_data->handle)
    {
    /* By writing into Characteristic Client Configuration descriptor
     * peer can enable or disable notification or indication */
    case HANDLE_HSENS_SERVICE_CHAR_CFG_DESC:
        if (p_data->val_len != 2)
        {
            return WICED_BT_GATT_INVALID_ATTR_LEN;
        }
        hello_sensor_hostinfo.characteristic_client_configuration = p_attr[0] | (p_attr[1] << 8);
        break;

    case HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL:
        if (p_data->val_len != 1)
        {
            return WICED_BT_GATT_INVALID_ATTR_LEN;
        }
        hello_sensor_hostinfo.number_of_blinks = p_attr[0];
        if (hello_sensor_hostinfo.number_of_blinks != 0)
        {
            WPRINT_BT_APP_INFO(("hello_sensor_write_handler:num blinks: %d\n", hello_sensor_hostinfo.number_of_blinks
                               ));
        }
        hello_sensor_write = attribute_value;
        WPRINT_BT_APP_INFO(("The value written is: %i\n", attribute_value));
        break;

    default:
        result = WICED_BT_GATT_INVALID_HANDLE;
        break;
    }
    return result;
}

/*
 * Write Execute Procedure
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_write_and_execute_request_handler(uint16_t conn_id, wiced_bt_gatt_exec_flag_t exec_flag)
{
    WPRINT_BT_APP_INFO(("write exec: flag:%d\n", exec_flag));
    return WICED_BT_GATT_SUCCESS;
}

/*
 * Process MTU request from the peer
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_mtu_request_handler(uint16_t conn_id, uint16_t mtu)
{
    WPRINT_BT_APP_INFO(("req_mtu: %d\n", mtu));
    return WICED_BT_GATT_SUCCESS;
}

static wiced_bt_gatt_status_t hello_sensor_gatt_server_confirmation_handler(uint16_t conn_id, uint16_t handle)
{
    WPRINT_BT_APP_INFO(("hello_sensor_indication_confirmation, conn %d hdl %d\n", conn_id, handle));

    if (!hello_sensor_state.flag_indication_sent)
    {
        WPRINT_BT_APP_INFO(("Hello: Wrong Confirmation!"));
        return WICED_BT_GATT_SUCCESS;
    }
    hello_sensor_state.flag_indication_sent = 0;
    hello_sensor_send_message();

    return WICED_BT_GATT_SUCCESS;
}

/* This function is invoked when connection is established */
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_up(wiced_bt_gatt_connection_status_t *p_status)
{
    wiced_result_t result;

    WPRINT_BT_APP_INFO(("hello_sensor_conn_up  id:%d\n:", p_status->conn_id));

    /* Update the connection handler.  Save address of the connected device. */
    hello_sensor_state.conn_id = p_status->conn_id;
    memcpy(hello_sensor_state.remote_addr, p_status->bd_addr, sizeof(BD_ADDR));

    /* Stop advertising */
    result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, 0, NULL);

    WPRINT_BT_APP_INFO(("Stopping Advertisements%d\n", result));

    memcpy(hello_sensor_hostinfo.bdaddr, p_status->bd_addr, sizeof(BD_ADDR));
    hello_sensor_hostinfo.characteristic_client_configuration = 0;
    hello_sensor_hostinfo.number_of_blinks                    = 0;

#if 1 //feichao
    result =  wiced_bt_gatt_configure_mtu(p_status->conn_id, 256);
    WPRINT_BT_APP_INFO(("wiced_bt_gatt_configure_mtu result = %d\n", result));
#endif

#if 1 //this parameter is for iphone
#define Iphone_min_connect_interval 12 //12 * 1.25 = 15ms
#define Iphone_max_connect_interval 12 // 12 * 1.25 = 15ms
#define Iphone_slave_latency 0
#define Iphone_conn_supervision_timeout 500 //5 seconds
    wiced_bt_l2cap_update_ble_conn_params(hello_sensor_state.remote_addr,
                                          Iphone_min_connect_interval,
                                          Iphone_max_connect_interval,
                                          Iphone_slave_latency,
                                          Iphone_conn_supervision_timeout);
#endif
    return WICED_BT_GATT_SUCCESS;
}

/*
 * This function is invoked when connection is lost
 */
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_down(wiced_bt_gatt_connection_status_t *p_status)
{
    wiced_result_t result;

    WPRINT_BT_APP_INFO(("connection_down  conn_id:%d reason:%d\n", p_status->conn_id, p_status->reason));

    /* Resetting the device info */
    memset(hello_sensor_state.remote_addr, 0, 6);
    hello_sensor_state.conn_id = 0;
    hello_sensor_char_notify_value[5] = '0';
    hello_sensor_char_indicate_value[8] = '0';

    /*
     * If we are configured to stay connected, disconnection was
     * caused by the peer, start low advertisements, so that peer
     * can connect when it wakes up
     */
    if (hello_sensor_state.flag_stay_connected)
    {
        result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_LOW, 0, NULL);
        WPRINT_BT_APP_INFO(("wiced_bt_start_advertisements %d\n", result));
    }

    /* app_powersave_sleep() needs to be called here only in case of Embedded mode.
     * In case of host mode this will be called when BTM_LPM_STATE_LOW_POWER event is received from stack */
#ifdef USE_WICED_HCI
    app_powersave_sleep();
#endif

    return WICED_BT_SUCCESS;
}

/*
 * Connection up/down event
 */
static wiced_bt_gatt_status_t hello_sensor_gatts_connection_status_handler(wiced_bt_gatt_connection_status_t *p_status)
{
    is_connected = p_status->connected;
    if (p_status->connected)
    {
        return hello_sensor_gatts_connection_up(p_status);
    }

    return hello_sensor_gatts_connection_down(p_status);
}

/*
 * Process GATT request from the peer
 */
static wiced_bt_gatt_status_t hello_sensor_gatt_server_request_handler(wiced_bt_gatt_attribute_request_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    WPRINT_BT_APP_INFO(("hello_sensor_gatt_server_request_handler. conn %d, type %d\n", p_data->conn_id, p_data->request_type));

    switch (p_data->request_type)
    {
    case GATTS_REQ_TYPE_READ:
        result = hello_sensor_gatt_server_read_request_handler(p_data->conn_id, &(p_data->data.read_req));
        break;

    case GATTS_REQ_TYPE_WRITE:
        result = hello_sensor_gatt_server_write_request_handler(p_data->conn_id, &(p_data->data.write_req));
        break;

    case GATTS_REQ_TYPE_WRITE_EXEC:
        result = hello_sensor_gatt_server_write_and_execute_request_handler(p_data->conn_id, p_data->data.exec_write);
        break;

    case GATTS_REQ_TYPE_MTU:
        result = hello_sensor_gatt_server_mtu_request_handler(p_data->conn_id, p_data->data.mtu);
        break;

    case GATTS_REQ_TYPE_CONF:
        result = hello_sensor_gatt_server_confirmation_handler(p_data->conn_id, p_data->data.handle);
        break;

    default:
        break;
    }
    return result;
}

/*
 * Callback for various GATT events.  As this application performs only as a GATT server, some of
 * the events are ommitted.
 */
static wiced_bt_gatt_status_t hello_sensor_gatts_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        result = hello_sensor_gatts_connection_status_handler(&p_data->connection_status);
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        result = hello_sensor_gatt_server_request_handler(&p_data->attribute_request);
        break;

    default:
        break;
    }

    return result;
}

#if DEBUG_POWERSAVE
static void application_sleep_event_handler(wiced_sleep_event_type_t event, int handler)
{
    switch (event)
    {
    case WICED_SLEEP_EVENT_ENTER:
        WPRINT_BT_APP_INFO(("ble_hello_sensor : App Entering  Low power mode \n"));
        break;

    case WICED_SLEEP_EVENT_CANCEL:
        WPRINT_BT_APP_INFO(("ble_hello_sensor : App Cancelling Low power mode \n"));
        break;

    case WICED_SLEEP_EVENT_LEAVE:
        WPRINT_BT_APP_INFO(("ble_hello_sensor : App Leaving Low power mode \n"));
        break;

    default:
        break;
    }
}

/* Call back , which is called before entering or after leaving sleep state */
WICED_SLEEP_EVENT_HANDLER(application_sleep_event_handler_1)
{
    application_sleep_event_handler(event, 0);
}
#endif

extern void wiced_bt_gatt_deregister(void);
wiced_result_t hello_sensor_application_deinit(void)
{
    wiced_bt_gatt_status_t gatt_status;
    wiced_result_t         result;

    WPRINT_BT_APP_INFO(("hello_sensor_application_deinit\n"));

    hello_sensor_state.flag_stay_connected = 0;

    if (hello_sensor_state.conn_id != 0)
    {
        result = wiced_bt_gatt_disconnect(hello_sensor_state.conn_id);
        WPRINT_BT_APP_INFO(("wiced_bt_gatt_disconnect: result=%d\n", result));
    }

    rt_thread_mdelay(500);

    /* Stop advertising */
    result =  wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, 0, NULL);
    WPRINT_BT_APP_INFO(("Stopping Advertisements %d\n", result));

    //wiced_bt_gatt_cancel_connect( remote_bd_addr, WICED_TRUE );
    wiced_bt_gatt_deregister();

    memset(&hello_sensor_hostinfo, 0, sizeof(host_info_t));
    memset(&hello_sensor_state, 0, sizeof(hello_sensor_state_t));
    is_connected = WICED_FALSE;

    return result;
}

static void ble_speed(int argc, char **argv)
{
    uint8_t i = 0, ret = 0;
    uint32_t tick1 = 0, tick2 = 0, total = 0;
    tick1 = HAL_GetTick();
    uint32_t delay_ms =  atoi(argv[1]);
    uint32_t speed_avag = 0, cnt = 6;

    rt_kprintf("delay_ms: %d\n", delay_ms);

    while (1)
    {
        i++;
retry:
        ret = wiced_bt_gatt_send_notification(hello_sensor_state.conn_id, HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL, sizeof(hello_sensor_char_notify_value), hello_sensor_char_notify_value);
        if (ret != WICED_BT_GATT_SUCCESS)
        {
            rt_thread_mdelay(delay_ms);
            goto retry;
        }

        total += 244;
        tick2 = HAL_GetTick();
        if ((tick2 - tick1) > 5 * 1000)
        {
            rt_kprintf("speed: %d KB/s\n", total / (tick2 - tick1));
            speed_avag += total / (tick2 - tick1);
            tick1 = HAL_GetTick();
            tick2 = 0;
            total = 0;
            if (--cnt == 0)
            {
                rt_kprintf("speed: %d KB/s last_data; 0x%x\n", speed_avag / 6, --i);
                break;
            }
        }
    }
}
MSH_CMD_EXPORT(ble_speed, ble speed);

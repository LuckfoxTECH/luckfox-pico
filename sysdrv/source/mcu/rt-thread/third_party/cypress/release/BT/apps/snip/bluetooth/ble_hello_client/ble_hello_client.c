/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
*
* BLE Vendor Specific Client Device
*
* The Hello Client application is designed to connect and access services
* of the Hello Sensor device.  Hello Client can connect up to three
* Hello Sensor Device's.  Because handles of the all attributes of
* the Hello Sensor are well known, Hello Client does not perform GATT
* discovery, but uses them directly.  In addition to that Hello Client
* allows another master to connect, so the device will behave as a slave
* in one Bluetooth piconet and a master in another.  To accomplish that
* application can do both advertisements and scan.  Hello Client assumes
* that Hello Sensor advertises a special UUID and connects to the device
* which publishes it.
*
* Features demonstrated
*  - Registration with LE stack for various events
*  - Connection to a master and a slave
*  - As a master processing notifications from the server and
*    sending notifications to the client
*  - As a slave processing writes from the client and sending writes
*    to the server
*
*/
#include <string.h>
#include "wiced_bt_dev.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced.h"
#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "wwd_debug.h"
#include "wiced_bt_dev.h"
#include "wiced_low_power.h"
#include "wiced_bt_ble.h"


/******************************************************************************
 *                                Constants
 ******************************************************************************/

#define HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL          0x2B
#define HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL           0x2D

/* UUID value of the Hello Sensor Service */
#define UUID_HELLO_SERVICE                    0x23, 0x20, 0x56, 0x7c, 0x05, 0xcf, 0x6e, 0xb4, 0xc3, 0x41, 0x77, 0x28, 0x51, 0x82, 0x7e, 0x1b

/******************************************************************************
 *                          Type  Definitions
 ******************************************************************************/
/* structure to store GATT attributes for read/write operations */
typedef struct
{
    uint16_t    handle;
    uint16_t    attr_len;
    const void *p_attr;
} gatt_attribute_t;

/* Peer Info */
typedef struct
{
    uint16_t conn_id;                   // Connection Identifier
    uint8_t  role;                      // master or slave in the current connection
    uint8_t  addr_type;                 // peer address type
    uint8_t  transport;                 // peer connected transport
    uint8_t  peer_addr[BD_ADDR_LEN];    // Peer BD Address
} hello_client_peer_info_t;

/* Host information to be stored in NVRAM */
typedef struct
{
    BD_ADDR  bdaddr;                                // BD address of the bonded host
    uint16_t characteristic_client_configuration;   // Current value of the client configuration descriptor
} hclient_host_info_t;

/* Hello client application info */
typedef struct
{
    uint32_t                 app_timer_count;                         // App Timer Count
    uint16_t                 conn_id;                                 // Hold the slave connection id
    uint16_t                 master_conn_id;                          // Handle of the master connection
    uint8_t                  battery_level;                           // dummy battery level
    hclient_host_info_t      host_info;                               // NVRAM save area
    hello_client_peer_info_t peer_info; // Peer Info
} hello_client_app_t;

/******************************************************************************
 *                            Variables Definitions
 ******************************************************************************/

/* Holds the hello client app info */
hello_client_app_t g_hello_client;

/* Hello service UUID  */
const uint8_t hello_service[16] = {UUID_HELLO_SERVICE};

/* Variable to indicate if the scan has to be started.
 * Set to 1 if the user pushes and holds the button for more than 5 seconds */
uint8_t start_scan = 0;

extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;
extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
static void                     hello_client_app_init(void);
static wiced_result_t           hello_client_management_cback(wiced_bt_management_evt_t event,  wiced_bt_management_evt_data_t *p_event_data);
static wiced_bt_gatt_status_t   hello_client_gatt_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data);
static wiced_bt_gatt_status_t   hello_client_gatt_connection_up(wiced_bt_gatt_connection_status_t *p_conn_status);
static wiced_bt_gatt_status_t   hello_client_gatt_connection_down(wiced_bt_gatt_connection_status_t *p_conn_status);
static wiced_bt_gatt_status_t   hello_client_gatt_op_comp_cb(wiced_bt_gatt_operation_complete_t *p_data);
static void                     hello_client_scan_result_cback(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data);
static void                     hello_client_smp_bond_result(BD_ADDR bda, uint8_t result);
static void                     hello_client_encryption_changed(wiced_result_t result, uint8_t *p_bd_addr);
static void                     hello_client_process_data_from_slave(int len, uint8_t *data);
static void                     hello_client_gatt_enable_notification(void);

hello_client_peer_info_t *hello_client_get_peer_information(uint16_t conn_id);
void hello_client_add_peer_info(uint16_t conn_id, uint8_t *p_bd_addr, uint8_t role, uint8_t transport, uint8_t address_type);
void hello_client_remove_peer_info(uint16_t conn_id);

/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
void application_start(void)
{
    wiced_core_init();

    // WICED BT Stack initialization and registering the managment callback
    wiced_bt_stack_init(hello_client_management_cback,
                        &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools);
}

/*
 * hello client bt/ble device and link management callbacks
 */
wiced_result_t hello_client_management_cback(wiced_bt_management_evt_t event,  wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_result_t               result = WICED_BT_SUCCESS;

    WPRINT_APP_INFO(("hello_client_management_cback: %x\n", event));

    switch (event)
    {
    /* Bluetooth  stack enabled */
    case BTM_ENABLED_EVT:

        hello_client_app_init();

        //start looking for hello sensor
        start_scan = 1;
        if (wiced_bt_ble_get_current_scan_state() == BTM_BLE_SCAN_TYPE_NONE)
        {
            result = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, hello_client_scan_result_cback);
            WPRINT_APP_INFO(("\nhello_client_start_scan: %d\n", result));
        }

        break;

    case BTM_DISABLED_EVT:
        break;

    case BTM_USER_CONFIRMATION_REQUEST_EVT:
        WPRINT_APP_INFO(("Numeric_value: %ld \n", p_event_data->user_confirmation_request.numeric_value));
        wiced_bt_dev_confirm_req_reply(WICED_BT_SUCCESS, p_event_data->user_confirmation_request.bd_addr);
        break;

    case BTM_PASSKEY_NOTIFICATION_EVT:
        WPRINT_APP_INFO(("PassKey Notification. Key %ld \n", p_event_data->user_passkey_notification.passkey));
        wiced_bt_dev_confirm_req_reply(WICED_BT_SUCCESS, p_event_data->user_passkey_notification.bd_addr);
        break;

    case BTM_PAIRING_IO_CAPABILITIES_BLE_REQUEST_EVT:
        p_event_data->pairing_io_capabilities_ble_request.local_io_cap      = BTM_IO_CAPABILITIES_NONE;
        p_event_data->pairing_io_capabilities_ble_request.oob_data          = BTM_OOB_NONE;
        p_event_data->pairing_io_capabilities_ble_request.auth_req          = BTM_LE_AUTH_REQ_NO_BOND;
        p_event_data->pairing_io_capabilities_ble_request.max_key_size      = 0x10;
        p_event_data->pairing_io_capabilities_ble_request.init_keys         = BTM_LE_KEY_PENC | BTM_LE_KEY_PID ;
        p_event_data->pairing_io_capabilities_ble_request.resp_keys         = BTM_LE_KEY_PENC | BTM_LE_KEY_PID ;
        break;

    case BTM_PAIRING_COMPLETE_EVT:
    {
        wiced_bt_dev_ble_pairing_info_t *p_info = &p_event_data->pairing_complete.pairing_complete_info.ble;

        WPRINT_APP_INFO(("Pairing Complete: %d\n", p_info->reason));

        hello_client_smp_bond_result(p_event_data->pairing_complete.bd_addr, p_info->reason);
    }
    break;

    case BTM_ENCRYPTION_STATUS_EVT:
    {
        wiced_bt_dev_encryption_status_t *p_status =
            &p_event_data->encryption_status;

        WPRINT_APP_INFO(("encryption status: res %d\n", p_status->result));

        hello_client_encryption_changed(p_status->result,
                                        p_status->bd_addr);
    }
    break;

    case BTM_SECURITY_REQUEST_EVT:
        /* Use the default security */
        wiced_bt_ble_security_grant(p_event_data->security_request.bd_addr, WICED_BT_SUCCESS);
        break;

    case BTM_BLE_SCAN_STATE_CHANGED_EVT:
        WPRINT_APP_INFO(("Scan State Change: %d\n", p_event_data->ble_scan_state_changed));
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
        return WICED_ERROR;
        break;

    default:
        break;
    }

    return result;
}

/*
 * WICED BT Init Complete.  This function is called when device initialization
 * has been completed.  Perform the App Initializations & Callback Registrations
 */
void hello_client_app_init(void)
{
    wiced_bt_gatt_status_t gatt_status;

    WPRINT_APP_INFO(("hello_client_app_init\n"));

    memset(&g_hello_client, 0, sizeof(g_hello_client));

    /* Register with stack to receive GATT related events */
    gatt_status = wiced_bt_gatt_register(hello_client_gatt_callback);

    WPRINT_APP_INFO(("wiced_bt_gatt_register status %d \n", gatt_status));

}

/*
 * Callback function is executed to process various GATT events
 */
wiced_bt_gatt_status_t hello_client_gatt_callback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data)
{
    wiced_bt_gatt_status_t result = WICED_BT_SUCCESS;

    WPRINT_APP_INFO(("hello_client_gatt_callback event %d \n", event));

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        if (p_data->connection_status.connected)
        {
            result = hello_client_gatt_connection_up(&p_data->connection_status);
        }
        else
        {
            result = hello_client_gatt_connection_down(&p_data->connection_status);
        }
        break;

    case GATT_OPERATION_CPLT_EVT:
        result = hello_client_gatt_op_comp_cb(&p_data->operation_complete);
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        WPRINT_APP_INFO(("received GATT_ATTRIBUTE_REQUEST_EVT\n"));
        break;

    default:
        break;
    }

    return result;
}

/* This function will be called on every connection establishment */
/* This function is invoked when connection is established */
wiced_bt_gatt_status_t hello_client_gatt_connection_up(wiced_bt_gatt_connection_status_t *p_conn_status)
{
    uint8_t dev_role;

    wiced_bt_dev_get_role(p_conn_status->bd_addr, &dev_role, BT_TRANSPORT_LE);

    // Adding the peer info
    hello_client_add_peer_info(p_conn_status->conn_id, p_conn_status->bd_addr, dev_role, p_conn_status->transport, p_conn_status->addr_type);

    WPRINT_APP_INFO(("hclient_connection_up Conn Id:%d Addr:<%X:%X:%X:%X:%X:%X> role:%d\n ", p_conn_status->conn_id,
                     p_conn_status->bd_addr[0], p_conn_status->bd_addr[1], p_conn_status->bd_addr[2],
                     p_conn_status->bd_addr[3], p_conn_status->bd_addr[4], p_conn_status->bd_addr[5],
                     dev_role));

    g_hello_client.conn_id = p_conn_status->conn_id;

    /* Configure to receive notification from server */
    hello_client_gatt_enable_notification();

    return WICED_BT_GATT_SUCCESS;
}

/* This function will be called when connection goes down */
wiced_bt_gatt_status_t hello_client_gatt_connection_down(wiced_bt_gatt_connection_status_t *p_conn_status)
{
    WPRINT_APP_INFO(("hello_client_connection_down <%X:%X:%X:%X:%X:%X>\n", p_conn_status->bd_addr[0], p_conn_status->bd_addr[1], p_conn_status->bd_addr[2],
                     p_conn_status->bd_addr[3], p_conn_status->bd_addr[4], p_conn_status->bd_addr[5]));

    //Remove the peer info
    hello_client_remove_peer_info(p_conn_status->conn_id);

    return WICED_BT_GATT_SUCCESS;
}

/*
 * GATT operation started by the client has been completed
 */
wiced_bt_gatt_status_t hello_client_gatt_op_comp_cb(wiced_bt_gatt_operation_complete_t *p_data)
{

    WPRINT_APP_INFO(("hello_client_gatt_op_comp_cb conn %d op %d st %d\n", p_data->conn_id, p_data->op, p_data->status));

    switch (p_data->op)
    {
    case GATTC_OPTYPE_READ:
        WPRINT_APP_INFO(("read_rsp status:%d\n", p_data->status));
        break;

    case GATTC_OPTYPE_WRITE:
        WPRINT_APP_INFO(("write_rsp status:%d\n", p_data->status));
        break;

    case GATTC_OPTYPE_CONFIG:
        WPRINT_APP_INFO(("peer mtu:%d\n", p_data->response_data.mtu));
        break;

    case GATTC_OPTYPE_NOTIFICATION:
        hello_client_process_data_from_slave(p_data->response_data.att_value.len, p_data->response_data.att_value.p_data);
        break;

    case GATTC_OPTYPE_INDICATION:
        hello_client_process_data_from_slave(p_data->response_data.att_value.len, p_data->response_data.att_value.p_data);
        wiced_bt_gatt_send_indication_confirm(p_data->conn_id, p_data->response_data.handle);
        break;
    }
    return WICED_BT_GATT_SUCCESS;
}

/*
 * This function handles notification/indication data received fromt the slave device
 */
void hello_client_process_data_from_slave(int len, uint8_t *data)
{

    wiced_bt_gatt_status_t status;
    static uint16_t val = 0;
    wiced_bt_gatt_value_t write_buffer;
    uint8_t *buf_data = &write_buffer.value[0];

    WPRINT_APP_INFO(("hello_client_process_data_from_slave len:%d master conn_id:%d\n", len, g_hello_client.master_conn_id));

    write_buffer.handle   = HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL; /* hard coded server ccd */
    write_buffer.offset   = 0;
    write_buffer.len      = 2;
    write_buffer.auth_req = GATT_AUTH_REQ_NONE;
    buf_data[0] = val & 0xff;           /* write_buffer->value[0] */
    buf_data[1] = (val >> 8) & 0xff;;   /* write_buffer->value[1] - fix compiler error */


    val++;

    // Register with the server to receive notification
    status = wiced_bt_gatt_send_write(g_hello_client.conn_id, GATT_WRITE, &write_buffer);

    WPRINT_APP_INFO(("wiced_bt_gatt_send_write %d\n", status));
}

/*
 * Process SMP bonding result.  If we successfully paired with the
 * central device, save its BDADDR in the NVRAM and initialize
 * associated data
 */
void hello_client_smp_bond_result(BD_ADDR bda, uint8_t result)
{
    wiced_result_t status;

    /*  Start the inquiry to search for other available slaves */
    if (start_scan)
    {
        if (wiced_bt_ble_get_current_scan_state() == BTM_BLE_SCAN_TYPE_NONE)
        {
            status = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, hello_client_scan_result_cback);
            WPRINT_APP_INFO(("hello_client_smp_bond_result wiced_bt_ble_scan: %d\n", status));
        }
    }
}

/*
 * Process notification from the stack that encryption has been set.
 * If connected client is registered for notification or indication,
 * it is a good time to send it out
 */
void hello_client_encryption_changed(wiced_result_t result, uint8_t *p_bd_addr)
{
    WPRINT_APP_INFO(("hello_client_encryption_changed %d", result));

    /* Bonding success */
    if (result == WICED_BT_SUCCESS)
    {
        // When we are connected as a master, we need to enable notifications from the slave
        // This needs to be done only once, because client configuration descriptor value
        // should be persistent across connections with bonded devices.
        hello_client_gatt_enable_notification();
    }
}

/*
 * This function handles the scan results
 */
void hello_client_scan_result_cback(wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data)
{
    wiced_result_t         status;
    wiced_bool_t           ret_status;
    uint8_t                length;
    uint8_t               *p_data;

    WPRINT_APP_INFO((" Found Device : %X:%X:%X:%X:%X:%X \n", p_scan_result->remote_bd_addr[0], p_scan_result->remote_bd_addr[1], p_scan_result->remote_bd_addr[2],
                     p_scan_result->remote_bd_addr[3], p_scan_result->remote_bd_addr[4], p_scan_result->remote_bd_addr[5]));

    if (p_scan_result)
    {
        // Advertisement data from hello_server should have Advertisement type SERVICE_UUID_128
        p_data = wiced_bt_ble_check_advertising_data(p_adv_data, BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE, &length);

        // Check if  the hello service uuid is there in the advertisement
        if ((p_data == NULL) || (length != LEN_UUID_128) || (memcmp(p_data, hello_service, LEN_UUID_128) != 0))
        {
            // wrong device
            return;
        }

        start_scan = 0;

        /* Stop the scan since the desired device is found */
        status = wiced_bt_ble_scan(BTM_BLE_SCAN_TYPE_NONE, WICED_TRUE, hello_client_scan_result_cback);

        WPRINT_APP_INFO(("scan off status %d\n", status));

        /* Initiate the connection */
        ret_status = wiced_bt_gatt_le_connect(p_scan_result->remote_bd_addr, p_scan_result->ble_addr_type, BLE_CONN_MODE_HIGH_DUTY, TRUE);

        WPRINT_APP_INFO(("wiced_bt_gatt_connect status %d\n", ret_status));
    }
    else
    {
        WPRINT_APP_INFO(("Scan completed.\n"));
    }
}

/*
 * This function writes into peer's client configuration descriptor to enable notifications
 */
void hello_client_gatt_enable_notification(void)
{
    wiced_bt_gatt_status_t status;
    uint16_t               u16 = GATT_CLIENT_CONFIG_NOTIFICATION;
    wiced_bt_gatt_value_t write_buffer;
    wiced_bt_gatt_value_t *p_write = &write_buffer;

    if (p_write)
    {
        uint8_t *data = p_write->value;
        p_write->handle   = HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL; /* hard coded server ccd */
        p_write->offset   = 0;
        p_write->len      = 2;
        p_write->auth_req = GATT_AUTH_REQ_NONE;
        data[0] = u16 & 0xff;           /* p_write->value[0] */
        data[1] = (u16 >> 8) & 0xff;;   /* p_write->value[1] - fix compiler error */

        // Register with the server to receive notification
        status = wiced_bt_gatt_send_write(g_hello_client.conn_id, GATT_WRITE, p_write);

        WPRINT_APP_INFO(("wiced_bt_gatt_send_write %d\n", status));
    }
}

/*
 * This function gets the peer address from  the table
 */
hello_client_peer_info_t *hello_client_get_peer_information(uint16_t conn_id)
{
    if (g_hello_client.peer_info.conn_id == conn_id)
    {
        return &g_hello_client.peer_info;
    }

    return NULL;
}

/*
 * This function adds the peer information to the table
 */
void hello_client_add_peer_info(uint16_t conn_id, uint8_t *p_bd_addr, uint8_t role, uint8_t transport, uint8_t address_type)
{
    if (g_hello_client.peer_info.conn_id == 0)
    {
        g_hello_client.peer_info.conn_id         = conn_id;
        g_hello_client.peer_info.role            = role;
        g_hello_client.peer_info.transport       = transport;
        g_hello_client.peer_info.addr_type       = address_type;
        memcpy(g_hello_client.peer_info.peer_addr, p_bd_addr, BD_ADDR_LEN);
    }
}

/*
 * This function removes the peer information from the table
 */
void hello_client_remove_peer_info(uint16_t conn_id)
{
    if (g_hello_client.peer_info.conn_id == conn_id)
    {
        g_hello_client.peer_info.conn_id = 0;
    }
}


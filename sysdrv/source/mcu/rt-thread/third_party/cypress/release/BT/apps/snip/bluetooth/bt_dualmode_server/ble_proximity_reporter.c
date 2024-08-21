/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * BLE Proximity Reporter Sample Application
 *
 */

#include "wiced_bt_cfg.h"
#include <string.h>
#include <stdio.h>
#include "wiced.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced_bt_gatt_db.h"
#include "bt_rfcomm_server.h"
#include "wwd_debug.h"

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
 *               Function Declarations
 ******************************************************/
extern void                   ble_proximity_reporter_init(void);
static void                   ble_proximity_tx_power_callback(wiced_bt_tx_power_result_t *p_tx_power);
static wiced_bt_gatt_status_t ble_proximity_gatt_write_request(wiced_bt_gatt_write_t *p_write_request);
static wiced_bt_gatt_status_t ble_proximity_gatt_read_request(wiced_bt_gatt_read_t *p_read_request);
static wiced_bt_gatt_status_t ble_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data);

/******************************************************
 *               Variable Definitions
 ******************************************************/
extern wiced_bt_cfg_settings_t wiced_bt_cfg_settings;

/* Proximity reporter attribute values */
static uint8_t  proximity_immediate_alert_level;
static uint8_t  proximity_link_loss_alert_level;
static int8_t   proximity_tx_power_level;

/* GATT attribute values */
static uint32_t proximity_gatt_attribute_service_changed = 0;
static uint16_t proximity_gatt_generic_access_appearance = 0;

/******************************************************
 *               Function Definitions
 ******************************************************/

/* TX Power report handler */
static void ble_proximity_tx_power_callback(wiced_bt_tx_power_result_t *p_tx_power)
{
    if ((p_tx_power->status == WICED_BT_SUCCESS) && (p_tx_power->hci_status == HCI_SUCCESS))
    {
        WPRINT_BT_APP_INFO(("Local TX power: %i\n", p_tx_power->tx_power));
        proximity_tx_power_level = p_tx_power->tx_power;
    }
    else
    {
        WPRINT_BT_APP_INFO(("Unable to read Local TX power. (btm_status=0x%x, hci_status=0x%x)\n", p_tx_power->status, p_tx_power->hci_status));
        proximity_tx_power_level = 0;
    }
}

/* Handler for attribute write requests */
static wiced_bt_gatt_status_t ble_proximity_gatt_write_request(wiced_bt_gatt_write_t *p_write_request)
{
    uint8_t attribute_value = *(uint8_t *)p_write_request->p_val;
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;

    switch (p_write_request->handle)
    {
    case HDLC_LINK_LOSS_ALERT_LEVEL_VALUE:
        proximity_link_loss_alert_level = attribute_value;
        WPRINT_BT_APP_INFO(("Link loss alert level changed to: %i\n", attribute_value));
        break;

    case HDLC_IMMEDIATE_ALERT_LEVEL_VALUE:
        proximity_immediate_alert_level = attribute_value;
        WPRINT_BT_APP_INFO(("Proximity alert (level: %i)\n", attribute_value));
        break;

    default:
        WPRINT_BT_APP_INFO(("Write request to invalid handle: 0x%x\n", p_write_request->handle));
        status = WICED_BT_GATT_WRITE_NOT_PERMIT;
        break;
    }
    return (status);
}

/* Handler for attribute read requests */
static wiced_bt_gatt_status_t ble_proximity_gatt_read_request(wiced_bt_gatt_read_t *p_read_request)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;
    void *p_attribute_value_source;
    uint16_t attribute_value_length = 0;

    switch (p_read_request->handle)
    {
    case HDLC_GENERIC_ATTRIBUTE_SERVICE_CHANGED_VALUE:
        p_attribute_value_source    = &proximity_gatt_attribute_service_changed;
        attribute_value_length      = sizeof(proximity_gatt_attribute_service_changed);
        break;

    case HDLC_GENERIC_ACCESS_DEVICE_NAME_VALUE:
        p_attribute_value_source    = (void *)wiced_bt_cfg_settings.device_name;
        attribute_value_length      = strlen((char *)wiced_bt_cfg_settings.device_name);
        WPRINT_BT_APP_INFO(("WICED  BT NAME: %s \r\n", ((char *)p_attribute_value_source)));
        break;

    case HDLC_GENERIC_ACCESS_APPEARANCE_VALUE:
        p_attribute_value_source    = &proximity_gatt_generic_access_appearance;
        attribute_value_length      = sizeof(proximity_gatt_generic_access_appearance);
        break;

    case HDLC_TX_POWER_LEVEL_VALUE:
        p_attribute_value_source    = &proximity_tx_power_level;
        attribute_value_length      = sizeof(proximity_tx_power_level);
        break;

    case HDLC_LINK_LOSS_ALERT_LEVEL_VALUE:
        p_attribute_value_source    = &proximity_link_loss_alert_level;
        attribute_value_length      = sizeof(proximity_link_loss_alert_level);
        break;

    default:
        status = WICED_BT_GATT_READ_NOT_PERMIT;
        WPRINT_BT_APP_INFO(("Read request to invalid handle: 0x%x\n", p_read_request->handle));
        break;
    }
    /* Validate destination buffer size */
    if (attribute_value_length > *p_read_request->p_val_len)
    {
        *p_read_request->p_val_len = attribute_value_length;
    }
    /* Copy the attribute value */
    if (attribute_value_length)
    {
        memcpy(p_read_request->p_val, p_attribute_value_source, attribute_value_length);
    }
    /* Indicate number of bytes copied */
    *p_read_request->p_val_len = attribute_value_length;
    return (status);
}

/* GATT event handler */
static wiced_bt_gatt_status_t ble_proximity_gatt_cback(wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_event_data)
{
    wiced_bt_gatt_status_t status = WICED_BT_GATT_SUCCESS;
    uint8_t *bda;

    switch (event)
    {
    case GATT_CONNECTION_STATUS_EVT:
        /* GATT connection status change */
        bda = p_event_data->connection_status.bd_addr;
        WPRINT_BT_APP_INFO(("GATT connection to [%02X:%02X:%02X:%02X:%02X:%02X] %s.\n", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5], (p_event_data->connection_status.connected ? "established" : "released")));

        if (p_event_data->connection_status.connected)
        {
            /* Connection established. Get current TX power  (required for setting TX power attribute in GATT database) */
            wiced_bt_dev_read_tx_power(p_event_data->connection_status.bd_addr, p_event_data->connection_status.transport, (wiced_bt_dev_cmpl_cback_t *) ble_proximity_tx_power_callback);

            /* Disable connectability. */
            wiced_bt_start_advertisements(BTM_BLE_ADVERT_OFF, 0, NULL);
        }
        else
        {
            /* Connection released. Re-enable BLE connectability. */
            wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
            /* test code for directed adv */
            /* wiced_bt_start_advertisements (BTM_BLE_ADVERT_DIRECTED_HIGH, 0, p_event_data->connection_status.bd_addr); */
            WPRINT_BT_APP_INFO(("Waiting for proximity monitor to connect...\n"));
        }
        break;

    case GATT_ATTRIBUTE_REQUEST_EVT:
        /* GATT attribute read/write request */
        if (p_event_data->attribute_request.request_type == GATTS_REQ_TYPE_WRITE)
        {
            status = ble_proximity_gatt_write_request(&p_event_data->attribute_request.data.write_req);
        }
        else if (p_event_data->attribute_request.request_type == GATTS_REQ_TYPE_READ)
        {
            status = ble_proximity_gatt_read_request(&p_event_data->attribute_request.data.read_req);

        }
        break;

    default:
        break;
    }

    return (status);
}

/* Initialize Proximity Reporter */
void ble_proximity_reporter_init(void)
{
    wiced_bt_ble_advert_elem_t adv_elem[2];
    uint8_t num_elem                            = 0;
    uint8_t ble_advertisement_flag_value        = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len          = 1;
    adv_elem[num_elem].p_data       = &ble_advertisement_flag_value;

    num_elem ++;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len          = strlen((const char *)wiced_bt_cfg_settings.device_name);
    adv_elem[num_elem].p_data       = wiced_bt_cfg_settings.device_name;

    num_elem ++;

    /* Set advertising data: device name and discoverable flag */
    wiced_bt_ble_set_raw_advertisement_data(num_elem, adv_elem);

    /* Register for gatt event notifications */
    wiced_bt_gatt_register(&ble_proximity_gatt_cback);

    /* Initialize GATT database */
    wiced_bt_gatt_db_init((uint8_t *)gatt_db, gatt_db_size);

    /* Enable Bluetooth LE advertising and connectability */

    /* start LE advertising */
    wiced_bt_start_advertisements(BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL);
    WPRINT_BT_APP_INFO(("Waiting for proximity monitor to connect...\n"));
}

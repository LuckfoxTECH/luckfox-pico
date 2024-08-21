/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * This file provides definitions and function prototypes for Hello Sensor
 * device
 *
 */
#include "wiced_bt_dev.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 *                                Constants
 ******************************************************************************/
#define HELLO_SENSOR_MAX_NUM_CLIENTS 1

/* Hello Sensor App Timer Timeout in seconds  */
#define HELLO_SENSOR_APP_TIMEOUT_IN_SECONDS                 1

/* Hello Sensor App Fine Timer Timeout in milli seconds  */
#define HELLO_SENSOR_APP_FINE_TIMEOUT_IN_MS                 1000

/* Hello Sensor Connection Idle  Timeout in milli seconds  */
#define HELLO_SENSOR_CONN_IDLE_TIMEOUT_IN_SECONDS           3


/******************************************************************************
 *                          Constants
 ******************************************************************************/

/* UUID value of the Hello Sensor Service */
#define UUID_HELLO_SERVICE                    0x23, 0x20, 0x56, 0x7c, 0x05, 0xcf, 0x6e, 0xb4, 0xc3, 0x41, 0x77, 0x28, 0x51, 0x82, 0x7e, 0x1b

/* UUID value of the Hello Sensor Characteristic, Value Notification */
#define UUID_HELLO_CHARACTERISTIC_NOTIFY      0x26, 0xf6, 0x69, 0x91, 0x68, 0xee, 0xc2, 0xbe, 0x44, 0x4d, 0xb9, 0x5c, 0x3f, 0x2d, 0xc3, 0x8a

/* UUID value of the Hello Sensor Characteristic, Configuration */
#define UUID_HELLO_CHARACTERISTIC_CONFIG      0x1a, 0x89, 0x07, 0x4a, 0x2f, 0x3b, 0x7e, 0xa6, 0x81, 0x44, 0x3f, 0xf9, 0xa8, 0xf2, 0x9b, 0x5e

/* UUID value of the Hello Sensor Characteristic, Configuration */
#define UUID_HELLO_CHARACTERISTIC_LONG_MSG    0x2a, 0x99, 0x17, 0x5a, 0x3f, 0x4b, 0x8e, 0xb6, 0x91, 0x54, 0x2f, 0x09, 0xb8, 0x02, 0xab, 0x6e

/******************************************************************************
 *                         Type Definitions
 ******************************************************************************/
typedef enum
{
    HANDLE_HSENS_GATT_SERVICE = 0x1,

    HANDLE_HSENS_GAP_SERVICE = 0x14,
    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME,
    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_NAME_VAL,

    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE,
    HANDLE_HSENS_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,

    HANDLE_HSENS_SERVICE = 0x28,
    HANDLE_HSENS_SERVICE_CHAR_NOTIFY,
    HANDLE_HSENS_SERVICE_CHAR_NOTIFY_VAL,
    HANDLE_HSENS_SERVICE_CHAR_CFG_DESC,

    HANDLE_HSENS_SERVICE_CHAR_BLINK,
    HANDLE_HSENS_SERVICE_CHAR_BLINK_VAL,

    HANDLE_HSENS_SERVICE_CHAR_LONG_MSG,
    HANDLE_HSENS_SERVICE_CHAR_LONG_MSG_VAL,

    HANDLE_HSENS_DEV_INFO_SERVICE = 0x40,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MFR_NAME_VAL,

    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_MODEL_NUM_VAL,

    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID,
    HANDLE_HSENS_DEV_INFO_SERVICE_CHAR_SYSTEM_ID_VAL,

    HANDLE_HSENS_BATTERY_SERVICE = 0x60,
    HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL,
    HANDLE_HSENS_BATTERY_SERVICE_CHAR_LEVEL_VAL,

    /* Client Configuration */
    HDLD_CURRENT_TIME_SERVICE_CURRENT_TIME_CLIENT_CONFIGURATION,
} hello_sensor_db_tags;

extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;
extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];


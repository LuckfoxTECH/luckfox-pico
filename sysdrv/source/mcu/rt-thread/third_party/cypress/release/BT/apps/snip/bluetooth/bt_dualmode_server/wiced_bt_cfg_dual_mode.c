/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * Runtime Bluetooth stack configuration parameters
 *
 */
#include "wiced_bt_cfg.h"

/******************************************************
 *                     Macros
*******************************************************/

/******************************************************
 *               Variables Definitions
 *****************************************************/

/*****************************************************
 *         wiced_bt core stack configuration
 *****************************************************/
wiced_bt_cfg_settings_t wiced_bt_cfg_settings =
{
    (uint8_t *)"WICED DUAL MODE",                                   /**< Local device name (NULL terminated) */
    {0x00, 0x00, 0x00},                                             /**< Local device class */
    BTM_SEC_NONE,                                                   /**< Security requirements mask (BTM_SEC_NONE, or combinination of BTM_SEC_IN_AUTHENTICATE, BTM_SEC_OUT_AUTHENTICATE, BTM_SEC_ENCRYPT (see #wiced_bt_sec_level_e)) */
    3,                                                              /**< Maximum number simultaneous links to different devices */

    /* BR/EDR scan config */
    {
        BTM_SCAN_TYPE_STANDARD,                                     /**< Inquiry scan type (BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED) */
        WICED_BT_CFG_DEFAULT_INQUIRY_SCAN_INTERVAL,                 /**< Inquiry scan interval  (0 to use default) */
        WICED_BT_CFG_DEFAULT_INQUIRY_SCAN_WINDOW,                   /**< Inquiry scan window (0 to use default) */

        BTM_SCAN_TYPE_STANDARD,                                     /**< Page scan type (BTM_SCAN_TYPE_STANDARD or BTM_SCAN_TYPE_INTERLACED) */
        WICED_BT_CFG_DEFAULT_PAGE_SCAN_INTERVAL,                    /**< Page scan interval  (0 to use default) */
        WICED_BT_CFG_DEFAULT_PAGE_SCAN_WINDOW                       /**< Page scan window (0 to use default) */
    },

    /* BLE scan settings  */
    {
        BTM_BLE_SCAN_MODE_PASSIVE,                                  /**< BLE scan mode (BTM_BLE_SCAN_MODE_PASSIVE, BTM_BLE_SCAN_MODE_ACTIVE, or BTM_BLE_SCAN_MODE_NONE) */

        WICED_BT_CFG_DEFAULT_HIGH_DUTY_SCAN_INTERVAL,               /**< High duty scan interval */
        WICED_BT_CFG_DEFAULT_HIGH_DUTY_SCAN_WINDOW,                 /**< High duty scan window */
        5,                                                          /**< High duty scan duration in seconds (0 for infinite) */

        WICED_BT_CFG_DEFAULT_LOW_DUTY_SCAN_INTERVAL,                /**< Low duty scan interval  */
        WICED_BT_CFG_DEFAULT_LOW_DUTY_SCAN_WINDOW,                  /**< Low duty scan window */
        5,                                                          /**< Low duty scan duration in seconds (0 for infinite) */

        /* Connection scan intervals */
        WICED_BT_CFG_DEFAULT_HIGH_DUTY_CONN_SCAN_INTERVAL,          /**< High duty cycle connection scan interval */
        WICED_BT_CFG_DEFAULT_HIGH_DUTY_CONN_SCAN_WINDOW,            /**< High duty cycle connection scan window */
        30,                                                         /**< High duty cycle connection duration in seconds (0 for infinite) */

        WICED_BT_CFG_DEFAULT_LOW_DUTY_CONN_SCAN_INTERVAL,           /**< Low duty cycle connection scan interval */
        WICED_BT_CFG_DEFAULT_LOW_DUTY_CONN_SCAN_WINDOW,             /**< Low duty cycle connection scan window */
        30,                                                         /**< Low duty cycle connection duration in seconds (0 for infinite) */

        /* Connection configuration */
        WICED_BT_CFG_DEFAULT_CONN_MIN_INTERVAL,                     /**< Minimum connection event interval */
        WICED_BT_CFG_DEFAULT_CONN_MAX_INTERVAL,                     /**< Maximum connection event interval */
        WICED_BT_CFG_DEFAULT_CONN_LATENCY,                          /**< Connection Latency */
        WICED_BT_CFG_DEFAULT_CONN_SUPERVISION_TIMEOUT               /**< Connection supervision timeout */
    },

    /* BLE advertisement settings */
    {
        BTM_BLE_ADVERT_CHNL_37 |                                    /**< Advertising channel map (mask of BTM_BLE_ADVERT_CHNL_37, BTM_BLE_ADVERT_CHNL_38, BTM_BLE_ADVERT_CHNL_39) */
        BTM_BLE_ADVERT_CHNL_38 |
        BTM_BLE_ADVERT_CHNL_39,

        WICED_BT_CFG_DEFAULT_HIGH_DUTY_ADV_MIN_INTERVAL,            /**< High duty undirected connectable minimum advertising interval */
        WICED_BT_CFG_DEFAULT_HIGH_DUTY_ADV_MAX_INTERVAL,            /**< High duty undirected connectable maximum advertising interval */
        60,                                                         /**< High duty undirected connectable advertising duration in seconds (0 for infinite) */

        WICED_BT_CFG_DEFAULT_LOW_DUTY_ADV_MIN_INTERVAL,             /**< Low duty undirected connectable minimum advertising interval */
        WICED_BT_CFG_DEFAULT_LOW_DUTY_ADV_MAX_INTERVAL,             /**< Low duty undirected connectable maximum advertising interval */
        60,                                                         /**< Low duty undirected connectable advertising duration in seconds (0 for infinite) */

        WICED_BT_CFG_DEFAULT_HIGH_DUTY_DIRECTED_ADV_MIN_INTERVAL,   /**< High duty directed connectable minimum advertising interval */
        WICED_BT_CFG_DEFAULT_HIGH_DUTY_DIRECTED_ADV_MAX_INTERVAL,   /**< High duty directed connectable maximum advertising interval */

        WICED_BT_CFG_DEFAULT_LOW_DUTY_DIRECTED_ADV_MIN_INTERVAL,    /**< Low duty directed connectable minimum advertising interval */
        WICED_BT_CFG_DEFAULT_LOW_DUTY_DIRECTED_ADV_MAX_INTERVAL,    /**< Low duty directed connectable maximum advertising interval */
        30,                                                         /**< Low duty directed connectable advertising duration in seconds (0 for infinite) */

        WICED_BT_CFG_DEFAULT_HIGH_DUTY_NONCONN_ADV_MIN_INTERVAL,    /**< High duty non-connectable minimum advertising interval */
        WICED_BT_CFG_DEFAULT_HIGH_DUTY_NONCONN_ADV_MAX_INTERVAL,    /**< High duty non-connectable maximum advertising interval */
        30,                                                         /**< High duty non-connectable advertising duration in seconds (0 for infinite) */

        WICED_BT_CFG_DEFAULT_LOW_DUTY_NONCONN_ADV_MIN_INTERVAL,     /**< Low duty non-connectable minimum advertising interval */
        WICED_BT_CFG_DEFAULT_LOW_DUTY_NONCONN_ADV_MAX_INTERVAL,     /**< Low duty non-connectable maximum advertising interval */
        0                                                           /**< Low duty non-connectable advertising duration in seconds (0 for infinite) */
    },

    /* GATT configuration */
    {
        APPEARANCE_GENERIC_TAG,                                     /**< GATT appearance (see gatt_appearance_e) */
        1,                                                          /**< Client config: maximum number of servers that local client can connect to  */
        1,                                                          /**< Server config: maximum number of remote clients connections allowed by the local */
        255,                                                        /**< Maximum attribute length; gki_cfg must have a corresponding buffer pool that can hold this length */
        517,                                                        /**< Maxiimum MTU size;gki_cfg must have a corresponding buffer pool that can hold this length */
    },

    /* RFCOMM configuration */
    {
        1,                                                          /**< Maximum number of simultaneous connected remote devices*/
        1                                                           /**< Maximum number of simultaneous RFCOMM ports */
    },

    /* Application managed l2cap protocol configuration */
    {
        0,                                                          /**< Maximum number of application-managed PSMs */
        0,                                                          /**< Maximum number of application-managed l2cap links */
        0,                                                          /**< Maximum number of application-managed BR/EDR channels  */
        0,                                                          /**< Maximum number of application-managed LE PSMs */
        0,                                                          /**< Maximum number of application-managed LE channels */
        0,                                                          /**< Maximum l2cap fixed channels */
    },

    {
        0,
        0,                                                         /**< Maximum simultaneous audio/video links */
    },
    /* AVRC configuration */
    {
        0,                                                          /* MTU for AVRC browsing messages (minimum is 335 bytes, or 0 if AVRC browsing is not required) */
        0                                                           /**< Maximum simultaneous remote control links */
    },

    .max_number_of_buffer_pools = WICED_BT_CFG_NUM_BUF_POOLS,

    .addr_resolution_db_size = 3,

    /* rpa_refresh_timeout > 0 for enabling privacy */
    .rpa_refresh_timeout = 900, /**< random address refreshing rate in seconds */
};

/*****************************************************************************
 * wiced_bt_stack buffer pool configuration
 *
 * Configure buffer pools used by the stack
 *
 * Pools must be ordered in increasing buf_size.
 * If a pool runs out of buffers, the next  pool will be used
 *****************************************************************************/
const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[ WICED_BT_CFG_NUM_BUF_POOLS ] =
{
    /*  { buf_size, buf_count } */
    { 64,       4   },      /* Small Buffer Pool */
    { 360,      8   },      /* Medium Buffer Pool (used for HCI & RFCOMM control messages, min recommended size is 360) */
    { 360,      24  },      /* Large Buffer Pool  (used for HCI ACL messages) */
    { 360,      0   },      /* Extra Large Buffer Pool - Used for avdt media packets and miscellaneous (if not needed, set buf_count to 0) */
};
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
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

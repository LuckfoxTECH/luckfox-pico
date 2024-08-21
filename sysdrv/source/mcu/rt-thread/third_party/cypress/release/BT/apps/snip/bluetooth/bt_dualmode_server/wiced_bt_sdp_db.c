/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * Pre-built SDP server database
 *
 * (Available soon: tool for generating SDP database)
 *
 */
#include "wiced_bt_cfg.h"
#include "bt_rfcomm_server.h"
#include "wiced_bt_sdp.h"

/******************************************************
 *                     Macros
*******************************************************/

/******************************************************
 *               Variables Definitions
 *****************************************************/

const uint8_t wiced_bt_sdp_db[] =
{
    SDP_ATTR_SEQUENCE_1(166),                                           // length is the sum of all records

    // first SDP record Serial Port
    SDP_ATTR_SEQUENCE_1(93),                                            // 2 bytes, length of the record
    SDP_ATTR_RECORD_HANDLE(0x10001),                                    // 8 byte
    SDP_ATTR_CLASS_ID(UUID_SERVCLASS_SERIAL_PORT),                      // 8
    SDP_ATTR_RFCOMM_PROTOCOL_DESC_LIST(BT_RFCOMM_SERVER_APP_SCN),       // 17
    SDP_ATTR_BROWSE_LIST,                                               // 8
    SDP_ATTR_LANGUAGE_BASE_ATTR_ID_LIST,                                // 14
    SDP_ATTR_PROFILE_DESC_LIST(0x1101, 0x0100),                         // 13
    SDP_ATTR_SERVICE_NAME(4),                                           // 9
    'P', 'o', 'r', 't',
    SDP_ATTR_SERVICE_DESCRIPTION(11),                                   // 16
    'S', 'e', 'r', 'i', 'a', 'l', ' ', 'P', 'o', 'r', 't',

    // second SDP record Device ID
    SDP_ATTR_SEQUENCE_1(69),                                            // 2 bytes, length of the record
    SDP_ATTR_RECORD_HANDLE(0x10002),                                    // 8 byte
    SDP_ATTR_CLASS_ID(UUID_SERVCLASS_PNP_INFORMATION),                  // 8
    SDP_ATTR_PROTOCOL_DESC_LIST(1),                                     // 18
    SDP_ATTR_UINT2(ATTR_ID_SPECIFICATION_ID, 0x103),                    // 6
    SDP_ATTR_UINT2(ATTR_ID_VENDOR_ID, 0x0f),                            // 6
    SDP_ATTR_UINT2(ATTR_ID_PRODUCT_ID, 0x0144),                         // 6
    SDP_ATTR_UINT2(ATTR_ID_PRODUCT_VERSION, 0x0001),                    // 6
    SDP_ATTR_BOOLEAN(ATTR_ID_PRIMARY_RECORD, 0x01),                     // 5
    SDP_ATTR_UINT2(ATTR_ID_VENDOR_ID_SOURCE, DI_VENDOR_ID_SOURCE_BTSIG) // 6
};

const uint16_t wiced_bt_sdp_db_size = (sizeof(wiced_bt_sdp_db));

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

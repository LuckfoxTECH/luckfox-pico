/**
 * @file tuya_cloud_wifi_defs.h
 * @brief Basic definitions of TUYA cloud
 * @version 0.1
 * @date 2016-04-25
 *
 * @copyright Copyright 2016-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_CLOUD_BASE_DEFS_H
#define TUYA_CLOUD_BASE_DEFS_H

#include "tuya_iot_config.h"

#if (defined(WIFI_GW) && (WIFI_GW == 0)) ||                                                        \
    (defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1))
#include "tuya_hal_wired.h"
#endif
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* offline in LAN.  user wired callback <tuya_hal_wired_station_conn> return <false> */
#define GB_STAT_LAN_UNCONN 0

/* online in LAN, offline in WAN.
   user wired callback <tuya_hal_wired_station_conn> return <true> but mqtt is offline
*/
#define GB_STAT_LAN_CONN 1

/* online in WAN.
   user wired callback <tuya_hal_wired_station_conn> return <true> and mqtt is online
*/
#define GB_STAT_CLOUD_CONN 2

/* GPRS: SIM is not ready. */
#define GB_STAT_GPRS_SIM_DISC 3

/* GPRS: search network. */
#define GB_STAT_GPRS_SEARCH_NETWORK 4

/* GPRS: regist network. */
#define GB_STAT_GPRS_REG_NETWORK 5

/* GPRS: got ip address. */
#define GB_STAT_GPRS_GOT_IP 6

/**
 * @brief Definition of product info(wired)
 */
typedef struct {
	CHAR_T *uuid;     // strlen(uuid) <= 16,must not be null
	CHAR_T *auth_key; // strlen(auth_key) <= 32,must not be null
} GW_PROD_INFO_S;

#if (defined(WIFI_GW) && (WIFI_GW == 0)) ||                                                        \
    (defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1))
/**
 * @brief Handler when network state error happens(wired)
 *
 * @param[in] stat State code, see GW_BASE_NW_STAT_T
 */
typedef VOID (*GET_NW_STAT_CB)(IN CONST GW_BASE_NW_STAT_T stat);
#endif

#ifdef __cplusplus
}
#endif

#endif

/*
tuya_cloud_base_defs.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef TUYA_CLOUD_BASE_DEFS_H
#define TUYA_CLOUD_BASE_DEFS_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* tuya sdk definition of wired-net status */
typedef BYTE_T GW_BASE_NW_STAT_T;

/* offline in LAN.  user wired callback <hwl_bnw_station_conn> return <false> */
#define GB_STAT_LAN_UNCONN 0

/* online in LAN, offline in WAN.
   user wired callback <hwl_bnw_station_conn> return <true> but mqtt is offline
*/
#define GB_STAT_LAN_CONN 1

/* online in WAN.
   user wired callback <hwl_bnw_station_conn> return <true> and mqtt is online
*/
#define GB_STAT_CLOUD_CONN 2

/* tuya-sdk product info (wired version) */
typedef struct {
	CHAR_T *uuid;     // strlen(uuid) <= 16,must not be null
	CHAR_T *auth_key; // strlen(auth_key) <= 32,must not be null
} GW_PROD_INFO_S;

/***********************************************************
 *  callback function: GET_NW_STAT_CB
 *  Desc:    tuya-sdk network check callback (wired version)
 *  Input:   stat: curr network status
 ***********************************************************/
typedef VOID (*GET_NW_STAT_CB)(IN CONST GW_BASE_NW_STAT_T stat);

#ifdef __cplusplus
}
#endif

#endif

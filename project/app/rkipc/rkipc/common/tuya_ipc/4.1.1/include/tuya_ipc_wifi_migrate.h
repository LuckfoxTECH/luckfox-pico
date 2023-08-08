/*
tuya_ipc_wifi_migrate.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/
#ifndef __TUYA_IPC_WIFI_MIGRATE_H__
#define __TUYA_IPC_WIFI_MIGRATE_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"
#include "tuya_ipc_mqt_proccess.h"

/***********************************************************
 *  Function: tuya_ipc_wifi_migrate_init
 *  Input: INT_T disconnect_time
 *  Data: Time required to disconnect wifi link, default 3s.
 *  Output: NA
 *  Brief: initialize wifi migrate, register the mqtt callback event
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_ipc_wifi_migrate_init(INT_T disconnect_time);

/***********************************************************
 *  Function: tuya_ipc_wifi_migrate_uninit
 *  Input: none
 *  Output: NA
 *  Brief: uninit wifi migrate, free used memory
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_ipc_wifi_migrate_uninit();

/***********************************************************
 *  Function: tuya_ipc_wifi_migrate_wifilist
 *  Input: TUYA_IPC_WIFI_MIGRATE_S data
 *  Data param[in]:tid(from app json data)
 *  Output: NA
 *  Brief: report the scanned wifi information
 *  Need: OPERATE_RET hwl_wf_all_ap_scan(OUT AP_IF_S **ap_ary,OUT UINT_T *num)
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_ipc_wifi_migrate_wifilist(TUYA_IPC_WIFI_MIGRATE_S data);

/***********************************************************
 *  Function: tuya_ipc_wifi_migrate_wificonnect
 *  Input: TUYA_IPC_WIFI_MIGRATE_S data
 *  Data param[in]:tid,ssid,passwd,iv(from app json data)
 *  Output: NA
 *  Brief: decrypt wifi password and connect
 *  Need: OPERATE_RET hwl_wf_station_connect(IN CONST CHAR_T *ssid,IN CONST CHAR_T *passwd)
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_ipc_wifi_migrate_wificonnect(TUYA_IPC_WIFI_MIGRATE_S data);

/***********************************************************
 *  Function: tuya_ipc_wifi_migrate_wifiinfo
 *  Input: TUYA_IPC_WIFI_MIGRATE_S data
 *  Data param[in]:tid(from app json data)
 *  Output: NA
 *  Brief: report the current network information of the device
 *  Need: OPERATE_RET hwl_wf_network_mode(OUT WF_NETWORK_MODE_E *mode);
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_ipc_wifi_migrate_wifiinfo(TUYA_IPC_WIFI_MIGRATE_S data);

#ifdef __cplusplus
}
#endif

#endif

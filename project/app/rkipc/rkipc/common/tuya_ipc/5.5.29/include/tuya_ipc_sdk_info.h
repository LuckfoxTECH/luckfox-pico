/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_sdk_info.h
 **********************************************************************************/

#ifndef _TUYA_IPC_SDK_INFO_H
#define _TUYA_IPC_SDK_INFO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	E_IPC_UNREGISTERED = 0,
	E_IPC_REGISTERED = 1,
	E_IPC_ACTIVEATED = 2 // after successful register and active process, device is in this status
} IPC_REGISTER_STATUS;
/**
 * \brief Status for IPC
 * \enum IPC_GW_STATUS_E
 */
typedef enum {
	IPC_GW_UNREGISTERED = 0,
	IPC_GW_REGISTERED,
	IPC_GW_ACTIVED,
	IPC_GW_MQTT_ONLINE,
	IPC_GW_MQTT_OFFLINE,
} IPC_GW_STATUS_E;

/**
 * \fn CHAR *tuya_ipc_get_sdk_info(VOID)
 * \brief return sdk info, sdk version etc.
 * \return CHAR *
 */
CHAR_T *tuya_ipc_get_sdk_info(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_get_register_status(VOID)
 * \brief get register status on tuya cloud
 * \return OPERATE_RET
 */
IPC_REGISTER_STATUS tuya_ipc_get_register_status(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_get_service_time(OUT TIME_T *time_utc, OUT INT *time_zone)
 * \brief get time which sync with tuya server and APP
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_service_time(OUT TIME_T *time_utc, OUT INT_T *time_zone);

/**
 * \fn OPERATE_RET tuya_ipc_check_in_dls(IN TIME_T time_utc, OUT BOOL * pIsDls)
 * \brief check if specified utc time is in daylight saving time夏令时
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_check_in_dls(IN TIME_T time_utc, OUT BOOL_T *pIsDls);

/**
 * \fn OPERATE_RET tuya_ipc_get_local_time(IN TIME_T inTime, OUT struct tm *localTime)
 * \brief transfer utc time to local time with timezone
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_local_time(IN TIME_T inTime, OUT struct tm *localTime);

/**
 * \fn OPERATE_RET tuya_ipc_get_utc_tm(OUT struct tm *time_utc)
 * \brief get utc time in tm format
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_get_utc_tm(OUT struct tm *localTime);
/**
 * \fn OPERATE_RET tuya_ipc_get_utc_time(OUT TIME_T *time_utc)
 * \brief get utc time
 * \return OPERATE_RET
 */

OPERATE_RET tuya_ipc_get_utc_time(OUT TIME_T *time_utc);

/**
 * \fn OPERATE_RET tuya_ipc_get_free_ram(IN ULONG *free)
 * \brief get free memory of system in KB, only standard linux supported
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_free_ram(IN ULONG_T *free);

/**
 * \fn OPERATE_RET tuya_ipc_local_key_get
 * \brief Get signatrue key from ipc sdk.
 * \ make destKeyBuf len >=17;
 * \return return OPRT_OK if get key success
 */

OPERATE_RET tuya_ipc_local_key_get(OUT CHAR_T *destKeybuf, OUT UINT_T *len);

/**
 * \fn OPERATE_RET tuya_ipc_device_id_get
 * \brief get device id
 * \
 * \return return OPRT_OK if get device id success .other is failed;
 */

OPERATE_RET tuya_ipc_device_id_get(IN OUT CHAR_T *devid, IN OUT INT_T *id_len);
/**
 * \fn OPERATE_RET tuya_ipc_low_power_server_get
 * \brief get low power server info
 *
 * \return return OPRT_OK if get ip、port success .other is failed;
 */
OPERATE_RET tuya_ipc_low_power_server_get(OUT UINT_T *ip, OUT UINT_T *port);

/**
 * \fn OPERATE_RET tuya_ipc_low_power_server_get_v2
 * \brief get low power server info
 *
 * \return return OPRT_OK if get domain、domain_len、ip、port success .other is failed;
 */
OPERATE_RET tuya_ipc_low_power_server_get_v2(OUT CHAR_T *domain, IN INT_T domain_len,
                                             OUT UINT_T *ip, OUT UINT_T *port);

/**
 * \fn OPERATE_RET tuya_ipc_get_mqtt_status(VOID)
 * \brief get mqtt connection status, mqtt is used by tuya SDK to commnicate with tuya cloud
 * \return 0:offline 1:online
 */
OPERATE_RET tuya_ipc_get_mqtt_status(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc)
 * \brief set time of tuya SDK
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_service_time(IN TIME_T new_time_utc);

OPERATE_RET tuya_ipc_get_service_time_force(OUT TIME_T *time_utc, OUT INT_T *time_zone);

/**
 * \fn OPERATE_RET tuya_ipc_report_p2p_msg
 * \brief send a msg to enable p2p func
 * \return SUCCESS -- OPERATE_RET , FAIL -- COMM ERROR
 */
OPERATE_RET tuya_ipc_report_p2p_msg();

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_SDK_INFO_H*/

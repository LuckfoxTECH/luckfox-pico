/*
 * tuya_ipc_lowpower_api.h
 *
 *  Created on: 2020年8月13日
 *      Author: kuiba
 */

#ifndef SVC_LOWPOWER_INCLUDE_TUYA_IPC_LOWPOWER_API_H_
#define SVC_LOWPOWER_INCLUDE_TUYA_IPC_LOWPOWER_API_H_

/**
 * \fn  int tuya_ipc_lowpower_server_connect
 * \brief connect tuya low power servivce
 * \return  int 0:success.other:failed
 */

int tuya_ipc_low_power_server_connect(unsigned int serverIp, signed int port, char *pdevId,
                                      signed int idLen, char *pkey, signed int keyLen);
/**
 * \fn  int tuya_ipc_low_power_socket_fd_get
 * \brief get tuya low power keep alive tcp handler
 * \return  int 0:success.other:failed
 */
int tuya_ipc_low_power_socket_fd_get();
/**
 * \fn  int tuya_ipc_lowpower_server_connect
 * \brief get tuya low power wakeup data
 * \return  int 0:success.other:failed
 */
int tuya_ipc_low_power_wakeup_data_get(char *pdata, unsigned int *plen);
/**
 * \fn  int tuya_ipc_lowpower_server_connect
 * \brief get tuya low power heart beat data;
 * \return  int 0:success.other:failed
 */
int tuya_ipc_low_power_heart_beat_get(char *pdata, unsigned int *plen);

#endif /* SVC_LOWPOWER_INCLUDE_TUYA_IPC_LOWPOWER_API_H_ */

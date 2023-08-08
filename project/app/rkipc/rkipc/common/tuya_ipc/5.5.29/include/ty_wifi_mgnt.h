/**
 * @file ty_wifi_mgnt.h
 * @brief wifi interface
 * @version 0.1
 * @date 2021-01-28
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _WIFI_MGNT_H
#define _WIFI_MGNT_H

#include "tuya_cloud_types.h"
#include "tuya_hal_network.h"
#include "tuya_hal_wifi.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIFI_MGNT_GLOBAL
#define _WIFI_MGNT_EXT
#else
#define _WIFI_MGNT_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
#define PROBE_SSID "tuya_smart"

/***********************************************************
*************************variable define********************
***********************************************************/

/**
 * @brief send wifi management packets probe request.
 *
 * @param[in] in_buf data to be sent in tuya vsie
 * @param[in] in_len data length to be sent
 * @param[in] ssid the ssid of the probe request
 * @param[in] source mac address of probe request
 * @param[in] destination mac address of probe request
 *
 * @note send probe request packets.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
_WIFI_MGNT_EXT
OPERATE_RET ty_wf_send_probe_request_mgnt(IN CONST UCHAR_T *in_buf, IN CONST UINT_T in_len,
                                          IN UCHAR_T *ssid, NW_MAC_S *srcmac, NW_MAC_S *dstmac);

/**
 * @brief send wifi management packets probe response.
 *
 * @param[in] in_buf data to be sent in tuya vsie
 * @param[in] in_len data length to be sent
 * @param[in] ssid the ssid of the probe request
 * @param[in] source mac address of probe request
 * @param[in] destination mac address of probe request
 *
 * @note send probe request packets.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
_WIFI_MGNT_EXT
OPERATE_RET ty_wf_send_probe_response_mgnt(IN CONST UCHAR_T *in_buf, IN CONST UINT_T in_len,
                                           IN UCHAR_T *ssid, NW_MAC_S *srcmac, NW_MAC_S *dstmac);

/**
 * @brief enable wifi management packets reception in wifi station mode.
 *
 * @param[in] enbale disable or enable wifi management packets reception
 * @param[in] recv_cb callback function to receive management packets
 *
 * @note called in wifi station mode, register callback function to process wifi management packets.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
_WIFI_MGNT_EXT
OPERATE_RET ty_wf_register_recv_mgnt_callback(BOOL_T enbale, WIFI_REV_MGNT_CB recv_cb);

/**
 * @brief send wifi beacon packets.
 *
 * @param[in] ssid the beacon ssid
 * @param[in] chan the beacon's wifi channel
 * @param[in] src_mac the beacon's bssid
 * @param[in] vsie_data tuya vsie data to be inserted into beacon
 * @param[in] vsie_data_len length of vsie data
 *
 * @note called to send beacon frame.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
_WIFI_MGNT_EXT
OPERATE_RET ty_wf_send_beacon(UCHAR_T *ssid, UCHAR_T chan, BYTE_T *src_mac, BYTE_T *vsie_data,
                              UINT_T vsie_data_len);

/**
 * @brief filter wifi recv mng frame cb
 *
 * @param[in] buf frame buf
 * @param[in] len frame len
 *
 * @note called to filter mng recv.
 *
 * @return TRUE for needed, FLASE for not needed
 */
typedef BOOL_T (*WIFI_REV_MGNT_FILTER)(UCHAR_T *buf, UINT_T len);

/**
 * @brief register wifi recv mng frame cb
 *
 * @param[in] mng_tp mng type
 * @param[in] cb callback
 *
 * @note called to register mng recv.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_wf_recv_mng_frame_reg(WIFI_REV_MGNT_FILTER filter_cb, WIFI_REV_MGNT_CB cb);

/**
 * @brief unregister wifi recv mng frame cb
 *
 * @param[in] cb callback
 *
 * @note called to unregister mng recv.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_wf_recv_mng_frame_unreg(WIFI_REV_MGNT_CB cb);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

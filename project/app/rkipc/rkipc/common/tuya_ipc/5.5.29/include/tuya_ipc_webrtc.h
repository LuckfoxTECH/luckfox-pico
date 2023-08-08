#ifndef __TUYA_IPC_WEBRTC_H__
#define __TUYA_IPC_WEBRTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief webrtc mode
 *
 */
typedef enum TUYA_WEBRTC_MODE_ {
	TUYA_WEBRTC_ECHOSHOW,
	TUYA_WEBRTC_CHROMECAST,
	TUYA_WEBRTC_WEB,
} TUYA_WEBRTC_MODE_E;

/**
 * \fn callback
 * \brief indicate webrtc stream start
 * \param context: user data
 * \      mode: stream mode
 * \      priv_data: reserve
 * \return INT_T 0:success, other: fail
 */
typedef INT_T (*TUYA_WEBRTC_START)(PVOID_T context, TUYA_WEBRTC_MODE_E mode, PVOID_T priv_data);

/**
 * \fn callback
 * \brief indicate webrtc stream stop
 * \param context: user data
 * \      mode: stream mode
 * \      priv_data: reserve
 * \return INT_T 0:success, other: fail
 */
typedef INT_T (*TUYA_WEBRTC_STOP)(PVOID_T context, TUYA_WEBRTC_MODE_E mode, PVOID_T priv_data);

typedef struct {
	PVOID_T pcontext;        /* context, user data */
	TUYA_WEBRTC_START start; /* indicate webrtc stream start */
	TUYA_WEBRTC_STOP stop;   /* indicate webrtc stream stop */
} TUYA_WEBRTC_CALLBACK;

/**
 * \brief webrtc params
 *
 */
typedef struct TUYA_WEBRTC_PARAM_ {
	TUYA_WEBRTC_CALLBACK cbk;
} TUYA_WEBRTC_PARAM_S;

/**
 * \fn OPERATE_RET tuya_ipc_webrtc_init(IN TUYA_WEBRTC_PARAM_S* pparam);
 * \brief initialize tuya webrtc sdk
 * \ param: pparam: webrtc param
 * \return OPERATE_RET 0:success, other: fail
 */
OPERATE_RET tuya_ipc_webrtc_init(IN TUYA_WEBRTC_PARAM_S *pparam);

/**
 * \fn OPERATE_RET tuya_ipc_webrtc_deinit(VOID);
 * \brief uninitialize tuya webrtc sdk
 * \return OPERATE_RET 0:success, other: fail
 */
OPERATE_RET tuya_ipc_webrtc_deinit(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_webrtc_stop(VOID);
 * \brief stop current webrtc services
 * \return OPERATE_RET 0:success, other: fail
 */
OPERATE_RET tuya_ipc_webrtc_stop(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_webrtc_alive_cnt(INT_T * pCnt);
 * \brief cur alive web cnt
 * \return OPERATE_RET 0:success, other: fail
 */
OPERATE_RET tuya_ipc_webrtc_alive_cnt(INT_T *pCnt);

/**
 * \fn OPERATE_RET tuya_ipc_webrtc_stop(VOID);
 * \brief stop current webrtc services
 * \return OPERATE_RET 0:success, other: fail
 */
OPERATE_RET __webRtc_play_pause_set(CHAR_T pause_flag);

/**
 * \fn OPERATE_RET tuya_ipc_webrtc_stop(VOID);
 * \brief stop current webrtc services
 * \return OPERATE_RET 0:success, other: fail
 */
OPERATE_RET __webRtc_play_pause_set(CHAR_T pause_flag);

#ifdef __cplusplus
}
#endif

#endif

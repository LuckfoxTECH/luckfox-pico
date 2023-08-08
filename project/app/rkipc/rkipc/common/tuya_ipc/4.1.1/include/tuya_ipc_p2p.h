/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_p2p.h
 **********************************************************************************/

#ifndef __TUYA_IPC_P2P_H__
#define __TUYA_IPC_P2P_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

/**
 * \brief  p2p callback message enums
 * \struct TRANSFER_EVENT_E
 */
typedef enum {
	TRANS_LIVE_VIDEO_START = 0,
	TRANS_LIVE_VIDEO_STOP,
	TRANS_LIVE_AUDIO_START,
	TRANS_LIVE_AUDIO_STOP,
	TRANS_LIVE_VIDEO_CLARITY_SET,   /* set clarity from APP */
	TRANS_LIVE_VIDEO_CLARITY_QUERY, /* query clarity informations*/
	TRANS_LIVE_LOAD_ADJUST,
	TRANS_PLAYBACK_LOAD_ADJUST,
	TRANS_PLAYBACK_QUERY_MONTH_SIMPLIFY, /* query storage info of month  */
	TRANS_PLAYBACK_QUERY_DAY_TS,         /* query storage info of day */

	TRANS_PLAYBACK_START_TS, /* start playback */
	TRANS_PLAYBACK_PAUSE,    /* pause playback */
	TRANS_PLAYBACK_RESUME,   /* resume playback */
	TRANS_PLAYBACK_MUTE,     /* mute playback */
	TRANS_PLAYBACK_UNMUTE,   /* unmute playback */
	TRANS_PLAYBACK_STOP,     /* stop playback */

	TRANS_SPEAKER_START, /* start APP-to-IPC speak */
	TRANS_SPEAKER_STOP,  /* stop APP-to-IPC speak */
	TRANS_ABILITY_QUERY, /* query the alibity of audion video strraming */

	TRANS_DOWNLOAD_START, /* start to download */
	TRANS_DOWNLOAD_STOP,
	TRANS_DOWNLOAD_PAUSE,
	TRANS_DOWNLOAD_RESUME,
	TRANS_DOWNLOAD_CANCLE,

	TRANS_STREAMING_VIDEO_START = 100,
	TRANS_STREAMING_VIDEO_STOP = 101
} TRANSFER_EVENT_E;

typedef enum {
	TRANSFER_SOURCE_TYPE_P2P = 1,
	TRANSFER_SOURCE_TYPE_WEBRTC = 2,
	TRANSFER_SOURCE_TYPE_STREAMER = 3,
} TRANSFER_SOURCE_TYPE_E;

/**
 * \brief P2P online status
 * \enum TRANSFER_ONLINE_E
 */
typedef enum {
	TY_DEVICE_OFFLINE,
	TY_DEVICE_ONLINE,
} TRANSFER_ONLINE_E;

typedef enum {
	TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_VIDEO = 0x1,   // if support video
	TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_SPEAKER = 0x2, // if support speaker
	TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE_MIC = 0x4,     // is support MIC
} TY_CMD_QUERY_IPC_FIXED_ABILITY_TYPE;

// request, response
typedef struct tagC2CCmdQueryFixedAbility {
	unsigned int channel;
	unsigned int ability_mask; // ability is assigned by bit
} C2C_TRANS_QUERY_FIXED_ABI_REQ, C2C_TRANS_QUERY_FIXED_ABI_RESP;

typedef enum {
	TY_VIDEO_CLARITY_STANDARD = 0,
	TY_VIDEO_CLARITY_HIGH,
} TRANSFER_VIDEO_CLARITY_TYPE_E;

typedef struct {
	TRANSFER_VIDEO_CLARITY_TYPE_E clarity;
	VOID *pReserved;
} C2C_TRANS_LIVE_CLARITY_PARAM_S;

typedef struct tagC2C_TRANS_CTRL_LIVE_VIDEO {
	unsigned int channel;
} C2C_TRANS_CTRL_VIDEO_START, C2C_TRANS_CTRL_VIDEO_STOP;

typedef struct tagC2C_TRANS_CTRL_LIVE_AUDIO {
	unsigned int channel;
} C2C_TRANS_CTRL_AUDIO_START, C2C_TRANS_CTRL_AUDIO_STOP;

typedef struct {
	UINT_T start_timestamp; /* start timestamp in second of playback */
	UINT_T end_timestamp;   /* end timestamp in second of playback */
} PLAYBACK_TIME_S;

typedef struct tagPLAY_BACK_ALARM_FRAGMENT {
	unsigned int type; // not used now
	PLAYBACK_TIME_S time_sect;
} PLAY_BACK_ALARM_FRAGMENT;

typedef struct {
	unsigned int file_count;              // file count of the day
	PLAY_BACK_ALARM_FRAGMENT file_arr[0]; // play back file array
} PLAY_BACK_ALARM_INFO_ARR;

typedef struct {
	unsigned int channel;
	unsigned int year;
	unsigned int month;
	unsigned int day;
} C2C_TRANS_QUERY_PB_DAY_REQ;

typedef struct {
	unsigned int channel;
	unsigned int year;
	unsigned int month;
	unsigned int day;
	PLAY_BACK_ALARM_INFO_ARR *alarm_arr;
} C2C_TRANS_QUERY_PB_DAY_RESP;

// query playback data by month
typedef struct tagC2CCmdQueryPlaybackInfoByMonth {
	unsigned int channel;
	unsigned int year;
	unsigned int month;
	unsigned int day; // list days that have playback data. Use each bit for one day. For example
	                  // day=26496=0110 0111 1000 0000 means day 7/8/9/19/13/14 have playback data.
} C2C_TRANS_QUERY_PB_MONTH_REQ, C2C_TRANS_QUERY_PB_MONTH_RESP;

typedef struct tagC2C_TRANS_CTRL_PB_START {
	unsigned int channel;
	PLAYBACK_TIME_S time_sect;
	UINT_T playTime; /* the actual playback time, in second */
} C2C_TRANS_CTRL_PB_START;

typedef struct tagC2C_TRANS_CTRL_PB_STOP {
	unsigned int channel;
} C2C_TRANS_CTRL_PB_STOP;

typedef struct tagC2C_TRANS_CTRL_PB_PAUSE {
	unsigned int channel;
} C2C_TRANS_CTRL_PB_PAUSE, C2C_TRANS_CTRL_PB_RESUME;

typedef struct tagC2C_TRANS_CTRL_PB_MUTE {
	unsigned int channel;
} C2C_TRANS_CTRL_PB_MUTE, C2C_TRANS_CTRL_PB_UNMUTE;

/**
 * \brief network load change callback struct
 * \note NOT supported now
 */
typedef struct {
	INT_T client_index;
	INT_T curr_load_level; /**< 0:best 5:worst */
	INT_T new_load_level;  /**< 0:best 5:worst */

	VOID *pReserved;
} C2C_TRANS_PB_LOAD_PARAM_S;

typedef struct {
	INT_T client_index;
	INT_T curr_load_level; /**< 0:best 5:worst */
	INT_T new_load_level;  /**< 0:best 5:worst */

	VOID *pReserved;
} C2C_TRANS_LIVE_LOAD_PARAM_S;

typedef struct {
	UINT_T start_timestamp; /* download start time, in second */
	UINT_T end_timestamp;   /* download end time, in second */
} DOWNLOAD_TIME_S;

typedef struct tagC2C_TRANS_CTRL_DL_START {
	unsigned int channel;
	PLAYBACK_TIME_S time_sect;
} C2C_TRANS_CTRL_DL_START;

typedef struct tagC2C_TRANS_CTRL_DL_STOP {
	unsigned int channel;
} C2C_TRANS_CTRL_DL_STOP, C2C_TRANS_CTRL_DL_PAUSE, C2C_TRANS_CTRL_DL_RESUME,
    C2C_TRANS_CTRL_DL_CANCLE;

// END

/**
 * \brief audio frame struct for P2P
 * \struct TRANSFER_AUDIO_FRAME_S
 */
typedef struct {
	TUYA_CODEC_ID audio_codec;
	TUYA_AUDIO_SAMPLE_E audio_sample;
	TUYA_AUDIO_DATABITS_E audio_databits;
	TUYA_AUDIO_CHANNEL_E audio_channel;
	BYTE_T *p_audio_buf;
	UINT_T buf_len;
	UINT64_T pts;
	UINT64_T timestamp; // in milliseconds
	VOID *p_reserved;
} TRANSFER_AUDIO_FRAME_S;

/**
 * \brief video frame type for P2P
 * \enum TRANSFER_VIDEO_FRAME_TYPE_E
 */
typedef enum {
	TY_VIDEO_FRAME_PBFRAME, /**< P/B frame */
	TY_VIDEO_FRAME_IFRAME,  /**< I frame */
} TRANSFER_VIDEO_FRAME_TYPE_E;

/**
 * \brief video frame struct for P2P
 * \struct TRANSFER_VIDEO_FRAME_S
 */
typedef struct {
	TUYA_CODEC_ID video_codec;
	TRANSFER_VIDEO_FRAME_TYPE_E video_frame_type;
	BYTE_T *p_video_buf;
	UINT_T buf_len;
	UINT64_T pts;
	UINT64_T timestamp; // in milliseconds
	VOID *p_reserved;
} TRANSFER_VIDEO_FRAME_S;

/**
 * \typedef TRANSFER_EVENT_CB
 * \brief P2P transfer events callback function
 * \param[in] event: event type
 * \param[in] args: event info needed
 */
typedef VOID (*TRANSFER_EVENT_CB)(IN CONST TRANSFER_EVENT_E event, IN CONST PVOID_T args);

/**
 * \typedef TRANSFER_REV_AUDIO_CB
 * \brief audio receiving (APP->IPC) callback function
 * \param [in] p_audio_frame: one frame received from APP via P2P
 * \param [in] frame_no: frame sequence number
 */
typedef VOID (*TRANSFER_REV_AUDIO_CB)(IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame,
                                      IN CONST UINT_T frame_no);

/**
 * \typedef TRANSFER_ONLINE_CB
 * \brief callback function when P2P status changes
 */
typedef VOID (*TRANSFER_ONLINE_CB)(IN TRANSFER_ONLINE_E status);

/**
 * \brief quality for live P2P transferring
 * \enum TRANS_LIVE_QUALITY_E
 */
typedef enum {
	TRANS_LIVE_QUALITY_MAX = 0, /**< ex. 640*480, 15fps, 320kbps (or 1280x720, 5fps, 320kbps) */
	TRANS_LIVE_QUALITY_HIGH,    /**< ex. 640*480, 10fps, 256kbps */
	TRANS_LIVE_QUALITY_MIDDLE,  /**< ex. 320*240, 15fps, 256kbps */
	TRANS_LIVE_QUALITY_LOW,     /**< ex. 320*240, 10fps, 128kbps */
	TRANS_LIVE_QUALITY_MIN,     /**< ex. 160*120, 10fps, 64kbps */
} TRANS_LIVE_QUALITY_E;

/**
 * \brief default quality for live P2P transferring
 * \enum TRANS_DEFAULT_QUALITY_E
 */
typedef enum {
	TRANS_DEFAULT_STANDARD = 0, /* ex. 640*480, 15fps */
	TRANS_DEFAULT_HIGH,         /* ex. 1920*1080, 20fps */
} TRANS_DEFAULT_QUALITY_E;

/**
 * \brief P2P settings
 * \struct TUYA_IPC_TRANSFER_VAR_S
 */
typedef struct {
	TRANSFER_ONLINE_CB online_cb;          /* callback function when P2P status changes */
	TRANSFER_REV_AUDIO_CB on_rev_audio_cb; /* audio receiving (APP->IPC) callback function */
	TUYA_CODEC_ID rev_audio_codec;         /* supported audio codec type for data APP->IPC */
	TUYA_AUDIO_SAMPLE_E audio_sample;      /* supported audio sampling for data APP->IPC  */
	TUYA_AUDIO_DATABITS_E audio_databits;  /* supported audio databits for data APP->IPC  */
	TUYA_AUDIO_CHANNEL_E audio_channel;    /* supported audio channel type for data APP->IPC  */
	TRANSFER_EVENT_CB on_event_cb;         /* p2p event callback function */
	TRANS_LIVE_QUALITY_E live_quality;
	INT_T max_client_num;
	IPC_MEDIA_INFO_S AVInfo;
	TRANS_DEFAULT_QUALITY_E
	defLiveMode; /* for multi-streaming ipc, the default quality for live preview */
	VOID *p_reserved;
} TUYA_IPC_TRANSFER_VAR_S;

/**
 * \fn OPERATE_RET tuya_ipc_tranfser_init(IN CONST TUYA_IPC_TRANSFER_VAR_S *p_var)
 * \brief initialize tuya P2P
 * \suggestion do init after ipc has been activated(mqtt online)
 * \param[in] p_var P2P settings
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_tranfser_init(IN CONST TUYA_IPC_TRANSFER_VAR_S *p_var);

/**
 * \fn OPERATE_RET tuya_ipc_tranfser_close()
 * \brief close all P2P conections, live preivew & playback
 * \note
 * \param[in]
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_tranfser_close(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_tranfser_quit(VOID)
 * \brief quit tuya P2P, free resources, memory e.g.
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_tranfser_quit(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_playback_send_video_frame(IN CONST UINT_T client, IN CONST
 * TRANSFER_VIDEO_FRAME_S *p_video_frame) \brief send playback video frame to APP via P2P channel
 * \param[in] client cliend id
 * \param[in] p_video_frame
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_video_frame(IN CONST UINT_T client,
                                               IN CONST TRANSFER_VIDEO_FRAME_S *p_video_frame);

/**
 * \fn OPERATE_RET tuya_ipc_playback_send_audio_frame(IN CONST UINT_T client, IN CONST
 * TRANSFER_AUDIO_FRAME_S *p_audio_frame) \brief send playback audio frame to APP via P2P channel
 * \param[in] client cliend id
 * \param[in] p_audio_frame
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_audio_frame(IN CONST UINT_T client,
                                               IN CONST TRANSFER_AUDIO_FRAME_S *p_audio_frame);

/**
 * \fn OPERATE_RET tuya_ipc_playback_send_fragment_end(IN CONST UINT_T client, IN CONST
 * PLAYBACK_TIME_S * fgmt) \brief notify client(APP) playback fragment is finished, send frag info
 * to app \param[in] client cliend id \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_fragment_end(IN CONST UINT_T client,
                                                IN CONST PLAYBACK_TIME_S *fgmt);

/**
 * \fn OPERATE_RET tuya_ipc_playback_send_finish(IN CONST UINT_T client)
 * \brief notify client(APP) playback data is finished, no more data outgoing
 * \param[in] client cliend id
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_playback_send_finish(IN CONST UINT_T client);

/**
 * \brief P2P connection information
 * \struct TUYA_IPC_TRANSFER_VAR_S
 */
typedef struct {
	UCHAR_T p2p_mode;        /**< 0: P2P mode, 1: Relay mode, 2: LAN mode, 255: Not connected. */
	UCHAR_T local_nat_type;  /**< The local NAT type, 0: Unknown type, 1: Type 1, 2: Type 2, 3: Type
	                            3, 10: TCP only */
	UCHAR_T remote_nat_type; /**< The remote NAT type, 0: Unknown type, 1: Type 1, 2: Type 2, 3:
	                            Type 3, 10: TCP only */
	UCHAR_T relay_type;      /**< 0: Not Relay, 1: UDP Relay, 2: TCP Relay */

	VOID *p_reserved;
} CLIENT_CONNECT_INFO_S;

/**
 * \fn OPERATE_RET tuya_ipc_get_client_conn_info(OUT UINT_T *p_client_num, OUT CLIENT_CONNECT_INFO_S
 * **p_p_conn_info) \brief get P2P connection information \param[out] p_client_num: current
 * connected client number \param[out] p_p_conn_info \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_get_client_conn_info(OUT UINT_T *p_client_num,
                                          OUT CLIENT_CONNECT_INFO_S **p_p_conn_info);

/**
 * \fn OPERATE_RET tuya_ipc_free_client_conn_info(IN CLIENT_CONNECT_INFO_S *p_conn_info)
 * \brief free resource required by tuya_ipc_get_client_conn_info in param p_p_conn_info
 * \param[in] p_conn_info
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_free_client_conn_info(IN CLIENT_CONNECT_INFO_S *p_conn_info);

/***********************************************************
 *  Function: tuya_ipc_4_app_download_data
 *  Note:  download data transfer api
 *  Input: client ,pData data buff; pHead  size for pData
 *  Output: none
 *  Return:
 ***********************************************************/
OPERATE_RET tuya_ipc_4_app_download_data(IN CONST UINT_T client,
                                         IN CONST STORAGE_FRAME_HEAD_S *pHead,
                                         IN CONST CHAR_T *pData);

/***********************************************************
 *  Function: tuya_ipc_4_app_download_status
 *  Note:  download status
 *  Input: client ,percent(0-100),cur only 100 in use
 *  Output: none
 *  Return:
 ***********************************************************/
OPERATE_RET tuya_ipc_4_app_download_status(IN CONST UINT_T client, IN CONST UINT_T percent);

/***********************************************************
 *  Function: tuya_ipc_tranfser_secret_mode
 *  Note:  user set p2p to secret mode
 *  Input: mode TRUE/FALSE secret/no secret
 *  Output: none
 *  Return: OPERATE_RET
 ***********************************************************/
OPERATE_RET tuya_ipc_tranfser_secret_mode(BOOL_T mode);

#ifdef __cplusplus
}
#endif

#endif

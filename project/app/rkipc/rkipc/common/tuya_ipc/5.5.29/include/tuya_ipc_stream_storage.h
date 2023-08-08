/*********************************************************************************
 *Copyright(C),2018, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_stream_storage.h
 **********************************************************************************/

#ifndef __TUYA_IPC_STREAM_STORAGE_H__
#define __TUYA_IPC_STREAM_STORAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "aes_inf.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

#define SS_BASE_PATH_LEN 128
#define DISK_CAPACITY_LOW_THRESH_KB 20 * 1024 // threshold to trigger a low disk space waning to APP
#define DISK_CLEAN_THRESH_KB 100 * 1024       // threshold to trigger auto deleting the oldest data
#define CAPACITY_REPORT_CLEAN_THRESH_KB                                                            \
	100 * 1024 // threshold to trigger a disk-space-change notification to APP

typedef enum {
	SD_STATUS_UNKNOWN = 0,
	SD_STATUS_NORMAL,
	SD_STATUS_ABNORMAL,
	SD_STATUS_LACK_SPACE,
	SD_STATUS_FORMATING,
	SD_STATUS_NOT_EXIST,
	SD_STATUS_MAX
} E_SD_STATUS;

typedef enum {
	E_DELETE_NONE,
	E_DELETE_YEAR,
	E_DELETE_MONTH,
	E_DELETE_DAY,
	E_DELETE_EVENT,
	E_DELETE_INVALID,
	E_DELETE_FS_ERROR
} DELETE_LEVEL_E;

typedef enum {
	E_STORAGE_STOP,
	E_STORAGE_START,
	// below are internal status, should NOT be used for developer
	E_STORAGE_ONGOING,
	E_STORAGE_READY_TO_STOP,
	E_STORAGE_RESTART,
	E_STORAGE_UNWRITEABLE,
	E_STORAGE_INVALID
} STORAGE_STATUS_E;

/**
 * \brief storage settings
 * \struct STREAM_STORAGE_INFO_S
 */
typedef struct {
	CHAR_T base_path[SS_BASE_PATH_LEN]; /* base path of storage, /mnt/sdcard/ e.g. */
	TUYA_CODEC_ID video_codec;          /* codec type of video stream */
	TUYA_CODEC_ID audio_codec;          /* codec type of audio stream */
	VOID *p_reserved;
} STREAM_STORAGE_INFO_S;

/**
 * \brief storage write mode
 * \enum STREAM_STORAGE_WRITE_MODE_E
 */
typedef enum {
	SS_WRITE_MODE_NONE,  /* do NOT write av data */
	SS_WRITE_MODE_EVENT, /* write av data only event happens */
	SS_WRITE_MODE_ALL,   /* write av data all time */
	SS_WRITE_MODE_MAX
} STREAM_STORAGE_WRITE_MODE_E;

typedef enum { SD_MODE_NONE = 0, SD_MODE_EVENT = 1, SD_MODE_ALL_TIME = 2, SD_MODE_MAX } E_SD_MODE;

typedef enum {
	SS_EVENT_TYPE_NONE = 0,
	SS_EVENT_TYPE_MD,
	SS_EVENT_TYPE_FACE,
	SS_EVENT_TYPE_BODY,
	SS_EVENT_TYPE_DECIBEl,
	SS_EVENT_TYPE_MAX
} E_STORAGE_EVENT_TYPE;

// APIs should be implemented by developer
extern E_SD_STATUS tuya_ipc_sd_get_status(VOID);
extern VOID tuya_ipc_sd_get_capacity(UINT_T *p_total, UINT_T *p_used, UINT_T *p_free);
extern VOID tuya_ipc_sd_remount(VOID);
extern CHAR_T *tuya_ipc_get_sd_mount_path(VOID);
extern STREAM_STORAGE_WRITE_MODE_E tuya_ipc_sd_get_mode_config();
/*
    According to different chip platforms, users need to implement the interface of capture.
    size is size of pjbuf
*/
extern VOID tuya_ipc_get_snapshot_cb(char *pjbuf, int *size);

#define TUYA_IPC_SKILL_BASIC (1 << 0)
#define TUYA_IPC_SKILL_DOWNLOAD (1 << 1)
#define TUYA_IPC_SKILL_SPEED_PLAY_0Point5 (1 << 8)
#define TUYA_IPC_SKILL_SPEED_PLAY_2 (1 << 9)
#define TUYA_IPC_SKILL_SPEED_PLAY_4 (1 << 10)  // currently not supported
#define TUYA_IPC_SKILL_SPEED_PLAY_8 (1 << 11)  // currently not supported
#define TUYA_IPC_SKILL_SPEED_PLAY_16 (1 << 12) // currently not supported
#define TUYA_IPC_SKILL_DELETE_BY_DAY (1 << 16)
#define TUYA_IPC_SKILL_EVENT_PIC (1 << 24)
typedef OPERATE_RET (*SS_SD_STATUS_CHANGED_CB)(int status);
typedef struct {
	CHAR_T base_path[SS_BASE_PATH_LEN];
	IPC_MEDIA_INFO_S media_setting;
	UINT_T max_event_per_day;
	AES_HW_CBC_FUNC aes_func;
	SS_SD_STATUS_CHANGED_CB sd_status_changed_cb;
	UINT_T skills; // 0 means all skills, refers to TUYA_IPC_SKILL_BASIC | TUYA_IPC_SKILL_DOWNLOAD
} TUYA_IPC_STORAGE_VAR_S;

/**
 * \fn OPERATE_RET tuya_ipc_ss_init(IN CHAR_T *base_path, IN IPC_MEDIA_INFO_S *media_setting, IN
 UINT_T max_event_per_day, AES_HW_CBC_FUNC * aes_func)
 * \brief initialize tuya ipc stream storage
 * \param[in] base_path: base path for storage device
 * \param[in] media_setting: video/audio settings for tuya sdk to determine storage strategy
 * \param[in] max_event_per_day: max event number per day, this limitation should be based on Soc
 ability, to query and return playback information within acceptable time. tuya sdk will reject
 query message if there are more event than this limit.
 * \param[in] aes_func: encrytion functions
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_init(IN TUYA_IPC_STORAGE_VAR_S *pStorage_var);

/**
 * \fn VOID tuya_ipc_ss_uninit(VOID)
 * uninit tuya stream storage, free used resource, memory e.g.
 * note: should NOT be used more than once
 */
VOID tuya_ipc_ss_uninit(VOID);

/**
 * \fn BOOL_T tuya_ipc_ss_is_inited(VOID)
 * \brief if stream storage is inited
 * \return TRUE/FALSE
 */
BOOL_T tuya_ipc_ss_is_inited(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_ss_set_write_mode(write_mode)
 * \brief set wtote mode of stream storage
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_set_write_mode(IN CONST STREAM_STORAGE_WRITE_MODE_E write_mode);

/**
 * \fn STREAM_STORAGE_WRITE_MODE_E tuya_ipc_ss_get_write_mode
 * \brief get current stream storage mode
 */
STREAM_STORAGE_WRITE_MODE_E tuya_ipc_ss_get_write_mode(VOID);

/**
 * \fn VOID tuya_ipc_ss_start_event(VOID);
 * \brief start an event
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_start_event(VOID);

/**
 * \fn VOID tuya_ipc_ss_stop_event(VOID);
 * \brief stop an event
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_stop_event(VOID);

/**
 * \fn VOID tuya_ipc_ss_start_event_by_type(VOID);
 * \brief start an event by type
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_start_event_by_type(E_STORAGE_EVENT_TYPE type);

/**
 * \fn VOID tuya_ipc_ss_stop_event_by_type(VOID);
 * \brief stop an event by type
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_stop_event_by_type(E_STORAGE_EVENT_TYPE type);

/**
 * \fn VOID tuya_ipc_ss_set_pre_record_time(UINT_T pre_record_second);
 * \brief set pre-record time, invoke this API only if needed, or it is 2 seconds by default.
 * \Should be invoked once after init and before start.
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_set_pre_record_time(UINT_T pre_record_second);

/**
 * \fn VOID tuya_ipc_ss_set_max_event_duration(UINT_T max_event_duration);
 * \brief set max event duration, invoke this API only if needed, or it is 600 seconds by default.
 * \Should be invoked onece after init and before start.
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_set_max_event_duration(UINT_T max_event_duration);

/**
 * \fn VOID tuya_ipc_ss_delete_oldest_event(VOID);
 * \brief delete the oldest event. Event is the smallest unit to delete.
 * \return INT
 */
DELETE_LEVEL_E tuya_ipc_ss_delete_oldest_event(VOID);

/**
 * \fn VOID tuya_ipc_ss_delete_all_files(VOID);
 * \brief delete all video/audio data stored by tuya SDK
 * \return OPERATE_RET
 */
VOID tuya_ipc_ss_delete_all_files(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_ss_get_status
 * \brief get current stream storage status
 * \return OPERATE_RET
 */
STORAGE_STATUS_E tuya_ipc_ss_get_status(VOID);

/**
 * \fn OPERATE_RET tuya_ipc_ss_query_by_month(IN USHORT_T query_year, IN USHORT_T query_month, OUT
 * UINT_T *p_return_days) \brief query the accumulative day count that has stream stored, in
 * specific year and month \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_pb_query_by_month(IN USHORT_T query_year, IN USHORT_T query_month,
                                       OUT UINT_T *p_return_days);

typedef struct {
	UINT_T type;            /* event type */
	UINT_T start_timestamp; /* start time of the the video/audio file, in second */
	UINT_T end_timestamp;   /* end time of the the video/audio file, in second */
} SS_FILE_TIME_TS_S;

/**
 * \struct SS_QUERY_DAY_TS_ARR_S
 */
typedef struct {
	UINT_T file_count;             /* file count within one day */
	SS_FILE_TIME_TS_S file_arr[0]; /* file list */
} SS_QUERY_DAY_TS_ARR_S;

/**
 * \fn OPERATE_RET tuya_ipc_ss_query_by_day( IN USHORT_T query_year, IN USHORT_T query_month, IN
 UCHAR_T query_day, OUT SS_QUERY_DAY_TS_ARR_S **p_p_query_ts_arr)
 * \brief query video/audio informarion in certain year/month/day
 * \param[in] pbIdx: playback/query index, for different client do query in the same time
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_pb_query_by_day(IN UINT_T pbIdx, IN USHORT_T year, IN USHORT_T month,
                                     IN UCHAR_T day, OUT SS_QUERY_DAY_TS_ARR_S **pTsArr);

/**
 * \fn OPERATE_RET tuya_ipc_ss_query_free_ts_arr( IN SS_QUERY_DAY_TS_ARR_S *p_query_ts_arr)
 * \brief free resource of API tuya_ipc_pb_query_by_day output
 * \param[in] p_query_ts_arr
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_pb_query_free_ts_arr(IN SS_QUERY_DAY_TS_ARR_S *p_query_ts_arr);

/**
 * \brief playback event status
 * \struct SS_PB_EVENT_E
 */
typedef enum {
	SS_PB_FINISH = 0, /* end of event, no more data file */
	SS_PB_NEWFILE,    /* new file found, keep sending playback data */
} SS_PB_EVENT_E;

/**
 * \typedef SS_PB_EVENT_CB
 * \brief callback function of playback event
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \param[in] pb_event: playback event id
 * \param[in] args: extra data needed
 */
typedef VOID (*SS_PB_EVENT_CB)(IN UINT_T pb_idx, IN SS_PB_EVENT_E pb_event, IN PVOID_T args);

/**
 * \typedef SS_PB_GET_MEDIA_CB
 * \brief callback function of getting playback media data
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \param[in] p_frame: video/audio frame data
 */
typedef VOID (*SS_PB_GET_MEDIA_CB)(IN UINT_T pb_idx, IN CONST MEDIA_FRAME_S *p_frame);

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_start(OUT UINT_T *p_pb_idx, IN SS_PB_EVENT_CB event_cb, IN
 * SS_PB_GET_MEDIA_CB video_cb, IN SS_PB_GET_MEDIA_CB audio_cb) \brief start a new playback
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \param[in] event_cb: callback function of playback event
 * \param[in] video_cb: callback function of getting playback video data
 * \param[in] audio_cb: callback function of getting playback audio data
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_start(IN UINT_T pb_idx, IN SS_PB_EVENT_CB event_cb,
                                 IN SS_PB_GET_MEDIA_CB video_cb, IN SS_PB_GET_MEDIA_CB audio_cb);

typedef enum {
	SS_PB_05TIMES = 0, // 0.5倍速
	SS_PB_10TIMES = 1, // 正常速度
	SS_PB_20TIMES = 3, // 2.0倍
	SS_PB_SPEED_MAX
} SS_PB_SPEED_E;

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_set_speed(IN UINT_T pb_idx, IN SS_PB_SPEED_E speed)
 * \brief set playback speed
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \param[in] pb_speed: playback speed
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_set_speed(IN UINT_T pb_idx, IN SS_PB_SPEED_E pb_speed);

typedef enum {
	SS_PB_MUTE,
	SS_PB_UN_MUTE,
	SS_PB_PAUSE,
	SS_PB_RESUME,
} SS_PB_STATUS_E;

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_accelerate_send_frame(IN INT time_ms)
 * \brief  playback accelerate send one frame by time_ms faster, default 8ms
 * \param[in] time_ms: time to accelerate for send frame
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_accelerate_send_frame(IN INT_T time_ms);

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_set_status(IN UINT_T pb_idx, IN SS_PB_STATUS_E new_status)
 * \brief set playback status
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \param[in] new_status: status to be set
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_set_status(IN UINT_T pb_idx, IN SS_PB_STATUS_E new_status);

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_stop(IN UINT_T pb_idx)
 * \brief stop an ongoing playback
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_stop(IN UINT_T pb_idx);

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_seek(IN UINT_T pb_idx, IN SS_FILE_TIME_TS_S *pb_file_info, IN
 * UINT_T play_timestamp) \brief seek file to match wanted timestamp \param[in] pb_idx:
 * playback/query index, for different client do query in the same time \param[in] pb_file_info:
 * file to seek \param[in] play_timestamp: time to seek \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_seek(IN UINT_T pb_idx, IN SS_FILE_TIME_TS_S *pb_file_info,
                                IN UINT_T play_timestamp);

/**
 * \fn OPERATE_RET tuya_ipc_ss_pb_stop_all(VOID)
 * \brief stio all ongoing playback
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_pb_stop_all(VOID);

typedef struct {
	UINT_T start_timestamp; /* start time of the the video/audio file, in second */
	UINT_T end_timestamp;   /* end time of the the video/audio file, in second */
} SS_FILE_INFO_S;

typedef struct {
	UINT_T dl_start_time; /* download start time */
	UINT_T dl_end_time;   /* download end time */
	UINT_T file_count;
	SS_FILE_INFO_S *pFileInfoArr; /* pointer of array of files to download */
} SS_DOWNLOAD_FILES_TS_S;

/**
 * \fn OPERATE_RET tuya_ipc_ss_donwload_pre(IN UINT_T pb_idx, IN SS_FILE_TIME_TS_S *pb_file_info)
 * \brief do some pre work, before download
 * \param[in] pb_idx: playback/query index, for different client do query in the same time
 * \param[in] pb_file_info: file to seek
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_donwload_pre(IN UINT_T pb_idx, IN SS_DOWNLOAD_FILES_TS_S *pb_files_info);

typedef enum {
	SS_DL_IDLE,
	SS_DL_START,
	SS_DL_STOP, /* abondoned*/
	SS_DL_PAUSE,
	SS_DL_RESUME,
	SS_DL_CANCLE,
} SS_DOWNLOAD_STATUS_E;

/**
 * \fn OPERATE_RET tuya_ipc_ss_download_set_status(IN UINT_T pb_idx, IN SS_DOWNLOAD_STATUS_E
 * new_status) \brief set download status \param[in] pb_idx: playback/query index, for different
 * client do query in the same time \param[in] new_status: status \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_download_set_status(IN UINT_T pb_idx, IN SS_DOWNLOAD_STATUS_E new_status);

/**
 * \fn OPERATE_RET tuya_ipc_download_image(IN UINT_T pbIdx, IN SS_IMAGE_FILE_S fileInfo,
        OUT int *len , OUT char **pBuf)
 * \brief query video/audio informarion in certain year/month/day
 * \param[in] pbIdx: playback/query index, for different client do query in the same time
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_download_image(IN UINT_T pbIdx, IN SS_FILE_INFO_S fileInfo, OUT INT_T *len,
                                    OUT CHAR_T **pBuf);

/**
 * \fn OPERATE_RET tuya_ipc_ss_delete_video(IN UINT_T pbIdx, IN UINT_T year, IN UINT_T month, IN
 * UINT_T day) \brief delete video/audio/image in certain year/month/day \param[in] pbIdx:
 * playback/query index,  year/month/day \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_delete_video(IN UINT_T pbIdx, IN UINT_T year, IN UINT_T month,
                                     IN UINT_T day);

/**
 * \fn OPERATE_RET tuya_ipc_ss_set_data_mode(IN CONST BOOL_T isAudioEnable)
 * \brief set write data mode  for video calm
 * \param[in] isAudioEnable: TRUE/FALSE
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ss_set_data_mode(IN CONST BOOL_T isAudioEnable);

OPERATE_RET tuya_ipc_ss_pb_accelerate_send_frame_by_id(IN UINT_T pb_idx, IN INT_T time_ms);

#ifdef __cplusplus
}
#endif

#endif

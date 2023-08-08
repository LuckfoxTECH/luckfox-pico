/**
 * @file tuya_ir_control.h
 * @brief
 * @author tuya
 * @version 1.0.0
 * @date 2021-01-13
 * @copyright Copyright(C),2017, 涂鸦科技 www.tuya.comm
 */

#ifndef _TUYA_IR_CONTROL_H
#define _TUYA_IR_CONTROL_H

#include "mem_pool.h"
#include "string.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_iot_com_api.h"
#include "tuya_iot_internal_api.h"
#include "tuya_ir_code.h"
#include "tuya_ir_ws_db.h"
#include "tuya_system_dp.h"
#include "ty_cJSON.h"
#include "uni_base64.h"
#include "uni_log.h"
#include "uni_msg_queue.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TUYA_IR_ATTRIBUTE_REPORT_URL "tuya.device.infrared.attribute.batch.save"
#define TUYA_IR_ATTRIBUTE_REPORT_URL_VER "1.0"

#define TY_CODE_FASTLZ_VER 0x03
#define IR_MQTT_UP_VER 0x03
#define IR_KEY_MAX_NUM_FASTLZ 10
#define IR_MQTT_UP_CYCLE_TIME 2000       // ms
#define IR_MQTT_UP_CYCLE_TYPE TIMER_ONCE // ms
#define IR_MQTT_PUSH_TYPE_BUF_MAX_LEN 128

/**
 * @brief define infrared init type
 */
typedef CHAR_T TY_IR_INIT_TYPE;
#define TY_IR_INIT_NO_TYPE 0
#define TY_IR_INIT_CODE_TYPE 1
#define TY_IR_INIT_CODE_FASTLZ 2
#define TY_IR_INIT_CODE_KK 3

/**
 * @brief define infrared study state type
 */
typedef CHAR_T TY_IR_STUDY_CTL_E;
#define TY_IR_STUDY_START 0
#define TY_IR_STUDY_EXIT 1

/**
 * @brief define IR_FAST_LZ
 */
typedef struct {
	TY_IR_INIT_TYPE ir_init_type;
	TM_MSG_S *ir_type_sync;
} IR_FAST_LZ;

/**
 * @brief define TY_IR_CODE_S
 */
typedef struct {
	USHORT_T *code;
	USHORT_T code_len;
	BYTE_T send_count; // repeat send count, don't need delay
	UINT_T feq;
	USHORT_T delay; // ms
} TY_IR_CODE_S;

typedef VOID (*TY_IR_STUDY_CTL_CB)(TY_IR_STUDY_CTL_E mode);
typedef VOID (*TY_IR_SEND_CB)(TY_IR_CODE_S *ir_code, UCHAR_T code_num_type);

/**
 * @brief define TY_IR_CONTROL_CBS_S
 */
typedef struct {
	TY_IR_STUDY_CTL_CB study_ctl_cb;
	TY_IR_SEND_CB send_cb;
} TY_IR_CONTROL_CBS_S;

/**
 * @brief tuya_ir_control_register
 *
 * @param[in] p_ir_cbs
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_control_register(IN CONST TY_IR_CONTROL_CBS_S *p_ir_cbs);

/**
 * @brief tuya_ir_study_code_report
 *
 * @param[in] p_code
 * @param[in] code_len
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_study_code_report(IN CONST USHORT_T *p_code, IN CONST UINT_T code_len);

/**
 * @brief tuya_ir_study_error_report
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_study_error_report(VOID);

/**
 * @brief tuya_ir_code_free
 *
 * @param[in] ir_code
 * @param[in] code_num
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_code_free(IN TY_IR_CODE_S *ir_code, IN UCHAR_T code_num);

/**
 * @brief tuya_ir_study_stat_report
 *
 * @param[in] mode
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_study_stat_report(IN TY_IR_STUDY_CTL_E mode);

//红外压缩编码新增接口
/*************************************2.0***********************************/
// 1.若采用红外压缩接口(tuya_ir_control_register_v20)初始化，红外下发为压缩格式，由应用层调用tuya_ir_fzcode2timecode解码成时间码再进行红外发送
// 2.若采用红外压缩接口(tuya_ir_control_register_v20)初始化，红外学习上报需先调用tuya_ir_fastlz_compress()将时间码压缩后再调用tuya_ir_study_fz_compress_report上报

/**
 * @brief define TY_IR_GW_ID_S
 */
typedef struct {
	CHAR_T *devid;
	CHAR_T *vir_devid;
} TY_IR_GW_ID_S;

/**
 * @brief define TY_IR_GW_KEY_S
 */
typedef struct {
	UINT_T key_id;
	CHAR_T *key;
} TY_IR_GW_KEY_S;

/**
 * @brief define TY_IR_CODE_HK_S
 */
typedef struct {
	CHAR_T *ir_head;
	CHAR_T *ir_key;
} TY_IR_CODE_HK_S;

/**
 * @brief define TY_IR_CODE_FZ_S
 */
typedef struct {
	CHAR_T *ir_fzcode;
	USHORT_T ir_fzcode_len;
	USHORT_T feq;
	UCHAR_T send_cnt;

} TY_IR_CODE_FZ_S;

/**
 * @brief define TY_IR_CODE_HKORFZ_U
 */
typedef union {
	TY_IR_CODE_HK_S ir_hk_info;
	TY_IR_CODE_FZ_S ir_fz_info;
} TY_IR_CODE_HKORFZ_U;

/**
 * @brief define TY_IR_CODE_V20_S
 */
typedef struct {
	TY_IR_CODE_HKORFZ_U ir_code_info;
	CHAR_T is_fz_code;
	USHORT_T delay; // ms
	CHAR_T relearn;
	TY_IR_GW_KEY_S ir_gw_info_s;
} TY_IR_CODE_V20_S;

typedef VOID (*TY_IR_STUDY_CTL_CB_V20)(TY_IR_STUDY_CTL_E mode, TY_IR_GW_ID_S *gwid_info);
typedef VOID (*TY_IR_SEND_CB_V20)(TY_IR_CODE_V20_S *ir_code, UCHAR_T code_num,
                                  TY_IR_GW_ID_S *gwid_info);

/**
 * @brief define TY_IR_COMPRESS_CONTROL_CBS_S
 */
typedef struct {
	TY_IR_STUDY_CTL_CB_V20 study_ctl_cb;
	TY_IR_SEND_CB_V20 send_cb;
} TY_IR_COMPRESS_CONTROL_CBS_S;

/**
 * @brief: tuya_ir_control_register_v20
 * @desc: infrared function init v2.0
 *
 * @param[in] p_ir_cbs: callback of infrared emission and studt
 *
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_control_register_v20(IN CONST TY_IR_COMPRESS_CONTROL_CBS_S *p_ir_cbs);

/**
 * @brief: tuya_ir_study_fz_compress_report
 * @desc: infrared study code compress and report
 *
 * @param[in] p_code: infrared study code
 * @param[in] code_num: length of study code
 *
 * @return OPERATE_RET
 *
 * @note get the infrared code, compressed by tuya_ir_fastlz_compress(), then used
 * tuya_ir_study_fz_compress_report() to report
 */
OPERATE_RET tuya_ir_study_fz_compress_report(IN CHAR_T *dev_id, IN CONST CHAR_T *p_code,
                                             IN CONST UINT_T code_len);

/**
 * @brief: tuya_ir_lib2timecode
 * @desc: decode the headkey information to the struct of infrared time code
 *
 * @param[in] head: head information
 * @param[in] key: key information
 *
 * @return OPERATE_RET
 *
 * @note timecode need free memory by tuya_ir_code_free()
 */
OPERATE_RET tuya_ir_lib2timecode(IN CHAR_T *head, IN CHAR_T *key, INOUT TY_IR_CODE_S **timecode);

/**
 * @brief: tuya_ir_code_free_v20
 * @desc: free the struct of infrared code
 *
 * @param[in] ir_code: point of infrared code struct
 * @param[in] code_num: total of infrared code struct
 *
 * @return OPERATE_RET
 *
 * @note shulule be use the function free memory after infrared emission
 */
OPERATE_RET tuya_ir_code_free_v20(IN TY_IR_CODE_V20_S *ir_code, IN UCHAR_T code_num);

/**
 * @brief: tuya_ir_sub_attribute_report
 * @desc: Report infrared subdevice properties
 *
 * @param[in] p_type: Infrared coding type
 * @param[in] ir_ver: Infrared version number(default 0x03)
 * @param[in] c_devid: IR child device id
 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_sub_attribute_report(IN TY_IR_INIT_TYPE p_type, IN CHAR_T ir_ver,
                                         IN CONST CHAR_T *c_devid);

/**
 * @brief: tuya_ir_report_ir_attr
 * @desc: Report infrared device attribute
 *
 * @param[in] TY_IR_INIT_TYPE: Infrared encode type

 * @return OPERATE_RET
 */
OPERATE_RET tuya_ir_report_ir_attr(TY_IR_INIT_TYPE type);

/***********************************************RF射频新增相关接口***********************************************/

/**
 * @brief define TY_RF_STUDY_CTL_E
 */
typedef UCHAR_T TY_RF_STUDY_CTL_E;
#define TY_RF_STUDY_START 0
#define TY_RF_STUDY_EXIT 1

/**
 * @brief define TY_RF_SEND_CTL_E
 */
typedef UCHAR_T TY_RF_SEND_CTL_E;
#define TY_RF_SEND_CONFIG_PKG 0
#define TY_RF_SEND_PAIR_PKG 1
#define TY_RF_SEND_CMD_PKG 2
#define TY_RF_SEND_STUDYCODE_PKG 3

typedef VOID (*TY_RF_STUDY_STATE_CB)(TY_RF_STUDY_CTL_E mode);
typedef VOID (*TY_RF_SEND_CB)(UCHAR_T *rf_data, UCHAR_T rf_data_len, TY_RF_SEND_CTL_E type);

/**
 * @brief define TY_RF_CONTROL_CBS_S
 */
typedef struct {
	TY_RF_STUDY_STATE_CB rf_study_state_cb; // A callback is sent in the RF learning state
	TY_RF_SEND_CB rf_lib_send_cb;           // A callback is sent in the RF data
} TY_RF_CONTROL_CBS_S;

/**
 * @brief: tuya_rf_control_register
 * @desc: rf function init
 * @param[in]: p_ir_cbs: point of rf cb struct
 * @return: OPERATE_RET
 * @note :none
 */
OPERATE_RET tuya_rf_control_register(IN CONST TY_RF_CONTROL_CBS_S *p_ir_cbs);

/**
 * @brief: tuya_rf_study_code_report
 * @desc: Report RF learning data
 * @param[in]: p_code: point of rf learn data
 * @param[in]: code_len: Radio frequency learning data length (unit bytes)
 * @param[in]: max_len: point of rf cb struct
 * @return: OPERATE_RET
 * @note :none
 */
OPERATE_RET tuya_rf_study_code_report(IN CONST UCHAR_T *p_code, IN CONST UINT_T code_len,
                                      IN CONST UINT_T max_len);

/**
 * @brief: tuya_rf_study_stat_report
 * @desc: Report radio learning status
 * @param[in]: mode: Radio learning state
 * @return: OPERATE_RET
 * @note :none
 */
OPERATE_RET tuya_rf_study_stat_report(IN TY_RF_STUDY_CTL_E mode);

#ifdef __cplusplus
}
#endif

#endif

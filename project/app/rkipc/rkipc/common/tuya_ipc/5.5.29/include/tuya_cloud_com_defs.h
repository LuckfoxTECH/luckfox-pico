/**
 * @file tuya_cloud_wifi_defs.h
 * @brief Common definitions of TUYA cloud
 * @version 0.1
 * @date 2016-04-25
 *
 * @copyright Copyright 2016-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef TUYA_CLOUD_COM_DEFS_H
#define TUYA_CLOUD_COM_DEFS_H

#include "tuya_cloud_types.h"
#include "ty_cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

// tuya sdk constants definitions
#define ENCRYPT_KEY_LEN 16 // max string length of storage encrypt
#define GW_UUID_LEN 25     // max string length of UUID
#define AUTH_KEY_LEN 32    // max string length of AUTH_KEY
#define PSK_KEY_LEN 64     // max string length of PSK_KEY
#define PRODUCT_KEY_LEN 16 // max string length of PRODUCT_KEY
#define SW_VER_LEN 10      // max string length of VERSION
#define SW_MD5_LEN 32      // max string length of attatch MD5

#define BT_HID_LEN 19 // max string length of HID
#define BT_MAC_LEN 12 // max string length of MAC

#define TUYA_PSK_LEN 32   // max string length of PSK
#define TUYA_PSK21_LEN 44 // max string length of PSK21
#define TUYA_IMEI_LEN 18  // max string length of IMEI
#define TUYA_SN_LEN 20    // max string length of SN

#define SYS_ENV_LEN 20         // max string length of ENV
#define LOG_SEQ_PATH_LEN 128   // max string length of LOG SEQ PATH
#define GW_ID_LEN 25           // max string length of GW_ID
#define DEV_UUID_LEN 25        // max string length of DEV UUID
#define DEV_ID_LEN 25          // max string length of DEV_ID
#define SCHEMA_ID_LEN 16       // max string length of SCHEMA
#define SCHEMA_VER_LEN 16      // max string length of SCHEMA VERSION
#define BIND_TOKEN_LEN 16      // max string length of BIND_TOKEN
#define TOKEN_LEN 8            // max string length of TOKEN
#define REGION_LEN 2           // max string length of REGIN IN TOKEN
#define REGIST_KEY_LEN 4       // max string length of REGIST_KEY IN TOKEN
#define HTTP_URL_LMT 128       // max string length of URL
#define MQ_URL_LMT 128         // max string length of MQTT_URL
#define TIME_ZONE_LEN 10       // max string length of TIME_ZONE
#define SUMMER_TM_ZONE_LEN 256 // max string length of SUMMER_TIME_ZONE
#define COUNTRY_CODE_LEN 8     // max string length of COUNTRY_CODE
#define IPV4_LEN 15            // max string length of IPV4_LEN 255.255.255.255

#define SEC_KEY_LEN 16   // max string length of SECURITY_KEY
#define LOCAL_KEY_LEN 16 // max string length of LOCAL_KEY

#define WXAPPID_LEN 32 // max string length of wechat_appid
#define WXUUID_LEN 32  // max string length of wechat_uuid

#define GRP_ID_LEN 5   // max string length of group id range:1-65535
#define SCENE_ID_LEN 3 // max string length of scene id range:1-255

#define NET_MODE_LEN 32 // max string length of NET MODE

#define LC_GW_SLEEP_HB_LMT_S 12 * 3600 // 12h
// group and scene error code
#define M_SCE_SUCCESS 0     // success
#define M_SCE_ERR_EXCEED 1  // space not enough
#define M_SCE_ERR_TIMEOUT 2 // timeout
#define M_SCE_ERR_PARAM 3   // param not in range
#define M_SCE_WRITE_FILE 4  // failed to write db
#define M_SCE_ERR_OTHER 5   // other failure

// Product Function Specification
typedef BYTE_T GW_ABI;
#define GW_VIRTUAL                                                                                 \
	0x00 // product doesn't have subdevices functions,it only has device function. For example: SOC
	     // or MCU
#define GW_ENTITY_ATTH_DEV 0x01 // Product has subdevices functions and it also has device function.
#define GW_ENTITY 0x02          // Product only has subdevices functions.

// Product Network Connection
typedef BYTE_T GW_NW_STAT_T;
#define GNS_UNVALID 0   // product is offline in LAN and WAN
#define GNS_LAN_VALID 1 // product is online in LAN but offline in WAN
#define GNS_WAN_VALID 2 // product is online in LAN and WAN

// Product Extend State
typedef BYTE_T GW_EXT_STAT_T;
#define EXT_UNAUTH 0         // UNAUTH
#define EXT_PROD_TEST 1      // PROD_TEST
#define EXT_NORMAL_S 2       // NORMAL_S
#define EXT_GW_UPGRD 3       // GW_UPGRD
#define EXT_DEV_UPGRD 4      // DEV_UPGRD
#define EXT_DEV_ADD 5        // DEV_ADD
#define EXT_REG_FAIL 6       // REG_FAIL
#define EXT_NET_FAIL 7       // NET_FAIL
#define EXT_CONFIG_BACK_UP 8 // CONFIG_BACK_UP
#define EXT_CONFIG_RESTORE 9 // CONFIG_RESTORE

/**
 * @brief Definition of device ota channel
 *
 * @note 0~9 are consistent among all TUYA devices
 * 10~19 are customized by device itself
 */
typedef BYTE_T DEV_TYPE_T;
#define DEV_NM_ATH_SNGL 0     // main netlink module
#define DEV_BLE_SNGL 1        // ble
#define DEV_ZB_SNGL 3         // ZigBee
#define DEV_NM_NOT_ATH_SNGL 9 // MCU
#define DEV_ATTACH_MOD_1 10   // attach 1
#define DEV_ATTACH_MOD_2 11   // attach 2
#define DEV_ATTACH_MOD_3 12   // attach 3
#define DEV_ATTACH_MOD_4 13   // attach 4
#define DEV_ATTACH_MOD_5 14   // attach 5
#define DEV_ATTACH_MOD_6 15   // attach 6
#define DEV_ATTACH_MOD_7 16   // attach 7
#define DEV_ATTACH_MOD_8 17   // attach 8
#define DEV_ATTACH_MOD_9 18   // attach 9
#define DEV_ATTACH_MOD_10 19  // attach 10

#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1)
typedef BYTE_T IOT_GW_NET_TYPE_T;
#define IOT_GW_NET_WIRED 0      // only support wried
#define IOT_GW_NET_WIFI 1       // only support wifi
#define IOT_GW_NET_WIRED_WIFI 2 // support wired and wifi

typedef BYTE_T CUR_NET_STATUS_T;
#define NET_WIRED 0 // wired
#define NET_WIFI 1  // wifi
#endif

// sub-device detail type
typedef UINT_T USER_DEV_DTL_DEF_T; // user detial type define

/**
 * @brief Definition of all attache types
 */
typedef BYTE_T GW_PERMIT_DEV_TP_T;
#define GP_DEV_DEF 0xFF                 // default device type
#define GP_DEV_ZB DEV_ZB_SNGL           // zigbee
#define GP_DEV_BLE DEV_BLE_SNGL         // ble
#define GP_DEV_MCU DEV_NM_NOT_ATH_SNGL  // mcu
#define GP_DEV_ATH_1 DEV_ATTACH_MOD_1   // attach 1
#define GP_DEV_ATH_2 DEV_ATTACH_MOD_2   // attach 2
#define GP_DEV_ATH_3 DEV_ATTACH_MOD_3   // attach 3
#define GP_DEV_ATH_4 DEV_ATTACH_MOD_4   // attach 4
#define GP_DEV_ATH_5 DEV_ATTACH_MOD_5   // attach 5
#define GP_DEV_ATH_6 DEV_ATTACH_MOD_6   // attach 6
#define GP_DEV_ATH_7 DEV_ATTACH_MOD_7   // attach 7
#define GP_DEV_ATH_8 DEV_ATTACH_MOD_8   // attach 8
#define GP_DEV_ATH_9 DEV_ATTACH_MOD_9   // attach 9
#define GP_DEV_ATH_10 DEV_ATTACH_MOD_10 // attach 10

#define GP_DEV_SUPPORT_MAX GP_DEV_ATH_10 // max attach id

/**
 * @brief Definition of TUYA DevOS init param
 */
typedef struct {
	/** kv init or not */
	BOOL_T init_db;
	/** sys env settings */
	CHAR_T sys_env[SYS_ENV_LEN];
	/** log seq path */
	CHAR_T log_seq_path[LOG_SEQ_PATH_LEN];
} TY_INIT_PARAMS_S;

/**
 * @brief Definition of attach moudule attribute
 */
typedef struct {
	/** attach ota channel */
	GW_PERMIT_DEV_TP_T tp;
	/** attach version, format xx.xx.xx */
	CHAR_T ver[SW_VER_LEN + 1];
	CHAR_T md5[SW_MD5_LEN + 1];
} GW_ATTACH_ATTR_T;

#define CH_NAME_LMT 15        // max string length of operator name
#define CH_CODE_LMT 20        // max string length of operator code
#define CH_SN_LMT 20          // max string length of device sn
#define CH_REPORT_CODE_LMT 20 // max string length of report code
#define CH_MANU_ID_LMT 10     // max string length of manufacture id
#define CH_VERSION_LMT 10     // max string length of sw version
#define CH_ENCRYPT_KEY_LMT 20 // max string length of encrypt key

/**
 * @brief Definition of operator code info
 */
typedef struct {
	/** operator name */
	CHAR_T ch_name[CH_NAME_LMT + 1];
	/** operator code */
	CHAR_T ch_code[CH_CODE_LMT + 1];
	/** device sn */
	CHAR_T ch_sn[CH_SN_LMT + 1];
	/** report code */
	CHAR_T ch_report_code[CH_REPORT_CODE_LMT + 1];
	/** manufacture id */
	CHAR_T ch_manu_id[CH_MANU_ID_LMT + 1];
	/** sw version */
	CHAR_T ch_version[CH_VERSION_LMT + 1];
	/** encrypt key */
	CHAR_T ch_encrype_key[CH_ENCRYPT_KEY_LMT + 1];
} CH_CODE_ST;

/**
 * @brief Definition of device Quaility of Service
 */
typedef struct {
	/** short nodeID */
	USHORT_T sNodeID;
	/** father short nodeID */
	USHORT_T fNodeID;
	/** RSSI */
	CHAR_T rssi;
	/** lqi info */
	UCHAR_T lqi;
	/** father node ID str */
	CHAR_T fid[DEV_ID_LEN + 1];
} DEV_QOS_ST;

#define GW_ATTACH_ATTR_LMT 4

/**
 * @brief  Definition of dp report type
 */
typedef BYTE_T DP_REPT_TYPE_E;
#define T_OBJ_REPT 0       // dp is value,str,enum,bool,bitmap
#define T_RAW_REPT 1       // raw type
#define T_STAT_REPT 2      // stat type
#define T_RE_TRANS_REPT 10 // repeat report

/**
 * @brief  Definition of dp type
 */
typedef BYTE_T DP_TYPE_E;
#define T_OBJ 0  // dp is value,str,enum,bool,bitmap
#define T_RAW 1  // raw type
#define T_FILE 2 // file type

/**
 * @brief  Definition of dp mode
 */
typedef BYTE_T DP_MODE_E;
#define M_RW 0 // cloud/app can read and write
#define M_WR 1 // cloud/app can only write to device
#define M_RO 2 // cloud/app can only read from device

/**
 * @brief  Definition of dp property type
 */
typedef BYTE_T DP_PROP_TP_E;
#define PROP_BOOL 0
#define PROP_VALUE 1
#define PROP_STR 2
#define PROP_ENUM 3
#define PROP_BITMAP 4

/**
 * @brief  Definition of map type
 */
typedef BYTE_T UP_MAP_TYPE_E;
#define UP_CLEANER_MAP 0
#define UP_CLEANER_PATH 1
#define UP_CLEANER_MAP_PATH 2

/**
 * @brief  Definition of upgrade type
 */
typedef BYTE_T UPGRADE_TYPE_T;
#define UPGRADE_TYPE_NORMAL 0
#define UPGRADE_TYPE_SILENT 1

/**
 * @brief  Definition of dp prop
 */
typedef struct {
	/** min value */
	INT_T min;
	/** max value */
	INT_T max;
	/** step */
	SHORT_T step;
	/** scale */
	USHORT_T scale;
	/** dp value */
	INT_T value;
} DP_PROP_VAL_S;

/**
 * @brief  Definition of dp prop
 */
typedef struct {
	/** enum count */
	INT_T cnt;
	/** enum value */
	CHAR_T **pp_enum;
	/** current value */
	INT_T value;
} DP_PROP_ENUM_S;

/**
 * @brief  Definition of dp prop
 */
typedef struct {
	/** max len */
	INT_T max_len;
	/** cur len */
	INT_T cur_len;
	/** mutex */
	void *dp_str_mutex;
	/** dp value */
	CHAR_T *value;
} DP_PROP_STR_S;

/**
 * @brief  Definition of dp prop
 */
typedef struct {
	/** bool value */
	BOOL_T value;
} DP_BOOL_S;

/**
 * @brief  Definition of dp prop
 */
typedef struct {
	/** max len */
	UINT_T max_len;
	/** value */
	UINT_T value;
} DP_PROP_BITMAP;

/**
 * @brief  Definition of dp prop value
 */
typedef union {
	/** see DP_PROP_VAL_S */
	DP_PROP_VAL_S prop_value;
	/** see DP_PROP_ENUM_S */
	DP_PROP_ENUM_S prop_enum;
	/** see DP_PROP_STR_S */
	DP_PROP_STR_S prop_str;
	/** see DP_BOOL_S */
	DP_BOOL_S prop_bool;
	/** see DP_PROP_BITMAP */
	DP_PROP_BITMAP prop_bitmap;
} DP_PROP_VALUE_U;

/**
 * @brief  Definition of dp trigger type
 */
typedef BYTE_T DP_TRIG_T_E;
#define TRIG_PULSE 0  // upload when value changes
#define TRIG_DIRECT 1 // upload without value change check

/**
 * @brief  Definition of dp positive upload policy
 */
typedef BYTE_T DP_PSV_E;
#define PSV_FALSE 0  /* disabled */
#define PSV_TRUE 1   /* app hasn't query yet */
#define PSV_F_ONCE 2 /* app has queryed, shoulb report */

/**
 * @brief  Definition of dp statistics type
 */
typedef BYTE_T DP_STAT_TP_T;
#define DST_NONE 0  // no need statistics
#define DST_INC 1   // dp statistics increase
#define DST_TOTAL 2 // dp statistics total

/**
 * @brief Definition of dp description
 */
typedef struct {
	/** dp id */
	BYTE_T dp_id;
	/** see DP_MODE_E */
	DP_MODE_E mode;
	/** see DP_PSV_E */
	DP_PSV_E passive;
	/** see DP_TYPE_E */
	DP_TYPE_E type;
	/** see DP_PROP_TP_E */
	DP_PROP_TP_E prop_tp;
	/** see DP_TRIG_T_E */
	DP_TRIG_T_E trig_t;
	/** see DP_STAT_TP_T */
	DP_STAT_TP_T stat;
} DP_DESC_IF_S;

/**
 * @brief tuya sdk dp cmd type
 */
typedef BYTE_T DP_CMD_TYPE_E;
#define DP_CMD_LAN 0               // cmd from LAN
#define DP_CMD_MQ 1                // cmd from MQTT
#define DP_CMD_TIMER 2             // cmd from Local Timer
#define DP_CMD_SCENE_LINKAGE 3     // cmd from scene linkage
#define DP_CMD_RELIABLE_TRANSFER 4 // cmd from reliable transfer
#define DP_CMD_BT 5                // cmd from bt
#define DP_CMD_SCENE_LINKAGE_LAN 6 // cmd from lan scene linkage

/**
 * @brief tuya sdk dp trans type
 */
typedef BYTE_T DP_TRANS_TYPE_T;
#define DTT_SCT_UNC 0   // unicast
#define DTT_SCT_BNC 1   // boardcast
#define DTT_SCT_MNC 2   // multicast
#define DTT_SCT_SCENE 3 // scene

/**
 * @brief tuya sdk dp value union
 */
typedef union {
	INT_T dp_value;   // valid when dp type is value
	UINT_T dp_enum;   // valid when dp type is enum
	CHAR_T *dp_str;   // valid when dp type is str
	BOOL_T dp_bool;   // valid when dp type is bool
	UINT_T dp_bitmap; // valid when dp type is bitmap
} TY_OBJ_DP_VALUE_U;

/**
 * @brief Definition of structured dp
 */
typedef struct {
	/** dp id */
	BYTE_T dpid;
	/** dp type, see DP_PROP_TP_E */
	DP_PROP_TP_E type;
	/** dp value, see TY_OBJ_DP_VALUE_U */
	TY_OBJ_DP_VALUE_U value;
	/** dp happen time. if 0, mean now */
	UINT_T time_stamp;
} TY_OBJ_DP_S;

/**
 * @brief Definition of recved structured dp
 */
typedef struct {
	/** see DP_CMD_TYPE_E */
	DP_CMD_TYPE_E cmd_tp;
	/** see DP_TRANS_TYPE_T */
	DP_TRANS_TYPE_T dtt_tp;
	/** if(NULL == cid) then then the cid represents gwid */
	CHAR_T *cid;
	/** mb id */
	CHAR_T *mb_id;
	/** count of dp */
	UINT_T dps_cnt;
	/** the dp data */
	TY_OBJ_DP_S dps[0];
} TY_RECV_OBJ_DP_S;

/**
 * @brief Definition of recved raw dp
 */
typedef struct {
	/** see DP_CMD_TYPE_E */
	DP_CMD_TYPE_E cmd_tp;
	/** see DP_TRANS_TYPE_T */
	DP_TRANS_TYPE_T dtt_tp;
	/** if(NULL == cid) then then the cid represents gwid */
	CHAR_T *cid;
	/** dp id */
	BYTE_T dpid;
	/** mb id */
	CHAR_T *mb_id;
	/** data len */
	UINT_T len;
	/** the data */
	BYTE_T data[0];
} TY_RECV_RAW_DP_S;

/**
 * @brief Definition of DP query
 */
typedef struct {
	/** if(NULL == cid) then then the cid represents gwid */
	CHAR_T *cid;
	/** dpid cnt if(0 == cnt) then query all object dp */
	UINT_T cnt;
	/** dpid */
	BYTE_T dpid[0];
} TY_DP_QUERY_S;

// add by sunkz
#define DP_REPT_USER_REG_MAX_NUM 5
/**
 * @brief Definition of DP report callbak
 */
typedef struct {
	/** report type, see DP_REPT_TYPE_E */
	DP_REPT_TYPE_E dp_rept_type;
	/** obj:为TY_OBJ_DP_REPT_S，stat:dp为TY_STAT_DP_REPT_S，RAW:TY_RAW_DP_REPT_S */
	VOID_T *dp_data;
	/** Json decoded dp string */
	CHAR_T *dp_data_json;
	/** is query or not */
	BOOL_T is_query;
} DP_REPT_CB_DATA;

/**
 * @brief Handler of dp report pre-process
 *
 * @param[in] dp_rslt Default report result
 * @param[in] dp_data DP data, see DP_REPT_CB_DATA
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*DP_REPT_PRE_HANDLE)(IN CONST DP_REPT_CB_DATA *dp_data);

/**
 * @brief Handler of dp report post-process
 *
 * @param[in] dp_rslt Default report result
 * @param[in] dp_data DP data, see DP_REPT_CB_DATA
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*DP_REPT_POST_HANDLE)(IN CONST OPERATE_RET dp_rslt,
                                           IN CONST DP_REPT_CB_DATA *dp_data);

/**
 * @brief Definition of DP handlers
 */
typedef struct {
	/** handler of pre-precess */
	DP_REPT_PRE_HANDLE dp_rept_pre_cb;
	/** handler of post-precess */
	DP_REPT_POST_HANDLE dp_rept_post_cb;
	/** send dp force or not */
	BOOL_T need_dp_force;
} DP_REPT_HADLE_CB_S;
// add by sunkz

#define FW_URL_LEN 255 // max length of firmware download url
#define FW_MD5_LEN 32  // max length of firmware md5
#define FW_HMAC_LEN 64 // max length of firmware hmac

/**
 * @brief tuya sdk ota firmware info
 */
typedef struct {
	/** firmware type */
	DEV_TYPE_T tp;
	/** upgrade type, see UPGRADE_TYPE_T */
	UPGRADE_TYPE_T type;
	/** firmware download url */
	CHAR_T fw_url[FW_URL_LEN + 1];
	/** firmware version */
	CHAR_T sw_ver[SW_VER_LEN + 1];
	/** firmware size in BYTE */
	UINT_T file_size;
	/** firmware hmac */
	CHAR_T fw_hmac[FW_HMAC_LEN + 1];
	/** firmware md5 */
	CHAR_T fw_md5[FW_MD5_LEN + 1];
	/** is difference ota or not */
	BOOL_T diff_ota;
} FW_UG_S;

/**
 * @brief tuya sdk gateway status info
 */
typedef BYTE_T GW_STATUS_E;
#define GW_RESET 0       // gw reset
#define GW_ACTIVED 1     // gw actived
#define GW_FIRST_START 2 // start tuya-sdk in the first time.
#define GW_NORMAL 3      // tuya-sdk is actived and started
#define GW_BLE_ACTIVED 4 // gw ble actived
/**
 * @brief Handler to process gateway state change
 *
 * @param[in] status Gateway status, see GW_STATUS_E
 */
typedef VOID (*GW_STATUS_CHANGED_CB)(IN CONST GW_STATUS_E status);

/**
 * @brief Handler to process structured DP info
 *
 * @param[in] dp DP query info, see TY_RECV_OBJ_DP_S
 */
typedef VOID (*DEV_OBJ_DP_CMD_CB)(IN CONST TY_RECV_OBJ_DP_S *dp);

/**
 * @brief Handler to process raw DP info
 *
 * @param[in] dp DP query info, see TY_RECV_RAW_DP_S
 */
typedef VOID (*DEV_RAW_DP_CMD_CB)(IN CONST TY_RECV_RAW_DP_S *dp);

/**
 * @brief Handler to process structured DP query info
 *
 * @param[in] dp_qry DP query info, see TY_DP_QUERY_S
 */
typedef VOID (*DEV_DP_QUERY_CB)(IN CONST TY_DP_QUERY_S *dp_qry);

/**
 * @brief Handler to process gateway upgrade inform
 *
 * @param[in] fw Firmware info, see FW_UG_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef int (*GW_UG_INFORM_CB)(IN CONST FW_UG_S *fw);

/* tuya sdk gateway reset type */
typedef enum {
	GW_LOCAL_RESET_FACTORY = 0,
	GW_REMOTE_UNACTIVE,
	GW_LOCAL_UNACTIVE,
	GW_REMOTE_RESET_FACTORY,
	GW_RESET_DATA_FACTORY, // need clear local data when active
} GW_RESET_TYPE_E;

/**
 * @brief Handler to process gateway reset
 *
 * @param[in] type Reset type, see DEV_RESET_TYPE_E
 */
typedef VOID (*GW_RESET_IFM_CB)(GW_RESET_TYPE_E type);

/**
 * @brief Handler to process sub-device upgrade inform
 *
 * @param[in] dev_id Device ID, NULL indicates gateway
 * @param[in] fw Firmware info, see FW_UG_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef int (*DEV_UG_INFORM_CB)(IN CONST CHAR_T *dev_id, IN CONST FW_UG_S *fw);

typedef enum {
	DEV_REMOTE_RESET_FACTORY,
	DEV_RESET_DATA_FACTORY, // need clear local data when bind
} DEV_RESET_TYPE_E;

/**
 * @brief Handler to process reset
 *
 * @param[in] dev_id Device ID, NULL indicates gateway
 * @param[in] type Reset type, see DEV_RESET_TYPE_E
 */
typedef VOID (*DEV_RESET_IFM_CB)(IN CONST CHAR_T *dev_id, IN DEV_RESET_TYPE_E type);

#if defined(TUYA_GW_OPERATOR) && (TUYA_GW_OPERATOR == 1)
/**
 * @brief Handler to get operator code
 *
 * @param[in] is_gw Whether gateway or not
 * @param[in] devid Device ID, NULL indicates gateway
 * @param[out] ch_code Operator code, see CH_CODE_ST
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*OPE_HTTPC_GET_CHCODE_CB)(IN CONST BOOL_T is_gw, IN CONST CHAR_T *devid,
                                               INOUT CH_CODE_ST *ch_code);
#endif

/**
 * @brief Handler to process firmware download content
 *
 * @param[in] fw Firmware info, see FW_UG_S
 * @param[in] total_len Total length of firmware
 * @param[in] offset Current offset
 * @param[in] data File data buffer
 * @param[in] len Length of buffer
 * @param[in] remain_len Length not handled
 * @param[in] pri_data Private data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*GET_FILE_DATA_CB)(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len,
                                        IN CONST UINT_T offset, IN CONST BYTE_T *data,
                                        IN CONST UINT_T len, OUT UINT_T *remain_len,
                                        IN PVOID_T pri_data);

/**
 * @brief Handler to process device active short url
 *
 * @param[out] shorturl Short url to active
 */
typedef VOID (*ACTIVE_SHORTURL_CB)(OUT CONST CHAR_T *shorturl);

/**
 * @brief Handler to process download result
 *
 * @param[in] fw Firmware info, see FW_UG_S
 * @param[in] download_result download result, 0 indicates success
 * @param[in] pri_data Private data
 */
typedef VOID (*UPGRADE_NOTIFY_CB)(IN CONST FW_UG_S *fw, IN CONST INT_T download_result,
                                  IN PVOID_T pri_data);

/**
 * @brief Definition of gateway callback funtions
 */
typedef struct {
	/** status update */
	GW_STATUS_CHANGED_CB gw_status_cb;
	/** gateway upgrade */
	GW_UG_INFORM_CB gw_ug_cb;
	/** gateway reset */
	GW_RESET_IFM_CB gw_reset_cb;
	/** structured DP info */
	DEV_OBJ_DP_CMD_CB dev_obj_dp_cb;
	/** raw DP info */
	DEV_RAW_DP_CMD_CB dev_raw_dp_cb;
	/** DP query */
	DEV_DP_QUERY_CB dev_dp_query_cb;
	/** sub-device upgrade */
	DEV_UG_INFORM_CB dev_ug_cb;
	/** sub-device reset */
	DEV_RESET_IFM_CB dev_reset_cb;
#if defined(TUYA_GW_OPERATOR) && (TUYA_GW_OPERATOR == 1)
	/** opoerator code */
	OPE_HTTPC_GET_CHCODE_CB ope_get_chcode_cb;
#endif

#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
	/** active short url */
	ACTIVE_SHORTURL_CB active_shorturl;
#endif
	/** gateway upgrade pre-condition */
	GW_UG_INFORM_CB pre_gw_ug_cb;
	/** sub-device upgrade pre-condition */
	DEV_UG_INFORM_CB pre_dev_ug_cb;
} TY_IOT_CBS_S;

#if defined(ENABLE_CLOUD_STREAM) && (ENABLE_CLOUD_STREAM == 1)
typedef OPERATE_RET (*GW_CLOUD_STREAM_MQTT_CB)(IN struct ty_cJSON *root_json);
#endif

/* tuya-sdk gateway subdevices management callback functions */

// mqtt media data interface
typedef BYTE_T TRAN_STEP_T;
#define TS_START 0
#define TS_TRANSFER 1
#define TS_END 2

typedef BYTE_T CLEAR_TYPE_T;
#define CLEAR_CONTINUE_ADD 0
#define CLEAR_REMOVE_MAP 1

typedef BYTE_T DATA_TYPE_T;
#define DATA_INCREMENT_MAP 0
#define DATA_COMPLETE_MAP 1

#pragma pack(1)
typedef struct {
	USHORT_T id;
	UINT_T posix;
	TRAN_STEP_T step;
	UINT_T offset;
	USHORT_T len;
	BYTE_T data[0];
} FLOW_BODY_ST;

typedef struct {
	USHORT_T id;
	BYTE_T map_id;
	BYTE_T clear_type;
	BYTE_T data_type;
	UINT_T posix;
	UINT_T offset;
	USHORT_T len;
	BYTE_T data[0];
} FLOW_BODY_V2_ST;

#pragma pack()

/**
 * @brief Definition of region info
 */
typedef struct {
	/** region info */
	CHAR_T region[REGION_LEN + 1];
	/** timezone info */
	CHAR_T time_zone[TIME_ZONE_LEN + 1];
} TY_IOT_REGION_INFO_S;

/**
 * Definition of customer config type
 */
typedef enum {
	/** device cfg */
	CFG_DEVICE = 1,
	/** product cfg */
	CFG_PRODUCT = 2,
	CFG_INVALID,
} TY_CUSTOM_CFG_E;

/**
 * @brief Definition of location info
 */
typedef struct {
	/** country code */
	CHAR_T country_code[COUNTRY_CODE_LEN + 1];
	/** ip addr. */
	CHAR_T ip[IPV4_LEN + 1];
} TY_LOCATION_INFO_S;

/**
 * @brief Handler for log upload
 *
 * @param path Log upload path
 * @param len Length of path
 */
typedef VOID (*GW_APP_LOG_PATH_CB)(OUT CHAR_T *path, IN CONST INT_T len);

/**
 * @brief Definition of IoT callbacks used by APP
 */
typedef struct {
	/** path for log upload */
	GW_APP_LOG_PATH_CB gw_app_log_path_cb;
} TY_IOT_APP_CBS_S;

#ifdef __cplusplus
}
#endif

#endif

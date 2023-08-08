/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_sdk_init.h
 **********************************************************************************/

#ifndef _TUYA_IPC_SDK_INIT_H
#define _TUYA_IPC_SDK_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_iot_config.h"
#if defined(WIFI_GW) && (WIFI_GW == 1)
#include "tuya_cloud_wifi_defs.h"
#endif
#include "cJSON.h"
#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include <stdio.h>
#include <time.h>

//#include "tuya_ipc_sdk_com_defs.h"
#include "tuya_ipc_upgrade.h"

#define IPC_STORAGE_PATH_LEN 64
#define IPC_PRODUCT_KEY_LEN 16
#define IPC_UUID_LEN 25
#define IPC_AUTH_KEY_LEN 32
#define IPC_SW_VER_LEN 20
#define IPC_SERIAL_NUM_LEN 32
#define IPC_P2P_ID_LEN 25

typedef enum {
	WIFI_INIT_AUTO,
	WIFI_INIT_AP,
	WIFI_INIT_DEBUG,
	WIFI_INIT_NULL,
	WIFI_INIT_CONCURRENT
} WIFI_INIT_MODE_E;
typedef enum { NORMAL_POWER_DEV = 0, LOW_POWER_DEV, DEV_TYPE_MAX } TUYA_IPC_DEV_TYPE_E;

/**
 * \brief IPC环境变量结构体
 * \struct TUYA_IPC_ENV_VAR_S
 */
typedef VOID (*TUYA_RESTART_INFORM_CB)(VOID);
typedef VOID (*TUYA_RST_INFORM_CB)(GW_RESET_TYPE_E from);
#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
typedef VOID (*TUYA_QRCODE_ACTIV_CB)(CHAR_T *shorturl);
#endif
typedef struct {
	CHAR_T storage_path[IPC_STORAGE_PATH_LEN +
	                    1]; /* path to store TUYA database files, readable and writable */
	CHAR_T online_log_path[IPC_STORAGE_PATH_LEN +
	                       1]; /* path to store TUYA online log, readable and writable */

	CHAR_T product_key[IPC_PRODUCT_KEY_LEN + 1]; /* one product key(PID) for one product  */
	CHAR_T
	uuid[IPC_UUID_LEN + 1]; /* one uuid-authkey pair for one device, to register on TUYA coud */
	CHAR_T auth_key[IPC_AUTH_KEY_LEN + 1];
	CHAR_T p2p_id[IPC_P2P_ID_LEN + 1]; /* this param is not used now */

	CHAR_T dev_sw_version[IPC_SW_VER_LEN + 1]; /* software version with format x1.x2.x3.x4.x5.x6,
	                                              five dots maximum, zero dots minimum */
	CHAR_T dev_serial_num[IPC_SERIAL_NUM_LEN + 1]; /* this param is not used in IPC now */

	DEV_OBJ_DP_CMD_CB
	dev_obj_dp_cb; /* dp cmd callback function, which hanles cmd from TUYA cloud and APP */
	DEV_RAW_DP_CMD_CB
	dev_raw_dp_cb; /* raw dp cmd callback function, which hanles cmd from TUYA cloud and APP */
	DEV_DP_QUERY_CB dev_dp_query_cb; /* dp query callback function */

	GW_STATUS_CHANGED_CB status_changed_cb; /* status change callback function for developer, could
	                                           be NULL if not needed */
#if defined(GW_SUPPORT_WIRED_WIFI) && (GW_SUPPORT_WIRED_WIFI == 1)
	GET_WF_NW_STAT_CB wf_status_changed_cb; /* status change callback function for developer, could
	                                           be NULL if not needed */
#endif
	TUYA_IPC_SDK_UPGRADE_S
	upgrade_cb_info; /* OTA callback function, triggered by uprading from APP and TUYA cloud */
	TUYA_RST_INFORM_CB
	gw_rst_cb; /* reset callback fucntion, triggered when user unbind device from a account */
	TUYA_RESTART_INFORM_CB gw_restart_cb; /* restart callback function */
#if defined(QRCODE_ACTIVE_MODE) && (QRCODE_ACTIVE_MODE == 1)
	TUYA_QRCODE_ACTIV_CB qrcode_active_cb; /*qrcode active get short  url callback*/
#endif
	BOOL_T mem_save_mode;         /* woking in low memory mode */
	TUYA_IPC_DEV_TYPE_E dev_type; /*device type ,eg low power device*/

	VOID *p_reserved;
} TUYA_IPC_ENV_VAR_S;

/**
 * \brief direct connect mode, compared to wifi EZ mode, get ssid/psw via QR etc.
 * \enum TUYA_IPC_DIRECT_CONNECT_TYPE_E
 */
typedef enum {
	TUYA_IPC_DIRECT_CONNECT_QRCODE = 0,
	TUYA_IPC_DIRECT_CONNECT_MAX,
} TUYA_IPC_DIRECT_CONNECT_TYPE_E;
// TODO 默认打开
#define TUYA_IPC_SDK_SUPPORT_FFS 1
#if defined(TUYA_IPC_SDK_SUPPORT_FFS) && (TUYA_IPC_SDK_SUPPORT_FFS == 1)
/**
 * \brief ffs info
 * \struct TUYA_IPC_FFS_INFO_S
 */
typedef struct {
	char *ffsPid;                   // ffs amazon ubikey
	unsigned int ffsPidLen;         // ffs pid length
	char *ffsPin;                   // ffs pin
	unsigned int ffsPinLen;         // ffs pin length
	char *ffsDpss;                  // ffs public key
	unsigned int ffsDpssLen;        // ffs public key length
	char *ffsDsn;                   // ffs dsn
	unsigned int ffsDsnLen;         // ffs device serial no lenth
	char *ffsPrivateKey;            // ffs  private key
	unsigned int ffsPrivateKeyLen;  // ffs private key length
	char *ffsCertificate;           // ffs certificate
	unsigned int ffsCertificateLen; // ffs certificate length
} TUYA_IPC_FFS_INFO_S;

#endif

/**
 * \fn OPERATE_RET tuya_ipc_init_sdk(IN CONST TUYA_IPC_ENV_VAR_S *p_var)
 * \brief initialize tuya sdk for embeded devices
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_init_sdk(IN CONST TUYA_IPC_ENV_VAR_S *p_var);

/**
 * \fn OPERATE_RET tuya_ipc_start_sdk(IN CONST TUYA_WIFI_DEV_INFO_S *p_wifi_info)
 * \brief start ipc as WIFI_DEVICE
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_start_sdk(IN CONST WIFI_INIT_MODE_E wifi_mode, IN CONST char *pToken);

#if defined(ENABLE_IPC_GW_BOTH) && (ENABLE_IPC_GW_BOTH == 1)

#include "tuya_gw_init_api.h"

OPERATE_RET tuya_ipc_gw_set_gw_cbs(TY_IOT_GW_CBS_S *p_gw_cbs);

OPERATE_RET tuya_ipc_gw_start_sdk(IN CONST WIFI_INIT_MODE_E wifi_mode, IN CONST char *pToken);
#endif
#if defined(TUYA_IPC_SDK_SUPPORT_FFS) && (TUYA_IPC_SDK_SUPPORT_FFS == 1)
/**
 * \fn OPERATE_RET tuya_ipc_ffs_info_write
 * \brief write ffs certificate
 * \param[in] TUYA_IPC_FFS_INFO_S: ffs certificate info
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_ffs_info_write(TUYA_IPC_FFS_INFO_S *ffs_info);
#endif

/**
 * \fn VOID tuya_ipc_upload_skills
 * \brief 初始化完成后，向服务器上报能力值
 */

VOID tuya_ipc_upload_skills(VOID);

/**
 * \fn VOID tuya_ipc_cascade_enable
 * \brief 级联使能，tuya_ipc_upload_skills前调用
 */
VOID tuya_ipc_cascade_enable();

OPERATE_RET tuya_ipc_set_dns_cache_priority(IN CONST INT_T priority);

/**
 * \fn OPERATE_RET tuya_ipc_direct_connect(IN CONST CHAR *p_str, IN CONST
 * TUYA_IPC_DIRECT_CONNECT_TYPE_E source) \brief use this API to connect to specific wifi and
 * register to tuya cloud, data can be obtain via QR code e.g. \param[in] p_str in format:
 * {"p":"password_of_wifi","s":"ssid_of_wifi","t":"token_to_register"} \param[in] source only
 * TUYA_IPC_DIRECT_CONNECT_QRCODE is supported now \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_direct_connect(IN CONST CHAR_T *p_str,
                                    IN CONST TUYA_IPC_DIRECT_CONNECT_TYPE_E source);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_SDK_INIT_H*/

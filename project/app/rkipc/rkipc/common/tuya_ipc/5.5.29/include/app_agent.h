/**
 * @file app_agent.h
 * @brief Common process - app agent
 * @version 0.1
 * @date 2015-06-18
 *
 * @copyright Copyright 2015-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef _APP_AGENT_H
#define _APP_AGENT_H

#include "tuya_cloud_types.h"
#include "ty_cJSON.h"
#include "uni_network.h"

#ifdef __cplusplus
extern "C" {
#endif

// group test
#define FRM_GRP_OPER_ENGR 0xd0
#define FRM_GRP_CMD 0xd1

typedef enum {
	CFG_UDP_DISCOVERY_FORCE,    // send upd discovery even if clients exceed(BOOL_T)
	CFG_UDP_EXT_CONTENT,        // deprecated(reserved for gw/ipc)
	CFG_UDP_EXT_UPDATE,         // add/update new key/value(ty_cJSON)
	CFG_UDP_EXT_DELETE,         // delete key/value(ty_cJSON)
	CFG_SET_CLT_NUM,            // set clinet number(UINT_T)
	CFG_UDP_DISCOVERY_INTERVAL, // set udp discovery interval(UINT_T, unit:s, default:5)
	CFG_REV_BUF_SIZE,           // receive buffer size(UINT, default:512)
	CFG_DEFAULT_LINKAGE,        // default lan linkage (netmgr_linkage_t)
	CFG_MAX
} Lan_Cfg_e;

/**
 * @brief lan protocol init
 *
 * @param[in] wechat true/false
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_init();
#define lan_pro_cntl_init(wechat) tuya_svc_lan_init()

/**
 * @brief lan protocol exit
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_exit(VOID);
#define lan_pro_cntl_exit() tuya_svc_lan_exit()

/**
 * @brief lan protocol diable
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_disable(VOID);
#define lan_pro_cntl_disable() tuya_svc_lan_disable()

/**
 * @brief lan protocol enable
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_enable(VOID);
#define lan_pro_cntl_enable() tuya_svc_lan_enable()

/**
 * @brief lan dp report
 *
 * @param[in] data data buf
 * @param[in] len buf length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_dp_report(IN CONST VOID *data, IN CONST UINT_T len);
#define lan_dp_sata_report(data, len) tuya_svc_lan_dp_report(data, len)

/**
 * @brief lan dp report callback
 *
 * @param[in] fr_type refer to LAN_PRO_HEAD_APP_S
 * @param[in] ret_code refer to LAN_PRO_HEAD_APP_S
 * @param[in] data refer to LAN_PRO_HEAD_APP_S
 * @param[in] len refer to LAN_PRO_HEAD_APP_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_data_report(IN CONST UINT_T fr_type, IN CONST UINT_T ret_code,
                                     IN CONST BYTE_T *data, IN CONST UINT_T len);
#define lan_data_report_cb(fr_type, ret_code, data, len)                                           \
	tuya_svc_lan_data_report(fr_type, ret_code, data, len)

/**
 * @brief disconnect all sockets
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_disconnect_all(VOID);
#define lan_disconnect_all_sockets() tuya_svc_lan_disconnect_all()

/**
 * @brief get vaild connect count
 *
 * @return vaild count
 */
INT_T tuya_svc_lan_get_valid_connections(VOID);
#define lan_pro_cntl_get_valid_connect_cnt() tuya_svc_lan_get_valid_connections()

/**
 * @brief lan configure
 *
 * @param[in] cfg refer to Lan_Cfg_e
 * @param[in] data buf
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_cfg(IN CONST Lan_Cfg_e cfg, IN CONST VOID *data);
#define lan_pro_cntl_cfg(cfg, data) tuya_svc_lan_cfg(cfg, data)

/**
 * @brief unregister callback
 *
 * @param[in] frame_type refer to LAN_PRO_HEAD_APP_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_unregister_cb(IN UINT_T frame_type);
#define lan_pro_cntl_unregister_cb(frame_type) tuya_svc_lan_unregister_cb(frame_type)

/**
 * @brief judge if lan connect
 *
 * @return TRUE/FALSE
 */
INT_T tuya_svc_lan_get_valid_connections(VOID);
#define is_lan_connected() (tuya_svc_lan_get_valid_connections() != 0)

/**
 * @brief lan cmd extersion, caller will free out
 *
 * @param[in] data data
 * @param[out] out buf
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*lan_cmd_handler_cb)(IN CONST BYTE_T *data, OUT BYTE_T **out);

/**
 * @brief register callback
 *
 * @param[in] frame_type refer to LAN_PRO_HEAD_APP_S
 * @param[in] frame_type refer to lan_cmd_handler_cb
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_register_cb(IN UINT_T frame_type, IN lan_cmd_handler_cb handler);

/**
 * @brief register callback
 *
 * @param[in] frame_type refer to LAN_PRO_HEAD_APP_S
 * @param[in] frame_type refer to lan_cmd_handler_cb
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_pro_cntl_register_cb(UINT_T frame_type, lan_cmd_handler_cb handler);

/**
 * @brief unregister callback
 *
 * @param[in] frame_type refer to LAN_PRO_HEAD_APP_S
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET lan_pro_cntl_unregister_cb(UINT_T frame_type);

/**
 * @brief get lan client number
 *
 * @return client number
 */
UINT_T tuya_svc_lan_get_client_num(VOID);

/**
 * @brief Callback to handle lan protocol data
 *
 * @param[in] root_json Json encoded protocol data
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
typedef OPERATE_RET (*lan_ext_protocol_handler_cb)(IN ty_cJSON *root_json);

/**
 * @brief Register extend lan protocol
 *
 * @param[in] ext_lan_pro Protocol string
 * @param[in] handler Protocol handler
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_ext_proto_reg(CONST CHAR_T *ext_lan_pro,
                                       lan_ext_protocol_handler_cb handler);

/**
 * @brief Unregister extend lan protocol
 *
 * @param[in] ext_lan_pro Protocol string
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_ext_proto_unreg(CONST CHAR_T *ext_lan_pro);

/**
 * @brief uninit extend lan protocol
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_ext_proto_uninit(VOID);

/**
 * @brief extend lan protocol data report
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_ext_proto_data_report(IN CONST VOID *data, IN CONST UINT_T len);

/**
 * @brief register callback
 *
 * @param[in] frame_type refer to LAN_PRO_HEAD_APP_S
 * @param[in] frame_type refer to lan_cmd_handler_cb
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_svc_lan_register_cb(IN UINT_T frame_type, IN lan_cmd_handler_cb handler);
#define lan_pro_cntl_register_cb(frame_type, handler) tuya_svc_lan_register_cb(frame_type, handler)

typedef struct {
	INT_T socket;
	UINT_T frame_num;
	UINT_T frame_type;
	UINT_T frame_len;
} LAN_EXT_CMD_PARAM_S;

OPERATE_RET tuya_svc_lan_pro_cntl_get_ext_cmd_param(LAN_EXT_CMD_PARAM_S *ext_cmd_param);

OPERATE_RET tuya_svc_lan_data_com_send(IN CONST INT_T socket, IN CONST UINT_T fr_num,
                                       IN CONST UINT_T fr_type, IN CONST UINT_T ret_code,
                                       IN CONST BYTE_T *data, IN CONST UINT_T len);

#ifdef __cplusplus
}
#endif
#endif

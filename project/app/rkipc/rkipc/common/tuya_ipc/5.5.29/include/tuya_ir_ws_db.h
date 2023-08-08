/**
 * @file tuya_ir_ws_db.h
 * @brief Infrared type flash record
 * @author wangwei
 * @version 1.0.0
 * @date 2020-11-06
 */

#ifndef TUYA_IR_WS_DB_
#define TUYA_IR_WS_DB_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "ty_cJSON.h"
#include "uni_log.h"

#define IR_TYPE_FLASH_KEY "ir_type_flg"
#define IR_GW_ID "ir_recode_gwid"
#define IR_INIT_MODE "ir_init_mode"

/**
 * @brief ws_db_ir_type_mqtt_push_flg_write
 *
 * @param[in] ir_init_mode
 *
 * @return OPERATE_RET
 */
OPERATE_RET ws_db_ir_type_mqtt_push_flg_write(CHAR_T ir_init_mode);

/**
 * @brief ws_db_ir_type_mqtt_push_flg_read
 *
 * @param[in] p_data
 *
 * @return OPERATE_RET
 */
OPERATE_RET ws_db_ir_type_mqtt_push_flg_read(INOUT CHAR_T **p_data);

/**
 * @brief ws_db_ir_type_mqtt_push_judge
 *
 * @param[in] ir_init_mode
 *
 * @return BOOL_T
 */
BOOL_T ws_db_ir_type_mqtt_push_judge(CHAR_T ir_init_mode);

#ifdef __cplusplus
}
#endif

#endif

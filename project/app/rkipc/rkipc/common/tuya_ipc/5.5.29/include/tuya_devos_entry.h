/**
 * @file tuya_devos_entry.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_DEVOS_ENTRY_H__
#define __TUYA_DEVOS_ENTRY_H__

#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Init param of IoT device
 */
typedef struct {
	/** The key of firmware, assigned by TUYA */
	CHAR_T *firmware_key;
	/** The key of product, assigned by TUYA */
	CHAR_T *product_key;
	/** The version of firmware, format is "x.y.z" */
	CHAR_T *sw_ver;

	/** The count of attach modules */
	UINT_T attr_num;
	/** The attributes of attach modules */
	GW_ATTACH_ATTR_T *attrs;
} tuya_dev_init_t;

/**
 * @brief Device is activated, and begin to launch
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_iot_dev_activated(VOID);

/**
 * @brief Update versions of all attach modules.
 *
 * @param[in] attr_num Count of attach modules
 * @param[in] attrs Attributes of attach modules
 *
 * @note This API is used for attach modules updating in case of dynamic add/remove/update.
 * Info of main netlink module will also be included when upload.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET tuya_iot_dev_update_attachs(UINT_T attr_num, GW_ATTACH_ATTR_T *attrs);

OPERATE_RET tuya_iot_dev_set_attach_attr(GW_PERMIT_DEV_TP_T tp, GW_ATTACH_ATTR_T *attr);
OPERATE_RET tuya_iot_dev_get_attach_attr(GW_PERMIT_DEV_TP_T tp, GW_ATTACH_ATTR_T *attr);

/**
 * @brief Enable reset log upload or not
 *
 * @param[in] upload Enable or not
 */
VOID gw_upload_rst_log_set(IN CONST BOOL_T upload);

/**
 * @brief Enable oem or not
 *
 * @param[in] oem Enable or not
 */
VOID gw_oem_set(IN CONST BOOL_T oem);

/**
 * @brief Whether oem is enabled or not
 *
 * @return TRUE on positive, FALSE on negative
 */
BOOL_T is_gw_oem(VOID);

/**
 * @brief Force to upload all versions
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET gw_update_versions(VOID);

/**
 * @brief Write stride info
 */
VOID gw_ctrl_write_stride(VOID);

/**
 * @brief reset stride info
 */
VOID gw_ctrl_reset_stride(VOID);

/**
 * @brief Update stride info
 */
VOID gw_ctrl_update_stride(VOID);

/**
 * @brief Get stride info
 *
 * @return NULL on faile, others see GW_STRIDE_UPDATE_S
 */
GW_STRIDE_UPDATE_S *gw_get_new_pid_key(VOID);

#ifdef __cplusplus
}
#endif

#endif //__TUYA_DEVOS_ENTRY_H__

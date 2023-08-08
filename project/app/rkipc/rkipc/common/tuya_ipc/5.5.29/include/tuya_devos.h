/**
 * @file tuya_devos.h
 * @brief Common process - SDK object management
 * @version 0.1
 * @date 2019-08-28
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 */

#ifndef __TUYA_DEV_H__
#define __TUYA_DEV_H__

#include "tuya_cloud_com_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Definition of customized active info
 */
typedef struct {
	/** support udf active or not */
	BOOL_T is_udf_active_url;
	/** udf active api name */
	CHAR_T *p_active_api;
	/** udf active api version */
	CHAR_T *p_active_ver;
} ty_devos_udf_active_t;

/**
 * @brief Definition of user defined features(udf)
 */
typedef struct {
	/** ignore dp or not when in upgrade state */
	BOOL_T is_ignore_upgrade;
	/** customized active info */
	ty_devos_udf_active_t udf_active;
} ty_devos_udf_cfg_t;

/**
 * @brief Definitioin of cfg params
 */
typedef struct {
	/** user defined params, see ty_devos_udf_cfg_t */
	ty_devos_udf_cfg_t udf_cfg;
} ty_devos_t;

/**
 * @brief Set ignore dp or not when in upgrade state
 *
 * @param[in] is_ignore Ignore or not
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int devos_set_udf_ignore_upgrade(const BOOL_T is_ignore);

/**
 * @brief Get ignore dp or not when in upgrade state
 *
 * @return TRUE on positive, FALSE on negative
 */
BOOL_T devos_get_udf_ignore_upgrade();

/**
 * @brief Set customized active info
 *
 * @param[in] active_api Customized active api name
 * @param[in] active_ver Customized active api version
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int devos_set_udf_active(CHAR_T *active_api, CHAR_T *active_ver);

/**
 * @brief Get customized active info
 *
 * @param[out] active_api Customized active api name
 * @param[out] active_ver Customized active api version
 *
 * @return TRUE on positive, FALSE on negative
 */
BOOL_T devos_get_udf_active(CHAR_T **active_api, CHAR_T **active_ver);

#ifdef __cplusplus
}
#endif

#endif

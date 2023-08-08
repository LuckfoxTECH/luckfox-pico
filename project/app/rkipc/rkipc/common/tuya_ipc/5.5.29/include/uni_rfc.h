/**
 * @file uni_rfc.h
 * @brief tuya reference data module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */
#ifndef _UNI_RFC_H
#define _UNI_RFC_H

#include "tuya_cloud_types.h"
#include "tuya_hal_mutex.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief the reference data
 *
 */
typedef struct {
	MUTEX_HANDLE mutex;
	BOOL_T malk;
	UINT_T rfc;
	UINT_T data_len;
	VOID *data;
} RFC_DATA_S;

/**
 * @brief create a reference data
 *
 * @param[in] data the data buffer
 * @param[in] data_len the date length
 * @param[in] malk need malloc memory for the data
 * @param[in] cnt the Initial value of the reference
 * @return the reference data address
 */
RFC_DATA_S *cr_rfc_data(IN VOID *data, IN CONST UINT_T data_len, IN CONST BOOL_T malk,
                        IN CONST UINT_T cnt);

/**
 * @brief get the reference data, increase the reference
 *
 * @param[inout] rfc_data the reference data
 * @return VOID
 */
VOID get_rfc_data(INOUT RFC_DATA_S *rfc_data);

/**
 * @brief put the reference data, decrease the reference
 *
 * @param[inout] rfc_data the reference data
 * @return VOID
 *
 * @note the reference data will be released when reference is 0
 */
VOID put_rfc_data(INOUT RFC_DATA_S *rfc_data);

/**
 * @brief delete the reference data, ignore the reference
 *
 * @param[inout] rfc_data the reference data
 * @return VOID
 */
VOID del_rfc_data(INOUT RFC_DATA_S *rfc_data);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

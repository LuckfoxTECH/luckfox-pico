/**
 * @file tuya_hal_memory.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_MEMORY_H__
#define __TUYA_HAL_MEMORY_H__

#include "tuya_cloud_types.h"
#include "tuya_os_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Alloc memory of system
 *
 * @param[in] size: memory size
 *
 * @note This API is used to alloc memory of system.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T *tuya_hal_system_malloc(CONST SIZE_T size);

/**
 * @brief Free memory of system
 *
 * @param[in] ptr: memory point
 *
 * @note This API is used to free memory of system.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tuya_hal_system_free(VOID_T *ptr);

/**
 * @brief tuya_hal_system_calloc 用于分配并清除干净内存
 *
 * @param[in]       nitems      需要分配内存块的数量
 * @param[in]       size        需要分配块的内存大小
 */
VOID_T *tuya_hal_system_calloc(size_t nitems, size_t size);

/**
 * @brief tuya_hal_system_realloc 重新调整内存大小
 *
 * @param[in]       nitems      源内存指针
 * @param[in]       size        调整后的大小
 */
VOID_T *tuya_hal_system_realloc(VOID_T *ptr, size_t size);

/* add begin: by sunkz, interface regist */
VOID_T tuya_os_memory_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // __TUYA_HAL_MEMORY_H__

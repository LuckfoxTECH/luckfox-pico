/**
 * @file tuya_hal_storage.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_STORAGE_H__
#define __TUYA_HAL_STORAGE_H__

#include "tuya_os_adapter.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Read flash
 *
 * @param[in] addr: flash address
 * @param[out] dst: pointer of buffer
 * @param[in] size: size of buffer
 *
 * @note This API is used for reading flash.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_flash_read(const unsigned int addr, unsigned char *dst, const unsigned int size);

/**
 * @brief Write flash
 *
 * @param[in] addr: flash address
 * @param[in] src: pointer of buffer
 * @param[in] size: size of buffer
 *
 * @note This API is used for reading flash.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_flash_write(const unsigned int addr, const unsigned char *src,
                         const unsigned int size);

/**
 * @brief Erase flash
 *
 * @param[in] addr: flash address
 * @param[in] size: size of flash block
 *
 * @note This API is used for erasing flash.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_flash_erase(const unsigned int addr, const unsigned int size);

/**
 * @brief Get description of storage
 *
 * @param void
 *
 * @note This API is used to get description of storage.
 *
 * @return the description of storage
 */
UNI_STORAGE_DESC_S *tuya_hal_storage_get_desc(void);

/**
 * @brief Get UF file description
 *
 * @param void
 *
 * @note This API is used to get UF file description.
 *
 * @return the UF file description
 */
UF_PARTITION_TABLE_S *tuya_hal_uf_get_desc(void);

/**
 * @brief Get legacy swap description
 *
 * @param[out] addr: legacy swap address
 * @param[out] size: size of legacy swap
 *
 * @note This API is used to get legacy swap description.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_legacy_swap_get_desc(unsigned int *addr, unsigned int *size);

/**
 * @brief get rcd flash description
 *
 * @return  pointer to descrtion of rcd flash
 */
PARTITION_TABLE_S *tuya_hal_get_rcd_flash_desc(void);

/**

 * @brief tuya_os_storage_intf_init用于初始化tuya storage组件接口
 *
 * @return int 0=成功，非0=失败
 * @brief Storage interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of storage interface.
 *
 * @return VOID
 */
void tuya_os_storage_intf_init(void);

#ifdef __cplusplus
}
#endif

#endif // __TUYA_HAL_STORAGE_H__

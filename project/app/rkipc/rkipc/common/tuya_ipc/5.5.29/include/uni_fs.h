/**
 * @file uni_fs.h
 * @brief tuya file system module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef __TUYA_UNI_FS_H__
#define __TUYA_UNI_FS_H__

#include "tuya_hal_fs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief create directory, ignore if existed
 *
 * @param[in]       path        path of the directory
 * @return  OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET uni_mkdir(const char *path);

/**
 * @brief create multi-level directory
 *
 * @param[in]       path        path of the directory
 * @return  OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET uni_mkdir_all(const char *path);

/**
 * @brief remove file or directory, ignore if not existed
 *
 * @param[in]       path        the path of the file or directory
 * @return  OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
OPERATE_RET uni_remove(const char *path);

/**
 * @brief check if the file or directory existed
 *
 * @param[in]       path        the path of the file or directory
 * @return  TRUE on existed, others on not existed
 */
BOOL_T uni_is_exist(const char *path);

/**
 * @brief check if the file readable
 *
 * @param[in]       path        the path of the file
 * @return  TRUE on readable, others on un-readable
 */
BOOL_T uni_is_readable(const char *path);

/**
 * @brief check if the file writeable
 *
 * @param[in]       path        the path of the file
 * @return  TRUE on writable, others on un-writable
 */
BOOL_T uni_is_writable(const char *path);

/**
 * @brief check if the file excuteable
 *
 * @param[in]       path        the path of the file
 * @return  TRUE on excutable, others on un-excutable
 */
BOOL_T uni_is_executable(const char *path);

#ifdef __cplusplus
} // extern "C"
#endif /* __cplusplus */

#endif // __TUYA_UNI_FS_H__

/**
 * @file tuya_hal_fs.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_FS_H__
#define __TUYA_HAL_FS_H__

#include "tuya_os_adapter.h"
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Make directory
 *
 * @param[in] path: path of directory
 *
 * @note This API is used for making a directory
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fs_mkdir(CONST CHAR_T *path);

/**
 * @brief Remove directory
 *
 * @param[in] path: path of directory
 *
 * @note This API is used for removing a directory
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fs_remove(CONST CHAR_T *path);

/**
 * @brief Get file mode
 *
 * @param[in] path: path of directory
 * @param[out] mode: bit attibute of directory
 *
 * @note This API is used for getting file mode.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fs_mode(CONST CHAR_T *path, UINT_T *mode);

/**
 * @brief Check whether the file or directory exists
 *
 * @param[in] path: path of directory
 * @param[out] is_exist: the file or directory exists or not
 *
 * @note This API is used to check whether the file or directory exists.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fs_is_exist(CONST CHAR_T *path, BOOL_T *is_exist);

/**
 * @brief File rename
 *
 * @param[in] path_old: old path of directory
 * @param[in] path_new: new path of directory
 *
 * @note This API is used to rename the file.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fs_rename(CONST CHAR_T *path_old, CONST CHAR_T *path_new);

/**
 * @brief Open directory
 *
 * @param[in] path: path of directory
 * @param[out] dir: handle of directory
 *
 * @note This API is used to open a directory
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_dir_open(CONST CHAR_T *path, TUYA_DIR *dir);

/**
 * @brief Close directory
 *
 * @param[in] dir: handle of directory
 *
 * @note This API is used to close a directory
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_dir_close(TUYA_DIR dir);

/**
 * @brief Read directory
 *
 * @param[in] dir: handle of directory
 * @param[out] info: file information
 *
 * @note This API is used to read a directory.
 * Read the file information of the current node, and the internal pointer points to the next node.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_dir_read(TUYA_DIR dir, TUYA_FILEINFO *info);

/**
 * @brief Get the name of the file node
 *
 * @param[in] info: file information
 * @param[out] name: file name
 *
 * @note This API is used to get the name of the file node.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_dir_name(TUYA_FILEINFO info, CONST CHAR_T **name);

/**
 * @brief Check whether the node is a directory
 *
 * @param[in] info: file information
 * @param[out] is_dir: is directory or not
 *
 * @note This API is used to check whether the node is a directory.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_dir_is_directory(TUYA_FILEINFO info, BOOL_T *is_dir);

/**
 * @brief Check whether the node is a normal file
 *
 * @param[in] info: file information
 * @param[out] is_regular: is normal file or not
 *
 * @note This API is used to check whether the node is a normal file.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */

INT_T tuya_hal_dir_is_regular(TUYA_FILEINFO info, BOOL_T *is_regular);

/**
 * @brief Open file
 *
 * @param[in] path: path of file
 * @param[in] mode: file open mode: "r","w"...
 *
 * @note This API is used to open a file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
TUYA_FILE tuya_hal_fopen(CONST CHAR_T *path, CONST CHAR_T *mode);

/**
 * @brief Close file
 *
 * @param[in] file: file handle
 *
 * @note This API is used to close a file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fclose(TUYA_FILE file);

/**
 * @brief Read file
 *
 * @param[in] buf: buffer for reading file
 * @param[in] bytes: buffer size
 * @param[in] file: file handle
 *
 * @note This API is used to read a file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fread(VOID_T *buf, INT_T bytes, TUYA_FILE file);

/**
 * @brief write file
 *
 * @param[in] buf: buffer for writing file
 * @param[in] bytes: buffer size
 * @param[in] file: file handle
 *
 * @note This API is used to write a file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fwrite(VOID_T *buf, INT_T bytes, TUYA_FILE file);

/**
 * @brief write buffer to flash
 *
 * @param[in] file: file handle
 *
 * @note This API is used to write buffer to flash
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fsync(TUYA_FILE file);

/**
 * @brief Read string from file
 *
 * @param[in] buf: buffer for reading file
 * @param[in] len: buffer size
 * @param[in] file: file handle
 *
 * @note This API is used to read string from file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
CHAR_T *tuya_hal_fgets(CHAR_T *buf, INT_T len, TUYA_FILE file);

/**
 * @brief Check wheather to reach the end fo the file
 *
 * @param[in] file: file handle
 *
 * @note This API is used to check wheather to reach the end fo the file
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_feof(TUYA_FILE file);

/**
 * @brief Seek to the offset position of the file
 *
 * @param[in] file: file handle
 * @param[in] offs: offset
 * @param[in] whence: seek start point mode
 *
 * @note This API is used to seek to the offset position of the file.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT_T tuya_hal_fseek(TUYA_FILE file, INT64_T offs, INT_T whence);

/**
 * @brief Get current position of file
 *
 * @param[in] file: file handle
 *
 * @note This API is used to get current position of file.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
INT64_T tuya_hal_ftell(TUYA_FILE file);

/**
 * @brief File system interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of file system interface.
 *
 * @return VOID
 */
VOID_T tuya_os_fs_intf_init(VOID_T);

#ifdef __cplusplus
} // extern "C"
#endif /* __cplusplus */

#endif // __TUYA_HAL_FS_H__

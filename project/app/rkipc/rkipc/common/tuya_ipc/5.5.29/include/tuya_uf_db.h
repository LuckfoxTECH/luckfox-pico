/**
 * @file tuya_uf_db.h
 * @brief tuya unique file database module
 * @date 2019-04-25
 *
 * copyright Copyright (c) {2018-2020} 涂鸦科技 www.tuya.com
 *
 */
#ifndef _TUYA_UF_DB_H_
#define _TUYA_UF_DB_H_
#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief unique file handle
 *
 */
typedef VOID uFILE;

/**
 * @brief unique file mode
 *
 */
typedef unsigned char ufmode_t;
#define UF_SEEK_SET 0 // seek from head of the file
#define UF_SEEK_CUR 1 // seek from current position
#define UF_SEEK_END 2 // seek from end of the file

/**
 * @brief tuya unique file database system initialization
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET uf_db_init(VOID);

/**
 * @brief write user parameter to unique file database
 *
 * @param[in] data data write to user parameter database
 * @param[in] len data length write to user parameter database
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note the user parameter database is a file named "user_param_key", and max size is 4096 byte
 */
OPERATE_RET uf_db_user_param_write(IN CONST BYTE_T *data, IN CONST UINT_T len);

/**
 * @brief read user parameter from unique file database
 *
 * @param[out] buf buffer of the data read from user parameter file database
 * @param[out] len length of the data read from user parameter file database
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note the user parameter database is a file named "user_param_key", and max size is 4096 byte
 */
OPERATE_RET uf_db_user_param_read(OUT BYTE_T **buf, OUT UINT_T *len);

/**
 * @brief unique file system initialization
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note it will call by uf_db_init
 */
OPERATE_RET ufinit(VOID);

/**
 * @brief open a unique file
 *
 * @param[in] filepath the full file path
 * @param[in] mode the open mode
 * @return the handle of the file
 *
 * @note the mode is same as file system open mode, contains "r","r+","a","a+","w","w+",
 * and also support binary mode.
 */
uFILE *ufopen(IN CONST CHAR_T *filepath, IN CONST CHAR_T *mode);

/**
 * @brief close a unique file
 *
 * @param fd the file handle
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ufclose(IN uFILE *fd);

/**
 * @brief delete the unique file
 *
 * @param[in] filepath the full file path
 * @return  OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ufdelete(IN CONST CHAR_T *filepath);

/**
 * @brief write to unique file
 *
 * @param[in] fd the file handle
 * @param[in] buf the buffer write to the unique file
 * @param[in] len the length write to the unique file, in byte
 * @return characters written to the unique file
 */
UINT_T ufwrite(IN uFILE *fd, IN CONST UCHAR_T *buf, IN CONST UINT_T len);

/**
 * @brief read from unique file
 *
 * @param[in] fd the file handle
 * @param[in] buf the buffer used to save the data read from the unique file
 * @param[in] len the length want to read
 * @return the number of byte actually read from the unique file
 *
 */
UINT_T ufread(IN uFILE *fd, IN UCHAR_T *buf, IN CONST UINT_T len);

/**
 * @brief seek to the offset of the unique file
 *
 * @param[in] fd the file handle
 * @param[in] offset the offset want to seek
 * @param[in] uf_mode the seek mode
 * @return the current offset after seek
 */
INT_T ufseek(IN uFILE *fd, IN UINT_T offset, IN CONST ufmode_t uf_mode);

/**
 * @brief get current offset of the unique file
 *
 * @param fd the file handle
 * @return the current offset of the file
 */
INT_T uftell(IN uFILE *fd);

/**
 * @brief check if the current offset reach the end of the file
 *
 * @param[in] fd the file handle
 * @return TRUE on reach the EOF, FLASE on not reach the EOF.
 */
BOOL_T ufeof(IN uFILE *fd);

/**
 * @brief get the size of the unique file
 *
 * @param[in] filepath the full file path
 * @return the size of the file
 */
INT_T ufgetsize(IN CONST CHAR_T *filepath);

/**
 * @brief write a specail format to unique file
 *
 * @param[in] stream the file handle
 * @param[in] pFmt the variable parameters format
 * @param ...
 * @return the counts of written characters
 */
INT_T ufprintf(IN uFILE *stream, IN const char *pFmt, ...);

/**
 * @brief read a special format from unique file
 *
 * @param[in] stream the file handle
 * @param[in] count the counts read from file
 * @param[in] pFmt the variable parameters format
 * @param ...
 * @return the items in the paramter list
 */
INT_T ufscanf(IN uFILE *stream, IN INT_T count, IN PCHAR_T pFmt, ...);

/**
 * @brief check if unique file is exist
 *
 * @param[in] filepath the full file path
 * @return TRUE on existed, FLASE on not existed.
 */
BOOL_T ufexist(IN CONST CHAR_T *filepath);

/**
 * @brief test the permission of the unique file
 *
 * @param[in] filepath the full file path
 * @param[in] mode check the access mode
 * @return INT_T
 *
 * @note the access mode contains "R_OK": read permission,"W_OK": write permission,"X_OK":excute
 * permission,"F_OK": existed, in flash unique file system, only support "F_OK" mode.
 */
INT_T ufaccess(IN CONST CHAR_T *filepath, IN INT_T mode);

/**
 * @brief flush the unique file, write the cache to physical disk
 *
 * @param[in] stream the file handle
 * @return 0 on success, others on failed
 *
 * @note in flash unique file system, always return success because the data was write to flash
 * directly.
 */
INT_T ufflush(IN uFILE *stream);

/**
 * @brief sync the unique file, write the file info to pysical disk
 *
 * @param[in] fd the file handle
 * @return 0 on success, others on failed
 *
 * @note in flash unique file system, always return success because the data was write to flash
 * directly.
 */
INT_T ufsync(IN INT_T fd);

/**
 * @brief get the file description
 *
 * @param[in] stream the file handle
 * @return the file description
 *
 * @note in flash unique file system, always return 0, it not support file description
 */
INT_T ufileno(IN uFILE *stream);

/**
 * @brief read a character from unique file
 *
 * @param[in] stream the file handle
 * @return -1 on failed, others means success
 */
INT_T ufgetc(IN uFILE *stream);

/**
 * @brief read a line from unique file
 *
 * @param[in] s the buffer used to save the string
 * @param[in] n the numbers want to get from the file
 * @param[in] stream the file handle
 * @return NULL on failed, others on success
 *
 * @note stop and return if reach the n or find "\n" character
 */
CHAR_T *ufgets(IN CHAR_T *s, IN INT_T n, IN uFILE *stream);

/**
 * @brief format uf flash
 *
 * @return  OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ufformat(VOID);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

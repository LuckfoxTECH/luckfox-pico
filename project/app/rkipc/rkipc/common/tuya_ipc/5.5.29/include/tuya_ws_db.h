/**
 * @file tuya_ws_db.h
 * @author niezy@tuya.com
 * @brief tuya key-value database module
 *
 * @version 1.0
 * @date 2021-01-09
 *
 * @copyright Copyright (c) 2021 tuya inc
 *
 */

#ifndef _TUYA_WS_DB_H
#define _TUYA_WS_DB_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tuya key-value database property define, used for serialize/deserialize data to json
 * format
 *
 */
typedef BYTE_T VAR_TP_T;
#define VT_CHAR 0   // char, same as int, need 11+6 byte
#define VT_BYTE 1   // byte, same as int, need 11+6 byte
#define VT_SHORT 2  // short, same as int, need 11+6 byte
#define VT_USHORT 3 // unsigned short, same as int, need 11+6 byte
#define VT_INT 4    // int, need 11+6 byte when serialize to json format
#define VT_BOOL 5   // bool, need 6+6 byte when serialize to json format
#define VT_STRING 6 // string, need len+6 byte when serialize to json format
#define VT_RAW 7    // raw, need len/3*4+len%3?4:0+6 (base64 encode) when serialize to json format

/**
 * @brief tuya key-value database property, used for serialize/deserialize data to json format
 *
 */
typedef struct {
	CHAR_T *key;  // property name
	VAR_TP_T vt;  // property type
	VOID *val;    // property value
	USHORT_T len; // property length
} TY_DB_RW_S;

/**
 * @brief tuya key-value database initialization
 *
 * @param[in] fs_storge_path the storage path of the key-value database, only used in linux system
 * @param[in] p_key the encypto key of the key-value database, if p_key==NULL, will generate a
 * random one
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note user don't need to care about this function, it will be called when tuya IoTOS
 * initialization.
 *
 */
OPERATE_RET ws_db_init(IN CONST CHAR_T *fs_storge_path, IN CONST BYTE_T *p_key);

/**
 * @brief tuya key-value database initialization in mf
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note user don't need to care about this function, it will be called when tuya IoTOS
 * initialization.
 *
 */
OPERATE_RET ws_db_init_mf(VOID);

/**
 * @brief tuya key-value database de-initialization
 *
 * @return VOID
 */
VOID ws_db_uninit(VOID);

/**
 * @brief tuya key-value database format
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ws_db_format(VOID);

/**
 * @brief tuya key-value database rebuild to recycle
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET ws_db_rebuild(VOID);

/**
 * @brief tuya key-value database get storage path
 *
 * @note Caller shall free the buffer returned
 *
 * @return Buffer for storage path, NULL on error
 */
CHAR_T *ws_db_get_storage_path(VOID);

/**
 * @brief tuya key-value database write entry
 *
 * @param[in] key key of the entry you want to write
 * @param[in] value value buffer you want to write
 * @param[in] len the numbers of byte you want to write
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_common_write(IN CONST CHAR_T *key, IN CONST BYTE_T *value, IN CONST UINT_T len);

/**
 * @brief tuya key-value database read entry
 *
 * @param[in] key  key of the entry you want to read
 * @param[out] value buffer of the value
 * @param[out] p_len length of the buffer
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note must free the value buffer with wd_common_free_data when you nolonger need the buffer
 */
OPERATE_RET wd_common_read(IN CONST CHAR_T *key, OUT BYTE_T **value, OUT UINT_T *p_len);

/**
 * @brief tuya key-value database fuzzy read entry
 *
 * @param[in] fuzzy_name  key of the entry you want to read
 * @param[in] index index of the value sequnence
 * @param[out] data buffer of the value
 * @param[out] p_len length of the buffer
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note if we have three entry "x1","x2","x3" in database, and read use "x" as the key will got all
 * these 3 entrys and will chose the return entry by index
 *
 * @note must free the value buffer with wd_common_free_data when you nolonger need the buffer
 */
OPERATE_RET wd_common_fuzzy_read(IN CONST CHAR_T *fuzzy_name, INOUT UINT_T *index,
                                 OUT BYTE_T **data, OUT UINT_T *len);

/**
 * @brief free the buffer which allocated by wd_common_read or wd_common_fuzzy_read
 *
 * @param[in] data the buffer got from wd_common_read or wd_common_fuzzy_read
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_common_free_data(IN BYTE_T *data);

/**
 * @brief delete the entry from key-value database
 *
 * @param[in] key key of the entry you want to delete
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_common_delete(IN CONST CHAR_T *key);

/**
 * @brief fuzzy delete the entry from key-value database
 *
 * @param[in] key key of the entry you want to delete
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note delete all entries if the key match the prefix of entry key
 */
OPERATE_RET wd_common_fuzzy_delete(IN CONST CHAR_T *key);

/**
 * @brief check the entry from key-value database
 *
 * @param[in] key key of the entry you want to check
 * @param[OUT] exist TRUE on exist
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_common_exist(IN CONST CHAR_T *key, OUT BOOL_T *exist);

/**
 * @brief tuya protected storage write entry
 *
 * @param[in] key key of the entry you want to write
 * @param[in] value value buffer you want to write
 * @param[in] len the numbers of byte you want to write
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_protected_write(IN CONST CHAR_T *key, IN CONST BYTE_T *value, IN CONST UINT_T len);

/**
 * @brief tuya protected storage read entry
 *
 * @param[in] key  key of the entry you want to read
 * @param[out] value buffer of the value
 * @param[out] p_len length of the buffer
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note must free the value buffer with wd_common_free_data when you nolonger need the buffer
 */
OPERATE_RET wd_protected_read(IN CONST CHAR_T *key, OUT BYTE_T **value, OUT UINT_T *p_len);

/**
 * @brief delete the entry from protected storage
 *
 * @param[in] key key of the entry you want to delete
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_protected_delete(IN CONST CHAR_T *key);

/**
 * @brief write the user parameter to tuya key-value database
 *
 * @param[in] data buffer of the data
 * @param[in] len length of the data
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note the key of user parameter entry is "user_param_key"
 */
OPERATE_RET wd_user_param_write(IN CONST BYTE_T *data, IN CONST UINT_T len);

/**
 * @brief read the user parameter from tuya key-value database
 *
 * @param[out] buf buffer of the data
 * @param[out] len length of the data
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note the key of user parameter entry is "user_param_key"，must free the value buffer with
 * wd_common_free_data when you nolonger need the buffer
 */
OPERATE_RET wd_user_param_read(OUT BYTE_T **buf, OUT UINT_T *len);

/**
 * @brief tuya key-value serialize, transfer the data from TY_DB_RW_S array to json format
 *
 * @param[in] rw the database entry which contains a property array
 * @param[in] rw_cnt the database entry property array count
 * @param[out] out the serialized json format buffer
 * @param[out] out_len the serialized json format length
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_utils_serialize(IN CONST TY_DB_RW_S *rw, IN CONST UINT_T rw_cnt, OUT CHAR_T **out,
                               OUT UINT_T *out_len);

/**
 * @brief tuya key-value serialize and save in tuya key-value database
 *
 * @param[in] key key of the database entry
 * @param[in] rw the database entry which contains a property array
 * @param[in] rw_cnt the database entry property array count
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_utils_serialize_save(IN CONST CHAR_T *key, IN CONST TY_DB_RW_S *rw,
                                    IN CONST UINT_T rw_cnt);

/**
 * @brief tuya key-value serialize restore, will read the data from tuya key-value database and
 * restore to database property entry
 *
 * @param[in] key key of the database entry
 * @param[inout] rw the database entry which contains a property array
 * @param[inout] rw_cnt the database entry property array count
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note tuya key-value database saved the json format data and this api will read and deserialize
 * it to property entry according the rw and rw_cnt, so must fill the rw and rw_cnt in right format.
 */
OPERATE_RET wd_utils_serialize_restore(IN CONST CHAR_T *key, INOUT TY_DB_RW_S *rw,
                                       IN CONST UINT_T rw_cnt);

/**
 * @brief tuya key-value deserialize, transfer the data from json format to property array
 *
 * @param[in] in the json format data
 * @param[inout] rw the database entry which contains a property array
 * @param[inout] rw_cnt the database entry property array count
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 * @note deserialize the json format data to database property entry according the rw and rw_cnt, so
 * must fill the rw and rw_cnt in right format.
 */
OPERATE_RET wd_utils_deserialize(IN CONST CHAR_T *in, INOUT TY_DB_RW_S *rw, IN CONST UINT_T rw_cnt);

/**
 * @brief free the buffer which allocated by wd_utils_serialize_restore
 *
 * @param out_buf the buffer got from wd_utils_serialize_restore
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET wd_utils_free_outbuf(IN CHAR_T *out_buf);

#ifdef __cplusplus
}
#endif
#endif

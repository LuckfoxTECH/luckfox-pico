/**
 * @file log_seq.h
 * @brief provide log sequence api
 * @version 0.1
 * @date 2019-04-22
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __LOG_SEQ_H
#define __LOG_SEQ_H

#include "tuya_cloud_types.h"
#include "tuya_os_adapter.h"
#include "uni_pointer.h"

#ifdef __cplusplus
extern "C" {
#endif

#define STR_DATA_MAX_LENGTH 64

typedef BYTE_T LS_DATA_TP_T; // log sequence data type
#define LDT_NULL 0
#define LDT_DEC 1
#define LDT_HEX 2
#define LDT_TIMESTAMP 3
#define LDT_STRING 4

/**
 * @brief data storage structure of log sequence
 */
typedef union {
	INT_T dec_data;
	UINT_T hex_data;
	TIME_T tm_data;
	CHAR_T *str_data;
} LOG_DA_TP_U;

/**
 * @brief data structure of log sequence
 */
typedef struct {
	/** log id */
	BYTE_T id;
	/** log type */
	LS_DATA_TP_T type;
	/** log data */
	LOG_DA_TP_U data;
	/** log list node */
	struct tuya_list_head node;
} LS_DATA_S;

typedef BYTE_T LS_STAT_T;
#define LSS_INIT 0
#define LSS_RECORDING 1
#define LSS_LOCKED 2 // LOG_SEQ_T set locked status when record the error log sequence

// Calling when record the error log sequence
typedef int (*LOG_SEQ_UPLOAD_CB)(const char *p_log);

#define LS_DELIMITER " | "

int log_seq_insert_log(const char *ls_name, BYTE_T id, LS_DATA_TP_T type, VOID_T *data);
int log_seq_insert_error_log(const char *ls_name, BYTE_T id, LS_DATA_TP_T type, VOID_T *data);
int log_seq_insert_report_log(const char *ls_name, BYTE_T id, LS_DATA_TP_T type, VOID_T *data);

#define INSERT_LOG_SEQ_NULL(seq) log_seq_insert_log(LOGSEQ_OBJ, seq, LDT_NULL, NULL)
#define INSERT_LOG_SEQ_DEC(seq, dec)                                                               \
	log_seq_insert_log(LOGSEQ_OBJ, seq, LDT_DEC, (VOID_T *)(intptr_t)dec)
#define INSERT_LOG_SEQ_HEX(seq, hex)                                                               \
	log_seq_insert_log(LOGSEQ_OBJ, seq, LDT_HEX, (VOID_T *)(uintptr_t)hex)
#define INSERT_LOG_SEQ_TM(seq, tm) log_seq_insert_log(LOGSEQ_OBJ, seq, LDT_TIMESTAMP, (VOID_T *)tm)
// string max length is STR_DATA_MAX_LENGTH
#define INSERT_LOG_SEQ_STR(seq, str) log_seq_insert_log(LOGSEQ_OBJ, seq, LDT_STRING, (VOID_T *)str)
#define INSERT_ERROR_LOG_SEQ_NULL(seq) log_seq_insert_error_log(LOGSEQ_OBJ, seq, LDT_NULL, NULL)
#define INSERT_ERROR_LOG_SEQ_DEC(seq, dec)                                                         \
	log_seq_insert_error_log(LOGSEQ_OBJ, seq, LDT_DEC, (VOID_T *)(intptr_t)dec)
#define INSERT_ERROR_LOG_SEQ_HEX(seq, hex)                                                         \
	log_seq_insert_error_log(LOGSEQ_OBJ, seq, LDT_HEX, (VOID_T *)(uintptr_t)hex)
#define INSERT_ERROR_LOG_SEQ_TM(seq, tm)                                                           \
	log_seq_insert_error_log(LOGSEQ_OBJ, seq, LDT_TIMESTAMP, (VOID_T *)tm)
// string max length is STR_DATA_MAX_LENGTH
#define INSERT_ERROR_LOG_SEQ_STR(seq, str)                                                         \
	log_seq_insert_error_log(LOGSEQ_OBJ, seq, LDT_STRING, (VOID_T *)str)
#define INSERT_REPORT_LOG_SEQ_NULL(seq) log_seq_insert_report_log(LOGSEQ_OBJ, seq, LDT_NULL, NULL)
#define INSERT_REPORT_LOG_SEQ_DEC(seq, dec)                                                        \
	log_seq_insert_report_log(LOGSEQ_OBJ, seq, LDT_DEC, (VOID_T *)(intptr_t)dec)
#define INSERT_REPORT_LOG_SEQ_HEX(seq, hex)                                                        \
	log_seq_insert_report_log(LOGSEQ_OBJ, seq, LDT_HEX, (VOID_T *)(uintptr_t)hex)
#define INSERT_REPORT_LOG_SEQ_TM(seq, tm)                                                          \
	log_seq_insert_report_log(LOGSEQ_OBJ, seq, LDT_TIMESTAMP, (VOID_T *)tm)
// string max length is STR_DATA_MAX_LENGTH
#define INSERT_REPORT_LOG_SEQ_STR(seq, str)                                                        \
	log_seq_insert_report_log(LOGSEQ_OBJ, seq, LDT_STRING, (VOID_T *)str)

/**
 * @brief initialize of log sequence.
 *
 * @param[in] log_seq_path log sequence path
 *
 * @note This API is used for initialize log sequence.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_init(const char *log_seq_path);

/**
 * @brief enable or disable log sequence.
 *
 * @param[in] enable if value is true, enable log sequence, otherwise disable log sequence
 *
 * @note This API is used for enalbe or disable log sequence.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_set_enable(bool_t enable);

/**
 * @brief set log sequence depth.
 *
 * @param[in] depth log sequence depth
 *
 * @note This API is used for set depth of log sequence.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_set_depth(const unsigned int depth);

/**
 * @brief reset log sequence, clean all log sequence, this is used when device reset.
 *
 * @param[in] is_force_clean_all, force clean all log sequence object
 *
 * @note This API is used for clean all log sequence object.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_clean(int is_force_clean_all);

/**
 * @brief force synchronization of log sequence .
 *
 * @param[in] NONE
 *
 * @note This API is used for synchronization of log sequence.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_force_sync(void);

/**
 * @brief upload customer log sequence .
 *
 * @param[in] p_log user log
 *
 * @note This API is used for upload customer log sequence.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_upload_custom_log(const char *p_log);

/**
 * @brief get network configure log sequence.
 *
 * @param[in] log_buff, log buffer
 * @param[in] log_len, log size
 *
 * @note This API is used for get network configure log sequence.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int log_seq_get_netcfg_log(char **log_buff, int *log_len);

#ifdef __cplusplus
}
#endif
#endif

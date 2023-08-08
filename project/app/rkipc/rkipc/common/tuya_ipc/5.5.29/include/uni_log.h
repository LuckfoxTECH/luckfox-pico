/**
 * @file uni_log.h
 * @brief provide log management api
 * @version 0.1
 * @date 2018-11-09
 *
 * @copyright Copyright 2018-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef _UNI_LOG_H
#define _UNI_LOG_H

#include "tuya_cloud_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef INT_T LOG_LEVEL;

/**
 * @brief Definition of log level
 */
#define TY_LOG_LEVEL_ERR 0    // error information
#define TY_LOG_LEVEL_WARN 1   // warnning information
#define TY_LOG_LEVEL_NOTICE 2 // notice information
#define TY_LOG_LEVEL_INFO 3   // information
#define TY_LOG_LEVEL_DEBUG 4  // debug information , should delete in release version
#define TY_LOG_LEVEL_TRACE 5  // trace information , should delete in release version

// default global log level
#if defined(TUYA_IOT_DEBUG) && (TUYA_IOT_DEBUG == 1)
#define TY_DEAULT_LOG_LEVEL TY_LOG_LEVEL_DEBUG
#else
#define TY_DEAULT_LOG_LEVEL TY_LOG_LEVEL_INFO
#endif

#define DEF_LOG_BUF_LEN 1024
#define LOG_LEVEL_MIN 0
#define LOG_LEVEL_MAX 5

// file name maybe define from complie parameter
#ifndef _THIS_FILE_NAME_
#define _THIS_FILE_NAME_ __FILE__
#endif

/**
 * @brief output log
 *
 * @param[in] pModuleName, module name
 * @param[in] logLevel, this log level
 * @param[in] pFile, file name of this log
 * @param[in] line, line number in the file of this log
 * @param[in] pFmt, format string
 * @param[in] ..., parameters
 *
 * @note This API is used for output log.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int PrintLog(const char *pModuleName, const LOG_LEVEL logLevel, const char *pFile, const int line,
             const char *pFmt, ...);

#define SAK_PRINT_LOG(module, level, fmt, ...)                                                     \
	do {                                                                                           \
		PrintLog(module, level, _THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                   \
	} while (0)

// this level, log dont has line_number information
#define TUYA_REDUCE_BIN_LEVEL1 1

// this level, log dont has file_name and line_number information
#define TUYA_REDUCE_BIN_LEVEL2 2

// this level, log dont has file_name , line_number and no tare and debug information
#define TUYA_REDUCE_BIN_LEVEL3 3

// all type of log output interface
#if defined(TUYA_REDUCE_BIN) && (TUYA_REDUCE_BIN >= TUYA_REDUCE_BIN_LEVEL2)
int PrintErrLog(const char *pFmt, ...);
int PrintWarnLog(const char *pFmt, ...);
int PrintInfoLog(const char *pFmt, ...);
int PrintNoticeLog(const char *pFmt, ...);
int PrintDebugLog(const char *pFmt, ...);
int PrintTraceLog(const char *pFmt, ...);
#elif defined(TUYA_REDUCE_BIN) && (TUYA_REDUCE_BIN >= TUYA_REDUCE_BIN_LEVEL1)
int PrintErrLog(const char *pFile, const char *pFmt, ...);
int PrintWarnLog(const char *pFile, const char *pFmt, ...);
int PrintInfoLog(const char *pFile, const char *pFmt, ...);
int PrintNoticeLog(const char *pFile, const char *pFmt, ...);
int PrintDebugLog(const char *pFile, const char *pFmt, ...);
int PrintTraceLog(const char *pFile, const char *pFmt, ...);
#else
int PrintErrLog(const char *pFile, const int line, const char *pFmt, ...);
int PrintWarnLog(const char *pFile, const int line, const char *pFmt, ...);
int PrintInfoLog(const char *pFile, const int line, const char *pFmt, ...);
int PrintNoticeLog(const char *pFile, const int line, const char *pFmt, ...);
int PrintDebugLog(const char *pFile, const int line, const char *pFmt, ...);
int PrintTraceLog(const char *pFile, const int line, const char *pFmt, ...);
#endif

// define different function prototype of log output to reduce bin size
#if defined(TUYA_REDUCE_BIN) && (TUYA_REDUCE_BIN >= TUYA_REDUCE_BIN_LEVEL2)
#define SAK_PRINT_ERR_LOG(fmt, ...)                                                                \
	do {                                                                                           \
		PrintErrLog(fmt, ##__VA_ARGS__);                                                           \
	} while (0)

#define SAK_PRINT_WARN_LOG(fmt, ...)                                                               \
	do {                                                                                           \
		PrintWarnLog(fmt, ##__VA_ARGS__);                                                          \
	} while (0)

#define SAK_PRINT_INFO_LOG(fmt, ...)                                                               \
	do {                                                                                           \
		PrintInfoLog(fmt, ##__VA_ARGS__);                                                          \
	} while (0)

#define SAK_PRINT_NOTICE_LOG(fmt, ...)                                                             \
	do {                                                                                           \
		PrintNoticeLog(fmt, ##__VA_ARGS__);                                                        \
	} while (0)

#define SAK_PRINT_DEBUG_LOG(fmt, ...)                                                              \
	do {                                                                                           \
		PrintDebugLog(fmt, ##__VA_ARGS__);                                                         \
	} while (0)

#define SAK_PRINT_TRACE_LOG(fmt, ...)                                                              \
	do {                                                                                           \
		PrintTraceLog(fmt, ##__VA_ARGS__);                                                         \
	} while (0)

#elif defined(TUYA_REDUCE_BIN) && (TUYA_REDUCE_BIN >= TUYA_REDUCE_BIN_LEVEL1)

#define SAK_PRINT_ERR_LOG(fmt, ...)                                                                \
	do {                                                                                           \
		PrintErrLog(_THIS_FILE_NAME_, fmt, ##__VA_ARGS__);                                         \
	} while (0)

#define SAK_PRINT_WARN_LOG(fmt, ...)                                                               \
	do {                                                                                           \
		PrintWarnLog(_THIS_FILE_NAME_, fmt, ##__VA_ARGS__);                                        \
	} while (0)

#define SAK_PRINT_INFO_LOG(fmt, ...)                                                               \
	do {                                                                                           \
		PrintInfoLog(_THIS_FILE_NAME_, fmt, ##__VA_ARGS__);                                        \
	} while (0)

#define SAK_PRINT_NOTICE_LOG(fmt, ...)                                                             \
	do {                                                                                           \
		PrintNoticeLog(_THIS_FILE_NAME_, fmt, ##__VA_ARGS__);                                      \
	} while (0)

#define SAK_PRINT_DEBUG_LOG(fmt, ...)                                                              \
	do {                                                                                           \
		PrintDebugLog(_THIS_FILE_NAME_, fmt, ##__VA_ARGS__);                                       \
	} while (0)

#define SAK_PRINT_TRACE_LOG(fmt, ...)                                                              \
	do {                                                                                           \
		PrintTraceLog(_THIS_FILE_NAME_, fmt, ##__VA_ARGS__);                                       \
	} while (0)

#else

#define SAK_PRINT_ERR_LOG(fmt, ...)                                                                \
	do {                                                                                           \
		PrintErrLog(_THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                               \
	} while (0)

#define SAK_PRINT_WARN_LOG(fmt, ...)                                                               \
	do {                                                                                           \
		PrintWarnLog(_THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                              \
	} while (0)

#define SAK_PRINT_INFO_LOG(fmt, ...)                                                               \
	do {                                                                                           \
		PrintInfoLog(_THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                              \
	} while (0)

#define SAK_PRINT_NOTICE_LOG(fmt, ...)                                                             \
	do {                                                                                           \
		PrintNoticeLog(_THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                            \
	} while (0)

#define SAK_PRINT_DEBUG_LOG(fmt, ...)                                                              \
	do {                                                                                           \
		PrintDebugLog(_THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                             \
	} while (0)

#define SAK_PRINT_TRACE_LOG(fmt, ...)                                                              \
	do {                                                                                           \
		PrintTraceLog(_THIS_FILE_NAME_, __LINE__, fmt, ##__VA_ARGS__);                             \
	} while (0)
#endif

// there is no trace or debug information when REDUCE_BIN leven bigger than LEVEL3
#if defined(TUYA_REDUCE_BIN) && (TUYA_REDUCE_BIN >= TUYA_REDUCE_BIN_LEVEL3)
#define PR_TRACE(fmt, ...)
#define PR_TRACE_ENTER()
#define PR_TRACE_LEAVE()
#define MPR_TRACE(module, fmt, ...)
#define PR_DEBUG(fmt, ...)
#define MPR_DEBUG(module, fmt, ...)
#else
#define PR_TRACE(fmt, ...) SAK_PRINT_TRACE_LOG(fmt, ##__VA_ARGS__)
#define PR_TRACE_ENTER() PR_TRACE("enter [%s]", (const char *)__func__)
#define PR_TRACE_LEAVE() PR_TRACE("leave [%s]", (const char *)__func__)

#define MPR_TRACE(module, fmt, ...) SAK_PRINT_LOG(module, TY_LOG_LEVEL_TRACE, fmt, ##__VA_ARGS__)
#define PR_DEBUG(fmt, ...) SAK_PRINT_DEBUG_LOG(fmt, ##__VA_ARGS__)
#define MPR_DEBUG(module, fmt, ...) SAK_PRINT_LOG(module, TY_LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#endif

// log interface for user
#define PR_ERR(fmt, ...) SAK_PRINT_ERR_LOG(fmt, ##__VA_ARGS__)
#define PR_WARN(fmt, ...) SAK_PRINT_WARN_LOG(fmt, ##__VA_ARGS__)
#define PR_NOTICE(fmt, ...) SAK_PRINT_NOTICE_LOG(fmt, ##__VA_ARGS__)
#define PR_INFO(fmt, ...) SAK_PRINT_INFO_LOG(fmt, ##__VA_ARGS__)

// set log level
#define SET_PR_DEBUG_LEVEL(level) SetLogManageAttr(level)

// log output interface with module name
#define MPR_ERR(module, fmt, ...) SAK_PRINT_LOG(module, TY_LOG_LEVEL_ERR, fmt, ##__VA_ARGS__)
#define MPR_WARN(module, fmt, ...) SAK_PRINT_LOG(module, TY_LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define MPR_NOTICE(module, fmt, ...) SAK_PRINT_LOG(module, TY_LOG_LEVEL_NOTICE, fmt, ##__VA_ARGS__)
#define MPR_INFO(module, fmt, ...) SAK_PRINT_LOG(module, TY_LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

#if defined(TUYA_REDUCE_BIN) && (TUYA_REDUCE_BIN >= TUYA_REDUCE_BIN_LEVEL3)
#define PR_DEBUG_RAW(fmt, ...)
#else
#if defined(TUYA_IOT_DEBUG) && (TUYA_IOT_DEBUG == 1)
#define PR_DEBUG_RAW(fmt, ...) PrintLogRaw(fmt, ##__VA_ARGS__)
#else
#define PR_DEBUG_RAW(fmt, ...)
#endif
#endif

// prototype of log output function
typedef VOID (*LOG_OUTPUT)(IN CONST CHAR_T *str);

/**
 * @brief initialize log management.
 *
 * @param[in] curLogLevel , set log level
 * @param[in] buf_len , set log buffer size
 * @param[in] output , log print function pointer
 *
 * @note This API is used for initializing log management.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET CreateLogManageAndInit(IN CONST LOG_LEVEL curLogLevel, IN CONST INT_T buf_len,
                                   IN CONST LOG_OUTPUT output);

/**
 * @brief add one output terminal.
 *
 * @param[in] name , terminal name
 * @param[in] term , output function pointer
 *
 * @note This API is used for adding one output terminal.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET AddOutputTerm(IN CONST CHAR_T *name, IN CONST LOG_OUTPUT term);

/**
 * @brief delete one output terminal.
 *
 * @param[in] name , terminal name
 *
 * @note This API is used for delete one output terminal.
 *
 * @return NONE
 */
VOID DelOutputTerm(IN CONST CHAR_T *name);

/**
 * @brief set global log level.
 *
 * @param[in] curLogLevel , log level
 *
 * @note This API is used for setting global log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET SetLogManageAttr(IN CONST LOG_LEVEL curLogLevel);

/**
 * @brief set log time whether show in millisecond.
 *
 * @param[in] if_ms_level, whether log time include millisecond
 *
 * @note This API is used for setting log time whether include milisecond.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET SetLogManagemtime(BOOL_T if_ms_level);

/**
 * @brief get global log level.
 *
 * @param[in] pCurLogLevel, global log level
 *
 * @note This API is used for getting global log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET GetLogManageAttr(OUT LOG_LEVEL *pCurLogLevel);

/**
 * @brief add one module's log level
 *
 * @param[in] pModuleName, module name
 * @param[in] logLevel, this module's log level
 *
 * @note This API is used for adding one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET AddModuleLevel(IN CONST PCHAR_T pModuleName, IN CONST LOG_LEVEL logLevel);

/**
 * @brief add one module's log level
 *
 * @param[in] pModuleName, module name
 * @param[in] logLevel, this module's log level
 *
 * @note This API is used for adding one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET SetModuleLevel(IN CONST PCHAR_T pModuleName, LOG_LEVEL log_level);

/**
 * @brief get one module's log level
 *
 * @param[in] pModuleName, module name
 * @param[in] logLevel, this module's log level
 *
 * @note This API is used for getting one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET GetModuleLevel(IN CONST PCHAR_T pModuleName, OUT LOG_LEVEL *pLogLevel);

/**
 * @brief delete one module's log level
 *
 * @param[in] pModuleName, module name
 *
 * @note This API is used for deleting one module's log level.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET DeleteModuleLevel(IN CONST PCHAR_T pModuleName);

/**
 * @brief only print user log information
 *
 * @param[in] pFmt, format string
 * @param[in] ..., parameter
 *
 * @note This API is used for print only user log info.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
OPERATE_RET PrintLogRaw(IN CONST PCHAR_T pFmt, ...);

/**
 * @brief destroy log management
 *
 * @param[in] pFmt, format string
 * @param[in] ..., parameter
 *
 * @note This API is used for destroy log management.
 *
 * @return NONE
 */
VOID ReleaseLogManage(VOID);

/**
 * @brief print a buffer in hex format
 *
 * @param[in] title, buffer title for print
 * @param[in] width, one line width
 * @param[in] buf, buffer address
 * @param[in] size, buffer size
 *
 * @note This API is used for print one buffer.
 *
 * @return NONE
 */
VOID tuya_debug_hex_dump(CHAR_T *title, UINT8_T width, UINT8_T *buf, UINT16_T size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

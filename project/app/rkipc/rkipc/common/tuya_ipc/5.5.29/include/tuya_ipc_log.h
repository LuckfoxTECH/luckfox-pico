/*********************************************************************************
 *Copyright(C),2017, 涂鸦科技 www.tuya.comm
 *FileName:    tuya_ipc_log.h
 **********************************************************************************/

#ifndef _TUYA_IPC_LOG_H
#define _TUYA_IPC_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \fn OPERATE_RET tuya_ipc_set_log_attr(IN CONST INT log_level, CHAR *filename)
 * \brief set tuya SDK log level and destination
 * \param[in] log_level 0-5, the bigger, the more log
 * \param[in] filename if need to print to filesystem
 * \return OPERATE_RET
 */
OPERATE_RET tuya_ipc_set_log_attr(IN CONST INT_T log_level, CHAR_T *filename);

OPERATE_RET tuya_ipc_log_setting(IN CONST INT_T log_level, FILE *filename, BOOL_T if_ms_level);

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_LOG_H*/

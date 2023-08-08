/**
 * @file tuya_ir_code.h
 * @brief
 * @author tuya
 * @version 1.0.0
 * @date 2021-01-13
 */

#include "tuya_cloud_types.h"

#ifndef __TUYA_IR_CODE_H__
#define __TUYA_IR_CODE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief tuya_ir_clear_irtime
 *
 * @return none
 */
VOID tuya_ir_clear_irtime(VOID);

/**
 * @brief tuya_ir_get_irtime
 *
 * @param[in] data[]
 * @param[in] data_len
 *
 * @return BOOL_T
 */
BOOL_T tuya_ir_get_irtime(USHORT_T data[], USHORT_T data_len);

/**
 * @brief tuya_ir_encode_head
 *
 * @param[in] feq
 * @param[in] data[]
 * @param[in] data_len
 * @param[in] ir_head
 * @param[in] ir_head_size
 *
 * @return BOOL_T
 */
BOOL_T tuya_ir_encode_head(USHORT_T feq, USHORT_T data[], USHORT_T data_len, CHAR_T *ir_head,
                           USHORT_T ir_head_size);

/**
 * @brief tuya_ir_encode
 * @desc encode
 *
 * @param[in] framecount
 * @param[in] data[]
 * @param[in] length
 * @param[in] ir_head
 * @param[in] ir_head_size
 * @param[in] ir_key
 * @param[in] ir_key_size
 *
 * @return BOOL_T
 */
BOOL_T tuya_ir_encode(BYTE_T framecount, USHORT_T data[], USHORT_T length, CHAR_T *ir_head,
                      USHORT_T ir_head_size, CHAR_T *ir_key, USHORT_T ir_key_size);

/**
 * @brief tuya_ir_decode
 * @desc decode
 *
 * @param[in] head
 * @param[in] key
 * @param[in] decode_buf
 * @param[in] buf_size
 * @param[in] framecount
 * @param[in] feq
 *
 * @return USHORT_T
 */
USHORT_T tuya_ir_decode(CHAR_T *head, CHAR_T *key, USHORT_T *decode_buf, USHORT_T buf_size,
                        BYTE_T *framecount, USHORT_T *feq);

#ifdef __cplusplus
}
#endif

#endif

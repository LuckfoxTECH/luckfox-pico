#ifndef TUYA_OPENSSL_ENCRYPT_H
#define TUYA_OPENSSL_ENCRYPT_H

#include <string.h>

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"
#include "tuya_ipc_media.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************
*  Function: OpensslAES_CBC128_encrypt
*  Input:
  IN CONST BYTE *pdata_in,    \\需要加密数据的BUFFER地址
  IN CONST UINT data_len,     \\需要加密数据总长度
  IN UINT out_buffer_len,     \\加密BUFFER的长度
  IN CONST BYTE *pkey         \\加密的密钥
  IN BYTE *piv                \\加密向量
  Output:
  OUT BYTE *pdata_out,        \\加密BUFFER地址
  OUT UINT *pdata_out_len,    \\加密后数据的长度
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET OpensslAES_CBC128_encrypt(IN CONST BYTE_T *pdata_in, IN CONST UINT_T data_len,
                                      OUT BYTE_T *pdata_out, OUT UINT_T *pdata_out_len,
                                      IN CONST BYTE_T *pkey, IN BYTE_T *piv);

/***********************************************************
*  Function: OpensslAES_CBC128_decrypt
*  Input:
  IN CONST BYTE *pdata_in,    \\需要解密数据的BUFFER地址
  IN CONST UINT data_len,     \\需要解密数据总长度
  IN UINT out_buffer_len,     \\解密BUFFER的长度
  IN CONST BYTE *pkey         \\解密的密钥
  IN BYTE *piv                \\解密向量
  Output:
  OUT BYTE *pdata_out,        \\解密BUFFER地址
  OUT UINT *pdata_out_len,    \\解密后数据的长度
*  Return: OPERATE_RET
***********************************************************/
OPERATE_RET OpensslAES_CBC128_decrypt(IN CONST BYTE_T *pdata_in, IN CONST UINT_T data_len,
                                      OUT BYTE_T *pdata_out, OUT UINT_T *pdata_out_len,
                                      IN CONST BYTE_T *pkey, IN BYTE_T *piv);

#ifdef __cplusplus
}
#endif

#endif //

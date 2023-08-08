#ifndef __ENC_PIC_H__
#define __ENC_PIC_H__

#ifdef __cplusplus
extern "C" {
#endif
OPERATE_RET cloud_encrypt_pic_with_random_iv(IN BYTE_T *buffer, IN UINT_T size,
                                             OUT BYTE_T **en_buffer, OUT UINT_T *en_size,
                                             IN BYTE_T *local_aes_key);

OPERATE_RET cloud_decrypt_pic_with_random_iv(IN BYTE_T *buffer, IN UINT_T size,
                                             OUT BYTE_T *de_buffer, OUT UINT_T *de_size,
                                             IN BYTE_T *local_aes_key);

OPERATE_RET cloud_encrypt_pic_get_aes_key(BYTE_T *key, INT_T len);

OPERATE_RET cloud_encrypt_pic_init();

OPERATE_RET cloud_generate_random_iv(UCHAR_T *iv, UINT_T iv_size);

OPERATE_RET cloud_generate_random_key(UCHAR_T *key, UINT_T iv_size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __ENC_PIC_H__

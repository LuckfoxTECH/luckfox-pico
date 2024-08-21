#ifndef __HASH_SM3_H__
#define __HASH_SM3_H__

#ifdef __cplusplus
extern "C" {
#endif


int rk_hash_sm3(const unsigned char *in, unsigned int in_len, 
							unsigned char *out, unsigned int *out_len);


#ifdef __cplusplus
}
#endif

#endif /* sm3.h */

#ifndef AENC_MP3_REGISTER_H_
#define AENC_MP3_REGISTER_H_
#include "rk_type.h"
#include "rk_comm_aio.h"
// open audio encoder
RK_S32 RKAduioMp3EncoderOpen(RK_VOID *pEncoderAttr, RK_VOID **ppEncoder);
// close audio encoder
RK_S32 RKAduioMp3EncoderClose(RK_VOID *pEncoder);
RK_S32 RKAduioMp3EncoderEncode(RK_VOID *pEncoder, RK_VOID *pEncParam);

#endif

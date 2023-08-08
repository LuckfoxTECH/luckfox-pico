/** ==================================================================
 *  @file   tuya_ipc_g711_rtp.h
 *
 *  @path
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c)
 *
 * ===================================================================*/

#ifndef TUYA_IPC_G711_RTP_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define TUYA_IPC_G711_RTP_H_

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

#define RTP_PCMU_PAYLOAD (0)
#define RTP_PCMA_PAYLOAD (8)
#define RTP_PCM_PAYLOAD (99)

#define RTP_PCM_MAX_LEN (1414)

OPERATE_RET tuya_ipc_pack_pcm_rtp(UCHAR_T *pPcm, USHORT_T pcm_len, INT_T mode, INT_T *rtp_len,
                                  CHAR_T *pRtpBuff, UINT64_T pts, UINT_T client);

OPERATE_RET tuya_ipc_unpack_pcm_rtp(UCHAR_T *pRtpBuff, INT_T rtp_len, UCHAR_T *pPcm,
                                    USHORT_T *pPcmLen);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/** ==================================================================
 *  @file   tuya_ipc_rtp.h
 *
 *  @path
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c)
 *
 * ===================================================================*/

#ifndef TUYA_IPC_RTP_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define TUYA_IPC_RTP_H_

#define RTP_MTU_LEN (1250) //(1400)
#define RTP_SPLIT_LEN (RTP_MTU_LEN)
#define RTP_TIMESTEMP_RFC (90000) // rfc3984默认1s划分时间片个数
#define MAX_RTP_PACK_CLIENT_NUM 20

#define TUYA_RTP_HEAD 0x12345678 //自定义rtp识别包头

typedef enum {
	TY_AV_CODEC_VIDEO_UNKOWN = 0,
	TY_AV_CODEC_VIDEO_MPEG4 = 0x10,
	TY_AV_CODEC_VIDEO_H263 = 0x11,
	TY_AV_CODEC_VIDEO_H264 = 0x12,
	TY_AV_CODEC_VIDEO_MJPEG = 0x13,
	TY_AV_CODEC_VIDEO_H265 = 0x14,

	TY_AV_CODEC_AUDIO_ADPCM = 0x80,
	TY_AV_CODEC_AUDIO_PCM = 0x81,
	TY_AV_CODEC_AUDIO_AAC_RAW = 0x82,
	TY_AV_CODEC_AUDIO_AAC_ADTS = 0x83,
	TY_AV_CODEC_AUDIO_AAC_LATM = 0x84,
	TY_AV_CODEC_AUDIO_G711U = 0x85, // 10
	TY_AV_CODEC_AUDIO_G711A = 0x86,
	TY_AV_CODEC_AUDIO_G726 = 0x87,
	TY_AV_CODEC_AUDIO_SPEEX = 0x88,
	TY_AV_CODEC_AUDIO_MP3 = 0x89,

	TY_AV_CODEC_MAX = 0xFF
} TY_AV_CODEC_ID;

typedef struct {
#ifdef RTP_BIG_ENDIAN
	unsigned char version : 2; /**/   /* expect 2 */
	unsigned char padding : 1; /**/   /* expect 0 */
	unsigned char extension : 1; /**/ /* expect 1, see RTP_OP below */
	unsigned char csrc_len : 4; /**/  /* expect 0 */
	/**/                              /* byte 1 */
	unsigned char marker : 1; /**/    /* expect 1 */
	unsigned char payload : 7; /**/   /* RTP_PAYLOAD_RTSP */

#else
	/**/                              /* byte 0 */
	unsigned char csrc_len : 4; /**/  /* expect 0 */
	unsigned char extension : 1; /**/ /* expect 1, see RTP_OP below */
	unsigned char padding : 1; /**/   /* expect 0 */
	unsigned char version : 2; /**/   /* expect 2 */
	/**/                              /* byte 1 */
	unsigned char payload : 7; /**/   /* RTP_PAYLOAD_RTSP */
	unsigned char marker : 1; /**/    /* expect 1 */
#endif
	/**/ /* bytes 2, 3 */
	unsigned short seq_no;
	/**/ /* bytes 4-7 */
	unsigned int timestamp;
	/**/                    /* bytes 8-11 */
	unsigned int ssrc; /**/ /* stream number is used here. */
} RTP_FIXED_HEADER;

typedef struct {

#ifdef RTP_BIG_ENDIAN
	// byte 0
	unsigned char F : 1;
	unsigned char NRI : 2;
	unsigned char TYPE : 5;
#else
	// byte 0
	unsigned char TYPE : 5;
	unsigned char NRI : 2;
	unsigned char F : 1;
#endif
} FU_INDICATOR; /**/ /* 1 BYTES */

typedef struct {
#ifdef RTP_BIG_ENDIAN
	// byte 0
	unsigned char S : 1;
	unsigned char E : 1;
	unsigned char R : 1;
	unsigned char TYPE : 5;
#else
	// byte 0
	unsigned char TYPE : 5;
	unsigned char R : 1;
	unsigned char E : 1;
	unsigned char S : 1;
#endif
} FU_HEADER; /**/ /* 1 BYTES */

typedef struct {

#if 1
	unsigned short F : 1;
	unsigned short TYPE : 6;
	unsigned short LAYID : 6;
	unsigned short TID : 3;
#else
	unsigned short TID : 3;
	unsigned short LAYID : 6;
	unsigned short TYPE : 6;
	unsigned short F : 1;
#endif
} NALU_HEVC_HEADER; /**/ /* 1 BYTES */

#pragma pack(1)
typedef struct {
	NALU_HEVC_HEADER naluHead;
#ifdef RTP_BIG_ENDIAN
	// byte 0
	unsigned char S : 1;
	unsigned char E : 1;
	unsigned char TYPE : 6;
#else
	// byte 0
	unsigned char TYPE : 6;
	unsigned char E : 1;
	unsigned char S : 1;
#endif
} FU_HEVC_HEADER; /**/ /* 1 BYTES */
#pragma pack()

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

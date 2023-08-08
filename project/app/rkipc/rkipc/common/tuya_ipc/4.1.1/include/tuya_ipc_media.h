#ifndef _TUYA_IPC_MEDIA_H_
#define _TUYA_IPC_MEDIA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"

#define MAX_MEDIA_FRAME_SIZE (300 * 1024)
#define AES_ENCRYPT_KEY_LEN 16

typedef enum {
	E_CHANNEL_VIDEO_MAIN = 0,
	E_CHANNEL_VIDEO_SUB,
	E_CHANNEL_VIDEO_3RD,
	E_CHANNEL_VIDEO_4TH,
	E_CHANNEL_VIDEO_MAX = 8, // max video streams
	E_CHANNEL_AUDIO,
	E_CHANNEL_AUDIO_2RD,
	E_CHANNEL_AUDIO_3RD,
	E_CHANNEL_AUDIO_4TH,
	E_CHANNEL_MAX = 16
} CHANNEL_E;

typedef enum {
	E_VIDEO_PB_FRAME = 0,
	E_VIDEO_I_FRAME,
	E_VIDEO_TS_FRAME,
	E_AUDIO_FRAME,
	E_CMD_FRAME,
	E_MEDIA_FRAME_TYPE_MAX
} MEDIA_FRAME_TYPE_E;

// enum for codec, sync with tuya cloud and APP, should NOT be changed
typedef enum {
	TUYA_CODEC_VIDEO_MPEG4 = 0,
	TUYA_CODEC_VIDEO_H263,
	TUYA_CODEC_VIDEO_H264,
	TUYA_CODEC_VIDEO_MJPEG,
	TUYA_CODEC_VIDEO_H265,
	TUYA_CODEC_VIDEO_MAX = 99,

	TUYA_CODEC_AUDIO_ADPCM,
	TUYA_CODEC_AUDIO_PCM,
	TUYA_CODEC_AUDIO_MP3_RAW,
	TUYA_CODEC_AUDIO_MP3_RAW1,
	TUYA_CODEC_AUDIO_MP3_RAW2,
	TUYA_CODEC_AUDIO_G711U,
	TUYA_CODEC_AUDIO_G711A,
	TUYA_CODEC_AUDIO_G726,
	TUYA_CODEC_AUDIO_SPEEX,
	TUYA_CODEC_AUDIO_MP3,
	TUYA_CODEC_AUDIO_MAX = 199,
	TUYA_CODEC_INVALID
} TUYA_CODEC_ID;

typedef enum {
	TUYA_AUDIO_SAMPLE_8K = 8000,
	TUYA_AUDIO_SAMPLE_11K = 11000,
	TUYA_AUDIO_SAMPLE_12K = 12000,
	TUYA_AUDIO_SAMPLE_16K = 16000,
	TUYA_AUDIO_SAMPLE_22K = 22000,
	TUYA_AUDIO_SAMPLE_24K = 24000,
	TUYA_AUDIO_SAMPLE_32K = 32000,
	TUYA_AUDIO_SAMPLE_44K = 44000,
	TUYA_AUDIO_SAMPLE_48K = 48000,
	TUYA_AUDIO_SAMPLE_MAX = 0xFFFFFFFF
} TUYA_AUDIO_SAMPLE_E;

typedef enum {
	TUYA_VIDEO_BITRATE_64K = 64,
	TUYA_VIDEO_BITRATE_128K = 128,
	TUYA_VIDEO_BITRATE_256K = 256,
	TUYA_VIDEO_BITRATE_512K = 512,
	TUYA_VIDEO_BITRATE_768K = 768,
	TUYA_VIDEO_BITRATE_1M = 1024,
	TUYA_VIDEO_BITRATE_1_5M = 1536,
	TUYA_VIDEO_BITRATE_2M =
	    2048 // maximum 2Mbps stream is supported, as consideration of cloud storage order price
} TUYA_VIDEO_BITRATE_E; // Kbps

typedef enum {
	TUYA_AUDIO_DATABITS_8 = 8,
	TUYA_AUDIO_DATABITS_16 = 16,
	TUYA_AUDIO_DATABITS_MAX = 0xFF
} TUYA_AUDIO_DATABITS_E;

typedef enum {
	TUYA_AUDIO_CHANNEL_MONO,
	TUYA_AUDIO_CHANNEL_STERO,
} TUYA_AUDIO_CHANNEL_E;

typedef struct {
	BOOL_T channel_enable[E_CHANNEL_MAX];

	UINT_T video_fps[E_CHANNEL_VIDEO_MAX];
	UINT_T video_gop[E_CHANNEL_VIDEO_MAX];
	TUYA_VIDEO_BITRATE_E video_bitrate[E_CHANNEL_VIDEO_MAX];
	UINT_T video_width[E_CHANNEL_VIDEO_MAX];
	UINT_T video_height[E_CHANNEL_VIDEO_MAX];
	UINT_T video_freq[E_CHANNEL_VIDEO_MAX];
	TUYA_CODEC_ID video_codec[E_CHANNEL_VIDEO_MAX];

	TUYA_CODEC_ID audio_codec[E_CHANNEL_MAX];
	UINT_T audio_fps[E_CHANNEL_MAX];
	TUYA_AUDIO_SAMPLE_E audio_sample[E_CHANNEL_MAX];
	TUYA_AUDIO_DATABITS_E audio_databits[E_CHANNEL_MAX];
	TUYA_AUDIO_CHANNEL_E audio_channel[E_CHANNEL_MAX];
} IPC_MEDIA_INFO_S;

typedef struct {
	UINT_T type;
	UINT_T size;
	UINT64_T timestamp;
	UINT64_T pts;
} STORAGE_FRAME_HEAD_S;

typedef struct {
	MEDIA_FRAME_TYPE_E type;
	BYTE_T *p_buf;
	UINT_T size;
	UINT64_T pts;
	UINT64_T timestamp;
} MEDIA_FRAME_S;

typedef INT_T (*Tuya_CBC_AES128_Init)(VOID);
typedef INT_T (*Tuya_CBC_AES128_Encrypt)(
    IN BYTE_T *pdata_in,       // data to be encrypted, should NOT be changed
    IN UINT_T data_len,        // date length to be encrypted
    IN BYTE_T *pdata_out,      // data after encrytion, memory is MALLOC inside tuya SDK already
    OUT UINT_T *pdata_out_len, // data length after encrytion
    IN BYTE_T *pkey,           // aes key
    IN BYTE_T *piv);           // aes iv for cbc mode

typedef INT_T (*Tuya_CBC_AES128_Decrypt)(
    IN BYTE_T *pdata_in,       // date to be decryted, should NOT be changed
    IN UINT_T data_len,        // data length after decryption
    IN BYTE_T *pdata_out,      // data after decryption, memory is MALLOC inside tuya SDK already
    OUT UINT_T *pdata_out_len, // data length after decrytion
    IN BYTE_T *pkey,           // aes key
    IN BYTE_T *piv);           // aes iv for cbc mode

typedef INT_T (*Tuya_CBC_AES128_Destroy)(VOID);

typedef struct {
	Tuya_CBC_AES128_Init init;
	Tuya_CBC_AES128_Encrypt encrypt;
	Tuya_CBC_AES128_Decrypt decrypt;
	Tuya_CBC_AES128_Destroy destory;
} AES_HW_CBC_FUNC;

#ifdef __cplusplus
}
#endif

#endif /*_TUYA_IPC_MEDIA_H_*/

/** ==================================================================
 *  @file   tuya_ipc_h264_rtp.h
 *
 *  @path
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c)
 *
 * ===================================================================*/

#ifndef TUYA_IPC_H264_RTP_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define TUYA_IPC_H264_RTP_H_

#define NAL3_LEN (3) // 00 00 01
#define NAL4_LEN (4) // 00 00 00 01

#define IS_NAL3(x) (0 == (x)[0] && 0 == (x)[1] && 1 == (x)[2])
#define IS_NAL4(x) (0 == (x)[0] && 0 == (x)[1] && 0 == (x)[2] && 1 == (x)[3])

#define DEFAULT_NALU_LEN (200 * 1024) //默认nalu长度200k
#define RTP_NAL_MIN_INTERV (4)        // nal包发送最小纠错间隔ms
#define NAL_TYPE_SPS (7)
#define NAL_TYPE_PPS (8)
#define NAL_TYPE_SEI (6)
#define NAL_TYPE_I (5)
#define NAL_TYPE_P (1)

//数据加密时使用
#define NAL_I_FRAME (0x65)
#define NAL_P_FRAME (0x61)

typedef struct {
	int startcodeprefix_len; //! 4 for parameter sets and first slice in picture, 3 for everything
	                         //! else (suggested)
	unsigned int len; //! Length of the NAL unit (Excluding the start code, which does not belong to
	                  //! the NALU)
	unsigned int max_size;       //! Nal Unit Buffer size
	int forbidden_bit;           //! should be always FALSE
	int nal_reference_idc;       //! NALU_PRIORITY_xxxx
	int nal_unit_type;           //! NALU_TYPE_xxxx
	char *buf;                   //! contains the first byte followed by the EBSP
	unsigned short lost_packets; //! true, if packet loss is detected
} NALU_T;

typedef struct {
#ifdef RTP_BIG_ENDIAN
	unsigned char F : 1;
	unsigned char NRI : 2;
	unsigned char TYPE : 5;
#else
	unsigned char TYPE : 5;
	unsigned char NRI : 2;
	unsigned char F : 1;
#endif
} NALU_HEADER; /**/ /* 1 BYTES */

typedef enum NAL_SPLIT_ {
	SPLIT_NO = 0,   //不分片
	SPLIT_HEAD = 1, //分片头
	SPLIT_MID = 2,  //分片中
	SPLIT_TAIL = 3, //分片尾
} NAL_SPLIT_E;

int tuya_ipc_get_nal_offset(char *pData, int len, int *nal_len);

// int tuya_ipc_get_next_nal(FILE * pFile, unsigned char * pOut);

NALU_T *tuya_ipc_alloc_nalu();

void tuya_ipc_free_nalu(NALU_T *p);

int tuya_ipc_get_annexb_nalu(NALU_T *nalu, char *pSrcBuf, int packLen);

int tuya_ipc_unpack_h264_rtp(char *pRtp, int len, char *pOutBuff, int *pOutLen);

int tuya_ipc_pack_h264_rtp_pack(NALU_T *nalu, int offset, NAL_SPLIT_E split, char *pOutBuff,
                                int *pOutLen, unsigned long long pts, int client);

int tuya_ipc_pack_h265_rtp_pack(NALU_T *nalu, int offset, NAL_SPLIT_E split, char *pOutBuff,
                                int *pOutLen, unsigned long long pts, int client);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

/** ==================================================================
 *  @file   tuya_ipc_aac_rtp.h
 *
 *  @path
 *
 *  @desc   This  File contains.
 * ===================================================================
 *  Copyright (c)
 *
 * ===================================================================*/

#ifndef TUYA_IPC_AAC_RTP_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#define TUYA_IPC_AAC_RTP_H_

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

#define ADTS_HEADER_LENGTH (7)
#define RTP_AAC_PAYLOAD (97)
#define AAC_RTP_MAX_LEN (1416) // rtp封装aac最大长度
#define RTP_AAC_TIMETEMP_INCRESE (1024)

// AAC规格profile
typedef enum AAC_PF_TYPE_ {
	AAC_4_MAIN = 0,   //主规格
	AAC_4_LC = 1,     //低复杂度规格，比较常用
	AAC_4_SSR = 2,    //可变采样率规格
	AAC_4_LTP = 3,    //长时期预测规格
	AAC_4_HE = 4,     //高效率规格，比较常用
	AAC_4_HE_V2 = 28, //
	AAC_4_LD = 22,    //
	AAC_4_ELD = 38,   //
	AAC_2_LC = 128,   //
	AAC_2_HE = 131,   //
} AAC_PF_TYPE_E;

//通道配置元素信息RawDataChanCfg
typedef enum RD_CHAN_CFG_ {
	TYPE_SCE = 1, //单通道元素，基本上由一个ICS组成
	TYPE_CPE,     //双通道元素，较常用
	TYPE_CCE,     //耦合通道元素
	TYPE_LFE,     //低频元素
	TYPE_DSE,     //数据流元素
	TYPE_PCE,     //程序配置元素
	TYPE_FIL,     //填充元素
	TYPE_END,     //
} RD_CHAN_CFG_E;

//采样率信息sampling frequeny [Hz]
typedef enum SAMPL_FREQ_ {
	SF_96 = 0, // 96khz
	SF_882,    // 88.2khz
	SF_64,     // 64khz
	SF_48,     // 48khz
	SF_441,    // 44.1khz  比较常用
	SF_32,     // 32khz
	SF_24,     // 24khz
	SF_2205,   // 22.05khz
	SF_16,     // 16khz
	SF_2,      // 2khz
	SF_11025,  // 11.025khz
	SF_8,      // 8khz
} SAMPL_FREQ_E;

// aac头部结构体，参照rfc3640共7byte
typedef struct {
	unsigned int syncword; // 12 bits 同步字  ‘1111 1111 1111’ ，说明一个ADTS帧得开始
	unsigned int id;       // 1 bits	MPEG标识符，设置为1
	unsigned int layer;    // 2 bits Indicates which layer is used. Set to ‘00‘
	unsigned int protection_absent; // 1 bits	表示是否误码校验
	unsigned int profile;           // 2 bits  表示使用哪个级别的AAC，一般为AAC_4_LC
	unsigned int sf_index;          // 4 bits  表示使用的采样率下标
	unsigned int private_bit;       // 1 bits
	unsigned int channel_configuration; // 3 bits  表示声道数
	unsigned int original;              // 1 bits
	unsigned int home;                  // 1 bits
	/*下面的为改变的参数即每一帧都不同*/
	unsigned int copyright_identification_bit;   // 1 bits
	unsigned int copyright_identification_start; // 1 bits
	unsigned int aac_frame_length; // 13 bits  一个ADTS帧的长度包括ADTS头和raw data block
	unsigned int adts_buffer_fullness; // 11 bits 	0x7FF 说明是码率可变的码流
	/*no_raw_data_blocks_in_frame 表示ADTS帧中有number_of_raw_data_blocks_in_frame + 1个AAC原始帧.
	所以说number_of_raw_data_blocks_in_frame == 0
	表示说ADTS帧中有一个AAC数据块并不是说没有。(一个AAC原始帧包含一段时间内1024个采样及相关数据)
	*/
	unsigned int no_raw_data_blocks_in_frame; // 2 bits
} ADTS_HEADER;

OPERATE_RET tuya_ipc_parse_adts_header(UCHAR_T *adts_headerbuf, ADTS_HEADER *pAdts);

OPERATE_RET tuya_ipc_comb_adts_header(AAC_PF_TYPE_E profile, SAMPL_FREQ_E sf, RD_CHAN_CFG_E chan,
                                      UINT_T aac_len, UCHAR_T *pAdtsHeader);

OPERATE_RET tuya_ipc_pack_aac_rtp(UCHAR_T *pAac, USHORT_T aac_len, INT_T *rtp_len, CHAR_T *pRtpBuff,
                                  UINT_T client);

OPERATE_RET tuya_ipc_unpack_aac_rtp(UCHAR_T *pRtpBuff, INT_T rtp_len, UCHAR_T *pAac,
                                    USHORT_T *pAacLen);

OPERATE_RET tuya_ipc_show_adts_info(ADTS_HEADER *pAdts);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

#ifndef _RKAUDIO_PREPROCESS_H_
#define _RKAUDIO_PREPROCESS_H_

#include <stdio.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif
#define NUM_CHANNEL					 8
#define NUM_REF_CHANNEL              1
#define NUM_DROP_CHANNEL             0
#define REF_POSITION				 1
//static short int Array[NUM_SRC_CHANNEL] = { 9,7,5,3,2,4,6,8 };
//static short int Array[NUM_CHANNEL] = {2, 3, 0, 1}; //src first, ref second
static short int Array[NUM_CHANNEL] = { 1, 0, 3, 2, 5, 4, 6, 7 };
//Array[NUM_SRC_CHANNEL] = { 2,3,4,5,6,7};
/**********************EQ Parameter**********************/
static short EqPara_16k[5][13] =
{
	//filter_bank 1
	{-1 ,-1 ,-1 ,-1 ,-2 ,-1 ,-1 ,-1 ,-1 ,-1 ,-1 ,-2 ,-3 },
	//filter_bank 2
	{-1 ,-1 ,-1 ,-1 ,-2 ,-2 ,-3 ,-5 ,-3 ,-2 ,-1 ,-1 ,-2 },
	//filter_bank 3
	{-2 ,-5 ,-9 ,-4 ,-2 ,-2 ,-1 ,-5 ,-5 ,-11 ,-20 ,-11 ,-5 },
	//filter_bank 4
	{-5 ,-1 ,-7 ,-7 ,-19 ,-40 ,-20 ,-9 ,-10 ,-1 ,-20 ,-24 ,-60 },
	//filter_bank 5
	{-128 ,-76 ,-40 ,-44 ,-1 ,-82 ,-111 ,-383 ,-1161 ,-1040 ,-989 ,-3811 ,32764 },
};

/**********************NLP Parameter**********************/
static short int ashwAecBandNlpPara_16k[8][2] =
{
	/* BandPassThd      SuperEstFactor*/
	{      10,                 6      },                                         /* Hz: 0    - 300  */
	{      10,                 10      },                                         /* Hz: 300  - 575  */
	{      10,                 10      },                                         /* Hz: 575  - 950  */
	{      5,                  10      },                                        /* Hz: 950  - 1425 */
	{      5,                  10      },                                        /* Hz: 1425 - 2150 */
	{      5,                  10      },                                        /* Hz: 2150 - 3350 */
	{      0,                  6      },                                        /* Hz: 3350 - 5450 */
	{      0,                  6      },                                        /* Hz: 5450 - 8000 */
};
/*************************************************/
/*The Main Enable which used to control the AEC,BF and RX*/
typedef enum RKAUDIOEnable_
{
	RKAUDIO_EN_AEC = 1 << 0,
	RKAUDIO_EN_BF = 1 << 1,
	RKAUDIO_EN_RX = 1 << 2,
	RKAUDIO_EN_CMD = 1 << 3,
} RKAUDIOEnable;

/* The Sub-Enable which used to control the AEC,BF and RX*/
typedef enum RKAecEnable_
{
	EN_DELAY = 1 << 0,
	EN_ARRAY_RESET = 1<<1,
	EN_HOWLING = 1 << 2,
} RKAecEnable;
typedef enum RKPreprocessEnable_
{
	EN_Fastaec = 1 << 0,
	EN_Wakeup = 1 << 1,
	EN_Dereverberation = 1 << 2,
	EN_Nlp = 1 << 3,
	EN_AES = 1<<4,
	EN_Agc = 1 << 5,
	EN_Anr = 1 << 6,
	EN_GSC = 1 << 7,
	GSC_Method = 1 << 8,
	EN_Fix = 1 << 9,
	EN_STDT = 1 << 10,
	EN_CNG = 1<<11,
	EN_EQ = 1<<12,
	EN_CHN_SELECT = 1<<13,
} RKPreprocessEnable;
typedef enum RkaudioRxEnable_
{
	EN_RX_Anr = 1 << 0,
} RkaudioRxEnable;
/*****************************************/

/* Set the three Main Para which used to initialize the AEC,BF and RX*/
typedef struct SKVAECParameter_ {
	int pos;
	int drop_ref_channel;
	int model_aec_en;
	int delay_len;
	int look_ahead;
	short int *Array_list;
} SKVAECParameter;
typedef struct SKVPreprocessParam_
{
	/* Parameters of agc */
	int model_bf_en;
	int ref_pos;
	int Targ;
	int num_ref_channel;
	int drop_ref_channel;
	void* dereverb_para;
	void* aes_para;
	void* nlp_para;
	void* anr_para;
	void* agc_para;
	void* cng_para;
	void* dtd_para;
	void* eq_para;
}SKVPreprocessParam;
typedef struct RkaudioRxParam_
{
	/* Parameters of agc */
	int model_rx_en;
	void* anr_para;
}RkaudioRxParam;
/****************************************/
/*The param struct of sub-mudule of AEC,BF and RX*/
typedef struct RKAudioDereverbParam_
{
	int				   rlsLg;
	int                curveLg;
	int				   delay;
	float              forgetting;
	float              T60;
	float			   coCoeff;
} RKAudioDereverbParam;

typedef struct RKAudioAESParameter_ {
	float Beta_Up;
	float Beta_Down;
} RKAudioAESParameter;

typedef struct SKVNLPParameter_ {
	short int g_ashwAecBandNlpPara_16k[8][2];
} SKVNLPParameter;
typedef struct SKVANRParam_ {
	float noiseFactor;
	int swU;
	float PsiMin;
	float PsiMax;
	float fGmin;
} SKVANRParam;

typedef struct RKAGCParam_ {
	/* 新版AGC参数 */
	float              attack_time;  /* 触发时间，即AGC增益下降所需要的时间 */
	float			   release_time; /* 施放时间，即AGC增益上升所需要的时间 */
	float              max_gain; /* 最大增益，同时也是线性段增益，单位：dB */
	float 			   max_peak; /* 经AGC处理后，输出语音的最大能量，范围：单位：dB */
	float              fRth0;    /* 扩张段结束能量dB阈值，同时也是线性段开始阈值 */
	float              fRk0;     /* 扩张段斜率 */
	float              fRth1;    /* 压缩段起始能量dB阈值，同时也是线性段结束阈值 */

	/* 无效参数 */
	int            fs;                       /* 数据采样率 */
	int            frmlen;                   /* 处理帧长 */
	float          attenuate_time; /* 噪声衰减时间，即噪声段增益衰减到1所需的时间 */
	float          fRth2;                     /* 压缩段起始能量dB阈值 */
	float          fRk1;                      /* 扩张段斜率 */
	float          fRk2;                      /* 扩张段斜率 */
	float          fLineGainDb;               /* 线性段提升dB数 */
	int            swSmL0;                    /* 扩张段时域平滑点数 */
	int            swSmL1;                    /* 线性段时域平滑点数 */
	int            swSmL2;                    /* 压缩段时域平滑点数 */

} RKAGCParam;

typedef struct RKCNGParam_
{
	/*CNG Parameter*/
	float              fGain;                     /* INT16 Q0 施加舒适噪声幅度比例 */
	float              fMpy;						/* INT16 Q0 白噪随机数生成幅度 */
	float              fSmoothAlpha;              /* 舒适噪声平滑系数 */
	float              fSpeechGain;               /* 根据语音能量额外施加舒适噪声比例增益 */
} RKCNGParam;

typedef struct RKDTDParam_
{
	float ksiThd_high;			 /* 单双讲判决阈值 */
	float ksiThd_low;			 /* 单双讲判决阈值 */

}RKDTDParam;

typedef struct RKaudioEqParam_ {
    int shwParaLen;           // 滤波器系数个数
    short pfCoeff[5][13];          // 滤波器系数
} RKaudioEqParam;


/* Set the Sub-Para which used to initialize the Dereverb*/
inline static void * rkaudio_dereverb_param_init(){
	RKAudioDereverbParam* param = (RKAudioDereverbParam*)malloc(sizeof(RKAudioDereverbParam));
	param->rlsLg = 4; /* RLS滤波器阶数 */
	param->curveLg = 10; /* 分布曲线阶数 */
	param->delay = 2; /* RLS滤波器延时 */
	param->forgetting = 0.98; /* RLS滤波器遗忘因子 */
	param->T60 = 0.5;//1.5; /* 混响时间估计值（单位：s），越大，去混响能力越强，但是越容易过消除 */
	param->coCoeff = 1.0; /* 互相干性调整系数，防止过消除，越大能力越强，建议取值：0.5到2之间 */
	return (void*)param;
}
inline static void* rkaudio_aes_param_init() {
	RKAudioAESParameter* param = (RKAudioAESParameter*)malloc(sizeof(RKAudioAESParameter));
	//param->Beta_Up = 0.001f; /* 上升速度 */
	//param->Beta_Down = 0.005f; /* 下降速度 */
	param->Beta_Up = 0.005f; /* 上升速度 */
	param->Beta_Down = 0.001f; /* 下降速度 */
	//param->Beta_Up = 0.0005f; /* 上升速度 */
	//param->Beta_Down = 0.01f; /* 下降速度 */
	return (void*)param;
}
/* Set the Sub-Para which used to initialize the NLP*/
inline static void* rkaudio_nlp_param_init()
{
	SKVNLPParameter* param = (SKVNLPParameter*)malloc(sizeof(SKVNLPParameter));
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			param->g_ashwAecBandNlpPara_16k[i][j] = ashwAecBandNlpPara_16k[i][j];
		}
	}
	return (void*)param;
}
/* Set the Sub-Para which used to initialize the ANR*/
inline static void *rkaudio_anr_param_init() {
	SKVANRParam* param = (SKVANRParam*)malloc(sizeof(SKVANRParam));
	/* anr parameters */
	param->noiseFactor = 0.88;
	param->swU = 10;					  //2
	//param->PsiMin = 0.2;				//0.05
	//param->PsiMax = 0.316;			  //0.516
	//param->fGmin = 0.1;					 //0.1
	param->PsiMin = 0.01;				//0.05
	param->PsiMax = 0.516;			  //0.516
	param->fGmin = 0.05;					 //0.1
	return (void*)param;
}
/* Set the Sub-Para which used to initialize the AGC*/
inline static void* rkaudio_agc_param_init()
{
	RKAGCParam* param = (RKAGCParam*)malloc(sizeof(RKAGCParam));

	/* 新版AGC参数 */
	param->attack_time = 200.0;		/* 触发时间，即AGC增益上升所需要的时间 */
	param->release_time = 200.0;	/* 施放时间，即AGC增益下降所需要的时间 */
	//param->max_gain = 35.0;		/* 最大增益，同时也是线性段增益，单位：dB */
	param->max_gain = 20;			/* 最大增益，同时也是线性段增益，单位：dB */
	param->max_peak = -1.0;			/* 经AGC处理后，输出语音的最大能量，范围：单位：dB */
	param->fRk0 = 2;				/* 扩张段斜率 */
	param->fRth2 = -35;				/* 压缩段起始能量dB阈值，同时也是线性段结束阈值，增益逐渐降低，注意 fRth2 + max_gain < max_peak */
	param->fRth1 = -40;				/* 扩张段结束能量dB阈值，同时也是线性段开始阈值，能量高于改区域以max_gain增益 */
	param->fRth0 = -45;				/* 噪声门阈值 */

	/* 无效参数 */
	param->fs = 16000;                       /* 数据采样率 */
	param->frmlen = 256;                   /* 处理帧长 */
	param->attenuate_time = 1000; /* 噪声衰减时间，即噪声段增益衰减到1所需的时间 */
	param->fRk1 = 0.8;                      /* 扩张段斜率 */
	param->fRk2 = 0.4;                      /* 扩张段斜率 */
	param->fLineGainDb = -25.0f;               /* 低于该值，起始的attenuate_time(ms)内不做增益 */
	param->swSmL0 = 40;                    /* 扩张段时域平滑点数 */
	param->swSmL1 = 80;                    /* 线性段时域平滑点数 */
	param->swSmL2 = 80;                    /* 压缩段时域平滑点数 */

	return (void*)param;
}
/* Set the Sub-Para which used to initialize the CNG*/
inline static void* rkaudio_cng_param_init()
{
	RKCNGParam* param = (RKCNGParam*)malloc(sizeof(RKCNGParam));
	/* cng paremeters */
	param->fSmoothAlpha = 0.99f;										            /* INT16 Q15 施加舒适噪声平滑度 */
	param->fSpeechGain = 0;										                /* INT16 Q15 施加舒适噪声语音纹理模拟程度 */
	param->fGain = 20.0;                                           /* INT16 Q0 施加舒适噪声幅度比例 */
	param->fMpy = 20;                                            /* INT16 Q0 白噪随机数生成幅度 */
	return (void*)param;
}
/* Set the Sub-Para which used to initialize the DTD*/
inline static void* rkaudio_dtd_param_init()
{
	RKDTDParam* param = (RKDTDParam*)malloc(sizeof(RKDTDParam));
	/* dtd paremeters*/
	param->ksiThd_high = 0.60f;										            /* 单双讲判决阈值 */
	param->ksiThd_low = 0.30f;
	return (void*)param;
}
/* Set the Sub-Para which used to initialize the AEC*/

inline static void* rkaudio_aec_param_init()
{
	SKVAECParameter* param = (SKVAECParameter*)malloc(sizeof(SKVAECParameter));
	param->pos = REF_POSITION;
	param->drop_ref_channel = NUM_DROP_CHANNEL;
	param->model_aec_en = 0;//param->model_aec_en = EN_DELAY;
	param->delay_len = 0;
	param->look_ahead = 0;
	param->Array_list = Array;
	return (void*)param;
}

inline static void * rkaudio_eq_param_init(){
	RKaudioEqParam* param = (RKaudioEqParam*)malloc(sizeof(RKaudioEqParam));
	param->shwParaLen = 65;
	int i, j;
	for (i = 0; i < 5; i++){
		for (j = 0; j < 13; j++) {
			param->pfCoeff[i][j] = EqPara_16k[i][j];
		}
	}
	return (void*)param;
}

/* Set the Sub-Para which used to initialize the BF*/
inline static void* rkaudio_preprocess_param_init()
{
	SKVPreprocessParam* param = (SKVPreprocessParam*)malloc(sizeof(SKVPreprocessParam));
	//param->model_bf_en = EN_Fastaec |EN_AES |EN_Wakeup;
	//param->model_bf_en = EN_Agc | EN_Anr | EN_Dereverberation;
	param->model_bf_en = EN_Fastaec | EN_AES | EN_Anr | EN_Agc | EN_GSC | EN_Dereverberation;
	//param->model_bf_en = EN_Wakeup ;
	//param->model_bf_en = EN_Fix;
	//param->model_bf_en = EN_Anr | EN_GSC |EN_Agc;
	//param->model_bf_en = EN_Fastaec | EN_AES | EN_Anr | EN_Agc;
	param->Targ = 4;
	param->ref_pos = REF_POSITION;
	param->num_ref_channel = NUM_REF_CHANNEL;
	param->drop_ref_channel = NUM_DROP_CHANNEL;
	param->dereverb_para = rkaudio_dereverb_param_init();
	param->aes_para = rkaudio_aes_param_init();
	param->nlp_para = rkaudio_nlp_param_init();
	param->anr_para = rkaudio_anr_param_init();
	param->agc_para = rkaudio_agc_param_init();
	param->cng_para = rkaudio_cng_param_init();
	param->dtd_para = rkaudio_dtd_param_init();
	param->eq_para = rkaudio_eq_param_init();
	return (void*)param;
}
/* Set the Sub-Para which used to initialize the RX*/
inline static void* rkaudio_rx_param_init()
{
	RkaudioRxParam* param = (RkaudioRxParam*)malloc(sizeof(RkaudioRxParam));
	param->anr_para = rkaudio_anr_param_init();
	param->model_rx_en = EN_RX_Anr;
	return (void*)param;
}
typedef struct RKAUDIOParam_
{
	int model_en;
	void *aec_param;
	void *bf_param;
	void* rx_param;
} RKAUDIOParam;

inline static void rkaudio_aec_param_destory(void* param_)
{
	SKVAECParameter* param = (SKVAECParameter*)param_;
	free(param); param = NULL;
}

inline static void rkaudio_preprocess_param_destory(void* param_)
{
	SKVPreprocessParam* param = (SKVPreprocessParam*)param_;
	free(param->dereverb_para); param->dereverb_para = NULL;
	free(param->aes_para); param->aes_para = NULL;
	free(param->anr_para); param->anr_para = NULL;
	free(param->agc_para); param->agc_para = NULL;
	free(param->nlp_para); param->nlp_para = NULL;
	free(param->cng_para); param->cng_para = NULL;
	free(param->dtd_para); param->dtd_para = NULL;
	free(param->eq_para); param->eq_para = NULL;
	free(param); param = NULL;
}

inline static void rkaudio_rx_param_destory(void* param_)
{
	RkaudioRxParam* param = (RkaudioRxParam*)param_;
	free(param->anr_para); param->anr_para = NULL;
	free(param); param = NULL;
}

inline static void rkaudio_param_deinit(void* param_)
{
	RKAUDIOParam* param = (RKAUDIOParam*)param_;
	rkaudio_aec_param_destory(param->aec_param);
	rkaudio_preprocess_param_destory(param->bf_param);
	rkaudio_rx_param_destory(param->rx_param);
}

void *rkaudio_preprocess_init(int rate, int bits, int src_chan, int ref_chan, RKAUDIOParam *param);
void rkaudio_param_printf(int src_chan, int ref_chan, RKAUDIOParam* param);
int rkaudio_Doa_invoke(void* st_ptr);
int rkaudio_Cir_Doa_invoke(void* st_ptr, int* ang_doa, int* pth_doa);
int rkaudio_preprocess_get_cmd_id(void* st_ptr, float* asr_score, int* cmd_id);

void rkaudio_preprocess_destory(void *st_ptr);

int rkaudio_preprocess_short(void *st_ptr, short *in, short *out, int in_size, int *is_wakeup);
int rkaudio_rx_short(void* st_ptr, short* in, short* out);
int rkaudio_preprocess_key_short(void *st_ptr, short *in, short *out, int in_size);
int rkaudio_preprocess_command_short(void *st_ptr, short *in, int in_size, float *asr_scor);
void rkaudio_asr_set_param(float min, float max, float keep);

void rkaudio_param_deinit(void* param_);
#ifdef __cplusplus
}
#endif

#endif    // _RKAUDIO_PREPROCESS_H_
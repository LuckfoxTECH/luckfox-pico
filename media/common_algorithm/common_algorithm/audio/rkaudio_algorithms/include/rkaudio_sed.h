/* Copyright (C)  RK
Written by Ryne 
Date : 20221214  v1.2.1*/


#ifndef RKAUDIO_SED_H
#define RKAUDIO_SED_H

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JUMP_FRAME 20

#define FIR_EXAMPLE_LENC 4

static float FIR_EXAMPLE_COEFFS[FIR_EXAMPLE_LENC] = { -0.429928160597777, - 0.816685462483343,	0.816685462483343,	0.429928160597777 };

	typedef struct RKSEDAGCParam_ 
	{
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
	} RKSEDAGCParam;

	inline static RKSEDAGCParam* rkaudio_sedagc_param_init()
	{
		RKSEDAGCParam* param = (RKSEDAGCParam*)malloc(sizeof(RKSEDAGCParam));
		
		param->attack_time = 200.0;		
		param->release_time = 400.0;	
		param->max_gain = 30.0;		
		param->max_peak = -3.0;			
		param->fRk0 = 2;				
		param->fRth2 = -40;				
		param->fRth1 = -45;				
		param->fRth0 = -70;		
		param->fs = 16000;                     
		param->frmlen = 256;                  
		param->attenuate_time = 1000; 
		param->fRk1 = 0.8;                     
		param->fRk2 = 0.4;                   
		param->fLineGainDb = -25.0f;             
		param->swSmL0 = 40;                  
		param->swSmL1 = 80;                    
		param->swSmL2 = 80;                   
		return param;
	}


	typedef struct RKFIRParam_ {
		int fir_len; 
		float* fir_coeffs;
	} RKFIRParam;

	inline static RKFIRParam* rkaudio_fir_param_init()
	{
		RKFIRParam* param = (RKFIRParam*)malloc(sizeof(RKFIRParam));

		param->fir_len = FIR_EXAMPLE_LENC;
		param->fir_coeffs = FIR_EXAMPLE_COEFFS;
		return param;
	}

	typedef struct SedAedParam_
	{
		float	snr_db; 	 
		float	lsd_db; 	 
		int 	policy; 	
		float smooth_param; 
	} SedAedParam;
	
	typedef struct SedParam_
	{
		int 	frm_len;		
		int    nclass;         
		int    babycry_decision_len;    
		int    buzzer_decision_len;     
		int    glassbreaking_decision_len;
		float    babycry_confirm_prob;      
		float    buzzer_confirm_prob;    
		float    glassbreaking_confirm_prob;   
	} SedParam;

	typedef struct RKAudioSedRes_ 
	{
		int snr_res;
		int lsd_res;
		int bcd_res;
		int buz_res;
		int gbs_res;
	} RKAudioSedRes;

	typedef enum RKAudioSedType_ {
		SED_TYPE_BCD = 1 << 0,
		SED_TYPE_BUZ = 1 << 1,
		SED_TYPE_GBS = 1 << 2,
	} RKAudioSedType;

	typedef enum RKAudioSedEnable_
	{
		EN_AGC = 1 << 0,
		EN_AED = 1 << 1,
		EN_SED = 1 << 2,
		EN_FIR = 1 << 3,
	} RKAudioSedEnable;

	typedef struct RKAudioSedParam_
	{
		int 	      model_en;
		RKSEDAGCParam* agc_param;
		SedAedParam	 *aed_param;
		SedParam* sed_param;
		RKFIRParam* fir_param;
	} RKAudioSedParam;

	static SedAedParam *rkaudio_sed_param_aed()
	{
		SedAedParam* param = (SedAedParam *)calloc(sizeof(SedAedParam), 1);
		param->snr_db	 = 10;
		param->lsd_db	 = -35;
		param->policy	 = 1;
		param->smooth_param = 0.9;
		return param;
	}
	
	static SedParam* rkaudio_sed_param()
	{
		SedParam* param = (SedParam*)malloc(sizeof(SedParam));
		param->frm_len = 90;
		param->nclass = 1;
		param->babycry_decision_len = 60;
		param->buzzer_decision_len = 100;
		param->glassbreaking_decision_len = 30;
		param->babycry_confirm_prob = 0.85;
		param->buzzer_confirm_prob = 0.5;
		param->glassbreaking_confirm_prob = 0.98;
		return param;
	}

	inline static RKAudioSedParam *rkaudio_sed_param_init()
	{
		RKAudioSedParam *param = (RKAudioSedParam *)calloc(sizeof(RKAudioSedParam), 1);

		param->model_en  = EN_AGC | EN_AED |  EN_SED | EN_FIR;
		param->agc_param = rkaudio_sedagc_param_init();
		param->aed_param = rkaudio_sed_param_aed();
		param->sed_param = rkaudio_sed_param();
		param->fir_param = rkaudio_fir_param_init();
		return param;
	}

	inline static void rkaudio_sed_param_destroy(RKAudioSedParam *param)
	{
		if (param == NULL)
			return;
		if (param->agc_param)
		{
			free(param->agc_param);
			param->agc_param = NULL;
		}
		if (param->aed_param)
		{
			free(param->aed_param);
			param->aed_param = NULL;
		}
		if (param->sed_param)
		{
			free(param->sed_param);
			param->sed_param = NULL;
		}
		if (param->fir_param)
		{
			free(param->fir_param);
			param->fir_param = NULL;
		}
		free(param);
	}

	void *rkaudio_sed_init(int fs, int bit, int chan, RKAudioSedParam *param);

	char rkaudio_sed_init_res(void* st_);

	void rkaudio_sed_destroy(void *st_);

	int rkaudio_sed_process(void *st_, short *in, int in_size, RKAudioSedRes *res);

	float rkaudio_sed_lsd_db(void *st_);

	int rkaudio_sed_param_set(void* st_, void* param, int type);

	int rkaudio_sed_bcd_model_set(char* sed_rknn_path);
#ifdef __cplusplus
}
#endif

/** @}*/
#endif

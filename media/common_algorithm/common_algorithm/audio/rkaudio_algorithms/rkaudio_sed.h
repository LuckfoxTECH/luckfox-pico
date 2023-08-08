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
		/* 新版AGC参数 */
		param->attack_time = 100.0;		/* 触发时间，即AGC增益上升所需要的时间 */
		param->release_time = 200.0;	/* 施放时间，即AGC增益下降所需要的时间 */
		param->max_gain = 15.0;		/* 最大增益，同时也是线性段增益，单位：dB */
		param->max_peak = -3.0;			/* 经AGC处理后，输出语音的最大能量，范围：单位：dB */
		param->fRk0 = 2;				/* 扩张段斜率 */
		param->fRth2 = -25;				/* 压缩段起始能量dB阈值，同时也是线性段结束阈值，注意 fRth2 + max_gain < max_peak */
		param->fRth1 = -45;				/* 扩张段结束能量dB阈值，同时也是线性段开始阈值 */
		param->fRth0 = -70;				/* 噪声门阈值 */
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
		return param;
	}

	typedef struct SedAedParam_
	{
		float	snr_db; 	 // 信噪比大于snr输出1，单位为db
		float	lsd_db; 	 // 响度大于db值输出1, 最高为0db
		int 	policy; 	 // vad灵敏度，0—>2， 灵敏度等级提升。默认为1.
	} SedAedParam;

	// 声音事件检测
	typedef struct SedParam_
	{
		int 	frm_len;		 // 统计帧长  建议长度：110-150
		int    nclass;          // 类别数目
		int    babycry_decision_len;     // 哭声确认帧长
		int    buzzer_decision_len;     //蜂鸣器确认帧长
		int    glassbreaking_decision_len;  //玻璃破碎声确认帧长
	} SedParam;

	typedef struct RKAudioSedRes_
	{
		int snr_res;
		int lsd_res;
		int bcd_res;
		int buz_res;
		int gbs_res;
	} RKAudioSedRes;

	typedef enum RKAudioSedEnable_
	{
		EN_AGC = 1 << 0,
		EN_AED = 1 << 1,
		EN_SED = 1 << 2,
	} RKAudioSedEnable;

	typedef struct RKAudioSedParam_
	{
		int 	      model_en;
		RKSEDAGCParam* agc_param;
		SedAedParam	 *aed_param;
		SedParam* sed_param;
	} RKAudioSedParam;

	static SedAedParam *rkaudio_sed_param_aed()
	{
		SedAedParam* param = (SedAedParam *)calloc(sizeof(SedAedParam), 1);
		param->snr_db	 = 10;
		param->lsd_db	 = -35;
		param->policy	 = 1;
		return param;
	}

	static SedParam* rkaudio_sed_param()
	{
		SedParam* param = (SedParam*)malloc(sizeof(SedParam));
		param->frm_len = 125;
		param->nclass = 3;
		param->babycry_decision_len = 100;
		param->buzzer_decision_len = 100;
		param->glassbreaking_decision_len = 30;
		return param;
	}

	inline static RKAudioSedParam *rkaudio_sed_param_init()
	{
		RKAudioSedParam *param = (RKAudioSedParam *)calloc(sizeof(RKAudioSedParam), 1);

		param->model_en  = EN_AGC | EN_AED |  EN_SED ;
		param->agc_param = rkaudio_sedagc_param_init();
		param->aed_param = rkaudio_sed_param_aed();
		param->sed_param = rkaudio_sed_param();
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
		free(param);
	}

	void *rkaudio_sed_init(int fs, int bit, int chan, RKAudioSedParam *param);
	char rkaudio_sed_init_res(void* st_);
	void rkaudio_sed_destroy(void *st_);
	int rkaudio_sed_process(void *st_, short *in, int in_size, RKAudioSedRes *res);
	float rkaudio_sed_lsd_db(void *st_);

#ifdef __cplusplus
}
#endif

/** @}*/
#endif

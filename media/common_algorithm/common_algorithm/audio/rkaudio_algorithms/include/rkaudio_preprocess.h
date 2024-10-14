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
	//static short Array[NUM_SRC_CHANNEL] = { 9,7,5,3,2,4,6,8 }
	//static short Array[NUM_CHANNEL] = {2, 3, 0, 1}; //src first, ref second
	//static short Array[NUM_CHANNEL] = { 0, 1, 3, 2 };// , 4, 5 };
	//static short Array[NUM_CHANNEL] = { 10, 3, 11, 2, 4, 13, 5, 12, 0, 1, 6, 7, 8, 9, 14, 15};
	static short Array[NUM_CHANNEL] = { 6, 7, 2, 3, 4, 5, 0, 1};
	//static short Array[NUM_CHANNEL] = { 4, 13, 5, 12, 10, 3, 11, 2, 0, 1, 6, 7, 8, 9 ,14, 15};//8mic + 8ref,drop last 6ref

	//Array[NUM_SRC_CHANNEL] = { 2,3,4,5,6,7};
	/**********************EQ Parameter**********************/
	static short EqPara_16k[5][13] =
	{
		// filter_bank_0
		{-1 ,-2 ,-1 ,0 ,0 ,-3 ,-4 ,-1 ,0 ,-3 ,-6 ,-4 ,-1 },
		// filter_bank_1
		{-1 ,-4 ,-5 ,-6 ,-10 ,-9 ,-3 ,-3 ,-11 ,-12 ,2 ,12 ,0 },
		// filter_bank_2
		{-16 ,-11 ,2 ,-1 ,-11 ,-8 ,-4 ,-10 ,-6 ,13 ,7 ,-40 ,-62 },
		// filter_bank_3
		{-17 ,23 ,-22 ,-83 ,-57 ,6 ,-14 ,-92 ,-126 ,-141 ,-200 ,-197 ,-54 },
		// filter_bank_4
		{-8 ,-249 ,-390 ,10 ,428 ,-142 ,-1341 ,-1365 ,208 ,664 ,-2836 ,-8715 ,32764 },
	};

	/**********************AES Parameter**********************/
	static float LimitRatio[2][3] = {
		/* low freq   median freq    high freq*/
		{   1.2f,        1.2f,          1.2f  },  //limit
		{   1.2f,        1.2f,          1.2f  },  //ratio
	};

	/**********************THD Parameter**********************/
	static short ThdSplitFreq[4][2] = {
		{ 500,1000},//For low frequency
		{ 1000,2400},
		{ 2400,4000},
		{ 4000,8000},
	};

	static float ThdSupDegree[4][10] =
	{
		/* 2th		3th		4th		5th		6th		7th		8th		9th		10th	11th order  */
		/*
		{0.005f, 0.005f,	0,		0,		0,		0,		0,		0,		0,		0},
		{ 0.005f, 0.005f,   0.005f,      0,		0,		0,		0,		0,		0,		0},
		{ 0.005f, 0.005f, 0.005f,   0.005f, 0,	0,	0,0,		0,		0,},
		{ 0.003f, 0.003f, 0.004f,	0.005f, 0.003f, 0.003f, 0.003f,	0,		0,		0},
		*/
		/* 2th		3th		4th		5th		6th		7th		8th		9th		10th	11th order  */
		{ 0.005f, 0.005f,	0,		0,		0,		0,		0,		0,		0,		0},
		{ 0.005f, 0.005f,   0.005f,      0,		0,		0,		0,		0,		0,		0},
		{ 0.005f, 0.005f, 0.005f,   0.005f, 0,	0,	0,0,		0,		0,},
		{ 0.003f, 0.003f, 0.002f,	0.002f, 0.002f, 0.002f, 0.002f,	0,		0,		0},
	};
	static short HardSplitFreq[5][2] = {
		{ 500,2500},	//1 to 4 is select hard suppress freq bin
		{ 0,0},
		{ 0,0},
		{ 0,0},
		{ 2000,4000},//freq use to calculate mean_G
	};
	static float HardThreshold[4] = { 0.35,0.15, 0.25, 0.15 };

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
		EN_ARRAY_RESET = 1 << 1,
	} RKAecEnable;
	typedef enum RKPreprocessEnable_
	{
		EN_Fastaec = 1 << 0,
		EN_Wakeup = 1 << 1,
		EN_Dereverberation = 1 << 2,
		EN_Nlp = 1 << 3,
		EN_AES = 1 << 4,
		EN_Agc = 1 << 5,
		EN_Anr = 1 << 6,
		EN_GSC = 1 << 7,
		GSC_Method = 1 << 8,
		EN_Fix = 1 << 9,
		EN_STDT = 1 << 10,
		EN_CNG = 1 << 11,
		EN_EQ = 1 << 12,
		EN_CHN_SELECT = 1 << 13,
		EN_HOWLING = 1 << 14,
		EN_DOA = 1 << 15,
		EN_WIND = 1 << 16,
		EN_AINR = 1 << 17,
	} RKPreprocessEnable;
	typedef enum RkaudioRxEnable_
	{
		EN_RX_Anr = 1 << 0,
		EN_RX_HOWLING = 1 << 1,
		EN_RX_AGC = 1 << 2,
	} RkaudioRxEnable;
	/*****************************************/

	/* Set the three Main Para which used to initialize the AEC,BF and RX*/
	typedef struct SKVAECParameter_ {
		int pos;
		int drop_ref_channel;
		int model_aec_en;
		int delay_len;
		int look_ahead;
		short * Array_list;
		//mdf
		short filter_len;
		//delay
		void* delay_para;
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
		void* howl_para;
		void* doa_para;
	}SKVPreprocessParam;

	typedef struct RkaudioRxParam_
	{
		/* Parameters of agc */
		int model_rx_en;
		void* anr_para;
		void* howl_para;
		void* agc_para;
	}RkaudioRxParam;
	/****************************************/
	/*The param struct of sub-mudule of AEC,BF and RX*/
	typedef struct RKAudioDelayParam_ {
		short	MaxFrame;
		short	LeastDelay;
		short	JumpFrame;
		short	DelayOffset;
		short	MicAmpThr;
		short	RefAmpThr;
		short	StartFreq;
		short	EndFreq;
		float	SmoothFactor;
	}RKAudioDelayParam;

	typedef struct SKVANRParam_ {
		float noiseFactor;
		int   swU;
		float PsiMin;
		float PsiMax;
		float fGmin;

		short Sup_Freq1;
		short Sup_Freq2;
		float Sup_Energy1;
		float Sup_Energy2;

		short InterV;
		float BiasMin;
		short UpdateFrm;
		float NPreGammaThr;
		float NPreZetaThr;
		float SabsGammaThr0;
		float SabsGammaThr1;
		float InfSmooth;
		float ProbSmooth;
		float CompCoeff;
		float PrioriMin;
		float PostMax;
		float PrioriRatio;
		float PrioriRatioLow;
		int   SplitBand;
		float PrioriSmooth;
		//transient
		short TranMode;
	} SKVANRParam;

	typedef struct RKAudioDereverbParam_
	{
		int		rlsLg;
		int     curveLg;
		int		delay;
		float   forgetting;
		float   T60;
		float	coCoeff;
	} RKAudioDereverbParam;

	typedef struct RKAudioAESParameter_ {
		float	Beta_Up;
		float	Beta_Down;
		float	Beta_Up_Low;
		float	Beta_Down_Low;
		short	low_freq;
		short	high_freq;
		short	THD_Flag;
		short	HARD_Flag;
		float	LimitRatio[2][3];
		short	ThdSplitFreq[4][2];
		float	ThdSupDegree[4][10];
		short	HardSplitFreq[5][2];
		float	HardThreshold[4];
	} RKAudioAESParameter;

	typedef struct RKDTDParam_
	{
		float ksiThd_high;			 /* µ¥Ë«½²ÅÐ¾öãÐÖµ */
		float ksiThd_low;			 /* µ¥Ë«½²ÅÐ¾öãÐÖµ */

	}RKDTDParam;

	typedef struct SKVNLPParameter_ {
		short int g_ashwAecBandNlpPara_16k[8][2];
	} SKVNLPParameter;

	typedef struct RKAGCParam_ {
		/* ÐÂ°æAGC²ÎÊý */
		float              attack_time;  /* ´¥·¢Ê±¼ä£¬¼´AGCÔöÒæÏÂ½µËùÐèÒªµÄÊ±¼ä */
		float			   release_time; /* Ê©·ÅÊ±¼ä£¬¼´AGCÔöÒæÉÏÉýËùÐèÒªµÄÊ±¼ä */
		float              max_gain; /* ×î´óÔöÒæ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶ÎÔöÒæ£¬µ¥Î»£ºdB */
		float 			   max_peak; /* ¾­AGC´¦Àíºó£¬Êä³öÓïÒôµÄ×î´óÄÜÁ¿£¬·¶Î§£ºµ¥Î»£ºdB */
		float              fRth0;    /* À©ÕÅ¶Î½áÊøÄÜÁ¿dBãÐÖµ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶Î¿ªÊ¼ãÐÖµ */
		float              fRk0;     /* À©ÕÅ¶ÎÐ±ÂÊ */
		float              fRth1;    /* Ñ¹Ëõ¶ÎÆðÊ¼ÄÜÁ¿dBãÐÖµ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶Î½áÊøãÐÖµ */

		/* ÎÞÐ§²ÎÊý */
		int            fs;                       /* Êý¾Ý²ÉÑùÂÊ */
		int            frmlen;                   /* ´¦ÀíÖ¡³¤ */
		float          attenuate_time; /* ÔëÉùË¥¼õÊ±¼ä£¬¼´ÔëÉù¶ÎÔöÒæË¥¼õµ½1ËùÐèµÄÊ±¼ä */
		float          fRth2;                     /* Ñ¹Ëõ¶ÎÆðÊ¼ÄÜÁ¿dBãÐÖµ */
		float          fRk1;                      /* À©ÕÅ¶ÎÐ±ÂÊ */
		float          fRk2;                      /* À©ÕÅ¶ÎÐ±ÂÊ */
		float          fLineGainDb;               /* ÏßÐÔ¶ÎÌáÉýdBÊý */
		int            swSmL0;                    /* À©ÕÅ¶ÎÊ±ÓòÆ½»¬µãÊý */
		int            swSmL1;                    /* ÏßÐÔ¶ÎÊ±ÓòÆ½»¬µãÊý */
		int            swSmL2;                    /* Ñ¹Ëõ¶ÎÊ±ÓòÆ½»¬µãÊý */

	} RKAGCParam;

	typedef struct RKCNGParam_
	{
		/*CNG Parameter*/
		float              fGain;                     /* INT16 Q0 Ê©¼ÓÊæÊÊÔëÉù·ù¶È±ÈÀý */
		float              fMpy;						/* INT16 Q0 °×ÔëËæ»úÊýÉú³É·ù¶È */
		float              fSmoothAlpha;              /* ÊæÊÊÔëÉùÆ½»¬ÏµÊý */
		float              fSpeechGain;               /* ¸ù¾ÝÓïÒôÄÜÁ¿¶îÍâÊ©¼ÓÊæÊÊÔëÉù±ÈÀýÔöÒæ */
	} RKCNGParam;

	typedef struct RKaudioEqParam_ {
		int shwParaLen;           // ÂË²¨Æ÷ÏµÊý¸öÊý
		short pfCoeff[5][13];          // ÂË²¨Æ÷ÏµÊý
	} RKaudioEqParam;

	typedef struct RKHOWLParam_
	{
		short howlMode;
	}RKHOWLParam;
	typedef struct RKDOAParam_
	{
		float rad;//ÏßÕó2mic¼ä¾à£¬Ô²ÕóÔòÎª°ë¾¶£»ÕóÁÐ²»Ö§³ÖÖ¸¶¨£¬±ØÐë¸ù¾Ý¿â¶ø¶¨£¨±ÈÈç³öµÄÔ²Õó¿âÔòÖ»Ö§³ÖÔ²Õó¶¨Î»¡££©
		short start_freq;
		short end_freq;
		short lg_num;			//¸ÃÊýÖµÓ¦¸ÃÎªÅ¼Êý
		short lg_pitch_num;		//only used for circle array, linear array must be 1, ¸©Ñö½ÇÉ¨Ãè¡£
	}RKDOAParam;
	/*************** TX ***************/

	/* Set the Sub-Para which used to initialize the DELAY*/
	inline static void* rkaudio_delay_param_init() {
		/*RKAudioDelayParam* param = (RKAudioDelayParam*)malloc(sizeof(RKAudioDelayParam));*/
		RKAudioDelayParam* param = (RKAudioDelayParam*)calloc(1, sizeof(RKAudioDelayParam));
		param->MaxFrame = 32;		/* delay×î³¤¹À¼ÆÖ¡Êý */
		param->LeastDelay = 0;		/* delay×î¶Ì¹À¼ÆÖ¡Êý */
		param->JumpFrame = 12;		/* Ìø¹ýÖ¡Êý */
		param->DelayOffset = 1;		/* delay offsetÖ¡Êý */
		param->MicAmpThr = 50;		/* mic¶Ë×îÐ¡ÄÜÁ¿ãÐÖµ */
		param->RefAmpThr = 50;		/* ref¶Ë×îÐ¡ÄÜÁ¿ãÐÖµ */
		param->StartFreq = 1000;		/* ÑÓÊ±¹À¼ÆÆðÊ¼Æµ¶ÎµÄÆµÂÊ */
		param->EndFreq = 4000;		/* ÑÓÊ±¹À¼ÆÖÕÖ¹Æµ¶ÎµÄÆµÂÊ */
		param->SmoothFactor = 0.99f;
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the ANR*/
	inline static void* rkaudio_anr_param_init_tx() {
		/*SKVANRParam* param = (SKVANRParam*)malloc(sizeof(SKVANRParam));*/
		SKVANRParam* param = (SKVANRParam*)calloc(1, sizeof(SKVANRParam));
		/* anr parameters */
		param->noiseFactor = 0.88f;//-3588.0f to compatible old json
		//param->noiseFactor = -3588.0f;
		param->swU = 10;
		param->PsiMin = 0.02;
		param->PsiMax = 0.516;
		param->fGmin = 0.05;
		param->Sup_Freq1 = -3588;
		param->Sup_Freq2 = -3588;
		param->Sup_Energy1 = 10000;
		param->Sup_Energy2 = 10000;

		param->InterV = 8;				//ANR_NOISE_EST_V
		param->BiasMin = 1.67f;			//ANR_NOISE_EST_BMIN
		param->UpdateFrm = 15;			//UPDATE_FRAME
		param->NPreGammaThr = 4.6f;		//ANR_NOISE_EST_GAMMA0
		param->NPreZetaThr = 1.67f;		//ANR_NOISE_EST_PSI0
		param->SabsGammaThr0 = 1.0f;	//ANR_NOISE_EST_GAMMA2
		param->SabsGammaThr1 = 3.0f;	//ANR_NOISE_EST_GAMMA1
		param->InfSmooth = 0.8f;		//ANR_NOISE_EST_ALPHA_S
		param->ProbSmooth = 0.7f;		//ANR_NOISE_EST_ALPHA_D
		param->CompCoeff = 1.4f;		//ANR_NOISE_EST_BETA
		param->PrioriMin = 0.0316f;		//ANR_NOISE_EST_ESP_MIN
		param->PostMax = 40.0f;			//ANR_NOISE_EST_GAMMA_MAX
		param->PrioriRatio = 0.95f;		//ANR_NOISE_EST_ALPHA
		param->PrioriRatioLow = 0.95f;	//ANR_NOISE_EST_ALPHA
		param->SplitBand = 20;
		param->PrioriSmooth = 0.7f;		//ANR_ENHANCE_BETA

		//transient
		param->TranMode = 0;
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the Dereverb*/
	inline static void* rkaudio_dereverb_param_init() {
		/*RKAudioDereverbParam* param = (RKAudioDereverbParam*)malloc(sizeof(RKAudioDereverbParam));*/
		RKAudioDereverbParam* param = (RKAudioDereverbParam*)calloc(1, sizeof(RKAudioDereverbParam));
		param->rlsLg = 4;			/* RLSÂË²¨Æ÷½×Êý */
		param->curveLg = 30;		/* ·Ö²¼ÇúÏß½×Êý */
		param->delay = 2;			/* RLSÂË²¨Æ÷ÑÓÊ± */
		param->forgetting = 0.98;	/* RLSÂË²¨Æ÷ÒÅÍüÒò×Ó */
		param->T60 = 0.3;//1.5;		/* »ìÏìÊ±¼ä¹À¼ÆÖµ£¨µ¥Î»£ºs£©£¬Ô½´ó£¬È¥»ìÏìÄÜÁ¦Ô½Ç¿£¬µ«ÊÇÔ½ÈÝÒ×¹ýÏû³ý */
		param->coCoeff = 1;			/* »¥Ïà¸ÉÐÔµ÷ÕûÏµÊý£¬·ÀÖ¹¹ýÏû³ý£¬Ô½´óÄÜÁ¦Ô½Ç¿£¬½¨ÒéÈ¡Öµ£º0.5µ½2Ö®¼ä */
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the AES*/
	inline static void* rkaudio_aes_param_init() {
		/*RKAudioAESParameter* param = (RKAudioAESParameter*)malloc(sizeof(RKAudioAESParameter));*/
		RKAudioAESParameter* param = (RKAudioAESParameter*)calloc(1, sizeof(RKAudioAESParameter));
		//param->Beta_Up = 0.002f; /* ÉÏÉýËÙ¶È -3588.0f to compatible old json*/
		param->Beta_Up = 0.002f;
		param->Beta_Down = 0.001f; /* ÏÂ½µËÙ¶È */
		param->Beta_Up_Low = 0.002f; /* µÍÆµÉÏÉýËÙ¶È */
		param->Beta_Down_Low = 0.001f; /* µÍÆµÏÂ½µËÙ¶È */
		param->low_freq = 500;
		param->high_freq = 3750;
		param->THD_Flag = 1;	/* 1 open THD, 0 close THD */
		param->HARD_Flag = 1;	/* 1 open Hard Suppress, 0 close Hard Suppress */
		int i, j;
		for (i = 0; i < 2; i++)
			for (j = 0; j < 3; j++)
				param->LimitRatio[i][j] = LimitRatio[i][j];
		for (i = 0; i < 4; i++)
			for (j = 0; j < 2; j++)
				param->ThdSplitFreq[i][j] = ThdSplitFreq[i][j];
		for (i = 0; i < 4; i++)
			for (j = 0; j < 10; j++)
				param->ThdSupDegree[i][j] = ThdSupDegree[i][j];
		
		for (i = 0; i < 5; i++)
			for (j = 0; j < 2; j++)
				param->HardSplitFreq[i][j] = HardSplitFreq[i][j];
		for (i = 0; i < 4; i++)
			param->HardThreshold[i] = HardThreshold[i];
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the DTD*/
	inline static void* rkaudio_dtd_param_init()
	{
		/*RKDTDParam* param = (RKDTDParam*)malloc(sizeof(RKDTDParam));*/
		RKDTDParam* param = (RKDTDParam*)calloc(1, sizeof(RKDTDParam));
		/* dtd paremeters*/
		param->ksiThd_high = 0.60f;										            /* µ¥Ë«½²ÅÐ¾öãÐÖµ */
		param->ksiThd_low = 0.50f;
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the AGC*/
	inline static void* rkaudio_agc_param_init()
	{
		/*RKAGCParam* param = (RKAGCParam*)malloc(sizeof(RKAGCParam));*/
		RKAGCParam* param = (RKAGCParam*)calloc(1, sizeof(RKAGCParam));

		param->attack_time = 100.0;		/* 触发时间，即AGC增益上升所需要的时间 */
		param->release_time = 200.0;	/* 施放时间，即AGC增益下降所需要的时间 */
		//param->max_gain = 35.0;		/* 最大增益，同时也是线性段增益，单位：dB */
		param->max_gain = 30;			/* 最大增益，同时也是线性段增益，单位：dB */
		param->max_peak = -3.0;			/* 经AGC处理后，输出语音的最大能量，范围：单位：dB */
		param->fRk0 = 2;				/* 扩张段斜率 */
		param->fRth2 = -45;				/* 压缩段起始能量dB阈值，同时也是线性段结束阈值，增益逐渐降低，注意 fRth2 + max_gain < max_peak */
		param->fRth1 = -60;				/* 扩张段结束能量dB阈值，同时也是线性段开始阈值，能量高于改区域以max_gain增益 */
		param->fRth0 = -65;				/* 噪声门阈值 */

		/* ÎÞÐ§²ÎÊý */
		param->fs = 16000;                       /* Êý¾Ý²ÉÑùÂÊ */
		param->frmlen = 256;                   /* ´¦ÀíÖ¡³¤ */
		param->attenuate_time = 1000; /* ÔëÉùË¥¼õÊ±¼ä£¬¼´ÔëÉù¶ÎÔöÒæË¥¼õµ½1ËùÐèµÄÊ±¼ä */
		param->fRk1 = 0.8;                      /* À©ÕÅ¶ÎÐ±ÂÊ */
		param->fRk2 = 0.4;                      /* À©ÕÅ¶ÎÐ±ÂÊ */
		param->fLineGainDb = -25.0f;               /* µÍÓÚ¸ÃÖµ£¬ÆðÊ¼µÄattenuate_time(ms)ÄÚ²»×öÔöÒæ */
		param->swSmL0 = 1;                    /* use for AINR pre_gain */
		param->swSmL1 = 80;                    /* ÏßÐÔ¶ÎÊ±ÓòÆ½»¬µãÊý */
		param->swSmL2 = 80;                    /* Ñ¹Ëõ¶ÎÊ±ÓòÆ½»¬µãÊý */

		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the CNG*/
	inline static void* rkaudio_cng_param_init()
	{
		/*RKCNGParam* param = (RKCNGParam*)malloc(sizeof(RKCNGParam));*/
		RKCNGParam* param = (RKCNGParam*)calloc(1, sizeof(RKCNGParam));
		/* cng paremeters */
		param->fSmoothAlpha = 0.99f;										            /* INT16 Q15 Ê©¼ÓÊæÊÊÔëÉùÆ½»¬¶È */
		param->fSpeechGain = 0;										                /* INT16 Q15 Ê©¼ÓÊæÊÊÔëÉùÓïÒôÎÆÀíÄ£Äâ³Ì¶È */
		param->fGain = 10.0;                                           /* INT16 Q0 Ê©¼ÓÊæÊÊÔëÉù·ù¶È±ÈÀý */
		param->fMpy = 10;                                            /* INT16 Q0 °×ÔëËæ»úÊýÉú³É·ù¶È */
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the EQ*/
	inline static void* rkaudio_eq_param_init() {
		/*RKaudioEqParam* param = (RKaudioEqParam*)malloc(sizeof(RKaudioEqParam));*/
		RKaudioEqParam* param = (RKaudioEqParam*)calloc(1, sizeof(RKaudioEqParam));
		param->shwParaLen = 65;
		int i, j;
		for (i = 0; i < 5; i++) {
			for (j = 0; j < 13; j++) {
				param->pfCoeff[i][j] = EqPara_16k[i][j];
			}
		}
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the HOWL*/
	inline static void* rkaudio_howl_param_init_tx() {
		/*RKHOWLParam* param = (RKHOWLParam*)malloc(sizeof(RKHOWLParam));*/
		RKHOWLParam* param = (RKHOWLParam*)calloc(1, sizeof(RKHOWLParam));
		param->howlMode = 5;
		return (void*)param;
	}
	inline static void* rkaudio_doa_param_init() {
		/*RKDOAParam* param = (RKDOAParam*)malloc(sizeof(RKDOAParam));*/
		RKDOAParam* param = (RKDOAParam*)calloc(1, sizeof(RKDOAParam));
		param->rad = 0.04f;
		param->start_freq = 1000;
		param->end_freq = 4000;
		param->lg_num = 40;
		param->lg_pitch_num = 1; 
		return (void*)param;
	}
	/************* RX *************/
	inline static void* rkaudio_anr_param_init_rx() {
		/*SKVANRParam* param = (SKVANRParam*)malloc(sizeof(SKVANRParam));*/
		SKVANRParam* param = (SKVANRParam*)calloc(1, sizeof(SKVANRParam));
		/* anr parameters */
		param->noiseFactor = 0.88f;
		param->swU = 10;
		param->PsiMin = 0.02;
		param->PsiMax = 0.516;
		param->fGmin = 0.05;

		param->Sup_Freq1 = -3588;
		param->Sup_Freq2 = -3588;
		param->Sup_Energy1 = 100000;
		param->Sup_Energy2 = 100000;

		param->InterV = 8;				//ANR_NOISE_EST_V
		param->BiasMin = 1.67f;			//ANR_NOISE_EST_BMIN
		param->UpdateFrm = 15;			//UPDATE_FRAME
		param->NPreGammaThr = 4.6f;		//ANR_NOISE_EST_GAMMA0
		param->NPreZetaThr = 1.67f;		//ANR_NOISE_EST_PSI0
		param->SabsGammaThr0 = 1.0f;	//ANR_NOISE_EST_GAMMA2
		param->SabsGammaThr1 = 3.0f;	//ANR_NOISE_EST_GAMMA1
		param->InfSmooth = 0.8f;		//ANR_NOISE_EST_ALPHA_S
		param->ProbSmooth = 0.7f;		//ANR_NOISE_EST_ALPHA_D
		param->CompCoeff = 1.4f;		//ANR_NOISE_EST_BETA
		param->PrioriMin = 0.0316f;		//ANR_NOISE_EST_ESP_MIN
		param->PostMax = 40.0f;			//ANR_NOISE_EST_GAMMA_MAX
		param->PrioriRatio = 0.95f;		//ANR_NOISE_EST_ALPHA
		param->PrioriRatioLow = 0.95f;	//ANR_NOISE_EST_ALPHA
		param->SplitBand = 20;
		param->PrioriSmooth = 0.7f;		//ANR_ENHANCE_BETA

		//transient
		param->TranMode = 0;

		return (void*)param;
	}
	inline static void* rkaudio_howl_param_init_rx() {
		/*RKHOWLParam* param = (RKHOWLParam*)malloc(sizeof(RKHOWLParam));*/
		RKHOWLParam* param = (RKHOWLParam*)calloc(1, sizeof(RKHOWLParam));
		param->howlMode = 4;
		return (void*)param;
	}
	inline static void* rkaudio_agc_param_init_rx()
	{
		RKAGCParam* param = (RKAGCParam*)malloc(sizeof(RKAGCParam));

		/* ÐÂ°æAGC²ÎÊý */
		param->attack_time = 200.0;		/* ´¥·¢Ê±¼ä£¬¼´AGCÔöÒæÉÏÉýËùÐèÒªµÄÊ±¼ä */
		param->release_time = 200.0;	/* Ê©·ÅÊ±¼ä£¬¼´AGCÔöÒæÏÂ½µËùÐèÒªµÄÊ±¼ä */
		//param->max_gain = 35.0;		/* ×î´óÔöÒæ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶ÎÔöÒæ£¬µ¥Î»£ºdB */
		param->max_gain = 5.0;			/* ×î´óÔöÒæ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶ÎÔöÒæ£¬µ¥Î»£ºdB */
		param->max_peak = -1;			/* ¾­AGC´¦Àíºó£¬Êä³öÓïÒôµÄ×î´óÄÜÁ¿£¬·¶Î§£ºµ¥Î»£ºdB */
		param->fRk0 = 2;				/* À©ÕÅ¶ÎÐ±ÂÊ */
		param->fRth2 = -25;				/* Ñ¹Ëõ¶ÎÆðÊ¼ÄÜÁ¿dBãÐÖµ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶Î½áÊøãÐÖµ£¬ÔöÒæÖð½¥½µµÍ£¬×¢Òâ fRth2 + max_gain < max_peak */
		param->fRth1 = -35;				/* À©ÕÅ¶Î½áÊøÄÜÁ¿dBãÐÖµ£¬Í¬Ê±Ò²ÊÇÏßÐÔ¶Î¿ªÊ¼ãÐÖµ£¬ÄÜÁ¿¸ßÓÚ¸ÄÇøÓòÒÔmax_gainÔöÒæ */
		param->fRth0 = -45;				/* ÔëÉùÃÅãÐÖµ */

		/* ÎÞÐ§²ÎÊý */
		param->fs = 16000;                       /* Êý¾Ý²ÉÑùÂÊ */
		param->frmlen = 256;                   /* ´¦ÀíÖ¡³¤ */
		param->attenuate_time = 1000; /* ÔëÉùË¥¼õÊ±¼ä£¬¼´ÔëÉù¶ÎÔöÒæË¥¼õµ½1ËùÐèµÄÊ±¼ä */
		param->fRk1 = 0.8;                      /* À©ÕÅ¶ÎÐ±ÂÊ */
		param->fRk2 = 0.4;                      /* À©ÕÅ¶ÎÐ±ÂÊ */
		param->fLineGainDb = -25.0f;               /* µÍÓÚ¸ÃÖµ£¬ÆðÊ¼µÄattenuate_time(ms)ÄÚ²»×öÔöÒæ */
		param->swSmL0 = 40;                    /* À©ÕÅ¶ÎÊ±ÓòÆ½»¬µãÊý */
		param->swSmL1 = 80;                    /* ÏßÐÔ¶ÎÊ±ÓòÆ½»¬µãÊý */
		param->swSmL2 = 80;                    /* Ñ¹Ëõ¶ÎÊ±ÓòÆ½»¬µãÊý */

		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the AEC*/
	inline static void* rkaudio_aec_param_init()
	{
		/*SKVAECParameter* param = (SKVAECParameter*)malloc(sizeof(SKVAECParameter));*/
		SKVAECParameter* param = (SKVAECParameter*)calloc(1, sizeof(SKVAECParameter));
		param->pos = REF_POSITION;
		param->drop_ref_channel = NUM_DROP_CHANNEL;
		param->model_aec_en = 0;	//param->model_aec_en = EN_DELAY;
		param->delay_len = 0;	//-3588 to compatible old json
		//param->delay_len = -3588;
		param->look_ahead = 0;
		param->Array_list = Array;
		//mdf
		param->filter_len = 2;
		//delay
		param->delay_para = rkaudio_delay_param_init();
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the BF*/
	inline static void* rkaudio_preprocess_param_init()
	{
		/*SKVPreprocessParam* param = (SKVPreprocessParam*)malloc(sizeof(SKVPreprocessParam));*/
		SKVPreprocessParam* param = (SKVPreprocessParam*)calloc(1, sizeof(SKVPreprocessParam));
		//param->model_bf_en = EN_Fastaec;
		//param->model_bf_en = EN_AINR | EN_Anr | EN_Agc; // | EN_Wakeup | EN_WIND | EN_Dereverberation | EN_HOWLING;
		//param->model_bf_en = EN_Fastaec | EN_AES | EN_Dereverberation | EN_Agc;
		//param->model_bf_en = EN_Fix | EN_Agc | EN_AINR | EN_Dereverberation;
		param->model_bf_en = EN_DOA | EN_Fix | EN_AINR | EN_Anr ;
		//param->model_bf_en = EN_Wakeup;
		//param->model_bf_en = EN_Fastaec | EN_Fix | EN_Agc | EN_Anr;
		//param->model_bf_en = EN_Fastaec | EN_AES | EN_Agc | EN_Fix | EN_Anr | EN_HOWLING;
		//param->model_bf_en = EN_Fastaec | EN_AES | EN_Anr | EN_Agc;
		param->Targ = 2;
		param->ref_pos = REF_POSITION;
		param->num_ref_channel = NUM_REF_CHANNEL;
		param->drop_ref_channel = NUM_DROP_CHANNEL;
		param->anr_para = rkaudio_anr_param_init_tx();
		param->dereverb_para = rkaudio_dereverb_param_init();
		param->aes_para = rkaudio_aes_param_init();
		param->dtd_para = rkaudio_dtd_param_init();
		param->agc_para = rkaudio_agc_param_init();
		param->cng_para = rkaudio_cng_param_init();
		param->eq_para = rkaudio_eq_param_init();
		param->howl_para = rkaudio_howl_param_init_tx();
		param->doa_para = rkaudio_doa_param_init();
		return (void*)param;
	}
	/* Set the Sub-Para which used to initialize the RX*/
	inline static void* rkaudio_rx_param_init()
	{
		//RkaudioRxParam* param = (RkaudioRxParam*)malloc(sizeof(RkaudioRxParam));
		RkaudioRxParam* param = (RkaudioRxParam*)calloc(1, sizeof(RkaudioRxParam));
		param->model_rx_en = EN_RX_AGC | EN_RX_Anr | EN_RX_HOWLING;
		param->anr_para = rkaudio_anr_param_init_rx();
		param->howl_para = rkaudio_howl_param_init_rx();
		param->agc_para = rkaudio_agc_param_init_rx();
		return (void*)param;
	}
	typedef struct RKAUDIOParam_
	{
		int model_en;
		void* aec_param;
		void* bf_param;
		void* rx_param;
		int read_size;
	} RKAUDIOParam;

	inline static void rkaudio_aec_param_destory(void* param_)
	{
		SKVAECParameter* param = (SKVAECParameter*)param_;
		free(param->delay_para); param->delay_para = NULL;
		free(param); param = NULL;
	}

	inline static void rkaudio_preprocess_param_destory(void* param_)
	{
		SKVPreprocessParam* param = (SKVPreprocessParam*)param_;
		free(param->dereverb_para); param->dereverb_para = NULL;
		free(param->aes_para); param->aes_para = NULL;
		free(param->anr_para); param->anr_para = NULL;
		free(param->agc_para); param->agc_para = NULL;
		param->nlp_para = NULL;
		free(param->cng_para); param->cng_para = NULL;
		free(param->dtd_para); param->dtd_para = NULL;
		free(param->eq_para); param->eq_para = NULL;
		free(param->howl_para); param->howl_para = NULL;
		free(param->doa_para); param->doa_para = NULL;
		free(param); param = NULL;
	}

	inline static void rkaudio_rx_param_destory(void* param_)
	{
		RkaudioRxParam* param = (RkaudioRxParam*)param_;
		free(param->anr_para); param->anr_para = NULL;
		free(param->howl_para); param->howl_para = NULL;
		free(param); param = NULL;
	}

	inline static void rkaudio_param_deinit(void* param_)
	{
		RKAUDIOParam* param = (RKAUDIOParam*)param_;
		if (param->aec_param != NULL)
			rkaudio_aec_param_destory(param->aec_param);
		if (param->bf_param != NULL)
			rkaudio_preprocess_param_destory(param->bf_param);
		if (param->rx_param != NULL)
			rkaudio_rx_param_destory(param->rx_param);
	}

	void* rkaudio_preprocess_init(int rate, int bits, int src_chan, int ref_chan, RKAUDIOParam* param);
	void rkaudio_param_printf(int src_chan, int ref_chan, RKAUDIOParam* param);
	int rkaudio_Doa_invoke(void* st_ptr);
	int rkaudio_Cir_Doa_invoke(void* st_ptr, int* ang_doa, int* pth_doa);

	int rkaudio_preprocess_get_cmd_id(void* st_ptr, float* cmd_score, int* cmd_id);

	int rkaudio_preprocess_get_asr_id(void* st_ptr, float* asr_score, int* asr_id);

	int rkaudio_param_set(void* st_ptr, int rkaudio_enable, int rkaec_enable, int rkbf_enable);

	void rkaudio_preprocess_destory(void* st_ptr);

	int rkaudio_preprocess_short(void* st_ptr, short* in, short* out, int in_size, int* wakeup_status);
	int rkaudio_rx_short(void* st_ptr, short* in, short* out);

	int rkaudio_mdf_dump(void* st_ptr, short* out);

	void rkaudio_asr_set_param(float min, float max, float keep);
	int rkaudio_rknn_path_set(char* asr_rknn_path_, char* kws_rknn_path_, char* dns_rknn_path_);
	void rkaudio_param_deinit(void* param_);
#ifdef __cplusplus
}
#endif

#endif    // _RKAUDIO_PREPROCESS_H_
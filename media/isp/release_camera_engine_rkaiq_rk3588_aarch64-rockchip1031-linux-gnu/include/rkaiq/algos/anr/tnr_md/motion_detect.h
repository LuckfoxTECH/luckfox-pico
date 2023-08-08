#ifndef _MOTION_DETECT_H
#define _MOTION_DETECT_H

#ifndef WIN32
#include "arm_neon.h"
#else
#include "NEON_2_SSE.h"
#endif

//#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "common.h"

#define		SIGN(a)								(((a) > 0) ? (1) : (-(1)))
//#define     MIN(a,b)                            ((a) <= (b) ? (a):(b))
//#define		MAX(a,b)							((a) >= (b) ? (a):(b))
#define		CLIP(a, min_v, max_v)			    (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))
#define		ABS(a)								(((a) > 0) ? (a) : (-(a)))
//#define		ROUND_F(x)                      	(int32_t)(((float)x)+(((x) > 0) ? 0.5 : (-0.5)))
#define		ROUND_INT(x, shf_bit)           	(int)((((x) > 0) ? 1 : -1) * ((ABS(x) + (1<<((shf_bit)-1)))>>(shf_bit)))
#define		ROUND_UINT16(x, shf_bit)  			((uint16_t)		(MIN(x, (1 << (uint32_t)16) - 1) + (1<<(shf_bit - 1))) >> (shf_bit))


#define		IMG_DOWN_SCALE_X_BIT				2
#define		IMG_DOWN_SCALE_Y_BIT				3

#define		IMG_DOWN_SCALE_X					(1 << IMG_DOWN_SCALE_X_BIT)
#define		IMG_DOWN_SCALE_Y					(1 << IMG_DOWN_SCALE_Y_BIT)
#define		LDIFF_DOWN_SCALE_X_BIT				3
#define		LDIFF_DOWN_SCALE_Y_BIT				3
#define		LDIFF_DOWN_SCALE_X					(1 << LDIFF_DOWN_SCALE_X_BIT)
#define		LDIFF_DOWN_SCALE_Y					(1 << LDIFF_DOWN_SCALE_Y_BIT)


#define		GAUS_FILTER_RADIUS_X				1
#define		GAUS_FILTER_RADIUS_Y				1
#define		SIGMA_FILTER_RADIUS_X				4
#define		SIGMA_FILTER_RADIUS_Y				4

#define		ALPHA_FIX_BITS						7
#define		UV_RATIO_FIX_BITS					7
#define		ALPHA_DIV_FIX_BITS					(7 + 7)

#define 	RATIO_BITS_NUM 						7
#define 	RATIO_BITS_R_NUM 					5



#define		MT_VERSION 							1
#define		ENABLE_NEON

void motion_detect(
	uint8_t *pCurIn,
	uint8_t *pPreIn,
	int16_t *pTmpBuf,
	uint8_t *pAlpha,
	uint8_t *pPreAlpha,
	uint8_t *gain_table_u8,
	int		imgHeight,
	int		imgWidth,
	int		proHeight,
	int		proWidth,
	int     gainStride,
	float	sigmaHScale,
	float	sigmaLScale,
	float	uv_ratio,
	float	light_clip,
	int 	static_ratio_r_bit);


#endif

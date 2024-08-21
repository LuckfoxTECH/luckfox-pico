#ifndef _MOTION_DETECT_H
#define _MOTION_DETECT_H

#if defined(__linux__)
#include <sys/stat.h>
#include<unistd.h>
#elif defined(_WIN32)
#include "direct.h"
#include "io.h"
#endif
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

#if defined(__linux__)
#define     ACCESS(a, b)                    access(a, b)
#define     MKDIR(a, b)                     mkdir(a, b )
#define     GETCWD(a,b)                     getcwd(a,b)
#define     fseek64(fp, offset, type)       fseeko64(fp, offset, type);
#define     ftell64(fp)                     ftello64(fp)
#elif defined(_WIN32)
#define     ACCESS(a, b)                    _access(a, b)
#define     MKDIR(a, b)                     _mkdir(a )
#define     GETCWD(a,b)                     _getcwd(a,b)
#define     fseek64(fp, offset, type)       _fseeki64(fp, offset, type) ;
#define     ftell64(fp)                     _ftelli64(fp)
#else
#define     ACCESS(a, b)                    1
#define     MKDIR(a, b)
#define     GETCWD(a,b)
#define     fseek64(fp, offset, type)
#define     ftell64(fp)
#endif
#define     SIGN(a)                             (((a) > 0) ? (1) : (-(1)))
#ifndef MIN
#define     MIN(a,b)                            ((a) <= (b) ? (a):(b))
#endif
#ifndef MAX
#define     MAX(a,b)                            ((a) >= (b) ? (a):(b))
#endif
#define     CLIP(a, min_v, max_v)               (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))
#define     ABS(a)                              (((a) > 0) ? (a) : (-(a)))
#ifndef ROUND_F
#define     ROUND_F(x)                          (int32_t)(((float)x)+(((x) > 0) ? 0.5 : (-0.5)))
#endif
#define     ROUND_INT(x, shf_bit)               (int)((((x) > 0) ? 1 : -1) * ((ABS(x) + (1<<((shf_bit)-1)))>>(shf_bit)))
#define     ROUND_UINT16(x, shf_bit)            ((uint16_t)     (MIN(x, (1 << (uint32_t)16) - 1) + (1<<(shf_bit - 1))) >> (shf_bit))
#define     ROUND(x)                            (int)(((x) >= 0)?((x) + 0.5) : ((x) - 0.5))

#define     MT_ALIGN_STEP                       16
#define     MT_ALIGN_STEP_AND                   (0xffff - (MT_ALIGN_STEP - 1))

#define     MT_ALIGN_STEP_Y                     8
#define     MT_ALIGN_STEP_Y_AND                 (0xffff - (MT_ALIGN_STEP_Y - 1))


#define     IMG_DOWN_SCALE_X_BIT                2
#define     IMG_DOWN_SCALE_Y_BIT                3

#define     IMG_DOWN_SCALE_X                    (1 << IMG_DOWN_SCALE_X_BIT)
#define     IMG_DOWN_SCALE_Y                    (1 << IMG_DOWN_SCALE_Y_BIT)
#define     LDIFF_DOWN_SCALE_X_BIT              3
#define     LDIFF_DOWN_SCALE_Y_BIT              3
#define     LDIFF_DOWN_SCALE_X                  (1 << LDIFF_DOWN_SCALE_X_BIT)
#define     LDIFF_DOWN_SCALE_Y                  (1 << LDIFF_DOWN_SCALE_Y_BIT)


#define     GAUS_FILTER_RADIUS_X                1
#define     GAUS_FILTER_RADIUS_Y                1
#define     SIGMA_FILTER_RADIUS_X               4
#define     SIGMA_FILTER_RADIUS_Y               4

#define     ALPHA_FIX_BITS                      7

#define     UV_RATIO_FIX_BITS                   7
#define     ALPHA_DIV_FIX_BITS                  (7 + 7)
#define     ALPHA_MAX_VAL                       (1 << ALPHA_FIX_BITS)
#define     ALPHA_MIN_VAL                       0

#define     SIGMA_FIX_BITS                      5
#define     SIGMA_DIV_FIX_BITS                  8
#define     DIFF_L_RATIO_FIX_BITS               6
#define     UV_RATIO_DIV_FIX_BITS               6
#define     RATIO_BITS_NUM                      7
#define     RATIO_BITS_R_NUM                    5

#define     YUV_SCALE_FIX_BITS                  5
#define     GAIN_SCALE_MOTION_BITS              5

#define     GAIN_MIN_VAL                        1
#define     JMJ_TEST_OUT
#define     JMJ_TEST1
#define     DS_ORI_FLG                          0

#define     SIGMA_CONV_FLG                      0
#define     ENABLE_NEON

typedef struct RKAnr_Mt_Params_Select_t
{
    uint32_t enable;
    uint32_t gain_upt_flg;
    float sigmaHScale;
    float sigmaLScale;
    float uv_weight;
    float mfnr_sigma_scale;
    float mfnr_gain_scale;
    float motion_dn_str;

    float yuvnr_gain_scale[4];
    float yuvnr_gain_scale_glb;
    float yuvnr_gain_th;

    float md_sigma_scale;

    float frame_limit_y;
    float frame_limit_uv;
    float gain;
    float gain_ratio;
    double noise_curve[6];
    float threshold[4];
    uint32_t med_en;
    uint32_t imgHeight;
    uint32_t imgWidth;
	uint32_t proHeight;
	uint32_t proWidth;
	uint32_t imgStride;

	uint32_t gainHeight;
	uint32_t gainWidth;
	uint32_t gainStride;

	uint32_t img_ds_size_x;//4
	uint32_t img_ds_size_y;//8
	uint32_t static_ratio_r_bit;
    uint32_t wr_flg;
    uint32_t wr_flg_last;
//
    float reserved[128];

    uint32_t static_flg;
    uint32_t static_crc;
    float gain_ratio_cur;
    float gain_ratio_last;

} RKAnr_Mt_Params_Select_t;
void motion_detect(
    uint8_t *pCurIn,
    uint8_t *pPreIn,
    int16_t *pTmpBuf,
    uint8_t *pAlpha,
    uint8_t *pPreAlpha,
    uint8_t *gain_table_u8,
    RKAnr_Mt_Params_Select_t *mParams);


#endif

/*
 *  Copyright (c) 2019 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef _RK_AIQ_COMM_H_
#define _RK_AIQ_COMM_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef  __cplusplus
#define RKAIQ_BEGIN_DECLARE  extern "C" {
#define RKAIQ_END_DECLARE    }
#else
#define RKAIQ_BEGIN_DECLARE
#define RKAIQ_END_DECLARE
#endif

RKAIQ_BEGIN_DECLARE

typedef int RESULT;

typedef enum RKAiqResult_e {
    RK_AIQ_RET_SUCCESS              = 0,   // this has to be 0, if clauses rely on it
    RK_AIQ_RET_FAILURE              = 1,   // process failure
    RK_AIQ_RET_INVALID_PARM     = 2,   // invalid parameter
    RK_AIQ_RET_WRONG_CONFIG     = 3,   // feature not supported
    RK_AIQ_RET_BUSY             = 4,   // there's already something going on...
    RK_AIQ_RET_CANCELED         = 5,   // operation canceled
    RK_AIQ_RET_OUTOFMEM         = 6,   // out of memory
    RK_AIQ_RET_OUTOFRANGE           = 7,   // parameter/value out of range
    RK_AIQ_RET_NULL_POINTER     = 8,   // the/one/all parameter(s) is a(are) NULL pointer(s)
    RK_AIQ_RET_DIVISION_BY_ZERO = 9,   // a divisor equals ZERO
    RK_AIQ_RET_NO_INPUTIMAGE        = 10   // no input image
} RKAiqResult_t;

typedef enum RKAiqState_e {
    RK_AIQ_STATE_INVALID           = 0,                   /**< initialization value */
    RK_AIQ_STATE_INITIALIZED       = 1,                   /**< instance is created, but not initialized */
    RK_AIQ_STATE_STOPPED           = 2,                   /**< instance is confiured (ready to start) or stopped */
    RK_AIQ_STATE_RUNNING           = 3,                   /**< instance is running (processes frames) */
    RK_AIQ_STATE_LOCKED            = 4,                   /**< instance is locked (for taking snapshots) */
    RK_AIQ_STATE_MAX                                      /**< max */
} RKAiqState_t;

typedef enum RKAiqOPMode_e {
    RK_AIQ_OP_MODE_INVALID           = 0,                   /**< initialization value */
    RK_AIQ_OP_MODE_AUTO               = 1,                   /**< instance is created, but not initialized */
    RK_AIQ_OP_MODE_MANUAL             = 2,                   /**< instance is confiured (ready to start) or stopped */
    RK_AIQ_OP_MODE_MAX                                      /**< max */
} RKAiqOPMode_t;

#define ABS(a) (((a) > 0) ? (a) : (-(a)))
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
#define ROUND_D(x) (long)(((double)x)+(((x) > 0) ? 0.5 : (-0.5)))
#define ROUND_F(x) (int)(((float)x)+(((x) > 0) ? 0.5 : (-0.5)))
#define FLOOR(a)   (int)( ((double)(a) < (int)(a)) ? (int)((a)-1) : (int)(a) )
#define FLOOR_INT64(a) (long)( ((double)(a) < (long)(a)) ? (long)((a)-1) : (long)(a) )
#define INTERP1(x0, x1, ratio)  ((ratio) * ((x1) - (x0)) + x0)
#define CLIPBIT(a,b) ((a)>((1<<(b))-1)?((1<<(b))-1):(a))
#define SWAP(_T_,A,B)                   { _T_ tmp = (A); (A) = (B); (B) = tmp; }
#define MIN2(a, b) ((a) > (b) ? (b) : (a))
#define CLIP(a, min_v, max_v)               (((a) < (min_v)) ? (min_v) : (((a) > (max_v)) ? (max_v) : (a)))
#define ROUND_INT(x, shf_bit)               (int)((((x) > 0) ? 1 : -1) * ((ABS(x) + (1<<((shf_bit)-1)))>>(shf_bit)))
#define LOG2(x)                             (log((double)x)                 / log((double)2))


#define RETURN_RESULT_IF_DIFFERENT( cur_res, exp_res ) if ( exp_res != cur_res ) { return ( cur_res ); }
#define DCT_ASSERT(exp) ((void)0)

#ifndef __FLT_EPSILON__
#define __FLT_EPSILON__     0.000000119209289550781250000000
#endif /* __FLT_EPSILON__ */

#ifndef FLT_EPSILON
#define FLT_EPSILON         __FLT_EPSILON__
#endif /* FLT_EPSILON */


#define DIVMIN 0.00001

typedef unsigned int        uint32_t;
typedef int                 sint32_t;
typedef char                sint8_t;
typedef unsigned long long  uint64t;


/******************************************************************************/
/**
 * @brief   A structure to represent a 5x5 matrix.
 *
 *          The 25 values are laid out as follows (zero based index):
 *
 *               | 00 01 02 03 04 | \n
 *               | 05 06 07 08 09 | \n
 *               | 10 11 12 13 14 | \n
 *               | 15 16 17 18 19 | \n
 *               | 20 21 22 23 24 | \n
 *
 * @note    The 25 values are represented as unsigned char numbers.
 *
 *****************************************************************************/
typedef struct Cam5x5UCharMatrix_s {
    uint8_t uCoeff[5 * 5];            /**< array of 5x5 unsigned char values */
} Cam5x5UCharMatrix_t;

typedef struct Cam15x15UCharMatrix_s {
    uint8_t uCoeff[15 * 15];            /**< array of 15x15 unsigned char values */
} Cam15x15UCharMatrix_t;


typedef struct Cam1x3IntMatrix_s
{
    // M4_ARRAY_DESC("Coeff", "s32", M4_SIZE(1,3), M4_RANGE(-65535,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int Coeff[3];
} Cam1x3IntMatrix_t;

typedef struct Cam1x4IntMatrix_s
{
    int Coeff[4];
} Cam1x4IntMatrix_t;

typedef struct Cam1x6IntMatrix_s
{
    int Coeff[6];
} Cam1x6IntMatrix_t;

typedef struct Cam1x8IntMatrix_s
{
    int Coeff[8];
} Cam1x8IntMatrix_t;

typedef struct Cam1x9IntMatrix_s
{
    int Coeff[9];
} Cam1x9IntMatrix_t;

typedef struct Cam1x12IntMatrix_s
{
    int Coeff[12];
} Cam1x12IntMatrix_t;
typedef struct Cam1x17IntMatrix_s
{
    int Coeff[17];
} Cam1x17IntMatrix_t;

typedef struct Cam2x3IntMatrix_s
{
    int Coeff[2 * 3];
} Cam2x3IntMatrix_t;

typedef struct Cam3x3IntMatrix_s
{
    int Coeff[3 * 3];
} Cam3x3IntMatrix_t;

typedef struct Cam5x5IntMatrix_s
{
    int Coeff[5 * 5];
} Cam5x5IntMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam1x1FloatMatrix_s {
    float fCoeff[1];
} Cam1x1FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | 2 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam1x3FloatMatrix_s {
    // M4_ARRAY_DESC("fCoeff", "f32", M4_SIZE(1,3), M4_RANGE(-65535,65535), "0", M4_DIGIT(6), M4_DYNAMIC(0))
    float fCoeff[3];
} Cam1x3FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | ... | 4 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam1x4FloatMatrix_s {
    // M4_ARRAY_DESC("fCoeff", "f32", M4_SIZE(1,4), M4_RANGE(-65535,65535), "0", M4_DIGIT(6), M4_DYNAMIC(0))
    float fCoeff[4];
} Cam1x4FloatMatrix_t;

typedef struct Cam1x5FloatMatrix_s
{
    float fCoeff[5];
} Cam1x5FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | ... | 6 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam1x6FloatMatrix_s {
    float fCoeff[6];
} Cam1x6FloatMatrix_t;

typedef struct Cam1x8FloatMatrix_s
{
    float fCoeff[8];
} Cam1x8FloatMatrix_t;

typedef struct Cam1x9FloatMatrix_s
{
    float fCoeff[9];
} Cam1x9FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | ... | 15 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam1x16FloatMatrix_s {
    float fCoeff[16];
} Cam1x16FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam2x1FloatMatrix {
    // M4_ARRAY_DESC("fCoeff", "f32", M4_SIZE(1,2), M4_RANGE(-65535,65535), "0", M4_DIGIT(6), M4_DYNAMIC(0))
    float fCoeff[2];
} Cam2x1FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 |
 *          | 2 | 3 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam2x2FloatMatrix {
    float fCoeff[4];
} Cam2x2FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 |  2 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam3x1FloatMatrix {
    float fCoeff[3];
} Cam3x1FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 |  2 |
 *          | 3 | 4 |  5 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam3x2FloatMatrix_s {
    float fCoeff[6];
} Cam3x2FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 |  2 |
 *          | 3 | 4 |  5 |
 *          | 6 | 7 |  8 |
 *
 * @note    Coefficients are represented as float numbers
 */
/*****************************************************************************/
typedef struct Cam3x3FloatMatrix_s {
    float fCoeff[9];
} Cam3x3FloatMatrix_t;

/******************************************************************************/
/**
 * @brief   A structure to represent a 5x5 matrix.
 *
 *          The 25 values are laid out as follows (zero based index):
 *
 *               | 00 01 02 03 04 | \n
 *               | 05 06 07 08 09 | \n
 *               | 10 11 12 13 14 | \n
 *               | 15 16 17 18 19 | \n
 *               | 20 21 22 23 24 | \n
 *
 * @note    The 25 values are represented as float numbers.
 *
 *****************************************************************************/
typedef struct Cam5x5FloatMatrix_s {
    float fCoeff[25U];              /**< array of 5x5 float values */
} Cam5x5FloatMatrix_t;


/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          |   0 |   1 |   2 |   3 |   4 |   5 |   6 |   7 | ....
 *          |  17 |  18 |  19 |  20 |  21 |  22 |  23 |  24 | ....
 *          |  34 |  35 |  36 |  37 |  38 |  39 |  40 |  41 | ....
 *          ...
 *          ...
 *          ...
 *          | 271 | 272 | 273 | 274 | 275 | 276 | 277 | 278 | .... | 288 |
 *
 * @note    Coefficients are represented as short numbers
 */
/*****************************************************************************/
typedef struct Cam17x17FloatMatrix_s {
    float fCoeff[17 * 17];
} Cam17x17FloatMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | 2 |
 *
 * @note    Coefficients are represented as short numbers
 */
/*****************************************************************************/
typedef struct Cam1x3ShortMatrix_s {
    // M4_ARRAY_DESC("fCoeff", "s16", M4_SIZE(1,3), M4_RANGE(-65535,65535), "0", M4_DIGIT(0), M4_DYNAMIC(0))
    int16_t Coeff[3];
} Cam1x3ShortMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | 2 | ... | 4 |
 *
 * @note    Coefficients are represented as short numbers
 */
/*****************************************************************************/
typedef struct Cam1x4UShortMatrix_s {
    uint16_t uCoeff[4];
} Cam1x4UShortMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          | 0 | 1 | 2 | ... | 16 |
 *
 * @note    Coefficients are represented as short numbers
 */
/*****************************************************************************/
typedef struct Cam1x17UShortMatrix_s {
    uint16_t uCoeff[17];
} Cam1x17UShortMatrix_t;

/*****************************************************************************/
/**
 * @brief   Matrix coefficients
 *
 *          |   0 |   1 |   2 |   3 |   4 |   5 |   6 |   7 | ....
 *          |  17 |  18 |  19 |  20 |  21 |  22 |  23 |  24 | ....
 *          |  34 |  35 |  36 |  37 |  38 |  39 |  40 |  41 | ....
 *          ...
 *          ...
 *          ...
 *          | 271 | 272 | 273 | 274 | 275 | 276 | 277 | 278 | .... | 288 |
 *
 * @note    Coefficients are represented as short numbers
 */
/*****************************************************************************/
typedef struct Cam17x17UShortMatrix_s {
    // M4_ARRAY_DESC("uCoeff", "u16", M4_SIZE(17,17), M4_RANGE(0,10000), "1024", M4_DIGIT(0), M4_DYNAMIC(0))
    uint16_t uCoeff[17 * 17];
} Cam17x17UShortMatrix_t;

typedef enum {
    RK_AIQ_WORKING_MODE_NORMAL,
    RK_AIQ_WORKING_MODE_ISP_HDR2    = 0x10,
    RK_AIQ_WORKING_MODE_ISP_HDR3    = 0x20,
//    RK_AIQ_WORKING_MODE_SENSOR_HDR = 10, // sensor built-in hdr mode
} rk_aiq_working_mode_t;

typedef enum {
    RK_AIQ_ISP_HDR_MODE_2_FRAME_HDR = RK_AIQ_WORKING_MODE_ISP_HDR2 + 1,
    RK_AIQ_ISP_HDR_MODE_2_LINE_HDR = RK_AIQ_WORKING_MODE_ISP_HDR2 + 2,
    RK_AIQ_ISP_HDR_MODE_3_FRAME_HDR = RK_AIQ_WORKING_MODE_ISP_HDR3 + 1,
    RK_AIQ_ISP_HDR_MODE_3_LINE_HDR = RK_AIQ_WORKING_MODE_ISP_HDR3 + 2,
} rk_aiq_isp_hdr_mode_t;

typedef enum {
    RKAIQ_SENSOR_HDR_MODE_DCG, // 2frame: share the same exptime, use dual conversion gain; 3frame: DCG+VS, VS frame use individual gain & time
    RKAIQ_SENSOR_HDR_MODE_STAGGER, // 2frame or 3frame
} rk_aiq_sensor_hdr_line_mode_t;

#define RK_AIQ_HDR_GET_WORKING_MODE(mode) (mode & 0xF0)

typedef enum {
    RKAIQ_ISPP_TNR_MODE_2TO1,
    RKAIQ_ISPP_TNR_MODE_3TO1,
} rkaiq_ispp_tnr_mode_t;

typedef enum {
    RKAIQ_ISPP_FEC_MODE_STABLE,
    RKAIQ_ISPP_FEC_MODE_FISHEYE,
} rkaiq_ispp_fec_mode_t;

typedef enum {
    RK_MODULE_INVAL = 0,
    RK_MODULE_DPCC,
    RK_MODULE_BLS,
    RK_MODULE_LSC,
    RK_MODULE_AWB_GAIN,
    RK_MODULE_CTK,
    RK_MODULE_GOC,
    RK_MODULE_SHARP,
    RK_MODULE_AE,
    RK_MODULE_AWB,
    RK_MODULE_NR,//10
    RK_MODULE_GIC,
    RK_MODULE_3DLUT,
    RK_MODULE_LDCH,
    RK_MODULE_TNR,//14
    RK_MODULE_FEC,
    RK_MODULE_RAWNR,//16
    RK_MODULE_MAX
} rk_aiq_module_id_t;

typedef enum
{
    RK_AIQ_BAYER_INVALID    = -1,
    RK_AIQ_BAYER_BGGR       = 0,
    RK_AIQ_BAYER_GBRG       = 1,
    RK_AIQ_BAYER_GRBG       = 2,
    RK_AIQ_BAYER_RGGB       = 3,
} RkAiqBayerPattern_t;

typedef enum {
    RK_AIQ_UAPI_MODE_DEFAULT = 0,
    RK_AIQ_UAPI_MODE_SYNC,
    RK_AIQ_UAPI_MODE_ASYNC
} rk_aiq_uapi_mode_sync_e;

/*
 * @sync_mode (param in): flags for param update mode,
 *   @setAttrib:
 *     RK_AIQ_UAPI_MODE_DEFAULT:      default is sync mode.
 *     RK_AIQ_UAPI_MODE_SYNC:         sync mode.
 *     RK_AIQ_UAPI_MODE_ASYNC:        async mode.
 *   @getAttrib:
 *     RK_AIQ_UAPI_MODE_DEFAULT: default to get the attrib that was
 *       set(sync_mode == RK_AIQ_UAPI_MODE_ASYNC) last time, which
 *       may not be in effect yet.
 *     RK_AIQ_UAPI_MODE_SYNC:    get the attrib currently in use.
 *     RK_AIQ_UAPI_MODE_ASYNC:   the same as RK_AIQ_UAPI_MODE_DEFAULT.
 *
 * @done (parsm out): flags for param update status,
 *     true indicate param has been updated, false
 *     indicate param has not been updated.
 */
typedef struct rk_aiq_uapi_sync_s {
    rk_aiq_uapi_mode_sync_e     sync_mode;
    bool                        done;
} rk_aiq_uapi_sync_t;

extern int g_rkaiq_isp_hw_ver;

#define CHECK_ISP_HW_V20() \
    (g_rkaiq_isp_hw_ver == 20 ? true : false)

#define CHECK_ISP_HW_V21() \
    (g_rkaiq_isp_hw_ver == 21 ? true : false)

#define CHECK_ISP_HW_V30() \
    (g_rkaiq_isp_hw_ver == 30 ? true : false)

#define CHECK_ISP_HW_V32() \
    (g_rkaiq_isp_hw_ver == 32 ? true : false)

#define CHECK_ISP_HW_V3X() \
    (g_rkaiq_isp_hw_ver == 30 ? true : \
     g_rkaiq_isp_hw_ver == 31 ? true : false)

RKAIQ_END_DECLARE

#endif

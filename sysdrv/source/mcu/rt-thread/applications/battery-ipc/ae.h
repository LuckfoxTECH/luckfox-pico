/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @version V0.0.1
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-07-07     ISP Team      first implementation
  *
  ******************************************************************************
  */

#ifndef _ISP_AE_H_
#define _ISP_AE_H_

typedef uint32_t AE_RESULT;
typedef int32_t FIXED;
typedef int64_t LFIXED;

#define AE_RET_SUCCESS                  0   //!< this has to be 0, if clauses rely on it
#define AE_RET_FAILURE                  1   //!< general failure
#define AE_RET_NOTSUPP                  2   //!< feature not supported
#define AE_RET_BUSY                     3   //!< there's already something going on...
#define AE_RET_CANCELED                 4   //!< operation canceled
#define AE_RET_OUTOFMEM                 5   //!< out of memory
#define AE_RET_OUTOFRANGE               6   //!< parameter/value out of range
#define AE_RET_IDLE                     7   //!< feature/subsystem is in idle state
#define AE_RET_WRONG_HANDLE             8   //!< handle is wrong
#define AE_RET_NULL_POINTER             9   //!< the/one/all parameter(s) is a(are) NULL pointer(s)
#define AE_RET_NOTAVAILABLE            10   //!< profile not available
#define AE_RET_DIVISION_BY_ZERO        11   //!< a divisor equals ZERO
#define AE_RET_WRONG_STATE             12   //!< state machine in wrong state
#define AE_RET_INVALID_PARM            13   //!< invalid parameter
#define AE_RET_PENDING                 14   //!< command pending
#define AE_RET_WRONG_CONFIG            15   //!< given configuration is invalid
#define AE_RET_SOC_AF                  16   //!< SOC AF   zyh@rock-chips.com: v0.0x20.0

#define AE_GAIN_RANGE_NUM               10
#define AE_TIME_FACTOR_NUM              4
#define AE_DOT_NO                       10  // maximum support for 10 dots
#define START_EXP_NO                    3
#define AE_HDR_EXP_NUM                  3
#define AE_DB_GAIN_RANGE_NUM            13
#define RAWAEBIG_SUBWIN_NUM             4
#define ADC_CALIB_NO                    4
#define ADC_TABLE_LEN                   15
#define ENV_CALIB_NO                    2
#define EXP_CALIB_NO                    3
#define AE_EXP_GRID_ITEMS_NUM           225

#define ISP2X_MIPI_LUMA_MEAN_MAX        16
#define ISP2X_MIPI_RAW_MAX              3
#define ISP2X_RAW0_Y_STATE              (1 << 0)
#define ISP2X_RAW1_Y_STATE              (1 << 1)
#define ISP2X_RAW2_Y_STATE              (1 << 2)

#define FIXED_FLT_EPSILON               1  // minimum precision limit
#define FIXEDPREC                       16 // Fixed Point Number, 16 decimal places
#define FIXED_INT_PART                  0xFFFF0000 // integer part
#define FIXED_FRA_PART                  0x0000FFFF // fractional part
#define INT2FIX(i)                      (((int32_t) (i)) << FIXEDPREC)
#define FLOAT2FIX(i)                    (int32_t)((i) * (1 << FIXEDPREC))
#define FLOAT2LFIX(i)                   (int64_t)((i) * (1 << FIXEDPREC))
#define FIX2INT(i)                      (int32_t)((i) >> FIXEDPREC)
#define FIX2FRA(i)                      ((int32_t)((i) & FIXED_FRA_PART) * 10000) >> FIXEDPREC
#define FIX_RINT(i)                     ((i + (1 << (FIXEDPREC - 1))) >> FIXEDPREC) // floor(i+0.5)
#define FIX_ADD(a,b)                    ((a) + (b))
#define FIX_DIF(a,b)                    ((a) - (b))
#define FIX_NEG(a)                      (-(a))
#define FIX_ONE                         INT2FIX(1)

#define FIX_MUL(a,b)                    ((FIXED)(((LFIXED)(a)) * ((LFIXED)(b)) >> FIXEDPREC))
#define FIX_LMUL(a,b)                   ((LFIXED)(((LFIXED)(a)) * ((LFIXED)(b)) >> FIXEDPREC))
#define FIX_RMUL(a,b)                   ((FIXED)((((LFIXED)(a)) * ((LFIXED)(b)) + (FIX_ONE >> 1)) >> FIXEDPREC))
#define FIX_DIV(a,b)                    ((FIXED)((((LFIXED)(a)) << FIXEDPREC) / (LFIXED)(MAX(b, FIXED_FLT_EPSILON))))
#define FIX_LDIV(a,b)                   ((LFIXED)((((LFIXED)(a)) << FIXEDPREC) / (LFIXED)(MAX(b, FIXED_FLT_EPSILON))))
#define FIX_POW(a,b)                    (b == INT2FIX(-1) ? FIX_DIV(FIX_ONE, a) : a)
#define FIX_ABS(a,b)                    (a > b ? (a-b) : (b-a))

typedef enum IR_STATUS_e {
    IR_STATUS_CUT_OFF    = 0,
    IR_STATUS_CUT_ON     = 1,
} IR_STATUS_t;

typedef enum DayORNight_STATUS_e {
    DayORNight_STATUS_INVALID  = 0,
    DayORNight_STATUS_DAY      = 1,
    DayORNight_STATUS_NIGHT    = 2,
} DayORNight_STATUS_t;

typedef enum ALS_TYPE_e {
    ALS_TYPE_NONE        = 0,
    ALS_TYPE_ANALOG      = 1,
    ALS_TYPE_DIGITAL     = 2,
} ALS_TYPE_t;

typedef enum night_mode_e
{
    NIGHT_OFF = 0,
    NIGHT_WHITE_AUTO = 1,
    NIGHT_IR_AUTO = 2,
    NIGHT_WHITE_ON = 3,
    NIGHT_IR_ON = 4
} night_mode_t;

typedef enum color_mode_e
{
    COLOR_MODE = 0,
    BW_MODE = 1
} color_mode_t;

typedef enum AE_ExpGainMode_e
{
    AEC_EXPGAIN_MODE_LINEAR         = 0,
    AEC_EXPGAIN_MODE_NONLINEAR_DB   = 1
} AE_ExpGainMode_t;

typedef enum dcg_mode_e
{
    DCG_MODE_NORMAL         = 0,
    DCG_MODE_LCG            = 1,
    DCG_MODE_HCG            = 2
} dcg_mode_t;

struct rk_real_exp_val
{
    FIXED time[AE_HDR_EXP_NUM];
    FIXED gain[AE_HDR_EXP_NUM];
    FIXED isp_gain[AE_HDR_EXP_NUM];
};

struct smart_ir_cfg
{
    FIXED d2n_envL_th;
    FIXED n2d_envL_th;
    FIXED rggain_base;
    FIXED bggain_base;
    FIXED awbgain_rad;
    FIXED awbgain_dis;
};

struct fastae_info
{
    //aiq exp params
    struct rk_camera_exp_val reg_exp_aiq;
    struct rk_real_exp_val exp_aiq;
    //quick start exp params
    struct rk_camera_exp_val reg_exp;
    struct rk_real_exp_val exp;

    FIXED new_exposure;
    FIXED current_exposure;
    uint32_t is_match;
    uint32_t is_over_range;

    //soft lux params
    struct isp_awbgain_cfg awbgain;
    DayORNight_STATUS_t day_or_night;
    IR_STATUS_t ir_state;
    uint32_t use_old_exp;

    //info
    FIXED mean_luma;

    //aiq auto fps
    uint32_t dst_vts;

    struct rkisp_stats_buffer *stats;
};

struct fastae_gain_range
{
    FIXED range_min;
    FIXED range_max;
    FIXED C1;
    FIXED C0;
    FIXED M0;
    FIXED minReg;
    FIXED maxReg;
};

struct fastae_time_range
{
    uint32_t reg_min;
    uint32_t reg_max[2];
    FIXED sumreg_fac[2];
    uint32_t odevity_fac[2];
};

struct fastae_dcg_cfg
{
    bool support_en;
    bool dcg_optype;
    bool sync_switch;
    dcg_mode_t dcg_mode[3];
    FIXED dcg_ratio;
    FIXED lcg2hcg_gain_th;
    FIXED hcg2lcg_gain_th;
};

struct fastae_init_info
{
    uint32_t fps;
    uint32_t hts;
    uint32_t vts;
    uint32_t fps_aiq;
    uint32_t hts_aiq;
    uint32_t vts_aiq;

    bool optype;
    FIXED manual_gain[3];
    FIXED manual_time[3];
    FIXED manual_ispgain[3];

    FIXED black_lvl;
    FIXED exp_level[AE_DOT_NO];
    FIXED setpoint[AE_DOT_NO];
    FIXED time_dot[AE_DOT_NO];
    FIXED gain_dot[AE_DOT_NO];
    FIXED ispdgain_max_algo[3]; //only get max from IQ
    FIXED ispdgain_max; // get ispdgain max ISP support
    FIXED flicker_enable;
    FIXED flicker_select;
    FIXED tolerance;
    FIXED toleranceOut;
    FIXED start_exp[START_EXP_NO];
    FIXED start_isp_gain[START_EXP_NO];
    uint32_t start_reg_time[START_EXP_NO];
    uint32_t start_reg_gain[START_EXP_NO];
    uint32_t start_dcg_mode[START_EXP_NO];

    FIXED lexp_level[AE_DOT_NO];
    FIXED lsetpoint[AE_DOT_NO];
    FIXED darksetpoint[AE_DOT_NO];
    FIXED ltime_dot[AE_DOT_NO];
    FIXED lgain_dot[AE_DOT_NO];
    FIXED stime_dot[AE_DOT_NO];
    FIXED sgain_dot[AE_DOT_NO];
    FIXED exp_ratio_dot[AE_DOT_NO];
    FIXED exp_ratio[AE_DOT_NO];

    FIXED time_factor[AE_TIME_FACTOR_NUM];
    AE_ExpGainMode_t eGainMode;
    uint32_t gain_range_size;
    struct fastae_time_range time_range;
    struct fastae_gain_range gain_range[AE_GAIN_RANGE_NUM];
    struct fastae_dcg_cfg dcg_cfg;
    FIXED env_calib[ENV_CALIB_NO];

    uint32_t adc_calib_type; //0:Linear 1:Table
    uint32_t adc_range_type; //0:dual 1:single
    uint32_t adc_table_len;
    FIXED adc_calib[ADC_CALIB_NO];
    FIXED adc_table_low[ADC_TABLE_LEN][2];
    FIXED adc_table_high[ADC_TABLE_LEN][2];

    FIXED exp_calib[EXP_CALIB_NO];
    struct smart_ir_cfg ir_config;

    bool  is_hdr;
    bool  is_ind_gain;
    ALS_TYPE_t als_type;
    FIXED low_adc;
    FIXED high_adc;
    LFIXED env_lux;
    night_mode_t night_mode;
    int led_value;

    bool is_fps_fix;
    FIXED iq_fps_value;
    FIXED cis_min_fps;

    FIXED AecGridWeight[AE_EXP_GRID_ITEMS_NUM];

    //rtt conv result
    bool     is_wakeup;
    uint8_t  conv_day_or_night;
    uint16_t conv_r_gain;
    uint16_t conv_b_gain;
    uint16_t conv_ae_stats[225];
    FIXED    conv_exp;
    FIXED    conv_isp_dgain;
    FIXED    conv_reg_time;
    FIXED    conv_reg_gain;
    FIXED    conv_dcg_mode;
    FIXED    nr_diff_th;
    FIXED    ae_diff_th;
    uint64_t last_lodif;
    uint64_t last_hidif;
};

struct fastae_md_param {
    uint16_t width;
    uint16_t height;
    uint32_t lo_sigma; //linear domain
    uint32_t hi_sigma; // linear domain
    uint32_t log_losigma; // log domain
    uint32_t log_hisigma; // log domain
    uint64_t last_lodif;
    uint64_t last_hidif;
};

AE_RESULT FastAeInit(struct fastae_init_info *ae_init_info);
AE_RESULT FastAeNightInit(struct fastae_init_info *ae_init_info);
AE_RESULT FastAeRelease();
AE_RESULT FastAeRun(struct fastae_info *ae_info);
AE_RESULT FastAeGetMeanLuma(struct fastae_info *ae_info, FIXED *mean_luma);
AE_RESULT FastAeInitExpCalc(struct fastae_init_info *ae_init_info);
AE_RESULT FastAeNightExpCalc(struct fastae_init_info *ae_init_info);

//dual cam
AE_RESULT FastAeSecInit(struct fastae_init_info *ae_init_info);
AE_RESULT FastAeMainMapSec(struct fastae_info *ae_info);
uint8_t FastAeMd(uint16_t* ori_stats, struct rkisp_stats* new_stats, struct fastae_md_param param);

#endif

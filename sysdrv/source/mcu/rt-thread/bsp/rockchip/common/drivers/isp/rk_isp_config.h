/*
 *  Rockchip driver for RK ISP 1.1 ON RT-Thread
 *
 * Copyright (C) 2016 Fuzhou Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-7-20     swallow team     first implementation
 *
 */

#ifndef _RK_ISP_CONFIG_H
#define _RK_ISP_CONFIG_H

#ifdef __RT_THREAD__
#include <sys/time.h>
#include "rk_isp_reg.h"
#endif

#include "isp_adptive_camera_config.h"

#define CONFIG_ADDRESS                                    0x100000
#define CONFIG_MAGIC                                      "CONMAG"

#if defined(RT_USING_ISP)

#define SUBDEV_DEVICE_NUM_MAX                             4
#define AWB_SCENE_DEFAULT_NUM                             6
#define RK_ISP_DEVICE_NAME_SIZE                           12
#define SUBDEV_STRM_DESC_NUM                              2

#define IS_ERR_VALUE(x)                                  ((x != 0) ? 1 : 0 )

#define RK_ISP_MODULE_MAX                                18
/* ISP Other module ID */
#define RK_ISP_DPCC_ID                                   0
#define RK_ISP_BLS_ID                                    1
#define RK_ISP_SDG_ID                                    2
#define RK_ISP_LSC_ID                                    3
#define RK_ISP_AWB_GAIN_ID                               4
#define RK_ISP_FLT_ID                                    5
#define RK_ISP_BDM_ID                                    6
#define RK_ISP_CTK_ID                                    7
#define RK_ISP_GOC_ID                                    8
#define RK_ISP_CPROC_ID                                  9
#define RK_ISP_IE_ID                                     10
#define RK_ISP_WDR_ID                                    11
#define RK_ISP_DPF_ID                                    12
#define RK_ISP_DPF_STRENGTH_ID                           13
/* ISP Meas module ID, It must be after other id */
#define RK_ISP_MEAS_ID                                   14
#define RK_ISP_AEC_ID                                (RK_ISP_MEAS_ID + 0)
#define RK_ISP_AFC_ID                                (RK_ISP_MEAS_ID + 1)
#define RK_ISP_AWB_ID                                (RK_ISP_MEAS_ID + 2)
#define RK_ISP_HST_ID                                (RK_ISP_MEAS_ID + 3)

#define RK_ISP_MODULE_DPCC                           (1 << RK_ISP_DPCC_ID)
#define RK_ISP_MODULE_BLS                            (1 << RK_ISP_BLS_ID)
#define RK_ISP_MODULE_SDG                            (1 << RK_ISP_SDG_ID)
#define RK_ISP_MODULE_HST                            (1 << RK_ISP_HST_ID)
#define RK_ISP_MODULE_LSC                            (1 << RK_ISP_LSC_ID)
#define RK_ISP_MODULE_AWB_GAIN                       (1 << RK_ISP_AWB_GAIN_ID)
#define RK_ISP_MODULE_FLT                            (1 << RK_ISP_FLT_ID)
#define RK_ISP_MODULE_BDM                            (1 << RK_ISP_BDM_ID)
#define RK_ISP_MODULE_CTK                            (1 << RK_ISP_CTK_ID)
#define RK_ISP_MODULE_GOC                            (1 << RK_ISP_GOC_ID)
#define RK_ISP_MODULE_CPROC                          (1 << RK_ISP_CPROC_ID)
#define RK_ISP_MODULE_AFC                            (1 << RK_ISP_AFC_ID)
#define RK_ISP_MODULE_AWB                            (1 << RK_ISP_AWB_ID)
#define RK_ISP_MODULE_IE                             (1 << RK_ISP_IE_ID)
#define RK_ISP_MODULE_AEC                            (1 << RK_ISP_AEC_ID)
#define RK_ISP_MODULE_WDR                            (1 << RK_ISP_WDR_ID)
#define RK_ISP_MODULE_DPF                            (1 << RK_ISP_DPF_ID)
#define RK_ISP_MODULE_DPF_STRENGTH                   (1 << RK_ISP_DPF_STRENGTH_ID)

#define RK_ISP_CTK_COEFF_MAX                         0x100
#define RK_ISP_CTK_OFFSET_MAX                        0x800

#define RK_ISP_AE_MEAN_MAX                           25
#define RK_ISP_HIST_BIN_N_MAX                        16
#define RK_ISP_AFM_MAX_WINDOWS                       3
#define RK_ISP_DEGAMMA_CURVE_SIZE                    17

#define RK_ISP_BDM_MAX_TH                            0xFF

/* maximum value for horizontal start address*/
#define RKISP_BLS_START_H_MAX                        (0x00000FFF)
/* maximum value for horizontal stop address */
#define RKISP_BLS_STOP_H_MAX                         (0x00000FFF)
/* maximum value for vertical start address*/
#define RKISP_BLS_START_V_MAX                        (0x00000FFF)
/* maximum value for vertical stop address*/
#define RKISP_BLS_STOP_V_MAX                         (0x00000FFF)
/* maximum is 2^18 = 262144*/
#define RKISP_BLS_SAMPLES_MAX                        (0x00000012)
/* maximum value for fixed black level*/
#define RKISP_BLS_FIX_SUB_MAX                        (0x00000FFF)
/* minimum value for fixed black level*/
#define RKISP_BLS_FIX_SUB_MIN                        (0xFFFFF000)
/* 13 bit range (signed)*/
#define RKISP_BLS_FIX_MASK                           (0x00001FFF)
/* AWB */
#define RK_ISP_AWB_MAX_GRID                          1
#define RK_ISP_AWB_MAX_FRAMES                        7

/* Gamma out*/
/* Maximum number of color samples supported*/
#define RK_ISP_GAMMA_OUT_MAX_SAMPLES                 34

/* WDR */
#define RK_ISP_WDR_SIZE                              48

/* LSC */
#define RK_ISP_LSC_GRAD_TBL_SIZE                      8
#define RK_ISP_LSC_SIZE_TBL_SIZE                      8
/* The following matches the tuning process,
 * not the max capabilites of the chip. */
#define RK_ISP_LSC_DATA_TBL_SIZE                     289
/* HIST */
#define RK_ISP_HISTOGRAM_WEIGHT_GRIDS_SIZE            25

/* DPCC */
#define RK_ISP_DPCC_METHODS_MAX                      (3)

/* DPF */
#define RK_ISP_DPF_MAX_NLF_COEFFS                    17
#define RK_ISP_DPF_MAX_SPATIAL_COEFFS                6

#define RK_ISP_STAT_AWB                             (1 << 0)
#define RK_ISP_STAT_AUTOEXP                         (1 << 1)
#define RK_ISP_STAT_AFM_FIN                         (1 << 2)
#define RK_ISP_STAT_HIST                            (1 << 3)

#define RK_ISP_HDRAE_MSGID                          0x00000001
#define RK_ISP_HDRAE_HIST_BIN_NUM                   (1 << 8)
#define RK_ISP_HDRAE_MAXFRAMES                      3
#define RK_ISP_HDRAE_MAXGRIDITEMS                   (15 * 15)
#define RK_ISP_HDRAE_SFREGION0_NUM                  (5 * 15)
#define RK_ISP_HDRAE_SFREGION1_NUM                  (10 * 3)
#define RK_ISP_HDRAE_SFREGION2_NUM                  (10 * 3)
#define RK_ISP_HDRAE_SFREGION3_NUM                  (10 * 9)
#define RK_ISP_HDRAE_EXP_DATA_NUM                   (sizeof(int) * 6)

#define RK_ISP_HDRAE_1_FRAME_OFFSET                 2
#define RK_ISP_HDRAE_2_FRAME_OFFSET                 (369 + RK_ISP_HDRAE_1_FRAME_OFFSET)
#define RK_ISP_HDRAE_3_FRAME_OFFSET                 (369 + RK_ISP_HDRAE_2_FRAME_OFFSET)
#define RK_ISP_HDRAE_NEW_DATA_START                 (369 + RK_ISP_HDRAE_3_FRAME_OFFSET)
#define RK_ISP_HDRAE_OEDATA_OFFSET                  (369 + RK_ISP_HDRAE_3_FRAME_OFFSET)
#define RK_ISP_HDRAE_SFREGION0_OFFSET               (RK_ISP_HDRAE_OEDATA_OFFSET + 3)
#define RK_ISP_HDRAE_SFREGION1_OFFSET               (RK_ISP_HDRAE_SFREGION0_OFFSET + RK_ISP_HDRAE_SFREGION0_NUM)
#define RK_ISP_HDRAE_SFREGION2_OFFSET               (RK_ISP_HDRAE_SFREGION1_OFFSET + RK_ISP_HDRAE_SFREGION1_NUM)
#define RK_ISP_HDRAE_SFREGION3_OFFSET               (RK_ISP_HDRAE_SFREGION2_OFFSET + RK_ISP_HDRAE_SFREGION2_NUM)
#define RK_ISP_HDRAE_DYDATA_OFFSET                  (RK_ISP_HDRAE_SFREGION3_OFFSET + RK_ISP_HDRAE_SFREGION3_NUM)

#define RK_ISP_HDRAE_EXP_DATA_OFFSET                (RK_ISP_HDRAE_NEW_DATA_START + 5)
#define RK_ISP_HDRAE_EXP_LGMEAN_OFFSET              (RK_ISP_HDRAE_EXP_DATA_OFFSET + 6)
#define RK_ISP_GAMMA_OUT_OFFSET                     (RK_ISP_HDRAE_EXP_LGMEAN_OFFSET + 1)

enum rk_isp_pix_fmt
{
    /* YUV */
    RK_YUV400               = 0x10008000,
    RK_YVU400               = 0x10008004,
    RK_Y10                  = 0x1000a000,
    RK_Y12                  = 0x10010000,/* bpp is 16bit */
    RK_Y12_420SP            = 0x10014211,/* bpp is 20bit */
    RK_Y12_422SP            = 0x10018221,/* bpp is 24bit */

    RK_YUV420I              = 0x1000c220,
    RK_YUV420SP             = 0x1000c221,   /* NV12 */
    RK_YUV420P              = 0x1000c222,
    RK_YVU420I              = 0x1000c224,
    RK_YVU420SP             = 0x1000c225,   /* NV21 */
    RK_YVU420P              = 0x1000c226,   /* YV12 */

    RK_YUV422I              = 0x10010240,
    RK_YUV422SP             = 0x10010241,
    RK_YUV422P              = 0x10010242,
    RK_YVU422I              = 0x10010244,
    RK_YVU422SP             = 0x10010245,
    RK_YVU422P              = 0x10010246,

    RK_YUV444I              = 0x10018440,
    RK_YUV444SP             = 0x10018441,
    RK_YUV444P              = 0x10018442,
    RK_YVU444I              = 0x10018444,
    RK_YVU444SP             = 0x10018445,
    RK_YVU444P              = 0x10018446,

    RK_UYV400               = 0x10008008,

    RK_UYV420I              = 0x1000c228,
    RK_UYV420SP             = 0x1000c229,
    RK_UYV420P              = 0x1000c22a,
    RK_VYU420I              = 0x1000c22c,
    RK_VYU420SP             = 0x1000c22d,
    RK_VYU420P              = 0x1000c22e,

    RK_UYV422I              = 0x10010248,
    RK_UYV422SP             = 0x10010249,
    RK_UYV422P              = 0x1001024a,
    RK_VYU422I              = 0x1001024c,
    RK_VYU422SP             = 0x1001024d,
    RK_VYU422P              = 0x1001024e,

    RK_UYV444I              = 0x10018448,
    RK_UYV444SP             = 0x10018449,
    RK_UYV444P              = 0x1001844a,
    RK_VYU444I              = 0x1001844c,
    RK_VYU444SP             = 0x1001844d,
    RK_VYU444P              = 0x1001844e,

    /* RGB */
    RK_RGB565               = 0x20010000,
    RK_RGB666               = 0x20012000,
    RK_RGB888               = 0x20018000,

    /* RAW Bayer */
    RK_BAYER_SBGGR8         = 0x30008000,
    RK_BAYER_SGBRG8         = 0x30008010,
    RK_BAYER_SGRBG8         = 0x30008020,
    RK_BAYER_SRGGB8         = 0x30008030,

    RK_BAYER_SBGGR10        = 0x3000a000,
    RK_BAYER_SGBRG10        = 0x3000a010,
    RK_BAYER_SGRBG10        = 0x3000a020,
    RK_BAYER_SRGGB10        = 0x3000a030,

    RK_BAYER_SBGGR12        = 0x3000c000,
    RK_BAYER_SGBRG12        = 0x3000c010,
    RK_BAYER_SGRBG12        = 0x3000c020,
    RK_BAYER_SRGGB12        = 0x3000c030,

    /* JPEG */
    RK_JPEG                 = 0x40008000,

    /* Data */
    RK_DATA                 = 0x70000000,

    RK_UNKNOWN_FORMAT       = 0x80000000
};

enum rk_isp_pix_fmt_quantization
{
    RK_ISP_QUANTIZATION_DEFAULT      = 0,
    RK_ISP_QUANTIZATION_FULL_RANGE   = 1,
    RK_ISP_QUANTIZATION_LIM_RANGE    = 2
};

enum rk_isp_histogram_mode
{
    RK_ISP_HISTOGRAM_MODE_DISABLE         = 0,
    RK_ISP_HISTOGRAM_MODE_RGB_COMBINED    = 1,
    RK_ISP_HISTOGRAM_MODE_R_HISTOGRAM     = 2,
    RK_ISP_HISTOGRAM_MODE_G_HISTOGRAM     = 3,
    RK_ISP_HISTOGRAM_MODE_B_HISTOGRAM     = 4,
    RK_ISP_HISTOGRAM_MODE_Y_HISTOGRAM     = 5
};

struct rk_isp_hdrae_hist_meas_res
{
    unsigned int hist_bin[RK_ISP_HDRAE_HIST_BIN_NUM];
};

struct rk_isp_hdrae_mean_meas_res
{
    unsigned short y_meas[RK_ISP_HDRAE_MAXGRIDITEMS];
};

struct rk_isp_hdrae_oneframe_result
{
    struct rk_isp_hdrae_hist_meas_res hist_meas;
    struct rk_isp_hdrae_mean_meas_res mean_meas;
};

struct rk_isp_hdrae_sframe_regionmatrix_s
{
    unsigned int Region0[RK_ISP_HDRAE_SFREGION0_NUM];
    unsigned int Region1[RK_ISP_HDRAE_SFREGION1_NUM];
    unsigned int Region2[RK_ISP_HDRAE_SFREGION2_NUM];
    unsigned int Region3[RK_ISP_HDRAE_SFREGION3_NUM];
};

struct rk_isp_hdrae_OE_meas_res
{
    unsigned int OE_Pixel;
    unsigned int SumHistPixel;
    unsigned int SframeMaxLuma;
    struct rk_isp_hdrae_sframe_regionmatrix_s SfRegionMatrix;
};

struct rk_isp_hdrae_DRIndex_res
{
    unsigned int fNormalIndex;
    unsigned int fLongIndex;
};

struct sensor_hdrmetadata_s
{
    unsigned int exp_time_l;
    unsigned int exp_time;
    unsigned int exp_time_s;
    unsigned int gain_l;
    unsigned int gain;
    unsigned int gain_s;
};

struct rk_isp_hdrae_result
{
    unsigned int mesg_id;
    unsigned int mesg_size;
    struct rk_isp_hdrae_oneframe_result oneframe[RK_ISP_HDRAE_MAXFRAMES];
    struct rk_isp_hdrae_OE_meas_res OEMeasRes;
    struct rk_isp_hdrae_DRIndex_res DRIndexRes;
    struct sensor_hdrmetadata_s sensor;
    unsigned int lgmean;
    unsigned short gammaOut[RK_ISP_GAMMA_OUT_MAX_SAMPLES];
};

struct rk_isp_dspmsg_head
{
    unsigned int mesg_total_size;
    unsigned int frame_id;
    unsigned int mesg_count;
};

enum rk_isp_exp_ctrl_autostop
{
    RK_ISP_EXP_CTRL_AUTOSTOP_0 = 0,
    RK_ISP_EXP_CTRL_AUTOSTOP_1 = 1
};

enum rk_isp_image_effect
{
    RK_ISP_IE_BW = 0,
    RK_ISP_IE_NEGATIVE = 1,
    RK_ISP_IE_SEPIA = 2,
    RK_ISP_IE_C_SEL = 3,
    RK_ISP_IE_EMBOSS = 4,
    RK_ISP_IE_SKETCH = 5,
    RK_ISP_IE_NONE /* not a bit field value */
};

enum rk_isp_exp_meas_mode
{
    RK_ISP_EXP_MEASURING_MODE_0 = 0,    /**< Y = 16 + 0.25R + 0.5G + 0.1094B */
    RK_ISP_EXP_MEASURING_MODE_1 = 1,    /**< Y = (R + G + B) x (85/256) */
};

struct rk_isp_window
{
    unsigned short h_offs;
    unsigned short v_offs;
    unsigned short h_size;
    unsigned short v_size;
};

enum rk_isp_awb_mode_type
{
    RK_ISP_AWB_MODE_MANUAL  = 0,
    RK_ISP_AWB_MODE_RGB     = 1,
    RK_ISP_AWB_MODE_YCBCR   = 2
};

enum rk_isp_bls_win_enable
{
    ISP_BLS_CTRL_WINDOW_ENABLE_0 = 0,
    ISP_BLS_CTRL_WINDOW_ENABLE_1 = 1,
    ISP_BLS_CTRL_WINDOW_ENABLE_2 = 2,
    ISP_BLS_CTRL_WINDOW_ENABLE_3 = 3
};

enum rk_isp_flt_mode
{
    RK_ISP_FLT_STATIC_MODE,
    RK_ISP_FLT_DYNAMIC_MODE
};

struct rk_isp_awb_meas
{
    unsigned int cnt;
    unsigned char mean_y;
    unsigned char mean_cb;
    unsigned char mean_cr;
    unsigned short mean_r;
    unsigned short mean_b;
    unsigned short mean_g;
};

struct rk_isp_awb_stat
{
    struct rk_isp_awb_meas awb_mean[RK_ISP_AWB_MAX_GRID];//16
};

struct rk_isp_hist_stat
{
    unsigned int hist_bins[RK_ISP_HIST_BIN_N_MAX];//64
};

/*! BLS mean measured values*/
struct rk_isp_bls_meas_val                       //8
{
    /*! Mean measured value for Bayer pattern position A.*/
    unsigned short meas_r;
    /*! Mean measured value for Bayer pattern Gr.*/
    unsigned short meas_gr;
    /*! Mean measured value for Bayer pattern Gb.*/
    unsigned short meas_gb;
    /*! Mean measured value for Bayer pattern B.*/
    unsigned short meas_b;
};

/*! BLS fixed subtraction values. The values will be subtracted from the sensor
 *  values. Therefore a negative value means addition instead of subtraction!*/
struct rk_isp_bls_fixed_val    //8
{
    /*! Fixed (signed!) subtraction value for Bayer pattern R.*/
    signed short r;
    /*! Fixed (signed!) subtraction value for Bayer pattern Gr.*/
    signed short gr;
    /*! Fixed (signed!) subtraction value for Bayer pattern Gb.*/
    signed short gb;
    /*! Fixed (signed!) subtraction value for Bayer pattern B.*/
    signed short b;
};

/* Configuration used by black level subtraction */
struct rk_isp_bls_config
{
    /*! Automatic mode activated means that the measured values
     * are subtracted.Otherwise the fixed subtraction
     * values will be subtracted.*/
    bool enable_auto;
    unsigned char en_windows;
    struct rk_isp_window bls_window1;      /*!< Measurement window 1.*/
    struct rk_isp_window bls_window2;      /*!< Measurement window 2*/
    /*! Set amount of measured pixels for each Bayer position
     * (A, B,C and D) to 2^bls_samples.*/
    unsigned char bls_samples;
    struct rk_isp_bls_fixed_val fixed_val; /*!< Fixed subtraction values.*/
};


struct rk_isp_ae_stat
{
    unsigned char exp_mean[RK_ISP_AE_MEAN_MAX];
    struct rk_isp_bls_meas_val bls_val; /*available wit exposure results*/
};

struct rk_isp_af_meas_val
{
    unsigned int sum;
    unsigned int lum;
};

struct rk_isp_af_stat
{
    struct rk_isp_af_meas_val window[RK_ISP_AFM_MAX_WINDOWS];
};

enum rk_isp_lightsensor_val
{
    RK_ISP_LS_INVAL = -1,
    RK_ISP_LS_DAY   = 0,
    RK_ISP_LS_NIGHT = 1,
    RK_ISP_LS_HOLD  = 2
};

struct rk_isp_lightsensor_stat
{
    enum rk_isp_lightsensor_val val;
};

enum rk_isp_vcm_val
{
    RK_ISP_VCM_INVAL        = -1,
    RK_ISP_VCM_MOVE_START   = 0,
    RK_ISP_VCM_MOVE_RUNNING = 1,
    RK_ISP_VCM_MOVE_END     = 2
};

struct rk_isp_stat
{
    struct rk_isp_awb_stat awb;
    struct rk_isp_ae_stat ae;
    struct rk_isp_af_stat af;
    struct rk_isp_hist_stat hist;
};

struct vcm_move_data
{
    unsigned int start_move_tv;
    unsigned int end_move_tv;
};

struct rk_isp_stat_buffer
{
    unsigned int meas_type;
    struct rk_isp_stat params;
    struct isp_supplemental_sensor_mode_data sensor_mode;
    unsigned int vs_t;
    unsigned int fi_t;
    struct vcm_move_data vcm_move;
};


struct rk_isp_dpcc_methods_config
{
    unsigned int method;
    unsigned int  line_thresh;
    unsigned int  line_mad_fac;
    unsigned int  pg_fac;
    unsigned int  rnd_thresh;
    unsigned int  rg_fac;
};

struct rk_isp_dpcc_config
{
    unsigned int  mode;
    unsigned int  output_mode;
    unsigned int  set_use;
    struct rk_isp_dpcc_methods_config methods[RK_ISP_DPCC_METHODS_MAX];
    unsigned int  ro_limits;
    unsigned int  rnd_offs;
};

struct rk_isp_gamma_corr_curve
{
    unsigned short gamma_y[RK_ISP_DEGAMMA_CURVE_SIZE];
};

struct rk_isp_gamma_curve_x_axis_pnts
{
    unsigned int  gamma_dx0;
    unsigned int  gamma_dx1;
};

/* Configuration used by sensor degamma */
struct rk_isp_sdg_config
{
    struct rk_isp_gamma_corr_curve curve_r;
    struct rk_isp_gamma_corr_curve curve_g;
    struct rk_isp_gamma_corr_curve curve_b;
    struct rk_isp_gamma_curve_x_axis_pnts xa_pnts;
};


/* Configuration used by Lens shading correction */
struct rk_isp_lsc_config
{
    unsigned int r_data_tbl[RK_ISP_LSC_DATA_TBL_SIZE];
    unsigned int gr_data_tbl[RK_ISP_LSC_DATA_TBL_SIZE];
    unsigned int gb_data_tbl[RK_ISP_LSC_DATA_TBL_SIZE];
    unsigned int b_data_tbl[RK_ISP_LSC_DATA_TBL_SIZE];

    unsigned short x_grad_tbl[RK_ISP_LSC_GRAD_TBL_SIZE];
    unsigned short y_grad_tbl[RK_ISP_LSC_GRAD_TBL_SIZE];

    unsigned short x_size_tbl[RK_ISP_LSC_SIZE_TBL_SIZE];
    unsigned short y_size_tbl[RK_ISP_LSC_SIZE_TBL_SIZE];
    unsigned short config_width;
    unsigned short config_height;
};

enum rk_isp_colorfx
{
    RKISP_COLORFX_NONE          = 0,
    RKISP_COLORFX_BW            = 1,
    RKISP_COLORFX_SEPIA         = 2,
    RKISP_COLORFX_NEGATIVE              = 3,
    RKISP_COLORFX_EMBOSS                = 4,
    RKISP_COLORFX_SKETCH                = 5,
    RKISP_COLORFX_SKY_BLUE              = 6,
    RKISP_COLORFX_GRASS_GREEN           = 7,
    RKISP_COLORFX_SKIN_WHITEN           = 8,
    RKISP_COLORFX_VIVID         = 9,
    RKISP_COLORFX_AQUA          = 10,
    RKISP_COLORFX_ART_FREEZE            = 11,
    RKISP_COLORFX_SILHOUETTE            = 12,
    RKISP_COLORFX_SOLARIZATION          = 13,
    RKISP_COLORFX_ANTIQUE               = 14,
    RKISP_COLORFX_SET_CBCR              = 15,
};

struct rk_isp_ie_config
{
    enum rk_isp_colorfx effect;
    unsigned short color_sel;
    /*3x3 Matrix Coefficients for Emboss Effect 1*/
    unsigned short eff_mat_1;
    /*3x3 Matrix Coefficients for Emboss Effect 2*/
    unsigned short eff_mat_2;
    /*3x3 Matrix Coefficients for Emboss 3/Sketch 1*/
    unsigned short eff_mat_3;
    /*3x3 Matrix Coefficients for Sketch Effect 2*/
    unsigned short eff_mat_4;
    /*3x3 Matrix Coefficients for Sketch Effect 3*/
    unsigned short eff_mat_5;
    /*Chrominance increment values of tint (used for sepia effect)*/
    unsigned short eff_tint;
};

/* Configuration used by auto white balance */
struct rk_isp_awb_meas_config
{
    /*! white balance measurement window (in pixels)
     * Note: currently the h and v offsets are mapped to grid offsets*/
    struct rk_isp_window awb_wnd;
    enum rk_isp_awb_mode_type awb_mode;
    /*! only pixels values < max_y contribute to awb measurement
     * (set to 0 to disable this feature)*/
    unsigned char    max_y;
    /*! only pixels values > min_y contribute to awb measurement*/
    unsigned char    min_y;
    /*! Chrominance sum maximum value, only consider pixels with Cb+Cr
     *  smaller than threshold for awb measurements*/
    unsigned char    max_csum;
    /*! Chrominance minimum value, only consider pixels with Cb/Cr
     *  each greater than threshold value for awb measurements*/
    unsigned char    min_c;
    /*! number of frames - 1 used for mean value calculation
     *  (ucFrames=0 means 1 Frame)*/
    unsigned char    frames;
    /*! reference Cr value for AWB regulation, target for AWB*/
    unsigned char    awb_ref_cr;
    /*! reference Cb value for AWB regulation, target for AWB*/
    unsigned char    awb_ref_cb;
    bool enable_ymax_cmp;
};

struct rk_isp_awb_gain_config
{
    unsigned short  gain_red;
    unsigned short  gain_green_r;
    unsigned short  gain_blue;
    unsigned short  gain_green_b;
};

/* Configuration used by ISP filtering */
struct rk_isp_flt_config
{
    enum rk_isp_flt_mode  mode;    /* ISP_FILT_MODE register fields*/
    unsigned char grn_stage1;    /* ISP_FILT_MODE register fields*/
    unsigned char chr_h_mode;    /* ISP_FILT_MODE register fields*/
    unsigned char chr_v_mode;    /* ISP_FILT_MODE register fields*/
    unsigned int  thresh_bl0;
    unsigned int  thresh_bl1;
    unsigned int  thresh_sh0;
    unsigned int  thresh_sh1;
    unsigned int  lum_weight;
    unsigned int  fac_sh1;
    unsigned int  fac_sh0;
    unsigned int  fac_mid;
    unsigned int  fac_bl0;
    unsigned int  fac_bl1;
};

/* Configuration used by Bayer DeMosaic */
struct rk_isp_bdm_config
{
    unsigned char demosaic_th;
};

/* Configuration used by Cross Talk correction */
struct rk_isp_ctk_config
{
    unsigned short coeff0;
    unsigned short coeff1;
    unsigned short coeff2;
    unsigned short coeff3;
    unsigned short coeff4;
    unsigned short coeff5;
    unsigned short coeff6;
    unsigned short coeff7;
    unsigned short coeff8;
    /* offset for the crosstalk correction matrix */
    unsigned short ct_offset_r;
    unsigned short ct_offset_g;
    unsigned short ct_offset_b;
};

enum rk_isp_goc_mode
{
    RK_ISP_GOC_MODE_LOGARITHMIC,
    RK_ISP_GOC_MODE_EQUIDISTANT
};

/* Configuration used by Gamma Out correction */
struct rk_isp_goc_config
{
    enum rk_isp_goc_mode mode;
    unsigned short gamma_y[RK_ISP_GAMMA_OUT_MAX_SAMPLES];
};

/* CCM (Color Correction) */
struct rk_isp_cproc_config
{
    unsigned char c_out_range;
    unsigned char y_in_range;
    unsigned char y_out_range;
    unsigned char contrast;
    unsigned char brightness;
    unsigned char sat;
    unsigned char hue;
};

/* Configuration used by Histogram */
struct rk_isp_hst_config
{
    enum rk_isp_histogram_mode mode;
    unsigned char histogram_predivider;
    struct rk_isp_window meas_window;
    unsigned char hist_weight[RK_ISP_HISTOGRAM_WEIGHT_GRIDS_SIZE];
};

/* Configuration used by Auto Exposure Control */
struct rk_isp_aec_config
{
    enum rk_isp_exp_meas_mode mode;
    enum rk_isp_exp_ctrl_autostop autostop;
    struct rk_isp_window meas_window;
};

struct rk_isp_afc_config
{
    unsigned char num_afm_win;  /* max RK_ISP_AFM_MAX_WINDOWS */
    struct rk_isp_window afm_win[RK_ISP_AFM_MAX_WINDOWS];
    unsigned int thres;
    unsigned int var_shift;
};

enum rk_isp_dpf_gain_usage
{
    RK_ISP_DPF_GAIN_USAGE_DISABLED      = 1,   /**< don't use any gains in preprocessing stage */
    RK_ISP_DPF_GAIN_USAGE_NF_GAINS      = 2,   /**< use only the noise function gains  from registers DPF_NF_GAIN_R, ... */
    RK_ISP_DPF_GAIN_USAGE_LSC_GAINS     = 3,   /**< use only the gains from LSC module */
    RK_ISP_DPF_GAIN_USAGE_NF_LSC_GAINS  = 4,   /**< use the moise function gains and the gains from LSC module */
    RK_ISP_DPF_GAIN_USAGE_AWB_GAINS     = 5,   /**< use only the gains from AWB module */
    RK_ISP_DPF_GAIN_USAGE_AWB_LSC_GAINS = 6,   /**< use the gains from AWB and LSC module */
    RK_ISP_DPF_GAIN_USAGE_MAX                  /**< upper border (only for an internal evaluation) */
};

enum rk_isp_dpf_rb_filtersize
{
    RK_ISP_DPF_RB_FILTERSIZE_13x9      = 0,    /**< red and blue filter kernel size 13x9 (means 7x5 active pixel) */
    RK_ISP_DPF_RB_FILTERSIZE_9x9       = 1,    /**< red and blue filter kernel size 9x9 (means 5x5 active pixel) */
};

enum rk_isp_dpf_nll_scale_mode
{
    RK_ISP_NLL_SCALE_LINEAR        = 0,        /**< use a linear scaling */
    RK_ISP_NLL_SCALE_LOGARITHMIC   = 1,        /**< use a logarithmic scaling */
} ;

struct rk_isp_dpf_nll
{
    unsigned short coeff[RK_ISP_DPF_MAX_NLF_COEFFS];
    enum rk_isp_dpf_nll_scale_mode scale_mode;
};

struct rk_isp_dpf_rb_flt
{
    enum rk_isp_dpf_rb_filtersize fltsize;
    unsigned char spatial_coeff[RK_ISP_DPF_MAX_SPATIAL_COEFFS];
    bool r_enable;
    bool b_enable;
};

struct rk_isp_dpf_g_flt
{
    unsigned char spatial_coeff[RK_ISP_DPF_MAX_SPATIAL_COEFFS];
    bool gr_enable;
    bool gb_enable;
};

struct rk_isp_dpf_gain
{
    enum rk_isp_dpf_gain_usage mode;
    unsigned short nf_r_gain;
    unsigned short nf_b_gain;
    unsigned short nf_gr_gain;
    unsigned short nf_gb_gain;
};

struct rk_isp_dpf_config
{
    struct rk_isp_dpf_gain gain;
    struct rk_isp_dpf_g_flt g_flt;
    struct rk_isp_dpf_rb_flt rb_flt;
    struct rk_isp_dpf_nll nll;
};

struct rk_isp_dpf_strength_config
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct rk_isp_last_capture_config
{
    struct rk_isp_cproc_config cproc;
    struct rk_isp_goc_config   goc;
    struct rk_isp_ctk_config   ctk;
    struct rk_isp_bdm_config   bdm;
    struct rk_isp_flt_config   flt;
    struct rk_isp_awb_gain_config awb_gain;
    struct rk_isp_awb_meas_config awb_meas;
    struct rk_isp_lsc_config lsc;
    struct rk_isp_sdg_config sdg;
    struct rk_isp_bls_config bls;
};

enum rk_isp_wdr_mode
{
    RK_ISP_WDR_MODE_BLOCK,
    RK_ISP_WDR_MODE_GLOBAL
};

/* Configuration used by Gamma Out correction */
struct rk_isp_wdr_config
{
    enum rk_isp_wdr_mode mode;
    unsigned int c_wdr[RK_ISP_WDR_SIZE];
};
struct rk_isp_other_cfg
{
    unsigned int s_frame_id;/* Set isp hardware frame id */

    unsigned int module_ens;

    struct rk_isp_dpcc_config dpcc_config;
    struct rk_isp_bls_config bls_config;
    struct rk_isp_sdg_config sdg_config;
    struct rk_isp_lsc_config lsc_config;
    struct rk_isp_awb_gain_config awb_gain_config;
    struct rk_isp_flt_config flt_config;
    struct rk_isp_bdm_config bdm_config;
    struct rk_isp_ctk_config ctk_config;
    struct rk_isp_goc_config goc_config;
    struct rk_isp_cproc_config cproc_config;
    struct rk_isp_ie_config ie_config;
    struct rk_isp_dpf_config dpf_config;
    struct rk_isp_dpf_strength_config dpf_strength_config;
    struct rk_isp_wdr_config wdr_config;
};

struct rk_isp_meas_cfg
{
    unsigned int s_frame_id;        /* Set isp hardware frame id */

    unsigned int module_ens;

    struct rk_isp_awb_meas_config awb_meas_config;
    struct rk_isp_hst_config hst_config;
    struct rk_isp_aec_config aec_config;
    struct rk_isp_afc_config afc_config;
};

struct awb_init_info
{
    unsigned int range[2];
    unsigned int exp[2];
    unsigned int gain[4];
};

enum rk_isp_inp
{
    RK_ISP_INP_CSI = 0x10000000,
    RK_ISP_INP_CPI = 0x20000000,

    RK_ISP_INP_DMA = 0x30000000, /* DMA -> ISP */
    RK_ISP_INP_DMA_IE = 0x31000000, /* DMA -> IE */
    RK_ISP_INP_DMA_SP = 0x32000000, /* DMA -> SP */
    RK_ISP_INP_DMA_Y12 = 0x33000000, /* DMA -> Y12 */
    RK_ISP_INP_DMA_MAX = 0x34000000,

    RK_ISP_INP_MAX = 0x7fffffff
};

enum rk_isp_stream_id
{
    RK_ISP_STREAM_SP = 0x1,
    RK_ISP_STREAM_MP = 0x2,
    RK_ISP_STREAM_DMA = 0x4,
    RK_ISP_STREAM_ISP = 0x8,
    RK_ISP_STREAM_Y12 = 0x10
};

struct rk_isp_frm_intrvl
{
    unsigned int numerator;
    unsigned int denominator;
};

struct rk_isp_frm_fmt
{
    unsigned int width;
    unsigned int height;
    unsigned int stride;
    unsigned int std_id;
    enum rk_isp_pix_fmt pix_fmt;
    enum rk_isp_pix_fmt_quantization quantization;
#ifdef __RT_THREAD__
    struct rk_camera_crop_rect defrect;
#endif
};

struct rk_isp_strm_fmt
{
    struct rk_isp_frm_fmt frm_fmt;
    struct rk_isp_frm_intrvl frm_intrvl;
};

struct rk_isp_strm_fmt_desc
{
    bool discrete_frmsize;
    struct
    {
        unsigned int width;
        unsigned int height;
    } min_frmsize;
    struct
    {
        unsigned int width;
        unsigned int height;
    } max_frmsize;
    enum rk_isp_pix_fmt pix_fmt;
    bool discrete_intrvl;
    struct rk_isp_frm_intrvl min_intrvl;
    struct rk_isp_frm_intrvl max_intrvl;
#ifdef __RT_THREAD__
    struct rk_camera_crop_rect defrect;
#endif
    unsigned int std_id;
};

struct rk_isp_strm_fmt_init
{
    unsigned int pix_fmt; //from xml default boot stream
    unsigned int width; // from xml default boot stream
    unsigned int height; //from xml default boot stream
    unsigned int light_vol; // reference struct pltfrm_cam_ls
    unsigned int v_blank_us;
#ifdef __RT_THREAD__
    struct rk_camera_crop_rect defrect; // ref cmd PLTFRM_CIFCAM_G_DEFRECT --- ref struct pltfrm_cam_defrect
#endif
};

struct rk_isp_strm_desc_init
{
    unsigned int pix_format; // ref cmd enum_frameintervals --- ref struct v4l2_frmivalenum
    unsigned int width;
    unsigned int height;
    unsigned int numerator;
    unsigned int denominator;
    unsigned int type;
};

struct isp_init_info
{
    unsigned int subdev_cnt;
    char subdev_name[SUBDEV_DEVICE_NUM_MAX * RK_ISP_DEVICE_NAME_SIZE];
    struct awb_init_info scene[AWB_SCENE_DEFAULT_NUM];//from xml
    struct rk_isp_strm_fmt_init strm_fmt;
    struct rk_isp_strm_desc_init strm_desc[SUBDEV_STRM_DESC_NUM];
    struct pltfrm_cam_itf cam_itf; // ref camera cmd PLTFRM_CIFCAM_G_ITF_CFG
    struct isp_supplemental_sensor_mode_data sensor_data;
    enum rk_isp_stream_id   data_path;
};

struct config_param
{
    char magic[8];
    int is_ready;
    int boot_from;
    struct isp_init_info isp;
    int hash;
};

struct rk_isp_metadata
{
    unsigned int frame_id;
    struct rk_isp_stat_buffer meas_stat;
};

struct rk_buffer_metadata_s
{
    unsigned int frame_id;
    unsigned int lights_stat;
    struct rk_isp_metadata isp_meta;
};
#endif
#endif

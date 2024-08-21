/*
 *  Rockchip driver for RK ISP 1.1
 * (Based on Intel driver for sofiaxxx)
 *
 * Copyright (C) 2015 Intel Mobile Communications GmbH
 * Copyright (C) 2016 Fuzhou Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-7-20       ISP_TEAM     first implementation
 *
 */

#ifndef _RK_ISP_ISP_H
#define _RK_ISP_ISP_H

#include <rtdef.h>
#include <rthw.h>
#include "soc.h"

#include "rk_isp_config.h"

#if defined(RT_USING_ISP)

#ifndef NULL
#define NULL RT_NULL
#endif

/****************************************************************************
*                                                     ISP device struct
****************************************************************************/
#define RK_ISP_META_INFO_NUM        2
#define ISP_VIDEO_BUF_NUM_MAX       32
#define RKISP_MEAS_SEND_ALONE                   (RK_ISP_AFM_FIN)

#define _GET_                                   0
#define _SET_                                   1
#define RKISP_MODULE_EN(v, m)                   (v |= m)
#define RKISP_MODULE_DIS(v, m)                  (v &= ~m)
#define RKISP_MODULE_IS_EN(v, m)                ((v & m) == m)
#define RKISP_MODULE_UPDATE(v, m)               (v |= m)
#define RKISP_MODULE_CLR_UPDATE(v, m)           (v &= ~m)
#define RKISP_MODULE_IS_UPDATE(v, m)            ((v & m) == m)

#define RKISP_MODULE_UNACTIVE(v, m)             ((v) |= (m))
#define RKISP_MODULE_ACTIVE(v, m)               ((v) &= ~(m))
#define RKISP_MODULE_IS_UNACTIVE(v, m)          (((v) & (m)) == (m))

/* Demosaic */
#define RKISP_BDM_BYPASS_EN(val)                ((val) << 10)
/* HIST */
#define RKISP_HIST_PREDIV_SET(val)              ((val) << 3)
#define RKISP_HIST_WEIGHT_SET(v0, v1, v2, v3)   (v0 | (v1<<8)  |(v2<<16) | (v3<<24))
#define RKISP_HIST_WINDOW_OFFSET_RESERVED       (0xFFFFF000)
#define RKISP_HIST_WINDOW_SIZE_RESERVED         (0xFFFFF800)
#define RKISP_HIST_WEIGHT_RESERVED              (0xE0E0E0E0)
#define RKISP_MAX_HIST_PREDIVIDER               (0x0000007F)
#define RKISP_HIST_ROW_NUM                      (5)
#define RKISP_HIST_COLUMN_NUM                   (5)
/* ISP Ctrl */
#define RKISP_CTRL_ISP_GAMMA_IN_ENA             (1<<6)
#define RKISP_CTRL_ISP_AWB_ENA                  (1<<7)
#define RKISP_CTRL_ISP_GAMMA_OUT_ENA            (1<<11)
/* AWB */
#define RKISP_AWB_GAIN_R_SET(val)               ((val) << 16)
#define RKISP_AWB_GAIN_R_READ(val)              ((val) >> 16)
#define RKISP_AWB_GAIN_B_READ(val)              ((val) & 0xFFFF)

#define RKISP_AWB_YMAX_CMP_EN                   (1 << 2)
#define RKISP_AWB_REF_CR_SET(val)               ((val) << 8)
#define RKISP_AWB_REF_CR_READ(val)              ((val) >> 8)
#define RKISP_AWB_REF_CB_READ(val)              ((val) & 0xFF)
#define RKISP_AWB_MAX_CS_SET(val)               ((val) << 8)
#define RKISP_AWB_MAX_CS_READ(val)              (((val) >> 8) & 0xFF)
#define RKISP_AWB_MIN_C_READ(val)               ((val) & 0xFF)
#define RKISP_AWB_MIN_Y_SET(val)                ((val) << 16)
#define RKISP_AWB_MIN_Y_READ(val)               (((val) >> 16) & 0xFF)
#define RKISP_AWB_MAX_Y_SET(val)                ((val) << 24)
#define RKISP_AWB_MAX_Y_READ(val)               ((val) >> 24)
#define RKISP_AWB_MODE_RGB_EN                   ((1 << 31) | (0x02 << 0))
#define RKISP_AWB_MODE_YCBCR_EN                 ((0 << 31) | (0x02 << 0))
#define RKISP_AWB_MODE_READ(val)                ((val) & 3)
#define RKISP_AWB_YMAX_READ(val)                (((val) >> 2) & 1)

#define RKISP_AWB_GET_MEAN_CR(val)              ((val) & 0xFF)
#define RKISP_AWB_GET_MEAN_CB(val)              (((val) >> 8) & 0xFF)
#define RKISP_AWB_GET_MEAN_Y(val)               (((val) >> 16) & 0xFF)
#define RKISP_AWB_GET_MEAN_R(val)               ((val) & 0xFF)
#define RKISP_AWB_GET_MEAN_B(val)               (((val) >> 8) & 0xFF)
#define RKISP_AWB_GET_MEAN_G(val)               (((val) >> 16) & 0xFF)
#define RKISP_AWB_GET_PIXEL_CNT(val)                    ((val) & 0x3FFFFFF)

#define RKISP_AWB_GAINS_MAX_VAL                 (0x000003FF)
#define RKISP_AWB_WINDOW_OFFSET_MAX             (0x00000FFF)
#define RKISP_AWB_WINDOW_MAX_SIZE               (0x00001FFF)
#define RKISP_AWB_CBCR_MAX_REF                  (0x000000FF)
#define RKISP_AWB_THRES_MAX_YC                  (0x000000FF)
/* AE */
#define RKISP_EXP_ENA                           (1)
#define RKISP_EXP_DIS                           (0)
#define RKISP_EXP_ROW_NUM                       (5)
#define RKISP_EXP_COLUMN_NUM                    (5)
#define RKISP_EXP_NUM_LUMA_REGS                 (RKISP_EXP_ROW_NUM *\
                                                 RKISP_EXP_COLUMN_NUM)
#define RKISP_EXP_MAX_HOFFS                     (2424)
#define RKISP_EXP_MAX_VOFFS                     (1806)
#define RKISP_EXP_BLOCK_MAX_HSIZE               (516)
#define RKISP_EXP_BLOCK_MIN_HSIZE               (35)
#define RKISP_EXP_BLOCK_MAX_VSIZE               (390)
#define RKISP_EXP_BLOCK_MIN_VSIZE               (28)
#define RKISP_EXP_MAX_HSIZE                     (RKISP_EXP_BLOCK_MAX_HSIZE*RKISP_EXP_COLUMN_NUM + 1)
#define RKISP_EXP_MIN_HSIZE                     (RKISP_EXP_BLOCK_MIN_HSIZE*RKISP_EXP_COLUMN_NUM + 1)
#define RKISP_EXP_MAX_VSIZE                     (RKISP_EXP_BLOCK_MAX_VSIZE*RKISP_EXP_ROW_NUM + 1)
#define RKISP_EXP_MIN_VSIZE                     (RKISP_EXP_BLOCK_MIN_VSIZE*RKISP_EXP_ROW_NUM + 1)
#define RKISP_EXP_HEIGHT_MASK                   (0x000007FF)
#define RKISP_EXP_MAX_HOFFSET                   (0x00000FFF)
#define RKISP_EXP_MAX_VOFFSET                   (0x00000FFF)

#define RKISP_EXP_CTRL_AUTOSTOP(val)            ((val) << 1)
#define RKISP_EXP_CTRL_MEASMODE(val)            (val << 31)
#define RKISP_EXP_HSIZE(val)                    (val & 0x7FF)
#define RKISP_EXP_VSIZE(val)                    (val & 0x7FE)
/* LSC */
#define RKISP_LSC_GRADH_SET(val)                ((val) << 11)
#define RKISP_LSC_SECTH_SET(val)                ((val) << 10)

/* FLT */
#define RKISP_FLT_MODE_MAX                      (1)
#define RKISP_FLT_CHROMA_MODE_MAX               (3)
#define RKISP_FLT_GREEN_STAGE1_MAX              (8)
#define RKISP_FLT_MODE(v)                       (v << 1)
#define RKISP_FLT_CHROMA_V_MODE(v)              (v << 4)
#define RKISP_FLT_CHROMA_H_MODE(v)              (v << 6)
#define RKISP_FLT_GREEN_STAGE1(v)               (v << 8)
#define RKISP_FLT_THREAD_RESERVED               (0xfffffc00)
#define RKISP_FLT_FAC_RESERVED                  (0xffffffc0)
#define RKISP_FLT_LUM_WEIGHT_RESERVED           (0xfff80000)
#define RKISP_FLT_ENA                           (1)
#define RKISP_FLT_DIS                           (0)

#define RKISP_CTK_COEFF_RESERVED                0xFFFFF800
#define RKISP_XTALK_OFFSET_RESERVED             0xFFFFF000

/* GOC */
#define RKISP_GOC_MODE_MAX                      (1)
#define RKISP_GOC_RESERVED                      0xFFFFF800
#define RK_ISP_CTRL_ISP_GAMMA_OUT_ENA_READ(value)   (((value) >> 11) & 1)
/* DPCC */
#define RKISP_DPCC_ENA                          (1<<0)
#define RKISP_DPCC_DIS                          (0<<0)
#define RKISP_DPCC_MODE_MAX                     (0x07)
#define RKISP_DPCC_OUTPUTMODE_MAX               (0x0f)
#define RKISP_DPCC_SETUSE_MAX                   (0x0f)
#define RKISP_DPCC_METHODS_SET_RESERVED         (0xFFFFE000)
#define RKISP_DPCC_LINE_THRESH_RESERVED         (0xFFFF0000)
#define RKISP_DPCC_LINE_MAD_FAC_RESERVED        (0xFFFFC0C0)
#define RKISP_DPCC_PG_FAC_RESERVED              (0xFFFFC0C0)
#define RKISP_DPCC_RND_THRESH_RESERVED          (0xFFFF0000)
#define RKISP_DPCC_RG_FAC_RESERVED              (0xFFFFC0C0)
#define RKISP_DPCC_RO_LIMIT_RESERVED            (0xFFFFF000)
#define RKISP_DPCC_RND_OFFS_RESERVED            (0xFFFFF000)
/* BLS */
#define RKISP_BLS_ENA                           (1 << 0)
#define RKISP_BLS_DIS                           (0 << 0)
#define RKISP_BLS_MODE_MEASURED                 (1 << 1)
#define RKISP_BLS_MODE_FIXED                    (0 << 1)
#define RKISP_BLS_WINDOW_1                      (1 << 2)
#define RKISP_BLS_WINDOW_2                      (1 << 3)
/* GAMMA-IN */
#define RKISP_DEGAMMA_X_RESERVED                ((1 << 31)|(1 << 27)|(1 << 23)|(1 << 19)|\
                                                (1 << 15)|(1 << 11)|(1 << 7)|(1 << 3))
#define RKISP_DEGAMMA_Y_RESERVED                0xFFFFF000
/*CPROC*/
#define RKISP_CPROC_CTRL_RESERVED               0xFFFFFFFE
#define RKISP_CPROC_CONTRAST_RESERVED           0xFFFFFF00
#define RKISP_CPROC_BRIGHTNESS_RESERVED         0xFFFFFF00
#define RKISP_CPROC_HUE_RESERVED                0xFFFFFF00
#define RKISP_CPROC_SATURATION_RESERVED         0xFFFFFF00
#define RKISP_CPROC_MACC_RESERVED               0xE000E000
#define RKISP_CPROC_TONE_RESERVED               0xF000
#define RKISP_CPROC_TONE_Y(value)               ((value)<<16)
#define RKISP_CPROC_TONE_C(value)               ((value))
#define RKISP_CPROC_TONE_Y_READ(value)          ((value)>>16)
#define RKISP_CPROC_TONE_C_READ(value)          ((value)&0xFFFF)
#define RKISP_CPROC_EN                          1
#define RKISP_CPROC_MACC_EN                     (1<<4)
#define RKISP_CPROC_TMAP_EN                     (1<<5)
/* LSC */
#define RKISP_LSC_SECT_SIZE_RESERVED            0xFC00FC00
#define RKISP_LSC_GRAD_RESERVED                 0xF000F000
#define RKISP_LSC_SAMPLE_RESERVED               0xF000F000
#define RKISP_LSC_SECTORS_MAX                   16
#define RKISP_LSC_TABLE_DATA(v0, v1)            (v0 | (v1 << 12))
#define RKISP_LSC_SECT_SIZE(v0, v1)             (v0 | (v1 << 16))
#define RKISP_LSC_GRAD_SIZE(v0, v1)             (v0 | (v1 << 16))
/* AFC */
#define RKISP_AFC_THRES_RESERVED                0xFFFF0000
#define RKISP_AFC_VAR_SHIFT_RESERVED            0xFFF8FFF8
#define RKISP_AFC_WINDOW_X_RESERVED             0xE000
#define RKISP_AFC_WINDOW_Y_RESERVED             0xF000
#define RKISP_AFC_WINDOW_X_MIN                  0x5
#define RKISP_AFC_WINDOW_Y_MIN                  0x2
#define RKISP_AFC_WINDOW_X(value)               ((value)<<16)
#define RKISP_AFC_WINDOW_Y(value)               (value)
#define RKISP_AFC_ENA                           (1)
#define RKISP_AFC_DIS                           (0)
/* WDR */
#define RKISP_WDR_MODE_MAX                      (1)

/* DPF */
#define RKISP_DPF_NF_GAIN_RESERVED              0xFFFFF000
#define RKISP_DPF_SPATIAL_COEFF_MAX             0x1f
#define RKISP_DPF_NLL_COEFF_N_MAX               0x3ff

#define RKISP_DPF_MODE_USE_NF_GAIN              (1 << 9)
#define RKISP_DPF_MODE_LSC_GAIN_COMP            (1 << 8)
#define RKISP_DPF_MODE_AWB_GAIN_COMP            (1 << 7)
#define RKISP_DPF_MODE_NLL_SEGMENTATION(a)      (a << 6)
#define RKISP_DPF_MODE_RB_FLTSIZE(a)            (a << 5)
#define RKISP_DPF_MODE_R_FLT_DIS                (1 << 4)
#define RKISP_DPF_MODE_R_FLT_EN                 (0 << 4)
#define RKISP_DPF_MODE_GR_FLT_DIS               (1 << 3)
#define RKISP_DPF_MODE_GR_FLT_EN                (0 << 3)
#define RKISP_DPF_MODE_GB_FLT_DIS               (1 << 2)
#define RKISP_DPF_MODE_GB_FLT_EN                (0 << 2)
#define RKISP_DPF_MODE_B_FLT_DIS                (1 << 1)
#define RKISP_DPF_MODE_B_FLT_EN                 (0 << 1)
#define RKISP_DPF_MODE_EN                       (1 << 0)

#define RKISP_FLT_INIT_PARAM_MODE                (1)
#define RKISP_FLT_INIT_PARAM_GRNSTAGE            (6)
#define RKISP_FLT_INIT_PARAM_CHRHMODE            (3)
#define RKISP_FLT_INIT_PARAM_CHRVMODE            (3)
#define RKISP_FLT_INIT_PARAM_THRESHBLO           (15)
#define RKISP_FLT_INIT_PARAM_THRESHBL1           (9)
#define RKISP_FLT_INIT_PARAM_THRESHSH0           (50)
#define RKISP_FLT_INIT_PARAM_THRESHSH1           (70)
#define RKISP_FLT_INIT_PARAM_LUMWEIGHT           (204864)
#define RKISP_FLT_INIT_PARAM_FACSH1              (30)
#define RKISP_FLT_INIT_PARAM_FACSH0              (30)
#define RKISP_FLT_INIT_PARAM_FACMID              (26)
#define RKISP_FLT_INIT_PARAM_FACBL0              (26)
#define RKISP_FLT_INIT_PARAM_FACBL1              (23)

#define RKISP_DEBUG                             (1<<0)
#define RKISP_ERROR                             (1<<1)

/* Empirical rough (relative) times it takes to perform
    given function. */
#define RKISP_MODULE_DPCC_PROC_TIME             3
#define RKISP_MODULE_BLS_PROC_TIME              10
#define RKISP_MODULE_LSC_PROC_TIME              1747
#define RKISP_MODULE_FLT_PROC_TIME              15
#define RKISP_MODULE_BDM_PROC_TIME              1
#define RKISP_MODULE_SDG_PROC_TIME              53
#define RKISP_MODULE_GOC_PROC_TIME              1000
#define RKISP_MODULE_CTK_PROC_TIME              772
#define RKISP_MODULE_AWB_PROC_TIME              8
#define RKISP_MODULE_HST_PROC_TIME              5
#define RKISP_MODULE_AEC_PROC_TIME              5
#define RKISP_MODULE_AWB_GAIN_PROC_TIME         2
#define RKISP_MODULE_CPROC_PROC_TIME            5
#define RKISP_MODULE_AFC_PROC_TIME              8
#define RKISP_MODULE_IE_PROC_TIME               5
#define RKISP_MODULE_DPF_TIME                   5
#define RKISP_MODULE_DPF_STRENGTH_TIME          2
#define RKISP_MODULE_CSM_PROC_TIME              8
#define RKISP_MODULE_WDR_TIME                   8


#define BUG() do { \
       rt_kprintf("BUG: failure at %s:%d/%s()!\n", __FILE__, __LINE__, __func__); \
} while (0)

enum rk_isp_pix_fmt;

struct rk_isp_cfgs_log
{
    unsigned int s_frame_id[2];
    unsigned int new_id;
    unsigned int curr_id;
};

struct rk_isp_other_cfgs
{
    struct rk_isp_cfgs_log log[RK_ISP_MODULE_MAX];
    struct rk_isp_other_cfg cfgs[2];
    unsigned int module_updates;
    unsigned int module_actives;
};

struct rk_isp_meas_cfgs
{
    struct rk_isp_cfgs_log log[RK_ISP_MODULE_MAX];
    struct rk_isp_meas_cfg cfgs[2];
    unsigned int module_updates;
    unsigned int module_actives;
};

struct rk_isp_meas_stats
{
    unsigned int g_frame_id;
    struct rk_isp_stat_buffer stat;
};

struct rt_rkisp_isp_dev
{
    /* Current ISP parameters */

    rt_mutex_t isp_mutex;
    struct rk_isp_other_cfgs other_cfgs;
    struct rk_isp_meas_cfgs meas_cfgs;
    struct rk_isp_meas_stats meas_stats;

    bool rk_ism_cropping;

    enum rk_isp_pix_fmt_quantization quantization;

    /* input resolution needed for LSC param check */
    unsigned int input_width;
    unsigned int input_height;
    unsigned int active_lsc_width;
    unsigned int active_lsc_height;
    unsigned int v_blanking_us;

    unsigned int frame_id;
    unsigned int frame_id_setexp;
    unsigned int active_meas;
    unsigned int meas_send_alone;

    bool awb_meas_ready;
    bool afm_meas_ready;
    bool aec_meas_ready;
    bool hst_meas_ready;

    unsigned int vs_t; /* updated each frame */
    unsigned int fi_t; /* updated each frame */
};

enum rt_rkisp_readout_cmd
{
    RT_RKISP_READOUT_MEAS = 0,
    RT_RKISP_READOUT_META = 1,
};

int rt_do_cmd_ctrl_rkisp_module(
    struct rt_rkisp_isp_dev *isp,
    unsigned int cmd,
    void *arg);
void rt_do_configure_rkisp(
    struct rt_rkisp_isp_dev *isp_dev,
    enum rk_isp_pix_fmt in_pix_fmt,
    enum rk_isp_pix_fmt_quantization quantization);
void rt_do_disable_rkisp(struct rt_rkisp_isp_dev *isp_dev);
int rt_do_set_rkisp_isp_module_interrupt(struct rt_rkisp_isp_dev *isp_dev, unsigned int isp_mis);
void rt_do_clear_rkisp_readout_wq(struct rt_rkisp_isp_dev *isp_dev);
void rt_do_rkisp_v_start(struct rt_rkisp_isp_dev *isp_dev, unsigned int *timestamp);
void rt_do_rkisp_frame_in(
    struct rt_rkisp_isp_dev *isp_dev,
    unsigned int *fi_t);
void rt_do_rkisp_frame_id_reset(
    struct rt_rkisp_isp_dev *isp_dev);
int rt_do_rkisp_meas_queue_work(struct rt_rkisp_isp_dev *isp_dev,
                                unsigned int send_meas, bool force_send);
int rt_do_rkisp_open(struct rt_rkisp_isp_dev *isp_dev);

int rt_do_rkisp_vb_metadata(
    struct rt_rkisp_isp_dev *isp_dev);

int rt_do_rkisp_set_mod(struct rt_rkisp_isp_dev *isp_dev, int *value, unsigned int mod_id);

int rt_do_rkisp_get_mod(struct rt_rkisp_isp_dev *isp_dev, int *value, unsigned int mod_id);

void rt_do_get_meas_data_buf(struct rt_rkisp_isp_dev *isp_dev, void *arg);

void rt_do_dump_rkisp_reg(struct rt_rkisp_isp_dev *isp_dev, int level, int active_flag);

#endif

#endif

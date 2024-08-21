/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_isp_module.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef __DRV_ISP_MODULE_H__
#define __DRV_ISP_MODULE_H__

#include <rthw.h>
#include "common.h"

#define ISP_MODULE_RAWAE0    BIT(0)
#define ISP_MODULE_RAWAE1    BIT(1)
#define ISP_MODULE_RAWAWB    BIT(2)
#define ISP_MODULE_BLS       BIT(3)
#define ISP_MODULE_AWBGAIN   BIT(4)

#define ISP_RAWAELITE_MEAN_NUM 25
#define ISP_RAWAEBIG_MEAN_NUM 225

#define ISP_BAYNR_XY_NUM      16
#define ISP_BAYNR_GAIN_NUM    16
#define ISP_BAY3D_XY_NUM      16

#define BAY3D_BUF_ADDR                    RT_USING_ISP_DDR_ADRESS+0x100000

#define ISP_PACK_4BYTE(a, b, c, d)  \
    (((a) & 0xFF) << 0 | ((b) & 0xFF) << 8 | \
     ((c) & 0xFF) << 16 | ((d) & 0xFF) << 24)

#define ISP_PACK_2SHORT(a, b)   \
    (((a) & 0xFFFF) << 0 | ((b) & 0xFFFF) << 16)

struct rk_isp_dev;

/***************************params***************************/

struct isp_bls_cfg
{
    int16_t r;
    int16_t gr;
    int16_t gb;
    int16_t b;
};

struct isp_awbgain_cfg
{
    uint16_t gb;
    uint16_t gr;
    uint16_t b;
    uint16_t r;
};

struct isp_baynr_cfg
{
    /* BAYNR_CTRL */
    uint8_t bay3d_gain_en;
    uint8_t lg2_mode;
    uint8_t gauss_en;
    uint8_t log_bypass;
    /* BAYNR_DGAIN */
    uint16_t dgain1;
    uint16_t dgain0;
    uint16_t dgain2;
    /* BAYNR_PIXDIFF */
    uint16_t pix_diff;
    /* BAYNR_THLD */
    uint16_t diff_thld;
    uint16_t softthld;
    /* BAYNR_W1_STRENG */
    uint16_t bltflt_streng;
    uint16_t reg_w1;
    /* BAYNR_SIGMA */
    uint16_t sigma_x[ISP_BAYNR_XY_NUM];
    uint16_t sigma_y[ISP_BAYNR_XY_NUM];
    /* BAYNR_WRIT_D */
    uint16_t weit_d2;
    uint16_t weit_d1;
    uint16_t weit_d0;
    /* BAYNR_LG_OFF */
    uint16_t lg2_lgoff;
    uint16_t lg2_off;
    /* BAYNR_DAT_MAX */
    uint32_t dat_max;
    /* BAYNR_SIGOFF */
    uint16_t rgain_off;
    uint16_t bgain_off;
    /* BAYNR_GAIN */
    uint8_t gain_x[ISP_BAYNR_GAIN_NUM];
    uint16_t gain_y[ISP_BAYNR_GAIN_NUM];
} ;

struct isp_bay3d_cfg
{
    uint8_t is_first;
    /* BAY3D_CTRL */
    uint8_t bypass_en;
    uint8_t hibypass_en;
    uint8_t lobypass_en;
    uint8_t himed_bypass_en;
    uint8_t higaus_bypass_en;
    uint8_t hiabs_possel;
    uint8_t hichnsplit_en;
    uint8_t lomed_bypass_en;
    uint8_t logaus5_bypass_en;
    uint8_t logaus3_bypass_en;
    uint8_t glbpk_en;
    uint8_t loswitch_protect;
    uint8_t bwsaving_en;
    /* BAY3D_CTRL1 */
    uint8_t hiwgt_opt_en;
    uint8_t hichncor_en;
    uint8_t bwopt_gain_dis;
    uint8_t lo4x8_en;
    uint8_t lo4x4_en;
    uint8_t hisig_ind_sel;
    uint8_t pksig_ind_sel;
    uint8_t iirwr_rnd_en;
    uint8_t curds_high_en;
    uint8_t higaus3_mode;
    uint8_t higaus5x5_en;
    uint8_t wgtmix_opt_en;

    /* for isp32_lite */
    uint8_t wgtmm_opt_en;
    uint8_t wgtmm_sel_en;

    /* BAY3D_SIGGAUS */
    uint8_t siggaus0;
    uint8_t siggaus1;
    uint8_t siggaus2;
    uint8_t siggaus3;
    /* BAY3D_KALRATIO */
    uint16_t softwgt;
    uint16_t hidif_th;
    /* BAY3D_WGTLMT */
    uint16_t wgtlmt;
    uint16_t wgtratio;
    /* BAY3D_SIG */
    uint16_t sig0_x[ISP_BAY3D_XY_NUM];
    uint16_t sig0_y[ISP_BAY3D_XY_NUM];
    uint16_t sig1_x[ISP_BAY3D_XY_NUM];
    uint16_t sig1_y[ISP_BAY3D_XY_NUM];
    uint16_t sig2_x[ISP_BAY3D_XY_NUM];
    uint16_t sig2_y[ISP_BAY3D_XY_NUM];

    /* LODIF_STAT1 for isp32_lite */
    uint16_t wgtmin;

    /* BAY3D_HISIGRAT */
    uint16_t hisigrat0;
    uint16_t hisigrat1;
    /* BAY3D_HISIGOFF */
    uint16_t hisigoff0;
    uint16_t hisigoff1;
    /* BAY3D_LOSIG */
    uint16_t losigoff;
    uint16_t losigrat;
    /* BAY3D_SIGPK */
    uint16_t rgain_off;
    uint16_t bgain_off;
    /* BAY3D_GLBPK2 */
    uint32_t glbpk2;

    /*IIR buf size*/
    uint32_t wsize;
    uint32_t cur_size;
    uint32_t iir_size;
    uint32_t ds_size;
    uint32_t wrap_line;
    uint32_t buf_off;
} ;

struct rkisp_params
{
    struct isp_bls_cfg bls;
    struct isp_awbgain_cfg awbgain;
    struct isp_baynr_cfg baynr;
    struct isp_bay3d_cfg bay3d;
};

struct rkisp_params_buffer
{
    uint32_t module_cfg_update;
    uint32_t module_en_update;
    uint32_t module_ens;

    uint32_t frame_id;
    struct rkisp_params params;
};

/***************************stats****************************/

struct isp_rawae_meas_data
{
    uint16_t channelg_xy;
    uint16_t channelb_xy;
    uint16_t channelr_xy;
};

struct isp_rawaelite_stat
{
    struct isp_rawae_meas_data data[ISP_RAWAELITE_MEAN_NUM];
};

struct isp_rawaebig_stat
{
    struct isp_rawae_meas_data data[ISP_RAWAEBIG_MEAN_NUM];
};

struct isp_rawawb_stat
{
    uint32_t rgain_nor;
    uint32_t bgain_nor;
    uint32_t wp_num_nor;
    uint32_t wp_num2;

    uint32_t rgain_big;
    uint32_t bgain_big;
    uint32_t wp_num_big;
};

struct isp_baytnr_stat
{
    uint32_t sum_lodif_0;
    uint32_t sum_lodif_1;
    uint32_t sum_hidif_0;
    uint32_t sum_hidif_1;
};

struct rkisp_stats
{
    struct isp_rawaelite_stat rawae0;
    struct isp_rawaebig_stat rawae1;
    struct isp_rawawb_stat rawawb;
    struct isp_baytnr_stat baytnr;
};

struct rkisp_stats_buffer
{
    uint32_t meas_type;
    uint32_t frame_id;
    struct rkisp_stats stats;
};

/*************************************************************/

rt_err_t rk_isp_module_init(struct rk_isp_dev *dev);
rt_err_t rk_isp_set_params(struct rk_isp_dev *dev, struct rkisp_params_buffer *buf);
rt_err_t rk_isp_get_stat(struct rk_isp_dev *dev, struct rkisp_stats_buffer *buf);
rt_err_t rk_isp_isr(struct rk_isp_dev *dev, uint32_t mis);

#endif


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

#ifndef _UAPI_RKISP2_CONFIG_H
#define _UAPI_RKISP2_CONFIG_H

#define RKISP_API_VERSION       KERNEL_VERSION(1, 9, 0)

/*************************************************************/

#ifndef BIT_ULL
#define BIT_ULL(x)      (1ULL << (x))
#endif

#ifndef BIT
#define BIT(x) (~0ULL & (1ULL << x))
#endif

#define ISP2X_ID_DPCC           (0)
#define ISP2X_ID_BLS            (1)
#define ISP2X_ID_SDG            (2)
#define ISP2X_ID_SIHST          (3)
#define ISP2X_ID_LSC            (4)
#define ISP2X_ID_AWB_GAIN       (5)
#define ISP2X_ID_BDM            (7)
#define ISP2X_ID_CCM            (8)
#define ISP2X_ID_GOC            (9)
#define ISP2X_ID_CPROC          (10)
#define ISP2X_ID_SIAF           (11)
#define ISP2X_ID_SIAWB          (12)
#define ISP2X_ID_IE         (13)
#define ISP2X_ID_YUVAE          (14)
#define ISP2X_ID_WDR            (15)
#define ISP2X_ID_RK_IESHARP     (16)
#define ISP2X_ID_RAWAF          (17)
#define ISP2X_ID_RAWAE0         (18)
#define ISP2X_ID_RAWAE1         (19)
#define ISP2X_ID_RAWAE2         (20)
#define ISP2X_ID_RAWAE3         (21)
#define ISP2X_ID_RAWAWB         (22)
#define ISP2X_ID_RAWHIST0       (23)
#define ISP2X_ID_RAWHIST1       (24)
#define ISP2X_ID_RAWHIST2       (25)
#define ISP2X_ID_RAWHIST3       (26)
#define ISP2X_ID_HDRMGE         (27)
#define ISP2X_ID_RAWNR          (28)
#define ISP2X_ID_HDRTMO         (29)
#define ISP2X_ID_GIC            (30)
#define ISP2X_ID_DHAZ           (31)
#define ISP2X_ID_3DLUT          (32)
#define ISP2X_ID_LDCH           (33)
#define ISP2X_ID_GAIN           (34)
#define ISP2X_ID_DEBAYER        (35)
#define ISP2X_ID_MAX            (63)

#define ISP2X_MODULE_DPCC       BIT_ULL(ISP2X_ID_DPCC)
#define ISP2X_MODULE_BLS        BIT_ULL(ISP2X_ID_BLS)
#define ISP2X_MODULE_SDG        BIT_ULL(ISP2X_ID_SDG)
#define ISP2X_MODULE_SIHST      BIT_ULL(ISP2X_ID_SIHST)
#define ISP2X_MODULE_LSC        BIT_ULL(ISP2X_ID_LSC)
#define ISP2X_MODULE_AWB_GAIN       BIT_ULL(ISP2X_ID_AWB_GAIN)
#define ISP2X_MODULE_BDM        BIT_ULL(ISP2X_ID_BDM)
#define ISP2X_MODULE_CCM        BIT_ULL(ISP2X_ID_CCM)
#define ISP2X_MODULE_GOC        BIT_ULL(ISP2X_ID_GOC)
#define ISP2X_MODULE_CPROC      BIT_ULL(ISP2X_ID_CPROC)
#define ISP2X_MODULE_SIAF       BIT_ULL(ISP2X_ID_SIAF)
#define ISP2X_MODULE_SIAWB      BIT_ULL(ISP2X_ID_SIAWB)
#define ISP2X_MODULE_IE         BIT_ULL(ISP2X_ID_IE)
#define ISP2X_MODULE_YUVAE      BIT_ULL(ISP2X_ID_YUVAE)
#define ISP2X_MODULE_WDR        BIT_ULL(ISP2X_ID_WDR)
#define ISP2X_MODULE_RK_IESHARP     BIT_ULL(ISP2X_ID_RK_IESHARP)
#define ISP2X_MODULE_RAWAF      BIT_ULL(ISP2X_ID_RAWAF)
#define ISP2X_MODULE_RAWAE0     BIT_ULL(ISP2X_ID_RAWAE0)
#define ISP2X_MODULE_RAWAE1     BIT_ULL(ISP2X_ID_RAWAE1)
#define ISP2X_MODULE_RAWAE2     BIT_ULL(ISP2X_ID_RAWAE2)
#define ISP2X_MODULE_RAWAE3     BIT_ULL(ISP2X_ID_RAWAE3)
#define ISP2X_MODULE_RAWAWB     BIT_ULL(ISP2X_ID_RAWAWB)
#define ISP2X_MODULE_RAWHIST0       BIT_ULL(ISP2X_ID_RAWHIST0)
#define ISP2X_MODULE_RAWHIST1       BIT_ULL(ISP2X_ID_RAWHIST1)
#define ISP2X_MODULE_RAWHIST2       BIT_ULL(ISP2X_ID_RAWHIST2)
#define ISP2X_MODULE_RAWHIST3       BIT_ULL(ISP2X_ID_RAWHIST3)
#define ISP2X_MODULE_HDRMGE     BIT_ULL(ISP2X_ID_HDRMGE)
#define ISP2X_MODULE_RAWNR      BIT_ULL(ISP2X_ID_RAWNR)
#define ISP2X_MODULE_HDRTMO     BIT_ULL(ISP2X_ID_HDRTMO)
#define ISP2X_MODULE_GIC        BIT_ULL(ISP2X_ID_GIC)
#define ISP2X_MODULE_DHAZ       BIT_ULL(ISP2X_ID_DHAZ)
#define ISP2X_MODULE_3DLUT      BIT_ULL(ISP2X_ID_3DLUT)
#define ISP2X_MODULE_LDCH       BIT_ULL(ISP2X_ID_LDCH)
#define ISP2X_MODULE_GAIN       BIT_ULL(ISP2X_ID_GAIN)
#define ISP2X_MODULE_DEBAYER        BIT_ULL(ISP2X_ID_DEBAYER)

#define ISP2X_MODULE_FORCE      BIT_ULL(ISP2X_ID_MAX)

/*
 * Measurement types
 */
#define ISP2X_STAT_SIAWB        BIT(0)
#define ISP2X_STAT_YUVAE        BIT(1)
#define ISP2X_STAT_SIAF         BIT(2)
#define ISP2X_STAT_SIHST        BIT(3)
#define ISP2X_STAT_EMB_DATA     BIT(4)
#define ISP2X_STAT_RAWAWB       BIT(5)
#define ISP2X_STAT_RAWAF        BIT(6)
#define ISP2X_STAT_RAWAE0       BIT(7)
#define ISP2X_STAT_RAWAE1       BIT(8)
#define ISP2X_STAT_RAWAE2       BIT(9)
#define ISP2X_STAT_RAWAE3       BIT(10)
#define ISP2X_STAT_RAWHST0      BIT(11)
#define ISP2X_STAT_RAWHST1      BIT(12)
#define ISP2X_STAT_RAWHST2      BIT(13)
#define ISP2X_STAT_RAWHST3      BIT(14)
#define ISP2X_STAT_BLS          BIT(15)
#define ISP2X_STAT_HDRTMO       BIT(16)
#define ISP2X_STAT_DHAZ         BIT(17)

#define ISP2X_LSC_GRAD_TBL_SIZE     8
#define ISP2X_LSC_SIZE_TBL_SIZE     8
#define ISP2X_LSC_DATA_TBL_SIZE     290

#define ISP2X_DEGAMMA_CURVE_SIZE    17

#define ISP2X_GAIN_HDRMGE_GAIN_NUM  3
#define ISP2X_GAIN_IDX_NUM      15
#define ISP2X_GAIN_LUT_NUM      17

#define ISP2X_AWB_MAX_GRID      1
#define ISP2X_RAWAWB_SUM_NUM        7
#define ISP2X_RAWAWB_MULWD_NUM      8
#define ISP2X_RAWAWB_RAMDATA_NUM    225

#define ISP2X_RAWAEBIG_SUBWIN_NUM   4
#define ISP2X_RAWAEBIG_MEAN_NUM     225
#define ISP2X_RAWAELITE_MEAN_NUM    25
#define ISP2X_YUVAE_SUBWIN_NUM      4
#define ISP2X_YUVAE_MEAN_NUM        225

#define ISP2X_RAWHISTBIG_SUBWIN_NUM 225
#define ISP2X_RAWHISTLITE_SUBWIN_NUM    25
#define ISP2X_SIHIST_WIN_NUM        1
#define ISP2X_HIST_WEIGHT_NUM       225
#define ISP2X_HIST_BIN_N_MAX        256
#define ISP2X_SIHIST_BIN_N_MAX      32

#define ISP2X_RAWAF_WIN_NUM     2
#define ISP2X_RAWAF_LINE_NUM        5
#define ISP2X_RAWAF_GAMMA_NUM       17
#define ISP2X_RAWAF_SUMDATA_ROW     15
#define ISP2X_RAWAF_SUMDATA_COLUMN  15
#define ISP2X_RAWAF_SUMDATA_NUM     225
#define ISP2X_AFM_MAX_WINDOWS       3

#define ISP2X_DPCC_PDAF_POINT_NUM   16

#define ISP2X_HDRMGE_L_CURVE_NUM    17
#define ISP2X_HDRMGE_E_CURVE_NUM    17

#define ISP2X_RAWNR_LUMA_RATION_NUM 8

#define ISP2X_HDRTMO_MINMAX_NUM     32

#define ISP2X_GIC_SIGMA_Y_NUM       15

#define ISP2X_CCM_CURVE_NUM     17

/* WDR */
#define ISP2X_WDR_SIZE          48

#define ISP2X_DHAZ_CONV_COEFF_NUM   6
#define ISP2X_DHAZ_HIST_IIR_NUM     64

#define ISP2X_GAMMA_OUT_MAX_SAMPLES 45

#define ISP2X_MIPI_LUMA_MEAN_MAX    16
#define ISP2X_MIPI_RAW_MAX      3
#define ISP2X_RAW0_Y_STATE      (1 << 0)
#define ISP2X_RAW1_Y_STATE      (1 << 1)
#define ISP2X_RAW2_Y_STATE      (1 << 2)

#define ISP2X_3DLUT_DATA_NUM        729

#define ISP2X_LDCH_MESH_XY_NUM      0x80000
#define ISP2X_LDCH_BUF_NUM      2

#define ISP2X_THUNDERBOOT_VIDEO_BUF_NUM 30

#define ISP2X_FBCBUF_FD_NUM     64

#define ISP2X_MESH_BUF_NUM      2

enum rkisp_isp_mode
{
    /* frame input related */
    RKISP_ISP_NORMAL = BIT(0),
    RKISP_ISP_HDR2 = BIT(1),
    RKISP_ISP_HDR3 = BIT(2),
    RKISP_ISP_COMPR = BIT(3),

    /* isp function related */
    RKISP_ISP_BIGMODE = BIT(28),
};

struct rkisp_isp_info
{
    enum rkisp_isp_mode mode;
    uint32_t act_width;
    uint32_t act_height;
    uint8_t compr_bit;
} __attribute__((packed));

enum isp2x_mesh_buf_stat
{
    MESH_BUF_INIT = 0,
    MESH_BUF_WAIT2CHIP,
    MESH_BUF_CHIPINUSE,
};

struct rkisp_meshbuf_info
{
    uint64_t module_id;
    uint32_t unite_isp_id;
    int32_t buf_fd[ISP2X_MESH_BUF_NUM];
    uint32_t buf_size[ISP2X_MESH_BUF_NUM];
} __attribute__((packed));

struct rkisp_meshbuf_size
{
    uint64_t module_id;
    uint32_t unite_isp_id;
    uint32_t meas_width;
    uint32_t meas_height;
} __attribute__((packed));

struct isp2x_mesh_head
{
    enum isp2x_mesh_buf_stat stat;
    uint32_t data_oft;
} __attribute__((packed));

#define RKISP_CMSK_WIN_MAX 8
#define RKISP_CMSK_MOSAIC_MODE 0
#define RKISP_CMSK_COVER_MODE 1

/* struct rkisp_cmsk_win
 * Priacy Mask Window configture, support 8 windows, and
 * support for mainpath and selfpath output stream channel.
 *
 * mode: 0:mosaic mode, 1:cover mode
 * win_index: window index 0~7. windows overlap, priority win7 > win0.
 * cover_color_y: cover mode effective, share for stream channel when same win_index.
 * cover_color_u: cover mode effective, share for stream channel when same win_index.
 * cover_color_v: cover mode effective, share for stream channel when same win_index.
 *
 * h_offs: window horizontal offset, share for stream channel when same win_index. 2 align.
 * v_offs: window vertical offset, share for stream channel when same win_index. 2 align.
 * h_size: window horizontal size, share for stream channel when same win_index. 8 align.
 * v_size: window vertical size, share for stream channel when same win_index. 8 align.
 */
struct rkisp_cmsk_win
{
    unsigned char mode;
    unsigned char win_en;

    unsigned char cover_color_y;
    unsigned char cover_color_u;
    unsigned char cover_color_v;

    unsigned short h_offs;
    unsigned short v_offs;
    unsigned short h_size;
    unsigned short v_size;
} __attribute__((packed));

/* struct rkisp_cmsk_cfg
 * win: priacy mask window
 * width_ro: isp full resolution, h_offs + h_size <= width_ro.
 * height_ro: isp full resolution, v_offs + v_size <= height_ro.
 */
struct rkisp_cmsk_cfg
{
    struct rkisp_cmsk_win win[RKISP_CMSK_WIN_MAX];
    unsigned int width_ro;
    unsigned int height_ro;
} __attribute__((packed));

/* struct rkisp_stream_info
 * cur_frame_id: stream current frame id
 * input_frame_loss: isp input frame loss num
 * output_frame_loss: stream output frame loss num
 * stream_on: stream on/off
 */
struct rkisp_stream_info
{
    unsigned int cur_frame_id;
    unsigned int input_frame_loss;
    unsigned int output_frame_loss;
    unsigned char stream_on;
} __attribute__((packed));

/* struct rkisp_mirror_flip
 * mirror: global for all output stream
 * flip: independent for all output stream
 */
struct rkisp_mirror_flip
{
    unsigned char mirror;
    unsigned char flip;
} __attribute__((packed));

/* trigger event mode
 * T_TRY: trigger maybe with retry
 * T_TRY_YES: trigger to retry
 * T_TRY_NO: trigger no to retry
 *
 * T_START_X1: isp read one frame
 * T_START_X2: isp read hdr two frame
 * T_START_X3: isp read hdr three frame
 * T_START_C: isp read hdr linearised and compressed data
 */
enum isp2x_trigger_mode
{
    T_TRY = BIT(0),
    T_TRY_YES = BIT(1),
    T_TRY_NO = BIT(2),

    T_START_X1 = BIT(4),
    T_START_X2 = BIT(5),
    T_START_X3 = BIT(6),
    T_START_C = BIT(7),
};

struct isp2x_csi_trigger
{
    /* timestamp in ns */
    uint64_t sof_timestamp;
    uint64_t frame_timestamp;
    uint32_t frame_id;
    int times;
    enum isp2x_trigger_mode mode;
} __attribute__((packed));

/* isp csi dmatx/dmarx memory mode
 * 0: raw12/raw10/raw8 8bit memory compact
 * 1: raw12/raw10 16bit memory one pixel
 *    big endian for rv1126/rv1109
 *    |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 *    | 3| 2| 1| 0| -| -| -| -|11|10| 9| 8| 7| 6| 5| 4|
 *    little align for rk356x
 *    |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 *    | -| -| -| -|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 * 2: raw12/raw10 16bit memory one pixel
 *    big align for rv1126/rv1109/rk356x
 *    |15|14|13|12|11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0|
 *    |11|10| 9| 8| 7| 6| 5| 4| 3| 2| 1| 0| -| -| -| -|
 */
enum isp_csi_memory
{
    CSI_MEM_COMPACT = 0,
    CSI_MEM_WORD_BIG_END = 1,
    CSI_MEM_WORD_LITTLE_ALIGN = 1,
    CSI_MEM_WORD_BIG_ALIGN = 2,
};

#define RKISP_INFO2DDR_BUF_MAX  4
/* 32bit flag for user set to memory after buf used */
#define RKISP_INFO2DDR_BUF_INIT 0x5AA5

enum rkisp_info2ddr_owner
{
    RKISP_INFO2DRR_OWNER_NULL,
    RKISP_INFO2DRR_OWNER_GAIN,
    RKISP_INFO2DRR_OWNER_AWB,
};

/* struct rkisp_info2ddr
 * awb and gain debug info write to ddr
 *
 * owner: 0: off, 1: gain, 2: awb.
 * u: gain or awb mode parameters.
 * buf_cnt: buf num to request. return actual result.
 * buf_fd: fd of memory alloc result.
 * wsize: data width to request. if useless to 0. return actual result.
 * vsize: data height to request. if useless to 0. return actual result.
 */
struct rkisp_info2ddr
{
    enum rkisp_info2ddr_owner owner;

    union
    {
        struct
        {
            uint8_t gain2ddr_mode;
        } gain;

        struct
        {
            uint8_t awb2ddr_sel;
        } awb;
    } u;

    uint8_t buf_cnt;
    int32_t buf_fd[RKISP_INFO2DDR_BUF_MAX];

    uint32_t wsize;
    uint32_t vsize;
} __attribute__((packed));

struct isp2x_ispgain_buf
{
    uint32_t gain_dmaidx;
    uint32_t mfbc_dmaidx;
    uint32_t gain_size;
    uint32_t mfbc_size;
    uint32_t frame_id;
} __attribute__((packed));

struct isp2x_buf_idxfd
{
    uint32_t buf_num;
    uint32_t index[ISP2X_FBCBUF_FD_NUM];
    int32_t dmafd[ISP2X_FBCBUF_FD_NUM];
} __attribute__((packed));

struct isp2x_window
{
    uint16_t h_offs;
    uint16_t v_offs;
    uint16_t h_size;
    uint16_t v_size;
} __attribute__((packed));

struct isp2x_bls_fixed_val
{
    int16_t r;
    int16_t gr;
    int16_t gb;
    int16_t b;
} __attribute__((packed));

struct isp2x_bls_cfg
{
    uint8_t enable_auto;
    uint8_t en_windows;
    struct isp2x_window bls_window1;
    struct isp2x_window bls_window2;
    uint8_t bls_samples;
    struct isp2x_bls_fixed_val fixed_val;
} __attribute__((packed));

struct isp2x_bls_stat
{
    uint16_t meas_r;
    uint16_t meas_gr;
    uint16_t meas_gb;
    uint16_t meas_b;
} __attribute__((packed));

struct isp2x_dpcc_pdaf_point
{
    uint8_t y;
    uint8_t x;
} __attribute__((packed));

struct isp2x_dpcc_cfg
{
    //mode 0x0000
    uint8_t stage1_enable;
    uint8_t grayscale_mode;

    //output_mode 0x0004
    uint8_t sw_rk_out_sel;
    uint8_t sw_dpcc_output_sel;
    uint8_t stage1_rb_3x3;
    uint8_t stage1_g_3x3;
    uint8_t stage1_incl_rb_center;
    uint8_t stage1_incl_green_center;

    //set_use 0x0008
    uint8_t stage1_use_fix_set;
    uint8_t stage1_use_set_3;
    uint8_t stage1_use_set_2;
    uint8_t stage1_use_set_1;

    //methods_set_1 0x000c
    uint8_t sw_rk_red_blue1_en;
    uint8_t rg_red_blue1_enable;
    uint8_t rnd_red_blue1_enable;
    uint8_t ro_red_blue1_enable;
    uint8_t lc_red_blue1_enable;
    uint8_t pg_red_blue1_enable;
    uint8_t sw_rk_green1_en;
    uint8_t rg_green1_enable;
    uint8_t rnd_green1_enable;
    uint8_t ro_green1_enable;
    uint8_t lc_green1_enable;
    uint8_t pg_green1_enable;

    //methods_set_2 0x0010
    uint8_t sw_rk_red_blue2_en;
    uint8_t rg_red_blue2_enable;
    uint8_t rnd_red_blue2_enable;
    uint8_t ro_red_blue2_enable;
    uint8_t lc_red_blue2_enable;
    uint8_t pg_red_blue2_enable;
    uint8_t sw_rk_green2_en;
    uint8_t rg_green2_enable;
    uint8_t rnd_green2_enable;
    uint8_t ro_green2_enable;
    uint8_t lc_green2_enable;
    uint8_t pg_green2_enable;

    //methods_set_3 0x0014
    uint8_t sw_rk_red_blue3_en;
    uint8_t rg_red_blue3_enable;
    uint8_t rnd_red_blue3_enable;
    uint8_t ro_red_blue3_enable;
    uint8_t lc_red_blue3_enable;
    uint8_t pg_red_blue3_enable;
    uint8_t sw_rk_green3_en;
    uint8_t rg_green3_enable;
    uint8_t rnd_green3_enable;
    uint8_t ro_green3_enable;
    uint8_t lc_green3_enable;
    uint8_t pg_green3_enable;

    //line_thresh_1 0x0018
    uint8_t sw_mindis1_rb;
    uint8_t sw_mindis1_g;
    uint8_t line_thr_1_rb;
    uint8_t line_thr_1_g;

    //line_mad_fac_1 0x001c
    uint8_t sw_dis_scale_min1;
    uint8_t sw_dis_scale_max1;
    uint8_t line_mad_fac_1_rb;
    uint8_t line_mad_fac_1_g;

    //pg_fac_1 0x0020
    uint8_t pg_fac_1_rb;
    uint8_t pg_fac_1_g;

    //rnd_thresh_1 0x0024
    uint8_t rnd_thr_1_rb;
    uint8_t rnd_thr_1_g;

    //rg_fac_1 0x0028
    uint8_t rg_fac_1_rb;
    uint8_t rg_fac_1_g;

    //line_thresh_2 0x002c
    uint8_t sw_mindis2_rb;
    uint8_t sw_mindis2_g;
    uint8_t line_thr_2_rb;
    uint8_t line_thr_2_g;

    //line_mad_fac_2 0x0030
    uint8_t sw_dis_scale_min2;
    uint8_t sw_dis_scale_max2;
    uint8_t line_mad_fac_2_rb;
    uint8_t line_mad_fac_2_g;

    //pg_fac_2 0x0034
    uint8_t pg_fac_2_rb;
    uint8_t pg_fac_2_g;

    //rnd_thresh_2 0x0038
    uint8_t rnd_thr_2_rb;
    uint8_t rnd_thr_2_g;

    //rg_fac_2 0x003c
    uint8_t rg_fac_2_rb;
    uint8_t rg_fac_2_g;

    //line_thresh_3 0x0040
    uint8_t sw_mindis3_rb;
    uint8_t sw_mindis3_g;
    uint8_t line_thr_3_rb;
    uint8_t line_thr_3_g;

    //line_mad_fac_3 0x0044
    uint8_t sw_dis_scale_min3;
    uint8_t sw_dis_scale_max3;
    uint8_t line_mad_fac_3_rb;
    uint8_t line_mad_fac_3_g;

    //pg_fac_3 0x0048
    uint8_t pg_fac_3_rb;
    uint8_t pg_fac_3_g;

    //rnd_thresh_3 0x004c
    uint8_t rnd_thr_3_rb;
    uint8_t rnd_thr_3_g;

    //rg_fac_3 0x0050
    uint8_t rg_fac_3_rb;
    uint8_t rg_fac_3_g;

    //ro_limits 0x0054
    uint8_t ro_lim_3_rb;
    uint8_t ro_lim_3_g;
    uint8_t ro_lim_2_rb;
    uint8_t ro_lim_2_g;
    uint8_t ro_lim_1_rb;
    uint8_t ro_lim_1_g;

    //rnd_offs 0x0058
    uint8_t rnd_offs_3_rb;
    uint8_t rnd_offs_3_g;
    uint8_t rnd_offs_2_rb;
    uint8_t rnd_offs_2_g;
    uint8_t rnd_offs_1_rb;
    uint8_t rnd_offs_1_g;

    //bpt_ctrl 0x005c
    uint8_t bpt_rb_3x3;
    uint8_t bpt_g_3x3;
    uint8_t bpt_incl_rb_center;
    uint8_t bpt_incl_green_center;
    uint8_t bpt_use_fix_set;
    uint8_t bpt_use_set_3;
    uint8_t bpt_use_set_2;
    uint8_t bpt_use_set_1;
    uint8_t bpt_cor_en;
    uint8_t bpt_det_en;

    //bpt_number 0x0060
    uint16_t bp_number;

    //bpt_addr 0x0064
    uint16_t bp_table_addr;

    //bpt_data 0x0068
    uint16_t bpt_v_addr;
    uint16_t bpt_h_addr;

    //bp_cnt 0x006c
    uint32_t bp_cnt;

    //pdaf_en 0x0070
    uint8_t sw_pdaf_en;

    //pdaf_point_en 0x0074
    uint8_t pdaf_point_en[ISP2X_DPCC_PDAF_POINT_NUM];

    //pdaf_offset 0x0078
    uint16_t pdaf_offsety;
    uint16_t pdaf_offsetx;

    //pdaf_wrap 0x007c
    uint16_t pdaf_wrapy;
    uint16_t pdaf_wrapx;

    //pdaf_scope 0x0080
    uint16_t pdaf_wrapy_num;
    uint16_t pdaf_wrapx_num;

    //pdaf_point_0 0x0084
    struct isp2x_dpcc_pdaf_point point[ISP2X_DPCC_PDAF_POINT_NUM];

    //pdaf_forward_med 0x00a4
    uint8_t pdaf_forward_med;
} __attribute__((packed));

struct isp2x_hdrmge_curve
{
    uint16_t curve_1[ISP2X_HDRMGE_L_CURVE_NUM];
    uint16_t curve_0[ISP2X_HDRMGE_L_CURVE_NUM];
} __attribute__((packed));

struct isp2x_hdrmge_cfg
{
    uint8_t mode;

    uint16_t gain0_inv;
    uint16_t gain0;

    uint16_t gain1_inv;
    uint16_t gain1;

    uint8_t gain2;

    uint8_t lm_dif_0p15;
    uint8_t lm_dif_0p9;
    uint8_t ms_diff_0p15;
    uint8_t ms_dif_0p8;

    struct isp2x_hdrmge_curve curve;
    uint16_t e_y[ISP2X_HDRMGE_E_CURVE_NUM];
} __attribute__((packed));

struct isp2x_rawnr_cfg
{
    uint8_t gauss_en;
    uint8_t log_bypass;

    uint16_t filtpar0;
    uint16_t filtpar1;
    uint16_t filtpar2;

    uint32_t dgain0;
    uint32_t dgain1;
    uint32_t dgain2;

    uint16_t luration[ISP2X_RAWNR_LUMA_RATION_NUM];
    uint16_t lulevel[ISP2X_RAWNR_LUMA_RATION_NUM];

    uint32_t gauss;
    uint16_t sigma;
    uint16_t pix_diff;

    uint32_t thld_diff;

    uint8_t gas_weig_scl2;
    uint8_t gas_weig_scl1;
    uint16_t thld_chanelw;

    uint16_t lamda;

    uint16_t fixw0;
    uint16_t fixw1;
    uint16_t fixw2;
    uint16_t fixw3;

    uint32_t wlamda0;
    uint32_t wlamda1;
    uint32_t wlamda2;

    uint16_t rgain_filp;
    uint16_t bgain_filp;
} __attribute__((packed));

struct isp2x_lsc_cfg
{
    uint16_t r_data_tbl[ISP2X_LSC_DATA_TBL_SIZE];
    uint16_t gr_data_tbl[ISP2X_LSC_DATA_TBL_SIZE];
    uint16_t gb_data_tbl[ISP2X_LSC_DATA_TBL_SIZE];
    uint16_t b_data_tbl[ISP2X_LSC_DATA_TBL_SIZE];

    uint16_t x_grad_tbl[ISP2X_LSC_GRAD_TBL_SIZE];
    uint16_t y_grad_tbl[ISP2X_LSC_GRAD_TBL_SIZE];

    uint16_t x_size_tbl[ISP2X_LSC_SIZE_TBL_SIZE];
    uint16_t y_size_tbl[ISP2X_LSC_SIZE_TBL_SIZE];
} __attribute__((packed));

enum isp2x_goc_mode
{
    ISP2X_GOC_MODE_LOGARITHMIC,
    ISP2X_GOC_MODE_EQUIDISTANT
};

struct isp2x_goc_cfg
{
    enum isp2x_goc_mode mode;
    uint8_t gamma_y[17];
} __attribute__((packed));

struct isp2x_hdrtmo_predict
{
    uint8_t global_tmo;
    int32_t iir_max;
    int32_t global_tmo_strength;

    uint8_t scene_stable;
    int32_t k_rolgmean;
    int32_t iir;
} __attribute__((packed));

struct isp2x_hdrtmo_cfg
{
    uint16_t cnt_vsize;
    uint8_t gain_ld_off2;
    uint8_t gain_ld_off1;
    uint8_t big_en;
    uint8_t nobig_en;
    uint8_t newhst_en;
    uint8_t cnt_mode;

    uint16_t expl_lgratio;
    uint8_t lgscl_ratio;
    uint8_t cfg_alpha;

    uint16_t set_gainoff;
    uint16_t set_palpha;

    uint16_t set_lgmax;
    uint16_t set_lgmin;

    uint8_t set_weightkey;
    uint16_t set_lgmean;

    uint16_t set_lgrange1;
    uint16_t set_lgrange0;

    uint16_t set_lgavgmax;

    uint8_t clipgap1_i;
    uint8_t clipgap0_i;
    uint8_t clipratio1;
    uint8_t clipratio0;
    uint8_t ratiol;

    uint16_t lgscl_inv;
    uint16_t lgscl;

    uint16_t lgmax;

    uint16_t hist_low;
    uint16_t hist_min;

    uint8_t hist_shift;
    uint16_t hist_0p3;
    uint16_t hist_high;

    uint16_t palpha_lwscl;
    uint16_t palpha_lw0p5;
    uint16_t palpha_0p18;

    uint16_t maxgain;
    uint16_t maxpalpha;

    struct isp2x_hdrtmo_predict predict;
} __attribute__((packed));

struct isp2x_hdrtmo_stat
{
    uint16_t lglow;
    uint16_t lgmin;
    uint16_t lghigh;
    uint16_t lgmax;
    uint16_t weightkey;
    uint16_t lgmean;
    uint16_t lgrange1;
    uint16_t lgrange0;
    uint16_t palpha;
    uint16_t lgavgmax;
    uint16_t linecnt;
    uint32_t min_max[ISP2X_HDRTMO_MINMAX_NUM];
} __attribute__((packed));

struct isp2x_gic_cfg
{
    uint8_t edge_open;

    uint16_t regmingradthrdark2;
    uint16_t regmingradthrdark1;
    uint16_t regminbusythre;

    uint16_t regdarkthre;
    uint16_t regmaxcorvboth;
    uint16_t regdarktthrehi;

    uint8_t regkgrad2dark;
    uint8_t regkgrad1dark;
    uint8_t regstrengthglobal_fix;
    uint8_t regdarkthrestep;
    uint8_t regkgrad2;
    uint8_t regkgrad1;
    uint8_t reggbthre;

    uint16_t regmaxcorv;
    uint16_t regmingradthr2;
    uint16_t regmingradthr1;

    uint8_t gr_ratio;
    uint16_t dnloscale;
    uint16_t dnhiscale;
    uint8_t reglumapointsstep;

    uint16_t gvaluelimitlo;
    uint16_t gvaluelimithi;
    uint8_t fusionratiohilimt1;

    uint8_t regstrength_fix;

    uint16_t sigma_y[ISP2X_GIC_SIGMA_Y_NUM];

    uint8_t noise_cut_en;
    uint16_t noise_coe_a;

    uint16_t noise_coe_b;
    uint16_t diff_clip;
} __attribute__((packed));

struct isp2x_debayer_cfg
{
    uint8_t filter_c_en;
    uint8_t filter_g_en;

    uint8_t thed1;
    uint8_t thed0;
    uint8_t dist_scale;
    uint8_t max_ratio;
    uint8_t clip_en;

    int8_t filter1_coe5;
    int8_t filter1_coe4;
    int8_t filter1_coe3;
    int8_t filter1_coe2;
    int8_t filter1_coe1;

    int8_t filter2_coe5;
    int8_t filter2_coe4;
    int8_t filter2_coe3;
    int8_t filter2_coe2;
    int8_t filter2_coe1;

    uint16_t hf_offset;
    uint8_t gain_offset;
    uint8_t offset;

    uint8_t shift_num;
    uint8_t order_max;
    uint8_t order_min;
} __attribute__((packed));

struct isp2x_ccm_cfg
{
    int16_t coeff0_r;
    int16_t coeff1_r;
    int16_t coeff2_r;
    int16_t offset_r;

    int16_t coeff0_g;
    int16_t coeff1_g;
    int16_t coeff2_g;
    int16_t offset_g;

    int16_t coeff0_b;
    int16_t coeff1_b;
    int16_t coeff2_b;
    int16_t offset_b;

    uint16_t coeff0_y;
    uint16_t coeff1_y;
    uint16_t coeff2_y;

    uint16_t alp_y[ISP2X_CCM_CURVE_NUM];

    uint8_t bound_bit;
} __attribute__((packed));

struct isp2x_gammaout_cfg
{
    uint8_t equ_segm;
    uint16_t offset;
    uint16_t gamma_y[ISP2X_GAMMA_OUT_MAX_SAMPLES];
} __attribute__((packed));

enum isp2x_wdr_mode
{
    ISP2X_WDR_MODE_BLOCK,
    ISP2X_WDR_MODE_GLOBAL
};

struct isp2x_wdr_cfg
{
    enum isp2x_wdr_mode mode;
    unsigned int c_wdr[ISP2X_WDR_SIZE];
} __attribute__((packed));

struct isp2x_dhaz_cfg
{
    uint8_t enhance_en;
    uint8_t hist_chn;
    uint8_t hpara_en;
    uint8_t hist_en;
    uint8_t dc_en;
    uint8_t big_en;
    uint8_t nobig_en;

    uint8_t yblk_th;
    uint8_t yhist_th;
    uint8_t dc_max_th;
    uint8_t dc_min_th;

    uint16_t wt_max;
    uint8_t bright_max;
    uint8_t bright_min;

    uint8_t tmax_base;
    uint8_t dark_th;
    uint8_t air_max;
    uint8_t air_min;

    uint16_t tmax_max;
    uint16_t tmax_off;

    uint8_t hist_th_off;
    uint8_t hist_gratio;

    uint16_t hist_min;
    uint16_t hist_k;

    uint16_t enhance_value;
    uint16_t hist_scale;

    uint16_t iir_wt_sigma;
    uint16_t iir_sigma;
    uint16_t stab_fnum;

    uint16_t iir_tmax_sigma;
    uint16_t iir_air_sigma;

    uint16_t cfg_wt;
    uint16_t cfg_air;
    uint16_t cfg_alpha;

    uint16_t cfg_gratio;
    uint16_t cfg_tmax;

    uint16_t dc_weitcur;
    uint16_t dc_thed;

    uint8_t sw_dhaz_dc_bf_h3;
    uint8_t sw_dhaz_dc_bf_h2;
    uint8_t sw_dhaz_dc_bf_h1;
    uint8_t sw_dhaz_dc_bf_h0;

    uint8_t sw_dhaz_dc_bf_h5;
    uint8_t sw_dhaz_dc_bf_h4;

    uint16_t air_weitcur;
    uint16_t air_thed;

    uint8_t air_bf_h2;
    uint8_t air_bf_h1;
    uint8_t air_bf_h0;

    uint8_t gaus_h2;
    uint8_t gaus_h1;
    uint8_t gaus_h0;

    uint8_t conv_t0[ISP2X_DHAZ_CONV_COEFF_NUM];
    uint8_t conv_t1[ISP2X_DHAZ_CONV_COEFF_NUM];
    uint8_t conv_t2[ISP2X_DHAZ_CONV_COEFF_NUM];
} __attribute__((packed));

struct isp2x_dhaz_stat
{
    uint16_t dhaz_adp_air_base;
    uint16_t dhaz_adp_wt;

    uint16_t dhaz_adp_gratio;
    uint16_t dhaz_adp_tmax;

    uint16_t h_r_iir[ISP2X_DHAZ_HIST_IIR_NUM];
    uint16_t h_g_iir[ISP2X_DHAZ_HIST_IIR_NUM];
    uint16_t h_b_iir[ISP2X_DHAZ_HIST_IIR_NUM];
} __attribute__((packed));

struct isp2x_cproc_cfg
{
    uint8_t c_out_range;
    uint8_t y_in_range;
    uint8_t y_out_range;
    uint8_t contrast;
    uint8_t brightness;
    uint8_t sat;
    uint8_t hue;
} __attribute__((packed));

struct isp2x_ie_cfg
{
    uint16_t effect;
    uint16_t color_sel;
    uint16_t eff_mat_1;
    uint16_t eff_mat_2;
    uint16_t eff_mat_3;
    uint16_t eff_mat_4;
    uint16_t eff_mat_5;
    uint16_t eff_tint;
} __attribute__((packed));

struct isp2x_rkiesharp_cfg
{
    uint8_t coring_thr;
    uint8_t full_range;
    uint8_t switch_avg;
    uint8_t yavg_thr[4];
    uint8_t delta1[5];
    uint8_t delta2[5];
    uint8_t maxnumber[5];
    uint8_t minnumber[5];
    uint8_t gauss_flat_coe[9];
    uint8_t gauss_noise_coe[9];
    uint8_t gauss_other_coe[9];
    uint8_t line1_filter_coe[6];
    uint8_t line2_filter_coe[9];
    uint8_t line3_filter_coe[6];
    uint16_t grad_seq[4];
    uint8_t sharp_factor[5];
    uint8_t uv_gauss_flat_coe[15];
    uint8_t uv_gauss_noise_coe[15];
    uint8_t uv_gauss_other_coe[15];
    uint8_t lap_mat_coe[9];
} __attribute__((packed));

struct isp2x_superimp_cfg
{
    uint8_t transparency_mode;
    uint8_t ref_image;

    uint16_t offset_x;
    uint16_t offset_y;

    uint8_t y_comp;
    uint8_t cb_comp;
    uint8_t cr_comp;
} __attribute__((packed));

struct isp2x_gamma_corr_curve
{
    uint16_t gamma_y[ISP2X_DEGAMMA_CURVE_SIZE];
} __attribute__((packed));

struct isp2x_gamma_curve_x_axis_pnts
{
    uint32_t gamma_dx0;
    uint32_t gamma_dx1;
} __attribute__((packed));

struct isp2x_sdg_cfg
{
    struct isp2x_gamma_corr_curve curve_r;
    struct isp2x_gamma_corr_curve curve_g;
    struct isp2x_gamma_corr_curve curve_b;
    struct isp2x_gamma_curve_x_axis_pnts xa_pnts;
} __attribute__((packed));

struct isp2x_bdm_config
{
    unsigned char demosaic_th;
} __attribute__((packed));

struct isp2x_gain_cfg
{
    uint8_t dhaz_en;
    uint8_t wdr_en;
    uint8_t tmo_en;
    uint8_t lsc_en;
    uint8_t mge_en;

    uint32_t mge_gain[ISP2X_GAIN_HDRMGE_GAIN_NUM];
    uint16_t idx[ISP2X_GAIN_IDX_NUM];
    uint16_t lut[ISP2X_GAIN_LUT_NUM];
} __attribute__((packed));

struct isp2x_3dlut_cfg
{
    uint8_t bypass_en;
    uint32_t actual_size;   // word unit
    uint16_t lut_r[ISP2X_3DLUT_DATA_NUM];
    uint16_t lut_g[ISP2X_3DLUT_DATA_NUM];
    uint16_t lut_b[ISP2X_3DLUT_DATA_NUM];
} __attribute__((packed));

enum isp2x_ldch_buf_stat
{
    LDCH_BUF_INIT = 0,
    LDCH_BUF_WAIT2CHIP,
    LDCH_BUF_CHIPINUSE,
};

struct rkisp_ldchbuf_info
{
    int32_t buf_fd[ISP2X_LDCH_BUF_NUM];
    uint32_t buf_size[ISP2X_LDCH_BUF_NUM];
} __attribute__((packed));

struct rkisp_ldchbuf_size
{
    uint32_t meas_width;
    uint32_t meas_height;
} __attribute__((packed));

struct isp2x_ldch_head
{
    enum isp2x_ldch_buf_stat stat;
    uint32_t data_oft;
} __attribute__((packed));

struct isp2x_ldch_cfg
{
    uint32_t hsize;
    uint32_t vsize;
    int32_t buf_fd;
} __attribute__((packed));

struct isp2x_awb_gain_cfg
{
    uint16_t gain_red;
    uint16_t gain_green_r;
    uint16_t gain_blue;
    uint16_t gain_green_b;
} __attribute__((packed));

struct isp2x_siawb_meas_cfg
{
    struct isp2x_window awb_wnd;
    uint8_t awb_mode;
    uint8_t max_y;
    uint8_t min_y;
    uint8_t max_csum;
    uint8_t min_c;
    uint8_t frames;
    uint8_t awb_ref_cr;
    uint8_t awb_ref_cb;
    uint8_t enable_ymax_cmp;
} __attribute__((packed));

struct isp2x_rawawb_meas_cfg
{
    uint8_t rawawb_sel;
    uint8_t sw_rawawb_light_num;            //CTRL
    uint8_t sw_rawawb_wind_size;            //CTRL
    uint8_t sw_rawawb_c_range;          //CTRL
    uint8_t sw_rawawb_y_range;          //CTRL
    uint8_t sw_rawawb_3dyuv_ls_idx3;        //CTRL
    uint8_t sw_rawawb_3dyuv_ls_idx2;        //CTRL
    uint8_t sw_rawawb_3dyuv_ls_idx1;        //CTRL
    uint8_t sw_rawawb_3dyuv_ls_idx0;        //CTRL
    uint8_t sw_rawawb_xy_en;            //CTRL
    uint8_t sw_rawawb_uv_en;            //CTRL
    uint8_t sw_rawlsc_bypass_en;            //CTRL
    uint8_t sw_rawawb_blk_measure_mode;     //BLK_CTRL
    uint8_t sw_rawawb_store_wp_flag_ls_idx2;    //BLK_CTRL
    uint8_t sw_rawawb_store_wp_flag_ls_idx1;    //BLK_CTRL
    uint8_t sw_rawawb_store_wp_flag_ls_idx0;    //BLK_CTRL
    uint16_t sw_rawawb_store_wp_th0;        //BLK_CTRL
    uint16_t sw_rawawb_store_wp_th1;        //BLK_CTRL
    uint16_t sw_rawawb_store_wp_th2;        //RAW_CTRL
    uint16_t sw_rawawb_v_offs;          //WIN_OFFS
    uint16_t sw_rawawb_h_offs;          //WIN_OFFS
    uint16_t sw_rawawb_v_size;          //WIN_SIZE
    uint16_t sw_rawawb_h_size;          //WIN_SIZE
    uint16_t sw_rawawb_g_max;           //LIMIT_RG_MAX
    uint16_t sw_rawawb_r_max;           //LIMIT_RG_MAX
    uint16_t sw_rawawb_y_max;           //LIMIT_BY_MAX
    uint16_t sw_rawawb_b_max;           //LIMIT_BY_MAX
    uint16_t sw_rawawb_g_min;           //LIMIT_RG_MIN
    uint16_t sw_rawawb_r_min;           //LIMIT_RG_MIN
    uint16_t sw_rawawb_y_min;           //LIMIT_BY_MIN
    uint16_t sw_rawawb_b_min;           //LIMIT_BY_MIN
    uint16_t sw_rawawb_coeff_y_g;       //RGB2Y_0
    uint16_t sw_rawawb_coeff_y_r;       //RGB2Y_0
    uint16_t sw_rawawb_coeff_y_b;       //RGB2Y_1
    uint16_t sw_rawawb_coeff_u_g;       //RGB2U_0
    uint16_t sw_rawawb_coeff_u_r;       //RGB2U_0
    uint16_t sw_rawawb_coeff_u_b;       //RGB2U_1
    uint16_t sw_rawawb_coeff_v_g;       //RGB2V_0
    uint16_t sw_rawawb_coeff_v_r;       //RGB2V_0
    uint16_t sw_rawawb_coeff_v_b;       //RGB2V_1
    uint16_t sw_rawawb_vertex0_v_0;     //UV_DETC_VERTEX0_0
    uint16_t sw_rawawb_vertex0_u_0;     //UV_DETC_VERTEX0_0
    uint16_t sw_rawawb_vertex1_v_0;     //UV_DETC_VERTEX1_0
    uint16_t sw_rawawb_vertex1_u_0;     //UV_DETC_VERTEX1_0
    uint16_t sw_rawawb_vertex2_v_0;     //UV_DETC_VERTEX2_0
    uint16_t sw_rawawb_vertex2_u_0;     //UV_DETC_VERTEX2_0
    uint16_t sw_rawawb_vertex3_v_0;     //UV_DETC_VERTEX3_0
    uint16_t sw_rawawb_vertex3_u_0;     //UV_DETC_VERTEX3_0
    uint32_t sw_rawawb_islope01_0;      //UV_DETC_ISLOPE01_0
    uint32_t sw_rawawb_islope12_0;      //UV_DETC_ISLOPE12_0
    uint32_t sw_rawawb_islope23_0;      //UV_DETC_ISLOPE23_0
    uint32_t sw_rawawb_islope30_0;      //UV_DETC_ISLOPE30_0
    uint16_t sw_rawawb_vertex0_v_1;     //UV_DETC_VERTEX0_1
    uint16_t sw_rawawb_vertex0_u_1;     //UV_DETC_VERTEX0_1
    uint16_t sw_rawawb_vertex1_v_1;     //UV_DETC_VERTEX1_1
    uint16_t sw_rawawb_vertex1_u_1;     //UV_DETC_VERTEX1_1
    uint16_t sw_rawawb_vertex2_v_1;     //UV_DETC_VERTEX2_1
    uint16_t sw_rawawb_vertex2_u_1;     //UV_DETC_VERTEX2_1
    uint16_t sw_rawawb_vertex3_v_1;     //UV_DETC_VERTEX3_1
    uint16_t sw_rawawb_vertex3_u_1;     //UV_DETC_VERTEX3_1
    uint32_t sw_rawawb_islope01_1;      //UV_DETC_ISLOPE01_1
    uint32_t sw_rawawb_islope12_1;      //UV_DETC_ISLOPE12_1
    uint32_t sw_rawawb_islope23_1;      //UV_DETC_ISLOPE23_1
    uint32_t sw_rawawb_islope30_1;      //UV_DETC_ISLOPE30_1
    uint16_t sw_rawawb_vertex0_v_2;     //UV_DETC_VERTEX0_2
    uint16_t sw_rawawb_vertex0_u_2;     //UV_DETC_VERTEX0_2
    uint16_t sw_rawawb_vertex1_v_2;     //UV_DETC_VERTEX1_2
    uint16_t sw_rawawb_vertex1_u_2;     //UV_DETC_VERTEX1_2
    uint16_t sw_rawawb_vertex2_v_2;     //UV_DETC_VERTEX2_2
    uint16_t sw_rawawb_vertex2_u_2;     //UV_DETC_VERTEX2_2
    uint16_t sw_rawawb_vertex3_v_2;     //UV_DETC_VERTEX3_2
    uint16_t sw_rawawb_vertex3_u_2;     //UV_DETC_VERTEX3_2
    uint32_t sw_rawawb_islope01_2;      //UV_DETC_ISLOPE01_2
    uint32_t sw_rawawb_islope12_2;      //UV_DETC_ISLOPE12_2
    uint32_t sw_rawawb_islope23_2;      //UV_DETC_ISLOPE23_2
    uint32_t sw_rawawb_islope30_2;      //UV_DETC_ISLOPE30_2
    uint16_t sw_rawawb_vertex0_v_3;     //UV_DETC_VERTEX0_3
    uint16_t sw_rawawb_vertex0_u_3;     //UV_DETC_VERTEX0_3
    uint16_t sw_rawawb_vertex1_v_3;     //UV_DETC_VERTEX1_3
    uint16_t sw_rawawb_vertex1_u_3;     //UV_DETC_VERTEX1_3
    uint16_t sw_rawawb_vertex2_v_3;     //UV_DETC_VERTEX2_3
    uint16_t sw_rawawb_vertex2_u_3;     //UV_DETC_VERTEX2_3
    uint16_t sw_rawawb_vertex3_v_3;     //UV_DETC_VERTEX3_3
    uint16_t sw_rawawb_vertex3_u_3;     //UV_DETC_VERTEX3_3
    uint32_t sw_rawawb_islope01_3;      //UV_DETC_ISLOPE01_3
    uint32_t sw_rawawb_islope12_3;      //UV_DETC_ISLOPE12_3
    uint32_t sw_rawawb_islope23_3;      //UV_DETC_ISLOPE23_3
    uint32_t sw_rawawb_islope30_3;      //UV_DETC_ISLOPE30_3
    uint16_t sw_rawawb_vertex0_v_4;     //UV_DETC_VERTEX0_4
    uint16_t sw_rawawb_vertex0_u_4;     //UV_DETC_VERTEX0_4
    uint16_t sw_rawawb_vertex1_v_4;     //UV_DETC_VERTEX1_4
    uint16_t sw_rawawb_vertex1_u_4;     //UV_DETC_VERTEX1_4
    uint16_t sw_rawawb_vertex2_v_4;     //UV_DETC_VERTEX2_4
    uint16_t sw_rawawb_vertex2_u_4;     //UV_DETC_VERTEX2_4
    uint16_t sw_rawawb_vertex3_v_4;     //UV_DETC_VERTEX3_4
    uint16_t sw_rawawb_vertex3_u_4;     //UV_DETC_VERTEX3_4
    uint32_t sw_rawawb_islope01_4;      //UV_DETC_ISLOPE01_4
    uint32_t sw_rawawb_islope12_4;      //UV_DETC_ISLOPE12_4
    uint32_t sw_rawawb_islope23_4;      //UV_DETC_ISLOPE23_4
    uint32_t sw_rawawb_islope30_4;      //UV_DETC_ISLOPE30_4
    uint16_t sw_rawawb_vertex0_v_5;     //UV_DETC_VERTEX0_5
    uint16_t sw_rawawb_vertex0_u_5;     //UV_DETC_VERTEX0_5
    uint16_t sw_rawawb_vertex1_v_5;     //UV_DETC_VERTEX1_5
    uint16_t sw_rawawb_vertex1_u_5;     //UV_DETC_VERTEX1_5
    uint16_t sw_rawawb_vertex2_v_5;     //UV_DETC_VERTEX2_5
    uint16_t sw_rawawb_vertex2_u_5;     //UV_DETC_VERTEX2_5
    uint16_t sw_rawawb_vertex3_v_5;     //UV_DETC_VERTEX3_5
    uint16_t sw_rawawb_vertex3_u_5;     //UV_DETC_VERTEX3_5
    uint32_t sw_rawawb_islope01_5;      //UV_DETC_ISLOPE01_5
    uint32_t sw_rawawb_islope12_5;      //UV_DETC_ISLOPE10_5
    uint32_t sw_rawawb_islope23_5;      //UV_DETC_ISLOPE23_5
    uint32_t sw_rawawb_islope30_5;      //UV_DETC_ISLOPE30_5
    uint16_t sw_rawawb_vertex0_v_6;     //UV_DETC_VERTEX0_6
    uint16_t sw_rawawb_vertex0_u_6;     //UV_DETC_VERTEX0_6
    uint16_t sw_rawawb_vertex1_v_6;     //UV_DETC_VERTEX1_6
    uint16_t sw_rawawb_vertex1_u_6;     //UV_DETC_VERTEX1_6
    uint16_t sw_rawawb_vertex2_v_6;     //UV_DETC_VERTEX2_6
    uint16_t sw_rawawb_vertex2_u_6;     //UV_DETC_VERTEX2_6
    uint16_t sw_rawawb_vertex3_v_6;     //UV_DETC_VERTEX3_6
    uint16_t sw_rawawb_vertex3_u_6;     //UV_DETC_VERTEX3_6
    uint32_t sw_rawawb_islope01_6;      //UV_DETC_ISLOPE01_6
    uint32_t sw_rawawb_islope12_6;      //UV_DETC_ISLOPE10_6
    uint32_t sw_rawawb_islope23_6;      //UV_DETC_ISLOPE23_6
    uint32_t sw_rawawb_islope30_6;      //UV_DETC_ISLOPE30_6
    uint32_t sw_rawawb_b_uv_0;          //YUV_DETC_B_UV_0
    uint32_t sw_rawawb_slope_vtcuv_0;       //YUV_DETC_SLOPE_VTCUV_0
    uint32_t sw_rawawb_inv_dslope_0;        //YUV_DETC_INV_DSLOPE_0
    uint32_t sw_rawawb_slope_ydis_0;        //YUV_DETC_SLOPE_YDIS_0
    uint32_t sw_rawawb_b_ydis_0;            //YUV_DETC_B_YDIS_0
    uint32_t sw_rawawb_b_uv_1;          //YUV_DETC_B_UV_1
    uint32_t sw_rawawb_slope_vtcuv_1;       //YUV_DETC_SLOPE_VTCUV_1
    uint32_t sw_rawawb_inv_dslope_1;        //YUV_DETC_INV_DSLOPE_1
    uint32_t sw_rawawb_slope_ydis_1;        //YUV_DETC_SLOPE_YDIS_1
    uint32_t sw_rawawb_b_ydis_1;            //YUV_DETC_B_YDIS_1
    uint32_t sw_rawawb_b_uv_2;          //YUV_DETC_B_UV_2
    uint32_t sw_rawawb_slope_vtcuv_2;       //YUV_DETC_SLOPE_VTCUV_2
    uint32_t sw_rawawb_inv_dslope_2;        //YUV_DETC_INV_DSLOPE_2
    uint32_t sw_rawawb_slope_ydis_2;        //YUV_DETC_SLOPE_YDIS_2
    uint32_t sw_rawawb_b_ydis_2;            //YUV_DETC_B_YDIS_2
    uint32_t sw_rawawb_b_uv_3;          //YUV_DETC_B_UV_3
    uint32_t sw_rawawb_slope_vtcuv_3;       //YUV_DETC_SLOPE_VTCUV_3
    uint32_t sw_rawawb_inv_dslope_3;        //YUV_DETC_INV_DSLOPE_3
    uint32_t sw_rawawb_slope_ydis_3;        //YUV_DETC_SLOPE_YDIS_3
    uint32_t sw_rawawb_b_ydis_3;            //YUV_DETC_B_YDIS_3
    uint32_t sw_rawawb_ref_u;           //YUV_DETC_REF_U
    uint8_t sw_rawawb_ref_v_3;          //YUV_DETC_REF_V_1
    uint8_t sw_rawawb_ref_v_2;          //YUV_DETC_REF_V_1
    uint8_t sw_rawawb_ref_v_1;          //YUV_DETC_REF_V_1
    uint8_t sw_rawawb_ref_v_0;          //YUV_DETC_REF_V_1
    uint16_t sw_rawawb_dis1_0;          //YUV_DETC_DIS01_0
    uint16_t sw_rawawb_dis0_0;          //YUV_DETC_DIS01_0
    uint16_t sw_rawawb_dis3_0;          //YUV_DETC_DIS23_0
    uint16_t sw_rawawb_dis2_0;          //YUV_DETC_DIS23_0
    uint16_t sw_rawawb_dis5_0;          //YUV_DETC_DIS45_0
    uint16_t sw_rawawb_dis4_0;          //YUV_DETC_DIS45_0
    uint8_t sw_rawawb_th3_0;            //YUV_DETC_TH03_0
    uint8_t sw_rawawb_th2_0;            //YUV_DETC_TH03_0
    uint8_t sw_rawawb_th1_0;            //YUV_DETC_TH03_0
    uint8_t sw_rawawb_th0_0;            //YUV_DETC_TH03_0
    uint8_t sw_rawawb_th5_0;            //YUV_DETC_TH45_0
    uint8_t sw_rawawb_th4_0;            //YUV_DETC_TH45_0
    uint16_t sw_rawawb_dis1_1;          //YUV_DETC_DIS01_1
    uint16_t sw_rawawb_dis0_1;          //YUV_DETC_DIS01_1
    uint16_t sw_rawawb_dis3_1;          //YUV_DETC_DIS23_1
    uint16_t sw_rawawb_dis2_1;          //YUV_DETC_DIS23_1
    uint16_t sw_rawawb_dis5_1;          //YUV_DETC_DIS45_1
    uint16_t sw_rawawb_dis4_1;          //YUV_DETC_DIS45_1
    uint8_t sw_rawawb_th3_1;            //YUV_DETC_TH03_1
    uint8_t sw_rawawb_th2_1;            //YUV_DETC_TH03_1
    uint8_t sw_rawawb_th1_1;            //YUV_DETC_TH03_1
    uint8_t sw_rawawb_th0_1;            //YUV_DETC_TH03_1
    uint8_t sw_rawawb_th5_1;            //YUV_DETC_TH45_1
    uint8_t sw_rawawb_th4_1;            //YUV_DETC_TH45_1
    uint16_t sw_rawawb_dis1_2;          //YUV_DETC_DIS01_2
    uint16_t sw_rawawb_dis0_2;          //YUV_DETC_DIS01_2
    uint16_t sw_rawawb_dis3_2;          //YUV_DETC_DIS23_2
    uint16_t sw_rawawb_dis2_2;          //YUV_DETC_DIS23_2
    uint16_t sw_rawawb_dis5_2;          //YUV_DETC_DIS45_2
    uint16_t sw_rawawb_dis4_2;          //YUV_DETC_DIS45_2
    uint8_t sw_rawawb_th3_2;            //YUV_DETC_TH03_2
    uint8_t sw_rawawb_th2_2;            //YUV_DETC_TH03_2
    uint8_t sw_rawawb_th1_2;            //YUV_DETC_TH03_2
    uint8_t sw_rawawb_th0_2;            //YUV_DETC_TH03_2
    uint8_t sw_rawawb_th5_2;            //YUV_DETC_TH45_2
    uint8_t sw_rawawb_th4_2;            //YUV_DETC_TH45_2
    uint16_t sw_rawawb_dis1_3;          //YUV_DETC_DIS01_3
    uint16_t sw_rawawb_dis0_3;          //YUV_DETC_DIS01_3
    uint16_t sw_rawawb_dis3_3;          //YUV_DETC_DIS23_3
    uint16_t sw_rawawb_dis2_3;          //YUV_DETC_DIS23_3
    uint16_t sw_rawawb_dis5_3;          //YUV_DETC_DIS45_3
    uint16_t sw_rawawb_dis4_3;          //YUV_DETC_DIS45_3
    uint8_t sw_rawawb_th3_3;            //YUV_DETC_TH03_3
    uint8_t sw_rawawb_th2_3;            //YUV_DETC_TH03_3
    uint8_t sw_rawawb_th1_3;            //YUV_DETC_TH03_3
    uint8_t sw_rawawb_th0_3;            //YUV_DETC_TH03_3
    uint8_t sw_rawawb_th5_3;            //YUV_DETC_TH45_3
    uint8_t sw_rawawb_th4_3;            //YUV_DETC_TH45_3
    uint16_t sw_rawawb_wt1;         //RGB2XY_WT01
    uint16_t sw_rawawb_wt0;         //RGB2XY_WT01
    uint16_t sw_rawawb_wt2;         //RGB2XY_WT2
    uint16_t sw_rawawb_mat0_y;          //RGB2XY_MAT0_XY
    uint16_t sw_rawawb_mat0_x;          //RGB2XY_MAT0_XY
    uint16_t sw_rawawb_mat1_y;          //RGB2XY_MAT1_XY
    uint16_t sw_rawawb_mat1_x;          //RGB2XY_MAT1_XY
    uint16_t sw_rawawb_mat2_y;          //RGB2XY_MAT2_XY
    uint16_t sw_rawawb_mat2_x;          //RGB2XY_MAT2_XY
    uint16_t sw_rawawb_nor_x1_0;            //XY_DETC_NOR_X_0
    uint16_t sw_rawawb_nor_x0_0;            //XY_DETC_NOR_X_0
    uint16_t sw_rawawb_nor_y1_0;            //XY_DETC_NOR_Y_0
    uint16_t sw_rawawb_nor_y0_0;            //XY_DETC_NOR_Y_0
    uint16_t sw_rawawb_big_x1_0;            //XY_DETC_BIG_X_0
    uint16_t sw_rawawb_big_x0_0;            //XY_DETC_BIG_X_0
    uint16_t sw_rawawb_big_y1_0;            //XY_DETC_BIG_Y_0
    uint16_t sw_rawawb_big_y0_0;            //XY_DETC_BIG_Y_0
    uint16_t sw_rawawb_sma_x1_0;            //XY_DETC_SMA_X_0
    uint16_t sw_rawawb_sma_x0_0;            //XY_DETC_SMA_X_0
    uint16_t sw_rawawb_sma_y1_0;            //XY_DETC_SMA_Y_0
    uint16_t sw_rawawb_sma_y0_0;            //XY_DETC_SMA_Y_0
    uint16_t sw_rawawb_nor_x1_1;            //XY_DETC_NOR_X_1
    uint16_t sw_rawawb_nor_x0_1;            //XY_DETC_NOR_X_1
    uint16_t sw_rawawb_nor_y1_1;            //XY_DETC_NOR_Y_1
    uint16_t sw_rawawb_nor_y0_1;            //XY_DETC_NOR_Y_1
    uint16_t sw_rawawb_big_x1_1;            //XY_DETC_BIG_X_1
    uint16_t sw_rawawb_big_x0_1;            //XY_DETC_BIG_X_1
    uint16_t sw_rawawb_big_y1_1;            //XY_DETC_BIG_Y_1
    uint16_t sw_rawawb_big_y0_1;            //XY_DETC_BIG_Y_1
    uint16_t sw_rawawb_sma_x1_1;            //XY_DETC_SMA_X_1
    uint16_t sw_rawawb_sma_x0_1;            //XY_DETC_SMA_X_1
    uint16_t sw_rawawb_sma_y1_1;            //XY_DETC_SMA_Y_1
    uint16_t sw_rawawb_sma_y0_1;            //XY_DETC_SMA_Y_1
    uint16_t sw_rawawb_nor_x1_2;            //XY_DETC_NOR_X_2
    uint16_t sw_rawawb_nor_x0_2;            //XY_DETC_NOR_X_2
    uint16_t sw_rawawb_nor_y1_2;            //XY_DETC_NOR_Y_2
    uint16_t sw_rawawb_nor_y0_2;            //XY_DETC_NOR_Y_2
    uint16_t sw_rawawb_big_x1_2;            //XY_DETC_BIG_X_2
    uint16_t sw_rawawb_big_x0_2;            //XY_DETC_BIG_X_2
    uint16_t sw_rawawb_big_y1_2;            //XY_DETC_BIG_Y_2
    uint16_t sw_rawawb_big_y0_2;            //XY_DETC_BIG_Y_2
    uint16_t sw_rawawb_sma_x1_2;            //XY_DETC_SMA_X_2
    uint16_t sw_rawawb_sma_x0_2;            //XY_DETC_SMA_X_2
    uint16_t sw_rawawb_sma_y1_2;            //XY_DETC_SMA_Y_2
    uint16_t sw_rawawb_sma_y0_2;            //XY_DETC_SMA_Y_2
    uint16_t sw_rawawb_nor_x1_3;            //XY_DETC_NOR_X_3
    uint16_t sw_rawawb_nor_x0_3;            //XY_DETC_NOR_X_3
    uint16_t sw_rawawb_nor_y1_3;            //XY_DETC_NOR_Y_3
    uint16_t sw_rawawb_nor_y0_3;            //XY_DETC_NOR_Y_3
    uint16_t sw_rawawb_big_x1_3;            //XY_DETC_BIG_X_3
    uint16_t sw_rawawb_big_x0_3;            //XY_DETC_BIG_X_3
    uint16_t sw_rawawb_big_y1_3;            //XY_DETC_BIG_Y_3
    uint16_t sw_rawawb_big_y0_3;            //XY_DETC_BIG_Y_3
    uint16_t sw_rawawb_sma_x1_3;            //XY_DETC_SMA_X_3
    uint16_t sw_rawawb_sma_x0_3;            //XY_DETC_SMA_X_3
    uint16_t sw_rawawb_sma_y1_3;            //XY_DETC_SMA_Y_3
    uint16_t sw_rawawb_sma_y0_3;            //XY_DETC_SMA_Y_3
    uint16_t sw_rawawb_nor_x1_4;            //XY_DETC_NOR_X_4
    uint16_t sw_rawawb_nor_x0_4;            //XY_DETC_NOR_X_4
    uint16_t sw_rawawb_nor_y1_4;            //XY_DETC_NOR_Y_4
    uint16_t sw_rawawb_nor_y0_4;            //XY_DETC_NOR_Y_4
    uint16_t sw_rawawb_big_x1_4;            //XY_DETC_BIG_X_4
    uint16_t sw_rawawb_big_x0_4;            //XY_DETC_BIG_X_4
    uint16_t sw_rawawb_big_y1_4;            //XY_DETC_BIG_Y_4
    uint16_t sw_rawawb_big_y0_4;            //XY_DETC_BIG_Y_4
    uint16_t sw_rawawb_sma_x1_4;            //XY_DETC_SMA_X_4
    uint16_t sw_rawawb_sma_x0_4;            //XY_DETC_SMA_X_4
    uint16_t sw_rawawb_sma_y1_4;            //XY_DETC_SMA_Y_4
    uint16_t sw_rawawb_sma_y0_4;            //XY_DETC_SMA_Y_4
    uint16_t sw_rawawb_nor_x1_5;            //XY_DETC_NOR_X_5
    uint16_t sw_rawawb_nor_x0_5;            //XY_DETC_NOR_X_5
    uint16_t sw_rawawb_nor_y1_5;            //XY_DETC_NOR_Y_5
    uint16_t sw_rawawb_nor_y0_5;            //XY_DETC_NOR_Y_5
    uint16_t sw_rawawb_big_x1_5;            //XY_DETC_BIG_X_5
    uint16_t sw_rawawb_big_x0_5;            //XY_DETC_BIG_X_5
    uint16_t sw_rawawb_big_y1_5;            //XY_DETC_BIG_Y_5
    uint16_t sw_rawawb_big_y0_5;            //XY_DETC_BIG_Y_5
    uint16_t sw_rawawb_sma_x1_5;            //XY_DETC_SMA_X_5
    uint16_t sw_rawawb_sma_x0_5;            //XY_DETC_SMA_X_5
    uint16_t sw_rawawb_sma_y1_5;            //XY_DETC_SMA_Y_5
    uint16_t sw_rawawb_sma_y0_5;            //XY_DETC_SMA_Y_5
    uint16_t sw_rawawb_nor_x1_6;            //XY_DETC_NOR_X_6
    uint16_t sw_rawawb_nor_x0_6;            //XY_DETC_NOR_X_6
    uint16_t sw_rawawb_nor_y1_6;            //XY_DETC_NOR_Y_6
    uint16_t sw_rawawb_nor_y0_6;            //XY_DETC_NOR_Y_6
    uint16_t sw_rawawb_big_x1_6;            //XY_DETC_BIG_X_6
    uint16_t sw_rawawb_big_x0_6;            //XY_DETC_BIG_X_6
    uint16_t sw_rawawb_big_y1_6;            //XY_DETC_BIG_Y_6
    uint16_t sw_rawawb_big_y0_6;            //XY_DETC_BIG_Y_6
    uint16_t sw_rawawb_sma_x1_6;            //XY_DETC_SMA_X_6
    uint16_t sw_rawawb_sma_x0_6;            //XY_DETC_SMA_X_6
    uint16_t sw_rawawb_sma_y1_6;            //XY_DETC_SMA_Y_6
    uint16_t sw_rawawb_sma_y0_6;            //XY_DETC_SMA_Y_6
    uint8_t sw_rawawb_multiwindow_en;       //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region6_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region6_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region6_excen; //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region5_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region5_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region5_excen; //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region4_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region4_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region4_excen; //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region3_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region3_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region3_excen; //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region2_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region2_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region2_excen; //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region1_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region1_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region1_excen; //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region0_domain;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region0_measen;    //MULTIWINDOW_EXC_CTRL
    uint8_t sw_rawawb_exc_wp_region0_excen; //MULTIWINDOW_EXC_CTRL
    uint16_t sw_rawawb_multiwindow0_v_offs; //MULTIWINDOW0_OFFS
    uint16_t sw_rawawb_multiwindow0_h_offs; //MULTIWINDOW0_OFFS
    uint16_t sw_rawawb_multiwindow0_v_size; //MULTIWINDOW0_SIZE
    uint16_t sw_rawawb_multiwindow0_h_size; //MULTIWINDOW0_SIZE
    uint16_t sw_rawawb_multiwindow1_v_offs; //MULTIWINDOW1_OFFS
    uint16_t sw_rawawb_multiwindow1_h_offs; //MULTIWINDOW1_OFFS
    uint16_t sw_rawawb_multiwindow1_v_size; //MULTIWINDOW1_SIZE
    uint16_t sw_rawawb_multiwindow1_h_size; //MULTIWINDOW1_SIZE
    uint16_t sw_rawawb_multiwindow2_v_offs; //MULTIWINDOW2_OFFS
    uint16_t sw_rawawb_multiwindow2_h_offs; //MULTIWINDOW2_OFFS
    uint16_t sw_rawawb_multiwindow2_v_size; //MULTIWINDOW2_SIZE
    uint16_t sw_rawawb_multiwindow2_h_size; //MULTIWINDOW2_SIZE
    uint16_t sw_rawawb_multiwindow3_v_offs; //MULTIWINDOW3_OFFS
    uint16_t sw_rawawb_multiwindow3_h_offs; //MULTIWINDOW3_OFFS
    uint16_t sw_rawawb_multiwindow3_v_size; //MULTIWINDOW3_SIZE
    uint16_t sw_rawawb_multiwindow3_h_size; //MULTIWINDOW3_SIZE
    uint16_t sw_rawawb_multiwindow4_v_offs; //MULTIWINDOW4_OFFS
    uint16_t sw_rawawb_multiwindow4_h_offs; //MULTIWINDOW4_OFFS
    uint16_t sw_rawawb_multiwindow4_v_size; //MULTIWINDOW4_SIZE
    uint16_t sw_rawawb_multiwindow4_h_size; //MULTIWINDOW4_SIZE
    uint16_t sw_rawawb_multiwindow5_v_offs; //MULTIWINDOW5_OFFS
    uint16_t sw_rawawb_multiwindow5_h_offs; //MULTIWINDOW5_OFFS
    uint16_t sw_rawawb_multiwindow5_v_size; //MULTIWINDOW5_SIZE
    uint16_t sw_rawawb_multiwindow5_h_size; //MULTIWINDOW5_SIZE
    uint16_t sw_rawawb_multiwindow6_v_offs; //MULTIWINDOW6_OFFS
    uint16_t sw_rawawb_multiwindow6_h_offs; //MULTIWINDOW6_OFFS
    uint16_t sw_rawawb_multiwindow6_v_size; //MULTIWINDOW6_SIZE
    uint16_t sw_rawawb_multiwindow6_h_size; //MULTIWINDOW6_SIZE
    uint16_t sw_rawawb_multiwindow7_v_offs; //MULTIWINDOW7_OFFS
    uint16_t sw_rawawb_multiwindow7_h_offs; //MULTIWINDOW7_OFFS
    uint16_t sw_rawawb_multiwindow7_v_size; //MULTIWINDOW7_SIZE
    uint16_t sw_rawawb_multiwindow7_h_size; //MULTIWINDOW7_SIZE
    uint16_t sw_rawawb_exc_wp_region0_xu1;  //EXC_WP_REGION0_XU
    uint16_t sw_rawawb_exc_wp_region0_xu0;  //EXC_WP_REGION0_XU
    uint16_t sw_rawawb_exc_wp_region0_yv1;  //EXC_WP_REGION0_YV
    uint16_t sw_rawawb_exc_wp_region0_yv0;  //EXC_WP_REGION0_YV
    uint16_t sw_rawawb_exc_wp_region1_xu1;  //EXC_WP_REGION1_XU
    uint16_t sw_rawawb_exc_wp_region1_xu0;  //EXC_WP_REGION1_XU
    uint16_t sw_rawawb_exc_wp_region1_yv1;  //EXC_WP_REGION1_YV
    uint16_t sw_rawawb_exc_wp_region1_yv0;  //EXC_WP_REGION1_YV
    uint16_t sw_rawawb_exc_wp_region2_xu1;  //EXC_WP_REGION2_XU
    uint16_t sw_rawawb_exc_wp_region2_xu0;  //EXC_WP_REGION2_XU
    uint16_t sw_rawawb_exc_wp_region2_yv1;  //EXC_WP_REGION2_YV
    uint16_t sw_rawawb_exc_wp_region2_yv0;  //EXC_WP_REGION2_YV
    uint16_t sw_rawawb_exc_wp_region3_xu1;  //EXC_WP_REGION3_XU
    uint16_t sw_rawawb_exc_wp_region3_xu0;  //EXC_WP_REGION3_XU
    uint16_t sw_rawawb_exc_wp_region3_yv1;  //EXC_WP_REGION3_YV
    uint16_t sw_rawawb_exc_wp_region3_yv0;  //EXC_WP_REGION3_YV
    uint16_t sw_rawawb_exc_wp_region4_xu1;  //EXC_WP_REGION4_XU
    uint16_t sw_rawawb_exc_wp_region4_xu0;  //EXC_WP_REGION4_XU
    uint16_t sw_rawawb_exc_wp_region4_yv1;  //EXC_WP_REGION4_YV
    uint16_t sw_rawawb_exc_wp_region4_yv0;  //EXC_WP_REGION4_YV
    uint16_t sw_rawawb_exc_wp_region5_xu1;  //EXC_WP_REGION5_XU
    uint16_t sw_rawawb_exc_wp_region5_xu0;  //EXC_WP_REGION5_XU
    uint16_t sw_rawawb_exc_wp_region5_yv1;  //EXC_WP_REGION5_YV
    uint16_t sw_rawawb_exc_wp_region5_yv0;  //EXC_WP_REGION5_YV
    uint16_t sw_rawawb_exc_wp_region6_xu1;  //EXC_WP_REGION6_XU
    uint16_t sw_rawawb_exc_wp_region6_xu0;  //EXC_WP_REGION6_XU
    uint16_t sw_rawawb_exc_wp_region6_yv1;  //EXC_WP_REGION6_YV
    uint16_t sw_rawawb_exc_wp_region6_yv0;  //EXC_WP_REGION6_YV
} __attribute__((packed));

struct isp2x_rawaebig_meas_cfg
{
    uint8_t rawae_sel;
    uint8_t wnd_num;
    uint8_t subwin_en[ISP2X_RAWAEBIG_SUBWIN_NUM];
    struct isp2x_window win;
    struct isp2x_window subwin[ISP2X_RAWAEBIG_SUBWIN_NUM];
} __attribute__((packed));

struct isp2x_rawaelite_meas_cfg
{
    uint8_t rawae_sel;
    uint8_t wnd_num;
    struct isp2x_window win;
} __attribute__((packed));

struct isp2x_yuvae_meas_cfg
{
    uint8_t ysel;
    uint8_t wnd_num;
    uint8_t subwin_en[ISP2X_YUVAE_SUBWIN_NUM];
    struct isp2x_window win;
    struct isp2x_window subwin[ISP2X_YUVAE_SUBWIN_NUM];
} __attribute__((packed));

struct isp2x_rawaf_meas_cfg
{
    uint8_t rawaf_sel;
    uint8_t num_afm_win;
    uint8_t gaus_en;
    uint8_t gamma_en;
    struct isp2x_window win[ISP2X_RAWAF_WIN_NUM];
    uint8_t line_en[ISP2X_RAWAF_LINE_NUM];
    uint8_t line_num[ISP2X_RAWAF_LINE_NUM];
    uint8_t gaus_coe_h2;
    uint8_t gaus_coe_h1;
    uint8_t gaus_coe_h0;
    uint16_t afm_thres;
    uint8_t lum_var_shift[ISP2X_RAWAF_WIN_NUM];
    uint8_t afm_var_shift[ISP2X_RAWAF_WIN_NUM];
    uint16_t gamma_y[ISP2X_RAWAF_GAMMA_NUM];
} __attribute__((packed));

struct isp2x_siaf_win_cfg
{
    uint8_t sum_shift;
    uint8_t lum_shift;
    struct isp2x_window win;
} __attribute__((packed));

struct isp2x_siaf_cfg
{
    uint8_t num_afm_win;
    uint32_t thres;
    struct isp2x_siaf_win_cfg afm_win[ISP2X_AFM_MAX_WINDOWS];
} __attribute__((packed));

struct isp2x_rawhistbig_cfg
{
    uint8_t wnd_num;
    uint8_t data_sel;
    uint8_t waterline;
    uint8_t mode;
    uint8_t stepsize;
    uint8_t off;
    uint8_t bcc;
    uint8_t gcc;
    uint8_t rcc;
    struct isp2x_window win;
    uint8_t weight[ISP2X_RAWHISTBIG_SUBWIN_NUM];
} __attribute__((packed));

struct isp2x_rawhistlite_cfg
{
    uint8_t data_sel;
    uint8_t waterline;
    uint8_t mode;
    uint8_t stepsize;
    uint8_t off;
    uint8_t bcc;
    uint8_t gcc;
    uint8_t rcc;
    struct isp2x_window win;
    uint8_t weight[ISP2X_RAWHISTLITE_SUBWIN_NUM];
} __attribute__((packed));

struct isp2x_sihst_win_cfg
{
    uint8_t data_sel;
    uint8_t waterline;
    uint8_t auto_stop;
    uint8_t mode;
    uint8_t stepsize;
    struct isp2x_window win;
} __attribute__((packed));

struct isp2x_sihst_cfg
{
    uint8_t wnd_num;
    struct isp2x_sihst_win_cfg win_cfg[ISP2X_SIHIST_WIN_NUM];
    uint8_t hist_weight[ISP2X_HIST_WEIGHT_NUM];
} __attribute__((packed));

struct isp2x_isp_other_cfg
{
    struct isp2x_bls_cfg bls_cfg;
    struct isp2x_dpcc_cfg dpcc_cfg;
    struct isp2x_hdrmge_cfg hdrmge_cfg;
    struct isp2x_rawnr_cfg rawnr_cfg;
    struct isp2x_lsc_cfg lsc_cfg;
    struct isp2x_awb_gain_cfg awb_gain_cfg;
    //struct isp2x_goc_cfg goc_cfg;
    struct isp2x_gic_cfg gic_cfg;
    struct isp2x_debayer_cfg debayer_cfg;
    struct isp2x_ccm_cfg ccm_cfg;
    struct isp2x_gammaout_cfg gammaout_cfg;
    struct isp2x_wdr_cfg wdr_cfg;
    struct isp2x_cproc_cfg cproc_cfg;
    struct isp2x_ie_cfg ie_cfg;
    struct isp2x_rkiesharp_cfg rkiesharp_cfg;
    struct isp2x_superimp_cfg superimp_cfg;
    struct isp2x_sdg_cfg sdg_cfg;
    struct isp2x_bdm_config bdm_cfg;
    struct isp2x_hdrtmo_cfg hdrtmo_cfg;
    struct isp2x_dhaz_cfg dhaz_cfg;
    struct isp2x_gain_cfg gain_cfg;
    struct isp2x_3dlut_cfg isp3dlut_cfg;
    struct isp2x_ldch_cfg ldch_cfg;
} __attribute__((packed));

struct isp2x_isp_meas_cfg
{
    struct isp2x_siawb_meas_cfg siawb;
    struct isp2x_rawawb_meas_cfg rawawb;
    struct isp2x_rawaelite_meas_cfg rawae0;
    struct isp2x_rawaebig_meas_cfg rawae1;
    struct isp2x_rawaebig_meas_cfg rawae2;
    struct isp2x_rawaebig_meas_cfg rawae3;
    struct isp2x_yuvae_meas_cfg yuvae;
    struct isp2x_rawaf_meas_cfg rawaf;
    struct isp2x_siaf_cfg siaf;
    struct isp2x_rawhistlite_cfg rawhist0;
    struct isp2x_rawhistbig_cfg rawhist1;
    struct isp2x_rawhistbig_cfg rawhist2;
    struct isp2x_rawhistbig_cfg rawhist3;
    struct isp2x_sihst_cfg sihst;
} __attribute__((packed));

struct sensor_exposure_s
{
    uint32_t fine_integration_time;
    uint32_t coarse_integration_time;
    uint32_t analog_gain_code_global;
    uint32_t digital_gain_global;
    uint32_t isp_digital_gain;
} __attribute__((packed));

struct sensor_exposure_cfg
{
    struct sensor_exposure_s linear_exp;
    struct sensor_exposure_s hdr_exp[3];
} __attribute__((packed));

struct isp2x_isp_params_cfg
{
    uint64_t module_en_update;
    uint64_t module_ens;
    uint64_t module_cfg_update;

    uint32_t frame_id;
    struct isp2x_isp_meas_cfg meas;
    struct isp2x_isp_other_cfg others;
    struct sensor_exposure_cfg exposure;
} __attribute__((packed));

struct isp2x_siawb_meas
{
    uint32_t cnt;
    uint8_t mean_y_or_g;
    uint8_t mean_cb_or_b;
    uint8_t mean_cr_or_r;
} __attribute__((packed));

struct isp2x_siawb_stat
{
    struct isp2x_siawb_meas awb_mean[ISP2X_AWB_MAX_GRID];
} __attribute__((packed));

struct isp2x_rawawb_ramdata
{
    uint32_t wp;
    uint32_t r;
    uint32_t g;
    uint32_t b;
};

struct isp2x_rawawb_meas_stat
{
    uint32_t ro_rawawb_sum_r_nor[ISP2X_RAWAWB_SUM_NUM];     //SUM_R_NOR_0
    uint32_t ro_rawawb_sum_g_nor[ISP2X_RAWAWB_SUM_NUM];     //SUM_G_NOR_0
    uint32_t ro_rawawb_sum_b_nor[ISP2X_RAWAWB_SUM_NUM];     //SUM_B_NOR_0
    uint32_t ro_rawawb_wp_num_nor[ISP2X_RAWAWB_SUM_NUM];        //WP_NUM_NOR_0
    uint32_t ro_rawawb_sum_r_big[ISP2X_RAWAWB_SUM_NUM];     //SUM_R_BIG_0
    uint32_t ro_rawawb_sum_g_big[ISP2X_RAWAWB_SUM_NUM];     //SUM_G_BIG_0
    uint32_t ro_rawawb_sum_b_big[ISP2X_RAWAWB_SUM_NUM];     //SUM_B_BIG_0
    uint32_t ro_rawawb_wp_num_big[ISP2X_RAWAWB_SUM_NUM];        //WP_NUM_BIG_0
    uint32_t ro_rawawb_sum_r_sma[ISP2X_RAWAWB_SUM_NUM];     //SUM_R_SMA_0
    uint32_t ro_rawawb_sum_g_sma[ISP2X_RAWAWB_SUM_NUM];     //SUM_G_SMA_0
    uint32_t ro_rawawb_sum_b_sma[ISP2X_RAWAWB_SUM_NUM];     //SUM_B_SMA_0
    uint32_t ro_rawawb_wp_num_sma[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_sum_r_nor_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_R_NOR_MULTIWINDOW_0
    uint32_t ro_sum_g_nor_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_G_NOR_MULTIWINDOW_0
    uint32_t ro_sum_b_nor_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_B_NOR_MULTIWINDOW_0
    uint32_t ro_wp_nm_nor_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //WP_NM_NOR_MULTIWINDOW_0
    uint32_t ro_sum_r_big_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_R_BIG_MULTIWINDOW_0
    uint32_t ro_sum_g_big_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_G_BIG_MULTIWINDOW_0
    uint32_t ro_sum_b_big_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_B_BIG_MULTIWINDOW_0
    uint32_t ro_wp_nm_big_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //WP_NM_BIG_MULTIWINDOW_0
    uint32_t ro_sum_r_sma_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_R_SMA_MULTIWINDOW_0
    uint32_t ro_sum_g_sma_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_G_SMA_MULTIWINDOW_0
    uint32_t ro_sum_b_sma_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //SUM_B_SMA_MULTIWINDOW_0
    uint32_t ro_wp_nm_sma_multiwindow[ISP2X_RAWAWB_MULWD_NUM];  //WP_NM_SMA_MULTIWINDOW_0
    uint32_t ro_sum_r_exc[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_sum_g_exc[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_sum_b_exc[ISP2X_RAWAWB_SUM_NUM];
    uint32_t ro_wp_nm_exc[ISP2X_RAWAWB_SUM_NUM];
    struct isp2x_rawawb_ramdata ramdata[ISP2X_RAWAWB_RAMDATA_NUM];
} __attribute__((packed));

struct isp2x_rawae_meas_data
{
    uint16_t channelr_xy;
    uint16_t channelb_xy;
    uint16_t channelg_xy;
};

struct isp2x_rawaebig_stat
{
    uint32_t sumr[ISP2X_RAWAEBIG_SUBWIN_NUM];
    uint32_t sumg[ISP2X_RAWAEBIG_SUBWIN_NUM];
    uint32_t sumb[ISP2X_RAWAEBIG_SUBWIN_NUM];
    struct isp2x_rawae_meas_data data[ISP2X_RAWAEBIG_MEAN_NUM];
} __attribute__((packed));

struct isp2x_rawaelite_stat
{
    struct isp2x_rawae_meas_data data[ISP2X_RAWAELITE_MEAN_NUM];
} __attribute__((packed));

struct isp2x_yuvae_stat
{
    uint32_t ro_yuvae_sumy[ISP2X_YUVAE_SUBWIN_NUM];
    uint8_t mean[ISP2X_YUVAE_MEAN_NUM];
} __attribute__((packed));

struct isp2x_rawaf_stat
{
    uint32_t int_state;
    uint32_t afm_sum[ISP2X_RAWAF_WIN_NUM];
    uint32_t afm_lum[ISP2X_RAWAF_WIN_NUM];
    uint32_t ramdata[ISP2X_RAWAF_SUMDATA_NUM];
} __attribute__((packed));

struct isp2x_siaf_meas_val
{
    uint32_t sum;
    uint32_t lum;
} __attribute__((packed));

struct isp2x_siaf_stat
{
    struct isp2x_siaf_meas_val win[ISP2X_AFM_MAX_WINDOWS];
} __attribute__((packed));

struct isp2x_rawhistbig_stat
{
    uint32_t hist_bin[ISP2X_HIST_BIN_N_MAX];
} __attribute__((packed));

struct isp2x_rawhistlite_stat
{
    uint32_t hist_bin[ISP2X_HIST_BIN_N_MAX];
} __attribute__((packed));

struct isp2x_sihst_win_stat
{
    uint32_t hist_bins[ISP2X_SIHIST_BIN_N_MAX];
} __attribute__((packed));

struct isp2x_sihst_stat
{
    struct isp2x_sihst_win_stat win_stat[ISP2X_SIHIST_WIN_NUM];
} __attribute__((packed));

struct isp2x_stat
{
    struct isp2x_siawb_stat siawb;
    struct isp2x_rawawb_meas_stat rawawb;
    struct isp2x_rawaelite_stat rawae0;
    struct isp2x_rawaebig_stat rawae1;
    struct isp2x_rawaebig_stat rawae2;
    struct isp2x_rawaebig_stat rawae3;
    struct isp2x_yuvae_stat yuvae;
    struct isp2x_rawaf_stat rawaf;
    struct isp2x_siaf_stat siaf;
    struct isp2x_rawhistlite_stat rawhist0;
    struct isp2x_rawhistbig_stat rawhist1;
    struct isp2x_rawhistbig_stat rawhist2;
    struct isp2x_rawhistbig_stat rawhist3;
    struct isp2x_sihst_stat sihst;

    struct isp2x_bls_stat bls;
    struct isp2x_hdrtmo_stat hdrtmo;
    struct isp2x_dhaz_stat dhaz;
} __attribute__((packed));

/**
 * struct rkisp_isp2x_stat_buffer - Rockchip ISP2 Statistics Meta Data
 *
 * @meas_type: measurement types (CIFISP_STAT_ definitions)
 * @frame_id: frame ID for sync
 * @params: statistics data
 */
struct rkisp_isp2x_stat_buffer
{
    unsigned int meas_type;
    unsigned int frame_id;
    struct isp2x_stat params;
} __attribute__((packed));

/**
 * struct rkisp_mipi_luma - statistics mipi y statistic
 *
 * @exp_mean: Mean luminance value of block xx
 *
 * Image is divided into 5x5 blocks.
 */
struct rkisp_mipi_luma
{
    unsigned int exp_mean[ISP2X_MIPI_LUMA_MEAN_MAX];
} __attribute__((packed));

/**
 * struct rkisp_isp2x_luma_buffer - Rockchip ISP1 Statistics Mipi Luma
 *
 * @meas_type: measurement types (CIFISP_STAT_ definitions)
 * @frame_id: frame ID for sync
 * @params: statistics data
 */
struct rkisp_isp2x_luma_buffer
{
    unsigned int meas_type;
    unsigned int frame_id;
    struct rkisp_mipi_luma luma[ISP2X_MIPI_RAW_MAX];
} __attribute__((packed));

/**
 * struct rkisp_thunderboot_video_buf
 */
struct rkisp_thunderboot_video_buf
{
    uint32_t index;
    uint32_t frame_id;
    uint32_t timestamp;
    uint32_t time_reg;
    uint32_t gain_reg;
    uint32_t bufaddr;
    uint32_t bufsize;
} __attribute__((packed));

/**
 * struct rkisp_thunderboot_resmem_head
 */
struct rkisp_thunderboot_resmem_head
{
    uint16_t enable;
    uint16_t complete;
    uint16_t frm_total;
    uint16_t hdr_mode;
    uint16_t width;
    uint16_t height;
    uint16_t camera_num;
    uint16_t camera_index;

    uint32_t blc_lvl;
    uint32_t exp_time[3];
    uint32_t exp_gain[3];
    uint32_t exp_time_reg[3];
    uint32_t exp_gain_reg[3];
    uint16_t awb1_gain_gb;
    uint16_t awb1_gain_gr;
    uint16_t awb1_gain_b;
    uint16_t awb1_gain_r;
} __attribute__((packed));

/**
 * struct rkisp_thunderboot_resmem - shared buffer for thunderboot with risc-v side
 */
struct rkisp_thunderboot_resmem
{
    uint32_t resmem_padr;
    uint32_t resmem_size;
} __attribute__((packed));

/**
 * struct rkisp_thunderboot_shmem
 */
struct rkisp_thunderboot_shmem
{
    uint32_t shm_start;
    uint32_t shm_size;
    int32_t shm_fd;
} __attribute__((packed));

#endif /* _UAPI_RKISP2_CONFIG_H */

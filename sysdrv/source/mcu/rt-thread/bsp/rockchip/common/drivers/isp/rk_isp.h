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
 * 2020-7-24       ISP_TEAM     first implementation
 *
 */

#ifndef _RK_ISP_H
#define _RK_ISP_H

#include "isp_adptive_camera_config.h"
#include "rk_isp_isp.h"
#include "isp.h"
#include "drv_clock.h"

#if defined(RT_USING_ISP)
/*****************************************************************************/
/* Definitions */
#define ISP_DEBUG 0
#define ISP_LOGERR 1
#if ISP_DEBUG
#include <stdio.h>
#define ISP_DBG(...)                \
do {\
rt_kprintf("[ISP]:");    \
rt_kprintf(__VA_ARGS__);    \
} while (0)
#define rk_isp_function_enter()             rt_kprintf("[ISP]:(%s) enter \n",__FUNCTION__)
#define rk_isp_function_exit()              rt_kprintf("[ISP]:(%s) exit \n",__FUNCTION__)
#else
#define ISP_DBG(...)
#define rk_isp_function_enter()
#define rk_isp_function_exit()
#endif

#if ISP_LOGERR
#include <stdio.h>
#define ISP_ERR(...)                \
do {\
rt_kprintf("[ISP ERR]:");    \
rt_kprintf(__VA_ARGS__);    \
} while (0)
#else
#define ISP_ERR(...)
#endif

#define CONFIG_RK_ISP_AUTO_UPD_CFG_BUG
//#define FPGA_TEST

/**Defines********************************************************************/
#ifndef DIV_TRUNCATE
#define DIV_TRUNCATE(x, y) ((x) / (y))
#endif

#define RK_ISP_INVALID_BUFF_ADDR ((unsigned int)~0)

#define RKISP_MI_IS_BUSY(dev)\
    (dev->config.mi_config.data_path.busy)

enum
{
    RK_ISP_ASYNC_JPEG = 0x1,
    RK_ISP_ASYNC_YCFLT = 0x2,
    RK_ISP_ASYNC_ISM = 0x4,
    RK_ISP_ASYNC_DMA = 0x8
};

#define RK_ISP_ALWAYS_ASYNC 0x00
#define RK_ISP_ALWAYS_STALL_ON_NO_BUFS (false)


/* Exported macro ------------------------------------------------------------*/
/* Rockchip ISP version information */
#define ISP_MAJOR_VERSION                   0L/* major version number */
#define ISP_MINOR_VERSION                   0L/* minor version number */
#define ISP_REVISE_REVISION                 1L/* revise version number */

/* Rockchip ISP version */
#define ISP_VERSION                         ((ISP_MAJOR_VERSION * 10000) + \
                                               (ISP_MINOR_VERSION * 100) + \
                                                ISP_REVISE_REVISION)
#define RK_ISP_DEVICE_NAME                  "isp1"
#define RK_ISP_SUBDEV_NAME                  "sensor_0"

#define RK_ISP_NUM_INPUTS                   10
#define RK_PINGPONG_BUFFER_NUM              2

/* FORMAT */
#define MAX_NB_FORMATS                      30

#define CONTRAST_DEF                        0x80
#define BRIGHTNESS_DEF                      0x0
#define HUE_DEF                             0x0

/*
    MIPI CSI2.0
*/
#define CSI2_DT_YUV420_8b                   (0x18)
#define CSI2_DT_YUV420_10b                  (0x19)
#define CSI2_DT_YUV422_8b                   (0x1E)
#define CSI2_DT_YUV422_10b                  (0x1F)
#define CSI2_DT_RGB565                      (0x22)
#define CSI2_DT_RGB666                      (0x23)
#define CSI2_DT_RGB888                      (0x24)
#define CSI2_DT_RAW8                        (0x2A)
#define CSI2_DT_RAW10                       (0x2B)
#define CSI2_DT_RAW12                       (0x2C)

#define MIPI_FIFO_BUFFER_NUM                 2
#define MIPI_ADD_DATA_FIFO_SIZE              2048

#define RKISP_INP_IS_DMA(inp)               ((inp & 0xf0000000) == RK_ISP_INP_DMA)
#define RKISP_INP_IS_MIPI(inp)              ((inp & 0xf0000000) == RK_ISP_INP_CSI)
#define RKISP_INP_IS_DVP(inp)               ((inp & 0xf0000000) == RK_ISP_INP_CPI)
#define RKISP_INP_NEED_ISP(inp)             (inp <  RK_ISP_INP_DMA_IE)
#define RKISP_INP_DMA_CNT()                 ((RK_ISP_INP_DMA_MAX -\
                            RK_ISP_INP_DMA) >> 24)

#define RK_ISP_PIX_FMT_MASK                 0xf0000000
#define RK_ISP_PIX_FMT_MASK_BPP             0x0003f000

#define RK_ISP_PIX_FMT_YUV_MASK_CPLANES     0x00000003
#define RK_ISP_PIX_FMT_YUV_MASK_UVSWAP      0x00000004
#define RK_ISP_PIX_FMT_YUV_MASK_YCSWAP      0x00000008
#define RK_ISP_PIX_FMT_YUV_MASK_X           0x00000f00
#define RK_ISP_PIX_FMT_YUV_MASK_Y           0x000000f0

#define RK_ISP_PIX_FMT_RGB_MASK_PAT         0x000000f0

#define RK_ISP_PIX_FMT_BAYER_MASK_PAT       0x000000f0

#define RK_ISP_PIX_FMT_Y_AS_BAYER           0
#define RK_ISP_SUBDEV_DATA_NUM              6

#define UTL_FIX_MAX_U0107                   1.996f //exactly this would be < 2 - 0.5/127
#define UTL_FIX_MIN_U0107                   0.0f
#define UTL_FIX_PRECISION_U0107             128.0f
#define UTL_FIX_MASK_U0107                  0x0ff

#define UTL_FIX_MAX_S0800                   127.499f //exactly this would be < 16 - 0.5/16
#define UTL_FIX_MIN_S0800                   -128.0f
#define UTL_FIX_PRECISION_S0800             0.0f
#define UTL_FIX_MASK_S0800                  0x00ff
#define UTL_FIX_SIGN_S0800                  0x0080


#define RKISP_PIX_FMT_GET_BPP(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_MASK_BPP) >> 12)
#define RKISP_PIX_FMT_SET_BPP(pix_fmt, bpp) \
    { \
        pix_fmt = ((pix_fmt & ~RK_ISP_PIX_FMT_MASK_BPP) | \
            ((bpp << 12) & RK_ISP_PIX_FMT_MASK_BPP)); \
    }

#define RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(pix_fmt) \
    (pix_fmt & RK_ISP_PIX_FMT_YUV_MASK_CPLANES)
#define RKISP_PIX_FMT_YUV_IS_YC_SWAPPED(pix_fmt) \
    (pix_fmt & RK_ISP_PIX_FMT_YUV_MASK_YCSWAP)
#define RKISP_PIX_FMT_YUV_IS_UV_SWAPPED(pix_fmt) \
        (pix_fmt & RK_ISP_PIX_FMT_YUV_MASK_UVSWAP)
#define RKISP_PIX_FMT_YUV_GET_X_SUBS(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_YUV_MASK_X) >> 8)
#define RKISP_PIX_FMT_YUV_GET_Y_SUBS(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_YUV_MASK_Y) >> 4)
#define RKISP_PIX_FMT_YUV_SET_Y_SUBS(pix_fmt, y_subs) \
    { \
        pix_fmt = ((pix_fmt & ~RK_ISP_PIX_FMT_YUV_MASK_Y) | \
            ((y_subs << 4) & RK_ISP_PIX_FMT_YUV_MASK_Y)); \
    }
#define RKISP_PIX_FMT_YUV_SET_X_SUBS(pix_fmt, x_subs) \
    { \
        pix_fmt = ((pix_fmt & ~RK_ISP_PIX_FMT_YUV_MASK_X) | \
            ((x_subs << 8) & RK_ISP_PIX_FMT_YUV_MASK_X)); \
    }
#define RKISP_PIX_FMT_YUV_SET_YC_SWAPPED(pix_fmt, yc_swapped) \
    { \
        pix_fmt = ((pix_fmt & ~RK_ISP_PIX_FMT_YUV_MASK_YCSWAP) | \
            ((yc_swapped << 3) & \
            RK_ISP_PIX_FMT_YUV_MASK_YCSWAP)); \
    }

#define RKISP_PIX_FMT_BAYER_PAT_IS_BGGR(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_BAYER_MASK_PAT) == 0x0)
#define RKISP_PIX_FMT_BAYER_PAT_IS_GBRG(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_BAYER_MASK_PAT) == 0x10)
#define RKISP_PIX_FMT_BAYER_PAT_IS_GRBG(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_BAYER_MASK_PAT) == 0x20)
#define RKISP_PIX_FMT_BAYER_PAT_IS_RGGB(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_BAYER_MASK_PAT) == 0x30)

#define RKISP_PIX_FMT_IS_YUV(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_MASK) == 0x10000000)
#define RKISP_PIX_FMT_IS_RGB(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_MASK) == 0x20000000)
#define RKISP_PIX_FMT_IS_RAW_BAYER(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_MASK) == 0x30000000)
#define RKISP_PIX_FMT_IS_JPEG(pix_fmt) \
    ((pix_fmt & RK_ISP_PIX_FMT_MASK) == 0x40000000)

#define RKISP_PIX_FMT_IS_INTERLEAVED(pix_fmt) \
    (!RKISP_PIX_FMT_IS_YUV(pix_fmt) ||\
    !RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(pix_fmt))

#define RKISP_PIX_FMT_IS_Y_ONLY(pix_fmt) \
    ((RKISP_PIX_FMT_IS_YUV(pix_fmt) == 1) && \
    (RKISP_PIX_FMT_YUV_GET_X_SUBS(pix_fmt) == 0) && \
    (RKISP_PIX_FMT_YUV_GET_Y_SUBS(pix_fmt) == 0) && \
    (RKISP_PIX_FMT_YUV_IS_YC_SWAPPED(pix_fmt) == 0))

#define RK_ISP_ALL_STREAMS \
    (RK_ISP_STREAM_SP | \
    RK_ISP_STREAM_MP | \
    RK_ISP_STREAM_DMA | \
    RK_ISP_STREAM_Y12)


enum rk_isp_subdev_state
{
    RK_ISP_SUBDEV_STATE_OFF = 0,
    RK_ISP_SUBDEV_STATE_SW_STNDBY = 1,
    RK_ISP_SUBDEV_STATE_STREAMING = 2
};

enum rk_isp_state
{
    /* path not yet opened: */
    RK_ISP_STATE_DISABLED = 0,
    /* path opened but not yet configured: */
    RK_ISP_STATE_INACTIVE = 1,
    /* path opened and configured, ready for streaming: */
    RK_ISP_STATE_READY = 2,
    /* path is streaming: */
    RK_ISP_STATE_STREAMING = 3
};

enum rk_isp_pm_state
{
    RK_ISP_PM_STATE_OFF,
    RK_ISP_PM_STATE_SUSPENDED,
    RK_ISP_PM_STATE_SW_STNDBY,
    RK_ISP_PM_STATE_STREAMING
};

enum rk_isp_pinctrl_state
{
    RK_ISP_PINCTRL_STATE_SLEEP,
    RK_ISP_PINCTRL_STATE_INACTIVE,
    RK_ISP_PINCTRL_STATE_DEFAULT,
    RK_ISP_PINCTRL_STATE_ACTIVE
};

enum rk_isp_flash_mode
{
    RK_ISP_FLASH_MODE_OFF,
    RK_ISP_FLASH_MODE_FLASH,
    RK_ISP_FLASH_MODE_TORCH,
};

enum rk_isp_cid
{
    RK_ISP_CID_FLASH_MODE                   = 0,
    RK_ISP_CID_EXPOSURE_TIME                = 1,
    RK_ISP_CID_ANALOG_GAIN                  = 2,
    RK_ISP_CID_WB_TEMPERATURE               = 3,
    RK_ISP_CID_BLACK_LEVEL                  = 4,
    RK_ISP_CID_AUTO_GAIN                    = 5,
    RK_ISP_CID_AUTO_EXPOSURE                = 6,
    RK_ISP_CID_AUTO_WHITE_BALANCE           = 7,
    RK_ISP_CID_FOCUS_ABSOLUTE               = 8,
    RK_ISP_CID_AUTO_N_PRESET_WHITE_BALANCE  = 9,
    RK_ISP_CID_SCENE_MODE                   = 10,
    RK_ISP_CID_SUPER_IMPOSE                 = 11,
    RK_ISP_CID_JPEG_QUALITY                 = 12,
    RK_ISP_CID_IMAGE_EFFECT                 = 13,
    RK_ISP_CID_HFLIP                        = 14,
    RK_ISP_CID_VFLIP                        = 15,
    RK_ISP_CID_AUTO_FPS                     = 16,
    RK_ISP_CID_VBLANKING                    = 17,
    RK_ISP_CID_ISO_SENSITIVITY              = 18,
    RK_ISP_CID_BAND_STOP_FILTER             = 19,
    RK_ISP_CID_VTS                          = 20,
    RK_ISP_CID_L_EXP                        = 21,
    RK_ISP_CID_L_GAIN                       = 22,
    RK_ISP_CID_S_EXP                        = 23,
    RK_ISP_CID_S_GAIN                       = 24,
};

enum rk_isp_buff_fmt
{
    /* values correspond to bitfield values */
    RK_ISP_BUFF_FMT_PLANAR                  = 0,
    RK_ISP_BUFF_FMT_SEMIPLANAR              = 1,
    RK_ISP_BUFF_FMT_INTERLEAVED             = 2,

    RK_ISP_BUFF_FMT_RAW8                    = 0,
    RK_ISP_BUFF_FMT_RAW12                   = 2
};

struct rk_isp_hw_error
{
    char *name;
    unsigned int count;
    unsigned int mask;
    unsigned int type;  /*isp:0 ;mipi:1*/
};

struct rk_isp_rsz_config
{
    struct rk_isp_frm_fmt *input;
    struct rk_isp_frm_fmt output;
    bool ycflt_adjust;
    bool ism_adjust;
};

struct rk_isp_dcrop_config
{
    unsigned int h_offs;
    unsigned int v_offs;
    unsigned int h_size;
    unsigned int v_size;
};

struct rk_isp_sp_config
{
    struct rk_isp_rsz_config rsz_config;
    struct rk_isp_dcrop_config dcrop;
};

struct rk_isp_mp_config
{
    struct rk_isp_rsz_config rsz_config;
    struct rk_isp_dcrop_config dcrop;
};

struct rk_isp_mi_path_config
{
    struct rk_isp_frm_fmt *input;
    struct rk_isp_frm_fmt output;
    unsigned int llength;
    unsigned int curr_buff_addr;
    unsigned int next_buff_addr;
    unsigned int cb_offs;
    unsigned int cr_offs;
    unsigned int y_size;
    unsigned int cb_size;
    unsigned int cr_size;
    bool busy;
};

struct rk_isp_mi_config
{
    bool raw_enable;
    unsigned int async_updt;
    struct rk_isp_mi_path_config data_path;
};

struct rk_isp_mi_state
{
    unsigned long flags;
    unsigned int isp_ctrl;
    unsigned int y_base_ad;
    unsigned int y_size;
    unsigned int cb_base_ad;
    unsigned int cb_size;
    unsigned int cr_base_ad;
    unsigned int cr_size;
};

struct rk_isp_image_effect_config
{
    enum rk_isp_image_effect effect;
};

/* IS */
struct rk_isp_ism_params
{
    unsigned int ctrl;
    unsigned int recenter;
    unsigned int h_offs;
    unsigned int v_offs;
    unsigned int h_size;
    unsigned int v_size;
    unsigned int max_dx;
    unsigned int max_dy;
    unsigned int displace;
};

struct rk_isp_ism_config
{
    bool ism_en;
    struct rk_isp_ism_params ism_params;
    bool ism_update_needed;
};

struct rk_isp_isp_config
{
    struct rk_isp_ie_config ie_config;
    struct rk_isp_ism_config ism_config;
    struct rk_isp_frm_fmt *input;
    struct rk_isp_frm_fmt output;
    bool si_enable;
};

struct rk_isp_config
{
    enum rk_isp_flash_mode flash_mode;
    enum rk_isp_inp input_sel;
    struct rk_isp_mi_config mi_config;
    struct rk_isp_sp_config sp_config;
    struct rk_isp_mp_config mp_config;
    struct rk_isp_strm_fmt img_src_output;
    struct rk_isp_isp_config isp_config;
    struct pltfrm_cam_itf cam_itf;
    bool out_of_buffer_stall;
};

struct rk_isp_subdev_ctrl
{
    unsigned int id;
    int val;
    unsigned int actual_val;
};

struct rk_isp_subdev_add_ctrl
{
    unsigned short r_gain;
    unsigned short b_gain;
    unsigned short gr_gain;
    unsigned short gb_gain;
    int lsc_table[17 * 17];
};

struct rk_isp_subdev_name
{
    unsigned char data[100];
};

struct rk_isp_subdev_ext_ctrl
{
    int cnt;
    unsigned int class;
    struct rk_isp_img_src_ctrl *ctrls;
    struct rk_isp_img_src_add_ctrl *add_ctrls;
};

struct rk_isp_subdev_exp
{
    rt_list_t list;
    struct rk_isp_subdev_ext_ctrl exp;
    unsigned char exp_idx;
    unsigned char exp_valid_frms[2];
    unsigned char inited;
};

struct rk_isp_subdev_data
{
    unsigned int v_frame_id;
    struct isp_supplemental_sensor_mode_data data;
};

struct rk_isp_exps
{
    rt_mutex_t mutex;   /* protect frm_exp */
    struct rk_isp_subdev_data data[RK_ISP_SUBDEV_DATA_NUM];
    unsigned char exp_idx;
    unsigned char exp_valid_frms[2];
    unsigned char inited;
};


enum rk_isp_light_stat
{
    RK_ISP_LIGHT_OFF = 0,
    RK_ISP_LIGHT_ON
};

struct rk_isp_light_ctl
{
    enum rk_isp_light_stat new_stat;
    enum rk_isp_light_stat old_stat;
    enum rk_isp_light_stat cur_stat;
    enum light_ctl_mode_s ctl_mode;
    bool is_exclusive;
    bool is_autoctl_running;
    bool is_ae_work;
    unsigned int lgt_dlynum;
    unsigned int start_frame_id;
    unsigned int frame_num;
};

struct rk_isp_light
{
    struct rk_isp_light_ctl light_ctl[LIGHT_NUM_MAX];
    unsigned int light_num;
    char is_frame_discard;
    int lgt_alter_dlynum;
    char is_alterctl_running;
};

struct rk_isp_stream_fmt
{
    char *name;
    unsigned int fourcc;
    int flags;
    int depth;
    unsigned char rotation;
    unsigned char overlay;
};

struct rk_fmtdesc
{
    unsigned int    index;             /* Format number      */
    unsigned int    type;              /* enum v4l2_buf_type */
    unsigned int    flags;
    unsigned char   description[32];   /* Description string */
    unsigned int    pixelformat;       /* Format fourcc      */
    unsigned int    reserved[4];
};

struct rk_isp_fifo_data
{
    unsigned int write_id;
    unsigned int read_id;
    unsigned int fifo_data_cnt;
    unsigned int fifo_data[MIPI_ADD_DATA_FIFO_SIZE];
};

struct mipi_dphy_hsfreqrange
{
    unsigned int range_l;
    unsigned int range_h;
    unsigned char cfg_bit;
};

struct rk_isp_frame_buffer
{
    unsigned int addr[3];
    unsigned int size;
};

struct rk_isp_stream
{
    enum rk_isp_stream_id id;
    enum rk_isp_state state;
    bool updt_cfg;
    bool stall;
    bool stop;
};

enum rk_isp_irq_index
{
    RK_ISP_IRQ_ISP,
    RK_ISP_IRQ_MI,
    RK_ISP_IRQ_MIPI,
    RK_ISP_IRQ_MAX,
};

struct rk_isp_irq
{
    int irq_num;
    char name[20];
    rt_isr_handler_t irq_handler;
};

struct set_ctrl
{
    int id;
    int value;
};

struct rk_isp_clk_set
{
    struct clk_gate *p_hclk_isp_gate;
    struct clk_gate *p_aclk_isp_gate;
    struct clk_gate *p_pll_isp_gate;
    bool clk_en_flag;
};

struct rk_isp_device
{
    struct rk_isp_dev parent;

    enum rk_isp_stream_id rk_isp_data_path;
    char name[RK_ISP_DEVICE_NAME_SIZE];

    struct rk_isp_clk_set isp_clk_set;
    struct rk_camera_device *subdev;
    struct rk_camera_device *subdev_array[RK_ISP_NUM_INPUTS];
    unsigned int subdev_cnt;
    struct rk_isp_exps subdev_exps;

    struct rt_rkisp_isp_dev isp_dev;
    struct rk_isp_config config;

    struct rk_isp_irq *irq_set;
    struct isp_init_info *isp_init;

    struct rk_isp_stream data_stream;

    struct rk_buffer_metadata_s isp_buf_metadata[RK_PINGPONG_BUFFER_NUM];
    struct rk_isp_frame_buffer frame_buffer[RK_PINGPONG_BUFFER_NUM];

    rt_mutex_t  mutex;
    int     buf_index;
    int     frame_id;
    int     frame_seg_last;
    int     frame_seg_count;
    int     frame_seg_offset;
    bool    b_stream_on;
    bool    b_isp_frame_in;
    bool    b_mi_frame_end;
    bool    b_set_out_addr;
    bool    b_set_uv_flag;
    bool    b_set_eff_en_flag;
    bool    b_set_eff_clr_flag;

};

struct rk_isp_fmt *get_rk_isp_output_format(int index);
int get_rk_isp_output_format_size(void);

int rt_do_get_rkisp_target_frm_size(
    struct rk_isp_device *dev,
    unsigned int *target_width,
    unsigned int *target_height);

int rt_do_calc_rkisp_isp_cropping(
    struct rk_isp_device *dev,
    unsigned int *width,
    unsigned int *height,
    unsigned int *h_offs,
    unsigned int *v_offs);

int rt_do_set_rkisp_input(
    struct rk_isp_device *dev,
    unsigned int input);


/*Clean code starts from here*************************************************/

int rt_do_set_rkisp_metadata(
    struct rk_isp_device *dev);

void rt_do_sync_rkisp_sensor_mode_data(
    struct rk_isp_device *dev,
    unsigned int frame_id,
    struct isp_supplemental_sensor_mode_data *data);

rt_err_t rk_isp_init(struct rk_isp_dev *dev);



/* #define FPGA_TEST */
#endif

#endif

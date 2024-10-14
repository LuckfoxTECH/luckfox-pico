/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    jpeg_enc.h
  * @version V0.0.1
  * @brief   jpeg enc device abstract for swallow
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-12     jpeg enc Team   first implementation
  *
  ******************************************************************************
  */
#ifndef __DRV_JPEG_ENC_H__
#define __DRV_JPEG_ENC_H__

#include <rtdevice.h>
#include <rtthread.h>
#include "hal_base.h"

#include "jpege_hdr.h"

#define JPEGE_TAG "jpeg_enc"

/* add uvc external app2 struct length */
#define JPEGE_HEADER_LEN (623 + (sizeof(struct __PACKED uvc_ext_data) + 4))

#define JPEGE_DEBUG 0
#define JPEGE_ERROR 1
#if JPEGE_DEBUG
#define jpege_dbg(fmt, ...) \
    do {                                                \
        rt_kprintf("%s: " fmt, JPEGE_TAG, ##__VA_ARGS__);\
    } while(0)
#else
#define jpege_dbg(fmt, ...) \
do { \
} while(0)
#endif

#if JPEGE_ERROR
#define jpege_err(fmt, ...) \
    do {                                                \
        rt_kprintf("%s err: " fmt, JPEGE_TAG, ##__VA_ARGS__);\
    } while(0)
#endif

#define MPX_PATCH_NUM               (16)
#define MPP_FRAME_FMT_MASK          (0x000fffff)
#define MPP_FRAME_FMT_COLOR_MASK    (0x000f0000)
#define MPP_FRAME_FMT_YUV           (0x00000000)
#define MPP_FRAME_FMT_LE_MASK       (0x01000000)

#define JPEGE0_BASE                 (0x40140000U)
#define JPEGE1_BASE                 (0x40150000U)
enum frame_format
{
    MPP_FMT_YUV420SP        = MPP_FRAME_FMT_YUV,        /* YYYY... UV... (NV12)     */
    /*
     * A rockchip specific pixel format, without gap between pixel aganist
     * the P010_10LE/P010_10BE
     */
    MPP_FMT_YUV420SP_10BIT,
    MPP_FMT_YUV422SP,                                   /* YYYY... UVUV... (NV16)   */
    MPP_FMT_YUV422SP_10BIT,                             ///< Not part of ABI
    MPP_FMT_YUV420P,                                    /* YYYY... U...V...  (I420) */
    MPP_FMT_YUV420SP_VU,                                /* YYYY... VUVUVU... (NV21) */
    MPP_FMT_YUV422P,                                    /* YYYY... UU...VV...(422P) */
    MPP_FMT_YUV422SP_VU,                                /* YYYY... VUVUVU... (NV61) */
    MPP_FMT_YUV422_YUYV,                                /* YUYVYUYV... (YUY2)       */
    MPP_FMT_YUV422_YVYU,                                /* YVYUYVYU... (YVY2)       */
    MPP_FMT_YUV422_UYVY,                                /* UYVYUYVY... (UYVY)       */
    MPP_FMT_YUV422_VYUY,                                /* VYUYVYUY... (VYUY)       */
    MPP_FMT_YUV400,                                     /* YYYY...                  */
    MPP_FMT_YUV440SP,                                   /* YYYY... UVUV...          */
    MPP_FMT_YUV411SP,                                   /* YYYY... UV...            */
    MPP_FMT_YUV444SP,                                   /* YYYY... UVUVUVUV...      */
    MPP_FMT_YUV_BUTT,

    MPP_FMT_BUTT,
};

struct jpege_cfg
{
    uint32_t              width;
    uint32_t              height;
    uint32_t              hor_stride;
    uint32_t              ver_stride;

    /* For quantization table */
    uint32_t              quality;
    int32_t               q_factor;
    int32_t               qf_min;
    int32_t               qf_max;
    uint8_t               *qtable_y;
    uint8_t               *qtable_c;
    uint8_t               qtables[2][64];

    /*
     * For color conversion
     *
     * 0 = bt601
     * 1 = bt709
     * 2 = user defined
     */
    uint32_t              color_conversion_type;
    uint32_t              coeffA;
    uint32_t              coeffB;
    uint32_t              coeffC;
    uint32_t              coeffE;
    uint32_t              coeffF;

    /* For slice encoding mode */
    uint32_t              slice_enable;
    uint32_t              slice_size_mb_rows;
    uint32_t              restart_interval;

    /*
     * For unit type and density
     *
     * units_type   0 - no unit
     *              1 - dots per inch
     *              2 - dots per cm
     *
     * X/Y density  specify the pixel aspect ratio
     */
    uint32_t              units_type;
    uint32_t              density_x;
    uint32_t              density_y;

    /* For comment header */
    uint32_t              comment_length;
    uint8_t               *comment_data;
    int32_t               buffer_size;
    uint8_t               *yuv_y_addr;
    uint8_t               *yuv_u_addr;
    uint8_t               *yuv_v_addr;
    uint8_t               *jpeg_header_addr;
    uint8_t               *output_addr;
    enum frame_format     format;
    uint32_t              jpeg_dev_id;
    uint32_t              stream_len;
    uint32_t              cfg_change;
};

struct jpege_feedback
{
    uint32_t hw_status;       /* zero -> correct; non-zero -> error */
    uint32_t stream_length;
};

struct reg_patch_slot_info
{
    uint32_t          reg_idx;
    uint32_t          offset;
};

struct reg_ext_info
{
    uint32_t          magic;      // Fix magic value 0x4C4A46
    uint32_t          count;      // valid patch info count
    struct reg_patch_slot_info patchs[MPX_PATCH_NUM];
};

struct jpege_ioc_reginfo
{
    uint32_t              *regs;
    struct reg_ext_info   extra_info;
};

struct jpege_ctx
{
    void                 *bits;
    struct jpege_ioc_reginfo  ioctl_info;
    struct jpege_cfg          *cfg;
};

enum vepu_format
{
    VEPU_FMT_YUV420PLANAR,          //0
    VEPU_FMT_YUV420SEMIPLANAR,      //1
    VEPU_FMT_YUYV422INTERLEAVED,    //2
    VEPU_FMT_UYVY422INTERLEAVED,    //3
    VEPU_FMT_RGB565,                //4
    VEPU_FMT_RGB555,                //5
    VEPU_FMT_RGB444,                //6
    VEPU_FMT_RGB888,                //7
    VEPU_FMT_RGB101010,             //8
    VEPU_FMT_BUTT,                  //9
};

struct vepu_fmt_cfg
{
    enum vepu_format format;
    uint8_t r_mask;
    uint8_t g_mask;
    uint8_t b_mask;
    uint8_t swap_8_in;
    uint8_t swap_16_in;
    uint8_t swap_32_in;
};

#define SHA_LEN          32
#define SIGN_LEN         64
#define MAX_DATA_SIZE    (1024 * 10)
#define STREAM_SESSION_LEN 32

struct __PACKED uvc_ext_data
{
    int16_t timestamp_s;
    int64_t timestamp;
    int32_t adc;
    uint32_t rgb_frm_id;
    uint32_t ir_frm_id;
    uint32_t dp_frm_id;
    uint8_t rgb_sha[SHA_LEN];
    uint8_t ir_sha[SHA_LEN];
    uint8_t dp_sha[SHA_LEN];
    uint8_t sign[SIGN_LEN];
    uint32_t frame_size;
    uint32_t temp_fps;
    uint8_t is_gray;
    uint8_t stream_session[STREAM_SESSION_LEN];
    uint8_t reserved[29];
    uint32_t version;
};

rt_err_t jpege_gen_regs(void *ctx);
rt_err_t jpege_init(struct jpege_ctx **ctx);
rt_err_t jpege_deinit(struct jpege_ctx *ctx);
rt_err_t jpege_start(void *hal);
rt_err_t jpege_end(void *hal);
rt_err_t jpege_clear_interrupt(void *hal);
rt_err_t jpege_reset(void *hal);

#endif // JPEGE_HAL_H_INCLUDED

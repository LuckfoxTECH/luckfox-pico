/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    jpeg_enc.c
  * @version V0.0.1
  * @brief   jpeg enc device abstract for swallow
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-09-12     jpeg enc Team   first implementation
  *
  ******************************************************************************
  */
#include <string.h>
#include "jpeg_enc.h"
#include "jpege_hdr.h"
#include "swallow.h"

#define JPEGE_TAG "jpeg_enc"
#define MPP_FRAME_FMT_IS_YUV(fmt)   (((fmt & MPP_FRAME_FMT_COLOR_MASK) == MPP_FRAME_FMT_YUV) && \
                                     ((fmt & MPP_FRAME_FMT_MASK) < MPP_FMT_YUV_BUTT))
#define MPP_FRAME_FMT_IS_LE(fmt)    ((fmt & MPP_FRAME_FMT_LE_MASK) == MPP_FRAME_FMT_LE_MASK)
#define VEPU_JPEGE_VEPU2_NUM_REGS   184
#define CRU_JPEGE_RESET_BASE (CRU_BASE + CRU_SOFTRST_CON02_OFFSET)
#define CRU_JPEGE0_ARESET_BIT (10)
#define CRU_JPEGE0_HRESET_BIT (0)
#define CRU_JPEGE1_ARESET_BIT (11)
#define CRU_JPEGE1_HRESET_BIT (1)

static struct vepu_fmt_cfg vepu_yuv_cfg[MPP_FMT_YUV_BUTT] =
{
    //MPP_FMT_YUV420SP
    { .format = VEPU_FMT_YUV420SEMIPLANAR, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV420SP_10BIT
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422SP
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422SP_10BIT
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV420P
    { .format = VEPU_FMT_YUV420PLANAR, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV420SP_VU
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422P
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422SP_VU
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422_YUYV
    { .format = VEPU_FMT_YUYV422INTERLEAVED, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422_YVYU
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422_UYVY
    { .format = VEPU_FMT_UYVY422INTERLEAVED, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV422_VYUY
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV400
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV440SP
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV411SP
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
    //MPP_FMT_YUV444SP
    { .format = VEPU_FMT_BUTT, .r_mask = 0, .g_mask = 0, .b_mask = 0, .swap_8_in = 1, .swap_16_in = 1, .swap_32_in = 1, },
};

typedef struct jpege_vepu2_reg_set_t
{
    uint32_t  val[VEPU_JPEGE_VEPU2_NUM_REGS];
} jpege_vepu2_reg_set;

static const uint32_t qp_reorder_table[64] =
{
    0,  8, 16, 24,  1,  9, 17, 25, 32, 40, 48, 56, 33, 41, 49, 57,
    2, 10, 18, 26,  3, 11, 19, 27, 34, 42, 50, 58, 35, 43, 51, 59,
    4, 12, 20, 28,  5, 13, 21, 29, 36, 44, 52, 60, 37, 45, 53, 61,
    6, 14, 22, 30,  7, 15, 23, 31, 38, 46, 54, 62, 39, 47, 55, 63
};

rt_err_t jpege_init(struct jpege_ctx **hal)
{
    rt_err_t ret = RT_EOK;
    struct jpege_ctx *ctx = (struct jpege_ctx *)rt_malloc(sizeof(struct jpege_ctx));
    jpege_bits_init(&ctx->bits);

    ctx->cfg = (struct jpege_cfg *)rt_malloc(sizeof(struct jpege_cfg));

    memset(ctx->cfg, 0, sizeof(struct jpege_cfg));
    memset(&(ctx->ioctl_info), 0, sizeof(ctx->ioctl_info));

    ctx->ioctl_info.regs = (uint32_t *)rt_malloc(sizeof(struct JPEG_ENC_REG));
    if (!ctx->ioctl_info.regs)
    {
        jpege_dbg("failed to rt_malloc vdpu2 regs\n");
        return RT_ERROR;
    }
    *hal = ctx;
    jpege_dbg("int done %p\n", hal);
    return RT_EOK;
}

rt_err_t jpege_deinit(struct jpege_ctx *ctx)
{
    jpege_dbg("jpeg deinit\n");
    jpege_bits_deinit(ctx->bits);
    jpege_dbg("jpeg free regs %p\n", ctx->ioctl_info.regs);
    if (ctx->ioctl_info.regs)
        rt_free(ctx->ioctl_info.regs);
    jpege_dbg("jpeg free cfg %p\n", ctx->cfg);
    if (ctx->cfg)
        rt_free(ctx->cfg);
    jpege_dbg("jpeg free ctx\n");
    if (ctx)
        rt_free(ctx);
    return RT_EOK;

}

static rt_err_t get_vepu_fmt(struct vepu_fmt_cfg *cfg, enum frame_format format)
{
    struct vepu_fmt_cfg *fmt_cfg = NULL;
    rt_err_t ret = RT_EOK;

    cfg->swap_16_in = 1;
    cfg->swap_32_in = 1;
    cfg->swap_8_in = 1;

    if (MPP_FRAME_FMT_IS_YUV(format))
    {
        if (!MPP_FRAME_FMT_IS_LE(format))
            fmt_cfg = &vepu_yuv_cfg[format - MPP_FRAME_FMT_YUV];
    }
    else
    {
        memset(cfg, 0, sizeof(*cfg));
        cfg->format = VEPU_FMT_BUTT;
    }

    if (fmt_cfg && fmt_cfg->format != VEPU_FMT_BUTT)
    {
        memcpy(cfg, fmt_cfg, sizeof(*cfg));
    }
    else
    {
        cfg->format = VEPU_FMT_BUTT;
        ret = RT_ERROR;
    }

    return ret;
}

rt_err_t jpege_gen_regs(void *hal)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)hal;
    struct jpege_cfg *cfg = ctx->cfg;
    uint32_t width        = cfg->width;
    uint32_t height       = cfg->height;
    enum frame_format fmt = cfg->format;
    uint32_t hor_stride   = RT_ALIGN(width, 16);
    uint32_t ver_stride   = RT_ALIGN(height, 16);
    JpegeBits bits        = ctx->bits;
    uint32_t *regs        = ctx->ioctl_info.regs;
    struct reg_ext_info *extra_info = &(ctx->ioctl_info.extra_info);
    uint8_t  *buf = NULL;//mpp_buffer_get_ptr(output);
    size_t size = cfg->buffer_size;//mpp_buffer_get_size(output);
    //const uint8_t *qtable[2];
    uint32_t val32;
    int32_t bitpos;
    int32_t bytepos;
    uint32_t x_fill = 0;
    struct vepu_fmt_cfg fmt_cfg;
    uint32_t offset_u, offset_v;

    //hor_stride must be align with 8, and ver_stride mus align with 2
    if ((cfg->hor_stride & 0x7) || (cfg->ver_stride & 0x1))
    {
        jpege_dbg("illegal resolution, hor_stride %d, ver_stride %d, width %d, height %d\n",
                  cfg->hor_stride, cfg->ver_stride,
                  cfg->width, cfg->height);
    }

    x_fill = (hor_stride - width) / 4;
    if (x_fill > 3)
        jpege_dbg("right fill is illegal, hor_stride = %d, width = %d\n", hor_stride, width);

    jpege_dbg("enter gen_regs %p\n", hal);

    memset(regs, 0, sizeof(struct JPEG_ENC_REG));
    /*     switch (cfg->format) {
            case MPP_FMT_YUV420P : {
                offset_u = hor_stride * ver_stride;
                offset_v = hor_stride * ver_stride * 5 / 4;
            } break;
            case MPP_FMT_YUV420SP : {
                offset_u = hor_stride * ver_stride;
                offset_v = hor_stride * ver_stride;
            } break;
            default : break;
        } */

    // input address setup
    regs[48] = (uint32_t)cfg->yuv_y_addr;
    regs[49] = (uint32_t)cfg->yuv_u_addr;
    regs[50] = (uint32_t)cfg->yuv_v_addr;

    // output address setup
    bitpos = jpege_bits_get_bitpos(bits);
    bytepos = 0;//(bitpos + 7) >> 3;
    buf = cfg->output_addr;//jpege_bits_get_buf(bits);
    {
        int32_t left_byte = bytepos & 0x7;
        uint8_t *tmp = buf + (bytepos & (~0x7));

        // clear the rest bytes in 64bit
        if (left_byte)
        {
            uint32_t i;

            for (i = left_byte; i < 8; i++)
                tmp[i] = 0;
        }

        val32 = (tmp[0] << 24) |
                (tmp[1] << 16) |
                (tmp[2] <<  8) |
                (tmp[3] <<  0);

        regs[51] = val32;

        if (left_byte > 4)
        {
            val32 = (tmp[4] << 24) |
                    (tmp[5] << 16) |
                    (tmp[6] <<  8);
        }
        else
            val32 = 0;

        regs[52] = val32;
    }

    regs[53] = size - bytepos;

    // bus config
    regs[54] = 16 << 8;

    regs[60] = (((bytepos & 7) * 8) << 16) |
               (x_fill << 4) |
               (ver_stride - height);
    regs[61] = cfg->hor_stride;

    regs[77] = (uint32_t)cfg->output_addr;//(uint32_t)buf + bytepos;//mpp_buffer_get_fd(output) + (bytepos << 10);

    /* 95 - 97 color conversion parameter */
    {
        uint32_t coeffA;
        uint32_t coeffB;
        uint32_t coeffC;
        uint32_t coeffE;
        uint32_t coeffF;

        switch (cfg->color_conversion_type)
        {
        case 0 :    /* BT.601 */
        {
            /*
             * Y  = 0.2989 R + 0.5866 G + 0.1145 B
             * Cb = 0.5647 (B - Y) + 128
             * Cr = 0.7132 (R - Y) + 128
             */
            coeffA = 19589;
            coeffB = 38443;
            coeffC = 7504;
            coeffE = 37008;
            coeffF = 46740;
        }
        break;
        case 1 :    /* BT.709 */
        {
            /*
             * Y  = 0.2126 R + 0.7152 G + 0.0722 B
             * Cb = 0.5389 (B - Y) + 128
             * Cr = 0.6350 (R - Y) + 128
             */
            coeffA = 13933;
            coeffB = 46871;
            coeffC = 4732;
            coeffE = 35317;
            coeffF = 41615;
        }
        break;
        case 2 :
        {
            coeffA = cfg->coeffA;
            coeffB = cfg->coeffB;
            coeffC = cfg->coeffC;
            coeffE = cfg->coeffE;
            coeffF = cfg->coeffF;
        }
        break;
        default :
        {
            jpege_dbg("invalid color conversion type %d\n",
                      cfg->color_conversion_type);
            coeffA = 19589;
            coeffB = 38443;
            coeffC = 7504;
            coeffE = 37008;
            coeffF = 46740;
        }
        break;
        }

        regs[95] = coeffA | (coeffB << 16);
        regs[96] = coeffC | (coeffE << 16);
        regs[97] = coeffF;
    }

    regs[103] = (hor_stride >> 4) << 8  |
                (ver_stride >> 4) << 20 |
                (1 << 6) |  /* intra coding  */
                (2 << 4) |  /* format jpeg   */
                0;          /* encoder start */

    if (!get_vepu_fmt(&fmt_cfg, fmt))
    {
        regs[74] = fmt_cfg.format << 4;
        regs[98] = (fmt_cfg.b_mask & 0x1f) << 16 |
                   (fmt_cfg.g_mask & 0x1f) << 8  |
                   (fmt_cfg.r_mask & 0x1f);
        regs[105] = 7 << 26 | (fmt_cfg.swap_32_in & 1) << 29 |
                    (fmt_cfg.swap_16_in & 1) << 30 |
                    (fmt_cfg.swap_8_in & 1) << 31;
    }

    /* encoder interrupt */
    regs[109] = 1 << 12 |   /* clock gating */
                0 << 10;    /* enable timeout interrupt */

    /* 0 ~ 31 quantization tables */
    {
        int32_t i;

        for (i = 0; i < 16; i++)
        {
            /* qtable need to reorder in particular order */
            regs[i] = cfg->qtables[0][qp_reorder_table[i * 4 + 0]] << 24 |
                      cfg->qtables[0][qp_reorder_table[i * 4 + 1]] << 16 |
                      cfg->qtables[0][qp_reorder_table[i * 4 + 2]] << 8 |
                      cfg->qtables[0][qp_reorder_table[i * 4 + 3]];
        }
        for (i = 0; i < 16; i++)
        {
            /* qtable need to reorder in particular order */
            regs[i + 16] = cfg->qtables[1][qp_reorder_table[i * 4 + 0]] << 24 |
                           cfg->qtables[1][qp_reorder_table[i * 4 + 1]] << 16 |
                           cfg->qtables[1][qp_reorder_table[i * 4 + 2]] << 8 |
                           cfg->qtables[1][qp_reorder_table[i * 4 + 3]];
        }
    }

    jpege_dbg("leave gen_regs %p\n", hal);
    return RT_EOK;
}

rt_err_t jpege_start(void *hal)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)hal;
    uint32_t *regs = ctx->ioctl_info.regs;
    struct JPEG_ENC_REG *hw_reg = RT_NULL;
    uint32_t id = ctx->cfg->jpeg_dev_id;
    if (id)
        hw_reg = JPEG_ENC1;
    else
        hw_reg = JPEG_ENC0;

    hw_reg->SWREG_103 = regs[103];
    rt_memcpy(hw_reg, regs, sizeof(struct JPEG_ENC_REG));
    /* enable set */
    hw_reg->SWREG_103 = regs[103] | 0x1;
    return RT_EOK;
}

rt_err_t jpege_end(void *hal)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)hal;
    if (ctx == RT_NULL)
        return RT_ERROR;
    struct JPEG_ENC_REG *hw_reg = RT_NULL;
    uint32_t id = ctx->cfg->jpeg_dev_id;
    uint32_t *regs = ctx->ioctl_info.regs;
    uint32_t val, sw_bits, hw_bits;

    if (id)
        hw_reg = JPEG_ENC1;
    else
        hw_reg = JPEG_ENC0;

    //rt_memcpy(regs, hw_reg, sizeof(struct JPEG_ENC_REG));
    /* check hw status */
    val = hw_reg->SWREG_109;
    jpege_dbg("reg[109] 0x%0x\n", val);
    if (val & 0x70)
    {
        jpege_err("jpege err 0x%x\n", val);
        /* reset */
        jpege_reset(ctx);
    }
    /* get stream len */
    val = hw_reg->SWREG_53;
    hw_bits = val;
    sw_bits = jpege_bits_get_bitpos(ctx->bits);
    jpege_dbg("jpege end hw_bits %d, sw_bits %d\n", hw_bits, sw_bits);
    ctx->cfg->stream_len = (sw_bits / 8) + hw_bits / 8;

    return RT_EOK;
}

rt_err_t jpege_reset(void *hal)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)hal;
    if (ctx == RT_NULL)
        return RT_ERROR;
    uint32_t val = 0;
    uint32_t id = ctx->cfg->jpeg_dev_id;
    uint32_t *reset_base = (uint32_t *)(CRU_JPEGE_RESET_BASE);

    if (id)
    {
        val = (1 << CRU_JPEGE1_ARESET_BIT | 1 << CRU_JPEGE1_HRESET_BIT);
        *reset_base |= val;
        HAL_DelayUs(5);
        *reset_base &= ~val;
    }
    else
    {
        val = (1 << CRU_JPEGE0_ARESET_BIT | 1 << CRU_JPEGE0_HRESET_BIT);
        *reset_base |= val;
        HAL_DelayUs(5);
        *reset_base &= ~val;
    }
    jpege_dbg("reset jpege\n");
    return RT_EOK;
}

rt_err_t jpege_clear_interrupt(void *hal)
{
    struct jpege_ctx *ctx = (struct jpege_ctx *)hal;
    if (ctx == RT_NULL)
        return RT_ERROR;
    struct JPEG_ENC_REG *hw_reg = RT_NULL;
    uint32_t id = ctx->cfg->jpeg_dev_id;

    if (id)
        hw_reg = JPEG_ENC1;
    else
        hw_reg = JPEG_ENC0;

    hw_reg->SWREG_109 = 0x0;
    jpege_dbg("clear interrupt\n");
    return RT_EOK;
}
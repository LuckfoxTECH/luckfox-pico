
/*
**************************************************************************
 * Rockchip driver for RK ISP 1.1
 * (Based on Intel driver for sofiaxxx)
 *
 * Copyright (C) 2015 Intel Mobile Communications GmbH
 * Copyright (C) 2016 Fuzhou Rockchip Electronics Co., Ltd.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
**************************************************************************
 */

#include "rk_isp_isp.h"
#include "rk_isp.h"
#include "isp_interface.h"
#include "hal_base.h"
#include "drv_mipi_dphy.h"
#include "ov_camera_module.h"


#if defined(RT_USING_ISP)

rt_event_t event_isp = NULL;
static void init_output_formats(void);

struct rk_fmtdesc output_formats[MAX_NB_FORMATS];

static struct rk_isp_device *g_rk_isp_instance = NULL;
static struct rk_isp_stream g_data_stream = {0};
static bool g_isp_meas_flag = false;
static uint32_t g_raw_pic_sec_count = 0;

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif
#ifndef CLIP
#define CLIP(a, min, max) (MIN(MAX((a), (min)), (max)))
#endif

static struct rk_isp_hw_error rk_isp_hw_errors[] =
{
    {
        .name = "isp_data_loss",
        .count = 0,
        .mask = RK_ISP_DATA_LOSS,
        .type = 0,
    },
    {
        .name = "isp_pic_size_err",
        .count = 0,
        .mask = RK_ISP_PIC_SIZE_ERROR,
        .type = 0,
    },
    {
        .name = "mipi_fifo_err",
        .count = 0,
        .mask = RKMIPI_SYNC_FIFO_OVFLW(1),
        .type = 1,
    },
    {
        .name = "dphy_err_sot",
        .count = 0,
        .mask = RKMIPI_ERR_SOT(3),
        .type = 1,
    },
    {
        .name = "dphy_err_sot_sync",
        .count = 0,
        .mask = RKMIPI_ERR_SOT_SYNC(3),
        .type = 1,
    },
    {
        .name = "dphy_err_eot_sync",
        .count = 0,
        .mask = RKMIPI_ERR_EOT_SYNC(3),
        .type = 1,
    },
    {
        .name = "dphy_err_ctrl",
        .count = 0,
        .mask = RKMIPI_ERR_CTRL(3),
        .type = 1,
    },
    {
        .name = "csi_err_protocol",
        .count = 0,
        .mask = RK_MIPI_ERR_PROTOCOL,
        .type = 2,
    },
    {
        .name = "csi_err_ecc1",
        .count = 0,
        .mask = RK_MIPI_ERR_ECC1,
        .type = 2,
    },
    {
        .name = "csi_err_ecc2",
        .count = 0,
        .mask = RK_MIPI_ERR_ECC2,
        .type = 2,
    },
    {
        .name = "csi_err_cs",
        .count = 0,
        .mask = RK_MIPI_ERR_CS,
        .type = 2,
    },
    {
        .name = "fifo_ovf",
        .count = 0,
        .mask = (3 << 0),
        .type = 2,
    },
    {
        .name = "isp_outform",
        .count = 0,
        .mask = RK_ISP_ERR_OUTFORM_SIZE,
        .type = 0,
    },
    {
        .name = "isp_stab",
        .count = 0,
        .mask = RK_ISP_ERR_IS_SIZE,
        .type = 0,
    },
    {
        .name = "isp_inform",
        .count = 0,
        .mask = RK_ISP_ERR_INFORM_SIZE,
        .type = 0,
    }
};

/**Static Functions***********************************************************/

static const char *rk_isp_interface_string(
    enum pltfrm_cam_itf_type itf)
{
    switch (itf)
    {
    case PLTFRM_CAM_ITF_MIPI:
        return "MIPI";
    case PLTFRM_CAM_ITF_BT601_8:
        return "DVP_BT601_8Bit";
    case PLTFRM_CAM_ITF_BT656_8:
        return "DVP_BT656_8Bit";
    case PLTFRM_CAM_ITF_BT601_10:
        return "DVP_BT601_10Bit";
    case PLTFRM_CAM_ITF_BT656_10:
        return "DVP_BT656_10Bit";
    case PLTFRM_CAM_ITF_BT601_12:
        return "DVP_BT601_12Bit";
    case PLTFRM_CAM_ITF_BT656_12:
        return "DVP_BT656_12Bit";
    case PLTFRM_CAM_ITF_BT601_16:
        return "DVP_BT601_16Bit";
    case PLTFRM_CAM_ITF_BT656_16:
        return "DVP_BT656_16Bit";
    default:
        return "UNKNOWN/UNSUPPORTED";
    }
}
static const char *rk_isp_subdev_state_string(
    enum rk_isp_subdev_state state)
{
    switch (state)
    {
    case RK_ISP_SUBDEV_STATE_OFF:
        return "OFF";
    case RK_ISP_SUBDEV_STATE_SW_STNDBY:
        return "SW_STNDBY";
    case RK_ISP_SUBDEV_STATE_STREAMING:
        return "STREAMING";
    default:
        return "UNKNOWN/UNSUPPORTED";
    }
}

static const char *rk_isp_state_string(
    enum rk_isp_state state)
{
    switch (state)
    {
    case RK_ISP_STATE_DISABLED:
        return "DISABLED";
    case RK_ISP_STATE_INACTIVE:
        return "INACTIVE";
    case RK_ISP_STATE_READY:
        return "READY";
    case RK_ISP_STATE_STREAMING:
        return "STREAMING";
    default:
        return "UNKNOWN/UNSUPPORTED";
    }
}

static const char *rk_isp_pm_state_string(
    enum rk_isp_pm_state pm_state)
{
    switch (pm_state)
    {
    case RK_ISP_PM_STATE_OFF:
        return "OFF";
    case RK_ISP_PM_STATE_SW_STNDBY:
        return "STANDBY";
    case RK_ISP_PM_STATE_SUSPENDED:
        return "SUSPENDED";
    case RK_ISP_PM_STATE_STREAMING:
        return "STREAMING";
    default:
        return "UNKNOWN/UNSUPPORTED";
    }
}

static const char *rk_isp_stream_id_string(
    enum rk_isp_stream_id stream_id)
{
    switch (stream_id)
    {
    case RK_ISP_STREAM_SP:
        return "SP";
    case RK_ISP_STREAM_MP:
        return "MP";
    case RK_ISP_STREAM_DMA:
        return "DMA";
    case RK_ISP_STREAM_ISP:
        return "ISP";
    case RK_ISP_STREAM_Y12:
        return "Y12";
    default:
        return "UNKNOWN/UNSUPPORTED";
    }
}

static const char *rk_isp_inp_string(
    enum rk_isp_inp inp)
{
    switch (inp)
    {
    case RK_ISP_INP_CSI:
        return "CSI";
    case RK_ISP_INP_CPI:
        return "CPI";
    case RK_ISP_INP_DMA:
        return "DMA(ISP)";
    case RK_ISP_INP_DMA_IE:
        return "DMA(Image Effects)";
    case RK_ISP_INP_DMA_SP:
        return "DMA(SP)";
    case RK_ISP_INP_DMA_Y12:
        return "DMA(Y12)";
    default:
        return "UNKNOWN/UNSUPPORTED";
    }
}

static const char *rk_isp_pix_fmt_string(int pixfmt)
{
    switch (pixfmt)
    {
    case RK_YUV400:
        return "YUV400";
    case RK_YUV420I:
        return "YUV420I";
    case RK_YUV420SP:
        return "YUV420SP";
    case RK_YUV420P:
        return "YUV420P";
    case RK_YVU420I:
        return "YVU420I";
    case RK_YVU420SP:
        return "YVU420SP";
    case RK_YVU420P:
        return "YVU420P";
    case RK_YUV422I:
        return "YUV422I";
    case RK_YUV422SP:
        return "YUV422SP";
    case RK_YUV422P:
        return "YUV422P";
    case RK_YVU422I:
        return "YVU422I";
    case RK_YVU422SP:
        return "YVU422SP";
    case RK_YVU422P:
        return "YVU422P";
    case RK_YUV444I:
        return "YUV444I";
    case RK_YUV444SP:
        return "YUV444SP";
    case RK_YUV444P:
        return "YUV444P";
    case RK_YVU444I:
        return "YVU444I";
    case RK_YVU444SP:
        return "YVU444SP";
    case RK_YVU444P:
        return "YVU444SP";
    case RK_UYV400:
        return "UYV400";
    case RK_UYV420I:
        return "UYV420I";
    case RK_UYV420SP:
        return "UYV420SP";
    case RK_UYV420P:
        return "UYV420P";
    case RK_VYU420I:
        return "VYU420I";
    case RK_VYU420SP:
        return "VYU420SP";
    case RK_VYU420P:
        return "VYU420P";
    case RK_UYV422I:
        return "UYV422I";
    case RK_UYV422SP:
        return "UYV422I";
    case RK_UYV422P:
        return "UYV422P";
    case RK_VYU422I:
        return "VYU422I";
    case RK_VYU422SP:
        return "VYU422SP";
    case RK_VYU422P:
        return "VYU422P";
    case RK_UYV444I:
        return "UYV444I";
    case RK_UYV444SP:
        return "UYV444SP";
    case RK_UYV444P:
        return "UYV444P";
    case RK_VYU444I:
        return "VYU444I";
    case RK_VYU444SP:
        return "VYU444SP";
    case RK_VYU444P:
        return "VYU444P";
    case RK_RGB565:
        return "RGB565";
    case RK_RGB666:
        return "RGB666";
    case RK_RGB888:
        return "RGB888";
    case RK_BAYER_SBGGR8:
        return "BAYER BGGR8";
    case RK_BAYER_SGBRG8:
        return "BAYER GBRG8";
    case RK_BAYER_SGRBG8:
        return "BAYER GRBG8";
    case RK_BAYER_SRGGB8:
        return "BAYER RGGB8";
    case RK_BAYER_SBGGR10:
        return "BAYER BGGR10";
    case RK_BAYER_SGBRG10:
        return "BAYER GBRG10";
    case RK_BAYER_SGRBG10:
        return "BAYER GRBG10";
    case RK_BAYER_SRGGB10:
        return "BAYER RGGB10";
    case RK_BAYER_SBGGR12:
        return "BAYER BGGR12";
    case RK_BAYER_SGBRG12:
        return "BAYER GBRG12";
    case RK_BAYER_SGRBG12:
        return "BAYER GRBG12";
    case RK_BAYER_SRGGB12:
        return "BAYER RGGB12";
    case RK_DATA:
        return "DATA";
    case RK_JPEG:
        return "JPEG";
    case RK_Y10:
        return "Y10";
    case RK_Y12:
        return "Y12";
    case RK_Y12_420SP:
        return "Y12_420sp";
    case RK_Y12_422SP:
        return "Y12_422sp";
    default:
        return "unknown/unsupported";
    }
}

static void rk_isp_debug_print_mi_sp(struct rk_isp_device *dev)
{
    ISP_DBG("\n  MI_CTRL 0x%08x/0x%08x\n"
            "  MI_STATUS 0x%08x\n"
            "  MI_RIS 0x%08x/0x%08x\n"
            "  MI_IMSC 0x%08x\n"
            "  MI_SP_Y_SIZE %d/%d\n"
            "  MI_SP_CB_SIZE %d/%d\n"
            "  MI_SP_CR_SIZE %d/%d\n"
            "  MI_SP_PIC_WIDTH %d\n"
            "  MI_SP_PIC_HEIGHT %d\n"
            "  MI_SP_PIC_LLENGTH %d\n"
            "  MI_SP_PIC_SIZE %d\n"
            "  MI_SP_Y_BASE_AD 0x%08x/0x%08x\n"
            "  MI_SP_Y_OFFS_CNT %d/%d\n"
            "  MI_SP_Y_OFFS_CNT_START %d\n"
            "  MI_SP_CB_OFFS_CNT %d/%d\n"
            "  MI_SP_CB_OFFS_CNT_START %d\n"
            "  MI_SP_CR_OFFS_CNT %d/%d\n"
            "  MI_SP_CR_OFFS_CNT_START %d\n",
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_MI_CTRL_SHD),
            READ_RKISP_REG(RK_MI_STATUS),
            READ_RKISP_REG(RK_MI_RIS),
            READ_RKISP_REG(RK_MI_MIS),
            READ_RKISP_REG(RK_MI_IMSC),
            READ_RKISP_REG(RK_MI_SP_Y_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_Y_SIZE_SHD),
            READ_RKISP_REG(RK_MI_SP_CB_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_CB_SIZE_SHD),
            READ_RKISP_REG(RK_MI_SP_CR_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_CR_SIZE_SHD),
            READ_RKISP_REG(RK_MI_SP_Y_PIC_WIDTH),
            READ_RKISP_REG(RK_MI_SP_Y_PIC_HEIGHT),
            READ_RKISP_REG(RK_MI_SP_Y_LLENGTH),
            READ_RKISP_REG(RK_MI_SP_Y_PIC_SIZE),
            READ_RKISP_REG(RK_MI_SP_Y_BASE_AD_INIT),
            READ_RKISP_REG(RK_MI_SP_Y_BASE_AD_SHD),
            READ_RKISP_REG(RK_MI_SP_Y_OFFS_CNT_INIT),
            READ_RKISP_REG(RK_MI_SP_Y_OFFS_CNT_SHD),
            READ_RKISP_REG(RK_MI_SP_Y_OFFS_CNT_START),
            READ_RKISP_REG(RK_MI_SP_CB_OFFS_CNT_INIT),
            READ_RKISP_REG(RK_MI_SP_CB_OFFS_CNT_SHD),
            READ_RKISP_REG(RK_MI_SP_CB_OFFS_CNT_START),
            READ_RKISP_REG(RK_MI_SP_CR_OFFS_CNT_INIT),
            READ_RKISP_REG(RK_MI_SP_CR_OFFS_CNT_SHD),
            READ_RKISP_REG(RK_MI_SP_CR_OFFS_CNT_START));
}

static void rk_isp_debug_print_mi_mp(struct rk_isp_device *dev)
{
    ISP_DBG(
        "\n  MI_CTRL 0x%08x/0x%08x\n"
        "  MI_STATUS 0x%08x\n"
        "  MI_BYTE_CNT %d\n"
        "  MI_RIS 0x%08x/0x%08x\n"
        "  MI_IMSC 0x%08x\n"
        "  MI_MP_Y_SIZE %d/%d\n"
        "  MI_MP_CB_SIZE %d/%d\n"
        "  MI_MP_CR_SIZE %d/%d\n"
        "  MI_MP_Y_BASE_AD 0x%08x/0x%08x\n"
        "  MI_MP_Y_OFFS_CNT %d/%d\n"
        "  MI_MP_Y_OFFS_CNT_START %d\n"
        "  MI_MP_CB_OFFS_CNT %d/%d\n"
        "  MI_MP_CB_OFFS_CNT_START %d\n"
        "  MI_MP_CR_OFFS_CNT %d/%d\n"
        "  MI_MP_CR_OFFS_CNT_START %d\n",
        READ_RKISP_REG(RK_MI_CTRL),
        READ_RKISP_REG(RK_MI_CTRL_SHD),
        READ_RKISP_REG(RK_MI_STATUS),
        READ_RKISP_REG(RK_MI_BYTE_CNT),
        READ_RKISP_REG(RK_MI_RIS),
        READ_RKISP_REG(RK_MI_MIS),
        READ_RKISP_REG(RK_MI_IMSC),
        READ_RKISP_REG(RK_MI_MP_Y_SIZE_INIT),
        READ_RKISP_REG(RK_MI_MP_Y_SIZE_SHD),
        READ_RKISP_REG(RK_MI_MP_CB_SIZE_INIT),
        READ_RKISP_REG(RK_MI_MP_CB_SIZE_SHD),
        READ_RKISP_REG(RK_MI_MP_CR_SIZE_INIT),
        READ_RKISP_REG(RK_MI_MP_CR_SIZE_SHD),
        READ_RKISP_REG(RK_MI_MP_Y_BASE_AD_INIT),
        READ_RKISP_REG(RK_MI_MP_Y_BASE_AD_SHD),
        READ_RKISP_REG(RK_MI_MP_Y_OFFS_CNT_INIT),
        READ_RKISP_REG(RK_MI_MP_Y_OFFS_CNT_SHD),
        READ_RKISP_REG(RK_MI_MP_Y_OFFS_CNT_START),
        READ_RKISP_REG(RK_MI_MP_CB_OFFS_CNT_INIT),
        READ_RKISP_REG(RK_MI_MP_CB_OFFS_CNT_SHD),
        READ_RKISP_REG(RK_MI_MP_CB_OFFS_CNT_START),
        READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_INIT),
        READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_SHD),
        READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_START));
}

static void rk_isp_debug_print_mi_y12(struct rk_isp_device *dev)
{
    ISP_DBG(
        "\n  MI_CTRL 0x%08x/0x%08x\n"
        "  MI_STATUS 0x%08x\n"
        "  MI_BYTE_CNT %d\n"
        "  MI_RIS 0x%08x/0x%08x\n"
        "  MI_IMSC 0x%08x\n"
        "  MI_MP_CR_SIZE %d/%d\n"
        "  MI_MP_CR_OFFS_CNT %d/%d\n"
        "  MI_MP_CR_OFFS_CNT_START %d\n",
        READ_RKISP_REG(RK_MI_CTRL),
        READ_RKISP_REG(RK_MI_CTRL_SHD),
        READ_RKISP_REG(RK_MI_STATUS),
        READ_RKISP_REG(RK_MI_BYTE_CNT),
        READ_RKISP_REG(RK_MI_RIS),
        READ_RKISP_REG(RK_MI_MIS),
        READ_RKISP_REG(RK_MI_IMSC),
        READ_RKISP_REG(RK_MI_MP_CR_SIZE_INIT),
        READ_RKISP_REG(RK_MI_MP_CR_SIZE_SHD),
        READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_INIT),
        READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_SHD),
        READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_START));
}

static void rk_isp_debug_print_srsz(struct rk_isp_device *dev)
{
    ISP_DBG(
        "\n  SRSZ_CTRL 0x%08x/0x%08x\n"
        "  SRSZ_SCALE_HY %d/%d\n"
        "  SRSZ_SCALE_HCB %d/%d\n"
        "  SRSZ_SCALE_HCR %d/%d\n"
        "  SRSZ_SCALE_VY %d/%d\n"
        "  SRSZ_SCALE_VC %d/%d\n"
        "  SRSZ_PHASE_HY %d/%d\n"
        "  SRSZ_PHASE_HC %d/%d\n"
        "  SRSZ_PHASE_VY %d/%d\n"
        "  SRSZ_PHASE_VC %d/%d\n",
        READ_RKISP_REG(RK_SRSZ_CTRL),
        READ_RKISP_REG(RK_SRSZ_CTRL_SHD),
        READ_RKISP_REG(RK_SRSZ_SCALE_HY),
        READ_RKISP_REG(RK_SRSZ_SCALE_HY_SHD),
        READ_RKISP_REG(RK_SRSZ_SCALE_HCB),
        READ_RKISP_REG(RK_SRSZ_SCALE_HCB_SHD),
        READ_RKISP_REG(RK_SRSZ_SCALE_HCR),
        READ_RKISP_REG(RK_SRSZ_SCALE_HCR_SHD),
        READ_RKISP_REG(RK_SRSZ_SCALE_VY),
        READ_RKISP_REG(RK_SRSZ_SCALE_VY_SHD),
        READ_RKISP_REG(RK_SRSZ_SCALE_VC),
        READ_RKISP_REG(RK_SRSZ_SCALE_VC_SHD),
        READ_RKISP_REG(RK_SRSZ_PHASE_HY),
        READ_RKISP_REG(RK_SRSZ_PHASE_HY_SHD),
        READ_RKISP_REG(RK_SRSZ_PHASE_HC),
        READ_RKISP_REG(RK_SRSZ_PHASE_HC_SHD),
        READ_RKISP_REG(RK_SRSZ_PHASE_VY),
        READ_RKISP_REG(RK_SRSZ_PHASE_VY_SHD),
        READ_RKISP_REG(RK_SRSZ_PHASE_VC),
        READ_RKISP_REG(RK_SRSZ_PHASE_VC_SHD));
}

static void rk_isp_debug_print_mrsz(struct rk_isp_device *dev)
{
    ISP_DBG(
        "\n  MRSZ_CTRL 0x%08x/0x%08x\n"
        "  MRSZ_SCALE_HY %d/%d\n"
        "  MRSZ_SCALE_HCB %d/%d\n"
        "  MRSZ_SCALE_HCR %d/%d\n"
        "  MRSZ_SCALE_VY %d/%d\n"
        "  MRSZ_SCALE_VC %d/%d\n"
        "  MRSZ_PHASE_HY %d/%d\n"
        "  MRSZ_PHASE_HC %d/%d\n"
        "  MRSZ_PHASE_VY %d/%d\n"
        "  MRSZ_PHASE_VC %d/%d\n",
        READ_RKISP_REG(RK_MRSZ_CTRL),
        READ_RKISP_REG(RK_MRSZ_CTRL_SHD),
        READ_RKISP_REG(RK_MRSZ_SCALE_HY),
        READ_RKISP_REG(RK_MRSZ_SCALE_HY_SHD),
        READ_RKISP_REG(RK_MRSZ_SCALE_HCB),
        READ_RKISP_REG(RK_MRSZ_SCALE_HCB_SHD),
        READ_RKISP_REG(RK_MRSZ_SCALE_HCR),
        READ_RKISP_REG(RK_MRSZ_SCALE_HCR_SHD),
        READ_RKISP_REG(RK_MRSZ_SCALE_VY),
        READ_RKISP_REG(RK_MRSZ_SCALE_VY_SHD),
        READ_RKISP_REG(RK_MRSZ_SCALE_VC),
        READ_RKISP_REG(RK_MRSZ_SCALE_VC_SHD),
        READ_RKISP_REG(RK_MRSZ_PHASE_HY),
        READ_RKISP_REG(RK_MRSZ_PHASE_HY_SHD),
        READ_RKISP_REG(RK_MRSZ_PHASE_HC),
        READ_RKISP_REG(RK_MRSZ_PHASE_HC_SHD),
        READ_RKISP_REG(RK_MRSZ_PHASE_VY),
        READ_RKISP_REG(RK_MRSZ_PHASE_VY_SHD),
        READ_RKISP_REG(RK_MRSZ_PHASE_VC),
        READ_RKISP_REG(RK_MRSZ_PHASE_VC_SHD));
}

static void rk_isp_debug_print_block(
    struct rk_isp_device *dev,
    const char *block_name)
{
    if (!rt_strncmp(block_name, "all", 3))
    {
        rk_isp_debug_print_srsz(dev);
        rk_isp_debug_print_mrsz(dev);
        rk_isp_debug_print_mi_sp(dev);
        rk_isp_debug_print_mi_mp(dev);
        rk_isp_debug_print_mi_y12(dev);
    }
    else if (!rt_strncmp(block_name, "srsz", 4))
        rk_isp_debug_print_srsz(dev);
    else if (!rt_strncmp(block_name, "mrsz", 4))
        rk_isp_debug_print_mrsz(dev);
    else if (!rt_strncmp(block_name, "mi_sp", 5))
        rk_isp_debug_print_mi_sp(dev);
    else if (!rt_strncmp(block_name, "mi_mp", 5))
        rk_isp_debug_print_mi_mp(dev);
    else
        ISP_ERR("unknown block %s\n", block_name);
}

enum rk_isp_inp rk_isp_input_index2inp(
    struct rk_isp_device *dev,
    unsigned int input)
{
    struct pltfrm_cam_itf itf;

    if (input >= dev->subdev_cnt)
    {
        return input - dev->subdev_cnt + RK_ISP_INP_DMA;
    }
    else
    {
#if defined(RT_USING_CAMERA) && RT_USING_CAMERA_CMD
        struct rk_camera_device *subdev = dev->subdev_array[input];
        ret = subdev->ops->control(subdev,
                                   PLTFRM_RKCAM_G_ITF_CFG, (void *)&itf);
#else
        itf = dev->isp_init->cam_itf;
#endif
        if (PLTFRM_CAM_ITF_IS_MIPI(itf.type))
            return RK_ISP_INP_CSI;
        if (PLTFRM_CAM_ITF_IS_DVP(itf.type))
            return RK_ISP_INP_CPI;
    }

    return -RT_EINVAL;
}

enum rk_isp_pix_fmt rt_do_rk_isp_fmt_trans(unsigned int v4l2_pix_fmt)
{
    switch (v4l2_pix_fmt)
    {
    case V4L2_PIX_FMT_GREY:
#if (RK_ISP_PIX_FMT_Y_AS_BAYER)
                return RK_BAYER_SBGGR8;
#else
                return RK_YUV400;
#endif
    case V4L2_PIX_FMT_Y10:
#if (RK_ISP_PIX_FMT_Y_AS_BAYER)
        return RK_BAYER_SBGGR10;
#else
        return RK_Y10;
#endif
    case V4L2_PIX_FMT_Y12:
#if (RK_ISP_PIX_FMT_Y_AS_BAYER)
        return RK_BAYER_SBGGR12;
#else
        return RK_Y12;
#endif
    case V4L2_PIX_FMT_YUV420:
        return RK_YUV420P;
    case V4L2_PIX_FMT_YVU420:
        return RK_YVU420P;
    case V4L2_PIX_FMT_NV12:
        return RK_YUV420SP;
    case V4L2_PIX_FMT_NV21:
        return RK_YVU420SP;
    case V4L2_PIX_FMT_YUYV:
        return RK_YUV422I;
    case V4L2_PIX_FMT_UYVY:
        return RK_UYV422I;
    case V4L2_PIX_FMT_YUV422P:
        return RK_YUV422P;
    case V4L2_PIX_FMT_NV16:
        return RK_YUV422SP;
    case V4L2_PIX_FMT_YUV444:
        return RK_YUV444P;
    case V4L2_PIX_FMT_NV24:
        return RK_YUV444SP;
    case V4L2_PIX_FMT_RGB565:
        return RK_RGB565;
    case V4L2_PIX_FMT_RGB24:
        return RK_RGB888;
    case V4L2_PIX_FMT_SBGGR8:
        return RK_BAYER_SBGGR8;
    case V4L2_PIX_FMT_SGBRG8:
        return RK_BAYER_SGBRG8;
    case V4L2_PIX_FMT_SGRBG8:
        return RK_BAYER_SGRBG8;
    case V4L2_PIX_FMT_SRGGB8:
        return RK_BAYER_SRGGB8;
    case V4L2_PIX_FMT_SBGGR10:
        return RK_BAYER_SBGGR10;
    case V4L2_PIX_FMT_SGBRG10:
        return RK_BAYER_SGBRG10;
    case V4L2_PIX_FMT_SGRBG10:
        return RK_BAYER_SGRBG10;
    case V4L2_PIX_FMT_SRGGB10:
        return RK_BAYER_SRGGB10;
    case V4L2_PIX_FMT_SBGGR12:
        return RK_BAYER_SBGGR12;
    case V4L2_PIX_FMT_SGBRG12:
        return RK_BAYER_SGBRG12;
    case V4L2_PIX_FMT_SGRBG12:
        return RK_BAYER_SGRBG12;
    case V4L2_PIX_FMT_SRGGB12:
        return RK_BAYER_SRGGB12;
    case V4L2_PIX_FMT_JPEG:
        return RK_JPEG;
    default:
        ISP_ERR("unknown or unsupported V4L2 pixel format %c%c%c%c\n",
                (unsigned char)(v4l2_pix_fmt & 0xff),
                (unsigned char)((v4l2_pix_fmt >> 8) & 0xff),
                (unsigned char)((v4l2_pix_fmt >> 16) & 0xff),
                (unsigned char)((v4l2_pix_fmt >> 24) & 0xff));
        return RK_UNKNOWN_FORMAT;
    }
}

static uint rt_do_trans_sensor_code(uint code)
{
    switch (code)
    {
    case V4L2_MBUS_FMT_Y8_1X8:
        return V4L2_PIX_FMT_GREY;
    case V4L2_MBUS_FMT_Y10_1X10:
        return V4L2_PIX_FMT_Y10;
    case V4L2_MBUS_FMT_Y12_1X12:
        return V4L2_PIX_FMT_Y12;
    case V4L2_MBUS_FMT_YUYV8_1_5X8:
    case V4L2_MBUS_FMT_YUYV8_2X8:
    case V4L2_MBUS_FMT_YUYV10_2X10:
    case V4L2_MBUS_FMT_YUYV8_1X16:
    case V4L2_MBUS_FMT_YUYV10_1X20:
        return V4L2_PIX_FMT_YUYV;
    case V4L2_MBUS_FMT_UYVY8_1_5X8:
    case V4L2_MBUS_FMT_UYVY8_2X8:
    case V4L2_MBUS_FMT_UYVY8_1X16:
        return V4L2_PIX_FMT_UYVY;
    case V4L2_MBUS_FMT_RGB565_2X8_BE:
        return V4L2_PIX_FMT_RGB565X;
    case V4L2_MBUS_FMT_RGB565_2X8_LE:
        return V4L2_PIX_FMT_RGB565;
    case V4L2_MBUS_FMT_RGB666_1X18:
        return V4L2_PIX_FMT_BGR666;
    case V4L2_MBUS_FMT_RGB888_1X24:
    case V4L2_MBUS_FMT_RGB888_2X12_BE:
    case V4L2_MBUS_FMT_RGB888_2X12_LE:
        return V4L2_PIX_FMT_RGB24;
    case V4L2_MBUS_FMT_SBGGR8_1X8:
        return V4L2_PIX_FMT_SBGGR8;
    case V4L2_MBUS_FMT_SGBRG8_1X8:
        return V4L2_PIX_FMT_SGBRG8;
    case V4L2_MBUS_FMT_SGRBG8_1X8:
        return V4L2_PIX_FMT_SGRBG8;
    case V4L2_MBUS_FMT_SRGGB8_1X8:
        return V4L2_PIX_FMT_SRGGB8;
    case V4L2_MBUS_FMT_SBGGR10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SBGGR10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADHI_BE:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADHI_LE:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADLO_BE:
    case V4L2_MBUS_FMT_SBGGR10_2X8_PADLO_LE:
    case V4L2_MBUS_FMT_SBGGR10_1X10:
        return V4L2_PIX_FMT_SBGGR10;
    case V4L2_MBUS_FMT_SGBRG10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SGBRG10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SGBRG10_1X10:
        return V4L2_PIX_FMT_SGBRG10;
    case V4L2_MBUS_FMT_SGRBG10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SGRBG10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SGRBG10_1X10:
        return V4L2_PIX_FMT_SGRBG10;
    case V4L2_MBUS_FMT_SRGGB10_ALAW8_1X8:
    case V4L2_MBUS_FMT_SRGGB10_DPCM8_1X8:
    case V4L2_MBUS_FMT_SRGGB10_1X10:
        return V4L2_PIX_FMT_SRGGB10;
    case V4L2_MBUS_FMT_SBGGR12_1X12:
        return V4L2_PIX_FMT_SBGGR12;
    case V4L2_MBUS_FMT_SGBRG12_1X12:
        return V4L2_PIX_FMT_SGBRG12;
    case V4L2_MBUS_FMT_SGRBG12_1X12:
        return V4L2_PIX_FMT_SGRBG12;
    case V4L2_MBUS_FMT_SRGGB12_1X12:
        return V4L2_PIX_FMT_SRGGB12;
    case V4L2_MBUS_FMT_JPEG_1X8:
        return V4L2_PIX_FMT_JPEG;
    default:
        return RK_UNKNOWN_FORMAT;
    }
}

static unsigned int rt_do_calc_rkisp_llength(
    unsigned int width,
    unsigned int stride,
    enum rk_isp_pix_fmt pix_fmt)
{
    if (stride == 0)
        return width;

    if (RKISP_PIX_FMT_IS_YUV(pix_fmt))
    {
        unsigned int num_cplanes =
            RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(pix_fmt);
        if (num_cplanes == 0)
            return 8 * stride / RKISP_PIX_FMT_GET_BPP(pix_fmt);
        else
            return stride;
    }
    else if (RKISP_PIX_FMT_IS_RGB(pix_fmt))
        return 8 * stride / RKISP_PIX_FMT_GET_BPP(pix_fmt);
    else
        return width;
}

static int rt_do_init_rkisp_subdev(
    struct rk_isp_device *dev)
{
    int ret = 0, i = 0;
    int count = 0;
    struct rt_device *subdev;

    rt_memset(dev->subdev_array, 0x00, sizeof(dev->subdev_array));
    struct rk_isp_subdev_name *subdev_name =
        (struct rk_isp_subdev_name *)dev->isp_init->subdev_name;
    count = dev->isp_init->subdev_cnt;

    for (int k = 0; k < count; k++)
    {
        subdev = rt_device_find(subdev_name[k].data);
        if (subdev)
        {
            dev->subdev_array[i++] = (struct rk_camera_device *)subdev;
#if defined(RT_USING_CAMERA)
            dev->subdev_array[i - 1]->ops->open(dev->subdev_array[i - 1],
                                                RT_DEVICE_OFLAG_OPEN);
#endif
        }
        else
        {
            ISP_ERR(" Can't find isp subdev:%s\n", subdev_name[k].data);
        }
    }

    dev->subdev_cnt = i;

    if (dev->subdev_cnt > 0)
    {
        return 0;
    }

    dev->subdev_cnt = 0;
    ret = -RT_ERROR;

    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_enum_rkisp_subdev_strm_fmts(void *dev, unsigned int index,
        struct rk_isp_strm_fmt_desc *frm_desc)
{

    struct rk_isp_device *ispdev = (struct rk_isp_device *)dev;
    struct rk_isp_strm_desc_init strm_desc = ispdev->isp_init->strm_desc[index];
    struct rk_isp_strm_fmt_init strm_init = ispdev->isp_init->strm_fmt;
    int ret;

    ISP_DBG("index is %d, strm_desc.pix_format is %d, strm_init.pix_fmt is %d\n",
            index, strm_desc.pix_format, strm_init.pix_fmt);
    if (index < (sizeof(ispdev->isp_init->strm_desc) / sizeof(struct rk_isp_strm_desc_init)))
    {
        if (strm_desc.height == 0 && strm_desc.width == 0 &&
                strm_desc.numerator == 0 && strm_desc.denominator == 0)
            goto __ERR;
        frm_desc->discrete_intrvl = true;
        frm_desc->pix_fmt = rt_do_rk_isp_fmt_trans(strm_desc.pix_format);
        frm_desc->min_frmsize.width = strm_desc.width;
        frm_desc->min_frmsize.height = strm_desc.height;
        frm_desc->discrete_frmsize = true;
        frm_desc->min_intrvl.numerator = strm_desc.numerator;
        frm_desc->min_intrvl.denominator = strm_desc.denominator;
        if (strm_init.height == 0 || strm_init.width == 0)
        {
            frm_desc->defrect.x_axis = 0;
            frm_desc->defrect.y_axis = 0;
            frm_desc->defrect.height = strm_desc.height;
            frm_desc->defrect.width = strm_desc.width;
        }
        else
        {
            frm_desc->defrect = strm_init.defrect;
        }
        ret = RT_EOK;
    }
    else
    {
__ERR:
        ret = RT_ERROR;
    }
    return ret;
}

static int rt_select_rkisp_subdev_strm_fmt(
    struct rk_isp_device *dev)
{
    int ret = 0;
    unsigned int index;
    struct rk_isp_strm_fmt_desc strm_fmt_desc;
    struct rk_camera_device *sub_dev;
    struct rk_isp_strm_fmt request_strm_fmt;
    bool matching_format_found = false;
    bool better_match = false;
    unsigned int target_width, target_height;
    unsigned int img_src_width, img_src_height;
    unsigned int best_diff = ~0;
    int vblanking;

    sub_dev = dev->subdev;

    if (!(sub_dev))
    {
        ISP_ERR("no image source selected as input (call s_input first)\n");
        ret = -RT_ERROR;
        goto err;
    }

    ret = rt_do_get_rkisp_target_frm_size(dev, &target_width, &target_height);
    if (IS_ERR_VALUE(ret))
        goto err;

    /* find the best matching format from the image source */
    /* TODO: frame interval and pixel format handling */
    for (index = 0;; index++)
    {
        if (rt_do_enum_rkisp_subdev_strm_fmts(dev, index, &strm_fmt_desc))
            break;
        if (!strm_fmt_desc.discrete_frmsize)
        {
            if (strm_fmt_desc.min_frmsize.width >= target_width)
                img_src_width = strm_fmt_desc.min_frmsize.width;
            else if (strm_fmt_desc.max_frmsize.width >=
                     target_width)
                img_src_width = target_width;
            else
                img_src_width = strm_fmt_desc.max_frmsize.width;
            if (strm_fmt_desc.min_frmsize.height >= target_height)
                img_src_height =
                    strm_fmt_desc.min_frmsize.height;
            else if (strm_fmt_desc.max_frmsize.height >=
                     target_height)
                img_src_height = target_height;
            else
                img_src_height = strm_fmt_desc.max_frmsize.height;
        }
        else
        {
            img_src_width = strm_fmt_desc.min_frmsize.width;
            img_src_height = strm_fmt_desc.min_frmsize.height;
        }
        if ((img_src_width >= target_width) &&
                (img_src_height >= target_height))
        {
            unsigned int diff = (target_height - (target_width * img_src_height / img_src_width))
                                > 0 ? (target_height - (target_width * img_src_height / img_src_width))
                                : -(target_height - (target_width * img_src_height / img_src_width));
            if (matching_format_found)
            {
                if (((img_src_width >=
                        request_strm_fmt.frm_fmt.width) &&
                        (img_src_height >
                         request_strm_fmt.frm_fmt.height)))
                    /* for image capturing we try to
                        maximize the size */
                    better_match = true;
                else if (((strm_fmt_desc.min_intrvl.denominator
                           /
                           strm_fmt_desc.min_intrvl.numerator)
                          >
                          (request_strm_fmt.frm_intrvl.denominator
                           /
                           request_strm_fmt.frm_intrvl.numerator)))
                    /* maximize fps */
                    better_match = true;
                else if (((strm_fmt_desc.min_intrvl.denominator
                           /
                           strm_fmt_desc.min_intrvl.numerator)
                          ==
                          (request_strm_fmt.frm_intrvl.denominator
                           /
                           request_strm_fmt.frm_intrvl.numerator))
                         &&
                         (diff < best_diff))
                    /* chose better aspect ratio
                        match if fps equal */
                    better_match = true;
                else
                    better_match = false;
            }

            if (!matching_format_found || better_match)
            {
                request_strm_fmt.frm_fmt.width =
                    strm_fmt_desc.min_frmsize.width;
                request_strm_fmt.frm_fmt.height =
                    strm_fmt_desc.min_frmsize.height;
                request_strm_fmt.frm_fmt.std_id =
                    strm_fmt_desc.std_id;
                request_strm_fmt.frm_fmt.pix_fmt =
                    strm_fmt_desc.pix_fmt;
                request_strm_fmt.frm_intrvl.numerator =
                    strm_fmt_desc.min_intrvl.numerator;
                request_strm_fmt.frm_intrvl.denominator =
                    strm_fmt_desc.min_intrvl.denominator;
                request_strm_fmt.frm_fmt.defrect =
                    strm_fmt_desc.defrect;
                best_diff = diff;
                matching_format_found = true;
            }
        }
    }

    if (!matching_format_found)
    {
        ISP_ERR(
            "no matching image source format (%dx%d) found\n",
            target_width, target_height);
        ret = -RT_EINVAL;
        goto err;
    }

    ISP_DBG(
        "requesting format %s %dx%d(%d,%d,%d,%d)@%d/%dfps\n",
        rk_isp_pix_fmt_string(request_strm_fmt.frm_fmt.pix_fmt),
        request_strm_fmt.frm_fmt.width,
        request_strm_fmt.frm_fmt.height,
        request_strm_fmt.frm_fmt.defrect.x_axis,
        request_strm_fmt.frm_fmt.defrect.y_axis,
        request_strm_fmt.frm_fmt.defrect.width,
        request_strm_fmt.frm_fmt.defrect.height,
        request_strm_fmt.frm_intrvl.denominator,
        request_strm_fmt.frm_intrvl.numerator);

#if defined(RT_USING_CAMERA) && RT_USING_CAMERA_CMD
    ret = subdev->ops->control(dev->subdev, RK_CAMERA_DEVICE_STREAM_FMT, &request_strm_fmt);
    if (IS_ERR_VALUE(ret))
        goto err;
#endif
    dev->config.img_src_output = request_strm_fmt;

#if defined(RT_USING_CAMERA) && RT_USING_CAMERA_CMD
    ret = subdev->ops->control(dev->subdev, RK_DEVICE_CTRL_CAMERA_GET_BLANK, &vblanking);
    if (IS_ERR_VALUE(ret))
    {
        ISP_ERR("get vblanking failed: %d\n", ret);
        vblanking = 0;
    }

    if (vblanking >= 0)
        dev->isp_dev.v_blanking_us = vblanking;
#else
    dev->isp_dev.v_blanking_us = 3000;
#endif

    return 0;
err:
    ISP_ERR("[%s]: failed with err %d\n", __FUNCTION__,  ret);
    return ret;
}

/* This should only be called when configuring RK
    or at the frame end interrupt */
static void rt_do_config_rkisp_ism(struct rk_isp_device *dev, bool async)
{
    const struct rk_isp_ism_config *pconfig =
        &(dev->config.isp_config.ism_config);
    int ret = 0;
    unsigned int h_offs;
    unsigned int v_offs;
    struct rk_isp_frm_fmt *output;

    output = &dev->config.isp_config.output;

    if (pconfig->ism_en)
    {
        ISP_DBG("%dx%d -> %dx%d@(%d,%d)\n",
                dev->isp_dev.input_width,
                dev->isp_dev.input_height,
                pconfig->ism_params.h_size,
                pconfig->ism_params.v_size,
                pconfig->ism_params.h_offs,
                pconfig->ism_params.v_offs);
        WRITE_RKISP_REG(pconfig->ism_params.recenter, RK_ISP_IS_RECENTER);
        WRITE_RKISP_REG(pconfig->ism_params.max_dx, RK_ISP_IS_MAX_DX);
        WRITE_RKISP_REG(pconfig->ism_params.max_dy, RK_ISP_IS_MAX_DY);
        WRITE_RKISP_REG(pconfig->ism_params.displace, RK_ISP_IS_DISPLACE);
        WRITE_RKISP_REG(pconfig->ism_params.h_offs, RK_ISP_IS_H_OFFS);
        WRITE_RKISP_REG(pconfig->ism_params.v_offs, RK_ISP_IS_V_OFFS);
        WRITE_RKISP_REG(pconfig->ism_params.h_size, RK_ISP_IS_H_SIZE);
        WRITE_RKISP_REG(pconfig->ism_params.v_size, RK_ISP_IS_V_SIZE);
        WRITE_RKISP_REG_OR(1, RK_ISP_IS_CTRL);
        dev->config.isp_config.output.width =
            dev->config.isp_config.ism_config.ism_params.h_size;
        dev->config.isp_config.output.height =
            dev->config.isp_config.ism_config.ism_params.v_size;
    }
    else
    {
        /* do cropping to match output aspect ratio */
        ret = rt_do_calc_rkisp_isp_cropping(dev,
                                            &output->width, &output->height,
                                            &h_offs, &v_offs);
        if (IS_ERR_VALUE(ret))
            goto _err;
        WRITE_RKISP_REG(pconfig->ism_params.recenter, RK_ISP_IS_RECENTER);
        WRITE_RKISP_REG(pconfig->ism_params.max_dx, RK_ISP_IS_MAX_DX);
        WRITE_RKISP_REG(pconfig->ism_params.max_dy, RK_ISP_IS_MAX_DY);
        WRITE_RKISP_REG(pconfig->ism_params.displace, RK_ISP_IS_DISPLACE);
        WRITE_RKISP_REG(h_offs, RK_ISP_IS_H_OFFS);
        WRITE_RKISP_REG(v_offs, RK_ISP_IS_V_OFFS);
        WRITE_RKISP_REG(output->width, RK_ISP_IS_H_SIZE);
        WRITE_RKISP_REG(output->height, RK_ISP_IS_V_SIZE);
        WRITE_RKISP_REG(0, RK_ISP_IS_CTRL);
    }

    if (async)
        WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);

    ISP_DBG(
        "\n  ISP_IS_H_OFFS %d/%d\n"
        "  ISP_IS_V_OFFS %d/%d\n"
        "  ISP_IS_H_SIZE %d/%d\n"
        "  ISP_IS_V_SIZE %d/%d\n"
        "  ISP_IS_RECENTER 0x%08x\n"
        "  ISP_IS_MAX_DX %d\n"
        "  ISP_IS_MAX_DY %d\n"
        "  ISP_IS_DISPLACE 0x%08x\n"
        "  ISP_IS_CTRL 0x%08x\n",
        READ_RKISP_REG(RK_ISP_IS_H_OFFS),
        READ_RKISP_REG(RK_ISP_IS_H_OFFS_SHD),
        READ_RKISP_REG(RK_ISP_IS_V_OFFS),
        READ_RKISP_REG(RK_ISP_IS_V_OFFS_SHD),
        READ_RKISP_REG(RK_ISP_IS_H_SIZE),
        READ_RKISP_REG(RK_ISP_IS_H_SIZE_SHD),
        READ_RKISP_REG(RK_ISP_IS_V_SIZE),
        READ_RKISP_REG(RK_ISP_IS_V_SIZE_SHD),
        READ_RKISP_REG(RK_ISP_IS_RECENTER),
        READ_RKISP_REG(RK_ISP_IS_MAX_DX),
        READ_RKISP_REG(RK_ISP_IS_MAX_DY),
        READ_RKISP_REG(RK_ISP_IS_DISPLACE),
        READ_RKISP_REG(RK_ISP_IS_CTRL));
_err:
    return;
}

static int rt_do_config_rkisp_subdev_exps(
    struct rk_isp_device *dev)
{
    int ret = 0, addr;
    struct isp_supplemental_sensor_mode_data *sensor_mode;
    struct rk_camera_device *sub_dev;

    sub_dev = dev->subdev;

#if defined(RT_USING_CAMERA)
    ret = sub_dev->ops->control(
              sub_dev,
              RK_VIDIOC_SENSOR_MODE_DATA,
              &addr);
    sensor_mode = (struct isp_supplemental_sensor_mode_data *)addr;
#else
    sensor_mode = dev->isp_init->sensor_data;
#endif

    if (IS_ERR_VALUE(ret))
    {
        dev->subdev_exps.exp_valid_frms[VALID_FR_EXP_T_INDEX] = 4;
        dev->subdev_exps.exp_valid_frms[VALID_FR_EXP_G_INDEX] = 4;
    }
    else
    {
        if ((sensor_mode->exposure_valid_frame[VALID_FR_EXP_T_INDEX] < 2) ||
                (sensor_mode->exposure_valid_frame[VALID_FR_EXP_T_INDEX] > 6))
            dev->subdev_exps.exp_valid_frms[VALID_FR_EXP_T_INDEX] = 4;
        else
            dev->subdev_exps.exp_valid_frms[VALID_FR_EXP_T_INDEX] =
                sensor_mode->exposure_valid_frame[VALID_FR_EXP_T_INDEX];
        if ((sensor_mode->exposure_valid_frame[VALID_FR_EXP_G_INDEX] < 2) ||
                (sensor_mode->exposure_valid_frame[VALID_FR_EXP_G_INDEX] > 6))
            dev->subdev_exps.exp_valid_frms[VALID_FR_EXP_G_INDEX] = 2;
        else
            dev->subdev_exps.exp_valid_frms[VALID_FR_EXP_G_INDEX] =
                sensor_mode->exposure_valid_frame[VALID_FR_EXP_G_INDEX];
    }

    return ret;
}

static int rt_do_config_rkisp_subdev(
    struct rk_isp_device *dev)
{
    int ret = 0;

    struct rk_camera_device *sub_dev;
    sub_dev = dev->subdev;

    if (!g_data_stream.updt_cfg)
        return 0;

#if defined(RT_USING_CAMERA) && RT_USING_CAMERA_CMD
    ret = sub_dev->ops.control(
              sub_dev,
              RK_ISP_GET_CAM_ITF_CFG,
              (void *)&dev->config.cam_itf);
    if (IS_ERR_VALUE(ret))
    {
        ISP_ERR("ioctl PLTFRM_RKCAM_G_ITF_CFG failed!\n");
        goto err;
    }
#else
    dev->config.cam_itf = dev->isp_init->cam_itf;
#endif

    if (!dev->subdev_exps.inited)
    {
        rt_do_config_rkisp_subdev_exps(dev);
        dev->subdev_exps.inited = 1;
    }

    ISP_DBG("cam_itf: (type: 0x%x, dphy: %d, vc: %d, nb_lanes: %d, bitrate: %d)",
            dev->config.cam_itf.type,
            dev->config.cam_itf.cfg.mipi.dphy_index,
            dev->config.cam_itf.cfg.mipi.vc,
            dev->config.cam_itf.cfg.mipi.nb_lanes,
            dev->config.cam_itf.cfg.mipi.bit_rate);
    return 0;
err:
    ISP_ERR("[%s]:failed with error %d\n", __FUNCTION__, ret);
    return ret;
}

static int rt_do_config_rkisp_isp(
    struct rk_isp_device *dev)
{
    int ret = 0;
    unsigned int h_offs;
    unsigned int v_offs;
    unsigned int yuv_seq = 0;
    unsigned int bpp;
    unsigned int isp_input_sel = 0;
    unsigned int isp_bayer_pat = 0;
    unsigned int acq_mult = 1;
    unsigned int irq_mask = 0;
    unsigned int signal = 0;
    unsigned int y12_uv_ds_mode = 0;
    enum rk_isp_pix_fmt in_pix_fmt;
    struct rk_isp_frm_fmt *output;
    struct pltfrm_cam_itf *cam_itf;
    struct awb_init_info *awb_scene =
        (struct awb_init_info *)dev->isp_init->scene;
    unsigned int idx;

    if (dev->config.input_sel == RK_ISP_INP_DMA_IE)
    {
        dev->config.isp_config.output =
            dev->config.mi_config.data_path.output;
        rt_do_disable_rkisp(&dev->isp_dev);
        return 0;
    }
    else if (dev->config.input_sel == RK_ISP_INP_DMA_SP)
    {
        WRITE_RKISP_REG_AND(~RK_ICCL_ISP_CLK, RK_ICCL);
        ISP_ERR(
            "ISP disabled\n");
        return 0;
    }
    WRITE_RKISP_REG_OR(RK_ICCL_ISP_CLK, RK_ICCL);

    in_pix_fmt = dev->config.isp_config.input->pix_fmt;

    output = &dev->config.isp_config.output;
    cam_itf = &dev->config.cam_itf;

    if (RKISP_PIX_FMT_IS_RAW_BAYER(in_pix_fmt) ||
            RKISP_PIX_FMT_IS_Y_ONLY(in_pix_fmt))
    {
        if (!dev->config.mi_config.raw_enable)
        {
            output->pix_fmt = RK_YUV422I;

            if (g_data_stream.state == RK_ISP_STATE_READY)
            {
                output->quantization =
                    dev->config.mi_config.data_path.output.quantization;
            }

            if (RKISP_PIX_FMT_IS_Y_ONLY(in_pix_fmt))
            {
                WRITE_RKISP_REG(0x40c, RK_ISP_DEMOSAIC);
            }
            else
            {
                WRITE_RKISP_REG(0xc, RK_ISP_DEMOSAIC);
            }

            if (PLTFRM_CAM_ITF_IS_BT656(dev->config.cam_itf.type))
            {
                WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_BAYER_ITU656, RK_ISP_CTRL);
            }
            else
            {
                WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_BAYER_ITU601, RK_ISP_CTRL);
            }

            if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12)
            {
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_Y12_Y_OUT_EN |
                                   RK_ISP_CTRL_ISP_GAMMA_OUT_12BIT_EN,
                                   RK_ISP_CTRL);
            }
            if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12_420SP)
            {
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_Y12_Y_OUT_EN |
                                   RK_ISP_CTRL_ISP_Y12_UV_OUT_EN |
                                   RK_ISP_CTRL_ISP_GAMMA_OUT_12BIT_EN,
                                   RK_ISP_CTRL);
                y12_uv_ds_mode = RK_ISP_ACQ_PROP_Y12_UV_DS_MODE_YUV420;
            }
            if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12_422SP)
            {
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_Y12_Y_OUT_EN |
                                   RK_ISP_CTRL_ISP_Y12_UV_OUT_EN |
                                   RK_ISP_CTRL_ISP_GAMMA_OUT_12BIT_EN,
                                   RK_ISP_CTRL);
                y12_uv_ds_mode = RK_ISP_ACQ_PROP_Y12_UV_DS_MODE_YUV422;
            }
        }
        else
        {
            output->pix_fmt = in_pix_fmt;
            if (PLTFRM_CAM_ITF_IS_BT656(dev->config.cam_itf.type))
            {
                WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_RAW_PICT_ITU656, RK_ISP_CTRL);
            }
            else
            {
                WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_RAW_PICT, RK_ISP_CTRL);
            }
        }

        bpp = RKISP_PIX_FMT_GET_BPP(in_pix_fmt);
        if (bpp == 8)
        {
            isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_8B_MSB;
        }
        else if (bpp == 10)
        {
            isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_10B_MSB;
        }
        else if (bpp == 12)
        {
            isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_12B;
        }
        else
        {
            ISP_ERR("%d bits per pixel not supported\n", bpp);
            ret = -RT_EINVAL;
            goto err;
        }

        if (RKISP_PIX_FMT_IS_RAW_BAYER(in_pix_fmt))
        {
            if (RKISP_PIX_FMT_BAYER_PAT_IS_BGGR(in_pix_fmt))
            {
                isp_bayer_pat = RK_ISP_ACQ_PROP_BAYER_PAT_BGGR;
            }
            else if (RKISP_PIX_FMT_BAYER_PAT_IS_GBRG(in_pix_fmt))
            {
                isp_bayer_pat = RK_ISP_ACQ_PROP_BAYER_PAT_GBRG;
            }
            else if (RKISP_PIX_FMT_BAYER_PAT_IS_GRBG(in_pix_fmt))
            {
                isp_bayer_pat = RK_ISP_ACQ_PROP_BAYER_PAT_GRBG;
            }
            else if (RKISP_PIX_FMT_BAYER_PAT_IS_RGGB(in_pix_fmt))
            {
                isp_bayer_pat = RK_ISP_ACQ_PROP_BAYER_PAT_RGGB;
            }
            else
            {
                ISP_ERR("BAYER pattern not supported\n");
                ret = -RT_EINVAL;
                goto err;
            }
        }
    }
    else if (RKISP_PIX_FMT_IS_YUV(in_pix_fmt))
    {
        output->pix_fmt = in_pix_fmt;
        acq_mult = 2;
        if (dev->config.input_sel == RK_ISP_INP_DMA)
        {
            bpp = RKISP_PIX_FMT_GET_BPP(in_pix_fmt);
            bpp = bpp * 4 /
                  (4 + (RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) *
                        RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) / 2));
            if (bpp == 8)
                isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_8B_MSB;
            else if (bpp == 10)
                isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_10B_MSB;
            else if (bpp == 12)
                isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_12B;
            else
            {
                ISP_ERR("format %s not supported, invalid bpp %d\n",
                        rk_isp_pix_fmt_string(in_pix_fmt), bpp);
                ret = -RT_EINVAL;
                goto err;
            }
            WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_ITU601, RK_ISP_CTRL);
        }
        else
        {
            if (PLTFRM_CAM_ITF_IS_MIPI(dev->config.cam_itf.type))
            {
                isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_12B;
                WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_ITU601, RK_ISP_CTRL);
            }
            else if (PLTFRM_CAM_ITF_IS_DVP(dev->config.cam_itf.type))
            {
                if (PLTFRM_CAM_ITF_IS_BT656(dev->config.cam_itf.type))
                {
                    WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_ITU656, RK_ISP_CTRL);
                }
                else
                {
                    WRITE_RKISP_REG(RK_ISP_CTRL_ISP_MODE_ITU601, RK_ISP_CTRL);
                }

                switch (PLTFRM_CAM_ITF_DVP_BW(dev->config.cam_itf.type))
                {
                case 8:
                    isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_8B_ZERO;
                    break;
                case 10:
                    isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_10B_ZERO;
                    break;
                case 12:
                    isp_input_sel = RK_ISP_ACQ_PROP_IN_SEL_12B;
                    break;
                default:
                    ISP_ERR("%s isn't support for RK isp11\n",
                            rk_isp_interface_string(dev->config.cam_itf.type));
                    break;
                }
            }
            else
            {
                ISP_ERR("%s isn't support for RK isp11\n",
                        rk_isp_interface_string(dev->config.cam_itf.type));
            }
            /* ISP DATA LOSS is only meaningful
                when input is not DMA */
            irq_mask |= RK_ISP_DATA_LOSS;
        }
        if (RKISP_PIX_FMT_YUV_IS_YC_SWAPPED(in_pix_fmt))
        {
            yuv_seq = RK_ISP_ACQ_PROP_CBYCRY;
            RKISP_PIX_FMT_YUV_SET_YC_SWAPPED(output->pix_fmt, 0);
        }
        else if (RKISP_PIX_FMT_YUV_IS_UV_SWAPPED(in_pix_fmt))
            yuv_seq = RK_ISP_ACQ_PROP_YCRYCB;
        else
            yuv_seq = RK_ISP_ACQ_PROP_YCBYCR;
    }
    else
    {
        ISP_ERR("format %s not supported\n",
                rk_isp_pix_fmt_string(in_pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    /* Set up input acquisition properties*/
    if (PLTFRM_CAM_ITF_IS_DVP(cam_itf->type))
    {
        signal =
            ((cam_itf->cfg.dvp.pclk == PLTFRM_CAM_SDR_POS_EDG) ?
             RK_ISP_ACQ_PROP_POS_EDGE : RK_ISP_ACQ_PROP_NEG_EDGE);

        if (PLTFRM_CAM_ITF_IS_BT601(cam_itf->type))
        {
            signal |=
                (cam_itf->cfg.dvp.vsync == PLTFRM_CAM_SIGNAL_HIGH_LEVEL) ?
                RK_ISP_ACQ_PROP_VSYNC_HIGH : RK_ISP_ACQ_PROP_VSYNC_LOW;

            signal |=
                (cam_itf->cfg.dvp.hsync == PLTFRM_CAM_SIGNAL_HIGH_LEVEL) ?
                RK_ISP_ACQ_PROP_HSYNC_HIGH : RK_ISP_ACQ_PROP_HSYNC_LOW;

        }
        else
        {
            signal |= RK_ISP_ACQ_PROP_HSYNC_HIGH |
                      RK_ISP_ACQ_PROP_VSYNC_HIGH;
        }
    }
    else
    {
        signal = RK_ISP_ACQ_PROP_NEG_EDGE |
                 RK_ISP_ACQ_PROP_HSYNC_HIGH |
                 RK_ISP_ACQ_PROP_VSYNC_HIGH;
    }

    dev->isp_dev.input_width =
        dev->config.isp_config.input->defrect.width;
    dev->isp_dev.input_height =
        dev->config.isp_config.input->defrect.height;

    WRITE_RKISP_REG(signal |
                    yuv_seq |
                    RK_ISP_ACQ_PROP_FIELD_SEL_ALL |
                    isp_input_sel |
                    y12_uv_ds_mode |
                    isp_bayer_pat |
                    (0 << 20),  /*input_selection_no_app */
                    RK_ISP_ACQ_PROP);

    WRITE_RKISP_REG(0, RK_ISP_ACQ_NR_FRAMES);

    /* Acquisition Size */
    WRITE_RKISP_REG(dev->config.isp_config.input->defrect.x_axis, RK_ISP_ACQ_H_OFFS);
    WRITE_RKISP_REG(dev->config.isp_config.input->defrect.y_axis, RK_ISP_ACQ_V_OFFS);
    WRITE_RKISP_REG(acq_mult * dev->config.isp_config.input->defrect.width, RK_ISP_ACQ_H_SIZE);
    WRITE_RKISP_REG(dev->config.isp_config.input->defrect.height, RK_ISP_ACQ_V_SIZE);

    /* Acquisition Size */

    if (!dev->config.mi_config.raw_enable)
    {
        WRITE_RKISP_REG(dev->config.isp_config.input->defrect.x_axis, RK_ISP_OUT_H_OFFS);
        WRITE_RKISP_REG(dev->config.isp_config.input->defrect.y_axis, RK_ISP_OUT_V_OFFS);
        WRITE_RKISP_REG(acq_mult * dev->config.isp_config.input->defrect.width, RK_ISP_OUT_H_SIZE);
        WRITE_RKISP_REG(dev->config.isp_config.input->defrect.height, RK_ISP_OUT_V_SIZE);
    }
    else
    {
        WRITE_RKISP_REG(dev->config.isp_config.input->defrect.x_axis, RK_ISP_OUT_H_OFFS);
        WRITE_RKISP_REG(dev->config.isp_config.input->defrect.y_axis, RK_ISP_OUT_V_OFFS);
        WRITE_RKISP_REG(acq_mult * dev->config.isp_config.input->defrect.width, RK_ISP_OUT_H_SIZE);
        WRITE_RKISP_REG(dev->config.mi_config.data_path.output.height / 4, RK_ISP_OUT_V_SIZE);
    }

#if defined(RT_ISP_GREY_ENABLE)
    WRITE_RKISP_REG_OR(RK_ICCL_IE_CLK, RK_ICCL);
    WRITE_RKISP_REG(RK_IMG_EFF_CTRL_ENABLE |
                    RK_IMG_EFF_CTRL_MODE_BLACKWHITE, RK_IMG_EFF_CTRL);
    WRITE_RKISP_REG_OR(RK_IMG_EFF_CTRL_CFG_UPD, RK_IMG_EFF_CTRL);
#endif

    /* interrupt mask */
    irq_mask |=
        RK_ISP_FRAME |
        RK_ISP_PIC_SIZE_ERROR |
        RK_ISP_FRAME_IN |
        RK_ISP_V_START |
        RK_ISP_AWB_DONE |
        RK_ISP_AFM_FIN |
        RK_ISP_EXP_END |
        RK_ISP_HIST_MEASURE_RDY;
    WRITE_RKISP_REG(irq_mask, RK_ISP_IMSC);

    if (!dev->config.mi_config.raw_enable)
        rt_do_configure_rkisp(&dev->isp_dev,
                              in_pix_fmt,
                              output->quantization);
    else
        rt_do_disable_rkisp(&dev->isp_dev);

    struct pltfrm_cam_ls cam_ls;
#if defined(RT_USING_CAMERA) && RT_USING_CAMERA_CMD
    ret = subdev->ops->control(isp_dev->subdev,
                               PLTFRM_RKCAM_R_LIGHTSENSOR,
                               &cam_ls);
#else
    cam_ls.light_vol = dev->isp_init->strm_fmt.light_vol;
#endif

#if 0
    for (int idx = 0; idx < AWB_SCENE_DEFAULT_NUM; idx++)
    {
        if (cam_ls.light_vol >= awb_scene[idx].range[0] &&
                cam_ls.light_vol < awb_scene[idx].range[1])
        {
            WRITE_RKISP_REG(
                awb_scene[idx].gain[0] << 16 |
                awb_scene[idx].gain[1],
                RK_ISP_AWB_GAIN_G);
            WRITE_RKISP_REG(
                awb_scene[idx].gain[2] << 16 |
                awb_scene[idx].gain[3],
                RK_ISP_AWB_GAIN_RB);

            WRITE_RKISP_REG_OR(
                RK_ISP_CTRL_ISP_AWB_ENA,
                RK_ISP_CTRL);
        }
    }
#endif

    ISP_DBG(
        "\n  ISP_CTRL 0x%08x\n"
        "  ISP_IMSC 0x%08x\n"
        "  ISP_ACQ_PROP 0x%08x\n"
        "  ISP_ACQ %dx%d@(%d,%d)\n"
        "  ISP_OUT %dx%d@(%d,%d)\n"
        "  ISP_IS %dx%d@(%d,%d)\n",
        READ_RKISP_REG(RK_ISP_CTRL),
        READ_RKISP_REG(RK_ISP_IMSC),
        READ_RKISP_REG(RK_ISP_ACQ_PROP),
        READ_RKISP_REG(RK_ISP_ACQ_H_SIZE),
        READ_RKISP_REG(RK_ISP_ACQ_V_SIZE),
        READ_RKISP_REG(RK_ISP_ACQ_H_OFFS),
        READ_RKISP_REG(RK_ISP_ACQ_V_OFFS),
        READ_RKISP_REG(RK_ISP_OUT_H_SIZE),
        READ_RKISP_REG(RK_ISP_OUT_V_SIZE),
        READ_RKISP_REG(RK_ISP_OUT_H_OFFS),
        READ_RKISP_REG(RK_ISP_OUT_V_OFFS),
        READ_RKISP_REG(RK_ISP_IS_H_SIZE),
        READ_RKISP_REG(RK_ISP_IS_V_SIZE),
        READ_RKISP_REG(RK_ISP_IS_H_OFFS),
        READ_RKISP_REG(RK_ISP_IS_V_OFFS));

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_mipi(
    struct rk_isp_device *dev)
{
    int ret = 0, flag = 0;;
    unsigned int data_type;
    unsigned int mipi_ctrl;
    unsigned int shutdown_lanes;
    unsigned int i;
    enum rk_isp_pix_fmt in_pix_fmt;
    unsigned int embedded_data_vc;
    unsigned int embedded_data_type;
    struct rk_mipi_dphy_device *mipi;

    if (!RKISP_INP_IS_MIPI(dev->config.input_sel))
    {
        WRITE_RKISP_REG_AND(~RK_ICCL_MIPI_CLK, RK_ICCL);
        ISP_ERR("MIPI disabled\n");
        return 0;
    }
    WRITE_RKISP_REG_OR(RK_ICCL_MIPI_CLK, RK_ICCL);

    in_pix_fmt = dev->config.img_src_output.frm_fmt.pix_fmt;

    ISP_DBG("input %s, vc = %d, nb_lanes = %d\n",
            rk_isp_inp_string(dev->config.input_sel),
            dev->config.cam_itf.cfg.mipi.vc,
            dev->config.cam_itf.cfg.mipi.nb_lanes);

    if ((dev->config.cam_itf.cfg.mipi.nb_lanes == 0) ||
            (dev->config.cam_itf.cfg.mipi.nb_lanes > 4))
    {
        ISP_ERR("invalid number (%d) of MIPI lanes, valid range is [1..4]\n",
                dev->config.cam_itf.cfg.mipi.nb_lanes);
        ret = -RT_EINVAL;
        goto err;
    }

#if defined(RT_USING_MIPI_DPHY)
    mipi = (struct rk_mipi_dphy_device *)rt_device_find("mipi_dphy");
    if (mipi == NULL)
    {
        rt_kprintf("get mipi device handle failed\n");
        return 0;
    }
    mipi->dev.ops->open(&mipi->dev, 0);
    mipi->dev.ops->control(&mipi->dev, (int)1, (void *)&flag);
#endif

    WRITE_RKISP_REG_OR(0x2, GRF_BASE + GRF_SOC_CON1_OFFSET);
    WRITE_RKISP_REG_AND(~(GRF_GPIO1B_IOMUX_L_GPIO1B3_SEL_MASK),
                        GRF_BASE + GRF_GPIO1B_IOMUX_L_OFFSET);
    WRITE_RKISP_REG_OR((0xB << CRU_CLKSEL_CON09_OUTCLOCK_TEST_SEL_SHIFT),
                       CRU_BASE + CRU_CLKSEL_CON09_OFFSET);


    /* Configure Data Type and Virtual Channel */
    if (RKISP_PIX_FMT_IS_YUV(in_pix_fmt))
    {
        if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 2) &&
                (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 2) &&
                (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 12))
            data_type = CSI2_DT_YUV420_8b;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 15))
            data_type = CSI2_DT_YUV420_10b;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 4) &&
                 (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 16))
            data_type = CSI2_DT_YUV422_8b;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 4) &&
                 (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 20))
            data_type = CSI2_DT_YUV422_10b;
        else if (in_pix_fmt == RK_YUV400)
            data_type = CSI2_DT_RAW8;
        else if (in_pix_fmt == RK_Y10)
            data_type = CSI2_DT_RAW10;
        else if (in_pix_fmt == RK_Y12)
            data_type = CSI2_DT_RAW12;
        else
        {
            ISP_ERR("unsupported format %s\n",
                    rk_isp_pix_fmt_string(in_pix_fmt));
            ret = -RT_EINVAL;
            goto err;
        }
    }
    else if (RKISP_PIX_FMT_IS_RAW_BAYER(in_pix_fmt))
    {
        if (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 8)
            data_type = CSI2_DT_RAW8;
        else if (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 10)
            data_type = CSI2_DT_RAW10;
        else if (RKISP_PIX_FMT_GET_BPP(in_pix_fmt) == 12)
            data_type = CSI2_DT_RAW12;
        else
        {
            ISP_ERR("unsupported format %s\n",
                    rk_isp_pix_fmt_string(in_pix_fmt));
            ret = -RT_EINVAL;
            goto err;
        }
    }
    else if (in_pix_fmt == RK_RGB565)
    {
        data_type = CSI2_DT_RGB565;
    }
    else if (in_pix_fmt == RK_RGB666)
    {
        data_type = CSI2_DT_RGB666;
    }
    else if (in_pix_fmt == RK_RGB888)
    {
        data_type = CSI2_DT_RGB888;
    }
    else
    {
        ISP_ERR("unsupported format %s\n",
                rk_isp_pix_fmt_string(in_pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    WRITE_RKISP_REG(
        RKMIPI_DATA_SEL_DT(data_type) |
        RKMIPI_DATA_SEL_VC(dev->config.cam_itf.cfg.mipi.vc),
        RK_MIPI_IMG_DATA_SEL);

    embedded_data_vc = dev->config.cam_itf.cfg.mipi.embedded_data_vc;
    embedded_data_type = dev->config.cam_itf.cfg.mipi.embedded_data_type;
    if (embedded_data_vc <= 3)
    {
        WRITE_RKISP_REG(
            RKMIPI_DATA_SEL_DT(embedded_data_type) |
            RKMIPI_DATA_SEL_VC(embedded_data_vc),
            RK_MIPI_ADD_DATA_SEL_1);
        WRITE_RKISP_REG(
            RKMIPI_DATA_SEL_DT(embedded_data_type) |
            RKMIPI_DATA_SEL_VC(embedded_data_vc),
            RK_MIPI_ADD_DATA_SEL_2);
        WRITE_RKISP_REG(
            RKMIPI_DATA_SEL_DT(embedded_data_type) |
            RKMIPI_DATA_SEL_VC(embedded_data_vc),
            RK_MIPI_ADD_DATA_SEL_3);
        WRITE_RKISP_REG(
            RKMIPI_DATA_SEL_DT(embedded_data_type) |
            RKMIPI_DATA_SEL_VC(embedded_data_vc),
            RK_MIPI_ADD_DATA_SEL_4);
    }

    ISP_DBG("\n  embedded_data_vc 0x%08x\n"
            "  embedded_data_type 0x%08x\n",
            embedded_data_vc,
            embedded_data_type);

    /* Enable MIPI interrupts */
    WRITE_RKISP_REG(~0, RK_MIPI_ICR);
    /*
    *  Disable RK_MIPI_ERR_DPHY interrupt here temporary for
    *isp bus may be dead when switch isp.
    */
    WRITE_RKISP_REG(
        RK_MIPI_FRAME_END |
        RK_MIPI_ERR_CSI |
        RK_MIPI_ERR_DPHY |
        RKMIPI_SYNC_FIFO_OVFLW(3) |
        RK_MIPI_ADD_DATA_OVFLW,
        RK_MIPI_IMSC);

    ISP_DBG("\n  MIPI_CTRL 0x%08x\n"
            "  MIPI_IMG_DATA_SEL 0x%08x\n"
            "  MIPI_STATUS 0x%08x\n"
            "  MIPI_IMSC 0x%08x\n",
            READ_RKISP_REG(RK_MIPI_CTRL),
            READ_RKISP_REG(RK_MIPI_IMG_DATA_SEL),
            READ_RKISP_REG(RK_MIPI_STATUS),
            READ_RKISP_REG(RK_MIPI_IMSC));

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_mi_mp(
    struct rk_isp_device *dev)
{
    enum rk_isp_pix_fmt out_pix_fmt =
        dev->config.mi_config.data_path.output.pix_fmt;
    unsigned int llength =
        dev->config.mi_config.data_path.llength;
    unsigned int width =
        dev->config.mi_config.data_path.output.width;
    unsigned int height =
        dev->config.mi_config.data_path.output.height;
    unsigned int writeformat = RK_ISP_BUFF_FMT_PLANAR;
    unsigned int swap_cb_cr = 0;
    unsigned int bpp = RKISP_PIX_FMT_GET_BPP(out_pix_fmt);
    unsigned int size = llength * height * bpp / 8;
    unsigned int mi_ctrl;

    dev->config.mi_config.data_path.input =
        &dev->config.mp_config.rsz_config.output;

    ISP_DBG("%s %dx%d, llength = %d\n",
            rk_isp_pix_fmt_string(out_pix_fmt),
            width,
            height,
            llength);

    dev->config.mi_config.data_path.y_size = size;
    dev->config.mi_config.data_path.cb_size = 0;
    dev->config.mi_config.data_path.cr_size = 0;
    if (RKISP_PIX_FMT_IS_YUV(out_pix_fmt))
    {
        unsigned int num_cplanes =
            RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(out_pix_fmt);
        if (num_cplanes == 0)
        {
            writeformat = RK_ISP_BUFF_FMT_INTERLEAVED;
        }
        else
        {
            dev->config.mi_config.data_path.y_size =
                (dev->config.mi_config.data_path.y_size * 4)
                /
                (4 + (RKISP_PIX_FMT_YUV_GET_X_SUBS(
                          out_pix_fmt) *
                      RKISP_PIX_FMT_YUV_GET_Y_SUBS(
                          out_pix_fmt) / 2));
            dev->config.mi_config.data_path.cb_size =
                size - dev->config.mi_config.data_path.y_size;
            if (num_cplanes == 1)
                writeformat = RK_ISP_BUFF_FMT_SEMIPLANAR;
            else if (num_cplanes == 2)
            {
                writeformat = RK_ISP_BUFF_FMT_PLANAR;
                dev->config.mi_config.data_path.cb_size /= 2;
            }
            /* for U<->V swapping: */
            dev->config.mi_config.data_path.cr_size =
                dev->config.mi_config.data_path.cb_size;
        }
        if (RKISP_PIX_FMT_YUV_IS_UV_SWAPPED(out_pix_fmt))
            swap_cb_cr = RK_MI_XTD_FMT_CTRL_MP_CB_CR_SWAP;

        if (writeformat == RK_ISP_BUFF_FMT_SEMIPLANAR)
        {
            dev->config.mi_config.data_path.cb_offs =
                dev->config.mi_config.data_path.y_size;
            dev->config.mi_config.data_path.cr_offs =
                dev->config.mi_config.data_path.cb_offs;
        }
        else if (writeformat == RK_ISP_BUFF_FMT_PLANAR)
        {
            if (swap_cb_cr)
            {
                swap_cb_cr = 0;
                dev->config.mi_config.data_path.cr_offs =
                    dev->config.mi_config.data_path.y_size;
                dev->config.mi_config.data_path.cb_offs =
                    dev->config.mi_config.data_path.cr_offs +
                    dev->config.mi_config.data_path.cr_size;
            }
            else
            {
                dev->config.mi_config.data_path.cb_offs =
                    dev->config.mi_config.data_path.y_size;
                dev->config.mi_config.data_path.cr_offs =
                    dev->config.mi_config.data_path.cb_offs +
                    dev->config.mi_config.data_path.cb_size;
            }
        }
    }
    else if (RKISP_PIX_FMT_IS_RAW_BAYER(out_pix_fmt))
    {
        if (RKISP_PIX_FMT_GET_BPP(out_pix_fmt) > 8)
        {
            writeformat = RK_ISP_BUFF_FMT_RAW12;
            dev->config.mi_config.data_path.y_size = width * height * 2;
        }
        else
        {
            writeformat = RK_ISP_BUFF_FMT_RAW8;
            dev->config.mi_config.data_path.y_size = width * height;
        }
        dev->config.mi_config.data_path.cb_offs = 0x00;
        dev->config.mi_config.data_path.cr_offs = 0x00;
        dev->config.mi_config.data_path.cb_size = 0x00;
        dev->config.mi_config.data_path.cr_size = 0x00;
    }

    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.y_size,
                           RK_MI_MP_Y_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.cb_size,
                           RK_MI_MP_CB_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.cr_size,
                           RK_MI_MP_CR_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_OR_VERIFY(RK_MI_MP_FRAME,
                              RK_MI_IMSC, ~0);

    if (swap_cb_cr)
    {
        WRITE_RKISP_REG_OR(swap_cb_cr, RK_MI_XTD_FORMAT_CTRL);
    }

    mi_ctrl = READ_RKISP_REG(RK_MI_CTRL) |
              RKMI_CTRL_MP_WRITE_FMT(writeformat) |
              RK_MI_CTRL_BURST_LEN_LUM_64 |
              RK_MI_CTRL_BURST_LEN_CHROM_64 |
              RK_MI_CTRL_INIT_BASE_EN |
              RK_MI_CTRL_INIT_OFFSET_EN |
              RK_MI_MP_AUTOUPDATE_ENABLE;

    WRITE_RKISP_REG_VERIFY(mi_ctrl, RK_MI_CTRL, ~0);

    ISP_DBG("\n  MI_CTRL 0x%08x\n"
            "  MI_STATUS 0x%08x\n"
            "  MI_MP_Y_SIZE %d\n"
            "  MI_MP_CB_SIZE %d\n"
            "  MI_MP_CR_SIZE %d\n",
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_MI_STATUS),
            READ_RKISP_REG(RK_MI_MP_Y_SIZE_INIT),
            READ_RKISP_REG(RK_MI_MP_CB_SIZE_INIT),
            READ_RKISP_REG(RK_MI_MP_CR_SIZE_INIT));

    return 0;
}

static int rt_do_config_rkisp_mi_y12(
    struct rk_isp_device *dev)
{
    enum rk_isp_pix_fmt out_pix_fmt =
        dev->config.mi_config.data_path.output.pix_fmt;
    unsigned int llength =
        dev->config.mi_config.data_path.llength;
    unsigned int width =
        dev->config.mi_config.data_path.output.width;
    unsigned int height =
        dev->config.mi_config.data_path.output.height;
    unsigned int swap_cb_cr = 0;
    unsigned int bpp = RKISP_PIX_FMT_GET_BPP(out_pix_fmt);
    unsigned int size = llength * height * bpp / 8;
    unsigned int mi_ctrl;

    dev->config.mi_config.data_path.input =
        &dev->config.isp_config.output;

    ISP_DBG("%s %dx%d, llength = %d\n",
            rk_isp_pix_fmt_string(out_pix_fmt),
            width,
            height,
            llength);

    dev->config.mi_config.data_path.y_size = size;
    dev->config.mi_config.data_path.cb_size = 0;
    dev->config.mi_config.data_path.cr_size = 0;
    if (RKISP_PIX_FMT_IS_YUV(out_pix_fmt))
    {
        unsigned int num_cplanes =
            RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(out_pix_fmt);
        if (num_cplanes <= 1)
        {
            dev->config.mi_config.data_path.y_size =
                (dev->config.mi_config.data_path.y_size * 4)
                /
                (4 + (RKISP_PIX_FMT_YUV_GET_X_SUBS(
                          out_pix_fmt) *
                      RKISP_PIX_FMT_YUV_GET_Y_SUBS(
                          out_pix_fmt) / 2));
            dev->config.mi_config.data_path.cb_size =
                size -
                dev->config.mi_config.data_path.y_size;
            dev->config.mi_config.data_path.cb_offs =
                dev->config.mi_config.data_path.y_size;
        }
        else
        {
            ISP_ERR("wrong num_cplanes %s for y12 path\n",
                    rk_isp_pix_fmt_string(out_pix_fmt));
            return -RT_EINVAL;
        }

        if (RKISP_PIX_FMT_YUV_IS_UV_SWAPPED(out_pix_fmt))
            swap_cb_cr = RK_MI_XTD_FMT_CTRL_Y12_CB_CR_SWAP;
    }
    else if (RKISP_PIX_FMT_IS_RAW_BAYER(out_pix_fmt))
    {
        ISP_ERR("wrong format %s for y12 path\n",
                rk_isp_pix_fmt_string(out_pix_fmt));
        return -RT_EINVAL;
    }
    if (out_pix_fmt == RK_Y12)
    {
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.y_size,
                               RK_MI_MP_CR_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_OR_VERIFY(RK_MI_Y12_Y_FRAME | RK_MI_ALL_FRAME, RK_MI_IMSC, ~0);
        WRITE_RKISP_REG_OR_VERIFY(RK_MI_INIT_Y12_Y_EN,
                                  RK_MI_INIT, ~0);
        ISP_DBG("RK_MI_INIT 0x%x\n", READ_RKISP_REG(RK_MI_INIT));
    }
    else
    {
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.y_size,
                               RK_MI_MP_CR_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.cb_size,
                               RK_MI_SP_CR_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_OR_VERIFY(RK_MI_Y12_Y_FRAME | RK_MI_Y12_UV_FRAME | RK_MI_ALL_FRAME,
                                  RK_MI_IMSC, ~0);
        WRITE_RKISP_REG_OR_VERIFY((RK_MI_INIT_Y12_Y_EN | RK_MI_INIT_Y12_UV_EN),
                                  RK_MI_INIT, ~0);
        ISP_DBG("RK_MI_INIT 0x%x\n", READ_RKISP_REG(RK_MI_INIT));
    }
    if (swap_cb_cr)
    {
        WRITE_RKISP_REG_OR(swap_cb_cr, RK_MI_XTD_FORMAT_CTRL);
    }

    mi_ctrl = READ_RKISP_REG(RK_MI_CTRL) |
              RK_MI_CTRL_BURST_LEN_LUM_64 |
              RK_MI_CTRL_BURST_LEN_CHROM_64 |
              RK_MI_CTRL_INIT_BASE_EN |
              RK_MI_CTRL_INIT_OFFSET_EN |
              RK_MI_MP_AUTOUPDATE_ENABLE;

    WRITE_RKISP_REG_VERIFY(mi_ctrl, RK_MI_CTRL, ~0);

    ISP_DBG("\n  MI_CTRL 0x%08x\n"
            "  MI_STATUS 0x%08x\n"
            "  MI_Y12_Y_SIZE %d\n"
            "  MI_Y12_CBCR_SIZE %d\n",
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_MI_STATUS),
            READ_RKISP_REG(RK_MI_MP_CR_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_CR_SIZE_INIT));

    return 0;
}

static int rt_do_config_rkisp_mi_sp(
    struct rk_isp_device *dev)
{
    int ret = 0;
    enum rk_isp_pix_fmt out_pix_fmt =
        dev->config.mi_config.data_path.output.pix_fmt;
    enum rk_isp_pix_fmt in_pix_fmt =
        dev->config.sp_config.rsz_config.output.pix_fmt;
    unsigned int llength =
        dev->config.mi_config.data_path.llength;
    unsigned int width =
        dev->config.mi_config.data_path.output.width;
    unsigned int height =
        dev->config.mi_config.data_path.output.height;
    unsigned int writeformat = RK_ISP_BUFF_FMT_PLANAR;
    unsigned int swap_cb_cr = 0;
    unsigned int bpp = RKISP_PIX_FMT_GET_BPP(out_pix_fmt);
    unsigned int size = llength * height * bpp / 8;
    unsigned int input_format = 0;
    unsigned int output_format;
    unsigned int mi_ctrl;

    if (out_pix_fmt == RK_YUV400)
        in_pix_fmt = RK_YUV400;

    dev->config.mi_config.data_path.input =
        &dev->config.sp_config.rsz_config.output;

    ISP_DBG("%s %dx%d, llength = %d\n",
            rk_isp_pix_fmt_string(out_pix_fmt),
            width,
            height,
            llength);

    if (!RKISP_PIX_FMT_IS_YUV(in_pix_fmt))
    {
        ISP_ERR("unsupported format %s (must be YUV)\n",
                rk_isp_pix_fmt_string(in_pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    dev->config.mi_config.data_path.y_size = size;
    dev->config.mi_config.data_path.cb_size = 0;
    dev->config.mi_config.data_path.cr_size = 0;
    if (RKISP_PIX_FMT_IS_YUV(out_pix_fmt))
    {
        unsigned int num_cplanes =
            RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(out_pix_fmt);
        if (num_cplanes == 0)
        {
            writeformat = RK_ISP_BUFF_FMT_INTERLEAVED;

            if (out_pix_fmt == RK_YUV400)
                writeformat = RK_ISP_BUFF_FMT_PLANAR;

        }
        else
        {
            dev->config.mi_config.data_path.y_size =
                (dev->config.mi_config.data_path.y_size * 4)
                / (4 + (RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) *
                        RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) / 2));
            dev->config.mi_config.data_path.cb_size =
                size - dev->config.mi_config.data_path.y_size;
            if (num_cplanes == 1)
                writeformat = RK_ISP_BUFF_FMT_SEMIPLANAR;
            else if (num_cplanes == 2)
            {
                writeformat = RK_ISP_BUFF_FMT_PLANAR;
                dev->config.mi_config.data_path.cb_size /= 2;
            }
            /* for U<->V swapping: */
            dev->config.mi_config.data_path.cr_size =
                dev->config.mi_config.data_path.cb_size;
        }
        if (RKISP_PIX_FMT_YUV_IS_UV_SWAPPED(out_pix_fmt))
            swap_cb_cr = RK_MI_XTD_FMT_CTRL_SP_CB_CR_SWAP;

        if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 0) &&
                (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 0))
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_YUV400;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 2))
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_YUV420;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 4))
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_YUV422;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 4) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 4))
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_YUV444;
        else
        {
            ISP_ERR("unsupported YUV output format %s\n",
                    rk_isp_pix_fmt_string(out_pix_fmt));
            ret = -RT_EINVAL;
            goto err;
        }
    }
    else if (RKISP_PIX_FMT_IS_RGB(out_pix_fmt))
    {
        if (out_pix_fmt == RK_RGB565)
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_RGB565;
        else if (out_pix_fmt == RK_RGB666)
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_RGB666;
        else if (out_pix_fmt == RK_RGB888)
            output_format = RK_MI_CTRL_SP_OUTPUT_FMT_RGB888;
        else
        {
            ISP_ERR("unsupported RGB output format %s\n",
                    rk_isp_pix_fmt_string(out_pix_fmt));
            ret = -RT_EINVAL;
            goto err;
        }
    }
    else
    {
        ISP_ERR("unsupported output format %s\n",
                rk_isp_pix_fmt_string(out_pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 0) &&
            (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 0))
        input_format = RK_MI_CTRL_SP_INPUT_FMT_YUV400;
    else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 2) &&
             (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 2))
        input_format = RK_MI_CTRL_SP_INPUT_FMT_YUV420;
    else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 2) &&
             (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 4))
        input_format = RK_MI_CTRL_SP_INPUT_FMT_YUV422;
    else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(in_pix_fmt) == 4) &&
             (RKISP_PIX_FMT_YUV_GET_Y_SUBS(in_pix_fmt) == 4))
        input_format = RK_MI_CTRL_SP_INPUT_FMT_YUV444;
    else
    {
        ISP_ERR("unsupported YUV input format %s\n",
                rk_isp_pix_fmt_string(in_pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    if (writeformat == RK_ISP_BUFF_FMT_SEMIPLANAR)
    {
        dev->config.mi_config.data_path.cb_offs =
            dev->config.mi_config.data_path.y_size;
        dev->config.mi_config.data_path.cr_offs =
            dev->config.mi_config.data_path.cb_offs;
    }
    else if (writeformat == RK_ISP_BUFF_FMT_PLANAR)
    {
        if (swap_cb_cr)
        {
            swap_cb_cr = 0;
            dev->config.mi_config.data_path.cr_offs =
                dev->config.mi_config.data_path.y_size;
            dev->config.mi_config.data_path.cb_offs =
                dev->config.mi_config.data_path.cr_offs +
                dev->config.mi_config.data_path.cr_size;
        }
        else
        {
            dev->config.mi_config.data_path.cb_offs =
                dev->config.mi_config.data_path.y_size;
            dev->config.mi_config.data_path.cr_offs =
                dev->config.mi_config.data_path.cb_offs +
                dev->config.mi_config.data_path.cb_size;
        }
    }

    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.y_size,
                           RK_MI_SP_Y_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.y_size,
                           RK_MI_SP_Y_PIC_SIZE, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.cb_size,
                           RK_MI_SP_CB_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.cr_size,
                           RK_MI_SP_CR_SIZE_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
    WRITE_RKISP_REG_VERIFY(width,
                           RK_MI_SP_Y_PIC_WIDTH, ~0x3);
    WRITE_RKISP_REG_VERIFY(height,
                           RK_MI_SP_Y_PIC_HEIGHT, ~0x3);
    WRITE_RKISP_REG_VERIFY(llength,
                           RK_MI_SP_Y_LLENGTH, ~0x3);
    WRITE_RKISP_REG_OR_VERIFY(RK_MI_SP_FRAME, RK_MI_IMSC, ~0);

    if (swap_cb_cr)
    {
        WRITE_RKISP_REG_OR(swap_cb_cr, RK_MI_XTD_FORMAT_CTRL);
    }

    mi_ctrl = READ_RKISP_REG(RK_MI_CTRL) |
              RKMI_CTRL_SP_WRITE_FMT(writeformat) |
              input_format |
              output_format |
              RK_MI_CTRL_BURST_LEN_LUM_64 |
              RK_MI_CTRL_BURST_LEN_CHROM_64 |
              RK_MI_CTRL_INIT_BASE_EN |
              RK_MI_CTRL_INIT_OFFSET_EN |
              RK_MI_SP_AUTOUPDATE_ENABLE;
    WRITE_RKISP_REG_VERIFY(mi_ctrl, RK_MI_CTRL, ~0);

    ISP_DBG("\n  MI_CTRL 0x%08x\n"
            "  MI_STATUS 0x%08x\n"
            "  MI_SP_Y_SIZE %d\n"
            "  MI_SP_CB_SIZE %d\n"
            "  MI_SP_CR_SIZE %d\n"
            "  MI_SP_PIC_WIDTH %d\n"
            "  MI_SP_PIC_HEIGHT %d\n"
            "  MI_SP_PIC_LLENGTH %d\n"
            "  MI_SP_PIC_SIZE %d\n",
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_MI_STATUS),
            READ_RKISP_REG(RK_MI_SP_Y_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_CB_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_CR_SIZE_INIT),
            READ_RKISP_REG(RK_MI_SP_Y_PIC_WIDTH),
            READ_RKISP_REG(RK_MI_SP_Y_PIC_HEIGHT),
            READ_RKISP_REG(RK_MI_SP_Y_LLENGTH),
            READ_RKISP_REG(RK_MI_SP_Y_PIC_SIZE));

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_mi_dma(
    struct rk_isp_device *dev)
{
    int ret = 0;
    enum rk_isp_pix_fmt out_pix_fmt =
        dev->config.mi_config.data_path.output.pix_fmt;
    unsigned int llength =
        dev->config.mi_config.data_path.llength;
    unsigned int width =
        dev->config.mi_config.data_path.output.width;
    unsigned int height =
        dev->config.mi_config.data_path.output.height;
    unsigned int readformat = RK_ISP_BUFF_FMT_PLANAR;
    unsigned int bpp = RKISP_PIX_FMT_GET_BPP(out_pix_fmt);
    unsigned int size = llength * height * bpp / 8;
    unsigned int output_format;
    unsigned int mi_ctrl;

    ISP_DBG("%s %dx%d, llength = %d\n",
            rk_isp_pix_fmt_string(out_pix_fmt),
            width,
            height,
            llength);

    dev->config.mi_config.data_path.y_size = size;
    dev->config.mi_config.data_path.cb_size = 0;
    dev->config.mi_config.data_path.cr_size = 0;
    if (RKISP_PIX_FMT_IS_YUV(out_pix_fmt))
    {
        unsigned int num_cplanes =
            RKISP_PIX_FMT_YUV_GET_NUM_CPLANES(out_pix_fmt);
        if (num_cplanes == 0)
        {
            readformat = RK_ISP_BUFF_FMT_INTERLEAVED;
        }
        else
        {
            dev->config.mi_config.data_path.y_size =
                (dev->config.mi_config.data_path.y_size * 4)
                / (4 + (RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) *
                        RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) / 2));
            dev->config.mi_config.data_path.cb_size =
                size - dev->config.mi_config.data_path.y_size;
            if (num_cplanes == 1)
                readformat = RK_ISP_BUFF_FMT_SEMIPLANAR;
            else if (num_cplanes == 2)
            {
                readformat = RK_ISP_BUFF_FMT_PLANAR;
                dev->config.mi_config.data_path.cb_size /= 2;
            }
            /* for U<->V swapping: */
            dev->config.mi_config.data_path.cr_size =
                dev->config.mi_config.data_path.cb_size;
        }

        if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 0) &&
                (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 0))
            output_format = RK_MI_DMA_CTRL_FMT_YUV400;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 2))
            output_format = RK_MI_DMA_CTRL_FMT_YUV420;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 2) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 4))
            output_format = RK_MI_DMA_CTRL_FMT_YUV422;
        else if ((RKISP_PIX_FMT_YUV_GET_X_SUBS(out_pix_fmt) == 4) &&
                 (RKISP_PIX_FMT_YUV_GET_Y_SUBS(out_pix_fmt) == 4))
            output_format = RK_MI_DMA_CTRL_FMT_YUV444;
        else
        {
            ISP_ERR("unsupported YUV output format %s\n",
                    rk_isp_pix_fmt_string(out_pix_fmt));
            ret = -RT_EINVAL;
            goto err;
        }
    }
    else
    {
        ISP_ERR("unsupported output format %s\n",
                rk_isp_pix_fmt_string(out_pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    if (readformat == RK_ISP_BUFF_FMT_SEMIPLANAR)
    {
        dev->config.mi_config.data_path.cb_offs =
            dev->config.mi_config.data_path.y_size;
        dev->config.mi_config.data_path.cr_offs =
            dev->config.mi_config.data_path.cb_offs;
    }
    else if (readformat == RK_ISP_BUFF_FMT_PLANAR)
    {
        dev->config.mi_config.data_path.cb_offs =
            dev->config.mi_config.data_path.y_size;
        dev->config.mi_config.data_path.cr_offs =
            dev->config.mi_config.data_path.cb_offs +
            dev->config.mi_config.data_path.cb_size;
    }

    WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.y_size,
                           RK_MI_DMA_Y_PIC_SIZE, ~0x3);
    WRITE_RKISP_REG_VERIFY(width,
                           RK_MI_DMA_Y_PIC_WIDTH, ~0x3);
    WRITE_RKISP_REG_VERIFY(llength,
                           RK_MI_DMA_Y_LLENGTH, ~0x3);

    mi_ctrl = READ_RKISP_REG(RK_MI_DMA_CTRL) |
              RKMI_DMA_CTRL_READ_FMT(readformat) |
              output_format |
              RK_MI_DMA_CTRL_BURST_LEN_LUM_64 |
              RK_MI_DMA_CTRL_BURST_LEN_CHROM_64;
    WRITE_RKISP_REG_VERIFY(mi_ctrl, RK_MI_DMA_CTRL, ~0);

    WRITE_RKISP_REG_OR_VERIFY(RK_MI_DMA_READY, RK_MI_IMSC, ~0);

    ISP_DBG("\n  MI_DMA_CTRL 0x%08x\n"
            "  MI_DMA_STATUS 0x%08x\n"
            "  MI_DMA_Y_PIC_WIDTH %d\n"
            "  MI_DMA_Y_LLENGTH %d\n"
            "  MI_DMA_Y_PIC_SIZE %d\n"
            "  MI_DMA_Y_PIC_START_AD %d\n"
            "  MI_DMA_CB_PIC_START_AD %d\n"
            "  MI_DMA_CR_PIC_START_AD %d\n",
            READ_RKISP_REG(RK_MI_DMA_CTRL),
            READ_RKISP_REG(RK_MI_DMA_STATUS),
            READ_RKISP_REG(RK_MI_DMA_Y_PIC_WIDTH),
            READ_RKISP_REG(RK_MI_DMA_Y_LLENGTH),
            READ_RKISP_REG(RK_MI_DMA_Y_PIC_SIZE),
            READ_RKISP_REG(RK_MI_DMA_Y_PIC_START_AD),
            READ_RKISP_REG(RK_MI_DMA_CB_PIC_START_AD),
            READ_RKISP_REG(RK_MI_DMA_CR_PIC_START_AD));

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_path(
    struct rk_isp_device *dev,
    unsigned int stream_ids)
{
    unsigned int dpcl = 0;

    ISP_DBG("\n");

    /* if_sel */
    if (dev->config.input_sel == RK_ISP_INP_DMA)
        dpcl |= RK_VI_DPCL_DMA_SW_ISP;
    else if (dev->config.input_sel == RK_ISP_INP_DMA_IE)
        dpcl |= RK_VI_DPCL_DMA_IE_MUX_DMA |
                RK_VI_DPCL_DMA_SW_IE;
    else if (dev->config.input_sel == RK_ISP_INP_DMA_SP)
        dpcl |= RK_VI_DPCL_DMA_SP_MUX_DMA;
    else
    {
        if (PLTFRM_CAM_ITF_IS_DVP(dev->config.cam_itf.type))
        {
            dpcl |= RK_VI_DPCL_IF_SEL_PARALLEL;
        }
        else if (PLTFRM_CAM_ITF_IS_MIPI(dev->config.cam_itf.type))
        {
            dpcl |= RK_VI_DPCL_IF_SEL_MIPI;
        }
        else
        {
            ISP_ERR("Sensor Interface: 0x%x isn't support \n",
                    dev->config.cam_itf.type);
            return -RT_EINVAL;
        }
    }

    /* chan_mode */
    if (stream_ids & RK_ISP_STREAM_SP)
    {
        dpcl |= RK_VI_DPCL_CHAN_MODE_SP;
    }
    if (stream_ids & RK_ISP_STREAM_Y12)
    {
        /* do nothing */
    }

    if ((stream_ids & RK_ISP_STREAM_MP) &&
            !(dev->config.input_sel == RK_ISP_INP_DMA_SP))
    {
        dpcl |= RK_VI_DPCL_CHAN_MODE_MP;
        /* mp_dmux */
        dpcl |= RK_VI_DPCL_MP_MUX_MRSZ_MI;
    }

    WRITE_RKISP_REG(dpcl, RK_VI_DPCL);

    ISP_DBG("RK_DPCL 0x%08x\n", dpcl);

    return 0;
}

static int rt_do_config_rkisp_dcrop(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id,
    bool async)
{
    unsigned int dc_ctrl = READ_RKISP_REG(RK_DUAL_CROP_CTRL);

    if (stream_id == RK_ISP_STREAM_MP)
    {
        WRITE_RKISP_REG(0, RK_DUAL_CROP_M_H_OFFS);
        WRITE_RKISP_REG(0, RK_DUAL_CROP_M_V_OFFS);
        WRITE_RKISP_REG(0, RK_DUAL_CROP_M_H_SIZE);
        WRITE_RKISP_REG(0, RK_DUAL_CROP_M_V_SIZE);

        dc_ctrl |= RK_DUAL_CROP_MP_MODE_BYPASS;
        if (async)
            dc_ctrl |= RK_DUAL_CROP_GEN_CFG_UPD;
        else
            dc_ctrl |= RK_DUAL_CROP_CFG_UPD;

        WRITE_RKISP_REG(dc_ctrl, RK_DUAL_CROP_CTRL);
    }
    else if (stream_id == RK_ISP_STREAM_SP)
    {
        WRITE_RKISP_REG(0, RK_DUAL_CROP_S_H_OFFS);
        WRITE_RKISP_REG(0, RK_DUAL_CROP_S_V_OFFS);
        WRITE_RKISP_REG(0, RK_DUAL_CROP_S_H_SIZE);
        WRITE_RKISP_REG(0, RK_DUAL_CROP_S_V_SIZE);

        dc_ctrl |= RK_DUAL_CROP_MP_MODE_BYPASS;
        if (async)
            dc_ctrl |= RK_DUAL_CROP_GEN_CFG_UPD;
        else
            dc_ctrl |= RK_DUAL_CROP_CFG_UPD;

        WRITE_RKISP_REG(dc_ctrl, RK_DUAL_CROP_CTRL);
    }

    return 0;

}

static int rt_do_config_rkisp_rsz(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id,
    bool async)
{
    int ret;
    unsigned int i;
    unsigned int scale_h_y_addr;
    unsigned int scale_h_cr_addr;
    unsigned int scale_h_cb_addr;
    unsigned int scale_v_y_addr;
    unsigned int scale_v_c_addr;
    unsigned int rsz_ctrl_addr;
    struct rk_isp_frm_fmt *rsz_input;
    struct rk_isp_frm_fmt *rsz_output;
    struct rk_isp_frm_fmt *mi_output;
    unsigned int rsz_ctrl;
    unsigned int input_width_y;
    unsigned int output_width_y;
    unsigned int input_height_y;
    unsigned int output_height_y;
    unsigned int input_width_c;
    unsigned int output_width_c;
    unsigned int input_height_c;
    unsigned int output_height_c;
    unsigned int scale_h_c;

    if (stream_id == RK_ISP_STREAM_MP)
    {
        rsz_ctrl_addr = RK_MRSZ_CTRL;
        scale_h_y_addr = RK_MRSZ_SCALE_HY;
        scale_v_y_addr = RK_MRSZ_SCALE_VY;
        scale_h_cb_addr = RK_MRSZ_SCALE_HCB;
        scale_h_cr_addr = RK_MRSZ_SCALE_HCR;
        scale_v_c_addr = RK_MRSZ_SCALE_VC;
        dev->config.mp_config.rsz_config.input =
            &dev->config.isp_config.output;
        rsz_input = dev->config.mp_config.rsz_config.input;
        rsz_output = &dev->config.mp_config.rsz_config.output;
        mi_output = &dev->config.mi_config.data_path.output;
        /* No phase offset */
        WRITE_RKISP_REG(0, RK_MRSZ_PHASE_HY);
        WRITE_RKISP_REG(0, RK_MRSZ_PHASE_HC);
        WRITE_RKISP_REG(0, RK_MRSZ_PHASE_VY);
        WRITE_RKISP_REG(0, RK_MRSZ_PHASE_VC);
        /* Linear interpolation */
        for (i = 0; i < 64; i++)
        {
            WRITE_RKISP_REG(i, RK_MRSZ_SCALE_LUT_ADDR);
            WRITE_RKISP_REG(i, RK_MRSZ_SCALE_LUT);
        }
    }
    else
    {
        rsz_ctrl_addr = RK_SRSZ_CTRL;
        scale_h_y_addr = RK_SRSZ_SCALE_HY;
        scale_v_y_addr = RK_SRSZ_SCALE_VY;
        scale_h_cb_addr = RK_SRSZ_SCALE_HCB;
        scale_h_cr_addr = RK_SRSZ_SCALE_HCR;
        scale_v_c_addr = RK_SRSZ_SCALE_VC;
        if (dev->config.input_sel == RK_ISP_INP_DMA_SP)
            dev->config.sp_config.rsz_config.input =
                &dev->config.mi_config.data_path.output;
        else
            dev->config.sp_config.rsz_config.input =
                &dev->config.isp_config.output;

        rsz_input = dev->config.sp_config.rsz_config.input;
        rsz_output = &dev->config.sp_config.rsz_config.output;
        mi_output = &dev->config.mi_config.data_path.output;
        /* No phase offset */
        WRITE_RKISP_REG(0, RK_SRSZ_PHASE_HY);
        WRITE_RKISP_REG(0, RK_SRSZ_PHASE_HC);
        WRITE_RKISP_REG(0, RK_SRSZ_PHASE_VY);
        WRITE_RKISP_REG(0, RK_SRSZ_PHASE_VC);
        /* Linear interpolation */
        for (i = 0; i < 64; i++)
        {
            WRITE_RKISP_REG(i, RK_SRSZ_SCALE_LUT_ADDR);
            WRITE_RKISP_REG(i, RK_SRSZ_SCALE_LUT);
        }
    }

    /* set RSZ input and output */
    rsz_output->width = mi_output->width;
    rsz_output->height = mi_output->height;
    rsz_output->pix_fmt = rsz_input->pix_fmt;
    if (RKISP_PIX_FMT_IS_YUV(mi_output->pix_fmt))
    {
        RKISP_PIX_FMT_YUV_SET_Y_SUBS(
            rsz_output->pix_fmt,
            RKISP_PIX_FMT_YUV_GET_Y_SUBS(mi_output->pix_fmt));
        RKISP_PIX_FMT_YUV_SET_X_SUBS(
            rsz_output->pix_fmt,
            RKISP_PIX_FMT_YUV_GET_X_SUBS(mi_output->pix_fmt));
        RKISP_PIX_FMT_SET_BPP(
            rsz_output->pix_fmt,
            RKISP_PIX_FMT_GET_BPP(mi_output->pix_fmt));
    }
    else if (RKISP_PIX_FMT_IS_JPEG(mi_output->pix_fmt))
    {
        RKISP_PIX_FMT_YUV_SET_Y_SUBS(
            rsz_output->pix_fmt, 4);
        RKISP_PIX_FMT_YUV_SET_X_SUBS(
            rsz_output->pix_fmt, 2);
        RKISP_PIX_FMT_SET_BPP(
            rsz_output->pix_fmt, 16);
    }

    ISP_DBG("%s %s %dx%d -> %s %dx%d\n",
            rk_isp_stream_id_string(stream_id),
            rk_isp_pix_fmt_string(rsz_input->pix_fmt),
            rsz_input->width,
            rsz_input->height,
            rk_isp_pix_fmt_string(rsz_output->pix_fmt),
            rsz_output->width,
            rsz_output->height);

    /* set input and output sizes for scale calculation */
    input_width_y = rsz_input->width;
    output_width_y = rsz_output->width;
    input_height_y = rsz_input->height;
    output_height_y = rsz_output->height;
    input_width_c = input_width_y;
    output_width_c = output_width_y;
    input_height_c = input_height_y;
    output_height_c = output_height_y;

    if (RKISP_PIX_FMT_IS_YUV(rsz_output->pix_fmt))
    {
        input_width_c = (input_width_c *
                         RKISP_PIX_FMT_YUV_GET_X_SUBS(
                             rsz_input->pix_fmt)) / 4;
        input_height_c = (input_height_c *
                          RKISP_PIX_FMT_YUV_GET_Y_SUBS(
                              rsz_input->pix_fmt)) / 4;
        output_width_c = (output_width_c *
                          RKISP_PIX_FMT_YUV_GET_X_SUBS(
                              rsz_output->pix_fmt)) / 4;
        output_height_c = (output_height_c *
                           RKISP_PIX_FMT_YUV_GET_Y_SUBS(
                               rsz_output->pix_fmt)) / 4;

        ISP_DBG("chroma scaling %dx%d -> %dx%d\n",
                input_width_c, input_height_c,
                output_width_c, output_height_c);

        if (((input_width_c == 0) && (output_width_c > 0)) ||
                ((input_height_c == 0) && (output_height_c > 0)))
        {
            ISP_ERR("input is black and white, cannot output colour\n");
            ret = -RT_EINVAL;
            goto err;
        }
    }
    else
    {
        if ((input_width_y != output_width_y) ||
                (input_height_y != output_height_y))
        {
            ISP_ERR("%dx%d -> %dx%d isn't support,"
                    "can only scale YUV input\n",
                    input_width_y, input_height_y,
                    output_width_y, output_height_y);
            ret = -RT_EINVAL;
            goto err;
        }
    }

    /* calculate and set scale */
    rsz_ctrl = 0;
    if (input_width_y < output_width_y)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_HY_ENABLE |
                    RK_RSZ_CTRL_SCALE_HY_UP;
        WRITE_RKISP_REG(
            DIV_TRUNCATE((input_width_y - 1) * RK_RSZ_SCALER_BYPASS,
                         output_width_y - 1),
            scale_h_y_addr);
    }
    else if (input_width_y > output_width_y)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_HY_ENABLE;
        WRITE_RKISP_REG(
            DIV_TRUNCATE((output_width_y - 1) * RK_RSZ_SCALER_BYPASS,
                         input_width_y - 1) + 1,
            scale_h_y_addr);
    }
    if (input_width_c < output_width_c)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_HC_ENABLE |
                    RK_RSZ_CTRL_SCALE_HC_UP;
        scale_h_c = DIV_TRUNCATE((input_width_c - 1) * RK_RSZ_SCALER_BYPASS,
                                 output_width_c - 1);
        WRITE_RKISP_REG(scale_h_c, scale_h_cb_addr);
        WRITE_RKISP_REG(scale_h_c, scale_h_cr_addr);
    }
    else if (input_width_c > output_width_c)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_HC_ENABLE;
        scale_h_c = DIV_TRUNCATE((output_width_c - 1) * RK_RSZ_SCALER_BYPASS,
                                 input_width_c - 1) + 1;
        WRITE_RKISP_REG(scale_h_c, scale_h_cb_addr);
        WRITE_RKISP_REG(scale_h_c, scale_h_cr_addr);
    }

    if (input_height_y < output_height_y)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_VY_ENABLE |
                    RK_RSZ_CTRL_SCALE_VY_UP;
        WRITE_RKISP_REG(
            DIV_TRUNCATE((input_height_y - 1) * RK_RSZ_SCALER_BYPASS,
                         output_height_y - 1),
            scale_v_y_addr);
    }
    else if (input_height_y > output_height_y)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_VY_ENABLE;
        WRITE_RKISP_REG(
            DIV_TRUNCATE((output_height_y - 1) * RK_RSZ_SCALER_BYPASS,
                         input_height_y - 1) + 1,
            scale_v_y_addr);
    }

    if (input_height_c < output_height_c)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_VC_ENABLE |
                    RK_RSZ_CTRL_SCALE_VC_UP;
        WRITE_RKISP_REG(
            DIV_TRUNCATE((input_height_c - 1) * RK_RSZ_SCALER_BYPASS,
                         output_height_c - 1),
            scale_v_c_addr);
    }
    else if (input_height_c > output_height_c)
    {
        rsz_ctrl |= RK_RSZ_CTRL_SCALE_VC_ENABLE;
        WRITE_RKISP_REG(
            DIV_TRUNCATE((output_height_c - 1) * RK_RSZ_SCALER_BYPASS,
                         input_height_c - 1) + 1,
            scale_v_c_addr);
    }

#ifdef FPGA_TEST
    rsz_ctrl = 1 << 0
               | 1 << 1
               | 1 << 2
               | 1 << 3
               | 0x3 << 8;
    WRITE_RKISP_REG(rsz_ctrl, rsz_ctrl_addr);
#else
    WRITE_RKISP_REG(rsz_ctrl, rsz_ctrl_addr);
#endif

    if (stream_id == RK_ISP_STREAM_MP)
    {
        if (async)
            WRITE_RKISP_REG_OR(RK_RSZ_CTRL_CFG_UPD, RK_MRSZ_CTRL);
        dev->config.mp_config.rsz_config.ycflt_adjust = false;
        dev->config.mp_config.rsz_config.ism_adjust = false;
        ISP_DBG("\n  MRSZ_CTRL 0x%08x/0x%08x\n"
                "  MRSZ_SCALE_HY %d/%d\n"
                "  MRSZ_SCALE_HCB %d/%d\n"
                "  MRSZ_SCALE_HCR %d/%d\n"
                "  MRSZ_SCALE_VY %d/%d\n"
                "  MRSZ_SCALE_VC %d/%d\n"
                "  MRSZ_PHASE_HY %d/%d\n"
                "  MRSZ_PHASE_HC %d/%d\n"
                "  MRSZ_PHASE_VY %d/%d\n"
                "  MRSZ_PHASE_VC %d/%d\n",
                READ_RKISP_REG(RK_MRSZ_CTRL),
                READ_RKISP_REG(RK_MRSZ_CTRL_SHD),
                READ_RKISP_REG(RK_MRSZ_SCALE_HY),
                READ_RKISP_REG(RK_MRSZ_SCALE_HY_SHD),
                READ_RKISP_REG(RK_MRSZ_SCALE_HCB),
                READ_RKISP_REG(RK_MRSZ_SCALE_HCB_SHD),
                READ_RKISP_REG(RK_MRSZ_SCALE_HCR),
                READ_RKISP_REG(RK_MRSZ_SCALE_HCR_SHD),
                READ_RKISP_REG(RK_MRSZ_SCALE_VY),
                READ_RKISP_REG(RK_MRSZ_SCALE_VY_SHD),
                READ_RKISP_REG(RK_MRSZ_SCALE_VC),
                READ_RKISP_REG(RK_MRSZ_SCALE_VC_SHD),
                READ_RKISP_REG(RK_MRSZ_PHASE_HY),
                READ_RKISP_REG(RK_MRSZ_PHASE_HY_SHD),
                READ_RKISP_REG(RK_MRSZ_PHASE_HC),
                READ_RKISP_REG(RK_MRSZ_PHASE_HC_SHD),
                READ_RKISP_REG(RK_MRSZ_PHASE_VY),
                READ_RKISP_REG(RK_MRSZ_PHASE_VY_SHD),
                READ_RKISP_REG(RK_MRSZ_PHASE_VC),
                READ_RKISP_REG(RK_MRSZ_PHASE_VC_SHD));
    }
    else
    {
        if (async)
            WRITE_RKISP_REG_OR(RK_RSZ_CTRL_CFG_UPD, RK_SRSZ_CTRL);
        dev->config.sp_config.rsz_config.ycflt_adjust = false;
        dev->config.sp_config.rsz_config.ism_adjust = false;
        ISP_DBG("\n  SRSZ_CTRL 0x%08x/0x%08x\n"
                "  SRSZ_SCALE_HY %d/%d\n"
                "  SRSZ_SCALE_HCB %d/%d\n"
                "  SRSZ_SCALE_HCR %d/%d\n"
                "  SRSZ_SCALE_VY %d/%d\n"
                "  SRSZ_SCALE_VC %d/%d\n"
                "  SRSZ_PHASE_HY %d/%d\n"
                "  SRSZ_PHASE_HC %d/%d\n"
                "  SRSZ_PHASE_VY %d/%d\n"
                "  SRSZ_PHASE_VC %d/%d\n",
                READ_RKISP_REG(RK_SRSZ_CTRL),
                READ_RKISP_REG(RK_SRSZ_CTRL_SHD),
                READ_RKISP_REG(RK_SRSZ_SCALE_HY),
                READ_RKISP_REG(RK_SRSZ_SCALE_HY_SHD),
                READ_RKISP_REG(RK_SRSZ_SCALE_HCB),
                READ_RKISP_REG(RK_SRSZ_SCALE_HCB_SHD),
                READ_RKISP_REG(RK_SRSZ_SCALE_HCR),
                READ_RKISP_REG(RK_SRSZ_SCALE_HCR_SHD),
                READ_RKISP_REG(RK_SRSZ_SCALE_VY),
                READ_RKISP_REG(RK_SRSZ_SCALE_VY_SHD),
                READ_RKISP_REG(RK_SRSZ_SCALE_VC),
                READ_RKISP_REG(RK_SRSZ_SCALE_VC_SHD),
                READ_RKISP_REG(RK_SRSZ_PHASE_HY),
                READ_RKISP_REG(RK_SRSZ_PHASE_HY_SHD),
                READ_RKISP_REG(RK_SRSZ_PHASE_HC),
                READ_RKISP_REG(RK_SRSZ_PHASE_HC_SHD),
                READ_RKISP_REG(RK_SRSZ_PHASE_VY),
                READ_RKISP_REG(RK_SRSZ_PHASE_VY_SHD),
                READ_RKISP_REG(RK_SRSZ_PHASE_VC),
                READ_RKISP_REG(RK_SRSZ_PHASE_VC_SHD));
    }

    return 0;
err:
    ISP_ERR("failed with err %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_sp(
    struct rk_isp_device *dev)
{
    int ret = 0;

    ret = rt_do_config_rkisp_rsz(dev, RK_ISP_STREAM_SP, true);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret = rt_do_config_rkisp_dcrop(dev, RK_ISP_STREAM_SP, true);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret = rt_do_config_rkisp_mi_sp(dev);
    if (IS_ERR_VALUE(ret))
        goto err;

    g_data_stream.updt_cfg = false;

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_mp(
    struct rk_isp_device *dev)
{
    int ret = 0;

    ret = rt_do_config_rkisp_rsz(dev, RK_ISP_STREAM_MP, true);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret = rt_do_config_rkisp_dcrop(dev, RK_ISP_STREAM_MP, true);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret = rt_do_config_rkisp_mi_mp(dev);
    if (IS_ERR_VALUE(ret))
        goto err;

    g_data_stream.updt_cfg = false;

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_config_rkisp_y12(
    struct rk_isp_device *dev)
{
    int ret = 0;

    ret = rt_do_config_rkisp_mi_y12(dev);
    if (IS_ERR_VALUE(ret))
        goto err;

    g_data_stream.updt_cfg = false;

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static void rt_do_config_rkisp_clk(
    struct rk_isp_device *dev)
{
    WRITE_RKISP_REG(RK_CCL_RK_CLK_ENA, RK_CCL);
    //WRITE_RKISP_REG(0x000094B, RK_ICCL);

    ISP_DBG("\n  RK_CCL 0x%08x\n"
            "  RK_ICCL 0x%08x\n",
            READ_RKISP_REG(RK_CCL),
            READ_RKISP_REG(RK_ICCL));
}

static int rt_do_config_rkisp_input(
    struct rk_isp_device *dev,
    unsigned int stream_ids)
{
    int ret = 0;
    unsigned int RK_id;

    ISP_DBG("start configuring RK...\n");

    if ((stream_ids & RK_ISP_STREAM_MP) ||
            (stream_ids & RK_ISP_STREAM_SP) ||
            (stream_ids & RK_ISP_STREAM_Y12))
    {
        if (!RKISP_INP_IS_DMA(dev->config.input_sel))
        {
            /* configure sensor */
            ret = rt_do_config_rkisp_subdev(dev);
            if (IS_ERR_VALUE(ret))
                goto err;
        }

        RK_id = READ_RKISP_REG(RK_VI_ID);
        dev->config.out_of_buffer_stall =
            RK_ISP_ALWAYS_STALL_ON_NO_BUFS;

        ISP_DBG("RK_ID 0x%08x\n", RK_id);
        WRITE_RKISP_REG(RK_IRCL_RK_SW_RST, RK_IRCL);

        rt_do_config_rkisp_clk(dev);

        rt_do_rkisp_frame_id_reset(&dev->isp_dev);
        dev->isp_dev.frame_id_setexp = 0;

        /* Decide when to switch to asynchronous mode */
        /* TODO: remove dev->isp_dev.ycflt_en check for
            HW with the scaler fix. */
        dev->config.mi_config.async_updt = RK_ISP_ALWAYS_ASYNC;
        if (RKISP_INP_IS_DMA(dev->config.input_sel))
        {
            dev->config.mi_config.async_updt |= RK_ISP_ASYNC_DMA;
            ret = rt_do_config_rkisp_mi_dma(dev);
            if (IS_ERR_VALUE(ret))
                goto err;
        }

        if (dev->config.isp_config.ism_config.ism_en)
            dev->config.mi_config.async_updt |=
                RK_ISP_ASYNC_ISM;

        if (PLTFRM_CAM_ITF_IS_MIPI(dev->config.cam_itf.type))
        {
            ret = rt_do_config_rkisp_mipi(dev);
            if (IS_ERR_VALUE(ret))
                goto err;
        }

        ret = rt_do_config_rkisp_isp(dev);
        if (IS_ERR_VALUE(ret))
            goto err;

        rt_do_config_rkisp_ism(dev, true);

        dev->config.isp_config.ism_config.ism_update_needed = false;
        if (stream_ids & RK_ISP_STREAM_SP)
            dev->config.sp_config.rsz_config.ism_adjust = true;
        if (stream_ids & RK_ISP_STREAM_MP)
            dev->config.mp_config.rsz_config.ism_adjust = true;

        if (stream_ids & RK_ISP_STREAM_SP)
        {
            ret = rt_do_config_rkisp_sp(dev);
            if (IS_ERR_VALUE(ret))
                goto err;
        }

        if (stream_ids & RK_ISP_STREAM_MP)
        {
            ret = rt_do_config_rkisp_mp(dev);
            if (IS_ERR_VALUE(ret))
                goto err;
        }

        if (stream_ids & RK_ISP_STREAM_Y12)
        {
            ret = rt_do_config_rkisp_y12(dev);
            if (IS_ERR_VALUE(ret))
                goto err;
        }

        ret = rt_do_config_rkisp_path(dev, stream_ids);
        if (IS_ERR_VALUE(ret))
            ISP_ERR("set stream path failed\n");
    }

    /* Turn off XNR vertical subsampling when ism cropping is enabled */
    if (dev->config.isp_config.ism_config.ism_en)
    {
        if (dev->isp_dev.rk_ism_cropping == false)
        {
            dev->isp_dev.rk_ism_cropping = true;
        }
    }
    else
    {
        if (dev->isp_dev.rk_ism_cropping == true)
        {
            dev->isp_dev.rk_ism_cropping = false;
        }
    }

    if (dev->config.sp_config.rsz_config.ycflt_adjust ||
            dev->config.sp_config.rsz_config.ism_adjust)
    {
        if (g_data_stream.state == RK_ISP_STATE_READY)
        {
            ret = rt_do_config_rkisp_rsz(dev,
                                         RK_ISP_STREAM_SP, true);
            if (IS_ERR_VALUE(ret))
                goto err;
        }
        else
        {
            /* Disable SRSZ if SP is not used */
            WRITE_RKISP_REG(0, RK_SRSZ_CTRL);
            WRITE_RKISP_REG_OR(RK_RSZ_CTRL_CFG_UPD, RK_SRSZ_CTRL);
            dev->config.sp_config.rsz_config.ycflt_adjust = false;
            dev->config.sp_config.rsz_config.ism_adjust = false;
        }
    }

    if (dev->config.mp_config.rsz_config.ycflt_adjust ||
            dev->config.mp_config.rsz_config.ism_adjust)
    {
        if (g_data_stream.state == RK_ISP_STATE_READY)
        {
            ret = rt_do_config_rkisp_rsz(dev, RK_ISP_STREAM_MP, true);
            if (IS_ERR_VALUE(ret))
                goto err;
        }
        else
        {
            /* Disable MRSZ if MP is not used */
            WRITE_RKISP_REG(0, RK_MRSZ_CTRL);
            WRITE_RKISP_REG_OR(RK_RSZ_CTRL_CFG_UPD, RK_MRSZ_CTRL);
            dev->config.mp_config.rsz_config.ycflt_adjust = false;
            dev->config.mp_config.rsz_config.ism_adjust = false;
        }
    }

    if (dev->config.mi_config.async_updt)
        ISP_DBG("RK in asynchronous mode (0x%08x)\n",
                dev->config.mi_config.async_updt);
    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static void rt_do_init_rkisp_stream(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id)
{
    struct rk_isp_stream *stream;

    switch (stream_id)
    {
    case RK_ISP_STREAM_SP:
        stream = &g_data_stream;
        dev->config.sp_config.rsz_config.ycflt_adjust = false;
        dev->config.sp_config.rsz_config.ism_adjust = false;
        dev->config.mi_config.data_path.busy = false;
        break;
    case RK_ISP_STREAM_MP:
        stream = &g_data_stream;
        dev->config.mi_config.raw_enable = false;
        dev->config.mp_config.rsz_config.ycflt_adjust = false;
        dev->config.mp_config.rsz_config.ism_adjust = false;
        dev->config.mi_config.data_path.busy = false;
        break;
    case RK_ISP_STREAM_Y12:
        stream = &g_data_stream;
        dev->config.mi_config.raw_enable = false;
        dev->config.mi_config.data_path.busy = false;
        break;
    case RK_ISP_STREAM_DMA:
        stream = &g_data_stream;
        dev->config.mi_config.data_path.busy = false;
        break;
    default:
        ISP_ERR("unknown/unsupported stream ID %d\n", stream_id);
        break;
    }
    stream->updt_cfg = false;
    stream->stop = false;
    stream->stall = false;

    stream->state = RK_ISP_STATE_INACTIVE;
}


static void rt_do_update_rkisp_mi_buff_addr(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id strm_id)
{
    if (strm_id == RK_ISP_STREAM_SP)
    {
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr,
                               RK_MI_SP_Y_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);

        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                               dev->config.mi_config.data_path.cb_offs,
                               RK_MI_SP_CB_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                               dev->config.mi_config.data_path.cr_offs,
                               RK_MI_SP_CR_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        /* There have bee repeatedly issues with
            the offset registers, it is safer to write
            them each time, even though it is always
            0 and even though that is the
            register's default value */
        WRITE_RKISP_REG_VERIFY(0, RK_MI_SP_Y_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(0, RK_MI_SP_CB_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(0, RK_MI_SP_CR_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        ISP_DBG("\n  MI_SP_Y_BASE_AD 0x%08x/0x%08x\n"
                "  MI_SP_CB_BASE_AD 0x%08x/0x%08x\n"
                "  MI_SP_CR_BASE_AD 0x%08x/0x%08x\n",
                READ_RKISP_REG(RK_MI_SP_Y_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_SP_Y_BASE_AD_SHD),
                READ_RKISP_REG(RK_MI_SP_CB_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_SP_CB_BASE_AD_SHD),
                READ_RKISP_REG(RK_MI_SP_CR_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_SP_CR_BASE_AD_SHD));
    }
    else if (strm_id == RK_ISP_STREAM_MP)
    {
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr,
                               RK_MI_MP_Y_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                               dev->config.mi_config.data_path.cb_offs,
                               RK_MI_MP_CB_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                               dev->config.mi_config.data_path.cr_offs,
                               RK_MI_MP_CR_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        /* There have bee repeatedly issues with
            the offset registers, it is safer to write
            them each time, even though it is always
            0 and even though that is the
            register's default value */
        WRITE_RKISP_REG_VERIFY(0,
                               RK_MI_MP_Y_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(0,
                               RK_MI_MP_CB_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(0,
                               RK_MI_MP_CR_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        ISP_DBG("\n  MI_MP_Y_BASE_AD 0x%08x/0x%08x\n"
                "  MI_MP_CB_BASE_AD 0x%08x/0x%08x\n"
                "  MI_MP_CR_BASE_AD 0x%08x/0x%08x\n",
                READ_RKISP_REG(RK_MI_MP_Y_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_MP_Y_BASE_AD_SHD),
                READ_RKISP_REG(RK_MI_MP_CB_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_MP_CB_BASE_AD_SHD),
                READ_RKISP_REG(RK_MI_MP_CR_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_MP_CR_BASE_AD_SHD));
    }
    else if (strm_id == RK_ISP_STREAM_Y12)
    {
        if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12)
        {
            WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr,
                                   RK_MI_MP_CR_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        }
        else
        {
            WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr,
                                   RK_MI_MP_CR_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
            WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                                   dev->config.mi_config.data_path.cb_offs,
                                   RK_MI_SP_CR_BASE_AD_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        }
        /* There have bee repeatedly issues with
            the offset registers, it is safer to write
            them each time, even though it is always
            0 and even though that is the
            register's default value */
        if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12)
        {
            WRITE_RKISP_REG_VERIFY(0, RK_MI_MP_CR_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        }
        else
        {
            WRITE_RKISP_REG_VERIFY(0, RK_MI_MP_CR_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
            WRITE_RKISP_REG_VERIFY(0, RK_MI_SP_CR_OFFS_CNT_INIT, RK_MI_ADDR_SIZE_ALIGN_MASK);
        }
        ISP_DBG("\n  MI_MP_CR_BASE_AD 0x%08x/0x%08x\n"
                "  MI_SP_CR_BASE_AD 0x%08x/0x%08x\n",
                READ_RKISP_REG(RK_MI_MP_CR_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_MP_CR_BASE_AD_SHD),
                READ_RKISP_REG(RK_MI_SP_CR_BASE_AD_INIT),
                READ_RKISP_REG(RK_MI_SP_CR_BASE_AD_SHD));
    }
    else     /* DMA */
    {
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr,
                               RK_MI_DMA_Y_PIC_START_AD, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                               dev->config.mi_config.data_path.cb_offs,
                               RK_MI_DMA_CB_PIC_START_AD, RK_MI_ADDR_SIZE_ALIGN_MASK);
        WRITE_RKISP_REG_VERIFY(dev->config.mi_config.data_path.next_buff_addr +
                               dev->config.mi_config.data_path.cr_offs,
                               RK_MI_DMA_CR_PIC_START_AD, RK_MI_ADDR_SIZE_ALIGN_MASK);
        ISP_DBG("\n  MI_DMA_Y_PIC_START_AD 0x%08x\n"
                "  MI_DMA_CB_PIC_START_AD 0x%08x\n"
                "  MI_DMA_CR_PIC_START_AD 0x%08x\n",
                READ_RKISP_REG(RK_MI_DMA_Y_PIC_START_AD),
                READ_RKISP_REG(RK_MI_DMA_CB_PIC_START_AD),
                READ_RKISP_REG(RK_MI_DMA_CR_PIC_START_AD));
    }
}

static int rt_do_update_rkisp_mi_mp(
    struct rk_isp_device *dev)
{
    int ret = 0;
    enum rk_isp_pix_fmt out_pix_fmt =
        dev->config.mi_config.data_path.output.pix_fmt;

    ISP_DBG("%s curr 0x%08x next 0x%08x\n", __FUNCTION__,
            dev->config.mi_config.data_path.curr_buff_addr,
            dev->config.mi_config.data_path.next_buff_addr);

    if (dev->b_set_out_addr && dev->b_set_uv_flag)
    {
        dev->config.mi_config.data_path.cb_offs =
            dev->frame_buffer[dev->buf_index].addr[1] - dev->frame_buffer[dev->buf_index].addr[0];
        dev->config.mi_config.data_path.cr_offs =
            dev->frame_buffer[dev->buf_index].addr[2] - dev->frame_buffer[dev->buf_index].addr[1];
    }

    ISP_DBG("%s cb offs 0x%08x cr_offs 0x%08x\n", __FUNCTION__,
            dev->config.mi_config.data_path.cb_offs,
            dev->config.mi_config.data_path.cr_offs);

    if (dev->config.mi_config.data_path.next_buff_addr !=
            dev->config.mi_config.data_path.curr_buff_addr)
    {
        if (dev->config.mi_config.data_path.next_buff_addr ==
                RK_ISP_INVALID_BUFF_ADDR)
        {
            /* disable MI MP */
            ISP_DBG("disabling MP MI\n");
            WRITE_RKISP_REG_AND_VERIFY(
                ~(RK_MI_CTRL_MP_ENABLE_IN |
                  RK_MI_CTRL_JPEG_ENABLE |
                  RK_MI_CTRL_RAW_ENABLE),
                RK_MI_CTRL,
                ~0);
        }
        else if (dev->config.mi_config.data_path.curr_buff_addr ==
                 RK_ISP_INVALID_BUFF_ADDR)
        {
            /* re-enable MI MP */
            ISP_DBG("enabling MP MI\n");
            WRITE_RKISP_REG(RK_MI_MP_FRAME, RK_MI_ICR);
            WRITE_RKISP_REG_AND_VERIFY(
                ~(RK_MI_CTRL_MP_ENABLE_IN |
                  RK_MI_CTRL_JPEG_ENABLE |
                  RK_MI_CTRL_RAW_ENABLE),
                RK_MI_CTRL,
                ~0);
            if (RKISP_PIX_FMT_IS_RAW_BAYER(out_pix_fmt))
            {
                WRITE_RKISP_REG_OR_VERIFY(
                    RK_MI_CTRL_RAW_ENABLE,
                    RK_MI_CTRL,
                    ~0);
            }
            else if (RKISP_PIX_FMT_IS_YUV(out_pix_fmt))
            {
                WRITE_RKISP_REG_OR_VERIFY(
                    RK_MI_CTRL_MP_ENABLE_IN, RK_MI_CTRL,
                    ~0);
            }
        }
        rt_do_update_rkisp_mi_buff_addr(dev, RK_ISP_STREAM_MP);
        dev->config.mi_config.data_path.curr_buff_addr =
            dev->config.mi_config.data_path.next_buff_addr;
    }

    return 0;
err:
    ISP_DBG("failed with err %d\n", ret);
    return ret;
}

static int rt_do_update_rkisp_mi_y12(
    struct rk_isp_device *dev)
{
    ISP_DBG("curr 0x%08x next 0x%08x\n",
            dev->config.mi_config.data_path.curr_buff_addr,
            dev->config.mi_config.data_path.next_buff_addr);

    if (dev->config.mi_config.data_path.next_buff_addr !=
            dev->config.mi_config.data_path.curr_buff_addr)
    {
        if (dev->config.mi_config.data_path.next_buff_addr ==
                RK_ISP_INVALID_BUFF_ADDR)
        {
            /* disable MI Y12 */
            ISP_DBG("disabling Y12 MI\n");
            /* 'switch off' MI interface */
            if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12)
            {
                WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_INIT_Y12_Y_EN), RK_MI_INIT, ~0);
            }
            else
            {
                WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_INIT_Y12_Y_EN | RK_MI_INIT_Y12_UV_EN),
                                           RK_MI_INIT, ~0);
            }
        }
        else if (dev->config.mi_config.data_path.curr_buff_addr ==
                 RK_ISP_INVALID_BUFF_ADDR)
        {
            /* re-enable MI Y12 */
            ISP_DBG("enabling Y12 MI\n");
            if (dev->config.mi_config.data_path.output.pix_fmt == RK_Y12)
            {
                WRITE_RKISP_REG(RK_MI_Y12_Y_FRAME | RK_MI_Y12_UV_FRAME | RK_MI_ALL_FRAME,
                                RK_MI_ICR);
                WRITE_RKISP_REG_OR_VERIFY(RK_MI_INIT_Y12_Y_EN,
                                          RK_MI_INIT, ~0);
            }
            else
            {
                WRITE_RKISP_REG(RK_MI_Y12_Y_FRAME | RK_MI_Y12_UV_FRAME | RK_MI_ALL_FRAME,
                                RK_MI_ICR);
                WRITE_RKISP_REG_OR_VERIFY(RK_MI_INIT_Y12_Y_EN | RK_MI_INIT_Y12_UV_EN,
                                          RK_MI_INIT, ~0);
            }
            ISP_DBG("RK_MI_INIT 0x%x\n",
                    READ_RKISP_REG(RK_MI_INIT));
        }
        rt_do_update_rkisp_mi_buff_addr(dev, RK_ISP_STREAM_Y12);
        dev->config.mi_config.data_path.curr_buff_addr =
            dev->config.mi_config.data_path.next_buff_addr;
    }

    return 0;
}

static int rt_do_update_rkisp_mi_sp(
    struct rk_isp_device *dev)
{
    ISP_DBG("curr 0x%08x next 0x%08x\n",
            dev->config.mi_config.data_path.curr_buff_addr,
            dev->config.mi_config.data_path.next_buff_addr);

    if (dev->config.mi_config.data_path.next_buff_addr !=
            dev->config.mi_config.data_path.curr_buff_addr)
    {
        if (dev->config.mi_config.data_path.next_buff_addr ==
                RK_ISP_INVALID_BUFF_ADDR)
        {
            /* disable MI SP */
            ISP_DBG("disabling SP MI\n");
            /* 'switch off' MI interface */
            WRITE_RKISP_REG_AND_VERIFY(~RK_MI_CTRL_SP_ENABLE,
                                       RK_MI_CTRL, ~0);
        }
        else if (dev->config.mi_config.data_path.curr_buff_addr ==
                 RK_ISP_INVALID_BUFF_ADDR)
        {
            /* re-enable MI SP */
            ISP_DBG("enabling SP MI\n");
            WRITE_RKISP_REG(RK_MI_SP_FRAME, RK_MI_ICR);
            WRITE_RKISP_REG_OR_VERIFY(RK_MI_CTRL_SP_ENABLE,
                                      RK_MI_CTRL, ~0);
        }
        rt_do_update_rkisp_mi_buff_addr(dev, RK_ISP_STREAM_SP);
        dev->config.mi_config.data_path.curr_buff_addr =
            dev->config.mi_config.data_path.next_buff_addr;
    }

    return 0;
}

static int rt_do_set_rkisp_fmt_mp(
    struct rk_isp_device *dev,
    struct rk_isp_strm_fmt *strm_fmt,
    unsigned int stride)
{
    int ret = 0;

    /* TBD: check whether format is a valid format for MP */
    if (strm_fmt->frm_fmt.pix_fmt == RK_YUV400)
    {
        ISP_ERR("format %s %dx%d@%d/%dfps, stride = %d not supported on MP\n",
                rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
                strm_fmt->frm_fmt.width,
                strm_fmt->frm_fmt.height,
                strm_fmt->frm_intrvl.numerator,
                strm_fmt->frm_intrvl.denominator,
                stride);
        ret = -RT_EINVAL;
        goto err;
    }
    else
    {
        ISP_DBG("format %s %dx%d@%d/%dfps, stride = %d not supported on MP\n",
                rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
                strm_fmt->frm_fmt.width,
                strm_fmt->frm_fmt.height,
                strm_fmt->frm_intrvl.numerator,
                strm_fmt->frm_intrvl.denominator,
                stride);
    }

    if (RKISP_PIX_FMT_IS_RAW_BAYER(strm_fmt->frm_fmt.pix_fmt))
    {
        dev->config.mi_config.raw_enable = true;
    }

    dev->config.mi_config.data_path.output = strm_fmt->frm_fmt;
    dev->config.mi_config.data_path.output.stride = stride;

    dev->config.mi_config.data_path.llength =
        rt_do_calc_rkisp_llength(
            strm_fmt->frm_fmt.width,
            stride,
            strm_fmt->frm_fmt.pix_fmt);

    g_data_stream.updt_cfg = true;
    g_data_stream.state = RK_ISP_STATE_READY;

    if (!RKISP_INP_IS_DMA(dev->config.input_sel))
    {
        ret = rt_select_rkisp_subdev_strm_fmt(dev);
        if (IS_ERR_VALUE(ret))
        {
            g_data_stream.updt_cfg = false;
            g_data_stream.state = RK_ISP_STATE_INACTIVE;
            goto err;
        }
    }

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_set_rkisp_fmt_y12(
    struct rk_isp_device *dev,
    struct rk_isp_strm_fmt *strm_fmt,
    unsigned int stride)
{
    int ret = 0;
    ISP_DBG("%s %dx%d@%d/%dfps, stride = %d, quantization: %d\n",
            rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
            strm_fmt->frm_fmt.width,
            strm_fmt->frm_fmt.height,
            strm_fmt->frm_intrvl.numerator,
            strm_fmt->frm_intrvl.denominator,
            stride,
            strm_fmt->frm_fmt.quantization);

    if (!(strm_fmt->frm_fmt.pix_fmt == RK_Y12 ||
            strm_fmt->frm_fmt.pix_fmt == RK_Y12_420SP ||
            strm_fmt->frm_fmt.pix_fmt == RK_Y12_422SP))
    {
        ISP_ERR("Y12 path don't support format %s,"
                " only YUV420SP and YUV422SP support.",
                rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt));
        ret = -RT_EINVAL;
        goto err;
    }

    dev->config.mi_config.data_path.output = strm_fmt->frm_fmt;
    dev->config.mi_config.data_path.output.stride = stride;

    dev->config.mi_config.data_path.llength =
        rt_do_calc_rkisp_llength(
            strm_fmt->frm_fmt.width,
            stride,
            strm_fmt->frm_fmt.pix_fmt);
    ISP_DBG("mi_config.y12.llength=%d",
            dev->config.mi_config.data_path.llength);

    g_data_stream.updt_cfg = true;
    g_data_stream.state = RK_ISP_STATE_READY;

    if (!RKISP_INP_IS_DMA(dev->config.input_sel))
    {
        ret = rt_select_rkisp_subdev_strm_fmt(dev);
        if (IS_ERR_VALUE(ret))
        {
            g_data_stream.updt_cfg = false;
            g_data_stream.state = RK_ISP_STATE_INACTIVE;
            goto err;
        }
    }

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_set_rkisp_fmt_sp(
    struct rk_isp_device *dev,
    struct rk_isp_strm_fmt *strm_fmt,
    unsigned int stride)
{
    int ret = 0;

    ISP_DBG("%s %dx%d@%d/%dfps, stride = %d, quantization: %d\n",
            rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
            strm_fmt->frm_fmt.width,
            strm_fmt->frm_fmt.height,
            strm_fmt->frm_intrvl.numerator,
            strm_fmt->frm_intrvl.denominator,
            stride,
            strm_fmt->frm_fmt.quantization);

    if (dev->config.mi_config.raw_enable)
        ISP_ERR("cannot activate SP when MP is set to RAW data output, you will not be able to (re-)start streaming\n");

    /* TBD: more detailed check whether format is a valid format for SP */
    /* TBD: remove the mode stuff */
    if (!RKISP_PIX_FMT_IS_YUV(strm_fmt->frm_fmt.pix_fmt) &&
            !RKISP_PIX_FMT_IS_RGB(strm_fmt->frm_fmt.pix_fmt))
    {
        ISP_ERR("format %s %dx%d@%d/%dfps, stride = %d not supported on SP\n",
                rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
                strm_fmt->frm_fmt.width,
                strm_fmt->frm_fmt.height,
                strm_fmt->frm_intrvl.numerator,
                strm_fmt->frm_intrvl.denominator,
                stride);
        ret = -RT_EINVAL;
        goto err;
    }

    dev->config.mi_config.data_path.output = strm_fmt->frm_fmt;
    dev->config.mi_config.data_path.llength =
        rt_do_calc_rkisp_llength(
            strm_fmt->frm_fmt.width,
            stride,
            strm_fmt->frm_fmt.pix_fmt);

    g_data_stream.updt_cfg = true;
    g_data_stream.state = RK_ISP_STATE_READY;

    if (!RKISP_INP_IS_DMA(dev->config.input_sel))
    {
        ret = rt_select_rkisp_subdev_strm_fmt(dev);
        if (IS_ERR_VALUE(ret))
        {
            g_data_stream.updt_cfg = false;
            g_data_stream.state = RK_ISP_STATE_INACTIVE;
            goto err;
        }
    }

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_set_rkisp_fmt_dma(
    struct rk_isp_device *dev,
    struct rk_isp_strm_fmt *strm_fmt,
    unsigned int stride)
{
    int ret = 0;

    ISP_DBG(
        "%s %dx%d@%d/%dfps, stride = %d\n",
        rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
        strm_fmt->frm_fmt.width,
        strm_fmt->frm_fmt.height,
        strm_fmt->frm_intrvl.numerator,
        strm_fmt->frm_intrvl.denominator,
        stride);

    if (!RKISP_PIX_FMT_IS_YUV(strm_fmt->frm_fmt.pix_fmt) &&
            !RKISP_PIX_FMT_IS_RAW_BAYER(strm_fmt->frm_fmt.pix_fmt))
    {
        ISP_ERR("format %s %dx%d@%d/%dfps, stride = %d not supported for DMA\n",
                rk_isp_pix_fmt_string(strm_fmt->frm_fmt.pix_fmt),
                strm_fmt->frm_fmt.width,
                strm_fmt->frm_fmt.height,
                strm_fmt->frm_intrvl.numerator,
                strm_fmt->frm_intrvl.denominator,
                stride);
        ret = -RT_EINVAL;
        goto err;
    }

    dev->config.mi_config.data_path.output = strm_fmt->frm_fmt;
    dev->config.mi_config.data_path.llength =
        rt_do_calc_rkisp_llength(
            strm_fmt->frm_fmt.width,
            stride,
            strm_fmt->frm_fmt.pix_fmt);

    g_data_stream.updt_cfg = true;
    g_data_stream.state = RK_ISP_STATE_READY;

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

static int rt_do_rkisp_dma_next_buff(
    struct rk_isp_device *dev)
{

    struct isp_video_buf *vbuf;
    int ret = 0;
    if (!g_data_stream.stop)
    {

        dev->config.mi_config.data_path.next_buff_addr = dev->frame_buffer[dev->buf_index].addr[0];

        rt_do_update_rkisp_mi_buff_addr(dev, RK_ISP_STREAM_DMA);

        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
            dev->config.mi_config.data_path.busy = true;

        /* workaround for write register failure bug */
        do
        {
            WRITE_RKISP_REG(RK_MI_DMA_START_ENABLE, RK_MI_DMA_START);
            //rt_hw_us_delay(1);
        }
        while (!READ_RKISP_REG(RK_MI_DMA_STATUS));
    }
err:
    ISP_ERR("\n  MI_DMA_CTRL 0x%08x\n"
            "  MI_DMA_STATUS 0x%08x\n",
            READ_RKISP_REG(RK_MI_DMA_CTRL),
            READ_RKISP_REG(RK_MI_DMA_STATUS));
    return ret;
}

static void rt_do_ready_for_rkisp_dma(
    struct rk_isp_device *dev)
{
    unsigned int mi_mis_tmp;

    WRITE_RKISP_REG(RK_MI_DMA_READY, RK_MI_ICR);
    mi_mis_tmp = READ_RKISP_REG(RK_MI_MIS);
    if (mi_mis_tmp & RK_MI_DMA_READY)
        ISP_DBG("dma icr err: 0x%x\n", mi_mis_tmp);
    dev->config.mi_config.data_path.busy = false;
}

static int rt_do_rkisp_mi_frame_end(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id)
{
    struct rk_isp_stream *stream;
    unsigned int y_base_addr;
    int (*update_mi)(
        struct rk_isp_device * dev);

    if (stream_id == RK_ISP_STREAM_MP)
    {
        stream = &g_data_stream;
        y_base_addr = RK_MI_MP_Y_BASE_AD_SHD;
        update_mi = rt_do_update_rkisp_mi_mp;
    }
    else if (stream_id == RK_ISP_STREAM_SP)
    {
        stream = &g_data_stream;
        y_base_addr = RK_MI_SP_Y_BASE_AD_SHD;
        update_mi = rt_do_update_rkisp_mi_sp;
    }
    else if (stream_id == RK_ISP_STREAM_Y12)
    {
        stream = &g_data_stream;
        y_base_addr = RK_MI_MP_CR_BASE_AD_SHD;
        update_mi = rt_do_update_rkisp_mi_y12;
    }
    else
        ISP_ERR("WRONG DATA PATH IN %s\n", __FUNCTION__);

    ISP_DBG("%s Y_BASE_AD_INIT/Y_BASE_AD_SHD (0x%08x/0x%08x)\n",
            rk_isp_stream_id_string(stream_id),
            (stream_id & RK_ISP_STREAM_MP) ?
            READ_RKISP_REG(RK_MI_MP_Y_BASE_AD_INIT) :
            READ_RKISP_REG(RK_MI_SP_Y_BASE_AD_INIT),
            READ_RKISP_REG(y_base_addr));

    dev->buf_index = (dev->frame_id % 2);
    dev->config.mi_config.data_path.next_buff_addr =
        dev->frame_buffer[0].addr[0];

    (void)update_mi(dev);

    stream->stall = false;

    return 0;
}

static int rt_do_update_rkisp_mi_immediate(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id)
{
    struct rk_isp_stream *stream;
    unsigned int *next_buff_addr;
    unsigned int y_base_addr;
    int (*update_mi)(
        struct rk_isp_device * dev);

    stream = &g_data_stream;
    next_buff_addr = &dev->config.mi_config.data_path.next_buff_addr;
    if (stream_id == RK_ISP_STREAM_MP)
    {
        y_base_addr = RK_MI_MP_Y_BASE_AD_SHD;
        update_mi = &rt_do_update_rkisp_mi_mp;
    }
    else if (stream_id == RK_ISP_STREAM_SP)
    {
        y_base_addr = RK_MI_SP_Y_BASE_AD_SHD;
        update_mi = &rt_do_update_rkisp_mi_sp;
    }
    else if (stream_id == RK_ISP_STREAM_Y12)
    {
        y_base_addr = RK_MI_MP_CR_BASE_AD_SHD;
        update_mi = &rt_do_update_rkisp_mi_y12;
    }
    else
    {
        BUG();
    }

    if (stream->state != RK_ISP_STATE_STREAMING)
        return 0;

    update_mi(dev);

    return 0;
}

static void rt_do_start_rkisp_mi(
    struct rk_isp_device *dev,
    bool start_mi)
{
    rt_base_t level;
    if (start_mi &&
            (g_data_stream.state == RK_ISP_STATE_STREAMING))
        start_mi = false;

    if ((start_mi &&
            (g_data_stream.state == RK_ISP_STATE_STREAMING)))
        BUG();

    if (start_mi)
    {
        dev->config.mi_config.data_path.next_buff_addr =
            RK_ISP_INVALID_BUFF_ADDR;
        dev->config.mi_config.data_path.curr_buff_addr =
            RK_ISP_INVALID_BUFF_ADDR;
        level = rt_mutex_take(dev->mutex, RT_WAITING_FOREVER);
        rt_do_rkisp_mi_frame_end(dev, dev->rk_isp_data_path);
        rt_mutex_release(dev->mutex);
        g_data_stream.stall = false;
    }

    /*
     *   New mi register update mode is invalidate in raw/jpeg for rv1108,
     * All path used old mode for rv1108;
     */
    WRITE_RKISP_REG_AND_VERIFY(~RK_MI_INIT_UPDATE_MODE_NEW, RK_MI_INIT, ~0);
    WRITE_RKISP_REG_OR(RK_MI_INIT_SOFT_UPD, RK_MI_INIT);

    if (start_mi)
    {
        level = rt_mutex_take(dev->mutex, RT_WAITING_FOREVER);
        rt_do_rkisp_mi_frame_end(dev, dev->rk_isp_data_path);
        rt_mutex_release(dev->mutex);
    }

    if (!dev->config.mi_config.async_updt)
        WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_GEN_CFG_UPD, RK_ISP_CTRL);

}

static void rt_do_stop_rkisp_mi(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id rk_isp_data_path)
{
    bool stop_mi_sp = rk_isp_data_path & RK_ISP_STREAM_SP;
    bool stop_mi_mp = rk_isp_data_path & RK_ISP_STREAM_MP;
    bool stop_mi_y12 = rk_isp_data_path & RK_ISP_STREAM_Y12;

    if (stop_mi_sp &&
            (g_data_stream.state != RK_ISP_STATE_STREAMING))
        stop_mi_sp = false;
    if (stop_mi_mp &&
            (g_data_stream.state != RK_ISP_STATE_STREAMING))
        stop_mi_mp = false;
    if (stop_mi_y12 &&
            (g_data_stream.state != RK_ISP_STATE_STREAMING))
        stop_mi_y12 = false;

    if (!stop_mi_sp && !stop_mi_mp && !stop_mi_y12)
        return;

    if (stop_mi_sp && stop_mi_mp && stop_mi_y12)
    {
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_SP_FRAME |
                                     RK_MI_MP_FRAME), RK_MI_IMSC, ~0);
        WRITE_RKISP_REG(RK_MI_SP_FRAME | RK_MI_MP_FRAME, RK_MI_ICR);
        /* Y12 */
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_Y12_Y_FRAME |
                                     RK_MI_Y12_UV_FRAME | RK_MI_ALL_FRAME), RK_MI_IMSC, ~0);
        WRITE_RKISP_REG(RK_MI_Y12_Y_FRAME |
                        RK_MI_Y12_UV_FRAME |
                        RK_MI_ALL_FRAME, RK_MI_ICR);
        WRITE_RKISP_REG_AND_VERIFY(~RK_MI_CTRL_SP_ENABLE, RK_MI_CTRL, ~0);
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_CTRL_MP_ENABLE_IN |
                                     RK_MI_CTRL_SP_ENABLE |
                                     RK_MI_CTRL_JPEG_ENABLE |
                                     RK_MI_CTRL_RAW_ENABLE), RK_MI_CTRL, ~0);
        /* Y12 */
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_INIT_Y12_Y_EN |
                                     RK_MI_INIT_Y12_UV_EN), RK_MI_INIT, ~0);
        WRITE_RKISP_REG(RK_MI_INIT_SOFT_UPD, RK_MI_INIT);
        ISP_DBG("RK_MI_INIT 0x%x\n",
                READ_RKISP_REG(RK_MI_INIT));
    }
    else if (stop_mi_sp)
    {
        WRITE_RKISP_REG(RK_MI_SP_FRAME, RK_MI_ICR);
        WRITE_RKISP_REG_AND_VERIFY(~RK_MI_CTRL_SP_ENABLE, RK_MI_CTRL, ~0);
    }
    else if (stop_mi_mp)
    {
        WRITE_RKISP_REG(RK_MI_MP_FRAME, RK_MI_ICR);
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_CTRL_MP_ENABLE_IN |
                                     RK_MI_CTRL_RAW_ENABLE), RK_MI_CTRL, ~0);
    }
    else if (stop_mi_y12)
    {
        WRITE_RKISP_REG(RK_MI_Y12_Y_FRAME |
                        RK_MI_Y12_UV_FRAME |
                        RK_MI_ALL_FRAME, RK_MI_ICR);
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_Y12_Y_FRAME |
                                     RK_MI_Y12_UV_FRAME | RK_MI_ALL_FRAME), RK_MI_IMSC, ~0);
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_INIT_Y12_Y_EN |
                                     RK_MI_INIT_Y12_UV_EN), RK_MI_INIT, ~0);
        ISP_DBG("RK_MI_INIT 0x%x\n",
                READ_RKISP_REG(RK_MI_INIT));
    }
}

static void rt_do_stop_rkisp_isp(
    struct rk_isp_device *dev)
{
    unsigned int status;
    unsigned long flags = 0;

    /* stop and clear MI, MIPI, and ISP interrupts */
    WRITE_RKISP_REG(0, RK_MIPI_IMSC);
    WRITE_RKISP_REG(~0, RK_MIPI_ICR);

    WRITE_RKISP_REG(0,  RK_ISP_IMSC);
    WRITE_RKISP_REG(~0, RK_ISP_ICR);

    WRITE_RKISP_REG_VERIFY(0, RK_MI_IMSC, ~0);
    WRITE_RKISP_REG(~0, RK_MI_ICR);

    WRITE_RKISP_REG_AND(~RK_MIPI_CTRL_OUTPUT_ENA, RK_MIPI_CTRL);
    /* stop ISP */
    WRITE_RKISP_REG_AND(~(RK_ISP_CTRL_ISP_INFORM_ENABLE |
                          RK_ISP_CTRL_ISP_ENABLE),
                        RK_ISP_CTRL);
    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
}

static int rt_do_stop_rkisp(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_ids)
{
    unsigned long flags = 0;
    bool stop_sp = stream_ids & RK_ISP_STREAM_SP;
    bool stop_mp = stream_ids & RK_ISP_STREAM_MP;
    bool stop_y12 = stream_ids & RK_ISP_STREAM_Y12;

    ISP_DBG("img_src state = %s, stop_sp = %d, stop_mp = %d, stop_y12 = %d\n",
            rk_isp_state_string(g_data_stream.state),
            stop_sp,
            stop_mp,
            stop_y12);

    if (!((stop_mp || stop_sp || stop_y12) &&
            (g_data_stream.state == RK_ISP_STATE_STREAMING)))
    {
        return 0;
    }

    rt_do_stop_rkisp_isp(dev);

    if (stop_sp)
    {
        if (!dev->config.mi_config.async_updt)
        {
            rt_do_stop_rkisp_mi(dev, stream_ids);
        }
        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
        {
            g_data_stream.stop = true;
            unsigned int status = 0;
            rt_tick_t tick_now = rt_tick_get();
            do
            {
                status &= READ_RKISP_REG(RK_ISP_MIS);
                status &= READ_RKISP_REG(RK_MI_MIS);
                status &= READ_RKISP_REG(RK_MIPI_MIS);
                if (!status)
                    g_data_stream.state = RK_ISP_STATE_READY;
            }
            while (!((g_data_stream.state != RK_ISP_STATE_STREAMING) || ((rt_tick_get() - tick_now) >= 1000)));
            g_data_stream.stop = false;
            if (dev->config.mi_config.data_path.busy)
                ISP_ERR("DATA PATH STILL STREAMING WHILE STOPPING IT\n");
        }
        WRITE_RKISP_REG_AND_VERIFY(~RK_MI_SP_FRAME, RK_MI_IMSC, ~0);
    }
    else if (stop_y12)
    {
        if (!dev->config.mi_config.async_updt)
        {
            rt_do_stop_rkisp_mi(dev, stream_ids);
        }
        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
        {
            g_data_stream.stop = true;
            unsigned int status = 0;
            rt_tick_t tick_now = rt_tick_get();
            do
            {
                status &= READ_RKISP_REG(RK_ISP_MIS);
                status &= READ_RKISP_REG(RK_MI_MIS);
                status &= READ_RKISP_REG(RK_MIPI_MIS);
                if (!status)
                    g_data_stream.state = RK_ISP_STATE_READY;
            }
            while (!((g_data_stream.state != RK_ISP_STATE_STREAMING) || ((rt_tick_get() - tick_now) >= 1000)));
            g_data_stream.stop = false;
            if (dev->config.mi_config.data_path.busy)
                ISP_DBG("DATA PATH STILL STREAMING WHILE STOPPING IT\n");
        }
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_Y12_Y_FRAME |
                                     RK_MI_Y12_UV_FRAME |
                                     RK_MI_ALL_FRAME),
                                   RK_MI_IMSC, ~0);
        WRITE_RKISP_REG_AND_VERIFY(~(RK_ISP_CTRL_ISP_Y12_Y_OUT_EN |
                                     RK_ISP_CTRL_ISP_Y12_UV_OUT_EN),
                                   RK_ISP_CTRL, ~0);
    }
    else /* stop_mp */
    {
        if (!dev->config.mi_config.async_updt)
        {
            rt_do_stop_rkisp_mi(dev, stream_ids);
        }

        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
        {
            g_data_stream.stop = true;
            unsigned int status = 0;
            rt_tick_t tick_now = rt_tick_get();
            do
            {
                status &= READ_RKISP_REG(RK_ISP_MIS);
                status &= READ_RKISP_REG(RK_MI_MIS);
                status &= READ_RKISP_REG(RK_MIPI_MIS);
                if (!status)
                    g_data_stream.state = RK_ISP_STATE_READY;
            }
            while (!((g_data_stream.state != RK_ISP_STATE_STREAMING) || ((rt_tick_get() - tick_now) >= 1000)));
            g_data_stream.stop = false;
            if (dev->config.mi_config.data_path.busy)
                ISP_DBG("DATA PATH STILL STREAMING WHILE STOPPING IT\n");
        }
        WRITE_RKISP_REG_AND_VERIFY(~(RK_MI_MP_FRAME),
                                   RK_MI_IMSC, ~0);

    }

    if ((stop_mp || stop_sp || stop_y12) && (g_data_stream.state == RK_ISP_STATE_STREAMING))
        g_data_stream.state = RK_ISP_STATE_READY;

    ISP_DBG("SP state = %s, MP state = %s, DMA state = %s, img_src state = %s\n"
            "  MI_CTRL 0x%08x\n"
            "  ISP_CTRL 0x%08x\n"
            "  MIPI_CTRL 0x%08x\n",
            rk_isp_state_string(g_data_stream.state),
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_ISP_CTRL),
            READ_RKISP_REG(RK_MIPI_CTRL));

    return 0;
}

static int rt_do_start_rkisp(
    struct rk_isp_device *dev,
    bool start)
{
    unsigned int ret;
    struct isp_video_buf *vb, *n;
    unsigned int i;
    int sensormode_addr;
    struct rk_camera_device *sub_dev;
    rk_isp_function_enter() ;

    sub_dev = dev->subdev;
    ISP_DBG("Stream state = %s\n"
            "MI_INIT = 0x%08x\n",
            rk_isp_state_string(g_data_stream.state),
            READ_RKISP_REG(RK_MI_INIT));

    if (!((start &&
            (g_data_stream.state != RK_ISP_STATE_STREAMING))))
        return 0;

    if (RKISP_INP_IS_DMA(dev->config.input_sel) &&
            (g_data_stream.state < RK_ISP_STATE_READY))
    {
        HAL_DelayMs(1);
        ("cannot start streaming, input source (DMA) not ready\n");
        ret = -RT_ERROR;
        goto err;
    }

    /* Activate MI */
    rt_do_start_rkisp_mi(dev, start);

    if ((g_data_stream.state != RK_ISP_STATE_STREAMING))
    {
        /* Activate MIPI */
        if (RKISP_INP_IS_MIPI(dev->config.input_sel))
            WRITE_RKISP_REG_OR(RK_MIPI_CTRL_OUTPUT_ENA, RK_MIPI_CTRL);

        /* Activate ISP ! */
        if (RKISP_INP_NEED_ISP(dev->config.input_sel))
            WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD |
                               RK_ISP_CTRL_ISP_INFORM_ENABLE |
                               RK_ISP_CTRL_ISP_ENABLE, RK_ISP_CTRL);

        g_data_stream.state = RK_ISP_STATE_STREAMING;
    }

    if (!RKISP_INP_IS_DMA(dev->config.input_sel))
    {
        /* RK spec says to wait for sufficient time after enabling
            the MIPI interface and before starting the
            sensor output. */
        HAL_DelayMs(1);
        /* start sensor output! */
        rt_do_rkisp_frame_id_reset(&dev->isp_dev);
        dev->isp_dev.frame_id_setexp = 0;

        rt_mutex_take(dev->subdev_exps.mutex, RT_WAITING_FOREVER);

#if defined(RT_USING_CAMERA)
        ret = sub_dev->ops->control(
                  sub_dev,
                  RK_VIDIOC_SENSOR_MODE_DATA, (void *)sensormode_addr);
        dev->subdev_exps.data[0].data = *((struct isp_supplemental_sensor_mode_data *)sensormode_addr);
        if (IS_ERR_VALUE(ret))
            goto err;
#endif

        dev->subdev_exps.data[0].v_frame_id = 0;
        dev->subdev_exps.data[0].data.isp_input_width =
            dev->config.isp_config.input->defrect.width;
        dev->subdev_exps.data[0].data.isp_input_height =
            dev->config.isp_config.input->defrect.height;
        dev->subdev_exps.data[0].data.isp_input_horizontal_start =
            dev->config.isp_config.input->defrect.x_axis;
        dev->subdev_exps.data[0].data.isp_input_vertical_start =
            dev->config.isp_config.input->defrect.y_axis;

        dev->subdev_exps.data[0].data.isp_output_width =
            dev->config.isp_config.output.width;
        dev->subdev_exps.data[0].data.isp_output_height =
            dev->config.isp_config.output.height;

        for (i = 1; i < RK_ISP_SUBDEV_DATA_NUM; i++)
            dev->subdev_exps.data[i] = dev->subdev_exps.data[0];

        dev->subdev_exps.exp_idx = 0;
        rt_mutex_release(dev->subdev_exps.mutex);

        ISP_DBG("starting image source...\n");

        if (IS_ERR_VALUE(ret))
            goto err;
    }
    else
    {
        ISP_DBG("starting DMA...\n");
        g_data_stream.state = RK_ISP_STATE_STREAMING;
        g_data_stream.stop = false;
        ret = rt_do_rkisp_dma_next_buff(dev);
        if (IS_ERR_VALUE(ret))
            goto err;
    }

    ISP_DBG("Stream state = %s\n"
            "  MI_CTRL 0x%08x\n"
            "  MI_INIT 0x%08x\n"
            "  ISP_CTRL 0x%08x\n"
            "  MIPI_CTRL 0x%08x\n",
            rk_isp_state_string(g_data_stream.state),
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_MI_INIT),
            READ_RKISP_REG(RK_ISP_CTRL),
            READ_RKISP_REG(RK_MIPI_CTRL));
    rk_isp_function_exit() ;

    return 0;
err:
    ISP_ERR("Stream state = %s\n"
            "  MI_CTRL 0x%08x\n"
            "  MI_INIT 0x%08x\n"
            "  ISP_CTRL 0x%08x\n"
            "  MIPI_CTRL 0x%08x\n",
            rk_isp_state_string(g_data_stream.state),
            READ_RKISP_REG(RK_MI_CTRL),
            READ_RKISP_REG(RK_MI_INIT),
            READ_RKISP_REG(RK_ISP_CTRL),
            READ_RKISP_REG(RK_MIPI_CTRL));
    ISP_ERR("failed with err %d\n", ret);
    return ret;
}

/* Function to be called inside ISR to update RK ISM/DCROP/RSZ */
static int rt_do_update_rkisp_ism_dcr_rsz(
    struct rk_isp_device *dev)
{
    int ret = 0;

    if (dev->config.isp_config.ism_config.ism_update_needed)
    {

        if (dev->config.isp_config.ism_config.ism_en)
        {
            if (dev->isp_dev.rk_ism_cropping == false)
            {
                dev->isp_dev.rk_ism_cropping = true;
            }
        }
        else
        {
            if (dev->isp_dev.rk_ism_cropping == true)
            {
                dev->isp_dev.rk_ism_cropping = false;
            }
        }
    }

    /* Update ISM, rk_isp_config_ism() changes the output size of isp,
    so it must be called before rk_isp_config_rsz() */
    if (dev->config.isp_config.ism_config.ism_update_needed)
    {
        rt_do_config_rkisp_ism(dev, false);
        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
            dev->config.mp_config.rsz_config.ism_adjust = true;

        dev->config.isp_config.ism_config.ism_update_needed = false;
        WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);

        if (dev->config.isp_config.ism_config.ism_en)
            dev->config.mi_config.async_updt |= RK_ISP_ASYNC_ISM;
    }

    /* Update RSZ */
    if ((dev->config.mp_config.rsz_config.ycflt_adjust ||
            dev->config.mp_config.rsz_config.ism_adjust))
    {
        ret = rt_do_config_rkisp_rsz(dev, RK_ISP_STREAM_MP, true);
        if (IS_ERR_VALUE(ret))
            goto err;
    }
    if ((dev->config.sp_config.rsz_config.ycflt_adjust ||
            dev->config.sp_config.rsz_config.ism_adjust))
    {
        ret = rt_do_config_rkisp_rsz(dev, RK_ISP_STREAM_SP, true);
        if (IS_ERR_VALUE(ret))
            goto err;
    }

    return 0;
err:
    ISP_DBG("failed with err %d\n", ret);
    return ret;
}

void rt_do_rkisp_mi_isr(int mis, void *cntxt)
{
    struct rk_isp_device *dev =
        (struct rk_isp_device *)(g_rk_isp_instance);
    unsigned int mi_mis_tmp;
    rt_base_t level;
    int mi_mis = READ_RKISP_REG(RK_MI_MIS);
    int ret = 0;
    rk_isp_function_enter();
    ISP_DBG("MI_RIS 0x%08x MI_IMSC 0x%08x MI_MIS 0x%08x MIS IN 0X%08X\n",
            READ_RKISP_REG(RK_MI_RIS),
            READ_RKISP_REG(RK_MI_IMSC),
            READ_RKISP_REG(RK_MI_MIS),
            mi_mis);

    rt_interrupt_enter();

    if (!mi_mis)
        goto __END;

    if (mi_mis & RK_MI_SP_FRAME)
    {
        dev->config.mi_config.data_path.busy = false;
        WRITE_RKISP_REG(RK_MI_SP_FRAME, RK_MI_ICR);
        mi_mis_tmp = READ_RKISP_REG(RK_MI_MIS);

        if (mi_mis_tmp & RK_MI_SP_FRAME)
            ISP_ERR("sp icr err: 0x%x\n", mi_mis_tmp);
    }

    if (mi_mis & RK_MI_MP_FRAME)
    {
        dev->config.mi_config.data_path.busy = false;
        WRITE_RKISP_REG(RK_MI_MP_FRAME, RK_MI_ICR);
        if (dev->config.mi_config.raw_enable)
        {

            if (g_raw_pic_sec_count == 1)
            {
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_OUT_V_OFFS);
                WRITE_RKISP_REG(8, RK_ISP_OUT_V_SIZE);
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_IS_V_OFFS);
                WRITE_RKISP_REG(8, RK_ISP_IS_V_SIZE);
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
            }
            else if (g_raw_pic_sec_count == 2)
            {
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_OUT_V_OFFS);
                WRITE_RKISP_REG(16, RK_ISP_OUT_V_SIZE);
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_IS_V_OFFS);
                WRITE_RKISP_REG(16, RK_ISP_IS_V_SIZE);
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
            }
            else if (g_raw_pic_sec_count == (dev->frame_seg_count - 2))
            {

                if (dev->frame_seg_last > 16)
                {
                    WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_OUT_V_OFFS);
                    WRITE_RKISP_REG(16, RK_ISP_OUT_V_SIZE);
                    WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_IS_V_OFFS);
                    WRITE_RKISP_REG(16, RK_ISP_IS_V_SIZE);
                    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
                }
                else
                {
                    goto _jump_here;
                }
            }
            else if (g_raw_pic_sec_count == (dev->frame_seg_count - 1))
            {
                if (dev->frame_seg_last > 16)
                {
                    WRITE_RKISP_REG(dev->frame_seg_offset - 16, RK_ISP_OUT_V_OFFS);
                    WRITE_RKISP_REG(dev->frame_seg_last - 16, RK_ISP_OUT_V_SIZE);
                    WRITE_RKISP_REG(dev->frame_seg_offset - 16, RK_ISP_IS_V_OFFS);
                    WRITE_RKISP_REG(dev->frame_seg_last - 16, RK_ISP_IS_V_SIZE);
                    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
                }
                else
                {
                    WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_OUT_V_OFFS);
                    WRITE_RKISP_REG(dev->frame_seg_last, RK_ISP_OUT_V_SIZE);
                    WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_IS_V_OFFS);
                    WRITE_RKISP_REG(dev->frame_seg_last, RK_ISP_IS_V_SIZE);
                    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
                }
            }
            else if (g_raw_pic_sec_count == dev->frame_seg_count)
            {
                dev->frame_seg_offset = 0;
                g_raw_pic_sec_count = 0;
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_OUT_V_OFFS);
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_IS_V_OFFS);
                WRITE_RKISP_REG(8, RK_ISP_OUT_V_SIZE);
                WRITE_RKISP_REG(8, RK_ISP_IS_V_SIZE);
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
            }
            else
            {
_jump_here:
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_OUT_V_OFFS);
                WRITE_RKISP_REG(dev->frame_seg_offset, RK_ISP_IS_V_OFFS);
                WRITE_RKISP_REG(32, RK_ISP_OUT_V_SIZE);
                WRITE_RKISP_REG(32, RK_ISP_IS_V_SIZE);
                WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
            }
        }
        mi_mis_tmp = READ_RKISP_REG(RK_MI_MIS);
        if (mi_mis_tmp & RK_MI_MP_FRAME)
            ISP_ERR("mp icr err: 0x%x\n", mi_mis_tmp);
    }

    if (mi_mis & (RK_MI_Y12_Y_FRAME | RK_MI_Y12_UV_FRAME))
    {
        dev->config.mi_config.data_path.busy = false;
        if (mi_mis & RK_MI_Y12_Y_FRAME)
        {
            WRITE_RKISP_REG(RK_MI_Y12_Y_FRAME, RK_MI_ICR);
            mi_mis_tmp = READ_RKISP_REG(RK_MI_MIS);
            if (mi_mis_tmp & RK_MI_Y12_Y_FRAME)
                ISP_ERR("y12 icr err: 0x%x\n", mi_mis_tmp);
        }
        if (mi_mis & RK_MI_Y12_UV_FRAME)
        {
            WRITE_RKISP_REG(RK_MI_Y12_UV_FRAME, RK_MI_ICR);
            mi_mis_tmp = READ_RKISP_REG(RK_MI_MIS);
            if (mi_mis_tmp & RK_MI_Y12_UV_FRAME)
                ISP_ERR("y12 icr err: 0x%x\n", mi_mis_tmp);
        }
    }

    if (mi_mis & RK_MI_DMA_READY)
        (void)rt_do_ready_for_rkisp_dma(dev);

    if (!RKISP_MI_IS_BUSY(dev))
    {
        if (dev->config.mi_config.async_updt)
        {
            unsigned int mp_y_off_cnt_shd =
                READ_RKISP_REG(RK_MI_MP_Y_OFFS_CNT_SHD);
            unsigned int sp_y_off_cnt_shd =
                READ_RKISP_REG(RK_MI_SP_Y_OFFS_CNT_SHD);
            unsigned int mp_cr_off_cnt_shd =/*y12*/
                READ_RKISP_REG(RK_MI_MP_CR_OFFS_CNT_SHD);

            WRITE_RKISP_REG(RK_MI_INIT_SOFT_UPD, RK_MI_INIT);
            ISP_DBG("RK_MI_INIT_SOFT_UPD\n");
            if (!dev->config.isp_config.ism_config.ism_en &&
                    (dev->config.mi_config.async_updt &
                     RK_ISP_ASYNC_ISM))
                dev->config.mi_config.async_updt &=
                    ~RK_ISP_ASYNC_ISM;
            if (((mp_y_off_cnt_shd != 0)) ||
                    (sp_y_off_cnt_shd != 0) || (mp_cr_off_cnt_shd != 0))
            {
                ISP_DBG("soft update too late (SP offset %d, MP offset %d,"
                        " Y12 offset %d)\n",
                        sp_y_off_cnt_shd, mp_y_off_cnt_shd, mp_cr_off_cnt_shd);
            }
        }

        if (g_data_stream.stop &&
                (g_data_stream.state == RK_ISP_STATE_STREAMING))
        {
            rt_do_stop_rkisp_mi(dev, dev->rk_isp_data_path);
            g_data_stream.state = RK_ISP_STATE_READY;
            g_data_stream.stop = false;

            /* Turn off MRSZ since it is not needed */
            WRITE_RKISP_REG(0, RK_MRSZ_CTRL);
            WRITE_RKISP_REG_OR(RK_RSZ_CTRL_CFG_UPD, RK_MRSZ_CTRL);

            ISP_DBG("MP has stopped\n");
        }

        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
        {
            rt_do_rkisp_mi_frame_end(dev, dev->rk_isp_data_path);
        }

        dev->b_mi_frame_end = true;

        if (g_data_stream.state == RK_ISP_STATE_STREAMING && (dev->rk_isp_data_path == RK_ISP_STREAM_DMA))
        {
            ret = rt_do_rkisp_dma_next_buff(dev);
        }

        if (dev->b_isp_frame_in &&
                (g_data_stream.state == RK_ISP_STATE_STREAMING) && ((dev->rk_isp_data_path == RK_ISP_STREAM_MP)))
            rt_do_update_rkisp_ism_dcr_rsz(dev);

    }

    WRITE_RKISP_REG(~(RK_MI_MP_FRAME |
                      RK_MI_SP_FRAME | RK_MI_DMA_READY | RK_MI_Y12_Y_FRAME |
                      RK_MI_Y12_UV_FRAME), RK_MI_ICR);
    ISP_DBG("MI INTERRUPT LEAVE STATUS IS %d\n", READ_RKISP_REG(RK_MI_MIS));
    rk_isp_function_exit();

__END:

    if (g_isp_meas_flag)
    {
        rt_event_send(event_isp, EVENT_3A_FLAG);
        g_isp_meas_flag = false;
    }

    rt_interrupt_leave();

    return;
}

void rt_do_sync_rkisp_sensor_mode_data(
    struct rk_isp_device *dev,
    unsigned int frame_id,
    struct isp_supplemental_sensor_mode_data *data)
{
    struct rk_isp_subdev_data *match_data;
    unsigned int i;
    unsigned char exp_idx;

    exp_idx = dev->subdev_exps.exp_idx;
    exp_idx = (exp_idx + 1) % RK_ISP_SUBDEV_DATA_NUM;
    match_data = &dev->subdev_exps.data[exp_idx];
    for (i = 0; i < RK_ISP_SUBDEV_DATA_NUM; i++)
    {
        if (frame_id >= dev->subdev_exps.data[exp_idx].v_frame_id)
            match_data = &dev->subdev_exps.data[exp_idx];

        exp_idx = (exp_idx + 1) % RK_ISP_SUBDEV_DATA_NUM;
    }

    rt_memcpy(data, &match_data->data,
              sizeof(struct isp_supplemental_sensor_mode_data));

    return;
}

int rt_do_rkisp_streamon(
    struct rk_isp_device *dev)
{
    int ret = 0;
    int stream_ids = dev->rk_isp_data_path;
    bool streamon_sp = stream_ids & RK_ISP_STREAM_SP;
    bool streamon_mp = stream_ids & RK_ISP_STREAM_MP;
    bool streamon_dma = stream_ids & RK_ISP_STREAM_DMA;
    bool streamon_y12 = stream_ids & RK_ISP_STREAM_Y12;
    int input_width = dev->config.isp_config.input->defrect.height;

    dev->frame_seg_offset = 0;
    dev->frame_seg_last = input_width - ((input_width >> 5) << 5);
    dev->frame_seg_count = input_width / 32;
    if (dev->frame_seg_last)
        dev->frame_seg_count++;
    else
        dev->frame_seg_last = 32;
    dev->frame_seg_count += 2;
    if (dev->frame_seg_last > 16)
    {
        dev->frame_seg_count++;
    }

    ISP_DBG("STREAM state = %s\n", rk_isp_state_string(g_data_stream.state));

    if (!(streamon_sp || streamon_mp || streamon_y12) && (g_data_stream.state != RK_ISP_STATE_STREAMING))
        return 0;

    if ((streamon_sp || streamon_mp || streamon_y12) &&
            (g_data_stream.state != RK_ISP_STATE_READY))
    {
        ISP_ERR("cannot start streaming on the current path, path not yet enabled\n");
        ret = -RT_ERROR;
        goto err;
    }

    if ((streamon_mp || streamon_sp || streamon_y12) &&
            (g_data_stream.state == RK_ISP_STATE_STREAMING))
        g_data_stream.updt_cfg = true;

    if (!dev->isp_clk_set.clk_en_flag)
    {
        clk_enable(dev->isp_clk_set.p_hclk_isp_gate);
        clk_enable(dev->isp_clk_set.p_aclk_isp_gate);
        clk_enable(dev->isp_clk_set.p_pll_isp_gate);
        dev->isp_clk_set.clk_en_flag = true;
    }

    ret = rt_do_config_rkisp_input(dev, stream_ids);
    if (IS_ERR_VALUE(ret))
        goto err;

    ret = rt_do_start_rkisp(dev, (streamon_mp || streamon_sp || streamon_y12));
    if (IS_ERR_VALUE(ret))
        goto err;

    rt_hw_interrupt_umask(dev->irq_set[0].irq_num);

#if defined(RT_USING_CAMERA)
    struct rk_camera_device *subdev = dev->subdev;
    ret = subdev->ops->control(subdev, 1, NULL);
#else
    ISP_DBG("no camera driver in rt_do_start_rkisp\n ");
#endif

    return 0;
err:
    ISP_DBG("Stream state = %s\n", rk_isp_state_string(g_data_stream.state));
    ISP_DBG("failed with error %d\n", ret);
    return ret;
}

int rt_do_rkisp_streamoff(struct rk_isp_device *dev)
{
    int stream_ids = dev->rk_isp_data_path;
    bool streamoff_sp = stream_ids & RK_ISP_STREAM_SP;
    bool streamoff_mp = stream_ids & RK_ISP_STREAM_MP;
    bool streamoff_dma = stream_ids & RK_ISP_STREAM_DMA;
    bool streamoff_y12 = stream_ids & RK_ISP_STREAM_Y12;

    unsigned int streamoff_all = 0;
    unsigned long lock_flags;
    struct rk_isp_img_src_vcm *vcm;
    rt_err_t ret;

    ISP_DBG("Stream state = %s\n",
            rk_isp_state_string(g_data_stream.state));

#if defined(RT_USING_CAMERA)
    bool enable = false;
    struct rk_camera_device *subdev;
    RT_ASSERT(dev != RT_NULL);
    subdev = dev->subdev;
    if (subdev->ops)
    {
        if (enable)
            ret = subdev->ops->control(subdev,
                                       RK_DEVICE_CTRL_CAMERA_STREAM_ON,
                                       NULL);
        else
            ret = subdev->ops->control(subdev,
                                       RK_DEVICE_CTRL_CAMERA_STREAM_OFF,
                                       NULL);
    }
    else
    {
        ISP_DBG("Warning: the subdev ops is null, check it!\n");
        ret = RT_ENOSYS;
    }
#endif

    ret = rt_do_stop_rkisp(dev, dev->rk_isp_data_path);
    if (IS_ERR_VALUE(ret))
        goto err;
    if ((streamoff_sp) &&
            (g_data_stream.state == RK_ISP_STATE_READY))
        g_data_stream.state = RK_ISP_STATE_INACTIVE;
    else if (streamoff_mp)
    {
        dev->config.mi_config.raw_enable = false;
        dev->config.mi_config.data_path.output.width = 0;
        dev->config.mi_config.data_path.output.height = 0;
        dev->config.mi_config.data_path.output.pix_fmt =
            RK_UNKNOWN_FORMAT;
        if (g_data_stream.state == RK_ISP_STATE_READY)
            g_data_stream.state = RK_ISP_STATE_INACTIVE;
    }

    if (streamoff_y12)
    {
        dev->config.mi_config.raw_enable = false;
        dev->config.mi_config.data_path.output.width = 0;
        dev->config.mi_config.data_path.output.height = 0;
        dev->config.mi_config.data_path.output.pix_fmt =
            RK_UNKNOWN_FORMAT;
        if (g_data_stream.state == RK_ISP_STATE_READY)
            g_data_stream.state = RK_ISP_STATE_INACTIVE;
    }

    if (streamoff_dma)
    {
        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
        {
            g_data_stream.stop = true;
            rt_tick_t tick_now = rt_tick_get();
            do
            {
                //rt_hw_us_delay(1);
            }
            while (!((g_data_stream.state != RK_ISP_STATE_STREAMING) || ((rt_tick_get() - tick_now) >= 1000)));
            g_data_stream.stop = false;
            if (dev->config.mi_config.data_path.busy)
                ISP_DBG("DATA PATH STILL STREAMING WHILE STOPPING IT\n");
        }
        if (g_data_stream.state == RK_ISP_STATE_READY)
            g_data_stream.state = RK_ISP_STATE_INACTIVE;
    }
    if ((g_data_stream.state <= RK_ISP_STATE_INACTIVE))
    {
        dev->isp_dev.input_width = 0;
        dev->isp_dev.input_height = 0;
        dev->config.isp_config.ism_config.ism_en = 0;
    }

    if (dev->isp_clk_set.clk_en_flag)
    {
        clk_disable(dev->isp_clk_set.p_pll_isp_gate);
        clk_disable(dev->isp_clk_set.p_aclk_isp_gate);
        clk_disable(dev->isp_clk_set.p_hclk_isp_gate);
        dev->isp_clk_set.clk_en_flag = false;
    }

    ISP_DBG("Stream state = %s, # frames received = %d\n",
            rk_isp_state_string(g_data_stream.state),
            dev->isp_dev.frame_id >> 1);

    rt_hw_interrupt_mask(dev->irq_set[0].irq_num);

    return 0;
err:
    ISP_ERR("Stream state = %s, # frames received = %d\n",
            rk_isp_state_string(g_data_stream.state),
            dev->isp_dev.frame_id >> 1);
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

int rt_do_set_rkisp_fmt(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id,
    struct rk_isp_strm_fmt *strm_fmt,
    unsigned int stride)
{
    int ret;
    ISP_DBG("set %s fmt\n", rk_isp_stream_id_string(stream_id));

    switch (stream_id)
    {
    case RK_ISP_STREAM_SP:
        return rt_do_set_rkisp_fmt_sp(dev, strm_fmt, stride);
    case RK_ISP_STREAM_MP:
        return rt_do_set_rkisp_fmt_mp(dev, strm_fmt, stride);
    case RK_ISP_STREAM_DMA:
        return rt_do_set_rkisp_fmt_dma(dev, strm_fmt, stride);
    case RK_ISP_STREAM_Y12:
        return rt_do_set_rkisp_fmt_y12(dev, strm_fmt, stride);
    default:
        ISP_ERR("unknown/unsupported stream ID %d\n", stream_id);
        ret = -RT_EINVAL;
        goto err;
    }

    ISP_DBG("FINISH SET %s fmt\n", rk_isp_stream_id_string(stream_id));
    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

int rt_do_rkisp_init(
    struct rk_isp_device *dev,
    unsigned int stream_ids)
{
    int ret;

    ISP_DBG("0x%08x\n", stream_ids);

    if (stream_ids & ~(RK_ISP_ALL_STREAMS))
    {
        ISP_ERR("unknown/unsupported stream IDs 0x%08x\n",
                stream_ids);
        ret = -RT_EINVAL;
        goto err;
    }

    /* set default input, failure is not fatal here */
    if ((g_data_stream.state == RK_ISP_STATE_DISABLED))
    {
        (void)rt_do_set_rkisp_input(dev, 0);
        dev->config.isp_config.si_enable = false;
        dev->config.isp_config.ie_config.effect =
            RK_ISP_IE_NONE;
    }

    if (stream_ids & RK_ISP_STREAM_SP)
        rt_do_init_rkisp_stream(dev, RK_ISP_STREAM_SP);
    if (stream_ids & RK_ISP_STREAM_MP)
        rt_do_init_rkisp_stream(dev, RK_ISP_STREAM_MP);
    if (stream_ids & RK_ISP_STREAM_DMA)
        rt_do_init_rkisp_stream(dev, RK_ISP_STREAM_DMA);
    if (stream_ids & RK_ISP_STREAM_Y12)
        rt_do_init_rkisp_stream(dev, RK_ISP_STREAM_Y12);

    return 0;
err:
    ISP_ERR("failed with error %d in func %s\n", ret, __FUNCTION__);
    return ret;
}

int rt_do_rkisp_release(
    struct rk_isp_device *dev,
    int stream_ids)
{
    int ret;
    unsigned long flags = 0;
    struct rk_isp_stream *strm_dev;

    ISP_DBG("0x%08x\n", stream_ids);

    if ((g_data_stream.state == RK_ISP_STATE_DISABLED))
        return 0;

    if (stream_ids & ~(RK_ISP_ALL_STREAMS))
    {
        ISP_ERR("unknown/unsupported stream IDs 0x%08x\n",
                stream_ids);
        ret = -RT_EINVAL;
        goto err;
    }
    else
    {
        if (g_data_stream.state == RK_ISP_STATE_STREAMING)
        {
            ISP_DBG("RK data path in streaming state,"
                    "should be stopped before release, trying to stop it\n");
            ret = rt_do_stop_rkisp(dev, stream_ids);
            if (IS_ERR_VALUE(ret))
                goto err;
        }
        g_data_stream.state = RK_ISP_STATE_DISABLED;
    }

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

void rt_do_rkisp_destroy(
    struct rk_isp_device *dev)
{
    rt_memset(dev, 0, sizeof(struct rk_isp_device));
}

int rt_do_set_rkisp_input(
    struct rk_isp_device *dev,
    unsigned int input)
{
    int ret;
    enum rk_isp_inp inp;

    ISP_DBG("setting input to %s\n",
            rk_isp_inp_string(
                rk_isp_input_index2inp(dev, input)));

    if (input >= dev->subdev_cnt + RKISP_INP_DMA_CNT())
    {
        ISP_ERR("invalid input %d\n", input);
        ret = -EINVAL;
        goto err;
    }

    dev->subdev = NULL;

    inp = rk_isp_input_index2inp(dev, input);

    /* DMA -> ISP or DMA -> IE */
    if ((inp == RK_ISP_INP_DMA) || (inp == RK_ISP_INP_DMA_IE))
        dev->config.isp_config.input =
            &dev->config.mi_config.data_path.output;
    else
    {
        dev->subdev = dev->subdev_array[input];
        dev->config.isp_config.input =
            &dev->config.img_src_output.frm_fmt;
    }
    dev->config.input_sel = inp;

    return 0;
err:
    ISP_ERR("failed with error %d\n", ret);
    return ret;
}

int rt_do_set_rkisp_vcm(
    struct rk_isp_device *dev,
    unsigned int id,
    int val)
{
    return 0;
}

int rt_do_set_rkisp_metadata(
    struct rk_isp_device *dev)
{
    unsigned long flags = 0;
    int ret = 0;
    struct isp_supplemental_sensor_mode_data sensor_mode;
    struct rk_buffer_metadata_s *metadata;

    if (!dev->b_stream_on)
    {
        ISP_ERR("stream is not streaming\n");
        return -1;
    }

#if 0
    rt_do_sync_rkisp_sensor_mode_data(dev,
                                      dev->isp_dev.frame_id,
                                      &sensor_mode);

    rt_mutex_take(dev->mutex, RT_WAITING_FOREVER);
    {
        metadata = &dev->isp_buf_metadata[dev->buf_index];

        metadata->frame_id = dev->isp_dev.frame_id;
        metadata->sensor.exp_time =
            sensor_mode.exp_time;
        metadata->sensor.gain =
            sensor_mode.gain;
        metadata->sensor.exp_time_l =
            sensor_mode.exp_time_l;
        metadata->sensor.gain_l =
            sensor_mode.gain_l;
        metadata->sensor.exp_time_s =
            sensor_mode.exp_time_s;
        metadata->sensor.gain_s =
            sensor_mode.gain_s;

    }
    rt_mutex_release(dev->mutex);
#endif
    return 0;
}

int rt_do_get_rkisp_target_frm_size(
    struct rk_isp_device *dev,
    unsigned int *target_width,
    unsigned int *target_height)
{
    if (g_data_stream.state >= RK_ISP_STATE_READY)
    {
        *target_width = dev->config.mi_config.data_path.output.width;
        *target_height = dev->config.mi_config.data_path.output.height;
    }
    else
    {
        ISP_ERR("cannot get target frame size, no path ready\n");
        return -RT_ERROR;
    }
    return 0;
}

int rt_do_calc_rkisp_isp_cropping(
    struct rk_isp_device *dev,
    unsigned int *width,
    unsigned int *height,
    unsigned int *h_offs,
    unsigned int *v_offs)
{
    int ret = 0;
    unsigned int input_width;
    unsigned int input_height;
    unsigned int target_width;
    unsigned int target_height;

    if (dev->config.isp_config.input == RT_NULL)
    {
        ISP_ERR("no input selected for ISP\n");
        ret = -RT_ERROR;
        goto err;
    }

    input_width = dev->config.isp_config.input->defrect.width;
    input_height = dev->config.isp_config.input->defrect.height;

    ret = rt_do_get_rkisp_target_frm_size(dev,
                                          &target_width, &target_height);

    ISP_DBG("GET target height %d width %d success\n", target_height, target_width);
    if (IS_ERR_VALUE(ret))
        goto err;

    *width = input_width;
    *height = input_width * target_height / target_width;
    *v_offs = 0;
    *h_offs = 0;
    *height &= ~1;

    if (dev->config.mi_config.raw_enable)
    {
        if (*height == 32)
            goto __return_;
    }

    if (*height < input_height)
        /* vertical cropping */
        *v_offs = (input_height - *height) >> 1;
    else if (*height > input_height)
    {
        /* horizontal cropping */
        *height = input_height;
        *width = input_height * target_width / target_height;
        *width &= ~1;
        *h_offs = (input_width - *width) >> 1;
    }
__return_:

    return 0;
err:
    ISP_ERR("failed with err %d\n", ret);
    return ret;
}

int rt_do_calc_rkisp_min_out_buff_size(
    struct rk_isp_device *dev,
    enum rk_isp_stream_id stream_id,
    unsigned int *size)
{
    int ret = 0;
    enum rk_isp_pix_fmt pix_fmt;
    unsigned int llength;
    unsigned int height;
    unsigned int bpp;
    struct rk_isp_mi_path_config *mi_path;
    struct rk_isp_stream *stream;

    ISP_DBG("%s\n", rk_isp_stream_id_string(stream_id));

    mi_path = &dev->config.mi_config.data_path;
    stream = &g_data_stream;

    if (stream->state < RK_ISP_STATE_READY)
    {
        ISP_ERR("cannot calculate buffer size, %s stream not ready\n",
                rk_isp_stream_id_string(stream_id));
        ret = -RT_EINVAL;
        goto err;
    }
    pix_fmt = mi_path->output.pix_fmt;
    llength = mi_path->llength;
    height = mi_path->output.height;
    ISP_DBG("mi_path->llength: 0x%x\n", mi_path->llength);

    if (RKISP_PIX_FMT_IS_RAW_BAYER(pix_fmt) &&
            RKISP_PIX_FMT_GET_BPP(pix_fmt) > 8)
        /* RAW input > 8BPP is stored with 16BPP by MI */
        bpp = 16;
    else
        bpp = RKISP_PIX_FMT_GET_BPP(pix_fmt);
    *size = llength * height * bpp / 8;

    ISP_DBG("calculated buffer size: %d\n", *size);

    return 0;
err:
    ISP_ERR("failed with err %d\n", ret);
    return ret;
}

enum
{
    isp_data_loss = 0,
    isp_pic_size_err,
    mipi_fifo_err,
    dphy_err_sot,
    dphy_err_sot_sync,
    dphy_err_eot_sync,
    dphy_err_ctrl,
    csi_err_protocol,
    csi_ecc1_err,
    csi_ecc2_err,
    csi_cs_err,
};

static void rt_do_restart_rkisp_hw(struct rk_isp_device *dev)
{
    rk_isp_hw_errors[isp_pic_size_err].count = 0;
    rk_isp_hw_errors[isp_data_loss].count = 0;
    rk_isp_hw_errors[csi_err_protocol].count = 0;
    rk_isp_hw_errors[csi_ecc1_err].count = 0;
    rk_isp_hw_errors[csi_ecc2_err].count = 0;
    rk_isp_hw_errors[csi_cs_err].count = 0;
    WRITE_RKISP_REG(0x00000841, RK_IRCL);
    WRITE_RKISP_REG(0x0, RK_IRCL);

    /* enable mipi frame end interrupt*/
    WRITE_RKISP_REG(RK_MIPI_FRAME_END, RK_MIPI_IMSC);
    /* enable csi protocol errors interrupts*/
    WRITE_RKISP_REG_OR(RK_MIPI_ERR_CSI, RK_MIPI_IMSC);
    /* enable dphy errors interrupts*/
    WRITE_RKISP_REG_OR(RK_MIPI_ERR_DPHY, RK_MIPI_IMSC);
    /* add fifo error*/
    WRITE_RKISP_REG_OR(RKMIPI_SYNC_FIFO_OVFLW(3), RK_MIPI_IMSC);
    /* add data overflow_error*/
    WRITE_RKISP_REG_OR(RK_MIPI_ADD_DATA_OVFLW, RK_MIPI_IMSC);

    WRITE_RKISP_REG(0x0, RK_MI_MP_Y_OFFS_CNT_INIT);
    WRITE_RKISP_REG(0x0, RK_MI_MP_CR_OFFS_CNT_INIT);
    WRITE_RKISP_REG(0x0, RK_MI_MP_CB_OFFS_CNT_INIT);
    WRITE_RKISP_REG(0x0, RK_MI_SP_Y_OFFS_CNT_INIT);
    WRITE_RKISP_REG(0x0, RK_MI_SP_CR_OFFS_CNT_INIT);
    WRITE_RKISP_REG(0x0, RK_MI_SP_CB_OFFS_CNT_INIT);
    WRITE_RKISP_REG_OR(RK_MI_CTRL_INIT_OFFSET_EN, RK_MI_CTRL);

    /* Enable ISP ! */
    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD |
                       RK_ISP_CTRL_ISP_INFORM_ENABLE |
                       RK_ISP_CTRL_ISP_ENABLE, RK_ISP_CTRL);
    /*enable MIPI */
    WRITE_RKISP_REG_OR(RK_MIPI_CTRL_OUTPUT_ENA, RK_MIPI_CTRL);
}

static int rt_do_get_rkisp_stride(unsigned int pix_fmt,
                                  unsigned int width, unsigned int *stride)
{
    switch (pix_fmt)
    {
    case V4L2_PIX_FMT_NV12:
    case V4L2_PIX_FMT_NV21:
    case V4L2_PIX_FMT_YVU420:
    case V4L2_PIX_FMT_NV16:
    case V4L2_PIX_FMT_YUV422P:
        *stride = width;
        break;
    case V4L2_PIX_FMT_RGB565:
    case V4L2_PIX_FMT_YUYV:
        *stride = width * 2;
        break;
    case V4L2_PIX_FMT_RGB32:
        *stride = width * 4;
        break;
    case V4L2_PIX_FMT_JPEG:
    case V4L2_PIX_FMT_MJPEG:
    case V4L2_PIX_FMT_H264:
        /* not used */
        *stride = 0;
        break;
    case V4L2_PIX_FMT_SBGGR10:
    case V4L2_PIX_FMT_SGBRG10:
    case V4L2_PIX_FMT_SGRBG10:
    case V4L2_PIX_FMT_SRGGB10:
        *stride = width * 2;
        break;
    case V4L2_PIX_FMT_SBGGR8:
    case V4L2_PIX_FMT_SGBRG8:
    case V4L2_PIX_FMT_SGRBG8:
    case V4L2_PIX_FMT_SRGGB8:
        *stride = width;
        break;
    case V4L2_PIX_FMT_Y10:
        *stride = width * 2;
        break;
    case V4L2_PIX_FMT_Y12:
        *stride = width * 2;
        break;
    case V4L2_PIX_FMT_GREY:
        *stride = width;
        break;
    default:
        ISP_ERR("%s: Unsupported V4L2 pixel format %c%c%c%c\n",
                __FUNCTION__,
                pix_fmt & 0xFF, (pix_fmt >> 8) & 0xFF,
                (pix_fmt >> 16) & 0xFF, (pix_fmt >> 24) & 0xFF);
        return -1;
    }

    return 0;
}

inline rt_err_t rt_do_set_isp_ie_rgb2grey_enable()
{
    unsigned int status = 0;
    rt_err_t ret = RT_EOK;

    status = READ_RKISP_REG(RK_IMG_EFF_CTRL);

    if (status & RK_IMG_EFF_CTRL_MODE_BLACKWHITE)
        return ret;
    else
    {
        WRITE_RKISP_REG_OR(RK_ICCL_IE_CLK, RK_ICCL);
        WRITE_RKISP_REG(RK_IMG_EFF_CTRL_ENABLE |
                        RK_IMG_EFF_CTRL_MODE_BLACKWHITE, RK_IMG_EFF_CTRL);
        WRITE_RKISP_REG_OR(RK_IMG_EFF_CTRL_CFG_UPD, RK_IMG_EFF_CTRL);
    }

    return ret;
}

inline rt_err_t rt_do_set_isp_ie_grey2rgb_enable()
{
    unsigned int status = 0;
    rt_err_t ret = RT_EOK;

    status = READ_RKISP_REG(RK_IMG_EFF_CTRL);

    if ((status & RK_IMG_EFF_CTRL_ENABLE))
    {
        WRITE_RKISP_REG_AND(~RK_IMG_EFF_CTRL_ENABLE, RK_IMG_EFF_CTRL);
        WRITE_RKISP_REG_AND(~RK_ICCL_IE_CLK, RK_ICCL);
    }

    return ret;
}

#define READ_MIPI_REG(base, addr)       (*(unsigned int *)(addr+(base)))

void rt_do_rkisp_mipi_isr(int mipi_mis, void *cntxt)
{
    struct rk_isp_device *dev =
        (struct rk_isp_device *)(g_rk_isp_instance);
    unsigned int mipi_ris = 0;
    unsigned int embedded_data_vc;

    rt_interrupt_enter();
    mipi_ris = READ_RKISP_REG(RK_MIPI_RIS);
    mipi_mis = READ_RKISP_REG(RK_MIPI_MIS);

    ISP_DBG("MIPI_MIS %08x, MIPI_RIS %08x, MIPI_IMSC %08x\n",
            mipi_mis, mipi_ris, READ_RKISP_REG(RK_MIPI_IMSC));

    if (mipi_mis & RK_MIPI_ERR_DPHY)
    {
        unsigned int mipi_status = READ_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_STATUS_OFFSET);
        unsigned int cru_gate_status = READ_MIPI_REG(CRU_BASE, CRU_GATE_CON04_OFFSET);
        ISP_ERR("RK_MIPI_ERR_DPHY: 0x%x mipi status %x cru_gating_con04 %x\n", mipi_mis, mipi_status, cru_gate_status);
        if (mipi_mis & RKMIPI_ERR_CTRL(3))
            WRITE_RKISP_REG_AND(~(RKMIPI_ERR_CTRL(3)), RK_MIPI_IMSC);
    }

    if (mipi_mis & RK_MIPI_ERR_CSI)
    {
        ISP_ERR("RK_MIPI_ERR_CSI: 0x%x\n", mipi_mis);
    }

    if (mipi_mis & RKMIPI_SYNC_FIFO_OVFLW(3))
    {
        ISP_ERR("RK_MIPI_SYNC_FIFO_OVFLW: 0x%x\n", mipi_mis);
    }

    if (mipi_mis & RK_MIPI_ADD_DATA_OVFLW)
    {
        ISP_ERR("RK_MIPI_ADD_DATA_OVFLW: 0x%x\n", mipi_mis);
    }

    if (mipi_mis == RK_MIPI_FRAME_END)
    {
        WRITE_RKISP_REG_OR(RKMIPI_ERR_CTRL(3), RK_MIPI_IMSC);
        dev->frame_id++;
    }

    if (mipi_mis)
        WRITE_RKISP_REG(~0, RK_MIPI_ICR);

    rt_interrupt_leave();
    return;
}

void rt_do_rkisp_isr(int mis, void *cntxt)
{
    struct rk_isp_device *dev =
        (struct rk_isp_device *)(g_rk_isp_instance);
    unsigned int isp_mis_tmp = 0;
    unsigned int isp_err = 0;
    rk_isp_function_enter();
    unsigned int time_data;
    bool flag_v_start = false;
    rt_base_t level;

    ISP_DBG("inter in 0x%08x ISP_MIS 0x%08x, ISP_RIS 0x%08x, ISP_IMSC 0x%08x\n",
            mis,
            READ_RKISP_REG(RK_ISP_MIS),
            READ_RKISP_REG(RK_ISP_RIS),
            READ_RKISP_REG(RK_ISP_IMSC));

    int isp_mis = READ_RKISP_REG(RK_ISP_MIS);

    rt_interrupt_enter();

    dev->b_isp_frame_in = false;
    dev->b_mi_frame_end = false;

    if (isp_mis & RK_ISP_V_START)
    {
        flag_v_start = true;
        g_isp_meas_flag = true;
        time_data = HAL_GetTick();

        rt_do_rkisp_v_start(&dev->isp_dev, &time_data);

        if (dev->config.mi_config.raw_enable)
        {
#if 0
            rt_kprintf("[%d %d %d %d]\n",
                       READ_RKISP_REG(RK_ISP_OUT_V_OFFS),
                       READ_RKISP_REG(RK_ISP_OUT_H_OFFS),
                       READ_RKISP_REG(RK_ISP_OUT_V_SIZE),
                       READ_RKISP_REG(RK_ISP_OUT_H_SIZE));
#endif

            g_raw_pic_sec_count++;

            if (g_raw_pic_sec_count < 3)
                dev->frame_seg_offset += 8;
            else if (g_raw_pic_sec_count == 3)
                dev->frame_seg_offset += 16;
            else
                dev->frame_seg_offset += 32;
        }

        WRITE_RKISP_REG(RK_ISP_V_START, RK_ISP_ICR);
        isp_mis_tmp = READ_RKISP_REG(RK_ISP_MIS);
        if (isp_mis_tmp & RK_ISP_V_START)
            ISP_ERR("isp icr v_statr err: 0x%x\n", isp_mis_tmp);

        if (!dev->config.mi_config.async_updt)
        {
            WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_GEN_CFG_UPD, RK_ISP_CTRL);
            ISP_DBG("RK_ISP_CTRL_ISP_GEN_CFG_UPD\n");
        }
    }

    if ((isp_mis & (RK_ISP_DATA_LOSS | RK_ISP_PIC_SIZE_ERROR)))
    {
        g_data_stream.stall = true;

        if ((isp_mis & RK_ISP_PIC_SIZE_ERROR))
        {
            /* Clear pic_size_error */
            WRITE_RKISP_REG(RK_ISP_PIC_SIZE_ERROR, RK_ISP_ICR);
            rk_isp_hw_errors[isp_pic_size_err].count++;
            isp_err = READ_RKISP_REG(RK_ISP_ERR);
            ISP_ERR("RK_ISP_PIC_SIZE_ERROR (0x%08x)\n", isp_err);
            WRITE_RKISP_REG(isp_err, RK_ISP_ERR_CLR);
        }
        else if ((isp_mis & RK_ISP_DATA_LOSS))
        {
            /* Clear data_loss */
            WRITE_RKISP_REG(RK_ISP_DATA_LOSS, RK_ISP_ICR);
            rk_isp_hw_errors[isp_data_loss].count++;
            ISP_ERR("RK_ISP_DATA_LOSS\n");
            WRITE_RKISP_REG(RK_ISP_DATA_LOSS, RK_ISP_ICR);
        }

        /* Stop ISP */
        WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_INFORM_ENABLE &
                            ~RK_ISP_CTRL_ISP_ENABLE,
                            RK_ISP_CTRL);
        /*   isp_update */
        WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CFG_UPD, RK_ISP_CTRL);
        rt_do_restart_rkisp_hw(dev);

    }

    if (isp_mis & RK_ISP_FRAME_IN)
    {
        time_data = HAL_GetTick();

        WRITE_RKISP_REG(RK_ISP_FRAME_IN, RK_ISP_ICR);

        rt_do_rkisp_frame_in(&dev->isp_dev, &time_data);
        isp_mis_tmp = READ_RKISP_REG(RK_ISP_MIS);
        if (isp_mis_tmp & RK_ISP_FRAME_IN)
            ISP_ERR("isp icr frame_in err: 0x%x\n", isp_mis_tmp);

        /* restart MI if RK has run out of buffers */
        if (!RKISP_INP_IS_DMA(dev->config.input_sel) &&
                !RKISP_MI_IS_BUSY(dev) &&
                (dev->config.mi_config.async_updt))
        {
            unsigned int mi_isr = 0;

            if (g_data_stream.state == RK_ISP_STATE_STREAMING)
            {
                if (dev->rk_isp_data_path & RK_ISP_STREAM_MP)
                    mi_isr |= RK_MI_MP_FRAME;
                if (dev->rk_isp_data_path & RK_ISP_STREAM_SP)
                    mi_isr |= RK_MI_SP_FRAME;
                if (dev->rk_isp_data_path & RK_ISP_STREAM_Y12)
                    mi_isr |= RK_MI_Y12_Y_FRAME | RK_MI_Y12_UV_FRAME |
                              RK_MI_ALL_FRAME;
            }
            WRITE_RKISP_REG(mi_isr, RK_MI_ISR);
        }
    }

    if (isp_mis & RK_ISP_FRAME)
    {
        dev->isp_buf_metadata[dev->frame_id % 2].frame_id = dev->frame_id;
        /* Clear Frame In (ISP) */
        WRITE_RKISP_REG(RK_ISP_FRAME, RK_ISP_ICR);
        isp_mis_tmp = READ_RKISP_REG(RK_ISP_MIS);
        if (isp_mis_tmp & RK_ISP_FRAME)
            ISP_ERR("isp icr frame end err: 0x%x\n", isp_mis_tmp);

        if (dev->b_mi_frame_end &&
                ((g_data_stream.state == RK_ISP_STATE_STREAMING)))
            rt_do_update_rkisp_ism_dcr_rsz(dev);
        dev->b_isp_frame_in = true;

        if (dev->b_set_eff_en_flag)
        {
            rt_do_set_isp_ie_rgb2grey_enable();
            dev->b_set_eff_en_flag = false;
        }

        if (dev->b_set_eff_clr_flag)
        {
            rt_do_set_isp_ie_grey2rgb_enable();
            dev->b_set_eff_clr_flag = false;
        }
    }

    int ret = rt_do_set_rkisp_isp_module_interrupt(&dev->isp_dev, isp_mis);
    rk_isp_function_exit();
    rt_interrupt_leave();
    return;
}

void rt_do_handle_rkisp_inter(int mis, void *cntxt)
{
    rt_interrupt_enter();

    rt_do_rkisp_mipi_isr(mis, cntxt);
    rt_do_rkisp_isr(mis, cntxt);
    rt_do_rkisp_mi_isr(mis, cntxt);

    rt_interrupt_leave();
}

rt_err_t rk_isp_rx_indicate(struct rk_isp_dev *dev, rt_size_t size)
{
    /*TODO:maybe not used*/
    return RT_EOK;
}

static inline struct rk_isp_device *rk_get_isp_dev(struct rk_isp_dev *dev)
{
    struct rk_isp_device *ispdev = dev->parent.user_data;

    return ispdev;
}

rt_err_t rt_do_rkisp_create_aaa_event()
{
    if (!event_isp)
    {
        event_isp = rt_event_create("event_isp", RT_IPC_FLAG_PRIO);

        if (event_isp != RT_NULL)
        {
            ISP_DBG("RT-Thread create event successful\r\n");
        }
        else
        {
            ISP_ERR("RT-Thread create event failed\r\n");
            return -RT_ENOMEM;
        }
    }

    return RT_EOK;
}

rt_err_t rk_isp_init(struct rk_isp_dev *dev)
{
    unsigned char i, scene_idx = 0, ret, length;
    unsigned int width, height, bpp, bpl, stride;
    struct rk_isp_device *ispdev = rk_get_isp_dev(dev);
    struct rk_camera_device *subdev = ispdev->subdev;
    struct rk_isp_strm_fmt strm_fmt;

    struct isp_init_info *isp_mem_config = ispdev->isp_init;
    struct awb_init_info *p_scene = isp_mem_config->scene;

    ISP_DBG("run in func rk_isp_init, init param addr is 0x%08x \n", isp_mem_config);
    ispdev->rk_isp_data_path = isp_mem_config->data_path;

    ISP_DBG("INIT STREAM ID is %s -- %s", rk_isp_stream_id_string(isp_mem_config->data_path),
            rk_isp_stream_id_string(ispdev->rk_isp_data_path));

    RT_ASSERT(dev != RT_NULL);

    ret = rt_do_init_rkisp_subdev(ispdev);

    if (ret)
    {
        ISP_ERR("rt_do_init_rkisp_subdev failed, plz check\n");
        return RT_ERROR;
    }
    else
    {
        ISP_DBG("init rkisp_subdev success\n");
    }

    struct v4l2_mbus_framefmt frame_fmt;
    struct v4l2_subdev_frame_interval frame_inter;
    struct pltfrm_cam_itf *cam_itf = &(ispdev->isp_init->cam_itf);
    subdev = ispdev->subdev_array[0];
    ret = subdev->ops->control(subdev, RK_DEVICE_CTRL_CAMERA_GET_FORMAT, &frame_fmt);
    ret |= subdev->ops->control(subdev, RK_DEVICE_CTRL_CAMERA_GET_FRMINTVAL, &frame_inter);
    ret |= subdev->ops->control(subdev, RK_DEVICE_CTRL_CAMERA_GET_ITF_CFG, cam_itf);

    if (ret)
        return RT_ERROR;

    ispdev->isp_init->strm_desc[0].width = frame_fmt.width;
    ispdev->isp_init->strm_desc[0].height = frame_fmt.height;
    ispdev->isp_init->strm_desc[0].pix_format = rt_do_trans_sensor_code(frame_fmt.code);
    ispdev->isp_init->strm_desc[0].denominator = frame_inter.interval.denominator;
    ispdev->isp_init->strm_desc[0].numerator = frame_inter.interval.numerator;

    width = strm_fmt.frm_fmt.width = isp_mem_config->strm_fmt.width;
    height = strm_fmt.frm_fmt.height = isp_mem_config->strm_fmt.height;
    strm_fmt.frm_fmt.pix_fmt = rt_do_rk_isp_fmt_trans(
                                   isp_mem_config->strm_fmt.pix_fmt);

    strm_fmt.frm_fmt.quantization = 2;

    ret = rt_do_get_rkisp_stride(isp_mem_config->strm_fmt.pix_fmt,
                                 isp_mem_config->strm_fmt.width, &stride);

    if (ret)
    {
        ISP_ERR("rt_do_get_rkisp_stride failed, plz check\n");
        return RT_ERROR;
    }

#ifdef RK_ISP_USING_AWB
    for (i = 0; i < AWB_SCENE_DEFAULT_NUM; i++)
    {
        if (isp_mem_config->strm_fmt.light_vol >= p_scene[i].range[0] &&
                isp_mem_config->strm_fmt.light_vol < p_scene[i].range[1])
        {
            scene_idx = i;
            break;
        }
    }
#endif

    ret = rt_do_rkisp_init(ispdev, ispdev->rk_isp_data_path);

    if (ret)
        ISP_ERR("rt_do_rkisp_init failed, plz check\n");
    else
        ISP_DBG("rt_do_rkisp_init success\n");

    ret = rt_do_set_rkisp_fmt(ispdev, ispdev->rk_isp_data_path,
                              &strm_fmt, stride);

    if (ret)
    {
        ISP_DBG("rt_do_set_rkisp_fmt failed, plz check\n");
        return RT_ERROR;
    }
    else
        ISP_DBG("rt_do_set_rkisp_fmt success\n");

    rt_do_stop_rkisp_isp(ispdev);

    if (!ispdev->b_set_out_addr)
    {
        if (ispdev->frame_buffer[0].addr[0] == 0 || ispdev->frame_buffer[1].addr[0] == 0)
        {
            int num = sizeof(ispdev->frame_buffer) / sizeof(struct rk_isp_frame_buffer);
            for (int k = 0; k < num; k++)
            {
                struct rk_isp_frame_buffer *frm_buf =
                        &ispdev->frame_buffer[k];
                if (!ispdev->b_set_out_addr)
                {
#ifdef FPGA_TEST
                    frm_buf->addr[0] = 0x60000;
                    frm_buf->size = stride * height;
#else
                    frm_buf->size = stride * height;
                    frm_buf->addr[0] = (unsigned int)rt_malloc(frm_buf->size);
#endif
                }
            }
        }
        else
        {
            ISP_DBG("no need to malloc for frame ping-pong buffer\n");
        }
    }

#if defined(RT_USING_CAMERA) && RT_USING_CAMERA_CMD
    struct rk_isp_subdev_ctrl ctrls[3];
    struct rk_isp_subdev_ext_ctrl exp_ctrl;
    ctrls[0].id = V4L2_CID_EXPOSURE;
    ctrls[0].val = p_scene[scene_idx].exp[0];
    ctrls[1].id = V4L2_CID_GAIN;
    ctrls[1].val = p_scene[scene_idx].exp[1];
    ctrls[2].id = RK_V4L2_CID_GAIN_PERCENT;
    ctrls[2].val = 100;
    ret = subdev->ops->control(subdev, RK_DEVICE_CTRL_CAMERA_SET_AEC, (void *)&exp_ctrl);
#endif

    rk_isp_function_exit();
    return RT_EOK;
}

/**
* @brief  Open the isp device for application.
* @param dev : The isp device.
* @param oflag : The flag of open.
*/
rt_err_t rk_isp_open(struct rk_isp_dev *dev, unsigned short oflag)
{
    struct rk_isp_device *ispdev;

    ispdev = (struct rk_isp_device *)dev->parent.user_data;

    if (!ispdev->mutex)
    {
        ispdev->mutex = rt_mutex_create("isp_mutex_lock",
                                        RT_IPC_FLAG_FIFO);
        if (!ispdev->mutex)
        {
            ISP_ERR("create queue queued mutex failed!\n");
            return -RT_ENOMEM;
        }
    }

    if (!ispdev->isp_dev.isp_mutex)
    {
        ispdev->isp_dev.isp_mutex = rt_mutex_create("isp_module_mutex_lock",
                                    RT_IPC_FLAG_FIFO);
        if (!ispdev->isp_dev.isp_mutex)
        {
            ISP_ERR("create queue queued mutex failed!\n");
            return -RT_ENOMEM;
        }
    }

    if (!ispdev->subdev_exps.mutex)
    {
        ispdev->subdev_exps.mutex = rt_mutex_create("isp_subdev_exps_mutex_lock",
                                    RT_IPC_FLAG_FIFO);
        if (!ispdev->subdev_exps.mutex)
        {
            ISP_ERR("create queue queued mutex failed!\n");
            return -RT_ENOMEM;
        }
    }

    if (!event_isp)
    {
        event_isp = rt_event_create("event_isp", RT_IPC_FLAG_PRIO);

        if (event_isp != RT_NULL)
        {
            ISP_DBG("RT-Thread create event successful\r\n");
        }
        else
        {
            ISP_ERR("RT-Thread create event failed\r\n");
            return -RT_ENOMEM;
        }
    }

    ispdev->frame_id = 0;

    rt_do_rkisp_open(&ispdev->isp_dev);

    return RT_EOK;
}

/**
* @brief  Close the isp device for application.
* @param dev : The isp device.
*/
rt_err_t rk_isp_close(struct rk_isp_dev *dev)
{
    struct isp_videobuf_queue *queue;
    struct rk_isp_device *ispdev;
    struct rt_device *dev_object;
    int ret;

    ispdev = (struct rk_isp_device *)dev->parent.user_data;

    dev_object = &ispdev->parent.parent;

#if defined(RT_USING_CAMERA)
    for (int i = 0; i < ispdev->subdev_cnt; i++)
        ispdev->subdev_array[i - 1]->ops->close(ispdev->subdev_array[i - 1]);
#endif

    rt_do_rkisp_release(ispdev, ispdev->rk_isp_data_path);

    if (ret)
    {
        ISP_ERR("rk isp data path is streaming, can not close\n");
        return RT_ERROR;
    }

    if (dev_object->open_flag & RT_DEVICE_OFLAG_OPEN)
    {
        if (ispdev->mutex)
        {
            rt_mutex_delete(ispdev->mutex);
            ispdev->mutex = RT_NULL;
        }

        if (ispdev->isp_dev.isp_mutex)
        {
            rt_mutex_delete(ispdev->isp_dev.isp_mutex);
            ispdev->isp_dev.isp_mutex = RT_NULL;
        }

        if (ispdev->subdev_exps.mutex)
        {
            rt_mutex_delete(ispdev->subdev_exps.mutex);
            ispdev->isp_dev.isp_mutex = RT_NULL;
        }
    }
    else
    {
        ISP_ERR("Warning: isp maybe has been closed!\n");
        return RT_ENOSYS;
    }

    if (event_isp)
    {
        rt_event_delete(event_isp);
        event_isp = RT_NULL;
    }

    rt_do_rkisp_destroy(ispdev);

    return RT_EOK;

}

rt_err_t rk_isp_control(struct rk_isp_dev *dev, int cmd, void *args)
{
    struct rk_isp_device *isp = rk_get_isp_dev(dev);
    int ret;
    int *data = (int *)args;
    struct rk_isp_set_addr *set_addr;
    struct set_ctrl *ctrl;

    switch (cmd)
    {
    case    RK_ISP_CMD_DEVICE_INIT:
    {
        ret = rk_isp_init(dev);
    }
    break;

    case    RK_ISP_CMD_STREAM_ON:
    {
        ret = rt_do_rkisp_streamon(isp);
    }
    break;

    case    RK_ISP_CMD_STREAM_OFF:
    {
        ret = rt_do_rkisp_streamoff(isp);
    }
    break;

    case    RK_ISP_CMD_G_DPCC:
    case    RK_ISP_CMD_S_DPCC:
    case    RK_ISP_CMD_G_BLS:
    case    RK_ISP_CMD_S_BLS:
    case    RK_ISP_CMD_G_SDG:
    case    RK_ISP_CMD_S_SDG:
    case    RK_ISP_CMD_G_LSC:
    case    RK_ISP_CMD_S_LSC:
    case    RK_ISP_CMD_G_AWB_MEAS:
    case    RK_ISP_CMD_S_AWB_MEAS:
    case    RK_ISP_CMD_G_FLT:
    case    RK_ISP_CMD_S_FLT:
    case    RK_ISP_CMD_G_BDM:
    case    RK_ISP_CMD_S_BDM:
    case    RK_ISP_CMD_G_CTK:
    case    RK_ISP_CMD_S_CTK:
    case    RK_ISP_CMD_G_GOC:
    case    RK_ISP_CMD_S_GOC:
    case    RK_ISP_CMD_G_HST:
    case    RK_ISP_CMD_S_HST:
    case    RK_ISP_CMD_G_AEC:
    case    RK_ISP_CMD_S_AEC:
    case    RK_ISP_CMD_G_BPL:
    case    RK_ISP_CMD_G_AWB_GAIN:
    case    RK_ISP_CMD_S_AWB_GAIN:
    case    RK_ISP_CMD_G_CPROC:
    case    RK_ISP_CMD_S_CPROC:
    case    RK_ISP_CMD_G_AFC:
    case    RK_ISP_CMD_S_AFC:
    case    RK_ISP_CMD_G_IE:
    case    RK_ISP_CMD_S_IE:
    case    RK_ISP_CMD_G_DPF:
    case    RK_ISP_CMD_S_DPF:
    case    RK_ISP_CMD_G_DPF_STRENGTH:
    case    RK_ISP_CMD_S_DPF_STRENGTH:
    case    RK_ISP_CMD_G_LAST_CONFIG:
    case    RK_ISP_CMD_S_WDR:
    case    RK_ISP_CMD_G_WDR:
    {
        ret = rt_do_cmd_ctrl_rkisp_module(&isp->isp_dev, cmd, args);
    }
    break;

    case    RK_ISP_SET_MOD_EN:
        ctrl = (struct set_ctrl *)args;
        ret = rt_do_rkisp_set_mod(&isp->isp_dev, &ctrl->value, ctrl->id);
        break;

    case    RK_ISP_GET_MOD_EN:
        ctrl = (struct set_ctrl *)args;
        ret = rt_do_rkisp_get_mod(&isp->isp_dev, &ctrl->value, ctrl->id);
        break;

    case RK_ISP_CMD_G_MEAS_BUF:
        rt_do_get_meas_data_buf(&isp->isp_dev, args);
        ret = 0;
        break;

    case RK_ISP_CMD_SUPER_IMPOSE:
        isp->config.isp_config.si_enable = data[0];
        ret = 0;
        break;
    case RK_ISP_CMD_IMAGE_EFFECT:
        if ((unsigned int)data[0] > RK_ISP_IE_NONE)
        {
            ISP_ERR("unknown/unsupported image effect %d\n", data[0]);
            return -RT_EINVAL;
        }
        isp->config.isp_config.ie_config.effect = data[0];
        ret = 0;
        break;
    case RK_ISP_CMD_SET_ISP_INIT_INFO:
        isp->isp_init = (struct isp_init_info *)args;
        ISP_DBG("isp config init param success\n");
        ret = 0;
        break;

    case RK_ISP_CMD_SET_OUT_ADDR:
        set_addr = (struct rk_isp_set_addr *)args;
        isp->b_set_out_addr = true;
        if (set_addr->b_set_uv_flag)
        {
            isp->b_set_uv_flag = true;
            isp->frame_buffer[0].addr[0] = set_addr->y_addr;
            isp->frame_buffer[0].addr[1] = set_addr->u_addr;
            isp->frame_buffer[0].addr[2] = set_addr->v_addr;
            isp->frame_buffer[1].addr[0] = isp->frame_buffer[0].addr[0];
            isp->frame_buffer[1].addr[1] = isp->frame_buffer[0].addr[1];
            isp->frame_buffer[1].addr[2] = isp->frame_buffer[0].addr[2];
        }
        else
        {
            isp->frame_buffer[0].addr[0] = set_addr->y_addr;
            isp->frame_buffer[1].addr[0] = set_addr->y_addr;
        }
        break;

    case RK_ISP_CMD_SET_OUT_PING_PONG_ADDR:
        set_addr = (struct rk_isp_set_addr *)args;
        isp->b_set_out_addr = true;
        if (set_addr->b_set_uv_flag)
        {
            isp->b_set_uv_flag = true;
            isp->frame_buffer[0].addr[0] = set_addr[0].y_addr;
            isp->frame_buffer[0].addr[1] = set_addr[0].u_addr;
            isp->frame_buffer[0].addr[2] = set_addr[0].v_addr;

            isp->frame_buffer[1].addr[0] = set_addr[1].y_addr;
            isp->frame_buffer[1].addr[1] = set_addr[1].u_addr;
            isp->frame_buffer[1].addr[2] = set_addr[1].v_addr;
        }
        else
        {
            isp->frame_buffer[0].addr[0] = set_addr[0].y_addr;
            isp->frame_buffer[1].addr[0] = set_addr[1].v_addr;
        }
        break;
    case RK_ISP_GET_OUT_SIZE:
        data[0] = isp->config.mi_config.data_path.output.width;
        data[1] = isp->config.mi_config.data_path.output.height;
        break;
    case RK_ISP_SET_IMG_EFF_RGB2GREY:
        isp->b_set_eff_en_flag = true;
        break;
    case RK_ISP_SET_IMG_EFF_GREY2RGB:
        isp->b_set_eff_clr_flag = true;
        break;
    case RK_ISP_SET_HUE:
        ret = READ_RKISP_REG(RK_C_PROC_CTRL) & 1;
        if (ret)
        {
            uint32_t ulFix;
            // precision is 1, thus no multiplication is required
            float fFloat = ((float *)args)[0];
            //RT_ASSERT(fFloat <= 87.0);
            // RT_ASSERT(fFloat >= -89.5);
            // round, two's complement if negative
            if (fFloat > 0.0f)
            {
                fFloat *= 127.0 / 90.0;
                ulFix = (uint32_t)(fFloat + 0.5f);
            }
            else
            {
                fFloat = -fFloat;
                fFloat *= 128.0 / 90.0;
                ulFix  = (uint32_t)(fFloat + 0.5f);
                ulFix  = ~ulFix;
                ulFix++;
            }

            // set upper (unused) bits to 0
            ulFix &= UTL_FIX_MASK_S0800;
            WRITE_RKISP_REG(ulFix, RK_C_PROC_HUE);

            ret = 0;
        }
        else
            ret = 1;
        break;
    case RK_ISP_SET_BRIGHTNESS:
        ret = READ_RKISP_REG(RK_C_PROC_CTRL) & 1;
        if (ret)
        {
            float fFloat = ((float *)args)[0];
            CLIP(fFloat, UTL_FIX_MIN_S0800, UTL_FIX_MAX_S0800);
            uint32_t ulFix;

            // round, two's complement if negative
            if (fFloat > 0.0f)
            {
                ulFix = (uint32_t)(fFloat + 0.5f);
            }
            else
            {
                fFloat = -fFloat;
                ulFix  = (uint32_t)(fFloat + 0.5f);
                ulFix  = ~ulFix;
                ulFix++;
            }

            ISP_DBG("amcap set brightness is %d\n", ulFix);
            // set upper (unused) bits to 0
            ulFix &= UTL_FIX_MASK_S0800;
            WRITE_RKISP_REG(ulFix, RK_C_PROC_BRIGHTNESS);
            ret = 0;
        }
        else
            ret = 1;
        break;
    case RK_ISP_SET_SAT:
        ret = READ_RKISP_REG(RK_C_PROC_CTRL) & 1;
        if (ret)
        {
            float fFloat = ((float *)args)[0];
            CLIP(fFloat, UTL_FIX_MIN_U0107, UTL_FIX_MAX_U0107);
            fFloat *= UTL_FIX_PRECISION_U0107;

            uint32_t ulFix = (uint32_t)(fFloat + 0.5f);

            ISP_DBG("set isp_sat val is %d\n", ulFix);
            if (ulFix > 255)
                ulFix = 255;
            WRITE_RKISP_REG(ulFix, RK_C_PROC_SATURATION);
            ret = 0;
        }
        else
            ret = 1;
        break;
    case RK_ISP_SET_CONTRAST:
        ret = READ_RKISP_REG(RK_C_PROC_CTRL) & 1;
        if (ret)
        {
            float fFloat = ((float *)args)[0];
            CLIP(fFloat, UTL_FIX_MIN_U0107, UTL_FIX_MAX_U0107);
            fFloat *= UTL_FIX_PRECISION_U0107;

            uint32_t ulFix = (uint32_t)(fFloat + 0.5f);
            ISP_DBG("set isp_contrast val is %d\n", ulFix);
            if (ulFix > 255)
                ulFix = 255;
            WRITE_RKISP_REG(ulFix, RK_C_PROC_CONTRAST);
            ret = 0;
        }
        else
            ret = 1;
        break;
    default:
    {
        ret = RT_ENOSYS;
    }
    break;
    }

    return ret;
}

const static struct rk_isp_ops rk_isp_ops =
{
    rk_isp_init,
    rk_isp_open,
    rk_isp_close,
    rk_isp_control,
    rk_isp_rx_indicate
};

static struct rk_isp_irq rkisp_irq_set[RK_ISP_IRQ_MAX] =
{
    [RK_ISP_IRQ_ISP] = {
        .name = "isp_irq_isp",
        .irq_num = ISP_IRQn,
        .irq_handler = rt_do_handle_rkisp_inter,
    }
};

static rt_err_t rk_isp_register(struct rk_isp_device *dev, char *name)
{
    unsigned int index;
    struct rk_isp_dev *isp;
    struct rk_isp_irq *irq_set;

    rk_isp_function_enter();

    RT_ASSERT(dev != RT_NULL);
    RT_ASSERT(name != RT_NULL);

    isp = &dev->parent;
    isp->ops = &rk_isp_ops;
    irq_set = dev->irq_set;

    for (index = 0; index < RK_ISP_IRQ_MAX; index++)
    {
        if (irq_set[index].irq_handler != RT_NULL)
        {
            ISP_DBG("interrupt_install %s, vector is %d, install cntxt is 0x%08x\n", irq_set[index].name, irq_set[index].irq_num, dev);
            rt_hw_interrupt_install(irq_set[index].irq_num,
                                    irq_set[index].irq_handler,
                                    (void *)dev,
                                    irq_set[index].name);
        }
    }
    rk_isp_function_exit();

    return rk_isp_controller_register(isp, name, dev);
}


/**
* @brief  The probe function for registering isp device.
*/
int rk_device_isp_init(void)
{
    rt_err_t ret;
    ISP_DBG("rk_isp_driver in %s start\n", __FUNCTION__);

    if (!g_rk_isp_instance)
        g_rk_isp_instance = (struct rk_isp_device *)rt_malloc(sizeof(struct rk_isp_device));
    RT_ASSERT(g_rk_isp_instance != RT_NULL);

    struct rk_isp_device *rk_ispdev = g_rk_isp_instance;

    rt_memset(rk_ispdev, 0, sizeof(struct rk_isp_device));

    rt_strncpy(rk_ispdev->name, RK_ISP_DEVICE_NAME,
               RK_ISP_DEVICE_NAME_SIZE);

    rk_ispdev->isp_clk_set.p_hclk_isp_gate = get_clk_gate_from_id(HCLK_ISP_GATE);
    rk_ispdev->isp_clk_set.p_aclk_isp_gate = get_clk_gate_from_id(ACLK_ISP_GATE);
    rk_ispdev->isp_clk_set.p_pll_isp_gate = get_clk_gate_from_id(CLK_ISP_PLL_GATE);
    rk_ispdev->isp_clk_set.clk_en_flag = false;
    rk_ispdev->irq_set = rkisp_irq_set;

    ret = rk_isp_register(rk_ispdev, rk_ispdev->name);

    ISP_DBG("rk_isp_driver in %s end, struct device addr is 0x%08x\n", __FUNCTION__, rk_ispdev);
    return ret;
}

/** @} */  // ISP_Public_Functions

INIT_DEVICE_EXPORT(rk_device_isp_init);

#endif

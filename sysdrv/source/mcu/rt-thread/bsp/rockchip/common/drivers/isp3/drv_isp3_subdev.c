/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_isp3_subdev.c
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */
#include "drv_isp3_subdev.h"
#include "regs_v3x.h"
#include "drv_isp3.h"

#if defined(RT_USING_ISP3)

#define MIPI_LVDS_DEBUG                       0
#define MIPI_LVDS_DEBUG_REGISTER              0

#define MIPI_LVDS_INFO(fmt, args...)         rt_kprintf("[ISP]: %s "fmt"", __FUNCTION__, ##args)
#define MIPI_LVDS_ERR(fmt, args...)         rt_kprintf("[ISP]: %s ERR "fmt"", __FUNCTION__, ##args)

#if MIPI_LVDS_DEBUG
#include <stdio.h>
#define MIPI_LVDS_DBG(...)                \
            do { \
                rt_kprintf("[ISP]:");    \
                rt_kprintf(__VA_ARGS__);    \
                }while(0)
#else
#define MIPI_LVDS_DBG(...)
#endif

struct dphy_param
{
    uint32_t data_rate_mbps;   /* mhz/lane = mbps/lane div 2 */
    uint8_t hs_settle;
};

#if TEST_FPGA_MIPI_DPHY_ENABLE

static const struct dphy_param fpga_mipidphy_hsfreq_ranges[] =
{
    {  89, 0x00}, {  99, 0x10}, { 109, 0x20}, { 129, 0x01},
    { 139, 0x11}, { 149, 0x21}, { 169, 0x02}, { 179, 0x12},
    { 199, 0x22}, { 219, 0x03}, { 239, 0x13}, { 249, 0x23},
    { 269, 0x04}, { 299, 0x14}, { 329, 0x05}, { 359, 0x15},
    { 399, 0x25}, { 449, 0x06}, { 499, 0x16}, { 549, 0x07},
    { 599, 0x17}, { 649, 0x08}, { 699, 0x18}, { 749, 0x09},
    { 799, 0x19}, { 849, 0x29}, { 899, 0x39}, { 949, 0x0a},
    { 999, 0x1a}, {1049, 0x2a}, {1099, 0x3a}, {1149, 0x0b},
    {1199, 0x1b}, {1249, 0x2b}, {1299, 0x3b}, {1349, 0x0c},
    {1399, 0x1c}, {1449, 0x2c}, {1500, 0x3c}
};

static void mipidphy_fpga_write(uint32_t dphy_base, uint8_t addr, uint8_t data)
{
    writel(0x01000100, dphy_base + 0x20);/* TESTCLK=1 */
    writel(0x00ff0000 | addr, dphy_base + 0x20); /* TESTDIN=addr */
    writel(0x02000200, dphy_base + 0x20);/* TESTEN =1 */
    writel(0x01000000, dphy_base + 0x20);/* TESTCLK=0 */
    writel(0x02000000, dphy_base + 0x20);/* TESTEN =0 */
    writel(0x01000000, dphy_base + 0x20);/* TESTCLK=0 */
    writel(0x00ff0000 | data, dphy_base + 0x20);/* TESTDIN=data */
    writel(0x01000100, dphy_base + 0x20);/* TESTCLK=1 */

}

static uint8_t mipidphy_fpga_read(uint32_t dphy_base, uint8_t addr)
{
    uint8_t data;

    writel(0x02ff0200 | addr, dphy_base + 0x20); /* TESTEN =1,TESTDIN=addr */

    data = readl(dphy_base + 0x18);
    writel(0x03000100, dphy_base + 0x20);/* TESTEN =0,TESETCLK=1 */

    return data;
}

static void _delay_()
{
    int i, j;
    for (i = 0; i < 100; i++)
    {
        for (j = 0; j < 100;)
            j++;
    }
}

void mipidphy_fpga_stream_on(struct rk_isp_dev *dev)
{
    uint32_t index, dphy_base, lane_num, len, data_rate_mbps;
    struct rk_camera_info *cam_info;
    struct rk_camera_mbus_framefmt *mbus_fmt;
    struct rk_camera_mbus_config *mbus_config;

    dphy_base = (uint32_t)dev->mipi_dphy;
    cam_info = &dev->input.cam_info;
    mbus_fmt = &cam_info->mbus_fmt;
    mbus_config = &cam_info->mbus_config;
    /* reset */
    writel(0x04000000, dphy_base + 0x20);   /* RSTZ */
    writel(0x08000000, dphy_base + 0x20);   /* SHUTDOWNZ */
    _delay_();
    lane_num = mbus_config->flags & MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_MASK;
    writel(0x01000100, dphy_base + 0x20);   /* TESTCLK=1 */
    writel(0x80008000, dphy_base + 0x20);   /* TESTCLR=1 */
    writel(0x80000000, dphy_base + 0x20);   /* TESTCLR=0 */
    writel(0x40004000, dphy_base + 0x20);   /* RSTZCAL=1 */
    writel(0x08000800, dphy_base + 0x20);   /* SHUTDOWNZ=1 */
    writel(0x04000400, dphy_base + 0x20);   /* RSTZ     =1 */
    mipidphy_fpga_write(dphy_base, 0xAC, lane_num); /* lane */

    len = sizeof(fpga_mipidphy_hsfreq_ranges) / sizeof(fpga_mipidphy_hsfreq_ranges[0]);
    data_rate_mbps = roundup(mbus_config->linked_freq, 1000000) << 1;
    for (index = 0; index < len; index++)
    {
        if (data_rate_mbps <= fpga_mipidphy_hsfreq_ranges[index].data_rate_mbps)
            break;
    }

    if (index == len - 1)
    {
        index = len - 2;
        MIPI_LVDS_ERR("can't match data rate:%d mbps", data_rate_mbps);
    }

    mipidphy_fpga_write(dphy_base, 0x34,
                        fpga_mipidphy_hsfreq_ranges[index].hs_settle);

    mipidphy_fpga_read(dphy_base, 0x0); /* Normal operation */
    writel(0x03000100, dphy_base + 0x20); /* TESTEN   =0,TESETCLK=1 */
    if (lane_num == MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_1)
        writel(0x30000000, dphy_base + 0x20);/* ENABLE_0=0,ENABLE_1=0 */
    else if (lane_num == MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_2)
        writel(0x30002000, dphy_base + 0x20);/* ENABLE_0=1,ENABLE_1=0 */
    else
        writel(0x30003000, dphy_base + 0x20);/* ENABLE_0=1,ENABLE_1=1 */

    writel(0x08000800, dphy_base + 0x20);   /* SHUTDOWNZ=1 */
    writel(0x04000400, dphy_base + 0x20);   /* RSTZ     =1 */

}

#else

static const struct dphy_param rv1109_dphy_param[] =
{
    {110000000, 0x02},
    {150000000, 0x03},
    {200000000, 0x06},
    {250000000, 0x06},
    {300000000, 0x06},
    {400000000, 0x08},
    {500000000, 0x0b},
    {600000000, 0x0e},
    {700000000, 0x10},
    {800000000, 0x12},
    {1000000000, 0x16},
    {1200000000, 0x1e},
    {1400000000, 0x23},
    {1600000000, 0x2d},
    {1800000000, 0x32},
    {2000000000, 0x37},
    {2200000000, 0x3c},
    {2400000000, 0x41},
    {2500000000, 0x46},
    {0xffff, 0xff},
};

int rk_isp_hw_mipi_lvds_dphy_init(struct rk_isp_dev *dev)
{
    struct rk_camera_info *cam_info;
    struct rk_camera_mbus_config *mbus_config;
    uint32_t mask, dphy_base, lanes_en = 0, data_rate_mbps,
                              grf_lanes_en = 0, lvds_mode = 0;
    uint8_t index, len;
    bool dual_cam_en;
    struct dphy_board_desc *csi2_dphy = dev->csi2_dphy_board;

    RT_ASSERT(dev != RT_NULL);

    if (!csi2_dphy->enable)
    {
        MIPI_LVDS_ERR("dphy is not enable\n");
        return -RT_ERROR;
    }

    dphy_base = csi2_dphy->reg;
    cam_info = &dev->input.cam_info;
    mbus_config = &cam_info->mbus_config;

    /* mipi-dphy related settting */
    len = sizeof(rv1109_dphy_param) / sizeof(rv1109_dphy_param[0]);
    data_rate_mbps = roundup(mbus_config->linked_freq, 1000000) << 1;
    for (index = 0; index < len; index++)
    {
        if (data_rate_mbps <= rv1109_dphy_param[index].data_rate_mbps)
            break;
    }

    if (index == len - 1)
    {
        index = len - 2;
        MIPI_LVDS_ERR("can't match data rate: %dmbps, use %dmbps as default", data_rate_mbps,
                      rv1109_dphy_param[index].data_rate_mbps);
    }

    if (csi2_dphy->csi2_dphy1.enable && csi2_dphy->csi2_dphy2.enable)
        dual_cam_en = 1;
    else
        dual_cam_en = 0;

    if (dual_cam_en)
    {
        /* split mode */
        RKISP_WRITE(dphy_base + CSI2_DPHY_DUAL_CAL_EN, 0x5e);
        RKISP_WRITE(dphy_base + CSI2_DPHY_DUAL_CAL_EN, 0x5f);
    }
    else
    {
        RKISP_WRITE(dphy_base + CSI2_DPHY_DUAL_CAL_EN, 0x1e);
        RKISP_WRITE(dphy_base + CSI2_DPHY_DUAL_CAL_EN, 0x1f);
    }

    grf_lanes_en = CSIPHY_CLKLANE_ENABLE;
    RKISP_WRITE(dphy_base + CSI2_DPHY_CLK_WR_THS_SETTLE, rv1109_dphy_param[index].hs_settle);

    if (csi2_dphy->csi2_dphy1.enable)
    {
        if (csi2_dphy->csi2_dphy1.data_lanes >= 1)
        {
            lanes_en |= DPHYRX_LANE_EN_LANE0;
            grf_lanes_en |= CSIPHY_LANE_0_ENABLE;
            RKISP_WRITE(dphy_base + CSI2_DPHY_LANE0_WR_THS_SETTLE, rv1109_dphy_param[index].hs_settle);
        }
        if (csi2_dphy->csi2_dphy1.data_lanes >= 2)
        {
            lanes_en |= DPHYRX_LANE_EN_LANE1;
            grf_lanes_en |= CSIPHY_LANE_1_ENABLE;
            RKISP_WRITE(dphy_base + CSI2_DPHY_LANE1_WR_THS_SETTLE, rv1109_dphy_param[index].hs_settle);
        }
    }
    if (csi2_dphy->csi2_dphy2.enable)
    {
        if (csi2_dphy->csi2_dphy2.data_lanes >= 1)
        {
            lanes_en |= DPHYRX_LANE_EN_LANE2;
            grf_lanes_en |= CSIPHY_LANE_2_ENABLE;
            RKISP_WRITE(dphy_base + CSI2_DPHY_LANE2_WR_THS_SETTLE, rv1109_dphy_param[index].hs_settle);
        }
        if (csi2_dphy->csi2_dphy2.data_lanes >= 2)
        {
            lanes_en |= DPHYRX_LANE_EN_LANE3;
            grf_lanes_en |= CSIPHY_LANE_3_ENABLE;
            RKISP_WRITE(dphy_base + CSI2_DPHY_LANE3_WR_THS_SETTLE, rv1109_dphy_param[index].hs_settle);
        }
    }
    lanes_en |= 0x41;
    RKISP_WRITE(dphy_base + CSI2_DPHY_CTRL_LANE_ENABLE, lanes_en);

    if (mbus_config->mbus_type == CAMERA_MBUS_CSI2_DPHY)
    {
        /* when use mipi, do not write DPHYRX_MIPI_LVDS_ENABLE */
        RKISP_WRITE(dphy_base + CSI2_DPHY_PATH0_MODE_SEL, 0x02);
        if (dual_cam_en)
            /* split_mode */
            RKISP_WRITE(dphy_base + CSI2_DPHY_CLK_INV, 0x14);
        else
            RKISP_WRITE(dphy_base + CSI2_DPHY_CLK_INV, 0x04);
    }
    else if (mbus_config->mbus_type == CAMERA_MBUS_LVDS_DPHY)
    {
        RKISP_WRITE(dphy_base + CSI2_DPHY_PATH0_MODE_SEL, 0x04);

        mask = mbus_config->flags & MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_MASK;
        if (mask == MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_8BITS)
        {
            lvds_mode = DPHYRX_LVDS_MODE_8BITS;
        }
        else if (mask == MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_10BITS)
        {
            lvds_mode = DPHYRX_LVDS_MODE_10BITS;
        }
        else if (mask == MEDIA_BUS_FLAGS_CSI2_LVDS_BITS_MODE_12BITS)
        {
            lvds_mode = DPHYRX_LVDS_MODE_12BITS;
        }
        lvds_mode |= DPHYRX_LVDS_MODE_ENABLE;
        RKISP_WRITE(dphy_base + CSI2_DPHY_PATH0_LVDS_MODE_SEL, lvds_mode);

    }

    mask = mbus_config->flags & MEDIA_BUS_FLAGS_CSI2_LVDS_CLOCK_MODE_MASK;
    if (mask)
    {
        /* continue */
        RKISP_WRITE(dphy_base + CSI2_DPHY_CLK_MODE, 0x1);
    }
    else
    {
        /* non-continue */
        RKISP_WRITE(dphy_base + CSI2_DPHY_CLK_MODE, 0x0);
    }

    RKISP_WRITE(VI_CSIPHY_CON5, grf_lanes_en);
#if MIPI_LVDS_DEBUG_REGISTER
    rk_mipi_lvds_dphy_dump_register(dev);
#endif

    return RT_EOK;
}
#endif

int rk_isp_hw_mipi_lvds_dphy_reinit(struct rk_isp_dev *dev)
{
    uint32_t dphy_base;
    struct dphy_board_desc *csi2_dphy = dev->csi2_dphy_board;

    dphy_base = (uint32_t)csi2_dphy->reg;
    RKISP_WRITE(dphy_base + CSI2_DPHY_CTRL_LANE_ENABLE, 0x01);

    return RT_EOK;
}

int rk_isp_hw_csi_host_init(struct rk_isp_dev *dev)
{
    struct rk_camera_info *cam_info;
    struct rk_camera_mbus_config *mbus_config;
    uint32_t lane_num, csi_host0_base, csi_host1_base;
    struct csi2_board_desc *csi_host0 = (struct csi2_board_desc *)&csi2_host0_board;
    struct csi2_board_desc *csi_host1 = (struct csi2_board_desc *)&csi2_host1_board;

    RT_ASSERT(dev != RT_NULL);

    csi_host0_base = csi_host0->reg;
    csi_host1_base = csi_host1->reg;
    cam_info = &dev->input.cam_info;
    mbus_config = &cam_info->mbus_config;

    lane_num = mbus_config->flags & MEDIA_BUS_FLAGS_CSI2_LVDS_LANES_MASK;

    if (csi_host0->enable)
    {
        RKISP_WRITE(csi_host0_base + CSIHOST_N_LANES, lane_num);
        RKISP_WRITE(csi_host0_base + CSIHOST_CONTROL, 0x0c204000);
        RKISP_WRITE(csi_host0_base + CSIHOST_RESETN, 0x1);
    }
    if (csi_host1->enable)
    {
        RKISP_WRITE(csi_host1_base + CSIHOST_N_LANES, lane_num);
        RKISP_WRITE(csi_host1_base + CSIHOST_CONTROL, 0x0c204000);
        RKISP_WRITE(csi_host1_base + CSIHOST_RESETN, 0x1);
    }

    return RT_EOK;
}

int rk_isp_hw_csi_host_reinit(struct rk_isp_dev *dev)
{
    uint32_t csi_host0_base, csi_host1_base;
    struct csi2_board_desc *csi_host0 = (struct csi2_board_desc *)&csi2_host0_board;
    struct csi2_board_desc *csi_host1 = (struct csi2_board_desc *)&csi2_host1_board;

    csi_host0_base = csi_host0->reg;
    csi_host1_base = csi_host1->reg;
    if (csi_host0->enable)
    {
        RKISP_READ(csi_host0_base + CSIHOST_ERR1);
        RKISP_READ(csi_host0_base + CSIHOST_ERR2);
        RKISP_WRITE(csi_host0_base + CSIHOST_RESETN, 0x0);
    }
    if (csi_host1->enable)
    {
        RKISP_READ(csi_host1_base + CSIHOST_ERR1);
        RKISP_READ(csi_host1_base + CSIHOST_ERR2);
        RKISP_WRITE(csi_host1_base + CSIHOST_RESETN, 0x0);
    }

    return RT_EOK;
}

int rk_isp_hw_vicap_init(struct rk_isp_dev *dev)
{
    struct isp_output_info *output_info;
    uint32_t width, height, val, vicap_base;
    struct rk_camera_mbus_config *mbus_config;
    struct isp_input_info *input_info;
    struct isp_input_fmt *input_fmt;
    struct rk_camera_info *cam_info;
    struct vicap_board_desc *vicap = dev->vicap_board;
    struct csi2_board_desc *csi_host0 = dev->csi2_host0_board;
    struct csi2_board_desc *csi_host1 = dev->csi2_host1_board;

    RT_ASSERT(dev != RT_NULL);

    vicap_base = vicap->reg;
    output_info = &dev->output;
    input_info = &dev->input;
    input_fmt = &input_info->input_fmt;
    cam_info = &input_info->cam_info;
    mbus_config = &cam_info->mbus_config;

    if (output_info->is_crop)
    {
        width = output_info->crop.width;
        height = output_info->crop.height;
    }
    else
    {
        width = output_info->pix_format.width;
        height = output_info->pix_format.height;
    }

    if (mbus_config->mbus_type == CAMERA_MBUS_CSI2_DPHY)
    {
        val = VICAP_MIPI0_ID0_CTRL0_SW_CAP_EN_ID0_MASK |
              VICAP_MIPI0_ID0_CTRL0_SW_CROP_EN_ID0_MASK;
        switch (input_fmt->mbus_code)
        {
        case MEDIA_BUS_FMT_SBGGR8_1X8:
        case MEDIA_BUS_FMT_SGRBG8_1X8:
        case MEDIA_BUS_FMT_SGBRG8_1X8:
        case MEDIA_BUS_FMT_SRGGB8_1X8:
            val |= (0 << VICAP_MIPI0_ID0_CTRL0_SW_PARSE_TYPE_ID0_SHIFT);
            val |= (0x2a << VICAP_MIPI0_ID0_CTRL0_SW_DT_ID0_SHIFT);
            break;
        case MEDIA_BUS_FMT_SBGGR10_1X10:
        case MEDIA_BUS_FMT_SGBRG10_1X10:
        case MEDIA_BUS_FMT_SRGGB10_1X10:
        case MEDIA_BUS_FMT_SGRBG10_1X10:
            val |= 1 << VICAP_MIPI0_ID0_CTRL0_SW_PARSE_TYPE_ID0_SHIFT;
            val |= (0x2b << VICAP_MIPI0_ID0_CTRL0_SW_DT_ID0_SHIFT);
            break;
        case MEDIA_BUS_FMT_SGRBG12_1X12:
        case MEDIA_BUS_FMT_SBGGR12_1X12:
        case MEDIA_BUS_FMT_SGBRG12_1X12:
        case MEDIA_BUS_FMT_SRGGB12_1X12:
            val |= (2 << VICAP_MIPI0_ID0_CTRL0_SW_PARSE_TYPE_ID0_SHIFT);
            val |= (0x2c << VICAP_MIPI0_ID0_CTRL0_SW_DT_ID0_SHIFT);
            break;
        default:
            break;
        }
        if (csi_host0->enable)
            RKISP_WRITE(vicap_base + CSI_MIPI0_ID0_CTRL0, val);//0xac13
        if (csi_host1->enable)
            RKISP_WRITE(vicap_base + CSI_MIPI1_ID0_CTRL0, val);//0xac13
    }
    if (csi_host0->enable)
    {
        RKISP_WRITE(vicap_base + CSI_MIPI0_ID0_CTRL1, (height << 16) | width);
        RKISP_WRITE(vicap_base + CSI_MIPI0_CTRL, 0x7075);
        RKISP_WRITE(vicap_base + TOISP0_CH_CTRL, VICAP_TOISP0_SEL_MIPI0);
    }
    if (csi_host1->enable)
    {
        RKISP_WRITE(vicap_base + CSI_MIPI1_ID0_CTRL1, (height << 16) | width);
        RKISP_WRITE(vicap_base + CSI_MIPI1_CTRL, 0x7075);
        RKISP_WRITE(vicap_base + TOISP0_CH_CTRL, VICAP_TOISP0_SEL_MIPI1);
    }

    RKISP_WRITE(vicap_base + TOISP0_CROP_SIZE, (height << 16) | width);

    return RT_EOK;
}

int rk_isp_hw_vicap_reinit(struct rk_isp_dev *dev)
{
    RKISP_WRITE(CSI_MIPI1_CTRL, 0x7074);
    return RT_EOK;
}

#endif

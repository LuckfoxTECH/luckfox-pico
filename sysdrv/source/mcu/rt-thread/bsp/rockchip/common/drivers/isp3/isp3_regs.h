/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    isp3_regs.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */

#ifndef __ISP3_REGS_H__
#define __ISP3_REGS_H__

#if defined(RT_USING_ISP3)

#include "common.h"

/* Memory Base */
#define FPGA_MIPI_DPHY_BASE                                 (0xff490800U)
#define FPGA_MIPI_DPHY                                      ((struct DPHYRX_REG *) FPGA_MIPI_DPHY_BASE)

/* DPHYRX_LANE_EN */
#define DPHYRX_LANE_EN_LANE0_SHIFT                          (2)
#define DPHYRX_LANE_EN_LANE0                                (0x1 << DPHYRX_LANE_EN_LANE0_SHIFT)
#define DPHYRX_LANE_EN_LANE0_MASK                           (0x1 << DPHYRX_LANE_EN_LANE0_SHIFT)

#define DPHYRX_LANE_EN_LANE1_SHIFT                          (3)
#define DPHYRX_LANE_EN_LANE1                                (0x1 << DPHYRX_LANE_EN_LANE1_SHIFT)
#define DPHYRX_LANE_EN_LANE1_MASK                           (0x1 << DPHYRX_LANE_EN_LANE1_SHIFT)

#define DPHYRX_LANE_EN_LANE2_SHIFT                          (4)
#define DPHYRX_LANE_EN_LANE2                                (0x1 << DPHYRX_LANE_EN_LANE2_SHIFT)
#define DPHYRX_LANE_EN_LANE2_MASK                           (0x1 << DPHYRX_LANE_EN_LANE2_SHIFT)

#define DPHYRX_LANE_EN_LANE3_SHIFT                          (5)
#define DPHYRX_LANE_EN_LANE3                                (0x1 << DPHYRX_LANE_EN_LANE3_SHIFT)
#define DPHYRX_LANE_EN_LANE3_MASK                           (0x1 << DPHYRX_LANE_EN_LANE3_SHIFT)

#define DPHYRX_LANE_EN_CLK_SHIFT                            (6)
#define DPHYRX_LANE_EN_CLK                                  (0x1 << DPHYRX_LANE_EN_CLK_SHIFT)
#define DPHYRX_LANE_EN_CLK_MASK                             (0x1 << DPHYRX_LANE_EN_CLK_SHIFT)

/* DPHYRX_MIPI_LVDS_ENABLE */
#define DPHYRX_MIPI_LVDS_ENABLE_LVDS_SHIFT                  (5)
#define DPHYRX_MIPI_LVDS_ENABLE_LVDS                        (0x1 << DPHYRX_MIPI_LVDS_ENABLE_LVDS_SHIFT)
#define DPHYRX_MIPI_LVDS_ENABLE_LVDS_MASK                   (0x1 << DPHYRX_MIPI_LVDS_ENABLE_LVDS_SHIFT)

#define DPHYRX_MIPI_LVDS_ENABLE_MIPI_SHIFT                  (0)
#define DPHYRX_MIPI_LVDS_ENABLE_MIPI                        (0x1 << DPHYRX_MIPI_LVDS_ENABLE_MIPI_SHIFT)
#define DPHYRX_MIPI_LVDS_ENABLE_MIPI_MASK                   (0x1 << DPHYRX_MIPI_LVDS_ENABLE_MIPI_SHIFT)

/* DPHYRX_MIPI_LVDS_MODEL */
#define DPHYRX_MIPI_LVDS_MODEL_SHIFT                        (0)
#define DPHYRX_MIPI_LVDS_MODEL_LVDS                         (0x2 << DPHYRX_MIPI_LVDS_MODEL_SHIFT)
#define DPHYRX_MIPI_LVDS_MODEL_MIPI                         (0x1 << DPHYRX_MIPI_LVDS_MODEL_SHIFT)
#define DPHYRX_MIPI_LVDS_MODEL_MASK                         (0x3 << DPHYRX_MIPI_LVDS_MODEL_SHIFT)

/* DPHYRX_LVDS_MODE */
#define DPHYRX_LVDS_MODE_SHIFT                              (4)
#define DPHYRX_LVDS_MODE_8BITS                              (0x2 << DPHYRX_LVDS_MODE_SHIFT)
#define DPHYRX_LVDS_MODE_10BITS                             (0x0 << DPHYRX_LVDS_MODE_SHIFT)
#define DPHYRX_LVDS_MODE_12BITS                             (0x1 << DPHYRX_LVDS_MODE_SHIFT)
#define DPHYRX_LVDS_MODE_MASK                               (0x3 << DPHYRX_LVDS_MODE_SHIFT)

#define DPHYRX_LVDS_MODE_ENABLE_SHIFT                       (0x0)
#define DPHYRX_LVDS_MODE_ENABLE                             (0x1 << DPHYRX_LVDS_MODE_ENABLE_SHIFT)
#define DPHYRX_LVDS_MODE_DISABLE                            (0x0 << DPHYRX_LVDS_MODE_ENABLE_SHIFT)
#define DPHYRX_LVDS_MODE_ENABLE_MASK                        (0x1 << DPHYRX_LVDS_MODE_ENABLE_SHIFT)

/* CSI2RX_1C00_CTRL0 */
#define CSI2RX_SW_DMA_2FRM_MODE_SHIFT                       (12)
#define CSI2RX_SW_DMA_2FRM_MODE_NORMAL                      (0x0 << CSI2RX_SW_DMA_2FRM_MODE_SHIFT)
#define CSI2RX_SW_DMA_2FRM_MODE_2FRAMES                     (0x0 << CSI2RX_SW_DMA_2FRM_MODE_SHIFT)
#define CSI2RX_SW_DMA_2FRM_MODE_3FRAMES                     (0x0 << CSI2RX_SW_DMA_2FRM_MODE_SHIFT)
#define CSI2RX_SW_DMA_2FRM_MODE_MASK                        (0x3 << CSI2RX_SW_DMA_2FRM_MODE_SHIFT)

/* CSI2RX_1C00_CTRL1 */
#define CSI2RX_SW_IBUF_OP_MODE_SHIFT                        (8)
#define CSI2RX_SW_IBUF_OP_MODE_NORMAL                       (0x0 << CSI2RX_SW_IBUF_OP_MODE_SHIFT)

#define CSI2RX_SW_CSI_LANE_SHIFT                            (0)
#define CSI2RX_SW_CSI_LANE_1LANE                            (0x0 << CSI2RX_SW_CSI_LANE_SHIFT)
#define CSI2RX_SW_CSI_LANE_2LANE                            (0x1 << CSI2RX_SW_CSI_LANE_SHIFT)
#define CSI2RX_SW_CSI_LANE_3LANE                            (0x2 << CSI2RX_SW_CSI_LANE_SHIFT)
#define CSI2RX_SW_CSI_LANE_4LANE                            (0x3 << CSI2RX_SW_CSI_LANE_SHIFT)
#define CSI2RX_SW_CSI_LANE_MASK                             (0x3 << CSI2RX_SW_CSI_LANE_SHIFT)

/* CSI2RX_1C00_CTRL2 */
#define CSI2RX_ISP_LINECNT_INT_MAX_LINE                     (0x3fff)

/* CSI2RX_1C00_PHY_STATE_RO */
#define CSI2RX_STOPSTATE_MASK(lane_num)                     \
({\
  __IO uint32_t mask;\
    switch(lane_num)\
    {\
        case 0:\
        mask = 0x1;\
        break;\
        case 1:\
        mask = 0x3;\
        break;\
        case 2:\
        mask = 0x7;\
        break;\
        case 3:\
        mask = 0xf;\
        break;\
        default:\
        mask = 0x1;\
        break;\
    }\
    mask;\
})

#define CSI2RX_RO_PHY_STATE_SHIFT                           (0)
#define CSI2RX_RO_PHY_STATE_ULPSACTIVENOTCLK(val)           (((val) & 0x800) >> 11)
#define CSI2RX_RO_PHY_STATE_STOPSTATECLK(val)               (((val) & 0x400) >> 10)
#define CSI2RX_RO_PHY_STATE_RXULPSCLKNOT(val)               (((val) & 0x200) >> 9)
#define CSI2RX_RO_PHY_STATE_RXCLKACTIVEHS(val)              (((val) & 0x100) >> 8)
#define CSI2RX_RO_PHY_STATE_STOPSTATE(val, lane_num)        ((((val) & 0x0f0) >> 4) & CSI2RX_STOPSTATE_MASK(lane_num))
#define CSI2RX_RO_PHY_STATE_RXULPSESC(val, lane_num)        (((val) & 0x00f) >> 0)

#define CSI2RX_ERR_PHY_ALL_MASKED                           (0xF0FFFF)
#define CSI2RX_ERR_PACKET_ALL_MASKED                        (0xF1FFFFF)
#define CIS2RX_ERR_OVERFLOW_MASKED                          (0x7F7FF1)
#define CIS2RX_ERR_STAT_MASKED                              (0x7FFEFF7F)

/* ISP_TOP */
#define ISP_TOP_CTRL_ISP_MODE_SHIFT                                 (1)
#define ISP_TOP_CTRL_ISP_MODE_RAW_BT601                             (0x0 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_BT656_YUV_EMBEDDED_SYNC               (0x1 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_BT601_YUV_H_V_SYNC                    (0x2 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_BAYER_RGB_BT601                       (0x3 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_DATA_MODE                             (0x4 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_BAYER_RGB_BT656                       (0x5 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_RAW_BT656                             (0x6 << ISP_TOP_CTRL_ISP_MODE_SHIFT)
#define ISP_TOP_CTRL_ISP_MODE_MASK                                  (0x7 << ISP_TOP_CTRL_ISP_MODE_SHIFT)

#define ISP_TOP_ACQ_PROP_SAMPLE_EDGE_SHIFT                          (0)
#define ISP_TOP_ACQ_PROP_SAMPLE_EDGE_NEGATIVE                       (0x0 << ISP_TOP_ACQ_PROP_SAMPLE_EDGE_SHIFT)
#define ISP_TOP_ACQ_PROP_SAMPLE_EDGE_POSITIVE                       (0x1 << ISP_TOP_ACQ_PROP_SAMPLE_EDGE_SHIFT)
#define ISP_TOP_ACQ_PROP_SAMPLE_EDGE_MASK                           (0x1 << ISP_TOP_ACQ_PROP_SAMPLE_EDGE_SHIFT)

#define ISP_TOP_ACQ_PROP_HSYNC_POL_SHIFT                            (1)
#define ISP_TOP_ACQ_PROP_HSYNC_POL_HIGH                             (0x0 << ISP_TOP_ACQ_PROP_HSYNC_POL_SHIFT)
#define ISP_TOP_ACQ_PROP_HSYNC_POL_LOW                              (0x1 << ISP_TOP_ACQ_PROP_HSYNC_POL_SHIFT)
#define ISP_TOP_ACQ_PROP_HSYNC_POL_MASK                             (0x1 << ISP_TOP_ACQ_PROP_HSYNC_POL_SHIFT)

#define ISP_TOP_ACQ_PROP_VSYNC_POL_SHIFT                            (2)
#define ISP_TOP_ACQ_PROP_VSYNC_POL_HIGH                             (0x0 << ISP_TOP_ACQ_PROP_VSYNC_POL_SHIFT)
#define ISP_TOP_ACQ_PROP_VSYNC_POL_LOW                              (0x1 << ISP_TOP_ACQ_PROP_VSYNC_POL_SHIFT)
#define ISP_TOP_ACQ_PROP_VSYNC_POL_MASK                             (0x1 << ISP_TOP_ACQ_PROP_VSYNC_POL_SHIFT)

#define ISP_TOP_ACQ_PROP_BAYER_PATTERN_SHIFT                        (3)
#define ISP_TOP_ACQ_PROP_BAYER_PATTERN_RGGB                         (0x0 << ISP_TOP_ACQ_PROP_BAYER_PATTERN_SHIFT)
#define ISP_TOP_ACQ_PROP_BAYER_PATTERN_GRBG                         (0x1 << ISP_TOP_ACQ_PROP_BAYER_PATTERN_SHIFT)
#define ISP_TOP_ACQ_PROP_BAYER_PATTERN_GBRG                         (0x2 << ISP_TOP_ACQ_PROP_BAYER_PATTERN_SHIFT)
#define ISP_TOP_ACQ_PROP_BAYER_PATTERN_BGGR                         (0x3 << ISP_TOP_ACQ_PROP_BAYER_PATTERN_SHIFT)

#define ISP_TOP_ACQ_PROP_CCIR_SEQ_SHIFT                             (7)
#define ISP_TOP_ACQ_PROP_CCIR_SEQ_YUYV                              (0x0 << ISP_TOP_ACQ_PROP_CCIR_SEQ_SHIFT)
#define ISP_TOP_ACQ_PROP_CCIR_SEQ_YVYU                              (0x1 << ISP_TOP_ACQ_PROP_CCIR_SEQ_SHIFT)
#define ISP_TOP_ACQ_PROP_CCIR_SEQ_UYVY                              (0x2 << ISP_TOP_ACQ_PROP_CCIR_SEQ_SHIFT)
#define ISP_TOP_ACQ_PROP_CCIR_SEQ_VYUY                              (0x3 << ISP_TOP_ACQ_PROP_CCIR_SEQ_SHIFT)
#define ISP_TOP_ACQ_PROP_CCIR_SEQ_MASK                              (0x3 << ISP_TOP_ACQ_PROP_CCIR_SEQ_SHIFT)

#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT                      (12)
#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_12BITS                     (0x0 << ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT)
#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_10BITS_2ZEROES_LSBS        (0x1 << ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT)
#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_10BITS_2MSBS_LSBS          (0x2 << ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT)
#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_8BITS_4ZEROES_LSBS         (0x3 << ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT)
#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_8BITS_4MSBS_LSBS           (0x4 << ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT)
#define ISP_TOP_ACQ_PROP_INPUT_SELECTION_MASK                       (0x7 << ISP_TOP_ACQ_PROP_INPUT_SELECTION_SHIFT)

#define ISP_TOP_IMSC_ISP_OFF_SHIFT                                  (0)
#define ISP_TOP_IMSC_ISP_OFF                                        (0x1 << ISP_TOP_IMSC_ISP_OFF_SHIFT)
#define ISP_TOP_IMSC_ISP_OFF_MASK                                   (0x1 << ISP_TOP_IMSC_ISP_OFF_SHIFT)

#define ISP_TOP_IMSC_GAIN_DONE_SHIFT                                (22)
#define ISP_TOP_IMSC_GAIN_DONE                                      (0x1 << ISP_TOP_IMSC_PIC_SIZE_ERR_SHIFT)
#define ISP_TOP_IMSC_GAIN_DONE_MASK                                 (0x1 << ISP_TOP_IMSC_PIC_SIZE_ERR_SHIFT)

/* MARVIN CTRL */
#define MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_SHIFT                   (0x8)
#define MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_PARAL                   (0x0 << MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_SHIFT)
#define MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_LVDS                    (0x1 << MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_SHIFT)
#define MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_MIPI                    (0x2 << MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_SHIFT)
#define MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_MASK                    (0x3 << MARVIN_CTRL_VI_DPCL_INPUT_INTERFACE_SHIFT)

#define MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT                      (0x2)
#define MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_PATH_MAIN_RAW              (0x1 << MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT)
#define MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_PATH_SELF                  (0x2 << MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT)
#define MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_PATH_MAIN_SELF             (0x3 << MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT)
#define MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_MASK                       (0x3 << MARVIN_CTRL_VI_DPCL_VI_CHAN_MODE_SHIFT)

#define MARVIN_CTRL_VI_MP_MUX_SHIFT                                 (0x0)
#define MARVIN_CTRL_VI_MP_MUX_DMA2JPEG                              (0x0 << MARVIN_CTRL_VI_MP_MUX_SHIFT)
#define MARVIN_CTRL_VI_MP_MUX_MAIN2MI                               (0x1 << MARVIN_CTRL_VI_MP_MUX_SHIFT)
#define MARVIN_CTRL_VI_MP_MUX_MAIN2JPEG                             (0x2 << MARVIN_CTRL_VI_MP_MUX_SHIFT)
#define MARVIN_CTRL_VI_MP_MUX_MASK                                  (0x3 << MARVIN_CTRL_VI_MP_MUX_SHIFT)

#define MARVIN_CTRL_LVDS_CTRL_START_Y_SHIFT                         (21)
#define MARVIN_CTRL_LVDS_CTRL_START_Y(offset)                       ((offset) << MARVIN_CTRL_LVDS_CTRL_START_Y_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_START_Y_MASK                          (0x7fff << MARVIN_CTRL_LVDS_CTRL_START_Y_SHIFT)

#define MARVIN_CTRL_LVDS_CTRL_START_X_SHIFT                         (10)
#define MARVIN_CTRL_LVDS_CTRL_START_X(offset)                       ((offset) << MARVIN_CTRL_LVDS_CTRL_START_X_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_START_X_MASK                          (0x7fff << MARVIN_CTRL_LVDS_CTRL_START_X_SHIFT)

#define MARVIN_CTRL_LVDS_CTRL_MAIN_LANE_SHIFT                       (8)
#define MARVIN_CTRL_LVDS_CTRL_MAIN_LANE(num)                        ((num) << MARVIN_CTRL_LVDS_CTRL_MAIN_LANE_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_MAIN_LANE_MASK                        (0x3 << MARVIN_CTRL_LVDS_CTRL_MAIN_LANE_SHIFT)

#define MARVIN_CTRL_LVDS_CTRL_LANE_EN_SHIFT                         (4)
#define MARVIN_CTRL_LVDS_CTRL_LANE_EN_0                             (0x1 << MARVIN_CTRL_LVDS_CTRL_LANE_EN_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_LANE_EN_1                             (0x2 << MARVIN_CTRL_LVDS_CTRL_LANE_EN_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_LANE_EN_2                             (0x4 << MARVIN_CTRL_LVDS_CTRL_LANE_EN_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_LANE_EN_3                             (0x8 << MARVIN_CTRL_LVDS_CTRL_LANE_EN_SHIFT)

#define MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_SHIFT                      (2)
#define MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_8BITS                      (0x0 << MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_10BITS                     (0x1 << MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_12BITS                     (0x2 << MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_MASK                       (0x3 << MARVIN_CTRL_LVDS_CTRL_BITS_WIDTH_SHIFT)

#define MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_SHIFT                  (1)
#define MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_SAV_EAV_INTERVAL       (0x0 << MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_FS_LE_LS_FE            (0x1 << MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_SHIFT)
#define MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_MASK                   (0x1 << MARVIN_CTRL_LVDS_CTRL_SYNC_CODE_MODE_SHIFT)

#define MARVIN_CTRL_LVDS_CAP_EN_SHIFT                               (0)
#define MARVIN_CTRL_LVDS_CAP_EN_ENABLE                              (0x1 << MARVIN_CTRL_LVDS_CAP_EN_SHIFT)
#define MARVIN_CTRL_LVDS_CAP_EN_DISABLE                             (0x0 << MARVIN_CTRL_LVDS_CAP_EN_SHIFT)
#define MARVIN_CTRL_LVDS_CAP_EN_MASK                                (0x1 << MARVIN_CTRL_LVDS_CAP_EN_SHIFT)

#define MARVIN_CTRL_LVDS_SYNC_CODE_EAV_ACT_SHIFT                    (16)
#define MARVIN_CTRL_LVDS_SYNC_CODE_EAV_ACT(code)                    ((code) << MARVIN_CTRL_LVDS_SYNC_CODE_EAV_ACT_SHIFT)
#define MARVIN_CTRL_LVDS_SYNC_CODE_EAV_ACT_MASK                     (0xfff << MARVIN_CTRL_LVDS_SYNC_CODE_EAV_ACT_SHIFT)

#define MARVIN_CTRL_LVDS_SYNC_CODE_SAV_ACT_SHIFT                    (0)
#define MARVIN_CTRL_LVDS_SYNC_CODE_SAV_ACT(code)                    ((code) << MARVIN_CTRL_LVDS_SYNC_CODE_SAV_ACT_SHIFT)
#define MARVIN_CTRL_LVDS_SYNC_CODE_SAV_ACT_MASK                     (0xfff << MARVIN_CTRL_LVDS_SYNC_CODE_SAV_ACT_SHIFT)

#define MARVIN_CTRL_LVDS_SYNC_TIME_EAV_BLK_SHIFT                    (16)
#define MARVIN_CTRL_LVDS_SYNC_TIME_EAV_BLK(code)                    ((code) << MARVIN_CTRL_LVDS_SYNC_TIME_EAV_BLK_SHIFT)
#define MARVIN_CTRL_LVDS_SYNC_TIME_EAV_BLK_MASK                     (0xfff << MARVIN_CTRL_LVDS_SYNC_TIME_EAV_BLK_SHIFT)

#define MARVIN_CTRL_LVDS_SYNC_TIME_SAV_BLK_SHIFT                    (0)
#define MARVIN_CTRL_LVDS_SYNC_TIME_SAV_BLK(code)                    ((code) << MARVIN_CTRL_LVDS_SYNC_TIME_SAV_BLK_SHIFT)
#define MARVIN_CTRL_LVDS_SYNC_TIME_SAV_BLK_MASK                     (0xfff << MARVIN_CTRL_LVDS_SYNC_TIME_SAV_BLK_SHIFT)

/* MI_CTRL */
#define CIF_MI_CTRL_MP_ENABLE                                       (1 << 0)
#define CIF_MI_CTRL_SP_ENABLE                                       (2 << 0)
#define CIF_MI_CTRL_JPEG_ENABLE                                     (4 << 0)
#define CIF_MI_CTRL_RAW_ENABLE                                      (8 << 0)
#define CIF_MI_CTRL_HFLIP                                           BIT(4)
#define CIF_MI_CTRL_VFLIP                                           BIT(5)
#define CIF_MI_CTRL_ROT                                             BIT(6)
#define CIF_MI_BYTE_SWAP                                            BIT(7)
#define CIF_MI_SP_Y_FULL_YUV2RGB                                    BIT(8)
#define CIF_MI_SP_CBCR_FULL_YUV2RGB                                 BIT(9)
#define CIF_MI_SP_422NONCOSITEED                                    BIT(10)
#define CIF_MI_MP_PINGPONG_ENABEL                                   BIT(11)
#define CIF_MI_SP_PINGPONG_ENABEL                                   BIT(12)
#define CIF_MI_MP_AUTOUPDATE_ENABLE                                 BIT(13)
#define CIF_MI_SP_AUTOUPDATE_ENABLE                                 BIT(14)
#define CIF_MI_LAST_PIXEL_SIG_ENABLE                                BIT(15)
#define CIF_MI_CTRL_BURST_LEN_LUM_4                                 (0 << 16)
#define CIF_MI_CTRL_BURST_LEN_LUM_8                                 (1 << 16)
#define CIF_MI_CTRL_BURST_LEN_LUM_16                                (2 << 16)
#define CIF_MI_CTRL_BURST_LEN_CHROM_4                               (0 << 18)
#define CIF_MI_CTRL_BURST_LEN_CHROM_8                               (1 << 18)
#define CIF_MI_CTRL_BURST_LEN_CHROM_16                              (2 << 18)
#define CIF_MI_CTRL_INIT_BASE_EN                                    BIT(20)
#define CIF_MI_CTRL_INIT_OFFSET_EN                                  BIT(21)
#define MI_CTRL_MP_WRITE_YUV_PLA_OR_RAW8                            (0 << 22)
#define MI_CTRL_MP_WRITE_YUV_SPLA                                   (1 << 22)
#define MI_CTRL_MP_WRITE_YUVINT                                     (2 << 22)
#define MI_CTRL_MP_WRITE_RAW12                                      (2 << 22)
#define MI_CTRL_SP_WRITE_PLA                                        (0 << 24)
#define MI_CTRL_SP_WRITE_SPLA                                       (1 << 24)
#define MI_CTRL_SP_WRITE_INT                                        (2 << 24)
#define MI_CTRL_SP_INPUT_YUV400                                     (0 << 26)
#define MI_CTRL_SP_INPUT_YUV420                                     (1 << 26)
#define MI_CTRL_SP_INPUT_YUV422                                     (2 << 26)
#define MI_CTRL_SP_INPUT_YUV444                                     (3 << 26)
#define MI_CTRL_SP_OUTPUT_YUV400                                    (0 << 28)
#define MI_CTRL_SP_OUTPUT_YUV420                                    (1 << 28)
#define MI_CTRL_SP_OUTPUT_YUV422                                    (2 << 28)
#define MI_CTRL_SP_OUTPUT_YUV444                                    (3 << 28)
#define MI_CTRL_SP_OUTPUT_RGB565                                    (4 << 28)
#define MI_CTRL_SP_OUTPUT_RGB666                                    (5 << 28)
#define MI_CTRL_SP_OUTPUT_RGB888                                    (6 << 28)

#define MI_MI_WR_CTRL_SW_MI_WR_INIT_BASE_EN_SHIFT          (20U)
#define MI_MI_WR_CTRL_SW_MI_WR_INIT_BASE_EN_MASK           (0x1U << MI_MI_WR_CTRL_SW_MI_WR_INIT_BASE_EN_SHIFT)          /* 0x00100000 */
#define MI_MI_WR_CTRL_SW_MI_WR_INIT_OFFSET_EN_SHIFT        (21U)
#define MI_MI_WR_CTRL_SW_MI_WR_INIT_OFFSET_EN_MASK         (0x1U << MI_MI_WR_CTRL_SW_MI_WR_INIT_OFFSET_EN_SHIFT)        /* 0x00200000 */
#define MI_MI_WR_CTRL_SW_MP_WR_AUTO_UPD_SHIFT              (13U)
#define MI_MI_WR_CTRL_SW_MP_WR_AUTO_UPD_MASK               (0x1U << MI_MI_WR_CTRL_SW_MP_WR_AUTO_UPD_SHIFT)              /* 0x00002000 */


#define MI_CTRL_MP_FMT_MASK                                         GENMASK(23, 22)
#define MI_CTRL_SP_FMT_MASK                                         GENMASK(30, 24)

/* MI_INIT */
#define CIF_MI_INIT_SKIP                BIT(2)
#define CIF_MI_INIT_SOFT_UPD            BIT(4)

/* MI_CTRL_SHD */
#define CIF_MI_CTRL_SHD_MP_IN_ENABLED       BIT(0)
#define CIF_MI_CTRL_SHD_SP_IN_ENABLED       BIT(1)
#define CIF_MI_CTRL_SHD_JPEG_IN_ENABLED     BIT(2)
#define CIF_MI_CTRL_SHD_RAW_IN_ENABLED      BIT(3)
#define CIF_MI_CTRL_SHD_MP_OUT_ENABLED      BIT(16)
#define CIF_MI_CTRL_SHD_SP_OUT_ENABLED      BIT(17)
#define CIF_MI_CTRL_SHD_JPEG_OUT_ENABLED    BIT(18)
#define CIF_MI_CTRL_SHD_RAW_OUT_ENABLED     BIT(19)

/* MI_CTRL2 */
#define SW_GAIN_WR_AUTOUPD                  BIT(13)
#define SW_GAIN_WR_PINGPONG                 BIT(12)
#define SW_DBR_WR_AUTOUPD                   BIT(10)
#define SW_MIMUX_BYTE_SWAP                  BIT(9)
#define SW_MIMUX_EN                         BIT(8)
#define SW_RAW3_WR_AUTOUPD                  BIT(3)
#define SW_RAW2_WR_AUTOUPD                  BIT(2)
#define SW_RAW1_WR_AUTOUPD                  BIT(1)
#define SW_RAW0_WR_AUTOUPD                  BIT(0)

/* RSZ_CTRL */
#define CIF_RSZ_CTRL_SCALE_HY_ENABLE        BIT(0)
#define CIF_RSZ_CTRL_SCALE_HC_ENABLE        BIT(1)
#define CIF_RSZ_CTRL_SCALE_VY_ENABLE        BIT(2)
#define CIF_RSZ_CTRL_SCALE_VC_ENABLE        BIT(3)
#define CIF_RSZ_CTRL_SCALE_HY_UP            BIT(4)
#define CIF_RSZ_CTRL_SCALE_HC_UP            BIT(5)
#define CIF_RSZ_CTRL_SCALE_VY_UP            BIT(6)
#define CIF_RSZ_CTRL_SCALE_VC_UP            BIT(7)
#define CIF_RSZ_CTRL_CFG_UPD                BIT(8)
#define CIF_RSZ_CTRL_CFG_UPD_AUTO           BIT(9)
#define CIF_RSZ_SCALER_FACTOR               BIT(16)

/* MI_XTD_FORMAT_CTRL  */
#define CIF_MI_XTD_FMT_CTRL_MP_CB_CR_SWAP                           BIT(0)
#define CIF_MI_XTD_FMT_CTRL_SP_CB_CR_SWAP                           BIT(1)
#define CIF_MI_XTD_FMT_CTRL_DMA_CB_CR_SWAP                          BIT(2)

/* ISP TOP CTRL0 */
#define ISP_CTRL0_OFFSET                                   (0x0U)
#define ISP_CTRL0_SW_ISP_ENABLE_SHIFT                      (0U)
#define ISP_CTRL0_SW_ISP_ENABLE_MASK                       (0x1U << ISP_CTRL0_SW_ISP_ENABLE_SHIFT)                      /* 0x00000001 */
#define ISP_CTRL0_SW_ISP_MODE_SHIFT                        (1U)
#define ISP_CTRL0_SW_ISP_MODE_MASK                         (0x7U << ISP_CTRL0_SW_ISP_MODE_SHIFT)                        /* 0x0000000E */
#define ISP_CTRL0_SW_INFORM_ENABLE_SHIFT                   (4U)
#define ISP_CTRL0_SW_INFORM_ENABLE_MASK                    (0x1U << ISP_CTRL0_SW_INFORM_ENABLE_SHIFT)                   /* 0x00000010 */
#define ISP_CTRL0_SW_MIR_ENABLE_SHIFT                      (5U)
#define ISP_CTRL0_SW_MIR_ENABLE_MASK                       (0x1U << ISP_CTRL0_SW_MIR_ENABLE_SHIFT)                      /* 0x00000020 */
#define ISP_CTRL0_SW_GAMMA_IN_ENABLE_SHIFT                 (6U)
#define ISP_CTRL0_SW_GAMMA_IN_ENABLE_MASK                  (0x1U << ISP_CTRL0_SW_GAMMA_IN_ENABLE_SHIFT)                 /* 0x00000040 */
#define ISP_CTRL0_ISP_AWB_ENABLE_SHIFT                     (7U)
#define ISP_CTRL0_ISP_AWB_ENABLE_MASK                      (0x1U << ISP_CTRL0_ISP_AWB_ENABLE_SHIFT)                     /* 0x00000080 */
#define ISP_CTRL0_SYS_CFG_GEN_UPD_FIX_SHIFT                (8U)
#define ISP_CTRL0_SYS_CFG_GEN_UPD_FIX_MASK                 (0x1U << ISP_CTRL0_SYS_CFG_GEN_UPD_FIX_SHIFT)                /* 0x00000100 */
#define ISP_CTRL0_SYS_CFG_FORCE_UPD_SHIFT                  (9U)
#define ISP_CTRL0_SYS_CFG_FORCE_UPD_MASK                   (0x1U << ISP_CTRL0_SYS_CFG_FORCE_UPD_SHIFT)                  /* 0x00000200 */
#define ISP_CTRL0_SYS_CFG_GEN_UPD_SHIFT                    (10U)
#define ISP_CTRL0_SYS_CFG_GEN_UPD_MASK                     (0x1U << ISP_CTRL0_SYS_CFG_GEN_UPD_SHIFT)                    /* 0x00000400 */
#define ISP_CTRL0_SW_FLASH_MODE_SHIFT                      (12U)
#define ISP_CTRL0_SW_FLASH_MODE_MASK                       (0x1U << ISP_CTRL0_SW_FLASH_MODE_SHIFT)                      /* 0x00001000 */
#define ISP_CTRL0_SW_CSM_Y_RANGE_SHIFT                     (13U)
#define ISP_CTRL0_SW_CSM_Y_RANGE_MASK                      (0x1U << ISP_CTRL0_SW_CSM_Y_RANGE_SHIFT)                     /* 0x00002000 */
#define ISP_CTRL0_SW_CSM_C_RANGE_SHIFT                     (14U)
#define ISP_CTRL0_SW_CSM_C_RANGE_MASK                      (0x1U << ISP_CTRL0_SW_CSM_C_RANGE_SHIFT)                     /* 0x00004000 */
#define ISP_CTRL0_SW_EN_12BIT_PACKED_SHIFT                 (15U)
#define ISP_CTRL0_SW_EN_12BIT_PACKED_MASK                  (0x1U << ISP_CTRL0_SW_EN_12BIT_PACKED_SHIFT)                 /* 0x00008000 */
#define ISP_CTRL0_SW_EN_12BIT_UNPACK_SHIFT                 (16U)
#define ISP_CTRL0_SW_EN_12BIT_UNPACK_MASK                  (0x1U << ISP_CTRL0_SW_EN_12BIT_UNPACK_SHIFT)                 /* 0x00010000 */
#define ISP_CTRL0_SW_CGC_YUV_LIMIT_SHIFT                   (28U)
#define ISP_CTRL0_SW_CGC_YUV_LIMIT_MASK                    (0x1U << ISP_CTRL0_SW_CGC_YUV_LIMIT_SHIFT)                   /* 0x10000000 */
#define ISP_CTRL0_SW_CGC_RATIO_EN_SHIFT                    (29U)
#define ISP_CTRL0_SW_CGC_RATIO_EN_MASK                     (0x1U << ISP_CTRL0_SW_CGC_RATIO_EN_SHIFT)

/* ISP_IMSC */
#define ISP_ISP_IMSC_OFFSET                                (0x1BCU)
#define ISP_ISP_IMSC_SW_IMSC_ISP_OFF_SHIFT                 (0U)
#define ISP_ISP_IMSC_SW_IMSC_ISP_OFF_MASK                  (0x1U << ISP_ISP_IMSC_SW_IMSC_ISP_OFF_SHIFT)                 /* 0x00000001 */
#define ISP_ISP_IMSC_SW_IMSC_FRAME_SHIFT                   (1U)
#define ISP_ISP_IMSC_SW_IMSC_FRAME_MASK                    (0x1U << ISP_ISP_IMSC_SW_IMSC_FRAME_SHIFT)                   /* 0x00000002 */
#define ISP_ISP_IMSC_SW_IMSC_DATA_LOSS_SHIFT               (2U)
#define ISP_ISP_IMSC_SW_IMSC_DATA_LOSS_MASK                (0x1U << ISP_ISP_IMSC_SW_IMSC_DATA_LOSS_SHIFT)               /* 0x00000004 */
#define ISP_ISP_IMSC_SW_IMSC_PIC_SIZE_ERR_SHIFT            (3U)
#define ISP_ISP_IMSC_SW_IMSC_PIC_SIZE_ERR_MASK             (0x1U << ISP_ISP_IMSC_SW_IMSC_PIC_SIZE_ERR_SHIFT)            /* 0x00000008 */
#define ISP_ISP_IMSC_SW_IMSC_AWB_DONE_SHIFT                (4U)
#define ISP_ISP_IMSC_SW_IMSC_AWB_DONE_MASK                 (0x1U << ISP_ISP_IMSC_SW_IMSC_AWB_DONE_SHIFT)                /* 0x00000010 */
#define ISP_ISP_IMSC_SW_IMSC_FRAME_IN_SHIFT                (5U)
#define ISP_ISP_IMSC_SW_IMSC_FRAME_IN_MASK                 (0x1U << ISP_ISP_IMSC_SW_IMSC_FRAME_IN_SHIFT)                /* 0x00000020 */
#define ISP_ISP_IMSC_SW_IMSC_V_START_SHIFT                 (6U)
#define ISP_ISP_IMSC_SW_IMSC_V_START_MASK                  (0x1U << ISP_ISP_IMSC_SW_IMSC_V_START_SHIFT)                 /* 0x00000040 */
#define ISP_ISP_IMSC_SW_IMSC_H_START_SHIFT                 (7U)
#define ISP_ISP_IMSC_SW_IMSC_H_START_MASK                  (0x1U << ISP_ISP_IMSC_SW_IMSC_H_START_SHIFT)                 /* 0x00000080 */
#define ISP_ISP_IMSC_SW_IMSC_FLASH_ON_SHIFT                (8U)
#define ISP_ISP_IMSC_SW_IMSC_FLASH_ON_MASK                 (0x1U << ISP_ISP_IMSC_SW_IMSC_FLASH_ON_SHIFT)                /* 0x00000100 */
#define ISP_ISP_IMSC_SW_IMSC_FLASH_OFF_SHIFT               (9U)
#define ISP_ISP_IMSC_SW_IMSC_FLASH_OFF_MASK                (0x1U << ISP_ISP_IMSC_SW_IMSC_FLASH_OFF_SHIFT)               /* 0x00000200 */
#define ISP_ISP_IMSC_SW_IMSC_SHUTTER_ON_SHIFT              (10U)
#define ISP_ISP_IMSC_SW_IMSC_SHUTTER_ON_MASK               (0x1U << ISP_ISP_IMSC_SW_IMSC_SHUTTER_ON_SHIFT)              /* 0x00000400 */
#define ISP_ISP_IMSC_SW_IMSC_SHUTTER_OFF_SHIFT             (11U)
#define ISP_ISP_IMSC_SW_IMSC_SHUTTER_OFF_MASK              (0x1U << ISP_ISP_IMSC_SW_IMSC_SHUTTER_OFF_SHIFT)             /* 0x00000800 */
#define ISP_ISP_IMSC_SW_IMSC_AFM_SUM_OF_SHIFT              (12U)
#define ISP_ISP_IMSC_SW_IMSC_AFM_SUM_OF_MASK               (0x1U << ISP_ISP_IMSC_SW_IMSC_AFM_SUM_OF_SHIFT)              /* 0x00001000 */
#define ISP_ISP_IMSC_SW_IMSC_AFM_LUM_OF_SHIFT              (13U)
#define ISP_ISP_IMSC_SW_IMSC_AFM_LUM_OF_MASK               (0x1U << ISP_ISP_IMSC_SW_IMSC_AFM_LUM_OF_SHIFT)              /* 0x00002000 */
#define ISP_ISP_IMSC_SW_IMSC_AFM_FIN_SHIFT                 (14U)
#define ISP_ISP_IMSC_SW_IMSC_AFM_FIN_MASK                  (0x1U << ISP_ISP_IMSC_SW_IMSC_AFM_FIN_SHIFT)                 /* 0x00004000 */
#define ISP_ISP_IMSC_SW_IMSC_HIST_MEASURE_RDY_SHIFT        (15U)
#define ISP_ISP_IMSC_SW_IMSC_HIST_MEASURE_RDY_MASK         (0x1U << ISP_ISP_IMSC_SW_IMSC_HIST_MEASURE_RDY_SHIFT)        /* 0x00008000 */
#define ISP_ISP_IMSC_SW_IMSC_LSC_LUT_ERR_SHIFT             (16U)
#define ISP_ISP_IMSC_SW_IMSC_LSC_LUT_ERR_MASK              (0x1U << ISP_ISP_IMSC_SW_IMSC_LSC_LUT_ERR_SHIFT)             /* 0x00010000 */
#define ISP_ISP_IMSC_SW_IMSC_FLASH_CAP_SHIFT               (17U)
#define ISP_ISP_IMSC_SW_IMSC_FLASH_CAP_MASK                (0x1U << ISP_ISP_IMSC_SW_IMSC_FLASH_CAP_SHIFT)               /* 0x00020000 */
#define ISP_ISP_IMSC_SW_IMSC_EXP_END_SHIFT                 (18U)
#define ISP_ISP_IMSC_SW_IMSC_EXP_END_MASK                  (0x1U << ISP_ISP_IMSC_SW_IMSC_EXP_END_SHIFT)                 /* 0x00040000 */
#define ISP_ISP_IMSC_SW_IMSC_HDR_DONE_SHIFT                (20U)
#define ISP_ISP_IMSC_SW_IMSC_HDR_DONE_MASK                 (0x1U << ISP_ISP_IMSC_SW_IMSC_HDR_DONE_SHIFT)                /* 0x00100000 */
#define ISP_ISP_IMSC_SW_IMSC_DHAZ_DONE_SHIFT               (21U)
#define ISP_ISP_IMSC_SW_IMSC_DHAZ_DONE_MASK                (0x1U << ISP_ISP_IMSC_SW_IMSC_DHAZ_DONE_SHIFT)               /* 0x00200000 */
#define ISP_ISP_IMSC_SW_IMSC_GAIN_ERR_SHIFT                (22U)
#define ISP_ISP_IMSC_SW_IMSC_GAIN_ERR_MASK                 (0x1U << ISP_ISP_IMSC_SW_IMSC_GAIN_ERR_SHIFT)                /* 0x00400000 */
#define ISP_ISP_IMSC_SW_IMSC_OUT_FRM_END_SHIFT             (23U)
#define ISP_ISP_IMSC_SW_IMSC_OUT_FRM_END_MASK              (0x1U << ISP_ISP_IMSC_SW_IMSC_OUT_FRM_END_SHIFT)             /* 0x00800000 */
#define ISP_ISP_IMSC_SW_IMSC_OUT_HALF_SHIFT                (24U)
#define ISP_ISP_IMSC_SW_IMSC_OUT_HALF_MASK                 (0x1U << ISP_ISP_IMSC_SW_IMSC_OUT_HALF_SHIFT)                /* 0x01000000 */
#define ISP_ISP_IMSC_SW_IMSC_OUT_QUARTER_SHIFT             (25U)
#define ISP_ISP_IMSC_SW_IMSC_OUT_QUARTER_MASK              (0x1U << ISP_ISP_IMSC_SW_IMSC_OUT_QUARTER_SHIFT)             /* 0x02000000 */
#define ISP_ISP_IMSC_SW_IMSC_BAY3D_IN_DONE_SHIFT           (26U)
#define ISP_ISP_IMSC_SW_IMSC_BAY3D_IN_DONE_MASK            (0x1U << ISP_ISP_IMSC_SW_IMSC_BAY3D_IN_DONE_SHIFT)           /* 0x04000000 */
#define ISP_ISP_IMSC_SW_IMSC_BAY3D_IN_LINECNT_DONE_SHIFT   (27U)
#define ISP_ISP_IMSC_SW_IMSC_BAY3D_IN_LINECNT_DONE_MASK    (0x1U << ISP_ISP_IMSC_SW_IMSC_BAY3D_IN_LINECNT_DONE_SHIFT)   /* 0x08000000 */
#define ISP_ISP_IMSC_SW_IMSC_BAY3D_POST_ST_SHIFT           (28U)
#define ISP_ISP_IMSC_SW_IMSC_BAY3D_POST_ST_MASK            (0x1U << ISP_ISP_IMSC_SW_IMSC_BAY3D_POST_ST_SHIFT)           /* 0x10000000 */
#define ISP_ISP_IMSC_SW_IMSC_BAYNR_FRAME_END_SHIFT         (29U)
#define ISP_ISP_IMSC_SW_IMSC_BAYNR_FRAME_END_MASK          (0x1U << ISP_ISP_IMSC_SW_IMSC_BAYNR_FRAME_END_SHIFT)         /* 0x20000000 */
#define ISP_ISP_IMSC_SW_IMSC_FETCH_LUT_END_SHIFT           (30U)
#define ISP_ISP_IMSC_SW_IMSC_FETCH_LUT_END_MASK            (0x1U << ISP_ISP_IMSC_SW_IMSC_FETCH_LUT_END_SHIFT)           /* 0x40000000 */

/* GRF_CSIPHY0_CON & GRF_CSIPHY1_CON */
#define CSIPHY_CLK_INV_SHIFT                                        (9)
#define CSIPHY_CLK_INV_WRITE_SHIFT                                  (CSIPHY_CLK_INV_SHIFT + 16)
#define CSIPHY_CLK_INV_WRITE_ENABLE                                 (0x1 << CSIPHY_CLK_INV_WRITE_SHIFT)
#define CSIPHY_CLK_INV_ENABLE                                       ((0x1 << CSIPHY_CLK_INV_SHIFT) | CSIPHY_CLK_INV_WRITE_ENABLE)
#define CSIPHY_CLK_INV_DISABLE                                      ((0x0 << CSIPHY_CLK_INV_SHIFT) | CSIPHY_CLK_INV_WRITE_ENABLE)

#define CSIPHY_CLKLANE_EN_SHIFT                                     (8)
#define CSIPHY_CLKLANE_EN_WRITE_SHIFT                               (CSIPHY_CLKLANE_EN_SHIFT + 16)
#define CSIPHY_CLKLANE_EN_WRITE_ENABLE                              (0x1 << CSIPHY_CLKLANE_EN_WRITE_SHIFT)
#define CSIPHY_CLKLANE_ENABLE                                       ((0x1 << CSIPHY_CLKLANE_EN_SHIFT) | CSIPHY_CLKLANE_EN_WRITE_ENABLE)
#define CSIPHY_CLKLANE_DISABLE                                      ((0x0 << CSIPHY_CLKLANE_EN_SHIFT) | CSIPHY_CLKLANE_EN_WRITE_ENABLE)

#define CSIPHY_LANE_3_SHIFT                                         (7)
#define CSIPHY_LANE_3_WRITE_SHIFT                                   (CSIPHY_LANE_3_SHIFT + 16)
#define CSIPHY_LANE_3_WRITE_ENABLE                                  (0x1 << CSIPHY_LANE_3_WRITE_SHIFT)
#define CSIPHY_LANE_3_ENABLE                                        ((0x1 << CSIPHY_LANE_3_SHIFT) | CSIPHY_LANE_3_WRITE_ENABLE)
#define CSIPHY_LANE_3_DISABLE                                       ((0x0 << CSIPHY_LANE_3_SHIFT) | CSIPHY_LANE_3_WRITE_ENABLE)

#define CSIPHY_LANE_2_SHIFT                                         (6)
#define CSIPHY_LANE_2_WRITE_SHIFT                                   (CSIPHY_LANE_2_SHIFT + 16)
#define CSIPHY_LANE_2_WRITE_ENABLE                                  (0x1 << CSIPHY_LANE_2_WRITE_SHIFT)
#define CSIPHY_LANE_2_ENABLE                                        ((0x1 << CSIPHY_LANE_2_SHIFT) | CSIPHY_LANE_2_WRITE_ENABLE)
#define CSIPHY_LANE_2_DISABLE                                       ((0x0 << CSIPHY_LANE_2_SHIFT) | CSIPHY_LANE_2_WRITE_ENABLE)

#define CSIPHY_LANE_1_SHIFT                                         (5)
#define CSIPHY_LANE_1_WRITE_SHIFT                                   (CSIPHY_LANE_1_SHIFT + 16)
#define CSIPHY_LANE_1_WRITE_ENABLE                                  (0x1 << CSIPHY_LANE_1_WRITE_SHIFT)
#define CSIPHY_LANE_1_ENABLE                                        ((0x1 << CSIPHY_LANE_1_SHIFT) | CSIPHY_LANE_1_WRITE_ENABLE)
#define CSIPHY_LANE_1_DISABLE                                       ((0x0 << CSIPHY_LANE_1_SHIFT) | CSIPHY_LANE_1_WRITE_ENABLE)

#define CSIPHY_LANE_0_SHIFT                                         (4)
#define CSIPHY_LANE_0_WRITE_SHIFT                                   (CSIPHY_LANE_0_SHIFT + 16)
#define CSIPHY_LANE_0_WRITE_ENABLE                                  (0x1 << CSIPHY_LANE_0_WRITE_SHIFT)
#define CSIPHY_LANE_0_ENABLE                                        ((0x1 << CSIPHY_LANE_0_SHIFT) | CSIPHY_LANE_0_WRITE_ENABLE)
#define CSIPHY_LANE_0_DISABLE                                       ((0x0 << CSIPHY_LANE_0_SHIFT) | CSIPHY_LANE_0_WRITE_ENABLE)

#define CSIPHY_FORCERXMODE_3_SHIFT                                  (3)
#define CSIPHY_FORCERXMODE_3_WRITE_SHIFT                            (CSIPHY_FORCERXMODE_3_SHIFT + 16)
#define CSIPHY_FORCERXMODE_3_WRITE_ENABLE                           (0x1 << CSIPHY_FORCERXMODE_3_WRITE_SHIFT)
#define CSIPHY_FORCERXMODE_3_ENABLE                                 ((0x1 << CSIPHY_FORCERXMODE_3_SHIFT) | CSIPHY_FORCERXMODE_3_WRITE_ENABLE)
#define CSIPHY_FORCERXMODE_3_DISABLE                                ((0x0 << CSIPHY_FORCERXMODE_3_SHIFT) | CSIPHY_FORCERXMODE_3_WRITE_ENABLE)

#define CSIPHY_FORCERXMODE_2_SHIFT                                  (2)
#define CSIPHY_FORCERXMODE_2_WRITE_SHIFT                            (CSIPHY_FORCERXMODE_2_SHIFT + 16)
#define CSIPHY_FORCERXMODE_2_WRITE_ENABLE                           (0x1 << CSIPHY_FORCERXMODE_2_WRITE_SHIFT)
#define CSIPHY_FORCERXMODE_2_ENABLE                                 ((0x1 << CSIPHY_FORCERXMODE_2_SHIFT) | CSIPHY_FORCERXMODE_2_WRITE_ENABLE)
#define CSIPHY_FORCERXMODE_2_DISABLE                                ((0x0 << CSIPHY_FORCERXMODE_2_SHIFT) | CSIPHY_FORCERXMODE_2_WRITE_ENABLE)

#define CSIPHY_FORCERXMODE_1_SHIFT                                  (1)
#define CSIPHY_FORCERXMODE_1_WRITE_SHIFT                            (CSIPHY_FORCERXMODE_1_SHIFT + 16)
#define CSIPHY_FORCERXMODE_1_WRITE_ENABLE                           (0x1 << CSIPHY_FORCERXMODE_1_WRITE_SHIFT)
#define CSIPHY_FORCERXMODE_1_ENABLE                                 ((0x1 << CSIPHY_FORCERXMODE_1_SHIFT) | CSIPHY_FORCERXMODE_1_WRITE_ENABLE)
#define CSIPHY_FORCERXMODE_1_DISABLE                                ((0x0 << CSIPHY_FORCERXMODE_1_SHIFT) | CSIPHY_FORCERXMODE_1_WRITE_ENABLE)

#define CSIPHY_FORCERXMODE_0_SHIFT                                  (0)
#define CSIPHY_FORCERXMODE_0_WRITE_SHIFT                            (CSIPHY_FORCERXMODE_0_SHIFT + 16)
#define CSIPHY_FORCERXMODE_0_WRITE_ENABLE                           (0x1 << CSIPHY_FORCERXMODE_0_WRITE_SHIFT)
#define CSIPHY_FORCERXMODE_0_ENABLE                                 ((0x1 << CSIPHY_FORCERXMODE_0_SHIFT) | CSIPHY_FORCERXMODE_0_WRITE_ENABLE)
#define CSIPHY_FORCERXMODE_0_DISABLE                                ((0x0 << CSIPHY_FORCERXMODE_0_SHIFT) | CSIPHY_FORCERXMODE_0_WRITE_ENABLE)

#endif

#endif

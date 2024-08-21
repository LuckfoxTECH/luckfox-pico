/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_isp3_subdev.h
  * @version V0.0.1
  * @date    6-July-2022
  * @brief
  *
  ******************************************************************************
  */
#ifndef __DRV_ISP3_SUBDEV_H__
#define __DRV_ISP3_SUBDEV_H__

#include <rthw.h>
#include "common.h"
#include "drv_isp3.h"
#include "rtconfig.h"
#include "isp3_regs.h"
#include "../camera/camera.h"

#if defined(RT_USING_ISP3)

#define CSI_MIPI0_ID0_CTRL0         (0x0100)
#define CSI_MIPI0_ID0_CTRL1         (0x0104)
#define CSI_MIPI0_CTRL              (0x0120)
#define CSI_MIPI1_ID0_CTRL0         (0x0300)
#define CSI_MIPI1_ID0_CTRL1         (0x0304)
#define CSI_MIPI1_CTRL              (0x0320)
#define TOISP0_CH_CTRL              (0x0780)
#define TOISP0_CROP_SIZE            (0x0784)

#define CSIHOST_N_LANES             (0x04)
#define CSIHOST_RESETN              (0x10)
#define CSIHOST_ERR1                (0x20)
#define CSIHOST_ERR2                (0x24)
#define CSIHOST_CONTROL             (0x40)

#define CSI2_DPHY_CTRL_LANE_ENABLE      (0x00)
#define CSI2_DPHY_DUAL_CAL_EN           (0x80)
#define CSI2_DPHY_CLK_INV               (0X84)
#define CSI2_DPHY_CLK_MODE              (0x128)
#define CSI2_DPHY_CLK_WR_THS_SETTLE     (0x160)
#define CSI2_DPHY_CLK_CALIB_EN          (0x168)
#define CSI2_DPHY_LANE0_WR_THS_SETTLE   (0x1e0)
#define CSI2_DPHY_LANE0_CALIB_EN        (0x1e8)
#define CSI2_DPHY_LANE1_WR_THS_SETTLE   (0x260)
#define CSI2_DPHY_LANE1_CALIB_EN        (0x268)
#define CSI2_DPHY_LANE2_WR_THS_SETTLE   (0x2e0)
#define CSI2_DPHY_LANE2_CALIB_EN        (0x2e8)
#define CSI2_DPHY_LANE3_WR_THS_SETTLE   (0x360)
#define CSI2_DPHY_LANE3_CALIB_EN        (0x368)
#define CSI2_DPHY_CLK1_WR_THS_SETTLE    (0x3e0)
#define CSI2_DPHY_CLK1_CALIB_EN         (0x3e8)
#define CSI2_DPHY_PATH0_MODE_SEL        (0x44C)
#define CSI2_DPHY_PATH0_LVDS_MODE_SEL   (0x480)

#define VI_GRF_BASE             (0xff050000)
#define VI_CSIPHY_CON5              (VI_GRF_BASE + 0x14)

/* MIPI DATA_TYPE */
#define MIPI_CSI2_DT_YUV420_8BITS                   0x18
#define MIPI_CSI2_DT_YUV420_10BITS                  0x19
#define MIPI_CSI2_DT_YUV422_8BITS                   0x1E
#define MIPI_CSI2_DT_YUV422_10BITS                  0x1F
#define MIPI_CSI2_DT_RGB565                         0x22
#define MIPI_CSI2_DT_RGB666                         0x23
#define MIPI_CSI2_DT_RGB888                         0x24
#define MIPI_CSI2_DT_RAW8                           0x2A
#define MIPI_CSI2_DT_RAW10                          0x2B
#define MIPI_CSI2_DT_RAW12                          0x2C
#define MIPI_SET_DATA_VC(vc)                        (((vc) & 0x3) << 6)
#define MIPI_SET_DATA_TYPE(type)                    (((type) & 0x3f) << 0)
#define MIPI_SET_DATA_IDS(id, dt, vc)               ((MIPI_SET_DATA_VC(vc) |\
                                                     MIPI_SET_DATA_TYPE(dt)) << ( 8 *(id)))

#define SW_IBUF_OP_MODE(mode)                       ((mode) << 8)
#define SW_CSI_RAW_WR_EN_ORG                        BIT(0)
#define SW_CSI_RAW_WR_SIMG_MODE_ENABLE              BIT(1)
#define SW_CSI_RWA_WR_SIMG_SWP                      BIT(2)
#define SW_CSI_RAW_WR_H_OUT                         BIT(3)
#define SW_CSI_RAW_WR_CRC_OUT                       BIT(4)
#define SW_CSI_RAW_WR_CHANNEL_ENABLE(id)            ((BIT(id) & 0xff) << 8)
#define SW_CSI_RAW_WR_EN_SHD                        BIT(31)

#define SW_CSI_RAW_PIC_V_SIZE(a)                    (((a) & 0x3FFF) << 16)
#define SW_CSI_RAW_PIC_H_SIZE(a)                    ((a) & 0x3FFF)

#define SW_CSI_RAW_PIC_V_OFF(a)                     (((a) & 0x3FFF) << 16)
#define SW_CSI_RAW_PIC_H_OFF(a)                     ((a) & 0x3FFF)

/* CSI2_DPHY *///todo
#define CSI2_DPHY_DUAL_CLK_MODE_EN_SHIFT                   (6)
#define CSI2_DPHY_DUAL_CLK_MODE_EN_MASK                    (0x1U << CSI2_DPHY_DUAL_CLK_MODE_EN_SHIFT)                   /* 0x00000040 */

/* MIPI1_ID0_CTRL0 */

/* MIPI0_ID0_CTRL0 */
#define VICAP_MIPI0_ID0_CTRL0_OFFSET                       (0x100U)
#define VICAP_MIPI0_ID0_CTRL0_SW_CAP_EN_ID0_SHIFT          (0U)
#define VICAP_MIPI0_ID0_CTRL0_SW_CAP_EN_ID0_MASK           (0x1U << VICAP_MIPI0_ID0_CTRL0_SW_CAP_EN_ID0_SHIFT)          /* 0x00000001 */
#define VICAP_MIPI0_ID0_CTRL0_SW_PARSE_TYPE_ID0_SHIFT      (1U)
#define VICAP_MIPI0_ID0_CTRL0_SW_PARSE_TYPE_ID0_MASK       (0x7U << VICAP_MIPI0_ID0_CTRL0_SW_PARSE_TYPE_ID0_SHIFT)      /* 0x0000000E */
#define VICAP_MIPI0_ID0_CTRL0_SW_CROP_EN_ID0_SHIFT         (4U)
#define VICAP_MIPI0_ID0_CTRL0_SW_CROP_EN_ID0_MASK          (0x1U << VICAP_MIPI0_ID0_CTRL0_SW_CROP_EN_ID0_SHIFT)         /* 0x00000010 */
#define VICAP_MIPI0_ID0_CTRL0_SW_WRDDR_TYPE_ID0_SHIFT      (5U)
#define VICAP_MIPI0_ID0_CTRL0_SW_WRDDR_TYPE_ID0_MASK       (0x7U << VICAP_MIPI0_ID0_CTRL0_SW_WRDDR_TYPE_ID0_SHIFT)      /* 0x000000E0 */
#define VICAP_MIPI0_ID0_CTRL0_SW_VC_ID0_SHIFT              (8U)
#define VICAP_MIPI0_ID0_CTRL0_SW_VC_ID0_MASK               (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_VC_ID0_SHIFT)              /* 0x00000300 */
#define VICAP_MIPI0_ID0_CTRL0_SW_DT_ID0_SHIFT              (10U)
#define VICAP_MIPI0_ID0_CTRL0_SW_DT_ID0_MASK               (0x3FU << VICAP_MIPI0_ID0_CTRL0_SW_DT_ID0_SHIFT)             /* 0x0000FC00 */
#define VICAP_MIPI0_ID0_CTRL0_SW_YUYV_IN_ORDER_ID0_SHIFT   (16U)
#define VICAP_MIPI0_ID0_CTRL0_SW_YUYV_IN_ORDER_ID0_MASK    (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_YUYV_IN_ORDER_ID0_SHIFT)   /* 0x00030000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_YUYV_OUT_ORDER_ID0_SHIFT  (18U)
#define VICAP_MIPI0_ID0_CTRL0_SW_YUYV_OUT_ORDER_ID0_MASK   (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_YUYV_OUT_ORDER_ID0_SHIFT)  /* 0x000C0000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_HDR_MODE_ID0_SHIFT        (20U)
#define VICAP_MIPI0_ID0_CTRL0_SW_HDR_MODE_ID0_MASK         (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_HDR_MODE_ID0_SHIFT)        /* 0x00300000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_ON_HDR_MODE_ID0_SHIFT     (22U)
#define VICAP_MIPI0_ID0_CTRL0_SW_ON_HDR_MODE_ID0_MASK      (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_ON_HDR_MODE_ID0_SHIFT)     /* 0x00C00000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_ON_HDR_LINE_CNT_ID0_SHIFT (24U)
#define VICAP_MIPI0_ID0_CTRL0_SW_ON_HDR_LINE_CNT_ID0_MASK  (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_ON_HDR_LINE_CNT_ID0_SHIFT) /* 0x03000000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_COMMAND_MODE_EN_ID0_SHIFT (26U)
#define VICAP_MIPI0_ID0_CTRL0_SW_COMMAND_MODE_EN_ID0_MASK  (0x1U << VICAP_MIPI0_ID0_CTRL0_SW_COMMAND_MODE_EN_ID0_SHIFT) /* 0x04000000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_ALIGN_ID0_SHIFT           (27U)
#define VICAP_MIPI0_ID0_CTRL0_SW_ALIGN_ID0_MASK            (0x1U << VICAP_MIPI0_ID0_CTRL0_SW_ALIGN_ID0_SHIFT)           /* 0x08000000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_DMA_EN_ID0_SHIFT          (28U)
#define VICAP_MIPI0_ID0_CTRL0_SW_DMA_EN_ID0_MASK           (0x1U << VICAP_MIPI0_ID0_CTRL0_SW_DMA_EN_ID0_SHIFT)          /* 0x10000000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_VC_HDR_PROTECT_ID0_SHIFT  (29U)
#define VICAP_MIPI0_ID0_CTRL0_SW_VC_HDR_PROTECT_ID0_MASK   (0x1U << VICAP_MIPI0_ID0_CTRL0_SW_VC_HDR_PROTECT_ID0_SHIFT)  /* 0x20000000 */
#define VICAP_MIPI0_ID0_CTRL0_SW_VC_HDR_ID_MAIN_ID0_SHIFT  (30U)
#define VICAP_MIPI0_ID0_CTRL0_SW_VC_HDR_ID_MAIN_ID0_MASK   (0x3U << VICAP_MIPI0_ID0_CTRL0_SW_VC_HDR_ID_MAIN_ID0_SHIFT)  /* 0xC0000000 */

#define VICAP_TOISP0_SEL_MIPI0                             (0x1)
#define VICAP_TOISP0_SEL_MIPI1                             (0x21)

int rk_isp_hw_vicap_init(struct rk_isp_dev *dev);
int rk_isp_hw_vicap_reinit(struct rk_isp_dev *dev);
int rk_isp_hw_csi_host_init(struct rk_isp_dev *dev);
int rk_isp_hw_csi_host_reinit(struct rk_isp_dev *dev);

#if TEST_FPGA_MIPI_DPHY_ENABLE
void mipidphy_fpga_stream_on(struct rk_isp_dev *dev);
#else
int rk_isp_hw_mipi_lvds_dphy_init(struct rk_isp_dev *dev);
int rk_isp_hw_mipi_lvds_dphy_reinit(struct rk_isp_dev *dev);
#endif

#endif

#endif

/*
*  Rockchip driver for RK ISP 1.0 ON RT-Thread
*
* Copyright (C) 2016 Fuzhou Rockchip Electronics Co., Ltd.
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date            Author            Notes
* 2020-7-20       ISP_TEAM     first implementation
*
*/

#ifndef _ISP_ADAPTIVE_CAMERA_CONFIG_H
#define _ISP_ADAPTIVE_CAMERA_CONFIG_H

#include "camera.h"

#if defined(RT_USING_ISP)
#define CAMERA_STRLEN           32
#define CAMERA_METADATA_LEN     (4 * 4096)
#define VALID_FR_EXP_T_INDEX    0
#define VALID_FR_EXP_G_INDEX    1
#define SENSOR_CONFIG_NUM       4
#define SENSOR_READ_MODE        0
#define SENSOR_WRITE_MODE       1
#define SUBDEV_NUM_MAX          10
#define LIGHT_NUM_MAX           5

#ifndef bool
#define bool unsigned int
#define true (1)
#define false (0)
#endif

enum light_id_s
{
    LIGHT0 = 0,
    LIGHT1,
    LIGHT2,
    LIGHT3,
    LIGHT4,
    LIGHT_ID_INVAILD = 0xff
};

enum light_power_s
{
    LIGHT_ON = 0,
    LIGHT_OFF,
    LIGHT_INVALID = 0xffff
};

enum light_ctl_mode_s
{
    LIGHT_MANUAL_CTL = 0,
    LIGHT_AUTO_CTL,
    LIGHT_ALTERNATE_CTL
};

enum pltfrm_cam_io_voltage
{
    PLTFRM_CAM_IO_1800 = 1800,
    PLTFRM_CAM_IO_3300 = 3300,
};

enum pltfrm_cam_signal_polarity
{
    PLTFRM_CAM_SIGNAL_HIGH_LEVEL = 0,
    PLTFRM_CAM_SIGNAL_LOW_LEVEL = 1,
};

enum pltfrm_cam_sample_type
{
    PLTFRM_CAM_SDR_NEG_EDG          = 0x10000001,
    PLTFRM_CAM_SDR_POS_EDG          = 0x10000002,
    PLTFRM_CAM_DDR                  = 0x20000000
};

enum pltfrm_cam_itf_type
{
    PLTFRM_CAM_ITF_MIPI             = 0x10000000,
    PLTFRM_CAM_ITF_BT601_8          = 0x20000071,
    PLTFRM_CAM_ITF_BT601_8_FIELD    = 0x20000171,
    PLTFRM_CAM_ITF_BT656_8          = 0x20000072,
    PLTFRM_CAM_ITF_BT656_8_1        = 0x20000272,
    PLTFRM_CAM_ITF_BT656_8_2        = 0x20000672,
    PLTFRM_CAM_ITF_BT656_8_4        = 0x20000E72,
    PLTFRM_CAM_ITF_BT601_10         = 0x20000091,
    PLTFRM_CAM_ITF_BT656_10         = 0x20000092,
    PLTFRM_CAM_ITF_BT601_12         = 0x200000B1,
    PLTFRM_CAM_ITF_BT656_12         = 0x200000B2,
    PLTFRM_CAM_ITF_BT601_16         = 0x200000F1,
    PLTFRM_CAM_ITF_BT656_16         = 0x200000F2,
};

#define PLTFRM_CAM_ITF_MAIN_MASK        0xf0000000
#define PLTFRM_CAM_ITF_SUB_MASK         0x0000000f
#define PLTFRM_CAM_ITF_DVP_BW_MASK      0x000000f0
#define PLTFRM_CAM_ITF_DVP_FIELD_MASK   0x00000100
#define PLTFRM_CAM_ITF_DVP_MIX_MASK     0x00000200
#define PLTFRM_CAM_ITF_DVP_CHS_MASK     0x00000C00

#define PLTFRM_CAM_ITF_IS_MIPI(a)   \
    ((a & PLTFRM_CAM_ITF_MAIN_MASK) == 0x10000000)
#define PLTFRM_CAM_ITF_IS_DVP(a)    \
    ((a & PLTFRM_CAM_ITF_MAIN_MASK) == 0x20000000)
#define PLTFRM_CAM_ITF_IS_BT656(a)  \
    (PLTFRM_CAM_ITF_IS_DVP(a) &&    \
        ((a & PLTFRM_CAM_ITF_SUB_MASK) == 0x02))
#define PLTFRM_CAM_ITF_IS_BT656_MIX(a)  \
    (PLTFRM_CAM_ITF_IS_BT656(a) &&  \
        ((a & PLTFRM_CAM_ITF_DVP_MIX_MASK) == 0x200))
#define PLTFRM_CAM_ITF_IS_BT601(a)  \
    (PLTFRM_CAM_ITF_IS_DVP(a) &&    \
        ((a & PLTFRM_CAM_ITF_SUB_MASK) == 0x01))
#define PLTFRM_CAM_ITF_IS_BT601_FIELD(a)    \
    (PLTFRM_CAM_ITF_IS_BT601(a) &&  \
        ((a & PLTFRM_CAM_ITF_DVP_FIELD_MASK) == 0x100))
#define PLTFRM_CAM_ITF_DVP_BW(a)    \
    (((a & PLTFRM_CAM_ITF_DVP_BW_MASK) >> 4) + 1)
#define PLTFRM_CAM_ITF_DVP_CHS(a)   \
    (((a & PLTFRM_CAM_ITF_DVP_CHS_MASK) >> 10) + 1)


struct pltfrm_cam_mipi_config
{
    unsigned int dphy_index;
    unsigned int vc;
    unsigned int nb_lanes;
    unsigned int bit_rate;
    unsigned int embedded_data_vc;
    unsigned int embedded_data_type;
};

struct pltfrm_cam_dvp_config
{
    enum pltfrm_cam_signal_polarity vsync;
    enum pltfrm_cam_signal_polarity hsync;
    enum pltfrm_cam_sample_type pclk;
    enum pltfrm_cam_io_voltage io_vol;
    unsigned int pclk_hz;
};

struct pltfrm_cam_itf
{
    enum pltfrm_cam_itf_type type;
    union
    {
        struct pltfrm_cam_mipi_config mipi;
        struct pltfrm_cam_dvp_config dvp;
    } cfg;
    unsigned int mclk_hz;
};

struct pltfrm_cam_light_cfg
{
    unsigned int light_id;
    bool on;
};

enum pltfrm_cam_ls_val
{
    PLTFRM_LS_INVAL = -1,
    PLTFRM_LS_DAY = 0,
    PLTFRM_LS_NIGHT = 1,
    PLTFRM_LS_HOLD = 2
};

struct pltfrm_cam_ls
{
    enum pltfrm_cam_ls_val val;
    int light_vol;
};

/* Sensor resolution specific data for AE calculation.*/
struct isp_supplemental_sensor_mode_data
{
    unsigned int coarse_integration_time_min;
    unsigned int coarse_integration_time_max_margin;
    unsigned int fine_integration_time_min;
    unsigned int fine_integration_time_max_margin;
    unsigned int frame_length_lines;
    unsigned int line_length_pck;
    unsigned int vt_pix_clk_freq_hz;
    unsigned int crop_horizontal_start; /* Sensor crop start cord. (x0,y0)*/
    unsigned int crop_vertical_start;
    unsigned int crop_horizontal_end; /* Sensor crop end cord. (x1,y1)*/
    unsigned int crop_vertical_end;
    unsigned int sensor_output_width; /* input size to ISP */
    unsigned int sensor_output_height;
    unsigned int isp_input_horizontal_start;
    unsigned int isp_input_vertical_start;
    unsigned int isp_input_width;       /* cif isp input */
    unsigned int isp_input_height;
    unsigned int isp_output_width;  /* cif isp output */
    unsigned int isp_output_height;
    unsigned char binning_factor_x; /* horizontal binning factor used */
    unsigned char binning_factor_y; /* vertical binning factor used */
    /*
    *0: Exposure time valid fileds;
    *1: Exposure gain valid fileds;
    *(2 fileds == 1 frames)
    */
    unsigned char exposure_valid_frame[2];
    int exp_time;
    unsigned short gain;
    unsigned char max_exp_gain_h;
    unsigned char max_exp_gain_l;
    int exp_time_l;
    unsigned short gain_l;
    int exp_time_s;
    unsigned short gain_s;
    unsigned char lane_num;
};
#endif
#endif

/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    ov_camera_module.h
  * @version V0.0.1
  * @brief   video input processor driver for rk625
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */

#ifndef __OV_CAMERA_MODULE_H__
#define __OV_CAMERA_MODULE_H__

#include <rtthread.h>
#include <rtdef.h>
#include "data_type.h"
#include "camera.h"
#include "../isp/isp_adptive_camera_config.h"

#define ov_camera_module_reg pltfrm_camera_module_reg
#define SENSOR_CONFIG_NUM   4


struct pltfrm_camera_module_reg
{
//  u32 flag;
    u16 reg;
    u16 val;
};


/* sensor mode ------------------------------------------------------------*/
struct ov_camera_module_timings ;
struct ov_camera_module ;

enum sensor_hdr_type_t
{
    SENSOR_HDR_NORMAL,
    SENSOR_HDR_SIMULATION
};

/* frame format  ----------------------------------------------------------*/
/**
 * struct v4l2_subdev_frame_interval - Pad-level frame rate
 * @pad: pad number, as reported by the media API
 * @interval: frame interval in seconds
 */
struct v4l2_subdev_frame_interval
{
    u32 pad;
    struct v4l2_fract interval;
};

struct v4l2_mbus_framefmt
{
    u32         width;
    u32         height;
    u32         code;
    u32         field;
    u32         colorspace;
};

/* ov camera module  ------------------------------------------------------------*/

enum ov_camera_module_state
{
    OV_CAMERA_MODULE_POWER_OFF = 0,
    OV_CAMERA_MODULE_HW_STANDBY = 1,
    OV_CAMERA_MODULE_SW_STANDBY = 2,
    OV_CAMERA_MODULE_STREAMING = 3
};

struct sensor_resolution_s
{
    unsigned short width;
    unsigned short height;
};

struct sensor_config_info_s
{
    unsigned char config_num;
    unsigned char sensor_fmt[SENSOR_CONFIG_NUM];
    struct sensor_resolution_s reso[SENSOR_CONFIG_NUM];
};

struct ov_camera_module;

struct ov_camera_module_exp_config
{
    s32 exp_time;
    bool auto_exp;
    u16 gain;
    u16 gain_percent;
    bool auto_gain;
    u32 vts_value;
    s32 exp_time_l;
    u16 gain_l;
    s32 exp_time_s;
    u16 gain_s;
};

struct ov_camera_module_wb_config
{
    u32 temperature;
    u32 preset_id;
    bool auto_wb;
};

struct ov_camera_module_af_config
{
    u32 abs_pos;
    u32 rel_pos;
};

struct ov_camera_module_timings
{
    /* public */
    u32 coarse_integration_time_min;
    u32 coarse_integration_time_max_margin;
    u32 fine_integration_time_min;
    u32 fine_integration_time_max_margin;
    u32 frame_length_lines;
    u32 line_length_pck;
    u32 vt_pix_clk_freq_hz;
    u32 sensor_output_width;
    u32 sensor_output_height;
    u32 crop_horizontal_start; /* Sensor crop start cord. (x0,y0)*/
    u32 crop_vertical_start;
    u32 crop_horizontal_end;   /* Sensor crop end cord. (x1,y1)*/
    u32 crop_vertical_end;
    u8 binning_factor_x;
    u8 binning_factor_y;
    u32 exp_time;
    u32 gain;
};


struct ov_camera_module_config
{
    const char *name;
    struct v4l2_mbus_framefmt frm_fmt;
    struct v4l2_subdev_frame_interval frm_intrvl;
    bool auto_exp_enabled;
    bool auto_gain_enabled;
    bool auto_wb_enabled;
    struct ov_camera_module_reg *reg_table;
    u32 reg_table_num_entries;
    struct ov_camera_module_reg *reg_diff_table;
    u32 reg_diff_table_num_entries;
    u32 v_blanking_time_us;
    u32 line_length_pck;
    u32 frame_length_lines;
    struct ov_camera_module_timings timings;
    bool soft_reset;
    bool ignore_measurement_check;
    u8 max_exp_gain_h;
    u8 max_exp_gain_l;
    struct pltfrm_cam_itf itf_cfg;
};

struct module_awb_cfg
{
    unsigned int enable;
    unsigned int golden_r_value;
    unsigned int golden_b_value;
    unsigned int golden_gr_value;
    unsigned int golden_gb_value;
};

struct ov_camera_module_custom_config
{
    struct ov_camera_module_config *configs;
    u32 num_configs;
    u32 power_up_delays_ms[3];
    u8 exposure_valid_frame[2];
    void *priv;
};

struct ov_camera_module
{
    /* public */
    struct v4l2_mbus_framefmt frm_fmt;
    struct v4l2_subdev_frame_interval frm_intrvl;
    struct ov_camera_module_exp_config exp_config;
    struct ov_camera_module_wb_config wb_config;
    struct ov_camera_module_af_config af_config;
    struct ov_camera_module_custom_config custom;
    enum ov_camera_module_state state;
    enum ov_camera_module_state state_before_suspend;
    struct ov_camera_module_config *active_config;
    u32 ctrl_updt;
    u32 vts_cur;
    u32 vts_min;
    u8 off_save_config;
    u8 auto_adjust_fps;
    u8 update_config;
    u8 frm_fmt_valid;
    u8 frm_intrvl_valid;
    u8 hflip;
    u8 vflip;
    u8 flip_flg;
    u32 rotation;
    void *pltfm_data;
    bool inited;
    bool testpattern_en;

    struct module_awb_cfg awb_cfg;
};

#endif

/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    jx_camera_module.h
  * @version V0.0.1
  * @brief   video input processor driver for rk625
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-29     ISP Team      first implementation
  *
  ******************************************************************************
  */
#ifndef JX_CAMERA_MODULE_H
#define JX_CAMERA_MODULE_H
#include <rtthread.h>
#include <rtdef.h>
#include "data_type.h"
#include "camera.h"
#define SENSOR_CONFIG_NUM   4

#define JX_CAMERA_MODULE_CTRL_UPDT_GAIN             0x01
#define JX_CAMERA_MODULE_CTRL_UPDT_EXP_TIME         0x02
#define JX_CAMERA_MODULE_CTRL_UPDT_WB_TEMPERATURE   0x04
#define JX_CAMERA_MODULE_CTRL_UPDT_AUTO_WB          0x08
#define JX_CAMERA_MODULE_CTRL_UPDT_AUTO_GAIN        0x10
#define JX_CAMERA_MODULE_CTRL_UPDT_AUTO_EXP         0x20
#define JX_CAMERA_MODULE_CTRL_UPDT_FOCUS_ABSOLUTE   0x40
#define JX_CAMERA_MODULE_CTRL_UPDT_PRESET_WB        0x80
#define JX_CAMERA_MODULE_CTRL_UPDT_VTS_VALUE        0x100
#define JX_CAMERA_MODULE_CTRL_UPDT_TEMPFPS_VALUE    0x200

struct jx_camera_module_reg
{
//  u32 flag;
    u8 reg;
    u8 val;
};

/* sensor mode ------------------------------------------------------------*/
struct jx_camera_module_timings ;
struct jx_camera_module ;

enum sensor_hdr_type_t
{
    SENSOR_HDR_NORMAL,
    SENSOR_HDR_SIMULATION
};

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
    //enum sensor_hdr_type_t hdr_type;
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
///* LIGHT_VOL ------------------------------------------------------------*/
//enum pltfrm_cam_ls_val {
//  PLTFRM_LS_INVAL = -1,
//  PLTFRM_LS_DAY = 0,
//  PLTFRM_LS_NIGHT = 1,
//  PLTFRM_LS_HOLD = 2
//};
//struct pltfrm_cam_ls {
//  enum pltfrm_cam_ls_val val;
//  int light_vol;
//};
//
//struct lightsensor_data {
//  struct platform_device *pdev;
//  struct device *child_dev;
//  struct delayed_work work;
//  struct iio_channel *chan;
//  int id;
//  int max_voltage;
//  int threshold;
//};
//
//struct iio_channel {
//  struct iio_dev *indio_dev;
//  const struct iio_chan_spec *channel;
//  void *data;
//};
//
//#define LIGHTSENSOR_STABLE_COUNT     10
//int lightsensor_vol_r(struct device *dev,
//  int *light_vol)
//{
//  struct lightsensor_data *gdata;
//  struct iio_channel *channel;
//  int i, result;
//  int value = 0;
//
//  gdata = dev_get_drvdata(dev);
//  channel = gdata->chan;
//  /* Read ADC value */
//  for (i = 0; i < LIGHTSENSOR_STABLE_COUNT; i++) {
//      result = iio_read_channel_raw(channel, &value);
//          if (result < 0) {
//          dev_err(dev, "read adc channel 2 error:%d\n", result);
//          goto out;
//          }
//      *light_vol = *light_vol + value;
//  }
//  *light_vol = *light_vol / LIGHTSENSOR_STABLE_COUNT;
//  return 0;
//out:
//  return -1;
//}

/* itf cfg ------------------------------------------------------------*/
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

struct pltfrm_cam_mipi_config
{
    u32 dphy_index;
    u32 vc;
    u32 nb_lanes;
    u32 bit_rate;
    u32 embedded_data_vc;
    u32 embedded_data_type;
};

enum pltfrm_cam_signal_polarity
{
    PLTFRM_CAM_SIGNAL_HIGH_LEVEL    = 0,
    PLTFRM_CAM_SIGNAL_LOW_LEVEL     = 1,
};

enum pltfrm_cam_sample_type
{
    PLTFRM_CAM_SDR_NEG_EDG  = 0x10000001,
    PLTFRM_CAM_SDR_POS_EDG  = 0x10000002,
    PLTFRM_CAM_DDR          = 0x20000000
};

enum pltfrm_cam_io_voltage
{
    PLTFRM_CAM_IO_1800 = 1800,
    PLTFRM_CAM_IO_3300 = 3300,
};

struct pltfrm_cam_dvp_config
{
    enum pltfrm_cam_signal_polarity vsync;
    enum pltfrm_cam_signal_polarity hsync;
    enum pltfrm_cam_sample_type     pclk;
    enum pltfrm_cam_io_voltage      io_vol;
    u32 pclk_hz;
};

struct pltfrm_cam_itf
{
    enum pltfrm_cam_itf_type type;
//    short cif_id;
    union
    {
        struct pltfrm_cam_mipi_config   mipi;
        struct pltfrm_cam_dvp_config    dvp;
    } cfg;
    unsigned int mclk_hz;
};

/* frame format  ----------------------------------------------------------*/
struct v4l2_subdev_frame_interval
{
    u32 pad;
    struct v4l2_fract interval;
    u32 reserved[9];
};

struct v4l2_mbus_framefmt
{
    u32         width;
    u32         height;
    u32         code;
    u32         field;
    u32         colorspace;
    u32         reserved[7];
};

/* f camera module  ------------------------------------------------------------*/
enum jx_camera_module_state
{
    JX_CAMERA_MODULE_POWER_OFF  = 0,
    JX_CAMERA_MODULE_HW_STANDBY = 1,
    JX_CAMERA_MODULE_SW_STANDBY = 2,
    JX_CAMERA_MODULE_STREAMING  = 3
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

struct jx_camera_module_exp_config
{
    s32 exp_time;
    bool auto_exp;
    u16 gain;
    u16 gain_percent;
    bool auto_gain;
//  enum v4l2_flash_led_mode flash_mode;
    u32 vts_value;
    u32 temp_fps;
    s32 exp_time_l;
    u16 gain_l;
    s32 exp_time_s;
    u16 gain_s;
};

struct jx_camera_module_wb_config
{
    u32 temperature;
    u32 preset_id;
    bool auto_wb;
};

struct jx_camera_module_af_config
{
    u32 abs_pos;
    u32 rel_pos;
};

struct jx_camera_module_timings
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
    u32 crop_horizontal_end; /* Sensor crop end cord. (x1,y1)*/
    u32 crop_vertical_end;
    u8 binning_factor_x;
    u8 binning_factor_y;
    u32 exp_time;
    u32 gain;
};

struct jx_camera_module_config
{
    const char *name;
    struct v4l2_mbus_framefmt frm_fmt;
    struct v4l2_subdev_frame_interval frm_intrvl;
    bool auto_exp_enabled;
    bool auto_gain_enabled;
    bool auto_wb_enabled;
    struct jx_camera_module_reg *reg_table;
    u32 reg_table_num_entries;
    struct jx_camera_module_reg *reg_diff_table;
    u32 reg_diff_table_num_entries;
    u32 v_blanking_time_us;
    u32 line_length_pck;
    u32 frame_length_lines;
    struct jx_camera_module_timings timings;
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

struct jx_camera_module_custom_config
{
//  int (*start_streaming)(struct jx_camera_module *cam_mod);
//  int (*stop_streaming)(struct jx_camera_module *cam_mod);
//  int (*check_camera_id)(struct jx_camera_module *cam_mod);
//  int (*s_ctrl)(struct jx_camera_module *cam_mod, u32 ctrl_id);
//  int (*g_ctrl)(struct jx_camera_module *cam_mod, u32 ctrl_id);
//  int (*g_timings)(struct jx_camera_module *cam_mod,
//      struct jx_camera_module_timings *timings);
//  int (*s_vts)(struct jx_camera_module *cam_mod,
//      u32 vts);
//  int (*s_ext_ctrls)(struct jx_camera_module *cam_mod,
//      struct jx_camera_module_ext_ctrls *ctrls);
//  int (*set_flip)(
//      struct jx_camera_module *cam_mod,
//      struct pltfrm_camera_module_reg reglist[],
//      int len);
//  int (*init_common)(struct jx_camera_module *cam_mod);
//  int (*read_otp)(struct jx_camera_module *cam_mod);
    struct jx_camera_module_config *configs;
    u32 num_configs;
    u32 power_up_delays_ms[3];
    unsigned char exposure_valid_frame[2];
//  enum sensor_hdr_type_t hdr_type;
    void *priv;
};

struct jx_camera_module
{
    /* public */
//  struct v4l2_subdev sd;
    struct v4l2_mbus_framefmt frm_fmt;
    struct v4l2_subdev_frame_interval frm_intrvl;
    struct jx_camera_module_exp_config exp_config;
    struct jx_camera_module_wb_config wb_config;
    struct jx_camera_module_af_config af_config;
    struct jx_camera_module_custom_config custom;
    enum jx_camera_module_state state;
    enum jx_camera_module_state state_before_suspend;
    struct jx_camera_module_config *active_config;
//  struct jx_camera_module_otp_work otp_work;
    u32 ctrl_updt;
    u32 vts_cur;
    u32 vts_min;
    bool auto_adjust_fps;
    bool update_config;
    bool frm_fmt_valid;
    bool frm_intrvl_valid;
    bool hflip;
    bool vflip;
    bool flip_flg;
    bool testpattern_en;
    u32 rotation;
    void *pltfm_data;
    bool inited;
//  struct mutex lock;
    struct module_awb_cfg awb_cfg;
};

#endif

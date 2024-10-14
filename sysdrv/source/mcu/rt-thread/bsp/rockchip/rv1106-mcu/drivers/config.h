/**
  * Copyright (c) 2020 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    board.h
  * @author  Jason Zhu
  * @version V0.1
  * @date    26-Feb-2020
  * @brief
  *
  ******************************************************************************
  */

#ifndef _CONFIG_H_
#define _CONFIG_H_

#define CONFIG_ADDRESS 0x100000
#define CONFIG_MAGIC   "CONMAG"

#define BOOT_FROM_UNKNOWN 0
#define BOOT_FROM_SPL     1
#define BOOT_FROM_UBOOT   2

struct isp_init_info
{
    uint32_t share_mem_addr;
    uint32_t share_mem_size;
};

struct sensor_init_info
{
    char dev_name[12]; /* sc210iot, imx334, imx415.... */
    uint32_t frame_cnt; /* total frame, 1...n */
    int bitw;   /* pixel bit width 8 10 12... */
    int width;  /* image width */
    int height; /* image height */
    int hdr_mode; /* 0: liner, 1: hdr1, 2: hdr2, 3: hdr3... */
    uint32_t dst_vts;
};

struct bord_init_info
{
    int camera_pwdn_gpio_bank;
    int camera_pwdn_gpio_pin;
    int camera_rst_gpio_bank;
    int camera_rst_gpio_pin;
    int camera_mclk_gpio_bank;
    int camera_mclk_gpio_pin;
    int camera_pw_en_bank;
    int camera_pw_en_gpio_pin;
    int ircut_enb_gpio_bank;
    int ircut_enb_gpio_pin;
    int ircut_fbc_gpio_bank;
    int ircut_fbc_gpio_pin;
    int ir_led_en_gpio_bank;
    int ir_led_en_gpio_pin;
    int ir_led_pwm_gpio_bank;
    int ir_led_pwm_gpio_pin;
    int red_led_pwm_gpio_bank;
    int red_led_pwm_gpio_pin;
    int white_led_en_gpio_bank;
    int white_led_en_gpio_pin;
    int white_led_pwm_gpio_bank;
    int white_led_pwm_gpio_pin;
    int blue_led_pwm_gpio_bank;
    int blue_led_pwm_gpio_pin;
    int lights_sensor_adc_ch;
};

struct config_param
{
    char magic[8];
    int is_ready;
    int boot_from;
    struct isp_init_info isp;
    struct sensor_init_info sensor;
    struct sensor_init_info secondary_sensor;
    struct bord_init_info board;
    int hash;
};

int config_is_integrity(struct config_param *param);
int config_param_parse(struct config_param *param);

#endif

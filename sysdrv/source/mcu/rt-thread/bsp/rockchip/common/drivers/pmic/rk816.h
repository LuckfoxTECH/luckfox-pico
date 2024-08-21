/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816.h
  * @version V0.1
  * @brief   rk816 interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-12-26     Joseph Chen     first implementation
  *
  ******************************************************************************
  */
#ifndef _RK816_H_
#define _RK816_H_

/* Register address definition */
#define SECONDS_REG                     0x00
#define MINUTES_REG                     0x01
#define HOURS_REG                       0x02
#define DAYS_REG                        0x03
#define MONTHS_REG                      0x04
#define YEARS_REG                       0x05
#define WEEKS_REG                       0x06
#define ALARM_SECONDS_REG               0x08
#define ALARM_MINUTES_REG               0x09
#define ALARM_HOURS_REG                 0x0a
#define ALARM_DAYS_REG                  0x0b
#define ALARM_MONTHS_REG                0x0c
#define ALARM_YEARS_REG                 0x0d

#define RTC_CTRL_REG                    0x10
#define RTC_STATUS_REG                  0x11
#define RTC_INT_REG                     0x12

#define CHIP_NAME_REG                   0x17
#define CHIP_VER_REG                    0x18
#define OTP_VER_REG                     0x19
#define VB_MON_REG                      0x21
#define THERMAL_REG                     0x22
#define DCDC_EN_REG1                    0x23
#define DCDC_EN_REG2                    0x24
#define SLP_DCDC_EN_REG                 0x25
#define SLP_LDO_EN_REG                  0x26
#define LDO_EN_REG1                     0x27
#define LDO_EN_REG2                     0x28
#define OTG_BUCK_LDO_CONFIG_REG         0x2A
#define CHRG_CONFIG_REG                 0x2B
#define BUCK1_CONFIG_REG                0x2E
#define BUCK1_ON_VSEL_REG               0x2F
#define BUCK1_SLP_VSEL_REG              0x30
#define BUCK2_CONFIG_REG                0x32
#define BUCK2_ON_VSEL_REG               0x33
#define BUCK2_SLP_VSEL_REG              0x34
#define BUCK3_CONFIG_REG                0x36
#define BUCK4_CONFIG_REG                0x37
#define BUCK4_ON_VSEL_REG               0x38
#define BUCK4_SLP_VSEL_REG              0x39
#define LDO1_ON_VSEL_REG                0x3B
#define LDO1_SLP_VSEL_REG               0x3C
#define LDO2_ON_VSEL_REG                0x3D
#define LDO2_SLP_VSEL_REG               0x3E
#define LDO3_ON_VSEL_REG                0x3F
#define LDO3_SLP_VSEL_REG               0x40
#define LDO4_ON_VSEL_REG                0x41
#define LDO4_SLP_VSEL_REG               0x42
#define LDO5_ON_VSEL_REG                0x43
#define LDO5_SLP_VSEL_REG               0x44
#define LDO6_ON_VSEL_REG                0x45
#define LDO6_SLP_VSEL_REG               0x46
#define PWRON_LP_INT_TIME_REG           0x47
#define PWRON_DB_REG                    0x48
#define INT_STS_REG1                    0x49
#define INT_MSK_REG1                    0x4A
#define DEV_CTRL_REG                    0x4B
#define INT_STS_REG2                    0x4C
#define INT_MSK_REG2                    0x4D
#define INT_STS_REG3                    0x4E
#define INT_MSK_REG3                    0x4F
#define GPIO_IO_POL_REG                 0x50
#define BOOST_ON_VESL_REG               0x54
#define BOOST_SLP_VSEL_REG              0x55
#define CHRG_BOOST_CONFIG_REG           0x9A
#define SUP_STS_REG                     0xA0
#define USB_CTRL_REG                    0xA1
#define CHRG_CTRL_REG1                  0xA3
#define CHRG_CTRL_REG2                  0xA4
#define CHRG_CTRL_REG3                  0xA5
#define BAT_CTRL_REG                    0xA6
#define BAT_HTS_TS_REG                  0xA8
#define BAT_LTS_TS_REG                  0xA9
#define TS_CTRL_REG                     0xAC
#define ADC_CTRL_REG                    0xAD
#define ON_SOURCE_REG                   0xAE
#define OFF_SOURCE_REG                  0xAF
#define GGCON_REG                       0xB0
#define GGSTS_REG                       0xB1
#define ZERO_CUR_ADC_REGH               0xB2
#define ZERO_CUR_ADC_REGL               0xB3
#define GASCNT_CAL_REG3                 0xB4
#define GASCNT_CAL_REG2                 0xB5
#define GASCNT_CAL_REG1                 0xB6
#define GASCNT_CAL_REG0                 0xB7
#define GASCNT_REG3                     0xB8
#define GASCNT_REG2                     0xB9
#define GASCNT_REG1                     0xBA
#define GASCNT_REG0                     0xBB
#define BAT_CUR_AVG_REGH                0xBC
#define BAT_CUR_AVG_REGL                0xBD
#define TS_ADC_REGH                     0xBE
#define TS_ADC_REGL                     0xBF
#define USB_ADC_REGH                    0xC0
#define USB_ADC_REGL                    0xC1
#define BAT_OCV_REGH                    0xC2
#define BAT_OCV_REGL                    0xC3
#define BAT_VOL_REGH                    0xC4
#define BAT_VOL_REGL                    0xC5
#define RELAX_ENTRY_THRES_REGH          0xC6
#define RELAX_ENTRY_THRES_REGL          0xC7
#define RELAX_EXIT_THRES_REGH           0xC8
#define RELAX_EXIT_THRES_REGL           0xC9
#define RELAX_VOL1_REGH                 0xCA
#define RELAX_VOL1_REGL                 0xCB
#define RELAX_VOL2_REGH                 0xCC
#define RELAX_VOL2_REGL                 0xCD
#define RELAX_CUR1_REGH                 0xCE
#define RELAX_CUR1_REGL                 0xCF
#define RELAX_CUR2_REGH                 0xD0
#define RELAX_CUR2_REGL                 0xD1
#define CAL_OFFSET_REGH                 0xD2
#define CAL_OFFSET_REGL                 0xD3
#define NON_ACT_TIMER_CNT_REG           0xD4
#define VCALIB0_REGH                    0xD5
#define VCALIB0_REGL                    0xD6
#define VCALIB1_REGH                    0xD7
#define VCALIB1_REGL                    0xD8
#define FCC_GASCNT_REG3                 0xD9
#define FCC_GASCNT_REG2                 0xDA
#define FCC_GASCNT_REG1                 0xDB
#define FCC_GASCNT_REG0                 0xDC
#define IOFFSET_REGH                    0xDD
#define IOFFSET_REGL                    0xDE
#define SLEEP_CON_SAMP_CUR_REG          0xDF
#define SOC_REG                         0xE0
#define REMAIN_CAP_REG3                 0xE1
#define REMAIN_CAP_REG2                 0xE2
#define REMAIN_CAP_REG1                 0xE3
#define REMAIN_CAP_REG0                 0xE4
#define UPDATE_LEVE_REG                 0xE5
#define NEW_FCC_REG3                    0xE6
#define NEW_FCC_REG2                    0xE7
#define NEW_FCC_REG1                    0xE8
#define NEW_FCC_REG0                    0xE9
#define NON_ACT_TIMER_CNT_REG_SAVE      0xEA
#define OCV_VOL_VALID_REG               0xEB
#define REBOOT_CNT_REG                  0xEC
#define PCB_IOFFSET_REG                 0xED
#define MISC_MARK_REG                   0xEE
#define HALT_CNT_REG                    0xEF
#define CALC_REST_REGH                  0xF0
#define CALC_REST_REGL                  0xF1
#define DATA18_REG                      0xF2

/* Register field bit definition */
/* BAT_CTRL_REG */
#define USB_SYS_EN                      (1 << 6)

/* CHRG_CTRL_REG1 */
#define BAT_VOL_MASK                    0x70
#define BAT_CUR_MASK                    0x0f
#define CHRG_EN                         (1 << 7)

/* CHRG_CTRL_REG2 */
#define FINISH_CUR_MASK                 0xc7
#define FNS_CUR_MASK                    0xc0

/* CHRG_CTRL_REG3 */
#define CHRG_DIG_SIGNAL                 (1 << 5)
#define CHRG_T_CCCV_EN                  (1 << 2)

/* THERMAL_REG */
#define TEMP_115C                       (0x03 << 2)
#define TEMP_MASK                       0x0c
#define HOTDIE_STS                      (1 << 1)

/* USB_CTRL_REG */
#define CHRG_CT_EN                      (1 << 7)
#define SRC_CUR_MASK                    0x0f

/* TS_CTRL_REG */
#define GG_EN                           (1 << 7)
#define ADC_CUR_EN                      (1 << 6)
#define ADC_TS1_EN                      (1 << 5)
#define ADC_TS2_EN                      (1 << 4)

/* GGCON */
#define ADC_CUR_MODE                    (0x01 << 1)
#define AVG_CUR_MODE                    (0x00 << 0)
#define ADC_CAL_MIN_MSK                 0x30
#define ADC_CAL_8MIN                    (0x00 << 4)
#define OCV_SAMP_MIN_MSK                0x0c
#define OCV_SAMP_8MIN                   (0x00 << 2)

/* GGSTS */
#define VOL_INSTANT                     (1 << 0)
#define FCC_LOCK                        (1 << 5)
#define BAT_CON                         (1 << 4)
#define RELAX_STS                       (1 << 1)
#define RELAX_VOL1_UPD                  (1 << 3)
#define RELAX_VOL2_UPD                  (1 << 2)
#define RELAX_VOL12_UPD_MSK             (RELAX_VOL1_UPD | RELAX_VOL2_UPD)

/* SUP_STS_REG */
#define USB_VLIMIT_EN                   (1 << 3)
#define USB_CLIMIT_EN                   (1 << 2)
#define BAT_EXS                         (1 << 7)
#define CHARGE_OFF                      (0x00 << 4)
#define DEAD_CHARGE                     (0x01 << 4)
#define TRICKLE_CHARGE                  (0x02 << 4)
#define CC_OR_CV                        (0x03 << 4)
#define CHARGE_FINISH                   (0x04 << 4)
#define USB_OVER_VOL                    (0x05 << 4)
#define BAT_TMP_ERR                     (0x06 << 4)
#define TIMER_ERR                       (0x07 << 4)
#define USB_EXIST                       (1 << 1)
#define USB_EFF                         (1 << 0)
#define CHRG_STATUS_MSK                 0x70

/* DCDC_EN_REG2 */
#define BOOST_OTG_MASK                  ((0x3 << 5) | (0x3 << 1))
#define BOOST_OTG_ON                    ((0x3 << 5) | (0x3 << 1))
#define BOOST_OTG_OFF                   ((0x3 << 5) | (0x0 << 1))
#define BOOST_OFF_OTG_ON                ((0x3 << 5) | (0x2 << 1))
#define OTG_BOOST_SLP_ON                (0x3 << 5)

/* MISC_MARK_REG */
#define FG_INIT                         (1 << 3)
#define FG_RESET_LATE                   (1 << 1)
#define FG_RESET_NOW                    (1 << 0)
#define ALGO_REST_MODE_MSK              (0xc0)
#define ALGO_REST_MODE_SHIFT            6

/* INT_STS_REG */
#define POWERON_RTC_ALARM_IM            (1 << 5)
#define POWERON_FALL_IM                 (1 << 5)
#define POWERON_RISE_IM                 (1 << 6)
#define PLUG_OUT_IM                     (1 << 1)
#define PLUG_IN_IM                      (1 << 0)
#define PWRON_LP_INT                    (1 << 3)

/* BAT_CTRL_REG */
#define BAT_DIS_ILIM_EN                 (1 << 7)
#define BAT_DIS_ILIM_MASK               (1 << 7)
#define BAT_DIS_ILIM_DIS                (0 << 7)

#define MAX_PERCENTAGE                  100
#define MAX_INT                         0x7FFF
#define MAX_INTERPOLATE                 1000

#define RTC_STATUS_MASK                 0xFE
#define BIT_RTC_CTRL_REG_STOP_RTC_M     0x01
#define BIT_RTC_CTRL_REG_RTC_GET_TIME   (1 << 6)
#define BIT_RTC_CTRL_REG_RTC_READSEL_M  (1 << 7)
#define BIT_RTC_CTRL_REG_RTC_AMPM_MODE  (1 << 3)
#define BIT_RTC_INT_REG_IT_ALARM_M      (1 << 3)

#define SECONDS_REG_MSK                 0x7F
#define MINUTES_REG_MAK                 0x7F
#define HOURS_REG_MSK                   0x3F
#define DAYS_REG_MSK                    0x3F
#define MONTHS_REG_MSK                  0x1F
#define YEARS_REG_MSK                   0xFF
#define WEEKS_REG_MSK                   0x7
#define NUM_TIME_REGS                   7
#define NUM_ALARM_REGS                  6

/* Helper macros */
#define BAT_VOL_REGVAL(v)               ((v) << 4)
#define BAT_CUR_REGVAL(v)               ((v) << 0)
#define FNS_CUR_REGVAL(v)               ((v) << 6)
#define SRC_CUR_REGVAL(v)               ((v) << 0)

/* 'res_fac' has been *10, so we need divide 10 */
#define RES_FAC_MUX(value, res_fac) ((value) * res_fac / 10)
/* 'res_fac' has been *10, so we need 'value * 10' before divide 'res_fac' */
#define RES_FAC_DIV(value, res_fac) ((value) * 10 / res_fac)

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(ar)     (sizeof(ar)/sizeof(ar[0]))
#endif

extern const rt_int16_t bat_vol_level[7];
extern const rt_int16_t bat_cur_level[8];
extern const rt_int16_t fns_cur_level[4];
extern const rt_int16_t src_cur_level[8];

struct reg_data
{
    rt_uint8_t reg;
    rt_uint8_t mask;
    rt_uint8_t val;
};

struct rk816_fg_platform_data
{
    rt_uint32_t ocv_table[21];
    rt_uint32_t design_capacity;   /* unit: mAh */
    rt_uint32_t design_qmax;       /* unit: mAh */
    rt_uint32_t bat_res;
    rt_uint32_t power_off_thresd;

    rt_uint32_t sample_res;        /* unit: mR */
    rt_uint32_t zero_algorithm_vol;
    rt_uint32_t virtual_power;
    rt_uint32_t max_soc_offset;
    rt_uint32_t monitor_sec;       /* unit: s */
};

struct rk816_charger_platform_data
{
    rt_uint32_t sample_res;        /* unit: mR */
};

struct rk816_sensor_platform_data
{
    rt_int32_t ntc_degree_min;     /* unit: 'C */
    rt_int32_t ntc_degree_step;    /* unit: 'C */
    const rt_uint32_t *ntc_table;
    rt_uint32_t ntc_num;
};

#endif

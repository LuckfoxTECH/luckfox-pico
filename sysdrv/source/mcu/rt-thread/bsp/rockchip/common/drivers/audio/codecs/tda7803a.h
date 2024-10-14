/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  *
  ******************************************************************************
  * @file    tda7803a.c
  * @author  Jun Zeng
  * @version v0.1
  * @date    2022.11.25
  * @brief   The rt-thread codec driver for Rockchip
  ******************************************************************************
  */

#ifndef __TDA7803A_H__
#define __TDA7803A_H__

/* TDA7803A I2C Registers0 */
#define TDA7803A_REGISTER0                          (0x00)
#define CHANNEL_1_AMPLIFIER_SBI_MODE                (0x00 << 0)
#define CHANNEL_1_AMPLIFIER_AB_MODE                 (0x01 << 0)
#define CHANNEL_2_AMPLIFIER_SBI_MODE                (0x00 << 1)
#define CHANNEL_2_AMPLIFIER_AB_MODE                 (0x01 << 1)
#define CHANNEL_3_AMPLIFIER_SBI_MODE                (0x00 << 2)
#define CHANNEL_3_AMPLIFIER_AB_MODE                 (0x01 << 2)
#define CHANNEL_4_AMPLIFIER_SBI_MODE                (0x00 << 3)
#define CHANNEL_4_AMPLIFIER_AB_MODE                 (0x01 << 3)
#define CHANNEL_1_TRISTATE_MODE_OFF                 (0x00 << 4)
#define CHANNEL_1_TRISTATE_MODE_ON                  (0x01 << 4)
#define CHANNEL_2_TRISTATE_MODE_OFF                 (0x00 << 5)
#define CHANNEL_2_TRISTATE_MODE_ON                  (0x01 << 5)
#define CHANNEL_3_TRISTATE_MODE_OFF                 (0x00 << 6)
#define CHANNEL_3_TRISTATE_MODE_ON                  (0x01 << 6)
#define CHANNEL_4_TRISTATE_MODE_OFF                 (0x00 << 7)
#define CHANNEL_4_TRISTATE_MODE_ON                  (0x01 << 7)

/* TDA7803A I2C Registers1 */
#define TDA7803A_REGISTER1                          (0x01)
#define GAIN_CHANNEL_2_AND_4_GV1                    (0x00 << 0)
#define GAIN_CHANNEL_2_AND_4_GV2                    (0x01 << 0)
#define GAIN_CHANNEL_2_AND_4_GV3                    (0x02 << 0)
#define GAIN_CHANNEL_2_AND_4_GV4                    (0x03 << 0)
#define GAIN_CHANNEL_1_AND_3_GV1                    (0x00 << 2)
#define GAIN_CHANNEL_1_AND_3_GV2                    (0x01 << 2)
#define GAIN_CHANNEL_1_AND_3_GV3                    (0x02 << 2)
#define GAIN_CHANNEL_1_AND_3_GV4                    (0x03 << 2)
#define DIGITAL_GAIN_SELECTION_NO_GAIN              (0x00 << 4)
#define DIGITAL_GAIN_SELECTION_6DB_GAIN             (0x01 << 4)
#define DIGITAL_GAIN_SELECTION_12DB_GAIN            (0x02 << 4)
#define DIGITAL_GAIN_SELECTION_NOT_USED             (0x03 << 4)
#define IMPEDANCE_EFFICIENCY_OPTIMIZER_REAR_2OHM    (0x00 << 6)
#define IMPEDANCE_EFFICIENCY_OPTIMIZER_REAR_4OHM    (0x01 << 6)
#define IMPEDANCE_EFFICIENCY_OPTIMIZER_FRONT_2OHM   (0x00 << 7)
#define IMPEDANCE_EFFICIENCY_OPTIMIZER_FRONT_4OHM   (0x01 << 7)

/* TDA7803A I2C Registers2 */
#define TDA7803A_REGISTER2                          (0x02)
#define LOW_BATTERY_MUTE_THRESHOLD_5_POINT_3        (0x00 << 0)
#define LOW_BATTERY_MUTE_THRESHOLD_7_POINT_3        (0x01 << 0)
#define DISABLE_DIGITAL_MUTE_ON                     (0x00 << 2)
#define DISABLE_DIGITAL_MUTE_OFF                    (0x01 << 2)
#define CHANNEL_2_AND_4_MUTE                        (0x00 << 3)
#define CHANNEL_2_AND_4_UMUTE                       (0x01 << 3)
#define CHANNEL_1_AND_3_MUTE                        (0x00 << 4)
#define CHANNEL_1_AND_3_UMUTE                       (0x01 << 4)
#define MUTE_TIME_SETTING_1_45MS                    (0x00 << 5)
#define MUTE_TIME_SETTING_5_8MS                     (0x01 << 5)
#define MUTE_TIME_SETTING_11_6MS                    (0x02 << 5)
#define MUTE_TIME_SETTING_23_2MS                    (0x03 << 5)
#define MUTE_TIME_SETTING_46_4MS                    (0x04 << 5)
#define MUTE_TIME_SETTING_92_8MS                    (0x05 << 5)
#define MUTE_TIME_SETTING_185_5MS                   (0x06 << 5)
#define MUTE_TIME_SETTING_371_1MS                   (0x07 << 5)

/* TDA7803A I2C Registers3 */
#define TDA7803A_REGISTER3                          (0x03)
#define DIGITAL_HIGH_PASS_FILTER_DISABLE            (0x00 << 0)
#define DIGITAL_HIGH_PASS_FILTER_ENABLE             (0x01 << 0)
#define INPUT_OFFSET_DETECTION_DISABLE              (0x00 << 1)
#define INPUT_OFFSET_DETECTION_ENABLE               (0x01 << 1)
#define NOISE_GATING_FUNCTION_ENABLE                (0x00 << 2)
#define NOISE_GATING_FUNCTION_DISABLE               (0x01 << 2)
#define DIGITAL_INPUT_FORMAT_I2S_STANDARD           (0x00 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_4CH                (0x01 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_8CH_DEVICE_1       (0x02 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_8CH_DEVICE_2       (0x03 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_16CH_DEVICE_1      (0x04 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_16CH_DEVICE_2      (0x05 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_16CH_DEVICE_3      (0x06 << 3)
#define DIGITAL_INPUT_FORMAT_TDM_16CH_DEVICE_4      (0x07 << 3)
#define SAMPLE_FREQUENCY_RANGE_44100HZ              (0x00 << 6)
#define SAMPLE_FREQUENCY_RANGE_48000HZ              (0x01 << 6)
#define SAMPLE_FREQUENCY_RANGE_96000HZ              (0x02 << 6)
#define SAMPLE_FREQUENCY_RANGE_192000HZ             (0x03 << 6)

/* TDA7803A I2C Registers4 */
#define TDA7803A_REGISTER4                          (0x04)
#define DIAGNOSTIC_MODE_DISABLE                     (0x00 << 0)
#define DIAGNOSTIC_MODE_ENABLE                      (0x01 << 0)
#define CHANNEL_2_AND_4_SPEAKER_MODE                (0x00 << 1)
#define CHANNEL_2_AND_4_LINE_DRIVER_MODE            (0x01 << 1)
#define CHANNEL_1_AND_3_SPEAKER_MODE                (0x00 << 2)
#define CHANNEL_1_AND_3_LINE_DRIVER_MODE            (0x01 << 2)
#define AC_DIAGNOSTIC_DISABLE                       (0X00 << 3)
#define AC_DIAGNOSTIC_ENABLE                        (0X01 << 3)
#define AC_DIAGNOSTIC_CURRENT_THRESHOLD_HIGH        (0X00 << 4)
#define AC_DIAGNOSTIC_CURRENT_THRESHOLD_LOW         (0X01 << 4)
#define OFFSET_INFORMATION_ON_CDDIAG_PIN_YES        (0X00 << 5)
#define OFFSET_INFORMATION_ON_CDDIAG_PIN_NO         (0X01 << 5)
#define SHORT_FAULT_INFORMATION_ON_CDDIAG_PIN_YES   (0X00 << 6)
#define SHORT_FAULT_INFORMATION_ON_CDDIAG_PIN_NO    (0X01 << 6)

/* TDA7803A I2C Registers5 */
#define TDA7803A_REGISTER5                          (0x05)
#define CURRENT_CAPABILITY_ENHANCER_DISABLE         (0x00 << 1)
#define CURRENT_CAPABILITY_ENHANCER_ENABLE          (0x0F << 1)
#define THERMAL_THRESHOLD_DEFAULT                   (0x00 << 6)
#define THERMAL_THRESHOLD_TW_NEGATIVE_10            (0x01 << 6)
#define THERMAL_THRESHOLD_TW_NEGATIVE_20            (0x02 << 6)

/* TDA7803A I2C Registers6 */
#define TDA7803A_REGISTER6                          (0x06)
#define PARALLEL_MODE_CONFIG_MODE_1                 (0x00 << 2)
#define PARALLEL_MODE_CONFIG_MODE_2                 (0x01 << 2)
#define PARALLEL_MODE_CONFIG_MODE_3                 (0x02 << 2)
#define PARALLEL_MODE_CONFIG_MODE_4                 (0x03 << 2)
#define DIAGNOSITC_PULSE_STRETCH_MODE_1             (0x00 << 5)
#define DIAGNOSITC_PULSE_STRETCH_MODE_2             (0x01 << 5)
#define DIAGNOSITC_PULSE_STRETCH_MODE_3             (0x02 << 5)
#define DIAGNOSITC_PULSE_STRETCH_MODE_4             (0x03 << 5)
#define DIAGNOSITC_PULSE_STRETCH_MODE_5             (0x04 << 5)
#define DIAGNOSITC_PULSE_STRETCH_DEFAULT            (0x05 << 5)

/* TDA7803A I2C Registers7 */
#define TDA7803A_REGISTER7                          (0x07)
#define AMPLIEFIR_SWITCH_OFF                        (0x00 << 0)
#define AMPLIEFIR_SWITCH_ON                         (0x01 << 0)
#define CLIPP_LEVEL_1_REAR_CHANNELS2_4              (0x00 << 1)
#define CLIPP_LEVEL_2_REAR_CHANNELS2_4              (0x01 << 1)
#define CLIPP_LEVEL_3_REAR_CHANNELS2_4              (0x02 << 1)
#define NOT_CLIPP_FOR_REAR_CHANNELS2_4              (0x03 << 1)
#define CLIPP_LEVEL_1_REAR_CHANNELS1_3              (0x00 << 3)
#define CLIPP_LEVEL_2_REAR_CHANNELS1_3              (0x01 << 3)
#define CLIPP_LEVEL_3_REAR_CHANNELS1_3              (0x02 << 3)
#define NOT_CLIPP_FOR_REAR_CHANNELS1_3              (0x03 << 3)
#define TEMPERATURE_WARNING_TW1                     (0x00 << 5)
#define TEMPERATURE_WARNING_TW2                     (0x01 << 5)
#define TEMPERATURE_WARNING_TW3                     (0x02 << 5)
#define TEMPERATURE_WARNING_TW4                     (0x03 << 5)
#define NOT_TEMPERATURE_WARNING                     (0x04 << 5)

#endif


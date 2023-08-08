/**
 * @file tuya_pwm.h
 * @brief Common process - driver pwm
 * @version 0.1
 * @date 2019-08-20
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_PWM_H__
#define __TUYA_PWM_H__

#include "tuya_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TUYA_PWM_FREQUENCY(__PWM) ((uint32_t)(1000000000 / (__PWM)->cfg.period_ns))
#define TUYA_PWM_PERCENT(__PWM) ((__PWM)->cfg.percent)

#define TUYA_PWM_CFG(__PWM, __PIN, __FREQUENCY, __PERCENT)                                         \
	(__PWM)->cfg.pin = __PIN;                                                                      \
	(__PWM)->cfg.period_ns = (uint32_t)1000000000 / (__FREQUENCY);                                 \
	(__PWM)->cfg.percent = __PERCENT;                                                              \
	(__PWM)->cfg.pulse_ns = (uint32_t)((__PWM)->cfg.period_ns * (__PERCENT));                      \
	(__PWM)->cfg.polarity = TUYA_PWM_POSITIVE

typedef enum {
	TUYA_PWM0 = 0x00,
	TUYA_PWM1,
	TUYA_PWM2,
	TUYA_PWM3,
	TUYA_PWM4,
	TUYA_PWM5,
	TUYA_PWM6,
	TUYA_PWM7,
	TUYA_PWM8,
} tuya_pwm_port_t;

typedef enum {
	TUYA_PWM_POSITIVE = 0,
	TUYA_PWM_NEGATIVE,
} tuya_pwm_polarity_t;

typedef enum {
	TUYA_PWM_SET_CMD = TUYA_DRV_CUSTOM_CMD,
	TUYA_PWM_PERIOD_SET_CMD,
	TUYA_PWM_PULSE_SET_CMD,
	TUYA_PWM_POLARIITY_SET_CMD,
} tuya_pwm_cmd_t;

typedef struct {
	uint8_t pin;
	uint8_t polarity;
	uint32_t period_ns;
	uint32_t pulse_ns;
	float percent;
} tuya_pwm_cfg_t;

typedef struct tuya_pwm tuya_pwm_t;

typedef struct {
	int (*init)(tuya_pwm_t *pwm, tuya_pwm_cfg_t *cfg);
	int (*start)(tuya_pwm_t *pwm);
	int (*stop)(tuya_pwm_t *pwm);
	int (*control)(tuya_pwm_t *pwm, uint8_t cmd, void *arg);
	int (*deinit)(tuya_pwm_t *pwm);
} tuya_pwm_ops_t;

struct tuya_pwm {
	tuya_drv_node_t node;
	tuya_pwm_cfg_t cfg;
	tuya_pwm_ops_t *ops;
};

/**
 * @brief pwm init
 *
 * @param[in] pwm refer to tuya_pwm_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_init(tuya_pwm_t *pwm);

/**
 * @brief pwm start
 *
 * @param[in] pwm refer to tuya_pwm_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_start(tuya_pwm_t *pwm);

/**
 * @brief pwm stop
 *
 * @param[in] pwm refer to tuya_pwm_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_stop(tuya_pwm_t *pwm);

/**
 * @brief pwm control
 *
 * @param[in] pwm refer to tuya_pwm_t
 * @param[in] cmd control command
 * @param[in] arg command argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_control(tuya_pwm_t *pwm, uint8_t cmd, void *arg);

/**
 * @brief pwm deinit
 *
 * @param[in] pwm refer to tuya_pwm_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_deinit(tuya_pwm_t *pwm);

/**
 * @brief pwm set
 *
 * @param[in] pwm refer to tuya_pwm_t
 * @param[in] frequency frequency
 * @param[in] percent percent
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_set(tuya_pwm_t *pwm, float frequency, float percent);

/**
 * @brief set frequency
 *
 * @param[in] pwm refer to tuya_pwm_t
 * @param[in] frequency frequency
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_frequency_set(tuya_pwm_t *pwm, float frequency);

/**
 * @brief set duty
 *
 * @param[in] pwm refer to tuya_pwm_t
 * @param[in] percent percent
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_duty_set(tuya_pwm_t *pwm, float percent);

/**
 * @brief set polarity
 *
 * @param[in] pwm refer to tuya_pwm_t
 * @param[in] polarity refer to tuya_pwm_polarity_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_pwm_polarity_set(tuya_pwm_t *pwm, tuya_pwm_polarity_t polarity);

#ifdef __cplusplus
}
#endif

#endif

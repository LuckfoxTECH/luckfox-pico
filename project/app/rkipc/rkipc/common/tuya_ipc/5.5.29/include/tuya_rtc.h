/**
 * @file tuya_rtc.h
 * @brief Common process - driver rtc
 * @version 0.1
 * @date 2019-08-20
 *
 * @copyright Copyright 2019-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_RTC_H__
#define __TUYA_RTC_H__

#include "time.h"
#include "tuya_driver.h"

typedef struct {
	int (*init)(void);
	int (*time_get)(time_t *timestamp);
	int (*time_set)(time_t timestamp);
	int (*control)(uint8_t cmd, void *arg);
	int (*deinit)(void);
} tuya_rtc_ops_t;

/**
 * @brief rtc init
 *
 */
int tuya_rtc_init(void);

/**
 * @brief time set
 *
 * @param[in] timestamp time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_rtc_time_set(time_t timestamp);

/**
 * @brief time get
 *
 * @param[in] timestamp time
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_rtc_time_get(time_t *timestamp);

/**
 * @brief time control
 *
 * @param[in] cmd command
 * @param[in] arg command argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_rtc_control(uint8_t cmd, void *arg);

/**
 * @brief rtc deinit
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_rtc_deinit(void);

/**
 * @brief rtc register
 *
 * @param[in] ops refer to tuya_rtc_ops_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_rtc_register(tuya_rtc_ops_t *ops);

#endif

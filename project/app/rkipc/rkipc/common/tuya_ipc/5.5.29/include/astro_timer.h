/**
 * @file astro_timer.h
 * @brief Common process - astronomy timer
 * @version 0.1
 * @date 2020-03-16
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __SVC_ASTRO_TIMER__
#define __SVC_ASTRO_TIMER__

#ifdef __cplusplus
extern "C" {
#endif

#include "ty_cJSON.h"

/**
 * @brief astronomy timer init function
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_astro_timer_init(void);

/**
 * @brief astronomy timer mqtt notify
 *
 * @param[in] atop Atop command
 * @param[in] ver Version
 * @param[in] params Json parameter
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_astro_timer_mqc_notify(char *atop, char *ver, ty_cJSON *params);

/**
 * @brief astronomy timer reset
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 *
 */
int tuya_astro_timer_reset(void);

#ifdef __cplusplus
}
#endif
#endif

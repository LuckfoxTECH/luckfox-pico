/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once

#include "wiced_result.h"
#include "wiced_log.h"
#include "bluetooth_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

/*
 * PLL tuning helper primitives (create/destroy/run/rest)
 */

wiced_result_t pll_tuner_create(bt_audio_context_t *player);
wiced_result_t pll_tuner_destroy(bt_audio_context_t *player);
wiced_result_t pll_tuner_run(bt_audio_context_t *player);
wiced_result_t pll_tuner_rest(bt_audio_context_t *player);

#ifdef __cplusplus
} /* extern "C" */
#endif

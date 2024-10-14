/*
 * $ Copyright Cypress Semiconductor $
 */

#include "wiced_time.h"
#include "wiced_utilities.h"
#include "wiced_platform.h"
#include "wiced_log.h"
#include "bluetooth_audio_pll_tuning.h"
#include "wwd_debug.h"
#include "wiced_rtos.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#ifndef AUDIO_PLL_TUNER_BUFFER_AVERAGING_WINDOW
#define BUFFER_AVERAGING_WINDOW        (384)
#else
#define BUFFER_AVERAGING_WINDOW        AUDIO_PLL_TUNER_BUFFER_AVERAGING_WINDOW
#endif

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

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

static int pll_tuner_log_output(WICED_LOG_LEVEL_T level, char *logmsg)
{
    UNUSED_PARAMETER(level);
    WPRINT_APP_INFO(("%s", logmsg));
    return 0;
}


static wiced_result_t pll_tuner_get_time_cbf(audio_pll_tuner_timestamp_t *ts, void *user_context)
{
    ts->ts_reference = wiced_get_nanosecond_clock_value();
    return WICED_SUCCESS;
}


static wiced_result_t  pll_tuner_start_timer_cbf(uint32_t audio_sample_count, void *user_context)
{
    bt_audio_context_t *player = (bt_audio_context_t *) user_context;
    player->pll_tuner_period_counter = 0;
    return wiced_audio_timer_enable(audio_sample_count);
}


static wiced_result_t pll_tuner_stop_timer_cbf(void *user_context)
{
    UNUSED_PARAMETER(user_context);
    return wiced_audio_timer_disable();
}


static wiced_result_t pll_tuner_wait_for_period_cbf(uint32_t timeout_ms, void *user_context)
{
    UNUSED_PARAMETER(user_context);
    return wiced_audio_timer_get_frame_sync(timeout_ms);
}


static wiced_result_t pll_tuner_get_buffer_level_cbf(uint32_t *level_in_bytes, void *user_context)
{
    bt_audio_context_t *player = (bt_audio_context_t *) user_context;
    wiced_result_t result;
    uint32_t buffer_weight;
    uint32_t queue_occupancy;

    result = wiced_audio_get_current_buffer_weight(player->bluetooth_audio_session_handle, &buffer_weight);
    wiced_action_jump_when_not_true(result == WICED_SUCCESS, _exit,  WPRINT_APP_ERROR(("wiced_audio_get_current_buffer_weight() failed !\n")));
    *level_in_bytes = buffer_weight;

    result = wiced_rtos_get_queue_occupancy(&player->queue, &queue_occupancy);
    wiced_action_jump_when_not_true(result == WICED_SUCCESS, _exit,  WPRINT_APP_ERROR(("wiced_rtos_get_queue_occupancy() failed !\n")));
    queue_occupancy *= player->pcm_packet_length;
    *level_in_bytes += queue_occupancy;

    player->pll_tuner_period_counter++;
    if (player->pll_tuner_period_counter == BUFFER_AVERAGING_WINDOW)
    {
        player->pll_tuner_period_counter = 0;
        wiced_log_msg(WLF_AUDIO, WICED_LOG_DEBUG1, "(%06lu, %05lu)\n", buffer_weight, queue_occupancy);
    }

_exit:
    return result;
}


static wiced_result_t pll_tuner_set_ppb_cbf(int32_t ppb, void *user_context)
{
    bt_audio_context_t *player = (bt_audio_context_t *) user_context;
    return wiced_audio_set_pll_fractional_divider(player->bluetooth_audio_session_handle, (float)(ppb / 1000.0f));
}


wiced_result_t pll_tuner_create(bt_audio_context_t *player)
{
    player->pll_tuner_init_params.adjustment_ppm_max                 = AUDIO_PLL_TUNER_DEFAULT_ADJ_PPM_MAX;
    player->pll_tuner_init_params.adjustment_ppm_min                 = AUDIO_PLL_TUNER_DEFAULT_ADJ_PPM_MIN;
    player->pll_tuner_init_params.adjustment_ppb_per_msec            = AUDIO_PLL_TUNER_DEFAULT_ADJ_RATE_PPB_PER_MSEC;
    player->pll_tuner_init_params.adjustment_attack_rate             = AUDIO_PLL_TUNER_DEFAULT_ADJ_ATTACK_RATE;
    player->pll_tuner_init_params.adjustment_decay_rate              = AUDIO_PLL_TUNER_DEFAULT_ADJ_DECAY_RATE;
    player->pll_tuner_init_params.level_correction_threshold_high    = AUDIO_PLL_TUNER_DEFAULT_LEVEL_CORRECTION_THRES_MAX;
    player->pll_tuner_init_params.level_correction_threshold_low     = AUDIO_PLL_TUNER_DEFAULT_LEVEL_CORRECTION_THRES_MIN;
    player->pll_tuner_init_params.adjustment_proportional_gain       = AUDIO_PLL_TUNER_DEFAULT_ADJ_PROPORTIONAL_GAIN;
    player->pll_tuner_init_params.adjustment_integral_gain           = AUDIO_PLL_TUNER_DEFAULT_ADJ_INTEGRAL_GAIN;

    player->pll_tuner_init_params.user_context                       = player;
    player->pll_tuner_init_params.timer_start                        = pll_tuner_start_timer_cbf;
    player->pll_tuner_init_params.timer_stop                         = pll_tuner_stop_timer_cbf;
    player->pll_tuner_init_params.period_wait                        = pll_tuner_wait_for_period_cbf;
    player->pll_tuner_init_params.buffer_level_get                   = pll_tuner_get_buffer_level_cbf;
    player->pll_tuner_init_params.ppb_set                            = pll_tuner_set_ppb_cbf;
    player->pll_tuner_init_params.get_time                           = pll_tuner_get_time_cbf;

    wiced_init_nanosecond_clock();

    wiced_log_init(WICED_LOG_ERR, pll_tuner_log_output, NULL);

    return audio_pll_tuner_init(&player->pll_tuner_init_params, &player->pll_tuner);
}


wiced_result_t pll_tuner_destroy(bt_audio_context_t *player)
{
    wiced_result_t result;
    result = audio_pll_tuner_deinit(player->pll_tuner);
    player->pll_tuner = NULL;

    wiced_deinit_nanosecond_clock();

    return result;
}


wiced_result_t pll_tuner_run(bt_audio_context_t *player)
{
    wiced_result_t result;
    uint32_t       target_buffer_level_msecs = (player->audio_buffer_target_percent * player->audio_buffer_max_duration_msecs) / 100;

    player->pll_tuner_start_params.sample_rate               = player->bluetooth_audio_config.sample_rate;
    player->pll_tuner_start_params.bits_per_sample           = player->bluetooth_audio_config.bits_per_sample;
    player->pll_tuner_start_params.channels                  = player->bluetooth_audio_config.channels;
    player->pll_tuner_start_params.target_buffer_level_bytes = (uint32_t) MILLISECONDS_TO_BYTES(target_buffer_level_msecs, player->bluetooth_audio_config.sample_rate, player->bluetooth_audio_config.frame_size);

    WPRINT_APP_INFO(("Audio PLL tuner target buffer level: %lu bytes\n", player->pll_tuner_start_params.target_buffer_level_bytes));

    wiced_reset_nanosecond_clock();

    result = audio_pll_tuner_start(player->pll_tuner, &player->pll_tuner_start_params);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("audio_pll_tuner_start() failed !\n"));
    }

    return result;
}


wiced_result_t pll_tuner_rest(bt_audio_context_t *player)
{
    wiced_result_t result;

    result = audio_pll_tuner_stop(player->pll_tuner);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("audio_pll_tuner_stop() failed !\n"));
    }

    return result;
}

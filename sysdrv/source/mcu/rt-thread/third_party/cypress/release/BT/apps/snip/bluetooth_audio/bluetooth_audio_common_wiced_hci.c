/*
 * $ Copyright Cypress Semiconductor $
 */
#include "wiced.h"
#include "wiced_rtos.h"
#include "wiced_audio.h"
#include "bluetooth_audio.h"

/*****************************************************************************
**
**  Name:           bluetooth_audio_common_wiced_hci.c
**
**  Description:
**
*****************************************************************************/

/******************************************************
 *                      Macros
 ******************************************************/
#define VOLUME_CONVERSION(step,level,min)                    ((double)step*(double)level + min)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/
#define BT_AUDIO_DEFAULT_PERIOD_SIZE        ( 1024 )

/******************************************************
 *                 Type Definitions
 ******************************************************/
/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
/******************************************************
 *               Variables Definitions
 ******************************************************/
bt_audio_context_t player;

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_bool_t is_bt_audio_player_initialized(void)
{
    return (wiced_bool_t)(player.state > BT_AUDIO_DEVICE_STATE_IDLE);
}

/*This is stub function*/
wiced_result_t  bt_audio_init_player(void)
{
    wiced_result_t result = WICED_SUCCESS;

    if (player.state == BT_AUDIO_DEVICE_STATE_UNINITIALIZED)
    {
        player.state = BT_AUDIO_DEVICE_STATE_IDLE;
    }
    WPRINT_APP_INFO(("bt_audio_init_player\n"));
    return result;
}

wiced_result_t bt_audio_configure_player(bt_audio_config_t *p_audio_config)
{
    wiced_result_t    result;
    wiced_audio_config_t wiced_audio_conf = {0, };
    //WPRINT_APP_INFO(("bt_audio_configure_player: INIT\n"));

    if (p_audio_config == NULL)
        return WICED_BADARG;

    if (player.state != BT_AUDIO_DEVICE_STATE_IDLE)
        return WICED_ERROR;

    /* Initialize and configure audio framework for needed audio format */
    result = wiced_audio_init(PLATFORM_DEFAULT_BT_AUDIO_OUTPUT, &player.bluetooth_audio_session_handle, BT_AUDIO_DEFAULT_PERIOD_SIZE);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_configure_player:  Error Initializing Wiced Audio Framework[err: %d]\n", result));
        return result;
    }

    memcpy(&player.bluetooth_audio_config, p_audio_config, sizeof(bt_audio_config_t));
    WPRINT_APP_INFO(("bt_audio_configure_player: config sample_rate:%u channels:%d bps:%d\n", (unsigned int) player.bluetooth_audio_config.sample_rate,
                     (int)player.bluetooth_audio_config.channels, (int)player.bluetooth_audio_config.bits_per_sample));

    wiced_audio_conf.sample_rate     = player.bluetooth_audio_config.sample_rate;
    wiced_audio_conf.channels        = player.bluetooth_audio_config.channels;
    wiced_audio_conf.bits_per_sample = player.bluetooth_audio_config.bits_per_sample;
    wiced_audio_conf.frame_size      = player.bluetooth_audio_config.frame_size;

    result = wiced_audio_configure(player.bluetooth_audio_session_handle, &wiced_audio_conf);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_configure_player: Error configuring Wiced Audio framework [err: %d]\n", result));
        return result;
    }
    player.state = BT_AUDIO_DEVICE_STATE_CONFIGURED;

    /* Update the Volume */
    result = bt_audio_update_player_volume(player.bluetooth_audio_config.volume);

    return result;
}

wiced_result_t bt_audio_stop_player(void)
{
    if (player.state <= BT_AUDIO_DEVICE_STATE_IDLE)
        return WICED_SUCCESS;

    //Send stop event to reset the player state to IDLE
    wiced_rtos_set_event_flags(&player.events, BT_AUDIO_EVENT_STOP_PLAYER);
    wiced_rtos_get_semaphore(&player.wait_for_cmd_completion, 34); //max time player can wait for buffer @8K

    return WICED_SUCCESS;
}
wiced_result_t bt_audio_update_player_volume(uint8_t level)
{
    wiced_result_t result = WICED_ERROR;
    double min_volume_db = 0.0, max_volume_db = 0.0, volume_db, step_db;

    if (level > BT_AUDIO_VOLUME_MAX)
        level = BT_AUDIO_DEFAULT_VOLUME;

    if (is_bt_audio_player_initialized() != WICED_TRUE)
    {
        WPRINT_APP_INFO(("bt_audio_update_player_volume: Player not initialized\n"));
        return result;
    }

    result = wiced_audio_get_volume_range(player.bluetooth_audio_session_handle, &min_volume_db, &max_volume_db);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_update_player_volume: wiced_audio_get_volume_range failed\n"));
        return result;
    }

    step_db = (double)(max_volume_db - min_volume_db) / (BT_AUDIO_VOLUME_MAX);
    volume_db = (double) VOLUME_CONVERSION(step_db, level, min_volume_db);

    return wiced_audio_set_volume(player.bluetooth_audio_session_handle, volume_db);
}

/*This is stub function*/
void bt_audio_player_task(uint32_t args)
{
    UNUSED_PARAMETER(args);
}

/*This is stub function*/
wiced_result_t bt_audio_write_to_decoder_queue(bt_audio_codec_data_t *audio)
{
    UNUSED_PARAMETER(audio);

    return WICED_SUCCESS;
}

/*This is stub function*/
wiced_result_t bt_audio_decoder_context_init(void)
{
    return WICED_SUCCESS;
}

/*This is stub function*/
wiced_result_t bt_audio_reset_decoder_config(void)
{
    return WICED_SUCCESS;
}

/*This is stub function*/
wiced_result_t bt_audio_configure_decoder(wiced_bt_a2dp_codec_info_t *decoder_config)
{
    UNUSED_PARAMETER(decoder_config);
    return WICED_SUCCESS;
}

/*This is stub function*/
void bt_audio_decoder_task(uint32_t arg)
{
    UNUSED_PARAMETER(arg);
}

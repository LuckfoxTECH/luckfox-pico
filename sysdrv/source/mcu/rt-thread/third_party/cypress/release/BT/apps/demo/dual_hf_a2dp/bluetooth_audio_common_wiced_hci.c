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
bt_audio_context_t recorder;
/******************************************************
 *               Function Definitions
 ******************************************************/
wiced_bool_t is_bt_audio_player_initialized(void)
{
    return (wiced_bool_t)(player.state > BT_AUDIO_DEVICE_STATE_IDLE);
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
void bta_wiced_audio_recorder_task(uint32_t context)
{

}
void bt_audio_write_to_player_buffer(bt_audio_codec_data_t *pcm)
{
    UNUSED_PARAMETER(pcm);
}


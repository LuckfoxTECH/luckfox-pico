/*
 * $ Copyright Cypress Semiconductor $
 */
#include "wiced.h"
#include "wiced_rtos.h"
#include "wiced_audio.h"
#include "bluetooth_audio.h"

#ifdef USE_MEM_POOL
#include "mem_pool.h"
#endif
#include "wwd_debug.h"
/*****************************************************************************
**
**  Name:           bt_audio_wiced_audio_player.c
**
**  Description:    BTA AVK interface to the Wiced audio subsystem
**
*****************************************************************************/

/******************************************************
 *                      Macros
 ******************************************************/
#define VOLUME_CONVERSION(step,level,min)                    ((double)step*(double)level + min)

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
static wiced_bool_t read_bluetooth_audio_data(uint8_t *buffer, uint16_t *size);

/******************************************************
 *               Variables Definitions
 ******************************************************/
bt_audio_context_t player;
#ifdef USE_MEM_POOL
extern bt_buffer_pool_handle_t  mem_pool;
#endif

/******************************************************
 *               Function Definitions
 ******************************************************/
wiced_result_t bt_audio_update_player_volume(uint8_t level)
{
    wiced_result_t result = WICED_ERROR;
    double min_volume_db = 0.0, max_volume_db = 0.0, volume_db, step_db;

    if (level > BT_AUDIO_VOLUME_MAX)
        level = BT_AUDIO_DEFAULT_VOLUME;

    if (is_bt_audio_device_initialized(BT_AUDIO_DEVICE_PLAYER) != WICED_TRUE)
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

extern void bt_audio_tx_recorder_data_to_sco(bt_audio_codec_data_t *pcm);
void bt_audio_write_to_player_buffer(bt_audio_codec_data_t *pcm)
{
    static uint8_t q_count = 0;
    wiced_result_t result;
    //rt_kprintf("w%d\n", is_bt_audio_device_initialized(BT_AUDIO_DEVICE_PLAYER));
    //if (0)
    if (WICED_TRUE == is_bt_audio_device_initialized(BT_AUDIO_DEVICE_PLAYER))
    {
        result = wiced_rtos_push_to_queue(&player.queue, &pcm, WICED_NO_WAIT);
        if (result != WICED_SUCCESS)
        {
            WPRINT_APP_DEBUG(("%s: push to queue failed, freeing buffer.\n", __func__));
#ifdef USE_MEM_POOL
            bt_buffer_pool_free_buffer(pcm);
#else
            rt_free(pcm);
#endif
            return;
        }
        if (player.state == BT_AUDIO_DEVICE_STATE_STARTED)
        {
            return;
        }
        else
        {
            q_count++;
            if (q_count >= BT_AUDIO_QUEUE_THRESHOLD)
            {
                bt_audio_start_loop(BT_AUDIO_DEVICE_PLAYER);
                player.state = BT_AUDIO_DEVICE_STATE_STARTED;
                rt_kprintf("%s: Sending Player Start Event, pcm_length = %d\n", __func__, (int)pcm->length);
                //WPRINT_APP_INFO ( ("%s: Sending Player Start Event, pcm_length = %d\n", __func__, (int)pcm->length) );
                q_count = 0;
            }
        }
    }
    else
    {
#ifdef USE_MEM_POOL
        bt_buffer_pool_free_buffer(pcm);
#else

#if 0
        //this function only used to verify lookback function of SCO over hci, not in release code.
        bt_audio_tx_recorder_data_to_sco(pcm);
#endif
        WPRINT_APP_DEBUG(("%s: freeing buffer.\n", __func__));
        rt_free(pcm);
#endif
    }
}


static wiced_bool_t read_bluetooth_audio_data(uint8_t *buffer, uint16_t *size)
{
    static bt_audio_codec_data_t *pcm = NULL;
    uint32_t        current_buffer_pos = 0;
    wiced_result_t result;
    uint32_t        wait_time_limit;
    wiced_bool_t    done = WICED_FALSE;
    uint32_t copy_len;

    /* If there is a chunk which was not written in the buffer on the previous call */
    /* write it know */
    if (pcm != NULL)
    {
        if (pcm->length != 0)
        {
            memcpy(buffer, (pcm->data + pcm->offset), pcm->length);
            current_buffer_pos = pcm->length;
        }
#ifdef USE_MEM_POOL
        bt_buffer_pool_free_buffer(pcm);
#else
        rt_free(pcm);
#endif
        pcm = NULL;
    }
    else
    {
        current_buffer_pos = 0;
    }

    wait_time_limit = ((double)((*size) * NUM_MSECONDS_IN_SECONDS)) / ((double)(player.bluetooth_audio_config.sample_rate * 4));

    /* Read continuously packets from the bluetooth audio queue */
    while (wait_time_limit > 0)
    {
        result = wiced_rtos_pop_from_queue(&player.queue, &pcm, 1);
        if (result != WICED_SUCCESS)
        {
            wait_time_limit--;
            continue;
        }

        /*Calculate how many bytes can be copied*/
        copy_len = ((current_buffer_pos + pcm->length) <= *size) ? pcm->length : (*size - current_buffer_pos);

        memcpy(&buffer[current_buffer_pos], pcm->data, copy_len);
        current_buffer_pos += copy_len;

        if (copy_len == pcm->length)
        {
#ifdef USE_MEM_POOL
            bt_buffer_pool_free_buffer(pcm);
#else
            rt_free(pcm);
#endif
            pcm = NULL;
        }
        else //if(copy_len < pcm->length)
        {
            pcm->offset += copy_len;
            pcm->length -= copy_len;
        }

        if (current_buffer_pos == *size)
        {
            done = WICED_TRUE;
            break;
        }
    }

    if (done != WICED_TRUE)
    {
        /* If the time to fill a buffer with at least one audio period is expired
         * Fill left part of the buffer with silence
         */
        memset(&buffer[current_buffer_pos], 0x00, *size - current_buffer_pos);
        done = WICED_TRUE;
    }

    return done;
}

static uint8_t audio_buffer[4 * 1024];
extern void a2d_audio_play(uint8_t *data, int len);

void bt_audio_player_task(uint32_t args)
{
    uint8_t          *ptr;
    uint16_t          n;
    uint16_t          available_bytes;
    wiced_result_t    result;
    uint32_t   flags_set;

    for (;;)
    {
        /* Wait for an audio ready event( all bluetooth audio buffers must be filled before starting playback ) */
        result = wiced_rtos_wait_for_event_flags(&player.events, BT_AUDIO_EVENT_ALL, &flags_set, WICED_TRUE, WAIT_FOR_ANY_EVENT, WICED_WAIT_FOREVER);
        if (result != WICED_SUCCESS)
            continue;

        if ((flags_set & BT_AUDIO_EVENT_DEINIT) == BT_AUDIO_EVENT_DEINIT)
        {
            WPRINT_APP_INFO(("[BT-PLAYER] exit bluetooth playback \n"));
            break;
        }

        WPRINT_APP_INFO(("[BT-PLAYER] Start bluetooth playback (flags_set: 0x%x) \n", flags_set));
        /* Audio thread is started, now it will read contents of queued bluetooth audio buffers and give it for further processing
         *  to the audio framework */
        while (!(flags_set & BT_AUDIO_EVENT_STOP_LOOP))
        {
            available_bytes = 1 * 512;
            while (available_bytes > 0)
            {
                n = available_bytes;
                ptr = audio_buffer;
                if (wiced_rtos_wait_for_event_flags(&player.events, BT_AUDIO_EVENT_STOP_LOOP, &flags_set, WICED_FALSE, WAIT_FOR_ANY_EVENT, WICED_NO_WAIT) == WICED_SUCCESS)
                {
                    if ((flags_set & BT_AUDIO_EVENT_STOP_LOOP) == BT_AUDIO_EVENT_STOP_LOOP)
                    {
                        break;
                    }
                }

                if (WICED_FALSE == read_bluetooth_audio_data(ptr, &n))
                    break;

                a2d_audio_play(ptr, n);
                available_bytes -= n;

            } /* while (available_bytes > 0) */
            result = wiced_rtos_wait_for_event_flags(&player.events, BT_AUDIO_EVENT_ALL, &flags_set, WICED_TRUE, WAIT_FOR_ANY_EVENT, WICED_NO_WAIT);
        }
        bt_audio_stop(BT_AUDIO_DEVICE_PLAYER);
        WPRINT_APP_INFO(("[bt_audio_wiced_audio_player_task] out of PLAYER loop\n"));
    } /*end for (;;)*/
}


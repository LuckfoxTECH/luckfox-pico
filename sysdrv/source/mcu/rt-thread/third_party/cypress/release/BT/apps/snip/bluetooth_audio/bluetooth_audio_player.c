/*
 * $ Copyright Cypress Semiconductor $
 */
#include "wiced.h"
#include "wiced_rtos.h"
#include "wiced_audio.h"
#include "bluetooth_audio.h"
#include <rtthread.h>

#ifdef USE_MEM_POOL
#include "mem_pool.h"
#endif

#ifdef USE_AUDIO_PLL
#include "bluetooth_audio_pll_tuning.h"
#endif
#include "wwd_debug.h"
#include "hal_base.h"
static unsigned int pop_push_play_queue = 0;

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
 *                   Enumerations
 ******************************************************/
#ifdef USE_AUDIO_PLL
typedef enum
{
    AUDIO_PLL_CONSOLE_CMD_TARGET    = 0,
    AUDIO_PLL_CONSOLE_CMD_THRESHOLD,
    AUDIO_PLL_CONSOLE_CMD_LOG,
    AUDIO_PLL_CONSOLE_CMD_MAX,
} audio_pll_console_cmd_t;
#endif /* USE_AUDIO_PLL */

/******************************************************
 *                    Constants
 ******************************************************/
#define BT_AUDIO_DEFAULT_PERIOD_SIZE        ( 4 * 1024 )
#define BT_AUDIO_BUFFER_SIZE                ( 4*BT_AUDIO_DEFAULT_PERIOD_SIZE )
#define WAIT_TIME_DELTA                     (20)
#define BT_AUDIO_PLAYER_QUEUE_MAX_SIZE      (64*2)
#define BT_AUDIO_PLAYER_QUEUE_THRESHOLD     (10)
#define BT_AUDIO_BUFFER_UNDERRUN_THRESHOLD  ( 2*BT_AUDIO_DEFAULT_PERIOD_SIZE )

/* only used for initial sizing of PCM packet queue */
#define BT_AUDIO_DEFAULT_SAMPLE_RATE_HZ     (44100)
#define BT_AUDIO_DEFAULT_SAMPLE_SIZE_BYTE   (4)
#define BT_AUDIO_DEFAULT_SAMPLE_PER_PACKET  (128)

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/
#ifdef USE_AUDIO_PLL
typedef struct
{
    char *cmd;
} cmd_lookup_t;
#endif /* USE_AUDIO_PLL */

/******************************************************
 *               Function Definitions
 ******************************************************/
static wiced_bool_t read_bluetooth_audio_data(uint8_t *buffer, uint16_t *size);

/******************************************************
 *               Variables Definitions
 ******************************************************/
static bt_audio_context_t player;
#ifdef USE_MEM_POOL
extern bt_buffer_pool_handle_t  mem_pool;
#endif

#ifdef USE_AUDIO_PLL
static cmd_lookup_t command_lookup[AUDIO_PLL_CONSOLE_CMD_MAX] =
{
    { "target"    },
    { "threshold" },
    { "log"       },
};
#endif /* USE_AUDIO_PLL */

/******************************************************
 *               Function Definitions
 ******************************************************/

#ifdef USE_AUDIO_PLL
int audio_pll_console_command(int argc, char *argv[])
{
    int i;
    int percent;
    int log_level;

    for (i = 0; i < AUDIO_PLL_CONSOLE_CMD_MAX; ++i)
    {
        if (strcmp(command_lookup[i].cmd, argv[0]) == 0)
        {
            break;
        }
    }

    if (i >= AUDIO_PLL_CONSOLE_CMD_MAX)
    {
        WPRINT_APP_ERROR(("Unrecognized command: %s\n", argv[0]));
        return 0;
    }

    switch (i)
    {
    case AUDIO_PLL_CONSOLE_CMD_TARGET:
        percent = atoi(argv[1]);
        if (percent < 1 || percent > 100)
        {
            WPRINT_APP_ERROR(("Buffer level target must be between 1% and 100% (inclusive)\n"));
        }
        else
        {
            player.audio_buffer_target_percent = percent;
        }
        break;

    case AUDIO_PLL_CONSOLE_CMD_THRESHOLD:
        percent = atoi(argv[1]);
        if (percent < 1 || percent > 100)
        {
            WPRINT_APP_ERROR(("Buffer playback threshold must be between 1% and 100% (inclusive)\n"));
        }
        else
        {
            player.audio_buffer_threshold_percent = percent;
        }
        break;

    case AUDIO_PLL_CONSOLE_CMD_LOG:
        log_level = atoi(argv[1]);
        if (log_level < WICED_LOG_OFF || log_level > WICED_LOG_DEBUG4)
        {
            WPRINT_APP_ERROR(("Log level must be between %d and %d (inclusive)\n", WICED_LOG_OFF, WICED_LOG_DEBUG4));
        }
        else
        {
            wiced_log_set_facility_level(WLF_AUDIO, log_level);
        }
        break;

    default:
        break;
    }

    return 0;
}
#endif /* USE_AUDIO_PLL */

wiced_bool_t is_bt_audio_player_initialized(void)
{
    return (wiced_bool_t)(player.state > BT_AUDIO_DEVICE_STATE_IDLE);
}

wiced_bool_t is_bt_audio_player_prepared(void)
{
    return (wiced_bool_t)(player.state == BT_AUDIO_DEVICE_STATE_CONFIGURED ||
                          player.state == BT_AUDIO_DEVICE_STATE_STOPPED);
}

wiced_result_t  bt_audio_init_player(void)
{
    wiced_result_t result = WICED_SUCCESS;

    if (player.state == BT_AUDIO_DEVICE_STATE_UNINITIALIZED)
    {
        player.queue_max_entry_count = BT_AUDIO_PLAYER_QUEUE_MAX_SIZE;
        player.queue_threshold_count = BT_AUDIO_PLAYER_QUEUE_THRESHOLD;

        result = wiced_rtos_init_queue(&player.queue, "PLAYER_QUEUE", sizeof(bt_audio_codec_data_t *), player.queue_max_entry_count);
        result = wiced_rtos_init_event_flags(&player.events);
        result = wiced_rtos_init_semaphore(&player.wait_for_cmd_completion);
        player.state = BT_AUDIO_DEVICE_STATE_IDLE;
    }
    WPRINT_APP_INFO(("bt_audio_init_player: pcm_queue_entry_count=%lu, result=%d queue: %p\n", player.queue_max_entry_count, result, player.queue));
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
    result = wiced_audio_init(0xab00, &player.bluetooth_audio_session_handle, BT_AUDIO_DEFAULT_PERIOD_SIZE);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_configure_player:  Error Initializing Wiced Audio Framework[err: %d]\n", result));
        return result;
    }

    result = wiced_audio_create_buffer(player.bluetooth_audio_session_handle, BT_AUDIO_BUFFER_SIZE, NULL, NULL);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_configure_player: Error Registering Buffer to Wiced Audio Framework [err: %d]\n", result));
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

    /*
     * Reset stats gathering
     */

    player.buffer_unavail_count    = 0;
    player.underrun_count          = 0;
    player.overrun_count           = 0;
    player.silence_insertion_count = 0;

    player.single_audio_period_duration = (uint32_t)BYTES_TO_MILLISECONDS(BT_AUDIO_DEFAULT_PERIOD_SIZE, player.bluetooth_audio_config.sample_rate, player.bluetooth_audio_config.frame_size);
    WPRINT_APP_INFO(("single_audio_period_duration %d frame_size : %d\n",
                     player.single_audio_period_duration,
                     player.bluetooth_audio_config.frame_size));

    return result;
}


void bt_audio_write_to_player_buffer(bt_audio_codec_data_t *pcm)
{
    static uint8_t q_count = 0;
    wiced_result_t result;

    if (WICED_TRUE == is_bt_audio_player_initialized())
    {
        //rt_kprintf("+%d \n", pop_push_play_queue++);
        bt_buffer_mem.decoder_buffer_size += pcm->length;
        result = wiced_rtos_push_to_queue(&player.queue, &pcm, WICED_NO_WAIT);
        //printf("push[0x%08x %d]\n", HAL_GetTick(), pcm->length);
        if (result != WICED_SUCCESS)
        {
            WPRINT_APP_INFO(("%s: push to queue failed, freeing buffer.\n", __func__));
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
            if (q_count >= player.queue_threshold_count)
            {
                wiced_rtos_set_event_flags(&player.events, BT_AUDIO_EVENT_START_PLAYER);
                player.state = BT_AUDIO_DEVICE_STATE_STARTED;
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
        rt_free(pcm);
#endif
    }
}


wiced_result_t bt_audio_stop_player(void)
{
    if (player.state <= BT_AUDIO_DEVICE_STATE_IDLE)
        return WICED_SUCCESS;

    WPRINT_APP_INFO(("bt_audio_stop_player\n"));

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


static wiced_result_t bt_audio_start(void)
{
    wiced_result_t    result = WICED_ERROR;

    if (is_bt_audio_player_prepared())
        result = wiced_audio_start(player.bluetooth_audio_session_handle);

    if (result == WICED_SUCCESS)
        player.state = BT_AUDIO_DEVICE_STATE_STARTED;

    WPRINT_APP_INFO(("bt_audio_start: Player start state [result: %d]\n", result));
    return result;
}

static void bt_audio_stop(void)
{
    uint8_t stop_audio = 0;
    bt_audio_codec_data_t *pcm = NULL;

    if ((player.state > BT_AUDIO_DEVICE_STATE_IDLE))
    {
        if (player.state == BT_AUDIO_DEVICE_STATE_STARTED)
            stop_audio = 1;

        if (stop_audio)
        {
            wiced_audio_stop(player.bluetooth_audio_session_handle);
            if (WICED_SUCCESS != wiced_rtos_is_queue_empty(&player.queue))
            {
                //rt_kprintf("bt_audio_stop %d \n", pop_push_play_queue);
                while (WICED_SUCCESS == wiced_rtos_pop_from_queue(&player.queue, &pcm, WICED_NO_WAIT))
                {
                    if (pcm != NULL)
                    {
#ifdef USE_MEM_POOL
                        bt_buffer_pool_free_buffer(pcm);
#else
                        rt_free(pcm);
#endif
                        pcm = NULL;
                    }
                }
            }
        }
        wiced_audio_deinit(player.bluetooth_audio_session_handle);
        player.state = BT_AUDIO_DEVICE_STATE_IDLE;
    }
    wiced_rtos_set_semaphore(&player.wait_for_cmd_completion);
}


wiced_result_t  bt_audio_deinit_player(void)
{
    wiced_result_t ret = WICED_SUCCESS;

#ifdef USE_AUDIO_PLL
    if (pll_tuner_destroy(&player) != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("bt_audio_deinit_player: pll_tuner_destroy() failed with %d\n", result));
    }
#endif /* USE_AUDIO_PLL */

    wiced_rtos_deinit_queue(&player.queue);
    wiced_rtos_deinit_event_flags(&player.events);
    wiced_rtos_deinit_semaphore(&player.wait_for_cmd_completion);

    /* reset player control block */
    memset(&player, 0, sizeof(bt_audio_context_t));
    player.state = BT_AUDIO_DEVICE_STATE_UNINITIALIZED;
    return ret;
}


static wiced_bool_t read_bluetooth_audio_data(uint8_t *buffer, uint16_t *size)
{
    static bt_audio_codec_data_t *pcm = NULL;
    uint32_t       current_buffer_pos = 0;
    //uint32_t flags_set;
    wiced_result_t result;
    uint32_t       wait_time_limit    = player.single_audio_period_duration;
    wiced_bool_t   done               = WICED_FALSE;
    uint32_t       copy_len;
    uint32_t       audio_buffer_weight = 0;
    int free_cnt = 0;
    int pop_cnt = 0;

    //wait_time_limit = 200;

    UNUSED_VARIABLE(audio_buffer_weight);

    /* If there is a chunk which was not written in the buffer on the previous call */
    /* write it now */
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

    /* Read continuously packets from the bluetooth audio queue */
    while (wait_time_limit > 0)
    {
        result = wiced_rtos_pop_from_queue(&player.queue, &pcm, 1);
        if (result != WICED_SUCCESS)
        {
            wait_time_limit--;
            continue;
        }
        else
        {
            bt_buffer_mem.play_buffer_size += pcm->length;
            pop_cnt++;
        }

        //rt_kprintf("[%d]\n", pcm->length);

        /*Calculate how many bytes can be copied*/
        copy_len = ((current_buffer_pos + pcm->length) <= *size) ? pcm->length : (*size - current_buffer_pos);

        memcpy(&buffer[current_buffer_pos], pcm->data, copy_len);
        current_buffer_pos += copy_len;

        if (copy_len == pcm->length)
        {
#ifdef USE_MEM_POOL
            bt_buffer_pool_free_buffer(pcm);
#else
            free_cnt++;
            bt_buffer_mem.play_buffer--;
            bt_buffer_mem.play_buffer_dec++;
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
        //rt_kprintf("====fill zero (%d %d)====\n", player.single_audio_period_duration, wait_time_limit);
        bt_buffer_mem.free_size += (*size - current_buffer_pos);
        memset(&buffer[current_buffer_pos], 0x00, *size - current_buffer_pos);
        player.silence_insertion_count++;
        done = WICED_TRUE;
    }

    if (free_cnt != pop_cnt)
        rt_kprintf("$ [%d %d %d %d]\n", *size, free_cnt, pop_cnt, current_buffer_pos);

    return done;
}

static uint8_t audio_buffer[4 * 1024];

extern void a2d_audio_play(uint8_t *data, int len);
void bt_audio_player_task(uint32_t args)
{
    uint8_t       *ptr;
    uint16_t       n;
    uint16_t       available_bytes;
    wiced_result_t result;
    uint32_t       flags_set;

    for (;;)
    {
        /* Wait for an audio ready event( all bluetooth audio buffers must be filled before starting playback ) */
        flags_set = 0;
        result = wiced_rtos_wait_for_event_flags(&player.events, BT_AUDIO_EVENT_ALL, &flags_set, WICED_TRUE, WAIT_FOR_ANY_EVENT, WICED_WAIT_FOREVER);
        if (result != WICED_SUCCESS)
            continue;

        WPRINT_APP_INFO(("[BT-PLAYER] Start bluetooth playback \n"));
        /* Audio thread is started, now it will read contents of queued bluetooth audio buffers and give it for further processing
         *  to the audio framework */
        while (!(flags_set & BT_AUDIO_EVENT_STOP_PLAYER))
        {
            available_bytes = 4 * 1024;
            while (available_bytes > 0)
            {
                n = available_bytes;
                ptr = audio_buffer;
                if (WICED_FALSE == read_bluetooth_audio_data(ptr, &n))
                {
                    break;
                }

                //WPRINT_APP_INFO(("%d\n", player.state));

                a2d_audio_play(ptr, n);
                available_bytes -= n;

            } /* while (available_bytes > 0) */
            result = wiced_rtos_wait_for_event_flags(&player.events, BT_AUDIO_EVENT_ALL, &flags_set, WICED_TRUE, WAIT_FOR_ANY_EVENT, WICED_NO_WAIT);
        }
        bt_audio_stop();
        WPRINT_APP_INFO(("[%s] out of playback loop\n", __func__));
        WPRINT_APP_INFO(("Playback stats: UNDER=%lu, SILENCE=%lu, OVER=%lu, UNAVAIL=%lu\n",
                         player.underrun_count, player.silence_insertion_count, player.overrun_count, player.buffer_unavail_count));
    } /*end for (;;)*/
}

static void bt_mem(void)
{
    rt_kprintf("decoder_buffer: [%d %d %d], play_buffer: [%d %d %d] [%d, %d, %d free %d (%d)]\n",
               bt_buffer_mem.decoder_buffer, bt_buffer_mem.decoder_buffer_plus, bt_buffer_mem.decoder_buffer_dec,
               bt_buffer_mem.play_buffer, bt_buffer_mem.play_buffer_plus, bt_buffer_mem.play_buffer_dec,
               bt_buffer_mem.decoder_buffer_size, bt_buffer_mem.play_buffer_size,
               bt_buffer_mem.decoder_buffer_size - bt_buffer_mem.play_buffer_size,
               bt_buffer_mem.free_size, HAL_GetTick());
}
MSH_CMD_EXPORT(bt_mem, bt mem);

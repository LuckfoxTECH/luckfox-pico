/*
 * $ Copyright Cypress Semiconductor $
 */

/**  \file    wiced_tone.c
        \brief  Implementation of APIs in #include "wiced_tone.h".
*/

#include "wiced.h"
#include "wiced_audio.h"
#include "platform_audio.h"
#include "wwd_debug.h"

typedef struct
{
    uint8_t *data;
    uint32_t length;
    wiced_audio_config_t audio_info;
    uint8_t repeat;
    uint32_t cur_idx;
} tone_data_t;

#define TONE_SAMPLE_RATE            44100
#define TONE_NUM_CHANNELS           2
#define TONE_SAMPLE_DEPTH_BYTES     2
#define NR_USECONDS_IN_SECONDS      (1000*1000)
#define NR_MSECONDS_IN_SECONDS      (1000)
#define TONE_PLAYER_THREAD_PRI             10
#define TONE_PLAYER_THREAD_NAME            ((const char*) "TONE_THREAD")
#define TONE_PLAYER_THREAD_STACK_SIZE      0x1000
#define AUDIO_PERIOD_SIZE                  (1*1024)
#define AUDIO_BUFFER_SIZE                   (6*AUDIO_PERIOD_SIZE)
#define TX_START_THRESHOLD                 (3*AUDIO_PERIOD_SIZE)
#define GUARD_TIME_MS                      (50)
#define TONE_DATA_LEN                      45
#define DEFAULT_TONE_LENGTH_MS             600 /*Should be > FADE_IN_DURATION_MS+FADE_OUT_DURATION_MS*/
#define DEFAULT_TONE_DATA_LENGTH           (DEFAULT_TONE_LENGTH_MS*TONE_SAMPLE_RATE*TONE_NUM_CHANNELS*TONE_SAMPLE_DEPTH_BYTES/1000)
#define DEFAULT_TONE_VOLUME_LEVEL          ((double)(7.0/15.0))

#define FADE_IN_DURATION_MS               (150)
#define FADE_OUT_DURATION_MS              (100)
#define FADE_IN_DATA_LENGTH               (FADE_IN_DURATION_MS*TONE_SAMPLE_RATE*TONE_NUM_CHANNELS*TONE_SAMPLE_DEPTH_BYTES/1000)
#define FADE_OUT_DATA_LENGTH              (FADE_OUT_DURATION_MS*TONE_SAMPLE_RATE*TONE_NUM_CHANNELS*TONE_SAMPLE_DEPTH_BYTES/1000)

#define TONE_PLAYER_CLEANUP() \
                                do { \
                                    wiced_rtos_lock_mutex(p_session_init_mutex); \
                                    is_session_initialized = 0; \
                                    wiced_audio_stop(session); \
                                    wiced_audio_deinit(session); \
                                    if(args) \
                                        memset(args, 0, sizeof(tone_data_t)); \
                                    wiced_rtos_unlock_mutex(p_session_init_mutex); \
                                    destroy_session_init_mutex(); \
                                }while(0)


static const int16_t            tone_data[TONE_DATA_LEN] =
{
    0xffff, 0xe42, 0x1c38, 0x29a8, 0x3643, 0x41d1, 0x4c1b, 0x54e3,
    0x5c0b, 0x6162, 0x64d9, 0x6655, 0x65d9, 0x635a, 0x5ef2, 0x58af,
    0x50af, 0x4725, 0x3c2e, 0x3015, 0x2304, 0x154c, 0x723, 0xf8dd,
    0xeab5, 0xdcf9, 0xcfef, 0xc3cf, 0xb8dc, 0xaf52, 0xa74e, 0xa112,
    0x9ca2, 0x9a2b, 0x99a7, 0x9b2b, 0x9e99, 0xa3fa, 0xab19, 0xb3e9,
    0xbe2b, 0xc9c0, 0xd656, 0xe3c9, 0xf1be
};

wiced_thread_t                     tone_player_thread_id;
static wiced_audio_session_ref     session;
static uint8_t                     is_session_initialized;
static wiced_mutex_t               s_session_init_mutex;
static wiced_mutex_t              *p_session_init_mutex = &s_session_init_mutex ;
static tone_data_t *args = NULL;
//uint8_t tone_player_thread_stack[TONE_PLAYER_THREAD_STACK_SIZE] __attribute__((section (".ccm")));
extern wiced_bool_t wiced_audio_is_underrun(wiced_audio_session_ref sh);
extern wiced_result_t wiced_audio_underrun_sem_wait(wiced_audio_session_ref sh);

static void wiced_tone_player_task(uint32_t context);

static void destroy_session_init_mutex(void);


wiced_result_t wiced_tone_play(uint8_t *data, uint32_t length, wiced_audio_config_t *audio_info, uint8_t repeat)
{
    wiced_result_t ret;

    if (data && !audio_info)
        return WICED_BADARG;

    if (data && !length)
        return WICED_BADARG;

    if (!args)
        args = (tone_data_t *)rt_malloc(sizeof(tone_data_t));
    if (!args)
        return WICED_OUT_OF_HEAP_SPACE;

    wiced_rtos_delete_thread(&tone_player_thread_id);
    args->data = data;
    args->length = length;
    if (audio_info)
        memcpy(&args->audio_info, audio_info, sizeof(wiced_audio_config_t));
    else
    {
        args->audio_info.bits_per_sample = 16;
        args->audio_info.channels        = 2;
        args->audio_info.sample_rate     = 44100;
        args->audio_info.frame_size      = 4;
    }
    args->repeat = repeat;

    //ret = wiced_rtos_create_thread_with_stack(&tone_player_thread_id, TONE_PLAYER_THREAD_PRI, TONE_PLAYER_THREAD_NAME, wiced_tone_player_task,
    //                                                                tone_player_thread_stack, TONE_PLAYER_THREAD_STACK_SIZE, (void*)args);
    ret = wiced_rtos_create_thread(&tone_player_thread_id, TONE_PLAYER_THREAD_PRI, TONE_PLAYER_THREAD_NAME, wiced_tone_player_task,
                                   TONE_PLAYER_THREAD_STACK_SIZE, (void *)args);
    WPRINT_APP_INFO(("[TONE] wiced_tone_play return = %d\n", ret));
    return ret;

}


wiced_result_t wiced_tone_stop(void)
{
    if (p_session_init_mutex && is_session_initialized)
    {
        wiced_rtos_lock_mutex(p_session_init_mutex);
        wiced_rtos_delete_thread(&tone_player_thread_id);
        wiced_rtos_unlock_mutex(p_session_init_mutex);
        TONE_PLAYER_CLEANUP();
    }

    WPRINT_APP_INFO(("[TONE] wiced_tone_stop return = %d\n", WICED_SUCCESS));
    return WICED_SUCCESS;
}

#if 0
static wiced_result_t initialize_audio_device(const platform_audio_device_id_t dev_id, wiced_audio_config_t *config, wiced_audio_session_ref *shout)
{
    wiced_result_t          result;
    wiced_audio_session_ref sh;

    result = WICED_SUCCESS;

    /* Initialize device. */
    if (result == WICED_SUCCESS)
    {
        result = wiced_audio_init(dev_id, &sh, AUDIO_PERIOD_SIZE);
        wiced_assert("wiced_audio_init", WICED_SUCCESS == result);
    }

    /* Allocate audio buffer. */
    if (result == WICED_SUCCESS)
    {
        result = wiced_audio_create_buffer(sh, AUDIO_BUFFER_SIZE, NULL, NULL);
        wiced_assert("wiced_audio_create_buffer", WICED_SUCCESS == result);
    }

    /* Configure session. */
    if (result == WICED_SUCCESS)
    {
        result = wiced_audio_configure(sh, config);
        wiced_assert("wiced_audio_configure", WICED_SUCCESS == result);
    }

    if (result == WICED_SUCCESS)
    {
        /* Copy-out. */
        *shout = sh;
    }
    else
    {
        /* Error handling. */
        if (wiced_audio_deinit(sh) != WICED_SUCCESS)
        {
            wiced_assert("wiced_audio_deinit", 0);
        }
    }
    return result;
}
#endif
static wiced_result_t create_session_init_mutex(void)
{
    wiced_result_t res = WICED_OUT_OF_HEAP_SPACE;

    res = wiced_rtos_init_mutex(p_session_init_mutex);
    WPRINT_APP_INFO(("[TONE] create_session_init_mutex ret = %d\n", res));
    return res;
}

static void destroy_session_init_mutex(void)
{
    wiced_rtos_deinit_mutex(p_session_init_mutex);
    p_session_init_mutex = NULL;
}


static wiced_result_t copy_data(tone_data_t *tone, uint8_t *buf, uint16_t nrbytes)
{
    int i;
    static unsigned int last_pos = 0;
    uint16_t words_to_copy = nrbytes / 2;
    int16_t *buf16 = (int16_t *)buf;

    double ramp;

    if (tone->cur_idx + nrbytes >= tone->length)
    {
        words_to_copy = (tone->length - (tone->cur_idx)) / 2;
    }

    if (tone->cur_idx == 0)
        last_pos = 0;

    for (i = 0; i < words_to_copy;)
    {
        if (tone->cur_idx <= FADE_IN_DATA_LENGTH)
        {
            ramp = (double) tone->cur_idx / (double)FADE_IN_DATA_LENGTH;
        }
        else if ((tone->length - tone->cur_idx) <= FADE_OUT_DATA_LENGTH)
        {
            ramp = ((double)(tone->length - tone->cur_idx)) / (double)FADE_OUT_DATA_LENGTH;
        }
        else
        {
            ramp = 1;
        }

        buf16[i++] = (int16_t)(((double) tone_data[last_pos]) * ramp);
        buf16[i++] = (int16_t)(((double) tone_data[last_pos]) * ramp);

        if (++last_pos >= sizeof(tone_data) / sizeof(tone_data[0]))
        {
            last_pos = 0;
        }
    }
    if (words_to_copy < nrbytes / 2)
    {
        memset(&buf16[i], 0, (nrbytes - words_to_copy * 2));
        //WPRINT_APP_INFO(("[TONE] cur_idx = %d, memset to 0 = %d bytes\n", tone->cur_idx, (nrbytes-words_to_copy*2)));
    }

    tone->cur_idx += nrbytes;
    return WICED_SUCCESS;
}

static wiced_result_t copy_from_mem(tone_data_t *tone, uint8_t *dst, uint32_t sz)
{
    wiced_result_t result;

    result = WICED_SUCCESS;

    /* Copy predefined data. */
    if (tone->data)
    {
        uint32_t rem_len = tone->length - tone->cur_idx;
        uint32_t cpy_len = MIN(rem_len, sz);

        memcpy(dst, &tone->data[tone->cur_idx], cpy_len);
        tone->cur_idx += cpy_len;

        if (cpy_len < sz)
            memset(&dst[cpy_len], 0, sz - cpy_len);
    }
    else
    {
        result = copy_data(tone, dst, sz);
        wiced_assert("copy_data", result == WICED_SUCCESS);
    }

    return result;
}

uint32_t get_tone_length(tone_data_t *tone)
{
    uint32_t len;

    if (tone->data)
    {
        len = tone->length;
    }
    else
    {
        len = DEFAULT_TONE_DATA_LENGTH;
    }
    return len;
}


static void wiced_tone_player_task(uint32_t context)
{
    uint8_t is_tx_started = 0, silence = 0;
    tone_data_t *args = (tone_data_t *)context;
    wiced_result_t result = WICED_SUCCESS;
    uint32_t weight;
    uint16_t remaining;
    double decibels = 0.0, min_volume_in_db = 0.0, max_volume_in_db = 0.0;

    if (!args)
        return;

    if (WICED_SUCCESS != create_session_init_mutex())
    {
        WPRINT_APP_INFO(("[TONE] mutex creation failed\n"));
        return;
    }

    wiced_rtos_lock_mutex(p_session_init_mutex);
    //result = initialize_audio_device( PLATFORM_DEFAULT_AUDIO_OUTPUT, &args->audio_info, &session);
    is_session_initialized = 1;
    wiced_rtos_unlock_mutex(p_session_init_mutex);

    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("[TONE] init audio failed\n"));
        destroy_session_init_mutex();
        return;
    }

    if (WICED_SUCCESS == wiced_audio_get_volume_range(session, &min_volume_in_db, &max_volume_in_db))
    {
        decibels = (double)((max_volume_in_db - min_volume_in_db) * DEFAULT_TONE_VOLUME_LEVEL) + min_volume_in_db ;
        wiced_audio_set_volume(session, decibels);
        //WPRINT_APP_INFO(( "[TONE] Volume changed[ Min:%.2f Max:%.2f now:%.2f ]\n",min_volume_in_db, max_volume_in_db, decibels ));
    }

    args->length = get_tone_length(args);
    WPRINT_APP_INFO(("[TONE] tone length = %u\n", (unsigned int)args->length));

    do
    {
        args->cur_idx = 0;
        while (args->cur_idx < args->length)
        {
            if (!is_tx_started)
            {
                /* Determine if we should start TX. */
                wiced_audio_get_current_buffer_weight(session, &weight);
                if (weight >= TX_START_THRESHOLD)
                {
                    result = wiced_audio_start(session);
                    if (result == WICED_SUCCESS)
                    {
                        is_tx_started = 1;
                    }
                    if (result != WICED_SUCCESS)
                    {
                        WPRINT_APP_INFO(("wiced_audio_start error %d\n", result));
                    }
                }
            }

            if (result == WICED_SUCCESS)
            {
                /* Wait for slot in transmit buffer. */
                result = wiced_audio_wait_buffer(session, AUDIO_PERIOD_SIZE, 100);
                //wiced_assert(("wait buffer returned error = %d",result), (result == WICED_SUCCESS));
                RT_ASSERT(result == WICED_SUCCESS);
                if (result != WICED_SUCCESS)
                {
                    WPRINT_APP_INFO(("wiced_audio_wait_buffer error %d\n", result));
                    break;
                }
                /* Copy available data to transmit buffer. */
                remaining = AUDIO_PERIOD_SIZE;
                while (0 != remaining && result == WICED_SUCCESS)
                {
                    uint8_t *buf;
                    uint16_t avail = remaining;

                    result = wiced_audio_get_buffer(session, &buf, &avail);
                    if (result != WICED_SUCCESS)
                    {
                        WPRINT_APP_INFO(("wiced_audio_get_buffer error %d\n", result));
                        break;
                    }
                    if (!silence)
                        copy_from_mem(args, buf, avail);
                    else
                    {
                        memset(buf, 0, avail);
                        args->cur_idx += avail;
                    }
                    result = wiced_audio_release_buffer(session, avail);
                    remaining -= avail;
                }
            }

            if (result != WICED_SUCCESS)
            {
                args->repeat = 0;
                break;
            }
        }

        if (args->repeat)
        {
            if (!silence)
            {
                silence = 1;
            }
            else
            {
                args->repeat--;
                silence = 0;
            }
        }
        else
        {
            break;
        }

    }
    while (1);

    if (result == WICED_SUCCESS)
    {
        wiced_audio_get_current_buffer_weight(session, &weight);
        //WPRINT_APP_INFO( ("\n[TONE] weight=%d\n",(int)weight) );
        if (weight > 0)
        {
            uint32_t timeout = (((NR_USECONDS_IN_SECONDS / (args->audio_info.sample_rate * args->audio_info.channels * 2)) * weight) / NR_MSECONDS_IN_SECONDS) + GUARD_TIME_MS;
            wiced_rtos_delay_milliseconds(timeout);
        }
    }

    wiced_audio_get_current_buffer_weight(session, &weight);

    TONE_PLAYER_CLEANUP();
    WPRINT_APP_INFO(("[TONE] exit tone player loop\n"));
}


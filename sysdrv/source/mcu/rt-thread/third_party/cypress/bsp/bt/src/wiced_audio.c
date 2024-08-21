/*
 * $ Copyright Cypress Semiconductor $
 */

/**
 * @file WICED audio framework
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>

#include "wiced_rtos.h"
#include "wiced_audio.h"
#include "wwd_assert.h"
#include "wwd_debug.h"

#define WICED_USE_AUDIO  1
#ifdef WICED_USE_AUDIO

#include "platform_i2s.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define wiced_required( X, LABEL ) \
        do                         \
        {                          \
            if( !( X ) )           \
            {                      \
                goto LABEL;        \
            }                      \
        }   while( 0 )

#define wiced_required_action( X, LABEL, ACTION ) \
        do                                        \
        {                                         \
            if( !( X ) )                          \
            {                                     \
                { ACTION; }                       \
                goto LABEL;                       \
            }                                     \
        }   while( 0 )

/******************************************************
 *                    Constants
 ******************************************************/

#define MAX_AUDIO_SESSIONS               ( 2 )

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

#if defined(WICED_AUDIO_BUFFER_BYTE_ALIGNMENT_SHIFT)

typedef struct
{
    /* Audio ring buffer. */
    wiced_audio_buffer_header_t *buffer;
    /* Start 0-index into the period. */
    uint16_t head;
    /* Bytes pending playback/capture transfer..
     * The inverse represents the bytes available to write (playback)
     * or read (captured).
    */
    uint16_t count;
    /* Total number of audio bytes in the buffer. */
    uint16_t length;
} audio_buffer_t;

#else /* NOT defined(WICED_AUDIO_BUFFER_BYTE_ALIGNMENT_SHIFT) */

typedef struct
{
    uint8_t *buffer;
    uint16_t tail;
    uint16_t head;
    uint16_t count;
    uint16_t length;
} audio_buffer_t;

#endif /* defined(WICED_AUDIO_BUFFER_BYTE_ALIGNMENT_SHIFT) */

/******************************************************
 *                    Structures
 ******************************************************/

struct wiced_audio_session_t
{
    wiced_i2s_t                     i2s_id;
    wiced_audio_device_channel_t    i2s_direction;
    wiced_i2s_spdif_mode_t          i2s_spdif_mode;
    wiced_bool_t                    i2s_running;
    wiced_bool_t                    i2s_disabled;
    wiced_semaphore_t               available_periods;
    uint16_t                        num_periods_requested;
    uint8_t                         frame_size;
    audio_buffer_t                  audio_buffer;
    uint16_t                        period_size;
    wiced_audio_device_interface_t *audio_dev;
    wiced_mutex_t                   session_lock;
    wiced_bool_t                    underrun_occurred;
    wiced_bool_t                    is_initialised;
};

/******************************************************
 *               Variables Definitions
 ******************************************************/
static wiced_audio_device_interface_t platform_wiced_audio_devices[(PLATFORM_AUDIO_NUM_OUTPUTS + PLATFORM_AUDIO_NUM_INPUTS)];
audio_device_class_t     audio_dev_class =
{
    .audio_devices = platform_wiced_audio_devices,
    .device_count  = 0,
};

struct wiced_audio_session_t    sessions[MAX_AUDIO_SESSIONS];

#if 0
#define lock_session(sh)        wiced_rtos_lock_mutex(&(sh)->session_lock)
#define unlock_session(sh)      wiced_rtos_unlock_mutex(&(sh)->session_lock)
#endif

/******************************************************
 *               Function Declarations
 ******************************************************/

static wiced_audio_device_interface_t *search_audio_device(const platform_audio_device_id_t device_id);

/******************************************************
 *               Function Definitions
 ******************************************************/

static wiced_audio_device_interface_t *search_audio_device(const platform_audio_device_id_t device_id)
{
    int i = 0;
    /* Search device in the list of already registered */
    for (i = 0; i < audio_dev_class.device_count; i++)
    {
        if (device_id != audio_dev_class.audio_devices[i].device_id)
        {
            continue;
        }
        else
        {
            return &audio_dev_class.audio_devices[i];
        }
    }
    return NULL;
}


static wiced_audio_session_ref wiced_audio_get_available_session(void)
{
    int i = 0;

    for (i = 0; i < MAX_AUDIO_SESSIONS ; i++)
    {
        if (sessions[i].is_initialised == WICED_FALSE && sessions[i].audio_dev == NULL)
        {
            return &sessions[i];
        }
    }
    return NULL;
}


static wiced_result_t wiced_audio_check_device_is_available_internal(wiced_audio_device_interface_t *audio)
{
    int i = 0;

    /* Search the table of the sessions which are initialised and check which
     * audio device is attached to them. Audio device is considered as available
     * when is not attached to any of the audio sessions which passed through the initialisation
     * stage already */
    for (i = 0; i < MAX_AUDIO_SESSIONS ; i++)
    {
        if (sessions[i].is_initialised == WICED_TRUE)
        {
            if (sessions[i].audio_dev == audio)
            {
                return WICED_FALSE;
            }
        }
    }
    return WICED_TRUE;
}

wiced_result_t wiced_audio_init(const platform_audio_device_id_t device_id, wiced_audio_session_ref *sh, uint16_t period_size)
{
    wiced_audio_device_interface_t *audio     = NULL;
    wiced_bool_t                    available = WICED_FALSE;
    wiced_result_t                  result    = WICED_SUCCESS;
    int                             attempts = 0;
    wiced_audio_session_ref         session  = NULL;
    wiced_audio_data_port_t         data_port;
    wiced_bool_t                    mutex_init = WICED_FALSE;
    wiced_bool_t                    sema_init  = WICED_FALSE;

#ifdef I2S_PERIOD_BYTES_MAX
    /* Period size cannot exceed device limit. */
    wiced_required_action(period_size <= I2S_PERIOD_BYTES_MAX, exit2, result = WICED_BADARG);
#endif

    /* Check whether platform has registered at least one audio device */
    wiced_required_action(audio_dev_class.device_count != 0, exit2, result = WICED_ERROR);

    /* Check whether this audio device is not used by any of the current audio sessions */
    audio = search_audio_device(device_id);
    rt_kprintf("audio: %p \n", audio);
    wiced_required_action(audio != NULL, exit2, result = WICED_ERROR);

    available = wiced_audio_check_device_is_available_internal(audio);
    wiced_required_action(available == WICED_TRUE, exit2, result = WICED_ERROR);

    memset(&data_port, 0, sizeof(data_port));
    do
    {
        result = audio->audio_device_init(audio->audio_device_driver_specific, &data_port);
    }
    while (result != WICED_SUCCESS && ++attempts < 50 && (wiced_rtos_delay_milliseconds(1) == WICED_SUCCESS));

    wiced_required_action(result == WICED_SUCCESS, exit2, result = WICED_ERROR);

    wiced_assert("Can't initialize audio device", result == WICED_SUCCESS);

    /* Get an available session and prepare to use it for the audio device. */
    session = wiced_audio_get_available_session();
    rt_kprintf("session: %p\n", session);
    wiced_required_action(session != NULL, exit, result = WICED_ERROR);
#if 0
    result = wiced_rtos_init_mutex(&session->session_lock);
    wiced_required_action(result == WICED_SUCCESS, exit, result = result);
    mutex_init = WICED_TRUE;
#endif

    *sh = session;
    session->audio_dev = audio;
    if (period_size != 0)
    {
        session->period_size = period_size;
    }
    else
    {
        session->period_size = WICED_AUDIO_DEVICE_PERIOD_SIZE;
    }
    session->underrun_occurred = WICED_FALSE;
    session->is_initialised    = WICED_TRUE;

    session->i2s_disabled = WICED_TRUE;
    session->i2s_running = WICED_FALSE;

    rt_kprintf("session->i2s_running disabled\n");

exit:
    if (result != WICED_SUCCESS)
    {
        if (audio->audio_device_deinit(audio->audio_device_driver_specific) != WICED_SUCCESS)
        {
            WPRINT_LIB_DEBUG(("failed to deinit the audio device\n"));
        }
#if 0
        if (session != NULL)
        {
            if (mutex_init == WICED_TRUE)
            {
                wiced_rtos_deinit_mutex(&session->session_lock);
            }
        }
#endif
    }
exit2:
    return result;
}


wiced_result_t wiced_audio_configure(wiced_audio_session_ref sh, wiced_audio_config_t *config)
{
    wiced_result_t result;
    wiced_i2s_params_t params;
    uint32_t mclk = 0;

    wiced_required_action(sh->audio_dev->audio_device_configure != NULL, exit, result = WICED_ERROR);

    /* Set the audio device to the mode required  */
    params.period_size     = sh->period_size;
    params.sample_rate     = config->sample_rate;
    params.bits_per_sample = config->bits_per_sample;
    params.channels        = config->channels;

    sh->frame_size = config->frame_size;
    wiced_assert("Bits per sample must be multiple of 8", (config->bits_per_sample % 8) == 0);

    /* Configure the audio codec device */
    result = sh->audio_dev->audio_device_configure(sh->audio_dev->audio_device_driver_specific, config, &mclk);
    wiced_assert("Can't initialize audio device\r\n", result == WICED_SUCCESS);

exit:
    return result;
}

wiced_result_t wiced_audio_set_volume(wiced_audio_session_ref sh, double volume_in_db)
{
    wiced_result_t result = WICED_UNSUPPORTED;

    if (sh->audio_dev->audio_device_set_volume != NULL)
    {
        result = sh->audio_dev->audio_device_set_volume(sh->audio_dev->audio_device_driver_specific, volume_in_db);
        if (result != WICED_SUCCESS)
        {
            /* Do some error handling... */
            result = WICED_ERROR;
        }
    }

    return result;
}


wiced_result_t wiced_audio_get_volume_range(wiced_audio_session_ref sh, double *min_volume_in_db, double *max_volume_in_db)
{
    wiced_result_t result = WICED_UNSUPPORTED;

    if (sh->audio_dev->audio_device_get_volume_range != NULL)
    {
        result = sh->audio_dev->audio_device_get_volume_range(sh->audio_dev->audio_device_driver_specific, min_volume_in_db, max_volume_in_db);
        if (result != WICED_SUCCESS)
        {
            /* Do some error handling... */
            result = WICED_ERROR;
        }
    }

    return result;
}


wiced_result_t wiced_audio_deinit(wiced_audio_session_ref sh)
{
    wiced_result_t result = WICED_SUCCESS;

    /* Stop audio device. Initiate power-down sequence on the audio device */
    result = sh->audio_dev->audio_device_deinit(sh->audio_dev->audio_device_driver_specific);

#if 0
    wiced_rtos_deinit_mutex(&sh->session_lock);
#endif

    sh->audio_dev           = NULL;
    sh->is_initialised      = WICED_FALSE;

    if (result != WICED_SUCCESS)
    {
        /* Do some error handling... */
        result = WICED_ERROR;
    }

exit:
    return result;
}

wiced_result_t wiced_audio_clean(void)
{
    rt_kprintf("wiced_audio_clean\n");
    memset(sessions, 0, sizeof(sessions));
    audio_dev_class.device_count = 0;
    return WICED_SUCCESS;
}

wiced_result_t wiced_register_audio_device(const platform_audio_device_id_t device_id, const wiced_audio_device_interface_t *interface)
{
    static wiced_bool_t first_device = WICED_FALSE;

    UNUSED_PARAMETER(device_id);

    /* Add interface to the first empty slot */
    if (first_device == WICED_FALSE)
    {
        int i = 0;

        for (i = 0; i < MAX_NUM_AUDIO_DEVICES; i++)
        {
            memset(&audio_dev_class.audio_devices[i], 0x00, sizeof(wiced_audio_device_interface_t));
        }

        audio_dev_class.device_count = 0;
        first_device = WICED_TRUE;
    }

    memcpy(&audio_dev_class.audio_devices[audio_dev_class.device_count++], interface, sizeof(*interface));
    return WICED_SUCCESS;
}

wiced_result_t wiced_audio_stop(wiced_audio_session_ref sh)
{
    wiced_result_t result = WICED_SUCCESS;

    if (sh->audio_dev->audio_device_stop_streaming != NULL)
    {
        sh->audio_dev->audio_device_stop_streaming(sh->audio_dev->audio_device_driver_specific);
    }

    sh->underrun_occurred = WICED_FALSE;
    sh->num_periods_requested = 0;

exit:
    return result;
}


wiced_result_t wiced_audio_start(wiced_audio_session_ref sh)
{
    wiced_result_t result = WICED_ERROR;

    //启动播放
    if (sh->audio_dev->audio_device_start_streaming != NULL)
    {
        result = sh->audio_dev->audio_device_start_streaming(sh->audio_dev->audio_device_driver_specific);

    }
    return result;
}

wiced_result_t wiced_audio_device_ioctl(wiced_audio_session_ref sh, wiced_audio_device_ioctl_t cmd, wiced_audio_device_ioctl_data_t *cmd_data)
{
    wiced_result_t result = WICED_UNSUPPORTED;

    if (sh->audio_dev->audio_device_ioctl != NULL)
    {
        result = sh->audio_dev->audio_device_ioctl(sh->audio_dev->audio_device_driver_specific, cmd, cmd_data);
    }

    return result;
}

#if 0
wiced_result_t wiced_audio_set_pll_fractional_divider(wiced_audio_session_ref sh, float value)
{
    wiced_result_t result = WICED_UNSUPPORTED;

    if (sh->i2s_disabled == WICED_FALSE)
    {
        result = wiced_i2s_pll_set_fractional_divider(sh->i2s_id, value);
    }

    return result;
}
#endif

uint8_t platform_audio_get_device_count(void)
{
    return ((uint8_t)PLATFORM_AUDIO_NUM_OUTPUTS + (uint8_t)PLATFORM_AUDIO_NUM_INPUTS);
}

#else /* WICED_USE_AUDIO not defined ! */


wiced_result_t wiced_audio_init(const platform_audio_device_id_t device_id, wiced_audio_session_ref *sh, uint16_t period_size)
{
    UNUSED_PARAMETER(device_id);
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(period_size);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_configure(wiced_audio_session_ref sh, wiced_audio_config_t *config)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(config);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_create_buffer(wiced_audio_session_ref sh, uint16_t size, uint8_t *buffer_ptr_aligned, void *(*allocator)(uint16_t size))
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(size);
    UNUSED_PARAMETER(buffer_ptr_aligned);
    UNUSED_PARAMETER(allocator);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_set_volume(wiced_audio_session_ref sh, double volume_in_db)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(volume_in_db);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_get_volume_range(wiced_audio_session_ref sh, double *min_volume_in_db, double *max_volume_in_db)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(min_volume_in_db);
    UNUSED_PARAMETER(max_volume_in_db);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_deinit(wiced_audio_session_ref sh)
{
    UNUSED_PARAMETER(sh);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_register_audio_device(const platform_audio_device_id_t device_id, wiced_audio_device_interface_t *interface)
{
    UNUSED_PARAMETER(device_id);
    UNUSED_PARAMETER(interface);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_get_current_hw_pointer(wiced_audio_session_ref sh, uint32_t *hw_pointer)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(hw_pointer);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_get_buffer(wiced_audio_session_ref sh, uint8_t **ptr, uint16_t *size)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(ptr);
    UNUSED_PARAMETER(size);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_get_latency(wiced_audio_session_ref sh, uint32_t *latency)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(latency);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_stop(wiced_audio_session_ref sh)
{
    UNUSED_PARAMETER(sh);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_start(wiced_audio_session_ref sh)
{
    UNUSED_PARAMETER(sh);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_release_buffer(wiced_audio_session_ref sh, uint16_t size)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(size);
    return WICED_UNSUPPORTED;
}

uint16_t wiced_audio_buffer_platform_get_periods(wiced_audio_session_ref sh)
{
    UNUSED_PARAMETER(sh);
    return 0;
}

wiced_result_t wiced_audio_get_current_buffer_weight(wiced_audio_session_ref sh, uint32_t *weight)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(weight);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_device_ioctl(wiced_audio_session_ref sh, wiced_audio_device_ioctl_t cmd, wiced_audio_device_ioctl_data_t *cmd_data)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(cmd);
    UNUSED_PARAMETER(cmd_data);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_set_pll_fractional_divider(wiced_audio_session_ref sh, float value)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(value);
    return WICED_UNSUPPORTED;
}

wiced_result_t wiced_audio_update_period_size(wiced_audio_session_ref sh, uint16_t period_size)
{
    UNUSED_PARAMETER(sh);
    UNUSED_PARAMETER(period_size);
    return WICED_UNSUPPORTED;
}

#endif /* WICED_USE_AUDIO */

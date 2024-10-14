/*
 * $ Copyright Cypress Semiconductor $
 */
#include "wiced.h"
#include "wiced_rtos.h"
#include "wiced_audio.h"
#include "bluetooth_audio.h"
#include "dual_a2dp_hfp_audio.h"

#ifdef USE_MEM_POOL
#include "mem_pool.h"
#endif

#include "wwd_debug.h"
#ifdef  __ICCARM__
#include "iar_gnudecls.h"
#endif
/*****************************************************************************
**
**  Name:           bt_audio_common.c
**
**  Description:    Interface to the Wiced audio subsystem
**
*****************************************************************************/

/******************************************************
 *                      Macros
 ******************************************************/
#define GET_DEVICE_CONTEXT_PTR(device_type) (( device_type == BT_AUDIO_DEVICE_PLAYER )?&player:&recorder)
#define BT_AUDIO_RECORDER_BUFFER_SIZE               WICED_AUDIO_BUFFER_ARRAY_DIM_SIZEOF(4, BT_AUDIO_DEFAULT_PERIOD_SIZE)

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/

/******************************************************
 *               Variables Definitions
 ******************************************************/
extern bt_audio_context_t player;
extern bt_audio_context_t recorder;

#ifndef USE_WICED_HCI
uint8_t tx_buf[BT_AUDIO_RECORDER_BUFFER_SIZE];
#endif

/******************************************************
 *                   Enumerations
 ******************************************************/

enum dac_output_select
{
    DAC_OUTPUT_SELECT_MUTE    = 0,
    DAC_OUTPUT_SELECT_LCH     = 1,
    DAC_OUTPUT_SELECT_RCH     = 2,
    DAC_OUTPUT_SELECT_LCH_RCH = 3,
};

wiced_result_t bt_audio_configure_device(bt_audio_config_t *p_audio_config, bt_audio_device_type_t device_type, service_type_t service_type);
/******************************************************
 *               Function Definitions
 ******************************************************/
wiced_bool_t is_bt_audio_device_initialized(bt_audio_device_type_t device_type)
{
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type) ;

    return (wiced_bool_t)(device->state > BT_AUDIO_DEVICE_STATE_IDLE);
}

wiced_bool_t is_bt_audio_device_prepared(bt_audio_device_type_t device_type)
{
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type) ;

    return (wiced_bool_t)(device->state == BT_AUDIO_DEVICE_STATE_CONFIGURED ||
                          device->state == BT_AUDIO_DEVICE_STATE_STOPPED);
}

wiced_result_t  bt_audio_init_device(bt_audio_device_type_t device_type)
{
    wiced_result_t result = WICED_SUCCESS;
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type) ;
    char queue_name[20];

    if (device_type == BT_AUDIO_DEVICE_PLAYER)
        strlcpy(queue_name, "PLAYER_Q", sizeof(queue_name));
    else
        strlcpy(queue_name, "RECORDER_Q", sizeof(queue_name));

    if (device->state == BT_AUDIO_DEVICE_STATE_UNINITIALIZED)
    {
        result = wiced_rtos_init_queue(&device->queue, queue_name, sizeof(bt_audio_codec_data_t *), BT_AUDIO_QUEUE_MAX_SIZE);
        result = wiced_rtos_init_event_flags(&device->events);
        result = wiced_rtos_init_semaphore(&device->wait_for_cmd_completion);
        device->state = BT_AUDIO_DEVICE_STATE_IDLE;
    }
    WPRINT_APP_INFO(("%s: result=%d\n", __func__, result));
    return result;
}


wiced_result_t bt_audio_configure_device(bt_audio_config_t *p_audio_config, bt_audio_device_type_t device_type, service_type_t service_type)
{
    wiced_result_t             result;
    wiced_audio_config_t       wiced_audio_conf = {0, };
    bt_audio_context_t        *device           = GET_DEVICE_CONTEXT_PTR(device_type);
    platform_audio_device_id_t player_dev_id    = 0xab00; //PLATFORM_DEFAULT_AUDIO_OUTPUT;
    platform_audio_device_id_t recorder_dev_id  = 0xaaaa; //PLATFORM_DEFAULT_AUDIO_INPUT;

    WPRINT_APP_INFO(("bt_audio_configure_device: device = %d\n", device_type));

    if (p_audio_config == NULL)
        return WICED_BADARG;

    if (device->state != BT_AUDIO_DEVICE_STATE_IDLE)
        return WICED_ERROR;

    /* Initialize and configure audio framework for needed audio format */
    if (device_type == BT_AUDIO_DEVICE_PLAYER)
        result = wiced_audio_init(player_dev_id, &device->bluetooth_audio_session_handle, BT_AUDIO_DEFAULT_PERIOD_SIZE);
    else //recorder
        result = wiced_audio_init(recorder_dev_id, &device->bluetooth_audio_session_handle, BT_AUDIO_DEFAULT_PERIOD_SIZE);

    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_configure_device: device %d,  Error Initializing Wiced Audio Framework[err: %d]\n", (unsigned int) device_type, result));
        return result;
    }

    memcpy(&device->bluetooth_audio_config, p_audio_config, sizeof(bt_audio_config_t));
    WPRINT_APP_INFO(("bt_audio_configure_device: device_type %d, config sample_rate:%u channels:%d bps:%d\n",
                     (unsigned int) device_type, (unsigned int) device->bluetooth_audio_config.sample_rate,
                     (int)device->bluetooth_audio_config.channels, (int)device->bluetooth_audio_config.bits_per_sample));

    wiced_audio_conf.sample_rate     = device->bluetooth_audio_config.sample_rate;
    wiced_audio_conf.channels        = device->bluetooth_audio_config.channels;
    wiced_audio_conf.bits_per_sample = device->bluetooth_audio_config.bits_per_sample;
    wiced_audio_conf.frame_size      = (wiced_audio_conf.channels * wiced_audio_conf.bits_per_sample) / 8;

    result = wiced_audio_configure(device->bluetooth_audio_session_handle, &wiced_audio_conf);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_configure_device: device %d Error configuring Wiced Audio framework [err: %d]\n", (unsigned int) device_type, result));
        return result;
    }
    device->state = BT_AUDIO_DEVICE_STATE_CONFIGURED;

    //Set the default volume if player
    if (device_type == BT_AUDIO_DEVICE_PLAYER)
        result = bt_audio_update_player_volume(device->bluetooth_audio_config.volume);


    UNUSED_PARAMETER(service_type);

    return result;
}

void bt_audio_start_loop(bt_audio_device_type_t device_type)
{
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type);

    wiced_rtos_set_event_flags(&device->events, BT_AUDIO_EVENT_START_LOOP);
    //WPRINT_APP_INFO(("%s: Send [%d] loop start event [result: %d]\n",__func__, device_type, result));
}

wiced_result_t bt_audio_end_loop(uint32_t device_type)
{
    bt_audio_context_t *p_player = NULL;
    bt_audio_context_t *p_recorder = NULL;

    if (device_type & BT_AUDIO_DEVICE_PLAYER)
    {
        if (player.state > BT_AUDIO_DEVICE_STATE_IDLE)
        {
            p_player = GET_DEVICE_CONTEXT_PTR((device_type & BT_AUDIO_DEVICE_PLAYER));
            wiced_rtos_set_event_flags(&p_player->events, BT_AUDIO_EVENT_STOP_LOOP);
        }
    }
    if (device_type & BT_AUDIO_DEVICE_RECORDER)
    {
        if (recorder.state > BT_AUDIO_DEVICE_STATE_IDLE)
        {
            p_recorder = GET_DEVICE_CONTEXT_PTR((device_type & BT_AUDIO_DEVICE_RECORDER));
            wiced_rtos_set_event_flags(&p_recorder->events, BT_AUDIO_EVENT_STOP_LOOP);
        }
    }

    WPRINT_APP_INFO(("%s device type 0x%x\n", __func__, (unsigned int) device_type));
    //wait for the device to go to IDLE
    if (p_player)
        wiced_rtos_get_semaphore(&p_player->wait_for_cmd_completion, 34); //max time player can wait for buffer @8K
    if (p_recorder)
        wiced_rtos_get_semaphore(&p_recorder->wait_for_cmd_completion, 34); //max time player can wait for buffer @8K
    WPRINT_APP_INFO(("%s device to go to IDLE\n", __func__));

    return WICED_SUCCESS;
}

#if 0
wiced_result_t bt_audio_start(bt_audio_device_type_t device_type)
{
    wiced_result_t    result = WICED_ERROR;
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type) ;
    printf("==%s device_type: %d==\n", __func__, device_type);

    //if (device_type == BT_AUDIO_DEVICE_RECORDER)
    //  return;

    if (is_bt_audio_device_prepared(device_type))
        result = wiced_audio_start(device->bluetooth_audio_session_handle);

    if (result == WICED_SUCCESS)
        device->state = BT_AUDIO_DEVICE_STATE_STARTED;

    WPRINT_APP_INFO(("bt_audio_start: start state [device %d] [result: %d]\n", device_type, result));
    return result;
}
#endif

void bt_audio_stop(bt_audio_device_type_t device_type)
{
    uint8_t stop_audio = 0;
    bt_audio_codec_data_t *pcm = NULL;
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type) ;
    printf("==%s device_type: %d==\n", __func__, device_type);

    //if (device_type == BT_AUDIO_DEVICE_RECORDER)
    //  return;

    if ((device->state > BT_AUDIO_DEVICE_STATE_IDLE))
    {
        if (device->state == BT_AUDIO_DEVICE_STATE_STARTED)
            stop_audio = 1;

        if (stop_audio)
        {
            wiced_audio_stop(device->bluetooth_audio_session_handle);
            if (WICED_SUCCESS != wiced_rtos_is_queue_empty(&device->queue))
            {
                while (WICED_SUCCESS == wiced_rtos_pop_from_queue(&device->queue, &pcm, WICED_NO_WAIT))
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
        wiced_audio_deinit(device->bluetooth_audio_session_handle);
        device->state = BT_AUDIO_DEVICE_STATE_IDLE;
    }
    wiced_rtos_set_semaphore(&device->wait_for_cmd_completion);
}


wiced_result_t  bt_audio_deinit_device(bt_audio_device_type_t device_type)
{
    wiced_result_t ret = WICED_SUCCESS;
    bt_audio_context_t *device = GET_DEVICE_CONTEXT_PTR(device_type) ;
    bt_audio_codec_data_t *pcm = NULL;

    ret =  wiced_rtos_set_event_flags(&device->events, BT_AUDIO_EVENT_DEINIT);
    if (ret != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_set_event_flags: result=%d\n", ret));
        return ret;
    }

    rt_thread_mdelay(1000);

    do
    {
        WPRINT_APP_INFO(("wiced_rtos_pop_from_queue pcm\n"));
        ret = wiced_rtos_pop_from_queue(&device->queue, &pcm, 1);
        if (ret == WICED_SUCCESS)
        {
            rt_free(pcm);
            pcm = NULL;
        }
        else
        {
            WPRINT_APP_INFO(("wiced_rtos_pop_from_queue break\n"));
        }
    }
    while (ret == WICED_SUCCESS);

    ret = wiced_rtos_deinit_queue(&device->queue);
    if (ret != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_deinit_queue: result=%d\n", ret));
        return ret;
    }

    ret = wiced_rtos_deinit_event_flags(&device->events);
    if (ret != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_deinit_event_flags: result=%d\n", ret));
        return ret;
    }

    ret = wiced_rtos_deinit_semaphore(&device->wait_for_cmd_completion);
    if (ret != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_deinit_semaphore: result=%d\n", ret));
        return ret;
    }

    /* reset player control block */
    memset(device, 0, sizeof(bt_audio_context_t));
    device->state = BT_AUDIO_DEVICE_STATE_UNINITIALIZED;
    return ret;
}


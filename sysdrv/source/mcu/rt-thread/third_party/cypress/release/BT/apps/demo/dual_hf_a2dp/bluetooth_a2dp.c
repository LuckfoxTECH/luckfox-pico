/*
 * $ Copyright Cypress Semiconductor $
 */

/* @bluetooth_a2dp.c
 *
 * This contains the relevant application code for the Bluetooth A2DP profile.
 *
 */

#include "wiced.h"
#include "dual_a2dp_hfp_audio.h"
#include "bluetooth_a2dp.h"
#include "hashtable.h"
#include "rtos.h"
#include "wiced_bt_types.h"
#include "bluetooth_audio.h"
#include "wiced_bt_remote_control.h"
#include "wiced_bt_a2dp_sink.h"
#include "bluetooth_nv.h"
#ifdef USE_MEM_POOL
#include "mem_pool.h"
#endif

#ifdef USE_AUDIO_DISPLAY
#include "audio_display.h"
#endif

#include "wwd_debug.h"
#include "wiced_rtos.h"
#include "rtthread.h"

/******************************************************
 *                      Macros
 ******************************************************/
//#define DUAL_A2DP_LOG_ENABLE

/******************************************************
 *                    Constants
 ******************************************************/
/*  Recommended max_bitpool for high quality audio */
#define BT_AUDIO_A2DP_SBC_MAX_BITPOOL 53
#define BT_A2DP_CONTEXT_MAX_LIMIT     2

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
 *               Static Function Declarations
 ******************************************************/
static void bt_audio_a2dp_init_service(void);
void bt_audio_a2dp_connect_service(void);
void bt_audio_a2dp_disconnect_service(void);
static void bt_audio_app_sink_avrc_connect(void);
/* Keypad action functions */
static wiced_result_t bt_audio_a2dp_volume_up(void);
static wiced_result_t bt_audio_a2dp_volume_down(void);
static wiced_result_t bt_audio_a2dp_stop(void);
static wiced_result_t bt_audio_a2dp_skip_forward(void);
static wiced_result_t bt_audio_a2dp_skip_backward(void);
static wiced_result_t bt_audio_a2dp_pause_play(void);
/* service-specific routines */
static void bt_audio_a2dp_prevent_service(wiced_app_service_t *service);
static void bt_audio_a2dp_allow_service(wiced_app_service_t *service);
static void bt_audio_remote_control_connection_state_cback(wiced_bt_device_address_t remote_addr, wiced_result_t status,
        wiced_bt_remote_control_connection_state_t connection_state, uint32_t peer_features);
static void bt_audio_remote_control_cmd_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_command_t *avrc_cmd);
static void bt_audio_remote_control_rsp_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_response_t *avrc_rsp);
static wiced_result_t bt_audio_get_config_from_cie(wiced_bt_a2dp_codec_info_t *p_codec_config, bt_audio_config_t *p_audio_config);
static void bt_audio_sink_control_cb(wiced_bt_a2dp_sink_event_t event, wiced_bt_a2dp_sink_event_data_t *p_data);
static void bt_audio_sink_data_cb(wiced_bt_a2dp_sink_codec_t codec_type, wiced_bt_a2dp_sink_audio_data_t *p_audio_data);
static wiced_result_t bt_audio_a2dp_button_event_handler(app_service_action_t action);
static int get_a2dp_context(wiced_bt_device_address_t remote_addr);
static int set_a2dp_context(wiced_bt_device_address_t remote_addr);

wiced_result_t bt_audio_a2dp_send_passthru_command(uint8_t cmd, wiced_bt_device_address_t remote_addr);

/******************************************************
 *               External Function Declarations
 ******************************************************/
extern wiced_result_t bt_audio_configure_device(bt_audio_config_t *p_audio_config, bt_audio_device_type_t device_type, service_type_t service_type);

/******************************************************
 *               Variable Definitions
 ******************************************************/
wiced_bt_a2dp_codec_info_t bt_audio_codec_capabilities[] =
{
    {
        .codec_id = WICED_BT_A2DP_SINK_CODEC_SBC,
        .cie =
        {
            .sbc =
            {
                (A2D_SBC_IE_SAMP_FREQ_44 | A2D_SBC_IE_SAMP_FREQ_48),    /* samp_freq */
                (A2D_SBC_IE_CH_MD_MONO | A2D_SBC_IE_CH_MD_STEREO |
                 A2D_SBC_IE_CH_MD_JOINT | A2D_SBC_IE_CH_MD_DUAL),       /* ch_mode */
                (A2D_SBC_IE_BLOCKS_16 | A2D_SBC_IE_BLOCKS_12 |
                 A2D_SBC_IE_BLOCKS_8 | A2D_SBC_IE_BLOCKS_4),            /* block_len */
                (A2D_SBC_IE_SUBBAND_4 | A2D_SBC_IE_SUBBAND_8),          /* num_subbands */
                (A2D_SBC_IE_ALLOC_MD_L | A2D_SBC_IE_ALLOC_MD_S),        /* alloc_mthd */
                BT_AUDIO_A2DP_SBC_MAX_BITPOOL,          /* max_bitpool for high quality audio */
                A2D_SBC_IE_MIN_BITPOOL                                  /* min_bitpool */
            }
        }
    },
    {
        .codec_id = WICED_BT_A2DP_SINK_CODEC_SBC,
        .cie =
        {
            .sbc =
            {
                (A2D_SBC_IE_SAMP_FREQ_44 | A2D_SBC_IE_SAMP_FREQ_48),    /* samp_freq */
                (A2D_SBC_IE_CH_MD_MONO | A2D_SBC_IE_CH_MD_STEREO |
                 A2D_SBC_IE_CH_MD_JOINT | A2D_SBC_IE_CH_MD_DUAL),       /* ch_mode */
                (A2D_SBC_IE_BLOCKS_16 | A2D_SBC_IE_BLOCKS_12 |
                 A2D_SBC_IE_BLOCKS_8 | A2D_SBC_IE_BLOCKS_4),            /* block_len */
                (A2D_SBC_IE_SUBBAND_4 | A2D_SBC_IE_SUBBAND_8),          /* num_subbands */
                (A2D_SBC_IE_ALLOC_MD_L | A2D_SBC_IE_ALLOC_MD_S),        /* alloc_mthd */
                BT_AUDIO_A2DP_SBC_MAX_BITPOOL,          /* max_bitpool for high quality audio */
                A2D_SBC_IE_MIN_BITPOOL                                  /* min_bitpool */
            }
        }
    }
};

wiced_bt_a2dp_config_data_t bt_audio_codec_config =
{
    .feature_mask = WICED_BT_A2DP_SINK_FEAT_INVALID,
    .codec_capabilities =
    {
        .count = 2,
        .info = &bt_audio_codec_capabilities[0],
    },
};

/*A2DP/AVRCP & audio configuration details of the 2 peer A2DP/AVRCP devices*/
a2dp_context_data_t a2dp_context[BT_A2DP_CONTEXT_MAX_LIMIT] =
{
    {
        .sink_connected = WICED_FALSE,
        .rc_connected = WICED_FALSE,
        .avrc_peer_address = {0,},
        .a2dp_peer_address = {0,},
        .peer_features = 0,
        .audio_config = { .volume = BT_AUDIO_DEFAULT_VOLUME, },
        .decoder_config = {WICED_BT_A2DP_SINK_CODEC_SBC,},
        .in_use_context = WICED_FALSE,
        .stream_status = STREAM_IDLE
    },
    {
        .sink_connected = WICED_FALSE,
        .rc_connected = WICED_FALSE,
        .avrc_peer_address = {0,},
        .a2dp_peer_address = {0,},
        .peer_features = 0,
        .audio_config = { .volume = BT_AUDIO_DEFAULT_VOLUME, },
        .decoder_config = {WICED_BT_A2DP_SINK_CODEC_SBC,},
        .in_use_context = WICED_FALSE,
        .stream_status = STREAM_IDLE
    }
};

#ifdef USE_MEM_POOL
extern bt_buffer_pool_handle_t  mem_pool;
#endif

wiced_bt_device_address_t null_addr = {0,};
wiced_bt_a2dp_codec_info_t  codec_config;
#if 0
wiced_timer_t bt_audio_avrcp_timer;
wiced_timer_t bt_audio_avdt_suspend_timer;
#endif

/******************************************************
 *               Function Definitions
 ******************************************************/

/**
 * void wiced_add_bluetooth_audio_service(void)
 *
 * Use this function to add the bluetooth service to the service table.
 */

void bt_audio_add_a2dp_service(void)
{
    wiced_app_service_t cell;
    wiced_result_t result = WICED_ERROR;

    cell.priority                   = SERVICE_BT_A2DP_PRIORITY;
    cell.type                       = SERVICE_BT_A2DP;
    cell.state                      = SERVICE_DISABLED;


    cell.init_service               = bt_audio_a2dp_init_service;
    cell.connect_service            = bt_audio_a2dp_connect_service;
    cell.disconnect_service         = bt_audio_a2dp_disconnect_service;
    cell.deinit_service             = NULL;
    cell.prevent_service            = bt_audio_a2dp_prevent_service;
    cell.allow_service              = bt_audio_a2dp_allow_service;
    cell.button_handler             = bt_audio_a2dp_button_event_handler;

    result = wiced_add_entry(&cell);

    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("[BT-A2DP] Failed to add Service entry[Service:%d Error:%d]\n", cell.type, result));
        /* TODO. May be we wish to send the result to the app_worker_thread as well */
        return;
    }

    WPRINT_APP_INFO(("[BT-A2DP] Service Added\n"));
}

static void bt_audio_a2dp_init_service(void)
{
    wiced_result_t result;
    uint32_t local_features = REMOTE_CONTROL_FEATURE_CONTROLLER | REMOTE_CONTROL_FEATURE_TARGET;

    result = wiced_bt_a2dp_sink_init(&bt_audio_codec_config, bt_audio_sink_control_cb, bt_audio_sink_data_cb);
    WPRINT_APP_INFO(("[BT-A2DP]: a2dp sink init result = %d\n", result));

    if (result == WICED_SUCCESS)
        result = wiced_bt_remote_control_init(local_features, bt_audio_remote_control_connection_state_cback,
                                              bt_audio_remote_control_cmd_cback, bt_audio_remote_control_rsp_cback);
    WPRINT_APP_INFO(("[BT-A2DP]: Service started\n"));
}

void bt_audio_a2dp_connect_service(void)
{
    wiced_result_t result;
    bt_audio_paired_device_info_t out_device;

    if (app_data.a2dp_last_stream)
    {
        result = wiced_bt_a2dp_sink_connect(app_data.a2dp_last_stream->a2dp_peer_address);
    }
    else
    {
        if (bt_audio_nv_get_device_info_by_index(0, &out_device) != WICED_SUCCESS)
        {
            WPRINT_APP_INFO(("[BT-A2DP] Can't connect A2DP profile- no previously connected device exist\n"));
            goto signal_and_exit;
        }
        result = wiced_bt_a2dp_sink_connect(out_device.device_link.bd_addr);
    }

    if (WICED_SUCCESS != result)
    {
        WPRINT_APP_INFO(("[BT-A2DP] A2DP trying to connect - failed !\n"));
        goto signal_and_exit;
    }

    return;

signal_and_exit:
#if 0
    signal_for_event_semaphore(WICED_BT_CONNECT_A2DP, WICED_FALSE);
#endif
    return;
}

void bt_audio_avrcp_connect_service(void)
{
    wiced_result_t result;
    bt_audio_paired_device_info_t out_device;

    if (app_data.a2dp_last_stream)
    {
        result = wiced_bt_remote_control_connect(app_data.a2dp_last_stream->a2dp_peer_address);
    }
    else
    {
        if (bt_audio_nv_get_device_info_by_index(0, &out_device) != WICED_SUCCESS)
        {
            WPRINT_APP_INFO(("[BT-A2DP] Can't connect AVRCP profile- no previously connected device exist\n"));
            goto signal_and_exit;
        }
        result = wiced_bt_remote_control_connect(out_device.device_link.bd_addr);
    }
    if (WICED_SUCCESS != result)
    {
        WPRINT_APP_INFO(("[BT-A2DP] AVRCP trying to connect - failed !\n"));
        goto signal_and_exit;
    }

    return;

signal_and_exit:
#if 0
    signal_for_event_semaphore(WICED_BT_CONNECT_AVRCP, WICED_FALSE);
#endif
    return;
}

void bt_audio_a2dp_disconnect_service(void)
{
    wiced_result_t result;
    int context_index;

    for (context_index = 0; context_index < BT_A2DP_CONTEXT_MAX_LIMIT; context_index ++)
    {
        if (a2dp_context[context_index].rc_connected == WICED_FALSE && a2dp_context[context_index].sink_connected == WICED_TRUE)
            break;
    }

    if (context_index < BT_A2DP_CONTEXT_MAX_LIMIT)
    {
        result = wiced_bt_a2dp_sink_disconnect(a2dp_context[context_index].a2dp_peer_address);
        if (WICED_SUCCESS != result)
        {
            WPRINT_APP_INFO(("[BT-A2DP] A2DP trying to disconnect - failed !\n"));
#if 0
            signal_for_event_semaphore(WICED_BT_DISCONNECT_A2DP, WICED_FALSE);
#endif
        }
    }
    return;
}

void bt_audio_avrcp_disconnect_service(void)
{
    wiced_result_t result;
    int context_index;

    for (context_index = 0; context_index < BT_A2DP_CONTEXT_MAX_LIMIT; context_index ++)
    {
        if (a2dp_context[context_index].rc_connected == WICED_TRUE && a2dp_context[context_index].sink_connected == WICED_TRUE)
            break;
    }

    if (context_index < BT_A2DP_CONTEXT_MAX_LIMIT)
    {
        result = wiced_bt_remote_control_disconnect(a2dp_context[context_index].avrc_peer_address);
        if (WICED_SUCCESS != result)
        {
            WPRINT_APP_INFO(("[BT-A2DP] AVRCP trying to disconnect - failed !\n"));
#if 0
            signal_for_event_semaphore(WICED_BT_DISCONNECT_AVRCP, WICED_FALSE);
#endif
        }
    }
    return;

}

#if 0
void bt_audio_timer_expiry_handler(void *arg)
{
    WPRINT_APP_INFO(("bt_audio_timer_expiry_handler\n"));

    app_queue_element_t message;
    wiced_rtos_stop_timer(&bt_audio_avrcp_timer);
    message.event = WICED_BT_CONNECT_AVRCP;
    message.function = bt_audio_app_sink_avrc_connect;
    message.wait_for_event_complete = WICED_FALSE;
    wiced_rtos_push_to_queue(&app_data.queue, &message, WICED_NO_WAIT);
}

void bt_audio_suspend_req_timer_expiry_handler(void *arg)
{
    WPRINT_APP_INFO(("bt_audio_suspend_req_timer_expiry_handler\n"));
    wiced_rtos_stop_timer(&bt_audio_avdt_suspend_timer);
    wiced_bt_a2dp_sink_suspend(app_data.a2dp_last_stream->a2dp_peer_address);
}
#endif

static void bt_audio_app_sink_avrc_connect(void)
{
    int context_index;

    for (context_index = 0; context_index < BT_A2DP_CONTEXT_MAX_LIMIT; context_index ++)
    {
        if (a2dp_context[context_index].rc_connected == WICED_FALSE && a2dp_context[context_index].sink_connected == WICED_TRUE)
            break;
    }

    if (context_index < BT_A2DP_CONTEXT_MAX_LIMIT)
    {
        wiced_bt_remote_control_connect(a2dp_context[context_index].a2dp_peer_address);
    }
    return;
}

static void bt_audio_suspend_a2dp_stream(a2dp_context_data_t *current_stream)
{
    bt_audio_a2dp_send_passthru_command(AVRC_ID_PAUSE, current_stream->a2dp_peer_address);
#ifdef USE_WICED_HCI
    wiced_bt_a2dp_sink_suspend(current_stream->a2dp_peer_address);
#endif
    current_stream->stream_status = STREAM_SUSPEND_PENDING;
    bt_audio_end_loop(BT_AUDIO_DEVICE_PLAYER);
    bt_audio_reset_decoder_config();
    app_reset_current_service(SERVICE_BT_A2DP);
    app_set_service_state(SERVICE_BT_A2DP, SERVICE_PAUSED_AUDIO);
    app_data.a2dp_media_state = WICED_FALSE;
    app_data.play_button_state = WICED_FALSE;
}

static void bt_audio_suspend_cfm_a2dp_stream(a2dp_context_data_t *current_stream)
{
    current_stream->stream_status = STREAM_IDLE;
    bt_audio_end_loop(BT_AUDIO_DEVICE_PLAYER);
    bt_audio_reset_decoder_config();
    app_reset_current_service(SERVICE_BT_A2DP);
    app_set_service_state(SERVICE_BT_A2DP, SERVICE_PAUSED_AUDIO);
    app_data.a2dp_media_state    = WICED_FALSE;
    app_data.play_button_state   = WICED_FALSE;
    app_data.a2dp_last_stream    = app_data.a2dp_current_stream;
    app_data.a2dp_current_stream = NULL;
}

static void bt_audio_start_a2dp_stream(a2dp_context_data_t *current_stream)
{
    bt_audio_configure_decoder(&current_stream->decoder_config);
    bt_audio_configure_device(&current_stream->audio_config, BT_AUDIO_DEVICE_PLAYER, SERVICE_BT_A2DP);
    app_set_current_service(SERVICE_BT_A2DP);
    app_set_service_state(SERVICE_BT_A2DP, SERVICE_PLAYING_AUDIO);
    current_stream->stream_status = STREAM_PLAYING;
#ifdef USE_WICED_HCI
    wiced_bt_a2dp_sink_start_rsp(current_stream ->a2dp_peer_address, A2D_SUCCESS);
#endif
    app_data.a2dp_media_state = WICED_TRUE;
    app_data.play_button_state = WICED_TRUE;
    app_data.a2dp_current_stream = current_stream;
}

static void bt_audio_setup_new_a2dp_stream(a2dp_context_data_t *new_stream, stream_state_t state)
{
    app_data.a2dp_current_stream = new_stream;
    app_data.a2dp_current_stream->stream_status = state;
}

static void bt_audio_sink_control_cb(wiced_bt_a2dp_sink_event_t event, wiced_bt_a2dp_sink_event_data_t *p_data)
{
    wiced_app_service_t        *service = NULL;
    wiced_bt_device_address_t   bda;
    int                         context_index;

    switch (event)
    {
    case WICED_BT_A2DP_SINK_CONNECT_EVT:
        WPRINT_APP_INFO(("%s:CONNECT EVENT status = %d\n", __func__, p_data->connect.result));
        memset(bda, 0, sizeof(wiced_bt_device_address_t));
        memcpy(bda, p_data->connect.bd_addr, sizeof(wiced_bt_device_address_t));
        WPRINT_APP_INFO(("bt_audio_sink_control_cb: Remote Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                         bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));

        context_index = set_a2dp_context(bda);
        if (context_index >= BT_A2DP_CONTEXT_MAX_LIMIT)
        {
            WPRINT_APP_INFO(("%s:Invalid Index \n", __func__));
            break;
        }
        if (p_data->connect.result == WICED_SUCCESS)
        {
            a2dp_context[context_index].sink_connected = WICED_TRUE;
            memcpy(a2dp_context[context_index].a2dp_peer_address, p_data->connect.bd_addr, sizeof(wiced_bt_device_address_t));
            memcpy(&a2dp_context[context_index].decoder_config, &codec_config, sizeof(wiced_bt_a2dp_codec_info_t));
            bt_audio_get_config_from_cie(&a2dp_context[context_index].decoder_config, &a2dp_context[context_index].audio_config);
            a2dp_context[context_index].stream_status = STREAM_IDLE;

            service = wiced_get_entry(SERVICE_BT_A2DP);
            if (service->state == SERVICE_DISABLED)
            {
                service->state = SERVICE_ENABLED;
            }
#if 0
            if (a2dp_context[context_index].rc_connected == WICED_FALSE)
            {
                wiced_rtos_start_timer(&bt_audio_avrcp_timer);
            }
            signal_for_event_semaphore(WICED_BT_CONNECT_A2DP, WICED_TRUE);
#endif
#ifdef USE_AUDIO_DISPLAY
            audio_display_header_update_options(BATTERY_ICON_IS_VISIBLE | BATTERY_ICON_SHOW_PERCENT
                                                | BLUETOOTH_IS_CONNECTED);
            audio_display_footer_update_options(FOOTER_IS_VISIBLE | FOOTER_CENTER_ALIGN);
#endif /* USE_AUDIO_DISPLAY */
        }
        else
        {
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("%s:CONNECT EVENT FAIL CASE FOR device= %d\n", __func__, context_index));
#endif
            a2dp_context[context_index].sink_connected = WICED_FALSE;
#if 0
            signal_for_event_semaphore(WICED_BT_CONNECT_A2DP, WICED_FALSE);
#endif
        }
        break;

    case WICED_BT_A2DP_SINK_DISCONNECT_EVT:
    {
        wiced_bool_t disable_a2dp_service = WICED_TRUE;
        memset(bda, 0, sizeof(wiced_bt_device_address_t));
        memcpy(bda, p_data->disconnect.bd_addr, sizeof(wiced_bt_device_address_t));
        WPRINT_APP_INFO(("%s:DISCONNECTED EVENT reason = %d \nRemote Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                         __func__, p_data->disconnect.result, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));
        context_index = get_a2dp_context(bda);
        if (context_index >= BT_A2DP_CONTEXT_MAX_LIMIT)
        {
            WPRINT_APP_INFO(("%s:Invalid Index \n", __func__));
            break;
        }

        a2dp_context[context_index].sink_connected = WICED_FALSE;
        memset(a2dp_context[context_index].a2dp_peer_address, 0, sizeof(wiced_bt_device_address_t));
        memset(&a2dp_context[context_index].decoder_config, 0, sizeof(wiced_bt_a2dp_codec_info_t));
        if (!memcmp(a2dp_context[context_index].avrc_peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
        {
            a2dp_context[context_index].rc_connected = WICED_FALSE;
            a2dp_context[context_index].in_use_context = WICED_FALSE;
        }

        /*if the src BT is turned off, we may not get suspend, if playback is on, stop it here*/
        if (a2dp_context[context_index].stream_status == STREAM_PLAYING)
        {
            bt_audio_end_loop(BT_AUDIO_DEVICE_PLAYER);
            app_data.play_button_state   = WICED_FALSE;
            app_data.a2dp_last_stream    = app_data.a2dp_current_stream;
            app_data.a2dp_current_stream = NULL;
        }
        a2dp_context[context_index].stream_status = STREAM_IDLE;

        /* Disable A2DP service when there is no peer A2DP device active*/
        for (int index = 0; index < BT_A2DP_CONTEXT_MAX_LIMIT ; index++)
        {
            if (a2dp_context[index].in_use_context == WICED_TRUE)
            {
                disable_a2dp_service = WICED_FALSE;
                break;
            }
        }
        if (disable_a2dp_service == WICED_TRUE)
        {
            app_disable_service(SERVICE_BT_A2DP);
#ifdef USE_AUDIO_DISPLAY
            audio_display_header_update_options(BATTERY_ICON_IS_VISIBLE | BATTERY_ICON_SHOW_PERCENT
                                                | INVERT_BLUETOOTH_ICON_COLORS);
            audio_display_footer_update_options(FOOTER_HIDE_SONG_DURATION | FOOTER_CENTER_ALIGN);
#endif /* USE_AUDIO_DISPLAY */
        }
#if 0
        signal_for_event_semaphore(WICED_BT_DISCONNECT_A2DP, WICED_FALSE);
#endif
    }
    break;

    case WICED_BT_A2DP_SINK_START_EVT:
    {
        a2dp_context_data_t *new_stream;
        memset(bda, 0, sizeof(wiced_bt_device_address_t));
        memcpy(bda, p_data->start.bd_addr, sizeof(wiced_bt_device_address_t));
        WPRINT_APP_INFO(("bt_audio_sink_control_cb:STREAM START EVENT status = %d Remote Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                         p_data->start.result, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));
        context_index = get_a2dp_context(bda);
        if (context_index >= BT_A2DP_CONTEXT_MAX_LIMIT)
        {
            WPRINT_APP_INFO(("%s:Invalid Index \n", __func__));
            break;
        }

        service = wiced_get_entry(SERVICE_BT_A2DP);
        if (service == NULL)
        {
            WPRINT_APP_ERROR(("[BT-A2DP] No A2DP service entry\n"));
            return;
        }
        while ((service->state == SERVICE_PREEMPTED) || (service->state == SERVICE_PREVENTED))
        {
            app_set_service_state(SERVICE_BT_A2DP, SERVICE_PENDING);
            return;
        }

        new_stream = &a2dp_context[context_index];

        if (app_data.a2dp_current_stream && app_data.a2dp_current_stream->stream_status == STREAM_PLAYING)
        {
            bt_audio_suspend_a2dp_stream(app_data.a2dp_current_stream);
            /*Android Phones initiates AVDT suspend on AVRC_ID_PAUSE passthru_command from DUT where as Iphone doesn't initiates AVDT Suspend.
            If wiced_bt_a2dp_sink_suspend() is called along with AVRC_ID_PAUSE command Android phones[Eg: Nexus 5 Android 5.0 version] fails to respond for AVDT suspend
            causing AVRC disconnect whereas it works fine in case of Iphone. So to handle this case we use timer,if AV source doesn't initiate suspend on AVRC Pause, DUT initiates. */
            app_data.a2dp_last_stream = app_data.a2dp_current_stream;
#ifndef USE_WICED_HCI
#if 0
            wiced_rtos_start_timer(&bt_audio_avdt_suspend_timer);
#endif
#endif
            bt_audio_setup_new_a2dp_stream(new_stream, STREAM_SUSPEND_PENDING);
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("sending PAUSE to current streaming peer device \n"));
            WPRINT_APP_INFO(("WICED_BT_A2DP_SINK_START_EVT app_data.a2dp_current_stream:%p\n", app_data.a2dp_current_stream));
#endif
        }
        else
        {
            bt_audio_start_a2dp_stream(new_stream);
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("WICED_BT_A2DP_SINK_START_EVT app_data.a2dp_current_stream:%p\n", app_data.a2dp_current_stream));
            WPRINT_APP_INFO(("[BT-A2DP]: Music streaming from the phone\n"));
#endif
        }
    }
    break;

    case WICED_BT_A2DP_SINK_SUSPEND_EVT:
    {
        a2dp_context_data_t *suspend_stream;
        memset(bda, 0, sizeof(wiced_bt_device_address_t));
        memcpy(bda, p_data->suspend.bd_addr, sizeof(wiced_bt_device_address_t));
        WPRINT_APP_INFO(("bt_audio_sink_control_cb:STREAM SUSPEND EVENT status = %d Remote Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                         p_data->suspend.result, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));
        context_index = get_a2dp_context(bda);

#ifndef USE_WICED_HCI
#if 0
        if (wiced_rtos_is_timer_running(&bt_audio_avdt_suspend_timer) == WICED_SUCCESS)
        {
            wiced_rtos_stop_timer(&bt_audio_avdt_suspend_timer);
            WPRINT_APP_INFO(("stopping bt_audio_avdt_suspend_timer\n"));
        }
#endif
#endif

        if (context_index >= BT_A2DP_CONTEXT_MAX_LIMIT)
        {
            WPRINT_APP_INFO(("%s:Invalid Index \n", __func__));
            break;
        }

        suspend_stream = &a2dp_context[context_index];
        if (suspend_stream == app_data.a2dp_current_stream && a2dp_context[context_index].stream_status == STREAM_PLAYING)
        {
            bt_audio_suspend_cfm_a2dp_stream(suspend_stream);
            WPRINT_APP_INFO(("[BT-A2DP]: Music suspended from the phone\n"));
        }
        else
        {
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("WICED_BT_A2DP_SINK_SUSPEND_EVT app_data.a2dp_current_stream->stream_status:%d, a2dp_context[context_index].stream_status:%d\n",
                             app_data.a2dp_current_stream->stream_status, a2dp_context[context_index].stream_status));
#endif
            a2dp_context[context_index].stream_status = STREAM_IDLE;
            if (app_data.a2dp_current_stream->stream_status == STREAM_SUSPEND_PENDING)
            {
                bt_audio_start_a2dp_stream(app_data.a2dp_current_stream);
                WPRINT_APP_INFO(("[BT-A2DP]: Music streaming from the other AV Source\n"));
            }
        }
#ifdef USE_MEM_POOL
        bt_buffer_pool_print_debug_info(mem_pool);
#endif
    }
    break;

    case WICED_BT_A2DP_SINK_CODEC_CONFIG_EVT:
    {
        WPRINT_APP_INFO(("bt_audio_sink_control_cb:CODEC CONFIG \nCODEC ID:0x%02X  FS:0x%02X  CH_MODE:0x%02X  BLOCK_LEN:0x%02X  NUM_SUBBANDS:0x%02X  ALLOC_METHOD:0x%02X  MAX_BITPOOL:0x%02X  MIN_BITPOOL:0x%02X\n",
                         p_data->codec_config.codec_id, p_data->codec_config.cie.sbc.samp_freq, p_data->codec_config.cie.sbc.ch_mode, p_data->codec_config.cie.sbc.block_len,
                         p_data->codec_config.cie.sbc.num_subbands, p_data->codec_config.cie.sbc.alloc_mthd, p_data->codec_config.cie.sbc.max_bitpool, p_data->codec_config.cie.sbc.min_bitpool));

        /* Save the codec configuration received from the peer, this is used to configure decoder and player at the time of starting the audio stream */
        memcpy(&codec_config, &p_data->codec_config, sizeof(wiced_bt_a2dp_codec_info_t));
    }
    break;
    }
}


static void bt_audio_sink_data_cb(wiced_bt_a2dp_sink_codec_t codec_type, wiced_bt_a2dp_sink_audio_data_t *p_audio_data)
{
    bt_audio_codec_data_t *audio = NULL;
    uint8_t *in_audio;
    uint16_t in_length;
    wiced_app_service_t *service = wiced_get_entry(SERVICE_BT_A2DP);
    //rt_kprintf("[BT-A2DP] bt_audio_sink_data_cb\n");

    if (service == NULL)
    {
        WPRINT_APP_ERROR(("[BT-A2DP] No A2DP service entry\n"));
        return;
    }

    if ((service->state != SERVICE_PLAYING_AUDIO))
    {
        rt_kprintf("[BT-A2DP] No SERVICE_PLAYING_AUDIO %d\n", service->state);
        WPRINT_APP_ERROR(("[BT-A2DP] No SERVICE_PLAYING_AUDIO %d\n", service->state));
        return;
    }

    /*Skip one byte for SBC payload... TBD if this should be done in profile library*/
    p_audio_data->p_pkt->len--;
    p_audio_data->p_pkt->offset++;

    in_length = p_audio_data->p_pkt->len;

#ifdef USE_MEM_POOL
    audio = bt_buffer_pool_allocate_buffer(mem_pool);
#else
    audio = rt_malloc(sizeof(bt_audio_codec_data_t) + in_length);
#endif
    if (audio == NULL)
    {
        WPRINT_APP_INFO(("bt_audio_sink_data_cb: mem alloc failed\n"));
        return;
    }
    in_audio = ((uint8_t *)(p_audio_data->p_pkt + 1)) + p_audio_data->p_pkt->offset;
    memcpy(audio->data, in_audio, in_length);
    audio->length = in_length;
    audio->offset = 0;

    //rt_kprintf("[BT-A2DP] bt_audio_write_to_decoder_queue\n");
    bt_audio_write_to_decoder_queue(audio);
}

#ifdef DUAL_A2DP_LOG_ENABLE
static char *remote_control_event_id_str(uint8_t event_id)
{
    switch (event_id)
    {
    case AVRC_EVT_PLAY_STATUS_CHANGE:
        return "PLAY STATUS CHANGED";
    case AVRC_EVT_TRACK_CHANGE:
        return "TRACK CHANGED";
    case AVRC_EVT_TRACK_REACHED_END:
        return "TRACK REACHED END";
    case AVRC_EVT_TRACK_REACHED_START:
        return "TRACK REACHED START";
    case AVRC_EVT_PLAY_POS_CHANGED:
        return "PLAY POS CHANGED";
    case AVRC_EVT_NOW_PLAYING_CHANGE:
        return "PLAYING CHANGE";
    case AVRC_EVT_VOLUME_CHANGE:
        return "VOLUME CHANGED";
    default:
        return "EVENT NOT HANDLED";
        break;
    }
    return NULL;
}

static char *remote_control_play_status_str(uint8_t status)
{
    switch (status)
    {
    case AVRC_PLAYSTATE_STOPPED:
        return "PLAYSTATE STOPPED";
    case AVRC_PLAYSTATE_PLAYING:
        return "PLAYSTATE PLAYING";
    case AVRC_PLAYSTATE_PAUSED:
        return "PLAYSTATE PAUSED";
    case AVRC_PLAYSTATE_FWD_SEEK:
        return "PLAYSTATE FWD SEEK";
    case AVRC_PLAYSTATE_REV_SEEK:
        return "PLAYSTATE REV SEEK";
    case AVRC_PLAYSTATE_ERROR:
        return "PLAYSTATE ERROR";
    default:
        return "EVENT NOT HANDLED";
        break;
    }
    return NULL;
}
#endif

static void bt_audio_remote_control_connection_state_cback(wiced_bt_device_address_t remote_addr, wiced_result_t status,
        wiced_bt_remote_control_connection_state_t connection_state, uint32_t peer_features)
{
    int context_index;
    wiced_bt_device_address_t null_addr = {0, 0, 0, 0, 0, 0,};

    if (status != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("%s: something went wrong, status = %d \n", __func__, status));
    }

    if (connection_state == REMOTE_CONTROL_CONNECTED)
    {
        context_index = set_a2dp_context(remote_addr);
        if (context_index < BT_A2DP_CONTEXT_MAX_LIMIT)
        {
            WPRINT_APP_INFO(("%s: context_index = %d \n", __func__, context_index));
        }
        else
        {
            WPRINT_APP_INFO(("%s: Invalid Index context_index = %d \n", __func__, context_index));
            return;
        }
        if (!memcmp(a2dp_context[context_index].avrc_peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
        {
            memcpy(a2dp_context[context_index].avrc_peer_address, remote_addr, sizeof(wiced_bt_device_address_t)) ;
        }
        else
        {
            wiced_assert("peer bd adress for a2dp and avrcp do not match!\n", (!memcmp(remote_addr, a2dp_context[context_index].avrc_peer_address, sizeof(wiced_bt_device_address_t))));
            if (memcmp(remote_addr, a2dp_context[context_index].avrc_peer_address, sizeof(wiced_bt_device_address_t)))
            {
                if (!memcmp(a2dp_context[context_index].a2dp_peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
                {
                    a2dp_context[context_index].sink_connected = WICED_FALSE;
                    a2dp_context[context_index].in_use_context = WICED_FALSE;
                }
                return;
            }
        }
        a2dp_context[context_index].peer_features = peer_features;
        a2dp_context[context_index].rc_connected = WICED_TRUE;
#if 0
        if (wiced_rtos_is_timer_running(&bt_audio_avrcp_timer))
        {
            wiced_rtos_stop_timer(&bt_audio_avrcp_timer);
        }
        WPRINT_APP_INFO(("%s: AVRCP Connected \n", __func__));

        signal_for_event_semaphore(WICED_BT_CONNECT_AVRCP, WICED_TRUE);
#endif
    }

    else //REMOTE_CONTROL_DISCONNECTED
    {
        context_index = get_a2dp_context(remote_addr);
        if (context_index < BT_A2DP_CONTEXT_MAX_LIMIT)
        {
            WPRINT_APP_INFO(("%s: context_index = %d \n", __func__, context_index));
        }
        else
        {
            WPRINT_APP_INFO(("%s: Invalid Index context_index = %d \n", __func__, context_index));
            return;
        }
        memset(a2dp_context[context_index].avrc_peer_address, 0, sizeof(wiced_bt_device_address_t));
        a2dp_context[context_index].peer_features = 0;
        a2dp_context[context_index].rc_connected = WICED_FALSE;
        if (!memcmp(a2dp_context[context_index].a2dp_peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
        {
            a2dp_context[context_index].sink_connected = WICED_FALSE;
            a2dp_context[context_index].in_use_context = WICED_FALSE;
        }
#if 0
        signal_for_event_semaphore(WICED_BT_DISCONNECT_AVRCP, WICED_TRUE);
#endif
    }
}


static void bt_audio_remote_control_cmd_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_command_t *avrc_cmd)
{
    wiced_result_t result;
    int index;

    index = get_a2dp_context(remote_addr);
    if (index >= 0 && index < BT_A2DP_CONTEXT_MAX_LIMIT)
    {
        WPRINT_APP_INFO(("%s: index = %d \n", __func__, index));
    }
    else
    {
        WPRINT_APP_INFO(("%s: Invalid Index = %d \n", __func__, index));
        return;
    }

    if (memcmp(remote_addr, a2dp_context[index].avrc_peer_address, sizeof(wiced_bt_device_address_t)))
    {
        WPRINT_APP_INFO(("%s: bt addresses don't match\n", __func__));
        if (!memcmp(a2dp_context[index].a2dp_peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
        {
            a2dp_context[index].sink_connected = WICED_FALSE;
            a2dp_context[index].in_use_context = WICED_FALSE;
        }
        return;
    }

    switch (avrc_cmd->pdu)
    {
    case AVRC_PDU_SET_ABSOLUTE_VOLUME:
        a2dp_context[index].audio_config.volume = avrc_cmd->volume.volume;
        if (app_data.a2dp_media_state == WICED_TRUE)
        {
            if ((app_data.a2dp_current_stream != NULL) &&
                    (!memcmp(remote_addr, app_data.a2dp_current_stream->a2dp_peer_address, sizeof(wiced_bt_device_address_t))))
            {
                result = bt_audio_update_player_volume(a2dp_context[index].audio_config.volume);
                WPRINT_APP_INFO(("%s: AVRC_PDU_SET_ABSOLUTE_VOLUME vol:%d, set volume result=%d\n", __func__, avrc_cmd->volume.volume, result));
            }
            else
            {
                WPRINT_APP_INFO(("%s: Ignore the volume update from Non streaming device\n", __func__));
            }
        }
        break;

    default:
        break;
    }
}

static void bt_audio_remote_control_rsp_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_response_t *avrc_rsp)
{
    int index;

#ifdef DUAL_A2DP_LOG_ENABLE
    WPRINT_APP_INFO(("%s: \n", __func__));
#endif
    index = get_a2dp_context(remote_addr);
    if (index >= BT_A2DP_CONTEXT_MAX_LIMIT)
    {
        WPRINT_APP_INFO(("%s: Invalid index = %d \n", __func__, index));
        return;
    }

    if (memcmp(remote_addr, a2dp_context[index].avrc_peer_address, sizeof(wiced_bt_device_address_t)))
    {
        WPRINT_APP_INFO(("%s: bt addresses don't match\n", __func__));
        if (!memcmp(a2dp_context[index].a2dp_peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
        {
            a2dp_context[index].in_use_context = WICED_FALSE;
        }
        return;
    }
#ifdef DUAL_A2DP_LOG_ENABLE
    WPRINT_APP_INFO(("[REMOTE-CONTROL]: %s pdu [%x] status [%d] from Device Idx:%d\n", __func__, avrc_rsp->pdu, avrc_rsp->rsp.status, index));
#endif
    switch (avrc_rsp->pdu)
    {
    case AVRC_PDU_GET_PLAY_STATUS:
    {
#ifdef DUAL_A2DP_LOG_ENABLE
        wiced_bt_avrc_get_play_status_rsp_t *play_status = (wiced_bt_avrc_get_play_status_rsp_t *)avrc_rsp;;

        WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s GET PLAY STATUS: play status: %s song_pos [%d] song_len [%d]\n", __func__,
                         remote_control_play_status_str(play_status->play_status), (unsigned int)play_status->song_pos, (unsigned int)play_status->song_len));
#endif
#ifdef USE_AUDIO_DISPLAY
        wiced_bt_avrc_get_play_status_rsp_t *display_play_status_ptr = (wiced_bt_avrc_get_play_status_rsp_t *)avrc_rsp;;
        audio_display_footer_update_time_info(((display_play_status_ptr->song_pos) / 1000), ((display_play_status_ptr->song_len) / 1000));
        audio_display_footer_update_options(FOOTER_IS_VISIBLE | FOOTER_CENTER_ALIGN);
#endif /* USE_AUDIO_DISPLAY */

    }
    break;
    case AVRC_PDU_GET_ELEMENT_ATTR:
    {
        wiced_bt_avrc_get_elem_attrs_rsp_t *elements = (wiced_bt_avrc_get_elem_attrs_rsp_t *)avrc_rsp;
        uint32_t localAttr = 0;
        uint8_t  strName[25] = {0};

        while (localAttr < elements->num_attr)
        {
            memcpy(strName, elements->p_attrs[localAttr].name.p_str,
                   elements->p_attrs[localAttr].name.str_len > 25 ? 24 : (elements->p_attrs[localAttr].name.str_len));
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s ELEMENT ATTR RSP: Attr-ID [%d] strName [%s] FROM Device Idx:%d\n ", __func__,
                             (unsigned int)elements->p_attrs[localAttr].attr_id, strName, index));
#endif
            memset(strName, 0, sizeof(strName));
            localAttr++;
        }
#ifdef USE_AUDIO_DISPLAY
        wiced_bt_avrc_get_elem_attrs_rsp_t *elements1 = (wiced_bt_avrc_get_elem_attrs_rsp_t *)avrc_rsp;
        uint8_t strName1[25] = {'\0'};
        uint8_t strName2[25] = {'\0'};
        if (elements1)
        {
            if (elements1->p_attrs)
            {
                if (elements1->p_attrs[0].name.p_str)
                {
                    memcpy(strName1, elements1->p_attrs[0].name.p_str,
                           elements1->p_attrs[0].name.str_len > 25 ? 24 : (elements1->p_attrs[0].name.str_len));
                    WPRINT_APP_INFO(("VALUE = %d strname1 = %s\n\n", elements1->p_attrs[0].name.str_len, strName1));
                    strName1[24] = '\0';
                }
                else break;
            }
            else break;
        }
        else break;


        if (elements1 && (elements1->num_attr > 1))
        {
            if (elements1->p_attrs)
            {
                if (elements1->p_attrs[1].name.p_str)
                {
                    memcpy(strName2, elements1->p_attrs[1].name.p_str,
                           elements1->p_attrs[1].name.str_len > 25 ? 24 : (elements1->p_attrs[1].name.str_len));
                    WPRINT_APP_INFO(("VALUE = %d strname2 = %s\n\n", elements1->p_attrs[1].name.str_len, strName2));
                    strName2[24] = '\0';
                }
                else break;
            }
            else break;
        }
        else break;

        audio_display_footer_update_song_info((char *)strName1, (char *)strName2);
        audio_display_footer_update_options(FOOTER_IS_VISIBLE | FOOTER_CENTER_ALIGN);
#endif /* USE_AUDIO_DISPLAY */
    }
    break;
    case AVRC_PDU_REGISTER_NOTIFICATION:
    {
        wiced_bt_avrc_reg_notif_rsp_t *reg_notif = NULL;
        reg_notif = (wiced_bt_avrc_reg_notif_rsp_t *)avrc_rsp;

        switch (reg_notif->event_id)
        {
        case AVRC_EVT_PLAY_STATUS_CHANGE:
        {
            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s REG_NOTIF: AVRC_EVT_PLAY_STATUS_CHANGE\n", __func__));
            wiced_bt_remote_control_get_play_status_cmd(a2dp_context[index].avrc_peer_address);
        }
        break;
        case AVRC_EVT_TRACK_CHANGE:
        {
            wiced_bt_avrc_uid_t element_id = {0};
            uint32_t attrs[] = { AVRC_MEDIA_ATTR_ID_TITLE, AVRC_MEDIA_ATTR_ID_ARTIST, AVRC_MEDIA_ATTR_ID_PLAYING_TIME };
            uint8_t num_attr = 3;

            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s REG_NOTIF: AVRC_EVT_TRACK_CHANGE\n", __func__));
            wiced_bt_remote_control_get_element_attr_cmd(a2dp_context[index].avrc_peer_address, element_id, num_attr, attrs);
        }
        break;
        case AVRC_EVT_NOW_PLAYING_CHANGE:
        {
            wiced_bt_avrc_uid_t element_id = {0};
            uint32_t attrs = AVRC_MEDIA_ATTR_ID_TITLE;
            uint8_t num_attr = 1;

            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s REG_NOTIF: AVRC_EVT_NOW_PLAYING_CHANGE\n", __func__));
            wiced_bt_remote_control_get_element_attr_cmd(a2dp_context[index].avrc_peer_address, element_id, num_attr, &attrs);
        }
        break;
        case AVRC_EVT_TRACK_REACHED_END:
        case AVRC_EVT_TRACK_REACHED_START:
        case AVRC_EVT_PLAY_POS_CHANGED:
        {
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s event-id [%s] : \n", __func__,
                             remote_control_event_id_str(reg_notif->event_id)));
#endif
#if 0
            wiced_bt_remote_control_get_play_status_cmd(a2dp_context[index].avrc_peer_address);
#endif

        }
        break;
        case AVRC_EVT_VOLUME_CHANGE:
        {
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s event-id [%s] : volume: %d FROM Device Idx:%d\n", __func__,
                             remote_control_event_id_str(reg_notif->event_id), reg_notif->param.volume, index));
#endif
            a2dp_context[index].audio_config.volume = reg_notif->param.volume;
        }
        break;
        }
    }
    break;
    }
}


/**
 * static wiced_result_t handle_skip_backward_action( void )
 *
 * @return WICED_SUCCESS BACK action succeeded
 *         WICED_ERROR   action failed.
 */
wiced_result_t bt_audio_a2dp_send_passthru_command(uint8_t cmd, wiced_bt_device_address_t remote_addr)
{
    wiced_result_t result;
    WPRINT_APP_INFO(("bt_audio_a2dp_send_passthru_command: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                     remote_addr[0], remote_addr[1], remote_addr[2], remote_addr[3], remote_addr[4], remote_addr[5]));

    result = wiced_bt_remote_control_send_pass_through_cmd(remote_addr, cmd, AVRC_STATE_PRESS, 0, NULL);
    if (result != WICED_SUCCESS)
        return result;
    wiced_rtos_delay_milliseconds(50);
    result = wiced_bt_remote_control_send_pass_through_cmd(remote_addr, cmd, AVRC_STATE_RELEASE, 0, NULL);
    return result;
}

static a2dp_context_data_t *bt_audio_get_active_a2dp_device()
{
    int index;
    bt_audio_paired_device_info_t out_device;
    a2dp_context_data_t *active_stream = NULL;

    /*If there is active streaming device*/
    if (app_data.a2dp_current_stream)
    {
        return app_data.a2dp_current_stream;
    }
    if (app_data.a2dp_last_stream)
    {
        return app_data.a2dp_last_stream;
    }

    /*If no device is streaming then choose the device that is paired*/
    if (bt_audio_nv_get_device_info_by_index(0, &out_device) == WICED_SUCCESS)
    {
        index = get_a2dp_context(out_device.device_link.bd_addr);
        if (a2dp_context[index].in_use_context == WICED_TRUE &&
                a2dp_context[index].rc_connected == WICED_TRUE &&
                a2dp_context[index].sink_connected == WICED_TRUE)
            return &a2dp_context[index];
        else
            return active_stream;
    }
    else
        return active_stream;
}

static wiced_result_t bt_audio_a2dp_skip_backward(void)
{
    WPRINT_APP_INFO(("[BT-A2DP]: sending SKIP_BACKWARD\n"));

    if (app_data.a2dp_current_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active stream\n", __func__));
        return WICED_ERROR;
    }
    return bt_audio_a2dp_send_passthru_command(AVRC_ID_BACKWARD, app_data.a2dp_current_stream->a2dp_peer_address);
}

static wiced_result_t bt_audio_a2dp_pause_play(void)
{
    wiced_result_t result;
    a2dp_context_data_t *active_stream = bt_audio_get_active_a2dp_device();

    if (active_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active device\n", __func__));
        return WICED_ERROR;
    }

    if (app_data.play_button_state == WICED_TRUE)
    {
        WPRINT_APP_INFO(("[BT-A2DP] play_state: 1 sending PAUSE\n"));
        result = bt_audio_a2dp_send_passthru_command(AVRC_ID_PAUSE, active_stream->a2dp_peer_address);
        if (result == WICED_SUCCESS)
            app_data.play_button_state = WICED_FALSE;
    }
    else
    {
        WPRINT_APP_INFO(("[BT-A2DP] play_state: 0 sending PLAY\n"));
        result = bt_audio_a2dp_send_passthru_command(AVRC_ID_PLAY, active_stream->a2dp_peer_address);
        if (result == WICED_SUCCESS)
            app_data.play_button_state = WICED_TRUE;
    }
    return result;
}

static wiced_result_t bt_audio_a2dp_update_volume(uint8_t vol_level)
{
    wiced_result_t result;

    result = bt_audio_update_player_volume(vol_level);
    if (result != WICED_SUCCESS)
    {
        return result;
    }

    if (app_data.a2dp_current_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active stream\n", __func__));
        return WICED_ERROR;
    }

    result = wiced_bt_remote_control_set_volume_cmd(app_data.a2dp_current_stream->a2dp_peer_address, vol_level);
    return result;
}

static wiced_result_t bt_audio_a2dp_volume_up(void)
{
    wiced_result_t result;
    uint8_t vol_level;

    if (app_data.a2dp_current_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active stream\n", __func__));
        return WICED_ERROR;
    }

    vol_level = app_data.a2dp_current_stream->audio_config.volume;

    if (vol_level == BT_AUDIO_VOLUME_MAX)
    {
        WPRINT_APP_INFO(("[BT-A2DP] Volume level already Maximum\n"));
        return WICED_SUCCESS;
    }

    if (vol_level == BT_AUDIO_VOLUME_MIN)
    {
        vol_level = (BT_AUDIO_VOLUME_STEP - 1);
    }
    else
    {
        vol_level += BT_AUDIO_VOLUME_STEP;
    }

    if (vol_level > BT_AUDIO_VOLUME_MAX)
    {
        vol_level = BT_AUDIO_VOLUME_MAX;
    }
    WPRINT_APP_INFO(("[BT-A2DP] bt_audio_a2dp_volume_up\n"));

    result = bt_audio_a2dp_update_volume(vol_level);
    app_data.a2dp_current_stream->audio_config.volume = vol_level;

    return result;
}


/**
 * static wiced_result_t handle_volume_down( void )
 *
 * @return
 */
static wiced_result_t bt_audio_a2dp_volume_down(void)
{
    wiced_result_t result;
    uint8_t vol_level;

    if (app_data.a2dp_current_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active stream\n", __func__));
        return WICED_ERROR;
    }

    vol_level = app_data.a2dp_current_stream->audio_config.volume;

    if (vol_level == BT_AUDIO_VOLUME_MIN)
    {
        WPRINT_APP_INFO(("[BT-A2DP] Volume level already Minimum\n"));
        return WICED_SUCCESS;
    }

    if ((vol_level - BT_AUDIO_VOLUME_STEP) < (BT_AUDIO_VOLUME_MIN))
    {
        vol_level = BT_AUDIO_VOLUME_MIN ;
    }
    else
    {
        vol_level -= BT_AUDIO_VOLUME_STEP;
    }
    WPRINT_APP_INFO(("[BT-A2DP] bt_audio_a2dp_volume_down\n"));

    result = bt_audio_a2dp_update_volume(vol_level);
    app_data.a2dp_current_stream->audio_config.volume = vol_level;

    return result;
}

/**
 * static wiced_result_t handle_stop_action( void )
 *
 * @return
 */
static wiced_result_t bt_audio_a2dp_stop(void)
{
    if (app_data.a2dp_current_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active stream\n", __func__));
        return WICED_ERROR;
    }
    WPRINT_APP_INFO(("[BT-A2DP]: sending AVRC_ID_STOP\n"));

    return bt_audio_a2dp_send_passthru_command(AVRC_ID_STOP, app_data.a2dp_current_stream->a2dp_peer_address);
}

/**
 * static wiced_result_t bt_audio_a2dp_skip_forward( void )
 *
 * @return
 */
static wiced_result_t bt_audio_a2dp_skip_forward(void)
{
    if (app_data.a2dp_current_stream == NULL)
    {
        WPRINT_APP_INFO(("[BT-A2DP] :%s No Active stream\n", __func__));
        return WICED_ERROR;
    }
    WPRINT_APP_INFO(("[BT-A2DP]: sending SKIP_FORWARD\n"));
    return bt_audio_a2dp_send_passthru_command(AVRC_ID_FORWARD, app_data.a2dp_current_stream->a2dp_peer_address);
}

static void bt_audio_a2dp_allow_service(wiced_app_service_t *service)
{
    WPRINT_APP_INFO(("[BT-A2DP] %s\n", __func__));

    if (service == NULL || service->type != SERVICE_BT_A2DP)
    {
        WPRINT_APP_ERROR(("[BT-A2DP] Invalid argument\n"));
        return;
    }

    if (service->state == SERVICE_PREEMPTED || service->state == SERVICE_PREVENTED)
    {
        service->state = SERVICE_IDLE;
    }

    if (service->state == SERVICE_PENDING && app_data.a2dp_current_stream != NULL)
    {
        bt_audio_configure_device(&app_data.a2dp_current_stream->audio_config, BT_AUDIO_DEVICE_PLAYER, SERVICE_BT_A2DP);
        app_set_current_service(SERVICE_BT_A2DP);
        app_set_service_state(SERVICE_BT_A2DP, SERVICE_PLAYING_AUDIO);
        app_data.a2dp_current_stream->stream_status = STREAM_PLAYING;
        app_data.a2dp_media_state = WICED_TRUE;
        app_data.play_button_state = WICED_TRUE;
    }
#ifdef USE_AUDIO_DISPLAY
    audio_display_header_update_options(BATTERY_ICON_IS_VISIBLE | BATTERY_ICON_SHOW_PERCENT | SIGNAL_STRENGTH_IS_VISIBLE | BLUETOOTH_IS_CONNECTED);
    audio_display_footer_update_options(FOOTER_IS_VISIBLE | FOOTER_CENTER_ALIGN);
#endif /* USE_AUDIO_DISPLAY */
}

static void bt_audio_a2dp_prevent_service(wiced_app_service_t *service)
{
    WPRINT_APP_INFO(("[BT-A2DP] %s\n", __func__));

    if (service == NULL || service->type != SERVICE_BT_A2DP)
    {
        WPRINT_APP_ERROR(("[BT-A2DP] Invalid argument\n"));
        return;
    }

    bt_audio_end_loop(BT_AUDIO_DEVICE_PLAYER);

    if (service->state == SERVICE_PLAYING_AUDIO && app_data.a2dp_current_stream != NULL)
    {
        bt_audio_a2dp_send_passthru_command(AVRC_ID_PAUSE, app_data.a2dp_current_stream->a2dp_peer_address);
        wiced_bt_a2dp_sink_suspend(app_data.a2dp_current_stream->a2dp_peer_address);
        service->state = SERVICE_PREEMPTED;
    }
    else
    {
        service->state = SERVICE_PREVENTED;
    }
}

static wiced_result_t bt_audio_a2dp_button_event_handler(app_service_action_t action)
{
    wiced_result_t ret = WICED_ERROR;
    switch (action)
    {
    case ACTION_VOLUME_UP:
        ret = bt_audio_a2dp_volume_up();
        break;

    case ACTION_VOLUME_DOWN:
        ret = bt_audio_a2dp_volume_down();
        break;

    case ACTION_PAUSE_PLAY:
        ret = bt_audio_a2dp_pause_play();
        break;

    case ACTION_FORWARD:
        ret = bt_audio_a2dp_skip_forward();
        break;

    case ACTION_BACKWARD:
        ret = bt_audio_a2dp_skip_backward();
        break;

    case ACTION_STOP:
        ret = bt_audio_a2dp_stop();
        break;

    case ACTION_FACTORY_RESET:
    case ACTION_MULTI_FUNCTION_SHORT_RELEASE:
    case ACTION_MULTI_FUNCTION_LONG_RELEASE:
    case NO_ACTION:
    default:
        break;
    }

    return ret;
}


static wiced_result_t bt_audio_get_config_from_cie(wiced_bt_a2dp_codec_info_t *p_codec_config, bt_audio_config_t *p_audio_config)
{
    wiced_result_t result = WICED_BADARG;

    if (p_codec_config == NULL || p_audio_config == NULL)
        return result;

    if (p_codec_config->codec_id == WICED_BT_A2DP_SINK_CODEC_SBC)
    {
        if (p_codec_config->cie.sbc.samp_freq == A2D_SBC_IE_SAMP_FREQ_44)
            p_audio_config->sample_rate = 44100;
        else if (p_codec_config->cie.sbc.samp_freq == A2D_SBC_IE_SAMP_FREQ_48)
            p_audio_config->sample_rate = 48000;
        else if (p_codec_config->cie.sbc.samp_freq == A2D_SBC_IE_SAMP_FREQ_32)
            p_audio_config->sample_rate = 32000;
        else if (p_codec_config->cie.sbc.samp_freq == A2D_SBC_IE_SAMP_FREQ_16)
            p_audio_config->sample_rate = 16000;
        else
            return WICED_ERROR;

        if (p_codec_config->cie.sbc.ch_mode == A2D_SBC_IE_CH_MD_MONO)
            p_audio_config->channels = 1;
        else
            p_audio_config->channels = 2;

        p_audio_config->bits_per_sample = 16;
        result = WICED_SUCCESS;
    }
    return result;
}

static int get_a2dp_context(wiced_bt_device_address_t remote_addr)
{
    int i = 0;

#ifdef DUAL_A2DP_LOG_ENABLE
    WPRINT_APP_INFO(("get_a2dp_context for remote addr : [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                     remote_addr[0], remote_addr[1], remote_addr[2],
                     remote_addr[3], remote_addr[4], remote_addr[5]));
#endif
    while (i < BT_A2DP_CONTEXT_MAX_LIMIT)
    {
#ifdef DUAL_A2DP_LOG_ENABLE
        WPRINT_APP_INFO(("a2dp_context[%d].in_use_context:%d \n", i, a2dp_context[i].in_use_context));
#endif
        if ((a2dp_context[i].in_use_context == WICED_TRUE) &&
                ((!memcmp(remote_addr, a2dp_context[i].avrc_peer_address, sizeof(wiced_bt_device_address_t))) ||
                 (!memcmp(remote_addr, a2dp_context[i].a2dp_peer_address, sizeof(wiced_bt_device_address_t)))))
        {
#ifdef DUAL_A2DP_LOG_ENABLE
            WPRINT_APP_INFO(("get_a2dp_context Already present i:%d\n", i));
#endif
            return i;
        }
        i++;
    }
    return BT_A2DP_CONTEXT_MAX_LIMIT;
}

static int set_a2dp_context(wiced_bt_device_address_t remote_addr)
{
    int i = 0;

#ifdef DUAL_A2DP_LOG_ENABLE
    WPRINT_APP_INFO(("set_a2dp_context for remote addr : [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                     remote_addr[0], remote_addr[1], remote_addr[2],
                     remote_addr[3], remote_addr[4], remote_addr[5]));
#endif
    while (i < BT_A2DP_CONTEXT_MAX_LIMIT)
    {
#ifdef DUAL_A2DP_LOG_ENABLE
        WPRINT_APP_INFO(("a2dp_context[%d].in_use_context:%d \n", i, a2dp_context[i].in_use_context));
#endif
        if ((a2dp_context[i].in_use_context == WICED_TRUE) &&
                ((!memcmp(remote_addr, a2dp_context[i].avrc_peer_address, sizeof(wiced_bt_device_address_t))) ||
                 (!memcmp(remote_addr, a2dp_context[i].a2dp_peer_address, sizeof(wiced_bt_device_address_t)))))
        {
            WPRINT_APP_INFO(("set_a2dp_context Already present i:%d\n", i));
            return i;
        }
        else if (a2dp_context[i].in_use_context == WICED_FALSE)
        {
            a2dp_context[i].in_use_context = WICED_TRUE;
            WPRINT_APP_INFO(("set_a2dp_context New entry i:%d\n", i));
            return i;
        }
        i++;
    }
    return BT_A2DP_CONTEXT_MAX_LIMIT;
}

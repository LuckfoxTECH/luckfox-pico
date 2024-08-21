/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * Bluetooth Audio AVDT Sink Application
 *
 * The application demonstrates the following features:
 *
 *     - Bluetooth intialization
 *     - A2DP sink
 *         - Playback of Bluetooth Audio
 *         - SBC Decoding
 *     - AVRCP
 *         - Remote control (PLAY/PAUSE/FWD/BACKWARD etc.) using the command line.
 *         - Buttons are not supported in this app.
 *
 * Application Instructions
 *
 *   1. This application needs bluetooth for A2DP/AVRCP and hardware that supports I2S and a speaker.
 *
 *   2. The Bluetooth BD ADDRESS can be changed by the #define WICED_BLUETOOTH_DEVICE_ADDRESS in bt_config_dct.h.
 *      The device name can be changed by the #define WICED_BLUETOOTH_DEVICE_NAME in bt_config_dct.h.
 *
 * Usage:
 *    On startup device will be discoverable and connectable,
 *    allowing a BT audio source to connect and stream audio.
 *    AVRCP console command support added.
 *    Once the connection is established the commands which
 *    are configured [eg:play-pause, stop] can be used to
 *    control the audio.
 *
 * Notes: Currently supports 44.1kHz and 48kHz audio
 */

#include <stdlib.h>
#include "wiced.h"
#include "wiced_rtos.h"
#include "wiced_result.h"
#include "platform_audio.h"
#include "wiced_bt_stack.h"
#include "wiced_bt_dev.h"
#include "bluetooth_audio.h"
#include "bluetooth_audio_nv.h"
#include "command_console.h"
#include "wiced_bt_remote_control.h"

#ifdef USE_MEM_POOL
#include "mem_pool.h"
#endif
#include "wwd_debug.h"

/******************************************************
 *                    Constants
 ******************************************************/
// SDP Record for Device ID
#define HDLR_DEVICE_ID                  0x10001
// SDP Record for AVDT Sink
#define HDLR_AVDT_SINK                  0x10002
// SDP Record for AVRC Controller
#define HDLR_AVRC_CONTROLLER            0x10003
// SDP Record for AVRC Target
#define HDLR_AVRC_TARGET                0x10004

#define BLUETOOTH_DEVICE_NAME           "WICED BLUETOOTH AUDIO"

#define EIR_DATA_LENGTH                 240
#define EIR_COMPLETE_LOCAL_NAME_TYPE    0x09
#define EIR_COMPLETE_16BITS_UUID_TYPE   0x03

#define BT_AUDIO_TASK_PRIORITY_BASE     ( 10 )
#define BT_AUDIO_PLAYER_TASK_PRIORITY   ( BT_AUDIO_TASK_PRIORITY_BASE )
#define BT_AUDIO_DECODER_TASK_PRIORITY  ( BT_AUDIO_TASK_PRIORITY_BASE + 1 )
#define BT_SOFT_DMA_IRQ_PRI             (BT_AUDIO_PLAYER_TASK_PRIORITY - 1)

#define PLAYER_STACK_SIZE               0x1000
#define DECODER_STACK_SIZE              0x1000

#define LOCAL_FEATURES                  REMOTE_CONTROL_FEATURE_CONTROLLER | REMOTE_CONTROL_FEATURE_TARGET

/*  Recommended max_bitpool for high quality audio */
#define BT_AUDIO_A2DP_SBC_MAX_BITPOOL 53

#define BT_AUDIO_MEM_POOL_DEFAULT_BUFFER_COUNT (20)
#define BT_AUDIO_MEM_POOL_DEFAULT_BUFFER_SIZE  (660)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef struct
{
    wiced_bool_t sink_connected;
    wiced_bool_t rc_connected;
    wiced_bt_device_address_t peer_address;
    uint32_t     peer_features;
    bt_audio_config_t audio_config;
} bt_audio_a2dp_context_data_t;

/******************************************************
 *               Function Definitions
 ******************************************************/
static wiced_bt_dev_status_t bt_audio_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data);
void bt_audio_sink_data_cb(wiced_bt_a2dp_sink_codec_t codec_type, wiced_bt_a2dp_sink_audio_data_t *p_audio_data);
static void bt_audio_sink_control_cb(wiced_bt_a2dp_sink_event_t event, wiced_bt_a2dp_sink_event_data_t *p_data);
static int  bt_audio_console_command(int arg_count, const char *command[]);
static void bt_audio_remote_control_cmd_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_command_t *avrc_cmd);
static void bt_audio_remote_control_rsp_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_response_t *avrc_rsp);
static wiced_result_t bt_audio_a2dp_update_volume(uint8_t vol_level);
static void bt_audio_remote_control_connection_state_cback(wiced_bt_device_address_t remote_addr, wiced_result_t status,
        wiced_bt_remote_control_connection_state_t connection_state, uint32_t peer_features);
static void bt_audio_connection_status_callback(wiced_bt_device_address_t bd_addr, uint8_t *p_features, wiced_bool_t is_connected, uint16_t handle,
        wiced_bt_transport_t transport, uint8_t reason);

/* RT_THREAD MSH*/

static void play()
{
    const char *cmd = "play";
    bt_audio_console_command(1,  &cmd);
}
MSH_CMD_EXPORT(play, AVK play);

static void pause()
{
    const char *cmd = "pause";
    bt_audio_console_command(1, &cmd);
}

MSH_CMD_EXPORT(pause, AVK pause);

static void stop()
{
    const char *cmd = "stop";
    bt_audio_console_command(1, &cmd);
}

MSH_CMD_EXPORT(stop, AVK stop);

static void next()
{
    const char *cmd = "next";
    bt_audio_console_command(1, &cmd);
}
MSH_CMD_EXPORT(next, AVK next);

static void previous()
{
    const char *cmd = "previous";
    bt_audio_console_command(1, &cmd);
}
MSH_CMD_EXPORT(previous, AVK previous);

static void volup()
{
    const char *cmd = "volumeup";
    bt_audio_console_command(1, &cmd);
}
MSH_CMD_EXPORT(volup, AVK volume up);

static void voldown()
{
    const char *cmd = "volumedown";
    bt_audio_console_command(1, &cmd);
}
MSH_CMD_EXPORT(voldown, AVK volume down);

/******************************************************
 *                      Macros
 ******************************************************/
#define BT_AUDIO_CONSOLE_COMMAND_HISTORY_LENGTH  (10)
#define BT_AUDIO_COMMAND_LENGTH              (85)
#define BT_AUDIO_CONSOLE_COMMANDS \
    { (char*) "play",           bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Start Playing" }, \
    { (char*) "pause",          bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Pause Playing" }, \
    { (char*) "stop",           bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Stop Playing" },  \
    { (char*) "next",           bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Next Song" },     \
    { (char*) "previous",       bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Previous Song" }, \
    { (char*) "volumeup",       bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Volume Up" },     \
    { (char*) "volumedown",     bt_audio_console_command,    0, NULL, NULL, (char *)"", (char *)  "Volume Down" },  \

/******************************************************
 *               Variables Definitions
 ******************************************************/
wiced_thread_t         decoder_thread;
wiced_thread_t         player_thread;

/*
 * simulate HW to do some IRQ work. its priority should be high
 */
//#ifdef BT_AVK_SOFT_IRQ
#if 0
typedef struct
{
    wiced_thread_t     tskID;
    rt_event_t         evt;
} tHW_SIMULATOR;
tHW_SIMULATOR hw_simulator;

#define HW_SIM_STACK_SIZE              0x400

#define SIMULATOR_EVENT_DMA_DONE   1

static void soft_hw_init()
{
    hw_simulator.evt = rt_event_create("hw", RT_IPC_FLAG_FIFO);
}

void platform_i2s_tx_dma_irq(uint32_t i2s);
static void bt_hw_simulate_task(uint32_t x)
{
    rt_uint32_t recved;
    for (;;)
    {
        rt_err_t  err = rt_event_recv(hw_simulator.evt, SIMULATOR_EVENT_DMA_DONE, RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recved);
        if (recved & SIMULATOR_EVENT_DMA_DONE)
        {
            platform_i2s_tx_dma_irq(0);
        }
        continue;
    }
}

#endif

DEFINE_APP_DCT(bt_dct_t)
{
    .bt_hash_table              = {0},
    .bt_paired_device_info[0]   = {0},
    .bt_local_id_keys           = {{0}},
};

//static uint8_t bluetooth_device_name[249] = BLUETOOTH_DEVICE_NAME;

uint8_t uuid_list[] =
{
    0x08, 0x11, /* Headset */
    0x1E, 0x11, /* Handsfree */
    0x0E, 0x11, /* AV Remote Control */
    0x0B, 0x11, /* Audio Sink */
};

const uint8_t sdp_database[] = // Define SDP database
{
    SDP_ATTR_SEQUENCE_2(327),

    // SDP Record for Device ID
    SDP_ATTR_SEQUENCE_1(77),
    SDP_ATTR_RECORD_HANDLE(HDLR_DEVICE_ID),
    SDP_ATTR_CLASS_ID(UUID_SERVCLASS_PNP_INFORMATION),
    SDP_ATTR_PROTOCOL_DESC_LIST(1),
    SDP_ATTR_BROWSE_LIST,
    SDP_ATTR_UINT2(ATTR_ID_SPECIFICATION_ID, 0x103),
    SDP_ATTR_UINT2(ATTR_ID_VENDOR_ID, 0x000F),
    SDP_ATTR_UINT2(ATTR_ID_PRODUCT_ID, 0x0000),
    SDP_ATTR_UINT2(ATTR_ID_PRODUCT_VERSION, 0x0001),
    SDP_ATTR_BOOLEAN(ATTR_ID_PRIMARY_RECORD, 0x01),
    SDP_ATTR_UINT2(ATTR_ID_VENDOR_ID_SOURCE, DI_VENDOR_ID_SOURCE_BTSIG),

    // SDP Record for AVDT Sink
    SDP_ATTR_SEQUENCE_1(77),
    SDP_ATTR_RECORD_HANDLE(HDLR_AVDT_SINK),
    SDP_ATTR_CLASS_ID(UUID_SERVCLASS_AUDIO_SINK),
    SDP_ATTR_ID(ATTR_ID_PROTOCOL_DESC_LIST), SDP_ATTR_SEQUENCE_1(16),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_PROTOCOL_L2CAP),
    SDP_ATTR_VALUE_UINT2(BT_PSM_AVDTP),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_PROTOCOL_AVDTP),
    SDP_ATTR_VALUE_UINT2(0x100),
    SDP_ATTR_ID(ATTR_ID_BT_PROFILE_DESC_LIST), SDP_ATTR_SEQUENCE_1(8),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_SERVCLASS_ADV_AUDIO_DISTRIBUTION),
    SDP_ATTR_VALUE_UINT2(0x100),
    SDP_ATTR_UINT2(ATTR_ID_SUPPORTED_FEATURES, 0x000B),
    SDP_ATTR_SERVICE_NAME(16),
    'W', 'I', 'C', 'E', 'D', ' ', 'A', 'u', 'd', 'i', 'o', ' ', 'S', 'i', 'n', 'k',

    // SDP Record for AVRC Controller
    SDP_ATTR_SEQUENCE_1(86),
    SDP_ATTR_RECORD_HANDLE(HDLR_AVRC_CONTROLLER),
    SDP_ATTR_ID(ATTR_ID_SERVICE_CLASS_ID_LIST), SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_SERVCLASS_AV_REMOTE_CONTROL),
    SDP_ATTR_UUID16(UUID_SERVCLASS_AV_REM_CTRL_CONTROL),
    SDP_ATTR_ID(ATTR_ID_PROTOCOL_DESC_LIST), SDP_ATTR_SEQUENCE_1(16),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_PROTOCOL_L2CAP),
    SDP_ATTR_VALUE_UINT2(BT_PSM_AVCTP),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_PROTOCOL_AVCTP),
    SDP_ATTR_VALUE_UINT2(0x104),
    SDP_ATTR_ID(ATTR_ID_BT_PROFILE_DESC_LIST), SDP_ATTR_SEQUENCE_1(8),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_SERVCLASS_AV_REMOTE_CONTROL),
    SDP_ATTR_VALUE_UINT2(0x0103),
    SDP_ATTR_UINT2(ATTR_ID_SUPPORTED_FEATURES, 0x0001),
    SDP_ATTR_SERVICE_NAME(22),
    'W', 'I', 'C', 'E', 'D', ' ', 'A', 'u', 'd', 'i', 'o', ' ', 'C', 'o', 'n', 't', 'r', 'o', 'l', 'l', 'e', 'r',

    // SDP Record for AVRC Target
    SDP_ATTR_SEQUENCE_1(79),
    SDP_ATTR_RECORD_HANDLE(HDLR_AVRC_TARGET),
    SDP_ATTR_CLASS_ID(UUID_SERVCLASS_AV_REM_CTRL_TARGET),
    SDP_ATTR_ID(ATTR_ID_PROTOCOL_DESC_LIST), SDP_ATTR_SEQUENCE_1(16),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_PROTOCOL_L2CAP),
    SDP_ATTR_VALUE_UINT2(BT_PSM_AVCTP),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_PROTOCOL_AVCTP),
    SDP_ATTR_VALUE_UINT2(0x104),
    SDP_ATTR_ID(ATTR_ID_BT_PROFILE_DESC_LIST), SDP_ATTR_SEQUENCE_1(8),
    SDP_ATTR_SEQUENCE_1(6),
    SDP_ATTR_UUID16(UUID_SERVCLASS_AV_REMOTE_CONTROL),
    SDP_ATTR_VALUE_UINT2(0x0105),
    SDP_ATTR_UINT2(ATTR_ID_SUPPORTED_FEATURES, 0x0002),
    SDP_ATTR_SERVICE_NAME(18),
    'W', 'I', 'C', 'E', 'D', ' ', 'A', 'u', 'd', 'i', 'o', ' ', 'T', 'a', 'r', 'g', 'e', 't',
};

/* Name, address and class of device shall be over-written using DCT entries if present.
* Please use the DCT entries to configure these parameters */
wiced_bt_cfg_settings_t wiced_bt_audio_cfg_settings =
{
    .device_name               = (uint8_t *)BLUETOOTH_DEVICE_NAME,
    .device_class              = {0x24, 0x04, 0x18},
    .security_requirement_mask = BTM_SEC_NONE,
    .max_simultaneous_links    = 3,/*TBD*/

    /*BR/EDR scan settings */
    .br_edr_scan_cfg =
    {
        .inquiry_scan_type     = BTM_SCAN_TYPE_INTERLACED,
        .inquiry_scan_interval = WICED_BT_CFG_DEFAULT_INQUIRY_SCAN_INTERVAL,
        .inquiry_scan_window   = WICED_BT_CFG_DEFAULT_INQUIRY_SCAN_WINDOW,

        .page_scan_type        = BTM_SCAN_TYPE_INTERLACED,
        .page_scan_interval    = WICED_BT_CFG_DEFAULT_PAGE_SCAN_INTERVAL,
        .page_scan_window      = WICED_BT_CFG_DEFAULT_PAGE_SCAN_WINDOW,
    },

    /*BLE scan settings */
    .ble_scan_cfg =
    {
        .scan_mode = BTM_BLE_SCAN_MODE_NONE,
    },

    .l2cap_application =
    {
        .max_links       = 2,
        .max_psm         = 0, /*TBD*/
        .max_channels    = 6,
        /* LE L2cap connection-oriented channels configuration */
        .max_le_psm      = 0, /**< Maximum number of application-managed LE PSMs */
        .max_le_channels = 0, /**< Maximum number of application-managed LE channels */
        .max_le_l2cap_fixed_channels = 0,/**< Maximum l2cap fixed channels */
    },

    .avdt_cfg =
    {
        .max_links = 1,
    },

    /* Audio/Video Remote Control configuration */
    .avrc_cfg =
    {
        .roles     = AVRC_CONN_ACCEPTOR, /**< Mask of local roles supported (AVRC_CONN_INITIATOR|AVRC_CONN_ACCEPTOR) */
        .max_links = 1  /**< Maximum simultaneous remote control links */
    },

    .max_number_of_buffer_pools = WICED_BT_CFG_NUM_BUF_POOLS,

    .addr_resolution_db_size = 3,

};

const wiced_bt_cfg_buf_pool_t wiced_bt_audio_cfg_buf_pools[ WICED_BT_CFG_NUM_BUF_POOLS ] =
{
    /*  { buf_size, buf_count } */
    { 64,       20   },      /* Small Buffer Pool */
    { 360,      20   },      /* Medium Buffer Pool (used for HCI & RFCOMM control messages, min recommended size is 360) */
    { 660,      60   },      /* Large Buffer Pool  (used for HCI ACL messages) */
    { 1024,     60   },      /* Extra Large Buffer Pool - Used for avdt media packets and miscellaneous (if not needed, set buf_count to 0) */
};

wiced_bt_a2dp_codec_info_t bt_audio_codec_capabilities =
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
            BT_AUDIO_A2DP_SBC_MAX_BITPOOL,         /* max_bitpool for high quality audio */
            A2D_SBC_IE_MIN_BITPOOL                                  /* min_bitpool */
        }
    }
};

wiced_bt_a2dp_config_data_t bt_audio_codec_config =
{
    .feature_mask = WICED_BT_A2DP_SINK_FEAT_INVALID,
    .codec_capabilities =
    {
        .count = 1,
        .info = &bt_audio_codec_capabilities,
    }
};

/*BD address of the connected peer device*/
wiced_bt_device_address_t remote_address = {0};
wiced_bt_device_address_t local_address = {0x81, 0x34, 0x09, 0xAD, 0xCD, 0xAC};
wiced_bt_a2dp_codec_info_t  codec_config;
#ifdef USE_MEM_POOL
bt_buffer_pool_handle_t  mem_pool;
#endif

/*
const command_t bt_audio_command_table[] = {
    BT_AUDIO_CONSOLE_COMMANDS
    AUDIO_PLL_CONSOLE_COMMANDS
    CMD_TABLE_END
};*/

static char bt_audio_command_buffer[ BT_AUDIO_COMMAND_LENGTH ];
static char bt_audio_command_history_buffer[ BT_AUDIO_COMMAND_LENGTH * BT_AUDIO_CONSOLE_COMMAND_HISTORY_LENGTH ];

/* BD address of the connected peer device */
bt_audio_a2dp_context_data_t a2dp_context =
{
    .sink_connected = WICED_FALSE,
    .rc_connected = WICED_FALSE,
    .peer_address = {0,},
    .peer_features = 0,
    .audio_config = { .volume = BT_AUDIO_DEFAULT_VOLUME, }
};

//static wiced_bool_t a2dp_media_state = WICED_FALSE;
static wiced_bool_t audio_state = WICED_FALSE;
static wiced_bool_t is_connected = WICED_FALSE;

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_result_t bt_audio_context_init(void)
{
    wiced_result_t result;
#ifdef USE_MEM_POOL
    result = bt_buffer_pool_init(&mem_pool, BT_AUDIO_MEM_POOL_DEFAULT_BUFFER_COUNT, BT_AUDIO_MEM_POOL_DEFAULT_BUFFER_SIZE);
    if (result != WICED_SUCCESS)
    {
        return result;
    }
#endif
    result = bt_audio_decoder_context_init();
    if (result != WICED_SUCCESS)
    {
        return result;
    }

    result = bt_audio_init_player();

    return result;
}

void application_start()
{
    wiced_result_t result;

    wiced_core_init();

    platform_init_audio();

    WPRINT_APP_INFO(("Starting Bluetooth...\n"));

    result = bt_audio_context_init();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("Failed to initialize Bluetooth App Context variables... aborting.\n"));
        return;
    }

    //wiced_rtos_create_thread_with_stack( &player_thread, BT_AUDIO_PLAYER_TASK_PRIORITY, "PLAYER TASK", bt_audio_player_task, (UINT16 *)player_stack, PLAYER_STACK_SIZE, NULL );
    //wiced_rtos_create_thread_with_stack( &decoder_thread, BT_AUDIO_DECODER_TASK_PRIORITY, "DECODER TASK", bt_audio_decoder_task, (UINT16 *)decoder_stack, DECODER_STACK_SIZE, NULL );
    wiced_rtos_create_thread(&player_thread, BT_AUDIO_PLAYER_TASK_PRIORITY, "PLAYER TASK", bt_audio_player_task, PLAYER_STACK_SIZE, NULL);
    wiced_rtos_create_thread(&decoder_thread, BT_AUDIO_DECODER_TASK_PRIORITY, "DECODER TASK", bt_audio_decoder_task, DECODER_STACK_SIZE, NULL);
#ifdef BT_AVK_SOFT_IRQ___
    soft_hw_init();
    wiced_rtos_create_thread(&hw_simulator.tskID, BT_SOFT_DMA_IRQ_PRI, "HW SIM", bt_hw_simulate_task, HW_SIM_STACK_SIZE, NULL);
#endif

    /* Initialize BT NVRAM with APP_OFFSET for Bluetooth */
    result = bt_audio_nv_init();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("bt_audio_nv_init() failed !!\r\n"));
        return;
    }

#ifdef WICED_DCT_INCLUDE_BT_CONFIG
    {
        /* Configure the Device Name and Class of Device from the DCT */
        platform_dct_bt_config_t *dct_bt_config;
        // Read config
        wiced_dct_read_lock((void **) &dct_bt_config, WICED_TRUE, DCT_BT_CONFIG_SECTION, 0, sizeof(platform_dct_bt_config_t));
        WPRINT_APP_INFO(("WICED DCT BT NAME: %s \r\n", dct_bt_config->bluetooth_device_name));
        strlcpy((char *)bluetooth_device_name, (char *)dct_bt_config->bluetooth_device_name, sizeof(bluetooth_device_name));
        wiced_bt_audio_cfg_settings.device_name = bluetooth_device_name;
        WPRINT_APP_INFO(("WICED DCT BT DEVICE CLASS : %02x %02x %02x\r\n", dct_bt_config->bluetooth_device_class[0],
                         dct_bt_config->bluetooth_device_class[1], dct_bt_config->bluetooth_device_class[2]));
        memcpy(wiced_bt_audio_cfg_settings.device_class, dct_bt_config->bluetooth_device_class, sizeof(dct_bt_config->bluetooth_device_class));
        wiced_dct_read_unlock((void *) dct_bt_config, WICED_TRUE);
    }
#endif
    WPRINT_APP_INFO(("WICED DCT name %s\r\n", wiced_bt_audio_cfg_settings.device_name));

    /* Initialize BT stack */
    result = wiced_bt_stack_init(bt_audio_management_callback, &wiced_bt_audio_cfg_settings, wiced_bt_audio_cfg_buf_pools);
    WPRINT_APP_INFO(("wiced_add_bluetooth_dm result: 0x%x, \n", result));
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("Failed to initialize Bluetooth\n"));
        return;
    }

    /* Command Line initialization */
    result = command_console_init(STDIO_UART, sizeof(bt_audio_command_buffer), bt_audio_command_buffer,
                                  BT_AUDIO_CONSOLE_COMMAND_HISTORY_LENGTH, bt_audio_command_history_buffer, " ");

    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("Error starting the command console\r\n"));
    }
    //console_add_cmd_table( bt_audio_command_table );
}

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
    //return NULL;
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
    //return NULL;
}

static void bt_audio_remote_control_connection_state_cback(wiced_bt_device_address_t remote_addr, wiced_result_t status,
        wiced_bt_remote_control_connection_state_t connection_state, uint32_t peer_features)
{
    wiced_bt_device_address_t null_addr = {0,};

    if (status != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("%s: something went wrong, status = %d \n", __func__, status));
    }

    if (connection_state == REMOTE_CONTROL_CONNECTED)
    {
        if (!memcmp(a2dp_context.peer_address, null_addr, sizeof(wiced_bt_device_address_t)))
        {
            memcpy(a2dp_context.peer_address, remote_addr, sizeof(wiced_bt_device_address_t)) ;
        }
        else
        {
            wiced_assert("peer bd adress for a2dp and avrcp do not match!\n", (!memcmp(remote_addr, a2dp_context.peer_address, sizeof(wiced_bt_device_address_t))));
            if (memcmp(remote_addr, a2dp_context.peer_address, sizeof(wiced_bt_device_address_t)))
            {
                return;
            }
        }
        a2dp_context.peer_features = peer_features;
        a2dp_context.rc_connected = WICED_TRUE;
        WPRINT_APP_INFO(("%s: AVRCP Connected \n", __func__));
    }
    else /* REMOTE_CONTROL_DISCONNECTED */
    {
        memset(a2dp_context.peer_address, 0, sizeof(wiced_bt_device_address_t));
        a2dp_context.peer_features = 0;
        a2dp_context.rc_connected = WICED_FALSE;
    }
}

static void bt_audio_remote_control_cmd_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_command_t *avrc_cmd)
{
    wiced_result_t result;

    if (memcmp(remote_addr, a2dp_context.peer_address, sizeof(wiced_bt_device_address_t)))
    {
        WPRINT_APP_INFO(("%s: bt addresses don't match\n", __func__));
        return;
    }

    switch (avrc_cmd->pdu)
    {
    case AVRC_PDU_SET_ABSOLUTE_VOLUME:
        a2dp_context.audio_config.volume = avrc_cmd->volume.volume;
        result = bt_audio_update_player_volume(a2dp_context.audio_config.volume);
        WPRINT_APP_INFO(("remote_control_cmd_cback AVRC_PDU_SET_ABSOLUTE_VOLUME vol:%d, set volume result=%d\n", avrc_cmd->volume.volume, result));
        break;

    default:
        break;
    }
}

/** Response callback from peer device for AVRCP commands */
static void bt_audio_remote_control_rsp_cback(wiced_bt_device_address_t remote_addr, wiced_bt_avrc_response_t *avrc_rsp)
{
    if (memcmp(remote_addr, a2dp_context.peer_address, sizeof(wiced_bt_device_address_t)))
    {
        WPRINT_APP_INFO(("%s: bt addresses don't match\n", __func__));
        return;
    }

    WPRINT_APP_DEBUG(("[REMOTE-CONTROL]: %s pdu [%d] status [%d]\n", __func__, avrc_rsp->pdu, avrc_rsp->rsp.status));
    switch (avrc_rsp->pdu)
    {
    case AVRC_PDU_GET_PLAY_STATUS:
    {
        wiced_bt_avrc_get_play_status_rsp_t *play_status = (wiced_bt_avrc_get_play_status_rsp_t *)avrc_rsp;;

        WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s GET PLAY STATUS: play status: %s song_pos [%d] song_len [%d]\n", __func__,
                         remote_control_play_status_str(play_status->play_status), (unsigned int)play_status->song_pos, (unsigned int)play_status->song_len));

        if (play_status->play_status == AVRC_PLAYSTATE_PLAYING)
            audio_state = WICED_TRUE;
        else
            audio_state = WICED_FALSE;
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
            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s ELEMENT ATTR RSP: Attr-ID [%d] strName [%s]\n", __func__,
                             (unsigned int)elements->p_attrs[localAttr].attr_id, strName));
            memset(strName, 0, sizeof(strName));
            localAttr++;
        }
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
            wiced_bt_remote_control_get_play_status_cmd(a2dp_context.peer_address);
        }
        break;
        case AVRC_EVT_TRACK_CHANGE:
        {
            wiced_bt_avrc_uid_t element_id = {0};
            uint32_t attrs[] = { AVRC_MEDIA_ATTR_ID_TITLE, AVRC_MEDIA_ATTR_ID_ARTIST, AVRC_MEDIA_ATTR_ID_ALBUM, AVRC_MEDIA_ATTR_ID_NUM_TRACKS,
                                 AVRC_MEDIA_ATTR_ID_TRACK_NUM, AVRC_MEDIA_ATTR_ID_PLAYING_TIME, AVRC_MEDIA_ATTR_ID_GENRE
                               };
            uint8_t num_attr = 7;

            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s REG_NOTIF: AVRC_EVT_TRACK_CHANGE\n", __func__));
            wiced_bt_remote_control_get_element_attr_cmd(a2dp_context.peer_address, element_id, num_attr, attrs);
        }
        break;
        case AVRC_EVT_NOW_PLAYING_CHANGE:
        {
            wiced_bt_avrc_uid_t element_id = {0};
            uint32_t attrs = AVRC_MEDIA_ATTR_ID_TITLE;
            uint8_t num_attr = 1;

            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s REG_NOTIF: AVRC_EVT_NOW_PLAYING_CHANGE\n", __func__));
            wiced_bt_remote_control_get_element_attr_cmd(a2dp_context.peer_address, element_id, num_attr, &attrs);
        }
        break;
        case AVRC_EVT_TRACK_REACHED_END:
        case AVRC_EVT_TRACK_REACHED_START:
        case AVRC_EVT_PLAY_POS_CHANGED:
        {
            WPRINT_APP_DEBUG(("[REMOTE-CONTROL]:%s event-id [%s] : \n", __func__,
                              remote_control_event_id_str(reg_notif->event_id)));
        }
        break;
        case AVRC_EVT_VOLUME_CHANGE:
        {
            WPRINT_APP_INFO(("[REMOTE-CONTROL]:%s event-id [%s] : volume: %d\n", __func__,
                             remote_control_event_id_str(reg_notif->event_id), reg_notif->param.volume));

            a2dp_context.audio_config.volume = reg_notif->param.volume;
        }
        break;
        }
    }
    break;
    }
}

wiced_result_t bt_audio_a2dp_send_passthru_command(uint8_t cmd)
{
    wiced_result_t result;
    result = wiced_bt_remote_control_send_pass_through_cmd(a2dp_context.peer_address, cmd, AVRC_STATE_PRESS, 0, NULL);
    if (result != WICED_SUCCESS)
        return result;
    wiced_rtos_delay_milliseconds(50);
    result = wiced_bt_remote_control_send_pass_through_cmd(a2dp_context.peer_address, cmd, AVRC_STATE_RELEASE, 0, NULL);
    return result;
}

static wiced_result_t bt_audio_a2dp_update_volume(uint8_t vol_level)
{
    wiced_result_t result;

    result = bt_audio_update_player_volume(vol_level);
    /* send volume to peer */
    if (result != WICED_SUCCESS)
    {
        return result;
    }

    result = wiced_bt_remote_control_set_volume_cmd(a2dp_context.peer_address, vol_level);
    return result;
}

/* Take the input from console and control the music */
static int bt_audio_console_command(int arg_count, const char *command[])
{
    const char *command1 = command[0];
    wiced_result_t result;
    uint8_t vol_level = a2dp_context.audio_config.volume;
    if (is_connected)
    {
        WPRINT_APP_INFO(("Received command: %s\n", command[0]));
        if (strcmp(command1, "play") == 0)
        {
            if (audio_state == WICED_FALSE)
            {
                WPRINT_APP_INFO(("[BT-A2DP] play_state: 0 sending PLAY\n"));
                result = bt_audio_a2dp_send_passthru_command(AVRC_ID_PLAY);
                if (result == WICED_SUCCESS)
                    audio_state = WICED_TRUE;
            }
        }
        else if (strcmp(command1, "pause") == 0)
        {
            if (audio_state == WICED_TRUE)
            {
                WPRINT_APP_INFO(("[BT-A2DP] play_state: 1 sending PAUSE\n"));
                result = bt_audio_a2dp_send_passthru_command(AVRC_ID_PAUSE);
                if (result == WICED_SUCCESS)
                    audio_state = WICED_FALSE;
            }
        }
        else if (strcmp(command1, "stop") == 0)
        {
            WPRINT_APP_INFO(("Stop Success\n"));
            bt_audio_a2dp_send_passthru_command(AVRC_ID_STOP);
        }
        else if (strcmp(command1, "next") == 0)
        {
            WPRINT_APP_INFO(("[BT-A2DP]: sending SKIP_FORWARD\n"));
            bt_audio_a2dp_send_passthru_command(AVRC_ID_FORWARD);
        }
        else if (strcmp(command1, "previous") == 0)
        {
            WPRINT_APP_INFO(("[BT-A2DP]: sending SKIP_BACKWARD\n"));
            bt_audio_a2dp_send_passthru_command(AVRC_ID_BACKWARD);
        }
        else if (strcmp(command1, "volumeup") == 0)
        {
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

            bt_audio_a2dp_update_volume(vol_level);
            a2dp_context.audio_config.volume = vol_level;
        }
        else if (strcmp(command1, "volumedown") == 0)
        {
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

            bt_audio_a2dp_update_volume(vol_level);
            a2dp_context.audio_config.volume = vol_level;
        }
        return ERR_CMD_OK;
    }
    else
    {
        WPRINT_APP_INFO(("Command Not Received, Device not connected\n"));
        return ERR_CMD_OK;
    }
}

/*Utility Functions */
wiced_result_t bt_audio_get_config_from_cie(wiced_bt_a2dp_codec_info_t *p_codec_config, bt_audio_config_t *p_audio_config)
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

        p_audio_config->frame_size = (p_audio_config->channels * p_audio_config->bits_per_sample) / 8;

        result = WICED_SUCCESS;
    }
    return result;
}

/* DM Functions */
wiced_result_t bt_audio_write_eir(uint8_t *device_name)
{
    uint8_t eir_cfg[EIR_DATA_LENGTH] = {0};
    uint8_t *p = eir_cfg;
    uint8_t name_len = strlen((char *)device_name);

    *p++ = (uint8_t)(name_len + 1);               /* Length */
    *p++ = (uint8_t)EIR_COMPLETE_LOCAL_NAME_TYPE; /* EIR Data Type */
    memcpy(p, device_name, name_len);     /* Name string */
    p += name_len;

    *p++ = sizeof(uuid_list) + 1;
    *p++ = (uint8_t) EIR_COMPLETE_16BITS_UUID_TYPE;
    memcpy(p, uuid_list, sizeof(uuid_list));

    return wiced_bt_dev_write_eir(eir_cfg, EIR_DATA_LENGTH);
}

static wiced_bt_dev_status_t bt_audio_management_callback(wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data)
{
    wiced_bt_dev_status_t status = WICED_BT_SUCCESS;
    wiced_bt_device_address_t bda;
    wiced_result_t result = WICED_ERROR;

    switch (event)
    {
    case BTM_ENABLED_EVT:
    {
        /* Bluetooth controller and host stack enabled */
        WPRINT_APP_INFO(("bt_audio_management_callback: Bluetooth enabled (%s)\n", ((p_event_data->enabled.status == WICED_BT_SUCCESS) ? "success" : "failure")));

        if (p_event_data->enabled.status == WICED_BT_SUCCESS)
        {
#ifdef WICED_DCT_INCLUDE_BT_CONFIG
            /* Configure the Device Name and Class of Device from the DCT */
            platform_dct_bt_config_t *dct_bt_config;
            /* Read config */
            wiced_dct_read_lock((void **) &dct_bt_config, WICED_TRUE, DCT_BT_CONFIG_SECTION, 0, sizeof(platform_dct_bt_config_t));
            WPRINT_APP_INFO(("WICED DCT BT ADDR 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x \r\n",
                             dct_bt_config->bluetooth_device_address[0], dct_bt_config->bluetooth_device_address[1],
                             dct_bt_config->bluetooth_device_address[2], dct_bt_config->bluetooth_device_address[3],
                             dct_bt_config->bluetooth_device_address[4], dct_bt_config->bluetooth_device_address[5]));
            wiced_bt_set_local_bdaddr(dct_bt_config->bluetooth_device_address);
            WPRINT_APP_INFO(("bt_audio_management_callback:wiced_bt_dev_write_local_addr result = 0x%x\n", (unsigned int)result));
            result = bt_audio_write_eir(dct_bt_config->bluetooth_device_name);
            wiced_dct_read_unlock((void *) dct_bt_config, WICED_TRUE);
#else
            result = bt_audio_write_eir((uint8_t *)BLUETOOTH_DEVICE_NAME);
#endif
            WPRINT_APP_INFO(("bt_audio_management_callback: wiced_bt_dev_write_eir result = 0x%x\n", (unsigned int)result));

            wiced_bt_dev_read_local_addr(bda);
            WPRINT_APP_INFO(("bt_audio_management_callback:Local Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n", bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));
        }

        /**
         * TODO Remove this, and make changes in the embedded app to solicit this information.
         */
#ifdef USE_WICED_HCI
        {
            uint8_t count = 0;
            bt_audio_paired_device_info_t out_device;
            wiced_bt_device_link_keys_t link_keys;

            for (count = 0; count < BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES; count++)
            {
                if (bt_audio_nv_get_device_info_by_index(count, &out_device) == WICED_BADARG)
                {
                    WPRINT_APP_INFO(("%s no more entries in NVRAM\n", __func__));
                    break;
                }
                else
                {
                    WPRINT_APP_INFO(("%s Bluetooth address of the paired device extracted \n", __func__));
                    WPRINT_APP_INFO(("from NVRAM: %x %x %x %x %x %x\n", out_device.device_link.bd_addr[0], out_device.device_link.bd_addr[1],
                                     out_device.device_link.bd_addr[2], out_device.device_link.bd_addr[3], out_device.device_link.bd_addr[4], out_device.device_link.bd_addr[5]));
                    memcpy(&link_keys, &out_device.device_link, sizeof(out_device.device_link));
                    wiced_bt_dev_push_nvram_data(&link_keys);
                }
            }
        }
#endif

        result = (wiced_result_t)wiced_bt_sdp_db_init((uint8_t *)sdp_database, sizeof(sdp_database));
        WPRINT_APP_INFO(("bt_audio_management_callback: wiced_bt_sdp_db_init result (bool) = 0x%x\n", (unsigned int)result));

        result = wiced_bt_dev_set_discoverability(BTM_GENERAL_DISCOVERABLE, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.inquiry_scan_window, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.inquiry_scan_interval);
        WPRINT_APP_INFO(("bt_audio_management_callback: discoverability result = 0x%x\n", (unsigned int)result));

        result = wiced_bt_dev_set_connectability(BTM_CONNECTABLE, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.page_scan_window, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.page_scan_interval);
        WPRINT_APP_INFO(("bt_audio_management_callback:connectability result = 0x%x\n", (unsigned int)result));

        result = wiced_bt_dev_register_connection_status_change(bt_audio_connection_status_callback);
        WPRINT_APP_INFO(("bt_audio_management_callback registering acl_change callback result: 0x%x, cb = 0x%x\n", result, (unsigned int) bt_audio_connection_status_callback));

        /* Sink and Remote Control */
        result = wiced_bt_a2dp_sink_init(&bt_audio_codec_config, bt_audio_sink_control_cb, bt_audio_sink_data_cb);
        WPRINT_APP_INFO(("bt_audio_management_callback:wiced_bt_a2dp_sink_init result = 0x%x\n", (unsigned int)result));
        if (result == WICED_SUCCESS)
            result = wiced_bt_remote_control_init(LOCAL_FEATURES, bt_audio_remote_control_connection_state_cback,
                                                  bt_audio_remote_control_cmd_cback, bt_audio_remote_control_rsp_cback);
        WPRINT_APP_INFO(("[BT-A2DP]: Service Started\n"));
    }
    break;

    case BTM_PAIRING_IO_CAPABILITIES_BR_EDR_REQUEST_EVT:
        WPRINT_APP_INFO(("bt_audio_management_callback: IO capabilities BR/EDR request: \n"));
        p_event_data->pairing_io_capabilities_br_edr_request.local_io_cap = BTM_IO_CAPABILITIES_NONE;
        p_event_data->pairing_io_capabilities_br_edr_request.auth_req = BTM_AUTH_SINGLE_PROFILE_GENERAL_BONDING_NO;
        break;

    case BTM_USER_CONFIRMATION_REQUEST_EVT:
        WPRINT_APP_INFO(("bt_audio_management_callback: User confirmation request: \n"));
        wiced_bt_dev_confirm_req_reply(WICED_BT_SUCCESS, p_event_data->user_confirmation_request.bd_addr);
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_UPDATE_EVT:
        WPRINT_APP_INFO(("bt_audio_management_callback: Link key update evt\n"));
        bt_audio_nv_update_device_link_key(&p_event_data->paired_device_link_keys_update);
        break;

    case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
    {
        bt_audio_paired_device_info_t out_device;
        WPRINT_APP_INFO(("bt_audio_management_callback: Link key request evt\n"));
        if (WICED_SUCCESS == bt_audio_nv_get_device_info_by_addr(&p_event_data->paired_device_link_keys_request.bd_addr, &out_device))
        {
            memcpy(&p_event_data->paired_device_link_keys_request.key_data, &out_device.device_link.key_data, sizeof(out_device.device_link.key_data));
        }
        else
        {
            status = WICED_BT_ERROR;
        }
    }
    break;

    case BTM_LOCAL_IDENTITY_KEYS_UPDATE_EVT:
        WPRINT_APP_INFO(("bt_audio_management_callback: Local ID key update evt\n"));
        bt_audio_nv_update_local_id_keys(&p_event_data->local_identity_keys_update);
        break;

    case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:

#ifndef USE_WICED_HCI
        if (WICED_SUCCESS != bt_audio_nv_get_local_id_keys(&p_event_data->local_identity_keys_request))
            status = WICED_BT_ERROR;
#endif
        break;

    case BTM_SECURITY_REQUEST_EVT:
        WPRINT_APP_INFO(("bt_audio_management_callback: Security reqeust\n"));
        break;

    case BTM_PAIRING_COMPLETE_EVT:
        WPRINT_APP_INFO(("bt_audio_management_callback: Pairing complete %i.\n", p_event_data->pairing_complete.pairing_complete_info.ble.status));
        break;

    default:
        WPRINT_APP_INFO(("bt_audio_management_callback: Unhandled Bluetooth Management Event: 0x%x\n", event));
        status = WICED_BT_ERROR;
        break;
    }

    return (status);
}

static void bt_audio_connection_status_callback(wiced_bt_device_address_t bd_addr, uint8_t *p_features, wiced_bool_t is_connected, uint16_t handle, wiced_bt_transport_t transport, uint8_t reason)
{
    WPRINT_APP_INFO(("[%s] Connection status callback is_connected:%d reason:%x\n", __func__, is_connected, reason));

    if (is_connected)
    {
        wiced_bt_dev_set_discoverability(BTM_NON_DISCOVERABLE, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.inquiry_scan_window, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.inquiry_scan_interval);
    }
    else
    {
        wiced_bt_dev_set_discoverability(BTM_GENERAL_DISCOVERABLE, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.inquiry_scan_window, wiced_bt_audio_cfg_settings.br_edr_scan_cfg.inquiry_scan_interval);
    }
}

/*Audio Sink Functions*/
static void bt_audio_sink_control_cb(wiced_bt_a2dp_sink_event_t event, wiced_bt_a2dp_sink_event_data_t *p_data)
{
    switch (event)
    {
    case WICED_BT_A2DP_SINK_CONNECT_EVT:
        WPRINT_APP_INFO(("bt_audio_sink_control_cb:CONNECT EVENT \nstatus = %d\n", p_data->connect.result));

        if (p_data->connect.result == WICED_SUCCESS)
        {
            memcpy(remote_address, p_data->connect.bd_addr, sizeof(wiced_bt_device_address_t));
            is_connected = WICED_TRUE;
            WPRINT_APP_INFO(("bt_audio_sink_control_cb: Remote Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                             remote_address[0], remote_address[1], remote_address[2], remote_address[3], remote_address[4], remote_address[5]));
        }
        break;

    case WICED_BT_A2DP_SINK_DISCONNECT_EVT:
    {
        wiced_bt_device_address_t bda;

        bt_audio_stop_player(); /* stop if player was running, needed here when src goes out of range or BT is turned off at src and it doesn't send suspend before disconnecting */
        memcpy(bda, p_data->disconnect.bd_addr, sizeof(wiced_bt_device_address_t));
        WPRINT_APP_INFO(("bt_audio_sink_control_cb:DISCONNECTED EVENT \nreason = %d \nRemote Bluetooth Address: [%02X:%02X:%02X:%02X:%02X:%02X]\n",
                         p_data->disconnect.result, bda[0], bda[1], bda[2], bda[3], bda[4], bda[5]));
        memset(remote_address, 0, sizeof(wiced_bt_device_address_t));
        audio_state = WICED_FALSE;
#ifdef USE_MEM_POOL
        bt_buffer_pool_print_debug_info(mem_pool);
#endif
    }
    break;

    case WICED_BT_A2DP_SINK_START_EVT:
    {
        /* WPRINT_APP_INFO( ("bt_audio_sink_control_cb:STREAM START EVENT \nstatus = %d \n", p_data->start.result) ); */
        /* Configure codec and player(DAC) with the configuration received thru Codec Config event from the peer */
        bt_audio_configure_decoder(&codec_config);
        bt_audio_get_config_from_cie(&codec_config, &a2dp_context.audio_config);
        bt_audio_configure_player(&a2dp_context.audio_config);
        //a2dp_media_state = WICED_TRUE;
        audio_state = WICED_TRUE;
#ifdef USE_WICED_HCI
        wiced_bt_a2dp_sink_start_rsp(remote_address, A2D_SUCCESS);
#endif
    }
    break;

    case WICED_BT_A2DP_SINK_SUSPEND_EVT:
        WPRINT_APP_INFO(("bt_audio_sink_control_cb:STREAM SUSPEND EVENT \nstatus = %d \n", p_data->start.result));
        /*Stop the player task and reset the decoder configuration*/
        bt_audio_stop_player();
        bt_audio_reset_decoder_config();
        audio_state = WICED_FALSE;
#ifdef USE_MEM_POOL
        bt_buffer_pool_print_debug_info(mem_pool);
#endif
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

void bt_audio_sink_data_cb(wiced_bt_a2dp_sink_codec_t codec_type, wiced_bt_a2dp_sink_audio_data_t *p_audio_data)
{
    bt_audio_codec_data_t *audio = NULL;
    uint8_t *in_audio;
    uint16_t in_length;

    p_audio_data->p_pkt->len--;
    p_audio_data->p_pkt->offset++;
    in_length = p_audio_data->p_pkt->len;

#ifdef USE_MEM_POOL
    audio = bt_buffer_pool_allocate_buffer(mem_pool);
#else
    audio = rt_malloc(sizeof(bt_audio_codec_data_t) + in_length);
    bt_buffer_mem.decoder_buffer++;
    bt_buffer_mem.decoder_buffer_plus++;
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
    bt_audio_write_to_decoder_queue(audio);
}

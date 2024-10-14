/*
 * $ Copyright Cypress Semiconductor $
 */

/*
 * @file:dual_a2dp_hfp_audio.h
 *
 * This file contains definitions required for implementing an
 * application framework that can handle multiple profiles
 * simultaneously or according to policy definitions.
 */

#pragma once

#include "platform.h"
#include "bluetooth_audio.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                     Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

/* Our table is based on the levels of priority. with the current design, Multiple services can have same priority.
 * This macro defines the total number of priority levels we have and not the total number of services.
 * Though, it is quite possible that total number of services is same as priority levels.
 */
#define WICED_MAXIMUM_PRIORITY_LEVELS              4

#define WICED_APP_REBOOT_DELAY_MSECS               (500)

/******************************************************
 *                   Enumerations
 ******************************************************/

typedef enum
{
    NO_ACTION = 0x0,
    /*
     * ACTION_BT_CONNECT_TO_LAST_CONN_DEVICE
     * ACTION_BT_DIAL_LAST_NUMBER
     */
    ACTION_MULTI_FUNCTION_SHORT_RELEASE = 0x1,
    /*
     * ACTION_BT_SET_INQUIRY_SCAN_MODE
     * ACTION_BT_START_VOICE_DIAL
     */
    ACTION_MULTI_FUNCTION_LONG_RELEASE,

    /* Multiple HFP actions mapped:
     * ACTION_HFP_ACCEPT_CALL
     * ACTION_HFP_REJECT_CALL
     * ACTION_HFP_REJECT_OUTGOING_CALL
     * ACTION_HFP_HANGUP_CALL
     * ACTION_HFP_HOLD_CALL
     * ACTION_HFP_ACCEPT_INCOMING_END_ACTIVE
     * ACTION_HFP_ACCEPT_INCOMING_HOLD_ACTIVE
     * ACTION_HFP_REJECT_INCOMING_WHILE_ACTIVE
     * ACTION_HFP_END_ACTIVE_RELEASE_HELD_CALL
     * ACTION_HFP_START_THREE_WAY_CALL
     */
    /* Play or Pause on the remote audio source[With Bluetooth]  */
    ACTION_PAUSE_PLAY,
    /* Sends "Stop" music(a remote control command) command on the remote audio source[With Bluetooth]  */
    ACTION_STOP,
    /* Skip Forward the Audio/Music on the remote audio source[With Bluetooth]  */
    ACTION_FORWARD,
    /* Stop backwards Audio/Music on the remote audio source[With Bluetooth]  */
    ACTION_BACKWARD,
    /* Send Volume UP command on the remote audio source[With Bluetooth]  */
    ACTION_VOLUME_UP,
    /* Send Volume DOWN on the remote audio source[With Bluetooth]  */
    ACTION_VOLUME_DOWN,

    ACTION_FACTORY_RESET,

} app_service_action_t;


/**
 * Service type
 * The service type should always be 1 left shifted by a predefined number, as the
 * service type also helps decide pre-emption policies.
 */
typedef enum
{
    SERVICE_NONE        = (0),
    SERVICE_BT_A2DP     = (1),           //!< SERVICE_BT_A2DP
    SERVICE_BT_HFP      = (2),           //!< SERVICE_BT_HFP
} service_type_t;

typedef enum
{
    SERVICE_GROUP_BLUETOOTH = 0,
} service_group_type_t;

/**
 * The states of a service are defined here.
 */
typedef enum
{
    /* Service has been disabled or disactivated either by application[user] or got some events
     * from remote device which made the service disabled or It has not been started at all */
    SERVICE_DISABLED = 0,
    /* Service is idle when it is connected but not consuming any system resources, same as ENABLED */
    SERVICE_IDLE,//!< SERVICE_IDLE
    /* same as IDLE. Service is enabled but not using any critical system resource */
    SERVICE_ENABLED = SERVICE_IDLE,
    /* Service is pending getting enabled due to a resource lock elsewhere. */
    SERVICE_PENDING,
    /* Service is playing; owner of audio resources */
    SERVICE_PLAYING_AUDIO, //!< SERVICE_PLAYING
    /* Service has been paused, may or may not release audio resources */
    SERVICE_PAUSED_AUDIO,  //!< SERVICE_PAUSED
    /* Service has been stopped(playing), may or may not release audio resources */
    SERVICE_STOPPED_AUDIO, //!< SERVICE_STOPPED

    /* Service has been 'prevented' by a higher-priority service. Future requests from
     * this service to get audio resources must be rejected by the library until application
     * 'allow' it back
     */
    SERVICE_PREVENTED,//!< SERVICE_PREVENTED

    /* Service has been preempted, i.e. the service was active when it got 'prevented' by
     * a higher priority service */
    SERVICE_PREEMPTED,//!< SERVICE_PREEMPTED

} service_state_t;

/**
 * Service priorities are defined here.
 */
typedef enum
{
    SERVICE_DAEMON_PRIORITY = 0,                //!< SERVICE_DAEMON_PRIORITY
    SERVICE_BT_A2DP_PRIORITY,                   //!< SERVICE_BT_A2DP_PRIORITY
    SERVICE_BT_HFP_PRIORITY,                    //!< SERVICE_BT_HFP_PRIORITY
} service_priority_t;

/**
 * Specific service events.
 * For new services, append to the enum.
 */
typedef enum
{
    WICED_BT_RECONNECT_ON_LINK_LOSS = (1 << 0),
    WICED_BT_CONNECT_A2DP           = (1 << 1),
    WICED_BT_DISCONNECT_A2DP        = (1 << 2),
    WICED_BT_CONNECT_AVRCP          = (1 << 3),
    WICED_BT_DISCONNECT_AVRCP       = (1 << 4),
    WICED_BT_CONNECT_HFP            = (1 << 5),
    WICED_BT_DISCONNECT_HFP         = (1 << 6),
} app_event_t ;

/******************************************************
 *                 Type Definitions
 ******************************************************/

/**
 * API Typedefs.
 */
typedef struct wiced_app_service wiced_app_service_t;

typedef void (*WICED_APP_INIT_SERVICE)(void);
typedef void (*WICED_APP_DEINIT_SERVICE)(void);
typedef void (*WICED_APP_CONNECT_SERVICE)(void);
typedef void (*WICED_APP_DISCONNECT_SERVICE)(void);

typedef void (*WICED_APP_PREVENT_SERVICE)(wiced_app_service_t *service);
typedef void (*WICED_APP_ALLOW_SERVICE)(wiced_app_service_t *service);
typedef void (*WICED_APP_FORCE_PLAYBACK)(void);
typedef void (*WICED_APP_FORCE_STOP_PLAYBACK)(void);
typedef wiced_result_t (*WICED_APP_BUTTON_HANDLER)(app_service_action_t action);

/******************************************************
 *                    Structures
 ******************************************************/

/**
 * Service Struct: Definition
 */
struct wiced_app_service
{
    /* Priority of the service */
    int                             priority;
    /* Index */
    int                             type;

    /* State of the service */
    volatile service_state_t        state;


    /* Function pointers for controlling the state of the service */

    /* initialize service function pointer. Typically, will invoke library-specific initialization routines */
    WICED_APP_INIT_SERVICE          init_service;
    /* connect service */
    WICED_APP_CONNECT_SERVICE       connect_service;
    /* dusconncet service */
    WICED_APP_DISCONNECT_SERVICE    disconnect_service;
    /* deinitialize service */
    WICED_APP_DEINIT_SERVICE        deinit_service;

    /* Flag the library to prevent this service from being activated until allow_service is called;
     * Underlying libraries should ensure that if 'prevent_service' has been called and the service
     * is currently active, then it must free up the system resources.
     */
    WICED_APP_PREVENT_SERVICE       prevent_service;

    /* Opposite of prevent_service */
    WICED_APP_ALLOW_SERVICE         allow_service;

    /* Feed Button-events and action to this handler */
    WICED_APP_BUTTON_HANDLER        button_handler;
};

/**
 * Queue element for app framework queue.
 */
typedef struct
{
    app_event_t         event;
    void (*function)(void);
    wiced_bool_t        wait_for_event_complete;
    service_type_t      event_source;
} app_queue_element_t;

/**
 * Single cell of the app service table.
 */
typedef struct _app_service_cell
{
    wiced_app_service_t service;
    struct _app_service_cell *next;
} wiced_app_service_cell_t;

typedef enum
{
    STREAM_IDLE = 0,
    STREAM_PLAYING,
    STREAM_SUSPEND_PENDING,
} stream_state_t;

typedef struct
{
    wiced_bool_t                        sink_connected;
    wiced_bool_t                        rc_connected;
    wiced_bt_device_address_t           avrc_peer_address;
    wiced_bt_device_address_t           a2dp_peer_address;
    uint32_t                            peer_features;
    bt_audio_config_t                   audio_config;
    wiced_bt_a2dp_codec_info_t          decoder_config;
    wiced_bool_t                        in_use_context;
    stream_state_t                      stream_status;
} a2dp_context_data_t;

typedef struct
{
    wiced_mutex_t               lock;
    wiced_thread_t              thread_handle;
    wiced_queue_t               queue;
    wiced_app_service_t        *current_service;
    wiced_bool_t                play_button_state;
    wiced_bool_t                a2dp_media_state;
    a2dp_context_data_t        *a2dp_current_stream;
    a2dp_context_data_t        *a2dp_last_stream;
    wiced_semaphore_t           queue_semaphore;
    uint32_t                    event_mask;
} app_worker_data_t;

typedef wiced_result_t (*button_event_function_t)(void);

typedef struct
{
    /* Logical action-enum */
    app_service_action_t    action;

    /* Primary routine which will be executed to service above action */
    button_event_function_t    primary;
    /* Secondary routine which need to be executed only when either primary routine has not been defined
     * or when Primary routines fails to service the action */
    button_event_function_t    secondary;
} button_action_function_table_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

extern app_worker_data_t app_data;
extern wiced_app_service_cell_t *wiced_service_array[];

/******************************************************
 *               Function Declarations
 ******************************************************/
wiced_app_service_t *get_app_current_service(void);
wiced_result_t app_set_service_state(service_type_t service_type, service_state_t state);
wiced_result_t app_disable_service(service_type_t service_type);
wiced_result_t app_set_current_service(service_type_t service_type);
wiced_result_t app_reset_current_service(service_type_t service_type);

wiced_result_t wiced_app_shutdown_action(void);
void wiced_app_system_reboot(void);
void signal_for_event_semaphore(app_event_t event, wiced_bool_t completed);
#ifdef __cplusplus
} /*extern "C" */
#endif

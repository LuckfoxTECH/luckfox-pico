/*
 * $ Copyright Cypress Semiconductor $
 */

/*
 *@app_button.c : Application's Button-list entries; What button does what...
 *
 */
#include "wiced.h"
#include "dual_a2dp_hfp_audio.h"
#include "wiced_bt_dev.h"
#include "wiced_bt_hfp_hf.h"
#include "bluetooth_dm.h"
#include "bluetooth_a2dp.h"
#include "bluetooth_hfp.h"
#include "hashtable.h"
#include "app_button_interface.h"
#include "button_manager.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define BUTTON_EVENT_ID(app_button, event, state)      ( ((uint32_t)app_button << 24) + ((uint32_t)event<<1) + state )

/******************************************************
 *                    Constants
 ******************************************************/

/* This Application's MMI has been designed to use 6 logical buttons and *should* be one-to-one mapped with physical buttons
 * provided from the Platform. In case, the platform provides lesser number of physical buttons, some of the MMI functionality
 * will get skipped.
 */
#define BUTTON_WORKER_STACK_SIZE            ( 2048 )
#define BUTTON_WORKER_QUEUE_SIZE            ( 8 )

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
 *               Function Declarations
 ******************************************************/
extern wiced_result_t bt_audio_hfp_dial_last_number(void);

static wiced_result_t app_multi_func_short_release(void);
static wiced_result_t app_multi_func_long_release(void);
static wiced_result_t app_pause_play(void);
static wiced_result_t app_factory_reset(void);

static wiced_result_t service_multi_functions_short_release(void);
static wiced_result_t service_multi_functions_long_release(void);
static wiced_result_t service_media_backward(void);
static wiced_result_t service_media_forward(void);
static wiced_result_t service_media_stop(void);
static wiced_result_t service_pause_play(void);
static wiced_result_t service_volume_up(void);
static wiced_result_t service_volume_down(void);

static void app_service_action_arbitrator(app_service_action_t action);
static void app_button_event_handler(const button_manager_button_t *button, button_manager_event_t event, button_manager_button_state_t state);

/******************************************************
 *               Variable Definitions
 ******************************************************/

static const wiced_button_manager_configuration_t button_manager_configuration =
{
    .short_hold_duration     = 500  * MILLISECONDS,
    .medium_hold_duration    = 1500  * MILLISECONDS,
    .long_hold_duration      = 4000  * MILLISECONDS,
    .very_long_hold_duration = 8000  * MILLISECONDS,
    .debounce_duration       = 150   * MILLISECONDS, /* typically a click takes around ~150-200 ms */

    .event_handler = app_button_event_handler,
};

/* Static button configuration */
static const wiced_button_configuration_t button_configurations[] =
{
#if ( WICED_PLATFORM_BUTTON_COUNT > 0 )
    [ BACK_BUTTON           ]     = { PLATFORM_BUTTON_1, BUTTON_CLICK_EVENT | BUTTON_LONG_DURATION_EVENT, 0 },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 1 )
    [ VOLUME_DOWN_BUTTON    ]     = { PLATFORM_BUTTON_2, BUTTON_CLICK_EVENT, 0 },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 2 )
    [ VOLUME_UP_BUTTON      ]     = { PLATFORM_BUTTON_3, BUTTON_CLICK_EVENT, 0 },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 3 )
    [ PLAY_PAUSE_BUTTON     ]     = { PLATFORM_BUTTON_4, BUTTON_CLICK_EVENT, 0 },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 4 )
    [ MULTI_FUNCTION_BUTTON ]     = { PLATFORM_BUTTON_5, BUTTON_CLICK_EVENT | BUTTON_SHORT_DURATION_EVENT | BUTTON_LONG_DURATION_EVENT, 0 },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 5 )
    [ FORWARD_BUTTON        ]     = { PLATFORM_BUTTON_6, BUTTON_CLICK_EVENT, 0 },
#endif
};

/* Button objects for the button manager */
button_manager_button_t buttons[] =
{
#if ( WICED_PLATFORM_BUTTON_COUNT > 0 )
    [ BACK_BUTTON ]             = { &button_configurations[ BACK_BUTTON ]        },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 1 )
    [ FORWARD_BUTTON ]          = { &button_configurations[ FORWARD_BUTTON ]     },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 2 )
    [ VOLUME_UP_BUTTON ]        = { &button_configurations[ VOLUME_UP_BUTTON ]   },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 3 )
    [ VOLUME_DOWN_BUTTON ]      = { &button_configurations[ VOLUME_DOWN_BUTTON ] },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 4 )
    [ PLAY_PAUSE_BUTTON ]       = { &button_configurations[ PLAY_PAUSE_BUTTON ]  },
#endif
#if ( WICED_PLATFORM_BUTTON_COUNT > 5 )
    [ MULTI_FUNCTION_BUTTON ]   = { &button_configurations[ MULTI_FUNCTION_BUTTON ]  },
#endif
};

static button_manager_t button_manager;
wiced_worker_thread_t   button_worker_thread;

/* A table for deciding how a particular action will be handled...
 * Table must be in same order as app_service_action_t enum variables */
static const button_action_function_table_t action_table[] =
{
    { NO_ACTION,                                NULL,                                    NULL },
    { ACTION_MULTI_FUNCTION_SHORT_RELEASE,      service_multi_functions_short_release,   app_multi_func_short_release },
    { ACTION_MULTI_FUNCTION_LONG_RELEASE,       service_multi_functions_long_release,    app_multi_func_long_release },
    { ACTION_PAUSE_PLAY,                        service_pause_play,                      app_pause_play },
    { ACTION_STOP,                              service_media_stop,                      NULL },
    { ACTION_FORWARD,                           service_media_forward,                   NULL },
    { ACTION_BACKWARD,                          service_media_backward,                  NULL },
    { ACTION_VOLUME_UP,                         service_volume_up,                       NULL },
    { ACTION_VOLUME_DOWN,                       service_volume_down,                     NULL },
    { ACTION_FACTORY_RESET,                     app_factory_reset,                       NULL },
};

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_result_t app_init_button_interface()
{
    wiced_rtos_create_worker_thread(&button_worker_thread, WICED_DEFAULT_WORKER_PRIORITY, BUTTON_WORKER_STACK_SIZE, BUTTON_WORKER_QUEUE_SIZE);
    return button_manager_init(&button_manager, &button_manager_configuration, &button_worker_thread, buttons, ARRAY_SIZE(buttons));
}

static wiced_bool_t is_service_valid(wiced_app_service_t  *service)
{
    return ((service != NULL && service->button_handler != NULL) ? WICED_TRUE : WICED_FALSE);
}

static wiced_result_t service_volume_up(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return result;
    }
    result = service->button_handler(ACTION_VOLUME_UP);

    return result;
}

static wiced_result_t service_volume_down(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return result;
    }

    result = service->button_handler(ACTION_VOLUME_DOWN);
    return result;
}

static wiced_result_t service_media_backward(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return result;
    }

    result = service->button_handler(ACTION_BACKWARD);
    return result;
}

static wiced_result_t service_media_forward(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return result;
    }

    result = service->button_handler(ACTION_FORWARD);
    return result;
}

static wiced_result_t service_pause_play(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();

    if (!is_service_valid(service))
    {
        result = WICED_UNSUPPORTED;
        goto _exit;
    }

    result = service->button_handler(ACTION_PAUSE_PLAY);
_exit:
    return result;
}

static wiced_result_t service_media_stop(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return  result;
    }

    result = service->button_handler(ACTION_STOP);
    return result;
}

static wiced_result_t service_multi_functions_short_release(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return result;
    }

    result = service->button_handler(ACTION_MULTI_FUNCTION_SHORT_RELEASE);
    return result;
}

static wiced_result_t service_multi_functions_long_release(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    service = get_app_current_service();
    if (!is_service_valid(service))
    {
        return result;
    }

    result = service->button_handler(ACTION_MULTI_FUNCTION_LONG_RELEASE);
    return result;
}

static wiced_result_t app_multi_func_short_release(void)
{
    wiced_result_t result = WICED_ERROR;

    /* if HFP is connected -- always do the dial-last-number */
    if (bt_audio_hfp_get_connection_state() == WICED_BT_HFP_HF_STATE_SLC_CONNECTED)
    {
        result = bt_audio_hfp_dial_last_number();
    }
    else
    {
#if 0
        bt_audio_connect_services();
#endif
        result = WICED_SUCCESS;
    }
    return result;
}

static wiced_result_t app_multi_func_long_release(void)
{
    wiced_result_t result = WICED_ERROR;

    if (bt_audio_hfp_get_connection_state() == WICED_BT_HFP_HF_STATE_SLC_CONNECTED)
    {
        //result = wiced_app_start_voice_dial();
    }
    else
    {
        //result = wiced_bt_dm_reset_bluetooth_scan_mode();
    }

    return result;
}

static wiced_result_t app_pause_play(void)
{
    wiced_result_t result = WICED_ERROR;
    wiced_app_service_t  *service = NULL;
    /* FIXME : Here we can figure out which was last played audio service and
     * and send the play event back to that service only. For time being,
     * We are routing it to A2DP only*/
    service = wiced_get_entry(SERVICE_BT_A2DP);
    if (service == NULL)
    {
        result = WICED_UNSUPPORTED;
        goto _exit;
    }
    if (service->state == SERVICE_DISABLED)
    {
        result = WICED_ERROR;
        goto _exit;
    }
    if (service->button_handler)
    {
        result = service->button_handler(ACTION_PAUSE_PLAY);
    }

_exit:
    return result;
}

static wiced_result_t app_factory_reset(void)
{
    /* do something */
    WPRINT_APP_INFO(("[App] Factory resetting not implemented !\n"));
    return WICED_ERROR;
}

static void app_service_action_arbitrator(app_service_action_t action)
{
    wiced_result_t result = WICED_ERROR;

    if (action == NO_ACTION)
    {
        return;
    }

    if (action_table[action].primary)
    {
        result = action_table[action].primary();
    }

    if (result != WICED_SUCCESS && action_table[action].secondary)
    {
        result = action_table[action].secondary();
    }

    return;
}

/**
 *
 * Button events thread callback
 *
 * @param button
 * @param state
 * @return
 */
static void app_button_event_handler(const button_manager_button_t *button, button_manager_event_t event, button_manager_button_state_t state)
{
    uint32_t button_event_id = BUTTON_EVENT_ID(ARRAY_POSITION(buttons, button), event, state);

    WPRINT_LIB_INFO(("[App] Button-%d [event: %x state:%x]\n", button->configuration->button, (unsigned int)event, (unsigned int)state));

    app_service_action_t new_action = NO_ACTION;

    switch (button_event_id)
    {
    case BUTTON_EVENT_ID(MULTI_FUNCTION_BUTTON, BUTTON_CLICK_EVENT,          BUTTON_STATE_RELEASED):
        new_action = ACTION_MULTI_FUNCTION_SHORT_RELEASE;
        break;

    case BUTTON_EVENT_ID(MULTI_FUNCTION_BUTTON, BUTTON_LONG_DURATION_EVENT,  BUTTON_STATE_RELEASED):
        new_action = ACTION_MULTI_FUNCTION_LONG_RELEASE;
        break;

    case BUTTON_EVENT_ID(BACK_BUTTON,           BUTTON_CLICK_EVENT,          BUTTON_STATE_RELEASED):
        new_action = ACTION_BACKWARD;
        break;

    case BUTTON_EVENT_ID(PLAY_PAUSE_BUTTON,     BUTTON_CLICK_EVENT,          BUTTON_STATE_RELEASED):
        new_action = ACTION_PAUSE_PLAY;
        break;
    case BUTTON_EVENT_ID(FORWARD_BUTTON,        BUTTON_CLICK_EVENT,          BUTTON_STATE_RELEASED):
        new_action = ACTION_FORWARD;
        break;
    case BUTTON_EVENT_ID(VOLUME_UP_BUTTON,      BUTTON_CLICK_EVENT,          BUTTON_STATE_RELEASED):
        new_action = ACTION_VOLUME_UP;
        break;
    case BUTTON_EVENT_ID(VOLUME_DOWN_BUTTON,    BUTTON_CLICK_EVENT,          BUTTON_STATE_RELEASED):
        new_action = ACTION_VOLUME_DOWN;
        break;
    default:
        WPRINT_LIB_INFO(("[App] Button unhandled button_event_id: %x\n", (unsigned int)button_event_id));
        return;
    }

    app_service_action_arbitrator(new_action);

    return;
}

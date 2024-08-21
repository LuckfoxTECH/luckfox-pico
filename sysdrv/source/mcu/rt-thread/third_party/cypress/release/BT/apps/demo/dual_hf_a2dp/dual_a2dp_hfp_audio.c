/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file dual_a2dp_hfp_audio.c
 *
 * File Description: Dual A2DP, Dual HFP audio demo application
 *
 * This application demonstrates how to stream A2DP audio and make/receive Handsfree calls from 2 smartphones.
 *
 * Features Supported:
 *   - Playback of Bluetooth Audio from two Bluetooth AV sources separately.
 *   - Remote control using respective AVRCP connections
 *   - PLAY/PAUSE/FWD/BACKWARD etc. from the DUT.
 *   - Receiving phone calls through respective HFP profile connections with AG devices
 *   - Last number redialling by means of a button press (MFB button)
 *   - Displays the song information on devices that support display (e.g. 43907WAE_1)

.*  Application Instructions:
 *  1. The Bluetooth BD ADDRESS can be changed by the #define WICED_BLUETOOTH_DEVICE_ADDRESS in bt_config_dct.h.
 *      The device name can be changed by the #define WICED_BLUETOOTH_DEVICE_NAME in bt_config_dct.h.
 *  2. To enable display, ensure that GLOBAL_DEFINES     += USE_AUDIO_DISPLAY is uncommented in dual_hf_a2dp.mk
 *  3. Connect DUT with remote device1 and remote device 2 that has HFP/A2DP/AVRCP profile support.
 *  4. Stream Music or Perform call operation on Remote device 1.
 *  5. Stream Music or Perform call operations on Remote device 2.
 *
 * Notes:
 *  1. On initiating Stream Music from 2nd AV Source the 1st/Current AV Source streaming is suspended and AV Stream from 2nd AV started
 *  on 1st AV Suspend confirmation
 *  2. When there is active SCO connection or Active CALL with one AG device, user can't accept call or setup SCO connection from 2nd AG device
 */

#include "wiced.h"
#include "platform_audio.h"
#include "wiced_bt_stack.h"
#include "bluetooth_dm.h"
#include "bluetooth_a2dp.h"
#include "bluetooth_nv.h"
#include "bluetooth_hfp.h"
#include "wiced_bt_remote_control.h"
#include "hashtable.h"
#include "platform.h"
#include "app_button_interface.h"
#ifdef USE_AUDIO_DISPLAY
#include "audio_display.h"
#endif
#include "wwd_debug.h"
/******************************************************
 *                      Macros
 ******************************************************/

//#define APP_WORKER_THREAD_STACK_SIZE                (2048)
#define APP_WORKER_THREAD_STACK_SIZE                (512)
#define APP_QUEUE_MAX_ENTRIES                       (5)
#define AVRCP_INITIATE_CONNECTION_TIMEOUT          500
#define AVDT_SUSPEND_REQ_TIMEOUT                   4000

/******************************************************
 *                    Constants
 ******************************************************/

#define FIRMWARE_VERSION                    "wiced-1.0"

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
void bt_audio_bluetooth_service_init(void);

void host_get_firmware_version(const char **firmware_string);
#if 0
static void app_worker_thread(uint32_t arg);

extern void bt_audio_timer_expiry_handler(void *arg);
extern void bt_audio_suspend_req_timer_expiry_handler(void *arg);
#endif

/******************************************************
 *               Variables Definitions
 ******************************************************/
extern uint8_t bluetooth_device_name[249];
extern wiced_bt_cfg_settings_t wiced_bt_audio_cfg_settings;
#if 0
extern wiced_timer_t bt_audio_avrcp_timer;
extern wiced_timer_t bt_audio_avdt_suspend_timer;
#endif
uint8_t app_worker_thread_stack[APP_WORKER_THREAD_STACK_SIZE] __attribute__((section(".ccm")));

app_worker_data_t app_data =
{
    .current_service        =   NULL,
    .play_button_state      =   WICED_FALSE,
    .a2dp_media_state       =   WICED_FALSE,
    .a2dp_current_stream    =   NULL,
    .a2dp_last_stream       =   NULL,
};

const char *firmware_version = FIRMWARE_VERSION;

#ifdef USE_AUDIO_DISPLAY
static wiced_thread_t display_thread;
#endif /* USE_AUDIO_DISPLAY */

/******************************************************
 *               Function Definitions
 ******************************************************/
#if 0
void signal_for_event_semaphore(app_event_t event, wiced_bool_t completed)
{
    wiced_rtos_lock_mutex(&app_data.lock);
    app_data.event_mask = event;
    wiced_rtos_unlock_mutex(&app_data.lock);

    wiced_rtos_set_semaphore(&app_data.queue_semaphore);
}

wiced_result_t wait_for_event_semaphore(app_event_t event)
{
    while (1)
    {
        wiced_rtos_get_semaphore(&app_data.queue_semaphore, WICED_NEVER_TIMEOUT);
        if (app_data.event_mask == event)
        {
            break;
        }
    }
    return WICED_TRUE;
}

static void app_worker_thread(uint32_t args)
{
    app_queue_element_t message;
    wiced_result_t result;

    WPRINT_APP_INFO(("[App] Dual A2DP HFP thread started...\n"));

    while (WICED_TRUE)
    {
        result = wiced_rtos_pop_from_queue(&app_data.queue, &message, WICED_NEVER_TIMEOUT);
        if (result != WICED_SUCCESS)
        {
            WPRINT_APP_ERROR(("[App] Error receiving event from app-queue\n"));
            continue;
        }

        if (message.function)
        {
            message.function();
        }

        if (message.wait_for_event_complete == WICED_TRUE)
        {
            wait_for_event_semaphore(message.event);
        }
    }
}
#endif

wiced_result_t wiced_app_shutdown_action(void)
{
    //wiced_core_deinit();
    RT_ASSERT(0);
    return WICED_SUCCESS;
}

void wiced_app_system_reboot(void)
{
    /*
       wiced_core_deinit();
       wiced_rtos_delay_milliseconds( WICED_APP_REBOOT_DELAY_MSECS );
       wiced_framework_reboot( );*/
    RT_ASSERT(0);
}
wiced_app_service_t *get_app_current_service(void)
{
    wiced_app_service_t  *current_service;

    current_service = app_data.current_service;
    return (current_service);
}

wiced_result_t app_set_service_state(service_type_t service_type, service_state_t state)
{
#if 1
    wiced_app_service_t *service;

    if (service_type == SERVICE_NONE)
    {
        WPRINT_APP_INFO(("[%s] new service type is NONE. Can't proceed\n", __func__));
        return WICED_ERROR;
    }

    wiced_rtos_lock_mutex(&app_data.lock);
    service = wiced_get_entry(service_type);
    if (service == NULL)
    {
        WPRINT_APP_INFO(("[%s] new service is NULL. Can't proceed\n",  __func__));
        wiced_rtos_unlock_mutex(&app_data.lock);
        return WICED_ERROR;
    }

    service->state = state;
    wiced_rtos_unlock_mutex(&app_data.lock);

    return WICED_SUCCESS;
#else
    rt_kprintf("[stub] %s\n", __FUNCTION__);
    return WICED_SUCCESS;
#endif
}

wiced_result_t app_disable_service(service_type_t service_type)
{
    wiced_result_t result;
    /*if this is the current service, then reset the "current service"*/
    result = app_reset_current_service(service_type);
    result = app_set_service_state(service_type, SERVICE_DISABLED);

    return result;
}

wiced_result_t app_set_current_service(service_type_t service_type)
{
#if 1
    int i = 0;
    wiced_app_service_cell_t *cell = NULL;
    wiced_app_service_t *new_service;
    wiced_app_service_t *current;
    wiced_result_t result = WICED_ERROR;

    if (service_type == SERVICE_NONE)
    {
        WPRINT_APP_ERROR(("[%s] new service type is NONE. Can't proceed\n", __func__));
        return result;
    }

    wiced_rtos_lock_mutex(&app_data.lock);
    /* Get the service pointer to new service */
    new_service = wiced_get_entry(service_type);

    /* Get the current service pointer */
    current = get_app_current_service();

    if (new_service == NULL)
    {
        WPRINT_APP_ERROR(("[%s] new service is NULL. Can't proceed\n", __func__));
        goto SET_UNLOCK_AND_END;
    }

    if (current != NULL)
    {
        /* A lower priority service 'should' never interrupt a higher-priority service because
         * currently active higher priority service 'should' have erstwhile prevented them.
         */
        if (current->priority > new_service->priority)
        {
            WPRINT_APP_ERROR(("[%s] New service[%d] lower priority than current [%d]..Aborting !!\n", __func__, new_service->priority, current->priority));
            goto SET_UNLOCK_AND_END;
        }
    }

    app_data.current_service = new_service;

    for (i = 1; i <= new_service->priority; i++)
    {
        for (cell = (wiced_service_array[i]); cell != NULL; cell = cell->next)
        {
            /* if same service, skip... */
            if (cell->service.type == new_service->type)
            {
                continue;
            }

            /* prevent the services which are not in prevented/preempted state already */
            if (cell->service.prevent_service &&
                    (cell->service.state != SERVICE_PREEMPTED || cell->service.state != SERVICE_PREVENTED))
            {
                cell->service.prevent_service(&(cell->service));
            }
        }
    }
    result = WICED_SUCCESS;

SET_UNLOCK_AND_END:
    wiced_rtos_unlock_mutex(&app_data.lock);
    return result;
#else
    rt_kprintf("[stub] %s\n", __FUNCTION__);
    return WICED_SUCCESS;
#endif
}

wiced_result_t app_reset_current_service(service_type_t service_type)
{
    uint8_t i = 0;
    wiced_app_service_cell_t *cell = NULL;
    wiced_app_service_t *old_service;
    wiced_app_service_t *current;

    if (service_type == SERVICE_NONE)
    {
        WPRINT_APP_ERROR(("[%s] new service type is NONE. Can't proceed\n", __func__));
        return WICED_ERROR;
    }

    wiced_rtos_lock_mutex(&app_data.lock);

    old_service = wiced_get_entry(service_type);
    current = get_app_current_service();
    if (old_service == NULL || current == NULL || (old_service->type != current->type))
    {
        wiced_rtos_unlock_mutex(&app_data.lock);
        return WICED_ERROR;
    }

    old_service->state = SERVICE_IDLE;
    app_data.current_service = NULL;
    /* Iterate through all low-priority services */
    for (i = old_service->priority; i >= 1; i--)
    {
        cell = (wiced_service_array[i]);

        for (cell = wiced_service_array[i]; cell != NULL; cell = cell->next)
        {
            /* if same service, skip... */
            if (cell->service.type == service_type)
                continue;

            /* if some service had been preempted from playing */
            if (cell->service.state == SERVICE_PREEMPTED)
            {
                /* allow the 'preempted' one and make sure that others remain 'prevented' state */
                if (cell->service.allow_service)
                {
                    cell->service.allow_service(&(cell->service));
                }
                wiced_rtos_unlock_mutex(&app_data.lock);
                return WICED_SUCCESS;
            }

            /* keep allowing whatever would have been prevented */
            if (cell->service.allow_service)
                cell->service.allow_service(&(cell->service));
        }
    }

    wiced_rtos_unlock_mutex(&app_data.lock);
    return WICED_SUCCESS;
}

void host_get_firmware_version(const char **firmware_string)
{
    *firmware_string = firmware_version;
}

static void app_data_init(void)
{
    /* some book-keeping framework initialization */
    wiced_init_hashtable();
    wiced_rtos_init_mutex(&app_data.lock);
#if 0
    wiced_rtos_init_semaphore(&app_data.queue_semaphore);
    //app_init_button_interface();
    wiced_rtos_init_queue(&app_data.queue, "app_worker_queue", sizeof(app_queue_element_t), APP_QUEUE_MAX_ENTRIES);
    //wiced_rtos_create_thread_with_stack( &app_data.thread_handle, WICED_DEFAULT_WORKER_PRIORITY, "app_worker_thread", app_worker_thread,
    //                                                                   app_worker_thread_stack, APP_WORKER_THREAD_STACK_SIZE, NULL );
    wiced_rtos_create_thread(&app_data.thread_handle, WICED_DEFAULT_WORKER_PRIORITY, "app_worker_thread", app_worker_thread,
                             APP_WORKER_THREAD_STACK_SIZE, NULL);
    wiced_rtos_init_timer(&bt_audio_avrcp_timer, AVRCP_INITIATE_CONNECTION_TIMEOUT, bt_audio_timer_expiry_handler, NULL);
    wiced_rtos_init_timer(&bt_audio_avdt_suspend_timer, AVDT_SUSPEND_REQ_TIMEOUT, bt_audio_suspend_req_timer_expiry_handler, NULL);
#endif
}

/**
 * void application_start(void)
 *
 * Application start.
 */
extern void DispBtSnoopInit(void);

void application_start(void)
{
#if defined(BTSNOOPDISP_INCLUDED) && (BTSNOOPDISP_INCLUDED == TRUE)
    DispBtSnoopInit();
#endif
    wiced_core_init();
    WPRINT_APP_INFO(("[App] Starting Dual A2DP-HFP App\n"));

    platform_init_audio();
    WPRINT_APP_INFO(("[App] Initialized Platform-audio...\n"));

    app_data_init();
    /* Initialize BT NVRAM with APP_OFFSET for Bluetooth */

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

#ifdef USE_AUDIO_DISPLAY
    audio_display_create_management_thread(&display_thread, WICED_AUDIO_DISPLAY_ENABLE_WIFI);
    audio_display_header_update_options(BATTERY_ICON_IS_VISIBLE | BATTERY_ICON_SHOW_PERCENT);
#endif /*USE_AUDIO_DISPLAY */
    bt_audio_bluetooth_service_init();
}

extern wiced_result_t hello_sensor_application_deinit(void);
extern void bt_audio_avrcp_disconnect_service(void);
extern void bt_audio_a2dp_disconnect_service(void);
extern void bt_audio_hfp_disconnect_service(void);
extern wiced_result_t bt_audio_decoder_context_deinit(void);
extern wiced_result_t wiced_dct_clean(void);
extern wiced_result_t wiced_remove_entry(void);
extern void bt_audio_hfp_clear_data(void);
extern wiced_result_t brcm_dac_deinit(void *driver_data);
extern wiced_result_t rk_recorder_deinit(void *driver_data);
extern wiced_result_t wiced_audio_clean(void);
extern audio_device_class_t audio_dev_class;

void application_stop(void)
{
    wiced_result_t result;

    result = hello_sensor_application_deinit();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("hello_sensor_application_deinit: result=%d\n", result));
        return;
    }
    rt_thread_mdelay(500);

    bt_audio_avrcp_disconnect_service();
    rt_thread_mdelay(500);

    bt_audio_a2dp_disconnect_service();
    rt_thread_mdelay(500);

    bt_audio_hfp_disconnect_service();
    rt_thread_mdelay(500);

    result = wiced_bt_remote_control_deinit();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_bt_remote_control_deinit: result=%d\n", result));
        return;
    }

    result = wiced_bt_a2dp_sink_deinit();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_bt_a2dp_sink_deinit: result=%d\n", result));
        return;
    }

    result = wiced_bt_hfp_hf_deinit();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_bt_hfp_hf_deinit: result=%d\n", result));
        return;
    }

    rt_thread_mdelay(500);

    result = wiced_bt_stack_deinit();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_ERROR(("Failed to deinit Bluetooth\n"));
        return;
    }

    bt_audio_nv_deinit();

    rt_thread_mdelay(500);

    result = bt_audio_reset_decoder_config();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_rtos_delete_thread: result=%d\n", result));
        return;
    }

    result = bt_audio_decoder_context_deinit();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_decoder_context_deinit: result=%d\n", result));
        return;
    }

    result = bt_audio_deinit_device(BT_AUDIO_DEVICE_PLAYER);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_deinit_player: result=%d\n", result));
        return;
    }

    result = bt_audio_deinit_device(BT_AUDIO_DEVICE_RECORDER);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("bt_audio_deinit_player: result=%d\n", result));
        return;
    }

    result = brcm_dac_deinit(NULL);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("brcm_dac_deinit: result=%d\n", result));
        return;
    }

    result = rk_recorder_deinit(NULL);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("rk_recorder_deinit: result=%d\n", result));
        return;
    }

    result = wiced_audio_clean();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_audio_clean: result=%d\n", result));
        return;
    }

    result = wiced_dct_clean();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_audio_clean: result=%d\n", result));
        return;
    }

    result = wiced_rtos_deinit_mutex(&global_trace_mutex);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("global_trace_mutex: result=%d\n", result));
        return;
    }

    result = wiced_rtos_deinit_mutex(&app_data.lock);
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("app_data.lock: result=%d\n", result));
        return;
    }

    result = wiced_remove_entry();
    if (result != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("wiced_remove_entry: result=%d\n", result));
        return;
    }

    bt_audio_hfp_clear_data();

    WPRINT_APP_INFO(("application_stop: success \n"));
}

static void bt_deinit(void)
{
    application_stop();
}
MSH_CMD_EXPORT(bt_deinit, stop Bluetooth);

void DispBtSnoopdeInit(void);
static void btsnoop_deinit(void)
{
    DispBtSnoopdeInit();
}
MSH_CMD_EXPORT(btsnoop_deinit, stop btsnoop);

extern void bt_audio_hfp_connect_service(void);
static void hfp_connect(void)
{
    bt_audio_hfp_connect_service();
}
MSH_CMD_EXPORT(hfp_connect, connect hfp);

extern void bt_audio_hfp_disconnect_service(void);
static void hfp_disconnect(void)
{
    bt_audio_hfp_disconnect_service();
}
MSH_CMD_EXPORT(hfp_disconnect, disconnect hfp);


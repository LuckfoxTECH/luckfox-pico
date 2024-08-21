/*
 * $ Copyright Cypress Semiconductor $
 */

/*
 * @file:   bluetooth_dm.h
 *
 * This file Contains Bluetooth Device Manager related info and definitions.
 *
 */

#pragma once
#include "rtos.h"
#include "wwd_constants.h"
#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define DM_LINK_LOSS_RETRY_INTERVAL      (15000)
#define DM_LINK_LOSS_NUM_RETRIES         (3)
#define HANDS_FREE_SCN                        0x01

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct
{
    wiced_bool_t    connect_state;
    /* Timer to start on link-loss */
    wiced_timer_t   timer;
    /* Total number of retries for which Device will try to connect after link loss */
    uint8_t         num_retries;
} bluetooth_dm_context_t;

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

void bt_audio_enable_bluetooth(void);
void bt_audio_connect_services(void);
void bt_audio_disconnect_services(void);
bluetooth_dm_context_t *wiced_bt_dm_get_context(void);

#ifdef __cplusplus
} /*extern "C" */
#endif

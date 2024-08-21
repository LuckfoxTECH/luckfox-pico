/*
 * $ Copyright Cypress Semiconductor $
 */

/*
 * @file:   bluetooth_hfp.h
 *
 */

#pragma once
#include "wiced_bt_hfp_hf.h"
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
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

void bt_audio_add_hfp_service(void);

wiced_result_t bt_audio_hfp_dial_last_number(void);

void bt_audio_hfp_sco_connection_evt_handler(wiced_bt_sco_connected_t *connected);

void bt_audio_hfp_sco_disconnection_evt_handler(wiced_bt_sco_disconnected_t *disconnected);

void bt_audio_hfp_sco_connection_request_evt_handler(wiced_bt_sco_connection_request_t *conn_request);

void bt_audio_hfp_sco_connection_change_evt_handler(wiced_bt_sco_connection_change_t *conn_change);

wiced_bt_hfp_hf_connection_state_t bt_audio_hfp_get_connection_state(void);

#ifdef __cplusplus
} /*extern "C" */
#endif

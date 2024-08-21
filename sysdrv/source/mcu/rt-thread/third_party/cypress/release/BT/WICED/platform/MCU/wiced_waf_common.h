/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/* Number of milliseconds User holds "Factory Reset" button
 * during reset to cause a Factory Reset
 */
#ifndef PLATFORM_FACTORY_RESET_TIMEOUT
#define PLATFORM_FACTORY_RESET_TIMEOUT      ( 10000 )
#endif
/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/
typedef struct wiced_app_s
{
    uint8_t           app_id;                 /**< Application Identifier                   */
    uint32_t          offset;                 /**< Offset from the start of the application */
    uint32_t          last_erased_sector;     /**< Last Erased Sector                       */
    image_location_t  app_header_location;    /**< Location of Application header           */
} wiced_app_t;

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/
void           wiced_waf_start_app(uint32_t entry_point);
wiced_result_t wiced_waf_check_factory_reset(void);
uint32_t       wiced_waf_get_button_press_time(int button_index, int led_index, uint32_t max_time);
uint32_t       wiced_waf_get_factory_reset_button_time(void);
wiced_result_t wiced_waf_reboot(void);
wiced_result_t wiced_waf_app_set_boot(uint8_t app_id, char load_once);
wiced_result_t wiced_waf_app_open(uint8_t app_id, wiced_app_t *app);
wiced_result_t wiced_waf_app_close(wiced_app_t *app);
wiced_result_t wiced_waf_app_erase(wiced_app_t *app);
wiced_result_t wiced_waf_app_get_size(wiced_app_t *app, uint32_t *size);
wiced_result_t wiced_waf_app_set_size(wiced_app_t *app, uint32_t size);
wiced_result_t wiced_waf_app_write_chunk(wiced_app_t *app, const uint8_t *data, uint32_t size);
wiced_result_t wiced_waf_app_read_chunk(wiced_app_t *app, uint32_t offset, uint8_t *data, uint32_t size);
wiced_result_t wiced_waf_app_load(const image_location_t *app_header_location, uint32_t *destination);

#ifdef __cplusplus
} /*extern "C" */
#endif

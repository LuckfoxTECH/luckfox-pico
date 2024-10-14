/*
 * Copyright 2019, Cypress Semiconductor Corporation or a subsidiary of
 * Cypress Semiconductor Corporation. All Rights Reserved.
 *
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/**
 * @file
 *
 * Wi-Fi Scan Application using WICED
 *
 * This application scans for Wi-Fi Networks in range and prints
 * scan results to a console. A new scan is initiated at 5 second
 * intervals.
 *
 * The application works with WICED
 *
 */

#include "wwd_management.h"
#include "wwd_wifi.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "network/wwd_buffer_interface.h"
#include "network/wwd_network_constants.h"
#include "RTOS/wwd_rtos_interface.h"
#include "wwd_network.h"
#include "wiced_utilities.h"

#include <rtthread.h>

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#define CIRCULAR_RESULT_BUFF_SIZE     (256)
#define APP_THREAD_STACKSIZE          (4096)

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
static void scan_results_handler(wiced_scan_result_t **result_ptr, void *user_data, wiced_scan_status_t status);

/******************************************************
 *               Variable Definitions
 ******************************************************/
static wiced_mac_t             bssid_list[200]; /* List of BSSID (AP MAC addresses) that have been scanned */
static host_semaphore_type_t   num_scan_results_semaphore;
static wiced_scan_result_t     result_buff[CIRCULAR_RESULT_BUFF_SIZE];
static uint16_t                result_buff_write_pos = 0;
static uint16_t                result_buff_read_pos  = 0;

/******************************************************
 *               Function Definitions
 ******************************************************/

/**
 * Main Scan app
 *
 * Initialises Wiced, starts a scan, waits till scan is finished,
 * fetches scan results and prints them.
 */
static void scan_main(void)   /*@globals killed num_scan_results_semaphore, undef num_scan_results_semaphore, result_buff@*/   /*@modifies bssid_list, result_buff_read_pos, result_buff_write_pos @*/
{
    wiced_scan_result_t *result_ptr = (wiced_scan_result_t *) &result_buff;
    wiced_scan_result_t *record;
    int                   record_number;

    /* Initialise Wiced */
    WPRINT_APP_INFO(("Starting Wiced v" WICED_VERSION "\n"));

#if 0
    wwd_buffer_init(NULL);
    result = wwd_management_wifi_on(COUNTRY);
    if (result != WWD_SUCCESS)
    {
        WPRINT_APP_INFO(("Error %d while starting WICED!\n", (int) result));
    }
#endif

    host_rtos_init_semaphore(&num_scan_results_semaphore);

    /* one-shot Scan */ /*@-infloops@*/
    //while ( 1 == 1 )
    {
        record_number = 1;

        /* Clear list of BSSID addresses already parsed */
        memset(&bssid_list, 0, sizeof(bssid_list));

        /* Start Scan */
        WPRINT_APP_INFO(("Starting Scan\n"));

        /* Examples of scan parameter values */
#if 0
        wiced_ssid_t ssid = { 12,  "YOUR_AP_SSID" };                     /* Scan for networks named YOUR_AP_SSID only */
        wiced_mac_t mac = {{0x00, 0x1A, 0x30, 0xB9, 0x4E, 0x90}};        /* Scan for Access Point with the specified BSSID */
        uint16_t chlist[] = { 6, 0 };                                    /* Scan channel 6 only */
        wiced_scan_extended_params_t extparam = { 5, 1000, 1000, 100 };  /* Spend much longer scanning (1 second and 5 probe requests per channel) */
#endif /* if 0 */

        result_buff_read_pos = 0;
        result_buff_write_pos = 0;

        if (WWD_SUCCESS != wwd_wifi_scan(WICED_SCAN_TYPE_ACTIVE,
                                         WICED_BSS_TYPE_ANY,
                                         NULL, NULL, NULL, NULL,
                                         scan_results_handler,
                                         (wiced_scan_result_t **) &result_ptr,
                                         NULL,
                                         WWD_STA_INTERFACE))
        {
            WPRINT_APP_ERROR(("Error starting scan\n"));
            host_rtos_deinit_semaphore(&num_scan_results_semaphore);
            return;
        }
        WPRINT_APP_INFO(("Waiting for scan results...\n"));

        while (host_rtos_get_semaphore(&num_scan_results_semaphore, NEVER_TIMEOUT, WICED_FALSE) == WWD_SUCCESS)
        {
            int k;

            record = &result_buff[result_buff_read_pos];

            /*TODO: change 0xff to a defined flag */
            if (record->channel == (uint8_t) 0xff)
            {
                /* Scan completed */
                break;
            }

            /* Print SSID */
            WPRINT_APP_INFO(("\n#%03d SSID          : ", record_number));
            for (k = 0; k < (int)record->SSID.length; k++)
            {
                WPRINT_APP_INFO(("%c", (char)record->SSID.value[k]));
            }
            WPRINT_APP_INFO(("\n"));

            wiced_assert("error", (record->bss_type == WICED_BSS_TYPE_INFRASTRUCTURE) || (record->bss_type == WICED_BSS_TYPE_ADHOC));

            /* Print other network characteristics */
            WPRINT_APP_INFO(("     BSSID         : %02X:%02X:%02X:%02X:%02X:%02X\n", (unsigned int) record->BSSID.octet[0],
                             (unsigned int) record->BSSID.octet[1],
                             (unsigned int) record->BSSID.octet[2],
                             (unsigned int) record->BSSID.octet[3],
                             (unsigned int) record->BSSID.octet[4],
                             (unsigned int) record->BSSID.octet[5]));
            WPRINT_APP_INFO(("     RSSI          : %ddBm", (int)record->signal_strength));
            if (record->flags & WICED_SCAN_RESULT_FLAG_RSSI_OFF_CHANNEL)
            {
                WPRINT_APP_INFO((" (off-channel)\n"));
            }
            else
            {
                WPRINT_APP_INFO(("\n"));
            }
            WPRINT_APP_INFO(("     Max Data Rate : %.1f Mbits/s\n", (float)record->max_data_rate / 1000.0));
            WPRINT_APP_INFO(("     Network Type  : %s\n", (record->bss_type == WICED_BSS_TYPE_INFRASTRUCTURE) ? "Infrastructure" : (record->bss_type == WICED_BSS_TYPE_ADHOC) ? "Ad hoc" : "Unknown"));
            WPRINT_APP_INFO(("     Security      : %s\n", (record->security == WICED_SECURITY_OPEN) ? "Open" :
                             (record->security == WICED_SECURITY_WEP_PSK) ? "WEP" :
                             (record->security == WICED_SECURITY_WPA_TKIP_PSK) ? "WPA" :
                             (record->security == WICED_SECURITY_WPA2_AES_PSK) ? "WPA2 AES" :
                             (record->security == WICED_SECURITY_WPA2_MIXED_PSK) ? "WPA2 Mixed" : "Unknown"));
            WPRINT_APP_INFO(("     Radio Band    : %s\n", (record->band == WICED_802_11_BAND_5GHZ) ? "5GHz" : "2.4GHz"));
            WPRINT_APP_INFO(("     Channel       : %d\n", (int) record->channel));
            result_buff_read_pos++;
            if (result_buff_read_pos >= (uint16_t) CIRCULAR_RESULT_BUFF_SIZE)
            {
                result_buff_read_pos = 0;
            }
            record_number++;

        }

        /* Done! */
        WPRINT_APP_INFO(("\nEnd of scan results - next scan in 5 seconds\n"));

        /* Wait 5 seconds till next scan */
        (void) host_rtos_delay_milliseconds((uint32_t) 5000);
    }
    /*@+infloops@*/

    /**
     *  Not required due to while (1)
     *
     *  host_rtos_deinit_semaphore( &num_scan_results_semaphore );
     */
}

void cywifiscan(void)
{
    scan_main();
}

MSH_CMD_EXPORT(cywifiscan, start wscan);

/**
 *  Scan result callback
 *  Called whenever a scan result is available
 *
 *  @param result_ptr : pointer to pointer for location where result is stored. The inner pointer
 *                      can be updated to cause the next result to be put in a new location.
 *  @param user_data : unused
 */
static void scan_results_handler(wiced_scan_result_t **result_ptr, void *user_data, wiced_scan_status_t status)
{
    if (result_ptr == NULL)
    {
        /* finished */
        WPRINT_APP_INFO(("End of scan results\n"));

        result_buff[result_buff_write_pos].channel = 0xff;
        host_rtos_set_semaphore(&num_scan_results_semaphore, WICED_FALSE);
        return;
    }

    wiced_scan_result_t *record = (*result_ptr);

    /* Check the list of BSSID values which have already been printed */
    wiced_mac_t *tmp_mac = bssid_list;
    while (NULL_MAC(tmp_mac->octet) == WICED_FALSE)
    {
        if (CMP_MAC(tmp_mac->octet, record->BSSID.octet) == WICED_TRUE)
        {
            /* already seen this BSSID */
            return;
        }
        tmp_mac++;
    }

    /* New BSSID - add it to the list */
    memcpy(&tmp_mac->octet, record->BSSID.octet, sizeof(wiced_mac_t));

    /* Add the result to the list and set the pointer for the next result */
    result_buff_write_pos++;
    if (result_buff_write_pos >= CIRCULAR_RESULT_BUFF_SIZE)
    {
        result_buff_write_pos = 0;
    }
    *result_ptr = &result_buff[result_buff_write_pos];
    host_rtos_set_semaphore(&num_scan_results_semaphore, WICED_FALSE);

    WPRINT_APP_INFO(("Scan for next\n"));
    wiced_assert("Circular result buffer overflow", result_buff_write_pos != result_buff_read_pos);
}



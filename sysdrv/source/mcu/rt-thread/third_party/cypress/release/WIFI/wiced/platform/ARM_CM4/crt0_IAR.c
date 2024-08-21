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

#include "platform_init.h"
#include "platform_cmsis.h"


extern void __iar_program_start(void);
extern void platform_init_system_clocks(void);
extern void platform_init_memory(void);


int __low_level_init(void);

/* This is the code that gets called on processor reset. To initialize the */
/* device. */
#pragma section=".intvec"
int __low_level_init(void)
{
    /* IAR allows init functions in __low_level_init(), but it is run before global
     * variables have been initialised, so the following init still needs to be done
     * When using GCC, this is done in crt0_GCC.c
     */
    /* Setup the interrupt vectors address */
    SCB->VTOR = (unsigned long)__section_begin(".intvec");

    /* Enable CPU Cycle counting */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; /* Global Enable for DWT */
    DWT->CYCCNT = 0;                                /* Reset the counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            /* Enable cycle counter */

    platform_init_system_clocks();
    platform_init_memory();
    return 1; /* return 1 to force memory init */
}


/* Use stackless  because we might have arrived here
 * because the PC was set by a debugger.  If the vector
 * table isn't installed, then the SP will be bogus.
 * Setup the stack before calling the IAR entry point.
 */
#pragma section="CSTACK"
__stackless void _start(void)
{
    /* Stack pointer is usually set up by boot process, but if program was loaded via jtag in RAM, that might not have happened. */
    __set_MSP((unsigned long)__section_end("CSTACK"));

    /* SP should be setup by this time. */
    __iar_program_start();
}

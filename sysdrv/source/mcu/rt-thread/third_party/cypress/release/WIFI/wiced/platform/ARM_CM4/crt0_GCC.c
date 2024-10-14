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

/* For information on duties of crt0, see http://www.fhi-berlin.mpg.de/th/locserv/alphas/progs/doc/cygnus_doc-99r1/html/1_GS/int04.html#crt0,_the_main_startup_file
 * For details of various segments, see http://www.acsu.buffalo.edu/~charngda/elf.html
 */

#include <string.h>
#include "platform_init.h"
#include "platform_cmsis.h"
#include "platform_toolchain.h"

#define SCB_AIRCR_VECTKEY        ( (unsigned long)( 0x5FA << SCB_AIRCR_VECTKEY_Pos ))

extern void *link_global_data_initial_values;
extern void *link_global_data_start;
extern void *link_global_data_end;
#define link_global_data_size   ((unsigned long)&link_global_data_end - (unsigned long)&link_global_data_start)

extern void *link_run_from_ram_code_flash_location;
extern void *link_run_from_ram_code_ram_location;
extern void *link_run_from_ram_code_ram_end;
#define link_run_from_ram_code_size   ((unsigned long)&link_run_from_ram_code_ram_end - (unsigned long)&link_run_from_ram_code_ram_location)

extern void *link_interrupt_vectors_location;

extern void *link_bss_location;
extern void *link_bss_end;
#define link_bss_size   ((unsigned long)&link_bss_end  -  (unsigned long)&link_bss_location )

#ifdef DEBUG
extern void *link_stack_location;
extern void *link_stack_end;
#define link_stack_size   ((unsigned long)&link_stack_end  -  (unsigned long)&link_stack_location )
#endif /* ifdef DEBUG */

typedef void (*constructor_ptr_t)(void);
extern constructor_ptr_t link_constructors_location[];
extern constructor_ptr_t link_constructors_end;

WEAK void _start(void) __attribute__((naked));      /* Must be naked to avoid overwriting a program which is in RAM during function prologue when stack pointer is invalid */
WEAK void _exit(int status);

#define link_constructors_size   ((unsigned long)&link_constructors_end  -  (unsigned long)&link_constructors_location )

void _start(void)
{
    unsigned long ctor_num;

    /* Stack pointer is usually set up by boot process, but if program was loaded via jtag in RAM, that might not have happened */
    __asm__("   ldr r1, =link_stack_end\n"
            "   mov sp, r1\n"
           );

    /* Setup the interrupt vectors address */
    SCB->VTOR = (unsigned long) &link_interrupt_vectors_location;

    /* Enable CPU Cycle counting */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; /* Global Enable for DWT */
    DWT->CYCCNT = 0;                                /* Reset the counter */
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            /* Enable cycle counter */

    /* Copy from flash any code to be run from RAM. Setup .fastcode section first in case init_clocks() needs it */
    if ((&link_run_from_ram_code_ram_location != &link_run_from_ram_code_flash_location) && (link_run_from_ram_code_size != 0))
    {
        memcpy(&link_run_from_ram_code_ram_location, &link_run_from_ram_code_flash_location, (size_t) link_run_from_ram_code_size);
    }

    /* Initialise clocks and memory. init_clocks() and init_memory() must NOT depend on globals as global data and bss sections aren't initialised yet */
    platform_init_system_clocks();
    platform_init_memory();

    /* Copy initial values for global variables into RAM  */
    if ((&link_global_data_start != &link_global_data_initial_values) && (link_global_data_size != 0))
    {
        memcpy(&link_global_data_start, &link_global_data_initial_values, (size_t) link_global_data_size);
    }

    /* BSS segment is for zero initialised elements, so memset it to zero */
    memset(&link_bss_location, 0, (size_t) link_bss_size);

#if 0 /* was ifdef DEBUG */
    /* This is not a valid way to fill the stack, since it is currently in use - causes a problem in release with debug on - optimisation of active stack overwriting causes hardfault */
    memset(&link_stack_location, 0xA5, link_stack_size);   /* Fill stack with pattern to allow checking of stack usage */
#endif /* if 0 */

    /*
     * Run global C++ constructors if any
     */

    /* TODO: make this an unconditional goto?, so that return address stuff doesn't get put on the stack. (what happens if main returns in this case?) */
    platform_init_mcu_infrastructure();
    platform_init_external_devices();

    for (ctor_num = 0; ctor_num < link_constructors_size / sizeof(constructor_ptr_t); ctor_num++)
    {
        link_constructors_location[ctor_num]();
    }

    main();

    /* the main loop has returned - there is now nothing to do - reboot. */

    /* Reset request */
    SCB->AIRCR = SCB_AIRCR_SYSRESETREQ_Msk | SCB_AIRCR_VECTKEY;

}

void _exit(int status)
{
    /* the main loop has returned - there is now nothing to do - reboot. */

    /* Allow some time for any printf calls to complete */
    volatile unsigned int i; /* try to make this not get optimized out by declaring the variable as volatile */
    volatile unsigned int j; /* try to make this not get optimized out by declaring the variable as volatile */

    (void) status; /* unused parameter */

    for (i = 0; i < (unsigned int) 1000; i++)
    {
        for (j = 0; j < (unsigned int) 10000; j++)
        {
            __asm("NOP");
        }
    }

    /* Reset request */
    SCB->AIRCR = SCB_AIRCR_SYSRESETREQ_Msk | SCB_AIRCR_VECTKEY;

    /* Should never get here, but this will get rid of error: "noreturn function does return" */
    while (1 == 1)
    {
        /* do nothing */
    }
}


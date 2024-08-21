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
#include <stdint.h>
#include "platform_cmsis.h"
#include "platform_constants.h"
#include "platform_isr.h"
#include "platform_isr_interface.h"
#include "platform_assert.h"

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/

#ifdef DEBUG
#define DEBUG_HARDFAULT
#endif /* ifdef DEBUG */

/* Bit Definitions for SCB_CFSR */
#define  SCB_CFSR_IACCVIOL                   ((uint32_t)0x00000001)
#define  SCB_CFSR_DACCVIOL                   ((uint32_t)0x00000002)
#define  SCB_CFSR_MUNSTKERR                  ((uint32_t)0x00000008)
#define  SCB_CFSR_MSTKERR                    ((uint32_t)0x00000010)
#define  SCB_CFSR_MMARVALID                  ((uint32_t)0x00000080)
#define  SCB_CFSR_IBUSERR                    ((uint32_t)0x00000100)
#define  SCB_CFSR_PRECISERR                  ((uint32_t)0x00000200)
#define  SCB_CFSR_IMPRECISERR                ((uint32_t)0x00000400)
#define  SCB_CFSR_UNSTKERR                   ((uint32_t)0x00000800)
#define  SCB_CFSR_STKERR                     ((uint32_t)0x00001000)
#define  SCB_CFSR_BFARVALID                  ((uint32_t)0x00008000)
#define  SCB_CFSR_UNDEFINSTR                 ((uint32_t)0x00010000)
#define  SCB_CFSR_INVSTATE                   ((uint32_t)0x00020000)
#define  SCB_CFSR_INVPC                      ((uint32_t)0x00040000)
#define  SCB_CFSR_NOCP                       ((uint32_t)0x00080000)
#define  SCB_CFSR_UNALIGNED                  ((uint32_t)0x01000000)
#define  SCB_CFSR_DIVBYZERO                  ((uint32_t)0x02000000)

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

typedef struct exception_stacked_registers_struct
{
    /* Stacked registers */
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;
    uint32_t LR;
    uint32_t PC;  /* (Return Address) */
    uint32_t PSR;
} exception_stacked_registers_t;

typedef enum EXC_RETURN_enum
{
    HANDLER_MSP_MSP = 0xF1, /* Return to Handler mode. Exception return gets state from MSP. Execution uses MSP after return. */
    THREAD_MSP_MSP  = 0xF9, /* Return to Thread mode.  Exception return gets state from MSP. Execution uses MSP after return. */
    THREAD_PSP_PSP  = 0xFD  /* Return to Thread mode.  Exception return gets state from PSP. Execution uses PSP after return. */
} EXC_RETURN_t;

/******************************************************
 *               Static Function Declarations
 ******************************************************/

void HardFaultException_handler(uint32_t MSP, uint32_t PSP, uint32_t LR);

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *               Function Definitions
 ******************************************************/
#ifdef DEBUG_HARDFAULT

PLATFORM_DEFINE_NAKED_ISR(HardFaultException)
{
    __ASM("MRS R0, MSP");
    __ASM("MRS R1, PSP");
    __ASM("MOV R2, LR");
    __ASM("B HardFaultException_handler");
}

#if defined( __GNUC__ ) && ( ! defined( __clang__ ) )
#pragma GCC optimize ("O0")
#endif /* if defined( __GNUC__ ) && ( ! defined( __clang__ ) ) */

void HardFaultException_handler(uint32_t MSP, uint32_t PSP, uint32_t LR)
{
    exception_stacked_registers_t  *stackframe;
    uint32_t MMFAR = 0;
    uint32_t BFAR = 0;

    /* Get the Link Register value which contains the EXC_RETURN code */
    EXC_RETURN_t EXC_RETURN = (EXC_RETURN_t)(LR & 0xff);

    /* The location of the stack frame of the offending code is indicated by the EXC_RETURN code */
    if ((EXC_RETURN & 0x00000004) != 0)
    {
        stackframe = (exception_stacked_registers_t *) PSP;
    }
    else
    {
        stackframe = (exception_stacked_registers_t *) MSP;
    }
    (void) stackframe; /* may be unused */

    /* Disable interrupts - this is so that when debugger continues, it will go to caller, not an interrupt routine */
    /* This will mean the system cannot run properly when returning */
    __set_PRIMASK(0x01);

    /* Find cause of hardfault */
    if ((SCB->HFSR & SCB_HFSR_VECTTBL_Msk) != 0)
    {
        WICED_TRIGGER_BREAKPOINT(); /* Vector Table Hard Fault - Bus fault during vector table read during exception processing. */
    }
    else if ((SCB->HFSR & SCB_HFSR_FORCED_Msk) != 0)
    {
        /* Hard Fault is an escalated fault that was not handled */
        /* Need to read the other fault status registers */


        if ((SCB->CFSR & SCB_CFSR_MMARVALID) != 0)
        {
            /* Memory Management Fault address register is valid - read it. */
            MMFAR = SCB->MMFAR;
        }

        if ((SCB->CFSR & SCB_CFSR_BFARVALID) != 0)
        {
            /* Bus Fault address register is valid - read it. */
            BFAR = SCB->BFAR;
        }

        if ((SCB->CFSR & SCB_CFSR_IACCVIOL) != 0)
        {
            /* Memory Management Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Instruction Access Violation - Attempt to execute an instruction from a region marked Execute Never */
            (void) stackframe->LR; /* Check this variable for the jump instruction that jumped to an invalid region */
            (void) stackframe->PC; /* Check this variable for the location that was attempted to be executed */
            /* You may try stepping past the return of this handler, which may return near the location of the error */

        }
        else if ((SCB->CFSR & SCB_CFSR_DACCVIOL) != 0)
        {
            /* Memory Management Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Data Access Violation */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            (void) MMFAR;           /* Check this variable for the address of the attempted access */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_MUNSTKERR) != 0)
        {
            /* Memory Management Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Unstacking fault returning from an exception - stack possibly corrupted during exception handler */
            /* New stackframe is not saved in this case */
        }
        else if ((SCB->CFSR & SCB_CFSR_MSTKERR) != 0)
        {
            /* Memory Management Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Stacking fault whilst entering an exception - probably a bad stack pointer */
            /* Stack frame may be incorrect due to bad stack pointer */

        }
        else if ((SCB->CFSR & SCB_CFSR_IBUSERR) != 0)
        {
            /* Bus Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Instruction Bus Error whilst fetching an instruction*/
        }
        else if ((SCB->CFSR & SCB_CFSR_PRECISERR) != 0)
        {
            /* Bus Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Precise Data Bus Error - i.e. Data Bus fault at well defined location */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            (void) BFAR;           /* Check this variable for the faulting address */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_IMPRECISERR) != 0)
        {
            /* Bus Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Imprecise Data Bus Error - i.e. Data Bus fault occurred but details have been lost due to priorities delaying processing of the fault */
            /* No fault details are available in this case*/
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_UNSTKERR) != 0)
        {
            /* Bus Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Unstacking fault returning from an exception - stack possibly corrupted during exception handler */
            /* New stackframe is not saved in this case */
        }
        else if ((SCB->CFSR & SCB_CFSR_STKERR) != 0)
        {
            /* Bus Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Stacking fault whilst entering an exception - probably a bad stack pointer */
            /* Stack frame may be incorrect due to bad stack pointer */

        }
        else if ((SCB->CFSR & SCB_CFSR_UNDEFINSTR) != 0)
        {
            /* Usage Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Undefined Instruction Usage fault - probably corrupted memory in code space */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_INVSTATE) != 0)
        {
            /* Usage Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Invalid State usage fault - This is probably due to a branch with the LSB=0 - i.e. attempt to execute non-thumb code - Illegal use of EPSR was attempted */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_INVPC) != 0)
        {
            /* Usage Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Invalid PC load usage fault - the EXC_RETURN value in LR was invalid on return from an exception - possibly stack corruption in exception */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_NOCP) != 0)
        {
            /* Usage Fault */
            WICED_TRIGGER_BREAKPOINT();  /* No Coprocessor usage fault - coprocessor instruction attempted on processor without support for them */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_UNALIGNED) != 0)
        {
            /* Usage Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Unaligned access usage fault - Unaligned access whilst UNALIGN_TRP bit of SCB_CCR is set, or any unaligned access to LDM, STM, LDRD or STRD */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else if ((SCB->CFSR & SCB_CFSR_DIVBYZERO) != 0)
        {
            /* Usage Fault */
            WICED_TRIGGER_BREAKPOINT();  /* Divide by zero usage fault */
            (void) stackframe->PC; /* Check this variable for the location of the offending instruction */
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }
        else
        {
            /* Unknown Fault */
            WICED_TRIGGER_BREAKPOINT();
            /* You may try stepping past the return of this handler, which may return near the location of the error */
        }

    }
    else
    {
        /* Unknown Hard Fault cause */
        WICED_TRIGGER_BREAKPOINT();
        /* You may try stepping past the return of this handler, which may return near the location of the error */
    }

    (void) MMFAR; /* This is for debug usage and need not be used programmatically */
    (void) BFAR; /* This is for debug usage and need not be used programmatically */
}
#if defined( __GNUC__ ) && ( ! defined( __clang__ ) )
#pragma GCC reset_options
#endif /* if defined( __GNUC__ ) && ( ! defined( __clang__ ) ) */

#endif /* ifdef DEBUG_HARDFAULT */


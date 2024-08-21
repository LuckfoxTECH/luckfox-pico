;/* SPDX-License-Identifier: BSD-3-Clause */
;/*
; * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
; */


;<h> Stack Configuration
;  <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>

Stack_Size      EQU      0x00000400

                AREA     STACK, NOINIT, READWRITE, ALIGN=3
__stack_limit
Stack_Mem       SPACE    Stack_Size
__initial_sp


;<h> Heap Configuration
;  <o> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
;</h>

Heap_Size       EQU      0x00000C00

                IF       Heap_Size != 0                      ; Heap is provided
                AREA     HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE    Heap_Size
__heap_limit
                ENDIF


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA     RESET, DATA, READONLY
                EXPORT   __Vectors
                EXPORT   __Vectors_End
                EXPORT   __Vectors_Size

__Vectors       DCD      __initial_sp                        ;     Top of Stack
                DCD      Reset_Handler                       ;     Reset Handler
                DCD      NMI_Handler                         ; -14 NMI Handler
                DCD      HardFault_Handler                   ; -13 Hard Fault Handler
                DCD      MemManage_Handler                   ; -12 MPU Fault Handler
                DCD      BusFault_Handler                    ; -11 Bus Fault Handler
                DCD      UsageFault_Handler                  ; -10 Usage Fault Handler
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      0                                   ;     Reserved
                DCD      SVC_Handler                         ;  -5 SVCall Handler
                DCD      DebugMon_Handler                    ;  -4 Debug Monitor Handler
                DCD      0                                   ;     Reserved
                DCD      PendSV_Handler                      ;  -2 PendSV Handler
                DCD      SysTick_Handler                     ;  -1 SysTick Handler

                ; Interrupts
                DCD      Interrupt0_Handler                  ;   0 Interrupt 0
                DCD      Interrupt1_Handler                  ;   1 Interrupt 1
                DCD      Interrupt2_Handler                  ;   2 Interrupt 2
                DCD      Interrupt3_Handler                  ;   3 Interrupt 3
                DCD      Interrupt4_Handler                  ;   4 Interrupt 4
                DCD      Interrupt5_Handler                  ;   5 Interrupt 5
                DCD      Interrupt6_Handler                  ;   6 Interrupt 6
                DCD      Interrupt7_Handler                  ;   7 Interrupt 7
                DCD      Interrupt8_Handler                  ;   8 Interrupt 8
                DCD      Interrupt9_Handler                  ;   9 Interrupt 9

                SPACE    (214 * 4)                           ; Interrupts 10 .. 224 are left out
__Vectors_End
__Vectors_Size  EQU      __Vectors_End - __Vectors


                AREA     |.text|, CODE, READONLY

; Reset Handler

Reset_Handler   PROC
                EXPORT   Reset_Handler             [WEAK]
                IMPORT   SystemInit
                IMPORT   __main

                LDR      R0, =SystemInit
                BLX      R0
                LDR      R0, =__main
                BX       R0
                ENDP


; Macro to define default exception/interrupt handlers.
; Default handler are weak symbols with an endless loop.
; They can be overwritten by real handlers.
                MACRO
                Set_Default_Handler  $Handler_Name
$Handler_Name   PROC
                EXPORT   $Handler_Name             [WEAK]
                B        .
                ENDP
                MEND


; Default exception/interrupt handler

                Set_Default_Handler  NMI_Handler
                Set_Default_Handler  HardFault_Handler
                Set_Default_Handler  MemManage_Handler
                Set_Default_Handler  BusFault_Handler
                Set_Default_Handler  UsageFault_Handler
                Set_Default_Handler  SVC_Handler
                Set_Default_Handler  DebugMon_Handler
                Set_Default_Handler  PendSV_Handler
                Set_Default_Handler  SysTick_Handler

                Set_Default_Handler  Interrupt0_Handler
                Set_Default_Handler  Interrupt1_Handler
                Set_Default_Handler  Interrupt2_Handler
                Set_Default_Handler  Interrupt3_Handler
                Set_Default_Handler  Interrupt4_Handler
                Set_Default_Handler  Interrupt5_Handler
                Set_Default_Handler  Interrupt6_Handler
                Set_Default_Handler  Interrupt7_Handler
                Set_Default_Handler  Interrupt8_Handler
                Set_Default_Handler  Interrupt9_Handler

                ALIGN


; User setup Stack & Heap

                EXPORT   __stack_limit
                EXPORT   __initial_sp
                IF       Heap_Size != 0                      ; Heap is provided
                EXPORT   __heap_base
                EXPORT   __heap_limit
                ENDIF

                END

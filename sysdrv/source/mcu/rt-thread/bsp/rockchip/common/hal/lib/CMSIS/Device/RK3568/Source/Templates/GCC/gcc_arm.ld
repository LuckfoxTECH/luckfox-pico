/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd.
 */

/* Program Entry, set to mark it as "used" and avoid gc */
MEMORY
{
    SRAM  (rxw) : ORIGIN = 0xfdcc0000, LENGTH = 64K   /* system sram 64k */
    DRAM  (rxw) : ORIGIN = 0x00a00000, LENGTH = 512K  /* DRAM */
}

ENTRY(Reset_Handler)

__STACK_SIZE = 0x400;
__FIQ_STACK_SIZE = 0x400;
__IRQ_STACK_SIZE = 0x400;
__SVC_STACK_SIZE = 0x400;
__ABT_STACK_SIZE = 0x400;
__UND_STACK_SIZE = 0x400;

SECTIONS
{
    .text :
    {
        KEEP(*(RESET))                  /* Startup code */

        . = ALIGN(16);
        *(.text)                        /* remaining code */
        *(.text.*)                      /* remaining code */
        *(.rodata)                      /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.gnu.linkonce.t*)

        . = ALIGN(16);
        _etext = .;
    } > DRAM

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > DRAM

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)

        /* This is used by the startup in order to initialize the .data secion */
        _sidata = .;
    } > DRAM
    __exidx_end = .;

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG ((__bss_end__ - __bss_start__) / 4)
        __zero_table_end__ = .;
    } > DRAM

    . = ALIGN(4);

    .data :
    {
        *(.data)
        *(.data.*)

        . = ALIGN(16);
        *(.sdata)
        *(.sdata.*)
    } > DRAM

    . = ALIGN(16);

    .ctors :
    {
        PROVIDE(__ctors_start__ = .);
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
        PROVIDE(__ctors_end__ = .);
    } > DRAM

    .dtors :
    {
        PROVIDE(__dtors_start__ = .);
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
        PROVIDE(__dtors_end__ = .);
    } > DRAM

    . = ALIGN(16);
    .bss (NOLOAD) :
    {
        PROVIDE(__bss_start__ = .);
        *(.bss)
        *(.bss.*)
        *(.dynbss)
        *(COMMON)
        . = ALIGN(16);
        PROVIDE(__bss_end__ = .);
    } > DRAM
    _end = .;

    .stack :
    {
        . = ALIGN(16);

        PROVIDE(__StackTop = .);
        Image$$SYS_STACK$$ZI$$Base = .;
        . += __STACK_SIZE;
        Image$$SYS_STACK$$ZI$$Limit = .;
        __stack = .;

        Image$$FIQ_STACK$$ZI$$Base = .;
        . += __FIQ_STACK_SIZE;
        Image$$FIQ_STACK$$ZI$$Limit = .;

        Image$$IRQ_STACK$$ZI$$Base = .;
        . += __IRQ_STACK_SIZE;
        Image$$IRQ_STACK$$ZI$$Limit = .;

        Image$$SVC_STACK$$ZI$$Base = .;
        . += __SVC_STACK_SIZE;
        Image$$SVC_STACK$$ZI$$Limit = .;

        Image$$ABT_STACK$$ZI$$Base = .;
        . += __ABT_STACK_SIZE;
        Image$$ABT_STACK$$ZI$$Limit = .;

        Image$$UND_STACK$$ZI$$Base = .;
        . += __UND_STACK_SIZE;
        Image$$UND_STACK$$ZI$$Limit = .;

    } > DRAM

    .heap :
    {
        . = ALIGN(16);
        PROVIDE(__heap_start = .);
        . = ORIGIN(DRAM) + LENGTH(DRAM);
        PROVIDE(__heap_end = .);
        end = __heap_end;
    } > DRAM
}

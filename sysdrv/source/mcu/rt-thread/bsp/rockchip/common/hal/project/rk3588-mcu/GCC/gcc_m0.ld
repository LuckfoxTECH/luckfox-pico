/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */

MEMORY
{
    RAM(rxw) : ORIGIN = 0x00000000, LENGTH = 0x100000
}

ENTRY(Reset_Handler)

__STACK_SIZE = 0x1000;

SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        _stext = .;
        . = ALIGN(16);
        KEEP(*(.init))
        KEEP(*(.fini))
        *(.text)                        /* remaining code */
        *(.text.*)                      /* remaining code */
        *(.rodata)                      /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.gnu.linkonce.t*)

        . = ALIGN(16);
        _etext = .;
    } > RAM

    .ARM.extab :
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > RAM

    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > RAM
    __exidx_end = .;

    .zero.table :
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__ / 4)
        __zero_table_end__ = .;
    } > RAM

    .copy.table :
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        __copy_table_end__ = .;
    } > RAM

    . = ALIGN (4);

    .data :
    {
        PROVIDE(__data_start__ = .);
        *(vtable)
        *(.data)
        *(.data.*)

        . = ALIGN(4);
        /* preinit data */
        PROVIDE_HIDDEN (__preinit_array_start = .);
        KEEP(*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        . = ALIGN(4);
        /* init data */
        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array))
        PROVIDE_HIDDEN (__init_array_end = .);

        . = ALIGN(4);
        /* finit data */
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP(*(SORT(.fini_array.*)))
        KEEP(*(.fini_array))
        PROVIDE_HIDDEN (__fini_array_end = .);

        KEEP(*(.jcr*))

        . = ALIGN(16);
        *(.sdata)
        *(.sdata.*)
        PROVIDE(__data_end__ = .);
    } > RAM

    . = ALIGN(16);

    .ctors :
    {
        PROVIDE(__ctors_start__ = .);
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
        PROVIDE(__ctors_end__ = .);
    } > RAM

    .dtors :
    {
        PROVIDE(__dtors_start__ = .);
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
        PROVIDE(__dtors_end__ = .);
    } > RAM

    . = ALIGN(16);

    .bss :
    {
        PROVIDE(__bss_start__ = .);
        *(.bss)
        *(.bss.*)
        *(.dynbss)
        *(COMMON)
        . = ALIGN(16);
        PROVIDE(__bss_end__ = .);
    } > RAM
    _end = .;
   PROVIDE(end = .);

    . = ALIGN(16);
    __STACK_START = ORIGIN(RAM) + LENGTH(RAM) - __STACK_SIZE;
    __HEAP_SIZE = (__STACK_START - .);

    .heap :
    {
        PROVIDE(__heap_start = .);
        . += __HEAP_SIZE;
        PROVIDE(__heap_end = .);
        __HeapLimit = .;
    } > RAM

    .stack __STACK_START :
    {
        __StackLimit = .;
        PROVIDE(__StackTop = .);
        . += __STACK_SIZE;
        __stack = .;
    } > RAM
}

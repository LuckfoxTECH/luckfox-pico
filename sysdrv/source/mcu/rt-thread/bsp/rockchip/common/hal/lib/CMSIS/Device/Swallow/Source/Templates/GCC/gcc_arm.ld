MEMORY
{
    XIP   (rx)  : ORIGIN = 0x10000000, LENGTH = 2M
    SRAM  (rxw) : ORIGIN = 0x00020000, LENGTH = 64K
    TCM   (rxw) : ORIGIN = 0x00030000, LENGTH = 64K
}

FIRMWARE_MAGIC_LEN = 0;
FW_MAGIC_LEN        = FIRMWARE_MAGIC_LEN;

MAIN_STACK_SIZE     = 0x400;

ENTRY(Reset_Handler)

SECTIONS
{
    OS_PART_OFFSET = ORIGIN(XIP) + FW_MAGIC_LEN;
    _stext = OS_PART_OFFSET;
    .vectors OS_PART_OFFSET : AT (OS_PART_OFFSET)
    {
        . = ALIGN(4);
        __ISR_Vector_start__ = .;
        KEEP(*(.vectors))               /* vector table */
        __ISR_Vector_end__ = .;
        *start_m0.o (.text .text.*)
        . = ALIGN(4);
    }

    SRAM_TEXT_DEST = ORIGIN(SRAM);
    SRAM_TEXT_SRC  = LOADADDR(.vectors) + SIZEOF(.vectors);
    _stext_ext = SRAM_TEXT_DEST;
    .sram.text SRAM_TEXT_DEST : AT (SRAM_TEXT_SRC)
    {
        . = ALIGN(4);
        KEEP(*(.sram.text))
        /* *libForceSramDrivers.a: (.text .text.* .rodata .rodata*) 
        *hal_cache.o(.text .text.* .rodata .rodata*) */
        . = ALIGN(32);
    }
    _etext_ext = .;

    __vector_remap__ = ORIGIN(TCM);
    TCM_TEXT_DEST = ORIGIN(TCM) + SIZEOF(.vectors);
    TCM_TEXT_SRC  = LOADADDR(.sram.text) + SIZEOF(.sram.text);
    .tcm.text TCM_TEXT_DEST : AT (TCM_TEXT_SRC)
    {
        . = ALIGN(4);
        KEEP(*(.tcm.text))
        . = ALIGN(32);
    }

    XIP_TEXT_DEST = LOADADDR(.tcm.text) + SIZEOF(.tcm.text);
    XIP_TEXT_SRC  = XIP_TEXT_DEST;
    .xip.text XIP_TEXT_DEST : AT (XIP_TEXT_SRC)
    {
        KEEP(*(.init))
        KEEP(*(.fini))

        *(.text)                        /* remaining code */
        *(.text.*)                      /* remaining code */
        *(.rodata)                      /* read-only data (constants) */
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.gnu.linkonce.t*)

        . = ALIGN(4);
        PROVIDE(__ctors_start__ = .);
        /* old GCC version uses .ctors */
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
        /* new GCC version uses .init_array */
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        PROVIDE(__ctors_end__ = .);

        /* section information for finsh shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);

        /* section information for initial. */
        . = ALIGN(4);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
        . = ALIGN(4);

        /* section information for modules */
        . = ALIGN(4);
        __rtmsymtab_start = .;
        KEEP(*(RTMSymTab))
        __rtmsymtab_end = .;
        . = ALIGN(4);

        . = ALIGN(32);
    }

    SRAM_DATA_DEST = ADDR(.sram.text) + SIZEOF(.sram.text);
    SRAM_DATA_SRC  = LOADADDR(.xip.text) + SIZEOF(.xip.text);
    _etext = SRAM_DATA_SRC;
    .sram.data SRAM_DATA_DEST : AT (SRAM_DATA_SRC)
    {
        . = ALIGN(32);
        __data_start__ = .;
        KEEP(*(.sram.data))
        *(vtable)
        *(.data*)

        . = ALIGN(4);
        PROVIDE(__dtors_start__ = .);
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
        PROVIDE(__dtors_end__ = .);

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
        . = ALIGN(4);
        /* All data end */
        __data_end__ = .;
    }

    TCM_DATA_DEST = ADDR(.tcm.text) + SIZEOF(.tcm.text);
    TCM_DATA_SRC  = LOADADDR(.sram.data) + SIZEOF(.sram.data);
    .tcm.data TCM_DATA_DEST : AT (TCM_DATA_SRC)
    {
        . = ALIGN(32);
        KEEP(*(.tcm.data))
        . = ALIGN(32);
    }

    COPY_TABLE_DEST = LOADADDR(.tcm.data) + SIZEOF(.tcm.data);
    COPY_TABLE_SRC  = COPY_TABLE_DEST;
    .copy.table COPY_TABLE_DEST : AT (COPY_TABLE_SRC)
    {
        . = ALIGN(4);
        __copy_table_start__ = .;
        LONG (LOADADDR(.vectors))
        LONG (ORIGIN(TCM))
        LONG (SIZEOF(.vectors))
        LONG (LOADADDR(.sram.text))
        LONG (ADDR(.sram.text))
        LONG (SIZEOF(.sram.text))
        LONG (LOADADDR(.sram.data))
        LONG (ADDR(.sram.data))
        LONG (SIZEOF(.sram.data))
        LONG (LOADADDR(.tcm.text))
        LONG (ADDR(.tcm.text))
        LONG (SIZEOF(.tcm.text))
        LONG (LOADADDR(.tcm.data))
        LONG (ADDR(.tcm.data))
        LONG (SIZEOF(.tcm.data))
        __copy_table_end__ = .;
    }

    ZERO_TABLE_DEST = LOADADDR(.copy.table) + SIZEOF(.copy.table);
    ZERO_TABLE_SRC  = ZERO_TABLE_DEST;
    .zero.table ZERO_TABLE_DEST : AT (ZERO_TABLE_SRC)
    {
        . = ALIGN(4);
        __zero_table_start__ = .;
        LONG (ADDR(.sram.bss))
        LONG (SIZEOF(.sram.bss))
        LONG (ADDR(.tcm.bss))
        LONG (SIZEOF(.tcm.bss))
        __zero_table_end__ = .;
    }

    .ARM.exidx (ADDR(.zero.table) + SIZEOF(.zero.table)) : AT (LOADADDR(.zero.table) + SIZEOF(.zero.table))
    {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        /* This is used by the startup in order to initialize the .data secion */
        _sidata = .;
        __exidx_end = .;
    }

    .ARM.extab (ADDR(.ARM.exidx) + SIZEOF(.ARM.exidx)) : AT (LOADADDR(.ARM.exidx) + SIZEOF(.ARM.exidx))
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    }

    SRAM_BSS_DEST = ADDR(.sram.data) + SIZEOF(.sram.data);
    .sram.bss SRAM_BSS_DEST (NOLOAD) :
    {
        . = ALIGN(4);
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        __bss_end__ = .;
        . = ALIGN(4);
    }

    TCM_BSS_DEST = ADDR(.tcm.data) + SIZEOF(.tcm.data);
    .tcm.bss TCM_BSS_DEST (NOLOAD) :
    {
        . = ALIGN(32);
        KEEP(*(.tcm.bss))
        . = ALIGN(32);
    }

    PROVIDE( __sram_heap_start__ = ADDR(.sram.bss) + SIZEOF(.sram.bss) );
    PROVIDE( end = __sram_heap_start__);
    MAIN_STACK_DEST =  ORIGIN(SRAM) + LENGTH(SRAM) - MAIN_STACK_SIZE;
    .stack MAIN_STACK_DEST (NOLOAD) :
    {
        PROVIDE( __sram_heap_end__ = . );
        . = ALIGN(4);
        _sstack = .;
        . = . + MAIN_STACK_SIZE;
        _estack = .;
        . = ALIGN(4);
        PROVIDE(__StackTop = .);
    }

    PROVIDE( __tcm_heap_start__ = ADDR(.tcm.bss) + SIZEOF(.tcm.bss) );
    PROVIDE( __tcm_heap_end__ = ORIGIN(TCM) + LENGTH(TCM) );
}

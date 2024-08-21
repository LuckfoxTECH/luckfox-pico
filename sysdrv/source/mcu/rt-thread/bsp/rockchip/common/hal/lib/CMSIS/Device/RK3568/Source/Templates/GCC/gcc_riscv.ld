OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
    RAM (rwx) : ORIGIN = 0x8200000, LENGTH = 0x100000
}

STACK_SIZE = (1024 * 1);
HEAP_SIZE = (1024 * 32);

CL_SIZE = 32;

SECTIONS {
    .text : {
        PROVIDE(__TEXT_START__ = .);
        *startup_rk3568_mcu.o(.text .text.*)
        *(.text .text.*)
        *(sc_test_section)

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

        . = ALIGN(CL_SIZE);
        PROVIDE(__TEXT_END__ = .);
    } > RAM

    /* data segment */
    .data : {
        *(.data .data.*)
        . = ALIGN(CL_SIZE);
    } > RAM

    .sdata : {
        _gp = . + 0x800;
        *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        . = ALIGN(CL_SIZE);
    } > RAM

    /* thread-local data segment */
    .tdata : {
        PROVIDE(_tls_data = .);
        PROVIDE(_tdata_begin = .);
        *(.tdata .tdata.*)
        PROVIDE(_tdata_end = .);
        . = ALIGN(CL_SIZE);
    } > RAM

    .tbss : {
        PROVIDE(__BSS_START__ = .);
        *(.tbss .tbss.*)
        . = ALIGN(CL_SIZE);
        PROVIDE(_tbss_end = .);
    } > RAM

    /* bss segment */
    .sbss : {
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.scommon)
    } > RAM

    .bss : {
        *(.bss .bss.*)
        . = ALIGN(CL_SIZE);
        PROVIDE(__BSS_END__ = .);
    } > RAM

    /* End of uninitalized data segement */
    ._user_heap (NOLOAD):
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        PROVIDE ( __HEAP_START__ = .);
        . = . + HEAP_SIZE;
        . = ALIGN(8);
        PROVIDE ( __HEAP_END__ = .);
        PROVIDE ( _heap_end = .);
    } > RAM

    .stack ORIGIN(RAM) + LENGTH(RAM) - STACK_SIZE : {
        FILL(0);
        PROVIDE(__STACK_START__ = .);
        . += STACK_SIZE;
        PROVIDE(__C_STACK_TOP__ = .);
        PROVIDE(_sp = .);
        PROVIDE(__STACK_END__ = .);
    } > RAM
}

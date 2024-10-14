OUTPUT_ARCH( "riscv" )
ENTRY(_start)

MEMORY {
    DRAM (rwx) : ORIGIN = 0x40000, LENGTH = 0x40000
}

STACK_SIZE = 0x800;

GP_SIZE = 0x400;
CL_SIZE = 0x20;

SECTIONS {
    .text : {
        PROVIDE(__TEXT_START__ = .);
        *start_rv1106_mcu.o(.text .text.*)
        *(.text .text.*)
        *(sc_test_section)

        . = ALIGN(CL_SIZE);
        PROVIDE(__TEXT_END__ = .);
    } > DRAM

    /* data segment */
    .data : {
        *(.data .data.*)
        . = ALIGN(CL_SIZE);
    } > DRAM

    .sdata : {
        _gp = . + GP_SIZE;
        *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata*)
        *(.sdata .sdata.* .gnu.linkonce.s.*)
        . = ALIGN(CL_SIZE);
    } > DRAM

    /* thread-local data segment */
    .tdata : {
        PROVIDE(_tls_data = .);
        PROVIDE(_tdata_begin = .);
        *(.tdata .tdata.*)
        PROVIDE(_tdata_end = .);
        . = ALIGN(CL_SIZE);
    } > DRAM

    .tbss : {
        PROVIDE(__BSS_START__ = .);
        *(.tbss .tbss.*)
        . = ALIGN(CL_SIZE);
        PROVIDE(_tbss_end = .);
    } > DRAM

    /* bss segment */
    .sbss : {
        *(.sbss .sbss.* .gnu.linkonce.sb.*)
        *(.scommon)
    } > DRAM

    .bss : {
        *(.bss .bss.*)
        . = ALIGN(CL_SIZE);
        PROVIDE(__BSS_END__ = .);
    } > DRAM

    /* End of uninitalized data segement */
    ._user_heap (NOLOAD):
    {
        . = ALIGN(CL_SIZE);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        PROVIDE ( __HEAP_START__ = .);
        . = ORIGIN(DRAM) + LENGTH(DRAM) - STACK_SIZE;
        PROVIDE ( __HEAP_END__ = .);
        PROVIDE ( _heap_end = .);
    } > DRAM

    .stack :
    {
        . = ALIGN(CL_SIZE);
        PROVIDE(__STACK_START__ = .);
        . += STACK_SIZE;
        PROVIDE(__C_STACK_TOP__ = .);
        PROVIDE(_sp = .);
        PROVIDE(__STACK_END__ = .);
    } > DRAM
}

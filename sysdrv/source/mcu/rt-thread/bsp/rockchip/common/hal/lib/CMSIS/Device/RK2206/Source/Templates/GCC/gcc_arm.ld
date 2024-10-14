/* Linker script to configure memory regions. */
MEMORY
{
 SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 256K
}
BOOT_ROM_BASE = 0x00000000;
SYS_CODE_BASE = 0x20000000;
SYS_CODE_SIZE = 0x00010000;
 /* 64K */
SYS_CODE_LIMIT = 0x20010000;
SYS_DATA_BASE = 0x20010000;
SYS_DATA_SIZE = 0x00010000;
 /* 64K */
SYS_DATA_LIMIT = 0x20020000;
IDLE_STACK_SIZE = 1K;
MAIN_STACK_SIZE = 16K;
PROVIDE(xAppHeapSize = 128K);
PROVIDE(xHeapSize = 16K);
EXTERN(_write _close _fstat _isatty _lseek _read)
EXTERN(vTaskSwitchContext)
EXTERN(__Vectors)
EXTERN(Reset_Handler)
ENTRY(Main)
SECTIONS
{
 . = 0x20000000;
 __text_start__ = .;
 .AP_SYS_CODE :
 {
  . = ALIGN(4);
  KEEP(*(.isr_vector))
  *(.text*)
  KEEP(*(.init))
  KEEP(*(.fini))
  /* .ctors */
  *crtbegin.o(.ctors)
  *crtbegin?.o(.ctors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
  *(SORT(.ctors.*))
  *(.ctors)
  /* .dtors */
  *crtbegin.o(.dtors)
  *crtbegin?.o(.dtors)
  *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
  *(SORT(.dtors.*))
  *(.dtors)
  . = ALIGN(4);
  __rodata_start__ = .;
  *(.rodata*)
  . = ALIGN(4);
  __rodata_end__ = .;
  KEEP(*(.eh_frame*))
 }
 .ARM.extab :
 {
  *(.ARM.extab* .gnu.linkonce.armextab.*)
 }
 .ARM.exidx :
 {
  __exidx_start = .;
  *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  __exidx_end = .;
 }
 /* All text end */
 __text_end__ = .;
 .AP_SYS_DATA :
 {
  . = ALIGN(4);
  __data_start__ = .;
  _sdata = .;
  *(.data*)
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
  _edata = .;
 }
 .AP_SYS_BSS (NOLOAD) :
 {
  /* All bss start */
  PROVIDE(__bss_start__ = .);
  *(.bss*)
  . = ALIGN(4);
  /* All bss end */
  PROVIDE(__bss_end__ = .);
 }
 .HEAP (COPY) :
 {
  . = ALIGN(8);
  PROVIDE(end = .);
  *(.heap*)
  PROVIDE(ucHeap = .);
  . += xHeapSize;
  . = ALIGN(8);
 } > SRAM
 .APP_HEAP (ADDR(.HEAP) + SIZEOF(.HEAP)) (COPY) :
 {
  . = ALIGN(8);
  PROVIDE(ucAppHeap = .);
  . += xAppHeapSize;
  . = ALIGN(8);
 } > SRAM
 .IDLE_STACK (ADDR(.APP_HEAP) + SIZEOF(.APP_HEAP)) (COPY) :
 {
  . = ALIGN(8);
  . += IDLE_STACK_SIZE;
  . = ALIGN(8);
 } > SRAM
 .AP_MAIN_STACK (ADDR(.IDLE_STACK) + SIZEOF(.IDLE_STACK)) (COPY) :
 {
  . = ALIGN(8);
  . += MAIN_STACK_SIZE;
  . = ALIGN(8);
  PROVIDE(__StackTop = .);
 } > SRAM
/*********************************************************************************
*
* PROVIDE DEFINE
*
*********************************************************************************/
 PROVIDE(Load$$AP_SYS_DATA$$Base = LOADADDR(.AP_SYS_DATA));
 PROVIDE(Image$$AP_SYS_DATA$$ZI$$Length = __bss_end__ - __bss_start__);
 PROVIDE(Image$$AP_SYS_DATA$$ZI$$Base = __bss_start__);
 PROVIDE(Image$$AP_SYS_DATA$$RW$$Base = ADDR(.AP_SYS_DATA));
 PROVIDE(Image$$AP_SYS_DATA$$RW$$Length = _edata - _sdata);
 PROVIDE(Image$$AP_MAIN_STACK$$RW$$Limit = ADDR(.AP_MAIN_STACK) + SIZEOF(.AP_MAIN_STACK));
 PROVIDE(Image$$AP_MAIN_STACK$$ZI$$Limit = ADDR(.AP_MAIN_STACK) + SIZEOF(.AP_MAIN_STACK));
 PROVIDE(Load$$AP_SYS_CODE$$Base = LOADADDR(.AP_SYS_CODE));
 PROVIDE(Image$$AP_SYS_CODE$$Base = ADDR(.AP_SYS_CODE));
 PROVIDE(Image$$AP_SYS_CODE$$Length = SIZEOF(.AP_SYS_CODE));
 PROVIDE(Image$$AP_MAIN_STACK$$RW$$Base = ADDR(.AP_MAIN_STACK));
 PROVIDE(Image$$AP_MAIN_STACK$$RW$$Length = SIZEOF(.AP_MAIN_STACK));
 PROVIDE(Image$$AP_MAIN_STACK$$ZI$$Base = ADDR(.AP_MAIN_STACK));
 PROVIDE(Image$$AP_MAIN_STACK$$ZI$$Length = SIZEOF(.AP_MAIN_STACK));
 PROVIDE(Image$$IDLE_STACK$$RW$$Base = ADDR(.IDLE_STACK));
 PROVIDE(Image$$IDLE_STACK$$RW$$Length = SIZEOF(.IDLE_STACK));
 PROVIDE(Image$$IDLE_STACK$$ZI$$Base = ADDR(.IDLE_STACK));
 PROVIDE(Image$$IDLE_STACK$$ZI$$Length = SIZEOF(.IDLE_STACK));
}

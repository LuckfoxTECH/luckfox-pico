/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "hal_bsp.h"
#include "hal_base.h"
#include "test_conf.h"
#include "task_ipc.h"
#include <stdlib.h>

#ifdef SRAM_USAGE

/********************* Private MACRO Definition ******************************/
#define _SRAM_TEXT_ __attribute__((section(".sram.text")))
#define _SRAM_DATA_ __attribute__((section(".sram.data")))
#define _SRAM_BSS_  __attribute__((section(".sram.bss")))

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

// Example for variable define in SRAM --> "_SRAM_BSS_"
_SRAM_BSS_ static int st_buffer[16];
_SRAM_BSS_ int g_buffer[16];

// Example for data table define in SRAM --> "_SRAM_DATA_"
_SRAM_DATA_ static int st_table[16] = { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55 };
_SRAM_DATA_ int g_table[16] = { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa };

// Example for code define in SRAM --> "_SRAM_TEXT_"
_SRAM_TEXT_ void sram_usage(void)
{
    int i;

    printf("\nsram_example start!!!\n");

    //.....
    printf("\nst_table addr = 0x%08x\n", st_table);
    for (i = 0; i < 16; i++) {
        printf("%02x ", st_table[i]);
    }
    printf("\n");

    //.....
    printf("\nst_buffer addr = 0x%08x\n", st_buffer);
    for (i = 0; i < 16; i++) {
        printf("%02x ", st_buffer[i]);
    }
    printf("\n");
    printf("st_table-->st_buffer:\n");
    for (i = 0; i < 16; i++) {
        st_buffer[i] = st_table[i];
    }
    for (i = 0; i < 16; i++) {
        printf("%02x ", st_buffer[i]);
    }
    printf("\n");

    //.....
    printf("\ng_table addr = 0x%08x\n", g_table);
    for (i = 0; i < 16; i++) {
        printf("%02x ", g_table[i]);
    }
    printf("\n");

    //.....
    printf("\ng_buffer addr = 0x%08x\n", g_buffer);
    for (i = 0; i < 16; i++) {
        printf(" %02x", g_buffer[i]);
    }
    printf("\n");
    printf("g_table-->g_buffer:\n");
    for (i = 0; i < 16; i++) {
        g_buffer[i] = g_table[i];
    }
    for (i = 0; i < 16; i++) {
        printf(" %02x", g_buffer[i]);
    }
    printf("\n");

    printf("\nsram_example exit!!!\n\n");
}

#endif //SRAM_USAGE

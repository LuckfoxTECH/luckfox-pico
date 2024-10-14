/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */
#include <stdio.h>
#include "hal_base.h"
#include "simple_console.h"
#include "command_wdt.h"

#define WDT_TEST_FREQ PLL_INPUT_OSC_RATE

struct command_wdt_info {
    bool auto_mode;
    bool feed;
    int time_s;
};

static void command_wdt_help(void)
{
    printf("WDT utility\n");
    printf("\nwdt [-a] <time>\n");
    printf("Options: \n");
    printf("\n    -a                  Auto feed the dog mode\n");
    printf("    <time>              Time for WDT in seconds\n");
    printf("\nWDT without any params means feed the dog if WDT is enabled\n");
}

static int command_wdt_get_parameter(uint8_t * *start, uint8_t *input,
                                     uint8_t * *next, int *len)
{
    *start = console_get_paramter(input, len, next);
    if (!(*start)) {
        return -1;
    }

    return 0;
}

static int command_wdt_parse(uint8_t *in, int len,
                             struct command_wdt_info *command)
{
    uint8_t *next = in;

    if (!len) {
        command->feed = true;

        return 0;
    }

    while (len) {
        if (command_wdt_get_parameter(&in, next, &next, &len)) {
            return -1;
        }

        if (in[0] == '-' && in[1] == 'a') {
            command->auto_mode = true;
            continue;
        }

        command->time_s = strtoul((char *)in, NULL, 0);
        if (!command->time_s) {
            return -1;
        }
    }

    return 0;
}

static void wdt_isr(uint32_t irq, void *args)
{
    printf("wdt_test: isr eoi\n");
    HAL_WDT_ClearInterrupt();
}

static void command_wdt_process(uint8_t *in, int len)
{
    struct command_wdt_info command = { 0 };

    if (command_wdt_parse(in, len, &command)) {
        command_wdt_help();

        return;
    }

    HAL_WDT_Init(WDT_TEST_FREQ, WDT);

    if (command.feed) {
        printf("wdt: feed the dog\n");
        HAL_WDT_KeepAlive();

        return;
    }

    HAL_WDT_SetTimeout(command.time_s);
    printf("wdt_test: timeout (%s) set-%ds, get-%ds, TORR-0x%lx\n",
           command.auto_mode ? "auto_mode" : "normal_mode",
           command.time_s, HAL_WDT_GetTimeout(), WDT->TORR);
    if (command.auto_mode) {
        HAL_INTMUX_SetIRQHandler(WDT0_IRQn, wdt_isr, NULL);
        HAL_WDT_Start(INDIRECT_SYSTEM_RESET);
        HAL_INTMUX_EnableIRQ(WDT0_IRQn);
    } else {
        HAL_WDT_Start(DIRECT_SYSTEM_RESET);
    }
}

struct console_command command_wdt = {
    .name = "wdt",
    .help = "Set Watch Dog",
    .process = command_wdt_process,
};

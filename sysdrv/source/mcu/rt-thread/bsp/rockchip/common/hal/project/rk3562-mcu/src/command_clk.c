/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2023 Rockchip Electronics Co., Ltd.
 */
#include <stdio.h>
#include "hal_base.h"
#include "simple_console.h"
#include "command_clk.h"

/* Copy STRING from CLOCK_NAME, remove *SRC/*FRAC */
#define FOREACH_CLK(S) \
    S(PLL_APLL)        \
    S(PLL_GPLL)        \
    S(PLL_VPLL)        \
    S(PLL_HPLL)        \
    S(PLL_CPLL)        \
    S(PLL_DPLL)        \
    S(HCLK_BUS)        \
    S(CLK_PMU0_I2C0)   \
    S(CLK_I2C)         \
    S(CLK_PMU1_SPI0)   \
    S(CLK_SPI1)        \
    S(CLK_SPI2)        \
    S(SCLK_SFC)        \
    S(CLK_PMU1_UART0)  \
    S(CLK_UART1)       \
    S(CLK_UART2)       \
    S(CLK_UART3)       \
    S(CLK_UART4)       \
    S(CLK_UART5)       \
    S(CLK_UART6)       \
    S(CLK_UART7)       \
    S(CLK_UART8)       \
    S(CLK_UART9)       \
    S(CLK_PMU1_PWM0)   \
    S(CLK_PWM1_PERI)   \
    S(CLK_PWM2_PERI)   \
    S(CLK_PWM3_PERI)

#define ENUMMATCH(str, match) \
        if (!strncmp(str, #match, strlen(#match))) return match

#define GS(S) #S,
#define EM(S) ENUMMATCH(str, S);

enum clk_operation {
    CLK_OPERATION_NONE = 0,
    CLK_OPERATION_SET,
    CLK_OPERATION_GET,
    CLK_OPERATION_GET_ALL,
};

struct command_clk_info {
    enum CLOCK_Name clk_id;
    uint32_t rate;
    enum clk_operation op;
};

static const char *clk[] = { FOREACH_CLK(GS) };

static enum CLOCK_Name string_to_clk(const char *str)
{
    FOREACH_CLK(EM);

    return CLK_INVALID;
}

static void command_clk_help(void)
{
    int i;

    printf("clk <clk_name> [<rate>]\n");
    printf("    [rate]                  The rate write to <clk_name>\n");
    printf("                            Read clk without [rate]\n");
    printf("    <clk_name>              can be one of list\n");
    for (i = 0; i < (sizeof(clk) / sizeof(clk[1])); i++) {
        printf("                            %s\n", clk[i]);
    }
    printf("                            CLK_ALL\n");
}

static int command_clk_parse(uint8_t *input, int len,
                             struct command_clk_info *command)
{
    uint8_t *next, *tmp;

    input = console_get_paramter(input, &len, &next);
    if (!input) {
        return -1;
    }

    command->clk_id = string_to_clk((char *)input);
    if (!command->clk_id) {
        if (!strncmp((char *)input, "CLK_ALL", strlen("CLK_ALL"))) {
            command->op = CLK_OPERATION_GET_ALL;

            return 0;
        }

        return -1;
    }

    input = console_get_paramter(next, &len, &next);
    if (!input) {
        command->op = CLK_OPERATION_GET;

        return 0;
    }

    command->rate = strtoul((char *)input, (char * *)&tmp, 0);
    if (input == tmp) {
        return -1;
    }
    command->op = CLK_OPERATION_SET;

    return 0;
}

static void command_clk_dump_all_clk(void)
{
    int i;

    for (i = 0; i < (sizeof(clk) / sizeof(clk[1])); i++) {
        printf("    %-20s %ld\n", clk[i], HAL_CRU_ClkGetFreq(string_to_clk(clk[i])));
    }
}

static void command_clk_process(uint8_t *in, int len)
{
    struct command_clk_info command = { 0 };

    if (command_clk_parse(in, len, &command)) {
        command_clk_help();

        return;
    }

    switch (command.op) {
    case CLK_OPERATION_SET:
        HAL_CRU_ClkSetFreq(command.clk_id, command.rate);
        break;
    case CLK_OPERATION_GET:
        printf("%ld\n", HAL_CRU_ClkGetFreq(command.clk_id));
        break;
    case CLK_OPERATION_GET_ALL:
        command_clk_dump_all_clk();
        break;
    default:
        break;
    }
}

struct console_command command_clk = {
    .name = "clk",
    .help = "Set clk param",
    .process = command_clk_process,
};

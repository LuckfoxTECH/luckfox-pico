/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd.
 */

#include "int_mux.h"

void HAL_RISCVIC_SetActiveIRQ(IRQn_Type IRQn)
{
    unsigned int index, set;
    unsigned int temp;

    index = IRQn / 32;
    set = IRQn % 32;
    temp = *(unsigned int *)(INT_MUX + index * 4);
    *(unsigned int *)(INT_MUX + index * 4) = (1 << set) | temp;
}

uint32_t HAL_RISCVIC_GetActiveIRQ(void)
{
    uint32_t cause = -1;
    int i, n;
    unsigned int int_reg;

    for (i = 0; i < INT_GROUP; i++) {
        if (*(unsigned int *)(INT_MUX + INT_MUX_GROUP + i * 4)) {
            break;
        }
    }

    if (i < INT_GROUP) {
        int_reg = *(unsigned int *)(INT_MUX + INT_MUX_GROUP + i * 4);
        for (n = 0; n < 32; n++) {
            if ((int_reg >> n) & 0x1 == 0x1) {
                break;
            }
        }

        cause = i * 32 + n;
    }

    return cause;
}

/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    int_mux.c
  * @author  Jason Zhu
  * @version V0.1
  * @date    11-Jan-2022
  * @brief
  *
  ******************************************************************************
  */

#include <rtthread.h>
#include "int_mux.h"

void rt_hw_interrupt_active(int vector)
{
    unsigned int index, set;
    unsigned int temp;

    index = vector / 32;
    set = vector % 32;
    temp = *(unsigned int *)(INT_MUX + index * 4);
    *(unsigned int *)(INT_MUX + index * 4) = (1 << set) | temp;
}

void rt_hw_interrupt_deactive(int vector)
{
    unsigned int index, set;

    index = vector / 32;
    set = vector % 32;
    *(unsigned int *)(INT_MUX + index * 4) &= ~(1 << set);
}

rt_uint32_t rt_hw_interrupt_get_active(void)
{
    rt_uint32_t cause = -1;
    int i, n;
    unsigned int int_reg;

    for (i = 0; i < INT_GROUP; i++)
    {
        if (*(unsigned int *)(INT_MUX + INT_MUX_GROUP + i * 4))
            break;
    }

    if (i < INT_GROUP)
    {
        int_reg = *(unsigned int *)(INT_MUX + INT_MUX_GROUP + i * 4);
        for (n = 0; n < 32; n++)
        {
            if (((int_reg >> n) & 0x1) == 0x1)
                break;
        }

        cause = i * 32 + n;
    }

    return cause;
}

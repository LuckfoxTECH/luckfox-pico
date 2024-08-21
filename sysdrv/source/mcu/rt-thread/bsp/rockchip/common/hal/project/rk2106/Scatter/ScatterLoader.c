/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020 Rockchip Electronics Co., Ltd.
 */

#define _IN_STARTLOAD_

#include "hal_def.h"
#include "ModuleInfoTab.h"

extern uint32_t Load$$MODULE_INFO$$Base;

extern uint32_t Load$$AP_SYS_CODE$$Base;
extern uint32_t Image$$AP_SYS_CODE$$Base;
extern uint32_t Image$$AP_SYS_CODE$$Length;
extern uint32_t Load$$AP_SYS_DATA$$Base;
extern uint32_t Image$$AP_SYS_DATA$$RW$$Base;
extern uint32_t Image$$AP_SYS_DATA$$RW$$Length;
extern uint32_t Image$$AP_SYS_DATA$$ZI$$Base;
extern uint32_t Image$$AP_SYS_DATA$$ZI$$Length;

extern uint32_t Load$$TASK_INF_DATA$$Base;
extern uint32_t Image$$TASK_INF_DATA$$Base;
extern uint32_t Image$$TASK_INF_DATA$$Length;
extern uint32_t Image$$TASK_INF_DATA$$ZI$$Base;
extern uint32_t Image$$TASK_INF_DATA$$ZI$$Length;

extern uint32_t Image$$IDLE_STACK$$ZI$$Base;
extern uint32_t Image$$IDLE_STACK$$ZI$$Length;
extern uint32_t Image$$IDLE_STACK$$RW$$Base;
extern uint32_t Image$$IDLE_STACK$$RW$$Length;

extern uint32_t Image$$AP_MAIN_STACK$$ZI$$Base;
extern uint32_t Image$$AP_MAIN_STACK$$ZI$$Length;
extern uint32_t Image$$AP_MAIN_STACK$$RW$$Base;
extern uint32_t Image$$AP_MAIN_STACK$$RW$$Length;

extern uint32_t Image$$APP_HEAP_STACK$$RW$$Base;
extern uint32_t Image$$APP_HEAP_STACK$$RW$$Length;
extern uint32_t Image$$APP_HEAP_STACK$$ZI$$Base;
extern uint32_t Image$$APP_HEAP_STACK$$ZI$$Length;

extern uint32_t Image$$HEAP_STACK$$RW$$Base;
extern uint32_t Image$$HEAP_STACK$$RW$$Length;
extern uint32_t Image$$HEAP_STACK$$ZI$$Base;
extern uint32_t Image$$HEAP_STACK$$ZI$$Length;

extern uint32_t Load$$SRC_FILTER_DATA$$Base;
extern uint32_t Image$$SRC_FILTER_DATA$$Base;
extern uint32_t Image$$SRC_FILTER_DATA$$Length;
extern uint32_t Image$$SRC_FILTER_DATA$$ZI$$Base;
extern uint32_t Image$$SRC_FILTER_DATA$$ZI$$Length;

__attribute__((section("_ModuleInfo_"))) FIRMWARE_INFO_T const ModuleInfo = {

    (&Load$$MODULE_INFO$$Base),

    {
        (uint32_t)(MAX_SEGMENT_NUM),
        {
            {
                (&Load$$AP_SYS_CODE$$Base),
                (&Image$$AP_SYS_CODE$$Base),
                (&Image$$AP_SYS_CODE$$Length),

                (&Load$$AP_SYS_DATA$$Base),
                (&Image$$AP_SYS_DATA$$RW$$Base),
                (&Image$$AP_SYS_DATA$$RW$$Length),

                (&Image$$AP_SYS_DATA$$ZI$$Base),
                (&Image$$AP_SYS_DATA$$ZI$$Length),
                "system",
            },

            {
                NULL,
                (&Image$$APP_HEAP_STACK$$RW$$Base),
                (&Image$$APP_HEAP_STACK$$RW$$Length),

                NULL,
                (&Image$$APP_HEAP_STACK$$ZI$$Base),
                (&Image$$APP_HEAP_STACK$$ZI$$Length),

                (&Image$$HEAP_STACK$$ZI$$Base),
                (&Image$$HEAP_STACK$$ZI$$Length),
                "stack1",
            },

            {
                NULL,
                (&Image$$AP_MAIN_STACK$$RW$$Base),
                (&Image$$AP_MAIN_STACK$$RW$$Length),

                NULL,
                (&Image$$AP_MAIN_STACK$$ZI$$Base),
                (&Image$$AP_MAIN_STACK$$ZI$$Length),

                (&Image$$HEAP_STACK$$RW$$Base),
                (&Image$$HEAP_STACK$$RW$$Length),
                "stack2",
            },

            {
                NULL,
                (&Image$$IDLE_STACK$$RW$$Base),
                (&Image$$IDLE_STACK$$RW$$Length),

                NULL,
                (&Image$$IDLE_STACK$$ZI$$Base),
                (&Image$$IDLE_STACK$$ZI$$Length),

                NULL,
                NULL,
                "stack3",
            },

            {
                (&Load$$SRC_FILTER_DATA$$Base),
                (&Image$$SRC_FILTER_DATA$$Base),
                (&Image$$SRC_FILTER_DATA$$Length),

                0,
                0,
                0,

                (&Image$$SRC_FILTER_DATA$$ZI$$Base),
                (&Image$$SRC_FILTER_DATA$$ZI$$Length),
                "src filter",
            },

        },
    },

    {
    },
};

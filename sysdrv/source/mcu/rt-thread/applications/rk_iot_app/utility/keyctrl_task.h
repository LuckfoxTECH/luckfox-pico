/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _RK_KEYCTRL_TASK_H_
#define _RK_KEYCTRL_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    KEY_CTRL_CMD_LONG_PRESS,            //  长按
    KEY_CTRL_CMD_SHORT_PRESS,           //  短按
    KEY_CTRL_CMD_REPEAT,                //  重复
    KEY_CTRL_CMD_RELEASE,               //  释放
    KEY_CTRL_CMD_PRESS_DOWN,            //  按下
    KEY_CTRL_CMD_COMBINATION,           //  组合键按下
    KEY_CTRL_CMD_NULL,
} KEY_CTRL_CMD;

typedef enum
{
    KEY_CTRL_0,
    KEY_CTRL_1,
    KEY_CTRL_2,
    KEY_CTRL_3,
    KEY_CTRL_4,
    KEY_CTRL_5,
    KEY_CTRL_6,
    KEY_CTRL_COMB,
    KEY_CTRL_NUM,
    KEY_CTRL_ALL_RELEASE,
} KEY_CTRL_ID;

#define BOARD_TEST_BUTTON_ID    KEY_CTRL_0
#define BOARD_OTA_BUTTON_ID     KEY_CTRL_1


typedef struct
{
    KEY_CTRL_CMD    cmd;
    KEY_CTRL_ID     id;
} KEYCTRL_CMD_INFO;


void key_ctrl_init();
void key_ctrl_deinit();


#ifdef __cplusplus
}
#endif

#endif /* _RK_KEYCTRL_TASK_H_ */


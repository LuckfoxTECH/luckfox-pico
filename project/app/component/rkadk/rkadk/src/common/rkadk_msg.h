/*
 * Copyright (c) 2023 Rockchip, Inc. All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "rkadk_common.h"

typedef enum {
  RKADK_CMD_SLEEP,
  RKADK_CMD_EXIT,
} RKADK_COMMAND_E;

typedef struct _RKADK_MSG {
  RKADK_U32 u32CamId;
  RKADK_U32 u32VencChn;
  RKADK_COMMAND_E command;
} RKADK_MSG;

int RKADK_MSG_Init(int msgKey, int *pMsgId);
void RKADK_MSG_Deinit(int msgKey, int *pMsgId);
int RKADK_MSG_Recv(int msgId, RKADK_MSG *msg);
int RKADK_MSG_Send(int msgId, RKADK_MSG *msg);

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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "rkadk_log.h"
#include "rkadk_msg.h"

#define RKADK_MSG_TYPE 100

typedef struct _IPC_MSG {
  long msgType;
  char msg[sizeof(RKADK_MSG)];
} RKADK_IPC_MSG;

int RKADK_MSG_GetNum(int msgId)
{
  int ret;
  struct msqid_ds msgInfo;

  if (msgId < 0) {
    RKADK_LOGE("Invalid msgId = %d", msgId);
    return -1;
  }

  ret = msgctl(msgId, IPC_STAT, &msgInfo);
  if (ret < 0) {
    RKADK_LOGE("failed to get info, errno[%d, %s]", errno, strerror(errno));
    return -1;
  }

  return msgInfo.msg_qnum;
}

int RKADK_MSG_SetNum(int msgId, int number)
{
  int ret;
  struct msqid_ds msgInfo;

  if (msgId < 0) {
    RKADK_LOGE("Invalid msgId = %d", msgId);
    return -1;
  }

  ret = msgctl(msgId, IPC_STAT, &msgInfo);
  if (ret < 0) {
    RKADK_LOGE("failed to get info, errno[%d, %s]", errno, strerror(errno));
    return -1;
  }

  msgInfo.msg_qnum = msgInfo.msg_qnum * number;
  msgInfo.msg_qbytes = msgInfo.msg_qbytes * number;

  ret = msgctl(msgId, IPC_SET, &msgInfo);
  if (ret < 0) {
    RKADK_LOGE("failed to get info, errno[%d, %s]", errno, strerror(errno));
    return -1;
  }

  ret = msgctl(msgId, IPC_STAT, &msgInfo);
  if (ret < 0) {
    RKADK_LOGE("failed to get info, errno[%d, %s]", errno, strerror(errno));
    return -1;
  }

  return msgInfo.msg_qnum;
}

int RKADK_MSG_Init(int msgKey, int *pMsgId)
{
  int msgId;
  int ret;
  int retryNum = 0;

RETRY:
  msgId = msgget(msgKey, IPC_EXCL);  /*check msg*/
  if (msgId < 0) {
    RKADK_LOGD("Create msg, key[%d] retryNum[%d]", msgKey, retryNum);
    msgId = msgget(msgKey, IPC_CREAT | 0666); /*create msg*/
    if (msgId < 0) {
      RKADK_LOGE("failed to create msq, errno[%d, %s]", errno, strerror(errno));
      return -1;
    }
  }

  ret = RKADK_MSG_GetNum(msgId);
  if (ret > 0) {
    ret = msgctl(msgId, IPC_RMID, NULL);
    if (ret < 0) {
      RKADK_LOGE("failed to remove msq[%d], errno[%d, %s]", msgId, errno, strerror(errno));
      return -1;
    }

    retryNum++;
    if (retryNum > 3) {/*retry, but the msg can't delete*/
      RKADK_LOGE("retry create msq failed, errno[%d, %s]", errno, strerror(errno));
      return -1;
    }

    goto RETRY;
  }

  *pMsgId = msgId;
  return 0;
}

void RKADK_MSG_Deinit(int msgKey, int *pMsgId)
{
  int msgId;

  /* if ipcmsg init fail, the pMsgId will set to -1, so don't deinit again */
  if (*pMsgId < 0)
    return;

  msgId = msgget(msgKey, IPC_EXCL);  /*check ipc msg*/
  if (msgId < 0) {
    RKADK_LOGE("failed to get id, errno[%d, %s]", errno, strerror(errno));
    return;
  }

  msgctl(msgId, IPC_RMID, NULL); /*remove ipc msg*/
  *pMsgId = -1;
  return;
}

int RKADK_MSG_Recv(int msgId, RKADK_MSG *msg)
{
  RKADK_IPC_MSG ipcMsgRecv;

  if (msg == NULL) {
    RKADK_LOGE("fs_msg == NULL");
    return -1;
  }

  if (msgId < 0) {
    RKADK_LOGE("Invalid msgId[%d]", msgId);
    return -1;
  }

  int ret = msgrcv(msgId, &ipcMsgRecv, sizeof(RKADK_MSG), RKADK_MSG_TYPE, 0);
  if (ret < 0) {
    RKADK_LOGE("read msg failed, errno[%d, %s]", errno, strerror(errno));
    return -1;
  }

  memcpy((char *)msg, ipcMsgRecv.msg, sizeof(RKADK_MSG));
  return 0;
}

int RKADK_MSG_Send(int msgId, RKADK_MSG *msg)
{
  RKADK_IPC_MSG ipcMsgSend;

  if (msg == NULL) {
    RKADK_LOGE("fs_msg == NULL");
    return -1;
  }

  if (msgId < 0) {
    RKADK_LOGE("Invalid msgId[%d]", msgId);
    return -1;
  }

  ipcMsgSend.msgType = RKADK_MSG_TYPE;
  memcpy(ipcMsgSend.msg, (char *)msg, sizeof(RKADK_MSG));
  int ret = msgsnd(msgId, (void *)&ipcMsgSend, sizeof(RKADK_MSG), IPC_NOWAIT);
  if (ret < 0) {
    RKADK_LOGE("send msg[%d] failed[%d] errno[%d, %s]",
              RKADK_MSG_GetNum(msgId), ret, errno, strerror(errno));
    return -1;
  }

  return 0;
}

/*
 * Copyright (C) 2019 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include <linux/netlink.h>
#include <sys/socket.h>
#include <sys/time.h>

#include "uevent.h"
#include "uvc_control.h"
#include "uvc_log.h"

extern int app_quit;

static void handle_uvc_event(struct uevent *uevent) {
  if (strcmp(uevent->subsystem, "android_usb"))
    return;

  if (!strcmp(uevent->usb_state, "DISCONNECTED")) {
    LOG_INFO("udc disconnected\n");
    app_quit = 1;
  } else if (!strcmp(uevent->usb_state, "CONNECTED")) {
    LOG_INFO("udc connected\n");
  } else if (!strcmp(uevent->usb_state, "CONFIGURED")) {
    LOG_INFO("udc configured\n");
  } else {
    LOG_INFO("unknow usb event\n");
  }
}

static void parse_event(const char *msg, struct uevent *uevent) {
  uevent->action = "";
  uevent->path = "";
  uevent->subsystem = "";
  uevent->usb_state = "";
  uevent->device_name = "";

  while (*msg) {
    if (!strncmp(msg, "ACTION=", 7)) {
      msg += 7;
      uevent->action = msg;
    } else if (!strncmp(msg, "DEVPATH=", 8)) {
      msg += 8;
      uevent->path = msg;
    } else if (!strncmp(msg, "SUBSYSTEM=", 10)) {
      msg += 10;
      uevent->subsystem = msg;
    } else if (!strncmp(msg, "USB_STATE=", 10)) {
      msg += 10;
      uevent->usb_state = msg;
    } else if (!strncmp(msg, "DEVNAME=", 8)) {
      msg += 8;
      uevent->device_name = msg;
    }
    /* advance to after the next \0 */
    while (*msg++)
      ;
  }

  LOG_DEBUG("event { '%s', '%s', '%s', '%s', '%s' }\n", uevent->action,
            uevent->path, uevent->subsystem, uevent->usb_state,
            uevent->device_name);
  handle_uvc_event(uevent);
}

static void *event_monitor_thread(void *arg) {
  int sockfd;
  int i, j, len;
  char buf[1024 + 2];
  struct iovec iov;
  struct msghdr msg;
  struct sockaddr_nl sa;
  struct uevent uevent;
  uint32_t flags = *(uint32_t *)arg;

  prctl(PR_SET_NAME, "event_monitor", 0, 0, 0);

  memset(&sa, 0, sizeof(sa));
  sa.nl_family = AF_NETLINK;
  sa.nl_groups = NETLINK_KOBJECT_UEVENT;
  sa.nl_pid = 0;
  memset(&msg, 0, sizeof(msg));
  iov.iov_base = (void *)buf;
  iov.iov_len = sizeof(buf);
  msg.msg_name = (void *)&sa;
  msg.msg_namelen = sizeof(sa);
  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;

  sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_KOBJECT_UEVENT);
  if (sockfd == -1) {
    LOG_ERROR("socket creating failed:%s\n", strerror(errno));
    goto err_event_monitor;
  }

  if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    LOG_ERROR("bind error:%s\n", strerror(errno));
    goto err_event_monitor;
  }

  while (1) {
    len = recvmsg(sockfd, &msg, 0);
    if (len < 0) {
      LOG_ERROR("receive error\n");
    } else if (len < 32 || len > sizeof(buf)) {
      LOG_INFO("invalid message");
    } else {
      buf[len] = '\0';
      buf[len + 1] = '\0';
      parse_event(buf, &uevent);
    }
  }

err_event_monitor:
  pthread_detach(pthread_self());
  pthread_exit(NULL);
}

int uevent_monitor_run(uint32_t flags) {
  pthread_t tid;

  return pthread_create(&tid, NULL, event_monitor_thread, &flags);
}

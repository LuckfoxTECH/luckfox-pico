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
//#ifdef __cplusplus

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

#include "uac_control.h"
#include "uac_log.h"
#include "uac_uevent.h"
#include <linux/netlink.h>
#include <sys/socket.h>
#include <sys/time.h>

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "audio_event"
#endif

/*
 * case 1:
 * the UAC1 uevent when pc/remote close(play sound of usb close)
 *
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0   // UAC2_Gadget
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_INTERFACE
 * strs[4] = STREAM_DIRECTION=OUT
 * strs[5] = STREAM_STATE=OFF
 *
 *
 * case 2:
 * the UAC1 uevent when pc/remote play start(play sound of usb open)
 *
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_INTERFACE
 * strs[4] = STREAM_DIRECTION=OUT
 * strs[5] = STREAM_STATE=ON
 *
 *
 * case 3:
 * the UAC1 uevent when pc/remote capture start(record sound of usb open)
 *
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_INTERFACE
 * strs[4] = STREAM_DIRECTION=IN
 * strs[5] = STREAM_STATE=ON
 *
 *
 * case 4:
 * the UAC1 uevent when pc/remote capture stop(record sound of usb open)
 *
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_INTERFACE
 * strs[4] = STREAM_DIRECTION=IN
 * strs[5] = STREAM_STATE=OFF
 *
 *
 * case 5:
 * the UAC1 uevent
 *
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_SAMPLE_RATE
 * strs[4] = STREAM_DIRECTION=IN
 * strs[5] = SAMPLE_RATE=48000
 */
#define UAC_UEVENT_AUDIO "SUBSYSTEM=u_audio"
#define UAC_UEVENT_SET_INTERFACE "USB_STATE=SET_INTERFACE"
#define UAC_UEVENT_SET_SAMPLE_RATE "USB_STATE=SET_SAMPLE_RATE"
#define UAC_UEVENT_SET_VOLUME "USB_STATE=SET_VOLUME"
#define UAC_UEVENT_SET_MUTE "USB_STATE=SET_MUTE"
#define UAC_UEVENT_SET_AUDIO_CLK "USB_STATE=SET_AUDIO_CLK"

#define UAC_STREAM_DIRECT "STREAM_DIRECTION="
#define UAC_STREAM_STATE "STREAM_STATE="
#define UAC_SAMPLE_RATE "SAMPLE_RATE="
#define UAC_SET_VOLUME "VOLUME="
#define UAC_SET_MUTE "MUTE="
#define UAC_PPM "PPM="

// remote device/pc->our device
#define UAC_REMOTE_PLAY "OUT"

// our device->remote device/pc
#define UAC_REMOTE_CAPTURE "IN"

// sound card is opened
#define UAC_STREAM_START "ON"

// sound card is closed
#define UAC_STREAM_STOP "OFF"

enum UAC_UEVENT_KEY {
  UAC_KEY_AUDIO = 2,
  UAC_KEY_USB_STATE = 3,
  UAC_KEY_DIRECTION = 4,
  UAC_KEY_PPM = 4,
  UAC_KEY_STREAM_STATE = 5,
  UAC_KEY_SAMPLE_RATE = UAC_KEY_STREAM_STATE,
  UAC_KEY_VOLUME = UAC_KEY_STREAM_STATE,
  UAC_KEY_MUTE = UAC_KEY_STREAM_STATE,
};

bool compare(const char *dst, const char *srt) {
  if ((dst == NULL) || (srt == NULL))
    return false;

  if (!strncmp(dst, srt, strlen(srt))) {
    return true;
  }

  return false;
}

void audio_play(const struct _uevent *uevent) {
  char *direct = uevent->strs[UAC_KEY_DIRECTION];
  char *status = uevent->strs[UAC_KEY_STREAM_STATE];

  if (compare(direct, UAC_STREAM_DIRECT) && compare(status, UAC_STREAM_STATE)) {
    char *device = &direct[strlen(UAC_STREAM_DIRECT)];
    char *state = &status[strlen(UAC_STREAM_STATE)];
    // remote device/pc open/close usb sound card to write data
    if (compare(device, UAC_REMOTE_PLAY)) {
      if (compare(UAC_STREAM_START, state)) {
        // stream start, we need to open usb card to record datas
        ALOGD("remote device/pc start to play data to us, we need to open usb "
              "to capture datas\n");
        uac_start(UAC_STREAM_RECORD);
      } else if (compare(UAC_STREAM_STOP, state)) {
        ALOGD("remote device/pc stop to play data to us, we need to stop "
              "capture datas\n");
        uac_stop(UAC_STREAM_RECORD);
      }
    } else if (compare(device, UAC_REMOTE_CAPTURE)) {
      // our device->remote device/pc
      if (compare(UAC_STREAM_START, state)) {
        // stream start, we need to open usb card to record datas
        ALOGD("remote device/pc start to record from us, we need to open usb "
              "to send datas\n");
        uac_start(UAC_STREAM_PLAYBACK);
      } else if (compare(UAC_STREAM_STOP, state)) {
        ALOGD("remote device/pc stop to record from us, we need to stop write "
              "datas to usb\n");
        uac_stop(UAC_STREAM_PLAYBACK);
      }
    }
  }
}

void audio_set_samplerate(const struct _uevent *uevent) {
  char *direct = uevent->strs[UAC_KEY_DIRECTION];
  char *samplerate = uevent->strs[UAC_KEY_SAMPLE_RATE];
  ALOGD("%s: %s\n", __FUNCTION__, direct);
  ALOGD("%s: %s\n", __FUNCTION__, samplerate);
  if (compare(direct, UAC_STREAM_DIRECT)) {
    char *device = &direct[strlen(UAC_STREAM_DIRECT)];
    char *rate = &samplerate[strlen(UAC_SAMPLE_RATE)];
    int sampleRate = atoi(rate);
    if (compare(device, UAC_REMOTE_PLAY)) {
      ALOGD("set samplerate %d to usb record\n", sampleRate);
      uac_set_sample_rate(UAC_STREAM_RECORD, sampleRate);
    } else if (compare(device, UAC_REMOTE_CAPTURE)) {
      ALOGD("set samplerate %d to usb playback\n", sampleRate);
      uac_set_sample_rate(UAC_STREAM_PLAYBACK, sampleRate);
    }
  }
}

/*
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devicges/virtual/u_audio/UAC1_Gadgeta 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_VOLUME
 * strs[4] = STREAM_DIRECTION=OUT
 * strs[5] = VOLUME=0x7FFF
 *    index       db
 *   0x7FFF:   127.9961
 *   ......
 *   0x0100:   1.0000
 *   ......
 *   0x0002:   0.0078
 *   0x0001:   0.0039
 *   0x0000:   0.0000
 *   0xFFFF:  -0.0039
 *   0xFFFE:  -0.0078
 *   ......
 *   0xFE00:  -1.0000
 *   ......
 *   0x8002:  -127.9922
 *   0x8001:  -127.9961
 *
 */
void audio_set_volume(const struct _uevent *uevent) {
  char *direct = uevent->strs[UAC_KEY_DIRECTION];
  char *volumeStr = uevent->strs[UAC_KEY_VOLUME];
  int unit = 0x100;
  ALOGD("direct = %s volume = %s\n", direct, volumeStr);
  if (compare(direct, UAC_STREAM_DIRECT)) {
    char *device = &direct[strlen(UAC_STREAM_DIRECT)];
    short volume = 0;
    float db = 0;
    sscanf(volumeStr, "VOLUME=0x%x", &volume);
    db = volume / (float)unit;
    double precent = pow(10, db / 10);
    int precentInt = (int)(precent * 100);
    ALOGD("set db = %f, precent = %lf, precentInt = %d\n", db, precent,
          precentInt);
    if (compare(device, UAC_REMOTE_PLAY)) {
      ALOGD("set volume %d  to usb record\n", precentInt);
      uac_set_volume(UAC_STREAM_RECORD, precentInt);
    } else if (compare(device, UAC_REMOTE_CAPTURE)) {
      ALOGD("set volume %d  to usb playback\n", precentInt);
      uac_set_volume(UAC_STREAM_PLAYBACK, precentInt);
    }
  }
}

/*
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_MUTE
 * strs[4] = STREAM_DIRECTION=OUT
 * strs[5] = MUTE=1
*/
void audio_set_mute(const struct _uevent *uevent) {
  char *direct = uevent->strs[UAC_KEY_DIRECTION];
  char *muteStr = uevent->strs[UAC_KEY_MUTE];
  ALOGD("direct = %s mute = %s\n", direct, muteStr);

  if (compare(direct, UAC_STREAM_DIRECT)) {
    char *device = &direct[strlen(UAC_STREAM_DIRECT)];
    int mute = 0;
    sscanf(muteStr, "MUTE=%d", &mute);
    if (compare(device, UAC_REMOTE_PLAY)) {
      ALOGD("set mute = %d to usb record\n", mute);
      uac_set_mute(UAC_STREAM_RECORD, mute);
    } else if (compare(device, UAC_REMOTE_CAPTURE)) {
      ALOGD("set mute = %d to usb playback\n", mute);
      uac_set_mute(UAC_STREAM_PLAYBACK, mute);
    }
  }
}

/*
 * strs[0] = ACTION=change
 * strs[1] = DEVPATH=/devices/virtual/u_audio/UAC1_Gadget 0
 * strs[2] = SUBSYSTEM=u_audio
 * strs[3] = USB_STATE=SET_AUDIO_CLK
 * strs[4] = PPM=-21
 * strs[5] = SEQNUM=1573
 */
void audio_set_ppm(const struct _uevent *uevent) {
  char *ppmStr = uevent->strs[UAC_KEY_PPM];

  if (compare(ppmStr, UAC_PPM)) {
    int ppm = 0;
    sscanf(ppmStr, "PPM=%d", &ppm);
    uac_set_ppm(UAC_STREAM_RECORD, ppm);
    uac_set_ppm(UAC_STREAM_PLAYBACK, ppm);
  }
}

void audio_event(const struct _uevent *uevent) {
  char *event = uevent->strs[UAC_KEY_USB_STATE];
  char *direct = uevent->strs[UAC_KEY_DIRECTION];
  char *status = uevent->strs[UAC_KEY_STREAM_STATE];
  ALOGD("event = %s\n", event);
  ALOGD("direct = %s\n", direct);
  ALOGD("status = %s\n", status);
  if ((event == NULL) || (direct == NULL) || (status == NULL)) {
    return;
  }

  bool setInterface = compare(event, UAC_UEVENT_SET_INTERFACE);
  bool setSampleRate = compare(event, UAC_UEVENT_SET_SAMPLE_RATE);
  bool setVolume = compare(event, UAC_UEVENT_SET_VOLUME);
  bool setMute = compare(event, UAC_UEVENT_SET_MUTE);
  bool setClk = compare(event, UAC_UEVENT_SET_AUDIO_CLK);
  if (!setInterface && !setSampleRate && !setVolume && !setMute && !setClk) {
    return;
  }

  if (setInterface) {
    ALOGD("uevent---------------audio_play\n");
    audio_play(uevent);
  } else if (setSampleRate) {
    ALOGD("uevent---------------audio_set_samplerate\n");
    audio_set_samplerate(uevent);
  } else if (setVolume) {
    ALOGD("uevent---------------setVolume\n");
    audio_set_volume(uevent);
  } else if (setMute) {
    ALOGD("uevent---------------setMute\n");
    audio_set_mute(uevent);
  } else if (setClk) {
    ALOGD("uevent---------------setClk\n");
    audio_set_ppm(uevent);
  }
}

static void parse_event(const struct _uevent *event) {
  if (event->size <= 0)
    return;

#if 0
    for (int i = 0 ; i < 10; i++) {
        if (event->strs[i] != NULL) {
            ALOGD("strs[%d] = %s\n", i, event->strs[i]);
        }
    }
#endif

  if (compare(event->strs[UAC_KEY_AUDIO], UAC_UEVENT_AUDIO)) {
    audio_event(event);
  }
}

static void *uac_event_monitor_thread(void *arg) {
  int sockfd;
  int i, j, len;
  char buf[512];
  struct iovec iov;
  struct msghdr msg;
  struct sockaddr_nl sa;
  struct _uevent event;
  // uint32_t flags = *(uint32_t *)arg;

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
    ALOGE("socket creating failed:%s\n", strerror(errno));
    goto err_event_monitor;
  }

  if (bind(sockfd, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    ALOGE("bind error:%s\n", strerror(errno));
    goto err_event_monitor;
  }

  while (1) {
    event.size = 0;
    len = recvmsg(sockfd, &msg, 0);
    if (len < 0) {
      ALOGD("receive error\n");
    } else if (len < 32 || len > sizeof(buf)) {
      ALOGD("invalid message");
    } else {
      for (i = 0, j = 0; i < len; i++) {
        if (*(buf + i) == '\0' && (i + 1) != len) {
          event.strs[j++] = buf + i + 1;
          event.size = j;
        }
      }
    }
    parse_event(&event);
  }

err_event_monitor:
  pthread_detach(pthread_self());
  pthread_exit(NULL);
}

int uac_uevent_monitor_run() {
  pthread_t tid;

  return pthread_create(&tid, NULL, uac_event_monitor_thread, NULL);
}
//#endif

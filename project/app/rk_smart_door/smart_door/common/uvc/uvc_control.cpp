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

#include "uvc_control.h"
#include "camera_control.h"
#include "uevent.h"
#include "uvc-gadget.h"
#include "uvc_configfs.h"
#include "uvc_log.h"
#include "uvc_process_unit.h"
#include "uvc_video.h"
#include <fcntl.h>
#include <map>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UVC_STREAMING_INTF_PATH                                                \
  "/sys/kernel/config/usb_gadget/rockchip/functions/uvc.gs6/streaming/"        \
  "bInterfaceNumber"

#define UVC_STREAMING_MAXPACKET_PATH                                           \
  "/sys/kernel/config/usb_gadget/rockchip/functions/uvc.gs6/"                  \
  "streaming_maxpacket"
int enable_minilog;
int rk_mpi_uvc_log_level;
int app_quit;
int uvc_depth_cnt;
int uvc_rgb_cnt;

static int (*camera_start_callback)(int fd, int width, int height, int fps,
                                    int format, int eptz);
static void (*camera_stop_callback)();
uvc_pu_control_callback_t uvc_pu_control_cb = NULL;

struct uvc_ctrl {
  int id;
  bool start;
  bool stop;
  int width;
  int height;
  int fps;
  int format;
  int eptz;
  int suspend;
  struct uvc_function_config *fc;
};

static struct uvc_ctrl uvc_ctrl[3] = {
    {-1, false, false, -1, -1, -1, 1, 0, 0, NULL},
    {-1, false, false, -1, -1, -1, 1, 0, 0, NULL},
    {-1, false, false, -1, -1, -1, 1, 0, 0, NULL}, // isp
};

static std::map<int, UVC_CTRL_INFO> gUVCIdCtlInfoMap;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int uvc_streaming_intf = -1;
static int uvc_streaming_maxpacket = 1024;

bool uvc_process_init_flag = false;
bool uvc_process_config_flag = false;

static pthread_t run_id = 0;
static bool uvc_restart = false;
static bool run_flag = true;
static uint32_t uvc_flags = UVC_CONTROL_CAMERA;

static pthread_mutex_t run_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t run_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t video_added = PTHREAD_COND_INITIALIZER;

static void query_uvc_streaming_maxpacket(void) {
  int fd;

  fd = open(UVC_STREAMING_MAXPACKET_PATH, O_RDONLY);
  if (fd >= 0) {
    char intf[32] = {0};
    read(fd, intf, sizeof(intf) - 1);
    uvc_streaming_maxpacket = atoi(intf);
    if (uvc_streaming_maxpacket < 1023)
      uvc_streaming_maxpacket = 1024;
    LOG_DEBUG("uvc_streaming_maxpacket = %d\n", uvc_streaming_maxpacket);
    close(fd);
  } else {
    LOG_ERROR("open %s failed!\n", UVC_STREAMING_MAXPACKET_PATH);
  }
}

int get_uvc_streaming_maxpacket(void) { return uvc_streaming_maxpacket; }

static void query_uvc_streaming_intf(void) {
  int fd;

  fd = open(UVC_STREAMING_INTF_PATH, O_RDONLY);
  if (fd >= 0) {
    char intf[32] = {0};
    read(fd, intf, sizeof(intf) - 1);
    uvc_streaming_intf = atoi(intf);
    LOG_DEBUG("uvc_streaming_intf = %d\n", uvc_streaming_intf);
    close(fd);
  } else {
    LOG_ERROR("open %s failed!\n", UVC_STREAMING_INTF_PATH);
  }
}

void register_uvc_pu_control_callback(uvc_pu_control_callback_t cb) {
  uvc_pu_control_cb = cb;
}

void uvc_control_start_setcallback(int (*callback)(int fd, int width,
                                                   int height, int fps,
                                                   int format, int eptz)) {
  camera_start_callback = callback;
}

void uvc_control_stop_setcallback(void (*callback)()) {
  camera_stop_callback = callback;
}

static void get_uvc_index_to_name(unsigned int index, char *name) {
  if (strstr(name, "depth") || strstr(name, "DEPTH") || strstr(name, "ir") ||
      strstr(name, "IR")) {
    uvc_depth_cnt = index;
    LOG_DEBUG("uvc_depth_cnt = %d, name:%s\n", uvc_depth_cnt, name);
  } else if (strstr(name, "rgb") || strstr(name, "RGB")) {
    uvc_rgb_cnt = index;
    LOG_DEBUG("uvc_rgb_cnt = %d, name:%s\n", uvc_rgb_cnt, name);
  }
}

int check_uvc_video_id(void) {
  struct uvc_function_config *fc[CAM_MAX_NUM];
  unsigned int i;
  for (i = 0; i < CAM_MAX_NUM; i++) {
    fc[i] = configfs_parse_uvc_function(i);
    if (fc[i]) {
      get_uvc_index_to_name(fc[i]->index, fc[i]->dev_name);
      uvc_ctrl[i].id = fc[i]->video;
      uvc_ctrl[i].fc = fc[i];
      LOG_DEBUG("uvc_function_config fc->video:%d,fc->streaming.num_formats:%d",
                fc[i]->video, uvc_ctrl[i].fc->streaming.num_formats);
    }
  }

  if (uvc_ctrl[0].id < 0 && uvc_ctrl[1].id < 0) {
    LOG_WARN("Please configure uvc...\n");
    return -1;
  }
  // query_uvc_streaming_intf();
  // query_uvc_streaming_maxpacket();
  return 0;
}

void add_uvc_video() {
  if (uvc_ctrl[0].id >= 0) {
    uvc_video_id_add(uvc_ctrl[0].fc);
    LOG_INFO("uvc_ctrl[0].id:%d, uvc_ctrl[0].fc->video:%d\n", uvc_ctrl[0].id,
             uvc_ctrl[0].fc->video);
  }

  if (uvc_ctrl[1].id >= 0) {
    uvc_video_id_add(uvc_ctrl[1].fc);
  }
}

int uvc_control_loop(void) {
  if (uvc_restart) {
    // uvc_video_id_exit_all();
    // add_uvc_video();
    uvc_restart = false;
    return 0;
  }
  if (uvc_ctrl[2].stop) {
    if (camera_stop_callback)
      camera_stop_callback();
    uvc_ctrl[2].stop = false;
  }

  if (uvc_ctrl[2].start) {
    LOG_INFO("%s: video_id:%d, width:%d,height:%d,fps:%d,format:%d,eptz:%d !\n",
             __func__, uvc_ctrl[2].id, uvc_ctrl[2].width, uvc_ctrl[2].height,
             uvc_ctrl[2].fps, uvc_ctrl[2].format, uvc_ctrl[2].eptz);
    if (camera_start_callback) {
      LOG_INFO("%s  camera_start_callback start!\n", __func__);
      camera_start_callback(uvc_ctrl[2].id, uvc_ctrl[2].width,
                            uvc_ctrl[2].height, uvc_ctrl[2].fps,
                            uvc_ctrl[2].format, uvc_ctrl[2].eptz);
    }
    // camera_control_start(uvc_ctrl[2].id, uvc_ctrl[2].width,
    // uvc_ctrl[2].height, uvc_ctrl[2].fps);
    uvc_ctrl[2].start = false;
  }

  if (uvc_ctrl[2].suspend) {
    if (access("/tmp/uvc_no_suspend", 0)) {
      sleep(5);
      if (uvc_ctrl[2].suspend) {
        if (!uvc_video_get_uvc_process(uvc_ctrl[2].id)) {
          LOG_INFO("uvc ready to suspend...\n");
          uvc_ctrl[2].suspend = 0;
          system("touch /tmp/uvc_goto_suspend");
        }
      }
    }
  }
  return 1;
}

int uvc_control_run(uint32_t flags) {
  uvc_depth_cnt = -1;
  uvc_rgb_cnt = -1;
  uvc_flags = flags;
  if ((flags & UVC_CONTROL_CHECK_STRAIGHT) || (flags & UVC_CONTROL_CAMERA)) {
    if (!check_uvc_video_id()) {
      add_uvc_video();
    }
  }

  return 0;
}

void set_uvc_control_suspend(int suspend) {
  LOG_INFO("suspend is %d\n", suspend);
  uvc_ctrl[2].suspend = suspend;
  if ((suspend == 0) && !access("/tmp/uvc_goto_suspend", 0))
    system("rm /tmp/uvc_goto_suspend");
}

// TODO: use video id start uvc_ctrl
void set_uvc_control_start(int video_id, int width, int height, int fps,
                           int format, int eptz) {
  LOG_DEBUG("%s!\n", __func__);
  if (/*uvc_video_id_get(0) == video_id*/ 1) {
    LOG_DEBUG("%s: video_id:%d, width:%d,height:%d,fps:%d,eptz:%d!\n", __func__,
              video_id, width, height, fps, eptz);
    uvc_ctrl[2].id = video_id;
    uvc_ctrl[2].width = width;
    uvc_ctrl[2].height = height;
    uvc_ctrl[2].fps = fps;
    uvc_ctrl[2].format = format;
    uvc_ctrl[2].start = true;
    uvc_ctrl[2].eptz = eptz;
  } else
    LOG_ERROR("unexpect uvc!\n");
}

// TODO: use video id stop uvc_ctrl
void set_uvc_control_stop(void) {
  LOG_INFO("%s!\n", __func__);
  uvc_ctrl[2].stop = true;
}

void set_uvc_control_restart(void) {
  if (uvc_flags & UVC_CONTROL_CAMERA) {
    LOG_INFO("received uvc to exit,restart to recovery now!\n");
    // system("killall -9 uvc_app &");
    uvc_restart = true;
  }
}

UVC_CTRL_INFO uvc_find_uvc_ctrl_info_by_id(int id) {
  int index = 0;
  for (std::map<int, UVC_CTRL_INFO>::iterator iter = gUVCIdCtlInfoMap.begin();
       iter != gUVCIdCtlInfoMap.end(); ++iter) {
    if (iter->first == id) {
      LOG_DEBUG("index:%d find uvc_enc:%p by id:%d\n", index, &iter->second,
                id);
      iter->second.index = index;
      return iter->second;
    }
    index++;
  }
  UVC_CTRL_INFO uvc_ctrl_info;
  uvc_ctrl_info.index = gUVCIdCtlInfoMap.size();
  gUVCIdCtlInfoMap.insert(
      std::map<int, UVC_CTRL_INFO>::value_type(id, uvc_ctrl_info));
  LOG_INFO("index:%d new uvc_ctrl_info:%p by id:%d\n", uvc_ctrl_info.index,
           &uvc_ctrl_info, id);

  return uvc_ctrl_info;
}

void uvc_control_config(struct uvc_device *dev) {
  pthread_mutex_lock(&lock);
  UVC_CTRL_INFO uvc_ctrl_info = uvc_find_uvc_ctrl_info_by_id(dev->video_id);
  uvc_ctrl_info.width = dev->width;
  uvc_ctrl_info.height = dev->height;
  uvc_ctrl_info.fcc = dev->fcc;
  uvc_ctrl_info.fps = dev->fps;
  uvc_ctrl_info.video_id = dev->video_id;
#if UVC_EPTZ
  uvc_ctrl_info.eptz[UVC_EPTZ_AUTO] = dev->eptz_flag; // read by cfg
  uvc_ctrl_info.eptz[UVC_EPTZ_ZOOM] = 10;
  uvc_ctrl_info.eptz[UVC_EPTZ_PAN] = 0;
  uvc_ctrl_info.eptz[UVC_EPTZ_TILT] = 0;
#endif
  camera_control_set_rate(uvc_ctrl_info.index, dev->fps);
  dev->nbufs = uvc_process_config(uvc_ctrl_info);
  if (dev->nbufs <= 0) {
    LOG_ERROR("%s fail!\n", __func__);
    abort();
  }
  LOG_INFO("uvc_enc->video_id:%d dev->nbufs:%d\n", uvc_ctrl_info.video_id,
           dev->nbufs);
  uvc_process_init_flag = true;
  uvc_process_config_flag = true;
  pthread_mutex_unlock(&lock);
}

void uvc_control_streamon(struct uvc_device *dev) {
  pthread_mutex_lock(&lock);
  UVC_CTRL_INFO uvc_ctrl_info = uvc_find_uvc_ctrl_info_by_id(dev->video_id);
  uvc_ctrl_info.width = dev->width;
  uvc_ctrl_info.height = dev->height;
  uvc_ctrl_info.fcc = dev->fcc;
  uvc_ctrl_info.fps = dev->fps;
#if UVC_EPTZ
  uvc_ctrl_info.eptz[UVC_EPTZ_AUTO] = dev->eptz_flag;
#endif
  uvc_ctrl_info.video_id = dev->video_id;
  // uvc_ctrl_info->zoom = dev->zoom_val;  // init have not zoom info
  if (uvc_process_start(uvc_ctrl_info)) {
    LOG_ERROR("%s fail!\n", __func__);
    abort();
  }
  LOG_INFO("uvc_enc->video_id:%d", uvc_ctrl_info.video_id);
  uvc_process_init_flag = true;
  pthread_mutex_unlock(&lock);
}

void uvc_control_inbuf_deinit() {
  pthread_mutex_lock(&lock);
  //    if(uvc_process_init_flag)
  //        uvc_encode_inbuf_deinit(&uvc_enc);
  pthread_mutex_unlock(&lock);
}

void uvc_control_streamoff(int video_id) {
  pthread_mutex_lock(&lock);
  if (uvc_process_init_flag) {
    UVC_CTRL_INFO uvc_ctrl_info = uvc_find_uvc_ctrl_info_by_id(video_id);
    uvc_process_stop(uvc_ctrl_info);
  }
  uvc_process_init_flag = false;
  pthread_mutex_unlock(&lock);
}

void uvc_control_delete(int video_id) {
  pthread_mutex_lock(&lock);
  if (uvc_process_config_flag) {
    UVC_CTRL_INFO uvc_ctrl_info = uvc_find_uvc_ctrl_info_by_id(video_id);
    uvc_process_delete(uvc_ctrl_info);
  }
  uvc_process_config_flag = false;
  pthread_mutex_unlock(&lock);
}

// include the eptz/ptz
#if UVC_EPTZ
void uvc_control_config_eptz(struct uvc_device *dev, UVC_EPTZ_TYPE type,
                             unsigned int value) {
  int ret = 0;

  pthread_mutex_lock(&lock);
  UVC_CTRL_INFO uvc_ctrl_info = uvc_find_uvc_ctrl_info_by_id(dev->video_id);
  uvc_ctrl_info.width = dev->width;
  uvc_ctrl_info.height = dev->height;
  uvc_ctrl_info.fcc = dev->fcc;
  uvc_ctrl_info.fps = dev->fps;
  uvc_ctrl_info.type = type;
  uvc_ctrl_info.eptz[type] = value;
  uvc_ctrl_info.video_id = dev->video_id;
  ret = uvc_process_config_eptz(uvc_ctrl_info);
  if (ret < 0) {
    LOG_ERROR("%s fail: %d!\n", __func__, ret);
    abort();
  }
  LOG_INFO("set type:%d value:%d\n", type, value);
  pthread_mutex_unlock(&lock);
}
#endif

void uvc_control_release_frame_nonlock(struct uvc_device *dev, void *frame) {
  int ret = 0;

  UVC_CTRL_INFO uvc_ctrl_info = uvc_find_uvc_ctrl_info_by_id(dev->video_id);
  ret = uvc_process_release_frame(uvc_ctrl_info, frame);
  if (ret < 0) {
    LOG_ERROR("%s fail: %d!\n", __func__, ret);
    abort();
  }
}

void uvc_control_release_frame(struct uvc_device *dev, void *frame) {
  pthread_mutex_lock(&lock);
  uvc_control_release_frame_nonlock(dev, frame);
  pthread_mutex_unlock(&lock);
}

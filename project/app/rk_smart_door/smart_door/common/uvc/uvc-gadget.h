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

#ifndef _UVC_GADGET_H_
#define _UVC_GADGET_H_

#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/usb/ch9.h>

#define UVC_EVENT_FIRST (V4L2_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_CONNECT (V4L2_EVENT_PRIVATE_START + 0)
#define UVC_EVENT_DISCONNECT (V4L2_EVENT_PRIVATE_START + 1)
#define UVC_EVENT_STREAMON (V4L2_EVENT_PRIVATE_START + 2)
#define UVC_EVENT_STREAMOFF (V4L2_EVENT_PRIVATE_START + 3)
#define UVC_EVENT_SETUP (V4L2_EVENT_PRIVATE_START + 4)
#define UVC_EVENT_DATA (V4L2_EVENT_PRIVATE_START + 5)
#define UVC_EVENT_SUSPEND (V4L2_EVENT_PRIVATE_START + 6)
#define UVC_EVENT_RESUME (V4L2_EVENT_PRIVATE_START + 7)
#define UVC_EVENT_LAST (V4L2_EVENT_PRIVATE_START + 8)

#define MAX_UVC_REQUEST_DATA_LENGTH 60

#define UVC_CTRL_INTERFACE_ID 0x00
#define UVC_CTRL_CAMERA_TERMINAL_ID 0x01
#define UVC_CTRL_PROCESSING_UNIT_ID 0x02
#define UVC_CTRL_EXTENSION_UNIT_ID 0x03
#define UVC_CTRL_OUTPUT_TERMINAL_ID 0x04
#define UVC_CTRL_XU_UNIT_ID 0x06

struct uvc_request_data {
  __s32 length;
  __u8 data[60];
};

struct uvc_event {
  union {
    enum usb_device_speed speed;
    struct usb_ctrlrequest req;
    struct uvc_request_data data;
  };
};

#define UVCIOC_SEND_RESPONSE _IOW('U', 1, struct uvc_request_data)

#define UVC_INTF_CONTROL 0
#define UVC_INTF_STREAMING 1

#ifdef __cplusplus
extern "C" {
#endif

#include "uvc_video.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "uvc_configfs.h"
#include <linux/usb/ch9.h>
#include <linux/usb/video.h>
#include <linux/videodev2.h>

#define V4L2_PIX_FMT_H265                                                      \
  v4l2_fourcc('H', '2', '6', '5') /* H265 with start codes */
#define UVC_PU_FPS_CONTROL 0xff
#define UVC_PU_ROLL_CONTROL 0xfd
#define UVC_PU_EXPOSURE_TIME_CONTROL 0xfc
#define UVC_PU_AE_MODE_CONTROL 0xfb

/* ---------------------------------------------------------------------------
 * Generic stuff
 */

enum XuCmd {
  CMD_TOOLS_CTRL_1 = 0x01,
  CMD_GET_CAMERA_VERSION,
  CMD_SET_EPTZ,
  CMD_SHUTDOWN_CAMERA,
  CMD_RESET_CAMERA,
  CMD_SET_MOTOR_RATE = 0x06,
  CMD_SET_MOTOR_BY_STEPS = 0x07,
  CMD_SET_MOTOR_BY_USER = 0x08,
  CMD_STOP_MOTOR_BY_USER = 0x09,
  CMD_SET_CAMERA_IP = 0x0a,
  CMD_SET_H265 = 0x0b,
  CMD_MAX_NUM = CMD_SET_H265,
};

/* IO methods supported */
enum io_method {
  IO_METHOD_MMAP,
  IO_METHOD_USERPTR,
  IO_METHOD_DMA_BUFF,
};

#define UVC_IO_METHOD_MMAP 0
#define UVC_IO_METHOD_USERPTR 1
#define UVC_IO_METHOD_DMA_BUFF 2

#define USE_MPI_BUF_TO_UVC_VIDEO_BUFFER 1
#define UVC_IO_METHOD UVC_IO_METHOD_USERPTR

/* Buffer representing one video frame */
struct buffer {
  struct v4l2_buffer buf;
  void *start;
  size_t length;
};

struct v4l2_buffer_info {
  // struct uvc_buffer *uvc_buf;
  void *frame; // mb
  int fd;
  int index;
  unsigned long userptr;
};

enum USB_STATE {
  USB_STATE_FIRST_GET_READY,
  USB_STATE_FIRST_GET_OK,
  USB_STATE_FIRST_SEND_OK,
  USB_STATE_NORMAL_RUN
};

/* Represents a UVC based video output device */
struct uvc_device {
  int video_id;
  /* uvc device specific */
  int uvc_fd;
  int is_streaming;
  int run_standalone;
  char *uvc_devname;
  struct uvc_function_config *fc;
  int suspend;
  int need_bypass;
  /* uvc control request specific */
  struct uvc_streaming_control probe;
  struct uvc_streaming_control commit;
  int control;
  struct uvc_request_data request_error_code;
  unsigned int brightness_val;
  unsigned short contrast_val;
  int hue_val;
  unsigned int saturation_val;
  unsigned int sharpness_val;
  unsigned int gamma_val;
  unsigned int white_balance_temperature_val;
  unsigned int white_balance_temperature_auto_val;
  unsigned int gain_val;
  unsigned int hue_auto_val;
  unsigned int zoom_val;
  int pan_val;
  int tilt_val;
  short roll_val;
  short iris_val;
  int exposure_time_val;
  unsigned char ae_mode_val;
  unsigned char power_line_frequency_val;
  unsigned char ex_tool_ctrl1[4];
  unsigned char ex_sn_data[MAX_UVC_REQUEST_DATA_LENGTH];
  unsigned char ex_ip_data[MAX_UVC_REQUEST_DATA_LENGTH]; //
  unsigned char ex_date_data[MAX_UVC_REQUEST_DATA_LENGTH];
  unsigned int eptz_flag;
  unsigned int xu_h265;

  /* uvc buffer specific */
  enum io_method io;
  struct buffer *mem;
  struct buffer *dummy_buf;
  int nbufs;
  unsigned int fcc;
  unsigned int width;
  unsigned int height;
  unsigned int fps;

  unsigned int bulk;
  uint8_t color;
  unsigned int imgsize;
  void *imgdata;

  /* USB speed specific */
  int mult;
  int burst;
  int maxpkt;
  enum usb_device_speed speed;

  /* uvc specific flags */
  int first_buffer_queued;
  int uvc_shutdown_requested;

  /* uvc buffer queue and dequeue counters */
  unsigned long long int qbuf_count;
  unsigned long long int dqbuf_count;

  /* v4l2 device hook */
  struct v4l2_device *vdev;
  uint8_t cs;
  uint8_t entity_id;
  struct v4l2_buffer ubuf;

  struct v4l2_buffer_info *vbuf_info;
  int drop_count;
  int try_count;
  enum USB_STATE usb_state;
  unsigned int first_usb_get_ready_pts;
  unsigned int first_usb_get_ok_pts;
  unsigned int first_usb_send_ok_pts;
  unsigned int first_cmd_pts;
  unsigned int stream_on_pts;
  int get_buf_count;
};

#include "uvc_control.h"

int uvc_gadget_main(struct uvc_function_config *fc);
int uvc_video_reqbufs(struct uvc_device *dev, int nbufs);
int uvc_video_stream(struct uvc_device *dev, int enable);

#ifdef __cplusplus
}
#endif

#endif /* _UVC_GADGET_H_ */

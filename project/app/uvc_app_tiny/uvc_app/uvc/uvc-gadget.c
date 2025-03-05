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

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "camera_control.h"
#include "camera_pu_control.h"
#include "uvc-gadget.h"
#include "uvc_log.h"
#include "uvc_process_unit.h"
#include "uvc_video.h"
extern void camera_control_set_zoom(int val);

/* Enable debug prints. */
//#define ENABLE_BUFFER_DEBUG
#define ENABLE_USB_REQUEST_DEBUG

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define clamp(val, min, max)                                                   \
  ({                                                                           \
    typeof(val) __val = (val);                                                 \
    typeof(min) __min = (min);                                                 \
    typeof(max) __max = (max);                                                 \
    (void)(&__val == &__min);                                                  \
    (void)(&__val == &__max);                                                  \
    __val = __val < __min ? __min : __val;                                     \
    __val > __max ? __max : __val;                                             \
  })

#define pixfmtstr(x)                                                           \
  (x) & 0xff, ((x) >> 8) & 0xff, ((x) >> 16) & 0xff, ((x) >> 24) & 0xff

static int silent = 1;
#define DBG(...)                                                               \
  do {                                                                         \
    if (silent)                                                                \
      LOG_INFO(__VA_ARGS__);                                                   \
  } while (0)

/*
 * The UVC webcam gadget kernel driver (g_webcam.ko) supports changing
 * the Brightness attribute of the Processing Unit (PU). by default. If
 * the underlying video capture device supports changing the Brightness
 * attribute of the image being acquired (like the Virtual Video, VIVI
 * driver), then we should route this UVC request to the respective
 * video capture device.
 *
 * Incase, there is no actual video capture device associated with the
 * UVC gadget and we wish to use this application as the final
 * destination of the UVC specific requests then we should return
 * pre-cooked (static) responses to GET_CUR(BRIGHTNESS) and
 * SET_CUR(BRIGHTNESS) commands to keep command verifier test tools like
 * UVC class specific test suite of USBCV, happy.
 *
 * Note that the values taken below are in sync with the VIVI driver and
 * must be changed for your specific video capture device. These values
 * also work well in case there in no actual video capture device.
 */
#define PU_BRIGHTNESS_MIN_VAL 0
#define PU_BRIGHTNESS_MAX_VAL 100
#define PU_BRIGHTNESS_STEP_SIZE 1
#define PU_BRIGHTNESS_DEFAULT_VAL 50

#define PU_CONTRAST_MIN_VAL 0
#define PU_CONTRAST_MAX_VAL 100
#define PU_CONTRAST_STEP_SIZE 1
#define PU_CONTRAST_DEFAULT_VAL 50

#define PU_HUE_MIN_VAL 0
#define PU_HUE_MAX_VAL 100
#define PU_HUE_STEP_SIZE 1
#define PU_HUE_DEFAULT_VAL 50

#define PU_SATURATION_MIN_VAL 0
#define PU_SATURATION_MAX_VAL 100
#define PU_SATURATION_STEP_SIZE 1
#define PU_SATURATION_DEFAULT_VAL 50

#define PU_SHARPNESS_MIN_VAL 0
#define PU_SHARPNESS_MAX_VAL 100
#define PU_SHARPNESS_STEP_SIZE 1
#define PU_SHARPNESS_DEFAULT_VAL 50

#define PU_GAMMA_MIN_VAL 1
#define PU_GAMMA_MAX_VAL 500
#define PU_GAMMA_STEP_SIZE 1
#define PU_GAMMA_DEFAULT_VAL 100 // 1.0

#define PU_WHITE_BALANCE_TEMPERATURE_MIN_VAL 2800
#define PU_WHITE_BALANCE_TEMPERATURE_MAX_VAL 6500
#define PU_WHITE_BALANCE_TEMPERATURE_STEP_SIZE 37
#define PU_WHITE_BALANCE_TEMPERATURE_DEFAULT_VAL 5000

#define PU_GAIN_MIN_VAL 0
#define PU_GAIN_MAX_VAL 5
#define PU_GAIN_STEP_SIZE 1
#define PU_GAIN_DEFAULT_VAL 1

#define PU_HUE_AUTO_DEFAULT_VAL 0

// ZOOM
#define CT_ZOOM_ABSOLUTE_CONTROL_MIN_VAL 10
#define CT_ZOOM_ABSOLUTE_CONTROL_MAX_VAL 50
#define CT_ZOOM_ABSOLUTE_CONTROL_STEP_SIZE 1
#define CT_ZOOM_ABSOLUTE_CONTROL_DEFAULT_VAL 10

// PANTILT
#define CT_PANTILT_ABSOLUTE_CONTROL_MIN_VAL -36000
#define CT_PANTILT_ABSOLUTE_CONTROL_MAX_VAL 36000
#define CT_PANTILT_ABSOLUTE_CONTROL_STEP_SIZE 3600
#define CT_PANTILT_ABSOLUTE_CONTROL_DEFAULT_VAL 0

// ROLL
#define CT_ROLL_ABSOLUTE_CONTROL_MIN_VAL 0
#define CT_ROLL_ABSOLUTE_CONTROL_MAX_VAL 3
#define CT_ROLL_ABSOLUTE_CONTROL_STEP_SIZE 1
#define CT_ROLL_ABSOLUTE_CONTROL_DEFAULT_VAL 0

// EXPOSURE_TIME_ABSOLUTE
#define CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_MIN_VAL 5
#define CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_MAX_VAL 2500
#define CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_STEP_SIZE 1 // 100us
#define CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_DEFAULT_VAL                          \
  312 // 5 10 20 39 78 156 312 625 1250 2500 (-11 ~ -2)

// IRIS_ABSOLUTE
#define CT_IRIS_ABSOLUTE_CONTROL_MIN_VAL 0
#define CT_IRIS_ABSOLUTE_CONTROL_MAX_VAL 10
#define CT_IRIS_ABSOLUTE_CONTROL_STEP_SIZE 1
#define CT_IRIS_ABSOLUTE_CONTROL_DEFAULT_VAL 5

#define PU_DIGITAL_MULTIPLIER_CONTROL_MIN_VAL 10
#define PU_DIGITAL_MULTIPLIER_CONTROL_MAX_VAL 50
#define PU_DIGITAL_MULTIPLIER_CONTROL_STEP_SIZE 1
#define PU_DIGITAL_MULTIPLIER_CONTROL_DEFAULT_VAL 10

#define XU_CAMERA_VERSION_DEFAULT "1100010233010110010"
#define XU_EPTZ_FLAG_DEFAULT_VAL 0
#define XU_H265_DEFAULT_VAL 0

/* ---------------------------------------------------------------------------
 * V4L2 and UVC device instances
 */

/* Represents a V4L2 based video capture device */
struct v4l2_device {
  /* v4l2 device specific */
  int v4l2_fd;
  int is_streaming;
  char *v4l2_devname;

  /* v4l2 buffer specific */
  enum io_method io;
  struct buffer *mem;
  unsigned int nbufs;

  /* v4l2 buffer queue and dequeue counters */
  unsigned long long int qbuf_count;
  unsigned long long int dqbuf_count;

  /* uvc device hook */
  struct uvc_device *udev;
};

void update_camera_ip(struct uvc_device *dev) {
  char cmd[32] = {0};
  char ip[20] = {0};
  int num =
      snprintf(ip, sizeof(ip), "%d.%d.%d.%d", dev->ex_ip_data[0],
               dev->ex_ip_data[1], dev->ex_ip_data[2], dev->ex_ip_data[3]);
  snprintf(cmd, 32, "ifconfig usb0 %d.%d.%d.%d", dev->ex_ip_data[0],
           dev->ex_ip_data[1], dev->ex_ip_data[2], dev->ex_ip_data[3]);
  // system("ifconfig usb0 down");
  LOG_DEBUG("update_camera_ip num:%d,cmd:%s\n", num, cmd);
  system(cmd);
  // system("ifconfig usb0 up");

  char *next = "/data/uvc_xu_ip_save";
  FILE *fp_output = NULL;
  strncpy(cmd, next, 32);
  cmd[strlen(next)] = '\0';
  fp_output = fopen(cmd, "w+b");
  if (NULL == fp_output) {
    LOG_ERROR("failed to open uvc xu ip file %s\n", cmd);
  } else {
    ip[num + 1] = "\0";
    fwrite(ip, num + 1, 1, fp_output);
    fclose(fp_output);
  }
}

/* ---------------------------------------------------------------------------
 * V4L2 streaming related
 */

static int v4l2_uninit_device(struct v4l2_device *dev) {
  unsigned int i;
  int ret;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    for (i = 0; i < dev->nbufs; ++i) {
      ret = munmap(dev->mem[i].start, dev->mem[i].length);
      if (ret < 0) {
        LOG_ERROR("V4L2: munmap failed\n");
        return ret;
      }
    }

    free(dev->mem);
    break;

  case IO_METHOD_USERPTR:
  default:
    break;
  }

  return 0;
}

static int v4l2_reqbufs_mmap(struct v4l2_device *dev, int nbufs) {
  struct v4l2_requestbuffers req;
  unsigned int i = 0;
  int ret;

  CLEAR(req);

  req.count = nbufs;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  ret = ioctl(dev->v4l2_fd, VIDIOC_REQBUFS, &req);
  if (ret < 0) {
    if (ret == -EINVAL)
      LOG_ERROR("V4L2: does not support memory mapping\n");
    else
      LOG_ERROR("V4L2: VIDIOC_REQBUFS error %s (%d).\n", strerror(errno),
                errno);
    goto err;
  }

  if (!req.count)
    return 0;

  if (req.count < 2) {
    LOG_ERROR("V4L2: Insufficient buffer memory.\n");
    ret = -EINVAL;
    goto err;
  }

  /* Map the buffers. */
  dev->mem = calloc(req.count, sizeof dev->mem[0]);
  if (!dev->mem) {
    LOG_ERROR("V4L2: Out of memory\n");
    ret = -ENOMEM;
    goto err;
  }

  for (i = 0; i < req.count; ++i) {
    memset(&dev->mem[i].buf, 0, sizeof(dev->mem[i].buf));

    dev->mem[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    dev->mem[i].buf.memory = V4L2_MEMORY_MMAP;
    dev->mem[i].buf.index = i;

    ret = ioctl(dev->v4l2_fd, VIDIOC_QUERYBUF, &(dev->mem[i].buf));
    if (ret < 0) {
      LOG_ERROR("V4L2: VIDIOC_QUERYBUF failed for buf %d: "
                "%s (%d).\n",
                i, strerror(errno), errno);
      ret = -EINVAL;
      goto err_free;
    }

    dev->mem[i].start = mmap(NULL /* start anywhere */, dev->mem[i].buf.length,
                             PROT_READ | PROT_WRITE /* required */,
                             MAP_SHARED /* recommended */, dev->v4l2_fd,
                             dev->mem[i].buf.m.offset);

    if (MAP_FAILED == dev->mem[i].start) {
      LOG_ERROR("V4L2: Unable to map buffer %u: %s (%d).\n", i, strerror(errno),
                errno);
      dev->mem[i].length = 0;
      ret = -EINVAL;
      goto err_free;
    }

    dev->mem[i].length = dev->mem[i].buf.length;
    LOG_DEBUG("V4L2: Buffer %u mapped at address %p.\n", i, dev->mem[i].start);
  }

  dev->nbufs = req.count;
  LOG_DEBUG("V4L2: %u buffers allocated.\n", req.count);

  return 0;

err_free:
  free(dev->mem);
err:
  return ret;
}

static int v4l2_reqbufs_userptr(struct v4l2_device *dev, int nbufs) {
  struct v4l2_requestbuffers req;
  int ret;

  CLEAR(req);

  req.count = nbufs;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_USERPTR;

  ret = ioctl(dev->v4l2_fd, VIDIOC_REQBUFS, &req);
  if (ret < 0) {
    if (ret == -EINVAL)
      LOG_ERROR("V4L2: does not support user pointer i/o\n");
    else
      LOG_ERROR("V4L2: VIDIOC_REQBUFS error %s (%d).\n", strerror(errno),
                errno);
    return ret;
  }

  dev->nbufs = req.count;
  LOG_DEBUG("V4L2: %u buffers allocated.\n", req.count);

  return 0;
}

static int v4l2_reqbufs(struct v4l2_device *dev, int nbufs) {
  int ret = 0;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    ret = v4l2_reqbufs_mmap(dev, nbufs);
    break;

  case IO_METHOD_USERPTR:
    ret = v4l2_reqbufs_userptr(dev, nbufs);
    break;

  default:
    LOG_ERROR("no support such io:%d\n", dev->io);
    ret = -EINVAL;
    break;
  }

  return ret;
}

static int v4l2_qbuf_mmap(struct v4l2_device *dev) {
  unsigned int i;
  int ret;

  for (i = 0; i < dev->nbufs; ++i) {
    memset(&dev->mem[i].buf, 0, sizeof(dev->mem[i].buf));

    dev->mem[i].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    dev->mem[i].buf.memory = V4L2_MEMORY_MMAP;
    dev->mem[i].buf.index = i;

    ret = ioctl(dev->v4l2_fd, VIDIOC_QBUF, &(dev->mem[i].buf));
    if (ret < 0) {
      LOG_ERROR("V4L2: VIDIOC_QBUF failed : %s (%d).\n", strerror(errno),
                errno);
      return ret;
    }

    dev->qbuf_count++;
  }

  return 0;
}

static int v4l2_qbuf(struct v4l2_device *dev) {
  int ret = 0;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    ret = v4l2_qbuf_mmap(dev);
    break;

  case IO_METHOD_USERPTR:
    /* Empty. */
    ret = 0;
    break;

  default:
    ret = -EINVAL;
    break;
  }

  return ret;
}

static int v4l2_process_data(struct v4l2_device *dev) {
  int ret;
  struct v4l2_buffer vbuf;
  struct v4l2_buffer ubuf;

  /* Return immediately if V4l2 streaming has not yet started. */
  if (!dev->is_streaming)
    return 0;

  if (dev->udev->first_buffer_queued)
    if (dev->dqbuf_count >= dev->qbuf_count)
      return 0;

  /* Dequeue spent buffer rom V4L2 domain. */
  CLEAR(vbuf);

  vbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  switch (dev->io) {
  case IO_METHOD_USERPTR:
    vbuf.memory = V4L2_MEMORY_USERPTR;
    break;

  case IO_METHOD_MMAP:
  default:
    vbuf.memory = V4L2_MEMORY_MMAP;
    break;
  }

  ret = ioctl(dev->v4l2_fd, VIDIOC_DQBUF, &vbuf);
  if (ret < 0)
    return ret;

  dev->dqbuf_count++;

#ifdef ENABLE_BUFFER_DEBUG
  LOG_INFO("Dequeueing buffer at V4L2 side = %d\n", vbuf.index);
#endif

  /* Queue video buffer to UVC domain. */
  CLEAR(ubuf);

  ubuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  switch (dev->udev->io) {
  case IO_METHOD_MMAP:
    ubuf.memory = V4L2_MEMORY_MMAP;
    ubuf.length = vbuf.length;
    ubuf.index = vbuf.index;
    ubuf.bytesused = vbuf.bytesused;
    break;

  case IO_METHOD_USERPTR:
  default:
    ubuf.memory = V4L2_MEMORY_USERPTR;
    ubuf.m.userptr = (unsigned long)dev->mem[vbuf.index].start;
    ubuf.length = dev->mem[vbuf.index].length;
    ubuf.index = vbuf.index;
    ubuf.bytesused = vbuf.bytesused;
    break;
  }

  ret = ioctl(dev->udev->uvc_fd, VIDIOC_QBUF, &ubuf);
  if (ret < 0) {
    LOG_ERROR("UVC: Unable to queue buffer %d: %s (%d).\n", ubuf.index,
              strerror(errno), errno);
    /* Check for a USB disconnect/shutdown event. */
    if (errno == ENODEV) {
      dev->udev->uvc_shutdown_requested = 1;
      LOG_INFO("UVC: Possible USB shutdown requested from "
               "Host, seen during VIDIOC_QBUF\n");
      return 0;
    } else {
      return ret;
    }
  }

  dev->udev->qbuf_count++;

#ifdef ENABLE_BUFFER_DEBUG
  LOG_INFO("Queueing buffer at UVC side = %d\n", ubuf.index);
#endif

  if (!dev->udev->first_buffer_queued && !dev->udev->run_standalone) {
    uvc_video_stream(dev->udev, 1);
    dev->udev->first_buffer_queued = 1;
    dev->udev->is_streaming = 1;
  }

  return 0;
}

/* ---------------------------------------------------------------------------
 * V4L2 generic stuff
 */

static int v4l2_get_format(struct v4l2_device *dev) {
  struct v4l2_format fmt;
  int ret;

  CLEAR(fmt);
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  ret = ioctl(dev->v4l2_fd, VIDIOC_G_FMT, &fmt);
  if (ret < 0) {
    LOG_ERROR("V4L2: Unable to get format: %s (%d).\n", strerror(errno), errno);
    return ret;
  }

  LOG_DEBUG("V4L2: Getting current format: %c%c%c%c %ux%u\n",
            pixfmtstr(fmt.fmt.pix.pixelformat), fmt.fmt.pix.width,
            fmt.fmt.pix.height);

  return 0;
}

static int v4l2_set_format(struct v4l2_device *dev, struct v4l2_format *fmt) {
  int ret;

  ret = ioctl(dev->v4l2_fd, VIDIOC_S_FMT, fmt);
  if (ret < 0) {
    LOG_ERROR("V4L2: Unable to set format %s (%d).\n", strerror(errno), errno);
    return ret;
  }

  LOG_DEBUG("V4L2: Setting format to: %c%c%c%c %ux%u\n",
            pixfmtstr(fmt->fmt.pix.pixelformat), fmt->fmt.pix.width,
            fmt->fmt.pix.height);

  return 0;
}

static int v4l2_set_ctrl(struct v4l2_device *dev, int new_val, int ctrl) {
  struct v4l2_queryctrl queryctrl;
  struct v4l2_control control;
  int ret;

  CLEAR(queryctrl);

  switch (ctrl) {
  case V4L2_CID_BRIGHTNESS:
    queryctrl.id = V4L2_CID_BRIGHTNESS;
    ret = ioctl(dev->v4l2_fd, VIDIOC_QUERYCTRL, &queryctrl);
    if (-1 == ret) {
      if (errno != EINVAL)
        LOG_ERROR("V4L2: VIDIOC_QUERYCTRL"
                  " failed: %s (%d).\n",
                  strerror(errno), errno);
      else
        LOG_ERROR("V4L2_CID_BRIGHTNESS is not"
                  " supported: %s (%d).\n",
                  strerror(errno), errno);

      return ret;
    } else if (queryctrl.flags & V4L2_CTRL_FLAG_DISABLED) {
      LOG_ERROR("V4L2_CID_BRIGHTNESS is not supported.\n");
      ret = -EINVAL;
      return ret;
    } else {
      CLEAR(control);
      control.id = V4L2_CID_BRIGHTNESS;
      control.value = new_val;

      ret = ioctl(dev->v4l2_fd, VIDIOC_S_CTRL, &control);
      if (-1 == ret) {
        LOG_ERROR("V4L2: VIDIOC_S_CTRL failed: %s (%d).\n", strerror(errno),
                  errno);
        return ret;
      }
    }
    LOG_INFO("V4L2: Brightness control changed to value = 0x%x\n", new_val);
    break;

  default:
    /* TODO: We don't support any other controls. */
    return -EINVAL;
  }

  return 0;
}

static int v4l2_start_capturing(struct v4l2_device *dev) {
  int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  int ret;

  ret = ioctl(dev->v4l2_fd, VIDIOC_STREAMON, &type);
  if (ret < 0) {
    LOG_ERROR("V4L2: Unable to start streaming: %s (%d).\n", strerror(errno),
              errno);
    return ret;
  }

  LOG_DEBUG("V4L2: Starting video stream.\n");

  return 0;
}

static int v4l2_stop_capturing(struct v4l2_device *dev) {
  enum v4l2_buf_type type;
  int ret;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    ret = ioctl(dev->v4l2_fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
      LOG_ERROR("V4L2: VIDIOC_STREAMOFF failed: %s (%d).\n", strerror(errno),
                errno);
      return ret;
    }

    break;
  default:
    /* Nothing to do. */
    break;
  }

  return 0;
}

static int v4l2_open(struct v4l2_device **v4l2, char *devname,
                     struct v4l2_format *s_fmt) {
  struct v4l2_device *dev;
  struct v4l2_capability cap;
  int fd;
  int ret = -EINVAL;

  fd = open(devname, O_RDWR | O_NONBLOCK, 0);
  if (fd == -1) {
    LOG_ERROR("V4L2: device open failed: %s (%d).\n", strerror(errno), errno);
    return ret;
  }

  ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
  if (ret < 0) {
    LOG_ERROR("V4L2: VIDIOC_QUERYCAP failed: %s (%d).\n", strerror(errno),
              errno);
    goto err;
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    LOG_ERROR("V4L2: %s is no video capture device\n", devname);
    goto err;
  }

  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    LOG_ERROR("V4L2: %s does not support streaming i/o\n", devname);
    goto err;
  }

  dev = calloc(1, sizeof *dev);
  if (dev == NULL) {
    ret = -ENOMEM;
    goto err;
  }

  LOG_DEBUG("V4L2 device is %s on bus %s\n", cap.card, cap.bus_info);

  dev->v4l2_fd = fd;

  /* Get the default image format supported. */
  ret = v4l2_get_format(dev);
  if (ret < 0)
    goto err_free;

  /*
   * Set the desired image format.
   * Note: VIDIOC_S_FMT may change width and height.
   */
  ret = v4l2_set_format(dev, s_fmt);
  if (ret < 0)
    goto err_free;

  /* Get the changed image format. */
  ret = v4l2_get_format(dev);
  if (ret < 0)
    goto err_free;

  LOG_DEBUG("v4l2 open succeeded, file descriptor = %d\n", fd);

  *v4l2 = dev;

  return 0;

err_free:
  free(dev);
err:
  close(fd);

  return ret;
}

static void v4l2_close(struct v4l2_device *dev) {
  close(dev->v4l2_fd);
  free(dev);
}

/* ---------------------------------------------------------------------------
 * UVC generic stuff
 */

static int uvc_video_set_format(struct uvc_device *dev) {
  struct v4l2_format fmt;
  int ret;

  CLEAR(fmt);

  fmt.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  fmt.fmt.pix.width = dev->width;
  fmt.fmt.pix.height = dev->height;
  fmt.fmt.pix.pixelformat = dev->fcc;
  fmt.fmt.pix.field = V4L2_FIELD_NONE;
  if (dev->fcc == V4L2_PIX_FMT_MJPEG)
    fmt.fmt.pix.sizeimage = dev->width * dev->height/*1.5*/;
  if ((dev->fcc == V4L2_PIX_FMT_H264) || (dev->fcc == V4L2_PIX_FMT_H265))
#ifdef RK_ENABLE_FASTBOOT
    fmt.fmt.pix.sizeimage = dev->width * dev->height / 2;
#else
    fmt.fmt.pix.sizeimage = dev->width * dev->height;
#endif
  ret = ioctl(dev->uvc_fd, VIDIOC_S_FMT, &fmt);
  if (ret < 0) {
    LOG_ERROR("UVC: Unable to set format %s (%d).\n", strerror(errno), errno);
    return ret;
  }

  LOG_INFO("UVC: Setting format to: %c%c%c%c %ux%u\n", pixfmtstr(dev->fcc),
           dev->width, dev->height);

  return 0;
}

int uvc_video_stream(struct uvc_device *dev, int enable) {
  int type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  int ret;

  if (!enable) {
    ret = ioctl(dev->uvc_fd, VIDIOC_STREAMOFF, &type);
    if (ret < 0) {
      LOG_ERROR("UVC: VIDIOC_STREAMOFF failed: %s (%d).\n", strerror(errno),
                errno);
      return ret;
    }

    LOG_DEBUG("UVC: Stopping video stream.\n");

    return 0;
  }

  ret = ioctl(dev->uvc_fd, VIDIOC_STREAMON, &type);
  if (ret < 0) {
    LOG_ERROR("UVC: Unable to start streaming %s (%d).\n", strerror(errno),
              errno);
    return ret;
  }

  LOG_DEBUG("UVC: Starting video stream.\n");

  dev->uvc_shutdown_requested = 0;

  return 0;
}

static int uvc_uninit_device(struct uvc_device *dev) {
  unsigned int i;
  int ret;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    for (i = 0; i < dev->nbufs; ++i) {
      ret = munmap(dev->mem[i].start, dev->mem[i].length);
      if (ret < 0) {
        LOG_ERROR("UVC: munmap failed\n");
        return ret;
      }
    }

    free(dev->mem);
    break;

  case IO_METHOD_USERPTR:

    if (dev->run_standalone) {
      for (i = 0; i < dev->nbufs; ++i)
        free(dev->dummy_buf[i].start);

      free(dev->dummy_buf);
    }
    break;
  case IO_METHOD_DMA_BUFF:

    break;
  default:
    break;
  }

  return 0;
}

static int uvc_open(struct uvc_device **uvc, char *devname) {
  struct uvc_device *dev;
  struct v4l2_capability cap;
  int fd;
  int ret = -EINVAL;

  fd = open(devname, O_RDWR | O_NONBLOCK);
  if (fd == -1) {
    LOG_ERROR("UVC: device open failed: %s (%d).\n", strerror(errno), errno);
    return ret;
  }

  ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
  if (ret < 0) {
    LOG_ERROR("UVC: unable to query uvc device: %s (%d)\n", strerror(errno),
              errno);
    goto err;
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)) {
    LOG_ERROR("UVC: %s is no video output device\n", devname);
    goto err;
  }

  dev = calloc(1, sizeof *dev);
  if (dev == NULL) {
    ret = -ENOMEM;
    goto err;
  }

  LOG_DEBUG("uvc device is %s on bus %s\n", cap.card, cap.bus_info);
  LOG_DEBUG("uvc open succeeded, file descriptor = %d\n", fd);

  dev->uvc_fd = fd;
  dev->eptz_flag = XU_EPTZ_FLAG_DEFAULT_VAL;
  dev->xu_h265 = XU_H265_DEFAULT_VAL;
  camera_pu_control_init(UVC_PU_BRIGHTNESS_CONTROL, PU_BRIGHTNESS_DEFAULT_VAL,
                         PU_BRIGHTNESS_MIN_VAL, PU_BRIGHTNESS_MAX_VAL);
  dev->brightness_val = camera_pu_control_get(UVC_PU_BRIGHTNESS_CONTROL,
                                              PU_BRIGHTNESS_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_CONTRAST_CONTROL, PU_CONTRAST_DEFAULT_VAL,
                         PU_CONTRAST_MIN_VAL, PU_CONTRAST_MAX_VAL);
  dev->contrast_val =
      camera_pu_control_get(UVC_PU_CONTRAST_CONTROL, PU_CONTRAST_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_HUE_CONTROL, PU_HUE_DEFAULT_VAL, PU_HUE_MIN_VAL,
                         PU_HUE_MAX_VAL);
  dev->hue_val = camera_pu_control_get(UVC_PU_HUE_CONTROL, PU_HUE_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_SATURATION_CONTROL, PU_SATURATION_DEFAULT_VAL,
                         PU_SATURATION_MIN_VAL, PU_SATURATION_MAX_VAL);
  dev->saturation_val = camera_pu_control_get(UVC_PU_SATURATION_CONTROL,
                                              PU_SATURATION_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_SHARPNESS_CONTROL, PU_SHARPNESS_DEFAULT_VAL,
                         PU_SHARPNESS_MIN_VAL, PU_SHARPNESS_MAX_VAL);
  dev->sharpness_val =
      camera_pu_control_get(UVC_PU_SHARPNESS_CONTROL, PU_SHARPNESS_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_GAMMA_CONTROL, PU_GAMMA_DEFAULT_VAL,
                         PU_GAMMA_MIN_VAL, PU_GAMMA_MAX_VAL);
  dev->gamma_val =
      camera_pu_control_get(UVC_PU_GAMMA_CONTROL, PU_GAMMA_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL,
                         PU_WHITE_BALANCE_TEMPERATURE_DEFAULT_VAL,
                         PU_WHITE_BALANCE_TEMPERATURE_MIN_VAL,
                         PU_WHITE_BALANCE_TEMPERATURE_MAX_VAL);
  dev->white_balance_temperature_val =
      camera_pu_control_get(UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL,
                            PU_WHITE_BALANCE_TEMPERATURE_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL, 1, 0,
                         1);
  dev->white_balance_temperature_auto_val =
      camera_pu_control_get(UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL, 1);

  camera_pu_control_init(UVC_PU_GAIN_CONTROL, PU_GAIN_DEFAULT_VAL,
                         PU_GAIN_MIN_VAL, PU_GAIN_MAX_VAL);
  dev->gain_val =
      camera_pu_control_get(UVC_PU_GAIN_CONTROL, PU_GAIN_DEFAULT_VAL);

  camera_pu_control_init(UVC_PU_HUE_AUTO_CONTROL, 1, 0, 1);
  dev->hue_auto_val = camera_pu_control_get(UVC_PU_HUE_AUTO_CONTROL, 1);
  camera_pu_control_set(UVC_PU_POWER_LINE_FREQUENCY_CONTROL,
                        1); // set default AntiFlickerMode 1:50hz,2:60hz
  dev->power_line_frequency_val = V4L2_CID_POWER_LINE_FREQUENCY_50HZ;

  // zoom
  dev->zoom_val = CT_ZOOM_ABSOLUTE_CONTROL_DEFAULT_VAL;
  // pan
  dev->pan_val = CT_PANTILT_ABSOLUTE_CONTROL_DEFAULT_VAL;
  // tilt
  dev->tilt_val = CT_PANTILT_ABSOLUTE_CONTROL_DEFAULT_VAL;
  // roll
  dev->roll_val = CT_ROLL_ABSOLUTE_CONTROL_DEFAULT_VAL;
  // exposure_time
  dev->exposure_time_val = CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_DEFAULT_VAL;
  // ae mode
  dev->ae_mode_val = 0x02; // Auto Mode – auto Exposure Time, auto Iris

  char *ver = XU_CAMERA_VERSION_DEFAULT;
  strncpy(dev->ex_sn_data, ver, MAX_UVC_REQUEST_DATA_LENGTH);
  memset(dev->ex_tool_ctrl1, 0, sizeof(dev->ex_tool_ctrl1));

  *uvc = dev;
  return 0;

err:
  close(fd);
  return ret;
}

static void uvc_close(struct uvc_device *dev) {
  if (dev) {
    if (dev->uvc_fd)
      close(dev->uvc_fd);
    if (dev->vbuf_info)
      free(dev->vbuf_info);
    if (dev->imgdata)
      free(dev->imgdata);
    free(dev);
  }
}

/* ---------------------------------------------------------------------------
 * UVC streaming related
 */
static void uvc_video_release_cache_buffer(struct uvc_device *dev,
                                           struct v4l2_buffer *buf) {
  uvc_user_release_cache_buffer(dev, buf, dev->video_id);
}

static struct uvc_buffer *uvc_video_fill_buffer(struct uvc_device *dev,
                                                struct v4l2_buffer *buf) {
  return uvc_user_fill_buffer(dev, buf, dev->video_id);
}

static int uvc_video_process(struct uvc_device *dev) {
  struct v4l2_buffer vbuf;
  unsigned int i;
  int ret;
  int retry_cnt = 0;
  bool get_ok = false;
  struct uvc_buffer *uvc_buf = NULL;

  /*
   * Return immediately if UVC video output device has not started
   * streaming yet.
   */
  if (!dev->is_streaming) {
    usleep(10000);
    return 0;
  }
#ifdef ENABLE_BUFFER_TIME_DEBUG
  struct timeval process_time;
  gettimeofday(&process_time, NULL);
  LOG_ERROR("UVC V4L2 READY TO WRITE BUFFER:%d.%d (s)", process_time.tv_sec,
            process_time.tv_usec);
#endif
  /* Prepare a v4l2 buffer to be dequeued from UVC domain. */
  CLEAR(dev->ubuf);

  dev->ubuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  switch (dev->io) {
  case IO_METHOD_MMAP:
    dev->ubuf.memory = V4L2_MEMORY_MMAP;
    break;

  case IO_METHOD_USERPTR:
    dev->ubuf.memory = V4L2_MEMORY_USERPTR;
    break;
  case IO_METHOD_DMA_BUFF:
  default:
    dev->ubuf.memory = V4L2_MEMORY_DMABUF;
    break;
  }

  if (dev->run_standalone) {
  REDQBUF:
    do {
      ret = ioctl(dev->uvc_fd, VIDIOC_DQBUF, &dev->ubuf);
      if (ret == 0) {
        dev->dqbuf_count++;
        uvc_video_release_cache_buffer(dev, &dev->ubuf);
        LOG_DEBUG(
            "dev->dqbuf_count:%lld, %d: DeQueued buffer at UVC side = %d\n",
            dev->dqbuf_count, dev->video_id, dev->ubuf.index);
      } else if (ret < 0) {
        break;
        // when stream on(not qbuf ever) dqbuf will go here.
      } else {
        LOG_ERROR("%d: UVC: Unable to DeQueued buffer: %s (%d) ret(%d).\n",
                  dev->video_id, strerror(errno), errno, ret);
      }
    } while (ret == 0);

    do {
      if (!uvc_buf)
        uvc_buf = uvc_video_fill_buffer(dev, &dev->ubuf);
      if (!uvc_buf) { // !dev->ubuf.bytesused && !dev->ubuf.m.fd
        dev->drop_count++;
        LOG_INFO("%d: UVC: Unable to queue buffer length is 0, dqbuf:%lld "
                 "qbuf:%lld drop count:%d\n",
                 dev->video_id, dev->dqbuf_count, dev->qbuf_count,
                 dev->drop_count);
        if (dev->qbuf_count && retry_cnt < 50) {
          retry_cnt++;
          goto REDQBUF; // ret null packet will cause uvc err between send
                        // data(need alloc a null buf fix this case?)
        } else {
          // todo: how to aviod this case?
          return 0;
        }
      }
      ret = ioctl(dev->uvc_fd, VIDIOC_QBUF, &dev->ubuf);
      if (ret < 0 && errno != 19) { // 19:No such device,disconnect
        dev->try_count++;
        LOG_ERROR("%d: UVC: Unable to queue buffer:fd:%d frame:%p %s "
                  "(%d).retry qbuf:%d\n",
                  dev->video_id, uvc_buf->fd, uvc_buf->frame, strerror(errno),
                  errno, dev->try_count);
        // uvc_buffer_read_set(dev->video_id, uvc_buf);
        // wait for moment
        usleep(1000);
        // reqbuf this uvc_buf
        goto REDQBUF;
        // return ret;  // will err if just return.
      } else {
        dev->qbuf_count++;
        uvc_buffer_write_set(dev->video_id, uvc_buf);
        // for save mb buf(when destory uvc need clear cache mb buf)
        uvc_buffer_cache_set(dev->video_id, uvc_buf);
      }
      LOG_DEBUG("dev->qbuf_count:%lld,%d: ReQueueing buffer at UVC side = %d "
                "size = %d\n",
                dev->qbuf_count, dev->video_id, dev->ubuf.index,
                dev->ubuf.bytesused);
#ifdef ENABLE_BUFFER_TIME_DEBUG
      struct timeval buffer_time;
      gettimeofday(&buffer_time, NULL);
      LOG_ERROR("UVC V4L2 BUFFER TIME END:%d.%d (s)", buffer_time.tv_sec,
                buffer_time.tv_usec);
#endif
      uvc_buf = NULL;
    } while (uvc_buffer_read_enable(dev->video_id));
  }

  return 0;
}

static int uvc_video_qbuf_mmap(struct uvc_device *dev) {
  unsigned int i;
  int ret;

  for (i = 0; i < dev->nbufs; ++i) {
    memset(&dev->mem[i].buf, 0, sizeof(dev->mem[i].buf));

    dev->mem[i].buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    dev->mem[i].buf.memory = V4L2_MEMORY_MMAP;
    dev->mem[i].buf.index = i;

    ret = ioctl(dev->uvc_fd, VIDIOC_QBUF, &(dev->mem[i].buf));
    if (ret < 0) {
      LOG_ERROR("UVC: VIDIOC_QBUF failed : %s (%d).\n", strerror(errno), errno);
      return ret;
    }

    dev->qbuf_count++;
  }

  return 0;
}

static int uvc_video_qbuf_userptr(struct uvc_device *dev) {
  unsigned int i;
  int ret;

  /* UVC standalone setup. */
  if (dev->run_standalone) {
    for (i = 0; i < dev->nbufs; ++i) {
      struct v4l2_buffer buf;

      CLEAR(buf);
      buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
      buf.memory = V4L2_MEMORY_USERPTR;
      buf.m.userptr = (unsigned long)dev->dummy_buf[i].start;
      buf.length = dev->dummy_buf[i].length;
      buf.index = i;

      ret = ioctl(dev->uvc_fd, VIDIOC_QBUF, &buf);
      if (ret < 0) {
        LOG_ERROR("UVC: VIDIOC_QBUF failed : %s (%d).\n", strerror(errno),
                  errno);
        return ret;
      }

      dev->qbuf_count++;
    }
  }

  return 0;
}

int uvc_video_qbuf(struct uvc_device *dev) {
  int ret = 0;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    ret = uvc_video_qbuf_mmap(dev);
    break;

  case IO_METHOD_USERPTR:
    ret = uvc_video_qbuf_userptr(dev);
    break;
  case IO_METHOD_DMA_BUFF:
    break;

  default:
    ret = -EINVAL;
    break;
  }

  return ret;
}

static int uvc_video_reqbufs_mmap(struct uvc_device *dev, int nbufs) {
  struct v4l2_requestbuffers rb;
  unsigned int i;
  int ret;

  CLEAR(rb);

  rb.count = nbufs;
  rb.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  rb.memory = V4L2_MEMORY_MMAP;

  ret = ioctl(dev->uvc_fd, VIDIOC_REQBUFS, &rb);
  if (ret < 0) {
    if (ret == -EINVAL)
      LOG_ERROR("UVC: does not support memory mapping\n");
    else
      LOG_ERROR("UVC: Unable to allocate buffers: %s (%d).\n", strerror(errno),
                errno);
    goto err;
  }

  if (!rb.count)
    return 0;

  if (rb.count < 2) {
    LOG_ERROR("UVC: Insufficient buffer memory.\n");
    ret = -EINVAL;
    goto err;
  }

  /* Map the buffers. */
  dev->mem = calloc(rb.count, sizeof dev->mem[0]);
  if (!dev->mem) {
    LOG_ERROR("UVC: Out of memory\n");
    ret = -ENOMEM;
    goto err;
  }

  for (i = 0; i < rb.count; ++i) {
    memset(&dev->mem[i].buf, 0, sizeof(dev->mem[i].buf));

    dev->mem[i].buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
    dev->mem[i].buf.memory = V4L2_MEMORY_MMAP;
    dev->mem[i].buf.index = i;

    ret = ioctl(dev->uvc_fd, VIDIOC_QUERYBUF, &(dev->mem[i].buf));
    if (ret < 0) {
      LOG_ERROR("UVC: VIDIOC_QUERYBUF failed for buf %d: "
                "%s (%d).\n",
                i, strerror(errno), errno);
      ret = -EINVAL;
      goto err_free;
    }

    dev->mem[i].start = mmap(NULL /* start anywhere */, dev->mem[i].buf.length,
                             PROT_READ | PROT_WRITE /* required */,
                             MAP_SHARED /* recommended */, dev->uvc_fd,
                             dev->mem[i].buf.m.offset);

    if (MAP_FAILED == dev->mem[i].start) {
      LOG_ERROR("UVC: Unable to map buffer %u: %s (%d).\n", i, strerror(errno),
                errno);
      dev->mem[i].length = 0;
      ret = -EINVAL;
      goto err_free;
    }

    dev->mem[i].length = dev->mem[i].buf.length;
    LOG_DEBUG("UVC: Buffer %u mapped at address %p.\n", i, dev->mem[i].start);
  }

  dev->nbufs = rb.count;
  LOG_DEBUG("UVC: %u buffers allocated.\n", rb.count);

  return 0;

err_free:
  free(dev->mem);
err:
  return ret;
}

static int uvc_video_reqbufs_dmabuff(struct uvc_device *dev, int nbufs) {
  struct v4l2_requestbuffers rb;
  unsigned int i, j, bpl = 0, payload_size;
  int ret;

  CLEAR(rb);

  rb.count = nbufs;
  rb.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  rb.memory = V4L2_MEMORY_DMABUF;

  ret = ioctl(dev->uvc_fd, VIDIOC_REQBUFS, &rb);
  if (ret < 0) {
    if (ret == -EINVAL)
      LOG_ERROR("UVC: does not support dma buff i/o\n");
    else
      LOG_ERROR("UVC: VIDIOC_REQBUFS error %s (%d).\n", strerror(errno), errno);
    goto err;
  }

  if (!rb.count)
    return 0;
err:
  return ret;
}

static int uvc_video_reqbufs_userptr(struct uvc_device *dev, int nbufs) {
  struct v4l2_requestbuffers rb;
  unsigned int i, j, bpl = 0, payload_size;
  int ret;

  CLEAR(rb);

  rb.count = nbufs;
  rb.type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
  rb.memory = V4L2_MEMORY_USERPTR;

  ret = ioctl(dev->uvc_fd, VIDIOC_REQBUFS, &rb);
  if (ret < 0) {
    if (ret == -EINVAL)
      LOG_ERROR("UVC: does not support user pointer i/o\n");
    else
      LOG_ERROR("UVC: VIDIOC_REQBUFS error %s (%d).\n", strerror(errno), errno);
    goto err;
  }

  if (!rb.count)
    return 0;

  dev->nbufs = rb.count;
  LOG_DEBUG("UVC: %u buffers allocated.\n", rb.count);

  if (dev->run_standalone) {
    /* Allocate buffers to hold dummy data pattern. */
    dev->dummy_buf = calloc(rb.count, sizeof dev->dummy_buf[0]);
    if (!dev->dummy_buf) {
      LOG_ERROR("UVC: Out of memory\n");
      ret = -ENOMEM;
      goto err;
    }

    switch (dev->fcc) {
    case V4L2_PIX_FMT_YUYV:
    case V4L2_PIX_FMT_NV12:
      bpl = dev->width * 2;
      payload_size = dev->width * dev->height * 2;
      break;
    case V4L2_PIX_FMT_MJPEG:
    case V4L2_PIX_FMT_H264:
    case V4L2_PIX_FMT_H265:
      payload_size = dev->imgsize;
      break;
    default:
      return -1;
    }

    for (i = 0; i < rb.count; ++i) {
      dev->dummy_buf[i].length = payload_size;
      dev->dummy_buf[i].start = malloc(payload_size);
      if (!dev->dummy_buf[i].start) {
        LOG_ERROR("UVC: Out of memory\n");
        ret = -ENOMEM;
        goto err;
      }

      if (V4L2_PIX_FMT_YUYV == dev->fcc)
        for (j = 0; j < dev->height; ++j)
          memset(dev->dummy_buf[i].start + j * bpl, dev->color++, bpl);

      if (V4L2_PIX_FMT_MJPEG == dev->fcc && dev->imgdata)
        memcpy(dev->dummy_buf[i].start, dev->imgdata, dev->imgsize);
    }
  }

  return 0;

err:
  return ret;
}

int uvc_video_reqbufs(struct uvc_device *dev, int nbufs) {
  int ret = 0;

  switch (dev->io) {
  case IO_METHOD_MMAP:
    ret = uvc_video_reqbufs_mmap(dev, nbufs);
    break;

  case IO_METHOD_USERPTR:
    ret = uvc_video_reqbufs_userptr(dev, nbufs);
    break;
  case IO_METHOD_DMA_BUFF:
    ret = uvc_video_reqbufs_dmabuff(dev, nbufs);
    break;

  default:
    ret = -EINVAL;
    break;
  }

  return ret;
}

/*
 * This function is called in response to either:
 *  - A SET_ALT(interface 1, alt setting 1) command from USB host,
 *    if the UVC gadget supports an ISOCHRONOUS video streaming endpoint
 *    or,
 *
 *  - A UVC_VS_COMMIT_CONTROL command from USB host, if the UVC gadget
 *    supports a BULK type video streaming endpoint.
 */
static int uvc_handle_streamon_event(struct uvc_device *dev) {
  int ret;
  if (!dev->run_standalone) {
    /* UVC - V4L2 integrated path. */
    if (IO_METHOD_USERPTR == dev->vdev->io) {
      /*
       * Ensure that the V4L2 video capture device has already
       * some buffers queued.
       */
      ret = v4l2_reqbufs(dev->vdev, dev->vdev->nbufs);
      if (ret < 0)
        goto err;
    }
    ret = v4l2_qbuf(dev->vdev);
    if (ret < 0)
      goto err;

    /* Start V4L2 capturing now. */
    ret = v4l2_start_capturing(dev->vdev);
    if (ret < 0)
      goto err;

    dev->vdev->is_streaming = 1;
  }

  /* Common setup. */
  if (dev->run_standalone) {
    dev->first_buffer_queued = 1;
    dev->is_streaming = 1;
  }

  uvc_control_streamon(dev);

  set_uvc_control_start(dev->video_id, dev->width, dev->height, dev->fps,
                        dev->fcc, dev->eptz_flag);
  return 0;

err:
  return ret;
}

/* ---------------------------------------------------------------------------
 * UVC Request processing
 */

static void uvc_fill_streaming_control(struct uvc_device *dev,
                                       struct uvc_streaming_control *ctrl,
                                       int iformat, int iframe,
                                       unsigned int ival) {
  const struct uvc_function_config_format *format;
  const struct uvc_function_config_frame *frame;
  unsigned int i;

  /*
   * Restrict the iformat, iframe and ival to valid values. Negative
   * values for iformat or iframe will result in the maximum valid value
   * being selected.
   */
  iformat = clamp((unsigned int)iformat, 1U, dev->fc->streaming.num_formats);
  format = &dev->fc->streaming.formats[iformat - 1];

  iframe = clamp((unsigned int)iframe, 1U, format->num_frames);
  frame = &format->frames[iframe - 1];

  for (i = 0; i < frame->num_intervals; ++i) {
    if (ival <= frame->intervals[i]) {
      ival = frame->intervals[i];
      break;
    }
  }
  if (i == frame->num_intervals)
    ival = frame->intervals[frame->num_intervals - 1];

  memset(ctrl, 0, sizeof *ctrl);

  ctrl->bmHint = 1;
  ctrl->bFormatIndex = iformat;
  ctrl->bFrameIndex = iframe;
  ctrl->dwFrameInterval = ival;

  dev->width = frame->width;
  dev->height = frame->height;
  dev->fcc = format->fcc;
  switch (format->fcc) {
  case V4L2_PIX_FMT_YUYV:
  case V4L2_PIX_FMT_NV12:
    ctrl->dwMaxVideoFrameSize = frame->width * frame->height * 2;
    break;
  case V4L2_PIX_FMT_MJPEG:
    dev->imgsize = frame->width * frame->height;
    ctrl->dwMaxVideoFrameSize = dev->imgsize;
  break;
  case V4L2_PIX_FMT_H264:
  case V4L2_PIX_FMT_H265:
#ifdef RK_ENABLE_FASTBOOT
    dev->imgsize = frame->width * frame->height / 2;
#else
    dev->imgsize = frame->width * frame->height;
#endif
    ctrl->dwMaxVideoFrameSize = dev->imgsize;
    break;
  }
  LOG_INFO("dev->fcc:%d,dev->width:%d,dev->height:%d,dev->imgsize:%d,ctrl->"
           "dwFrameInterval:%d\n",
           dev->fcc, dev->width, dev->height, dev->imgsize,
           ctrl->dwFrameInterval);

  /* TODO: the UVC maxpayload transfer size should be filled
   * by the driver.
   */
  if (!dev->bulk) {
    ctrl->dwMaxPayloadTransferSize =
        dev->fc->streaming.ep
            .wMaxPacketSize; /*get_uvc_streaming_maxpacket();/*(dev->maxpkt) *
(dev->mult + 1) * (dev->burst + 1);*/
    LOG_INFO("+++++++++dwMaxPayloadTransferSize:%d\n",
             ctrl->dwMaxPayloadTransferSize);
  } else {
    ctrl->dwMaxPayloadTransferSize = ctrl->dwMaxVideoFrameSize;
  }

  ctrl->bmFramingInfo = 3;
  ctrl->bPreferedVersion = 1;
  ctrl->bMaxVersion = 1;
}

static void uvc_events_process_standard(struct uvc_device *dev,
                                        struct usb_ctrlrequest *ctrl,
                                        struct uvc_request_data *resp) {
  LOG_DEBUG("standard request\n");
  (void)dev;
  (void)ctrl;
  (void)resp;
}

static void uvc_events_process_control(struct uvc_device *dev, uint8_t req,
                                       uint8_t cs, uint8_t entity_id,
                                       uint8_t len,
                                       struct uvc_request_data *resp) {
  LOG_DEBUG("req = %d cs = %d entity_id =%d len = %d \n", req, cs, entity_id,
            len);
  dev->cs = cs;
  dev->entity_id = entity_id;

  switch (entity_id) {
  case UVC_CTRL_INTERFACE_ID:
    switch (cs) {
    case UVC_VC_REQUEST_ERROR_CODE_CONTROL:
      /* Send the request error code last prepared. */
      resp->data[0] = dev->request_error_code.data[0];
      resp->length = dev->request_error_code.length;
      break;

    default:
      /*
       * If we were not supposed to handle this
       * 'cs', prepare an error code response.
       */
      dev->request_error_code.data[0] = 0x06;
      dev->request_error_code.length = 1;
      break;
    }
    break;

  /* Camera terminal unit 'UVC_VC_INPUT_TERMINAL'. */
  case UVC_CTRL_CAMERA_TERMINAL_ID:
    switch (cs) {
    case UVC_CT_ZOOM_ABSOLUTE_CONTROL:
      switch (req) {
        LOG_INFO("UVC_CT_ZOOM_ABSOLUTE_CONTROL:req=%d \n", req);
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = CT_ZOOM_ABSOLUTE_CONTROL_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = CT_ZOOM_ABSOLUTE_CONTROL_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->zoom_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = CT_ZOOM_ABSOLUTE_CONTROL_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = CT_ZOOM_ABSOLUTE_CONTROL_STEP_SIZE; // must be 1
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_CT_PANTILT_ABSOLUTE_CONTROL:
      switch (req) {
        LOG_INFO("UVC_CT_PANTILT_ABSOLUTE_CONTROL:req=%d len:%d\n", req, len);
      case UVC_SET_CUR:
        // resp->data[0] = 0x0;
        // resp->data[4] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN: {
        int a = CT_PANTILT_ABSOLUTE_CONTROL_MIN_VAL;
        int b = CT_PANTILT_ABSOLUTE_CONTROL_MIN_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &a, 4);
        memcpy(&resp->data[4], &b, 4);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_MAX: {
        int c = CT_PANTILT_ABSOLUTE_CONTROL_MAX_VAL;
        int d = CT_PANTILT_ABSOLUTE_CONTROL_MAX_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &c, 4);
        memcpy(&resp->data[4], &d, 4);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_CUR: {
        resp->length = len;
        memset(resp->data, 0, sizeof(resp->data));
        memcpy(&resp->data[0], &dev->pan_val, 4);
        memcpy(&resp->data[4], &dev->tilt_val, 4);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_INFO:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x03;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES: {
        int e = CT_PANTILT_ABSOLUTE_CONTROL_STEP_SIZE;
        int f = CT_PANTILT_ABSOLUTE_CONTROL_STEP_SIZE;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &e, 4);
        memcpy(&resp->data[4], &f, 4);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_CT_ROLL_ABSOLUTE_CONTROL:
      switch (req) {
        LOG_DEBUG("UVC_CT_ROLL_ABSOLUTE_CONTROL:req=%d len:%d\n", req, len);
      case UVC_SET_CUR:
        // resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN: {
        short min = CT_ROLL_ABSOLUTE_CONTROL_MIN_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &min, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_MAX: {
        short max = CT_ROLL_ABSOLUTE_CONTROL_MAX_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &max, resp->length);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_CUR:
        resp->length = len;
        memcpy(&resp->data[0], &dev->roll_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = CT_ROLL_ABSOLUTE_CONTROL_STEP_SIZE;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_CT_AE_MODE_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        /* Incase of auto exposure, attempts to
         * programmatically set the auto-adjusted
         * controls are ignored.
         */
        resp->data[0] = 0x01;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;

      case UVC_GET_INFO:
        /*
         * TODO: We support Set and Get requests, but
         * don't support async updates on an video
         * status (interrupt) endpoint as of
         * now.
         */
        resp->data[0] = 0x03;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;

      case UVC_GET_CUR:
        resp->length = 1;
        /* Auto Mode - auto Exposure Time, auto Iris. */
        memcpy(&resp->data[0], &dev->ae_mode_val, resp->length);
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
      case UVC_GET_RES:
        /* Auto Mode - auto Exposure Time, auto Iris. */
        resp->data[0] = 0x02;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        /*
         * We don't support this control, so STALL the
         * control ep.
         */
        resp->length = -EL2HLT;
        /*
         * For every unsupported control request
         * set the request error code to appropriate
         * value.
         */
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_CT_FOCUS_AUTO_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        /* Incase of auto exposure, attempts to
         * programmatically set the auto-adjusted
         * controls are ignored.
         */
        resp->data[0] = 0x01;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;

      case UVC_GET_INFO:
        /*
         * TODO: We support Set and Get requests, but
         * don't support async updates on an video
         * status (interrupt) endpoint as of
         * now.
         */
        resp->data[0] = 0x03;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;

      case UVC_GET_CUR:
      case UVC_GET_DEF:
      case UVC_GET_RES:
        resp->data[0] = 1;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        /*
         * We don't support this control, so STALL the
         * control ep.
         */
        resp->length = -EL2HLT;
        /*
         * For every unsupported control request
         * set the request error code to appropriate
         * value.
         */
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_CT_FOCUS_ABSOLUTE_CONTROL: {
      /*
      * We don't support this control, so STALL the
      * control ep.
      */
      resp->length = -EL2HLT;
      /*
       * For every unsupported control request
       * set the request error code to appropriate
       * value.
       */
      dev->request_error_code.data[0] = 0x07;
      dev->request_error_code.length = 1;
    } break;
    case UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL:
      LOG_DEBUG("++++++++UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL req:%d", req);
      switch (req) {
      case UVC_SET_CUR:
        // resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN: {
        int min_time = CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_MIN_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &min_time, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_MAX: {
        int max_time = CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_MAX_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &max_time, resp->length);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_CUR:
        resp->length = len;
        memcpy(&resp->data[0], &dev->exposure_time_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_STEP_SIZE;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF: {
        int def_time = CT_EXPOSURE_TIME_ABSOLUTE_CONTROL_DEFAULT_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &def_time, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      default:
        /*
         * We don't support this control, so STALL the
         * control ep.
         */
        resp->length = -EL2HLT;
        /*
         * For every unsupported control request
         * set the request error code to appropriate
         * value.
         */
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
      }
      break;
    case UVC_CT_IRIS_ABSOLUTE_CONTROL:
      LOG_DEBUG("++++++++UVC_CT_IRIS_ABSOLUTE_CONTROL req:%d", req);
      switch (req) {
      case UVC_SET_CUR:
        // resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN: {
        int min_time = CT_IRIS_ABSOLUTE_CONTROL_MIN_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &min_time, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_MAX: {
        int max_time = CT_IRIS_ABSOLUTE_CONTROL_MAX_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &max_time, resp->length);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_CUR:
        resp->length = len;
        memcpy(&resp->data[0], &dev->iris_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = CT_IRIS_ABSOLUTE_CONTROL_STEP_SIZE;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF: {
        int def_time = CT_IRIS_ABSOLUTE_CONTROL_DEFAULT_VAL;
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        memcpy(&resp->data, &def_time, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      default:
        /*
         * We don't support this control, so STALL the
         * control ep.
         */
        resp->length = -EL2HLT;
        /*
         * For every unsupported control request
         * set the request error code to appropriate
         * value.
         */
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
      }
      break;

    default:
      LOG_DEBUG("++++++++Input Terminal usb default req ,cs:%d", cs);
      switch (req) {
      case UVC_GET_LEN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = sizeof(resp->data);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    }
    break;

  /* processing unit 'UVC_VC_PROCESSING_UNIT' */
  case UVC_CTRL_PROCESSING_UNIT_ID:
    switch (cs) {
    case UVC_PU_BRIGHTNESS_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        LOG_INFO("set brightness\n");
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_BRIGHTNESS_MIN_VAL;
        resp->length = 2;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_BRIGHTNESS_MAX_VAL;
        resp->length = 2;
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->brightness_val, resp->length);
        /*
         * For every successfully handled control
         * request set the request error code to no
         * error
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        /*
         * We support Set and Get requests and don't
         * support async updates on an interrupt endpt
         */
        resp->data[0] = 0x03;
        resp->length = 1;
        /*
         * For every successfully handled control
         * request, set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_BRIGHTNESS_DEFAULT_VAL;
        resp->length = 2;
        /*
         * For every successfully handled control
         * request, set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_BRIGHTNESS_STEP_SIZE;
        resp->length = 2;
        /*
         * For every successfully handled control
         * request, set the request error code to no
         * error.
         */
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        /*
         * We don't support this control, so STALL the
         * default control ep.
         */
        resp->length = -EL2HLT;
        /*
         * For every unsupported control request
         * set the request error code to appropriate
         * code.
         */
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_CONTRAST_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_CONTRAST_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_CONTRAST_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->contrast_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_CONTRAST_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_CONTRAST_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_HUE_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_HUE_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_HUE_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->hue_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_HUE_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_HUE_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_SATURATION_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_SATURATION_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_SATURATION_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->saturation_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_SATURATION_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_SATURATION_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_SHARPNESS_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_SHARPNESS_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_SHARPNESS_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->sharpness_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_SHARPNESS_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_SHARPNESS_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_GAMMA_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_GAMMA_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_GAMMA_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->gamma_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_GAMMA_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_GAMMA_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN: {
        int wbt_min = PU_WHITE_BALANCE_TEMPERATURE_MIN_VAL;
        resp->length = 2;
        memcpy(&resp->data[0], &wbt_min, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_MAX: {
        int wbt_max = PU_WHITE_BALANCE_TEMPERATURE_MAX_VAL;
        resp->length = 2;
        memcpy(&resp->data[0], &wbt_max, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->white_balance_temperature_val,
               resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF: {
        int wbt_def = PU_WHITE_BALANCE_TEMPERATURE_DEFAULT_VAL;
        resp->length = 2;
        memcpy(&resp->data[0], &wbt_def, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      case UVC_GET_RES: {
        int wbt_res = PU_WHITE_BALANCE_TEMPERATURE_STEP_SIZE;
        resp->length = 2;
        memcpy(&resp->data[0], &wbt_res, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      }
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 1;
        resp->data[0] = dev->white_balance_temperature_auto_val;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        break;
      case UVC_GET_DEF:
        resp->data[0] = 1;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_GAIN_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_GAIN_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_GAIN_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->gain_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_GAIN_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_GAIN_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_HUE_AUTO_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 1;
        resp->data[0] = dev->hue_auto_val;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = 1;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_POWER_LINE_FREQUENCY_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 1;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = 2;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 1;
        memcpy(&resp->data[0], &dev->power_line_frequency_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 3;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = 1;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case UVC_PU_DIGITAL_MULTIPLIER_CONTROL:
      switch (req) {
      case UVC_SET_CUR:
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        resp->data[0] = PU_DIGITAL_MULTIPLIER_CONTROL_MIN_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        resp->data[0] = PU_DIGITAL_MULTIPLIER_CONTROL_MAX_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = 2;
        memcpy(&resp->data[0], &dev->zoom_val, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        resp->data[0] = PU_DIGITAL_MULTIPLIER_CONTROL_DEFAULT_VAL;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        resp->data[0] = PU_DIGITAL_MULTIPLIER_CONTROL_STEP_SIZE;
        resp->length = 2;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    default:
      LOG_DEBUG("++++++++Processing unit usb default req ,cs:%d", cs);
      switch (req) {
      case UVC_GET_LEN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = sizeof(resp->data);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    }

    break;

  /* Extension unit 'UVC_VC_Extension_Unit'. */
  case UVC_CTRL_XU_UNIT_ID:
    switch (cs) {
    case CMD_TOOLS_CTRL_1:
      switch (req) {
      case UVC_GET_LEN:
        resp->data[0] = 0x4;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length =
            len < sizeof(dev->ex_tool_ctrl1) ? len : sizeof(dev->ex_tool_ctrl1);
        memcpy(resp->data, dev->ex_tool_ctrl1, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
    case CMD_GET_CAMERA_VERSION:
      switch (req) {
      case UVC_GET_LEN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = sizeof(resp->data);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length =
            len < sizeof(dev->ex_sn_data) ? len : sizeof(dev->ex_sn_data);
        memcpy(resp->data, dev->ex_sn_data, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;

    case CMD_SET_CAMERA_IP:
      switch (req) {
      case UVC_GET_LEN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = sizeof(resp->data);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length =
            len < sizeof(dev->ex_ip_data) ? len : sizeof(dev->ex_ip_data);
        memcpy(resp->data, dev->ex_ip_data, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;

    case CMD_SET_H265:
      switch (req) {
      case UVC_GET_LEN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = sizeof(resp->data);
        resp->length = 4;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = 4;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = 4;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = 4;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        resp->length = len < sizeof(dev->xu_h265) ? len : sizeof(dev->xu_h265);
        memcpy(&resp->data[0], &dev->xu_h265, resp->length);
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = 4;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;

    default:
      LOG_DEBUG("+++++++++Extension unit usb default req ,cs:%d", cs);
      switch (req) {
      case UVC_GET_LEN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = sizeof(resp->data);
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_SET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MIN:
        memset(resp->data, 0, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_MAX:
        memset(resp->data, 0xFF, sizeof(resp->data));
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_CUR:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0x0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_INFO:
        resp->data[0] = 0x03;
        resp->length = 1;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_DEF:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 0;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      case UVC_GET_RES:
        memset(resp->data, 0, sizeof(resp->data));
        resp->data[0] = 1;
        resp->length = len;
        dev->request_error_code.data[0] = 0x00;
        dev->request_error_code.length = 1;
        break;
      default:
        resp->length = -EL2HLT;
        dev->request_error_code.data[0] = 0x07;
        dev->request_error_code.length = 1;
        break;
      }
      break;
      /*
       * We don't support this control, so STALL the control
       * ep.
      resp->length = -EL2HLT;
      /*
       * If we were not supposed to handle this
       * 'cs', prepare a Request Error Code response.
      dev->request_error_code.data[0] = 0x06;
      dev->request_error_code.length = 1;
      break;
       */
    }
    break;

  default:
    /*
     * If we were not supposed to handle this
     * 'cs', prepare a Request Error Code response.
     */
    dev->request_error_code.data[0] = 0x06;
    dev->request_error_code.length = 1;
    break;
  }
  if (resp->length == -EL2HLT) {
    LOG_DEBUG("unsupported: req=%02x, cs=%d, entity_id=%d, len=%d\n", req, cs,
              entity_id, len);
    resp->length = 0;
  }
  LOG_DEBUG("control request (req %02x cs %02x)\n", req, cs);
}

static void uvc_events_process_streaming(struct uvc_device *dev, uint8_t req,
                                         uint8_t cs,
                                         struct uvc_request_data *resp) {
  struct uvc_streaming_control *ctrl;

  LOG_DEBUG("streaming request (req %02x cs %02x)\n", req, cs);

  if (cs != UVC_VS_PROBE_CONTROL && cs != UVC_VS_COMMIT_CONTROL)
    return;

  ctrl = (struct uvc_streaming_control *)&resp->data;
  resp->length = sizeof *ctrl;

  switch (req) {
  case UVC_SET_CUR:
    dev->control = cs;
    resp->length = 34;
    break;

  case UVC_GET_CUR:
    if (cs == UVC_VS_PROBE_CONTROL)
      memcpy(ctrl, &dev->probe, sizeof *ctrl);
    else
      memcpy(ctrl, &dev->commit, sizeof *ctrl);
#if 1
    LOG_DEBUG("bmHint: %u\n", ctrl->bmHint);
    LOG_DEBUG("bFormatIndex: %u\n", ctrl->bFormatIndex);
    LOG_DEBUG("bFrameIndex: %u\n", ctrl->bFrameIndex);
    LOG_DEBUG("dwFrameInterval: %u\n", ctrl->dwFrameInterval);
    LOG_DEBUG("wKeyFrameRate: %u\n", ctrl->wKeyFrameRate);
    LOG_DEBUG("wPFrameRate: %u\n", ctrl->wPFrameRate);
    LOG_DEBUG("wCompQuality: %u\n", ctrl->wCompQuality);
    LOG_DEBUG("wCompWindowSize: %u\n", ctrl->wCompWindowSize);
    LOG_DEBUG("wDelay: %u\n", ctrl->wDelay);
    LOG_DEBUG("dwMaxVideoFrameSize: %u\n", ctrl->dwMaxVideoFrameSize);
    LOG_DEBUG("dwMaxPayloadTransferSize: %u\n", ctrl->dwMaxPayloadTransferSize);
    LOG_DEBUG("dwClockFrequency: %u\n", ctrl->dwClockFrequency);
    LOG_DEBUG("bmFramingInfo: %u\n", ctrl->bmFramingInfo);
    LOG_DEBUG("bPreferedVersion: %u\n", ctrl->bPreferedVersion);
    LOG_DEBUG("bMinVersion: %u\n", ctrl->bMinVersion);
    LOG_DEBUG("bMaxVersion: %u\n", ctrl->bMaxVersion);
#endif
    break;

  case UVC_GET_MIN:
  case UVC_GET_MAX:
  case UVC_GET_DEF:
    if (req == UVC_GET_MAX)
      uvc_fill_streaming_control(dev, ctrl, -1, -1, UINT_MAX);
    else
      uvc_fill_streaming_control(dev, ctrl, 1, 1, 0);
    break;

  case UVC_GET_RES:
    CLEAR(ctrl);
    break;

  case UVC_GET_LEN:
    resp->data[0] = 0x00;
    resp->data[1] = 0x22;
    resp->length = 2;
    break;

  case UVC_GET_INFO:
    resp->data[0] = 0x03;
    resp->length = 1;
    break;
  }
}

static void uvc_events_process_class(struct uvc_device *dev,
                                     struct usb_ctrlrequest *ctrl,
                                     struct uvc_request_data *resp) {
  if ((ctrl->bRequestType & USB_RECIP_MASK) != USB_RECIP_INTERFACE)
    return;
  unsigned int interface = ctrl->wIndex & 0xff;

  if (interface == dev->fc->control.intf.bInterfaceNumber) {
    uvc_events_process_control(dev, ctrl->bRequest, ctrl->wValue >> 8,
                               ctrl->wIndex >> 8, ctrl->wLength, resp);
  } else if (interface == dev->fc->streaming.intf.bInterfaceNumber) {
    uvc_events_process_streaming(dev, ctrl->bRequest, ctrl->wValue >> 8, resp);
  }
}

static void uvc_events_process_setup(struct uvc_device *dev,
                                     struct usb_ctrlrequest *ctrl,
                                     struct uvc_request_data *resp) {
  dev->control = 0;

#ifdef ENABLE_USB_REQUEST_DEBUG
  LOG_DEBUG("\nbRequestType %02x bRequest %02x wValue %04x wIndex %04x "
            "wLength %04x\n",
            ctrl->bRequestType, ctrl->bRequest, ctrl->wValue, ctrl->wIndex,
            ctrl->wLength);
#endif

  switch (ctrl->bRequestType & USB_TYPE_MASK) {
  case USB_TYPE_STANDARD:
    uvc_events_process_standard(dev, ctrl, resp);
    break;

  case USB_TYPE_CLASS:
    uvc_events_process_class(dev, ctrl, resp);
    break;

  default:
    break;
  }
}

static int uvc_events_process_control_data(struct uvc_device *dev, uint8_t cs,
                                           uint8_t entity_id,
                                           struct uvc_request_data *data) {
  unsigned int *val = (unsigned int *)data->data;
  LOG_DEBUG(" data = %d, length = %d  , current_cs = %d\n", *val, data->length,
            dev->cs);
  switch (entity_id) {
  case 1:
    switch (cs) {
    case UVC_CT_EXPOSURE_TIME_ABSOLUTE_CONTROL:
      if (sizeof(dev->exposure_time_val) >= data->length) {
        memcpy(&dev->exposure_time_val, data->data, data->length);
        LOG_INFO("set exposure_time :%d \n", dev->exposure_time_val);
        camera_pu_control_set(UVC_PU_EXPOSURE_TIME_CONTROL,
                              dev->exposure_time_val);
      }
      break;
    case UVC_CT_IRIS_ABSOLUTE_CONTROL:
      if (sizeof(dev->iris_val) >= data->length) {
        memcpy(&dev->iris_val, data->data, data->length);
        LOG_INFO("set iris value :%d \n", dev->iris_val);
      }
      break;
    case UVC_CT_AE_MODE_CONTROL:
      if (sizeof(dev->ae_mode_val) >= data->length) {
        memcpy(&dev->ae_mode_val, data->data, data->length);
        LOG_INFO("set AE Mode :%d \n", dev->ae_mode_val);
        camera_pu_control_set(UVC_PU_AE_MODE_CONTROL, dev->ae_mode_val);
      }
      break;

    default:
      break;
    }

    break;
  /* Processing unit 'UVC_VC_PROCESSING_UNIT'. */
  case 2:
    switch (cs) {
    case UVC_PU_BRIGHTNESS_CONTROL:
      if (sizeof(dev->brightness_val) >= data->length) {
        memcpy(&dev->brightness_val, data->data, data->length);
      }
      if (!dev->run_standalone)
        /*
         * Try to change the Brightness attribute on
         * Video capture device. Note that this try may
         * succeed or end up with some error on the
         * video capture side. By default to keep tools
         * like USBCV's UVC test suite happy, we are
         * maintaining a local copy of the current
         * brightness value in 'dev->brightness_val'
         * variable and we return the same value to the
         * Host on receiving a GET_CUR(BRIGHTNESS)
         * control request.
         *
         * FIXME: Keeping in view the point discussed
         * above, notice that we ignore the return value
         * from the function call below. To be strictly
         * compliant, we should return the same value
         * accordingly.
         */
        v4l2_set_ctrl(dev->vdev, dev->brightness_val, V4L2_CID_BRIGHTNESS);

      break;
    case UVC_PU_CONTRAST_CONTROL:
      LOG_INFO("UVC_PU_CONTRAST_CONTROL receive\n");
      if (sizeof(dev->contrast_val) >= data->length) {
        memcpy(&dev->contrast_val, data->data, data->length);
        LOG_INFO("UVC_PU_CONTRAST_CONTROL: 0x%02x 0x%02x\n", data->data[0],
                 data->data[1]);
      }
      break;
    case UVC_PU_HUE_CONTROL:
      if (sizeof(dev->hue_val) >= data->length) {
        memcpy(&dev->hue_val, data->data, data->length);
      }
      break;
    case UVC_PU_SATURATION_CONTROL:
      if (sizeof(dev->saturation_val) >= data->length) {
        memcpy(&dev->saturation_val, data->data, data->length);
      }
      break;
    case UVC_PU_SHARPNESS_CONTROL:
      if (sizeof(dev->sharpness_val) >= data->length)
        memcpy(&dev->sharpness_val, data->data, data->length);
      break;
    case UVC_PU_GAMMA_CONTROL:
      if (sizeof(dev->gamma_val) >= data->length)
        memcpy(&dev->gamma_val, data->data, data->length);
      break;
    case UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL:
      LOG_INFO("UVC_PU_WHITE_BALANCE_TEMPERATURE_CONTROL: 0x%02x 0x%02x\n",
               data->data[0], data->data[1]);
      if (sizeof(dev->white_balance_temperature_val) >= data->length) {
        memcpy(&dev->white_balance_temperature_val, data->data, data->length);
      }
      break;
    case UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL:
      LOG_INFO("UVC_PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL: 0x%02x\n",
               data->data[0]);
      if (sizeof(dev->white_balance_temperature_auto_val) >= data->length) {
        memcpy(&dev->white_balance_temperature_auto_val, data->data,
               data->length);
      }
      break;
    case UVC_PU_GAIN_CONTROL:
      if (sizeof(dev->gain_val) >= data->length)
        memcpy(&dev->gain_val, data->data, data->length);
      break;
    case UVC_PU_HUE_AUTO_CONTROL:
      if (sizeof(dev->hue_auto_val) >= data->length)
        memcpy(&dev->hue_auto_val, data->data, data->length);
      break;
    case UVC_PU_POWER_LINE_FREQUENCY_CONTROL:
      if (sizeof(dev->power_line_frequency_val) >= data->length) {
        memcpy(&dev->power_line_frequency_val, data->data, data->length);
      }
      break;
    case UVC_PU_DIGITAL_MULTIPLIER_CONTROL:
      if (sizeof(dev->zoom_val) >= data->length) {
        memcpy(&dev->zoom_val, data->data, data->length);
        camera_control_set_zoom(dev->zoom_val);
      }
      break;
    default:
      break;
    }
    camera_pu_control_set(cs, *val);
    if (uvc_pu_control_cb)
      uvc_pu_control_cb(cs, *val);

    break;
  /* Extension unit 'UVC_VC_Extension_Unit'. */
  case 6:
    switch (cs) {
    case CMD_TOOLS_CTRL_1:
      if (sizeof(dev->ex_tool_ctrl1) >= data->length) {
        memset(dev->ex_tool_ctrl1, 0, sizeof(dev->ex_tool_ctrl1));
        memcpy(dev->ex_tool_ctrl1, data->data, data->length);
        LOG_INFO("Extension:CMD_TOOLS_CTRL_1 set cur data: 0x%02x 0x%02x "
                 "0x%02x 0x%02x\n",
                 data->data[0], data->data[1], data->data[2], data->data[3]);
        unsigned int command = 0;
        memcpy(&command, dev->ex_tool_ctrl1, sizeof(command));
        if (command == 0xFFFFFFFF) {
          LOG_INFO("recive host reboot loader cmd");
          // system("reboot loader &");
          system("touch /tmp/reboot_loader &");
        }
      }
      break;

    case CMD_GET_CAMERA_VERSION:
      if (sizeof(dev->ex_sn_data) >= data->length) {
        memset(dev->ex_sn_data, 0, sizeof(dev->ex_sn_data));
        memcpy(dev->ex_sn_data, data->data, data->length);
        LOG_INFO("Extension:CMD_GET_CAMERA_VERSION set cur data: 0x%02x 0x%02x "
                 "0x%02x\n",
                 data->data[0], data->data[1], data->data[2]);
      }
      break;

    case CMD_SET_CAMERA_IP:
      if (sizeof(dev->ex_ip_data) >= data->length) {
        memcpy(dev->ex_ip_data, data->data, data->length);
        LOG_INFO("CMD_SET_CAMERA_IP : %d.%d.%d.%d \n", dev->ex_ip_data[0],
                 dev->ex_ip_data[1], dev->ex_ip_data[2], dev->ex_ip_data[3]);
        update_camera_ip(dev);
      }
      break;

    case CMD_SET_H265:
      if (sizeof(dev->xu_h265) >= data->length) {
        memcpy(&dev->xu_h265, data->data, data->length);
        LOG_INFO("Extension: CMD_SET_H265 set cur data: %d\n", dev->xu_h265);
      }
      break;

    default:
      break;
    }
    break;

  default:
    break;
  }
  LOG_DEBUG("Control Request data phase (cs %02x  data %d entity %02x)\n", cs,
            *val, entity_id);
  return 0;
}

static int uvc_events_process_data(struct uvc_device *dev,
                                   struct uvc_request_data *data) {
  struct uvc_streaming_control *target;
  struct uvc_streaming_control *ctrl;
  struct v4l2_format fmt;
  int ret = 0;

  if (dev->usb_state == USB_STATE_INIT) {
    dev->usb_state = USB_STATE_FIRST_CMD;
    struct timespec now_tm = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    dev->first_cmd_pts =
        now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us
  }

  switch (dev->control) {
  case UVC_VS_PROBE_CONTROL:
    LOG_DEBUG("setting probe control, length = %d\n", data->length);
    target = &dev->probe;
    break;

  case UVC_VS_COMMIT_CONTROL:
    LOG_DEBUG("setting commit control, length = %d\n", data->length);
    target = &dev->commit;
    break;

  default:
    LOG_DEBUG("setting process control, length = %d\n", data->length);

    LOG_DEBUG("cs: %u, entity_id: %u\n", dev->cs, dev->entity_id);
    ret = uvc_events_process_control_data(dev, dev->cs, dev->entity_id, data);
    if (ret < 0)
      goto err;

    return 0;
  }

  ctrl = (struct uvc_streaming_control *)&data->data;
  if (dev->control == UVC_VS_PROBE_CONTROL) {
#if 1
    LOG_DEBUG("host probe want ++++vs config:\n");
    LOG_DEBUG("bmHint: %u\n", ctrl->bmHint);
    LOG_DEBUG("bFormatIndex: %u\n", ctrl->bFormatIndex);
    LOG_DEBUG("bFrameIndex: %u\n", ctrl->bFrameIndex);
    LOG_DEBUG("dwFrameInterval: %u\n", ctrl->dwFrameInterval);
    LOG_DEBUG("wKeyFrameRate: %u\n", ctrl->wKeyFrameRate);
    LOG_DEBUG("wPFrameRate: %u\n", ctrl->wPFrameRate);
    LOG_DEBUG("wCompQuality: %u\n", ctrl->wCompQuality);
    LOG_DEBUG("wCompWindowSize: %u\n", ctrl->wCompWindowSize);
    LOG_DEBUG("wDelay: %u\n", ctrl->wDelay);
    LOG_DEBUG("dwMaxVideoFrameSize: %u\n", ctrl->dwMaxVideoFrameSize);
    LOG_DEBUG("dwMaxPayloadTransferSize: %u\n", ctrl->dwMaxPayloadTransferSize);
    LOG_DEBUG("dwClockFrequency: %u\n", ctrl->dwClockFrequency);
    LOG_DEBUG("bmFramingInfo: %u\n", ctrl->bmFramingInfo);
    LOG_DEBUG("bPreferedVersion: %u\n", ctrl->bPreferedVersion);
    LOG_DEBUG("bMinVersion: %u\n", ctrl->bMinVersion);
    LOG_DEBUG("bMaxVersion: %u\n", ctrl->bMaxVersion);
#endif
  }
  uvc_fill_streaming_control(dev, target, ctrl->bFormatIndex, ctrl->bFrameIndex,
                             ctrl->dwFrameInterval);
  dev->fps = 10000000 / target->dwFrameInterval;
  // uvc_control_config(dev);

  if (dev->control == UVC_VS_COMMIT_CONTROL) {
    if (uvc_video_get_uvc_process(dev->video_id))
      return 0;

    uvc_control_config(dev);

    /*
     * Try to set the default format at the V4L2 video capture
     * device as requested by the user.
     */
    CLEAR(fmt);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.field = V4L2_FIELD_ANY;
    fmt.fmt.pix.width = dev->width;
    fmt.fmt.pix.height = dev->height;
    fmt.fmt.pix.pixelformat = dev->fcc;

    switch (dev->fcc) {
    case V4L2_PIX_FMT_YUYV:
    case V4L2_PIX_FMT_NV12:
      fmt.fmt.pix.sizeimage = (fmt.fmt.pix.width * fmt.fmt.pix.height * 2);
      break;
    case V4L2_PIX_FMT_MJPEG:
      fmt.fmt.pix.sizeimage = (fmt.fmt.pix.width * fmt.fmt.pix.height);
    break;
    case V4L2_PIX_FMT_H264:
    case V4L2_PIX_FMT_H265:
#ifdef RK_ENABLE_FASTBOOT
      fmt.fmt.pix.sizeimage = (fmt.fmt.pix.width * fmt.fmt.pix.height) / 2;
#else
      fmt.fmt.pix.sizeimage = (fmt.fmt.pix.width * fmt.fmt.pix.height);
#endif
      break;
    }

    uvc_set_user_resolution(fmt.fmt.pix.width, fmt.fmt.pix.height,
                            dev->video_id);
    uvc_set_user_fcc(fmt.fmt.pix.pixelformat, dev->video_id);
    if (uvc_buffer_init(dev))
      goto err;

    /*
     * As per the new commit command received from the UVC host
     * change the current format selection at both UVC and V4L2
     * sides.
     */
    ret = uvc_video_set_format(dev);
    if (ret < 0)
      goto err;

    if (!dev->run_standalone) {
      /* UVC - V4L2 integrated path. */
      ret = v4l2_set_format(dev->vdev, &fmt);
      if (ret < 0)
        goto err;
    }

    if (dev->bulk) {
      ret = uvc_handle_streamon_event(dev);
      if (ret < 0)
        goto err;
    }
  }

  return 0;

err:
  return ret;
}

static void uvc_events_process(struct uvc_device *dev) {
  struct v4l2_event v4l2_event;
  struct uvc_event *uvc_event = (void *)&v4l2_event.u.data;
  struct uvc_request_data resp;
  int ret = 0;

  ret = ioctl(dev->uvc_fd, VIDIOC_DQEVENT, &v4l2_event);
  if (ret < 0) {
    LOG_ERROR("VIDIOC_DQEVENT failed: %s (%d)\n", strerror(errno), errno);
    return;
  }

  memset(&resp, 0, sizeof resp);
  resp.length = -EL2HLT;

  switch (v4l2_event.type) {
  case UVC_EVENT_CONNECT:
    return;

  case UVC_EVENT_DISCONNECT:
    dev->uvc_shutdown_requested = 1;
    LOG_DEBUG("UVC: Possible USB shutdown requested from "
              "Host, seen via UVC_EVENT_DISCONNECT \n");
    return;

  case UVC_EVENT_SETUP:
    LOG_DEBUG("uvc_events_process:UVC_EVENT_SETUP \n");
    if (dev->suspend) {
      dev->suspend = 0;
      set_uvc_control_suspend(0);
    }
    uvc_events_process_setup(dev, &uvc_event->req, &resp);
    break;

  case UVC_EVENT_DATA:
    LOG_DEBUG("uvc_events_process:UVC_EVENT_DATA \n");
    ret = uvc_events_process_data(dev, &uvc_event->data);
    if (ret < 0)
      break;

    return;

  case UVC_EVENT_STREAMON:
    LOG_INFO("uvc_events_process:UVC_EVENT_STREAMON dev->io=%d dev->bulk:%d, "
             "video_id:%d\n",
             dev->io, dev->bulk, dev->video_id);
#ifdef ENABLE_BUFFER_TIME_DEBUG
    struct timeval buffer_time;
    gettimeofday(&buffer_time, NULL);
    LOG_ERROR("UVC STREAMON START TIME:%d.%d (us)", buffer_time.tv_sec,
              buffer_time.tv_usec);
#endif
    if (!dev->bulk)
      uvc_handle_streamon_event(dev);
    dev->drop_count = 0;
    dev->get_buf_count = 0;
    dev->qbuf_count = 0;
    dev->dqbuf_count = 0;
    dev->try_count = 0;
    dev->usb_state = USB_STATE_FIRST_GET_READY;

    struct timespec now_tm = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &now_tm);
    dev->stream_on_pts =
        now_tm.tv_sec * 1000000LL + now_tm.tv_nsec / 1000; // us

#if UVC_DYNAMIC_DEBUG_FPS
    uvc_debug_info.stream_on_pts = dev->stream_on_pts;
    uvc_debug_info.first_frm = true;
    gettimeofday(&uvc_debug_info.enter_time, NULL);
#endif
    return;

  case UVC_EVENT_STREAMOFF:
    LOG_INFO("uvc_events_process:UVC_EVENT_STREAMOFF enter\n");
    uvc_control_streamoff(dev->video_id);
    // uvc_buffer_deinit(dev->video_id);
    // sleep(1);
    /* Stop V4L2 streaming... */
    if (!dev->run_standalone && dev->vdev->is_streaming) {
      /* UVC - V4L2 integrated path. */
      v4l2_stop_capturing(dev->vdev);
      v4l2_uninit_device(dev->vdev);
      v4l2_reqbufs(dev->vdev, 0);
      dev->vdev->is_streaming = 0;
    }

    set_uvc_control_stop(dev->video_id);
    /* ... and now UVC streaming.. */
    if (dev->is_streaming) {
      uvc_video_stream(dev, 0);
      uvc_uninit_device(dev);
      uvc_video_reqbufs(dev, 0);
      dev->is_streaming = 0;
      dev->first_buffer_queued = 0;
    }
    dev->usb_state = USB_STATE_INIT;
    LOG_INFO("UVC_EVENT_STREAMOFF exit\n");
    return;
  case UVC_EVENT_RESUME:
    LOG_INFO("UVC_EVENT_RESUME:\n");
    if (dev->suspend) {
      dev->suspend = 0;
      set_uvc_control_suspend(0);
    }
    return;
  case UVC_EVENT_SUSPEND:
    LOG_INFO("UVC_EVENT_SUSPEND\n");
    set_uvc_control_suspend(1);
    dev->suspend = 1;
    return;
  }

  ret = ioctl(dev->uvc_fd, UVCIOC_SEND_RESPONSE, &resp);
  if (ret < 0) {
    LOG_ERROR("UVCIOC_S_EVENT failed: %s (%d)\n", strerror(errno), errno);
    return;
  }
}

static void uvc_events_init(struct uvc_device *dev) {
  struct v4l2_event_subscription sub;
  unsigned int payload_size;

  switch (dev->fcc) {
  case V4L2_PIX_FMT_YUYV:
  case V4L2_PIX_FMT_NV12:
    payload_size = dev->width * dev->height * 2;
    break;
  case V4L2_PIX_FMT_MJPEG:
  case V4L2_PIX_FMT_H264:
  case V4L2_PIX_FMT_H265:
    payload_size = dev->imgsize;
    break;
  default:
    return;
  }

  uvc_fill_streaming_control(dev, &dev->probe, 1, 1, 0);
  uvc_fill_streaming_control(dev, &dev->commit, 1, 1, 0);

  if (dev->bulk) {
    /* FIXME Crude hack, must be negotiated with the driver. */
    dev->probe.dwMaxPayloadTransferSize = dev->commit.dwMaxPayloadTransferSize =
        payload_size;
  }

  memset(&sub, 0, sizeof sub);
  sub.type = UVC_EVENT_SETUP;
  ioctl(dev->uvc_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
  sub.type = UVC_EVENT_DATA;
  ioctl(dev->uvc_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
  sub.type = UVC_EVENT_STREAMON;
  ioctl(dev->uvc_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
  sub.type = UVC_EVENT_STREAMOFF;
  ioctl(dev->uvc_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
  sub.type = UVC_EVENT_RESUME;
  ioctl(dev->uvc_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
  sub.type = UVC_EVENT_SUSPEND;
  ioctl(dev->uvc_fd, VIDIOC_SUBSCRIBE_EVENT, &sub);
}

/* ---------------------------------------------------------------------------
 * main
 */

static void image_load(struct uvc_device *dev, const char *img) {
  int fd = -1;

  if (img == NULL)
    return;

  fd = open(img, O_RDONLY);
  if (fd == -1) {
    LOG_ERROR("Unable to open MJPEG image '%s'\n", img);
    return;
  }

  dev->imgsize = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  dev->imgdata = malloc(dev->imgsize);
  if (dev->imgdata == NULL) {
    LOG_ERROR("Unable to allocate memory for MJPEG image\n");
    dev->imgsize = 0;
    return;
  }

  read(fd, dev->imgdata, dev->imgsize);
  close(fd);
}

extern int app_quit;
int uvc_gadget_main(struct uvc_function_config *fc) {
  struct uvc_device *udev = NULL;
  struct v4l2_device *vdev;
  struct timeval tv;
  struct v4l2_format fmt;
  char uvc_devname[32] = {0};
  char *v4l2_devname = "/dev/video0";
  char *mjpeg_image = NULL;
  fd_set fdsv, fdsu;
  int ret, nfds;
  int bulk_mode = 0;
  int dummy_data_gen_mode = 1;
  /* Frame format/resolution related params. */
  int default_format = 1;
  int default_resolution = 1;
  /* USB speed related params */
  int mult = 0;
  int burst = 0;
  enum usb_device_speed speed = USB_SPEED_HIGH; /* High-Speed  1109 is usb 2.0*/
#if (UVC_IO_METHOD == UVC_IO_METHOD_MMAP)
  enum io_method uvc_io_method = IO_METHOD_MMAP;
#elif (UVC_IO_METHOD == UVC_IO_METHOD_USERPTR)
  enum io_method uvc_io_method = IO_METHOD_USERPTR;
#else
  enum io_method uvc_io_method = IO_METHOD_DMA_BUFF;
#endif
  int id = fc->video;
  snprintf(uvc_devname, sizeof(uvc_devname), "/dev/video%d", id);

  /* Open the UVC device. */
  ret = uvc_open(&udev, uvc_devname);
  if (udev == NULL || ret < 0)
    return 1;

  udev->uvc_devname = uvc_devname;
  udev->video_id = id;
  udev->fc = fc;

  /* Set parameters as passed by user. */
  udev->width = (default_resolution == 0) ? 640 : 1280;
  udev->height = (default_resolution == 0) ? 480 : 720;
  udev->imgsize = (default_format == 0)
                      ? (udev->width * udev->height * 2)
                      : (udev->width * udev->height);
  switch (default_format) {
  case 1:
    udev->fcc = V4L2_PIX_FMT_MJPEG;
    break;

  case 2:
    udev->fcc = V4L2_PIX_FMT_H264;
    break;

  case 3:
    udev->fcc = V4L2_PIX_FMT_H265;
    break;

  case 0:
  default:
    udev->fcc = V4L2_PIX_FMT_YUYV;
    break;
  }
  uvc_set_user_fcc(udev->fcc, udev->video_id);
  udev->io = uvc_io_method;
  udev->bulk = bulk_mode;
  udev->nbufs = 0;
  udev->mult = mult;
  udev->burst = burst;
  udev->speed = speed;
  udev->control = 0;

  if (dummy_data_gen_mode || mjpeg_image)
    /* UVC standalone setup. */
    udev->run_standalone = 1;
  switch (speed) {
  case USB_SPEED_FULL:
    /* Full Speed. */
    if (bulk_mode)
      udev->maxpkt = 64;
    else
      udev->maxpkt = 1023;
    break;

  case USB_SPEED_HIGH:
    /* High Speed. */
    if (bulk_mode)
      udev->maxpkt = 512;
    else
      udev->maxpkt = 1024;
    break;

  case USB_SPEED_SUPER:
  default:
    /* Super Speed. */
    if (bulk_mode)
      udev->maxpkt = 1024;
    else
      udev->maxpkt = 1024;
    break;
  }

  if (mjpeg_image)
    image_load(udev, mjpeg_image);

  /* Init UVC events. */
  uvc_events_init(udev);
  uvc_set_user_run_state(true, udev->video_id);

  while (uvc_get_user_run_state(udev->video_id)) {
    FD_ZERO(&fdsu);
    /* We want both setup and data events on UVC interface.. */
    FD_SET(udev->uvc_fd, &fdsu);
    fd_set efds = fdsu;
    fd_set dfds = fdsu;
    /* Timeout. */
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    ret = select(udev->uvc_fd + 1, NULL, &dfds, &efds, &tv);
    if (-1 == ret) {
      LOG_ERROR("select error %d, %s\n", errno, strerror(errno));
      if (EINTR == errno)
        continue;

      break;
    }

    if (0 == ret) {
      if (udev->bulk)
        continue;
      LOG_ERROR("select timeout\n");
      if (!access("/tmp/uvc_no_timeout", 0))
        continue;
      break;
    }

    if (app_quit) {
      LOG_ERROR("app quit=%d...\n", app_quit);
      if (3 == app_quit) {
        uvc_control_inbuf_deinit();
        break;
        app_quit = 0;
      } else
        break;
    }
    if (FD_ISSET(udev->uvc_fd, &efds))
      uvc_events_process(udev);
    if (FD_ISSET(udev->uvc_fd, &dfds))
      uvc_video_process(udev);
  }

  set_uvc_control_stop(id);
  if (udev->is_streaming) {
    /* ... and now UVC streaming.. */
    uvc_video_stream(udev, 0);
    uvc_uninit_device(udev);
    uvc_video_reqbufs(udev, 0);
    udev->is_streaming = 0;
  }
  uvc_control_streamoff(id);
  uvc_control_delete(id);
  uvc_close(udev);
  set_uvc_control_restart();
  LOG_INFO("uvc_gadget_main exit");

  return 0;
}

/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * ConfigFS Gadget device handling
 *
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 *
 */

/* To provide basename and asprintf from the GNU library. */
#define _GNU_SOURCE

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <linux/videodev2.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "uvc_configfs.h"

#define V4L2_PIX_FMT_H265                                                      \
  v4l2_fourcc('H', '2', '6', '5') /* H265 with start codes */

#define UVC_FUNCTION_NAME "uvc*"

#define UVC_GUID_FORMAT_MJPEG                                                  \
  {                                                                            \
    'M', 'J', 'P', 'G', 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa, 0x00,  \
        0x38, 0x9b, 0x71                                                       \
  }
#define UVC_GUID_FORMAT_YUY2                                                   \
  {                                                                            \
    'Y', 'U', 'Y', '2', 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa, 0x00,  \
        0x38, 0x9b, 0x71                                                       \
  }
#define UVC_GUID_FORMAT_NV12                                                   \
  {                                                                            \
    'N', 'V', '1', '2', 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa, 0x00,  \
        0x38, 0x9b, 0x71                                                       \
  }
#define UVC_GUID_FORMAT_H264                                                   \
  {                                                                            \
    'H', '2', '6', '4', 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa, 0x00,  \
        0x38, 0x9b, 0x71                                                       \
  }
#define UVC_GUID_FORMAT_H265                                                   \
  {                                                                            \
    'H', '2', '6', '5', 0x00, 0x00, 0x10, 0x00, 0x80, 0x00, 0x00, 0xaa, 0x00,  \
        0x38, 0x9b, 0x71                                                       \
  }

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

struct uvc_function_format_info {
  uint8_t guid[16];
  uint32_t fcc;
};

static struct uvc_function_format_info uvc_formats[] = {
    {
        .guid = UVC_GUID_FORMAT_YUY2, .fcc = V4L2_PIX_FMT_YUYV,
    },
    {
        .guid = UVC_GUID_FORMAT_MJPEG, .fcc = V4L2_PIX_FMT_MJPEG,
    },
    {
        .guid = UVC_GUID_FORMAT_H264, .fcc = V4L2_PIX_FMT_H264,
    },
    {
        .guid = UVC_GUID_FORMAT_H265, .fcc = V4L2_PIX_FMT_H265,
    },
    {
        .guid = UVC_GUID_FORMAT_NV12, .fcc = V4L2_PIX_FMT_NV12,
    }};

/* -----------------------------------------------------------------------------
 * Path handling and support
 */
static char *path_join(const char *dirname, const char *name) {
  char *path;
  int ret;

  ret = asprintf(&path, "%s/%s", dirname, name);

  /*
   * asprintf returns -1 on allocation or other errors, leaving 'path'
   * undefined. We shouldn't even call free(path) here. We want to return
   * NULL on error, so we must manually set it.
   */
  if (ret < 0)
    path = NULL;

  return path;
}

static char *path_glob_index_match(const char *g, unsigned int index) {
  glob_t globbuf;
  char *match = NULL;

  glob(g, 0, NULL, &globbuf);

  if (globbuf.gl_pathc > index)
    match = strdup(globbuf.gl_pathv[index]);

  globfree(&globbuf);

  return match;
}

/*
 * Find and return the full path of the first directory entry that satisfies
 * the match function.
 */
static char *dir_first_match(const char *dir,
                             int (*match)(const struct dirent *)) {
  struct dirent **entries;
  unsigned int i;
  int n_entries;
  char *path;

  n_entries = scandir(dir, &entries, match, alphasort);
  if (n_entries < 0)
    return NULL;

  if (n_entries == 0) {
    free(entries);
    return NULL;
  }

  path = path_join(dir, entries[0]->d_name);

  for (i = 0; i < (unsigned int)n_entries; ++i)
    free(entries[i]);

  free(entries);

  return path;
}

/* -----------------------------------------------------------------------------
 * Attribute handling
 */
static int attribute_read(const char *path, const char *file, void *buf,
                          unsigned int len) {
  char *f;
  int ret;
  int fd;

  f = path_join(path, file);
  if (!f)
    return -ENOMEM;

  fd = open(f, O_RDONLY);
  free(f);
  if (fd == -1) {
    printf("Failed to open attribute %s: %s\n", file, strerror(errno));
    return -ENOENT;
  }

  ret = read(fd, buf, len);
  close(fd);

  if (ret < 0) {
    printf("Failed to read attribute %s: %s\n", file, strerror(errno));
    return -ENODATA;
  }

  return len;
}

static int attribute_read_uint(const char *path, const char *file,
                               unsigned int *val) {
  char buf[11] = {0};
  char *endptr;
  int ret;

  ret = attribute_read(path, file, buf, sizeof(buf) - 1);
  if (ret < 0)
    return ret;

  buf[ret] = '\0';
  errno = 0;

  /* base 0: Autodetect hex, octal, decimal. */
  *val = strtoul(buf, &endptr, 0);
  if (errno)
    return -errno;

  if (endptr == buf)
    return -ENODATA;

  return 0;
}

static char *attribute_read_str(const char *path, const char *file) {
  char buf[1024] = {0};
  char *p;
  int ret;

  ret = attribute_read(path, file, buf, sizeof(buf) - 1);
  if (ret < 0)
    return NULL;

  buf[ret] = '\0';

  p = strrchr(buf, '\n');
  if (p == NULL)
    return NULL;
  if (p != buf)
    *p = '\0';

  return strdup(buf);
}

static char *configfs_find_uvc_function(unsigned int index) {
  const char *target = UVC_FUNCTION_NAME;
  const char *format;
  char *func_path;
  char *path;
  int ret;

  format = "%s/usb_gadget/*/functions/%s";
  ret = asprintf(&path, format, "/sys/kernel/config", target);
  if (!ret)
    return NULL;

  func_path = path_glob_index_match(path, index);

  free(path);

  return func_path;
}

static int udc_find_video_device(const char *udc, const char *function) {
  char *vpath;
  char *video = NULL;
  glob_t globbuf;
  int video_id = -1;
  unsigned int i;
  int ret;

  ret = asprintf(&vpath, "/sys/class/udc/%s/device/gadget/video4linux/video*",
                 udc ? udc : "*");
  if (!ret)
    return -1;

  glob(vpath, 0, NULL, &globbuf);
  free(vpath);

  for (i = 0; i < globbuf.gl_pathc; ++i) {
    char *config;
    bool match;

    /* Match on the first if no search string. */
    if (!function)
      break;

    config = attribute_read_str(globbuf.gl_pathv[i], "function_name");
    match = strcmp(function, config) == 0;
    free(config);
    if (match)
      break;
  }

  if (i < globbuf.gl_pathc) {
    const char *v = basename(globbuf.gl_pathv[i]);
    video = path_join("/dev", v);
    if (sscanf(video, "/dev/video%d", &video_id) != 1)
      video_id = -1;
    free(video);
  }

  globfree(&globbuf);

  return video_id;
}

/*
 * configfs_free_uvc_function - Free a uvc_function_config object
 * @fc: The uvc_function_config to be freed
 *
 * Free the given @fc function previously allocated by a call to
 * configfs_parse_uvc_function().
 */
void configfs_free_uvc_function(struct uvc_function_config *fc) {
  unsigned int i, j;

  free(fc->udc);
  free(fc->dev_name);

  for (i = 0; i < fc->streaming.num_formats; ++i) {
    struct uvc_function_config_format *format = &fc->streaming.formats[i];

    for (j = 0; j < format->num_frames; ++j) {
      struct uvc_function_config_frame *frame = &format->frames[j];

      free(frame->intervals);
    }
    free(format->frames);
  }
  free(fc->streaming.formats);
  free(fc);
}

#define configfs_parse_child(parent, child, cfg, parse)                        \
  ({                                                                           \
    char *__path;                                                              \
    int __ret;                                                                 \
                                                                               \
    __path = path_join((parent), (child));                                     \
    if (__path) {                                                              \
      __ret = parse(__path, (cfg));                                            \
      free(__path);                                                            \
    } else {                                                                   \
      __ret = -ENOMEM;                                                         \
    }                                                                          \
                                                                               \
    __ret;                                                                     \
  })

static int configfs_parse_interface(const char *path,
                                    struct uvc_function_config_interface *cfg) {
  int ret;

  ret = attribute_read_uint(path, "bInterfaceNumber", &cfg->bInterfaceNumber);

  return ret;
}

static int configfs_parse_control(const char *path,
                                  struct uvc_function_config_control *cfg) {
  int ret;

  ret = configfs_parse_interface(path, &cfg->intf);

  return ret;
}

static int
configfs_parse_streaming_frame(const char *path,
                               struct uvc_function_config_frame *frame) {
  char *intervals;
  char *p;
  int ret = 0;

  ret = ret ?: attribute_read_uint(path, "bFrameIndex", &frame->index);
  ret = ret ?: attribute_read_uint(path, "wWidth", &frame->width);
  ret = ret ?: attribute_read_uint(path, "wHeight", &frame->height);
  if (ret)
    return ret;

  intervals = attribute_read_str(path, "dwFrameInterval");
  if (!intervals)
    return -EINVAL;

  for (p = intervals; *p;) {
    unsigned int interval;
    unsigned int *mem;
    char *endp;
    size_t size;

    interval = strtoul(p, &endp, 10);
    if (*endp != '\0' && *endp != '\n') {
      ret = -EINVAL;
      break;
    }

    p = *endp ? endp + 1 : endp;

    size = sizeof *frame->intervals * (frame->num_intervals + 1);
    mem = realloc(frame->intervals, size);
    if (!mem) {
      ret = -ENOMEM;
      break;
    }
    frame->intervals = mem;
    frame->intervals[frame->num_intervals++] = interval;
  }
  free(intervals);

  return ret;
}

static int frame_filter(const struct dirent *ent) {
  /* Accept all directories but "." and "..". */
  if (ent->d_type != DT_DIR)
    return 0;
  if (!strcmp(ent->d_name, "."))
    return 0;
  if (!strcmp(ent->d_name, ".."))
    return 0;
  return 1;
}

static int frame_compare(const void *a, const void *b) {
  const struct uvc_function_config_frame *fa = a;
  const struct uvc_function_config_frame *fb = b;

  if (fa->index < fb->index)
    return -1;
  else if (fa->index == fb->index)
    return 0;
  else
    return 1;
}

static int
configfs_parse_streaming_format(const char *path,
                                struct uvc_function_config_format *format) {
  struct dirent **entries;
  char link_target[1024] = {0};
  char *segment;
  unsigned int i;
  int n_entries;
  int ret;

  ret = attribute_read_uint(path, "bFormatIndex", &format->index);
  if (ret < 0)
    return ret;

  ret = readlink(path, link_target, sizeof(link_target) - 1);
  if (ret < 0)
    return ret;

  link_target[ret] = '\0';

  /*
   * Extract the second-to-last path component of the link target,
   * which contains the format descriptor type name as exposed by
   * the UVC function driver.
   */
  segment = strrchr(link_target, '/');
  if (!segment)
    return -EINVAL;
  *segment = '\0';
  segment = strrchr(link_target, '/');
  if (!segment)
    return -EINVAL;
  segment++;
  if (!strcmp(segment, "framebased")) {
    ret =
        attribute_read(path, "guidFormat", format->guid, sizeof(format->guid));
  } else if (!strcmp(segment, "mjpeg")) {
    static const uint8_t guid[16] = UVC_GUID_FORMAT_MJPEG;
    memcpy(format->guid, guid, 16);
  } else if (!strcmp(segment, "uncompressed")) {
    ret =
        attribute_read(path, "guidFormat", format->guid, sizeof(format->guid));
    if (ret < 0)
      return ret;
  } else {
    return -EINVAL;
  }

  for (i = 0; i < ARRAY_SIZE(uvc_formats); ++i) {
    if (!memcmp(uvc_formats[i].guid, format->guid, 16)) {
      format->fcc = uvc_formats[i].fcc;
      break;
    }
  }

  /* Find all entries corresponding to a frame and parse them. */
  n_entries = scandir(path, &entries, frame_filter, alphasort);
  if (n_entries < 0)
    return -errno;

  if (n_entries == 0) {
    free(entries);
    return -EINVAL;
  }

  format->num_frames = n_entries;
  format->frames = calloc(sizeof *format->frames, format->num_frames);
  if (!format->frames)
    return -ENOMEM;

  for (i = 0; i < (unsigned int)n_entries; ++i) {
    char *frame;

    frame = path_join(path, entries[i]->d_name);
    if (!frame) {
      ret = -ENOMEM;
      goto done;
    }

    ret = configfs_parse_streaming_frame(frame, &format->frames[i]);
    free(frame);
    if (ret < 0)
      goto done;
  }

  /* Sort the frames by index. */
  qsort(format->frames, format->num_frames, sizeof *format->frames,
        frame_compare);

done:
  for (i = 0; i < (unsigned int)n_entries; ++i)
    free(entries[i]);
  free(entries);

  return ret;
}

static int format_filter(const struct dirent *ent) {
  char *path;
  bool valid;

  /*
   * Accept all links that point to a directory containing a
   * "bFormatIndex" file.
   */
  if (ent->d_type != DT_LNK)
    return 0;

  path = path_join(ent->d_name, "bFormatIndex");
  if (!path)
    return 0;

  valid = access(path, R_OK);
  free(path);
  return valid;
}

static int format_compare(const void *a, const void *b) {
  const struct uvc_function_config_format *fa = a;
  const struct uvc_function_config_format *fb = b;

  if (fa->index < fb->index)
    return -1;
  else if (fa->index == fb->index)
    return 0;
  else
    return 1;
}

static int
configfs_parse_streaming_header(const char *path,
                                struct uvc_function_config_streaming *cfg) {
  struct dirent **entries;
  unsigned int i;
  int n_entries;
  int ret;

  /* Find all entries corresponding to a format and parse them. */
  n_entries = scandir(path, &entries, format_filter, alphasort);
  if (n_entries < 0)
    return -errno;

  if (n_entries == 0) {
    free(entries);
    return -EINVAL;
  }

  cfg->num_formats = n_entries;
  cfg->formats = calloc(sizeof *cfg->formats, cfg->num_formats);
  if (!cfg->formats)
    return -ENOMEM;

  for (i = 0; i < (unsigned int)n_entries; ++i) {
    char *format;

    format = path_join(path, entries[i]->d_name);
    if (!format) {
      ret = -ENOMEM;
      goto done;
    }

    ret = configfs_parse_streaming_format(format, &cfg->formats[i]);
    free(format);
    if (ret < 0)
      goto done;
  }

  /* Sort the formats by index. */
  qsort(cfg->formats, cfg->num_formats, sizeof *cfg->formats, format_compare);

done:
  for (i = 0; i < (unsigned int)n_entries; ++i)
    free(entries[i]);
  free(entries);

  return ret;
}

static int link_filter(const struct dirent *ent) {
  /* Accept all links. */
  return ent->d_type == DT_LNK;
}

static int configfs_parse_streaming(const char *path,
                                    struct uvc_function_config_streaming *cfg) {
  char *header;
  char *class;
  int ret;

  ret = configfs_parse_interface(path, &cfg->intf);
  if (ret < 0)
    return ret;
  /*
   * Handle the high-speed class descriptors only for now. Find the first
   * link to the class descriptors.
   */
  class = path_join(path, "class/hs");
  if (!class)
    return -ENOMEM;

  header = dir_first_match(class, link_filter);
  free(class);
  if (!header)
    return -EINVAL;

  ret = configfs_parse_streaming_header(header, cfg);
  free(header);
  return ret;
}

static int configfs_parse_uvc(const char *fpath,
                              struct uvc_function_config *fc) {
  int ret = 0;

  ret = ret ?: configfs_parse_child(fpath, "control", &fc->control,
                                    configfs_parse_control);
  ret = ret ?: configfs_parse_child(fpath, "streaming", &fc->streaming,
                                    configfs_parse_streaming);

  /*
   * These parameters should be part of the streaming interface in
   * ConfigFS, but for legacy reasons they are located directly in the
   * function directory.
   */
  ret = ret ?: attribute_read_uint(fpath, "streaming_interval",
                                   &fc->streaming.ep.bInterval);
  ret = ret ?: attribute_read_uint(fpath, "streaming_maxburst",
                                   &fc->streaming.ep.bMaxBurst);
  ret = ret ?: attribute_read_uint(fpath, "streaming_maxpacket",
                                   &fc->streaming.ep.wMaxPacketSize);

  return ret;
}

struct uvc_function_config *configfs_parse_uvc_function(unsigned int index) {
  struct uvc_function_config *fc;
  char *fpath;
  char *function;
  int ret = 0;

  fc = malloc(sizeof *fc);
  if (fc == NULL)
    return NULL;

  memset(fc, 0, sizeof *fc);

  /* Find the function in ConfigFS. */
  fpath = configfs_find_uvc_function(index);
  if (!fpath) {
    free(fc);
    printf("[configfs_parse_uvc_function] configfs_find_uvc_function error!\n");
    return NULL;
  }

  function = basename(fpath);

  fc->dev_name = attribute_read_str(fpath, "device_name");
  if (fc->dev_name == NULL)
    fc->dev_name = "UVC CAMERA";
  fc->udc = attribute_read_str(fpath, "../../UDC");
  fc->video = udc_find_video_device(fc->udc, function);
  printf("fc->dev_name:%s, fc->udc:%s, fc->video:%d \n", fc->dev_name, fc->udc,
         fc->video);
  if (fc->video < 0) {
    ret = -ENODEV;
    goto done;
  }
  fc->index = index;

  ret = configfs_parse_uvc(fpath, fc);

done:
  if (ret) {
    configfs_free_uvc_function(fc);
    printf("[configfs_parse_uvc_function] configfs_free_uvc_function\n");
    fc = NULL;
  }

  free(fpath);

  return fc;
}

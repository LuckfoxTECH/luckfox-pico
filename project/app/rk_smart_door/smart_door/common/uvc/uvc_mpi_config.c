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
#include "uvc_mpi_config.h"
#include "cJSON.h"
#include "rk_mpi_sys.h"
#include "uvc_log.h"
#include "uvc_mpi_vi.h"
#include "uvc_video.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <sys/stat.h>

#define UVC_MPI_CFG_DEBUG_FILE_NAME "/data/uvc.bin"
#define UVC_MPI_CFG_NN_DEBUG_FILE_NAME "/data/nn.bin"
#define UVC_MPI_CFG_ORIGINAL_PATH "/oem/usr/share/uvc_mpi_cfg.conf"
#define UVC_MPI_CFG_MODIFY_PATH "/usr/share/uvc_mpi_cfg.conf"
#define UVC_MPI_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))

#define UVC_FIND_ENTRY_VALUE_BY_STRCMP(INPUT, MAP, KEY, VALUE)                 \
  if (INPUT) {                                                                 \
    for (size_t i = 0; i < UVC_MPI_ARRAY_ELEMS(MAP); i++) {                    \
      if (!strcmp(INPUT, MAP[i].KEY))                                          \
        return MAP[i].VALUE;                                                   \
    }                                                                          \
  }

static void copy_str_to_array(char *dst, char *src, int dst_len) {
  memset(dst, 0, dst_len);
  memcpy(dst, src, strlen(src));
}

static unsigned long get_file_size(const char *filename) {
  struct stat buf;
  if (stat(filename, &buf) < 0)
    return 0;

  return (unsigned long)buf.st_size;
}

typedef struct _RkFormatNameMap {
  PIXEL_FORMAT_E format;
  const char *name;
} RkFormatNameMap;

const static RkFormatNameMap sRkFormatNames[] = {
    {RK_FMT_YUV420SP, "nv12"},          {RK_FMT_YUV420SP_VU, "nv21"},
    {RK_FMT_YUV422_YUYV, "yuv422yuyv"}, {RK_FMT_YUV422_UYVY, "yuv422uyvy"},
    {RK_FMT_RGB888, "rgb888"},          {RK_FMT_BGR888, "bgr888"},
    {RK_FMT_ARGB8888, "argb8888"},      {RK_FMT_BGRA8888, "bgra8888"},
};

int pix_format_rt_string_to_gl(const char *stFormat) {
  UVC_FIND_ENTRY_VALUE_BY_STRCMP(stFormat, sRkFormatNames, name, format);
  return RK_FMT_YUV420SP;
}

static cJSON *parse_find_uvc_index_cfg(cJSON *root, int index, bool init) {
  int ret = 0;
  char index_node[10] = {0};
  sprintf(index_node, "index_%d", index);

  cJSON *child = cJSON_GetObjectItem(root, index_node);
  if (!child) {
    LOG_ERROR("find %s fail\n", index_node);
    return NULL;
  }

  return child;
}

static int parse_check_uvc_common_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                      bool init) {
  int ret = 0;

  MpiUvcCommonCfg *commonCfg = &mpiCfg->common_cfg;
  if (NULL == commonCfg)
    return -1;

  commonCfg->change = 0;
  cJSON *child = cJSON_GetObjectItem(root, "common_cfg");
  if (!child) {
    LOG_ERROR("parse common_cfg err\n");
    return -1;
  }
  cJSON *child_version = cJSON_GetObjectItem(child, "version");
  if (child_version) {
    LOG_DEBUG("parse common version:%d\n", child_version->valueint);
    commonCfg->version = child_version->valueint;
  }

  cJSON *child_property = cJSON_GetObjectItem(child, "property");
  if (!child_property) {
    LOG_ERROR("parse common property err\n");
    return -1;
  } else {
    cJSON *child_property_param = NULL;
    if (init)
      child_property_param = cJSON_GetObjectItem(child_property, "param_init");
    else
      child_property_param =
          cJSON_GetObjectItem(child_property, "param_change");
    if (child_property_param) {
      cJSON *child_item = cJSON_GetObjectItem(child_property_param,
                                              "check_param_change_thread");
      if (child_item) {
        commonCfg->need_check = strstr(child_item->valuestring, "on") ? 1 : 0;
        commonCfg->change |= MPI_UVC_COMMON_CFG_NEED_CHECK;
      }
      child_item = cJSON_GetObjectItem(child_property_param, "uvc_debug");
      if (child_item) {
        commonCfg->uvc_debug = strstr(child_item->valuestring, "on") ? 1 : 0;
        commonCfg->change |= MPI_UVC_COMMON_CFG_UVC_DEBUG;
      }
      child_item = cJSON_GetObjectItem(child_property_param, "yuyv_debug");
      if (child_item) {
        commonCfg->yuyv_debug = strstr(child_item->valuestring, "on") ? 1 : 0;
        commonCfg->change |= MPI_UVC_COMMON_CFG_UVC_YUYV_DEBUG;
      }
      child_item = cJSON_GetObjectItem(child_property_param, "nn_enable");
      if (child_item) {
        commonCfg->nn_enable = strstr(child_item->valuestring, "on") ? 1 : 0;
        commonCfg->change |= MPI_UVC_COMMON_CFG_UVC_NN_ENABLE;
      }
      child_item =
          cJSON_GetObjectItem(child_property_param, "uvc_debug_file_name");
      if (child_item) {
        copy_str_to_array(commonCfg->uvc_debug_file, child_item->valuestring,
                          UVC_MPI_STREAM_SAVE_FILE_LEN);
        commonCfg->change |= MPI_UVC_COMMON_CFG_UVC_DEBUG_FILE_NAME;
      }
      child_item =
          cJSON_GetObjectItem(child_property_param, "uvc_debug_file_cnt");
      if (child_item) {
        commonCfg->uvc_debug_cnt = child_item->valueint;
        commonCfg->change |= MPI_UVC_COMMON_CFG_UVC_DEBUG_FILE_CNT;
      }
      child_item =
          cJSON_GetObjectItem(child_property_param, "nn_debug_file_name");
      if (child_item) {
        copy_str_to_array(commonCfg->nn_debug_file, child_item->valuestring,
                          UVC_MPI_STREAM_SAVE_FILE_LEN);
        commonCfg->change |= MPI_UVC_COMMON_CFG_NN_DEBUG_FILE_NAME;
      }
      child_item =
          cJSON_GetObjectItem(child_property_param, "nn_debug_file_cnt");
      if (child_item) {
        commonCfg->nn_debug_cnt = child_item->valueint;
        commonCfg->change |= MPI_UVC_COMMON_CFG_NN_DEBUG_FILE_CNT;
      }
      child_item =
          cJSON_GetObjectItem(child_property_param, "geometric_output");
      if (child_item) {
        RK_S32 cnt = 0;
        cnt = sscanf(child_item->valuestring, "%d:%d",
                     &commonCfg->geometric_output_num,
                     &commonCfg->geometric_output_den);
        if (!cnt) {
          LOG_ERROR("invalid set geometric_output num:den,such as 16:9\n");
        } else {
          commonCfg->change |= MPI_UVC_COMMON_CFG_GEOMETRIC_OUT;
        }
      }
      child_item = cJSON_GetObjectItem(child_property_param, "uvc_enable_vpss");
      if (child_item) {
        commonCfg->uvc_enable_vpss =
            strstr(child_item->valuestring, "on") ? 1 : 0;
        commonCfg->change |= MPI_UVC_COMMON_CFG_UVC_ENABLE_VPSS;
      }
      LOG_INFO("uvc common change:0x%x\n", commonCfg->change);
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_vi_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                      bool init) {
  int ret = 0;
  int version;
  MpiViCfg *mpiViCfg = NULL;
  cJSON *child_property = NULL;

  cJSON *child = cJSON_GetObjectItem(root, "vi_cfg");
  if (!child) {
    LOG_ERROR("parse vi_cfg err\n");
    return -1;
  }
  cJSON *child_version = cJSON_GetObjectItem(child, "version");
  if (child_version) {
    LOG_DEBUG("parse vi version:%d\n", child_version->valueint);
    version = child_version->valueint;
  }

  for (int chnType = MPI_VI_CHANNEL_TYPE_UVC; chnType < MPI_VI_CHANNEL_TYPE_MAX;
       chnType++) {
    mpiViCfg = &mpiCfg->vi_cfg[chnType];
    if (NULL == mpiViCfg)
      return -1;
    mpiViCfg->version = version;
    mpiViCfg->change = 0;
    switch (chnType) {
    case MPI_VI_CHANNEL_TYPE_UVC:
      child_property = cJSON_GetObjectItem(child, "uvc");
      break;
    case MPI_VI_CHANNEL_TYPE_NN:
      child_property = cJSON_GetObjectItem(child, "nn");
      break;
    default:
      LOG_ERROR("no support this chnType:%d\n", chnType);
      child_property = NULL;
      break;
    }

    if (!child_property) {
      LOG_ERROR("parse vi property err\n");
      return -1;
    } else {
      cJSON *child_property_param = NULL;
      if (init)
        child_property_param =
            cJSON_GetObjectItem(child_property, "param_init");
      else
        child_property_param =
            cJSON_GetObjectItem(child_property, "param_change");
      if (child_property_param) {
        cJSON *child_item = cJSON_GetObjectItem(child_property_param, "dev_id");
        if (child_item) {
          mpiViCfg->dev_id = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_DEV_ID;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "channel_id");
        if (child_item) {
          mpiViCfg->channel_id = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_CHANNEL_ID;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "buf_cnt");
        if (child_item) {
          mpiViCfg->buf_cnt = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_BUF_CNT;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "memory_type");
        if (child_item) {
          switch (child_item->valueint) {
          case 1:
            mpiViCfg->memory_type = VI_V4L2_MEMORY_TYPE_MMAP;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MEMORY_TYPE;
            break;
          case 2:
            mpiViCfg->memory_type = VI_V4L2_MEMORY_TYPE_USERPTR;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MEMORY_TYPE;
            break;
          case 3:
            mpiViCfg->memory_type = VI_V4L2_MEMORY_TYPE_OVERLAY;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MEMORY_TYPE;
            break;
          case 4:
            mpiViCfg->memory_type = VI_V4L2_MEMORY_TYPE_DMABUF;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MEMORY_TYPE;
            break;
          default:
            LOG_WARN("memory_type:%d not support\n", child_item->valueint);
            mpiViCfg->memory_type = VI_V4L2_MEMORY_TYPE_DMABUF;
            break;
          }
        }
        child_item = cJSON_GetObjectItem(child_property_param, "fbc");
        if (child_item) {
          mpiViCfg->fbc = strstr(child_item->valuestring, "on")
                              ? COMPRESS_AFBC_16x16
                              : COMPRESS_MODE_NONE;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_FBC;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "fps");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(child_item->valuestring, "%d:%d", &mpiViCfg->fps_in,
                       &mpiViCfg->fps_out);
          if (!cnt) {
            LOG_ERROR("invalid set vi fps in:out,such as 30:25\n");
          } else {
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_FPS;
          }
        }
        child_item = cJSON_GetObjectItem(child_property_param, "rotation");
        if (child_item) {
          switch (child_item->valueint) {
          case 0:
            mpiViCfg->rotation = ROTATION_0;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_ROTATION;
            break;
          case 90:
            mpiViCfg->rotation = ROTATION_90;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_ROTATION;
            break;
          case 180:
            mpiViCfg->rotation = ROTATION_180;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_ROTATION;
            break;
          case 270:
            mpiViCfg->rotation = ROTATION_270;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_ROTATION;
            break;
          default:
            LOG_WARN("rotation:%d not support\n", child_item->valueint);
            mpiViCfg->rotation = ROTATION_0;
            break;
          }
        }
        child_item = cJSON_GetObjectItem(child_property_param, "mirror");
        if (child_item) {
          switch (child_item->valueint) {
          case 0:
            mpiViCfg->mirror = MIRROR_NONE;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MIRROR;
            break;
          case 1:
            mpiViCfg->mirror = MIRROR_HORIZONTAL;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MIRROR;
            break;
          case 2:
            mpiViCfg->mirror = MIRROR_VERTICAL;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MIRROR;
            break;
          case 3:
            mpiViCfg->mirror = MIRROR_BOTH;
            mpiViCfg->change |= MPI_VI_CFG_CHANGE_MIRROR;
            break;
          default:
            LOG_WARN("mirror:%d not support\n", child_item->valueint);
            mpiViCfg->mirror = MIRROR_NONE;
            break;
          }
        }
        child_item = cJSON_GetObjectItem(child_property_param, "dev_name");
        if (child_item && !strstr(child_item->valuestring, "null")) {
          copy_str_to_array(mpiViCfg->dev_name, child_item->valuestring,
                            MAX_VI_ENTITY_NAME_LEN);
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_DEV_NAME;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "assign_width");
        if (child_item) {
          mpiViCfg->assign_width = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_ASSIGN_WIDTH;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "assign_height");
        if (child_item) {
          mpiViCfg->assign_height = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_ASSIGN_HEIGHT;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "min_width");
        if (child_item) {
          mpiViCfg->min_width = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_MIN_WIDTH;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "min_height");
        if (child_item) {
          mpiViCfg->min_height = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_MIN_HEIGHT;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "max_width");
        if (child_item) {
          mpiViCfg->max_width = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_MAX_WIDTH;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "max_height");
        if (child_item) {
          mpiViCfg->max_height = child_item->valueint;
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_MAX_HEIGHT;
        }
        child_item = cJSON_GetObjectItem(child_property_param, "format");
        if (child_item) {
          mpiViCfg->format =
              pix_format_rt_string_to_gl(child_item->valuestring);
          mpiViCfg->change |= MPI_VI_CFG_CHANGE_FORMAT;
          LOG_INFO("mpiViCfg[%d]->format:0x%x\n", chnType, mpiViCfg->format);
        }
        LOG_INFO("vi_cfg[%d] change:0x%x\n", chnType, mpiViCfg->change);
      } else {
        LOG_WARN("no find vi_cfg[%d] param_init or param_change\n", chnType);
      }
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_vpss_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                        bool init) {
  int ret = 0;
  int version;
  MpiVpssCfg *mpiVpssCfg = NULL;
  cJSON *child_property = NULL;

  cJSON *child = cJSON_GetObjectItem(root, "vpss_cfg");
  if (!child) {
    LOG_ERROR("parse vpss_cfg err\n");
    return -1;
  }
  cJSON *child_version = cJSON_GetObjectItem(child, "version");
  if (child_version) {
    LOG_DEBUG("parse vi version:%d\n", child_version->valueint);
    version = child_version->valueint;
  }

  for (int chnType = MPI_VPSS_CHANNEL_TYPE_UVC;
       chnType < MPI_VPSS_CHANNEL_TYPE_MAX; chnType++) {
    mpiVpssCfg = &mpiCfg->vpss_cfg[chnType];
    if (NULL == mpiVpssCfg)
      return -1;
    mpiVpssCfg->version = version;
    mpiVpssCfg->change = 0;
    switch (chnType) {
    case MPI_VPSS_CHANNEL_TYPE_UVC:
      child_property = cJSON_GetObjectItem(child, "uvc");
      break;
    case MPI_VPSS_CHANNEL_TYPE_NN:
      child_property = cJSON_GetObjectItem(child, "nn");
      break;
    default:
      LOG_ERROR("no support this chnType:%d\n", chnType);
      child_property = NULL;
      break;
    }

    for (int chn = 0; chn < 1; chn++) {
      if (!child_property) {
        LOG_ERROR("parse vpss property err\n");
        return -1;
      } else {
        cJSON *child_property_param = NULL;
        if (init)
          child_property_param =
              cJSON_GetObjectItem(child_property, "param_init");
        else
          child_property_param =
              cJSON_GetObjectItem(child_property, "param_change");
        if (child_property_param) {
          cJSON *child_item =
              cJSON_GetObjectItem(child_property_param, "group_id");
          if (child_item) {
            mpiVpssCfg->group_id = child_item->valueint;
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_GROUP_ID;
          }
          child_item = cJSON_GetObjectItem(child_property_param, "channel_id");
          if (child_item) {
            mpiVpssCfg->channel_id = child_item->valueint;
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_CHANNEL_ID;
          }
          child_item = cJSON_GetObjectItem(child_property_param, "buf_cnt");
          if (child_item) {
            mpiVpssCfg->buf_cnt = child_item->valueint;
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_BUF_CNT;
          }
          child_item = cJSON_GetObjectItem(child_property_param, "fbc");
          if (child_item) {
            mpiVpssCfg->fbc = strstr(child_item->valuestring, "on")
                                  ? COMPRESS_AFBC_16x16
                                  : COMPRESS_MODE_NONE;
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_FBC;
          }
          child_item = cJSON_GetObjectItem(child_property_param, "fps");
          if (child_item) {
            RK_S32 cnt = 0;
            cnt = sscanf(child_item->valuestring, "%d:%d", &mpiVpssCfg->fps_in,
                         &mpiVpssCfg->fps_out);
            if (!cnt) {
              LOG_ERROR("invalid set vpss fps in:out,such as 30:25\n");
            } else {
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_FPS;
            }
          }
          child_item = cJSON_GetObjectItem(child_property_param, "format");
          if (child_item) {
            mpiVpssCfg->format =
                pix_format_rt_string_to_gl(child_item->valuestring);
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_FORMAT;
            LOG_INFO("mpiVpssCfg[%d]->format:0x%x\n", chnType,
                     mpiVpssCfg->format);
          }
          child_item =
              cJSON_GetObjectItem(child_property_param, "assign_width");
          if (child_item) {
            mpiVpssCfg->assign_width = child_item->valueint;
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_ASSIGN_WIDTH;
          }
          child_item =
              cJSON_GetObjectItem(child_property_param, "assign_height");
          if (child_item) {
            mpiVpssCfg->assign_height = child_item->valueint;
            mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_ASSIGN_HEIGHT;
          }
          child_item = cJSON_GetObjectItem(child_property_param, "rotation");
          if (child_item) {
            switch (child_item->valueint) {
            case 0:
              mpiVpssCfg->rotation = ROTATION_0;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_ROTATION;
              break;
            case 90:
              mpiVpssCfg->rotation = ROTATION_90;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_ROTATION;
              break;
            case 180:
              mpiVpssCfg->rotation = ROTATION_180;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_ROTATION;
              break;
            case 270:
              mpiVpssCfg->rotation = ROTATION_270;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_ROTATION;
              break;
            default:
              LOG_WARN("rotation:%d not support\n", child_item->valueint);
              mpiVpssCfg->rotation = ROTATION_0;
              break;
            }
          }
          child_item = cJSON_GetObjectItem(child_property_param, "mirror");
          if (child_item) {
            switch (child_item->valueint) {
            case 0:
              mpiVpssCfg->mirror = MIRROR_NONE;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_MIRROR;
              break;
            case 1:
              mpiVpssCfg->mirror = MIRROR_HORIZONTAL;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_MIRROR;
              break;
            case 2:
              mpiVpssCfg->mirror = MIRROR_VERTICAL;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_MIRROR;
              break;
            case 3:
              mpiVpssCfg->mirror = MIRROR_BOTH;
              mpiVpssCfg->change |= MPI_VPSS_CFG_CHANGE_MIRROR;
              break;
            default:
              LOG_WARN("mirror:%d not support\n", child_item->valueint);
              mpiVpssCfg->mirror = MIRROR_NONE;
              break;
            }
          }
          LOG_INFO("vpss_cfg change:0x%x\n", mpiVpssCfg->change);
        } else {
          LOG_WARN("no find vpss_cfg param_init or param_change\n");
        }
      }
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_venc_common_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                               bool init) {
  int ret = 0;

  MpiVencCommonCfg *mpiVencCommonCfg = &mpiCfg->venc_cfg.common_cfg;
  if (NULL == mpiVencCommonCfg)
    return -1;

  mpiVencCommonCfg->change = 0;
  cJSON *child_common = cJSON_GetObjectItem(root, "common");
  if (!child_common) {
    LOG_ERROR("parse venc_cfg common err\n");
    return -1;
  } else {
    cJSON *child_common_param = NULL;
    if (init) {
      child_common_param = cJSON_GetObjectItem(child_common, "param_init");
      mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_CREATE;
    } else {
      child_common_param = cJSON_GetObjectItem(child_common, "param_change");
    }
    if (child_common_param) {
      cJSON *child_item = cJSON_GetObjectItem(child_common_param, "rotation");
      if (child_item) {
        switch (child_item->valueint) {
        case 0:
          mpiVencCommonCfg->rotation = ROTATION_0;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_ROTATION;
          break;
        case 90:
          mpiVencCommonCfg->rotation = ROTATION_90;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_ROTATION;
          break;
        case 180:
          mpiVencCommonCfg->rotation = ROTATION_180;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_ROTATION;
          break;
        case 270:
          mpiVencCommonCfg->rotation = ROTATION_270;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_ROTATION;
          break;
        default:
          LOG_WARN("rotation:%d not support\n", child_item->valueint);
          mpiVencCommonCfg->rotation = ROTATION_0;
          break;
        }
      }
      child_item = cJSON_GetObjectItem(child_common_param, "mirror");
      if (child_item) {
        switch (child_item->valueint) {
        case 0:
          mpiVencCommonCfg->mirror = MIRROR_NONE;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_MIRROR;
          break;
        case 1:
          mpiVencCommonCfg->mirror = MIRROR_HORIZONTAL;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_MIRROR;
          break;
        case 2:
          mpiVencCommonCfg->mirror = MIRROR_VERTICAL;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_MIRROR;
          break;
        case 3:
          mpiVencCommonCfg->mirror = MIRROR_BOTH;
          mpiVencCommonCfg->change |= MPI_VENC_COMMON_CFG_CHANGE_MIRROR;
          break;
        default:
          LOG_WARN("mirror:%d not support\n", child_item->valueint);
          mpiVencCommonCfg->mirror = MIRROR_NONE;
          break;
        }
      }
      child_item = cJSON_GetObjectItem(child_common_param, "channel_id");
      if (child_item) {
        mpiVencCommonCfg->channel_id = child_item->valueint;
      }
      LOG_INFO("venc_cfg common change:0x%x\n", mpiVencCommonCfg->change);
    } else {
      LOG_WARN("no find venc_cfg common param_init or param_change\n");
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_venc_mjpeg_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                              bool init) {
  int ret = 0;
  RK_U32 fps;
  RK_U32 loop = 1;
  cJSON *child_resolution_param = NULL;

  MpiVencMjpegCfg *mpiVencMjpegCfg = &mpiCfg->venc_cfg.mjpeg_cfg;
  if (NULL == mpiVencMjpegCfg)
    return -1;

  mpiVencMjpegCfg->change = 0;
  cJSON *child_mjpeg = cJSON_GetObjectItem(root, "mjpeg");
  if (!child_mjpeg) {
    LOG_ERROR("parse venc_cfg mjpeg err\n");
    return -1;
  } else {
    cJSON *child_mjpeg_param = NULL;
    if (init) {
      cJSON *child_mjpeg_param_init =
          cJSON_GetObjectItem(child_mjpeg, "param_init");
      char resolution_name[15] = "";
      sprintf(resolution_name, "%d*%dp%d", mpiCfg->uvc_cfg.width,
              mpiCfg->uvc_cfg.height, mpiCfg->uvc_cfg.fps);
      child_resolution_param =
          cJSON_GetObjectItem(child_mjpeg_param_init, resolution_name);
      if (child_resolution_param) {
        loop = 2;
      }
      child_mjpeg_param =
          cJSON_GetObjectItem(child_mjpeg_param_init, "default");
    } else {
      child_mjpeg_param = cJSON_GetObjectItem(child_mjpeg, "param_change");
    }

    for (RK_U32 parse = 0; parse < loop; parse++) {
      if (parse == 1) {
        child_mjpeg_param = child_resolution_param;
      }
      if (child_mjpeg_param) {
        cJSON *child_item = cJSON_GetObjectItem(child_mjpeg_param, "buf_cnt");
        if (child_item) {
          mpiVencMjpegCfg->buf_cnt = child_item->valueint;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_BUF_CNT;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "fbc");
        if (child_item) {
          mpiVencMjpegCfg->fbc = strstr(child_item->valuestring, "on")
                                     ? COMPRESS_AFBC_16x16
                                     : COMPRESS_MODE_NONE;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_FBC;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "rc_mode");
        if (child_item) {
          mpiVencMjpegCfg->rc_mode =
              strstr(child_item->valuestring, "cbr")
                  ? VENC_RC_MODE_MJPEGCBR
                  : strstr(child_item->valuestring, "vbr")
                        ? VENC_RC_MODE_MJPEGVBR
                        : strstr(child_item->valuestring, "fixqp")
                              ? VENC_RC_MODE_MJPEGFIXQP
                              : VENC_RC_MODE_MJPEGCBR;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_RC_MODE;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "fps");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(child_item->valuestring, "%d:%d",
                       &mpiVencMjpegCfg->fps_in, &mpiVencMjpegCfg->fps_out);
          if (!cnt) {
            LOG_ERROR("invalid set mjpeg fps in:out,such as 30:25\n");
          } else {
            mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_FPS;
          }
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "range");
        if (child_item) {
          mpiVencMjpegCfg->range_full =
              strstr(child_item->valuestring, "full") ? 1 : 0;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_RANGE;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "sei");
        if (child_item) {
          mpiVencMjpegCfg->sei = child_item->valueint;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_SEI;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "qfactor");
        if (child_item) {
          mpiVencMjpegCfg->qp.stParamMjpeg.u32Qfactor = child_item->valueint;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "qfactor_min");
        if (child_item) {
          mpiVencMjpegCfg->qfactor_min = child_item->valueint;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR_MIN;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "qfactor_max");
        if (child_item) {
          mpiVencMjpegCfg->qfactor_max = child_item->valueint;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_QFACTOR_MAX;
        }
        child_item = cJSON_GetObjectItem(child_mjpeg_param, "bps");
        if (child_item) {
          mpiVencMjpegCfg->bps = child_item->valueint;
          mpiVencMjpegCfg->change |= MPI_VENC_MJPEG_CFG_CHANGE_BPS;
        }
        child_mjpeg_param = child_resolution_param;
      } else {
        LOG_WARN("no find venc_cfg mjpeg param_init or param_change\n");
      }
      LOG_INFO("venc_cfg mjpeg change:0x%x\n", mpiVencMjpegCfg->change);
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_venc_h264_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                             bool init) {
  int ret = 0;
  RK_U32 fps;
  RK_U32 loop = 1;
  cJSON *child_resolution_param = NULL;

  MpiVencH264Cfg *cfg = &mpiCfg->venc_cfg.h264_cfg;
  if (NULL == cfg)
    return -1;

  cfg->change = 0;
  cJSON *child = cJSON_GetObjectItem(root, "h264");
  if (!child) {
    LOG_ERROR("parse venc_cfg h264 err\n");
    return -1;
  } else {
    cJSON *child_param = NULL;
    if (init) {
      cJSON *child_h264_param_init = cJSON_GetObjectItem(child, "param_init");
      char resolution_name[15] = "";
      sprintf(resolution_name, "%d*%dp%d", mpiCfg->uvc_cfg.width,
              mpiCfg->uvc_cfg.height, mpiCfg->uvc_cfg.fps);
      child_resolution_param =
          cJSON_GetObjectItem(child_h264_param_init, resolution_name);
      if (child_resolution_param) {
        loop = 2;
      }
      child_param = cJSON_GetObjectItem(child_h264_param_init, "default");
    } else {
      child_param = cJSON_GetObjectItem(child, "param_change");
    }

    for (RK_U32 parse = 0; parse < loop; parse++) {
      if (parse == 1) {
        child_param = child_resolution_param;
      }
      if (child_param) {
        cJSON *child_item = cJSON_GetObjectItem(child_param, "buf_cnt");
        if (child_item) {
          cfg->buf_cnt = child_item->valueint;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_BUF_CNT;
        }
        child_item = cJSON_GetObjectItem(child_param, "fbc");
        if (child_item) {
          cfg->fbc = strstr(child_item->valuestring, "on") ? COMPRESS_AFBC_16x16
                                                           : COMPRESS_MODE_NONE;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_FBC;
        }
        child_item = cJSON_GetObjectItem(child_param, "gop");
        if (child_item) {
          cfg->gop = child_item->valueint;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP;
        }
        child_item = cJSON_GetObjectItem(child_param, "rc_mode");
        if (child_item) {
          cfg->rc_mode =
              strstr(child_item->valuestring, "cbr")
                  ? VENC_RC_MODE_H264CBR
                  : strstr(child_item->valuestring, "avbr")
                        ? VENC_RC_MODE_H264AVBR
                        : strstr(child_item->valuestring, "vbr")
                              ? VENC_RC_MODE_H264VBR
                              : strstr(child_item->valuestring, "fixqp")
                                    ? VENC_RC_MODE_H264FIXQP
                                    : VENC_RC_MODE_H264CBR;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_RC_MODE;
        }
        child_item = cJSON_GetObjectItem(child_param, "fps");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(child_item->valuestring, "%d:%d", &cfg->fps_in,
                       &cfg->fps_out);
          if (!cnt) {
            LOG_ERROR("invalid set H264 fps in:out,such as 30:25\n");
          } else {
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_FPS;
          }
        }
        child_item = cJSON_GetObjectItem(child_param, "range");
        if (child_item) {
          cfg->range_full = strstr(child_item->valuestring, "full") ? 1 : 0;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_RANGE;
        }
        child_item = cJSON_GetObjectItem(child_param, "qp");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(
              child_item->valuestring, "%d:%d:%d:%d:%d:%d:%d:%d",
              &cfg->qp.s32FirstFrameStartQp, &cfg->qp.stParamH264.u32StepQp,
              &cfg->qp.stParamH264.u32MinIQp, &cfg->qp.stParamH264.u32MaxIQp,
              &cfg->qp.stParamH264.u32MinQp, &cfg->qp.stParamH264.u32MaxQp,
              &cfg->qp.stParamH264.s32DeltIpQp,
              &cfg->qp.stParamH264.s32MaxReEncodeTimes);
          if (!cnt) {
            LOG_ERROR("invalid set H264 qp "
                      "init:step:minI:maxI:minP:maxP:deltaIP:reEncodeCnt,"
                      "such as 26:4:20:40:8:48:2:1\n");
          } else {
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_QP;
          }
        }
        child_item = cJSON_GetObjectItem(child_param, "sei");
        if (child_item) {
          cfg->sei = child_item->valueint;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_SEI;
        }
        child_item = cJSON_GetObjectItem(child_param, "profile");
        if (child_item) {
          cfg->profile = child_item->valueint;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_PROFILE;
        }
        child_item = cJSON_GetObjectItem(child_param, "vi_len");
        if (child_item) {
          cfg->gop_mode.s32VirIdrLen = child_item->valueint;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_VI_LEN;
        }
        child_item = cJSON_GetObjectItem(child_param, "gop_mode");
        if (child_item) {
          switch (child_item->valueint) {
          case 0:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_INIT;
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP_MODE;
            break;
          case 1:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_NORMALP;
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP_MODE;
            break;
          case 2:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_TSVC2;
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP_MODE;
            break;
          case 3:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_TSVC3;
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP_MODE;
            break;
          case 4:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_TSVC4;
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP_MODE;
            break;
          case 5:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_SMARTP;
            cfg->change |= MPI_VENC_H264_CFG_CHANGE_GOP_MODE;
            break;
          default:
            LOG_WARN("gop_mode:%d not support\n", child_item->valueint);
            cfg->gop_mode.enGopMode = VENC_GOPMODE_INIT;
            break;
          }
        }
        child_item = cJSON_GetObjectItem(child_param, "bps");
        if (child_item) {
          cfg->bps = child_item->valueint;
          cfg->change |= MPI_VENC_H264_CFG_CHANGE_BPS;
        }
        child_item = cJSON_GetObjectItem(child_param, "force_idr");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(child_item->valuestring, "%d:%d:%d",
                       &mpiCfg->venc_cfg.common_cfg.idr_gop,
                       &mpiCfg->venc_cfg.common_cfg.idr_cnt,
                       &mpiCfg->venc_cfg.common_cfg.idr_bps);
          if (!cnt) {
            LOG_ERROR("invalid set H264 force_idr idr_gop:idr_cnt:idr_bps,such "
                      "as 5:5:50000\n");
          } else {
            cfg->change |= MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR;
          }
        }
      } else {
        LOG_WARN("no find venc_cfg H264 param_init or param_change\n");
      }
      LOG_INFO("venc_cfg H264 change:0x%x\n", cfg->change);
    }
    // set to idr info for the begin frame
    if (cfg->change & MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR &&
        (mpiCfg->venc_cfg.common_cfg.idr_gop *
         mpiCfg->venc_cfg.common_cfg.idr_cnt *
         mpiCfg->venc_cfg.common_cfg.idr_bps)) {
      mpiCfg->venc_cfg.common_cfg.normal_bps = cfg->bps;
      mpiCfg->venc_cfg.common_cfg.normal_gop = cfg->gop;
      cfg->gop = mpiCfg->venc_cfg.common_cfg.idr_gop;
      cfg->bps = mpiCfg->venc_cfg.common_cfg.idr_bps;
      LOG_INFO("force_idr:start gop:%d bps:%d\n", cfg->gop, cfg->bps);
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_venc_h265_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                             bool init) {
  int ret = 0;
  RK_U32 fps;
  RK_U32 loop = 1;
  cJSON *child_resolution_param = NULL;

  MpiVencH265Cfg *cfg = &mpiCfg->venc_cfg.h265_cfg;
  if (NULL == cfg)
    return -1;

  cfg->change = 0;
  cJSON *child = cJSON_GetObjectItem(root, "h265");
  if (!child) {
    LOG_ERROR("parse venc_cfg h265 err\n");
    return -1;
  } else {
    cJSON *child_param = NULL;
    if (init) {
      cJSON *child_h265_param_init = cJSON_GetObjectItem(child, "param_init");
      char resolution_name[15] = "";
      sprintf(resolution_name, "%d*%dp%d", mpiCfg->uvc_cfg.width,
              mpiCfg->uvc_cfg.height, mpiCfg->uvc_cfg.fps);
      child_resolution_param =
          cJSON_GetObjectItem(child_h265_param_init, resolution_name);
      if (child_resolution_param) {
        loop = 2;
      }
      child_param = cJSON_GetObjectItem(child_h265_param_init, "default");
    } else {
      child_param = cJSON_GetObjectItem(child, "param_change");
    }

    for (RK_U32 parse = 0; parse < loop; parse++) {
      if (parse == 1) {
        child_param = child_resolution_param;
      }
      if (child_param) {
        cJSON *child_item = cJSON_GetObjectItem(child_param, "buf_cnt");
        if (child_item) {
          cfg->buf_cnt = child_item->valueint;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_BUF_CNT;
        }
        child_item = cJSON_GetObjectItem(child_param, "fbc");
        if (child_item) {
          cfg->fbc = strstr(child_item->valuestring, "on") ? COMPRESS_AFBC_16x16
                                                           : COMPRESS_MODE_NONE;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_FBC;
        }
        child_item = cJSON_GetObjectItem(child_param, "gop");
        if (child_item) {
          cfg->gop = child_item->valueint;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP;
        }
        child_item = cJSON_GetObjectItem(child_param, "rc_mode");
        if (child_item) {
          cfg->rc_mode =
              strstr(child_item->valuestring, "cbr")
                  ? VENC_RC_MODE_H265CBR
                  : strstr(child_item->valuestring, "avbr")
                        ? VENC_RC_MODE_H265AVBR
                        : strstr(child_item->valuestring, "vbr")
                              ? VENC_RC_MODE_H265VBR
                              : strstr(child_item->valuestring, "fixqp")
                                    ? VENC_RC_MODE_H265FIXQP
                                    : VENC_RC_MODE_H265CBR;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_RC_MODE;
        }
        child_item = cJSON_GetObjectItem(child_param, "fps");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(child_item->valuestring, "%d:%d", &cfg->fps_in,
                       &cfg->fps_out);
          if (!cnt) {
            LOG_ERROR("invalid set H265 fps in:out,such as 30:25\n");
          } else {
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_FPS;
          }
        }
        child_item = cJSON_GetObjectItem(child_param, "range");
        if (child_item) {
          cfg->range_full = strstr(child_item->valuestring, "full") ? 1 : 0;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_RANGE;
        }
        child_item = cJSON_GetObjectItem(child_param, "qp");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(
              child_item->valuestring, "%d:%d:%d:%d:%d:%d:%d:%d",
              &cfg->qp.s32FirstFrameStartQp, &cfg->qp.stParamH265.u32StepQp,
              &cfg->qp.stParamH265.u32MinIQp, &cfg->qp.stParamH265.u32MaxIQp,
              &cfg->qp.stParamH265.u32MinQp, &cfg->qp.stParamH265.u32MaxQp,
              &cfg->qp.stParamH265.s32DeltIpQp,
              &cfg->qp.stParamH265.s32MaxReEncodeTimes);
          if (!cnt) {
            LOG_ERROR("invalid set H265 qp "
                      "init:step:minI:maxI:minP:maxP:deltaIP:reEncodeCnt,"
                      "such as 26:4:20:40:8:48:2:1\n");
          } else {
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_QP;
          }
        }
        child_item = cJSON_GetObjectItem(child_param, "sei");
        if (child_item) {
          cfg->sei = child_item->valueint;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_SEI;
        }
        child_item = cJSON_GetObjectItem(child_param, "profile");
        if (child_item) {
          cfg->profile = child_item->valueint;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_PROFILE;
        }
        child_item = cJSON_GetObjectItem(child_param, "vi_len");
        if (child_item) {
          cfg->gop_mode.s32VirIdrLen = child_item->valueint;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_VI_LEN;
        }
        child_item = cJSON_GetObjectItem(child_param, "gop_mode");
        if (child_item) {
          switch (child_item->valueint) {
          case 0:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_INIT;
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP_MODE;
            break;
          case 1:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_NORMALP;
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP_MODE;
            break;
          case 2:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_TSVC2;
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP_MODE;
            break;
          case 3:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_TSVC3;
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP_MODE;
            break;
          case 4:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_TSVC4;
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP_MODE;
            break;
          case 5:
            cfg->gop_mode.enGopMode = VENC_GOPMODE_SMARTP;
            cfg->change |= MPI_VENC_H265_CFG_CHANGE_GOP_MODE;
            break;
          default:
            LOG_WARN("gop_mode:%d not support\n", child_item->valueint);
            cfg->gop_mode.enGopMode = VENC_GOPMODE_INIT;
            break;
          }
        }
        child_item = cJSON_GetObjectItem(child_param, "bps");
        if (child_item) {
          cfg->bps = child_item->valueint;
          cfg->change |= MPI_VENC_H265_CFG_CHANGE_BPS;
        }
        child_item = cJSON_GetObjectItem(child_param, "force_idr");
        if (child_item) {
          RK_S32 cnt = 0;
          cnt = sscanf(child_item->valuestring, "%d:%d:%d",
                       &mpiCfg->venc_cfg.common_cfg.idr_gop,
                       &mpiCfg->venc_cfg.common_cfg.idr_cnt,
                       &mpiCfg->venc_cfg.common_cfg.idr_bps);
          if (!cnt) {
            LOG_ERROR("invalid set H265 force_idr idr_gop:idr_cnt:idr_bps,such "
                      "as 5:5:50000\n");
          } else {
            cfg->change |= MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR;
          }
        }
      } else {
        LOG_WARN("no find venc_cfg H265 param_init or param_change\n");
      }
      LOG_INFO("venc_cfg H265 change:0x%x\n", cfg->change);
    }
    // set to idr info for the begin frame
    if (cfg->change & MPI_VENC_COMMON_CFG_CHANGE_CHN_ATTR &&
        (mpiCfg->venc_cfg.common_cfg.idr_gop *
         mpiCfg->venc_cfg.common_cfg.idr_cnt *
         mpiCfg->venc_cfg.common_cfg.idr_bps)) {
      mpiCfg->venc_cfg.common_cfg.normal_bps = cfg->bps;
      mpiCfg->venc_cfg.common_cfg.normal_gop = cfg->gop;
      cfg->gop = mpiCfg->venc_cfg.common_cfg.idr_gop;
      cfg->bps = mpiCfg->venc_cfg.common_cfg.idr_bps;
      LOG_INFO("force_idr:start gop:%d bps:%d\n", cfg->gop, cfg->bps);
    }
  }

  return ret;
}

static int parse_check_uvc_mpi_venc_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                        bool init) {
  int ret = 0;

  cJSON *child_venc_cfg = cJSON_GetObjectItem(root, "venc_cfg");
  if (!child_venc_cfg) {
    LOG_ERROR("parse venc_cfg err\n");
    return -1;
  }

  switch (mpiCfg->uvc_cfg.fcc) {
  case V4L2_PIX_FMT_YUYV:
  case V4L2_PIX_FMT_NV12:
    break;
  case V4L2_PIX_FMT_MJPEG:
    ret |= parse_check_uvc_mpi_venc_common_cfg(child_venc_cfg, mpiCfg, init);
    ret |= parse_check_uvc_mpi_venc_mjpeg_cfg(child_venc_cfg, mpiCfg, init);
    break;
  case V4L2_PIX_FMT_H264:
    ret |= parse_check_uvc_mpi_venc_common_cfg(child_venc_cfg, mpiCfg, init);
    ret |= parse_check_uvc_mpi_venc_h264_cfg(child_venc_cfg, mpiCfg, init);
    break;
  case V4L2_PIX_FMT_H265:
    ret |= parse_check_uvc_mpi_venc_common_cfg(child_venc_cfg, mpiCfg, init);
    ret |= parse_check_uvc_mpi_venc_h265_cfg(child_venc_cfg, mpiCfg, init);
    break;
  default:
    LOG_ERROR("not support this fcc 0x%x\n", mpiCfg->uvc_cfg.fcc);
    break;
  }

  return ret;
}
#if UVC_EPTZ
static int parse_check_uvc_eptz_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                    bool init) {
  int ret = 0;
  cJSON *child_property_param = NULL;

  MpiUvcEptzCfg *eptzCfg = &mpiCfg->eptz_cfg;
  if (NULL == eptzCfg)
    return -1;

  eptzCfg->change = 0;
  cJSON *child = cJSON_GetObjectItem(root, "eptz_cfg");
  if (!child) {
    LOG_ERROR("parse eptz_cfg err\n");
    return -1;
  }
  cJSON *child_version = cJSON_GetObjectItem(child, "version");
  if (child_version) {
    LOG_DEBUG("parse eptz version:%d\n", child_version->valueint);
    eptzCfg->version = child_version->valueint;
  }

  if (init)
    child_property_param = cJSON_GetObjectItem(child, "param_init");
  else
    child_property_param = cJSON_GetObjectItem(child, "param_change");
  if (child_property_param) {
    cJSON *child_item = cJSON_GetObjectItem(child_property_param, "debug");
    if (child_item) {
      eptzCfg->debug = strstr(child_item->valuestring, "on") ? 1 : 0;
      eptzCfg->change |= MPI_EPTZ_CFG_CHANGE_ENABLE_DEBUG;
    }
    child_item = cJSON_GetObjectItem(child_property_param, "enable");
    if (child_item) {
      eptzCfg->enable = strstr(child_item->valuestring, "on") ? 1 : 0;
      eptzCfg->change |= MPI_EPTZ_CFG_CHANGE_ENABLE_EPTZ;
    }
    child_item = cJSON_GetObjectItem(child_property_param, "enable_boot");
    if (child_item) {
      eptzCfg->enable_boot = strstr(child_item->valuestring, "on") ? 1 : 0;
      eptzCfg->change |= MPI_EPTZ_CFG_CHANGE_ENABLE_EPTZ_BOOT;
    }

    LOG_INFO("uvc eptz change:0x%x\n", eptzCfg->change);
  }

  return ret;
}
#endif
#if UVC_OSD
static int parse_check_uvc_osd_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg,
                                   bool init) {
  int ret = 0;
  cJSON *child_property_param = NULL;

  MpiOsdCfg *osdCfg = &mpiCfg->osd_cfg;
  if (NULL == osdCfg)
    return -1;

  osdCfg->change = 0;
  cJSON *child = cJSON_GetObjectItem(root, "osd_cfg");
  if (!child) {
    LOG_ERROR("parse osd_cfg err\n");
    return -1;
  }
  cJSON *child_version = cJSON_GetObjectItem(child, "version");
  if (child_version) {
    LOG_DEBUG("parse osd version:%d\n", child_version->valueint);
    osdCfg->version = child_version->valueint;
  }

  if (init)
    child_property_param = cJSON_GetObjectItem(child, "param_init");
  else
    child_property_param = cJSON_GetObjectItem(child, "param_change");
  if (child_property_param) {
    cJSON *child_item = cJSON_GetObjectItem(child_property_param, "enable");
    if (child_item) {
      osdCfg->enable = strstr(child_item->valuestring, "on") ? 1 : 0;
      osdCfg->change |= MPI_OSD_CFG_CHANGE_ENABLE_OSD;
    }

    LOG_INFO("uvc osd change:0x%x\n", osdCfg->change);
  }

  return ret;
}
#endif

#if UVC_OSD
static int parse_check_uvc_mpi_osd_cfg(cJSON *root, UVC_MPI_CFG *mpiCfg, bool init) {
    int ret = 0;
    RK_U32 fps;
    RK_U32 loop = 1;
    cJSON *child_resolution_param = NULL; 

    MpiVencH265Cfg *cfg = &mpiCfg->venc_cfg.h265_cfg;
    if (NULL == cfg)
        return -1;

    cJSON *child_osd = cJSON_GetObjectItem(child, "osd");
    if (!child_osd)
    {
        LOG_INFO("no osd info\n");
    }
    else
    {
        cJSON *child_osd_enable = cJSON_GetObjectItem(child_osd, "enable");
        if (child_osd_enable)
        {
            mpiVencCfg->osd_enable = strstr(child_osd_enable->valuestring, "on") ?
                                   true : false;
        }
        cJSON *child_osd_count = cJSON_GetObjectItem(child_osd, "count");
        if (child_osd_count)
        {
            mpiVencCfg->osd_count = child_osd_count->valueint;
            mpiVencCfg->osd_count = mpiVencCfg->osd_count < 0 ? 0 :
                           mpiVencCfg->osd_count > OSD_REGIONS_CNT ? OSD_REGIONS_CNT :
                           mpiVencCfg->osd_count;
        }
        cJSON *child_osd_plt_user = cJSON_GetObjectItem(child_osd, "plt_user");
        if (child_osd_plt_user)
        {
            mpiVencCfg->osd_plt_user = child_osd_plt_user->valueint;
        }

        char osd_name[6] = "osd_0";
        for (int i = 0; i < mpiVencCfg->osd_count; i ++)
        {
            osd_name[4] = i + 48;
            cJSON *child_osd_index = cJSON_GetObjectItem(child_osd, osd_name);
            if (child_osd_index)
            {
                mpiVencCfg->osd_cfg[i].set_ok = true;
                cJSON *child_osd_index_type = cJSON_GetObjectItem(child_osd_index, "type");;
                if (child_osd_index_type)
                {
                     mpiVencCfg->osd_cfg[i].type = strstr(child_osd_index_type->valuestring, "picture") ?
                                                 OSD_REGION_TYPE_PICTURE : OSD_REGION_TYPE_PICTURE;
                }
                cJSON *child_osd_index_enable = cJSON_GetObjectItem(child_osd_index, "enable");
                if (child_osd_index_enable)
                {
                     mpiVencCfg->osd_cfg[i].enable = strstr(child_osd_index_enable->valuestring, "on") ?
                                                 true : false;
                }

                char resolution_name[10] = "";
                sprintf(resolution_name, "%d*%d",mpiVencCfg->width, mpiVencCfg->height);
                cJSON *child_osd_resolution_name = cJSON_GetObjectItem(child_osd_index, resolution_name);
                if (!child_osd_resolution_name) {
                    child_osd_resolution_name = cJSON_GetObjectItem(child_osd_index, "common");
                }
                if (child_osd_resolution_name) {
                    cJSON *child_osd_index_path = cJSON_GetObjectItem(child_osd_resolution_name, "path");
                    if (child_osd_index_path) {
                        if (strlen(child_osd_index_path->valuestring) >
                            (MPP_ENC_OSD_IMAGE_PATH_LEN - 1)) {
                            LOG_ERROR("osd img path:%s, the name too long\n",
                                       child_osd_index_path->valuestring);
                            mpiVencCfg->osd_cfg[i].enable = false;
                            mpiVencCfg->osd_cfg[i].set_ok = false;
                        } else {
                            sprintf(mpiVencCfg->osd_cfg[i].image_path, "%s",
                                    child_osd_index_path->valuestring);
                        }
                    }
                    cJSON *child_osd_index_start_x = cJSON_GetObjectItem(child_osd_resolution_name, "start_x");
                    if (child_osd_index_start_x)
                    {
                         mpiVencCfg->osd_cfg[i].start_x = UPALIGNTO16((int)(child_osd_index_start_x->valuedouble * mpiVencCfg->width));
                    }
                    cJSON *child_osd_index_start_y = cJSON_GetObjectItem(child_osd_resolution_name, "start_y");
                    if (child_osd_index_start_y)
                    {
                         mpiVencCfg->osd_cfg[i].start_y = UPALIGNTO16((int)(child_osd_index_start_y->valuedouble * mpiVencCfg->height));
                    }
                }
                else
                {
                    LOG_INFO("no such resolution_name:%s && common path\n", resolution_name);
                }
            }
            else
            {
                LOG_INFO("no such osd %s\n", osd_name);
            }
        }
    }
    return ret;
}
#endif

static int parse_check_uvc_mpi_cfg(cJSON *root, UVC_MPI_CFG *cfg, bool init) {
  int ret = 0;

  LOG_INFO("parse_uvc_mpi index: %d init:%d\n", cfg->index, init);
  cJSON *index_root = parse_find_uvc_index_cfg(root, cfg->index, init);
  if (index_root) {
    cJSON *index_version = cJSON_GetObjectItem(index_root, "version");
    if (index_version) {
      LOG_INFO("parse index_version:%d\n", index_version->valueint);
      cfg->version = index_version->valueint;
    }
    ret |= parse_check_uvc_common_cfg(index_root, cfg, init);
    ret |= parse_check_uvc_mpi_vi_cfg(index_root, cfg, init);
    ret |= parse_check_uvc_mpi_vpss_cfg(index_root, cfg, init);
    ret |= parse_check_uvc_mpi_venc_cfg(index_root, cfg, init);
#if UVC_EPTZ
    ret |= parse_check_uvc_eptz_cfg(index_root, cfg, init);
#endif
#if UVC_OSD
    ret |= parse_check_uvc_osd_cfg(index_root, cfg, init);
#endif
  } else {
    ret = -1;
  }

  return ret;
}

static int read_uvc_mpi_cfg_modify_file(UVC_MPI_CFG *mpiCfg, bool init) {
  int ret = -1;
  unsigned long read_size = 0;

  FILE *modify_fd = fopen(UVC_MPI_CFG_MODIFY_PATH, "rb");
  unsigned long size = get_file_size(UVC_MPI_CFG_MODIFY_PATH);
  LOG_DEBUG("get cfg size=%ld\n", size);
  char *cfg = (char *)malloc(size);
  while (read_size != size) {
    read_size += fread(cfg, 1, size - read_size, modify_fd);
  }
  // LOG_INFO("get cfg =%s read_size=%ld\n", cfg, read_size);
  cJSON *root = cJSON_Parse(cfg);
  if (root == NULL) {
    LOG_ERROR("the %s is broken\n", UVC_MPI_CFG_MODIFY_PATH);
  } else {
    ret = parse_check_uvc_mpi_cfg(root, mpiCfg, init);
  }
  if (modify_fd)
    fclose(modify_fd);
  if (cfg)
    free(cfg);
  if (root)
    cJSON_Delete(root);

  return ret;
}

int check_uvc_mpi_cfg_file_init(UVC_MPI_CFG *cfg) {
  int ret = -1;
  char cmd[128] = {0};
  if (NULL == cfg)
    return -1;

  if (!access(UVC_MPI_CFG_MODIFY_PATH, F_OK)) {
    ret = read_uvc_mpi_cfg_modify_file(cfg, true);
  }

  if (ret) {
    if (!access(UVC_MPI_CFG_ORIGINAL_PATH, F_OK)) {
      sprintf(cmd, "cp %s %s", UVC_MPI_CFG_ORIGINAL_PATH,
              UVC_MPI_CFG_MODIFY_PATH);
      system(cmd);
      LOG_DEBUG("copy enc cfg file...\n");
      ret = read_uvc_mpi_cfg_modify_file(cfg, true);
    } else {
      LOG_ERROR("file :%s not exit!\n", UVC_MPI_CFG_ORIGINAL_PATH);
      ret = -1;
    }
  }

  /*   if (cfg->common_cfg.need_check) {
         mpiVencCfg->cfg_notify_fd = inotify_init();
     }*/

  return ret;
}

#if 0
void *thread_check_mpp_enc_chenge_loop(void *user) {
    int ret = 0;
    int mpi_ret;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, PTHREAD_CREATE_JOINABLE);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    MpiEncTestData *mpiVencCfg = (MpiEncTestData *)user;
    int last_wd = 0;
    unsigned char buf[1024] = {0};
    struct inotify_event *event = NULL;
    bool modify = false;

    while (1) {
        last_wd = mpiVencCfg->cfg_notify_wd;
        mpiVencCfg->cfg_notify_wd = inotify_add_watch(mpiVencCfg->cfg_notify_fd,
                                             RK_UVC_MPI_CFG_MODIFY_PATH,
                                             IN_MODIFY | IN_DELETE_SELF);
        modify = false;
        if (mpiVencCfg->cfg_notify_wd != -1) {
            if ((last_wd == -1 && mpiVencCfg->cfg_notify_wd == 1)) {
                modify = true;
            } else {
                fd_set fds;
                FD_ZERO(&fds);
                FD_SET(mpiVencCfg->cfg_notify_fd, &fds);

                if (select(mpiVencCfg->cfg_notify_fd + 1, &fds, NULL, NULL, NULL) > 0) {
                    int len, index = 0;
                    memset(buf, 0, sizeof(buf));
                    len = read(mpiVencCfg->cfg_notify_fd, &buf, sizeof(buf));
                    while (len > 0 && index < len) {
                        event = (struct inotify_event *)(buf + index);
                        if (event->mask == IN_MODIFY) {
                            modify = true;
                        } else if (event->mask == IN_DELETE_SELF) {
                            mpiVencCfg->cfg_notify_wd = -1;
                        }
                        index += sizeof(struct inotify_event) + event->len;
                    }
                }

            }
        }
        sleep(1);
        if (modify) {
            LOG_INFO("the enc cfg file change or creat, do update.wd=%d,last_wd=%d\n", mpiVencCfg->cfg_notify_wd, last_wd);
            ret = read_uvc_mpi_cfg_modify_file(mpiVencCfg, false);
            if (ret)
                LOG_ERROR("error: the enc cfg file is broken.please check.\n");
            else {
                mpp_ret = mpp_enc_cfg_set(mpiVencCfg, false);
                dump_mpp_enc_cfg(mpiVencCfg);
                if (mpp_ret) {
                    LOG_ERROR("mpp_enc_cfg_set failed ret %d\n", mpp_ret);
                }
            }
        }

        inotify_rm_watch(mpiVencCfg->cfg_notify_fd, mpiVencCfg->cfg_notify_wd);
        close(mpiVencCfg->cfg_notify_fd);
        mpiVencCfg->cfg_notify_fd = inotify_init();
    }
}
#endif

void uvc_mpi_cfg_set_default(UVC_MPI_CFG *mpiCfg) {
  // common cfg
  mpiCfg->common_cfg.change = MPI_UVC_COMMON_CFG_CHANGE_ALL;
  mpiCfg->common_cfg.need_check = 0;
  mpiCfg->common_cfg.uvc_debug = 0;
  mpiCfg->common_cfg.uvc_debug_cnt = 0;
  copy_str_to_array(mpiCfg->common_cfg.uvc_debug_file,
                    UVC_MPI_CFG_DEBUG_FILE_NAME, UVC_MPI_STREAM_SAVE_FILE_LEN);
  copy_str_to_array(mpiCfg->common_cfg.nn_debug_file,
                    UVC_MPI_CFG_NN_DEBUG_FILE_NAME,
                    UVC_MPI_STREAM_SAVE_FILE_LEN);
  mpiCfg->common_cfg.geometric_output_den = 1;
  mpiCfg->common_cfg.geometric_output_num = 1;
  mpiCfg->common_cfg.uvc_enable_vpss = 1;
  // vi_cfg uvc
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].change = MPI_VI_CFG_CHANGE_ALL;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].dev_id = 0;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].channel_id = 1;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].assign_width = 0;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].assign_height = 0;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].min_width = 640;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].min_height = 480;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].max_width = 2560;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].max_height = 1440;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].format = RK_FMT_YUV420SP;
  // 3588 need up to 3 for fps to 30
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].buf_cnt = 3;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].fps_in = -1;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].fps_out = -1;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].fbc = COMPRESS_MODE_NONE;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].memory_type =
      VI_V4L2_MEMORY_TYPE_DMABUF;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].rotation = ROTATION_0;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_UVC].mirror = MIRROR_NONE;
  // vi_cfg nn
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].change = MPI_VI_CFG_CHANGE_ALL;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].dev_id = 0;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].channel_id = 1;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].assign_width = 1280;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].assign_height = 720;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].format = RK_FMT_YUV420SP;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].buf_cnt = 2;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].fps_in = -1;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].fps_out = -1;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].fbc = COMPRESS_MODE_NONE;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].memory_type =
      VI_V4L2_MEMORY_TYPE_DMABUF;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].rotation = ROTATION_0;
  mpiCfg->vi_cfg[MPI_VI_CHANNEL_TYPE_NN].mirror = MIRROR_NONE;
  // vpss_cfg uvc
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].change = MPI_VPSS_CFG_CHANGE_ALL;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].group_id = 0;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].channel_id =
      0; // now only support chn 0
  // now the rockit limit min 3/now the rga limit min 5
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].buf_cnt = 5;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].fps_in = -1;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].fps_out = -1;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].fbc = COMPRESS_MODE_NONE;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].rotation = ROTATION_0;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_UVC].mirror = MIRROR_NONE;
  // vpss_cfg nn
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].change = MPI_VPSS_CFG_CHANGE_ALL;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].group_id = 0;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].channel_id =
      0; // now only support chn 0
  // now the rockit limit min 3/now the rga limit min 5
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].buf_cnt = 5;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].fps_in = -1;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].fps_out = -1;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].fbc = COMPRESS_MODE_NONE;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].rotation = ROTATION_0;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].mirror = MIRROR_NONE;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].assign_width = 1280;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].assign_height = 720;
  mpiCfg->vpss_cfg[MPI_VPSS_CHANNEL_TYPE_NN].format = RK_FMT_RGB888;

  // venc common_cfg
  mpiCfg->venc_cfg.common_cfg.change = MPI_VENC_COMMON_CFG_CHANGE_ALL;
  mpiCfg->venc_cfg.common_cfg.channel_id = 1;
  mpiCfg->venc_cfg.common_cfg.rotation = ROTATION_0;
  mpiCfg->venc_cfg.common_cfg.mirror = MIRROR_NONE;
  mpiCfg->venc_cfg.common_cfg.idr_gop = 5;
  mpiCfg->venc_cfg.common_cfg.idr_cnt = 5;
  mpiCfg->venc_cfg.common_cfg.idr_bps = 20000;
  mpiCfg->venc_cfg.common_cfg.enable_vpss = 1;
  // venc mjpeg_cfg
  mpiCfg->venc_cfg.mjpeg_cfg.change = MPI_VENC_MJPEG_CFG_CHANGE_ALL;
  mpiCfg->venc_cfg.mjpeg_cfg.qp.stParamMjpeg.u32Qfactor = 85;
  mpiCfg->venc_cfg.mjpeg_cfg.qfactor_min = 30;
  mpiCfg->venc_cfg.mjpeg_cfg.qfactor_max = 99;
  mpiCfg->venc_cfg.mjpeg_cfg.range_full = 1;
  mpiCfg->venc_cfg.mjpeg_cfg.bps = 100000;
  mpiCfg->venc_cfg.mjpeg_cfg.sei = 0;
  mpiCfg->venc_cfg.mjpeg_cfg.buf_cnt = 2;
  mpiCfg->venc_cfg.mjpeg_cfg.fps_in = -1;
  mpiCfg->venc_cfg.mjpeg_cfg.fps_out = -1;
  mpiCfg->venc_cfg.mjpeg_cfg.fbc = COMPRESS_MODE_NONE;
  mpiCfg->venc_cfg.mjpeg_cfg.rc_mode = VENC_RC_MODE_MJPEGFIXQP;
  // venc h264_cfg
  mpiCfg->venc_cfg.h264_cfg.change = MPI_VENC_H264_CFG_CHANGE_ALL;
  mpiCfg->venc_cfg.h264_cfg.gop = 60;
  mpiCfg->venc_cfg.h264_cfg.range_full = 1;
  mpiCfg->venc_cfg.h264_cfg.sei = 0;
  mpiCfg->venc_cfg.h264_cfg.profile = 100;
  mpiCfg->venc_cfg.h264_cfg.bps = 10000;
  mpiCfg->venc_cfg.h264_cfg.buf_cnt = 2;
  mpiCfg->venc_cfg.h264_cfg.fps_in = -1;
  mpiCfg->venc_cfg.h264_cfg.fps_out = -1;
  mpiCfg->venc_cfg.h264_cfg.fbc = COMPRESS_MODE_NONE;
  mpiCfg->venc_cfg.h264_cfg.rc_mode = VENC_RC_MODE_H264CBR;
  mpiCfg->venc_cfg.h264_cfg.qp.s32FirstFrameStartQp = 24;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.u32StepQp = 4;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.u32MinIQp = 20;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.u32MaxIQp = 40;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.u32MinQp = 10;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.u32MaxQp = 48;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.s32DeltIpQp = 2;
  mpiCfg->venc_cfg.h264_cfg.qp.stParamH264.s32MaxReEncodeTimes = 1;
  mpiCfg->venc_cfg.h264_cfg.gop_mode.enGopMode = VENC_GOPMODE_INIT;
  mpiCfg->venc_cfg.h264_cfg.gop_mode.s32VirIdrLen = 30;
  // venc h265_cfg
  mpiCfg->venc_cfg.h265_cfg.change = MPI_VENC_H265_CFG_CHANGE_ALL;
  mpiCfg->venc_cfg.h265_cfg.gop = 60;
  mpiCfg->venc_cfg.h265_cfg.range_full = 1;
  mpiCfg->venc_cfg.h265_cfg.sei = 0;
  mpiCfg->venc_cfg.h265_cfg.profile = 100;
  mpiCfg->venc_cfg.h265_cfg.bps = 10000;
  mpiCfg->venc_cfg.h265_cfg.buf_cnt = 2;
  mpiCfg->venc_cfg.h265_cfg.fps_in = -1;
  mpiCfg->venc_cfg.h265_cfg.fps_out = -1;
  mpiCfg->venc_cfg.h265_cfg.fbc = COMPRESS_MODE_NONE;
  mpiCfg->venc_cfg.h265_cfg.rc_mode = VENC_RC_MODE_H265CBR;
  mpiCfg->venc_cfg.h265_cfg.qp.s32FirstFrameStartQp = 24;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.u32StepQp = 4;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.u32MinIQp = 20;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.u32MaxIQp = 40;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.u32MinQp = 10;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.u32MaxQp = 48;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.s32DeltIpQp = 2;
  mpiCfg->venc_cfg.h265_cfg.qp.stParamH265.s32MaxReEncodeTimes = 1;
  mpiCfg->venc_cfg.h265_cfg.gop_mode.enGopMode = VENC_GOPMODE_INIT;
  mpiCfg->venc_cfg.h265_cfg.gop_mode.s32VirIdrLen = 30;
  // eptz cfg
#if UVC_EPTZ
  mpiCfg->eptz_cfg.change = MPI_EPTZ_CFG_CHANGE_ALL;
  mpiCfg->eptz_cfg.debug = 0;
  mpiCfg->eptz_cfg.enable = 0;
  mpiCfg->eptz_cfg.enable_boot = 0;
#endif
}

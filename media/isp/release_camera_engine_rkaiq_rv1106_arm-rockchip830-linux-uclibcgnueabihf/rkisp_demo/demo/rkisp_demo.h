/*
 *  Copyright (c) 2019 Rockchip Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef _RKISP_DEMO_H_
#define _RKISP_DEMO_H_

#include <linux/videodev2.h>
#include "uAPI2/rk_aiq_user_api2_imgproc.h"
#include "uAPI2/rk_aiq_user_api2_camgroup.h"

#define DBG(...) do { if(!silent) printf(__VA_ARGS__); } while(0)
#define ERR(...) do { printf(__VA_ARGS__); } while (0)

typedef struct _demo_context {
    char out_file[255];
    char dev_name[255];
    char dev_name2[255];
    char sns_name[32];
    int dev_using;
    int width;
    int height;
    int format;
    int fd;
    enum v4l2_buf_type buf_type;
    struct buffer *buffers;
    unsigned int n_buffers;
    int frame_count;
    FILE *fp;
    rk_aiq_sys_ctx_t* aiq_ctx;
    rk_aiq_camgroup_ctx_t* camgroup_ctx;
    int vop;
    int rkaiq;
    int writeFile;
    int writeFileSync;
    int pponeframe;
    int hdrmode;
    int limit_range;
    int fd_pp_input;
    int fd_isp_mp;
    struct buffer *buffers_mp;
    int outputCnt;
    int skipCnt;

    char yuv_dir_path[64];
    bool _is_yuv_dir_exist;
    int capture_yuv_num;
    bool is_capture_yuv;
    int ctl_type;
    char iqpath[256];
     char orppath[256];
     int orpRawW;
     int orpRawH;
     char orpRawFmt[10];
     bool isOrp;
     bool orpStop;
     bool orpStopped;
     bool camGroup;
} demo_context_t;

#endif

/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 * Authors:
 *  PutinLee <putin.lee@rock-chips.com>
 *  Cerf Yu <cerf.yu@rock-chips.com>
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
 */

#ifndef ARGS_H
#define ARGS_H

#include "im2d.hpp"

typedef enum _mode_code {
    MODE_QUERYSTRING = 0,
    MODE_COPY,
    MODE_RESIZE,
    MODE_CROP,
    MODE_ROTATE,
    MODE_FLIP,
    MODE_TRANSLATE,
    MODE_BLEND,
    MODE_CVTCOLOR,
    MODE_FILL,
    MODE_WHILE,
    MODE_HELP,
    MODE_NONE,
    MODE_MAX
} MODE_CODE;

#define MODE_QUERYSTRING_CHAR     (char) (MODE_QUERYSTRING+'0')
#define MODE_COPY_CHAR            (char) (MODE_COPY       +'0')
#define MODE_RESIZE_CHAR          (char) (MODE_RESIZE     +'0')
#define MODE_CROP_CHAR            (char) (MODE_CROP       +'0')
#define MODE_ROTATE_CHAR          (char) (MODE_ROTATE     +'0')
#define MODE_FLIP_CHAR            (char) (MODE_FLIP       +'0')
#define MODE_TRANSLATE_CHAR       (char) (MODE_TRANSLATE  +'0')
#define MODE_BLEND_CHAR           (char) (MODE_BLEND      +'0')
#define MODE_CVTCOLOR_CHAR        (char) (MODE_CVTCOLOR   +'0')
#define MODE_FILL_CHAR            (char) (MODE_FILL       +'0')
#define MODE_WHILE_CHAR           'w'
#define MODE_HELP_CHAR            'h'
#define MODE_NONE_CHAR            (char) (MODE_NONE       +'0')

#define BLUE_COLOR  0xffff0000
#define GREEN_COLOR 0xff00ff00
#define RED_COLOR   0xff0000ff
#define WHILE_FLAG  (1 << 7)

int readArguments(int argc, char *argv[], int* parm);
IM_INFORMATION readInfo(char* targ);
int readParm(char* targ);

#endif


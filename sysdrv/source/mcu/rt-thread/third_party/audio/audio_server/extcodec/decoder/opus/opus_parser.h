/*
 * Copyright (c) 2020 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-04-20     Jair Wu      First version
 *
 */

#ifndef OPUS_PARSER_H
#define OPUS_PARSER_H

#include "extcodec.h"

#ifdef RT_USING_EXT_OPUS_DECODER
#include "ogg.h"

uint32_t opus_get_duration(char *filename);

#endif  // RT_USING_EXT_OPUS_DECODER
#endif  // OGG_PARSER_H

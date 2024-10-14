#ifndef __PATTERN_H__
#define __PATTERN_H__

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "drv_display.h"

struct util_color_component
{
    unsigned int length;
    unsigned int offset;
};

struct util_rgb_info
{
    struct util_color_component red;
    struct util_color_component green;
    struct util_color_component blue;
    struct util_color_component alpha;
};

enum util_yuv_order
{
    YUV_YCbCr = 1,
    YUV_YCrCb = 2,
    YUV_YC = 4,
    YUV_CY = 8,
};

struct util_yuv_info
{
    enum util_yuv_order order;
    unsigned int xsub;
    unsigned int ysub;
    unsigned int chroma_stride;
};

struct util_format_info
{
    uint32_t format;
    const char *name;
    const struct util_rgb_info rgb;
    const struct util_yuv_info yuv;
};

uint32_t util_format_fourcc(const char *name);
const struct util_format_info *util_format_info_find(uint32_t format);

enum util_fill_pattern
{
    UTIL_PATTERN_TILES,
    UTIL_PATTERN_PLAIN,
    UTIL_PATTERN_SMPTE,
};

void util_fill_pattern(uint32_t format,
                       void *planes[3], unsigned int width,
                       unsigned int height, unsigned int stride);

#endif

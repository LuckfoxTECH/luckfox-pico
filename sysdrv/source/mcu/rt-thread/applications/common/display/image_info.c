/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <rtdevice.h>

#include <stdio.h>
#include <math.h>
#include "image_info.h"
#include "jbig2dec.h"

/*
 **************************************************************************************************
 *
 * Image decompress.
 *
 **************************************************************************************************
 */
#define NULL_CODE           -1      // indicates a NULL prefix
#define CLEAR_CODE          256     // code to flush dictionary and restart decoder
#define FIRST_STRING        257     // code of first dictionary string

static size_t read_count, write_count;

/**
 * decompress read buffer.
 */
static int rt_decompress_read_buff(image_info_t *img_info)
{
    int value = img_info->data[read_count++];
    if (read_count == img_info->size)
    {
        value = EOF;
    }
    return value;
}

/**
 * decompress write buffer.
 */
static void rt_decompress_write_buff(int value, rt_uint8_t *fb, rt_int32_t fbsize, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset, int width)
{
    if (write_count < fbsize)
    {
        xoffset = xoffset + write_count % width;
        yoffset = yoffset + write_count / width;
        *((uint8_t *)((uint32_t)fb + (yoffset)* xVir + xoffset)) = value;
    }

    if (value == EOF)
    {
        return;
    }

    write_count++;
}

/**
 * decompress sub function.
 */
static int rt_display_decompress(image_info_t *img_info, rt_uint8_t *fb, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset)
{
    int read_byte, next = FIRST_STRING, prefix = CLEAR_CODE, bits = 0, total_codes;
    unsigned char *terminators, *reverse_buffer;
    unsigned long shifter = 0;
    short *prefixes;

    write_count = 0;
    read_count = 0;

    if ((read_byte = (rt_decompress_read_buff(img_info))) == EOF || (read_byte & 0xfc))  //sanitize first byte
    {
        //rt_kprintf("rt_display_decompress 111\n");
        return RT_ERROR;
    }

    // based on the "maxbits" parameter, compute total codes and allocate dictionary storage
    total_codes = 512 << (read_byte & 0x3);
    reverse_buffer = malloc((total_codes - 256) * sizeof(reverse_buffer[0]));
    prefixes = malloc((total_codes - 256) * sizeof(prefixes[0]));
    terminators = malloc((total_codes - 256) * sizeof(terminators[0]));

    if (!reverse_buffer || !prefixes || !terminators)       // check for mallco() failure
    {
        if (reverse_buffer)
            free(reverse_buffer);
        if (prefixes)
            free(prefixes);
        if (terminators)
            free(terminators);

        //rt_kprintf("rt_display_decompress 222\n");
        return RT_ERROR;
    }

    // This is the main loop where we read input symbols. The values range from 0 to the code value
    // of the "next" string in the dictionary (although the actual "next" code cannot be used yet,
    // and so we reserve that code for the END_CODE). Note that receiving an EOF from the input
    // stream is actually an error because we should have gotten the END_CODE first.
    while (1)
    {
        int code_bits = next < 1024 ? (next < 512 ? 8 : 9) : (next < 2048 ? 10 : 11), code;
        int extras = (1 << (code_bits + 1)) - next - 1;

        do
        {
            if ((read_byte = (rt_decompress_read_buff(img_info))) == EOF)
            {
                free(terminators);
                free(prefixes);
                free(reverse_buffer);

                //rt_kprintf("rt_display_decompress 333\n");
                //return RT_ERROR;
                return RT_EOK;
            }

            shifter |= (long)read_byte << bits;
        }
        while ((bits += 8) < code_bits);

        // first we assume the code will fit in the minimum number of required bits
        code = (int)shifter & ((1 << code_bits) - 1);
        shifter >>= code_bits;
        bits -= code_bits;

        // but if code >= extras, then we need to read another bit to calculate the real code
        // (this is the "adjusted binary" part)
        if (code >= extras)
        {
            if (!bits)
            {
                if ((read_byte = (rt_decompress_read_buff(img_info))) == EOF)
                {
                    free(terminators);
                    free(prefixes);
                    free(reverse_buffer);

                    //rt_kprintf("rt_display_decompress 444\n");
                    //return RT_ERROR;
                    return RT_EOK;
                }

                shifter = (long)read_byte;
                bits = 8;
            }

            code = (code << 1) - extras + (shifter & 1);
            shifter >>= 1;
            bits--;
        }

        if (code == next)                   // sending the maximum code is reserved for the end of the file
            break;
        else if (code == CLEAR_CODE)        // otherwise check for a CLEAR_CODE to start over early
            next = FIRST_STRING;
        else if (prefix == CLEAR_CODE)      // this only happens at the first symbol which is always sent
        {
            rt_decompress_write_buff(code, fb, img_info->w * img_info->h, xVir, xoffset, yoffset, img_info->w);                   // literally and becomes our initial prefix
            next++;
        }
        // Otherwise we have a valid prefix so we step through the string from end to beginning storing the
        // bytes in the "reverse_buffer", and then we send them out in the proper order. One corner-case
        // we have to handle here is that the string might be the same one that is actually being defined
        // now (code == next-1). Also, the first 256 entries of "terminators" and "prefixes" are fixed and
        // not allocated, so that messes things up a bit.
        else
        {
            int cti = (code == next - 1) ? prefix : code;
            unsigned char *rbp = reverse_buffer, c;

            do *rbp++ = cti < 256 ? cti : terminators[cti - 256];      // step backward through string...
            while ((cti = (cti < 256) ? NULL_CODE : prefixes[cti - 256]) != NULL_CODE);

            c = *--rbp;     // the first byte in this string is the terminator for the last string, which is
            // the one that we'll create a new dictionary entry for this time
            do rt_decompress_write_buff(*rbp, fb,  img_info->w * img_info->h, xVir, xoffset, yoffset, img_info->w);                        // send string in corrected order (except for the terminator
            while (rbp-- != reverse_buffer);          // which we don't know yet)

            if (code == next - 1)
                rt_decompress_write_buff(c, fb,  img_info->w * img_info->h, xVir, xoffset, yoffset, img_info->w);

            prefixes[next - 1 - 256] = prefix;     // now update the next dictionary entry with the new string
            terminators[next - 1 - 256] = c;       // (but we're always one behind, so it's not the string just sent)

            if (++next == total_codes)              // check for full dictionary, which forces a reset (and, BTW,
                next = FIRST_STRING;                // means we'll never use the dictionary entry we just wrote)
        }

        prefix = code;      // the code we just received becomes the prefix for the next dictionary string entry
        // (which we'll create once we find out the terminator)
    }

    free(terminators);
    free(prefixes);
    free(reverse_buffer);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * Image rotate.
 *
 **************************************************************************************************
 */
#define PI 3.1415926535898

/**
 * float angle : rotate angle
 * int w
 * int h :
 * unsigned char *src :
 * unsigned char *dst :
 * int dst_str :
*/
static unsigned char rt_display_SegGradSelect_8bit(int x, int y, int w, int h, int x_res, int y_res, unsigned char *data, unsigned char *src)
{
    unsigned char alpha = 0xff;
    int pos = 0;

    if (x == -1 && y == -1)
    {
        alpha = (x_res * y_res) >> 8;
        pos = (y + 1) * w + (x + 1);
    }
    else if (x == w && y == -1)
    {
        alpha = ((255 - x_res) * y_res) >> 8;
        pos = (y + 1) * w + (x - 1);
    }
    else if (x == -1 && y == h)
    {
        alpha = ((x_res) * (255 - y_res)) >> 8;
        pos = (y - 1) * w + (x + 1);
    }
    else if (x == w && y == h)
    {
        alpha = ((255 - x_res) * (255 - y_res)) >> 8;
        *data = src[(y - 1) * w + (x - 1)];
    }
    else if (x == -1 && y > -1 && y < h)
    {
        alpha = x_res;
        pos = y * w + (x + 1);
    }
    else if (y == -1 && x > -1 && x < w)
    {
        alpha = y_res;
        pos = (y + 1) * w + x;
    }
    else if (x == w && y > -1 && y < h)
    {
        alpha = 255 - x_res;
        pos = y * w + x - 1;
    }
    else if (y == h && x > -1 && x < w)
    {
        alpha = 255 - y_res;
        pos = (y - 1) * w + x;
    }

    *data = src[pos];

    return alpha;
}

static unsigned char rt_display_SegGradSelect_16bit(int x, int y, int w, int h, int x_res, int y_res, unsigned short *data, unsigned short *src)
{
    unsigned char alpha = 0xff;
    int pos = 0;
    if (x == -1 && y == -1)
    {
        alpha = (x_res * y_res) >> 8;
        pos = (y + 1) * w + (x + 1);
    }
    else if (x == w && y == -1)
    {
        alpha = ((255 - x_res) * y_res) >> 8;
        pos = (y + 1) * w + (x - 1);
    }
    else if (x == -1 && y == h)
    {
        alpha = ((x_res) * (255 - y_res)) >> 8;
        pos = (y - 1) * w + (x + 1);
    }
    else if (x == w && y == h)
    {
        alpha = ((255 - x_res) * (255 - y_res)) >> 8;
        *data = src[(y - 1) * w + (x - 1)];
    }
    else if (x == -1 && y > -1 && y < h)
    {
        alpha = x_res;
        pos = y * w + (x + 1);
    }
    else if (y == -1 && x > -1 && x < w)
    {
        alpha = y_res;
        pos = (y + 1) * w + x;
    }
    else if (x == w && y > -1 && y < h)
    {
        alpha = 255 - x_res;
        pos = y * w + x - 1;
    }
    else if (y == h && x > -1 && x < w)
    {
        alpha = 255 - y_res;
        pos = (y - 1) * w + x;
    }

    *data = src[pos];

    return alpha;
}

static unsigned char rt_display_SegGradSelect_32bit(int x, int y, int w, int h, int x_res, int y_res, unsigned int *data, unsigned int *src)
{
    unsigned char alpha = 0xff;
    int pos = 0;
    if (x == -1 && y == -1)
    {
        alpha = (x_res * y_res) >> 8;
        pos = (y + 1) * w + (x + 1);
    }
    else if (x == w && y == -1)
    {
        alpha = ((255 - x_res) * y_res) >> 8;
        pos = (y + 1) * w + (x - 1);
    }
    else if (x == -1 && y == h)
    {
        alpha = ((x_res) * (255 - y_res)) >> 8;
        pos = (y - 1) * w + (x + 1);
    }
    else if (x == w && y == h)
    {
        alpha = ((255 - x_res) * (255 - y_res)) >> 8;
        *data = src[(y - 1) * w + (x - 1)];
    }
    else if (x == -1 && y > -1 && y < h)
    {
        alpha = x_res;
        pos = y * w + (x + 1);
    }
    else if (y == -1 && x > -1 && x < w)
    {
        alpha = y_res;
        pos = (y + 1) * w + x;
    }
    else if (x == w && y > -1 && y < h)
    {
        alpha = 255 - x_res;
        pos = y * w + x - 1;
    }
    else if (y == h && x > -1 && x < w)
    {
        alpha = 255 - y_res;
        pos = (y - 1) * w + x;
    }

    *data = src[pos];

    return alpha;
}

static unsigned char rt_display_SegGradSelect_4bit(int x, int y, int w, int h, int x_res, int y_res, unsigned char *data, unsigned char *src)
{
    unsigned char alpha = 0xff;
    int pos = 0;
    if (x == -1 && y == -1)
    {
        alpha = (x_res * y_res) >> 8;
        pos = (y + 1) * w + (x + 1);
    }
    else if (x == w && y == -1)
    {
        alpha = ((255 - x_res) * y_res) >> 8;
        pos = (y + 1) * w + (x - 1);
    }
    else if (x == -1 && y == h)
    {
        alpha = ((x_res) * (255 - y_res)) >> 8;
        pos = (y - 1) * w + (x + 1);
    }
    else if (x == w && y == h)
    {
        alpha = ((255 - x_res) * (255 - y_res)) >> 8;
        *data = src[(y - 1) * w + (x - 1)];
    }
    else if (x == -1 && y > -1 && y < h)
    {
        alpha = x_res;
        pos = y * w + (x + 1);
    }
    else if (y == -1 && x > -1 && x < w)
    {
        alpha = y_res;
        pos = (y + 1) * w + x;
    }
    else if (x == w && y > -1 && y < h)
    {
        alpha = 255 - x_res;
        pos = y * w + x - 1;
    }
    else if (y == h && x > -1 && x < w)
    {
        alpha = 255 - y_res;
        pos = (y - 1) * w + x;
    }

    if ((pos & 1) == 0)
        *data = src[pos];

    return alpha;
}

/**
 * display rotate.
 */
void rt_display_rotate_32bit(float angle, int w, int h, unsigned int *src, unsigned int *dst, int dst_str, int xcen, int ycen)
{
    int x, y;

    int xlt, xrt, xld, xrd;
    int ylt, yrt, yld, yrd;
    int xmin, xmax, ymin, ymax;

    //rt_kprintf("angle = %d, w = %d, h = %d, src = 0x%08x, dst = 0x%08x, dst_str = %d, xcen = %d, ycen = %d\n",
    //            (int)angle, w, h, src, dst, dst_str, xcen, ycen);

    float cosa = cos((angle * PI) / 180);
    float sina = sin((angle * PI) / 180);

    xlt = cosa * (-xcen) + -sina * (-ycen);
    ylt = sina * (-xcen) +  cosa * (-ycen);
    xrt = cosa * (w - xcen) + -sina * (-ycen);
    yrt = sina * (w - xcen) +  cosa * (-ycen);
    xld = cosa * (-xcen) + -sina * (h - ycen);
    yld = sina * (-xcen) +  cosa * (h - ycen);
    xrd = cosa * (w - xcen) + -sina * (h - ycen);
    yrd = sina * (w - xcen) +  cosa * (h - ycen);

    xmin = MIN(xrd, MIN(xld, MIN(xlt, xrt))) - 1;
    xmax = MAX(xrd, MAX(xld, MAX(xlt, xrt))) + 1;
    ymin = MIN(yrd, MIN(yld, MIN(ylt, yrt))) - 1;
    ymax = MAX(yrd, MAX(yld, MAX(ylt, yrt))) + 1;

    float x_pos, y_pos;
    unsigned char alpha;

    int m = ymin;
    for (int j = ymin; j < ymax; j++)
    {
        int n = xmin;
        for (int i = xmin; i < xmax; i++)
        {
            x_pos = (cosa * i + sina * j) + xcen;
            y_pos = (-sina * i + cosa * j) + ycen;
            x = floor(x_pos);
            y = floor(y_pos);

            if ((x_pos >= 0) && (x_pos < w) && (y_pos >= 0) && (y_pos < h))
            {
                dst[m * dst_str + n] = src[y * w + x];
            }
            else if ((x >= -1) && (y >= -1) && (x <= w) && (y <= h))
            {
                int x_res = (x_pos - x) * (1 << 8);
                int y_res = (y_pos - y) * (1 << 8);
                unsigned int data;

                alpha = rt_display_SegGradSelect_32bit(x, y, w, h, x_res, y_res, &data, src);

                unsigned char /*sa,*/ sr, sg, sb, da, dr, dg, db;

                sr = (dst[m * dst_str + n]) & 0xff;
                sg = (dst[m * dst_str + n] >> 8) & 0xff;
                sb = (dst[m * dst_str + n] >> 16) & 0xff;
                //sa = (dst[m*dst_str + n] >> 24) & 0xff;

                dr = (data) & 0xff;
                dg = (data >> 8) & 0xff;
                db = (data >> 16) & 0xff;
                da = (data >> 24) & 0xff;
                alpha = (alpha * da) >> 8;

                dr = (sr * (255 - alpha) + dr * alpha) >> 8;
                dg = (sg * (255 - alpha) + dg * alpha) >> 8;
                db = (sb * (255 - alpha) + db * alpha) >> 8;

                dst[m * dst_str + n] = dr | (dg << 8) | (db << 16) | (da << 24);
            }
            n++;
        }
        m++;
    }
}

void rt_display_rotate_24bit(float angle, int w, int h, unsigned char *src, unsigned char *dst, int dst_str, int xcen, int ycen)
{
    int x, y;

    int xlt, xrt, xld, xrd;
    int ylt, yrt, yld, yrd;
    int xmin, xmax, ymin, ymax;

    float cosa = cos((angle * PI) / 180);
    float sina = sin((angle * PI) / 180);

    xlt = cosa * (-xcen) + -sina * (-ycen);
    ylt = sina * (-xcen) +  cosa * (-ycen);
    xrt = cosa * (w - xcen) + -sina * (-ycen);
    yrt = sina * (w - xcen) +  cosa * (-ycen);
    xld = cosa * (-xcen) + -sina * (h - ycen);
    yld = sina * (-xcen) +  cosa * (h - ycen);
    xrd = cosa * (w - xcen) + -sina * (h - ycen);
    yrd = sina * (w - xcen) +  cosa * (h - ycen);

    xmin = MIN(xrd, MIN(xld, MIN(xlt, xrt))) - 1;
    xmax = MAX(xrd, MAX(xld, MAX(xlt, xrt))) + 1;
    ymin = MIN(yrd, MIN(yld, MIN(ylt, yrt))) - 1;
    ymax = MAX(yrd, MAX(yld, MAX(ylt, yrt))) + 1;

    float x_pos, y_pos;
    unsigned char alpha;
    //int cur_x_pos;
    //int cur_y_pos;

    int m = ymin;
    for (int j = ymin; j < ymax; j++)
    {
        int n = xmin;
        for (int i = xmin; i < xmax; i++)
        {
            x_pos = (cosa * i + sina * j) + xcen;
            y_pos = (-sina * i + cosa * j) + ycen;
            x = floor(x_pos);
            y = floor(y_pos);

            if (x_pos >= 0 && x_pos < w && y_pos >= 0 && y_pos < h)
            {
                dst[m * dst_str + n] = src[y * w + x];
            }
            else if ((x >= -1) && (y >= -1) && (x <= w) && (y <= h))
            {
                int x_res = (x_pos - x) * (1 << 8);
                int y_res = (y_pos - y) * (1 << 8);
                unsigned char data;

                alpha = rt_display_SegGradSelect_8bit(x, y, w, h, x_res, y_res, &data, src);

                dst[m * dst_str + n] = (dst[m * dst_str + n] * (255 - alpha) + data * alpha) >> 8;
            }
            n++;
        }
        m++;
    }
}

void rt_display_rotate_8bit(float angle, int w, int h, unsigned char *src, unsigned char *dst, int dst_str, int xcen, int ycen)
{
    int x, y;

    int xlt, xrt, xld, xrd;
    int ylt, yrt, yld, yrd;
    int xmin, xmax, ymin, ymax;

    float cosa = cos((angle * PI) / 180);
    float sina = sin((angle * PI) / 180);

    xlt = cosa * (-xcen) + -sina * (-ycen);
    ylt = sina * (-xcen) +  cosa * (-ycen);
    xrt = cosa * (w - xcen) + -sina * (-ycen);
    yrt = sina * (w - xcen) +  cosa * (-ycen);
    xld = cosa * (-xcen) + -sina * (h - ycen);
    yld = sina * (-xcen) +  cosa * (h - ycen);
    xrd = cosa * (w - xcen) + -sina * (h - ycen);
    yrd = sina * (w - xcen) +  cosa * (h - ycen);

    xmin = MIN(xrd, MIN(xld, MIN(xlt, xrt))) - 1;
    xmax = MAX(xrd, MAX(xld, MAX(xlt, xrt))) + 1;
    ymin = MIN(yrd, MIN(yld, MIN(ylt, yrt))) - 1;
    ymax = MAX(yrd, MAX(yld, MAX(ylt, yrt))) + 1;

    float x_pos, y_pos;
    unsigned char alpha;

    int m = ymin;
    for (int j = ymin; j < ymax; j++)
    {
        int n = xmin;
        for (int i = xmin; i < xmax; i++)
        {
            x_pos = (cosa * i + sina * j) + xcen;
            y_pos = (-sina * i + cosa * j) + ycen;
            x = floor(x_pos);
            y = floor(y_pos);

            if (x_pos >= 0 && x_pos < w && y_pos >= 0 && y_pos < h)
            {
                dst[m * dst_str + n] = src[y * w + x];
            }
            else if ((x >= -1) && (y >= -1) && (x <= w) && (y <= h))
            {
                int x_res = (x_pos - x) * (1 << 8);
                int y_res = (y_pos - y) * (1 << 8);
                unsigned char data = 0;

                alpha = rt_display_SegGradSelect_8bit(x, y, w, h, x_res, y_res, &data, src);

                unsigned char sr, sg, sb, dr, dg, db;
                sr = (dst[m * dst_str + n]) & 0x7;
                sg = (dst[m * dst_str + n] >> 3) & 0x7;
                sb = (dst[m * dst_str + n] >> 6) & 0x3;

                dr = (data) & 0x7;
                dg = (data >> 3) & 0x7;
                db = (data >> 6) & 0x3;

                dr = (sr * (255 - alpha) + dr * alpha) >> 8;
                dg = (sg * (255 - alpha) + dg * alpha) >> 8;
                db = (sb * (255 - alpha) + db * alpha) >> 8;

                dst[m * dst_str + n] = dr | (dg << 3) | (db << 6);
            }
            n++;
        }
        m++;
    }
}

void rt_display_rotate_16bit(float angle, int w, int h, unsigned short *src, unsigned short *dst, int dst_str, int xcen, int ycen)
{
    int x, y;

    int xlt, xrt, xld, xrd;
    int ylt, yrt, yld, yrd;
    int xmin, xmax, ymin, ymax;

    float cosa = cos((angle * PI) / 180);
    float sina = sin((angle * PI) / 180);

    xlt = cosa * (-xcen) + -sina * (-ycen);
    ylt = sina * (-xcen) +  cosa * (-ycen);
    xrt = cosa * (w - xcen) + -sina * (-ycen);
    yrt = sina * (w - xcen) +  cosa * (-ycen);
    xld = cosa * (-xcen) + -sina * (h - ycen);
    yld = sina * (-xcen) +  cosa * (h - ycen);
    xrd = cosa * (w - xcen) + -sina * (h - ycen);
    yrd = sina * (w - xcen) +  cosa * (h - ycen);

    xmin = MIN(xrd, MIN(xld, MIN(xlt, xrt))) - 1;
    xmax = MAX(xrd, MAX(xld, MAX(xlt, xrt))) + 1;
    ymin = MIN(yrd, MIN(yld, MIN(ylt, yrt))) - 1;
    ymax = MAX(yrd, MAX(yld, MAX(ylt, yrt))) + 1;

    float x_pos, y_pos;
    unsigned char alpha;

    int m = ymin;
    for (int j = ymin; j < ymax; j++)
    {
        int n = xmin;
        for (int i = xmin; i < xmax; i++)
        {
            x_pos = (cosa * i + sina * j) + xcen;
            y_pos = (-sina * i + cosa * j) + ycen;
            x = floor(x_pos);
            y = floor(y_pos);

            if (x_pos >= 0 && x_pos < w && y_pos >= 0 && y_pos < h)
            {
                dst[m * dst_str + n] = src[y * w + x];
            }
            else if ((x >= -1) && (y >= -1) && (x <= w) && (y <= h))
            {
                int x_res = (x_pos - x) * (1 << 8);
                int y_res = (y_pos - y) * (1 << 8);
                unsigned short data = 0;

                alpha = rt_display_SegGradSelect_16bit(x, y, w, h, x_res, y_res, &data, src);

                unsigned char sr, sg, sb, dr, dg, db;
                sr = (dst[m * dst_str + n]) & 0x1f;
                sg = (dst[m * dst_str + n] >> 5) & 0x3f;
                sb = (dst[m * dst_str + n] >> 11) & 0x1f;

                dr = (data) & 0x1f;
                dg = (data >> 5) & 0x3f;
                db = (data >> 11) & 0x1f;

                dr = (sr * (255 - alpha) + dr * alpha) >> 8;
                dg = (sg * (255 - alpha) + dg * alpha) >> 8;
                db = (sb * (255 - alpha) + db * alpha) >> 8;

                dst[m * dst_str + n] = dr | (dg << 5) | (db << 11);
            }
            n++;
        }
        m++;
    }
}

void rt_display_rotate_4bit(float angle, int w, int h, unsigned char *src, unsigned char *dst, int dst_str, int xcen, int ycen)
{
    int x, y;

    int xlt, xrt, xld, xrd;
    int ylt, yrt, yld, yrd;
    int xmin, xmax, ymin, ymax;

    float cosa = cos((angle * PI) / 180);
    float sina = sin((angle * PI) / 180);

    xlt = cosa * (-xcen) + -sina * (-ycen);
    ylt = sina * (-xcen) +  cosa * (-ycen);
    xrt = cosa * (w - xcen) + -sina * (-ycen);
    yrt = sina * (w - xcen) +  cosa * (-ycen);
    xld = cosa * (-xcen) + -sina * (h - ycen);
    yld = sina * (-xcen) +  cosa * (h - ycen);
    xrd = cosa * (w - xcen) + -sina * (h - ycen);
    yrd = sina * (w - xcen) +  cosa * (h - ycen);

    xmin = MIN(xrd, MIN(xld, MIN(xlt, xrt))) - 1;
    xmax = MAX(xrd, MAX(xld, MAX(xlt, xrt))) + 1;
    ymin = MIN(yrd, MIN(yld, MIN(ylt, yrt))) - 1;
    ymax = MAX(yrd, MAX(yld, MAX(ylt, yrt))) + 1;

    float x_pos, y_pos;
    unsigned char alpha;

    int m = ymin;
    for (int j = ymin; j < ymax; j++)
    {
        int n = xmin;
        for (int i = xmin; i < xmax; i++)
        {
            x_pos = (cosa * i + sina * j) + xcen;
            y_pos = (-sina * i + cosa * j) + ycen;
            x = floor(x_pos);
            y = floor(y_pos);

            int dst_pos = ((m * dst_str) + n) >> 1;
            int src_pos = ((y * w >> 1) + x) >> 1;

            if (x_pos >= 0 && x_pos < w && y_pos >= 0 && y_pos < h)
            {
                if ((x & 1) == 0)
                {
                    dst[dst_pos] = (dst[dst_pos] & 0xf0) | ((src[src_pos] & 0xf));
                }
                else
                {
                    dst[dst_pos] = (dst[dst_pos] & 0x0f) | ((src[src_pos] & 0xf) << 4);
                }
            }
            else if ((x >= -1) && (y >= -1) && (x <= w) && (y <= h))
            {
                int x_res = (x_pos - x) * (1 << 8);
                int y_res = (y_pos - y) * (1 << 8);
                unsigned char data = 0;

                alpha = rt_display_SegGradSelect_4bit(x, y, w, h, x_res, y_res, &data, src);

                unsigned char sr, sg, sb, dr, dg, db;
                sr = (dst[m * dst_str + n]) & 0x7;
                sg = (dst[m * dst_str + n] >> 3) & 0x7;
                sb = (dst[m * dst_str + n] >> 6) & 0x3;

                dr = (data) & 0x7;
                dg = (data >> 3) & 0x7;
                db = (data >> 6) & 0x3;

                dr = (sr * (255 - alpha) + dr * alpha) >> 8;
                dg = (sg * (255 - alpha) + dg * alpha) >> 8;
                db = (sb * (255 - alpha) + db * alpha) >> 8;

                dst[m * dst_str + n] = dr | (dg << 3) | (db << 6);
            }
            n++;
        }
        m++;
    }
}

/*
 **************************************************************************************************
 *
 * Image fill.
 *
 **************************************************************************************************
 */
/**
 * fill image data to fb buffer
 */
void rt_display_img_fill(image_info_t *img_info, rt_uint8_t *fb, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset)
{
    rt_err_t ret = RT_EOK;
    rt_int32_t x, y, i;
    rt_uint8_t bitval;

    if (img_info->type == IMG_TYPE_COMPRESS)
    {
        if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_RGB332)
        {
            ret = rt_display_decompress(img_info, fb, xVir, xoffset, yoffset);
            RT_ASSERT(ret == RT_EOK);
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_RGB565)
        {
            image_info_t info;
            rt_memcpy(&info, img_info, sizeof(image_info_t));
            info.x  *= 2;
            info.w  *= 2;
            xVir    *= 2;
            xoffset *= 2;
            ret = rt_display_decompress(&info, fb, xVir, xoffset, yoffset);
            RT_ASSERT(ret == RT_EOK);
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_ARGB888)
        {
            image_info_t info;
            rt_memcpy(&info, img_info, sizeof(image_info_t));
            info.x  *= 4;
            info.w  *= 4;
            xVir    *= 4;
            xoffset *= 4;
            ret = rt_display_decompress(&info, fb, xVir, xoffset, yoffset);
            RT_ASSERT(ret == RT_EOK);
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_GRAY16)
        {
            image_info_t info;
            rt_memcpy(&info, img_info, sizeof(image_info_t));
            info.x  /= 2;
            info.w  /= 2;
            xVir    /= 2;
            xoffset /= 2;
            ret = rt_display_decompress(&info, fb, xVir, xoffset, yoffset);
            RT_ASSERT(ret == RT_EOK);
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_GRAY4)
        {
            image_info_t info;
            rt_memcpy(&info, img_info, sizeof(image_info_t));
            info.x  /= 4;
            info.w  /= 4;
            xVir    /= 4;
            xoffset /= 4;
            ret = rt_display_decompress(&info, fb, xVir, xoffset, yoffset);
            RT_ASSERT(ret == RT_EOK);
        }
        else //if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_GRAY1)
        {
            ret = jbig2_decompression(img_info, fb, xVir, xoffset, yoffset);
            RT_ASSERT(ret == 0);
        }
    }
    else //if (img_info->type == IMG_TYPE_RAW)
    {
        if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_RGB332)
        {
            for (i = 0, y = yoffset; y < yoffset + img_info->h; y++)
            {
                for (x = xoffset; x < xoffset + img_info->w; x++)
                {
                    fb[(y * xVir) + x] = img_info->data[i++];
                }
            }
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_RGB565)
        {
            for (i = 0, y = yoffset; y < yoffset + img_info->h; y++)
            {
                for (x = xoffset * 2; x < xoffset * 2 + img_info->w * 2; x++)
                {
                    fb[(y * xVir * 2) + x] = img_info->data[i++];
                }
            }
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_RGB888)
        {
            for (i = 0, y = yoffset; y < yoffset + img_info->h; y++)
            {
                for (x = xoffset * 3; x < xoffset * 3 + img_info->w * 3; x++)
                {
                    fb[(y * xVir * 3) + x] = img_info->data[i++];
                }
            }
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_ARGB888)
        {
            for (i = 0, y = yoffset; y < yoffset + img_info->h; y++)
            {
                for (x = xoffset * 4; x < xoffset * 4 + img_info->w * 4; x++)
                {
                    fb[(y * xVir * 4) + x] = img_info->data[i++];
                }
            }
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_GRAY1)
        {
            RT_ASSERT((xVir % 8) == 0);

            rt_uint8_t colorkey = (rt_uint8_t)(img_info->colorkey & 0xff);

            if (((xoffset % 8) == 0) && ((img_info->colorkey & COLOR_KEY_EN) == 0))
            {
                for (y = yoffset; y < yoffset + img_info->h; y++)
                {
                    i = (y - yoffset) * ((img_info->w + 7) / 8);
                    for (x = xoffset / 8; x < (xoffset + img_info->w) / 8; x++)
                    {
                        fb[y * (xVir / 8) + x] = img_info->data[i++];
                    }

                    if (((xoffset + img_info->w) % 8) != 0)
                    {
                        rt_uint8_t maskval = 0xff >> (img_info->w % 8);
                        fb[y * (xVir / 8) + x] &= maskval;
                        fb[y * (xVir / 8) + x] |= (img_info->data[i++] & (~maskval));
                    }
                }
            }
            else
            {
                for (y = yoffset; y < yoffset + img_info->h; y++)
                {
                    i = (y - yoffset) * ((img_info->w + 7) / 8) * 8;
                    for (x = xoffset; x < xoffset + img_info->w; x++)
                    {
                        bitval = (img_info->data[i / 8] << (i % 8)) & 0x80;

                        i++;
                        if (img_info->colorkey & COLOR_KEY_EN)
                        {
                            if (((colorkey != 0) && (bitval != 0)) ||
                                    ((colorkey == 0) && (bitval == 0)))
                            {
                                continue;
                            }
                        }

                        fb[y * (xVir / 8) + x / 8] &= ~(0x80 >> (x % 8));
                        fb[y * (xVir / 8) + x / 8] |= bitval >> (x % 8);
                    }
                }
            }
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_GRAY4)
        {
            RT_ASSERT((xVir % 4) == 0);

            if ((xoffset % 4) == 0)
            {
                for (y = yoffset; y < yoffset + img_info->h; y++)
                {
                    i = (y - yoffset) * ((img_info->w + 3) / 4);
                    for (x = xoffset / 4; x < (xoffset + img_info->w) / 4; x++)
                    {
                        fb[y * (xVir / 4) + x] = img_info->data[i++];
                    }

                    if (((xoffset + img_info->w) % 4) != 0)
                    {
                        rt_uint8_t maskval = 0xff >> (img_info->w % 4);
                        fb[y * (xVir / 4) + x] &= maskval;
                        fb[y * (xVir / 4) + x] |= (img_info->data[i++] & (~maskval));
                    }
                }
            }
            else
            {
                for (y = yoffset; y < yoffset + img_info->h; y++)
                {
                    i = (y - yoffset) * ((img_info->w + 3) / 4) * 4;
                    for (x = xoffset; x < xoffset + img_info->w; x++)
                    {
                        bitval = (img_info->data[i / 4] << (i % 4)) & 0xc0;
                        i++;

                        fb[y * (xVir / 4) + x / 4] &= ~(0xc0 >> (x % 4));
                        fb[y * (xVir / 4) + x / 4] |= bitval >> (x % 4);
                    }
                }
            }
        }
        else if (img_info->pixel == RTGRAPHIC_PIXEL_FORMAT_GRAY16)
        {
            RT_ASSERT((xVir % 2) == 0);

            if ((xoffset % 2) == 0)
            {
                for (y = yoffset; y < yoffset + img_info->h; y++)
                {
                    i = (y - yoffset) * ((img_info->w + 1) / 2);
                    for (x = xoffset / 2; x < (xoffset + img_info->w) / 2; x++)
                    {
                        fb[y * (xVir / 2) + x] = img_info->data[i++];
                    }

                    if (((xoffset + img_info->w) % 2) != 0)
                    {
                        rt_uint8_t maskval = 0xff >> (img_info->w % 2);
                        fb[y * (xVir / 2) + x] &= maskval;
                        fb[y * (xVir / 2) + x] |= (img_info->data[i++] & (~maskval));
                    }
                }
            }
            else
            {
                for (y = yoffset; y < yoffset + img_info->h; y++)
                {
                    i = (y - yoffset) * ((img_info->w + 1) / 2) * 2;
                    for (x = xoffset; x < xoffset + img_info->w; x++)
                    {
                        bitval = (img_info->data[i / 2] << (i % 2)) & 0xf0;
                        i++;

                        fb[y * (xVir / 2) + x / 2] &= ~(0xf0 >> (x % 2));
                        fb[y * (xVir / 2) + x / 2] |= bitval >> (x % 2);
                    }
                }
            }
        }
    }
}

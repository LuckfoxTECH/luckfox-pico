/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef PACKAGE
#define PACKAGE "jbig2dec"
#endif

#include <rtthread.h>
#include <rtdevice.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "drv_display.h"
#include "drv_heap.h"
#include "image_info.h"
#include "jbig2dec.h"

#if !defined (UINT32_MAX)
#define UINT32_MAX 0xffffffff
#endif

#define get_uint16(bptr)\
    (((bptr)[0] << 8) | (bptr)[1])
#define get_int16(bptr)\
    (((int)get_uint16(bptr) ^ 0x8000) - 0x8000)

#define  OPTI   1

#define CONTEXT_13 1

#if CONTEXT_13
#define  CONTEXT_VALUE0 0x0efb
#define  CONTEXT_VALUE1 0x0008
#define  CONTEXT_VALUE2 0x0200
#define  CONTEXT_VALUE3 1
#else //else CONTEXT = 16
#define  CONTEXT_VALUE0 0x7bf7
#define  CONTEXT_VALUE1 0x10
#define  CONTEXT_VALUE2 0x800
#define  CONTEXT_VALUE3 0
#endif


void
jbig2_free(Jbig2Allocator *allocator, void *p)
{
    allocator->free(allocator, p);
}

int16_t
jbig2_get_int16(const unsigned char *bptr)
{
    return get_int16(bptr);
}

int32_t
jbig2_get_int32(const unsigned char *bptr)
{
    return ((int32_t)get_int16(bptr) << 16) | get_uint16(bptr + 2);
}

uint32_t
jbig2_get_uint32(const unsigned char *bptr)
{
    return ((uint32_t)get_uint16(bptr) << 16) | get_uint16(bptr + 2);
}

/* could put bit fields in to minimize memory usage */
typedef struct
{
    unsigned short Qe;
    unsigned char mps_xor;               /* mps_xor = index ^ NMPS */
    unsigned char lps_xor;               /* lps_xor = index ^ NLPS ^ (SWITCH << 7) */
} Jbig2ArithQe;

const Jbig2ArithQe jbig2_arith_Qe[47] =
{
    { 0x5601, 1 ^ 0, 1 ^ 0 ^ 0x80 },
    { 0x3401, 2 ^ 1, 6 ^ 1 },
    { 0x1801, 3 ^ 2, 9 ^ 2 },
    { 0x0AC1, 4 ^ 3, 12 ^ 3 },
    { 0x0521, 5 ^ 4, 29 ^ 4 },
    { 0x0221, 38 ^ 5, 33 ^ 5 },
    { 0x5601, 7 ^ 6, 6 ^ 6 ^ 0x80 },
    { 0x5401, 8 ^ 7, 14 ^ 7 },
    { 0x4801, 9 ^ 8, 14 ^ 8 },
    { 0x3801, 10 ^ 9, 14 ^ 9 },
    { 0x3001, 11 ^ 10, 17 ^ 10 },
    { 0x2401, 12 ^ 11, 18 ^ 11 },
    { 0x1C01, 13 ^ 12, 20 ^ 12 },
    { 0x1601, 29 ^ 13, 21 ^ 13 },
    { 0x5601, 15 ^ 14, 14 ^ 14 ^ 0x80 },
    { 0x5401, 16 ^ 15, 14 ^ 15 },
    { 0x5101, 17 ^ 16, 15 ^ 16 },
    { 0x4801, 18 ^ 17, 16 ^ 17 },
    { 0x3801, 19 ^ 18, 17 ^ 18 },
    { 0x3401, 20 ^ 19, 18 ^ 19 },
    { 0x3001, 21 ^ 20, 19 ^ 20 },
    { 0x2801, 22 ^ 21, 19 ^ 21 },
    { 0x2401, 23 ^ 22, 20 ^ 22 },
    { 0x2201, 24 ^ 23, 21 ^ 23 },
    { 0x1C01, 25 ^ 24, 22 ^ 24 },
    { 0x1801, 26 ^ 25, 23 ^ 25 },
    { 0x1601, 27 ^ 26, 24 ^ 26 },
    { 0x1401, 28 ^ 27, 25 ^ 27 },
    { 0x1201, 29 ^ 28, 26 ^ 28 },
    { 0x1101, 30 ^ 29, 27 ^ 29 },
    { 0x0AC1, 31 ^ 30, 28 ^ 30 },
    { 0x09C1, 32 ^ 31, 29 ^ 31 },
    { 0x08A1, 33 ^ 32, 30 ^ 32 },
    { 0x0521, 34 ^ 33, 31 ^ 33 },
    { 0x0441, 35 ^ 34, 32 ^ 34 },
    { 0x02A1, 36 ^ 35, 33 ^ 35 },
    { 0x0221, 37 ^ 36, 34 ^ 36 },
    { 0x0141, 38 ^ 37, 35 ^ 37 },
    { 0x0111, 39 ^ 38, 36 ^ 38 },
    { 0x0085, 40 ^ 39, 37 ^ 39 },
    { 0x0049, 41 ^ 40, 38 ^ 40 },
    { 0x0025, 42 ^ 41, 39 ^ 41 },
    { 0x0015, 43 ^ 42, 40 ^ 42 },
    { 0x0009, 44 ^ 43, 41 ^ 43 },
    { 0x0005, 45 ^ 44, 42 ^ 44 },
    { 0x0001, 45 ^ 45, 43 ^ 45 },
    { 0x5601, 46 ^ 46, 46 ^ 46 }
};

static void *
jbig2_default_alloc(Jbig2Allocator *allocator, size_t size)
{
    return malloc(size);
}

static void
jbig2_default_free(Jbig2Allocator *allocator, void *p)
{
    free(p);
}

static void *
jbig2_default_realloc(Jbig2Allocator *allocator, void *p, size_t size)
{
    return realloc(p, size);
}

static Jbig2Allocator jbig2_default_allocator =
{
    jbig2_default_alloc,
    jbig2_default_free,
    jbig2_default_realloc
};

void *
jbig2_alloc(Jbig2Allocator *allocator, size_t size, size_t num)
{
    if (num > 0 && size > SIZE_MAX / num)
    {
        return NULL;
    }

    return allocator->alloc(allocator, size * num);
}

void *
jbig2_realloc(Jbig2Allocator *allocator, void *p, size_t size, size_t num)
{
    /* check for integer multiplication overflow */
    if (num > 0 && size >= SIZE_MAX / num)
    {
        return NULL;
    }
    return allocator->realloc(allocator, p, size * num);
}

Jbig2Ctx *
jbig2_ctx_new(Jbig2Allocator *allocator)
{
    Jbig2Ctx *result;

    if (allocator == NULL)
        allocator = &jbig2_default_allocator;

    result = (Jbig2Ctx *)jbig2_alloc(allocator, sizeof(Jbig2Ctx), 1);
    if (NULL == result)
    {
        return NULL;
    }


    result->allocator = allocator;
    result->state = JBIG2_FILE_HEADER;
    result->buf = NULL;
    result->n_segments = 0;
    result->segments = jbig2_new(result, Jbig2Segment *, 8);
    if (NULL == result->segments)
    {
        rt_kprintf("malloc failed : fail to malloc result->segments\n");
        return NULL;
    }
    result->segment_index = 0;

    result->current_page = 0;
    result->max_page_index = 4;
    result->pages = jbig2_new(result, Jbig2Page, result->max_page_index);
    if (NULL == result->pages)
    {
        jbig2_ctx_free(result);
        rt_kprintf("malloc failed : fail to malloc result->pages\n");
        return NULL;
    }
    {
        int index;

        for (index = 0; index < result->max_page_index; index++)
        {
            result->pages[index].state = JBIG2_PAGE_FREE;
            result->pages[index].number = 0;
            result->pages[index].width = 0;
            result->pages[index].height = 0xffffffff;
            result->pages[index].x_resolution = 0;
            result->pages[index].y_resolution = 0;
            result->pages[index].flags = 0;
            result->pages[index].image = NULL;
        }
    }

    return result;
}

typedef struct
{
    Jbig2WordStream super;
    const unsigned char *data;
    size_t size;
} Jbig2WordStreamBuf;//

static int//
jbig2_word_stream_buf_get_next_word(Jbig2WordStream *self, size_t offset, uint32_t *word)
{
    Jbig2WordStreamBuf *z = (Jbig2WordStreamBuf *)self;
    uint32_t val = 0;
    int ret = 0;

    if (self == NULL || word == NULL)
        return -1;
    if (offset >= z->size)
    {
        *word = 0;
        return 0;
    }

    if (offset < z->size)
    {
        val |= z->data[offset] << 24;
        ret++;
    }
    if (offset + 1 < z->size)
    {
        val |= z->data[offset + 1] << 16;
        ret++;
    }
    if (offset + 2 < z->size)
    {
        val |= z->data[offset + 2] << 8;
        ret++;
    }
    if (offset + 3 < z->size)
    {
        val |= z->data[offset + 3];
        ret++;
    }
    *word = val;
    return ret;
}

Jbig2WordStream *//
jbig2_word_stream_buf_new(Jbig2Ctx *ctx, const unsigned char *data, size_t size)
{
    Jbig2WordStreamBuf *result = jbig2_new(ctx, Jbig2WordStreamBuf, 1);
    if (NULL == result)
    {
        rt_kprintf("malloc failed : fail to malloc result\n");
        return NULL;
    }
    //Jbig2WordStreamBuf *result = (Jbig2WordStreamBuf *)malloc(sizeof(Jbig2WordStreamBuf) * 1);

    if (result == NULL)
    {
        return NULL;
    }

    result->super.get_next_word = jbig2_word_stream_buf_get_next_word;
    result->data = data;
    result->size = size;

    return &result->super;
}

static void//
jbig2_arith_bytein(Jbig2ArithState *as)
{
    int new_bytes;
    unsigned char B;

    /* invariant: as->next_word_bytes > 0 */

    /* Figure G.3 */
    B = (unsigned char)((as->next_word >> 24) & 0xFF);
    if (B == 0xFF)
    {
        unsigned char B1;

        if (as->next_word_bytes == 1)
        {
            Jbig2WordStream *ws = as->ws;

            new_bytes = ws->get_next_word(ws, as->offset, &as->next_word);
            as->next_word_bytes = new_bytes;
            as->offset += new_bytes;

            B1 = (unsigned char)((as->next_word >> 24) & 0xFF);
            if (B1 > 0x8F)
            {
                as->C += 0xFF00;
                as->CT = 8;
                as->next_word = 0xFF000000 | (as->next_word >> 8);
                as->next_word_bytes = 4;
                as->offset--;
            }
            else
            {
                as->C += B1 << 9;
                as->CT = 7;
            }
        }
        else
        {
            B1 = (unsigned char)((as->next_word >> 16) & 0xFF);
            if (B1 > 0x8F)
            {
                as->C += 0xFF00;
                as->CT = 8;
            }
            else
            {
                as->next_word_bytes--;
                as->next_word <<= 8;
                as->C += (B1 << 9);
                as->CT = 7;
            }
        }
    }
    else
    {
        as->CT = 8;
        as->next_word <<= 8;
        as->next_word_bytes--;
        if (as->next_word_bytes == 0)
        {
            Jbig2WordStream *ws = as->ws;

            new_bytes = ws->get_next_word(ws, as->offset, &as->next_word);
            as->offset += new_bytes;
            as->next_word_bytes = new_bytes;
        }
        B = (unsigned char)((as->next_word >> 24) & 0xFF);
        as->C += (B << 8);
    }
}

Jbig2ArithState *//
jbig2_arith_new(Jbig2Ctx *ctx, Jbig2WordStream *ws)
{
    Jbig2ArithState *result;
    int new_bytes;

    result = jbig2_new(ctx, Jbig2ArithState, 1);
    if (result == NULL)
    {
        return NULL;
    }

    result->ws = ws;

    new_bytes = ws->get_next_word(ws, 0, &result->next_word);
    result->next_word_bytes = new_bytes;
    result->offset = new_bytes;
    result->C = (result->next_word >> 8) & 0xFF0000;
    jbig2_arith_bytein(result);
    result->C <<= 7;
    result->CT -= 7;
    result->A = 0x8000;

    return result;
}

static void//
jbig2_arith_renormd(Jbig2ArithState *as)
{
    /* Figure E.18 */
    do
    {
        if (as->CT == 0)
            jbig2_arith_bytein(as);
        as->A <<= 1;
        as->C <<= 1;
        as->CT--;
    }
    while ((as->A & 0x8000) == 0);
}

#if OPTI
char
jbig2_arith_decode(Jbig2ArithState *as, uint8_t *pcx) // , int *code
{
    uint8_t cx = *pcx;
    const Jbig2ArithQe *pqe;
    unsigned int index = cx & 0x7f;
    char D;

    if (index >= 47)
    {
        return 0;
    }
    else
    {
        pqe = &jbig2_arith_Qe[index];
    }

    /* Figure E.15 */
    as->A -= pqe->Qe;
    if (
        !((as->C >> 16) < pqe->Qe)
    )
    {
        as->C -= pqe->Qe << 16;
        if ((as->A & 0x8000) == 0)
        {
            /* MPS_EXCHANGE, Figure E.16 */
            if (as->A < pqe->Qe)
            {
                D = 1 - (cx >> 7);
                *pcx ^= pqe->lps_xor;
            }
            else
            {
                D = cx >> 7;
                *pcx ^= pqe->mps_xor;
            }

#if 0 //OPTI
            {
                /* Figure E.18 */
                do
                {
                    if (as->CT == 0)
                        jbig2_arith_bytein(as);
                    as->A <<= 1;
                    as->C <<= 1;
                    as->CT--;
                }
                while ((as->A & 0x8000) == 0);
            }
#else
            jbig2_arith_renormd(as);
#endif
            return D;
        }
        else
        {
            return cx >> 7;
        }
    }
    else
    {
        /* LPS_EXCHANGE, Figure E.17 */
        if (as->A < pqe->Qe)
        {
            as->A = pqe->Qe;
            D = cx >> 7;
            *pcx ^= pqe->mps_xor;
        }
        else
        {
            as->A = pqe->Qe;
            D = 1 - (cx >> 7);
            *pcx ^= pqe->lps_xor;
        }
#if 0 //OPTI
        {
            /* Figure E.18 */
            do
            {
                if (as->CT == 0)
                    jbig2_arith_bytein(as);
                as->A <<= 1;
                as->C <<= 1;
                as->CT--;
            }
            while ((as->A & 0x8000) == 0);
        }
#else
        jbig2_arith_renormd(as);
#endif
        return D;
    }
}
#else
char
jbig2_arith_decode(Jbig2ArithState *as, uint8_t *pcx, int *code)
{
    uint8_t cx = *pcx;
    const Jbig2ArithQe *pqe;
    unsigned int index = cx & 0x7f;
    char D;

    if (index >= 47)
    {
        *code = -1;
        return 0;
    }
    else
    {
        pqe = &jbig2_arith_Qe[index];
    }

    /* Figure E.15 */
    as->A -= pqe->Qe;
    if (
        !((as->C >> 16) < pqe->Qe)
    )
    {
        as->C -= pqe->Qe << 16;
        if ((as->A & 0x8000) == 0)
        {
            /* MPS_EXCHANGE, Figure E.16 */
            if (as->A < pqe->Qe)
            {
                D = 1 - (cx >> 7);
                *pcx ^= pqe->lps_xor;
            }
            else
            {
                D = cx >> 7;
                *pcx ^= pqe->mps_xor;
            }

#if 0 //OPTI
            {
                /* Figure E.18 */
                do
                {
                    if (as->CT == 0)
                        jbig2_arith_bytein(as);
                    as->A <<= 1;
                    as->C <<= 1;
                    as->CT--;
                }
                while ((as->A & 0x8000) == 0);
            }
#else
            jbig2_arith_renormd(as);
#endif
            *code = 0;
            return D;
        }
        else
        {
            *code = 0;
            return cx >> 7;
        }
    }
    else
    {
        /* LPS_EXCHANGE, Figure E.17 */
        if (as->A < pqe->Qe)
        {
            as->A = pqe->Qe;
            D = cx >> 7;
            *pcx ^= pqe->mps_xor;
        }
        else
        {
            as->A = pqe->Qe;
            D = 1 - (cx >> 7);
            *pcx ^= pqe->lps_xor;
        }
#if 0 //OPTI
        {
            /* Figure E.18 */
            do
            {
                if (as->CT == 0)
                    jbig2_arith_bytein(as);
                as->A <<= 1;
                as->C <<= 1;
                as->CT--;
            }
            while ((as->A & 0x8000) == 0);
        }
#else
        jbig2_arith_renormd(as);
#endif
        *code = 0;
        return D;
    }
}
#endif

void//
jbig2_image_free(Jbig2Ctx *ctx, Jbig2Image *image)
{
    if (image != NULL)
    {
        //jbig2_free(ctx->allocator, image->data);
        jbig2_free(ctx->allocator, image);
    }
}

void
jbig2_image_release(Jbig2Ctx *ctx, Jbig2Image *image)
{
    if (image == NULL)
        return;
    image->refcount--;
    if (image->refcount == 0)
        jbig2_image_free(ctx, image);
}

Jbig2Allocator *
jbig2_ctx_free(Jbig2Ctx *ctx)
{
    Jbig2Allocator *ca;
    int i;

    if (ctx == NULL)
    {
        return NULL;
    }
    ca = ctx->allocator;
    jbig2_free(ca, ctx->buf);
    if (ctx->segments != NULL)
    {
        for (i = 0; i < ctx->n_segments; i++)
        {
            if (ctx->segments[i] == NULL)
                continue;
            jbig2_free(ctx->allocator, ctx->segments[i]->referred_to_segments);
            if ((4 == (ctx->segments[i]->flags & 63) || 40 == (ctx->segments[i]->flags & 63)) && ctx->segments[i]->result != NULL)
            {
                jbig2_image_release(ctx, (Jbig2Image *)ctx->segments[i]->result);
            }
            jbig2_free(ctx->allocator, ctx->segments[i]);
        }
        jbig2_free(ca, ctx->segments);
    }

    if (ctx->pages != NULL)
    {
        for (i = 0; i <= ctx->current_page; i++)
            if (ctx->pages[i].image != NULL)
                jbig2_image_release(ctx, ctx->pages[i].image);
        jbig2_free(ca, ctx->pages);
    }

    jbig2_free(ca, ctx);

    return ca;
}

int jbig2_decompression(image_info_t *img_info, rt_uint8_t *fb, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset)
{

    Jbig2Ctx *ctx = NULL;
    Jbig2WordStream *ws = NULL;
    Jbig2ArithState *as = NULL;
    uint8_t *GB_stats = NULL;
    int stats_size;
    rt_int16_t width = img_info->w;
    rt_int16_t height = img_info->h;
    size_t length = img_info->size;
    rt_uint8_t *headbuf = RT_NULL;
    rt_uint8_t *tailbuf = RT_NULL;
    rt_uint16_t align8_x1, align8_x2;
    rt_uint8_t  mod8_x1, mod8_x2;

    // Initial buffer(clear)
    align8_x1 = xoffset / 8;
    align8_x2 = (xoffset + img_info->w + 7) / 8;
    mod8_x1   = xoffset % 8;
    mod8_x2   = (xoffset + img_info->w) % 8;
    //rt_kprintf("xoffset   = %d, width     = %d\n", xoffset, width);
    //rt_kprintf("align8_x1 = %d, align8_x2 = %d\n", align8_x1, align8_x2);
    //rt_kprintf("mod8_x1   = %d, mod8_x2   = %d\n", mod8_x1, mod8_x2);
    {
        rt_uint16_t x, y;

        if (mod8_x1 != 0)
        {
            headbuf = rt_malloc(height);
            RT_ASSERT(headbuf != RT_NULL);
        }

        if (mod8_x2 != 0)
        {
            tailbuf = rt_malloc(height);
            RT_ASSERT(tailbuf != RT_NULL);
        }

        for (y = yoffset; y < yoffset + height; y++)
        {
            // Backup left margin if not align8
            if (mod8_x1 != 0)
            {
                headbuf[y - yoffset] = fb[y * (xVir >> 3) + align8_x1];
            }

            // Backup right margin if not align8
            if (mod8_x2 != 0)
            {
                tailbuf[y - yoffset] = fb[y * (xVir >> 3) + (align8_x2 - 1)];
            }

            for (x = align8_x1; x < align8_x2; x++)
            {
                fb[y * (xVir >> 3) + x] = 0;
            }
        }
    }

    ctx = jbig2_ctx_new(NULL);
    if (NULL == ctx)
    {
        rt_kprintf("malloc failed : fail to malloc ctx\n");
        return -1;
    }
#if CONTEXT_13
    stats_size = 1 << 13;
#else
    stats_size = 1 << 16;
#endif
    GB_stats = jbig2_new(ctx, uint8_t, stats_size);
    if (NULL == GB_stats)
    {
        rt_kprintf("malloc failed : fail to malloc GB_stats\n");
        return -1;
    }
    memset(GB_stats, 0, stats_size);

    ws = jbig2_word_stream_buf_new(ctx, img_info->data, length);
    if (NULL == ws)
    {
        rt_kprintf("malloc failed : fail to malloc ws\n");
        return -1;
    }
    as = jbig2_arith_new(ctx, ws);
    if (NULL == as)
    {
        rt_kprintf("malloc failed : fail to malloc as\n");
        return -1;
    }

    if (1)
    {
        const uint32_t GBW = width;
        const uint32_t GBH = height;
        uint32_t x, y;
        unsigned char *line2 = NULL;
        unsigned char *line1 = NULL;

        if (GBW <= 0 || GBH <= 0)
            return -1;

        for (y = 0; y < GBH; y++)
        {
            uint32_t CONTEXT;
            uint32_t line_m1;
            uint32_t line_m2;
            uint32_t padded_width = (GBW + 7) & (-8);
            int code_t = 0;

#if CONTEXT_13
            line_m1 = line1 ? (line1[0]) : 0;
            line_m2 = line2 ? line2[0] << 4 : 0;
            CONTEXT = ((line_m1 >> 1) & 0x1f8) | (line_m2 & 0x1e00);
#else
            line_m1 = line1 ? line1[0] : 0;
            line_m2 = line2 ? line2[0] << 6 : 0;
            CONTEXT = (line_m1 & 0x7f0) | (line_m2 & 0xf800);
#endif

            for (x = 0; x < padded_width; x += 8)
            {
#if OPTI
                unsigned char result = 0;
#else
                unsigned char result = 0;
#endif
                int x_minor;
                int minor_width = (GBW - x) > 8 ? 8 : GBW - x;

                if (line1)
                    line_m1 = (line_m1 << 8) | (x + 8 < GBW ? line1[(x >> 3) + 1] : 0);

                if (line2)
                {
#if CONTEXT_13
                    line_m2 = (line_m2 << 8) | (x + 8 < GBW ? line2[(x >> 3) + 1] << 4 : 0);
#else
                    line_m2 = (line_m2 << 8) | (x + 8 < GBW ? line2[(x >> 3) + 1] << 6 : 0);
#endif
                }
#if OPTI
                {
                    if (minor_width == 8)
                    {
                        uint8_t bit;
                        // 0
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (7 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (7)) & CONTEXT_VALUE2);
                        result = (bit << 7);
                        // 1
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (6 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (6)) & CONTEXT_VALUE2);
                        result |= (bit << 6);
                        // 2
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (5 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (5)) & CONTEXT_VALUE2);
                        result |= (bit << 5);
                        // 3
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (4 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (4)) & CONTEXT_VALUE2);
                        result |= (bit << 4);
                        // 4
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (3 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (3)) & CONTEXT_VALUE2);
                        result |= (bit << 3);
                        // 5
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (2 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (2)) & CONTEXT_VALUE2);
                        result |= (bit << 2);
                        // 6
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (1 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2 >> (1)) & CONTEXT_VALUE2);
                        result |= (bit << 1);
                        // 7
                        bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                        CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (0 + CONTEXT_VALUE3)) & CONTEXT_VALUE1) | ((line_m2)& CONTEXT_VALUE2);
                        result |= (bit);
                    }
                    else
                    {
                        for (x_minor = 0; x_minor < minor_width; x_minor++)
                        {
                            uint8_t bit;

                            bit = jbig2_arith_decode(as, &GB_stats[CONTEXT]);
                            if (code_t)
                                return -1;
                            result |= bit << (7 - x_minor);
                            CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (7 + CONTEXT_VALUE3 - x_minor)) & CONTEXT_VALUE1) | ((line_m2 >> (7 - x_minor)) & CONTEXT_VALUE2);
                        }
                    }
                }
#else
                for (x_minor = 0; x_minor < minor_width; x_minor++)
                {
                    uint8_t bit;

                    bit = jbig2_arith_decode(as, &GB_stats[CONTEXT], &code_t);
                    if (code_t)
                        return -1;
                    result |= bit << (7 - x_minor);
                    CONTEXT = ((CONTEXT & CONTEXT_VALUE0) << 1) | bit | ((line_m1 >> (7 + CONTEXT_VALUE3 - x_minor)) & CONTEXT_VALUE3) | ((line_m2 >> (7 - x_minor)) & CONTEXT_VALUE2);
                }
#endif
                if (result != 0)
                {
                    fb[(y + yoffset) * (xVir >> 3) + ((x + xoffset) >> 3)] = result;
                }
            }
            line2 = line1;
            line1 = fb + (xVir / 8) * (y + yoffset) + (xoffset >> 3);
        }

    }

    // Restore left margin if not align8 & shift the decode result
    if (mod8_x1 != 0)
    {
        rt_uint16_t x, y;
        rt_uint8_t bitmask = 0xff >> (mod8_x1);
        for (y = yoffset; y < yoffset + height; y++)
        {
            rt_uint8_t temp  = 0;
            rt_uint8_t shift = headbuf[y - yoffset] & (rt_uint8_t)(~bitmask);
            for (x = align8_x1; x < align8_x2; x++)
            {
                temp = fb[y * (xVir >> 3) + x];

                fb[y * (xVir >> 3) + x] >>= mod8_x1;
                fb[y * (xVir >> 3) + x]  |= shift;

                shift = temp << (8 - mod8_x1);

            }
        }
        rt_free(headbuf);
    }

    // Restore right margin if not align8
    if (mod8_x2 != 0)
    {
        rt_uint16_t y;
        for (y = yoffset; y < yoffset + height; y++)
        {
            rt_uint8_t bitmask = 0xff << (8 - mod8_x2);
            tailbuf[y - yoffset] &= (~bitmask);
            fb[y * (xVir >> 3) + (align8_x2 - 1)] &= bitmask;
            fb[y * (xVir >> 3) + (align8_x2 - 1)] |= tailbuf[y - yoffset];
        }
        rt_free(tailbuf);
    }

    jbig2_free(ctx->allocator, as);
    jbig2_free(ctx->allocator, ws);
    jbig2_free(ctx->allocator, GB_stats);
    jbig2_ctx_free(ctx);
    return 0;
}

/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#pragma once
#include <stdint.h>

/* warning levels */
typedef enum
{
    JBIG2_SEVERITY_DEBUG,
    JBIG2_SEVERITY_INFO,
    JBIG2_SEVERITY_WARNING,
    JBIG2_SEVERITY_FATAL
} Jbig2Severity;

typedef enum
{
    JBIG2_OPTIONS_EMBEDDED = 1
} Jbig2Options;

///* forward public structure declarations */
typedef struct _Jbig2Allocator Jbig2Allocator;
typedef struct _Jbig2Ctx Jbig2Ctx;
typedef struct _Jbig2GlobalCtx Jbig2GlobalCtx;

/*
this is the general image structure used by the jbig2dec library
images are 1 bpp, packed into rows a byte at a time. stride gives
the byte offset to the next row, while width and height define
the size of the image area in pixels.
*/
typedef struct _Jbig2Image Jbig2Image;
struct _Jbig2Image
{
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint8_t *data;
    int refcount;
};

/* errors are returned from the library via a callback. If no callback
is provided (a NULL argument is passed to jbig2_ctx_new) a default
handler is used which prints fatal errors to the stderr stream. */

/* error callback */
typedef void(*Jbig2ErrorCallback)(void *data, const char *msg, Jbig2Severity severity, int32_t seg_idx);

/* memory allocation is likewise done via a set of callbacks so that
clients can better control memory usage. If a NULL is passed for
this argument of jbig2_ctx_new, a default allocator based on malloc()
is used. */

/* dynamic memory callbacks */
struct _Jbig2Allocator
{
    void *(*alloc)(Jbig2Allocator *allocator, size_t size);
    void(*free)(Jbig2Allocator *allocator, void *p);
    void *(*realloc)(Jbig2Allocator *allocator, void *p, size_t size);
};

/* decoder context */
Jbig2Ctx *jbig2_ctx_new(Jbig2Allocator *allocator);
Jbig2Allocator *jbig2_ctx_free(Jbig2Ctx *ctx);

///* submit data to the decoder */
int jbig2_data_in(Jbig2Ctx *ctx, const unsigned char *data, size_t size, rt_uint8_t *fb, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset);
//
///* get the next available decoded page image. NULL means there isn't one. */
Jbig2Image *jbig2_page_out(Jbig2Ctx *ctx);
///* mark a returned page image as no longer needed. */
void jbig2_release_page(Jbig2Ctx *ctx, Jbig2Image *image);
///* mark the current page as complete, simulating an end-of-page segment (for broken streams) */
int jbig2_complete_page(Jbig2Ctx *ctx);

//jbig2_priv.h
typedef struct _Jbig2Page Jbig2Page;
typedef struct _Jbig2Segment Jbig2Segment;

typedef enum
{
    JBIG2_FILE_HEADER,
    JBIG2_FILE_SEQUENTIAL_HEADER,
    JBIG2_FILE_SEQUENTIAL_BODY,
    JBIG2_FILE_EOF
} Jbig2FileState;

struct _Jbig2Ctx
{
    Jbig2Allocator *allocator;
    Jbig2Options options;
    const Jbig2Ctx *global_ctx;
    Jbig2ErrorCallback error_callback;
    void *error_callback_data;

    uint8_t *buf;
    size_t buf_size;
    unsigned int buf_rd_ix;
    unsigned int buf_wr_ix;

    Jbig2FileState state;

    uint8_t file_header_flags;
    uint32_t n_pages;

    int n_segments_max;
    Jbig2Segment **segments;
    int n_segments;             /* index of last segment header parsed */
    int segment_index;          /* index of last segment body parsed */

    /* list of decoded pages, including the one in progress,
    currently stored as a contiguous, 0-indexed array. */
    int current_page;
    int max_page_index;
    Jbig2Page *pages;
};

///* dynamic memory management */
void *jbig2_alloc(Jbig2Allocator *allocator, size_t size, size_t num);

void jbig2_free(Jbig2Allocator *allocator, void *p);

void *jbig2_realloc(Jbig2Allocator *allocator, void *p, size_t size, size_t num);

#define jbig2_new(ctx, t, size) ((t *)jbig2_alloc(ctx->allocator, size, sizeof(t)))


/* The word stream design is a compromise between simplicity and
trying to amortize the number of method calls. Each ::get_next_word
invocation pulls 4 bytes from the stream, packed big-endian into a
32 bit word. The offset argument is provided as a convenience. It
begins at 0 and increments by 4 for each successive invocation. */
typedef struct _Jbig2WordStream Jbig2WordStream;

struct _Jbig2WordStream
{
    int(*get_next_word)(Jbig2WordStream *self, size_t offset, uint32_t *word);
};
Jbig2WordStream *jbig2_word_stream_buf_new(Jbig2Ctx *ctx, const unsigned char *data, size_t size);

typedef enum
{
    JBIG2_COMPOSE_OR = 0,
    JBIG2_COMPOSE_AND = 1,
    JBIG2_COMPOSE_XOR = 2,
    JBIG2_COMPOSE_XNOR = 3,
    JBIG2_COMPOSE_REPLACE = 4
} Jbig2ComposeOp;

void jbig2_image_release(Jbig2Ctx *ctx, Jbig2Image *image);
Jbig2Image *jbig2_image_reference(Jbig2Ctx *ctx, Jbig2Image *image);
void jbig2_image_free(Jbig2Ctx *ctx, Jbig2Image *image);
void jbig2_image_clear(Jbig2Ctx *ctx, Jbig2Image *image, int value);

struct _Jbig2Segment
{
    uint32_t number;
    uint8_t flags;
    uint32_t page_association;
    size_t data_length;
    int referred_to_segment_count;
    uint32_t *referred_to_segments;
    uint32_t rows;
    void *result;
};

Jbig2Segment *jbig2_parse_segment_header(Jbig2Ctx *ctx, uint8_t *buf, size_t buf_size, size_t *p_header_size);
int jbig2_parse_segment(Jbig2Ctx *ctx, Jbig2Segment *segment, const uint8_t *segment_data, rt_uint8_t *fb, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset);
uint32_t jbig2_get_uint32(const unsigned char *bptr);
int16_t jbig2_get_int16(const unsigned char *bptr);

typedef struct
{
    uint32_t width;
    uint32_t height;
    uint32_t x;
    uint32_t y;
    Jbig2ComposeOp op;
    uint8_t flags;
} Jbig2RegionSegmentInfo;

typedef enum
{
    JBIG2_PAGE_FREE,
    JBIG2_PAGE_NEW,
    JBIG2_PAGE_COMPLETE,
    JBIG2_PAGE_RETURNED,
    JBIG2_PAGE_RELEASED
} Jbig2PageState;

struct _Jbig2Page
{
    Jbig2PageState state;
    uint32_t number;
    uint32_t height, width;     /* in pixels */
    uint32_t x_resolution, y_resolution;        /* in pixels per meter */
    uint16_t stripe_size;
    uint8_t striped;
    uint32_t end_row;
    uint8_t flags;
    Jbig2Image *image;
};

int jbig2_page_info(Jbig2Ctx *ctx, Jbig2Segment *segment, const uint8_t *segment_data, uint32_t flag);
int jbig2_end_of_page(Jbig2Ctx *ctx, Jbig2Segment *segment, const uint8_t *segment_data);

typedef struct _Jbig2ArithState Jbig2ArithState;

struct _Jbig2ArithState
{
    uint32_t C;
    int A;

    int CT;

    uint32_t next_word;
    int next_word_bytes;

    Jbig2WordStream *ws;
    int offset;
};

/* An arithmetic coding context is stored as a single byte, with the
index in the low order 7 bits (actually only 6 are used), and the
MPS in the top bit. */

/* allocate and initialize a new arithmetic coding state */
Jbig2ArithState *jbig2_arith_new(Jbig2Ctx *ctx, Jbig2WordStream *ws);

/* decode a bit */
//char jbig2_arith_decode(Jbig2ArithState *as, uint8_t *pcx);

typedef struct
{
    int GBTEMPLATE;
    uint8_t TPGDON;
    uint8_t USESKIP;
    Jbig2Image *SKIP;
    int8_t gbat[8];
} Jbig2GenericRegionParams;

int jbig2_decompression(image_info_t *img_info, rt_uint8_t *fb, rt_int32_t xVir, rt_int32_t xoffset, rt_int32_t yoffset);
/*
 * Copyright (c) 2021 Fuzhou Rockchip Electronic Co.,Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-09     Jair Wu      First version
 *
 */

#ifndef _FLAC_DECODE_H_
#define _FLAC_DECODE_H_

#include "extcodec.h"

#ifdef RT_USING_EXT_FLAC_DECODER

typedef enum
{

    FLAC__STREAM_DECODER_SEARCH_FOR_METADATA = 0,
    /**< The decoder is ready to search for metadata. */

    FLAC__STREAM_DECODER_READ_METADATA,
    /**< The decoder is ready to or is in the process of reading metadata. */

    FLAC__STREAM_DECODER_SEARCH_FOR_FRAME_SYNC,
    /**< The decoder is ready to or is in the process of searching for the
     * frame sync code.
     */

    FLAC__STREAM_DECODER_READ_FRAME,
    /**< The decoder is ready to or is in the process of reading a frame. */

    FLAC__STREAM_DECODER_END_OF_STREAM,
    /**< The decoder has reached the end of the stream. */

    FLAC__STREAM_DECODER_OGG_ERROR,
    /**< An error occurred in the underlying Ogg layer.  */

    FLAC__STREAM_DECODER_SEEK_ERROR,
    /**< An error occurred while seeking.  The decoder must be flushed
     * with FLAC__stream_decoder_flush() or reset with
     * FLAC__stream_decoder_reset() before decoding can continue.
     */

    FLAC__STREAM_DECODER_ABORTED,
    /**< The decoder was aborted by the read or write callback. */

    FLAC__STREAM_DECODER_MEMORY_ALLOCATION_ERROR,
    /**< An error occurred allocating memory.  The decoder is in an invalid
     * state and can no longer be used.
     */

    FLAC__STREAM_DECODER_UNINITIALIZED
    /**< The decoder is in the uninitialized state; one of the
     * FLAC__stream_decoder_init_*() functions must be called before samples
     * can be processed.
     */

} FLAC__StreamDecoderState;

struct flac_dec
{
    void *decoder;
    uint32_t total_samples;
    uint16_t sample_rate;
    uint8_t channels;
    uint8_t bits;

    uint8_t *buf_in;
    uint32_t buf_in_size;
    uint32_t buf_in_left;
    uint32_t *buf_out[8];
    uint32_t buf_out_size;

    uint32_t file_size;
    uint32_t file_tell;

    uint8_t error_code;
    uint8_t out_of_data;
};

int32_t flac_dec_init(struct flac_dec *dec);
int32_t flac_dec_process(struct flac_dec *dec);
int32_t flac_dec_deinit(struct flac_dec *dec);
#endif  // RT_USING_EXT_FLAC_DECODER

#endif  // _FLAC_DECODE_H_

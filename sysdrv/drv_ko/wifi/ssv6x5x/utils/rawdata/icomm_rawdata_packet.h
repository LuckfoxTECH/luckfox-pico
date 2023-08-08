/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _ICOMM_RAWDATA_PACKET_H_
#define _ICOMM_RAWDATA_PACKET_H_

unsigned char raw_data_packet[] = {
    0x40, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x78,
    0x00, 0x00, 0x01, 0x04, 0x02, 0x04, 0x0b, 0x16,
    0x32, 0x08, 0x0c, 0x12, 0x18, 0x24, 0x30, 0x48,
    0x60, 0x6c
    };

#endif

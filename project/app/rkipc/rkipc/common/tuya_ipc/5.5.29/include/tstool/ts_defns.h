/*
 * Definitions for working with H.222 Transport Stream packets
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the MPEG TS, PS and ES tools.
 *
 * The Initial Developer of the Original Code is Amino Communications Ltd.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Amino Communications Ltd, Swavesey, Cambridge UK
 *
 * ***** END LICENSE BLOCK *****
 */

#ifndef _ts_defns
#define _ts_defns

// Transport Stream packets are always the same size
#define TS_PACKET_SIZE 188

// When we are putting data into a TS packet, we need the first four
// bytes for heading information, which means that we will have at most
// 184 bytes for our payload
#define MAX_TS_PAYLOAD_SIZE (TS_PACKET_SIZE - 4)

// ------------------------------------------------------------
// The number of TS packets to read ahead
#define TS_READ_AHEAD_COUNT 1024 // aim for multiple of block boundary -- used to be 50
// Thus the number of bytes to read ahead
#define TS_READ_AHEAD_BYTES TS_READ_AHEAD_COUNT *TS_PACKET_SIZE

// A read-ahead buffer for reading TS packets.
//
// Note that `posn` always gives the file position of the *next* TS packet to
// be read from the file (so after reading a TS packet with
// `read_next_TS_packet`, the position of said packet is `posn`-TS_PACKET_SIZE)
struct _ts_reader {
	int file;      // the file to read from
	offset_t posn; // the position of the next-to-be-read TS packet
	void *handle;  // handle to pass to read_fn and seek_fn.

	// Reader and seek functions. If these are non-NULL we call them
	//  when we would call read() or seek().
	int (*read_fn)(void *, byte *, size_t);
	int (*seek_fn)(void *, offset_t);

	byte read_ahead[TS_READ_AHEAD_COUNT * TS_PACKET_SIZE];
	byte *read_ahead_ptr; // location of next packet in said array
	byte *read_ahead_end; // pointer just after the end of `read_ahead`
};
typedef struct _ts_reader *TS_reader_p;
#define SIZEOF_TS_READER sizeof(struct _ts_reader)

#endif // _ts_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

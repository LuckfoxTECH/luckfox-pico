/*
 * Datastructures for handling H.264 elementary streams.
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

#ifndef _es_defns
#define _es_defns

#include "compat.h"
#include "pes_defns.h"
#include <stdio.h>

// ------------------------------------------------------------
// A "file" offset in an ES stream, suitable for seeking to
// For an ES based on a "bare" file, the `infile` value is all that is needed
// For an ES based on a PES, the file offset of the PES packet and the byte
// offset within that packet's ES data are needed.
struct _ES_offset {
	offset_t infile; // as used by lseek
	int32_t inpacket;
};
typedef struct _ES_offset ES_offset;
typedef struct _ES_offset *ES_offset_p;
#define SIZEOF_ES_OFFSET sizeof(struct _ES_offset)

// The number of bytes to "read ahead" when reading directly from an
// elementary stream
#define ES_READ_AHEAD_SIZE 1000

// ------------------------------------------------------------
// A datastructure to represent our input elementary stream (ES)
// (*output* elementary streams shouldn't need any particular housekeeping)
struct elementary_stream {
	int reading_ES; // TRUE if we're reading ES data direct, FALSE if PES

	// If we're reading from an elementary data stream directly, then
	// we use the input directly
	int input;
	// And maintain a buffer of "read ahead" bytes
	byte read_ahead[ES_READ_AHEAD_SIZE];
	offset_t read_ahead_posn; // location of this data in the file
	int32_t read_ahead_len;   // actual number of bytes in the buffer

	// And the next byte to be read is specified by its offset in said
	// data stream. For "bare" ES data, the `infile` value is used to
	// remember the next bytes actual position in the file, and for PES
	// based ES data, the `inpacket` value is used to remember the next
	// bytes offset in the current PES packet. In both cases, the "unused"
	// quantity in the ES_offset is undefined.
	// (this is, in fact, more used by tsserve than by anything else)
	ES_offset posn_of_next_byte;

	// If we're reading from PES packets (from either a PS or TS file),
	// then we need to remember our PES reader
	PES_reader_p reader;

	byte *data;     // Where we're reading our bytes from
	byte *data_end; // How to tell we've read them all
	byte *data_ptr; // And which byte we're interested in

	offset_t last_packet_posn;       // Where the last PES packet was in the file
	int32_t last_packet_es_data_len; // And its number of ES bytes

	// Regardless, our triple byte memory is the same
	byte cur_byte;   // The current (last read) byte
	byte prev1_byte; // The previous byte
	byte prev2_byte; // The byte before *that*
};
typedef struct elementary_stream *ES_p;
#define SIZEOF_ES sizeof(struct elementary_stream)

// ------------------------------------------------------------
// And a representation of a single unit from the elementary stream
// (whether an MPEG-2 (H.262) item, or an MPEG-4/AVC (H.264) NAL unit)
// - basically, the thing that starts with a 00 00 01 prefix, and continues
// to end of file or before the next 00 00 01 prefix (so note that it
// contains any "trailing" 00 bytes).
//
// The normal way to acquire such a datastructure is via `build_ES_unit()`,
// or `find_and_build_next_ES_unit()`. If instead you want to use the
// address of a `struct ES_unit`, it is imperative that it be set up
// correctly with `setup_ES_unit()` before it is passed to any of the
// functions that use it, otherwise the contents will not be valid (and,
// particularly, the "data" pointer will reference random memory).
struct ES_unit {
	ES_offset start_posn; // The start of the current data unit
	byte *data;           // Its data, including the leading 00 00 01
	uint32_t data_len;    // Its length
	uint32_t data_size;   // The total buffer size

	byte start_code; // The byte after the 00 00 01 prefix

	// Something of a hack - if we were reading PES, did any of the PES packets
	// we read to make this ES unit contain a PTS?
	byte PES_had_PTS;
};
typedef struct ES_unit *ES_unit_p;
#define SIZEOF_ES_UNIT sizeof(struct ES_unit)

// Start and increment sizes for the es_unit/data array.
#define ES_UNIT_DATA_START_SIZE 1000 // was 500
#define ES_UNIT_DATA_INCREMENT 500   // was 100

// ------------------------------------------------------------
// An expandable list of ES units
struct ES_unit_list {
	struct ES_unit *array; // The current array of ES units
	int length;            // How many there are
	int size;              // How big the array is
};
typedef struct ES_unit_list *ES_unit_list_p;
#define SIZEOF_ES_UNIT_LIST sizeof(struct ES_unit_list)

#define ES_UNIT_LIST_START_SIZE 20
#define ES_UNIT_LIST_INCREMENT 20

#endif // _es_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

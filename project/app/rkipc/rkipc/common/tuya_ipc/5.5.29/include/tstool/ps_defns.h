/*
 * Datastructures for working with H.222 Program Stream packets - in
 * particular, for reading PES packets.
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

#ifndef _ps_defns
#define _ps_defns

#include "compat.h"
#include "h222_defns.h"
#include "tswrite_defns.h"

// ------------------------------------------------------------
// A program stream context, used to read PS and manage a read-ahead cache

#define PS_READ_AHEAD_SIZE 5000 // The number of bytes to read ahead

struct ps_reader {
	int input;      // where we're reading from
	offset_t start; // the offset at which our data starts

	byte data[PS_READ_AHEAD_SIZE];
	offset_t data_posn; // location of this data in the file
	int32_t data_len;   // actual number of bytes in the buffer
	byte *data_end;     // off the end of `data`
	byte *data_ptr;     // which byte we're interested in (next)
};
typedef struct ps_reader *PS_reader_p;
#define SIZEOF_PS_READER sizeof(struct ps_reader)

// ------------------------------------------------------------
// A program stream pack header (not including the system header packets)
struct PS_pack_header {
	int id;        // A number to identify this packet
	byte data[10]; // The data excluding the leading 00 00 01 BA
	uint64_t scr;  // Formed from scr_base and scr_ext
	uint64_t scr_base;
	uint32_t scr_extn;
	uint32_t program_mux_rate;
	int pack_stuffing_length;
};
typedef struct PS_pack_header *PS_pack_header_p;
#define SIZEOF_PS_PACK_HEADER sizeof(struct PS_pack_header)

// ------------------------------------------------------------
// A program stream packet (specifically one that starts with six bytes
// organised as 00 00 01 <stream id> <packet length>)
struct PS_packet {
	int id; // A number to identify this packet

	byte *data;   // The data including the leading 00 00 01
	int data_len; // Its length

	byte stream_id;    // Its stream id (i.e., data[4])
	int packet_length; // The packet length (6 less than data_len)
};
typedef struct PS_packet *PS_packet_p;
#define SIZEOF_PS_PACKET sizeof(struct PS_packet)

// ------------------------------------------------------------
// Number of streams of various types

#define NUMBER_VIDEO_STREAMS 0x0F
#define NUMBER_AUDIO_STREAMS 0x1F
#define NUMBER_AC3_SUBSTREAMS 0x08

// DVD private_stream_1 substream identifiers
// (also used for non-DVD data when we have identified the private data
// appropriately)
#define SUBSTREAM_OTHER 0
#define SUBSTREAM_AC3 1          // AC-3 audio (Dolby 5.1)
#define SUBSTREAM_DTS 2          // DTS  audio
#define SUBSTREAM_LPCM 3         // LPCM audio (CD audio)
#define SUBSTREAM_SUBPICTURES 4  // Sub-pictures
#define SUBSTREAM_ERROR 5        // Error in deciding
#define NUMBER_SUBSTREAM_TYPES 6 // useful for array sizing

#define SUBSTREAM_STR(what)                                                                        \
	((what) == SUBSTREAM_OTHER                                                                     \
	     ? "other"                                                                                 \
	     : (what) == SUBSTREAM_AC3                                                                 \
	           ? "AC3"                                                                             \
	           : (what) == SUBSTREAM_DTS                                                           \
	                 ? "DTS"                                                                       \
	                 : (what) == SUBSTREAM_LPCM                                                    \
	                       ? "LPCM"                                                                \
	                       : (what) == SUBSTREAM_SUBPICTURES ? "subpictures" : "???")

#define SUBSTREAM_IS_AUDIO(what)                                                                   \
	((what) == SUBSTREAM_AC3 || (what) == SUBSTREAM_DTS || (what) == SUBSTREAM_LPCM)

#define BSMOD_STR(bsmod, acmod)                                                                    \
	((bsmod) == 0                                                                                  \
	     ? "main audio service: complete main (CM)"                                                \
	     : (bsmod) == 1                                                                            \
	           ? "main audio service: music & effects (ME)"                                        \
	           : (bsmod) == 2                                                                      \
	                 ? "associated service: visually impaired (VI)"                                \
	                 : (bsmod) == 3                                                                \
	                       ? "associated service: hearing impaired (HI)"                           \
	                       : (bsmod) == 4                                                          \
	                             ? "associated service: dialogue (D)"                              \
	                             : (bsmod) == 5                                                    \
	                                   ? "associated service: commentary (C)"                      \
	                                   : (bsmod) == 6                                              \
	                                         ? "associated service: emergency (E)"                 \
	                                         : (bsmod) == 7 && (acmod) == 1                        \
	                                               ? "associated service: voice over (VO)"         \
	                                               : (bsmod) == 7 && (acmod) >= 2 && (acmod) <= 7  \
	                                                     ? "main audio service: karaoke"           \
	                                                     : "???")

#define ACMOD_STR(acmod)                                                                           \
	((acmod) == 0                                                                                  \
	     ? "1+1 Ch1,Ch2"                                                                           \
	     : (acmod) == 1                                                                            \
	           ? "1/0 C"                                                                           \
	           : (acmod) == 2                                                                      \
	                 ? "2/0 L,R"                                                                   \
	                 : (acmod) == 3                                                                \
	                       ? "3/0 L,C,R"                                                           \
	                       : (acmod) == 4                                                          \
	                             ? "2/1 L,R,S"                                                     \
	                             : (acmod) == 5                                                    \
	                                   ? "3/1 L,C,R,S"                                             \
	                                   : (acmod) == 6 ? "2/2 L,R,SL,SR"                            \
	                                                  : (acmod) == 7 ? "3/2 L,C,R,SL,SR" : "???")

#endif // _ps_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

/*
 * Datastructures for reading PES packets from TS or PS files
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

#ifndef _pes_defns
#define _pes_defns

#include "compat.h"
#include "pidint_defns.h"
#include "ps_defns.h"
#include "ts_defns.h"
#include "tswrite_defns.h"

// ------------------------------------------------------------
// A PES packet comes with some useful associated data
struct PES_packet_data {
	byte *data;       // The actual packet data
	int32_t data_len; // The length of the `data` array [1]
	int32_t length;   // Its length
	offset_t posn;    // The offset of its start in the file [2]
	int is_video;     // Is this video data? (as opposed to audio)

	// For convenience, it's useful to be able to get at the PES packet's
	// "payload" (i.e., the ES data) as if it were a separate array. This
	// is, of course, just an offset into `data`
	byte *es_data;
	int32_t es_data_len;
	// The PES packet *does* tell us if its data starts with an ES packet
	// (i.e., if the 00 00 01 bytes come as the first bytes in the data),
	// so that's worth remembering
	int data_alignment_indicator;

	// Some applications want to know if a particular packet contains
	// a PTS or not
	int has_PTS;
};
// [1] For PS data, data_len and length will always be the same.
//     For TS data, length is set when the first TS packet of the
//     PES packet is read, and data_len gradually increases to length
//     as "chunks" of the PES packet are read in
// [2] For TS data, this is actually the offset of the first TS packet
//     containing the PES packet
typedef struct PES_packet_data *PES_packet_data_p;
#define SIZEOF_PES_PACKET_DATA sizeof(struct PES_packet_data)

// ------------------------------------------------------------
// An expandable list of PID vs. PES packet data
struct peslist {
	uint32_t *pid;           // An array of the PIDs
	PES_packet_data_p *data; // An array of the corresponding PES data
	int length;              // How many there are
	int size;                // How big the arrays are
};
typedef struct peslist *peslist_p;
#define SIZEOF_PESLIST sizeof(struct peslist)

#define PESLIST_START_SIZE 2 // Guess at one audio, one video
#define PESLIST_INCREMENT 1  // And a very conservative extension policy

// ------------------------------------------------------------
// A PES "reader" datastructure is the interface through which one reads
// PES packets from a TS or PS file
struct PES_reader {
	int is_TS; // Is it is TS (as opposed to PS)?

	// If it is TS, we read via a TS read-ahead buffer
	TS_reader_p tsreader;
	// If it is PS, we read via a PS read-ahead buffer
	PS_reader_p psreader;

	int give_info;    // Should information messages be output?
	int give_warning; // Should warning messages be output (to stderr)?

	PES_packet_data_p packet; // The current PES packet

	// When reading PS packets, `posn` is the position of the current (or last)
	// PS or TS packet.
	offset_t posn;

	// For PS data, we need to know if it is H.264 (MPEG-4/AVC) or not
	int is_h264;    // for backwards compatibility
	int video_type; // the actual (believed) video type

	// For PS and TS, we can choose to ignore audio entirely
	int video_only;
	// For PS, if we're not ignoring audio, we either look for a specific
	// audio stream id (specified by the user), or we will take the first
	// we find that is not Dolby. This latter is indicated by audio_stream
	// being set to 0
	byte audio_stream_id; // If not, the stream id of the audio we want

	// When reading TS data, we need the program information to make sense
	// of what is going on
	int got_program_data; // Do we know our program data yet?
	pmt_p program_map;    // The content of the (current/last) PMT
	// And from that, we can work out our video and audio (if any) pids, etc.
	uint32_t video_pid;      // Zero if not yet known
	uint32_t audio_pid;      // Ditto
	uint32_t pcr_pid;        // A copy of the value from the PMT
	uint16_t program_number; // Which program are we reading? (0=first)
	uint32_t pmt_pid;        // What's the PMT PID?

	// PMTs may be split over several TS packets, so we need a buffer
	// to build them in
	byte *pmt_data;    // The buffer (NULL when not in use)
	int pmt_data_len;  // The buffers length = the PMT section length + 3
	int pmt_data_used; // How much of said data we've already got

	// In order to write out TS data, we also need program information.
	// Obviously, the simplest case is when reading TS and writing it out
	// again, with the same settings. However, we also have to cope with
	// reading in PS data (which has no TS program information), and writing
	// out TS data with *different* program information.
	// If we're reading TS data, the default is to use the program data we
	// find therein. If `override_program_data` is TRUE, then we ignore that,
	// and use the values given by the user instead.
	int override_program_data;
	// Regardless, the following are the values to use when writing TS data out:
	uint32_t output_video_pid;
	uint32_t output_audio_pid;
	uint32_t output_pcr_pid;
	uint16_t output_program_number;
	uint32_t output_pmt_pid;

	// If we're reading Dolby (AC-3) audio, then there are two choices for the
	// stream type. DVB uses stream type 0x06, and ATSC uses stream type 0x81.
	byte dolby_stream_type; // The Dolby stream type we read (if any)
	byte output_dolby_stream_type;
	int override_dolby_stream_type; // Override whatever we read

	// Before we can write out TS data, we need some basic program information.
	// This is read in automatically if the input is TS, and must be supplied
	// by the user (via set_PES_reader_program_data) if the input is PS.

	// When reading a TS file, more than one PES packet may be being built
	// at the same time. At any time, the "next" read PES packet will be the
	// first one to be completely read in
	peslist_p packets; // The packets currently being read

	// If we are reading TS, and a PES packet has a declared length of 0,
	// then it can only be ended by the *next* PES packet of the same PID
	// (or by EOF, of course). In this case, we want to return the newly
	// ended PES packet, and the *next* read request should continue
	// with the PES packet we hadn't yet finished with. However, it is
	// technically possible (although unlikely) that the new (just started)
	// PES packet will end in its first TS packet. In that case, we want
	// to return *it* next time we try to read a TS packet. To facilitate
	// that, we can remember it here...
	PES_packet_data_p deferred;

	// If we ended such a packet on EOF, it's moderately convenient to
	// remember that we had found EOF, rather than try to bump into it again
	int had_eof;

	// When being used by a server, we want PES packets to be written out
	// as a "side effect" of reading them in to analyse their contents.
	// Thus we provide:
	int write_PES_packets; // TRUE if to write them out to:
	TS_writer_p tswriter;  // this TS writer context
	int program_freq;      // how often to write PAT/PMT out
	int program_index;     // how long since we last did so
	// Sometimes, for instance when going from fast forwards to normal playing,
	// we've already output the (end of) the current PES packet by hand, and
	// thus don't want the automated server mechanism to output it for us.
	// It's thus useful to have a flag indicating this (which will be unset
	// as soon as the current PES packet has, indeed, not been written out)
	// Since this is (definitely) an internal detail, it must be set explicitly.
	int dont_write_current_packet;
	// For benchmarking purposes (of the recipient), it can be useful to be able
	// to "pad out" the data we're sending, so that it is <n> times as big. If
	// ``expand`` is greater than 0, then ``expand`` "dummy" PES packets (of the
	// same size as the real one) will be output for each real PES packet (but
	// with an irrelevant stream id).
	int pes_padding;

	// If the original data is TS, and we want to send *all* of said data
	// to the server, it is sensible to write the *TS packets* as a side
	// effect of reading, rather than the PES packets. Thus we also have
	int write_TS_packets;
	// Obviously, one assumes that we are not going to be doing both at
	// the same time (since they write through the same tswriter interface)

	// In either case, sometimes it is useful to suppress writing packets
	// out for a while
	int suppress_writing;

	// Debugging: if this is set, and the appropriate code is compiled into
	// pes.c (see DEBUG_READ_PACKETS), then report on each PES packet read
	// and written. Even if DEBUG_READ_PACKETS is not defined, some output
	// will be produced.
	int debug_read_packets;
};
typedef struct PES_reader *PES_reader_p;
#define SIZEOF_PES_READER sizeof(struct PES_reader)

// Given the PES packet data (i.e., the data starting 00 00 01 <stream_id>
// <packet_length>), decide if this PES packet is MPEG-1 (11172-1) or
// H.222.0 (13818-1)
#define IS_H222_PES(data) ((data[6] & 0xC0) == 0x80)

#endif // _pes_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

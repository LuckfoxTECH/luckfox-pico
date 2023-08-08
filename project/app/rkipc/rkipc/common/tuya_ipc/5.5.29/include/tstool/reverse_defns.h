/*
 * Support for reversing
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
#ifndef _reverse_defns
#define _reverse_defns

#include "compat.h"
#include "es_defns.h"

// Since reverse_data refers to h262 and acces_unit datastructures, and
// *they* refer to reverse_data, we need to break the circular referencing
// at some point
typedef struct reverse_data *reverse_data_p;
#include "accessunit_defns.h"
#include "h262_defns.h"

// ------------------------------------------------------------
// As the software progresses through the data stream forwards, it remembers
// the location, size and details for frames that it might want to output in
// reverse
struct reverse_data {
	int is_h264; // Dealing with H.264 or H.262 data?

	// To do anything useful, we must then be linked to one or the other
	// type of input context (a union would save 4 bytes, but it doesn't
	// seem worth the bother at the moment).
	h262_context_p h262;
	access_unit_context_p h264;

	// Information for managing our arrays. `use_seq_offset` will be TRUE
	// for H.262 data, and FALSE for H.264 (MPEG-4/AVC)
	int length;            // Number of items in our arrays
	int size;              // How big our arrays are
	uint32_t num_pictures; // How many pictures we have
	// Four useful arrays (although the last is not used for H.264 data)
	uint32_t *index;      // Which picture this is, counted from the start
	offset_t *start_file; // The start offset of an item in the input file
	int32_t *start_pkt;   // and then within the PES packet (if needed)
	int32_t *data_len;    // Its length in bytes

	byte *seq_offset; // For MPEG-2, the offset backwards in the arrays
	                  // to the nearest earlier sequence header, or 0
	                  // for a sequence header entry
	byte *afd_byte;   // For MPEG-2, the AFD byte current for the picture

	// @@@ To be added later: for H.264 it's useful to know if a particular
	//     entry is an IDR or not. Thus add a ``byte *`` value called something
	//     like `is_IDR`, and make it point to `seq_offset`, since that is
	//     not used for H.264 data.
	//     (as a precursor to this, the code in reverse.c already allocates
	//     and extends the seq_offset array whether it is H.262 or H.264 data)

	// Is our "counting" in `index` going to last long enough? Well, if
	// we assume (worst case) that every picture was remembered in our arrays,
	// then we would have 2**32-1 pictures. At (another worst case) 50 frames
	// per second, that gives us (2**32-1)/50 seconds, which is 85899345
	// seconds (rounded down), or 23860 hours. Which I think should be enough
	// (about two years - yes, that should be enough).

	// When a function (output_in_reverse_as_XX) is called to output
	// reversed data, the reversal can either start from a specific index,
	// or from the "default", which is the current index. To make this easier,
	// we need to remember the index of the last entry added (which may be
	// less than the length of the array, since we might have rewound, and
	// be adding pictures again). Note that this is undefined if `length`
	// is 0.
	//
	// (There's probably a better name for this, since it's really more
	// like the highwater mark for what we've played forwards since we last
	// started playing forwards again, but I can't think of anything startlingly
	// illustrative, so this name will stay for now.)
	uint32_t last_posn_added;

	// Do we want to output sequence headers or not, when reversing H.262?
	int output_sequence_headers;

	// If we're outputting TS packets, we need to know the PID and stream id
	// to use. We *could* pass that down to each reverse call, but it's easier
	// to set it once and for all.
	uint32_t pid;
	byte stream_id;

	// When a function (output_in_reverse_as_XX) is called to output
	// reversed data, some statistics are maintained by the call.
	// Despite saying "picture", these also apply to H.264 access units as well
	// (and in fact "frame" would be a better term).
	int pictures_written; // The number of pictures written out
	int pictures_kept;    // The number of *different* pictures written
	int first_written;    // Which picture was the first written out
	int last_written;     // Which picture was the last written out
	                      // (for both of the latter, the value is the index of said picture in
	                      // our arrays). Remember that if forwards action is to be made after
	                      // reversing, it is important to reset the picture index in the H.262
	                      // or access_unit context to the picture index of the last written
	                      // picture. This must be done by the caller.
};
#define SIZEOF_REVERSE_DATA sizeof(struct reverse_data)

#define REVERSE_ARRAY_START_SIZE 1000
#define REVERSE_ARRAY_INCREMENT_SIZE 500

#endif // _reverse_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

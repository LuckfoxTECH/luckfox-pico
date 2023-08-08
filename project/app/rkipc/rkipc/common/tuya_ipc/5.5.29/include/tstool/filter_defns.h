/*
 * Datastructures for filtering ES data ("fast forward") and writing to ES or
 * TS.
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

#ifndef _filter_defns
#define _filter_defns

#include "accessunit_defns.h"
#include "compat.h"
#include "es_defns.h"
#include "h262_defns.h"
#include "reverse_defns.h"

// Filtering comes in two varieties:
// - "stripping" means retaining just reference pictures. For H.262 this
//   means the I pictures (and maybe the P pictures), for H.264 this means
//   the IDR and I pictures (or maybe all reference pictures). This is simple
//   to do, but the speedup resulting is very dependant on the data.
// - "filtering" means attempting to keep frames as a particular frequency,
//   so, for instance, a frequency of 8 would mean trying to keep every 8th
//   frame, or a speedup of 8x. This is harder to do as it depends rather
//   crucially on the distribution of reference frames in the data.

// ------------------------------------------------------------
struct h262_filter_context {
	h262_context_p h262; // The H.262 stream we are reading from
	int filter;          // TRUE if filtering, FALSE if stripping
	int freq;            // Frequency of frames to try to keep if filtering
	int allref;          // Keep all I and P pictures if stripping?
	// (the name `allref` is used for compatibility with the H.264 filter
	// context - it's a little easier to have one name for both filters)

	// For any operation on H.262, we want:
	int pending_EOF; // next time a function is called, say we had EOF

	// When filtering, we want:
	int count; // a rolling count to compare with the desired frequency
	int last_was_slice;
	int had_previous_picture;
	h262_picture_p last_seq_hdr;

	// When stripping, we want:
	int new_seq_hdr; // has the sequence header changed?

	int frames_seen;    // number of pictures seen this filter run
	int frames_written; // number of pictures written (or, returned)
};
typedef struct h262_filter_context *h262_filter_context_p;
#define SIZEOF_H262_FILTER_CONTEXT sizeof(struct h262_filter_context)

// ------------------------------------------------------------
struct h264_filter_context {
	access_unit_context_p access_unit_context; // our "reader" for access units
	int filter;                                // TRUE if filtering, FALSE if stripping
	int freq;                                  // Frequency of frames to try to keep if filtering
	int allref;                                // Keep all reference pictures

	// When filtering, we want:
	// a rolling count to compare with the desired frequency
	int count;
	// `skipped_ref_pic` is TRUE if we've skipped any reference pictures
	// since our last IDR.
	int skipped_ref_pic;
	// `last_accepted_was_not_IDR` is TRUE if the last frame kept (output)
	// was not an IDR. We set it TRUE initially so that we will decide
	// to output the first IDR we *do* find, regardless of the count.
	int last_accepted_was_not_IDR;
	int had_previous_access_unit;

	// Have we had an IDR in this run of the filter?
	int not_had_IDR;

	int frames_seen;    // number seen this filter run
	int frames_written; // number written (or, returned)
};
typedef struct h264_filter_context *h264_filter_context_p;
#define SIZEOF_H264_FILTER_CONTEXT sizeof(struct h264_filter_context)

#endif // _filter_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

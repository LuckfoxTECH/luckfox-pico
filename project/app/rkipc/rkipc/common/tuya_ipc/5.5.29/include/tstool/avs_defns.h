/*
 * Datastructures for reading AVS elementary streams.
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

#ifndef _avs_defns
#define _avs_defns

#include "compat.h"
#include "es_defns.h"
#include "ts_defns.h"
#include <stdio.h>

// Since reverse_data refers to avs and acces_unit datastructures, and
// *they* refer to reverse_data, we need to break the circular referencing
// at some point
typedef struct avs_context *avs_context_p;
#include "reverse_defns.h"

// ------------------------------------------------------------
// An AVS frame. This might be a picture or a sequence header (and
// its associated ES units).
struct _avs_frame {
	// The main thing we need is a list of the units that make up this picture
	ES_unit_list_p list;

	// An AVS "picture" might be a "proper" picture, a sequence header,
	// or (just) a sequence end item. It's useful to be able to identify
	// the two more common cases easily
	int is_frame;
	int is_sequence_header;

	// It's also useful to remember what the first ES unit is
	int start_code;

	// Data defined for a frame.
	byte picture_coding_type; // I, P or B (0, 1, 2)
	byte picture_distance;    // presentation order of P/B frames

	// Data defined for a sequence header
	byte aspect_ratio;    // 1=SAR/1.0 2=4/3, 3=16/9, 4=2.21/1 (?)
	byte frame_rate_code; // see Table 7-6
};
typedef struct _avs_frame *avs_frame_p;
#define SIZEOF_AVS_FRAME sizeof(struct _avs_frame)

// ------------------------------------------------------------
#define is_avs_slice_item(unit) ((unit)->start_code < 0xB0)
#define is_avs_frame_item(unit) ((unit)->start_code == 0xB3 || (unit)->start_code == 0xB6)
#define is_avs_user_data_item(unit) ((unit)->start_code == 0xB2)
#define is_avs_seq_header_item(unit) ((unit)->start_code == 0xB0)
#define is_avs_seq_end_item(unit) ((unit)->start_code == 0xB1)
#define is_avs_extension_start_item(unit) ((unit)->start_code == 0xB5)
#define is_avs_video_edit_item(unit) ((unit)->start_code == 0xB7)

#define avs_frame_rate(code)                                                                       \
	((code) == 1 ? 24000.0 / 1001 : /* 23.967... */                                                \
	     (code) == 2                                                                               \
	         ? 24                                                                                  \
	         : (code) == 3                                                                         \
	               ? 25                                                                            \
	               : (code) == 4 ? 30000.0 / 1001 : /* 29.97... */                                 \
	                     (code) == 5                                                               \
	                         ? 30                                                                  \
	                         : (code) == 6 ? 50                                                    \
	                                       : (code) == 7 ? 60000.0 / 1001 : /* 59.94... */         \
	                                             (code) == 8 ? 60 : 25) /* Hmm-really an error */

#define AVS_I_PICTURE_CODING 0 // our invention, but reasonable in context
#define AVS_P_PICTURE_CODING 1
#define AVS_B_PICTURE_CODING 2

// Note that "I" is made up by us (there is no picture coding on I frames)
#define AVS_PICTURE_CODING_STR(s)                                                                  \
	((s) == AVS_I_PICTURE_CODING                                                                   \
	     ? "I"                                                                                     \
	     : (s) == AVS_P_PICTURE_CODING ? "P" : (s) == AVS_B_PICTURE_CODING ? "B" : "Reserved")

// ------------------------------------------------------------
// Context for looping over the AVS items and pictures in an elementary
// stream
struct avs_context {
	ES_p es;

	// We count all of the frames as we read them (this is useful
	// when we are building up reverse_data arrays). If functions
	// move around in the data stream, we assume that they will
	// (re)set this to a sensible value.
	// The index of the first frame read is 1, and this value is
	// incremented by each call of `get_next_avs_frame` (note that
	// for this purpose, sequence headers are *not* considered frames)
	uint32_t frame_index; // The index of the last frame read

	// We detect the end of an AVS frame (or sequence header) by
	// reading the first item that cannot be part of it. We then need
	// to remember that item for *next* time we try to read a frame.
	ES_unit_p last_item;

	// If we are collecting reversing information, then we keep a reference
	// to the reverse data here
	reverse_data_p reverse_data;
	// In the same context, we need to remember how long it is since the
	// last sequence header
	byte count_since_seq_hdr;
};
#define SIZEOF_AVS_CONTEXT sizeof(struct avs_context)

#endif // _avs_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

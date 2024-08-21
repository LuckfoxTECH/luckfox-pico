/*
 * Datastructures for reading H.262 (MPEG-2) elementary streams.
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

#ifndef _h262_defns
#define _h262_defns

#include "compat.h"
#include "es_defns.h"
#include "ts_defns.h"
#include <stdio.h>

// Since reverse_data refers to h262 and acces_unit datastructures, and
// *they* refer to reverse_data, we need to break the circular referencing
// at some point
typedef struct h262_context *h262_context_p;
#include "reverse_defns.h"

// ------------------------------------------------------------
// An MPEG "item", the set of bytes that starts with a start code prefix.
struct _h262_item {
	struct ES_unit unit; // The actual data

	// MPEG2 specific data
	byte picture_coding_type; // only defined if unit.start_code == 0
};
typedef struct _h262_item *h262_item_p;
#define SIZEOF_H262_ITEM sizeof(struct _h262_item)

// ------------------------------------------------------------
// An H.262 "picture". This might be a picture or a sequence header (and
// its associated items).
struct _h262_picture {
	// The main thing we need is a list of the items that make up this picture
	ES_unit_list_p list;

	// An H.262 "picture" might be a "proper" picture, a sequence header,
	// or (just) a sequence end item. It's useful to be able to identify
	// the two more common cases easily
	int is_picture;
	int is_sequence_header;

	// Data defined for a picture. When a picture is composed of the data
	// from two fields, then these will be the values taken from the first
	// field "picture".
	byte picture_coding_type;    // I, P or B
	byte picture_structure;      // top/bottom field or frame
	uint16_t temporal_reference; // presentation order within a group
	byte afd;                    // its "Active Format Description" value
	                             // (NB: with 0xF0 bits set at top of byte)
	byte is_real_afd;            // was it a *real* AFD?
	int was_two_fields;          // TRUE if it's a frame merged from two fields

	// Data defined for a sequence header/extension
	// Note that H.262 requires that data given in one sequence extension
	// shall be the same as that in all the others in the stream. Thus, in
	// particular, we know that if fields are allowed by one sequence
	// extension, they will be allowed by all.
	byte progressive_sequence; // frames or frames and fields allowed?

	// Data defined for both
	// (in a frame, this is the value from the previous section header)
	byte aspect_ratio_info; // its aspect ratio code
};
typedef struct _h262_picture *h262_picture_p;
#define SIZEOF_H262_PICTURE sizeof(struct _h262_picture)

// ------------------------------------------------------------
// Produce a nice string for the start code. `b` must be a byte.
#define H262_START_CODE_STR(b)                                                                     \
	((b) == 0x00                                                                                   \
	     ? "picture"                                                                               \
	     : (b) >= 0x01 && b <= 0xAF                                                                \
	           ? "slice"                                                                           \
	           : (b) == 0xB0                                                                       \
	                 ? "reserved"                                                                  \
	                 : (b) == 0xB1                                                                 \
	                       ? "reserved"                                                            \
	                       : (b) == 0xB2                                                           \
	                             ? "user data"                                                     \
	                             : (b) == 0xB3                                                     \
	                                   ? "sequence header"                                         \
	                                   : (b) == 0xB4                                               \
	                                         ? "sequence error"                                    \
	                                         : (b) == 0xB5                                         \
	                                               ? "extension start"                             \
	                                               : (b) == 0xB6                                   \
	                                                     ? "reserved"                              \
	                                                     : (b) == 0xB7                             \
	                                                           ? "sequence end"                    \
	                                                           : (b) == 0xB8                       \
	                                                                 ? "group start"               \
	                                                                 : (b) >= 0xB9                 \
	                                                                       ? "system start"        \
	                                                                       : "???")

#define is_h262_picture_item(item) ((item)->unit.start_code == 0x00)
#define is_h262_slice_item(item)                                                                   \
	((item)->unit.start_code >= 0x01 && (item)->unit.start_code <= 0xAF)
#define is_h262_user_data_item(item) ((item)->unit.start_code == 0xB2)
#define is_h262_seq_header_item(item) ((item)->unit.start_code == 0xB3)
#define is_h262_seq_end_item(item) ((item)->unit.start_code == 0xB7)
#define is_h262_group_start_item(item) ((item)->unit.start_code == 0xB8)
#define is_h262_extension_start_item(item) ((item)->unit.start_code == 0xB5)

#define H262_PICTURE_CODING_STR(s)                                                                 \
	((s) == 0 ? "Forbidden"                                                                        \
	          : (s) == 1 ? "I" : (s) == 2 ? "P" : (s) == 3 ? "B" : (s) == 4 ? "D" : "Reserved")

// The following two macros can be used on H.262 items *or* pictures
#define is_I_picture(picture) ((picture)->picture_coding_type == 1)
#define is_P_picture(picture) ((picture)->picture_coding_type == 2)

#define H262_PICTURE_STRUCTURE_STR(s)                                                              \
	((s) == 0 ? "Reserved"                                                                         \
	          : (s) == 1 ? "Top Field" : (s) == 2 ? "Bottom Field" : (s) == 3 ? "Frame" : "???")

#define is_h262_field_picture(picture)                                                             \
	((picture)->is_picture &&                                                                      \
	 ((picture)->picture_structure == 1 || (picture)->picture_structure == 2))

#define is_h262_AFD_user_data_item(item)                                                           \
	((item)->unit.start_code == 0xB2 && (item)->unit.data_len > 8 &&                               \
	 (item)->unit.data[4] == 0x44 && (item)->unit.data[5] == 0x54 &&                               \
	 (item)->unit.data[6] == 0x47 && (item)->unit.data[7] == 0x31)

#define UNSET_AFD_BYTE 0xF8 // i.e., '1111 1000'

// String values taken from ATSC Digital Television Standard, Rev C,
// (A/53C) 12 May 2004, which will hopefully be correct for DVB as well...
#define AFD_STR(afd)                                                                               \
	((afd) == 0xF2                                                                                 \
	     ? "ATSC: box 16:9 (top)"                                                                  \
	     : (afd) == 0xF3                                                                           \
	           ? "ATSC: box 14:9 (top)"                                                            \
	           : (afd) == 0xF4                                                                     \
	                 ? "ATSC: box > 16:9 (center)"                                                 \
	                 : (afd) == 0xF8                                                               \
	                       ? "Active format as coded frame"                                        \
	                       : (afd) == 0xF9                                                         \
	                             ? "4:3 (centre)"                                                  \
	                             : (afd) == 0xFA                                                   \
	                                   ? "16:9 (centre)"                                           \
	                                   : (afd) == 0xFB                                             \
	                                         ? "14:9 (centre)"                                     \
	                                         : (afd) == 0xFC                                       \
	                                               ? "reserved"                                    \
	                                               : (afd) == 0xFD                                 \
	                                                     ? "4:3 (with shoot & protect 14:9 "       \
	                                                       "centre)"                               \
	                                                     : (afd) == 0xFE                           \
	                                                           ? "16:9 (with shoot & protect "     \
	                                                             "14:9 centre)"                    \
	                                                           : (afd) == 0xFF                     \
	                                                                 ? "16:9 (with shoot & "       \
	                                                                   "protect 4:3 centre)"       \
	                                                                 : "reserved")

#define SHORT_AFD_STR(afd)                                                                         \
	((afd) == 0xF2                                                                                 \
	     ? "ATSC: box 16:9 (top)"                                                                  \
	     : (afd) == 0xF3                                                                           \
	           ? "ATSC: box 14:9 (top)"                                                            \
	           : (afd) == 0xF4                                                                     \
	                 ? "ATSC: box > 16:9 (center)"                                                 \
	                 : (afd) == 0xF8                                                               \
	                       ? "As coded frame"                                                      \
	                       : (afd) == 0xF9                                                         \
	                             ? "4:3 (centre)"                                                  \
	                             : (afd) == 0xFA                                                   \
	                                   ? "16:9 (centre)"                                           \
	                                   : (afd) == 0xFB                                             \
	                                         ? "14:9 (centre)"                                     \
	                                         : (afd) == 0xFC                                       \
	                                               ? "reserved"                                    \
	                                               : (afd) == 0xFD                                 \
	                                                     ? "4:3 (14:9)"                            \
	                                                     : (afd) == 0xFE                           \
	                                                           ? "16:9 (14:9)"                     \
	                                                           : (afd) == 0xFF ? "16:9 (4:3)"      \
	                                                                           : "reserved")

// The standard does not define value FF, so I'm choosing it as an "unset"
// value, so that I get sensible diagnostics when a picture is reported
// on before a section header has been read
#define H262_UNSET_ASPECT_RATIO_INFO 0xFF
#define H262_ASPECT_RATIO_INFO_STR(rat)                                                            \
	((rat) == 0xFF                                                                                 \
	     ? "Unset"                                                                                 \
	     : (rat) == 0                                                                              \
	           ? "Forbidden aspect ratio"                                                          \
	           : (rat) == 1 ? "Square"                                                             \
	                        : (rat) == 2 ? "4:3" : (rat) == 3 ? "16:9" : "Reserved aspect ratio")

// ------------------------------------------------------------
// Context for looping over the H.262 items and pictures in an elementary
// stream
struct h262_context {
	ES_p es;

	// We count all of the pictures as we read them (this is useful
	// when we are building up reverse_data arrays). If functions
	// move around in the data stream, we assume that they will
	// (re)set this to a sensible value.
	// The index of the first picture read is 1, and this value is
	// incremented by each call of `get_next_h262_picture` (note that
	// for this purpose, sequence headers are *not* considered pictures)
	uint32_t picture_index; // The index of the last picture read

	// We detect the end of an H.262 picture (or sequence header) by
	// reading the first item that cannot be part of it. We then need
	// to remember that item for *next* time we try to read a picture.
	h262_item_p last_item;

	// What was the aspect ratio code from the last sequence header?
	byte last_aspect_ratio_info;

	// When we are reading MPEG-2, we may encounter AFD (Active Format
	// Definiton) user data. This sets the aspect ratio of the following
	// picture(s), possibly overriding information present in a preceding
	// sequence header. It should only be present for I pictures, really,
	// but we remember it for all pictures.
	// Once an AFD value has been set, it stands until another value is read.
	// This all becomes important when reversing, which see.
	// Anyway, in order to set the AFD on pictures which don't have it
	// explicitly, we obviously need to remember the last value found.
	// For simplicity of use, we remember the whole byte it came in,
	// including the 0xF0 reserved bits at the top of the byte.
	// Note that '1000' (8) is the "unset" value.
	byte last_afd;

	// When we are reading an H.262 picture back in, for reversing purposes, or
	// reading frames for filtering, it is useful to *insist* that an AFD be
	// found for the picture. Thus, if `add_fake_afd` is TRUE, a dummy AFD,
	// containing the value in `last_afd`, will be inserted into the picture's
	// ES unit `list` (of course, if the picture contains a *real* AFD, this is
	// not necessary). Beware that the ES unit inserted won't stand up to close
	// observation, as its start position (for instance) will clearly be
	// wrong...
	// (this is manipulated by the reversing and filtering code - it is not
	// intended for use for any other purpose)
	int add_fake_afd;

	// If we are collecting reversing information, then we keep a reference
	// to the reverse data here
	reverse_data_p reverse_data;
	// In the same context, we need to remember how long it is since the
	// last sequence header
	byte count_since_seq_hdr;
};
#define SIZEOF_H262_CONTEXT sizeof(struct h262_context)

#endif // _h262_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

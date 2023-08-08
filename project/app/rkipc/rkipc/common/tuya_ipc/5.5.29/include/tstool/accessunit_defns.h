/*
 * Datastructures for working with access units in H.264 elementary streams.
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

#ifndef _accessunit_defns
#define _accessunit_defns

#include "es_defns.h"
#include "nalunit_defns.h"

// Since reverse_data refers to h262 and acces_unit datastructures, and
// *they* refer to reverse_data, we need to break the circular referencing
// at some point
typedef struct access_unit_context *access_unit_context_p;
#include "reverse_defns.h"

// ------------------------------------------------------------
// A single access unit
struct access_unit {
	uint32_t index;              // The (notional) index of this unit in the stream
	                             // (i.e., from the context's access_unit_index)
	int started_primary_picture; // True if we have, indeed, done so
	nal_unit_p primary_start;    // First NAL unit of our primary picture
	                             // (a pointer into `nal_units`)

	// A "list" of the NAL units that form us
	// (the only reason we don't use an ES unit list is that I want to be
	// able to report on the content of an access unit in terms of the NAL
	// units that form it - perhaps more important in development than in
	// prodcution, but still)
	nal_unit_list_p nal_units;

	// Did we ignore any "broken" NAL units when we were being built?
	// (if so, there's a case to say we might be broken too)
	int ignored_broken_NAL_units;

	// Information derived from the slices in this access unit
	// (all slices in an access unit must have the same values for these,
	// so these could actually just be derived from the `primary_start`,
	// but it's slightly easier to have them more available, and saves
	// the need to check if `primary_start` is defined before using them).
	uint32_t frame_num;
	byte field_pic_flag;    // frame or field?
	byte bottom_field_flag; // for a field (only), bottom or top?
	                        // (After merging two field access units into a single frame,
	                        // `field_pic_flag` will be set to 0, to "pretend" that we have a
	                        // "proper" frame access unit)
};
typedef struct access_unit *access_unit_p;
#define SIZEOF_ACCESS_UNIT sizeof(struct access_unit)

// ------------------------------------------------------------
// Context for looping over the access units in an elementary stream
struct access_unit_context {
	// ---------------------------------------------------------------
	// Public information - things it makes sense for users to inspect
	// ---------------------------------------------------------------
	nal_unit_context_p nac; // short and nasty "mnemonic"

	// If we read an EndOfStream or EndOfSequence NAL unit, then we
	// want to remember as much. We don't really want to put it into
	// the preceding access unit (for a start, that would mess up
	// reversing the stream), but we also don't want to forget it,
	// and in the case of EndOfStream, we don't want to carry on
	// reading the ES after finding it. The simplest thing is just
	// to remember them.
	nal_unit_p end_of_sequence;
	nal_unit_p end_of_stream;

	// We count all of the access units as we read them (this is useful
	// when we are building up reverse_data arrays). If functions
	// move around in the data stream, we assume that they will
	// (re)set this to a sensible value.
	// The index of the first access unit read is 1, and this value is
	// incremented by each call of `get_next_access_unit`
	uint32_t access_unit_index; // The index of the last access unit read

	// If we are collecting reversing information, then we keep a reference
	// to the reverse data here
	reverse_data_p reverse_data;

	// -------------------------------------------------------------
	// Private information - used internally by the software, not to
	// be relied upon by outsiders
	// -------------------------------------------------------------
	// If we ended the previous access unit because of finding a NAL
	// unit that provokes a *new* access unit, then we remember it
	// (after all, we'll want to put it into the new access unit)
	nal_unit_p pending_nal;
	// We need to remember the VCL NAL unit that started the previous
	// primary picture, so that we can compare a later VCL NAL unit
	// to it, to see if we've got a new primary picture starting
	// (actually, we'll only remember a "stub" of information for it)
	nal_unit_p earlier_primary_start;
	// Some items go "in front of" the next VCL NAL unit, so we need
	// a list of such
	nal_unit_list_p pending_list;
	// If we read an end of stream NAL unit, then next time we try
	// to read an access unit, we want to know that there is no point.
	// Similarly, if we read EOF on the input stream.
	byte no_more_data;
};
#define SIZEOF_ACCESS_UNIT_CONTEXT sizeof(struct access_unit_context)

#endif // _accessunit_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

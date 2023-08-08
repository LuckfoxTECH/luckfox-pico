/*
 * Functions for working with access units in H.264 elementary streams.
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

#ifndef _accessunit_fns
#define _accessunit_fns

#include "accessunit_defns.h"

/*
 * Build a new access unit context datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_access_unit_context(ES_p es, access_unit_context_p *context);

/*
 * Free a new access unit context datastructure.
 *
 * Clears the datastructure, frees it, and returns `context` as NULL.
 *
 * Does not free any `reverse_data` datastructure.
 *
 * Does nothing if `context` is already NULL.
 */
extern void free_access_unit_context(access_unit_context_p *context);

/*
 * Reset an acccess unit context, so it "forgets" its current information
 * about what it is reading, etc.
 */
extern void reset_access_unit_context(access_unit_context_p context);

/*
 * Rewind a file being read as access units.
 *
 * This is a wrapper for `rewind_nal_unit_context` that also knows to
 * unset things appropriate to the access unit context.
 *
 * If a reverse context is attached to this access unit, it also will
 * be "rewound" appropriately.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int rewind_access_unit_context(access_unit_context_p context);

/*
 * Tidy up and free an access unit datastructure after we've finished with it.
 *
 * Clears the datastructure, frees it, and returns `acc_unit` as NULL.
 *
 * Does nothing if `acc_unit` is already NULL.
 */
extern void free_access_unit(access_unit_p *acc_unit);

/*
 * Report on this access unit, on the given output stream.
 */
extern void report_access_unit(FILE *stream, access_unit_p access_unit);

/*
 * Retrieve the bounds of this access unit in the file it was read from.
 *
 * - `access_unit` is the access unit we're interested in
 * - `start` is its start position (i.e., the location at which to start
 *   reading to retrieve all of the data for the access unit, including
 *   the 00 00 01 prefix at the start of the first NAL unit therein)
 * - `length` is the total length of the NAL units within this access unit
 *
 * Returns 0 if all goes well, 1 if the access unit has no content.
 */
extern int get_access_unit_bounds(access_unit_p access_unit, ES_offset *start, uint32_t *length);

/*
 * Are all slices in this access unit I slices?
 */
extern int all_slices_I(access_unit_p access_unit);

/*
 * Are all slices in this access unit P slices?
 */
extern int all_slices_P(access_unit_p access_unit);

/*
 * Are all slices in this access unit I or P slices?
 */
extern int all_slices_I_or_P(access_unit_p access_unit);

/*
 * Are all slices in this access unit B slices?
 */
extern int all_slices_B(access_unit_p access_unit);

/*
 * Write out an access unit as ES.
 *
 * Also writes out any end of sequence or end of stream NAL unit found in the
 * `context` (since they are assumed to have immediately followed this access
 * unit).
 *
 * - `access_unit` is the access unit to write out
 * - `context` may contain additional things to write (see above), but may
 *   legitimately be NULL if there is no context.
 * - `output` is the ES file to write to
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_access_unit_as_ES(access_unit_p access_unit, access_unit_context_p context,
                                   FILE *output);
/*
 * Write out an access unit as TS.
 *
 * Also writes out any end of sequence or end of stream NAL unit found in the
 * `context` (since they are assumed to have immediately followed this access
 * unit).
 *
 * - `access_unit` is the access unit to write out
 * - `context` may contain additional things to write (see above), but may
 *   legitimately be NULL if there is no context.
 * - `tswriter` is the TS context to write with
 * - `video_pid` is the PID to use to write the data
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_access_unit_as_TS(access_unit_p access_unit, access_unit_context_p context,
                                   TS_writer_p tswriter, uint32_t video_pid);
/*
 * Write out an access unit as TS, with PTS timing in the first PES packet
 * (and PCR timing in the first TS of the frame).
 *
 * Also writes out any end of sequence or end of stream NAL unit found in the
 * `context` (since they are assumed to have immediately followed this access
 * unit).
 *
 * - `access_unit` is the access unit to write out
 * - `context` may contain additional things to write (see above), but may
 *   legitimately be NULL if there is no context.
 * - `tswriter` is the TS context to write with
 * - `video_pid` is the PID to use to write the data
 * - `pts` is the PTS time (which is also used as the PCR base).
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_access_unit_as_TS_with_pts_dts(access_unit_p access_unit,
                                                access_unit_context_p context, TS_writer_p tswriter,
                                                uint32_t video_pid, int got_pts, uint64_t pts,
                                                int got_dts, uint64_t dts);
/*
 * Write out an access unit as TS, with PCR timing in the first TS of the
 * frame.
 *
 * Also writes out any end of sequence or end of stream NAL unit found in the
 * `context` (since they are assumed to have immediately followed this access
 * unit).
 *
 * - `access_unit` is the access unit to write out
 * - `context` may contain additional things to write (see above), but may
 *   legitimately be NULL if there is no context.
 * - `tswriter` is the TS context to write with
 * - `video_pid` is the PID to use to write the data
 * - `pcr_base` and `pcr_extn` encode the PCR value.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_access_unit_as_TS_with_PCR(access_unit_p access_unit,
                                            access_unit_context_p context, TS_writer_p tswriter,
                                            uint32_t video_pid, uint64_t pcr_base,
                                            uint32_t pcr_extn);
/*
 * Retrieve the next access unit from the given elementary stream.
 *
 * - `context` is the context information needed to allow us to find
 *   successive access units.
 * - `quiet` is true if we should try to be silent about it
 * - `show_details` is true if we should output more info than normal
 * - `ret_access_unit` is the next access unit.
 *
 * If the access unit was ended because an end of sequence or end of
 * stream NAL unit was encountered, then said end of sequence/stream
 * NAL unit will be remembered in the `context`.
 *
 * Note that it is possible to get back an *empty* access unit in
 * certain situations - the most obvious of which is if we get two
 * ``end of sequence`` NAL units with nothing betwen them.
 *
 * Because of this possibility, some care should be taken to allow for
 * access units that do not contain a primary picture (no VCL NAL unit),
 * and contain zero NAL units. Also, if one is trying for an accurate
 * count of access units, such instances should probably be ignored.
 *
 * Returns 0 if it succeeds, EOF if there is no more data to read, or 1 if
 * some error occurs.
 *
 * EOF can be returned because the end of file has been reached, or because an
 * end of stream NAL unit has been encountered. The two may be distinguished
 * by looking at `context->end_of_stream`, which will be NULL if it was a true
 * EOF.
 *
 * Note that `ret_access_unit` will be NULL if EOF is returned.
 */
extern int get_next_access_unit(access_unit_context_p context, int quiet, int show_details,
                                access_unit_p *ret_access_unit);
/*
 * Retrieve the next H.264 frame from the given elementary stream.
 *
 * The next access unit is retrieved from the input stream (using
 * get_next_access_unit).
 *
 * If that access unit represents a frame, it is returned.
 *
 * If it represents a field, then the *following* access unit is retrieved,
 * and if that is the second field of its frame, it is merged into the first,
 * and the resultant frame is returned.
 *
 * If a field with frame number A is followed by a field with frame number B,
 * it is assumed that synchronisation has been lost. In this case, the first
 * field (frame A) will be discarded, and an attempt made to read the second
 * field of frame B.
 *
 * Similarly, if a frame is found instead of the second field, the first
 * field will be discarded and the frame returned.
 *
 *   Note that if the context is associated with a reverse context,
 *   then appropriate frames will automatically be remembered therein.
 *
 * - `context` is the context information needed to allow us to find
 *   successive access units.
 * - `quiet` is true if we should try to be silent about it
 * - `show_details` is true if we should output more info than normal
 * - `frame` is an access unit datastructure representing the next
 *   frame.
 *
 * If the access unit was ended because an end of sequence or end of
 * stream NAL unit was encountered, then said end of sequence/stream
 * NAL unit will be remembered in the `context`.
 *
 * Returns 0 if it succeeds, EOF if there is no more data to read, or 1 if
 * some error occurs.
 *
 * EOF can be returned because the end of file has been reached, or because an
 * end of stream NAL unit has been encountered. The two may be distinguished
 * by looking at `context->end_of_stream`, which will be NULL if it was a true
 * EOF.
 *
 * Note that `ret_access_unit` will be NULL if EOF is returned.
 */
extern int get_next_h264_frame(access_unit_context_p context, int quiet, int show_details,
                               access_unit_p *frame);
/*
 * If this access unit was read from PES, did any of its PES packets contain
 * a PTS?
 *
 * Returns TRUE if so, FALSE if not.
 */
extern int access_unit_has_PTS(access_unit_p access_unit);

#endif // _accessunit_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

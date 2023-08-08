/*
 * Prototypes for reading AVS elementary streams.
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

#ifndef _avs_fns
#define _avs_fns

#include "avs_defns.h"

/*
 * Return a string representing the start code
 */
extern const char *avs_start_code_str(byte start_code);
/*
 * Print out information derived from the start code, to the given stream.
 */
extern void print_avs_start_code_str(FILE *stream, byte start_code);
/*
 * Determine the picture coding type of an AVS ES unit
 *
 * P/B frames are distinguished by their picture coding types. For I frames,
 * we make one up...
 *
 * Returns an appropriate value (0 if none suitable)
 */
extern int avs_picture_coding_type(ES_unit_p unit);
/*
 * Build a new AVS frame reading context.
 *
 * This acts as a "jacket" around the ES context, and is used when reading
 * AVS frames with get_next_avs_frame(). It "remembers" the last
 * item read, which is the first item that was not part of the frame.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_avs_context(ES_p es, avs_context_p *context);
/*
 * Free an AVS frame reading context.
 *
 * Clears the datastructure, frees it, and returns `context` as NULL.
 *
 * Does not free any `reverse_data` datastructure.
 *
 * Does nothing if `context` is already NULL.
 */
extern void free_avs_context(avs_context_p *context);
/*
 * Rewind a file being read as AVS frames
 *
 * This is a wrapper for `seek_ES` that also knows to unset things appropriate
 * to the AVS frame reading context.
 *
 * If a reverse context is attached to this context, it also will
 * be "rewound" appropriately.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int rewind_avs_context(avs_context_p context);
/*
 * Free an AVS "frame".
 *
 * Clears the datastructure, frees it, and returns `frame` as NULL.
 *
 * Does nothing if `frame` is already NULL.
 */
extern void free_avs_frame(avs_frame_p *frame);
/*
 * Retrieve the the next AVS "frame".
 *
 * The AVS "frame" returned can be one of:
 *
 * 1. A frame, including its slices.
 * 2. A sequence header, including its sequence extension, if any.
 * 3. A sequence end.
 *
 * Specifically, the next AVS "frame" is retrieved from the input stream.
 *
 * If that "frame" represents a sequence header or a frame, it is returned.
 *
 *   Note that if the context is associated with a reverse context,
 *   then appropriate frames/sequence headers will automatically be
 *   remembered therein.
 *
 * - `context` is the AVS frame reading context.
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `frame` is the AVS "frame", containing a frame, a sequence header or a
 *   sequence end
 *
 * Returns 0 if it succeeds, EOF if we reach the end of file, or 1 if some
 * error occurs.
 */
extern int get_next_avs_frame(avs_context_p context, int verbose, int quiet, avs_frame_p *frame);
/*
 * Write out an AVS frame as TS
 *
 * - `tswriter` is TS the output stream
 * - `frame` is the frame to write out
 * - `pid` is the PID to use for the TS packets
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_avs_frame_as_TS(TS_writer_p tswriter, avs_frame_p frame, uint32_t pid);
/*
 * Write out an AVS frame as TS, with PTS timing in the first PES packet
 * (and PCR timing in the first TS of the frame).
 *
 * - `frame` is the frame to write out
 * - `tswriter` is the TS context to write with
 * - `video_pid` is the PID to use to write the data
 * - `got_pts` is TRUE if we have a PTS value, in which case
 * - `pts` is said PTS value
 * - `got_dts` is TRUE if we also have DTS, in which case
 * - `dts` is said DTS value.
 *
 * If we are given a DTS (which must, by definition, always go up) we will also
 * use it as the value for PCR.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_avs_frame_as_TS_with_pts_dts(avs_frame_p frame, TS_writer_p tswriter,
                                              uint32_t video_pid, int got_pts, uint64_t pts,
                                              int got_dts, uint64_t dts);
/*
 * Write out an AVS frame as TS, with PCR timing in the first TS of the
 * frame.
 *
 * - `frame` is the frame to write out
 * - `tswriter` is the TS context to write with
 * - `video_pid` is the PID to use to write the data
 * - `pcr_base` and `pcr_extn` encode the PCR value.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_avs_frame_as_TS_with_PCR(avs_frame_p frame, TS_writer_p tswriter,
                                          uint32_t video_pid, uint64_t pcr_base, uint32_t pcr_extn);
/*
 * Write out a frame (as stored in an ES unit list) as ES
 *
 * - `output` is the ES output file
 * - `frame` is the frame to write out
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_avs_frame_as_ES(FILE *output, avs_frame_p frame);
/*
 * Report on an AVS frame's contents.
 *
 * - `stream` is where to write the information
 * - `frame` is the frame to report on
 * - if `report_data`, then the component ES units will be printed out as well
 */
extern void report_avs_frame(FILE *stream, avs_frame_p frame, int report_data);

#endif // _avs_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

/*
 * Functions for filtering ES data ("fast forward") and writing to ES or TS.
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

#ifndef _filter_fns
#define _filter_fns

#include "filter_defns.h"

/*
 * Build a new filter context for "stripping" H.262 data
 *
 * - `fcontext` is the new filter context
 * - `h262` is the H.262 stream to read from
 * - `all_IP` is true if the software should keep all I and P pictures
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int build_h262_filter_context_strip(h262_filter_context_p *fcontext, h262_context_p h262,
                                           int all_IP);
/*
 * Build a new filter context for "filtering" H.262 data
 *
 * - `fcontext` is the new filter context
 * - `h262` is the H.262 stream to read from
 * - `freq` is the desired speed-up, or the frequency at which frames
 *   should (ideally) be kept
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int build_h262_filter_context(h262_filter_context_p *fcontext, h262_context_p h262,
                                     int freq);
/*
 * Reset an H.262 filter context, ready to start filtering anew.
 */
extern void reset_h262_filter_context(h262_filter_context_p fcontext);
/*
 * Free a filter context
 *
 * NOTE that this does *not* free the H.262 datastructure to which the
 * filter context refers.
 *
 * - `fcontext` is the filter context, which will be freed, and returned
 *   as NULL.
 */
extern void free_h262_filter_context(h262_filter_context_p *fcontext);
/*
 * Build a new filter context for "stripping" ES data
 *
 * - `fcontext` is the new filter context
 * - `access` is the access unit context to read from
 * - `allref` is true if the software should keep all reference pictures
 *   (H.264) or all I and P pictures (H.264)
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int build_h264_filter_context_strip(h264_filter_context_p *fcontext,
                                           access_unit_context_p access, int allref);
/*
 * Build a new filter context for "filtering" ES data
 *
 * - `fcontext` is the new filter context
 * - `access` is the access unit context to read from
 * - `freq` is the desired speed-up, or the frequency at which frames
 *   should (ideally) be kept
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int build_h264_filter_context(h264_filter_context_p *fcontext, access_unit_context_p access,
                                     int freq);
/*
 * Reset an H.264 filter context, ready to start filtering anew.
 */
extern void reset_h264_filter_context(h264_filter_context_p fcontext);
/*
 * Free an H.264 filter context
 *
 * NOTE that this does *not* free the access unit context to which the
 * filter context refers.
 *
 * - `fcontext` is the filter context, which will be freed, and returned
 *   as NULL.
 */
extern void free_h264_filter_context(h264_filter_context_p *fcontext);

/*
 * Retrieve the next I (and/or, if fcontext->allref, P) frame in this H.262 ES.
 *
 * Any sequence end "pictures" will be ignored.
 *
 * Note that the ES data being read should be video-only.
 *
 * - `fcontext` is the information that tells us what to filter and how
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 *
 * - `seq_hdr` is a sequence header, i.e., that used by the next frame to
 *   output. This will be NULL if the sequence header has not changed since
 *   the last call of this function.
 *
 *   Note that the caller should *not* free this, and that it will not be
 *   maintained over calls of this function (i.e., it is a reference to a
 *   value within the `fcontext` which is altered by this function).
 *
 * - `frame` is the next frame to output.
 *
 *   Note that it is the caller's responsibility to free this with
 *   `free_h262_picture()`.
 *
 *   If an error or EOF is returned, this value is undefined.
 *
 * - `frames_seen` is the number of I and P frames (start code 0)
 *   found by this call of the function, including the item returned
 *   if appropriate.
 *
 * Returns 0 if it succeeds, EOF if end-of-file is read (or the last call
 * returned a sequence end item), 1 if some error occurs.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int get_next_stripped_h262_frame(h262_filter_context_p fcontext, int verbose, int quiet,
                                        h262_picture_p *seq_hdr, h262_picture_p *frame,
                                        int *frames_seen);
/*
 * Retrieve the next I frame, from the H.262 ES, aiming for an "apparent" kept
 * frequency as stated.
 *
 * Any sequence end "pictures" will be ignored.
 *
 * Note that the ES data being read should be video-only.
 *
 * - `fcontext` is the information that tells us what to filter and how
 *   (including the desired frequency)
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 *
 * - `seq_hdr` is a sequence header, i.e., that used by the next picture to
 *   output. This will be NULL if `frame` is NULL.
 *
 *   Note that the caller should *not* free this, and that it will not be
 *   maintained over calls of this function (i.e., it is a reference to a
 *   value within the `fcontext` which is altered by this function).
 *
 * - `frame` is the next frame to output. This will be NULL if the last frame
 *   should be output again, to provide the requested apparent frequency.
 *
 *   Note that it is the caller's responsibility to free this with
 *   `free_h262_picture()`.
 *
 *   If an error or EOF is returned, this value is undefined.
 *
 * - `frames_seen` is the number of I and P frames found by this call of
 *   the function, including the item returned if appropriate.
 *
 * Returns 0 if it succeeds, EOF if end-of-file is read, or we've just read a
 * sequence end item, or the last call ended a picture on a sequence end
 * item, 1 if some error occurs.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int get_next_filtered_h262_frame(h262_filter_context_p fcontext, int verbose, int quiet,
                                        h262_picture_p *seq_hdr, h262_picture_p *frame,
                                        int *frames_seen);
/*
 * Return the next IDR or I (and maybe any reference) frame from this H.264 ES.
 *
 * Note that the ES data being read should be video-only.
 *
 * - `fcontext` is the information that tells us what to filter and how
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `frame` is the next frame to output.
 *   Note that it is the caller's responsibility to free this with
 *   `free_access_unit()`.
 *   If an error or EOF is returned, this value is undefined.
 * - `frames_seen` is the number of frames found by this call
 *   of the function, including the frame returned.
 *
 * Returns 0 if it succeeds, EOF if end-of-file is read (or an an end of
 * stream NAL unit has been passed), 1 if some error occurs.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int get_next_stripped_h264_frame(h264_filter_context_p fcontext, int verbose, int quiet,
                                        access_unit_p *frame, int *frames_seen);
/*
 * Retrieve the next frame from the H.264 (MPEG-4/AVC) ES, aiming
 * for an "apparent" kept frequency as stated.
 *
 * Note that the ES data being read should be video-only.
 *
 * - `fcontext` is the information that tells us what to filter and how
 *   (including the desired frequency)
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 *
 * - `frame` is the next frame to output.
 *
 *   If the function succeeds and `frame` is NULL, it means that the
 *   last frame should be output again.
 *
 *   Note that it is the caller's responsibility to free this frame with
 *   `free_access_unit()`.
 *
 *   If an error or EOF is returned, this value is undefined.
 *
 * - `frames_seen` is the number of frames found by this call of the function,
 *   including the frame returned.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int get_next_filtered_h264_frame(h264_filter_context_p fcontext, int verbose, int quiet,
                                        access_unit_p *frame, int *frames_seen);

#endif // _filter_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

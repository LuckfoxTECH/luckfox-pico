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
#ifndef _reverse_fns
#define _reverse_fns

#include "accessunit_defns.h"
#include "h262_defns.h"
#include "reverse_defns.h"

/*
 * Build the internal arrays to remember video sequence bounds in,
 * for reversing.
 *
 * Builds a new `reverse_data` datastructure. If `is_h264` is FALSE (i.e., the
 * data to be reversed is not MPEG-1 or MPEG-2), then this datastructure may
 * be smaller.
 *
 * To collect reversing data, attach this datastructure to an H.262 or access
 * unit context (with add_h262/access_unit_reverse_context), and then use
 * get_next_h262_frame() or get_next_h264_frame() to read through the data
 * stream - appropriate pictures/access units will be remembered
 * automatically.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_reverse_data(reverse_data_p *reverse_data, int is_h264);
/*
 * Set the video PID and stream id for TS output.
 *
 * This need only be called if reverse data *is* being output as TS,
 * and if the standard default values (DEFAULT_VIDEO_PID and
 * DEFAULT_VIDEO_STREAM_ID) are not correct.
 */
extern void set_reverse_pid(reverse_data_p reverse_data, uint32_t pid, byte stream_id);
/*
 * Add a reversing context to an H.262 context (and vice versa).
 *
 * Does not check if there is one present already.
 *
 * Returns 0 if all is well, 1 if something goes wrong.
 */
extern int add_h262_reverse_context(h262_context_p h262, reverse_data_p reverse_data);
/*
 * Add a reversing context to an access unit context (and vice versa).
 *
 * Does not check if there is one present already.
 *
 * Returns 0 if all is well, 1 if something goes wrong.
 */
extern int add_access_unit_reverse_context(access_unit_context_p context,
                                           reverse_data_p reverse_data);

/*
 * Free the datastructure we used to remember reversing data
 *
 * Sets `reverse_data` to NULL.
 */
extern void free_reverse_data(reverse_data_p *reverse_data);
/*
 * Remember video sequence bounds for H.262 data
 *
 * - `reverse_data` is the datastructure we want to add our entry to
 * - `index` indicates which picture (counted from the start of the file)
 *   this one is (i.e., we're assuming that not all pictures will be stored).
 *   If the entry is an H.262 sequence header, then this is ignored.
 * - `start_posn` is the location of the start of the entry in the file,
 *   The entry will be ignored if `start_posn` comes before the last
 *   existing entry in the arrays.
 * - `length` is the number of bytes in the entry
 * - in H.262 data, `seq_offset` should be 0 for a sequence header, and is
 *   otherwise the offset backwards to the previous nearest sequence header
 *   (i.e., 1 if the sequence header is the previous entry).
 * - `afd` is the effective AFD byte for this picture
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int remember_reverse_h262_data(reverse_data_p reverse_data, uint32_t index,
                                      ES_offset start_posn, uint32_t length, byte seq_offset,
                                      byte afd);
/*
 * Remember video sequence bounds for H.264 data
 *
 * - `reverse_data` is the datastructure we want to add our entry to
 * - `index` indicates which picture (counted from the start of the file)
 *   this one is (i.e., we're assuming that not all pictures will be stored).
 *   If the entry is an H.262 sequence header, then this is ignored.
 * - `start_posn` is the location of the start of the entry in the file,
 *   The entry will be ignored if `start_posn` comes before the last
 *   existing entry in the arrays.
 * - `length` is the number of bytes in the entry
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int remember_reverse_h264_data(reverse_data_p reverse_data, uint32_t index,
                                      ES_offset start_posn, uint32_t length);
/*
 * Retrieve video sequence bounds for entry `which`
 *
 * - `reverse_data` is the datastructure we want to get our entry from
 * - `which` indicates which entry we'd like to retrieve. The first
 *   entry in the `reverse_data` is number 0.
 * - `index` indicates which picture (counted from the start of the file)
 *   this one is (i.e., we're assuming that not all pictures will be stored).
 *   `index` may be passed as NULL if the value is of no interest - i.e.,
 *   typically when the entry is for an H.262 sequence header.
 * - `start_posn` is the location of the start of the entry in the file,
 * - `length` is the number of bytes in the entry
 * - for H.262 data, if the entry is a picture, then `seq_offset` will
 *   be the offset backwards to the previous nearest sequence header
 *   (i.e., 1 if the sequence header is the previous entry), and if it is
 *   a sequence header, `seq_offset` will be 0. For H.264 data, the value
 *   will always be 0. `seq_offset` may be passed as NULL if the value is
 *   of no interest.
 * - for H.262 data, if the entry is a picture, then `afd` will be its
 *   (effective) AFD byte. Otherwise it will be 0. `afd` may be passed as NULL
 *   if the value if of no interest.
 *
 * To clarify, all of the following are legitimate calls::
 *
 *    err = get_reverse_data(reverse_data,10,&index,&start,&length,&offset,&afd);
 *    err = get_reverse_data(reverse_data,10,&index,&start,&length,NULL,NULL);
 *    err = get_reverse_data(reverse_data,10,NULL,&start,&length,NULL,NULL);
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int get_reverse_data(reverse_data_p reverse_data, int which, uint32_t *index,
                            ES_offset *start_posn, uint32_t *length, byte *seq_offset, byte *afd);

// ============================================================
// Collecting pictures
// ============================================================
/*
 * Locate and remember sequence headers and I pictures, for later reversal.
 *
 * - `h262` is the H.262 stream reading context
 * - if `max` is non-zero, then collecting will stop after `max` pictures
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 *
 * Returns 0 if all went well, EOF if the end of file is reached,
 * and 1 if an error occurred.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int collect_reverse_h262(h262_context_p h262, int max, int verbose, int quiet);
/*
 * Find IDR and I slices, and remember their access units for later output
 * in reverse order.
 *
 * - `acontext` is the access unit reading context
 * - if `max` is non-zero, then collecting will stop after `max` access units
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 *
 * Returns 0 if all went well, EOF if the end of file is reached,
 * and 1 if an error occurred.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int collect_reverse_access_units(access_unit_context_p acontext, int max, int verbose,
                                        int quiet);
/*
 * Output the last picture (or an earlier one) from the reverse arrays.
 * This version writes the data out as Transport Stream.
 *
 * This is expected to be used after the whole of the data stream has been
 * played, so that the last picture in the reverse arrays is the last I or
 * IDR picture in the data stream.
 *
 * - `es` is the input elementary stream
 * - `tswriter` is the transport stream writer
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `offset` is the offset from the end of the array of the picture
 *   to output - so 0 means the last picture, 1 the picture before that,
 *   and so on. Sequence headers do not count for this purpose.
 * - `reverse_data` is the reverse data context.
 *
 * Returns 0 if all went well, 1 if an error occurred.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int output_from_reverse_data_as_TS(ES_p es, TS_writer_p tswriter, int verbose, int quiet,
                                          uint32_t offset, reverse_data_p reverse_data);
/*
 * Output the last picture (or an earlier one) from the reverse arrays.
 * This version writes the data out as Elementary Stream.
 *
 * This is expected to be used after the whole of the data stream has been
 * played, so that the last picture in the reverse arrays is the last I or
 * IDR picture in the data stream.
 *
 * - `es` is the input elementary stream
 * - `output` is the stream to write to
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `offset` is the offset from the end of the array of the picture
 *   to output - so 0 means the last picture, 1 the picture before that,
 *   and so on. Sequence headers do not count for this purpose.
 * - `reverse_data` is the reverse data context.
 *
 * Returns 0 if all went well, 1 if an error occurred.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int output_from_reverse_data_as_ES(ES_p es, FILE *output, int verbose, int quiet,
                                          uint32_t offset, reverse_data_p reverse_data);
/*
 * Output the H.262 pictures or H.264 access units we remembered earlier - but
 * in reverse order. This version writes the data out as Transport Stream.
 *
 * - `es` is the input elementary stream
 * - `tswriter` is the transport stream writer
 * - if `frequency` is non-zero, then attempt to produce the effect of
 *   keeping every <frequency>th picture (similar to reversing at a
 *   multiplication factor of `frequency`) If 0, just output all the
 *   pictures that were remembered.
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `start_with` is the index at which to start outputting from the
 *   reverse data arrays. The value -1 may be used to indicate the most
 *   recent picture in the arrays. If `start_with` is less than -1 then this
 *   function will do nothing. If `start_with` is off the end of the
 *   arrays, then reversing will start from the end of the arrays.
 * - if `max` is non-zero, then output will stop after at least `max`
 *   pictures have been reversed past.
 * - `reverse_data` contains the list of pictures/access units to reverse.
 *
 * Returns 0 if all went well, 1 if an error occurred.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int output_in_reverse_as_TS(ES_p es, TS_writer_p tswriter, int frequency, int verbose,
                                   int quiet, int32_t start_with, int max,
                                   reverse_data_p reverse_data);
/*
 * Output the H.262 pictures or H.264 access units we remembered earlier - but
 * in reverse order. This version writes the data out as Elementary Stream.
 *
 * - `es` is the input elementary stream
 * - `output` is the stream to write to
 * - if `frequency` is non-zero, then attempt to produce the effect of
 *   keeping every <frequency>th picture (similar to reversing at a
 *   multiplication factor of `frequency`) If 0, just output all the
 *   pictures that were remembered.
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `start_with` is the index at which to start outputting from the
 *   reverse data arrays. The value -1 may be used to indicate the most
 *   recent in the arrays. If `start_with` is less than -1 then this
 *   function will do nothing. If `start_with` is off the end of the
 *   arrays, then reversing will start from the end of the arrays.
 * - if `max` is non-zero, then output will stop after at least `max`
 *   pictures have been reversed past.
 * - `reverse_data` contains the list of pictures/access units to reverse.
 *
 * Returns 0 if all went well, 1 if an error occurred.
 *
 * If command input is enabled, then it can also return COMMAND_RETURN_CODE
 * if the current command has changed.
 */
extern int output_in_reverse_as_ES(ES_p es, FILE *output, int frequency, int verbose, int quiet,
                                   int32_t start_with, int max, reverse_data_p reverse_data);

#endif // _reverse_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

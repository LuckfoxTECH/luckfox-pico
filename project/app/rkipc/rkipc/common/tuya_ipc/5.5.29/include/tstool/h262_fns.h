/*
 * Prototypes for reading H.262 (MPEG-2) elementary streams.
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

#ifndef _h262_fns
#define _h262_fns

#include "h262_defns.h"

/*
 * Print out information derived from the start code, to the given stream.
 *
 * Note that if a "SYSTEM START" code is reported, then the data is
 * likely to be PES or Transport Stream data, not Elementary Stream.
 *
 * Similarly, if a "TRANSPORT STREAM sync byte" is reported, then
 * the stream is probably Transport Stream.
 *
 * If the stream is *not* Elementary Stream data, then it is possible
 * that some of the apparent start code prefixes are actually false
 * detections.
 */
extern void print_h262_start_code_str(FILE *stream, byte start_code);
/*
 * Build a new MPEG2 item datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_h262_item(h262_item_p *item);
/*
 * Tidy up and free an MPEG2 item datastructure after we've finished with it.
 *
 * Empties the MPEG2 item datastructure, frees it, and sets `item` to NULL.
 *
 * If `item` is already NULL, does nothing.
 */
extern void free_h262_item(h262_item_p *item);
/*
 * Print out useful information about this MPEG2 item, on the given stream.
 */
extern void report_h262_item(FILE *stream, h262_item_p item);
// ------------------------------------------------------------
// MPEG2 item *data* stuff
// ------------------------------------------------------------
/*
 * Find and read in the next MPEG2 item.
 *
 * Be careful if using this in conjunction with reading H.262 pictures
 * via an `h262_context_p`, as it does not maintain the "last item read"
 * information therein.
 *
 * - `es` is the elementary stream we're reading from.
 * - `item` is the datastructure containing the MPEG2 item found, or NULL
 *   if there was none.
 *
 * Returns 0 if it succeeds, EOF if the end-of-file is read (i.e., there
 * is no next MPEG2 item), otherwise 1 if some error occurs.
 */
extern int find_next_h262_item(ES_p es, h262_item_p *item);
/*
 * Build a new H.262 picture reading context.
 *
 * This acts as a "jacket" around the ES context, and is used when reading
 * H.262 pictures with get_next_h262_picture(). It "remembers" the last
 * item read, which is the first item that was not part of the picture.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_h262_context(ES_p es, h262_context_p *context);
/*
 * Free an H.262 picture reading context.
 *
 * Clears the datastructure, frees it, and returns `context` as NULL.
 *
 * Does not free any `reverse_data` datastructure.
 *
 * Does nothing if `context` is already NULL.
 */
extern void free_h262_context(h262_context_p *context);
/*
 * Rewind a file being read as H.262 pictures
 *
 * This is a wrapper for `seek_ES` that also knows to unset things appropriate
 * to the H.262 picture reading context.
 *
 * If a reverse context is attached to this context, it also will
 * be "rewound" appropriately.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int rewind_h262_context(h262_context_p context);
/*
 * Free an H.262 "picture".
 *
 * Clears the datastructure, frees it, and returns `picture` as NULL.
 *
 * Does nothing if `picture` is already NULL.
 */
extern void free_h262_picture(h262_picture_p *picture);
/*
 * Compare two H.262 pictures. The comparison does not include the start
 * position of the picture, but just the actual data - i.e., two pictures
 * read from different locations in the input stream may be considered the
 * same if their data content is identical.
 *
 * Returns TRUE if the lists contain identical content, FALSE otherwise.
 */
extern int same_h262_picture(h262_picture_p picture1, h262_picture_p picture2);
/*
 * Retrieve the the next H.262 "picture".
 *
 * The H.262 "picture" returned can be one of:
 *
 * 1. A field or frame, including its slices.
 * 2. A sequence header, including its sequence extension, if any.
 * 3. A sequence end.
 *
 * - `context` is the H.262 picture reading context.
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `picture` is the H.262 "picture", containing a field or frame picture,
 *   a sequence header or a sequence end
 *
 * Returns 0 if it succeeds, EOF if we reach the end of file, or 1 if some
 * error occurs.
 */
extern int get_next_h262_single_picture(h262_context_p context, int verbose,
                                        h262_picture_p *picture);
/*
 * Retrieve the the next H.262 "picture".
 *
 * The H.262 "picture" returned can be one of:
 *
 * 1. A frame, including its slices. This may be the concatenation of two
 *    adjacent field pictures.
 * 2. A sequence header, including its sequence extension, if any.
 * 3. A sequence end.
 *
 * Specifically, the next H.262 "picture" is retrieved from the input stream.
 *
 * If that "picture" represents a sequence header or a frame, it is returned.
 *
 * If it represents a field, then the *following* "picture" is retrieved, and
 * if that is the second field of its frame, it is merged into the first,
 * and the resultant frame is returned.
 *
 * If a field with temporal reference A is followed by a field with temporal
 * reference B, it is assumed that synchronisation has been lost. In this
 * case, the first field (frame A) will be discarded, and an attempt made to
 * read the second field of frame B.
 *
 * Similarly, if a frame or sequence header is found instead of the second
 * field, the first field will be discarded and the frame returned.
 *
 *   Note that if the context is associated with a reverse context,
 *   then appropriate frames/sequence headers will automatically be
 *   remembered therein.
 *
 * - `context` is the H.262 picture reading context.
 * - if `verbose` is true, then extra information will be output
 * - if `quiet` is true, then only errors will be reported
 * - `picture` is the H.262 "picture", containing a frame picture,
 *   a sequence header or a sequence end
 *
 * Returns 0 if it succeeds, EOF if we reach the end of file, or 1 if some
 * error occurs.
 */
extern int get_next_h262_frame(h262_context_p context, int verbose, int quiet,
                               h262_picture_p *picture);
/*
 * Write out an H.262 picture as TS
 *
 * - `tswriter` is TS the output stream
 * - `picture` is the picture to write out
 * - `pid` is the PID to use for the TS packets
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_h262_picture_as_TS(TS_writer_p tswriter, h262_picture_p picture, uint32_t pid);
/*
 * Write out a picture (as stored in an ES unit list) as ES
 *
 * - `output` is the ES output file
 * - `picture` is the picture to write out
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int write_h262_picture_as_ES(FILE *output, h262_picture_p picture);
/*
 * Report on an H.262 picture's contents.
 *
 * - `stream` is where to write the information
 * - `picture` is the picture to report on
 * - if `report_data`, then the component ES units will be printed out as well
 */
extern void report_h262_picture(FILE *stream, h262_picture_p picture, int report_data);

#endif // _h262_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

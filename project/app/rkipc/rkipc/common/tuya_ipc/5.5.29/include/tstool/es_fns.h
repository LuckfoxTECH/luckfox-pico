/*
 * Prototypes for handling H.264 elementary streams.
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

#ifndef _es_fns
#define _es_fns

#include "es_defns.h"

// ============================================================
// Elementary stream functions - basic
// ============================================================

/*
 * Open an ES file and build an elementary stream datastructure to read
 * it with.
 *
 * - `filename` is the ES files name
 *
 * Opens the file for read, builds the datastructure, and reads the first 3
 * bytes of the input file (this is done to prime the triple-byte search
 * mechanism).
 *
 * Use `close_elementary_stream` to close the stream and the file.
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
extern int open_elementary_stream(char *filename, ES_p *es);

/*
 * Build an elementary stream datastructure attached to an input file.
 * This is intended for reading ES data files.
 *
 * - `input` is the file stream to read from.
 *
 * Builds the datastructure, and reads the first 3 bytes of the input
 * file (this is done to prime the triple-byte search mechanism).
 *
 * Use `free_elementary_stream` to release the ES context without closing
 * the associated file.
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
extern int build_elementary_stream_file(int input, ES_p *es);

/*
 * Build an elementary stream datastructure for use with a PES reader.
 * Reads the first (or next) three bytes of the ES.
 *
 * This reads data from the PES video data, ignoring any audio data.
 *
 * - `reader` is the PES reader we want to use to read our TS or PS data.
 *
 * The caller must explicitly close the PES reader as well as closing the
 * elementary stream (closing the ES does not affect the PES reader).
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
extern int build_elementary_stream_PES(PES_reader_p reader, ES_p *es);

/*
 * Tidy up the elementary stream datastructure after we've finished with it.
 *
 * Specifically:
 *
 * - free the datastructure
 * - set `es` to NULL
 *
 * No return status is given, since there's not much one can do if anything
 * *did* go wrong, and if something went wrong and the program is continuing,
 * it's bound to show up pretty soon.
 */
extern void free_elementary_stream(ES_p *es);

/*
 * Tidy up the elementary stream datastructure after we've finished with it.
 *
 * Specifically:
 *
 * - close the input file (if its stream is set, and if it's not STDIN)
 * - call `free_elementary_stream()`
 *
 * No return status is given, since there's not much one can do if anything
 * *did* go wrong, and if something went wrong and the program is continuing,
 * it's bound to show up pretty soon.
 */
extern void close_elementary_stream(ES_p *es);

/*
 * Ask an ES context if changed input is available.
 *
 * This is a convenience wrapper to save querying the ES context to see
 * if it is (a) reading from PES, (b) automatically writing the PES packets
 * out via a TS writer, and (c) if said TS writer has a changed command.
 *
 * Calls `tswrite_command_changed()` on the TS writer associated with this ES.
 *
 * Returns TRUE if there is a changed command.
 */
extern int es_command_changed(ES_p es);

// ============================================================
// Elementary stream functions - item/unit reading
// ============================================================
/*
 * Prepare the contents of a (new) ES unit datastructure.
 *
 * Allocates a new data array, and unsets the counts.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int setup_ES_unit(ES_unit_p unit);

/*
 * Tidy up an ES unit datastructure after we've finished with it.
 *
 * (Frees the internal data array, and unsets the counts)
 */
extern void clear_ES_unit(ES_unit_p unit);

/*
 * Build a new ES unit datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_ES_unit(ES_unit_p *unit);

/*
 * Build a new ES unit datastructure, from a given data array.
 *
 * Takes a copy of 'data'. Sets 'start_code' appropriately,
 * sets 'start_posn' to (0,0), and 'PES_had_PTS' to FALSE.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_ES_unit_from_data(ES_unit_p *unit, byte *data, uint32_t data_len);

/*
 * Tidy up and free an ES unit datastructure after we've finished with it.
 *
 * Empties the ES unit datastructure, frees it, and sets `unit` to NULL.
 *
 * If `unit` is already NULL, does nothing.
 */
extern void free_ES_unit(ES_unit_p *unit);

/*
 * Print out some information this ES unit, on the given stream
 */
extern void report_ES_unit(FILE *stream, ES_unit_p unit);

/*
 * Retrieve ES data from the end of a PES packet. It is assumed (i.e, things
 * will go wrong if it is not true) that at least one ES unit has been read
 * from the PES data stream via the ES reader.
 *
 * - `es` is our ES reader. It must be reading ES from PES packets.
 * - `data` is the ES data remaining (to be read) in the current PES packet.
 *   It is up to the caller to free this data.
 * - `data_len` is the length of said data. If this is 0, then `data`
 *   will be NULL.
 *
 * Returns 0 if all goes well, 1 if an error occurs.
 */
extern int get_end_of_underlying_PES_packet(ES_p es, byte **data, int *data_len);

/*
 * Find and read in the next ES unit.
 *
 * In general, unless there are compelling reasons, use
 * `find_and_build_next_ES_unit()` instead.
 *
 * - `es` is the elementary stream we're reading from.
 * - `unit` is the datastructure into which to read the ES unit
 *   - any previous content will be lost.
 *
 * Returns 0 if it succeeds, EOF if the end-of-file is read (i.e., there
 * is no next ES unit), otherwise 1 if some error occurs.
 */
extern int find_next_ES_unit(ES_p es, ES_unit_p unit);

/*
 * Find and read the next ES unit into a new datastructure.
 *
 * - `es` is the elementary stream we're reading from.
 * - `count` is an integer to use as an id for this ES unit - typically
 *   its index in the input stream
 * - `unit` is the datastructure containing the ES unit found, or NULL
 *   if there was none.
 *
 * Returns 0 if it succeeds, EOF if the end-of-file is read (i.e., there
 * is no next ES unit), otherwise 1 if some error occurs.
 */
extern int find_and_build_next_ES_unit(ES_p es, ES_unit_p *unit);

/*
 * Write (copy) the current ES unit to the output stream.
 *
 * Note that it writes out all of the data for this ES unit,
 * including its 00 00 01 start code prefix.
 *
 * - `output` is the output stream (file descriptor) to write to
 * - `unit` is the ES unit to write
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int write_ES_unit(FILE *output, ES_unit_p unit);

// ------------------------------------------------------------
// Arbitrary reading from ES data
// ------------------------------------------------------------
/*
 * "Seek" to the given position in the ES data, which is assumed to
 * be an offset ready to read a 00 00 01 sequence.
 *
 * If the ES reader is using PES to read its data, then both fields
 * of `where` are significant, but if the underlying file *is* just a file,
 * only `where.infile` is used.
 *
 * Returns 0 if all went well, 1 is something went wrong
 */
extern int seek_ES(ES_p es, ES_offset where);
/*
 * Read in some ES data from disk.
 *
 * Suitable for use when reading in a set of ES units whose bounds
 * (start offset and total number of bytes) have been remembered.
 *
 * "Seeks" to the given position in the ES data, which is assumed to
 * be an offset ready to read a 00 00 01 sequence, and reads data thereafter.
 *
 * After this function, the triple byte context is set to FF FF FF, and the
 * position of said bytes are undefined, but the next position to read a byte
 * from *is* defined.
 *
 * The intent is to allow the caller to have a data array (`data`) that
 * always contains the last data read, and is of the required size, and
 * need only be freed when no more data is needed.
 *
 * - `es` is where to read our data from
 * - `start_posn` is the file offset to start reading at
 * - `num_bytes` is how many bytes we want to read
 * - `data_len` may be NULL or a pointer to a value.
 *   If it is NULL, then the data array will be reallocated to size
 *   `num_bytes` regardless. If it is non-NULL, it should be passed *in*
 *    as the size that `data` *was*, and will be returned as the size
 *    that `data` is when the function returns.
 * - `data` is the data array to read into. If this is NULL, or if `num_bytes`
 *   is NULL, or if `num_bytes` is greater than `data_len`, then it will be
 *   reallocated to size `num_bytes`.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int read_ES_data(ES_p es, ES_offset start_posn, uint32_t num_bytes, uint32_t *data_len,
                        byte **data);

// ============================================================
// Lists of ES units
// ============================================================
/*
 * Build a new list-of-ES-units datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_ES_unit_list(ES_unit_list_p *list);

/*
 * Add a copy of an ES unit to the end of the ES unit list
 *
 * Note that since this takes a copy of the ES unit's data, it is safe
 * to free the original ES unit.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int append_to_ES_unit_list(ES_unit_list_p list, ES_unit_p unit);

/*
 * Reset (empty) an ES unit list.
 */
extern void reset_ES_unit_list(ES_unit_list_p list);

/*
 * Tidy up and free an ES unit list datastructure after we've finished with it.
 *
 * Clears the datastructure, frees it and returns `list` as NULL.
 *
 * Does nothing if `list` is already NULL.
 */
extern void free_ES_unit_list(ES_unit_list_p *list);

/*
 * Report on an ES unit list's contents.
 *
 * - `stream` is where to write the information
 * - `name` is the name of the list (used in the header)
 * - `list` is the list to report on
 */
extern void report_ES_unit_list(FILE *stream, char *name, ES_unit_list_p list);

/*
 * Retrieve the bounds of this ES unit list in the file it was read from.
 *
 * - `list` is the ES unit list we're interested in
 * - `start` is its start position (i.e., the location at which to start
 *   reading to retrieve all of the data for the list)
 * - `length` is the total length of the ES units within this list
 *
 * Returns 0 if all goes well, 1 if the ES unit list has no content.
 */
extern int get_ES_unit_list_bounds(ES_unit_list_p list, ES_offset *start, uint32_t *length);
/*
 * Compare two ES unit lists. The comparison does not include the start
 * position of the unit data, but just the actual data - i.e., two unit lists
 * read from different locations in the input stream may be considered the
 * same if their data content is identical.
 *
 * - `list1` and `list2` are the two ES unit lists to compare.
 *
 * Returns TRUE if the lists contain identical content, FALSE otherwise.
 */
extern int same_ES_unit_list(ES_unit_list_p list1, ES_unit_list_p list2);

/*
 * Compare two ES offsets
 *
 * Returns -1 if offset1 < offset2, 0 if they are the same, and 1 if
 * offset1 > offset2.
 */
extern int compare_ES_offsets(ES_offset offset1, ES_offset offset2);

// ============================================================
// Simple file type guessing
// ============================================================
/*
 * Look at the start of an elementary stream to try to determine its
 * video type.
 *
 * "Eats" the ES units that it looks at, and doesn't rewind the stream
 * afterwards.
 *
 * - `es` is the ES file
 * - if `print_dots` is true, print a dot for each ES unit that is inspected
 * - if `show_reasoning` is true, then output messages explaining how the
 *   decision is being made
 * - `video_type` is the final decision -- one of VIDEO_H264, VIDEO_H262,
 *   VIDEO_AVS, or VIDEO_UNKNOWN.
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int decide_ES_video_type(ES_p es, int print_dots, int show_reasoning, int *video_type);
/*
 * Look at the start of an elementary stream to try to determine it's
 * video type.
 *
 * Note that it is easier to prove something is H.262 (or AVS) than to prove
 * that it is H.264, and that the result of this routine is a best-guess, not a
 * guarantee.
 *
 * Rewinds back to the original position in the file after it has finished.
 *
 * - `input` is the file to look at
 * - if `print_dots` is true, print a dot for each ES unit that is inspected
 * - if `show_reasoning` is true, then output messages explaining how the
 *   decision is being made
 * - `video_type` is the final decision -- one of VIDEO_H264, VIDEO_H262,
 *   VIDEO_AVS, or VIDEO_UNKNOWN.
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int decide_ES_file_video_type(int input, int print_dots, int show_reasoning,
                                     int *video_type);
#endif // _es_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

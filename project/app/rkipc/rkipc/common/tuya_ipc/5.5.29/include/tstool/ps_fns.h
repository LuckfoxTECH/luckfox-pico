/*
 * Functions for working with H.222 Program Stream packets - in particular,
 * for reading PES packets.
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

#ifndef _ps_fns
#define _ps_fns

#include "compat.h"
#include "h222_defns.h"
#include "ps_defns.h"
#include "tswrite_defns.h"

// ============================================================
// Program stream reading functions
// ============================================================
/*
 * Build a program stream context attached to an input file. This handles
 * read-ahead buffering for the PS.
 *
 * - `input` is the file stream to read from.
 * - If `quiet`, then don't report on ignored bytes at the start of the file
 * - `ps` is the new PS context
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
extern int build_PS_reader(int input, int quiet, PS_reader_p *ps);
/*
 * Tidy up the PS read-ahead context after we've finished with it.
 *
 * Specifically:
 *
 * - free the datastructure
 * - set `ps` to NULL
 *
 * Does not close the associated file.
 */
extern void free_PS_reader(PS_reader_p *ps);
/*
 * Open a PS file for reading.
 *
 * - `name` is the name of the file
 * - If `quiet`, then don't report on ignored bytes at the start of the file
 * - `ps` is the new PS context
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
extern int open_PS_file(char *name, int quiet, PS_reader_p *ps);
/*
 * Close a PS file, and free the reader context
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
extern int close_PS_file(PS_reader_p *ps);
/*
 * Given a program stream, attempt to determine if it holds H.262 or H.264
 * data.
 *
 * Leaves the PS rewound to its "start".
 *
 * NOTE: It is probably better to use determine_PS_video_type().
 *
 * - `ps` is the program stream to check (assumed just to have been
 *   opened/built). This cannot be standard input, as it must be
 *   seekable.
 * - `is_h264` is the result
 *
 * Returns 0 if all goes well, 1 if there was an error (including the
 * stream not appearing to be either).
 */
extern int determine_if_PS_is_h264(PS_reader_p ps, int *is_h264);
/*
 * Given a program stream, attempt to determine what type of video data it
 * contains.
 *
 * Leaves the PS rewound to its "start".
 *
 * - `ps` is the program stream to check (assumed just to have been
 *   opened/built). This cannot be standard input, as it must be
 *   seekable.
 * - `video_type` is the result. Calls determine_PES_video_type().
 *
 * Returns 0 if all goes well, 1 if there was an error (including the
 * stream not appearing to be either).
 */
extern int determine_PS_video_type(PS_reader_p ps, int *video_type);
/*
 * Seek within the PS file.
 *
 * Note that if the intent is to *rewind* to the start of the PS data,
 * then `rewind_program_stream` should be used instead, as offset 0 is
 * not necessarily the same as the start of the program stream.
 *
 * - `ps` is the PS read-ahead context
 * - `posn` is the file offset to seek to
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int seek_using_PS_reader(PS_reader_p ps, offset_t posn);
/*
 * Rewind the PS context to the remembered "start of data"
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int rewind_program_stream(PS_reader_p ps);
/*
 * Print out a stream id in a manner consistent with the PS usages
 * of the stream id values.
 */
extern void print_stream_id(FILE *stream, byte stream_id);
/*
 * Look for the start (the first 4 bytes) of the next program stream packet.
 *
 * Assumes that (for some reason) alignment has been lost, and thus it is
 * necessary to scan forwards to find the next 00 00 01 prefix.
 *
 * Otherwise equivalent to a call of `read_PS_packet_start`.
 *
 * - `ps` is the PS read-ahead context we're reading from
 * - if `verbose`, then we want to explain what we're doing
 * - if `max` is non-zero, then it is the maximum number of bytes
 *   to scan before giving up.
 * - `posn` is the file offset of the start of the packet
 * - `stream_id` is the identifying byte, after the 00 00 01 prefix. Note
 *   that this is set correctly if MPEG_program_end_code was read, and is
 *   0 if an error occurred.
 *
 * Returns:
 *   * 0 if it succeeds,
 *   * EOF if EOF is read, or an MPEG_program_end_code is read, or
 *   * 1 if some error (including the first 3 bytes not being 00 00 01) occurs.
 */
extern int find_PS_packet_start(PS_reader_p ps, int verbose, uint32_t max, offset_t *posn,
                                byte *stream_id);
/*
 * Look for the next PS pack header.
 *
 * Equivalent to calling `find_PS_packet_start` until `stream_id` is 0xBA
 * (in other words, equivalent to having read the pack header start with
 * `read_PS_packet_start`).
 *
 * If you want to call `read_PS_packet_start` to read this pack header start
 * in again, then call ``seek_using_PS_reader(ps,posn)`` to reposition ready
 * to read it.
 *
 * - `ps` is the PS read-ahead context we're reading from
 * - if `verbose`, then the 00 00 01 XX sequences found will be logged
 *   to stderr, indicating the progress of our search
 * - if `max` is non-zero, then it is the maximum number of bytes
 *   to scan before giving up.
 * - `posn` is the file offset of the start of the packet found
 *
 * Returns:
 *   * 0 if it succeeds,
 *   * EOF if EOF is read, or an MPEG_program_end_code is read, or
 *   * 1 if some error (including the first 3 bytes not being 00 00 01) occurs.
 */
extern int find_PS_pack_header_start(PS_reader_p ps, int verbose, uint32_t max, offset_t *posn);
/*
 * Read in (the rest of) a PS packet according to its length.
 *
 * Suitable for use reading PS PES packets and PS system header packets.
 *
 * NOTE that the `data` buffer in the `packet` is realloc'ed by this
 * function. It is thus important to ensure that the `packet` datastructure
 * contains a NULL pointer for said buffer before the first call of this
 * function.
 *
 * - `ps` is the PS read-ahead context we're reading from
 * - `stream_id` identifies what sort of packet it is
 * - `packet` is the packet we're reading the PES packet into.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int read_PS_packet_body(PS_reader_p ps, byte stream_id, PS_packet_p packet);
/*
 * Read in the body of the pack header (but *not* the system header packets
 * therein).
 *
 * - `ps` is the PS read-ahead context we're reading from
 * - `hdr` is the packet we've read
 *
 * Returns 0 if it succeeds, or 1 if some error occurs.
 */
extern int read_PS_pack_header_body(PS_reader_p ps, PS_pack_header_p hdr);

/*
 * Clear the contents of a PS packet datastructure. Frees the internal
 * `data` array.
 */
extern void clear_PS_packet(PS_packet_p packet);
/*
 * Tidy up and free a PS packet datastructure after we've finished with it.
 *
 * Empties the PS packet datastructure, frees it, and sets `unit` to NULL.
 *
 * If `unit` is already NULL, does nothing.
 */
extern void free_PS_packet(PS_packet_p *packet);
/*
 * Read in the start (the first 4 bytes) of the next program stream packet.
 *
 * If the bytes read don't appear to be valid (i.e., they do not start with
 * the 00 00 01 prefix), then the next pack header will be sought and read in.
 *
 * Note that sequences of 00 bytes before the 00 00 01 will be ignored.
 *
 * - `ps` is the PS read-ahead context we're reading from
 * - if `verbose`, then we want to explain what we're doing
 * - `posn` is the file offset of the start of the packet
 * - `stream_id` is the identifying byte, after the 00 00 01 prefix. Note
 *   that this is set correctly if MPEG_program_end_code was read, and is
 *   0 if an error occurred or reading was ended because `max` packets had
 *   been read.
 *
 * Returns:
 *   * 0 if it succeeds,
 *   * EOF if EOF is read, or an MPEG_program_end_code is read,
 *   * 2 if the bytes read are not 00 00 01 `stream_id`, or
 *   * 1 if some other error occurs.
 */
extern int read_PS_packet_start(PS_reader_p ps, int verbose, offset_t *posn, byte *stream_id);

/*
 * Inspect the given PS packet, and determine if it contains AC3 or DTS audio data.
 *
 * - `packet` is the packet's data, already established as private_data_1
 * - `is_dvd` is true if the data should be interpreted as DVD data
 * - if `verbose`, report on the details of what we find out
 * - `substream_index` returns the substream's index, taken from the low
 *   nibble of the substream id, and adjusted to start at 0. This will be
 *   a value in the range 0-7 for DTS, AC3 and LPCM, and in the range 0-1F
 *   (0-31) for subpictures.
 * - for AC3, `bsmod` and `acmod` return the appropriate quantities,
 *   otherwise they are 0.
 *
 * Returns one of the SUBSTREAM_* values.
 */
extern int identify_private1_data(struct PS_packet *packet, int is_dvd, int verbose,
                                  int *substream_index, byte *bsmod, byte *acmod);

// ============================================================
// PS to TS functions
// ============================================================
/*
 * Read program stream and write transport stream
 *
 * - `ps` is the program stream
 * - `output` is the transport stream
 * - `pad_start` is the number of filler TS packets to start the output
 *   with.
 * - `program_repeat` is how often (after how many PS packs) to repeat
 *   the program information (PAT/PMT)
 * - `video_type` indicates what type of video is being transferred. It should
 *   be VIDEO_H264, VIDEO_H262, etc.
 * - `is_dvd` should be true if this input represents DVD data; i.e., with
 *   private_stream_1 used for AC-3/DTS/etc., and with substream headers
 *   therein.
 * - `video_stream` indicates which video stream we want - i.e., the stream
 *   with id 0xE0 + <video_stream>. -1 means the first encountered.
 * - `audio_stream` indicates which audio stream we want. If `want_ac3_audio`
 *   is false, then this will be the stream with id 0xC0 + <audio_stream>,
 *   or -1 for the first audio stream encountered.
 * - if `want_ac3_audio` is true, then if `is_dvd` is true, then we want
 *   audio from private_stream_1 (0xBD) with substream id <audio_stream>,
 *   otherwise we ignore `audio_stream` and assume that all data in
 *   private_stream_1 is the audio we want.
 * - `dolby_is_dvb` should be true if Dolby (AC-3) audio (if selected) should
 *   be output using the DVB stream type 0x06, false if using the ATSC stream
 *   type 0x81. This is ignored if the audio being output is not Dolby.
 * - `pmt_pid` is the PID of the PMT to write
 * - `pcr_pid` is the PID of the TS unit containing the PCR
 * - `video_pid` is the PID for the video we write
 * - `keep_audio` is true if the audio stream should be output, false if
 *   it should be ignored
 * - `audio_pid` is the PID for the audio we write
 * - if `max` is non-zero, then we want to stop reading after we've read
 *   `max` packs
 * - if `verbose` then we want to output diagnostic information
 * - if `quiet` then we want to be as quiet as we can
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int ps_to_ts(PS_reader_p ps, TS_writer_p output, int pad_start, int program_repeat,
                    int video_type, int is_dvd, int video_stream, int audio_stream,
                    int want_ac3_audio, int dolby_is_dvb, uint32_t pmt_pid, uint32_t pcr_pid,
                    uint32_t video_pid, int keep_audio, uint32_t audio_pid, int max, int verbose,
                    int quiet);

#endif // _ps_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

/*
 * Functions for reading PES packets from TS or PS files
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

#ifndef _pes_fns
#define _pes_fns

#include "es_defns.h"
#include "pes_defns.h"

/*
 * Free a PES packet datastructure
 *
 * - `data` is the PES packet datastructure, which will be freed,
 *   and returned as NULL.
 */
extern void free_PES_packet_data(PES_packet_data_p *data);
/*
 * Look at the start of a file to determine if it appears to be transport
 * stream. Rewinds the file when it is finished.
 *
 * The file is assumed to be Transport Stream if it starts with 0x47 as
 * the first byte, and 0x47 recurs at 188 byte intervals (in other words,
 * it appears to start with several TS packets).
 *
 * - `input` is the file to check
 * - `is_TS` is TRUE if it looks like TS, as described above.
 *
 * Returns 0 if all goes well, 1 if there was an error.
 */
extern int determine_if_TS_file(int input, int *is_TS);
/*
 * Build a PES reader datastructure for PS data
 *
 * - `ps` is the Program Stream to read the PES data from
 * - `give_info` is TRUE if information about program data, etc., should be
 *   output (to stdout).
 * - `give_warnings` is TRUE if warnings (starting with "!!!") should be
 *   output (to stderr), FALSE if they should be suppressed.
 * - `reader` is the resulting PES reader
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int build_PS_PES_reader(PS_reader_p ps, int give_info, int give_warnings,
                               PES_reader_p *reader);
/*
 * Build a PES reader datastructure for TS data
 *
 * - `tsreader` is the Transport Stream to read the PES data from
 * - `give_info` is TRUE if information about program data, etc., should be
 *   output (to stdout).
 * - `give_warnings` is TRUE if warnings (starting with "!!!") should be
 *   output (to stderr), FALSE if they should be suppressed.
 * - `program_number` is only used for TS data, and identifies which program
 *   to read. If this is 0 then the first program encountered in the first PAT
 *   will be read.
 * - `reader` is the resulting PES reader
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int build_TS_PES_reader(TS_reader_p tsreader, int give_info, int give_warnings,
                               uint16_t program_number, PES_reader_p *reader);
/*
 * Build a PES reader datastructure
 *
 * - `input` is the file to read the PES data from
 * - `is_TS` should be TRUE if the data is TS, FALSE if it is PS
 * - `give_info` is TRUE if information about program data, etc., should be
 *   output (to stdout).
 * - `give_warnings` is TRUE if warnings (starting with "!!!") should be
 *   output (to stderr), FALSE if they should be suppressed.
 * - `program_number` is only used for TS data, and identifies which program
 *   to read. If this is 0 then the first program encountered in the first PAT
 *   will be read.
 * - `reader` is the resulting PES reader
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int build_PES_reader(int input, int is_TS, int give_info, int give_warnings,
                            uint16_t program_number, PES_reader_p *reader);
/*
 * Open a Transport Stream file for PES packet reading
 *
 * - `filename` is the name of the file to open.
 * - `program_number` identifies which program to read. If this is 0
 *   then the first program encountered in the first PAT will be read.
 * - `give_info` is TRUE if information about program data, etc., should be
 *   output (to stdout). If information messages are requested, and the
 *   program number is given as 0, the actual program number chosen will
 *   be reported as well.
 * - `give_warnings` is TRUE if warnings (starting with "!!!") should be
 *   output (to stderr), FALSE if they should be suppressed.
 * - `reader` is the PES reader context corresponding to the newly
 *   opened file.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int open_PES_reader_for_TS(char *filename, uint16_t program_number, int give_info,
                                  int give_warnings, PES_reader_p *reader);
/*
 * Open a Program Stream file for PES packet reading
 *
 * - `filename` is the name of the file to open.
 * - `give_info` is TRUE if information about program data, etc., should be
 *   output (to stdout).
 * - `give_warnings` is TRUE if warnings (starting with "!!!") should be
 *   output (to stderr), FALSE if they should be suppressed.
 * - `reader` is the PES reader context corresponding to the newly
 *   opened file.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int open_PES_reader_for_PS(char *filename, int give_info, int give_warnings,
                                  PES_reader_p *reader);
/*
 * Open a Program Stream or Transport Stream file for PES packet reading
 *
 * - `filename` is the name of the file to open.
 * - `give_info` is TRUE if information about program data, etc., should be
 *   output (to stdout).
 * - `give_warnings` is TRUE if warnings (starting with "!!!") should be
 *   output (to stderr), FALSE if they should be suppressed.
 * - `reader` is the PES reader context corresponding to the newly
 *   opened file.
 *
 * If the file is Transport Stream, then this is equivalent to a call
 * of::
 *
 *    err = open_PES_reader_for_TS(filename,0,give_info,give_warnings,&reader);
 *
 * i.e., the first program found is the program that will be read.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int open_PES_reader(char *filename, int give_info, int give_warnings, PES_reader_p *reader);
/*
 * Tell the PES reader whether we only want video data
 *
 * - `video_only` should be TRUE if audio is to be ignored, FALSE
 *   if audio should be retained.
 *
 * By default, the PES reader returns video data and a single audio
 * stream (taken from the first audio stream encountered).
 */
extern void set_PES_reader_video_only(PES_reader_p reader, int video_only);
/*
 * Tell the PES reader which audio stream we want.
 *
 * By default, the PES reader returns video data and a single audio
 * stream (taken from the first audio stream encountered).
 *
 * - `reader` is the PES reader context
 * - `stream_number` is the number of the audio stream to read, from
 *   0 to 31 (0x1F).
 *
 * This call only has effect if the input data is PS.
 *
 * Returns 0 if all went well, or 1 if there was an error (specifically,
 * that `stream_number` was not in the range 0-31).
 */
extern int set_PES_reader_audio_stream(PES_reader_p reader, int stream_number);
/*
 * Tell the PES reader to get its audio stream from private stream 1
 * (this is the stream that is conventionally used for Dolby in DVD data).
 *
 * By default, the PES reader returns video data and a single audio
 * stream (taken from the first audio stream encountered).
 *
 * - `reader` is the PES reader context
 *
 * This call only has effect if the input data is PS.
 */
extern void set_PES_reader_audio_private1(PES_reader_p reader);
/*
 * Tell the PES reader to "pretend" it has read a PAT and PMT with
 * the given program information.
 *
 * This is intended for use in setting up sensible values when reading
 * PS data (which does not contain such information). It will silently
 * do nothing for TS data.
 *
 * Note that calling it more than once is allowed - it will happily
 * overwrite any previous values.
 *
 * - `reader` is the PES reader context
 * - `program_number` is the program number to assume. If this is 0,
 *   then 1 will be used.
 * - `pmt_pid` is the PID for the PMT we've pretended to read.
 * - `video_pid` is the PID to assume for the video data
 * - `audio_pid` is the PID to assume for the audio data (if any)
 * - `pcr_pid` is the PID to assume for the PCR data - this will often
 *   be the same as the `video_pid`
 */
extern void set_PES_reader_program_data(PES_reader_p reader, uint16_t program_number,
                                        uint32_t pmt_pid, uint32_t video_pid, uint32_t audio_pid,
                                        uint32_t pcr_pid);
/*
 * Tell the PES reader that the PS data it is reading is MPEG-4/AVC,
 * as opposed to MPEG-1/MPEG-2.
 */
extern void set_PES_reader_h264(PES_reader_p reader);
/*
 * Tell the PES reader that the PS data it is reading is of
 * type `video_type` (which is assumed to be a legitimate value
 * such as VIDEO_H264, etc.)
 */
extern void set_PES_reader_video_type(PES_reader_p reader, int video_type);
/*
 * Tell the PES reader whether to output any Dolby (AC-3) audio data
 * it may read using the DVB stream type (0x06) or the ATSC stream
 * type (0x81).
 *
 * If it is reading TS data, then the default is to use whatever stream type
 * the Dolby audio was read with.
 *
 * If it is reading PS data, then the default is to assume DVB data.
 *
 * This call only has effect if Dolby audio data is actually selected.
 */
extern void set_PES_reader_dolby_stream_type(PES_reader_p reader, int is_dvb);
/*
 * Reposition the PES reader to an earlier packet
 *
 * It is the caller's responsibility to choose a sensible `posn` to seek to.
 *
 * Note that using this to reposition in a PES reader does not affect any
 * "higher" reading context using this PES reader - specifically, if data
 * is being read via an ES reader, then calling this function directly
 * will result in the ES reader losing its positional information.
 *
 * In this case, `seek_ES` should be called.
 *
 * - `reader` is the PES reader context
 * - `posn` is a packet position obtained from an earlier PES packet
 *   datastructure (this should *not* be a random offset in the input
 *   file, as that will not in general work).
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int set_PES_reader_position(PES_reader_p reader, offset_t posn);
/*
 * Free a PES reader, and the relevant datastructures. Does not close
 * the underlying file.
 *
 * - `reader` is the PES reader context. This will be freed, and
 *   returned as NULL.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int free_PES_reader(PES_reader_p *reader);
/*
 * Close a PES reader, and free the relevant datastructures.
 *
 * - `reader` is the PES reader context. This will be freed, and
 *   returned as NULL.
 *
 * Returns 0 if all goes well, 1 if something goes wrong with closing the
 * file (although in that case, the `reader` will still have been freed).
 */
extern int close_PES_reader(PES_reader_p *reader);
/*
 * Return the next PES packet from the input file
 *
 * - `reader` is a PES reader context
 *
 * Returns 0 if all goes well, EOF if end of file is read, and 1 if
 * something goes wrong.
 */
extern int read_next_PES_packet(PES_reader_p reader);

/*
 * Given an MPEG-1 PES packet, determine the offset of the ES data.
 *
 * - `data` is the PES packet data, starting "00 00 01 <stream_id>
 *   <packet_length>"
 * - `data_len` is the actual length of the data therein
 *
 * Returns the required offset (i.e., packet[offset] is the first byte
 * of the ES data within the PES packet).
 */
extern int calc_mpeg1_pes_offset(byte *data, int data_len);

/*
 * Read in the next PES packet that contains ES data we are interested in
 * Ignores non-video packets.
 *
 * - `reader` is a PES reader context
 *
 * Returns 0 if all goes well, EOF if end of file is read, and 1 if
 * something goes wrong.
 */
extern int read_next_PES_ES_packet(PES_reader_p reader);
/*
 * If the given PES packet data contains a PTS field, return it
 *
 * - `data` is the data for this PES packet
 * - `data_len` is its length
 * - `got_pts` is TRUE if a PTS field was found, in which case
 * - `pts` is that PTS value
 *
 * Returns 0 if all went well, 1 if an error occurs.
 */
extern int find_PTS_in_PES(byte data[], int32_t data_len, int *got_pts, uint64_t *pts);
/*
 * If the given PES packet data contains a DTS field, return it
 *
 * - `data` is the data for this PES packet
 * - `data_len` is its length
 * - `got_dts` is TRUE if a DTS field was found, in which case
 * - `dts` is that DTS value
 *
 * Returns 0 if all went well, 1 if an error occurs.
 */
extern int find_DTS_in_PES(byte data[], int32_t data_len, int *got_dts, uint64_t *dts);
/*
 * If the given PES packet data contains a PTS and/or DTS field, return it
 *
 * - `data` is the data for this PES packet
 * - `data_len` is its length
 * - `got_pts` is TRUE if a PTS field was found, in which case
 * - `pts` is that PTS value
 * - `got_dts` is TRUE if a DTS field was found, in which case
 * - `dts` is that DTS value
 *
 * Returns 0 if all went well, 1 if an error occurs.
 */
extern int find_PTS_DTS_in_PES(byte data[], int32_t data_len, int *got_pts, uint64_t *pts,
                               int *got_dts, uint64_t *dts);
/*
 * If the given PES packet data contains an ESCR field, return it
 *
 * - `data` is the data for this PES packet
 * - `data_len` is its length
 * - `got_escr` is TRUE if an ESCR field was found, in which case
 * - `escr` is that ESCR value
 *
 * Returns 0 if all went well, 1 if an error occurs.
 */
extern int find_ESCR_in_PES(byte data[], int32_t data_len, int *got_escr, uint64_t *escr);
/*
 * Decode a PTS or DTS value.
 *
 * - `bytes` is the 5 bytes containing the encoded PTS or DTS value
 * - `required_guard` should be 2 for a PTS alone, 3 for a PTS before
 *   a DTS, or 1 for a DTS after a PTS
 * - `value` is the PTS or DTS value as decoded
 *
 * Returns 0 if the PTS/DTS value is decoded successfully, 1 if an error occurs
 */
extern int decode_pts_dts(byte data[], int required_guard, uint64_t *value);
/*
 * Encode a PTS or DTS.
 *
 * - `data` is the array of 5 bytes into which to encode the PTS/DTS
 * - `guard_bits` are the required guard bits: 2 for a PTS alone, 3 for
 *   a PTS before a DTS, or 1 for a DTS after a PTS
 * - `value` is the PTS or DTS value to be encoded
 */
extern void encode_pts_dts(byte data[], int guard_bits, uint64_t value);
/*
 * Does the given PES packet contain a PTS?
 *
 * - `packet` is the PES packet datastructure
 *
 * Returns TRUE if it does, FALSE if it does not (or is in error)
 */
extern int PES_packet_has_PTS(PES_packet_data_p packet);
/*
 * Report on the content of a PES packet - specifically, its header data.
 *
 * - `prefix` is a string to put before each line of output
 * - `data` is the packet data, and `data_len` its length
 * - `show_data` should be TRUE if the start of the data for each packet should
 *   be shown
 *
 * Returns 0 if all went well, 1 if an error occurs.
 */
extern int report_PES_data_array(char *prefix, byte *data, int data_len, int show_data);
/*
 * Report on the content of a PES packet.
 *
 * This gives a longer form of report than report_PES_data_array(), and
 * can also present substream data for audio stream_types.
 *
 * - `stream_type` is the stream type of the data, or -1 if it is not
 *   known (i.e., if this packet is from PS data).
 * - `payload` is the packet data.
 * - `payload_len` is the actual length of the payload (for a TS packet,
 *   this will generally be less than the PES packet's length).
 * - if `show_data_len` is non-0 then the data for the PES packet will
 *   also be shown, up to that length
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern void report_PES_data_array2(int stream_type, byte *payload, int payload_len,
                                   int show_data_len);

// ============================================================
// Server support
// ============================================================
/*
 * Packets can be written out to a client via a TS writer, as a
 * "side effect" of reading them. The original mechanism was to
 * write out PES packets (as TS) as they are read. This will work
 * for PS or TS data, and writes out only those PES packets that
 * have been read for video or audio data.
 *
 * An alternative, which will only work for TS input data, is
 * to write out TS packets as they are read. This will write all
 * TS packets to the client.
 *
 * - `reader` is our PES reader context
 * - `tswriter` is the TS writer
 * - if `write_PES`, then write PES packets out as they are read from
 *   the input, otherwise write TS packets.
 * - `program_freq` is how often to write out program data (PAT/PMT)
 *   if we are writing PES data (if we are writing TS data, then the
 *   program data will be in the original TS packets)
 */
extern void set_server_output(PES_reader_p reader, TS_writer_p tswriter, int write_PES,
                              int program_freq);
/*
 * Start packets being written out to a TS writer (again).
 *
 * If packets were already being written out, this does nothing.
 *
 * If set_server_output() has not been called to define a TS writer
 * context, this will have no effect.
 *
 * If `reader` is NULL, nothing is done.
 */
extern void start_server_output(PES_reader_p reader);
/*
 * Stop packets being written out to a TS writer.
 *
 * If packets were already not being written out, this does nothing.
 *
 * If set_server_output() has not been called to define a TS writer
 * context, this will have no effect.
 *
 * If `reader` is NULL, nothing is done.
 */
extern void stop_server_output(PES_reader_p reader);
/*
 * When outputting PES packets in "normal play" mode, add ``extra`` PES
 * packets (of the same size as each real packet) to the output. This
 * makes the amount of data output be about ``extra``+1 times the amount
 * read (the discrepancy is due to any program data being written).
 *
 * This "expansion" or "padding" of the data can be useful for benchmarking
 * the recipient, as the extra data (which has an irrelevant stream id)
 * will be ignored by the video processor, but not by preceding systems.
 *
 * This does nothing if TS packets are being output directly.
 *
 * - `reader` is our PES reader context
 * - `extra` is how many extra packets to output per "real" packet.
 */
extern void set_server_padding(PES_reader_p reader, int extra);
/*
 * Write out TS program data based on the information we have within the given
 * PES reader context (as amended by any calls of
 * `set_PES_reader_program_data`).
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int write_program_data(PES_reader_p reader, TS_writer_p output);

#endif // _pes_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

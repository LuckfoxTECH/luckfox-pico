/*
 * Functions for working with H.222 Transport Stream packets - in particular,
 * for writing PES packets.
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

#ifndef _ts_fns
#define _ts_fns

#include "compat.h"
#include "h222_defns.h"
#include "pidint_defns.h"
#include "ts_defns.h"
#include "tswrite_defns.h"

// ============================================================
// Writing a Transport Stream
// ============================================================

/*
 * Write out a Transport Stream PAT and PMT.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `transport_stream_id` is the id for this particular transport stream.
 * - `program_number` is the program number to use for the PID.
 * - `pmt_pid` is the PID for the PMT.
 * - `pid` is the PID of the stream to enter in the tables. This is also
 *    used as the PCR PID.
 * - `stream_type` is the type of stream. MPEG-2 video is 0x01,
 *   MPEG-4/AVC (H.264) is 0x1b.
 *
 * Since we're outputting a TS representing a single ES, we only need to
 * support a single program stream, containing a single PID.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_TS_program_data(TS_writer_p output, uint32_t transport_stream_id,
                                 uint32_t program_number, uint32_t pmt_pid, uint32_t pid,
                                 byte stream_type);

/*
 * Write out a Transport Stream PAT and PMT, for multiple streams.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `transport_stream_id` is the id for this particular transport stream.
 * - `program_number` is the program number to use for the PMT PID.
 * - `pmt_pid` is the PID for the PMT.
 * - `pcr_pid` is the PID that contains the PCR.
 * - `num_progs` is how many program streams are to be defined.
 * - `prog_pid` is an array of audio/video PIDs
 * - `prog_type` is an array of the corresponding stream types
 *
 * Note that if `num_progs` is 0, `pcr_pid` is ignored.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_TS_program_data2(TS_writer_p output, uint32_t transport_stream_id,
                                  uint32_t program_number, uint32_t pmt_pid, uint32_t pcr_pid,
                                  int num_progs, uint32_t prog_pid[], byte prog_type[]);

/*
 * Write out a Transport Stream PAT.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `transport_stream_id` is the id for this particular transport stream.
 * - `prog_list` is a PIDINT list of program number / PID pairs.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_pat(TS_writer_p output, uint32_t transport_stream_id, pidint_list_p prog_list);
/*
 * Write out a Transport Stream PMT, given a PMT datastructure
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `pmt_pid` is the PID for the PMT.
 * - 'pmt' is the datastructure containing the PMT information
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_pmt(TS_writer_p output, uint32_t pmt_pid, pmt_p pmt);
/*
 * Write out a Transport Stream PAT and PMT, given the appropriate
 * datastructures
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `transport_stream_id` is the id for this particular transport stream.
 * - `prog_list` is a PIDINT list of program number / PID pairs.
 * - `pmt_pid` is the PID for the PMT.
 * - 'pmt' is the datastructure containing the PMT information
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_pat_and_pmt(TS_writer_p output, uint32_t transport_stream_id,
                             pidint_list_p prog_list, uint32_t pmt_pid, pmt_p pmt);
/*
 * Write out a Transport Stream PAT, for a single program.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `transport_stream_id` is the id for this particular transport stream.
 * - `program_number` is the program number to use for the PID.
 * - `pmt_pid` is the PID for the PMT.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_single_program_pat(TS_writer_p output, uint32_t transport_stream_id,
                                    uint32_t program_number, uint32_t pmt_pid);
/*
 * Write out our ES data as a Transport Stream PES packet.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `data` is our ES data (e.g., a NAL unit)
 * - `data_len` is its length
 * - `pid` is the PID to use for this TS packet
 * - `stream_id` is the PES packet stream id to use (e.g.,
 *    DEFAULT_VIDEO_STREAM_ID)
 *
 * If the data to be written is more than 65535 bytes long (i.e., the
 * length will not fit into 2 bytes), then the PES packet written will
 * have PES_packet_length set to zero (see ISO/IEC 13818-1 (H.222.0)
 * 2.4.3.7, Semantic definitions of fields in PES packet). This is only
 * allowed for video streams.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_ES_as_TS_PES_packet(TS_writer_p output, byte data[], uint32_t data_len,
                                     uint32_t pid, byte stream_id);
/*
 * Write out our ES data as a Transport Stream PES packet, with PTS and/or DTS
 * if we've got them, and some attempt to write out a sensible PCR.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `data` is our ES data (e.g., a NAL unit)
 * - `data_len` is its length
 * - `pid` is the PID to use for this TS packet
 * - `stream_id` is the PES packet stream id to use (e.g.,
 *    DEFAULT_VIDEO_STREAM_ID)
 * - `got_pts` is TRUE if we have a PTS value, in which case
 * - `pts` is said PTS value
 * - `got_dts` is TRUE if we also have DTS, in which case
 * - `dts` is said DTS value.
 *
 * We also want to try to write out a sensible PCR value.
 *
 * PTS can go up as well as down (it is the time at which the next frame
 * should be presented to the user, but frames do not necessarily occur
 * in presentation order).
 *
 * DTS only goes up, since it is the time that the frame should be decoded.
 *
 * Thus, if we have it, the DTS is sensible to use for the PCR...
 *
 * If the data to be written is more than 65535 bytes long (i.e., the
 * length will not fit into 2 bytes), then the PES packet written will
 * have PES_packet_length set to zero (see ISO/IEC 13818-1 (H.222.0)
 * 2.4.3.7, Semantic definitions of fields in PES packet). This is only
 * allowed for video streams.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_ES_as_TS_PES_packet_with_pts_dts(TS_writer_p output, byte data[],
                                                  uint32_t data_len, uint32_t pid, byte stream_id,
                                                  int got_pts, uint64_t pts, int got_dts,
                                                  uint64_t dts);
/*
 * Write out our ES data as a Transport Stream PES packet, with PCR.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `data` is our ES data (e.g., a NAL unit)
 * - `data_len` is its length
 * - `pid` is the PID to use for this TS packet
 * - `stream_id` is the PES packet stream id to use (e.g.,
 *    DEFAULT_VIDEO_STREAM_ID)
 * - `pcr_base` and `pcr_extn` encode the PCR value.
 *
 * If the data to be written is more than 65535 bytes long (i.e., the
 * length will not fit into 2 bytes), then the PES packet written will
 * have PES_packet_length set to zero (see ISO/IEC 13818-1 (H.222.0)
 * 2.4.3.7, Semantic definitions of fields in PES packet). This is only
 * allowed for video streams.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_ES_as_TS_PES_packet_with_pcr(TS_writer_p output, byte data[], uint32_t data_len,
                                              uint32_t pid, byte stream_id, uint64_t pcr_base,
                                              uint32_t pcr_extn);
/*
 * Write out a PES packet's data as a Transport Stream PES packet.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 * - `data` is our PES data (e.g., a program stream video data packet)
 * - `data_len` is its length
 * - `pid` is the PID to use for this TS packet
 * - `stream_id` is the PES packet stream id to use (e.g.,
 *    DEFAULT_VIDEO_STREAM_ID)
 * - `got_pcr` is TRUE if we have values for the PCR in this packet,
 *   in which case `pcr_base` and `pcr_extn` are the parts of the PCR.
 *
 * If the data to be written is more than 65535 bytes long (i.e., the
 * length will not fit into 2 bytes), then the PES packet written will
 * have PES_packet_length set to zero (see ISO/IEC 13818-1 (H.222.0)
 * 2.4.3.7, Semantic definitions of fields in PES packet). This is only
 * allowed for video streams.
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_PES_as_TS_PES_packet(TS_writer_p output, byte data[], uint32_t data_len,
                                      uint32_t pid, byte stream_id, int got_pcr, uint64_t pcr_base,
                                      uint32_t pcr_extn);
/*
 * Write out a Transport Stream Null packet.
 *
 * - `output` is the TS output context returned by `tswrite_open`
 *
 * Returns 0 if it worked, 1 if something went wrong.
 */
extern int write_TS_null_packet(TS_writer_p output);

// ============================================================
// Reading a Transport Stream
// ============================================================
// ------------------------------------------------------------
// File handling
// ------------------------------------------------------------
/*
 * Build a TS packet reader, including its read-ahead buffer
 *
 * - `file` is the file that the TS packets will be read from
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int build_TS_reader(int file, TS_reader_p *tsreader);

/*
 * Build a TS packet reader using the given functions as read() and seek().
 *
 * Returns 0 on success, 1 on failure.
 */
extern int build_TS_reader_with_fns(void *handle, int (*read_fn)(void *, byte *, size_t),
                                    int (*seek_fn)(void *, offset_t), TS_reader_p *tsreader);

/*
 * Open a file to read TS packets from.
 *
 * If `filename` is NULL, then the input will be taken from standard input.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int open_file_for_TS_read(char *filename, TS_reader_p *tsreader);
/*
 * Free a TS packet read-ahead buffer
 *
 * Sets `buffer` to NULL.
 */
extern void free_TS_reader(TS_reader_p *tsreader);
/*
 * Free a TS packet read-ahead buffer and close the referenced file
 * (if it is not standard input).
 *
 * Sets `buffer` to NULL, whether the file close succeeds or not.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int close_TS_reader(TS_reader_p *tsreader);
/*
 * Seek to a given offset in the TS reader's file
 *
 * (This should be used in preference to just seeking on the "bare" file
 * since it also unsets the read-ahead buffer. However, it is still just
 * a wrapper around `seek_file`.)
 *
 * It is assumed (but not checked) that the seek will end up at an appropriate
 * offset for reading a TS packet - i.e., presumably some multiple of
 * TS_PACKET_SIZE.
 *
 * Returns 0 if all goes well, 1 if something goes wrong
 */
extern int seek_using_TS_reader(TS_reader_p tsreader, offset_t posn);
/*
 * Read the (rest of the) first TS packet, given its first four bytes
 *
 * This is intended for use after inspecting the first four bytes of the
 * input file, to determine if the file is TS or PS.
 *
 * - `tsreader` is the TS packet reading context
 * - `start` is the first four bytes of the file
 * - `packet` is (a pointer to) the resultant TS packet.
 *
 *   This is a pointer into the reader's read-ahead buffer, and so should not
 *   be freed. Note that this means that it may not persist after another call
 *   of this function (and will not persist after a call of
 *   `free_TS_reader`).
 *
 * Note that the caller is trusted to call this only when appropriate.
 *
 * Returns 0 if all goes well, EOF if end of file was read, or 1 if some
 * other error occurred (in which case it will already have output a message
 * on stderr about the problem).
 */
extern int read_rest_of_first_TS_packet(TS_reader_p tsreader, byte start[4], byte **packet);
/*
 * Read the next TS packet.
 *
 * - `tsreader` is the TS packet reading context
 * - `packet` is (a pointer to) the resultant TS packet.
 *
 *   This is a pointer into the reader's read-ahead buffer, and so should not
 *   be freed. Note that this means that it may not persist after another call
 *   of this function (and will not persist after a call of
 *   `free_TS_reader`).
 *
 * Returns 0 if all goes well, EOF if end of file was read, or 1 if some
 * other error occurred (in which case it will already have output a message
 * on stderr about the problem).
 */
extern int read_next_TS_packet(TS_reader_p tsreader, byte **packet);

// ------------------------------------------------------------
// Reading a transport stream with buffered timing
// Keeps a PCR in hand, so that it has accurate timing information
// for each TS packet
// ------------------------------------------------------------
/* Retrieve the first TS packet from the PCR read-ahead buffer,
 * complete with its calculated PCR time.
 *
 * This should be called the first time a TS packet is to be read
 * using the PCR read-ahead buffer. It "primes" the read-ahead mechanism.
 *
 * - `pcr_pid` is the PID within which we should look for PCR entries
 * - `start_count` is the index of the current (last read) TS entry (which will
 *   generally be the PMT).
 * - `data` returns a pointer to the TS packet data
 * - `pid` is its PID
 * - `pcr` is its PCR, calculated using the previous known PCR and
 *   the following known PCR.
 * - `count` is the index of the returned TS packet in the file
 *
 * Note that, like read_next_TS_packet, we return a pointer to our data,
 * and, similarly, warn that it will go away next time this function
 * is called.
 *
 * Returns 0 if all went well, 1 if something went wrong, EOF if EOF was read.
 */
extern int read_first_TS_packet_from_buffer(TS_reader_p tsreader, uint32_t pcr_pid,
                                            uint32_t start_count, byte *data[TS_PACKET_SIZE],
                                            uint32_t *pid, uint64_t *pcr, uint32_t *count);
/* Retrieve the next TS packet from the PCR read-ahead buffer,
 * complete with its calculated PCR time.
 *
 * - `data` returns a pointer to the TS packet data
 * - `pid` is its PID
 * - `pcr` is its PCR, calculated using the previous known PCR and
 *   the following known PCR.
 *
 * Note that, like read_next_TS_packet, we return a pointer to our data,
 * and, similarly, warn that it might go away next time this function
 * is called.
 *
 * Returns 0 if all went well, 1 if something went wrong, EOF if EOF was read.
 */
extern int read_next_TS_packet_from_buffer(TS_reader_p tsreader, byte *data[TS_PACKET_SIZE],
                                           uint32_t *pid, uint64_t *pcr);
/* Let the "looping" buffered TS packet reader know what its PCR PID is
 *
 * Call this before the first call of read_buffered_TS_packet().
 *
 * - `pcr_pid` is the PID within which we should look for PCR entries
 */
extern void prime_read_buffered_TS_packet(uint32_t pcr_pid);
/*
 * Read the next TS packet, coping with looping, etc.
 *
 * prime_read_buffered_TS_packet() should have been called first.
 *
 * This differs from ``read_TS_packet`` in that it assumes that the
 * underlying code will already have read to the next PCR, so that
 * it can know the *actual* (PCR-based) time for each TS packet.
 *
 * - `tsreader` is the TS reader context
 * - `count` is a running count of TS packets read from this input
 * - `data` is a pointer to the data for the packet
 * - `pid` is the PID of the TS packet
 * - `pcr` is the PCR value (possibly calculated) for this TS packet
 * - if `max` is greater than zero, then at most `max` TS packets should
 *   be read from the input
 * - if `loop`, play the input file repeatedly (up to `max` TS packets
 *   if applicable) - i.e., rewind to `start_posn` and start again if
 *   `count` reaches `max` (obviously only if `max` is greater than zero).
 * - `start_count` is the value `count` should have after we've looped back
 *   to `start_posn`
 * - if `quiet` is true, then only error messages should be written out
 *
 * Returns 0 if all went well, 1 if something went wrong, EOF if `loop` is
 * false and either EOF was read, or `max` TS packets were read.
 */
extern int read_buffered_TS_packet(TS_reader_p tsreader, uint32_t *count,
                                   byte *data[TS_PACKET_SIZE], uint32_t *pid, uint64_t *pcr,
                                   int max, int loop, offset_t start_posn, uint32_t start_count,
                                   int quiet);

// ------------------------------------------------------------
// Packet interpretation
// ------------------------------------------------------------
/*
 * Retrieve the PCR (if any) from a TS packet's adaptation field
 *
 * - `adapt` is the adaptation field content
 * - `adapt_len` is its length
 * - `got_PCR` is TRUE if the adaptation field contains a PCR
 * - `pcr` is then the PCR value itself
 */
extern void get_PCR_from_adaptation_field(byte adapt[], int adapt_len, int *got_pcr, uint64_t *pcr);
/*
 * Report on the contents of this TS packet's adaptation field
 *
 * - `adapt` is the adaptation field content
 * - `adapt_len` is its length
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern void report_adaptation_field(byte adapt[], int adapt_len);
/*
 * Report on the timing information from this TS packet's adaptation field
 *
 * - if `times` is non-NULL, then timing information (derived from the PCR)
 *   will be calculated and reported
 * - `adapt` is the adaptation field content
 * - `adapt_len` is its length
 * - `packet_count` is a count of how many TS packets up to now
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern void report_adaptation_timing(timing_p times, byte adapt[], int adapt_len, int packet_count);
/*
 * Report on the contents of this TS packet's payload. The packet is assumed
 * to have a payload that is (part of) a PES packet.
 *
 * - if `show_data` then the data for the PES packet will also be shown
 * - `stream_type` is the stream type of the data, or -1 if it is not
 *   known
 * - `payload` is the payload of the TS packet. We know it can't be more
 *   than 184 bytes long, because of the packet header bytes.
 * - regardless, `payload_len` is the actual length of the payload.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern void report_payload(int show_data, int stream_type, byte payload[MAX_TS_PAYLOAD_SIZE],
                           int payload_len, int payload_unit_start_indicator);
/*
 * Print out information about program descriptors
 * (either from the PMT program info, or the PMT/stream ES info)
 *
 * - `stream` is the stream to print on
 * - `leader1` and `leader2` are the text to write at the start of each line
 *   (either or both may be NULL)
 * - `desc_data` is the data containing the descriptors
 * - `desc_data_len` is its length
 *
 * Returns 0 if all went well, 1 if something went wrong
 */
extern int print_descriptors(FILE *stream, char *leader1, char *leader2, byte *desc_data,
                             int desc_data_len);
/*
 * Extract the program list from a PAT packet (PID 0x0000).
 *
 * Handles the result of calling build_psi_data() for this PAT.
 *
 * - if `verbose`, then report on what we're doing
 * - `payload` is the payload of the TS packet. We know it can't be more
 *   than 184 bytes long, because of the packet header bytes.
 * - regardless, `payload_len` is the actual length of the payload.
 * - `prog_list` is the list of program numbers versus PIDs.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int extract_prog_list_from_pat(int verbose, byte payload[MAX_TS_PAYLOAD_SIZE],
                                      int payload_len, pidint_list_p *prog_list);
/*
 * Extract the stream list (and PCR PID) from a PMT packet.
 *
 * Handles the result of calling build_psi_data() for this PMT.
 *
 * - if `verbose`, then report on what we're doing
 * - `payload` is the payload of the TS packet. We know it can't be more
 *   than 184 bytes long, because of the packet header bytes.
 * - regardless, `payload_len` is the actual length of the payload.
 * - `pid` is the PID of this TS packet.
 * - `program_number` is the program number.
 * - `pcr_pid` is the PID of packets containing the PCR, or 0.
 * - `stream_list` is a list of stream versus PID.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int extract_stream_list_from_pmt(int verbose, byte payload[MAX_TS_PAYLOAD_SIZE],
                                        int payload_len, uint32_t pid, int *program_number,
                                        uint32_t *pcr_pid, pidint_list_p *stream_list);
/*
 * Given a TS packet, extract the (next bit of) a PAT/PMT's data.
 *
 * - if `verbose`, then report on what we're doing
 * - `payload` is the payload of the current TS packet. We know it can't be
 *   more than 184 bytes long, because of the packet header bytes.
 * - regardless, `payload_len` is the actual length of the payload.
 * - `pid` is the PID of this TS packet.
 * - `data` is the data array for the whole of the data of this PSI.
 *   If it is passed as NULL, then the TS packet must be the first for
 *   this PSI, and this function will malloc an array of the appropriate
 *   length (and return it here). If it is non-NULL, then it is partially
 *   full.
 * - `data_len` is the actual length of the `data` array -- if `data` is NULL
 *   then this will be set by the function.
 * - `data_used` is how many bytes of data are already in the `data` array.
 *   This will be updated by this function - if it is returned as equal to
 *   `data_len`, then the PMT packet data is complete.
 *
 * Usage:
 *
 *  If a PSI packet has PUSI set, then it is the first packet of said PSI
 *  (which, for our purposes, means PAT or PMT). If it does not, then it
 *  is a continuation. If PUSI was set, call this with ``data`` NULL, otherwise
 *  pass it some previous data to continue.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int build_psi_data(int verbose, byte payload[MAX_TS_PAYLOAD_SIZE], int payload_len,
                          uint32_t pid, byte **data, int *data_len, int *data_used);
/*
 * Extract the program map table from a PMT packet.
 *
 * Assumes that the whole content of the PMT is in this single packet.
 *
 * - `data` is the data for the PMT packet.
 * - `data_len` is the length of said data.
 * - `pid` is the PID of this PMT
 * - `pmt` is the new PMT datastructure
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int extract_pmt(int verbose, byte data[], int data_len, uint32_t pid, pmt_p *pmt);
/*
 * Split a TS packet into its main parts
 *
 * - `buf` is the data for the packet
 * - `pid` is the PID of said data
 * - `payload_unit_start_indicator` is TRUE if any payload in this
 *   packet forms the start of a PES packet. Its meaning is not significant
 *   if there is no payload, or if the payload is not (part of) a PES packet.
 * - `adapt` is an offset into `buf`, acting as an array of the actual
 *   adaptation control bytes. It will be NULL if there are no adaptation
 *   controls.
 * - `adapt_len` is the length of the adaptation controls (i.e., the
 *   number of bytes). It will be 0 if there are no adaptation controls.
 * - `payload` is an offset into `buf`, acting as an array of the actual
 *   payload bytes. It will be NULL if there is no payload.
 * - `payload_len` is the length of the payload *in this packet* (i.e., the
 *   number of bytes. It will be 0 if there is no payload.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int split_TS_packet(byte buf[TS_PACKET_SIZE], uint32_t *pid,
                           int *payload_unit_start_indicator, byte *adapt[], int *adapt_len,
                           byte *payload[], int *payload_len);
/*
 * Return the next TS packet, as payload and adaptation controls.
 *
 * This is a convenience wrapping of `read_next_TS_packet` and
 * `split_TS_packet`. Because of this, the data referenced by `adapt` and
 * `payload` will generally not persist over further calls of this function
 * and `read_next_TS_packet`, as it is held within the TS reader's read-ahead
 * buffer.
 *
 * - `tsreader` is the TS packet reading context
 * - `pid` is the PID of said data
 * - `payload_unit_start_indicator` is TRUE if any payload in this
 *   packet forms the start of a PES packet. Its meaning is not significant
 *   if there is no payload, or if the payload is not (part of) a PES packet.
 * - `adapt` is an offset into `buf`, acting as an array of the actual
 *   adaptation control bytes. It will be NULL if there are no adaptation
 *   controls.
 * - `adapt_len` is the length of the adaptation controls (i.e., the
 *   number of bytes). It will be 0 if there are no adaptation controls.
 * - `payload` is an offset into `buf`, acting as an array of the actual
 *   payload bytes. It will be NULL if there is no payload.
 * - `payload_len` is the length of the payload *in this packet* (i.e., the
 *   number of bytes. It will be 0 if there is no payload.
 *
 * Returns 0 if all went well, EOF if there is no more data, 1 if something
 * went wrong.
 */
extern int get_next_TS_packet(TS_reader_p tsreader, uint32_t *pid,
                              int *payload_unit_start_indicator, byte *adapt[], int *adapt_len,
                              byte *payload[], int *payload_len);
/*
 * Find the first (next) PAT.
 *
 * - `tsreader` is the TS packet reading context
 * - if `max` is non-zero, then it is the maximum number of TS packets to read
 * - if `verbose` is true, then output extra information
 * - if `quiet` is true, then don't output normal informational messages
 * - `num_read` is the number of packets read to find the PAT (or before
 *   giving up)
 * - `prog_list` is the program list from the PAT, or NULL if none was found
 *
 * Returns 0 if all went well, EOF if no PAT was found,
 * 1 if something else went wrong.
 */
extern int find_pat(TS_reader_p tsreader, int max, int verbose, int quiet, int *num_read,
                    pidint_list_p *prog_list);
/*
 * Find the next PMT, and report on it.
 *
 * - `tsreader` is the TS packet reading context
 * - `pmt_pid` is the PID of the PMT we are looking for
 * - if `max` is non-zero, then it is the maximum number of TS packets to read
 * - if `verbose` is true, then output extra information
 * - if `quiet` is true, then don't output normal informational messages
 * - `num_read` is the number of packets read to find the PMT (or before
 *   giving up)
 * - `pmt` is a new datastructure representing the PMT found
 *
 * Returns 0 if all went well, EOF if no PMT was found,
 * 1 if something else went wrong.
 */
extern int find_next_pmt(TS_reader_p tsreader, uint32_t pmt_pid, int max, int verbose, int quiet,
                         int *num_read, pmt_p *pmt);
/*
 * Find the next PAT, and from that the next PMT.
 *
 * Looks for the next PAT in the input stream, and then for the first
 * PMT thereafter. If there is more than one program stream in the PAT,
 * it looks for the PMT for the first.
 *
 * - `tsreader` is the TS packet reading context
 * - if `max` is non-zero, then it is the maximum number of TS packets to read
 * - if `verbose` is true, then output extra information
 * - if `quiet` is true, then don't output normal informational messages
 * - `num_read` is the number of packets read to find the PMT (or before
 *   giving up)
 * - `pmt` is a new datastructure containing the information from the PMT.
 *
 * Returns 0 if all went well, EOF if no PAT or PMT was found (and thus
 * no program stream), -2 if a PAT was found but it did not contain any
 * programs, 1 if something else went wrong.
 */
extern int find_pmt(TS_reader_p tsreader, int max, int verbose, int quiet, int *num_read,
                    pmt_p *pmt);

#endif // _ts_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

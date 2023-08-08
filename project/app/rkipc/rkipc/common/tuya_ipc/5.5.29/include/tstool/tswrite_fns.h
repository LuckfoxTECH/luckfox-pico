/*
 * Support for writing out TS packets, to file, or over TCP/IP or UDP
 *
 * When writing asynchronously, provides automated producer/consumer
 * behaviour via a circular buffer, optionally taking timing from the
 * TS PCR entries.
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

#ifndef _tswrite_fns
#define _tswrite_fns

#include "tswrite_defns.h"

/*
 * Open a file for TS output.
 *
 * - `how` is how to open the file or connect to the host
 * - `name` is the name of the file or host to open/connect to
 *   (this is ignored if `how` is TS_W_STDOUT)
 * - if `how` is TS_W_UDP, and `name` is a multicast address,
 *   then `multicast_if` may be the IP address of the network
 *   address to use, or NULL if the default interface should
 *   be used.
 * - if it is a socket (i.e., if `how` is TS_W_TCP or TS_W_UDP),
 *   then `port` is the port to use, otherwise this is ignored
 * - `quiet` is true if only error messages should be printed
 * - `tswriter` is the new context to use for writing TS output,
 *   which should be closed using `tswrite_close`.
 *
 * For TS_W_STDOUT, there is no need to open anything.
 *
 * For TS_W_FILE, ``open(name,O_CREAT|O_WRONLY|O_TRUNC|O_BINARY,00777)``
 * is used - i.e., the file is opened so that anyone may read/write/execute
 * it. If ``O_BINARY`` is not defined (e.g., on Linux), then it is
 * omitted.
 *
 * For TS_W_TCP and TS_W_UDP, the ``connect_socket`` function is called,
 * which uses ``socket`` and ``connect``.
 *
 * In all cases (even when using TS_W_STDOUT), the `tswriter` should be
 * closed using `tswrite_stdout`.
 *
 * For TS_W_UDP, the ``tswrite_startt_buffering`` function must be called
 * before any output is written via the `tswriter`. For other forms of output,
 * this is optional.
 *
 * Returns 0 if all goes well, 1 if something went wrong.
 */
extern int tswrite_open(TS_WRITER_TYPE how, char *name, char *multicast_if, int port, int quiet,
                        TS_writer_p *tswriter);
/*
 * Open a network connection for TS output.
 *
 * This is a convenience wrapper around `tswrite_open`.
 *
 * - `name` is the name of the host to connect to
 * - `port` is the port to connect to
 * - `use_tcp` is TRUE if TCP/IP should be use, FALSE if UDP should be used
 * - `quiet` is true if only error messages should be printed
 * - `tswriter` is the new context to use for writing TS output,
 *   which should be closed using `tswrite_close`.
 *
 * In all cases, the `tswriter` should be closed using `tswrite_stdout`.
 *
 * For TS_W_UDP, the ``tswrite_start_buffering`` function must be called
 * before any output is written via the `tswriter`. For other forms of output,
 * this is optional.
 *
 * Returns 0 if all goes well, 1 if something went wrong.
 */
extern int tswrite_open_connection(int use_tcp, char *name, int port, int quiet,
                                   TS_writer_p *tswriter);
/*
 * Open a file for TS output.
 *
 * This is a convenience wrapper around `tswrite_open`.
 *
 * - `name` is the name of the file to open, or NULL if stdout should be used
 * - `quiet` is true if only error messages should be printed
 * - `tswriter` is the new context to use for writing TS output,
 *   which should be closed using `tswrite_close`.
 *
 * In all cases (even when using TS_W_STDOUT), the `tswriter` should be
 * closed using `tswrite_stdout`.
 *
 * Returns 0 if all goes well, 1 if something went wrong.
 */
extern int tswrite_open_file(char *name, int quiet, TS_writer_p *tswriter);
/*
 * Wait for a client to connect and then both write TS data to it and
 * listen for command from it. Uses TCP/IP.
 *
 * - `server_socket` is the socket on which we will listen for a connection
 * - `quiet` is true if only error messages should be printed
 * - `tswriter` is the new context to use for writing TS output,
 *   which should be closed using `tswrite_close`.
 *
 * Returns 0 if all goes well, 1 if something went wrong.
 */
extern int tswrite_wait_for_client(int server_socket, int quiet, TS_writer_p *tswriter);
/*
 * Set up internal buffering for TS output. This is necessary for UDP
 * output, and optional otherwise.
 *
 * Builds the internal circular buffer and other datastructures, and
 * forks a child proces to send data over the socket.
 *
 * (This is *intended* for use when outputting via a socket, but there
 * is nothing actually stopping it from being used to output to a file.
 * This is unlikely to be useful for other than testing purposes, however.)
 *
 * See also `tswrite_start_buffering_from_context`, which uses the `context`
 * datastructure that is prepared by `tswrite_process_args`.
 *
 * - `tswriter` is the TS output context returned by `tswrite_open`
 * - `circ_buf_size` is the number of buffer entries (plus one) we would
 *   like in the underlying circular buffer.
 * - `TS_in_packet` is the number of TS packets to allow in each network
 *   packet.
 * - `maxnowait` is the maximum number of packets to send to the target
 *   host with no wait between packets
 * - `waitfor` is the number of microseconds to wait for thereafter
 * - `byterate` is the (initial) rate at which we'd like to output our data
 * - `use_pcrs` is TRUE if PCRs in the data stream are to be used for
 *   timing output (the normal case), otherwise the specified byte rate
 *   will be used directly.
 * - `prime_size` is how much to prime the circular buffer output timer
 * - `prime_speedup` is the percentage of "normal speed" to use for the priming
 *   rate. This should normally be set to 100 (i.e., no effect).
 * - `pcr_scale` determines how much to "accelerate" each PCR - see the
 *   notes elsewhere on how this works.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int tswrite_start_buffering(TS_writer_p tswriter, int circ_buf_size, int TS_in_packet,
                                   int maxnowait, int waitfor, int byterate, int use_pcrs,
                                   int prime_size, int prime_speedup, double pcr_scale);
/*
 * Set up internal buffering for TS output. This is necessary for UDP output,
 * and optional otherwise.
 *
 * This alternative takes the `context` datastructure that is prepared
 * by `tswrite_process_args`.
 *
 * - `tswriter` is the TS output context returned by `tswrite_open`
 * - `context` contains the necessary information, as given by the user
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int tswrite_start_buffering_from_context(TS_writer_p tswriter, TS_context_p context);
/*
 * Indicate to a TS output context that `input` is to be used as
 * command input.
 *
 * This function may only be used if output is via TCP/IP.
 *
 * - `tswriter` is the TS output context returned by `tswrite_open`
 * - `input` is the socket (or, on Linux/BSD, file descriptor) on which
 *   to listen for commands.
 *
 *   Note that this should either be ``tswriter->where.socket`` or
 *   STDIN_FILENO - no other values are currently supported (particularly
 *   since no attempt is made to close this socket when things are finished,
 *   which doesn't matter for the given values).
 *
 * This function:
 *
 * - makes the socket on which data will be written non-blocking
 *   (i.e., if the socket is not ready to be written to, it will not
 *   accept input and block until it can be used, which means that it
 *   becomes our responsibility to ask if the socket is ready for output)
 * - makes tswrite_write "look" on the `input` to see if a (single
 *   character) command has been given, and if it has, put it into
 *   the `tswriter` datastructure for use
 *
 * The command state is set to 'p'ause - i.e., as if the client had sent
 * a COMMAND_PAUSE command.
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int tswrite_start_input(TS_writer_p tswriter, SOCKET input);
/*
 * Set/unset "atomic" status - i.e., whether a command may be interrupted
 * by the next command.
 *
 * Most commands (normal play, fast forwards, etc.) should be interrupted
 * by a new command. However, some (the skip forwards and backwards commands)
 * make sense only if they will always complete. This function allows that
 * state to be toggled.
 */
extern void tswrite_set_command_atomic(TS_writer_p tswriter, int atomic);
/*
 * Ask a TS writer if changed input is available.
 *
 * If the TS writer is enabled for command input, then if the command
 * currently being executed has declared itself "atomic" (i.e., not able to be
 * interrupted), it returns FALSE, otherwise it returns TRUE if the command
 * character has changed.
 */
extern int tswrite_command_changed(TS_writer_p tswriter);
/*
 * Close a file or socket opened using `tswrite_open`, and if necessary,
 * send the child process used for output buffering an end-of-file
 * indicator, and wait for it to finish.
 *
 * - `tswriter` is the TS output context returned by `tswrite_open`
 * - if `quiet` is true, then waiting for the child to exit should
 *   not be reported on (i.e., only errors should produce output)
 *
 * Returns 0 if all goes well, 1 if something went wrong.
 */
extern int tswrite_close(TS_writer_p tswriter, int quiet);

/*
 * Wait for a new command after 'p'ausing.
 *
 * - `tswriter` is the TS output context returned by `tswrite_open`
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int wait_for_command(TS_writer_p tswriter);

/*
 * Write a Transport Stream packet out via the TS writer.
 *
 * - `tswriter` is the TS output context returned by `tswrite_open`
 * - `packet` is the TS packet
 * - if the packets payload_unit_start_indicator is set, then
 *   `pid` is the PID for this packet, `got_pcr` is TRUE if it
 *   contains a PCR in its adaptation field, and `pcr` contains
 *   said PCR. These values are only used when outputting via
 *   buffered output.
 *
 * Returns 0 if all goes well, 1 if something went wrong, and EOF if command
 * input is enabled (only allowed for TCP/IP output) and the 'q'uit command
 * has been given (in which case, no further commands will be read, and no
 * more output will be written, by any subsequent calls of this function).
 */
extern int tswrite_write(TS_writer_p tswriter, byte packet[TS_PACKET_SIZE], uint32_t pid,
                         int got_pcr, uint64_t pcr);

/*
 * Write a usage string (to standard output) describing the tuning
 * options processed by tswrite_process_args.
 */
extern void tswrite_help_tuning();
/*
 * Write a usage string (to standard output) describing the testing
 * options processed by tswrite_process_args.
 */
extern void tswrite_help_testing();
/*
 * Write a usage string (to standard output) describing the
 * debugging options processed by tswrite_process_args.
 */
extern void tswrite_help_debug();
/*
 * Report on the values within our argument context.
 *
 * Also reports on the various global/debug values.
 *
 * Note that it is up to the caller to ensure that they *use* all
 * the values reported on here!
 */
extern void tswrite_report_args(TS_context_p context);
/*
 * Various command line switches that are useful for tswrite are really
 * only interpretable by tswrite itself. Thus we provide a function that
 * will process such switches.
 *
 * This function extracts appropriate switches from `argv`, and returns it
 * altered appropriately.
 *
 * - `prefix` is a prefix for any error messages - typically the
 *   short name of the program running.
 * - `argc` and `argv` are as passed to `main`. After
 *   this function has finished, any arguments that it has processed will have
 *   had their `argv` array elements changed to point to the string
 *   "<processed>" (this is defined as the string TSWRITE_PROCESSED in the
 *   tswrite.h header file).
 * - values are set in `context` to indicate the user's requests,
 *   and also any appropriate defaults.
 *
 * Note that `tswrite_print_usage` may be used to print out a description of
 * the switches processed by this function.
 *
 * Returns 0 if all goes well, 1 if there was an error. Note that not
 * specifying an output file or host counts as an error.
 */
extern int tswrite_process_args(char *prefix, int argc, char *argv[], TS_context_p context);

#endif // _tswrite_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

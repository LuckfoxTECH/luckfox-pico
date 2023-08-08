/*
 * Miscellaneous useful functions.
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
#ifndef _misc_fns
#define _misc_fns

#include "es_defns.h"
#include "misc_defns.h"
#ifndef _WIN32
#include <stdint.h>
#endif

#ifdef _WIN32
#include "compat.h"
#include <winsock2.h>
#endif // _WIN32

#define CRC32_POLY 0x04c11db7L

/*
 * Compute CRC32 over a block of data, by table method.
 *
 * Returns a working value, suitable for re-input for further blocks
 *
 * Notes: Input value should be 0xffffffff for the first block,
 *        else return value from previous call (not sure if that
 *        needs complementing before being passed back in).
 */
extern uint32_t crc32_block(uint32_t crc, byte *pData, int blk_len);

/*
 * Print out the bottom N bits from a byte on the given stream
 */
extern void print_bits(FILE *stream, int num_bits, byte value);

/*
 * Print out (the first `max`) bytes of a byte array.
 *
 * - `stream` is the stream to print on.
 * - `name` is identifying text to start the report with.
 * - `data` is the byte data to print. This may be NULL.
 * - `length` is its length
 * - `max` is the maximum number of bytes to print
 *
 * Prints out::
 *
 *    <name> (<length>): b1 b2 b3 b4 ...
 *
 * where no more than `max` bytes are to be printed (and "..." is printed
 * if not all bytes were shown).
 */
extern void print_data(FILE *stream, char *name, byte data[], int length, int max);
/*
 * Print out (the last `max`) bytes of a byte array.
 *
 * - `stream` is the stream to print on.
 * - `name` is identifying text to start the report with.
 * - `data` is the byte data to print. This may be NULL.
 * - `length` is its length
 * - `max` is the maximum number of bytes to print
 *
 * Prints out::
 *
 *    <name> (<length>): ... b1 b2 b3 b4
 *
 * where no more than `max` bytes are to be printed (and "..." is printed
 * if not all bytes were shown).
 */
extern void print_end_of_data(FILE *stream, char *name, byte data[], int length, int max);
/*
 * Calculate log2 of `x` - for some reason this is missing from <math.h>
 */
extern double log2(double x);

// ============================================================
// Simple file I/O utilities
// ============================================================
/*
 * Read a given number of bytes from a file.
 *
 * This is a jacket for `read`, allowing for the future possibility of
 * buffered input, and simplifying error handling.
 *
 * - `input` is the file descriptor for the file
 * - `num_bytes` is how many bytes to read
 * - `data` is the buffer to read the bytes into
 *
 * Returns 0 if all goes well, EOF if end of file was read, or 1 if some
 * other error occurred (in which case it will already have output a message
 * on stderr about the problem).
 */
extern int read_bytes(int input, int num_bytes, byte *data);
/*
 * Utility function to seek within a file
 *
 * - `filedes` is the file to seek within
 * - `posn` is the position to which to seek
 *
 * This is a jacket for::
 *
 *    new_posn = lseek(filedes,posn,SEEK_SET);
 *
 * Returns 0 if all went well, 1 if the seek failed (either because
 * it returned -1, or because the position reached was not the position
 * requested). If an error occurs, then an explanatory message will
 * already have been written to stderr.
 */
extern int seek_file(int filedes, offset_t posn);
/*
 * Utility function to report the current location within a file
 *
 * - `filedes` is the file to seek within
 *
 * This is a jacket for::
 *
 *    posn = lseek(filedes,0,SEEK_CUR);
 *
 * Returns the current position in the file if all went well, otherwise
 * -1 (in which case an error message will already have been written
 * on stderr)
 */
extern offset_t tell_file(int filedes);
/*
 * Utility function to open a file (descriptor), and report any errors
 *
 * This is intended only for very simple usage, and is not mean to be
 * a general purpose "open" replacement.
 *
 * - `filename` is the name of the file to open
 * - `for_write` should be TRUE if the file is to be written to,
 *   in which case it will be opened with flags O_WRONLY|O_CREAT|O_TRUNC,
 *   or FALSE if the file is to be read, in which case it will be
 *   opened with flag O_RDONLY. In both cases, on Windows the flag
 *   O_BINARY will also be set.
 *
 * Returns the file descriptor for the file, or -1 if it failed to open
 * the file.
 */
extern int open_binary_file(char *filename, int for_write);
/*
 * Utility function to close a file (descriptor), and report any errors
 *
 * Returns 0 if all went well, 1 if an error occurred.
 */
extern int close_file(int filedes);

// ============================================================
// More complex file I/O utilities
// ============================================================
/*
 * Open an input file appropriately for reading as ES.
 *
 * - `name` is the name of the file, or NULL if standard input
 *   is to be read from (which is not allowed if `use_pes` is
 *   TRUE).
 *
 * - If `use_pes` is true then the input file is PS or TS and should
 *   be read via a PES reader.
 *
 * - If `quiet` is true then information about the file being read will
 *   not be written out. Otherwise, its name and what is decided about
 *   its content will be printed.
 *
 * - If `force_stream_type` is true, then the caller asserts that
 *   the input shall be read according to `want_data`, and not whatever
 *   might be deduced from looking at the file itself.
 *
 * - If `force_stream_type` is true, then `want_data` should be one of
 *   VIDEO_H262, VIDEO_H264 or VIDEO_AVS. `is_data` will then be
 *   returned with the same value.
 *
 * - If `force_stream_type` is false, then the function will attempt
 *   to determine what type of data it has, and `is_data` will be set
 *   to whatever is determined (presumably one of VIDEO_H262, VIDEO_H264
 *   or VIDEO_AVS).
 *
 * - If input is from standard input, and `force_stream_type` is FALSE,
 *   `is_data` will always be set to VIDEO_H262, which may be incorrect.
 *
 * - `es` is the new ES reader context.
 *
 * Returns 0 if all goes well, 1 if something goes wrong. In the latter case,
 * suitable messages will have been written out to standard error.
 */
extern int open_input_as_ES(char *name, int use_pes, int quiet, int force_stream_type,
                            int want_data, int *is_data, ES_p *es);
/*
 * Close an input ES stream opened with `open_input_as_ES`.
 *
 * Specifically, this will close the ES stream and also any underlying PES
 * reader and file (unless the input was standard input).
 *
 * - `name` is the name of the file, used for error reporting.
 * - `es` is the ES stream to close. This will be set to NULL.
 *
 * Returns 0 if all goes well, 1 if something goes wrong. In the latter case,
 * suitable messages will have been written out to standard error.
 */
extern int close_input_as_ES(char *name, ES_p *es);

// ============================================================
// Command line "helpers"
// ============================================================
/*
 * Read in an unsigned integer value, checking for extraneous characters.
 *
 * - `prefix` is an optional prefix for error messages, typically the
 *   name of the program. It may be NULL.
 * - `cmd` is the command switch we're reading for (typically ``argv[ii]``),
 *   which is used in error messages.
 * - `str` is the string to read (typically ``argv[ii+1]``).
 * - `base` is the base to read to. If it is 0, then the user can use
 *   C-style expressions like "0x68" to specify the base on the command line.
 * - `value` is the value read.
 *
 * Returns 0 if all went well, 1 otherwise (in which case a message
 * explaining will have been written to stderr).
 */
extern int unsigned_value(char *prefix, char *cmd, char *arg, int base, uint32_t *value);
/*
 * Read in an integer value, checking for extraneous characters.
 *
 * - `prefix` is an optional prefix for error messages, typically the
 *   name of the program. It may be NULL.
 * - `cmd` is the command switch we're reading for (typically ``argv[ii]``),
 *   which is used in error messages.
 * - `str` is the string to read (typically ``argv[ii+1]``).
 * - if `positive` is true, then the number read must be positive (0 or more).
 * - `base` is the base to read to. If it is 0, then the user can use
 *   C-style expressions like "0x68" to specify the base on the command line.
 * - `value` is the value read.
 *
 * Returns 0 if all went well, 1 otherwise (in which case a message
 * explaining will have been written to stderr).
 */
extern int int_value(char *prefix, char *cmd, char *str, int positive, int base, int *value);
/*
 * Read in an integer value, checking for extraneous characters and a range.
 *
 * - `prefix` is an optional prefix for error messages, typically the
 *   name of the program. It may be NULL.
 * - `cmd` is the command switch we're reading for (typically ``argv[ii]``),
 *   which is used in error messages.
 * - `str` is the string to read (typically ``argv[ii+1]``).
 * - `minimum` is the minimum value allowed.
 * - `maximum` is the maximum value allowed.
 * - `base` is the base to read to. If it is 0, then the user can use
 *   C-style expressions like "0x68" to specify the base on the command line.
 * - `value` is the value read.
 *
 * Returns 0 if all went well, 1 otherwise (in which case a message
 * explaining will have been written to stderr).
 */
extern int int_value_in_range(char *prefix, char *cmd, char *arg, int minimum, int maximum,
                              int base, int *value);
/*
 * Read in a double value, checking for extraneous characters.
 *
 * - `prefix` is an optional prefix for error messages, typically the
 *   name of the program. It may be NULL.
 * - `cmd` is the command switch we're reading for (typically ``argv[ii]``),
 *   which is used in error messages.
 * - `str` is the string to read (typically ``argv[ii+1]``).
 * - if `positive` is true, then the number read must be positive (0 or more).
 * - `value` is the value read.
 *
 * Returns 0 if all went well, 1 otherwise (in which case a message
 * explaining will have been written to stderr).
 */
extern int double_value(char *prefix, char *cmd, char *arg, int positive, double *value);
/*
 * Read in a hostname and (optional) port
 *
 * - `prefix` is an optional prefix for error messages, typically the
 *   name of the program. It may be NULL.
 * - `cmd` is the command switch we're reading for (typically ``argv[ii]``),
 *   which is used in error messages.
 * - `arg` is the string to read (typically ``argv[ii+1]``).
 * - `hostname` is the host name read
 * - `port` is the port read (note that this is not touched if there is
 *   no port number, so it may be set to a default before calling this
 *   function)
 *
 * Note that this works by pointing `hostname` to the start of the `arg`
 * string, and then if there is a ':' in `arg`, changing that colon to
 * a '\0' delimiter, and interpreting the string thereafter as the port
 * number. If *that* fails, it resets the '\0' as a ':'.
 *
 * Returns 0 if all went well, 1 otherwise (in which case a message
 * explaining will have been written to stderr).
 */
extern int host_value(char *prefix, char *cmd, char *arg, char **hostname, int *port);

// ============================================================
// Sockets
// ============================================================
#ifdef _WIN32
/*
 * Start up WINSOCK so we can use sockets.
 *
 * Note that each successful call of this *must* be matched by a call
 * of winsock_cleanup().
 *
 * Returns 0 if it works, 1 if it fails.
 */
extern int winsock_startup();
/*
 * Convert a WinSock error number into a string and print it out on stderr
 */
extern void print_winsock_err(int err);
#endif // _WIN32
/*
 * Connect to a socket, to allow us to write to it, using TCP/IP.
 *
 * - `hostname` is the name of the host to connect to
 * - `port` is the port to use
 * - if `use_tcpip`, then a TCP/IP connection will be made, otherwise UDP.
 *   For UDP, multicast TTL will be enabled.
 * - If the destination address (`hostname`) is multicast and `multicast_ifaddr`
 *   is supplied, it is used to select (by IP address) the network interface
 *   on which to send the multicasts.  It may be NULL to use the default,
 *   or for non-multicast cases.
 *
 * A socket connected to via this function must be disconnected from with
 * disconnect_socket().
 *
 * Returns a positive integer (the file descriptor for the socket) if it
 * succeeds, or -1 if it fails, in which case it will have complained on
 * stderr.
 */
extern int connect_socket(char *hostname, int port, int use_tcpip, char *multicast_ifaddr);

/*
 * Disconnect from a socket (close it).
 *
 * Returns 0 if all goes well, 1 otherwise.
 */
#ifdef _WIN32
extern int disconnect_socket(SOCKET socket);
#else  // _WIN32
extern int disconnect_socket(int socket);
#endif // _WIN32

/*! Decode a host byte order address to a static buffer. */
const char *ipv4_addr_to_string(const uint32_t addr);

/*! Decode a string to a host byte order address */
int ipv4_string_to_addr(uint32_t *dest, const char *addr);

// ============================================================
// Byte order handling
// ============================================================

static inline uint32_t uint_32_be(const uint8_t *const p) {
	return (((int)p[0] & 0xff) << 24) | (((int)p[1] & 0xff) << 16) | (((int)p[2] & 0xff) << 8) |
	       (((int)p[3] & 0xff));
}

static inline uint32_t uint_32_le(const uint8_t *const p) {
	return (((int)p[0] & 0xff) | (((int)p[1] & 0xff) << 8) | (((int)p[2] & 0xff) << 16) |
	        (((int)p[3] & 0xff) << 24));
}

static inline uint16_t uint_16_be(const uint8_t *const p) {
	return ((((int)p[0]) & 0xff) << 8) | ((((int)p[1]) & 0xff));
}

static inline uint16_t uint_16_le(const uint8_t *const p) {
	return (((int)p[0] & 0xff) | ((int)p[1] & 0xff) << 8);
}

#endif // _misc_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

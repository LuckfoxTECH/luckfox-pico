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

#ifndef _tswrite_defns
#define _tswrite_defns

#include "compat.h"
#include "h222_defns.h"
#include "ts_defns.h"

#ifdef _WIN32
#include <winsock2.h> // for definition of SOCKET
#else
typedef int SOCKET; // for compatibility with Windows
#include <termios.h> // for struct termios
#endif

// ============================================================
// CIRCULAR BUFFER
// ============================================================

// We default to using a "packet" of 7 transport stream packets because 7*188 =
// 1316, but 8*188 = 1504, and we would like to output as much data as we can
// that is guaranteed to fit into a single ethernet packet, size 1500.
#define DEFAULT_TS_PACKETS_IN_ITEM 7

// For simplicity, we'll have a maximum on that (it allows us to have static
// array sizes in some places). This should be a big enough size to more than
// fill a jumbo packet on a gigabit network.
#define MAX_TS_PACKETS_IN_ITEM 100

// ------------------------------------------------------------
// A circular buffer, usable as a queue
//
// We "waste" one buffer item so that we don't have to maintain a count
// of items in the buffer
//
// To get an understanding of how it works, choose a small BUFFER_SIZE
// (e.g., 11), enable DISPLAY_BUFFER, and select --visual - this will show the
// reading/writing of the circular buffer in action, including the
// "unused item".
//
// The data for the circular buffer
// Each circular buffer item "contains" (up to) N TS packets (where N defaults
// to 7, and is specified as `item_size` in the circular buffer header), and a
// time (in microseconds) when we would like it to be output (relative to the
// time for the first packet "sent").
//
// Said data is stored at the address indicated by the circular buffer
// "header", as `item_data`.
//
struct circular_buffer_item {
	uint32_t time;     // when we would like this data output
	int discontinuity; // TRUE if our timeline has "broken"
	int length;        // number of bytes of data in the array
};
typedef struct circular_buffer_item *circular_buffer_item_p;

#define SIZEOF_CIRCULAR_BUFFER_ITEM sizeof(struct circular_buffer_item)

// ------------------------------------------------------------
// The header for the circular buffer
//
// Note that `start` is only ever written to by the child process, and this is
// the only thing that the child process ever changes in the circular buffer.
//
// `maxnowait` is the maximum number of packets to send to the target host
// without forcing an intermediate wait - required to stop us "swamping" the
// target with too much data, and overrunning its buffers.
struct circular_buffer {
	int start; // start of data "pointer"
	int end;   // end of data "pointer" (you guessed)
	int size;  // the actual length of the `item` array

	int TS_in_item; // max number of TS packets in a circular buffer item
	int item_size;  // and thus the size of said item's data array

	int maxnowait; // max number consecutive packets to send with no wait
	int waitfor;   // the number of microseconds to wait thereafter

	// The location of the packet data for the circular buffer items
	byte *item_data;

	// The "header" data for each circular buffer item
	struct circular_buffer_item item[];
};
typedef struct circular_buffer *circular_buffer_p;

// Note that size doesn't include the final `item`
#define SIZEOF_CIRCULAR_BUFFER sizeof(struct circular_buffer)

#define DEFAULT_CIRCULAR_BUFFER_SIZE 1024 // used to be 100

// ============================================================
// BUFFERED OUTPUT
// ============================================================

// Information about each TS packet in our circular buffer item
struct TS_packet_info {
	int index;
	uint32_t pid; // do we need the PIDs?
	int got_pcr;
	uint64_t pcr;
};
typedef struct TS_packet_info *TS_packet_info_p;
#define SIZEOF_TS_PACKET_INFO sizeof(struct TS_packet_info);

// If we're going to support output via our circular buffer in a manner
// similar to that for output to a file or socket, then we need a structure
// to maintain the relevant information. It seems a bit wasteful to burden
// the circular buffer itself with this, particularly as only the writer
// cares about this data, so it needn't be shared.
struct buffered_TS_output {
	circular_buffer_p buffer;
	int which;   // Which buffer index we're writing to
	int started; // TRUE if we've started writing therein

	// For each TS packet in the circular buffer, remember its `count`
	// within the input stream, whether it had a PCR, and if so what that
	// PCR was. To make it simpler to access these arrays, also keep a fill
	// index into them (the alternative would be to always re-zero the
	// `got_pcr` values whenever we start a new circular buffer entry,
	// which would be a pain...)
	int num_packets; // how many TS packets we've got
	struct TS_packet_info packet[MAX_TS_PACKETS_IN_ITEM];

	// `rate` is the rate (in bytes per second) we would like to output data at
	uint32_t rate;

	// `pcr_scale` is a multiplier for PCRs - each PCR found gets its value
	// multiplied by this
	double pcr_scale;

	// `use_pcrs` indicates if we should use PCRs in the data to drive our
	// timing, rather than use the specified byte rate directly. The `priming`
	// values are only relevant if `use_pcrs` is true.
	int use_pcrs;

	// 'prime_size' is the amount of space/time to 'prime' the circular buffer
	// output timing mechanism with. This is effectively multiples of the
	// size of a circular buffer item.
	int prime_size;

	// Percentage "too fast" speedup for our priming rate
	int prime_speedup;
};
typedef struct buffered_TS_output *buffered_TS_output_p;
#define SIZEOF_BUFFERED_TS_OUTPUT sizeof(struct buffered_TS_output)

// ============================================================
// EXTERNAL DATASTRUCTURES - these are *intended* for external use
// ============================================================

// Our supported target types
// On Unix-type systems, there is little distinction between file and
// socket, but on Windows this becomes more interesting
enum TS_writer_type {
	TS_W_UNDEFINED,
	TS_W_STDOUT, // standard output
	TS_W_FILE,   // a file
	TS_W_TCP,    // a socket, over TCP/IP
	TS_W_UDP,    // a socket, over UDP
};
typedef enum TS_writer_type TS_WRITER_TYPE;

// ------------------------------------------------------------
// So, *is* it a file or a socket?
union TS_writer_output {
	FILE *file;
	SOCKET socket;
};

// ------------------------------------------------------------
// A datastructure to allow us to write to various different types of target
//
// When writing to a file, "how" will be TS_W_STDOUT or TS_W_FILE, and
// "where" will be the appropriate file interface. "writer" is not necessary
// (there's no point in putting a circular buffer and other stuff above
// the file writes), and no child process is needed.
//
// When writing over UDP, "how" will be TS_W_UDP, and "where" will be the
// socket that is being written to. For UDP, timing needs to be managed, and
// thus the circular buffer support is necessary, so "writer" should be
// set to a buffered output context. Since the circular buffer is being
// used, there will also be a child process.
//
// When writing over TCP/IP, "how" will be TS_W_TCP, and "where" will be the
// socket that is being written to. Timing is not an issue, so "writer" will
// not be needed, and nor will there be a child process.  However, it is
// possible that we will want to respond to commands (over the same or another
// socket (or, on Linux/BSD, file descriptor)), so "commander" may be set.
struct TS_writer {
	enum TS_writer_type how;      // what type of output we want
	union TS_writer_output where; // where it's going to
	buffered_TS_output_p writer;  // our buffered output interface, if needed
	int count;                    // a count of how many TS packets written

	// Support for the child fork/thread, which actually does the writing when
	// buffered output is enabled.
#ifdef _WIN32
	HANDLE child; // the handle for the child thread (if any)
#else             // _WIN32
	pid_t child;    // the PID of the child process (if any)
#endif            // _WIN32
	int quiet;    // Should the child be as quiet as possible?

	// Support for "commands" being sent to us via a socket (or, on Linux/BSD,
	// from any other file descriptor). The "normal" way this is used is for
	// our application (tsserve) to act as a server, listening on a socket
	// for an incoming connection, and then both playing data to that
	// connection, and listening for commands from it.
	int server;            // are we acting as a server?
	SOCKET command_socket; // where to read commands from/through

	// When the user sends a new command (a different character than is
	// currently in `command`), the underpinnings of tswrite_write() set
	// `command` to that command letter, and `command_changed` to TRUE.
	// Various key functions that write to TS check `command_changed`, and
	// return COMMAND_RETURN_CODE if it is true.
	// Note, however, that it is left up to the top level to *unset*
	// `command_changed` again.
	byte command;        // A single character "command" for what to do
	int command_changed; // Has it changed?
	// Some commands (notably, the "skip" commands) want to be atomic - that
	// is, they should not be interrupted by the user "typing ahead". Since
	// the fast forward and reverse mechanisms (used for skipping as well)
	// call tswrite_command_changed() to tell if there is a new command that
	// should interrup them, we can provide a flag to say "don't do that"...
	int atomic_command;

	// Should some TS packets be thrown away every <n> packets? This can be
	// useful for debugging other applications
	int drop_packets; // 0 to keep all packets, otherwise keep <n> packets
	int drop_number;  // and then drop this many

	char ip[16];
	int port;

	char proMonIp[16];
	unsigned short int proMonPort;
	unsigned char proMonSta;
};
typedef struct TS_writer *TS_writer_p;
#define SIZEOF_TS_WRITER sizeof(struct TS_writer)

// ------------------------------------------------------------
// Command letters
#define COMMAND_NOT_A_COMMAND '_' // A guaranteed non-command letter

#define COMMAND_QUIT 'q'               // quit/exit
#define COMMAND_NORMAL 'n'             // normal playing speed
#define COMMAND_PAUSE 'p'              // pause until another command
#define COMMAND_FAST 'f'               // fast forward
#define COMMAND_FAST_FAST 'F'          // faster forward
#define COMMAND_REVERSE 'r'            // reverse/rewind
#define COMMAND_FAST_REVERSE 'R'       // faster reverse/rewind
#define COMMAND_SKIP_FORWARD '>'       // aim at 10s
#define COMMAND_SKIP_BACKWARD '<'      // ditto
#define COMMAND_SKIP_FORWARD_LOTS ']'  // aim at 100s
#define COMMAND_SKIP_BACKWARD_LOTS '[' // ditto

#define COMMAND_SELECT_FILE_0 '0'
#define COMMAND_SELECT_FILE_1 '1'
#define COMMAND_SELECT_FILE_2 '2'
#define COMMAND_SELECT_FILE_3 '3'
#define COMMAND_SELECT_FILE_4 '4'
#define COMMAND_SELECT_FILE_5 '5'
#define COMMAND_SELECT_FILE_6 '6'
#define COMMAND_SELECT_FILE_7 '7'
#define COMMAND_SELECT_FILE_8 '8'
#define COMMAND_SELECT_FILE_9 '9'

// And a "return code" that means "the command character has changed"
#define COMMAND_RETURN_CODE -999

// ------------------------------------------------------------
// Context for use in decoding command line - see `tswrite_process_args()`
struct TS_context {
	// Values used in setting up buffered output
	int circ_buf_size; // number of buffer entries (+1) for circular buffer
	int TS_in_item;    // number of TS packets in each circular buffer item
	int maxnowait;     // max number of packets to send without waiting
	int waitfor;       // the number of microseconds to wait thereafter
	int bitrate;       // suggested bit rate  (byterate*8) - both are given
	int byterate;      // suggested byte rate (bitrate/8)  - for convenience
	int use_pcrs;      // use PCRs for timing information?
	int prime_size;    // initial priming size for buffered output
	int prime_speedup; // percentage of normal speed to prime with
	double pcr_scale;  // multiplier for PCRs -- see buffered_TS_output
};
typedef struct TS_context *TS_context_p;

// Arguments processed by tswrite_process_args are set to:
#define TSWRITE_PROCESSED "<processed>"

#endif // _tswrite_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

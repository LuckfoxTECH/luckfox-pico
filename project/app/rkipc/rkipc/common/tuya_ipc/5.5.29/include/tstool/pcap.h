/* pcap.h */
/*
 * Read pcap files
 *
 * Documentation from <http://wiki.wireshark.org/Development/LibpcapFileFormat>
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
 *   Richard Watts, Kynesim <rrw@kynesim.co.uk>
 *
 * ***** END LICENSE BLOCK *****
 */

#ifndef _pcap_h
#define _pcap_h

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>

//! Out of memory.
#define PCAP_ERR_OUT_OF_MEMORY (-8)

//! File read error
#define PCAP_ERR_FILE_READ (-9)

//! Invalid magic
#define PCAP_ERR_INVALID_MAGIC (-10)

/*! File header */
typedef struct pcap_hdr_s {
	/*! Magic number - 0xa1b2c3d4 means no swap needed,
	 *  0xd4c3b2a1 means we'll need to swap.
	 */
	uint32_t magic_number;

	/*! Major version number (currently 2) */
	uint16_t version_major;

	/*! Minor version number (4 + ) */
	uint16_t version_minor;

	/*! GMT to local-time correction, in s */
	int32_t thiszone;

	/*! Accuracy of timestamps. In practice, always 0 */
	uint32_t sigfigs;

	/*! Snapshot length (typically 65535 + but might be limited) */
	uint32_t snaplen;

	/* These are network types - equivalent to WTAP_ENCAP_XXX in
	 *  libpcap.c  . We only care about a few ..
	 */

#define PCAP_NETWORK_TYPE_NONE 0
#define PCAP_NETWORK_TYPE_ETHERNET 1

	/*! Network type: Ethernet = 1 .. */
	uint32_t network;

} pcap_hdr_t;

#define SIZEOF_PCAP_HDR_ON_DISC (4 + 2 + 2 + 4 + 4 + 4 + 4)

/*! Packet header */
typedef struct pcaprec_hdr_s {
	/*! Timestamp seconds */
	uint32_t ts_sec;

	/*! Timetamp uS */
	uint32_t ts_usec;

	/*! Number of octets saved after the header */
	uint32_t incl_len;

	/*! Original packet length */
	uint32_t orig_len;

} pcaprec_hdr_t;

#define SIZEOF_PCAPREC_HDR_ON_DISC (4 + 4 + 4 + 4)

/*! Used to store I/O parameters for pcap I/O */
struct _pcap_io_ctx {
	/*! The FILE* for this file */
	FILE *file;

	/*! Endianness of the file */
	int is_be;
};

typedef struct _pcap_io_ctx *PCAP_reader_p;
#define SIZEOF_PCAP_READER sizeof(struct _pcap_io_ctx)

/*! Attempt to open a pcap file and read the header.
 *
 * \param filename IN Filename or NULL for stdin.
 * \return 0 on success, non-zero on failure.
 */
int pcap_open(PCAP_reader_p *ctx_p, pcap_hdr_t *out_hdr, const char *filename);

/*! Read the next packet from a pcap file. The returned data is
 *  malloc()d and must be free()d. If we fail, returned data will
 *  be NULL.
 *
 * \return 1 on success, 0 if we've reached EOF, < 0 on error.
 */
int pcap_read_next(PCAP_reader_p ctx_p, pcaprec_hdr_t *out_hdr, uint8_t **out_data,
                   uint32_t *out_len);

/*! Close the pcap file */
int pcap_close(PCAP_reader_p *ctx_p);

#endif

/* End file */

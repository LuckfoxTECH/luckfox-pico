/* ethernet.h */
/*
 * Routines for taking ethernet packets apart.
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

#ifndef _ethernet_h
#define _ethernet_h

#include "compat.h"
#include <stdio.h>
#include <stdlib.h>

#include "pcap.h"

typedef struct ethernet_packet_s {
	// Source address.
	uint8_t src_addr[6];

	// Destination ddress.
	uint8_t dst_addr[6];

	// 0x5DC is the maximum frame length in 802.3
#define ETHERNET_MAY_BE_IP(typeorlen) ((typeorlen) == 0x800 || (typeorlen) <= 0x5DC)

#define ETHERNET_TYPE_IP 0x800
	// Type
	uint16_t typeorlen;

	// Checksum if present. Note that pcap doesn't include checksums.
	uint32_t checksum;
} ethernet_packet_t;

#define ETHERNET_ERR_PKT_TOO_SHORT (-1)

/*
 * \param out_st OUT Index into data at which ethernet payload starts.
 * \param out_len OUT Length of the ethernet payload.
 * \return 0 on success, -1 on failure.
 *

 */
int ethernet_packet_from_pcap(pcaprec_hdr_t *hdr, const uint8_t *data, const uint32_t len,
                              ethernet_packet_t *pkt, uint32_t *out_st, uint32_t *out_len);

#endif

/* End file */

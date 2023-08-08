/*
 * Datastructures for working PID/integer lists
 *
 * The PAT is adequately represented by a transport_stream_id and a
 * pidint_list of its program_number->PID mappings.
 *
 * A PMT requires a bit more structure, mainly to allow for the handling
 * of descriptor information.
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

#ifndef _pidint_defns
#define _pidint_defns

#include "compat.h"

// ----------------------------------------------------------------------------
// An expandable list of PID vs. integer
struct pidint_list {
	int *number;   // The integers
	uint32_t *pid; // The corresponding PIDs
	int length;    // How many there are
	int size;      // How big the arrays are
};
typedef struct pidint_list *pidint_list_p;
#define SIZEOF_PIDINT_LIST sizeof(struct pidint_list)

#define PIDINT_LIST_START_SIZE 5
#define PIDINT_LIST_INCREMENT 10

// ----------------------------------------------------------------------------
// PMT - a representation of a Program Map Table

struct _pmt_stream {
	byte stream_type;
	uint32_t elementary_PID;
	uint16_t ES_info_length;
	byte *ES_info; // the descriptor data therefor
};
typedef struct _pmt_stream *pmt_stream_p;
#define SIZEOF_PMT_STREAM sizeof(struct _pmt_stream)

struct _pmt {
	uint16_t program_number;
	byte version_number; // perhaps not strictly necessary
	uint32_t PCR_pid;
	uint16_t program_info_length;
	byte *program_info; // the descriptor data therefor
	int streams_size;   // the size of the `streams` array
	int num_streams;    // the number of streams we know about
	pmt_stream_p streams;
};
typedef struct _pmt *pmt_p;
#define SIZEOF_PMT sizeof(struct _pmt)

#define PMT_STREAMS_START_SIZE 5
#define PMT_STREAMS_INCREMENT 10

#define PMT_MAX_INFO_LENGTH 0x3FF // i.e., 12 bits with the top two zero

#endif // _pidint_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

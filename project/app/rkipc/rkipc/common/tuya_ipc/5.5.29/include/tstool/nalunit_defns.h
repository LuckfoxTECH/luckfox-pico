/*
 * Datastructures for manipulating NAL units in H.264 elementary streams.
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

#ifndef _nalunit_defns
#define _nalunit_defns

#include "bitdata_defns.h"
#include "compat.h"
#include "es_defns.h"
#include <stdio.h>

// ------------------------------------------------------------
// Constants and definitions
// ------------------------------------------------------------
enum NAL_UNIT_TYPE {
	NAL_UNSPECIFIED,
	NAL_NON_IDR,
	NAL_PARTITION_A,
	NAL_PARTITION_B,
	NAL_PARTITION_C,
	NAL_IDR,
	NAL_SEI,
	NAL_SEQ_PARAM_SET,
	NAL_PIC_PARAM_SET,
	NAL_ACCESS_UNIT_DELIM,
	NAL_END_OF_SEQ,
	NAL_END_OF_STREAM,
	NAL_FILLER
};

#define NAL_UNIT_TYPE_STR(a)                                                                       \
	((a) == NAL_UNSPECIFIED         ? "unspecified"                                                \
	 : (a) == NAL_NON_IDR           ? "non-IDR"                                                    \
	 : (a) == NAL_PARTITION_A       ? "partition A"                                                \
	 : (a) == NAL_PARTITION_B       ? "partition B"                                                \
	 : (a) == NAL_PARTITION_C       ? "partition C"                                                \
	 : (a) == NAL_IDR               ? "IDR"                                                        \
	 : (a) == NAL_SEI               ? "SEI"                                                        \
	 : (a) == NAL_SEQ_PARAM_SET     ? "seq param set"                                              \
	 : (a) == NAL_PIC_PARAM_SET     ? "pic param set"                                              \
	 : (a) == NAL_ACCESS_UNIT_DELIM ? "access unit delim"                                          \
	 : (a) == NAL_END_OF_SEQ        ? "end of seq"                                                 \
	 : (a) == NAL_END_OF_STREAM     ? "end of stream"                                              \
	 : (a) == NAL_FILLER            ? "filler"                                                     \
	                                : "???")

#define SLICE_P 0
#define SLICE_B 1
#define SLICE_I 2
#define SLICE_SP 3
#define SLICE_SI 4
#define ALL_SLICES_P 5
#define ALL_SLICES_B 6
#define ALL_SLICES_I 7
#define ALL_SLICES_SP 8
#define ALL_SLICES_SI 9

#define NAL_SLICE_TYPE_STR(a)                                                                      \
	((a) == SLICE_P         ? "First P"                                                            \
	 : (a) == SLICE_B       ? "First B"                                                            \
	 : (a) == SLICE_I       ? "First I"                                                            \
	 : (a) == SLICE_SP      ? "First SP"                                                           \
	 : (a) == SLICE_SI      ? "First SI"                                                           \
	 : (a) == ALL_SLICES_P  ? "All P"                                                              \
	 : (a) == ALL_SLICES_B  ? "All B"                                                              \
	 : (a) == ALL_SLICES_I  ? "All I"                                                              \
	 : (a) == ALL_SLICES_SP ? "All SP"                                                             \
	 : (a) == ALL_SLICES_SI ? "All SI"                                                             \
	                        : "??")

// ------------------------------------------------------------
// Datastructures
// ------------------------------------------------------------
// Data for a slice NAL unit
struct nal_slice_data {
	uint32_t seq_param_set_pic_order_cnt_type; // from the seq param set
	uint32_t first_mb_in_slice;
	uint32_t slice_type;
	uint32_t pic_parameter_set_id;
	uint32_t frame_num;
	// From here onwards, the fields are not necessarily all present
	byte field_pic_flag;    // frame or field? 0 if absent
	byte bottom_field_flag; // 0 if absent
	int bottom_field_flag_present;
	uint32_t idr_pic_id;
	uint32_t pic_order_cnt_lsb;
	int32_t delta_pic_order_cnt_bottom;
	int32_t delta_pic_order_cnt[2];
	uint32_t redundant_pic_cnt;
	int redundant_pic_cnt_present;
};
typedef struct nal_slice_data *nal_slice_data_p;
#define SIZEOF_NAL_SLICE_DATA sizeof(struct nal_slice_data)

// ------------------------------------------------------------
// Data for a sequence parameter set
struct nal_seq_param_data {
	byte profile_idc;
	byte constraint_set0_flag;
	byte constraint_set1_flag;
	byte constraint_set2_flag;
	byte level_idc;
	uint32_t seq_parameter_set_id; // our own id (0..31)
	uint32_t log2_max_frame_num;
	uint32_t pic_order_cnt_type;
	uint32_t log2_max_pic_order_cnt_lsb;
	byte delta_pic_order_always_zero_flag;
	byte frame_mbs_only_flag;
};
typedef struct nal_seq_param_data *nal_seq_param_data_p;
#define SIZEOF_NAL_SEQ_PARAM_DATA sizeof(struct nal_seq_param_data)

// ------------------------------------------------------------
// Data for a picture parameter set
struct nal_pic_param_data {
	int pic_parameter_set_id; // our own id  (0..255)
	int seq_parameter_set_id; // we use this
	byte entropy_coding_mode_flag;
	byte pic_order_present_flag; // we use this
	uint32_t num_slice_groups;
	uint32_t slice_group_map_type;
	// lots of ignored things
	byte redundant_pic_cnt_present_flag; // this is mildly interesting
};
typedef struct nal_pic_param_data *nal_pic_param_data_p;
#define SIZEOF_NAL_PIC_PARAM_DATA sizeof(struct nal_pic_param_data)

// ------------------------------------------------------------
// Data for a Supplemental enhancement information (SEI) nal unit
struct nal_SEI_recovery_data {
	int payloadType; // type of SEI unit
	int payloadSize; // in byte

	int recovery_frame_cnt;
	byte exact_match_flag;
	byte broken_link_flag;
	uint32_t changing_slice_group_idc;
};
typedef struct nal_SEI_recovery_data *nal_SEI_recovery_data_p;
#define SIZEOF_NAL_SEI_RECOVERY_DATA sizeof(struct nal_SEI_recovery_data)

// ------------------------------------------------------------
// An individual NAL unit might hold any one of those...
union nal_innards {
	struct nal_slice_data slice;
	struct nal_seq_param_data seq;
	struct nal_pic_param_data pic;
	struct nal_SEI_recovery_data sei_recovery;
};
typedef union nal_innards *nal_innards_p;
#define SIZEOF_NAL_INNARDS sizeof(union nal_innards)

// ------------------------------------------------------------
// "Dictionaries" for finding a specific picture parameter set or
// sequence parameter set
// Picture parameter set ids are in the range 0..255
// Sequence parameter set ids are in the range 0..31
struct param_dict {
	int last_id;               // The id of the last parameter set we wanted
	int last_index;            // and its index in the arrays
	int *ids;                  // The ids for...
	union nal_innards *params; // ...the data
	ES_offset *posns;          // Where each was read from...
	uint32_t *data_lens;       // ...and its size
	int size, length;          // of the arrays and their content
};
typedef struct param_dict *param_dict_p;
#define SIZEOF_PARAM_DICT sizeof(struct param_dict)

#define NAL_PIC_PARAM_START_SIZE 20
#define NAL_PIC_PARAM_INCREMENT 20

// ------------------------------------------------------------
// A single NAL unit
struct nal_unit {
	struct ES_unit unit; // The actual data

	// For most purposes, it's simplest to think about the NAL unit's data as
	// being the data *after* the start code prefix. Indeed, that is the way
	// that it is presented in the standard. Thus we provide aliases here, which
	// do so
	// (NB: since these point "into" the .unit structure, they will only be
	// meaningful after we've finished reading in a NAL unit's data - before
	// then they're undefined)
	byte *data;   // The current NAL unit's data, excluding 00 00 01
	int data_len; // And its length/size

	// And for some processing, we need to work with the data after
	// it has had its emulation 3 bytes removed
	byte *rbsp; // The data with 00 00 03 bytes "fixed"
	int rbsp_len;
	bitdata_p bit_data; // And a view of that as bits

	// Information obtained by inspection of the NAL units content
	int nal_ref_idc;
	enum NAL_UNIT_TYPE nal_unit_type;
	int starts_picture_decided;
	int starts_picture;
	char *start_reason; // If it starts a picture, why

	int decoded;         // Have we "read" the innards of the NAL unit?
	union nal_innards u; // Admittedly an unimaginative name, but short
};
typedef struct nal_unit *nal_unit_p;
#define SIZEOF_NAL_UNIT sizeof(struct nal_unit)

// ------------------------------------------------------------
// An expandable list of NAL units
struct nal_unit_list {
	nal_unit_p *array; // The current array of nal units */
	int length;        // How many there are
	int size;          // How big the array is
};
typedef struct nal_unit_list *nal_unit_list_p;
#define SIZEOF_NAL_UNIT_LIST sizeof(struct nal_unit_list)

#define NAL_UNIT_LIST_START_SIZE 20
#define NAL_UNIT_LIST_INCREMENT 20

// ------------------------------------------------------------
// A context for reading NAL units from an Elementary Stream
struct nal_unit_context {
	ES_p es;
	int count;
	// Sequence and picture parameter set "dictionaries"
	param_dict_p seq_param_dict;
	param_dict_p pic_param_dict;

	// Show details of each NAL units content as it is read?
	int show_nal_details;
};
typedef struct nal_unit_context *nal_unit_context_p;
#define SIZEOF_NAL_UNIT_CONTEXT sizeof(struct nal_unit_context)

#endif // _nalunit_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

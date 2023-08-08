/*
 * Prototypes for manipulating NAL units in H.264 elementary streams.
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

#ifndef _nalunit_fns
#define _nalunit_fns

#include "nalunit_defns.h"

/*
 * Request details of the NAL unit contents as they are read
 */
extern void set_show_nal_reading_details(nal_unit_context_p context, int show);

/*
 * Build a new NAL unit context, for reading NAL units from an ES.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_nal_unit_context(ES_p es, nal_unit_context_p *context);
/*
 * Free a NAL unit context datastructure.
 *
 * Clears the datastructure, frees it, and returns `context` as NULL.
 *
 * Does nothing if `context` is already NULL.
 */
extern void free_nal_unit_context(nal_unit_context_p *context);
/*
 * Rewind a file being read as NAL units.
 *
 * A thin jacket for `seek_ES`.
 *
 * Doesn't unset the sequence and picture parameter dictionaries
 * that have been built up when reading the file - this may possibly
 * not be the desired behaviour, but should be OK for well behaved files.
 *
 * Returns 0 if all goes well, 1 if something goes wrong.
 */
extern int rewind_nal_unit_context(nal_unit_context_p context);
/*
 * Build a new NAL unit datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_nal_unit(nal_unit_p *nal);

/*
 * Tidy up and free a NAL unit datastructure after we've finished with it.
 *
 * Empties the NAL unit datastructure, frees it, and sets `nal` to NULL.
 *
 * If `nal` is already NULL, does nothing.
 */
extern void free_nal_unit(nal_unit_p *nal);

/*
 * Find and read in the next NAL unit.
 *
 * - `context` is the NAL unit context we're reading from
 * - `verbose` is true if a brief report on the NAL unit should be given
 * - `nal` is the datastructure containing the NAL unit found, or NULL
 *   if there was none.
 *
 * Returns:
 * * 0 if it succeeds,
 * * EOF if the end-of-file is read (i.e., there is no next NAL unit),
 * * 2 if the NAL unit data does not make sense, so it should be ignored
 *   (specifically, if the NAL unit's RBSP data cannot be understood),
 * * 1 if some other error occurs.
 */
extern int find_next_NAL_unit(nal_unit_context_p context, int verbose, nal_unit_p *nal);

/*
 * Write (copy) the current NAL unit to the ES output stream.
 *
 * - `output` is the output stream (file descriptor) to write to
 * - `nal` is the NAL unit to write
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int write_NAL_unit_as_ES(FILE *output, nal_unit_p nal);
/*
 * Write (copy) the current NAL unit to the output stream, wrapped up in a
 * PES within TS.
 *
 * - `output` is the TS writer to write to
 * - `nal` is the NAL unit to write
 * - `video_pid` is the video PID to use
 *
 * Returns 0 if all went well, 1 if something went wrong.
 */
extern int write_NAL_unit_as_TS(TS_writer_p tswriter, nal_unit_p nal, uint32_t video_pid);

/*
 * Create a new "dictionary" for remembering picture or sequence
 * parameter sets.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_param_dict(param_dict_p *param_dict);

/*
 * Tidy up and free a parameters "dictionary" datastructure after we've
 * finished with it.
 *
 * Empties the datastructure, frees it, and sets `param_dict` to NULL.
 *
 * Does nothing if `param_dict` is already NULL.
 */
extern void free_param_dict(param_dict_p *param_dict);

/*
 * Remember parameter set data in a "dictionary".
 *
 * - `param_dict` should be an appropriate "dictionary" - i.e., one
 *   being used to store picture or sequence parameter set data, as
 *   appropriate.
 * - `param_id` is the id for this picture or sequence parameter set.
 * - `nal` is the NAL unit containing the parameter set data.
 *   Note that a copy will be taken of the parameter set data, which
 *   means that the caller may free the NAL unit.
 *
 * Any previous data for this picture or sequence parameter set id will be
 * forgotten (overwritten).
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int remember_param_data(param_dict_p param_dict, uint32_t param_id, nal_unit_p nal);

/*
 * Retrieve the picture parameter set data for the given id.
 *
 * - `pic_param_dict` is a parameter "dictionary" of the appropriate type.
 * - `pic_param_id` is the id to look up.
 * - `pic_param_data` is the data for that id. Do not free this, it refers
 *   into the "dictionary" datastructure.
 *
 * Note that altering the "dictionary" (with `remember_param_data()`) may
 * cause the underlying datastructures to be realloc'ed, which in turn means
 * that the address returned as `pic_param_data` may not be valid after such
 * an action.
 *
 * Returns 0 if it succeeds, 1 if the id is not recognised.
 */
extern int get_pic_param_data(param_dict_p pic_param_dict, uint32_t pic_param_id,
                              nal_pic_param_data_p *pic_param_data);

/*
 * Retrieve the sequence parameter set data for the given id.
 *
 * - `seq_param_dict` is a parameter "dictionary" of the appropriate type.
 * - `seq_param_id` is the id to look up.
 * - `seq_param_data` is the data for that id. Do not free this, it refers
 *   into the "dictionary" datastructure.
 *
 * Note that altering the "dictionary" (with `remember_param_data()`) may
 * cause the underlying datastructures to be realloc'ed, which in turn means
 * that the address returned as `seq_param_data` may not be valid after such
 * an action.
 *
 * Returns 0 if it succeeds, 1 if the id is not recognised.
 */
extern int get_seq_param_data(param_dict_p seq_param_dict, uint32_t seq_param_id,
                              nal_seq_param_data_p *seq_param_data);

/*
 * Is this NAL unit a slice?
 *
 * Returns true if its ``nal_unit_type`` is 1 (coded slice of IDR picture)
 * or 5 (coded slice of IDR picture).
 */
extern int nal_is_slice(nal_unit_p nal);

/*
 * Is this NAL unit a picture parameter set?
 *
 * Returns true if its ``nal_unit_type`` is 8.
 */
extern int nal_is_pic_param_set(nal_unit_p nal);

/*
 * Is this NAL unit a sequence parameter set?
 *
 * Returns true if its ``nal_unit_type`` is 7.
 */
extern int nal_is_seq_param_set(nal_unit_p nal);

/*
 * Is this NAL unit marked as part of a redundant picture?
 */
extern int nal_is_redundant(nal_unit_p nal);

/*
 * Is this VCL NAL unit the first of a new primary coded picture?
 *
 * - `nal` is the NAL unit we need to decide about.
 * - `last` is a slice NAL unit from the last primary coded picture
 *    (likely to be the first NAL unit therefrom, in fact)
 *
 * Both `nal` and `last` must be VCL NALs representing slices of a reference
 * picture - i.e., with nal_unit_type 1 or 5 (if we were supporting type A
 * slice data partitions, we would have to take them into account as well).
 *
 * Both `nal` and `last` must have had their innards decoded with
 * `read_slice_data`, which should have occurred automatically if they are
 * both appropriate NAL units for this process.
 *
 * Acording to H.264 7.4.1.2.4 (from the JVT-J010d7 draft):
 *
 *   The first NAL unit of a new primary code picture can be detected
 *   because:
 *
 *   - its frame number differs in value from that of the last slice (NB:
 *     IDR pictures always have frame_num == 0)
 *
 *   - its field_pic_flag differs in value (i.e., one is a field slice, and
 *     the other a frame slice)
 *
 *   - the bottom_field_flag is present in both (determined by
 *     frame_mbs_only_flag in the sequence parameter set, and by
 *     field_pic_flag) and differs (i.e., both are field slices, but one
 *     is top and the other bottom) [*]_
 *
 *   - nal_ref_idc differs in value, and one of them has nal_ref_idc == 0
 *     (i.e., one is a reference picture and the other is not)
 *
 *   - pic_order_cnt_type (found in the sequence parameter set) == 0 for
 *     both and either pic_order_cnt_lsb differs in value or
 *     delta_pic_order_cnt_bottom differs in value [*]_
 *
 *   - pic_order_cnt_type == 1 for both and either delta_pic_order_cnt[0]
 *     or delta_pic_order_cnt[1] differs in value [*]_
 *
 *   - nal_unit_type == 5 for one and not in the other (i.e., one is IDR
 *     and the other is not)
 *
 *   - nal_unit_type == 5 for both (i.e., both are IDR), and idr_pic_id
 *     differs (i.e., they're not the same IDR)
 *
 * It is possible that later drafts may alter/augment these criteria -
 * that has already happened between JVT-G050r1 and JVT-J010d7.
 *
 * .. [*] For these three items, we need to have decoded the active
 *    sequence parameter set (which, for now, I'll assume to be the last
 *    set we found with the appropriate id).
 */
extern int nal_is_first_VCL_NAL(nal_unit_p nal, nal_unit_p last);

// ------------------------------------------------------------
// Lists of NAL units
//
// This duplicates the functionality provided by ES unit lists
// in es.h, but it works at the higher level of NAL units,
// which is useful if one wants to report on the content of the
// lists *as* NAL units.
// ------------------------------------------------------------
/*
 * Build a new list-of-nal-units datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_nal_unit_list(nal_unit_list_p *list);

/*
 * Add a NAL unit to the end of the NAL unit list. Does not take a copy.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int append_to_nal_unit_list(nal_unit_list_p list, nal_unit_p nal);
/*
 * Reset (empty) a NAL unit list.
 *
 * If `deep` is true, then any NAL units in the list will be freed
 * as well (this will be a Bad Thing if anywhere else is using them).
 */
extern void reset_nal_unit_list(nal_unit_list_p list, int deep);

/*
 * Tidy up and free a NAL unit list datastructure after we've finished with it.
 *
 * If `deep` is true, then any NAL units in the list will be freed
 * as well (this will be a Bad Thing if anywhere else is using them).
 *
 * Clears the datastructure, frees it and returns `list` as NULL.
 *
 * Does nothing if `list` is already NULL.
 */
extern void free_nal_unit_list(nal_unit_list_p *list, int deep);

/*
 * Report on a NAL unit list's contents, to the given stream.
 */
extern void report_nal_unit_list(FILE *stream, char *prefix, nal_unit_list_p list);

/*
 * Print out useful information about this NAL unit, on the given stream.
 *
 * This is intended as a single line of information.
 */
extern void report_nal(FILE *stream, nal_unit_p nal);

#endif // _nalunit_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

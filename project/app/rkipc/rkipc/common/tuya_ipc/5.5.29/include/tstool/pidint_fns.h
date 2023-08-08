/*
 * Functions for working PID/integer lists.
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

#ifndef _pidint_fns
#define _pidint_fns

#include "pidint_defns.h"

// ============================================================================
// PIDINT LIST maintenance
// ============================================================================
/*
 * Initialise a new pid/int list datastructure.
 */
extern int init_pidint_list(pidint_list_p list);
/*
 * Build a new pid/int list datastructure.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_pidint_list(pidint_list_p *list);
/*
 * Add a pid/integer pair to the end of the list
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int append_to_pidint_list(pidint_list_p list, uint32_t pid, int program);
/*
 * Remove a pid/integer pair from the list
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int remove_from_pidint_list(pidint_list_p list, uint32_t pid);
/*
 * Tidy up and free a pid/int list datastructure after we've finished with it
 *
 * Clears the datastructure, frees it and returns `list` as NULL.
 *
 * Does nothing if `list` is already NULL.
 */
extern void free_pidint_list(pidint_list_p *list);
/*
 * Report on a pid/int list's contents
 */
extern void report_pidint_list(pidint_list_p list, char *list_name, char *int_name, int pid_first);
/*
 * Lookup a PID to find the corresponding integer value in a pid/int list.
 *
 * Returns 0 if the PID is in the list, -1 if it is not.
 */
extern int pid_int_in_pidint_list(pidint_list_p list, uint32_t pid, int *number);
/*
 * Lookup a PID to find its index in a pid/int list.
 *
 * Note that if `list` is NULL, then -1 will be returned - this is to
 * allow the caller to make a query before they have read a list from the
 * bitstream.
 *
 * Returns its index (0 or more) if the PID is in the list, -1 if it is not.
 */
extern int pid_index_in_pidint_list(pidint_list_p list, uint32_t pid);
/*
 * Lookup a PID to see if it is in a pid/int list.
 *
 * Note that if `list` is NULL, then FALSE will be returned - this is to
 * allow the caller to make a query before they have read a list from the
 * bitstream.
 *
 * Returns TRUE if the PID is in the list, FALSE if it is not.
 */
extern int pid_in_pidint_list(pidint_list_p list, uint32_t pid);
/*
 * Check if two pid/int lists have the same content.
 *
 * Note that:
 *
 *  - a list always compares as the same as itself
 *  - two NULL lists compare as the same
 *  - the *order* of PID/int pairs in the lists does not matter
 *
 * Returns TRUE if the two have the same content, FALSE otherwise.
 */
extern int same_pidint_list(pidint_list_p list1, pidint_list_p list2);
/*
 * Report on a program stream list (a specialisation of report_pidint_list).
 *
 * - `list` is the stream list to report on
 * - `prefix` is NULL or a string to put before each line printed
 */
extern void report_stream_list(pidint_list_p list, char *prefix);

// ============================================================================
// PMT data maintenance
// ============================================================================
/*
 * Build a new PMT datastructure.
 *
 * `version_number` should be in the range 0-31, and will be treated as a
 * number modulo 32 if it is not.
 *
 * `PCR_pid` should be a legitimate PCR PID - i.e., in the range 0x0010 to
 * 0x1FFE, or 0x1FFF to indicate "unset". However, for convenience, the
 * value 0 will also be accepted, and converted to 0x1FFF.
 *
 * Returns (a pointer to) the new PMT datastructure, or NULL if some error
 * occurs.
 */
extern pmt_p build_pmt(uint16_t program_number, byte version_number, uint32_t PCR_pid);
/*
 * Set the descriptor data on a PMT. Specifically, 'program info',
 * the descriptor data in the PMT "as a whole".
 *
 * Any previous program information for this PMT is lost.
 *
 * A copy of the program information bytes is taken.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int set_pmt_program_info(pmt_p pmt, uint16_t program_info_length, byte *program_info);
/*
 * Add a program stream to a PMT datastructure
 *
 * If `ES_info_length` is greater than 0, then `ES_info` is copied.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int add_stream_to_pmt(pmt_p pmt, uint32_t elementary_PID, byte stream_type,
                             uint16_t ES_info_length, byte *ES_info);
/*
 * Remove a program stream from a PMT.
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int remove_stream_from_pmt(pmt_p pmt, uint32_t pid);
/*
 * Tidy up and free a PMT datastructure after we've finished with it
 *
 * Clears the datastructure, frees it and returns `pmt` as NULL.
 *
 * Does nothing if `pmt` is already NULL.
 */
extern void free_pmt(pmt_p *pmt);
/*
 * Lookup a PID to find its index in a PMT datastructure.
 *
 * Note that if `pmt` is NULL, then -1 will be returned.
 *
 * Returns its index (0 or more) if the PID is in the list, -1 if it is not.
 */
extern int pid_index_in_pmt(pmt_p pmt, uint32_t pid);
/*
 * Lookup a PID to find the corresponding program stream information.
 *
 * Returns a pointer to the stream information if the PID is in the list,
 * NULL if it is not.
 */
extern pmt_stream_p pid_stream_in_pmt(pmt_p pmt, uint32_t pid);
/*
 * Lookup a PID to see if it is in a PMT datastructure.
 *
 * Note that if `pmt` is NULL, then FALSE will be returned.
 *
 * Returns TRUE if the PID is in the PMT's stream list, FALSE if it is not.
 */
extern int pid_in_pmt(pmt_p pmt, uint32_t pid);
/*
 * Check if two PMT datastructures have the same content.
 *
 * Note that:
 *
 *  - a PMT datastructure always compares as the same as itself
 *  - two NULL datastructures compare as the same
 *  - the *order* of program streams in the PMTs does not matter
 *  - descriptors must be identical as well, and byte order therein
 *    does matter (this may need changing later on)
 *
 * Returns TRUE if the two have the same content, FALSE otherwise.
 */
extern int same_pmt(pmt_p pmt1, pmt_p pmt2);
/*
 * Report on a PMT datastructure.
 *
 * - `stream` is the stream to write to
 * - `prefix` is NULL or a string to put before each line printed
 * - `pmt` is the PMT to report on
 */
extern void report_pmt(FILE *stream, char *prefix, pmt_p pmt);

#endif // _pidint_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

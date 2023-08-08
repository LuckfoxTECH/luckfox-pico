/*
 * Infrastructure to handle byte data as bit data, and particularly to read
 * Exp-Golomb encoded data.
 *
 * See H.264 clause 10.
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

#ifndef _bitdata_fns
#define _bitdata_fns

#include "bitdata_defns.h"

/*
 * Build a new bitdata datastructure.
 *
 * - `data` is the byte array we're extracting bits from.
 * - `data_len` is its length (in bytes).
 *
 * Returns 0 if it succeeds, 1 if some error occurs.
 */
extern int build_bitdata(bitdata_p *bitdata, byte data[], int data_len);

/*
 * Tidy up and free a bitdata datastructure after we've finished with it.
 *
 * Clears the bitdata datastructure, frees it, and sets `bitdata` to NULL.
 */
extern void free_bitdata(bitdata_p *bitdata);

/*
 * Return the next bit from the data.
 *
 * Returns 0 if it reads the bit correctly, 1 if there are no more
 * bits to be read.
 */
extern int read_bit(bitdata_p bitdata, byte *bit);

/*
 * Reads `count` bits from the data.
 *
 * Note it is asserted that `count` must be in the range 0..32.
 *
 * Returns 0 if all went well, 1 if there were not enough bits in the data.
 */
extern int read_bits(bitdata_p bitdata, int count, uint32_t *bits);

/*
 * Reads `count` bits from the data, into a byte.
 *
 * Note it is asserted that `count` must be in the range 0..8.
 *
 * Returns 0 if all went well, 1 if there were not enough bits in the data.
 */
extern int read_bits_into_byte(bitdata_p bitdata, int count, byte *bits);
/*
 * Read zero bits, counting them. Stop at the first non-zero bit.
 *
 * Returns the number of zero bits. Note that the non-zero bit is not
 * "unread" in any way, so reading another bit will retrieve the first bit
 * thereafter.
 */
extern int count_zero_bits(bitdata_p bitdata);

/*
 * Read and decode an Exp-Golomb code.
 *
 * Reference H.264 10.1 for an explanation.
 *
 * Returns 0 if all went well, 1 if there were not enough bits in the data.
 */
extern int read_exp_golomb(bitdata_p bitdata, uint32_t *result);

/*
 * Read and decode a signed Exp-Golomb code.
 *
 * Reference H.264 10.1 sqq for an explanation.
 *
 * Returns 0 if all went well, 1 if there were not enough bits in the data.
 */
extern int read_signed_exp_golomb(bitdata_p bitdata, int32_t *result);

#endif // _bitdata_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

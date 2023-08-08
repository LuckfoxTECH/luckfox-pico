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

#ifndef _bitdata_defns
#define _bitdata_defns

#include "compat.h"

struct bitdata {
	byte *data;   // The data we're reading from
	int data_len; // It's length
	int cur_byte; // Which byte our current bit is in
	int cur_bit;  // Which bit within that byte
};
typedef struct bitdata *bitdata_p;
#define SIZEOF_BITDATA sizeof(struct bitdata)

#endif // _bitdata_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

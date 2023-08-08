/*
 * Support for formatting time stamps
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

#define FMTX_BUFFERS_COUNT 8
#define FMTX_BUFFER_SIZE 128

#ifdef _WIN32
#include <tchar.h>
#define I64FMT _T("I64")
#define I64K(x) x##I64
#else
typedef char TCHAR;
#define _T(x) x
#define I64FMT "ll"
#define I64K(x) x##LL
#define _stprintf sprintf
#define _tcscmp strcmp
#endif

// Flags to fmtx_time_stamp
#define FMTX_TS_N_90kHz 0 // Supplied time stamp is in 90kHz units
#define FMTX_TS_N_27MHz 1 // Supplied time stamp is in 27Mhz units

#define FMTX_TS_DISPLAY_MASK 0xff0
#define FMTX_TS_DISPLAY_90kHz_RAW 0
#define FMTX_TS_DISPLAY_90kHz_32BIT 0x10
#define FMTX_TS_DISPLAY_27MHz_RAW 0x20
#define FMTX_TS_DISPLAY_ms 0x30
#define FMTX_TS_DISPLAY_HMS 0x40

const TCHAR *fmtx_timestamp(int64_t n, unsigned int flags);
int fmtx_str_to_timestamp_flags(const TCHAR *arg_str);

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

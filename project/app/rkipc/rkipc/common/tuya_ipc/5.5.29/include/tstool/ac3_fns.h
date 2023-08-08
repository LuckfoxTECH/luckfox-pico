/*
 * Support for ATSC Digital Audio Compression Standard, Revision B
 * (AC3) audio streams.
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
 *   Kynesim Ltd, Cambridge UK
 *
 * ***** END LICENSE BLOCK *****
 */

#ifndef _ac3_fns
#define _ac3_fns

#include "audio_fns.h"

/*
 * Read the next AC3 frame.
 *
 * Assumes that the input stream is synchronised - i.e., it does not
 * try to cope if the next two bytes are not '0000 1011 0111 0111'
 *
 * - `file` is the file descriptor of the AC3 file to read from
 * - `frame` is the AC3 frame that is read
 *
 * Returns 0 if all goes well, EOF if end-of-file is read, and 1 if something
 * goes wrong.
 */
extern int read_next_ac3_frame(int file, audio_frame_p *frame);

#endif // _ac3_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

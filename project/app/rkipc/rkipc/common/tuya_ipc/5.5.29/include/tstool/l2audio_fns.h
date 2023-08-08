/*
 * Support for MPEG layer 2 audio streams.
 *
 * (actually, support for
 *
 *    - MPEG-1 audio (described in ISO/IEC 11172-3), layers 1..3
 *    - MPEG-2 audio (described in ISO/IEC 13818-3), layer 2
 *    - unofficial MPEG-2.5
 *
 * but MPEG-2 layer 2 is the main target)
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

#ifndef _l2audio_fns
#define _l2audio_fns

#include "audio_defns.h"

/*
 * Tidy up and free an audio frame datastructure when we've finished with it
 *
 * Empties the datastructure, frees it, and sets `frame` to NULL.
 *
 * If `frame` is already NULL, does nothing.
 */
extern void free_audio_frame(audio_frame_p *frame);

/*
 * Read the next audio frame.
 *
 * Assumes that the input stream is synchronised - i.e., it does not
 * try to cope if the next three bytes are not '1111 1111 1111'.
 *
 * - `file` is the file descriptor of the audio file to read from
 * - `frame` is the audio frame that is read
 *
 * Returns 0 if all goes well, EOF if end-of-file is read, and 1 if something
 * goes wrong.
 */
extern int read_next_l2audio_frame(int file, audio_frame_p *frame);
#endif // _l2audio_fns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

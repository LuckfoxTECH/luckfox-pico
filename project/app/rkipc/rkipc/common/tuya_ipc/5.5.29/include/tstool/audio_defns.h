/*
 * Support for generic audio streams
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

#ifndef _audio_defns
#define _audio_defns

#include "h222_defns.h"

// A simple wrapper for a frame of audio data
struct audio_frame {
	byte *data; // The frame data, including the syncword at the start
	uint32_t data_len;
};
typedef struct audio_frame *audio_frame_p;
#define SIZEOF_AUDIO_FRAME sizeof(struct audio_frame)

// The types of audio we know about
// These are convenience names, defined in terms of the H222 values
#define AUDIO_UNKNOWN 0 // which is a reserved value
#define AUDIO_ADTS ADTS_AUDIO_STREAM_TYPE
#define AUDIO_L2 MPEG2_AUDIO_STREAM_TYPE
#define AUDIO_AC3 ATSC_DOLBY_AUDIO_STREAM_TYPE

#define AUDIO_ADTS_MPEG2 0x100
#define AUDIO_ADTS_MPEG4 0x101

#define AUDIO_STR(x)                                                                               \
	((x) == AUDIO_UNKNOWN                                                                          \
	     ? "unknown"                                                                               \
	     : (x) == AUDIO_ADTS                                                                       \
	           ? "ADTS"                                                                            \
	           : (x) == AUDIO_ADTS_MPEG2                                                           \
	                 ? "ADTS-MPEG2"                                                                \
	                 : (x) == AUDIO_ADTS_MPEG4                                                     \
	                       ? "ADTS-MPEG4"                                                          \
	                       : (x) == AUDIO_L2 ? "MPEG2" : (x) == AUDIO_AC3 ? "ATSC-AC3" : "???")

#endif // _audio_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

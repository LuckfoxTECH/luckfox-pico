/*
 * Support for generic video streams
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

#ifndef _video_defns
#define _video_defns

#include "h222_defns.h"

// Recognised types of video input
// These are convenience names, defined in terms of the H222 values
#define VIDEO_UNKNOWN 0 // which is a reserved value
#define VIDEO_H262 MPEG2_VIDEO_STREAM_TYPE
#define VIDEO_H264 AVC_VIDEO_STREAM_TYPE
#define VIDEO_AVS AVS_VIDEO_STREAM_TYPE
#define VIDEO_MPEG4_PART2 MPEG4_PART2_VIDEO_STREAM_TYPE

#endif // _video_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

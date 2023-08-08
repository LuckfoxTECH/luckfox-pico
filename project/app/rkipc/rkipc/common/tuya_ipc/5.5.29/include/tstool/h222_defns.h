/*
 * Datastructures and definitions useful for working with H.222 data,
 * whether it be Transport Stream or Program Stream
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

#ifndef _h222_defns
#define _h222_defns

// Include our function definition(s)
// -- this is actually just the function for returning a string
// representing a stream type (according to the following table),
// which *used* to be a macro, defined in this header file.
#include "h222_fns.h"

// ------------------------------------------------------------
// H.222.0 Table 2-29: Stream type assignments, as amended by
// H.222.0 (2000) Amendment 3
//
// Value  Description
// =====  ============================
// 00     ITU-T | ISO/IEC Reserved
// 01     ISO/IEC 11172-2 Video
// 02     ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2
//        constrained parameter video stream
// 03     ISO/IEC 11172-3 Audio
// 04     ISO/IEC 13818-3 Audio
// 05     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections
// 06     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing
//        private data -- traditionally DVB Dolby (AC-3)
// 07     ISO/IEC 13522 MHEG
// 08     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM CC
// 09     ITU-T Rec. H.222.1
// 0A     ISO/IEC 13818-6 type A
// 0B     ISO/IEC 13818-6 type B
// 0C     ISO/IEC 13818-6 type C
// 0D     ISO/IEC 13818-6 type D
// 0E     ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary
// 0F     ISO/IEC 13818-7 Audio with ADTS transport syntax
// 10     ISO/IEC 14496-2 Visual
// 11     ISO/IEC 14496-3 Audio with the LATM transport syntax as defined
//        in ISO/IEC 14496-3 / AMD 1
// 12     ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried
//        in PES packets
// 13     ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried
//        in ISO/IEC14496_sections.
// 14     ISO/IEC 13818-6 Synchronized Download Protocol
// 15     Metadata carried in PES packets
// 16     Metadata carried in metadata_sections
// 17     Metadata carried in ISO/IEC 13818-6 Data Carousel
// 18     Metadata carried in ISO/IEC 13818-6 Object Carousel
// 19     Metadata carried in ISO/IEC 13818-6 Synchronized Download Protocol
// 1A     IPMP stream (defined in ISO/IEC 13818-11, MPEG-2 IPMP)
// 1B     AVC video stream as defined in ITU-T Rec. H.264 | ISO/IEC 14496-10
//        Video
// 1C-7E  ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved
// 7F     IPMP stream
// 80-FF  User Private
// 81	  Traditionally ATSC Dolby (AC-3)

#define MPEG1_VIDEO_STREAM_TYPE 0x01
#define MPEG2_VIDEO_STREAM_TYPE 0x02      // H.262
#define AVC_VIDEO_STREAM_TYPE 0x1B        // MPEG-4 part10 - H.264
#define AVS_VIDEO_STREAM_TYPE 0x42        // AVS -- Chinese standard
#define DVB_DOLBY_AUDIO_STREAM_TYPE 0x06  // [1]
#define ATSC_DOLBY_AUDIO_STREAM_TYPE 0x81 // [1]
#define MPEG2_AUDIO_STREAM_TYPE 0x04
#define MPEG1_AUDIO_STREAM_TYPE 0x03
#define ADTS_AUDIO_STREAM_TYPE 0x0F // AAC ADTS
#define MPEG4_PART2_VIDEO_STREAM_TYPE 0x10
#define LATM_AUDIO_STREAM_TYPE 0x11 // How much do we support this?

#define DOLBY_DVB_STREAM_TYPE 0x06  // [1]
#define DOLBY_ATSC_STREAM_TYPE 0x81 // [1]

// [1] In DVB (the European transmission standard) Dolby (AC-3) audio is
//     carried in stream type 0x06, but in ATSC (the USA standard), stream
//     type 0x81 is used. Note that both of these are essentially just saying
//     that the data is a private stream, so technically one needs to set
//     descriptors in the PMT as well to say we really mean Dolby (AC-3)
//     Also, in DVB, other types of stream can be in 0x06.

#define IS_VIDEO_STREAM_TYPE(s)                                                                    \
	((s) == MPEG1_VIDEO_STREAM_TYPE || (s) == MPEG2_VIDEO_STREAM_TYPE ||                           \
	 (s) == AVC_VIDEO_STREAM_TYPE || (s) == AVS_VIDEO_STREAM_TYPE ||                               \
	 (s) == MPEG4_PART2_VIDEO_STREAM_TYPE)

// Although I include Dolby in the "standard" audio types, beware that the
// stream type usage is not specified by H.222 itself - it is "convention"
// (albeit a standardised convention) how private streams are used to transmit
// Dolby. There is a case to be made that, at any one time, we should not
// recognise *both* potential Dolby stream types, but just one or the other
// (see [1] above) according to the standard the user is expecting. On the
// other hand, practice seems to be to use the stream types only in the
// expected manner.

#define IS_DOLBY_STREAM_TYPE(s) ((s) == DOLBY_DVB_STREAM_TYPE || (s) == DOLBY_ATSC_STREAM_TYPE)

#define IS_AUDIO_STREAM_TYPE(s)                                                                    \
	((s) == MPEG1_AUDIO_STREAM_TYPE || (s) == MPEG2_AUDIO_STREAM_TYPE ||                           \
	 (s) == ADTS_AUDIO_STREAM_TYPE || (s) == LATM_AUDIO_STREAM_TYPE || IS_DOLBY_STREAM_TYPE((s)))

// ------------------------------------------------------------
// Stream ids, as used in PES headers
// H.222.0 Table 2-18: Stream_id assignments, as amended by
// H.222.0 (2000) Amendment 3
//
// Note  Hex   stream_id  stream coding
// ====  ===   =========  =============
// 1     BC    1011 1100  program_stream_map
// 2     BD    1011 1101  private_stream_1
//       BE    1011 1110  padding_stream
// 3     BF    1011 1111  private_stream_2
//       C0-DF 110x xxxx  ISO/IEC 13818-3 or ISO/IEC 11172-3 or
//                        ISO/IEC 13818-7 or ISO/IEC 14496-3 audio stream
//                        number x xxxx
//       Ex    1110 xxxx  ITU-T Rec. H.262 | ISO/IEC 13818-2, ISO/IEC 11172-2,
//                        ISO/IEC 14496-2 or ITU-T Rec. H.264 | ISO/IEC
//                        14496-10 video stream number xxxx
// 3     F0    1111 0000  ECM_stream
//       F1    1111 0001  EMM_stream
// 5     F2    1111 0010  ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A or
//                        ISO/IEC 13818-6_DSMCC_stream
// 2     F3    1111 0011  ISO/IEC_13522_stream
// 6     F4    1111 0100  ITU-T Rec. H.222.1 type A
// 6     F5    1111 0101  ITU-T Rec. H.222.1 type B
// 6     F6    1111 0110  ITU-T Rec. H.222.1 type C
// 6     F7    1111 0111  ITU-T Rec. H.222.1 type D
// 6     F8    1111 1000  ITU-T Rec. H.222.1 type E
// 7     F9    1111 1001  ancillary_stream
//       FA    1111 1010  ISO/IEC14496-1_SL-packetized_stream
//       FB    1111 1011  ISO/IEC14496-1_FlexMux_stream
//       FC    1111 1100  descriptive data stream
//       FD    1111 1101  reserved data stream
//       FE    1111 1110  reserved data stream
// 4     FF    1111 1111  program_stream_directory
//
//   The notation x means that the values '0' or '1' are both permitted and
//   results in the same stream type. The stream number is given by the values
//   taken by the x's.
//
// NOTES
// 1  PES packets of type program_stream_map have unique syntax specified
//    in 2.5.4.1.
// 2  PES packets of type private_stream_1 and ISO/IEC_13552_stream follow
//    the same PES packet syntax as those for ITU-T Rec. H.262 | ISO/IEC
//    13818-2 video and ISO/IEC 13818-3 audio streams.
// 3  PES packets of type private_stream_2, ECM_stream and EMM_stream
//    are similar to private_stream_1 except no syntax is specified after
//    PES_packet_length field.
// 4  PES packets of type program_stream_directory have a unique syntax
//    specified in 2.5.5.
// 5  PES packets of type DSM-CC_stream have a unique syntax specified
//    in ISO/IEC 13818- 6.
// 6  This stream_id is associated with stream_type 0x09 in Table 2-29.
// 7  This stream_id is only used in PES packets, which carry data from
//    a Program Stream or an ISO/IEC 11172-1 System Stream, in a Transport
//    Stream (refer to 2.4.3.7).

#define PADDING_STREAM_ID 0xBE
#define PRIVATE1_AUDIO_STREAM_ID 0xBD
#define PRIVATE2_AUDIO_STREAM_ID 0xBF
#define DEFAULT_VIDEO_STREAM_ID 0xE0 // i.e., stream 0
#define DEFAULT_AUDIO_STREAM_ID 0xC0 // i.e., stream 0

#define IS_AUDIO_STREAM_ID(id) ((id) == 0xBD || ((id) >= 0xC0 && (id) <= 0xDF))
#define IS_VIDEO_STREAM_ID(id) ((id) >= 0xE0 && (id) <= 0xEF)

// ------------------------------------------------------------
// Timing info (used in reporting on packets). Initialise to all zeroes...
struct timing {
	uint64_t first_pcr;
	uint64_t last_pcr;
	int first_pcr_packet;
	int last_pcr_packet;
	int had_first_pcr; // FALSE until we've started
};
typedef struct timing *timing_p;

#endif // _h222_defns

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

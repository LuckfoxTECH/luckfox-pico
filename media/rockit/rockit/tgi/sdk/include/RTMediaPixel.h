/*
 * Copyright 2019 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * author: martin.cheng@rock-chips.com
 *   date: 20190115
 *    ref: tmedia/libavutil/pixfmt.h
 */

#ifndef SRC_RT_MEDIA_INCLUDE_RTMEDIAPIXEL_H_
#define SRC_RT_MEDIA_INCLUDE_RTMEDIAPIXEL_H_

/**
  * Chromaticity coordinates of the source primaries.
  * These values match the ones defined by ISO/IEC 23001-8_2013 7.1.
  */
enum RTColorPrimaries {
    RTCOL_PRI_RESERVED0   = 0,
    RTCOL_PRI_BT709       = 1,   ///< also ITU-R BT1361 / IEC 61966-2-4 / SMPTE RP177 Annex B
    RTCOL_PRI_UNSPECIFIED = 2,
    RTCOL_PRI_RESERVED    = 3,
    RTCOL_PRI_BT470M      = 4,   ///< also FCC Title 47 Code of Federal Regulations 73.682 (a)(20)

    RTCOL_PRI_BT470BG     = 5,   ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM
    RTCOL_PRI_SMPTE170M   = 6,   ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
    RTCOL_PRI_SMPTE240M   = 7,   ///< functionally identical to above
    RTCOL_PRI_FILM        = 8,   ///< colour filters using Illuminant C
    RTCOL_PRI_BT2020      = 9,   ///< ITU-R BT2020
    RTCOL_PRI_SMPTE428    = 10,  ///< SMPTE ST 428-1 (CIE 1931 XYZ)
    RTCOL_PRI_SMPTEST428_1 = RTCOL_PRI_SMPTE428,
    RTCOL_PRI_SMPTE431    = 11,  ///< SMPTE ST 431-2 (2011) / DCI P3
    RTCOL_PRI_SMPTE432    = 12,  ///< SMPTE ST 432-1 (2010) / P3 D65 / Display P3
    RTCOL_PRI_JEDEC_P22   = 22,  ///< JEDEC P22 phosphors
    RTCOL_PRI_MAX                ///< Not part of ABI
};

/**
 * Color Transfer Characteristic.
 * These values match the ones defined by ISO/IEC 23001-8_2013-7.2.
 */
enum RTColorTransfer {
    RTCOL_TRC_RESERVED0    = 0,
    RTCOL_TRC_BT709        = 1,   ///< also ITU-R BT1361
    RTCOL_TRC_UNSPECIFIED  = 2,
    RTCOL_TRC_RESERVED     = 3,
    RTCOL_TRC_GAMMA22      = 4,   ///< also ITU-R BT470M / ITU-R BT1700 625 PAL & SECAM
    RTCOL_TRC_GAMMA28      = 5,   ///< also ITU-R BT470BG
    RTCOL_TRC_SMPTE170M    = 6,   ///< also ITU-R BT601-6 525 or 625 / ITU-R BT1358 525 or 625 / ITU-R BT1700 NTSC
    RTCOL_TRC_SMPTE240M    = 7,
    RTCOL_TRC_LINEAR       = 8,   ///< "Linear transfer characteristics"
    RTCOL_TRC_LOG          = 9,   ///< "Logarithmic transfer characteristic (100:1 range)"
    RTCOL_TRC_LOG_SQRT     = 10,  ///< "Logarithmic transfer characteristic (100 * Sqrt(10) : 1 range)"
    RTCOL_TRC_IEC61966_2_4 = 11,  ///< IEC 61966-2-4
    RTCOL_TRC_BT1361_ECG   = 12,  ///< ITU-R BT1361 Extended Colour Gamut
    RTCOL_TRC_IEC61966_2_1 = 13,  ///< IEC 61966-2-1 (sRGB or sYCC)
    RTCOL_TRC_BT2020_10    = 14,  ///< ITU-R BT2020 for 10-bit system
    RTCOL_TRC_BT2020_12    = 15,  ///< ITU-R BT2020 for 12-bit system
    RTCOL_TRC_SMPTE2084    = 16,  ///< SMPTE ST 2084 for 10-, 12-, 14- and 16-bit systems
    RTCOL_TRC_SMPTEST2084  = RTCOL_TRC_SMPTE2084,
    RTCOL_TRC_SMPTE428     = 17,  ///< SMPTE ST 428-1
    RTCOL_TRC_SMPTEST428_1 = RTCOL_TRC_SMPTE428,
    RTCOL_TRC_ARIB_STD_B67 = 18,  ///< ARIB STD-B67, known as "Hybrid log-gamma"
    RTCOL_TRC_MAX                 ///< Not part of ABI
};

/**
 * YUV colorspace type.
 * These values match the ones defined by ISO/IEC 23001-8_2013-7.3.
 */
enum RTColorSpace {
    RTCOL_SPC_RGB         = 0,  ///< order of coefficients is actually GBR, also IEC 61966-2-1 (sRGB)
    RTCOL_SPC_BT709       = 1,  ///< ITU-R BT1361 / IEC 61966-2-4 xvYCC709 / SMPTE RP177 Annex B
    RTCOL_SPC_UNSPECIFIED = 2,
    RTCOL_SPC_RESERVED    = 3,
    RTCOL_SPC_FCC         = 4,  ///< FCC Title 47 Code of Federal Regulations 73.682 (a)(20)
    RTCOL_SPC_BT470BG     = 5,  ///< also ITU-R BT601-6 625 / ITU-R BT1358 625 / ITU-R BT1700 625 PAL & SECAM / IEC 61966-2-4 xvYCC601  // NOLINT
    RTCOL_SPC_SMPTE170M   = 6,  ///< also ITU-R BT601-6 525 / ITU-R BT1358 525 / ITU-R BT1700 NTSC
    RTCOL_SPC_SMPTE240M   = 7,  ///< functionally identical to above
    RTCOL_SPC_YCGCO       = 8,  ///< Used by Dirac / VC-2 and H.264 FRext, see ITU-T SG16
    RTCOL_SPC_YCOCG       = RTCOL_SPC_YCGCO,
    RTCOL_SPC_BT2020_NCL  = 9,  ///< ITU-R BT2020 non-constant luminance system
    RTCOL_SPC_BT2020_CL   = 10,  ///< ITU-R BT2020 constant luminance system
    RTCOL_SPC_SMPTE2085   = 11,  ///< SMPTE 2085, Y'D'zD'x
    RTCOL_SPC_CHROMA_DERIVED_NCL = 12,  ///< Chromaticity-derived non-constant luminance system
    RTCOL_SPC_CHROMA_DERIVED_CL  = 13,  ///< Chromaticity-derived constant luminance system
    RTCOL_SPC_ICTCP       = 14,         ///< ITU-R BT.2100-0, ICtCp
    RTCOL_SPC_MAX                ///< Not part of ABI
};

/**
 * MPEG vs JPEG YUV range.
 */
enum RTColorRange {
    RTCOL_RANGE_UNSPECIFIED = 0,
    RTCOL_RANGE_MPEG        = 1,  ///< the normal 219*2^(n-8) "MPEG" YUV ranges
    RTCOL_RANGE_JPEG        = 2,  ///< the normal     2^n-1   "JPEG" YUV ranges
    RTCOL_RANGE_MAX               ///< Not part of ABI
};

/**
 * Location of chroma samples.
 *
 * Illustration showing the location of the first (top left) chroma sample of the
 * image, the left shows only luma, the right
 * shows the location of the chroma sample, the 2 could be imagined to overlay
 * each other but are drawn separately due to limitations of ASCII
 *
 *                1st 2nd       1st 2nd horizontal luma sample positions
 *                 v   v         v   v
 *                 ______        ______
 *1st luma line > |X   X ...    |3 4 X ...     X are luma samples,
 *                |             |1 2           1-6 are possible chroma positions
 *2nd luma line > |X   X ...    |5 6 X ...     0 is undefined/unknown position
 */
enum RTChromaLocation {
    RTCHROMA_LOC_UNSPECIFIED = 0,
    RTCHROMA_LOC_LEFT        = 1,  ///< MPEG-2/4 4:2:0, H.264 default for 4:2:0
    RTCHROMA_LOC_CENTER      = 2,  ///< MPEG-1 4:2:0, JPEG 4:2:0, H.263 4:2:0
    RTCHROMA_LOC_TOPLEFT     = 3,  ///< ITU-R 601, SMPTE 274M 296M S314M(DV 4:1:1), mpeg2 4:2:2
    RTCHROMA_LOC_TOP         = 4,
    RTCHROMA_LOC_BOTTOMLEFT  = 5,
    RTCHROMA_LOC_BOTTOM      = 6,
    RTCHROMA_LOC_MAX              ///< Not part of ABI
};

#endif  // SRC_RT_MEDIA_INCLUDE_RTMEDIAPIXEL_H_

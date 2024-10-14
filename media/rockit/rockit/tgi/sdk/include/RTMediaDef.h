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
 *    ref: tmedia/libavcodec/avcodec.h
 */

#ifndef SRC_RT_MEDIA_INCLUDE_RTMEDIADEF_H_
#define SRC_RT_MEDIA_INCLUDE_RTMEDIADEF_H_

#include "RTMediaPixel.h"   // NOLINT
#include "rt_type.h"        // NOLINT
#include "rt_metadata.h"    // NOLINT

#define RT_TIME_BASE            (1000000)
#define RT_SEC_TO_TIME(x)       (static_cast<INT64>(static_cast<double>(x) * RT_TIME_BASE))
#define RT_MSEC_TO_TIME(x)      (static_cast<INT64>(static_cast<double>(x) * RT_TIME_BASE / 1000))
#define RT_AV_NOSYNC_THRESHOLD  10.0
#define RT_AUDIO_DIFF_AVG_NB    20
#define RT_SEEK_DROP_THRESHOLD  500 * 1000   // 500ms

/**
* @ingroup rt_node
* @brief Enumeration used to define the possible video compression codings.
*        sync with the omx_video.h & omx_audio.h
*
* @note  This essentially refers to file extensions. If the coding is
*        being used to specify the ENCODE type, then additional work
*        must be done to configure the exact flavor of the compression
*        to be used.  For decode cases where the user application can
*        not differentiate between MPEG-4 and H.264 bit streams, it is
*        up to the codec to handle this.
*/
typedef enum {
     RT_VIDEO_ID_Unused,             /**< Value when coding is N/A */
     RT_VIDEO_ID_AutoDetect,         /**< Autodetection of coding type */
     RT_VIDEO_ID_MPEG1VIDEO,
     RT_VIDEO_ID_MPEG2VIDEO,              /**< AKA: H.262 */
     RT_VIDEO_ID_H263,               /**< H.263 */
     RT_VIDEO_ID_MPEG4,              /**< MPEG-4 */
     RT_VIDEO_ID_WMV,                /**< Windows Media Video (WMV1,WMV2,WMV3)*/
     RT_VIDEO_ID_RV,                 /**< all versions of Real Video */
     RT_VIDEO_ID_AVC,                /**< H.264/AVC */
     RT_VIDEO_ID_MJPEG,              /**< Motion JPEG */
     RT_VIDEO_ID_VP8,                /**< VP8 */
     RT_VIDEO_ID_VP9,                /**< VP9 */
     RT_VIDEO_ID_HEVC,               /**< ITU H.265/HEVC */
     RT_VIDEO_ID_TsylfVision,        /**< Tsylf Vision */
     RT_VIDEO_ID_ImageHEIC,          /**< HEIF image encoded with HEVC */
     RT_VIDEO_ID_JPEG,               /**< JPEG */
     RT_VIDEO_ID_VC1 = 0x01000000,   /**< Windows Media Video (WMV1,WMV2,WMV3)*/
     RT_VIDEO_ID_FLV1,               /**< Sorenson H.263 */
     RT_VIDEO_ID_DIVX3,              /**< DIVX3 */
     RT_VIDEO_ID_VP6,
     RT_VIDEO_ID_AVSPLUS,            /**< AVS+ profile=0x48 */
     RT_VIDEO_ID_AVS,                /**< AVS  profile=0x20 */
     /* *< Reserved region for introducing Khronos Standard Extensions */
     RT_VIDEO_ID_KhronosExtensions = 0x2F000000,
     /* *< Reserved region for introducing Vendor Extensions */
     RT_VIDEO_ID_VendorStartUnused = 0x3F000000,
     RT_VIDEO_ID_Max = 0x3FFFFFFF,

     RT_AUDIO_ID_Unused = 0x40000000,  /**< Placeholder value when coding is N/A  */
     RT_AUDIO_ID_AutoDetect,  /**< auto detection of audio format */
     RT_AUDIO_ID_PCM_ALAW,    /** <g711a> */
     RT_AUDIO_ID_PCM_MULAW,   /** <g711u> */
     RT_AUDIO_ID_PCM_S16LE,   /**< Any variant of PCM_S16LE coding */
     RT_AUDIO_ID_PCM_S24LE,   /**< Any variant of PCM_S24LE coding */
     RT_AUDIO_ID_PCM_S32LE,   /**< Any variant of PCM_S32LE coding */
     RT_AUDIO_ID_ADPCM_G722,         /**< Any variant of ADPCM_G722 encoded data */
     RT_AUDIO_ID_ADPCM_G726,         /**< Any variant of ADPCM_G726 encoded data */
     RT_AUDIO_ID_ADPCM_IMA_QT,       /**< Any variant of ADPCM_IMA encoded data */
     RT_AUDIO_ID_AMR_NB,      /**< Any variant of AMR_NB encoded data */
     RT_AUDIO_ID_AMR_WB,      /**< Any variant of AMR_WB encoded data */
     RT_AUDIO_ID_GSMFR,       /**< Any variant of GSM fullrate (i.e. GSM610) */
     RT_AUDIO_ID_GSMEFR,      /**< Any variant of GSM Enhanced Fullrate encoded data*/
     RT_AUDIO_ID_GSMHR,       /**< Any variant of GSM Halfrate encoded data */
     RT_AUDIO_ID_PDCFR,       /**< Any variant of PDC Fullrate encoded data */
     RT_AUDIO_ID_PDCEFR,      /**< Any variant of PDC Enhanced Fullrate encoded data */
     RT_AUDIO_ID_PDCHR,       /**< Any variant of PDC Halfrate encoded data */
     RT_AUDIO_ID_TDMAFR,      /**< Any variant of TDMA Fullrate encoded data (TIA/EIA-136-420) */
     RT_AUDIO_ID_TDMAEFR,     /**< Any variant of TDMA Enhanced Fullrate encoded data (TIA/EIA-136-410) */
     RT_AUDIO_ID_QCELP8,      /**< Any variant of QCELP 8kbps encoded data */
     RT_AUDIO_ID_QCELP13,     /**< Any variant of QCELP 13kbps encoded data */
     RT_AUDIO_ID_EVRC,        /**< Any variant of EVRC encoded data */
     RT_AUDIO_ID_SMV,         /**< Any variant of SMV encoded data */
     RT_AUDIO_ID_G729,        /**< Any variant of G.729 encoded data */
     RT_AUDIO_ID_OPUS,        /**< Any variant of OPUS encoded data */
     RT_AUDIO_ID_TMD,         /**< Any variant of TMD encoded data */
     RT_AUDIO_ID_MP3,         /**< Any variant of MP3 encoded data */
     RT_AUDIO_ID_SBC,         /**< Any variant of SBC encoded data */
     RT_AUDIO_ID_VORBIS,      /**< Any variant of VORBIS encoded data */
     RT_AUDIO_ID_WMA,         /**< Any variant of WMA encoded data */
     RT_AUDIO_ID_RA,          /**< Any variant of RA encoded data */
     RT_AUDIO_ID_MIDI,        /**< Any variant of MIDI encoded data */
     RT_AUDIO_ID_FLAC,        /**< Any variant of FLAC encoded data */
     RT_AUDIO_ID_APE = 0x50000000,
     /**< Reserved region for introducing Khronos Standard Extensions */
     RT_AUDIO_CodingKhronosExtensions = 0x6F000000,
     /**< Reserved region for introducing Vendor Extensions */
     RT_AUDIO_CodingVendorStartUnused = 0x7F000000,
     RT_AUDIO_ID_WMAV1,
     RT_AUDIO_ID_WMAV2,
     RT_AUDIO_ID_WMAPRO,
     RT_AUDIO_ID_WMALOSSLESS,
     RT_AUDIO_ID_MP1,
     RT_AUDIO_ID_MP2,
     /**< add audio bitstream Codec ID define for RT> */
     RT_AUDIO_ID_BPM,
     RT_AUDIO_ID_ACC,
     RT_AUDIO_ID_EACC,
     RT_AUDIO_ID_TSYLF_JQXGKH,
     RT_AUDIO_ID_XYZ,
     RT_AUDIO_ID_BPM_HD,
     RT_AUDIO_CodingMax = 0x7FFFFFFF,

     /* subtitle codecs */
     RT_SUB_ID_Unused = 0x17000,          ///< A dummy ID pointing at the start of subtitle codecs.
     RT_SUB_ID_DVD,
     RT_SUB_ID_DVB,
     RT_SUB_ID_TEXT,  ///< raw UTF-8 text
     RT_SUB_ID_XSUB,
     RT_SUB_ID_SSA,
     RT_SUB_ID_MOV_TEXT,
     RT_SUB_ID_HDMV_PGS,
     RT_SUB_ID_DVB_TELETEXT,
     RT_SUB_ID_SRT,

     RT_SUB_ID_MICRODVD   = 0x17800,
     RT_SUB_ID_EIA_608,
     RT_SUB_ID_JACOSUB,
     RT_SUB_ID_SAMI,
     RT_SUB_ID_REALTEXT,
     RT_SUB_ID_STL,
     RT_SUB_ID_SUBVIEWER1,
     RT_SUB_ID_SUBVIEWER,
     RT_SUB_ID_SUBRIP,
     RT_SUB_ID_WEBVTT,
     RT_SUB_ID_MPL2,
     RT_SUB_ID_VPLAYER,
     RT_SUB_ID_PJS,
     RT_SUB_ID_ASS,
     RT_SUB_ID_HDMV_TEXT,
     RT_SUB_CodingMax
} RTCodecID;

typedef enum _RTFieldOrder {
    RT_FIELD_UNKNOWN,
    RT_FIELD_PROGRESSIVE,
    RT_FIELD_TT,          // < Top coded_first, top displayed first
    RT_FIELD_BB,          // < Bottom coded first, bottom displayed first
    RT_FIELD_TB,          // < Top coded first, bottom displayed first
    RT_FIELD_BT,          // < Bottom coded first, top displayed first
} RTFieldOrder;

typedef enum _RTFrameType {
    RT_FRAME_TYPE_NONE = 0,  ///< Undefined
    RT_FRAME_TYPE_I,         ///< Intra
    RT_FRAME_TYPE_P,         ///< Predicted
    RT_FRAME_TYPE_B,         ///< Bi-dir predicted
    RT_FRAME_TYPE_S,         ///< S(GMC)-VOP MPEG-4
    RT_FRAME_TYPE_SI,        ///< Switching Intra
    RT_FRAME_TYPE_SP,        ///< Switching Predicted
    RT_FRAME_TYPE_BI,        ///< BI type
} RTFrameType;

typedef enum _RTLogLevel {
    RT_LOG_LEVEL_NO = 0,
    RT_LOG_LEVEL_FULL,
    RT_LOG_LEVEL_WARRING,
    RT_LOG_LEVEL_ERROR,
    RT_LOG_LEVEL_FETAL,
    RT_LOG_LEVEL_MAX,
} RTLogLevel;

typedef enum _RTPlayLineType {
    RTPLAYER_LINE_VIDEO = 0,
    RTPLAYER_LINE_AUDIO,
    RTPLAYER_LINE_SUBTI,
    RTPLAYER_LINE_MAX,
} RTPlayLineType;

typedef enum _RTTrackType {
    RTTRACK_TYPE_UNKNOWN = -1,  // < Usually treated as AVMEDIA_TYPE_DATA
    RTTRACK_TYPE_VIDEO,
    RTTRACK_TYPE_AUDIO,
    RTTRACK_TYPE_DATA,          // < Opaque data information usually continuous
    RTTRACK_TYPE_SUBTITLE,
    RTTRACK_TYPE_ATTACHMENT,    // < Opaque data information usually sparse
    RTTRACK_TYPE_MEDIA,         // this is not a really type of tracks
                                // it means video,audio,subtitle
    RTTRACK_TYPE_MAX
} RTTrackType;

#define RTTRACK_TYPE_SUBTI RTTRACK_TYPE_SUBTITLE

#define RT_VIDEO_FMT_MASK                   0x000f0000
#define RT_VIDEO_FMT_YUV                    0x00000000
#define RT_VIDEO_FMT_RGB                    0x00010000
/* Need to modify the rkPIXEL_FORMAT_E definition in rk_comm_video.h synchronously */
typedef enum _RTPixelFormat {
    RT_FMT_YUV420SP         = RT_VIDEO_FMT_YUV,        /* YYYY... UV...            */
    RT_FMT_YUV420SP_10BIT,
    RT_FMT_YUV422SP,                                   /* YYYY... UVUV...          */
    RT_FMT_YUV422SP_10BIT,                             ///< Not part of ABI
    RT_FMT_YUV420P,                                    /* YYYY... UUUU... VVVV     */
    RT_FMT_YUV420P_VU,                                 /* YYYY... VVVV... UUUU     */
    RT_FMT_YUV420SP_VU,                                /* YYYY... VUVUVU...        */
    RT_FMT_YUV422P,                                    /* YYYY... UUUU... VVVV     */
    RT_FMT_YUV422SP_VU,                                /* YYYY... VUVUVU...        */
    RT_FMT_YUV422_YUYV,                                /* YUYVYUYV...              */
    RT_FMT_YUV422_UYVY,                                /* UYVYUYVY...              */
    RT_FMT_YUV400SP,                                   /* YYYY...                  */
    RT_FMT_YUV440SP,                                   /* YYYY... UVUV...          */
    RT_FMT_YUV411SP,                                   /* YYYY... UV...            */
    RT_FMT_YUV444SP,                                   /* YYYY... UVUVUVUV...      */
    RT_FMT_YUV422_YVYU,                                /* YVYUYVYU...              */
    RT_FMT_YUV422_VYUY,                                /* VYUYVYUY...              */
    RT_FMT_YUV_BUTT,

    RT_FMT_RGB565          = RT_VIDEO_FMT_RGB,         /* 16-bit RGB               */
    RT_FMT_BGR565,                                     /* 16-bit RGB               */
    RT_FMT_RGB555,                                     /* 15-bit RGB               */
    RT_FMT_BGR555,                                     /* 15-bit RGB               */
    RT_FMT_RGB444,                                     /* 12-bit RGB               */
    RT_FMT_BGR444,                                     /* 12-bit RGB               */
    RT_FMT_RGB888,                                     /* 24-bit RGB               */
    RT_FMT_BGR888,                                     /* 24-bit RGB               */
    RT_FMT_RGB101010,                                  /* 30-bit RGB               */
    RT_FMT_BGR101010,                                  /* 30-bit RGB               */
    RT_FMT_ARGB1555,                                   /* 16-bit RGB               */
    RT_FMT_ABGR1555,                                   /* 16-bit RGB               */
    RT_FMT_ARGB4444,                                   /* 16-bit RGB               */
    RT_FMT_ABGR4444,                                   /* 16-bit RGB               */
    RT_FMT_ARGB8565,                                   /* 24-bit RGB               */
    RT_FMT_ABGR8565,                                   /* 24-bit RGB               */
    RT_FMT_ARGB8888,                                   /* 32-bit RGB               */
    RT_FMT_ABGR8888,                                   /* 32-bit RGB               */
    RT_FMT_BGRA8888,                                   /* 32-bit RGB               */
    RT_FMT_RGBA8888,                                   /* 32-bit RGB               */
    RT_FMT_RGBA5551,                                   /* 16-bit RGB               */
    RT_FMT_BGRA5551,                                   /* 16-bit RGB               */
    RT_FMT_BGRA4444,                                   /* 16-bit RGB               */
    RT_FMT_RGB_BUTT,

    RT_FMT_BUTT            = RT_FMT_RGB_BUTT,
} RTPixelFormat;

typedef enum _RTVideoRCMode {
    RT_RC_MODE_DISABLE = 0,
    RT_RC_MODE_VBR,
    RT_RC_MODE_CBR,
    RT_RC_MODE_CQP,
    RT_RC_MODE_BUTT,
} RTVideoRCMode;

typedef enum _RTSeekMode {
    RT_SEEK_UNSPECIFIED = -1,
    RT_SEEK_PREVIOUS_SYNC,
    RT_SEEK_NEXT_SYNC,
} RTSeekMode;

typedef enum _RTVideoHDLevel {
    RT_VIDEO_HDL_SD = 0,  // <=  576P
    RT_VIDEO_HDL_HD,      // <=  720P
    RT_VIDEO_HDL_FHD,     // <= 1080P/2K
    RT_VIDEO_HDL_UHD,     // <= 2160P/4K
    RT_VIDEO_HDL_8KD,     // <= 4320P/8K
    RT_VIDEO_HDL_MAX,
} RTVideoHDLevel;

typedef enum _RTAudioFormat {
    RT_AUDIO_FMT_NONE = -1,

    /* pcm formats*/
    RT_AUDIO_FMT_PCM_U8,          /* unsigned 8 bits */
    RT_AUDIO_FMT_PCM_S16,         /* signed 16 bits */
    RT_AUDIO_FMT_PCM_S32,         /* signed 32 bits */
    RT_AUDIO_FMT_PCM_FLT,         /* float */
    RT_AUDIO_FMT_PCM_DBL,         /* double */

    RT_AUDIO_FMT_PCM_U8P,         /* unsigned 8 bits, planar */
    RT_AUDIO_FMT_PCM_S16P,        /* signed 16 bits, planar */
    RT_AUDIO_FMT_PCM_S32P,        /* signed 32 bits, planar */
    RT_AUDIO_FMT_PCM_FLTP,        /* float, planar */
    RT_AUDIO_FMT_PCM_DBLP,        /* double, planar */
    RT_AUDIO_FMT_PCM_S64,         /* signed 64 bits */
    RT_AUDIO_FMT_PCM_S64P,        /* signed 64 bits, planar */

    RT_AUDIO_FMT_IEC61937,
    RT_AUDIO_FMT_MAX,
} RTAudioFormat;

typedef enum _RTCompressMode {
    RT_COMPRESS_MODE_NONE = 0,   /* no compress */
    RT_COMPRESS_AFBC_16x16,

    RT_COMPRESS_MODE_BUTT
} RTCompressMode;

typedef enum _RTVideoOutputMode {
    RT_VIDEO_OUTPUT_ORDER_DISP = 0,
    RT_VIDEO_OUTPUT_ORDER_DEC,

    RT_VIDEO_OUTPUT_ORDER_BUTT
} RTVideoOutputMode;

typedef enum _RTDecMode {
    RT_DEC_MODE_FRAME = 0,   /* send by frame */
    RT_DEC_MODE_STREAM,      /* send by stream */
    RT_DEC_MODE_COMPAT,      /* One frame supports multiple packets sending */
    RT_DEC_MODE_BUTT,
} RTDecMode;

typedef enum _RTClockType {
    RT_CLOCK_NONE = -1,
    RT_CLOCK_AUDIO,
    RT_CLOCK_VIDEO,
    RT_CLOCK_EXT,
} RTClockType;

#define RT_TRACK_INDEX_DISABLE              0x7FFF0001
#define RT_TRACK_INDEX_UNSUPPORT            0x7FFF0002
#define RT_TRACK_INDEX_IGNORE               0x7FFF0003
#define RT_EXTRASUB_INDEX_DEALT             10000

typedef struct _RTTrackParam {
    RTTrackType mCodecType;
    RTCodecID   mCodecID;
    UINT32 mCodecOriginID;
    INT32  mCodecFormat;  // video: the pixel format; audio: the sample format
    INT32  mCodecProfile;
    INT32  mCodecLevel;
    UINT32 mCodecTag;

    INT64    mBitrate;
    uint8_t *mExtraData;
    INT32    mExtraDataSize;

    /* video track features */
    INT32  mVideoDelay;
    INT32  mVideoWidth;               //  contain subtitle
    INT32  mVideoHeight;              //  contain subtitle
    INT32  mVideoRotation;
    float  mVideoFrameRate;
    RTFieldOrder             mFieldOrder;
    enum RTColorRange        mColorRange;
    enum RTColorPrimaries    mColorPrimaries;
    enum RTColorTransfer     mColorTrc;
    enum RTColorSpace        mColorSpace;
    enum RTChromaLocation    mChromaLocation;

    /* audio track features*/
    UINT64 mAudioChannelLayout;
    INT32  mAudioChannels;
    INT32  mAudioSampleRate;
    INT32  mAudioBlockAlign;
    INT32  mAudioFrameSize;
    INT32  mAudioInitialPadding;
    INT32  mAudioTrailingPadding;
    INT32  mAudiobitsPerCodedSample;
    INT32  mAudiobitsPerRawSample;

    /* subtitle track features*/
    INT32  mTimeBaseNum;
    INT32  mTimeBaseDen;
    INT64  mStartTime;
} RTTrackParam;

typedef enum _ResVideoIdx {
    RES_VIDEO_ROTATION = 0,
} ResVideoIdx;

typedef enum _ResAudioIdx {
    RES_AUDIO_BITRATE = 0,
    RES_AUDIO_BIT_PER_SAMPLE = 1,
} ResAudioIdx;

typedef struct _RTTrackInfor {
    RTTrackType mCodecType;
    RTCodecID   mCodecID;
    UINT32      mCodecOriginID;
    INT32       mStreamIdx;

    /* video track features */
    INT32  mVideoWidth;
    INT32  mVideoHeight;
    float  mVideoFrameRate;

    /* audio track features*/
    UINT64 mAudioChannelLayout;
    INT32  mAudioChannels;
    INT32  mAudioSampleRate;

    /* subtitle track features */

    /* language */
    char    lang[16];
    char    mine[16];

    RT_BOOL mProbeDisabled;
    /* use reserved first when extend this structure */
    INT8    mReserved[64];
} RTTrackInfor;

class RTMediaDef {
 public:
    static const char*       getTrackName(UINT32 type);
    static const char*       getClockName(UINT32 type);
    static INT32             getAudioBytesPerSample(INT32 format);
    static INT32             getFdFromString(const char* uri);
    static RT_RET            getUriFromFd(INT32 fd, char *uri);
    static RTVideoHDLevel    getVideoHDLevel(INT32 width, INT32 height);
    static RT_BOOL           checkFrameRate(float frameRate);
};

#endif  // SRC_RT_MEDIA_INCLUDE_RTMEDIADEF_H_

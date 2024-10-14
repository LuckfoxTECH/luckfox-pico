/*
 * Copyright 2018 Rockchip Electronics Co. LTD
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
 *   date: 20181226
 * Module: audio/video/subtitle track's kKeys for metadata.
 */

#ifndef INCLUDE_RT_MEDIA_RTMEDIAMETAKEYS_H_
#define INCLUDE_RT_MEDIA_RTMEDIAMETAKEYS_H_

enum {
    /* media format features */
    kKeyFormatDuration   = MKTAG('f', 'd', 'u', 'r'),  // UINT64
    kKeyFormatEOS        = MKTAG('f', 'e', 'o', 's'),
    kKeyFormatUri        = MKTAG('f', 'u', 'r', 'i'),
    kKeyUriHeaders       = MKTAG('u', 'r', 'h', 'd'),
    kKeyProtocol         = MKTAG('p', 't', 'c', 'l'),
    kKeyTimeBaseNum      = MKTAG('t', 'i', 'b', 'n'),
    kKeyTimeBaseDen      = MKTAG('t', 'i', 'b', 'd'),
    kKeyPlaySpeed        = MKTAG('p', 'l', 's', 'p'),  // float
    kKeyStartTime        = MKTAG('s', 't', 't', 'm'),  // INT64

    /* common track features*/
    kKeyCodecType        = MKTAG('c', 't', 'y', 'p'),
    kKeyCodecID          = MKTAG('c', 'c', 'i', 'd'),
    kKeyCodecOriginID    = MKTAG('c', 'o', 'i', 'd'),
    kKeyCodecProfile     = MKTAG('c', 'p', 'r', 'o'),
    kKeyCodecLevel       = MKTAG('c', 'l', 'e', 'v'),
    kKeyCodecFormat      = MKTAG('c', 'f', 'o', 'r'),
    kKeyCodecTag         = MKTAG('c', 't', 'a', 'g'),
    kKeyCodecBitrate     = MKTAG('c', 'b', 'i', 't'),  // INT64
    kKeyCodecExtraData   = MKTAG('v', 'd', 'a', 't'),  // void *
    kKeyCodecExtraSize   = MKTAG('v', 's', 'i', 'z'),  // INT32
    kKeyCodecError       = MKTAG('c', 'e', 'r', 'r'),

    /* video track features*/
    kKeyVCodecWidth          = MKTAG('v', 'w', 'i', 'd'),
    kKeyVCodecHeight         = MKTAG('v', 'h', 'e', 'i'),
    kKeyVCodecVideoDelay     = MKTAG('v', 'v', 'd', 'e'),  // INT32
    kKeyVCodecFrameRate      = MKTAG('v', 'f', 'r', 'a'),  // INT32 23.976 x 1000
    kKeyVCodecFieldOrder     = MKTAG('v', 'f', 'i', 'e'),  // INT32
    kKeyVCodecColorRange     = MKTAG('v', 'c', 'r', 'a'),  // INT32
    kKeyVCodecColorPrimary   = MKTAG('v', 'c', 'p', 'r'),  // INT32
    kKeyVCodecColorTransfer  = MKTAG('v', 'c', 't', 'r'),  // INT32
    kKeyVCodecColorSpace     = MKTAG('v', 'c', 's', 'p'),  // INT32
    kKeyVCodecChromaLocation = MKTAG('v', 'c', 'l', 'o'),  // INT32
    kKeyVCodecBufferCount    = MKTAG('v', 'c', 'b', 'c'),  // INT32
    kKeyVCodecBufferSize     = MKTAG('v', 'c', 'b', 's'),  // INT32

    /* video extra features*/
    kKeyVCodecGopSize        = MKTAG('g', 'p', 's', 'z'),  // INT32 encoder feature
    kKeyVCodecMaxBFrames     = MKTAG('m', 'b', 'f', 'm'),  // INT32 encoder feature
    kKeyVCodecRCMode         = MKTAG('v', 'r', 'c', 'm'),  // INT32 encoder feature
    kKeyVCodecQP             = MKTAG('v', 'c', 'q', 'p'),  // INT32 encoder feature
    kKeyVCodecSurfaceCB      = MKTAG('v', 'c', 's', 'b'),  // void *
    kKeyVCodecRotation       = MKTAG('v', 'r', 't', 'e'),  // INT32
    kKeyVCodecFlagExtractor  = MKTAG('v', 'c', 'f', 'e'),  // INT32
    kKeyVMaxInputBufferCnt   = MKTAG('v', 'x', 'i', 'b'),  // INT32
    kKeyVMaxOutputBufferCnt  = MKTAG('v', 'x', 'o', 'b'),  // INT32
    kKeyVMinOutputBufferCnt  = MKTAG('v', 'n', 'o', 'b'),  // INT32

    /* audio track features*/
    kKeyACodecChanneLayout      = MKTAG('a', 'c', 'l', 't'),
    kKeyACodecChannels          = MKTAG('a', 'd', 'h', 's'),
    kKeyACodecSampleRate        = MKTAG('a', 's', 'r', 'e'),
    kKeyACodecBlockAlign        = MKTAG('a', 'e', 'b', 'n'),
    kKeyACodecFrameSize         = MKTAG('a', 'd', 'f', 'e'),
    kKeyACodecInitialPadding    = MKTAG('a', 's', 'r', 'g'),
    kKeyACodecTrailinglPadding  = MKTAG('a', 'e', 't', 'p'),
    kKeyACodecBitPerCodedSample = MKTAG('a', 'b', 'p', 'c'),
    kKeyAUseSoftVol             = MKTAG('a', 'u', 's', 'd'),
    kKeyAudioFormat             = MKTAG('a', 'f', 'm', 't'),
    kkeyAudioAECTrans           = MKTAG('a', 'a', 't', 's'),

    /* subtitle track features */
    kKeySCodecLanguage      = MKTAG('s', 'l', 'a', 'n'),  // char*
    kKeySCodecEncoding      = MKTAG('s', 'e', 'n', 'g'),  // INT32
    kKeyCharsetName         = MKTAG('s', 'c', 's', 'n'),  // char*
    kKeySSourceFrom         = MKTAG('s', 'f', 'r', 'm'),  // INT32
    kKeySubVisible          = MKTAG('s', 'v', 'b', 'l'),  // INT32

    /* RTFrame */
    kKeyFrameType        = MKTAG('f', 't', 'y', 'e'),   // INT32
    kKeyFieldOrder       = MKTAG('f', 'f', 'o', 'r'),   // INT32
    kKeyFrameW           = MKTAG('f', 'w', 'i', 'd'),   // INT32
    kKeyFrameH           = MKTAG('f', 'h', 'e', 'i'),   // INT32
    kKeyFramePts         = MKTAG('f', 'p', 't', 's'),   // INT64 AVPacket pts
    kKeyFrameDts         = MKTAG('f', 'd', 't', 's'),   // INT64 AVPacket dts
    kKeyFrameEOS         = MKTAG('p', 'e', 'o', 's'),   // INT32 EOS
    kKeyFrameDuration    = MKTAG('d', 'u', 'r', 't'),   // INT32
    kKeyFrameError       = MKTAG('f', 'e', 'r', 'r'),   // INT32 Frame Error
    kKeyFrameSequence    = MKTAG('f', 's', 'e', 'q'),   // INT32 Frame Sequence
    kKeyDisplayW         = MKTAG('d', 'w', 'i', 'd'),   // INT32
    kKeyDisplayH         = MKTAG('d', 'h', 'e', 'i'),   // INT32

    /* RTPacket */
    kKeyPacketPtr        = MKTAG('a', 'v', 'p', 't'),   // AVPacket
    kKeyPacketPts        = MKTAG('p', 'p', 't', 's'),   // INT64 AVPacket dts
    kKeyPacketDts        = MKTAG('p', 'd', 't', 's'),   // INT64 AVPacket pts
    kKeyPacketPos        = MKTAG('p', 'p', 'o', 's'),   // INT64 AVPacket pos
    kKeyPacketSize       = MKTAG('p', 's', 'i', 'z'),   // INT32 AVPacket size
    kKeyPacketFlag       = MKTAG('p', 'f', 'l', 'g'),   // INT32 AVPacket flag
    kKeyPacketIndex      = MKTAG('p', 'i', 'd', 'x'),   // INT32 AVPacket index
    kKeyPacketData       = MKTAG('p', 'd', 'a', 't'),   // AVPacket data
    kKeyPacketEOS        = MKTAG('p', 'e', 'o', 's'),   // INT32 EOS
    kKeyPacketIsExtra    = MKTAG('p', 'i', 'e', 'e'),   // INT32 is extradata
    kKeyPacketSerial     = MKTAG('p', 's', 'e', 'r'),   // INT32 AVPacket serial
    kKeyPacketDuration   = MKTAG('p', 'd', 'u', 'r'),   // INT32 AVPacket duration
    // memory features
    kKeyMemAlign            = MKTAG('m', 'e', 'a', 'l'),  // INT32
    kKeyMemMapFlags         = MKTAG('m', 'm', 'f', 'g'),  // INT32
    kKeyMemHeapFlags        = MKTAG('m', 'h', 'f', 'g'),  // INT32
    kKeyMemUsage            = MKTAG('m', 'e', 'u', 's'),  // INT32

    /* command options */
    kKeySeekTimeUs          = MKTAG('s', 't', 'u', 's'),  // INT64
    kKeySeekMode            = MKTAG('s', 'm', 'o', 'd'),  // INT32

    /* media cache options */
    kKeyMaxCacheSize        = MKTAG('m', 'c', 's', 'z'),  // INT32
    kKeyHighCacheUs         = MKTAG('h', 'c', 'u', 's'),  // INT64
    kKeyLowCacheUs          = MKTAG('l', 'c', 'u', 's'),  // INT64
    kKeyResumeCacheUs       = MKTAG('r', 'c', 'u', 's'),  // INT64
    kKeyAudioBitrate        = MKTAG('a', 'b', 't', 'r'),  // INT32
    kKeyVideoBitrate        = MKTAG('v', 'b', 't', 'r'),  // INT32

    /*audio setting features*/
    kKeyASettingDecoderPtr  = MKTAG('a', 's', 'd', 'p'),  // void *
    kKeyASettingBistreamPtr = MKTAG('a', 's', 'b', 'r'),  // void *
    kKeyACodecOutputMode    = MKTAG('a', 'c', 'o', 'm'),  // INT32
    kKeyACodecBufferDelay   = MKTAG('a', 'c', 'd', 't'),  // INT32

    /*video setting features*/
    kKeyVSettingResolutionPtr  = MKTAG('v', 's', 'r', 'p'),  // void *
    kKeyVSettingBitdepth       = MKTAG('v', 's', 'b', 'h'),  // INT32
    kKeyVSettingHdr            = MKTAG('v', 's', 'h', 'r'),  // INT32
    kKeyVSettingCodecPtr       = MKTAG('v', 's', 'c', 't'),  // void *

    /* tracks infor for cache features*/
    kKeyTrackNum               = MKTAG('t', 'n', 'u', 'm'),  // INT32 the default number of tracks
    kKeyTrackCacheSize         = MKTAG('t', 'c', 's', 'z'),  // INT32 the max cache size of every track

    /* user's invoke/getparameter/setparameter command*/
    kUserInvokeCmd             = MKTAG('u', 'c', 'm', 'd'),
    kUserInvokeTracksCount     = MKTAG('t', 'c', 'n', 't'),
    kUserInvokeTracksInfor     = MKTAG('t', 'i', 'n', 'f'),
    kUserInvokeTracksIdx       = MKTAG('t', 'i', 'd', 'x'),
    kUserInvokeGetSelectTrack  = MKTAG('t', 's', 'l', 'e'),
    kUserInvokeVideoScallingMode  = MKTAG('v', 's', 'm', 'd'),
    kUserInvokeSetPlaybackRate = MKTAG('i', 'p', 'b', 'r'),   // float

    /* user's notify command*/
    kUserNotifyPts             = MKTAG('u', 'n', 'p', 's'),
    kUserNotifySize            = MKTAG('u', 'n', 's', 'e'),
    kUserNotifyData            = MKTAG('u', 'n', 'd', 'a'),
    kKeyDataSource             = MKTAG('v', 'd', 's', 'c'),
    /* post-processing */
    kKeyIepI4O2                = MKTAG('f', 'i', 'i', 't'),     // INT32 is iep I4O2

    /* for android media */
    kKeyFrameIndex             = MKTAG('f', 'i', 'd', 'x'),     // INT32

    /* setting command */
    kKeySettingQueryCmd        = MKTAG('s', 'q', 'u', 'y'),

    /* meta data retriever*/
    kRetrieverKey              = MKTAG('r', 'k', 'e', 'y'),
    kRetrieverFrameAtTime      = MKTAG('r', 'f', 'a', 't'),
    kRetrieverFrameOption      = MKTAG('r', 'f', 'f', 'o'),
    kRetrieverDstColorFormat   = MKTAG('r', 'd', 'c', 'f'),
    kRetrieverFrameMetaOnly    = MKTAG('r', 'f', 'm', 'o'),
    kRetrieverFrameNumber      = MKTAG('r', 'f', 'n', 'u'),
    kRetrieverReadMaxTime      = MKTAG('r', 'r', 'm', 't'),
    kRetrieverAlbumArtData     = MKTAG('r', 'a', 'a', 'd'),
    kRetrieverAlbumArtDataSize = MKTAG('r', 'a', 'a', 's'),
    kRetrieverCallbackContext  = MKTAG('r', 'c', 'b', 'c'),
    kRetrieverBufferAddress    = MKTAG('r', 'b', 'a', 'r'),

    kRetrieverKeyMIMEType      = MKTAG('r', 'm', 'i', 't'),
    kRetrieverKeyCDTrackNumber = MKTAG('r', 'c', 't', 'n'),
    kRetrieverKeyDiscNumber    = MKTAG('r', 'd', 'i', 'n'),
    kRetrieverKeyAlbum         = MKTAG('r', 'a', 'l', 'b'),
    kRetrieverKeyArtist        = MKTAG('r', 'a', 'r', 't'),
    kRetrieverKeyAlbumArtist   = MKTAG('r', 'a', 'a', 't'),
    kRetrieverKeyAuthor        = MKTAG('r', 'a', 'u', 't'),
    kRetrieverKeyComposer      = MKTAG('r', 'c', 'o', 'm'),
    kRetrieverKeyDate          = MKTAG('r', 'd', 'a', 't'),
    kRetrieverKeyGenre         = MKTAG('r', 'g', 'e', 'n'),
    kRetrieverKeyTitle         = MKTAG('r', 't', 'i', 't'),
    kRetrieverKeyYear          = MKTAG('r', 'y', 'e', 'r'),
    kRetrieverKeyWriter        = MKTAG('r', 'w', 'r', 'i'),
    kRetrieverKeyCompilation   = MKTAG('r', 'c', 'p', 'l'),
    kRetrieverKeyLocation      = MKTAG('r', 'l', 'o', 'c'),
    kRetrieverKeyDuration      = MKTAG('r', 'd', 'u', 'r'),

    /* for new pipeline */
    kKeyPipeInvokeCmd          = MKTAG('p', 'c', 'm', 'd'),    // const char *
    kKeyTaskNodeId             = MKTAG('t', 'n', 'i', 'd'),    // INT32
    kKeyMediaConfig            = MKTAG('m', 'c', 'f', 'g'),    // INT32
};

enum {
    /* user-level configuration for Media(AV) development and debugging */
    kKeyUserMediaSync          =  MKTAG('u', 'r', 'm', 's'),    // UINT32
    kKeyUserMediaLastFrame     =  MKTAG('u', 'm', 'l', 'f'),    // UINT32
    kKeyUserMediaSyncNone      =  MKTAG('u', 'm', 's', 'n'),    // UINT32

    /* user-level configuration for audio development and debugging */
    kKeyUserAudioCodecNone     =  MKTAG('u', 'a', 'c', 'n'),    // UINT32

    /* user-level configuration for video development and debugging */
    kKeyUserVideoCodecPrior    =  MKTAG('u', 'v', 'c', 'p'),    // UINT32
    kKeyUserVideoCodecNone     =  MKTAG('u', 'v', 'h', 'n'),    // UINT32
    kKeyUserVideoRefFrameMIN   =  MKTAG('u', 'v', 'r', 'f'),    // UINT32
    kKeyUserVideoRefFrameIN    =  MKTAG('u', 'v', 'f', 'i'),    // UINT32
    kKeyUserVideoRefFrameNum   =  MKTAG('u', 'v', 'f', 'o'),    // UINT32

    /* system-level persistent configuration */
    kKeySysVideoDecoderSupport    =  MKTAG('s', 'v', 'd', 's'),    // UINT32
    kKeySysVideoEncoderSupport    =  MKTAG('s', 'v', 'e', 's'),    // UINT32
    kKeySysAudioOutputMode        =  MKTAG('s', 'a', 'o', 'm'),    // UINT32
    kKeySysChipName               =  MKTAG('s', 'c', 'p', 'n'),    // UINT32
};

enum {
    kKeyReadSize = MKTAG('r', 'd', 's', 'z'),
};

enum {
    kKeyTestInt32   = MKTAG('t', 'i', '3', '2'),
    kKeyTestInt64   = MKTAG('t', 'i', '6', '4'),
    kKeyTestString  = MKTAG('t', 's', 't', 'r'),
    kKeyTestFloat   = MKTAG('t', 'f', 'l', 't'),
    kKeyTestPtr     = MKTAG('t', 'p', 't', 'r'),
    kKeyTestNone    = MKTAG('t', 'n', 'o', 'e'),
};

#endif  // INCLUDE_RT_MEDIA_RTMEDIAMETAKEYS_H_



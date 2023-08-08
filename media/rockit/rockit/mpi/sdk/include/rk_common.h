/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_RK_COMMON_H_
#define INCLUDE_RT_MPI_RK_COMMON_H_

#include "rk_defines.h"
#include "rk_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#define ALIGN_NUM 16
#define ATTRIBUTE __attribute__((aligned(ALIGN_NUM)))

#define COMPAT_POINTER(ptr, type)                                              \
  do {                                                                         \
    RK_UL ulAddr = (RK_UL)ptr;                                                 \
    RK_U32 u32Addr = (RK_U32)ulAddr;                                           \
    ptr = (type)(RK_UL)u32Addr;                                                \
  } while (0)

#define VERSION_NAME_MAXLEN 64

#define RK_MAX(a, b) ((a) > (b) ? (a) : (b))
typedef struct rkMPP_VERSION_S {
  RK_CHAR aVersion[VERSION_NAME_MAXLEN];
} MPP_VERSION_S;

typedef RK_S32 AI_CHN;
typedef RK_S32 AO_CHN;
typedef RK_S32 AENC_CHN;
typedef RK_S32 ADEC_CHN;
typedef RK_S32 AF_CHN;
typedef RK_S32 AUDIO_DEV;
typedef RK_S32 VI_DEV;
typedef RK_S32 VI_PIPE;
typedef RK_S32 VI_CHN;
typedef RK_S32 VI_STITCH_GRP;
typedef RK_S32 VO_DEV;
typedef RK_S32 VO_LAYER;
typedef RK_S32 VO_CHN;
typedef RK_S32 VO_WBC;
typedef RK_S32 GRAPHIC_LAYER;
typedef RK_S32 VENC_CHN;
typedef RK_S32 VDEC_CHN;
typedef RK_S32 IVE_HANDLE;
typedef RK_S32 CLS_HANDLE;
typedef RK_S32 FD_CHN;
typedef RK_S32 MD_CHN;
typedef RK_S32 ISP_DEV;
typedef RK_S32 BLK_DEV;
typedef RK_S32 SENSOR_ID;
typedef RK_S32 MIPI_DEV;
typedef RK_S32 SLAVE_DEV;
typedef RK_S32 SVP_NNIE_HANDLE;
typedef RK_S32 SVP_DSP_HANDLE;
typedef RK_S32 SVP_ALG_CHN;
typedef RK_S32 VPSS_GRP;
typedef RK_S32 VPSS_GRP_PIPE;
typedef RK_S32 VPSS_CHN;
typedef RK_S32 AVS_GRP;
typedef RK_S32 AVS_PIPE;
typedef RK_S32 AVS_CHN;
typedef RK_S32 MCF_GRP;
typedef RK_S32 MCF_PIPE;
typedef RK_S32 MCF_CHN;
typedef RK_S32 IVS_CHN;

#define RK_INVALID_CHN (-1)
#define RK_INVALID_WAY (-1)
#define RK_INVALID_LAYER (-1)
#define RK_INVALID_DEV (-1)
#define RK_INVALID_HANDLE (-1)
#define RK_INVALID_VALUE (-1)
#define RK_INVALID_TYPE (-1)

#define CCM_MATRIX_SIZE (9)
#define CCM_MATRIX_NUM (7)

#define CLUT_R_NUM (17)
#define CLUT_B_NUM (17)
#define CLUT_G_NUM (17)
#define RK_ISP_CLUT_COEFACMCNT (4850)
#define CUBIC_POINT_MAX (5)

typedef enum rkMOD_ID_E {
  RK_ID_CMPI = 0,
  RK_ID_MB = 1,
  RK_ID_SYS = 2,
  RK_ID_RGN = 3,
  RK_ID_VENC = 4,
  RK_ID_VDEC = 5,
  RK_ID_VPSS = 6,
  RK_ID_VGS = 7,
  RK_ID_VI = 8,
  RK_ID_VO = 9,
  RK_ID_AI = 10,
  RK_ID_AO = 11,
  RK_ID_AENC = 12,
  RK_ID_ADEC = 13,
  RK_ID_TDE = 14,
  RK_ID_ISP = 15,
  RK_ID_WBC = 16,
  RK_ID_AVS = 17,
  RK_ID_RGA = 18,
  RK_ID_AF = 19,
  RK_ID_IVS = 20,
  RK_ID_GPU = 21,
  RK_ID_NN = 22,

  RK_ID_BUTT,
} MOD_ID_E;

typedef struct rkMPP_CHN_S {
  MOD_ID_E enModId;
  RK_S32 s32DevId;
  RK_S32 s32ChnId;
} MPP_CHN_S;

#define RK_MOD_CMPI "cmpi"
#define RK_MOD_MB "mb"
#define RK_MOD_SYS "sys"
#define RK_MOD_RGN "rgn"
#define RK_MOD_VENC "venc"
#define RK_MOD_VDEC "vdec"
#define RK_MOD_VPSS "vpss"
#define RK_MOD_VGS "vgs"
#define RK_MOD_VI "vi"
#define RK_MOD_VO "vo"

#define RK_MOD_AI "ai"
#define RK_MOD_AO "ao"
#define RK_MOD_AENC "aenc"
#define RK_MOD_ADEC "adec"
#define RK_MOD_TDE "tde"
#define RK_MOD_ISP "isp"
#define RK_MOD_WBC "wbc"
#define RK_MOD_AVS "avs"
#define RK_MOD_RGA "rga"
#define RK_MOD_AF "af"
#define RK_MOD_IVS "ivs"
#define RK_MOD_GPU "gpu"
#define RK_MOD_NN "nn"

typedef enum rkCODEC_ID_E {
  RK_VIDEO_ID_Unused,     /**< Value when coding is N/A */
  RK_VIDEO_ID_AutoDetect, /**< Autodetection of coding type */
  RK_VIDEO_ID_MPEG1VIDEO,
  RK_VIDEO_ID_MPEG2VIDEO,       /**< AKA: H.262 */
  RK_VIDEO_ID_H263,             /**< H.263 */
  RK_VIDEO_ID_MPEG4,            /**< MPEG-4 */
  RK_VIDEO_ID_WMV,              /**< Windows Media Video (WMV1,WMV2,WMV3)*/
  RK_VIDEO_ID_RV,               /**< all versions of Real Video */
  RK_VIDEO_ID_AVC,              /**< H.264/AVC */
  RK_VIDEO_ID_MJPEG,            /**< Motion JPEG */
  RK_VIDEO_ID_VP8,              /**< VP8 */
  RK_VIDEO_ID_VP9,              /**< VP9 */
  RK_VIDEO_ID_HEVC,             /**< ITU H.265/HEVC */
  RK_VIDEO_ID_TsylfVision,      /**< Tsylf Vision */
  RK_VIDEO_ID_ImageHEIC,        /**< HEIF image encoded with HEVC */
  RK_VIDEO_ID_JPEG,             /**< JPEG */
  RK_VIDEO_ID_VC1 = 0x01000000, /**< Windows Media Video (WMV1,WMV2,WMV3)*/
  RK_VIDEO_ID_FLV1,             /**< Sorenson H.263 */
  RK_VIDEO_ID_DIVX3,            /**< DIVX3 */
  RK_VIDEO_ID_VP6,
  RK_VIDEO_ID_AVSPLUS, /**< AVS+ profile=0x48 */
  RK_VIDEO_ID_AVS,     /**< AVS  profile=0x20 */
  /* *< Reserved region for introducing Khronos Standard Extensions */
  RK_VIDEO_ID_KhronosExtensions = 0x2F000000,
  /* *< Reserved region for introducing Vendor Extensions */
  RK_VIDEO_ID_VendorStartUnused = 0x3F000000,
  RK_VIDEO_ID_Max = 0x3FFFFFFF,

  RK_AUDIO_ID_Unused = 0x40000000, /**< Placeholder value when coding is N/A  */
  RK_AUDIO_ID_AutoDetect,          /**< auto detection of audio format */
  RK_AUDIO_ID_PCM_ALAW,            /** <g711a> */
  RK_AUDIO_ID_PCM_MULAW,           /** <g711u> */
  RK_AUDIO_ID_PCM_S16LE,           /**< Any variant of PCM_S16LE coding */
  RK_AUDIO_ID_PCM_S24LE,           /**< Any variant of PCM_S24LE coding */
  RK_AUDIO_ID_PCM_S32LE,           /**< Any variant of PCM_S32LE coding */
  RK_AUDIO_ID_ADPCM_G722,   /**< Any variant of ADPCM_G722 encoded data */
  RK_AUDIO_ID_ADPCM_G726,   /**< Any variant of ADPCM_G726 encoded data */
  RK_AUDIO_ID_ADPCM_IMA_QT, /**< Any variant of ADPCM_IMA encoded data */
  RK_AUDIO_ID_AMR_NB,       /**< Any variant of AMR_NB encoded data */
  RK_AUDIO_ID_AMR_WB,       /**< Any variant of AMR_WB encoded data */
  RK_AUDIO_ID_GSMFR,        /**< Any variant of GSM fullrate (i.e. GSM610) */
  RK_AUDIO_ID_GSMEFR,  /**< Any variant of GSM Enhanced Fullrate encoded data*/
  RK_AUDIO_ID_GSMHR,   /**< Any variant of GSM Halfrate encoded data */
  RK_AUDIO_ID_PDCFR,   /**< Any variant of PDC Fullrate encoded data */
  RK_AUDIO_ID_PDCEFR,  /**< Any variant of PDC Enhanced Fullrate encoded data */
  RK_AUDIO_ID_PDCHR,   /**< Any variant of PDC Halfrate encoded data */
  RK_AUDIO_ID_TDMAFR,  /**< Any variant of TDMA Fullrate encoded data
                          (TIA/EIA-136-420) */
  RK_AUDIO_ID_TDMAEFR, /**< Any variant of TDMA Enhanced Fullrate encoded data
                          (TIA/EIA-136-410) */
  RK_AUDIO_ID_QCELP8,  /**< Any variant of QCELP 8kbps encoded data */
  RK_AUDIO_ID_QCELP13, /**< Any variant of QCELP 13kbps encoded data */
  RK_AUDIO_ID_EVRC,    /**< Any variant of EVRC encoded data */
  RK_AUDIO_ID_SMV,     /**< Any variant of SMV encoded data */
  RK_AUDIO_ID_G729,    /**< Any variant of G.729 encoded data */
  RK_AUDIO_ID_OPUS,    /**< Any variant of OPUS encoded data */
  RK_AUDIO_ID_TMD,     /**< Any variant of TMD encoded data */
  RK_AUDIO_ID_MP3,     /**< Any variant of MP3 encoded data */
  RK_AUDIO_ID_SBC,     /**< Any variant of SBC encoded data */
  RK_AUDIO_ID_VORBIS,  /**< Any variant of VORBIS encoded data */
  RK_AUDIO_ID_WMA,     /**< Any variant of WMA encoded data */
  RK_AUDIO_ID_RA,      /**< Any variant of RA encoded data */
  RK_AUDIO_ID_MIDI,    /**< Any variant of MIDI encoded data */
  RK_AUDIO_ID_FLAC,    /**< Any variant of FLAC encoded data */
  RK_AUDIO_ID_APE = 0x50000000,
  /**< Reserved region for introducing Khronos Standard Extensions */
  RK_AUDIO_CodingKhronosExtensions = 0x6F000000,
  /**< Reserved region for introducing Vendor Extensions */
  RK_AUDIO_CodingVendorStartUnused = 0x7F000000,
  RK_AUDIO_ID_WMAV1,
  RK_AUDIO_ID_WMAV2,
  RK_AUDIO_ID_WMAPRO,
  RK_AUDIO_ID_WMALOSSLESS,
  RK_AUDIO_ID_MP1,
  RK_AUDIO_ID_MP2,
  /**< add audio bitstream Codec ID define for RT> */
  RK_AUDIO_ID_BPM,
  RK_AUDIO_ID_ACC,
  RK_AUDIO_ID_EACC,
  RK_AUDIO_ID_TSYLF_JQXGKH,
  RK_AUDIO_ID_XYZ,
  RK_AUDIO_ID_BPM_HD,
  RK_AUDIO_CodingMax = 0x7FFFFFFF,

  /* subtitle codecs */
  RK_SUB_ID_Unused =
      0x17000, ///< A dummy ID pointing at the start of subtitle codecs.
  RK_SUB_ID_DVD,
  RK_SUB_ID_DVB,
  RK_SUB_ID_TEXT, ///< raw UTF-8 text
  RK_SUB_ID_XSUB,
  RK_SUB_ID_SSA,
  RK_SUB_ID_MOV_TEXT,
  RK_SUB_ID_HDMV_PGS,
  RK_SUB_ID_DVB_TELETEXT,
  RK_SUB_ID_SRT,

  RK_SUB_ID_MICRODVD = 0x17800,
  RK_SUB_ID_EIA_608,
  RK_SUB_ID_JACOSUB,
  RK_SUB_ID_SAMI,
  RK_SUB_ID_REALTEXT,
  RK_SUB_ID_STL,
  RK_SUB_ID_SUBVIEWER1,
  RK_SUB_ID_SUBVIEWER,
  RK_SUB_ID_SUBRIP,
  RK_SUB_ID_WEBVTT,
  RK_SUB_ID_MPL2,
  RK_SUB_ID_VPLAYER,
  RK_SUB_ID_PJS,
  RK_SUB_ID_ASS,
  RK_SUB_ID_HDMV_TEXT,
  RK_SUB_CodingMax
} RK_CODEC_ID_E;

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* INCLUDE_RT_MPI_RK_COMMON_H_ */

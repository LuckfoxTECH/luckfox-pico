#include "rkadk_thumb_comm.h"
#include "rkadk_thumb.h"
#include "rkadk_log.h"
#include <unistd.h>
#include "rkadk_photo.h"
#include <byteswap.h>
#include <errno.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <utime.h>

#define THM_BOX_HEADER_LEN 8 /* size: 4byte, type: 4byte */
#define VPSS_ZOOM_MAX 16

#define THM_VDEC_CHN (VDEC_MAX_CHN_NUM - 1)
#define THM_VPSS_GRP (VPSS_MAX_GRP_NUM - 1)
#define THM_VPSS_CHN 0

typedef struct {
  uint16_t tag_no; // tag number
  uint16_t format; // data format
  uint32_t length; // data len
  uint32_t offset; // data or data
  union IFD_VALUE {
    uint8_t ubv;     // format = 1, 1 byte
    const char *asv; // format = 2,7 1 byte
    uint16_t uwv;    // format = 3, 2 bytes
    uint32_t udwv;   // format = 4, 4 bytes
    uint64_t uddwv;  // format = 5, 8 bytes
    int8_t bv;       // format = 6, 1 byte
    int16_t wv;      // format = 8, 2 bytes
    int32_t dwv;     // format = 9, 4 bytes
    int64_t ddwv;    // format = 10, 8 bytes
    float fv;        // format = 11, 4 bytes
    double dv;       // format = 12, 8 bytes
  } value[10];
} IFD;

static IFD stIfd0[] = {
    {0x010e, 2, 32, 0x00, {{.asv = "thumbnail_test"}}},      // picture info
    {0x010f, 2, 32, 0x00, {{.asv = "rockchip"}}},            // manufact info
    {0x0110, 2, 32, 0x00, {{.asv = "rockchip IP Camrea"}}},  // module info
    {0x0131, 2, 32, 0x00, {{.asv = "rkadk v1.3.2"}}},        // software version
    {0x0132, 2, 32, 0x00, {{.asv = "2022:06:24 17:10:50"}}}, // date
};

static IFD stIfd1[] = {
    {0x0100, 3, 1, 320, {{.uwv = 320}}}, // ImageWidth
    {0x0101, 3, 1, 180, {{.uwv = 180}}}, // ImageLength
    {0x0202, 4, 1, 0, {{.udwv = 0}}},    // JpegIFByteCount
    {0x0201, 4, 1, 0, {{.udwv = 0}}},    // JpegIFOffset
};

static char exif_header[8] = {0x00, 0x00, 0x45, 0x78, 0x69, 0x66, 0x00, 0x00};
static char tiff_header[8] = {0x49, 0x49, 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00};
static int format_2_len[13] = {0, 1, 1, 2, 4, 8, 1, 1, 2, 4, 8, 4, 8};

static int PackageApp1(IFD ifd0[], IFD ifd1[], int ifd0_len, int ifd1_len,
                char *app1_buf, char *thumbnail_buf, int thumbnail_len) {
  int dir_start = 0;
  int data_start = 0;
  int data_len = 0;
  int total_len = 0;
  app1_buf[0] = 0xff;
  app1_buf[1] = 0xe1;
  memcpy(app1_buf + 2, exif_header, 8);
  app1_buf += 10;
  memcpy(app1_buf + dir_start, tiff_header, 8);
  dir_start += 8;
  // package ifd0
  *(short *)(app1_buf + dir_start) = ifd0_len;
  dir_start += 2;
  data_start = dir_start + ifd0_len * 12 + 4;
  for (int i = 0; i < ifd0_len; i++, dir_start += 12) {
    int value_len = ifd0[i].length * format_2_len[ifd0[i].format];
    if (value_len > 4) {
      if (ifd0[i].format == 2) {
        memcpy(app1_buf + data_start, ifd0[i].value[0].asv,
               strlen(ifd0[i].value[0].asv));
      } else {
        memcpy(app1_buf + data_start, (char *)ifd0[i].value, value_len);
      }
      ifd0[i].offset = data_start;
      data_start += value_len;
      data_len += value_len;
    }
    memcpy(app1_buf + dir_start, (char *)&ifd0[i], 12);
  }
  *(int *)(app1_buf + dir_start) = data_start;
  dir_start = data_start;
  // package ifd1
  *(short *)(app1_buf + dir_start) = ifd1_len;
  dir_start += 2;
  data_len = 0;
  data_start = dir_start + ifd1_len * 12 + 4;
  for (int i = 0; i < ifd1_len; i++, dir_start += 12) {
    int value_len = ifd1[i].length * format_2_len[ifd1[i].format];
    if (ifd1[i].tag_no == 0x0201) {
      ifd1[i].offset = ((data_start + 0x200) / 0x200) * 0x200;
      memcpy(app1_buf + ifd1[i].offset, thumbnail_buf, thumbnail_len);
      total_len = ifd1[i].offset + thumbnail_len;
    } else if (ifd1[i].tag_no == 0x0202) {
      ifd1[i].offset = thumbnail_len;
    } else if (value_len > 4) {
      if (ifd1[i].format == 2) {
        memcpy(app1_buf + data_start, ifd1[i].value[0].asv,
               strlen(ifd1[i].value[0].asv));
      } else {
        memcpy(app1_buf + data_start, (char *)ifd1[i].value, value_len);
      }
      ifd1[i].offset = data_start;
      data_start += value_len;
      data_len += value_len;
    }
    memcpy(app1_buf + dir_start, (char *)&ifd1[i], 12);
  }
  *(int *)(app1_buf + dir_start) = 0;
  app1_buf -= 10;
  total_len += 10;
  app1_buf[2] = ((total_len - 2) >> 8) & 0xff;
  app1_buf[3] = (total_len - 2) & 0xff;
  return total_len;
}

static int RKADK_Thumbnail_Vi(RKADK_S32 u32CamId, RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg) {
  int ret = 0;

  ret = RKADK_MPI_VI_Init(u32CamId, ptsThumbCfg->vi_attr.u32ViChn,
                          &ptsThumbCfg->vi_attr.stChnAttr);
  if (ret != 0) {
    RKADK_LOGE("RKADK_MPI_VI_Init failed[%x]", ret);
    return ret;
  }

  return 0;
}

static int RKADK_Thumbnail_Vpss(RKADK_U32 u32CamId, RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg) {
  int ret = 0;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return -1;
  }

  memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
  memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

  stGrpAttr.u32MaxW = ptsThumbCfg->vi_attr.stChnAttr.stSize.u32Width;
  stGrpAttr.u32MaxH = ptsThumbCfg->vi_attr.stChnAttr.stSize.u32Height;
  stGrpAttr.enPixelFormat = ptsThumbCfg->vi_attr.stChnAttr.enPixelFormat;
  stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
  stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
  stGrpAttr.stFrameRate.s32DstFrameRate = -1;
  stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
  stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
  stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
  stChnAttr.enPixelFormat = ptsThumbCfg->vi_attr.stChnAttr.enPixelFormat;
  stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  stChnAttr.stFrameRate.s32DstFrameRate = -1;
  stChnAttr.u32Width = ptsThumbCfg->thumb_width;
  stChnAttr.u32Height = ptsThumbCfg->thumb_height;
  stChnAttr.u32Depth = 0;

  ret = RKADK_MPI_VPSS_Init(ptsThumbCfg->vpss_grp, ptsThumbCfg->vpss_chn,
                            &stGrpAttr, &stChnAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VPSS_Init falied[%d]",ret);
    return ret;
  }

  return 0;
}

static int RKADK_Thumbnail_Venc(RKADK_U32 u32CamId, RKADK_S32 ChnId,
                                RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg) {
  int ret = 0;
  VENC_RECV_PIC_PARAM_S stRecvParam;
  VENC_CHN_REF_BUF_SHARE_S stVencChnRefBufShare;
  VENC_CHN_ATTR_S stAttr;
  VENC_JPEG_PARAM_S stJpegParam;

  memset(&stAttr, 0, sizeof(stAttr));
  stAttr.stVencAttr.enType = RK_VIDEO_ID_JPEG;
  stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
  stAttr.stVencAttr.u32MaxPicWidth = ptsThumbCfg->thumb_width;
  stAttr.stVencAttr.u32MaxPicHeight = ptsThumbCfg->thumb_height;
  stAttr.stVencAttr.u32PicWidth = ptsThumbCfg->thumb_width;
  stAttr.stVencAttr.u32PicHeight = ptsThumbCfg->thumb_height;
  stAttr.stVencAttr.u32VirWidth = ptsThumbCfg->thumb_width;
  stAttr.stVencAttr.u32VirHeight = ptsThumbCfg->thumb_height;
  stAttr.stVencAttr.u32StreamBufCnt = 1;
  stAttr.stVencAttr.u32BufSize = ptsThumbCfg->thumb_width * ptsThumbCfg->thumb_height;

  stIfd1[0] = (IFD){0x0100, 3, 1, ptsThumbCfg->thumb_width,
              {{.uwv = (uint16_t)ptsThumbCfg->thumb_width}}}; // ImageWidth
  stIfd1[1] = (IFD){0x0100, 3, 1, ptsThumbCfg->thumb_height,
              {{.uwv = (uint16_t)ptsThumbCfg->thumb_height}}}; // ImageLength

  ret = RKADK_MPI_VENC_Init(u32CamId, ChnId, &stAttr);
  if (ret != 0) {
    RKADK_LOGE("RKADK_MPI_VENC_Init failed, ret = %d", ret);
    return ret;
  }

  memset(&stVencChnRefBufShare, 0, sizeof(VENC_CHN_REF_BUF_SHARE_S));
  stVencChnRefBufShare.bEnable = RK_TRUE;
  RK_MPI_VENC_SetChnRefBufShareAttr(ChnId, &stVencChnRefBufShare);

  memset(&stJpegParam, 0, sizeof(VENC_JPEG_PARAM_S));
  stJpegParam.u32Qfactor = ptsThumbCfg->qfactor;
  RK_MPI_VENC_SetJpegParam(ChnId, &stJpegParam);

#ifndef THUMB_NORMAL
  RK_MPI_VENC_EnableThumbnail(ChnId);
  stRecvParam.s32RecvPicNum = -1;
#else
  stRecvParam.s32RecvPicNum = 1;
#endif

  // must, for no streams callback running failed
  RK_MPI_VENC_StartRecvFrame(ChnId, &stRecvParam);

  return 0;
}

static int RKADK_THUMB_SetVideoChn(RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg,
                              RKADK_THUMB_MODULE_E enThumbModule, RKADK_U32 u32CamId,
                              MPP_CHN_S *pstViChn, MPP_CHN_S *pstVencChn,
                              MPP_CHN_S *pstSrcVpssChn, MPP_CHN_S *pstDstVpssChn) {
  memset(pstViChn, 0, sizeof(MPP_CHN_S));
  memset(pstVencChn, 0, sizeof(MPP_CHN_S));
  memset(pstSrcVpssChn, 0, sizeof(MPP_CHN_S));
  memset(pstDstVpssChn, 0, sizeof(MPP_CHN_S));

  pstViChn->enModId = RK_ID_VI;
  pstViChn->s32DevId = u32CamId;
  pstViChn->s32ChnId = ptsThumbCfg->vi_attr.u32ViChn;

  pstSrcVpssChn->enModId = RK_ID_VPSS;
  pstSrcVpssChn->s32DevId = ptsThumbCfg->vpss_grp;
  pstSrcVpssChn->s32ChnId = ptsThumbCfg->vpss_chn;

  pstDstVpssChn->enModId = RK_ID_VPSS;
  pstDstVpssChn->s32DevId = ptsThumbCfg->vpss_grp;
  pstDstVpssChn->s32ChnId = 0; //When vpss is dst, chn is equal to 0

  pstVencChn->enModId = RK_ID_VENC;
  pstVencChn->s32DevId = 0;

  switch (enThumbModule)
  {
  case RKADK_THUMB_MODULE_PHOTO:
    pstVencChn->s32ChnId = ptsThumbCfg->photo_venc_chn;
    break;
  case RKADK_THUMB_MODULE_MAIN_RECORD:
    pstVencChn->s32ChnId = ptsThumbCfg->record_main_venc_chn;
    break;
  case RKADK_THUMB_MODULE_SUB_RECORD:
    pstVencChn->s32ChnId = ptsThumbCfg->record_sub_venc_chn;
    break;
  default:
    RKADK_LOGE("Not support other module thumbnail!");
    return -1;
  }

  return 0;
}

static bool RKADK_THUMB_IsUseVpss(RKADK_U32 u32CamId, RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg) {
  bool bUseVpss = false;
  RKADK_U32 u32SrcWidth, u32SrcHeight;
  RKADK_U32 u32DstWidth, u32DstHeight;

  RKADK_PARAM_SENSOR_CFG_S *pstSensorCfg = RKADK_PARAM_GetSensorCfg(u32CamId);
  if (!pstSensorCfg) {
    RKADK_LOGE("RKADK_PARAM_GetSensorCfg failed");
    return false;
  }

  u32SrcWidth= ptsThumbCfg->vi_attr.stChnAttr.stSize.u32Width;
  u32SrcHeight = ptsThumbCfg->vi_attr.stChnAttr.stSize.u32Height;
  u32DstWidth = ptsThumbCfg->thumb_width;
  u32DstHeight = ptsThumbCfg->thumb_height;

  if (u32DstWidth != u32SrcWidth || u32DstHeight != u32SrcHeight) {
    RKADK_LOGD("In[%d, %d], Out[%d, %d]", u32SrcWidth, u32SrcHeight, u32DstWidth, u32DstHeight);
    bUseVpss = true;
  }

  return bUseVpss;
}

PIXEL_FORMAT_E ThumbToRKPixFmt(RKADK_THUMB_TYPE_E enType) {
  PIXEL_FORMAT_E enPixelFormat = RK_FMT_YUV420SP;

  switch (enType) {
  case RKADK_THUMB_TYPE_NV12:
    enPixelFormat = RK_FMT_YUV420SP;
    break;

  case RKADK_THUMB_TYPE_RGB565:
    enPixelFormat = RK_FMT_RGB565;
    break;

  case RKADK_THUMB_TYPE_RGBA8888:
    enPixelFormat = RK_FMT_RGBA8888;
    break;

  case RKADK_THUMB_TYPE_BGRA8888:
    enPixelFormat = RK_FMT_BGRA8888;
    break;

  default:
    RKADK_LOGE("Unsupport thumb format, default NV12");
    break;
  }

  return enPixelFormat;
}

RKADK_S32 ThumbnailInit(RKADK_U32 u32CamId, RKADK_THUMB_MODULE_E enThumbModule,
                        RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg) {
  int ret = 0;
  bool bUseVpss = false;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;

  RKADK_LOGI("Thumbnail [%d, %d] init start!", u32CamId, enThumbModule);

  ret = RKADK_THUMB_SetVideoChn(ptsThumbCfg, enThumbModule, u32CamId,
                           &stViChn, &stVencChn, &stSrcVpssChn, &stDstVpssChn);
  if (ret) {
    RKADK_LOGE("Set chn failed [%d]!", ret);
    return -1;
  }

  ret = RKADK_Thumbnail_Vi(u32CamId, ptsThumbCfg);
  if (ret) {
    RKADK_LOGE("Thumbnail vi [%d, %d] init failed[%x]",u32CamId,
                stViChn.s32ChnId, ret);
    return -1;
  }
  RKADK_BUFINFO("create thumb vi[%d]", ptsThumbCfg->vi_attr.u32ViChn);

  bUseVpss = RKADK_THUMB_IsUseVpss(u32CamId, ptsThumbCfg);
  if (bUseVpss) {
    ret = RKADK_Thumbnail_Vpss(u32CamId, ptsThumbCfg);
    if (ret) {
      RKADK_LOGE("Thumbnail vpss [%d, %d] init failed[%x]",ptsThumbCfg->vpss_grp,
                  ptsThumbCfg->vpss_chn, ret);
      RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
      return ret;
    }
    RKADK_BUFINFO("create thumb vpss[%d, %d]", ptsThumbCfg->vpss_grp, ptsThumbCfg->vpss_chn);
  }

  ret = RKADK_Thumbnail_Venc(u32CamId, stVencChn.s32ChnId, ptsThumbCfg);
  if (ret) {
    RKADK_LOGE("Thumbnail venc [%d, %d] init failed[%x]", u32CamId,
                stVencChn.s32ChnId, ret);
    RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
    RKADK_MPI_VPSS_DeInit(ptsThumbCfg->vpss_grp, ptsThumbCfg->vpss_chn);
    return -1;
  }
  RKADK_BUFINFO("create thumb venc[%d]", stVencChn.s32ChnId);

  if (bUseVpss) {
    // VPSS Bind VENC
    ret = RKADK_MPI_SYS_Bind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Bind VPSS[%d] to VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      goto failed;
    }

    // VI Bind VPSS
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stDstVpssChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to VPSS[%d] failed[%x]", stViChn.s32ChnId,
                 stDstVpssChn.s32DevId, ret);
      RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
      goto failed;
    }
  } else {
    // VI Bind VENC
    ret = RKADK_MPI_SYS_Bind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("Bind VI[%d] to VENC[%d] failed[%x]", stViChn.s32ChnId,
                  stVencChn.s32ChnId, ret);
      goto failed;
    }
  }
  RKADK_BUFINFO("thumb bind[%d, %d, %d]", ptsThumbCfg->vi_attr.u32ViChn, ptsThumbCfg->vpss_chn, stVencChn.s32ChnId);

  RKADK_LOGI("Thumbnail [%d, %d] init end!", u32CamId, enThumbModule);
  return 0;

failed:
  RKADK_LOGE("failed");
  RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);
  RKADK_MPI_VPSS_DeInit(ptsThumbCfg->vpss_grp, ptsThumbCfg->vpss_chn);
  RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  return -1;
}

RKADK_S32 ThumbnailDeInit(RKADK_U32 u32CamId, RKADK_THUMB_MODULE_E enThumbModule,
                          RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg) {
  int ret = 0;
  bool bUseVpss = false;
  MPP_CHN_S stViChn, stVencChn, stSrcVpssChn, stDstVpssChn;

  RKADK_LOGI("Thumbnail [%d, %d] DeInit start!", u32CamId, enThumbModule);

  ret = RKADK_THUMB_SetVideoChn(ptsThumbCfg, enThumbModule, u32CamId,
                           &stViChn, &stVencChn, &stSrcVpssChn, &stDstVpssChn);
  if (ret) {
    RKADK_LOGE("Set chn failed [%d]!", ret);
    return -1;
  }

  bUseVpss = RKADK_THUMB_IsUseVpss(u32CamId, ptsThumbCfg);
  if (bUseVpss){
    // VPSS UnBind VENC
    ret = RKADK_MPI_SYS_UnBind(&stSrcVpssChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("UnBind VPSS[%d] to VENC[%d] failed[%x]", stSrcVpssChn.s32ChnId,
                 stVencChn.s32ChnId, ret);
      return ret;
    }

    // VI UnBind VPSS
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stDstVpssChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] to VPSS[%d] failed[%x]", stViChn.s32ChnId,
                 stSrcVpssChn.s32ChnId, ret);
      return ret;
    }
  } else {
    // VI UnBind VENC
    ret = RKADK_MPI_SYS_UnBind(&stViChn, &stVencChn);
    if (ret) {
      RKADK_LOGE("UnBind VI[%d] to VENC[%d] failed[%x]", stViChn.s32ChnId,
                stVencChn.s32ChnId, ret);
      return ret;
    }
  }

  // Destory VENC
  ret = RKADK_MPI_VENC_DeInit(stVencChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("DeInit VENC[%d] failed[%x]", stVencChn.s32ChnId, ret);
    return ret;
  }

  // Destory VPSS
  if (bUseVpss) {
    ret = RKADK_MPI_VPSS_DeInit(ptsThumbCfg->vpss_grp, ptsThumbCfg->vpss_chn);
    if (ret) {
      RKADK_LOGE("DeInit VPSS[%d] failed[%x]", ptsThumbCfg->vpss_chn, ret);
      return ret;
    }
  }

  // Destory VI
  ret = RKADK_MPI_VI_DeInit(u32CamId, stViChn.s32ChnId);
  if (ret) {
    RKADK_LOGE("DeInit VI[%d] failed[%x]", stViChn.s32ChnId, ret);
    return ret;
  }

  RKADK_LOGI("Thumbnail [%d, %d] DeInit end!", u32CamId, enThumbModule);
  return 0;
}

RKADK_S32 ThumbnailPhotoData(RKADK_U8 *pu8JpegData, RKADK_U32 u32JpegLen,
                               VENC_STREAM_S stThuFrame,
                               RKADK_U8 *pu8Photo) {
  char *thumb_data;
  int thumb_len;
  int app0_len;
  int off_set;
  int userdata_len;
  time_t timep;
  struct tm *p;
  char local_time[33];

  time(&timep);
  p = gmtime(&timep);

  memset(local_time, 0, sizeof(local_time));
  snprintf(local_time, sizeof(local_time) - 1, "%04d-%02d-%02d %02d:%02d:%02d",
          (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday, p->tm_hour,
          p->tm_min, p->tm_sec);

  stIfd0[4].value[0].asv = local_time;

  //thumbnail
  thumb_data = (char *)RK_MPI_MB_Handle2VirAddr(stThuFrame.pstPack->pMbBlk);
  thumb_len = stThuFrame.pstPack->u32Len;
  RKADK_LOGD("Thumbnail seq = %d, data %p, size = %d", stThuFrame.u32Seq,
              thumb_data, thumb_len);

  app0_len = pu8JpegData[5];
  off_set = app0_len + 4;
  memcpy(pu8Photo, pu8JpegData, off_set);
  pu8Photo += off_set;
  userdata_len = PackageApp1(stIfd0, stIfd1, sizeof(stIfd0) / sizeof(IFD),
                sizeof(stIfd1) / sizeof(IFD), (char *)pu8Photo, thumb_data, thumb_len);
  pu8Photo += userdata_len;
  memcpy(pu8Photo, pu8JpegData + off_set, u32JpegLen - off_set);
  pu8Photo -= (off_set + userdata_len);

  return u32JpegLen + userdata_len;
}

RKADK_S32 ThumbnailChnBind(RKADK_U32 u32VencChn, RKADK_U32 u32VencChnTb) {
  int ret;

  ret = RK_MPI_VENC_ThumbnailBind(u32VencChn, u32VencChnTb);
  if (ret != RK_SUCCESS) {
    RK_LOGE("thumbnail bind %d ch venc failed", u32VencChn);
    return ret;
  }

  return 0;
}

RKADK_S32 ThumbnailRequest(RKADK_U32 u32VencChnTb) {
  int ret;
  VENC_RECV_PIC_PARAM_S stRecvParam;

  memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
  stRecvParam.s32RecvPicNum = 1;
  ret = RK_MPI_VENC_StartRecvFrame(u32VencChnTb, &stRecvParam);
  if (ret) {
    RKADK_LOGE("Request thumbnail failed %x", ret);
    return -1;
  }
  return 0;
}

static RKADK_S32 VdecThmFree(void *opaque) {
  RKADK_LOGD("vdec free: %p", opaque);
  if (opaque) {
    free(opaque);
    opaque = NULL;
  }
  return 0;
}

static RKADK_S32 ThumbDataCopy(RKADK_THUMB_ATTR_S *pstDstThmAttr,
                                        RK_U8 *pVdecData, RK_U64 VdecDataLen) {
  if (!pVdecData) {
    RKADK_LOGE("pVdecData is null");
    return -1;
  }

  if (!pstDstThmAttr->pu8Buf) {
    pstDstThmAttr->pu8Buf = (RKADK_U8 *)malloc(VdecDataLen);
    if (!pstDstThmAttr->pu8Buf) {
      RKADK_LOGE("malloc thumb buffer failed, VdecDataLen: %lld", VdecDataLen);
      return -1;
    }
    RKADK_LOGD("malloc thumb buffer[%p, %lld]", pstDstThmAttr->pu8Buf, VdecDataLen);

    pstDstThmAttr->u32BufSize = (RKADK_U32)VdecDataLen;
  } else {
    if (pstDstThmAttr->u32BufSize < VdecDataLen)
      RKADK_LOGW("buffer size[%d] < thumb size[%lld]",
                 pstDstThmAttr->u32BufSize, VdecDataLen);
    else
      pstDstThmAttr->u32BufSize = VdecDataLen;
  }

  memcpy(pstDstThmAttr->pu8Buf, pVdecData, pstDstThmAttr->u32BufSize);
  return 0;
}

static RKADK_S32 ThumbVpssInit(MPP_CHN_S stVpssChn, RKADK_THUMB_ATTR_S *pstSrcThmAttr,
                                       RKADK_THUMB_ATTR_S *pstDstThmAttr) {
  RKADK_U32 u32MaxW, u32MaxH;
  VPSS_GRP_ATTR_S stGrpAttr;
  VPSS_CHN_ATTR_S stChnAttr;

  memset(&stGrpAttr, 0, sizeof(VPSS_GRP_ATTR_S));
  memset(&stChnAttr, 0, sizeof(VPSS_CHN_ATTR_S));

  u32MaxW = pstSrcThmAttr->u32Width > pstDstThmAttr->u32Width ?
    pstSrcThmAttr->u32Width : pstDstThmAttr->u32Width;
  u32MaxH = pstSrcThmAttr->u32Height > pstDstThmAttr->u32Height ?
    pstSrcThmAttr->u32Height : pstDstThmAttr->u32Height;

  stGrpAttr.u32MaxW = u32MaxW;
  stGrpAttr.u32MaxH = u32MaxH;
  stGrpAttr.enPixelFormat = RK_FMT_YUV420SP;
  stGrpAttr.enCompressMode = COMPRESS_MODE_NONE;
  stGrpAttr.stFrameRate.s32SrcFrameRate = -1;
  stGrpAttr.stFrameRate.s32DstFrameRate = -1;
  stChnAttr.enChnMode = VPSS_CHN_MODE_USER;
  stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
  stChnAttr.enDynamicRange = DYNAMIC_RANGE_SDR8;
  stChnAttr.enPixelFormat = ThumbToRKPixFmt(pstDstThmAttr->enType);
  stChnAttr.stFrameRate.s32SrcFrameRate = -1;
  stChnAttr.stFrameRate.s32DstFrameRate = -1;
  stChnAttr.u32Width = pstDstThmAttr->u32Width;
  stChnAttr.u32Height = pstDstThmAttr->u32Height;
  stChnAttr.u32Depth = 1;

  return RKADK_MPI_VPSS_Init(stVpssChn.s32DevId, stVpssChn.s32ChnId,
                            &stGrpAttr, &stChnAttr);
}

RKADK_S32 ThumbnailJpgDecode(RKADK_THUMB_ATTR_S *pstSrcThmAttr,
                                   RKADK_THUMB_ATTR_S *pstDstThmAttr, bool *bFree) {
  int ret = 0, deinitRet = 0;
  VDEC_CHN_ATTR_S stAttr;
  VDEC_CHN_PARAM_S stVdecParam;
  MB_BLK jpgMbBlk = RK_NULL;
  MB_EXT_CONFIG_S stMbExtConfig;
  VDEC_STREAM_S stStream;
  VIDEO_FRAME_INFO_S sFrame = {0};
  RK_U8 *pVdecData = RK_NULL;
  RK_U64 VdecDataLen = 0;
  MPP_CHN_S stVdecChn, stVpssChn;
  bool bIsEnableVpss = false;

#ifdef RV1106_1103
  VIDEO_FRAME_INFO_S sFrameIn = {0};
#endif

  if (pstSrcThmAttr->pu8Buf[0] != 0xFF || pstSrcThmAttr->pu8Buf[1] != 0xD8) {
    RKADK_LOGD("Invalid jpeg data");
    *bFree = true;
    return -1;
  }

  memset(&stAttr, 0, sizeof(VDEC_CHN_ATTR_S));
  memset(&stVdecParam, 0, sizeof(VDEC_CHN_PARAM_S));
  memset(&stMbExtConfig, 0, sizeof(MB_EXT_CONFIG_S));
  memset(&stStream, 0, sizeof(VDEC_STREAM_S));

  stVdecChn.enModId = RK_ID_VDEC;
  stVdecChn.s32DevId = 0;
  stVdecChn.s32ChnId = pstDstThmAttr->s32VdecChn;

  stVpssChn.enModId = RK_ID_VPSS;
  stVpssChn.s32DevId = pstDstThmAttr->s32VpssGrp;
  stVpssChn.s32ChnId = pstDstThmAttr->s32VpssChn;

  stAttr.enMode = VIDEO_MODE_FRAME;
  stAttr.enType = RK_VIDEO_ID_JPEG;
  stAttr.u32PicWidth = pstSrcThmAttr->u32Width;
  stAttr.u32PicHeight = pstSrcThmAttr->u32Height;
  stAttr.u32FrameBufCnt = 1;
  stAttr.u32StreamBufCnt = 1;
#ifdef RV1106_1103
  stAttr.u32FrameBufDepth = 1;
#endif
  ret = RK_MPI_VDEC_CreateChn(stVdecChn.s32ChnId, &stAttr);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("create vdec[%d] failed[%x]", stVdecChn.s32ChnId, ret);
    *bFree = true;
    return ret;
  }

  stVdecParam.enType = RK_VIDEO_ID_JPEG;
  stVdecParam.stVdecPictureParam.enPixelFormat = RK_FMT_YUV420SP;
  ret = RK_MPI_VDEC_SetChnParam(stVdecChn.s32ChnId, &stVdecParam);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("set vdec chn[%d] param failed[%x]", stVdecChn.s32ChnId, ret);
    *bFree = true;
    goto exit;
  }

#ifndef RV1106_1103
  //create vpss
  ret = ThumbVpssInit(stVpssChn, pstSrcThmAttr, pstDstThmAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VPSS_Init vpss_grp[%d] vpss_chn[%d] falied[%x]",
                stVpssChn.s32DevId, stVpssChn.s32ChnId, ret);
    *bFree = true;
    goto exit;
  }
  bIsEnableVpss = true;

  //vdec bind vpss
  ret = RK_MPI_SYS_Bind(&stVdecChn, &stVpssChn);
  if (ret) {
    RKADK_LOGE("Bind VDEC[%d] to VPSS[%d, %d] failed[%x]", stVdecChn.s32ChnId,
               stVpssChn.s32DevId, stVpssChn.s32ChnId, ret);
    *bFree = true;
    goto exit;
  }
#endif

  //decode
  ret = RK_MPI_VDEC_StartRecvStream(stVdecChn.s32ChnId);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("start recv vdec[%d] failed[%x]", stVdecChn.s32ChnId, ret);
    *bFree = true;
    goto exit;
  }

  stMbExtConfig.pFreeCB = VdecThmFree;
  stMbExtConfig.pOpaque = pstSrcThmAttr->pu8Buf;
  stMbExtConfig.pu8VirAddr = pstSrcThmAttr->pu8Buf;
  stMbExtConfig.u64Size = pstSrcThmAttr->u32BufSize;
  ret = RK_MPI_SYS_CreateMB(&jpgMbBlk, &stMbExtConfig);
  if (ret) {
    RKADK_LOGE("Create vdec[%d] MB failed[%d]", stVdecChn.s32ChnId, ret);
    *bFree = true;
    goto exit;
  }

  stStream.u64PTS = 0;
  stStream.pMbBlk = jpgMbBlk;
  stStream.u32Len = pstSrcThmAttr->u32BufSize;
  stStream.bEndOfStream = RK_FALSE;
  stStream.bEndOfFrame = RK_FALSE;
  stStream.bBypassMbBlk = RK_TRUE;
  ret = RK_MPI_VDEC_SendStream(stVdecChn.s32ChnId, &stStream, -1);
  if(ret) {
    RKADK_LOGE("Send vdec[%d] stream failed[%d]", stVdecChn.s32ChnId, ret);
    goto exit;
  }

#ifdef RV1106_1103
  //get decode frame
  memset(&sFrameIn, 0, sizeof(VIDEO_FRAME_INFO_S));
  ret = RK_MPI_VDEC_GetFrame(stVdecChn.s32ChnId, &sFrameIn, -1);
  if(ret) {
    RKADK_LOGE("Get vdec[%d] frame failed[%d]", stVdecChn.s32ChnId, ret);
    goto exit;
  }

  if (sFrameIn.stVFrame.enPixelFormat == ThumbToRKPixFmt(pstDstThmAttr->enType)
        && sFrameIn.stVFrame.u32Width == pstDstThmAttr->u32Width
        && sFrameIn.stVFrame.u32Height == pstDstThmAttr->u32Height) {
    pVdecData = (RK_U8 *)RK_MPI_MB_Handle2VirAddr(sFrameIn.stVFrame.pMbBlk);
    VdecDataLen = RK_MPI_MB_GetSize(sFrameIn.stVFrame.pMbBlk);
    RKADK_LOGD("vdec output[%p, %lld], w*h[%d, %d]", pVdecData, VdecDataLen,
                sFrameIn.stVFrame.u32Width, sFrameIn.stVFrame.u32Height);

    ret = ThumbDataCopy(pstDstThmAttr, pVdecData, VdecDataLen);
    if (ret)
      RKADK_LOGE("Data copy failed");

    goto exit;
  }

  //create vpss
  ret = ThumbVpssInit(stVpssChn, pstSrcThmAttr, pstDstThmAttr);
  if (ret) {
    RKADK_LOGE("RKADK_MPI_VPSS_Init vpss_grp[%d] vpss_chn[%d] falied[%x]",
                stVpssChn.s32DevId, stVpssChn.s32ChnId, ret);
    goto exit;
  }
  bIsEnableVpss = true;

  //send frame to vpss
  ret = RK_MPI_VPSS_SendFrame(stVpssChn.s32DevId, 0, &sFrameIn, -1);
  if (ret != RK_SUCCESS) {
    RKADK_LOGE("Send vpss[%d] frame failed[%d]", stVpssChn.s32DevId, ret);
    RK_MPI_VDEC_ReleaseFrame(stVdecChn.s32ChnId, &sFrameIn);
    goto exit;
  }

  RK_MPI_VDEC_ReleaseFrame(stVdecChn.s32ChnId, &sFrameIn);
#endif

  //get vpss frame
  memset(&sFrame, 0, sizeof(VIDEO_FRAME_INFO_S));
  ret = RK_MPI_VPSS_GetChnFrame(stVpssChn.s32DevId, stVpssChn.s32ChnId, &sFrame, -1);
  if(ret) {
    RKADK_LOGE("Get vpss[%d] frame failed[%d]", stVdecChn.s32ChnId, ret);
    goto exit;
  }

  pVdecData = (RK_U8 *)RK_MPI_MB_Handle2VirAddr(sFrame.stVFrame.pMbBlk);
  VdecDataLen = RK_MPI_MB_GetSize(sFrame.stVFrame.pMbBlk);
  RKADK_LOGD("vpss output[%p, %lld], w*h[%d, %d]", pVdecData, VdecDataLen,
              sFrame.stVFrame.u32Width, sFrame.stVFrame.u32Height);
  RK_MPI_SYS_MmzFlushCache(sFrame.stVFrame.pMbBlk, RK_TRUE);

  ret = ThumbDataCopy(pstDstThmAttr, pVdecData, VdecDataLen);
  if (ret)
    RKADK_LOGE("Data copy failed");

  RK_MPI_VPSS_ReleaseChnFrame(stVpssChn.s32DevId, stVpssChn.s32ChnId, &sFrame);

exit:
  if (bIsEnableVpss) {
#ifndef RV1106_1103
    deinitRet = RK_MPI_SYS_UnBind(&stVdecChn, &stVpssChn);
    if (deinitRet)
      RKADK_LOGE("UnBind VDEC[%d] to VPSS[%d, %d] failed[%x]", stVdecChn.s32ChnId,
                 stVpssChn.s32DevId, stVpssChn.s32ChnId, ret);
#endif

    deinitRet = RKADK_MPI_VPSS_DeInit(stVpssChn.s32DevId, stVpssChn.s32ChnId);
    if (deinitRet)
      RKADK_LOGE("RKADK_MPI_VPSS_DeInit[%d, %d] failed[%d]", stVpssChn.s32DevId, stVpssChn.s32ChnId, deinitRet);
  }

  RK_MPI_VDEC_StopRecvStream(stVdecChn.s32ChnId);
  deinitRet = RK_MPI_VDEC_DestroyChn(stVdecChn.s32ChnId);
  if (deinitRet)
    RKADK_LOGE("RK_MPI_VDEC_DestroyChn[%d] failed[%d]", stVdecChn.s32ChnId, deinitRet);

  if(jpgMbBlk)
    RK_MPI_MB_ReleaseMB(jpgMbBlk);

  return ret;
}

static RKADK_S32 BuildInThmToMp4(FILE *fd, RKADK_S64 s64FileSize, RKADK_CHAR *pszFileName,
                            RKADK_THUMB_ATTR_S *pstThumbAttr) {
  RKADK_S64 cur = 0, s64BoxSize = 0;
  bool bBuildInThm = false;
  RKADK_S32 s32BoxSize;
  RKADK_U32 u32Width, u32Height, u32VirWidth, u32VirHeight;
  char boxHeader[THM_BOX_HEADER_LEN];
  char largeSize[THM_BOX_HEADER_LEN];

  if (fseek(fd, 0, SEEK_SET)) {
    RKADK_LOGE("seek to %s header failed", pszFileName);
    return -1;
  }

  while (!feof(fd)) {
    if (fread(boxHeader, THM_BOX_HEADER_LEN, 1, fd) != 1) {
      if (feof(fd)) {
        RKADK_LOGD("EOF");
        bBuildInThm = true;
      } else {
        RKADK_LOGE("Can't read box header");
      }
      break;
    }

    s32BoxSize = boxHeader[0] << 24 | boxHeader[1] << 16 | boxHeader[2] << 8 |
                 boxHeader[3];

    if (!s32BoxSize) {
      if (!boxHeader[4] && !boxHeader[5] && !boxHeader[6] && !boxHeader[7]) {
        RKADK_LOGE("invalid data, cover!!!");

        if (fseek(fd, -THM_BOX_HEADER_LEN, SEEK_CUR))
          RKADK_LOGE("seek failed");
        else
          bBuildInThm = true;
      } else {
        RKADK_LOGE("u32BoxSize = %d, invalid data", s32BoxSize);
      }

      break;
    } else if (s32BoxSize == 1) {
      if (fread(largeSize, THM_BOX_HEADER_LEN, 1, fd) != 1) {
        RKADK_LOGE("read largeSize failed");
        break;
      }

      s64BoxSize = (RKADK_S64)largeSize[0] << 56 |
                   (RKADK_S64)largeSize[1] << 48 |
                   (RKADK_S64)largeSize[2] << 40 |
                   (RKADK_S64)largeSize[3] << 32 | largeSize[4] << 24 |
                   largeSize[5] << 16 | largeSize[6] << 8 | largeSize[7];

      cur = ftell(fd);

      if (s64BoxSize - THM_BOX_HEADER_LEN * 2 + cur < 0 ||
          s64BoxSize - THM_BOX_HEADER_LEN * 2 + cur > s64FileSize) {
        RKADK_LOGE("bad largeSize value, this box is invalid. Insert thumb here!");
        fseek(fd, 0 - THM_BOX_HEADER_LEN * 2, SEEK_CUR);
        bBuildInThm = true;
        break;
      } else {
        fseek(fd, s64BoxSize - (THM_BOX_HEADER_LEN * 2), SEEK_CUR);
      }
    } else if (boxHeader[4] == 't' && boxHeader[5] == 'h'
          && boxHeader[6] == 'm' && boxHeader[7] != RKADK_THUMB_TYPE_JPEG) {
      if (fseek(fd, -THM_BOX_HEADER_LEN, SEEK_CUR)) {
        RKADK_LOGE("seek failed");
      } else {
        RKADK_LOGD("already exist thm[%d] tag, cover", boxHeader[7]);
        bBuildInThm = true;
      }

      break;
    } else {
      cur = ftell(fd);

      if (s32BoxSize - THM_BOX_HEADER_LEN + cur < 0 ||
          s32BoxSize - THM_BOX_HEADER_LEN + cur > s64FileSize) {
        RKADK_LOGE("bad BoxSize value, this box is invalid. Insert thumb here!");
        fseek(fd, 0 - THM_BOX_HEADER_LEN, SEEK_CUR);
        bBuildInThm = true;
        break;
      } else {
          fseek(fd, s32BoxSize - THM_BOX_HEADER_LEN, SEEK_CUR);
      }
    }
  }

  if (!bBuildInThm)
    return -1;

  // 16: 4bytes width + 4bytes height + 4bytes VirWidth + 4bytes VirHeight
  s32BoxSize = pstThumbAttr->u32BufSize + THM_BOX_HEADER_LEN + 16;
  boxHeader[0] = s32BoxSize >> 24;
  boxHeader[1] = (s32BoxSize & 0x00FF0000) >> 16;
  boxHeader[2] = (s32BoxSize & 0x0000FF00) >> 8;
  boxHeader[3] = s32BoxSize & 0x000000FF;
  boxHeader[4] = 't';
  boxHeader[5] = 'h';
  boxHeader[6] = 'm';
  boxHeader[7] = pstThumbAttr->enType;

  u32Width = bswap_32(pstThumbAttr->u32Width);
  u32Height = bswap_32(pstThumbAttr->u32Height);
  u32VirWidth = bswap_32(pstThumbAttr->u32VirWidth); 
  u32VirHeight = bswap_32(pstThumbAttr->u32VirHeight);

  if (fwrite(boxHeader, THM_BOX_HEADER_LEN, 1, fd) != 1) {
    RKADK_LOGE("write thm box header failed");
    return -1;
  }

  if (fwrite(&(u32Width), 4, 1, fd) != 1) {
    RKADK_LOGE("write thm width failed");
    return -1;
  }

  if (fwrite(&(u32Height), 4, 1, fd) != 1) {
    RKADK_LOGE("write thm height failed");
    return -1;
  }

  if (fwrite(&(u32VirWidth), 4, 1, fd) != 1) {
    RKADK_LOGE("write thm virtual width failed");
    return -1;
  }

  if (fwrite(&(u32VirHeight), 4, 1, fd) != 1) {
    RKADK_LOGE("write thm virtual height failed");
    return -1;
  }

  if (fwrite(pstThumbAttr->pu8Buf, pstThumbAttr->u32BufSize, 1, fd) != 1) {
    RKADK_LOGE("write thm box body failed");
    return -1;
  }

  RKADK_LOGD("done!");
  return 0;
}

static RKADK_S64 SeekToThmInMp4(const RKADK_U8 *pFile, RKADK_S64 s64FileSize,
                                      RKADK_THUMB_TYPE_E enType,
                                      RKADK_U64 *u64JpgThmPos) {
  RKADK_S64 s64BoxSize = 0, cur = 0;

  /* at least 16 bytes remains if thumbnail exist */
  while (cur + 2 * THM_BOX_HEADER_LEN < s64FileSize) {
    if (pFile[cur + 4] == 't' && pFile[cur + 5] == 'h' && pFile[cur + 6] == 'm') {
      if (pFile[cur + 7] == enType)
        return cur;
      else if (pFile[cur + 7] == RKADK_THUMB_TYPE_JPEG)
        *u64JpgThmPos = cur;
    }

    s64BoxSize = bswap_32(*(RKADK_U32*) (pFile + cur));
    if (s64BoxSize == 1)
      s64BoxSize = bswap_64(*(RKADK_U64*) (pFile + cur + THM_BOX_HEADER_LEN));
    else if (s64BoxSize <= 0) {
      RKADK_LOGE("Last one box, not find thm box");
      break;
    }

    cur += s64BoxSize;
    if (cur < 0) {
      RKADK_LOGE("cur = %lld invalid value, u64BoxSize = %lld", cur, s64BoxSize);
      break;
    }
  }

  return -1;
}

static RKADK_S32 GetSpecificThmInMp4(RKADK_U8 *pFile, RKADK_S64 s64FileSize,
                                RKADK_THUMB_ATTR_S *pstThumbAttr, RKADK_U64 *u64JpgThmPos) {
  int boxSize = 0;
  RKADK_S64 cur = 0;

  if (*u64JpgThmPos > 0)
    cur = *u64JpgThmPos;
  else
    cur = SeekToThmInMp4(pFile, s64FileSize, pstThumbAttr->enType, u64JpgThmPos);

  if (cur > 0 && (boxSize = bswap_32(*(int*) (pFile + cur))) > 0 &&
      cur + boxSize <= s64FileSize) {
    // 16: 4bytes width + 4bytes height + 4bytes VirWidth + 4bytes VirHeight
    RKADK_U32 u32DataSize = boxSize - THM_BOX_HEADER_LEN - 16;

    if (!pstThumbAttr->pu8Buf) {
      pstThumbAttr->pu8Buf = (RKADK_U8 *)malloc(u32DataSize);
      if (!pstThumbAttr->pu8Buf) {
        RKADK_LOGE("malloc thumbnail buffer failed, size: %d", u32DataSize);
        return -1;
      }
      pstThumbAttr->u32BufSize = u32DataSize;
      RKADK_LOGD("malloc jpg thumb buffer[%p, %d]", pstThumbAttr->pu8Buf, u32DataSize);
    } else {
      if (pstThumbAttr->u32BufSize < u32DataSize)
        RKADK_LOGW("buffer size[%d] < thm data size[%d]",
                   pstThumbAttr->u32BufSize, u32DataSize);
      else
        pstThumbAttr->u32BufSize = u32DataSize;
    }

    memcpy(pstThumbAttr->pu8Buf, pFile + cur + THM_BOX_HEADER_LEN + 16,
           pstThumbAttr->u32BufSize);
    pstThumbAttr->u32Width = bswap_32(*(RKADK_U32*) (pFile + cur + THM_BOX_HEADER_LEN));
    pstThumbAttr->u32Height = bswap_32(*(RKADK_U32*) (pFile + cur + THM_BOX_HEADER_LEN + 4));
    pstThumbAttr->u32VirWidth = bswap_32(*(RKADK_U32*) (pFile + cur + THM_BOX_HEADER_LEN + 8));
    pstThumbAttr->u32VirHeight = bswap_32(*(RKADK_U32*) (pFile + cur + THM_BOX_HEADER_LEN + 12));
    return 0;
  }

  return -1;
}

static RKADK_S32 GetThmInMp4(RKADK_U32 u32CamId, RKADK_CHAR *pszFileName,
                             RKADK_THUMB_ATTR_S *pstThumbAttr) {
  FILE *fd = NULL;
  int ret = 0, result = 0;
  RKADK_THUMB_ATTR_S stTmpThmAttr;
  bool bFree = false;
  RKADK_U64 u64JpgThmPos = 0;
  RKADK_S64 s64FileSize = 0;
  RKADK_U8 *pFile = NULL;
  struct stat stStatBuf;
  struct utimbuf stTimebuf;

  RKADK_PARAM_THUMB_CFG_S *ptsThumbCfg = RKADK_PARAM_GetThumbCfg(u32CamId);
  if (!ptsThumbCfg) {
    RKADK_LOGE("RKADK_PARAM_GetThumbCfg failed");
    return -1;
  }

  if (!pstThumbAttr->u32Width || !pstThumbAttr->u32Height) {
    pstThumbAttr->u32Width = UPALIGNTO(ptsThumbCfg->thumb_width, 4);
    pstThumbAttr->u32Height = UPALIGNTO(ptsThumbCfg->thumb_height, 2);
  }

  if (!pstThumbAttr->u32VirWidth || !pstThumbAttr->u32VirHeight) {
    pstThumbAttr->u32VirWidth = pstThumbAttr->u32Width;
    pstThumbAttr->u32VirHeight = pstThumbAttr->u32Height;
  }

  fd = fopen(pszFileName, "r+");
  if (!fd) {
    RKADK_LOGE("open %s failed", pszFileName);
    return -1;
  }

  if (fseek(fd, 0, SEEK_END) || (s64FileSize = ftell(fd)) == -1) {
    RKADK_LOGE("get file[%s] size failed", pszFileName);
    fclose(fd);
    return -1;
  }

  pFile = (RKADK_U8 *)mmap(NULL, s64FileSize, PROT_READ, MAP_SHARED, fileno(fd), 0);
  if (pFile == MAP_FAILED) {
    RKADK_LOGE("mmap %s failed, errno: %d", pszFileName, errno);
    fclose(fd);
    return -1;
  }

  memset(&stTimebuf, 0, sizeof(struct utimbuf));
  result = stat(pszFileName, &stStatBuf);
  if (result) {
    RKADK_LOGW("stat[%s] failed[%d]", pszFileName, result);
  } else {
    stTimebuf.actime = stStatBuf.st_atime;
    stTimebuf.modtime = stStatBuf.st_mtime;
  }

  //get specified type thumb
  ret = GetSpecificThmInMp4(pFile, s64FileSize, pstThumbAttr, &u64JpgThmPos);
  if (!ret)
    goto exit;

  //get jpg thumb, then decode
  memset(&stTmpThmAttr, 0, sizeof(RKADK_THUMB_ATTR_S));
  stTmpThmAttr.enType = RKADK_THUMB_TYPE_JPEG;
  ret = GetSpecificThmInMp4(pFile, s64FileSize, &stTmpThmAttr, &u64JpgThmPos);
  if (ret) {
    RKADK_LOGE("Get jpg thumbnail in %s failed!", pszFileName);
    goto exit;
  }

  ret = ThumbnailJpgDecode(&stTmpThmAttr, pstThumbAttr, &bFree);
  if (!ret) {
    if (BuildInThmToMp4(fd, s64FileSize, pszFileName, pstThumbAttr))
      RKADK_LOGE("BuildInThm to %s failed", pszFileName);
  }

  if (bFree)
    RKADK_ThmBufFree(&stTmpThmAttr);

exit:
  if (fd)
    fclose(fd);

  munmap(pFile, s64FileSize);

  if (ret)
    RKADK_ThmBufFree(pstThumbAttr);

  if (stTimebuf.actime != 0 && stTimebuf.modtime != 0) {
    result = utime(pszFileName, &stTimebuf);
    if (result)
      RKADK_LOGW("utime[%s] failed[%d]", pszFileName, result);
  }

  return ret;
}

RKADK_S32 RKADK_GetThmInMp4(RKADK_U32 u32CamId, RKADK_CHAR *pszFileName,
                            RKADK_U8 *pu8Buf, RKADK_U32 *pu32Size) {
  int ret;
  RKADK_THUMB_ATTR_S stThumbAttr;

  RKADK_CHECK_POINTER(pszFileName, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pu8Buf, RKADK_FAILURE);

  memset(&stThumbAttr, 0, sizeof(RKADK_THUMB_ATTR_S));

  /* thm size is unchangable */
  stThumbAttr.u32Width = 0;
  stThumbAttr.u32Height = 0;
  stThumbAttr.u32VirWidth = 0;
  stThumbAttr.u32VirHeight = 0;
  stThumbAttr.enType = RKADK_THUMB_TYPE_JPEG;
  stThumbAttr.pu8Buf = pu8Buf;
  stThumbAttr.u32BufSize = *pu32Size;
  stThumbAttr.s32VdecChn = THM_VDEC_CHN;
  stThumbAttr.s32VpssGrp = THM_VPSS_GRP;
  stThumbAttr.s32VpssChn = THM_VPSS_CHN;

  ret = GetThmInMp4(u32CamId, pszFileName, &stThumbAttr);
  if (ret) {
    RKADK_LOGE("Get thumbnail in %s failed!", pszFileName);
    return ret;
  }

  *pu32Size = stThumbAttr.u32BufSize;
  return ret;
}

RKADK_S32 RKADK_ThmBufFree(RKADK_THUMB_ATTR_S *pstThumbAttr) {
  return RKADK_MEDIA_FrameFree((RKADK_FRAME_ATTR_S *)pstThumbAttr);
}

RKADK_S32 RKADK_GetThmInMp4Ex(RKADK_U32 u32CamId, RKADK_CHAR *pszFileName,
                              RKADK_THUMB_ATTR_S *pstThumbAttr) {
  int ret;

  RKADK_CHECK_POINTER(pszFileName, RKADK_FAILURE);
  RKADK_CHECK_POINTER(pstThumbAttr, RKADK_FAILURE);

  if (pstThumbAttr->s32VdecChn < 0)
    pstThumbAttr->s32VdecChn = THM_VDEC_CHN;

  if (pstThumbAttr->s32VpssGrp < 0)
    pstThumbAttr->s32VpssGrp = THM_VPSS_GRP;

  if (pstThumbAttr->s32VpssChn < 0)
    pstThumbAttr->s32VpssChn = THM_VPSS_CHN;

  ret = GetThmInMp4(u32CamId, pszFileName, pstThumbAttr);
  if (ret)
    RKADK_ThmBufFree(pstThumbAttr);

  return ret;
}

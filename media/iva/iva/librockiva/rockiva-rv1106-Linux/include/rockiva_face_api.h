/****************************************************************************
 *
 *    Copyright (c) 2022 by Rockchip Corp.  All rights reserved.
 *
 *    The material in this file is confidential and contains trade secrets
 *    of Rockchip Corporation. This is proprietary information owned by
 *    Rockchip Corporation. No part of this work may be disclosed,
 *    reproduced, copied, transmitted, or used in any way for any purpose,
 *    without the express written permission of Rockchip Corporation.
 *
 *****************************************************************************/

#ifndef __ROCKIVA_FACE_API_H__
#define __ROCKIVA_FACE_API_H__

#include "rockiva_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------ */

#define ROCKIVA_FACE_MAX_FACE_NUM (40)   /* 最大人脸数目 */
#define ROCKIVA_AREA_POINT_NUM_MAX (6)   /* 最大区域点数 */
#define ROCKIVA_FACE_FORBIDDEN_AREA (16) /* 最大支持的屏蔽检测区域 */
#define ROCKIVA_FACE_FEATURE_SIZE_MAX (4096) /* 特征值空间大小 */
#define ROCKIVA_FACE_MODEL_VERSION (64) /* 模型版本号字符串长度 */
#define ROCKIVA_FACE_INFO_SIZE_MAX (32) /* 人脸入库信息字符串长度(用户填入) */
#define ROCKIVA_FACE_ID_MAX_NUM (100) /* 特征比对结果的TOP数目（最大）*/

/* ---------------------------类型定义----------------------------------- */

/* 人脸优选类型 */
typedef enum {
  ROCKIVA_FACE_OPT_BEST, /* 效果优先模式 目标从出现到消失的最优人脸 */
  ROCKIVA_FACE_OPT_FAST, /* 快速优先模式 目标从出现的一个时间段内的最优人脸 */
} RockIvaFaceOptType;

/* 人脸业务类型 */
typedef enum {
  ROCKIVA_FACE_MODE_NORMAL = 0, /* 正常模式(根据RockIvaWorkMode配置) */
  ROCKIVA_FACE_MODE_IMPORT = 1, /* 导库模式(底图特征提取) */
  ROCKIVA_FACE_MODE_SEARCH = 2, /* 以图搜图模式 */
  ROCKIVA_FACE_MODE_PANEL =
      3, /* 人脸面板机模式（近距离人脸识别分析，支持多摄） */
} RockIvaFaceWorkMode;

/* 人脸状态 */
typedef enum {
  ROCKIVA_FACE_STATE_NONE,      /* 人脸状态未知 */
  ROCKIVA_FACE_STATE_FIRST,     /* 人脸第一次出现 */
  ROCKIVA_FACE_STATE_TRACKING,  /* 人脸检测跟踪过程中 */
  ROCKIVA_FACE_STATE_CAPTURING, /* 人脸进入抓拍处理 */
  ROCKIVA_FACE_STATE_ANALYZING, /* 人脸触发抓拍上报，进行人脸分析中 */
  ROCKIVA_FACE_STATE_CAPTURED, /* 人脸抓拍处理完成
                                  （完成抓拍回调函数结束后进入该状态，或没有满足质量且不需上报则直接进入该状态）
                                */
  ROCKIVA_FACE_STATE_LAST      /* 目标最后一次出现 */
} RockIvaFaceState;

/* 性别 */
typedef enum {
  ROCKIVA_GENDER_TYPE_UNKNOWN = 0, /* 未知 */
  ROCKIVA_GENDER_TYPE_MALE = 1,    /* 男性 */
  ROCKIVA_GENDER_TYPE_FEMALE = 2   /* 女性 */
} RockIvaFaceGenderType;

/* 年龄 */
typedef enum {
  ROCKIVA_AGE_TYPE_UNKNOWN = 0,    /* 未知 */
  ROCKIVA_AGE_TYPE_CHILD = 1,      /* 儿童 */
  ROCKIVA_AGE_TYPE_EARLYYOUTH = 2, /* 少年 */
  ROCKIVA_AGE_TYPE_YOUTH = 3,      /* 青年 */
  ROCKIVA_AGE_TYPE_MIDLIFE = 4,    /* 中年 */
  ROCKIVA_AGE_TYPE_OLD = 5         /* 老年 */
} RockIvaFaceAgeType;

/* 情感 */
typedef enum {
  ROCKIVA_EMOTION_TYPE_UNKNOWN = 0,   /* 未知 */
  ROCKIVA_EMOTION_TYPE_ANGRY = 1,     /* 生气的 */
  ROCKIVA_EMOTION_TYPE_CALM = 2,      /* 平静的 */
  ROCKIVA_EMOTION_TYPE_CONFUSED = 3,  /* 迷茫的 */
  ROCKIVA_EMOTION_TYPE_DISGUST = 4,   /* 厌恶的 */
  ROCKIVA_EMOTION_TYPE_HAPPY = 5,     /* 高兴的 */
  ROCKIVA_EMOTION_TYPE_SAD = 6,       /* 悲伤的 */
  ROCKIVA_EMOTION_TYPE_SCARED = 7,    /* 害怕的 */
  ROCKIVA_EMOTION_TYPE_SURPRISED = 8, /* 吃惊的 */
  ROCKIVA_EMOTION_TYPE_SQUINT = 9,    /* 眯眼的 */
  ROCKIVA_EMOTION_TYPE_SCREAM = 10,   /* 尖叫的 */
  ROCKIVA_EMOTION_TYPE_OTHER = 11     /* 其他 */
} RockIvaFaceEmotionType;

/* 眼镜 */
typedef enum {
  ROCKIVA_GLASSES_TYPE_UNKNOWN = 0,   /* 未知 */
  ROCKIVA_GLASSES_TYPE_NOGLASSES = 1, /* 不戴眼镜 */
  ROCKIVA_GLASSES_TYPE_GLASSES = 2,   /* 戴眼镜 */
  ROCKIVA_GLASSES_TYPE_SUNGLASSES = 3 /* 太阳眼镜 （预留）*/
} RockIvaFaceGlassesType;

/* 是否微笑 */
typedef enum {
  ROCKIVA_SMILE_TYPE_UNKNOWN = 0, /* 未知 */
  ROCKIVA_SMILE_TYPE_YES = 1,     /* 微笑 */
  ROCKIVA_SMILE_TYPE_NO = 2       /* 不微笑 */
} RockIvaFaceSmileType;

/* 是否戴口罩 */
typedef enum {
  ROCKIVA_MASK_TYPE_UNKNOWN = 0, /* 未知 */
  ROCKIVA_MASK_TYPE_YES = 1,     /* 戴口罩 */
  ROCKIVA_MASK_TYPE_NO = 2       /* 不戴口罩 */
} RockIvaFaceMaskType;

/* 是否有胡子 */
typedef enum {
  ROCKIVA_BEARD_TYPE_UNKNOWN = 0, /* 未知 */
  ROCKIVA_BEARD_TYPE_YES = 1,     /* 有胡子 */
  ROCKIVA_BEARD_TYPE_NO = 2       /* 没有胡子 */
} RockIvaFaceBeardType;

/* 抓拍类型 */
typedef enum {
  ROCKIVA_FACE_CAP_TYPE_UNKNOWN, /* 未知 */
  ROCKIVA_FACE_CAP_TYPE_IMPORT,  /* 人脸导库模式结果帧 */
  ROCKIVA_FACE_CAP_TYPE_PICTURE, /* 单图运行模式结果帧 */
  ROCKIVA_FACE_CAP_TYPE_DISPEAR, /* 目标消失抓拍帧 */
  ROCKIVA_FACE_CAP_TYPE_TIMEOUT, /* 超时抓拍帧 */
  ROCKIVA_FACE_CAP_TYPE_QUALITY, /* 满足阈值抓拍 */
  ROCKIVA_FACE_CAP_TYPE_FORENOTICE, /* 提前上报人脸分析结果（如口罩佩戴判断等）
                                     */
  ROCKIVA_FACE_CAP_TYPE_CUSTOM, /* 用户指定 */
} RockIvaFaceCapFrameType;

/* 人脸质量结果 */
typedef enum {
  ROCKIVA_FACE_QUALITY_OK,           /* 人脸质量合格 */
  ROCKIVA_FACE_QUALITY_NO_FACE,      /* 没有检测到人脸 */
  ROCKIVA_FACE_QUALITY_SCORE_FAIL,   /* 人脸质量分过低 */
  ROCKIVA_FACE_QUALITY_SIZE_FAIL,    /* 人脸过小 */
  ROCKIVA_FACE_QUALITY_CLARITY_FAIL, /* 人脸模糊 */
  ROCKIVA_FACE_QUALITY_ANGLE_FAIL,   /* 人脸角度过大 */
  ROCKIVA_FACE_QUALITY_MASK_FAIL     /* 人脸遮挡 */
} RockIvaFaceQualityResultCode;

/* ---------------------------规则配置----------------------------------- */

/* 算法业务类型 */
typedef struct {
  uint8_t faceCaptureEnable;   /* 人脸抓拍业务 1：有效 */
  uint8_t faceRecognizeEnable; /* 人脸识别业务 1：有效 */
  uint8_t faceAttributeEnable; /* 人脸属性分析业务 1:有效 */
  uint8_t relatedPersonEnable; /* 是否关联人体 1：有效 */
} RockIvaFaceTaskType;

/* 人脸SDK处理能力 */
typedef struct {
  uint32_t maxDetectNum; /* 最大的检测个数（未使用）*/
  uint32_t
      maxCaptureNum; /* 最大的抓拍个数， 默认0为ROCKIVA_FACE_MAX_FACE_NUM */
  uint32_t maxRecogNum; /* 最大的识别个数（未使用）*/
} RockIvaFaceCapacity;

/* 人脸质量过滤配置 */
typedef struct {
  uint16_t minScore; /* 最小质量分(默认0不过滤) */
  uint16_t minSize; /* 最小人脸大小（万分比[0-10000]，默认0为30像素） */
  uint16_t minClarity;   /* 最小清晰度（默认0不过滤） */
  RockIvaAngle maxAngle; /* 最大人脸角度（默认0不过滤） */
  uint16_t
      minEyescore; /* 眼睛遮挡阈值，低于该阈值将被当作遮挡过滤，值范围[0,100] */
  uint16_t
      minNoseScore; /* 鼻子遮挡阈值，低于该阈值将被当作遮挡过滤，值范围[0,100]
                     */
  uint16_t
      minMouthScore; /* 嘴巴遮挡阈值，低于该阈值将被当作遮挡过滤，值范围[0,100]
                      */
} RockIvaFaceQualityConfig;

/* 抓拍上报图像配置 */
typedef struct {
  uint8_t mode; /* 上报抓拍图像模式，[0：无； 1：小图； 2：大图； 3：大小图] */
  uint8_t resizeMode; /* 抓拍图像缩放方式 [0:
                         保持目标大小，除非目标大小超过imageInfo设定大小； 1:
                         固定缩放到imageInfo设定大小] */
  uint8_t alignWidth; /* 抓拍图像的对齐宽度 */
  RockIvaRectExpandRatio expand; /* 抓拍图像扩展人脸框上下左右的比例大小配置 */
  RockIvaImageInfo
      imageInfo; /* 抓拍图像信息设置，宽高需要设置4对齐。人脸小图开辟的内存空间与设置的宽、高、图像格式和最大人脸抓拍个数有关
                  */
} RockIvaCaptureImageConfig;

/* 人脸抓拍规则设置 */
typedef struct {
  uint8_t sensitivity;    /* 检测灵敏度[1,100] */
  uint8_t detectAreaEn;   /* 是否设置检测区域[0关 1开] */
  RockIvaArea detectArea; /* 检测区域 */
  uint8_t
      qualityFilterMode;                  /* 人脸质量过滤模式
                                             [0不满足不上报，1不满足也上报但不进行人脸分析] */
  RockIvaFaceQualityConfig qualityConfig; /* 人脸质量过滤配置 */
  RockIvaFaceCapacity
      faceCapacity; /* 人脸最大检测、抓拍和识别个数配置，目前只实现设置最大抓拍个数，0[不限制]
                     */
  RockIvaCaptureImageConfig captureImageConfig; /* 抓拍上报人脸图像配置 */
  RockIvaFaceOptType optType;                   /* 人脸优选类型 */
  uint32_t
      optBestNum; /* ROCKIVA_FACE_OPT_BEST：人脸优选张数（未实现） 范围：1-3 */
  uint32_t
      optBestOverTime; /* 人脸质量最优抓拍超时时间设置ms，若人脸在此时段内未消失则上报此时段内的质量最优人脸；若为0，则在消失后才上报质量最优人脸
                        */
  uint32_t faceQualityThrehold; /* 快速抓拍时满足抓拍的人脸质量分阈值 */
  uint8_t
      captureWithMask; /* 支持抓拍戴口罩的人脸并上报人脸是否佩戴口罩[0：关；1：开；2：开且口罩提前上报(满足qualityConfig配置)]，若打开则qualityConfig的minMouthScore和minNoseScore过滤失效
                        */
  uint8_t
      faceIouThreshold; /* 多摄模式下RGB和IR图像中人脸需要满足的IOU阈值，百分比范围[0,
                           100]，默认0为50% */
} RockIvaFaceRule;

/* 人脸分析业务初始化参数配置 */
typedef struct {
  RockIvaFaceWorkMode mode;        /* 人脸任务模式 */
  RockIvaFaceRule faceCaptureRule; /* 人脸抓拍规则 */
  RockIvaFaceTaskType
      faceTaskType; /* 人脸业务类型：人脸抓拍业务/人脸识别业务 */
} RockIvaFaceTaskParams;

/* ------------------------------------------------------------------ */

/* -------------------------- 算法处理结果 --------------------------- */

/* 人脸属性结构体 */
typedef struct {
  RockIvaFaceGenderType gender;    /* 性别 */
  RockIvaFaceAgeType age;          /* 年龄 */
  RockIvaFaceEmotionType emotion;  /* 情感 */
  RockIvaFaceGlassesType eyeGlass; /* 眼镜 */
  RockIvaFaceSmileType smile;      /* 笑容 */
  RockIvaFaceMaskType mask;        /* 口罩 */
  RockIvaFaceBeardType beard;      /* 胡子 */
  uint32_t attractive;             /* 颜值 */
} RockIvaFaceAttribute;

/* 人脸质量信息 */
typedef struct {
  uint16_t score;     /* 人脸质量分数(值范围0~100) */
  uint16_t clarity;   /* 人脸清晰度(值范围0~100, 100表示最清晰) */
  RockIvaAngle angle; /* 人脸角度 */
  uint16_t eyesScore; /* 眼睛遮挡分数（值范围[0,100]，值越低表示遮挡越严重） */
  uint16_t noseScore; /* 鼻子遮挡分数（值范围[0,100]，值越低表示遮挡越严重） */
  uint16_t mouthScore; /* 嘴巴遮挡分数（值范围[0,100]，值越低表示遮挡越严重） */
  uint16_t faceScore; /* 人脸分数（值范围[0,100]）*/
} RockIvaFaceQualityInfo;

/* 单个目标人脸检测基本信息 */
typedef struct {
  uint32_t objId;                     /* 目标ID[0,2^32) */
  uint32_t frameId;                   /* 人脸所在帧序号 */
  RockIvaRectangle faceRect;          /* 人脸区域原始位置 */
  RockIvaFaceQualityInfo faceQuality; /* 人脸质量信息 */
  RockIvaFaceState faceState;         /* 人脸状态 */
  RockIvaObjectInfo person;           /* 关联的人体检测信息 */
  RockIvaObjectInfo faceOnIR;         /* IR图像中关联的人脸 */
} RockIvaFaceInfo;

/* 单个目标人脸分析信息 */
typedef struct {
  uint32_t featureSize; /* 人脸特征长度,单位字节 */
  char feature[ROCKIVA_FACE_FEATURE_SIZE_MAX]; /* 人脸特征数据 */
  RockIvaFaceAttribute faceAttr;               /* 人脸属性 */
} RockIvaFaceAnalyseInfo;

/* 人脸检测处理结果 */
typedef struct {
  uint32_t frameId;   /* 帧ID */
  uint32_t channelId; /* 通道号 */
  RockIvaImage frame; /* 对应的输入图像帧 */
  uint32_t objNum;    /* 人脸个数 */
  RockIvaFaceInfo faceInfo[ROCKIVA_FACE_MAX_FACE_NUM]; /* 各目标检测信息 */
  uint32_t recordNum;                                  /* 人脸记录数 */
  RockIvaFaceInfo faceRecord[ROCKIVA_FACE_MAX_FACE_NUM]; /* 人脸记录信息 */
} RockIvaFaceDetResult;

/* 人脸抓拍处理结果 */
typedef struct {
  uint32_t channelId;                       /* 通道号 */
  uint32_t frameId;                         /* 帧ID */
  RockIvaImage frame;                       /* 对应的输入图像帧,
                                               如果没有配置缓存大图此时图像缓存数据不能访问 */
  RockIvaFaceCapFrameType faceCapFrameType; /* 抓拍帧类型 */
  RockIvaFaceInfo faceInfo;                 /* 人脸基本检测信息 */
  RockIvaFaceQualityResultCode
      qualityResult; /* 人脸质量结果，如果结果不是OK则没有人脸分析信息 */
  RockIvaFaceAnalyseInfo faceAnalyseInfo; /* 人脸分析信息 */
  RockIvaImage captureImage;              /* 人脸抓拍小图 */
  RockIvaRectangle faceRectOnCaptureImage; /* 人脸抓拍小图上的人脸位置 */
} RockIvaFaceCapResult;

/* 入库特征对应的详细信息，用户输入 */
typedef struct {
  char faceIdInfo[ROCKIVA_FACE_INFO_SIZE_MAX];
} RockIvaFaceIdInfo;

/* 特征更新类型 */
typedef enum {
  ROCKIVA_FACE_FEATURE_INSERT = 0,    /* 添加特征 */
  ROCKIVA_FACE_FEATURE_DELETE = 1,    /* 删除特征 */
  ROCKIVA_FACE_FEATURE_UPDATE = 2,    /* 更新特征 */
  ROCKIVA_FACE_FEATURE_RETRIEVAL = 3, /* 查找标签 */
  ROCKIVA_FACE_FEATURE_CLEAR = 4,     /* 清除库信息 */
} RockIvaFaceLibraryAction;

/* 特征比对返回结果 */
typedef struct {
  char faceIdInfo[ROCKIVA_FACE_INFO_SIZE_MAX]; /* 人脸详细信息 */
  float score;                                 /* 比对分数 */
} RockIvaFaceSearchResult;

/* 特征比对返回结果列表 */
typedef struct {
  RockIvaFaceSearchResult faceIdScore[ROCKIVA_FACE_ID_MAX_NUM];
  int num;
} RockIvaFaceSearchResults;

/* ---------------------------------------------------------------- */

/**
 * @brief 检测结果回调函数
 *
 * result 结果
 * status 状态码
 * userdata 用户自定义数据
 */
typedef void (*ROCKIVA_FACE_DetResultCallback)(
    const RockIvaFaceDetResult *result, const RockIvaExecuteStatus status,
    void *userdata);

/**
 * @brief 抓拍和人脸分析结果回调函数
 *
 * result 结果
 * status 状态码
 * userdata 用户自定义数据
 */
typedef void (*ROCKIVA_FACE_AnalyseResultCallback)(
    const RockIvaFaceCapResult *result, const RockIvaExecuteStatus status,
    void *userdata);

typedef struct {
  ROCKIVA_FACE_DetResultCallback detCallback;
  ROCKIVA_FACE_AnalyseResultCallback analyseCallback;
} RockIvaFaceCallback;

/**
 * @brief 初始化
 *
 * @param handle [INOUT] 初始化完成的handle
 * @param initParams [IN] 初始化参数
 * @param resultCallback [IN] 回调函数
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_Init(RockIvaHandle handle,
                                 const RockIvaFaceTaskParams *initParams,
                                 const RockIvaFaceCallback callback);

/**
 * @brief
 * 运行时重新配置(重新配置会导致内部的一些记录清空复位，但是模型不会重新初始化)
 *
 * @param handle [IN] handle
 * @param initParams [IN] 配置参数
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_Reset(RockIvaHandle handle,
                                  const RockIvaFaceTaskParams *params);

/**
 * @brief 销毁
 *
 * @param handle [IN] handle
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_Release(RockIvaHandle handle);

/**
 * @brief
 * 指定需要抓拍分析识别的人脸，可以调用多次设定多个人脸（需要在检测结果回调中使用，仅faceCaptureEnable==0时候可用）
 *
 * @param handle [IN] handle
 * @param trackId [IN] 人脸跟踪id
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_SetAnalyseFace(RockIvaHandle handle, int trackId);

/**
 * @brief 1:1人脸特征比对接口
 *
 * @param feature1 [IN] 人脸特征1
 * @param feature2 [IN] 人脸特征2
 * @param score [OUT] 人脸1:1比对相似度(范围0-1.0)
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_FeatureCompare(const void *feature1,
                                           const void *feature2, float *score);

/**
 * @brief 人脸特征布控接口,用于对某个人脸库进行增、删、改
 *
 * @param libName [IN] 人脸库名称
 * @param action [IN] 人脸库操作类型
 * @param faceIdInfo [IN] 人脸ID
 * @param faceIdNum [IN] 人脸ID数量
 * @param featureData [IN] 人脸特征数据
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_FeatureLibraryControl(
    const char *libName, RockIvaFaceLibraryAction action,
    RockIvaFaceIdInfo *faceIdInfo, uint32_t faceIdNum, const void *featureData,
    int featureSize);

/**
 * @brief 人脸特征库检索接口,用于对某个人脸库的特征进行检索
 *
 * @param libName [IN] 人脸库名称
 * @param featureData [IN] 人脸特征
 * @param num [IN] 比对特征的个数
 * @param topK [IN] 前K个最相似的特征值
 * @param results [OUT] 比对结果
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_FACE_SearchFeature(const char *libName,
                                          const void *featureData,
                                          int featureSize, uint32_t num,
                                          int32_t topK,
                                          RockIvaFaceSearchResults *results);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of #ifndef __ROCKIVA_FACE_API_H__ */
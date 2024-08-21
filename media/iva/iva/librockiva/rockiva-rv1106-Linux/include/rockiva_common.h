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

#ifndef __ROCKIVA_COMMON_H__
#define __ROCKIVA_COMMON_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/********************************************************************/
/*                             宏定义                                */
/********************************************************************/

#define ROCKIVA_MAX_FRAMERATE      (60)   /* 最大帧率 */
#define ROCKIVA_AREA_POINT_NUM_MAX (32)   /* 最大区域点数 */
#define ROCKIVA_AREA_NUM_MAX       (16)   /* 最大区域数 */

#define ROCKIVA_PIXEL_RATION_CONVERT(base, x) ((x) * 10000 / (base))  /* 像素到万分比的转换 */
#define ROCKIVA_RATIO_PIXEL_CONVERT(base, x) (((x) * (base)) / 10000) /* 万分比到像素的转换 */

#define ROCKIVA_OBJECT_TYPE_BITMASK(index) (1<<index)
#define ROCKIVA_OBJECT_TYPE_FULL (0U-1)

#define ROCKIVA_MAX_OBJ_NUM          (128)                    /* 场景最大目标个数 */

#define ROCKIVA_PATH_LENGTH         (128)                    /* 文件路径字符串长度 */

#define ROCKIVA_PARAM_MAX_NUM (8)
#define ROCKIVA_PARAM_VALUE_MAX_SIZE (32)

/********************************************************************/
/*                          枚举定义                                 */
/********************************************************************/

/* 日志打印级别 */
typedef enum {
    ROCKIVA_LOG_ERROR = 0,                  /* Error级别 */
    ROCKIVA_LOG_WARN = 1,                   /* Warn级别 */
    ROCKIVA_LOG_DEBUG = 2,                  /* Debug级别 */
    ROCKIVA_LOG_INFO = 3,                   /* Info级别 */
    ROCKIVA_LOG_TRACE = 4                   /* Trace级别 */
} RockIvaLogLevel;

/* 函数返回错误码 */
typedef enum {
    ROCKIVA_RET_SUCCESS = 0,                 /* 成功 */
    ROCKIVA_RET_FAIL = -1,                   /* 失败 */
    ROCKIVA_RET_NULL_PTR = -2,               /* 空指针 */
    ROCKIVA_RET_INVALID_HANDLE = -3,         /* 无效句柄 */
    ROCKIVA_RET_LICENSE_ERROR = -4,          /* License错误 */
    ROCKIVA_RET_UNSUPPORTED = -5,            /* 不支持 */
    ROCKIVA_RET_STREAM_SWITCH = -6,          /* 码流切换 */
    ROCKIVA_RET_BUFFER_FULL = -7,            /* 缓存区域满 */
} RockIvaRetCode;

/* 图像像素格式 */
typedef enum {
    ROCKIVA_IMAGE_FORMAT_GRAY8 = 0,       /* Gray8 */
    ROCKIVA_IMAGE_FORMAT_RGB888,          /* RGB888 */
    ROCKIVA_IMAGE_FORMAT_BGR888,          /* BGR888 */
    ROCKIVA_IMAGE_FORMAT_RGBA8888,        /* RGBA8888 */
    ROCKIVA_IMAGE_FORMAT_BGRA8888,        /* BGRA8888 */
    ROCKIVA_IMAGE_FORMAT_YUV420P_YU12,    /* YUV420P YU12 */
    ROCKIVA_IMAGE_FORMAT_YUV420P_YV12,    /* YUV420P YV12 */
    ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12,   /* YUV420SP NV12 */
    ROCKIVA_IMAGE_FORMAT_YUV420SP_NV21,   /* YUV420SP NV21 */
    ROCKIVA_IMAGE_FORMAT_YUV422P_YU16,   ///< YUV422P YU16: YYYYYYYYUUUUVVVV
    ROCKIVA_IMAGE_FORMAT_YUV422P_YV16,   ///< YUV422P YV16: YYYYYYYYVVVVUUUU
    ROCKIVA_IMAGE_FORMAT_YUV422SP_NV16,  ///< YUV422SP NV16: YYYYYYYYUVUVUVUV
    ROCKIVA_IMAGE_FORMAT_YUV422SP_NV61,  ///< YUV422SP NV61: YYYYYYYYVUVUVUVU
    ROCKIVA_IMAGE_FORMAT_YUV422_YUYV,    ///< YUV422 YUYV: YUYVYUYV
    ROCKIVA_IMAGE_FORMAT_YUV422_YVYU,    ///< YUV422 YVYU: YVYUYVYU
    ROCKIVA_IMAGE_FORMAT_GRAY16,         ///< Gray16
    ROCKIVA_IMAGE_FORMAT_JPEG,            /* JPEG(输入图像不支持该格式) */
    ROCKIVA_IMAGE_FORMAT_MAX,
} RockIvaImageFormat;

/**
 * 输入图像的旋转模式
 */
typedef enum {
    ROCKIVA_IMAGE_TRANSFORM_NONE              = 0x00,  /* 正常 */
    ROCKIVA_IMAGE_TRANSFORM_FLIP_H            = 0x01,  /* 水平翻转 */
    ROCKIVA_IMAGE_TRANSFORM_FLIP_V            = 0x02,  /* 垂直翻转 */
    ROCKIVA_IMAGE_TRANSFORM_ROTATE_90         = 0x04,  /* 顺时针90度 */
    ROCKIVA_IMAGE_TRANSFORM_ROTATE_180        = 0x03,  /* 顺时针180度 */
    ROCKIVA_IMAGE_TRANSFORM_ROTATE_270        = 0x07,  /* 顺时针270度 */
} RockIvaImageTransform;

/**
 * 内存类型
 */
typedef enum {
    ROCKIVA_MEM_TYPE_CPU = 0,       /* 虚拟地址内存 */
    ROCKIVA_MEM_TYPE_DMA = 1        /* DMA BUF内存 */
} RockIvaMemType;

/* 执行回调结果状态码 */
typedef enum {
    ROCKIVA_SUCCESS = 0,            /* 运行结果正常   */
    ROCKIVA_UNKNOWN = 1,            /* 错误类型未知   */
    ROCKIVA_NULL_PTR = 2,           /* 操作空指针     */
    ROCKIVA_ALLOC_FAILED = 3,       /* 申请空间失败   */
    ROCKIVA_INVALID_INPUT = 4,      /* 无效的输入     */
    ROCKIVA_EXECUTE_FAILED = 6,     /* 内部执行错误   */
    ROCKIVA_NOT_CONFIGURED = 7,     /* 未配置的类型   */
    ROCKIVA_NO_CAPACITY = 8,        /* 业务已建满     */
    ROCKIVA_BUFFER_FULL = 9,        /* 缓存区域满     */
    ROCKIVA_LICENSE_ERROR = 10,     /* license异常   */
    ROCKIVA_JPEG_DECODE_ERROR = 11, /* 解码出错       */
    ROCKIVA_DECODER_EXIT = 12       /* 解码器内部退出  */
} RockIvaExecuteStatus;

/* 目标类型  */
typedef enum {
    ROCKIVA_OBJECT_TYPE_NONE = 0,           /* 未知 */
    ROCKIVA_OBJECT_TYPE_PERSON = 1,         /* 行人 */
    ROCKIVA_OBJECT_TYPE_VEHICLE = 2,        /* 机动车 */
    ROCKIVA_OBJECT_TYPE_NON_VEHICLE = 3,    /* 非机动车 */
    ROCKIVA_OBJECT_TYPE_FACE = 4,           /* 人脸 */
    ROCKIVA_OBJECT_TYPE_HEAD = 5,           /* 人头 */
    ROCKIVA_OBJECT_TYPE_PET = 6,            /* 宠物(猫/狗) */
    ROCKIVA_OBJECT_TYPE_MOTORCYCLE = 7,     /* 电瓶车 */
    ROCKIVA_OBJECT_TYPE_BICYCLE = 8,        /* 自行车 */
    ROCKIVA_OBJECT_TYPE_PLATE = 9,          /* 车牌 */
    ROCKIVA_OBJECT_TYPE_BABY = 10,          /* 婴幼儿 */
    ROCKIVA_OBJECT_TYPE_PACKAGE = 11,       /* 快递包裹 */
    ROCKIVA_OBJECT_TYPE_MAX
} RockIvaObjectType;

/* 目标状态 */
typedef enum {
    ROCKIVA_OBJECT_STATE_NONE,                /* 目标状态未知 */
    ROCKIVA_OBJECT_STATE_FIRST,               /* 目标第一次出现 */
    ROCKIVA_OBJECT_STATE_TRACKING,            /* 目标检测跟踪过程中 */
    ROCKIVA_OBJECT_STATE_LOST,                /* 目标检测丢失 */
    ROCKIVA_OBJECT_STATE_DISPEAR,             /* 目标消失 */
} RockIvaObjectState;

/* 前级目标检测算法模型 */
typedef enum {
    ROCKIVA_DET_MODEL_NONE = 0,           /* 未设置，不需要前级目标检测 */
    ROCKIVA_DET_MODEL_CLS7,           /* 检测类别：人形、人脸、机动车、车牌、非机动车、宠物 */
    ROCKIVA_DET_MODEL_PFCP,           /* 检测类别：人形、人脸、机动车、宠物 */
    ROCKIVA_DET_MODEL_PFP,            /* 检测类别：人形、人脸、宠物 */
    ROCKIVA_DET_MODEL_PHS,            /* 检测类别：人形、头肩 */
    ROCKIVA_DET_MODEL_PHCP,           /* 检测类别：人形、头肩、机动车、宠物 */
    ROCKIVA_DET_MODEL_PERSON,         /* 检测类别：人形 */
    ROCKIVA_DET_MODEL_NONVEHICLE,     /* 检测类别：电动车 */
    ROCKIVA_DET_MODEL_FHS,            /* 检测类别：人脸、头肩 */
    ROCKIVA_DET_MODEL_PHCN,           /* 检测类别：人形、头肩、机动车、非机动车 */
    ROCKIVA_DET_MODEL_CLS8,           /* 检测类别：人形、人脸、机动车、车牌、非机动车、宠物、人头 */
    ROCKIVA_DET_MODEL_PFP_V3,         /* 检测类别：人形、人脸、宠物 */
    ROCKIVA_DET_MODEL_PFCPP           /* 检测类别：人形、人脸、车辆、宠物、快递包裹 */
} RockIvaDetModel;

/* 工作模式 */
typedef enum {
    ROCKIVA_MODE_VIDEO   = 0,              /* 视频流模式(使能跟踪) */
    ROCKIVA_MODE_PICTURE = 1,              /* 图片模式(不使能跟踪) */
} RockIvaWorkMode;
 
typedef enum {
    ROCKIVA_CAMERA_TYPE_ONE = 0,           /* 单摄 */
    ROCKIVA_CAMERA_TYPE_DUAL = 1,          /* 双摄，能够提供RGB和IR两路图像 */
    ROCKIVA_CAMERA_TYPE_SL = 2,            /* 结构光摄像头，能够提供RGB、IR和Depth三路图像 */
} RockIvaCameraType;

/********************************************************************/
/*                          类型定义                                 */
/********************************************************************/

typedef void* RockIvaHandle;

/********************************************************************/
/*                           结构体                                  */
/********************************************************************/
typedef struct {
    void* memAddr;    /* 内存地址 */
    uint32_t memSize; /* 内存长度 */
} RockIvaMemInfo;

typedef struct {
    const char* key;
    char value[ROCKIVA_PARAM_VALUE_MAX_SIZE];
} RockIvaParam;

typedef struct {
    int num;
    RockIvaParam params[ROCKIVA_PARAM_MAX_NUM];
} RockIvaParams;

/* 点坐标 */
typedef struct {
    int16_t x; /* 横坐标，万分比表示，数值范围0~9999 */
    int16_t y; /* 纵坐标，万分比表示，数值范围0~9999 */
} RockIvaPoint;

/* 线坐标 */
typedef struct {
    RockIvaPoint head; /* 头坐标(竖直方向走向的线上方为头) */
    RockIvaPoint tail; /* 尾坐标(竖直方向走向的线下方为尾) */
} RockIvaLine;

/* 正四边形坐标 */
typedef struct {
    RockIvaPoint topLeft;     /* 左上角坐标 */
    RockIvaPoint bottomRight; /* 右下角坐标 */
} RockIvaRectangle;

/* 四边形坐标 */
typedef struct {
    RockIvaPoint topLeft;     /* 左上角坐标 */
    RockIvaPoint topRight;    /* 右上角坐标 */
    RockIvaPoint bottomLeft;  /* 左下角坐标 */
    RockIvaPoint bottomRight; /* 右下角坐标 */
} RockIvaQuadrangle;

/* 区域结构体 */
typedef struct {
    uint32_t pointNum;                               /* 点个数 */
    RockIvaPoint points[ROCKIVA_AREA_POINT_NUM_MAX]; /* 围成区域的所有点坐标 */
} RockIvaArea;

/* 多区域结构体 */
typedef struct {
    uint32_t areaNum;
    RockIvaArea areas[ROCKIVA_AREA_NUM_MAX];
} RockIvaAreas;

/* 目标大小 */
typedef struct {
    uint16_t width;  /* 宽度 */
    uint16_t height; /* 高度 */
} RockIvaSize;

/* 检测框向四周扩展的比例大小配置 */
typedef struct {
    float up;        /* 检测框向上按框高扩展的比例大小，扩展大小为up*人脸框高 */
    float down;      /* 检测框向下按框高扩展的比例大小，扩展大小为down*人脸框高 */
    float left;      /* 检测框向左按框宽扩展的比例大小，扩展大小为left*人脸框宽 */
    float right;     /* 检测框向右按框宽扩展的比例大小，扩展大小为right*人脸框宽 */
} RockIvaRectExpandRatio;

/* 角度信息 */
typedef struct {
    int16_t pitch;             /* 俯仰角,表示绕x轴旋转角度 */
    int16_t yaw;               /* 偏航角,表示绕y轴旋转角度 */
    int16_t roll;              /* 翻滚角,表示绕z轴旋转角度 */
} RockIvaAngle;

/* 图像信息 */
typedef struct {
    uint16_t width;                      /* 图像宽度 */
    uint16_t height;                     /* 图像高度 */
    RockIvaImageFormat format;           /* 图像格式 */
    RockIvaImageTransform transformMode; /* 旋转模式 */
} RockIvaImageInfo;

/* 图像 */
typedef struct {
    uint32_t frameId;      /* 原始采集帧序号 */
    uint32_t channelId;    /* 通道号 */
    RockIvaImageInfo info; /* 图像信息 */
    uint32_t size;         /* 图像数据大小, 图像格式为JPEG时需要 */
    uint8_t* dataAddr;     /* 图像数据地址 */
    uint8_t* dataPhyAddr;  /* 图像数据物理地址 */
    int32_t dataFd;        /* 图像数据DMA buffer fd */
    void* extData;         /* 用户自定义扩展数据 */
} RockIvaImage;

/* 单个目标检测结果信息 */
typedef struct {
    uint32_t objId;            /* 目标ID[0,2^32) */
    uint32_t frameId;          /* 所在帧序号 */
    uint32_t score;            /* 目标检测分数 [1-100] */
    uint32_t clsScore;         /* 目标分类分数，仅开启分类模型有效 [1-100] */
    RockIvaRectangle rect;     /* 目标区域框 (万分比) */
    RockIvaObjectType type;    /* 目标类别 */
} RockIvaObjectInfo;

/* 目标检测结果 */
typedef struct {
    uint32_t frameId;                               /* 帧ID */
    RockIvaImage frame;                             /* 对应的输入图像帧 */
    uint32_t channelId;                             /* 帧通道号 */
    uint32_t objNum;                                /* 目标个数 */
    RockIvaObjectInfo objInfo[ROCKIVA_MAX_OBJ_NUM]; /* 各目标检测信息 */
} RockIvaDetectResult;

/* 需要释放的帧列表 */
typedef struct {
    uint32_t channelId;
    uint32_t count;
    RockIvaImage frames[ROCKIVA_MAX_OBJ_NUM];
} RockIvaReleaseFrames;

 /* 多媒体操作函数 */
typedef struct {
    /**
     * @brief 将图像编码为JPEG图像
     * @param inputImg [in] 输入图像
     * @param outputImg [out] 编码后图像(注意,outputImg.dataAddr需要外部自己申请和释放内存,
     *                        当内部对该图像内存不再使用将会在抓拍回调函数ROCKIVA_FrameReleaseCallback上报)
     */
    int (*ROCKIVA_Yuv2Jpeg)(RockIvaImage *inputImg, RockIvaImage *outputImg, void *userdata);
 } RockIvaMediaOps;

/* SDK通用配置 */
typedef struct {
    RockIvaLogLevel logLevel;            /* 日志等级 */
    char logPath[ROCKIVA_PATH_LENGTH];   /* 日志输出路径 */
    char modelPath[ROCKIVA_PATH_LENGTH]; /* 存放算法模型的目录路径 */
    RockIvaMemInfo license;              /* License信息 */
    uint32_t coreMask;                   /* 指定使用哪个NPU核跑(仅RK3588平台有效) */
    uint32_t channelId;                  /* 通道号 */
    RockIvaImageInfo imageInfo;          /* 输入图像信息 */
    RockIvaAreas roiAreas;               /* 有效区域(该配置对所有算法模块有效) */
    RockIvaCameraType cameraType;        /* 摄像头类型 */
    RockIvaDetModel detModel;            /* 指定前级目标检测算法模型 */
    uint32_t detClasses;                 /* 需要的一些特殊检测类别，如婴儿检测：ROCKIVA_OBJECT_TYPE_BITMASK(ROCKIVA_OBJECT_TYPE_BABY) */
    RockIvaMemInfo detModelData;         /* 指定检测模型数据（用于快启直接配置模型内存数据直接用于加载） */
    uint8_t trackerVersion;              /* 指定跟踪算法版本 0:默认v2; 1:v1; 2:v2 */
    RockIvaMediaOps mediaOps;            /* 设置媒体操作 */
    uint8_t newModelInstance;            /* 每个持有的模型重新创建实例，NVR产品需要跑多路时候开启（设为1），内存占用会增加 */
} RockIvaInitParam;

/* 推帧提供的额外信息 */
typedef struct {
    RockIvaAreas roiAreas;               /* 有效区域（该参数优先级高于RockIvaInitParam.roiArea） */
    RockIvaObjectInfo objInfo;           /* 需要处理的目标信息（可以直接用于识别分析不用再做目标检测） */
    RockIvaParams moduleParams;          /* 模块自定义参数 */
} RockIvaFrameExtraInfo;

typedef struct {
    RockIvaImage rgb;
    RockIvaImage ir;
    RockIvaImage depth;
} RockIvaMultiImage;

/********************************************************************/

/**
 * @brief 帧释放回调函数
 * 
 * releaseFrames 释放帧
 * userdata 用户自定义数据
 * 
 */
typedef void (*ROCKIVA_FrameReleaseCallback)(const RockIvaReleaseFrames* releaseFrames, void *userdata);

/**
 * @brief 算法SDK全局初始化配置
 *
 * @param handle [IN] 要初始化的实例句柄
 * @param mode [IN] 工作模式
 * @param param [IN] 初始化参数
 * @param userdata [IN] 用户自定义数据
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_Init(RockIvaHandle* handle, RockIvaWorkMode mode, const RockIvaInitParam* param, void *userdata);

/**
 * @brief 算法SDK全局销毁释放
 *
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_Release(RockIvaHandle handle);

/**
 * @brief 等待算法帧处理完成
 * 
 * @param handle [IN] handle
 * @param frameId [IN] 要等待的帧ID，如果设为小于0表示等待所有帧处理完
 * @param timeoutMS [IN] 超时时间（毫秒）
 * @return RockIvaRetCode 
 */
RockIvaRetCode ROCKIVA_WaitFinish(RockIvaHandle handle, long frameId, int timeoutMS);

/**
 * @brief 设置帧释放回调
 * 
 * @param handle [IN] handle
 * @param callback [IN] 帧释放回调函数
 * @return RockIvaRetCode 
 */
RockIvaRetCode ROCKIVA_SetFrameReleaseCallback(RockIvaHandle handle, ROCKIVA_FrameReleaseCallback callback);

/**
 * @brief 输入图像帧
 *
 * @param handle [IN] handle
 * @param inputImg [IN] 输入图像帧
 * @param extraInfo [IN] 额外信息(不需要可设NULL)
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_PushFrame(RockIvaHandle handle, const RockIvaImage* inputImg, const RockIvaFrameExtraInfo* extraInfo);

/**
 * @brief 输入图像帧(多摄)
 * 
 * @param handle [IN] handle
 * @param inputImg [IN] 输入图像帧
 * @param extraInfo [IN] 额外信息(不需要可设NULL)
 * @return RockIvaRetCode 
 */
RockIvaRetCode ROCKIVA_PushFrame2(RockIvaHandle handle, const RockIvaMultiImage* inputImg, const RockIvaFrameExtraInfo* extraInfo);

/**
 * @brief 设置参数
 * 
 * @param params [in] 要设置的参数对象
 * @param key [in] 参数键
 * @param value [in] 参数值
 * @return RockIvaRetCode 
 */
RockIvaRetCode ROCKIVA_SetParam(RockIvaParams* params, const char* key, const char* value);

/**
 * @brief 获取SDK版本号
 *
 * @param maxLen [IN] 版本号buffer大小(存储空间需大于64*char)
 * @param version [INOUT] 版本号buffer地址
 * @return RockIvaRetCode
 */
RockIvaRetCode ROCKIVA_GetVersion(const uint32_t maxLen, char* version);

/**
 * @brief 切换工作状态（使能/关闭跟踪，建议只对目标检测模块使用，如果设置为ROCKIVA_MODE_PICTURE模式，一些依赖跟踪的模块结果会异常）
 * 
 * @param handle [in] handle
 * @param mode [in] 工作模式
 * @return RockIvaRetCode 
 */
RockIvaRetCode ROCKIVA_SetWorkMode(RockIvaHandle handle, RockIvaWorkMode mode);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* end of #ifndef __ROCKIVA_COMMON_H__ */

// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _IVM_DEPENDENT_H
#define _IVM_DEPENDENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#ifndef _IVM_API
#define _IVM_API __attribute__((visibility("default")))
#endif

#define IVM_REGIONS_MAX 4
#define IVM_POINT_MAX 10

/* 区域设置
@ivm_regions_t: 多个区域
@ivm_region_t: 单个区域
@ivm_point_t: 单区域内点坐标，取值[0, 1000], 千分比坐标
*/
typedef struct {
	int x;
	int y;
} ivm_point_t;
typedef struct {
	int points_num;
	ivm_point_t points[IVM_POINT_MAX];
} ivm_region_t;
typedef struct {
	int regions_num;
	ivm_region_t regions[IVM_REGIONS_MAX];
} ivm_regions_t;

typedef enum { IVM_SNAP_BEST = 0, IVM_SNAP_FAST, IVM_SNAP_MODE_MAX } ivm_snap_mode_t;
typedef struct {
	ivm_snap_mode_t mode;
	int snap_time_s; // 快速抓拍下，最大抓拍时间，超过该时间即使未达到分数阈值，也将进行抓拍
	int snap_max_num; // 快速抓拍下，最大抓拍张数，同个id多次达到分数阈值，或多次超时时，若已抓拍次数小于该值将会抓拍，否则不处理。设置负数则不限制次数，设置0将被强制为1
} ivm_snap_mode_para_t;

// 画框能力bit
typedef enum {
	IVM_DRAW_PEOPLE = 1,
	IVM_DRAW_FACE = 2,
	IVM_DRAW_CAR = 4,
} ivm_draw_cap_t;

typedef struct {
	ivm_draw_cap_t type;
	unsigned int rgb888_color;
} ivm_draw_color_t;

/* 检测结果
@ivm_type_t = IVM_DETECT
@ivm_ai_object_array_t: 检测结果列表
  @int count: 个数
@ivm_ai_object_t:
  @int id: 目标id(追踪后)
  @ivm_ai_cls_t cls_idx: 目标类型
  @float score: 目标分数
  @ivm_rect_t box: 目标位置，千分比坐标
@ivm_rect_t
  @left: [0, 1000]
  @top: [0, 1000]
  @right: [0, 1000]
  @bottom: [0, 1000]
*/
#define IVM_AI_OBJECT_COUNT_MAX 128
typedef enum {
	IVM_CLS_UNKNOW = 0,
	IVM_CLS_PERSON = 1,
	IVM_CLS_FACE = 2,
	IVM_CLS_CAR = 4,
} ivm_ai_cls_t;
typedef struct {
	int left;
	int top;
	int right;
	int bottom;
} ivm_rect_t;
typedef struct ivp_ai_object_s {
	int id;
	ivm_ai_cls_t cls_idx;
	float score;
	ivm_rect_t box;
} ivm_ai_object_t;
typedef struct {
	int count;
	int human_alarm; // if person in region
	ivm_ai_object_t object[IVM_AI_OBJECT_COUNT_MAX];
} ivm_ai_object_array_t;

/* 客流统计结果
@ivm_type_t = IVM_FACE_REG
@int person_in: 进入
@int person_out: 离开
@tip: 回调内数据不累计
*/
typedef struct {
	int person_in;
	int person_out;
} pf_statistics_t;

/* 人脸识别结果
@ivm_type_t = IVM_FACE_REG
@int id: 注册时返回id
@char name[32]: 注册时使用路径
@float similarity: 相似度[0.0, 1.0],
越大越相似，底库小于3000，推荐0.5以上为识别成功
@int x: 人脸在背景图中位置x
@int y: 人脸在背景图中位置y
@int w: 人脸宽
@int h: 人脸高
@int pic_len: 背景图jpg大小
@char *pic_data: 背景图jpg
@int crop_len: 人脸jpg大小
@char *crop_data: 人脸jpg
*/
typedef struct {
	int id;
	char name[256];
	float similarity;
	int x;
	int y;
	int w;
	int h;
	int pic_len;
	char *pic_data;
	int crop_len;
	char *crop_data;
} ivm_reg_msg_t;

/* 车牌识别结果
@ivm_type_t = IVM_CARPLATE_REG
@char name[32]: 车牌
@int x: 车牌在背景图中位置x
@int y: 车牌在背景图中位置y
@int w: 车牌宽
@int h: 车牌高
@int pic_len: 背景图jpg大小
@char *pic_data: 背景图jpg
@int crop_len: 车牌jpg大小
@char *crop_data: 车牌jpg
*/
typedef struct {
	char name[32];
	int x;
	int y;
	int w;
	int h;
	int pic_len;
	char *pic_data;
	int crop_len;
	char *crop_data;
} ivm_car_msg_t;

/* 人脸抓拍结果，非人脸识别，仅人脸检测, 图像格式jpg
@ivm_type_t = IVM_FACE_SNAP
@int id: 抓拍id
@unsigned int uBestImgSize: 最佳jpg大小
@unsigned char *bestImgBuf: 最佳jpg指针
@unsigned int uBackgroundSize: 最佳时背景图大小
@unsigned char *backgroundImgBuf: 最佳时背景图指针
@float best_score: 最佳分数
*/
typedef struct {
	int id;
	unsigned int uBestImgSize;
	unsigned char *bestImgBuf;
	unsigned int uBackgroundSize;
	unsigned char *backgroundImgBuf;
	float best_score;
} ivm_snap_msg_t;

/*
@int detect_width: 送入检测函数yuv宽
@int detect_height: 送入检测函数yuv高
@char runtime_so_path[128]:
librknn_runtime.so路径，如/usr/lib/librknn_runtime.so
@char lic_path[128]: rockx license路径, 如/data/model/key.lic
@char data_path[128]: rockx 模型dir路径, 如/data/
@char face_db_path[128]: 人脸数据库路径, 如/data/.face.db
@char face_db_bak_path[128]: 人脸数据库备份路径, 如/data/.face_bak.db
@int venc_chn_id: 抓拍功能使用rkmedia编码器通道号, 注意外部不要操作此编码器i,
                  dpd版本无需此参数
*/
typedef struct {
	int detect_width;
	int detect_height;
	char runtime_so_path[128];
	char lic_path[128];
	char data_path[128];
	char face_db_path[128];
	char face_db_bak_path[128];
	int venc_chn_id;
} ivm_init_attr_t;

typedef struct {
	ivm_init_attr_t attr;
	// TODO: venc method
} ivm_dpd_attr_t;

typedef enum {
	IVM_INVALID = 0,
	IVM_DETECT,
	IVM_DRAW,
	IVM_BORDER_ALARM,
	IVM_PERSON_STATISTIC,
	IVM_FACE_SNAP,
	IVM_FACE_REG,
	IVM_CARPLATE_REG,
	IVM_END,
} ivm_type_t;

/*
@IVM_FACE_PERCENTAGE: 取值[0, 100000] 人脸占全图比例 * 100000，
  小于阈值的人脸将不进行人脸抓拍以及人脸识别
@IVM_FAST_SNAP_SCORE: 取值[0, 100], 快速抓拍阈值,
  当检测到分数大于此值时, 且满足最小瞳距时，立即进行快速抓拍,
  仅当ivm_snap_mode_para_t中mode为IVM_SNAP_FAST时生效
@IVM_SNAP_PUPIL_DISTANCE: 取值[0, int_max], 抓拍人脸最小瞳距,
  仅当瞳距大于设定值时进行抓拍
*/
typedef enum {
	IVM_SEN_INVALID = 0,
	IVM_FACE_PERCENTAGE,
	IVM_FAST_SNAP_SCORE,
	IVM_SNAP_PUPIL_DISTANCE,
	IVM_SEN_END,
} ivm_sen_type_t;

/*
@IVM_FLAG_DETECT_REGIONS_LINK_DRAW 检测区域关联画框，设置后，区域外将不画
@IVM_FLAG_DETECT_REGIONS_LINK_SNAP
检测区域关联抓拍，设置后，检测区域外将不进行抓拍
*/
typedef enum {
	IVM_FLAG_INVALID = 0,
	IVM_FLAG_DETECT_REGIONS_LINK_DRAW,
	IVM_FLAG_DETECT_REGIONS_LINK_SNAP,
	IVM_FLAG_END,
} ivm_flag_t;

typedef void *IVM_HANDLE;
/* 结果回调
@ivm_type_t type: 算法类型
@void *event_result: 结果指针，根据算法类型解析
@void *userdata: 启动算法时传入的自定义数据
@tip: 不要在回调内进行耗时操作，否则将会堵塞算法
*/
typedef int (*ivm_event_callback_t)(ivm_type_t type, void *event_result, void *userdata);

/* 初始化
@ivm_dpd_attr_t *attr: 初始参数
@return(IVM_HANDLE): NULL: 初始化失败，否则成功
*/
_IVM_API IVM_HANDLE ivm_dpd_init(ivm_dpd_attr_t *attr);
/* 反初始化
@IVM_HANDLE handle: 句柄
@return: NULL: 初始化失败，否则成功
*/
_IVM_API int ivm_deinit(IVM_HANDLE handle);
/* 启动算法
@IVM_HANDLE handle: 句柄
@ivm_type_t type: 算法类型，支持IVM_DETECT, IVM_BORDER_ALARM,
IVM_PERSON_STATISTIC, IVM_FACE_SNAP, IVM_FACE_REG, IVM_CARPLATE_REG
@void *userdata: 用户自定义数据，将在回调返回
@ivm_event_callback_t cb: 回调
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_open(IVM_HANDLE handle, ivm_type_t type, void *userdata, ivm_event_callback_t cb);
/* 关闭算法
@IVM_HANDLE handle: 句柄
@ivm_type_t type: 算法类型，支持IVM_DETECT, IVM_BORDER_ALARM,
IVM_PERSON_STATISTIC, IVM_FACE_SNAP, IVM_FACE_REG, IVM_CARPLATE_REG
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_close(IVM_HANDLE handle, ivm_type_t type);
/* 设置区域
@IVM_HANDLE handle: 句柄
@ivm_type_t type: 算法类型，支持IVM_DETECT, IVM_BORDER_ALARM,
IVM_PERSON_STATISTIC, IVM_FACE_SNAP IVM_DETECT: 支持1-4个3-10边形, 默认区域全屏
  IVM_BORDER_ALARM: 支持1-4个3-10边形, 默认区域无
  IVM_PERSON_STATISTIC: 支持1个3-10边形, 默认区域无
  IVM_FACE_SNAP: 支持1个矩形, 抓拍作为人脸识别前级，过滤将会影响人脸识别,
默认区域全屏
@ivm_regions_t *regions: 具体见结构体介绍
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_set_regions(IVM_HANDLE handle, ivm_type_t type, ivm_regions_t *regions);
/* 设置灵敏度
@IVM_HANDLE handle: 句柄
@ivm_type_t type: 算法类型，支持IVM_DETECT, IVM_BORDER_ALARM,
IVM_PERSON_STATISTIC, IVM_FACE_SNAP
@int sensitivity: [1, 5] 越大越灵敏, 默认3
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_set_sensitivity(IVM_HANDLE handle, ivm_type_t type, int sensitivity);
/* 设置特殊灵敏度
对于多灵敏度算法由此接口设定其他灵敏度
@IVM_HANDLE handle: 句柄
@ivm_sen_type_t 灵敏度类型
@int sensitivity 取值见灵敏度类型介绍
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_set_sen2(IVM_HANDLE handle, ivm_sen_type_t type, int sen);
/* 设置IVM模块flag
@IVM_HANDLE handle: 句柄
@ivm_flag_t flag: flag
@bool en: 开关，所有flag默认关
*/
_IVM_API int ivm_set_flag(IVM_HANDLE handle, ivm_flag_t flag, bool en);

/* 送入检测数据
@IVM_HANDLE handle: 句柄
@输入buffer格式nv12
@fd: buffer fd
@addr: buf 虚拟地址
@size: buf 大小
@timestamp: buf时间戳
*/
_IVM_API int ivm_dpd_detect_send(IVM_HANDLE handle, int fd, void *addr, size_t size,
                                 unsigned long long timestamp);
/* 画框
@IVM_HANDLE handle: 句柄
@int fd: buf fd, buf格式nv12
@timestamp: buffer时戳，用于校验信息是否过期
@int width: buf宽
@int height: buf高
@int line_width: 线框，设置-1默认
@ivm_draw_cap_t cap: 画框能力bit, 可&传入
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_dpd_draw(IVM_HANDLE handle, int fd, unsigned long long timestamp, int width,
                          int height, int line_width, ivm_draw_cap_t cap);
/*
@IVM_HANDLE handle: 句柄
@ivm_draw_color_t *color: 颜色结构体，指定类型与对应框线颜色
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_draw_color_set(IVM_HANDLE handle, ivm_draw_color_t *color);
/* 设置抓拍模式
@IVM_HANDLE handle: 句柄
@ivm_snap_mode_para_t *para: 抓怕模式，具体见结构体定义
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_set_snap_mode(IVM_HANDLE handle, ivm_snap_mode_para_t *para);
/* 注册人脸
@IVM_HANDLE handle: 句柄
@char *path: 人脸jpg路径
@return >=0: 注册成功，返回人脸id, < 0: 注册失败或重复注册(路径相同)
*/
_IVM_API int ivm_face_register(IVM_HANDLE handle, char *path);
/* 删除人脸
@IVM_HANDLE handle: 句柄
@int id: 注册时返回id
@return: 0: 成功，-1: 失败
*/
_IVM_API int ivm_face_delete(IVM_HANDLE handle, int id);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

// rkAVS_stitchFor1106.h: ��׼ϵͳ�����ļ��İ����ļ�
// ����Ŀ�ض��İ����ļ���
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef BUILDING_DLL
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__((dllexport))
#else
// Note: actually gcc seems to also supports this syntax.
#define DLL_PUBLIC __declspec(dllexport)
#endif
#else
#ifdef __GNUC__
#define DLL_PUBLIC __attribute__((dllimport))
#else
// Note: actually gcc seems to also supports this syntax.
#define DLL_PUBLIC
#endif
#define DLL_LOCAL
#endif
#else
#if __GNUC__ >= 4
#define DLL_PUBLIC __attribute__((visibility("default")))
#define DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define DLL_PUBLIC
#define DLL_LOCAL
#endif
#endif

#define CAMERA_NUM 2
#define BAND_NUM 4
#define MESH_SCALE_BIT 4
/** @struct RK_AVS_IMAGE_SIZE
 *  @brief  Struct of image size.
 */
typedef struct RK_AVS_IMAGE_SIZE {
	int32_t s32ImageWidth;  /**< Width */
	int32_t s32ImageHeight; /**< Height */
} AVS_IMAGE_SIZE;

/** @struct RK_AVS_IMAGE_YUV_PLANAR
 *  @brief  Struct of Yuv Planar format.
 */
typedef struct RK_AVS_IMAGE_YUV_PLANAR {
	void *y; /**< Head pointer of the Y image */
	void *u; /**< Head pointer of the U image */
	void *v; /**< Head pointer of the V image */
} AVS_IMAGE_YUV_PLANAR;

/** @struct RK_AVS_IMAGE_YUV_SEMI_PLANAR
 *  @brief  Struct of Yuv SemiPlanar format.
 */
typedef struct RK_AVS_IMAGE_YUV_SEMI_PLANAR {
	void *y;  /**< Head pointer of the Y image */
	void *uv; /**< Head pointer of the UV image */
} AVS_IMAGE_YUV_SEMI_PLANAR;

/** @struct RK_AVS_IMAGE_DATA
 *  @brief  Image Data.
 */
typedef struct RK_AVS_IMAGE_DATA {
	AVS_IMAGE_SIZE imageSize; /**< Size of image */
	union {
		void *p;                              /**< Head pointer of the image */
		AVS_IMAGE_YUV_PLANAR planar;          /**< Planar struct */
		AVS_IMAGE_YUV_SEMI_PLANAR semiPlanar; /**< Semiplanar struct */
	} dat;                                    /**< image data pointer */
} AVS_IMAGE_DATA;

/** @struct RK_AVS_IMAGE_YUV_PLANAR_PITCH
 *  @brief  Pitch value of Yuv Planar format.
 */
typedef struct RK_AVS_IMAGE_YUV_PLANAR_PITCH {
	int32_t y; /**< Y data. Number of bytes from head of line to the head of next line */
	int32_t u; /**< U data. Number of bytes from head of line to the head of next line */
	int32_t v; /**< V data. Number of bytes from head of line to the head of next line */
} AVS_IMAGE_YUV_PLANAR_PITCH;

/** @struct RK_AVS_IMAGE_YUV_SEMI_PLANAR_PITCH
 *  @brief  Pitch value of Yuv SemiPlanar format.
 */
typedef struct RK_AVS_IMAGE_YUV_SEMI_PLANAR_PITCH {
	int32_t y; /**< Y data. Number of bytes from head of line to the head of next line */
	int32_t
	    uv; /**< UV data. Number of bytes from head of line to the head of next line */
} AVS_IMAGE_YUV_SEMI_PLANAR_PITCH;

/** @struct RK_AVS_IMAGE_DATA_EX
 *  @brief  Image Data.
 */
typedef struct RK_AVS_IMAGE_DATA_EX {
	AVS_IMAGE_SIZE imageSize; /**< Size of image */
	int32_t fd;               /**< file descriptor */
	union {
		void *p;                              /**< Head pointer of the image data */
		AVS_IMAGE_YUV_PLANAR planar;          /**< Planar struct */
		AVS_IMAGE_YUV_SEMI_PLANAR semiPlanar; /**< Semiplanar struct */
	} dat;                                    /**< image data pointer */
	union {
		int32_t p; /**< Number of bytes from head of line to the head of next line */
		AVS_IMAGE_YUV_PLANAR_PITCH planar;           /**< Planar pitch struct */
		AVS_IMAGE_YUV_SEMI_PLANAR_PITCH semi_planar; /**< Semiplanar pitch struct */
	} pitch;                                         /**< image pitch union */
} AVS_IMAGE_DATA_EX;

/** @struct RK_AVS_STICH_PARAMS
 *  @brief  Struct of image stitch params.
 */
typedef struct RK_AVS_STICH_BLOCK_PARAMS {
	int32_t s32SrcOverlapStartY;
	int32_t s32SrcNonverlapStartY;
	int32_t s32SrcOverlapHeight;
	int32_t s32SrcNonverlapHeight;

	int32_t s32DstOverlapStartY;
	int32_t s32DstNonverlapStartY;
	int32_t s32DstOverlapHeight;
	int32_t s32DstNonverlapHeight;
} AVS_STICH_BLOCK_PARAMS;

typedef struct RK_AVS_STITCH_ROI_PARAMS {
	int32_t s32SrcCopyStartX;
	int32_t s32SrcCopyStartY;
	int32_t s32SrcCopyHeight;
	int32_t s32SrcCopyWidth;

	int32_t s32DstCopyStartX;
	int32_t s32DstCopyStartY;
	int32_t s32DstCopyHeight;
	int32_t s32DstCopyWidth;

	int32_t s32SrcRemapStartX;
	int32_t s32SrcRemapStartY;
	int32_t s32SrcRemapHeight;
	int32_t s32SrcRemapWidth;
} AVS_STITCH_ROI_PARAMS;

typedef struct rkAVS_STITCH_PARAMS_S {
	int32_t s32BandNum;
	AVS_IMAGE_SIZE stStitchImageSize;
	AVS_IMAGE_SIZE stAlphaSize;
	AVS_STITCH_ROI_PARAMS stStitchRoiParams[CAMERA_NUM];
	AVS_STICH_BLOCK_PARAMS stStitchBlockParams[CAMERA_NUM];
} AVS_STITCH_PARAMS_S;

/** @struct RK_AVS_INPUT_PARAMS
 *  @brief  Struct of input params.
 */
typedef struct RK_AVS_INPUT_PARAMS {
	int32_t s32LogLevel;
	uint8_t *pu8LeftMesh;
	uint8_t *pu8RightMesh;
	uint8_t *pu8AlphaYuv;
	uint8_t *pu8TmpBuffer;
	AVS_STITCH_PARAMS_S stStitchParams;
} AVS_INPUT_PARAMS;

typedef struct RK_AVS_ENGINE {
	void *p;
	int32_t s32AsynchSelect;
} AVS_ENGINE;
DLL_PUBLIC int32_t rkAVS_getVersion(char avsToolVersion[128]);
DLL_PUBLIC int32_t rkAVS_initParams(AVS_ENGINE *stEngine);
DLL_PUBLIC int32_t rkAVS_stitchImages(AVS_ENGINE *stEngine,
                                      AVS_IMAGE_DATA_EX *stLeftImageEx,
                                      AVS_IMAGE_DATA_EX *stRightImageEx,
                                      AVS_IMAGE_DATA_EX *stStitchImageEx,
                                      AVS_INPUT_PARAMS *stInputParams);
DLL_PUBLIC int32_t rkAVS_destroy(AVS_ENGINE *stEngine);

#ifdef __cplusplus
} /* extern "C" { */
#endif
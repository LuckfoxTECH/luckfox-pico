/*********************************************************************************
* Copyright(C),2019, TUYA www.tuya.com

* FileName:		tuya_ipc_img_proc.h
* Note			tuya_ipc_img_proc api
* Version		V1.0.0
* Data			2019.04
**********************************************************************************/

#ifndef _TUYA_IPC_IMG_PROC_H_
#define _TUYA_IPC_IMG_PROC_H_

#include "tuya_ipc_img_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************
 * Image resize enum
 * lINEAR		Faster but low quality
 * CUBIC			Slower but high quality
 **********************************************************************************/
typedef enum {
	LINEAR,
	CUBIC,

} IMG_RESIZE_TYPE;

/*********************************************************************************
 * Image resize struct, please scale the width and height equally
 * srcWidth		Input width
 * srcHeight		Input height
 * dstWidth		Output width
 * dstHeight		Output height
 * type			Scale type
 **********************************************************************************/
typedef struct _TUYA_IMG_RESIZE_PARA {
	INT_T srcWidth;
	INT_T srcHeight;
	INT_T dstWidth;
	INT_T dstHeight;
	IMG_TYPE img_type;
	IMG_RESIZE_TYPE resize_type;

} TUYA_IMG_RESIZE_PARA;

/*********************************************************************************
 * YUV420 image scale interface
 * in_data  		input YUV420
 * paras			scale struct
 * out_data		output YUV420
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Img_Resize(UCHAR_T *in_data, TUYA_IMG_RESIZE_PARA paras, UCHAR_T *out_data);

/*********************************************************************************
 * yuv4202b8g8r8
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_ImgConvert_yuv4202bgr888(UCHAR_T *yImg, UCHAR_T *uImg, UCHAR_T *vImg,
                                              INT_T width, INT_T height, UCHAR_T *dstB,
                                              UCHAR_T *dstG, UCHAR_T *dstR);

/*********************************************************************************
 * spec of TUYA  AI
 **********************************************************************************/
typedef struct _TUYA_AI_SPEC {
	INT_T num;
	INT_T *pclass;
	TUYA_AI_RECT *rect;
} TUYA_AI_SPEC;

/*********************************************************************************
 * draw rect to Yuv
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Img_DrawRect_YUV(UCHAR_T *yImg, INT_T width, INT_T height,
                                      TUYA_AI_SPEC ai_spec);

/*********************************************************************************
 * get Yuv of the first rect
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Img_Get_Rect_YUV(UCHAR_T *srcImg, INT_T width, INT_T height,
                                      TUYA_AI_SPEC ai_spec, UCHAR_T *dstImg);

/*********************************************************************************
 * ARGB1555 OSD
 * srcImg  		input data and output data, RGB24
 * width			width of srcImg
 * height		height of srcImg
 * osdImg		osd argb1555 data
 * osdRect		chan0 0,0,640,64	chan1 0,0,160,16
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Img_OSD_ARGB1555(UCHAR_T *srcImg, INT_T width, INT_T height, UINT16_T *osdImg,
                                      TUYA_AI_RECT osdRect);

#ifdef __cplusplus
}
#endif

#endif // !_TUYA_IPC_IMG_PROC_H_

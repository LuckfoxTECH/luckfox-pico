/*********************************************************************************
* Copyright(C),2019, TUYA www.tuya.com

* FileName:		tuya_ipc_img_defs.h
* Note			tuya img lib basic defs
* Version		V1.0.0
* Data			2019.12
**********************************************************************************/

#ifndef _TUYA_IPC_IMG_DEFS_H_
#define _TUYA_IPC_IMG_DEFS_H_

#include "tuya_cloud_error_code.h"
#include "tuya_cloud_types.h"

/*  */
#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************
 * Image type
 * Y
 * YUV
 * BGR
 **********************************************************************************/
typedef enum {
	Y,
	YUV,
} IMG_TYPE;

/*********************************************************************************
 * Coordinate of pixels
 **********************************************************************************/
typedef struct _TUYA_POINT {
	INT_T x;
	INT_T y;

} TUYA_POINT;

/*********************************************************************************
 * ROI of Motion Detect
 * x y  				Percent of pixels' coordinate
 * width				Percent of width
 * height 			Percent of height
 **********************************************************************************/
typedef struct _TUYA_RPERCENT_ECT {
	INT_T x_percent;
	INT_T y_percent;
	INT_T width_percent;
	INT_T height_percent;

} TUYA_RPERCENT_ECT;

/*********************************************************************************
 * rect of TUYA  AI
 **********************************************************************************/
typedef struct _TUYA_RECT {
	INT_T left;
	INT_T top;
	INT_T right;
	INT_T bottom;
} TUYA_AI_RECT;

#ifdef __cplusplus
}
#endif

#endif // !_TUYA_IPC_IMG_DEFS_H_

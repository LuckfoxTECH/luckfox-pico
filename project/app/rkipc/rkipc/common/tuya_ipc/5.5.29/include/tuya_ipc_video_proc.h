/*********************************************************************************
* Copyright(C),2019, TUYA www.tuya.com

* FileName:		tuya_ipc_video_proc.h
* Note			tuya_ipc_video_proc api
* Version		V1.0.0
* Data			2019.02
**********************************************************************************/

#ifndef _TUYA_IPC_VIDEO_PROC_H_
#define _TUYA_IPC_VIDEO_PROC_H_

#include "tuya_ipc_img_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************
 * multi_regions when setting multi md region, for example 5x5 4x3
 * rows
 * cols
 * region_num
 * region_list		rect data
 **********************************************************************************/
typedef struct _TUYA_MULTI_MD_REGION {
	INT_T rows;
	INT_T cols;
	INT_T region_num;
	TUYA_AI_RECT *region_list;
} TUYA_MULTI_MD_REGION;

/*********************************************************************************
 * setting region type MD
 * SINGLE
 * MULTI
 **************************
 ********************************************************/
typedef enum {
	SINGLE = 1,
	MULTI,
} TUYA_MD_REGION_TYPE;

/*********************************************************************************
* frame_w 			Input width
* frame_h 			Input height
* y_thd         	Thres value of Motion Detect, default 30, set 5 when low-light, recommend 5-30
* sensitivity		Sensitivity of Motion Detect，range 1-7, more sensitive
* rect_type			use NONE when set no rect;
                    use TUYA_RPERCENT_RECT when set SINGLE;
                    use TUYA_MULTI_MD_REGION when set MULTI
* roi				ROI of Motion Detect
* motion_region			MULTI ROI of Motion Detect; range 1x1 - 5x5
* tracking_enable	Switch of Motion Tracking，O close ,1 open
**********************************************************************************/
typedef struct _TUYA_MOTION_TRACKING_CFG {
	INT_T frame_w;
	INT_T frame_h;

	INT_T y_thd;
	INT_T sensitivity; // 1-7

	TUYA_MD_REGION_TYPE rect_type;
	TUYA_RPERCENT_RECT roi;
	TUYA_MULTI_MD_REGION motion_region;
	INT_T tracking_enable; // 1 open 0 close

} TUYA_MOTION_TRACKING_CFG;

/*********************************************************************************
 * Init input config.
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Motion_Init(TUYA_MOTION_TRACKING_CFG mt_cfg);

/*********************************************************************************
 * Set config dynamically.
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Set_Motion(TUYA_MOTION_TRACKING_CFG mt_cfg);

/*********************************************************************************
 * Get config dynamically.
 **********************************************************************************/
void Tuya_Ipc_Get_Motion(TUYA_MOTION_TRACKING_CFG *mt_cfg);

/*********************************************************************************
 * release.
 **********************************************************************************/
void Tuya_Ipc_Motion_Release();

/*********************************************************************************
 * get a background frame.
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_Get_BackGround_Frame();

/*********************************************************************************
 * do background sub.
 **********************************************************************************/
OPERATE_RET Tuya_Ipc_BackGround_Sub(INT_T *background_motion_flag);

/*********************************************************************************
* Execute Motion Detect \ Motion Tracking.
* in_data  		Input YUV.
* motion_flag 	Return the value of Motion Detect.
                0 for no moving; 1 for moving exist.
* motion_point	Return the center point coordinate of the largest moving object.
                Both values(x, y) are 0 for no moving; otherwise, moving exist.
                Return values(0, 0) when tracking_enable==0
**********************************************************************************/
OPERATE_RET Tuya_Ipc_Motion(UCHAR_T *in_data, INT_T *motion_flag, TUYA_POINT *motion_point);

#ifdef __cplusplus
}
#endif

#endif // !_TUYA_IPC_VIDEO_PROC_H_

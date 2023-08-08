/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RT_MPI_VGS_H_
#define INCLUDE_RT_MPI_VGS_H_

#include "rk_comm_vgs.h"
#include "rk_comm_video.h"
#include "rk_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /*__cplusplus*/

/*****************************************************************************
 Prototype    : RK_MPI_VGS_BeginJob
 Description  : Begin a vgs job,then add task into the job,vgs will finish all
the task in the job. Input        : VGS_HANDLE *phHandle Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_BeginJob(VGS_HANDLE *phHandle);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_EndJob
 Description  : End a job,all tasks in the job will be submmitted to vgs
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_EndJob(VGS_HANDLE hHandle);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_CancelJob
 Description  : Cancel a job,then all tasks in the job will not be submmitted to
vgs Input        : VGS_HANDLE hHandle Output       : None Return Value : Calls :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_CancelJob(VGS_HANDLE hHandle);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddScaleTask
 Description  : Add a Scale task to a vgs job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddScaleTask(VGS_HANDLE hHandle,
                               const VGS_TASK_ATTR_S *pstTask,
                               VGS_SCLCOEF_MODE_E enScaleCoefMode);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddRotationTask
 Description  : add a Rotation task into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddRotationTask(VGS_HANDLE hHandle,
                                  const VGS_TASK_ATTR_S *pstTask,
                                  ROTATION_E enRotationAngle);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddCropTask
 Description  : add crop task into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddCropTask(VGS_HANDLE hHandle,
                              const VGS_TASK_ATTR_S *pstTask,
                              const VGS_CROP_INFO_S *pstVgsCrop);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddDrawLineTask
 Description  : add a draw line task into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddDrawLineTask(VGS_HANDLE hHandle,
                                  const VGS_TASK_ATTR_S *pstTask,
                                  const VGS_DRAW_LINE_S *pstVgsDrawLine);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddCoverTask
 Description  : add a Cover task into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddCoverTask(VGS_HANDLE hHandle,
                               const VGS_TASK_ATTR_S *pstTask,
                               const VGS_ADD_COVER_S *pstVgsAddCover);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddOsdTask
 Description  : add a Osd task into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddOsdTask(VGS_HANDLE hHandle, const VGS_TASK_ATTR_S *pstTask,
                             const VGS_ADD_OSD_S *pstVgsAddOsd);

/****************************************
 Prototype    : RK_MPI_VGS_AddMosaicTask
 Description  : add mosaic task into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddMosaicTask(VGS_HANDLE hHandle,
                                const VGS_TASK_ATTR_S *pstTask,
                                const VGS_MOSAIC_S *pstVgsMosaic);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddDrawLineTaskArray
 Description  : add draw line tasks into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddDrawLineTaskArray(VGS_HANDLE hHandle,
                                       const VGS_TASK_ATTR_S *pstTask,
                                       const VGS_DRAW_LINE_S astVgsDrawLine[],
                                       RK_U32 u32ArraySize);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddCoverTaskArray
 Description  : add cover tasks into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddCoverTaskArray(VGS_HANDLE hHandle,
                                    const VGS_TASK_ATTR_S *pstTask,
                                    const VGS_ADD_COVER_S astVgsAddCover[],
                                    RK_U32 u32ArraySize);

/*****************************************************************************
 Prototype    : RK_MPI_VGS_AddOsdTaskArray
 Description  : add osd tasks into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddOsdTaskArray(VGS_HANDLE hHandle,
                                  const VGS_TASK_ATTR_S *pstTask,
                                  const VGS_ADD_OSD_S astVgsAddOsd[],
                                  RK_U32 u32ArraySize);

/****************************************
 Prototype    : RK_MPI_VGS_AddMosaicTask
 Description  : add mosaic tasks into a job
 Input        : VGS_HANDLE hHandle
 Output       : None
 Return Value :
 Calls        :
 Called By    :
*****************************************************************************/
RK_S32 RK_MPI_VGS_AddMosaicTaskArray(VGS_HANDLE hHandle,
                                     const VGS_TASK_ATTR_S *pstTask,
                                     const VGS_MOSAIC_S astVgsMosaic[],
                                     RK_U32 u32ArraySize);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /*__cplusplus*/

#endif /*end of __MPI_VGS_H__*/

// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdbool.h>
#include "rk_type.h"
#include "smart_door.h"
#include "vi_face.h"
#include "rk_comm_tde.h"
#include "rk_mpi_tde.h"

#include "face_unlock.hh"
#include "smart_unlock.hh"

using namespace std;

// #define SAVE_FRAME
#define SAVE_FRAME_CNT_MAX	1

#define SENSOR_WIDTH		1280
#define SENSOR_HEIGHT		720
#define TDE_ROTATE

#define MODEL_PATH           "/oem/usr/share/models/"
#define FACE_DATA_PATH       "/userdata/lib/"
#define DEPTH_CALIB_PATH     "/userdata/lib/"
#define DEPTH_INIT_DATA_PATH "/userdata/lib/"

typedef struct rk_smart_door_face_cfg {
	SMART_DOOR_VI_CFG vi_cfg[CAM_NUM];
	int origin_ir_h;
	int origin_ir_w;
	int in_ir_h;
	int in_ir_w;
	int in_speckle_h;
	int in_speckle_w;
	int tar_id;
	char *frame_right;
	char *frame_left;
	RK_BOOL frame_ready;
	face_use_e use_state;
	detect_res_t detect_res;	//return from smart_unlock
} SMART_DOOR_FACE_CFG;

SMART_DOOR_FACE_CFG face_cfg;
Smart_unlock *smart_unlock = NULL;

#ifdef TDE_ROTATE
static RK_S32 tde_create_dstBlk(uint32_t u32DstWidth, uint32_t u32DstHeight, MB_BLK *pstDstBlk)
{
	RK_S32 s32Ret = RK_FAILURE;
	PIC_BUF_ATTR_S stPicBufAttr;
	MB_PIC_CAL_S stMbPicCalResult;

	stPicBufAttr.u32Width = u32DstWidth;
	stPicBufAttr.u32Height = u32DstHeight;
	stPicBufAttr.enPixelFormat = RK_FMT_YUV420SP;
	stPicBufAttr.enCompMode = COMPRESS_MODE_NONE;
	s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
	if (s32Ret != RK_SUCCESS) {
		return s32Ret;
	}

	s32Ret = RK_MPI_SYS_MmzAlloc(pstDstBlk, RK_NULL, RK_NULL, stMbPicCalResult.u32MBSize);
	return s32Ret;
}

static RK_S32 tde_free_dstBlk(MB_BLK pstDstBlk)
{
	if (pstDstBlk)
		RK_MPI_SYS_Free(pstDstBlk);

	return RK_SUCCESS;
}

static RK_S32 vi_frame_tde_rotate(SMART_DOOR_VI_CFG *vi_cfg, MB_BLK dstBlk, void **dst_data,
				  RK_S32 *dst_size)
{
	RK_S32 s32Ret = RK_FAILURE;
	TDE_SURFACE_S pstSrc;
	TDE_RECT_S pstSrcRect;
	TDE_SURFACE_S pstDst;
	TDE_RECT_S pstDstRect;
	MB_BLK srcBlk = RK_NULL;
	PIC_BUF_ATTR_S stPicBufAttr;
	MB_PIC_CAL_S stMbPicCalResult;

	if (!vi_cfg || !dstBlk || !dst_size)
		return s32Ret;

	srcBlk = vi_cfg->stViFrame.stVFrame.pMbBlk;

	TDE_HANDLE hHandle = RK_TDE_BeginJob();
	if (hHandle == RK_ERR_TDE_INVALID_HANDLE) {
		RK_LOGE("start tde job fail");
		return s32Ret;
	}

	pstSrc.pMbBlk = srcBlk;
	pstDst.pMbBlk = dstBlk;

	pstSrc.u32Width = vi_cfg->stChnAttr.stSize.u32Width;
	pstSrc.u32Height = vi_cfg->stChnAttr.stSize.u32Height;
	pstSrc.enColorFmt = RK_FMT_YUV420SP;
	pstSrc.enComprocessMode = COMPRESS_MODE_NONE;
	pstSrcRect.s32Xpos = 0;
	pstSrcRect.s32Ypos = 0;
	pstSrcRect.u32Width = pstSrc.u32Width;
	pstSrcRect.u32Height = pstSrc.u32Height;
	pstDst.u32Width = vi_cfg->stChnAttr.stSize.u32Height;
	pstDst.u32Height = vi_cfg->stChnAttr.stSize.u32Width;
	pstDst.enColorFmt = RK_FMT_YUV420SP;
	pstDst.enComprocessMode = COMPRESS_MODE_NONE;
	pstDstRect.s32Xpos = 0;
	pstDstRect.s32Ypos = 0;
	pstDstRect.u32Width = pstDst.u32Width;
	pstDstRect.u32Height = pstDst.u32Height;

	s32Ret = RK_TDE_Rotate(hHandle, &pstSrc, &pstSrcRect, &pstDst, &pstDstRect, ROTATION_270);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_Rotate fail");
		RK_TDE_CancelJob(hHandle);
		return s32Ret;
	}

	s32Ret = RK_TDE_EndJob(hHandle, RK_FALSE, RK_TRUE, 10);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("RK_TDE_EndJob fail");
		RK_TDE_CancelJob(hHandle);
		return s32Ret;
	}

	RK_TDE_WaitForDone(hHandle);

	stPicBufAttr.u32Width = pstDst.u32Width;
	stPicBufAttr.u32Height = pstDst.u32Height;
	stPicBufAttr.enPixelFormat = pstDst.enColorFmt;
	stPicBufAttr.enCompMode = pstDst.enComprocessMode;
	s32Ret = RK_MPI_CAL_TDE_GetPicBufferSize(&stPicBufAttr, &stMbPicCalResult);
	if (s32Ret != RK_SUCCESS) {
		RK_LOGE("get picture buffer size failed. err 0x%x", s32Ret);
		RK_TDE_CancelJob(hHandle);
		return s32Ret;
	}

	*dst_size = stMbPicCalResult.u32MBSize;
	*dst_data = RK_MPI_MB_Handle2VirAddr(dstBlk);
	RK_LOGD("get frame:%p, size:%d, bBlk:%p", *dst_data, *dst_size, dstBlk);
	RK_MPI_SYS_MmzFlushCache(dstBlk, RK_TRUE);

	return s32Ret;
}
#endif

static void init_vi(SMART_DOOR_VI_CFG *vi_cfg, RK_S32 dev_id, RK_S32 channel_id,
		    RK_S32 width, RK_S32 height)
{
	if (!vi_cfg) {
		RK_LOGE("init_vi error!");
		return;
	}

	vi_cfg->devId = dev_id;
	vi_cfg->pipeId = vi_cfg->devId;
	vi_cfg->channelId = channel_id;
	memcpy(vi_cfg->stChnAttr.stIspOpt.aEntityName, "rkisp_mainpath", strlen("rkisp_mainpath"));
	vi_cfg->loopCountSet = SAVE_FRAME_CNT_MAX;
	vi_cfg->bFreeze = RK_FALSE;
	vi_cfg->bIfIspGroupInit = RK_TRUE;
	vi_cfg->stChnAttr.stSize.u32Width = width;
	vi_cfg->stChnAttr.stSize.u32Height = height;
	vi_cfg->stChnAttr.enCompressMode = COMPRESS_MODE_NONE;
	vi_cfg->stChnAttr.stIspOpt.u32BufCount = 2;
	vi_cfg->stChnAttr.stIspOpt.enMemoryType = VI_V4L2_MEMORY_TYPE_DMABUF;
	vi_cfg->stChnAttr.u32Depth = 1;
	vi_cfg->stChnAttr.enPixelFormat = RK_FMT_YUV420SP;
	vi_cfg->stChnAttr.stFrameRate.s32SrcFrameRate = -1;
	vi_cfg->stChnAttr.stFrameRate.s32DstFrameRate = -1;
}

static RK_S32 create_vi(SMART_DOOR_VI_CFG *vi_cfg)
{
	return smart_door_create_vi(*vi_cfg);
}

static void print_face_detect_res(detect_res_t *detect_res)
{
	if (!detect_res)
		return;

	printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Face recognition result↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n");
	switch (detect_res->unlock_state) {
	case RESULT_DETECT_FACE_OK:
		printf("\t\tFace detection completed.\n");
		break;
	case RESULT_NO_FACE:
		printf("\t\tNo face detected.\n");
		break;
	case RESULT_BIG_ANGLE:
		printf("\t\tIncorrect face angle.\n");
		break;
	case RESULT_IR_FAIL:
		printf("\t\tIR liveness detection failed.\n");
		break;
	case RESULT_DEPTH_FAIL:
		printf("\t\tDepth liveness detection failed.\n");
		break;
	case RESULT_TOO_CLOSE:
		printf("\t\tFace is too close.\n");
		break;
	case RESULT_TOO_FAR:
		printf("\t\tFace is too far.\n");
		break;
	case RESULT_TOO_LEFT:
		printf("\t\tFace is too left.\n");
		break;
	case RESULT_TOO_RIGHT:
		printf("\t\tFace is too right.\n");
		break;
	case RESULT_TOO_HIGH:
		printf("\t\tFace is too high.\n");
		break;
	case RESULT_TOO_LOW:
		printf("\t\tFace is too low.\n");
		break;
	case RESULT_NO_ID:
		printf("\t\tFace ID not found. Registration required.\n");
		break;
	case RESULT_COMPARE_FACE_OK:
		printf("\t\tFace recognition successful. ID=%d.\n",
		       detect_res->face_id);
		break;
	case RESULT_SAVE_FACE_OK:
		printf("\t\tFace ID=%d saved successfully.\n",
		       detect_res->face_id);
		break;
	case RESULT_SAVE_FACE_FAIL:
		printf("\t\tFailed to save face ID=%d.\n",
		       detect_res->face_id);
		break;
	case RESULT_NULL:
		printf("\t\tEmpty result.\n");
		break;
	case RESULT_FAS_OK:
		printf("\t\tLiveness detection successful.\n");
		break;
	case RESULT_FACE_EXIST:
		printf("\t\tThe face ID=%d already exists.\n",
		       detect_res->face_id);
		break;
	case RESULT_REGISTER_FULL:
		printf("\t\tRegistration for face ID is full.\n");
		break;
	case RESULT_IR_OK:
		printf("\t\tIR liveness detection successful.\n");
		break;
	case RESULT_DEPTH_OK:
		printf("\t\tDepth liveness detection successful.\n");
		break;
	case RESULT_DIR_ERROR:
		printf("\t\tIncorrect face orientation.\n");
		break;
	default:
		printf("\t\tError result!\n");
		break;
	}
	printf("↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Face recognition result↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n");
}

static RK_S32 face_unlock_flow(SMART_DOOR_FACE_CFG *face_cfg)
{
	if (!face_cfg) {
		RK_LOGE("check param error!");
		return RK_FAILURE;
	}

	if (face_cfg->frame_ready != RK_TRUE) {
		RK_LOGE("face frame is not ready, skip.");
		return RK_SUCCESS;
	}

	face_cfg->detect_res = smart_unlock->face_unlock_flow(face_cfg->frame_left,
							      face_cfg->frame_left,
							      face_cfg->frame_right,
							      face_cfg->use_state,
							      face_cfg->tar_id);
	print_face_detect_res(&face_cfg->detect_res);
	return RK_SUCCESS;
}

static void *vi_get_stream_face(void *arg)
{
	SMART_DOOR_FACE_CFG *face_cfg = (SMART_DOOR_FACE_CFG *)arg;
	RK_S32 s32Ret = RK_FAILURE;
	char name[CAM_NUM][64] = {0};
	void *data[CAM_NUM];
	RK_S32 size[CAM_NUM];
	int max_loopCount = 60;
	int loopCount = 0;
	int i;
	int max_pts_gap = 2;	//(ms), for judging if frames are sync or not

#ifdef SAVE_FRAME
	char const *dstFilePath = "/tmp/";
	FILE *fp[CAM_NUM];

	for (i = 0; i < CAM_NUM; i++) {
		snprintf(name[i], sizeof(name[i]), "/%s/vi_%d.bin", dstFilePath, face_cfg->vi_cfg[i].devId);
		fp[i] = fopen(name[i], "wb");
		if (fp[i] == RK_NULL) {
			RK_LOGE("dev id %d can't open %s file !\n", face_cfg->vi_cfg[i].devId, dstFilePath);
			return RK_NULL;
		}
	}
#endif

#ifdef TDE_ROTATE
	MB_BLK dstBlk[CAM_NUM];

	for (i = 0; i < CAM_NUM; i++) {
		//change height <--> width for rotate
		s32Ret = tde_create_dstBlk(face_cfg->vi_cfg[i].stChnAttr.stSize.u32Height,
					   face_cfg->vi_cfg[i].stChnAttr.stSize.u32Width,
					   &dstBlk[i]);
		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("tde_create_dstBlk fail: %x", s32Ret);
			goto exit;
		}
	}
#endif

	while (loopCount < max_loopCount) {
		for (i = 0; i < CAM_NUM; i++) {
			data[i] == NULL;
			s32Ret |= smart_door_vi_get_frame(&face_cfg->vi_cfg[i], &data[i]);
		}

		//For dual cameras, check if frames are sync or not
		if (s32Ret == RK_SUCCESS && CAM_NUM == 2) {
			//Keep seq synchronized
			int seq_0 = face_cfg->vi_cfg[0].stViFrame.stVFrame.u32TimeRef;
			int seq_1 = face_cfg->vi_cfg[1].stViFrame.stVFrame.u32TimeRef;
			while (abs(seq_0 - seq_1)) {
				int drop_dev_id = (seq_0 < seq_1) ? 0 : 1;
				RK_LOGI("drop unsynchronized frames: dev %d seq %d.", drop_dev_id,
					face_cfg->vi_cfg[drop_dev_id].stViFrame.stVFrame.u32TimeRef);
				smart_door_vi_release_frame(&face_cfg->vi_cfg[drop_dev_id]);
				data[drop_dev_id] == NULL;
				s32Ret |= smart_door_vi_get_frame(&face_cfg->vi_cfg[drop_dev_id], &data[drop_dev_id]);
				seq_0 = face_cfg->vi_cfg[0].stViFrame.stVFrame.u32TimeRef;
				seq_1 = face_cfg->vi_cfg[1].stViFrame.stVFrame.u32TimeRef;
			}

			//Keep pts synchronized
			int pts_0 = face_cfg->vi_cfg[0].stViFrame.stVFrame.u64PTS / 1000;
			int pts_1 = face_cfg->vi_cfg[1].stViFrame.stVFrame.u64PTS / 1000;
			if (abs(pts_0 - pts_1) > max_pts_gap) {
				RK_LOGI("drop unsynchronized frames: pts_0 %d and pts_1 %d.", pts_0, pts_1);
				for (i = 0; i < CAM_NUM; i++) {
					if (data[i]) {
						smart_door_vi_release_frame(&face_cfg->vi_cfg[i]);
					}
				}
				continue;
			}
		}

		if (s32Ret == RK_SUCCESS) {
			for (i = 0; i < CAM_NUM; i++) {
				memset(&size[i], 0x00, sizeof(size[i]));
#ifdef TDE_ROTATE
				s32Ret = vi_frame_tde_rotate(&face_cfg->vi_cfg[i], dstBlk[i], &data[i], &size[i]);
				if (s32Ret != RK_SUCCESS) {
					RK_LOGE("vi_frame_tde_rotate fail: %x", s32Ret);
					goto exit;
				}
#else
				size[i] = &face_cfg->vi_cfg[i].stViFrame.stVFrame.u64PrivateData;
#endif

#ifdef SAVE_FRAME
				//drop early frames then save
				if (loopCount >= 10 && loopCount < (10 + SAVE_FRAME_CNT_MAX)) {
					if (fp[i]) {
						fwrite(data[i], 1, size[i], fp[i]);
						fflush(fp[i]);
					}
				}
#endif
			}
		}

		loopCount++;

		for (i = 0; i < CAM_NUM; i++) {
			if (!data[i]) {
				RK_LOGI("Get NO frame from dev %d, face unlock it not ready.", i);
				face_cfg->frame_ready = RK_FALSE;
				break;
			} else {
				face_cfg->frame_ready = RK_TRUE;
			}
		}

		RK_LOGI("face unlock flow count: %d", loopCount);
		//face recognition flow
		face_cfg->frame_right = (char *)data[0];
		face_cfg->frame_left = (char *)data[1];

		s32Ret = face_unlock_flow(face_cfg);

		//release stream
		for (i = 0; i < CAM_NUM; i++) {
			if (data[i]) {
				smart_door_vi_release_frame(&face_cfg->vi_cfg[i]);
			}
		}

		if (s32Ret != RK_SUCCESS) {
			RK_LOGE("face_unlock_flow fail: %x", s32Ret);
			break;
		} else {
			//if a clear result, then exit
			if (face_cfg->detect_res.unlock_state == RESULT_NO_ID ||
			    face_cfg->detect_res.unlock_state == RESULT_COMPARE_FACE_OK ||
			    face_cfg->detect_res.unlock_state == RESULT_SAVE_FACE_OK ||
			    face_cfg->detect_res.unlock_state == RESULT_SAVE_FACE_FAIL ||
			    face_cfg->detect_res.unlock_state == RESULT_FACE_EXIST ||
			    face_cfg->detect_res.unlock_state == RESULT_REGISTER_FULL) {
				RK_LOGD("face unlock quit");
				break;
			}
		}
	}

exit:
#ifdef TDE_ROTATE
	for (i = 0; i < CAM_NUM; i++) {
		tde_free_dstBlk(dstBlk[i]);
	}
#endif

#ifdef SAVE_FRAME
	for (i = 0; i < CAM_NUM; i++) {
		if (fp[i])
			fclose(fp[i]);
	}
#endif
	return RK_NULL;
}

static RK_S32 destroy_vi(SMART_DOOR_VI_CFG *vi_cfg)
{
	return smart_door_destroy_vi(*vi_cfg);
}

static int smart_door_vi_face(SMART_DOOR_FACE_CFG *face_cfg)
{
	int ret = RK_FAILURE;
	pthread_t face_thread_id;
	RK_S32 channel_id = 0;
	int i;

#ifdef TDE_ROTATE
	RK_LOGI("Enable tde rotate.");
	ret = RK_TDE_Open();
	if (ret != RK_SUCCESS) {
		return RK_FAILURE;
	}
#endif

	for (i = 0; i < CAM_NUM; i++) {
		memset(&face_cfg->vi_cfg[i], 0, sizeof(SMART_DOOR_VI_CFG));

		init_vi(&face_cfg->vi_cfg[i], i, channel_id, SENSOR_WIDTH, SENSOR_HEIGHT);

		ret = create_vi(&face_cfg->vi_cfg[i]);
		if (ret != RK_SUCCESS) {
			RK_LOGE("create_vi fail:%d", ret);
			goto exit;
		}

		if (face_cfg->vi_cfg[i].bIfIspGroupInit) {
			ret = RK_MPI_VI_StartPipe(face_cfg->vi_cfg[i].pipeId);
			if (ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_StartPipe fail:%d pipe:%d", ret, face_cfg->vi_cfg[i].pipeId);
				goto exit;
			}
		}
	}

	pthread_create(&face_thread_id, 0, vi_get_stream_face, (void *)face_cfg);
	pthread_join(face_thread_id, NULL);

	for (i = 0; i < CAM_NUM; i++) {
		if (face_cfg->vi_cfg[i].bIfIspGroupInit) {
			ret = RK_MPI_VI_StopPipe(face_cfg->vi_cfg[i].pipeId);
			if (ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_StopPipe fail:%d pipe:%d", ret, face_cfg->vi_cfg[i].pipeId);
			}
		}

		ret = destroy_vi(&face_cfg->vi_cfg[i]);
		if (ret != RK_SUCCESS) {
			RK_LOGE("destroy_vi fail:%d", ret);
		}
	}

exit:
#ifdef TDE_ROTATE
	RK_TDE_Close();
#endif
	return ret;
}

int smart_door_face_init(void)
{
	int ret;

	smart_unlock = new Smart_unlock();
	memset(&face_cfg, 0x00, sizeof(SMART_DOOR_FACE_CFG));

#ifdef TDE_ROTATE
	face_cfg.origin_ir_h = SENSOR_WIDTH;
	face_cfg.origin_ir_w = SENSOR_HEIGHT;
	face_cfg.in_ir_h = SENSOR_WIDTH;
	face_cfg.in_ir_w = SENSOR_HEIGHT;
	face_cfg.in_speckle_h = SENSOR_WIDTH;
	face_cfg.in_speckle_w = SENSOR_HEIGHT;
#else
	face_cfg.origin_ir_h = SENSOR_HEIGHT;
	face_cfg.origin_ir_w = SENSOR_WIDTH;
	face_cfg.in_ir_h = SENSOR_HEIGHT;
	face_cfg.in_ir_w = SENSOR_WIDTH;
	face_cfg.in_speckle_h = SENSOR_HEIGHT;
	face_cfg.in_speckle_w = SENSOR_WIDTH;
#endif
	face_cfg.frame_ready = RK_FALSE;
	/* 1. model path, permission R required
	 * 2. face data path, permission RW required
	 * 3. depth calib data path, permission RW required
	 * 4. depth init data path, permission RW required
	 */
	smart_unlock->set_path(MODEL_PATH, FACE_DATA_PATH,
			       DEPTH_CALIB_PATH, DEPTH_INIT_DATA_PATH);

	ret = smart_unlock->unlock_init(face_cfg.origin_ir_h, face_cfg.origin_ir_w, face_cfg.in_ir_h,
					face_cfg.in_ir_w, face_cfg.in_speckle_h, face_cfg.in_speckle_w);
	if (ret) {
		RK_LOGE("unlock_init error:%d", ret);
	} else {
		RK_LOGI("unlock_init ok");
	}

	return ret;
}

void smart_door_face_deinit(void)
{
	if (smart_unlock)
		smart_unlock->unlock_deinit();

	delete (smart_unlock);
	smart_unlock = NULL;

	RK_LOGI("unlock_deinit ok");
}

void smart_door_face_delete_one(int id)
{
	if (!smart_unlock) {
		RK_LOGE("without unlock_init, error");
		return;
	}

	smart_unlock->delete_face(id);

	printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Face recognition result↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n");
	printf("\t\tDelete face ID=%d.\n", id);
	printf("↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Face recognition result↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n");
}

void smart_door_face_delete_all(void)
{
	int id = -1;

	if (!smart_unlock) {
		RK_LOGE("without unlock_init, error");
		return;
	}

	smart_unlock->delete_face(id);

	printf("\n↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓Face recognition result↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓\n");
	printf("\t\tDelete all face ID.\n");
	printf("↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Face recognition result↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑\n\n");
}

int smart_door_face_unlock_flow(int mode, int id)
{
	int ret = RK_FAILURE;
	if (!smart_unlock) {
		RK_LOGE("without unlock_init, error");
		return ret;
	}

	face_cfg.use_state = (face_use_e)mode;
	face_cfg.tar_id = id;

	ret = smart_door_vi_face(&face_cfg);
	return 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

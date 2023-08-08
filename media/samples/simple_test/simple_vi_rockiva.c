#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <sys/poll.h>
#include <time.h>
#include <unistd.h>

#include "sample_comm.h"

#ifdef ROCKIVA
#include "rockiva_ba_api.h"
#include "rockiva_det_api.h"
#include "rockiva_face_api.h"
#endif
#define MODEL_DATA_PATH "/userdata/rockiva_data" //模型的路径

#define OUT_DIR_PATH "./out"
#define OUT_FRAMES_PATH "/tmp"
#define OUT_CAPTURE_PATH "/tmp"
#define MAX_IMAGE_BUFFER_SIZE 5
#define PATH_MAX 260

static RK_S32 g_s32FrameCnt = -1;
RK_S32 s32chnlId = 0;
static bool quit = false;
FILE *file = NULL;
MB_POOL vencPoolInput;
RK_U32 u32BufferSize;
VENC_STREAM_S stFrame;
typedef struct {
	int index;
	int state; // 0:uninitial; 1:unused; 2:using
	RockIvaImage image;
} IvaImageBuf;

typedef struct {
	RockIvaHandle handle;
	RockIvaInitParam commonParams;
	RockIvaBaTaskParams baParams;
	RockIvaFaceTaskParams faceParams;
	IvaImageBuf imageBuf[MAX_IMAGE_BUFFER_SIZE];
	int hasImageBufAlloc;
} IvaAppContext;

static void sigterm_handler(int sig) {
	fprintf(stderr, "signal %d\n", sig);
	quit = true;
}

RK_U64 TEST_COMM_GetNowUs() {
	struct timespec time = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time);
	return (RK_U64)time.tv_sec * 1000000 + (RK_U64)time.tv_nsec / 1000; /* microseconds */
}

static void *GetVencBuffer(void *arg) {
	(void)arg;
	printf("========%s========\n", __func__);
	void *pData = RK_NULL;
	int loopCount = 0;
	int s32Ret;
	char jpeg_path[128];

	stFrame.pstPack = malloc(sizeof(VENC_PACK_S));

	while (!quit) {
		s32Ret = RK_MPI_VENC_GetStream(0, &stFrame, 2000);
		if (s32Ret == RK_SUCCESS) {
			memset(jpeg_path, 0, sizeof(jpeg_path));
			sprintf(jpeg_path, "/tmp/test_jpeg%d.jpeg", loopCount);
			file = fopen(jpeg_path, "w");
			if (file) {
				pData = RK_MPI_MB_Handle2VirAddr(stFrame.pstPack->pMbBlk);
				fwrite(pData, 1, stFrame.pstPack->u32Len, file);
				fflush(file);
				fclose(file);
			}
			RK_U64 nowUs = TEST_COMM_GetNowUs();

			RK_LOGD("chn:0, loopCount:%d enc->seq:%d wd:%d pts=%lld delay=%lldus\n",
			        loopCount, stFrame.u32Seq, stFrame.pstPack->u32Len,
			        stFrame.pstPack->u64PTS, nowUs - stFrame.pstPack->u64PTS);

			s32Ret = RK_MPI_VENC_ReleaseStream(0, &stFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VENC_ReleaseStream fail %x", s32Ret);
			}
			loopCount++;
		}
		usleep(10 * 1000);
	}
	free(stFrame.pstPack);
	return NULL;
}

static void *GetMediaBuffer0(void *arg) {
	IvaAppContext *iva_ctx = (IvaAppContext *)arg;
	(void)arg;
	printf("========%s========\n", __func__);
	int loopCount = 0;
	int s32Ret;
	RK_S32 waitTime = 330;
	int pipeId = 0;

	VIDEO_FRAME_INFO_S stViFrame;
	VI_CHN_STATUS_S stChnStatus;
	int fd;

	RockIvaImage *image = (RockIvaImage *)malloc(sizeof(RockIvaImage));

	while (!quit) {
		memset(image, 0, sizeof(RockIvaImage));
		s32Ret = RK_MPI_VI_GetChnFrame(pipeId, s32chnlId, &stViFrame, waitTime);
		if (s32Ret == RK_SUCCESS) {
			RK_U64 nowUs = TEST_COMM_GetNowUs();
			void *data = RK_MPI_MB_Handle2VirAddr(stViFrame.stVFrame.pMbBlk);
			fd = RK_MPI_MB_Handle2Fd(stViFrame.stVFrame.pMbBlk);

			RK_LOGD(
			    "RK_MPI_VI_GetChnFrame ok:data %p loop:%d seq:%d pts:%lld ms len=%llu",
			    data, loopCount, stViFrame.stVFrame.u32TimeRef,
			    stViFrame.stVFrame.u64PTS / 1000, stViFrame.stVFrame.u64PrivateData);

			// 6.get the channel status
			s32Ret = RK_MPI_VI_QueryChnStatus(pipeId, s32chnlId, &stChnStatus);
			RK_LOGD("RK_MPI_VI_QueryChnStatus ret %x, w:%d,h:%d,enable:%d,"
			        "current frame id:%d,input lost:%d,output lost:%d,"
			        "framerate:%d,vbfail:%d delay=%lldus",
			        s32Ret, stChnStatus.stSize.u32Width, stChnStatus.stSize.u32Height,
			        stChnStatus.bEnable, stChnStatus.u32CurFrameID,
			        stChnStatus.u32InputLostFrame, stChnStatus.u32OutputLostFrame,
			        stChnStatus.u32FrameRate, stChnStatus.u32VbFail,
			        nowUs - stViFrame.stVFrame.u64PTS);

			image->info.width = stChnStatus.stSize.u32Width;
			image->info.height = stChnStatus.stSize.u32Height;
			image->info.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
			image->dataAddr = NULL;
			image->dataPhyAddr = NULL;
			image->dataFd = fd;
			image->frameId = stChnStatus.u32CurFrameID;

			ROCKIVA_PushFrame(iva_ctx->handle, image, NULL);
			usleep(100000);

			// 7.release the frame
			s32Ret = RK_MPI_VI_ReleaseChnFrame(pipeId, s32chnlId, &stViFrame);
			if (s32Ret != RK_SUCCESS) {
				RK_LOGE("RK_MPI_VI_ReleaseChnFrame fail %x", s32Ret);
			}
			loopCount++;
		} else {
			RK_LOGE("RK_MPI_VI_GetChnFrame timeout %x", s32Ret);
		}

		if ((g_s32FrameCnt >= 0) && (loopCount > g_s32FrameCnt)) {
			quit = true;
			break;
		}
	}
	free(image);
	return NULL;
}

// demo板dev默认都是0，根据不同的channel 来选择不同的vi节点
int vi_dev_init() {
	printf("%s\n", __func__);
	int ret = 0;
	int devId = 0;
	int pipeId = devId;

	VI_DEV_ATTR_S stDevAttr;
	VI_DEV_BIND_PIPE_S stBindPipe;
	memset(&stDevAttr, 0, sizeof(stDevAttr));
	memset(&stBindPipe, 0, sizeof(stBindPipe));
	// 0. get dev config status
	ret = RK_MPI_VI_GetDevAttr(devId, &stDevAttr);
	if (ret == RK_ERR_VI_NOT_CONFIG) {
		// 0-1.config dev
		ret = RK_MPI_VI_SetDevAttr(devId, &stDevAttr);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_SetDevAttr %x\n", ret);
			return -1;
		}
	} else {
		printf("RK_MPI_VI_SetDevAttr already\n");
	}
	// 1.get dev enable status
	ret = RK_MPI_VI_GetDevIsEnable(devId);
	if (ret != RK_SUCCESS) {
		// 1-2.enable dev
		ret = RK_MPI_VI_EnableDev(devId);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_EnableDev %x\n", ret);
			return -1;
		}
		// 1-3.bind dev/pipe
		stBindPipe.u32Num = pipeId;
		stBindPipe.PipeId[0] = pipeId;
		ret = RK_MPI_VI_SetDevBindPipe(devId, &stBindPipe);
		if (ret != RK_SUCCESS) {
			printf("RK_MPI_VI_SetDevBindPipe %x\n", ret);
			return -1;
		}
	} else {
		printf("RK_MPI_VI_EnableDev already\n");
	}

	return 0;
}

int vi_chn_init(int channelId, int width, int height) {
	int ret;
	int buf_cnt = 2;
	// VI init
	VI_CHN_ATTR_S vi_chn_attr;
	memset(&vi_chn_attr, 0, sizeof(vi_chn_attr));
	vi_chn_attr.stIspOpt.u32BufCount = buf_cnt;
	vi_chn_attr.stIspOpt.enMemoryType =
	    VI_V4L2_MEMORY_TYPE_MMAP; // VI_V4L2_MEMORY_TYPE_DMABUF;//VI_V4L2_MEMORY_TYPE_MMAP;
	vi_chn_attr.stSize.u32Width = width;
	vi_chn_attr.stSize.u32Height = height;
	vi_chn_attr.enPixelFormat = RK_FMT_YUV420SP;
	vi_chn_attr.enCompressMode = COMPRESS_MODE_NONE; // COMPRESS_AFBC_16x16;
	vi_chn_attr.u32Depth = 2;
	ret = RK_MPI_VI_SetChnAttr(0, channelId, &vi_chn_attr);
	ret |= RK_MPI_VI_EnableChn(0, channelId);
	if (ret) {
		printf("ERROR: create VI error! ret=%d\n", ret);
		return ret;
	}

	return ret;
}

static RK_S32 test_venc_init(int chnId, int width, int height, RK_CODEC_ID_E enType) {
	printf("========%s========\n", __func__);
	VENC_RECV_PIC_PARAM_S stRecvParam;
	VENC_CHN_ATTR_S stAttr;
	VENC_CHN_PARAM_S stParam;
	memset(&stAttr, 0, sizeof(VENC_CHN_ATTR_S));
	memset(&stParam, 0, sizeof(VENC_CHN_PARAM_S));

	stAttr.stRcAttr.enRcMode =
	    VENC_RC_MODE_MJPEGFIXQP; // jpeg need VENC_RC_MODE_MJPEGFIXQP
	stAttr.stRcAttr.stH264Cbr.u32BitRate = 10 * 1024;
	stAttr.stRcAttr.stH264Cbr.u32Gop = 60;

	stAttr.stVencAttr.enType = enType;
	stAttr.stVencAttr.enPixelFormat = RK_FMT_YUV420SP;
	stAttr.stVencAttr.u32Profile = H264E_PROFILE_HIGH;
	stAttr.stVencAttr.u32PicWidth = width;
	stAttr.stVencAttr.u32PicHeight = height;
	stAttr.stVencAttr.u32VirWidth = width;
	stAttr.stVencAttr.u32VirHeight = height;
	stAttr.stVencAttr.u32StreamBufCnt = 5;
	stAttr.stVencAttr.u32BufSize = width * height * 3 / 2;
	stAttr.stVencAttr.enMirror = MIRROR_NONE;

	stAttr.stVencAttr.stAttrJpege.bSupportDCF = RK_FALSE;
	stAttr.stVencAttr.stAttrJpege.stMPFCfg.u8LargeThumbNailNum = 0;
	stAttr.stVencAttr.stAttrJpege.enReceiveMode = VENC_PIC_RECEIVE_SINGLE;

	RK_MPI_VENC_CreateChn(chnId, &stAttr);

	stParam.stFrameRate.bEnable = RK_FALSE;
	stParam.stFrameRate.s32SrcFrmRateNum = 25;
	stParam.stFrameRate.s32SrcFrmRateDen = 1;
	stParam.stFrameRate.s32DstFrmRateNum = 10;
	stParam.stFrameRate.s32DstFrmRateDen = 1;
	RK_MPI_VENC_SetChnParam(chnId, &stParam);

	VENC_JPEG_PARAM_S stJpegParam;
	memset(&stJpegParam, 0, sizeof(stJpegParam));
	stJpegParam.u32Qfactor = 77;
	RK_MPI_VENC_SetJpegParam(chnId, &stJpegParam);

	memset(&stRecvParam, 0, sizeof(VENC_RECV_PIC_PARAM_S));
	stRecvParam.s32RecvPicNum = -1;
	RK_MPI_VENC_StartRecvFrame(chnId, &stRecvParam);

	return 0;
}

void FrameReleaseCallback(const RockIvaReleaseFrames *releaseFrames, void *userdata) {
	// printf("*****************FrameReleaseCallback count=%d******************\n",
	// releaseFrames->count);
	for (int i = 0; i < releaseFrames->count; i++) {
		uint32_t frameId = releaseFrames->frames[i].frameId;
		printf("frameId = %d\n", frameId);
	}
}

void FaceDetResultCallback(const RockIvaFaceDetResult *result,
                           const RockIvaExecuteStatus status, void *userData) {
	// printf("FaceDetResultCallback frame %d face num: %d\n", result->frameId,
	// result->objNum);
	if (status == ROCKIVA_SUCCESS) {
	}
}

void FaceAnalyseResultCallback(const RockIvaFaceCapResult *result,
                               const RockIvaExecuteStatus status, void *userData) {
	printf("***************FaceAnalyseResultCallback face id=%d****************\n",
	       result->faceInfo.objId);

	if (status == ROCKIVA_SUCCESS) {
		printf(
		    "capture face id=%d box=(%d %d %d %d) attribute: gender=%d age=%d "
		    "eyeGlass=%d mask=%d beard=%d\n",
		    result->faceInfo.objId, result->faceInfo.faceRect.topLeft.x,
		    result->faceInfo.faceRect.topLeft.y, result->faceInfo.faceRect.bottomRight.x,
		    result->faceInfo.faceRect.bottomRight.y,
		    result->faceAnalyseInfo.faceAttr.gender, result->faceAnalyseInfo.faceAttr.age,
		    result->faceAnalyseInfo.faceAttr.eyeGlass,
		    result->faceAnalyseInfo.faceAttr.mask,
		    result->faceAnalyseInfo.faceAttr.beard);
		char save_path[PATH_MAX];
		memset(save_path, 0, PATH_MAX);
		snprintf(save_path, PATH_MAX, "%s/%d_%d_%d_%dx%d_mask%d_face.yuv",
		         OUT_CAPTURE_PATH, result->faceInfo.objId,
		         result->faceInfo.faceQuality.score, result->captureImage.frameId,
		         result->captureImage.info.width, result->captureImage.info.height,
		         result->faceAnalyseInfo.faceAttr.mask);
		// printf("save capture to %s\n", save_path);

		RK_U8 *pVirAddr = RK_NULL;
		VIDEO_FRAME_INFO_S stFrame;
		MB_BLK blk = RK_NULL;
		memset(&stFrame, 0, sizeof(VIDEO_FRAME_INFO_S));

		blk = RK_MPI_MB_GetMB(vencPoolInput, u32BufferSize, RK_TRUE);
		pVirAddr = RK_MPI_MB_Handle2VirAddr(blk);

		memcpy(pVirAddr, result->captureImage.dataAddr, u32BufferSize);

		RK_MPI_SYS_MmzFlushCache(blk, RK_FALSE);
		stFrame.stVFrame.pMbBlk = blk;
		stFrame.stVFrame.u32Width = result->captureImage.info.width;
		stFrame.stVFrame.u32Height = result->captureImage.info.height;
		stFrame.stVFrame.u32VirWidth = result->captureImage.info.width;
		stFrame.stVFrame.u32VirHeight = result->captureImage.info.height;
		stFrame.stVFrame.enPixelFormat = RK_FMT_YUV420SP;
		stFrame.stVFrame.u32FrameFlag |= 0;
		stFrame.stVFrame.enCompressMode = COMPRESS_MODE_NONE;
		RK_MPI_VENC_SendFrame(0, &stFrame, -1);
		RK_MPI_MB_ReleaseMB(blk);
	}
}

int InitIvaCommon(IvaAppContext *ctx) {
	printf("\n========%s========\n", __func__);
	snprintf(ctx->commonParams.modelPath, ROCKIVA_PATH_LENGTH, MODEL_DATA_PATH);

	RockIvaRetCode ret =
	    ROCKIVA_Init(&ctx->handle, ROCKIVA_MODE_VIDEO, &ctx->commonParams, ctx);
	if (ret != ROCKIVA_RET_SUCCESS) {
		printf("ROCKIVA_Init error %d\n", ret);
		return -1;
	}

	ROCKIVA_SetFrameReleaseCallback(ctx->handle, FrameReleaseCallback);

	return 0;
}

static int InitIvaFace(IvaAppContext *ctx) {
	printf("\n========%s========\n", __func__);

	RockIvaFaceTaskParams *faceParams = &ctx->faceParams;

	faceParams->faceTaskType.faceCaptureEnable = 1;
	faceParams->faceTaskType.faceAttributeEnable = 0;
	faceParams->faceTaskType.faceRecognizeEnable = 0;

	// 开启戴口罩人脸抓拍
	faceParams->faceCaptureRule.captureWithMask = 0;

	// 最优质量抓拍（人脸消失或超时选一张质量最高人脸）
	faceParams->faceCaptureRule.optType = ROCKIVA_FACE_OPT_BEST;
	// 超时时间，如果一个人脸超过设定时间还未消失也会触发抓拍
	faceParams->faceCaptureRule.optBestOverTime = 10000;

	// 快速抓拍（人脸质量超过设定阈值就触发抓拍）
	// faceParams->faceCaptureRule.optType = ROCKIVA_FACE_OPT_FAST;
	// faceParams->faceCaptureRule.faceQualityThrehold = 60;

	// 人脸过滤配置
	// 最低人脸质量分阈值，小于阈值将过滤
	faceParams->faceCaptureRule.qualityConfig.minScore = 50;
	// 遮挡阈值，小于阈值将过滤
	faceParams->faceCaptureRule.qualityConfig.minEyescore = 60;
	faceParams->faceCaptureRule.qualityConfig.minMouthScore = 0;

	// 抓拍小图配置
	faceParams->faceCaptureRule.faceCapacity.maxCaptureNum = 10;
	faceParams->faceCaptureRule.captureImageConfig.mode = 1;
	faceParams->faceCaptureRule.captureImageConfig.resizeMode = 1;
	faceParams->faceCaptureRule.captureImageConfig.imageInfo.width = 240;
	faceParams->faceCaptureRule.captureImageConfig.imageInfo.height = 320;
	faceParams->faceCaptureRule.captureImageConfig.imageInfo.format =
	    ROCKIVA_IMAGE_FORMAT_RGB888;
	faceParams->faceCaptureRule.captureImageConfig.alignWidth = 16;

	faceParams->faceCaptureRule.captureImageConfig.expand.up = 1;
	faceParams->faceCaptureRule.captureImageConfig.expand.down = 1;
	faceParams->faceCaptureRule.captureImageConfig.expand.left = 1;
	faceParams->faceCaptureRule.captureImageConfig.expand.right = 1;

	// 检测区域配置
	faceParams->faceCaptureRule.detectAreaEn = 0;
	faceParams->faceCaptureRule.detectArea.pointNum = 4;
	faceParams->faceCaptureRule.detectArea.points[0].x = 1000;
	faceParams->faceCaptureRule.detectArea.points[0].y = 1000;
	faceParams->faceCaptureRule.detectArea.points[1].x = 9000;
	faceParams->faceCaptureRule.detectArea.points[1].y = 1000;
	faceParams->faceCaptureRule.detectArea.points[2].x = 9000;
	faceParams->faceCaptureRule.detectArea.points[2].y = 9000;
	faceParams->faceCaptureRule.detectArea.points[3].x = 1000;
	faceParams->faceCaptureRule.detectArea.points[3].y = 9000;

	RockIvaFaceCallback callback;
	callback.detCallback = FaceDetResultCallback;
	callback.analyseCallback = FaceAnalyseResultCallback;
	RockIvaRetCode ret = ROCKIVA_FACE_Init(ctx->handle, &ctx->faceParams, callback);
	if (ret != ROCKIVA_RET_SUCCESS) {
		printf("ROCKIVA_FACE_Init error %d\n", ret);
		return -1;
	}

	return 0;
}

static RK_CHAR optstr[] = "?::w:h:c:I:";
static void print_usage(const RK_CHAR *name) {
	printf("usage example:\n");
	printf("\t%s -I 0 -w 1920 -h 1080 -o 1\n", name);
	printf("\t-w | --width: VI width, Default:1920\n");
	printf("\t-h | --heght: VI height, Default:1080\n");
	printf("\t-I | --camid: camera ctx id, Default 0. "
	       "0:rkisp_mainpath,1:rkisp_selfpath,2:rkisp_bypasspath\n");
}

//人脸识别抓拍保存jpeg
//使用前确保板子上已经有rockiva的库以及模型
int main(int argc, char *argv[]) {
	RK_S32 s32Ret = RK_FAILURE;
	RK_U32 u32Width = 1920;
	RK_U32 u32Height = 1080;
	RK_U32 snapshotW = 240;
	RK_U32 snapshotH = 320;
	int savefile = 0;
	int c;
	int ret = 0;

	MB_POOL_CONFIG_S stMbPoolCfg;

	while ((c = getopt(argc, argv, optstr)) != -1) {
		switch (c) {
		case 'w':
			u32Width = atoi(optarg);
			break;
		case 'h':
			u32Height = atoi(optarg);
			break;
		case 'I':
			s32chnlId = atoi(optarg);
			break;
		case '?':
		default:
			print_usage(argv[0]);
			return 0;
		}
	}

	printf("#Resolution: %dx%d\n", u32Width, u32Height);
	printf("#Output Path: %d\n", savefile);
	printf("#CameraIdx: %d\n\n", s32chnlId);

	signal(SIGINT, sigterm_handler);

	if (RK_MPI_SYS_Init() != RK_SUCCESS) {
		RK_LOGE("rk mpi sys init fail!");
		goto __FAILED;
	}

	vi_dev_init();
	vi_chn_init(s32chnlId, u32Width, u32Height);

	test_venc_init(s32chnlId, snapshotW, snapshotH, RK_VIDEO_ID_JPEG);

	u32BufferSize = snapshotW * snapshotH * 3 / 2;
	memset(&stMbPoolCfg, 0, sizeof(MB_POOL_CONFIG_S));
	stMbPoolCfg.u64MBSize = u32BufferSize;
	stMbPoolCfg.u32MBCnt = 3;
	stMbPoolCfg.enAllocType = MB_ALLOC_TYPE_DMA;
	stMbPoolCfg.bPreAlloc = RK_TRUE;
	vencPoolInput = RK_MPI_MB_CreatePool(&stMbPoolCfg);

	IvaAppContext iva_ctx;
	memset(&iva_ctx, 0, sizeof(IvaAppContext));

	ret = InitIvaCommon(&iva_ctx);
	if (ret != 0) {
		return -1;
	}

	ret = InitIvaFace(&iva_ctx);
	if (ret != 0) {
		return -1;
	}

	pthread_t main_thread;
	pthread_create(&main_thread, NULL, GetMediaBuffer0, &iva_ctx);

	pthread_t venc_thread;
	pthread_create(&venc_thread, NULL, GetVencBuffer, NULL);

	while (!quit) {
		usleep(50000);
	}
	pthread_join(&main_thread, NULL);
	pthread_join(&venc_thread, NULL);
	s32Ret = RK_MPI_VI_DisableChn(0, 0);
	RK_LOGE("RK_MPI_VI_DisableChn %x", s32Ret);
	s32Ret = RK_MPI_VI_DisableDev(0);
	RK_LOGE("RK_MPI_VI_DisableDev %x", s32Ret);
	ROCKIVA_Release(iva_ctx.handle);
__FAILED:
	RK_LOGE("test running exit:%d", s32Ret);
	RK_MPI_SYS_Exit();

	return 0;
}

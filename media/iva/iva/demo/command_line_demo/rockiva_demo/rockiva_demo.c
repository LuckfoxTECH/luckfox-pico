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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <linux/limits.h>

#include "rockiva_ba_api.h"
#include "rockiva_det_api.h"
#include "rockiva_face_api.h"
#include "rockiva_image.h"

#include "configs.h"

#include "utils/common_utils.h"
#include "utils/image_utils.h"
#include "utils/image_buffer.h"

#define MAX_IMAGE_BUFFER_SIZE 4

typedef struct
{
    RockIvaHandle handle;
    RockIvaInitParam commonParams;
    RockIvaBaTaskParams baParams;
    RockIvaFaceTaskParams faceParams;
    ImageBufferManager imgBufManager;
} IvaAppContext;

static void CreateDir(const char *path) {
    if (access(path, R_OK) != 0) {
        if (mkdir(path, 0755) == -1) {
            printf("create dir %s fail", path);
        } else {
            printf("create dir %s\n", path);
        }
    }
}

static void CreateOutDir() {
    if (access(OUT_DIR_PATH, R_OK) == 0) {
        rmdir(OUT_DIR_PATH);
    }
    CreateDir(OUT_DIR_PATH);
    CreateDir(OUT_FRAMES_PATH);
    CreateDir(OUT_CAPTURE_PATH);
}

void FrameReleaseCallback(const RockIvaReleaseFrames* releaseFrames, void* userdata)
{
    IvaAppContext* iva_ctx = (IvaAppContext*)userdata;
    printf("FrameReleaseCallback count=%d\n", releaseFrames->count);
    for (int i = 0; i < releaseFrames->count; i++) {
        IvaImageBuf* img_buf = releaseFrames->frames[i].extData;
        printf("release img_buf=%p\n", img_buf);
        if (img_buf != NULL) {
            RetureImageBuffer(iva_ctx->imgBufManager, img_buf);
        } else {
            printf("FrameReleaseCallback: can not find image buffer for frameId=%d\n", releaseFrames->frames[i].frameId);
        }
    }
}

void DetResultCallback(const RockIvaDetectResult* result, const RockIvaExecuteStatus status, void* userdata)
{
    IvaAppContext* iva_ctx = (IvaAppContext*)userdata;
    unsigned long cur_timestamp_ms = GetTimeStampMS();
    printf("%lu DetResultCallback frameId %d status=%d\n", cur_timestamp_ms, result->frameId, status);

    RockIvaImage* frame = &(result->frame);

    // draw result
    // 拷贝一份避免画框改写原图数据
    RockIvaImage drawImage;
    ROCKIVA_IMAGE_Clone(frame, &drawImage);

    for (int i = 0; i < result->objNum; i++) {
        RockIvaObjectInfo* obj = &result->objInfo[i];
        int obj_rect_x1 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.width, obj->rect.topLeft.x);
        int obj_rect_y1 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.height, obj->rect.topLeft.y);
        int obj_rect_x2 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.width, obj->rect.bottomRight.x);
        int obj_rect_y2 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.height, obj->rect.bottomRight.y);
        DrawRectangleRGB(drawImage.dataAddr, frame->info.width, frame->info.height,
            obj_rect_x1, obj_rect_y1, obj_rect_x2-obj_rect_x1+1, obj_rect_y2-obj_rect_y1+1, COLOR_BLUE, 2);
    }
    char save_path[PATH_MAX] = {0};
    snprintf(save_path, PATH_MAX, "%s/%04d.jpg", OUT_FRAMES_PATH, result->frameId);
    SaveImage(save_path, &(drawImage));
    printf("save detect result to: %s\n", save_path);

    ROCKIVA_IMAGE_Release(&drawImage);
}

static void GetObjectColor(const RockIvaObjectInfo *obj_info, uint8_t color[3]) {
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    if (obj_info->type == ROCKIVA_OBJECT_TYPE_PERSON) {
        color[0] = 255;
        color[1] = 255;
    } else if (obj_info->type == ROCKIVA_OBJECT_TYPE_VEHICLE) {
        color[1] = 255;
        color[2] = 255;
    } else {
        color[0] = 255;
        color[2] = 255;
    }
}

static void GetObjectTrigerColor(const RockIvaBaObjectInfo *ba_obj_info, uint8_t color[3]) {
    color[0] = 0;
    color[1] = 0;
    color[2] = 0;
    if (ba_obj_info->triggerRules != 0) {
        printf("object %d triger rule %d\n", ba_obj_info->objInfo.objId, ba_obj_info->triggerRules);
        color[0] = 255;
    } else {
        GetObjectColor(&ba_obj_info->objInfo, color);
    }
}

static int DrawBaRules(RockIvaImage *image, RockIvaBaTaskParams *params) {
    for (int i = 0; i < ROCKIVA_BA_MAX_RULE_NUM; i++) {
        RockIvaBaWireRule *rule = &(params->baRules.tripWireRule[i]);
        if (rule->ruleEnable == 1) {
            DrawLine(image, rule->line.head, rule->line.tail);
        }
    }
    for (int i = 0; i < ROCKIVA_BA_MAX_RULE_NUM; i++) {
        RockIvaBaAreaRule *rule = &(params->baRules.areaInBreakRule[i]);
        if (rule->ruleEnable == 1) {
            DrawArea(image, &(rule->area));
        }
    }
    for (int i = 0; i < ROCKIVA_BA_MAX_RULE_NUM; i++) {
        RockIvaBaAreaRule *rule = &(params->baRules.areaInRule[i]);
        if (rule->ruleEnable == 1) {
            DrawArea(image, &(rule->area));
        }
    }
    for (int i = 0; i < ROCKIVA_BA_MAX_RULE_NUM; i++) {
        RockIvaBaAreaRule *rule = &(params->baRules.areaOutRule[i]);
        if (rule->ruleEnable == 1) {
            DrawArea(image, &(rule->area));
        }
    }
    return 0;
}

void BaResultCallback(const RockIvaBaResult* result, const RockIvaExecuteStatus status, void* userdata)
{
    printf("BaResultCallback frame %d\n", result->frameId);
    IvaAppContext* iva_ctx = (IvaAppContext*)userdata;

    if (status == ROCKIVA_SUCCESS) {
        RockIvaImage* frame = &(result->frame);

        // draw result
        // TEST: 拷贝一份避免画框改写原图数据
        RockIvaImage drawImage;
        ROCKIVA_IMAGE_Clone(frame, &drawImage);

        for (int i = 0; i < result->objNum; i++) {
            uint8_t color[3];
            uint8_t text_color[3];
            char text[32];
            snprintf(text, 32, "%d - %d", result->triggerObjects[i].objInfo.objId, result->triggerObjects[i].objInfo.score);
            GetObjectColor(&(result->triggerObjects[i].objInfo), color);
            GetObjectTrigerColor(&(result->triggerObjects[i]), text_color);
            RockIvaObjectInfo* obj = &(result->triggerObjects[i].objInfo);
            int obj_rect_x1 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.width, obj->rect.topLeft.x);
            int obj_rect_y1 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.height, obj->rect.topLeft.y);
            int obj_rect_x2 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.width, obj->rect.bottomRight.x);
            int obj_rect_y2 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.height, obj->rect.bottomRight.y);
            DrawRectangleRGB(drawImage.dataAddr, frame->info.width, frame->info.height,
                obj_rect_x1, obj_rect_y1, obj_rect_x2-obj_rect_x1+1, obj_rect_y2-obj_rect_y1+1, color, 2);
            DrawTextRGB(drawImage.dataAddr, drawImage.info.width, drawImage.info.height, text, obj_rect_x1, obj_rect_y1-22, 10, text_color);
        }

        DrawBaRules(&drawImage, &iva_ctx->baParams);

        char out_img_path[PATH_MAX] = {0};
        snprintf(out_img_path, PATH_MAX, "%s/%d.jpg", OUT_FRAMES_PATH, result->frameId);
        printf("write img to %s\n", out_img_path);
        SaveImage(out_img_path, &drawImage);
    }
}

void FaceDetResultCallback(const RockIvaFaceDetResult* result, const RockIvaExecuteStatus status, void* userdata)
{
    IvaAppContext* iva_ctx = (IvaAppContext*)userdata;

    printf("FaceDetResultCallback frame %d face num: %d\n", result->frameId, result->objNum);

    if (status == ROCKIVA_SUCCESS) {
    }

    RockIvaImage* frame = &(result->frame);

    // draw result
    // 拷贝一份避免画框改写原图数据
    RockIvaImage drawImage;
    ROCKIVA_IMAGE_Clone(&result->frame, &drawImage);

    for (int i = 0; i < result->objNum; i++) {
        RockIvaFaceInfo* face = &result->faceInfo[i];
        int obj_rect_x1 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.width, face->faceRect.topLeft.x);
        int obj_rect_y1 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.height, face->faceRect.topLeft.y);
        int obj_rect_x2 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.width, face->faceRect.bottomRight.x);
        int obj_rect_y2 = ROCKIVA_RATIO_PIXEL_CONVERT(frame->info.height, face->faceRect.bottomRight.y);
        DrawRectangleRGB(drawImage.dataAddr, frame->info.width, frame->info.height,
            obj_rect_x1, obj_rect_y1, obj_rect_x2-obj_rect_x1+1, obj_rect_y2-obj_rect_y1+1, COLOR_BLUE, 2);
        char text[256] = {0};
        snprintf(text, 32, "%d-%d_%d_%d_%d_%d_%d_%d_%d_%d", face->objId, 
            face->faceQuality.score, face->faceQuality.faceScore, face->faceQuality.clarity,
            face->faceQuality.angle.pitch, face->faceQuality.angle.roll, face->faceQuality.angle.yaw,
            face->faceQuality.eyesScore, face->faceQuality.noseScore, face->faceQuality.mouthScore
        );
        DrawTextRGB(drawImage.dataAddr, frame->info.width, frame->info.height, text, obj_rect_x1, obj_rect_y1-22, 10, COLOR_BLUE);
    }
    char save_path[PATH_MAX] = {0};
    snprintf(save_path, PATH_MAX, "%s/%04d.jpg", OUT_FRAMES_PATH, result->frameId);
    printf("%lu before write image\n", GetTimeStampMS());
    SaveImage(save_path, &(drawImage));
    printf("%lu after write image\n", GetTimeStampMS());
    printf("save detect result to: %s\n", save_path);

    ROCKIVA_IMAGE_Release(&drawImage);
}

void FaceAnalyseResultCallback(const RockIvaFaceCapResult* result, const RockIvaExecuteStatus status, void* userdata)
{
    IvaAppContext* iva_ctx = (IvaAppContext*)userdata;
    printf("FaceAnalyseResultCallback type=%d face id=%d\n", result->faceCapFrameType, result->faceInfo.objId);

    if (status == ROCKIVA_SUCCESS) {
        printf("capture face id=%d box=(%d %d %d %d) mask=%d\n",
               result->faceInfo.objId, result->faceInfo.faceRect.topLeft.x, result->faceInfo.faceRect.topLeft.y,
               result->faceInfo.faceRect.bottomRight.x, result->faceInfo.faceRect.bottomRight.y,
               result->faceAnalyseInfo.faceAttr.mask);
        char faceId[ROCKIVA_FACE_INFO_SIZE_MAX] = {0};
        if (result->faceCapFrameType != ROCKIVA_FACE_CAP_TYPE_FORENOTICE) {
            char save_path[PATH_MAX];
            memset(save_path, 0, PATH_MAX);
            snprintf(save_path, PATH_MAX, "%s/%d_%d_%d_%d_%d_%d_quality%d_%d_%d_%d_%d_%d_%d_%d_%d_type%d_mask%d_%s_face.png", OUT_CAPTURE_PATH, result->faceInfo.objId, result->captureImage.frameId, 
                result->faceRectOnCaptureImage.topLeft.x, result->faceRectOnCaptureImage.topLeft.y, result->faceRectOnCaptureImage.bottomRight.x, result->faceRectOnCaptureImage.bottomRight.y, 
                result->faceInfo.faceQuality.score, result->faceInfo.faceQuality.faceScore, result->faceInfo.faceQuality.clarity,
                result->faceInfo.faceQuality.angle.pitch, result->faceInfo.faceQuality.angle.roll, result->faceInfo.faceQuality.angle.yaw,
                result->faceInfo.faceQuality.eyesScore, result->faceInfo.faceQuality.noseScore, result->faceInfo.faceQuality.mouthScore,
                result->faceCapFrameType, result->faceAnalyseInfo.faceAttr.mask, faceId);
            printf("save capture to %s\n", save_path);
            SaveImage(save_path, &(result->captureImage));
            printf("capture image data=%p size=%d size=%dx%d fmt=%d\n", result->captureImage.dataAddr, result->captureImage.size,
                result->captureImage.info.width, result->captureImage.info.height, result->captureImage.info.format);
        }
    }
}

static int InitIvaCommon(IvaAppContext* ctx)
{
    RockIvaInitParam* commonParams = &(ctx->commonParams);

    // 配置模型路径
    snprintf(commonParams->modelPath, ROCKIVA_PATH_LENGTH, MODEL_DATA_PATH);

    // 配置授权信息
    if (access(LICENSE_PATH, R_OK) == 0) {
        char *license_key;
        int license_size;
        license_size = ReadDataFile(LICENSE_PATH, &license_key);
        if (license_key != NULL && license_size > 0) {
            commonParams->license.memAddr = license_key;
            commonParams->license.memSize = license_size;
        }
    }

    // 宠物检测
    commonParams->detObjectType |= ROCKIVA_OBJECT_TYPE_PET;
    commonParams->detObjectType |= ROCKIVA_OBJECT_TYPE_FACE;
    commonParams->detObjectType |= ROCKIVA_OBJECT_TYPE_PERSON;

    RockIvaRetCode ret = ROCKIVA_Init(&ctx->handle, ROCKIVA_MODE_VIDEO, commonParams, ctx);
    if (ret != ROCKIVA_RET_SUCCESS) {
        printf("ROCKIVA_Init error %d\n", ret);
        return -1;
    }

    ROCKIVA_SetFrameReleaseCallback(ctx->handle, FrameReleaseCallback);

    if (commonParams->license.memAddr != NULL && commonParams->license.memSize > 0) {
        free(commonParams->license.memAddr);
    }

    return 0;
}

static int InitIvaDet(IvaAppContext* ctx)
{
    RockIvaRetCode ret = ROCKIVA_DETECT_Init(ctx->handle, DetResultCallback);
    if (ret != ROCKIVA_RET_SUCCESS) {
        printf("ROCKIVA_BA_Init error %d\n", ret);
        return -1;
    }
    return 0;
}

static int InitIvaBa(IvaAppContext* ctx)
{
    RockIvaBaTaskParams* baParams = &ctx->baParams;

    baParams->aiConfig.detectResultMode = 1;
    baParams->aiConfig.filterPersonMode = 1;

    // 构建一个区域入侵规则
    baParams->baRules.areaInBreakRule[0].ruleEnable = 1;
    baParams->baRules.areaInBreakRule[0].alertTime = 1000;  // 1000ms
    baParams->baRules.areaInBreakRule[0].event = ROCKIVA_BA_TRIP_EVENT_STAY;
    baParams->baRules.areaInBreakRule[0].ruleID = 1;
    baParams->baRules.areaInBreakRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
    baParams->baRules.areaInBreakRule[0].area.pointNum = 4;
    baParams->baRules.areaInBreakRule[0].area.points[0].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 900);
    baParams->baRules.areaInBreakRule[0].area.points[0].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 400);
    baParams->baRules.areaInBreakRule[0].area.points[1].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 600);
    baParams->baRules.areaInBreakRule[0].area.points[1].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 600);
    baParams->baRules.areaInBreakRule[0].area.points[2].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 1800);
    baParams->baRules.areaInBreakRule[0].area.points[2].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 600);
    baParams->baRules.areaInBreakRule[0].area.points[3].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 1500);
    baParams->baRules.areaInBreakRule[0].area.points[3].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 400);

    RockIvaRetCode ret = ROCKIVA_BA_Init(ctx->handle, &ctx->baParams, BaResultCallback);
    if (ret != ROCKIVA_RET_SUCCESS) {
        printf("ROCKIVA_BA_Init error %d\n", ret);
        return -1;
    }
    return 0;
}

static int InitIvaFace(IvaAppContext* ctx)
{
    RockIvaFaceTaskParams* faceParams = &ctx->faceParams;

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
    faceParams->faceCaptureRule.captureImageConfig.imageInfo.format = ROCKIVA_IMAGE_FORMAT_RGB888;
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

int main(int argc, char** argv)
{
    char* image_dir = argv[1];
    int image_num = atoi(argv[2]);
    printf("image dir: %s image_num: %d\n", image_dir, image_num);

    CreateOutDir();

    int ret = 0;

    IvaAppContext iva_ctx;
    memset(&iva_ctx, 0, sizeof(IvaAppContext));

    ret = InitIvaCommon(&iva_ctx);
    if (ret != 0) {
        return -1;
    }

    ret = InitIvaDet(&iva_ctx);
    if (ret != 0) {
        return -1;
    }

    // ret = InitIvaBa(&iva_ctx);
    // if (ret != 0) {
    //     return -1;
    // }

    // ret = InitIvaFace(&iva_ctx);
    // if (ret != 0) {
    //     return -1;
    // }

    char (*image_path_list)[IMAGE_PATH_MAX] = malloc(image_num*IMAGE_PATH_MAX);
    memset(image_path_list, 0, image_num*IMAGE_PATH_MAX);

    int real_num = GetImagePathList(image_dir, image_path_list, image_num);

    if (real_num <=0) {
        printf("can't get image from dir %d\n", image_dir);
        return -1;
    }

    int w = 0;
    int h = 0;
    ret = ReadImageInfo(image_path_list[0], &w, &h);
    if (ret !=0 || w <= 0 || h <= 0) {
        printf("get image info fail %d : %s\n", ret, image_path_list[0]);
        return -1;
    }

    iva_ctx.imgBufManager = CreateImageBufferManagerPreAlloc(3, w, h, 3, 1);

    for (int i = 0; i < real_num; i++) {
        printf("%d process image %s\n", i, image_path_list[i]);

        IvaImageBuf* img_buf = AcquireImageBuffer(iva_ctx.imgBufManager, 1000);

        printf("%lu before read image\n", GetTimeStampMS());
        ret = ReadImage(image_path_list[i], &img_buf->image);
        printf("%lu after read image\n", GetTimeStampMS());
        if (ret != 0) {
            printf("error get image fail!\n");
            goto next_frame;
        }

        img_buf->image.frameId = i;
        img_buf->image.extData = img_buf;
        printf("%lu PushFrame %d\n", GetTimeStampMS(), i);
        ret = ROCKIVA_PushFrame(iva_ctx.handle, &img_buf->image, NULL);

    next_frame:
        // about 10 fps
        // usleep(100 * 1000);
        continue;
    }
    free(image_path_list);

    ROCKIVA_Release(iva_ctx.handle);

    DestroyImageBufferManager(iva_ctx.imgBufManager);
    return 0;
}

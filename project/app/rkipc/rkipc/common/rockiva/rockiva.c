// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "rockiva.h"
#include "common.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "rockiva.c"

pthread_mutex_t g_rknn_list_mutex = PTHREAD_MUTEX_INITIALIZER;
RockIvaHandle rkba_handle;
RockIvaBaTaskParams initParams;
RockIvaInitParam globalParams;
int rockit_run_flag = 0;
rknn_list *rknn_list_;

void create_rknn_list(rknn_list **s) {
	pthread_mutex_lock(&g_rknn_list_mutex);
	if (*s != NULL) {
		pthread_mutex_unlock(&g_rknn_list_mutex);
		return;
	}
	*s = (rknn_list *)malloc(sizeof(rknn_list));
	(*s)->top = NULL;
	(*s)->size = 0;
	pthread_mutex_unlock(&g_rknn_list_mutex);
	printf("create rknn_list success\n");
}

void destory_rknn_list(rknn_list **s) {
	pthread_mutex_lock(&g_rknn_list_mutex);
	Node *t = NULL;
	if (*s == NULL) {
		pthread_mutex_unlock(&g_rknn_list_mutex);
		return;
	}
	while ((*s)->top) {
		t = (*s)->top;
		(*s)->top = t->next;
		free(t);
	}
	free(*s);
	*s = NULL;
	pthread_mutex_unlock(&g_rknn_list_mutex);
}

void rknn_list_push(rknn_list *s, long long timeval, RockIvaBaResult ba_result) {
	pthread_mutex_lock(&g_rknn_list_mutex);
	Node *t = NULL;
	t = (Node *)malloc(sizeof(Node));
	t->timeval = timeval;
	t->ba_result = ba_result;
	if (s->top == NULL) {
		s->top = t;
		t->next = NULL;
	} else {
		t->next = s->top;
		s->top = t;
	}
	s->size++;
	pthread_mutex_unlock(&g_rknn_list_mutex);
}

void rknn_list_pop(rknn_list *s, long long *timeval, RockIvaBaResult *ba_result) {
	pthread_mutex_lock(&g_rknn_list_mutex);
	Node *t = NULL;
	if (s == NULL || s->top == NULL) {
		pthread_mutex_unlock(&g_rknn_list_mutex);
		return;
	}
	t = s->top;
	*timeval = t->timeval;
	*ba_result = t->ba_result;
	s->top = t->next;
	free(t);
	s->size--;
	pthread_mutex_unlock(&g_rknn_list_mutex);
}

void rknn_list_drop(rknn_list *s) {
	pthread_mutex_lock(&g_rknn_list_mutex);
	Node *t = NULL;
	if (s == NULL || s->top == NULL) {
		pthread_mutex_unlock(&g_rknn_list_mutex);
		return;
	}
	t = s->top;
	s->top = t->next;
	free(t);
	s->size--;
	pthread_mutex_unlock(&g_rknn_list_mutex);
}

int rknn_list_size(rknn_list *s) {
	if (s == NULL)
		return -1;
	return s->size;
}

int rkipc_rknn_object_get(RockIvaBaResult *ba_result) {
	int ret = 0;
	long long time_before;
	if (rknn_list_size(rknn_list_)) {
		rknn_list_pop(rknn_list_, &time_before, ba_result);
		LOG_DEBUG("ba_result->objNum is %d\n", ba_result->objNum);
		ret = 0;
	} else {
		ret = -1; // no update
	}

	return ret;
}

void rkba_callback(const RockIvaBaResult *result, const RockIvaExecuteStatus status,
                   void *userData) {
	if (result->objNum == 0)
		return;

	// LOG_INFO("status is %d, frame %d, result->objNum is %d\n", status, result->frameId,
	//          result->objNum);
	rknn_list_push(rknn_list_, rkipc_get_curren_time_ms(), *result);
	int size = rknn_list_size(rknn_list_);
	if (size >= MAX_RKNN_LIST_NUM)
		rknn_list_drop(rknn_list_);
	LOG_DEBUG("size is %d\n", size);
	for (int i = 0; i < result->objNum; i++) {
		LOG_DEBUG("topLeft:[%d,%d], bottomRight:[%d,%d],"
		          "objId is %d, frameId is %d, score is %d, type is %d\n",
		          result->triggerObjects[i].objInfo.rect.topLeft.x,
		          result->triggerObjects[i].objInfo.rect.topLeft.y,
		          result->triggerObjects[i].objInfo.rect.bottomRight.x,
		          result->triggerObjects[i].objInfo.rect.bottomRight.y,
		          result->triggerObjects[i].objInfo.objId,
		          result->triggerObjects[i].objInfo.frameId,
		          result->triggerObjects[i].objInfo.score, result->triggerObjects[i].objInfo.type);
		// LOG_INFO("triggerRules is %d, ruleID is %d, triggerType is %d\n",
		//          result->triggerObjects[i].triggerRules,
		//          result->triggerObjects[i].firstTrigger.ruleID,
		//          result->triggerObjects[i].firstTrigger.triggerType);
	}

	// if (status == ROCKIVA_SUCCESS) {
	//     CachedImageMem *cached_image_mem = get_image_from_cache(result->frameId);
	//     if (cached_image_mem != nullptr) {
	//         RockIvaImage *image = cached_image_mem->img;
	//         for (int i = 0; i < result->objNum; i++) {
	//             if (result->objInfo[i].firstTrigger.ruleID != -1) {
	//                 capture_object(image, &(result->objInfo[i]));
	//             }
	//         }
	//         for (int i = 0; i < result->objNum; i++) {
	//             int color_r=0, color_g=0, color_b=0;
	//             int text_color_r=0, text_color_g=0, text_color_b=0;
	//             char text[32];
	//             snprintf(text, 32, "%d - %d", result->objInfo[i].objId,
	//             result->objInfo[i].confidence); get_object_color(&(result->objInfo[i]), &color_r,
	//             &color_g, &color_b); get_object_trigger_color(&(result->objInfo[i]),
	//             &text_color_r, &text_color_g, &text_color_b); draw_rect(image,
	//             result->objInfo[i].objRect, color_b, color_g, color_r); draw_text(image, text,
	//             result->objInfo[i].objRect, text_color_b, text_color_g, text_color_r);
	//         }

	//         draw_rule(image, &initParams);

	//         char out_img_path[PATH_MAX] = {0};
	//         snprintf(out_img_path, PATH_MAX, "%s/%d.jpg", OUT_FRAMES_PATH, result->frameId);
	//         printf("write img to %s\n", out_img_path);
	//         write_image(image, out_img_path);
	//         release_image(cached_image_mem);
	//     }
}

int rkipc_rockiva_init() {
	LOG_INFO("begin\n");
	RockIvaRetCode ret;
	const char *model_type;
	int rotation = rk_param_get_int("video.source:rotation", 0);
	// char *license_path = NULL;
	// char *license_key;
	// int license_size;

	memset(&initParams, 0, sizeof(initParams));
	memset(&globalParams, 0, sizeof(globalParams));

	// if (license_path != NULL) {
	//     license_size = read_data_file(license_path, &license_key);
	//     if (license_key != NULL && license_size > 0) {
	//         globalParams.license.memAddr = license_key;
	//         globalParams.license.memSize = license_size;
	//     }
	// }

	snprintf(globalParams.modelPath, ROCKIVA_PATH_LENGTH, "/usr/lib/");
	globalParams.coreMask = 0x04;
	globalParams.logLevel = ROCKIVA_LOG_ERROR;
	globalParams.detObjectType |= ROCKIVA_OBJECT_TYPE_FACE;
	globalParams.detObjectType |= ROCKIVA_OBJECT_TYPE_PERSON;
	model_type = rk_param_get_string("event.regional_invasion:rockiva_model_type", "small");
	if (!strcmp(model_type, "small") || !strcmp(model_type, "medium")) {
		globalParams.detObjectType |= ROCKIVA_OBJECT_TYPE_PET;
	} else if (!strcmp(model_type, "big")) {
		globalParams.detObjectType |= ROCKIVA_OBJECT_TYPE_NON_VEHICLE;
		globalParams.detObjectType |= ROCKIVA_OBJECT_TYPE_VEHICLE;
	}
	globalParams.imageInfo.width = rk_param_get_int("video.2:width", 960);
	globalParams.imageInfo.height = rk_param_get_int("video.2:height", 540);
	globalParams.imageInfo.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
	// temporary solution
	// which will be changed to reinitialize when the resolution is dynamically switched
	if (rotation == 0 || rotation == 180) {
		globalParams.imageInfo.transformMode = ROCKIVA_IMAGE_TRANSFORM_ROTATE_180;
	} else if (rotation == 90) {
		globalParams.imageInfo.transformMode = ROCKIVA_IMAGE_TRANSFORM_ROTATE_90;
	} else if (rotation == 270) {
		globalParams.imageInfo.transformMode = ROCKIVA_IMAGE_TRANSFORM_ROTATE_270;
	}

	ROCKIVA_Init(&rkba_handle, ROCKIVA_MODE_VIDEO, &globalParams, NULL);
	LOG_INFO("ROCKIVA_Init over\n");
#if 0
	// 构建一个进入区域规则
	initParams.baRules.areaInRule[0].ruleEnable = 1;
	initParams.baRules.areaInRule[0].alertTime = 2000; // 2000ms
	initParams.baRules.areaInRule[0].event = ROCKIVA_BA_TRIP_EVENT_IN;
	initParams.baRules.areaInRule[0].ruleID = 1;
	initParams.baRules.areaInRule[0].objType = ROCKIVA_BA_RULE_OBJ_PERSON;
	initParams.baRules.areaInRule[0].area.pointNum = 4;
	initParams.baRules.areaInRule[0].area.points[0].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 0);
	initParams.baRules.areaInRule[0].area.points[0].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 0);
	initParams.baRules.areaInRule[0].area.points[1].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 1920);
	initParams.baRules.areaInRule[0].area.points[1].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 0);
	initParams.baRules.areaInRule[0].area.points[2].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 1920);
	initParams.baRules.areaInRule[0].area.points[2].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 1080);
	initParams.baRules.areaInRule[0].area.points[3].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 0);
	initParams.baRules.areaInRule[0].area.points[3].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 1080);

	// 构建一个离开区域规则
	initParams.baRules.areaOutRule[0].ruleEnable = 1;
	initParams.baRules.areaOutRule[0].alertTime = 2000; // 2000ms
	initParams.baRules.areaOutRule[0].event = ROCKIVA_BA_TRIP_EVENT_OUT;
	initParams.baRules.areaOutRule[0].ruleID = 2;
	initParams.baRules.areaOutRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
	initParams.baRules.areaOutRule[0].area.pointNum = 4;
	initParams.baRules.areaOutRule[0].area.points[0].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 0);
	initParams.baRules.areaOutRule[0].area.points[0].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 0);
	initParams.baRules.areaOutRule[0].area.points[1].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 1920);
	initParams.baRules.areaOutRule[0].area.points[1].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 0);
	initParams.baRules.areaOutRule[0].area.points[2].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 1920);
	initParams.baRules.areaOutRule[0].area.points[2].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 1080);
	initParams.baRules.areaOutRule[0].area.points[3].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 0);
	initParams.baRules.areaOutRule[0].area.points[3].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 1080);

	// 构建一个越界规则
	initParams.baRules.tripWireRule[0].ruleEnable = 1;
	initParams.baRules.tripWireRule[0].event = ROCKIVA_BA_TRIP_EVENT_BOTH;
	initParams.baRules.tripWireRule[0].ruleID = 3;
	initParams.baRules.tripWireRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
	initParams.baRules.tripWireRule[0].line.head.x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 200);
	initParams.baRules.tripWireRule[0].line.head.y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 10);
	initParams.baRules.tripWireRule[0].line.tail.x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 200);
	initParams.baRules.tripWireRule[0].line.tail.y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 1000);

	// 构建一个区域入侵规则
	initParams.baRules.areaInBreakRule[0].ruleEnable = 1;
	initParams.baRules.areaInBreakRule[0].alertTime = 1000; // 1000ms
	initParams.baRules.areaInBreakRule[0].event = ROCKIVA_BA_TRIP_EVENT_STAY;
	initParams.baRules.areaInBreakRule[0].ruleID = 4;
	initParams.baRules.areaInBreakRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
	initParams.baRules.areaInBreakRule[0].area.pointNum = 4;
	initParams.baRules.areaInBreakRule[0].area.points[0].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 0);
	initParams.baRules.areaInBreakRule[0].area.points[0].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 0);
	initParams.baRules.areaInBreakRule[0].area.points[1].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(1920, 1920);
	initParams.baRules.areaInBreakRule[0].area.points[1].y = ROCKIVA_PIXEL_RATION_CONVERT(1080, 0);
	initParams.baRules.areaInBreakRule[0].area.points[2].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(1920, 1920);
	initParams.baRules.areaInBreakRule[0].area.points[2].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(1080, 1080);
	initParams.baRules.areaInBreakRule[0].area.points[3].x = ROCKIVA_PIXEL_RATION_CONVERT(1920, 0);
	initParams.baRules.areaInBreakRule[0].area.points[3].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(1080, 1080);
#else
	// 构建一个区域入侵规则
	int web_width = rk_param_get_int("osd.common:normalized_screen_width", 704);
	int web_height = rk_param_get_int("osd.common:normalized_screen_height", 480);
	int ri_x = rk_param_get_int("event.regional_invasion:position_x", 0);
	int ri_y = rk_param_get_int("event.regional_invasion:position_y", 0);
	int ri_w = rk_param_get_int("event.regional_invasion:width", 256);
	int ri_h = rk_param_get_int("event.regional_invasion:height", 256);

#if 1
	initParams.baRules.areaInBreakRule[0].ruleEnable =
	    rk_param_get_int("event.regional_invasion:enabled", 0);
	initParams.baRules.areaInBreakRule[0].sense =
	    rk_param_get_int("event.regional_invasion:sensitivity_level", 50); // [1, 100]
	initParams.baRules.areaInBreakRule[0].alertTime =
	    rk_param_get_int("event.regional_invasion:time_threshold", 1) * 1000; // ms
	initParams.baRules.areaInBreakRule[0].minObjSize[2].height =
	    web_height / 100 * rk_param_get_int("event.regional_invasion:proportion", 5);
	initParams.baRules.areaInBreakRule[0].minObjSize[2].width =
	    web_width / 100 * rk_param_get_int("event.regional_invasion:proportion", 5);
	initParams.baRules.areaInBreakRule[0].event = ROCKIVA_BA_TRIP_EVENT_STAY;
	initParams.baRules.areaInBreakRule[0].ruleID = 0;
	initParams.baRules.areaInBreakRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
	initParams.baRules.areaInBreakRule[0].area.pointNum = 4;
	initParams.baRules.areaInBreakRule[0].area.points[0].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x);
	initParams.baRules.areaInBreakRule[0].area.points[0].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y);
	initParams.baRules.areaInBreakRule[0].area.points[1].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x + ri_w);
	initParams.baRules.areaInBreakRule[0].area.points[1].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y);
	initParams.baRules.areaInBreakRule[0].area.points[2].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x + ri_w);
	initParams.baRules.areaInBreakRule[0].area.points[2].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y + ri_h);
	initParams.baRules.areaInBreakRule[0].area.points[3].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x);
	initParams.baRules.areaInBreakRule[0].area.points[3].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y + ri_h);
	LOG_INFO("(%d,%d), (%d,%d), (%d,%d), (%d,%d)\n",
	         initParams.baRules.areaInBreakRule[0].area.points[0].x,
	         initParams.baRules.areaInBreakRule[0].area.points[0].y,
	         initParams.baRules.areaInBreakRule[0].area.points[1].x,
	         initParams.baRules.areaInBreakRule[0].area.points[1].y,
	         initParams.baRules.areaInBreakRule[0].area.points[2].x,
	         initParams.baRules.areaInBreakRule[0].area.points[2].y,
	         initParams.baRules.areaInBreakRule[0].area.points[3].x,
	         initParams.baRules.areaInBreakRule[0].area.points[3].y);
	initParams.aiConfig.detectResultMode = 0;
#else
	initParams.baRules.areaInBreakRule[0].ruleEnable =
	    rk_param_get_int("event.regional_invasion:enabled", 0);
	initParams.baRules.areaInBreakRule[0].sense =
	    rk_param_get_int("event.regional_invasion:sensitivity_level", 50); // [1, 100]
	initParams.baRules.areaInBreakRule[0].alertTime =
	    rk_param_get_int("event.regional_invasion:time_threshold", 1) * 1000; // ms
	initParams.baRules.areaInBreakRule[0].minObjSize[2].height =
	    web_height / 100 * rk_param_get_int("event.regional_invasion:proportion", 5);
	initParams.baRules.areaInBreakRule[0].minObjSize[2].width =
	    web_width / 100 * rk_param_get_int("event.regional_invasion:proportion", 5);
	initParams.baRules.areaInBreakRule[0].event = ROCKIVA_BA_TRIP_EVENT_STAY;
	initParams.baRules.areaInBreakRule[0].ruleID = 0;
	initParams.baRules.areaInBreakRule[0].objType = ROCKIVA_BA_RULE_OBJ_FULL;
	initParams.baRules.areaInBreakRule[0].area.pointNum = 4;
	initParams.baRules.areaInBreakRule[0].area.points[0].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x);
	initParams.baRules.areaInBreakRule[0].area.points[0].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y);
	initParams.baRules.areaInBreakRule[0].area.points[1].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x + ri_w);
	initParams.baRules.areaInBreakRule[0].area.points[1].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y);
	initParams.baRules.areaInBreakRule[0].area.points[2].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x + ri_w);
	initParams.baRules.areaInBreakRule[0].area.points[2].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y + ri_h);
	initParams.baRules.areaInBreakRule[0].area.points[3].x =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_width, ri_x);
	initParams.baRules.areaInBreakRule[0].area.points[3].y =
	    ROCKIVA_PIXEL_RATION_CONVERT(web_height, ri_y + ri_h);
	LOG_INFO("(%d,%d), (%d,%d), (%d,%d), (%d,%d)\n",
	         initParams.baRules.areaInBreakRule[0].area.points[0].x,
	         initParams.baRules.areaInBreakRule[0].area.points[0].y,
	         initParams.baRules.areaInBreakRule[0].area.points[1].x,
	         initParams.baRules.areaInBreakRule[0].area.points[1].y,
	         initParams.baRules.areaInBreakRule[0].area.points[2].x,
	         initParams.baRules.areaInBreakRule[0].area.points[2].y,
	         initParams.baRules.areaInBreakRule[0].area.points[3].x,
	         initParams.baRules.areaInBreakRule[0].area.points[3].y);
	initParams.aiConfig.detectResultMode = 1; // 上报没有触发规则的检测目标, 临时调试用
#endif
#endif
	ret = ROCKIVA_BA_Init(rkba_handle, &initParams, rkba_callback);
	if (ret != ROCKIVA_RET_SUCCESS) {
		printf("ROCKIVA_BA_Init error %d\n", ret);
		return -1;
	}
	LOG_INFO("ROCKIVA_BA_Init success\n");
	create_rknn_list(&rknn_list_);
	rockit_run_flag = 1;
	LOG_INFO("end\n");

	return ret;
}

int rkipc_rockiva_deinit() {
	LOG_INFO("begin\n");
	rockit_run_flag = 0;
	ROCKIVA_BA_Release(rkba_handle);
	LOG_INFO("ROCKIVA_BA_Release over\n");
	ROCKIVA_Release(rkba_handle);
	destory_rknn_list(&rknn_list_);
	LOG_INFO("end\n");

	return 0;
}

int rkipc_rockiva_write_rgb888_frame(uint16_t width, uint16_t height, uint32_t frame_id,
                                     unsigned char *buffer) {
	int ret;
	RockIvaImage *image = (RockIvaImage *)malloc(sizeof(RockIvaImage));
	if (!rockit_run_flag)
		return 0;
	memset(image, 0, sizeof(RockIvaImage));
	image->info.width = width;
	image->info.height = height;
	image->info.format = ROCKIVA_IMAGE_FORMAT_BGR888;
	image->dataAddr = buffer;
	image->frameId = frame_id;
	ret = ROCKIVA_PushFrame(rkba_handle, image, NULL);
	free(image);

	return ret;
}

int rkipc_rockiva_write_rgb888_frame_by_fd(uint16_t width, uint16_t height, uint32_t frame_id,
                                           int32_t fd) {
	int ret;
	RockIvaImage *image = (RockIvaImage *)malloc(sizeof(RockIvaImage));
	if (!rockit_run_flag)
		return 0;
	memset(image, 0, sizeof(RockIvaImage));
	image->info.width = width;
	image->info.height = height;
	image->info.format = ROCKIVA_IMAGE_FORMAT_BGR888;
	image->frameId = frame_id;
	image->dataAddr = NULL;
	image->dataPhyAddr = NULL;
	image->dataFd = fd;
	ret = ROCKIVA_PushFrame(rkba_handle, image, NULL);
	free(image);

	return ret;
}

int rkipc_rockiva_write_nv12_frame_by_fd(uint16_t width, uint16_t height, uint32_t frame_id,
                                         int32_t fd) {
	int ret;
	RockIvaImage *image = (RockIvaImage *)malloc(sizeof(RockIvaImage));
	if (!rockit_run_flag)
		return 0;
	int rotation = rk_param_get_int("video.source:rotation", 0);
	memset(image, 0, sizeof(RockIvaImage));
	if (rotation == 0) {
		image->info.transformMode = ROCKIVA_IMAGE_TRANSFORM_NONE;
	} else if (rotation == 90) {
		image->info.transformMode = ROCKIVA_IMAGE_TRANSFORM_ROTATE_90;
	} else if (rotation == 180) {
		image->info.transformMode = ROCKIVA_IMAGE_TRANSFORM_ROTATE_180;
	} else if (rotation == 270) {
		image->info.transformMode = ROCKIVA_IMAGE_TRANSFORM_ROTATE_270;
	}
	image->info.width = width;
	image->info.height = height;
	image->info.format = ROCKIVA_IMAGE_FORMAT_YUV420SP_NV12;
	image->frameId = frame_id;
	image->dataAddr = NULL;
	image->dataPhyAddr = NULL;
	image->dataFd = fd;
	ret = ROCKIVA_PushFrame(rkba_handle, image, NULL);
	free(image);

	return ret;
}

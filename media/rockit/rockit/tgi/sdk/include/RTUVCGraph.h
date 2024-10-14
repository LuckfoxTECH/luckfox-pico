/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *     author: <rimon.xu@rock-chips.com> and <martin.cheng@rock-chips.com>
 *       date: 2020-04-03
 *     module: RTUVCGraph
 */

#ifndef SRC_RT_TASK_APP_GRAPH_RTUVCGRAPH_H_
#define SRC_RT_TASK_APP_GRAPH_RTUVCGRAPH_H_

#include "rt_header.h"
#include "RTTaskGraph.h"

#define NN_MODE_ST_ASTERIA              "st_asteria"
#define NN_MODE_ROCKX                   "rockx"

#define RT_AI_TYPE_FACE_DETECT          "face"
#define RT_AI_TYPE_FACE_LANDMARK        "face_landmark"
#define RT_AI_TYPE_FACE_ATTRIBUTE       "face_attribute"
#define RT_AI_TYPE_FACE_FEATURE         "face_feature"
#define RT_AI_TYPE_FACE_DISTANCE        "face_distance"
#define RT_AI_TYPE_BODY                 "body"
#define RT_AI_TYPE_HAND                 "hand"
#define RT_AI_TYPE_HAND_LANDMARK        "hand_landmark"

#define RT_UVC_MODE_EPTZ                "eptz"
#define RT_UVC_MODE_UVC                 "uvc"
#define RT_UVC_MODE_EPTZ_ZOOM           "eptz_zoom"
#define RT_UVC_MODE_UVC_ZOOM            "uvc_zoom"

typedef enum _RT_EPTZ_MODE {
    RT_EPTZ_PAN  = 0,
    RT_EPTZ_TILT = 1,
    RT_EPTZ_ROLL = 2,  // no support now
    RT_EPTZ_AUTO = 3,
} RT_EPTZ_MODE;

class RTUVCGraph {
 public:
    explicit RTUVCGraph(const char* tagName);
    ~RTUVCGraph();

    RT_RET prepare();
    RT_RET start();
    RT_RET stop();

    RT_RET observeUVCOutputStream(std::function<RT_RET(RTMediaBuffer *)> streamCallback);
    RT_RET observeNNOutputStream(std::function<RT_RET(RTMediaBuffer *)> streamCallback);
    RT_RET observeMattingOutputStream(std::function<RT_RET(RTMediaBuffer *)> streamCallback);
    RT_RET updateCameraParams(RtMetaData *params);
    RT_RET updateEncoderParams(RtMetaData *params);
    RT_RET updateNNParams(RtMetaData *params);
    RT_RET setCameraParams();

    RT_RET enableEPTZ(RT_BOOL enableEPTZ);
    RT_RET setZoom(float val);
    RT_RET setEptz(RT_EPTZ_MODE mode, int val);
    RT_RET openUVC();
    RT_RET closeUVC();
    RT_RET enableAIAlgorithm(std::string type);
    RT_RET disableAIAlgorithm(std::string type);

    RT_RET setAIAlgorithmHandle(void *handle);

    RT_RET openAIMatting();
    RT_RET closeAIMatting();

    RT_RET waitUntilDone();

    void *getCtx() { return mCtx; }
    RT_RET selectLinkMode();
    RT_RET preload(RtMetaData *meta);
    RT_RET invoke(INT32 cmd, void *data);

 private:
    RT_RET initialize();
    RT_RET deinitialize();

    RT_RET openAI();
    RT_RET closeAI();

    RT_RET setupGraphAndWaitDone();
    INT32  getDetectionByType(std::string type);
    std::string getAIAlgorithmType(std::string type);
    RT_RET updateAIAlgorithm();
    RT_RET setAIAlgorithm(RT_BOOL enable, std::string type);
    RT_RET setZoomPtz(RT_BOOL isZoom);

    static void* threadLoop(void* arg);

 private:
    void *mCtx;
    std::function<RT_RET(RTMediaBuffer *)> mNNCallback;
    std::function<RT_RET(RTMediaBuffer *)> mUVCCallback;
    std::function<RT_RET(RTMediaBuffer *)> mMattingCallback;
    std::map<std::string, RT_BOOL>         mVendorDetections;
};

#endif  // SRC_RT_TASK_APP_GRAPH_RTUVCGRAPH_H_

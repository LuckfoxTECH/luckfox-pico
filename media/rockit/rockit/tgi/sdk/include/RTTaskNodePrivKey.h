/*
 * Copyright 2021 Rockchip Electronics Co. LTD
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
 */

#ifndef SRC_RT_TASK_TASK_NODE_RTTASKNODEPRIVKEY_H_
#define SRC_RT_TASK_TASK_NODE_RTTASKNODEPRIVKEY_H_

#include "rt_common.h"
#include "RTTaskNodeDef.h"

#define KEY_COMM_BASE           RT_MERGE_INT64(kStubNone, 0)
#define KEY_COMM_DST_CROP       RT_MERGE_INT64(kStubNone, 1)
#define KEY_COMM_GRAPH_UID      RT_MERGE_INT64(kStubNone, 2)
#define KEY_COMM_EN_CROP        RT_MERGE_INT64(kStubNone, 3)
#define KEY_COMM_USER_PIC       RT_MERGE_INT64(kStubNone, 4)

#define KEY_EPTZ_BASE           RT_MERGE_INT64(kStubFilterEptz, 0)
#define KEY_EPTZ_INFO           RT_MERGE_INT64(kStubFilterEptz, 1)

#define KEY_VDEC_BASE           RT_MERGE_INT64(kStubCodecMppDecoder, 0)
#define KEY_VDEC_I4O2           RT_MERGE_INT64(kStubCodecMppDecoder, 1)

#define KEY_VENC_BASE           RT_MERGE_INT64(kStubCodecMppEncoder, 0)
#define KEY_VENC_NALU_TYPE      RT_MERGE_INT64(kStubCodecMppEncoder, 1)
#define KEY_VENC_TEMPORAL_ID    RT_MERGE_INT64(kStubCodecMppEncoder, 2)
#define KEY_VENC_RC_INFO        RT_MERGE_INT64(kStubCodecMppEncoder, 3)
#define KEY_VENC_CONFIG         RT_MERGE_INT64(kStubCodecMppEncoder, 4)
#define KEY_VENC_USER_DATA      RT_MERGE_INT64(kStubCodecMppEncoder, 5)
#define KEY_VENC_FRAME_INFO     RT_MERGE_INT64(kStubCodecMppEncoder, 6)

#define KEY_VI_BASE             RT_MERGE_INT64(kStubDeviceV4L2, 0)
#define KEY_VI_BUF_STATE        RT_MERGE_INT64(kStubDeviceV4L2, 1)

#define KEY_DMUX_BASE           RT_MERGE_INT64(kStubDemuxerTMedia, 0)
#define KEY_DMUX_STRM_IDX       RT_MERGE_INT64(kStubDemuxerTMedia, 1)

#define KEY_VPSS_BASE           RT_MERGE_INT64(kStubfilterVpss, 0)
#define KEY_VPSS_GRP_ATTR       RT_MERGE_INT64(kStubfilterVpss, 1)
#define KEY_VPSS_CHN_ATTR       RT_MERGE_INT64(kStubfilterVpss, 2)
#define KEY_VPSS_CHN_ID         RT_MERGE_INT64(kStubfilterVpss, 3)
#define KEY_VPSS_CHN_ENABLE     RT_MERGE_INT64(kStubfilterVpss, 4)
#define KEY_VPSS_BINDING        RT_MERGE_INT64(kStubfilterVpss, 5)
#define KEY_VPSS_MAX_CHN_NUM    RT_MERGE_INT64(kStubfilterVpss, 6)

#endif  // SRC_RT_TASK_TASK_NODE_RTTASKNODEPRIVKEY_H_

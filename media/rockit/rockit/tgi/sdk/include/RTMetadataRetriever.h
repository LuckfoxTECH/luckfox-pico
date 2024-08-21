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
 */

#ifndef SRC_RT_PLAYER_RTMETADATARETRIEVER_H_
#define SRC_RT_PLAYER_RTMETADATARETRIEVER_H_


#include "rt_error.h"         // NOLINT
#include "rt_metadata.h"      // NOLINT

class RTMediaBuffer;
class RTMediaExtractor;

class RTMetadataRetriever {
 public:
    RTMetadataRetriever();
    virtual ~RTMetadataRetriever();
    virtual RT_RET       setDataSource(const char *url, const char *headers);
    virtual RT_RET       setDataSource(int fd, int64_t offset, int64_t length);
    virtual RT_RET       query(RtMetaData* metaData);
    virtual RT_RET       getFrameAtTime(RtMetaData* metaData);
    RTMediaBuffer*       getSingleFrameAtTime(RtMetaData* metaData);

 private:
    RTMediaExtractor* mExtractor;
};

#endif  // SRC_RT_PLAYER_RTMETADATARETRIEVER_H_

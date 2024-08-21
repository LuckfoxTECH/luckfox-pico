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

#ifndef SRC_RT_MEDIA_INCLUDE_RTAVSHELLBUFFER_H_
#define SRC_RT_MEDIA_INCLUDE_RTAVSHELLBUFFER_H_

#include "RTMediaBuffer.h"

class RTAVShellBuffer : public RTMediaBuffer {
 public:
    RTAVShellBuffer();
    explicit RTAVShellBuffer(void* data, UINT32 size);
    explicit RTAVShellBuffer(UINT32 size);
    explicit RTAVShellBuffer(RTMediaBuffer *buffer);
    virtual ~RTAVShellBuffer();

    virtual void   setExtMediaBuffer(RTMediaBuffer *buffer);
    virtual RTMediaBuffer* getExtMediaBuffer();
    virtual void   release(bool debug = false);
    virtual void   signalBufferAvailable();
    virtual void*  getData() const;
    virtual UINT32 getSize() const;
    virtual UINT32 getRealSize() const;
    virtual UINT32 getOffset() const;
    virtual UINT32 getLength() const;
    virtual INT32  getFd() const;
    virtual INT32  getHandle() const;
    virtual UINT32 getPhyAddr() const;
    virtual INT32  getUniqueID() const;
    virtual INT32  getBufferSeq() const;
    virtual INT32  getPoolID() const;
    virtual RT_BOOL hasFlag(INT32 flag) const;

    virtual void   setData(void* data, UINT32 size);
    virtual void   setListener(RTBufferListener* listener);
    virtual void   setPhyAddr(UINT32 phyaddr);
    virtual void   setRange(UINT32 offset, UINT32 length);
    virtual void   setRealSize(UINT32 real);
    virtual void   setStatus(RtMediaBufferStatus status);
    virtual void   setSite(RTMediaBufferSite site);
    virtual void   setUserData(void *data, UserDataFree freeFunc = RT_NULL);
    virtual void   setBufferID(void *id);
    virtual void   setBufferSeq(INT32 seq);
    virtual void   setFd(INT32 fd);
    virtual void   setHandle(INT32 handle);
    virtual void   setUniqueID(INT32 uniqueId);
    virtual void   setPoolID(INT32 poolId);
    virtual void   setAllocator(RTAllocator *allocator);
    virtual void   setFlag(INT32 flag, INT32 value);

    virtual RtMetaData* getMetaData();

 protected:
    RTMediaBuffer *mExtBuffer;
};

#endif   // SRC_RT_MEDIA_INCLUDE_RTAVSHELLBUFFER_H_

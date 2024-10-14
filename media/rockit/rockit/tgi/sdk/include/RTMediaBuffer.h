/*
 * Copyright 2019 Rockchip Electronics Co. LTD
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
 * author: martin.cheng@rock-chips.com
 *   date: 2019/01/04
 * module: RTMediaBuffer
 * author: martin.cheng@rock-chips.com
 *   date: 2019/11/01
 * module: add RTBufferListener.
 */

#ifndef SRC_RT_MEDIA_INCLUDE_RTMEDIABUFFER_H_
#define SRC_RT_MEDIA_INCLUDE_RTMEDIABUFFER_H_

#include <map>

#include "rt_mutex.h"
#include "rt_metadata.h"
#include "RTObject.h"
#include "RTMediaData.h"
#include "RTMediaBufferDef.h"

class RTBufferListener {
 public:
    RTBufferListener() {}
    virtual ~RTBufferListener() {}

 public:
    // buffer callback.
    virtual void onBufferAvailable(void* buffer) = 0;
    virtual void onBufferRealloc(void* buffer, UINT32 size) = 0;
    virtual void onBufferRegister(UINT64 poolCapacity, UINT32 size) {}
    virtual void onBufferRelease(void* buffer, RT_BOOL render) = 0;
};

class RTAllocator;
class RTMediaBuffer : public RTObject {
 public:
    // The underlying data remains the responsibility of the caller!
    explicit RTMediaBuffer(void* data, UINT32 size);
    explicit RTMediaBuffer(
                 void* data,
                 UINT32 size,
                 INT32 handle,
                 INT32 fd,
                 RTAllocator *alloctor = RT_NULL);
    explicit RTMediaBuffer(UINT32 size);
    explicit RTMediaBuffer(const RTMediaBuffer* data);
    virtual ~RTMediaBuffer();
    RTMediaBuffer& operator = (const RTMediaBuffer &);

 public:
    // override RTObject methods
    void  summary(INT32 fd) { }
    const char* getName() {return "mediabuffer";}
    virtual RTMediaBufferType getType();
    virtual RT_BOOL checkType(RTMediaBufferType type);

 public:
    virtual void   release(bool debug = false);
    virtual void   signalBufferAvailable();
    virtual void   signalBufferRealloc(UINT32 size);
    virtual void   signalBufferRelease(bool render = false);
    virtual void   signalBufferRegister(UINT64 poolCapacity, UINT32 size);

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
    virtual UINT64 getModID() const;
    virtual void*  getUserData() const;

    virtual void   setData(void* data, UINT32 size);
    virtual void   setListener(RTBufferListener* listener);
    virtual void   setPhyAddr(UINT32 phyaddr);
    virtual void   setRange(UINT32 offset, UINT32 length);
    virtual void   setRealSize(UINT32 real) { mRealSize = real;}
    virtual void   setStatus(RtMediaBufferStatus status);
    virtual void   setSite(RTMediaBufferSite site);
    virtual void   setBufferID(void *id);
    virtual void   setModID(UINT64 modId);
    virtual void   setBufferSeq(INT32 seq);
    virtual void   setFd(INT32 fd);
    virtual void   setHandle(INT32 handle);
    virtual void   setUniqueID(INT32 uniqueId);
    virtual void   setPoolID(INT32 poolId);
    virtual void   setAllocator(RTAllocator *allocator);
    virtual void   setUserData(void *param, UserDataFree freeFunc = RT_NULL);

    virtual RT_BOOL hasFlag(INT32 flag) const;
    virtual void    setFlag(INT32 flag, INT32 value);
    virtual RtMetaData* getMetaData();

    RTAllocator *       getAllocator() { return mAllocator; }
    RtMediaBufferStatus getStatus();
    RTMediaBufferSite   getSite();
    RtMetaData*         extraMeta(const INT32 id);
    void                setExtraMeta(RtMetaData *meta, const INT32 id);
    RT_BOOL             hasMeta(const INT32 id) {
        return mExtraMetas.find(id) != mExtraMetas.end();
    }
    void*               getBufferID();

    void                setRegistered(RT_BOOL registered);
    RT_BOOL             isRegistered();

    // refs manage
    RT_RET              addRefs();
    INT32               refsCount();
    // Clears meta data and resets the range to the full extent.
    virtual void        reset();

 protected:
    void                decRefs();

 private:
    void                baseInit();

 protected:
    void           *mData;
    UINT32          mSize;
    UINT32          mRealSize;
    UINT32          mRangeOffset;
    UINT32          mRangeLength;
    INT32           mHandle;
    INT32           mFd;
    INT32           mUniqueId;    // all process can using this id
    INT32           mPoolID;
    UINT64          mModId;
    UINT32          mPhyAddr;
    RT_BOOL         mOwnsData;
    INT32           mRefCount;
    void           *mBufferID;
    INT32           mBufferSeq;
    RT_BOOL         mRegistered;
    RT_BOOL         mReleased;
    INT32           mFlag;
    RTMBUserData    mUserData;
    RTAllocator    *mAllocator;
    RtMutex        *mLock;
    RtMutex         mListenerLock;
    RtReference    *mBufObjRef;

    RtMediaBufferStatus     mStatus;
    RTMediaBufferSite       mSite;
    RTBufferListener       *mBufferListener;
    RtMetaData                     *mMetaData;
    std::map<INT32, RtMetaData *>   mExtraMetas;
};

#endif  // SRC_RT_MEDIA_INCLUDE_RTMEDIABUFFER_H_

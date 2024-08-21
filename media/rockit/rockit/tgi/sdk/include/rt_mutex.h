/*
 * Copyright 2018 Rockchip Electronics Co. LTD
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
 *   date: 20180719
 */

#include "rt_header.h" // NOLINT

#ifndef INCLUDE_RT_BASE_RT_MUTEX_H_
#define INCLUDE_RT_BASE_RT_MUTEX_H_

#define RET_CHECK_REF(ref, ret) \
    do { \
        if (ref->isExited()) \
            return ret; \
    } while (0)


typedef void *(*RTPthreadCallback)(void *);

class RtMutex;
class RtCondition;

/*
 * for shorter type name and function name
 */
class RtMutex {
 public:
    RtMutex();
    ~RtMutex();

    void lock();
    void unlock();
    int  trylock();

    class RtAutolock {
     public:
        explicit inline RtAutolock(RtMutex& rtMutex)     // NOLINT
                : mLock(rtMutex) {
            mLock.lock();
        }
        explicit inline RtAutolock(RtMutex* rtMutex)
                : mLock(*rtMutex) {
            mLock.lock();
        }
        inline ~RtAutolock() { mLock.unlock(); }

     private:
        RtMutex& mLock;
    };

 private:
    friend class RtCondition;

    void* mData;

    RtMutex(const RtMutex &);
    RtMutex &operator = (const RtMutex&);
};

typedef RtMutex::RtAutolock RtAutoMutex;

/*
 * for shorter type name and function name
 */
class RtCondition {
 public:
    RtCondition();
    explicit RtCondition(int type);
    ~RtCondition();

    /*These functions atomically release mutex,
      but block on the condition variable*/
    INT32 wait(const RtMutex& rtMutex);
    INT32 wait(RtMutex* rtMutex);

    /*returns with the timeout error*/
    INT32 timedwait(const RtMutex& rtMutex, UINT64 timeout_us);
    INT32 timedwait(RtMutex* rtMutex, UINT64 timeout_us);

    /*This wakes up at least one thread blocked on the condition variable*/
    INT32 signal();

    /*This wakes up all of the threads blocked on the condition variable*/
    INT32 broadcast();

 private:
    void* mData;
};

class RtSemaphore {
 public:
    RtSemaphore();
    ~RtSemaphore();

    INT32 init(INT32 pshared, UINT32 value);
    INT32 getValue(int *val);
    INT32 tryWait();
    INT32 wait();
    INT32 post();

 private:
    void* mData;
};

class RtReference {
 public:
    RtReference();
    ~RtReference();
    void addRef();
    void decRef();
    void wait();
    void waitExit();
    void reset();
    RT_BOOL isExited();
    INT32 getRefValue();

 private:
    RT_BOOL       mIsExit;
    INT32         mRefNum;
    RtCondition  *mCond;
    RtMutex      *mLock;
};

class RtAutoRef {
 public:
    explicit inline RtAutoRef(RtReference* ref)
            : mRef(ref) {
       mRef->addRef();
    }

    inline ~RtAutoRef() {
        mRef->decRef();
    }

 private:
    RtReference* mRef;
};


#endif  // INCLUDE_RT_BASE_RT_MUTEX_H_


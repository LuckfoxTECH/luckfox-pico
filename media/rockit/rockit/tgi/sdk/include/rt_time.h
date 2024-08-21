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
 *   date: 2018/07/05
 */

#ifndef INCLUDE_RT_BASE_RT_TIME_H_
#define INCLUDE_RT_BASE_RT_TIME_H_

#include "rt_header.h" // NOLINT
#include "rt_log.h"    // NOLINT

class RtTime {
 public:
    struct DateTime {
        UINT16 mYear;          //!< e.g. 2005
        UINT8  mMonth;         //!< 1..12
        UINT8  mDayOfWeek;     //!< 0..6, 0==Sunday
        UINT8  mDay;           //!< 1..31
        UINT8  mHour;          //!< 0..23
        UINT8  mMinute;        //!< 0..59
        UINT8  mSecond;        //!< 0..59
    };

    static void   getDateTime(DateTime*);
    static UINT64 getNowTimeMs();
    static UINT64 getNowTimeUs();
    static UINT64 getRelativeTimeMs();
    static UINT64 getRelativeTimeUs();
    static void   sleepMs(UINT64 time);
    static void   sleepUs(UINT64 time);
    static INT32  randInt();
};

class RtAutoTimeoutLog {
 public:
    // The label is not deep-copied, so its address must remain valid for the
    // lifetime of this object
    inline RtAutoTimeoutLog(const char* label = RT_NULL,
                                   UINT64 timeout_ms = 0)
            : mLabel(label) {
        mNow     = RtTime::getNowTimeMs();
        mTimeOut = timeout_ms;
    }
    inline ~RtAutoTimeoutLog() {
        UINT64 duration = RtTime::getNowTimeMs() - mNow;
        if (duration >= mTimeOut) {
            (void)mLabel;
            // RT_LOGE("%s [perf:%lld ms]\n", mLabel ? mLabel : "", duration);
        }
    }
 private:
    const char* mLabel;
    UINT64      mNow;
    UINT64      mTimeOut;
};

#endif  // INCLUDE_RT_BASE_RT_TIME_H_

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
 * src author: <mediapipe-team@google.com>
 * new author: modified by <rimon.xu@rock-chips.com>
 *       date: 2020-03-15
 *        ref: https://github.com/google/mediapipe
 */

#ifndef SRC_RT_TASK_TASK_GRAPH_RTTHREADPOOL_H_
#define SRC_RT_TASK_TASK_GRAPH_RTTHREADPOOL_H_

#include <deque>
#include <functional>
#include <string>
#include <vector>
#include <map>

#include "rt_header.h"              // NOLINT
#include "RTThreadOptions.h"        // NOLINT
#include "rt_thread.h"
#include "rt_mutex.h"

typedef enum _RTThreadPoolMode {
    RT_THREAD_POOL_RANDOM_MODE,
    RT_THREAD_POOL_ASSIGN_MODE,
} RTThreadPoolMode;

// A thread pool consists of a set of threads that sit around waiting
// for callbacks to appear on a queue.  When that happens, one of the
// threads pulls a callback off the queue and runs it.
//
// The thread pool is shut down when the pool is destroyed.
//
// Sample usage:
//
// {
//   RTThreadPool pool("testpool", num_workers);
//   pool.startWorkers();
//   for (INT32 i = 0; i < N; ++i) {
//     pool.schedule([i]() { DoWork(i); });
//   }
// }
//
class RTThreadPool {
 public:
    // Create a thread pool that provides a concurrency of "numThreads"
    // threads. I.e., if "numThreads" items are added, they are all
    // guaranteed to run concurrently without excessive delay.
    // It has an effectively infinite maximum queue length.
    // If numThreads is 1, the callbacks are run in FIFO order.
    explicit RTThreadPool(INT32 numThreads);
    RTThreadPool(const RTThreadPool&) = delete;
    RTThreadPool& operator=(const RTThreadPool&) = delete;

    // Like the RTThreadPool(INT32 numThreads) constructor, except that
    // it also associates "namePrefix" with each of the threads
    // in the thread pool.
    RTThreadPool(const std::string& namePrefix, INT32 numThreads,
                RTThreadPoolMode mode = RT_THREAD_POOL_RANDOM_MODE);

    // Create a thread pool that creates and can use up to "numThreads"
    // threads.  Any standard thread options, such as stack size, should
    // be passed via "threadOptions".  "namePrefix" specifies the
    // thread name prefix.
    RTThreadPool(const RTThreadOptions& threadOptions,
               const std::string& namePrefix, INT32 numThreads,
               RTThreadPoolMode mode = RT_THREAD_POOL_RANDOM_MODE);

    // Waits for closures (if any) to complete. May be called without
    // having called startWorkers().
    ~RTThreadPool();

    // REQUIRES: startWorkers has not been called
    // Actually start the worker threads.
    void startWorkers();

    // REQUIRES: startWorkers has been called
    // Add specified callback to queue of pending callbacks.  Eventually a
    // thread will pull this callback off the queue and execute it.
    void schedule(std::function<void()> callback, INT32 lockThreadId = 0);

    // Provided for debugging and testing only.
    INT32 getNumThreads() const;

    // Standard thread options.  Use this accessor to get them.
    const RTThreadOptions& getThreadOptions() const;

 private:
    class RTWorkerThread;
    typedef struct RTAssignWorker {
        RTWorkerThread *worker;
        std::deque<std::function<void()>> tasks;
        RtMutex mutex;
        RtCondition condition;
    } RTAssignWorker;

    void  runWorker(INT32 threadId);
    void  randomAndRunWorker();
    void  assignAndRunWorker(INT32 threadId);
    void  randomScheduleToWorker(std::function<void()> callback);
    void  assignScheduleToWorker(std::function<void()> callback, INT32 threadId);
    void  setThreadCount(INT32 numThreads);

    std::string mNamePrefix;
    /* thread id and worker */
    std::map<int, RTAssignWorker *> mWorkers;
    INT32 mNumThreads;

    RtMutex mMutex;
    RtCondition mCondition;
    bool mStopped = false;

    std::deque<std::function<void()>> mTasks;

    RTThreadOptions mThreadOptions;
    RTThreadPoolMode mMode;
};

namespace internal {

// Creates name for thread in a thread pool based on provided prefix and
// thread id. Length of the resulting name is guaranteed to be less or equal
// to 15. Name or thread id can be truncated to achieve that, see truncation
// samples below:
// namePrefix, 1234       -> namePrefix/123
// namePrefix, 1234567    -> namePrefix/123
// namePrefix_long, 1234  -> namePrefix_lon
std::string createThreadName(const std::string& prefix, INT32 threadId);

}  // namespace internal

#endif  // SRC_RT_TASK_TASK_GRAPH_RTTHREADPOOL_H_

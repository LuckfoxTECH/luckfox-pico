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
 * new author: modified by <rimon.xu@rock-chips.com> and <martin.cheng@rock-chips.com>
 *       date: 2020-03-19
 *  reference: https://github.com/google/mediapipe
 */

#ifndef SRC_RT_TASK_TASK_GRAPH_RTSCHEDULER_H_
#define SRC_RT_TASK_TASK_GRAPH_RTSCHEDULER_H_

#include <atomic>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <utility>
#include <vector>

#include "RTExecutor.h"
#include "RTSchedulerQueue.h"

class RTMediaBuffer;
class RTTaskNodeContext;
class RTTaskGraph;
// The class scheduling a calculator graph.
class RTScheduler {
 public:
    RTScheduler(const RTScheduler&) = delete;
    RTScheduler& operator=(const RTScheduler&) = delete;

    explicit RTScheduler(RTTaskGraph *ctx);

    ~RTScheduler();

    void setDefaultExecutor(RTExecutor* executor);
    RT_RET setExecutor(RTTaskNode *node, RTExecutor* executor);

    void scheduleNode(RTTaskNode *node, RTTaskNodeContext *nodeContext);

    void scheduleUnthrottledNodes(const std::vector<RTTaskNode *>& nodesToSchedule);
    void scheduleSuspendThrottledNodes(const std::vector<RTTaskNode *>& nodes);

    RT_RET waitUntilDone(INT64 timeoutUs = -1);
    // Wait until the running graph is in the idle mode, which is when nothing can
    // be scheduled and nothing is running in the worker threads.  This function
    // can be called only after Start().
    // Runs application thread tasks while waiting.
    RT_RET waitUntilIdle(INT64 timeoutUs = -1);
    RT_RET waitUntilEos(INT64 timeoutUs = -1);
    RT_RET cleanupAfterRun();
    RT_RET applicationThreadAwait(
          const std::function<bool()>& wakeupCondition, INT64 timeoutUs = -1);

    void queueIdleStateChanged(bool idle);
    void addNodeToOpenedQueue(RTTaskNode *node);
    void removeNodeFromOpenedQueue(RTTaskNode *node);
    RT_BOOL checkScheduleDone();
    RT_RET tryToScheduleIdleNode();
    void handleIdle();
    bool isIdle();
    RT_BOOL isRunning();
    void start();

    void flush();

    void reset();

    void quit();

    void reachEos();

    void pause() LOCKS_EXCLUDED(mStateMutex);

    void resume() LOCKS_EXCLUDED(mStateMutex);

    void stop() LOCKS_EXCLUDED(mStateMutex);

    void pauseAndWait();

    void setQueuesRunning(bool running);
    void submitWaitingTasksOnQueues();

    void notifyHasError(RT_BOOL hasError);
    void notifySchedulerPaused();

    void throttledGraphInputStream();
    void unthrottledGraphInputStream();
    RT_RET waitUntilGraphInputStreamUnthrottled(RtMutex *secondaryMutex, INT64 timeoutUs);
    void emittedObservedOutput();
    RT_RET waitForObservedOutput(INT64 timeoutUs = -1);
    RT_RET waitForUntilPaused();

 private:
    // State of the scheduler. The figure shows the allowed state transitons.
    //
    //   NOT_STARTED
    //        |
    //        v
    //     RUNNING--+
    //     | | ^    |
    //     | |  \   |
    //     | |   \  v
    //     | |  PAUSED
    //     | |    |
    //     | v    v
    //     | CANCELLING
    //     |     |
    //     v     v
    //   TERMINATING
    //        |
    //        v
    //    TERMINATED
    enum State {
      STATE_NOT_STARTED = 0,  // The initial state.
      STATE_RUNNING = 1,      // The scheduler is running and scheduling nodes.
      STATE_PAUSED = 2,       // The scheduler is not scheduling nodes.
      STATE_CANCELLING = 3,   // The scheduler is being cancelled. The scheduler
                              // cannot be paused in this state so that
                              // scheduler_queue_ can be drained.
      STATE_TERMINATED = 4,   // The scheduler has terminated.
    };
    std::atomic<State> mState = ATOMIC_VAR_INIT(STATE_NOT_STARTED);
    RtMutex     mStateMutex;
    RtMutex     mThrottleMutex;
    RtCondition mStateCondition;
    // Queue of nodes that need to be run.
    RTSchedulerQueue mDefaultQueue;
    std::vector<RTSchedulerQueue *> mSchedulerQueues;
    // Number of queues which are not idle.
    // Note: this indicates two slightly different things:
    //  a. the number of queues which still have nodes running;
    //  b. the number of queues whose executors may still access the scheduler.
    // When a queue becomes idle, it has stopped running nodes, and the scheduler
    // decrements the count. However, it is not done accessing the scheduler
    // until HandleIdle returns. Therefore, a and b are briefly out of sync.
    // This is ok, because it happens within a single critical section, which is
    // guarded by state_mutex_. If we wanted to split this critical section, we
    // would have to separate a and b into two variables.
    INT32 mNonIdleQueueCount = 0;
    // Used by HandleIdle to avoid multiple concurrent executions.
    // We cannot simply hold a mutex throughout it, for two reasons:
    // - We need it to be reentrant, which Mutex does not support.
    // - We want simultaneous calls to return immediately instead of waiting,
    //   and Mutex's TryLock is not guaranteed to work.
    bool mHandlingIdle = false;
    std::vector<RTTaskNode *> mOpendNodeQueue;
    std::vector<RTTaskNode *> mActiveSourceNodes;
    // Data accessed by all SchedulerQueues.
    RTSchedulerShared mShared;
    RTTaskGraph *mTaskGraph;
    // Number of throttled graph input streams.
    INT32 mThrottledGraphInputStreamCount = 0;
    // Used to stop WaitUntilGraphInputStreamUnthrottled.
    INT32 mUnthrottleSeqNum = 0;
    // Used to stop WaitForObservedOutput.
    RT_BOOL mObservedOutputSignal = RT_FALSE;
    // True if an application thread is waiting in waitForObservedOutput.
    RT_BOOL mWaitingForObservedOutput = RT_FALSE;
    RT_BOOL mWaitindForPaused = RT_FALSE;
    RT_BOOL mReachEos;
};

#endif  // SRC_RT_TASK_TASK_GRAPH_RTSCHEDULER_H_


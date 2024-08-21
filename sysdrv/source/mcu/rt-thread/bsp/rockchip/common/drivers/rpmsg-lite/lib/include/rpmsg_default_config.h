/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * Copyright (c) 2015 Xilinx, Inc.
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef RPMSG_DEFAULT_CONFIG_H_
#define RPMSG_DEFAULT_CONFIG_H_

#define RL_USE_CUSTOM_CONFIG (1)

#if RL_USE_CUSTOM_CONFIG
#include "rpmsg_config.h"
#endif

/*!
 * @addtogroup config
 * @{
 * @file
 */

//! @name Configuration options
//@{

//! @def RL_MS_PER_INTERVAL
//!
//! Delay in milliseconds used in non-blocking API functions for polling.
//! The default value is 1.
#ifndef RL_MS_PER_INTERVAL
#define RL_MS_PER_INTERVAL (1)
#endif

//! @def RL_ALLOW_CUSTOM_SHMEM_CONFIG
//!
//! This option allows to define custom shared memory configuration and replacing
//! the shared memory related global settings from rpmsg_config.h This is useful
//! when multiple instances are running in parallel but different shared memory
//! arrangement (vring size & alignment, buffers size & count) is required. Note,
//! that once enabled the platform_get_custom_shmem_config() function needs
//! to be implemented in platform layer. The default value is 0 (all RPMsg_Lite
//! instances use the same shared memory arrangement as defined by common config macros).
#ifndef RL_ALLOW_CUSTOM_SHMEM_CONFIG
#define RL_ALLOW_CUSTOM_SHMEM_CONFIG (0)
#endif

#if !(defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1))
//! @def RL_BUFFER_PAYLOAD_SIZE
//!
//! Size of the buffer payload, it must be equal to (240, 496, 1008, ...)
//! [2^n - 16]. Ensure the same value is defined on both sides of rpmsg
//! communication. The default value is 496U.
#ifndef RL_BUFFER_PAYLOAD_SIZE
#define RL_BUFFER_PAYLOAD_SIZE (496U)
#endif

//! @def RL_BUFFER_COUNT
//!
//! Number of the buffers, it must be power of two (2, 4, ...).
//! The default value is 2U.
//! Note this value defines the buffer count for one direction of the rpmsg
//! communication only, i.e. if the default value of 2 is used
//! in rpmsg_config.h files for the master and the remote side, 4 buffers
//! in total are created in the shared memory.
#ifndef RL_BUFFER_COUNT
#define RL_BUFFER_COUNT (2U)
#endif

#else
//! Define the buffer payload and count per different link IDs (rpmsg_lite instance) when RL_ALLOW_CUSTOM_SHMEM_CONFIG
//! is set.
//! Refer to the rpmsg_plaform.h for the used link IDs.
#ifndef RL_BUFFER_PAYLOAD_SIZE
#define RL_BUFFER_PAYLOAD_SIZE(link_id) (496U)
#endif

#ifndef RL_BUFFER_COUNT
#define RL_BUFFER_COUNT(link_id) (((link_id) == 0U) ? 256U : 2U)
#endif
#endif /* !(defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1))*/

//! @def RL_API_HAS_ZEROCOPY
//!
//! Zero-copy API functions enabled/disabled.
//! The default value is 1 (enabled).
#ifndef RL_API_HAS_ZEROCOPY
#define RL_API_HAS_ZEROCOPY (1)
#endif

//! @def RL_USE_STATIC_API
//!
//! Static API functions (no dynamic allocation) enabled/disabled.
//! The default value is 0 (static API disabled).
#ifndef RL_USE_STATIC_API
#define RL_USE_STATIC_API (0)
#endif

//! @def RL_CLEAR_USED_BUFFERS
//!
//! Clearing used buffers before returning back to the pool of free buffers
//! enabled/disabled.
//! The default value is 0 (disabled).
#ifndef RL_CLEAR_USED_BUFFERS
#define RL_CLEAR_USED_BUFFERS (0)
#endif

//! @def RL_USE_MCMGR_IPC_ISR_HANDLER
//!
//! When enabled IPC interrupts are managed by the Multicore Manager (IPC
//! interrupts router), when disabled RPMsg-Lite manages IPC interrupts
//! by itself.
//! The default value is 0 (no MCMGR IPC ISR handler used).
#ifndef RL_USE_MCMGR_IPC_ISR_HANDLER
#define RL_USE_MCMGR_IPC_ISR_HANDLER (0)
#endif

//! @def RL_USE_ENVIRONMENT_CONTEXT
//!
//! When enabled the environment layer uses its own context.
//! Added for QNX port mainly, but can be used if required.
//! The default value is 0 (no context, saves some RAM).
#ifndef RL_USE_ENVIRONMENT_CONTEXT
#define RL_USE_ENVIRONMENT_CONTEXT (0)
#endif

//! @def RL_DEBUG_CHECK_BUFFERS
//!
//! Do not use in RPMsg-Lite to Linux configuration
#ifndef RL_DEBUG_CHECK_BUFFERS
#define RL_DEBUG_CHECK_BUFFERS (0)
#endif

//! @def RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION
//!
//! When enabled the opposite side is notified each time received buffers
//! are consumed and put into the queue of available buffers.
//! Enable this option in RPMsg-Lite to Linux configuration to allow unblocking
//! of the Linux blocking send.
//! The default value is 0 (RPMsg-Lite to RPMsg-Lite communication).
#ifndef RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION
#define RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION (0)
#endif

//! @def RL_HANG
//!
//! Default implementation of hang assert function
static inline void RL_HANG(void)
{
    for (;;)
    {
    }
}

//! @def RL_ASSERT
//!
//! Assert implementation.
#ifndef RL_ASSERT
#define RL_ASSERT_BOOL(b) \
    do                    \
    {                     \
        if (!(b))         \
        {                 \
            RL_HANG();    \
        }                 \
    } while (0 == 1);
#define RL_ASSERT(x) RL_ASSERT_BOOL((int32_t)(x) != 0)

#endif
//@}

#endif /* RPMSG_DEFAULT_CONFIG_H_ */

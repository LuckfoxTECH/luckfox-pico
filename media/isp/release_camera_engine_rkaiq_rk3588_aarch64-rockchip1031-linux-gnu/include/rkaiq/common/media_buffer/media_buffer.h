/******************************************************************************
 *
 * Copyright 2007, Silicon Image, Inc.  All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of: Silicon Image, Inc., 1060
 * East Arques Avenue, Sunnyvale, California 94085
 *
 *****************************************************************************/
/**
 * @file media_buffer.h
 *
 * @brief
 *          Media Buffer interface
 *
 * <pre>
 *
 *   Principal Author: Joerg Detert
 *   Creation date:    Feb 28, 2008
 *
 * </pre>
 *
 *****************************************************************************/
#ifndef MEDIA_BUFFER_H_
#define MEDIA_BUFFER_H_

#include <assert.h>
#include <stddef.h>
#include "rk_aiq_comm.h"

#if defined (__cplusplus)
extern "C" {
#endif

#define RET_SUCCESS             0   //!< this has to be 0, if clauses rely on it
#define RET_FAILURE             1   //!< general failure
#define RET_NOTSUPP             2   //!< feature not supported
#define RET_BUSY                3   //!< there's already something going on...
#define RET_CANCELED            4   //!< operation canceled
#define RET_OUTOFMEM            5   //!< out of memory
#define RET_OUTOFRANGE          6   //!< parameter/value out of range
#define RET_IDLE                7   //!< feature/subsystem is in idle state
#define RET_WRONG_HANDLE        8   //!< handle is wrong
#define RET_NULL_POINTER        9   //!< the/one/all parameter(s) is a(are) NULL pointer(s)
#define RET_NOTAVAILABLE       10   //!< profile not available
#define RET_DIVISION_BY_ZERO   11   //!< a divisor equals ZERO
#define RET_WRONG_STATE        12   //!< state machine in wrong state
#define RET_INVALID_PARM       13   //!< invalid parameter
#define RET_PENDING            14   //!< command pending
#define RET_WRONG_CONFIG       15   //!< given configuration is invalid

typedef unsigned long          ulong_t;

/**
 * @brief The MediaBufferPool holds elements from type MediaBuffer_t.
 */
typedef struct MediaBuffer_s
{
    uint8_t*      pBaseAddress; /**< Base address of system memory buffer (can differ from
                                     actual buffer start address, set in ScmiBuffer). */
    uint32_t      baseSize;     /**< Base size of buffer (can differ from actual buffer
                                     size, set in ScmiBuffer). */
    uint32_t      lockCount;    /**< Counting how many times buffer is used. 0 means
                                     buffer belongs to pool and is free. */
    void*         pOwner;

    void*         pMetaData;    /**< Pointer to optional meta data structure. */
} MediaBuffer_t;

/*****************************************************************************/
/**
 * @brief   Initialize a mutex.
 *
 * @param
 *
 *****************************************************************************/
void AtomicMutexInit();

/*****************************************************************************/
/**
 * @brief   Destroy a mutex.
 *
 * @param
 *
 *****************************************************************************/
void AtomicMutexDestory();

/*****************************************************************************/
/**
 * @brief   Initialize a @ref MediaBuffer_t.
 *
 * @param   pBuf    Buffer to initialize.
 *
 *****************************************************************************/
extern void MediaBufInit(MediaBuffer_t* pBuf);

/*****************************************************************************/
/**
 * @brief Lock a buffer of a owning buffer pool. Buffer will not be available as
 *        empty buffer until unlocked as many times as locked before
 *        and released.
 *
 * @param   pBufQueue   Pointer to Media Buffer Queue object.
 * @param   pBuf        Pointer to media buffer.
 *
 * @return  Status of operation.
 *****************************************************************************/
extern RESULT MediaBufLockBuffer(MediaBuffer_t*   pBuf);


/*****************************************************************************/
/**
 * @brief Unlock a buffer of a owning buffer pool which has previously been locked.
 *
 * @param   pBufQueue   Pointer to Media Buffer Queue object.
 * @param   pBuf        Pointer to media buffer.
 *
 * @return  Status of operation.
 *****************************************************************************/
extern RESULT MediaBufUnlockBuffer(MediaBuffer_t*   pBuf);


#if defined (__cplusplus)
}
#endif


#endif /*MEDIA_BUFFER_H_*/

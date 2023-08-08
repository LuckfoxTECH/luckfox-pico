/**
 * @file uni_queue.h
 * @brief tuya common queue module
 * @version 1.0
 * @date 2019-10-30
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */
#ifndef _UNI_QUEUE_H
#define _UNI_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_error_code.h"
#include "tuya_os_adapter.h"

/**
 * @brief support reentry prevent
 *
 */
#define QUEUE_SAFE_FUNC

/**
 * @brief queue data structure define
 *
 */
typedef struct {
	unsigned int queUnitSize; // queue unit size
	unsigned int queTolNum;   // queue total unit number
	unsigned int queCurNum;   // queue current unit number

	unsigned char *queData; // queue buffer
	unsigned char *queIn;   // in queue position
	unsigned char *queOut;  // out queue position

#ifdef QUEUE_SAFE_FUNC
	MUTEX_HANDLE mutex; // safity access prevent reentry
#endif
} QUEUE_CLASS, *P_QUEUE_CLASS;

/**
 * @brief create and initialize a queue
 *
 * @param[in] queTolNum the total number of the queue
 * @param[in] queUnitSize the unit size of the queue
 * @return the queue handle
 */
P_QUEUE_CLASS CreateQueueObj(const unsigned int queTolNum, const unsigned int queUnitSize);

/**
 * @brief register a queue, the queue handle and queue buffer malloc from other place
 *
 * @param[in] pQueObj the queue handle
 * @param[in] pQueData the queue unit buffer
 * @param[in] queTolNum the total number of queue
 * @param[in] queUnitSize the unit size of queue
 * @return 1 on success, 0 on failed
 *
 * @warning this API was NOT USED!!!
 */
unsigned char RegisterQueueObj(P_QUEUE_CLASS pQueObj, const unsigned char *pQueData,
                               const unsigned int queTolNum, const unsigned int queUnitSize);

/**
 * @brief unit inqueue
 *
 * @param[in] pQueObj the queue handle
 * @param[in] pQueUnit the inqueue unit buffer
 * @param[in] queNum the inqueue unit counts
 * @return 1 on success, 0 on failed
 */
unsigned char InQueue(P_QUEUE_CLASS pQueObj, const unsigned char *pQueUnit,
                      const unsigned int queNum);

/**
 * @brief unit outqueue
 *
 * @param[in] pQueObj the queue handle
 * @param[in] pQueUnit the outqueue unit buffer
 * @param[in] queNum the outqueue unit counts
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
unsigned char OutQueue(P_QUEUE_CLASS pQueObj, unsigned char *pQueUnit, const unsigned int queNum);

/**
 * @brief get the unit from start postion, not outqueue
 *
 * @param[in] pQueObj the queue handle
 * @param[in] start the outqueue start postion
 * @param[in] pQueUnit the outqueue unit buffer
 * @param[in] queNum the outqueue unit counts
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
unsigned char GetQueueMember(P_QUEUE_CLASS pQueObj, const unsigned int start,
                             unsigned char *pQueUnit, const unsigned int queNum);

/**
 * @brief clear all unit of the queue
 *
 * @param[in] pQueObj the queue handle
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
unsigned char ClearQueue(P_QUEUE_CLASS pQueObj);

/**
 * @brief delete the unit from the queue out position
 *
 * @param[in] pQueObj the queue handle
 * @param[in] queNum the delete unit counts
 * @return OPRT_OK on success, others on failed, please refer to tuya_error_code.h
 */
unsigned char DelQueueMember(P_QUEUE_CLASS pQueObj, const unsigned int queNum);

/**
 * @brief get the free queue unit number
 *
 * @param[in] pQueObj the queue handle
 * @return the current free unit counts
 */
unsigned int GetCurFreeQueNum(P_QUEUE_CLASS pQueObj);

/**
 * @brief get the queue unit number
 *
 * @param[in] pQueObj the queue handle
 * @return the current unit counts
 */
unsigned int GetCurQueNum(P_QUEUE_CLASS pQueObj);

/**
 * @brief release the queue
 *
 * @param[in] pQueObj the queue handle
 */
void ReleaseQueueObj(P_QUEUE_CLASS pQueObj);

#ifdef __cplusplus
}
#endif
#endif

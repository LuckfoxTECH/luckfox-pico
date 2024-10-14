/*
 * Copyright (c) 2021 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Miscellaneous debug strings
 */


#ifndef __FMAC_STRS_H__
#define __FMAC_STRS_H__


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/spinlock.h>
#include <linux/completion.h>
#include "lmac_msg.h"
#include "ipc_shared.h"


/*******************************************************************************
 *         Defines
 ******************************************************************************/
#define SSV_ID2STR(tag)             (((MSG_T(tag) < ARRAY_SIZE(ssv_id2str)) && \
                                        (ssv_id2str[MSG_T(tag)]) && \
                                        ((ssv_id2str[MSG_T(tag)])[MSG_I(tag)])) ? \
                                        (ssv_id2str[MSG_T(tag)])[MSG_I(tag)] : "unknown")

#define SSV_TASKID2STR(tag) ((((tag) < ARRAY_SIZE(ssv_taskid2str)) && \
                                        (ssv_taskid2str[tag])) ? \
                                        (ssv_taskid2str[tag]) : "unknown")
/*******************************************************************************
 *         Enumerations
 ******************************************************************************/


/*******************************************************************************
 *         Structures
 ******************************************************************************/


/*******************************************************************************
 *         Variables
 ******************************************************************************/
extern const char *const *ssv_id2str[TASK_LAST_EMB + 1];
extern const char *const  ssv_taskid2str[TASK_MAX];

/*******************************************************************************
 *         Functions
 ******************************************************************************/


#endif /* __FMAC_STRS_H__ */


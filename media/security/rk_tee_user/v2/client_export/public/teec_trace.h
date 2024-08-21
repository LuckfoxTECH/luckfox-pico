/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
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
#ifndef TEEC_TRACE_H
#define TEEC_TRACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#ifdef RK_PRINT_TO_LOGCAT
#include <log/log.h>
#endif

#ifndef BINARY_PREFIX
#error "BINARY_PREFIX not defined"
#endif

/*
 * Trace levels.
 *
 * ERROR is used when some kind of error has happened, this is most likely the
 * print you will use most of the time when you report some kind of error.
 *
 * INFO is used when you want to print some 'normal' text to the user.
 * This is the default level.
 *
 * DEBUG is used to print extra information to enter deeply in the module.
 *
 * FLOW is used to print the execution flox, typically the in/out of functions.
 *
 * */

#define TRACE_ERROR  1
#define TRACE_INFO   2
#define TRACE_DEBUG  3
#define TRACE_FLOW   4

#if defined(DEBUGLEVEL_0) && !defined(DEBUGLEVEL)
#define DEBUGLEVEL TRACE_ERROR
#endif

#if defined(DEBUGLEVEL_1) && !defined(DEBUGLEVEL)
#define DEBUGLEVEL TRACE_ERROR
#endif

#if defined(DEBUGLEVEL_2) && !defined(DEBUGLEVEL)
#define DEBUGLEVEL TRACE_INFO
#endif

#if defined(DEBUGLEVEL_3) && !defined(DEBUGLEVEL)
#define DEBUGLEVEL TRACE_DEBUG
#endif

#if defined(DEBUGLEVEL_4) && !defined(DEBUGLEVEL)
#define DEBUGLEVEL TRACE_FLOW
#endif

#ifndef DEBUGLEVEL
/* Default debug level. */
#define DEBUGLEVEL TRACE_INFO
#endif

#define TEEC_LOG_TAG "tee_client"

/*
 * This define make sure that parameters are checked in the same manner as it
 * is done in the normal printf function.
 */
#define __PRINTFLIKE(__fmt, __varargs) __attribute__\
	((__format__(__printf__, __fmt, __varargs)))

void _dprintf(const char *function, int line, int level, const char *prefix,
	      const char *fmt, ...) __PRINTFLIKE(5, 6);

#ifdef RK_PRINT_TO_LOGCAT
#define dprintf(level, x...) do { \
		if ((level) <= DEBUGLEVEL) { \
			switch (level) { \
			case TRACE_ERROR: \
				__android_log_print(ANDROID_LOG_ERROR, TEEC_LOG_TAG, x); \
				break; \
			case TRACE_INFO: \
				__android_log_print(ANDROID_LOG_INFO, TEEC_LOG_TAG, x); \
				break; \
			case TRACE_DEBUG: \
				__android_log_print(ANDROID_LOG_DEBUG, TEEC_LOG_TAG, x); \
				break; \
			case TRACE_FLOW: \
				__android_log_print(ANDROID_LOG_VERBOSE, TEEC_LOG_TAG, x); \
				break; \
			default: \
				break; \
			} \
		} \
	} while (0)
#else
#define dprintf(level, x...) do { \
		if ((level) <= DEBUGLEVEL) { \
			_dprintf(__func__, __LINE__, level, \
				 BINARY_PREFIX, x); \
		} \
	} while (0)
#endif

#define EMSG(fmt, ...)   dprintf(TRACE_ERROR, fmt "\n", ##__VA_ARGS__)
#define IMSG(fmt, ...)   dprintf(TRACE_INFO, fmt "\n", ##__VA_ARGS__)
#define DMSG(fmt, ...)   dprintf(TRACE_DEBUG, fmt "\n", ##__VA_ARGS__)
#define FMSG(fmt, ...)   dprintf(TRACE_FLOW, fmt "\n", ##__VA_ARGS__)

#define INMSG(fmt, ...)  FMSG("> " fmt, ##__VA_ARGS__)
#define OUTMSG(fmt, ...) FMSG("< " fmt, ##__VA_ARGS__)
#define OUTRMSG(r)                              \
	do {                                            \
		if (r)                                      \
			EMSG("Function returns with [%d]", r);  \
		OUTMSG("r=[%d]", r);                        \
		return r;                                   \
	} while (0)

#ifdef RK_PRINT_TO_LOGCAT
#define dprintf_raw dprintf
#else
#define dprintf_raw(level, x...) do { \
		if ((level) <= DEBUGLEVEL) \
			_dprintf(0, 0, (level), BINARY_PREFIX, x); \
	} while (0)
#endif

#define EMSG_RAW(fmt, ...)   dprintf_raw(TRACE_ERROR, fmt, ##__VA_ARGS__)
#define IMSG_RAW(fmt, ...)   dprintf_raw(TRACE_INFO, fmt, ##__VA_ARGS__)
#define DMSG_RAW(fmt, ...)   dprintf_raw(TRACE_DEBUG, fmt, ##__VA_ARGS__)
#define FMSG_RAW(fmt, ...)   dprintf_raw(TRACE_FLOW, fmt, ##__VA_ARGS__)

/*
 * This function will hex and ascii dump a buffer.
 *
 * Note that this function will only print if debug flag
 * DEBUGLEVEL is INFO or FLOOD.
 *
 * @param bname     Information string describing the buffer.
 * @param buffer    Pointer to the buffer.
 * @param blen      Length of the buffer.
 *
 * @return void
 */
void dump_buffer(const char *bname, const uint8_t *buffer, size_t blen);

#ifdef __cplusplus
}
#endif

#endif

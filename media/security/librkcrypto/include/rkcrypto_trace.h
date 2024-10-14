/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _RKCRYPTO_TRACE_H_
#define _RKCRYPTO_TRACE_H_

#include <stdio.h>
#include "rkcrypto_common.h"

enum RKCRYPTO_TRACE_LEVEL {
	TRACE_TOP     = 0,
	TRACE_ERROR   = 1,
	TRACE_INFO    = 2,
	TRACE_DEBUG   = 3,
	TRACE_VERBOSE = 4,
	TRACE_BUTT,
};

void trace_printf(int level, const char *function, int line, const char *fmt, ...);
void hex_dump(int level, const char *function, int line, const char *buffer_name,
	      const void *buffer, int len);
#define V_TRACE(...) \
	trace_printf(TRACE_VERBOSE, __func__, __LINE__, __VA_ARGS__)

#define D_TRACE(...) \
	trace_printf(TRACE_DEBUG, __func__, __LINE__, __VA_ARGS__)

#define I_TRACE(...) \
	trace_printf(TRACE_INFO, __func__, __LINE__, __VA_ARGS__)

#define E_TRACE(...) \
	trace_printf(TRACE_ERROR, __func__, __LINE__, __VA_ARGS__)

#define VHEX_DUMP(buffer_name, buffer, len) \
	hex_dump(TRACE_VERBOSE, __func__, __LINE__, buffer_name, buffer, len)

#define RK_CRYPTO_CHECK_PARAM(_val)\
	do {\
		if (_val) {\
			E_TRACE("RK_CRYPTO_CHECK_PARAM ERR! 0x%08x", RK_CRYPTO_ERR_PARAMETER);\
			return RK_CRYPTO_ERR_PARAMETER;\
		}\
	} while (0)

RK_RES rkcrypto_set_trace_level(enum RKCRYPTO_TRACE_LEVEL level);

#endif /* _RKCRYPTO_TRACE_H_ */

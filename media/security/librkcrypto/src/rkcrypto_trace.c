/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "rkcrypto_trace.h"

#ifdef ANDROID
#include <android/log.h>
#include <cutils/properties.h>

#define RKCRYPTO_TRACE_LEVEL_NAME	"vendor.rkcrypto.trace.level"
#else
#define RKCRYPTO_TRACE_LEVEL_NAME	"rkcrypto_trace_level"
#endif

#define RKCRYPTO_LOG_TAG		"rkcrypto"

static int trace_level = -1;

static void rkcrypto_get_trace_level(void)
{
	char pick_value;

	if (trace_level >= TRACE_ERROR && trace_level <= TRACE_VERBOSE)
		return;

#ifdef ANDROID
	char property_value[PROPERTY_VALUE_MAX] = {0};

	if (property_get(RKCRYPTO_TRACE_LEVEL_NAME, property_value, NULL) > 0)
		pick_value = property_value[0];
#else
	char *env_value = getenv(RKCRYPTO_TRACE_LEVEL_NAME);
	if (env_value)
		pick_value = env_value[0];
#endif
	sscanf(&pick_value, "%d", &trace_level);

	if (trace_level < TRACE_ERROR || trace_level > TRACE_VERBOSE)
		trace_level = TRACE_INFO;
}

RK_RES rkcrypto_set_trace_level(enum RKCRYPTO_TRACE_LEVEL level)
{
	char ch[2];

	if (level < TRACE_ERROR || level > TRACE_VERBOSE)
		return RK_CRYPTO_ERR_PARAMETER;

	sprintf(ch, "%d", level);

#ifdef ANDROID
	if (property_set(RKCRYPTO_TRACE_LEVEL_NAME, ch)) {
		__android_log_print(ANDROID_LOG_ERROR, RKCRYPTO_LOG_TAG, "property_set error!");
		return RK_CRYPTO_ERR_GENERIC;
	}
#else
	if (setenv(RKCRYPTO_TRACE_LEVEL_NAME, ch, 1)) {
		printf("setenv error!");
		return RK_CRYPTO_ERR_GENERIC;
	}
#endif
	trace_level = -1;
	return RK_CRYPTO_SUCCESS;
}

void trace_printf(int level, const char *function, int line, const char *fmt, ...)
{
	rkcrypto_get_trace_level();

	if (level > trace_level) {
		return;
	} else {
		int i;
		char buffer[512];
		va_list args;

		va_start(args, fmt);

#ifdef ANDROID
		int android_prio = ANDROID_LOG_INFO;
		char head[100];

		switch (level) {
		case TRACE_ERROR:
			android_prio = ANDROID_LOG_ERROR;
			break;
		case TRACE_INFO:
			android_prio = ANDROID_LOG_INFO;
			break;
		case TRACE_DEBUG:
			android_prio = ANDROID_LOG_DEBUG;
			break;
		case TRACE_VERBOSE:
			android_prio = ANDROID_LOG_VERBOSE;
			break;
		default:
			break;
		}

		sprintf(head, "%s: [%s, %d]", RKCRYPTO_LOG_TAG, function, line);
		vsprintf(buffer, fmt, args);

		if (buffer[0] != '\0') {
			for (i = strlen(buffer) - 1; i >= 0; i--) {
				if (buffer[i] != '\n')
					break;
			}

			buffer[i + 1] = '\0';
		}

		__android_log_print(android_prio, head, "%s", buffer);
#else
		char linux_prio = 'I';

		switch (level) {
		case TRACE_ERROR:
			linux_prio = 'E';
			break;
		case TRACE_INFO:
			linux_prio = 'I';
			break;
		case TRACE_DEBUG:
			linux_prio = 'D';
			break;
		case TRACE_VERBOSE:
			linux_prio = 'V';
			break;
		default:
			break;
		}

		printf("%c %s: [%s, %d]: ", linux_prio, RKCRYPTO_LOG_TAG, function, line);
		vsprintf(buffer, fmt, args);

		if (buffer[0] != '\0') {
			for (i = strlen(buffer) - 1; i >= 0; i--) {
				if (buffer[i] != '\n')
					break;
			}

			buffer[i + 1] = '\0';
		}

		printf("%s\n", buffer);
#endif

		va_end(args);
	}
}

void hex_dump(int level, const char *function, int line, const char *buffer_name,
	      const void *buffer, int len)
{
	trace_printf(level, function, line, "%s(%d): ", buffer_name, len);

	if (level > trace_level) {
		return;
	} else {
		int i, j, line_count;
		char ch[4], tmp[64];
		char *in = (char *)buffer;

		line_count = len / 16;
		line_count = len % 16 ? line_count + 1 : line_count;

		for (i = 0; i < line_count; i++) {
			memset(tmp, 0, sizeof(tmp));
			for (j = 16 * i; j < 16 * (i + 1); j++) {
				if (j < len)
					sprintf(ch, "%02x ", in[j]);
				else
					sprintf(ch, "-- ");

				strcat(tmp, ch);
			}

#ifdef ANDROID
			__android_log_print(ANDROID_LOG_VERBOSE, RKCRYPTO_LOG_TAG, "%s", tmp);
			if (i % 16 == 0 && i != 0)
				__android_log_print(ANDROID_LOG_VERBOSE, RKCRYPTO_LOG_TAG, "\n");

#else
			printf("V %s: %s\n", RKCRYPTO_LOG_TAG, tmp);
			if (i % 16 == 0 && i != 0)
				printf("V %s: \n", RKCRYPTO_LOG_TAG);
#endif
		}
	}
}

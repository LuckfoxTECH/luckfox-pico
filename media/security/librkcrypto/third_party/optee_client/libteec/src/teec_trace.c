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
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <pthread.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#include "teec_trace.h"

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/*
 * The length of the prefix is 37, for example it looks like this:
 * P = Prefix
 * M = Message
 * F = Function name
 * L = Line number
 * PPPP: MMMMM [FFFFFFFFFFFFFFF : LLLLL]
 */
#define MAX_PRINT_SIZE 256

#ifdef TEEC_LOG_FILE
static void log_to_file(const char *buffer)
{
	FILE *log_file = fopen(TEEC_LOG_FILE, "a");

	if (log_file != NULL) {
		fprintf(log_file, "%s", buffer);
		fclose(log_file);
		log_file = NULL;
	}
}
#else
#define log_to_file(buffer)
#endif

static const char * const trace_level_strings[] = {
	"", "ERR", "INF", "DBG", "FLW"
};

void _dprintf(const char *function, int line, int level, const char *prefix,
	      const char *fmt, ...)
{
	char msg[MAX_PRINT_SIZE];
	int n = 0;
	va_list ap;

	if (function) {
		int thread_id = syscall(SYS_gettid);

		n = snprintf(msg, sizeof(msg), "%s [%d] %s:%s:%d: ",
			trace_level_strings[level], thread_id, prefix,
			function, line);
		if (n < 0)
			return;
	}

	if ((size_t)n < sizeof(msg)) {
		va_start(ap, fmt);
		n = vsnprintf(msg + n, sizeof(msg) - n, fmt, ap);
		va_end(ap);
		if (n < 0)
			return;
	}

	fprintf(stdout, "%s", msg);
	log_to_file(msg);
}

#if (defined(DEBUGLEVEL_3) || defined(DEBUGLEVEL_true) || defined(DEBUGLEVEL_4))
void dump_buffer(const char *bname, const uint8_t *buffer, size_t blen)
{
	fprintf(stderr, "#### %s\n", bname);

	while (blen > 0) {
		size_t n = 0;

		for (n = 0; n < 16; n++) {
			if (n < blen)
				fprintf(stderr, "%02x ", (int)buffer[n]);
			else
				fprintf(stderr, "   ");

			if (n == 7)
				fprintf(stderr, " ");
		}

		fprintf(stderr, " |");

		for (n = 0; n < 16; n++) {
			if (n < blen) {
				if (isprint(buffer[n]))
					fprintf(stderr, "%c", (int)buffer[n]);
				else
					fprintf(stderr, ".");
			}
		}

		fprintf(stderr, "|\n");

		blen -= MIN(blen, 16);
		buffer += 16;
	}
}
#else
void dump_buffer(const char *bname, const uint8_t *buffer, size_t blen)
{
	(void)bname;
	(void)buffer;
	(void)blen;
}
#endif

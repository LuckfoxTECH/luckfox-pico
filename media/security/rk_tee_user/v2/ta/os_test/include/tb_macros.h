/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2014, STMicroelectronics International N.V.
 * All rights reserved.
 */
#ifndef TB_MACROS_H
#define TB_MACROS_H

#include <tee_internal_api_extensions.h>

#define TB_HEADER(str)                               \
	printf("\n*********** TESTBENCH ***********" \
	       "\n***         RUNNING: <<< %s >>>"   \
	       "\n*********************************\n\n", str)

#define TB_FOOTER(str)                               \
	printf("\n*********** TESTBENCH ***********" \
	       "\n***         PASSED:  <<< %s >>>"   \
	       "\n*********************************\n\n", str)

#define TB_INFO(str) printf("*** INFO : %s \n", (str))

#define HALT                                                             \
	{                                                                \
		printf("\n*** FAILED ***"                                \
		       "\nTestbench halted at line %d in function %s\n", \
			 __LINE__, __func__);                            \
		printf("\nWaiting for keypress to enable debugging.\n"); \
		TEE_Panic(0);                                            \
	}

#define STARTING                                       \
	printf("\n*********** TESTBENCH ***********"   \
	       "\n*** For the GlobalPlatform Math API" \
	       "\n*********************************\n\n")

#define ALL_PASSED \
	printf("\n*********** TESTBENCH ***********" \
	       "\n***     ALL TESTS PASSED      ***" \
	       "\n*********************************\n\n")

/*
 * DEF_BIGINT defines and initialize a BigInt with name and size.
 */
#define DEF_BIGINT(name, size)                                                \
	TEE_BigInt *name;                                                     \
	size_t name##_size;                                                   \
	name##_size = TEE_BigIntSizeInU32(size);                              \
	name = (TEE_BigInt *)TEE_Malloc(name##_size * sizeof(TEE_BigInt), 0); \
	TEE_BigIntInit(name, name##_size)

/*
 * DEL_BIGINT frees the BigInt.
 */
#define DEL_BIGINT(name) TEE_Free(name)

/*
 * TB_PRINT_BIGINT prints the mpanum in base 16.
 */
#define TB_PRINT_BIGINT(n)                                                     \
do {                                                                           \
	char *str;                                                             \
	str = TEE_BigIntConvertToString(NULL, TEE_STRING_MODE_HEX_UC, 0, (n)); \
	printf("%s\n", str);                                                   \
	TEE_Free(str);                                                         \
} while (0)

#endif

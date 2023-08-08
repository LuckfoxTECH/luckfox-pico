/*
 * wpa_supplicant/hostapd - Default include files
 * Copyright (c) 2005-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 *
 * This header file is included into all C files so that commonly used header
 * files can be selected with OS specific ifdef blocks in one place instead of
 * having to have OS/C library specific selection in many files.
 */

#ifndef INCLUDES_H
#define INCLUDES_H

/* Include possible build time configuration before including anything else */
#include "build_config.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32_WCE
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#endif /* _WIN32_WCE */
#include <ctype.h>

#ifndef _MSC_VER
#include <unistd.h>
#endif /* _MSC_VER */

#ifndef CONFIG_NATIVE_WINDOWS
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifndef __vxworks
#include <sys/time.h>
#include <sys/uio.h>
#endif /* __vxworks */
#endif /* CONFIG_NATIVE_WINDOWS */

#endif /* INCLUDES_H */

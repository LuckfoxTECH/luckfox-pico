/*
 * Standard names for various quantities.
 *
 * These are:
 *
 * 1. for historical reasons
 * 2. to handle differences between BSD or Linux and Windows
 *
 * ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the MPEG TS, PS and ES tools.
 *
 * The Initial Developer of the Original Code is Amino Communications Ltd.
 * Portions created by the Initial Developer are Copyright (C) 2008
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Amino Communications Ltd, Swavesey, Cambridge UK
 *
 * ***** END LICENSE BLOCK *****
 */

#ifndef _compat
#define _compat

#ifdef _WIN32

// Kill deprecation warnings
#pragma warning(4 : 4996)

#include <io.h>

// Windows doesn't seem to supply <stdint.h>, so we shall have to try
// for values we hope work
typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;

typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef uint8_t byte;

// On BSD, lseek takes a 64-bit off_t value
// On Linux, if the system supports long files, it does the same
// On Windows, one has the choice of _lseek or _lseeki64
#define lseek _lseeki64

// MS Visual C 2003 for .Net defines off_t in sys/types.h as "long"
// I want to use the same name for my file offsets on Windows and Unix,
// but I also want to use a 64 bit quantity. So:
typedef __int64 offset_t;

// On Windows, printf supports %lld but only uses 32 bits of the input value,
// which leads to confusing results. Correct representation of 64 bit integers,
// requires the use of %I64d, which is suitable for printing out offset_t
#define OFFSET_T_FORMAT "%I64d"
#define OFFSET_T_FORMAT_8 "%8I64d"
#define OFFSET_T_FORMAT_08 "%08I64d"

// Whilst we're at it, define the format for a 64 bit integer as such
#define LLD_FORMAT "%I64d"
#define LLU_FORMAT "%I64u"
#define LLD_FORMAT_STUMP "I64d"
#define LLU_FORMAT_STUMP "I64u"

// The MSDN documentation for Visual Studio seems to indicate that
// the low-level "names" for stdin, etc., are not STDIN_FILENO, etc.,
// but are instead stdin, etc.
// This seems to naturally be confusing with the C terms stdin, etc.
// It *may* be that they actually are not distinct. However, the *numbers*
// follow the normal definitions.
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

// On Windows, "inline" is a C++ only keyword. In C, it is:
#define inline __inline

// Miscellaneous other Windows-related issues...
#define snprintf _snprintf

#else // _WIN32
// Other than on Windows, using the C99 integer definitions is what people
// expect, so do so
#include <stdint.h>

// Keep "byte" for historical/affectionate reasons
typedef uint8_t byte;

// lseek on BSD/Linux uses an off_t quantity to specify the required
// position. Where 64 bit file positions are supported, this is a 64 bit
// value. Unfortunately, Windows has off_t defined as being a long.
// For compatibility, therefore, we define a type that can be used on
// both Windows and Unix
// NB: On some systems, off_t is provided by unistd.h, but on some others
//     it may also be necessary to explicitly include sys/types.h. We shall
//     do both, here, for safety.
#include <sys/types.h>
#include <unistd.h>
typedef off_t offset_t;

#if defined(__linux__) && !defined(__USE_FILE_OFFSET64)
// If Linux does not have 64 bit support built in, then our offsets will
// be just 32 bit integers
#define OFFSET_T_FORMAT "%ld"
#define OFFSET_T_FORMAT_08 "%08ld" // deprecated, because it looks like hex/octal
#define OFFSET_T_FORMAT_8 "%8ld"
#else
// On Unices, printf supports %lld for 64 bit integers, and this is suitable
// for printing out offset_t when it is 64 bit
#define OFFSET_T_FORMAT "%lld"
#define OFFSET_T_FORMAT_08 "%08lld" // deprecated, because it looks like hex/octal
#define OFFSET_T_FORMAT_8 "%8lld"
#endif

// Whilst we're at it, define the format for a 64 bit integer as such
#define LLD_FORMAT "%lld"
#define LLU_FORMAT "%llu"
#define LLD_FORMAT_STUMP "lld"
#define LLU_FORMAT_STUMP "llu"

// Useful macros, but not side-effect free
#define max(i, j) ((i) > (j) ? (i) : (j))
#define min(i, j) ((i) < (j) ? (i) : (j))
#endif // WIN32

// Other useful things

typedef void *void_p;

#define TRUE 1
#define FALSE 0

// The following defaults are common, and it's difficult
// to decide which other header file they might belong in
#define DEFAULT_VIDEO_PID 0x68
#define DEFAULT_AUDIO_PID 0x67
#define DEFAULT_PMT_PID 0x66

#endif /* _compat */

// Local Variables:
// tab-width: 8
// indent-tabs-mode: nil
// c-basic-offset: 2
// End:
// vim: set tabstop=8 shiftwidth=2 expandtab:

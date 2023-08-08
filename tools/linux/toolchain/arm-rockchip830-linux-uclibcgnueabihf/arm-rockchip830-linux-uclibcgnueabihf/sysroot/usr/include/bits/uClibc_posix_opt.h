/*
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */
/*
 * Never include this file directly; use <unistd.h> instead.
 */

#ifndef	_BITS_UCLIBC_POSIX_OPT_H
#define	_BITS_UCLIBC_POSIX_OPT_H	1

/* This file works correctly only if posix_opt.h is the NPTL version */
#ifndef _POSIX_THREADS
# error posix_opt.h was incorrectly updated, use the NPTL version from glibc
#endif

/* change first options based on what glibc does */

/* this has to be adapted to uClibc, not all are thread related */

/* were in earlier version, used by sysconf */
#define	_POSIX_POLL	1
#define	_POSIX_SELECT	1

/* disable independently unsupported features */
#undef _POSIX_TRACE
#undef _POSIX_TRACE_EVENT_FILTER
#undef _POSIX_TRACE_INHERIT
#undef _POSIX_TRACE_LOG
#undef _POSIX_TYPED_MEMORY_OBJECTS
#undef _POSIX_SPAWN

#if 0 /* does uClibc support these? */
# undef _POSIX_ASYNCHRONOUS_IO
# undef _POSIX_ASYNC_IO
# undef _LFS_ASYNCHRONOUS_IO
# undef _POSIX_PRIORITIZED_IO
# undef _LFS64_ASYNCHRONOUS_IO
# undef _POSIX_MESSAGE_PASSING
#endif

/* change options based on uClibc config options */

#if 0 /*ndef __UCLIBC_HAS_POSIX_TIMERS__*/
# undef _POSIX_TIMERS
# undef _POSIX_THREAD_CPUTIME
#endif

#if 0 /*ndef __UCLIBC_HAS_POSIX_BARRIERS__*/
# undef _POSIX_BARRIERS
#endif

#if 0 /*ndef __UCLIBC_HAS_POSIX_SPINLOCKS__*/
# undef _POSIX_SPIN_LOCKS
#endif

#endif /* bits/uClibc_posix_opt.h */

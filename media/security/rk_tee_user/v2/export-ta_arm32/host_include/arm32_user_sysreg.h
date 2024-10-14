/* Automatically generated, do not edit */
#ifndef __ARM32_USER_SYSREG_H
#define __ARM32_USER_SYSREG_H
#include <compiler.h>
/* Based on register description in */
/* ARM Architecture Reference Manual */
/* ARMv7-A and ARMv7-R edition */
/* Issue C.c */

/* B8.2 Generic Timer registers summary */

/* Counter Frequency register */
static inline __noprof uint32_t read_cntfrq(void)
{
	uint32_t v;

	asm volatile ("mrc p15, 0, %0, c14, c0, 0" : "=r"  (v));

	return v;
}

/* Counter Frequency register */
static inline __noprof void write_cntfrq(uint32_t v)
{
	asm volatile ("mcr p15, 0, %0, c14, c0, 0" : : "r"  (v));
}

/* Physical Count register */
static inline __noprof uint64_t read_cntpct(void)
{
	uint64_t v;

	asm volatile ("mrrc p15, 0, %Q0, %R0, c14" : "=r"  (v));

	return v;
}

/* Physical Count register */
static inline __noprof void write_cntpct(uint64_t v)
{
	asm volatile ("mcrr p15, 0, %Q0, %R0, c14" : : "r"  (v));
}
#endif /*__ARM32_USER_SYSREG_H*/

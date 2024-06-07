#ifndef _RWNX_VERSION_H_
#define _RWNX_VERSION_H_

#include "rwnx_version_gen.h"

static inline void rwnx_print_version(void)
{
	AICWFDBG(LOGINFO, RWNX_VERS_BANNER"\n");
	AICWFDBG(LOGINFO, "RELEASE_DATE:%s \r\n", RELEASE_DATE);
}

#endif /* _RWNX_VERSION_H_ */

#ifndef SBC_TYPES_H
#define SBC_TYPES_H

#ifdef BUILDCFG
#include "bt_target.h"
#endif
#include "data_types.h"

/*TYPEDEFS*/

typedef short SINT16;
typedef INT32 SINT32;

#if (SBC_IPAQ_OPT == TRUE)

#if (SBC_FOR_EMBEDDED_LINUX == TRUE)
typedef long long SINT64;
#else
typedef __int64 SINT64;
#endif

#endif

#endif

#pragma once

#ifdef  __ICCARM__
#include  "platform_toolchain_iar.h"

#elif defined(__GNUC__)
#include  "platform_toolchain_gcc.h"
#else
#error toolchain error
#endif
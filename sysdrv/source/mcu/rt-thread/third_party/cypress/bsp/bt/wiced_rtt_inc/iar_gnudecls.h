#pragma once

#include <string.h>

#define strlcpy(dst, src, len) strcpy(dst, src)
#define strlcat(dst, tail, len) strcpy(dst, tail)
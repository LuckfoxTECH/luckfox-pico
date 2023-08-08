#ifndef _COMMON_UTILS_H
#define _COMMON_UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned long GetTimeStampMS();

void StrUInt16Array(char *str, int size, uint16_t *array, int num);

int ReadDataFile(const char *path, char **out_data);

int WriteDataToFile(const char *path, const char *data, unsigned int size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/time.h>

#include "common_utils.h"

unsigned long GetTimeStampMS() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void StrUInt16Array(char *str, int size, uint16_t *array, int num) {
  memset(str, 0, size);
  char str_tmp[size];
  for (int i = 0; i < num; i++) {
    memset(str_tmp, 0, size);
    snprintf(str_tmp, size, "%d", array[i]);
    strncat(str, str_tmp, size);
    if (i != num - 1) {
      strncat(str, ",", size);
    }
  }
}

int ReadDataFile(const char *path, char **out_data) {
  FILE *fp = fopen(path, "rb");
  if (fp == NULL) {
    printf("fopen %s fail!\n", path);
    return -1;
  }
  fseek(fp, 0, SEEK_END);
  int file_size = ftell(fp);
  char *data = (char *)malloc(file_size);
  fseek(fp, 0, SEEK_SET);
  if (file_size != fread(data, 1, file_size, fp)) {
    printf("fread %s fail!\n", path);
    free(data);
    return -1;
  }
  if (fp) {
    fclose(fp);
  }
  *out_data = data;
  return file_size;
}

int WriteDataToFile(const char *path, const char *data, unsigned int size) {
  FILE *fp;

  fp = fopen(path, "w");
  if (fp == NULL) {
    printf("open error: %s\n", path);
    return -1;
  }

  fwrite(data, 1, size, fp);
  fflush(fp);

  fclose(fp);
  return 0;
}
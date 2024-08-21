// Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __FILE_H__
#define __FILE_H__

#include <stdio.h>


FILE *osi_fopen(const char *path, const char *mode);

size_t osi_fwrite(const void *buffer, size_t size, size_t count, FILE *stream);

int osi_fclose(FILE *stream);

size_t osi_fread(void *buffer, size_t size, size_t count, FILE *stream);

int osi_open(const char *file, int flags);

int osi_write(int fd, const void *buf, size_t len);

int osi_close(int fd);

int osi_read(int fd, void *buf, size_t len);

int osi_unlink(const char *pathname);

int osi_stat(const char *file, struct stat *buf);

int osi_file_size_get(const char *filename);

int osi_file_erase_all_data(const char *filename);

#endif //__FILE_H__

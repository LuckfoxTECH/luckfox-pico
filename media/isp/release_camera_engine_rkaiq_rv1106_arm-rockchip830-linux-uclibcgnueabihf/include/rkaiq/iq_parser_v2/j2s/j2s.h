/*
 *  Copyright (c) 2020, Rockchip Electronics Co., Ltd
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 */

#ifndef J2S_H
#define J2S_H

#include "cJSON.h"
#include "common.h"

typedef struct {
    char name[MAX_NAME];
    uint8_t type;
    uint8_t flags;

    uint32_t offset; /* Offset in the parent struct */
    uint32_t elem_size; /* Elem size, e.g. char * for char ** */
    uint16_t num_elem; /* Elem num, e.g 3 for int [3] */
    uint32_t base_elem_size; /* Base elem size, e.g. char for char ** */

    int16_t next_index; /* Next child's index of the parent */

    int16_t struct_index; /* For struct type only */
    int16_t enum_index; /* For enum type only */
    int16_t len_index; /* For dynamic array only */
} __attribute__((packed)) j2s_obj;

typedef struct {
    char name[MAX_NAME]; /* Struct name */
    int16_t child_index; /* First child's index */
} __attribute__((packed)) j2s_struct;

typedef struct {
    char name[MAX_NAME]; /* Enum value name */
    int32_t value; /* Enum value */
} __attribute__((packed)) j2s_enum_value;

typedef struct {
    char name[MAX_NAME]; /* Enum name */
    int16_t value_index; /* First value's index */
    int16_t num_value; /* Number of enum values */
} __attribute__((packed)) j2s_enum;

typedef struct __map_index {
  void *dst_offset;
  void *ptr_offset;
  size_t len;
} map_index_t;

typedef struct _j2s_pool {
  uint8_t *data;
  size_t size;
  size_t used;
  map_index_t *maps_list;
  size_t map_len;
} j2s_pool_t;

typedef struct {
    /* Random magic number */
    int magic;

    /* Parsed members */
    int num_obj;
    j2s_obj* objs;

    /* Parsed structs */
    int num_struct;
    j2s_struct* structs;

    /* Parsed enums */
    int num_enum;
    j2s_enum* enums;

    /* Parsed enum values*/
    int num_enum_value;
    j2s_enum_value* enum_values;

    /* Parsed member desc */
    int num_desc;
    const char** descs;

    int root_index; /* Root struct's index */

    bool format_json; /* Generate formatted JSON */
    bool dump_desc; /* Dump desc when dumping structs */
    bool dump_enums; /* Dump enum info when dumping structs */
    bool manage_data; /* Free allocated data in deinit stage */

    void* priv; /* Private data */
} __attribute__((packed)) j2s_ctx;

/* Helpers for alloc/free ptr */
void* j2s_alloc_data(j2s_ctx* ctx, size_t size);
void *j2s_alloc_data2(j2s_ctx *ctx, size_t size, size_t* real_size);
int j2s_alloc_map_record(j2s_ctx *ctx, void *dst, void *ptr, size_t len);
int j2s_add_data(j2s_ctx* ctx, void* ptr, bool freeable);
void j2s_release_data(j2s_ctx* ctx, void* ptr);

/* Init/deinit j2s_ctx */
void j2s_init(j2s_ctx* ctx);
void j2s_camgroup_init(j2s_ctx* ctx);
void j2s_deinit(j2s_ctx* ctx);

/* Get size of struct */
int j2s_struct_size(j2s_ctx* ctx, int struct_index);

/* Get name of j2s type */
const char* j2s_type_name(j2s_type type);

cJSON* j2s_struct_to_template_json(j2s_ctx* ctx, const char* name);

/* Dump root struct to template cJSON */
#define j2s_root_struct_to_template_json(ctx) \
    j2s_struct_to_template_json(ctx, NULL)

cJSON* j2s_enums_to_json(j2s_ctx* ctx);
cJSON* j2s_struct_to_json(j2s_ctx* ctx, const char* name, void* ptr);
int j2s_json_to_struct(j2s_ctx* ctx, cJSON* json, const char* name, void* ptr);
int j2s_json_from_struct(j2s_ctx* ctx, cJSON* json, const char* name,
    void* ptr);
void j2s_struct_to_cache(j2s_ctx* ctx, const char* name, int fd, void* ptr);
int j2s_struct_from_cache(j2s_ctx* ctx, const char* name, int fd, void* ptr);

int j2s_struct_free(j2s_ctx* ctx, const char* name, void* ptr);

int j2s_struct_to_bin(j2s_ctx *ctx, int struct_index, void *ptr,
                      void *dts_ptr);

int j2s_calib_to_bin(j2s_ctx *ctx, const char *name, void *ptr,
                     void* bin_buf);

/* Dump root struct to cJSON */
#define j2s_root_struct_to_json(ctx, ptr) j2s_struct_to_json(ctx, NULL, ptr)

/* Apply cJSON to root struct */
#define j2s_json_to_root_struct(ctx, json, ptr) \
    j2s_json_to_struct(ctx, json, NULL, ptr)

/* Query cJSON from root struct */
#define j2s_json_from_root_struct(ctx, json, ptr) \
    j2s_json_from_struct(ctx, json, NULL, ptr)

/* Store root struct to cache fd */
#define j2s_root_struct_to_cache(ctx, fd, ptr) \
    j2s_struct_to_cache(ctx, NULL, fd, ptr)

/* Restore root struct from cache fd */
#define j2s_root_struct_from_cache(ctx, fd, ptr) \
    j2s_struct_from_cache(ctx, NULL, fd, ptr)

/* Read file content to buf */
void* j2s_read_file(const char* file, size_t* size);

/* Apply JSON file to struct */
int j2s_json_file_to_struct(j2s_ctx* ctx, const char* file, const char* name,
    void* ptr);

/* Apply JSON file to root struct */
#define j2s_json_file_to_root_struct(ctx, file, ptr) \
    j2s_json_file_to_struct(ctx, file, NULL, ptr)

char* j2s_dump_struct(j2s_ctx* ctx, const char* name, void* ptr);

/* Load/save root struct to cache file */
int j2s_load_struct_cache(j2s_ctx* ctx, const char* cache_file, void* ptr,
    void* auth_data, int auth_size);
void j2s_save_struct_cache(j2s_ctx* ctx, const char* cache_file, void* ptr,
    void* auth_data, int auth_size);
#define j2s_load_cache(ctx, cache_file, ptr) \
    j2s_load_struct_cache(ctx, cache_file, ptr, NULL, 0)
#define j2s_save_cache(ctx, cache_file, ptr) \
    j2s_save_struct_cache(ctx, cache_file, ptr, NULL, 0)

/* Dump root struct to JSON */
#define j2s_dump_root_struct(ctx, ptr) j2s_dump_struct(ctx, NULL, ptr)

char* j2s_dump_template_struct(j2s_ctx* ctx, const char* name);

/* Dump root struct to template JSON */
#define j2s_dump_template_root_struct(ctx) j2s_dump_template_struct(ctx, NULL)

/* Apply JSON to struct */
int j2s_modify_struct(j2s_ctx* ctx, const char* str, const char* name,
    void* ptr);

/* Query JSON from root struct */
char* j2s_query_struct(j2s_ctx* ctx, const char* str, void* ptr);

typedef struct {
    const char* name; /* Struct name */
    void* ptr; /* Struct instance */
} __attribute__((packed)) j2s_struct_info;

/* Dump structs to JSON, info should end with {NULL, NULL} */
char* j2s_dump_structs(j2s_ctx* ctx, j2s_struct_info* info);

#endif // J2S_H

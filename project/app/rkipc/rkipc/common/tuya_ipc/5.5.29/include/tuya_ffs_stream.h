
#ifndef __TUYA_FFS_STREAM_H__
#define __TUYA_FFS_STREAM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_com_defs.h"
#include "tuya_cloud_types.h"

typedef struct {
	CHAR_T *data;
	INT_T data_size; /* number of byte writen */
	INT_T max_size;  /* maximnm data size.*/
} tuya_ffs_stream_t;

#define TUYA_FFS_TEMPORARY_STREAM(name, size)                                                      \
	CHAR_T name##data[size] = {0};                                                                 \
	tuya_ffs_stream_t name;                                                                        \
	memset(&name, 0x00, sizeof(name));                                                             \
	name.data = name##data;                                                                        \
	name.max_size = size;

tuya_ffs_stream_t *tuya_ffs_stream_new(INT_T buf_len);

OPERATE_RET tuya_ffs_stream_free(tuya_ffs_stream_t *stream);

OPERATE_RET tuya_ffs_stream_write(tuya_ffs_stream_t *stream, UINT8_T *data, UINT_T len);

OPERATE_RET tuya_ffs_stream_bind_data(tuya_ffs_stream_t *stream, UINT8_T *data, UINT_T len);

OPERATE_RET tuya_ffs_stream_flush(tuya_ffs_stream_t *stream);

INT_T tuya_ffs_stream_get_space_size(const tuya_ffs_stream_t *stream);

CHAR_T *tuya_ffs_stream_get_write_pointer(tuya_ffs_stream_t *stream);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __TUYA_FFS_STREAM_H__

#ifndef __RK_IIO_FIFO_
#define __RK_IIO_FIFO_

#ifdef __cplusplus
extern "C" {
#endif

#include "rkiio_common.h"
#include "format.h"

typedef void *RKIIO_FIFO_HANDLE;
typedef void *RKIIO_FIFO_DATA;
typedef void *RKIIO_FIFO_DEV;

#define RKIIO_TIMESTAMP_MONOTONIC "monotonic"
#define RKIIO_TIMESTAMP_REALTIME "realtime"

typedef struct xyz_data_s {
    double x;
    double y;
    double z;
} xyz_data_t;

typedef struct rkiio_data_0_s {
    RK_U64 id;
    xyz_data_t anglvel;
    xyz_data_t accel;
    RK_S32 temp;
    RK_U64 timestamp;
} rkiio_data_0_t;

RKIIO_FIFO_HANDLE rkiio_fifo_handle_create(RKIIO_FIFO_DEV dev, RK_S32 last_index, struct rkiio_format_info *format_info, RK_U8 info_num, RK_U32 buf_max_num, double max_sampling_rate);
RK_S32 rkiio_fifo_list_len_get(RKIIO_FIFO_HANDLE fifo_handle, RK_U32 *len);
RK_S32 rkiio_fifo_handle_destroy(RKIIO_FIFO_HANDLE fifo_handle);
RK_S32 rkiio_fifo_handle_last_id_get(RKIIO_FIFO_HANDLE fifo_handle, RK_U64 *id);
RK_S32 rkiio_fifo_refill_once(RKIIO_FIFO_HANDLE fifo_handle);
RK_S32 rkiio_fifo_data_read_all(RKIIO_FIFO_HANDLE fifo_handle, RKIIO_FIFO_DATA fifo_data, RK_U64 *frame_id, RK_U64 *data_num);
RK_S32 rkiio_fifo_data_read_num(RKIIO_FIFO_HANDLE fifo_handle, RKIIO_FIFO_DATA fifo_data, RK_U64 *frame_id, RK_U64 data_num);
RK_S32 rkiio_fifo_data_release(RKIIO_FIFO_DATA buf);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __RK_IIO_FIFO_
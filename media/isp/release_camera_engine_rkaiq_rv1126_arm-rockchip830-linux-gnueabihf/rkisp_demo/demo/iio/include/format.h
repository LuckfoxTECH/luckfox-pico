#ifndef __FORMAT_H_
#define __FORMAT_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "rkiio_common.h"

// read each start
// struct iio_data_format;
struct rkiio_format_info {
    RK_U64 index;
    RK_BOOL vaild;
    RK_U32 storagebits;
    RK_U32 realbits;
    RK_BOOL is_signed;
    RK_BOOL is_be;
    RK_U32 shift;
    RK_BOOL with_scale;
    RK_DOUBLE scale;
};

RK_S32 rk_iio_format_fifo_data_u64(void *buf, struct rkiio_format_info format_info, RK_U64 *rst);
RK_S32 rk_iio_format_fifo_data_double(void *buf, struct rkiio_format_info format_info, double *rst);
RK_S32 rk_iio_format_fifo_data_s32(void *buf, struct rkiio_format_info format_info, RK_S32 *rst);
RK_S32 rk_iio_scale_option_get(const char *scale_str, double **data_list, RK_U8 *data_num);
RK_S32 rk_iio_scale_option_match(const char *scale_str, double match_data, char *buf, RK_U32 buf_len);
RK_S32 rk_iio_scale_option_release(double *data_list);

#ifdef __cplusplus
}
#endif

#endif

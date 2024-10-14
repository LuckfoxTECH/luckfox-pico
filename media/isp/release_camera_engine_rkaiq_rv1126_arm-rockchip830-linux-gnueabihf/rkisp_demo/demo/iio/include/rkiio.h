#ifndef __RK_IIO_
#define __RK_IIO_

#ifdef __cplusplus
extern "C" {
#endif

#include "rkiio_common.h"
#include "rkiio_fifo.h"

#define RK_IIO_MAX_DEV 4
#define RK_IIO_MAX_SENSOR 16
#define RK_IIO_STRING_MAX_SIZE 512

typedef void *RKIIO_DATA_HANDLE;

typedef enum rkiio_sensor_type_e {
  RKIIO_SENSOR_INVALID_TYPE = 0,
  RKIIO_SENSOR_ANGLVEL_TYPE,
  RKIIO_SENSOR_ACCEL_TYPE,
  RKIIO_SENSOR_TEMP_TYPE,
  RKIIO_SENSOR_ALL_TYPE, // only for get data
  RKIIO_SENSOR_MAX_TYPE,
} rkiio_sensor_type_t;

_CAPI void rk_libiio_version_info_dump();
_CAPI void rkiio_err_dump(const RK_CHAR *tip, RK_S32 ret);
_CAPI void rkiio_info_dump();
_CAPI RK_S32 rkiio_sys_init();
_CAPI RK_S32 rkiio_sys_uninit();
_CAPI RK_CHAR **rkiio_dev_list_get(rkiio_sensor_type_t type, RK_S32 *dev_num);
_CAPI RK_CHAR **rkiio_anglvel_dev_list_get(RK_S32 *dev_num);
_CAPI RK_CHAR **rkiio_accel_dev_list_get(RK_S32 *dev_num);
_CAPI RK_CHAR **rkiio_temp_dev_list_get(RK_S32 *dev_num);
_CAPI RK_CHAR **rkiio_all_dev_list_get(RK_S32 *dev_num);
_CAPI RK_S32 rkiio_dev_list_destroy(char **dev_list,  RK_S32 dev_num);

// config
_CAPI RK_S32 rkiio_timestamp_type_read(RK_CHAR *dev_name, RK_CHAR *type, RK_U32 len);
_CAPI RK_S32 rkiio_timestamp_type_set(RK_CHAR *dev_name, RK_CHAR *type);
_CAPI RK_S32 rkiio_timestamp_type_set_all(RK_CHAR *type);
_CAPI RK_S32 rkiio_sampling_rate_cap_get(RK_CHAR *dev_name, rkiio_sensor_type_t type, double **cap_list, RK_U8 *cap_num);
_CAPI RK_S32 rkiio_sampling_rate_cap_release(double *cap_list);
_CAPI RK_S32 rkiio_sampling_rate_get(RK_CHAR *dev_name, rkiio_sensor_type_t type, double *rate_num);
_CAPI RK_S32 rkiio_sampling_rate_set(RK_CHAR *dev_name, rkiio_sensor_type_t type, double rate_num);
_CAPI RK_S32 rkiio_sampling_rate_set_all(RK_CHAR *dev_name, RK_CHAR *rate);

// data
_CAPI RKIIO_DATA_HANDLE rkiio_data_handle_create(RK_CHAR *dev_name, RK_U32 data_idx, RK_U32 max_data_num);
_CAPI RK_S32 rkiio_data_handle_destroy(RKIIO_DATA_HANDLE data_handle);
_CAPI RK_S32 rkiio_data_handle_max_fifo_num_get(RKIIO_DATA_HANDLE data_handle, RK_U32 *data_num);
_CAPI RK_S32 rkiio_data_get_all(RKIIO_DATA_HANDLE data_handle, RKIIO_FIFO_DATA buf, RK_U64 *data_num);
_CAPI RK_S32 rkiio_data_read(RKIIO_DATA_HANDLE data_handle, RKIIO_FIFO_DATA buf, RK_U64 data_num);
_CAPI RK_S32 rkiio_fifo_data_destroy(RKIIO_FIFO_DATA fifo_data);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __RK_IIO_
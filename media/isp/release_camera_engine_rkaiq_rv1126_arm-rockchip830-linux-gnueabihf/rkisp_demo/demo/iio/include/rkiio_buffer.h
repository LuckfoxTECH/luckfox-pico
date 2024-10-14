#ifndef __RKIIO_BUFFER_
#define __RKIIO_BUFFER_

#include <pthread.h>
#include "rkiio_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *RKIIO_BUFFER;
typedef void *RKIIO_BUFFER_CELL;

// cell_id_mark - top_buffer.id 
typedef void (*rkiio_buffer_mark_get)(RKIIO_BUFFER_CELL buffer, RK_U64 buf_id, RK_U64 *mark);

struct rkiio_buffer_list_info {
    RK_U64 u64MaxSize;
    RK_U64 u64CellSize;
    RK_BOOL bFullStop;
    rkiio_buffer_mark_get getId;
};

RKIIO_BUFFER rkiio_buffer_list_create(struct rkiio_buffer_list_info buffer_info);
RK_S32 rkiio_buffer_list_destroy(RKIIO_BUFFER rkiio_buf);
RK_S32 rkiio_buffer_list_push(RKIIO_BUFFER rkiio_buf, RKIIO_BUFFER_CELL ib);
RK_S32 rkiio_buffer_list_pop(RKIIO_BUFFER rkiio_buf, RKIIO_BUFFER_CELL ib);
RKIIO_BUFFER_CELL rkiio_buffer_list_full_copy(RKIIO_BUFFER rkiio_buf, RK_U64 *cell_id_mark, RK_U64 *copy_num);
RKIIO_BUFFER_CELL rkiio_buffer_list_copy(RKIIO_BUFFER rkiio_buf, RK_U64 *cell_id_mark, RK_U64 *copy_num, RK_U64 max_copy_num);
RK_S32 rkiio_buffer_cell_release(RKIIO_BUFFER_CELL ib_list);

RK_S32 _rkiio_buffer_list_full_copy(RKIIO_BUFFER rkiio_buf, RKIIO_BUFFER_CELL ib_list, RK_U64 *cell_id_mark, RK_U64 *copy_num);
RK_S32 _rkiio_buffer_list_copy(RKIIO_BUFFER rkiio_buf, RKIIO_BUFFER_CELL ib_list, RK_U64 *cell_id_mark, RK_U64 *copy_num, RK_U64 max_copy_num);

#ifdef __cplusplus
}
#endif

#endif // #ifndef __RKIIO_BUFFER_
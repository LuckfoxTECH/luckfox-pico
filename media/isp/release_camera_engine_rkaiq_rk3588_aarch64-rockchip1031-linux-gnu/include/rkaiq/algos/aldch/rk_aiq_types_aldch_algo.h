#ifndef __RK_AIQ_TYPES_ALDCH_ALGO_H__
#define __RK_AIQ_TYPES_ALDCH_ALGO_H__

#define LDCH_MESH_XY_NUM_ALGO 524288 // ((4096/16) * (4096/8))*4

typedef struct {
    unsigned char update;
    unsigned int sw_ldch_en;
    unsigned int lut_h_size;
    unsigned int lut_v_size;
    unsigned int lut_map_size;
    //unsigned short lut_mapxy[LDCH_MESH_XY_NUM_ALGO];
    unsigned int lut_mapxy_buf_fd;
} ldch_process_result_t;;

#endif

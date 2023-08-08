#ifndef __RK_AIQ_TYPES_ALDCH_ALGO_H__
#define __RK_AIQ_TYPES_ALDCH_ALGO_H__

#define LDCH_MESH_XY_NUM_ALGO 524288 // ((4096/16) * (4096/8))*4

#ifndef ISP32_LDCH_BIC_NUM
#define ISP32_LDCH_BIC_NUM		36
#endif

typedef struct {
    unsigned char update;
    unsigned int sw_ldch_en;
    unsigned int lut_h_size;
    unsigned int lut_v_size;
    unsigned int lut_map_size;
    //unsigned short lut_mapxy[LDCH_MESH_XY_NUM_ALGO];
    unsigned int lut_mapxy_buf_fd;
} ldch_process_result_t;

typedef struct {
    ldch_process_result_t base;

    unsigned char frm_end_dis;
    unsigned char zero_interp_en;
    unsigned char sample_avr_en;
    unsigned char bic_mode_en;
    unsigned char force_map_en;
    unsigned char map13p3_en;

    unsigned char bicubic[ISP32_LDCH_BIC_NUM];
} ldch_v21_process_result_t;

#endif

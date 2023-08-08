#ifndef __RK_AIQ_TYPES_AFEC_ALGO_H__
#define __RK_AIQ_TYPES_AFEC_ALGO_H__

#define FEC_MESH_XY_NUM_ALGO 524288

typedef struct {
    unsigned char update;
    unsigned char sw_fec_en;
    unsigned char crop_en;
    unsigned int crop_width;
    unsigned int crop_height;
    unsigned char mesh_density;
    unsigned int mesh_size;
    unsigned int mesh_buf_fd;
    //unsigned short meshxi[FEC_MESH_XY_NUM_ALGO];
    //unsigned char meshxf[FEC_MESH_XY_NUM_ALGO];
    //unsigned short meshyi[FEC_MESH_XY_NUM_ALGO];
    //unsigned char meshyf[FEC_MESH_XY_NUM_ALGO];
    int img_buf_index;
} fec_preprocess_result_t;

typedef enum fec_correct_direction_e {
    FEC_CORRECT_DIRECTION_X = 0x1,
    FEC_CORRECT_DIRECTION_Y,
    FEC_CORRECT_DIRECTION_XY
} fec_correct_direction_t;

typedef enum fec_correct_mode_e {
    FEC_COMPRES_IMAGE_KEEP_FOV = 0x1,
    FEC_KEEP_ASPECT_RATIO_REDUCE_FOV,
    FEC_ALTER_ASPECT_RATIO_KEEP_FOV
} fec_correct_mode_t;

#endif

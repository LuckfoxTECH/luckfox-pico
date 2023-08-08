#ifndef __RK_AIQ_ORB_ALGO_H__
#define __RK_AIQ_ORB_ALGO_H__

#define MAX_POINTS                  10000
#define DESCRIPTOR_SIZE             15
#define ORB_FEATURE_DESCRIPTOR_BITS 120
#define ORB_FEATURE_DUMMY_NUM       13

#include "rk_aiq_comm.h"

RKAIQ_BEGIN_DECLARE

typedef struct {
    unsigned char brief[DESCRIPTOR_SIZE];
    unsigned int x : 13;
    unsigned int y : 13;
    unsigned int dmy1 : 6;
    unsigned char dmy2[ORB_FEATURE_DUMMY_NUM];
} rk_aiq_orb_featue_point;

typedef struct {
    unsigned int num_points;
    rk_aiq_orb_featue_point points[MAX_POINTS];
    uint32_t frame_id;
    int img_buf_index;
    unsigned int img_size;
    int buf_fd[16];
    int buf_idx[16];
    int buf_num;
	void *buf_map;
} rk_aiq_orb_algo_stat_t;

typedef struct {
    unsigned char update;
    unsigned char orb_en;
    unsigned char limit_value;
    unsigned short max_feature;
} rk_aiq_orb_algo_meas_t;

typedef struct {
    unsigned short row;
    unsigned short col;
    unsigned char descriptor[DESCRIPTOR_SIZE];
} orb_point_t;

typedef struct {
    unsigned int row1;
    unsigned int col1;
    unsigned int row2;
    unsigned int col2;
    unsigned int score;
} orb_matched_point_t;

typedef struct {
    unsigned int left;
    unsigned int top;
    unsigned int right;
    unsigned int bottom;
    unsigned int width;
    unsigned int height;
} orb_rect_t;

typedef struct {
    unsigned char valid;
    orb_rect_t tracking_roi_rect;
    unsigned int matched_keypoints_num;
    orb_matched_point_t matched_keypoints[MAX_POINTS];
} orb_preprocess_result_t;

RKAIQ_END_DECLARE

#endif

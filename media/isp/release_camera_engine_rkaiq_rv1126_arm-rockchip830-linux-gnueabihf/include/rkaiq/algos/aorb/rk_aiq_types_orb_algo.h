#ifndef __RK_AIQ_ORB_ALGO_H__
#define __RK_AIQ_ORB_ALGO_H__

#define MAX_POINTS 40000
#define DESCRIPTOR_SIZE 15
#define ORB_FEATURE_DESCRIPTOR_BITS 120

typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned char brief[DESCRIPTOR_SIZE];
} rk_aiq_orb_featue_point;

typedef struct {
    unsigned int num_points;
    rk_aiq_orb_featue_point points[MAX_POINTS];
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

#endif

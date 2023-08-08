#ifndef _MOVE_DETECTION_H
#define _MOVE_DETECTION_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	unsigned short flag;      // 1 ��Ч�� 0��Ч
	unsigned short up_left[2];     // ���Ͻ����꣬�²���ǰ��0��y���꣬1��x����
	unsigned short down_right[2];   // ���½����꣬�²���ǰ��0��y���꣬1��x����

} INFO_LIST;

typedef struct {
	unsigned short flag;      // 1 ��Ч�� 0��Ч
	unsigned short is_move;   // �˶���־
	unsigned short up_left[2];     // �²���ǰ���Ͻ����꣬0��y���꣬1��x����
	unsigned short down_right[2];   // �²���ǰ���½����꣬0��y���꣬1��x����
} ROI_INFO;

struct md_ctx;

typedef struct {
	int still_threshold0;
	int still_threshold1;
	int pix_threshold;
	int reserved[32];
} MD_PARAMS;

struct md_ctx *move_detection_init(int width, int height, int width_ds, int height_ds, int is_single_ref);
int move_detection_set_params(struct md_ctx *ctx, MD_PARAMS param);
void move_detection_deinit(struct md_ctx *ctx);
void move_detection(struct md_ctx *ctx, void *downscale_frame_y, ROI_INFO * p_roi_in, INFO_LIST *info_list); 
int move_detection_set_sensitivity(struct md_ctx *ctx, int value);
#ifdef __cplusplus
} //extern "C"
#endif

#endif

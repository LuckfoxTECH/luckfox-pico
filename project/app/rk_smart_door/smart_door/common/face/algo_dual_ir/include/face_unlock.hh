
#ifndef __FACE_UNLOCK_HH__
#define __FACE_UNLOCK_HH__
#include "rknn_api.h"
//value is real int value
typedef struct BBoxRect
{
    float score;
	int xmin;
	int ymin;
	int xmax;
	int ymax;
	int xkey_point[5];
	int ykey_point[5];
	float point18_x[18];
	float point18_y[18];
	float yaw;
	float pitch;
	float roll;
	float angle;
	int in_ir_h;
	int in_ir_w;
	int in_speckle_h;
	int in_speckle_w;

} BBoxRect_t;

//value is ratio value
typedef struct BBox
{
    float score;
	float xmin;
	float ymin;
	float xmax;
	float ymax;
	float xkey_point[5];
	float ykey_point[5];
	float point18_x[18];
	float point18_y[18];
	///float yaw;
	//float pitch;
	//float roll;
} BBox_t;

typedef enum unlock_state_enum {
	RESULT_DETECT_FACE_OK,     //= 0,
	RESULT_NO_FACE,            //= 1,
	RESULT_BIG_ANGLE,          //= 2,
	RESULT_IR_FAIL,            //= 3,
	RESULT_DEPTH_FAIL,         //= 4,
	RESULT_TOO_CLOSE,          //= 5,
	RESULT_TOO_FAR,            //= 6,
	RESULT_TOO_LEFT,           //= 7,
	RESULT_TOO_RIGHT,          //= 8,
	RESULT_TOO_HIGH,           //= 9,
	RESULT_TOO_LOW,            //= 10,
	RESULT_NO_ID,              //= 11,
	RESULT_COMPARE_FACE_OK,    //= 12,
	RESULT_SAVE_FACE_OK,       //= 13,
	RESULT_SAVE_FACE_FAIL,     //= 14,
	RESULT_NULL,               //= 15,
	RESULT_FAS_OK,             //= 16,
	RESULT_FACE_EXIST,         //= 17,
	RESULT_REGISTER_FULL,      //= 18,
	RESULT_IR_OK,              //= 19
	RESULT_DEPTH_OK,           //= 20
	RESULT_DIR_ERROR,          //=21

} unlock_state_e;

typedef enum face_use_enum{
    REGISTER,
    COMPARE,
	SAME_ID_REGISTER,
	REGISTER_DIR_MIDDLE,
	REGISTER_DIR_LEFT,
	REGISTER_DIR_RIGHT,
	REGISTER_DIR_UP,
	REGISTER_DIR_DOWN,

} face_use_e;

typedef enum similar_enum{
    COSINE,
    EUCLIDEAN,
} similar_e;


typedef enum angle_enum{
	ANGLE_OK,
	YAW_LEFT,
	YAW_RIGHT,
	PITCH_UP,
	PITCH_DOWN,
	ROLL_LEFT,
	ROLL_RIGHT,
} angle_state_e;

typedef struct HeadPose{
	float pitch;
	float yaw;
	float roll;
	float angle;
	angle_state_e angle_state;
} head_pose_t;


typedef struct face_vec{
	int id;
	int num;
	int vec_flag[5];
	float vec[5][128];

} face_vec_t;





typedef struct detect_res{

	unlock_state_e unlock_state;
	BBoxRect_t best_box_rect;
	head_pose_t head_pose;

	double sharp_score;
	int detect_cnt;
	int long_dis_detect_cnt;
	int distance;

	float ir_score;
	bool  ir_res;
	float depth_score;
	bool  depth_res;
	int face_id;

	int eye_state;

	int fas_result[6];
	const char* failed_state = "pass";
	float *reg_vec;
	detect_res();

} detect_res_t;


class NPU_RV1106{
protected:
	//char* model_path;
	char* input_path;
	int loop_count;
	rknn_context ctx;
	rknn_input_output_num io_num;
	rknn_sdk_version sdk_ver;
	rknn_custom_string custom_string;

	rknn_tensor_attr *input_attrs;
	rknn_tensor_attr *output_attrs;

	rknn_tensor_mem* *input_mems;
	rknn_tensor_mem* *output_mems;

	float* *output_mems_float;
	rknn_tensor_attr *orig_output_attrs;

public:
	NPU_RV1106();
	~NPU_RV1106();
	int npu_init(char const * model_path);
	int model_bind(void * nn_addr, int size);
	int check_md5(void * nn_addr, int size, const char* path);
	int get_sdk_version();
	int get_weight_and_mem_size();
	int get_io_info();
	int set_loop(int num);
	int get_custom_string();
	int get_mem_io();
	int io_mem_init(rknn_tensor_type   input_type,rknn_tensor_format input_layout);

	float**  nn_run(uint8_t *input_data, bool nhwc = true);
	int nn_run(uint8_t *input_data, float ** &res);
	int NC1HWC2_to_NCHW(const int8_t* src, float* dst, int* dims, int channel, int hw_dst, int zp, float scale);
	int NC1HWC2_to_NHWC(const int8_t* src, float* dst, int* dims, int channel, int hw_dst, int zp, float scale);
	int NCHW_to_NHWC_K(float* src, float* dst, int n, int c, int h, int w);

	void dump_tensor_attr(rknn_tensor_attr* attr);
	unsigned char * load_image(const char* image_path, bool auto_resize = true, int bits=8);
	unsigned char * loadmodel(const char *filename, int *model_size);
	unsigned char * load_bin(const char *filename);

};


class Face_detection :public NPU_RV1106{
public:

	char const *model_path;
	char const *bin_anchor_path;
	float box_thres;
	float nms_thres;


	float *anchor_buf;

	float * scores;
	float * locations;
	BBox_t * bbox_list;;
	uint8_t* re_detect_img;

public:
	~Face_detection();
	int param_init(char const *bin_anchor_path, float box_thres, float nms_thres, int img_h, int img_w);
	int nn_init(void *addr, int size);

	int init(char const *model_path, char const *bin_anchor_path,float box_thres,float nms_thres,int img_h,int img_w);//
	int npu_res_decodes(float* *output_mems, float *scores, float * locations);
	int outputs_decode(float * anchor_buf, float * out_buf, float* decode_res);
	int points_to_directions(BBoxRect_t &best_box);
	float overlap_area(const BBox_t *a, const BBox_t *b);
	int normal_nms_sorted_bboxes(BBox_t *bboxes, bool *select_list, int n, float nms_threshold);
	int decode_to_bboxs(float * scores, float * locations, float*anchor_buf, BBox_t *bbox_list, bool * select_list, int bbox_num);
	int select_best_roi(uint8_t* img, const BBox_t *bboxes, bool *select_list, const int bbox_num,  BBoxRect_t *best_box_rect,int img_h,int img_w);

	detect_res_t run_detect_core(uint8_t  * img, int origin_h, int origin_w);
	detect_res_t run_multi_detect_core(uint8_t  * img, int origin_h, int origin_w);
	detect_res_t run_long_distance_redetect(uint8_t  * in_ir, int origin_h, int origin_w, detect_res_t det_res);

};

class Fas_ir :public NPU_RV1106{
public:
	float  fas_ir_thres;

public:
	int param_init(float fas_ir_thres);
	int nn_init(void * addr, int size);
	int init(char const *model_path,float fas_ir_thres);
	void set_threshold(float thres);

	detect_res_t run_fas_ir_core(uint8_t  * ir_face);
	detect_res_t run_judge_face_core(uint8_t  * ir_face);
	detect_res_t trandition_judge(uint8_t  * ir_face);
	detect_res_t run_face_landmarks(uint8_t  * ir_face);
};

class Fas_depth :public NPU_RV1106
{
public:
	float  fas_depth_thres;


public:
	int param_init(float fas_depth_thres);
	int nn_init(void * addr, int size);
	int init(char const *model_path,	float fas_depth_thres);
	void set_threshold(float thres);

	detect_res_t run_fas_depth_core(uint8_t  * depth_face);
	detect_res_t cal_depth_roi(BBoxRect &best_box_rect, int &cal_w, int &cal_h, int &pos_x, int &pos_y);
	void align_depth_face_to_255(uint16_t *align_depth_face, int depth_roi_median, uint8_t*align_depth_face_adj, int valid_range, bool en_filter);
	bool depth_fake_reject(uint16_t* img,int median, int h, int w);
};

class Face_recognition :public  NPU_RV1106
{
public:

	float     face_recog_thres;
	int       face_num_max;
	similar_e dis_mode;
	char const * face_feat_lib_path;
	face_vec_t face_buffer;

public:
	int param_init(char const * face_feat_lib_path, float face_recog_thres, similar_e dis_mode);
	int nn_init(void * addr, int size);
	int init(char const *model_path, char const * face_feat_lib_path, float recognition_thres,similar_e dis_mode);

	float cal_distance(float *a, float *b, int n, similar_e dis_type);

	int save_face(float *feature_vector, face_use_e use_state);
	int query_face_id(float *feature_vector);
	int query_face_max_id();
	bool query_face_id_exists(int tar_id);
	int query_face_num();
	void delete_face(int id);

	void set_threshold(float thres);

	detect_res_t regitser_face(float** face_recog_res, face_use_e use_state, int tar_id);

	int set_max_face_num(int num);

	detect_res_t  run_recog_core(uint8_t* recog_3c_img, face_use_e use_state, int tar_id);

};

class Face_headpose :public  NPU_RV1106
{

public:
	float yaw_th;
	float pitch_th;
	float roll_th;

	int nn_init(void * addr, int size);
	int init(char const *model_path);

	int param_init(float yaw_th, float pitch_th, float roll_th);
	int set_threshold(float yaw_th, float pitch_th, float roll_th);
	detect_res_t cal_headpose_box(BBoxRect &best_box_rect, int &min, int &ymin, int &xmax, int &ymax);
	int cut_headpose_img(uint8_t *src,int h, int w, uint8_t *tmp, int r_xmin, int r_ymin, int r_xmax, int r_ymax, uint8_t* dst, int dst_h, int dst_w);

	detect_res_t  run_headpose_core(uint8_t* pose_img);

};


class Fas_Eye_State :public NPU_RV1106{

public:
	float  fas_ir_thres;

public:
	int param_init(float fas_ir_thres);
	int nn_init(void * addr, int size);
	int init(char const *model_path, float fas_ir_thres);
	void set_threshold(float thres);

	detect_res_t run_eye_state_core(uint8_t  * ir_face);
	detect_res_t run_single_eye_state_core(uint8_t  * ir_face);
};

#endif

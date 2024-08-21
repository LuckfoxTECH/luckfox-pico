#ifndef __SMART_UNLOCK_HH__
#define __SMART_UNLOCK_HH__

#define IR_MANY 10

#ifdef __cplusplus
extern "C" {
#endif


typedef struct NN_ADDR{
	void *detect_nn_addr;
	int   detect_nn_size;
	void *fas_ir_nn_addr;
	int   fas_ir_nn_size;
	void *fas_depth_nn_addr;
	int   fas_depth_nn_size;
	void *recog_nn_addr;
	int   recog_nn_size;
	void  *headpose_nn_addr;
	int   headpose_nn_size;
	void  *eye_state1_nn_addr;
	int  eye_state1_nn_size;
	void * judge_face_nn_addr;
	int judge_face_nn_size;
	void *fas_ir_second_nn_addr;
	int fas_ir_second_nn_size;
	void *ir_landmarks_nn_addr;
	int ir_landmarks_nn_size;
	
	void *fas_ir_many_nn_addr[IR_MANY];
	int fas_ir_many_nn_size[IR_MANY];
} nn_addr_t;


class Smart_unlock{
private:
	int origin_ir_h;
	int origin_ir_w;
	int in_speckle_h;
	int in_speckle_w;
	int in_ir_h;
	int in_ir_w;
	int in_depth_h;
	int in_depth_w;

	uint8_t*  detect_mark_img;
	uint8_t*  headpose_cut_img;
	uint16_t* depth_img;

	

public:
	
	Face_detection   *face_detect;
	Fas_ir           *fas_ir;
	Fas_ir           *fas_ir_second;
	Fas_ir           *ir_landmarks;
	Fas_depth        *fas_depth;
	Face_recognition *face_recog;
	Face_headpose    *face_headpose;
	Fas_Eye_State    *fas_eye_state;
	Fas_ir           *judge_face;

	bool detect_init_flag;
	bool fas_ir_init_flag;
	bool fas_depth_init_flag;
	bool face_recog_init_flag;
	bool face_headpose_init_flag;
	bool fas_eye_state_init_flag;
	bool depth_cal_init_flag;
	bool judge_face_init_flag;
	bool fas_ir_second_init_flag;
	bool ir_landmarks_init_flag;

	Fas_ir * fas_ir_many[IR_MANY];
	bool fas_ir_many_init_flag[IR_MANY];

	Smart_unlock();
	void set_path(char const *  model_path, char const * face_path, char const * depth_path, char const * map_path);
	int unlock_init(int origin_ir_h, int origin_ir_w, int in_ir_h, int in_ir_w, int in_speckle_h, int in_speckle_w);
	int unlock_param_init(int origin_ir_h, int origin_ir_w, int in_ir_h, int in_ir_w, int in_speckle_h, int in_speckle_w);
	int unlock_nn_init(nn_addr_t nn_models);
	int unlock_face_ae_init(int in_ir_h, int in_ir_w, int in_speckle_h, int in_speckle_w, nn_addr_t nn_models);
	int unlock_face_speckle_ae_init(int in_ir_h, int in_ir_w, int in_speckle_h, int in_speckle_w, nn_addr_t nn_models);
	nn_addr_t unlock_nn_read();
	void unlock_nn_free(nn_addr_t nn_models);

	detect_res_t face_unlock_flow(char*origin_ir, char* in_ir, char* in_speckle, face_use_e use_state, int tar_id=-1);

	int set_threshold(float fas_ir, float fas_depth, float recognition);
	void delete_face(int id);
	void set_max_face_num(int num);
	void unlock_deinit();

	void cal_depth_roi(detect_res_t & detect_res, int &cal_w, int &cal_h, int &pos_x, int &pos_y);
	detect_res_t face_unlock_flow_core(char*origin_ir, char* in_ir, char* in_speckle, face_use_e use_state, int tar_id);

	detect_res_t face_unlock_det_face(char*origin_ir, char* in_ir);
	detect_res_t face_unlock_speckle_det_face(char*speckle,int h, int w);
	uint16_t     face_unlock_cal_dis(char* in_speckle, int pos_x, int pos_y, int cal_w, int cal_h);
	bool is_write_able(const char* path);
	int set_depth_ratio(int ratio);
	detect_res_t face_unlock_nodetect_flow(uint8_t* in_ir_align, uint8_t* in_depth_align);

	detect_res_t face_unlock_det_face(uint8_t*img, int img_id, int h, int w);
};

#ifdef __cplusplus
};
#endif

#endif

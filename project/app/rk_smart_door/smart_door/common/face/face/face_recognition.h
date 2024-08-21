#ifndef __FACE_RECOGNITION_H__
#define __FACE_RECOGNITION_H__

struct face_recogniton_arg {
	char *sensor_name;
	char *mdev_path_cif_ir;
	char *mdev_path_isp_flood;
	char *mdev_path_isp_pro;
};

int smart_door_face_recogniton_start(struct face_recogniton_arg *arg);
void smart_door_face_recogniton_stop(void);
int smart_door_face_auto_test(struct face_recogniton_arg *arg);

#endif

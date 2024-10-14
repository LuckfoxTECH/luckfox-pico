#ifndef __v4l2_link_H__
#define __v4l2_link_H__

#include <stdbool.h>

#define DEV_PATH_LEN 32
struct media_path_s {
	char media_dev_path[DEV_PATH_LEN];
	char isp_dev_path[DEV_PATH_LEN];
	char csi_dev_path[DEV_PATH_LEN];
	char main_path[DEV_PATH_LEN];
	char rawrd2_s_path[DEV_PATH_LEN];
	char stats_path[DEV_PATH_LEN];
	char input_params_path[DEV_PATH_LEN];
};

int v4l2_media_link_rawrd(const char *media_dev_path, bool enable);
int v4l2_media_get_isp_info(const char *media_dev_path,
		struct media_path_s *isp_info);
int v4l2_media_get_entity_node(const char *media_dev_path,
		const char *name, char *node);
int v4l2_media_copy_isp_entity(const char *src_media_dev, const char *dst_media_dev);

#endif

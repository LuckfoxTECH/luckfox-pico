
#ifndef __TUYA_FFS_DSS_H__
#define __TUYA_FFS_DSS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_ffs.h"

#define TUYA_FFS_DSS_HOST_LEN (127)

typedef struct {
	char host[TUYA_FFS_DSS_HOST_LEN + 1];
	uint32_t port;
	char *session_id;    // use strdup, must free;
	char *session_token; // use strdup, must free;
	char *gw_endpoint;   // use strdup, must free;
	char *salt;          // use strdup, must free;
	uint32_t sequenceNumber;
	tuya_ffs_stream_t *http_body; // must free.
	const char *provisionee_state;
	bool connected_wifi;
	tuya_ffs_config_t *config;
	S_HTTP_MANAGER *httpManager;
	SESSION_ID httpSession;

} tuya_ffs_dss_context_t;

OPERATE_RET tuya_ffs_dss_init(tuya_ffs_config_t *config, uint32_t port);

OPERATE_RET tuya_ffs_dss_run(tuya_ffs_info_t *ffs_info);

char *tuya_ffs_dss_get_token();

char *tuya_ffs_dss_get_endpoint();

OPERATE_RET tuya_ffs_dss_destory();

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __TUYA_FFS_DSS_H__

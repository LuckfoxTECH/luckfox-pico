/* V2.1:
 *	1. remove VENDOR_SN_ID len limit
 *	2. add custom id
 *	3. exten max vendor string len to 1024
 *	4. support file read/write
 *	5. support build a library
 */

#include "vendor_storage.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define VENDOR_STORAGE_DEBUG
#ifdef VENDOR_STORAGE_DEBUG
#define DEBUG(fmt, args...) fprintf(stdout, "[Debug] " fmt, ##args)
#else
#define DEBUG(fmt, args...)
#endif
#define INFO(fmt, args...) fprintf(stdout, "[INFO] " fmt, ##args)
#define ERROR(fmt, args...) fprintf(stderr, "[ERROR] " fmt, ##args)

typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned char uint8;

#define VENDOR_MAX_SIZE 1024
#define VENDOR_REQ_TAG 0x56524551
#define VENDOR_READ_IO _IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO _IOW('v', 0x02, unsigned int)

#define VENDOR_ID_MAX 16
#define VENDOR_CMD_CUSTOM_LEN sizeof("VENDOR_CUSTOM_ID")
static char *vendor_id_table[] = {
    "VENDOR_SN_ID",
    "VENDOR_WIFI_MAC_ID",
    "VENDOR_LAN_MAC_ID",
    "VENDOR_BT_MAC_ID",
    "VENDOR_HDCP_14_HDMI_ID",
    "VENDOR_HDCP_14_DP_ID",
    "VENDOR_HDCP_2x_ID",
    "VENDOR_DRM_KEY_ID",
    "VENDOR_PLAYREADY_Cert_ID",
    "VENDOR_ATTENTION_KEY_ID",
    "VENDOR_PLAYREADY_ROOT_KEY_0_ID",
    "VENDOR_PLAYREADY_ROOT_KEY_1_ID",
    "VENDOR_SENSOR_CALIBRATION_ID",
    "VENODR_RESERVE_ID_14",
    "VENDOR_IMEI_ID",
    "VENDOR_CUSTOM_ID" /* CUSTOM_ID must be last one */
};

#define VENDOR_PR_HEX 0
#define VENDOR_PR_STRING 1

/* Set custom_id to hex print default */
#define GET_PR_FORMAT(ID, FORMAT)                                                                  \
	if ((ID) == VENDOR_IMEI_ID || (ID) == VENDOR_SN_ID)                                            \
		FORMAT = VENDOR_PR_STRING;                                                                 \
	else                                                                                           \
		FORMAT = VENDOR_PR_HEX;

struct rk_vendor_req {
	uint32 tag;
	uint16 id;
	uint16 len;
	uint8 data[1024];
};

int rkvendor_read(int vendor_id, char *data, int size) {
	int ret;
	uint8 p_buf[sizeof(struct rk_vendor_req)]; /* malloc req buffer or used extern buffer */
	struct rk_vendor_req *req;

	req = (struct rk_vendor_req *)p_buf;
	memset(p_buf, 0, sizeof(struct rk_vendor_req));
	int sys_fd = open("/dev/vendor_storage", O_RDONLY);
	if (sys_fd < 0) {
		fprintf(stderr, "vendor_storage open fail\n");
		return -1;
	}

	req->tag = VENDOR_REQ_TAG;
	req->id = vendor_id;
	req->len = VENDOR_MAX_SIZE;

	ret = ioctl(sys_fd, VENDOR_READ_IO, req);
	close(sys_fd);
	if (ret) {
		fprintf(stderr, "vendor read error %d\n", ret);
		return -1;
	}

	if (size < req->len) {
		fprintf(stderr, "vendor storage: param size is lower then read size %d\n",
		        strlen(req->data));
		return -1;
	}

	memcpy(data, req->data, req->len);
	return 0;
}

int rkvendor_write(int vendor_id, const char *data, int size) {
	int ret;
	uint8 p_buf[sizeof(struct rk_vendor_req)]; /* malloc req buffer or used extern buffer */
	struct rk_vendor_req *req;

	if (size > VENDOR_MAX_SIZE) {
		fprintf(stderr, "vendor storage input data overflow\n");
		return -1;
	}

	req = (struct rk_vendor_req *)p_buf;
	int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
	if (sys_fd < 0) {
		fprintf(stderr, "vendor storage open fail\n");
		return -1;
	}

	req->tag = VENDOR_REQ_TAG;
	req->id = vendor_id;

	req->len = size;
	memcpy(req->data, data, req->len);

	ret = ioctl(sys_fd, VENDOR_WRITE_IO, req);
	close(sys_fd);
	if (ret) {
		fprintf(stderr, "vendor write error\n");
		return -1;
	}

	return 0;
}
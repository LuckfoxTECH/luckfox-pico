#ifndef _H_VENDOR_STORAGE_INC
#define _H_VENDOR_STORAGE_INC

/* define from include/linux/soc/rockchip/rk_vendor_storage.h */
#define VENDOR_SN_ID 1
#define VENDOR_WIFI_MAC_ID 2
#define VENDOR_LAN_MAC_ID 3
#define VENDOR_BT_MAC_ID 4
#define VENDOR_HDCP_14_HDMI_ID 5
#define VENDOR_HDCP_14_DP_ID 6
#define VENDOR_HDCP_2x_ID 7
#define VENDOR_DRM_KEY_ID 8
#define VENDOR_PLAYREADY_Cert_ID 9
#define VENDOR_ATTENTION_KEY_ID 10
#define VENDOR_PLAYREADY_ROOT_KEY_0_ID 11
#define VENDOR_PLAYREADY_ROOT_KEY_1_ID 12
#define VENDOR_SENSOR_CALIBRATION_ID 13
#define VENDOR_IMEI_ID 15
#define VENDOR_CUSTOM_ID 16

#ifdef __cplusplus
extern "C" {
#endif

/*
 * vendor_id: VENDOR_SN_ID ... VENDOR_CUSTOM_ID, 1 ... 16
 * return: 0 SUCCESS
 */
int rkvendor_read(int vendor_id, char *data, int size);

/*
 * vendor_id: VENDOR_SN_ID ... VENDOR_CUSTOM_ID, 1 ... 16
 * return: 0 SUCCESS
 */
int rkvendor_write(int vendor_id, const char *data, int size);

#ifdef __cplusplus
}
#endif

#define VENDOR_READ(VENDOR_ID, DATA) rkvendor_read(VENDOR_ID, DATA, sizeof(DATA) / sizeof(DATA[0]))
#define VENDOR_WRITE(VENDOR_ID, DATA)                                                              \
	rkvendor_write(VENDOR_ID, DATA, sizeof(DATA) / sizeof(DATA[0]))
#endif /* _H_VENDOR_STORAGE_INC */

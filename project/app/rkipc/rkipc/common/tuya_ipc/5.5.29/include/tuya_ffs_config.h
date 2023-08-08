
#ifndef __TUYA_FFS_CONFIG_H__
#define __TUYA_FFS_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define TUYA_FFS_TEST_MODE (0)
#define TUYA_FFS_MODE (1)

#define TUYA_FFS_SUPPORT_MBEDTLS (1)
#define TUYA_FFS_SUPPORT_OPENSSL (0)

#define TUYA_FFS_DSS_API_VERSION ("v1")

#define FFS_MANUFACTURER_NAME ("Tuya")
#define FFS_MODEL_NUMBER ("RTL8720DN")
#define FFS_HARDWARE_REVISION ("1.0.0")
#define FFS_FIRMWARE_REVISION ("1.0.0")
#define FFS_DEVICE_NAME ("TestDevice")

#define FFS_HTTPS_PORT (443)
#define FFS_DSS_ENDPOINT_DEFAULT ("https://dp-sps-na.amazon.com")

#define FFS_DEFAULT_SSID ("simple_setup")

#define FFS_TASK_STACK_DEPTH (2048)
#define FFS_TASK_PRIORITY (TRD_PRIO_0)

#define FFS_CLIENT_NONCE_SIZE (12)
#define FFS_DSS_NONCE_SIZE (31)
#define FFS_PIN_SIZE (9)
#define FFS_PID_SIZE (4)
#define FFS_SN_SIZE (15)

#define FFS_CONNECT_WIFI_TIMEOUT_SEC (10)

#define FFS_WIFI_SSID_LEN (32)
#define FFS_WIFI_PASSWD_LEN (64)

#define FFS_CHECK_SUCCESS(ret)                                                                     \
	{                                                                                              \
		if (OPRT_OK != ret) {                                                                      \
			PR_ERR("fail.");                                                                       \
			return ret;                                                                            \
		}                                                                                          \
	}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __TUYA_FFS_CONFIG_H__

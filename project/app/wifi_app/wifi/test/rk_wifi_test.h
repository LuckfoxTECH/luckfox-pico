#ifndef __WIFI_TEST_H__
#define __WIFI_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

void rk_wifi_airkiss_start(void *data);
void rk_wifi_airkiss_stop(void *data);
void rk_wifi_softap_start(void *data);
void rk_wifi_softap_stop(void *data);
void rk_wifi_open(void *data);
void rk_wifi_openoff_test(char *data);
void rk_wifi_close(void *data);
void rk_wifi_connect(void *data);
void rk_wifi_scan(void *data);
void rk_wifi_getSavedInfo(void *data);
void rk_wifi_getConnectionInfo(void *data);
void rk_wifi_connect_with_ssid(void *data);
void rk_wifi_cancel(void *data);
void rk_wifi_forget_with_ssid(void *data);
void rk_wifi_disconnect(void *data);

#ifdef __cplusplus
}
#endif

#endif /* __WIFI_TEST_H__ */

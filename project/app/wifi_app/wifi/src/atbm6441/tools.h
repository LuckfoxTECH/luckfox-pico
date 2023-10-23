#ifndef ___ATBM_TOOLS_H__
#define ___ATBM_TOOLS_H__

typedef void (*key_long_press_event_func)();
typedef int (*key_short_press_event_func)();
typedef int (*wakeup_event_func)(int);
typedef int (*battery_level_event_func)(int);
typedef int (*pir_detect_event_func)(int);
typedef int (*wifi_connect_result_event_func)(int);

int enable_driver(int on);
int get_status();
int atbm_init();
int set_filter(char *cmd_0, char *cmd_1);
int enable_filter();
int fw_at(char *cmd);
void driver_go_sleep(void);
int enable_network(char *ssid, char *key);
int get_sta_model_status();
int get_ipaddr_v4(char *ip_addr, int ip_addr_size);
int clear_wifi_cfg();
int set_port_filter(int port, int enable);

int key_long_press_func_cb_register(key_long_press_event_func long_press_func_ptr);
int key_short_press_func_cb_register(key_short_press_event_func short_press_func_ptr);
int wakeup_event_func_cb_register(wakeup_event_func wakeup_event_func_ptr);
int battery_level_event_func_cb_register(battery_level_event_func battery_level_event_func_ptr);
int pir_detect_event_func_cb_register(pir_detect_event_func pir_detect_event_func_ptr);
int wifi_connect_result_event_cb_register(wifi_connect_result_event_func wifi_connect_result_event_func_ptr);

#endif

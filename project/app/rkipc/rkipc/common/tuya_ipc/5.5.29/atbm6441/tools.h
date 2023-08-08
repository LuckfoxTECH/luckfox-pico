int enable_driver(int on);
int get_status();
int atbm_init();
int set_filter(char *cmd_0, char *cmd_1);
int enable_filter();
int fw_at(char *cmd);
void driver_go_sleep(void);
int enable_network(char *ssid, char *key);
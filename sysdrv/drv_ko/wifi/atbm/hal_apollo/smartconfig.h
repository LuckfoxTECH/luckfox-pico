#ifndef __SMART_CONFIG_H__
#define  __SMART_CONFIG_H__
/*
enum smartconfig_status{
	CONFIG_ST_IDLE = 0,
	CONFIG_ST_START = 1,
	CONFIG_ST_GET_MAGIC = 2,
	CONFIG_ST_SWITCH_PAYLOAD_CH = 3,
	CONFIG_ST_GET_PAYLOAD = 4,
	CONFIG_ST_DONE_SUCCESS = 5,
	CONFIG_ST_DONE_FAIL =6,
	CONFIG_ST_DUPLICATE_FAIL =7,
	CONFIG_ST_STARTCONFIG_ERR =8,
};*/
enum smartconfig_status{
	CONFIG_ST_IDLE = 0,
	CONFIG_ST_REIDLE = 8,
	CONFIG_ST_START = 1,
	CONFIG_ST_GET_MAGIC = 2,
	CONFIG_ST_PAYLOAD = 3,
	CONFIG_ST_RE_PAYLOAD = 4,
	CONFIG_ST_GET_TOTALLEN = 5,
	CONFIG_ST_SWITCH_PAYLOAD_CH = 6,
	CONFIG_ST_GET_PAYLOAD = 7,
	CONFIG_ST_GET_PAYLOAD_CSUM = 8,
	CONFIG_ST_DONE_SUCCESS = 9,
	CONFIG_ST_DONE_FAIL =10,
	CONFIG_ST_DUPLICATE_FAIL =11,
	CONFIG_ST_STARTCONFIG_ERR =12,
};

enum smartconfig_type{
	CONFIG_TP_ATBM_SMART = 0,
	CONFIG_TP_AIRKISS = 1,
};

struct smartconfig_config{
	enum smartconfig_type type;
	/*cnt : 1 ~ 10*/
	int magic_cnt;
	/*ms : 20ms ~ 200ms*/
	int magic_time;
	/*ms : 500ms ~ 10000ms*/
	int payload_time;
};
int atbm_smartconfig_start(struct atbm_common *hw_priv,int enable);
int atbm_smartconfig_stop(struct atbm_common *hw_priv);
int smartconfig_start(struct atbm_common *hw_priv,struct smartconfig_config * st_cfg);
int smartconfig_stop(struct atbm_common *hw_priv);
int smartconfig_status(struct atbm_common *hw_priv);

/*cnt : 2 ~ 10*/
int smartconfig_magic_channel_cnt(int cnt);
/*ms : 20ms ~ 200ms*/
int smartconfig_magic_channel_timeout(int ms);
/*ms : 500ms ~ 10000ms*/
int smartconfig_payload_timeout(int ms);
void atbm_smart_scan_work(struct atbm_work_struct *work);
void atbm_smart_setchan_work(struct atbm_work_struct *work);
void atbm_smart_stop_work(struct atbm_work_struct *work);
void atbm_smartconfig_expire_timer(unsigned long arg);





#endif  /*__SMART_CONFIG_H__*/

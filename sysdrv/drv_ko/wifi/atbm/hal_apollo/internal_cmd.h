#ifndef __INTERNAL_CMD__
#define __INTERNAL_CMD__
#include <linux/hash.h>
#include "mac80211/ieee80211_i.h"
void send_signal(int sig_num,int user_pid);
bool atbm_internal_cmd_scan_triger(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_scan_request *req);
bool atbm_internal_cmd_stainfo(struct ieee80211_local *local,struct ieee80211_internal_sta_req *sta_req);
bool atbm_internal_cmd_monitor_req(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_monitor_req *monitor_req);
bool atbm_internal_cmd_stop_monitor(struct ieee80211_sub_if_data *sdata);
bool atbm_internal_wsm_adaptive(struct atbm_common *hw_priv,struct ieee80211_internal_wsm_adaptive *adaptive);
bool atbm_internal_wsm_txpwr_dcxo(struct atbm_common *hw_priv,struct ieee80211_internal_wsm_txpwr_dcxo *txpwr_dcxo);
bool atbm_internal_wsm_txpwr(struct atbm_common *hw_priv,struct ieee80211_internal_wsm_txpwr *txpwr);
bool atbm_internal_freq_set(struct ieee80211_hw *hw,struct ieee80211_internal_set_freq_req *req);
bool atbm_internal_cmd_scan_build(struct ieee80211_local *local,struct ieee80211_internal_scan_request *req,
											   u8* channels,int n_channels,struct cfg80211_ssid *ssids,int n_ssids,
											   struct ieee80211_internal_mac *macs,int n_macs);
bool atbm_internal_channel_auto_select_results(struct ieee80211_sub_if_data *sdata,
												struct ieee80211_internal_channel_auto_select_results *results);
bool atbm_internal_channel_auto_select(struct ieee80211_sub_if_data *sdata,
													  struct ieee80211_internal_channel_auto_select_req *req);
bool atbm_internal_request_chip_cap(struct ieee80211_hw *hw,struct ieee80211_internal_req_chip *req);
#ifdef CONFIG_ATBM_SUPPORT_AP_CONFIG
bool atbm_internal_update_ap_conf(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_ap_conf *conf_req,bool clear);
#endif
bool atbm_internal_wsm_set_rate(struct atbm_common *hw_priv,struct ieee80211_internal_rate_req *req);
bool atbm_internal_wsm_set_rate_power(struct atbm_common *hw_priv,struct ieee80211_internal_rate_power_req *req);
#ifdef CONFIG_ATBM_MONITOR_SPECIAL_MAC
bool atbm_internal_mac_monitor(struct ieee80211_hw *hw,struct ieee80211_internal_mac_monitor *monitor);
#endif
bool atbm_internal_cmd_req_iftype(struct ieee80211_sub_if_data *sdata,struct ieee80211_internal_iftype_req *req);
#define ATBM_GPIO_CONFIG__FUNCTION_CONFIGD		BIT(0)
#define ATBM_GPIO_CONFIG__INPUT					BIT(1)
#define ATBM_GPIO_CONFIG__OUTPUT				BIT(2)
#define ATBM_GPIO_CONFIG__PUP					BIT(3)
#define ATBM_GPIO_CONFIG__PUD					BIT(4)



struct atbm_ctr_addr{
	unsigned int base_addr;
	unsigned int val;
	char		 start_bit;
	char		 width;
};
struct atbm_gpio_config {
	unsigned int gpio;
	unsigned int flags;
	struct atbm_ctr_addr	fun_ctrl;
	struct atbm_ctr_addr	pup_ctrl;
	struct atbm_ctr_addr	pdu_ctrl;
	struct atbm_ctr_addr	dir_ctrl;
	struct atbm_ctr_addr	out_val;
	struct atbm_ctr_addr	in_val;
};
bool atbm_internal_gpio_config(struct atbm_common *hw_priv,int gpio,bool dir ,bool pu,bool default_val);
bool atbm_internal_gpio_output(struct atbm_common *hw_priv,int gpio,bool set);
bool atbm_internal_gpio_input(struct atbm_common *hw_priv,int gpio,bool *set);
bool atbm_internal_edca_update(struct ieee80211_sub_if_data *sdata,int queue,int aifs,int cw_win,int cw_max,int txop);

#endif

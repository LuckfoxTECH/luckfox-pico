#ifndef __WEXT_CFG_H__
#define __WEXT_CFG_H__

/*
start return  1
stop return 0
*/
int ETF_Test_is_Start(void);
void register_wext_common(struct ieee80211_local *local);

extern struct iw_handler_def atbm_handlers_def;




#endif /*__WEXT_CFG_H__*/


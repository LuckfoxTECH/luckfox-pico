#ifndef __MODULE_FS_H__
#define __MODULE_FS_H__
#include <linux/hash.h>
#include "mac80211/ieee80211_i.h"
#ifdef CONFIG_ATBM_MOULE_FS
extern int atbm_module_attribute_init(void);
extern void atbm_module_attribute_exit(void);
struct atbm_module_scan_node{
	struct ieee80211_internal_scan_sta sta;
	struct hlist_node hnode;
};
#else
#define atbm_module_attribute_init()
#define atbm_module_attribute_exit()
#endif
#endif

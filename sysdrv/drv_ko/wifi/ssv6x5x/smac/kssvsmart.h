#ifndef _SSVSMART_CONFIG_H
#define _SSVSMART_CONFIG_H

//#include <linux/nl80211.h>
#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
#include <ssv6200.h>
#include <smac/dev.h>

#ifdef CONFIG_SSV_SMARTLINK
enum {
	SI_ST_NG,
	SI_ST_OK,
	SI_ST_PROCESSING,
	SI_ST_MAX
};

void ssv6xxx_process_si_event(struct ssv_softc *sc, struct sk_buff *skb);
int ssv6xxx_send_si_cmd(u32 smart_icomm_cmd);

void set_si_status(u32 st);
int get_si_status(char *input);
int get_si_ssid(char *input);
int get_si_pass(char *input);

#endif //CONFIG_SSV_SMARTLINK

#endif //_SSVSMART_CONFIG_H

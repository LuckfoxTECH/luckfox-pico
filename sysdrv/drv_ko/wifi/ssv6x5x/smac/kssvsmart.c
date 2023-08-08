#include <ssv6200.h>
#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
#include <linux/module.h> 
#include <smac/dev.h>
#include "kssvsmart.h"
#include "lib.h"
#include "ssv_skb.h"
#include "init.h"

#ifdef CONFIG_SSV_SMARTLINK
#define VALID_SSID_LEN(x)    ((x) <= 32)
#define VALID_PASS_LEN(x)    ((x) == 0 || ((x) >= 8 && (x) <= 64))//do not support WEP
#define INVALID              "invalid"
#define NONE                 "none"

struct ssv6xxx_si_cfg si_cfg;
int si_st;
char si_status_st[][16] = {
	"NG",
	"OK",
	"PROCESSING",
	"MAX"
};

inline void set_si_status(u32 st)
{
    si_st = st;
}

int get_si_status(char *input)
{
    //printk("%s: %s\n", __FUNCTION__, si_status_st[si_st]);
    strcpy(input,si_status_st[si_st]);

    return strlen(si_status_st[si_st]);
}

int get_si_ssid(char *input)
{
    strcpy(input,si_cfg.ssid);
    return strlen(si_cfg.ssid);
}

int get_si_pass(char *input)
{
    strcpy(input,si_cfg.password);
    return strlen(si_cfg.password);
}

void ssv6xxx_process_si_event(struct ssv_softc *sc, struct sk_buff *skb)
{
    struct cfg_host_event *host_event;
    struct ssv6xxx_si_cfg *p_si_cfg;
    
    printk("received SOC_EVT_SMART_ICOMM event\n");
    host_event = (struct cfg_host_event *)skb->data;
    p_si_cfg = (struct ssv6xxx_si_cfg *)&host_event->dat[0];
    if (VALID_SSID_LEN(p_si_cfg->ssid_len) && VALID_PASS_LEN(p_si_cfg->password_len)) {
        memcpy(&si_cfg.ssid, p_si_cfg->ssid, p_si_cfg->ssid_len);
        if (p_si_cfg->password_len != 0)
            memcpy(&si_cfg.password, p_si_cfg->password, p_si_cfg->password_len);
        else
            memcpy(&si_cfg.password, NONE, sizeof(NONE));

        si_cfg.ssid_len = p_si_cfg->ssid_len;
        si_cfg.password_len = p_si_cfg->password_len;
        set_si_status(SI_ST_OK);
        printk("SSID: %s, LEN: %d\n", si_cfg.ssid, si_cfg.ssid_len);
        printk("PASS: %s, LEN: %d\n", si_cfg.password, si_cfg.password_len);
    }
    else {
        printk("!!! Oops, got invalid value of ssid_len:%d or password_len:%d !!!\n",
                p_si_cfg->ssid_len, p_si_cfg->password_len);
        set_si_status(SI_ST_NG);
        memcpy(&si_cfg.ssid, INVALID, sizeof(INVALID));
        memcpy(&si_cfg.password, INVALID, sizeof(INVALID));
    }        
}
EXPORT_SYMBOL(ssv6xxx_process_si_event);

int ssv6xxx_send_si_cmd(u32 smart_icomm_cmd)
{
    struct sk_buff      *skb;
    struct cfg_host_cmd *host_cmd;
    u32 total_ch_cfg_size, i;
    struct ssv6xxx_ch_cfg ch;
    int ret;
    struct ssv_softc *ssv_dbg_sc;
    struct ssv_hw *sh;

    ssv_dbg_sc = ssv6xxx_driver_attach(SSV_DRVER_NAME);
    if(ssv_dbg_sc == NULL)
    {
        printk("Get ssv_dbg_sc fail!!!\n");
        return -1;
    }
    sh = ssv_dbg_sc->sh;


    if (smart_icomm_cmd == START_SMART_ICOMM || smart_icomm_cmd == RESET_SMART_ICOMM)
        set_si_status(SI_ST_PROCESSING);
    else
        set_si_status(SI_ST_OK);

    memset(&si_cfg, 0, sizeof(si_cfg));
    total_ch_cfg_size = sh->ch_cfg_size * sizeof(struct ssv6xxx_ch_cfg);

    skb = ssv_skb_alloc(sh->sc, HOST_CMD_HDR_LEN + sizeof(u32) + sizeof(u32) + total_ch_cfg_size);

    if(skb == NULL)
    {
        printk("ssv command skb_alloc fail!!!\n");
        set_si_status(SI_ST_NG);
        return -1;
    }

    skb->data_len = HOST_CMD_HDR_LEN + sizeof(u32) + sizeof(u32) + total_ch_cfg_size;
    skb->len      = skb->data_len;

    host_cmd = (struct cfg_host_cmd *)skb->data;
    memset(host_cmd, 0x0, sizeof(struct cfg_host_cmd));
    host_cmd->c_type = HOST_CMD;
    host_cmd->h_cmd  = (u8)SSV6XXX_HOST_CMD_SMART_ICOMM;
    host_cmd->len    = skb->data_len;

    printk("smart icomm, cmd: %d, ch_cfg_size : %d\n", smart_icomm_cmd, sh->ch_cfg_size); 
    memcpy(host_cmd->un.dat8, &smart_icomm_cmd, sizeof(u32));
    memcpy(host_cmd->un.dat8 + sizeof(u32), &sh->ch_cfg_size, sizeof(u32));
    memcpy(host_cmd->un.dat8 + sizeof(s32) + sizeof(s32), sh->p_ch_cfg, total_ch_cfg_size);

    for (i = 0; i < sh->ch_cfg_size; i++) {
        memcpy(&ch, host_cmd->un.dat8+sizeof(u32) + sizeof(u32) + (i * sizeof(struct ssv6xxx_ch_cfg)), sizeof(struct ssv6xxx_ch_cfg)); 
        printk("reg: 0x%08x, ch1: 0x%08x, ch13: 0x%08x\n", ch.reg_addr, ch.ch1_12_value, ch.ch13_14_value);
    }

    ret = HCI_SEND_CMD(sh, skb);

    return ret;
}
EXPORT_SYMBOL(ssv6xxx_send_si_cmd);
#endif //CONFIG_SSV_SMARTLINK

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <net/genetlink.h>
#include <linux/init.h>
#include <net/sock.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <linux/version.h>


#include "ssvdevice/dev.h"
#include "ssv_ctl_common.h"
#include "ssv_wpas_ctl.h"
#include <hwif/hwif.h>
#include "fmac/fmac_msg_tx.h"
#include "ssv_debug.h"

extern struct ssv_softc *g_ssv_sc;
extern struct ssv_vif *g_ssv_vif;
static int wpas_ctl_extauth_status_cmd(struct sk_buff *skb, struct genl_info *info);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
static struct nla_policy ssv_wpas_ctl_genl_policy[E_SSV_WPAS_CTL_ATTR_MAX + 1] = {
    [E_SSV_WPAS_CTL_ATTR_UNSPEC]       = { .type = NLA_U32 },
    [E_SSV_WPAS_CTL_ATTR_EXTAUTH]      = { .type = NLA_U16 },
    [E_SSV_WPAS_CTL_ATTR_EXTAUTH_RSP]  = { .type = NLA_BINARY,
                                          .len = 64},
};
#endif

/* commands: ssv wpas ctl netlink ops */
struct genl_ops ssv_wpas_ctl_gnl_ops[] = {
    {
        .cmd = E_SSV_WPAS_CTL_CMD_EXTAUTH,
        .flags = 0,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
        .policy = ssv_wpas_ctl_genl_policy,
#endif
        .doit = wpas_ctl_extauth_status_cmd,
        .dumpit = NULL,
    },

};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
static struct genl_multicast_group ssv_mcgrp[] = {
    { .name = "extauth", },
};
#else
static struct genl_multicast_group ssv_mcgrp = {
    .name = "extauth",
};
#endif

/* family definition */
static struct genl_family ssv_wpas_ctl_gnl_family = {
    .id = SSV_WPAS_NL_ID,
    .hdrsize = 0,
    .name = "SSV_WPAS_CTL",
    .version = 1,
    .maxattr = E_SSV_WPAS_CTL_ATTR_MAX,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0)
    .ops        = ssv_wpas_ctl_gnl_ops,
    .n_ops      = ARRAY_SIZE(ssv_wpas_ctl_gnl_ops),
    .mcgrps     = ssv_mcgrp,
    .n_mcgrps   = ARRAY_SIZE(ssv_mcgrp),
#endif
};

static int wpas_ctl_extauth_status_cmd(struct sk_buff *skb, struct genl_info *info)
{
   u16 status = 0;

    if (info == NULL)
        return -EINVAL;

    if (!info->attrs[E_SSV_WPAS_CTL_ATTR_EXTAUTH])
        return -EINVAL;
    else
    {
	    status = *(u16 *)nla_data(info->attrs[E_SSV_WPAS_CTL_ATTR_EXTAUTH]);
    }

    if ((g_ssv_sc != NULL) && (g_ssv_vif != NULL)) {
        if (!(g_ssv_vif->sta.flags & SSV_STA_EXT_AUTH))
            return -EINVAL;

        ssv_external_auth_disable(g_ssv_vif);

        ssv_send_sm_external_auth_required_rsp(g_ssv_sc, g_ssv_vif, status);
    }

    return 0;
}

int ssv_wpas_ctl_extauth_req(void *data, int size)
{
    struct sk_buff *msg;
    void *hdr;
    int retval = 0;

    msg = genlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
    if (!msg)
    {
        SSV_LOG_DBG("new genlmsg error\n");
        return -1;
    }

    hdr = genlmsg_put(msg, 0, 0, &ssv_wpas_ctl_gnl_family, 0, E_SSV_WPAS_CTL_CMD_EXTAUTH);
    if (!hdr)
    {
        retval = -ENOBUFS;
        goto free_msg;
    }

    retval = nla_put(msg, E_SSV_WPAS_CTL_ATTR_EXTAUTH_RSP, size, data);
    if (retval)
    {
        SSV_LOG_DBG("Fail to add attribute in message\n");;
        goto free_msg;
    }

    genlmsg_end(msg, hdr);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
    genlmsg_multicast(&ssv_wpas_ctl_gnl_family, msg, 0, 0, GFP_ATOMIC);
#else
    genlmsg_multicast(msg, 0, ssv_mcgrp.id, GFP_ATOMIC);
#endif
    return 0;

free_msg:
    nlmsg_free(msg);
    return -1;
}
EXPORT_SYMBOL(ssv_wpas_ctl_extauth_req);

int ssv_wpas_ctl_init(void)
{
    int rc;

    SSV_LOG_DBG("~~~~~~~~~INIT SSV WPAS CTL GENERIC NETLINK MODULE\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0)
    rc = genl_register_family(&ssv_wpas_ctl_gnl_family);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
    rc = genl_register_family_with_ops_groups(&ssv_wpas_ctl_gnl_family,
            ssv_wpas_ctl_gnl_ops, ssv_mcgrp);
#else
    rc = genl_register_family_with_ops(&ssv_wpas_ctl_gnl_family,
            ssv_wpas_ctl_gnl_ops, ARRAY_SIZE(ssv_wpas_ctl_gnl_ops));
    genl_register_mc_group(&ssv_wpas_ctl_gnl_family, &ssv_mcgrp);
#endif
    if (0 != rc)
    {
        SSV_LOG_DBG("Fail to insert SSV WPAS CTL NETLINK MODULE\n");
        return -1;
    }

    
    return 0;
}
EXPORT_SYMBOL(ssv_wpas_ctl_init);

int ssv_wpas_ctl_deinit(void)
{
    int ret;

    SSV_LOG_DBG("EXIT SSV WPAS CTL GENERIC NETLINK MODULE\n");
    ret = genl_unregister_family(&ssv_wpas_ctl_gnl_family);
    if(0 != ret) {
        SSV_LOG_DBG("unregister family %i\n",ret);
    }

    return ret;
}
EXPORT_SYMBOL(ssv_wpas_ctl_deinit);

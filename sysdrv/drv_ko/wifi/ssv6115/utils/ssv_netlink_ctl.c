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
#include "ssv_netlink_ctl.h"
#include "ble/nimble/nimble_msg.h"
#include <hwif/hwif.h>
#include "ssv_debug.h"

extern struct ssv_hw *g_ssv_hw;
static int wifi_ctl_operation_cmd(struct sk_buff *skb, struct genl_info *info);
static int ssv_ctl_ssv_nimble_enable(struct sk_buff *skb, struct genl_info *info);
static int ssv_ctl_ssv_nimble_disable(struct sk_buff *skb, struct genl_info *info);
static int ssv_ctl_to_ssv_nimble(struct sk_buff *skb, struct genl_info *info);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
static struct nla_policy ssv_ctl_genl_policy[E_SSV_CTL_ATTR_MAX + 1] = {
    [E_SSV_CTL_ATTR_UNSPEC]        = { .type = NLA_U32 },
    [E_SSV_CTL_ATTR_OPERACMD]      = { .type = NLA_U32 },
    [E_SSV_CTL_ATTR_OPERACMD_RSP]  = { .type = NLA_BINARY, .len = 512 },
    [E_SSV_CTL_ATTR_SSV_NIMBLE_ENABLE]    = { .type = NLA_U32 },
    [E_SSV_CTL_ATTR_SSV_NIMBLE_DISABLE]   = { .type = NLA_U32 },
    [E_SSV_CTL_ATTR_TO_SSV_NIMBLE]        = { .type = NLA_BINARY, .len = 2304 },
    [E_SSV_CTL_ATTR_FROM_SSV_NIMBLE]      = { .type = NLA_BINARY, .len = 2304 },
};
#endif

/* commands: ssv ctl netlink ops */
struct genl_ops ssv_ctl_gnl_ops[] = {
    {
        .cmd = E_SSV_CTL_CMD_TO_DRV_OPERACMD,
        .flags = 0,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
        .policy = ssv_ctl_genl_policy,
#endif
        .doit = wifi_ctl_operation_cmd,
        .dumpit = NULL,
    },
    {
        .cmd = E_SSV_CTL_CMD_SSV_NIMBLE_ENABLE,
        .flags = 0,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
        .policy = ssv_ctl_genl_policy,
#endif
        .doit = ssv_ctl_ssv_nimble_enable,
        .dumpit = NULL,
    },
    {
        .cmd = E_SSV_CTL_CMD_SSV_NIMBLE_DISABLE,
        .flags = 0,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
        .policy = ssv_ctl_genl_policy,
#endif
        .doit = ssv_ctl_ssv_nimble_disable,
        .dumpit = NULL,
    },
    {
        .cmd = E_SSV_CTL_CMD_TO_SSV_NIMBLE,
        .flags = 0,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 4, 0))
        .policy = ssv_ctl_genl_policy,
#endif
        .doit = ssv_ctl_to_ssv_nimble,
        .dumpit = NULL,
    },
};

/* family definition */
static struct genl_family ssv_ctl_gnl_family = {
    //.id = GENL_ID_GENERATE,
    .id = SSV_CTL_NL_ID,
    .hdrsize = 0,
    .name = "SSV_CTL",
    .version = 1,
    .maxattr = E_SSV_CTL_ATTR_MAX,
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0)
    .ops        = ssv_ctl_gnl_ops,
    .n_ops      = ARRAY_SIZE(ssv_ctl_gnl_ops),
#endif
};


void wifi_ctl_oper_response(ST_WIFI_REG_PARAM *reg )
{
    struct sk_buff *msg;
    void *hdr;
    int retval = 0;

    msg = genlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
    if (!msg)
    {
        SSV_LOG_DBG("new genlmsg error\n");
        return;
    }

    hdr = genlmsg_put(msg, 0, 0, &ssv_ctl_gnl_family, 0, E_SSV_CTL_CMD_TO_DRV_OPERACMD);
    if (!hdr)
    {
        retval = -ENOBUFS;
        goto free_msg;
    }

    retval = nla_put(msg, E_SSV_CTL_ATTR_OPERACMD_RSP, sizeof(ST_WIFI_REG_PARAM), reg);
    if (retval)
    {
        SSV_LOG_DBG("Fail to add attribute in message\n");
        goto free_msg;
    }

    genlmsg_end(msg, hdr);
    genlmsg_unicast(g_ssv_hw->usernet, msg, g_ssv_hw->userport);
    return;

free_msg:
    nlmsg_free(msg);
}

static int wifi_ctl_operation_cmd(struct sk_buff *skb, struct genl_info *info)
{
    struct sk_buff *msg;
    void *hdr;
    int retval = 0,count = 0;
	u32 operaid;
    //u8 *dat;
    ST_WIFI_REG_PARAM *reg = NULL;
    int i;
    //char *endp;
    //struct ssv_softc *sc = ssv6xxx_driver_attach(SSV_DRVER_NAME);
    
    ST_WIFI_DRV_CMD *cmd = NULL;
    if (info == NULL)
        return -EINVAL;

    if (!info->attrs[E_SSV_CTL_ATTR_OPERACMD])
        return -EINVAL;
    else
    {
        cmd = (ST_WIFI_DRV_CMD *)nla_data(info->attrs[E_SSV_CTL_ATTR_OPERACMD]);        
    	operaid = cmd->cmdid;
    }

    mutex_lock(&g_ssv_hw->mutex);
	if(operaid == 0)
	{
		if(g_ssv_hw->userport == 0)
		{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
			g_ssv_hw->userport = info->snd_portid;
#else
			g_ssv_hw->userport = info->snd_pid;
#endif
			g_ssv_hw->usernet = genl_info_net(info);
		}
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
		else if(info->snd_portid != g_ssv_hw->userport)
#else
		else if(info->snd_pid != g_ssv_hw->userport)
#endif
		{
			SSV_LOG_DBG("previous netlink exists,please kill app first \n");
		}
	}
	else if(operaid == 1)
	{
		g_ssv_hw->userport = 0;
		g_ssv_hw->usernet = NULL;
	}
    
    mutex_unlock(&g_ssv_hw->mutex);

	//Notify app to terminate
	if(operaid == 1)
	{
		/* allocate new netlink packet */
		msg = genlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
		if (!msg)
			return -ENOMEM;

		hdr = genlmsg_put(msg, 0, info->snd_seq, &ssv_ctl_gnl_family, 0, E_SSV_CTL_CMD_TO_DRV_OPERACMD);
		if (!hdr)
		{
			retval = -ENOBUFS;
			goto free_msg;
		}
		retval = nla_put_u32(msg, E_SSV_CTL_ATTR_OPERACMD, 1);
		if (retval)
		{
			SSV_LOG_DBG("Fail to add attribute in message\n");
			goto free_msg;
		}

		genlmsg_end(msg, hdr);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
		return genlmsg_unicast(genl_info_net(info), msg, info->snd_portid);
#else
		return genlmsg_unicast(genl_info_net(info), msg, info->snd_pid);
#endif

free_msg:
		nlmsg_free(msg);
	}
    
    if(operaid == E_SSV_OPER_REGR){
        reg = (ST_WIFI_REG_PARAM *)cmd->data;
        count = reg->value;
        //SSV_LOG_DBG("addr is 0x%08x,value is 0x%08x \n ",reg->address,reg->value);           
        if(count == 0)
        {
            g_ssv_hw->hwif_ops->readreg(g_ssv_hw->dev,reg->address,&(reg->value));
            //SSV_LOG_DBG("reg read: 0x%08x, 0x%08x \n ",reg->address,value);
            wifi_ctl_oper_response(reg);                
        }
        else
        {
            for(i = 0; i< count; i++, reg->address += 4)
            {
                g_ssv_hw->hwif_ops->readreg(g_ssv_hw->dev,reg->address,&(reg->value));
                SSV_LOG_DBG("reg read: 0x%08x, 0x%08x \n ",reg->address,reg->value);
                wifi_ctl_oper_response(reg);
            }
        }

        retval = 0;
    }

    if(operaid == E_SSV_OPER_REGW)
    {
        reg = (ST_WIFI_REG_PARAM *)cmd->data;
        g_ssv_hw->hwif_ops->writereg(g_ssv_hw->dev,reg->address,reg->value);
        //SSV_LOG_DBG("addr is 0x%08x,value is 0x%08x \n ",reg->address,reg->value);           
        SSV_LOG_DBG("reg write: 0x%08x, 0x%08x \n ",reg->address,reg->value);                
        retval = 0;
    }
    return retval;
}

static u32 ssv_nimble_usr_pid=0; 
static u8 ssv_nimble_enable=0; 
static int ssv_ctl_ssv_nimble_enable(struct sk_buff *skb, struct genl_info *info)
{
    u32 enable;

    if (info == NULL)
        return -EINVAL;

    if (!info->attrs[E_SSV_CTL_ATTR_SSV_NIMBLE_ENABLE])
    {
        SSV_LOG_DBG("the attrs is not enable\n");
        return -EINVAL;
    }
    else 
    {
        enable = nla_get_u32(info->attrs[E_SSV_CTL_ATTR_SSV_NIMBLE_ENABLE]);
#ifdef SSV_CTL_DEBUG
        SSV_LOG_DBG("ssv ble api enable=%d\n", enable);
#endif
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
    ssv_nimble_usr_pid = info->snd_portid;
#else
    ssv_nimble_usr_pid = info->snd_pid;
#endif
    ssv_nimble_enable=1;
    SSV_LOG_DBG("\33[31m%s():pid=%d \33[0m\r\n",__FUNCTION__ ,ssv_nimble_usr_pid);
    return 0;

}

static int ssv_ctl_ssv_nimble_disable(struct sk_buff *skb, struct genl_info *info)
{
    u32 enable;

    if (info == NULL)
        return -EINVAL;

    if (!info->attrs[E_SSV_CTL_ATTR_SSV_NIMBLE_DISABLE])
    {
        SSV_LOG_DBG("the attrs is not enable\n");
        return -EINVAL;
    }
    else 
    {
        enable = nla_get_u32(info->attrs[E_SSV_CTL_ATTR_SSV_NIMBLE_DISABLE]);
#ifdef SSV_CTL_DEBUG
        SSV_LOG_DBG("ssv ble api enable=%d\n", enable);
#endif
    }
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0)
    ssv_nimble_usr_pid = 0;
#else
    ssv_nimble_usr_pid = 0;
#endif
    ssv_nimble_enable=0;
    SSV_LOG_DBG("\33[31m%s()\33[0m\r\n",__FUNCTION__);
    return 0;

}

static int ssv_ctl_to_ssv_nimble(struct sk_buff *skb, struct genl_info *info)
{
    int retval = 0;
    u8 *inval;
	int strlen;

    SSV_LOG_DBG("\33[31m%s():%d \33[0m\r\n",__FUNCTION__ ,__LINE__);
    if (info == NULL)
        return -EINVAL;

    if (!info->attrs[E_SSV_CTL_ATTR_TO_SSV_NIMBLE])
        return -EINVAL;
    else 
    {
        strlen = nla_len(info->attrs[E_SSV_CTL_ATTR_TO_SSV_NIMBLE]);
        inval = (char *)nla_data(info->attrs[E_SSV_CTL_ATTR_TO_SSV_NIMBLE]);
    }

    mutex_lock(&g_ssv_hw->mutex);
#ifdef CONFIG_NIMBLE
    ssv_ble_api_send(g_ssv_hw->snc, inval, strlen);
#endif
    mutex_unlock(&g_ssv_hw->mutex);
    
    return retval;
}

int ssv_ctl_from_ssv_nimble(char *pData, int len)
{
    struct sk_buff *skb;
    int retval;
    void *msg_head;
    unsigned char *pOutBuf=pData;
    int inBufLen=len;
    
    if((0==ssv_nimble_usr_pid)||(0==ssv_nimble_enable))
    {
        return -1;
    }
    /* allocate new netlink packet */
    skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
    if (skb == NULL)
        return -ENOMEM;

    /* create the message headers */
    msg_head = genlmsg_put(skb, 0, 0, &ssv_ctl_gnl_family, 0, E_SSV_CTL_CMD_FROM_SSV_NIMBLE);
    if (msg_head == NULL) 
    {
        retval = -ENOMEM;
        SSV_LOG_DBG("Fail to create the netlink message header\n");
        goto free_msg;
    }
    /* fill with message */ 
    retval = nla_put(skb, E_SSV_CTL_ATTR_FROM_SSV_NIMBLE, inBufLen, pOutBuf);
    if (retval != 0)
    {
        SSV_LOG_DBG("Fail to add attribute in message\n");
        goto free_msg;
    }
    /* finalize the message */
    genlmsg_end(skb, msg_head);
    return genlmsg_unicast(&init_net, skb, ssv_nimble_usr_pid);

free_msg:
    nlmsg_free(skb);
    return retval;
}
EXPORT_SYMBOL(ssv_ctl_from_ssv_nimble);

int ssv_ctl_init(void)
{
    int rc;

    g_ssv_hw->usernet = NULL;
    g_ssv_hw->userport = 0;
    //sc->wifi_ctl_respose_cb = wifi_ctl_cust_response;

    SSV_LOG_DBG("~~~~~~~~~INIT SSV CTL GENERIC NETLINK MODULE\n");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,10,0)
    rc = genl_register_family(&ssv_ctl_gnl_family);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
    rc = genl_register_family_with_ops(&ssv_ctl_gnl_family,
            ssv_ctl_gnl_ops);
#else
    rc = genl_register_family_with_ops(&ssv_ctl_gnl_family,
            ssv_ctl_gnl_ops, ARRAY_SIZE(ssv_ctl_gnl_ops));
#endif
    if (0 != rc)
    {
        SSV_LOG_DBG("Fail to insert SSV CTL NETLINK MODULE\n");
        return -1;
    }

    
    return 0;
}
EXPORT_SYMBOL(ssv_ctl_init);

int ssv_ctl_deinit(void)
{
    int ret;

    SSV_LOG_DBG("EXIT SSV CTL GENERIC NETLINK MODULE\n");
    ret = genl_unregister_family(&ssv_ctl_gnl_family);
    if(0 != ret) {
        SSV_LOG_DBG("unregister family %i\n",ret);
    }

    return ret;
}
EXPORT_SYMBOL(ssv_ctl_deinit);


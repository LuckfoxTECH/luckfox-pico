/**
 ******************************************************************************
 *
 * @file private_cmd.h
 *
 * Copyright (C) Aicsemi 2018-2024
 *
 ******************************************************************************
 */

#ifndef _AIC_PRIV_CMD_H_
#define _AIC_PRIV_CMD_H_

#include "rwnx_defs.h"

typedef struct _android_wifi_priv_cmd {
    char *buf;
    int used_len;
    int total_len;
} android_wifi_priv_cmd;

#ifdef CONFIG_COMPAT
typedef struct _compat_android_wifi_priv_cmd {
    compat_caddr_t buf;
    int used_len;
    int total_len;
} compat_android_wifi_priv_cmd;
#endif /* CONFIG_COMPAT */

int android_priv_cmd(struct net_device *net, struct ifreq *ifr, int cmd);

#endif /* _AIC_PRIV_CMD_H_ */


#ifndef __FFS_NETCFG__
#define __FFS_NETCFG__
typedef OPERATE_RET (*FN_FFS_NET_CFG_CB)(IN CONST CHAR_T *ssid, IN CONST CHAR_T *passwd,
                                         IN CONST CHAR_T *token);

int ffs_netcfg_init();

#endif

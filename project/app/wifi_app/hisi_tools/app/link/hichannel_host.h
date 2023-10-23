/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: sample link file.
 * Author: Hisilicon
 * Create: 2020-09-09
 */

#ifndef HISI_LINK_H
#define HISI_LINK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*****************************************************************************
  1 �궨��
*****************************************************************************/
#define SYSTEM_CMD_SIZE                 384     /* С�����ֵ�����ݱ���ͨ�������ȼ�ͨ������ */
#define MAX_SEND_DATA_SIZE              1500    /* С�����ֵ�����ݱ���ͨ�������ȼ�ͨ������ */
#define SYSTEM_NETDEV_NAME              "wlan0"

typedef void (*hi_channel_rx_func)(unsigned char *msg_data, int len);
/*****************************************************************************
  2 ��������
*****************************************************************************/
int hi_channel_send_to_dev(unsigned char *buf, int len);

int hi_channel_init(void);

int hi_channel_deinit(void);

int hi_channel_register_rx_cb(hi_channel_rx_func cb);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
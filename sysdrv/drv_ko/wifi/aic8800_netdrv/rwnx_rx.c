/**
 ******************************************************************************
 *
 * @file rwnx_rx.c
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */
#include <linux/dma-mapping.h>
#include <linux/ieee80211.h>
#include <linux/etherdevice.h>
#include <net/ieee80211_radiotap.h>

#include "rwnx_defs.h"
#include "rwnx_rx.h"
#include "rwnx_tx.h"
#include "rwnx_prof.h"
#include "rwnx_events.h"
#include "rwnx_compat.h"
#include "aicwf_txrxif.h"
#include "aicwf_custom_utils.h"
#include "rwnx_main.h"
#include "rwnx_platform.h"

#ifdef CONFIG_VNET_MODE
// If define CONFIG_APP_FASYNC, please make sure that the length of message is less than CDEV_BUF_MAX.
extern struct rwnx_aic_chardev chardev;
aicwf_custom_msg_vnet g_custom_msg_vnet;
void rwnx_rx_handle_msg( void *dev, struct ipc_e2a_msg *msg)
{
    //printk("rwnx_rx_handle_msg 0x%X \r\n", msg->id);
    int loop_idx = 0;
    switch(msg->id) {
        case CUST_CMD_CONNECT_CFM:
            printk("SET-MCU-WLAN: start connect\r\n");
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, start connect\r\n", CUST_CMD_CONNECT_CFM);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif
            break;
        case CUST_CMD_CONNECT_RESULT_CFM:
            memcpy(&g_custom_msg_vnet.wlan_conn_fail_result, msg->param, sizeof(struct custom_msg_common));
            if(g_custom_msg_vnet.wlan_conn_fail_result == WLAN_CONN_FAIL) {
                printk("STA_CONNECT_RESULT: connect fail\r\n");
                //memcpy(chardev.mem, "STA_CONNECT_RESULT: connect fail\r\n", sizeof("STA_CONNECT_RESULT: connect fail\r\n"));
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, connect fail\r\n", CUST_CMD_CONNECT_RESULT_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else if(g_custom_msg_vnet.wlan_conn_fail_result == WLAN_PSWD_WRONG) {
                printk("STA_CONNECT_RESULT: passward wrong\r\n");
                //memcpy(chardev.mem, "STA_CONNECT_RESULT: passward wrong\r\n", sizeof("STA_CONNECT_RESULT: passward wrong\r\n"));
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, passward wrong\r\n", CUST_CMD_CONNECT_RESULT_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else {
                // other
            }
            break;
        case CUST_CMD_CONNECT_IND:
            // Allow linux-app to send data-pkg by vnet_dev.
            netif_tx_wake_all_queues(vnet_dev);
            g_custom_msg_vnet.wlan_status = WLAN_CONNECTED;
            g_custom_msg_vnet.aic_mode_status = AIC_MODE_WLAN;
            memcpy(&g_custom_msg_vnet.connect_ind, msg->param, sizeof(struct custom_msg_connect_ind));
            printk("MCU-WLAN-INDICATE: connect to \'%s\' (%3d dBm)\r\n",
                g_custom_msg_vnet.connect_ind.ussid, g_custom_msg_vnet.connect_ind.rssi);
            printk("ip: %d.%d.%d.%d,  gw: %d.%d.%d.%d\n",
                (g_custom_msg_vnet.connect_ind.ip >> 0 ) & 0xff,   (g_custom_msg_vnet.connect_ind.ip >> 8 ) & 0xff,
                (g_custom_msg_vnet.connect_ind.ip >> 16) & 0xff,   (g_custom_msg_vnet.connect_ind.ip >> 24) & 0xff,
                (g_custom_msg_vnet.connect_ind.gw >> 0 ) & 0xff,   (g_custom_msg_vnet.connect_ind.gw >> 8 ) & 0xff,
                (g_custom_msg_vnet.connect_ind.gw >> 16) & 0xff,   (g_custom_msg_vnet.connect_ind.gw >> 24) & 0xff);

            printk("VNET_DEV: Allow send data-pkg\n");
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, ssid: %s, rssi: %d, ip: %x, gw: %x\r\n", CUST_CMD_CONNECT_IND, g_custom_msg_vnet.connect_ind.ussid,
                    g_custom_msg_vnet.connect_ind.rssi, g_custom_msg_vnet.connect_ind.ip, g_custom_msg_vnet.connect_ind.gw);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif
            break;
        case CUST_CMD_DISCONNECT_CFM:
            printk("SET-MCU-WLAN: start disconnect\r\n");
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, start disconnect\r\n", CUST_CMD_DISCONNECT_CFM);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif
            break;
        case CUST_CMD_DISCONNECT_IND:
            // Don`t allow linux-app to send data-pkg by vnet_dev.
            netif_tx_stop_all_queues(vnet_dev);
            g_custom_msg_vnet.wlan_status = WLAN_DISCONNECT;
            g_custom_msg_vnet.aic_mode_status = AIC_MODE_IDLE;
            printk("MCU-WLAN-INDICATE: disconnect\r\n");
            printk("VNET_DEV: Stop send data-pkg\n");
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, disconnect\r\n", CUST_CMD_DISCONNECT_IND);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif
            break;
        case CUST_CMD_ENTER_SLEEP_CFM:
            printk("MCU-SLEEP-STATE: enter sleep\r\n");
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, enter sleep\r\n", CUST_CMD_ENTER_SLEEP_CFM);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif
            break;
        case CUST_CMD_EXIT_SLEEP_CFM:
            printk("MCU-SLEEP-STATE: exit sleep\r\n");
            break;
        case CUST_CMD_GET_MAC_ADDR_CFM:
            memcpy(&g_custom_msg_vnet.macaddr_cfm, msg->param, sizeof(struct custom_msg_mac_addr_cfm));
            printk("MCU-MAC-ADDR: %x.%x.%x.%x.%x.%x\n",
                g_custom_msg_vnet.macaddr_cfm.mac_addr[0], g_custom_msg_vnet.macaddr_cfm.mac_addr[1],
                g_custom_msg_vnet.macaddr_cfm.mac_addr[2], g_custom_msg_vnet.macaddr_cfm.mac_addr[3],
                g_custom_msg_vnet.macaddr_cfm.mac_addr[4], g_custom_msg_vnet.macaddr_cfm.mac_addr[5]);

            #ifndef CONFIG_FAST_INSMOD
            if(!g_custom_msg_vnet.comp_sign_get_mac_ready) {
                g_custom_msg_vnet.comp_sign_get_mac_ready = true;
                rwnx_platform_get_mac_ready();
            }
            #endif

            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, mac: %x.%x.%x.%x.%x.%x\r\n", CUST_CMD_GET_MAC_ADDR_CFM,
                g_custom_msg_vnet.macaddr_cfm.mac_addr[0], g_custom_msg_vnet.macaddr_cfm.mac_addr[1],
                g_custom_msg_vnet.macaddr_cfm.mac_addr[2], g_custom_msg_vnet.macaddr_cfm.mac_addr[3],
                g_custom_msg_vnet.macaddr_cfm.mac_addr[4], g_custom_msg_vnet.macaddr_cfm.mac_addr[5]);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif

            break;
        case CUST_CMD_GET_WLAN_STATUS_CFM:
            memcpy(&g_custom_msg_vnet.get_wlan_cfm, msg->param, sizeof(struct custom_msg_wlan_status_cfm));
            if(g_custom_msg_vnet.get_wlan_cfm.status == WLAN_DISCONNECT) {
                printk("GET-WLAN-STATUS: DISCONNECT\r\n");
                g_custom_msg_vnet.wlan_status = WLAN_DISCONNECT;
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, DISCONNECT\r\n", CUST_CMD_GET_WLAN_STATUS_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif

            } else if(g_custom_msg_vnet.get_wlan_cfm.status == WLAN_CONNECTED) {
                printk("GET-WLAN-STATUS: CONNECT to \'%s\' (%3d dBm)\r\n",
                    g_custom_msg_vnet.get_wlan_cfm.ussid, g_custom_msg_vnet.get_wlan_cfm.rssi);
                printk("ip: %d.%d.%d.%d,  gw: %d.%d.%d.%d\n",
                    (g_custom_msg_vnet.get_wlan_cfm.ip >> 0 ) & 0xff,   (g_custom_msg_vnet.get_wlan_cfm.ip >> 8 ) & 0xff,
                    (g_custom_msg_vnet.get_wlan_cfm.ip >> 16) & 0xff,   (g_custom_msg_vnet.get_wlan_cfm.ip >> 24) & 0xff,
                    (g_custom_msg_vnet.get_wlan_cfm.gw >> 0 ) & 0xff,   (g_custom_msg_vnet.get_wlan_cfm.gw >> 8 ) & 0xff,
                    (g_custom_msg_vnet.get_wlan_cfm.gw >> 16) & 0xff,   (g_custom_msg_vnet.get_wlan_cfm.gw >> 24) & 0xff);
                g_custom_msg_vnet.wlan_status = WLAN_CONNECTED;

                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, ssid: %s, rssi: %d, ip: %x, gw: %x\r\n", CUST_CMD_GET_WLAN_STATUS_CFM, g_custom_msg_vnet.get_wlan_cfm.ussid,
                    g_custom_msg_vnet.get_wlan_cfm.rssi, g_custom_msg_vnet.get_wlan_cfm.ip, g_custom_msg_vnet.get_wlan_cfm.gw);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else {
                // other
            }

            if(!g_custom_msg_vnet.comp_sign_get_wlan_ready) {
                g_custom_msg_vnet.comp_sign_get_wlan_ready = true;
                rwnx_platform_get_wlan_ready();
            }
            break;
        case CUST_CMD_START_AP_CFM:
            memcpy(&g_custom_msg_vnet.ap_status, msg->param, sizeof(struct custom_msg_common));
            if(g_custom_msg_vnet.ap_status == AIC_AP_START) {
                printk("MCU-AP-STATE: start AP success\n");
                g_custom_msg_vnet.aic_mode_status = AIC_MODE_AP_CONFIG;
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, start AP success\r\n", CUST_CMD_START_AP_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else if(g_custom_msg_vnet.ap_status == AIC_AP_CLOSE) {
                printk("MCU-AP-STATE: start AP fail\n");
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, start AP fail\r\n", CUST_CMD_START_AP_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else {
                // other
            }
            break;
        case CUST_CMD_ASSOC_AP_IND:
            printk("AP-INDICATE: assoc\n");
            memcpy(&g_custom_msg_vnet.ap_assoc_sta_addr_ind, msg->param, sizeof(struct custom_msg_ap_assoc_sta_ind));
            printk("%x.%x.%x.%x.%x.%x\n",
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[0], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[1],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[2], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[3],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[4], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[5]);
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, AP-ASSOC: %x.%x.%x.%x.%x.%x\r\n", CUST_CMD_ASSOC_AP_IND,
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[0], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[1],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[2], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[3],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[4], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[5]);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif

            break;
        case CUST_CMD_DISASSOC_AP_IND:
            printk("AP-INDICATE: disassoc\n");
            memcpy(&g_custom_msg_vnet.ap_assoc_sta_addr_ind, msg->param, sizeof(struct custom_msg_ap_assoc_sta_ind));
            printk("%x.%x.%x.%x.%x.%x\n",
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[0], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[1],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[2], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[3],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[4], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[5]);
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, AP-DISASSOC: %x.%x.%x.%x.%x.%x\r\n", CUST_CMD_DISASSOC_AP_IND,
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[0], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[1],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[2], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[3],
                    g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[4], g_custom_msg_vnet.ap_assoc_sta_addr_ind.sub_sta_addr[5]);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif

            break;
        case CUST_CMD_CHANGE_AP_MODE_CFM:
            printk("AP_MODE:\n");
            memcpy(&g_custom_msg_vnet.aic_mode_status, msg->param, sizeof(struct custom_msg_common));
            if(g_custom_msg_vnet.aic_mode_status == AIC_MODE_AP_CONFIG) {
                // Don`t allow linux-app to send data-pkg by vnet_dev.
                netif_tx_stop_all_queues(vnet_dev);
                printk("AIC_MODE_AP_CONFIG\n");
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, AIC_MODE_AP_CONFIG\r\n", CUST_CMD_CHANGE_AP_MODE_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else if(g_custom_msg_vnet.aic_mode_status == AIC_MODE_AP_DIRECT) {
                // Allow linux-app to send data-pkg by vnet_dev.
                netif_tx_wake_all_queues(vnet_dev);
                printk("AIC_MODE_AP_DIRECT\n");
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, AIC_MODE_AP_DIRECT\r\n", CUST_CMD_CHANGE_AP_MODE_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif

            } else {
                // other
            }
            break;
        case CUST_CMD_STOP_AP_CFM:
            g_custom_msg_vnet.ap_status = AIC_AP_CLOSE;
            g_custom_msg_vnet.aic_mode_status = AIC_MODE_IDLE;
            printk("MCU-AP-STATE: stop AP\n");
            #ifdef CONFIG_APP_FASYNC
            sprintf(chardev.mem, "MSG: %x, stop AP\r\n", CUST_CMD_STOP_AP_CFM);
            if(chardev.async_queue) {
                printk("Send signal\n");
                kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
            }
            #endif
            break;
        case CUST_CMD_SCAN_WIFI_CFM:
            printk("MCU-SCAN-WIFI:\n");
            memcpy(&g_custom_msg_vnet.scan_wifi_cfm, msg->param, msg->param_len);
            g_custom_msg_vnet.scan_wifi_cfm_ptr = g_custom_msg_vnet.scan_wifi_cfm;
            do {
                printk("%2d:(%3d dBm) CH=%3d AKM=%3d BSSID=%02x:%02x:%02x:%02x:%02x:%02x SSID=%s\n",
                    g_custom_msg_vnet.scan_wifi_cfm_ptr->scan_num, g_custom_msg_vnet.scan_wifi_cfm_ptr->rssi,
                    g_custom_msg_vnet.scan_wifi_cfm_ptr->channal,  g_custom_msg_vnet.scan_wifi_cfm_ptr->akm,
                    g_custom_msg_vnet.scan_wifi_cfm_ptr->bssid[0], g_custom_msg_vnet.scan_wifi_cfm_ptr->bssid[1],
                    g_custom_msg_vnet.scan_wifi_cfm_ptr->bssid[2], g_custom_msg_vnet.scan_wifi_cfm_ptr->bssid[3],
                    g_custom_msg_vnet.scan_wifi_cfm_ptr->bssid[4], g_custom_msg_vnet.scan_wifi_cfm_ptr->bssid[5],
                    g_custom_msg_vnet.scan_wifi_cfm_ptr->ssid);
                g_custom_msg_vnet.scan_wifi_cfm_ptr++;
                loop_idx++;
            } while(loop_idx!=32 && g_custom_msg_vnet.scan_wifi_cfm_ptr->scan_num!=0);
            break;
        case CUST_CMD_HOST_OTA_CFM:
            memcpy(&g_custom_msg_vnet.ota_status, msg->param, sizeof(struct custom_msg_common));
            if(g_custom_msg_vnet.ota_status == OTA_STEP_FLASH_ERASE_OK) {
                printk("MCU-OTA: flash erase OK.\n");
                aicwf_mcu_ota_confirm_ready();
            } else if(g_custom_msg_vnet.ota_status == OTA_STEP_FR_PKG_WRITE_OK) {
                //printk("MCU-OTA: pkg write OK.\n");
                aicwf_mcu_ota_confirm_ready();
            } else if(g_custom_msg_vnet.ota_status == OTA_STEP_LT_PKG_WRITE_OK) {
                //printk("MCU-OTA: last pkg write OK.\n");
                aicwf_mcu_ota_confirm_ready();
            } else if(g_custom_msg_vnet.ota_status == OTA_STEP_HEADER_WRITE_OK) {
                printk("MCU-OTA: finish OTA...\n");
                aicwf_mcu_ota_confirm_ready();
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, host-OTA success\r\n", CUST_CMD_HOST_OTA_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            } else {
                printk("MCU-OTA: OTA error: %d. Please restart OTA.\n", g_custom_msg_vnet.ota_status);
                #ifdef CONFIG_APP_FASYNC
                sprintf(chardev.mem, "MSG: %x, host-OTA fail\r\n", CUST_CMD_HOST_OTA_CFM);
                if(chardev.async_queue) {
                    printk("Send signal\n");
                    kill_fasync(&chardev.async_queue, SIGIO, POLL_IN);
                }
                #endif
            }
            break;
    }
}
#endif

static bool rwnx_rx_data_skb( struct sk_buff *skb)
{
    #ifdef CONFIG_RAWDATA_MODE
    int ret = 0;
    #endif

    /* Write metadata, and then pass to the receive level */
    if(skb == NULL) {
        txrx_err("skb is NULL\n");
        return 0;
    }
    #ifdef CONFIG_VNET_MODE
    if(vnet_dev == NULL) {
        txrx_err("vnet_dev is NULL\n");
        return 0;
    }
    skb_push(skb, 2);
    skb_reserve(skb, 2);  /* align IP on 16B boundary */
    skb->dev = vnet_dev;
    skb->protocol = eth_type_trans(skb, vnet_dev);

    if (in_interrupt()) {
        netif_rx(skb);
    } else {
        /*
        * If the receive is not processed inside an ISR, the softirqd must be woken explicitly to service the NET_RX_SOFTIRQ.
        * * In 2.6 kernels, this is handledby netif_rx_ni(), but in earlier kernels, we need to do it manually.
        */
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
        netif_rx_ni(skb);
        #else
        ulong flags;
        netif_rx(skb);
        local_irq_save(flags);
        RAISE_RX_SOFTIRQ();
        local_irq_restore(flags);
        #endif
    }

    #elif defined(CONFIG_RAWDATA_MODE)
    ret = nlaic_rx_rawdata(skb->data, skb->len);
    if (ret) {
        printk("rx rawdata err=%d\n", ret);
    }
    dev_kfree_skb(skb);
    #endif
    return 0;
}

void reord_rxframe_free(spinlock_t *lock, struct list_head *q, struct list_head *list)
{
    spin_lock_bh(lock);
    list_add(list, q);
    spin_unlock_bh(lock);
}

struct recv_msdu *reord_rxframe_alloc(spinlock_t *lock, struct list_head *q)
{
    struct recv_msdu *rxframe;

    spin_lock_bh(lock);
    if (list_empty(q)) {
        spin_unlock_bh(lock);
        return NULL;
    }
    rxframe = list_entry(q->next, struct recv_msdu, rxframe_list);
    list_del_init(q->next);
    spin_unlock_bh(lock);
    return rxframe;
}

int reord_single_frame_ind(struct aicwf_rx_priv *rx_priv, struct recv_msdu *prframe)
{
    struct list_head *rxframes_freequeue = NULL;
    struct sk_buff *skb = NULL;

    rxframes_freequeue = &rx_priv->rxframes_freequeue;
    skb = prframe->pkt;
    if (skb == NULL) {
        txrx_err("skb is NULL\n");
        return -1;
    }

    #ifdef CONFIG_VNET_MODE
    if(vnet_dev == NULL) {
        txrx_err("vnet_dev is NULL\n");
        return -1;
    }
    //printk("reord_single_frame_ind sn=%d, len=%d\n", prframe->seq_num, skb->len);

    skb->data = prframe->rx_data;
    skb_set_tail_pointer(skb, prframe->len);
    skb->len = prframe->len;
    skb->dev = vnet_dev;
    skb->protocol = eth_type_trans(skb, vnet_dev);

    memset(skb->cb, 0, sizeof(skb->cb));
    if (in_interrupt()) {
        netif_rx(skb);
    } else {
    /*
    * If the receive is not processed inside an ISR, the softirqd must be woken explicitly to service the NET_RX_SOFTIRQ.
    * * In 2.6 kernels, this is handledby netif_rx_ni(), but in earlier kernels, we need to do it manually.
    */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
    netif_rx_ni(skb);
#else
    ulong flags;
    netif_rx(skb);
    local_irq_save(flags);
    RAISE_RX_SOFTIRQ();
    local_irq_restore(flags);
#endif
    }

    #elif defined(CONFIG_RAWDATA_MODE)
    //TBD:
    #endif

    prframe->pkt = NULL;
    reord_rxframe_free(&rx_priv->freeq_lock, rxframes_freequeue, &prframe->rxframe_list);

    return 0;
}

bool reord_rxframes_process(struct aicwf_rx_priv *rx_priv, struct reord_ctrl *preorder_ctrl, int bforced)
{
    struct list_head *phead, *plist;
    struct recv_msdu *prframe;
    bool bPktInBuf = false;

    if (bforced == true) {
        phead = &preorder_ctrl->reord_list;
        if (list_empty(phead)) {
            return false;
        }

        plist = phead->next;
        prframe = list_entry(plist, struct recv_msdu, reord_pending_list);
        preorder_ctrl->ind_sn = prframe->seq_num;
    }

    phead = &preorder_ctrl->reord_list;
    if (list_empty(phead)) {
        return bPktInBuf;
    }

    list_for_each_entry(prframe, phead, reord_pending_list) {
        if (!SN_LESS(preorder_ctrl->ind_sn, prframe->seq_num)) {
            if (SN_EQUAL(preorder_ctrl->ind_sn, prframe->seq_num)) {
                preorder_ctrl->ind_sn = (preorder_ctrl->ind_sn + 1) & 0xFFF;
            }
        } else {
            bPktInBuf = true;
            break;
        }
    }

    return bPktInBuf;
}

void reord_rxframes_ind(struct aicwf_rx_priv *rx_priv,
    struct reord_ctrl *preorder_ctrl)
{
    struct list_head *phead, *plist;
    struct recv_msdu *prframe;

    phead = &preorder_ctrl->reord_list;
    while (1) {
        spin_lock_bh(&preorder_ctrl->reord_list_lock);
        if (list_empty(phead)) {
            spin_unlock_bh(&preorder_ctrl->reord_list_lock);
            break;
        }

        plist = phead->next;
        prframe = list_entry(plist, struct recv_msdu, reord_pending_list);

        if (!SN_LESS(preorder_ctrl->ind_sn, prframe->seq_num)) {
            list_del_init(&(prframe->reord_pending_list));
            spin_unlock_bh(&preorder_ctrl->reord_list_lock);
            reord_single_frame_ind(rx_priv, prframe);
        } else {
            spin_unlock_bh(&preorder_ctrl->reord_list_lock);
            break;
        }
    }
}
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
void reord_timeout_handler (ulong data)
#else
void reord_timeout_handler (struct timer_list *t)
#endif
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
    struct reord_ctrl *preorder_ctrl = (struct reord_ctrl *)data;
#else
    struct reord_ctrl *preorder_ctrl = from_timer(preorder_ctrl, t, reord_timer);
#endif
    struct aicwf_rx_priv *rx_priv = preorder_ctrl->rx_priv;

    if (reord_rxframes_process(rx_priv, preorder_ctrl, true)==true) {
        mod_timer(&preorder_ctrl->reord_timer, jiffies + msecs_to_jiffies(REORDER_UPDATE_TIME));
    }

    if(!work_pending(&preorder_ctrl->reord_timer_work))
        schedule_work(&preorder_ctrl->reord_timer_work);
}

void reord_timeout_worker(struct work_struct *work)
{
    struct reord_ctrl *preorder_ctrl = container_of(work, struct reord_ctrl, reord_timer_work);
    struct aicwf_rx_priv *rx_priv = preorder_ctrl->rx_priv;

    reord_rxframes_ind(rx_priv, preorder_ctrl);
    return ;
}

#ifdef CONFIG_VNET_MODE
static int  rwnx_reorder_process(struct aicwf_rx_priv *rx_priv, struct sk_buff  *skb,  u16 seq_num, u16 tid)
{
    int ret=0;
    u8 *mac;
    struct recv_msdu *pframe;
    struct reord_ctrl *preorder_ctrl;
    struct reord_ctrl_info *reord_info;
    struct ethhdr *eh = (struct ethhdr *)(skb->data);
    u8 *da = eh->h_dest;
    u8 is_mcast = ((*da) & 0x01)? 1 : 0;

    if (skb->len <= 14) {
        dev_kfree_skb(skb);
        return -1;
    }
    if (is_mcast) {
        return rwnx_rx_data_skb(skb);
    }

    //printk("rwnx_reorder_process %d %d\r\n", seq_num, tid);

    pframe = reord_rxframe_alloc(&rx_priv->freeq_lock, &rx_priv->rxframes_freequeue);
       if (!pframe) {
           dev_kfree_skb(skb);
           return -1;
       }

       INIT_LIST_HEAD(&pframe->reord_pending_list);
       pframe->seq_num = seq_num;
       pframe->tid = tid;
       pframe->rx_data = skb->data;
       pframe->len = skb->len;
       pframe->pkt = skb;
       preorder_ctrl = pframe->preorder_ctrl;

       mac = eh->h_dest;

       spin_lock_bh(&rx_priv->stas_reord_lock);
       list_for_each_entry(reord_info, &rx_priv->stas_reord_list, list) {
           if (!memcmp(mac, reord_info->mac_addr, ETH_ALEN)) {
               preorder_ctrl = &reord_info->preorder_ctrl[pframe->tid];
               break;
           }
       }

       if (&reord_info->list == &rx_priv->stas_reord_list) {
           reord_info = reord_init_sta(rx_priv, mac);
           if (!reord_info) {
               spin_unlock_bh(&rx_priv->stas_reord_lock);
               dev_kfree_skb(skb);
               return -1;
           }
           list_add_tail(&reord_info->list, &rx_priv->stas_reord_list);
           preorder_ctrl = &reord_info->preorder_ctrl[pframe->tid];
       } else {
           if(preorder_ctrl->enable == false) {
               preorder_ctrl->enable = true;
               preorder_ctrl->ind_sn = 0xffff;
               preorder_ctrl->wsize_b = AICWF_REORDER_WINSIZE;
               preorder_ctrl->rx_priv= rx_priv;
           }
       }
       spin_unlock_bh(&rx_priv->stas_reord_lock);

    //printk("rwnx_reorder_process Enable %d\r\n", preorder_ctrl->enable);
       if (preorder_ctrl->enable == false) {
           preorder_ctrl->ind_sn = pframe->seq_num;
           reord_single_frame_ind(rx_priv, pframe);
           preorder_ctrl->ind_sn = (preorder_ctrl->ind_sn + 1)%4096;
           return 0;
       }

    spin_lock_bh(&preorder_ctrl->reord_list_lock);
    if (reord_need_check(preorder_ctrl, pframe->seq_num)) {
        reord_single_frame_ind(rx_priv, pframe);
        spin_unlock_bh(&preorder_ctrl->reord_list_lock);
        return 0;
    }

    if (reord_rxframe_enqueue(preorder_ctrl, pframe)) {
        spin_unlock_bh(&preorder_ctrl->reord_list_lock);
        goto fail;
    }

    if (reord_rxframes_process(rx_priv, preorder_ctrl, false) == true) {
        if (!timer_pending(&preorder_ctrl->reord_timer)) {
            ret = mod_timer(&preorder_ctrl->reord_timer, jiffies + msecs_to_jiffies(REORDER_UPDATE_TIME));
        }
    } else {
    if(timer_pending(&preorder_ctrl->reord_timer)) {
        ret = del_timer(&preorder_ctrl->reord_timer);
    }
    }
    spin_unlock_bh(&preorder_ctrl->reord_list_lock);
    reord_rxframes_ind(rx_priv, preorder_ctrl);

    return 0;

fail:
    if (pframe->pkt){
        dev_kfree_skb(pframe->pkt);
        pframe->pkt = NULL;
    }
    reord_rxframe_free(&rx_priv->freeq_lock, &rx_priv->rxframes_freequeue, &pframe->rxframe_list);
    return ret;
}
#endif

struct reord_ctrl_info *reord_init_sta(struct aicwf_rx_priv* rx_priv, const u8 *mac_addr)
{
    u8 i = 0;
    struct reord_ctrl *preorder_ctrl = NULL;
    struct reord_ctrl_info *reord_info;
#ifdef AICWF_SDIO_SUPPORT
    struct aicwf_bus *bus_if = rx_priv->sdiodev->bus_if;
#else
    struct aicwf_bus *bus_if = rx_priv->usbdev->bus_if;
#endif

    if (bus_if->state == BUS_DOWN_ST || rx_priv == NULL) {
        printk("bad stat!\n");
        return NULL;
    }

    reord_info = kmalloc(sizeof(struct reord_ctrl_info), GFP_ATOMIC);
    if (!reord_info)
        return NULL;

    memcpy(reord_info->mac_addr, mac_addr, ETH_ALEN);
    for (i=0; i < 8; i++) {
        preorder_ctrl = &reord_info->preorder_ctrl[i];
        preorder_ctrl->enable = true;
        preorder_ctrl->ind_sn = 0xffff;
        preorder_ctrl->wsize_b = AICWF_REORDER_WINSIZE;
        preorder_ctrl->rx_priv= rx_priv;
        INIT_LIST_HEAD(&preorder_ctrl->reord_list);
        spin_lock_init(&preorder_ctrl->reord_list_lock);
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,14,0)
        init_timer(&preorder_ctrl->reord_timer);
        preorder_ctrl->reord_timer.data = (ulong) preorder_ctrl;
        preorder_ctrl->reord_timer.function = reord_timeout_handler;
#else
        timer_setup(&preorder_ctrl->reord_timer, reord_timeout_handler, 0);
#endif
        INIT_WORK(&preorder_ctrl->reord_timer_work, reord_timeout_worker);
    }

    return reord_info;
}

int reorder_list_flush_tid(struct aicwf_rx_priv *rx_priv, struct sk_buff *skb, u8 tid)
{
    struct reord_ctrl_info *reord_info;
    struct reord_ctrl *preorder_ctrl;
    struct ethhdr *eh = (struct ethhdr *)(skb->data);
    u8 *mac;
    unsigned long flags;
    u8 found = 0;
    struct list_head *phead, *plist;
    struct recv_msdu *prframe;
    int ret;
    //printk("flush:tid=%d", tid);

    mac = eh->h_dest;

    spin_lock_bh(&rx_priv->stas_reord_lock);
    list_for_each_entry(reord_info, &rx_priv->stas_reord_list, list) {
        if (!memcmp(mac, reord_info->mac_addr, ETH_ALEN)) {
            found = 1;
            preorder_ctrl = &reord_info->preorder_ctrl[tid];
            break;
        }
    }
    if (!found) {
        spin_unlock_bh(&rx_priv->stas_reord_lock);
        return 0;
    }
    spin_unlock_bh(&rx_priv->stas_reord_lock);

    if(preorder_ctrl->enable == false)
        return 0;
    spin_lock_irqsave(&preorder_ctrl->reord_list_lock, flags);
    phead = &preorder_ctrl->reord_list;
    while (1) {
        if (list_empty(phead)) {
            break;
        }
        plist = phead->next;
        prframe = list_entry(plist, struct recv_msdu, reord_pending_list);
        reord_single_frame_ind(rx_priv, prframe);
        list_del_init(&(prframe->reord_pending_list));
    }

    preorder_ctrl->enable = false;
    spin_unlock_irqrestore(&preorder_ctrl->reord_list_lock, flags);
    if (timer_pending(&preorder_ctrl->reord_timer))
        ret = del_timer_sync(&preorder_ctrl->reord_timer);
    cancel_work_sync(&preorder_ctrl->reord_timer_work);

    return 0;
}

int reord_need_check(struct reord_ctrl *preorder_ctrl, u16 seq_num)
{
    u8 wsize = preorder_ctrl->wsize_b;
    u16 wend = (preorder_ctrl->ind_sn + wsize -1) & 0xFFF;

    if (preorder_ctrl->ind_sn == 0xFFFF) {
        preorder_ctrl->ind_sn = seq_num;
    }

    if( SN_LESS(seq_num, preorder_ctrl->ind_sn)) {
        return -1;
    }

    if (SN_EQUAL(seq_num, preorder_ctrl->ind_sn)) {
        preorder_ctrl->ind_sn = (preorder_ctrl->ind_sn + 1) & 0xFFF;
    } else if (SN_LESS(wend, seq_num)) {
        if (seq_num >= (wsize-1))
            preorder_ctrl->ind_sn = seq_num-(wsize-1);
        else
            preorder_ctrl->ind_sn = 0xFFF - (wsize - (seq_num + 1)) + 1;
    }

    return 0;
}

int reord_rxframe_enqueue(struct reord_ctrl *preorder_ctrl, struct recv_msdu *prframe)
{
    struct list_head *preord_list = &preorder_ctrl->reord_list;
    struct list_head *phead, *plist;
    struct recv_msdu *pnextrframe;

    phead = preord_list;
    plist = phead->next;

    while(phead != plist) {
        pnextrframe = list_entry(plist, struct recv_msdu, reord_pending_list);
        if(SN_LESS(pnextrframe->seq_num, prframe->seq_num))	{
            plist = plist->next;
            continue;
        } else if(SN_EQUAL(pnextrframe->seq_num, prframe->seq_num)) {
            return -1;
        } else {
            break;
        }
    }
    list_add_tail(&(prframe->reord_pending_list), plist);

    return 0;
}
void dump_buf(u8 *data, u16 len)
{
    u16  i;
    for ( i = 0; i< len; i++)
        printk("%02X ", data[i]);
    printk("\r\n ");
}
#define MAC_FCTRL_QS_DATA_T                     0x88
#define MAC_FCTRL_DATA_T                        0x08
#define MAC_FCTRL_TYPE_MASK                     0x0F
#define MAC_FCTRL_TYPESUBTYPE_MASK              0xFC
#define MAC_FCNTRL_TODS                         0x0100
#define MAC_FCNTRL_FROMDS                       0x0200
#define MAC_FCHTRL_ORDER                        0x8000
void rwnx_rx_handle_data(struct aicwf_rx_priv *rx_priv,  struct sk_buff  *skb)
{
    #ifdef CONFIG_VNET_MODE
    u32 frame_offset = offsetof(rx_buff_tag, payload);
    rx_buff_tag *rx_buf = ( rx_buff_tag *)skb->data;
    struct hw_rxhdr_t *hw_rxhdr = (struct hw_rxhdr_t  *)&(rx_buf->hw_rxhdr);
    struct mac_hdr_t *mac_hdr = NULL;
    u32 mac_hdr_len = sizeof(struct mac_hdr_t ),  tid = 0, is_qos = 0;
    u8 ra[MAC_ADDR_LEN] = {0};
    u8 ta[MAC_ADDR_LEN] = {0};
    u8 ether_type[2] = {0};
    u8 pull_len = 0;
    u16 seq_num = 0;
    //printk("rwnx_rx_handle_data %d\r\n", frame_offset);

    //if (hw_rxhdr->flags_upload)
    {
        skb_pull(skb, frame_offset);
       //printk("%02x %02x %02x %02x %02x %02x\r\n", skb->data[0], skb->data[1], skb->data[2], skb->data[3], skb->data[4], skb->data[5]);
        mac_hdr = (struct mac_hdr_t *)skb->data;
        if ((mac_hdr->fctl & MAC_FCTRL_TYPE_MASK) == MAC_FCTRL_DATA_T)  {
            if ((mac_hdr->fctl & MAC_FCTRL_TYPESUBTYPE_MASK) == MAC_FCTRL_QS_DATA_T)  {
                mac_hdr_len += 2;
                tid = skb->data[sizeof(struct mac_hdr_t )] & 0x0F;
                is_qos = 1;
            }

            if (mac_hdr->fctl & MAC_FCHTRL_ORDER) {
                mac_hdr_len += 4;
            }
            // 802.11 -> 802.3
            if(mac_hdr-> fctl & MAC_FCNTRL_TODS )  {// to ds
                memcpy(ra, mac_hdr->addr3, MAC_ADDR_LEN);
                memcpy(ta, mac_hdr->addr2, MAC_ADDR_LEN);
            } else { //from ds
                memcpy(ta, mac_hdr->addr3, MAC_ADDR_LEN);
                memcpy(ra, mac_hdr->addr1, MAC_ADDR_LEN);
            }
            pull_len += (mac_hdr_len + 8);
            seq_num = ((mac_hdr->seq & 0xFFF0) >> 4);

            //printk("rwnx_rx_handle_data %d tid%d\r\n", hw_rxhdr->hwvect.decr_status, tid);

            switch(hw_rxhdr->hwvect.decr_status)
            {
                case RWNX_RX_HD_DECR_CCMP128:
                    pull_len += 8;//ccmp_header
                    //skb_pull(&skb->data[skb->len-8], 8); //ccmp_mic_len
                    memcpy(ether_type, &skb->data[mac_hdr_len + 6 + 8], 2);
                    break;
                case RWNX_RX_HD_DECR_TKIP:
                    pull_len += 8;//tkip_header
                    memcpy(ether_type, &skb->data[mac_hdr_len + 6 + 8], 2);
                    break;
                case RWNX_RX_HD_DECR_WEP:
                    pull_len += 4;//wep_header
                    memcpy(ether_type, &skb->data[mac_hdr_len + 6 + 4], 2);
                    break;
                default:
                    memcpy(ether_type, &skb->data[mac_hdr_len + 6], 2);
                    break;
            }
            //printk("rwnx_rx_handle_data  %d %02X%02X\r\n", pull_len, ether_type[0], ether_type[1]);

            skb_pull(skb, pull_len);
            skb_push(skb, 14);
            memcpy(skb->data, ra, MAC_ADDR_LEN);
            memcpy(&skb->data[6], ta, MAC_ADDR_LEN);
            memcpy(&skb->data[12], ether_type, 2);
        }

        if(is_qos && hw_rxhdr->flags_need_reord) {
            rwnx_reorder_process(rx_priv, skb, seq_num, tid);
        }  else if(is_qos  && !hw_rxhdr->flags_need_reord) {
            reorder_list_flush_tid(rx_priv, skb, tid);
            rwnx_rx_data_skb(skb);
        } else {
            rwnx_rx_data_skb(skb);
        }
    }

    #elif defined(CONFIG_RAWDATA_MODE)
    rwnx_rx_data_skb(skb);
    #endif

    return ;
}


/**
 ******************************************************************************
 *
 * @file main.c
 *
 * @brief Entry point of the RWNX driver
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */

#include <linux/module.h>
#include <linux/pci.h>
#include <linux/inetdevice.h>
//#include <net/cfg80211.h>
#include <net/ip.h>
#include <linux/etherdevice.h>
#include <linux/netdevice.h>
#include <net/netlink.h>
#include <net/genetlink.h>
#include <linux/wireless.h>
#include <linux/if_arp.h>
#include <linux/ctype.h>
#include <linux/random.h>
#include <linux/string.h>
#include "rwnx_defs.h"
//#include "rwnx_dini.h"
//#include "rwnx_msg_tx.h"
#include "reg_access.h"
#include "hal_desc.h"
#include "rwnx_debugfs.h"
#include "rwnx_cfgfile.h"
#include "rwnx_irqs.h"
//#include "rwnx_radar.h"
#include "rwnx_version.h"
#ifdef CONFIG_RWNX_BFMER
#include "rwnx_bfmer.h"
#endif //(CONFIG_RWNX_BFMER)
//#include "rwnx_tdls.h"
#include "rwnx_events.h"
#include "rwnx_compat.h"
#include "rwnx_version.h"
#include "rwnx_main.h"
#include "aicwf_txrxif.h"
#include "aicwf_custom_utils.h"
#ifdef AICWF_SDIO_SUPPORT
#include "aicwf_sdio.h"
#endif
#ifdef AICWF_USB_SUPPORT
#include "aicwf_usb.h"
#endif

#define RW_DRV_DESCRIPTION  "Driver for Linux"
#define RW_DRV_COPYRIGHT    "Copyright(c) 2015-2017 RivieraWaves"
#define RW_DRV_AUTHOR       "RivieraWaves S.A.S"

#define RWNX_PRINT_CFM_ERR(req) \
        printk(KERN_CRIT "%s: Status Error(%d)\n", #req, (&req##_cfm)->status)

void rwnx_data_dump(char* tag, void* data, unsigned long len){
	unsigned long i = 0;
	char* data_ = (char* )data;

	printk("%s %s len:(%lu)\r\n", __func__, tag, len);

	for (i = 0; i < len; i += 16){
	printk("%02X %02X %02X %02X %02X %02X %02X %02X  %02X %02X %02X %02X %02X %02X %02X %02X\r\n",
		data_[0 + i],
		data_[1 + i],
		data_[2 + i],
		data_[3 + i],
		data_[4 + i],
		data_[5 + i],
		data_[6 + i],
		data_[7 + i],
		data_[8 + i],
		data_[9 + i],
		data_[10 + i],
		data_[11 + i],
		data_[12 + i],
		data_[13 + i],
		data_[14 + i],
		data_[15 + i]);
		}

}

#define CMD_MAXARGS 30
static int parse_line (char *line, char *argv[])
{
    int nargs = 0;

    while (nargs < CMD_MAXARGS) {
        /* skip any white space */
        while ((*line == ' ') || (*line == '\t')) {
            ++line;
        }

        if (*line == '\0') {    /* end of line, no more args    */
            argv[nargs] = 0;
            return (nargs);
        }

        /* Argument include space should be bracketed by quotation mark */
        if (*line == '\"') {
            /* Skip quotation mark */
            line++;

            /* Begin of argument string */
            argv[nargs++] = line;

            /* Until end of argument */
            while(*line && (*line != '\"')) {
                ++line;
            }
        } else {
            argv[nargs++] = line;    /* begin of argument string    */

            /* find end of string */
            while(*line && (*line != ' ') && (*line != '\t')) {
                ++line;
            }
        }

        if (*line == '\0') {    /* end of line, no more args    */
            argv[nargs] = 0;
            return (nargs);
        }

        *line++ = '\0';         /* terminate current arg     */
    }

    printk("** Too many args (max. %d) **\n", CMD_MAXARGS);

    return (nargs);
}

unsigned int command_strtoul(const char *cp, char **endp, unsigned int base)
{
    unsigned int result = 0, value, is_neg=0;

    if (*cp == '0') {
        cp++;
        if ((*cp == 'x') && isxdigit(cp[1])) {
            base = 16;
            cp++;
        }
        if (!base) {
            base = 8;
        }
    }
    if (!base) {
        base = 10;
    }
    if (*cp == '-') {
        is_neg = 1;
        cp++;
    }
    while (isxdigit(*cp) && (value = isdigit(*cp) ? *cp - '0' : (islower(*cp) ? toupper(*cp) : *cp) - 'A' + 10) < base) {
        result = result * base + value;
        cp++;
    }
    if (is_neg)
        result = (unsigned int)((int)result * (-1));

    if (endp)
        *endp = (char *)cp;
    return result;
}

int handle_private_cmd(struct net_device *net, char *command, u32 cmd_len)
{
    int bytes_written = 0;
    char* para = NULL;
    char* cmd = NULL;
    char *argv[CMD_MAXARGS + 1];
    int argc;
    #ifdef CONFIG_RFTEST
    struct dbg_rftest_cmd_cfm cfm = {{0,}};
    u8_l mac_addr[6];
    cmd_rf_settx_t settx_param;
    cmd_rf_rx_t setrx_param;
    int freq;
    cmd_rf_getefuse_t getefuse_param;
    cmd_rf_setfreq_t cmd_setfreq;
    u8_l ana_pwr;
    u8_l dig_pwr;
    u8_l pwr;
    u8_l xtal_cap;
    u8_l xtal_cap_fine;
    u8_l vendor_info;
    #ifdef CONFIG_USB_BT
    int bt_index;
    u8_l dh_cmd_reset[4];
    u8_l dh_cmd_txdh[18];
    u8_l dh_cmd_rxdh[17];
    u8_l dh_cmd_stop[5];
    #endif
    #endif
    //u8_l buf[2];
    //s8_l freq_ = 0;
    //u8_l func = 0;

    RWNX_DBG(RWNX_FN_ENTRY_STR);

    if ((argc = parse_line(command, argv)) == 0) {
        return -1;
    }

    do {
        #ifdef CONFIG_RFTEST
        #ifdef AICWF_SDIO_SUPPORT
        struct rwnx_hw *p_rwnx_hw = g_rwnx_plat->sdiodev->rwnx_hw;
        #endif
        #ifdef AICWF_USB_SUPPORT
        struct rwnx_hw *p_rwnx_hw = g_rwnx_plat->usbdev->rwnx_hw;
        #endif
        if (strcasecmp(argv[0], "GET_RX_RESULT") ==0) {
            printk("get_rx_result\n");
            rwnx_send_rftest_req(p_rwnx_hw, GET_RX_RESULT, 0, NULL, &cfm);
            memcpy(command, &cfm.rftest_result[0], 8);
            bytes_written = 8;
        } else if (strcasecmp(argv[0], "SET_TX") == 0) {
            printk("set_tx\n");
            if (argc < 6) {
                printk("wrong param\n");
                break;
            }
            settx_param.chan = command_strtoul(argv[1], NULL, 10);
            settx_param.bw = command_strtoul(argv[2], NULL, 10);
            settx_param.mode = command_strtoul(argv[3], NULL, 10);
            settx_param.rate = command_strtoul(argv[4], NULL, 10);
            settx_param.length = command_strtoul(argv[5], NULL, 10);
            printk("txparam:%d,%d,%d,%d,%d\n", settx_param.chan, settx_param.bw,
                settx_param.mode, settx_param.rate, settx_param.length);
            rwnx_send_rftest_req(p_rwnx_hw, SET_TX, sizeof(cmd_rf_settx_t), (u8_l *)&settx_param, NULL);
        } else if (strcasecmp(argv[0], "SET_TXSTOP") == 0) {
            printk("settx_stop\n");
            rwnx_send_rftest_req(p_rwnx_hw, SET_TXSTOP, 0, NULL, NULL);
        } else if (strcasecmp(argv[0], "SET_TXTONE") == 0) {
            printk("set_tx_tone,argc:%d\n",argc);
            if ((argc == 2) || (argc == 3)) {
                printk("argv 1:%s\n",argv[1]);
                //u8_l func = (u8_l)command_strtoul(argv[1], NULL, 16);
                func = (u8_l)command_strtoul(argv[1], NULL, 16);
                //s8_l freq;
                if (argc == 3) {
                    printk("argv 2:%s\n",argv[2]);
                    freq_ = (u8_l)command_strtoul(argv[2], NULL, 10);
                } else {
                    freq_ = 0;
                };
                //u8_l buf[2] = {func, (u8_l)freq};
                buf[0] = func;
                buf[1] = (u8_l)freq_;
                rwnx_send_rftest_req(p_rwnx_hw, SET_TXTONE, argc - 1, buf, NULL);
            } else {
                printk("wrong args\n");
            }
        } else if (strcasecmp(argv[0], "SET_RX") == 0) {
            printk("set_rx\n");
            if (argc < 3) {
                printk("wrong param\n");
                break;
            }
            setrx_param.chan = command_strtoul(argv[1], NULL, 10);
            setrx_param.bw = command_strtoul(argv[2], NULL, 10);
            rwnx_send_rftest_req(p_rwnx_hw, SET_RX, sizeof(cmd_rf_rx_t), (u8_l *)&setrx_param, NULL);
        } else if (strcasecmp(argv[0], "SET_RXSTOP") == 0) {
            printk("set_rxstop\n");
            rwnx_send_rftest_req(p_rwnx_hw, SET_RXSTOP, 0, NULL, NULL);
        } else if (strcasecmp(argv[0], "SET_RX_METER") == 0) {
            printk("set_rx_meter\n");
            freq = (int)command_strtoul(argv[1], NULL, 10);
            rwnx_send_rftest_req(p_rwnx_hw, SET_RX_METER, sizeof(freq), (u8_l *)&freq, NULL);
        } else if (strcasecmp(argv[0], "SET_FREQ_CAL") == 0) {
            printk("set_freq_cal\n");
            if (argc < 2) {
                printk("wrong param\n");
                break;
            }
            cmd_setfreq.val = command_strtoul(argv[1], NULL, 16);
            printk("param:%x\r\n", cmd_setfreq.val);
            rwnx_send_rftest_req(p_rwnx_hw, SET_FREQ_CAL, sizeof(cmd_rf_setfreq_t), (u8_l *)&cmd_setfreq, &cfm);
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        } else if (strcasecmp(argv[0], "SET_FREQ_CAL_FINE") == 0) {
            printk("set_freq_cal_fine\n");
            if (argc < 2) {
                printk("wrong param\n");
                break;
            }
            cmd_setfreq.val = command_strtoul(argv[1], NULL, 16);
            printk("param:%x\r\n", cmd_setfreq.val);
            rwnx_send_rftest_req(p_rwnx_hw, SET_FREQ_CAL_FINE, sizeof(cmd_rf_setfreq_t), (u8_l *)&cmd_setfreq, &cfm);
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        } else if (strcasecmp(argv[0], "GET_EFUSE_BLOCK") == 0) {
            printk("get_efuse_block\n");
            if (argc < 2) {
                printk("wrong param\n");
                break;
            }
            getefuse_param.block = command_strtoul(argv[1], NULL, 10);
            rwnx_send_rftest_req(p_rwnx_hw, GET_EFUSE_BLOCK, sizeof(cmd_rf_getefuse_t), (u8_l *)&getefuse_param, &cfm);
            printk("get val=%x\r\n", cfm.rftest_result[0]);
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        } else if (strcasecmp(argv[0], "SET_POWER") == 0) {
            printk("set_power\n");
            ana_pwr = command_strtoul(argv[1], NULL, 16);
            dig_pwr = command_strtoul(argv[2], NULL, 16);
            pwr = (ana_pwr << 4 | dig_pwr);
            if (ana_pwr > 0xf || dig_pwr > 0xf) {
                printk("invalid param\r\n");
                break;
            }
            printk("pwr =%x\r\n", pwr);
            rwnx_send_rftest_req(p_rwnx_hw, SET_POWER, sizeof(pwr), (u8_l *)&pwr, NULL);
        } else if (strcasecmp(argv[0], "SET_XTAL_CAP")==0) {
            printk("set_xtal_cap\n");
            if (argc < 2) {
                printk("wrong param\n");
                break;
            }
            xtal_cap = command_strtoul(argv[1], NULL, 10);
            printk("xtal_cap =%x\r\n", xtal_cap);
            rwnx_send_rftest_req(p_rwnx_hw, SET_XTAL_CAP, sizeof(xtal_cap), (u8_l *)&xtal_cap, &cfm);
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        } else if (strcasecmp(argv[0], "SET_XTAL_CAP_FINE")==0) {
            printk("set_xtal_cap_fine\n");
            if (argc < 2) {
                printk("wrong param\n");
                break;
            }
            xtal_cap_fine = command_strtoul(argv[1], NULL, 10);
            printk("xtal_cap_fine =%x\r\n", xtal_cap_fine);
            rwnx_send_rftest_req(p_rwnx_hw, SET_XTAL_CAP_FINE, sizeof(xtal_cap_fine), (u8_l *)&xtal_cap_fine, &cfm);
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        } else if (strcasecmp(argv[0], "SET_MAC_ADDR")==0) {
            printk("set_mac_addr\n");
            if (argc < 7) {
                printk("wrong param\n");
                break;
            }
            mac_addr[5] = command_strtoul(argv[1], NULL, 16);
            mac_addr[4] = command_strtoul(argv[2], NULL, 16);
            mac_addr[3] = command_strtoul(argv[3], NULL, 16);
            mac_addr[2] = command_strtoul(argv[4], NULL, 16);
            mac_addr[1] = command_strtoul(argv[5], NULL, 16);
            mac_addr[0] = command_strtoul(argv[6], NULL, 16);
            printk("set macaddr:%x,%x,%x,%x,%x,%x\n", mac_addr[5], mac_addr[4], mac_addr[3], mac_addr[2], mac_addr[1], mac_addr[0]);
            rwnx_send_rftest_req(p_rwnx_hw, SET_MAC_ADDR, sizeof(mac_addr), (u8_l *)&mac_addr, NULL);
        } else if (strcasecmp(argv[0], "GET_MAC_ADDR")==0) {
            printk("get mac addr\n");
            rwnx_send_rftest_req(p_rwnx_hw, GET_MAC_ADDR, 0, NULL, &cfm);
            memcpy(command, &cfm.rftest_result[0], 8);
            bytes_written = 8;
            printk("0x%x,0x%x\n", cfm.rftest_result[0], cfm.rftest_result[1]);
        } else if (strcasecmp(argv[0], "SET_BT_MAC_ADDR") == 0) {
            printk("set_bt_mac_addr\n");
            if (argc < 7) {
                printk("wrong param\n");
                break;
            }
            mac_addr[5] = command_strtoul(argv[1], NULL, 16);
            mac_addr[4] = command_strtoul(argv[2], NULL, 16);
            mac_addr[3] = command_strtoul(argv[3], NULL, 16);
            mac_addr[2] = command_strtoul(argv[4], NULL, 16);
            mac_addr[1] = command_strtoul(argv[5], NULL, 16);
            mac_addr[0] = command_strtoul(argv[6], NULL, 16);
            printk("set bt macaddr:%x,%x,%x,%x,%x,%x\n", mac_addr[5], mac_addr[4], mac_addr[3], mac_addr[2], mac_addr[1], mac_addr[0]);
            rwnx_send_rftest_req(p_rwnx_hw, SET_BT_MAC_ADDR, sizeof(mac_addr), (u8_l *)&mac_addr, NULL);
        } else if (strcasecmp(argv[0], "GET_BT_MAC_ADDR")==0) {
            printk("get bt mac addr\n");
            rwnx_send_rftest_req(p_rwnx_hw, GET_BT_MAC_ADDR, 0, NULL, &cfm);
            memcpy(command, &cfm.rftest_result[0], 8);
            bytes_written = 8;
            printk("0x%x,0x%x\n", cfm.rftest_result[0], cfm.rftest_result[1]);
        } else if (strcasecmp(argv[0], "SET_VENDOR_INFO")==0) {
            vendor_info = command_strtoul(argv[1], NULL, 16);
            printk("set vendor info:%x\n", vendor_info);
            rwnx_send_rftest_req(p_rwnx_hw, SET_VENDOR_INFO, 1, &vendor_info, &cfm);
            memcpy(command, &cfm.rftest_result[0], 1);
            bytes_written = 1;
            printk("0x%x\n", cfm.rftest_result[0]);
        } else if (strcasecmp(argv[0], "GET_VENDOR_INFO")==0) {
            printk("get vendor info\n");
            rwnx_send_rftest_req(p_rwnx_hw, GET_VENDOR_INFO, 0, NULL, &cfm);
            memcpy(command, &cfm.rftest_result[0], 1);
            bytes_written = 1;
            printk("0x%x\n", cfm.rftest_result[0]);
        } else if (strcasecmp(argv[0], "GET_FREQ_CAL") == 0) {
            printk("get freq cal\n");
            rwnx_send_rftest_req(p_rwnx_hw, GET_FREQ_CAL, 0, NULL, &cfm);
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
            printk("cap=0x%x, cap_fine=0x%x\n", cfm.rftest_result[0] & 0x0000ffff, (cfm.rftest_result[0] >> 16) & 0x0000ffff);
        } else if (strcasecmp(argv[0], "RDWR_PWRMM") == 0) {
            printk("read/write txpwr manul mode\n");
            if (argc <= 1) { // read cur
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_PWRMM, 0, NULL, &cfm);
            } else { // write
                u8_l pwrmm = (u8_l)command_strtoul(argv[1], NULL, 16);
                pwrmm = (pwrmm) ? 1 : 0;
                printk("set pwrmm = %x\r\n", pwrmm);
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_PWRMM, sizeof(pwrmm), (u8_l *)&pwrmm, &cfm);
            }
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        } else if (strcasecmp(argv[0], "RDWR_PWRIDX") == 0) {
            u8_l func = 0;
            printk("read/write txpwr index\n");
            if (argc > 1) {
                func = (u8_l)command_strtoul(argv[1], NULL, 16);
            }
            if (func == 0) { // read cur
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_PWRIDX, 0, NULL, &cfm);
            } else if (func <= 2) { // write 2.4g/5g pwr idx
                if (argc > 3) {
                    u8_l type = (u8_l)command_strtoul(argv[2], NULL, 16);
                    u8_l pwridx = (u8_l)command_strtoul(argv[3], NULL, 10);
                    u8_l buf[3] = {func, type, pwridx};
                    printk("set pwridx:[%x][%x]=%x\r\n", func, type, pwridx);
                    rwnx_send_rftest_req(p_rwnx_hw, RDWR_PWRIDX, sizeof(buf), buf, &cfm);
                } else {
                    printk("wrong args\n");
                }
            } else {
                printk("wrong func: %x\n", func);
            }
            memcpy(command, &cfm.rftest_result[0], 7);
            bytes_written = 7;
        } else if (strcasecmp(argv[0], "RDWR_PWROFST") == 0) {
            u8_l func = 0;
            printk("read/write txpwr offset\n");
            if (argc > 1) {
                func = (u8_l)command_strtoul(argv[1], NULL, 16);
            }
            if (func == 0) { // read cur
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_PWROFST, 0, NULL, &cfm);
            } else if (func <= 2) { // write 2.4g/5g pwr ofst
                if (argc > 3) {
                    u8_l chgrp = (u8_l)command_strtoul(argv[2], NULL, 16);
                    s8_l pwrofst = (u8_l)command_strtoul(argv[3], NULL, 10);
                    u8_l buf[3] = {func, chgrp, (u8_l)pwrofst};
                    printk("set pwrofst:[%x][%x]=%d\r\n", func, chgrp, pwrofst);
                    rwnx_send_rftest_req(p_rwnx_hw, RDWR_PWROFST, sizeof(buf), buf, &cfm);
                } else {
                    printk("wrong args\n");
                }
            } else {
                printk("wrong func: %x\n", func);
            }
            memcpy(command, &cfm.rftest_result[0], 7);
            bytes_written = 7;
        } else if (strcasecmp(argv[0], "RDWR_DRVIBIT") == 0) {
            u8_l func = 0;
            printk("read/write pa drv_ibit\n");
            if (argc > 1) {
                func = (u8_l)command_strtoul(argv[1], NULL, 16);
            }
            if (func == 0) { // read cur
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_DRVIBIT, 0, NULL, &cfm);
            } else if (func == 1) { // write 2.4g pa drv_ibit
                if (argc > 2) {
                    u8_l ibit = (u8_l)command_strtoul(argv[2], NULL, 16);
                    u8_l buf[2] = {func, ibit};
                    printk("set drvibit:[%x]=%x\r\n", func, ibit);
                    rwnx_send_rftest_req(p_rwnx_hw, RDWR_DRVIBIT, sizeof(buf), buf, &cfm);
                } else {
                    printk("wrong args\n");
                }
            } else {
                printk("wrong func: %x\n", func);
            }
            memcpy(command, &cfm.rftest_result[0], 16);
            bytes_written = 16;
        } else if (strcasecmp(argv[0], "RDWR_EFUSE_PWROFST") == 0) {
            u8_l func = 0;
            printk("read/write txpwr offset into efuse\n");
            if (argc > 1) {
                func = (u8_l)command_strtoul(argv[1], NULL, 16);
            }
            if (func == 0) { // read cur
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_EFUSE_PWROFST, 0, NULL, &cfm);
            } else if (func <= 2) { // write 2.4g/5g pwr ofst
                if (argc > 3) {
                    u8_l chgrp = (u8_l)command_strtoul(argv[2], NULL, 16);
                    s8_l pwrofst = (u8_l)command_strtoul(argv[3], NULL, 10);
                    u8_l buf[3] = {func, chgrp, (u8_l)pwrofst};
                    printk("set efuse pwrofst:[%x][%x]=%d\r\n", func, chgrp, pwrofst);
                    rwnx_send_rftest_req(p_rwnx_hw, RDWR_EFUSE_PWROFST, sizeof(buf), buf, &cfm);
                } else {
                    printk("wrong args\n");
                }
            } else {
                printk("wrong func: %x\n", func);
            }
            memcpy(command, &cfm.rftest_result[0], 7);
            bytes_written = 7;
        } else if (strcasecmp(argv[0], "RDWR_EFUSE_DRVIBIT") == 0) {
            u8_l func = 0;
            printk("read/write pa drv_ibit into efuse\n");
            if (argc > 1) {
                func = (u8_l)command_strtoul(argv[1], NULL, 16);
            }
            if (func == 0) { // read cur
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_EFUSE_DRVIBIT, 0, NULL, &cfm);
            } else if (func == 1) { // write 2.4g pa drv_ibit
                if (argc > 2) {
                u8_l ibit = (u8_l)command_strtoul(argv[2], NULL, 16);
                u8_l buf[2] = {func, ibit};
                printk("set efuse drvibit:[%x]=%x\r\n", func, ibit);
                rwnx_send_rftest_req(p_rwnx_hw, RDWR_EFUSE_DRVIBIT, sizeof(buf), buf, &cfm);
                } else {
                    printk("wrong args\n");
                }
            } else {
                printk("wrong func: %x\n", func);
            }
            memcpy(command, &cfm.rftest_result[0], 4);
            bytes_written = 4;
        }
        #ifdef CONFIG_USB_BT
        else if (strcasecmp(argv[0], "BT_RESET") == 0) {
            if (argc == 5) {
                printk("btrf reset\n");
                for (bt_index = 0; bt_index < 4; bt_index++) {
                    dh_cmd_reset[bt_index] = command_strtoul(argv[bt_index+1], NULL, 16);
                    printk("0x%x ",dh_cmd_reset[bt_index]);
                }
                printk("\n");
            } else {
                printk("wrong param\n");
                break;
            }
            rwnx_send_rftest_req(p_rwnx_hw, BT_RESET, sizeof(dh_cmd_reset), (u8_l *)&dh_cmd_reset, NULL);
        } else if (strcasecmp(argv[0], "BT_TXDH") == 0) {
            if (argc == 19) {
                printk("btrf txdh\n");
                for (bt_index = 0; bt_index < 18; bt_index++) {
                    dh_cmd_txdh[bt_index] = command_strtoul(argv[bt_index+1], NULL, 16);
                    printk("0x%x ", dh_cmd_txdh[bt_index]);
                }
                printk("\n");
            } else {
                printk("wrong param\n");
                break;
            }
            rwnx_send_rftest_req(p_rwnx_hw, BT_TXDH, sizeof(dh_cmd_txdh), (u8_l *)&dh_cmd_txdh, NULL);
        } else if (strcasecmp(argv[0], "BT_RXDH") == 0) {
            if (argc == 18) {
                printk("btrf rxdh\n");
                for (bt_index = 0; bt_index < 17; bt_index++) {
                    dh_cmd_rxdh[bt_index] = command_strtoul(argv[bt_index+1], NULL, 16);
                    printk("0x%x ", dh_cmd_rxdh[bt_index]);
                }
                printk("\n");
            } else {
                printk("wrong param\n");
                break;
            }
            rwnx_send_rftest_req(p_rwnx_hw, BT_RXDH, sizeof(dh_cmd_rxdh), (u8_l *)&dh_cmd_rxdh, NULL);
        } else if (strcasecmp(argv[0], "BT_STOP") == 0) {
            if (argc == 6) {
                printk("btrf stop\n");
                for (bt_index = 0; bt_index < 5; bt_index++) {
                    dh_cmd_stop[bt_index] = command_strtoul(argv[bt_index+1], NULL, 16);
                    printk("0x%x ", dh_cmd_stop[bt_index]);
                }
                printk("\n");
            } else {
                printk("wrong param\n");
                break;
            }
            rwnx_send_rftest_req(p_rwnx_hw, BT_STOP, sizeof(dh_cmd_stop), (u8_l *)&dh_cmd_stop, NULL);
        }
        #endif
        else {
            printk("wrong cmd:%s in %s\n", cmd, __func__);
        }
        #endif
    } while(0);
    kfree(cmd);
    kfree(para);
    return bytes_written;
}

int android_priv_cmd(struct net_device *net, struct ifreq *ifr, int cmd)
{
#define PRIVATE_COMMAND_MAX_LEN 8192
#define PRIVATE_COMMAND_DEF_LEN 4096
    int ret = 0;
    char *command = NULL;
    int bytes_written = 0;
    android_wifi_priv_cmd priv_cmd;
    int buf_size = 0;

    RWNX_DBG(RWNX_FN_ENTRY_STR);

    ///todo: add our lock
    //net_os_wake_lock(net);


    /*if (!capable(CAP_NET_ADMIN)) {
        ret = -EPERM;
        goto exit;
    }*/
    if (!ifr->ifr_data) {
        ret = -EINVAL;
        goto exit;
    }

#ifdef CONFIG_COMPAT
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 6, 0))
    if (in_compat_syscall())
#else
    if (is_compat_task())
#endif
    {
        compat_android_wifi_priv_cmd compat_priv_cmd;
        if (copy_from_user(&compat_priv_cmd, ifr->ifr_data, sizeof(compat_android_wifi_priv_cmd))) {
        ret = -EFAULT;
            goto exit;
        }
        priv_cmd.buf = compat_ptr(compat_priv_cmd.buf);
        priv_cmd.used_len = compat_priv_cmd.used_len;
        priv_cmd.total_len = compat_priv_cmd.total_len;
    } else
#endif /* CONFIG_COMPAT */
    {
        if (copy_from_user(&priv_cmd, ifr->ifr_data, sizeof(android_wifi_priv_cmd))) {
            ret = -EFAULT;
            goto exit;
        }
    }
    if ((priv_cmd.total_len > PRIVATE_COMMAND_MAX_LEN) || (priv_cmd.total_len < 0)) {
        printk("%s: buf length invalid:%d\n", __FUNCTION__, priv_cmd.total_len);
        ret = -EINVAL;
        goto exit;
    }

    buf_size = max(priv_cmd.total_len, PRIVATE_COMMAND_DEF_LEN);
    command = kmalloc((buf_size + 1), GFP_KERNEL);

    if (!command)
    {
        printk("%s: failed to allocate memory\n", __FUNCTION__);
        ret = -ENOMEM;
        goto exit;
    }
    if (copy_from_user(command, priv_cmd.buf, priv_cmd.total_len)) {
        ret = -EFAULT;
        goto exit;
    }
    command[priv_cmd.total_len] = '\0';

    /* outputs */
    printk("%s: Android private cmd \"%s\" on %s\n", __FUNCTION__, command, ifr->ifr_name);
    printk("cmd = %d\n", cmd);
    printk("buf_size=%d\n", buf_size);


    bytes_written = handle_private_cmd(net, command, priv_cmd.total_len);
    if (bytes_written >= 0) {
        if ((bytes_written == 0) && (priv_cmd.total_len > 0)) {
            command[0] = '\0';
        }
        if (bytes_written >= priv_cmd.total_len) {
            printk("%s: err. bytes_written:%d >= buf_size:%d \n",
                __FUNCTION__, bytes_written, buf_size);
            goto exit;
        }
        bytes_written++;
        priv_cmd.used_len = bytes_written;
        if (copy_to_user(priv_cmd.buf, command, bytes_written)) {
            printk("%s: failed to copy data to user buffer\n", __FUNCTION__);
            ret = -EFAULT;
        }
    }
    else {
        /* Propagate the error */
        ret = bytes_written;
    }

exit:
    ///todo: add our unlock
    //net_os_wake_unlock(net);
    kfree(command);
    return ret;
}

#ifdef CONFIG_VNET_MODE
int custom_msg_cmd_parse(char *command, u32 cmd_len, u32 *argc, char buf[][APP_CMD_BUF_MAX])
{
    int i = 0, len = 0, ret = 0, val = 0;
    char *cmd_cpy = NULL, *token = NULL;

    cmd_cpy = kzalloc((cmd_len+1), GFP_KERNEL);
    memcpy(cmd_cpy, command, cmd_len);
    cmd_cpy[cmd_len] = '\0';

    token = strsep(&cmd_cpy, " ");
    while(token && i<APP_CMD_NUM_MAX) {
        len = strlen(token);
        if(len > APP_CMD_BUF_MAX) {
            kfree(cmd_cpy);
            return 0;
        }

        strncpy(buf[i], token, len);
        buf[i++][len] = '\0';
        token = strsep(&cmd_cpy, " ");
    }
    *argc = i;
    kfree(cmd_cpy);

    // application insure that command is not null, so buf[0] is not null.
    ret = kstrtouint(buf[0], 10, &val);
    if(!ret)
        return val;
    else
        // str2uint process falid.
        return ret;
}

extern struct rwnx_aic_chardev chardev;
int handle_custom_msg(struct net_device *net, char *command, u32 cmd_len)
{
    //struct dbg_custom_msg_cfm *cust_msg_cfm;
    //printk("cmd: %s, %ld\n",command, strlen(command));

    int ret = 0;
    u32 argc = 0;
    aicwf_custom_msg_app_cmd cust_app_cmd;

    char (*buf)[APP_CMD_BUF_MAX] = (char (*)[APP_CMD_BUF_MAX])kzalloc(APP_CMD_NUM_MAX*APP_CMD_BUF_MAX, GFP_KERNEL);
    if(!buf) {
        printk("%s: alloc buf fail\n", __FUNCTION__);
        return 0;
    }

    ret = custom_msg_cmd_parse(command, cmd_len, &argc, buf);
    if(!ret) {
        printk("Please input valid args: custom_msg vnet0 [mode] <arg1> <arg2> <arg3>\n");
        kfree(buf);
        return 0;
    }

    switch(ret) {
        case APP_CMD_CONNECT:
            printk("APP_CMD_CONNECT\n");
            if(argc != 3) {
                printk("Please input valid args: custom_msg vnet0 1 ssid password\n");
                break;
            }
            if(strlen(buf[1]) >= AP_SSID_BUF_MAX || strlen(buf[2]) >= AP_PSWD_BUF_MAX) {
                printk("ssid or password is too long.\n");
                break;
            }
            cust_app_cmd.connect_req.hdr.cmd_id = CUST_CMD_CONNECT_REQ;
            strncpy(cust_app_cmd.connect_req.ssid, buf[1], strlen(buf[1]));
            strncpy(cust_app_cmd.connect_req.pw, buf[2], strlen(buf[2]));
            cust_app_cmd.connect_req.ssid[strlen(buf[1])] = '\0';
            cust_app_cmd.connect_req.pw[strlen(buf[2])] = '\0';
            rwnx_tx_msg((u8 *)&cust_app_cmd.connect_req, sizeof(cust_app_cmd.connect_req));
            break;
        case APP_CMD_DISCONNECT:
            printk("APP_CMD_DISCONNECT\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_DISCONNECT_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_ENTER_SLEEP:
            printk("APP_CMD_ENTER_SLEEP\n");
            netif_tx_stop_all_queues(net);
            printk("VNET_DEV: Stop send data-pkg\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_ENTER_SLEEP_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_EXIT_SLEEP:
            printk("APP_CMD_EXIT_SLEEP\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_EXIT_SLEEP_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_GET_MAC_ADDR:
            printk("APP_CMD_GET_MAC_ADDR\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_GET_MAC_ADDR_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_GET_WLAN_STATUS:
            printk("APP_CMD_GET_WLAN_STATUS\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_GET_WLAN_STATUS_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_START_AP:
            printk("APP_CMD_START_AP\n");
            if(argc != 4) {
                printk("Please input valid args: custom_msg vnet0 7 ssid password band\n");
                break;
            }
            if(strlen(buf[1]) >= AP_SSID_BUF_MAX || strlen(buf[2]) >= AP_PSWD_BUF_MAX) {
                printk("String ssid or password is too long.\n");
                break;
            }
            cust_app_cmd.ap_req.hdr.cmd_id = CUST_CMD_START_AP_REQ;
            strncpy(cust_app_cmd.ap_req.ssid, buf[1], strlen(buf[1]));
            strncpy(cust_app_cmd.ap_req.pw, buf[2], strlen(buf[2]));
            cust_app_cmd.ap_req.ssid[strlen(buf[1])] = '\0';
            cust_app_cmd.ap_req.pw[strlen(buf[2])] = '\0';
            if(!strncasecmp(buf[3], "2.4G", 4))
                cust_app_cmd.ap_req.band = 0;
            else if(!strncasecmp(buf[3], "5G", 2))
                cust_app_cmd.ap_req.band = 1;
            else
                cust_app_cmd.ap_req.band = 0;
            rwnx_tx_msg((u8 *)&cust_app_cmd.ap_req, sizeof(cust_app_cmd.ap_req));
            break;
        case APP_CMD_CHANGE_AP_MODE:
            printk("APP_CMD_CHANGE_AP_MODE\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_CHANGE_AP_MODE_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_STOP_AP:
            printk("APP_CMD_STOP_AP\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_STOP_AP_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_SCAN_WIFI:
            printk("APP_CMD_SCAN_WIFI\n");
            cust_app_cmd.common_req.cmd_id = CUST_CMD_SCAN_WIFI_REQ;
            rwnx_tx_msg((u8 *)&cust_app_cmd.common_req, sizeof(cust_app_cmd.common_req));
            break;
        case APP_CMD_HOST_OTA:
            printk("APP_CMD_HOST_OTA\n");
            if(argc != 2) {
                printk("Please input valid args: custom_msg vnet0 10 /your-filepath/update.bin\n");
                break;
            }
            if(strlen(buf[1]) >= APP_CMD_BUF_MAX) {
                printk("String file_path is too long.\n");
                break;
            }
            memcpy(cust_app_cmd.file_path, buf[1], strlen(buf[1]));
            cust_app_cmd.file_path[strlen(buf[1])] = '\0';
            host_ota_test(cust_app_cmd.file_path);
            break;
    }
    kfree(buf);
    return 0;
}

int mcu_cust_msg(struct net_device *net, struct ifreq *ifr, int cmd)
{
#ifdef PRIVATE_COMMAND_MAX_LEN
#undef PRIVATE_COMMAND_MAX_LEN
#undef PRIVATE_COMMAND_DEF_LEN
#define PRIVATE_COMMAND_MAX_LEN 8192
#define PRIVATE_COMMAND_DEF_LEN 4096
#endif
    int ret = 0;
    char *command = NULL;
    int bytes_written = 0;
    android_wifi_priv_cmd priv_cmd;
    int buf_size = 0;

    RWNX_DBG(RWNX_FN_ENTRY_STR);

    ///todo: add our lock
    //net_os_wake_lock(net);

    /*if (!capable(CAP_NET_ADMIN)) {
        ret = -EPERM;
        goto exit;
    }*/
    if (!ifr->ifr_data) {
        ret = -EINVAL;
        goto exit;
    }

#ifdef CONFIG_COMPAT
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 6, 0))
    if (in_compat_syscall())
#else
    if (is_compat_task())
#endif
    {
        compat_android_wifi_priv_cmd compat_priv_cmd;
        if (copy_from_user(&compat_priv_cmd, ifr->ifr_data, sizeof(compat_android_wifi_priv_cmd))) {
            ret = -EFAULT;
            goto exit;
        }
        priv_cmd.buf = compat_ptr(compat_priv_cmd.buf);
        priv_cmd.used_len = compat_priv_cmd.used_len;
        priv_cmd.total_len = compat_priv_cmd.total_len;
    } else
#endif /* CONFIG_COMPAT */
    {
        if (copy_from_user(&priv_cmd, ifr->ifr_data, sizeof(android_wifi_priv_cmd))) {
            ret = -EFAULT;
            goto exit;
        }
    }
    if ((priv_cmd.total_len > PRIVATE_COMMAND_MAX_LEN) || (priv_cmd.total_len < 0)) {
        printk("%s: buf length invalid:%d\n", __FUNCTION__, priv_cmd.total_len);
        ret = -EINVAL;
        goto exit;
    }

    buf_size = max(priv_cmd.total_len, PRIVATE_COMMAND_DEF_LEN);
    command = kmalloc((buf_size + 1), GFP_KERNEL);

    if (!command)
    {
        printk("%s: failed to allocate memory\n", __FUNCTION__);
        ret = -ENOMEM;
        goto exit;
    }
    if (copy_from_user(command, priv_cmd.buf, priv_cmd.used_len)) {
        ret = -EFAULT;
        goto exit;
    }
    command[priv_cmd.used_len] = '\0';

    /* outputs */
    printk("%s: Devipc custom msg \"%s\" on %s\n", __FUNCTION__, command, ifr->ifr_name);
    //printk("cmd = %x\n", cmd);
    //printk("buf_size=%d\n", buf_size);


    bytes_written = handle_custom_msg(net, command, priv_cmd.used_len);
    if (bytes_written >= 0) {
        if ((bytes_written == 0) && (priv_cmd.total_len > 0)) {
            command[0] = '\0';
        }
        if (bytes_written >= priv_cmd.total_len) {
            printk("%s: err. bytes_written:%d >= buf_size:%d \n",
                __FUNCTION__, bytes_written, buf_size);
            goto exit;
        }
        bytes_written++;
        priv_cmd.used_len = bytes_written;
        if (copy_to_user(priv_cmd.buf, command, bytes_written)) {
            printk("%s: failed to copy data to user buffer\n", __FUNCTION__);
            ret = -EFAULT;
        }
    }
    else {
        /* Propagate the error */
        ret = bytes_written;
    }

exit:
    ///todo: add our unlock
    //net_os_wake_unlock(net);
    if (command)
        kfree(command);
    return ret;
}

static int rwnx_do_ioctl(struct net_device *net, struct ifreq *req, int cmd)
{
    int ret = 0;
    printk(" %s cmd %d\n", __func__, cmd);
    switch(cmd)
    {
        case IOCTL_HOSTAPD:
            printk("IOCTL_HOSTAPD\n");
            break;
        case IOCTL_WPAS:
            printk("IOCTL_WPAS\n");
            break;
        case SIOCDEVPRIVATE:
            printk("IOCTL SIOCDEVPRIVATE\n");
            break;
        case (SIOCDEVPRIVATE+1):
            printk("IOCTL PRIVATE\n");
            android_priv_cmd(net, req, cmd);
            break;
        case (SIOCDEVPRIVATE+2):
            printk("IOCTL PRIVATE+2\n");
            mcu_cust_msg(net, req, cmd);
            break;
        default:
            ret = -EOPNOTSUPP;
    }
    return ret;
}

#if 0
/**
 * struct net_device_stats* (*ndo_get_stats)(struct net_device *dev);
 *	Called when a user wants to get the network device usage
 *	statistics. Drivers must do one of the following:
 *	1. Define @ndo_get_stats64 to fill in a zero-initialised
 *	   rtnl_link_stats64 structure passed by the caller.
 *	2. Define @ndo_get_stats to update a net_device_stats structure
 *	   (which should normally be dev->stats) and return a pointer to
 *	   it. The structure may be changed asynchronously only if each
 *	   field is written atomically.
 *	3. Update dev->stats asynchronously and atomically, and define
 *	   neither operation.
 */
static struct net_device_stats *rwnx_get_stats(struct net_device *dev)
{
    struct rwnx_vif *vif = netdev_priv(dev);

    return &vif->net_stats;
}
#endif

/*********************************************************************
 * netdev callbacks
 ********************************************************************/
/**
 * int (*ndo_open)(struct net_device *dev);
 *     This function is called when network device transistions to the up
 *     state.
 *
 * - Start FW if this is the first interface opened
 * - Add interface at fw level
 */
extern aicwf_custom_msg_vnet g_custom_msg_vnet;
static int rwnx_open(struct net_device *dev)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);
    //netif_carrier_off(dev);
    netif_tx_start_all_queues(dev);
    if (!netif_carrier_ok(dev))
        netif_carrier_on(dev);

    if(g_custom_msg_vnet.wlan_status == WLAN_DISCONNECT &&
            g_custom_msg_vnet.ap_status == AIC_AP_CLOSE) {
        // Don`t allow linux-app to send data-pkg by vnet_dev.
        netif_tx_stop_all_queues(dev);
        printk("VNET_DEV: Stop send data-pkg\n");
    }
    return 0;
}

/**
 * int (*ndo_stop)(struct net_device *dev);
 *     This function is called when network device transistions to the down
 *     state.
 *
 * - Remove interface at fw level
 * - Reset FW if this is the last interface opened
 */
static int rwnx_close(struct net_device *dev)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);

    netdev_info(dev, "CLOSE");

    if (netif_carrier_ok(dev)) {
        netif_tx_stop_all_queues(dev);
        netif_carrier_off(dev);
    }

    return 0;
}

/**
 * int (*ndo_set_mac_address)(struct net_device *dev, void *addr);
 * This function is called when the Media Access Control address
 * needs to be changed. If this interface is not defined, the
 * mac address can not be changed.
 */
static int rwnx_set_mac_address(struct net_device *dev, void *addr)
{
    int ret;
    struct sockaddr *sa = addr;
    struct rwnx_vif *rwnx_vif = netdev_priv(dev);
    RWNX_DBG(RWNX_FN_ENTRY_STR);

    ret = eth_mac_addr(dev, sa);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
    memcpy(rwnx_vif->wdev.address, dev->dev_addr, 6);
#else
    memcpy(rwnx_vif->address, dev->dev_addr, 6);
#endif

    //rwnx_platform_set_mac_addr();
    return ret;
}

const struct net_device_ops rwnx_netdev_ops = {
    .ndo_open               = rwnx_open,
    .ndo_stop               = rwnx_close,
    .ndo_do_ioctl           = rwnx_do_ioctl,
    .ndo_start_xmit         = rwnx_start_xmit,
    //.ndo_get_stats          = rwnx_get_stats,
    //.ndo_select_queue       = rwnx_select_queue,
    .ndo_set_mac_address    = rwnx_set_mac_address,
    //.ndo_set_features       = rwnx_set_features,
    //.ndo_set_rx_mode        = rwnx_set_multicast_list,
};

void rwnx_netdev_setup(struct net_device *dev)
{
    ether_setup(dev);
    dev->priv_flags &= ~IFF_TX_SKB_SHARING;
    dev->netdev_ops = &rwnx_netdev_ops;
#if LINUX_VERSION_CODE <  KERNEL_VERSION(4, 12, 0)
    dev->destructor = free_netdev;
#else
    dev->needs_free_netdev = true;
#endif
    dev->watchdog_timeo = RWNX_TX_LIFETIME_MS;

    dev->needed_headroom = sizeof(struct rwnx_txhdr) + RWNX_SWTXHDR_ALIGN_SZ;
#ifdef CONFIG_RWNX_AMSDUS_TX
    dev->needed_headroom = max(dev->needed_headroom,
                               (unsigned short)(sizeof(struct rwnx_amsdu_txhdr)
                                                + sizeof(struct ethhdr) + 4
                                                + sizeof(rfc1042_header) + 2));
#endif /* CONFIG_RWNX_AMSDUS_TX */

    dev->hw_features = 0;
}


#elif defined(CONFIG_RAWDATA_MODE)

#define NLAIC_GENL_NAME     "nlaic"
#define NLAIC_GENL_VERSION  0x1

/*
 * Commands sent from userspace
 */

enum {
    NLAIC_CMD_UNSPEC = 0,   /* Reserved */
    NLAIC_CMD_ECHO,         /* user->kernel request/get-response */
    NLAIC_CMD_REPLY,        /* kernel->user event */
    NLAIC_CMD_REGISTER_USERID,  /* user register portid */
    NLAIC_CMD_RAWDATA,          /* tx/rx raw binary data */

    /* add new commands above here */

    /* used to define NUM_NLAIC_CMD & NLAIC_CMD_MAX below */
    __NLAIC_CMD_AFTER_LAST,
    NUM_NLAIC_CMD = __NLAIC_CMD_AFTER_LAST,
    NLAIC_CMD_MAX = __NLAIC_CMD_AFTER_LAST - 1
};

enum {
    NLAIC_ATTR_UNSPEC = 0,
    NLAIC_ATTR_MESG,        /* message string */
    NLAIC_ATTR_WORDVAL,     /* word value */
    NLAIC_ATTR_USERID,      /* userid */
    NLAIC_ATTR_RAWDATA,     /* raw binary data */

    /* add new attributes above here */

    /* used to define NUM_NLAIC_ATTR & NLAIC_ATTR_MAX below */
    __NLAIC_ATTR_AFTER_LAST,
    NUM_NLAIC_ATTR = __NLAIC_ATTR_AFTER_LAST,
    NLAIC_ATTR_MAX = __NLAIC_ATTR_AFTER_LAST - 1
};

struct nlaic_userid_s {
    struct net *net;
    u32 portid;
};

static struct nlaic_userid_s nlaic_userid = {
    .net = NULL,
    .portid = 0
};

#define NLAIC_MESG_LEN_MAX      1024
#define NLAIC_RAWDATA_LEN_MAX   1600

// DEBUG
#define DBG_RX_RAWDATA_EN       1

#if (DBG_RX_RAWDATA_EN)
int dbg_rx_cnt = 0;
int dbg_eagain_cnt = 0;
u64 dbg_start_ms = 0;
#endif

int nlaic_handle_echo(struct sk_buff *skb, struct genl_info *info);
int nlaic_register_userid(struct sk_buff *skb, struct genl_info *info);
int nlaic_tx_rawdata(struct sk_buff *skb, struct genl_info *info);

static const struct nla_policy nlaic_policy[NUM_NLAIC_ATTR] = {
    [NLAIC_ATTR_MESG]       = { .type = NLA_STRING, .len = NLAIC_MESG_LEN_MAX },
    [NLAIC_ATTR_WORDVAL]    = { .type = NLA_S32, .len = sizeof(s32) },
    [NLAIC_ATTR_USERID]     = { .type = NLA_S32, .len = sizeof(s32) },
    [NLAIC_ATTR_RAWDATA]    = { .type = NLA_BINARY, .len = NLAIC_RAWDATA_LEN_MAX },
};

static const struct genl_ops nlaic_ops[] = {
    {
        .cmd        = NLAIC_CMD_ECHO,
        .doit       = nlaic_handle_echo,
        #if (LINUX_VERSION_CODE < KERNEL_VERSION(5,2,0))
        .policy     = nlaic_policy,
        #endif
        .flags      = GENL_ADMIN_PERM,
    },
    {
        .cmd        = NLAIC_CMD_REGISTER_USERID,
        .doit       = nlaic_register_userid,
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(5,2,0))
        .policy     = nlaic_policy,
    #endif
        .flags      = GENL_ADMIN_PERM,
    },
    {
        .cmd        = NLAIC_CMD_RAWDATA,
        .doit       = nlaic_tx_rawdata,
        #if (LINUX_VERSION_CODE < KERNEL_VERSION(5,2,0))
        .policy     = nlaic_policy,
        #endif
        .flags      = GENL_ADMIN_PERM,
    },
};

static struct genl_family nlaic_family = {
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(4,10,0))
    .id         = GENL_ID_GENERATE,
    #else
    .ops        = nlaic_ops,
    .n_ops      = sizeof(nlaic_ops) / sizeof(struct genl_ops),
    #endif
    .name       = NLAIC_GENL_NAME,
    .version    = NLAIC_GENL_VERSION,
    .maxattr    = NLAIC_ATTR_MAX,
};

static int nlaic_alloc_reply(struct genl_info *info, u8 cmd, struct sk_buff **skbp, size_t size)
{
    struct sk_buff *skb;
    void *reply;

    /*
     * If new attributes are added, please revisit this allocation
     */
    skb = genlmsg_new(size, GFP_KERNEL);
    if (!skb)
        return -ENOMEM;

    if (!info)
        return -EINVAL;

    reply = genlmsg_put_reply(skb, info, &nlaic_family, 0, cmd);
    if (reply == NULL) {
        nlmsg_free(skb);
        return -EINVAL;
    }

    *skbp = skb;
    return 0;
}

static int nlaic_fill_reply(struct sk_buff *skb, int aggr, void *data, int len)
{
    /* add a netlink attribute to a socket buffer */
    return nla_put(skb, aggr, len, data);
}

static int nlaic_send_reply(struct sk_buff *skb, struct genl_info *info)
{
    struct genlmsghdr *genlhdr = nlmsg_data(nlmsg_hdr(skb));
    void *reply = genlmsg_data(genlhdr);

    genlmsg_end(skb, reply);

    return genlmsg_reply(skb, info);
}

static int cmd_attr_echo_message(struct genl_info *info)
{
    struct nlattr *na;
    char *msg;
    struct sk_buff *rep_skb;
    size_t size;
    int ret;

    na = info->attrs[NLAIC_ATTR_MESG];
    if (!na)
        return -EINVAL;

    msg = (char *)nla_data(na);
    pr_info("demo generic netlink receive echo mesg %s\n", msg);

    size = nla_total_size(strlen(msg)+1);

    ret = nlaic_alloc_reply(info, NLAIC_CMD_REPLY, &rep_skb, size);
    if (ret < 0)
        return ret;

    ret = nlaic_fill_reply(rep_skb, NLAIC_ATTR_MESG, msg, size);
    if (ret < 0)
        goto err;

    return nlaic_send_reply(rep_skb, info);

err:
    nlmsg_free(rep_skb);
    return ret;
}

static int cmd_attr_echo_data(struct genl_info *info)
{
    struct nlattr *na;
    s32    data;
    struct sk_buff *rep_skb;
    size_t size;
    int ret;

    na = info->attrs[NLAIC_ATTR_WORDVAL];
    if (!na)
        return -EINVAL;

    data = nla_get_s32(info->attrs[NLAIC_ATTR_WORDVAL]);
    pr_info("demo generic netlink receive echo data %d\n", data);

    size = nla_total_size(sizeof(s32));

    ret = nlaic_alloc_reply(info, NLAIC_CMD_REPLY, &rep_skb, size);
    if (ret < 0)
        return ret;

    ret = nla_put_s32(rep_skb, NLAIC_ATTR_WORDVAL, data);
    if (ret < 0)
        goto err;

    return nlaic_send_reply(rep_skb, info);

err:
    nlmsg_free(rep_skb);

    return ret;
}

u64 get_systime_ms(void)
{
    u64 cur_ms;
    #if LINUX_VERSION_CODE < KERNEL_VERSION(3, 17, 0)
    struct timespec ts;
    get_monotonic_boottime(&ts);
    cur_ms = (u64)ts.tv_sec * 1000 + div_u64(ts.tv_nsec, 1000000);
    #elif LINUX_VERSION_CODE < KERNEL_VERSION(5, 6, 0)
    struct timespec ts;
    ts = ktime_to_timespec(ktime_get_boottime());
    cur_ms = (u64)ts.tv_sec * 1000 + div_u64(ts.tv_nsec, 1000000);
    #else
    struct timespec64 ts;
    ts = ktime_to_timespec64(ktime_get_boottime());
    cur_ms = (u64)ts.tv_sec * 1000 + div_u64(ts.tv_nsec, 1000000);
    #endif
    return cur_ms;
}

int nlaic_rx_rawdata(u8 *data, int len)
{
    int ret = 0;
    if (nlaic_userid.net) {
        struct sk_buff *skb;
        size_t size;
        void *hdr;
        size = nla_total_size(len);
        skb = genlmsg_new(size, GFP_KERNEL);
        if (!skb) {
            printk("genlmsg_new failed\n");
            return -ENOMEM;
        }

        hdr = genlmsg_put(skb, nlaic_userid.portid, 0, &nlaic_family, 0, NLAIC_CMD_RAWDATA);
        if (!hdr) {
            printk("genlmsg_put failed\n");
            nlmsg_free(skb);
            return -ENOMEM;
        }
        nla_put(skb, NLAIC_ATTR_RAWDATA, len, data);
        genlmsg_end(skb, hdr);
        #if 1
        ret = genlmsg_unicast(nlaic_userid.net, skb, nlaic_userid.portid);
        #if (DBG_RX_RAWDATA_EN)
        if (ret == -EAGAIN) {
            dbg_eagain_cnt++;
            ret = 0;
        }
        #endif
        #else
        do {
            ret = genlmsg_unicast(nlaic_userid.net, skb, nlaic_userid.portid);
            if (!ret) {
                break;
            } else if (ret == -EAGAIN) {
                #if (DBG_RX_RAWDATA_EN)
                if (retry_cnt == 0) {
                    dbg_eagain_cnt++;
                }
                #endif
                retry_cnt++;
                if (retry_cnt > 4) {
                    printk("retry %d times, giveup\n", retry_cnt);
                    ret = 0;
                    break;
                }
            } else {
                printk("genlmsg_unicast failed, ret=%d\n", ret);
                break;
            }
        } while (1);
        #endif
        #if (DBG_RX_RAWDATA_EN)
        dbg_rx_cnt++;
        if ((char)data[1] == 'S') {
            dbg_start_ms = get_systime_ms();
            printk("[%lld] start", dbg_start_ms);
        } else if ((char)data[1] == 'E') {
            u64 dbg_end_ms = get_systime_ms();
            printk("[%lld] cnt=%d,eag=%d,cost=%lld\n", dbg_end_ms, dbg_rx_cnt, dbg_eagain_cnt, (dbg_end_ms - dbg_start_ms));
        }
        #endif
    } else {
        ret = -EPERM;
    }
    return ret;
}

int nlaic_handle_echo(struct sk_buff *skb, struct genl_info *info)
{
    if (info->attrs[NLAIC_ATTR_MESG])
        return cmd_attr_echo_message(info);
    else if (info->attrs[NLAIC_ATTR_WORDVAL])
        return cmd_attr_echo_data(info);
    else
        return -EINVAL;
}

int nlaic_register_userid(struct sk_buff *skb, struct genl_info *info)
{
    if (!info->attrs[NLAIC_ATTR_USERID]) {
        return -EINVAL;
    }
    //if (!nlaic_userid.net)
    {
        nlaic_userid.net = genl_info_net(info);
        nlaic_userid.portid = info->snd_portid;
        //printk("nlaic_register_userid, portid=%d\n", info->snd_portid);
    }
    return 0;
}

int nlaic_tx_rawdata(struct sk_buff *skb, struct genl_info *info)
{
    int ret, len;
    u8 *buf;
    struct sk_buff *new_skb;
    struct nlattr *na = info->attrs[NLAIC_ATTR_RAWDATA];
    int headroom = 4;
    if (!na) {
        return -EINVAL;
    }
    buf = nla_data(na);
    len = nla_len(na);
    new_skb = dev_alloc_skb(len + headroom);
    if (!new_skb) {
        return -ENOMEM;
    }
    skb_pull(new_skb, headroom);
    /*printk("nlaic_tx_rawdata, len=%d\n", len);
    int idx;
    for (idx = 0; idx < len; idx+=4) {
        printk(" %02x %02x %02x %02x\n", buf[idx+0], buf[idx+1], buf[idx+2], buf[idx+3]);
    }*/
    memcpy(skb_put(new_skb, len), buf, len);
    ret = rwnx_tx_data(new_skb);
    if (ret) {
        dev_kfree_skb(new_skb);
        pr_err("tx data fail, %d\n", ret);
        return ret;
    }
    return 0;
}

int rwnx_nlaic_init(struct rwnx_plat *rwnx_plat, void **platform_data)
{
    int ret;
    pr_info("demo generic netlink module %d init...\n", NLAIC_GENL_VERSION);

    #if (LINUX_VERSION_CODE < KERNEL_VERSION(3,13,0))
    ret = genl_register_ops(&nlaic_family, nlaic_ops);
    #elif (LINUX_VERSION_CODE < KERNEL_VERSION(4,10,0))
    ret = genl_register_family_with_ops(&nlaic_family, nlaic_ops);
    #else
    ret = genl_register_family(&nlaic_family);
    #endif
    if (ret != 0) {
        pr_err("failed to init demo generic netlink example module\n");
        return ret;
    }

    pr_info("demo generic netlink module init success\n");
    return 0;
}

void rwnx_nlaic_deinit(void)
{
    int ret;
    pr_info("demo generic netlink deinit.\n");

    ret = genl_unregister_family(&nlaic_family);
    if(ret != 0) {
        pr_err("faled to unregister family:%i\n", ret);
    }
}
#endif

/*********************************************************************
 * Init/Exit functions
 *********************************************************************/

static void aicsmac_driver_register(void)
{
#ifdef AICWF_SDIO_SUPPORT
    aicwf_sdio_register();
#endif
#ifdef AICWF_USB_SUPPORT
    aicwf_usb_register();
#endif
#ifdef AICWF_PCIE_SUPPORT
    aicwf_pcie_register();
#endif
}

//static DECLARE_WORK(aicsmac_driver_work, aicsmac_driver_register);

struct completion hostif_register_done;

#define REGISTRATION_TIMEOUT                     6000

void aicwf_hostif_ready(void)
{
    complete(&hostif_register_done);
}

#define NB_DEVICE    2
#define TERM_DEV_ID  0
#define TRACE_DEV_ID 1

extern const struct file_operations rwnx_trace_ops;
extern const struct file_operations rwnx_term_ops;

static int __init rwnx_mod_init(void)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);
    rwnx_print_version();
    printk("RELEASE DATE:%s \r\n", RELEASE_DATE);

    init_completion(&hostif_register_done);

    aicsmac_driver_register();

#ifdef AICWF_SDIO_SUPPORT
    if ((wait_for_completion_timeout(&hostif_register_done, msecs_to_jiffies(REGISTRATION_TIMEOUT)) == 0)) {
        printk("register_driver timeout or error\n");
        aicwf_sdio_exit();
    return -ENODEV;
    }
#endif /* AICWF_SDIO_SUPPORT */

#ifdef AICWF_USB_SUPPORT
    if ((wait_for_completion_timeout(&hostif_register_done, msecs_to_jiffies(REGISTRATION_TIMEOUT)) == 0)) {
        printk("register_driver timeout or error\n");
        aicwf_usb_exit();
        return -ENODEV;
    }
#endif /*AICWF_USB_SUPPORT */


#ifdef AICWF_PCIE_SUPPORT
    return rwnx_platform_register_drv();
#else
    return 0;
#endif
}

static void __exit rwnx_mod_exit(void)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);

#ifdef AICWF_PCIE_SUPPORT
    rwnx_platform_unregister_drv();
#endif

#ifdef AICWF_SDIO_SUPPORT
    aicwf_sdio_exit();
#endif

#ifdef AICWF_USB_SUPPORT
    aicwf_usb_exit();
#endif
}

module_init(rwnx_mod_init);
module_exit(rwnx_mod_exit);
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);

MODULE_FIRMWARE(RWNX_CONFIG_FW_NAME);

MODULE_DESCRIPTION(RW_DRV_DESCRIPTION);
MODULE_VERSION(RWNX_VERS_MOD);
MODULE_AUTHOR(RW_DRV_COPYRIGHT " " RW_DRV_AUTHOR);
MODULE_LICENSE("GPL");


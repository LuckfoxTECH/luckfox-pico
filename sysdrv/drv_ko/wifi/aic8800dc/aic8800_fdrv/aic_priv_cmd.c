/**
 ******************************************************************************
 *
 * @file aic_priv_cmd.c
 *
 * @brief Entry point of the AIC driver
 *
 * Copyright (C) Aicsemi 2018-2024
 *
 ******************************************************************************
 */

#include <linux/netdevice.h>
#include <linux/ctype.h>
#include "rwnx_defs.h"
#include "rwnx_msg_tx.h"
#include "rwnx_debugfs.h"
#include "rwnx_main.h"
#include "aicwf_sdio.h"
#include "aic_priv_cmd.h"

extern int testmode;
static void print_help(const char *cmd);
struct dbg_rftest_cmd_cfm cfm = {{0,}};

#ifdef CONFIG_RFTEST
enum {
	SET_TX,
	SET_TXSTOP,
	SET_TXTONE,
	SET_RX,
	GET_RX_RESULT,
	SET_RXSTOP,
	SET_RX_METER,
	SET_POWER,
	SET_XTAL_CAP,
	SET_XTAL_CAP_FINE,
	GET_EFUSE_BLOCK,
	SET_FREQ_CAL,
	SET_FREQ_CAL_FINE,
	GET_FREQ_CAL,
	SET_MAC_ADDR,
	GET_MAC_ADDR,
	SET_BT_MAC_ADDR,
	GET_BT_MAC_ADDR,
	SET_VENDOR_INFO,
	GET_VENDOR_INFO,
	RDWR_PWRMM,
	RDWR_PWRIDX,
	RDWR_PWRLVL = RDWR_PWRIDX,
	RDWR_PWROFST,
	RDWR_DRVIBIT,
	RDWR_EFUSE_PWROFST,
	RDWR_EFUSE_DRVIBIT,
	SET_PAPR,
	SET_CAL_XTAL,
	GET_CAL_XTAL_RES,
	SET_COB_CAL,
	GET_COB_CAL_RES,
	RDWR_EFUSE_USRDATA,
	SET_NOTCH,
	RDWR_PWROFSTFINE,
	RDWR_EFUSE_PWROFSTFINE,
	RDWR_EFUSE_SDIOCFG,
	RDWR_EFUSE_USBVIDPID,
	SET_SRRC,
	SET_FSS,
	RDWR_EFUSE_HE_OFF,
	SET_USB_OFF,
	SET_PLL_TEST,

};

typedef struct {
	u8_l chan;
	u8_l bw;
	u8_l mode;
	u8_l rate;
	u16_l length;
	u16_l tx_intv_us;
	s8_l max_pwr;
} cmd_rf_settx_t;

typedef struct {
	u8_l val;
} cmd_rf_setfreq_t;

typedef struct {
	u8_l chan;
	u8_l bw;
} cmd_rf_rx_t;

typedef struct {
	u8_l block;
} cmd_rf_getefuse_t;

typedef struct {
	u8_l dutid;
	u8_l chip_num;
	u8_l dis_xtal;
} cmd_rf_setcobcal_t;

typedef struct {
	u16_l dut_rcv_golden_num;
	u8_l golden_rcv_dut_num;
	s8_l rssi_static;
	s8_l snr_static;
	s8_l dut_rssi_static;
	u16_l reserved;
} cob_result_ptr_t;
#endif

typedef struct
{
	u8_l func;
	u8_l cnt;
	u8_l reserved[2];
	u32_l usrdata[3]; // 3 words totally
} cmd_ef_usrdata_t;

#define CMD_MAXARGS 10
#define POWER_LEVEL_INVALID_VAL     (127)

#if 0//#include <linux/ctype.h>
#define isblank(c)		((c) == ' ' || (c) == '\t')
#define isascii(c)		(((unsigned char)(c)) <= 0x7F)

static int isdigit(unsigned char c)
{
	return ((c >= '0') && (c <='9'));
}

static int isxdigit(unsigned char c)
{
	if ((c >= '0') && (c <='9'))
		return 1;
	if ((c >= 'a') && (c <='f'))
		return 1;
	if ((c >= 'A') && (c <='F'))
		return 1;
	return 0;
}

static int islower(unsigned char c)
{
	return ((c >= 'a') && (c <='z'));
}

static unsigned char toupper(unsigned char c)
{
	if (islower(c))
		c -= 'a'-'A';
	return c;
}
#endif

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
			return nargs;
		}

		/* Argument include space should be bracketed by quotation mark */
		if (*line == '\"') {
			/* Skip quotation mark */
			line++;

			/* Begin of argument string */
			argv[nargs++] = line;

			/* Until end of argument */
			while (*line && (*line != '\"')) {
				++line;
			}
		} else {
			argv[nargs++] = line;    /* begin of argument string    */

			/* find end of string */
			while (*line && (*line != ' ') && (*line != '\t')) {
				++line;
			}
		}

		if (*line == '\0') {    /* end of line, no more args    */
			argv[nargs] = 0;
			return nargs;
		}

		*line++ = '\0';         /* terminate current arg     */
	}

	printk("** Too many args (max. %d) **\n", CMD_MAXARGS);

	return nargs;
}

unsigned int command_strtoul(const char *cp, char **endp, unsigned int base)
{
	unsigned int result = 0, value, is_neg = 0;

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

/*
 * aic_priv_cmd handers.
 */
static int aic_priv_cmd_set_tx (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_rf_settx_t settx_param;

	if (argc < 6)
		return -EINVAL;

	settx_param.chan = command_strtoul(argv[1], NULL, 10);
	settx_param.bw = command_strtoul(argv[2], NULL, 10);
	settx_param.mode = command_strtoul(argv[3], NULL, 10);
	settx_param.rate = command_strtoul(argv[4], NULL, 10);
	settx_param.length = command_strtoul(argv[5], NULL, 10);
	if (argc > 6) {
		settx_param.tx_intv_us = command_strtoul(argv[6], NULL, 10);
	} else {
		settx_param.tx_intv_us = 10000; // set default val 10ms
	}
	settx_param.max_pwr = POWER_LEVEL_INVALID_VAL;
	AICWFDBG(LOGINFO, "txparam:%d,%d,%d,%d,%d,%d\n", settx_param.chan, settx_param.bw,
		settx_param.mode, settx_param.rate, settx_param.length, settx_param.tx_intv_us);
	rwnx_send_rftest_req(rwnx_hw, SET_TX, sizeof(cmd_rf_settx_t), (u8_l *)&settx_param, NULL);
	return 0;
}

static int aic_priv_cmd_set_txstop (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	rwnx_send_rftest_req(rwnx_hw, SET_TXSTOP, 0, NULL, NULL);
	return 0;
}

static int aic_priv_cmd_set_rx (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_rf_rx_t setrx_param;

	if (argc < 3) {
		return -EINVAL;
	}
	setrx_param.chan = command_strtoul(argv[1], NULL, 10);
	setrx_param.bw = command_strtoul(argv[2], NULL, 10);
	rwnx_send_rftest_req(rwnx_hw, SET_RX, sizeof(cmd_rf_rx_t), (u8_l *)&setrx_param, NULL);
	return 0;
}

static int aic_priv_cmd_get_rx_result (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	rwnx_send_rftest_req(rwnx_hw, GET_RX_RESULT, 0, NULL, &cfm);
	memcpy(command, &cfm.rftest_result[0], 8);
	return 8;
}

static int aic_priv_cmd_set_rxstop (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	rwnx_send_rftest_req(rwnx_hw, SET_RXSTOP, 0, NULL, NULL);
	return 0;
}

static int aic_priv_cmd_set_tx_tone (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	u8_l buf[2];
	s8_l freq_ = 0;

	AICWFDBG(LOGINFO, "%s argc:%d\n", argv[0], argc);
	if ((argc == 2) || (argc == 3)) {
		AICWFDBG(LOGINFO, "argv 1:%s\n",argv[1]);
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
		if (argc == 3) {
			AICWFDBG(LOGINFO, "argv 2:%s\n",argv[2]);
			freq_ = (u8_l)command_strtoul(argv[2], NULL, 10);
		} else {
			freq_ = 0;
		};
		buf[0] = func;
		buf[1] = (u8_l)freq_;
		rwnx_send_rftest_req(rwnx_hw, SET_TXTONE, argc - 1, buf, NULL);
	} else {
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_set_rx_meter (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	s8_l freq = 0;

	freq = (int)command_strtoul(argv[1], NULL, 10);
	rwnx_send_rftest_req(rwnx_hw, SET_RX_METER, sizeof(freq), (u8_l *)&freq, NULL);
	return 0;
}

static int aic_priv_cmd_set_set_power (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l ana_pwr;
	u8_l dig_pwr;
	u8_l pwr;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	if (dev->chipid == PRODUCT_ID_AIC8801) {
		ana_pwr = command_strtoul(argv[1], NULL, 16);
		dig_pwr = command_strtoul(argv[2], NULL, 16);
		pwr = (ana_pwr << 4 | dig_pwr);
		if (ana_pwr > 0xf || dig_pwr > 0xf)
			return -EINVAL;
	} else {
		ana_pwr = command_strtoul(argv[1], NULL, 10);
		pwr = ana_pwr;
		if (ana_pwr > 0x1e)
			return -EINVAL;
	}
	AICWFDBG(LOGINFO, "pwr =%x\r\n", pwr);
	rwnx_send_rftest_req(rwnx_hw, SET_POWER, sizeof(pwr), (u8_l *)&pwr, NULL);
	return 0;
}

static int aic_priv_cmd_set_xtal_cap (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l xtal_cap;

	if (argc < 2)
		return -EINVAL;

	xtal_cap = command_strtoul(argv[1], NULL, 10);
	AICWFDBG(LOGINFO, "xtal_cap =%x\r\n", xtal_cap);
	rwnx_send_rftest_req(rwnx_hw, SET_XTAL_CAP, sizeof(xtal_cap), (u8_l *)&xtal_cap, &cfm);
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_set_xtal_cap_fine (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l xtal_cap_fine;

	if (argc < 2)
		return -EINVAL;

	xtal_cap_fine = command_strtoul(argv[1], NULL, 10);
	AICWFDBG(LOGINFO, "xtal_cap =%x\r\n", xtal_cap_fine);
	rwnx_send_rftest_req(rwnx_hw, SET_XTAL_CAP, sizeof(xtal_cap_fine), (u8_l *)&xtal_cap_fine, &cfm);
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_get_efuse_block (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_rf_getefuse_t getefuse_param;

	if (argc < 2)
		return -EINVAL;

	getefuse_param.block = command_strtoul(argv[1], NULL, 10);
	rwnx_send_rftest_req(rwnx_hw, GET_EFUSE_BLOCK, sizeof(cmd_rf_getefuse_t), (u8_l *)&getefuse_param, &cfm);
	AICWFDBG(LOGINFO, "get val=%x\r\n", cfm.rftest_result[0]);
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_set_freq_cal (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_rf_setfreq_t cmd_setfreq;

	if (argc < 2)
		return -EINVAL;

	cmd_setfreq.val = command_strtoul(argv[1], NULL, 16);
	AICWFDBG(LOGINFO, "param:%x\r\n", cmd_setfreq.val);
	rwnx_send_rftest_req(rwnx_hw, SET_FREQ_CAL, sizeof(cmd_rf_setfreq_t), (u8_l *)&cmd_setfreq, &cfm);
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_set_freq_cal_fine (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_rf_setfreq_t cmd_setfreq;

	if (argc < 2)
		return -EINVAL;

	cmd_setfreq.val = command_strtoul(argv[1], NULL, 16);
	AICWFDBG(LOGINFO, "param:%x\r\n", cmd_setfreq.val);
	rwnx_send_rftest_req(rwnx_hw, SET_FREQ_CAL_FINE, sizeof(cmd_rf_setfreq_t), (u8_l *)&cmd_setfreq, &cfm);
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_get_freq_cal (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l val;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	rwnx_send_rftest_req(rwnx_hw, GET_FREQ_CAL, 0, NULL, &cfm);
	memcpy(command, &cfm.rftest_result[0], 4);
	val = cfm.rftest_result[0];
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) {
		AICWFDBG(LOGINFO, "cap=0x%x (remain:%x), cap_fine=%x (remain:%x)\n",
				val & 0xff, (val >> 8) & 0xff, (val >> 16) & 0xff, (val >> 24) & 0xff);
	} else {
		AICWFDBG(LOGINFO, "cap=0x%x, cap_fine=0x%x\n", val & 0xff, (val >> 8) & 0xff);
	}
	return 4;
}

static int aic_priv_cmd_set_mac_addr (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l mac_addr[6];

	if (argc < 7)
		return -EINVAL;

	mac_addr[5] = command_strtoul(argv[1], NULL, 16);
	mac_addr[4] = command_strtoul(argv[2], NULL, 16);
	mac_addr[3] = command_strtoul(argv[3], NULL, 16);
	mac_addr[2] = command_strtoul(argv[4], NULL, 16);
	mac_addr[1] = command_strtoul(argv[5], NULL, 16);
	mac_addr[0] = command_strtoul(argv[6], NULL, 16);
	AICWFDBG(LOGINFO, "set macaddr:%x,%x,%x,%x,%x,%x\n", mac_addr[5], mac_addr[4], mac_addr[3], mac_addr[2], mac_addr[1], mac_addr[0]);
	rwnx_send_rftest_req(rwnx_hw, SET_MAC_ADDR, sizeof(mac_addr), (u8_l *)&mac_addr, NULL);
	return 0;
}

static int aic_priv_cmd_get_mac_addr (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u32_l addr0, addr1;
  int rem_cnt = 0;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	rwnx_send_rftest_req(rwnx_hw, GET_MAC_ADDR, 0, NULL, &cfm);
	memcpy(command, &cfm.rftest_result[0], 8);
	addr0 = cfm.rftest_result[0];
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) {
		rem_cnt = (cfm.rftest_result[1] >> 16) & 0x00FF;
		addr1 = cfm.rftest_result[1] & 0x0000FFFF;
		AICWFDBG(LOGINFO, "0x%x,0x%x (remain:%x)\n", addr0, addr1, rem_cnt);
	} else {
		addr1 = cfm.rftest_result[1];
		AICWFDBG(LOGINFO, "0x%x,0x%x\n", addr0, addr1);
	}
	return 8;
}

static int aic_priv_cmd_set_bt_mac_addr (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l mac_addr[6];

	if (argc < 7)
		return -EINVAL;

	mac_addr[5] = command_strtoul(argv[1], NULL, 16);
	mac_addr[4] = command_strtoul(argv[2], NULL, 16);
	mac_addr[3] = command_strtoul(argv[3], NULL, 16);
	mac_addr[2] = command_strtoul(argv[4], NULL, 16);
	mac_addr[1] = command_strtoul(argv[5], NULL, 16);
	mac_addr[0] = command_strtoul(argv[6], NULL, 16);
	AICWFDBG(LOGINFO, "set bt macaddr:%x,%x,%x,%x,%x,%x\n", mac_addr[5], mac_addr[4], mac_addr[3], mac_addr[2], mac_addr[1], mac_addr[0]);
	rwnx_send_rftest_req(rwnx_hw, SET_BT_MAC_ADDR, sizeof(mac_addr), (u8_l *)&mac_addr, NULL);
	return 0;
}

static int aic_priv_cmd_get_bt_mac_addr (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u32_l addr0, addr1;
  int rem_cnt = 0;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	rwnx_send_rftest_req(rwnx_hw, GET_BT_MAC_ADDR, 0, NULL, &cfm);
	memcpy(command, &cfm.rftest_result[0], 8);
	addr0 = cfm.rftest_result[0];
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) {
		rem_cnt = (cfm.rftest_result[1] >> 16) & 0x00FF;
		addr1 = cfm.rftest_result[1] & 0x0000FFFF;
		AICWFDBG(LOGINFO, "0x%x,0x%x (remain:%x)\n", addr0, addr1, rem_cnt);
	} else {
		addr1 = cfm.rftest_result[1];
		AICWFDBG(LOGINFO, "0x%x,0x%x\n", addr0, addr1);
	}
	return 8;
}

static int aic_priv_cmd_set_vendor_info (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l vendor_info;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	vendor_info = command_strtoul(argv[1], NULL, 16);
	AICWFDBG(LOGINFO, "set vendor info:%x\n", vendor_info);
	rwnx_send_rftest_req(rwnx_hw, SET_VENDOR_INFO, 1, &vendor_info, &cfm);
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) {
		memcpy(command, &cfm.rftest_result[0], 2);
		return 2;
	} else {
		memcpy(command, &cfm.rftest_result[0], 1);
		return 1;
	}
	AICWFDBG(LOGINFO, "0x%x\n", cfm.rftest_result[0]);
}

static int aic_priv_cmd_get_vendor_info (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	rwnx_send_rftest_req(rwnx_hw, GET_VENDOR_INFO, 0, NULL, &cfm);
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) {
		memcpy(command, &cfm.rftest_result[0], 2);
		return 2;
	} else {
		memcpy(command, &cfm.rftest_result[0], 1);
		return 1;
	}
	AICWFDBG(LOGINFO, "0x%x\n", cfm.rftest_result[0]);
}

static int aic_priv_cmd_rdwr_pwrmm (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	if (argc <= 1) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWRMM, 0, NULL, &cfm);
	} else { // write
		u8_l pwrmm = (u8_l)command_strtoul(argv[1], NULL, 16);
		pwrmm = (pwrmm) ? 1 : 0;
		AICWFDBG(LOGINFO, "set pwrmm = %x\r\n", pwrmm);
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWRMM, sizeof(pwrmm), (u8_l *)&pwrmm, &cfm);
	}
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_rdwr_pwridx (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	if (dev->chipid != PRODUCT_ID_AIC8801){
		AICWFDBG(LOGERROR, "unsupported cmd\n");
		return -EINVAL;
	}
	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
		}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWRIDX, 0, NULL, &cfm);
	} else if (func <= 2) { // write 2.4g/5g pwr idx
		if (argc > 3) {
			u8_l type = (u8_l)command_strtoul(argv[2], NULL, 16);
			u8_l pwridx = (u8_l)command_strtoul(argv[3], NULL, 10);
			u8_l buf[3] = {func, type, pwridx};
			AICWFDBG(LOGINFO, "set pwridx:[%x][%x]=%x\r\n", func, type, pwridx);
			rwnx_send_rftest_req(rwnx_hw, RDWR_PWRIDX, sizeof(buf), buf, &cfm);
		} else {
			return -EINVAL;
		}
	} else {
		return -EINVAL;
	}
	memcpy(command, &cfm.rftest_result[0], 9);
	return 9;
}

static int aic_priv_cmd_rdwr_pwrlvl (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	if (dev->chipid == PRODUCT_ID_AIC8801){
		AICWFDBG(LOGERROR, "unsupported cmd\n");
		return -EINVAL;
	}
	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWRLVL, 0, NULL, &cfm);
	} else if (func <= 2) { // write 2.4g/5g pwr lvl
		if (argc > 4) {
			u8_l grp = (u8_l)command_strtoul(argv[2], NULL, 16);
			u8_l idx, size;
			u8_l buf[14] = {func, grp,};
			if (argc > 12) { // set all grp
			AICWFDBG(LOGINFO, "set pwrlvl %s:\n"
				   "  [%x] =", (func == 1) ? "2.4g" : "5g", grp);
				if (grp == 1) { // TXPWR_LVL_GRP_11N_11AC
					size = 10;
				} else {
					size = 12;
				}
				for (idx = 0; idx < size; idx++) {
					s8_l pwrlvl = (s8_l)command_strtoul(argv[3 + idx], NULL, 10);
					buf[2 + idx] = (u8_l)pwrlvl;
					if (idx && !(idx & 0x3)) {
						AICWFDBG(LOGINFO, " ");
					}
					AICWFDBG(LOGINFO, " %2d", pwrlvl);
				}
				AICWFDBG(LOGINFO, "\n");
				size += 2;
			} else { // set grp[idx]
				u8_l idx = (u8_l)command_strtoul(argv[3], NULL, 10);
				s8_l pwrlvl = (s8_l)command_strtoul(argv[4], NULL, 10);
				buf[2] = idx;
				buf[3] = (u8_l)pwrlvl;
				size = 4;
				AICWFDBG(LOGINFO, "set pwrlvl %s:\n"
					   "  [%x][%d] = %d\n", (func == 1) ? "2.4g" : "5g", grp, idx, pwrlvl);
			}
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWRLVL, size, buf, &cfm);
		} else {
		AICWFDBG(LOGERROR, "wrong args\n");
		return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	if(dev->chipid == PRODUCT_ID_AIC8800D80){
		memcpy(command, &cfm.rftest_result[0], 6 * 12);
		return (6 * 12);
	} else {
		memcpy(command, &cfm.rftest_result[0], 3 * 12);
		return (3 * 12);
	}
}

static int aic_priv_cmd_rdwr_pwrofst (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	int res_len = 0;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWROFST, 0, NULL, &cfm);
	} else if (func <= 2) { // write 2.4g/5g pwr ofst
		if ((argc > 4) && (dev->chipid == PRODUCT_ID_AIC8800D80)) {
			u8_l type = (u8_l)command_strtoul(argv[2], NULL, 16);
			u8_l chgrp = (u8_l)command_strtoul(argv[3], NULL, 16);
			s8_l pwrofst = (u8_l)command_strtoul(argv[4], NULL, 10);
			u8_l buf[4] = {func, type, chgrp, (u8_l)pwrofst};
			AICWFDBG(LOGINFO, "set pwrofst_%s:[%x][%x]=%d\r\n", (func == 1) ? "2.4g" : "5g", type, chgrp, pwrofst);
			rwnx_send_rftest_req(rwnx_hw, RDWR_PWROFST, sizeof(buf), buf, &cfm);
		} else if ((argc > 3) && (dev->chipid != PRODUCT_ID_AIC8800D80)) {
			u8_l chgrp = (u8_l)command_strtoul(argv[2], NULL, 16);
			s8_l pwrofst = (u8_l)command_strtoul(argv[3], NULL, 10);
			u8_l buf[3] = {func, chgrp, (u8_l)pwrofst};
			AICWFDBG(LOGINFO, "set pwrofst_%s:[%x]=%d\r\n", (func == 1) ? "2.4g" : "5g", chgrp, pwrofst);
			rwnx_send_rftest_req(rwnx_hw, RDWR_PWROFST, sizeof(buf), buf, &cfm);
		} else {
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) { // 3 = 3 (2.4g)
		res_len = 3;
	} else if (dev->chipid == PRODUCT_ID_AIC8800D80) { // 3 * 2 (2.4g) + 3 * 6 (5g)
		res_len = 3 * 3 + 3 * 6;
	} else {
		res_len = 3 + 4;
	}
	memcpy(command, &cfm.rftest_result[0], res_len);
	return res_len;
}

static int aic_priv_cmd_rdwr_pwrofstfine (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_PWROFSTFINE, 0, NULL, &cfm);
	} else if (func <= 2) { // write 2.4g/5g pwr ofst
		if (argc > 3) {
			u8_l chgrp = (u8_l)command_strtoul(argv[2], NULL, 16);
			s8_l pwrofst = (u8_l)command_strtoul(argv[3], NULL, 10);
			u8_l buf[3] = {func, chgrp, (u8_l)pwrofst};
			AICWFDBG(LOGINFO, "set pwrofstfine:[%x][%x]=%d\r\n", func, chgrp, pwrofst);
			rwnx_send_rftest_req(rwnx_hw, RDWR_PWROFSTFINE, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;

		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;

	}
	memcpy(command, &cfm.rftest_result[0], 7);
	return 7;
}

static int aic_priv_cmd_rdwr_drvibit (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_DRVIBIT, 0, NULL, &cfm);
	} else if (func == 1) { // write 2.4g pa drv_ibit
		if (argc > 2) {
			u8_l ibit = (u8_l)command_strtoul(argv[2], NULL, 16);
			u8_l buf[2] = {func, ibit};
			AICWFDBG(LOGINFO, "set drvibit:[%x]=%x\r\n", func, ibit);
			rwnx_send_rftest_req(rwnx_hw, RDWR_DRVIBIT, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	memcpy(command, &cfm.rftest_result[0], 16);
	return 16;
}

static int aic_priv_cmd_rdwr_efuse_pwrofst (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	int res_len = 0;
#ifdef AICWF_SDIO_SUPPORT
	struct aic_sdio_dev *dev = g_rwnx_plat->sdiodev;
#endif
#ifdef AICWF_USB_SUPPORT
	struct aic_usb_dev *dev = g_rwnx_plat->usbdev;
#endif

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_PWROFST, 0, NULL, &cfm);
	} else if (func <= 2) { // write 2.4g/5g pwr ofst
		if ((argc > 4) && (dev->chipid == PRODUCT_ID_AIC8800D80)) {
			u8_l type = (u8_l)command_strtoul(argv[2], NULL, 16);
			u8_l chgrp = (u8_l)command_strtoul(argv[3], NULL, 16);
			s8_l pwrofst = (u8_l)command_strtoul(argv[4], NULL, 10);
			u8_l buf[4] = {func, type, chgrp, (u8_l)pwrofst};
			AICWFDBG(LOGINFO, "set efuse pwrofst_%s:[%x][%x]=%d\r\n", (func == 1) ? "2.4g" : "5g", type, chgrp, pwrofst);
			rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_PWROFST, sizeof(buf), buf, &cfm);
		} else if ((argc > 3) && (dev->chipid != PRODUCT_ID_AIC8800D80)) {
			u8_l chgrp = (u8_l)command_strtoul(argv[2], NULL, 16);
			s8_l pwrofst = (u8_l)command_strtoul(argv[3], NULL, 10);
			u8_l buf[3] = {func, chgrp, (u8_l)pwrofst};
			AICWFDBG(LOGINFO, "set efuse pwrofst_%s:[%x]=%d\r\n", (func == 1) ? "2.4g" : "5g", chgrp, pwrofst);
			rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_PWROFST, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	if ((dev->chipid == PRODUCT_ID_AIC8800DC) || (dev->chipid == PRODUCT_ID_AIC8800DW)) { // 6 = 3 (2.4g) * 2
		res_len = 3 * 2;
	} else if (dev->chipid == PRODUCT_ID_AIC8800D80) { // 3 * 2 (2.4g) + 3 * 6 (5g)
		res_len = (3 * 3 + 3 * 6) * 2;
	} else { // 7 = 3(2.4g) + 4(5g)
		res_len = 3 + 4;
	}
	memcpy(command, &cfm.rftest_result[0], res_len);
	return res_len;
}

static int aic_priv_cmd_rdwr_efuse_pwrofstfine (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_PWROFSTFINE, 0, NULL, &cfm);
	} else if (func <= 2) { // write 2.4g/5g pwr ofst
		if (argc > 3) {
			u8_l chgrp = (u8_l)command_strtoul(argv[2], NULL, 16);
			s8_l pwrofst = (u8_l)command_strtoul(argv[3], NULL, 10);
			u8_l buf[3] = {func, chgrp, (u8_l)pwrofst};
			AICWFDBG(LOGINFO, "set pwrofstfine:[%x][%x]=%d\r\n", func, chgrp, pwrofst);
			rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_PWROFSTFINE, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	memcpy(command, &cfm.rftest_result[0], 7);
	return 7;
}

static int aic_priv_cmd_rdwr_efuse_drvibit (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_DRVIBIT, 0, NULL, &cfm);
	} else if (func == 1) { // write 2.4g pa drv_ibit
		if (argc > 2) {
		u8_l ibit = (u8_l)command_strtoul(argv[2], NULL, 16);
		u8_l buf[2] = {func, ibit};
		AICWFDBG(LOGINFO, "set efuse drvibit:[%x]=%x\r\n", func, ibit);
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_DRVIBIT, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_rdwr_efuse_usrdata (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_ef_usrdata_t cmd_ef_usrdata;

	if (argc <= 1) { // read all
		cmd_ef_usrdata.func = 0;
		cmd_ef_usrdata.cnt = 3;
	} else if (argc >= 2) { // read/write
		cmd_ef_usrdata.func = (u8_l)command_strtoul(argv[1], NULL, 10);
		cmd_ef_usrdata.cnt = (u8_l)command_strtoul(argv[2], NULL, 10);
		if (cmd_ef_usrdata.func == 1) {
			int idx;
			for (idx = 0; idx < cmd_ef_usrdata.cnt; idx++) {
				cmd_ef_usrdata.usrdata[idx] = (u32_l)command_strtoul(argv[3 + idx], NULL, 16);
			}
		}
	} else {
		AICWFDBG(LOGERROR, "wrong argc: %x\n", argc);
		return -EINVAL;
	}
	rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_USRDATA, sizeof(cmd_ef_usrdata), (u8_l *)&cmd_ef_usrdata, &cfm);
	memcpy(command, &cfm.rftest_result[0], 12);
	return 12;
}

static int aic_priv_cmd_rdwr_efuse_sdiocfg (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;

	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_SDIOCFG, 0, NULL, &cfm);
	} else if (func == 1) { // write sdiocfg
		if (argc > 2) {
			u8_l ibit = (u8_l)command_strtoul(argv[2], NULL, 16);
			u8_l buf[2] = {func, ibit};
			AICWFDBG(LOGINFO, "set efuse sdiocfg:[%x]=%x\r\n", func, ibit);
			rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_SDIOCFG, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_rdwr_efuse_usbvidpid (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	AICWFDBG(LOGINFO, "read/write usb vid/pid into efuse\n");
	if (argc > 1) {
		func = (u8_l)command_strtoul(argv[1], NULL, 16);
	}
	if (func == 0) { // read cur
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_USBVIDPID, 0, NULL, &cfm);
	} else if (func == 1) { // write USB vid+pid
		if (argc > 2) {
			u32_l usb_id = (u32_l)command_strtoul(argv[2], NULL, 16);
			u8_l buf[5] = {func, (u8_l)usb_id, (u8_l)(usb_id >> 8), (u8_l)(usb_id >> 16), (u8_l)(usb_id >> 24)};
			AICWFDBG(LOGINFO, "set efuse usb vid/pid:[%x]=%x\r\n", func, usb_id);
			rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_USBVIDPID, sizeof(buf), buf, &cfm);
		} else {
			AICWFDBG(LOGERROR, "wrong args\n");
			return -EINVAL;
		}
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	memcpy(command, &cfm.rftest_result[0], 4);
	return 4;
}

static int aic_priv_cmd_rdwr_efuse_he_off (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;

	func = command_strtoul(argv[1], NULL, 10);
	AICWFDBG(LOGINFO, "set he off: %d\n", func);
	if(func == 1) {
		rwnx_send_rftest_req(rwnx_hw, RDWR_EFUSE_HE_OFF, sizeof(func), (u8_l *)&func, &cfm);
		AICWFDBG(LOGINFO, "he_off cfm: %d\n", cfm.rftest_result[0]);
		memcpy(command, &cfm.rftest_result[0], 4);
		return 4;
	}
	return 0;
}

static int aic_priv_cmd_set_cal_xtal (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	rwnx_send_rftest_req(rwnx_hw, SET_CAL_XTAL, 0, NULL, NULL);
	return 0;
}

static int aic_priv_cmd_get_cal_xtal_res (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	rwnx_send_rftest_req(rwnx_hw, GET_CAL_XTAL_RES, 0, NULL, &cfm);
	memcpy(command, &cfm.rftest_result[0], 4);
	AICWFDBG(LOGINFO, "cap=0x%x, cap_fine=0x%x\n", cfm.rftest_result[0] & 0x0000ffff, (cfm.rftest_result[0] >> 16) & 0x0000ffff);
	return 4;
}

static int aic_priv_cmd_set_cob_cal (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	cmd_rf_setcobcal_t setcob_cal;

	if (argc < 3)
		return -EINVAL;
	setcob_cal.dutid = command_strtoul(argv[1], NULL, 10);
	setcob_cal.chip_num = command_strtoul(argv[2], NULL, 10);
	setcob_cal.dis_xtal = command_strtoul(argv[3], NULL, 10);
	rwnx_send_rftest_req(rwnx_hw, SET_COB_CAL, sizeof(cmd_rf_setcobcal_t), (u8_l *)&setcob_cal, NULL);
	return 0;
}

static int aic_priv_cmd_get_cob_cal_res (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l state;
	cob_result_ptr_t *cob_result_ptr;

	rwnx_send_rftest_req(rwnx_hw, GET_COB_CAL_RES, 0, NULL, &cfm);
	state = (cfm.rftest_result[0] >> 16) & 0x000000ff;
	if (!state){
		AICWFDBG(LOGINFO, "cap= 0x%x, cap_fine= 0x%x, freq_ofst= %d Hz\n",
		cfm.rftest_result[0] & 0x000000ff, (cfm.rftest_result[0] >> 8) & 0x000000ff, cfm.rftest_result[1]);
		cob_result_ptr = (cob_result_ptr_t *) & (cfm.rftest_result[2]);
		AICWFDBG(LOGINFO, "golden_rcv_dut= %d , tx_rssi= %d dBm, snr = %d dB\ndut_rcv_godlden= %d , rx_rssi= %d dBm",
		cob_result_ptr->golden_rcv_dut_num, cob_result_ptr->rssi_static, cob_result_ptr->snr_static,
		cob_result_ptr->dut_rcv_golden_num, cob_result_ptr->dut_rssi_static);
		memcpy(command, &cfm.rftest_result, 16);
		return 16;
	} else {
		AICWFDBG(LOGERROR, "cob not idle\n");
		return -EINVAL;
	}
}

static int aic_priv_cmd_do_cob_test (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l state;
	cmd_rf_setcobcal_t setcob_cal;
	cob_result_ptr_t *cob_result_ptr;

	setcob_cal.dutid = 1;
	setcob_cal.chip_num = 1;
	setcob_cal.dis_xtal = 0;
	if (argc > 1)
		setcob_cal.dis_xtal = command_strtoul(argv[1], NULL, 10);
	rwnx_send_rftest_req(rwnx_hw, SET_COB_CAL, sizeof(cmd_rf_setcobcal_t), (u8_l *)&setcob_cal, NULL);
	msleep(2000);
	rwnx_send_rftest_req(rwnx_hw, GET_COB_CAL_RES, 0, NULL, &cfm);
	state = (cfm.rftest_result[0] >> 16) & 0x000000ff;
	if (!state){
		AICWFDBG(LOGINFO, "cap= 0x%x, cap_fine= 0x%x, freq_ofst= %d Hz\n",
		cfm.rftest_result[0] & 0x000000ff, (cfm.rftest_result[0] >> 8) & 0x000000ff, cfm.rftest_result[1]);
		cob_result_ptr = (cob_result_ptr_t *) & (cfm.rftest_result[2]);
		AICWFDBG(LOGINFO, "golden_rcv_dut= %d , tx_rssi= %d dBm, snr = %d dB\ndut_rcv_godlden= %d , rx_rssi= %d dBm",
		cob_result_ptr->golden_rcv_dut_num, cob_result_ptr->rssi_static, cob_result_ptr->snr_static,
		cob_result_ptr->dut_rcv_golden_num, cob_result_ptr->dut_rssi_static);
		memcpy(command, &cfm.rftest_result, 16);
		return 16;
	} else {
		AICWFDBG(LOGERROR, "cob not idle\n");
		return -EINVAL;
	}
}

static int aic_priv_cmd_set_papr (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	if (argc > 1) {
		func = command_strtoul(argv[1], NULL, 10);
		AICWFDBG(LOGINFO, "papr %d\r\n", func);
		rwnx_send_rftest_req(rwnx_hw, SET_PAPR, sizeof(func), (u8_l *)&func, NULL);
	} else {
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_set_notch (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	if (argc > 1) {
		func = command_strtoul(argv[1], NULL, 10);
		AICWFDBG(LOGINFO, "notch %d\r\n", func);
		rwnx_send_rftest_req(rwnx_hw, SET_NOTCH, sizeof(func), (u8_l *)&func, NULL);
	} else {
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_set_srrc (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	if (argc > 1) {
		func = command_strtoul(argv[1], NULL, 10);
		AICWFDBG(LOGINFO, "srrc %d\r\n", func);
		rwnx_send_rftest_req(rwnx_hw, SET_SRRC, sizeof(func), (u8_l *)&func, NULL);
	} else {
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_set_fss (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0;
	if (argc > 1) {
		func = command_strtoul(argv[1], NULL, 10);
		AICWFDBG(LOGINFO, "fss %d\r\n", func);
		rwnx_send_rftest_req(rwnx_hw, SET_FSS, sizeof(func), (u8_l *)&func, NULL);
	} else {
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_set_usb_off (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	rwnx_send_rftest_req(rwnx_hw, SET_USB_OFF, 0, NULL, NULL);
	return 0;
}

static int aic_priv_cmd_set_pll_test (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	u8_l func = 0, tx_pwr = 0xc;
	s8_l freq = 0;

	if (argc > 1) {
		func = command_strtoul(argv[1], NULL, 16);
	}
	if (argc > 3) {
		freq = (s8_l)command_strtoul(argv[2], NULL, 10);
		tx_pwr = command_strtoul(argv[3], NULL, 16);
	}
	if (func <= 1) {
		u8_l buf[3] = {func, (u8_l)freq, tx_pwr};
		AICWFDBG(LOGINFO, "set pll_test %d: freq=%d, tx_pwr=0x%x\n", func, freq, tx_pwr);
		rwnx_send_rftest_req(rwnx_hw, SET_PLL_TEST, sizeof(buf), buf, &cfm);
	} else {
		AICWFDBG(LOGERROR, "wrong func: %x\n", func);
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_get_txpwr(struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	s8_l power=0;
	power = get_txpwr_max(power);
	memcpy(command, &power, 1);
	return 1;
}

static int aic_priv_cmd_set_txpwr_loss(struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	s8_l func;
	if (argc > 1) {
		func = (s8_l)command_strtoul(argv[1], NULL, 10);
		printk("set txpwr loss: %d\n", func);
		if (g_rwnx_plat->sdiodev->chipid == PRODUCT_ID_AIC8800D80){
			set_txpwr_loss_ofst(func);
			rwnx_send_txpwr_lvl_v3_req(g_rwnx_plat->sdiodev->rwnx_hw);
		}else{
			AICWFDBG(LOGINFO,"error:don't support ,now only support D40 D80");
		}
	} else {
		printk("wrong args\n");
		return -EINVAL;
	}
	return 0;
}

static int aic_priv_cmd_help (struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command)
{
	print_help(argc > 0 ? argv[0] : NULL);
	return 0;
}

struct aic_priv_cmd {
	const char *cmd;
	int (*handler)(struct rwnx_hw *rwnx_hw, int argc, char *argv[], char *command);
	const char *usage;
};

static const struct aic_priv_cmd aic_priv_commands[] = {
	{ "set_tx", aic_priv_cmd_set_tx,
	  "<chan> <bw> <mode> <rate> <length> <interval>" },
	{ "set_txstop", aic_priv_cmd_set_txstop,
	  "= stop tx " },
	{ "set_rx", aic_priv_cmd_set_rx,
	  "<chan_num> <bw> " },
	{ "get_rx_result", aic_priv_cmd_get_rx_result,
	  "= display rx fcsok/total pkt num" },
	{ "set_rxstop", aic_priv_cmd_set_rxstop,
	  "= stop rx " },
	{ "set_txtone", aic_priv_cmd_set_tx_tone,
	  "<val> val = 0/off" },
	{ "set_rx_meter", aic_priv_cmd_set_rx_meter,
	  "= set rx meter " },
	{ "set_power", aic_priv_cmd_set_set_power,
	  "<dec val> " },
	{ "set_xtal_cap", aic_priv_cmd_set_xtal_cap,
	  "<dec val> [0 ~ 31]" },
	{ "set_xtal_cap_fine", aic_priv_cmd_set_xtal_cap_fine,
	  "<dec val> [0 ~ 63]" },
	{ "get_efuse_block", aic_priv_cmd_get_efuse_block,
	  "<val>" },
	{ "set_freq_cal", aic_priv_cmd_set_freq_cal,
	  "<hex val>" },
	{ "set_freq_cal_fine", aic_priv_cmd_set_freq_cal_fine,
	  "<hex val>" },
	{ "get_freq_cal", aic_priv_cmd_get_freq_cal,
	  "= display cap & cap fine" },
	{ "set_mac_addr", aic_priv_cmd_set_mac_addr,
	  "= write WiFi MAC into efuse or flash is limited to a maximum of two times" },
	{ "get_mac_addr", aic_priv_cmd_get_mac_addr,
	  "= display WiFi MAC stored in efuse or flash" },
	{ "set_bt_mac_addr", aic_priv_cmd_set_bt_mac_addr,
	  "= write BT MAC into efuse or flash is limited to a maximum of two times" },
	{ "get_bt_mac_addr", aic_priv_cmd_get_bt_mac_addr,
	  "= display BT MAC stored in efuse or flash" },
	{ "set_vendor_info", aic_priv_cmd_set_vendor_info,
	  "= write vendor info into efuse or flash is allowed only once" },
	{ "get_vendor_info", aic_priv_cmd_get_vendor_info,
	  "= display vendor info stored in efuse or flash" },
	{ "rdwr_pwrmm", aic_priv_cmd_rdwr_pwrmm,
	  "<val> = 0/rdwr_pwrlvl, 1/set_power = read/write txpwr manul mode" },
	{ "rdwr_pwridx", aic_priv_cmd_rdwr_pwridx,
	  "<band> <mod> <idx>" },
	{ "rdwr_pwrlvl", aic_priv_cmd_rdwr_pwrlvl,
	  "<band> <mod> <idx>" },
	{ "rdwr_pwrofst", aic_priv_cmd_rdwr_pwrofst,
	  "<band> <rate> <ch> <ofst>" },
	{ "rdwr_pwrofstfine", aic_priv_cmd_rdwr_pwrofstfine,
	  "<band> <rate> <ch> <ofstfine>" },
	{ "rdwr_drvibit", aic_priv_cmd_rdwr_drvibit,
	  "<func> <val> read/write 8800D pa drvibit" },
	{ "set_cal_xtal", aic_priv_cmd_set_cal_xtal,
	  "= set cal xtal" },
	{ "get_cal_xtal_res", aic_priv_cmd_get_cal_xtal_res,
	  "= get cal xtal result cap & cap_fine" },
	{ "set_cob_cal", aic_priv_cmd_set_cob_cal,
	  "<dutid> <chip_num> <disxtal> = dut cob test" },
	{ "get_cob_cal_res", aic_priv_cmd_get_cob_cal_res,
	  "= get cob cal result" },
	{ "do_cob_test", aic_priv_cmd_do_cob_test,
	  "<func> = 0/xtal, 1/dis_xtal, 2/only_xtal" },
	{ "rdwr_efuse_pwrofst", aic_priv_cmd_rdwr_efuse_pwrofst,
	  "<band> <rate> <ch> <ofst> limited to a maximum of two times" },
	{ "rdwr_efuse_pwrofstfine", aic_priv_cmd_rdwr_efuse_pwrofstfine,
	  "<band> <rate> <ch> <ofstfine> limited to a maximum of two times" },
	{ "rdwr_efuse_drvibit", aic_priv_cmd_rdwr_efuse_drvibit,
	  "<func> <val> = read/write 8800D efuse pa drvibitis allowed only once" },
	{ "rdwr_efuse_usrdata", aic_priv_cmd_rdwr_efuse_usrdata,
	  "<func> <val> = read/write efuse usrdata" },
	{ "rdwr_efuse_sdiocfg", aic_priv_cmd_rdwr_efuse_sdiocfg,
	  "<func> <val> = read/write sdiocfg_bit into efuse" },
	{ "rdwr_efuse_usbvidpid", aic_priv_cmd_rdwr_efuse_usbvidpid,
	  "<func> <val> = read/write usb vid/pid into efuse" },
	{ "rdwr_efuse_he_off", aic_priv_cmd_rdwr_efuse_he_off,
	  "<func> = read/write he_off into efuse" },
	{ "set_papr", aic_priv_cmd_set_papr,
	  "<val> = configure papr filter to optimize sideband suppression" },
	{ "set_notch", aic_priv_cmd_set_notch,
	  "<val> = configure filter to optimize sideband suppression" },
	{ "set_srrc", aic_priv_cmd_set_srrc,
	  "<func> = disable/enable sideband suppression for SRRC" },
	{ "set_fss", aic_priv_cmd_set_fss,
	  "<func> = disable/enable treatment of spurious emissions and burrs" },
	{ "set_usb_off", aic_priv_cmd_set_usb_off,
	  "= off usb configure before usb disconnect" },
	{ "set_pll_test", aic_priv_cmd_set_pll_test,
	  "<func> <freq> <tx_pwr> = use pll test to measure saturation power" },
	{ "get_txpwr", aic_priv_cmd_get_txpwr,
	  "= get userconfig max txpwr" },
	{ "set_txpwr_loss",aic_priv_cmd_set_txpwr_loss,
	  "<val> = txpwr will change ,val can be negative" },

//Reserve for new aic_priv_cmd.
	{ "help", aic_priv_cmd_help,
	  "= show usage help" },
	{ NULL, NULL, NULL }

};


/*
 * Prints command usage, lines are padded with the specified string.
 */
static void print_help(const char *cmd)
{
	int n;
	printk("commands:\n");
	for (n = 0; aic_priv_commands[n].cmd; n++) {
		if (cmd != NULL)
			printk("%s %s\n", aic_priv_commands[n].cmd, aic_priv_commands[n].usage);
	}
}

int handle_private_cmd(struct net_device *net, char *command, u32 cmd_len)
{
	const struct aic_priv_cmd *cmd, *match = NULL;
	int count;
	int bytes_written = 0;
	char *argv[CMD_MAXARGS + 1];
	int argc;
	struct rwnx_vif *vif = container_of(net->ieee80211_ptr, struct rwnx_vif, wdev);
	struct rwnx_hw *p_rwnx_hw = vif->rwnx_hw;

	RWNX_DBG(RWNX_FN_ENTRY_STR);

	argc = parse_line(command, argv);
	if (argc == 0) {
		return -1;
	}

	count = 0;
	cmd = aic_priv_commands;
	while (cmd->cmd) {
		if (strncasecmp(cmd->cmd, argv[0], strlen(argv[0])) == 0)
		{
			match = cmd;
			if (strcasecmp(cmd->cmd, argv[0]) == 0) {
				/* we have an exact match */
				count = 1;
				break;
			}
			count++;
		}
		cmd++;
	}

	if (count > 1) {
		AICWFDBG(LOGINFO, "Ambiguous command '%s'; possible commands:", argv[0]);
		cmd = aic_priv_commands;
		while (cmd->cmd) {
			if (strncasecmp(cmd->cmd, argv[0],
					   strlen(argv[0])) == 0) {
				AICWFDBG(LOGINFO, " %s", cmd->cmd);
			}
			cmd++;
		}
		AICWFDBG(LOGINFO, "\n");
	} else if (count == 0) {
		AICWFDBG(LOGERROR, "Unknown command '%s'\n", argv[0]);
	} else {
		AICWFDBG(LOGINFO, "match %s", match->cmd);
		bytes_written = match->handler(p_rwnx_hw, argc, &argv[0], command);
	}

	if (bytes_written < 0)
		AICWFDBG(LOGERROR, "wrong param\n");

	return bytes_written;
}

#define RWNX_COUNTRY_CODE_LEN 2
#define CMD_SET_COUNTRY "COUNTRY"
#define CMD_SET_VENDOR_EX_IE "SET_VENDOR_EX_IE"
#define CMD_SET_AP_WPS_P2P_IE "SET_AP_WPS_P2P_IE"
#define CMD_SETSUSPENDMODE "SETSUSPENDMODE"


struct ieee80211_regdomain *getRegdomainFromRwnxDB(struct wiphy *wiphy, char *alpha2);
struct ieee80211_regdomain *getRegdomainFromRwnxDBIndex(struct wiphy *wiphy, int index);
extern int reg_regdb_size;

#ifdef CONFIG_SET_VENDOR_EXTENSION_IE
extern u8_l vendor_extension_data[256];
extern int vendor_extension_len;

void set_vendor_extension_ie(char *command){

	char databyte[3]={0x00, 0x00, 0x00};
	int skip = strlen(CMD_SET_VENDOR_EX_IE) + 1;
	int command_index = skip;
	int data_index = 0;

	memset(vendor_extension_data, 0, 256);
	vendor_extension_len = 0;
	memcpy(databyte, command + command_index, 2);
	vendor_extension_len = command_strtoul(databyte, NULL, 16);
	printk("%s len:%d \r\n", __func__, vendor_extension_len);

	//parser command and save data in vendor_extension_data
	for(data_index = 0;data_index < vendor_extension_len; data_index++){
		command_index = command_index + 3;
		memcpy(databyte, command + command_index, 2);
		vendor_extension_data[data_index] = command_strtoul(databyte, NULL, 16);
	}

}
#endif//CONFIG_SET_VENDOR_EXTENSION_IE


int android_priv_cmd(struct net_device *net, struct ifreq *ifr, int cmd)
{
#define PRIVATE_COMMAND_MAX_LEN 8192
#define PRIVATE_COMMAND_DEF_LEN 4096

	struct rwnx_vif *vif = netdev_priv(net);
	int ret = 0;
	char *command = NULL;
	int bytes_written = 0;
	android_wifi_priv_cmd priv_cmd;
	int buf_size = 0;
	int skip = 0;
	char *country = NULL;
	int setsusp_mode;
	struct ieee80211_regdomain *regdomain;

	RWNX_DBG(RWNX_FN_ENTRY_STR);

	///todo: add our lock
	//net_os_wake_lock(net);


/*	if (!capable(CAP_NET_ADMIN)) {
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

	if (!command) {
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
	AICWFDBG(LOGINFO, "%s: Android private cmd \"%s\" on %s\n", __FUNCTION__, command, ifr->ifr_name);
	AICWFDBG(LOGINFO, "cmd = %d\n", cmd);
	AICWFDBG(LOGINFO, "buf_size=%d\n", buf_size);

#if 1//Handle Android command
		if(!strncasecmp(command, CMD_SET_COUNTRY, strlen(CMD_SET_COUNTRY))) {
			skip = strlen(CMD_SET_COUNTRY) + 1;
			country = command + skip;
			if (!country || strlen(country) < RWNX_COUNTRY_CODE_LEN) {
				printk("%s: invalid country code\n", __func__);
				ret = -EINVAL;
				goto exit;
			}
#if 0
			for(index = 0; index < reg_regdb_size; index++){
				regdomain = getRegdomainFromRwnxDBIndex(vif->rwnx_hw->wiphy, index);
				if((ret = regulatory_set_wiphy_regd(vif->rwnx_hw->wiphy, regdomain))){
					printk("regulatory_set_wiphy_regd fail \r\n");
				}else{
					printk("regulatory_set_wiphy_regd ok \r\n");
				}
			}
#endif
			AICWFDBG(LOGINFO, "%s country code:%c%c\n", __func__, toupper(country[0]), toupper(country[1]));
			regdomain = getRegdomainFromRwnxDB(vif->rwnx_hw->wiphy, country);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 0, 0)
			if((ret = regulatory_set_wiphy_regd(vif->rwnx_hw->wiphy, regdomain))){
				printk("regulatory_set_wiphy_regd fail \r\n");
			}
#else
			wiphy_apply_custom_regulatory(vif->rwnx_hw->wiphy, regdomain);
#endif
		}
#ifdef CONFIG_SET_VENDOR_EXTENSION_IE
		else if(!strncasecmp(command, CMD_SET_VENDOR_EX_IE, strlen(CMD_SET_VENDOR_EX_IE))){
			set_vendor_extension_ie(command);
		}
#endif//CONFIG_SET_VENDOR_EXTENSION_IE
		else if(!strncasecmp(command, CMD_SET_AP_WPS_P2P_IE, strlen(CMD_SET_AP_WPS_P2P_IE))){
			ret = 0;
			goto exit;
		}
		else if(!strncasecmp(command, CMD_SETSUSPENDMODE, strlen(CMD_SETSUSPENDMODE))){
			skip = strlen(CMD_SET_COUNTRY) + 1;
			setsusp_mode = command_strtoul(command + skip, NULL, 10);
#ifdef CONFIG_GPIO_WAKEUP
			if (setsusp_mode == 1) {
#if defined(CONFIG_SDIO_PWRCTRL)
				aicwf_sdio_pwr_stctl(g_rwnx_plat->sdiodev, SDIO_SLEEP_ST);
#endif//CONFIG_SDIO_PWRCTRL
				ret = aicwf_sdio_writeb(g_rwnx_plat->sdiodev, SDIOWIFI_WAKEUP_REG, 2);
				if (ret < 0) {
				sdio_err("reg:%d write failed!\n", SDIOWIFI_WAKEUP_REG);
				}
			}
			AICWFDBG(LOGINFO, "set suspend mode %d\n", setsusp_mode);
#endif//CONFIG_GPIO_WAKEUP
			goto exit;
		}
#endif//Handle Android command

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
	} else {
		/* Propagate the error */
		ret = bytes_written;
	}

exit:
	///todo: add our unlock
	//net_os_wake_unlock(net);
	kfree(command);
	return ret;
}

#ifdef CONFIG_MCU_MESSAGE
#define CMD_GET_VERSION_STR "GET_VERSION"
#define CMD_GET_SSID_STR    "GET_SSID"
#define CMD_SET_SSID_STR    "SET_SSID"
#define CMD_GET_PASS_STR    "GET_PASS"
#define CMD_SET_PASS_STR    "SET_PASS"
#define CMD_GET_VAR_STR     "GET_VAR"
#define CMD_SET_VAR_STR     "SET_VAR"

enum custmsg_cmd_tag
{
	CUST_CMD_GET_VERSION = 0,
	CUST_CMD_GET_SSID,
	CUST_CMD_SET_SSID,
	CUST_CMD_GET_PASS,
	CUST_CMD_SET_PASS,
	CUST_CMD_GET_VAR,
	CUST_CMD_SET_VAR,
	CUST_CMD_MAX
};

int handle_custom_msg(char *command, u32 cmd_len)
{
	int bytes_read = 0, max_bytes_to_read = 0;
	struct rwnx_hw *p_rwnx_hw = NULL;
	u32 cmd, len = 0, flags = 0;
	char *buf = NULL;
	struct dbg_custom_msg_cfm *cust_msg_cfm;
	printk("cmd,%s,%ld\n",command,strlen(command));
	if (strncasecmp(command, CMD_GET_VERSION_STR, strlen(CMD_GET_VERSION_STR)) == 0) {
		cmd = CUST_CMD_GET_VERSION;
		max_bytes_to_read = 32; // max str len for version
	} else if (strncasecmp(command, CMD_GET_SSID_STR, strlen(CMD_GET_SSID_STR)) == 0) {
		cmd = CUST_CMD_GET_SSID;
		max_bytes_to_read = 48; // max str len for ssid
	} else if (strncasecmp(command, CMD_SET_SSID_STR, strlen(CMD_SET_SSID_STR)) == 0) {
		cmd = CUST_CMD_SET_SSID;
		len = cmd_len - (strlen(CMD_SET_SSID_STR) + 1);
		buf = command + (strlen(CMD_SET_SSID_STR) + 1);
		max_bytes_to_read = 0;
	} else if (strncasecmp(command, CMD_GET_PASS_STR, strlen(CMD_GET_PASS_STR)) == 0) {
		cmd = CUST_CMD_GET_PASS;
		max_bytes_to_read = 64; // max str len for PASS
	} else if (strncasecmp(command, CMD_SET_PASS_STR, strlen(CMD_SET_PASS_STR)) == 0) {
		cmd = CUST_CMD_SET_PASS;
		len = cmd_len - (strlen(CMD_SET_PASS_STR) + 1);
		buf = command + (strlen(CMD_SET_PASS_STR) + 1);
		max_bytes_to_read = 0;
	} else if (strncasecmp(command, CMD_GET_VAR_STR, strlen(CMD_GET_VAR_STR)) == 0) {
		cmd = CUST_CMD_GET_VAR;
		max_bytes_to_read = 64; // max str len for VAR
	} else if (strncasecmp(command, CMD_SET_VAR_STR, strlen(CMD_SET_VAR_STR)) == 0) {
		cmd = CUST_CMD_SET_VAR;
		len = cmd_len - (strlen(CMD_SET_VAR_STR) + 1);
		buf = command + (strlen(CMD_SET_VAR_STR) + 1);
		max_bytes_to_read = 0;
	} else {
		printk("invalid cmd: %s\r\n", command);
		return -1;
	}
	if (len < 0) {
		printk("invalid len: %d\r\n", len);
		return -3;
	}
	#ifdef AICWF_SDIO_SUPPORT
	p_rwnx_hw = g_rwnx_plat->sdiodev->rwnx_hw;
	#endif
	#ifdef AICWF_USB_SUPPORT
	p_rwnx_hw = g_rwnx_plat->usbdev->rwnx_hw;
	#endif
	cust_msg_cfm = (struct dbg_custom_msg_cfm *)kmalloc((offsetof(struct dbg_custom_msg_cfm, buf) + max_bytes_to_read), GFP_KERNEL);
	if (cust_msg_cfm == NULL) {
		printk("msg cfm alloc fail\r\n");
		return -2;
	}
	rwnx_send_dbg_custom_msg_req(p_rwnx_hw, cmd, buf, len, flags, cust_msg_cfm);
	bytes_read = cust_msg_cfm->len;
	printk("Custom msg cfm: cmd=%d, len=%d, status=%x\n", cust_msg_cfm->cmd, bytes_read, cust_msg_cfm->status);
	if (bytes_read) {
		memcpy(command, cust_msg_cfm->buf, bytes_read);
		command[bytes_read] = '\0';
	} else {
		command[0] = '\0';
	}
	if (cust_msg_cfm->status) {
		printk("cfm status: %x", cust_msg_cfm->status);
	}
	return bytes_read;
}

int devipc_cust_msg(struct net_device *net, struct ifreq *ifr, int cmd)
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


/*	if (!capable(CAP_NET_ADMIN)) {
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
	printk("cmd = %x\n", cmd);
	printk("buf_size=%d\n", buf_size);


	bytes_written = handle_custom_msg(command, priv_cmd.used_len);
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
	Sreturn ret;
}
#endif


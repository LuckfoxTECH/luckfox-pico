/**
 * aicwf_sdio.h
 *
 * SDIO function declarations
 *
 * Copyright (C) AICSemi 2018-2020
 */

#ifndef _AICWF_SDMMC_H_
#define _AICWF_SDMMC_H_

#ifdef AICWF_SDIO_SUPPORT
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/ieee80211.h>
#include <linux/semaphore.h>
#include "aic_bsp_driver.h"

#define AICBSP_SDIO_NAME                "aicbsp_sdio"
#define SDIOWIFI_FUNC_BLOCKSIZE         512

#define SDIO_VENDOR_ID_AIC              0x8800
#define SDIO_DEVICE_ID_AIC              0x0001
#define SDIOWIFI_BYTEMODE_LEN_REG           0x02
#define SDIOWIFI_INTR_CONFIG_REG            0x04
#define SDIOWIFI_SLEEP_REG	                0x05
#define SDIOWIFI_WAKEUP_REG                 0x09
#define SDIOWIFI_FLOW_CTRL_REG              0x0A
#define SDIOWIFI_REGISTER_BLOCK             0x0B
#define SDIOWIFI_BYTEMODE_ENABLE_REG        0x11
#define SDIOWIFI_BLOCK_CNT_REG              0x12
#define SDIOWIFI_FLOWCTRL_MASK_REG          0x7F
#define SDIOWIFI_WR_FIFO_ADDR			    0x07
#define SDIOWIFI_RD_FIFO_ADDR			    0x08

#define SDIOWIFI_INTR_ENABLE_REG_V3         0x00
#define SDIOWIFI_INTR_PENDING_REG_V3        0x01
#define SDIOWIFI_INTR_TO_DEVICE_REG_V3      0x02
#define SDIOWIFI_FLOW_CTRL_Q1_REG_V3        0x03
#define SDIOWIFI_MISC_INT_STATUS_REG_V3     0x04
#define SDIOWIFI_BYTEMODE_LEN_REG_V3        0x05
#define SDIOWIFI_BYTEMODE_LEN_MSB_REG_V3    0x06
#define SDIOWIFI_BYTEMODE_ENABLE_REG_V3     0x07
#define SDIOWIFI_MISC_CTRL_REG_V3           0x08
#define SDIOWIFI_FLOW_CTRL_Q2_REG_V3        0x09
#define SDIOWIFI_CLK_TEST_RESULT_REG_V3     0x0A
#define SDIOWIFI_RD_FIFO_ADDR_V3            0x0F
#define SDIOWIFI_WR_FIFO_ADDR_V3            0x10

#define SDIOCLK_FREE_RUNNING_BIT        (1 << 6)

#define SDIOWIFI_PWR_CTRL_INTERVAL      30
#define FLOW_CTRL_RETRY_COUNT           50
#define BUFFER_SIZE                     1536
#define TAIL_LEN                        4
#define TXQLEN                          (2048*4)

#define SDIO_SLEEP_ST                    0
#define SDIO_ACTIVE_ST                   1

typedef enum {
	SDIO_TYPE_DATA         = 0X00,
	SDIO_TYPE_CFG          = 0X10,
	SDIO_TYPE_CFG_CMD_RSP  = 0X11,
	SDIO_TYPE_CFG_DATA_CFM = 0X12
} sdio_type;

enum AICWF_IC{
	PRODUCT_ID_AIC8801	=	0,
	PRODUCT_ID_AIC8800DC,
	PRODUCT_ID_AIC8800DW,
	PRODUCT_ID_AIC8800D80
};

struct aic_sdio_reg {
    u8 bytemode_len_reg;
    u8 intr_config_reg;
    u8 sleep_reg;
    u8 wakeup_reg;
    u8 flow_ctrl_reg;
    u8 flowctrl_mask_reg;
    u8 register_block;
    u8 bytemode_enable_reg;
    u8 block_cnt_reg;
    u8 misc_int_status_reg;
    u8 rd_fifo_addr;
    u8 wr_fifo_addr;
};

struct aic_sdio_dev {
	struct rwnx_cmd_mgr cmd_mgr;
	struct sdio_func *func;
	struct sdio_func *func_msg;
	struct device *dev;
	struct aicwf_bus *bus_if;

	struct aicwf_rx_priv *rx_priv;
	struct aicwf_tx_priv *tx_priv;
	u32 state;

#if defined(CONFIG_SDIO_PWRCTRL)
	//for sdio pwr ctrl
	struct timer_list timer;
	uint active_duration;
	struct completion pwrctrl_trgg;
	struct task_struct *pwrctl_tsk;
	spinlock_t pwrctl_lock;
	struct semaphore pwrctl_wakeup_sema;
#endif
	u16 chipid;
    struct aic_sdio_reg sdio_reg;
    void (*sdio_hal_irqhandler) (struct sdio_func *func);
};

void *aicbsp_get_drvdata(void *args);
int aicwf_sdio_writeb(struct aic_sdio_dev *sdiodev, uint regaddr, u8 val);
void aicwf_sdio_hal_irqhandler(struct sdio_func *func);
void aicwf_sdio_hal_irqhandler_func2(struct sdio_func *func);
#if defined(CONFIG_SDIO_PWRCTRL)
void aicwf_sdio_pwrctl_timer(struct aic_sdio_dev *sdiodev, uint duration);
int aicwf_sdio_pwr_stctl(struct  aic_sdio_dev *sdiodev, uint target);
#endif
void aicwf_sdio_reg_init(struct aic_sdio_dev *sdiodev);
int aicwf_sdio_func_init(struct aic_sdio_dev *sdiodev);
int aicwf_sdiov3_func_init(struct aic_sdio_dev *sdiodev);
void aicwf_sdio_func_deinit(struct aic_sdio_dev *sdiodev);
int aicwf_sdio_flow_ctrl(struct aic_sdio_dev *sdiodev);
int aicwf_sdio_recv_pkt(struct aic_sdio_dev *sdiodev, struct sk_buff *skbbuf, u32 size, u8 msg);
int aicwf_sdio_send_pkt(struct aic_sdio_dev *sdiodev, u8 *buf, uint count);
void *aicwf_sdio_bus_init(struct aic_sdio_dev *sdiodev);
void aicwf_sdio_release(struct aic_sdio_dev *sdiodev);
void aicbsp_sdio_exit(void);
int  aicbsp_sdio_init(void);
void aicbsp_sdio_release(struct aic_sdio_dev *sdiodev);
int aicwf_sdio_txpkt(struct aic_sdio_dev *sdiodev, struct sk_buff *pkt);
int aicwf_sdio_bustx_thread(void *data);
int aicwf_sdio_busrx_thread(void *data);
int aicwf_sdio_aggr(struct aicwf_tx_priv *tx_priv, struct sk_buff *pkt);
int aicwf_sdio_send(struct aicwf_tx_priv *tx_priv);
void aicwf_sdio_aggr_send(struct aicwf_tx_priv *tx_priv);
void aicwf_sdio_aggrbuf_reset(struct aicwf_tx_priv *tx_priv);
extern void aicwf_hostif_ready(void);
int aicwf_process_rxframes(struct aicwf_rx_priv *rx_priv);
uint8_t crc8_ponl_107(uint8_t *p_buffer, uint16_t cal_size);
#endif /* AICWF_SDIO_SUPPORT */

#endif /*_AICWF_SDMMC_H_*/

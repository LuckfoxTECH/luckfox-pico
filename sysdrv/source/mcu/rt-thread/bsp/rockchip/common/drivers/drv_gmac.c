/**
  * Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_gmac.c
  * @author  David Wu
  * @version V0.1
  * @date    01-Jul-2021
  * @brief   ETHERNET Driver
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup ETHERNET
 *  @{
 */

/** @defgroup ETHERNET_How_To_Use How To Use
 *  @{

  See more information, click [here](https://www.rt-thread.org/document/site/#/rt-thread-version/rt-thread-standard/programming-manual/netdev/netdev)

 @} */

#include <rthw.h>
#include "drv_pm.h"
#include "drv_heap.h"

#ifdef RT_USING_PM
#include <drivers/pm.h>
#endif

#ifdef RT_USING_GMAC

#include "hal_bsp.h"
#include "drv_clock.h"
#include "drv_gmac.h"

#ifdef RT_USING_LWIP

#include <netif/ethernetif.h>
#include "lwipopts.h"

#define RK_GMAC_DEBUG 0

#define MAX_ADDR_LEN 6
#define ETH_RXBUFNB 4
#define ETH_TXBUFNB 4

#if RK_GMAC_DEBUG
#define rk_gmac_dbg(dev, fmt, ...) \
    rt_kprintf("%s: " fmt, ((struct rt_device *)dev)->parent.name, ##__VA_ARGS__)

#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
static void rt_rockchip_dump_hex(const rt_uint8_t *ptr, rt_size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    rt_kprintf("len: 0x%X: ", buflen);
    for (i = 0; i < buflen; i += 16)
    {
        rt_kprintf("%08X: ", i);

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                rt_kprintf("%02X ", buf[i + j]);
            else
                rt_kprintf("   ");
        rt_kprintf(" ");

        for (j = 0; j < 16; j++)
            if (i + j < buflen)
                rt_kprintf("%c", __is_print(buf[i + j]) ? buf[i + j] : '.');
        rt_kprintf("\n");
    }
}
#else
#define rk_gmac_dbg(dev, fmt, ...) \
do { \
} while(0)

static void rt_rockchip_dump_hex(const rt_uint8_t *ptr, rt_size_t buflen)
{
}
#endif

/* GMAC controller driver's private data. */
struct rockchip_eth
{
    /* inherit from ethernet device */
    struct eth_device parent;

    struct rt_semaphore sem_lock;
    struct rt_event tx_event;

    /* interface address info, hw address */
    rt_uint8_t  dev_addr[MAX_ADDR_LEN];

    const char *name;
    rt_uint8_t id;
    uint32_t pmState;

    const struct rockchip_eth_config *config;

    /* irq handler */
    rt_isr_handler_t irq_handler;

    /* HAL */
    struct GMAC_HANDLE instance;
    const struct HAL_GMAC_DEV *dev;

    struct GMAC_Desc  *rx_desc;
    struct GMAC_Desc  *tx_desc;

    rt_uint8_t *rx_buff;
    rt_uint8_t *tx_buff;
};

extern void rt_hw_cpu_dcache_invalidate(void *addr, int size);
extern void rt_hw_cpu_dcache_clean(void *addr, int size);

/* interrupt service routine */
void rt_rockchip_eth_irq(struct rockchip_eth *eth)
{
    rt_uint32_t status;

    /* enter interrupt */
    rt_interrupt_enter();
    status = HAL_GMAC_IRQHandler(&eth->instance);

    if (status & DMA_HANLE_RX)
    {
        /* a frame has been received */
        eth_device_ready(&eth->parent);
    }

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * is_zero_ethaddr - Determine if give Ethernet address is all zeros.
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return true if the address is all zeroes.
 */
static inline int is_zero_ethaddr(const rt_uint8_t *addr)
{
    return !(addr[0] | addr[1] | addr[2] | addr[3] | addr[4] | addr[5]);
}

/**
 * is_multicast_ethaddr - Determine if the Ethernet address is a multicast.
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return true if the address is a multicast address.
 * By definition the broadcast address is also a multicast address.
 */
static inline int is_multicast_ethaddr(const rt_uint8_t *addr)
{
    return 0x01 & addr[0];
}

/*
 * is_broadcast_ethaddr - Determine if the Ethernet address is broadcast
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Return true if the address is the broadcast address.
 */
static inline int is_broadcast_ethaddr(const rt_uint8_t *addr)
{
    return (addr[0] & addr[1] & addr[2] & addr[3] & addr[4] & addr[5]) ==
           0xff;
}

/*
 * is_valid_ethaddr - Determine if the given Ethernet address is valid
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Check that the Ethernet address (GMAC) is not 00:00:00:00:00:00, is not
 * a multicast address, and is not FF:FF:FF:FF:FF:FF.
 *
 * Return true if the address is valid.
 */
static inline int is_valid_ethaddr(const rt_uint8_t *addr)
{
    /* FF:FF:FF:FF:FF:FF is a multicast address so we don't need to
     * explicitly check for it here. */
    return !is_multicast_ethaddr(addr) && !is_zero_ethaddr(addr);
}

/**
 * net_random_ethaddr - Generate software assigned random Ethernet address
 * @addr: Pointer to a six-byte array containing the Ethernet address
 *
 * Generate a random Ethernet address (GMAC) that is not multicast
 * and has the local assigned bit set.
 */
static inline void net_random_ethaddr(rt_uint8_t *addr)
{
    unsigned int seed = HAL_TIMER_GetCount(SYS_TIMER) | 0xffffffff;
    rt_int8_t i;

    for (i = 0; i < 6; i++)
        addr[i] = rand_r(&seed);

    addr[0] &= 0xfe;    /* clear multicast bit */
    addr[0] |= 0x02;    /* set local assignment bit (IEEE802) */
}

/* initialization function */
static rt_err_t rt_rockchip_eth_init(rt_device_t dev)
{
    struct rockchip_eth *eth = (struct rockchip_eth *)dev;
    struct GMAC_HANDLE *pGMAC;
    rt_err_t ret;

    rk_gmac_dbg(&eth->parent, "rt_rockchip_eth_init\n");
    pGMAC = &eth->instance;

    eth->rx_desc = rt_malloc_uncache(ETH_RXBUFNB * sizeof(struct GMAC_Desc));
    if (!eth->rx_desc)
    {
        rk_gmac_dbg(&eth->parent, "rt_malloc_uncache failed\n");
        return -RT_ENOMEM;
    }

    eth->tx_desc = rt_malloc_uncache(ETH_TXBUFNB * sizeof(struct GMAC_Desc));
    if (!eth->tx_desc)
    {
        rk_gmac_dbg(&eth->parent, "rt_malloc_uncache failed\n");
        ret = -RT_ENOMEM;
        goto err;
    }

    rt_memset(eth->rx_desc, 0, sizeof(struct GMAC_Desc) * ETH_RXBUFNB);
    rt_memset(eth->tx_desc, 0, sizeof(struct GMAC_Desc) * ETH_TXBUFNB);

    eth->rx_buff = rt_malloc_align(HAL_GMAC_MAX_PACKET_SIZE * ETH_RXBUFNB, HAL_GMAC_MAX_PACKET_SIZE);
    if (!eth->rx_buff)
    {
        rk_gmac_dbg(&eth->parent, "rt_malloc_align failed\n");
        ret = -RT_ENOMEM;
        goto err;
    }
    eth->tx_buff = rt_malloc_align(HAL_GMAC_MAX_PACKET_SIZE * ETH_TXBUFNB, HAL_GMAC_MAX_PACKET_SIZE);
    if (!eth->tx_buff)
    {
        rk_gmac_dbg(&eth->parent, "rt_malloc_align failed\n");
        ret = -RT_ENOMEM;
        goto err;
    }

    rt_memset(eth->rx_buff, 0, HAL_GMAC_MAX_PACKET_SIZE * ETH_RXBUFNB);
    rt_memset(eth->tx_buff, 0, HAL_GMAC_MAX_PACKET_SIZE * ETH_TXBUFNB);

    /* Initialize Rx Descriptors list */
    HAL_GMAC_DMARxDescInit(pGMAC, eth->rx_desc, eth->rx_buff, ETH_RXBUFNB);
    /* Initialize Tx Descriptors list */
    HAL_GMAC_DMATxDescInit(pGMAC, eth->tx_desc, eth->tx_buff, ETH_TXBUFNB);

    /* Register irq */
    rt_hw_interrupt_install(eth->dev->irqNum, eth->irq_handler, eth, eth->parent.parent.parent.name);
    rt_hw_interrupt_umask(eth->dev->irqNum);

    clk_enable_by_id(eth->dev->pclkGateID);
    clk_enable_by_id(eth->dev->clkGateID);

    /* Enable GMAC and DMA transmission and reception */
    ret = HAL_GMAC_Start(pGMAC, eth->dev_addr);
    if (ret == HAL_OK)
    {
        rk_gmac_dbg(&eth->parent, "GMAC hardware started\n");
        return RT_EOK;
    }
    else
    {
        rk_gmac_dbg(&eth->parent, "GMAC hardware start faild: %d\n", ret);
    }

err:
    if (eth->rx_desc)
        rt_free_uncache(eth->rx_desc);
    if (eth->tx_desc)
        rt_free_uncache(eth->tx_desc);

    if (eth->rx_buff)
        rt_free_align(eth->rx_buff);
    if (eth->tx_buff)
        rt_free_align(eth->tx_buff);

    return ret;
}

static rt_err_t rt_rockchip_eth_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t rt_rockchip_eth_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_size_t rt_rockchip_eth_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_set_errno(-RT_ENOSYS);
    return RT_EOK;
}

static rt_size_t rt_rockchip_eth_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    rt_set_errno(-RT_ENOSYS);
    return RT_EOK;
}

/* mac address control function */
static rt_err_t rt_rockchip_eth_control(rt_device_t dev, int cmd, void *args)
{
    struct rockchip_eth *eth = (struct rockchip_eth *)dev;

    switch (cmd)
    {
    case NIOCTL_GADDR:
        if (args) rt_memcpy(args, eth->dev_addr, 6);
        else return -RT_ERROR;
        break;

    default :
        break;
    }
    return RT_EOK;
}

/* phy hardware reset function */
static void rt_rockchip_phy_reset(const struct rockchip_eth_config *config)
{
    if (config->reset_gpio_bank)
    {
        HAL_GPIO_SetPinDirection(config->reset_gpio_bank,
                                 config->reset_gpio_num,
                                 GPIO_OUT);
        HAL_GPIO_SetPinLevel(config->reset_gpio_bank,
                             config->reset_gpio_num,
                             GPIO_HIGH);
        rt_thread_mdelay(config->reset_delay_ms[0]);
        HAL_GPIO_SetPinLevel(config->reset_gpio_bank,
                             config->reset_gpio_num,
                             GPIO_LOW);
        rt_thread_mdelay(config->reset_delay_ms[1]);
        HAL_GPIO_SetPinLevel(config->reset_gpio_bank,
                             config->reset_gpio_num,
                             GPIO_HIGH);
        rt_thread_mdelay(config->reset_delay_ms[2]);
    }
}

/* phy link status monitor thread */
static void phy_monitor_thread_entry(void *parameter)
{
    struct rockchip_eth *eth = (struct rockchip_eth *)parameter;
    struct GMAC_HANDLE *pGMAC = &eth->instance;
    rt_size_t status;

    status = HAL_GMAC_PHYStartup(pGMAC);
    if (status)
    {
        rk_gmac_dbg(&eth->parent, "HAL_PHY_Startup() failed: %d\n", status);
        return;
    }

    while (1)
    {
        status = HAL_GMAC_PHYUpdateLink(pGMAC);
        if ((status == HAL_OK) && (pGMAC->phyStatus.link != pGMAC->phyStatus.oldLink))
        {
            pGMAC->phyStatus.oldLink = pGMAC->phyStatus.link;
            if (pGMAC->phyStatus.link)
            {
                status = HAL_GMAC_PHYParseLink(pGMAC);
                if (PHY_SPEED_1000M ==  pGMAC->phyStatus.speed)
                {
                    rt_kprintf("%s: 1000M\n", eth->parent.parent.parent.name);
                }
                else if (PHY_SPEED_100M ==  pGMAC->phyStatus.speed)
                {
                    rt_kprintf("%s: 100M\n", eth->parent.parent.parent.name);
                }
                else
                {
                    rt_kprintf("%s: 10M\n", eth->parent.parent.parent.name);
                }

                if (PHY_DUPLEX_HALF == pGMAC->phyStatus.duplex)
                {
                    rt_kprintf("%s: half dumplex\n", eth->parent.parent.parent.name);

                }
                else
                {
                    rt_kprintf("%s: full dumplex\n", eth->parent.parent.parent.name);
                }

                if (pGMAC->phyStatus.pause)
                {
                    rt_kprintf("%s: flow control rx/tx\n", eth->parent.parent.parent.name);
                }
                else
                {
                    rt_kprintf("%s: flow control off\n", eth->parent.parent.parent.name);
                }

                status = HAL_GMAC_AdjustLink(pGMAC, eth ->config->tx_delay, eth ->config->rx_delay);
                if (status != HAL_OK)
                {
                    rk_gmac_dbg(&eth->parent, "HAL_GMAC_AdjustLink() failed: %d\n", status);
                }

                rt_kprintf("%s: link up.\n", eth->parent.parent.parent.name);
                eth_device_linkchange(&eth->parent, RT_TRUE);
            }
            else
            {
                eth_device_linkchange(&eth->parent, RT_FALSE);
                rt_kprintf("%s: link down.\n", eth->parent.parent.parent.name);
            }
        }

        rt_thread_delay(RT_TICK_PER_SECOND * 3);
    }
}

const struct rt_device_ops eth_ops =
{
    .init = rt_rockchip_eth_init,
    .open = rt_rockchip_eth_open,
    .close = rt_rockchip_eth_close,
    .read = rt_rockchip_eth_read,
    .write = rt_rockchip_eth_write,
    .control = rt_rockchip_eth_control,
};

RT_WEAK const struct rockchip_eth_config rockchip_eth_config_table[] = {0};

#define DEFINE_ROCKCHIP_ETH(ID)                             \
static void rockchip_eth##ID##_irq(int irq, void *param);   \
                                                            \
static struct rockchip_eth eth##ID =                        \
{                                                           \
    .name = "eth"#ID,                                       \
    .id = ID,                                                       \
    .dev = &g_gmac##ID##Dev,                                 \
    .irq_handler = rockchip_eth##ID##_irq,                  \
};                                                          \
                                                            \
static void rockchip_eth##ID##_irq(int irq, void *param)    \
{                                                           \
    rt_rockchip_eth_irq(&eth##ID);                             \
}

#ifdef RT_USING_GMAC0
DEFINE_ROCKCHIP_ETH(0)
#endif

#ifdef RT_USING_GMAC1
DEFINE_ROCKCHIP_ETH(1)
#endif

static struct rockchip_eth *const rockchip_eth_table[] =
{
#ifdef RT_USING_GMAC0
    &eth0,
#endif

#ifdef RT_USING_GMAC1
    &eth1,
#endif

    RT_NULL
};

/* Ethernet Device Interface */
/* transmit packet. */
rt_err_t rt_rockchip_eth_tx(rt_device_t dev, struct pbuf *p)
{
    struct rockchip_eth *eth = (struct rockchip_eth *)dev;
    struct GMAC_HANDLE *pGMAC = &eth->instance;
    rt_uint8_t *ptr = RT_NULL;
    rt_err_t status = RT_EOK;

    if (!pGMAC->phyStatus.link)
        return RT_EOK;

    /* Check the frame length. */
    if (p->tot_len > HAL_GMAC_MAX_FRAME_SIZE)
        return RT_EOK;

    /* lock ETH device */
    rt_sem_take(&eth ->sem_lock, RT_WAITING_FOREVER);

    /* copy data to tx buffer */
    ptr = (rt_uint8_t *)HAL_GMAC_GetTXBuffer(pGMAC);

    pbuf_copy_partial(p, ptr, p->tot_len, 0);
    rt_rockchip_dump_hex(p->payload, p->tot_len);

    rt_hw_cpu_dcache_clean(ptr, p->tot_len);

    status = HAL_GMAC_Send(pGMAC, ptr, p->tot_len);
    if (status)
    {
        rk_gmac_dbg(&eth->parent, "GMAC send failed: %d\n", status);
    }

    /* unlock ETH device */
    rt_sem_release(&eth ->sem_lock);

    return status;
}

/* reception packet. */
struct pbuf *rt_rockchip_eth_rx(rt_device_t dev)
{
    struct rockchip_eth *eth = (struct rockchip_eth *)dev;
    struct GMAC_HANDLE *pGMAC = &eth->instance;
    struct pbuf *p = RT_NULL;
    rt_uint8_t *ptr = RT_NULL;
    rt_int32_t size;

    if (!pGMAC->phyStatus.link)
        return RT_NULL;

    /* lock ETH device */
    rt_sem_take(&eth ->sem_lock, RT_WAITING_FOREVER);

    ptr = HAL_GMAC_Recv(pGMAC, &size);
    if (size > 0 && ptr)
    {
        rt_hw_cpu_dcache_invalidate(ptr, size);
        /* allocate buffer */
        p = pbuf_alloc(PBUF_LINK, size, PBUF_RAM);
        if (p != RT_NULL)
        {
            pbuf_take(p, ptr, size);
        }
        rt_rockchip_dump_hex(p->payload, p->tot_len);
        HAL_GMAC_CleanRX(pGMAC);
    }
    else
    {
        rk_gmac_dbg(&eth->parent, "GMAC recv failed: %d\n", size);
        /* unlock ETH device */
        rt_sem_release(&eth ->sem_lock);

        return RT_NULL;
    }

    /* unlock ETH device */
    rt_sem_release(&eth ->sem_lock);

    return p;
}

int rt_rockchip_hw_eth_init(void)
{
    struct rockchip_eth *const *eth;
    const struct rockchip_eth_config *cfg;
    rt_err_t state = RT_EOK;

    for (eth = rockchip_eth_table; *eth != RT_NULL; eth++)
    {
        const struct HAL_GMAC_DEV *gmac_dev = (*eth)->dev;
        struct GMAC_PHY_Config config;
        eGMAC_PHY_Interface interface;
        char name[32] = {0};
        rt_uint32_t freq;
        rt_size_t status;

        (*eth)->config = RT_NULL;
        for (cfg = rockchip_eth_config_table; cfg->id != 0; cfg++)
        {
            if ((*eth)->dev->pReg == cfg->id)
            {
                (*eth)->config = cfg;
            }
        }

        if (!(*eth)->config)
            continue;

        /* PHY reset */
        rt_rockchip_phy_reset((*eth)->config);

        interface = (*eth)->config->mode;
        if (interface == PHY_INTERFACE_MODE_RGMII)
        {
            clk_set_rate(gmac_dev->clkID, 125000000);
        }
        else
        {
            clk_set_rate(gmac_dev->clkID, 50000000);
        }
        freq = clk_get_rate(gmac_dev->pclkID);
        HAL_GMAC_Init(&(*eth)->instance, gmac_dev->pReg, freq, interface);

        config.speed = (*eth)->config->speed;
        config.maxSpeed = (*eth)->config->max_speed;
        config.duplexMode = PHY_DUPLEX_FULL;
        config.neg = PHY_AUTONEG_ENABLE;
        config.interface = interface;
        config.phyAddress = (*eth)->config->phy_addr;
        status = HAL_GMAC_PHYInit(&(*eth)->instance, &config);
        if (status)
        {
            rt_kprintf("HAL_PHY_Init() failed: %d\n", status);
            return status;
        }

        if (!is_valid_ethaddr((*eth)->dev_addr))
        {
            net_random_ethaddr((*eth)->dev_addr);
        }
        HAL_GMAC_WriteHWAddr(&(*eth)->instance, (*eth)->dev_addr);

        (*eth)->parent.parent.ops           = &eth_ops;
        (*eth)->parent.parent.user_data = *eth;
        (*eth)->parent.parent.type          = RT_Device_Class_NetIf;

        (*eth)->parent.eth_rx = rt_rockchip_eth_rx;
        (*eth)->parent.eth_tx = rt_rockchip_eth_tx;

        sprintf(name, "e%d_wait", (*eth)->id);
        /* init tx semaphore */
        rt_sem_init(&(*eth)->sem_lock, name, 1, RT_IPC_FLAG_FIFO);

        rt_memset(name, 0, sizeof(name));
        sprintf(name, "e%d", (*eth)->id);

        /* register eth device */
        state = eth_device_init(&((*eth)->parent), name);
        if (RT_EOK == state)
        {
            rt_kprintf("eth_device_init success\n");
        }
        else
        {
            rt_kprintf("eth_device_init faild: %d\n", state);
        }

        eth_device_linkchange(&((*eth)->parent), RT_FALSE);
        /* start phy monitor */
        {
            rt_thread_t tid;

            rt_memset(name, 0, sizeof(name));
            sprintf(name, "e%d_phy", (*eth)->id);
            tid = rt_thread_create(name,
                                   phy_monitor_thread_entry,
                                   (*eth),
                                   512 * 4,
                                   RT_THREAD_PRIORITY_MAX - 2,
                                   2);
            if (tid != RT_NULL)
                rt_thread_startup(tid);
        }
    }

    return state;
}

INIT_DEVICE_EXPORT(rt_rockchip_hw_eth_init);

/** @} */

/** @} */

#ifdef RK_GMAC_DEBUG
#ifdef RT_USING_FINSH
#include <finsh.h>

static int8_t eth_id = 0;
void switch_id(int8_t id)
{
    eth_id = id;
}

struct rockchip_eth *get_rockchip_eth_data(void)
{
    switch (eth_id)
    {
    case 0:
#ifdef RT_USING_GMAC0
        return &eth0;
#else
        return RT_NULL;
#endif
    case 1:
#ifdef RT_USING_GMAC1
        return &eth1;
#else
        return RT_NULL;
#endif
    default:
        return RT_NULL;
    }
    return RT_NULL;
}

void phy_read(uint32_t phyReg)
{
    struct GMAC_HANDLE *pGMAC;
    struct rockchip_eth *eth;
    int data;

    eth = get_rockchip_eth_data();
    if (eth)
        pGMAC = &eth->instance;
    else
        return;

    data = HAL_GMAC_MDIORead(pGMAC, 0, phyReg);
    if (data >= 0)
    {
        rt_kprintf("PHY_Read: %02X --> %08X\n", phyReg, data);
    }
    else
    {
        rt_kprintf("PHY_Read: %02X --> faild\n", phyReg);
    }
}

void phy_write(uint32_t phyReg, uint32_t data)
{
    struct GMAC_HANDLE *pGMAC;
    struct rockchip_eth *eth;
    int status;

    eth = get_rockchip_eth_data();
    if (eth)
        pGMAC = &eth->instance;
    else
        return;

    status = HAL_GMAC_MDIOWrite(pGMAC, 0, phyReg, data);
    if (!status)
    {
        rt_kprintf("PHY_Write: %02X --> %08X\n", phyReg, data);
    }
    else
    {
        rt_kprintf("PHY_Write: %02X --> faild\n", phyReg);
    }
}

void phy_dump(void)
{
    struct GMAC_HANDLE *pGMAC;
    struct rockchip_eth *eth;
    int data, i;

    eth = get_rockchip_eth_data();
    if (eth)
        pGMAC = &eth->instance;
    else
        return;

    for (i = 0; i < 32; i++)
    {
        data = HAL_GMAC_MDIORead(pGMAC, 0, i);
        if (data < 0)
        {
            rt_kprintf("phy_dump: %02X --> faild\n", i);
            break;
        }

        if (i % 8 == 7)
        {
            rt_kprintf("%02X --> %08X\n ", i, data);
        }
        else
        {
            rt_kprintf("%02X --> %08X\n\n", i, data);
        }
    }
}

void dump_net_stat(void)
{
    struct rockchip_eth *eth;
    struct GMAC_HANDLE *pGMAC;
    char *buf, *p;

    eth = get_rockchip_eth_data();
    if (eth)
        pGMAC = &eth->instance;
    else
        return;

    buf = rt_malloc(1024);
    if (!buf)
        return;
    rt_memset(buf, 0, 1024);
    p = buf;

    p += sprintf(p, "interface_name=\"%s\"\n", eth->parent.parent.parent.name);
    p += sprintf(p, "drive_name=\"%s\"\n", eth->name);
    p += sprintf(p, "num_tx_bytes = %ld\n", pGMAC->extraStatus.txPktN);
    p += sprintf(p, "num_rx_bytes = %ld\n",  pGMAC->extraStatus.rxPktN);
    p += sprintf(p, "num_tx_pkts = %ld\n", pGMAC->extraStatus.txPktN);
    p += sprintf(p, "num_rx_pkts = %ld\n",  pGMAC->extraStatus.rxPktN);
    p += sprintf(p, "num_tx_pkts_err = %ld\n", pGMAC->extraStatus.txErrors);
    p += sprintf(p, "num_rx_pkts_err = %ld\n", pGMAC->extraStatus.rxErrors);
    p += sprintf(p, "carrier %s\n", (pGMAC->phyStatus.link)
                 ? "on" : "off");
    rt_kprintf("%s", p);
}

void dump_desc_stat(void)
{
    struct rockchip_eth *eth;
    int i = 0;

    eth = get_rockchip_eth_data();
    if (!eth)
        return;

    for (i = 0; i < ETH_RXBUFNB; i++)
    {
        rt_kprintf("[%2d] des0: 0x%8x, des1: %0x%8x, des2: %0x%8x, des3: %0x%8x\n",
                   i,
                   eth->rx_desc[i].des0,
                   eth->rx_desc[i].des0,
                   eth->rx_desc[i].des0,
                   eth->rx_desc[i].des0);
    }

    for (i = 0; i < ETH_TXBUFNB; i++)
    {
        rt_kprintf("[%2d] des0: 0x%8x, des1: %0x%8x, des2: %0x%8x, des3: %0x%8x\n",
                   i,
                   eth->tx_desc[i].des0,
                   eth->tx_desc[i].des0,
                   eth->tx_desc[i].des0,
                   eth->tx_desc[i].des0);
    }
}

FINSH_FUNCTION_EXPORT(switch_id, switch eth id);
FINSH_FUNCTION_EXPORT(phy_read, read phy register);
FINSH_FUNCTION_EXPORT(phy_write, write phy register);
FINSH_FUNCTION_EXPORT(phy_dump, dump phy registers);
FINSH_FUNCTION_EXPORT(dump_net_stat, dump network info);
FINSH_FUNCTION_EXPORT(dump_desc_stat, dump description info);
#endif
#endif
#endif
#endif

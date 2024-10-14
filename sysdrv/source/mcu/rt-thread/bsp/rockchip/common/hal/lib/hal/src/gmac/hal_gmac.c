/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_GMAC_MODULE_ENABLED

/** @addtogroup RK_HAL_Driver
 *  @{
 */

/** @addtogroup GMAC
 *  @brief GMAC HAL module driver
 *  @{
 */

/** @defgroup GMAC_How_To_Use How To Use
 *  @{

 The GMAC HAL driver can be used as follows:

 - Declare a GMAC_HANDLE handle structure, for example:
   ```
   GMAC_HANDLE instance;
   ```
 - Invoke HAL_GMAC_Init() API to initialize base address, csr clock and interface:
   - Base register address;
   - Pclk clock rate;
   - Interface mode.

 - Invoke HAL_GMAC_PHYInit() API to connect PHY:
   - Must implement PHY hardware reset before invoke this function;
   - Interface mode;
   - PHY address;
   - Auto negotiation or not;
   - Duplex mode configured if auto negotiation is disable;
   - Alos config desired speed if it is disable auto negotiation;
   - Max speed is optional.

 - Invoke HAL_GMAC_WriteHWAddr() API to write mac address:

 - Invoke HAL_GMAC_DMARxDescInit() and HAL_GMAC_DMATxDescInit() APIs to init desc and buffer:
   - Desc memory must declare as uncached;
   - Buffer memory use cached meory;

 - Invoke HAL_GMAC_Start() API to start gmac.

 - Invoke HAL_GMAC_PHYStartup() API to configure PHY and do negotiation.
 - Invoke HAL_GMAC_PHYUpdateLink() API to get PHY is linked or not.
 - Invoke HAL_GMAC_PHYParseLink() API to get PHY linked information:
   - Linked speed;
   - Linked duplex;
   - Flow ctrl on or off.

 - Inloke HAL_GMAC_MDIORead() and HAL_GMAC_MDIOWrite() read/write phy reg value.

 - Invoke HAL_GMAC_Send() and HAL_GMAC_Recv() for transfer:
   - Must clean the dcached memory before use HAL_GMAC_Send();
   - Must invalidate dcached memory after use HAL_GMAC_Recv();
   - Use HAL_GMAC_IRQHandler() to handle Tx/Rx interrupt;
   - Use HAL_GMAC_GetTXBuffer() and HAL_GMAC_GetRXBuffer() to get current buffer;
   - Use HAL_GMAC_CleanRX() to clean Rx dirty description.

 - Invoke HAL_GMAC_Stop() to stop transfer.

 - Invoke HAL_GMAC_DeInit() if necessary.
 @} */

/** @defgroup GMAC_Private_Definition Private Definition
 *  @{
 */
/********************* Private MACRO Definition ******************************/

/* GMAC config */
#define GMAC_CONFIG_GPSLCE (1 << GMAC_MAC_CONFIGURATION_GPSLCE_SHIFT)
#define GMAC_CONFIG_IPC    (1 << GMAC_MAC_CONFIGURATION_IPC_SHIFT)
#define GMAC_CONFIG_2K     (1 << GMAC_MAC_CONFIGURATION_S2KP_SHIFT)
#define GMAC_CONFIG_CST    (1 << GMAC_MAC_CONFIGURATION_CST_SHIFT)
#define GMAC_CONFIG_ACS    (1 << GMAC_MAC_CONFIGURATION_ACS_SHIFT)
#define GMAC_CONFIG_WD     (1 << GMAC_MAC_CONFIGURATION_WD_SHIFT)
#define GMAC_CONFIG_BE     (1 << GMAC_MAC_CONFIGURATION_BE_SHIFT)
#define GMAC_CONFIG_JD     (1 << GMAC_MAC_CONFIGURATION_JD_SHIFT)
#define GMAC_CONFIG_JE     (1 << GMAC_MAC_CONFIGURATION_JE_SHIFT)
#define GMAC_CONFIG_PS     (1 << GMAC_MAC_CONFIGURATION_PS_SHIFT)
#define GMAC_CONFIG_FES    (1 << GMAC_MAC_CONFIGURATION_FES_SHIFT)
#define GMAC_CONFIG_DM     (1 << GMAC_MAC_CONFIGURATION_DM_SHIFT)
#define GMAC_CONFIG_DCRS   (1 << GMAC_MAC_CONFIGURATION_DCRS_SHIFT)
#define GMAC_CONFIG_TE     (1 << GMAC_MAC_CONFIGURATION_TE_SHIFT)
#define GMAC_CONFIG_RE     (1 << GMAC_MAC_CONFIGURATION_RE_SHIFT)

#define GMAC_CORE_INIT (GMAC_CONFIG_JD | GMAC_CONFIG_PS | GMAC_CONFIG_ACS | \
                        GMAC_CONFIG_BE | GMAC_CONFIG_DCRS)

/* GMAC HW features1 bitmap */
#define GMAC_HW_FEAT_AVSEL       (1 << GMAC_MAC_HW_FEATURE1_AVSEL_SHIFT)
#define GMAC_HW_TSOEN            (1 << GMAC_MAC_HW_FEATURE1_TSOEN_SHIFT)
#define GMAC_HW_TXFIFOSIZE_SHIFT GMAC_MAC_HW_FEATURE1_TXFIFOSIZE_SHIFT
#define GMAC_HW_RXFIFOSIZE_SHIFT GMAC_MAC_HW_FEATURE1_RXFIFOSIZE_SHIFT
#define GMAC_HW_TXFIFOSIZE       (0x1f << GMAC_MAC_HW_FEATURE1_TXFIFOSIZE_SHIFT)
#define GMAC_HW_RXFIFOSIZE       (0x1f << GMAC_MAC_HW_FEATURE1_RXFIFOSIZE_SHIFT)

/* Flow Ctrl */
#define GMAC_Q0_TX_FLOW_CTRL_PT_SHIFT GMAC_MAC_Q0_TX_FLOW_CTRL_PT_SHIFT
#define GMAC_Q0_TX_FLOW_CTRL_PT_MASK  0xffff
#define GMAC_Q0_TX_FLOW_CTRL_TFE      (1 << GMAC_MAC_Q0_TX_FLOW_CTRL_TFE_SHIFT)
#define GMAC_RX_FLOW_CTRL_RFE         (1 << GMAC_MAC_RX_FLOW_CTRL_RFE_SHIFT)

#define HAL_PAUSE_TIME 0xffff

/* MMC register */
#define MMC_CNTRL_COUNTER_RESET (1 << GMAC_MMC_CONTROL_CNTRST_SHIFT)
/* When set, do not roll over zero after reaching the max value */
#define MMC_CNTRL_COUNTER_STOP_ROLLOVER (1 << GMAC_MMC_CONTROL_CNTSTOPRO_SHIFT)
/* Reset after reading */
#define MMC_CNTRL_RESET_ON_READ (1 << GMAC_MMC_CONTROL_RSTONRD_SHIFT)
/* Freeze counter values to the current value */
#define MMC_CNTRL_COUNTER_FREEZER  (1 << GMAC_MMC_CONTROL_CNTFREEZ_SHIFT)
#define MMC_CNTRL_PRESET           (1 << GMAC_MMC_CONTROL_CNTPRST_SHIFT)
#define MMC_CNTRL_FULL_HALF_PRESET (1 << GMAC_MMC_CONTROL_CNTPRSTLVL_SHIFT)

#define MMC_RX_INTR_MASK 0x0c /* MMC Interrupt Mask */
#define MMC_TX_INTR_MASK 0x10 /* MMC Interrupt Mask */
#define MMC_DEFAULT_MASK 0xffffffff

/* Mdc/Mdio register */
#define GMAC_CSR_60_100M  0x0 /* MDC = clk_scr_i/42 */
#define GMAC_CSR_100_150M 0x1 /* MDC = clk_scr_i/62 */
#define GMAC_CSR_20_35M   0x2 /* MDC = clk_scr_i/16 */
#define GMAC_CSR_35_60M   0x3 /* MDC = clk_scr_i/26 */
#define GMAC_CSR_150_250M 0x4 /* MDC = clk_scr_i/102 */
#define GMAC_CSR_250_300M 0x5 /* MDC = clk_scr_i/122 */

#define GMAC_MDIO_ADDRESS_PA_SHIFT  (GMAC_MAC_MDIO_ADDRESS_PA_SHIFT)
#define GMAC_MDIO_ADDRESS_RDA_SHIFT (GMAC_MAC_MDIO_ADDRESS_RDA_SHIFT)
#define GMAC_MDIO_ADDRESS_CR_SHIFT  (GMAC_MAC_MDIO_ADDRESS_CR_SHIFT)
#define GMAC_MDIO_ADDRESS_SKAP      (0x1 << GMAC_MAC_MDIO_ADDRESS_SKAP_SHIFT)
#define GMAC_MDIO_ADDRESS_GOC_SHIFT (GMAC_MAC_MDIO_ADDRESS_GOC_0_SHIFT)
#define GMAC_MDIO_ADDRESS_GOC_READ  (0x3 << GMAC_MAC_MDIO_ADDRESS_GOC_0_SHIFT)
#define GMAC_MDIO_ADDRESS_GOC_WRITE (0x1 << GMAC_MAC_MDIO_ADDRESS_GOC_0_SHIFT)
#define GMAC_MDIO_ADDRESS_C45E      (0x1 << GMAC_MAC_MDIO_ADDRESS_C45E_SHIFT)
#define GMAC_MDIO_ADDRESS_GB        (0x1 << GMAC_MAC_MDIO_ADDRESS_GB_SHIFT)

#define GMAC_MDIO_DATA_GD_MASK 0xffff

#define GMAC_MDIO_TIMEOUT 100 /* ms */

/* MTL */
#define MTL_TXQ0_OPERATION_MODE_TSF (1 << GMAC_MTL_TXQ0_OPERATION_MODE_TSF_SHIFT)
#define MTL_TXQ0_OPERATION_MODE_FTQ (1 << GMAC_MTL_TXQ0_OPERATION_MODE_FTQ_SHIFT)

#define MTL_TXQ0_DEBUG_TXQSTS       (1 << GMAC_MTL_TXQ0_DEBUG_TXQSTS_SHIFT)
#define MTL_TXQ0_DEBUG_TRCSTS_SHIFT GMAC_MTL_TXQ0_DEBUG_TRCSTS_SHIFT
#define MTL_TXQ0_DEBUG_TRCSTS_MASK  GMAC_MTL_TXQ0_DEBUG_TRCSTS_MASK

#define MTL_RXQ0_OPERATION_MODE_EHFC (1 << GMAC_MTL_RXQ0_OPERATION_MODE_EHFC_SHIFT)
#define MTL_RXQ0_OPERATION_MODE_RSF  (1 << GMAC_MTL_RXQ0_OPERATION_MODE_RSF_SHIFT)
#define MTL_RXQ0_OPERATION_MODE_FEP  (1 << GMAC_MTL_RXQ0_OPERATION_MODE_FEP_SHIFT)
#define MTL_RXQ0_OPERATION_MODE_FUP  (1 << GMAC_MTL_RXQ0_OPERATION_MODE_FUP_SHIFT)

#define MTL_OP_MODE_TXQEN_MASK (3 << 2)
#define MTL_OP_MODE_TXQEN_AV   (1 << 2)
#define MTL_OP_MODE_TXQEN      (1 << 3)
#define MTL_OP_MODE_TSF        (1 << 1)

#define MTL_OP_MODE_TQS_MASK  (0x1f<< 16)
#define MTL_OP_MODE_TQS_SHIFT 16

#define MTL_OP_MODE_TTC_MASK  GMAC_MTL_TXQ0_OPERATION_MODE_TTC_MASK
#define MTL_OP_MODE_TTC_SHIFT GMAC_MTL_TXQ0_OPERATION_MODE_TTC_SHIFT

#define MTL_OP_MODE_TTC_32  0
#define MTL_OP_MODE_TTC_64  (1 << MTL_OP_MODE_TTC_SHIFT)
#define MTL_OP_MODE_TTC_96  (2 << MTL_OP_MODE_TTC_SHIFT)
#define MTL_OP_MODE_TTC_128 (3 << MTL_OP_MODE_TTC_SHIFT)
#define MTL_OP_MODE_TTC_192 (4 << MTL_OP_MODE_TTC_SHIFT)
#define MTL_OP_MODE_TTC_256 (5 << MTL_OP_MODE_TTC_SHIFT)
#define MTL_OP_MODE_TTC_384 (6 << MTL_OP_MODE_TTC_SHIFT)
#define MTL_OP_MODE_TTC_512 (7 << MTL_OP_MODE_TTC_SHIFT)

#define MTL_OP_MODE_RQS_SHIFT 20
#define MTL_OP_MODE_RQS_MASK  (0x3ff <<MTL_OP_MODE_RQS_SHIFT)

#define MTL_OP_MODE_RFD_SHIFT 14
#define MTL_OP_MODE_RFD_MASK  (0x3f << MTL_OP_MODE_RFD_SHIFT)

#define MTL_OP_MODE_RFA_SHIFT 8
#define MTL_OP_MODE_RFA_MASK  (0x3f << MTL_OP_MODE_RFA_SHIFT)

#define MTL_OP_MODE_RTC_SHIFT 3
#define MTL_OP_MODE_RTC_MASK  (0x3 << MTL_OP_MODE_RTC_SHIFT)

#define MTL_OP_MODE_RTC_32  (1 << MTL_OP_MODE_RTC_SHIFT)
#define MTL_OP_MODE_RTC_64  0
#define MTL_OP_MODE_RTC_96  (2 << MTL_OP_MODE_RTC_SHIFT)
#define MTL_OP_MODE_RTC_128 (3 << MTL_OP_MODE_RTC_SHIFT)

#define SF_DMA_MODE 1 /* DMA STORE-AND-FORWARD Operation Mode */

/*  GMAC Interrupt bitmap*/
#define GMAC_INT_RGSMIIS   (1 << 0)
#define GMAC_INT_PCS_LINK  (1 << 1)
#define GMAC_INT_PCS_ANE   (1 << 2)
#define GMAC_INT_PCS_PHYIS (1 << 3)
#define GMAC_INT_PMT_EN    (1 << 4)
#define GMAC_INT_LPI_EN    (1 << 5)

/*  MTL interrupt */
#define MTL_RX_OVERFLOW_INT_EN (1 << 24)
#define MTL_RX_OVERFLOW_INT    (1 << 16)

#define GMAC_PCS_IRQ_DEFAULT (GMAC_INT_RGSMIIS | GMAC_INT_PCS_LINK | \
                              GMAC_INT_PCS_ANE)

#define GMAC_INT_DEFAULT_ENABLE (GMAC_INT_PMT_EN | GMAC_INT_LPI_EN)
/* DMA MODE */
#define DMA_MODE_SWR (0x1 << GMAC_DMA_MODE_SWR_SHIFT)

#define DMA_SYSBUS_MODE_EAME   (1 << 11)
#define DMA_SYSBUS_MODE_BLEN16 (1 << GMAC_DMA_SYSBUS_MODE_BLEN16_SHIFT)
#define DMA_SYSBUS_MODE_BLEN8  (1 << GMAC_DMA_SYSBUS_MODE_BLEN8_SHIFT)
#define DMA_SYSBUS_MODE_BLEN4  (1 << GMAC_DMA_SYSBUS_MODE_BLEN4_SHIFT)

#define DMA_CH0_TX_CONTROL_TXPBL_SHIFT GMAC_DMA_CH0_TX_CONTROL_TXPBL_SHIFT
#define DMA_CH0_TX_CONTROL_TXPBL_MASK  0x3f
#define DMA_CH0_TX_CONTROL_OSF         (1 << GMAC_DMA_CH0_TX_CONTROL_OSF_SHIFT)
#define DMA_CH0_TX_CONTROL_ST          (1 << GMAC_DMA_CH0_TX_CONTROL_ST_SHIFT)
#define DMA_CH0_CONTROL_PBLX8          (1 << GMAC_DMA_CH0_CONTROL_PBLX8_SHIFT)

#define DMA_CH0_RX_CONTROL_RXPBL_SHIFT GMAC_DMA_CH0_RX_CONTROL_RXPBL_SHIFT
#define DMA_CH0_RX_CONTROL_RXPBL_MASK  0x3f
#define DMA_CH0_RX_CONTROL_RBSZ_SHIFT  GMAC_DMA_CH0_RX_CONTROL_RBSZ_3_0_SHIFT
#define DMA_CH0_RX_CONTROL_RBSZ_MASK   0x3fff
#define DMA_CH0_RX_CONTROL_SR          (1 << GMAC_DMA_CH0_RX_CONTROL_SR_SHIFT)

#define DMA_CHAN_STATUS_REB       (0x7 << GMAC_DMA_CH0_STATUS_REB_SHIFT)
#define DMA_CHAN_STATUS_REB_SHIFT (GMAC_DMA_CH0_STATUS_REB_SHIFT)
#define DMA_CHAN_STATUS_TEB       (0x7 << GMAC_DMA_CH0_STATUS_TEB_SHIFT)
#define DMA_CHAN_STATUS_TEB_SHIFT (GMAC_DMA_CH0_STATUS_TEB_SHIFT)
#define DMA_CHAN_STATUS_NIS       (1 << GMAC_DMA_CH0_STATUS_NIS_SHIFT)
#define DMA_CHAN_STATUS_AIS       (1 << GMAC_DMA_CH0_STATUS_AIS_SHIFT)
#define DMA_CHAN_STATUS_CDE       (1 << GMAC_DMA_CH0_STATUS_CDE_SHIFT)
#define DMA_CHAN_STATUS_FBE       (1 << GMAC_DMA_CH0_STATUS_FBE_SHIFT)
#define DMA_CHAN_STATUS_ERI       (1 << GMAC_DMA_CH0_STATUS_ERI_SHIFT)
#define DMA_CHAN_STATUS_ETI       (1 << GMAC_DMA_CH0_STATUS_ETI_SHIFT)
#define DMA_CHAN_STATUS_RWT       (1 << GMAC_DMA_CH0_STATUS_RWT_SHIFT)
#define DMA_CHAN_STATUS_RPS       (1 << GMAC_DMA_CH0_STATUS_RPS_SHIFT)
#define DMA_CHAN_STATUS_RBU       (1 << GMAC_DMA_CH0_STATUS_RBU_SHIFT)
#define DMA_CHAN_STATUS_RI        (1 << GMAC_DMA_CH0_STATUS_RI_SHIFT)
#define DMA_CHAN_STATUS_TBU       (1 << GMAC_DMA_CH0_STATUS_TBU_SHIFT)
#define DMA_CHAN_STATUS_TPS       (1 << GMAC_DMA_CH0_STATUS_TPS_SHIFT)
#define DMA_CHAN_STATUS_TI        (1 << GMAC_DMA_CH0_STATUS_TI_SHIFT)

/* Interrupt enable bits per channel */
#define DMA_CHAN_INTR_ENA_NIE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_NIE_SHIFT)
#define DMA_CHAN_INTR_ENA_AIE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_AIE_SHIFT)
#define DMA_CHAN_INTR_ENA_CDE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_CDEE_SHIFT)
#define DMA_CHAN_INTR_ENA_FBE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_FBEE_SHIFT)
#define DMA_CHAN_INTR_ENA_ERE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_ERIE_SHIFT)
#define DMA_CHAN_INTR_ENA_ETE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_ETIE_SHIFT)
#define DMA_CHAN_INTR_ENA_RWE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_RWTE_SHIFT)
#define DMA_CHAN_INTR_ENA_RSE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_RSE_SHIFT)
#define DMA_CHAN_INTR_ENA_RBUE (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_RBUE_SHIFT)
#define DMA_CHAN_INTR_ENA_RIE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_RIE_SHIFT)
#define DMA_CHAN_INTR_ENA_TBUE (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_TBUE_SHIFT)
#define DMA_CHAN_INTR_ENA_TSE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_TXSE_SHIFT))
#define DMA_CHAN_INTR_ENA_TIE  (1 << GMAC_DMA_CH0_INTERRUPT_ENABLE_TIE_SHIFT)

#define DMA_CHAN_INTR_NORMAL (DMA_CHAN_INTR_ENA_NIE | \
                              DMA_CHAN_INTR_ENA_RIE | \
                              DMA_CHAN_INTR_ENA_TIE)

#define DMA_CHAN_INTR_ABNORMAL (DMA_CHAN_INTR_ENA_AIE | \
                                DMA_CHAN_INTR_ENA_FBE)
/* DMA default interrupt mask */
#define DMA_CHAN_INTR_DEFAULT_MASK (DMA_CHAN_INTR_NORMAL | \
                                    DMA_CHAN_INTR_ABNORMAL)

/* Description related */
#define GMAC_DESC3_OWN   (0x1 << 31)
#define GMAC_DESC3_IOC   (0x1 << 30)
#define GMAC_DESC3_FD    (0x1 << 29)
#define GMAC_DESC3_LD    (0x1 << 28)
#define GMAC_DESC3_BUF1V (0x1 << 24)

#define DES3_ERROR_SUMMARY (1 << 15)
#define DES3_ERROR_SUMMARY (1 << 15)

/* Generic MII registers. */
#define MII_BMCR        0x00    /* Basic mode control register */
#define MII_BMSR        0x01    /* Basic mode status register  */
#define MII_PHYSID1     0x02    /* PHYS ID 1                   */
#define MII_PHYSID2     0x03    /* PHYS ID 2                   */
#define MII_ADVERTISE   0x04    /* Advertisement control reg   */
#define MII_LPA         0x05    /* Link partner ability reg    */
#define MII_EXPANSION   0x06    /* Expansion register          */
#define MII_CTRL1000    0x09    /* 1000BASE-T control          */
#define MII_STAT1000    0x0a    /* 1000BASE-T status           */
#define MII_MMD_CTRL    0x0d    /* MMD Access Control Register */
#define MII_MMD_DATA    0x0e    /* MMD Access Data Register    */
#define MII_ESTATUS     0x0f    /* Extended Status             */
#define MII_DCOUNTER    0x12    /* Disconnect counter          */
#define MII_FCSCOUNTER  0x13    /* False carrier counter       */
#define MII_NWAYTEST    0x14    /* N-way auto-neg test reg     */
#define MII_RERRCOUNTER 0x15    /* Receive error counter       */
#define MII_SREVISION   0x16    /* Silicon revision            */
#define MII_RESV1       0x17    /* Reserved...                 */
#define MII_LBRERROR    0x18    /* Lpback, rx, bypass error    */
#define MII_PHYADDR     0x19    /* PHY address                 */
#define MII_RESV2       0x1a    /* Reserved...                 */
#define MII_TPISTATUS   0x1b    /* TPI status for 10mbps       */
#define MII_NCONFIG     0x1c    /* Network interface config    */

/* Basic mode control register. */
#define BMCR_RESV      x003f      /* Unused...                   */
#define BMCR_SPEED1000 0x0040     /* MSB of Speed (1000)         */
#define BMCR_CTST      0x0080     /* Collision test              */
#define BMCR_FULLDPLX  0x0100     /* Full duplex                 */
#define BMCR_ANRESTART 0x0200     /* Auto negotiation restart    */
#define BMCR_ISOLATE   0x0400     /* Isolate data paths from MII */
#define BMCR_PDOWN     0x0800     /* Enable low power state      */
#define BMCR_ANENABLE  0x1000     /* Enable auto negotiation     */
#define BMCR_SPEED100  0x2000     /* Select 100Mbps              */
#define BMCR_LOOPBACK  0x4000     /* TXD loopback bits           */
#define BMCR_RESET     0x8000     /* Reset to default state      */
#define BMCR_SPEED10   0x0000     /* Select 10Mbps               */

/* Basic mode status register. */
#define BMSR_ERCAP        0x0001  /* Ext-reg capability          */
#define BMSR_JCD          0x0002  /* Jabber detected             */
#define BMSR_LSTATUS      0x0004  /* Link status                 */
#define BMSR_ANEGCAPABLE  0x0008  /* Able to do auto-negotiation */
#define BMSR_RFAULT       0x0010  /* Remote fault detected       */
#define BMSR_ANEGCOMPLETE 0x0020  /* Auto-negotiation complete   */
#define BMSR_RESV         0x00c0  /* Unused...                   */
#define BMSR_ESTATEN      0x0100  /* Extended Status in R15      */
#define BMSR_100HALF2     0x0200  /* Can do 100BASE-T2 HDX       */
#define BMSR_100FULL2     0x0400  /* Can do 100BASE-T2 FDX       */
#define BMSR_10HALF       0x0800  /* Can do 10mbps, half-duplex  */
#define BMSR_10FULL       0x1000  /* Can do 10mbps, full-duplex  */
#define BMSR_100HALF      0x2000  /* Can do 100mbps, half-duplex */
#define BMSR_100FULL      0x4000  /* Can do 100mbps, full-duplex */
#define BMSR_100BASE4     0x8000  /* Can do 100mbps, 4k packets  */

/* Advertisement control register. */
#define ADVERTISE_SLCT          0x001f  /* Selector bits               */
#define ADVERTISE_CSMA          0x0001  /* Only selector supported     */
#define ADVERTISE_10HALF        0x0020  /* Try for 10mbps half-duplex  */
#define ADVERTISE_1000XFULL     0x0020  /* Try for 1000BASE-X full-duplex */
#define ADVERTISE_10FULL        0x0040  /* Try for 10mbps full-duplex  */
#define ADVERTISE_1000XHALF     0x0040  /* Try for 1000BASE-X half-duplex */
#define ADVERTISE_100HALF       0x0080  /* Try for 100mbps half-duplex */
#define ADVERTISE_1000XPAUSE    0x0080  /* Try for 1000BASE-X pause    */
#define ADVERTISE_100FULL       0x0100  /* Try for 100mbps full-duplex */
#define ADVERTISE_1000XPSE_ASYM 0x0100  /* Try for 1000BASE-X asym pause */
#define ADVERTISE_100BASE4      0x0200  /* Try for 100mbps 4k packets  */
#define ADVERTISE_PAUSE_CAP     0x0400  /* Try for pause               */
#define ADVERTISE_PAUSE_ASYM    0x0800  /* Try for asymetric pause     */
#define ADVERTISE_RESV          0x1000  /* Unused...                   */
#define ADVERTISE_RFAULT        0x2000  /* Say we can detect faults    */
#define ADVERTISE_LPACK         0x4000  /* Ack link partners response  */
#define ADVERTISE_NPAGE         0x8000  /* Next page bit               */

#define ADVERTISE_FULL (ADVERTISE_100FULL | ADVERTISE_10FULL | \
                        ADVERTISE_CSMA)
#define ADVERTISE_ALL  (ADVERTISE_10HALF | ADVERTISE_10FULL | \
                        ADVERTISE_100HALF | ADVERTISE_100FULL)

/* Link partner ability register. */
#define LPA_SLCT            0x001f    /* Same as advertise selector  */
#define LPA_10HALF          0x0020    /* Can do 10mbps half-duplex   */
#define LPA_1000XFULL       0x0020    /* Can do 1000BASE-X full-duplex */
#define LPA_10FULL          0x0040    /* Can do 10mbps full-duplex   */
#define LPA_1000XHALF       0x0040    /* Can do 1000BASE-X half-duplex */
#define LPA_100HALF         0x0080    /* Can do 100mbps half-duplex  */
#define LPA_1000XPAUSE      0x0080    /* Can do 1000BASE-X pause     */
#define LPA_100FULL         0x0100    /* Can do 100mbps full-duplex  */
#define LPA_1000XPAUSE_ASYM 0x0100    /* Can do 1000BASE-X pause asym*/
#define LPA_100BASE4        0x0200    /* Can do 100mbps 4k packets   */
#define LPA_PAUSE_CAP       0x0400    /* Can pause                   */
#define LPA_PAUSE_ASYM      0x0800    /* Can pause asymetrically     */
#define LPA_RESV            0x1000    /* Unused...                   */
#define LPA_RFAULT          0x2000    /* Link partner faulted        */
#define LPA_LPACK           0x4000    /* Link partner acked us       */
#define LPA_NPAGE           0x8000    /* Next page bit               */

#define LPA_DUPLEX (LPA_10FULL | LPA_100FULL)
#define LPA_100    (LPA_100FULL | LPA_100HALF | LPA_100BASE4)

/* Expansion register for auto-negotiation. */
#define EXPANSION_NWAY        0x0001    /* Can do N-way auto-nego      */
#define EXPANSION_LCWP        0x0002    /* Got new RX page code word   */
#define EXPANSION_ENABLENPAGE 0x0004    /* This enables npage words    */
#define EXPANSION_NPCAPABLE   0x0008    /* Link partner supports npage */
#define EXPANSION_MFAULTS     0x0010    /* Multiple faults detected    */
#define EXPANSION_RESV        0xffe0    /* Unused...                   */

#define ESTATUS_1000_XFULL 0x8000       /* Can do 1000BX Full */
#define ESTATUS_1000_XHALF 0x4000       /* Can do 1000BX Half */
#define ESTATUS_1000_TFULL 0x2000       /* Can do 1000BT Full          */
#define ESTATUS_1000_THALF 0x1000       /* Can do 1000BT Half          */

/* N-way test register. */
#define NWAYTEST_RESV1    0x00ff        /* Unused...                   */
#define NWAYTEST_LOOPBACK 0x0100        /* Enable loopback for N-way   */
#define NWAYTEST_RESV2    0xfe00        /* Unused...                   */

/* 1000BASE-T Control register */
#define ADVERTISE_1000FULL    0x0200  /* Advertise 1000BASE-T full duplex */
#define ADVERTISE_1000HALF    0x0100  /* Advertise 1000BASE-T half duplex */
#define CTL1000_AS_MASTER     0x0800
#define CTL1000_ENABLE_MASTER 0x1000

/* 1000BASE-T Status register */
#define LPA_1000LOCALRXOK 0x2000        /* Link partner local receiver status */
#define LPA_1000REMRXOK   0x1000        /* Link partner remote receiver status */
#define LPA_1000FULL      0x0800        /* Link partner 1000BASE-T full duplex */
#define LPA_1000HALF      0x0400        /* Link partner 1000BASE-T half duplex */

/* Indicates what features are advertised by the interface. */
#define ADVERTISED_10baseT_Half      (1 << 0)
#define ADVERTISED_10baseT_Full      (1 << 1)
#define ADVERTISED_100baseT_Half     (1 << 2)
#define ADVERTISED_100baseT_Full     (1 << 3)
#define ADVERTISED_1000baseT_Half    (1 << 4)
#define ADVERTISED_1000baseT_Full    (1 << 5)
#define ADVERTISED_Autoneg           (1 << 6)
#define ADVERTISED_TP                (1 << 7)
#define ADVERTISED_AUI               (1 << 8)
#define ADVERTISED_MII               (1 << 9)
#define ADVERTISED_FIBRE             (1 << 10)
#define ADVERTISED_BNC               (1 << 11)
#define ADVERTISED_10000baseT_Full   (1 << 12)
#define ADVERTISED_Pause             (1 << 13)
#define ADVERTISED_Asym_Pause        (1 << 14)
#define ADVERTISED_2500baseX_Full    (1 << 15)
#define ADVERTISED_Backplane         (1 << 16)
#define ADVERTISED_1000baseKX_Full   (1 << 17)
#define ADVERTISED_10000baseKX4_Full (1 << 18)
#define ADVERTISED_10000baseKR_Full  (1 << 19)
#define ADVERTISED_10000baseR_FEC    (1 << 20)
#define ADVERTISED_1000baseX_Half    (1 << 21)
#define ADVERTISED_1000baseX_Full    (1 << 22)

/* MII_LPA */
#define PHY_ANLPAR_PSB_802_3 0x0001
#define PHY_ANLPAR_PSB_802_9 0x0002

/* MII_CTRL1000 masks */
#define PHY_1000BTCR_1000FD 0x0200
#define PHY_1000BTCR_1000HD 0x0100

/* MII_STAT1000 masks */
#define PHY_1000BTSR_MSCF   0x8000
#define PHY_1000BTSR_MSCR   0x4000
#define PHY_1000BTSR_LRS    0x2000
#define PHY_1000BTSR_RRS    0x1000
#define PHY_1000BTSR_1000FD 0x0800
#define PHY_1000BTSR_1000HD 0x0400

/* PHY EXSR */
#define ESTATUS_1000XF 0x8000
#define ESTATUS_1000XH 0x4000

#define ETH_FCS_LEN 4 /* Octets in the FCS */

/********************* Private Structure Definition **************************/

/********************* Private Variable Definition ***************************/

/********************* Private Function Definition ***************************/

/**
  * @brief  Wait for Mdio to idle state.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
static HAL_Status Mdio_WaitIdle(struct GMAC_HANDLE *pGMAC)
{
    uint32_t tickstart = 0;

    /* Get tick */
    tickstart = HAL_GetTick();

    while ((READ_REG(pGMAC->pReg->MAC_MDIO_ADDRESS) & GMAC_MDIO_ADDRESS_GB) ==
           GMAC_MDIO_ADDRESS_GB) {
        /* Check for the Timeout */
        if ((HAL_GetTick() - tickstart) > GMAC_MDIO_TIMEOUT) {
            return HAL_TIMEOUT;
        }
    }

    return HAL_OK;
}

/**
  * @brief  reads the its ID by specified addr.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  addr: PHY address on the MII bus.
  * @param  phyID: where to store the ID retrieved.
  *
  * @return HAL status
  */
static HAL_Status PHY_GetID(struct GMAC_HANDLE *pGMAC, int32_t addr,
                            uint32_t *phyID)
{
    int32_t phyReg;

    /*
     * Grab the bits from PHYIR1, and put them
     * in the upper half.
     */
    phyReg = HAL_GMAC_MDIORead(pGMAC, addr, MII_PHYSID1);
    if (phyReg < 0) {
        return phyReg;
    }

    *phyID = (phyReg & 0xffff) << 16;

    /* Grab the bits from PHYIR2, and put them in the lower half */
    phyReg = HAL_GMAC_MDIORead(pGMAC, addr, MII_PHYSID2);

    if (phyReg < 0) {
        return phyReg;
    }

    *phyID |= (phyReg & 0xffff);

    return HAL_OK;
}

/**
  * @brief  Store the PHY status.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  config: pointer to a PHY_Config structure that contains
  *                 the configration for PHY.
  * @param  addr: PHY address on the MII bus.
  * @param  id: where to store the ID retrieved.
  *
  * @return HAL status
  */
static HAL_Status PHY_InfoCreate(struct GMAC_HANDLE *pGMAC,
                                 struct GMAC_PHY_Config *config,
                                 int32_t addr, uint32_t id)
{
    pGMAC->phyStatus.link = 0;
    pGMAC->phyStatus.duplex = config->duplexMode;
    pGMAC->phyStatus.interface = config->interface;
    pGMAC->phyStatus.neg = config->neg;
    pGMAC->phyStatus.speed = config->speed;
    pGMAC->phyStatus.maxSpeed = config->maxSpeed;

    pGMAC->phyStatus.addr = addr;
    pGMAC->phyStatus.phyID = id;

    return HAL_OK;
}

/**
  * @brief  Connect the PHY by address.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  config: pointer to a PHY_Config structure that contains
  *                 the configration for PHY.
  * @param  addr: PHY address on the MII bus.
  *
  * @return HAL status
  */
static HAL_Status PHY_Connect(struct GMAC_HANDLE *pGMAC,
                              struct GMAC_PHY_Config *config,
                              int32_t addr)
{
    HAL_Status status;
    uint32_t phyID = 0xffffffff;

    status = PHY_GetID(pGMAC, addr, &phyID);
    if (status == HAL_OK && (phyID & 0x1fffffff) != 0x1fffffff) {
        return PHY_InfoCreate(pGMAC, config, addr, phyID);
    }

    return HAL_NODEV;
}

/**
  * @brief  configures/forces speed/duplex from PHY status
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
static HAL_Status PHY_SetupForced(struct GMAC_HANDLE *pGMAC)
{
    int32_t ctl = BMCR_ANRESTART;

    if (PHY_SPEED_1000M == pGMAC->phyStatus.speed) {
        ctl |= BMCR_SPEED1000;
    } else if (PHY_SPEED_100M == pGMAC->phyStatus.speed) {
        ctl |= BMCR_SPEED100;
    }

    if (PHY_DUPLEX_FULL == pGMAC->phyStatus.duplex) {
        ctl |= BMCR_FULLDPLX;
    }

    return HAL_GMAC_MDIOWrite(pGMAC, pGMAC->phyStatus.addr, MII_BMCR, ctl);
}

/**
  * @brief  configures PHY advert features
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
static int32_t PHY_ConfigAdvert(struct GMAC_HANDLE *pGMAC)
{
    uint32_t adverTise;
    int32_t oldAdv, adv, bmsr;
    int32_t err, changed = 0;

    /* Only allow advertising what this PHY supports */
    pGMAC->phyStatus.advertising &= pGMAC->phyStatus.supported;
    adverTise = pGMAC->phyStatus.advertising;

    /* Setup standard advertisement */
    adv = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_ADVERTISE);
    if (adv < 0) {
        return adv;
    }

    oldAdv = adv;
    adv &= ~(ADVERTISE_ALL | ADVERTISE_100BASE4 | ADVERTISE_PAUSE_CAP |
             ADVERTISE_PAUSE_ASYM);
    if (adverTise & ADVERTISED_10baseT_Half) {
        adv |= ADVERTISE_10HALF;
    }
    if (adverTise & ADVERTISED_10baseT_Full) {
        adv |= ADVERTISE_10FULL;
    }
    if (adverTise & ADVERTISED_100baseT_Half) {
        adv |= ADVERTISE_100HALF;
    }
    if (adverTise & ADVERTISED_100baseT_Full) {
        adv |= ADVERTISE_100FULL;
    }
    if (adverTise & ADVERTISED_Pause) {
        adv |= ADVERTISE_PAUSE_CAP;
    }
    if (adverTise & ADVERTISED_Asym_Pause) {
        adv |= ADVERTISE_PAUSE_ASYM;
    }
    if (adverTise & ADVERTISED_1000baseX_Half) {
        adv |= ADVERTISE_1000XHALF;
    }
    if (adverTise & ADVERTISED_1000baseX_Full) {
        adv |= ADVERTISE_1000XFULL;
    }

    if (adv != oldAdv) {
        err = HAL_GMAC_MDIOWrite(pGMAC, pGMAC->phyStatus.addr,
                                 MII_ADVERTISE, adv);

        if (err < 0) {
            return err;
        }
        changed = 1;
    }

    bmsr = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMSR);
    if (bmsr < 0) {
        return bmsr;
    }

    /*
     * Per 802.3-2008, Section 22.2.4.2.16 Extended status all
     * 1000Mbits/sec capable PHYs shall have the BMSR_ESTATEN bit set to a
     * logical 1.
     */
    if (!(bmsr & BMSR_ESTATEN)) {
        return changed;
    }

    /* Configure gigabit if it's supported */
    adv = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_CTRL1000);
    if (adv < 0) {
        return adv;
    }

    oldAdv = adv;

    adv &= ~(ADVERTISE_1000FULL | ADVERTISE_1000HALF);

    if (pGMAC->phyStatus.supported & (HAL_GMAC_PHY_SUPPORTED_1000baseT_Half |
                                      HAL_GMAC_PHY_SUPPORTED_1000baseT_Full)) {
        if (adverTise & HAL_GMAC_PHY_SUPPORTED_1000baseT_Half) {
            adv |= ADVERTISE_1000HALF;
        }
        if (adverTise & HAL_GMAC_PHY_SUPPORTED_1000baseT_Full) {
            adv |= ADVERTISE_1000FULL;
        }
    }

    if (adv != oldAdv) {
        changed = 1;
    }

    err = HAL_GMAC_MDIOWrite(pGMAC, pGMAC->phyStatus.addr, MII_CTRL1000, adv);
    if (err < 0) {
        return err;
    }

    return changed;
}

/**
  * @brief  Enable and Restart auto-negotiation
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
static HAL_Status PHY_RestartAneg(struct GMAC_HANDLE *pGMAC)
{
    int32_t ctl;

    ctl = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMCR);
    if (ctl < 0) {
        return ctl;
    }

    ctl |= (BMCR_ANENABLE | BMCR_ANRESTART);
    /* Don't isolate the PHY if we're negotiating */
    ctl &= ~(BMCR_ISOLATE);
    ctl = HAL_GMAC_MDIOWrite(pGMAC, pGMAC->phyStatus.addr, MII_BMCR, ctl);

    return ctl;
}

/**
  * @brief  If auto-negotiation is enabled, we configure the
  *         advertising, and then restart auto-negotiation. If it is not
  *         enabled, then we write the BMCR.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
static HAL_Status PHY_ConfigAneg(struct GMAC_HANDLE *pGMAC)
{
    HAL_Status result;

    if (PHY_AUTONEG_ENABLE != pGMAC->phyStatus.neg) {
        return PHY_SetupForced(pGMAC);
    }

    result = PHY_ConfigAdvert(pGMAC);
    if (result < 0) { /* error */
        return result;
    }

    if (result == 0) {
        /*
         * Advertisment hasn't changed, but maybe aneg was never on to
         * begin with? Or maybe phy was isolated?
         */
        int32_t ctl = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMCR);
        if (ctl < 0) {
            return ctl;
        }

        if (!(ctl & BMCR_ANENABLE) || (ctl & BMCR_ISOLATE)) {
            result = 1; /* do restart aneg */
        }
    }

    /*
     * Only restart aneg if we are advertising something different
     * than we were before.
     */
    if (result > 0) {
        result = PHY_RestartAneg(pGMAC);
    }

    return result;
}

/**
  * @brief  Set PHY max speed if provied
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  max_speed: max speed for PHY supported.
  *
  * @return HAL status
  */
static HAL_Status PHY_Setsupported(struct GMAC_HANDLE *pGMAC, uint32_t maxSpeed)
{
    pGMAC->phyStatus.supported &= HAL_GMAC_PHY_DEFAULT_FEATURES;

    switch (maxSpeed) {
    default:

        return HAL_ERROR;
    case PHY_SPEED_1000M:
        pGMAC->phyStatus.supported |= HAL_GMAC_PHY_1000BT_FEATURES;
    /* fall through */
    case PHY_SPEED_100M:
        pGMAC->phyStatus.supported |= HAL_GMAC_PHY_100BT_FEATURES;
    /* fall through */
    case PHY_SPEED_10M:
        pGMAC->phyStatus.supported |= HAL_GMAC_PHY_10BT_FEATURES;
    }

    return HAL_OK;
}

/**
  * @brief  Configure PHY by features
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
static HAL_Status PHY_Config(struct GMAC_HANDLE *pGMAC)
{
    int32_t val = 0;
    uint32_t features;
    HAL_Status status;

    if (pGMAC->phyOps.config) {
        status = pGMAC->phyOps.config(pGMAC);
        if (status) {
            return status;
        }
    }

    features = (HAL_GMAC_PHY_SUPPORTED_TP | HAL_GMAC_PHY_SUPPORTED_MII |
                HAL_GMAC_PHY_SUPPORTED_AUI | HAL_GMAC_PHY_SUPPORTED_FIBRE |
                HAL_GMAC_PHY_SUPPORTED_BNC);

    /* Do we support autonegotiation? */
    val = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMSR);
    if (val < 0) {
        return val;
    }

    if (val & BMSR_ANEGCAPABLE) {
        features |= HAL_GMAC_PHY_SUPPORTED_Autoneg;
    }

    if (val & BMSR_100FULL) {
        features |= HAL_GMAC_PHY_SUPPORTED_100baseT_Full;
    }
    if (val & BMSR_100HALF) {
        features |= HAL_GMAC_PHY_SUPPORTED_100baseT_Half;
    }
    if (val & BMSR_10FULL) {
        features |= HAL_GMAC_PHY_SUPPORTED_10baseT_Full;
    }
    if (val & BMSR_10HALF) {
        features |= HAL_GMAC_PHY_SUPPORTED_10baseT_Half;
    }

    if (val & BMSR_ESTATEN) {
        val = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_ESTATUS);
        if (val < 0) {
            return val;
        }

        if (val & ESTATUS_1000_TFULL) {
            features |= HAL_GMAC_PHY_SUPPORTED_1000baseT_Full;
        }
        if (val & ESTATUS_1000_THALF) {
            features |= HAL_GMAC_PHY_SUPPORTED_1000baseT_Half;
        }
        if (val & ESTATUS_1000_XFULL) {
            features |= HAL_GMAC_PHY_SUPPORTED_1000baseX_Full;
        }
        if (val & ESTATUS_1000_XHALF) {
            features |= HAL_GMAC_PHY_SUPPORTED_1000baseX_Half;
        }
    }

    pGMAC->phyStatus.supported &= features;
    pGMAC->phyStatus.advertising &= features;

    PHY_ConfigAneg(pGMAC);

    return HAL_OK;
}

/**
  * @brief  Configure GMAC flow ctrl
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  duplex: full duplex or half.
  * @param  fc: desired flow ctrl.
  * @param  pauseTime: time for pause.
  *
  * @return HAL status
  */
static HAL_Status GMAC_FlowCtrl(struct GMAC_HANDLE *pGMAC, int32_t duplex,
                                uint32_t fc, uint32_t pauseTime)
{
    unsigned int flow = 0;

    HAL_DBG("GMAC Flow-Control:\n");
    if (fc & HAL_GMAC_FLOW_RX) {
        HAL_DBG("\tReceive Flow-Control ON\n");
        flow |= GMAC_RX_FLOW_CTRL_RFE;
    }
    WRITE_REG(pGMAC->pReg->MAC_RX_FLOW_CTRL, flow);

    if (fc & HAL_GMAC_FLOW_TX) {
        HAL_DBG("\tTransmit Flow-Control ON\n");

        if (duplex) {
            HAL_DBG("\tduplex mode: PAUSE %ld\n", pauseTime);
        }

        flow = GMAC_Q0_TX_FLOW_CTRL_TFE;
        if (duplex) {
            flow |= (pauseTime << GMAC_Q0_TX_FLOW_CTRL_PT_SHIFT);
        }

        WRITE_REG(pGMAC->pReg->MAC_Q0_TX_FLOW_CTRL, flow);
    } else {
        WRITE_REG(pGMAC->pReg->MAC_Q0_TX_FLOW_CTRL, 0);
    }

    return HAL_OK;
}

/**
  * @brief  Configure DMA RX operation mode
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  mode: operation mode
  * @param  fifosz: Rx fifo size.
  *
  * @return HAL status
  */
static HAL_Status GMAC_DMARXOpMode(struct GMAC_HANDLE *pGMAC, int32_t mode,
                                   int32_t fifosz)
{
    uint32_t rqs = fifosz / 256 - 1;
    uint32_t mtlRXOP, mtlRXInt;

    mtlRXOP = READ_REG(pGMAC->pReg->MTL_RXQ0_OPERATION_MODE);
    if (mode == SF_DMA_MODE) {
        HAL_DBG("GMAC: enable RX store and forward mode\n");
        mtlRXOP |= MTL_RXQ0_OPERATION_MODE_RSF;
    } else {
        HAL_DBG("GMAC: disable RX SF mode (threshold %ld)\n", mode);
        mtlRXOP &= ~MTL_RXQ0_OPERATION_MODE_RSF;
        mtlRXOP &= MTL_OP_MODE_RTC_MASK;
        if (mode <= 32) {
            mtlRXOP |= MTL_OP_MODE_RTC_32;
        } else if (mode <= 64) {
            mtlRXOP |= MTL_OP_MODE_RTC_64;
        } else if (mode <= 96) {
            mtlRXOP |= MTL_OP_MODE_RTC_96;
        } else {
            mtlRXOP |= MTL_OP_MODE_RTC_128;
        }
    }

    mtlRXOP &= ~MTL_OP_MODE_RQS_MASK;
    mtlRXOP |= rqs << MTL_OP_MODE_RQS_SHIFT;

    /*
     * Enable flow control only if each channel gets 4 KiB or more FIFO and
     * only if channel is not an AVB channel.
     */
    if (fifosz >= 4096) {
        uint32_t rfd, rfa;

        mtlRXOP |= MTL_RXQ0_OPERATION_MODE_EHFC;

        /*
         * Set Threshold for Activating Flow Control to min 2 frames,
         * i.e. 1500 * 2 = 3000 bytes.
         *
         * Set Threshold for Deactivating Flow Control to min 1 frame,
         * i.e. 1500 bytes.
         */
        switch (fifosz) {
        case 4096:
            /*
             * This violates the above formula because of FIFO size
             * limit therefore overflow may occur in spite of this.
             */
            rfd = 0x03; /* Full-2.5K */
            rfa = 0x01; /* Full-1.5K */
            break;

        case 8192:
            rfd = 0x06; /* Full-4K */
            rfa = 0x0a; /* Full-6K */
            break;

        case 16384:
            rfd = 0x06; /* Full-4K */
            rfa = 0x12; /* Full-10K */
            break;

        default:
            rfd = 0x06; /* Full-4K */
            rfa = 0x1e; /* Full-16K */
            break;
        }

        mtlRXOP &= ~MTL_OP_MODE_RFD_MASK;
        mtlRXOP |= rfd << MTL_OP_MODE_RFD_SHIFT;

        mtlRXOP &= ~MTL_OP_MODE_RFA_MASK;
        mtlRXOP |= rfa << MTL_OP_MODE_RFA_SHIFT;
    }

    WRITE_REG(pGMAC->pReg->MTL_RXQ0_OPERATION_MODE, mtlRXOP);

    /* Enable MTL RX overflow */
    mtlRXInt = READ_REG(pGMAC->pReg->MTL_Q0_INTERRUPT_CTRL_STATUS);
    WRITE_REG(pGMAC->pReg->MTL_Q0_INTERRUPT_CTRL_STATUS, mtlRXInt |
              MTL_RX_OVERFLOW_INT_EN);

    return HAL_OK;
}

/**
  * @brief  Configure DMA TX operation mode
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  mode: operation mode
  * @param  fifosz: Tx fifo size.
  *
  * @return HAL status
  */
static HAL_Status GMAC_DMATXOpMode(struct GMAC_HANDLE *pGMAC, int32_t mode,
                                   int32_t fifosz)
{
    uint32_t tqs = fifosz / 256 - 1;
    uint32_t mtlTXOP;

    mtlTXOP = READ_REG(pGMAC->pReg->MTL_TXQ0_OPERATION_MODE);

    if (mode == SF_DMA_MODE) {
        HAL_DBG("GMAC: enable TX store and forward mode\n");
        /* Transmit COE type 2 cannot be done in cut-through mode. */
        mtlTXOP |= MTL_OP_MODE_TSF;
    } else {
        HAL_DBG("GMAC: disabling TX SF (threshold %ld)\n", mode);
        mtlTXOP &= ~MTL_OP_MODE_TSF;
        mtlTXOP &= MTL_OP_MODE_TTC_MASK;
        /* Set the transmit threshold */
        if (mode <= 32) {
            mtlTXOP |= MTL_OP_MODE_TTC_32;
        } else if (mode <= 64) {
            mtlTXOP |= MTL_OP_MODE_TTC_64;
        } else if (mode <= 96) {
            mtlTXOP |= MTL_OP_MODE_TTC_96;
        } else if (mode <= 128) {
            mtlTXOP |= MTL_OP_MODE_TTC_128;
        } else if (mode <= 192) {
            mtlTXOP |= MTL_OP_MODE_TTC_192;
        } else if (mode <= 256) {
            mtlTXOP |= MTL_OP_MODE_TTC_256;
        } else if (mode <= 384) {
            mtlTXOP |= MTL_OP_MODE_TTC_384;
        } else {
            mtlTXOP |= MTL_OP_MODE_TTC_512;
        }
    }
    /*
     * For an IP with DWC_EQOS_NUM_TXQ == 1, the fields TXQEN and TQS are RO
     * with reset values: TXQEN on, TQS == DWC_EQOS_TXFIFO_SIZE.
     * For an IP with DWC_EQOS_NUM_TXQ > 1, the fields TXQEN and TQS are R/W
     * with reset values: TXQEN off, TQS 256 bytes.
     *
     * TXQEN must be written for multi-channel operation and TQS must
     * reflect the available fifo size per queue (total fifo size / number
     * of enabled queues).
     */
    mtlTXOP &= ~MTL_OP_MODE_TXQEN_MASK;
    mtlTXOP |= MTL_OP_MODE_TXQEN;

    mtlTXOP &= ~MTL_OP_MODE_TQS_MASK;
    mtlTXOP |= tqs << MTL_OP_MODE_TQS_SHIFT;

    WRITE_REG(pGMAC->pReg->MTL_TXQ0_OPERATION_MODE, mtlTXOP);

    return HAL_OK;
}

/** @} */

/********************* Public Function Definition ****************************/

/** @defgroup GMAC_Exported_Functions_Group3 IO Functions

 This section provides functions allowing to IO controlling:

 *  @{
 */

/**
  * @brief  Read MDIO bus data
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  mdioAddr: PHY address.
  * @param  mdioReg: register value.
  *
  * @return readen data
  */
int32_t HAL_GMAC_MDIORead(struct GMAC_HANDLE *pGMAC, int32_t mdioAddr,
                          int32_t mdioReg)
{
    HAL_Status status;
    int32_t val;

    HAL_ASSERT(pGMAC != NULL);

    HAL_DBG("Mdio Read addr=%ld, reg=%ld\n", mdioAddr, mdioReg);
    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO not idle at entry");

        return status;
    }

    val = READ_REG(pGMAC->pReg->MAC_MDIO_ADDRESS);
    val &= GMAC_MDIO_ADDRESS_SKAP |
           GMAC_MDIO_ADDRESS_C45E;
    val |= (mdioAddr << GMAC_MDIO_ADDRESS_PA_SHIFT) |
           (mdioReg << GMAC_MDIO_ADDRESS_RDA_SHIFT) |
           (pGMAC->clkCSR <<
            GMAC_MDIO_ADDRESS_CR_SHIFT) |
           GMAC_MDIO_ADDRESS_GOC_READ |
           GMAC_MDIO_ADDRESS_GB;
    WRITE_REG(pGMAC->pReg->MAC_MDIO_ADDRESS, val);

    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO read didn't complete");

        return status;
    }

    val = READ_REG(pGMAC->pReg->MAC_MDIO_DATA);
    val &= GMAC_MDIO_DATA_GD_MASK;

    return val;
}

/**
  * @brief  Write MDIO bus data
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  mdioAddr: PHY address.
  * @param  mdioReg: register value.
  * @param  mdioVal: MDIO value be written.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_MDIOWrite(struct GMAC_HANDLE *pGMAC, int32_t mdioAddr,
                              int32_t mdioReg, uint16_t mdioVal)
{
    HAL_Status status;
    uint32_t val;

    HAL_ASSERT(pGMAC != NULL);

    HAL_DBG("%s(addr=%lx, reg=%ld, val=%x):\n", __func__,
            mdioAddr, mdioReg, mdioVal);
    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO not idle at entry");

        return status;
    }

    WRITE_REG(pGMAC->pReg->MAC_MDIO_DATA, mdioVal);
    val = READ_REG(pGMAC->pReg->MAC_MDIO_ADDRESS);
    val &= GMAC_MDIO_ADDRESS_SKAP |
           GMAC_MDIO_ADDRESS_C45E;
    val |= (mdioAddr << GMAC_MDIO_ADDRESS_PA_SHIFT) |
           (mdioReg << GMAC_MDIO_ADDRESS_RDA_SHIFT) |
           (pGMAC->clkCSR <<
            GMAC_MDIO_ADDRESS_CR_SHIFT) |
           GMAC_MDIO_ADDRESS_GOC_WRITE |
           GMAC_MDIO_ADDRESS_GB;

    WRITE_REG(pGMAC->pReg->MAC_MDIO_ADDRESS, val);

    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO read didn't complete");

        return status;
    }

    return HAL_OK;
}

/**
  * @brief  Init PHY by config to connect PHY
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  config: pointer to a PHY_Config structure that contains
  *                 the configuration.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_PHYInit(struct GMAC_HANDLE *pGMAC, struct GMAC_PHY_Config *config)
{
    int32_t i, found = 0, timeout = 500;
    HAL_Status status;
    int32_t reg;

    HAL_ASSERT(pGMAC != NULL);

    if (!config->features) {
        config->features = HAL_GMAC_PHY_GBIT_FEATURES | HAL_GMAC_PHY_SUPPORTED_MII |
                           HAL_GMAC_PHY_SUPPORTED_AUI | HAL_GMAC_PHY_SUPPORTED_FIBRE |
                           HAL_GMAC_PHY_SUPPORTED_BNC;
    }

    pGMAC->phyStatus.advertising = pGMAC->phyStatus.supported = config->features;

    /* softreset */
    if (HAL_GMAC_MDIOWrite(pGMAC, config->phyAddress, MII_BMCR, BMCR_RESET) < 0) {
        HAL_DBG("PHY reset failed\n");

        return HAL_ERROR;
    }

    reg = HAL_GMAC_MDIORead(pGMAC, config->phyAddress, MII_BMCR);
    while ((reg & BMCR_RESET) && timeout--) {
        reg = HAL_GMAC_MDIORead(pGMAC, config->phyAddress, MII_BMCR);
        if (reg < 0) {
            HAL_DBG("PHY status read failed\n");

            return HAL_ERROR;
        }
        HAL_DelayUs(1000);
    }

    if (reg & BMCR_RESET) {
        HAL_DBG("PHY reset timed out\n");

        return HAL_TIMEOUT;
    }

    HAL_DelayUs(100000);

    if (config->phyAddress < 0) {
        for (i = 0; i < 16; i++) {
            status = PHY_Connect(pGMAC, config, i);
            if (!status) {
                found++;
                break;
            }
        }
    } else {
        /* Use PHY addr to connect PHY */
        status = PHY_Connect(pGMAC, config, config->phyAddress);
        if (!status) {
            found = 1;
        }
    }

    if (!found) {
        HAL_DBG("not PHY found\n");

        return HAL_NODEV;
    } else {
        HAL_DBG("PHY found ID: 0x%lx\n", pGMAC->phyStatus.phyID);
    }

    if (pGMAC->phyOps.init) {
        status = pGMAC->phyOps.init(pGMAC);
    }

    return status;
}

/**
  * @brief  update link status up/down from PHY
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_PHYUpdateLink(struct GMAC_HANDLE *pGMAC)
{
    int32_t reg;

    HAL_ASSERT(pGMAC != NULL);

    /*
     * Wait if the link is up, and autonegotiation is in progress
     * (ie - we're capable and it's not done)
     */
    reg = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMSR);
    if (reg < 0) {
        return reg;
    }

    /*
     * If we already saw the link up, and it hasn't gone down, then
     * we don't need to wait for autoneg again
     */
    if (pGMAC->phyStatus.link && reg & BMSR_LSTATUS) {
        return HAL_OK;
    }

    reg = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMSR);
    if (reg & BMSR_LSTATUS) {
        pGMAC->phyStatus.link = 1;
    } else {
        pGMAC->phyStatus.link = 0;
    }

    return HAL_OK;
}

/**
  * @brief  Updates the speed and duplex. If autonegotiation is
  *         enabled, it uses the AND of the link partner's advertised
  *         capabilities and our advertised capabilities. If autonegotiation
  *         is disabled, we use the appropriate bits in the control register.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_PHYParseLink(struct GMAC_HANDLE *pGMAC)
{
    int32_t reg = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMSR);

    HAL_ASSERT(pGMAC != NULL);

    /* We're using autonegotiation */
    if (pGMAC->phyStatus.neg == PHY_AUTONEG_ENABLE) {
        uint32_t lpa = 0, estatus = 0;
        int32_t gblpa = 0;

        /* Check for gigabit capability */
        if (pGMAC->phyStatus.supported & (HAL_GMAC_PHY_SUPPORTED_1000baseT_Full |
                                          HAL_GMAC_PHY_SUPPORTED_1000baseT_Half)) {
            /*
             * We want a list of states supported by
             * both PHYs in the link
             */
            gblpa = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_STAT1000);
            if (gblpa < 0) {
                HAL_DBG("Could not read MII_STAT1000. Ignoring gigabit capability\n");
                gblpa = 0;
            }
            gblpa &= HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_CTRL1000) << 2;
        }

        /*
         * Set the baseline so we only have to set them
         * if they're different
         */
        pGMAC->phyStatus.speed = PHY_SPEED_10M;
        pGMAC->phyStatus.duplex = PHY_DUPLEX_HALF;

        /* Check the gigabit fields */
        if (gblpa & (PHY_1000BTSR_1000FD | PHY_1000BTSR_1000HD)) {
            pGMAC->phyStatus.speed = PHY_SPEED_1000M;

            if (gblpa & PHY_1000BTSR_1000FD) {
                pGMAC->phyStatus.duplex = PHY_DUPLEX_FULL;
            }

            /* We're done! */
            return HAL_OK;
        }

        lpa = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_ADVERTISE);
        lpa &= HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_LPA);

        if (lpa & (LPA_100FULL | LPA_100HALF)) {
            pGMAC->phyStatus.speed = PHY_SPEED_100M;

            if (lpa & LPA_100FULL) {
                pGMAC->phyStatus.duplex = PHY_DUPLEX_FULL;
            }
        } else if (lpa & LPA_10FULL) {
            pGMAC->phyStatus.duplex = PHY_DUPLEX_FULL;
        }

        /*
         * Extended status may indicate that the PHY supports
         * 1000BASE-T/X even though the 1000BASE-T registers
         * are missing. In this case we can't tell whether the
         * peer also supports it, so we only check extended
         * status if the 1000BASE-T registers are actually
         * missing.
         */
        if ((reg & BMSR_ESTATEN) && !(reg & BMSR_ERCAP)) {
            estatus = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_ESTATUS);
        }

        if (estatus & (ESTATUS_1000_XFULL | ESTATUS_1000_XHALF |
                       ESTATUS_1000_TFULL | ESTATUS_1000_THALF)) {
            pGMAC->phyStatus.speed = PHY_SPEED_1000M;
            if (estatus & (ESTATUS_1000_XFULL | ESTATUS_1000_TFULL)) {
                pGMAC->phyStatus.duplex = PHY_DUPLEX_FULL;
            }
        }

        if (pGMAC->phyStatus.duplex == PHY_DUPLEX_FULL) {
            pGMAC->phyStatus.pause = lpa & LPA_PAUSE_CAP ? 1 : 0;
        }
    } else {
        uint32_t bmcr = HAL_GMAC_MDIORead(pGMAC, pGMAC->phyStatus.addr, MII_BMCR);

        pGMAC->phyStatus.speed = PHY_SPEED_10M;
        pGMAC->phyStatus.duplex = PHY_DUPLEX_HALF;

        if (bmcr & BMCR_FULLDPLX) {
            pGMAC->phyStatus.duplex = PHY_DUPLEX_FULL;
        }

        if (bmcr & BMCR_SPEED1000) {
            pGMAC->phyStatus.speed = PHY_SPEED_1000M;
        } else if (bmcr & BMCR_SPEED100) {
            pGMAC->phyStatus.speed = PHY_SPEED_100M;
        }

        pGMAC->phyStatus.pause = 0;
    }

    return HAL_OK;
}

/**
  * @brief  Start the PHY after configuration
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_PHYStartup(struct GMAC_HANDLE *pGMAC)
{
    HAL_Status status;

    HAL_ASSERT(pGMAC != NULL);

    if (pGMAC->phyStatus.maxSpeed) {
        status = PHY_Setsupported(pGMAC, pGMAC->phyStatus.maxSpeed);
        if (status) {
            HAL_DBG_ERR("phy_set_supported() failed: %d", status);

            return status;
        }
    }

    status = PHY_Config(pGMAC);
    if (status) {
        HAL_DBG("PHY_ConfigAneg failed");

        return status;
    }

    if (pGMAC->phyOps.startup) {
        status = pGMAC->phyOps.startup(pGMAC);
        if (status) {
            return status;
        }
    }

    return HAL_OK;
}

/**
  * @brief  Enable DMA IRQ
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  */
void HAL_GMAC_EnableDmaIRQ(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);
    WRITE_REG(pGMAC->pReg->DMA_CH0_INTERRUPT_ENABLE,
              DMA_CHAN_INTR_DEFAULT_MASK);
}

/**
  * @brief  Disable DMA IRQ
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  */
void HAL_GMAC_DisableDmaIRQ(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);
    WRITE_REG(pGMAC->pReg->DMA_CH0_INTERRUPT_ENABLE, 0);
}

/**
  * @brief  Initializes the DMA Tx descriptors in chain mode.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  txDescs: DMATxDesc Pointer to the first Tx desc list
  * @param  txBuff: TxBuff Pointer to the first TxBuffer list
  * @param  count: Number of the used Tx desc in the list
  *
  * @retval HAL status
  */
HAL_Status HAL_GMAC_DMATxDescInit(struct GMAC_HANDLE *pGMAC,
                                  struct GMAC_Desc *txDescs,
                                  uint8_t *txBuff, uint32_t count)
{
    struct GMAC_Desc *desc;
    uint32_t i = 0;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(txDescs != NULL);
    HAL_ASSERT(txBuff != NULL);

    pGMAC->txDescIdx = 0;

    pGMAC->txDescs = txDescs;
    pGMAC->txBuf = txBuff;
    pGMAC->txSize = count;

    /* Fill each DMATxDesc descriptor with the right values */
    for (i = 0; i < count; i++) {
        /* Get the pointer on the ith member of the Tx Desc list */
        desc = txDescs + i;

        desc->des0 = 0;
        desc->des1 = 0;
        desc->des2 = 0;
        desc->des3 = 0;
    }

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Initializes the DMA Rx descriptors in chain mode.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  rxDescs: DMATxDesc Pointer to the first Rx desc list
  * @param  rxBuff: RxBuff Pointer to the first RxBuffer list
  * @param  count: Number of the used Rx desc in the list
  *
  * @retval HAL status
  */
HAL_Status HAL_GMAC_DMARxDescInit(struct GMAC_HANDLE *pGMAC,
                                  struct GMAC_Desc *rxDescs,
                                  uint8_t *rxBuff, uint32_t count)
{
    struct GMAC_Desc *desc;
    uint32_t i = 0;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(rxDescs != NULL);
    HAL_ASSERT(rxBuff != NULL);

    pGMAC->rxDescIdx = 0;

    pGMAC->rxDescs = rxDescs;
    pGMAC->rxBuf = rxBuff;
    pGMAC->rxSize = count;

    /* Fill each DMARxDesc descriptor with the right values */
    for (i = 0; i < count; i++) {
        /* Get the pointer on the ith member of the Rx Desc list */
        desc = rxDescs + i;

        desc->des0 = (uint32_t)(rxBuff + i * HAL_GMAC_MAX_PACKET_SIZE);
        desc->des1 = 0;
        desc->des2 = 0;
        desc->des3 = GMAC_DESC3_OWN | GMAC_DESC3_BUF1V | GMAC_DESC3_IOC;
    }

    /* Return function status */
    return HAL_OK;
}

/**
  * @brief  Handle GMAC interrupt for transfer.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return IRQ status
  */
eGMAC_IRQ_Status HAL_GMAC_IRQHandler(struct GMAC_HANDLE *pGMAC)
{
    eGMAC_IRQ_Status status = DMA_UNKNOWN;
    uint32_t intrStatus;

    HAL_ASSERT(pGMAC != NULL);

    intrStatus = READ_REG(pGMAC->pReg->DMA_CH0_STATUS);
    /* ABNORMAL interrupts */
    if (intrStatus & DMA_CHAN_STATUS_AIS) {
        if (intrStatus & DMA_CHAN_STATUS_RBU) {
            pGMAC->extraStatus.rxBufUnavIRQ++;
            status = DMA_RX_ERROR;
        }
        if (intrStatus & DMA_CHAN_STATUS_RPS) {
            pGMAC->extraStatus.rxProcessStoppedIRQ++;
            status = DMA_RX_ERROR;
        }
        if (intrStatus & DMA_CHAN_STATUS_RWT) {
            pGMAC->extraStatus.rxWatchdogIRQ++;
            status = DMA_RX_ERROR;
        }
        if (intrStatus & DMA_CHAN_STATUS_ETI) {
            pGMAC->extraStatus.txEarlyIRQ++;
            status = DMA_TX_ERROR;
        }
        if (intrStatus & DMA_CHAN_STATUS_TPS) {
            pGMAC->extraStatus.rxProcessStoppedIRQ++;
            status = DMA_TX_ERROR;
        }
        if (intrStatus & DMA_CHAN_STATUS_FBE) {
            pGMAC->extraStatus.fatalBusErrorIRQ++;
            status = DMA_TX_ERROR;
        }
    }

    /* TX/RX NORMAL interrupts */
    if (intrStatus & DMA_CHAN_STATUS_NIS) {
        pGMAC->extraStatus.normalIRQN++;
        if (intrStatus & DMA_CHAN_STATUS_RI) {
            uint32_t value;

            value = READ_REG(pGMAC->pReg->DMA_CH0_INTERRUPT_ENABLE);
            if (value & DMA_CHAN_INTR_ENA_RIE) {
                pGMAC->extraStatus.rxNormalIRQN++;
                status |= DMA_HANLE_RX;
            }
        }
        if (intrStatus & DMA_CHAN_STATUS_TI) {
            pGMAC->extraStatus.txNormallIRQN++;
            status |= DMA_HANLE_TX;
        }
        if (intrStatus & DMA_CHAN_STATUS_ERI) {
            pGMAC->extraStatus.rxEarlyIRQ++;
        }
    }

    /*
     * Clear the interrupt by writing a logic 1 to the chanX interrupt
     * status [21-0] expect reserved bits [5-3]
     */
    WRITE_REG(pGMAC->pReg->DMA_CH0_STATUS, intrStatus & 0x3fffc7);

    return status;
}

/**
  * @brief  Adjust timing register by link status.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *               the information for GMAC module.
  * @param  txDelay: RGMII tx delayline, only available for RGMII, set to 0 if RMII.
  * @param  rxDelay: RGMII rx delayline, only available for RGMII, set to 0 if RMII.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_AdjustLink(struct GMAC_HANDLE *pGMAC, int32_t txDelay,
                               int32_t rxDelay)
{
    uint32_t ctrl;

    HAL_ASSERT(pGMAC != NULL);

    ctrl = READ_REG(pGMAC->pReg->MAC_CONFIGURATION);
    ctrl |= GMAC_CORE_INIT;

    /* Flow Control operation */
    if (pGMAC->phyStatus.pause) {
        GMAC_FlowCtrl(pGMAC, pGMAC->phyStatus.duplex, HAL_GMAC_FLOW_AUTO,
                      HAL_PAUSE_TIME);
    }

    if (!pGMAC->phyStatus.duplex) {
        ctrl &= ~pGMAC->link.duplex;
    } else {
        ctrl |= pGMAC->link.duplex;
    }

    ctrl &= ~pGMAC->link.speedMask;
    switch (pGMAC->phyStatus.speed) {
    case PHY_SPEED_1000M:
        ctrl |= pGMAC->link.speed1000;
        break;
    case PHY_SPEED_100M:
        ctrl |= pGMAC->link.speed100;
        break;
    case PHY_SPEED_10M:
        ctrl |= pGMAC->link.speed10;
        break;
    default:
        HAL_DBG("broken speed: %d\n", pGMAC->phyStatus.speed);

        return HAL_ERROR;
    }

    switch (pGMAC->phyStatus.interface) {
    case PHY_INTERFACE_MODE_RMII:
        HAL_GMAC_SetToRMII(pGMAC);
        HAL_GMAC_SetRMIISpeed(pGMAC, pGMAC->phyStatus.speed);
        break;
    case PHY_INTERFACE_MODE_RGMII:
        HAL_GMAC_SetToRGMII(pGMAC, txDelay, rxDelay);
        HAL_GMAC_SetRGMIISpeed(pGMAC, pGMAC->phyStatus.speed);
        break;
    default:
        HAL_DBG("broken phy interface: %d\n", pGMAC->phyStatus.interface);

        return HAL_ERROR;
    }

    ctrl |= GMAC_CONFIG_TE;
    WRITE_REG(pGMAC->pReg->MAC_CONFIGURATION, ctrl);

    return HAL_OK;
}

/**
  * @brief  Start the GMAC work after configuration.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  addr: MAC address.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_Start(struct GMAC_HANDLE *pGMAC, uint8_t *addr)
{
    uint32_t mmc_mode = MMC_CNTRL_RESET_ON_READ | MMC_CNTRL_COUNTER_RESET |
                        MMC_CNTRL_PRESET | MMC_CNTRL_FULL_HALF_PRESET;
    uint32_t mode = SF_DMA_MODE;
    uint32_t value;
    int32_t limit = 10;
    uint32_t hwCap;
    int32_t rxFifosz;
    int32_t txFifosz;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(addr != NULL);

    /* GMAC Software Reset */
    value = READ_REG(pGMAC->pReg->DMA_MODE);
    WRITE_REG(pGMAC->pReg->DMA_MODE, value | DMA_MODE_SWR);
    /* Wait for software Reset */
    while (limit--) {
        if (!(READ_REG(pGMAC->pReg->DMA_MODE) & DMA_MODE_SWR)) {
            break;
        }
        HAL_DelayMs(10);
    }
    if (limit <= 0) {
        HAL_DBG("DMA_MODE_SWR stuck\n");

        return HAL_TIMEOUT;
    }

    HAL_DelayMs(100);

    /* DMA init */
    WRITE_REG(pGMAC->pReg->DMA_SYSBUS_MODE, DMA_SYSBUS_MODE_BLEN16 |
              DMA_SYSBUS_MODE_BLEN8 | DMA_SYSBUS_MODE_BLEN4);

    /* Mask interrupts by writing to CSR7 */
    WRITE_REG(pGMAC->pReg->DMA_CH0_INTERRUPT_ENABLE, DMA_CHAN_INTR_DEFAULT_MASK);

    hwCap = READ_REG(pGMAC->pReg->MAC_HW_FEATURE1);
    /* Set the HW DMA mode and the COE */
    txFifosz = 128 << ((hwCap & GMAC_HW_TXFIFOSIZE) >> GMAC_HW_TXFIFOSIZE_SHIFT);
    rxFifosz = 128 << ((hwCap & GMAC_HW_RXFIFOSIZE) >> GMAC_HW_RXFIFOSIZE_SHIFT);

    /* init rx chan */
    value = READ_REG(pGMAC->pReg->DMA_CH0_RX_CONTROL);
    value &= ~DMA_CH0_RX_CONTROL_RBSZ_MASK;
    value |= (rxFifosz << DMA_CH0_RX_CONTROL_RBSZ_SHIFT) & DMA_CH0_RX_CONTROL_RBSZ_MASK;
    value = value | (8 << DMA_CH0_RX_CONTROL_RXPBL_SHIFT);
    WRITE_REG(pGMAC->pReg->DMA_CH0_RX_CONTROL, value);
    WRITE_REG(pGMAC->pReg->DMA_CH0_RXDESC_LIST_ADDRESS, (uint32_t)pGMAC->rxDescs);
    WRITE_REG(pGMAC->pReg->DMA_CH0_RXDESC_TAIL_POINTER,
              (uint32_t)(pGMAC->rxDescs + pGMAC->rxSize));

    /* init tx chan */
    value = READ_REG(pGMAC->pReg->DMA_CH0_TX_CONTROL);
    value = value | (8 << DMA_CH0_TX_CONTROL_TXPBL_SHIFT);
    value |= DMA_CH0_TX_CONTROL_OSF;
    WRITE_REG(pGMAC->pReg->DMA_CH0_TX_CONTROL, value);

    WRITE_REG(pGMAC->pReg->DMA_CH0_TXDESC_LIST_ADDRESS, (uint32_t)pGMAC->txDescs);
    WRITE_REG(pGMAC->pReg->DMA_CH0_TXDESC_TAIL_POINTER, (uint32_t)pGMAC->txDescs);

    HAL_GMAC_WriteHWAddr(pGMAC, addr);

    /* core init */
    value = READ_REG(pGMAC->pReg->MAC_CONFIGURATION);
    value |= GMAC_CORE_INIT;
    WRITE_REG(pGMAC->pReg->MAC_CONFIGURATION, value);

    /* enable ipc */
    value = READ_REG(pGMAC->pReg->MAC_CONFIGURATION);
    value |= GMAC_CONFIG_IPC;
    WRITE_REG(pGMAC->pReg->MAC_CONFIGURATION, value);

    /* Enable the GMAC Rx/Tx */
    value = READ_REG(pGMAC->pReg->MAC_CONFIGURATION);
    value |= GMAC_CONFIG_TE | GMAC_CONFIG_RE;
    WRITE_REG(pGMAC->pReg->MAC_CONFIGURATION, value);

    GMAC_DMARXOpMode(pGMAC, mode, rxFifosz);
    GMAC_DMATXOpMode(pGMAC, mode, txFifosz);

    WRITE_REG(pGMAC->pReg->MMC_RX_INTERRUPT_MASK, MMC_DEFAULT_MASK);
    WRITE_REG(pGMAC->pReg->MMC_TX_INTERRUPT_MASK, MMC_DEFAULT_MASK);
    WRITE_REG(pGMAC->pReg->MMC_IPC_RX_INTERRUPT_MASK, MMC_DEFAULT_MASK);
    value = READ_REG(pGMAC->pReg->MMC_CONTROL);
    value |= (mmc_mode & 0x3F);
    WRITE_REG(pGMAC->pReg->MMC_CONTROL, value);

    HAL_ASSERT(pGMAC->txSize > 0);
    HAL_ASSERT(pGMAC->rxSize > 0);

    /* Set TX and RX rings length */
    WRITE_REG(pGMAC->pReg->DMA_CH0_TXDESC_RING_LENGTH, pGMAC->txSize - 1);
    WRITE_REG(pGMAC->pReg->DMA_CH0_RXDESC_RING_LENGTH, pGMAC->rxSize - 1);

    HAL_GMAC_EnableDmaIRQ(pGMAC);

    value = READ_REG(pGMAC->pReg->DMA_CH0_TX_CONTROL);
    value |= DMA_CH0_TX_CONTROL_ST;
    WRITE_REG(pGMAC->pReg->DMA_CH0_TX_CONTROL, value);

    value = READ_REG(pGMAC->pReg->DMA_CH0_RX_CONTROL);
    value |= DMA_CH0_RX_CONTROL_SR;
    WRITE_REG(pGMAC->pReg->DMA_CH0_RX_CONTROL, value);

    return HAL_OK;
}

/**
  * @brief  Stop GMAC work.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *               the information for GMAC module.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_Stop(struct GMAC_HANDLE *pGMAC)
{
    struct GMAC_Desc *desc;
    uint32_t value;
    uint32_t i = 0;

    HAL_ASSERT(pGMAC != NULL);

    value = READ_REG(pGMAC->pReg->DMA_CH0_TX_CONTROL);
    value &= ~DMA_CH0_TX_CONTROL_ST;
    WRITE_REG(pGMAC->pReg->DMA_CH0_TX_CONTROL, value);

    value = READ_REG(pGMAC->pReg->DMA_CH0_RX_CONTROL);
    value &= ~DMA_CH0_RX_CONTROL_SR;
    WRITE_REG(pGMAC->pReg->DMA_CH0_RX_CONTROL, value);

    value = READ_REG(pGMAC->pReg->MAC_CONFIGURATION);
    value &= ~(GMAC_CONFIG_TE | GMAC_CONFIG_RE);
    WRITE_REG(pGMAC->pReg->MAC_CONFIGURATION, value);

    for (i = 0; i < pGMAC->txSize; i++) {
        desc = pGMAC->txDescs + i;
        desc->des0 = 0;
        desc->des1 = 0;
        desc->des2 = 0;
        desc->des3 = 0;
    }

    for (i = 0; i < pGMAC->rxSize; i++) {
        desc = pGMAC->rxDescs + i;
        desc->des0 = 0;
        desc->des1 = 0;
        desc->des2 = 0;
        desc->des3 = 0;
    }

    return HAL_OK;
}

/**
  * @brief  Get current TX index in chain mode list.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return current tx desc index
  */
uint32_t HAL_GMAC_GetTXIndex(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);

    return pGMAC->txDescIdx;
}

/**
  * @brief  Get current RX index in chain mode list.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return current rx desc index
  */
uint32_t HAL_GMAC_GetRXIndex(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);

    return pGMAC->txDescIdx;
}

/**
  * @brief  Get current TX buffer.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
uint8_t *HAL_GMAC_GetTXBuffer(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);

    return pGMAC->txBuf + pGMAC->txDescIdx * HAL_GMAC_MAX_PACKET_SIZE;
}

/**
  * @brief  Get current RX buffer.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  *
  * @return HAL status
  */
uint8_t *HAL_GMAC_GetRXBuffer(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);

    return pGMAC->rxBuf + pGMAC->rxDescIdx * HAL_GMAC_MAX_PACKET_SIZE;
}

/**
  * @brief  Send a packet with the length, clean packet' dcached memory before send.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  packet: pointer to TX buffer.
  * @param  length: actual length for the packet.
  *
  * @return HAL status
  */
HAL_Status HAL_GMAC_Send(struct GMAC_HANDLE *pGMAC, void *packet,
                         uint32_t length)
{
    struct GMAC_Desc *desc;
    uint32_t i;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(packet != NULL);

    desc = pGMAC->txDescs + pGMAC->txDescIdx;
    if (desc->des3 & GMAC_DESC3_OWN) {
        HAL_DBG("%s(desc=%p, index=%ld) is busy\n", __func__, desc,
                pGMAC->txDescIdx);

        return HAL_TIMEOUT;
    }

    pGMAC->txDescIdx++;
    pGMAC->txDescIdx %= pGMAC->txSize;

    desc->des0 = (uint32_t)packet;
    desc->des1 = 0;
    desc->des2 = length;
    /*
     * Make sure that if HW sees the _OWN write below, it will see all the
     * writes to the rest of the descriptor too.
     */
    desc->des3 = GMAC_DESC3_OWN | GMAC_DESC3_FD | GMAC_DESC3_LD;

    WRITE_REG(pGMAC->pReg->DMA_CH0_TXDESC_TAIL_POINTER,
              (uint32_t)(pGMAC->txDescs + pGMAC->txDescIdx));

    for (i = 0; i < 1000000; i++) {
        if (!(desc->des3 & GMAC_DESC3_OWN)) {
            return HAL_BUSY;
        }
    }

    if (desc->des3 & DES3_ERROR_SUMMARY) {
        pGMAC->extraStatus.txErrors++;
    } else {
        pGMAC->extraStatus.txPktN++;
        pGMAC->extraStatus.txBytesN += length;
    }

    return HAL_OK;
}

/**
  * @brief  Recvive a packet, invalidate packet's dcached memory after received.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  length: pointer to length for RX packet.
  *
  * @return RX packet buffer pointer
  */
uint8_t *HAL_GMAC_Recv(struct GMAC_HANDLE *pGMAC, int32_t *length)
{
    struct GMAC_Desc *desc;
    uint8_t *packet;
    uint32_t des3;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(length != NULL);

    *length = 0;
    desc = pGMAC->rxDescs + pGMAC->rxDescIdx;
    des3 = desc->des3;
    if (des3 & GMAC_DESC3_OWN) {
        HAL_DBG("%s: RX packet not available\n", __func__);

        return NULL;
    }

    packet = pGMAC->rxBuf + (pGMAC->rxDescIdx * HAL_GMAC_MAX_PACKET_SIZE);
    *length = des3 & 0x7fff;

    if (des3 & DES3_ERROR_SUMMARY) {
        *length = 0;
        pGMAC->extraStatus.rxErrors++;

        return NULL;
    }

    /*
     * If frame length is greater than skb buffer size
     * (preallocated during init) then the packet is
     * ignored
     */
    if (*length > HAL_GMAC_MAX_FRAME_SIZE || *length <= ETH_FCS_LEN) {
        HAL_DBG("len %ld is incorrect for max size (%d)\n",
                *length, HAL_GMAC_MAX_FRAME_SIZE);
        *length = 0;
        pGMAC->extraStatus.rxErrors++;

        return NULL;
    }

    *length -= ETH_FCS_LEN;

    pGMAC->extraStatus.rxPktN++;
    pGMAC->extraStatus.rxBytesN += *length;

    return packet;
}

/**
  * @brief  Clean dirty RX description.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  */
void HAL_GMAC_CleanRX(struct GMAC_HANDLE *pGMAC)
{
    struct GMAC_Desc *desc;

    HAL_ASSERT(pGMAC != NULL);

    /* Get the pointer on the ith member of the Tx Desc list */
    desc = pGMAC->rxDescs + pGMAC->rxDescIdx;

    desc->des0 = (uint32_t)(pGMAC->rxBuf + (pGMAC->rxDescIdx *
                                            HAL_GMAC_MAX_PACKET_SIZE));
    desc->des1 = 0;
    desc->des2 = 0;
    desc->des3 = GMAC_DESC3_OWN | GMAC_DESC3_BUF1V | GMAC_DESC3_IOC;
    WRITE_REG(pGMAC->pReg->DMA_CH0_RXDESC_TAIL_POINTER, (uint32_t)desc);

    pGMAC->rxDescIdx++;
    pGMAC->rxDescIdx %= pGMAC->rxSize;
}

/**
  * @brief  Write MAC address to GMAC.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  enetAddr: pointer to array for MAC address.
  */
void HAL_GMAC_WriteHWAddr(struct GMAC_HANDLE *pGMAC, uint8_t *enetAddr)
{
    uint32_t val = 0;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(enetAddr != NULL);

    /* Update the GMAC address */
    val = (enetAddr[5] << 8) |
          (enetAddr[4]);
    WRITE_REG(pGMAC->pReg->MAC_ADDRESS0_HIGH, val);

    val = (enetAddr[3] << 24) |
          (enetAddr[2] << 16) |
          (enetAddr[1] << 8) |
          (enetAddr[0]);
    WRITE_REG(pGMAC->pReg->MAC_ADDRESS0_LOW, val);
}

/** @} */

/** @defgroup GMAC_Exported_Functions_Group4 Init and DeInit Functions

 This section provides functions allowing to init and deinit the module:

 *  @{
 */

/**
  * @brief  Initializes the Ethernet GMAC params including interface mode
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @param  pReg: base register for gmac controller.
  * @param  freq: pclk clock frequency rate.
  * @param  interface: RGMII or RMII.
  * @param  extClk: working clock from cru(output) or phy(external input clk).
  *
  * @retval HAL status
  */
HAL_Status HAL_GMAC_Init(struct GMAC_HANDLE *pGMAC, struct GMAC_REG *pReg,
                         uint32_t freq, eGMAC_PHY_Interface interface,
                         bool extClk)
{
    /* Check the GMAC handle allocation */
    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(IS_GMAC_INSTANCE(pReg));

    pGMAC->pReg = pReg;

    pGMAC->txDescIdx = 0;
    pGMAC->rxDescIdx = 0;

    /* Get CR bits depending on hclk value */
    if ((freq >= 20000000) && (freq < 35000000)) {
        /* CSR Clock Range between 20-35 MHz */
        pGMAC->clkCSR = GMAC_CSR_20_35M;
    } else if ((freq >= 35000000) && (freq < 60000000)) {
        /* CSR Clock Range between 35-60 MHz */
        pGMAC->clkCSR = GMAC_CSR_35_60M;
    } else if ((freq >= 60000000) && (freq < 100000000)) {
        /* CSR Clock Range between 60-100 MHz */
        pGMAC->clkCSR = GMAC_CSR_60_100M;
    } else if ((freq >= 100000000) && (freq < 150000000)) {
        /* CSR Clock Range between 100-150 MHz */
        pGMAC->clkCSR = GMAC_CSR_100_150M;
    } else if ((freq >= 150000000) && (freq < 250000000)) {
        /* CSR Clock Range between 150-250 MHz */
        pGMAC->clkCSR = GMAC_CSR_150_250M;
    } else {
        /* CSR Clock Range between 250-300 MHz */
        pGMAC->clkCSR = GMAC_CSR_250_300M;
    }

    pGMAC->link.duplex = GMAC_CONFIG_DM;
    pGMAC->link.speed10 = GMAC_CONFIG_PS;
    pGMAC->link.speed100 = GMAC_CONFIG_FES | GMAC_CONFIG_PS;
    pGMAC->link.speed1000 = 0;
    pGMAC->link.speedMask = GMAC_CONFIG_FES | GMAC_CONFIG_PS;

    pGMAC->mac.miiAddrShift = 21;
    pGMAC->mac.miiAddrMask = 0x03e00000;
    pGMAC->mac.miiRegShift = 16;
    pGMAC->mac.miiRegMask = 0x001f0000;
    pGMAC->mac.clkCsrShift = 8;
    pGMAC->mac.clkCsrMask = 0xf00;

    if (interface == PHY_INTERFACE_MODE_RGMII) {
        HAL_GMAC_SetToRGMII(pGMAC, 0, 0); /* update delayline at link */
    } else {
        HAL_GMAC_SetToRMII(pGMAC);
    }

    HAL_GMAC_SetExtclkSrc(pGMAC, extClk);

    return HAL_OK;
}

/**
  * @brief  DeInitialize the GMAC peripheral.
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  * @return HAL status
  */
HAL_Status HAL_GMAC_DeInit(struct GMAC_HANDLE *pGMAC)
{
    /* TO-DO */
    return HAL_OK;
}

/** @} */

/** @} */

/** @} */

#endif /* HAL_GMAC_MODULE_ENABLED */

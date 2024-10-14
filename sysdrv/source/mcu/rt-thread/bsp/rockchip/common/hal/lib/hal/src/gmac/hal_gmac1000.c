/* SPDX-License-Identifier: BSD-3-Clause */
/*
 * Copyright (c) 2020-2021 Rockchip Electronics Co., Ltd.
 */

#include "hal_base.h"

#ifdef HAL_GMAC1000_MODULE_ENABLED

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

/* Mdc/Mdio register */
#define GMAC_CSR_60_100M  0x0 /* MDC = clk_scr_i/42 */
#define GMAC_CSR_100_150M 0x1 /* MDC = clk_scr_i/62 */
#define GMAC_CSR_20_35M   0x2 /* MDC = clk_scr_i/16 */
#define GMAC_CSR_35_60M   0x3 /* MDC = clk_scr_i/26 */
#define GMAC_CSR_150_250M 0x4 /* MDC = clk_scr_i/102 */
#define GMAC_CSR_250_300M 0x5 /* MDC = clk_scr_i/122 */

#define MII_BUSY  0x00000001
#define MII_WRITE 0x00000002

#define MII_GMAC4_GOC_SHIFT 2
#define MII_GMAC4_WRITE     (1 << MII_GMAC4_GOC_SHIFT)
#define MII_GMAC4_READ      (3 << MII_GMAC4_GOC_SHIFT)

#define GMAC_MDIO_DATA_GD_MASK 0xffff
#define GMAC_MDIO_TIMEOUT      100/* ms */

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
#define DMA_MODE_SWR 1

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

/* DMA Normal interrupt */
#define DMA_INTR_ENA_NIE 0x00010000    /* Normal Summary */
#define DMA_INTR_ENA_TIE 0x00000001    /* Transmit Interrupt */
#define DMA_INTR_ENA_TUE 0x00000004    /* Transmit Buffer Unavailable */
#define DMA_INTR_ENA_RIE 0x00000040    /* Receive Interrupt */
#define DMA_INTR_ENA_ERE 0x00004000    /* Early Receive */

#define DMA_INTR_NORMAL (DMA_INTR_ENA_NIE | DMA_INTR_ENA_RIE | \
                    DMA_INTR_ENA_TIE)
/* DMA Abnormal interrupt */
#define DMA_INTR_ENA_AIE 0x00008000    /* Abnormal Summary */
#define DMA_INTR_ENA_FBE 0x00002000    /* Fatal Bus Error */
#define DMA_INTR_ENA_ETE 0x00000400    /* Early Transmit */
#define DMA_INTR_ENA_RWE 0x00000200    /* Receive Watchdog */
#define DMA_INTR_ENA_RSE 0x00000100    /* Receive Stopped */
#define DMA_INTR_ENA_RUE 0x00000080    /* Receive Buffer Unavailable */
#define DMA_INTR_ENA_UNE 0x00000020    /* Tx Underflow */
#define DMA_INTR_ENA_OVE 0x00000010    /* Receive Overflow */
#define DMA_INTR_ENA_TJE 0x00000008    /* Transmit Jabber */
#define DMA_INTR_ENA_TSE 0x00000002    /* Transmit Stopped */

#define DMA_INTR_ABNORMAL (DMA_INTR_ENA_AIE | DMA_INTR_ENA_FBE | \
                DMA_INTR_ENA_UNE)
/* DMA default interrupt mask */
#define DMA_INTR_DEFAULT_MASK (DMA_INTR_NORMAL | DMA_INTR_ABNORMAL)

/* DMA Status register defines */
#define DMA_STATUS_GLPII       0x40000000 /* GMAC LPI interrupt */
#define DMA_STATUS_GPI         0x10000000   /* PMT interrupt */
#define DMA_STATUS_GMI         0x08000000   /* MMC interrupt */
#define DMA_STATUS_GLI         0x04000000   /* GMAC Line interface int */
#define DMA_STATUS_EB_MASK     0x00380000   /* Error Bits Mask */
#define DMA_STATUS_EB_TX_ABORT 0x00080000       /* Error Bits - TX Abort */
#define DMA_STATUS_EB_RX_ABORT 0x00100000       /* Error Bits - RX Abort */
#define DMA_STATUS_TS_MASK     0x00700000   /* Transmit Process State */
#define DMA_STATUS_TS_SHIFT    20
#define DMA_STATUS_RS_MASK     0x000e0000   /* Receive Process State */
#define DMA_STATUS_RS_SHIFT    17
#define DMA_STATUS_NIS         0x00010000/* Normal Interrupt Summary */
#define DMA_STATUS_AIS         0x00008000/* Abnormal Interrupt Summary */
#define DMA_STATUS_ERI         0x00004000/* Early Receive Interrupt */
#define DMA_STATUS_FBI         0x00002000/* Fatal Bus Error Interrupt */
#define DMA_STATUS_ETI         0x00000400/* Early Transmit Interrupt */
#define DMA_STATUS_RWT         0x00000200/* Receive Watchdog Timeout */
#define DMA_STATUS_RPS         0x00000100/* Receive Process Stopped */
#define DMA_STATUS_RU          0x00000080/* Receive Buffer Unavailable */
#define DMA_STATUS_RI          0x00000040/* Receive Interrupt */
#define DMA_STATUS_UNF         0x00000020/* Transmit Underflow */
#define DMA_STATUS_OVF         0x00000010/* Receive Overflow */
#define DMA_STATUS_TJT         0x00000008/* Transmit Jabber Timeout */
#define DMA_STATUS_TU          0x00000004/* Transmit Buffer Unavailable */
#define DMA_STATUS_TPS         0x00000002/* Transmit Process Stopped */
#define DMA_STATUS_TI          0x00000001/* Transmit Interrupt */

/* Description related */
/* Normal receive descriptor defines */
/* RDES0 */
#define    RDES0_PAYLOAD_CSUM_ERR    (1 << 0)
#define    RDES0_CRC_ERROR           (1 << 1)
#define    RDES0_DRIBBLING           (1 << 2)
#define    RDES0_MII_ERROR           (1 << 3)
#define    RDES0_RECEIVE_WATCHDOG    (1 << 4)
#define    RDES0_FRAME_TYPE          (1 << 5)
#define    RDES0_COLLISION           (1 << 6)
#define    RDES0_IPC_CSUM_ERROR      (1 << 7)
#define    RDES0_LAST_DESCRIPTOR     (1 << 8)
#define    RDES0_FIRST_DESCRIPTOR    (1 << 9)
#define    RDES0_VLAN_TAG            (1 << 10)
#define    RDES0_OVERFLOW_ERROR      (1 << 11)
#define    RDES0_LENGTH_ERROR        (1 << 12)
#define    RDES0_SA_FILTER_FAIL      (1 << 13)
#define    RDES0_DESCRIPTOR_ERROR    (1 << 14)
#define    RDES0_ERROR_SUMMARY       (1 << 15)
#define    RDES0_FRAME_LEN_MASK      0x3fff0000
#define        RDES0_FRAME_LEN_SHIFT 16
#define    RDES0_DA_FILTER_FAIL      (1 << 30)
#define    RDES0_OWN                 (1 << 31)
/* RDES1 */
#define    RDES1_BUFFER1_SIZE_MASK      0x000007ff
#define    RDES1_BUFFER2_SIZE_MASK      0x003ff800
#define    RDES1_BUFFER2_SIZE_SHIFT     11
#define    RDES1_SECOND_ADDRESS_CHAINED (1 << 24)
#define    RDES1_END_RING               (1 << 25)
#define    RDES1_DISABLE_IC             (1 << 31)

/* Normal transmit descriptor defines */
/* TDES0 */
#define    TDES0_DEFERRED             (1 << 0)
#define    TDES0_UNDERFLOW_ERROR      (1 << 1)
#define    TDES0_EXCESSIVE_DEFERRAL   (1 << 2)
#define    TDES0_COLLISION_COUNT_MASK 0x78
#define    TDES0_VLAN_FRAME           (1 << 7)
#define    TDES0_EXCESSIVE_COLLISIONS (1 << 8)
#define    TDES0_LATE_COLLISION       (1 << 9)
#define    TDES0_NO_CARRIER           (1 << 10)
#define    TDES0_LOSS_CARRIER         (1 << 11)
#define    TDES0_PAYLOAD_ERROR        (1 << 12)
#define    TDES0_FRAME_FLUSHED        (1 << 13)
#define    TDES0_JABBER_TIMEOUT       (1 << 14)
#define    TDES0_ERROR_SUMMARY        (1 << 15)
#define    TDES0_IP_HEADER_ERROR      (1 << 16)
#define    TDES0_TIME_STAMP_STATUS    (1 << 17)
#define    TDES0_OWN                  (1 << 31)/* silence sparse */
/* TDES1 */
#define    TDES1_BUFFER1_SIZE_MASK        0x000007ff
#define    TDES1_BUFFER2_SIZE_MASK        0x003ff800
#define    TDES1_BUFFER2_SIZE_SHIFT       11
#define    TDES1_TIME_STAMP_ENABLE        (1 << 22)
#define    TDES1_DISABLE_PADDING          (1 << 23)
#define    TDES1_SECOND_ADDRESS_CHAINED   (1 << 24)
#define    TDES1_END_RING                 (1 << 25)
#define    TDES1_CRC_DISABLE              (1 << 26)
#define    TDES1_CHECKSUM_INSERTION_MASK  0x18000000
#define    TDES1_CHECKSUM_INSERTION_SHIFT 27
#define    TDES1_FIRST_SEGMENT            (1 << 29)
#define    TDES1_LAST_SEGMENT             (1 << 30)
#define    TDES1_INTERRUPT                (1 << 31)

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

/* GMAC Configuration defines */
#define GMAC_CONTROL_2K 0x08000000       /* IEEE 802.3as 2K packets */
#define GMAC_CONTROL_TC 0x01000000       /* Transmit Conf. in RGMII/SGMII */
#define GMAC_CONTROL_WD 0x00800000       /* Disable Watchdog on receive */
#define GMAC_CONTROL_JD 0x00400000       /* Jabber disable */
#define GMAC_CONTROL_BE 0x00200000       /* Frame Burst Enable */
#define GMAC_CONTROL_JE 0x00100000       /* Jumbo frame */

#define GMAC_CONTROL_DCRS 0x00010000       /* Disable carrier sense */
#define GMAC_CONTROL_PS   0x00008000       /* Port Select 0:GMI 1:MII */
#define GMAC_CONTROL_FES  0x00004000       /* Speed 0:10 1:100 */
#define GMAC_CONTROL_DO   0x00002000       /* Disable Rx Own */
#define GMAC_CONTROL_LM   0x00001000       /* Loop-back mode */
#define GMAC_CONTROL_DM   0x00000800       /* Duplex Mode */
#define GMAC_CONTROL_IPC  0x00000400       /* Checksum Offload */
#define GMAC_CONTROL_DR   0x00000200       /* Disable Retry */
#define GMAC_CONTROL_LUD  0x00000100       /* Link up/down */
#define GMAC_CONTROL_ACS  0x00000080       /* Auto Pad/FCS Stripping */
#define GMAC_CONTROL_DC   0x00000010       /* Deferral Check */
#define GMAC_CONTROL_TE   0x00000008       /* Transmitter Enable */
#define GMAC_CONTROL_RE   0x00000004       /* Receiver Enable */

#define GMAC_CORE_INIT (GMAC_CONTROL_JD | GMAC_CONTROL_PS | \
            GMAC_CONTROL_BE | GMAC_CONTROL_DCRS)

/* DMA Control register defines */
#define DMA_CONTROL_ST 0x00002000           /* Start/Stop Transmission */
#define DMA_CONTROL_SR 0x00000002           /* Start/Stop Receive */

/* interrupt mask register */
#define GMAC_INT_MASK              0x0000003c
#define GMAC_INT_DISABLE_RGMII     (1 << 0)
#define GMAC_INT_DISABLE_PCSLINK   (1 << 1)
#define GMAC_INT_DISABLE_PCSAN     (1 << 2)
#define GMAC_INT_DISABLE_PMT       (1 << 3)
#define GMAC_INT_DISABLE_TIMESTAMP (1 << 9)
#define GMAC_INT_DISABLE_PCS       (GMAC_INT_DISABLE_RGMII | \
                                GMAC_INT_DISABLE_PCSLINK |   \
                                GMAC_INT_DISABLE_PCSAN)
#define GMAC_INT_DEFAULT_MASK      (GMAC_INT_DISABLE_TIMESTAMP | \
                                GMAC_INT_DISABLE_PCS)

#define GMAC_FLOW_CTRL_UP  0x00000008         /* Unicast pause frame enable */
#define GMAC_FLOW_CTRL_RFE 0x00000004         /* Rx Flow Control Enable */
#define GMAC_FLOW_CTRL_TFE 0x00000002         /* Tx Flow Control Enable */

#define SF_DMA_MODE 1           /* DMA STORE-AND-FORWARD Operation Mode */

enum rtc_control {
    DMA_CONTROL_RTC_64  = 0x00000000,
    DMA_CONTROL_RTC_32  = 0x00000008,
    DMA_CONTROL_RTC_96  = 0x00000010,
    DMA_CONTROL_RTC_128 = 0x00000018,
};
#define DMA_CONTROL_OSF        0x00000004       /* Operate on second frame */
#define DMA_CONTROL_RSF        0x02000000       /* Receive Store and Forward */
#define DMA_CONTROL_TC_RX_MASK 0xffffffe7

#define DMA_CONTROL_TSF 0x00200000              /* Transmit  Store and Forward */

enum ttc_control {
    DMA_CONTROL_TTC_64  = 0x00000000,
    DMA_CONTROL_TTC_128 = 0x00004000,
    DMA_CONTROL_TTC_192 = 0x00008000,
    DMA_CONTROL_TTC_256 = 0x0000c000,
    DMA_CONTROL_TTC_40  = 0x00010000,
    DMA_CONTROL_TTC_32  = 0x00014000,
    DMA_CONTROL_TTC_24  = 0x00018000,
    DMA_CONTROL_TTC_16  = 0x0001c000,
};
#define DMA_CONTROL_TC_TX_MASK 0xfffe3fff

#define DMA_CONTROL_EFC 0x00000100
#define DMA_CONTROL_FEF 0x00000080
#define DMA_CONTROL_FUF 0x00000040

/* Receive flow control activation field
 * RFA field in DMA control register, bits 23,10:9
 */
#define DMA_CONTROL_RFA_MASK 0x00800600

/* Receive flow control deactivation field
 * RFD field in DMA control register, bits 22,12:11
 */
#define DMA_CONTROL_RFD_MASK 0x00401800

#define RFA_FULL_MINUS_1K 0x00000000
#define RFA_FULL_MINUS_2K 0x00000200

/* MMC control register */
/* When set, all counter are reset */
#define MMC_CNTRL_COUNTER_RESET 0x1
/* When set, do not roll over zero after reaching the max value*/
#define MMC_CNTRL_COUNTER_STOP_ROLLOVER 0x2
#define MMC_CNTRL_RESET_ON_READ         0x4     /* Reset after reading */
#define MMC_CNTRL_COUNTER_FREEZER       0x8     /* Freeze counter values to the current value.*/
#define MMC_CNTRL_PRESET                0x10
#define MMC_CNTRL_FULL_HALF_PRESET      0x20

/*--- DMA BLOCK defines ---*/
/* DMA Bus Mode register defines */
#define DMA_BUS_MODE_DA        0x00000002    /* Arbitration scheme */
#define DMA_BUS_MODE_DSL_MASK  0x0000007c      /* Descriptor Skip Length */
#define DMA_BUS_MODE_DSL_SHIFT 2           /*   (in DWORDS)      */
/* Programmable burst length (passed thorugh platform)*/
#define DMA_BUS_MODE_PBL_MASK  0x00003f00      /* Programmable Burst Len */
#define DMA_BUS_MODE_PBL_SHIFT 8
#define DMA_BUS_MODE_ATDS      0x00000080  /* Alternate Descriptor Size */

#define DMA_BUS_MODE_FB         0x00010000   /* Fixed burst */
#define DMA_BUS_MODE_MB         0x04000000   /* Mixed burst */
#define DMA_BUS_MODE_RPBL_MASK  0x007e0000      /* Rx-Programmable Burst Len */
#define DMA_BUS_MODE_RPBL_SHIFT 17
#define DMA_BUS_MODE_USP        0x00800000
#define DMA_BUS_MODE_MAXPBL     0x01000000
#define DMA_BUS_MODE_AAL        0x02000000

#define MMC_RX_INTR      0x04      /* MMC RX Interrupt */
#define MMC_TX_INTR      0x08      /* MMC TX Interrupt */
#define MMC_RX_INTR_MASK 0x0c       /* MMC Interrupt Mask */
#define MMC_TX_INTR_MASK 0x10       /* MMC Interrupt Mask */
#define MMC_DEFAULT_MASK 0xffffffff

#define dsb(opt) asm volatile("dsb " #opt : : : "memory")
#define wmb()    dsb(st)

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
    uint32_t timeout = 0;

    while ((READ_REG(pGMAC->pReg->GMII_ADDR) & MII_BUSY) ==
           MII_BUSY) {
        HAL_DelayMs(1);
        timeout++;
        if (timeout > GMAC_MDIO_TIMEOUT) {
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
    unsigned int flow = GMAC_FLOW_CTRL_UP;

    if (fc & HAL_GMAC_FLOW_RX) {
        HAL_DBG("\tReceive Flow-Control ON\n");
        flow |= GMAC_FLOW_CTRL_RFE;
    }

    if (fc & HAL_GMAC_FLOW_TX) {
        HAL_DBG("\tTransmit Flow-Control ON\n");

        if (duplex) {
            HAL_DBG("\tduplex mode: PAUSE %ld\n", pauseTime);
        }

        flow |= GMAC_FLOW_CTRL_TFE;
        if (duplex) {
            flow |= (pauseTime << GMAC_FLOW_CTRL_PT_SHIFT);
        }
    }

    WRITE_REG(pGMAC->pReg->FLOW_CTRL, flow);

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
    uint32_t csr6 = READ_REG(pGMAC->pReg->OP_MODE);

    if (mode == SF_DMA_MODE) {
        HAL_DBG("GMAC: enable RX store and forward mode\n");
        csr6 |= DMA_CONTROL_RSF;
    } else {
        HAL_DBG("GMAC: disable RX SF mode (threshold %ld)\n", mode);
        csr6 &= ~DMA_CONTROL_RSF;
        csr6 &= DMA_CONTROL_TC_RX_MASK;
        if (mode <= 32) {
            csr6 |= DMA_CONTROL_RTC_32;
        } else if (mode <= 64) {
            csr6 |= DMA_CONTROL_RTC_64;
        } else if (mode <= 96) {
            csr6 |= DMA_CONTROL_RTC_96;
        } else {
            csr6 |= DMA_CONTROL_RTC_128;
        }
    }

    /* Configure flow control based on rx fifo size */
    csr6 &= ~DMA_CONTROL_RFA_MASK;
    csr6 &= ~DMA_CONTROL_RFD_MASK;

    /* Leave flow control disabled if receive fifo size is less than
     * 4K or 0. Otherwise, send XOFF when fifo is 1K less than full,
     * and send XON when 2K less than full.
     */
    if (fifosz < 4096) {
        csr6 &= ~DMA_CONTROL_EFC;
    } else {
        csr6 |= DMA_CONTROL_EFC;
        csr6 |= RFA_FULL_MINUS_1K;
        csr6 |= RFA_FULL_MINUS_2K;
    }

    WRITE_REG(pGMAC->pReg->OP_MODE, csr6);

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
    uint32_t csr6 = READ_REG(pGMAC->pReg->OP_MODE);

    if (mode == SF_DMA_MODE) {
        /* Transmit COE type 2 cannot be done in cut-through mode. */
        csr6 |= DMA_CONTROL_TSF;
        /* Operating on second frame increase the performance
         * especially when transmit store-and-forward is used.
         */
        csr6 |= DMA_CONTROL_OSF;
    } else {
        csr6 &= ~DMA_CONTROL_TSF;
        csr6 &= DMA_CONTROL_TC_TX_MASK;
        /* Set the transmit threshold */
        if (mode <= 32) {
            csr6 |= DMA_CONTROL_TTC_32;
        } else if (mode <= 64) {
            csr6 |= DMA_CONTROL_TTC_64;
        } else if (mode <= 128) {
            csr6 |= DMA_CONTROL_TTC_128;
        } else if (mode <= 192) {
            csr6 |= DMA_CONTROL_TTC_192;
        } else {
            csr6 |= DMA_CONTROL_TTC_256;
        }
    }

    WRITE_REG(pGMAC->pReg->OP_MODE, csr6);

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
    int32_t val = MII_BUSY;

    HAL_ASSERT(pGMAC != NULL);

    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO not idle at entry");

        return status;
    }

    val |= (mdioAddr << pGMAC->mac.miiAddrShift) & pGMAC->mac.miiAddrMask;
    val |= (mdioReg << pGMAC->mac.miiRegShift) & pGMAC->mac.miiRegMask;
    val |= (pGMAC->clkCSR << pGMAC->mac.clkCsrShift) & pGMAC->mac.clkCsrMask;

    WRITE_REG(pGMAC->pReg->GMII_ADDR, val);

    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO read didn't complete\n");

        return status;
    }

    val = READ_REG(pGMAC->pReg->GMII_DATA);
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
    int32_t val = MII_BUSY;

    HAL_ASSERT(pGMAC != NULL);

    HAL_DBG("%s(addr=%lx, reg=%ld, val=%x):\n", __func__,
            mdioAddr, mdioReg, mdioVal);
    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO not idle at entry");

        return status;
    }

    val |= (mdioAddr << pGMAC->mac.miiAddrShift) & pGMAC->mac.miiAddrMask;
    val |= (mdioReg << pGMAC->mac.miiRegShift) & pGMAC->mac.miiRegMask;
    val |= (pGMAC->clkCSR << pGMAC->mac.clkCsrShift) & pGMAC->mac.clkCsrMask;

    val |= MII_WRITE;

    WRITE_REG(pGMAC->pReg->GMII_DATA, mdioVal);
    WRITE_REG(pGMAC->pReg->GMII_ADDR, val);

    status = Mdio_WaitIdle(pGMAC);
    if (status) {
        HAL_DBG("MDIO read didn't complete\n");

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
            pGMAC->phyStatus.pause = (lpa & LPA_PAUSE_CAP) ? 1 : 0;
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

        desc->des0 = RDES0_OWN;
        desc->des1 = RDES1_BUFFER1_SIZE_MASK & HAL_GMAC_MAX_PACKET_SIZE;
        if (i == (count - 1)) {
            desc->des1 |= RDES1_END_RING;
        }
        desc->des2 = (uint32_t)(rxBuff + i * HAL_GMAC_MAX_PACKET_SIZE);
        desc->des3 = 0;
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

    intrStatus = READ_REG(pGMAC->pReg->STATUS);
    /* ABNORMAL interrupts */
    if (intrStatus & DMA_STATUS_AIS) {
        if (intrStatus & DMA_STATUS_RU) {
            pGMAC->extraStatus.rxBufUnavIRQ++;
            status = DMA_RX_ERROR;
        }
        if (intrStatus & DMA_STATUS_RPS) {
            pGMAC->extraStatus.rxProcessStoppedIRQ++;
            status = DMA_RX_ERROR;
        }
        if (intrStatus & DMA_STATUS_RWT) {
            pGMAC->extraStatus.rxWatchdogIRQ++;
            status = DMA_RX_ERROR;
        }
        if (intrStatus & DMA_STATUS_ETI) {
            pGMAC->extraStatus.txEarlyIRQ++;
            status = DMA_TX_ERROR;
        }
        if (intrStatus & DMA_STATUS_TPS) {
            pGMAC->extraStatus.rxProcessStoppedIRQ++;
            status = DMA_TX_ERROR;
        }
        if (intrStatus & DMA_STATUS_FBI) {
            pGMAC->extraStatus.fatalBusErrorIRQ++;
            status = DMA_TX_ERROR;
        }
    }

    /* TX/RX NORMAL interrupts */
    if (intrStatus & DMA_STATUS_NIS) {
        pGMAC->extraStatus.normalIRQN++;
        if (intrStatus & DMA_STATUS_RI) {
            uint32_t value;

            value = READ_REG(pGMAC->pReg->INT_ENA);
            if (value & DMA_INTR_ENA_RIE) {
                pGMAC->extraStatus.rxNormalIRQN++;
                status |= DMA_HANLE_RX;
            }
        }
        if (intrStatus & DMA_STATUS_TI) {
            pGMAC->extraStatus.txNormallIRQN++;
            status |= DMA_HANLE_TX;
        }
        if (intrStatus & DMA_STATUS_ERI) {
            pGMAC->extraStatus.rxEarlyIRQ++;
        }
    }

    /*
     * Clear the interrupt by writing a logic 1 to the chanX interrupt
     * status [21-0] expect reserved bits [5-3]
     */
    WRITE_REG(pGMAC->pReg->STATUS, intrStatus & 0x1ffff);

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

    ctrl = READ_REG(pGMAC->pReg->MAC_CONF);
    ctrl |= GMAC_CORE_INIT;

    /* Flow Control operation */
    if (pGMAC->phyStatus.pause) {
        GMAC_FlowCtrl(pGMAC, pGMAC->phyStatus.duplex,
                      HAL_GMAC_FLOW_AUTO, HAL_PAUSE_TIME);
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

    //ctrl |= GMAC_CONFIG_TE;
    WRITE_REG(pGMAC->pReg->MAC_CONF, ctrl);

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
    WRITE_REG(pGMAC->pReg->INT_ENA, DMA_INTR_DEFAULT_MASK);
}

/**
  * @brief  Disable DMA IRQ
  * @param  pGMAC: pointer to a GMAC_HANDLE structure that contains
  *                the information for GMAC module.
  */
void HAL_GMAC_DisableDmaIRQ(struct GMAC_HANDLE *pGMAC)
{
    HAL_ASSERT(pGMAC != NULL);
    WRITE_REG(pGMAC->pReg->INT_ENA, 0);
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

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(addr != NULL);

    /* GMAC Software Reset */
    value = READ_REG(pGMAC->pReg->BUS_MODE);
    WRITE_REG(pGMAC->pReg->BUS_MODE, value | DMA_MODE_SWR);
    /* Wait for software Reset */
    while (limit--) {
        if (!(READ_REG(pGMAC->pReg->BUS_MODE) & DMA_MODE_SWR)) {
            break;
        }
        HAL_DelayMs(100);
    }
    if (limit <= 0) {
        HAL_DBG("DMA_MODE_SWR stuck\n");

        return HAL_TIMEOUT;
    }

    HAL_DelayMs(100);

    /* Core init */
    value = READ_REG(pGMAC->pReg->MAC_CONF);
    value |= GMAC_CORE_INIT;
    value &= ~GMAC_CONTROL_ACS;
    WRITE_REG(pGMAC->pReg->MAC_CONF, value);

    /* Mask GMAC interrupts */
    WRITE_REG(pGMAC->pReg->INT_MASK, GMAC_INT_DEFAULT_MASK);

    /* DMA init */
    value = READ_REG(pGMAC->pReg->BUS_MODE);
    value &= ~(DMA_BUS_MODE_PBL_MASK | DMA_BUS_MODE_RPBL_MASK);
    value |= DMA_BUS_MODE_MAXPBL;
    value |= (8 << DMA_BUS_MODE_PBL_SHIFT);
    value |= (8 << DMA_BUS_MODE_RPBL_SHIFT);
    WRITE_REG(pGMAC->pReg->BUS_MODE, value);

    /* init rx chan */
    WRITE_REG(pGMAC->pReg->RX_DESC_LIST_ADDR, (uint32_t)pGMAC->rxDescs);

    /* init tx chan */
    WRITE_REG(pGMAC->pReg->TX_DESC_LIST_ADDR, (uint32_t)pGMAC->txDescs);

    HAL_GMAC_WriteHWAddr(pGMAC, addr);

    value = READ_REG(pGMAC->pReg->OP_MODE);
    value |= DMA_CONTROL_ST;
    WRITE_REG(pGMAC->pReg->OP_MODE, value);

    value = READ_REG(pGMAC->pReg->OP_MODE);
    value |= DMA_CONTROL_SR;
    WRITE_REG(pGMAC->pReg->OP_MODE, value);

    /* enable ipc/disable acs */
    value = READ_REG(pGMAC->pReg->MAC_CONF);
    value |= GMAC_CONTROL_IPC;
    WRITE_REG(pGMAC->pReg->MAC_CONF, value);

    GMAC_DMARXOpMode(pGMAC, mode, 0);
    GMAC_DMATXOpMode(pGMAC, mode, 0);

    WRITE_REG(pGMAC->pReg->MAC_FRM_FILT, 0x00000404);
    WRITE_REG(pGMAC->pReg->HASH_TAB_HI, 0x00000001);
    WRITE_REG(pGMAC->pReg->HASH_TAB_LO, 0x00040002);

    value = READ_REG(pGMAC->pReg->BUS_MODE);
    value |= (1 << 23);
    WRITE_REG(pGMAC->pReg->BUS_MODE, value);
    /* MMC */
    WRITE_REG(pGMAC->pReg->MMC_RX_INT_MSK, MMC_DEFAULT_MASK);
    WRITE_REG(pGMAC->pReg->MMC_TX_INT_MSK, MMC_DEFAULT_MASK);
    WRITE_REG(pGMAC->pReg->MMC_IPC_INT_MSK, MMC_DEFAULT_MASK);
    value = READ_REG(pGMAC->pReg->MMC_CTRL);
    value |= (mmc_mode & 0x3F);
    WRITE_REG(pGMAC->pReg->MMC_CTRL, value);

    HAL_GMAC_EnableDmaIRQ(pGMAC);
    value = READ_REG(pGMAC->pReg->MAC_CONF);
    value |= GMAC_CONTROL_TE | GMAC_CONTROL_RE;
    WRITE_REG(pGMAC->pReg->MAC_CONF, value);

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

    value = READ_REG(pGMAC->pReg->OP_MODE);
    value &= ~DMA_CONTROL_ST;
    WRITE_REG(pGMAC->pReg->OP_MODE, value);

    value = READ_REG(pGMAC->pReg->OP_MODE);
    value &= ~DMA_CONTROL_SR;
    WRITE_REG(pGMAC->pReg->OP_MODE, value);

    value = READ_REG(pGMAC->pReg->MAC_CONF);
    value &= ~(GMAC_CONTROL_TE | GMAC_CONTROL_RE);
    WRITE_REG(pGMAC->pReg->MAC_CONF, value);

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
    uint32_t i, entry = pGMAC->txDescIdx;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(packet != NULL);

    desc = pGMAC->txDescs + pGMAC->txDescIdx;
    if (desc->des0 & TDES0_OWN) {
        HAL_DBG("%s(desc=%p, index=%ld) is busy\n", __func__, desc,
                pGMAC->txDescIdx);

        return HAL_TIMEOUT;
    }

    pGMAC->txDescIdx++;
    pGMAC->txDescIdx %= pGMAC->txSize;

    /*
     * Make sure that if HW sees the _OWN write below, it will see all the
     * writes to the rest of the descriptor too.
     */
    desc->des0 = TDES0_OWN;
    desc->des1 = (length & TDES1_BUFFER1_SIZE_MASK) | TDES1_FIRST_SEGMENT | TDES1_LAST_SEGMENT;
    desc->des1 |= (3 << TDES1_CHECKSUM_INSERTION_SHIFT);
    if (entry == (pGMAC->txSize - 1)) {
        desc->des1 |= TDES1_END_RING;
    }
    desc->des2 = (uint32_t)packet;

    wmb();
    WRITE_REG(pGMAC->pReg->TX_POLL_DEMAND, 1);

    for (i = 0; i < 1000000; i++) {
        if (!(desc->des0 & TDES0_OWN)) {
            return HAL_BUSY;
        }
    }

    if (desc->des0 & TDES0_ERROR_SUMMARY) {
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
    uint32_t des0;

    HAL_ASSERT(pGMAC != NULL);
    HAL_ASSERT(length != NULL);

    *length = 0;
    desc = pGMAC->rxDescs + pGMAC->rxDescIdx;
    des0 = desc->des0;
    if (des0 & RDES0_OWN) {
        return NULL;
    }

    packet = pGMAC->rxBuf + (pGMAC->rxDescIdx * HAL_GMAC_MAX_PACKET_SIZE);
    *length = (des0 & RDES0_FRAME_LEN_MASK) >> RDES0_FRAME_LEN_SHIFT;

    if (des0 & RDES0_ERROR_SUMMARY) {
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

    desc->des0 = RDES0_OWN;
    desc->des1 = RDES1_BUFFER1_SIZE_MASK & HAL_GMAC_MAX_PACKET_SIZE;
    if (pGMAC->rxDescIdx == (pGMAC->rxSize - 1)) {
        desc->des1 |= RDES1_END_RING;
    }
    desc->des2 = (uint32_t)(pGMAC->rxBuf + (pGMAC->rxDescIdx *
                                            HAL_GMAC_MAX_PACKET_SIZE));

    wmb();
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
    WRITE_REG(pGMAC->pReg->MAC_ADDR0_HI, val);

    val = (enetAddr[3] << 24) |
          (enetAddr[2] << 16) |
          (enetAddr[1] << 8) |
          (enetAddr[0]);
    WRITE_REG(pGMAC->pReg->MAC_ADDR0_LO, val);
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

    if (interface == PHY_INTERFACE_MODE_RGMII) {
        HAL_GMAC_SetToRGMII(pGMAC, 0, 0); /* update delayline at link */
    } else {
        HAL_GMAC_SetToRMII(pGMAC);
    }

    HAL_GMAC_SetExtclkSrc(pGMAC, extClk);

    pGMAC->link.duplex = GMAC_CONTROL_DM;
    pGMAC->link.speed10 = GMAC_CONTROL_PS;
    pGMAC->link.speed100 = GMAC_CONTROL_PS | GMAC_CONTROL_FES;
    pGMAC->link.speed1000 = 0;
    pGMAC->link.speedMask = GMAC_CONTROL_PS | GMAC_CONTROL_FES;

    pGMAC->mac.miiAddrShift = 11;
    pGMAC->mac.miiAddrMask = 0x0000F800;
    pGMAC->mac.miiRegShift = 6;
    pGMAC->mac.miiRegMask = 0x000007C0;
    pGMAC->mac.clkCsrShift = 2;
    pGMAC->mac.clkCsrMask = 0x3c;

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

#endif /* HAL_GMAC1000_MODULE_ENABLED */

#ifndef _AICWF_SDIO_BT_H_
#define _AICWF_SDIO_BT_H_

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/skbuff.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/poll.h>

#include <linux/version.h>
#include <linux/pm_runtime.h>
#include <linux/firmware.h>
#include <linux/suspend.h>


#ifdef CONFIG_PLATFORM_UBUNTU
#define CONFIG_BLUEDROID        1 /* bleuz 0, bluedroid 1 */
#else
#define CONFIG_BLUEDROID        1 /* bleuz 0, bluedroid 1 */
#endif
/* #define HCI_VERSION_CODE KERNEL_VERSION(3, 14, 41) */
#define HCI_VERSION_CODE LINUX_VERSION_CODE


#define PRINT_CMD_EVENT         1
#define PRINT_ACL_DATA          1
#define PRINT_SCO_DATA          1

#define AICBT_DBG_FLAG          1

#if AICBT_DBG_FLAG
#define AICBT_DBG(fmt, arg...) printk( "aic_btsdio: " fmt "\n" , ## arg)
#else
#define AICBT_DBG(fmt, arg...)
#endif

#define AICBT_INFO(fmt, arg...) printk("aic_btsdio: " fmt "\n" , ## arg)
#define AICBT_WARN(fmt, arg...) printk("aic_btsdio: " fmt "\n" , ## arg)
#define AICBT_ERR(fmt, arg...) printk("aic_btsdio: " fmt "\n" , ## arg)

#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 4, 0)
#define GET_DRV_DATA(x)        hci_get_drvdata(x)
#else
#define GET_DRV_DATA(x)        x->driver_data
#endif

struct btusb_data {
    struct hci_dev       *hdev;
    //struct usb_device    *udev;
    //struct usb_interface *intf;
    //struct usb_interface *isoc;

    spinlock_t lock;

    unsigned long flags;

    struct work_struct work;
    struct work_struct waker;

    /*struct usb_anchor tx_anchor;
    struct usb_anchor intr_anchor;
    struct usb_anchor bulk_anchor;
    struct usb_anchor isoc_anchor;
    struct usb_anchor deferred;*/
    int tx_in_flight;
    spinlock_t txlock;
	
#if (CONFIG_BLUEDROID == 0)
#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
		spinlock_t rxlock;
		struct sk_buff *evt_skb;
		struct sk_buff *acl_skb;
		struct sk_buff *sco_skb;
#endif
#endif

    /*struct usb_endpoint_descriptor *intr_ep;
    struct usb_endpoint_descriptor *bulk_tx_ep;
    struct usb_endpoint_descriptor *bulk_rx_ep;
    struct usb_endpoint_descriptor *isoc_tx_ep;
    struct usb_endpoint_descriptor *isoc_rx_ep;*/

    __u8 cmdreq_type;

    unsigned int sco_num;
    int isoc_altsetting;
    int suspend_count;
    uint16_t sco_handle;

#if (CONFIG_BLUEDROID == 0)
#if HCI_VERSION_CODE >= KERNEL_VERSION(3, 18, 0)
    int (*recv_bulk) (struct btusb_data * data, void *buffer, int count);
#endif
#endif

//#ifdef CONFIG_HAS_EARLYSUSPEND
#if 0
    struct early_suspend early_suspend;
#else
    struct notifier_block pm_notifier;
    struct notifier_block reboot_notifier;
#endif
    //firmware_info *fw_info;

#ifdef CONFIG_SCO_OVER_HCI
    AIC_sco_card_t  *pSCOSnd;
#endif
};



#define QUEUE_SIZE 500

/***************************************
** AicSemi - Integrate from bluetooth.h **
*****************************************/
/* Reserv for core and drivers use */
#define BT_SKB_RESERVE    8

/* BD Address */
typedef struct {
    __u8 b[6];
} __packed bdaddr_t;

/* Skb helpers */
struct bt_skb_cb {
    __u8 pkt_type;
    __u8 incoming;
    __u16 expect;
    __u16 tx_seq;
    __u8 retries;
    __u8 sar;
    __u8 force_active;
};

#define bt_cb(skb) ((struct bt_skb_cb *)((skb)->cb))


/***********************************
** AicSemi - Integrate from hci.h **
***********************************/
#define HCI_MAX_ACL_SIZE    1024
#define HCI_MAX_SCO_SIZE    255
#define HCI_MAX_EVENT_SIZE    260
#define HCI_MAX_FRAME_SIZE    (HCI_MAX_ACL_SIZE + 4)

/* HCI bus types */
#define HCI_VIRTUAL    0
#define HCI_USB        1
#define HCI_PCCARD    2
#define HCI_UART    3
#define HCI_RS232    4
#define HCI_PCI        5
#define HCI_SDIO    6

/* HCI controller types */
#define HCI_BREDR    0x00
#define HCI_AMP        0x01

/* HCI device flags */
enum {
    HCI_UP,
    HCI_INIT,
    HCI_RUNNING,

    HCI_PSCAN,
    HCI_ISCAN,
    HCI_AUTH,
    HCI_ENCRYPT,
    HCI_INQUIRY,

    HCI_RAW,

    HCI_RESET,
};

/*
 * BR/EDR and/or LE controller flags: the flags defined here should represent
 * states from the controller.
 */
enum {
    HCI_SETUP,
    HCI_AUTO_OFF,
    HCI_MGMT,
    HCI_PAIRABLE,
    HCI_SERVICE_CACHE,
    HCI_LINK_KEYS,
    HCI_DEBUG_KEYS,
    HCI_UNREGISTER,

    HCI_LE_SCAN,
    HCI_SSP_ENABLED,
    HCI_HS_ENABLED,
    HCI_LE_ENABLED,
    HCI_CONNECTABLE,
    HCI_DISCOVERABLE,
    HCI_LINK_SECURITY,
    HCI_PENDING_CLASS,
};

/* HCI data types */
#define HCI_COMMAND_PKT        0x01
#define HCI_ACLDATA_PKT        0x02
#define HCI_SCODATA_PKT        0x03
#define HCI_EVENT_PKT        0x04
#define HCI_VENDOR_PKT        0xff

#define HCI_MAX_NAME_LENGTH        248
#define HCI_MAX_EIR_LENGTH        240

#define HCI_OP_READ_LOCAL_VERSION    0x1001
struct hci_rp_read_local_version {
    __u8     status;
    __u8     hci_ver;
    __le16   hci_rev;
    __u8     lmp_ver;
    __le16   manufacturer;
    __le16   lmp_subver;
} __packed;

#define HCI_EV_CMD_COMPLETE        0x0e
struct hci_ev_cmd_complete {
    __u8     ncmd;
    __le16   opcode;
} __packed;

/* ---- HCI Packet structures ---- */
#define HCI_COMMAND_HDR_SIZE 3
#define HCI_EVENT_HDR_SIZE   2
#define HCI_ACL_HDR_SIZE     4
#define HCI_SCO_HDR_SIZE     3

struct hci_command_hdr {
    __le16    opcode;        /* OCF & OGF */
    __u8    plen;
} __packed;

struct hci_event_hdr {
    __u8    evt;
    __u8    plen;
} __packed;

struct hci_acl_hdr {
    __le16    handle;        /* Handle & Flags(PB, BC) */
    __le16    dlen;
} __packed;

struct hci_sco_hdr {
    __le16    handle;
    __u8    dlen;
} __packed;

static inline struct hci_event_hdr *hci_event_hdr(const struct sk_buff *skb)
{
    return (struct hci_event_hdr *) skb->data;
}

static inline struct hci_acl_hdr *hci_acl_hdr(const struct sk_buff *skb)
{
    return (struct hci_acl_hdr *) skb->data;
}

static inline struct hci_sco_hdr *hci_sco_hdr(const struct sk_buff *skb)
{
    return (struct hci_sco_hdr *) skb->data;
}

/* ---- HCI Ioctl requests structures ---- */
struct hci_dev_stats {
    __u32 err_rx;
    __u32 err_tx;
    __u32 cmd_tx;
    __u32 evt_rx;
    __u32 acl_tx;
    __u32 acl_rx;
    __u32 sco_tx;
    __u32 sco_rx;
    __u32 byte_rx;
    __u32 byte_tx;
};
/* AicSemi - Integrate from hci.h end */

/*****************************************
** AicSemi - Integrate from hci_core.h  **
*****************************************/
struct hci_conn_hash {
    struct list_head list;
    unsigned int     acl_num;
    unsigned int     sco_num;
    unsigned int     le_num;
};

#define HCI_MAX_SHORT_NAME_LENGTH    10

#define NUM_REASSEMBLY 4
struct hci_dev {
    struct mutex    lock;

    char        name[8];
    unsigned long    flags;
    __u16        id;
    __u8        bus;
    __u8        dev_type;

    struct sk_buff        *reassembly[NUM_REASSEMBLY];

    struct hci_conn_hash    conn_hash;

    struct hci_dev_stats    stat;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(3, 4, 0)
    atomic_t        refcnt;
    struct module           *owner;
    void                    *driver_data;
#endif

    atomic_t        promisc;

    struct device        *parent;
    struct device        dev;

    unsigned long        dev_flags;

    int (*open)(struct hci_dev *hdev);
    int (*close)(struct hci_dev *hdev);
    int (*flush)(struct hci_dev *hdev);
    int (*send)(struct sk_buff *skb);
#if LINUX_VERSION_CODE <= KERNEL_VERSION(3, 4, 0)
    void (*destruct)(struct hci_dev *hdev);
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(3, 7, 1)
    __u16               voice_setting;
#endif
    void (*notify)(struct hci_dev *hdev, unsigned int evt);
    int (*ioctl)(struct hci_dev *hdev, unsigned int cmd, unsigned long arg);
	u8 *align_data;
};

#if LINUX_VERSION_CODE <= KERNEL_VERSION(3, 4, 0)
static inline struct hci_dev *__hci_dev_hold(struct hci_dev *d)
{
    atomic_inc(&d->refcnt);
    return d;
}

static inline void __hci_dev_put(struct hci_dev *d)
{
    if (atomic_dec_and_test(&d->refcnt))
        d->destruct(d);
}
#endif

static inline void *hci_get_drvdata(struct hci_dev *hdev)
{
    return dev_get_drvdata(&hdev->dev);
}

static inline void hci_set_drvdata(struct hci_dev *hdev, void *data)
{
    dev_set_drvdata(&hdev->dev, data);
}

#define SET_HCIDEV_DEV(hdev, pdev) ((hdev)->parent = (pdev))


/* ---- HCI Packet structures ---- */
#define HCI_COMMAND_HDR_SIZE 3
#define HCI_EVENT_HDR_SIZE   2
#define HCI_ACL_HDR_SIZE     4
#define HCI_SCO_HDR_SIZE     3

/* -----  HCI Commands ---- */
#define HCI_OP_INQUIRY            0x0401
#define HCI_OP_INQUIRY_CANCEL        0x0402
#define HCI_OP_EXIT_PERIODIC_INQ    0x0404
#define HCI_OP_CREATE_CONN        0x0405
#define HCI_OP_DISCONNECT                0x0406
#define HCI_OP_ADD_SCO            0x0407
#define HCI_OP_CREATE_CONN_CANCEL    0x0408
#define HCI_OP_ACCEPT_CONN_REQ        0x0409
#define HCI_OP_REJECT_CONN_REQ        0x040a
#define HCI_OP_LINK_KEY_REPLY        0x040b
#define HCI_OP_LINK_KEY_NEG_REPLY    0x040c
#define HCI_OP_PIN_CODE_REPLY        0x040d
#define HCI_OP_PIN_CODE_NEG_REPLY    0x040e
#define HCI_OP_CHANGE_CONN_PTYPE    0x040f
#define HCI_OP_AUTH_REQUESTED        0x0411
#define HCI_OP_SET_CONN_ENCRYPT        0x0413
#define HCI_OP_CHANGE_CONN_LINK_KEY    0x0415
#define HCI_OP_REMOTE_NAME_REQ        0x0419
#define HCI_OP_REMOTE_NAME_REQ_CANCEL    0x041a
#define HCI_OP_READ_REMOTE_FEATURES    0x041b
#define HCI_OP_READ_REMOTE_EXT_FEATURES    0x041c
#define HCI_OP_READ_REMOTE_VERSION    0x041d
#define HCI_OP_SETUP_SYNC_CONN        0x0428
#define HCI_OP_ACCEPT_SYNC_CONN_REQ    0x0429
#define HCI_OP_REJECT_SYNC_CONN_REQ    0x042a
#define HCI_OP_SNIFF_MODE        0x0803
#define HCI_OP_EXIT_SNIFF_MODE        0x0804
#define HCI_OP_ROLE_DISCOVERY        0x0809
#define HCI_OP_SWITCH_ROLE        0x080b
#define HCI_OP_READ_LINK_POLICY        0x080c
#define HCI_OP_WRITE_LINK_POLICY    0x080d
#define HCI_OP_READ_DEF_LINK_POLICY    0x080e
#define HCI_OP_WRITE_DEF_LINK_POLICY    0x080f
#define HCI_OP_SNIFF_SUBRATE        0x0811
#define HCI_OP_Write_Link_Policy_Settings 0x080d
#define HCI_OP_SET_EVENT_MASK        0x0c01
#define HCI_OP_RESET            0x0c03
#define HCI_OP_SET_EVENT_FLT        0x0c05
#define HCI_OP_Write_Extended_Inquiry_Response        0x0c52
#define HCI_OP_Write_Simple_Pairing_Mode 0x0c56
#define HCI_OP_Read_Buffer_Size 0x1005
#define HCI_OP_Host_Buffer_Size 0x0c33
#define HCI_OP_Read_Local_Version_Information 0x1001
#define HCI_OP_Read_BD_ADDR 0x1009
#define HCI_OP_Read_Local_Supported_Commands 0x1002
#define HCI_OP_Write_Scan_Enable 0x0c1a
#define HCI_OP_Write_Current_IAC_LAP 0x0c3a
#define HCI_OP_Write_Inquiry_Scan_Activity 0x0c1e
#define HCI_OP_Write_Class_of_Device 0x0c24
#define HCI_OP_LE_Rand 0x2018
#define HCI_OP_LE_Set_Random_Address 0x2005
#define HCI_OP_LE_Set_Extended_Scan_Enable 0x2042
#define HCI_OP_LE_Set_Extended_Scan_Parameters 0x2041
#define HCI_OP_Set_Event_Filter 0x0c05
#define HCI_OP_Write_Voice_Setting 0x0c26
#define HCI_OP_Change_Local_Name 0x0c13
#define HCI_OP_Read_Local_Name 0x0c14
#define HCI_OP_Wirte_Page_Timeout 0x0c18
#define HCI_OP_LE_Clear_Resolving_List 0x0c29
#define HCI_OP_LE_Set_Addres_Resolution_Enable_Command 0x0c2e
#define HCI_OP_Write_Inquiry_mode 0x0c45
#define HCI_OP_Write_Page_Scan_Type 0x0c47
#define HCI_OP_Write_Inquiry_Scan_Type 0x0c43

#define HCI_OP_Delete_Stored_Link_Key 0x0c12
#define HCI_OP_LE_Read_Local_Resolvable_Address 0x202d
#define HCI_OP_LE_Extended_Create_Connection 0x2043
#define HCI_OP_Read_Remote_Version_Information 0x041d
#define HCI_OP_LE_Start_Encryption 0x2019
#define HCI_OP_LE_Add_Device_to_Resolving_List 0x2027
#define HCI_OP_LE_Set_Privacy_Mode 0x204e
#define HCI_OP_LE_Connection_Update 0x2013

/* -----  HCI events---- */
#define HCI_OP_DISCONNECT        0x0406
#define HCI_EV_INQUIRY_COMPLETE        0x01
#define HCI_EV_INQUIRY_RESULT        0x02
#define HCI_EV_CONN_COMPLETE        0x03
#define HCI_EV_CONN_REQUEST            0x04
#define HCI_EV_DISCONN_COMPLETE        0x05
#define HCI_EV_AUTH_COMPLETE        0x06
#define HCI_EV_REMOTE_NAME            0x07
#define HCI_EV_ENCRYPT_CHANGE        0x08
#define HCI_EV_CHANGE_LINK_KEY_COMPLETE    0x09

#define HCI_EV_REMOTE_FEATURES        0x0b
#define HCI_EV_REMOTE_VERSION        0x0c
#define HCI_EV_QOS_SETUP_COMPLETE    0x0d
#define HCI_EV_CMD_COMPLETE            0x0e
#define HCI_EV_CMD_STATUS            0x0f

#define HCI_EV_ROLE_CHANGE            0x12
#define HCI_EV_NUM_COMP_PKTS        0x13
#define HCI_EV_MODE_CHANGE            0x14
#define HCI_EV_PIN_CODE_REQ            0x16
#define HCI_EV_LINK_KEY_REQ            0x17
#define HCI_EV_LINK_KEY_NOTIFY        0x18
#define HCI_EV_CLOCK_OFFSET            0x1c
#define HCI_EV_PKT_TYPE_CHANGE        0x1d
#define HCI_EV_PSCAN_REP_MODE        0x20

#define HCI_EV_INQUIRY_RESULT_WITH_RSSI    0x22
#define HCI_EV_REMOTE_EXT_FEATURES    0x23
#define HCI_EV_SYNC_CONN_COMPLETE    0x2c
#define HCI_EV_SYNC_CONN_CHANGED    0x2d
#define HCI_EV_SNIFF_SUBRATE            0x2e
#define HCI_EV_EXTENDED_INQUIRY_RESULT    0x2f
#define HCI_EV_IO_CAPA_REQUEST        0x31
#define HCI_EV_SIMPLE_PAIR_COMPLETE    0x36
#define HCI_EV_REMOTE_HOST_FEATURES    0x3d
#define HCI_EV_LE_Meta 0x3e

/* ULP Event sub code */
#define HCI_BLE_CONN_COMPLETE_EVT 0x01
#define HCI_BLE_ADV_PKT_RPT_EVT 0x02
#define HCI_BLE_LL_CONN_PARAM_UPD_EVT 0x03
#define HCI_BLE_READ_REMOTE_FEAT_CMPL_EVT 0x04
#define HCI_BLE_LTK_REQ_EVT 0x05
#define HCI_BLE_RC_PARAM_REQ_EVT 0x06
#define HCI_BLE_DATA_LENGTH_CHANGE_EVT 0x07
#define HCI_BLE_ENHANCED_CONN_COMPLETE_EVT 0x0a
#define HCI_BLE_DIRECT_ADV_EVT 0x0b
#define HCI_BLE_PHY_UPDATE_COMPLETE_EVT 0x0c
#define HCI_LE_EXTENDED_ADVERTISING_REPORT_EVT 0x0D
#define HCI_BLE_PERIODIC_ADV_SYNC_EST_EVT      0x0E
#define HCI_BLE_PERIODIC_ADV_REPORT_EVT        0x0F
#define HCI_BLE_PERIODIC_ADV_SYNC_LOST_EVT     0x10
#define HCI_BLE_SCAN_TIMEOUT_EVT               0x11
#define HCI_LE_ADVERTISING_SET_TERMINATED_EVT 0x12
#define HCI_BLE_SCAN_REQ_RX_EVT                0x13
#define HCI_BLE_CIS_EST_EVT 0x19
#define HCI_BLE_CIS_REQ_EVT 0x1a
#define HCI_BLE_CREATE_BIG_CPL_EVT 0x1b
#define HCI_BLE_TERM_BIG_CPL_EVT 0x1c
#define HCI_BLE_BIG_SYNC_EST_EVT 0x1d
#define HCI_BLE_BIG_SYNC_LOST_EVT 0x1e
#define HCI_BLE_REQ_PEER_SCA_CPL_EVT 0x1f

#define HCI_VENDOR_SPECIFIC_EVT 0xFF /* Vendor specific events */

#define CONFIG_MAC_OFFSET_GEN_1_2       (0x3C)      //MAC's OFFSET in config/efuse for aic generation 1~2 bluetooth chip
#define CONFIG_MAC_OFFSET_GEN_3PLUS     (0x44)      //MAC's OFFSET in config/efuse for aic generation 3+ bluetooth chip

//Define ioctl cmd the same as HCIDEVUP in the kernel
#define DOWN_FW_CFG             _IOW('E', 176, int)
//#ifdef CONFIG_SCO_OVER_HCI
//#define SET_ISO_CFG             _IOW('H', 202, int)
//#else
#define SET_ISO_CFG             _IOW('E', 177, int)
//#endif
#define RESET_CONTROLLER        _IOW('E', 178, int)
#define DWFW_CMPLT              _IOW('E', 179, int)

#define GET_USB_INFO            _IOR('E', 180, int)

void bt_data_dump(char* tag, void* data, unsigned long len);
int aic_enqueue(struct sk_buff *skb);
int aic_queue_cnt(void);
int bt_sdio_recv(u8 *data,u32 data_len);


int btchr_init(void);
void btchr_exit(void);
int hdev_init(void);
void hdev_exit(void);


struct hci_dev *hci_dev_get(int index);
int hci_recv_fragment(struct hci_dev *hdev, int type, void *data, int count);

#endif//_AICWF_SDIO_BT_H_


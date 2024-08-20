#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/skbuff.h>

#include <linux/ioctl.h>
#include <linux/io.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#include "aic_btsdio.h"
#include "rwnx_msg_tx.h"

static spinlock_t queue_lock;

static inline struct sk_buff *bt_skb_alloc(unsigned int len, gfp_t how)
{
    struct sk_buff *skb;

    if ((skb = alloc_skb(len + BT_SKB_RESERVE, how))) {
        skb_reserve(skb, BT_SKB_RESERVE);
        bt_cb(skb)->incoming  = 0;
    }
    return skb;
}

static spinlock_t queue_lock;
static spinlock_t dlfw_lock;
static volatile uint16_t    dlfw_dis_state = 0;

/* Global parameters for bt usb char driver */
#define BT_CHAR_DEVICE_NAME "aicbt_dev"
struct mutex btchr_mutex;
static struct sk_buff_head btchr_readq;
static wait_queue_head_t btchr_read_wait;
static wait_queue_head_t bt_dlfw_wait;
static int bt_char_dev_registered;
static dev_t bt_devid; /* bt char device number */
static struct cdev bt_char_dev; /* bt character device structure */
static struct class *bt_char_class; /* device class for usb char driver */
static int bt_reset = 0;
//int aic_queue_cnt(void);
/* HCI device & lock */
DEFINE_RWLOCK(hci_dev_lock);

struct hci_dev *ghdev = NULL;

static struct sk_buff *aic_skb_queue[QUEUE_SIZE];
static int aic_skb_queue_front = 0;
static int aic_skb_queue_rear = 0;

static inline int check_set_dlfw_state_value(uint16_t change_value)
{
    spin_lock(&dlfw_lock);
    if(!dlfw_dis_state) {
        dlfw_dis_state = change_value;
    }
    spin_unlock(&dlfw_lock);
    return dlfw_dis_state;
}

static inline void set_dlfw_state_value(uint16_t change_value)
{
    spin_lock(&dlfw_lock);
    dlfw_dis_state = change_value;
    spin_unlock(&dlfw_lock);
}

static void print_acl(struct sk_buff *skb, int direction)
{
#if PRINT_ACL_DATA
    //uint wlength = skb->len;
    u16 *handle = (u16 *)(skb->data);
    u16 len = *(handle+1);
    //u8 *acl_data = (u8 *)(skb->data);

    AICBT_INFO("aic %s: direction %d, handle %04x, len %d",
            __func__, direction, *handle, len);
#endif
}

static void print_sco(struct sk_buff *skb, int direction)
{
#if PRINT_SCO_DATA
    uint wlength = skb->len;
    u16 *handle = (u16 *)(skb->data);
    u8 len = *(u8 *)(handle+1);
    //u8 *sco_data =(u8 *)(skb->data);

    AICBT_INFO("aic %s: direction %d, handle %04x, len %d,wlength %d",
            __func__, direction, *handle, len,wlength);
#endif
}

int bt_bypass_event(struct sk_buff *skb)
{
	int ret = 0;
	u8 *opcode = (u8*)(skb->data);
	//u8 len = *(opcode+1);
    //printk("bypass_event %x,%x,%x,%x,%x\r\n",opcode[0],opcode[1],opcode[2],opcode[3],opcode[4]);

	switch(opcode[1]) {
        case HCI_EV_LE_Meta:
            {
                u8 subevent_code;
                subevent_code = opcode[3];
                switch(subevent_code){
                    case HCI_BLE_ADV_PKT_RPT_EVT:
                    case HCI_LE_EXTENDED_ADVERTISING_REPORT_EVT:
                    {
                        if(aic_queue_cnt() > (QUEUE_SIZE-490)){
                            printk("more adv report bypass\r\n");
                            ret = 1;
                        }
                    }
                    break;
                }
            }
            break;
		default:
			break;
	}
	return ret;
}

int bt_sdio_recv(u8 *data,u32 data_len)
{
	struct sk_buff *skb;
	int type= data[0];
	struct hci_dev *hdev;
	u32 len = data_len;
	//int ret=0;
	hdev = hci_dev_get(0);
    if (!hdev) {
        AICWFDBG(LOGERROR,"%s: Failed to get hci dev[NULL]", __func__);
        return -ENODEV;
    }
	
	//printk("%s type %x  len %d \n",__func__,type,len);
	//bt_data_dump("bt_recv", ind->bt_data, 64);

	/*#if (CONFIG_BLUEDROID) || (HCI_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
		if (hci_recv_fragment(hdev, type,
						ind->bt_data[0],
						ind->data_len) < 0) {
			AICWFDBG(LOGERROR,"%s: Corrupted event packet", __func__);
			hdev->stat.err_rx++;
		}
	#endif*/
	skb = alloc_skb(len,GFP_ATOMIC);
	if(!skb){
		AICWFDBG(LOGERROR, "alloc skb fail %s \n",__func__);
	}
	memcpy(skb_put(skb,len) ,data, len);
	if(bt_bypass_event(skb)){
		kfree_skb(skb);
		return 0;
	}
	//bt_data_dump("bt_skb", skb, skb->len);
	
	if(aic_enqueue(skb)<0){
		kfree_skb(skb);
	}else{
		//printk("wake up \n");
		wake_up_interruptible(&btchr_read_wait);
	}
	return 0;
}


static int bypass_event(struct sk_buff *skb)
{
	int ret = 0;
	u8 *opcode = (u8*)(skb->data);
	//u8 len = *(opcode+1);
    //printk("bypass_event %x,%x,%x,%x,%x\r\n",opcode[0],opcode[1],opcode[2],opcode[3],opcode[4]);

	switch(*opcode) {
#ifdef CONFIG_SUPPORT_VENDOR_APCF
		case HCI_EV_CMD_COMPLETE:
            {
                u16 sub_opcpde;
                sub_opcpde = ((u16)opcode[3]|(u16)(opcode[4])<<8);
                if(sub_opcpde == 0xfd57){
                    if(vendor_apcf_sent_done){
                        vendor_apcf_sent_done--;
                        printk("apcf bypass\r\n");
                        ret = 1;
                    }
                }
            }
			break;
#endif//CONFIG_SUPPORT_VENDOR_APCF
        case HCI_EV_LE_Meta:
            {
                u8 subevent_code;
                subevent_code = opcode[2];
                switch(subevent_code){
                    case HCI_BLE_ADV_PKT_RPT_EVT:
                    case HCI_LE_EXTENDED_ADVERTISING_REPORT_EVT:
                    {
                        if(aic_queue_cnt() > (QUEUE_SIZE-100)){
                            printk("more adv report bypass\r\n");
                            ret = 1;
                        }
                    }
                    break;
                }
            }
            break;
		default:
			break;
	}
	return ret;
}
static void print_event(struct sk_buff *skb)
{
#if PRINT_CMD_EVENT
    //uint wlength = skb->len;
    //uint icount = 0;
    u8 *opcode = (u8*)(skb->data);
    //u8 len = *(opcode+1);

    printk("aic %s ", __func__);
    switch (*opcode) {
    case HCI_EV_INQUIRY_COMPLETE:
        printk("HCI_EV_INQUIRY_COMPLETE");
        break;
    case HCI_EV_INQUIRY_RESULT:
        printk("HCI_EV_INQUIRY_RESULT");
        break;
    case HCI_EV_CONN_COMPLETE:
        printk("HCI_EV_CONN_COMPLETE");
        break;
    case HCI_EV_CONN_REQUEST:
        printk("HCI_EV_CONN_REQUEST");
        break;
    case HCI_EV_DISCONN_COMPLETE:
        printk("HCI_EV_DISCONN_COMPLETE");
        break;
    case HCI_EV_AUTH_COMPLETE:
        printk("HCI_EV_AUTH_COMPLETE");
        break;
    case HCI_EV_REMOTE_NAME:
        printk("HCI_EV_REMOTE_NAME");
        break;
    case HCI_EV_ENCRYPT_CHANGE:
        printk("HCI_EV_ENCRYPT_CHANGE");
        break;
    case HCI_EV_CHANGE_LINK_KEY_COMPLETE:
        printk("HCI_EV_CHANGE_LINK_KEY_COMPLETE");
        break;
    case HCI_EV_REMOTE_FEATURES:
        printk("HCI_EV_REMOTE_FEATURES");
        break;
    case HCI_EV_REMOTE_VERSION:
        printk("HCI_EV_REMOTE_VERSION");
        break;
    case HCI_EV_QOS_SETUP_COMPLETE:
        printk("HCI_EV_QOS_SETUP_COMPLETE");
        break;
    case HCI_EV_CMD_COMPLETE:
        printk("HCI_EV_CMD_COMPLETE");
        break;
    case HCI_EV_CMD_STATUS:
        printk("HCI_EV_CMD_STATUS");
        break;
    case HCI_EV_ROLE_CHANGE:
        printk("HCI_EV_ROLE_CHANGE");
        break;
    case HCI_EV_NUM_COMP_PKTS:
        printk("HCI_EV_NUM_COMP_PKTS");
        break;
    case HCI_EV_MODE_CHANGE:
        printk("HCI_EV_MODE_CHANGE");
        break;
    case HCI_EV_PIN_CODE_REQ:
        printk("HCI_EV_PIN_CODE_REQ");
        break;
    case HCI_EV_LINK_KEY_REQ:
        printk("HCI_EV_LINK_KEY_REQ");
        break;
    case HCI_EV_LINK_KEY_NOTIFY:
        printk("HCI_EV_LINK_KEY_NOTIFY");
        break;
    case HCI_EV_CLOCK_OFFSET:
        printk("HCI_EV_CLOCK_OFFSET");
        break;
    case HCI_EV_PKT_TYPE_CHANGE:
        printk("HCI_EV_PKT_TYPE_CHANGE");
        break;
    case HCI_EV_PSCAN_REP_MODE:
        printk("HCI_EV_PSCAN_REP_MODE");
        break;
    case HCI_EV_INQUIRY_RESULT_WITH_RSSI:
        printk("HCI_EV_INQUIRY_RESULT_WITH_RSSI");
        break;
    case HCI_EV_REMOTE_EXT_FEATURES:
        printk("HCI_EV_REMOTE_EXT_FEATURES");
        break;
    case HCI_EV_SYNC_CONN_COMPLETE:
        printk("HCI_EV_SYNC_CONN_COMPLETE");
        break;
    case HCI_EV_SYNC_CONN_CHANGED:
        printk("HCI_EV_SYNC_CONN_CHANGED");
        break;
    case HCI_EV_SNIFF_SUBRATE:
        printk("HCI_EV_SNIFF_SUBRATE");
        break;
    case HCI_EV_EXTENDED_INQUIRY_RESULT:
        printk("HCI_EV_EXTENDED_INQUIRY_RESULT");
        break;
    case HCI_EV_IO_CAPA_REQUEST:
        printk("HCI_EV_IO_CAPA_REQUEST");
        break;
    case HCI_EV_SIMPLE_PAIR_COMPLETE:
        printk("HCI_EV_SIMPLE_PAIR_COMPLETE");
        break;
    case HCI_EV_REMOTE_HOST_FEATURES:
        printk("HCI_EV_REMOTE_HOST_FEATURES");
        break;
    default:
        printk("unknow event");
        break;
    }
	printk("\n");
#if 0
    printk("%02x,len:%d,", *opcode,len);
    for (icount = 2; (icount < wlength) && (icount < 24); icount++)
        printk("%02x ", *(opcode+icount));
    printk("\n");
#endif
#endif
}


static inline ssize_t sdio_put_user(struct sk_buff *skb,
        char __user *buf, int count)
{
    char __user *ptr = buf;
    int len = min_t(unsigned int, skb->len, count);

    if (copy_to_user(ptr, skb->data, len))
        return -EFAULT;

    return len;
}

int aic_enqueue(struct sk_buff *skb)
{
	unsigned long flags = 0;
	int ret = 0;
	spin_lock_irqsave(&queue_lock, flags);
    if (aic_skb_queue_front == (aic_skb_queue_rear + 1) % QUEUE_SIZE) {
        /*
         * If queue is full, current solution is to drop
         * the following entries.
         */
        AICBT_WARN("%s: Queue is full, entry will be dropped", __func__);
		ret = -1;
    } else {
        aic_skb_queue[aic_skb_queue_rear] = skb;

        aic_skb_queue_rear++;
        aic_skb_queue_rear %= QUEUE_SIZE;

    }
	spin_unlock_irqrestore(&queue_lock, flags);
	return ret;
}

static struct sk_buff *aic_dequeue_try(unsigned int deq_len)
{
    struct sk_buff *skb;
    struct sk_buff *skb_copy;
	unsigned long flags = 0;
	
	spin_lock_irqsave(&queue_lock, flags);
    if (aic_skb_queue_front == aic_skb_queue_rear) {
        AICBT_WARN("%s: Queue is empty", __func__);
		spin_unlock_irqrestore(&queue_lock, flags);
        return NULL;
    }

    skb = aic_skb_queue[aic_skb_queue_front];
    if (deq_len >= skb->len) {

        aic_skb_queue_front++;
        aic_skb_queue_front %= QUEUE_SIZE;

        /*
         * Return skb addr to be dequeued, and the caller
         * should free the skb eventually.
         */
		spin_unlock_irqrestore(&queue_lock, flags);
        return skb;
    } else {
        skb_copy = pskb_copy(skb, GFP_ATOMIC);
        skb_pull(skb, deq_len);
        /* Return its copy to be freed */
		spin_unlock_irqrestore(&queue_lock, flags);
        return skb_copy;
    }
}

static inline int is_queue_empty(void)
{
    return (aic_skb_queue_front == aic_skb_queue_rear) ? 1 : 0;
}

void aic_clear_queue(void)
{
    struct sk_buff *skb;
	unsigned long flags = 0;
	
	spin_lock_irqsave(&queue_lock, flags);
    while(!is_queue_empty()) {
        skb = aic_skb_queue[aic_skb_queue_front];
        aic_skb_queue[aic_skb_queue_front] = NULL;
        aic_skb_queue_front++;
        aic_skb_queue_front %= QUEUE_SIZE;
        if (skb) {
            kfree_skb(skb);
        }
    }
	spin_unlock_irqrestore(&queue_lock, flags);
}

int aic_queue_cnt(void)
{
    int ret_cnt = 0;
	unsigned long flags = 0;
	
	spin_lock_irqsave(&queue_lock, flags);
    if(is_queue_empty()) {
        ret_cnt = 0;
    }else{
        if(aic_skb_queue_rear > aic_skb_queue_front){
            ret_cnt = aic_skb_queue_rear-aic_skb_queue_front;
        }else{
            ret_cnt = aic_skb_queue_rear+QUEUE_SIZE-aic_skb_queue_front;
        }
    }
	spin_unlock_irqrestore(&queue_lock, flags);
    return ret_cnt;
}

/*
 * AicSemi - Integrate from hci_core.c
 */

/* Get HCI device by index.
 * Device is held on return. */
struct hci_dev *hci_dev_get(int index)
{
    if (index != 0)
        return NULL;

    return ghdev;
}

/* ---- HCI ioctl helpers ---- */
static int hci_dev_open(__u16 dev)
{
    struct hci_dev *hdev;
    int ret = 0;

    AICBT_DBG("%s: dev %d", __func__, dev);

    hdev = hci_dev_get(dev);
    if (!hdev) {
        AICBT_ERR("%s: Failed to get hci dev[Null]", __func__);
        return -ENODEV;
    }

    /*if (test_bit(HCI_UNREGISTER, &hdev->dev_flags)) {
        ret = -ENODEV;
        goto done;
    }

    if (test_bit(HCI_UP, &hdev->flags)) {
        ret = -EALREADY;
        goto done;
    }*/

done:
    return ret;
}

static int hci_dev_do_close(struct hci_dev *hdev)
{
    //if (hdev->flush)
    //    hdev->flush(hdev);
    /* After this point our queues are empty
     * and no tasks are scheduled. */
    //hdev->close(hdev);
    /* Clear flags */
    hdev->flags = 0;
    return 0;
}

static int hci_dev_close(__u16 dev)
{
    struct hci_dev *hdev;
    int err;
    hdev = hci_dev_get(dev);
    if (!hdev) {
        AICBT_ERR("%s: failed to get hci dev[Null]", __func__);
        return -ENODEV;
    }

    err = hci_dev_do_close(hdev);

    return err;
}

#if CONFIG_BLUEDROID
static struct hci_dev *hci_alloc_dev(void)
{
    struct hci_dev *hdev;

    hdev = kzalloc(sizeof(struct hci_dev), GFP_KERNEL);
    if (!hdev)
        return NULL;

    return hdev;
}

/* Free HCI device */
static void hci_free_dev(struct hci_dev *hdev)
{
    kfree(hdev);
}

/* Register HCI device */
static int hci_register_dev(struct hci_dev *hdev)
{
    int i, id;

    AICBT_DBG("%s: %p name %s bus %d", __func__, hdev, hdev->name, hdev->bus);
    /* Do not allow HCI_AMP devices to register at index 0,
     * so the index can be used as the AMP controller ID.
     */
    id = (hdev->dev_type == HCI_BREDR) ? 0 : 1;

    write_lock(&hci_dev_lock);

    sprintf(hdev->name, "hci%d", id);
    hdev->id = id;
    hdev->flags = 0;
    hdev->dev_flags = 0;
    mutex_init(&hdev->lock);

    AICBT_DBG("%s: id %d, name %s", __func__, hdev->id, hdev->name);


    for (i = 0; i < NUM_REASSEMBLY; i++)
        hdev->reassembly[i] = NULL;

    memset(&hdev->stat, 0, sizeof(struct hci_dev_stats));
    atomic_set(&hdev->promisc, 0);

    if (ghdev) {
        write_unlock(&hci_dev_lock);
        AICBT_ERR("%s: Hci device has been registered already", __func__);
        return -1;
    } else
        ghdev = hdev;

    write_unlock(&hci_dev_lock);

    return id;
}

/* Unregister HCI device */
static void hci_unregister_dev(struct hci_dev *hdev)
{
    int i;

    AICBT_DBG("%s: hdev %p name %s bus %d", __func__, hdev, hdev->name, hdev->bus);
    set_bit(HCI_UNREGISTER, &hdev->dev_flags);

    write_lock(&hci_dev_lock);
    ghdev = NULL;
    write_unlock(&hci_dev_lock);

    hci_dev_do_close(hdev);
    for (i = 0; i < NUM_REASSEMBLY; i++)
        kfree_skb(hdev->reassembly[i]);
}

static void hci_send_to_stack(struct hci_dev *hdev, struct sk_buff *skb)
{
    struct sk_buff *aic_skb_copy = NULL;

    //AICBT_DBG("%s", __func__);

    if (!hdev) {
        AICBT_ERR("%s: Frame for unknown HCI device", __func__);
        return;
    }

    if (!test_bit(HCI_RUNNING, &hdev->flags)) {
        AICBT_ERR("%s: HCI not running", __func__);
        return;
    }

    aic_skb_copy = pskb_copy(skb, GFP_ATOMIC);
    if (!aic_skb_copy) {
        AICBT_ERR("%s: Copy skb error", __func__);
        return;
    }

    memcpy(skb_push(aic_skb_copy, 1), &bt_cb(skb)->pkt_type, 1);
    aic_enqueue(aic_skb_copy);

    /* Make sure bt char device existing before wakeup read queue */
    hdev = hci_dev_get(0);
    if (hdev) {
        //AICBT_DBG("%s: Try to wakeup read queue", __func__);
        AICBT_DBG("%s", __func__);
        wake_up_interruptible(&btchr_read_wait);
    }

	
    return;
}

/* Receive frame from HCI drivers */
static int hci_recv_frame(struct sk_buff *skb)
{
    struct hci_dev *hdev = (struct hci_dev *) skb->dev;

    if (!hdev ||
        (!test_bit(HCI_UP, &hdev->flags) && !test_bit(HCI_INIT, &hdev->flags))) {
        kfree_skb(skb);
        return -ENXIO;
    }

    /* Incomming skb */
    bt_cb(skb)->incoming = 1;

    /* Time stamp */
    __net_timestamp(skb);

    if (atomic_read(&hdev->promisc)) {
#ifdef CONFIG_SCO_OVER_HCI
        if(bt_cb(skb)->pkt_type == HCI_SCODATA_PKT){
            hci_send_to_alsa_ringbuffer(hdev, skb);
        }else{
            if(bt_cb(skb)->pkt_type == HCI_EVENT_PKT){
				if(bypass_event(skb)){
					kfree_skb(skb);
					return 0;
				}
			}
			hci_send_to_stack(hdev, skb);
		}
#else
		if(bt_cb(skb)->pkt_type == HCI_EVENT_PKT){
			if(bypass_event(skb)){
				kfree_skb(skb);
				return 0;
			}
		}
		/* Send copy to the sockets */
		hci_send_to_stack(hdev, skb);
#endif

    }

    kfree_skb(skb);
    return 0;
}



static int hci_reassembly(struct hci_dev *hdev, int type, void *data,
                          int count, __u8 index)
{
    int len = 0;
    int hlen = 0;
    int remain = count;
    struct sk_buff *skb;
    struct bt_skb_cb *scb;

    //AICBT_DBG("%s", __func__);

    if ((type < HCI_ACLDATA_PKT || type > HCI_EVENT_PKT) ||
            index >= NUM_REASSEMBLY)
        return -EILSEQ;

    skb = hdev->reassembly[index];

    if (!skb) {
        switch (type) {
        case HCI_ACLDATA_PKT:
            len = HCI_MAX_FRAME_SIZE;
            hlen = HCI_ACL_HDR_SIZE;
            break;
        case HCI_EVENT_PKT:
            len = HCI_MAX_EVENT_SIZE;
            hlen = HCI_EVENT_HDR_SIZE;
            break;
        case HCI_SCODATA_PKT:
            len = HCI_MAX_SCO_SIZE;
            hlen = HCI_SCO_HDR_SIZE;
            break;
        }

        skb = bt_skb_alloc(len, GFP_ATOMIC);
        if (!skb)
            return -ENOMEM;

        scb = (void *) skb->cb;
        scb->expect = hlen;
        scb->pkt_type = type;

        skb->dev = (void *) hdev;
        hdev->reassembly[index] = skb;
    }

    while (count) {
        scb = (void *) skb->cb;
        len = min_t(uint, scb->expect, count);

        memcpy(skb_put(skb, len), data, len);

        count -= len;
        data += len;
        scb->expect -= len;
        remain = count;

        switch (type) {
        case HCI_EVENT_PKT:
            if (skb->len == HCI_EVENT_HDR_SIZE) {
                struct hci_event_hdr *h = hci_event_hdr(skb);
                scb->expect = h->plen;

                if (skb_tailroom(skb) < scb->expect) {
                    kfree_skb(skb);
                    hdev->reassembly[index] = NULL;
                    return -ENOMEM;
                }
            }
            break;

        case HCI_ACLDATA_PKT:
            if (skb->len  == HCI_ACL_HDR_SIZE) {
                struct hci_acl_hdr *h = hci_acl_hdr(skb);
                scb->expect = __le16_to_cpu(h->dlen);

                if (skb_tailroom(skb) < scb->expect) {
                    kfree_skb(skb);
                    hdev->reassembly[index] = NULL;
                    return -ENOMEM;
                }
            }
            break;

        case HCI_SCODATA_PKT:
            if (skb->len == HCI_SCO_HDR_SIZE) {
                struct hci_sco_hdr *h = hci_sco_hdr(skb);
                scb->expect = h->dlen;

                if (skb_tailroom(skb) < scb->expect) {
                    kfree_skb(skb);
                    hdev->reassembly[index] = NULL;
                    return -ENOMEM;
                }
            }
            break;
        }

        if (scb->expect == 0) {
            /* Complete frame */
            if(HCI_ACLDATA_PKT == type)
                print_acl(skb,0);
            if(HCI_SCODATA_PKT == type)
                print_sco(skb,0);
            if(HCI_EVENT_PKT == type)
                print_event(skb);

            bt_cb(skb)->pkt_type = type;
            hci_recv_frame(skb);

            hdev->reassembly[index] = NULL;
            return remain;
        }
    }

    return remain;
}

int hci_recv_fragment(struct hci_dev *hdev, int type, void *data, int count)
{
    int rem = 0;

    if (type < HCI_ACLDATA_PKT || type > HCI_EVENT_PKT)
        return -EILSEQ;

    while (count) {
        rem = hci_reassembly(hdev, type, data, count, type - 1);
        if (rem < 0)
            return rem;

        data += (count - rem);
        count = rem;
    }

    return rem;
}
#endif //CONFIG_BLUEDROID

static int btchr_open(struct inode *inode_p, struct file  *file_p)
{
    struct btusb_data *data;
    struct hci_dev *hdev;

    AICBT_DBG("%s: BT sdio char device is opening", __func__);
    /* Not open unless wanna tracing log */
    /* trace_printk("%s: open....\n", __func__); */

    hdev = hci_dev_get(0);
    if (!hdev) {
        AICBT_DBG("%s: Failed to get hci dev[NULL]", __func__);
        return -ENODEV;
    }
    data = GET_DRV_DATA(hdev);

    atomic_inc(&hdev->promisc);
    /*
     * As bt device is not re-opened when hotplugged out, we cannot
     * trust on file's private data(may be null) when other file ops
     * are invoked.
     */
    file_p->private_data = data;

    mutex_lock(&btchr_mutex);
    hci_dev_open(0);
    mutex_unlock(&btchr_mutex);

    aic_clear_queue();
    return nonseekable_open(inode_p, file_p);
}

static int btchr_close(struct inode  *inode_p, struct file   *file_p)
{
    struct btusb_data *data;
    struct hci_dev *hdev;

    AICBT_INFO("%s: BT sdio char device is closing", __func__);
    /* Not open unless wanna tracing log */
    /* trace_printk("%s: close....\n", __func__); */

    data = file_p->private_data;
    file_p->private_data = NULL;

#if CONFIG_BLUEDROID
    /*
     * If the upper layer closes bt char interfaces, no reset
     * action required even bt device hotplugged out.
     */
    bt_reset = 0;
#endif

    hdev = hci_dev_get(0);
    if (hdev) {
        atomic_set(&hdev->promisc, 0);
        mutex_lock(&btchr_mutex);
        hci_dev_close(0);
        mutex_unlock(&btchr_mutex);
    }

    return 0;
}

void bt_data_dump(char* tag, void* data, unsigned long len){
	unsigned long i = 0;
	uint8_t* data_ = (uint8_t* )data;

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

static ssize_t btchr_read(struct file *file_p,
        char __user *buf_p,
        size_t count,
        loff_t *pos_p)
{
    struct hci_dev *hdev;
    struct sk_buff *skb;
    ssize_t ret = 0;

    while (count) {
#if 1
		hdev = hci_dev_get(0);
        if (!hdev) {
            /*
             * Note: Only when BT device hotplugged out, we wil get
             * into such situation. In order to keep the upper layer
             * stack alive (blocking the read), we should never return
             * EFAULT or break the loop.
             */
            AICBT_ERR("%s: Failed to get hci dev[Null]", __func__);
        }
#endif
        ret = wait_event_interruptible(btchr_read_wait, !is_queue_empty());
        if (ret < 0) {
            AICBT_ERR("%s: wait event is signaled %d", __func__, (int)ret);
            break;
        }

        skb = aic_dequeue_try(count);
		//bt_data_dump("btchr_read", skb->data, skb->len);
		//printk("btchr_read \n");
        if (skb) {
            ret = sdio_put_user(skb, buf_p, count);
            if (ret < 0)
                AICBT_ERR("%s: Failed to put data to user space", __func__);
            kfree_skb(skb);
            break;
        }
    }

    return ret;
}

#ifdef CONFIG_SUPPORT_VENDOR_APCF
void btchr_external_write(char* buff, int len){
	struct hci_dev *hdev;
	struct sk_buff *skb;
	int i;
	struct btusb_data *data;

	AICBT_INFO("%s \r\n", __func__);
	for(i=0;i<len;i++){
		printk("0x%x ",(u8)buff[i]);
	}
	printk("\r\n");
	hdev = hci_dev_get(0);
	if (!hdev) {
		AICBT_WARN("%s: Failed to get hci dev[Null]", __func__);
		return;
	}
    /* Never trust on btusb_data, as bt device may be hotplugged out */
    data = GET_DRV_DATA(hdev);
    if (!data) {
        AICBT_WARN("%s: Failed to get bt sdio driver data[Null]", __func__);
        return;
    }
    vendor_apcf_sent_done++;

	skb = bt_skb_alloc(len, GFP_ATOMIC);
    if (!skb)
        return;
    skb_reserve(skb, -1); // Add this line
    skb->dev = (void *)hdev;
	memcpy((__u8 *)skb->data,(__u8 *)buff,len);
	skb_put(skb, len);
    bt_cb(skb)->pkt_type = *((__u8 *)skb->data);
    skb_pull(skb, 1);
    data->hdev->send(skb);
}

EXPORT_SYMBOL(btchr_external_write);
#endif //CONFIG_SUPPORT_VENDOR_APCF

//extern struct rwnx_plat *g_rwnx_plat;
static ssize_t btchr_write(struct file *file_p,
        const char __user *buf_p,
        size_t count,
        loff_t *pos_p)
{
    struct btusb_data *data = file_p->private_data;
    struct hci_dev *hdev;
    struct sk_buff *skb;
	int err=0;

    AICBT_DBG("%s", __func__);

    hdev = hci_dev_get(0);
    if (!hdev) {
        AICBT_WARN("%s: Failed to get hci dev[Null]", __func__);
        /*
         * Note: we bypass the data from the upper layer if bt device
         * is hotplugged out. Fortunatelly, H4 or H5 HCI stack does
         * NOT check btchr_write's return value. However, returning
         * count instead of EFAULT is preferable.
         */
        /* return -EFAULT; */
        return count;
    }

#if 0
    /* Never trust on btusb_data, as bt device may be hotplugged out */
    data = GET_DRV_DATA(hdev);
    if (!data) {
        AICBT_WARN("%s: Failed to get bt usb driver data[Null]", __func__);
        return count;
    }
#endif

    if (count > HCI_MAX_FRAME_SIZE)
        return -EINVAL;

    skb = bt_skb_alloc(count, GFP_ATOMIC);
    if (!skb)
        return -ENOMEM;
    skb_reserve(skb, -1); // Add this line

    if (copy_from_user(skb_put(skb, count), buf_p, count)) {
        AICBT_ERR("%s: Failed to get data from user space", __func__);
        kfree_skb(skb);
        return -EFAULT;
    }

    skb->dev = (void *)hdev;
    bt_cb(skb)->pkt_type = *((__u8 *)skb->data);
    //skb_pull(skb, 1);
    //data->hdev->send(skb);
	
	//bt_data_dump("btwrite", skb->data, skb->len);
	err = rwnx_sdio_bt_send_req(g_rwnx_plat->sdiodev->rwnx_hw, skb->len, skb);
	if(err<0){
		printk("%s rwnx_sdio_bt_send_req error %d",__func__,err);
	}

	kfree_skb(skb);
    return count;
}

static unsigned int btchr_poll(struct file *file_p, poll_table *wait)
{
    struct btusb_data *data = file_p->private_data;
    struct hci_dev *hdev;

    //AICBT_DBG("%s: BT sdio char device is polling", __func__);

    /*if(!bt_char_dev_registered) {
        AICBT_ERR("%s: char device has not registered!", __func__);
        return POLLERR | POLLHUP;
    }*/
	//printk("poll wait\n");
    poll_wait(file_p, &btchr_read_wait, wait);
	//printk("poll out\n");

    hdev = hci_dev_get(0);
    if (!hdev) {
        AICBT_ERR("%s: Failed to get hci dev[Null]", __func__);
        //mdelay(URB_CANCELING_DELAY_MS);
        return POLLERR | POLLHUP;
        return POLLOUT | POLLWRNORM;
    }

#if 0
    /* Never trust on btusb_data, as bt device may be hotplugged out */
    data = GET_DRV_DATA(hdev);
    if (!data) {
        /*
         * When bt device is hotplugged out, btusb_data will
         * be freed in disconnect.
         */
        AICBT_ERR("%s: Failed to get bt sdio driver data[Null]", __func__);
        mdelay(URB_CANCELING_DELAY_MS);
        return POLLOUT | POLLWRNORM;
    }
#endif

    if (!is_queue_empty())
        return POLLIN | POLLRDNORM;

    return POLLOUT | POLLWRNORM;
}
static long btchr_ioctl(struct file *file_p,unsigned int cmd, unsigned long arg)
{
    int ret = 0;
    struct hci_dev *hdev;
    struct btusb_data *data;
    //firmware_info *fw_info;

    /*if(!bt_char_dev_registered) {
        return -ENODEV;
    }*/

	printk("%s cmd support %d \n",__func__,cmd);

#if 1
    if(check_set_dlfw_state_value(1) != 1) {
        AICBT_ERR("%s bt controller is disconnecting!", __func__);
        return 0;
    }

    hdev = hci_dev_get(0);
    if(!hdev) {
        AICBT_ERR("%s device is NULL!", __func__);
        set_dlfw_state_value(0);
        return 0;
    }
    //data = GET_DRV_DATA(hdev);
    //fw_info = data->fw_info;

    AICBT_INFO(" btchr_ioctl DOWN_FW_CFG with Cmd:%d",cmd);
    switch (cmd) {
        case DOWN_FW_CFG:
            AICBT_INFO(" btchr_ioctl DOWN_FW_CFG");
            /*ret = usb_autopm_get_interface(data->intf);
            if (ret < 0){
                goto failed;
            }*/

            //ret = download_patch(fw_info,1);
            /*usb_autopm_put_interface(data->intf);
            if(ret < 0){
                AICBT_ERR("%s:Failed in download_patch with ret:%d",__func__,ret);
                goto failed;
            }

            ret = hdev->open(hdev);
            if(ret < 0){
                AICBT_ERR("%s:Failed in hdev->open(hdev):%d",__func__,ret);
                goto failed;
            }*/
            set_bit(HCI_UP, &hdev->flags);
            set_dlfw_state_value(0);
            wake_up_interruptible(&bt_dlfw_wait);
            return 1;
        case DWFW_CMPLT:
            AICBT_INFO(" btchr_ioctl DWFW_CMPLT");
#if 1
	case SET_ISO_CFG:
            AICBT_INFO("btchr_ioctl SET_ISO_CFG");
		if(copy_from_user(&(hdev->voice_setting), (__u16*)arg, sizeof(__u16))){
			AICBT_INFO(" voice settings err");
		}
		//hdev->voice_setting = *(uint16_t*)arg;
		AICBT_INFO(" voice settings = %d", hdev->voice_setting);
		//return 1;
#endif
        case GET_USB_INFO:
			//ret = download_patch(fw_info,1);
            AICBT_INFO(" btchr_ioctl GET_USB_INFO");
            /*ret = hdev->open(hdev);
            if(ret < 0){
                AICBT_ERR("%s:Failed in hdev->open(hdev):%d",__func__,ret);
                //goto done;
            }*/
            set_bit(HCI_UP, &hdev->flags);
            set_dlfw_state_value(0);
            wake_up_interruptible(&bt_dlfw_wait);
            return 1;
        case RESET_CONTROLLER:
            AICBT_INFO(" btchr_ioctl RESET_CONTROLLER");
            //reset_controller(fw_info);
            return 1;
        default:
            AICBT_ERR("%s:Failed with wrong Cmd:%d",__func__,cmd);
            goto failed;
        }
    failed:
        set_dlfw_state_value(0);
        wake_up_interruptible(&bt_dlfw_wait);
        return ret;
#endif
}

#ifdef CONFIG_PLATFORM_UBUNTU//AIDEN
typedef u32		compat_uptr_t;
static inline void __user *compat_ptr(compat_uptr_t uptr)
{
	return (void __user *)(unsigned long)uptr;
}
#endif

#ifdef CONFIG_COMPAT
static long compat_btchr_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
    AICBT_DBG("%s: enter",__func__);
    return btchr_ioctl(filp, cmd, (unsigned long) compat_ptr(arg));
}
#endif
static struct file_operations bt_chrdev_ops  = {
    open    :    btchr_open,
    release    :    btchr_close,
    read    :    btchr_read,
    write    :    btchr_write,
    poll    :    btchr_poll,
    unlocked_ioctl   :   btchr_ioctl,
#ifdef CONFIG_COMPAT
    compat_ioctl :  compat_btchr_ioctl,
#endif
};

int btchr_init()
{
    int res = 0;
    struct device *dev;

    AICBT_INFO("Register sdio char device interface for BT driver");
    /*
     * btchr mutex is used to sync between
     * 1) downloading patch and opening bt char driver
     * 2) the file operations of bt char driver
     */
    mutex_init(&btchr_mutex);

    skb_queue_head_init(&btchr_readq);
    init_waitqueue_head(&btchr_read_wait);
    init_waitqueue_head(&bt_dlfw_wait);

    bt_char_class = class_create(THIS_MODULE, BT_CHAR_DEVICE_NAME);
    if (IS_ERR(bt_char_class)) {
        AICBT_ERR("Failed to create bt char class");
        return PTR_ERR(bt_char_class);
    }

    res = alloc_chrdev_region(&bt_devid, 0, 1, BT_CHAR_DEVICE_NAME);
    if (res < 0) {
        AICBT_ERR("Failed to allocate bt char device");
        goto err_alloc;
    }

    dev = device_create(bt_char_class, NULL, bt_devid, NULL, BT_CHAR_DEVICE_NAME);
    if (IS_ERR(dev)) {
        AICBT_ERR("Failed to create bt char device");
        res = PTR_ERR(dev);
        goto err_create;
    }

    cdev_init(&bt_char_dev, &bt_chrdev_ops);
    res = cdev_add(&bt_char_dev, bt_devid, 1);
    if (res < 0) {
        AICBT_ERR("Failed to add bt char device");
        goto err_add;
    }

    return 0;

err_add:
    device_destroy(bt_char_class, bt_devid);
err_create:
    unregister_chrdev_region(bt_devid, 1);
err_alloc:
    class_destroy(bt_char_class);
    return res;
}

void btchr_exit(void)
{
    AICBT_INFO("Unregister sdio char device interface for BT driver");

    device_destroy(bt_char_class, bt_devid);
    cdev_del(&bt_char_dev);
    unregister_chrdev_region(bt_devid, 1);
    class_destroy(bt_char_class);

    return;
}

int hdev_init(void)
{
	struct hci_dev *hdev;
	int err=0;
	hdev = hci_alloc_dev();

	err = hci_register_dev(hdev);
	if (err < 0) {
        hci_free_dev(hdev);
        hdev = NULL;
        return err;
    }

	spin_lock_init(&queue_lock);

	return 0;
}

void hdev_exit(void)
{
	struct hci_dev *hdev;
	hdev = ghdev;
	hci_unregister_dev(hdev);
	hci_free_dev(hdev);
}


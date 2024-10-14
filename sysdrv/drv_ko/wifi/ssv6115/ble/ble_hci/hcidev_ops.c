/*
 * Copyright (c) 2022 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file hcidev_ops.c
 * @brief HCI device ops functions.
 */

/*******************************************************************************
 *         Include Files
 ******************************************************************************/

#include <linux/delay.h>
#include <linux/version.h>

#include <net/bluetooth/bluetooth.h>
#include <net/bluetooth/hci_core.h>

#include "ipc_msg.h"
#include "hci/drv_hci_ops.h"
#include "hcidev_ops.h"
#include "ble_hci_msg.h"
#include "ssv_debug.h"
 /*******************************************************************************
  *         Local Defines
  ******************************************************************************/
 
 
 /*******************************************************************************
  *         Local Enumerations
  ******************************************************************************/
 
 
 /*******************************************************************************
  *         Local Structures
  ******************************************************************************/
 
 
 /*******************************************************************************
  *         Global Variables
  ******************************************************************************/
 
 /*******************************************************************************
  *         Local Variables
  ******************************************************************************/
 
 /*******************************************************************************
  *         Local Functions
  ******************************************************************************/
 
static int ssv_ble_hci_flush(struct hci_dev *hdev)
 {
     struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
     
     SSV_LOG_DBG("\33[32m%s():%d\33[0m\r\n",__FUNCTION__ ,__LINE__);
     skb_queue_purge(&sbsc->ble_tx_queue);    
     return 0;
 }
 
 
 static int ssv_ble_hci_open(struct hci_dev *hdev)
 {
     struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
     
     SSV_LOG_DBG("\33[32m%s():%d \33[0m\r\n",__FUNCTION__ ,__LINE__);
     ssv_drv_hci_ble_start(sbsc->hci_priv, sbsc->hci_ops);
     ssv_ble_hci_send_bleinit(sbsc, 0);
     return 0;
 }
 
 
 static int ssv_ble_hci_close(struct hci_dev *hdev)
 {   
     struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
     
     SSV_LOG_DBG("\33[32m%s():%d \33[0m\r\n",__FUNCTION__ ,__LINE__);
     ssv_drv_hci_ble_stop(sbsc->hci_priv, sbsc->hci_ops);
     ssv_drv_hci_ble_txq_flush(sbsc->hci_priv, sbsc->hci_ops);
     skb_queue_purge(&sbsc->ble_tx_queue);    
     return 0;
 }

static int ssv_ble_process_unknow_hci_opcode(struct hci_dev *hdev, u16 opcode, struct sk_buff *skb)
 {
    static u8 ble_rx_packet[512]={0};
    
    //struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
    struct sk_buff   *rskb = NULL;
    
    rskb = bt_skb_alloc(sizeof(ble_rx_packet), GFP_KERNEL);
    if(NULL==rskb)
    {
        SSV_LOG_DBG("allocate skb for unknow blc hci cmd fail\n");
        return -1;
    }
    
    memset(ble_rx_packet,0,sizeof(ble_rx_packet));
    switch(opcode)
    {
        case 0xc14:
             ble_rx_packet[0]=0x04;
             ble_rx_packet[1]=0x0e;
             ble_rx_packet[2]=0xfc;
             ble_rx_packet[3]=0x01;
             ble_rx_packet[4]=0x14;
             ble_rx_packet[5]=0x0c;
             ble_rx_packet[6]=0x00;
             ble_rx_packet[7]=0x75;
             ble_rx_packet[8]=0x62;
             ble_rx_packet[9]=0x75;
             ble_rx_packet[10]=0x6e;
             ble_rx_packet[11]=0x74;
             ble_rx_packet[12]=0x75;
             ble_rx_packet[13]=0x2d;
             ble_rx_packet[14]=0x30;
        break;
        case 0xc23:
             ble_rx_packet[0]=0x04;
             ble_rx_packet[1]=0x0e;
             ble_rx_packet[2]=0x07;
             ble_rx_packet[3]=0x02;
             ble_rx_packet[4]=0x23;
             ble_rx_packet[5]=0x0c;
             ble_rx_packet[6]=0x00;
             ble_rx_packet[7]=0x00;
             ble_rx_packet[8]=0x01;
             ble_rx_packet[9]=0x6c;
            break;
        default:
            SSV_LOG_DBG("host driver doesn't implement this unknow hci cmd\r\n");
            return -1;           
     }
     
     rskb->len=skb->data_len = ble_rx_packet[2]+2;
     memcpy(rskb->data,&ble_rx_packet[1],ble_rx_packet[2]+2);
     bt_cb(rskb)->pkt_type = ble_rx_packet[0];

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
     hci_recv_frame(hdev,rskb);
#else
     rskb->dev = (void *)hdev;
     hci_recv_frame(rskb);
#endif

     return 0;
 } 
 /*******************************************************************************
  *         Global Functions
  ******************************************************************************/

 

int ssv_ble_hci_recv_packet(struct ssv_ble_softc *sbsc, u8 *rx_packet, u32 rx_len)
{
#if (CONFIG_BLE_HCI_BUS == SSV_BLE_HCI_OVER_HWIF)
        struct sk_buff *bskb = NULL;
#endif
        if(rx_packet[0]==0x04) //Packet Type
        {
            switch(rx_packet[1])
            {
                case 0x3E: //LE Event Code
                    //sub event code
                    switch(rx_packet[3])
                    {
                        case 0x01:SSV_LOG_DBG("BLE RX: LE Connection Complete. len=%d\n",rx_len); break;
                        case 0x02:SSV_LOG_DBG("BLE RX: LE Advertising Report.  len=%d\n",rx_len); break;
                        case 0x03:SSV_LOG_DBG("BLE RX: LE Connection Update Complete. len=%d\n",rx_len); break;
                        case 0x04:SSV_LOG_DBG("BLE RX: LE Read Remote Used Features Complete. len=%d\n",rx_len); break;
                        case 0x05:SSV_LOG_DBG("BLE RX: LE Long Term Key Requested. len=%d\n",rx_len); break;
                        case 0x06:SSV_LOG_DBG("BLE RX: LE Remote Connection Parameter Request. len=%d\n",rx_len); break;
                        case 0x07:SSV_LOG_DBG("BLE RX: LE Data Length Change. len=%d\n",rx_len); break;
                        case 0x08:SSV_LOG_DBG("BLE RX: LE Read Local P256 Public Key Complete. len=%d\n",rx_len); break;
                        case 0x09:SSV_LOG_DBG("BLE RX: LE Generate DHKey Complete. len=%d\n",rx_len); break;
                        case 0x0A:SSV_LOG_DBG("BLE RX: LE Enhanced Connection Complete. len=%d\n",rx_len); break;
                        case 0x0B:SSV_LOG_DBG("BLE RX: LE Direct Advertising Report. len=%d\n",rx_len); break;
                        default:SSV_LOG_DBG("BLE RX: Unknow sub event (%d)\n",rx_packet[3]); break;
                    }
                    break;
                //BT Event code
                case 0x05:SSV_LOG_DBG("BLE RX: Disconnection Complete. len=%d\n",rx_len); break;
                case 0x08:SSV_LOG_DBG("BLE RX: Encryption Change. len=%d\n",rx_len); break;
                case 0x0C:SSV_LOG_DBG("BLE RX: Read Remote Version Information Complete. len=%d\n",rx_len); break;
                case 0x0E:SSV_LOG_DBG("BLE RX: Command Complete. len=%d\n",rx_len); break;
                case 0x0F:SSV_LOG_DBG("BLE RX: Command Status. len=%d\n",rx_len); break;
                case 0x10:SSV_LOG_DBG("BLE RX: Hardware Error. len=%d\n",rx_len); break;
                case 0x13:SSV_LOG_DBG("BLE RX: Number Of Completed Packets. len=%d\n",rx_len); break;
                case 0x1A:SSV_LOG_DBG("BLE RX: Data Buffer Overflow. len=%d\n",rx_len); break;
                case 0x30:SSV_LOG_DBG("BLE RX: Encryption Key Refresh Complete. len=%d\n",rx_len); break;
                case 0x57:SSV_LOG_DBG("BLE RX: Authenticated Payload Timeout Expired. len=%d\n",rx_len); break;
                default:SSV_LOG_DBG("BLE RX: Unknow event (%d)\n",rx_packet[3]); break;
            }
        }
#if (CONFIG_BLE_HCI_BUS == SSV_BLE_HCI_OVER_HWIF)
        /* A SDIO packet is exactly containing a Bluetooth packet */
        bskb = bt_skb_alloc(rx_len, GFP_KERNEL);
        if (!bskb)
        {
            SSV_LOG_DBG("hci recv:no mem alloc\n");
            return -ENOMEM;
        }
        skb_put(bskb, rx_len);
        memcpy(bskb->data, rx_packet, rx_len);
        bt_cb(bskb)->pkt_type = rx_packet[0];
        skb_pull(bskb,1);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
        hci_recv_frame(sbsc->hdev, bskb);
#else
        bskb->dev = (void *)sbsc->hdev;
        hci_recv_frame(bskb);
#endif
#endif
    return 0;
}


#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
int ssv_ble_hci_send_frame(struct hci_dev *hdev, struct sk_buff *skb)
#else
int ssv_ble_hci_send_frame(struct sk_buff *skb)
#endif
 {
     #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
     struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
     #else
     struct hci_dev *hdev = (struct hci_dev *)(skb->dev);
     struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
     #endif
     
     u16 opcode=0;
     u16 unknow_opcode=0;
     if (hdev == NULL) {
         SSV_LOG_DBG("\33[32mssv_ble_hci_send_frame ssv_hdev is NULL return \33[0m\r\n");
         return 0;
     }

     switch (bt_cb(skb)->pkt_type) 
     {
         case HCI_COMMAND_PKT:
             hdev->stat.cmd_tx++;
             //SSV_LOG_DBG("\33[32mHCI_COMMAND_PKT \33[0m\r\n");
             break;
         case HCI_ACLDATA_PKT:
             hdev->stat.acl_tx++;
             //SSV_LOG_DBG("\33[32mHCI_COMMAND_PKT \33[0m\r\n");
             break;
         case HCI_SCODATA_PKT:
             hdev->stat.sco_tx++;
             //SSV_LOG_DBG("\33[32mHCI_SCODATA_PKT \33[0m\r\n");
             break;
         default:
             break;
     }
     opcode=(skb->data[0]|(skb->data[1]<<8));
     //SSV_LOG_DBG("opcode=0x%x\n",opcode);
     if((bt_cb(skb)->pkt_type)==HCI_COMMAND_PKT)
     {
         switch(opcode)
         {
             case 0x0406:SSV_LOG_DBG("BLE TX: Disconnect\n");break;
             case 0x041D:SSV_LOG_DBG("BLE TX: Read Remote Version Information\n");break;
             case 0x0C01:SSV_LOG_DBG("BLE TX: Set Event Mask\n");break;
             case 0x0C03:SSV_LOG_DBG("BLE TX: Reset\n");break;
             case 0x0C2D:SSV_LOG_DBG("BLE TX: Read Transmit Power Level\n");break;
             case 0x0C31:SSV_LOG_DBG("BLE TX: Set Controller To Host Flow Control (optional)\n");break;
             case 0x0C33:SSV_LOG_DBG("BLE TX: Host Buffer Size (optional)\n");break;
             case 0x0C35:SSV_LOG_DBG("BLE TX: Host Number Of Completed Packets (optional)\n");break;
             case 0x0C63:SSV_LOG_DBG("BLE TX: Set Event Mask Page\n");break;
             case 0x0C7B:SSV_LOG_DBG("BLE TX: Read Authenticated Payload Timeout\n");break;
             case 0x0C7C:SSV_LOG_DBG("BLE TX: Write Authenticated Payload Timeout\n");break;
             case 0x1001:SSV_LOG_DBG("BLE TX: Read Local Version Information\n");break;
             case 0x1002:SSV_LOG_DBG("BLE TX: Read Local Supported Commands -optional-\n");break;
             case 0x1003:SSV_LOG_DBG("BLE TX: Read Local Supported Features\n");break;
             case 0x1009:SSV_LOG_DBG("BLE TX: Read BD_ADDR\n");break;
             case 0x1405:SSV_LOG_DBG("BLE TX: Read RSSI\n");break;
             case 0x2001:SSV_LOG_DBG("BLE TX: LE Set Event Mask\n");break;
             case 0x2002:SSV_LOG_DBG("BLE TX: LE Read Buffer Size\n");break;
             case 0x2003:SSV_LOG_DBG("BLE TX: LE Read Local Supported Features\n");break;
             case 0x2005:SSV_LOG_DBG("BLE TX: LE Set Random Address\n");break;
             case 0x2006:SSV_LOG_DBG("BLE TX: LE Set Advertising Parameters\n");break;
             case 0x2007:SSV_LOG_DBG("BLE TX: LE Read Advertising Channel TX Power\n");break;
             case 0x2008:SSV_LOG_DBG("BLE TX: LE Set Advertising Data\n");break;
             case 0x2009:SSV_LOG_DBG("BLE TX: LE Set Scan Response Data\n");break;
             case 0x200A:SSV_LOG_DBG("BLE TX: LE Set Advertise Enable\n");break;
             case 0x200B:SSV_LOG_DBG("BLE TX: LE Set Scan Parameters\n");break;
             case 0x200C:SSV_LOG_DBG("BLE TX: LE Set Scan Enable\n");break;
             case 0x200D:SSV_LOG_DBG("BLE TX: LE Create Connection\n");break;
             case 0x200E:SSV_LOG_DBG("BLE TX: LE Create Connection Cancel\n");break;
             case 0x200F:SSV_LOG_DBG("BLE TX: LE Read White List Size\n");break;
             case 0x2010:SSV_LOG_DBG("BLE TX: LE Clear White Lis\n");break;
             case 0x2011:SSV_LOG_DBG("BLE TX: LE Add Device To White List\n");break;
             case 0x2012:SSV_LOG_DBG("BLE TX: LE Remove Device From White List\n");break;
             case 0x2013:SSV_LOG_DBG("BLE TX: LE Connection Update\n");break;
             case 0x2014:SSV_LOG_DBG("BLE TX: LE Set Host Channel Classification\n");break;
             case 0x2015:SSV_LOG_DBG("BLE TX: LE Read Channel Map\n");break;
             case 0x2016:SSV_LOG_DBG("BLE TX: LE Read Remote Used Features\n");break;
             case 0x2017:SSV_LOG_DBG("BLE TX: LE Encrypt\n");break;
             case 0x2018:SSV_LOG_DBG("BLE TX: LE Rand\n");break;
             case 0x2019:SSV_LOG_DBG("BLE TX: LE Start Encryption\n");break;
             case 0x201A:SSV_LOG_DBG("BLE TX: LE Long Term Key Requested Reply\n");break;
             case 0x201B:SSV_LOG_DBG("BLE TX: LE Long Term Key Requested Negative Reply\n");break;
             case 0x201C:SSV_LOG_DBG("BLE TX: LE Read Supported States\n");break;
             case 0x201D:SSV_LOG_DBG("BLE TX: LE Receiver Test\n");break;
             case 0x201E:SSV_LOG_DBG("BLE TX: LE Transmitter Test\n");break;
             case 0x201F:SSV_LOG_DBG("BLE TX: LE Test End Command\n");break;
             case 0x2020:SSV_LOG_DBG("BLE TX: LE Remote Connection Parameter Request Reply\n");break;
             case 0x2021:SSV_LOG_DBG("BLE TX: LE Remote Connection Parameter Request Negative Reply\n");break;
             case 0x2022:SSV_LOG_DBG("BLE TX: LE Set Data Length\n");break;
             case 0x2023:SSV_LOG_DBG("BLE TX: LE Read Suggested Default Data Length\n");break;
             case 0x2024:SSV_LOG_DBG("BLE TX: LE Write Suggested Default Data Length\n");break;
             case 0x2026:SSV_LOG_DBG("BLE TX: LE Read Local P256 Public Key 37 0x2025 LE Generate DHKey\n");break;
             case 0x2027:SSV_LOG_DBG("BLE TX: LE Add Device to Resolving List\n");break;
             case 0x2028:SSV_LOG_DBG("BLE TX: LE Remove Device from Resolving List\n");break;
             case 0x2029:SSV_LOG_DBG("BLE TX: LE Clear Resolving List\n");break;
             case 0x202A:SSV_LOG_DBG("BLE TX: LE Read Resolving List Size\n");break;
             case 0x202B:SSV_LOG_DBG("BLE TX: LE Read Peer Resolvable Address\n");break;
             case 0x202C:SSV_LOG_DBG("BLE TX: LE Read Local Resolvable Address\n");break;
             case 0x202D:SSV_LOG_DBG("BLE TX: LE Set Address Resolution Enable\n");break;
             case 0x202E:SSV_LOG_DBG("BLE TX: LE Set Resolvable Private Address Timeout\n");break;
             case 0x202F:SSV_LOG_DBG("BLE TX: LE Read Maximum Data Length\n");break;
             default:
                 //SSV_LOG_DBG("BLE TX: unknow hci cmd 0x%x\n",opcode);
                 unknow_opcode++;
                 break;
         }
     }
     if(0!=unknow_opcode)
     {
         ssv_ble_process_unknow_hci_opcode(hdev, opcode,skb);
         goto END;
     }
    
     /* Prepend skb with frame type */
     memcpy(skb_push(skb, 1), &bt_cb(skb)->pkt_type, 1);
 
     ssv_ble_hci_send_packet(sbsc,skb->data,skb->len);
 END:    
     skb_queue_tail(&sbsc->ble_tx_queue, skb);
     return 0;
 }
 
#define VENDOR_SPECIFIC_OP 0x3f
#define HCI_SET_BD_ADDR_OP 0x7
int ssv_ble_hci_set_bdaddr(struct hci_dev *hdev, const bdaddr_t *bdaddr)
 {
     u16 opcode = 0;
     u8 cmd[10] = {0};
     u8 len = 0;
 
     struct ssv_ble_softc *sbsc = hci_get_drvdata(hdev);
 
     if (hdev == NULL || bdaddr == NULL || hdev == NULL)
         return -1;
 
     SSV_LOG_DBG("\33[32m%s():%d \33[0m\r\n",__FUNCTION__ ,__LINE__);
     opcode = 0;
     len = 6;
     opcode = ((VENDOR_SPECIFIC_OP << 10) | HCI_SET_BD_ADDR_OP);
 
     cmd[0] = 1;
     memcpy(&cmd[1], &opcode, sizeof(opcode));
     memcpy(&cmd[1] + sizeof(opcode), &len, sizeof(len));
     memcpy(&cmd[1] + sizeof(opcode) + sizeof(len), bdaddr, 6);
     SSV_LOG_DBG("set ble addr - %pM\n", bdaddr);
     ssv_ble_hci_send_packet(sbsc, cmd, sizeof(cmd));
     return 0;
 }


int ssv_ble_hci_dev_register(struct ssv_ble_softc *sbsc)
{
    int id = 0;
    /* Initialize HCI device */
    sbsc->hdev = hci_alloc_dev();
	if (!sbsc->hdev) {
		dev_err(sbsc->dev, "Can't allocate HCI device");
		return -ENOMEM;
	}


    SSV_LOG_DBG("hci device name:%s\n", sbsc->dev->driver->name);
    if( strcmp(sbsc->dev->driver->name, "USB"))
    {
        sbsc->hdev->bus = HCI_USB;
    }
    else
    {
       sbsc->hdev->bus = HCI_SDIO; 
    }

    //ssv_hdev->dev_type = 0;//HCI_BREDR;
	hci_set_drvdata(sbsc->hdev, sbsc);
	SET_HCIDEV_DEV(sbsc->hdev, sbsc->dev);

    sbsc->hdev->open  = ssv_ble_hci_open;
	sbsc->hdev->close = ssv_ble_hci_close;
	sbsc->hdev->flush = ssv_ble_hci_flush;
	sbsc->hdev->send  = ssv_ble_hci_send_frame;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
	sbsc->hdev->set_bdaddr  = ssv_ble_hci_set_bdaddr;
	#endif
    
    skb_queue_head_init(&sbsc->ble_tx_queue);

	/* Register HCI device */
    id = hci_register_dev(sbsc->hdev);
    SSV_LOG_DBG("hci device id:%d\n",id);
	if (id< 0) {
		dev_err(sbsc->dev, "Can't register HCI device");
		hci_free_dev(sbsc->hdev);
		return -ENODEV;
	}
	
	return 0;
}

void ssv_ble_hci_dev_unregister(struct ssv_ble_softc *sbsc)
{
    if(sbsc->hdev)
    {
        hci_unregister_dev(sbsc->hdev);
        hci_free_dev(sbsc->hdev);
        sbsc->hdev = NULL;
    }
}

/*
 * $ Copyright Broadcom Corporation $
 */
/*******************************************************************************
 **  Name:       userial_bby.c
 **
 **  Description:
 **
 **  This file contains the universal driver wrapper for the BTE-QC serial
 **  drivers
 **
 **  Copyright (c) 2001-2004, WIDCOMM Inc., All Rights Reserved.
 **  WIDCOMM Bluetooth Core. Proprietary and confidential.
 *******************************************************************************/
#include <rtthread.h>
#include <string.h>
#include "wiced_rtos.h"
#include "bt_target.h"
#include "gki_common.h"
#include "userial.h"
#include "hcidefs.h"
#include "btm_cfg.h"
#include "wiced_bt_sco.h"
#include <stdbool.h>
#include <rtdevice.h>
#include "platform_bluetooth.h"
#include "uart_api.h"
/*
void wiced_assert( const char * msg, bool b)
{
  if(!b)
  {
    rt_kprintf(msg);
    RT_ASSERT(0);
  }
}*/
/* Verify if WICED Platform API returns success.
 * Otherwise, returns the error code immediately.
 * Assert in DEBUG build.
 */
#define VERIFY_RETVAL( x ) \
do \
{ \
    wiced_result_t verify_result = (x); \
    if ( verify_result != WICED_SUCCESS ) \
    { \
        wiced_assert( "bus error", 0!=0 ); \
        return verify_result; \
    } \
} while( 0 )

/* Macro for checking of bus is initialised */
#define IS_BUS_INITIALISED( ) \
do \
{ \
    if ( bus_initialised == WICED_FALSE ) \
    { \
        wiced_assert( "bus uninitialised", 0!=0 ); \
        return WICED_ERROR; \
    } \
}while ( 0 )

/* Macro for checking if bus is ready */
#define BT_BUS_IS_READY( ) \
do \
{ \
    if ( bt_bus_is_ready( ) == WICED_FALSE ) \
    { \
        wiced_assert( "bus not ready", 0!=0 ) \
        return WICED_ERROR; \
    } \
}while ( 0 )

/* Macro for waiting until bus is ready */
#define BT_BUS_WAIT_UNTIL_READY( ) \
do \
{ \
    while ( bt_bus_is_ready( ) == WICED_FALSE ) \
    { \
        rt_thread_mdelay( 10 ); \
    } \
} while ( 0 )

/* TODO: bring in bt_bus code to remove BTE dependency on wiced bluetooth library */
wiced_result_t bt_bus_init(void);
static wiced_result_t bt_bus_deinit(void);
static wiced_result_t bt_bus_transmit(const uint8_t *data_out, uint32_t size);
static wiced_result_t bt_bus_receive(uint8_t *data_in, uint32_t size, uint32_t timeout_ms);
wiced_result_t bt_bus_uart_reconifig_baud(uint32_t baud);

static const UINT32 userial_baud_tbl[] =
{
    300, /* USERIAL_BAUD_300          0 */
    600, /* USERIAL_BAUD_600          1 */
    1200, /* USERIAL_BAUD_1200         2 */
    2400, /* USERIAL_BAUD_2400         3 */
    9600, /* USERIAL_BAUD_9600         4 */
    19200, /* USERIAL_BAUD_19200        5 */
    57600, /* USERIAL_BAUD_57600        6 */
    115200, /* USERIAL_BAUD_115200       7 */
    230400, /* USERIAL_BAUD_230400       8 */
    460800, /* USERIAL_BAUD_460800       9 */
    921600, /* USERIAL_BAUD_921600       10 */
    1000000, /* USERIAL_BAUD_1M           11 */
    1500000, /* USERIAL_BAUD_1_5M         12 */
    2000000, /* USERIAL_BAUD_2M           13 */
    3000000, /* USERIAL_BAUD_3M           14 */
    4000000 /* USERIAL_BAUD_4M           15 */
};

wiced_thread_t read_thread_id;

#define READ_THREAD_STACK_SIZE      0x3000

#define READ_THREAD_NAME            ((INT8 *) "READ_THREAD")
//#define READ_THREAD_PRI             9
#define READ_THREAD_PRI             20   /* test: lower than player task */
// #define READ_LIMIT               (GKI_BUF3_SIZE-BT_HDR_SIZE)
// #define HCISU_EVT                   EVENT_MASK(APPL_EVENT_0)

#define HCI_TYPE_COMMAND            1
#define HCI_TYPE_ACL_DATA           2
#define HCI_TYPE_SCO_DATA           3
#define HCI_TYPE_EVENT              4

BUFFER_Q Userial_in_q;
static BT_HDR *pbuf_USERIAL_Read = NULL;

void userial_read_thread(wiced_thread_arg_t arg);
UINT8 g_readThreadAlive = 0;

#define UART_RX_EVENT   0x01
/* USERIAL control block */
typedef struct
{
    int fd;
    //bool needPause;
    //bool uartIsBlock;   // uart need to set this flag before select.
    tUSERIAL_CBACK *p_cback;
} tUSERIAL_CB;
tUSERIAL_CB userial_cb =
{
    .fd = -1,
    //.needPause = false,
    //.uartIsBlock = false,
    .p_cback = 0
};

void dumphex(const char *tag, const uint8_t *data, const int len)
{
    rt_kprintf("%s", tag);
    for (int i = 0; i < len; i++)
    {
        rt_kprintf("%02x ", data[i] & 0x0ff);

        if ((i != 0) && ((i % 16) == 15))
        {
            rt_kprintf("\n");
        }
    }
    rt_kprintf("\n");
}
/******************************************************
 *                   Enumerations
 ******************************************************/
//#define USERIAL_WICED_HCI_UART  (WICED_UART_2)

typedef struct
{
    hci_packet_type_t packet_type; /* This is transport layer packet type. Not transmitted if transport bus is USB */
    uint8_t content_length;
} hci_loopback_packet_header_t;


typedef struct
{
    hci_packet_type_t packet_type; /* This is transport layer packet type. Not transmitted if transport bus is USB */
    uint8_t event_code;
    uint8_t content_length;
} hci_event_header_t;

typedef struct
{
    hci_packet_type_t packet_type; /* This is transport layer packet type. Not transmitted if transport bus is USB */
    uint16_t hci_handle;
    uint16_t content_length;
} hci_acl_packet_header_t;

typedef struct
{
    hci_packet_type_t packet_type; /* This is transport layer packet type. Not transmitted if transport bus is USB */
    uint16_t hci_handle;
    uint8_t  content_length;
} hci_sco_packet_header_t;

/*****************************************************************************
 * Platform UART interface, taken from
 * ../Library/bluetooth/internal/bus/UART/bt_bus.c
 * (audio/2.4.x-bluetooth branch)
 *****************************************************************************/
//#include "wiced_bt_platform.h"

//#ifndef USERIAL_RX_FIFO_SIZE
//#define USERIAL_RX_FIFO_SIZE (3000)
//#endif

static volatile wiced_bool_t bus_initialised = WICED_FALSE;
static volatile wiced_bool_t device_powered  = WICED_FALSE;

/* RX ring buffer. Bluetooth chip UART receive can be asynchronous, therefore a ring buffer is required */
//static volatile wiced_ring_buffer_t rx_ring_buffer;
//static volatile uint8_t             rx_data[USERIAL_RX_FIFO_SIZE];

extern void wice_init_bt_gpio();
extern void wice_bt_rst();

wiced_result_t bt_bus_init(void)
{
    wiced_result_t init_result = WICED_SUCCESS;

    if (bus_initialised == WICED_FALSE)
    {
        wice_init_bt_gpio();
        wice_bt_rst();
        if (init_result == WICED_SUCCESS)
        {
            bus_initialised = WICED_TRUE;
        }
    }
    return init_result;
}

extern uint32_t wice_bt_power_down(void);
wiced_result_t bt_bus_deinit(void)
{
    wiced_result_t result = WICED_SUCCESS;
    if (bus_initialised == WICED_FALSE)
    {
        return WICED_SUCCESS;
    }
    wice_bt_power_down();
    bus_initialised = WICED_FALSE;

    return result;
}


wiced_result_t bt_bus_transmit(const uint8_t *data_out, uint32_t size)
{
    int ret = send_comm(userial_cb.fd, data_out, size);
#ifdef UART_TXRX_VERBOSE
    int x = size > 16 ? 16 : size;
    dumphex("tx-> ", data_out, x);
#endif

    return ret == 0 ?  WICED_SUCCESS : WICED_ERROR;
}

static wiced_result_t bt_bus_receive(uint8_t *data_in, uint32_t size, uint32_t timeout_ms)
{
    int tryCount = 60;
    int alreadyRead = 0;
    int remains = size;

    IS_BUS_INITIALISED();

    do
    {
        int cnt = recv_comm(userial_cb.fd, data_in + alreadyRead, remains, 0);

        if (cnt > 0)
        {
            alreadyRead += cnt;
            remains -= cnt;
            if (alreadyRead == size)
            {
#ifdef UART_TXRX_VERBOSE
                int x = size > 16 ? 16 : size;
                dumphex("rx <- ", data_in, x);
#endif
                return WICED_SUCCESS;
            }
        }
        if (size == 1)
        {
            rt_kprintf("bt_bus_receive return WICED_ERROR\n");
            return WICED_ERROR;
        }
#ifdef UART_TXRX_VERBOSE
        rt_kprintf("read one more time (%d / %d)\n", alreadyRead, size);
#endif
    }
    while (tryCount--);
    rt_kprintf("rcvd (%d / %d)\n", alreadyRead, size);
    return WICED_ERROR;
}

static void uart_init(uint32_t baudrate)
{
    if (userial_cb.fd == -1)
    {
        rt_kprintf("open uart %s\n", BT_UART_NAME);
        userial_cb.fd = open_comm(BT_UART_NAME);
        RT_ASSERT(userial_cb.fd != -1);
    }

    rt_kprintf("set host baudrate %d\n", baudrate);
    config_comm(userial_cb.fd, baudrate, 'n', 8, 1);
    flush_comm(userial_cb.fd);
    rt_kprintf("set host baudrate %d ok\n", baudrate);
}

static void uart_deinit(void)
{
    if (userial_cb.fd != -1)
    {
        rt_kprintf("uart_deinit %d\n", userial_cb.fd);
        flush_comm(userial_cb.fd);
        close_comm(userial_cb.fd);
        userial_cb.fd = -1;
    }
}

wiced_result_t bt_bus_uart_reconifig_baud(uint32_t baud)
{
    if (bus_initialised == WICED_TRUE)
    {
        uart_init(baud);

        return WICED_SUCCESS;
    }
    else
    {
        return WICED_ERROR;
    }
}


/*******************************************************************************
 **
 ** Function           USERIAL_GetLineSpeed
 **
 ** Description        This function convert USERIAL baud to line speed.
 **
 ** Output Parameter   None
 **
 ** Returns            line speed
 **
 *******************************************************************************/
UINT32 USERIAL_GetLineSpeed(UINT8 baud)
{
    if (baud <= USERIAL_BAUD_4M)
        return (userial_baud_tbl[baud - USERIAL_BAUD_300]);
    else
        return 0;
}

/*******************************************************************************
 **
 ** Function           USERIAL_GetBaud
 **
 ** Description        This function convert line speed to USERIAL baud.
 **
 ** Output Parameter   None
 **
 ** Returns            line speed
 **
 *******************************************************************************/
UDRV_API
extern UINT8 USERIAL_GetBaud(UINT32 line_speed)
{
    UINT8 i;
    for (i = USERIAL_BAUD_300; i <= USERIAL_BAUD_4M; i++)
    {
        if (userial_baud_tbl[i - USERIAL_BAUD_300] == line_speed)
            return i;
    }

    return USERIAL_BAUD_AUTO;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Init
 **
 ** Description        This function initializes the  serial driver.
 **
 ** Output Parameter   None
 **
 ** Returns            Nothing
 **
 *******************************************************************************/

UDRV_API
void USERIAL_Init(void *p_cfg)
{
    DRV_TRACE_DEBUG0("USERIAL_Init");

    //memset(&userial_cb, 0, sizeof(userial_cb));
    GKI_init_q(&Userial_in_q);

    return;
}

void USERIAL_Open_ReadThread(void)
{
    wiced_result_t result;

    DRV_TRACE_DEBUG0("USERIAL_Open_ReadThread++");

    g_readThreadAlive = 1;
    result = wiced_rtos_create_thread(&read_thread_id, READ_THREAD_PRI, (const char *) READ_THREAD_NAME, userial_read_thread, READ_THREAD_STACK_SIZE, NULL);
    if (result != WICED_SUCCESS)
    {
        DRV_TRACE_DEBUG0("USERIAL_Open failed to create userial_read_thread");
        USERIAL_Close(0);
        g_readThreadAlive = 0;
        return;
    }
}

void USERIAL_Close_ReadThread(void)
{
    // Close our read thread
    g_readThreadAlive = 0;
#if 0
    wiced_rtos_delete_thread(&read_thread_id);
#else
    rt_thread_mdelay(500);
    rt_kprintf("USERIAL_Close_ReadThread \n");
#endif
    // Flush the queue
    do
    {
        pbuf_USERIAL_Read = (BT_HDR *) GKI_dequeue(&Userial_in_q);
        if (pbuf_USERIAL_Read)
        {
            GKI_freebuf(pbuf_USERIAL_Read);
        }

    }
    while (pbuf_USERIAL_Read);
}

/*******************************************************************************
 **
 ** Function           USERIAL_Open
 **
 ** Description        Open the indicated serial port with the given configuration
 **
 ** Output Parameter   None
 **
 ** Returns            Nothing
 **
 *******************************************************************************/

UDRV_API
void USERIAL_Open(tUSERIAL_PORT port, tUSERIAL_OPEN_CFG *p_cfg, tUSERIAL_CBACK *p_cback)
{

    wiced_result_t result;

    DRV_TRACE_DEBUG0("USERIAL_Open");

    result = bt_bus_init();
    if (result != WICED_SUCCESS)
    {
        DRV_TRACE_DEBUG0("USERIAL_Open failed");
        return;
    }

    uart_init(115200);
    USERIAL_Open_ReadThread();

    rt_kprintf("USERIAL_Open %d \n",  __LINE__);

    userial_cb.p_cback = p_cback;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Read
 **
 ** Description        Read data from a serial port using byte buffers.
 **
 ** Output Parameter   None
 **
 ** Returns            Number of bytes actually read from the serial port and
 **                    copied into p_data.  This may be less than len.
 **
 *******************************************************************************/

UDRV_API
UINT16 USERIAL_Read(tUSERIAL_PORT port, UINT8 *p_data, UINT16 len)
{
    UINT16 total_len = 0;
    UINT16 copy_len = 0;
    UINT8 *current_packet = NULL;

    do
    {
        if (pbuf_USERIAL_Read != NULL)
        {
            current_packet = ((UINT8 *)(pbuf_USERIAL_Read + 1)) + (pbuf_USERIAL_Read->offset);

            if ((pbuf_USERIAL_Read->len) <= (len - total_len))
                copy_len = pbuf_USERIAL_Read->len;
            else
                copy_len = (len - total_len);

            memcpy((p_data + total_len), current_packet, copy_len);

            total_len += copy_len;

            pbuf_USERIAL_Read->offset += copy_len;
            pbuf_USERIAL_Read->len -= copy_len;

            if (pbuf_USERIAL_Read->len == 0)
            {
                GKI_freebuf(pbuf_USERIAL_Read);
                pbuf_USERIAL_Read = NULL;
            }
        }

        if (pbuf_USERIAL_Read == NULL)
        {
            pbuf_USERIAL_Read = (BT_HDR *) GKI_dequeue(&Userial_in_q);
//            DRV_TRACE_DEBUG1("USERIAL_Read dequeue %08x", pbuf_USERIAL_Read);
        }
    }
    while ((pbuf_USERIAL_Read != NULL) && (total_len < len));

//    DRV_TRACE_DEBUG1("USERIAL_Read %i bytes", total_len);

    return total_len;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Readbuf
 **
 ** Description        Read data from a serial port using GKI buffers.
 **
 ** Output Parameter   Pointer to a GKI buffer which contains the data.
 **
 ** Returns            Nothing
 **
 ** Comments           The caller of this function is responsible for freeing the
 **                    GKI buffer when it is finished with the data.  If there is
 **                    no data to be read, the value of the returned pointer is
 **                    NULL.
 **
 *******************************************************************************/
UDRV_API
void USERIAL_ReadBuf(tUSERIAL_PORT port, BT_HDR **p_buf)
{
    if (p_buf == NULL)
        return;

    *p_buf = (BT_HDR *) GKI_dequeue(&Userial_in_q);
    //DRV_TRACE_DEBUG1("USERIAL_ReadBuf dequeue %08x", *p_buf);
}


/*******************************************************************************
 **
 ** Function           USERIAL_WriteBuf
 **
 ** Description        Write data to a serial port using a GKI buffer.
 **
 ** Output Parameter   None
 **
 ** Returns            TRUE  if buffer accepted for write.
 **                    FALSE if there is already a buffer being processed.
 **
 ** Comments           The buffer will be freed by the serial driver.  Therefore,
 **                    the application calling this function must not free the
 **                    buffer.
 **
 *******************************************************************************/
UDRV_API
BOOLEAN USERIAL_WriteBuf(tUSERIAL_PORT port, BT_HDR *p_buf)
{
    return FALSE;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Write
 **
 ** Description        Write data to a serial port using a byte buffer.
 **
 ** Output Parameter   None
 **
 ** Returns            Number of bytes actually written to the serial port.  This
 **                    may be less than len.
 **
 *******************************************************************************/
UDRV_API
UINT16 USERIAL_Write(tUSERIAL_PORT port, UINT8 *p_data, UINT16 len)
{
    wiced_result_t result;

//    DRV_TRACE_DEBUG1("USERIAL_Write len=%d", len);
    result = bt_bus_transmit(p_data, len);
    if (result != WICED_SUCCESS)
    {
        DRV_TRACE_DEBUG0("USERIAL_Write failed");
        return 0;
    }
//    DRV_TRACE_DEBUG0("USERIAL_Write success");
    return len;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Ioctl
 **
 ** Description        Perform an operation on a serial port.
 **
 ** Output Parameter   The p_data parameter is either an input or output depending
 **                    on the operation.
 **
 ** Returns            Nothing
 **
 *******************************************************************************/

UDRV_API
void USERIAL_Ioctl(tUSERIAL_PORT port, tUSERIAL_OP op, tUSERIAL_IOCTL_DATA *p_data)
{

    switch (op)
    {
    case USERIAL_OP_FLUSH:
        break;
    case USERIAL_OP_FLUSH_RX:
        break;
    case USERIAL_OP_FLUSH_TX:
        break;
    case USERIAL_OP_BAUD_RD:
        break;
    case USERIAL_OP_BAUD_WR:
        //USERIAL_Close_ReadThread( ); // Close read thread before de-initing UART
        /*    rt_kprintf("change Baud rate event CB (%d)\n", p_data->baud);
            userial_cb.needPause = true;
            while(userial_cb.uartIsBlock == true)
            {
                rt_thread_mdelay(1);
            }*/
        bt_bus_uart_reconifig_baud(USERIAL_GetLineSpeed(p_data->baud));
        //userial_cb.needPause = false;
        //USERIAL_Open_ReadThread( ); // Start the read thread again
        break;
    default:
        break;
    }

    return;
}

/*******************************************************************************
 **
 ** Function           USERIAL_Close
 **
 ** Description        Close a serial port
 **
 ** Output Parameter   None
 **
 ** Returns            Nothing
 **
 *******************************************************************************/
UDRV_API
void USERIAL_Close(tUSERIAL_PORT port)
{
    wiced_result_t result;
    DRV_TRACE_DEBUG0("USERIAL_Close\n");

    USERIAL_Close_ReadThread();

    uart_deinit();

    rt_thread_mdelay(500);

    result = bt_bus_deinit();
    if (result != WICED_SUCCESS)
    {
        DRV_TRACE_DEBUG0("USERIAL_Close failed\n");
    }

    DRV_TRACE_DEBUG0("USERIAL_Close--\n");
}

/*******************************************************************************
 **
 ** Function           USERIAL_Feature
 **
 ** Description        Check whether a feature of the serial API is supported.
 **
 ** Output Parameter   None
 **
 ** Returns            TRUE  if the feature is supported
 **                    FALSE if the feature is not supported
 **
 *******************************************************************************/
UDRV_API BOOLEAN USERIAL_Feature(tUSERIAL_FEATURE feature)
{
    return FALSE;
}

#define USERIAL_DROP_BUF_SIZE   20
static void bt_hci_transport_driver_bus_drop_data(uint32_t size)
{
    UINT8       tmp[USERIAL_DROP_BUF_SIZE];
    uint32_t    read_size = size;
    uint32_t    left_size = size;
    while (left_size)
    {
        if (read_size > USERIAL_DROP_BUF_SIZE)
            read_size = USERIAL_DROP_BUF_SIZE;
        bt_bus_receive((uint8_t *) tmp, read_size, RT_WAITING_FOREVER);
        if (left_size >= read_size)
        {
            left_size -= read_size;
            read_size  = left_size;
        }
    }
}

wiced_result_t bt_hci_transport_driver_bus_read_handler(BT_HDR **pp_packet)
{
    hci_packet_type_t packet_type = HCI_UNINITIALIZED;
    UINT8 *current_packet;
    BT_HDR *packet = NULL;
    wiced_result_t result = WICED_SUCCESS;

    if (pp_packet == NULL || *pp_packet != NULL)
    {
        return WICED_ERROR;
    }

    // Read 1 byte:
    //    packet_type
    // Use a timeout here so we can shutdown the thread
    if (bt_bus_receive((uint8_t *) &packet_type, 1, RT_WAITING_FOREVER) != WICED_SUCCESS)
    {
        //DRV_TRACE_ERROR0("bt_bus error reading pkt_type");
        rt_kprintf("bt_hci_transport_driver_bus_read_handler return WICED_ERROR\n");
        return WICED_ERROR;
    }

    //DRV_TRACE_DEBUG1("bt_hci_transport_driver_bus_read_handler packet_type=0x%x", packet_type);
    //rt_kprintf("HCI EVT 0x%04x", packet_type & 0x0ffff);

    switch (packet_type)
    {
#if 1
    case HCI_LOOPBACK_MODE:

        ; /* make sure that below declaration is not the first statement after above case */
        hci_loopback_packet_header_t header;

        // Read 1 byte:
        //    content_length
        if (bt_bus_receive((uint8_t *) &header.content_length, 1, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            DRV_TRACE_ERROR0("bt_bus error loopback header");
            result = WICED_ERROR;
            break;
        }
        packet = (BT_HDR *) GKI_getpoolbuf(HCI_ACL_POOL_ID);

        if (!packet)
        {
            bt_hci_transport_driver_bus_drop_data((uint32_t) header.content_length);
            DRV_TRACE_ERROR0("unable to allocate from GKI ACL Pool (for Loopback)!!");
            result = WICED_OUT_OF_HEAP_SPACE;
            break;
        }

        packet->offset = 0;
        packet->layer_specific = 0;
        current_packet = (UINT8 *)(packet + 1);
        *current_packet++ = packet_type;
        *current_packet++ = header.content_length;
        packet->len       = header.content_length;

        //DRV_TRACE_DEBUG1("HCI_LOOPBACK_MODE packet->len=0x%x", packet->len);

        // Read payload
        if (bt_bus_receive(current_packet, packet->len, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            DRV_TRACE_DEBUG0("bt_bus_receive FAIL in bt_hci_transport_driver_bus_read_handler");
            result = WICED_ERROR;
            break;
        }

        packet->len = packet->len + 2; // +2 for packet type + read length
        break;
#endif
    case HCI_EVENT_PACKET:
    {
        hci_event_header_t header;

        // Read 2 bytes:
        //    event_code
        //    content_length
        if (bt_bus_receive((uint8_t *) &header.event_code, sizeof(header) - sizeof(packet_type), RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            result = WICED_ERROR;
            break;
        }

        packet = (BT_HDR *) GKI_getpoolbuf(HCI_CMD_POOL_ID);
        if (!packet)
        {
            bt_hci_transport_driver_bus_drop_data((uint32_t) header.content_length);
            DRV_TRACE_ERROR0("unable to allocate from GKI CMD Pool!!");
            result = WICED_OUT_OF_HEAP_SPACE;
            break;
        }

        packet->offset = 0;
        packet->layer_specific = 0;
        current_packet = (UINT8 *)(packet + 1);
        *current_packet++ = packet_type;
        *current_packet++ = header.event_code;
        *current_packet++ = header.content_length;
        packet->len = header.content_length + 3; // +3 to include sizeof: packet_type=1 event_code=1 content_length=1

        if (header.content_length > 280)
        {
            rt_kprintf("length error !!!! %d\n", header.content_length & 0x0FFFF);
            result = WICED_ERROR;
            break;
        }
        // Read payload
        if (bt_bus_receive((uint8_t *) current_packet, (uint32_t) header.content_length, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            result = WICED_ERROR;
            break;
        }
        //DRV_TRACE_ERROR0("EVENT PACKETl!!");

        break;
    }

    case HCI_ACL_DATA_PACKET:
    {
        hci_acl_packet_header_t header;

        // Read 4 bytes:
        //    hci_handle (2 bytes)
        //    content_length (2 bytes)
        if (bt_bus_receive((uint8_t *) &header.hci_handle, HCI_DATA_PREAMBLE_SIZE, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            DRV_TRACE_ERROR0("bt_bus error acl header");
            result = WICED_ERROR;
            break;
        }

        packet = (BT_HDR *) GKI_getpoolbuf(HCI_ACL_POOL_ID);
        if (!packet)
        {
            bt_hci_transport_driver_bus_drop_data((uint32_t) header.content_length);
            DRV_TRACE_ERROR0("unable to allocate from GKI ACL Pool!!");
            result = WICED_OUT_OF_HEAP_SPACE;
            break;
        }

        packet->offset = 0;
        packet->layer_specific = 0;
        current_packet = (UINT8 *)(packet + 1);
        UINT8_TO_STREAM(current_packet, packet_type);
        UINT16_TO_STREAM(current_packet, header.hci_handle);
        UINT16_TO_STREAM(current_packet, header.content_length);

        packet->len = header.content_length + 5; // +5 to include sizeof: packet_type=1 hci_handle=2 content_length=2

        if (header.content_length > (HCI_ACL_POOL_BUF_SIZE - BT_HDR_SIZE))
        {
            DRV_TRACE_ERROR2("bt_bus error invalid acl len %i, handle %04x", header.content_length, header.hci_handle);
            result = WICED_ERROR;
            // dump all data in driver then sleep forever
            void dump_uart_buffer(int fd);
            dump_uart_buffer(userial_cb.fd);
            while (1) wiced_rtos_delay_milliseconds(100);

            break;
        }
        // Read payload
        if (bt_bus_receive((uint8_t *) current_packet, (uint32_t) header.content_length, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            DRV_TRACE_ERROR1("bt_bus read failed, handle %d\n", header.content_length);
            result = WICED_ERROR;
            break;
        }
        //DRV_TRACE_ERROR0("ACL Success!!");
        break;
    }
#if 1
    case HCI_SCO_DATA_PACKET:
    {
        hci_sco_packet_header_t header;

        // Read 4 bytes:
        //    hci_handle (2 bytes)
        //    content_length (1 bytes)
        if (bt_bus_receive((uint8_t *) &header.hci_handle, HCI_SCO_PREAMBLE_SIZE, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            DRV_TRACE_ERROR0("bt_bus error sco header");
            result = WICED_ERROR;
            break;
        }

        packet = (BT_HDR *) wiced_get_pool_buf(wiced_bt_sco_get_buffer_pool());
        if (!packet)
        {
            bt_hci_transport_driver_bus_drop_data((uint32_t) header.content_length);
            DRV_TRACE_ERROR0("unable to allocate from GKI SCO Pool!!");
            result = WICED_OUT_OF_HEAP_SPACE;
            break;
        }

        packet->offset = 0;
        packet->layer_specific = 0;
        current_packet = (UINT8 *)(packet + 1);
        UINT8_TO_STREAM(current_packet, packet_type);
        UINT16_TO_STREAM(current_packet, header.hci_handle);
        UINT8_TO_STREAM(current_packet, header.content_length);

        packet->len = header.content_length + 4; // +4 to include sizeof: packet_type=1 hci_handle=2 content_length=1

        // Read payload
        if (bt_bus_receive((uint8_t *) current_packet, (uint32_t) header.content_length, RT_WAITING_FOREVER) != WICED_SUCCESS)
        {
            result = WICED_ERROR;
            break;
        }

        break;
    }
#endif
    case HCI_COMMAND_PACKET: /* Fall-through */
    default:
        DRV_TRACE_ERROR0("unknow packet!!!");
        result = WICED_ERROR;
        break;
    }

    if (result != WICED_SUCCESS)
    {
        if (packet != NULL)
            GKI_freebuf(packet);
        packet = NULL;
    }

    *pp_packet = packet;

    return result;
}


/*******************************************************************************
 **
 ** Function           userial_read_thread
 **
 ** Description        userial_read_thread
 **
 ** Output Parameter   None
 **
 ** Returns            None
 **
 *******************************************************************************/
void userial_read_thread(wiced_thread_arg_t arg)
{
    wiced_result_t ret;
    UNUSED_PARAMETER(arg);

    rt_kprintf("userial_read_thread running.\n");
    GKI_delay(10);
    while (g_readThreadAlive)
    {
        BT_HDR *p_buf = NULL;
        ret = bt_hci_transport_driver_bus_read_handler(&p_buf);
        if (ret == WICED_SUCCESS)
        {
            GKI_enqueue(&Userial_in_q, p_buf);

            if (userial_cb.p_cback)
            {
                (userial_cb.p_cback)(0, USERIAL_RX_READY_EVT, NULL);
            }
        }
        else if (ret == WICED_OUT_OF_HEAP_SPACE)
        {
            GKI_delay(500);
        }
    }
    rt_kprintf("userial_read_thread exit \n");
}

#define BT_FACTORY_TEST
/* factory test support */
#ifdef BT_FACTORY_TEST

uint32_t  bt_power_down(void);

typedef struct
{
    int fd;
    uint8_t rxBuf[1024];
} tFACTORY_CB;

static tFACTORY_CB factory;

wiced_result_t bt_tx(int fd, const uint8_t *data_out, uint32_t size)
{
    int ret = send_comm(fd, data_out, size);

    int x = size > 16 ? 16 : size;
    dumphex("tx-> ", data_out, x);

    return ret == 0 ?  WICED_SUCCESS : WICED_ERROR;
}

static wiced_result_t bt_rx(int fd, uint8_t *data_in, uint32_t size, uint32_t timeout_ms)
{
    int tryCount = 60;
    int alreadyRead = 0;
    int remains = size;

    do
    {
        int cnt = recv_comm(fd, data_in + alreadyRead, remains, 0);

        if (cnt > 0)
        {
            alreadyRead += cnt;
            remains -= cnt;
            if (alreadyRead == size)
            {
                int x = size > 16 ? 16 : size;
                dumphex("rx <- ", data_in, x);
                return WICED_SUCCESS;
            }
        }
        if (size == 1)
        {
            return WICED_ERROR;
        }

        rt_kprintf("read one more time (%d / %d)\n", alreadyRead, size);
    }
    while (tryCount--);

    return WICED_ERROR;
}

uint16_t read_event(int fd, uint8_t *buffer)
{
    int len = 1;
    wiced_result_t ret;

    ret = bt_rx(fd, buffer, 3, RT_NULL);
    rt_kprintf("ret %s, %02x, %02x, %02x\n", ret ? "error" : "success", buffer[0], buffer[1], buffer[2]);
    if ((ret == 0) && (buffer[0] == 4) && (buffer[1] == 0x0e))
    {
        len = buffer[2] & 0x0ff;
        ret = bt_rx(fd, buffer + 3, len, RT_NULL);
        return len + 3;
    }
    else
    {
        rt_kprintf("read type failed\n");
        return 0;
    }
}

bool snd_rcv(int fd, const uint8_t *inbuf, int inlen, uint8_t *outbuf, int *outlen)
{
    if (0 == bt_tx(fd, inbuf, inlen))
    {
        *outlen = read_event(fd, outbuf);
        return true;
    }
    else
    {
        rt_kprintf("snd_rcv failed.\n");
        return false;
    }
}
extern const uint8_t brcm_patchram_buf[];
extern const int brcm_patch_ram_length;
bool download_fw(int fd)
{
    int cnt;
    const uint8_t *p = brcm_patchram_buf;
    do
    {
        int length  = (p[2] & 0x0ff) + 3;
        memcpy(factory.rxBuf + 1, p, length);
        factory.rxBuf[0] = 0x01;
        snd_rcv(fd, factory.rxBuf, length + 1, factory.rxBuf, &cnt);
        p += length;
    }
    while (p < (brcm_patchram_buf + brcm_patch_ram_length));

    return true;
}

static void hci_rst()
{
    int cnt;
    const uint8_t hci_reset[] = { 0x01, 0x03, 0x0c, 0x00 };
    snd_rcv(factory.fd, hci_reset, sizeof(hci_reset), factory.rxBuf, &cnt);
}

MSH_CMD_EXPORT(hci_rst, Bluetooth HCI RESET);

static void dlhcd()
{
    int cnt;

    wice_init_bt_gpio();
    wice_bt_rst();

    if ((factory.fd = open_comm(BT_UART_NAME)) == -1)
    {
        rt_kprintf("open failed\n");
        return;
    }

    config_comm(factory.fd, 115200, 'n', 8, 1);
    flush_comm(factory.fd);

    rt_thread_mdelay(100);

    hci_rst();

    rt_thread_mdelay(100);
    const uint8_t miniDriver[] = {0x01, 0x2e, 0xfc, 0/*length*/};
    snd_rcv(factory.fd, miniDriver, sizeof(miniDriver), factory.rxBuf, &cnt);

    rt_thread_mdelay(100);
    rt_kprintf("download fw...\n");
    download_fw(factory.fd);

    rt_thread_mdelay(100);
    const uint8_t lunchRam[] = {0x01, 0x4E, 0xFC, 0x04, 0xFF, 0xFF, 0xFF, 0xFF};
    snd_rcv(factory.fd, lunchRam, sizeof(lunchRam), factory.rxBuf, &cnt);

    rt_thread_mdelay(1000);
    hci_rst();

    //close_comm(factory.fd);

    //bt_power_down();
    rt_kprintf("download fw complete.\n");
}

MSH_CMD_EXPORT(dlhcd, Bluetooth download HCD);

static bool isDigital(char c)
{
    return ((c >= '0') && (c <= '9'))
           || ((c >= 'a') && (c <= 'f'))
           || ((c >= 'A') && (c <= 'F'));
}

// c is in the range  0-9 or a-f or A-F
static uint8_t c2b(char c)
{
    if ((c >= '0') && (c <= '9'))
        return c - '0';

    if ((c >= 'a') && (c <= 'f'))
        return c - 'a' + 10;

    if ((c >= 'A') && (c <= 'F'))
        return c - 'A' + 10;
    return 0;
}

bool isDigitalString(const char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (!isDigital(s[i]))
            return false;
    }

    return true;
}

//
// string must be in the correct format, lengh = 2, every character is a hex char 0-9, a-f, A-F
// for example 01 or 34 or 00
//  caller should ensure the valid input
//
static uint8_t s2b(const char *s)
{
    uint8_t h = c2b(s[0]);
    uint8_t l = c2b(s[1]);

    return (h << 4) | l;
}

static bool check_param(const char *p)
{
    return (strlen(p) == 2)
           && isDigital(p[0])
           && isDigital(p[1]);
}

/* argc should be greater than 1
 * argv[i] should be 2 digital, example - 98 or 02 or 23...
 */
bool validArgs(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        if (!check_param(argv[i]))
            return false;
    }

    return true;
}

static void hcic(int argc, char **argv)
{
    if (argc < 4)
    {
        rt_kprintf("HCI CMD should be bigger than 3 \n");
        return;
    }

    factory.rxBuf[0] = 1;
    for (int i = 1; i < argc; i++)
    {
        if (!check_param(argv[i]))
        {
            rt_kprintf("invalid input value\n");
            return;
        }
        factory.rxBuf[i] = s2b(argv[i]);
    }
    int read;
    snd_rcv(factory.fd, factory.rxBuf, argc, factory.rxBuf, &read);
}

MSH_CMD_EXPORT(hcic, hcicmd sample: hcicmd 01 03 0C 00);

#define HCI_CMD_LEN  32
// this command array is used to enter DUT mode.
const uint8_t enter_dut_commands[][HCI_CMD_LEN] =
{
    {0x09, 0x10, 0x00},
    {0x05, 0x0c, 0x03, 0x02, 0x00, 0x02},
    {0x1A, 0x0c, 0x01, 0x03},
    {0x03, 0x18, 0x00},
    {-1}
};


void send_rcev_ex(const uint8_t cmds[][HCI_CMD_LEN], int items)
{
    int cnt;
    for (int i = 0; i < items; i ++)
    {
        int length = (cmds[i][2] & 0x0ff) + 3;
        factory.rxBuf[0] = 1;
        memcpy(factory.rxBuf + 1, cmds[i], length);
        length++;
        snd_rcv(factory.fd, factory.rxBuf, length, factory.rxBuf, &cnt);
    }
}

static void dut()
{
    send_rcev_ex(enter_dut_commands, 4);
}

MSH_CMD_EXPORT(dut, Bluetooth Enter DUT mode);

/* LE test command */

static void le_test_end()
{
    int ch;
    const uint8_t end_cmd[] = {0x01, 0x1f, 0x20, 0x00};
    // UINT8 hci_le_test_end_cmd_complete_event[] = {0x0e, 0x06, 0x01, 0x1f, 0x20, 0x00};  // expected event
    snd_rcv(factory.fd, end_cmd, sizeof(end_cmd), factory.rxBuf, &ch);
}

MSH_CMD_EXPORT(le_test_end, Bluetooth LE test end);

static void le_rx(int argc, char **argv)
{
    uint8_t cmd[] = {1, 0x1d, 0x20, 0x01, 0x00};
    if (argc != 2 || !isDigitalString(argv[1]))
    {
        rt_kprintf("input error\n");
        return;
    }
    int ch = atoi(argv[1]);
    if (ch > 40 || ch < 0)
    {
        rt_kprintf("LE channel range [0, 40]\n");
        return;
    }

    cmd[4] = (uint8_t)ch;
    snd_rcv(factory.fd, cmd, sizeof(cmd), factory.rxBuf, &ch);
}

MSH_CMD_EXPORT(le_rx, le receive test: le_rx 2);

static void le_tx(int argc, char **argv)
{
    int cnt;
    uint8_t cmd[] = {1, 0x1E, 0x20, 0x03, 0x02, 0x0A, 0x04};
    if (argc != 4 || !validArgs(argc, argv))
    {
        rt_kprintf("input error\n");
        return;
    }
    cmd[4] = s2b(argv[1]);
    cmd[5] = s2b(argv[2]);
    cmd[6] = s2b(argv[3]);

    snd_rcv(factory.fd, cmd, sizeof(cmd), factory.rxBuf, &cnt);
}

MSH_CMD_EXPORT(le_tx, le transmitter test: le_tx <channel> <data length> <pattern>);

/*
 * use the same format as
 *    Manufacturing-Bluetooth-Test-Tool.pdf
 */
static void set_tx_frequency_arm(int argc, char **argv)
{
    int cnt;
    uint8_t cmd[] = { 1, 0x14, 0xFC, 0x07, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
    if ((argc == 2) && !strcmp("0", argv[1]))
    {
        // do nothing here.
    }
    else if ((argc == 6) && !strcmp("1", argv[1]))
    {
        int freq = atoi(argv[2]);
        int mode = atoi(argv[3]);
        int type = atoi(argv[4]);
        int power = atoi(argv[5]);

        cmd[4] = 0;
        cmd[5] = (uint8_t)(freq - 2400);
        cmd[6] = (uint8_t)mode;
        cmd[7] = (uint8_t)type;
        cmd[8] = 8;
        cmd[9] = (uint8_t)power;
    }
    else
    {
        rt_kprintf("input error\n");
        return;
    }
    snd_rcv(factory.fd, cmd, sizeof(cmd), factory.rxBuf, &cnt);
}

MSH_CMD_EXPORT(set_tx_frequency_arm, set_tx_frequency_arm < carrier on / off > <tx_frequency> <tx_mode> <tx_modulation_type>);


/*
 * use the same format as
 *    Manufacturing-Bluetooth-Test-Tool.pdf
 */
static void receive_only(int argc, char **argv)
{
    int cnt;
    uint8_t cmd[] = { 1, 0x2B, 0xFC, 0x01, 0x06};
    if (argc == 2)
    {
        cmd[4] = (uint8_t)(atoi(argv[1]) - 2400);
    }
    else
    {
        rt_kprintf("input error\n");
        return;
    }
    snd_rcv(factory.fd, cmd, sizeof(cmd), factory.rxBuf, &cnt);
}

MSH_CMD_EXPORT(receive_only, receive_only <rx_frequency>);

#endif

extern void application_start();
static void bt()
{
    application_start();
}
MSH_CMD_EXPORT(bt, start Bluetooth);

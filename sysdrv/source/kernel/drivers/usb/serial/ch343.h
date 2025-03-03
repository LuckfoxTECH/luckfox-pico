#ifndef _CH343_H
#define _CH343_H

/*
 * Baud rate and default timeout
 */
#define DEFAULT_BAUD_RATE 9600
#define DEFAULT_TIMEOUT   2000

/*
 * CMSPAR, some architectures can't have space and mark parity.
 */

#ifndef CMSPAR
#define CMSPAR 0
#endif

/*
 * Major and minor numbers.
 */

#define CH343_TTY_MAJOR  170
#define CH343_TTY_MINORS 256

#define USB_MINOR_BASE 70

/*
 * Requests.
 */

#define USB_RT_CH343 (USB_TYPE_CLASS | USB_RECIP_INTERFACE)

#define CMD_R  0x95
#define CMD_W  0x9A
#define CMD_C1 0xA1
#define CMD_C2 0xA4
#define CMD_C3 0x05
#define CMD_C4 0xA8
#define CMD_C5 0x5E
#define CMD_C6 0x5F

#define CH343_CTO_O  0x10
#define CH343_CTO_D  0x20
#define CH343_CTO_R  0x40
#define CH343_CTO_A  0x80
#define CH343_CTI_C  0x01
#define CH343_CTI_DS 0x02
#define CH343_CTI_R  0x04
#define CH343_CTI_DC 0x08
#define CH343_CTI_ST 0x0f

#define CH343_CTT_M 0x08
#define CH343_CTT_F 0x44
#define CH343_CTT_P 0x04
#define CH343_CTT_O 0x02

#define CH343_LO 0x02
#define CH343_LE 0x04
#define CH343_LB
#define CH343_LP 0x00
#define CH343_LF 0x40
#define CH343_LM 0x08

#define CH343_L_R_CT 0x80
#define CH343_L_R_CL 0x04
#define CH343_L_R_T  0x08

#define CH343_L_E_R 0x80
#define CH343_L_E_T 0x40
#define CH343_L_P_S 0x38
#define CH343_L_P_M 0x28
#define CH343_L_P_E 0x18
#define CH343_L_P_O 0x08
#define CH343_L_SB  0x04
#define CH343_L_C8  0x03
#define CH343_L_C7  0x02
#define CH343_L_C6  0x01
#define CH343_L_C5  0x00

#define CH343_N_B  0x80
#define CH343_N_AB 0x10

/*
 * Internal driver structures.
 */

/*
 * The only reason to have several buffers is to accommodate assumptions
 * in line disciplines. They ask for empty space amount, receive our URB size,
 * and proceed to issue several 1-character writes, assuming they will fit.
 * The very first write takes a complete URB. Fortunately, this only happens
 * when processing onlcr, so we only need 2 buffers. These values must be
 * powers of 2.
 */
#define CH343_NW 16
#define CH343_NR 16

struct ch343_wb {
    unsigned char *buf;
    dma_addr_t dmah;
    int len;
    int use;
    struct urb *urb;
    struct ch343 *instance;
};

struct ch343_rb {
    int size;
    unsigned char *base;
    dma_addr_t dma;
    int index;
    struct ch343 *instance;
};

struct usb_ch343_line_coding {
    __u32 dwDTERate;
    __u8 bCharFormat;
#define USB_CH343_1_STOP_BITS   0
#define USB_CH343_1_5_STOP_BITS 1
#define USB_CH343_2_STOP_BITS   2

    __u8 bParityType;
#define USB_CH343_NO_PARITY    0
#define USB_CH343_ODD_PARITY   1
#define USB_CH343_EVEN_PARITY  2
#define USB_CH343_MARK_PARITY  3
#define USB_CH343_SPACE_PARITY 4

    __u8 bDataBits;
} __attribute__((packed));

typedef enum {
    CHIP_CH342F = 0x00,
    CHIP_CH342K,
    CHIP_CH343GP,
    CHIP_CH343G_AUTOBAUD,
    CHIP_CH343K,
    CHIP_CH343J,
    CHIP_CH344L,
    CHIP_CH344L_V2,
    CHIP_CH344Q,
    CHIP_CH347T,
    CHIP_CH9101UH,
    CHIP_CH9101RY,
    CHIP_CH9102F,
    CHIP_CH9102X,
    CHIP_CH9103M,
    CHIP_CH9104L,
} CHIPTYPE;

struct gpioinfo {
    int group;
    int pin;
};

struct ch343_gpio {
    int gpiocount;
    struct gpioinfo io[64];
};

struct ch343_gpio ch343_gpios[] = {
    { 0,                                                                                               {}},
    { 0,                                                                                               {}},
    { 0,                                                                                               {}},
    { 0,                                                                                               {}},
    { 0,                                                                                               {}},
    { 0,                                                                                               {}},
 /* CH344L */
    { 8,                                                                                               {}},
 /* CH344L-V2 */
    { 8,                                                                                               {}},
 /* CH344Q */
    { 8,                                                                                               {}},
 /* CH347T */
    { 4,                                                                                               {}},
 /* CH9101UH */
    { 5,                                                 {{3, 2}, {3, 3}, {1, 3}, {1, 2}, {1, 5}, {2, 4}}},
 /* CH9101RY */
    { 4,                                                                 {{1, 3}, {3, 3}, {3, 2}, {2, 4}}},
 /* CH9102F */
    { 5,                                                         {{2, 1}, {2, 7}, {2, 4}, {2, 6}, {2, 3}}},
 /* CH9102X */
    { 6,                                                 {{2, 3}, {2, 5}, {2, 1}, {2, 7}, {3, 0}, {2, 2}}},
 /* CH9103M */
    {12, {{1, 3}, {1, 2}, {3, 2}, {2, 6}, {1, 0}, {1, 6}, {2, 3}, {2, 5}, {3, 0}, {2, 2}, {1, 5}, {2, 4}}},
 /* CH9104L */
    {24,                                                                                               {}},
};

struct ch343 {
    struct usb_device *dev;        /* the corresponding usb device */
    struct usb_interface *control; /* control interface */
    struct usb_interface *data;    /* data interface */
    struct tty_port port;          /* our tty port data */
    struct urb *ctrlurb;           /* urbs */
    u8 *ctrl_buffer;               /* buffers of urbs */
    dma_addr_t ctrl_dma;           /* dma handles of buffers */
    struct ch343_wb wb[CH343_NW];
    unsigned long read_urbs_free;
    struct urb *read_urbs[CH343_NR];
    struct ch343_rb read_buffers[CH343_NR];
    int rx_buflimit;
    int rx_endpoint;
    spinlock_t read_lock;
    int write_used; /* number of non-empty write buffers */
    int transmitting;
    spinlock_t write_lock;
    struct mutex mutex;
    bool disconnected;
    struct usb_ch343_line_coding line; /* bits, stop, parity */
    struct work_struct work;           /* work queue entry for line discipline waking up */
    unsigned int ctrlin;               /* input control lines (DCD, DSR, RI, break, overruns) */
    unsigned int ctrlout;              /* output control lines (DTR, RTS) */
    struct async_icount iocount;       /* counters for control line changes */
    struct async_icount oldcount;      /* for comparison of counter */
    wait_queue_head_t wioctl;          /* for ioctl */
    unsigned int writesize;            /* max packet size for the output bulk endpoint */
    unsigned int readsize, ctrlsize;   /* buffer sizes for freeing */
    unsigned int minor;                /* ch343 minor number */
    unsigned char clocal;              /* termios CLOCAL */
    unsigned int susp_count;           /* number of suspended interfaces */
    u8 bInterval;
    struct usb_anchor delayed; /* writes queued for a device about to be woken */
    unsigned long quirks;
    u8 iface;
    CHIPTYPE chiptype;
    u16 idVendor;
    u16 idProduct;
    u8 gpio5dir;
};

#define CDC_DATA_INTERFACE_TYPE 0x0a

/* constants describing various quirks and errors */
#define NO_UNION_NORMAL          BIT(0)
#define SINGLE_RX_URB            BIT(1)
#define NO_CAP_LINE              BIT(2)
#define NO_DATA_INTERFACE        BIT(4)
#define IGNORE_DEVICE            BIT(5)
#define QUIRK_CONTROL_LINE_STATE BIT(6)
#define CLEAR_HALT_CONDITIONS    BIT(7)

#endif

/*
 * File      : tcpdump.c
 * This is file that captures the IP message based on the RT-Thread
 * and saves in the file system.
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-07-13     never        the first version
 */

#include <rtthread.h>
#include <dfs_posix.h>
#include "netif/ethernetif.h"
#include "optparse.h"

#ifdef PKG_NETUTILS_TCPDUMP_DBG
    #define DBG_ENABLE

    #define DBG_SECTION_NAME  "TCPDUMP"
    #define DBG_LEVEL         DBG_INFO
    #define DBG_COLOR
#else
    #undef  DBG_ENABLE
#endif
#include <rtdbg.h>

#define TCPDUMP_PIPE_DEVICE         ("urdbd")           /* rdb pipe */

#define TCPDUMP_DEFAULT_NANE        ("sample.pcap")

#define TCPDUMP_MAX_MSG             (10)
#define PCAP_FILE_HEADER_SIZE       (24)
#define PCAP_PKTHDR_SIZE            (16)

#define PCAP_FILE_ID                (0xA1B2C3D4)
#define PCAP_VERSION_MAJOR          (0x200)
#define PCAP_VERSION_MINOR          (0x400)
#define GREENWICH_MEAN_TIME         (0)
#define PRECISION_OF_TIME_STAMP     (0)
#define MAX_LENTH_OF_CAPTURE_PKG    (0xFFFF)

#define LINKTYPE_NULL               (0)
#define LINKTYPE_ETHERNET           (1)                 /* also for 100Mb and up */
#define LINKTYPE_EXP_ETHERNET       (2)                 /* 3Mb experimental Ethernet */
#define LINKTYPE_AX25               (3)
#define LINKTYPE_PRONET             (4)
#define LINKTYPE_CHAOS              (5)
#define LINKTYPE_TOKEN_RING         (6)                 /* DLT_IEEE802 is used for Token Ring */
#define LINKTYPE_ARCNET             (7)
#define LINKTYPE_SLIP               (8)
#define LINKTYPE_PPP                (9)
#define LINKTYPE_FDDI               (10)
#define LINKTYPE_PPP_HDLC           (50)                /* PPP in HDLC-like framing */
#define LINKTYPE_PPP_ETHER          (51)                /* NetBSD PPP-over-Ethernet */
#define LINKTYPE_ATM_RFC1483        (100)               /* LLC/SNAP-encapsulated ATM */
#define LINKTYPE_RAW                (101)               /* raw IP */
#define LINKTYPE_SLIP_BSDOS         (102)               /* BSD/OS SLIP BPF header */
#define LINKTYPE_PPP_BSDOS          (103)               /* BSD/OS PPP BPF header */
#define LINKTYPE_C_HDLC             (104)               /* Cisco HDLC */
#define LINKTYPE_IEEE802_11         (105)               /* IEEE 802.11 (wireless) */
#define LINKTYPE_ATM_CLIP           (106)               /* Linux Classical IP over ATM */
#define LINKTYPE_LOOP               (108)               /* OpenBSD loopback */
#define LINKTYPE_LINUX_SLL          (113)               /* Linux cooked socket capture */
#define LINKTYPE_LTALK              (114)               /* Apple LocalTalk hardware */
#define LINKTYPE_ECONET             (115)               /* Acorn Econet */
#define LINKTYPE_CISCO_IOS          (118)               /* For Cisco-internal use */
#define LINKTYPE_PRISM_HEADER       (119)               /* 802.11+Prism II monitor mode */
#define LINKTYPE_AIRONET_HEADER     (120)               /* FreeBSD Aironet driver stuff */

#define MSH_CMD ("phi::m::w::")                         /* [-p] [-h] [-i] [-m] [-w] */
#define STRCMP(a, R, b)   (rt_strcmp((a), (b)) R 0)

#define PACP_FILE_HEADER_CREATE(_head)                          \
do {                                                            \
    (_head)->magic = PCAP_FILE_ID;                              \
    (_head)->version_major = PCAP_VERSION_MAJOR;                \
    (_head)->version_minor = PCAP_VERSION_MINOR;                \
    (_head)->thiszone = GREENWICH_MEAN_TIME;                    \
    (_head)->sigfigs = PRECISION_OF_TIME_STAMP;                 \
    (_head)->snaplen = MAX_LENTH_OF_CAPTURE_PKG;                \
    (_head)->linktype = LINKTYPE_ETHERNET;                      \
} while (0)

#define PACP_PKTHDR_CREATE(_head, _p)                           \
do{                                                             \
    (_head)->ts.tv_sec = rt_tick_get() / RT_TICK_PER_SECOND;    \
    (_head)->ts.tv_msec = rt_tick_get() % RT_TICK_PER_SECOND;   \
    (_head)->caplen = _p->tot_len;                              \
    (_head)->len = _p->tot_len;                                 \
} while (0)

struct rt_pcap_file_header
{
    rt_uint32_t magic;
    rt_uint16_t version_major;
    rt_uint16_t version_minor;
    rt_int32_t thiszone;
    rt_uint32_t sigfigs;
    rt_uint32_t snaplen;
    rt_uint32_t linktype;
};

struct rt_timeval
{
    rt_uint32_t tv_sec;
    rt_uint32_t tv_msec;
};

struct rt_pcap_pkthdr
{
    struct rt_timeval ts;
    rt_uint32_t caplen;
    rt_uint32_t len;
};

enum rt_tcpdump_return_param
{
    STOP = -2,
    HELP = -3,
};

static struct rt_device *tcpdump_pipe;
static struct rt_mailbox *tcpdump_mb;

static struct netif *netif;
static netif_linkoutput_fn link_output;
static netif_input_fn input;

static const char *name;
static char *filename;
static const char *eth;
static char *ethname;
static const char *mode;
static int fd = -1;

static void rt_tcpdump_filename_del(void);
static void rt_tcpdump_ethname_del(void);
static void rt_tcpdump_error_info_deal(void);
static void rt_tcpdump_init_indicate(void);
static rt_err_t rt_tcpdump_pcap_file_save(const void *buf, int len);

static rt_err_t (*tcpdump_write)(const void *buf, int len);

#ifdef  PKG_NETUTILS_TCPDUMP_PRINT
#define __is_print(ch) ((unsigned int)((ch) - ' ') < 127u - ' ')
static void hex_dump(const rt_uint8_t *ptr, rt_size_t buflen)
{
    unsigned char *buf = (unsigned char *)ptr;
    int i, j;

    RT_ASSERT(ptr != RT_NULL);

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
#endif

/* get tx data */
static err_t _netif_linkoutput(struct netif *netif, struct pbuf *p)
{
    RT_ASSERT(netif != RT_NULL);

    if (p != RT_NULL)
    {
        pbuf_ref(p);

        if (rt_mb_send(tcpdump_mb, (rt_uint32_t)p) != RT_EOK)
        {
            pbuf_free(p);
        }
    }
    return link_output(netif, p);
}

/* get rx data */
static err_t _netif_input(struct pbuf *p, struct netif *inp)
{
    RT_ASSERT(inp != RT_NULL);

    if (p != RT_NULL)
    {
        pbuf_ref(p);
        if (rt_mb_send(tcpdump_mb, (rt_uint32_t)p) != RT_EOK)
        {
            pbuf_free(p);
        }
    }
    return input(p, inp);
}

/* import pcap file into your PC through file-system */
static rt_err_t rt_tcpdump_pcap_file_write(const void *buf, int len)
{
    int length;

    if (filename == RT_NULL)
    {
        dbg_log(DBG_ERROR, "file name is null!\n");
        return -RT_ERROR;
    }

    if ((len == 0) && (fd > 0))
    {
        dbg_log(DBG_ERROR, "ip mess error and close file!\n");
        close(fd);
        fd = -1;
        return -RT_ERROR;
    }

    if (fd < 0)
    {
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0);
        if (fd < 0)
        {
            dbg_log(DBG_ERROR, "open file failed!\n");
            return -RT_ERROR;
        }
    }

    length = write(fd, buf, len);
    if (length != len)
    {
        dbg_log(DBG_ERROR, "write data failed, length: %d\n", length);
        close(fd);
        return -RT_ERROR;
    }

    return RT_EOK;
}

/* Import pcap file into your PC through rdb tools */
static rt_err_t rt_tcpdump_pcap_file_save(const void *buf, int len)
{
    rt_device_write(tcpdump_pipe, 0, buf, len);
    return RT_EOK;
}

/* write ip mess and print */
static void rt_tcpdump_ip_mess_write(struct pbuf *p)
{
    rt_uint8_t *buf = (rt_uint8_t *)rt_malloc(p->tot_len);

    RT_ASSERT(buf != RT_NULL);

    pbuf_copy_partial(p, buf, p->tot_len, 0);

#ifdef PKG_NETUTILS_TCPDUMP_PRINT
    hex_dump(buf, p->tot_len);
#endif

    /* write ip mess */
    if (tcpdump_write != RT_NULL)
        tcpdump_write(buf, p->tot_len);

    rt_free(buf);
}

/* write pcap file header */
static rt_err_t rt_tcpdump_pcap_file_init(void)
{
    struct rt_pcap_file_header file_header;
    int res = -1;

    if (tcpdump_pipe != RT_NULL)
    {
        if (rt_device_open(tcpdump_pipe, RT_DEVICE_OFLAG_WRONLY) != RT_EOK)
        {
            dbg_log(DBG_LOG, "not found pipe device!\n");
            return -RT_ERROR;
        }
    }

    /* in rdb mode does not need to write pcap file header */
    if ((tcpdump_write != RT_NULL) && (tcpdump_write == rt_tcpdump_pcap_file_write))
    {
        PACP_FILE_HEADER_CREATE(&file_header);
        res = tcpdump_write(&file_header, sizeof(file_header));
    }

#ifdef  PKG_NETUTILS_TCPDUMP_PRINT
    hex_dump((rt_uint8_t *)&file_header, PCAP_FILE_HEADER_SIZE);
#endif

    if (res != RT_EOK)
        return -RT_ERROR;

    return RT_EOK;
}

static void rt_tcpdump_thread_entry(void *param)
{
    struct pbuf *pbuf = RT_NULL;
    struct rt_pcap_pkthdr pkthdr;
    rt_uint32_t mbval;

    while (1)
    {
        if (rt_mb_recv(tcpdump_mb, &mbval, RT_WAITING_FOREVER) == RT_EOK)
        {
            pbuf = (struct pbuf *)mbval;
            RT_ASSERT(pbuf != RT_NULL);

            /* write pkthdr */
            if ((tcpdump_write != RT_NULL) && (tcpdump_write == rt_tcpdump_pcap_file_write))
            {
                PACP_PKTHDR_CREATE(&pkthdr, pbuf);
                tcpdump_write(&pkthdr, sizeof(pkthdr));
            }

#ifdef  PKG_NETUTILS_TCPDUMP_PRINT
            hex_dump((rt_uint8_t *)&pkthdr, PCAP_PKTHDR_SIZE);
#endif
            rt_tcpdump_ip_mess_write(pbuf);
            pbuf_free(pbuf);
            pbuf = RT_NULL;
        }

        /* tcpdump deinit, the mailbox does not receive the data, exits the thread*/
        else
        {
            dbg_log(DBG_INFO, "tcpdump stop and tcpdump thread exit!\n");
            close(fd);
            fd = -1;
            
            if (tcpdump_pipe != RT_NULL)
                rt_device_close((rt_device_t)tcpdump_pipe);

            tcpdump_write = RT_NULL;
            rt_tcpdump_filename_del();
            rt_tcpdump_ethname_del();
            return;
        }
    }
}

/* set file name */
static void rt_tcpdump_filename_set(const char *name)
{
    filename = rt_strdup(name);
}

/* delete file name */
static void rt_tcpdump_filename_del(void)
{
    name = RT_NULL;
    if (filename != RT_NULL)
        rt_free(filename);

    filename = RT_NULL;
}

/* set network interface name */
static void rt_tcpdump_ethname_set(const char *eth)
{
    ethname = rt_strdup(eth);
}

/* delete network interface name */
static void rt_tcpdump_ethname_del(void)
{
    eth = RT_NULL;
    if (ethname != RT_NULL)
        rt_free(ethname);
}

static int rt_tcpdump_init(void)
{
    struct eth_device *device;

    rt_thread_t tid;
    rt_base_t level;

    if (netif != RT_NULL)
    {
        dbg_log(DBG_ERROR, "This command is running, please stop before you use the [tcpdump -p] command!\n");
        return -RT_ERROR;
    }

    /* print and set default state */
    rt_tcpdump_init_indicate();

    tcpdump_pipe = rt_device_find(TCPDUMP_PIPE_DEVICE);
    /* file-system mode does not judge pipe */
    if (tcpdump_write != rt_tcpdump_pcap_file_write)
    {
        if (tcpdump_pipe == RT_NULL)
        {
            dbg_log(DBG_ERROR, "pipe is error!\n");
            return -RT_ERROR;
        }
    }

    device = (struct eth_device *)rt_device_find(eth);
    if (device == RT_NULL)
    {
        dbg_log(DBG_ERROR, "network interface card [%s] device not find!\n", eth);
        dbg_log(DBG_ERROR, "tcpdump thread startup failed and enter the correct network interface please!\n");
        return -RT_ERROR;
    }
    if ((device->netif == RT_NULL) || (device->netif->linkoutput == RT_NULL))
    {
        dbg_log(DBG_ERROR, "this device not e0!\n");
        return -RT_ERROR;
    }

    tcpdump_mb = rt_mb_create("tdrmb", TCPDUMP_MAX_MSG, RT_IPC_FLAG_FIFO);
    if (tcpdump_mb == RT_NULL)
    {
        dbg_log(DBG_ERROR, "tcp dump mp create fail!\n");
        return -RT_ERROR;
    }

    tid = rt_thread_create("tcpdump", rt_tcpdump_thread_entry, RT_NULL, 2048, 12, 10);
    if (tid == RT_NULL)
    {
        rt_mb_delete(tcpdump_mb);
        tcpdump_mb = RT_NULL;
        dbg_log(DBG_ERROR, "tcpdump thread create fail!\n");
        return -RT_ERROR;
    }

    rt_tcpdump_filename_set(name);
    rt_tcpdump_ethname_set(eth);

    netif = device->netif;

    /* linkoutput and input init */
    level = rt_hw_interrupt_disable();
    link_output = netif->linkoutput;
    netif->linkoutput = _netif_linkoutput;

    input = netif->input;
    netif->input = _netif_input;
    rt_hw_interrupt_enable(level);
    /* linkoutput and input init */

    /* write pcap file header */
    rt_tcpdump_pcap_file_init();

    rt_thread_startup(tid);

    dbg_log(DBG_INFO, "tcpdump start!\n");

    return RT_EOK;
}

static void rt_tcpdump_deinit(void)
{
    rt_base_t level;

    if (netif == RT_NULL)
    {
        dbg_log(DBG_ERROR, "capture packet stopped, no repeat input required!\n");
        return;
    }

    /* linkoutput and input deinit */
    level = rt_hw_interrupt_disable();
    netif->linkoutput = link_output;
    netif->input = input;
    netif = RT_NULL;
    rt_hw_interrupt_enable(level);
    /* linkoutput and input deinit */

    rt_mb_delete(tcpdump_mb);
    tcpdump_mb = RT_NULL;
}

static void rt_tcpdump_help_info_print(void)
{
    rt_kprintf("\n");
    rt_kprintf("|>------------------------- help -------------------------<|\n");
    rt_kprintf("| tcpdump [-p] [-h] [-i interface] [-m mode] [-w file]     |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| -h: help                                                 |\n");
    rt_kprintf("| -i: specify the network interface for listening          |\n");
    rt_kprintf("| -m: choose what mode(file-system or rdb) to save the file|\n");
    rt_kprintf("| -w: write the captured packets into an xx.pcap file      |\n");
    rt_kprintf("| -p: stop capturing packets                               |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| e.g.:                                                    |\n");
    rt_kprintf("| specify network interface and select save mode \\         |\n");
    rt_kprintf("| and specify filename                                     |\n");
    rt_kprintf("| tcpdump -ie0 -mfile -wtext.pcap                          |\n");
    rt_kprintf("| tcpdump -ie0 -mrdb -wtext.pcap                           |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| -m: file-system mode                                     |\n");
    rt_kprintf("| tcpdump -mfile                                           |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| -m: rdb mode                                             |\n");
    rt_kprintf("| tcpdump -mrdb                                            |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| -w: file                                                 |\n");
    rt_kprintf("| tcpdump -wtext.pcap                                      |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| -p: stop                                                 |\n");
    rt_kprintf("| tcpdump -p                                               |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| -h: help                                                 |\n");
    rt_kprintf("| tcpdump -h                                               |\n");
    rt_kprintf("|                                                          |\n");
    rt_kprintf("| write commands but no arguments are illegal!!            |\n");
    rt_kprintf("| e.g.: tcpdump -i / -i -mfile  / -i -mfile -wtext.pcap    |\n");
    rt_kprintf("|>------------------------- help -------------------------<|\n");
    rt_kprintf("\n");
}

static void rt_tcpdump_error_info_deal(void)
{
    dbg_log(DBG_ERROR, "tcpdump command is incorrect, please refer to the help information!\n");
    rt_tcpdump_help_info_print();
}

/* print and set default state */
static void rt_tcpdump_init_indicate(void)
{
    int name_flag = 0, eth_flag = 0, mode_flag = 0;

    if (eth == RT_NULL)
    {
        rt_kprintf("[TCPDUMP]default selection [e0] network interface\n");
        eth = "e0";
        eth_flag = 1;
    }

    if (tcpdump_write == RT_NULL)
    {
        rt_kprintf("[TCPDUMP]default selection [file-system] mode\n");
        tcpdump_write = rt_tcpdump_pcap_file_write;
        mode_flag = 1;
    }

    if (name == RT_NULL)
    {
        rt_kprintf("[TCPDUMP]default save in [sample.pcap]\n");
        name = TCPDUMP_DEFAULT_NANE;
        name_flag = 1;
    }

    if (eth_flag == 0)
        rt_kprintf("[TCPDUMP]select  [%s] network interface\n", eth);

    if (mode_flag == 0)
    {
        if (STRCMP(mode, ==, "file"))
            rt_kprintf("[TCPDUMP]select  [file-system] mode\n");

        if (STRCMP(mode, ==, "rdb"))
            rt_kprintf("[TCPDUMP]select  [rdb] mode\n");
    }

    if (name_flag == 0)
        rt_kprintf("[TCPDUMP]save in [%s]\n", name);
}

/* msh command-line deal */
static int rt_tcpdump_cmd_deal(struct optparse *options)
{
    switch (options->optopt)
    {
    case 'p':
        rt_tcpdump_deinit();
        return STOP;

    case 'h':
        rt_tcpdump_help_info_print();
        return HELP;

    case 'i':
        /* it's illegal without parameters. */
        if (options->optarg == RT_NULL)
            return -RT_ERROR;

        eth = options->optarg;
        return RT_EOK;

    case 'm':
        if (options->optarg == RT_NULL)
            return -RT_ERROR;

        if (STRCMP(options->optarg, ==, "file"))
        {
            mode = options->optarg;
            tcpdump_write = rt_tcpdump_pcap_file_write;
            return RT_EOK;
        }

        if (STRCMP(options->optarg, ==, "rdb"))
        {
            mode = options->optarg;
            tcpdump_write = rt_tcpdump_pcap_file_save;
            return RT_EOK;
        }

        /* User input Error */
        return -RT_ERROR;

    case 'w':
        if (options->optarg == RT_NULL)
            return -RT_ERROR;

        name = options->optarg;
        break;

    default:
        return -RT_ERROR;
    }

    return RT_EOK;
}

/* msh command-line parsing */
static int rt_tcpdump_cmd_parse(char *argv[], const char *cmd)
{
    int ch, res, invalid_argv = 0;
    struct optparse options;

    optparse_init(&options, argv);

    while ((ch = optparse(&options, cmd)) != -1)
    {
        ch = ch;
        invalid_argv = 1;

        switch (options.optopt)
        {
        case 'p':
            return rt_tcpdump_cmd_deal(&options);

        case 'h':
            return rt_tcpdump_cmd_deal(&options);

        case 'i':
            res = rt_tcpdump_cmd_deal(&options);
            break;

        case 'm':
            res = rt_tcpdump_cmd_deal(&options);
            break;

        case 'w':
            res = rt_tcpdump_cmd_deal(&options);
            break;

        default:
            rt_tcpdump_error_info_deal();
            return -RT_ERROR;
        }

        if (res == -RT_ERROR)
        {
            rt_tcpdump_error_info_deal();
            return res;
        }
    }

    /* judge invalid command */
    if (invalid_argv == 0)
    {
        rt_tcpdump_error_info_deal();
        return -RT_ERROR;
    }

    return RT_EOK;
}

static void rt_tcpdump_cmd_argv_deinit(void)
{
    eth = RT_NULL;
    tcpdump_write = RT_NULL;
    name = RT_NULL;
}

static int tcpdump_test(int argc, char *argv[])
{
    int res = 0;

    if (argc == 1)
    {
        rt_tcpdump_cmd_argv_deinit();
        rt_tcpdump_init();
        return RT_EOK;
    }

    rt_tcpdump_cmd_argv_deinit();

    res = rt_tcpdump_cmd_parse(argv, MSH_CMD);

    if (res == STOP)
        return RT_EOK;

    if (res == HELP)
        return RT_EOK;

    if (res == -RT_ERROR)
        return -RT_ERROR;

    rt_tcpdump_init();

    return RT_EOK;
}
#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT_ALIAS(tcpdump_test, tcpdump, test optparse_short cmd.);
#endif
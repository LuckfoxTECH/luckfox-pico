/*
 *
 * (C) 2014 David Lettier.
 * (C) 2018 Armink (armink.ztl@gmail.com)
 *
 * http://www.lettier.com/
 *
 * NTP client.
 *
 * Compiled with gcc version 4.7.2 20121109 (Red Hat 4.7.2-8) (GCC).
 *
 * Tested on Linux 3.8.11-200.fc18.x86_64 #1 SMP Wed May 1 19:44:27 UTC 2013 x86_64 x86_64 x86_64 GNU/Linux.
 * Tested on RT-Thread 3.0.0+
 *
 * To compile: $ gcc main.c -o ntpClient.out
 *
 * Usage: $ ./ntpClient.out
 *
 */

#include <rtthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <rtdevice.h>

#if defined(RT_USING_SAL) && defined(RT_USING_NETDEV)
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#elif defined(RT_USING_LWIP)
#include <lwip/inet.h>
#include <lwip/sockets.h>
#include <lwip/netdb.h>
extern struct hostent *gethostbyname(const char *name);
extern int recvfrom(int s, void *mem, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);
extern int sendto(int s, const void *dataptr, size_t size, int flags, const struct sockaddr *to, socklen_t tolen);
extern int socket(int domain, int type, int protocol);
extern int closesocket(int s);
#endif /* RT_USING_SAL */

#if defined(RT_USING_NETDEV)
#include <netdev.h>
#elif defined(RT_USING_LWIP)
#include <lwip/netif.h>
#endif /* RT_USING_NETDEV */

#define DBG_SECTION_NAME               "ntp"
#define DBG_LEVEL                      DBG_INFO
#include <rtdbg.h>

#ifdef NETUTILS_NTP_TIMEZONE
#define NTP_TIMEZONE                   NETUTILS_NTP_TIMEZONE
#endif

#ifdef NETUTILS_NTP_HOSTNAME
#define NTP_HOSTNAME1                  NETUTILS_NTP_HOSTNAME
#else
#define NTP_HOSTNAME1                  NULL
#endif

#ifdef NETUTILS_NTP_HOSTNAME2
#define NTP_HOSTNAME2                  NETUTILS_NTP_HOSTNAME2
#else
#define NTP_HOSTNAME2                  NULL
#endif

#ifdef NETUTILS_NTP_HOSTNAME3
#define NTP_HOSTNAME3                  NETUTILS_NTP_HOSTNAME3
#else
#define NTP_HOSTNAME3                  NULL
#endif

#define NTP_TIMESTAMP_DELTA            2208988800ull

#ifndef NTP_TIMEZONE
#define NTP_TIMEZONE                   8
#endif

#define LI(packet)   (uint8_t) ((packet.li_vn_mode & 0xC0) >> 6) // (li   & 11 000 000) >> 6
#define VN(packet)   (uint8_t) ((packet.li_vn_mode & 0x38) >> 3) // (vn   & 00 111 000) >> 3
#define MODE(packet) (uint8_t) ((packet.li_vn_mode & 0x07) >> 0) // (mode & 00 000 111) >> 0

// Structure that defines the 48 byte NTP packet protocol.
typedef struct {

    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                             // li.   Two bits.   Leap indicator.
                             // vn.   Three bits. Version number of the protocol.
                             // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.

} ntp_packet;              // Total: 384 bits or 48 bytes.

static ntp_packet packet = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

static int sendto_ntp_server(int sockfd, const char *host_name, struct sockaddr_in *serv_addr)
{
    struct hostent *server;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    /* NTP UDP port number. */
    int portno = 123;

    server = gethostbyname(host_name);
    if (server == NULL)
    {
        LOG_D("No such host(%s)", host_name);
        return -RT_ERROR;
    }
    else
    {
        /* Zero out the server address structure. */
        memset((char *)serv_addr, 0, addr_len);

        serv_addr->sin_family = AF_INET;

        /* Convert the port number integer to network big-endian style and save it to the server address structure. */
        serv_addr->sin_port = htons(portno);

        /* Copy the server's IP address to the server address structure. */
        memcpy(&serv_addr->sin_addr.s_addr, (char *) server->h_addr, server->h_length);

        return sendto(sockfd, (char *) &packet, sizeof(ntp_packet), 0, (const struct sockaddr *)serv_addr, addr_len);
    }
}

/**
 * Get the UTC time from NTP server
 *
 * @param host_name NTP server host name, NULL: will using default host name
 *
 * @note this function is not reentrant
 *
 * @return >0: success, current UTC time
 *         =0: get failed
 */
time_t ntp_get_time(const char *host_name)
{
/* the delay(ms) between two receive */
#define RECV_TIME_DELAY_MS             10
/* NTP receive timeout(S) */
#define NTP_GET_TIMEOUT                5
/* number of NTP servers */
#define NTP_SERVER_NUM                 3

    int sockfd, n, i = 0, server_num = 0;
    struct sockaddr_in serv_addr[NTP_SERVER_NUM];
    rt_tick_t start = 0;
    time_t new_time = 0;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    const char *const host_name_buf[NTP_SERVER_NUM] = {NTP_HOSTNAME1, NTP_HOSTNAME2, NTP_HOSTNAME3};

    /* Create and zero out the packet. All 48 bytes worth. */
    memset(&packet, 0, sizeof(ntp_packet));

    /* Set the first byte's bits to 00,011,011 for li = 0, vn = 3, and mode = 3. The rest will be left set to zero.
       Represents 27 in base 10 or 00011011 in base 2. */
    *((char *) &packet + 0) = 0x1b;

#if defined(RT_USING_NETDEV) || defined(RT_USING_LWIP)
    {   
        #define NTP_INTERNET           0x02
        #define NTP_INTERNET_BUFF_LEN  18
        #define NTP_INTERNET_MONTH_LEN 4
        #define NTP_INTERNET_DATE_LEN  16
        #ifndef SW_VER_NUM
        #define SW_VER_NUM             0x00000000
        #endif

        const char month[][NTP_INTERNET_MONTH_LEN] = 
            {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
        char date[NTP_INTERNET_DATE_LEN] = {0};
        uint8_t send_data[NTP_INTERNET_BUFF_LEN] = {0};
        uint8_t index, moth_num = 0;
        uint16_t check = 0;

        /* get build moth value*/
        rt_snprintf(date, NTP_INTERNET_DATE_LEN, "%s", __DATE__);

        for (index = 0; index < sizeof(month) / NTP_INTERNET_MONTH_LEN; index++)
        {
            if (rt_memcmp(date, month[index], NTP_INTERNET_MONTH_LEN - 1) == 0)
            {
                moth_num = index + 1;
                break;
            }
        }

        send_data[0] = NTP_INTERNET;

        /* get hardware address */
        {
#if defined(RT_USING_LWIP) && !defined(RT_USING_NETDEV)
            #define netdev netif
            #define netdev_default netif_default
#endif
            extern struct netdev *netdev_default;
            struct netdev *dev = netdev_default;

            for (index = 0; index < dev->hwaddr_len; index++)
            {
                send_data[index + 1] = dev->hwaddr[index] + moth_num;
            }
        }

        send_data[9] = RT_VERSION;
        send_data[10] = RT_SUBVERSION;
        send_data[11] = RT_REVISION;
        send_data[12] = (uint8_t)(SW_VER_NUM >> 24);
        send_data[13] = (uint8_t)(SW_VER_NUM >> 16);
        send_data[14] = (uint8_t)(SW_VER_NUM >> 8);
        send_data[15] = (uint8_t)(SW_VER_NUM & 0xFF);

        /* get the check value */
        for (index = 0; index < NTP_INTERNET_BUFF_LEN - sizeof(check); index++)
        {
            check += (uint8_t)send_data[index];
        }
        send_data[NTP_INTERNET_BUFF_LEN - 2] = check >> 8;
        send_data[NTP_INTERNET_BUFF_LEN - 1] = check & 0xFF;

        rt_memcpy(((char *)&packet + 4), send_data, NTP_INTERNET_BUFF_LEN);
    }
#endif /* RT_USING_NETDEV || RT_USING_LWIP */
    

    /* Create a UDP socket. */
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0)
    {
        LOG_E("Create socket failed");
        return 0;
    }

    if (host_name)
    {
        /* access the incoming host_name server */
        if (sendto_ntp_server(sockfd, host_name, serv_addr) >= 0)
        {
            server_num = 1;
        }
    }
    else
    {
        /* use the static default NTP server */
        for (i = 0; i < NTP_SERVER_NUM; i++)
        {
            if (host_name_buf[i] == NULL || strlen(host_name_buf[i]) == 0)
                continue;

            if (sendto_ntp_server(sockfd, host_name_buf[i], &serv_addr[server_num]) >= 0)
            {
                server_num ++;
            }
        }
    }

    if (server_num <= 0)
    {
        closesocket(sockfd);
        return 0;
    }

    start = rt_tick_get();
    while (rt_tick_get() <= start + NTP_GET_TIMEOUT * RT_TICK_PER_SECOND)
    {
        for (i = 0; i < server_num; i++)
        {
            /* non-blocking receive the packet back from the server. If n == -1, it failed. */
            n = recvfrom(sockfd, (char *) &packet, sizeof(ntp_packet), MSG_DONTWAIT, (struct sockaddr *)&serv_addr[i], &addr_len);
            if (n <= 0)
            {
                LOG_D("Reading from server %s error (%d).", inet_ntoa(serv_addr[i].sin_addr.s_addr), n);
            }
            else if (n > 0)
            {
                goto __exit;
            }
        }

        rt_thread_mdelay(RECV_TIME_DELAY_MS);
    }

__exit:

    if (rt_tick_get() <= start + NTP_GET_TIMEOUT * RT_TICK_PER_SECOND)
    {
        /* These two fields contain the time-stamp seconds as the packet left the NTP server.
           The number of seconds correspond to the seconds passed since 1900.
           ntohl() converts the bit/byte order from the network's to host's "endianness". */
        packet.txTm_s = ntohl(packet.txTm_s); // Time-stamp seconds.
        packet.txTm_f = ntohl(packet.txTm_f); // Time-stamp fraction of a second.

        /* Extract the 32 bits that represent the time-stamp seconds (since NTP epoch) from when the packet left the server.
           Subtract 70 years worth of seconds from the seconds since 1900.
           This leaves the seconds since the UNIX epoch of 1970.
           (1900)------------------(1970)**************************************(Time Packet Left the Server) */
        new_time = (time_t)(packet.txTm_s - NTP_TIMESTAMP_DELTA);
    }
    else
    {
        LOG_E("Receive the socket from server timeout (%dS).", NTP_GET_TIMEOUT);
    }

    closesocket(sockfd);

    return new_time;
}

/**
 * Get the local time from NTP server
 *
 * @param host_name NTP server host name, NULL: will using default host name
 *
 * @return >0: success, current local time, offset timezone by NTP_TIMEZONE
 *         =0: get failed
 */
time_t ntp_get_local_time(const char *host_name)
{
    time_t cur_time = ntp_get_time(host_name);

    if (cur_time)
    {
        /* add the timezone offset for set_time/set_date */
        cur_time += NTP_TIMEZONE * 3600;
    }

    return cur_time;
}

/**
 * Sync current local time to RTC by NTP
 *
 * @param host_name NTP server host name, NULL: will using default host name
 *
 * @return >0: success, current local time, offset timezone by NTP_TIMEZONE
 *         =0: sync failed
 */
time_t ntp_sync_to_rtc(const char *host_name)
{
#ifdef RT_USING_RTC
    struct tm *cur_tm;
#endif

    time_t cur_time = ntp_get_local_time(host_name);

    if (cur_time)
    {

#ifdef RT_USING_RTC
        cur_tm = localtime(&cur_time);
        set_time(cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);

        cur_tm = localtime(&cur_time);
        set_date(cur_tm->tm_year + 1900, cur_tm->tm_mon + 1, cur_tm->tm_mday);
#endif /* RT_USING_RTC */

    }

    return cur_time;
}

static void ntp_sync(const char *host_name)
{
    time_t cur_time = ntp_sync_to_rtc(host_name);

    if (cur_time)
    {
        rt_kprintf("Get local time from NTP server: %s", ctime((const time_t *) &cur_time));

#ifdef RT_USING_RTC
        rt_kprintf("The system time is updated. Timezone is %d.\n", NTP_TIMEZONE);
#else
        rt_kprintf("The system time update failed. Please enable RT_USING_RTC.\n");
#endif /* RT_USING_RTC */

    }
}

static void cmd_ntp_sync(int argc, char **argv)
{
    char *host_name = NULL;

    if (argc > 1)
    {
        host_name = argv[1];
    }

    ntp_sync(host_name);
}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(ntp_sync, Update time by NTP(Network Time Protocol): ntp_sync(host_name));
MSH_CMD_EXPORT_ALIAS(cmd_ntp_sync, ntp_sync, Update time by NTP(Network Time Protocol): ntp_sync [host_name]);
#endif /* RT_USING_FINSH */

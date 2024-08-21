/**
* iperf-liked network performance tool
*
*/

#include <rtthread.h>
#include <rtdevice.h>

#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "netdb.h"

#define IPERF_PORT          5001
#define IPERF_BUFSZ         (4 * 1024)

#define IPERF_MODE_STOP     0
#define IPERF_MODE_SERVER   1
#define IPERF_MODE_CLIENT   2

typedef struct
{
    int mode;

    char *host;
    int port;
} IPERF_PARAM;
static IPERF_PARAM param = {IPERF_MODE_STOP, NULL, IPERF_PORT};

static void iperf_udp_client(void *thread_param)
{
    int sock;
    rt_uint32_t *buffer;
    struct sockaddr_in server;
    rt_uint32_t packet_count = 0;
    rt_uint32_t tick;
    int send_size;

    send_size = IPERF_BUFSZ > 1470 ? 1470 : IPERF_BUFSZ;
    buffer = malloc(IPERF_BUFSZ);
    if (buffer == NULL)
    {
        return;
    }
    memset(buffer, 0x00, IPERF_BUFSZ);
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        rt_kprintf("can't create socket!! exit\n");
        return;
    }
    server.sin_family = PF_INET;
    server.sin_port = htons(param.port);
    server.sin_addr.s_addr = inet_addr(param.host);
    rt_kprintf("iperf udp mode run...\n");
    while (param.mode != IPERF_MODE_STOP)
    {
        packet_count++;
        tick = rt_tick_get();
        buffer[0] = htonl(packet_count);
        buffer[1] = htonl(tick / RT_TICK_PER_SECOND);
        buffer[2] = htonl((tick % RT_TICK_PER_SECOND) * 1000);
        sendto(sock, buffer, send_size, 0, (struct sockaddr *)&server, sizeof(struct sockaddr_in));
    }
    closesocket(sock);
    free(buffer);
}

static void iperf_udp_server(void *thread_param)
{
    int sock;
    rt_uint32_t *buffer;
    struct sockaddr_in server;
    struct sockaddr_in sender;
    int sender_len, r_size;
    rt_uint64_t sentlen;
    rt_uint32_t pcount = 0, last_pcount = 0;
    rt_uint32_t lost, total;
    rt_tick_t tick1, tick2;
    float f;
    char speed[64] = { 0 };
    struct timeval timeout;

    buffer = malloc(IPERF_BUFSZ);
    if (buffer == NULL)
    {
        return;
    }
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if(sock < 0)
    {
        rt_kprintf("can't create socket!! exit\n");
        return;
    }
    server.sin_family = PF_INET;
    server.sin_port = htons(param.port);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");

    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        rt_kprintf("setsockopt failed!!");
        closesocket(sock);
        free(buffer);
        return;
    }
    if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) < 0)
    {
        rt_kprintf("iperf server bind failed!! exit\n");
        closesocket(sock);
        free(buffer);
        return;
    }
    while (param.mode != IPERF_MODE_STOP)
    {
        tick1 = rt_tick_get();
        tick2 = tick1;
        lost = 0;
        total = 0;
        sentlen = 0;
        while ((tick2 - tick1) < (RT_TICK_PER_SECOND * 5))
        {
            r_size = recvfrom(sock, buffer, IPERF_BUFSZ, 0, (struct sockaddr *)&sender, (socklen_t*)&sender_len);
            if (r_size > 12)
            {
                pcount = ntohl(buffer[0]);
                if (last_pcount < pcount)
                {
                    lost += pcount - last_pcount - 1;
                    total += pcount - last_pcount;
                }
                else
                {
                    last_pcount = pcount;
                }
                last_pcount = pcount;
                sentlen += r_size;
            }
            tick2 = rt_tick_get();
        }
        if (sentlen > 0)
        {
            f = (float)(sentlen * RT_TICK_PER_SECOND / 125 / (tick2 - tick1));
            f /= 1000.0f;
            snprintf(speed, sizeof(speed), "%.4f Mbps! lost:%d total:%d\n", f, lost, total);
            rt_kprintf("%s", speed);
        }
    }
    free(buffer);
    closesocket(sock);
}

static void iperf_client(void *thread_param)
{
    int i;
    int sock;
    int ret;
    int tips = 1;
    uint8_t *send_buf;
    rt_uint64_t sentlen;
    rt_tick_t tick1, tick2;
    struct sockaddr_in addr;

    char speed[32] = { 0 };

    send_buf = (uint8_t *) malloc(IPERF_BUFSZ);
    if (!send_buf) return ;

    for (i = 0; i < IPERF_BUFSZ; i ++)
        send_buf[i] = i & 0xff;

    while (param.mode != IPERF_MODE_STOP)
    {
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sock < 0)
        {
            rt_kprintf("create socket failed!\n");
            rt_thread_delay(RT_TICK_PER_SECOND);
            continue;
        }

        addr.sin_family = PF_INET;
        addr.sin_port = htons(param.port);
        addr.sin_addr.s_addr = inet_addr((char *)param.host);

        ret = connect(sock, (const struct sockaddr *)&addr, sizeof(addr));
        if (ret == -1)
        {
            if (tips)
            {
                rt_kprintf("Connect to iperf server faile, Waiting for the server to open!\n");
                tips = 0;
            }
            closesocket(sock);
            rt_thread_delay(RT_TICK_PER_SECOND);
            continue;
        }

        rt_kprintf("Connect to iperf server successful!\n");

        {
            int flag = 1;

            setsockopt(sock,
                       IPPROTO_TCP,     /* set option at TCP level */
                       TCP_NODELAY,     /* name of option */
                       (void *) &flag,  /* the cast is historical cruft */
                       sizeof(int));    /* length of option value */
        }

        sentlen = 0;

        tick1 = rt_tick_get();
        while (param.mode != IPERF_MODE_STOP)
        {
            tick2 = rt_tick_get();
            if (tick2 - tick1 >= RT_TICK_PER_SECOND * 5)
            {
                float f;

                f = (float)(sentlen * RT_TICK_PER_SECOND / 125 / (tick2 - tick1));
                f /= 1000.0f;
                snprintf(speed, sizeof(speed), "%.4f Mbps!\n", f);
                rt_kprintf("%s", speed);
                tick1 = tick2;
                sentlen = 0;
            }

            ret = send(sock, send_buf, IPERF_BUFSZ, 0);
            if (ret > 0)
            {
                sentlen += ret;
            }

            if (ret < 0) break;
        }

        closesocket(sock);

        rt_thread_delay(RT_TICK_PER_SECOND * 2);
        rt_kprintf("Disconnected, iperf server shut down!\n");
        tips = 1;
    }
    free(send_buf);
}

void iperf_server(void *thread_param)
{
    uint8_t *recv_data;
    socklen_t sin_size;
    rt_tick_t tick1, tick2;
    int sock = -1, connected, bytes_received;
    rt_uint64_t recvlen;
    struct sockaddr_in server_addr, client_addr;
    char speed[32] = { 0 };
    fd_set readset;
    struct timeval timeout;

    recv_data = (uint8_t *)malloc(IPERF_BUFSZ);
    if (recv_data == RT_NULL)
    {
        rt_kprintf("No memory\n");
        goto __exit;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        rt_kprintf("Socket error\n");
        goto __exit;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(param.port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0x0, sizeof(server_addr.sin_zero));

    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
    {
        rt_kprintf("Unable to bind\n");
        goto __exit;
    }

    if (listen(sock, 5) == -1)
    {
        rt_kprintf("Listen error\n");
        goto __exit;
    }

    timeout.tv_sec = 3;
    timeout.tv_usec = 0;

    while (param.mode != IPERF_MODE_STOP)
    {
        FD_ZERO(&readset);
        FD_SET(sock, &readset);

        if (select(sock + 1, &readset, RT_NULL, RT_NULL, &timeout) == 0)
            continue;

        sin_size = sizeof(struct sockaddr_in);

        connected = accept(sock, (struct sockaddr *)&client_addr, &sin_size);

        rt_kprintf("new client connected from (%s, %d)\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        {
            int flag = 1;

            setsockopt(connected,
                       IPPROTO_TCP,     /* set option at TCP level */
                       TCP_NODELAY,     /* name of option */
                       (void *) &flag,  /* the cast is historical cruft */
                       sizeof(int));    /* length of option value */
        }

        recvlen = 0;
        tick1 = rt_tick_get();
        while (param.mode != IPERF_MODE_STOP)
        {
            bytes_received = recv(connected, recv_data, IPERF_BUFSZ, 0);
            if (bytes_received <= 0) break;

            recvlen += bytes_received;

            tick2 = rt_tick_get();
            if (tick2 - tick1 >= RT_TICK_PER_SECOND * 5)
            {
                float f;

                f = (float)(recvlen * RT_TICK_PER_SECOND / 125 / (tick2 - tick1));
                f /= 1000.0f;
                snprintf(speed, sizeof(speed), "%.4f Mbps!\n", f);
                rt_kprintf("%s", speed);
                tick1 = tick2;
                recvlen = 0;
            }
        }
        rt_kprintf("client disconnected (%s, %d)\n",
                   inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        if (connected >= 0) closesocket(connected);
        connected = -1;
    }

__exit:
    if (sock >= 0) closesocket(sock);
    if (recv_data) free(recv_data);
}

void iperf_usage(void)
{
    rt_kprintf("Usage: iperf [-s|-c host] [options]\n");
    rt_kprintf("       iperf [-h|--stop]\n");
    rt_kprintf("\n");
    rt_kprintf("Client/Server:\n");
    rt_kprintf("  -p #         server port to listen on/connect to\n");
    rt_kprintf("\n");
    rt_kprintf("Server specific:\n");
    rt_kprintf("  -s           run in server mode\n");
    rt_kprintf("\n");
    rt_kprintf("Client specific:\n");
    rt_kprintf("  -c <host>    run in client mode, connecting to <host>\n");
    rt_kprintf("\n");
    rt_kprintf("Miscellaneous:\n");
    rt_kprintf("  -h           print this message and quit\n");
    rt_kprintf("  --stop       stop iperf program\n");
    rt_kprintf("  -u           testing UDP protocol");
    return;
}

int iperf(int argc, char **argv)
{
    int mode = 0; /* server mode */
    char *host = NULL;
    int port = IPERF_PORT;
    int use_udp = 0;
    int index = 1;

    if (argc == 1)
    {
        goto __usage;
    }
    if (strcmp(argv[1], "-u") == 0)
    {
        index = 2;
        use_udp = 1;
    }
    if (strcmp(argv[index], "-h") == 0) goto __usage;
    else if (strcmp(argv[index], "--stop") == 0)
    {
        /* stop iperf */
        param.mode = IPERF_MODE_STOP;
        return 0;
    }
    else if (strcmp(argv[index], "-s") == 0)
    {
        mode = IPERF_MODE_SERVER; /* server mode */

        /* iperf -s -p 5000 */
        if ((argc == 4) || (argc == 5))
        {
            if (strcmp(argv[index + 1], "-p") == 0)
            {
                port = atoi(argv[index + 2]);
            }
            else goto __usage;
        }
    }
    else if (strcmp(argv[index], "-c") == 0)
    {
        mode = IPERF_MODE_CLIENT; /* client mode */
        if (argc < 3) goto __usage;

        host = argv[index + 1];
        if ((argc == 5) || (argc == 6))
        {
            /* iperf -c host -p port */
            if (strcmp(argv[index + 2], "-p") == 0)
            {
                port = atoi(argv[index + 3]);
            }
            else goto __usage;
        }
    }
    else if (strcmp(argv[index], "-h") == 0)
    {
        goto __usage;
    }
    else goto __usage;

    /* start iperf */
    if (param.mode == IPERF_MODE_STOP)
    {
        rt_thread_t tid = RT_NULL;

        param.mode = mode;
        param.port = port;
        if (param.host)
        {
            rt_free(param.host);
            param.host = NULL;
        }
        if (host) param.host = rt_strdup(host);

        if (use_udp)
        {
            if (mode == IPERF_MODE_CLIENT)
            {
                tid = rt_thread_create("iperfc", iperf_udp_client, RT_NULL,
                                   2048, 20, 20);
            }
            else if (mode == IPERF_MODE_SERVER)
            {
                tid = rt_thread_create("iperfd", iperf_udp_server, RT_NULL,
                                   2048, 10, 20);
            }
        }
        else
        {
            if (mode == IPERF_MODE_CLIENT)
            {
                tid = rt_thread_create("iperfc", iperf_client, RT_NULL,
                                   2048, 20, 20);
            }
            else if (mode == IPERF_MODE_SERVER)
            {
                tid = rt_thread_create("iperfd", iperf_server, RT_NULL,
                                   2048, 20, 20);
            }
        }
        if (tid) rt_thread_startup(tid);
    }
    else
    {
        rt_kprintf("Please stop iperf firstly, by:\n");
        rt_kprintf("iperf --stop\n");
    }

    return 0;

__usage:
    iperf_usage();
    return 0;
}

#ifdef RT_USING_FINSH
    #include <finsh.h>
    MSH_CMD_EXPORT(iperf, the network bandwidth measurement tool);
#endif

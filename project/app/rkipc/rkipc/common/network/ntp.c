#include "common.h"
#include <arpa/inet.h>
#include <endian.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "ntp.c"

#define VERSION_3 3
#define VERSION_4 4

#define MODE_CLIENT 3
#define MODE_SERVER 4

#define NTP_LI 0
#define NTP_VN VERSION_3
#define NTP_MODE MODE_CLIENT
#define NTP_STRATUM 0
#define NTP_POLL 4
#define NTP_PRECISION -6

#define NTP_HLEN 48

#define NTP_PORT 123

#define TIMEOUT 10

#define BUFSIZE 1500

#define JAN_1970 0x83aa7e80

#define NTP_CONV_FRAC32(x) (uint64_t)((x) * ((uint64_t)1 << 32))
#define NTP_REVE_FRAC32(x) ((double)((double)(x) / ((uint64_t)1 << 32)))

#define NTP_CONV_FRAC16(x) (uint32_t)((x) * ((uint32_t)1 << 16))
#define NTP_REVE_FRAC16(x) ((double)((double)(x) / ((uint32_t)1 << 16)))

#define USEC2FRAC(x) ((uint32_t)NTP_CONV_FRAC32((x) / 1000000.0))
#define FRAC2USEC(x) ((uint32_t)NTP_REVE_FRAC32((x)*1000000.0))

#define NTP_LFIXED2DOUBLE(x)                                                                       \
	((double)(ntohl(((struct l_fixedpt *)(x))->intpart) - JAN_1970 +                               \
	          FRAC2USEC(ntohl(((struct l_fixedpt *)(x))->fracpart)) / 1000000.0))

struct s_fixedpt {
	uint16_t intpart;
	uint16_t fracpart;
};

struct l_fixedpt {
	uint32_t intpart;
	uint32_t fracpart;
};

struct ntphdr {
#if __BYTE_ORDER == __BID_ENDIAN
	unsigned int ntp_li : 2;
	unsigned int ntp_vn : 3;
	unsigned int ntp_mode : 3;
#endif
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int ntp_mode : 3;
	unsigned int ntp_vn : 3;
	unsigned int ntp_li : 2;
#endif
	uint8_t ntp_stratum;
	uint8_t ntp_poll;
	int8_t ntp_precision;
	struct s_fixedpt ntp_rtdelay;
	struct s_fixedpt ntp_rtdispersion;
	uint32_t ntp_refid;
	struct l_fixedpt ntp_refts;
	struct l_fixedpt ntp_orits;
	struct l_fixedpt ntp_recvts;
	struct l_fixedpt ntp_transts;
};

in_addr_t inet_host(const char *host) {
	in_addr_t saddr;
	struct hostent *hostent;

	if ((saddr = inet_addr(host)) == INADDR_NONE) {
		if ((hostent = gethostbyname(host)) == NULL)
			return INADDR_NONE;

		memmove(&saddr, hostent->h_addr, hostent->h_length);
	}

	return saddr;
}

int get_ntp_packet(void *buf, size_t *size) //构建并发送NTP请求报文
{
	struct ntphdr *ntp;
	struct timeval tv;

	if (!size || *size < NTP_HLEN)
		return -1;

	memset(buf, 0, *size);

	ntp = (struct ntphdr *)buf;
	ntp->ntp_li = NTP_LI;
	ntp->ntp_vn = NTP_VN;
	ntp->ntp_mode = NTP_MODE;
	ntp->ntp_stratum = NTP_STRATUM;
	ntp->ntp_poll = NTP_POLL;
	ntp->ntp_precision = NTP_PRECISION;

	gettimeofday(&tv, NULL); //把目前的时间用tv 结构体返回
	ntp->ntp_transts.intpart = htonl(tv.tv_sec + JAN_1970);
	ntp->ntp_transts.fracpart = htonl(USEC2FRAC(tv.tv_usec));

	*size = NTP_HLEN;

	return 0;
}

double get_rrt(const struct ntphdr *ntp, const struct timeval *recvtv) //往返时延
{
	double t1, t2, t3, t4;

	t1 = NTP_LFIXED2DOUBLE(&ntp->ntp_orits);
	t2 = NTP_LFIXED2DOUBLE(&ntp->ntp_recvts);
	t3 = NTP_LFIXED2DOUBLE(&ntp->ntp_transts);
	t4 = recvtv->tv_sec + recvtv->tv_usec / 1000000.0;

	return (t4 - t1) - (t3 - t2);
}

double get_offset(const struct ntphdr *ntp, const struct timeval *recvtv) //偏移量
{
	double t1, t2, t3, t4;

	t1 = NTP_LFIXED2DOUBLE(&ntp->ntp_orits);
	t2 = NTP_LFIXED2DOUBLE(&ntp->ntp_recvts);
	t3 = NTP_LFIXED2DOUBLE(&ntp->ntp_transts);
	t4 = recvtv->tv_sec + recvtv->tv_usec / 1000000.0;

	return ((t2 - t1) + (t3 - t4)) / 2;
}

int rkipc_ntp_update(const char *ntp_server_addr) {
	char dateBuf[64] = {0};
	char cmd[128] = {0};
	struct tm *local;
	char buf[BUFSIZE];
	size_t nbytes;
	int sockfd, maxfd1;
	struct sockaddr_in servaddr;
	fd_set readfds;
	struct timeval timeout, recvtv, tv;
	double offset;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(NTP_PORT);
	servaddr.sin_addr.s_addr = inet_host(ntp_server_addr);

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		LOG_ERROR("socket error\n");
		return -1;
	}

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) != 0) {
		LOG_ERROR("connect error\n");
		close(sockfd);
		return -1;
	}
	nbytes = BUFSIZE;
	if (get_ntp_packet(buf, &nbytes) != 0) {
		LOG_ERROR("construct ntp request error\n");
		close(sockfd);
		return -1;
	}
	send(sockfd, buf, nbytes, 0);

	FD_ZERO(&readfds);
	FD_SET(sockfd, &readfds);
	maxfd1 = sockfd + 1;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;

	if (select(maxfd1, &readfds, NULL, NULL, &timeout) > 0) {
		if (FD_ISSET(sockfd, &readfds)) {
			if ((nbytes = recv(sockfd, buf, BUFSIZE, 0)) < 0) {
				LOG_ERROR("recv error\n");
				close(sockfd);
				return -1;
			}

			//计算C/S时间偏移量
			gettimeofday(&recvtv, NULL);
			offset = get_offset((struct ntphdr *)buf, &recvtv);
			//更新系统时间

			gettimeofday(&tv, NULL);

			tv.tv_sec += (int)offset + 28800;
			tv.tv_usec += offset - (int)offset;

			local = localtime((time_t *)&tv.tv_sec);
			strftime(dateBuf, 64, "%Y-%m-%d %H:%M:%S", local);
			sprintf(cmd, "busybox date -s \"%s\" > /dev/null", dateBuf);
			system(cmd);

			// LOG_DEBUG("%s \n", ctime((time_t *) &tv.tv_sec));
		}
	}

	close(sockfd);

	return 0;
}
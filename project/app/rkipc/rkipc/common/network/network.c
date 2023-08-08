// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "network.h"
#include "Rk_wifi.h"
#include "common.h"
#include "ntp.h"
#include <linux/if.h> // must be included later than <net/if.h>

#ifdef LOG_TAG
#undef LOG_TAG
#endif
#define LOG_TAG "network.c"

#define BUFSIZE 8192
#define BUFLEN 20480

static char netmode[32];
static rk_network_cb rk_cb;
static int netlink_fd = 0;
static int g_network_run_ = 0;
static pthread_t ntp_client_thread_id;
static void *g_ntp_signal = NULL;

static int readNlSock(int sockFd, char *bufPtr, int seqNum, int pId) {
	struct nlmsghdr *nlHdr;
	int readLen = 0, msgLen = 0;
	do {
		/* Recieve response from the kernel */
		if ((readLen = recv(sockFd, bufPtr, BUFSIZE - msgLen, 0)) < 0) {
			perror("SOCK READ: ");
			return -1;
		}

		nlHdr = (struct nlmsghdr *)bufPtr;
		/* Check if the header is valid */
		if ((NLMSG_OK(nlHdr, readLen) == 0) || (nlHdr->nlmsg_type == NLMSG_ERROR)) {
			perror("Error in recieved packet");
			return -1;
		}
		/* Check if the its the last message */
		if (nlHdr->nlmsg_type == NLMSG_DONE) {
			break;
		} else {
			/* Else move the pointer to buffer appropriately */
			bufPtr += readLen;
			msgLen += readLen;
		}
		/* Check if its a multi part message */
		if ((nlHdr->nlmsg_flags & NLM_F_MULTI) == 0) {
			/* return if its not */
			break;
		}
	} while ((nlHdr->nlmsg_seq != seqNum) || (nlHdr->nlmsg_pid != pId));
	return msgLen;
}

/* For parsing the route info returned */
static void parseRoutes(struct nlmsghdr *nlHdr, struct route_info *rtInfo, char *gateway) {
	struct rtmsg *rtMsg;
	struct rtattr *rtAttr;
	int rtLen;
	char *tempBuf = NULL;

	tempBuf = (char *)malloc(100);
	rtMsg = (struct rtmsg *)NLMSG_DATA(nlHdr);

	/* If the route is not for AF_INET or does not belong to main routing table
	then return. */
	if ((rtMsg->rtm_family != AF_INET) || (rtMsg->rtm_table != RT_TABLE_MAIN)) {
		free(tempBuf);
		return;
	}
	/* get the rtattr field */
	rtAttr = (struct rtattr *)RTM_RTA(rtMsg);
	rtLen = RTM_PAYLOAD(nlHdr);
	for (; RTA_OK(rtAttr, rtLen); rtAttr = RTA_NEXT(rtAttr, rtLen)) {
		switch (rtAttr->rta_type) {
		case RTA_OIF:
			if_indextoname(*(int *)RTA_DATA(rtAttr), rtInfo->ifName);
			break;

		case RTA_GATEWAY:
			memcpy(&rtInfo->gateWay, RTA_DATA(rtAttr), sizeof(rtInfo->gateWay));
			break;

		case RTA_PREFSRC:
			memcpy(&rtInfo->srcAddr, RTA_DATA(rtAttr), sizeof(rtInfo->srcAddr));
			break;

		case RTA_DST:
			memcpy(&rtInfo->dstAddr, RTA_DATA(rtAttr), sizeof(rtInfo->dstAddr));
			break;
		}
	}
	if (strstr((char *)inet_ntoa(rtInfo->dstAddr), "0.0.0.0"))
		sprintf(gateway, (char *)inet_ntoa(rtInfo->gateWay));
	free(tempBuf);
	return;
}

static int get_gateway(char *gateway) {
	struct nlmsghdr *nlMsg;
	// struct rtmsg *rtMsg;
	struct route_info *rtInfo;
	char msgBuf[BUFSIZE];

	int sock, len, msgSeq = 0;
	/* Create Socket */
	if ((sock = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_ROUTE)) < 0)
		perror("Socket Creation: ");
	/* Initialize the buffer */
	memset(msgBuf, 0, BUFSIZE);
	/* point the header and the msg structure pointers into the buffer */
	nlMsg = (struct nlmsghdr *)msgBuf;
	// rtMsg = (struct rtmsg *)NLMSG_DATA(nlMsg);
	/* Fill in the nlmsg header*/
	nlMsg->nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg)); // Length of message.
	nlMsg->nlmsg_type = RTM_GETROUTE;                // Get the routes from kernel routing table .
	nlMsg->nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST; // The message is a request for dump.
	nlMsg->nlmsg_seq = msgSeq++;                     // Sequence of the message packet.
	nlMsg->nlmsg_pid = getpid();                     // PID of process sending the request.
	/* Send the request */
	if (send(sock, nlMsg, nlMsg->nlmsg_len, 0) < 0) {
		LOG_INFO("Write To Socket Failed...\n");
		close(sock);
		return -1;
	}
	/* Read the response */
	if ((len = readNlSock(sock, msgBuf, msgSeq, getpid())) < 0) {
		LOG_INFO("Read From Socket Failed...\n");
		return -1;
	}
	/* Parse and print the response */
	rtInfo = (struct route_info *)malloc(sizeof(struct route_info));

	for (; NLMSG_OK(nlMsg, len); nlMsg = NLMSG_NEXT(nlMsg, len)) {
		memset(rtInfo, 0, sizeof(struct route_info));
		parseRoutes(nlMsg, rtInfo, gateway);
	}
	LOG_INFO("gateway:%s\n", gateway);
	free(rtInfo);
	close(sock);
	return 0;
}

static size_t net_stringcopy(char *dst, const char *src, size_t siz) {
	if ((int)siz <= 0)
		return strlen(src);
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0'; /* NUL-terminate dst */
		while (*s++)
			;
	}

	return (s - src - 1); /* count does not include NUL */
}

RKIPC_MAYBE_UNUSED static int whether_ether_is_linked(char *ifname, int ifname_size) {
	struct ethtool_value edata;
	int fd = -1, err = 0;
	struct ifreq ifr;

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_name, "eth0");
	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		perror("Cannot get control socket\n");
		return 70;
	}
	edata.cmd = 0x0000000a;
	ifr.ifr_data = (caddr_t)&edata;
	err = ioctl(fd, 0x8946, &ifr);
	if (err == 0) {
		fprintf(stdout, "Is ethernet linked: %s\n", edata.data ? "yes" : "no");
	} else if (errno != EOPNOTSUPP) {
		perror("Cannot get link status\n");
	}
	return 1;
}

int rk_network_ipv4_set(char *interface, char *method, char *address, char *netmask,
                        char *gateway) {
	char netconfig_cmd[512] = {0};

	if (strcasecmp(method, "static") == 0) {
		snprintf(netconfig_cmd, sizeof(netconfig_cmd), "ifconfig %s %s netmask %s", interface,
		         address, netmask);
		memcpy(netmode, method, strlen(method));
		LOG_INFO("netconfig_cmd:%s\n", netconfig_cmd);
		system(netconfig_cmd);
		//处理路由
		// if( Ether_info.link_type != 1 ){
		snprintf(netconfig_cmd, sizeof(netconfig_cmd), "route add -net default gw %s", gateway);
		LOG_INFO("netconfig_cmd:%s\n", netconfig_cmd);
		system(netconfig_cmd);
		//}
	}
	// IPv4 DHCP
	if (strcasecmp(method, "dhcp") == 0) {
		// Ether_info.v4_is_dhcp = true;
		memcpy(netmode, method, strlen(method));
		snprintf(netmode, sizeof(netmode), "%s", method);
		system("udhcpc &");
	}

	return 0;
}

int rk_network_ipv4_get(char *interface, char *method, char *address, char *netmask,
                        char *gateway) {
	int fd;
	int interfaceNum = 0;
	struct ifreq buf[16];
	struct ifconf ifc;
	struct ifreq ifrcopy;
	char ip[32] = {0};
	char subnetMask[32] = {0};

	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		close(fd);
		return -1;
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;
	if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc)) { //获取所有网口信息
		interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
		LOG_INFO("interface num = %d\n", interfaceNum - 1);
		while (interfaceNum-- > 0) { //逐个提取
			if (interfaceNum == 0) { // buf[interfaceNum].ifr_name == " lo"
				break;
			}
			if (strcmp(interface, buf[interfaceNum].ifr_name))
				continue;
			LOG_INFO("ndevice name: %s\n", buf[interfaceNum].ifr_name);
			// ignore the interface that not up or not runing
			ifrcopy = buf[interfaceNum];
			if (ioctl(fd, SIOCGIFFLAGS, &ifrcopy)) {
				LOG_INFO("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
				close(fd);
				return -1;
			}
			// get the IP of this interface
			if (!ioctl(fd, SIOCGIFADDR, (char *)&buf[interfaceNum])) {
				snprintf(ip, sizeof(ip), "%s",
				         (char *)inet_ntoa(
				             ((struct sockaddr_in *)&(buf[interfaceNum].ifr_addr))->sin_addr));
				LOG_INFO("device ip: %s\n", ip);
				memcpy(address, ip, strlen(ip));
				memset(address + strlen(ip), '\0', 1); // set terminator
			} else {
				LOG_INFO("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
				close(fd);
				return -1;
			}
			// get the subnet mask of this interface
			if (!ioctl(fd, SIOCGIFNETMASK, &buf[interfaceNum])) {
				snprintf(subnetMask, sizeof(subnetMask), "%s",
				         (char *)inet_ntoa(
				             ((struct sockaddr_in *)&(buf[interfaceNum].ifr_netmask))->sin_addr));
				LOG_INFO("device subnetMask: %s\n", subnetMask);
				memcpy(netmask, subnetMask, strlen(subnetMask));
				memset(netmask + strlen(subnetMask), '\0', 1); // set terminator
			} else {
				LOG_INFO("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
				close(fd);
				return -1;
			}
			get_gateway(gateway);
			if (strcmp(netmode, "") == 0) {
				memcpy(netmode, "dhcp", strlen("dhcp")); //默认DHCP连接
			}
			memcpy(method, netmode, strlen(netmode));
			memset(method + strlen(netmode), '\0', 1); // set terminator
			LOG_INFO("Method:%s\n", method);
		}
	} else {
		LOG_INFO("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
		close(fd);
		return -1;
	}
	close(fd);
	return 0;
}

int rk_network_dns_get(char *ethernet_v4_dns, char *ethernet_v4_dns2) {
	char buf[128];
	int count = 0;
	FILE *pFile;

	pFile = fopen("/etc/resolv.conf", "r");

	if (NULL == pFile) {
		LOG_INFO("[/etc/resolv.conf] fopen fail, erroe 02 \n");
	} else {
		memset(buf, 0, sizeof(buf));
		while ((fgets(buf, sizeof(buf), pFile))) {
			if (strncmp(buf, "nameserver", 10) == 0) {
				int start = 0, end = 0;
				int i = 0;
				for (i = 1; i < 127; i++) {
					if ((buf[i - 1] == 'e') && (buf[i] == 'r') && (buf[i + 1] == ' ')) {
						start = i + 2;
					}
					if (buf[i] == '\n') {
						end = i - 1;
						break;
					}
				}
				if ((end - start) > 0) {
					count++;
					if (count == 1) {
						sscanf(buf, "%*s%s", ethernet_v4_dns);
						// memcpy(ethernet_v4_dns, &(buf[start]), end - start + 1);
					} else if (count == 2) {
						sscanf(buf, "%*s%s", ethernet_v4_dns2);
						// memcpy(ethernet_v4_dns2, &(buf[start]), end - start + 1);
					}
				} else {
					LOG_INFO("[/etc/resolv.conf] erroe 01 \n");
				}
				if (count == 2) { // 最多只找兩個，因此離開
					break;
				}
			}
		}
		LOG_INFO("DNS1:%s, DNS2:%s\n", ethernet_v4_dns, ethernet_v4_dns2);
		// LOG_INFO("%d\n",count);
		fclose(pFile);
		usleep(1000);
		return count;
	}

	return 0;
}

int rk_network_dns_set(char *dns, char *dns2) {
	int pos = 0;
	int req_len = 64;
	char *buf = NULL;
	buf = (char *)malloc(req_len);
	// char fake_dns[24];
	// char fake_dns1[24];

	// cnt = rk_network_dns_get(fake_dns,fake_dns1);
	char *FILE_NAME = "/etc/resolv.conf";
	FILE *file_fd;
	file_fd = fopen(FILE_NAME, "w+");

	pos += net_stringcopy(&buf[pos], "nameserver ", pos < req_len ? req_len - pos : 0);
	pos += net_stringcopy(&buf[pos], dns, pos < req_len ? req_len - pos : 0);
	pos += net_stringcopy(&buf[pos], "\n", pos < req_len ? req_len - pos : 0);
	pos += net_stringcopy(&buf[pos], "nameserver ", pos < req_len ? req_len - pos : 0);
	pos += net_stringcopy(&buf[pos], dns2, pos < req_len ? req_len - pos : 0);
	pos += net_stringcopy(&buf[pos], "\n", pos < req_len ? req_len - pos : 0);
	LOG_INFO("buf:\n%s\n", buf);

	if (file_fd == NULL) {
		perror("errno");
	} /*
	 if ( cnt <= 1 ){
	     fseek(file_fd,55,SEEK_CUR);
	     fwrite(buf,strlen(buf),1,file_fd);
	     fclose(file_fd);
	     free(buf);
	 }
	 else {*/
	fwrite(buf, strlen(buf), 1, file_fd);
	fclose(file_fd);
	free(buf);
	//}
	usleep(1000);

	return 0;
}

int rk_network_get_mac(const char *ifname, char *mac) {
	struct ifreq ifreq;
	int sock;
	/*
	LOG_INFO("%d",sizeof(mac));
	if (sizeof(mac) < 18){
	    LOG_INFO("Error:sizes should be above 18!\n");
	    return -1;
	}*/

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return -1;
	}
	strcpy(ifreq.ifr_name, ifname); // Currently, only get eth0

	if (ioctl(sock, SIOCGIFHWADDR, &ifreq) < 0) {
		perror("ioctl");
		return -1;
	}
	snprintf(mac, 64, "%X:%X:%X:%X:%X:%X", (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[1], (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[3], (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
	         (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);
	LOG_INFO("mac:%s\n", mac);
	return 0;
}

void rk_network_set_mac(const char *ifname, const char *mac) {
	char ifcmd[128];

	memset(ifcmd, 0, sizeof(ifcmd));
	snprintf(ifcmd, sizeof(ifcmd), "ifconfig %s down", ifname);
	system(ifcmd);

	memset(ifcmd, 0, sizeof(ifcmd));
	snprintf(ifcmd, sizeof(ifcmd), "ifconfig %s hw ether %s", ifname, mac);
	system(ifcmd);

	memset(ifcmd, 0, sizeof(ifcmd));
	snprintf(ifcmd, sizeof(ifcmd), "ifconfig %s up", ifname);
	system(ifcmd);
}

int rk_network_nicspeed_get(const char *ifname, int *speed, int *duplex, int *autoneg) {
	struct ifreq ifr;
	int fd = 0;
	int err = -1;
	struct ethtool_cmd ecmd;

	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifname);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	LOG_INFO("socket fd:%d\n", fd);
	if (fd < 0) {
		LOG_INFO("ethtool_gset cant grt control socket\n");
		return -1;
	}
	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_ifru.ifru_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, &ifr);
	if (err < 0) {
		LOG_INFO("cant get device settings\n");
	}
	LOG_INFO("PHY speed - %dMbps/%s\n", ecmd.speed, (ecmd.duplex == DUPLEX_FULL) ? "Full" : "Half");
	LOG_INFO("Auto-negotiation:%s\n", (ecmd.autoneg == AUTONEG_DISABLE) ? "off" : "on");

	switch (ecmd.speed) {
	case SPEED_10:
		*speed = ecmd.speed;
		break;
	case SPEED_100:
		*speed = ecmd.speed;
		break;
	case SPEED_1000:
		*speed = ecmd.speed;
		break;
	default:
		fprintf(stdout, "Unknown! (%i)\n", ecmd.speed);
		break;
	};

	switch (ecmd.duplex) {
	case DUPLEX_HALF:
		*duplex = ecmd.duplex;
		break;
	case DUPLEX_FULL:
		*duplex = ecmd.duplex;
		break;

	default:
		fprintf(stdout, "Unknown! (%i)\n", ecmd.duplex);
		break;
	};
	*autoneg = ecmd.autoneg;
	return 0;
}

int rk_network_nicspeed_set(const char *ifname, int speed, int duplex, int autoneg) {
	int speed_wanted = -1;
	int duplex_wanted = -1;
	int autoneg_wanted = autoneg;
	int advertising_wanted = -1;

	struct ethtool_cmd ecmd;
	struct ifreq ifr;
	int fd = 0;
	int err = -1;

	// pass args
	if (ifname == NULL) {
		LOG_INFO("ifname emtpy...\n");
		return -2;
	}
	speed_wanted = speed;
	duplex_wanted = duplex;
	autoneg_wanted = autoneg;

	strcpy(ifr.ifr_ifrn.ifrn_name, ifname);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0) {
		perror("ethtool_sset Cannot get control socket\n");
		return -1;
	}

	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_ifru.ifru_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, &ifr);
	if (err < 0) {
		perror("Cannot get current device settings\n");
		return -1;
	}
	if (speed_wanted != -1)
		ecmd.speed = speed_wanted;
	if (duplex_wanted != -1)
		ecmd.duplex = duplex_wanted;
	if (autoneg_wanted != -1)
		ecmd.autoneg = autoneg_wanted;
	if ((autoneg_wanted == AUTONEG_ENABLE) && (advertising_wanted < 0)) {
		if (speed_wanted == SPEED_10 && duplex_wanted == DUPLEX_HALF)
			advertising_wanted = ADVERTISED_10baseT_Half;
		else if (speed_wanted == SPEED_10 && duplex_wanted == DUPLEX_FULL)
			advertising_wanted = ADVERTISED_10baseT_Full;
		else if (speed_wanted == SPEED_100 && duplex_wanted == DUPLEX_HALF)
			advertising_wanted = ADVERTISED_100baseT_Half;
		else if (speed_wanted == SPEED_100 && duplex_wanted == DUPLEX_FULL)
			advertising_wanted = ADVERTISED_100baseT_Full;
		else if (speed_wanted == SPEED_1000 && duplex_wanted == DUPLEX_HALF)
			advertising_wanted = ADVERTISED_1000baseT_Half;
		else if (speed_wanted == SPEED_1000 && duplex_wanted == DUPLEX_FULL)
			advertising_wanted = ADVERTISED_1000baseT_Full;
		else
			advertising_wanted = 0;
	}
	if (advertising_wanted != -1) {
		if (advertising_wanted == 0)
			ecmd.advertising =
			    ecmd.supported &
			    (ADVERTISED_10baseT_Half | ADVERTISED_10baseT_Full | ADVERTISED_100baseT_Half |
			     ADVERTISED_100baseT_Full | ADVERTISED_1000baseT_Half | ADVERTISED_1000baseT_Full);
		else
			ecmd.advertising = advertising_wanted;
	}
	// strcpy(ifr.ifr_ifrn.ifrn_name, ifname);
	ecmd.cmd = ETHTOOL_SSET;
	ifr.ifr_ifru.ifru_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, &ifr);
	if (err < 0)
		perror("Cannot set new settings\n");

	if (err < 0) {
		if (speed_wanted != -1)
			fprintf(stderr, "  not setting speed\n");
		if (duplex_wanted != -1)
			fprintf(stderr, "  not setting duplex\n");
		if (autoneg_wanted != -1)
			fprintf(stderr, "  not setting autoneg\n");
	}

	close(fd);
	return 0;
}

int rk_network_nicspeed_support_get(const char *ifname, char *nic_supported) {
	struct ifreq ifr;
	int fd = 0;
	int err = -1;
	struct ethtool_cmd ecmd;
	int req_len = 512;
	char nic_support[req_len];
	int pos = 0;

	// if(devname == NULL) return -2;
	memset(&ifr, 0, sizeof(ifr));
	strcpy(ifr.ifr_ifrn.ifrn_name, ifname);

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	LOG_INFO("socket fd:%d\n", fd);
	if (fd < 0) {
		LOG_INFO("ethtool_gset cant grt control socket\n");
		return -1;
	}
	ecmd.cmd = ETHTOOL_GSET;
	ifr.ifr_ifru.ifru_data = (caddr_t)&ecmd;
	err = ioctl(fd, SIOCETHTOOL, &ifr);
	if (err < 0) {
		LOG_INFO("cant get device settings\n");
		return -1;
	}
	// LOG_INFO("Supported link modes:%d\n",ecmd.supported);

	pos += net_stringcopy(&nic_support[pos], "Auto ", pos < req_len ? req_len - pos : 0);

	LOG_INFO("Supported link modes:\n");
	if (ecmd.supported & ADVERTISED_10baseT_Half) {
		// LOG_INFO("10baseT/Half  ");
		pos +=
		    net_stringcopy(&nic_support[pos], "10baseT/Half ", pos < req_len ? req_len - pos : 0);
	}
	if (ecmd.supported & ADVERTISED_10baseT_Full) {
		// LOG_INFO("10baseT/Full\n");
		pos +=
		    net_stringcopy(&nic_support[pos], "10baseT/Full ", pos < req_len ? req_len - pos : 0);
	}
	if (ecmd.supported & ADVERTISED_100baseT_Half) {
		// LOG_INFO("100baseT/Half  ");
		pos +=
		    net_stringcopy(&nic_support[pos], "100baseT/Half ", pos < req_len ? req_len - pos : 0);
	}
	if (ecmd.supported & ADVERTISED_100baseT_Full) {
		// LOG_INFO("100baseT/Full\n");
		pos +=
		    net_stringcopy(&nic_support[pos], "100baseT/Full ", pos < req_len ? req_len - pos : 0);
	}
	if (ecmd.supported & ADVERTISED_1000baseT_Half) {
		// LOG_INFO("1000baseT/Half  ");
		pos +=
		    net_stringcopy(&nic_support[pos], "1000baseT/Half ", pos < req_len ? req_len - pos : 0);
	}
	if (ecmd.supported & ADVERTISED_1000baseT_Full) {
		// LOG_INFO("1000baseT/Full\n");
		pos +=
		    net_stringcopy(&nic_support[pos], "1000baseT/Full ", pos < req_len ? req_len - pos : 0);
	}

	memcpy(nic_supported, nic_support, strlen(nic_support));
	memset(nic_supported + strlen(nic_support), '\0', 1); // set terminator
	LOG_INFO("nic_supported:%s\n", nic_supported);
	return 0;
}

int rk_ethernet_power_set(const char *ifname, int powerswitch) {
	if (powerswitch == 1) {
		char netconfig_cmd[512] = {0};
		// ifconfig up 网络接口
		if (rk_nic_state_get(ifname) == ETH_DOWN) { //以太网，网卡未激活;*/
			snprintf(netconfig_cmd, sizeof(netconfig_cmd), "ifconfig %s up",
			         ifname); //激活网卡，如:ifconfig eth0 up
			system(netconfig_cmd);
		}
		return 0;
	}

	else if (powerswitch == 0) {
		char netconfig_cmd[512] = {0};
		snprintf(netconfig_cmd, sizeof(netconfig_cmd), "ifconfig %s down", ifname);
		LOG_INFO("%s\n", netconfig_cmd);
		system(netconfig_cmd);

		return 0;
	}

	return 0;
}

int rk_network_get_cable_state() {
	int fd, retval, status;
	char buf[BUFLEN] = {0};
	int len = BUFLEN;
	struct sockaddr_nl addr;
	struct nlmsghdr *nh;
	struct ifinfomsg *ifinfo;
	char name[IFNAMSIZ], cmd1[64], cmd2[64];
	// struct rtattr *attr;

	fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
	netlink_fd = fd;
	setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &len, sizeof(len));
	memset(&addr, 0, sizeof(addr));
	addr.nl_family = AF_NETLINK;
	addr.nl_groups = RTNLGRP_LINK;
	bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	while ((retval = read(fd, buf, BUFLEN)) > 0) {
		LOG_INFO("read Cable state\n");
		for (nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, retval); nh = NLMSG_NEXT(nh, retval)) {
			if (nh->nlmsg_type == NLMSG_DONE)
				break;
			else if (nh->nlmsg_type == NLMSG_ERROR)
				return -1;
			else if (nh->nlmsg_type != RTM_NEWLINK)
				continue;
			ifinfo = NLMSG_DATA(nh);

			if_indextoname(ifinfo->ifi_index, name);
			LOG_INFO("\n[%s] link %s\n", name, (ifinfo->ifi_flags & IFF_LOWER_UP) ? "up" : "down");

			memset(cmd1, 0, 32);
			memset(cmd2, 0, 32);
			sprintf(cmd1, "udhcpc -i %s -T 1 -A 0 -b -q", name);
			sprintf(cmd2, "ifconfig %s 0.0.0.0", name);

			if (ifinfo->ifi_flags & IFF_LOWER_UP) {
				status = 1;
				system("killall -9 udhcpc");
				system("route del default gw 0.0.0.0");
				system("cat /dev/null > /etc/resolv.conf");
				system(cmd1);
			} else {
				status = 0;
				system(cmd2);
			}

			if (rk_cb)
				rk_cb(status);

			/*
			 attr = (struct rtattr*)(((char*)nh) + NLMSG_SPACE(sizeof(*ifinfo)));
			 len = nh->nlmsg_len - NLMSG_SPACE(sizeof(*ifinfo));
			 for (; RTA_OK(attr, len); attr = RTA_NEXT(attr, len)){
			     if (attr->rta_type == IFLA_IFNAME)  {
			         //LOG_INFO(" %s\n", (char*)RTA_DATA(attr));
			         break;
			     }
			 }
			 LOG_INFO("\n");
			 */
		}
	}

	return 0;
}

int rk_nic_state_get(const char *ifname) {
	char buffer[BUFSIZ];
	char cmd[100];
	FILE *read_fp;
	int chars_read;
	int ret = 0;

	memset(buffer, 0, BUFSIZ);
	memset(cmd, 0, 100);
	sprintf(cmd, "ifconfig -a | grep %s", ifname);
	read_fp = popen(cmd, "r");
	if (read_fp != NULL) {
		chars_read = fread(buffer, sizeof(char), BUFSIZ - 1, read_fp);
		pclose(read_fp);

		if (chars_read > 0) {
			ret = 1;
		} else {
			fprintf(stderr, "%s: NO FOUND\r\n", ifname);
			return 0;
		}
	}
	if (ret == 1) {
		memset(buffer, 0, BUFSIZ);
		memset(cmd, 0, 100);
		sprintf(cmd, "ifconfig |grep %s", ifname);
		read_fp = popen(cmd, "r");
		if (read_fp != NULL) {
			chars_read = fread(buffer, sizeof(char), BUFSIZ - 1, read_fp);
			pclose(read_fp);

			if (chars_read > 0) {
				ret = 2;
			} else {
				fprintf(stderr, "%s: DOWN\r\n", ifname);
				return 1;
			}
		}
	}
	if (ret == 2) {
		memset(buffer, 0, BUFSIZ);
		memset(cmd, 0, 100);
		sprintf(cmd, "ifconfig %s | grep RUNNING | awk '{print $3}'", ifname);
		read_fp = popen(cmd, "r");
		if (read_fp != NULL) {
			chars_read = fread(buffer, sizeof(char), BUFSIZ - 1, read_fp);
			pclose(read_fp);

			if (chars_read > 0) {
				fprintf(stderr, "%s: LINKED\r\n", ifname);
				return 3;
			} else {
				fprintf(stderr, "%s: UNPLUGGED\r\n", ifname);
				return 2;
			}
		}
	}
	return -1;
}

static void *rk_net_proc() {
	prctl(PR_SET_NAME, "rk_net_proc", 0, 0, 0);

	LOG_INFO("Creat rk_net_proc thread!\n");

	rk_network_get_cable_state();

	LOG_INFO("exit rk_net_proc thread!\n");

	return NULL;
}

static void *ntp_client_thread() {
	prctl(PR_SET_NAME, "ntp_client_thread", 0, 0, 0);
	int refresh_time_s = rk_param_get_int("network.ntp:refresh_time_s", 60);
	const char *ntp_server = rk_param_get_string("network:ntp.ntp_server", "119.28.183.184");
	LOG_INFO("refresh_time_s is %d, ntp_server is %s\n", refresh_time_s, ntp_server);

	while (g_network_run_) {
		rk_signal_wait(g_ntp_signal, refresh_time_s * 1000); // 60s
		rkipc_ntp_update(ntp_server);
	}

	return NULL;
}

void rk_network_init(rk_network_cb func) { // func_cb func
	g_network_run_ = 1;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

	pthread_t Net_trd;
	if (pthread_create(&Net_trd, &attr, rk_net_proc, NULL) != 0) {
		LOG_INFO("Creat thread failed!\n");
	}
	if (func)
		rk_cb = func;

	if (g_ntp_signal)
		rk_signal_destroy(g_ntp_signal);
	g_ntp_signal = rk_signal_create(0, 1);
	if (!g_ntp_signal) {
		LOG_ERROR("create signal fail\n");
		return;
	}

	if (rk_param_get_int("network.ntp:enable", 0)) {
		pthread_create(&ntp_client_thread_id, NULL, ntp_client_thread, NULL);
	}
}

void rk_network_deinit() { // pthread_t pthid
	g_network_run_ = 0;
	if (rk_param_get_int("network.ntp:enable", 0)) {
		if (g_ntp_signal)
			rk_signal_give(g_ntp_signal);
		pthread_join(ntp_client_thread_id, NULL);
		if (g_ntp_signal) {
			rk_signal_destroy(g_ntp_signal);
			g_ntp_signal = NULL;
		}
	}
	close(netlink_fd);
}

int rk_wifi_power_get(int *on) {
	RK_WIFI_RUNNING_State_e pState;
	RK_wifi_running_getState(&pState);
	if (pState == RK_WIFI_State_OFF)
		*on = 0;
	else
		*on = 1;

	return 0;
}

int rk_wifi_power_set(int on) { return RK_wifi_enable(on); }

int rk_wifi_scan_wifi() { return RK_wifi_scan(); }

int rk_wifi_get_list(char **wifi_list) {
	*wifi_list = RK_wifi_scan_r();
	LOG_INFO("strlen(*wifi_list) is %ld\n", strlen(*wifi_list));

	return 0;
}

/**
 * @brief 连接wifi
 * @param[in] ssid
 * @param[in] psk
 * @return int 0:执行成功
 */
int rk_wifi_connect_with_ssid(const char *ssid, const char *psk) {
	return RK_wifi_connect(ssid, psk);
}

/**
 * @brief 删除wifi
 * @param[in] ssid
 * @return int 0:执行成功
 */
int rk_wifi_forget_with_ssid(const char *ssid) { return RK_wifi_forget_with_ssid(ssid); }

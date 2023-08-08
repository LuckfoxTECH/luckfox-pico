/*
uni_network.h
Copyright(C),2018-2020, 涂鸦科技 www.tuya.comm
*/

#ifndef _UNI_NETWORK_H
#define _UNI_NETWORK_H

#include "tuya_cloud_types.h"

#if ((OPERATING_SYSTEM == SYSTEM_LINUX) || (OPERATING_SYSTEM == SYSTEM_LITEOS))
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#endif

#if ((OPERATING_SYSTEM == SYSTEM_REALTEK8710_1M) || (OPERATING_SYSTEM == SYSTEM_REALTEK8710_2M))
#include "lwip/dns.h"
#include "lwip/err.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* tuya sdk definition of 127.0.0.1 */
#define TY_IPADDR_LOOPBACK ((UINT_T)0x7f000001UL)
/* tuya sdk definition of 0.0.0.0 */
#define TY_IPADDR_ANY ((UINT_T)0x00000000UL)
/* tuya sdk definition of 255.255.255.255 */
#define TY_IPADDR_BROADCAST ((UINT_T)0xffffffffUL)

/* tuya sdk definition of socket protocol */
typedef enum {
	PROTOCOL_TCP = 0,
	PROTOCOL_UDP = 1,
} UNW_PROTOCOL_TYPE;

/* tuya sdk definition of transfer type */
typedef enum {
	TRANS_RECV = 0,
	TRANS_SEND = 1,
} UNW_TRANS_TYPE_E;

/* tuya sdk definition of socket errno */
typedef INT_T UNW_ERRNO_T;
#define UNW_SUCCESS 0
#define UNW_FAIL -1
#define UNW_EINTR -2
#define UNW_EBADF -3
#define UNW_EAGAIN -4
#define UNW_EFAULT -5
#define UNW_EBUSY -6
#define UNW_EINVAL -7
#define UNW_ENFILE -8
#define UNW_EMFILE -9
#define UNW_ENOSPC -10
#define UNW_EPIPE -11
#define UNW_EWOULDBLOCK -12
#define UNW_ENOTSOCK -13
#define UNW_ENOPROTOOPT -14
#define UNW_EADDRINUSE -15
#define UNW_EADDRNOTAVAIL -16
#define UNW_ENETDOWN -17
#define UNW_ENETUNREACH -18
#define UNW_ENETRESET -19
#define UNW_ECONNRESET -20
#define UNW_ENOBUFS -21
#define UNW_EISCONN -22
#define UNW_ENOTCONN -23
#define UNW_ETIMEDOUT -24
#define UNW_ECONNREFUSED -25
#define UNW_EHOSTDOWN -26
#define UNW_EHOSTUNREACH -27
#define UNW_ENOMEM -28
#define UNW_EMSGSIZE -29

/* tuya sdk definition of fd operations */
typedef fd_set UNW_FD_SET;
#define UNW_FD_SET(n, p) FD_SET(n, p)
#define UNW_FD_CLR(n, p) FD_CLR(n, p)
#define UNW_FD_ISSET(n, p) FD_ISSET(n, p)
#define UNW_FD_ZERO(p) FD_ZERO(p)

/* tuya sdk definition of IP info */
typedef struct {
	CHAR_T ip[16];   /* ip addr:  xxx.xxx.xxx.xxx  */
	CHAR_T mask[16]; /* net mask: xxx.xxx.xxx.xxx  */
	CHAR_T gw[16];   /* gateway:  xxx.xxx.xxx.xxx  */
} NW_IP_S;

/* tuya sdk definition of MAC info */
typedef struct {
	BYTE_T mac[6]; /* mac address */
} NW_MAC_S;

/* tuya sdk definition of IP addr */
typedef UINT_T UNW_IP_ADDR_T;

/* tuya sdk definition of IP format change */
#define UNW_INET_ADDR(x) inet_addr(x)

/* tuya sdk definition of addrinfo */
typedef struct addrinfo ADDRINFO;

#define MAX_DOMAIN_NAME_LEN 128
#define MAX_DNS_CNT 16
#if defined(TLS_MODE) && ((TLS_MODE == TLS_TUYA_PSK_ONLY) || (TLS_MODE == TLS_TUYA_ECC_PSK))
#define HTTP_DNS_SERVER_DOMAIN "h1.iot-dns.com"
#else
#define HTTP_DNS_SERVER_DOMAIN "h2.iot-dns.com"
#endif

typedef enum {
	E_REGION_DYNAMIC = 0, //临时无固定区域
	E_REGION_CN,
	E_REGION_EU,
	E_REGION_US,
	E_REGION_UE,
	E_REGION_IN,
	E_REGION_MAX
} REGION_E;

typedef enum {
	E_DNS_PRIO_REGION = 0, //默认的最高优先级：使用region对应的IP地址
	E_DNS_PRIO_RANDOM,
	E_DNS_PRIO_SYSTEM,
	E_DNS_PRIO_MAX
} TY_DNS_PRIO_E;

/***********************************************************
 *  Function: unw_get_errno
 *  Desc:     tuya sdk definition of errno
 *  Return:   tuya sdk definition of socket errno
 ***********************************************************/
UNW_ERRNO_T unw_get_errno(VOID);

/***********************************************************
 *  Function: unw_select
 *  Desc:     tuya sdk definition of socket select
 *  Input && Output && Return: refer to std select
 ***********************************************************/
INT_T unw_select(IN CONST INT_T maxfd, INOUT UNW_FD_SET *readfds, INOUT UNW_FD_SET *writefds,
                 OUT UNW_FD_SET *errorfds, IN CONST UINT_T ms_timeout);

/***********************************************************
 *  Function: unw_get_nonblock
 *  Desc:     check where a socket fd is blocked
 *  Input:    fd: socket fd
 *  Return:   <0: fail  >0: non-block  0: block
 ***********************************************************/
INT_T unw_get_nonblock(IN CONST INT_T fd);

/***********************************************************
 *  Function: unw_set_block
 *  Desc:     set the socket fd to block/non-block state
 *  Input:    fd: socket fd  block: the new state
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_block(IN CONST INT_T fd, IN CONST BOOL_T block);

/***********************************************************
 *  Function: unw_close
 *  Desc:     tuya sdk definition of socket close
 *  Input && Output && Return: refer to std close
 ***********************************************************/
INT_T unw_close(IN CONST INT_T fd);

/***********************************************************
 *  Function: unw_shutdown
 *  Desc:     tuya sdk definition of socket shutdown
 *  Input && Output && Return: refer to std shutdown
 ***********************************************************/
INT_T unw_shutdown(IN CONST INT_T fd, IN CONST INT_T how);

/***********************************************************
 *  Function: unw_socket_create
 *  Desc:     create a tcp/udp socket
 *  Input:    type: tcp/udp type
 *  Return: refer to std socket
 ***********************************************************/
INT_T unw_socket_create(IN CONST UNW_PROTOCOL_TYPE type);

/***********************************************************
 *  Function: unw_connect
 *  Desc:     connect the socket fd to a addr and a port
 *  Return:   refer to std connect
 ***********************************************************/
INT_T unw_connect(IN CONST INT_T fd, IN CONST UNW_IP_ADDR_T addr, IN CONST USHORT_T port);

/***********************************************************
 *  Function: unw_connect_raw
 *  Desc:     tuya sdk definition of socket connect
 *  Input && Output && Return: refer to std connect
 ***********************************************************/
INT_T unw_connect_raw(IN CONST INT_T fd, void *p_socket, IN CONST INT_T len);

/***********************************************************
 *  Function: unw_bind
 *  Desc:     bind the socket fd to a addr and a port
 *  Return:   refer to std bind
 ***********************************************************/
INT_T unw_bind(IN CONST INT_T fd, IN CONST UNW_IP_ADDR_T addr, IN CONST USHORT_T port);

/***********************************************************
 *  Function: unw_listen
 *  Desc:     tuya sdk definition of socket listen
 *  Input && Output && Return: refer to std listen
 ***********************************************************/
INT_T unw_listen(IN CONST INT_T fd, IN CONST INT_T backlog);

/***********************************************************
 *  Function: unw_send
 *  Desc:     tuya sdk definition of socket send
 *  Input && Output && Return: refer to std send
 ***********************************************************/
INT_T unw_send(IN CONST INT_T fd, IN CONST VOID *buf, IN CONST UINT_T nbytes);

/***********************************************************
 *  Function: unw_send_to
 *  Desc:     tuya sdk definition of socket sendto
 *  Input && Output && Return: refer to std sendto
 ***********************************************************/
INT_T unw_send_to(IN CONST INT_T fd, IN CONST VOID *buf, IN CONST UINT_T nbytes,
                  IN CONST UNW_IP_ADDR_T addr, IN CONST USHORT_T port);

/***********************************************************
 *  Function: unw_recv
 *  Desc:     tuya sdk definition of socket recv
 *  Input && Output && Return: refer to std recv
 ***********************************************************/
INT_T unw_recv(IN CONST INT_T fd, OUT VOID *buf, IN CONST UINT_T nbytes);

/***********************************************************
 *  Function: unw_recvfrom
 *  Desc:     tuya sdk definition of socket recvfrom
 *  Input && Output && Return: refer to std recvfrom
 ***********************************************************/
INT_T unw_recvfrom(IN CONST INT_T fd, OUT VOID *buf, IN CONST UINT_T nbytes,
                   OUT UNW_IP_ADDR_T *addr, OUT USHORT_T *port);

/***********************************************************
 *  Function: unw_set_timeout
 *  Desc:     set socket fd timeout option
 *  Input:    fd: socket fd
 *  Input:    ms_timeout: timeout in ms
 *  Input:    type: transfer type
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_timeout(IN CONST INT_T fd, IN CONST INT_T ms_timeout, IN CONST UNW_TRANS_TYPE_E type);

/***********************************************************
 *  Function: unw_set_bufsize
 *  Desc:     set socket fd buffer_size option
 *  Input:    fd: socket fd
 *  Input:    buf_size: buffer size in byte
 *  Input:    type: transfer type
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_bufsize(IN CONST INT_T fd, IN CONST INT_T buf_size, IN CONST UNW_TRANS_TYPE_E type);

/***********************************************************
 *  Function: unw_set_reuse
 *  Desc:     enable socket fd reuse option
 *  Input:    fd: socket fd
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_reuse(IN CONST INT_T fd);

/***********************************************************
 *  Function: unw_disable_nagle
 *  Desc:     disable socket fd nagle option
 *  Input:    fd: socket fd
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_disable_nagle(IN CONST INT_T fd);

/***********************************************************
 *  Function: unw_set_boardcast
 *  Desc:     enable socket broadcast option
 *  Input:    fd: socket fd
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_boardcast(IN CONST INT_T fd);

/***********************************************************
 *  Function: unw_gethostbyname
 *  Desc:     change the domain to addr info
 *  Input:    domain: domin info
 *  Output:   addr: addr info
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_gethostbyname(IN CONST CHAR_T *domain, OUT UNW_IP_ADDR_T *addr);

typedef struct {
	REGION_E region;
	CHAR_T domain[MAX_DOMAIN_NAME_LEN];
	UNW_IP_ADDR_T ip;
} TY_DNS_INFO_S;

typedef OPERATE_RET (*UNW_QUERY_CUSTOM_DNS_CB)(IN CONST CHAR_T *domain, OUT UNW_IP_ADDR_T *addr);
typedef OPERATE_RET (*UNW_SAVE_DNS_INFO_CB)(IN CONST TY_DNS_PRIO_E dns_prio,
                                            IN CONST REGION_E region);

OPERATE_RET unw_init_dns_cache(UNW_QUERY_CUSTOM_DNS_CB dns_query_cb,
                               UNW_SAVE_DNS_INFO_CB dns_save_cb);
OPERATE_RET unw_add_dns_cache(IN CONST CHAR_T *domain, IN CONST UNW_IP_ADDR_T ip);
VOID unm_lower_dns_cache_priority(VOID);
INT_T unm_set_dns_cache_priority(TY_DNS_PRIO_E dns_prio);
VOID unm_set_dns_region(REGION_E region);

/***********************************************************
 *  Function: unw_clear_all_dns_cache unw_clear_dns_cache
 *  Desc:     clear saved dns cache for (all) domain
 *  Input:    domain: domin info
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
OPERATE_RET unw_clear_all_dns_cache(VOID);
OPERATE_RET unw_clear_dns_cache(IN CONST CHAR_T *domain);

/***********************************************************
 *  Function: unw_accept
 *  Desc:     accept the coming socket connect of the server fd
 *  Input:    fd: the server fd
 *  Output:   addr && port: the coming addr info && port
 *  Return:   >0: the coming socket fd.   others: fail
 ***********************************************************/
INT_T unw_accept(IN CONST INT_T fd, OUT UNW_IP_ADDR_T *addr, OUT USHORT_T *port);

/***********************************************************
 *  Function: unw_recv_nd_size
 *  Desc:     recv <nd_size> from the socket fd, store in the <buf>.
 *  Input:    fd: the socket fd
 *  Input:    buf && buf_size: the buffer info
 *  Input:    nd_size: the need size
 *  Output:   buf: the content recv from socket fd
 *  Return:   >0: success   others: fail
 ***********************************************************/
INT_T unw_recv_nd_size(IN CONST INT_T fd, INOUT VOID *buf, IN CONST UINT_T buf_size,
                       IN CONST UINT_T nd_size);

UNW_IP_ADDR_T unw_str2addr(IN CHAR_T *ip_str);

/***********************************************************
 *  Function: unw_set_keepalive
 *  Desc:     set the socket option:SO_KEEPALIVE TCP_KEEPIDLE TCP_KEEPINTVL TCP_KEEPCNT
 *  Input:    fd: the socket fd
 *  Input:    alive && idle && intr && cnt: options
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_keepalive(IN INT_T fd, IN CONST BOOL_T alive, IN CONST UINT_T idle,
                        IN CONST UINT_T intr, IN CONST UINT_T cnt);

/***********************************************************
 *  Function: unw_socket_bind
 *  Desc:     bind the socket fd to a ip
 *  Input:    fd: the socket fd
 *  Input:    ip: ip addr
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_socket_bind(IN INT_T fd, IN CONST CHAR_T *ip);

/***********************************************************
 *  Function: unw_set_cloexec
 *  Desc:     enable socket fd CLOEXEC
 *  Input:    fd: the socket fd
 *  Return:   UNW_SUCCESS: success   others: fail
 ***********************************************************/
INT_T unw_set_cloexec(IN CONST INT_T fd);

INT_T unw_connect_to_domain(INT_T *sockfd, CHAR_T *hostname, USHORT_T port, UINT_T retry_cnt,
                            UINT_T timeout);

#ifdef __cplusplus
}
#endif
#endif

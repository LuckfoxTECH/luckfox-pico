/**
 * @file tuya_hal_network.h
 * @brief Common process - Initialization
 * @version 0.1
 * @date 2020-11-09
 *
 * @copyright Copyright 2020-2021 Tuya Inc. All Rights Reserved.
 *
 */
#ifndef __TUYA_HAL_NETWORK_H__
#define __TUYA_HAL_NETWORK_H__

#include "tuya_os_adapter.h"
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get error code of network
 *
 * @param void
 *
 * @note This API is used for getting error code of network.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
UNW_ERRNO_T tuya_hal_net_get_errno(void);

/**
 * @brief Add file descriptor to set
 *
 * @param[in] fd: file descriptor
 * @param[in] fds: set of file descriptor
 *
 * @note This API is used to add file descriptor to set.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_net_fd_set(int fd, UNW_FD_SET_T *fds);

/**
 * @brief Clear file descriptor from set
 *
 * @param[in] fd: file descriptor
 * @param[in] fds: set of file descriptor
 *
 * @note This API is used to clear file descriptor from set.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_net_fd_clear(int fd, UNW_FD_SET_T *fds);

/**
 * @brief Check file descriptor is in set
 *
 * @param[in] fd: file descriptor
 * @param[in] fds: set of file descriptor
 *
 * @note This API is used to check the file descriptor is in set.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_net_fd_isset(int fd, UNW_FD_SET_T *fds);

/**
 * @brief Clear all file descriptor in set
 *
 * @param[in] fds: set of file descriptor
 *
 * @note This API is used to clear all file descriptor in set.
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_hal_net_fd_zero(UNW_FD_SET_T *fds);

// Add file descriptor to set
#define UNW_FD_SET(n, p) tuya_hal_net_fd_set(n, p)
// Clear file descriptor from set
#define UNW_FD_CLR(n, p) tuya_hal_net_fd_clear(n, p)
// Check file descriptor is in set
#define UNW_FD_ISSET(n, p) tuya_hal_net_fd_isset(n, p)
// Clear all descriptor in set
#define UNW_FD_ZERO(p) tuya_hal_net_fd_zero(p)

/**
 * @brief Get available file descriptors
 *
 * @param[in] maxfd: max count of file descriptor
 * @param[out] readfds: a set of readalbe file descriptor
 * @param[out] writefds: a set of writable file descriptor
 * @param[out] errorfds: a set of except file descriptor
 * @param[in] ms_timeout: time out
 *
 * @note This API is used to get available file descriptors.
 *
 * @return the count of available file descriptors.
 */
int tuya_hal_net_select(const int maxfd, UNW_FD_SET_T *readfds, UNW_FD_SET_T *writefds,
                        UNW_FD_SET_T *errorfds, const UINT_T ms_timeout);

/**
 * @brief Get no block file descriptors
 *
 * @param[in] fd: file descriptor
 *
 * @note This API is used to get no block file descriptors.
 *
 * @return the count of no block file descriptors.
 */
int tuya_hal_net_get_nonblock(const int fd);

/**
 * @brief Set block flag for file descriptors
 *
 * @param[in] fd: file descriptor
 * @param[in] block: block flag
 *
 * @note This API is used to set block flag for file descriptors.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_block(const int fd, const bool block);

/**
 * @brief Close file descriptors
 *
 * @param[in] fd: file descriptor
 *
 * @note This API is used to close file descriptors.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_close(const int fd);

/**
 * @brief Shutdown file descriptors
 *
 * @param[in] fd: file descriptor
 * @param[in] how: shutdown type
 *
 * @note This API is used to shutdown file descriptors.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_shutdown(const int fd, const int how);

/**
 * @brief Create a tcp/udp socket
 *
 * @param[in] type: protocol type, tcp or udp
 *
 * @note This API is used for creating a tcp/udp socket.
 *
 * @return file descriptor
 */
int tuya_hal_net_socket_create(const UNW_PROTOCOL_TYPE type);

/**
 * @brief Connect to network
 *
 * @param[in] fd: file descriptor
 * @param[in] addr: address information of server
 * @param[in] port: port information of server
 *
 * @note This API is used for connecting to network.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_connect(const int fd, const UNW_IP_ADDR_T addr, const UINT16_T port);

/**
 * @brief Connect to network with raw data
 *
 * @param[in] fd: file descriptor
 * @param[in] p_socket: raw socket data
 * @param[in] len: data lenth
 *
 * @note This API is used for connecting to network with raw data.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_connect_raw(const int fd, void *p_socket, const int len);

/**
 * @brief Bind to network
 *
 * @param[in] fd: file descriptor
 * @param[in] addr: address information of server
 * @param[in] port: port information of server
 *
 * @note This API is used for binding to network.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_bind(const int fd, const UNW_IP_ADDR_T addr, const UINT16_T port);

/**
 * @brief Listen to network
 *
 * @param[in] fd: file descriptor
 * @param[in] backlog: max count of backlog connection
 *
 * @note This API is used for listening to network.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_listen(const int fd, const int backlog);

/**
 * @brief Send data to network
 *
 * @param[in] fd: file descriptor
 * @param[in] buf: send data buffer
 * @param[in] nbytes: buffer lenth
 *
 * @note This API is used for sending data to network
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_send(const int fd, const void *buf, const UINT_T nbytes);

/**
 * @brief Send data to specified server
 *
 * @param[in] fd: file descriptor
 * @param[in] buf: send data buffer
 * @param[in] nbytes: buffer lenth
 * @param[in] addr: address information of server
 * @param[in] port: port information of server
 *
 * @note This API is used for sending data to network
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_send_to(const int fd, const void *buf, const UINT_T nbytes,
                         const UNW_IP_ADDR_T addr, const UINT16_T port);

/**
 * @brief Receive data from network
 *
 * @param[in] fd: file descriptor
 * @param[in] buf: receive data buffer
 * @param[in] nbytes: buffer lenth
 *
 * @note This API is used for receiving data from network
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_recv(const int fd, void *buf, const UINT_T nbytes);

/**
 * @brief Receive data from specified server
 *
 * @param[in] fd: file descriptor
 * @param[in] buf: receive data buffer
 * @param[in] nbytes: buffer lenth
 * @param[in] addr: address information of server
 * @param[in] port: port information of server
 *
 * @note This API is used for receiving data from specified server
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_recvfrom(const int fd, void *buf, const UINT_T nbytes, UNW_IP_ADDR_T *addr,
                          UINT16_T *port);

/**
 * @brief Set timeout option of socket fd
 *
 * @param[in] fd: file descriptor
 * @param[in] ms_timeout: timeout in ms
 * @param[in] type: transfer type, receive or send
 *
 * @note This API is used for setting timeout option of socket fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_timeout(const int fd, const int ms_timeout, const UNW_TRANS_TYPE_E type);

/**
 * @brief Set buffer_size option of socket fd
 *
 * @param[in] fd: file descriptor
 * @param[in] buf_size: buffer size in byte
 * @param[in] type: transfer type, receive or send
 *
 * @note This API is used for setting buffer_size option of socket fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_bufsize(const int fd, const int buf_size, const UNW_TRANS_TYPE_E type);

/**
 * @brief Enable reuse option of socket fd
 *
 * @param[in] fd: file descriptor
 *
 * @note This API is used to enable reuse option of socket fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_reuse(const int fd);

/**
 * @brief Disable nagle option of socket fd
 *
 * @param[in] fd: file descriptor
 *
 * @note This API is used to disable nagle option of socket fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_disable_nagle(const int fd);

/**
 * @brief Enable broadcast option of socket fd
 *
 * @param[in] fd: file descriptor
 *
 * @note This API is used to enable broadcast option of socket fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_boardcast(const int fd);

/**
 * @brief Set keepalive option of socket fd to monitor the connection
 *
 * @param[in] fd: file descriptor
 * @param[in] alive: keepalive option, enable or disable option
 * @param[in] idle: keep idle option, if the connection has no data exchange with the idle time(in
 * seconds), start probe.
 * @param[in] intr: keep interval option, the probe time interval.
 * @param[in] cnt: keep count option, probe count.
 *
 * @note This API is used to set keepalive option of socket fd to monitor the connection.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_keepalive(int fd, const bool alive, const UINT_T idle, const UINT_T intr,
                               const UINT_T cnt);

/**
 * @brief Get address information by domain
 *
 * @param[in] domain: domain information
 * @param[in] addr: address information
 *
 * @note This API is used for getting address information by domain.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_gethostbyname(const char *domain, UNW_IP_ADDR_T *addr);

/**
 * @brief Accept the coming socket connection of the server fd
 *
 * @param[in] fd: file descriptor
 * @param[in] addr: address information of server
 * @param[in] port: port information of server
 *
 * @note This API is used for accepting the coming socket connection of the server fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_accept(const int fd, UNW_IP_ADDR_T *addr, UINT16_T *port);

/**
 * @brief Receive data from network with need size
 *
 * @param[in] fd: file descriptor
 * @param[in] buf: receive data buffer
 * @param[in] nbytes: buffer lenth
 * @param[in] nd_size: the need size
 *
 * @note This API is used for receiving data from network with need size
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_recv_nd_size(const int fd, void *buf, const UINT_T buf_size, const UINT_T nd_size);

/**
 * @brief Change ip string to address
 *
 * @param[in] ip_str: ip string
 *
 * @note This API is used to change ip string to address.
 *
 * @return ip address
 */
UNW_IP_ADDR_T tuya_hal_net_str2addr(const char *ip_str);

/**
 * @brief Change ip address to string
 *
 * @param[in] ipaddr: ip address
 *
 * @note This API is used to change ip address(in host byte order) to string(in IPv4
 * numbers-and-dots(xx.xx.xx.xx) notion).
 *
 * @return ip string
 */
char *tuya_hal_net_addr2str(UNW_IP_ADDR_T ipaddr);

/**
 * @brief Bind to network with specified ip
 *
 * @param[in] fd: file descriptor
 * @param[in] ip: ip address
 *
 * @note This API is used for binding to network with specified ip.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_socket_bind(int fd, const char *ip);

/**
 * @brief Set socket fd close mode
 *
 * @param[in] fd: file descriptor
 *
 * @note This API is used for setting socket fd close mode, the socket fd will not be closed in
 * child processes generated by fork calls.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_set_cloexec(const int fd);

/**
 * @brief Get ip address by socket fd
 *
 * @param[in] fd: file descriptor
 * @param[out] addr: ip address
 *
 * @note This API is used for getting ip address by socket fd.
 *
 * @return UNW_SUCCESS on success. Others on error, please refer to tuya_os_adapter_error_code.h
 */
int tuya_hal_net_get_socket_ip(int fd, UNW_IP_ADDR_T *addr);

/**
 * @brief Get ip address by ip string
 *
 * @param[in] ip: ip string
 *
 * @note This API is used for getting ip address by ip string
 *
 * @return ip address
 */
UNW_IP_ADDR_T tuya_hal_net_addr(const char *ip);

/**
 * @brief Network interface initialization
 *
 * @param VOID
 *
 * @note This API is used for initialization of network interface.
 *
 * @return VOID
 */
VOID_T tuya_os_network_intf_init(VOID_T);

#ifdef __cplusplus
}
#endif

#endif // __TUYA_HAL_NETWORK_H__

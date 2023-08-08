#ifndef __TUYA_LITEOS_UV_H__
#define __TUYA_LITEOS_UV_H__

#include <stdint.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
//#include <linux/in.h>
//#include <linux/in6.h>
#include "tuya_uv_errno.h"
#include "unix.h"
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#ifdef __cplusplus
extern "C" {
#endif

/* Expand this list if necessary. */
#define UV_ERRNO_MAP(XX)                                                                           \
	XX(E2BIG, "argument list too long")                                                            \
	XX(EACCES, "permission denied")                                                                \
	XX(EADDRINUSE, "address already in use")                                                       \
	XX(EADDRNOTAVAIL, "address not available")                                                     \
	XX(EAFNOSUPPORT, "address family not supported")                                               \
	XX(EAGAIN, "resource temporarily unavailable")                                                 \
	XX(EAI_ADDRFAMILY, "address family not supported")                                             \
	XX(EAI_AGAIN, "temporary failure")                                                             \
	XX(EAI_BADFLAGS, "bad ai_flags value")                                                         \
	XX(EAI_BADHINTS, "invalid value for hints")                                                    \
	XX(EAI_CANCELED, "request canceled")                                                           \
	XX(EAI_FAIL, "permanent failure")                                                              \
	XX(EAI_FAMILY, "ai_family not supported")                                                      \
	XX(EAI_MEMORY, "out of memory")                                                                \
	XX(EAI_NODATA, "no address")                                                                   \
	XX(EAI_NONAME, "unknown node or service")                                                      \
	XX(EAI_OVERFLOW, "argument buffer overflow")                                                   \
	XX(EAI_PROTOCOL, "resolved protocol is unknown")                                               \
	XX(EAI_SERVICE, "service not available for socket type")                                       \
	XX(EAI_SOCKTYPE, "socket type not supported")                                                  \
	XX(EALREADY, "connection already in progress")                                                 \
	XX(EBADF, "bad file descriptor")                                                               \
	XX(EBUSY, "resource busy or locked")                                                           \
	XX(ECANCELED, "operation canceled")                                                            \
	XX(ECHARSET, "invalid Unicode character")                                                      \
	XX(ECONNABORTED, "software caused connection abort")                                           \
	XX(ECONNREFUSED, "connection refused")                                                         \
	XX(ECONNRESET, "connection reset by peer")                                                     \
	XX(EDESTADDRREQ, "destination address required")                                               \
	XX(EEXIST, "file already exists")                                                              \
	XX(EFAULT, "bad address in system call argument")                                              \
	XX(EFBIG, "file too large")                                                                    \
	XX(EHOSTUNREACH, "host is unreachable")                                                        \
	XX(EINTR, "interrupted system call")                                                           \
	XX(EINVAL, "invalid argument")                                                                 \
	XX(EIO, "i/o error")                                                                           \
	XX(EISCONN, "socket is already connected")                                                     \
	XX(EISDIR, "illegal operation on a directory")                                                 \
	XX(ELOOP, "too many symbolic links encountered")                                               \
	XX(EMFILE, "too many open files")                                                              \
	XX(EMSGSIZE, "message too long")                                                               \
	XX(ENAMETOOLONG, "name too long")                                                              \
	XX(ENETDOWN, "network is down")                                                                \
	XX(ENETUNREACH, "network is unreachable")                                                      \
	XX(ENFILE, "file table overflow")                                                              \
	XX(ENOBUFS, "no buffer space available")                                                       \
	XX(ENODEV, "no such device")                                                                   \
	XX(ENOENT, "no such file or directory")                                                        \
	XX(ENOMEM, "not enough memory")                                                                \
	XX(ENONET, "machine is not on the network")                                                    \
	XX(ENOPROTOOPT, "protocol not available")                                                      \
	XX(ENOSPC, "no space left on device")                                                          \
	XX(ENOSYS, "function not implemented")                                                         \
	XX(ENOTCONN, "socket is not connected")                                                        \
	XX(ENOTDIR, "not a directory")                                                                 \
	XX(ENOTEMPTY, "directory not empty")                                                           \
	XX(ENOTSOCK, "socket operation on non-socket")                                                 \
	XX(ENOTSUP, "operation not supported on socket")                                               \
	XX(EPERM, "operation not permitted")                                                           \
	XX(EPIPE, "broken pipe")                                                                       \
	XX(EPROTO, "protocol error")                                                                   \
	XX(EPROTONOSUPPORT, "protocol not supported")                                                  \
	XX(EPROTOTYPE, "protocol wrong type for socket")                                               \
	XX(ERANGE, "result too large")                                                                 \
	XX(EROFS, "read-only file system")                                                             \
	XX(ESHUTDOWN, "cannot send after transport endpoint shutdown")                                 \
	XX(ESPIPE, "invalid seek")                                                                     \
	XX(ESRCH, "no such process")                                                                   \
	XX(ETIMEDOUT, "connection timed out")                                                          \
	XX(ETXTBSY, "text file is busy")                                                               \
	XX(EXDEV, "cross-device link not permitted")                                                   \
	XX(UNKNOWN, "unknown error")                                                                   \
	XX(EOF, "end of file")                                                                         \
	XX(ENXIO, "no such device or address")                                                         \
	XX(EMLINK, "too many links")                                                                   \
	XX(EHOSTDOWN, "host is down")                                                                  \
	XX(EREMOTEIO, "remote I/O error")                                                              \
	XX(ENOTTY, "inappropriate ioctl for device")                                                   \
	XX(EFTYPE, "inappropriate file type or format")                                                \
	XX(EILSEQ, "illegal byte sequence")                                                            \
	XX(EUNKNOWN, "unknown")

typedef enum {
#define XX(code, _) UV_##code = UV__##code,
	UV_ERRNO_MAP(XX)
#undef XX
	    UV_ERRNO_MAX = UV__EOF - 1
} uv_errno_t;

#define FD_SET_RDINDEX 0
#define FD_SET_WRINDEX 1
#define FD_SET_EXINDEX 2
#define FD_SET_NUM 3

typedef enum { UV_LOOP_BLOCK_SIGNAL } uv_loop_option;

typedef enum { UV_RUN_DEFAULT = 0, UV_RUN_ONCE, UV_RUN_NOWAIT } uv_run_mode;

typedef enum { UV_UDP = 1, UV_TCP, UV_TIMER, UV_HANDLE_TYPE_MAX } uv_handle_type;

typedef struct uv_loop_s uv_loop_t;
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_stream_s uv_stream_t;
typedef struct uv_tcp_s uv_tcp_t;
typedef struct uv_udp_s uv_udp_t;
typedef struct uv_timer_s uv_timer_t;

/* Request types. */
typedef struct uv_req_s uv_req_t;
typedef struct uv_getaddrinfo_s uv_getaddrinfo_t;
typedef struct uv_shutdown_s uv_shutdown_t;
typedef struct uv_write_s uv_write_t;
typedef struct uv_connect_s uv_connect_t;
typedef struct uv_udp_send_s uv_udp_send_t;

typedef void (*uv_alloc_cb)(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
typedef void (*uv_read_cb)(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf);
typedef void (*uv_write_cb)(uv_write_t *req, int status);
typedef void (*uv_connect_cb)(uv_connect_t *req, int status);
typedef void (*uv_shutdown_cb)(uv_shutdown_t *req, int status);
typedef void (*uv_connection_cb)(uv_stream_t *server, int status);
typedef void (*uv_close_cb)(uv_handle_t *handle);
typedef void (*uv_timer_cb)(uv_timer_t *handle);
typedef void (*uv_udp_send_cb)(uv_udp_send_t *req, int status);
typedef void (*uv_udp_recv_cb)(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf,
                               const struct sockaddr *addr, unsigned flags);

typedef void (*uv_getaddrinfo_cb)(uv_getaddrinfo_t *req, int status, struct addrinfo *res);

struct uv_loop_s {
	/* User data - use this for whatever. */
	void *data;
	/* Loop reference counting. */
	unsigned int active_handles;
	void *handle_queue[2];
	union {
		void *unused[2];
		unsigned int count;
	} active_reqs;
	int stop_flag;
	UV_LOOP_PRIVATE_FIELDS
};

#define UV_HANDLE_PRIVATE_FIELDS                                                                   \
	uv_handle_t *next_closing;                                                                     \
	unsigned int flags;
#define UV_HANDLE_FIELDS                                                                           \
	/* public */                                                                                   \
	void *data;                                                                                    \
	/* read-only */                                                                                \
	uv_loop_t *loop;                                                                               \
	int type;                                                                                      \
	/* private */                                                                                  \
	uv_close_cb close_cb;                                                                          \
	void *handle_queue[2];                                                                         \
	UV_HANDLE_PRIVATE_FIELDS
/* The abstract base class of all handles. */
struct uv_handle_s {
	UV_HANDLE_FIELDS
};
struct uv_timer_s {
	UV_HANDLE_FIELDS
	UV_TIMER_PRIVATE_FIELDS
};
enum uv_udp_flags {
	/* Disables dual stack mode. */
	UV_UDP_IPV6ONLY = 1,
	/*
	 * Indicates message was truncated because read buffer was too small. The
	 * remainder was discarded by the OS. Used in uv_udp_recv_cb.
	 */
	UV_UDP_PARTIAL = 2,
	/*
	 * Indicates if SO_REUSEADDR will be set when binding the handle.
	 * This sets the SO_REUSEPORT socket flag on the BSDs and OS X. On other
	 * Unix platforms, it sets the SO_REUSEADDR flag.  What that means is that
	 * multiple threads or processes can bind to the same address without error
	 * (provided they all set the flag) but only the last one to bind will receive
	 * any traffic, in effect "stealing" the port from the previous listener.
	 */
	UV_UDP_REUSEADDR = 4
};
struct uv_udp_s {
	UV_HANDLE_FIELDS
	size_t send_queue_size;
	size_t send_queue_count;
	UV_UDP_PRIVATE_FIELDS
};
#define UV_STREAM_PRIVATE_FIELDS                                                                   \
	uv_connect_t *connect_req;                                                                     \
	uv_shutdown_t *shutdown_req;                                                                   \
	uv__io_t io_watcher;                                                                           \
	void *write_queue[2];                                                                          \
	void *write_completed_queue[2];                                                                \
	uv_connection_cb connection_cb;                                                                \
	int delayed_error;                                                                             \
	int accepted_fd;                                                                               \
	void *queued_fds;                                                                              \
	UV_STREAM_PRIVATE_PLATFORM_FIELDS
#define UV_STREAM_FIELDS                                                                           \
	/* number of bytes queued for writing */                                                       \
	size_t write_queue_size;                                                                       \
	uv_alloc_cb alloc_cb;                                                                          \
	uv_read_cb read_cb;                                                                            \
	/* private */                                                                                  \
	UV_STREAM_PRIVATE_FIELDS
/*
 * uv_stream_t is a subclass of uv_handle_t.
 *
 * uv_stream is an abstract class.
 *
 * uv_stream_t is the parent class of uv_tcp_t, uv_pipe_t and uv_tty_t.
 */
struct uv_stream_s {
	UV_HANDLE_FIELDS
	UV_STREAM_FIELDS
};
/*
 * uv_tcp_t is a subclass of uv_stream_t.
 *
 * Represents a TCP stream or TCP server.
 */
struct uv_tcp_s {
	UV_HANDLE_FIELDS
	UV_STREAM_FIELDS
	UV_TCP_PRIVATE_FIELDS
};
enum uv_tcp_flags {
	/* Used with uv_tcp_bind, when an IPv6 address is used. */
	UV_TCP_IPV6ONLY = 1
};

#define UV_REQ_TYPE_MAP(XX)                                                                        \
	XX(REQ, req)                                                                                   \
	XX(CONNECT, connect)                                                                           \
	XX(WRITE, write)                                                                               \
	XX(SHUTDOWN, shutdown)                                                                         \
	XX(UDP_SEND, udp_send)                                                                         \
	XX(FS, fs)                                                                                     \
	XX(WORK, work)                                                                                 \
	XX(GETADDRINFO, getaddrinfo)                                                                   \
	XX(GETNAMEINFO, getnameinfo)

typedef enum {
	UV_UNKNOWN_REQ = 0,
#define XX(uc, lc) UV_##uc,
	UV_REQ_TYPE_MAP(XX)
#undef XX
	    UV_REQ_TYPE_MAX
} uv_req_type;
#define UV_REQ_FIELDS                                                                              \
	/* public */                                                                                   \
	void *data;                                                                                    \
	/* read-only */                                                                                \
	uv_req_type type;                                                                              \
	/* private */                                                                                  \
	void *reserved[6];                                                                             \
	uv_loop_t *loop;                                                                               \
	UV_REQ_PRIVATE_FIELDS
/* Abstract base class of all requests. */
struct uv_req_s {
	UV_REQ_FIELDS
};
/* uv_udp_send_t is a subclass of uv_req_t. */
struct uv_udp_send_s {
	UV_REQ_FIELDS
	uv_udp_t *handle;
	uv_udp_send_cb cb;
	UV_UDP_SEND_PRIVATE_FIELDS
};
struct uv_shutdown_s {
	UV_REQ_FIELDS
	uv_stream_t *handle;
	uv_shutdown_cb cb;
	UV_SHUTDOWN_PRIVATE_FIELDS
};
/* uv_write_t is a subclass of uv_req_t. */
struct uv_write_s {
	UV_REQ_FIELDS
	uv_write_cb cb;
	uv_stream_t *handle;
	UV_WRITE_PRIVATE_FIELDS
};
/* uv_connect_t is a subclass of uv_req_t. */
struct uv_connect_s {
	UV_REQ_FIELDS
	uv_connect_cb cb;
	uv_stream_t *handle;
	UV_CONNECT_PRIVATE_FIELDS
};
typedef struct uv__work {
	struct uv__work *next_work;
	void (*work)(struct uv__work *w);
	void (*done)(struct uv__work *w, int status);
	int work_no;
} uv__work_t;
/*
 * uv_getaddrinfo_t is a subclass of uv_req_t.
 *
 * Request object for uv_getaddrinfo.
 */
struct uv_getaddrinfo_s {
	UV_REQ_FIELDS
	/* struct addrinfo* addrinfo is marked as private, but it really isn't. */
	UV_GETADDRINFO_PRIVATE_FIELDS
};
typedef struct uv_interface_address_s {
	char *name;
	char phys_addr[6];
	int is_internal;
	union {
		struct sockaddr_in address4;
		struct sockaddr_in6 address6;
	} address;
	union {
		struct sockaddr_in netmask4;
		struct sockaddr_in6 netmask6;
	} netmask;
} uv_interface_address_t;

typedef void *(*uv_malloc_func)(size_t size);
typedef void *(*uv_realloc_func)(void *ptr, size_t size);
typedef void *(*uv_calloc_func)(size_t count, size_t size);
typedef void (*uv_free_func)(void *ptr);

int uv_replace_allocator(uv_malloc_func malloc_func, uv_realloc_func realloc_func,
                         uv_calloc_func calloc_func, uv_free_func free_func);

int uv_run(uv_loop_t *, uv_run_mode mode);
uv_buf_t uv_buf_init(char *base, size_t len);
const char *uv_strerror(int err);
uint64_t uv_hrtime(void);

void uv_close(uv_handle_t *handle, uv_close_cb close_cb);
int uv_is_closing(const uv_handle_t *handle);
int uv_fileno(const uv_handle_t *handle, int *fd);
void *uv_handle_get_data(const uv_handle_t *handle);
void uv_handle_set_data(uv_handle_t *handle, void *data);

int uv_loop_init(uv_loop_t *loop);
uv_loop_t *uv_default_loop(void);
int uv_loop_close(uv_loop_t *loop);

int uv_timer_init(uv_loop_t *, uv_timer_t *handle);
int uv_timer_start(uv_timer_t *handle, uv_timer_cb cb, uint64_t timeout, uint64_t repeat);
int uv_timer_stop(uv_timer_t *handle);

int uv_mutex_init(uv_mutex_t *handle);
void uv_mutex_destroy(uv_mutex_t *handle);
void uv_mutex_lock(uv_mutex_t *handle);
void uv_mutex_unlock(uv_mutex_t *handle);
void uv_cond_signal(uv_cond_t *cond);
int uv_cond_init(uv_cond_t *cond);
void uv_cond_destroy(uv_cond_t *cond);
int uv_cond_timedwait(uv_cond_t *cond, uv_mutex_t *mutex, uint64_t timeout);

int uv_ip4_addr(const char *ip, int port, struct sockaddr_in *addr);
int uv_ip4_name(const struct sockaddr_in *src, char *dst, size_t size);
int uv_ip6_name(const struct sockaddr_in6 *src, char *dst, size_t size);
int uv_ip6_addr(const char *ip, int port, struct sockaddr_in6 *addr);
int uv_inet_ntop(int af, const void *src, char *dst, size_t size);
int uv_interface_addresses(uv_interface_address_t **addresses, int *count);
void uv_free_interface_addresses(uv_interface_address_t *addresses, int count);
int uv_inet_pton(int af, const char *src, void *dst);
void uv_freeaddrinfo(struct addrinfo *ai);
int uv_getaddrinfo(uv_loop_t *loop, uv_getaddrinfo_t *req, uv_getaddrinfo_cb getaddrinfo_cb,
                   const char *node, const char *service, const struct addrinfo *hints);
void uv_print_address(struct addrinfo *addrinfo, char *print_info);

int uv_cancel(uv_req_t *req);
int uv_shutdown(uv_shutdown_t *req, uv_stream_t *handle, uv_shutdown_cb cb);
size_t uv_stream_get_write_queue_size(const uv_stream_t *stream);
int uv_listen(uv_stream_t *stream, int backlog, uv_connection_cb cb);
int uv_accept(uv_stream_t *server, uv_stream_t *client);
int uv_read_start(uv_stream_t *, uv_alloc_cb alloc_cb, uv_read_cb read_cb);
int uv_read_stop(uv_stream_t *);
int uv_write(uv_write_t *req, uv_stream_t *handle, const uv_buf_t bufs[], unsigned int nbufs,
             uv_write_cb cb);
int uv_try_write(uv_stream_t *handle, const uv_buf_t bufs[], unsigned int nbufs);
int uv_is_readable(const uv_stream_t *handle);
int uv_is_writable(const uv_stream_t *handle);
int uv_stream_set_blocking(uv_stream_t *handle, int blocking);

int uv_tcp_init(uv_loop_t *, uv_tcp_t *handle);
int uv_tcp_init_ex(uv_loop_t *, uv_tcp_t *handle, unsigned int flags);
int uv_tcp_open(uv_tcp_t *handle, uv_os_sock_t sock);
int uv_tcp_nodelay(uv_tcp_t *handle, int enable);
int uv_tcp_keepalive(uv_tcp_t *handle, int enable, unsigned int delay);
int uv_tcp_simultaneous_accepts(uv_tcp_t *handle, int enable);
int uv_tcp_bind(uv_tcp_t *handle, const struct sockaddr *addr, unsigned int flags);
int uv_tcp_getsockname(const uv_tcp_t *handle, struct sockaddr *name, int *namelen);
int uv_tcp_getpeername(const uv_tcp_t *handle, struct sockaddr *name, int *namelen);
int uv_tcp_close_reset(uv_tcp_t *handle, uv_close_cb close_cb);
int uv_tcp_connect(uv_connect_t *req, uv_tcp_t *handle, const struct sockaddr *addr,
                   uv_connect_cb cb);

int uv_udp_init(uv_loop_t *, uv_udp_t *handle);
int uv_udp_init_ex(uv_loop_t *, uv_udp_t *handle, unsigned int flags);
int uv_udp_open(uv_udp_t *handle, uv_os_sock_t sock);
int uv_udp_bind(uv_udp_t *handle, const struct sockaddr *addr, unsigned int flags);
int uv_udp_connect(uv_udp_t *handle, const struct sockaddr *addr);
int uv_udp_getpeername(const uv_udp_t *handle, struct sockaddr *name, int *namelen);
int uv_udp_getsockname(const uv_udp_t *handle, struct sockaddr *name, int *namelen);
int uv_udp_send(uv_udp_send_t *req, uv_udp_t *handle, const uv_buf_t bufs[], unsigned int nbufs,
                const struct sockaddr *addr, uv_udp_send_cb send_cb);
int uv_udp_try_send(uv_udp_t *handle, const uv_buf_t bufs[], unsigned int nbufs,
                    const struct sockaddr *addr);
int uv_udp_recv_start(uv_udp_t *handle, uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb);
int uv_udp_recv_stop(uv_udp_t *handle);
size_t uv_udp_get_send_queue_size(const uv_udp_t *handle);
size_t uv_udp_get_send_queue_count(const uv_udp_t *handle);

#ifdef __cplusplus
}
#endif

#endif

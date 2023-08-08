#ifndef _UNIX_H_
#define _UNIX_H_

#include <pthread.h>

typedef pthread_t uv_thread_t;
typedef pthread_mutex_t uv_mutex_t;
typedef pthread_cond_t uv_cond_t;
typedef int uv_os_sock_t;
typedef struct uv_buf_t {
	char *base;
	size_t len;
} uv_buf_t;

#ifndef UV_IO_PRIVATE_PLATFORM_FIELDS
#define UV_IO_PRIVATE_PLATFORM_FIELDS /* empty */
#endif

struct uv__io_s;
struct uv_loop_s;
typedef void (*uv__io_cb)(struct uv_loop_s *loop, struct uv__io_s *w, unsigned int events);
typedef struct uv__io_s uv__io_t;

struct uv__io_s {
	uv__io_cb cb;
	void *pending_queue[2];
	void *watcher_queue[2];
	unsigned int pevents; /* Pending event mask i.e. mask at next tick. */
	unsigned int events;  /* Current event mask. */
	int fd;
	UV_IO_PRIVATE_PLATFORM_FIELDS
};

#ifndef UV_PLATFORM_LOOP_FIELDS
#define UV_PLATFORM_LOOP_FIELDS /* empty */
#endif

#define UV_LOOP_PRIVATE_FIELDS                                                                     \
	unsigned long flags;                                                                           \
	void *pending_queue[2];                                                                        \
	void *watcher_queue[2];                                                                        \
	uv__io_t **watchers;                                                                           \
	unsigned int nwatchers;                                                                        \
	unsigned int nfds;                                                                             \
	uv_handle_t *closing_handles;                                                                  \
	struct {                                                                                       \
		void *min;                                                                                 \
		unsigned int nelts;                                                                        \
	} timer_heap;                                                                                  \
	uint64_t timer_counter;                                                                        \
	uint64_t time;                                                                                 \
	struct uv__work *works;                                                                        \
	int work_alloc_no;                                                                             \
	uv_mutex_t lock;

#define UV_SHUTDOWN_PRIVATE_FIELDS /* empty */

#define UV_TIMER_PRIVATE_FIELDS                                                                    \
	uv_timer_cb timer_cb;                                                                          \
	void *heap_node[3];                                                                            \
	uint64_t timeout;                                                                              \
	uint64_t repeat;                                                                               \
	uint64_t start_id;                                                                             \
	uint16_t used;

#define UV_UDP_PRIVATE_FIELDS                                                                      \
	uv_udp_recv_cb recv_cb;                                                                        \
	uv_alloc_cb alloc_cb;                                                                          \
	uv__io_t io_watcher;                                                                           \
	void *write_queue[2];                                                                          \
	void *write_completed_queue[2];

#define UV_UDP_SEND_PRIVATE_FIELDS                                                                 \
	void *queue[2];                                                                                \
	struct sockaddr_storage addr;                                                                  \
	unsigned int nbufs;                                                                            \
	uv_buf_t *bufs;                                                                                \
	ssize_t status;                                                                                \
	uv_udp_send_cb send_cb;                                                                        \
	uv_buf_t bufsml[4];

#ifndef UV_STREAM_PRIVATE_PLATFORM_FIELDS
#define UV_STREAM_PRIVATE_PLATFORM_FIELDS /* empty */
#endif

#define UV_TCP_PRIVATE_FIELDS /* empty */

#define UV_REQ_PRIVATE_FIELDS /* empty */

#define UV_WRITE_PRIVATE_FIELDS                                                                    \
	void *queue[2];                                                                                \
	unsigned int write_index;                                                                      \
	uv_buf_t *bufs;                                                                                \
	unsigned int nbufs;                                                                            \
	int error;                                                                                     \
	uv_buf_t bufsml[4];

#define UV_CONNECT_PRIVATE_FIELDS void *queue[2];

#define UV_GETADDRINFO_PRIVATE_FIELDS                                                              \
	struct uv__work work_req;                                                                      \
	uv_getaddrinfo_cb cb;                                                                          \
	struct addrinfo *hints;                                                                        \
	char *hostname;                                                                                \
	char *service;                                                                                 \
	struct addrinfo *addrinfo;                                                                     \
	int retcode;

#endif
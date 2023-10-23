#ifndef __BT_LOG__
#define __BT_LOG__
#include <syslog.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

//#define SYSLOG_DEBUG

static char *get_cur_time()
{
	static char s[20];
	struct timeval vt;

	gettimeofday(&vt, NULL);
	sprintf(s, "%ld:%ld", vt.tv_sec, vt.tv_usec);

	return s;
}

#define pr_info(fmt, args...) \
		printf("[%s][%s][%s][l:%d]," fmt "\r\n", get_cur_time(), __FILE__, __FUNCTION__, __LINE__, ##args)

#define pr_err(fmt, args...) \
		printf("[%s][%s][%s][l:%d]," fmt "\r\n", get_cur_time(), __FILE__, __FUNCTION__, __LINE__, ##args)

#ifdef SYSLOG_DEBUG
#define sample_log_print(fmt, args...) \
					printf("[%s][%s][%s][l:%d]," fmt "\r\n", get_cur_time(), __FILE__, __FUNCTION__, __LINE__, ##args)
#define pr_debug(fmt, args...) \
					printf("[%s][%s][%s][l:%d]," fmt "\r\n", get_cur_time(), __FILE__, __FUNCTION__, __LINE__, ##args)
#else
#define sample_log_print(fmt, args...)
#define pr_debug(fmt, args...)
#endif

#ifdef __cplusplus
}
#endif

#endif

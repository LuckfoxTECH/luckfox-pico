#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <unistd.h>

#include "socket.h"

union {
  struct cmsghdr cm;
  char control[CMSG_SPACE(sizeof(int))];
} control_un;

int cli_begin(char *name) {
  int fd;
  int len = 0;

  fd = cli_connect(CS_PATH);
  if (fd < 0)
    return fd;
  len = strlen(name) + 1;
  sock_write(fd, (char *)&len, sizeof(int));
  sock_write(fd, name, len);

  return fd;
}

int cli_end(int fd) {
  int ret;

  sock_read(fd, &ret, sizeof(int));
  close(fd);

  return 0;
}

int cli_connect(const char *name) {
  int ret;
  int fd, len;
  struct sockaddr_un unix_addr;

  if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    exit(0);

  memset(&unix_addr, 0, sizeof(unix_addr));
  unix_addr.sun_family = AF_UNIX;
  strcpy(unix_addr.sun_path, name);
  len = sizeof(unix_addr.sun_family) + strlen(unix_addr.sun_path);

  if (connect(fd, (struct sockaddr *)&unix_addr, len) < 0)
    goto error;

  if (sock_read(fd, &ret, sizeof(int)) == SOCKERR_CLOSED)
    return -1;

  return (fd);

error:
  printf("%s err\n", __func__);
  close(fd);
  return (-1);
}

int sock_write(int fd, const void *buff, int count) {
  char *pts = (char *)buff;
  int status = 0, n;

  if (count < 0)
    return SOCKERR_OK;

  while (status != count) {
    n = write(fd, (void *)&pts[status], count - status);

    if (n < 0) {
      if (errno == EPIPE)
        return SOCKERR_CLOSED;
      else if (errno == EINTR)
        continue;
      else
        return SOCKERR_IO;
    }
    status += n;
  }

  return status;
}

int sock_read(int fd, void *buff, int count) {
  char *pts = (char *)buff;
  int status = 0, n;

  if (count <= 0)
    return SOCKERR_OK;
  while (status != count) {
    n = read(fd, (void *)&pts[status], count - status);

    if (n < 0) {
      if (errno == EINTR)
        continue;
      else
        return SOCKERR_IO;
    }

    if (n == 0)
      return SOCKERR_CLOSED;

    status += n;
  }

  return status;
}

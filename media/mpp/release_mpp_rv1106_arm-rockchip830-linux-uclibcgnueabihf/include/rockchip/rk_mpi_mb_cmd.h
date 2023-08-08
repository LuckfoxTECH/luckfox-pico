/* GPL-2.0 WITH Linux-syscall-note OR Apache 2.0 */
/* Copyright (c) 2021 Fuzhou Rockchip Electronics Co., Ltd */

#ifndef INCLUDE_RK_MPI_MB_CMD_H_
#define INCLUDE_RK_MPI_MB_CMD_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define IO_STRING_MAX_LEN 16
#ifndef SET_STRUCT_SIZE
#define SET_STRUCT_SIZE(struct_name, p)                                        \
  ((p)->struct_size = sizeof(struct struct_name))
#define STRUCT_SIZE_INVALID(struct_name, p)                                    \
  ((p)->struct_size != sizeof(struct struct_name))
#endif

/**
 * VALLOC_IOCTL_VERSION ioctl argument type.
 */
struct valloc_version {
  int version_major;            /**< Major version */
  int version_minor;            /**< Minor version */
  int version_patchlevel;       /**< Patch level */
  int name_len;                 /**< Length of name buffer */
  char name[IO_STRING_MAX_LEN]; /**< Name of driver */
  int struct_size;              /**< check struct size */
  int date_len;                 /**< Length of date buffer */
  char date[IO_STRING_MAX_LEN]; /**< User-space buffer to hold date */
  int desc_len;                 /**< Length of desc buffer */
  char desc[IO_STRING_MAX_LEN]; /**< User-space buffer to hold desc */
};

/**
 * mb flags
 */
enum mb_flag {
  /** support iommu */
  MB_FLAG_IOMMU = (1 << 0),
  /** continue physical memory */
  MB_FLAG_CONTIG = (1 << 1),
  /** use cache */
  MB_FLAG_CACHABLE = (1 << 2),
  /** use kmap area */
  MB_FLAG_KMAP = (1 << 3),
  /** write cache and memory at the same time */
  MB_FLAG_WC = (1 << 4),
};

/**
 * VALLOC_IOCTL_MB_GET_FD ioctl argument type.
 */
struct valloc_mb {
  int pool_id;
  int mpi_buf_id;
  int dma_buf_fd;
  int struct_size;
  int size;
  int flags;
  int paddr;
};

#define VALLOC_IOCTL_BASE 'a'
#define VALLOC_IO(nr) _IO(VALLOC_IOCTL_BASE, nr)
#define VALLOC_IOR(nr, type) _IOR(VALLOC_IOCTL_BASE, nr, type)
#define VALLOC_IOW(nr, type) _IOW(VALLOC_IOCTL_BASE, nr, type)
#define VALLOC_IOWR(nr, type) _IOWR(VALLOC_IOCTL_BASE, nr, type)

#define VALLOC_IOCTL_VERSION VALLOC_IOWR(0x00, struct valloc_version)
#define VALLOC_IOCTL_MB_GET_FD VALLOC_IOWR(0x01, struct valloc_mb)
#define VALLOC_IOCTL_MB_CREATE VALLOC_IOWR(0x02, struct valloc_mb)
#define VALLOC_IOCTL_MB_DELETE VALLOC_IOWR(0x03, struct valloc_mb)
#define VALLOC_IOCTL_MB_GET_PHY_ADDR VALLOC_IOWR(0x04, struct valloc_mb)
#define VALLOC_IOCTL_MB_GET_ID VALLOC_IOWR(0x05, struct valloc_mb)

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* End of INCLUDE_RK_MPI_SYS_CMD_H_ */

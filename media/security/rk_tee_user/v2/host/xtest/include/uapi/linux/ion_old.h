/* SPDX-License-Identifier: GPL-2.0 */
/*
 * drivers/staging/android/uapi/ion.h (This is the ION Uapi prior kernel 4.12)
 *
 * Copyright (C) 2011 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _UAPI_LINUX_ION_OLD_H
#define _UAPI_LINUX_ION_OLD_H

/* All ION resources are prefixed ION0_ to ditinguish from companion new API */

#include <linux/ioctl.h>
#include <linux/types.h>

typedef int ion0_user_handle_t;

/**
 * struct ion0_allocation_data - metadata passed from userspace for allocations
 * @len:		size of the allocation
 * @align:		required alignment of the allocation
 * @heap_id_mask:	mask of heap ids to allocate from
 * @flags:		flags passed to heap
 * @handle:		pointer that will be populated with a cookie to use to
 *			refer to this allocation
 *
 * Provided by userspace as an argument to the ioctl
 */
struct ion0_allocation_data {
	size_t len;
	size_t align;
	unsigned int heap_id_mask;
	unsigned int flags;
	ion0_user_handle_t handle;
};

/**
 * struct ion0_fd_data - metadata passed to/from userspace for a handle/fd pair
 * @handle:	a handle
 * @fd:		a file descriptor representing that handle
 *
 * For ION_IOC_SHARE or ION_IOC_MAP userspace populates the handle field with
 * the handle returned from ion alloc, and the kernel returns the file
 * descriptor to share or map in the fd field.  For ION_IOC_IMPORT, userspace
 * provides the file descriptor and the kernel returns the handle.
 */
struct ion0_fd_data {
	ion0_user_handle_t handle;
	int fd;
};

/**
 * struct ion0_handle_data - a handle passed to/from the kernel
 * @handle:	a handle
 */
struct ion0_handle_data {
	ion0_user_handle_t handle;
};

/**
 * struct ion0_custom_data - metadata passed to/from userspace for a custom ioctl
 * @cmd:	the custom ioctl function to call
 * @arg:	additional data to pass to the custom ioctl, typically a user
 *		pointer to a predefined structure
 *
 * This works just like the regular cmd and arg fields of an ioctl.
 */
struct ion0_custom_data {
	unsigned int cmd;
	unsigned long arg;
};

/**
 * DOC: ION_IOC_ALLOC - allocate memory
 *
 * Takes an ion_allocation_data struct and returns it with the handle field
 * populated with the opaque handle for the allocation.
 */
#define ION0_IOC_ALLOC		_IOWR(ION_IOC_MAGIC, 0, \
				      struct ion0_allocation_data)
/**
 * DOC: ION_IOC_FREE - free memory
 *
 * Takes an ion_handle_data struct and frees the handle.
 */
#define ION0_IOC_FREE		_IOWR(ION_IOC_MAGIC, 1, struct ion0_handle_data)

/**
 * DOC: ION_IOC_MAP - get a file descriptor to mmap
 *
 * Takes an ion0_fd_data struct with the handle field populated with a valid
 * opaque handle.  Returns the struct with the fd field set to a file
 * descriptor open in the current address space.  This file descriptor
 * can then be used as an argument to mmap.
 */
#define ION0_IOC_MAP		_IOWR(ION_IOC_MAGIC, 2, struct ion0_fd_data)

/**
 * DOC: ION_IOC_SHARE - creates a file descriptor to use to share an allocation
 *
 * Takes an ion_fd_data struct with the handle field populated with a valid
 * opaque handle.  Returns the struct with the fd field set to a file
 * descriptor open in the current address space.  This file descriptor
 * can then be passed to another process.  The corresponding opaque handle can
 * be retrieved via ION_IOC_IMPORT.
 */
#define ION0_IOC_SHARE		_IOWR(ION_IOC_MAGIC, 4, struct ion0_fd_data)

/**
 * DOC: ION_IOC_IMPORT - imports a shared file descriptor
 *
 * Takes an ion0_fd_data struct with the fd field populated with a valid file
 * descriptor obtained from ION_IOC_SHARE and returns the struct with the handle
 * filed set to the corresponding opaque handle.
 */
#define ION0_IOC_IMPORT		_IOWR(ION_IOC_MAGIC, 5, struct ion0_fd_data)

/**
 * DOC: ION_IOC_SYNC - syncs a shared file descriptors to memory
 *
 * Deprecated in favor of using the dma_buf api's correctly (syncing
 * will happen automatically when the buffer is mapped to a device).
 * If necessary should be used after touching a cached buffer from the cpu,
 * this will make the buffer in memory coherent.
 */
#define ION0_IOC_SYNC		_IOWR(ION_IOC_MAGIC, 7, struct ion0_fd_data)

/**
 * DOC: ION_IOC_CUSTOM - call architecture specific ion ioctl
 *
 * Takes the argument of the architecture specific ioctl to call and
 * passes appropriate userdata for that ioctl
 */
#define ION0_IOC_CUSTOM		_IOWR(ION_IOC_MAGIC, 6, struct ion0_custom_data)

#endif /* _UAPI_LINUX_ION_OLD_H */

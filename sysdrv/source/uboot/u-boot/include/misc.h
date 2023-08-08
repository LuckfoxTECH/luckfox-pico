/*
 * Copyright (C) 2015 Thomas Chou <thomas@wytron.com.tw>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _MISC_H_
#define _MISC_H_

#include <asm-generic/ioctl.h>

/*
 * Request command to be sent for misc_ioctl().
 */
#define IOCTL_REQ_START		_IO('m', 0x01)
#define IOCTL_REQ_STOP		_IO('m', 0x02)
#define IOCTL_REQ_POLL		_IO('m', 0x03)
#define IOCTL_REQ_CAPABILITY	_IO('m', 0x04)
#define IOCTL_REQ_DATA_SIZE	_IO('m', 0x05)

enum misc_mode {
	DECOM_LZ4	= BIT(0),
	DECOM_GZIP	= BIT(1),
	DECOM_ZLIB	= BIT(2),
	OTP_S		= BIT(3),
	OTP_NS		= BIT(4),
};

/*
 * Read the device to buffer, optional.
 *
 * @dev: the device
 * @offset: offset to read the device
 * @buf: pointer to data buffer
 * @size: data size in bytes to read the device
 * @return: 0 if OK, -ve on error
 */
int misc_read(struct udevice *dev, int offset, void *buf, int size);
/*
 * Write buffer to the device, optional.
 *
 * @dev: the device
 * @offset: offset to write the device
 * @buf: pointer to data buffer
 * @size: data size in bytes to write the device
 * @return: 0 if OK, -ve on error
 */
int misc_write(struct udevice *dev, int offset, void *buf, int size);
/*
 * Assert command to the device, optional.
 *
 * @dev: the device
 * @request: command to be sent to the device
 * @buf: pointer to buffer related to the request
 * @return: 0 if OK, -ve on error
 */
int misc_ioctl(struct udevice *dev, unsigned long request, void *buf);

/*
 * Send a message to the device and wait for a response.
 *
 * The caller provides the message type/ID and payload to be sent.
 * The callee constructs any message header required, transmits it to the
 * target, waits for a response, checks any error code in the response,
 * strips any message header from the response, and returns the error code
 * (or a parsed version of it) and the response message payload.
 *
 * @dev: the device.
 * @msgid: the message ID/number to send.
 * tx_msg: the request/transmit message payload.
 * tx_size: the size of the buffer pointed at by tx_msg.
 * rx_msg: the buffer to receive the response message payload. May be NULL if
 *         the caller only cares about the error code.
 * rx_size: the size of the buffer pointed at by rx_msg.
 * @return the response message size if OK, -ve on error
 */
int misc_call(struct udevice *dev, int msgid, void *tx_msg, int tx_size,
	      void *rx_msg, int rx_size);

/*
 * Get a misc device by capability
 *
 * The caller can get a misc device according to capability request, the driver
 * must implement the IOCTL_REQ_CAPABILITY callback.
 *
 * @capability: the value of enum misc_mode.
 * @return the require device if OK, NULL on error
 */
struct udevice *misc_get_device_by_capability(u32 capability);

/*
 * struct misc_ops - Driver model Misc operations
 *
 * The uclass interface is implemented by all miscellaneous devices which
 * use driver model.
 */
struct misc_ops {
	/*
	 * Read the device to buffer, optional.
	 *
	 * @dev: the device
	 * @offset: offset to read the device
	 * @buf: pointer to data buffer
	 * @size: data size in bytes to read the device
	 * @return: 0 if OK, -ve on error
	 */
	int (*read)(struct udevice *dev, int offset, void *buf, int size);
	/*
	 * Write buffer to the device, optional.
	 *
	 * @dev: the device
	 * @offset: offset to write the device
	 * @buf: pointer to data buffer
	 * @size: data size in bytes to write the device
	 * @return: 0 if OK, -ve on error
	 */
	int (*write)(struct udevice *dev, int offset, const void *buf,
		     int size);
	/*
	 * Assert command to the device, optional.
	 *
	 * @dev: the device
	 * @request: command to be sent to the device
	 * @buf: pointer to buffer related to the request
	 * @return: 0 if OK, -ve on error
	 */
	int (*ioctl)(struct udevice *dev, unsigned long request, void *buf);
	/*
	 * Send a message to the device and wait for a response.
	 *
	 * @dev: the device
	 * @msgid: the message ID/number to send
	 * tx_msg: the request/transmit message payload
	 * tx_size: the size of the buffer pointed at by tx_msg
	 * rx_msg: the buffer to receive the response message payload. May be
	 *         NULL if the caller only cares about the error code.
	 * rx_size: the size of the buffer pointed at by rx_msg
	 * @return the response message size if OK, -ve on error
	 */
	int (*call)(struct udevice *dev, int msgid, void *tx_msg, int tx_size,
		    void *rx_msg, int rx_size);
};

/* generic layer for otp */
struct udevice *misc_otp_get_device(u32 capability);
int misc_otp_read(struct udevice *dev, int offset, void *buf, int size);
int misc_otp_write(struct udevice *dev, int offset, const void *buf, int size);

/* generic layer for decompress */
struct decom_param {
	unsigned long addr_src;
	unsigned long addr_dst;
	u64 size_src;	/* compressed */
	u64 size_dst;	/* decompressed, to be filled for output */
	enum misc_mode mode;
	u32 flags;
};

/* function flags for decompress */
#define DCOMP_FLG_IRQ_ONESHOT	BIT(0)

void misc_decompress_async(u8 comp);
void misc_decompress_sync(u8 comp);
int misc_decompress_cleanup(void);
int misc_decompress_process(unsigned long dst, unsigned long src,
			    unsigned long src_len, u32 cap, bool sync,
			    u64 *size, u32 flags);
#endif	/* _MISC_H_ */

/*
 * Copyright (c) 2015 Google, Inc
 * Written by Simon Glass <sjg@chromium.org>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __REGMAP_H
#define __REGMAP_H

/**
 * struct regmap_range - a register map range
 *
 * @start:	Start address
 * @size:	Size in bytes
 */
struct regmap_range {
	ulong start;
	ulong size;
};

/**
 * struct regmap - a way of accessing hardware/bus registers
 *
 * @base:	Base address of register map
 * @range_count: Number of ranges available within the map
 * @range:	Pointer to the list of ranges, allocated if @range_count > 1
 * @base_range:	If @range_count is <= 1, @range points here
 */
struct regmap {
	phys_addr_t base;
	int range_count;
	struct regmap_range *range, base_range;
};

/*
 * Interface to provide access to registers either through a direct memory
 * bus or through a peripheral bus like I2C, SPI.
 */
int regmap_write(struct regmap *map, uint offset, uint val);
int regmap_read(struct regmap *map, uint offset, uint *valp);

#define regmap_write32(map, ptr, member, val) \
	regmap_write(map, (uint32_t *)(ptr)->member - (uint32_t *)(ptr), val)

#define regmap_read32(map, ptr, member, valp) \
	regmap_read(map, (uint32_t *)(ptr)->member - (uint32_t *)(ptr), valp)

/**
 * regmap_read_poll_timeout - Poll until a condition is met or a timeout occurs
 *
 * @map:	Regmap to read from
 * @addr:	Offset to poll
 * @val:	Unsigned integer variable to read the value into
 * @cond:	Break condition (usually involving @val)
 * @sleep_us:	Maximum time to sleep between reads in us (0 tight-loops).
 * @timeout_ms:	Timeout in ms, 0 means never timeout
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout or the regmap_read
 * error return value in case of a error read. In the two former cases,
 * the last read value at @addr is stored in @val. Must not be called
 * from atomic context if sleep_us or timeout_us are used.
 *
 * This is modelled after the regmap_read_poll_timeout macros in linux but
 * with millisecond timeout.
 */
#define regmap_read_poll_timeout(map, addr, val, cond, sleep_us, timeout_ms) \
({ \
	unsigned long __start = get_timer(0); \
	int __ret; \
	for (;;) { \
		__ret = regmap_read((map), (addr), &(val)); \
		if (__ret) \
			break; \
		if (cond) \
			break; \
		if ((timeout_ms) && get_timer(__start) > (timeout_ms)) { \
			__ret = regmap_read((map), (addr), &(val)); \
			break; \
		} \
		if ((sleep_us)) \
			udelay((sleep_us)); \
	} \
	__ret ?: ((cond) ? 0 : -ETIMEDOUT); \
})

/**
 * regmap_update_bits() - Perform a read/modify/write using a mask
 *
 * @map:	The map returned by regmap_init_mem*()
 * @offset:	Offset of the memory
 * @mask:	Mask to apply to the read value
 * @val:	Value to apply to the value to write
 */
int regmap_update_bits(struct regmap *map, uint offset, uint mask, uint val);

/**
 * regmap_init_mem() - Set up a new register map that uses memory access
 *
 * Use regmap_uninit() to free it.
 *
 * @dev:	Device that uses this map
 * @mapp:	Returns allocated map
 */
int regmap_init_mem(struct udevice *dev, struct regmap **mapp);

/**
 * regmap_init_mem_platdata() - Set up a new memory register map for of-platdata
 *
 * This creates a new regmap with a list of regions passed in, rather than
 * using the device tree. It only supports 32-bit machines.
 *
 * Use regmap_uninit() to free it.
 *
 * @dev:	Device that uses this map
 * @reg:	List of address, size pairs
 * @count:	Number of pairs (e.g. 1 if the regmap has a single entry)
 * @mapp:	Returns allocated map
 */
int regmap_init_mem_platdata(struct udevice *dev, fdt_val_t *reg, int count,
			     struct regmap **mapp);

/**
 * regmap_get_range() - Obtain the base memory address of a regmap range
 *
 * @map:	Regmap to query
 * @range_num:	Range to look up
 */
void *regmap_get_range(struct regmap *map, unsigned int range_num);

/**
 * regmap_uninit() - free a previously inited regmap
 */
int regmap_uninit(struct regmap *map);

#endif

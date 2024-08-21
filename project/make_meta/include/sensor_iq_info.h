#ifndef __SENSOR_IQ_INFO_COMMON_H__
#define __SENSOR_IQ_INFO_COMMON_H__

#define SENSOR_IQ_INFO_UPPER_ALIGN(size, align_size)   (((size) + align_size - 1) & (~(align_size - 1)))
#define SENSOR_IQ_INFO_UPPER_ALIGN_16(size)   (SENSOR_IQ_INFO_UPPER_ALIGN(size, 16))
#define SENSOR_IQ_INFO_UPPER_ALIGN_64(size)   (SENSOR_IQ_INFO_UPPER_ALIGN(size, 64))

struct sensor_iq_info
{
	unsigned int tag;
	unsigned int main_sensor_iq_offset;
	unsigned int main_sensor_iq_size;
	unsigned int secondary_sensor_iq_offset;
	unsigned int secondary_sensor_iq_size;
	int len;
	/*
	 * Ensure data's offset align with 64 byte
	 *
	 * 4 = sizeof(unsigned int), sizeof(int)
	 * 7 = count of (tag + main_sensor_iq_offset + main_sensor_iq_size
	 *      + secondary_sensor_iq_offset + secondary_sensor_iq_size + len + crc32)
	 */
	uint8_t  reserved[SENSOR_IQ_INFO_UPPER_ALIGN_64(7*4)-7*4];
	unsigned int crc32;
	uint8_t data[0]; /* Just a way for get data */
};

#endif //__SENSOR_IQ_INFO_COMMON_H__

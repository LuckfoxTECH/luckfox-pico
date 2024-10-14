#ifndef __CALIB_DATA_H__
#define __CALIB_DATA_H__

#define CALIB_VERSION_OFFSET    (16)
#define PREISP_CALIB_MAGIC      "#SLM_CALIB_DATA#"
#define PREISP_CALIB_MAGIC_END  "SLM_CALIB_END"
#define PREISP_CALIB_ITEM_NUM   24

struct calib_item {
	unsigned char name[48];
	unsigned int  offset;
	unsigned int  size;
	unsigned int  temp;
	unsigned int  crc32;
};

struct calib_head {
	unsigned char magic[16];
	unsigned int  version;
	unsigned int  head_size;
	unsigned int  image_size;
	unsigned int  items_number;
	unsigned char reserved0[32];
	unsigned int  hash_len;
	unsigned char hash[32];
	unsigned char reserved1[28];
	unsigned int  sign_tag;
	unsigned int  sign_len;
	unsigned char rsa_hash[256];
	unsigned char reserved2[120];
	struct calib_item item[PREISP_CALIB_ITEM_NUM];
};


#ifdef __cplusplus
extern "C" {
#endif

void eeprom_calib_data_check(const char*calib_path, const char *dp_init_path);

#ifdef __cplusplus
}
#endif
#endif

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "zlib.h"
#include "sensor_define.h"
#include "sensor_2a_common.h"
#include "sensor_init_info.h"
#include "sensor_iq_info.h"
#include "rk_meta.h"
#include "rk_meta_app_param.h"
#include "rk_meta_wakeup_param.h"

#ifdef IS_NAND_FLASH
#include "mtd_utils_all.h"
#endif

static int write_item_to_file(int fd, uint32_t offset, uint8_t *from, int len);

__attribute__((weak)) int flash_write_buf(char *file_path, char *input_buf, size_t start, size_t len) {
	int fd = 0;

	fd = open(file_path, O_RDWR);
	if (fd < 0) {
		printf("Open meta file failed.\n");
		return -1;
	}

	if (write_item_to_file(fd, start, (uint8_t *)input_buf, len)) {
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

__attribute__((weak)) int flash_read_buf(char *file_path, char *output_buf, size_t start, size_t len) {
	int read_len = 0;
	int fd = 0;

	fd = open(file_path, O_RDONLY);
	if (fd < 0)	{
		printf("Open meta file failed.\n");
		return -1;
	}

	lseek(fd, start, SEEK_SET);

	read_len = read(fd, output_buf, len);
	if (read_len < 0) {
		printf("read meta failed\n");
		close(fd);
		return -1;
	}

	close(fd);
	return 0;
}

/*      META layout        */
/***************************/      /* ----- ITEM start -----*/
/**     META_HEAD         **/
/***************************/
/**     ITEM 1            **/
/***************************/
/**     ITEM 2            **/
/***************************/
/**     ITEM 3            **/
/***************************/
/**       ....            **/
/***************************/      /* --- ITEM backup start ---*/
/**     ITEM 1 BACKUP     **/
/***************************/
/**     ITEM 2 BACKUP     **/
/***************************/
/**     ITEM 3 BACKUP     **/
/***************************/
/**       ....            **/
/***************************/
/**     META_END          **/
/***************************/      /* ----- ITEM backup end -----*/
/**     IQ FILE BIN       **/       /* 320K sensor iq file bin */
/***************************/

#define	META_LOAD_ADDR	"meta_load_addr"
#define	META_LOAD_SIZE	"meta_part_size"
#define	META_PARAM_UPDATE "--update"
#define	META_PARAM_META_PATH "--meta_path"
#define	META_TRUE (1)
#define	META_FALSE (0)
#define	_META_STRNCPY(x,y) { \
	strncpy(x, y, PATH_MAX); \
	x[PATH_MAX - 1] = '\0'; \
}

static char g_create = META_FALSE;
static char g_update = META_FALSE;
static char g_dump = META_FALSE;
static char g_recover = META_FALSE;
static char g_crc32_check = META_FALSE;
static char g_meta_path[PATH_MAX] = { 0 };
static char g_cmdline[MAX_CMDLINE_LENGTH] = { 0 };
static char g_sensor_init_path[PATH_MAX] = { 0 };
static char g_secondary_sensor_init_path[PATH_MAX] = { 0 };
static char g_ae_awb_table[PATH_MAX] = { 0 };
static char g_sensor_iq_bin[PATH_MAX] = { 0 };
static char g_secondary_sensor_iq_bin[PATH_MAX] = { 0 };
static uint32_t g_meta_load_addr = 0x800000; /* DO NOT MODIFY */
static uint32_t g_meta_part_size = 384*1024; /* DO NOT MODIFY */
static uint32_t g_meta_compress_size = 0;
static uint32_t g_meta_compress_flag = META_COMPRESS_TYPE_GZ;
static uint8_t *g_meta_buf = NULL;

static uint32_t g_rk_cam_w = 0;
static uint32_t g_rk_cam_h = 0;
static uint32_t g_rk_venc_w = 0;
static uint32_t g_rk_venc_h = 0;
static uint32_t g_rk_venc_type = RK_VENC_TYPE_INVALID;
static uint32_t g_rk_venc_bitrate = 0;
static uint32_t g_rk_cam2_w = 0;
static uint32_t g_rk_cam2_h = 0;
static uint32_t g_rk_venc2_w = 0;
static uint32_t g_rk_venc2_h = 0;
static uint32_t g_rk_venc2_type = RK_VENC_TYPE_INVALID;
static uint32_t g_rk_venc2_bitrate = 0;
static uint32_t g_rk_cam_mirror_flip = 0;
static uint32_t g_rk_cam2_mirror_flip = 0;
static uint32_t g_rk_cam_fps = 25;
static uint32_t g_rk_cam2_fps = 25;
static uint32_t g_rk_cam_hdr = 0;
static uint32_t g_rk_cam2_hdr = 0;
static uint32_t g_rk_cam_index = 0;
static uint32_t g_rk_cam_num = 1;
static uint32_t g_rk_fastae_max_frame = 0;
static uint32_t g_rk_led_value = 20;
static uint32_t g_rk_color_mode = 0;
static uint32_t g_rk_night_mode = 0;

static uint32_t g_rk_iqbin_secondary_size = 0;
static uint32_t g_rk_iqbin_main_size = 0;
static uint32_t g_rk_iqbin_secondary_offset = 0;
static uint32_t g_rk_iqbin_main_offset = 0;

static int g_app_param_need = META_FALSE;
static int g_cmdline_need = META_FALSE;
static uint8_t g_cmdline_need_clean = META_FALSE;
static uint8_t g_is_nand_flash = META_FALSE;

static int g_meta_startup_part_num = 0;
static uint32_t g_meta_startup_part_offset = 0;
static uint32_t g_meta_per_part_size = 0;
/* multiple part parameter */
#ifdef IS_NAND_FLASH
static uint32_t g_meta_total_part_num = 2; /* Modify the number of parts. */
static uint32_t g_meta_part_flag = 1; /* 0 or 1 */
static uint32_t g_meta_part_reserved_size = 256 * 1024; /* Modify reserved size. */
#else
static uint32_t g_meta_total_part_num = 1;
static uint32_t g_meta_part_flag = 0;
static uint32_t g_meta_part_reserved_size = 0;
#endif

static const char g_sensor_init_head[4] = {'S', 'N', 'I', 'F'};
static const char g_cmdline_head[4] = {'C', 'M', 'D', 'L'};
static const char g_ae_awb_table_head[4] = {'A', 'E', 'A', 'W'};
static const char g_sensor_iq_head[4] = {'S', 'N', 'I', 'Q'};
static const char g_app_param_head[4] = {'A', 'P', 'R', 'A'};
static const char g_wakeup_param_head[4] = {'W', 'K', 'U', 'P'};

static uint8_t g_set_als_type_none = META_FALSE;

/* ========================================================================= */

#if 0
static int dump_hex(char *buf_dump_addr, const char *msg)
{
	printf("\ndump [%s]\n", msg);
	char *pdata = (char *)(buf_dump_addr);
	for (char j=0; j<64; j++ ) {
		printf("%02x ", *(pdata+j) & 0xff);
		if ((j+1)%16 == 0) {
			printf("\n");
		}
	}
	printf("\n");
	return 0;
}
#endif

static int file_create_ok(const char *path)
{
	int fd = open(path, O_CREAT | O_TRUNC | O_RDWR, 0644);
	if (fd < 0) {
		printf("%s can't be create: %s\n", path, strerror(errno));
		return -1;
	}

	close(fd);

	unlink(path);
	return 0;
}

static int write_item_to_buffer(uint8_t *to, uint8_t *from, int len)
{
	memset((void *)to, 0xff, ITEM_SIZE);
	memcpy((void *)to, from, len);
	return 0;
}

static int write_item_to_file(int fd, uint32_t offset, uint8_t *from, int len)
{
	int ret;

	lseek(fd, offset, SEEK_SET);

	ret = write(fd, from, len);
	if (ret != len) {
		printf("write meta failed\n");
		return -1;
	}

	return 0;
}

/* #define	DEBUG_DUMP_GZ_COMP_DATA */
#ifdef DEBUG_DUMP_GZ_COMP_DATA
static int dump_gz_comp_data(const char *file_name, uint8_t *buf_dump, int buf_size)
{
	int fd_dump = open(file_name, O_RDWR|O_TRUNC|O_CREAT, 0644);
	if (fd_dump < 0) {
		printf ( "open file [%s] error\n", file_name);
		return 1;
	}
	write_item_to_file(fd_dump, 0, buf_dump, buf_size);
	close(fd_dump);
	return 0;
}
#endif

/* Uncompress gzip data */
static int uncompress_gzip(uint8_t *src, unsigned int src_size,
        uint8_t *dst, unsigned int *dst_size)
{
	int ret = 0;
	z_stream d_obj = {0};

	printf ( "info: meta start uncompress\n" );

	d_obj.zalloc = (alloc_func)0;
	d_obj.zfree = (free_func)0;
	d_obj.opaque = (voidpf)0;
	d_obj.next_in = (Bytef *)src;
	d_obj.avail_in = Z_NULL;
	d_obj.next_out = (void *)dst;
	if(inflateInit2(&d_obj, 15 + 16) != Z_OK) {
		printf ( "error: uncompress init\n" );
		return -1;
	}

	while (d_obj.total_out < *dst_size
			&& d_obj.total_in < src_size) {
		d_obj.avail_in = 1;
		d_obj.avail_out = 1;

		if ((ret = inflate(&d_obj, Z_NO_FLUSH)) == Z_STREAM_END) break;
		if (ret != Z_OK ) {
			printf ( "error: uncompress inflate\n");
			return -1;
		}
	}
	if (inflateEnd(&d_obj) != Z_OK) {
		printf ( "error: uncompress end\n");
		return -1;
	}
	*dst_size = d_obj.total_out;
	return 0;
}

/* Compress gzip data */
static int compress_gzip(const uint8_t *src, uint32_t src_size,
		Bytef *dst, int *dst_size)
{
	z_stream z_obj;
	int ret = 0;

	/* printf ( "info: meta start compress iq\n" ); */
	if (src && src_size > 0) {
		z_obj.zalloc = (alloc_func)0;
		z_obj.zfree = (free_func)0;
		z_obj.opaque = (voidpf)0;
		if (deflateInit2(&z_obj,
					Z_BEST_COMPRESSION,
					Z_DEFLATED,
					15 + 16, 8,
					Z_DEFAULT_STRATEGY) != Z_OK) {
			printf ( "error: compress init\n" );
			return -1;
		}
		z_obj.next_in = (void *)src;
		z_obj.avail_in = src_size;
		z_obj.next_out = (Bytef *)dst;
		z_obj.avail_out = *dst_size;
		while (z_obj.avail_in != 0
				&& z_obj.total_out < *dst_size) {
			if (deflate(&z_obj, Z_NO_FLUSH) != Z_OK) {
				printf ( "error: compress deflate error\n" );
				return -1;
			}
		}

		if (z_obj.avail_in != 0) {
			printf("compress error\n" );
			return z_obj.avail_in;
		}

		while (1) {
			if ((ret = deflate(&z_obj, Z_FINISH)) == Z_STREAM_END) {
				 break;
			}
			if (ret != Z_OK) {
				printf( "error: compress deflate Z_FINISH error\n" );
				return -1;
			}
		}
		if (deflateEnd(&z_obj) != Z_OK) {
			printf ( "error: compress end\n" );
			return -1;
		}
		*dst_size = z_obj.total_out;
		return 0;
	}
	return -1;
}

static int _get_file_size(const char* filename, int file_max_size)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed: %s\n", filename, strerror(errno));
		return EXIT_FAILURE;
	}

	int len_buffer = lseek(fd, 0, SEEK_END);
	if (len_buffer < 0) {
		printf("lseek %s failed: %s\n", filename, strerror(errno));
		goto out;
	} else if (len_buffer > file_max_size) {
		printf("length %d is too large, %d limit\n",
			   len_buffer,
			   file_max_size);
		goto out;
	}
out:
	close(fd);
	return len_buffer;
}

static uint32_t _get_meta_size(const char* filename)
{
	int fd = -1;
	int item_len = sizeof(struct meta_head);
	char item_buf[sizeof(struct meta_head)];

	memset(item_buf, 0xff, item_len);
	fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open %s fail: %s\n", filename, strerror(errno));
		return -1;
	}
	if (read(fd, item_buf, item_len) < 0) {
		printf("read meta file failed:%s\n", strerror(errno));
		close(fd);
		return -1;
	}
	close(fd);
	struct meta_head *meta_head_p = (struct meta_head *)item_buf;

	return meta_head_p->size;
}

static int _get_startup_part_num(const char* filename) {
	int ret = 0;
	uint32_t i = 1;
	unsigned int meta_buf_size = META_INFO_SIZE;

	/* Todo for emmc and spi_nor */
	if (g_is_nand_flash != META_TRUE) {
		return 1;
	}

	if (access(filename, R_OK) != 0) {
		printf("%s:%d can't be read\n", filename, __LINE__ );
		return -1;
	}

	uint8_t *meta_buf = (uint8_t *)malloc(meta_buf_size);
	if (meta_buf == NULL) {
		printf("malloc meta buf failed:%s\n", strerror(errno));
		return -1;
	}

	ret = flash_read_buf((char *)filename, (char *)meta_buf, 0, meta_buf_size);
	if (ret) {
		printf("read meta file failed: %d\n", ret);
		free(meta_buf);
		return -1;
	}

	struct meta_head *meta_head_p = (struct meta_head *)(meta_buf + META_INFO_HEAD_OFFSET);
	uint32_t nand_part_flag = meta_head_p->part_flag;
	g_meta_per_part_size = meta_head_p->size + meta_head_p->part_reserved_size;

	for (i = 1; i < g_meta_total_part_num; i++) {
		ret = flash_read_buf((char *)filename, (char *)meta_buf, i * g_meta_per_part_size, meta_buf_size);
		if (ret) {
			printf("read meta file failed: %d\n", ret);
			free(meta_buf);
			return -1;
		}

		meta_head_p = (struct meta_head *)(meta_buf + META_INFO_HEAD_OFFSET);
		if (meta_head_p->tag != RK_META) {
			printf("Not read meta tag from meta part[%d], think that part[%d]:part_flag = 0\n", i, i);
			meta_head_p->part_flag = 0;
		}

		printf("Check part[%d]:part_flag=%d  part[%d]:part_flag=%d\n", i - 1, nand_part_flag, i, meta_head_p->part_flag);
		if (nand_part_flag != meta_head_p->part_flag) {
			free(meta_buf);
			return i;
		}
	}

	free(meta_buf);
	return i;
}

static int read_cmdline_to_buf(void *buf, int len)
{
	int fd;
	int ret;
	if (buf == NULL || len < 0) {
		printf("%s: illegal para\n", __func__);
		return -1;
	}
	memset(buf, 0, len);
	fd = open("/proc/cmdline", O_RDONLY);
	if (fd < 0) {
		perror("open:");
		return -1;
	}
	ret = read(fd, buf, len);
	close(fd);
	return ret;
}

static int check_str_cmd(const char *string)
{
	char *addr;
	int ret = META_FALSE;
	char cmdline[1024];

	memset(cmdline, 0, sizeof(cmdline));
	read_cmdline_to_buf(cmdline, sizeof(cmdline));

	addr = strstr(cmdline, string);
	if (addr) {
		ret = META_TRUE;
		printf("found %s\n", string);
	}
	return ret;
}

int init_secondary_sensor_init_buf(uint8_t *buf)
{
	int ret = -1;
	int fd = open(g_secondary_sensor_init_path, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed: %s\n", g_secondary_sensor_init_path, strerror(errno));
		goto out;
	}

	int len_buffer = lseek(fd, 0, SEEK_END);
	if (len_buffer < 0) {
		printf("[%s] lseek %s failed: %s\n", __func__, g_secondary_sensor_init_path, strerror(errno));
		goto out;
	} else if (len_buffer == 0) {
		printf("[%s] length of %s is 0\n", __func__, g_secondary_sensor_init_path);
		goto out;
	} else if (len_buffer > SENSOR_INIT_MAX_SIZE) {
		printf("[%s] length %d is too large, %d limit\n",
				__func__,
			   len_buffer,
			   (int)SENSOR_INIT_MAX_SIZE);
		goto out;
	}

	lseek(fd, 0, SEEK_SET);
	len_buffer = read(fd, (void *)buf, SENSOR_INIT_MAX_SIZE);
	if (len_buffer < 0) {
		printf("read error\n");
		goto out;
	}

	struct meta_item *sensor_init = (struct meta_item *)buf;
	char *head_c = (char *)&sensor_init->tag;
	head_c[0] = 'S';
	head_c[1] = 'N';
	head_c[2] = 'I';
	head_c[3] = 'F';

	struct sensor_init_cfg *cam_cfg = (struct sensor_init_cfg *)buf;
	g_rk_cam2_h = cam_cfg->cam_h;
	g_rk_cam2_w = cam_cfg->cam_w;

	sensor_init->len = len_buffer;
	sensor_init->crc32 = crc32(0, sensor_init->data, sensor_init->len);

	ret = 0;
out:
	close(fd);
	return ret;
}

int init_sensor_init_buf(uint8_t *buf)
{
	int ret = -1;
	int fd = open(g_sensor_init_path, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed: %s\n", g_sensor_init_path, strerror(errno));
		goto out;
	}

	int len_buffer = lseek(fd, 0, SEEK_END);
	if (len_buffer < 0) {
		printf("[%s] lseek %s failed: %s\n", __func__, g_sensor_init_path, strerror(errno));
		goto out;
	} else if (len_buffer == 0) {
		printf("[%s] length of %s is 0\n", __func__, g_sensor_init_path);
		goto out;
	} else if (len_buffer > SENSOR_INIT_MAX_SIZE) {
		printf("[%s] length %d is too large, %d limit\n",
				__func__,
			   len_buffer,
			   (int)SENSOR_INIT_MAX_SIZE);
		goto out;
	}

	lseek(fd, 0, SEEK_SET);
	len_buffer = read(fd, (void *)buf, SENSOR_INIT_MAX_SIZE);
	if (len_buffer < 0) {
		printf("read error\n");
		goto out;
	}

	struct meta_item *sensor_init = (struct meta_item *)buf;
	char *head_c = (char *)&sensor_init->tag;
	head_c[0] = 'S';
	head_c[1] = 'N';
	head_c[2] = 'I';
	head_c[3] = 'F';

	struct sensor_init_cfg *cam_cfg = (struct sensor_init_cfg *)buf;
	g_rk_cam_h = cam_cfg->cam_h;
	g_rk_cam_w = cam_cfg->cam_w;

	sensor_init->len = len_buffer;
	sensor_init->crc32 = crc32(0, sensor_init->data, sensor_init->len);

	ret = 0;
out:
	close(fd);
	return ret;
}

int init_2a_table_buf(uint8_t *buf)
{
	int ret = -1;
	int fd = open(g_ae_awb_table, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed: %s\n", g_ae_awb_table, strerror(errno));
		goto out;
	}

	int len_buffer = lseek(fd, 0, SEEK_END);
	if (len_buffer < 0) {
		printf("[%s] lseek %s failed: %s\n", __func__, g_ae_awb_table, strerror(errno));
		goto out;
	} else if (len_buffer == 0) {
		printf("[%s] length of %s is 0\n", __func__, g_ae_awb_table);
		goto out;
	} else if (len_buffer > AE_TABLE_MAX_SIZE) {
		printf("[%s] length %d is too large, %d limit\n",
				__func__,
			   len_buffer,
			   (int)AE_TABLE_MAX_SIZE);
		goto out;
	}

	lseek(fd, 0, SEEK_SET);
	len_buffer = read(fd, (void *)buf, AE_TABLE_MAX_SIZE);
	if (len_buffer < 0) {
		printf("read error\n");
		goto out;
	}

	struct meta_item *ae_awb_tab_info = (struct meta_item *)buf;
	char *head_c = (char *)&ae_awb_tab_info->tag;
	head_c[0] = 'A';
	head_c[1] = 'E';
	head_c[2] = 'A';
	head_c[3] = 'W';
	ae_awb_tab_info->len = len_buffer;

	ae_awb_tab_info->crc32 = crc32(0, ae_awb_tab_info->data, ae_awb_tab_info->len);

	ret = 0;
out:
	close(fd);
	return ret;
}

int init_sensor_iqbin(uint8_t *meta_iq_buf, uint32_t iqbin_max_size)
{
	int ret = -1;
	int fd = -1;

	uint8_t *buf = (uint8_t *)malloc(iqbin_max_size);
	if (buf == NULL) {
		printf("malloc buf failed:%s\n", strerror(errno));
		goto out;
	}

	memcpy((void *)(buf), meta_iq_buf, iqbin_max_size);
	memset((void *)(meta_iq_buf), 0xff, iqbin_max_size);

	if (access(g_sensor_iq_bin, R_OK) == 0) {
		fd = open(g_sensor_iq_bin, O_RDONLY);
		if (fd < 0) {
			printf("open main sensor [%s] failed: %s\n", g_sensor_iq_bin, strerror(errno));
			goto out;
		}

		uint8_t *main_sensor_addr = (uint8_t *)(buf + sizeof(struct sensor_iq_info));
		if (g_update) {
			memset((void *)(main_sensor_addr),
					0xFF,
					SENSOR_IQ_BIN_MAX_SIZE - sizeof(struct sensor_iq_info));
		}
		ret = read(fd, (void *)(main_sensor_addr), g_rk_iqbin_main_size);
		if (ret < 0) {
			printf("read error\n");
			close(fd);
			goto out;
		}
		close(fd);
	}

	if (g_rk_cam_num == 2 && (access(g_secondary_sensor_iq_bin, R_OK) == 0)) {
		int fd_2 = open(g_secondary_sensor_iq_bin, O_RDONLY);
		if (fd_2 < 0) {
			printf("open secondary sensor [%s] failed: %s\n",
					g_secondary_sensor_iq_bin, strerror(errno));
			ret = 1;
			goto out;
		}
		uint8_t *secondary_sensor_addr = (uint8_t *)(buf + SENSOR_IQ_BIN_MAX_SIZE);

		if (g_update) {
			memset((void *)(secondary_sensor_addr),
					0xFF,
					SENSOR_IQ_BIN_MAX_SIZE);
		}

		ret = read(fd_2, (void *)(secondary_sensor_addr), g_rk_iqbin_secondary_size);
		if (ret < 0) {
			printf("read error\n");
			close(fd_2);
			goto out;
		}
		close(fd_2);
	}

	struct sensor_iq_info *iqinfo = (struct sensor_iq_info *)meta_iq_buf;
	char *head_c = (char *)&iqinfo->tag;
	head_c[0] = 'S';
	head_c[1] = 'N';
	head_c[2] = 'I';
	head_c[3] = 'Q';
	iqinfo->main_sensor_iq_offset = g_rk_iqbin_main_offset;
	iqinfo->main_sensor_iq_size = g_rk_iqbin_main_size;
	iqinfo->secondary_sensor_iq_offset = g_rk_iqbin_secondary_offset;
	iqinfo->secondary_sensor_iq_size = g_rk_iqbin_secondary_size;
	iqinfo->len = iqbin_max_size;

#ifdef DEBUG_DUMP_GZ_COMP_DATA
	dump_gz_comp_data("dat1.bin", (buf + sizeof(struct sensor_iq_info)), g_rk_iqbin_main_size);
	if (g_rk_cam_num == 2 && (access(g_secondary_sensor_iq_bin, R_OK) == 0)) {
		dump_gz_comp_data("dat2.bin", (buf + SENSOR_IQ_BIN_MAX_SIZE), g_rk_iqbin_secondary_size);
	}
#endif

	printf ( "info: crc32 main iq       [%#lx]\n", crc32(0, (buf + sizeof(struct sensor_iq_info)), g_rk_iqbin_main_size));
	if (g_rk_cam_num == 2 && (access(g_secondary_sensor_iq_bin, R_OK) == 0)) {
		printf ( "info: crc32 secondary iq  [%#lx]\n", crc32(0, (buf + SENSOR_IQ_BIN_MAX_SIZE), g_rk_iqbin_secondary_size));
	}

	if (g_meta_compress_flag == META_COMPRESS_TYPE_GZ) {
		ret = compress_gzip(
				(buf + sizeof(struct sensor_iq_info)),
				iqbin_max_size - sizeof(struct sensor_iq_info),
				(uint8_t *)iqinfo->data,
				&iqinfo->len);
		if (ret != 0) {
			printf("gzip compress iq bin fail.\n");
			goto out;
		}
	} else {
		memcpy(iqinfo->data, (buf + sizeof(struct sensor_iq_info)), iqbin_max_size - sizeof(struct sensor_iq_info));
	}

	g_meta_compress_size = iqinfo->len;
	iqinfo->crc32 = crc32(0, iqinfo->data, iqinfo->len);

	ret = 0;
out:
	if (buf) {
		free(buf);
		buf = NULL;
	}
	return ret;
}

static int dump_sensor_init(struct sensor_init_cfg *sensor_init, const char *sensor_init_name)
{
	uint32_t *sensor_init_head_p = (uint32_t *)g_sensor_init_head;
	if (sensor_init->head != *sensor_init_head_p) {
		printf("Invalid [%s] sensor init config\n", sensor_init_name);
	} else {
		printf("[%s] sensor init config : \n", sensor_init_name);
		printf("  length                       : 0x%08x\n", sensor_init->len);
		printf("  crc                          : 0x%08x\n", sensor_init->crc32);
		printf("  cam_w                        : %d\n", sensor_init->cam_w);
		printf("  cam_h                        : %d\n", sensor_init->cam_h);
		printf("  als_type                     : %d\n", sensor_init->als_type);
		printf("  als_value                    : %d\n", sensor_init->als_value);
		printf("  ircut_a.gpio_index           : %d\n", sensor_init->ircut_a.gpio_index);
		printf("  ircut_a.gpio_func            : 0x%08x\n", sensor_init->ircut_a.gpio_func);
		printf("  ircut_b.gpio_index           : %d\n", sensor_init->ircut_b.gpio_index);
		printf("  ircut_b.gpio_func            : 0x%08x\n", sensor_init->ircut_b.gpio_func);

		printf("  led_ir_enable.gpio_index     : %d\n", sensor_init->led_ir_enable.gpio_index);
		printf("  led_ir_enable.gpio_func      : 0x%08x\n", sensor_init->led_ir_enable.gpio_func);
		printf("  led_ir.gpio_index            : %d\n", sensor_init->led_ir.gpio_index);
		printf("  led_ir.gpio_func             : 0x%08x\n", sensor_init->led_ir.gpio_func);
		printf("  led_ir.pwm_channel           : %d\n", sensor_init->led_ir.pwm_channel);
		printf("  led_ir.pwm_period            : %d\n", sensor_init->led_ir.pwm_period);
		printf("  led_ir.pwm_pulse             : %d\n", sensor_init->led_ir.pwm_pulse);

		printf("  led_white_enable.gpio_index  : %d\n", sensor_init->led_white_enable.gpio_index);
		printf("  led_white_enable.gpio_func   : 0x%08x\n", sensor_init->led_white_enable.gpio_func);
		printf("  led_white.gpio_index         : %d\n", sensor_init->led_white.gpio_index);
		printf("  led_white.gpio_func          : 0x%08x\n", sensor_init->led_white.gpio_func);
		printf("  led_white.pwm_channel        : %d\n", sensor_init->led_white.pwm_channel);
		printf("  led_white.pwm_period         : %d\n", sensor_init->led_white.pwm_period);
		printf("  led_white.pwm_pulse          : %d\n", sensor_init->led_white.pwm_pulse);

		printf("  gpio_adc_enable.gpio_index   : %d\n", sensor_init->gpio_adc_enable.gpio_index);
		printf("  gpio_adc_enable.gpio_func    : 0x%08x\n", sensor_init->gpio_adc_enable.gpio_func);

		printf("  gpio_adc.gpio_index          : %d\n", sensor_init->gpio_adc.gpio_index);
		printf("  gpio_adc.gpio_func           : 0x%08x\n", sensor_init->gpio_adc.gpio_func);
		printf("  gpio_adc.adc_channel         : 0x%08x\n", sensor_init->gpio_adc.adc_channel);
	}
	printf ( "\n" );

	return 0;
}

static int dump_rtt_log (uint8_t flag)
{
#define RTT_LOG_PARAM "/proc/device-tree/reserved-memory/ramoops@rtos_log/reg"
#define MAP_MASK (sysconf(_SC_PAGE_SIZE) - 1)
#define uswap_32(x) \
       ((((x) & 0xff000000) >> 24) | \
        (((x) & 0x00ff0000) >>  8) | \
        (((x) & 0x0000ff00) <<  8) | \
        (((x) & 0x000000ff) << 24))

	void *RttLogVirMem = NULL;
	int mem_fd = -1, log_param_fd = -1;
	struct LogParam {
		unsigned int LogAddr;
		unsigned int LogSize;
	};

	struct LogParam log_param = { 0, 0};

	if ((log_param_fd = open(RTT_LOG_PARAM, O_RDONLY)) < 0) {
		printf("cannot open [%s]\n", RTT_LOG_PARAM);
		return -1;
	}

	if (read(log_param_fd, (void *)&log_param, sizeof(log_param)) == -1) {
		printf ( "read log param error.\n" );
		if (log_param_fd != -1)
			close(log_param_fd);
		return -1;
	}
	if (log_param_fd != -1)
		close(log_param_fd);
	log_param.LogAddr  = uswap_32(log_param.LogAddr);
	log_param.LogSize  = uswap_32(log_param.LogSize);
	printf ( "rtt log addr [%#x]\n", log_param.LogAddr );
	printf ( "rtt log size [%#x]\n", log_param.LogSize );

	if ((mem_fd = open("/dev/mem", O_RDWR )) < 0) {
		printf("cannot open /dev/mem.\n");
		return -1;
	}

	RttLogVirMem = mmap(NULL, log_param.LogSize, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd,
			log_param.LogAddr & ~(MAP_MASK));
	if (RttLogVirMem != MAP_FAILED) {
		char *p = (char *)RttLogVirMem;
		for ( ; p < (log_param.LogSize + (char*)RttLogVirMem); p++ ) {
			if (flag == META_TRUE) {
				printf ( "%c", *p );
			} else {
				*p = 0;
			}
		}
	} else {
		printf ( "map rtt log mem error [%s]\n", strerror(errno) );
	}
	if (RttLogVirMem != MAP_FAILED)
		munmap(RttLogVirMem, log_param.LogSize);
	if (mem_fd != -1)
		close(mem_fd);

	return 0;
}		/* -----  end of static function dump_rtt_log  ----- */

int dump_meta(const char *path)
{
	int ret = 0;
	unsigned int buf_size = MAX_META_SEGMENT_SIZE + ITEM_SIZE;

	uint8_t *buf = (uint8_t *)malloc(buf_size);
	if (buf == NULL) {
		printf("malloc dump buf failed:%s\n", strerror(errno));
		return -1;
	}

	if (g_is_nand_flash == META_TRUE) {
		printf("\n[dump meta]: Found nand flash. Current part number: %d, total: %d\n", g_meta_startup_part_num, g_meta_total_part_num);
		ret = flash_read_buf((char *)path, (char *)buf, g_meta_startup_part_offset, buf_size);
		if (ret) {
			printf("read meta file failed: %d\n", ret);
			free(buf);
			return -1;
		}
	} else {
		int fd = open(path, O_RDONLY);
		if (fd < 0)
		{
			free(buf);
			return -1;
		}

		printf("dump meta:\n");

		int len = read(fd, buf, buf_size);
		if (len == 0) {
			printf("empty file\n");
			free(buf);
			close(fd);
			return -1;
		}

		close(fd);
	}

	if (*(uint32_t *)buf != RK_META) {
		printf("Invalid meta head\n");
		free(buf);
		return -1;
	}

	if (*(uint32_t *)(buf + MAX_META_SEGMENT_SIZE - 4) != RK_META_END) {
		printf("Invalid meta end\n");
		free(buf);
		return -1;
	}

	struct meta_head *meta_head_p = (struct meta_head *)(buf + META_INFO_HEAD_OFFSET);
	printf ( "\nmeta head info:\n");
	printf("  load addr            : 0x%08x\n", meta_head_p->load);
	printf("  size                 : 0x%08x\n", meta_head_p->size);
	printf("  comp_type            : %d\n", meta_head_p->comp_type);
	printf("  comp_size            : 0x%08x\n", meta_head_p->comp_size);
	printf("  comp_off             : 0x%08x\n", meta_head_p->comp_off);
	printf("  iq_item_size         : 0x%08x\n", meta_head_p->iq_item_size);
	printf("  total_part_num       : %d\n", meta_head_p->total_part_num);
	printf("  part_flag            : %d\n", meta_head_p->part_flag);
	printf("  part_reserved_size   : 0x%08x\n", meta_head_p->part_reserved_size);
	printf("  crc32                : 0x%08x\n", meta_head_p->crc32);
	printf("  meta_flags           : 0x%08x\n", meta_head_p->meta_flags);
	printf ( "\n" );

	struct cmdline_info *cmdline_p = (struct cmdline_info *)(buf + CMDLINE_OFFSET);
	uint32_t *cmdline_head_p = (uint32_t *)&g_cmdline_head;
	if (cmdline_p->tag != *cmdline_head_p) {
		printf("Invalid cmdline\n");
	} else {
		printf("cmdline      : %s\n", cmdline_p->data);
		printf("cmdline crc32: 0x%08x\n", cmdline_p->crc32);
	}
	printf ( "\n" );

	dump_sensor_init((struct sensor_init_cfg *)(buf + SENSOR_INIT_OFFSET), "main");
	dump_sensor_init((struct sensor_init_cfg *)(buf + SECONDARY_SENSOR_INIT_OFFSET), "secondary");

	struct ae_awb_init_cfg *ae_awb = (struct ae_awb_init_cfg *)(buf + AE_TABLE_OFFSET);
	uint32_t *ae_awb_head_p = (uint32_t *)g_ae_awb_table_head;
	if (ae_awb->head != *ae_awb_head_p) {
		printf("Invalid sensor ae awb config\n");
	} else {
		printf("Sensor ae awb config : \n");
		printf("  offset             : 0x%08x\n", AE_TABLE_OFFSET);
		printf("  length             : 0x%08x\n", ae_awb->len);
		printf("  crc                : 0x%08x\n", ae_awb->crc32);

		printf("  black_lvl          : 0x%04x\n", ae_awb->ae.black_lvl);
		printf("  adc calib type     : 0x%04x\n", ae_awb->ae.adc_calib_type);
		printf("  adc range type     : 0x%04x\n", ae_awb->ae.adc_range_type);
		printf("  adc_calib          : 0x%04x 0x%04x 0x%04x 0x%04x\n", ae_awb->ae.adc_calib[0],ae_awb->ae.adc_calib[1],ae_awb->ae.adc_calib[2],ae_awb->ae.adc_calib[3]);
		printf("  start_exp          : 0x%04x 0x%04x 0x%04x\n", ae_awb->ae.start_exp[0],ae_awb->ae.start_exp[1],ae_awb->ae.start_exp[2]);
		printf("  rg_gain_base       : 0x%08x\n", ae_awb->awb.rg_gain_base);
		printf("  bg_gain_base       : 0x%08x\n", ae_awb->awb.bg_gain_base);
		printf("  awb_gain_rad       : 0x%08x\n", ae_awb->awb.awb_gain_rad);
		printf("  awb_gain_dis       : 0x%08x\n", ae_awb->awb.awb_gain_dis);

	}
	printf ( "\n" );

	struct app_param_info *app = (struct app_param_info *)(buf + APP_PARAM_OFFSET);
	uint32_t *app_head_p = (uint32_t *)g_app_param_head;
	if (app->head != *app_head_p) {
		printf("Invalid app param config\n");
	} else {
		printf("App parameter config : \n");
		printf("  offset             : 0x%08x\n", APP_PARAM_OFFSET);
		printf("  length             : 0x%08x\n", app->len);
		printf("  crc                : 0x%08x\n", app->crc32);
		printf("  %s : %d\n",RK_CAM_MIRROR_FLIP , app->cam_mirror_flip);
		printf("  %s : %d\n",RK_CAM2_MIRROR_FLIP , app->cam2_mirror_flip);
		printf("  %s         : %d\n",RK_CAM_FPS, app->cam_fps);
		printf("  %s         : %d\n",RK_CAM2_FPS, app->cam2_fps);
		printf("  %s         : %d\n",RK_CAM_HDR, app->cam_hdr);
		printf("  %s         : %d\n",RK_CAM2_HDR, app->cam2_hdr);
		printf("  %s       : %d\n",RK_CAM_INDEX, app->cam_index);
		printf("  %s       : %d\n",RK_LED_VALUE       , app->led_value);
		printf("  %s      : %d\n",RK_NIGHT_MODE      , app->night_mode);
		printf("  %s      : %d\n",RK_FULLCOLOR_GRAY_STRINGS, app->color_mode);
		printf("  %s          : %d\n",RK_VENC_W , app->venc_w);
		printf("  %s          : %d\n",RK_VENC_H , app->venc_h);
		printf("  %s       : %d\n",RK_VENC_TYPE , app->venc_type);
		printf("  %s    : %d\n",RK_VENC_BITRATE , app->venc_bitrate);
		printf("  %s         : %d\n",RK_VENC2_W , app->venc2_w);
		printf("  %s         : %d\n",RK_VENC2_H , app->venc2_h);
		printf("  %s      : %d\n",RK_VENC2_TYPE , app->venc2_type);
		printf("  %s   : %d\n",RK_VENC2_BITRATE , app->venc2_bitrate);
		printf("  %s       : %d\n",RK_FASTAE_MAX_FRAME, app->fastae_max_frame);
		printf("  %s         : %d\n",RK_CAM_SENSOR_NUM, app->cam_num);
	}
	printf ( "\n" );

	struct sensor_iq_info *iq_info = (struct sensor_iq_info *)(buf + SENSOR_IQ_BIN_OFFSET);
	uint32_t *iq_head_p = (uint32_t *)g_sensor_iq_head;
	if (iq_info->tag != *iq_head_p) {
		printf("Invalid sensor iq bin config\n");
	} else {
		printf("Sensor iq bin config : \n");
		printf("  length             : 0x%08x\n", iq_info->len);
		printf("  crc                : 0x%08x\n", iq_info->crc32);
		printf("  main sensor iq addr : 0x%08x\n", iq_info->main_sensor_iq_offset);
		printf("  main sensor iq size : 0x%08x\n", iq_info->main_sensor_iq_size);
		printf("  secondary sensor iq addr : 0x%08x\n", iq_info->secondary_sensor_iq_offset);
		printf("  secondary sensor iq size : 0x%08x\n", iq_info->secondary_sensor_iq_size);
	}
#ifdef DEBUG_DUMP_GZ_COMP_DATA
	dump_gz_comp_data("dat.bin.gz", (iq_info->data), iq_info->len);
#endif
	printf ( "\n" );

	struct wakeup_param_info *wakeup = (struct wakeup_param_info *)(buf + WAKEUP_PARAM_OFFSET);
	uint32_t *wakeup_head_p = (uint32_t *)g_wakeup_param_head;
	if (wakeup->head != *wakeup_head_p) {
		printf("Invalid wakeup param config\n");
	} else {
		printf("wakeup param config : \n");
		printf("  offset             : 0x%08x\n", WAKEUP_PARAM_OFFSET);
		printf("  length             : 0x%08x\n", wakeup->len);
		printf("  crc32              : 0x%08xx", wakeup->crc32);

		printf("  wakeup_mode        : %d\n", wakeup->wakeup_mode);
		printf("  ae_wakeup_mode     : %d\n", wakeup->ae_wakeup_mode);
		printf("  mcu_run_count      : %d\n", wakeup->mcu_run_count);
		printf("  mcu_max_run_count  : %d\n", wakeup->mcu_max_run_count);
		printf("  arm_run_count      : %d\n", wakeup->arm_run_count);
		printf("  arm_max_run_count  : %d\n", wakeup->arm_max_run_count);
	}

	/* dump the bakup. */
	printf("\nDump the backup info:\n");
	struct cmdline_info *cmdline_backup_p = (struct cmdline_info *)(buf + CMDLINE_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	if (cmdline_backup_p->tag != *cmdline_head_p) {
		printf("Invalid cmdline\n");
	} else {
		printf("cmdline      : %s\n", cmdline_backup_p->data );
		printf("cmdline crc32: 0x%08x\n", cmdline_backup_p->crc32 );
	}
	printf ( "\n" );

	dump_sensor_init((struct sensor_init_cfg *)(buf + SENSOR_INIT_OFFSET + BACKUP_META_SIZE - ITEM_SIZE), "main backup");
	dump_sensor_init((struct sensor_init_cfg *)(buf + SECONDARY_SENSOR_INIT_OFFSET + BACKUP_META_SIZE - ITEM_SIZE), "secondary backup");

	struct ae_awb_init_cfg *ae_awb_backup = (struct ae_awb_init_cfg *)(buf + AE_TABLE_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	uint32_t *ae_awb_head_backup_p = (uint32_t *)g_ae_awb_table_head;
	/* struct ae_awb_init_cfg *ae_awb_init_cfg_bak_p = (struct ae_awb_init_cfg *)ae_awb->data; */
	if (ae_awb_backup->head != *ae_awb_head_backup_p) {
		printf("Invalid sensor backup ae awb config\n");
	} else {
		printf("Sensor ae awb config backup : \n");
		printf("  length             : 0x%08x\n", ae_awb_backup->len);
		printf("  crc                : 0x%08x\n", ae_awb_backup->crc32);

		printf("  black_lvl          : 0x%04x\n", ae_awb_backup->ae.black_lvl);
		printf("  adc calib type     : 0x%04x\n", ae_awb_backup->ae.adc_calib_type);
		printf("  adc range type     : 0x%04x\n", ae_awb_backup->ae.adc_range_type);
		printf("  adc_calib          : 0x%04x 0x%04x 0x%04x 0x%04x\n", ae_awb_backup->ae.adc_calib[0],ae_awb_backup->ae.adc_calib[1],ae_awb_backup->ae.adc_calib[2],ae_awb_backup->ae.adc_calib[3]);
		printf("  start_exp          : 0x%04x 0x%04x 0x%04x\n", ae_awb_backup->ae.start_exp[0],ae_awb_backup->ae.start_exp[1],ae_awb_backup->ae.start_exp[2]);
		printf("  rg_gain_base       : 0x%08x\n", ae_awb_backup->awb.rg_gain_base);
		printf("  bg_gain_base       : 0x%08x\n", ae_awb_backup->awb.bg_gain_base);
		printf("  awb_gain_rad       : 0x%08x\n", ae_awb_backup->awb.awb_gain_rad);
		printf("  awb_gain_dis       : 0x%08x\n", ae_awb_backup->awb.awb_gain_dis);
	}

	printf ( "\n" );
	struct app_param_info *app_bak = (struct app_param_info *)(buf + APP_PARAM_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	if (app_bak->head != *(uint32_t *)g_app_param_head) {
		printf("Invalid app param backup config\n");
	} else {
		printf("App parameter config backup: \n");
		printf("  length             : 0x%08x\n", app_bak->len);
		printf("  crc                : 0x%08x\n", app_bak->crc32);
		printf("  %s : %d\n",RK_CAM_MIRROR_FLIP , app_bak->cam_mirror_flip);
		printf("  %s : %d\n",RK_CAM2_MIRROR_FLIP , app_bak->cam2_mirror_flip);
		printf("  %s         : %d\n",RK_CAM_FPS, app_bak->cam_fps);
		printf("  %s         : %d\n",RK_CAM2_FPS, app_bak->cam2_fps);
		printf("  %s         : %d\n",RK_CAM_HDR, app_bak->cam_hdr);
		printf("  %s         : %d\n",RK_CAM2_HDR, app_bak->cam2_hdr);
		printf("  %s       : %d\n",RK_CAM_INDEX, app_bak->cam_index);
		printf("  %s       : %d\n",RK_LED_VALUE       , app_bak->led_value);
		printf("  %s      : %d\n",RK_NIGHT_MODE      , app_bak->night_mode);
		printf("  %s      : %d\n",RK_FULLCOLOR_GRAY_STRINGS, app_bak->color_mode);
		printf("  %s          : %d\n",RK_VENC_W , app_bak->venc_w);
		printf("  %s          : %d\n",RK_VENC_H , app_bak->venc_h);
		printf("  %s       : %d\n",RK_VENC_TYPE , app_bak->venc_type);
		printf("  %s    : %d\n",RK_VENC_BITRATE , app_bak->venc_bitrate);
		printf("  %s         : %d\n",RK_VENC2_W , app_bak->venc2_w);
		printf("  %s         : %d\n",RK_VENC2_H , app_bak->venc2_h);
		printf("  %s      : %d\n",RK_VENC2_TYPE , app_bak->venc2_type);
		printf("  %s   : %d\n",RK_VENC2_BITRATE , app_bak->venc2_bitrate);
		printf("  %s       : %d\n",RK_FASTAE_MAX_FRAME, app_bak->fastae_max_frame);
		printf("  %s         : %d\n",RK_CAM_SENSOR_NUM, app_bak->cam_num);
	}

	free(buf);
	return 0;
}

int recover_meta(const char *path)
{
	int ret = -1, repair = 0;
	int meta_fd = 0;

	g_meta_part_size = _get_meta_size(path);
	if ( g_meta_part_size == -1 ) {
		printf ( "recovery get meta part size error\n" );
		return -1;
	}

	uint8_t *buf = (uint8_t *)malloc(g_meta_part_size);
	if (buf == NULL) {
		printf("malloc dump buf failed:%s\n", strerror(errno));
		return -1;
	}

	/********************************************/
	if (g_is_nand_flash == META_TRUE) {
		printf ( "found nand flash\n");
		ret = flash_read_buf(g_meta_path, (char *)buf, g_meta_startup_part_offset, g_meta_part_size);
		if (ret) {
			printf("read meta file failed: %d\n", ret);
			goto err;
		}
	}
	else {
		meta_fd = open(path, O_RDWR);
		if (meta_fd < 0) {
			free(buf);
			return -1;
		}

		int len = read(meta_fd, buf, g_meta_part_size);
		if (len == 0) {
			printf("empty file\n");
			goto err;
		}
	}
	/********************************************/

	if (*(uint32_t *)buf != RK_META) {
		printf("Invalid meta head\n");
		goto err;
	}

	if (*(uint32_t *)(buf + MAX_META_SEGMENT_SIZE - 4) != RK_META_END) {
		printf("Invalid meta end\n");
		goto err;
	}

	uint32_t chk_crc32 = 0;

	struct cmdline_info *cmdline_p = (struct cmdline_info *)(buf + CMDLINE_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	uint32_t *cmdline_head_p = (uint32_t *)&g_cmdline_head;
	chk_crc32 = crc32(0, (uint8_t *)cmdline_p, sizeof(struct cmdline_info)-4);
	if ((cmdline_p->tag == *cmdline_head_p) && (cmdline_p->crc32 == chk_crc32)) {
		write_item_to_buffer(buf + CMDLINE_OFFSET, (uint8_t *)cmdline_p, ITEM_SIZE);
		printf("recover cmdline data success.\n");
		repair++;
	} else {
		printf("Invalid cmdline, repair...\n");
	}

	struct meta_item *sensor_init = (struct meta_item *)(buf + SENSOR_INIT_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	uint32_t *sensor_init_head_p = (uint32_t *)&g_sensor_init_head;
	chk_crc32 = crc32(0, (uint8_t *)sensor_init->data, sensor_init->len);
	if ((sensor_init->tag == *sensor_init_head_p) && (sensor_init->crc32 == chk_crc32)) {
		write_item_to_buffer(buf + SENSOR_INIT_OFFSET, (uint8_t *)sensor_init, ITEM_SIZE);
		printf("recover sensor init success.\n");
		repair++;
	} else {
		printf("Invalid sensor init setting, repair...\n");
	}

	struct meta_item *ae_awb = (struct meta_item *)(buf + AE_TABLE_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	chk_crc32 = crc32(0, ae_awb->data, ae_awb->len);
	uint32_t *ae_awb_head_p = (uint32_t *)g_ae_awb_table_head;
	if ((ae_awb->tag == *ae_awb_head_p) && (ae_awb->crc32 == chk_crc32)) {
		write_item_to_buffer(buf + AE_TABLE_OFFSET, (uint8_t *)ae_awb, ITEM_SIZE);
		printf("recover ae awb success.\n");
		repair++;
	} else {
		printf("Invalid sensor ae awb head, repair faild...\n");
	}

	struct app_param_info *pApp = (struct app_param_info *)(buf + APP_PARAM_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
	chk_crc32 = crc32(0, (uint8_t *)pApp, pApp->len);
	uint32_t *app_param_head_p = (uint32_t *)g_app_param_head;
	if ((pApp->head == *app_param_head_p) && (pApp->crc32 == chk_crc32)) {
		write_item_to_buffer(buf + APP_PARAM_OFFSET, (uint8_t *)pApp, ITEM_SIZE);
		printf("recover app param success.\n");
		repair++;
	} else {
		printf("Invalid app param head, repair faild...\n");
	}

	if (pApp->cam_num == 2) {
		struct meta_item *secondary_sensor_init = (struct meta_item *)(buf + SECONDARY_SENSOR_INIT_OFFSET + BACKUP_META_SIZE - ITEM_SIZE);
		uint32_t *secondary_sensor_init_head_p = (uint32_t *)&g_sensor_init_head;
		chk_crc32 = crc32(0, (uint8_t *)secondary_sensor_init->data, secondary_sensor_init->len);
		if ((secondary_sensor_init->tag == *secondary_sensor_init_head_p) && (secondary_sensor_init->crc32 == chk_crc32)) {
			write_item_to_buffer(buf + SECONDARY_SENSOR_INIT_OFFSET, (uint8_t *)secondary_sensor_init, ITEM_SIZE);
			printf("recover secondary sensor init success.\n");
			repair++;
		} else {
			printf("Invalid secondary sensor init setting, repair...\n");
		}
	}

	if (g_is_nand_flash == META_TRUE) {
		ret = flash_write_buf(g_meta_path, (char *)buf, g_meta_startup_part_offset, g_meta_part_size);
		if (ret) {
			printf("write failed, ret=%d\n", ret);
			goto err;
		}
	} else {
		ret = write_item_to_file(meta_fd, META_INFO_HEAD_OFFSET, (uint8_t *)buf, g_meta_part_size);
		if (ret < 0) {
			printf("failed to write meta file\n");
			goto err;
		}
	}
	if (repair)
		printf("recover %d items\n", repair);
err:
	if (meta_fd) {
		close(meta_fd);
	}
	if (buf) {
		free(buf);
	}

	return ret;
}

static int get_param_val (uint32_t *pParam, const char *optarg)
{
#define	PARAMETER_BUF_LEN		17
	char *endptr = NULL;
	char l_parameter_buf[PARAMETER_BUF_LEN] = { 0 };
	strncpy(l_parameter_buf, optarg, PARAMETER_BUF_LEN);
	l_parameter_buf[PARAMETER_BUF_LEN-1] = '\0';

	uint32_t num = 0;
	errno = 0;    /* To distinguish success/failure after call */

	if ( l_parameter_buf[0] == '0' && l_parameter_buf[1] == 'x') {
		num = strtol(l_parameter_buf, &endptr, 16);
	} else {
		num = strtol(l_parameter_buf, &endptr, 10);
	}
	*pParam = num;
	if ((errno == ERANGE && (*pParam == LONG_MAX || *pParam == LONG_MIN))
			|| (errno != 0 && *pParam == 0)) {
		perror("strtol");
		return (EXIT_FAILURE);
	}

	if (endptr == l_parameter_buf) {
		fprintf(stderr, "No digits were found\n");
		return (EXIT_FAILURE);
	}
	if (*endptr != '\0') {
		/* Not necessarily an error... */
		fprintf(stderr, "[Error] Further characters after number: %s\n", endptr);
		return (EXIT_FAILURE);
	}

	return 0;
}

static int usage(void)
{
	printf("Usage:\n");
	printf("make_meta <argu1> <argu2> [...]\n");
	printf("\n");
	printf("Build Arguments:\n");
	printf("  --create | --update                    Create or Update a new meta data\n");
	printf("  -d <file path>                         Dump meta file\n");
	printf("  -r <file path>                         Recover meta file\n");
	printf("  --rtt-log                              Dump RTT log\n");
	printf("  --no-compress-iq                       Generate iq.bin without compress.\n");
	printf("  --crc32 <file path>                    Check the crc32 of file\n");
	printf("  --cmdline <cmdline string>             Add arguments to kernel CMDLINE\n");
	printf("  --meta_path <file path>                A path to write meta data\n");
	printf("  --sensor_init <file path>              A path to init_sensor.bin\n");
	printf("  --secondary_sensor_init <file path>    A path to init_sensor.bin\n");
	printf("  --ae_awb_tab <file path>               A path to ae_awb_table.bin\n");
	printf("  --sensor_iq_bin <file path>            A path to main camera sensor iq bin\n");
	printf("  --secondary_sensor_iq_bin <file path>  A path to secondary camera sensor iq bin\n");
	printf("  --%s <address>             Address of meta load\n", META_LOAD_ADDR);
	printf("  --%s <size>                Size of meta partition\n", META_LOAD_SIZE);
	printf("  --%s <size>                      Size of sensor width\n", RK_CAM_W);
	printf("  --%s <size>                      Size of sensor heigth\n", RK_CAM_H);
	printf("  --%s <size>                     Size of secondary sensor width\n", RK_CAM2_W);
	printf("  --%s <size>                     Size of secondary sensor heigth\n", RK_CAM2_H);
	printf("  --%s <size>                    Size of sensor frame rate\n", RK_CAM_FPS);
	printf("  --%s <size>                   Size of secondary sensor frame rate\n", RK_CAM2_FPS);
	printf("  --%s <size>                  Size of led value <size: 0~100>\n", RK_LED_VALUE);

/***night mode start {{{**************************************/
	printf("  --%s <mode>                 Night Mode of sesnor:\n", RK_NIGHT_MODE);
	printf("                                             0: NONE \n");
	printf("                                             1: White Led Auto \n");
	printf("                                             2: IR Led Auto \n");
	printf("                                             3: White Led Always On \n");
	printf("                                             4: IR Led Always On\n");
/***night mode end }}}**************************************/

	printf("  --%s <size>                     Size of venc width\n", RK_VENC_W);
	printf("  --%s <size>                     Size of venc heigth\n", RK_VENC_H);
	printf("  --%s <type>                  Type of venc coding type <1: h264, 2: h265, 3: mjpeg>\n", RK_VENC_TYPE);

	printf("  --%s <size>               Size of venc bit rate value <size> Kbps (default %s x %s / 1000)\n",
			RK_VENC_BITRATE, RK_VENC_W, RK_VENC_H);

	printf("  --%s <size>                    Size of venc2 width\n", RK_VENC2_W);
	printf("  --%s <size>                    Size of venc2 heigth\n", RK_VENC2_H);
	printf("  --%s <type>                 Type of venc2 coding type <1: h264, 2: h265, 3: mjpeg>\n", RK_VENC2_TYPE);

	printf("  --%s <size>              Size of venc2 bit rate value <size> Kbps (default %s x %s / 1000)\n",
			RK_VENC2_BITRATE, RK_VENC2_W, RK_VENC2_H);

/***camera sensor mirror flip start {{{**************************************/
	printf("  --%s <mode>            Mode of main sensor mirror and flip\n", RK_CAM_MIRROR_FLIP);
	printf("                                             0: NONE\n");
	printf("                                             1: Mirror Horizontally\n");
	printf("                                             2: Mirror Vertically\n");
	printf("                                             3: Mirror Horizontally and Vertically\n");

	printf("\n");
	printf("  --%s <mode>           Mode of secondary sensor mirror and flip\n", RK_CAM2_MIRROR_FLIP);
	printf("                                             0: NONE\n");
	printf("                                             1: Mirror Horizontally\n");
	printf("                                             2: Mirror Vertically\n");
	printf("                                             3: Mirror Horizontally and Vertically\n");
/***camera sensor mirror flip end }}}**************************************/

	printf("  --%s <mode>                    Mode of sensor hdr <0: normal, 5: hdr two frame>\n", RK_CAM_HDR);
	printf("  --%s <mode>                   Mode of secondary sensor hdr <0: normal, 5: hdr two frame>\n", RK_CAM2_HDR);
	printf("  --%s <index>                 The index of the sensor which boot firstly. \n", RK_CAM_INDEX);
	printf("  --%s <status>               Status of full color or gray <0: full color, 1: gray> \n", RK_FULLCOLOR_GRAY_STRINGS);
	printf("  --%s <count>          The max frames of the sensor fast AE (Automatic Exposure) while MCU run. \n", RK_FASTAE_MAX_FRAME);

	printf("\n");
	return 0;
}


static int pre_parse_args (int argc, char *argv[])
{
	const char* pchar = NULL;
	for (int i=0; i<argc; i++) {
			if (strncmp(argv[i], META_PARAM_UPDATE, strlen(META_PARAM_UPDATE)) == 0) {
				g_update = META_TRUE;
			} else if (strncmp(argv[i], META_PARAM_META_PATH, strlen(META_PARAM_META_PATH)) == 0) {
				pchar = (char *)argv[i];
				if ( *(pchar + strlen(META_PARAM_META_PATH)) == '=' ) {
					_META_STRNCPY(g_meta_path, (pchar+strlen(META_PARAM_META_PATH) + 1));
				} else {
					_META_STRNCPY(g_meta_path, argv[i+1]);
				}
			}
	}
	return 0;
}

static int check_file_crc32(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed: %s\n", filename, strerror(errno));
		return EXIT_FAILURE;
	}

	uint8_t *filebuf = NULL;
	int filesize = lseek(fd, 0, SEEK_END);
	if (filesize < 0) {
		printf("lseek %s failed: %s\n", filename, strerror(errno));
		goto out;
	}

	filebuf = (uint8_t *)malloc(filesize);
	if (filebuf == NULL) {
		printf("malloc buffer failed:%s\n", strerror(errno));
		return -1;
	}

	lseek(fd, 0, SEEK_SET);
	if (read(fd, (void *)filebuf, filesize) < 0) {
		printf("read failed:%s\n", strerror(errno));
		goto out;
	}

	printf ("info: check crc32 of %s is: [%#lx]\n", g_meta_path, crc32(0, filebuf, filesize));

out:
	if (filebuf) {
		free(filebuf);
		filebuf = NULL;
	}

	close(fd);
	return 0;
}

static int parse_args (int argc, char *argv[])
{
	int ret = 0;
	if (g_update) {
		struct app_param_info *app_cfg = (struct app_param_info *)(g_meta_buf + APP_PARAM_OFFSET);
		g_rk_cam_num = app_cfg->cam_num;
		g_rk_venc_w = app_cfg->venc_w;
		g_rk_venc_h = app_cfg->venc_h;
		g_rk_venc_type = app_cfg->venc_type;
		g_rk_venc_bitrate = app_cfg->venc_bitrate;
		if (g_rk_cam_num == 2) {
			g_rk_venc2_w = app_cfg->venc2_w;
			g_rk_venc2_h = app_cfg->venc2_h;
			g_rk_venc2_type = app_cfg->venc2_type;
			g_rk_venc2_bitrate = app_cfg->venc2_bitrate;
		}
		g_rk_cam_mirror_flip = app_cfg->cam_mirror_flip;
		g_rk_cam2_mirror_flip = app_cfg->cam2_mirror_flip;
		g_rk_cam_fps = app_cfg->cam_fps;
		g_rk_cam2_fps = app_cfg->cam2_fps;
		g_rk_night_mode = app_cfg->night_mode;
		g_rk_led_value = app_cfg->led_value;
		g_rk_color_mode = app_cfg->color_mode;
		g_rk_cam_hdr = app_cfg->cam_hdr;
		g_rk_cam2_hdr = app_cfg->cam2_hdr;
		g_rk_cam_index = app_cfg->cam_index;
		g_rk_fastae_max_frame = app_cfg->fastae_max_frame;

		struct sensor_init_cfg *cam_cfg = (struct sensor_init_cfg *)(g_meta_buf + SENSOR_INIT_OFFSET);
		g_rk_cam_h = cam_cfg->cam_h ;
		g_rk_cam_w = cam_cfg->cam_w ;

		struct sensor_init_cfg *cam2_cfg = (struct sensor_init_cfg *)(g_meta_buf + SECONDARY_SENSOR_INIT_OFFSET);
		if (cam2_cfg->head == (*(uint32_t *)g_sensor_init_head)) {
			g_rk_cam2_h = cam2_cfg->cam_h ;
			g_rk_cam2_w = cam2_cfg->cam_w ;
		}

		struct sensor_iq_info *iqinfo = (struct sensor_iq_info *)(g_meta_buf + SENSOR_IQ_BIN_OFFSET);
		g_rk_iqbin_main_offset = iqinfo->main_sensor_iq_offset;
		g_rk_iqbin_main_size = iqinfo->main_sensor_iq_size;
		g_rk_iqbin_secondary_offset = iqinfo->secondary_sensor_iq_offset;
		g_rk_iqbin_secondary_size= iqinfo->secondary_sensor_iq_size;

		struct meta_head *meta_head_p = (struct meta_head *)g_meta_buf;
		g_meta_part_size = meta_head_p->size;
		g_meta_compress_flag = meta_head_p->comp_type;
	}
	while (1) {
		int opt_index = 0;
		struct option long_options[] = {
			{"create", no_argument, 0, 0},
			{"update", no_argument, 0, 0},
			{"rtt-log", no_argument, 0 , 0} ,
			{"rtt-log-clean", no_argument, 0 , 0} ,
			{"no-compress-iq", no_argument, 0 , 0} ,
			{"crc32", required_argument , 0 , 0} ,
			{"meta_path", required_argument, 0, 0},
			{"cmdline", required_argument, 0, 0},
			{"als_tpye_none", no_argument, 0, 0},
			{RK_CAM_W           , required_argument , 0 , 0} ,
			{RK_CAM_H           , required_argument , 0 , 0} ,
			{RK_VENC_W           , required_argument , 0 , 0} ,
			{RK_VENC_H           , required_argument , 0 , 0} ,
			{RK_VENC_TYPE           , required_argument , 0 , 0} ,
			{RK_VENC_BITRATE           , required_argument , 0 , 0} ,
			{RK_CAM2_W           , required_argument , 0 , 0} ,
			{RK_CAM2_H           , required_argument , 0 , 0} ,
			{RK_VENC2_W           , required_argument , 0 , 0} ,
			{RK_VENC2_H           , required_argument , 0 , 0} ,
			{RK_VENC2_TYPE           , required_argument , 0 , 0} ,
			{RK_VENC2_BITRATE           , required_argument , 0 , 0} ,
			{RK_CAM_MIRROR_FLIP , required_argument , 0 , 0} ,
			{RK_CAM2_MIRROR_FLIP , required_argument , 0 , 0} ,
			{RK_CAM_FPS, required_argument , 0 , 0} ,
			{RK_CAM2_FPS, required_argument , 0 , 0} ,
			{RK_CAM_HDR         , required_argument , 0 , 0} ,
			{RK_CAM2_HDR         , required_argument , 0 , 0} ,
			{RK_CAM_INDEX         , required_argument , 0 , 0} ,
			{RK_FASTAE_MAX_FRAME , required_argument , 0 , 0} ,
			{RK_FULLCOLOR_GRAY_STRINGS , required_argument , 0 , 0} ,
			{RK_LED_VALUE       , required_argument , 0 , 0} ,
			{RK_NIGHT_MODE      , required_argument , 0 , 0} ,
			{"sensor_init", required_argument, 0, 0},
			{"secondary_sensor_init", required_argument, 0, 0},
			{"ae_awb_tab", required_argument, 0, 0},
			{"sensor_iq_bin", required_argument, 0, 0},
			{"secondary_sensor_iq_bin", required_argument, 0, 0},
			{RK_IQ_BIN_SECONDARY_SIZE_STRINGS, required_argument, 0, 0},
			{META_LOAD_ADDR, required_argument, 0, 0},
			{META_LOAD_SIZE, required_argument, 0, 0},
			{0, 0, 0, 0}
		};

		int c = getopt_long_only(argc, argv, "d:r:", long_options, &opt_index);
		if (c == -1) break;

		char const *option;
		switch (c) {
		case 0:
			option = long_options[opt_index].name;
			if (strcmp(option, "create") == 0) {
				g_create = META_TRUE;
			} else if (strcmp(option, "update") == 0) {
				g_update = META_TRUE;
			} else if (strcmp(option, "no-compress-iq") == 0) {
				g_meta_compress_flag = !META_COMPRESS_TYPE_GZ;
			} else if (strcmp(option, "rtt-log") == 0) {
				dump_rtt_log(META_TRUE);
				exit(EXIT_SUCCESS);
			} else if (strcmp(option, "rtt-log-clean") == 0) {
				dump_rtt_log(META_FALSE);
				exit(EXIT_SUCCESS);
			} else if (strcmp(option, "meta_path") == 0) {
				_META_STRNCPY(g_meta_path, optarg);
			} else if (strcmp(option, "als_tpye_none") == 0) {
				g_set_als_type_none = META_TRUE;
			} else if (strcmp(option, "cmdline") == 0) {
				sprintf(g_cmdline+strlen(g_cmdline), "%s ", optarg);
				g_cmdline_need = META_TRUE;
				g_cmdline_need_clean = META_TRUE;
			} else if (strcmp(option, "sensor_init") == 0) {
				_META_STRNCPY(g_sensor_init_path, optarg);
				g_cmdline_need = META_TRUE;
			} else if (strcmp(option, "secondary_sensor_init") == 0) {
				_META_STRNCPY(g_secondary_sensor_init_path, optarg);
				g_cmdline_need = META_TRUE;
			} else if (strcmp(option, "ae_awb_tab") == 0) {
				_META_STRNCPY(g_ae_awb_table, optarg);
			} else if (strcmp(option, "sensor_iq_bin") == 0) {
				_META_STRNCPY(g_sensor_iq_bin, optarg);
				if (access(g_sensor_iq_bin, R_OK) != 0) {
					printf( "error: no found main sensor iq bin, please check!!!\n");
					return -1;
				}
				g_cmdline_need = META_TRUE;
				g_rk_iqbin_main_size = _get_file_size(g_sensor_iq_bin, (SENSOR_IQ_BIN_MAX_SIZE - sizeof(struct sensor_iq_info)));
			} else if (strcmp(option, "secondary_sensor_iq_bin") == 0) {
				if (g_update && g_rk_cam_num == 1) {
					printf( "error: meta.img not contain dual camera sensor, please check!!!\n");
					return 1;
				}
				_META_STRNCPY(g_secondary_sensor_iq_bin, optarg);
				if (access(g_secondary_sensor_iq_bin, R_OK) != 0) {
					printf( "error: no found secondary sensor iq bin, please check!!!\n");
					return 1;
				}
				g_cmdline_need = META_TRUE;
				g_rk_cam_num = 2;
				g_rk_iqbin_secondary_size = _get_file_size(g_secondary_sensor_iq_bin, (SENSOR_IQ_BIN_MAX_SIZE - sizeof(struct sensor_iq_info)));
			} else if (strcmp(option, META_LOAD_ADDR) == 0) {
				get_param_val(&g_meta_load_addr, optarg);
			} else if (strcmp(option, META_LOAD_SIZE) == 0) {
				get_param_val(&g_meta_part_size, optarg);
			} else if (strcmp(option, RK_CAM_W) == 0) {
				if (get_param_val(&g_rk_cam_w, optarg) == 0) {
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM_H) == 0) {
				if (get_param_val(&g_rk_cam_h, optarg) == 0) {
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM2_W) == 0) {
				if (get_param_val(&g_rk_cam2_w, optarg) == 0) {
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM2_H) == 0) {
				if (get_param_val(&g_rk_cam2_h, optarg) == 0) {
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC_H) == 0) {
				if (get_param_val(&g_rk_venc_h, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC_W) == 0) {
				if (get_param_val(&g_rk_venc_w, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC_TYPE) == 0) {
				if (get_param_val(&g_rk_venc_type, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC_BITRATE) == 0) {
				if (get_param_val(&g_rk_venc_bitrate, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC2_H) == 0) {
				if (get_param_val(&g_rk_venc2_h, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC2_W) == 0) {
				if (get_param_val(&g_rk_venc2_w, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC2_TYPE) == 0) {
				if (get_param_val(&g_rk_venc2_type, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_VENC2_BITRATE) == 0) {
				if (get_param_val(&g_rk_venc2_bitrate, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM_MIRROR_FLIP) == 0) {
				if (get_param_val(&g_rk_cam_mirror_flip, optarg) == 0 ) {
					g_app_param_need = META_TRUE;
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM2_MIRROR_FLIP) == 0) {
				if (get_param_val(&g_rk_cam2_mirror_flip, optarg) == 0 ) {
					g_app_param_need = META_TRUE;
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM_FPS) == 0) {
				if (get_param_val(&g_rk_cam_fps, optarg) == 0) {
					g_app_param_need = META_TRUE;
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM2_FPS) == 0) {
				if (get_param_val(&g_rk_cam2_fps, optarg) == 0) {
					g_app_param_need = META_TRUE;
					g_cmdline_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM_HDR) == 0) {
				if (get_param_val(&g_rk_cam_hdr, optarg) == 0) {
					g_cmdline_need = META_TRUE;
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM2_HDR) == 0) {
				if (get_param_val(&g_rk_cam2_hdr, optarg) == 0) {
					g_cmdline_need = META_TRUE;
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_CAM_INDEX) == 0) {
				if (get_param_val(&g_rk_cam_index, optarg) == 0) {
					g_cmdline_need = META_TRUE;
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_FASTAE_MAX_FRAME) == 0) {
				if (get_param_val(&g_rk_fastae_max_frame, optarg) == 0) {
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_LED_VALUE) == 0) {
				if (get_param_val(&g_rk_led_value, optarg) == 0){
					g_cmdline_need = META_TRUE;
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_FULLCOLOR_GRAY_STRINGS) == 0) {
				if (get_param_val(&g_rk_color_mode, optarg) == 0){
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, RK_NIGHT_MODE) == 0) {
				if (get_param_val(&g_rk_night_mode, optarg) == 0) {
					g_cmdline_need = META_TRUE;
					g_app_param_need = META_TRUE;
				}
			} else if (strcmp(option, "crc32") == 0) {
				g_crc32_check = META_TRUE;
				_META_STRNCPY(g_meta_path, optarg);
				break;
			} else {
				fprintf(stderr, "Not found paramter\n");
				exit(EXIT_FAILURE);
			}

			break;

		case 'd':
			g_dump = META_TRUE;
			_META_STRNCPY(g_meta_path, optarg);
			break;

		case 'r':
			g_recover = META_TRUE;
			_META_STRNCPY(g_meta_path, optarg);
			break;

		default:
			return usage();
			break;
		}
	}

	if (strlen(g_meta_path) == 0) {
		printf("please set <meta_path>\n");
		return -1;
	}

	if (g_crc32_check) {
		if (access(g_meta_path, R_OK) != 0) {
			printf("%s:%d can't be read\n", g_meta_path, __LINE__ );
			return -1;
		}

		check_file_crc32(g_meta_path);
		exit(EXIT_SUCCESS);
	}

	if (!g_update) {
		g_meta_startup_part_num = _get_startup_part_num(g_meta_path);
		if (g_meta_startup_part_num <= 0) {
			printf("Get startup part number failed. Error number: %d\n", g_meta_startup_part_num);
			return -1;
		}
		/* The first part offset starts at 0x0 offset. */
		g_meta_startup_part_offset = (g_meta_startup_part_num - 1) * g_meta_per_part_size;
	}

	if (g_dump) {
		if (access(g_meta_path, R_OK) != 0) {
			printf("%s:%d can't be read\n", g_meta_path, __LINE__ );
			return -1;
		}
		dump_meta(g_meta_path);
		exit(EXIT_SUCCESS);
	}

	if (g_recover) {
		if (access(g_meta_path, R_OK) != 0) {
			printf("%s:%d can't be read\n", g_meta_path, __LINE__ );
			return -1;
		}
		ret = recover_meta(g_meta_path);
		if (ret < 0)
			printf("recover meta failed\n");
		exit(ret);
	}

	if (g_create && g_update) {
		printf("create and update can't be enabled at the same time\n");
		return -1;
	}

	if (!(g_create || g_update)) {
		printf("at least choose create or update\n");
		return -1;
	}

	if (g_create) {
		if (file_create_ok(g_meta_path) != 0) {
			printf("%s can't be created\n", g_meta_path);
			return -1;
		}
	}


#ifdef IS_NAND_FLASH
  if (g_create || g_update) {
		if ((g_rk_cam_num == 1) &&
				(g_rk_cam2_w != 0 ||
				 g_rk_cam2_h != 0 ||
				 g_rk_venc2_h != 0 ||
				 g_rk_venc2_type != RK_VENC_TYPE_INVALID ||
				 g_rk_venc2_w != 0 ||
				 g_rk_venc2_bitrate != 0 )) {
			printf ( "error: [%s] not support dual camera parameter.\n", g_meta_path );
			return -1;
		}
#else
  if (g_create || g_update) {
		if ((g_rk_cam_num == 1) &&
				(g_rk_cam2_w != 0 ||
				 g_rk_cam2_h != 0 ||
				 g_rk_venc2_h != 0 ||
				 g_rk_venc2_type != RK_VENC_TYPE_INVALID ||
				 g_rk_venc2_w != 0 ||
				 g_rk_venc2_bitrate != 0 )) {
			printf ( "error: [%s] not support dual camera parameter.\n", g_meta_path );
			return -1;
		} else if ( (g_rk_cam_num == 1) && (g_meta_part_size != (384*1024)) ) {
			printf ( "error: meta partiton size error.\n" );
			return -1;
		} else if ( (g_rk_cam_num == 2) && (g_meta_part_size != (704*1024)) ) {
			printf ( "error: meta partiton size error for dual camera.\n" );
			return -1;
		}
#endif
	}

	if (strlen(g_sensor_init_path) != 0) {
		if (access(g_sensor_init_path, R_OK) != 0) {
			printf("sensor init setting:%s can't be accessed\n", g_sensor_init_path);
			return -1;
		}
	}

	if (strlen(g_secondary_sensor_init_path) != 0) {
		if (access(g_secondary_sensor_init_path, R_OK) != 0) {
			printf("sensor init setting:%s can't be accessed\n", g_secondary_sensor_init_path);
			return -1;
		}
	}

	if (g_rk_iqbin_main_size == 0 && g_rk_iqbin_secondary_size != 0) {
		printf("error: not found main sensor iqbin.\n");
		return -1;
	}

	return 0;
}

static int _strings_remove_space(uint8_t *todo_str)
{
	uint8_t *pmsk = (uint8_t *)(todo_str + strlen((const char*)todo_str)-1);
	while (pmsk && *pmsk == ' ') {
		*pmsk = '\0';
		pmsk--;
	}
	return 0;
}

#define EQUAL "="
#define KEY_EQUAL(KEY) KEY EQUAL

static int _strings_remove(uint8_t *todo_str, const char *target)
{
	char *pmsk = strstr((const char *)todo_str, (const char *)target);
	while (pmsk && (*pmsk != ' ') && (*pmsk != '\0')) {
		*pmsk = ' ';
		pmsk++;
	}
	return 0;
}

static void update_meta_head (uint8_t *meta_buf)
{
	struct meta_head *meta_head_p = (struct meta_head *)meta_buf;

	/* write the head and thed end */
	meta_head_p->tag = RK_META;
	meta_head_p->load = g_meta_load_addr;
	meta_head_p->size = g_meta_part_size;

	 meta_head_p->comp_off = SENSOR_IQ_BIN_OFFSET + sizeof(struct sensor_iq_info);
	 meta_head_p->comp_size = g_meta_compress_size;
	 meta_head_p->comp_type = g_meta_compress_flag;
	 meta_head_p->iq_item_size = sizeof(struct sensor_iq_info);
	meta_head_p->total_part_num = g_meta_total_part_num;
	meta_head_p->part_flag = g_meta_part_flag;
	meta_head_p->part_reserved_size = g_meta_part_reserved_size;

	printf ( "info: meta load address   [0x%08x]\n", meta_head_p->load );
	printf ( "info: meta partition size [0x%08x]\n", meta_head_p->size );
	printf ( "info: meta comp offset    [0x%08x]\n", meta_head_p->comp_off);
	printf ( "info: meta comp size      [0x%08x]\n", meta_head_p->comp_size);
	printf ( "info: item IQ head size   [%d]\n", meta_head_p->iq_item_size);
	printf ( "info: meta total part num [%d]\n", meta_head_p->total_part_num);
	printf ( "info: part flag           [%d]\n", meta_head_p->part_flag);
	printf ( "info: part reserve size   [0x%08x]\n", meta_head_p->part_reserved_size);
	memset(meta_head_p->reserved, 0x0, META_HEAD_RESERVED_SIZE);
	meta_head_p->crc32 = crc32(0, (unsigned char *)meta_head_p, sizeof(struct meta_head) - 4 - 4);
	*(uint32_t *)(meta_buf + MAX_META_SEGMENT_SIZE - 4) = RK_META_END;

	return ;
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int meta_fd = 0, meta_len = 0;
	uint8_t *item_buf = NULL;
	uint8_t *meta_buf = NULL;
	uint32_t meta_final_file_size = 0;

	if (argc == 1) {
		usage();
		return 0;
	}

	g_is_nand_flash = check_str_cmd("mtdparts=spi-nand0");

	pre_parse_args(argc, argv);
	if (g_update) {
		if (access(g_meta_path, R_OK | W_OK) != 0) {
			printf("%s can't be accessed\n", g_meta_path);
			return -1;
		}

		g_meta_part_size = _get_meta_size(g_meta_path);
		if ( g_meta_part_size == -1 ) {
			printf ( "get meta part size error\n" );
			return -1;
		}

		meta_buf = (uint8_t *)malloc(g_meta_part_size);
		if (meta_buf == NULL) {
			printf("malloc meta buffer failed:%s\n", strerror(errno));
			return -1;
		}
		memset(meta_buf, 0xff, g_meta_part_size);
		g_meta_buf = meta_buf;

		item_buf = (uint8_t *)malloc(ITEM_SIZE);
		if (item_buf == NULL) {
			printf("malloc item_buf failed:%s\n", strerror(errno));
			ret = -1;
			goto free_buf;
		}

		g_meta_startup_part_num = _get_startup_part_num(g_meta_path);
		if (g_meta_startup_part_num <= 0) {
			printf("Get startup part number failed. Error number: %d\n", g_meta_startup_part_num);
			ret = -1;
			goto free_buf;
		}
		/* The first part offset starts at 0x0 offset. */
		g_meta_startup_part_offset = (g_meta_startup_part_num - 1) * g_meta_per_part_size;

		/******************************************/
		if (g_is_nand_flash == META_TRUE) {
			printf ( "found nand flash\n");
			ret = flash_read_buf(g_meta_path, (char *)meta_buf, g_meta_startup_part_offset, g_meta_part_size);
			if (ret) {
				printf("read meta file failed: %d\n", ret);
				goto free_buf;
			}
			meta_len = g_meta_part_size;
		} else {
			meta_fd = open(g_meta_path, O_RDWR);
			if (meta_fd < 0) {
				printf("open %s fail: %s\n", g_meta_path, strerror(errno));
				ret = -1;
				goto free_buf2;
			}

			meta_len = read(meta_fd, meta_buf, g_meta_part_size);
			if (meta_len < 0) {
				printf("read meta file failed:%s\n", strerror(errno));
				ret = -1;
				goto close_file;
			} else if (meta_len == 0) {
				printf("empty meta file\n");
				ret = -1;
				goto close_file;
			} else if (meta_len != g_meta_part_size) {
				printf("invalid meta size:%d\n", meta_len);
				ret = -1;
				goto close_file;
			}
		}
		/******************************************/

		/* verify valid meta or not */
		if (*(uint32_t *)meta_buf != RK_META) {
			printf("invalid meta head\n");
			ret = -1;
			goto close_file;
		}

		if (*(uint32_t *)(meta_buf + MAX_META_SEGMENT_SIZE - 4) != RK_META_END) {
			ret = -1;
			printf("invalid meta end\n");
			goto close_file;
		}
	}

	ret = parse_args(argc, argv);
	if (ret != 0) {
		printf("invalid parameter\n");
		goto close_file;
	}

	printf("Show arguments:\n");
	printf("  create=%s\n"          , g_create ? "true" : "false");
	printf("  update=%s\n"          , g_update ? "true" : "false");
	printf("  meta_path=%s\n"       , g_meta_path);
	printf("  sensor_init=%s\n"     , g_sensor_init_path);
	printf("  secondary_sensor_init=%s\n" , g_secondary_sensor_init_path);
	printf("  g_ae_awb_table=%s\n"  , g_ae_awb_table);
	printf("  g_sensor_iq_bin=%s\n" , g_sensor_iq_bin);
	printf("  g_secondary_sensor_iq_bin=%s\n" , g_secondary_sensor_iq_bin);

	if (g_create) {
		meta_final_file_size = (g_meta_part_size + g_meta_part_reserved_size) * g_meta_total_part_num;

		meta_buf = (uint8_t *)malloc(meta_final_file_size);
		if (meta_buf == NULL) {
			printf("malloc meta buffer failed:%s\n", strerror(errno));
			return -1;
		}
		memset(meta_buf, 0xff, meta_final_file_size);

		item_buf = (uint8_t *)malloc(ITEM_SIZE);
		if (item_buf == NULL) {
			printf("malloc item_buf failed:%s\n", strerror(errno));
			ret = -1;
			goto free_buf;
		}

		meta_fd = open(g_meta_path, O_CREAT | O_TRUNC | O_RDWR, 0644);
		if (meta_fd < 0) {
			printf("%s can't be create: %s\n", g_meta_path, strerror(errno));
			ret = -1;
			goto free_buf2;
		}

		meta_len = meta_final_file_size;
	}
	printf("info: size of iqbin for main camera sensor %d\n" , g_rk_iqbin_main_size);
	printf("info: size of iqbin for secondary camera sensor %d\n" , g_rk_iqbin_secondary_size);

	/* Sensor init */
	if (g_update) {
		struct sensor_init_cfg *cam_cfg = (struct sensor_init_cfg *)(meta_buf + SENSOR_INIT_OFFSET);
		cam_cfg->cam_w = g_rk_cam_w;
		cam_cfg->cam_h = g_rk_cam_h;
		if ( g_set_als_type_none == META_TRUE ) {
			cam_cfg->als_type = ALS_TYPE_NONE;
			cam_cfg->als_value = FIX2INT16(0.0);
		}
		struct meta_item *sensor_init = (struct meta_item *)(meta_buf + SENSOR_INIT_OFFSET);
		sensor_init->crc32 = crc32(0, sensor_init->data, sensor_init->len);

		if (g_rk_cam_num == 2) {
			struct sensor_init_cfg *cam2_cfg = (struct sensor_init_cfg *)(meta_buf + SECONDARY_SENSOR_INIT_OFFSET);
			if (cam2_cfg->head == *(uint32_t *)g_sensor_init_head) {
				cam2_cfg->cam_w = g_rk_cam2_w;
				cam2_cfg->cam_h = g_rk_cam2_h;
				if ( g_set_als_type_none == META_TRUE ) { /* TODO */ }
				struct meta_item *sensor2_init = (struct meta_item *)(meta_buf + SECONDARY_SENSOR_INIT_OFFSET);
				sensor2_init->crc32 = crc32(0, sensor2_init->data, sensor2_init->len);
			}
		}
	}
	if (access(g_sensor_init_path, R_OK) == 0) {
		uint8_t *sensor_init_buf = item_buf;
		memset((void *)(sensor_init_buf), 0xff, SENSOR_INIT_MAX_SIZE);

		if (init_sensor_init_buf(sensor_init_buf) != 0) {
			printf("init_sensor_init_buf failed\n");
			ret = -1;
			goto close_file;
		}

		ret = write_item_to_buffer(meta_buf + SENSOR_INIT_OFFSET, sensor_init_buf, SENSOR_INIT_MAX_SIZE);

		if (ret < 0) {
			printf("failed to write senser_init to meta file\n");
			goto close_file;
		}
	}
	if (access(g_secondary_sensor_init_path, R_OK) == 0) {
		uint8_t *secondary_sensor_init_buf = item_buf;
		memset((void *)(secondary_sensor_init_buf), 0xff, SECONDARY_SENSOR_INIT_MAX_SIZE);

		if (init_secondary_sensor_init_buf(secondary_sensor_init_buf) != 0) {
			printf("init secondary sensor failed\n");
			ret = -1;
			goto close_file;
		}

		ret = write_item_to_buffer(meta_buf + SECONDARY_SENSOR_INIT_OFFSET, secondary_sensor_init_buf, SECONDARY_SENSOR_INIT_MAX_SIZE);

		if (ret < 0) {
			printf("failed to write secondary senser_init to meta file\n");
			goto close_file;
		}
	}
	/* Sensor init end */

	/* App param start */
	if (g_app_param_need == META_TRUE || g_create) {
		struct app_param_info *pApp = (struct app_param_info *)item_buf;
		uint8_t *app_c = item_buf;
		memset((void *)(pApp), 0xff, APP_PARAM_MAX_SIZE);

		app_c[0] = 'A';
		app_c[1] = 'P';
		app_c[2] = 'R';
		app_c[3] = 'A';

		if (g_rk_venc_h == 0) g_rk_venc_h = g_rk_cam_h;
		if (g_rk_venc_w == 0) g_rk_venc_w = g_rk_cam_w;
		if (g_rk_venc_bitrate == 0)
			g_rk_venc_bitrate = g_rk_venc_h * g_rk_venc_w / 1000;

		if (g_rk_venc_type == RK_VENC_TYPE_INVALID)
			g_rk_venc_type = RK_VENC_TYPE_H264;

		if (g_rk_venc2_h == 0) g_rk_venc2_h = g_rk_cam2_h;
		if (g_rk_venc2_w == 0) g_rk_venc2_w = g_rk_cam2_w;
		if (g_rk_venc2_bitrate == 0)
			g_rk_venc2_bitrate = g_rk_venc2_h * g_rk_venc2_w / 1000;

		if (g_rk_venc2_type == RK_VENC_TYPE_INVALID)
			g_rk_venc2_type = RK_VENC_TYPE_H264;

		pApp->cam_mirror_flip = g_rk_cam_mirror_flip;
		pApp->cam2_mirror_flip = g_rk_cam2_mirror_flip;
		pApp->cam_fps = g_rk_cam_fps;
		pApp->cam2_fps = g_rk_cam2_fps;
		pApp->cam_hdr = g_rk_cam_hdr;
		pApp->cam2_hdr = g_rk_cam2_hdr;
		pApp->cam_index = g_rk_cam_index;
		pApp->cam_num = g_rk_cam_num;
		pApp->fastae_max_frame = g_rk_fastae_max_frame;
		pApp->night_mode = g_rk_night_mode;
		pApp->color_mode = g_rk_color_mode;
		pApp->led_value = g_rk_led_value;
		pApp->venc_w = g_rk_venc_w;
		pApp->venc_h = g_rk_venc_h;
		pApp->venc_type = g_rk_venc_type;
		pApp->venc_bitrate = g_rk_venc_bitrate;
		pApp->venc2_w = g_rk_venc2_w;
		pApp->venc2_h = g_rk_venc2_h;
		pApp->venc2_type = g_rk_venc2_type;
		pApp->venc2_bitrate = g_rk_venc2_bitrate;
		pApp->len = sizeof(struct app_param_info) - 4;
		pApp->crc32 = crc32(0, app_c, pApp->len);

		ret = write_item_to_buffer(meta_buf + APP_PARAM_OFFSET, app_c, APP_PARAM_MAX_SIZE);

		if (ret < 0) {
			printf("failed to write app param to meta file\n");
			goto close_file;
		}
	}
	/* App param end */


	/* Sensor AE AWB Table */
	if (access(g_ae_awb_table, R_OK) == 0) {
		uint8_t *ae_awb_table_buf = item_buf;
		memset((void *)(ae_awb_table_buf), 0xff, AE_TABLE_MAX_SIZE);

		int ae_table_buf_len = init_2a_table_buf(ae_awb_table_buf);
		if (ae_table_buf_len != 0) {
			printf("ae_table_buf_len failed\n");
			ret = -1;
			goto close_file;
		}

		ret = write_item_to_buffer(meta_buf + AE_TABLE_OFFSET, ae_awb_table_buf, AE_TABLE_MAX_SIZE);

		if (ret < 0) {
			printf("failed to write senser_init to meta file\n");
			goto close_file;
		}
	}
	/* Sensor AE AWB Table end */

	/* sensor iq bin */
	if (access(g_sensor_iq_bin, R_OK) == 0) {
		uint32_t sensor_iqbin_max_size = SENSOR_IQ_BIN_MAX_SIZE;
		g_rk_iqbin_main_offset = g_meta_load_addr + SENSOR_IQ_BIN_OFFSET + sizeof(struct sensor_iq_info);

		if (g_rk_cam_num == 2) {
			sensor_iqbin_max_size = SENSOR_IQ_BIN_MAX_SIZE * 2;
			g_rk_iqbin_secondary_offset = g_meta_load_addr + SECONDARY_SENSOR_IQ_BIN_OFFSET;
		}
		uint8_t *buf = (uint8_t *)malloc(sensor_iqbin_max_size);
		if (buf == NULL) {
			printf("malloc sensor iqbin buf failed:%s\n", strerror(errno));
			ret = -1;
			goto close_file;
		}

		memset((void *)(buf), 0xff, sensor_iqbin_max_size);

		if (g_update) {
			struct sensor_iq_info *sensor_iq_data =
				(struct sensor_iq_info *)(meta_buf + SENSOR_IQ_BIN_OFFSET);

			if (g_meta_compress_flag == META_COMPRESS_TYPE_GZ) {
				printf ( "compress\n");
				unsigned int uncomp_size = sensor_iqbin_max_size - sizeof(struct sensor_iq_info);
				ret = uncompress_gzip(
						(uint8_t *)sensor_iq_data->data,
						sensor_iq_data->len,
						(uint8_t *)(buf + sizeof(struct sensor_iq_info)),
						&uncomp_size);
				if (ret != 0) {
					printf ( "uncompress iqbin failed.\n" );
					free(buf);
					goto close_file;
				}
			} else {
				printf ( "NO compress\n");
				memcpy( (void *)(buf + sizeof(struct sensor_iq_info)), (void *)sensor_iq_data->data, sensor_iq_data->len);
			}
		}

		ret = init_sensor_iqbin(buf, sensor_iqbin_max_size);
		if (ret != 0) {
			printf("iq bin init failed\n");
			free(buf);
			goto close_file;
		}

		ret = write_item_to_buffer(meta_buf + SENSOR_IQ_BIN_OFFSET, buf, sensor_iqbin_max_size);

		free(buf);

		if (ret < 0) {
			printf("failed to write senser iq bin to meta file\n");
			ret = -1;
			goto close_file;
		}
	}
	/* sensor iq bin end */

	/* cmdline */
	if (g_cmdline_need == META_TRUE) {
		struct cmdline_info *cmdline_p = (struct cmdline_info *) item_buf;
		uint8_t *cmd_c = item_buf;
		memset((void *)(cmdline_p), 0xff, CMDLINE_MAX_SIZE);

		if (g_update) {
			uint8_t *cmd_default = (uint8_t *) (meta_buf + CMDLINE_OFFSET);
			if (g_cmdline_need_clean == META_TRUE) {
				memset(cmd_default, 0x0, CMDLINE_MAX_SIZE);
			}
			_strings_remove(cmd_default, KEY_EQUAL(META_LOAD_ADDR));
			_strings_remove(cmd_default, KEY_EQUAL(META_LOAD_SIZE));
			_strings_remove(cmd_default, KEY_EQUAL(RK_APP_PARAM_OFFSET));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_W));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_H));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM2_W));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM2_H));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_FPS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM2_FPS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_MIRROR_FLIP));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM2_MIRROR_FLIP));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_HDR));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM2_HDR));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_INDEX));
			_strings_remove(cmd_default, KEY_EQUAL(RK_FULLCOLOR_GRAY_STRINGS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_CAM_SENSOR_NUM));
			_strings_remove(cmd_default, KEY_EQUAL(RK_IQ_BIN_ADDR_STRINGS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_IQ_BIN_SIZE_STRINGS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_IQ_BIN_SECONDARY_ADDR_STRINGS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_IQ_BIN_SECONDARY_SIZE_STRINGS));
			_strings_remove(cmd_default, KEY_EQUAL(RK_NIGHT_MODE));
			_strings_remove(cmd_default, KEY_EQUAL(RK_LED_VALUE));
			_strings_remove_space(cmd_default);
			sprintf(g_cmdline+strlen(g_cmdline), "%s ",
					(cmd_default + MAX_HEAD_SIZE));
		}
		sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%08x",
				META_LOAD_ADDR,
				g_meta_load_addr);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%x",
				RK_APP_PARAM_OFFSET, APP_PARAM_OFFSET);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%08x",
				META_LOAD_SIZE,
				g_meta_part_size);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_MIRROR_FLIP,
				g_rk_cam_mirror_flip);
		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM2_MIRROR_FLIP,
				g_rk_cam2_mirror_flip);
		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_FPS,
				g_rk_cam_fps);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM2_FPS,
				g_rk_cam2_fps);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_HDR,
				g_rk_cam_hdr);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM2_HDR,
				g_rk_cam2_hdr);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_INDEX,
				g_rk_cam_index);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_NIGHT_MODE,
				g_rk_night_mode);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_LED_VALUE,
				g_rk_led_value);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_SENSOR_NUM,
				g_rk_cam_num);

		sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%x",
				RK_FULLCOLOR_GRAY_STRINGS,
				(uint32_t)(g_meta_load_addr + APP_PARAM_OFFSET
					+ offsetof(struct app_param_info, color_mode)));

		sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%08x",
				RK_IQ_BIN_ADDR_STRINGS,
				g_rk_iqbin_main_offset);
		sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%08x",
				RK_IQ_BIN_SIZE_STRINGS,
				g_rk_iqbin_main_size);

		if (g_rk_cam_num == 2) {
			sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%08x",
					RK_IQ_BIN_SECONDARY_ADDR_STRINGS,
					g_rk_iqbin_secondary_offset);
			sprintf(g_cmdline+strlen(g_cmdline), " %s=0x%08x",
					RK_IQ_BIN_SECONDARY_SIZE_STRINGS,
					g_rk_iqbin_secondary_size);
		}

		if (g_rk_cam_h != 0) {
		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_H,
				g_rk_cam_h);
		}

		if (g_rk_cam_w != 0) {
		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM_W,
				g_rk_cam_w);
		}

		if (g_rk_cam2_h != 0) {
		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM2_H,
				g_rk_cam2_h);
		}

		if (g_rk_cam2_w != 0) {
		sprintf(g_cmdline+strlen(g_cmdline), " %s=%d",
				RK_CAM2_W,
				g_rk_cam2_w);
		}

		cmd_c[0] = 'C';
		cmd_c[1] = 'M';
		cmd_c[2] = 'D';
		cmd_c[3] = 'L';
		strcpy((void *)(cmd_c + MAX_HEAD_SIZE), g_cmdline);
		cmdline_p->crc32 = crc32(0, cmd_c, sizeof(struct cmdline_info)-4);

		ret = write_item_to_buffer(meta_buf + CMDLINE_OFFSET, (uint8_t *)cmdline_p, CMDLINE_MAX_SIZE);

		if (ret < 0) {
			printf("failed to write cmdline to meta file\n");
			goto close_file;
		}
	}
	/* cmdline end */

	if (g_create) {
		update_meta_head(meta_buf);
		memcpy(meta_buf + BACKUP_META_SIZE, meta_buf + META_INFO_SIZE, BACKUP_META_SIZE - ITEM_SIZE);

		lseek(meta_fd, 0, SEEK_SET);
		ret = write(meta_fd, meta_buf, meta_len);
		if (ret != meta_len) {
			printf("write meta faled\n");
		}
	} else if (g_update) {
		if (access(g_sensor_iq_bin, R_OK) == 0) {
			update_meta_head(meta_buf);
		}

		/* If the current part is the last one, reverse flag. */
		if (g_meta_total_part_num > 1 && g_meta_startup_part_num == g_meta_total_part_num) {
			struct meta_head *meta_head_p = (struct meta_head *)meta_buf;
			meta_head_p->part_flag = (meta_head_p->part_flag == 0) ? 1 : 0;
			meta_head_p->crc32 = crc32(0, (unsigned char *)meta_head_p, sizeof(struct meta_head) - 4 - 4);
		}

		uint32_t meta_next_startup_offset = (g_meta_startup_part_num % g_meta_total_part_num) * g_meta_per_part_size;
		if (g_is_nand_flash == META_TRUE) {
			ret = flash_write_buf(g_meta_path, (char *)meta_buf, meta_next_startup_offset, g_meta_part_size);
			if (ret) {
				printf("write failed, ret=%d\n", ret);
				goto close_file;
			}
		} else {
			ret = write_item_to_file(meta_fd, META_INFO_HEAD_OFFSET + meta_next_startup_offset, (uint8_t *)meta_buf, g_meta_part_size);
			if (ret < 0) {
				printf("failed to update meta_head to meta file\n");
				goto close_file;
			}
		}
	}


close_file:
	if (meta_fd) {
		close(meta_fd);
	}
free_buf2:
	if (item_buf) {
		free(item_buf);
	}
free_buf:
	if (meta_buf) {
		free(meta_buf);
	}

    return ret;
}

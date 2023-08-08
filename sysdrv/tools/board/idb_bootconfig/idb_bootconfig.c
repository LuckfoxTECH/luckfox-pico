#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#ifdef RV1106_RV1103
#define	BOOTCONFIG_SPI_NOR_MISC_DEV  "/dev/nor_misc_spi2.0"
#else
#define	BOOTCONFIG_SPI_NOR_MISC_DEV NULL
#endif

#ifndef _UAPI__SPI_NOR_MISC_H__
#define _UAPI__SPI_NOR_MISC_H__

#include <linux/types.h>

#define SPI_NOR_MAX_ID_LEN	6

struct nor_flash_user_info {
	__u8	id[SPI_NOR_MAX_ID_LEN];
};

#define NOR_BASE	'P'
#define NOR_GET_FLASH_INFO		_IOR(NOR_BASE, 0, struct nor_flash_user_info)
#endif

#define BIT(n)			(1 << n)
typedef unsigned char           u8;
typedef signed char             s8;
typedef unsigned short          u16;
typedef signed short            s16;
typedef unsigned int            u32;
typedef signed int              s32;
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#define NOR_PAGE_SIZE		256
#define NOR_BLOCK_SIZE		(64 * 1024)
#define NOR_SECS_BLK		(NOR_BLOCK_SIZE / 512)
#define NOR_SECS_PAGE		8

#define FEA_READ_STATUE_MASK	(0x3 << 0)
#define FEA_STATUE_MODE0	0		/* Readstatus0 05h-SR1, 35h-SR2, Writestatus0 01h-SR1, 31h-SR2 */
#define FEA_STATUE_MODE1	1		/* Readstatus0 05h-SR1, 35h-SR2, Writestatus1 01h-SR1-SR2 */
#define FEA_STATUE_MODE2	2		/* Readstatus1 05h-SR1, 15h-CR1, Writestatus1 01h-SR1-CR1 */
#define FEA_4BIT_READ		BIT(2)
#define FEA_4BIT_PROG		BIT(3)
#define FEA_4BYTE_ADDR		BIT(4)
#define FEA_4BYTE_ADDR_MODE	BIT(5)

/*Command Set*/
#define CMD_READ_JEDECID        (0x9F)
#define CMD_READ_DATA           (0x03)
#define CMD_READ_STATUS         (0x05)
#define CMD_WRITE_STATUS        (0x01)
#define CMD_PAGE_PROG           (0x02)
#define CMD_SECTOR_ERASE        (0x20)
#define CMD_BLK64K_ERASE        (0xD8)
#define CMD_BLK32K_ERASE        (0x52)
#define CMD_CHIP_ERASE          (0xC7)
#define CMD_WRITE_EN            (0x06)
#define CMD_WRITE_DIS           (0x04)
#define CMD_PAGE_READ           (0x13)
#define CMD_PAGE_FASTREAD4B     (0x0C)
#define CMD_GET_FEATURE         (0x0F)
#define CMD_SET_FEATURE         (0x1F)
#define CMD_PROG_LOAD           (0x02)
#define CMD_PROG_EXEC           (0x10)
#define CMD_BLOCK_ERASE         (0xD8)
#define CMD_READ_DATA_X2        (0x3B)
#define CMD_READ_DATA_X4        (0x6B)
#define CMD_PROG_LOAD_X4        (0x32)
#define CMD_READ_STATUS2        (0x35)
#define CMD_READ_STATUS3        (0x15)
#define CMD_WRITE_STATUS2       (0x31)
#define CMD_WRITE_STATUS3       (0x11)
/* X1 cmd, X1 addr, X1 data */
#define CMD_FAST_READ_X1        (0x0B)
/* X1 cmd, X1 addr, X2 data */
#define CMD_FAST_READ_X2        (0x3B)
/* X1 cmd, X1 addr, X4 data SUPPORT GD MARCONIX WINBOND */
#define CMD_FAST_READ_X4        (0x6B)
/* X1 cmd, X1 addr, X4 data SUPPORT GD MARCONIX WINBOND */
#define CMD_FAST_4READ_X4       (0x6C)
/* X1 cmd, X4 addr, X4 data SUPPORT EON GD MARCONIX WINBOND */
#define CMD_FAST_READ_A4        (0xEB)
/* X1 cmd, X1 addr, X4 data, SUPPORT GD WINBOND */
#define CMD_PAGE_PROG_X4        (0x32)
/* X1 cmd, X4 addr, X4 data, SUPPORT MARCONIX */
#define CMD_PAGE_PROG_A4        (0x38)
/* X1 cmd, X4 addr, X4 data, SUPPORT MARCONIX */
#define CMD_PAGE_PROG_4PP       (0x3E)
#define CMD_RESET_NAND          (0xFF)
#define CMD_ENTER_4BYTE_MODE    (0xB7)
#define CMD_EXIT_4BYTE_MODE     (0xE9)
#define CMD_ENABLE_RESER	(0x66)
#define CMD_RESET_DEVICE	(0x99)
#define CMD_READ_PARAMETER	(0x5A)

#define IDB_BLOCK_TAG_ID1	0xFCDC8C3B
#define IDB_BLOCK_TAG_ID2	0x534E4B52
#define IDB_BLOCK_TAG_ID3	0x53534B52

enum NOR_ERASE_TYPE {
	ERASE_SECTOR = 0,
	ERASE_BLOCK64K,
	ERASE_CHIP
};

enum SNOR_IO_MODE {
	IO_MODE_SPI = 0,
	IO_MODE_QPI
};

enum SNOR_READ_MODE {
	READ_MODE_NOMAL = 0,
	READ_MODE_FAST
};

enum SNOR_ADDR_MODE {
	ADDR_MODE_3BYTE = 0,
	ADDR_MODE_4BYTE
};

/* Manufactory ID */
#define MID_WINBOND	0xEF
#define MID_GIGADEV	0xC8
#define MID_MICRON	0x2C
#define MID_MACRONIX	0xC2
#define MID_SPANSION	0x01
#define MID_EON		0x1C
#define MID_ST		0x20
#define MID_XTX		0x0B
#define MID_PUYA	0x85
#define MID_XMC		0x20
#define MID_DOSILICON	0xF8
#define MID_ZBIT	0x5E

/*------------------------------ Global Typedefs -----------------------------*/
enum SFC_DATA_LINES {
	DATA_LINES_X1 = 0,
	DATA_LINES_X2,
	DATA_LINES_X4
};

typedef int (*SNOR_WRITE_STATUS)(u32 reg_index, u8 status);

struct flash_info {
	u32 id;

	u8 block_size;
	u8 sector_size;
	u8 read_cmd;
	u8 prog_cmd;

	u8 read_cmd_4;
	u8 prog_cmd_4;
	u8 sector_erase_cmd;
	u8 block_erase_cmd;

	u8 feature;
	u8 density;  /* (1 << density) sectors*/
	u8 QE_bits;
	u8 reserved2;
};

struct SFNOR_DEV {
	u32	capacity;
	u8	manufacturer;
	u8	mem_type;
	u16	page_size;
	u32	blk_size;

	u8	read_cmd;
	u8	prog_cmd;
	u8	sec_erase_cmd;
	u8	blk_erase_cmd;
	u8	QE_bits;

	enum SNOR_READ_MODE  read_mode;
	enum SNOR_ADDR_MODE  addr_mode;
	enum SNOR_IO_MODE    io_mode;

	enum SFC_DATA_LINES read_lines;
	enum SFC_DATA_LINES prog_lines;
	enum SFC_DATA_LINES prog_addr_lines;

	SNOR_WRITE_STATUS write_status;
	u32 max_iosize;
};

struct snor_info_packet {
	u32 id;
	u32 head_hash; /*hash for head, check by bootrom.*/
	u16 head_len;  /*320 - 16 bytes*/
	u16 version;
	u8 read_cmd;
	u8 prog_cmd;
	u8 read_cmd_4;
	u8 prog_cmd_4;

	u8 sector_erase_cmd;
	u8 block_erase_cmd;
	u8 feature;
	u8 QE_bits;

	u32 spi_mode;
};

static struct flash_info spi_flash_tbl[] = {
	/* GD25Q40B */
	{ 0xc84013, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x05, 10, 9, 0 },
	/* GD25Q32B */
	{ 0xc84016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 13, 9, 0 },
	/* GD25Q64B/C/E */
	{ 0xc84017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
	/* GD25Q127C and GD25Q128C/E */
	{ 0xc84018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* GD25Q256B/C/D/E */
	{ 0xc84019, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1C, 16, 6, 0 },
	/* GD25Q512MC */
	{ 0xc84020, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1C, 17, 6, 0 },
	/* GD25LQ64C */
	{ 0xc86017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
	/* GD25LQ128 */
	{ 0xc86018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 9, 0 },
	/* GD25LQ32E */
	{ 0xc86016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 13, 9, 0 },
	/* GD25B512MEYIG */
	{ 0xc8471A, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x1C, 17, 0, 0 },
	/* GD25LQ255E and GD25LQ256C */
	{ 0xc86019, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x1D, 16, 9, 0 },
	/* GD25LB512MEYIG */
	{ 0xc8671A, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x1C, 17, 0, 0 },

	/* W25Q32JV */
	{ 0xef4016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
	/* W25Q64JVSSIQ */
	{ 0xef4017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
	/* W25Q128FV and W25Q128JV*/
	{ 0xef4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* W25Q256F/J */
	{ 0xef4019, 128, 8, 0x13, 0x02, 0x6C, 0x32, 0x20, 0xD8, 0x3C, 16, 9, 0 },
	/* W25Q32JW */
	{ 0xef6016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
	/* W25Q64FWSSIG */
	{ 0xef6017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
	/* W25Q128JWSQ */
	{ 0xef6018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* W25Q256JWEQ*/
	{ 0xef6019, 128, 8, 0x13, 0x02, 0x6C, 0x32, 0x20, 0xD8, 0x3C, 16, 9, 0 },
	/* W25Q128JVSIM */
	{ 0xef7018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* W25Q256JVEM */
	{ 0xef7019, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x3C, 16, 9, 0 },

	/* MX25L3233FM2I-08G */
	{ 0xc22016, 128, 8, 0x03, 0x02, 0x6B, 0x38, 0x20, 0xD8, 0x0E, 13, 6, 0 },
	/* MX25L6433F */
	{ 0xc22017, 128, 8, 0x03, 0x02, 0x6B, 0x38, 0x20, 0xD8, 0x0E, 14, 6, 0 },
	/* MX25L12835E/F MX25L12833FMI-10G */
	{ 0xc22018, 128, 8, 0x03, 0x02, 0x6B, 0x38, 0x20, 0xD8, 0x0E, 15, 6, 0 },
	/* MX25L25635E/F MX25L25645G MX25L25645GMI-08G */
	{ 0xc22019, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1E, 16, 6, 0 },
	/* MX25L51245GMI */
	{ 0xc2201a, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1E, 17, 6, 0 },
	/* MX25U51245G */
	{ 0xc2253a, 128, 8, 0x0C, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1E, 17, 6, 0 },
	/* MX25U3232F */
	{ 0xc22536, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0E, 13, 6, 0 },
	/* MX25U6432F */
	{ 0xc22537, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0E, 14, 6, 0 },
	/* MX25U12832F */
	{ 0xc22538, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0E, 15, 6, 0 },
	/* MX25U25645GZ4I-00 */
	{ 0xc22539, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1E, 16, 6, 0 },

	/* XM25QH32C */
	{ 0x204016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
	/* XM25QH64C */
	{ 0x204017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
	/* XM25QH128C */
	{ 0x204018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x05, 15, 9, 0 },
	/* XM25QH256C */
	{ 0x204019, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x1C, 16, 9, 0 },
	/* XM25QH64B */
	{ 0x206017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 6, 0 },
	/* XM25QH128B */
	{ 0x206018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 6, 0 },
	/* XM25QH(QU)256B */
	{ 0x206019, 128, 8, 0x13, 0x12, 0x6C, 0x3E, 0x21, 0xDC, 0x1D, 16, 6, 0 },
	/* XM25QH64A */
	{ 0x207017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 0, 0 },
	/* XM25QU128C */
	{ 0x204118, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* XM25QU64C */
	{ 0x204117, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

	/* XT25F128A XM25QH128A */
	{ 0x207018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 0, 0 },
	/* XT25F64BSSIGU-5 XT25F64F */
	{ 0x0b4017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
	/* XT25F128BSSIGU */
	{ 0x0b4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 9, 0 },
	/* XT25F256BSFIGU */
	{ 0x0b4019, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x1C, 16, 9, 0 },
	/* XT25F32BS XT25F32F */
	{ 0x0b4016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 13, 9, 0 },
	/* XT25F16BS */
	{ 0x0b4015, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 12, 9, 0 },

	/* EN25QH64A */
	{ 0x1c7017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 0, 0 },
	/* EN25QH128A */
	{ 0x1c7018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 0, 0 },
	/* EN25QH32B */
	{ 0x1c7016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 0, 0 },
	/* EN25S32A */
	{ 0x1c3816, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 0, 0 },
	/* EN25S64A */
	{ 0x1c3817, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 0, 0 },
	/* EN25QH256A */
	{ 0x1c7019, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x3C, 16, 0, 0 },

	/* P25Q64H */
	{ 0x856017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
	/* P25Q128H */
	{ 0x856018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* P25Q16H-SUH-IT */
	{ 0x856015, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 12, 9, 0 },
	/* P25Q32SL P25Q32SH-SSH-IT */
	{ 0x856016, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 13, 9, 0 },
	/* PY25Q128H */
	{ 0x852018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },

	/* ZB25VQ64 */
	{ 0x5e4017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },
	/* ZB25VQ128 */
	{ 0x5e4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* ZB25LQ128 */
	{ 0x5e5018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },

	/* BH25Q128AS */
	{ 0x684018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* BH25Q64BS */
	{ 0x684017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

	/* FM25Q128A */
	{ 0xA14018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },
	/* FM25Q64-SOB-T-G */
	{ 0xA14017, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 14, 9, 0 },

	/* FM25Q64A */
	{ 0xf83217, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },
	/* FM25M4AA */
	{ 0xf84218, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 15, 9, 0 },
	/* FM25M64C */
	{ 0xf84317, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0D, 14, 9, 0 },

	/* DS25M4AB-1AIB4 */
	{ 0xe54218, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },

	/* GM25Q128A */
	{ 0x1c4018, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 9, 0 },

	/* IS25LP512M */
	{ 0x9D601A, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x3C, 17, 6, 0 },
	/* IS25WP512M */
	{ 0x9D701A, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x3C, 17, 6, 0 },

	/* BY25Q256FSEIG */
	{ 0x684919, 128, 8, 0x13, 0x12, 0x6C, 0x34, 0x21, 0xDC, 0x1C, 16, 9, 0 },

	/* NM25Q128EVB */
	{ 0x522118, 128, 8, 0x03, 0x02, 0x6B, 0x32, 0x20, 0xD8, 0x0C, 15, 10, 0 },
};

#define SNOR_INFO_PACKET_ID_NEW_IDB	0x524F4E53
#define SNOR_INFO_PACKET_HEAD_LEN	14
#define SNOR_INFO_PACKET_SPI_MODE_RATE_SHIFT	25

static struct flash_info *snor_get_flash_info(u8 *flash_id)
{
	u32 i;
	u32 id = (flash_id[0] << 16) | (flash_id[1] << 8) | (flash_id[2] << 0);

	for (i = 0; i < ARRAY_SIZE(spi_flash_tbl); i++) {
		if (spi_flash_tbl[i].id == id)
			return &spi_flash_tbl[i];
	}
	return NULL;
}

static int snor_parse_flash_table(struct SFNOR_DEV *p_dev,
				  struct flash_info *g_spi_flash_info)
{
	int i, ret;
	u8 status;

	if (g_spi_flash_info) {
		p_dev->manufacturer = (g_spi_flash_info->id >> 16) & 0xFF;
		p_dev->mem_type = (g_spi_flash_info->id >> 8) & 0xFF;
		p_dev->capacity = 1 << g_spi_flash_info->density;
		p_dev->blk_size = g_spi_flash_info->block_size;
		p_dev->page_size = NOR_SECS_PAGE;
		p_dev->read_cmd = g_spi_flash_info->read_cmd;
		p_dev->prog_cmd = g_spi_flash_info->prog_cmd;
		p_dev->sec_erase_cmd = g_spi_flash_info->sector_erase_cmd;
		p_dev->blk_erase_cmd = g_spi_flash_info->block_erase_cmd;
		p_dev->prog_lines = DATA_LINES_X1;
		p_dev->read_lines = DATA_LINES_X1;
		p_dev->QE_bits = g_spi_flash_info->QE_bits;
		p_dev->addr_mode = ADDR_MODE_3BYTE;

		i = g_spi_flash_info->feature & FEA_READ_STATUE_MASK;
		if (g_spi_flash_info->feature & FEA_4BIT_READ) {
			ret = 0;
			p_dev->read_lines = DATA_LINES_X4;
			p_dev->read_cmd = g_spi_flash_info->read_cmd_4;
		}
		if (g_spi_flash_info->feature & FEA_4BIT_PROG &&
		    p_dev->read_lines == DATA_LINES_X4) {
			p_dev->prog_lines = DATA_LINES_X4;
			p_dev->prog_cmd = g_spi_flash_info->prog_cmd_4;
			if ((p_dev->manufacturer == MID_MACRONIX) &&
			    (p_dev->prog_cmd == CMD_PAGE_PROG_A4 ||
			     p_dev->prog_cmd == CMD_PAGE_PROG_4PP))
				p_dev->prog_addr_lines = DATA_LINES_X4;
		}

		if (g_spi_flash_info->feature & FEA_4BYTE_ADDR)
			p_dev->addr_mode = ADDR_MODE_4BYTE;
	}

	return 0;
}

void boot_config_print(void *buf)
{
	u32 i, j;
	u32 *temp = (u32 *) buf;

	j = 0;

	printf("[BOOT0_PARAM]\n");
	for (i = 0; i < 6; i++) {
		printf("WORD_%d=0x%x", i, temp[i]);
		printf("\n");
	}
	for (i = 6; i < 10; i++) {
		printf("WORD_%d=0x%x", i, 0);
		printf("\n");
	}

	printf("\n");
}

u8 parse_id_byte(char *str)
{
	u8 i, cnt = 0;
	char *p = str;
	u8 len = strlen(str);
	u32 temp;
	u8 sum = 0;

	if (len > 2)
		printf("wrong id %s\n", str);

	while (*p != '\0') {
		for (i = 0; i < len; i++) {
			if ((*p >= '0') && (*p <= '9'))
				temp = *p - '0';

			if ((*p >= 'A') && (*p <= 'Z'))
				temp = *p - 'A' + 10;

			if ((*p >= 'a') && (*p <= 'z'))
				temp = *p - 'a' + 10;

			if (i == 0)
				temp *= 16;
			sum += temp;
			p++;
			cnt++;
		}
	}

	return sum & 0xff;
}


int get_idb_bootconfig(struct snor_info_packet *packet, char *id_byte)
{
	struct SFNOR_DEV p_dev;
	struct flash_info *g_spi_flash_info;

	g_spi_flash_info = snor_get_flash_info(id_byte);
	if (g_spi_flash_info) {
		snor_parse_flash_table(&p_dev, g_spi_flash_info);
	} else {
		printf("The device not support yet! JEDEC id:%x %x %x\n", id_byte[0], id_byte[1], id_byte[2]);

		return -1;
	}

	memset(packet, 0, sizeof(struct snor_info_packet));
	if (g_spi_flash_info && g_spi_flash_info->feature & FEA_4BIT_READ) {
		packet->id = SNOR_INFO_PACKET_ID_NEW_IDB;
		packet->read_cmd = g_spi_flash_info->read_cmd;
		packet->read_cmd_4 = CMD_FAST_READ_X4; /* bootrom support */
		packet->prog_cmd = g_spi_flash_info->prog_cmd;
		packet->prog_cmd_4 = g_spi_flash_info->prog_cmd_4;
		packet->sector_erase_cmd = g_spi_flash_info->sector_erase_cmd;
		packet->block_erase_cmd = g_spi_flash_info->block_erase_cmd;
		packet->feature = g_spi_flash_info->feature;
		packet->QE_bits = g_spi_flash_info->QE_bits;
		packet->spi_mode = 1 << SNOR_INFO_PACKET_SPI_MODE_RATE_SHIFT;
		packet->head_len = SNOR_INFO_PACKET_HEAD_LEN;
	}

	//boot_config_print(packet);

	return 0;
}

void print_hex(char *s, void *buf, u_int32_t width, u_int32_t len)
{
	u_int32_t i, j;
	unsigned char *p8 = (unsigned char *) buf;
	unsigned short *p16 = (unsigned short *) buf;
	u_int32_t *p32 = (u_int32_t *) buf;

	j = 0;

	for (i = 0; i < len; i++) {
		if (j == 0)
			printf("%s %p + 0x%x:", s, buf, i * width);

		if (width == 4)
			printf("0x%08x,", p32[i]);
		else if (width == 2)
			printf("0x%04x,", p16[i]);
		else
			printf("0x%02x,", p8[i]);

		if (++j >= (16 / width)) {
			j = 0;
			printf("\n", "");
		}
	}

	printf("\n", "");
}

void P_RC4(unsigned char *buf, unsigned short len)
{
	unsigned char S[256], K[256], temp;
	unsigned short i, j, t, x;
	unsigned char key[16] = {124, 78, 3, 4, 85, 5, 9, 7, 45, 44, 123, 56, 23, 13, 23, 17};

	j = 0;
	for (i = 0; i < 256; i++) {
		S[i] = (unsigned char)i;
		j &= 0x0f;
		K[i] = key[j];
		j++;
	}

	j = 0;
	for (i = 0; i < 256; i++) {
		j = (j + S[i] + K[i]) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
	}

	i = j = 0;
	for (x = 0; x < len; x++) {
		i = (i+1) % 256;
		j = (j + S[i]) % 256;
		temp = S[i];
		S[i] = S[j];
		S[j] = temp;
		t = (S[i] + (S[j] % 256)) % 256;
		buf[x] = buf[x] ^ S[t];
	}
}

void show_help(void)
{
	printf("Input error, input idblock image like:");
	printf("          idb_bootconfig /path_to_idblock.img\n");
	printf("(default path_to_idblock.img is /dev/block/by-name/idblock)\n");
}

int main(int argc, char *argv[])
{
	int fd;
	struct nor_flash_user_info *info;
	struct snor_info_packet *packet;
	int i, ret;
	char *p;
	FILE *file;
	char *buffer;
	long file_size = 4096;
	char *dev_path = BOOTCONFIG_SPI_NOR_MISC_DEV;

	p = argv[1];
	if (argv[1] == NULL) {
		p = "/dev/block/by-name/idblock";
	}

	printf("idb bootconfig tool\n");

	file = fopen(p, "rb+");
	if (!file) {
		printf("open file %s failed\n", p);
		show_help();
		return -1;
	}

	fseek(file, 0L, SEEK_END);
	buffer = (char *)malloc(file_size * sizeof(char));
	if (!buffer) {
		printf("malloc buffer failed\n");
		fclose(file);
		return -1;
	}

	/*
	 * Check if idb bootconfig is exit.
	 */
	fseek(file, 0L, SEEK_SET);
	fread(buffer, sizeof(char), file_size, file);
	if (((u32 *)(buffer + 0x30))[0] == SNOR_INFO_PACKET_ID_NEW_IDB) {
		printf("idb bootconfig already config\n");
		goto skip_out;
	}

	if (((u32 *)(buffer + 0x00))[0] != IDB_BLOCK_TAG_ID2) {
		printf("idb bootconfig fail, not a supported idb tag %x\n", ((u32 *)buffer)[0]);
		goto skip_out;
	}

	info = malloc(sizeof(struct nor_flash_user_info));
	if (!info) {
		printf("malloc info failed\n");
		goto skip_out;
	}

	packet = malloc(sizeof(struct snor_info_packet));
	if (!packet) {
		printf("malloc packet failed\n");
		free(packet);
		goto skip_out;
	}

	//print_hex("idb", buffer, 4, 0x40);

	/*
	 * Check if mtd misc exit, and add bootconfig
	 */
	fd = open(dev_path, O_RDWR);
	if (fd < 0) {
		printf("failed to open %s\n", dev_path);
		goto out;
	}
	ret = ioctl(fd, NOR_GET_FLASH_INFO, info);
	if (!ret && info->id[0]) {
		ret = get_idb_bootconfig(packet, info->id);
		if (!ret) {
			memcpy(buffer + 0x30, packet, sizeof(struct snor_info_packet));
			fseek(file, 0L, SEEK_SET);
			fwrite(buffer, sizeof(char), file_size, file);
			//print_hex("buffer", buffer, 4,  0x40);
			fflush(file);
			printf("idb bootconfig success, %s %d\n", p, file_size);
		}
	} else {
		printf("idb bootconfig fail\n");
	}
out:
	free(packet);
	free(info);
skip_out:
	free(buffer);
	fclose(file);
	// close(fd);

	return ret;
}

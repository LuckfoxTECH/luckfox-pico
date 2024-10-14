/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <boot_rkimg.h>
#include <environment.h>
#include <memalign.h>
#include <part.h>
#include <part_efi.h>
#include <asm/unaligned.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * Example: ./tools/mkenvimage -s 0x8000 -p 0x0 -o env.img env.txt
 */
#define ENVF_MSG(fmt, args...)	printf("ENVF: "fmt, ##args)
#define ENVF_DBG(fmt, args...)	debug("ENVF: "fmt, ##args)

#define EMSG_ARGS		"error: please use \"sys_bootargs\" but not \"bootargs\""
#define BLK_CNT(desc, sz)	((sz) / (desc)->blksz)
#define ENVF_MAX		64

/*
 * env_dev maybe: boot-device, sdcard.
 *
 * env_size/offset/offset_redund should be updated when env_dev changed.
 */
static u32 env_dev;
static ulong env_size, env_offset, env_offset_redund;

/*
 * In case of env and env-backup partitions are too large that exceeds the limit
 * of CONFIG_SPL_SYS_MALLOC_F_LEN. we prefer to use a static address as an env
 * buffer. The tail of bss section is good choice from experience.
 */
#ifdef CONFIG_SPL_BUILD
static void *spl_env =
	(void *)CONFIG_SPL_BSS_START_ADDR + CONFIG_SPL_BSS_MAX_SIZE;
#else
static u32 envf_num;
static const char *envf_list[ENVF_MAX];
#endif

#ifdef CONFIG_DM_MMC
static int is_pmbr_valid(legacy_mbr * mbr)
{
	int i = 0;

	if (!mbr || le16_to_cpu(mbr->signature) != MSDOS_MBR_SIGNATURE)
		return 0;

	for (i = 0; i < 4; i++) {
		if (mbr->partition_record[i].sys_ind == 0xc)
			return 1;
	}

	return 0;
}

static int can_find_pmbr(struct blk_desc *dev_desc)
{
	ALLOC_CACHE_ALIGN_BUFFER_PAD(legacy_mbr, legacymbr, 1, dev_desc->blksz);

	/* Read legacy MBR from block 0 and validate it */
	if ((blk_dread(dev_desc, 0, 1, (ulong *)legacymbr) != 1)
		|| (is_pmbr_valid(legacymbr) != 1)) {
		return 0;
	}

	return 1;
}
#endif

static void envf_init_location(struct blk_desc *desc)
{
	if (env_dev == ((desc->if_type << 8) | desc->devnum))
		return;

	/* eMMC (default) */
	env_size = CONFIG_ENV_SIZE;
	env_offset = CONFIG_ENV_OFFSET;
	env_offset_redund = CONFIG_ENV_OFFSET_REDUND;

#if defined(CONFIG_MTD_SPI_NAND) || defined(CONFIG_CMD_NAND)
	/* nand or spi-nand */
	if (desc->if_type == IF_TYPE_MTD &&
	    (desc->devnum == BLK_MTD_SPI_NAND || desc->devnum == BLK_MTD_NAND)) {
		env_size = CONFIG_ENV_NAND_SIZE;
		env_offset = CONFIG_ENV_NAND_OFFSET;
		env_offset_redund = CONFIG_ENV_NAND_OFFSET_REDUND;
	}
#endif
#if defined(CONFIG_SPI_FLASH)
	/* spi-nor */
	if (desc->if_type == IF_TYPE_MTD && desc->devnum == BLK_MTD_SPI_NOR) {
		env_size = CONFIG_ENV_NOR_SIZE;
		env_offset = CONFIG_ENV_NOR_OFFSET;
		env_offset_redund = CONFIG_ENV_NOR_OFFSET_REDUND;
	}
#endif
	if (env_offset == env_offset_redund)
		env_offset_redund = 0;

	env_dev = (desc->if_type << 8) | desc->devnum;
}

static int env_read(struct blk_desc *desc, u32 offset, u32 size, env_t **envp)
{
	lbaint_t data_size;
	lbaint_t blk_off;
	lbaint_t blk_cnt;
	env_t *env;
	int ret;

#ifdef CONFIG_SPL_BUILD
	env = spl_env;
#else
	env = malloc(size);
	if (!env)
		return -ENOMEM;
#endif
	data_size = size - ENV_HEADER_SIZE;
	blk_off = BLK_CNT(desc, offset);
	blk_cnt = BLK_CNT(desc, size);

	if (blk_dread(desc, blk_off, blk_cnt, (void *)env) != blk_cnt) {
		ret = -EIO;
		goto fail;
	}

	if (crc32(0, env->data, data_size) != env->crc) {
		ENVF_MSG("!bad CRC @ 0x%x\n", offset);
		ret = -EINVAL;
		goto fail;
	}

	*envp = env;

	return 0;
fail:
#ifndef CONFIG_SPL_BUILD
	free(env);
#endif
	return ret;
}

static __maybe_unused env_t *envf_read(struct blk_desc *desc)
{
	env_t *env = NULL;
	int ret;

	if (!desc)
		return NULL;

	envf_init_location(desc);

#ifdef CONFIG_DM_MMC
	/* SD upgrade card: LBA0 is MBR, and LBA1 is env.img with 16KB size */
	if (desc->if_type == IF_TYPE_MMC && desc->devnum == 1 &&
	    !env_offset && can_find_pmbr(desc)) {
		env_offset = 512;
		env_size = SZ_16K;
	}
#endif
	ret = env_read(desc, env_offset, env_size, &env);
	if (ret < 0 && env_offset_redund)
		ret = env_read(desc, env_offset_redund, env_size, &env);

	return env;
}

#if CONFIG_IS_ENABLED(ENV_PARTITION)
static const char *env_get_string(env_t *env, u32 size, const char *str)
{
	const char *dp;
	u32 env_size;

#if CONFIG_IS_ENABLED(FIT_SIGNATURE)
	/* Do nothing, ignore 'sys_bootargs' from env.img */
	if (!strcmp(str, "sys_bootargs"))
		return NULL;
#endif

	dp = (const char *)env->data;
	env_size = size - ENV_HEADER_SIZE;
	do {
		/* skip leading white space */
		while (*dp == ' ' || *dp == '\t')
			++dp;

		debug("ENTRY: %s\n", dp);
		if (strstr(dp, str)) {
			debug("FIND: %s\n", dp);
			return dp;
		}

		/* point to next ENTRY */
		dp += strlen(dp) + 1;
	} while (((ulong)dp < (ulong)env->data + env_size) && *dp);

	debug("NOT-FIND: %s\n", str);

	return NULL;
}

char *envf_get(struct blk_desc *desc, const char *name)
{
	const char *list = NULL;
	static env_t *env; /* static */

	if (!env)
		env = envf_read(desc);
	if (!env)
		goto out;

	ENVF_MSG("Primary 0x%08lx - 0x%08lx\n", env_offset, env_offset + env_size);
	if (env_offset_redund)
		ENVF_MSG("Backup  0x%08lx - 0x%08lx\n",
			 env_offset_redund, env_offset_redund + env_size);

	list = env_get_string(env, env_size, name);
	if (!list)
		ENVF_DBG("Unavailable env %s\n", name);
out:
	return (char *)list;
}
#endif

#ifndef CONFIG_SPL_BUILD
static int envf_init_vars(void)
{
	char *tok, *p;

	tok = strdup(CONFIG_ENVF_LIST);
	if (!tok)
		return 0;

	envf_num = 0;
	p = strtok(tok, " ");
	while (p && envf_num < ENVF_MAX) {
		if (!strcmp(p, "bootargs")) {
			printf("%s\n", EMSG_ARGS);
			run_command("download", 0);
#if CONFIG_IS_ENABLED(FIT_SIGNATURE)
		} else if (!strcmp(p, "sys_bootargs")) {
			/* Do nothing, ignore 'sys_bootargs' from env.img */
#endif
		} else {
			envf_list[envf_num++] = p;
		}

		p = strtok(NULL, " ");
	}

	return envf_num;
}

static int envf_load(void)
{
	struct blk_desc *desc;
	env_t *env;

	desc = rockchip_get_bootdev();
	if (!desc) {
		printf("dev desc null!\n");
		return 0;
	}

	env = envf_read(desc);
	if (!env)
		return -EINVAL;

	if (envf_init_vars() > 0) {
		if (!himport_r(&env_htab, (char *)env->data, env_size, '\0',
			H_NOCLEAR, 0, envf_num, (char * const *)envf_list)) {
			ENVF_MSG("envf himport error: %d\n", errno);
			return -EINTR;
		}
	}

	return 0;
}

static int envf_save(void)
{
	ALLOC_CACHE_ALIGN_BUFFER(env_t, env, 1);
	struct blk_desc *desc;
	ssize_t	len;
	u32 blk_cnt;
	char *res;
	int ret = 0;

	desc = rockchip_get_bootdev();
	if (!desc) {
		printf("dev desc null!\n");
		return -EINVAL;
	}

	envf_init_location(desc);

	res = (char *)env->data;
	len = hexport_r(&env_htab, '\0', H_MATCH_KEY | H_MATCH_IDENT,
			&res, env_size - ENV_HEADER_SIZE,
			envf_num, (char * const *)envf_list);
	if (len < 0) {
		ENVF_MSG("hexpor error: %d\n", errno);
		return -EINVAL;
	}

	env->crc = crc32(0, env->data, env_size - ENV_HEADER_SIZE);
	blk_cnt = BLK_CNT(desc, env_size);
	if (blk_dwrite(desc, BLK_CNT(desc, env_offset),
		       blk_cnt, (char *)env) != blk_cnt) {
		ret = -EIO;
		ENVF_MSG("io error\n");
	}

	if (env_offset_redund) {
		if (blk_dwrite(desc, BLK_CNT(desc, env_offset_redund),
			       blk_cnt, (char *)env) != blk_cnt)
			ENVF_MSG("redundant: io error\n");
		else
			ret = 0;
	}

	return ret;
}

static int envf_nowhere_init(void)
{
	gd->env_addr	= (ulong)&default_environment[0];
	gd->env_valid	= ENV_INVALID;

	return 0;
}

U_BOOT_ENV_LOCATION(nowhere) = {
	.location	= ENVL_NOWHERE,
	.init		= envf_nowhere_init,
	.load		= envf_load,
	.save		= env_save_ptr(envf_save),
	ENV_NAME("envf")
};
#endif


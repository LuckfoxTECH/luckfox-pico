// SPDX-License-Identifier:     GPL-2.0+
/*
 * Copyright (C) 2023 Rockchip Electronics Co., Ltd
 */

#include <common.h>
#include <dm.h>
#include <misc.h>
#include <asm/io.h>
#include <linux/bitops.h>
#include <asm/arch/rockchip_smccc.h>

DECLARE_GLOBAL_DATA_PTR;

#define RK_ATAG_MCU_SLP_CORE		0x526b0001
#define RK_ATAG_MCU_SLP_MAX		0x526b00ff
#define RK_ATAG_NONE			0x00000000

/* rk_tag related defines */
#define sleep_tag_next(t)	\
	((struct rk_sleep_tag *)((__u32 *)(t) + (t)->hdr.size))

struct rk_tag_header {
	u32 size;
	u32 tag;
};

struct rk_sleep_tag {
	struct rk_tag_header hdr;
	u32 params[];
};

struct rk_mcu_sleep_core_tag {
	struct rk_tag_header hdr;
	u32 total_size;
	u32 reserve[13];
};

struct rk_mcu_sleep_tags {
	struct rk_mcu_sleep_core_tag core;
	struct rk_sleep_tag slp_tags;
};

static int rockchip_pm_config_ioctl(struct udevice *dev,
				    unsigned long request,
				    void *buf)
{
	int ret = -EINVAL;

	switch (request) {
	case IOCTL_REQ_START:
		break;
	default:
		printf("Unsupported ioctl: %ld\n", (ulong)request);
		break;
	}

	return ret;
}

static const struct misc_ops rockchip_pm_config_ops = {
	.ioctl = rockchip_pm_config_ioctl,
};

static int parse_mcu_sleep_config(ofnode node)
{
	int ret, len;
	ofnode mcu_sleep_node;
	ofnode child;
	struct arm_smccc_res res;
	struct rk_mcu_sleep_tags *config;
	struct rk_sleep_tag *slp_tag;
	char *end;

	mcu_sleep_node = ofnode_find_subnode(node, "rockchip-mcu-sleep-cfg");
	if (ofnode_valid(mcu_sleep_node) == 0) {
		ret = -ENODEV;
		goto out;
	}

	child = ofnode_first_subnode(mcu_sleep_node);
	if (ofnode_valid(child) == 0) {
		pr_err("%s: no valid child node in rockchip-mcu-sleep-cfg\n",
		       __func__);
		ret = -ENODEV;
		goto out;
	}

	/*
	 * 4kb for sleep parameters
	 */
	res = sip_smc_request_share_mem(1, SHARE_PAGE_TYPE_SLEEP);
	if (res.a0 != 0) {
		pr_err("%s: no trust memory for mcu_sleep\n", __func__);
		ret = -ENOMEM;
		goto out;
	}

	/* Initialize core tag */
	memset((void *)res.a1, 0, sizeof(struct rk_mcu_sleep_tags));
	config = (struct rk_mcu_sleep_tags *)res.a1;
	config->core.hdr.tag = RK_ATAG_MCU_SLP_CORE;
	config->core.hdr.size = sizeof(struct rk_mcu_sleep_core_tag) / sizeof(u32);
	config->core.total_size = sizeof(struct rk_mcu_sleep_tags) -
				  sizeof(struct rk_sleep_tag);

	slp_tag = &config->slp_tags;

	/* End point of sleep data  */
	end = (char *)config + PAGE_SIZE - sizeof(struct rk_sleep_tag);

	ofnode_for_each_subnode(child, mcu_sleep_node) {
		/* Is overflow? */
		if ((char *)slp_tag->params >= end)
			break;

		ret = ofnode_read_u32_array(child, "rockchip,tag",
					    &slp_tag->hdr.tag, 1);
		if (ret ||
		    slp_tag->hdr.tag <= RK_ATAG_MCU_SLP_CORE ||
		    slp_tag->hdr.tag >= RK_ATAG_MCU_SLP_MAX) {
			pr_err("%s: no or invalid rockchip,tag in %s\n",
				__func__, ofnode_get_name(child));

			continue;
		}

		len = ofnode_read_size(child, "rockchip,params");
		if (len > 0) {
			/* Is overflow? */
			if ((char *)(slp_tag->params + len) >= end) {
				pr_warn("%s: no more space for rockchip,tag in %s\n",
					__func__, ofnode_get_name(child));
				break;
			}

			ret = ofnode_read_u32_array(child, "rockchip,params",
						    slp_tag->params,
						    len / sizeof(u32));
			if (ret) {
				pr_err("%s: read rockchip,params error in %s\n",
				       __func__, ofnode_get_name(child));
				break;
			}

			slp_tag->hdr.size =
				(len + sizeof(struct rk_tag_header)) / sizeof(u32);
		} else if (len == 0) {
			slp_tag->hdr.size = 0;
		} else {
			continue;
		}

		config->core.total_size += slp_tag->hdr.size * sizeof(u32);

		slp_tag = sleep_tag_next(slp_tag);
	}

	/* Add none tag.
	 * Compiler will combine the follow code as "str xzr, [x28]", but
	 * "slp->hdr" may not be 8-byte alignment. So we use memset_io instead:
	 * slp_tag->hdr.size = 0;
	 * slp_tag->hdr.tag = RK_ATAG_NONE;
	 */
	memset_io(&slp_tag->hdr, 0, sizeof(slp_tag->hdr));

	config->core.total_size += sizeof(struct rk_sleep_tag);

	ret = 0;

out:
	return ret;
}

static int rockchip_pm_config_probe(struct udevice *dev)
{
	parse_mcu_sleep_config(dev_ofnode(dev));

	return 0;
}

static const struct udevice_id rockchip_pm_config_ids[] = {
	{ .compatible = "rockchip,pm-rk3588" },
	{}
};

U_BOOT_DRIVER(rockchip_pm_config) = {
	.name = "rockchip_pm_config",
	.id = UCLASS_MISC,
	.of_match = rockchip_pm_config_ids,
	.probe = rockchip_pm_config_probe,
	.ops = &rockchip_pm_config_ops,
};

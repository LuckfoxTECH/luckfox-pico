/*********************************************************************************
 *
 *       Copyright (C) 2016-2023 Ichiro Kawazome
 *       All rights reserved.
 *
 *       Redistribution and use in source and binary forms, with or without
 *       modification, are permitted provided that the following conditions
 *       are met:
 *
 *         1. Redistributions of source code must retain the above copyright
 *            notice, this list of conditions and the following disclaimer.
 *
 *         2. Redistributions in binary form must reproduce the above copyright
 *            notice, this list of conditions and the following disclaimer in
 *            the documentation and/or other materials provided with the
 *            distribution.
 *
 *       THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *       "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *       LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *       A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 *       OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *       SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *       LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *       DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *       THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *       (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *       OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ********************************************************************************/
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/configfs.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/limits.h>
#include <linux/file.h>
#include <linux/version.h>

#define DRIVER_NAME        "dtbocfg"
#define DRIVER_VERSION     "0.1.0"

/**
 * Device Tree Overlay Item Structure
 */
struct dtbocfg_overlay_item {
    struct config_item	    item;
#if (LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0))
    struct device_node*     node;
#endif
    int                     id;
    void*                   dtbo;
    int                     dtbo_size;
};

/**
 * dtbocfg_overlay_create() - Create Device Tree Overlay
 * @overlay: Pointer to Device Tree Overlay Item
 * return    Success(0) or Error Status.
 */
static int dtbocfg_overlay_item_create(struct dtbocfg_overlay_item *overlay)
{
    int ret_val;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0))
    {
        int ovcs_id = 0;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0))
        ret_val = of_overlay_fdt_apply(overlay->dtbo,overlay->dtbo_size, &ovcs_id, NULL);
#else
        ret_val = of_overlay_fdt_apply(overlay->dtbo,overlay->dtbo_size, &ovcs_id);
#endif
        if (ret_val != 0) {
            pr_err("%s: Failed to apply overlay (ret_val=%d)\n", __func__, ret_val);
            goto failed;
        }
        overlay->id = ovcs_id;
        pr_debug("%s: apply OK(id=%d)\n", __func__, ovcs_id);
    }
#else

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 7, 0))
    of_fdt_unflatten_tree(overlay->dtbo, NULL, &overlay->node);
#else
    of_fdt_unflatten_tree(overlay->dtbo, &overlay->node);
#endif
    if (overlay->node == NULL) {
        pr_err("%s: failed to unflatten tree\n", __func__);
        ret_val = -EINVAL;
        goto failed;
    }
    pr_debug("%s: unflattened OK\n", __func__);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
    {
        int ovcs_id = 0;

        ret_val = of_overlay_apply(overlay->node, &ovcs_id);
        if (ret_val != 0) {
            pr_err("%s: Failed to apply overlay (ret_val=%d)\n", __func__, ret_val);
            goto failed;
        }
        overlay->id = ovcs_id;
        pr_debug("%s: apply OK(id=%d)\n", __func__, ovcs_id);
    }
#else
    {
        of_node_set_flag(overlay->node, OF_DETACHED);

        ret_val = of_resolve_phandles(overlay->node);
        if (ret_val != 0) {
            pr_err("%s: Failed to resolve tree\n", __func__);
            goto failed;
        }
        pr_debug("%s: resolved OK\n", __func__);

        ret_val = of_overlay_create(overlay->node);
        if (ret_val < 0) {
            pr_err("%s: Failed to create overlay (ret_val=%d)\n", __func__, ret_val);
            goto failed;
        }
        overlay->id = ret_val;
    }
#endif

#endif
    pr_debug("%s: create OK\n", __func__);
    return 0;

  failed:
    return ret_val;
}

/**
 * dtbocfg_overlay_item_release() - Relase Device Tree Overlay
 * @overlay: Pointer to Device Tree Overlay Item
 * return    none
 */
static void dtbocfg_overlay_item_release(struct dtbocfg_overlay_item *overlay)
{
    if (overlay->id >= 0) {
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 15, 0))
        of_overlay_remove(&overlay->id);
#else
        of_overlay_destroy(overlay->id);
#endif
        overlay->id = -1;
    }
}

/**
 * container_of_dtbocfg_overlay_item() - Get Device Tree Overlay Item Pointer from Configuration Item
 * @item:  Pointer to Configuration Item
 * return  Pointer to Device Tree Overlay Item
 */
static inline struct dtbocfg_overlay_item* container_of_dtbocfg_overlay_item(struct config_item *item)
{
    return item ? container_of(item, struct dtbocfg_overlay_item, item) : NULL;
}

/**
 * dtbocfg_overlay_item_status_store() - Set Status Attibute
 * @item:  Pointer to Configuration Item
 * @page:  Pointer to Value Buffer
 * @count: Size of Value Buffer Size
 * return  Stored Size or Error Status.
 */
static ssize_t dtbocfg_overlay_item_status_store(struct config_item *item, const char *buf, size_t count)
{
    struct dtbocfg_overlay_item *overlay = container_of_dtbocfg_overlay_item(item);
    ssize_t       status;
    unsigned long value;
    if (0 != (status = kstrtoul(buf, 10, &value))) {
        goto failed;
    }
    if (value == 0) {
        if (overlay->id >= 0) {
            dtbocfg_overlay_item_release(overlay);
        }
    } else {
        if (overlay->id  < 0) {
            dtbocfg_overlay_item_create(overlay);
        }
    }
    return count;
  failed:
    return -EPERM;
}

/**
 * dtbocfg_overlay_item_status_show() - Show Status Attibute
 * @item : Pointer to Configuration Item
 * @page : Pointer to Value for Store
 * return  String Size or Error Status.
 */
static ssize_t dtbocfg_overlay_item_status_show(struct config_item *item, char *page)
{
    struct dtbocfg_overlay_item *overlay = container_of_dtbocfg_overlay_item(item);
    return sprintf(page, "%d\n", overlay->id >= 0 ? 1 : 0);
}

/**
 * dtbocfg_overlay_item_dtbo_write() - Write Device Tree Blob to Configuration Item
 * @item : Pointer to Configuration Item
 * @page : Pointer to Value Buffer
 * @count: Size of Value Buffer
 * return  Stored Size or Error Status.
 */
static ssize_t dtbocfg_overlay_item_dtbo_write(struct config_item *item, const void *buf, size_t count)
{
    struct dtbocfg_overlay_item *overlay = container_of_dtbocfg_overlay_item(item);

    if (overlay->dtbo_size > 0) {
        if (overlay->id >= 0) {
            return -EPERM;
        }
        kfree(overlay->dtbo);
        overlay->dtbo      = NULL;
        overlay->dtbo_size = 0;
    }

    overlay->dtbo = kmemdup(buf, count, GFP_KERNEL);
    if (overlay->dtbo == NULL) {
        overlay->dtbo_size = 0;
        return -ENOMEM;
    } else {
        overlay->dtbo_size = count;
        return count;
    }
}

/**
 * dtbocfg_overlay_item_dtbo_read() - Read Device Tree Blob from Configuration Item
 * @item : Pointer to Configuration Item
 * @page : Pointer to Value for Store, or NULL to query the buffer size
 * @size : Size of the supplied buffer
 * return  Read Size
 */
static ssize_t dtbocfg_overlay_item_dtbo_read(struct config_item *item, void *buf, size_t size)
{
    struct dtbocfg_overlay_item *overlay = container_of_dtbocfg_overlay_item(item);

    if (overlay->dtbo == NULL)
        return 0;

    if (buf != NULL)
        memcpy(buf, overlay->dtbo, overlay->dtbo_size);

    return overlay->dtbo_size;
}

/**
 * Device Tree Blob Overlay Attribute Structure
 */
CONFIGFS_BIN_ATTR(dtbocfg_overlay_item_, dtbo, NULL, 1024 * 1024); // 1MiB should be way more than enough
CONFIGFS_ATTR(dtbocfg_overlay_item_, status);

static struct configfs_attribute *dtbocfg_overlay_attrs[] = {
    &dtbocfg_overlay_item_attr_status,
    NULL,
};

static struct configfs_bin_attribute *dtbocfg_overlay_bin_attrs[] = {
    &dtbocfg_overlay_item_attr_dtbo,
    NULL,
};

/**
 * dtbocfg_overlay_release() - Release Device Tree Overlay Item
 * @item : Pointer to Configuration Item
 * Return  None
 */
static void dtbocfg_overlay_release(struct config_item *item)
{
    struct dtbocfg_overlay_item *overlay = container_of_dtbocfg_overlay_item(item);

    pr_debug("%s\n", __func__);

    dtbocfg_overlay_item_release(overlay);

    if (overlay->dtbo) {
        kfree(overlay->dtbo);
        overlay->dtbo      = NULL;
        overlay->dtbo_size = 0;
    }

    kfree(overlay);
}

/**
 * Device Tree Blob Overlay Item Structure
 */
static struct configfs_item_operations dtbocfg_overlay_item_ops = {
    .release        = dtbocfg_overlay_release,
};

static struct config_item_type dtbocfg_overlay_item_type = {
    .ct_item_ops    = &dtbocfg_overlay_item_ops,
    .ct_attrs       = dtbocfg_overlay_attrs,
    .ct_bin_attrs   = dtbocfg_overlay_bin_attrs,
    .ct_owner       = THIS_MODULE,
};

/**
 * dtbocfg_overlay_group_make_item() - Make Device Tree Overlay Group Item
 * @group: Pointer to Configuration Group
 * @name : Pointer to Group Name
 * Return  Pointer to Device Tree Overlay Group Item
 */
static struct config_item *dtbocfg_overlay_group_make_item(struct config_group *group, const char *name)
{
    struct dtbocfg_overlay_item *overlay;

    pr_debug("%s\n", __func__);

    overlay = kzalloc(sizeof(*overlay), GFP_KERNEL);

    if (!overlay)
        return ERR_PTR(-ENOMEM);
    overlay->id        = -1;
    overlay->dtbo      = NULL;
    overlay->dtbo_size = 0;

    config_item_init_type_name(&overlay->item, name, &dtbocfg_overlay_item_type);
    return &overlay->item;
}

/**
 * dtbocfg_overlay_group_drop_item() - Drop Device Tree Overlay Group Item
 * @group: Pointer to Configuration Group
 * @item : Pointer to Device Tree Overlay Group Item
 */
static void dtbocfg_overlay_group_drop_item(struct config_group *group, struct config_item *item)
{
    struct dtbocfg_overlay_item *overlay = container_of_dtbocfg_overlay_item(item);

    pr_debug("%s\n", __func__);

    config_item_put(&overlay->item);
}

/**
 * Device Tree Blob Overlay Sub Group Structures
 */
static struct configfs_group_operations dtbocfg_overlays_ops = {
    .make_item      = dtbocfg_overlay_group_make_item,
    .drop_item      = dtbocfg_overlay_group_drop_item,
};

static struct config_item_type dtbocfg_overlays_type = {
    .ct_group_ops   = &dtbocfg_overlays_ops,
    .ct_owner       = THIS_MODULE,
};

static struct config_group dtbocfg_overlay_group;

/**
 * Device Tree Blob Overlay Root Sub System Structures
 */
static struct configfs_group_operations dtbocfg_root_ops = {
    /* empty - we don't allow anything to be created */
};

static struct config_item_type dtbocfg_root_type = {
    .ct_group_ops   = &dtbocfg_root_ops,
    .ct_owner       = THIS_MODULE,
};

static struct configfs_subsystem dtbocfg_root_subsys = {
    .su_group = {
        .cg_item = {
            .ci_namebuf = "device-tree",
            .ci_type    = &dtbocfg_root_type,
        },
    },
  .su_mutex = __MUTEX_INITIALIZER(dtbocfg_root_subsys.su_mutex),
};

/**
 * dtbocfg_module_init()
 */
static int __init dtbocfg_module_init(void)
{
    int retval = 0;

    pr_info(DRIVER_NAME ": " DRIVER_VERSION "\n");

    config_group_init(&dtbocfg_root_subsys.su_group);
    config_group_init_type_name(&dtbocfg_overlay_group, "overlays", &dtbocfg_overlays_type);

    retval = configfs_register_subsystem(&dtbocfg_root_subsys);
    if (retval != 0) {
        pr_err( "%s: couldn't register subsys\n", __func__);
        goto register_subsystem_failed;
    }

    retval = configfs_register_group(&dtbocfg_root_subsys.su_group, &dtbocfg_overlay_group);
    if (retval != 0) {
        pr_err( "%s: couldn't register group\n", __func__);
        goto register_group_failed;
    }

    pr_info(DRIVER_NAME ": OK\n");
    return 0;

  register_group_failed:
    configfs_unregister_subsystem(&dtbocfg_root_subsys);
  register_subsystem_failed:
    return retval;
}

/**
 * dtbocfg_module_exit()
 */
static void __exit dtbocfg_module_exit(void)
{
    configfs_unregister_group(&dtbocfg_overlay_group);
    configfs_unregister_subsystem(&dtbocfg_root_subsys);
}

module_init(dtbocfg_module_init);
module_exit(dtbocfg_module_exit);

MODULE_AUTHOR("ikwzm");
MODULE_DESCRIPTION("Device Tree Overlay Configuration File System");
MODULE_LICENSE("Dual BSD/GPL");

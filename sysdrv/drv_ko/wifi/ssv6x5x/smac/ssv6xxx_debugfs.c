
/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <linux/nl80211.h>
#include <linux/etherdevice.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/time.h>
#ifdef SSV_MAC80211
#include "ssv_mac80211.h"
#else
#include <net/mac80211.h>
#endif
#include <ssv6200.h>
#include "dev.h"
#include "ssv6xxx_debugfs.h"


#ifdef CONFIG_SSV6XXX_DEBUGFS

#define QUEUE_STATUS_BUF_SIZE            (4096)
static ssize_t queue_status_read (struct file *file,
                                  char __user *user_buf, size_t count,
                                  loff_t *ppos)
{
    struct ssv_softc *sc = (struct ssv_softc *)file->private_data;
    char             *status_buf = kzalloc(QUEUE_STATUS_BUF_SIZE, GFP_KERNEL);
    ssize_t           status_size;
    ssize_t           ret;

    if (!status_buf)
        return -ENOMEM;

    status_size = ssv6xxx_tx_queue_status_dump(sc, status_buf,
                                       QUEUE_STATUS_BUF_SIZE);
    ret = simple_read_from_buffer(user_buf, count, ppos, status_buf,
                                  status_size);
    kfree(status_buf);

    return ret;
}

static int queue_status_open (struct inode *inode, struct file *file)
{
    file->private_data = inode->i_private;

    return 0;
}

static const struct file_operations queue_status_fops
    = { .read = queue_status_read,
        .open = queue_status_open };
#endif // CONFIG_SSV6XXX_DEBUGFS

int ssv6xxx_init_debugfs (struct ssv_softc *sc, const char *name)
{
#ifdef CONFIG_SSV6XXX_DEBUGFS
    struct ieee80211_hw *hw = sc->hw;
    struct dentry *phy_debugfs_dir = hw->wiphy->debugfsdir;
    struct dentry *drv_debugfs_dir;

    drv_debugfs_dir = debugfs_create_dir(name, phy_debugfs_dir);
    if (!drv_debugfs_dir)
    {
        dev_err(sc->dev, "Failed to create debugfs.\n");
        return -ENOMEM;
    }

    sc->debugfs_dir = drv_debugfs_dir;

    // Create debug files for queues.
    debugfs_create_file("queue_status", 00444, drv_debugfs_dir,
                        sc, &queue_status_fops);

#endif // CONFIG_SSV6XXX_DEBUGFS
    return 0;
}


void ssv6xxx_deinit_debugfs (struct ssv_softc *sc)
{
#ifdef CONFIG_SSV6XXX_DEBUGFS
    if (!sc->debugfs_dir)
        return;

    debugfs_remove_recursive(sc->debugfs_dir);
    
    sc->debugfs_dir = NULL;
#endif // CONFIG_SSV6XXX_DEBUGFS
}


int ssv6xxx_debugfs_add_interface(struct ssv_softc *sc, struct ieee80211_vif *vif)
{
#ifdef CONFIG_SSV6XXX_DEBUGFS
    struct dentry *drv_debugfs_dir = sc->debugfs_dir;
    struct dentry *vif_debugfs_dir;
    char vif_addr[18];
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    struct ssv_vif_info *vif_info = &sc->vif_info[vif_priv->vif_idx];

    snprintf(vif_addr, sizeof(vif_addr), "%02X-%02X-%02X-%02X-%02X-%02X",
            vif->addr[0], vif->addr[1], vif->addr[2],
            vif->addr[3], vif->addr[4], vif->addr[5]);
    vif_debugfs_dir = debugfs_create_dir(vif_addr, drv_debugfs_dir);
    if (!vif_debugfs_dir)
    {
        dev_err(sc->dev, "Failed to create interface debugfs for %s.\n", vif_addr);
        return -ENOMEM;
    }

    sc->debugfs_dir = drv_debugfs_dir;
    vif_info->debugfs_dir = vif_debugfs_dir;
#endif // CONFIG_SSV6XXX_DEBUGFS    
    return 0;
}


int ssv6xxx_debugfs_remove_interface(struct ssv_softc *sc, struct ieee80211_vif *vif)
{
#ifdef CONFIG_SSV6XXX_DEBUGFS
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)vif->drv_priv;
    struct ssv_vif_info *vif_info = &sc->vif_info[vif_priv->vif_idx];
    if ((vif_info->debugfs_dir == NULL) || (sc->debugfs_dir == NULL))
        return 0;

    debugfs_remove_recursive(vif_info->debugfs_dir);
    vif_info->debugfs_dir = NULL;
#endif // CONFIG_SSV6XXX_DEBUGFS
    return 0;
}


int ssv6xxx_debugfs_remove_sta(struct ssv_softc *sc, struct ssv_sta_info *sta)
{
#ifdef CONFIG_SSV6XXX_DEBUGFS
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)sta->vif->drv_priv;
    struct ssv_vif_info *vif_info = &sc->vif_info[vif_priv->vif_idx];
    if ((sc->debugfs_dir == NULL) || (vif_info->debugfs_dir == NULL) || (sta->debugfs_dir == NULL))
        return 0;

    debugfs_remove_recursive(sta->debugfs_dir);

    sta->debugfs_dir = NULL;
#endif // CONFIG_SSV6XXX_DEBUGFS    
    return 0;
}


int ssv6xxx_debugfs_add_sta(struct ssv_softc *sc, struct ssv_sta_info *sta)
{
#ifdef CONFIG_SSV6XXX_DEBUGFS
    struct ssv_vif_priv_data *vif_priv = (struct ssv_vif_priv_data *)sta->vif->drv_priv;
    struct ssv_vif_info *vif_info = &sc->vif_info[vif_priv->vif_idx];
    struct dentry *vif_debugfs_dir = vif_info->debugfs_dir;
    struct dentry *sta_debugfs_dir;
    char sta_addr[18];

    if (vif_debugfs_dir == NULL)
        return 0;

    snprintf(sta_addr, sizeof(sta_addr), "%02X-%02X-%02X-%02X-%02X-%02X",
             sta->sta->addr[0], sta->sta->addr[1], sta->sta->addr[2],
             sta->sta->addr[3], sta->sta->addr[4], sta->sta->addr[5]);
    sta_debugfs_dir = debugfs_create_dir(sta_addr, vif_debugfs_dir);
    if (!sta_debugfs_dir)
    {
        dev_err(sc->dev, "Failed to create interface debugfs for %s.\n", sta_addr);
        return -ENOMEM;
    }

    sta->debugfs_dir = sta_debugfs_dir;
#endif // CONFIG_SSV6XXX_DEBUGFS    
    return 0;
}


/* create and remove of files */
#define DEBUGFS_ADD_FILE(name, parent, mode) do {            \
    if (!debugfs_create_file(#name, mode, parent, priv,        \
                 &ssv_dbgfs_##name##_ops))        \
        goto err;                        \
} while (0)

#define DEBUGFS_ADD_BOOL(name, parent, ptr) do {            \
    struct dentry *__tmp;                        \
    __tmp = debugfs_create_bool(#name, S_IWUSR | S_IRUSR,        \
                    parent, ptr);            \
    if (IS_ERR(__tmp) || !__tmp)                    \
        goto err;                        \
} while (0)

#define DEBUGFS_ADD_X32(name, parent, ptr) do {                \
    struct dentry *__tmp;                        \
    __tmp = debugfs_create_x32(#name, S_IWUSR | S_IRUSR,        \
                   parent, ptr);            \
    if (IS_ERR(__tmp) || !__tmp)                    \
        goto err;                        \
} while (0)

#define DEBUGFS_ADD_U32(name, parent, ptr, mode) do {            \
    struct dentry *__tmp;                        \
    __tmp = debugfs_create_u32(#name, mode,                \
                   parent, ptr);            \
    if (IS_ERR(__tmp) || !__tmp)                    \
        goto err;                        \
} while (0)

/* file operation */
#define DEBUGFS_READ_FUNC(name)                                         \
static ssize_t ssv_dbgfs_##name##_read(struct file *file,               \
                    char __user *user_buf,          \
                    size_t count, loff_t *ppos);

#define DEBUGFS_WRITE_FUNC(name)                                        \
static ssize_t ssv_dbgfs_##name##_write(struct file *file,              \
                    const char __user *user_buf,    \
                    size_t count, loff_t *ppos);


#define DEBUGFS_READ_FILE_OPS(name)                                     \
    DEBUGFS_READ_FUNC(name);                                        \
static const struct file_operations ssv_dbgfs_##name##_ops = {          \
    .read = ssv_dbgfs_##name##_read,                               \
    .open = ssv_dbgfs_open_file_generic,                        \
    .llseek = generic_file_llseek,                    \
};

#define DEBUGFS_WRITE_FILE_OPS(name)                                    \
    DEBUGFS_WRITE_FUNC(name);                                       \
static const struct file_operations ssv_dbgfs_##name##_ops = {          \
    .write = ssv_dbgfs_##name##_write,                              \
    .open = ssv_dbgfs_open_file_generic,                        \
    .llseek = generic_file_llseek,                    \
};


#define DEBUGFS_READ_WRITE_FILE_OPS(name)                               \
    DEBUGFS_READ_FUNC(name);                                        \
    DEBUGFS_WRITE_FUNC(name);                                       \
static const struct file_operations ssv_dbgfs_##name##_ops = {          \
    .write = ssv_dbgfs_##name##_write,                              \
    .read = ssv_dbgfs_##name##_read,                                \
    .open = ssv_dbgfs_open_file_generic,                            \
    .llseek = generic_file_llseek,                    \
};

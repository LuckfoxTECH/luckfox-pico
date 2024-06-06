#ifndef __RWNX_WAKELOCK_H
#define __RWNX_WAKELOCK_H

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/platform_device.h>

struct wakeup_source *rwnx_wakeup_init(const char *name);
void rwnx_wakeup_deinit(struct wakeup_source *ws);

struct wakeup_source *rwnx_wakeup_register(struct device *dev, const char *name);
void rwnx_wakeup_unregister(struct wakeup_source *ws);

void rwnx_wakeup_lock(struct wakeup_source *ws);
void rwnx_wakeup_unlock(struct wakeup_source *ws);
void rwnx_wakeup_lock_timeout(struct wakeup_source *ws, unsigned int msec);

void aicwf_wakeup_lock_init(struct rwnx_hw *rwnx_hw);
void aicwf_wakeup_lock_deinit(struct rwnx_hw *rwnx_hw);

#endif

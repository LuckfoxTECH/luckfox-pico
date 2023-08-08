/*
 * Media controller interface library
 *
 * Copyright (C) 2010-2011 Ideas on board SPRL
 *
 * Contact: Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __MEDIA_PRIV_H__
#define __MEDIA_PRIV_H__

#include <linux/media.h>

#include "mediactl.h"

struct media_entity {
    struct media_device *media;
    struct media_entity_desc info;
    struct media_pad *pads;
    struct media_link *links;
    unsigned int max_links;
    unsigned int num_links;

    char devname[32];
    int fd;
};

struct media_device {
    int fd;
    int refcount;
    char *devnode;

    struct media_device_info info;
    struct media_entity *entities;
    unsigned int entities_count;

    void (*debug_handler)(void *, ...);
    void *debug_priv;

    struct {
        struct media_entity *v4l;
        struct media_entity *fb;
        struct media_entity *alsa;
        struct media_entity *dvb;
    } def;
};

#define media_dbg(media, ...) \
    (media)->debug_handler((media)->debug_priv, __VA_ARGS__)

#endif /* __MEDIA_PRIV_H__ */

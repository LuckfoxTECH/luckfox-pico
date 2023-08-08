/*
 * Media controller test application
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

#ifndef __TOOLS_H__
#define __TOOLS_H__

#define ARRAY_SIZE(array)   (sizeof(array) / sizeof((array)[0]))
#define FIELD_SIZEOF(t, f)  (sizeof(((t*)0)->f))

void media_print_streampos(struct media_device *media, const char *p,
                           const char *end);

#endif /* __TOOLS_H__ */

// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_EXPORT_H__
#define __CGI_EXPORT_H__

#if !defined(CGI_EXPORT)
#if defined(_WIN32)
#if defined(LIBCGI_IMPLEMENTATION)
#define CGI_EXPORT __declspec(dllexport)
#else
#define CGI_EXPORT __declspec(dllimport)
#endif
#elif defined(__GNUC__)
#if defined(LIBCGI_IMPLEMENTATION)
#define CGI_EXPORT __attribute__((visibility("default")))
#else
#define CGI_EXPORT
#endif
#else
#define CGI_EXPORT
#endif
#endif // !defined(CGI_EXPORT)

#if !defined(CGI_NO_EXPORT)
#if defined(__GNUC__)
#define CGI_NO_EXPORT __attribute__((visibility("hidden")))
#else
#define CGI_NO_EXPORT
#endif
#endif // !defined(CGI_NO_EXPORT)

#endif // __CGI_EXPORT_H__
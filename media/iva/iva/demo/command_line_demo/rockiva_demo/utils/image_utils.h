#ifndef __IMAGE_UTILS_H__
#define __IMAGE_UTILS_H__

#include "rockiva_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMAGE_PATH_MAX 4096

extern const unsigned char COLOR_GREEN[3];
extern const unsigned char COLOR_BLUE[3];
extern const unsigned char COLOR_RED[3];

int GetImagePathList(const char *dir, char (*image_path_list)[IMAGE_PATH_MAX],
                     int max_num);

int ReadImage(const char *path, RockIvaImage *image);

int SaveImage(const char *path, const RockIvaImage *img);

int ReadImageInfo(const char *path, int *w, int *h);

void DrawRectangleRGB(unsigned char *pixels, int w, int h, int rx, int ry,
                      int rw, int rh, unsigned char color[3], int thickness);

void DrawLineRGB(unsigned char *pixels, int w, int h, int x0, int y0, int x1,
                 int y1, unsigned char color[3], int thickness);

void DrawTextRGB(unsigned char *pixels, int w, int h, const char *text, int x,
                 int y, int fontpixelsize, unsigned char color[3]);

int DrawLine(RockIvaImage *image, RockIvaPoint start, RockIvaPoint end);

int DrawArea(RockIvaImage *image, RockIvaArea *area);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
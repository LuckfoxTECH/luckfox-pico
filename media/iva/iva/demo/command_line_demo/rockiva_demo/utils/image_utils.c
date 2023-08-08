#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_THREAD_LOCALS
#define STBI_ONLY_JPEG
#define STBI_ONLY_PNG
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "turbojpeg.h"

#include "font.h"
#include "image_utils.h"

extern const unsigned char COLOR_GREEN[3] = {0, 255, 0};
extern const unsigned char COLOR_BLUE[3] = {0, 0, 255};
extern const unsigned char COLOR_RED[3] = {255, 0, 0};

#define max(a, b) (((a) > (b)) ? (a) : (b))
#define min(a, b) (((a) < (b)) ? (a) : (b))

static const char *filter_image_names[] = {"jpg", "jpeg", "JPG", "JPEG",
                                           "png", "PNG",  NULL};

static const char *subsampName[TJ_NUMSAMP] = {"4:4:4",     "4:2:2", "4:2:0",
                                              "Grayscale", "4:4:0", "4:1:1"};

static const char *colorspaceName[TJ_NUMCS] = {"RGB", "YCbCr", "GRAY", "CMYK",
                                               "YCCK"};

static int AlphaSort(const struct dirent **a, const struct dirent **b) {
  return strcoll((*a)->d_name, (*b)->d_name);
}

static int ImageFileFilter(const struct dirent *entry) {
  const char **filter;

  for (filter = filter_image_names; *filter; ++filter) {
    if (strstr(entry->d_name, *filter) != NULL) {
      return 1;
    }
  }
  return 0;
}

static inline int distance_lessthan(int x, int y, int x0, int y0, int x1,
                                    int y1, float t) {
  int dx01 = x1 - x0;
  int dy01 = y1 - y0;
  int dx0 = x - x0;
  int dy0 = y - y0;

  float r = (float)(dx0 * dx01 + dy0 * dy01) / (dx01 * dx01 + dy01 * dy01);

  if (r < 0 || r > 1)
    return 0;

  float px = x0 + dx01 * r;
  float py = y0 + dy01 * r;
  float dx = x - px;
  float dy = y - py;
  float p = dx * dx + dy * dy;
  return p < t;
}

static int ReadImageJpeg(const char *path, RockIvaImage *image) {
  FILE *jpegFile = NULL;
  unsigned long jpegSize;
  int flags = 0;
  int width, height;
  unsigned char *imgBuf = NULL;
  unsigned char *jpegBuf = NULL;
  unsigned long size;
  if ((jpegFile = fopen(path, "rb")) == NULL) {
    printf("open input file failure\n");
  }
  if (fseek(jpegFile, 0, SEEK_END) < 0 || (size = ftell(jpegFile)) < 0 ||
      fseek(jpegFile, 0, SEEK_SET) < 0) {
    printf("determining input file size failure\n");
  }
  if (size == 0) {
    printf("determining input file size, Input file contains no data\n");
  }
  jpegSize = (unsigned long)size;
  if ((jpegBuf = (unsigned char *)malloc(jpegSize * sizeof(unsigned char))) ==
      NULL) {
    printf("allocating JPEG buffer\n");
  }
  if (fread(jpegBuf, jpegSize, 1, jpegFile) < 1) {
    printf("reading input file");
  }
  fclose(jpegFile);
  jpegFile = NULL;

  tjhandle handle = NULL;
  int subsample, colorspace;
  int padding = 1;
  int ret = 0;

  handle = tjInitDecompress();
  ret = tjDecompressHeader3(handle, jpegBuf, size, &width, &height, &subsample,
                            &colorspace);
  if (ret < 0) {
    printf("header file error, errorStr:%s, errorCode:%d\n", tjGetErrorStr(),
           tjGetErrorCode(handle));
    return -1;
  }
  printf("input image: %d x %d, %s subsampling, %s colorspace\n", width, height,
         subsampName[subsample], colorspaceName[colorspace]);
  int sw_out_size = width * height * 3;
  unsigned char *sw_out_buf = image->dataAddr;
  if (sw_out_buf == NULL) {
    sw_out_buf = (unsigned char *)malloc(sw_out_size * sizeof(unsigned char));
  }
  if (sw_out_buf == NULL) {
    printf("sw_out_buf is NULL\n");
    goto out;
  }

  flags |= 0;

  // ret<-1表示解码异常，此时通过tjGetErrorCode查看错误码，错误码为0时，表示警告，错误码为-1时表示错误
  int pixelFormat = TJPF_RGB;
  ret = tjDecompress2(handle, jpegBuf, size, sw_out_buf, width, 0, height,
                      pixelFormat, flags);
  // ret = tjDecompressToYUV2(handle, jpeg_buf, size, dst_buf, *width, padding,
  // *height, flags);
  if ((0 != tjGetErrorCode(handle)) && (ret < 0)) {
    printf("error : decompress to yuv failed, errorStr:%s, errorCode:%d\n",
           tjGetErrorStr(), tjGetErrorCode(handle));
    goto out;
  }
  if ((0 == tjGetErrorCode(handle)) && (ret < 0)) {
    printf("warning : errorStr:%s, errorCode:%d\n", tjGetErrorStr(),
           tjGetErrorCode(handle));
  }
  tjDestroy(handle);

  image->info.width = width;
  image->info.height = height;
  image->info.format = ROCKIVA_IMAGE_FORMAT_RGB888;
  image->dataAddr = sw_out_buf;
out:
  if (jpegBuf) {
    free(jpegBuf);
  }
  return 0;
}

static int WriteImageJpeg(const char *path, int quality, RockIvaImage *image) {
  int ret;
  int jpegSubsamp = TJSAMP_422;
  unsigned char *jpegBuf = NULL;
  unsigned long jpegSize = 0;
  int flags = 0;

  const unsigned char *data = image->dataAddr;
  int width = image->info.width;
  int height = image->info.height;
  int pixelFormat = TJPF_RGB;

  tjhandle handle = tjInitCompress();

  if (image->info.format == ROCKIVA_IMAGE_FORMAT_RGB888) {
    ret = tjCompress2(handle, data, width, 0, height, pixelFormat, &jpegBuf,
                      &jpegSize, jpegSubsamp, quality, flags);
  } else {
    printf("WriteImageJpeg: pixel format %d not support\n", image->info.format);
    return -1;
  }

  // printf("ret=%d jpegBuf=%p jpegSize=%d\n", ret, jpegBuf, jpegSize);
  if (jpegBuf != NULL && jpegSize > 0) {
    WriteDataToFile(path, jpegBuf, jpegSize);
    tjFree(jpegBuf);
  }
  tjDestroy(handle);

  return 0;
}

static int ReadImageStb(const char *path, RockIvaImage *image) {
  // 默认图像为3通道
  int w, h, c;
  unsigned char *pixeldata = stbi_load(path, &w, &h, &c, 3);
  if (!pixeldata) {
    printf("error: read image %s fail\n", path);
    return -1;
  }
  int size = w * h * c;

  // 设置图像数据
  if (image->dataAddr != NULL) {
    memcpy(image->dataAddr, pixeldata, size);
    stbi_image_free(pixeldata);
  } else {
    image->dataAddr = pixeldata;
  }
  image->info.width = w;
  image->info.height = h;
  image->info.format = ROCKIVA_IMAGE_FORMAT_RGB888;
  return 0;
}

int GetImagePathList(const char *dir, char (*image_path_list)[IMAGE_PATH_MAX],
                     int max_num) {
  struct dirent **namelist;
  int n;

  n = scandir(dir, &namelist, ImageFileFilter, AlphaSort);
  if (n == -1) {
    perror("scandir");
    return -1;
  }

  int index = 0;
  for (int i = 0; i < n; i++) {
    printf("%s\n", namelist[i]->d_name);

    memset(image_path_list[index], 0, IMAGE_PATH_MAX);
    snprintf(image_path_list[index], IMAGE_PATH_MAX, "%s/%s", dir,
             namelist[i]->d_name);

    index++;
    if (index >= max_num) {
      break;
    }
  }
  for (int i = 0; i < n; i++) {
    free(namelist[i]);
  }
  free(namelist);
  return index;
}

int ReadImageInfo(const char *path, int *w, int *h) {
  int c;
  int ret = stbi_info(path, w, h, &c);
  if (ret != 1) {
    printf("get image info fail %d\n", ret);
    return -1;
  }
  return 0;
}

int ReadImage(const char *path, RockIvaImage *image) {
  const char *_ext = strrchr(path, '.');
  if (!_ext) {
    // missing extension
    return -1;
  }
  if (strcmp(_ext, ".jpg") == 0 || strcmp(_ext, ".jpeg") == 0 ||
      strcmp(_ext, ".JPG") == 0 || strcmp(_ext, ".JPEG") == 0) {
    return ReadImageJpeg(path, image);
  } else {
    return ReadImageStb(path, image);
  }
}

int SaveImage(const char *path, const RockIvaImage *img) {
  int ret;
  int width = img->info.width;
  int height = img->info.height;
  int channel = 3;
  void *data = img->dataAddr;
  // printf("SaveImage path: %s width=%d height=%d channel=%d data=%p\n",
  //     path, width, height, channel, data);

  const char *_ext = strrchr(path, '.');
  if (!_ext) {
    // missing extension
    return -1;
  }
  if (strcmp(_ext, ".jpg") == 0 || strcmp(_ext, ".jpeg") == 0 ||
      strcmp(_ext, ".JPG") == 0 || strcmp(_ext, ".JPEG") == 0) {
    int quality = 95;
    ret = WriteImageJpeg(path, quality, img);
  } else if (strcmp(_ext, ".png") == 0 | strcmp(_ext, ".PNG") == 0) {
    ret = stbi_write_png(path, width, height, channel, data, 0);
  } else if (strcmp(_ext, ".data") == 0 | strcmp(_ext, ".DATA") == 0) {
    ret = WriteDataToFile(path, data, width * height * channel);
  } else {
    // unknown extension type
    return -1;
  }
  return ret;
}

void DrawRectangleRGB(unsigned char *pixels, int w, int h, int rx, int ry,
                      int rw, int rh, unsigned char color[3], int thickness) {
  int stride = w * 3;
  const unsigned char *pen_color = (const unsigned char *)color;

  if (thickness == -1) {
    // filled
    for (int y = ry; y < ry + rh; y++) {
      if (y < 0)
        continue;

      if (y >= h)
        break;

      unsigned char *p = pixels + stride * y;

      for (int x = rx; x < rx + rw; x++) {
        if (x < 0)
          continue;

        if (x >= w)
          break;

        p[x * 3 + 0] = pen_color[0];
        p[x * 3 + 1] = pen_color[1];
        p[x * 3 + 2] = pen_color[2];
      }
    }

    return;
  }

  const int t0 = thickness / 2;
  const int t1 = thickness - t0;

  // draw top
  {
    for (int y = ry - t0; y < ry + t1; y++) {
      if (y < 0)
        continue;

      if (y >= h)
        break;

      unsigned char *p = pixels + stride * y;

      for (int x = rx - t0; x < rx + rw + t1; x++) {
        if (x < 0)
          continue;

        if (x >= w)
          break;

        p[x * 3 + 0] = pen_color[0];
        p[x * 3 + 1] = pen_color[1];
        p[x * 3 + 2] = pen_color[2];
      }
    }
  }

  // draw bottom
  {
    for (int y = ry + rh - t0; y < ry + rh + t1; y++) {
      if (y < 0)
        continue;

      if (y >= h)
        break;

      unsigned char *p = pixels + stride * y;

      for (int x = rx - t0; x < rx + rw + t1; x++) {
        if (x < 0)
          continue;

        if (x >= w)
          break;

        p[x * 3 + 0] = pen_color[0];
        p[x * 3 + 1] = pen_color[1];
        p[x * 3 + 2] = pen_color[2];
      }
    }
  }

  // draw left
  for (int x = rx - t0; x < rx + t1; x++) {
    if (x < 0)
      continue;

    if (x >= w)
      break;

    for (int y = ry + t1; y < ry + rh - t0; y++) {
      if (y < 0)
        continue;

      if (y >= h)
        break;

      unsigned char *p = pixels + stride * y;

      p[x * 3 + 0] = pen_color[0];
      p[x * 3 + 1] = pen_color[1];
      p[x * 3 + 2] = pen_color[2];
    }
  }

  // draw right
  for (int x = rx + rw - t0; x < rx + rw + t1; x++) {
    if (x < 0)
      continue;

    if (x >= w)
      break;

    for (int y = ry + t1; y < ry + rh - t0; y++) {
      if (y < 0)
        continue;

      if (y >= h)
        break;

      unsigned char *p = pixels + stride * y;

      p[x * 3 + 0] = pen_color[0];
      p[x * 3 + 1] = pen_color[1];
      p[x * 3 + 2] = pen_color[2];
    }
  }
}

void DrawLineRGB(unsigned char *pixels, int w, int h, int x0, int y0, int x1,
                 int y1, unsigned char color[3], int thickness) {
  int stride = w * 3;

  const float t0 = thickness / 2.f;
  const float t1 = thickness - t0;

  int x_min = min(x0, x1);
  int x_max = max(x0, x1);
  int y_min = min(y0, y1);
  int y_max = max(y0, y1);

  for (int y = y_min - t0; y < y_max + t1; y++) {
    if (y < 0)
      continue;

    if (y >= h)
      break;

    unsigned char *p = pixels + stride * y;

    for (int x = x_min - t0; x < x_max + t1; x++) {
      if (x < 0)
        continue;

      if (x >= w)
        break;

      // distance from line
      if (distance_lessthan(x, y, x0, y0, x1, y1, t1)) {
        p[x * 3 + 0] = color[0];
        p[x * 3 + 1] = color[1];
        p[x * 3 + 2] = color[2];
      }
    }
  }
}

int ResizeBilinearGray(char *src_pixels, int w, int h, char *dst_pixels, int w2,
                       int h2) {
  int A, B, C, D, x, y, index, gray;
  float x_ratio = ((float)(w - 1)) / w2;
  float y_ratio = ((float)(h - 1)) / h2;
  float x_diff, y_diff, ya, yb;
  int offset = 0;
  for (int i = 0; i < h2; i++) {
    for (int j = 0; j < w2; j++) {
      x = (int)(x_ratio * j);
      y = (int)(y_ratio * i);
      x_diff = (x_ratio * j) - x;
      y_diff = (y_ratio * i) - y;
      index = y * w + x;

      // range is 0 to 255 thus bitwise AND with 0xff
      A = src_pixels[index] & 0xff;
      B = src_pixels[index + 1] & 0xff;
      C = src_pixels[index + w] & 0xff;
      D = src_pixels[index + w + 1] & 0xff;

      // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
      gray =
          (int)(A * (1 - x_diff) * (1 - y_diff) + B * (x_diff) * (1 - y_diff) +
                C * (y_diff) * (1 - x_diff) + D * (x_diff * y_diff));

      dst_pixels[offset++] = gray;
    }
  }
  return 0;
}

void DrawTextRGB(unsigned char *pixels, int w, int h, const char *text, int x,
                 int y, int fontpixelsize, unsigned char color[3]) {
  int stride = w * 3;
  unsigned char *resized_font_bitmap =
      malloc(fontpixelsize * fontpixelsize * 2);

  const int n = strlen(text);

  int cursor_x = x;
  int cursor_y = y;
  for (int i = 0; i < n; i++) {
    char ch = text[i];

    if (ch == '\n') {
      // newline
      cursor_x = x;
      cursor_y += fontpixelsize * 2;
    }

    if (isprint(ch) != 0) {
      int font_bitmap_index = ch - ' ';
      const unsigned char *font_bitmap = mono_font_data[font_bitmap_index];

      // draw resized character
      ResizeBilinearGray(font_bitmap, 20, 40, resized_font_bitmap,
                         fontpixelsize, fontpixelsize * 2);

      for (int j = cursor_y; j < cursor_y + fontpixelsize * 2; j++) {
        if (j < 0)
          continue;

        if (j >= h)
          break;

        const unsigned char *palpha =
            resized_font_bitmap + (j - cursor_y) * fontpixelsize;
        unsigned char *p = pixels + stride * j;

        for (int k = cursor_x; k < cursor_x + fontpixelsize; k++) {
          if (k < 0)
            continue;

          if (k >= w)
            break;

          unsigned char alpha = palpha[k - cursor_x];

          p[k * 3 + 0] =
              (p[k * 3 + 0] * (255 - alpha) + color[0] * alpha) / 255;
          p[k * 3 + 1] =
              (p[k * 3 + 1] * (255 - alpha) + color[1] * alpha) / 255;
          p[k * 3 + 2] =
              (p[k * 3 + 2] * (255 - alpha) + color[2] * alpha) / 255;
        }
      }

      cursor_x += fontpixelsize;
    }
  }

  free(resized_font_bitmap);
}

int DrawLine(RockIvaImage *image, RockIvaPoint start, RockIvaPoint end) {
  int pt1x = ROCKIVA_RATIO_PIXEL_CONVERT(image->info.width, start.x);
  int pt1y = ROCKIVA_RATIO_PIXEL_CONVERT(image->info.height, start.y);
  int pt2x = ROCKIVA_RATIO_PIXEL_CONVERT(image->info.width, end.x);
  int pt2y = ROCKIVA_RATIO_PIXEL_CONVERT(image->info.height, end.y);
  DrawLineRGB(image->dataAddr, image->info.width, image->info.height, pt1x,
              pt1y, pt2x, pt2y, COLOR_BLUE, 2);
  return 0;
}

int DrawArea(RockIvaImage *image, RockIvaArea *area) {
  for (int i = 0; i < area->pointNum; i++) {
    RockIvaPoint pt1 = area->points[i];
    RockIvaPoint pt2;
    if (i == area->pointNum - 1) {
      pt2 = area->points[0];
    } else {
      pt2 = area->points[i + 1];
    }
    DrawLine(image, pt1, pt2);
  }
  return 0;
}
#ifndef __BO_H_INCLUDED__
#define __BO_H_INCLUDED__

#include <stdint.h>

#undef USE_ATOMIC_API
struct sp_dev;

struct sp_bo {
  struct sp_dev* dev;

  uint32_t width;
  uint32_t height;
  uint32_t depth;
  uint32_t bpp;
  uint32_t format;
  uint32_t flags;

  uint32_t fb_id;
  uint32_t handle;
  int32_t fd;
  void* map_addr;
  uint32_t pitch;
  uint32_t size;
};

int add_fb_sp_bo(struct sp_bo* bo, uint32_t format);
struct sp_bo* create_sp_bo(struct sp_dev* dev, uint32_t width, uint32_t height,
                           uint32_t depth, uint32_t bpp, uint32_t format, uint32_t flags);

void fill_bo(struct sp_bo* bo, uint8_t a, uint8_t r, uint8_t g, uint8_t b);
void draw_rect(struct sp_bo* bo, uint32_t x, uint32_t y, uint32_t width,
               uint32_t height, uint8_t a, uint8_t r, uint8_t g, uint8_t b);

void free_sp_bo(struct sp_bo* bo);

#endif /* __BO_H_INCLUDED__ */

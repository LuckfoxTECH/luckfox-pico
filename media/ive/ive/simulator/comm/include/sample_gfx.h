#ifndef _SAMPLE_GFX_
#define _SAMPLE_GFX_

#include <rk_type.h>

#if __cplusplus
extern "C" {
#endif

void gfx_show_img(const char* name, void* _img_data, int pic_width, int pic_height, int type, int delay);
int gfx_query_frame(RK_U32* width, RK_U32* height, RK_U32* totalFrame, const char* fmt, ...);
int gfx_read_frame(void* data, int size, const char* fmt, ...);
int gfx_write_frame(void* data, int width, int height, int type, int flag, const char* fmt, ...);
#if __cplusplus
}
#endif
#endif

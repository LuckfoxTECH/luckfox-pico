#ifndef _RGA_API_H_
#define _RGA_API_H_

#ifdef __cplusplus
extern "C" {
#endif

struct rkRgaCfg {
    int width;
    int height;
    int fmt;
    int fd;
    void *addr;
};

void rkRgaInit();
int rkRgaBlit(struct rkRgaCfg *src_cfg, struct rkRgaCfg *dst_cfg);

#ifdef __cplusplus
};
#endif
#endif

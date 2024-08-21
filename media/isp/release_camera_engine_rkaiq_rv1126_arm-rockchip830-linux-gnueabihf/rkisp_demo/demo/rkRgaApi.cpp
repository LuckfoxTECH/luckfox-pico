#include "RockchipRga.h"
#include "rkRgaApi.h"

#ifdef __cplusplus
extern "C" {
#endif

static RockchipRga rkRga;

void rkRgaInit()
{
    rkRga.RkRgaInit();
}

int rkRgaBlit(struct rkRgaCfg *src_cfg, struct rkRgaCfg *dst_cfg)
{
    int ret = 0;

    /********** rga_info_t Init **********/
    rga_info_t src;
    rga_info_t dst;

    memset(&src, 0, sizeof(rga_info_t));
    src.fd = -1;
    src.mmuFlag = 1;
    src.virAddr = src_cfg->addr;

    memset(&dst, 0, sizeof(rga_info_t));
    dst.fd = -1;
    dst.mmuFlag = 1;
    dst.virAddr = dst_cfg->addr;

    /********** set the rect_info **********/
    rga_set_rect(&src.rect, 0, 0, src_cfg->width, src_cfg->height, src_cfg->width/*stride*/, src_cfg->height, src_cfg->fmt);
    rga_set_rect(&dst.rect, 0, 0, dst_cfg->width, dst_cfg->height, dst_cfg->width/*stride*/, dst_cfg->height, dst_cfg->fmt);

    /************ set the rga_mod ,rotation\composition\scale\copy .... **********/
#if 0
    src.rotation = 0;
    src.rotation = HAL_TRANSFORM_ROT_90;
    src.rotation = HAL_TRANSFORM_ROT_180;
    src.rotation = HAL_TRANSFORM_ROT_270;
    src.rotation = HAL_TRANSFORM_FLIP_V;
    src.rotation = HAL_TRANSFORM_FLIP_H;
#endif

    /********** call rga_Interface **********/
    ret = rkRga.RkRgaBlit(&src, &dst, NULL);
    if (ret) {
	    printf("rgaFillColor error : %s\n", strerror(errno));
    }

    return ret;
}

#ifdef __cplusplus
};
#endif

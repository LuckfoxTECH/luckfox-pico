#include "sample_file.h"

#include "sample_define.h"
#include "sample_mem.h"

#include <string.h>

RK_VOID RK_STRCPY(RK_CHAR *dest, size_t size, const RK_CHAR *src) {
#if defined WIN32 || defined _WIN32 || defined WINCE
    strcpy_s(dest, size, src);
#else
    strcpy(dest, src);
#endif
}

RK_VOID* RK_Memset(RK_VOID *ptr, RK_S32 value, size_t num) {
#if BUILD_SDK_ENV
    SYS_MmzFlush_Start(ptr);
#endif
    memset(ptr, value, num);
#if BUILD_SDK_ENV
    SYS_MmzFlush_End(ptr);
#endif
    return 0;
}

RK_S32 RK_Memcpy(void *dest, void* src, size_t size) {
#if BUILD_SDK_ENV
    SYS_MmzFlush_Start(src);
    SYS_MmzFlush_Start(dest);
#endif
    memcpy(dest, src, size);
#if BUILD_SDK_ENV
    SYS_MmzFlush_End(dest);
    SYS_MmzFlush_End(src);
#endif
    return 0;
}

RK_S32 RK_ReadFileSize(RK_CHAR *filename, FILE **fp_out) {
    RK_S32 size = -1;
    FILE *fp = *fp_out;

#if defined WIN32 || defined _WIN32 || defined WINCE
    fopen_s(&fp, filename, "rb");
#else
    fp = fopen(filename, "rb");
#endif
    if (fp == NULL) {
        printf("failed to open(%s)\n", filename);
        goto end;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    *fp_out = fp;

end:
    return size;
}

RK_S32 RK_ReadFile(RK_CHAR *filename, IVE_SRC_IMAGE_S *pstImage, FILE **fp_out) {
    int i;
    RK_U16 y;
    RK_S32 u32Succ;
    RK_U8 *pData;
    FILE *fp = *fp_out;

    RK_CHECK_ET_NULL_RET(pstImage, RK_FAILURE);

#if BUILD_SDK_ENV
    SYS_MmzFlush_Start((void *)pstImage->au64VirAddr[0]);
#endif

    if (fp == NULL) {
        if (filename == NULL) {
#if BUILD_SLT_UTC
            pseudo_data(pstImage);
            u32Succ = 0;
#else
            printf("filename is null\n");
            u32Succ = -1;
#endif
            goto end;
        }
#if defined WIN32 || defined _WIN32 || defined WINCE
        fopen_s(&fp, filename, "rb");
#else
        fp = fopen(filename, "rb");
#endif

        if (fp == NULL) {
            printf("failed to open(%s)\n", filename);
        }
    }

    RK_CHECK_ET_NULL_RET(fp, RK_FAILURE);

    if (feof(fp)) {
        fseek(fp, 0, SEEK_SET);
    }

    u32Succ = RK_SUCCESS;

    switch (pstImage->enType) {
        case IVE_IMAGE_TYPE_U8C1: {
            pData = (RK_U8 *)pstImage->au64VirAddr[0];
            for (y = 0; y < pstImage->u32Height;
                 y++, pData += pstImage->au32Stride[0]) {
                if (1 != fread(pData, pstImage->u32Width, 1, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }

        } break;
        case IVE_IMAGE_TYPE_YUV420SP: {
            pData = (RK_U8 *)pstImage->au64VirAddr[0];
            for (y = 0; y < pstImage->u32Height;
                 y++, pData += pstImage->au32Stride[0]) {
                if (1 != fread(pData, pstImage->u32Width, 1, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }

            pData = (RK_U8 *)pstImage->au64VirAddr[1];
            for (y = 0; y < pstImage->u32Height / 2;
                 y++, pData += pstImage->au32Stride[1]) {
                if (1 != fread(pData, pstImage->u32Width, 1, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }
        } break;
        case IVE_IMAGE_TYPE_YUV422SP: {
            pData = (RK_U8 *)pstImage->au64VirAddr[0];
            for (y = 0; y < pstImage->u32Height;
                 y++, pData += pstImage->au32Stride[0]) {
                if (1 != fread(pData, pstImage->u32Width, 1, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }

            pData = (RK_U8 *)pstImage->au64VirAddr[1];
            for (y = 0; y < pstImage->u32Height;
                 y++, pData += pstImage->au32Stride[1]) {
                if (1 != fread(pData, pstImage->u32Width, 1, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }
        } break;
        case IVE_IMAGE_TYPE_U8C3_PACKAGE: {
            pData = (RK_U8 *)pstImage->au64VirAddr[0];
            for (y = 0; y < pstImage->u32Height;
                 y++, pData += pstImage->au32Stride[0] * 3) {
                if (1 != fread(pData, pstImage->u32Width * 3, 1, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }
        } break;
        case IVE_IMAGE_TYPE_U8C3_PLANAR: {
            for (i = 0; i < 3; i++) {
                pData = (RK_U8 *)pstImage->au64VirAddr[i];
                for (y = 0; y < pstImage->u32Height;
                     y++, pData += pstImage->au32Stride[i]) {
                    if (1 != fread(pData, pstImage->u32Width, 1, fp)) {
                        u32Succ = RK_FAILURE;
                        break;
                    }
                }
            }

        } break;
        case IVE_IMAGE_TYPE_S16C1:
        case IVE_IMAGE_TYPE_U16C1: {
            pData = (RK_U8 *)pstImage->au64VirAddr[0];
            for (y = 0; y < pstImage->u32Height;
                 y++, pData += pstImage->au32Stride[0] * 2) {
                if (2 != fread(pData, pstImage->u32Width, 2, fp)) {
                    u32Succ = RK_FAILURE;
                    break;
                }
            }
        } break;
        default:
            u32Succ = RK_FAILURE;
            break;
    }

    *fp_out = fp;

end:
#if BUILD_SDK_ENV
    SYS_MmzFlush_End((void *)pstImage->au64VirAddr[0]);
#endif

    return u32Succ;
}

RK_S32 RK_WriteFile(RK_CHAR *filename, void *p_buf, RK_U32 size, FILE **fp_out) {
    int u32Succ = 0;
    FILE *fp = *fp_out;

#if BUILD_SDK_ENV
    SYS_MmzFlush_Start(p_buf);
#endif

    if (fp == NULL) {
        if (filename == NULL) {
#if BUILD_SLT_UTC
            check_crc(p_buf, size);
#else
            printf("filename is null\n");
            u32Succ = -1;
#endif
            goto end;
        }

 #if defined WIN32 || defined _WIN32 || defined WINCE
        fopen_s(&fp, filename, "wb");
 #else
        fp = fopen(filename, "wb");
#endif
        if (fp == NULL) {
            printf("failed to open(%s)\n", filename);
            return -1;
        }
    }

    fwrite(p_buf, size, 1, fp);

    *fp_out = fp;

end:
#if BUILD_SDK_ENV
    SYS_MmzFlush_End(p_buf);
#endif

    return u32Succ;
}

#include <rtthread.h>
#include <stdio.h>

#undef ALIGN
#define ALIGN(x, n) ((x + (n - 1)) & ~(n - 1))
typedef struct                       /**** BMP file header structure ****/
{
    unsigned int   bfSize;           /* Size of file */
    unsigned short bfReserved1;      /* Reserved */
    unsigned short bfReserved2;      /* ... */
    unsigned int   bfOffBits;        /* Offset to bitmap data */
} MyBITMAPFILEHEADER;

typedef struct                       /**** BMP file info structure ****/
{
    unsigned int   biSize;           /* Size of info header */
    int            biWidth;          /* Width of image */
    int            biHeight;         /* Height of image */
    unsigned short biPlanes;         /* Number of color planes */
    unsigned short biBitCount;       /* Number of bits per pixel */
    unsigned int   biCompression;    /* Type of compression to use */
    unsigned int   biSizeImage;      /* Size of image data */
    int            biXPelsPerMeter;  /* X pixels per meter */
    int            biYPelsPerMeter;  /* Y pixels per meter */
    unsigned int   biClrUsed;        /* Number of colors used */
    unsigned int   biClrImportant;   /* Number of important colors */
} MyBITMAPINFOHEADER;

void SaveBmp(const char *filename, unsigned char *rgbbuf, int width, int height, int bytePerDot, int stride)
{
    MyBITMAPFILEHEADER bfh;
    MyBITMAPINFOHEADER bih;
    /* Magic number for file. It does not fit in the header structure due to alignment requirements, so put it outside */
    unsigned short bfType = 0x4d42;
    unsigned char *buf = rgbbuf;
    unsigned char pad[4] = {0};

    bfh.bfReserved1 = 0;
    bfh.bfReserved2 = 0;
    bfh.bfSize = 2 + sizeof(MyBITMAPFILEHEADER) + sizeof(MyBITMAPINFOHEADER) + width * height * bytePerDot;
    bfh.bfOffBits = 0x36;

    bih.biSize = sizeof(MyBITMAPINFOHEADER);
    bih.biWidth = width;
    bih.biHeight = height;
    bih.biPlanes = 1;
    bih.biBitCount = bytePerDot * 8;
    bih.biCompression = 0;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 5000;
    bih.biYPelsPerMeter = 5000;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    FILE *file;
    file = fopen(filename, "wb");
    if (!file)
    {
        printf("Could not write file\n");
        return;
    }

    /*Write headers*/
    fwrite(&bfType, sizeof(bfType), 1, file);
    fwrite(&bfh, sizeof(bfh), 1, file);
    fwrite(&bih, sizeof(bih), 1, file);

    buf += (height - 1) * stride;
    for (int i = 0; i < height; i++)
    {
        fwrite(buf, width * bytePerDot, 1, file);
        if ((width * bytePerDot) % 4 != 0)
            fwrite(pad, ALIGN(width * bytePerDot, 4) - width * bytePerDot, 1, file);
        buf -= stride;
    }

    fclose(file);
}
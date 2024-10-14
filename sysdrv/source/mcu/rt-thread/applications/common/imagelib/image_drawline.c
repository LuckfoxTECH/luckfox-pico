/**
  * Copyright (c) 2020 Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtthread.h>
#include <math.h>

#include "lib_imageprocess.h"

int ipart(float x)
{
    return floor(x);
}

int mround(float x)
{
    return ipart(x + 0.5);
}

float fpart(float x)
{
    return x - floor(x);
}

float rfpart(float x)
{
    return 1 - fpart(x);
}

void drawLine(uint8_t *fb, float x0, float y0, float x1, float y1,
              void (*plot)(uint8_t *fb, int x, int y, float c),
              void (*plot_shadow)(uint8_t* fb, int x, int y, float c))
{
    int steep = ABS(y1 - y0) > ABS(x1 - x0);

    if (steep)
    {
        float temp;
        temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    if (x0 > x1)
    {
        float temp;
        temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }

    float dx = x1 - x0;
    float dy = y1 - y0;
    float gradient;
    if (dx == 0.0)
        gradient = 1.0;
    else
        gradient = dy / dx;

    // handle first endpoint
    int xend = mround(x0);
    int yend = y0 + gradient * (xend - x0);
    float xgap = rfpart(x0 + 0.5);
    int xpxl1 = xend; // this will be used in the main loop
    int ypxl1 = ipart(yend);
    if (steep)
    {
        if (plot_shadow)
        {
            for (int i = xpxl1; i > 0; i--)
            {
                plot_shadow(fb, ypxl1, i, 0);
                plot_shadow(fb, ypxl1 + 1, i, 0);
            }
        }
        plot(fb, ypxl1, xpxl1, rfpart(yend) * xgap);
        plot(fb, ypxl1 + 1, xpxl1, fpart(yend) * xgap);
    }
    else
    {
        if (plot_shadow)
        {
            for (int i = ypxl1; i > 0; i--)
            {
                plot_shadow(fb, xpxl1, i, 0);
            }
        }
        plot(fb, xpxl1, ypxl1, rfpart(yend) * xgap);
        plot(fb, xpxl1, ypxl1 + 1, fpart(yend) * xgap);
    }
    float intery = yend + gradient; // first y-intersection for the main loop

    // handle second endpoint
    xend = mround(x1);
    yend = y1 + gradient * (xend - x1);
    xgap = fpart(x1 + 0.5);
    int xpxl2 = xend; //this will be used in the main loop
    int ypxl2 = ipart(yend);
    if (steep)
    {
        if (plot_shadow)
        {
            for (int i = xpxl2; i > 0; i--)
            {
                plot_shadow(fb, ypxl2, i, 0);
                plot_shadow(fb, ypxl2 + 1, i, 0);
            }
        }
        plot(fb, ypxl2, xpxl2, rfpart(yend) * xgap);
        plot(fb, ypxl2 + 1, xpxl2, fpart(yend) * xgap);
    }
    else
    {
        if (plot_shadow)
        {
            for (int i = ypxl2; i > 0; i--)
            {
                plot_shadow(fb, xpxl2, i, 0);
            }
        }
        plot(fb, xpxl2, ypxl2, rfpart(yend) * xgap);
        plot(fb, xpxl2, ypxl2 + 1, fpart(yend) * xgap);
    }

    // main loop
    if (steep)
    {
        for (int x = xpxl1 + 1; x <= xpxl2 - 1; x++)
        {
            if (plot_shadow)
            {
                for (int i = x; i > 0; i--)
                {
                    plot_shadow(fb, ipart(intery), i, 0);
                    plot_shadow(fb, ipart(intery) + 1, i, 0);
                }
            }
            plot(fb, ipart(intery), x, rfpart(intery));
            plot(fb, ipart(intery) + 1, x, fpart(intery));
            intery = intery + gradient;
        }
    }
    else
    {
        for (int x = xpxl1 + 1; x <= xpxl2 - 1; x++)
        {
            if (plot_shadow)
            {
                for (int i = ipart(intery); i > 0; i--)
                {
                    plot_shadow(fb, x, i, 0);
                }
            }
            plot(fb, x, ipart(intery), rfpart(intery));
            plot(fb, x, ipart(intery) + 1, fpart(intery));
            intery = intery + gradient;
        }
    }
}

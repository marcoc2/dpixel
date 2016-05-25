/*
 * Copyright (C) 2003 Maxim Stepin ( maxst@hiend3d.com )
 *
 * Copyright (C) 2010 Cameron Zemek ( grom@zeminvaders.net)
 * Copyright (C) 2011 Francois Gannaz <mytskine@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef __HQX_COMMON_H_
#define __HQX_COMMON_H_

#include <stdlib.h>
#include <stdint.h>

#if defined(__linux__) || defined(_WIN32)
#include <omp.h>
#endif

#define MASK_2     0x0000FF00
#define MASK_13    0x00FF00FF
#define MASK_RGB   0x00FFFFFF
#define MASK_ALPHA 0xFF000000

#define Ymask 0x00FF0000
#define Umask 0x0000FF00
#define Vmask 0x000000FF
#define trY   0x00300000
#define trU   0x00000700
#define trV   0x00000006

#ifdef _WIN32
#define u_int32_t uint32_t
#define u_int8_t uint8_t
#elif defined __clang__
typedef u_int32_t uint32_t;
typedef uint8_t u_int8_t;
#endif

/* RGB to YUV lookup table */
u_int32_t RGBtoYUV[16777216];

static inline u_int32_t rgb_to_yuv(u_int32_t c)
{
    // Mask against MASK_RGB to discard the alpha channel
    return RGBtoYUV[MASK_RGB & c];
}

/* Test if there is difference in color */
static inline int yuv_diff(u_int32_t yuv1, u_int32_t yuv2) {
    return (( abs((yuv1 & Ymask) - (yuv2 & Ymask)) > trY ) ||
            ( abs((yuv1 & Umask) - (yuv2 & Umask)) > trU ) ||
            ( abs((yuv1 & Vmask) - (yuv2 & Vmask)) > trV ) );
}


void hqxInit(void)
{
    /* Initalize RGB to YUV lookup table */
    u_int c, r, g, b, y, u, v;

    #pragma omp parallel for
    for (c = 0; c < 16777215; c++) {
        r = (c & 0xFF0000) >> 16;
        g = (c & 0x00FF00) >> 8;
        b = c & 0x0000FF;
        y = (u_int)(0.299*r + 0.587*g + 0.114*b);
        u = (u_int)(-0.169*r - 0.331*g + 0.5*b) + 128;
        v = (u_int)(0.5*r - 0.419*g - 0.081*b) + 128;
        RGBtoYUV[c] = (y << 16) + (u << 8) + v;
    }
}

static inline int Diff(u_int32_t c1, u_int32_t c2)
{
    return yuv_diff(rgb_to_yuv(c1), rgb_to_yuv(c2));
}

/* Interpolate functions */
static inline u_int32_t Interpolate_2(u_int32_t c1, int w1, u_int32_t c2, int w2, int s)
{
    if (c1 == c2) {
        return c1;
    }
    return
        (((((c1 & MASK_ALPHA) >> 24) * w1 + ((c2 & MASK_ALPHA) >> 24) * w2) << (24-s)) & MASK_ALPHA) +
        ((((c1 & MASK_2) * w1 + (c2 & MASK_2) * w2) >> s) & MASK_2)	+
        ((((c1 & MASK_13) * w1 + (c2 & MASK_13) * w2) >> s) & MASK_13);
}

static inline u_int32_t Interpolate_3(u_int32_t c1, int w1, u_int32_t c2, int w2, u_int32_t c3, int w3, int s)
{
    return
        (((((c1 & MASK_ALPHA) >> 24) * w1 + ((c2 & MASK_ALPHA) >> 24) * w2 + ((c3 & MASK_ALPHA) >> 24) * w3) << (24-s)) & MASK_ALPHA) +
        ((((c1 & MASK_2) * w1 + (c2 & MASK_2) * w2 + (c3 & MASK_2) * w3) >> s) & MASK_2) +
        ((((c1 & MASK_13) * w1 + (c2 & MASK_13) * w2 + (c3 & MASK_13) * w3) >> s) & MASK_13);
}

static inline u_int32_t Interp1(u_int32_t c1, u_int32_t c2)
{
    //(c1*3+c2) >> 2;
    return Interpolate_2(c1, 3, c2, 1, 2);
}

static inline u_int32_t Interp2(u_int32_t c1, u_int32_t c2, u_int32_t c3)
{
    //(c1*2+c2+c3) >> 2;
    return Interpolate_3(c1, 2, c2, 1, c3, 1, 2);
}

static inline u_int32_t Interp3(u_int32_t c1, u_int32_t c2)
{
    //(c1*7+c2)/8;
    return Interpolate_2(c1, 7, c2, 1, 3);
}

static inline u_int32_t Interp4(u_int32_t c1, u_int32_t c2, u_int32_t c3)
{
    //(c1*2+(c2+c3)*7)/16;
    return Interpolate_3(c1, 2, c2, 7, c3, 7, 4);
}

static inline u_int32_t Interp5(u_int32_t c1, u_int32_t c2)
{
    //(c1+c2) >> 1;
    return Interpolate_2(c1, 1, c2, 1, 1);
}

static inline u_int32_t Interp6(u_int32_t c1, u_int32_t c2, u_int32_t c3)
{
    //(c1*5+c2*2+c3)/8;
    return Interpolate_3(c1, 5, c2, 2, c3, 1, 3);
}

static inline u_int32_t Interp7(u_int32_t c1, u_int32_t c2, u_int32_t c3)
{
    //(c1*6+c2+c3)/8;
    return Interpolate_3(c1, 6, c2, 1, c3, 1, 3);
}

static inline u_int32_t Interp8(u_int32_t c1, u_int32_t c2)
{
    //(c1*5+c2*3)/8;
    return Interpolate_2(c1, 5, c2, 3, 3);
}

static inline u_int32_t Interp9(u_int32_t c1, u_int32_t c2, u_int32_t c3)
{
    //(c1*2+(c2+c3)*3)/8;
    return Interpolate_3(c1, 2, c2, 3, c3, 3, 3);
}

static inline u_int32_t Interp10(u_int32_t c1, u_int32_t c2, u_int32_t c3)
{
    //(c1*14+c2+c3)/16;
    return Interpolate_3(c1, 14, c2, 1, c3, 1, 4);
}

#endif

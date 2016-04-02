#include <stdint.h>
#include <stdlib.h>
#include "xbr.h"

namespace xbr{

/*
   Hyllian's 2xBR v3.3b
   
   Copyright (C) 2011, 2012 Hyllian/Jararaca - sergiogdb@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#define RGB_MASK 0x00FFFFFF
#define RED_BLUE_MASK 0x00FF00FF
#define RED_MASK   0x00FF0000
#define GREEN_MASK 0x0000FF00
#define BLUE_MASK  0x000000FF

#define PG_LBMASK 0x00FFFFFF

#define Ymask 0x00FF0000
#define Umask 0x0000FF00
#define Vmask 0x000000FF
#define trY   0x00300000
#define trU   0x00000700
#define trV   0x00000006

#ifdef _WIN32
#define u_int32_t unsigned int
#endif

/* RGB to YUV lookup table */
u_int32_t RGBtoYUV[16777216];

static inline u_int32_t rgb_to_yuv(u_int32_t c)
{
    // Mask against MASK_RGB to discard the alpha channel
    return RGBtoYUV[RGB_MASK & c];
}

/* Test if there is difference in color */
static inline int yuv_diff(u_int32_t yuv1, u_int32_t yuv2) {
    return (( abs((yuv1 & Ymask) - (yuv2 & Ymask)) > trY ) ||
            ( abs((yuv1 & Umask) - (yuv2 & Umask)) > trU ) ||
            ( abs((yuv1 & Vmask) - (yuv2 & Vmask)) > trV ) );
}

void tableInit(void)
{
    /* Initalize RGB to YUV lookup table */
    u_int32_t c, r, g, b, y, u, v;
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

//static u_int RGBtoYUV[65536];
//static u_int tbl_5_to_8[32]={0, 8, 16, 25, 33, 41, 49,  58, 66, 74, 82, 90, 99, 107, 115, 123, 132, 140, 148, 156, 165, 173, 181, 189,  197, 206, 214, 222, 230, 239, 247, 255};
//static u_int tbl_6_to_8[64]={0, 4, 8, 12, 16, 20, 24,  28, 32, 36, 40, 45, 49, 53, 57, 61, 65, 69, 73, 77, 81, 85, 89, 93, 97, 101,  105, 109, 113, 117, 121, 125, 130, 134, 138, 142, 146, 150, 154, 158, 162, 166,  170, 174, 178, 182, 186, 190, 194, 198, 202, 206, 210, 215, 219, 223, 227, 231,  235, 239, 243, 247, 251, 255};

static const u_int pg_red_blue_mask = RED_BLUE_MASK;
static const u_int pg_red_mask = RED_MASK;
static const u_int pg_green_mask = GREEN_MASK;
static const u_int pg_blue_mask = BLUE_MASK;
static const u_int pg_lbmask = PG_LBMASK;

#define ALPHA_BLEND_128_W(dst, src) dst = ((src & pg_lbmask) >> 1) + ((dst & pg_lbmask) >> 1)

#define ALPHA_BLEND_32_W(dst, src) \
        dst = ( \
                    (pg_red_blue_mask & ((dst & pg_red_blue_mask) + \
                                                 ((((src & pg_red_blue_mask) - \
                                                            (dst & pg_red_blue_mask))) >>3))) | \
                    (pg_green_mask & ((dst & pg_green_mask) + \
                                              ((((src & pg_green_mask) - \
                                                         (dst & pg_green_mask))) >>3))))
 
#define ALPHA_BLEND_64_W(dst, src) \
        dst = ( \
                    (pg_red_blue_mask & ((dst & pg_red_blue_mask) + \
                                                 ((((src & pg_red_blue_mask) - \
                                                            (dst & pg_red_blue_mask))) >>2))) | \
                    (pg_green_mask & ((dst & pg_green_mask) + \
                                              ((((src & pg_green_mask) - \
                                                         (dst & pg_green_mask))) >>2))))

#define ALPHA_BLEND_X_W(dst, src, alpha) \
   ts = src; td = dst;\
   td = ((td|(td<<16)) & RGB_MASK); ts = ((ts|(ts<<16)) & RGB_MASK);\
   td = ((( ( (ts-td)*alpha ) >> 5 ) + td ) & RGB_MASK); \
   dst= (td|(td>>16));

#define ALPHA_BLEND_192_W(dst, src) ALPHA_BLEND_X_W(dst, src, 24)
#define ALPHA_BLEND_224_W(dst, src) ALPHA_BLEND_X_W(dst, src, 28)
 
/*
#define ALPHA_BLEND_192_W(dst, src) \
        dst = ( \
                    (pg_red_blue_mask & ((dst & pg_red_blue_mask) + \
                                                 ((((src & pg_red_blue_mask) - \
                                                            (dst & pg_red_blue_mask)) * 3) >>2))) | \
                    (pg_green_mask & ((dst & pg_green_mask) + \
                                              ((((src & pg_green_mask) - \
                                                         (dst & pg_green_mask)) * 3) >>2))))
 
#define ALPHA_BLEND_224_W(dst, src) \
        dst = ( \
                    (pg_red_blue_mask & ((dst & pg_red_blue_mask) + \
                                                 ((((src & pg_red_blue_mask) - \
                                                            (dst & pg_red_blue_mask)) * 7) >>3))) | \
                    (pg_green_mask & ((dst & pg_green_mask) + \
                                              ((((src & pg_green_mask) - \
                                                         (dst & pg_green_mask)) * 7) >>3))))
*/

/* Old alpha blend for separate red/green/blue components */
/*
#define ALPHA_BLEND_32_W(dst, src) \
	dst = ( \
    (pg_red_mask & ((dst & pg_red_mask) + \
        ((((src & pg_red_mask) - \
        (dst & pg_red_mask))) >>3))) | \
    (pg_green_mask & ((dst & pg_green_mask) + \
        ((((src & pg_green_mask) - \
        (dst & pg_green_mask))) >>3))) | \
    (pg_blue_mask & ((dst & pg_blue_mask) + \
        ((((src & pg_blue_mask) - \
        (dst & pg_blue_mask))) >>3))) )

#define ALPHA_BLEND_64_W(dst, src) \
	dst = ( \
    (pg_red_mask & ((dst & pg_red_mask) + \
        ((((src & pg_red_mask) - \
        (dst & pg_red_mask))) >>2))) | \
    (pg_green_mask & ((dst & pg_green_mask) + \
        ((((src & pg_green_mask) - \
        (dst & pg_green_mask))) >>2))) | \
    (pg_blue_mask & ((dst & pg_blue_mask) + \
        ((((src & pg_blue_mask) - \
        (dst & pg_blue_mask))) >>2))) )

#define ALPHA_BLEND_192_W(dst, src) \
	dst = ( \
    (pg_red_mask & ((dst & pg_red_mask) + \
        ((((src & pg_red_mask) - \
        (dst & pg_red_mask)) * 192) >>8))) | \
    (pg_green_mask & ((dst & pg_green_mask) + \
        ((((src & pg_green_mask) - \
        (dst & pg_green_mask)) * 192) >>8))) | \
    (pg_blue_mask & ((dst & pg_blue_mask) + \
        ((((src & pg_blue_mask) - \
        (dst & pg_blue_mask)) * 192) >>8))) )
        
#define ALPHA_BLEND_224_W(dst, src) \
	dst = ( \
    (pg_red_mask & ((dst & pg_red_mask) + \
        ((((src & pg_red_mask) - \
        (dst & pg_red_mask)) * 224) >>8))) | \
    (pg_green_mask & ((dst & pg_green_mask) + \
        ((((src & pg_green_mask) - \
        (dst & pg_green_mask)) * 224) >>8))) | \
    (pg_blue_mask & ((dst & pg_blue_mask) + \
        ((((src & pg_blue_mask) - \
        (dst & pg_blue_mask)) * 224) >>8))) );
*/


#define LEFT_UP_2_2X(N3, N2, N1, PIXEL)\
             ALPHA_BLEND_224_W(E[N3], PIXEL); \
             ALPHA_BLEND_64_W( E[N2], PIXEL); \
             E[N1] = E[N2]; \

        
#define LEFT_2_2X(N3, N2, PIXEL)\
             ALPHA_BLEND_192_W(E[N3], PIXEL); \
             ALPHA_BLEND_64_W( E[N2], PIXEL); \

#define UP_2_2X(N3, N1, PIXEL)\
             ALPHA_BLEND_192_W(E[N3], PIXEL); \
             ALPHA_BLEND_64_W( E[N1], PIXEL); \

#define DIA_2X(N3, PIXEL)\
             ALPHA_BLEND_128_W(E[N3], PIXEL); \

#define df(A, B)\
        abs((int)(RGBtoYUV[A] - RGBtoYUV[B]))\

#define eq(A, B)\
        (df(A, B) < 155)\


#define LEFT_UP_2_3X(N7, N5, N6, N2, N8, PIXEL)\
             ALPHA_BLEND_192_W(E[N7], PIXEL); \
             ALPHA_BLEND_64_W( E[N6], PIXEL); \
             E[N5] = E[N7]; \
             E[N2] = E[N6]; \
             E[N8] =  PIXEL;\

        
#define LEFT_2_3X(N7, N5, N6, N8, PIXEL)\
             ALPHA_BLEND_192_W(E[N7], PIXEL); \
             ALPHA_BLEND_64_W( E[N5], PIXEL); \
             ALPHA_BLEND_64_W( E[N6], PIXEL); \
             E[N8] =  PIXEL;\

#define UP_2_3X(N5, N7, N2, N8, PIXEL)\
             ALPHA_BLEND_192_W(E[N5], PIXEL); \
             ALPHA_BLEND_64_W( E[N7], PIXEL); \
             ALPHA_BLEND_64_W( E[N2], PIXEL); \
             E[N8] =  PIXEL;\

#define DIA_3X(N8, N5, N7, PIXEL)\
             ALPHA_BLEND_224_W(E[N8], PIXEL); \
             ALPHA_BLEND_32_W(E[N5], PIXEL); \
             ALPHA_BLEND_32_W(E[N7], PIXEL); \

#define LEFT_UP_2(N15, N14, N11, N13, N12, N10, N7, N3, PIXEL)\
                                ALPHA_BLEND_192_W(E[N13], PIXEL); \
                                ALPHA_BLEND_64_W( E[N12], PIXEL); \
                                E[N15] = E[N14] = E[N11] = PIXEL; \
                                E[N10] = E[N3] = E[N12]; \
                                E[N7]  = E[N13]; \

#define LEFT_2(N15, N14, N11, N13, N12, N10, PIXEL)\
                                ALPHA_BLEND_192_W(E[N11], PIXEL); \
                                ALPHA_BLEND_192_W(E[N13], PIXEL); \
                                ALPHA_BLEND_64_W( E[N10], PIXEL); \
                                ALPHA_BLEND_64_W( E[N12], PIXEL); \
                                E[N14] = PIXEL; \
                                E[N15] = PIXEL; \

#define UP_2(N15, N14, N11, N3, N7, N10, PIXEL)\
                                ALPHA_BLEND_192_W(E[N14], PIXEL); \
                                ALPHA_BLEND_192_W(E[N7 ], PIXEL); \
                                ALPHA_BLEND_64_W( E[N10], PIXEL); \
                                ALPHA_BLEND_64_W( E[N3 ], PIXEL); \
                                E[N11] = PIXEL; \
                                E[N15] = PIXEL; \

#define DIA(N15, N14, N11, PIXEL)\
                        ALPHA_BLEND_128_W(E[N11], PIXEL); \
                        ALPHA_BLEND_128_W(E[N14], PIXEL); \
                        E[N15] = PIXEL; \

#define FILTRO(PE, PI, PH, PF, PG, PC, PD, PB, PA, G5, C4, G0, D0, C1, B1, F4, I4, H5, I5, A0, A1, N15, N14, N11, N3, N7, N10, N13, N12, N9, N6, N2, N1, N5, N8, N4, N0) \
     ex   = (PE!=PH && PE!=PF); \
     if ( ex )\
     {\
          e = (df(PE,PC)+df(PE,PG)+df(PI,H5)+df(PI,F4))+(df(PH,PF)<<2); \
          i = (df(PH,PD)+df(PH,I5)+df(PF,I4)+df(PF,PB))+(df(PE,PI)<<2); \
          if ((e<i)  && ( !eq(PF,PB) && !eq(PH,PD) || eq(PE,PI) && (!eq(PF,I4) && !eq(PH,I5)) || eq(PE,PG) || eq(PE,PC)) )\
          {\
              ke=df(PF,PG); ki=df(PH,PC); \
              ex2 = (PE!=PC && PB!=PC); ex3 = (PE!=PG && PD!=PG); px = (df(PE,PF) <= df(PE,PH)) ? PF : PH; \
              if ( ((ke<<1)<=ki) && ex3 && (ke>=(ki<<1)) && ex2 ) \
              {\
                     LEFT_UP_2(N15, N14, N11, N13, N12, N10, N7, N3, px)\
              }\
              else if ( ((ke<<1)<=ki) && ex3 ) \
              {\
                     LEFT_2(N15, N14, N11, N13, N12, N10, px)\
              }\
              else if ( (ke>=(ki<<1)) && ex2 ) \
              {\
                     UP_2(N15, N14, N11, N3, N7, N10, px)\
              }\
              else \
              {\
                     DIA(N15, N14, N11, px)\
              }\
          }\
          else if (e<=i)\
          {\
               ALPHA_BLEND_128_W( E[N15], ((df(PE,PF) <= df(PE,PH)) ? PF : PH)); \
          }\
     }\

void xbr4x(u_char* input, u_char* output, u_int inputWidth, u_int inputHeight )
{
    tableInit();

    const u_int outputPitch = inputWidth * 4;
    const u_int inputPitch = inputWidth;

    const int nl = outputPitch / 2;
    const int nl1 = nl + nl;
    const int nl2 = nl1 + nl;
    u_int ts, td;

    for (u_int y = 0; y < inputHeight; y++){
        u_int * E = (u_int *)( output + y * outputPitch * 4 );

        /* middle. the -4 just makes the offsets later on work out */
        u_int * sa2 = (u_int *)( input + y * inputPitch - 4 );
        /* up one */
        u_int * sa1 = sa2 - inputPitch;
        /* up two */
        u_int * sa0 = sa1 - inputPitch;
        /* down one */
        u_int * sa3 = sa2 + inputPitch;
        /* down two */
        u_int * sa4 = sa3 + inputPitch;

        if (y <= 1){ 
            sa0 = sa1;
            if (y == 0){
                sa0 = sa1 = sa2;
            }
        }

        if (y >= inputHeight - 2){
            sa4 = sa3;
            if (y == inputHeight - 1){
                sa4 = sa3 = sa2;
            }
        }

        unsigned char pprev;
        unsigned char pprev2;
        pprev = pprev2 = 2;

        for (u_int x = 0; x < inputPitch; x++){
            u_int B1 = sa0[2];
            u_int PB = sa1[2];
            u_int PE = sa2[2];
            u_int PH = sa3[2];
            u_int H5 = sa4[2];

            u_int A1 = sa0[pprev];
            u_int PA = sa1[pprev];
            u_int PD = sa2[pprev];
            u_int PG = sa3[pprev];
            u_int G5 = sa4[pprev];

            u_int A0 = sa1[pprev2];
            u_int D0 = sa2[pprev2];
            u_int G0 = sa3[pprev2];

            u_int C1 = 0;
            u_int PC = 0;
            u_int PF = 0;
            u_int PI = 0;
            u_int I5 = 0;

            u_int C4 = 0;
            u_int F4 = 0;
            u_int I4 = 0;

            if (x >= inputWidth - 2){
                if (x == inputWidth - 1){
                    C1 = sa0[2];
                    PC = sa1[2];
                    PF = sa2[2];
                    PI = sa3[2];
                    I5 = sa4[2];

                    C4 = sa1[2];
                    F4 = sa2[2];
                    I4 = sa3[2];
                } else {
                    C1 = sa0[3];
                    PC = sa1[3];
                    PF = sa2[3];
                    PI = sa3[3];
                    I5 = sa4[3];
                
                    C4 = sa1[3];
                    F4 = sa2[3];
                    I4 = sa3[3];
                }
            } else {
                C1 = sa0[3];
                PC = sa1[3];
                PF = sa2[3];
                PI = sa3[3];
                I5 = sa4[3];

                C4 = sa1[4];
                F4 = sa2[4];
                I4 = sa3[4];
            }

            u_int e, i, p[10], px;
            u_int ex, ex2, ex3;
            u_int ke, ki;

            E[0]   = E[1]     = E[2]     = E[3]     = PE;
            E[nl]  = E[nl+1]  = E[nl+2]  = E[nl+3]  = PE; //  4,  5,  6,  7
            E[nl1] = E[nl1+1] = E[nl1+2] = E[nl1+3] = PE; //  8,  9, 10, 11
            E[nl2] = E[nl2+1] = E[nl2+2] = E[nl2+3] = PE; // 12, 13, 14, 15

            FILTRO(PE, PI, PH, PF, PG, PC, PD, PB, PA, G5, C4, G0, D0, C1, B1, F4, I4, H5, I5, A0, A1, nl2+3, nl2+2, nl1+3,  3,  nl+3, nl1+2, nl2+1, nl2,  nl1+1,  nl+2, 2,  1, nl+1, nl1, nl, 0);
            FILTRO(PE, PC, PF, PB, PI, PA, PH, PD, PG, I4, A1, I5, H5, A0, D0, B1, C1, F4, C4, G5, G0,  3,  nl+3,  2,  0,  1,  nl+2, nl1+3, nl2+3, nl1+2,  nl+1, nl,  nl1, nl1+1,nl2+2,nl2+1,nl2);
            FILTRO(PE, PA, PB, PD, PC, PG, PF, PH, PI, C1, G0, C4, F4, G5, H5, D0, A0, B1, A1, I4, I5,  0,  1,  nl, nl2,  nl1,  nl+1,  2,  3,  nl+2,  nl1+1, nl2+1,nl2+2,nl1+2, nl+3,nl1+3,nl2+3);
            FILTRO(PE, PG, PD, PH, PA, PI, PB, PF, PC, A0, I5, A1, B1, I4, F4, H5, G5, D0, G0, C1, C4, nl2,  nl1, nl2+1, nl2+3, nl2+2,  nl1+1,  nl,  0,  nl+1, nl1+2, nl1+3, nl+3, nl+2, 1, 2, 3);

            sa0 += 1;
            sa1 += 1;
            sa2 += 1;
            sa3 += 1;
            sa4 += 1;

            E += 4;

            if (pprev2){
                pprev2--;
                pprev = 1;
            }
        }
    }
}
#undef FILTRO

}

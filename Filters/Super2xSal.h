#ifndef SUPER2XSAL_H
#define SUPER2XSAL_H

#include <stdint.h>
#include "Filter.h"

#define uint32 uint32_t
#define uint8 uint8_t
#define uint16 uint16_t

static uint32 colorMask = 0xF7DEF7DE;
static uint32 lowPixelMask = 0x08210821;
static uint32 qcolorMask = 0xE79CE79C;
static uint32 qlowpixelMask = 0x18631863;
//static uint32 redblueMask = 0xF81F;
//static uint32 greenMask = 0x7E0;

class Super2xSal : public Filter
{
public:
    Super2xSal( Image* inputImage, int numberOfPasses );
    void apply();

private:

    static inline uint32 INTERPOLATE(uint32 A, uint32 B)
    {
        if (A !=B)
        {
           return ( ((A & colorMask) >> 1) + ((B & colorMask) >> 1) + (A & B & lowPixelMask) );
        }
        else return A;
    }


    static inline uint32 Q_INTERPOLATE(uint32 A, uint32 B, uint32 C, uint32 D)
    {
            register uint32 x = ((A & qcolorMask) >> 2) +
                                ((B & qcolorMask) >> 2) +
                                ((C & qcolorMask) >> 2) +
                                ((D & qcolorMask) >> 2);
            register uint32 y = (A & qlowpixelMask) +
                                (B & qlowpixelMask) +
                                (C & qlowpixelMask) +
                                (D & qlowpixelMask);
            y = (y>>2) & qlowpixelMask;
            return x+y;
    }

    static inline int GetResult(uint32 A, uint32 B, uint32 C, uint32 D)
    {
     int x = 0;
     int y = 0;
     int r = 0;
     if (A == C) x+=1; else if (B == C) y+=1;
     if (A == D) x+=1; else if (B == D) y+=1;
     if (x <= 1) r+=1;
     if (y <= 1) r-=1;
     return r;
    }

    Image* pass( int step, Image* inputImage );

    int _numberOfPasses;
};

#endif // SUPER2XSAL_H

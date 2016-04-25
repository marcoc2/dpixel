#include "Super2xSal.h"

Super2xSal::Super2xSal( Image* inputImage, int numberOfPasses ) :
    Filter( inputImage, 2.0f, numberOfPasses ),
    _numberOfPasses( numberOfPasses )
{
}


void Super2xSal::apply()
{
    Image* passOutput = _inputImage;
    for( int i = 0; i < _numberOfPasses; i++ )
    {
        passOutput = pass( i, passOutput );
    }

    _outputImage = passOutput;
    _outputImage->fillQImageRGB();
}


Image* Super2xSal::pass( int step, Image* inputImage )
{
    Image* outputImage = new Image( inputImage->getWidth() * 2,
                                    inputImage->getHeight() * 2 );

    uint16* bufferRGB565 = new uint16[ inputImage->getWidth() * inputImage->getHeight() ];
    inputImage->convertBufferToRGB565( bufferRGB565 );
    uint8* srcPtr = reinterpret_cast< uint8* >( bufferRGB565 );
    uint32 srcPitch = inputImage->getWidthStep();
    uint8* deltaPtr;
    int width = inputImage->getWidth();
    int height = inputImage->getHeight();

    uint32 line;
    uint32 dP;
    uint16* bP;
    uint32 inc_bP;
    line = ( outputImage->getHeight() - height * 2 ) >> 1;

    //_farsetsel (dstBitmap->seg);
    uint32 Nextline = srcPitch >> 1;

    inc_bP = 1;

    //_farsetsel (dstBitmap->seg);

    for( int j = height - 1; j >= 0; j-- )
    {
        bP = ( uint16* ) srcPtr;
        dP = 0; //bmp_write_line (dstBitmap, line) + x_offset;
        for( uint32 i = width; i; i -= inc_bP )
        {
            uint32 color4, color5, color6;
            uint32 color1, color2, color3;
            uint32 colorA0, colorA1, colorA2, colorA3,
                   colorB0, colorB1, colorB2, colorB3,
                   colorS1, colorS2;
            uint32 product1a, product1b,
                   product2a, product2b;

            //---------------------------------------    B1 B2
            //                                         4  5  6 S2
            //                                         1  2  3 S1
            //                                           A1 A2

            colorB0 = *( bP - Nextline - 1 );
            colorB1 = *( bP - Nextline );
            colorB2 = *( bP - Nextline + 1 );
            colorB3 = *( bP - Nextline + 2 );

            color4 = *( bP - 1 );
            color5 = *( bP );
            color6 = *( bP + 1 );
            colorS2 = *( bP + 2 );

            color1 = *( bP + Nextline - 1 );
            color2 = *( bP + Nextline );
            color3 = *( bP + Nextline + 1 );
            colorS1 = *( bP + Nextline + 2 );

            colorA0 = *( bP + Nextline + Nextline - 1 );
            colorA1 = *( bP + Nextline + Nextline );
            colorA2 = *( bP + Nextline + Nextline + 1 );
            colorA3 = *( bP + Nextline + Nextline + 2 );


//--------------------------------------
            if( color2 == color6 && color5 != color3 )
            {
                product2b = product1b = color2;
            }
            else
            if( color5 == color3 && color2 != color6 )
            {
                product2b = product1b = color5;
            }
            else
            if( color5 == color3 && color2 == color6 )
            {
                register int r = 0;

                r += GetResult( color6, color5, color1, colorA1 );
                r += GetResult( color6, color5, color4, colorB1 );
                r += GetResult( color6, color5, colorA2, colorS1 );
                r += GetResult( color6, color5, colorB2, colorS2 );

                if( r > 0 )
                {
                    product2b = product1b = color6;
                }
                else
                if( r < 0 )
                {
                    product2b = product1b = color5;
                }
                else
                {
                    product2b = product1b = INTERPOLATE( color5, color6 );
                }
            }
            else
            {
                if( color6 == color3 && color3 == colorA1 && color2 != colorA2 && color3 != colorA0 )
                {
                    product2b = Q_INTERPOLATE( color3, color3, color3, color2 );
                }
                else
                if( color5 == color2 && color2 == colorA2 && colorA1 != color3 && color2 != colorA3 )
                {
                    product2b = Q_INTERPOLATE( color2, color2, color2, color3 );
                }
                else
                {
                    product2b = INTERPOLATE( color2, color3 );
                }

                if( color6 == color3 && color6 == colorB1 && color5 != colorB2 && color6 != colorB0 )
                {
                    product1b = Q_INTERPOLATE( color6, color6, color6, color5 );
                }
                else
                if( color5 == color2 && color5 == colorB2 && colorB1 != color6 && color5 != colorB3 )
                {
                    product1b = Q_INTERPOLATE( color6, color5, color5, color5 );
                }
                else
                {
                    product1b = INTERPOLATE( color5, color6 );
                }
            }

            if( color5 == color3 && color2 != color6 && color4 == color5 && color5 != colorA2 )
            {
                product2a = INTERPOLATE( color2, color5 );
            }
            else
            if( color5 == color1 && color6 == color5 && color4 != color2 && color5 != colorA0 )
            {
                product2a = INTERPOLATE( color2, color5 );
            }
            else
            {
                product2a = color2;
            }

            if( color2 == color6 && color5 != color3 && color1 == color2 && color2 != colorB2 )
            {
                product1a = INTERPOLATE( color2, color5 );
            }
            else
            if( color4 == color2 && color3 == color2 && color1 != color5 && color2 != colorB0 )
            {
                product1a = INTERPOLATE( color2, color5 );
            }
            else
            {
                product1a = color5;
            }


            product1a = product1a | ( product1b << 16 );
            product2a = product2a | ( product2b << 16 );

            //_farnspokel (dP, product1a);
            //_farnspokel (dP + dstPitch, product2a);

            //*( dP ) = product1a;
            //*( dP + ( dstRowBytes >> 2 ) ) = product2a;

            outputImage->setPixel( i, j, Pixel( product1a & 0x00FF0000,
                                                product1a & 0x0000FF00,
                                                product1a & 0x000000FF ) );

            outputImage->setPixel( j * width + i + srcPitch * 2,
                                   Pixel( product1a & 0x00FF0000,
                                          product1a & 0x0000FF00,
                                          product1a & 0x000000FF ) );

            bP += inc_bP;
            dP += 4;
        }        //end of for ( finish= width etc..)

        line += 2;
        srcPtr += srcPitch;
        deltaPtr += srcPitch << 1;
    }  //endof: for (height; height; height--)


    if( step > 0 )
    {
        delete inputImage;
    }

    return outputImage;
}



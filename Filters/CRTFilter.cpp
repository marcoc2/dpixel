#include "CRTFilter.h"

#define APPERTURE_SCALE_FACTOR 10

CRTFilter::CRTFilter( Image* inputImage, float scaleFactor ) :
    Filter( inputImage, APPERTURE_SCALE_FACTOR )
{
    _finalScaleFactor = scaleFactor;
}


CRTFilter::CRTFilter( Image* inputImage, int outputWidth, int outputHeight, float scaleFactor ) :
    Filter( inputImage, outputWidth, outputHeight )
{
    _scaleFactor = scaleFactor;
    _finalScaleFactor = scaleFactor;
}


CRTFilter::~CRTFilter()
{
}


void CRTFilter::run()
{
    apply();
}


void CRTFilter::apply()
{
    applyAppertureGrill();
    return;

    #pragma omp parallel for
    for(u_int w = 0; w < _inputImage->getWidth(); w++)
    {
        for(u_int h = 0; h < _inputImage->getHeight(); h++)
        {
            const Pixel& pixel = _inputImage->getPixel( w, h );

            u_int outW = w * _scaleFactor;
            u_int outH = h * _scaleFactor;

            const unsigned char ground = 50;
            Pixel redPixel( pixel.red, ground, ground );
            Pixel greenPixel( ground, pixel.green, ground );
            Pixel bluePixel( ground, ground, pixel.blue );

            for( u_int outH_t = outH; outH_t < outH + _scaleFactor; outH_t++ )
            {
                for( u_int outW_t = outW; outW_t < outW + _scaleFactor / 3; outW_t++ )
                {
                    _outputImage->setPixel( outW_t, outH_t, redPixel );
                }

                for( u_int outW_t = outW + _scaleFactor / 3; outW_t < outW + ( 2 *_scaleFactor ) / 3; outW_t++ )
                {
                    _outputImage->setPixel( outW_t, outH_t, greenPixel );
                }

                for( u_int outW_t = outW + ( 2 *_scaleFactor ) / 3; outW_t < outW + _scaleFactor; outW_t++ )
                {
                    _outputImage->setPixel( outW_t, outH_t, bluePixel );
                }
            }
        }
    }

    _outputImage->fillQImageRGB();
}


void CRTFilter::applyAppertureGrill()
{
    for(u_int w = 0; w < _inputImage->getWidth(); w++)
    {
        #pragma omp parallel for
        for(u_int h = 0; h < _inputImage->getHeight(); h++)
        {
            const Pixel& pixel = _inputImage->getPixel( w, h );

            u_int w_ = w * APPERTURE_SCALE_FACTOR;
            u_int h_ = h * APPERTURE_SCALE_FACTOR;

            const unsigned char ground = 0;
            Pixel redPixel( pixel.red, ground, ground );
            Pixel redBorder( pixel.red / 2, ground, ground );
            Pixel midRedGreen( pixel.red / 2, pixel.green / 2, ground );

            Pixel greenPixel( ground, pixel.green, ground );
            Pixel greenBorder( ground, pixel.green / 2, ground );
            Pixel midGreenBlue( ground, pixel.green / 2, pixel.blue / 2 );

            Pixel bluePixel( ground, ground, pixel.blue );
            Pixel blueBorder( ground, ground, pixel.blue / 2 );

            for( u_int j = h_; j < h_ + APPERTURE_SCALE_FACTOR; j++ )
            {
                for( u_int i = w_ + 0; i < w_ + 3; i++ )
                {
                    if( ( j == h_ ) || ( j == ( h_ + 9 ) ) )
                    {
                        _outputImage->setPixel( i, j, redBorder );
                    }
                    else
                    {
                        _outputImage->setPixel( i, j, redPixel );
                    }
                }

                for( u_int i = w_ + 3; i < w_ + 7; i++ )
                {
                    if( ( j == h_ ) || ( j == ( h_ + 9 ) ) )
                    {
                        _outputImage->setPixel( i, j, greenBorder );
                    }
                    else
                    {
                        _outputImage->setPixel( i, j, greenPixel );
                    }
                }

                for( u_int i = w_ + 7; i < w_ + 10; i++ )
                {
                    if( ( j == h_ ) || ( j == ( h_ + 9 ) ) )
                    {
                        _outputImage->setPixel( i, j, blueBorder );
                    }
                    else
                    {
                        _outputImage->setPixel( i, j, bluePixel );
                    }
                }

                for( u_int i = w_; i < w_ + 10; i++ )
                {
                    if( ( j == h_ || j == h_ + 9 ) &&
                            ( i == w_ || i == w_ + 3 || i == w_ + 6 || i == w_ + 9 ) )
                    {
                        _outputImage->setPixel( i, j, Pixel( 0, 0, 0 ) );
                    }
                    else if( i == w_ )
                    {
                        _outputImage->setPixel( i, j, redBorder );
                    }
                    else if( i == w_ + 3 )
                    {
                        _outputImage->setPixel( i, j, midRedGreen );
                    }
                    else if( i == w_ + 6 )
                    {
                        _outputImage->setPixel( i, j, midGreenBlue );
                    }
                    else if( i == w_ + 9 )
                    {
                        _outputImage->setPixel( i, j, blueBorder );
                    }
                }
            }
        }
        emit setProgress( ( float ) w * 100 / ( float ) _inputImage->getWidth() );
    }

    _outputImage->fillQImageRGB();


    // Resize smoothly as second pass
    _outputImage->resize( _finalScaleFactor / ( float ) APPERTURE_SCALE_FACTOR, true );
}


void CRTFilter::applyShadowMask()
{
    const int maskWidth = _outputImage->getWidth();
    const int maskHeight = _outputImage->getHeight();

    for( int i = 0; i < maskWidth * _scaleFactor; i = i + 3 )
    {
        for( int j = 0; j < maskHeight * _scaleFactor; j++ )
        {
            Pixel pixel;
            _inputImage->getInterpolatedPixel( i / ( maskWidth * _scaleFactor ),
                                               j / ( maskHeight *_scaleFactor ), pixel );

            _outputImage->setPixel( i, j, pixel );
            _outputImage->setPixel( i + 1, j, pixel );
            _outputImage->setPixel( i + 2, j, pixel );
            //_outputImage->setPixel( i, j, Pixel( pixel.red, 0, 0 ) );
            //_outputImage->setPixel( i + 1, j, Pixel( 0, pixel.green, 0 ) );
            //_outputImage->setPixel( i + 2, j, Pixel( 0, 0, pixel.blue ) );
        }
    }

    _outputImage->fillQImageRGB();
}


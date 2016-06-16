#include "BeadsFilter.h"

#include <cmath>

#define BEAD_SIZE 10
#define BEAD_RADIUS BEAD_SIZE / 2

BeadsFilter::BeadsFilter( Image* inputImage, float scaleFactor ) :
    Filter( inputImage, scaleFactor )
{
    _name = std::string( "beads" );
    _intermediateImage = new Image( inputImage->getWidth() * BEAD_SIZE,
                                    inputImage->getHeight() * BEAD_SIZE );

}

BeadsFilter::~BeadsFilter()
{
}


void BeadsFilter::run()
{
    apply();
}


void BeadsFilter::apply()
{
    for( u_int w = 0; w < _inputImage->getWidth(); w++ )
    {
        #pragma omp parallel for
        for( u_int h = 0; h < _inputImage->getHeight(); h++ )
        {
            Pixel inputPixel = _inputImage->getPixel( w, h );
            for( int i = 0; i < BEAD_SIZE; i++ )
            {
                for( int j = 0; j < BEAD_SIZE; j++ )
                {
                    double d = std::pow( i - BEAD_RADIUS, 2 ) +
                               std::pow( j - BEAD_RADIUS, 2 );
                    if( d > 4.0f && d < 25.0f  )
                    {
                        _intermediateImage->setPixel( ( w * BEAD_SIZE ) + i,
                                                      ( h * BEAD_SIZE ) + j, inputPixel );
                    }
                    else
                    {
                        _intermediateImage->setPixel( ( w * BEAD_SIZE ) + i,
                                                      ( h * BEAD_SIZE ) + j, Pixel( 0, 0, 0 ) );
                    }
                }
            }
        }
        emit setProgress( ( float ) w * 100 / ( float ) _inputImage->getWidth() );
    }

    // Resize smoothly as second pass
    _intermediateImage->fillQImageRGB();

    QImage* intermediateQImage = _intermediateImage->getQImage();
    QImage* scaledImage = new QImage ( intermediateQImage->scaled(
                                           _scaleFactor / ( float ) BEAD_SIZE * intermediateQImage->size(),
                                       Qt::IgnoreAspectRatio,
                                       Qt::SmoothTransformation ) );

    delete _outputImage;
    _outputImage = new Image( scaledImage );
}

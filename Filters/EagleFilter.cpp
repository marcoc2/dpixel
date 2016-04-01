#include "EagleFilter.h"

EagleFilter::EagleFilter( Image* inputImage, float scaleFactor ) :
    Filter( inputImage, scaleFactor )
{
}


EagleFilter::~EagleFilter()
{
}


void EagleFilter::apply()
{
    #pragma omp parallel for
    for( u_int w = 0; w < _inputImage->getWidth(); w++ )
    {
        for( u_int h = 0; h < _inputImage->getHeight(); h++ )
        {
            const Pixel& A = _inputImage->getPixel( w - 1, h - 1 );
            const Pixel& B = _inputImage->getPixel( w, h - 1 );
            const Pixel& C = _inputImage->getPixel( w + 1, h - 1 );
            const Pixel& D = _inputImage->getPixel( w - 1, h );
            const Pixel& E = _inputImage->getPixel( w, h );
            const Pixel& F = _inputImage->getPixel( w + 1, h );
            const Pixel& G = _inputImage->getPixel( w - 1, h + 1 );
            const Pixel& H = _inputImage->getPixel( w, h + 1 );
            const Pixel& I = _inputImage->getPixel( w + 1, h + 1 );

            Pixel E0;
            Pixel E1;
            Pixel E2;
            Pixel E3;

            E0 = ( D == A ) && ( A == B ) ? A : E;
            E1 = ( B == C ) && ( C == F ) ? F : E;
            E2 = ( D == G ) && ( G == H ) ? G : E;
            E3 = ( H == I ) && ( I == F ) ? I : E;

            u_char w_index = ( w ) * 2;
            u_char h_index = ( h ) * 2;
            _outputImage->setPixel( w_index, h_index, E0 );
            _outputImage->setPixel( w_index + 1, h_index, E1 );
            _outputImage->setPixel( w_index, h_index + 1, E2 );
            _outputImage->setPixel( w_index + 1, h_index + 1, E3 );
        }
    }

    _outputImage->fillQImageRGB();
}



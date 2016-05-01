#include "EagleFilter.h"

EagleFilter::EagleFilter( Image* inputImage, float scaleFactor ) :
    Filter( inputImage, scaleFactor )
{
}


EagleFilter::EagleFilter( Image* inputImage, int numberOfPasses ) :
    Filter( inputImage, 2.0f,  numberOfPasses ),
    _numberOfPasses( numberOfPasses )
{

}


EagleFilter::~EagleFilter()
{
}


void EagleFilter::run()
{
    apply();
}


void EagleFilter::apply()
{
    Image* passOutput = _inputImage;
    for( int i = 0; i < _numberOfPasses; i++ )
    {
        passOutput = pass( i, passOutput );
    }

    _outputImage = passOutput;
    _outputImage->fillQImageRGB();
}


Image* EagleFilter::pass( int step, Image* inputImage )
{
    Image* outputImage = new Image( inputImage->getWidth() * 2,
                                    inputImage->getHeight() * 2 );

    #pragma omp parallel for
    for( u_int w = 0; w < inputImage->getWidth(); w++ )
    {
        for( u_int h = 0; h < inputImage->getHeight(); h++ )
        {
            const Pixel& A = inputImage->getPixel( w - 1, h - 1 );
            const Pixel& B = inputImage->getPixel( w, h - 1 );
            const Pixel& C = inputImage->getPixel( w + 1, h - 1 );
            const Pixel& D = inputImage->getPixel( w - 1, h );
            const Pixel& E = inputImage->getPixel( w, h );
            const Pixel& F = inputImage->getPixel( w + 1, h );
            const Pixel& G = inputImage->getPixel( w - 1, h + 1 );
            const Pixel& H = inputImage->getPixel( w, h + 1 );
            const Pixel& I = inputImage->getPixel( w + 1, h + 1 );

            Pixel E0;
            Pixel E1;
            Pixel E2;
            Pixel E3;

            E0 = ( D == A ) && ( A == B ) ? A : E;
            E1 = ( B == C ) && ( C == F ) ? F : E;
            E2 = ( D == G ) && ( G == H ) ? G : E;
            E3 = ( H == I ) && ( I == F ) ? I : E;

            int w_index = ( w ) * 2;
            int h_index = ( h ) * 2;
            outputImage->setPixel( w_index, h_index, E0 );
            outputImage->setPixel( w_index + 1, h_index, E1 );
            outputImage->setPixel( w_index, h_index + 1, E2 );
            outputImage->setPixel( w_index + 1, h_index + 1, E3 );
        }
    }

    if( step > 0 )
    {
        delete inputImage;
    }

    return outputImage;
}



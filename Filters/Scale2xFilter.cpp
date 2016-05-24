#include "Scale2xFilter.h"

Scale2xFilter::Scale2xFilter( Image* inputImage, Image* outputImage ) :
    Filter( inputImage, outputImage )
{
    _name = std::string( "scale2x" );
}


Scale2xFilter::Scale2xFilter( Image* inputImage, int numberOfPasses ) :
    Filter( inputImage, 2.0f,  numberOfPasses ),
    _numberOfPasses( numberOfPasses )
{
    _name = std::string( "scale2x" );
}


Scale2xFilter::~Scale2xFilter()
{
}


void Scale2xFilter::run()
{
    apply();
}


void Scale2xFilter::apply()
{
    Image* passOutput = _inputImage;
    for( int i = 0; i < _numberOfPasses; i++ )
    {
        passOutput = pass( i, passOutput );
    }

    _outputImage = passOutput;
    _outputImage->fillQImageRGB();
}


Image* Scale2xFilter::pass( int step, Image* inputImage )
{
    Image* outputImage = new Image( inputImage->getWidth() * 2,
                                    inputImage->getHeight() * 2 );

    for(u_int w = 0; w < inputImage->getWidth(); w++)
    {
        #pragma omp parallel for
        for(u_int h = 0; h < inputImage->getHeight(); h++)
        {
            const Pixel& B = inputImage->getPixel( w, h - 1 );

            const Pixel& D = inputImage->getPixel( w - 1, h );
            const Pixel& E = inputImage->getPixel( w, h );
            const Pixel& F = inputImage->getPixel( w + 1, h );

            const Pixel& H = inputImage->getPixel( w, h + 1 );

            Pixel E0;
            Pixel E1;
            Pixel E2;
            Pixel E3;

            if( (B != H) && (D != F) )
            {
                E0 = (D == B) ? D : E;
                E1 = (B == F) ? F : E;
                E2 = (D == H) ? D : E;
                E3 = (H == F) ? F : E;
            }
            else
            {
                E0 = E;
                E1 = E;
                E2 = E;
                E3 = E;
            }

            int w_index = (w) * 2;
            int h_index = (h) * 2;
            outputImage->setPixel( w_index, h_index, E0 );
            outputImage->setPixel( w_index + 1, h_index, E1 );
            outputImage->setPixel( w_index, h_index + 1, E2 );
            outputImage->setPixel( w_index + 1, h_index + 1, E3 );
        }

        emit setProgress( ( ( ( float ) w / ( float ) inputImage->getWidth() ) / ( float )_numberOfPasses +
                            ( float )step / ( float )_numberOfPasses ) * 100 );
        }

    if( step > 0 )
    {
        delete inputImage;
    }

    return outputImage;
}

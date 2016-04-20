#include "Scale2xFilter.h"

Scale2xFilter::Scale2xFilter( Image* inputImage, Image* outputImage ) :
    Filter( inputImage, outputImage )
{
}



Scale2xFilter::~Scale2xFilter()
{
}



void Scale2xFilter::apply()
{
    for(u_int w = 0; w < _inputImage->getWidth(); w++)
    {
        for(u_int h = 0; h < _inputImage->getHeight(); h++)
        {
            const Pixel& B = _inputImage->getPixel( w, h - 1 );

            const Pixel& D = _inputImage->getPixel( w - 1, h );
            const Pixel& E = _inputImage->getPixel( w, h );
            const Pixel& F = _inputImage->getPixel( w + 1, h );

            const Pixel& H = _inputImage->getPixel( w, h + 1 );

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
            _outputImage->setPixel( w_index, h_index, E0 );
            _outputImage->setPixel( w_index + 1, h_index, E1 );
            _outputImage->setPixel( w_index, h_index + 1, E2 );
            _outputImage->setPixel( w_index + 1, h_index + 1, E3 );
        }
    }

    _outputImage->fillQImageRGB();
}



void Scale2xFilter::apply2x( Image* output4xImage )
{
    apply();

    for(u_int w = 0; w < _outputImage->getWidth(); w++)
    {
        for(u_int h = 0; h < _outputImage->getHeight(); h++)
        {
            const Pixel& B = _outputImage->getPixel( w, h - 1 );

            const Pixel& D = _outputImage->getPixel( w - 1, h );
            const Pixel& E = _outputImage->getPixel( w, h );
            const Pixel& F = _outputImage->getPixel( w + 1, h );

            const Pixel& H = _outputImage->getPixel( w, h + 1 );

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
            output4xImage->setPixel( w_index, h_index, E0 );
            output4xImage->setPixel( w_index + 1, h_index, E1 );
            output4xImage->setPixel( w_index, h_index + 1, E2 );
            output4xImage->setPixel( w_index + 1, h_index + 1, E3 );
        }
    }

    output4xImage->fillQImageRGB();
}

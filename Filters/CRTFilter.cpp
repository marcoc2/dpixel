#include "CRTFilter.h"

CRTFilter::CRTFilter( Image* inputImage, Image* outputImage ) :
    Filter( inputImage, outputImage )
{

}



void CRTFilter::apply()
{
    for(u_int w = 0; w < _inputImage->getWidth(); w++)
    {
        for(u_int h = 0; h < _inputImage->getHeight(); h++)
        {
        }
    }
}

#include "XbrFilter.h"
#include "xbr.h"

XbrFilter::XbrFilter( Image* inputImage, Image* outputImage ) :
    Filter( inputImage, outputImage )
{
}



XbrFilter::~XbrFilter()
{
}



void XbrFilter::apply()
{
    u_char* inputBuffer = new u_char[ _inputImage->getSize() * 4 ];
    u_char* outputBuffer = new u_char[ _outputImage->getSize() * 4 ];

    fillBufferRGB( inputBuffer );

    xbr::xbr4x( inputBuffer, outputBuffer, _inputImage->getWidth(), _inputImage->getHeight() );

    fillImageRGB( outputBuffer );
}

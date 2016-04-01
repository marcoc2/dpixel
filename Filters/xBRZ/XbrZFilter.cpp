#include "XbrZFilter.h"
#include "xbrz.h"
#include "config.h"

XbrZFilter::XbrZFilter( Image* inputImage, float scaleFactor ) :
    Filter( inputImage, scaleFactor )
{
}



XbrZFilter::~XbrZFilter()
{
}



void XbrZFilter::apply()
{
    u_char* inputBuffer = new u_char[ _inputImage->getSize() * 4 ];
    u_char* outputBuffer = new u_char[ _outputImage->getSize() * 4 ];

    fillBufferBGRA( inputBuffer );

    //xbrz::xbr4x( inputBuffer, outputBuffer, _inputImage->getWidth(), _inputImage->getHeight() );
    xbrz::ScalerCfg scalerCfg;

    xbrz::scale( static_cast<size_t>( 4 ),
                 reinterpret_cast<u_int32_t*>( inputBuffer ),
                 reinterpret_cast<u_int32_t*>( outputBuffer ),
                 _inputImage->getWidth(),
                 _inputImage->getHeight(),
                 scalerCfg );

    fillImageBGRA( outputBuffer );
}


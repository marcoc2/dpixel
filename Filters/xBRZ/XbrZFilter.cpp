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


void XbrZFilter::run()
{
    apply();
}


void XbrZFilter::apply()
{
    u_char* inputBuffer = new u_char[ _inputImage->getSize() * ( int ) _scaleFactor ];
    u_char* outputBuffer = new u_char[ _outputImage->getSize() * ( int ) _scaleFactor ];

    fillBufferBGRA( inputBuffer );

    //xbrz::xbr4x( inputBuffer, outputBuffer, _inputImage->getWidth(), _inputImage->getHeight() );
    xbrz::ScalerCfg scalerCfg;

    xbrz::scale( static_cast< size_t >( _scaleFactor ),
                 reinterpret_cast< u_int* >( inputBuffer ),
                 reinterpret_cast< u_int* >( outputBuffer ),
                 _inputImage->getWidth(),
                 _inputImage->getHeight(),
                 scalerCfg );

    fillImageBGRA( outputBuffer );
}



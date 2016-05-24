#include "XbrFilter.h"
//#include "xbr.h"
#include "filters.h"

XbrFilter::XbrFilter( Image* inputImage, float scaleFactor ) :
    Filter( inputImage, scaleFactor )
{
}



XbrFilter::~XbrFilter()
{
}


void XbrFilter::run()
{
    apply();
}


void XbrFilter::apply()
{
    xbr_data *xbrData;
    xbr_params xbrParams;

    u_char* inputBuffer = new u_char[ _inputImage->getSize() * 4 ];
    u_char* outputBuffer = new u_char[ _outputImage->getSize() * 4 ];

    xbrData = static_cast< xbr_data* >( malloc(sizeof(xbr_data)) );
    xbr_init_data(xbrData);

    xbrParams.data = xbrData;
    xbrParams.input = inputBuffer;
    xbrParams.output = outputBuffer;
    xbrParams.inWidth = _inputImage->getWidth();
    xbrParams.inHeight = _inputImage->getHeight();
    xbrParams.inPitch = _inputImage->getWidth() * 4;
    xbrParams.outPitch = _inputImage->getWidth() * _scaleFactor * 4;

    fillBufferBGRA( inputBuffer );

    switch ( ( int )_scaleFactor )
    {
        case 2: xbr_filter_xbr2x(&xbrParams); break;
        case 3: xbr_filter_xbr3x(&xbrParams); break;
        case 4: xbr_filter_xbr4x(&xbrParams); break;
    }

    //xbr::xbr4x( inputBuffer, outputBuffer, _inputImage->getWidth(), _inputImage->getHeight() );

    fillImageBGRA( outputBuffer );

    free( xbrData );
    delete[] inputBuffer;
    delete[] outputBuffer;
}

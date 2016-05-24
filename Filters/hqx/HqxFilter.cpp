#include "HqxFilter.h"
#include "hq4x.h"

HqxFilter::HqxFilter(Image* inputImage, float scaleFactor ) :
    Filter( inputImage, scaleFactor )
{
}



HqxFilter::~HqxFilter()
{
}


void HqxFilter::run()
{
    apply();
}

void HqxFilter::apply()
{
    u_char* inputBuffer = new u_char[ _inputImage->getSize() * sizeof( u_int32_t ) ];
    u_char* outputBuffer = new u_char[ _outputImage->getSize() * sizeof( u_int32_t ) ];

    fillBufferBGRA( inputBuffer );

    hqxInit();
    hq4x_32( reinterpret_cast<u_int32_t*>( inputBuffer ),
             reinterpret_cast<u_int32_t*>( outputBuffer ),
             _inputImage->getWidth(),
             _inputImage->getHeight() );

    fillImageBGRA( outputBuffer );

    delete[] inputBuffer;
    delete[] outputBuffer;
}

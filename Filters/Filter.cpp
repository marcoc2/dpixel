#include "Filter.h"
#include <omp.h>

Filter::Filter( Image* inputImage, Image* outputImage ) :
    _inputImage( inputImage ),
    _outputImage( outputImage )
{
}


Filter::Filter( Image* inputImage, float scaleFactor ) :
    _inputImage( inputImage ),
    _scaleFactor( scaleFactor )
{
    _outputImage = new Image( _inputImage->getWidth() * scaleFactor,
                              _inputImage->getHeight() * scaleFactor );
}


Filter::Filter( Image* inputImage, float scaleFactor, int numberOfPasses ) :
    _inputImage( inputImage ),
    _scaleFactor( scaleFactor * numberOfPasses )
{
    // Multipass filter need to create the _outputImage
}

Filter::Filter( Image* inputImage, int outputWidth, int outputHeight ) :
    _inputImage( inputImage )
{
    _outputImage = new Image( outputWidth,
                              outputHeight );

    _scaleFactor = ( float ) _outputImage->getWidth() / ( float) _inputImage->getWidth();
}


Filter::Filter( Image* inputImage ) :
    _inputImage( inputImage ),
    _outputImage( 0 )
{
}


Filter::~Filter()
{
    delete _outputImage;
}


Image* Filter::getOutputImage()
{
    return _outputImage;
}


void Filter::fillBufferBGRA( u_char* inputBuffer )
{
    u_int width = _inputImage->getWidth();
    u_int height = _inputImage->getHeight();

    for( u_int w = 0; w < width; w++ )
    {
        for( u_int h = 0; h < height; h++ )
        {
            QRgb pixel = _inputImage->getQImage()->pixel( w, h );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 2 ] = ( u_char ) qRed( pixel );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 1 ] = ( u_char ) qGreen( pixel );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 0 ] = ( u_char ) qBlue( pixel );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 3 ] = ( u_char ) qAlpha( pixel );
        }
    }
}


void Filter::fillImageBGRA( u_char* outputBuffer )
{
    u_int width = _outputImage->getWidth();
    u_int height = _outputImage->getHeight();

    for( u_int w = 0; w < width; w++ )
    {
        for( u_int h = 0; h < height; h++ )
        {
            QRgb pixelRGB = qRgb(
                ( int ) outputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 2 ],
                ( int ) outputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 1 ],
                ( int ) outputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 0 ] );

            _outputImage->getQImage()->setPixel( w, h, pixelRGB );
        }
    }
}


void Filter::fillBufferRGB( u_char* inputBuffer )
{
    u_int width = _inputImage->getWidth();
    u_int height = _inputImage->getHeight();

    #pragma omp parallel for
    for( u_int w = 0; w < width; w++ )
    {
        for( u_int h = 0; h < height; h++ )
        {
            QRgb pixel = _inputImage->getQImage()->pixel( w, h );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 0 ] = ( u_char ) qRed( pixel );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 1 ] = ( u_char ) qGreen( pixel );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 2 ] = ( u_char ) qBlue( pixel );
            inputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 3 ] = ( u_char ) qAlpha( pixel );
        }
    }
}


void Filter::fillImageRGB( u_char* outputBuffer )
{
    u_int width = _outputImage->getWidth();
    u_int height = _outputImage->getHeight();

    #pragma omp for
    for( u_int w = 0; w < width; w++ )
    {
        for( u_int h = 0; h < height; h++ )
        {
            QRgb pixelRGB = qRgb(
                ( int ) outputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 0 ],
                ( int ) outputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 1 ],
                ( int ) outputBuffer[ ( h * width * 4 ) + ( w * 4 ) + 2 ] );

            _outputImage->getQImage()->setPixel( w, h, pixelRGB );
        }
    }
}


int Filter::getScaleFactor()
{
    return _scaleFactor;
}


void Filter::setNewInputImage( Image* image )
{
    _inputImage = image;
}

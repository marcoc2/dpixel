#include <stdio.h>
#include "Image.h"

Image::Image() :
    _buffer( 0 ),
    _qImage( 0 ),
    _size( 0 ),
    _width( 0 ),
    _height( 0 ),
    _widthStep( 0 ),
    _nChannels( 0 )
{
}


Image::Image( u_int width, u_int height ) :
    _size( width * height ),
    _width( width ),
    _height( height ),
    _widthStep( width )
{
    _qImage = new QImage( width, height, QImage::Format_RGB888 ),
    _nChannels = _qImage->depth() / sizeof( uchar );
    _buffer = new Pixel[ _size ];
}


Image::Image( QImage* qImage ) :
    _qImage( qImage ),
    _size( qImage->size().width() * qImage->size().height() ),
    _width( qImage->width() ),
    _height( qImage->height() ),
    _widthStep( qImage->bytesPerLine() / ( qImage->depth() / 8 ) ),
    _nChannels( qImage->depth() / 8 )
{
    //_buffer = reinterpret_cast<Pixel*>( _qImage->bits() );
    _buffer = new Pixel[ _size ];
    fillBufferRGB();
}


Image::Image( const char* fileName )
{
    _qImage = new QImage( fileName );
    if( _qImage->isNull() )
    {
        return;
    }
    _size = _qImage->size().width() * _qImage->size().height();
    _width = _qImage->width();
    _height = _qImage->height();
    _widthStep = _qImage->bytesPerLine() / ( _qImage->depth() / 8 );
    _nChannels = _qImage->depth() / 8;
    //_buffer = reinterpret_cast<Pixel*>( _qImage->bits() );

    _buffer = new Pixel[ _size ];
    fillBufferRGB();
}


Image::~Image()
{
    delete[] _buffer;
    delete _qImage;
}


inline u_char Image::pixel( u_int x, u_int y )
{
    return ( y * _widthStep ) + x;
}


bool Image::save()
{
    return true;
}


bool Image::load()
{
    return true;
}


void Image::setPixel( u_int x, u_int y, Pixel pixel )
{
    int index = ( y * _widthStep ) + x;

    if( index < 0 || index > static_cast< int >( _size ) )
    {
        return;
    }

    _buffer[ ( y * _widthStep ) + ( x ) ] = pixel;
}


void Image::setPixel( u_int index, Pixel pixel )
{
    if( index < 0 || index > _size )
    {
        return;
    }

    _buffer[ index ] = pixel;
}


Pixel& Image::getPixel( u_int x, u_int y )
{
    int index = ( y * _widthStep ) + x;

    if( index < 0 || index > static_cast< int >( _size ) )
    {
        return _buffer[ 0 ];
    }

    return ( _buffer[ index ] );
}


void Image::createTest()
{
    if( _width == 0 )
    {
        return;
    }

    _buffer = new Pixel[ _width * _height ];

    //for(u_int w = 0; w < _width; w++)
    //{
    //    for(u_int h = 0; h < _height; h++)
    //    {
    //        u_char pixelRGB = ( u_char ) 255 * ( (float) w / (float) _width);
    //        _buffer[ pixel( w, h ) ].red = pixelRGB;
    //        _buffer[ pixel( w, h ) ].green = pixelRGB;
    //        _buffer[ pixel( w, h ) ].blue = pixelRGB;
    //    }
    //}

    for( u_int w = 0; w < _width; w++ )
    {
        for( u_int h = 0; h < _height; h++ )
        {
            int channelValue = 255 * ( ( float ) w / ( float ) _width );
            QRgb pixelRGB = qRgb( channelValue, channelValue, channelValue );
            _qImage->setPixel( w, h, pixelRGB );
        }
    }

    //_qImage = new QImage( reinterpret_cast<u_char*>( _buffer ), _width, _height, _widthStep * _nChannels, QImage::Format_RGB888);
}


u_int Image::getSize()
{
    return _size;
}


u_int Image::getWidth()
{
    return _width;
}


u_int Image::getHeight()
{
    return _height;
}


u_int Image::getWidthStep()
{
    return _widthStep;
}


QImage* Image::getQImage()
{
    return _qImage;
}


QImage* Image::getQImageFromBuffer()
{
    //_qImage = new QImage( reinterpret_cast<u_char*>( _buffer ), _width, _height, _widthStep * _nChannels, QImage::Format_RGB888);

    #pragma omp for
    for( u_int w = 0; w < _width; w++ )
    {
        for( u_int h = 0; h < _height; h++ )
        {
//            u_char pixelRGB = ( u_char ) 255 * ( (float) w / (float) _width);
//            _buffer[ pixel( w, h ) ].red = pixelRGB;
//            _buffer[ pixel( w, h ) ].green = pixelRGB;
//            _buffer[ pixel( w, h ) ].blue = pixelRGB;

            _qImage->setPixel( w, h,
                               qRgb( _buffer[ pixel( w,
                                                     h ) ].red,
                                     _buffer[ pixel( w, h ) ].green, _buffer[ pixel( w, h ) ].blue ) );
        }
    }

    return _qImage;
}


Pixel* Image::getBuffer()
{
    return _buffer;
}


void Image::getBufferRGBA8( uint8_t* buffer)
{
    for( size_t i = 0; i < ( _width * _height ); i++ )
    {
        size_t j = i * 4;
        buffer[ j ] = _buffer[ i ].red;
        buffer[ j + 1 ] = _buffer[ i ].green;
        buffer[ j + 2 ] = _buffer[ i ].blue;
        buffer[ j + 3 ] = 255;
    }
}


void Image::fillBufferRGB()
{
    //#pragma omp for
    for( u_int w = 0; w < _width; w++ )
    {
        for( u_int h = 0; h < _height; h++ )
        {
            unsigned int index = ( h * _widthStep ) + ( w );
            QRgb pixel = _qImage->pixel( w, h );
            _buffer[ index ].red = ( u_char ) qRed( pixel );
            _buffer[ index ].green = ( u_char ) qGreen( pixel );
            _buffer[ index ].blue = ( u_char ) qBlue( pixel );
        }
    }
}


void Image::fillQImageRGB()
{
    //#pragma omp for
    for( u_int w = 0; w < _width; w++ )
    {
        for( u_int h = 0; h < _height; h++ )
        {
            unsigned int index = ( h * _widthStep ) + ( w );
            QRgb pixelRGB = qRgb(
                _buffer[ index ].red,
                _buffer[ index ].green,
                _buffer[ index ].blue );

            _qImage->setPixel( w, h, pixelRGB );
        }
    }
}


void Image::convertBufferToRGB565( uint16_t* outputBuffer )
{
    for( u_int w = 0; w < _width; w++ )
    {
        for( u_int h = 0; h < _height; h++ )
        {
            unsigned int index = ( h * _widthStep ) + ( w );
            int red = _buffer[ index ].red & 0xFF;
            int green = _buffer[ index ].green & 0xFF;
            int blue =  _buffer[ index ].blue & 0xFF;

            uint16_t  B = (blue >> 3) & 0x001F;
            uint16_t  G = ((green >> 2) < 5) & 0x07E0;
            uint16_t  R = ((red >> 3) < 11) & 0xF800;

            outputBuffer[ index ] = ( R | G | B );
        }
    }
}


void Image::getInterpolatedPixel( float i, float j, Pixel& interpolatedPixel )
{
    float w = i * _width;
    float h = j * _height;
    int intW = ( int ) w;
    int intH = ( int ) h;

    Pixel UL;
    Pixel UR;
    Pixel DL;
    Pixel DR;

    bool isLeft = ( w - ( float ) intW ) * 10 < 5;
    bool isUp = ( h - ( float ) intH ) * 10 < 5;

    if( isLeft && isUp )
    {
        UL = getPixel( intW, intH );
        UR = getPixel( intW + 1, intH );
        DL = getPixel( intW, intH + 1 );
        DR = getPixel( intW + 1, intH + 1 );
    }
    else if( isLeft && !isUp )
    {
        UL = getPixel( intW, intH - 1 );
        UR = getPixel( intW + 1, intH - 1 );
        DL = getPixel( intW, intH );
        DR = getPixel( intW + 1, intH );
    }
    else if( !isLeft && isUp )
    {
        UL = getPixel( intW - 1, intH );
        UR = getPixel( intW, intH );
        DL = getPixel( intW - 1, intH + 1 );
        DR = getPixel( intW, intH + 1 );
    }
    else if( !isLeft && !isUp )
    {
        UL = getPixel( intW - 1, intH - 1 );
        UR = getPixel( intW, intH - 1 );
        DL = getPixel( intW - 1, intH );
        DR = getPixel( intW, intH );
    }

    Pixel up;
    Pixel down;
    interpolatePixels( UL, UR, ( w - ( float ) intW ), up );
    interpolatePixels( DL, DR, ( w - ( float ) intW ), down );

    interpolatePixels( up, down, ( h - ( float ) intH ), interpolatedPixel );
}


void Image::interpolatePixels( const Pixel& a, const Pixel& b, float fraction,
                               Pixel& interpolatedPixel )
{
    interpolatedPixel.red = ( a.red * fraction ) + ( b.red * ( 1 - fraction ) );
    interpolatedPixel.green = ( a.green * fraction ) + ( b.green * ( 1 - fraction ) );
    interpolatedPixel.blue = ( a.blue * fraction ) + ( b.blue * ( 1 - fraction ) );
}

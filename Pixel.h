/*
 * 2016 - Marco Aurélio G. da Silva ( marcoc2@gmail.com )
 */

#ifndef PIXEL_H
#define PIXEL_H

class Pixel
{
public:
    Pixel();
    Pixel( unsigned int r, unsigned int g, unsigned int b );
    Pixel( const Pixel& pixel );
    ~Pixel();

    bool operator==( const Pixel& p ) const;
    bool operator!=( const Pixel& p ) const;
    Pixel operator*( double m ) const;
    Pixel& operator+=( const Pixel& p );

    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

#endif // PIXEL_H

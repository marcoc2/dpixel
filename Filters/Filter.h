#ifndef FILTER_H
#define FILTER_H

#include "Image.h"

class Filter
{
public:
    Filter( Image* inputImage, Image* outputImage );
    Filter( Image* inputImage, float scaleFactor );
    Filter( Image* inputImage );
    ~Filter();

    virtual void apply() = 0;

    Image* getOutputImage();

protected:

    void fillBufferBGRA( u_char* inputBuffer );
    void fillImageBGRA( u_char* outputBuffer );

    void fillBufferRGB( u_char* inputBuffer );
    void fillImageRGB( u_char* outputBuffer );

    Image* _inputImage;
    Image* _outputImage;

    float _scaleFactor;
};

#endif // FILTER_H

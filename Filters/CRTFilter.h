#ifndef CRTFILTER_H
#define CRTFILTER_H

#include "Filters/Filter.h"

class CRTFilter : public Filter
{
public:
    CRTFilter( Image* inputImage, float scaleFactor );
    CRTFilter( Image* inputImage, int outputWidth, int outputHeight, float scaleFactor );
    ~CRTFilter();

    void apply();
    void run();

    void applyAppertureGrill();
    void applyShadowMask();

private:

    void bloom( int w, int h, double luminance );
    double gaussian( double x, double mu, double sigma );
    double getPixelLuminance( const Pixel& pixel );

    Image* _intermediateImage;
    Image* _convolutedImage;
};

#endif // CRTFILTER_H

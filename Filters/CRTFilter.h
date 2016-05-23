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

    float _finalScaleFactor;
};

#endif // CRTFILTER_H

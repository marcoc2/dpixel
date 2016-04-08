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
    void applyAppertureGrill();
    void applyShadowMask();
};

#endif // CRTFILTER_H

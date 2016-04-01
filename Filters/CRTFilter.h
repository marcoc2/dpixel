#ifndef CRTFILTER_H
#define CRTFILTER_H

#include "Filters/Filter.h"

class CRTFilter : public Filter
{
public:
    CRTFilter( Image* inputImage, Image* outputImage );
    ~CRTFilter();
    void apply();
};

#endif // CRTFILTER_H

#ifndef XBRFILTER_H
#define XBRFILTER_H

#include "Filters/Filter.h"

class XbrFilter : public Filter
{
public:
    XbrFilter( Image *inputImage, Image *outputImage );
    ~XbrFilter();
    void apply();
};

#endif // XBRFILTER_H

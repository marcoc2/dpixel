#ifndef XBRFILTER_H
#define XBRFILTER_H

#include "Filters/Filter.h"

class XbrFilter : public Filter
{
public:
    XbrFilter( Image *inputImage, float scaleFactor );
    ~XbrFilter();
    void apply();
};

#endif // XBRFILTER_H

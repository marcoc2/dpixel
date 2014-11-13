#ifndef XBRZFILTER_H
#define XBRZFILTER_H

#include "Image.h"
#include "Filters/Filter.h"

class XbrZFilter : public Filter
{
public:
    XbrZFilter(Image *inputImage, Image *outputImage);
    ~XbrZFilter();
    void apply();
};

#endif // XBRZFILTER_H

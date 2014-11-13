#ifndef HQXFILTER_H
#define HQXFILTER_H

#include "Image.h"
#include "Filters/Filter.h"

class HqxFilter : public Filter
{
public:
    HqxFilter(Image *inputImage, Image *outputImage);
    ~HqxFilter();
    void apply();
};

#endif // HQXFILTER_H

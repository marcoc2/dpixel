#ifndef SCALE2XFILTER_H
#define SCALE2XFILTER_H

#include "Filter.h"

class Scale2xFilter : public Filter
{
public:
    Scale2xFilter( Image* inputImage, Image* outputImage );
    ~Scale2xFilter();
    void apply();
    void apply2x( Image* ouput4xImage );
};

#endif // SCALE2XFILTER_H

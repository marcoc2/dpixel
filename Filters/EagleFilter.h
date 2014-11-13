#ifndef EAGLEFILTER_H
#define EAGLEFILTER_H

#include "Filter.h"

class EagleFilter : public Filter
{
public:
    EagleFilter( Image* inputImage, Image* outputImage );
    ~EagleFilter();
    void apply();
};

#endif // EAGLEFILTER_H

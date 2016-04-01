#ifndef EAGLEFILTER_H
#define EAGLEFILTER_H

#include "Filter.h"

class EagleFilter : public Filter
{
public:
    EagleFilter( Image* inputImage, float scaleFactor );
    ~EagleFilter();
    void apply();
};

#endif // EAGLEFILTER_H

#ifndef NEARESTFILTER_H
#define NEARESTFILTER_H

#include "Filters/Filter.h"
#include "Image.h"

class NearestFilter : public Filter
{
public:

    NearestFilter(Image *inputImage);
    ~NearestFilter();

private:

    void apply();
};

#endif // NEARESTFILTER_H

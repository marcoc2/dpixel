#ifndef BEADSFILTER_H
#define BEADSFILTER_H

#include "Filter.h"

class BeadsFilter : public Filter
{
public:
    BeadsFilter(  Image* inputImage, float scaleFactor );
    ~BeadsFilter();

    void run();
    void apply();

private:

    Image* _intermediateImage;
};

#endif // BEADSFILTER_H

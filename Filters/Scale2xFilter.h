#ifndef SCALE2XFILTER_H
#define SCALE2XFILTER_H

#include "Filter.h"

class Scale2xFilter : public Filter
{
public:
    Scale2xFilter( Image* inputImage, Image* outputImage );
    Scale2xFilter( Image* inputImage, int numberOfPasses );
    ~Scale2xFilter();
    void run();
    void apply();
    Image* pass( int step, Image* inputImage );

private:

    int _numberOfPasses;
};

#endif // SCALE2XFILTER_H

#ifndef EAGLEFILTER_H
#define EAGLEFILTER_H

#include "Filter.h"

class EagleFilter : public Filter
{
public:
    explicit EagleFilter( Image* inputImage, float scaleFactor );
    explicit EagleFilter( Image* inputImage, int numberOfPasses );
    ~EagleFilter();

    void run();
    void apply();

    Image* pass( int step, Image* inputImage );

private:

    int _numberOfPasses;
};

#endif // EAGLEFILTER_H

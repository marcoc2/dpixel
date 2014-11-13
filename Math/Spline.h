#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include "Math/Point2D.h"

/*********************************************************************

     Parameters:
     numControlPoints  - the number of control points minus 1
     degree     - the degree of the polynomial plus 1
     control    - control point array made up of point stucture
     output     - array in which the calculate spline points are to be put
     num_output - how many points on the spline are to be calculated

     Pre-conditions:
     numControlPoints+2 > degree  (no curve results if numControlPoints+2 <= degree)
     control array contains the number of points specified by numControlPoints
     output array is the proper size to hold num_output point structures


 **********************************************************************/

class Spline
{
public:
    Spline( int numControlPoints,
            int degree,
            std::vector<Point2D> controlPoints,
            std::vector<Point2D>* outputPoints,
            int outputSize );

    ~Spline();

    void calculate();

private:

    // Figure out the knots
    void computeIntervals( std::vector<int> *u, int n, int t );

    // Calculate the blending value
    double blend( int k, int t, std::vector<int> *u, double v );

    void computePoint( std::vector<int> *u, int n, int t,
                        double v, std::vector<Point2D> control, Point2D *output );


    int _numControlPoints;
    int _degree;
    std::vector<Point2D> _controlPoints;
    std::vector<Point2D>* _outputPoints;
    int _outputSize;
};

#endif // SPLINE_H

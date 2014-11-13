#include "Spline.h"

Spline::Spline( int numControlPoints, int degree, std::vector<Point2D> controlPoints,
                      std::vector<Point2D>* outputPoints, int outputSize ) :
    _numControlPoints( numControlPoints ),
    _degree( degree ),
    _controlPoints( controlPoints ),
    _outputPoints( outputPoints ),
    _outputSize( outputSize )
{


}



Spline::~Spline()
{

}



void Spline::calculate()
{
    double increment, interval;
    Point2D calc_xy;
    int outputIndex;

    std::vector<int> u(_numControlPoints + _degree + 1);
    computeIntervals( &u, _numControlPoints, _degree );

    // How much parameter goes up each time
    increment = (double) ( _numControlPoints - _degree + 2 ) / ( _outputSize - 1 );
    interval = 0;

    for ( outputIndex = 0; outputIndex < _outputSize - 1; outputIndex++ )
    {
        computePoint( &u, _numControlPoints, _degree, interval, _controlPoints, &calc_xy );

        _outputPoints->at(outputIndex).x = calc_xy.x;
        _outputPoints->at(outputIndex).y = calc_xy.y;

        interval = interval + increment;
    }

    // Put in the last point
    _outputPoints->at(_outputSize - 1).x = _controlPoints[_numControlPoints].x;
    _outputPoints->at(_outputSize - 1).y = _controlPoints[_numControlPoints].y;
}



double Spline::blend( int index, int degree, std::vector<int> *u, double interval )
{
    double value;

    // Base case for the recursion
    if ( degree == 1 )
    {

        if ( ( u->at(index) <= interval ) && ( interval < u->at(index + 1) ) )
            value = 1;
        else
            value = 0;

    }
    else
    {
        // Check for divide by zero
        if ( ( u->at(index + degree - 1) == u->at(index) ) && ( u->at(index + degree) == u->at(index + 1) ) )
            value = 0;

        // if a term's denominator is zero,use just the other
        else if ( u->at(index + degree - 1) == u->at(index) )
            value = ( u->at(index + degree) - interval )
                                /
                    ( u->at(index + degree) - u->at(index + 1) ) * blend( index + 1, degree - 1, u, interval );

        else if ( u->at(index + degree) == u->at(index + 1) )
            value = ( interval - u->at(index) )
                                /
                    ( u->at(index + degree - 1) - u->at(index) ) * blend( index, degree - 1, u, interval );

        else
            value = ( interval - u->at(index) )
                                /
                    ( u->at(index + degree - 1) - u->at(index) ) * blend( index, degree - 1, u, interval ) +
                    ( u->at(index + degree) - interval ) / ( u->at(index + degree) - u->at(index + 1) ) * blend( index + 1, degree - 1, u, interval );
    }

    return value;
}



void Spline::computeIntervals( std::vector<int> *u, int numControlPoints, int degree )
{
    int j;

    for ( j = 0; j <= numControlPoints + degree; j++ ) {
        if ( j < degree )
            u->at(j) = 0;

        else if ( ( degree <= j ) && ( j <= numControlPoints ) )
            u->at(j) = j - degree + 1;

        // if numControlPoints - degree = -2 then we're screwed, everything goes to 0
        else if ( j > numControlPoints )
            u->at(j) = numControlPoints - degree + 2;
    }
}



void Spline::computePoint( std::vector<int> *u, int numControlPoints, int degree, double interval,
                           std::vector<Point2D> control, Point2D *output )
{
    int index;
    double temp;

    // Initialize the variables that will hold our outputted point
    output->x = 0;
    output->y = 0;
    //output->z = 0;

    for ( index = 0; index <= numControlPoints; index++ ) {
        // Same blend is used for each dimension coordinate
        temp = blend( index, degree, u, interval );
        output->x = output->x + ( control[index] ).x * temp;
        output->y = output->y + ( control[index] ).y * temp;
        //output->z = output->z + ( control[k] ).z * temp;
    }
}

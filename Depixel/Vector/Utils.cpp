/*
 * 2016 - Marco Aurélio G. da Silva ( marcoc2@gmail.com )
 */

#include "Utils.h"

#include <iostream>
#include "Math/Point2D.h"

/**
 * @brief c_neighbor_index          Return neighbor node index according to the connected edge
 * @param index                     Node index
 * @param edge                      Edge index (0-7)
 * @param width                     Image width
 * @return                          Neighbor node index according to the connected edge
 */
int c_neighbor_index(int index, int edge, int width)
{
    //int result;
    switch(edge)
    {
    case 0: return index + width - 1;
    case 1: return index + width;
    case 2: return index + width + 1;
    case 3: return index - 1;
    case 4: return index + 1;
    case 5: return index - width - 1;
    case 6: return index - width;
    case 7: return index - width + 1;
    }

    return -1;
}

/**
 * @brief calc_index_x              Return coordinate x of connected node index according to the connected edge
 * @param j                         Coordinate j (current x coordinate)
 * @param edge                      Edge index (0 - 7)
 * @return                          Coordinate x of connected node index according to the connected edge
 */
int calc_index_x(int j, int edge)
{
    //int result;
    switch(edge)
    {
    case 0: return --j;
    case 1: return j;
    case 2: return ++j;
    case 3: return --j;
    case 4: return ++j;
    case 5: return --j;
    case 6: return j;
    case 7: return ++j;
    }

    return -1;
}

/**
 * @brief calc_index_y              Return coordinate y of connected node index according to the connected edge
 * @param i                         Coordinate i (current y coordinate)
 * @param edge                      Edge index (0 - 7)
 * @return                          Coordinate y of connected node index according to the connected edge
 */
int calc_index_y(int i, int edge)
{
    //int result;
    switch(edge)
    {
    case 0: return ++i;
    case 1: return ++i;
    case 2: return ++i;
    case 3: return i;
    case 4: return i;
    case 5: return --i;
    case 6: return --i;
    case 7: return --i;
    }

    return -1;
}

/**
 * @brief connected_edge            Return edge index for the neighbor connected edge
 * @param edge                      Edge index
 * @return                          Edge index for the neighbor connected edge
 */
int connected_edge(int edge)
{
    return (8-1)-edge;

    /*    switch(edge)
        {
            case 0: return 7;
            case 1: return 6;
            case 2: return 5;
            case 3: return 4;
            case 4: return 3;
            case 5: return 2;
            case 6: return 1;
            case 7: return 0;
        }
    */
}

/**
 * @brief cross                     Implementation of Andrew's monotone chain 2D convex hull algorithm.
 *                                  2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
 *                                  Returns a positive value, if OAB makes a counter-clockwise turn,
 *                                  negative for clockwise turn, and zero if the points are collinear.
 * @link                            http://en.wikibooks.org/wiki/Algorithm_Implementation/Geometry/Convex_hull/Monotone_chain#C.2B.2B
 * @param O                         Point O
 * @param A                         Point A
 * @param B                         Point B
 * @return                          Cross product value for OA and OB
 */
coord2_t cross(const Point &O, const Point &A, const Point &B)
{
    return (A.x - O.x) * (coord2_t)(B.y - O.y) - (A.y - O.y) * (coord2_t)(B.x - O.x);
}

/**
 * @brief convex_hull               Returns a list of points on the convex hull in counter-clockwise order.
 *                                  Note: the last point in the returned list is the same as the first one.
 * @param P                         List of points
 * @return                          Points in the convex hull of P
 */
std::vector< Point > convex_hull( std::vector< Point > P )
{
    const int magnify = 100;
    int n = P.size();
    int k = 0;
    std::vector< Point > H( 2 * n );
    H.resize( 20 );

    // Sort points lexicographically
    sort( P.begin(), P.end() );

    // Build lower hull
    for( int i = 0; i < n; i++ )
    {
        while( k >= 2 && cross( H[ k - 2 ] * magnify, H[ k - 1 ] * magnify, P[ i ] * magnify ) <= 0 )
        {
            k--;
        }
        H[ k++ ] = P[ i ];
        //H.push_back(P[i]);
        //k++;
    }

    // Build upper hull
    for( int i = n - 2, t = k + 1; i >= 0; i-- )
    {
        while( k >= t && cross( H[ k - 2 ] * magnify, H[ k - 1 ] * magnify, P[ i ] * magnify ) <= 0 )
        {
            k--;
        }
        H[ k++ ] = P[ i ];
        //H.push_back(P[i]);
        //k++;
    }

    H.resize( k );
    return H;
}

/**
 * @brief getRealLinkIndex           Maps to real bit order for the graph links
 *                                  0 - 7 in clockwise order
 *
 *                                       Input               Output
 *
 *                                  +----+----+----+     +----+----+----+
 *                                  |    |    |    |     |    |    |    |
 *                                  | 0  | 1  | 2  |     | 0  | 1  | 2  |
 *                                  +----+----+----+     +----+----+----+
 *                                  |    |    |    |     |    |    |    |
 *                                  | 7  | x  | 3  |     | 3  | x  | 4  |
 *                                  +----+----+----+     +----+----+----+
 *                                  |    |    |    |     |    |    |    |
 *                                  | 6  | 5  | 4  |     | 5  | 6  | 7  |
 *                                  +----+----+----+     +----+----+----+
 *
 * @param index                     Node index
 * @return                          Edge index in real bit order
 */
int getRealLinkIndex(int index){
    switch(index)
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    case 3: return 4;
    case 4: return 7;
    case 5: return 6;
    case 6: return 5;
    case 7: return 3;
    }

    return -1;
}

/**
 * @brief getClockLinkIndex         Maps to clockwise bit order for the graph links
 *                                   0 - 7 in clockwise order
 *
 *                                          Input                Output
 *
 *                                      +----+----+----+     +----+----+----+
 *                                      |    |    |    |     |    |    |    |
 *                                      | 0  | 1  | 2  |     | 0  | 1  | 2  |
 *                                      +----+----+----+     +----+----+----+
 *                                      |    |    |    |     |    |    |    |
 *                                      | 3  | x  | 4  |     | 7  | x  | 3  |
 *                                      +----+----+----+     +----+----+----+
 *                                      |    |    |    |     |    |    |    |
 *                                      | 5  | 6  | 7  |     | 6  | 5  | 4  |
 *                                      +----+----+----+     +----+----+----+
 *
 * @param index                     Node index
 * @return                          Edge index in clockwise order
 */
int getClockLinkIndex(int index){
    switch(index)
    {
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    case 4: return 3;
    case 7: return 4;
    case 6: return 5;
    case 5: return 6;
    case 3: return 7;
    }

    return -1;
}

/**
 * @brief isIntersected             Verify if current cell edge is intersected by a graph link
 * @param a                         Point a
 * @param b                         Point b
 * @param node                      Node links as char value
 * @param index                     Node index
 * @param width                     Image/Graph width
 * @return                          TRUE if cell edge is intersected
 *                                  FALSE if cell edge is not intersected
 */
bool isIntersected(Point a, Point b, char node, int index, int width)
{
    float slope;
    int x = (index % width);
    int y = (int) (index / width);
    Point center(x + 0.5, y + 0.5);
    Point average((a.x+b.x)/2, (a.y+b.y)/2);

    if ((b.x - a.x) == 0)
        slope = 99;
    else
        slope = (b.y - a.y) / (b.x - a.x);

    if ( (slope != 1) && (slope != 0) && (slope != -1) && (slope != 99) )
        return false;

    switch ( (int) slope)
    {
    case -1:
    {
        if ((average.y > center.y) && (CHECK_BIT(node, 2)))
            return true;
        else if ((average.y < center.y) && (CHECK_BIT(node, 5)))
            return true;
        else
            return false;
    }
    case 0:
    {
        if ((average.y > center.y) && (CHECK_BIT(node, 1)))
            return true;
        else if ((average.y < center.y) && (CHECK_BIT(node, 6)))
            return true;
        else
            return false;
    }
    case 1:
    {
        if ((average.y > center.y) && (CHECK_BIT(node, 0)))
            return true;
        else if ((average.y < center.y) && (CHECK_BIT(node, 7)))
            return true;
        else
            return false;
    }
    case 99:
    {
        if ((average.x > center.x) && (CHECK_BIT(node, 4)))
            return true;
        else if ((average.x < center.x) && (CHECK_BIT(node, 3)))
            return true;
        else
            return false;
    }

    }

    /* For other any slope */
    return false;
}

/**
  @brief getNormalizedPoint         Return a point displaced by its related index.
                                    This is necessery cause cell's points have relative geometric data
  @param p                          Point with relative geometric data
  @param index                      Index relative to the pixel/node/cell location on image
  @param width                      Image/graph/diagram width
  @param height                     Image/graph/diagram height
  @return                           Point displaced in pixels according to the index
  */
Point getNormalizedPoint(Point p, int index, int width)
{
    int x = (index % width);
    int y = (int) (index / width);

    return Point(p.x + x, p.y + y);
}

/**
 * @brief                           Return a vector of points displaced by its related index.
 *                                  This is necessery cause cell's points have relative geometric data
 * @param v                         Vector of points with relative geometric data
 * @param index                     Index relative to the pixel/node/cell location on image
 * @param width                     Image/graph/diagram width
 * @param height                    Image/graph/diagram height
 * @return out                      Vector of points displaced in pixels according to the index
 */
std::vector<Point> getNormalizedPointVector(std::vector<Point> v, int index, int width)
{
    std::vector<Point> out;

    int x = (index % width);
    int y = (int) (index / width);

    for (unsigned int i = 0; i < v.size(); i++)
    {
        out.push_back(Point(v.at(i).x + x, v.at(i).y + y));
    }

    return out;
}

std::vector<Point> reverseVector(std::vector<Point> v)
{
    std::vector<Point> out;

    for (int i = (int) v.size() - 1; i >= 0; i--)
    {
        out.push_back(v.at(i));
    }

    return out;

}

bool isColorEqual(char* c1, char* c2)
{
    if ( ( c1[0] == c2[0] ) && ( c1[1] == c2[1] ) && ( c1[2] == c2[2] ) )
        return true;
    else
        return false;
}

int compareColors(char* c1, char* c2, char* c3, char* c4)
{
//    char r1, r2, r3, r4;
//    char g1, g2, g3, g4;
//    char b1, b2, b3, b4;

    /* At least one color */
    int nColors = 1;

    if (!isColorEqual(c1,c2))
        nColors++;

    if (!isColorEqual(c2,c3))
        if (!isColorEqual(c1,c3))
            nColors++;

    if (!isColorEqual(c2,c4))
        if (!isColorEqual(c3,c4))
            if (!isColorEqual(c1,c4))
                nColors++;

    return nColors;

}

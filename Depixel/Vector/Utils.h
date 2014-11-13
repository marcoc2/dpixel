#ifndef UTILS_H
#define UTILS_H

#include "Image.h"
#include <vector>
#include "Math/Point2D.h"

#define Point Point2D

/** Check bit value */
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

/** Clean bit then set */
#define SET_BIT(var,pos,data) var &= ~(1 << pos); var |= data << pos

int c_neighbor_index(int index, int edge, int width);

/**
 * @brief calc_index_x              Return coordinate x of connected node index according to the connected edge
 * @param j                         Coordinate j (current x coordinate)
 * @param edge                      Edge index (0 - 7)
 * @return                          Coordinate x of connected node index according to the connected edge
 */
int calc_index_x(int j, int edge);

/**
 * @brief calc_index_y              Return coordinate y of connected node index according to the connected edge
 * @param i                         Coordinate i (current y coordinate)
 * @param edge                      Edge index (0 - 7)
 * @return                          Coordinate y of connected node index according to the connected edge
 */
int calc_index_y(int i, int edge);

/**
 * @brief connected_edge            Return edge index for the neighbor connected edge
 * @param edge                      Edge index
 * @return                          Edge index for the neighbor connected edge
 */
int connected_edge(int edge);

/* coordinate type */
typedef int coord_t;
/* must be big enough to hold 2*max(|coordinate|)^2 */
typedef long long coord2_t;

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
coord2_t cross(const Point &O, const Point &A, const Point &B);

/**
 * @brief convex_hull               Returns a list of points on the convex hull in counter-clockwise order.
 *                                  Note: the last point in the returned list is the same as the first one.
 * @param P                         List of points
 * @return                          Points in the convex hull of P
 */
std::vector<Point> convex_hull(std::vector<Point> P);

/**
 * @brief getRealLinkIndex           Maps to real bit order for the graph links
 *                                   0 - 7 in clockwise order
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
int getRealLinkIndex(int index);

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
int getClockLinkIndex(int index);

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
bool isIntersected(Point a, Point b, char node, int index, int width);

/**
 * @brief getNormalizedPoint        Return a point displaced by its related index.
 *                                  This is necessery cause cell's points have relative geometric data
 * @param p                         Point with relative geometric data
 * @param index                     Index relative to the pixel/node/cell location on image
 * @param width                     Image/graph/diagram width
 * @param height                    Image/graph/diagram height
 * @return                          Point displaced in pixels according to the index
 */
Point getNormalizedPoint(Point p, int index, int width);

/**
 * @brief                           Return a vector of points displaced by its related index.
 *                                  This is necessery cause cell's points have relative geometric data
 * @param v                         Vector of points with relative geometric data
 * @param index                     Index relative to the pixel/node/cell location on image
 * @param width                     Image/graph/diagram width
 * @param height                    Image/graph/diagram height
 * @return out                      Vector of points displaced in pixels according to the index
 */
std::vector<Point> getNormalizedPointVector(std::vector<Point> v, int index, int width);

std::vector<Point> reverseVector(std::vector<Point> v);

bool isColorEqual(char* c1, char* c2);

int compareColors(char* c1, char* c2, char* c3, char* c4);

#endif // UTILS_H

/*
 * 2016 - Marco Aurélio G. da Silva ( marcoc2@gmail.com )
 */

#ifndef VORONOI_CELL_H
#define VORONOI_CELL_H

#include <vector>
#include "SimilarityGraph.h"
#include "Math/Point2D.h"

using namespace std;


/**
 * class Diagramcell
 */
class DiagramCell
{
public:

    /* Constructors/Destructors */

    /**
     * Empty Constructor
     */
    DiagramCell();

    /**
     * Empty Destructor
     */
    virtual ~DiagramCell();


private:

    /* Private attributes */

    int _numberOfPoints;
    char _color[ 3 ];
    std::vector< Point2D > _points;

public:

    /**
     * @brief createCellFromPattern             New version: automatic, not manual
     *                                          It gives a point for each bit
     *                                          If the bit is on the point is one "unity" away from the center
     *                                          If it is off the point is half "unity" away from the center
     *                                          The convex hull of these points return the polygon that forms the voronoi cell
     * @param node                              Graph node links as char value
     */
    void createCellFromPattern( SimilarityGraph::Node node, SimilarityGraph::Node nodeLeft,
                                SimilarityGraph::Node nodeRight );

    /**
     * @brief getBottomRightPoint       Return pointer to the bottom right point
     * @return                          Pointer to the bottom right point
     */
    Point2D* getBottomRightPoint();

    /**
     * @brief getBottomLeftPoint        Return pointer to the bottom left point
     * @return                          Pointer to the bottom left point
     */
    Point2D* getBottomLeftPoint();

    /**
     * @brief getUpperRightPoint        Return pointer to the upper right point
     * @return                          Pointer to the upper right point
     */
    Point2D* getUpperRightPoint();

    /**
     * @brief getUpperLeftPoint         Return pointer to the upper left point
     * @return                          Pointer to the upper left point
     */
    Point2D* getUpperLeftPoint();

    bool isDiagonalCell();

    bool isDiagonalBorderCell();


    void printCellInfo();


    /* Accessor methods */

    /**
     * @brief getN_points                   Get the value of n_points
     * @return                              The value of n_points
     */
    int getN_points ();

    /**
     * @brief getPoints                     Get the value of points
     * @return                              The value of points
     */
    vector< Point2D > getPoints ();

    void setColor( char* _color );
    char* getColor();

    /**
     * @brief vectorToArrayOfPoints         Convert an array of Voronoi_cell::Point to an array of CvPoint
     *                                      Argument x, y and scale is given to adjust points coord
     * @param inVector                      Input vector of points
     * @param x                             Offset in x axis
     * @param y                             Offset in y axis
     * @param scale                         Image scale
     * @return
     */
    //static CvPoint2D* vectorToArrayOfPoints(vector<Point2D> inVector, int weight, int height, int scale);
};


#endif // VORONOI_CELL_H

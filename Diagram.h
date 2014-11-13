#ifndef DIAGRAM_H
#define DIAGRAM_H

#include <string>
#include "DiagramCell.h"
#include "SimilarityGraph.h"
#include "Math/Spline.h"
#include "Math/Point2D.h"

/**
 * class Diagram
 */

class Diagram
{
public:

    /**
     * Empty Constructor
     */
    Diagram ( );

    /**
     * Empty Destructor
     */
    virtual ~Diagram ( );


private:

    /* Matrix of Voronoi Cells */
    DiagramCell** _cells;
    int _width;
    int _height;
    std::vector<Point2D> _TJunctionPoints;

public:

    /* Methods */

    /**
     * @brief createDiagram             Create a generalized Voronoi Diagram for the input graph
     * @param graph                     Similarity graph generated for the image
     */
    void createDiagram(SimilarityGraph* graph);

    /**
     * @brief printToImage              Make a image representation of the diagram
     * @param img                       Image container that will receive data
     * @param scale_graph               Image scale
     * @param show_borders              Define if cell's borders will be displayed
     */
    void printToImage(Image* img, float scale_graph, bool show_border);

    /**
     * @brief printPixelsBorders        Draw lines to represent pixels borders
     * @param img                       Image container that will receive data
     * @param scale_graph               Image scale
     */
    void printPixelsBorders(Image* img, float scale_graph);

    /**
     * @brief printToSplines            Make a image representation of the splines
     * @param img                       Image container that will receive data
     * @param scale_graphh              Image scale
     */
    void printToSplines(Image* img, float scale_graph);

    /**
     * @brief extractFirstCellEdges     Extract cell edges from first cell where starts
     *                                  extraction of control points for the spline
     * @param node                      SimilarityGraph node
     * @param cellPoints                Diagram cell points
     * @param index                     Node index
     * @param width                     SimilarityGraph width
     * @return                          List of spline first control points
     */
    vector<Point2D> extractFirstCellEdges(SimilarityGraph* graph, vector<Point2D> cellPoints, int index, int _width);

    /**
     * @brief extractCellEdges          Extract cell edges to be control points of the spline
     * @param node                      SimilarityGraph node
     * @param cellPoints                Diagram cell points
     * @param lastPoint                 Last point processed on previous cell
     * @param index                     Node index
     * @param width                     SimilarityGraph width
     * @return                          List of cell points that are part of spline control points
     */
    vector<Point2D> extractCellEdges(SimilarityGraph* graph, vector<Point2D> cellPoints, Point2D lastPoint, int index, int _width);

    /**
     * @brief exctractSplinePoints      Extracts the points of the cell in right order to build the spline
     * @param graph                     Similarity graph generated for the image
     * @param borderIndexes             Indexes of nodes that are in border of connected componenents of the graph
     * @return                          List of spline control points
     */
    vector<Point2D> extractSplinePoints(SimilarityGraph* graph, vector<int> borderIndexes);

    /**
     * @brief extractBorderPoints       Walk throught the borders of the graph's conected components.
     *                                  Extracts the cell's points during this process to be te input of the spline generation
     * @param graph                     Similarity graph generated for the image
     * @param colorList                 List of splines color
     * @return                          List of splines
     */
    vector<vector<Point2D> > extractBorderPoints(SimilarityGraph* graph, vector<char*> *colorList);

    void optimize();

    bool detectTJunction(int index, int _width);

    bool detectTPoint(int j, int i, int _width, Point2D *out);

    std::vector< std::vector <Point2D> > getPolygons();


    /* Accessor methods */

    /**
     * @brief setCells                  Set the value of cell
     * @param new_var                   The new value of cell
     */
    void setCells ( DiagramCell** new_var );

    /**
     * @brief getCells                  Get the value of cell
     * @return                          The value of cell
     */
    DiagramCell** getCells ( );

private:


};

#endif // DIAGRAM_H

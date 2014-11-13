#include "Diagram.h"
#include "Depixel/Vector/Utils.h"

/* Constructors/Destructors */

using namespace std;

Diagram::Diagram ( ) {

}

Diagram::~Diagram ( ) {
    free(_cells);
}

/* Methods */

/**
 * @brief createDiagram             Create a generalized Voronoi Diagram for the input graph
 * @param graph                     Similarity graph generated for the image
 */
void Diagram::createDiagram(SimilarityGraph* graph)
{
    _width = graph->getWidth();
    _height = graph->getHeight();

    _cells = (DiagramCell**) malloc(_width*_height*sizeof(DiagramCell*));

    int index;

    int indexLeft, indexRight;

    for(int i = 0 ; i < _height ; i++ ) {
        for(int j = 0 ; j < _width ; j++ ) {
            index = (i*_width + j);

            indexLeft = ((i*_width + j) - 1 );
            indexRight = ((i*_width + j) + 1);

            _cells[index] = new DiagramCell;
            _cells[index]->setColor(graph->getGraphBuffer()[index].color);
            _cells[index]->createCellFromPattern(graph->getNodeIndex(index), graph->getNodeIndex(indexLeft), graph->getNodeIndex(indexRight));
        }
    }
}



std::vector< std::vector <Point2D> > Diagram::getPolygons()
{
    std::vector< std::vector <Point2D> > polygonVector;
    polygonVector.reserve( _width * _height );

    for(int i = 0 ; i < _height ; i++ )
    {
        for(int j = 0 ; j < _width ; j++ )
        {
            int index = (i*_width + j);
            if (_cells[index]->getPoints().size() > 0)
            {
                polygonVector.push_back( _cells[index]->getPoints() );
            }
        }
    }

    return polygonVector;
}


void Diagram::printToImage(Image* img, float scale_graph, bool show_border)
{
//    int index;

//    for(int i = 0 ; i < _height ; i++ ) {
//        for(int j = 0 ; j < _width ; j++ ) {
//            index = (i*_width + j);

//            int fill_color[3] = {(unsigned int) (uchar) _cells[index]->getColor()[0],
//                                 (unsigned int) (uchar) _cells[index]->getColor()[1],
//                                 (unsigned int) (uchar) _cells[index]->getColor()[2]};
//            int line_color[3] = {128,128,128};

//            /* If it has a cell.. */
//            if (_cells[index]->getPoints().size() > 0){

//                CvPoint* polygon = DiagramCell::vectorToArrayOfPoints(_cells[index]->getPoints(), j, i, scale_graph) ;

//                cvFillConvexPoly(img->getImg(), polygon ,
//                                 _cells[index]->getPoints().size(), CV_RGB( fill_color[0], fill_color[1], fill_color[2] ), CV_AA, 0);
//                /* Verificar se esse free() faz efeito */
//                free(polygon);

//                if (show_border){

//                    /* Draw line between last and first point */
//                    cvLine( img->getImg(), cvPoint(int(((_cells[index]->getPoints().at(0)).x*scale_graph) +
//                                                       (j*scale_graph)),
//                                                   int(((_cells[index]->getPoints().at(0)).y*scale_graph) +
//                                                       (i*scale_graph))),
//                            cvPoint(int(((_cells[index]->getPoints().at(_cells[index]->getPoints().size()-1)).x*scale_graph) +
//                                        (j*scale_graph)),
//                                    int(((_cells[index]->getPoints().at(_cells[index]->getPoints().size()-1)).y*scale_graph) +
//                                        (i*scale_graph))),
//                            CV_RGB( line_color[0], line_color[1], line_color[2] ), 2, CV_AA, 0 );

//                    /* Draw the others lines of the cell */
//                    for (int t = 0; t < _cells[index]->getPoints().size() - 1; t++){
//                        cvLine( img->getImg(), cvPoint(int(((_cells[index]->getPoints().at(t)).x*scale_graph) +
//                                                           (j*scale_graph)),
//                                                       int(((_cells[index]->getPoints().at(t)).y*scale_graph) +
//                                                           (i*scale_graph))),
//                                cvPoint(int(((_cells[index]->getPoints().at(t+1)).x*scale_graph) +
//                                            (j*scale_graph)),
//                                        int(((_cells[index]->getPoints().at(t+1)).y*scale_graph) +
//                                            (i*scale_graph))),
//                                CV_RGB( line_color[0], line_color[1], line_color[2] ), 2, CV_AA, 0 );
//                    }
//                }
//            }
//        }
//    }
}



void Diagram::printPixelsBorders(Image* img, float scale_graph){

//    int index;


//    for(int i = 0 ; i < _height ; i++ ) {
//        for(int j = 0 ; j < _width ; j++ ) {
//            index = (i*_width + j);

//            cvLine( img->getImg(), cvPoint(j*scale_graph, i*scale_graph),
//                    cvPoint((j)*scale_graph, (i+1)*scale_graph), CV_RGB( 0, 0, 0 ), 1, CV_AA, 0 );

//            cvLine( img->getImg(), cvPoint(j*scale_graph, i*scale_graph),
//                    cvPoint((j+1)*scale_graph, (i)*scale_graph), CV_RGB( 0, 0, 0 ), 1, CV_AA, 0 );
//        }
//    }
}



vector<Point2D> Diagram::extractFirstCellEdges(SimilarityGraph* graph, vector<Point2D> cellPoints, int index, int width)
{
    vector<Point2D> cEdgePoints;

    bool hasTJunction = this->detectTJunction(index, width);

    /* Find the point with lower Y coordinate to start from
       RTK Try to rethink a way to generate the cell already starting from this point */
    float lowest = cellPoints.at(0).y;
    int lowestIndex = 0;

    for (unsigned int i = 1; i < cellPoints.size(); i++)
    {
        if (lowest >= cellPoints.at(i).y)
        {
            lowest = cellPoints.at(i).y;
            lowestIndex = i;
        }
    }

    int teste = lowestIndex - cellPoints.size();

    /* Loop reverse to walk in clockwise order
       + cellPoints.size() to take mod and guarantee non negative */
    for (int i = lowestIndex; i > (teste); i--)
    {
        if (!isIntersected(cellPoints.at((i + cellPoints.size() ) % cellPoints.size()),
                           cellPoints.at((i - 1 + cellPoints.size() ) % cellPoints.size()), graph->getNodeIndex(index).value,
                           index, width))
        {
            /* When there is t-juntion points in 2x2 block this must be inserted
               more times to work like a weighted control point in the spline */
            if (hasTJunction)
            {
                /* detectTJunction() function returns more than on points, so
                   its necessary to test if one of these points is the one to be spline control point */
                for (unsigned int j = 0; j < _TJunctionPoints.size(); j++)
                {
                    if (getNormalizedPoint(_TJunctionPoints.at(j), index, width) == cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()))
                    {
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        break;
                    }

                }

                cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
            }
            else
                cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
            //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
        }
        else
        {
            /* When there is t-juntion points in 2x2 block this must be inserted
               more times to work like a weighted control point in the spline */
            if (hasTJunction)
            {
                for (unsigned int j = 0; j < _TJunctionPoints.size(); j++)
                {
                    if (getNormalizedPoint(_TJunctionPoints.at(j), index, width) == cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()))
                    {
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        break;
                    }
                    else
                    {
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        break;
                    }
                }
                break;
            }
            else
            {
                cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                break;
            }
        }
    }

    return cEdgePoints;

}



vector<Point2D> Diagram::extractCellEdges(SimilarityGraph* graph, vector<Point2D> cellPoints, Point2D lastPoint, int index, int width)
{
    vector<Point2D> cEdgePoints;
    //vector<Point2D> TJunctionPoints;

    bool hasTJunction = this->detectTJunction(index, width);

    //vector<Point2D> tpoints = getNormalizedPointVector(_TJunctionPoints, index, width);

    /* Find the index of the lastPoint
       RTK Try to rethink a way to generate the cell already starting from this point */
    int lastPointIndex = -1;

    for (unsigned int i = 0; i < cellPoints.size(); i++)
    {
        if (lastPoint == cellPoints.at(i))
        {
            lastPointIndex = i;
            break;
        }
    }

    int teste = lastPointIndex - 1 - cellPoints.size();

    /* Loop reverse to walk in clockwise order
       "+ cellPoints.size()" to take mod and guarantee non negative
       "lastPointIndex - 1" cause last point is already stored */
    for (int i = lastPointIndex - 1; i > (teste); i--)
    {
        if (!isIntersected(cellPoints.at((i + cellPoints.size() ) % cellPoints.size()),
                           cellPoints.at((i - 1 + cellPoints.size() ) % cellPoints.size()), graph->getNodeIndex(index).value,
                           index, width))
        {
            /* When there is t-juntion points in 2x2 block this must be inserted
               more times to work like a weighted control point in the spline */
            if (hasTJunction)
            {
                /* detectTJunction() function returns more than on points, so
                   its necessary to test if one of these points is the one to be spline control point */
                for (unsigned int j = 0; j < _TJunctionPoints.size(); j++)
                {
                    if (getNormalizedPoint(_TJunctionPoints.at(j), index, width) == cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()))
                    {
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        break;
                    }

                }

                cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
            }
            else
                cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
            //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
        }
        else
        {
            /* When there is t-juntion points in 2x2 block this must be inserted
               more times to work like a weighted control point in the spline */
            if (hasTJunction)
            {
                for (unsigned int j = 0; j < _TJunctionPoints.size(); j++)
                {
                    if (getNormalizedPoint(_TJunctionPoints.at(j), index, width) == cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()))
                    {
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        //cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        break;
                    }
                    else
                    {
                        cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                        break;
                    }
                }
                break;
            }
            else
            {
                cEdgePoints.push_back( cellPoints.at( (i + cellPoints.size() ) % cellPoints.size()) );
                break;
            }
        }
    }

    return cEdgePoints;
}



vector<Point2D> Diagram::extractSplinePoints(SimilarityGraph* graph, vector<int> borderIndexes)
{
    //char node;
    SimilarityGraph::Node node;
    DiagramCell* cell;
    vector<Point2D> cellPoints;
    vector<Point2D> splinePoints;
    vector<Point2D> cEdgePoints;
    /* Point that will stitch the cell's border with the neighbor cell's border */
    Point lastPoint;

    /* ESPECIAL CASE: First Cell */
    //node = graph->getNodeIndex(borderIndexes.at(0));
    cell = this->_cells[borderIndexes.at(0)];
    cellPoints = getNormalizedPointVector(cell->getPoints(), borderIndexes.at(0), _width);

    vector<Point2D> cFirstEdgePoints = this->extractFirstCellEdges(graph, cellPoints, borderIndexes.at(0), _width);

//    lastPoint = getNormalizedPoint(cFirstEdgePoints.at(cFirstEdgePoints.size() - 1), borderIndexes.at(0),
//                                   width);
    lastPoint = cFirstEdgePoints.at( cFirstEdgePoints.size() - 1 );

    for (int j = cFirstEdgePoints.size(); j > 0; j--)
    {
//        splinePoints.push_back( getNormalizedPoint( cFirstEdgePoints.at( cFirstEdgePoints.size() - j),
//                                                    borderIndexes.at(0),
//                                                    width ) );
        splinePoints.push_back( cFirstEdgePoints.at( cFirstEdgePoints.size() - j ) );
    }

    /* i = 0 already processed */
    for (unsigned int i = 1; i < borderIndexes.size(); i++)
    {
        //node = graph->getNodeIndex(borderIndexes.at(i));
        cell = this->_cells[borderIndexes.at(i)];
        cellPoints = getNormalizedPointVector(cell->getPoints(), borderIndexes.at(i), _width);

        cEdgePoints = this->extractCellEdges(graph, cellPoints, lastPoint, borderIndexes.at(i), _width);

//        lastPoint = getNormalizedPoint(cEdgePoints.at(cEdgePoints.size() - 1), borderIndexes.at(i),
//                                       width);
        lastPoint = cEdgePoints.at(cEdgePoints.size() - 1);

        for (int j = cEdgePoints.size(); j > 0; j--)
        {
//            splinePoints.push_back( getNormalizedPoint( cEdgePoints.at( cEdgePoints.size() - j),
//                                    borderIndexes.at(i),
//                                    width) );
            splinePoints.push_back( cEdgePoints.at( cEdgePoints.size() - j) );
        }
    }

    /* Push back the other points from the first cell that haven't been processed yet */

    //node = graph->getNodeIndex(borderIndexes.at(0));
    cell = this->_cells[borderIndexes.at(0)];
    cellPoints = getNormalizedPointVector(cell->getPoints(), borderIndexes.at(0), _width);

    cEdgePoints = this->extractCellEdges(graph, cellPoints, lastPoint, borderIndexes.at(0), _width);

    /* This loop stops before reaches the first cell points already taken */
    for (unsigned int j = cEdgePoints.size(); j >= cFirstEdgePoints.size() ; j--)
    {
//        splinePoints.push_back( getNormalizedPoint( cEdgePoints.at( cEdgePoints.size() - j),
//                                borderIndexes.at(0),
//                                width) );
        splinePoints.push_back( cEdgePoints.at( cEdgePoints.size() - j) );
    }

    cEdgePoints.clear();
    cFirstEdgePoints.clear();
    cellPoints.clear();

    return splinePoints;
}



vector<vector<Point2D> > Diagram::extractBorderPoints(SimilarityGraph* graph, vector<char*> *colorList){
    /* List of border nodes for each connected components  */
    vector<vector<int> > CClist;

    /* List of already processed nodes */
    vector<int> listed;

    /* List of nodes that is not an output nor need to be recomputed  */
    vector<int> discarded;

    /* List of spline control points */
    vector<vector<Point2D> > splineList;

    /* First node index of a connected component */
    int firstNodeCC;

    int index;
    char node;

    /* How much nodes in the current connected component
       Used to traceback when computation hits a non valid node */
    int numCCNodes;

    /* Index of current connected component */
    int cc_index = 0;

    vector<int>::iterator it;
    vector<int>::iterator it_discarded;

    /* Loop throught nodes */
    for(int i = 0 ; i < _height ; i++ ) {
        for(int j = 0 ; j < _width ; j++ ) {

            index = i*_width + j;
            node = graph->getGraphBuffer()[index].value;

            /* *DO NOT* process if node is internal node */
            if (graph->isInternalNode(node)) continue;

            if (graph->isIslandNode(node))
            {
                vector<Point2D> islandSpline;

                islandSpline.push_back(getNormalizedPoint(getCells()[index]->getPoints()[0], index, _width));
                for (int t = 1; t < getCells()[index]->getN_points(); t++)
                {
                    islandSpline.push_back(getNormalizedPoint(getCells()[index]->getPoints()[t], index, _width));
                    islandSpline.push_back(getNormalizedPoint(getCells()[index]->getPoints()[t], index, _width));
                }
                islandSpline.push_back(getNormalizedPoint(getCells()[index]->getPoints()[getCells()[index]->getN_points()-1], index, _width));

                splineList.push_back(islandSpline);
                /* Spline color (from first node processed) */
                colorList->push_back(graph->getGraphBuffer()[i*_width + j].color);
                continue;
            }

            it = find(listed.begin(), listed.end(), index);

            /* Test if it is already stored for some connected component */
            if (it == listed.end())
            {
                firstNodeCC = index;
                /* Node is flagged as processed */
                //listed.push_back(index);

                int edge = graph->getFirstLink(node);

                /* Walking throught the border must end the loop on this edge for the firstNodeCC */
                int edgeOfArrival = graph->nextEdgeCounterClockwise(index, edge);
                //&& (connected_edge(edge) != edgeOfArrival)
                numCCNodes = 1;

                /* Listed only for this loop */
                vector<int> currentListed;
                currentListed.push_back(index);

                /* While the current index is not the first index (while the loop is not completed) */
                while  ( (c_neighbor_index(index, edge, _width) != firstNodeCC) || (connected_edge(edge) != edgeOfArrival) )
                {
                    graph->nextNodeClockwise(&index, &edge);
                    node = graph->getGraphBuffer()[index].value;

                    it = find(listed.begin(), listed.end(), index);
                    it_discarded = find(discarded.begin(), discarded.end(), index);

                    /* If is valence 2 node can be repeated */
                    //if (  !( !(it == listed.end()) && ( ( graph->isValence2(node)) || ( graph->isValence3(node))) )  )
                    //{
                        /* Walking clockwise do not work in internal loops cause
                           the next node will be an internal node or already processed (listed),
                           so it is treated as a exception and the node is not processed

                           The second line test if the loop ends in the arrival edge for the first node */
                        if (graph->isInternalNode(node) || !(it == listed.end()) || !(it_discarded == discarded.end()) )

                                //||
                                //((c_neighbor_index(index, edge, width) == firstNodeCC) && (connected_edge(edge) != edgeOfArrival)) )
                        {
                            while (numCCNodes > 0)
                            {
                                //discarded.push_back(listed.at(listed.size() - 1));
                                discarded.push_back(currentListed.at(currentListed.size() - 1));
                                //listed.pop_back();
                                numCCNodes--;
                            }
                            currentListed.clear();
                            break;
                        }
                    //}

                    //listed.push_back(index);
                    currentListed.push_back(index);
                    numCCNodes++;
                }
                /* If numCCNodes > 0 then a connected component was processed
                   so put the node's indexes on the list of nodes for each connected components (CClist) */
                if (numCCNodes > 0)
                {
                    vector<int> CCindexes;
                    vector<Point2D> spline;
                    while (numCCNodes > 0)
                    {
                        //CCindexes.push_back(listed.at(listed.size() - numCCNodes));
                        CCindexes.push_back(currentListed.at(currentListed.size() - numCCNodes));
                        listed.push_back(currentListed.at(currentListed.size() - numCCNodes));
                        numCCNodes--;
                    }

                    /* Extracts the points of the cell in right order to build the spline */
                    spline = this->extractSplinePoints(graph, CCindexes);
                    /* Put control points in the list of splines */
                    splineList.push_back(spline);
                    CCindexes.clear();

                    /* Spline color (from first node processed) */
                    colorList->push_back(graph->getGraphBuffer()[i*_width + j].color);

                    //CClist.push_back(CCindexes);
                    cc_index++;
                }
            }

        }
    }
    listed.clear();
    discarded.clear();
    return splineList;
}



void Diagram::optimize()
{
    DiagramCell *cell_1, *cell_2, *cell_3, *cell_4;
    Point *p1, *p2, *p3, *p4;
    Point p1_, p2_, p3_, p4_;

    for(int i = 0 ; i < (_height - 1) ; i++ )
    {
        for(int j = 0 ; j < (_width - 1) ; j++ )
        {
            cell_1 = getCells()[ (i+1) * _width + j   ];     //[j  ][i+1]
            cell_2 = getCells()[ (i+1) * _width + j+1 ];     //[j+1][i+1]
            cell_3 = getCells()[  i    * _width + j   ];     //[j  ][i  ]
            cell_4 = getCells()[  i    * _width + j+1 ];     //[j+1][i  ]

            if (cell_1->isDiagonalCell() || cell_2->isDiagonalCell() ||
                    cell_3->isDiagonalCell() || cell_4->isDiagonalCell())
                continue;

            if (cell_1->isDiagonalBorderCell() || cell_2->isDiagonalBorderCell() ||
                    cell_3->isDiagonalBorderCell() || cell_4->isDiagonalBorderCell())
                continue;

            p1 = cell_1->getBottomRightPoint();
            p2 = cell_2->getBottomLeftPoint();
            p3 = cell_3->getUpperRightPoint();
            p4 = cell_4->getUpperLeftPoint();

            p1_ = *p1;
            p2_ = *p2;
            p3_ = *p3;
            p4_ = *p4;


            p3_.y = p3_.y - 1;

            p4_.y = p4_.y - 1;

            if ( !(p1_ == p3_) )
            {
                p1->x = 1.0;
                p1->y = 0.0;

                p3->x = 1.0;
                p3->y = 1.0;
            }

            if ( !(p2_ == p4_) )
            {
                p2->x = 0.0;
                p2->y = 0.0;

                p4->x = 0.0;
                p4->y = 1.0;
            }

        }
    }
}



bool Diagram::detectTJunction(int index, int width)
{
    //return false;
    _TJunctionPoints.clear();
    bool hasPoint;
    Point out;

    int j = (index % width);
    int i = (int) (index / width);

    Point *p1, *p2, *p3, *p4;

    if ( (i == 0) || (j == 0) )
    {
        //*detected = false;
        return false;
    }

    DiagramCell *cell_1, *cell_2, *cell_3, *cell_4;

    //Point *p1, *p2, *p3, *p4;
    Point p1_, p2_, p3_, p4_;

    /* Border exception */
    if ( (i >= (_height - 1)) || (j >= (width - 1)) )
    {
        hasPoint = false;
    }
    else
    {

        cell_1 = getCells()[ (i+1) * width + j   ];     //[j  ][i+1]
        cell_2 = getCells()[ (i+1) * width + j+1 ];     //[j+1][i+1]
        cell_3 = getCells()[  i    * width + j   ];     //[j  ][i  ]
        cell_4 = getCells()[  i    * width + j+1 ];     //[j+1][i  ]

        int nColors = compareColors(cell_1->getColor(), cell_2->getColor(), cell_3->getColor(), cell_4->getColor());


        /* Do not process if there is one or two colors */
        if (nColors < 3)
            hasPoint = false;
        else
        {
            p1 = cell_1->getBottomRightPoint();
            p2 = cell_2->getBottomLeftPoint();
            p3 = cell_3->getUpperRightPoint();
            p4 = cell_4->getUpperLeftPoint();

            p1_ = *p1;
            p2_ = *p2;
            p3_ = *p3;
            p4_ = *p4;


            p3_.y = p3_.y - 1;

            p2_.x = p2_.x + 1;

            p4_.y = p4_.y - 1;
            p4_.x = p4_.x + 1;

            if ( ( p1_ == p2_ ) && ( p2_ == p3_ ) && (p3_  == p4_ ) )
            {
                out = *p3;
                hasPoint = true;
            }
            else
                hasPoint = false;

            if (cell_1->isDiagonalCell() || cell_2->isDiagonalCell() ||
                    cell_3->isDiagonalCell() || cell_4->isDiagonalCell())
                hasPoint = false;

            if (cell_1->isDiagonalBorderCell() || cell_2->isDiagonalBorderCell() ||
                    cell_3->isDiagonalBorderCell() || cell_4->isDiagonalBorderCell())
                hasPoint = false;
        }
    }

    if (hasPoint)
        _TJunctionPoints.push_back(out);

    j--;

    /* Border exception */
    if ( (i >= (_height - 1)) || (j >= (width - 1)) )
    {
        hasPoint = false;
    }
    else
    {

        cell_1 = getCells()[ (i+1) * width + j   ];     //[j  ][i+1]
        cell_2 = getCells()[ (i+1) * width + j+1 ];     //[j+1][i+1]
        cell_3 = getCells()[  i    * width + j   ];     //[j  ][i  ]
        cell_4 = getCells()[  i    * width + j+1 ];     //[j+1][i  ]

        int nColors = compareColors(cell_1->getColor(), cell_2->getColor(), cell_3->getColor(), cell_4->getColor());


        /* Do not process if there is one or two colors */
        if (nColors < 3)
            hasPoint = false;
        else
        {

            p1 = cell_1->getBottomRightPoint();
            p2 = cell_2->getBottomLeftPoint();
            p3 = cell_3->getUpperRightPoint();
            p4 = cell_4->getUpperLeftPoint();

            p1_ = *p1;
            p2_ = *p2;
            p3_ = *p3;
            p4_ = *p4;


            p3_.y = p3_.y - 1;

            p2_.x = p2_.x + 1;

            p4_.y = p4_.y - 1;
            p4_.x = p4_.x + 1;

            if ( ( p1_ == p2_ ) && ( p2_ == p3_ ) && (p3_  == p4_ ) )
            {
                out = *p4;
                hasPoint = true;
            }
            else
                hasPoint = false;

            if (cell_1->isDiagonalCell() || cell_2->isDiagonalCell() ||
                    cell_3->isDiagonalCell() || cell_4->isDiagonalCell())
                hasPoint = false;

            if (cell_1->isDiagonalBorderCell() || cell_2->isDiagonalBorderCell() ||
                    cell_3->isDiagonalBorderCell() || cell_4->isDiagonalBorderCell())
                hasPoint = false;
        }
    }

    if (hasPoint)
        _TJunctionPoints.push_back(out);

    j++; i--;

    /* Border exception */
    if ( (i >= (_height - 1)) || (j >= (width - 1)) )
    {
        hasPoint = false;
    }
    else
    {

        cell_1 = getCells()[ (i+1) * width + j   ];     //[j  ][i+1]
        cell_2 = getCells()[ (i+1) * width + j+1 ];     //[j+1][i+1]
        cell_3 = getCells()[  i    * width + j   ];     //[j  ][i  ]
        cell_4 = getCells()[  i    * width + j+1 ];     //[j+1][i  ]

        int nColors = compareColors(cell_1->getColor(), cell_2->getColor(), cell_3->getColor(), cell_4->getColor());


        /* Do not process if there is one or two colors */
        if (nColors < 3)
            hasPoint = false;
        else
        {

            p1 = cell_1->getBottomRightPoint();
            p2 = cell_2->getBottomLeftPoint();
            p3 = cell_3->getUpperRightPoint();
            p4 = cell_4->getUpperLeftPoint();

            p1_ = *p1;
            p2_ = *p2;
            p3_ = *p3;
            p4_ = *p4;


            p3_.y = p3_.y - 1;

            p2_.x = p2_.x + 1;

            p4_.y = p4_.y - 1;
            p4_.x = p4_.x + 1;

            if ( ( p1_ == p2_ ) && ( p2_ == p3_ ) && (p3_  == p4_ ) )
            {
                out = *p1;
                hasPoint = true;
            }
            else
                hasPoint = false;

            if (cell_1->isDiagonalCell() || cell_2->isDiagonalCell() ||
                    cell_3->isDiagonalCell() || cell_4->isDiagonalCell())
                hasPoint = false;

            if (cell_1->isDiagonalBorderCell() || cell_2->isDiagonalBorderCell() ||
                    cell_3->isDiagonalBorderCell() || cell_4->isDiagonalBorderCell())
                hasPoint = false;
        }
    }

    if (hasPoint)
        _TJunctionPoints.push_back(out);

    j--;

    /* Border exception */
    if ( (i >= (_height - 1)) || (j >= (width - 1)) )
    {
        hasPoint = false;
    }
    else
    {

        cell_1 = getCells()[ (i+1) * width + j   ];     //[j  ][i+1]
        cell_2 = getCells()[ (i+1) * width + j+1 ];     //[j+1][i+1]
        cell_3 = getCells()[  i    * width + j   ];     //[j  ][i  ]
        cell_4 = getCells()[  i    * width + j+1 ];     //[j+1][i  ]

        int nColors = compareColors(cell_1->getColor(), cell_2->getColor(), cell_3->getColor(), cell_4->getColor());


        /* Do not process if there is one or two colors */
        if (nColors < 3)
            hasPoint = false;
        else
        {

            p1 = cell_1->getBottomRightPoint();
            p2 = cell_2->getBottomLeftPoint();
            p3 = cell_3->getUpperRightPoint();
            p4 = cell_4->getUpperLeftPoint();

            p1_ = *p1;
            p2_ = *p2;
            p3_ = *p3;
            p4_ = *p4;


            p3_.y = p3_.y - 1;

            p2_.x = p2_.x + 1;

            p4_.y = p4_.y - 1;
            p4_.x = p4_.x + 1;

            if ( ( p1_ == p2_ ) && ( p2_ == p3_ ) && (p3_  == p4_ ) )
            {
                out = *p2;
                hasPoint = true;
            }
            else
                hasPoint = false;

            if (cell_1->isDiagonalCell() || cell_2->isDiagonalCell() ||
                    cell_3->isDiagonalCell() || cell_4->isDiagonalCell())
                hasPoint = false;

            if (cell_1->isDiagonalBorderCell() || cell_2->isDiagonalBorderCell() ||
                    cell_3->isDiagonalBorderCell() || cell_4->isDiagonalBorderCell())
                hasPoint = false;
        }
    }

    if (hasPoint)
        _TJunctionPoints.push_back(out);


    if (_TJunctionPoints.size() > 0)
        return true;
    else
        return false;

}



bool Diagram::detectTPoint(int j, int i, int width, Point *out)
{
//    int j = (index % width);
//    int i = (int) (index / width);

    /* Border exception */
    if ( (i == (_height - 1)) || (j == (width - 1)) )
        return false;

//    char *c1, *c2, *c3, *c4;

//    c1 = graph->getNodeIndex((i+1) * width + j  ).color;
//    c2 = graph->getNodeIndex((i+1) * width + j+1).color;
//    c3 = graph->getNodeIndex( i    * width + j  ).color;
//    c4 = graph->getNodeIndex( i    * width + j+1).color;

//    int nColors = compareColors(c1, c2, c3, c4);

    DiagramCell *cell_1, *cell_2, *cell_3, *cell_4;

    cell_1 = getCells()[ (i+1) * width + j   ];     //[j  ][i+1]
    cell_2 = getCells()[ (i+1) * width + j+1 ];     //[j+1][i+1]
    cell_3 = getCells()[  i    * width + j   ];     //[j  ][i  ]
    cell_4 = getCells()[  i    * width + j+1 ];     //[j+1][i  ]

    int nColors = compareColors(cell_1->getColor(), cell_2->getColor(), cell_3->getColor(), cell_4->getColor());


    /* Do not process if there is one or two colors */
    if (nColors < 3) return false;

    Point *p1, *p2, *p3, *p4;
    Point p1_, p2_, p3_, p4_;

//    cell_1 = getCells()[ (i+1) * width + j   ];     //[j  ][i+1]
//    cell_2 = getCells()[ (i+1) * width + j+1 ];     //[j+1][i+1]
//    cell_3 = getCells()[  i    * width + j   ];     //[j  ][i  ]
//    cell_4 = getCells()[  i    * width + j+1 ];     //[j+1][i  ]

    if (cell_1->isDiagonalCell() || cell_2->isDiagonalCell() ||
            cell_3->isDiagonalCell() || cell_4->isDiagonalCell())
        return false;

    if (cell_1->isDiagonalBorderCell() || cell_2->isDiagonalBorderCell() ||
            cell_3->isDiagonalBorderCell() || cell_4->isDiagonalBorderCell())
        return false;

    p1 = cell_1->getBottomRightPoint();
    p2 = cell_2->getBottomLeftPoint();
    p3 = cell_3->getUpperRightPoint();
    p4 = cell_4->getUpperLeftPoint();

    p1_ = *p1;
    p2_ = *p2;
    p3_ = *p3;
    p4_ = *p4;


    p3_.y = p3_.y - 1;

    p2_.x = p2_.x + 1;

    p4_.y = p4_.y - 1;
    p4_.x = p4_.x + 1;

    if ( ( p1_ == p2_ ) && ( p2_ == p3_ ) && (p3_  == p4_ ) )
    {
        *out = p1_;
        return true;
    }
    else
        return false;

}



void Diagram::setCells ( DiagramCell** new_var )   {
    _cells = new_var;
}



DiagramCell** Diagram::getCells ( )   {
    return _cells;
}

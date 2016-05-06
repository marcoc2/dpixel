/*
 * 2016 - Marco Aurélio G. da Silva ( marcoc2@gmail.com )
 */

#ifndef SIMILARITYGRAPH_H
#define SIMILARITYGRAPH_H

#include <vector>

class Image;

class SimilarityGraph
{
public:

    struct Node
    {
        char value;
        char color[ 3 ];
        Node( char value, char* color )
        {
            this->value = value;
            this->color[ 0 ] = color[ 0 ];
            this->color[ 1 ] = color[ 1 ];
            this->color[ 2 ] = color[ 2 ];
        }


        Node()
        {
        }
    };

    struct Point2D
    {
        float x;
        float y;
        Point2D() :
            x( 0 ),
            y( 0 )
        {
        }


        Point2D( float _x, float _y ) :
            x( _x ),
            y( _y )
        {
        }
    };

    SimilarityGraph( Image* inputImage );
    ~SimilarityGraph();

    void createGraph();
    std::vector< Point2D > getNodeLines( int x, int y );
    unsigned int getWidth();
    unsigned int getHeight();

    /**
     * @return Node
     * @param  index
     */
    Node getNodeIndex( int index );

    Node getNode( int i, int j );

    /**
    * Return node array
    * One node for each pixel of the image
    * @return graph as node array
    */
    Node* getGraphBuffer();

    /**
     * @brief isInternalNode                Test if is a internal node
     *                                      This is a pattern of a internal node:
     *                                                 |
     *                                               __N__
     *                                                 |
     *                                      It is 0101 1010 bit order which is "90" unsigned int
     * @param value                         Node links as char value
     * @return                              TRUE if is internal node
     *                                      FALSE if is not
     */
    bool isInternalNode( char value );

    /**
     * @brief isIslandNode                  Test if is a island node
     *                                      A island node has no links
     *                                      It is 0000 0000 bit order which is "0" unsigned int
     * @param value                         Nodes link as char value
     * @return                              TRUE if is island node
     *                                      FALSE if is not
     */
    bool isIslandNode( char value );

    /**
     * @brief getFirstLink                  Get first active link (bit)
     * @param value                         Node links as char value
     * @return                              First active link (bit)
     */
    int getFirstLink( char value );

    /**
     * @brief nextEdgeCounterClockwise          Return the counter clockwise edge starting from edge
     * @param index                             Node index
     * @param edge                              Edge index (0 - 7)
     * @return                                  First counter clockwise edge index
     */
    int nextEdgeCounterClockwise( int index, int edge );

    /**
     * @brief nextNodeClockwise             Return index and oposite edge for linked node
     *                                      walking clockwise, starting from the linked edge
     *                                      of the next edge
     * @param[in,out] index                 Node index
     * @param[in,out] edge                  Edge index
     */
    void nextNodeClockwise( int* index, int* edge );

    /**
     * @brief isValence1                    Verify if node is valence 1
     * @param value                         Node links as char value (RTK pass entire Graph::Node)
     * @return                              TRUE if node is valence 1
     *                                      FALSE if is not
     */
    bool isValence1( char value );

    /**
     * @brief getNextNodeInLine Return next node in line for 0/90/180/270 degrees cases only
     * @param index return param
     * @return if there is a node in line
     */
    bool getNextNodeInLine( int& index );

    bool isStartNodeOnHorizontal( int index );

private:

    /**
     * @brief crossCheck                    Change edges according to heuristics
     */
    void crossCheck();

    /**
     * @brief calcVal2PathSize              Calculate the size of a valence-2 path starting prom index
     * @param index                         Index of current node
     * @param edge                          Index of current edge (0 - 7)
     * @param result                        Sum of current size path
     * @return                              Sum of the size path (stop at size = 30)
     */
    int calcVal2PathSize( int index, int edge, int result );

    /**
     * @brief processHeuristics             Process heuristics to improve graph result
     * @param j                             Coordinate j - x axis
     * @param i                             Coordinate i - y axis
     */
    void processHeuristics( int j, int i );

    /**
     * @brief checkValence2Edge             Verify if path is valence 2
     * @param index_1                       First node index
     * @param index_2                       Second node index
     * @param edge_1                        First edge index
     * @param edge_2                        Second edge index
     * @return                              TRUE if path is valence 2 or FALSE if is not
     */
    bool checkValence2Edge( int index_1, int index_2, int edge_1, int edge_2 );

    /**
     * @brief checkValence2Vertex           RTK Check if there is another
     * @param index_1                       First node index
     * @param edge_1                        First edge index
     * @return                              TRUE if there isnt another edge (bit) active, FALSE if there is
     */
    bool checkValence2Vertex( int index_1, int edge_1 );

    /**
     * @brief findValence2Path              Find paths of valence 2.
     *                                      It will loop throught paths and store their nodes for each path
     */
    void findValence2Path();

    /**
     * @brief getAdjacentNodeByEdge         Get node linked by the a determined edge
     * @param index                         Node index
     * @param edge                          Edge index (0 - 7)
     * @return                              Node linked by edge
     */
    Node getAdjacentNodeByEdge( int index, int edge );

    /**
     * @brief isValence1or2                 Verify if node is valence 1 or 2
     * @param value                         Node links as char value (RTK pass entire Graph::Node)
     * @return                              TRUE if node is valence 1 or 2
     *                                      FALSE if is not
     */
    bool isValence1or2( char value );

    /**
     * @brief isValence2                    Verify if node is valence 2
     * @param value                         Node links as char value (RTK pass entire Graph::Node)
     * @return                              TRUE if node is valence 2
     *                                      FALSE if is not
     */
    bool isValence2( char value );

    /**
     * @brief nextNodeVal2                  Return index and oposite edge for linked node (VALENCE 2)
     * @param index                         Node index
     * @param edge                          Edge index (0 - 7)
     */
    void nextNodeVal2( int* index, int* edge );

    int diff( int w, int h, int pos );
    unsigned int DATAtoINT( unsigned int red,
                            unsigned int green,
                            unsigned int blue );
    void tableInit();

    Node* _graph;
    Image* _inputImage;

    /* RGB to YUV lookup table */
    int RGBtoYUV[ 16777216 ];
    unsigned int YUV1;
    unsigned int YUV2;
};

#endif // SIMILARITYGRAPH_H

#include "SimilarityGraph.h"
#include "Image.h"
#include "Depixel/Vector/Utils.h"

/** Check bit value */
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

/** Clean bit then set */
#define SET_BIT(var,pos,data) var &= ~(1 << pos); var |= data << pos

#define Ymask 0x00FF0000
#define Umask 0x0000FF00
#define Vmask 0x000000FF
/* Original trY  */
#define trY   0x00300000
/* Better value when not using Splines with difusion (Empirical value) */
//#define trY   0x00050000
#define trU   0x00000700
#define trV   0x00000006

SimilarityGraph::SimilarityGraph( Image *inputImage ) :
    _graph( 0 ),
    _inputImage( inputImage )
{
    tableInit();
}



SimilarityGraph::~SimilarityGraph()
{
}


void SimilarityGraph::createGraph()
{
    int width = _inputImage->getWidth();
    int height = _inputImage->getHeight();

    _graph = new Node[ width * height ];

    for(int i = 0; i < height ; i++ ) {
        for(int j = 0 ; j < width ; j++ ) {
            /* verificar se é getHeight ou getWidth na linha a baixo */
            int index = (i*width + j);

            /* Loop through eight neighbor */
            for(int e = 0; e < 8; e++)
            {
                if (diff(j ,i, e))
                {
                    SET_BIT( _graph[index].value, e, 0 );
                }
                else
                {
                    SET_BIT( _graph[index].value, e, 1 );
                }
            }

            _graph[index].color[0] = (char) _inputImage->getPixel( j, i ).red;
            _graph[index].color[1] = (char) _inputImage->getPixel( j, i ).green;
            _graph[index].color[2] = (char) _inputImage->getPixel( j, i ).blue;
        }
    }

    crossCheck();
}



void SimilarityGraph::crossCheck()
{
    unsigned int height = getHeight();
    unsigned int width = getWidth();

    for(int i = 0 ; i < height ; i++ ) {
        for(int j = 0 ; j < width ; j++ ) {

            /*    i2 --- i4
                  |      |
                  i1 --- i3    */

            int i1 = (i*width + j);
            int i2 = ((i+1)*width + j);
            int i3 = (i*width + j+1);
            int i4 = ((i+1)*width + j+1);

            // Checa se blocos 2x2 estão totalmente conectados

            /* Checa se blocos 2x2 estão totalmente conectados */

            /*  3--- i2 ---(4)   (3)--- i4 ---4
                    / | \              / | \
                   /  |  \            /  |  \
                  5  (6)  7          5  (6)  7


                  0  (1)   2         0  (1)   2
                   \  |   /           \  |   /
                    \ |  /             \ |  /
                 3--- i1 ---(4)   (3)--- i3 ---4         */

            if ( (CHECK_BIT(_graph[i1].value,1)) && (CHECK_BIT(_graph[i1].value,4)) &&
                 (CHECK_BIT(_graph[i2].value,6)) && (CHECK_BIT(_graph[i2].value,4)) &&
                 (CHECK_BIT(_graph[i3].value,3)) && (CHECK_BIT(_graph[i3].value,1)) &&
                 (CHECK_BIT(_graph[i4].value,6)) && (CHECK_BIT(_graph[i4].value,3)) )
            {
                //cvCircle( graph_img, cvPoint(n_j+half_sg,n_i+half_sg), 3, CV_RGB( 0,  0, 0 ), CV_FILLED, 8, 0 );
                //remove arestas cruzadas
                SET_BIT(_graph[i1].value,2,0);
                SET_BIT(_graph[i2].value,7,0);
                SET_BIT(_graph[i3].value,0,0);
                SET_BIT(_graph[i4].value,5,0);
            }
            // Checa pelas arestas cruzadas

            /*  3--- i2 ---4        3--- i4 ---4
                    / | \              / | \
                   /  |  \            /  |  \
                  5   6   (7)       (5)  6   7


                  0   1   (2)       (0)  1    2
                   \  |   /           \  |   /
                    \ |  /             \ |  /
                 3--- i1 ---4       3--- i3 ---4         */

            if ( (CHECK_BIT(_graph[i1].value,2)) &&
                 (CHECK_BIT(_graph[i2].value,7)) &&
                 (CHECK_BIT(_graph[i3].value,0)) &&
                 (CHECK_BIT(_graph[i4].value,5)) )
            {
                this->processHeuristics(j, i);
            }
        }
    }
}



void SimilarityGraph::processHeuristics(int j, int i)
{
    unsigned int width = getWidth();

    int seg_size = 0;
    int seg_size2 = 0;

    int i1 = (i*width + j);
    int i2 = ((i+1)*width + j);
    int i3 = (i*width + j+1);
    int i4 = ((i+1)*width + j+1);

    // Both edges are
    if ((this->checkValence2Edge(i1, i4, 2, 5)) && (this->checkValence2Edge(i3, i2, 0, 7)))
    {

    } else {
        /* Just i1-i4 edge is valence 2 */
        /*  i2  /i4
               /
            i1/  i3  */
        if (this->checkValence2Edge(i1, i4, 2, 5)){
            /* Remove i2-i3 edge */
            SET_BIT(_graph[i2].value,7,0);
            SET_BIT(_graph[i3].value,0,0);
        } else
            /* Just i2-i3 edge is valence 2 */
            /*  i2\  i4
                   \
                i1  \i3  */
            if(this->checkValence2Edge(i3, i2, 0, 7)){
                /* Remove i1-i4 edge */
                SET_BIT(_graph[i1].value,2,0);
                SET_BIT(_graph[i4].value,5,0);
            } else
                if( ( (this->checkValence2Vertex(i1, 2)) || (this->checkValence2Vertex(i4, 5)) ) &&
                        (!(this->checkValence2Vertex(i3, 0))) && (!(this->checkValence2Vertex(i2, 7))) ){
                    SET_BIT(_graph[i2].value,7,0);
                    SET_BIT(_graph[i3].value,0,0);
                } else
                    if( (this->checkValence2Vertex(i3, 0)) || (this->checkValence2Vertex(i2, 7)) &&
                            (!(this->checkValence2Vertex(i1, 2))) && (!(this->checkValence2Vertex(i4, 5))) ){
                        SET_BIT(_graph[i1].value,2,0);
                        SET_BIT(_graph[i4].value,5,0);
                    } else {

            // Calcula tamanho partindo da aresta 2 de i1 e partindo da aresta 5 de i4. Depois soma

            //usleep(5000 * 1000);
            seg_size = this->calcVal2PathSize(i1,2, 0);

            seg_size = seg_size + this->calcVal2PathSize(i4, 5, 0);
            //cout << "i1 seg_size total: " << seg_size << endl;


            // Calcula tamanho partindo da aresta 0 de i3 e partindo de i2 da aresta 0. Depois soma

            //usleep(5000 * 1000);
            seg_size2 = this->calcVal2PathSize(i3,0, 0);

            seg_size2 = seg_size2 + this->calcVal2PathSize(i2, 7, 0);
            //cout << "  i3 seg_size total: " << seg_size2 << endl;

            // compara tamanho de cada curva e retira aresta da menor
            if (seg_size2 < seg_size)
            {
                //cout << "seg_size maior retira aresta 7 e 0" << endl;
                SET_BIT(_graph[i2].value,7,0);
                SET_BIT(_graph[i3].value,0,0);
            } else
            {
                //cout << "seg_size2 maior retira aresta 2 e 5" << endl;
                SET_BIT(_graph[i1].value,2,0);
                SET_BIT(_graph[i4].value,5,0);
            }
        }
    }
}



bool SimilarityGraph::checkValence2Edge(int index_1, int index_2, int edge_1, int edge_2)
{
    int k_n;     // last conected edge
    int sum_1 = 0;
    int sum_2 = 0; // soma de arestas conectadas além da 2
    for (int k = (edge_1 + 1); k < (edge_1 + 8); k++)
    {
        if (CHECK_BIT(_graph[index_1].value,k%8))
        {
            sum_1++;
            k_n = k;
        }
    }
    for (int k = (edge_2 + 1); k < (edge_2 + 8); k++)
    {
        if (CHECK_BIT(_graph[index_2].value,k%8))
        {
            sum_2++;
            k_n = k;
        }
    }

    if ((sum_1 == 1) && (sum_2 == 1)) return true;
    else return false;
}



bool SimilarityGraph::checkValence2Vertex(int index_1, int edge_1)
{
    int k_n;     // last conected edge
    int sum = 0; // soma de arestas conectadas além da 2
    for (int k = (edge_1 + 1); k < (edge_1 + 8); k++)
    {
        if (CHECK_BIT(_graph[index_1].value,k%8))
        {
            sum++;
            k_n = k;
        }
    }
    //cout << sum << endl;
    if (sum == 0) return true;
    else return false;
}



void SimilarityGraph::findValence2Path(){
    std::vector<std::vector<int> > paths;
    std::vector<int > listed;
    int index;
    char node;
    int cc_index = 0;
    std::vector<int>::iterator it;

    unsigned int height = getHeight();
    unsigned int width = getWidth();

    /* Loop throught nodes */
    for(unsigned int i = 0 ; i < height ; i++ ) {
        for(unsigned int j = 0 ; j < width ; j++ ) {

            index = i*width + j;
            node = _graph[i*width + j].value;
            it = std::find(listed.begin(), listed.end(), index);


            /* Test if node is valence 1 or 2 and if it is already stored for some connected component */
            if ( ( isValence1or2(node) ) && (it == listed.end()) )
            {

                listed.push_back(index);
                std::vector<int> connected_comp;
                paths.push_back( connected_comp );

                /* Test if node is valence 2 */
                //if ( isValence2(node) )
                if (( isValence2(node) ) && (index == 5) ) // specific teste for index == 5
                {

                    int edge = getFirstLink(node);
                    it = std::find(listed.begin(), listed.end(), c_neighbor_index(index, edge, width));


                    /* Loop throught cyclical path */
                    while ( ( isValence2( getAdjacentNodeByEdge(index, edge).value )) && (it == listed.end()) )
                    {
                        paths[cc_index].push_back(index);
                        listed.push_back(index);
                        /* ! index and edge will be modified within function ! */
                        nextNodeVal2(&index, &edge);
                        node = _graph[index].value;

                        it = std::find(listed.begin(), listed.end(), index);
                    }

                }
                cc_index++;
            }
        }
    }
}



int SimilarityGraph::diff( int w, int h, int pos )
{
    int result;

    /* Pixels colors in RGB to be converted */
    Pixel inPixel;
    Pixel outPixel;

    /* Pixels colors in YUV space to be compared */
    u_int inPixelYUV;
    u_int outPixelYUV;

    /* Height and Width position of pixel_dst */
    int n_h = - 1;
    int n_w = - 1;

    inPixel = _inputImage->getPixel( w, h );

    inPixelYUV = DATAtoINT( (char) inPixel.red, (char) inPixel.green, (char) inPixel.blue );

    switch (pos){
    case 0:
        n_w = (w-1); n_h = (h+1);
        break;
    case 1:
        n_w = (w); n_h = (h+1);
        break;
    case 2:
        n_w = (w+1); n_h = (h+1);
        break;
    case 3:
        n_w = (w-1); n_h = (h);
        break;
    case 4:
        n_w = (w+1); n_h = (h);
        break;
    case 5:
        n_w = (w-1); n_h = (h-1);
        break;
    case 6:
        n_w = (w); n_h = (h-1);
        break;
    case 7:
        n_w = (w+1); n_h = (h-1);
        break;
    }

    /* Check if index is inside image boundaries */
    if (!((n_w < 0) || (n_h < 0) ||
            (n_w >= (int) _inputImage->getWidth()) ||
            (n_h >= (int) _inputImage->getHeight()) ))
    {
        outPixel = _inputImage->getPixel(n_w, n_h);

        outPixelYUV = DATAtoINT( (char) outPixel.red, (char) outPixel.green, (char) outPixel.blue );


        /* Mask against RGB_MASK to discard the alpha channel */
        YUV1 = RGBtoYUV[inPixelYUV];
        YUV2 = RGBtoYUV[outPixelYUV];

        result = ( ( abs((YUV1 & Ymask) - (YUV2 & Ymask)) > trY ) ||
                   ( abs((YUV1 & Umask) - (YUV2 & Umask)) > trU ) ||
                   ( abs((YUV1 & Vmask) - (YUV2 & Vmask)) > trV ) );

        return result;
    }
    else {return 1;}
}



unsigned int SimilarityGraph::DATAtoINT( unsigned int red, unsigned int green, unsigned int blue )
{

    unsigned int result;

    /* R_ = xxxx xxxx xxxx xxxx xxxx xxxx RRRR RRRR*/
    unsigned int R_ = (unsigned int) red;

    /* G_ = xxxx xxxx xxxx xxxx GGGG GGGG xxxx xxxx*/
    unsigned int G_ = (unsigned int) green << 8;

    /* B_ = xxxx xxxx BBBB BBBB xxxx xxxx rrrr rrrr*/
    unsigned int B_ = (unsigned int) blue << 16;
    //unsigned int A_ = 0x00FFFFFF;


    /* R_ = 0000 0000 0000 0000 0000 0000 RRRR RRRR*/
    R_ &= 0x000000FF;

    /* G_ = 0000 0000 0000 0000 GGGG GGGG 0000 0000*/
    G_ = (G_ & 0x0000FF00);

    /* B_ = 0000 0000 BBBB BBBB 0000 0000 0000 0000*/
    B_ = (B_ & 0x00FF0000);


    /* result = 0000 0000 BBBB BBBB GGGG GGGG RRRR RRRR*/
    result = ((R_|G_|B_));

    return result;
}



void SimilarityGraph::tableInit()
{
    int c, r, g, b, y, u, v;
    for (c = 0; c < 16777216; c++)
    {
        r = (c & 0x0000FF);
        g = (c & 0x00FF00) >> 8;
        b = (c & 0xFF0000) >> 16 ;

        y = (int)(0.299*r + 0.587*g + 0.114*b);
        u = (int)((b - y) * 0.492f);
        v = (int)((r - y) * 0.877f);


        RGBtoYUV[c] = (y << 16) + (u << 8) + v;
    }
}


std::vector<SimilarityGraph::Point2D> SimilarityGraph::getNodeLines( int x, int y )
{
    std::vector<Point2D> lines;
    int scale = 10;
    int half = scale / 2;
    int sceneX = x * scale;
    int sceneY = y * scale;
    int index = (y * _inputImage->getWidth() + x);

    if CHECK_BIT(_graph[index].value, 0)
    {
        lines.push_back( Point2D( sceneX - half, sceneY + half ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 1)
    {
        lines.push_back( Point2D( sceneX, sceneY + half ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 2)
    {
        lines.push_back( Point2D( sceneX + half, sceneY + half ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 3)
    {
        lines.push_back( Point2D( sceneX - half, sceneY ) )     ;
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 4)
    {
        lines.push_back( Point2D( sceneX + half, sceneY ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 5)
    {
        lines.push_back( Point2D( sceneX - half, sceneY - half ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 6)
    {
        lines.push_back( Point2D( sceneX, sceneY - half ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }
    if CHECK_BIT(_graph[index].value, 7)
    {
        lines.push_back( Point2D( sceneX + half, sceneY - half ) );
        lines.push_back( Point2D( sceneX, sceneY ) );
    }

    return lines;
}



unsigned int SimilarityGraph::getHeight()
{
    return _inputImage->getHeight();
}



unsigned int SimilarityGraph::getWidth()
{
    return _inputImage->getWidth();
}



SimilarityGraph::Node SimilarityGraph::getNodeIndex (int index )
{
    return _graph[index];
}


SimilarityGraph::Node SimilarityGraph::getNode( int i, int j )
{
    return _graph[ i * getWidth() + j ];
}


SimilarityGraph::Node* SimilarityGraph::getGraphBuffer()
{
    return _graph;
}



bool SimilarityGraph::isInternalNode(char value)
{
    if ( (unsigned int) value == 90)
        return true;
    else
        return false;
}



bool SimilarityGraph::isIslandNode(char value)
{
    if ( (unsigned int) value == 0)
        return true;
    else
        return false;
}



int SimilarityGraph::getFirstLink(char value)
{
    for(int e = 0; e<8; e++)
    {
        if ((bool) CHECK_BIT(value, getRealLinkIndex(e))){
            return getRealLinkIndex(e);
        }
    }
    /* Case not found */
    return -1;
}



int SimilarityGraph::nextEdgeCounterClockwise(int index, int edge)
{
    if (isValence1(_graph[index].value))
    {
        /* Only one edge so the number dont change */
        //edge = connected_edge(edge);
        return edge;
    } else
    {
        edge = getClockLinkIndex( edge);
        for (int e = (edge + 8 - 1); e >= (edge + 8 - 7); e--)
        {
            /* If bit is active (a link) and it is not *edge
               then it is the oposite link cause its valence 2 */
            if ( (bool) CHECK_BIT(_graph[index].value, getRealLinkIndex(e%8)) )
            {
                    edge = getRealLinkIndex(e%8);
                    break;
            }
        }
    }
    return edge;
}



void SimilarityGraph::nextNodeClockwise(int* index, int* edge)
{
    *index = c_neighbor_index(*index, *edge, getWidth());

    if (isValence1(_graph[*index].value))
    {
        /* Only one edge so the number dont change */
        *edge = connected_edge(*edge);
    } else
    {
        *edge = getClockLinkIndex( connected_edge(*edge));
        for (int e = (*edge + 1); e <= (*edge + 7); e++)
        {
            /* If bit is active (a link) and it is not *edge
               then it is the oposite link cause its valence 2 */
            if ( (bool) CHECK_BIT(_graph[*index].value, getRealLinkIndex(e%8)) )
            {
                    *edge = getRealLinkIndex(e%8);
                    break;
            }
        }
    }
}



bool SimilarityGraph::isValence1(char value)
{
    int count = 0;
    for(int e = 0; e<8; e++){
        if ((bool) CHECK_BIT(value, e)){
            count++;
        }
    }

    return (count == 1);
}



SimilarityGraph::Node SimilarityGraph::getAdjacentNodeByEdge(int index, int edge)
{
    unsigned int width = getWidth();
    switch(edge)
    {
        case 0: return _graph[index + width - 1];
        case 1: return _graph[index + width];
        case 2: return _graph[index + width + 1];
        case 3: return _graph[index - 1];
        case 4: return _graph[index + 1];
        case 5: return _graph[index - width - 1];
        case 6: return _graph[index - width];
        case 7: return _graph[index - width + 1];
    }
}



bool SimilarityGraph::isValence2(char value)
{
    int count = 0;
    for(int e = 0; e<8; e++){
        if ((bool) CHECK_BIT(value, e)){
            count++;
        }
    }

    return (count == 2);
}



bool SimilarityGraph::isValence1or2(char value)
{
    int count = 0;
    for(int e = 0; e<8; e++)
    {
        if ((bool) CHECK_BIT(value, e))
        {
            count++;
        }
    }

    return ((count <= 2) && (count > 0));
}



void SimilarityGraph::nextNodeVal2(int* index, int* edge)
{
    unsigned int width = getWidth();
    *index = c_neighbor_index(*index, *edge, width);
    *edge = connected_edge(*edge);

    for(int e = 0; e<8; e++)
    {
        /* If bit is active (a link) and it is not *edge
           then it is the oposite link cause its valence 2 */
        if ((bool) CHECK_BIT(_graph[*index].value, e))
        {
            if (*edge != e) {
                *edge = e;
                break;
            }
        }
    }
}



int SimilarityGraph::calcVal2PathSize(int index, int edge, int result)
{
    int k_n; // Outra aresta quando o segmento é de valência 2
    int sum = 0;
    //switch (side){
    //    case 0: {
            // a partir de edge + 1 (edge já é conexo)
             for (int k = (edge + 1); k < (edge + 7); k++)
             {
                 if (CHECK_BIT(_graph[index].value,k%8))
                 {
                     sum++;
                     k_n = k;
                 }
             }
             // Compare to a big number to not enter in loop
             if (result > 30) return result;
             if (sum == 1)
             {
                 result++;
                 calcVal2PathSize(c_neighbor_index(index, k_n%8, getWidth()), connected_edge(k_n%8), result);
             } else if (sum == 0) { return result;}
             return result;
        //}
    //}
}


bool SimilarityGraph::getNextNodeInLine( int i, int j, int& index )
{
    unsigned int width = getWidth();
    index = i * getWidth() + j;
    int edge = _graph[ index ].value;

    switch( edge )
    {
        case 0: return false;
        case 1:
        {
            index += width;
            return true;
        };
        case 2: return false;
        case 3:
        {
            index--;
            return true;
        }
        case 4:
        {
           index++;
           return true;
        }
        case 5: return false;
        case 6:
        {
            index = index - width;
            return true;
        }
        case 7: return false;
    }
}

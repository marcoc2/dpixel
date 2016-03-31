#include "DiagramCell.h"
#include "Depixel/Vector/Utils.h"

/** Check bit value */
#define CHECK_BIT( var, pos ) ( ( var ) & ( 1 << ( pos ) ) )

DiagramCell::DiagramCell() :
    _numberOfPoints( 0 )
{
}


DiagramCell::~DiagramCell()
{
}


//Point2D* DiagramCell::vectorToArrayOfPoints(vector<Point2D> inVector, int x, int y, int scale){
//    CvPoint* outArray;

//    outArray = (CvPoint*) malloc(inVector.size()*sizeof(CvPoint));

//    for (int i = 0; i < inVector.size(); i++){
//        CvPoint pt;
//        pt.x = inVector.at(i).x*scale + (x*scale);
//        pt.y = inVector.at(i).y*scale + (y*scale);
//        outArray[i] = pt;
//    }
//    return outArray;
//}



Point2D* DiagramCell::getBottomRightPoint()
{
    vector< Point2D* > right;

    /* Loop to find points in the right side of the cell */
    for( int i = 0; i < _numberOfPoints; i++ )
    {
        if( _points.at( i ).x > 0.5 )
        {
            /* Get pointer to the Point */
            right.push_back( &( _points.at( i ) ) );
        }
    }

    float less = 10.0;
    Point2D* pLess = 0;

    for( unsigned int i = 0; i < right.size(); i++ )
    {
        if( ( right.at( i ) )->y < less )
        {
            less = ( right.at( i ) )->y;
            pLess = right.at( i );
        }
    }

    return pLess;
}


Point2D* DiagramCell::getBottomLeftPoint()
{
    vector< Point2D* > left;

    /* Loop to find points in the right side of the cell */
    for( int i = 0; i < _numberOfPoints; i++ )
    {
        if( _points.at( i ).x < 0.5 )
        {
            /* Get pointer to the Point */
            left.push_back( &( _points.at( i ) ) );
        }
    }

    float less = 10.0;
    Point2D* pLess = 0;

    for( unsigned int i = 0; i < left.size(); i++ )
    {
        if( ( left.at( i ) )->y < less )
        {
            less = ( left.at( i ) )->y;
            pLess = left.at( i );
        }
    }

    return pLess;
}


Point2D* DiagramCell::getUpperRightPoint()
{
    vector< Point2D* > right;

    /* Loop to find points in the right side of the cell */
    for( int i = 0; i < _numberOfPoints; i++ )
    {
        if( _points.at( i ).x > 0.5 )
        {
            /* Get pointer to the Point */
            right.push_back( &( _points.at( i ) ) );
        }
    }

    float greatest = -10.0;
    Point2D* pGreatest = 0;

    for( unsigned int i = 0; i < right.size(); i++ )
    {
        if( ( right.at( i ) )->y > greatest )
        {
            greatest = ( right.at( i ) )->y;
            pGreatest = right.at( i );
        }
    }

    return pGreatest;
}


Point2D* DiagramCell::getUpperLeftPoint()
{
    vector< Point2D* > left;

    /* Loop to find points in the right side of the cell */
    for( int i = 0; i < _numberOfPoints; i++ )
    {
        if( _points.at( i ).x < 0.5 )
        {
            /* Get pointer to the Point */
            left.push_back( &( _points.at( i ) ) );
        }
    }

    float greatest = -10.0;
    Point2D* pGreatest = 0;
    for( unsigned int i = 0; i < left.size(); i++ )
    {
        if( ( left.at( i ) )->y > greatest )
        {
            greatest = ( left.at( i ) )->y;
            pGreatest = left.at( i );
        }
    }

    return pGreatest;
}


bool DiagramCell::isDiagonalCell()
{
    if( _numberOfPoints != 4 )
    {
        return false;
    }
    if( ( _points.at( 0 ).x == -0.25 ) && ( _points.at( 0 ).y == 0.25 ) &&
        ( _points.at( 1 ).x == 0.25 ) && ( _points.at( 1 ).y == -0.25 ) &&
        ( _points.at( 2 ).x == 1.25 ) && ( _points.at( 2 ).y == 0.75 ) &&
        ( _points.at( 3 ).x == 0.75 ) && ( _points.at( 3 ).y == 1.25 ) )
    {
        return true;
    }
    if( ( _points.at( 0 ).x == -0.25 ) && ( _points.at( 0 ).y == 0.75 ) &&
        ( _points.at( 1 ).x == 0.75 ) && ( _points.at( 1 ).y == -0.25 ) &&
        ( _points.at( 2 ).x == 1.25 ) && ( _points.at( 2 ).y == 0.25 ) &&
        ( _points.at( 3 ).x == 0.25 ) && ( _points.at( 3 ).y == 1.25 ) )
    {
        return true;
    }

    return false;
}


bool DiagramCell::isDiagonalBorderCell()
{
    if( ( _numberOfPoints < 5 ) && ( _numberOfPoints > 6 ) )
    {
        return false;
    }
    if( ( _points.at( 0 ).x == 0 ) && ( _points.at( 0 ).y == 0 ) &&
        ( _points.at( 1 ).x == 1 ) && ( _points.at( 1 ).y == 0 ) &&
        ( _points.at( 2 ).x == 1.25 ) && ( _points.at( 2 ).y == 0.25 ) &&
        ( _points.at( 3 ).x == 0.25 ) && ( _points.at( 3 ).y == 1.25 ) &&
        ( _points.at( 4 ).x == 0 ) && ( _points.at( 4 ).y == 1 ) )
    {
        return true;
    }
    if( ( _points.at( 0 ).x == -0.25 ) && ( _points.at( 0 ).y == 0.75 ) &&
        ( _points.at( 1 ).x == 0.75 ) && ( _points.at( 1 ).y == -0.25 ) &&
        ( _points.at( 2 ).x == 1 ) && ( _points.at( 2 ).y == 0 ) &&
        ( _points.at( 3 ).x == 1 ) && ( _points.at( 3 ).y == 1 ) &&
        ( _points.at( 4 ).x == 0 ) && ( _points.at( 4 ).y == 1 ) )
    {
        return true;
    }
    if( ( _points.at( 0 ).x == -0.25 ) && ( _points.at( 0 ).y == 0.25 ) &&
        ( _points.at( 1 ).x == 0 ) && ( _points.at( 1 ).y == 0 ) &&
        ( _points.at( 2 ).x == 1 ) && ( _points.at( 2 ).y == 0 ) &&
        ( _points.at( 3 ).x == 1 ) && ( _points.at( 3 ).y == 1 ) &&
        ( _points.at( 4 ).x == 0.75 ) && ( _points.at( 4 ).y == 1.25 ) )
    {
        return true;
    }
    if( ( _points.at( 0 ).x == 0 ) && ( _points.at( 0 ).y == 0 ) &&
        ( _points.at( 1 ).x == 0.25 ) && ( _points.at( 1 ).y == -0.25 ) &&
        ( _points.at( 2 ).x == 1.25 ) && ( _points.at( 2 ).y == 0.75 ) &&
        ( _points.at( 3 ).x == 1 ) && ( _points.at( 3 ).y == 1 ) &&
        ( _points.at( 4 ).x == 0 ) && ( _points.at( 4 ).y == 1 ) )
    {
        return true;
    }

    if( ( _points.at( 0 ).x == -0.25 ) && ( _points.at( 0 ).y == 0.25 ) &&
        ( _points.at( 1 ).x == 0.25 ) && ( _points.at( 1 ).y == -0.25 ) &&
        ( _points.at( 2 ).x == 0.75 ) && ( _points.at( 2 ).y == -0.25 ) &&
        ( _points.at( 3 ).x == 1 ) && ( _points.at( 3 ).y == 0 ) &&
        ( _points.at( 4 ).x == 1 ) && ( _points.at( 4 ).y == 1 ) &&
        ( _points.at( 5 ).x == 0.75 ) && ( _points.at( 5 ).y == 1.25 ) )
    {
        return true;
    }
    if( ( _points.at( 0 ).x == -0.25 ) && ( _points.at( 0 ).y == 0.25 ) &&
        ( _points.at( 1 ).x == 0.25 ) && ( _points.at( 1 ).y == -0.25 ) &&
        ( _points.at( 2 ).x == 1.25 ) && ( _points.at( 2 ).y == 0.75 ) &&
        ( _points.at( 3 ).x == 1 ) && ( _points.at( 3 ).y == 1 ) &&
        ( _points.at( 4 ).x == 0 ) && ( _points.at( 4 ).y == 1 ) )
    {
        return true;
    }

    return false;
}


void DiagramCell::printCellInfo()
{
    for( int i = 0; i < _numberOfPoints; i++ )
    {
//        cout << "Point" << i << " - ";
//        cout << "x: " << _points.at(i).x;
//        cout << " y: " << _points.at(i).y;
//        cout << endl;
    }
}


int DiagramCell::getN_points()
{
    return _numberOfPoints;
}


vector< Point2D > DiagramCell::getPoints()
{
    return _points;
}


void DiagramCell::setColor( char* color )
{
    //    this->color = color;
    this->_color[ 0 ] = color[ 0 ];
    this->_color[ 1 ] = color[ 1 ];
    this->_color[ 2 ] = color[ 2 ];
}


char* DiagramCell::getColor()
{
    return _color;
}


void DiagramCell::createCellFromPattern( SimilarityGraph::Node node, SimilarityGraph::Node nodeLeft,
                                         SimilarityGraph::Node nodeRight )
{
    // Clockwise order: 0, 1, 2, 4, 7, 6, 5, 3

    //if ((unsigned int)node == 90) return;

    _color[ 0 ] = node.color[ 0 ];
    _color[ 1 ] = node.color[ 1 ];
    _color[ 2 ] = node.color[ 2 ];

    /* RTK Verify if this is giving clockwise points or convex_hull */

    if( ( bool ) CHECK_BIT( node.value, 0 ) )
    {
        if( ( ( bool ) CHECK_BIT( node.value, 1 ) ) && ( !( bool ) CHECK_BIT( node.value, 3 ) ) )
        {
            _points.push_back( Point2D( -0.25, 0.75 ) );
        }
        else if( ( ( bool ) CHECK_BIT( node.value, 3 ) ) && ( !( bool ) CHECK_BIT( node.value, 1 ) ) )
        {
            _points.push_back( Point2D( 0.25, 1.25 ) );
        }
        else
        {
            _points.push_back( Point2D( -0.25, 0.75 ) );
            _points.push_back( Point2D( 0.25, 1.25 ) );
        }
    }
    else
    {
        /* Check neighbor edge */
        if( CHECK_BIT( nodeLeft.value, 2 ) )
        {
            _points.push_back( Point2D( 0.25, 0.75 ) );
        }
        else
        {
            _points.push_back( Point2D( 0.0, 1.0 ) );
        }
    }


    if( ( bool ) CHECK_BIT( node.value, 1 ) )
    {
        _points.push_back( Point2D( 0, 1 ) );
        _points.push_back( Point2D( 1, 1 ) );
    }
    else
    {
        _points.push_back( Point2D( 0.5, 0.75 ) );
    }

    if( ( bool ) CHECK_BIT( node.value, 2 ) )
    {
        if( ( ( bool ) CHECK_BIT( node.value, 1 ) ) && ( !( bool ) CHECK_BIT( node.value, 4 ) ) )
        {
            _points.push_back( Point2D( 1.25, 0.75 ) );
        }
        else if( ( ( bool ) CHECK_BIT( node.value, 4 ) ) && ( !( bool ) CHECK_BIT( node.value, 1 ) ) )
        {
            _points.push_back( Point2D( 0.75, 1.25 ) );
        }
        else
        {
            _points.push_back( Point2D( 0.75, 1.25 ) );
            _points.push_back( Point2D( 1.25, 0.75 ) );
        }
    }
    else
    {
        /* Check neighbor edge */
        if( CHECK_BIT( nodeRight.value, 0 ) )
        {
            _points.push_back( Point2D( 0.75, 0.75 ) );
        }
        else
        {
            _points.push_back( Point2D( 1.0, 1.0 ) );
        }
    }


    if( ( bool ) CHECK_BIT( node.value, 4 ) )
    {
        _points.push_back( Point2D( 1, 1 ) );
        _points.push_back( Point2D( 1, 0 ) );
    }
    else
    {
        _points.push_back( Point2D( 0.75, 0.5 ) );
    }

    if( ( bool ) CHECK_BIT( node.value, 7 ) )
    {
        if( ( ( bool ) CHECK_BIT( node.value, 4 ) ) && ( !( bool ) CHECK_BIT( node.value, 6 ) ) )
        {
            _points.push_back( Point2D( 0.75, -0.25 ) );
        }
        else if( ( ( bool ) CHECK_BIT( node.value, 6 ) ) && ( !( bool ) CHECK_BIT( node.value, 4 ) ) )
        {
            _points.push_back( Point2D( 1.25, 0.25 ) );
        }
        else
        {
            _points.push_back( Point2D( 1.25, 0.25 ) );
            _points.push_back( Point2D( 0.75, -0.25 ) );
        }
    }
    else
    {
        /* Check neighbor edge */
        if( CHECK_BIT( nodeRight.value, 5 ) )
        {
            _points.push_back( Point2D( 0.75, 0.25 ) );
        }
        else
        {
            _points.push_back( Point2D( 1.0, 0.0 ) );
        }
    }



    if( ( bool ) CHECK_BIT( node.value, 6 ) )
    {
        _points.push_back( Point2D( 1, 0 ) );
        _points.push_back( Point2D( 0, 0 ) );
    }
    else
    {
        _points.push_back( Point2D( 0.25, 0.75 ) );
    }

    if( ( bool ) CHECK_BIT( node.value, 5 ) )
    {
        if( ( ( bool ) CHECK_BIT( node.value, 3 ) ) && ( !( bool ) CHECK_BIT( node.value, 6 ) ) )
        {
            _points.push_back( Point2D( 0.25, -0.25 ) );
        }
        else if( ( ( bool ) CHECK_BIT( node.value, 6 ) ) && ( !( bool ) CHECK_BIT( node.value, 3 ) ) )
        {
            _points.push_back( Point2D( -0.25, 0.25 ) );
        }
        else
        {
            _points.push_back( Point2D( 0.25, -0.25 ) );
            _points.push_back( Point2D( -0.25, 0.25 ) );
        }
    }
    else
    {
        /* Check neighbor edge */
        if( CHECK_BIT( nodeLeft.value, 7 ) )
        {
            _points.push_back( Point2D( 0.25, 0.25 ) );
        }
        else
        {
            _points.push_back( Point2D( 0.0, 0.0 ) );
        }
    }



    if( ( bool ) CHECK_BIT( node.value, 3 ) )
    {
        _points.push_back( Point2D( 0, 0 ) );
        _points.push_back( Point2D( 0, 1 ) );
    }
    else
    {
        _points.push_back( Point2D( 0.25, 0.5 ) );
    }


    vector< Point2D > aux;

    aux = convex_hull( _points );
    /* RTK Remove last point. Convex Hull function give this point repeated */
    aux.pop_back();

    _points.clear();
    _points = aux;
    _numberOfPoints = _points.size();
}



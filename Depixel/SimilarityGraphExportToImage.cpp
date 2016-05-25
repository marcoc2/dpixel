#include "SimilarityGraphExportToImage.h"

#include "Depixel/SimilarityGraph.h"
#include "Image.h"

#include <cmath>

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

SimilarityGraphExportToImage::SimilarityGraphExportToImage( SimilarityGraph* similarityGraph ) :
    _similarityGraph( similarityGraph )
{

}


void SimilarityGraphExportToImage::draw( Image* image, int factor )
{
    int half = factor / 2;
    int diagonal = sqrt( half * half );
    for( unsigned int i = 0; i < _similarityGraph->getHeight(); i++ )
    {
        for( unsigned int j = 0; j < _similarityGraph->getWidth(); j++ )
        {
            SimilarityGraph::Node node = _similarityGraph->getNode( i, j );
            if CHECK_BIT( node.value, 0)
            {
                for( int t = 0; t < diagonal; t++ )
                {
                    //image->setPixel( t );
                }
            }
        }
    }

}

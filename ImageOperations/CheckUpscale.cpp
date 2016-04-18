#include <map>

#include "CheckUpscale.h"
#include "SimilarityGraph.h"

CheckUpscale::CheckUpscale()
{

}


int CheckUpscale::checkUpscale( SimilarityGraph* similarityGraph )
{
    std::vector< int > adjacentInLine;
    bool* visited = new bool[ similarityGraph->getWidth() * similarityGraph->getHeight() ]();
    int numberOfAdjacentNeighbors;

    for( unsigned int i = 0; i < similarityGraph->getHeight(); i++ )
    {
        for( unsigned int j = 0; j < similarityGraph->getWidth(); j++ )
        {
            numberOfAdjacentNeighbors = 0;
            if( !visited[ i * similarityGraph->getWidth() + j ] )
            {
                visited[ i * similarityGraph->getWidth() + j ] = true;
                int index = i * similarityGraph->getWidth() + j;

                if( similarityGraph->isStartNodeOnHorizontal( index ) )
                {

                    numberOfAdjacentNeighbors++;

                    while( similarityGraph->getNextNodeInLine( index ) )
                    {
                        visited[ index ] = true;
                        numberOfAdjacentNeighbors++;
                    }
                    visited[ index ] = true;

                    if( numberOfAdjacentNeighbors > 0 )
                    {
                        adjacentInLine.push_back( numberOfAdjacentNeighbors );
                    }
                }
            }
        }
    }

    delete[] visited;

    return createHistogram( adjacentInLine );
}


int CheckUpscale::createHistogram( std::vector<int>& adjacentInLine )
{
    int size = adjacentInLine.size();
    int* histogram = new int[ size ]();
    std::map< int, int > histogramMap;

    for( auto const& i : adjacentInLine )
    {
        histogram[ i ]++;
        histogramMap[ i ] = histogram[ i ];
    }

    //pos_min is short for position of min
    //int pos_min, temp;
    //int* adjacentSorted = new int[ size ]();

    //for( int i = 0; i < size - 1; i++ )
    //{
    //    pos_min = i;//set pos_min to the current index of array

    //    for( int j = i + 1; j < size; j++ )
    //    {
    //        if( histogram[ j ] < histogram[ pos_min ] )
    //        {
    //            pos_min = j;
    //        }
    //    }

    //    if( pos_min != i )
    //    {
    //        temp = histogram[ i ];
    //        histogram[ i ] = histogram[ pos_min ];
    //        histogram[ pos_min ] = temp;
    //        adjacentSorted[ pos_min ] = adjacentInLine[ i ];
    //    }
    //}

    printf( "Graph histogram:\n" );
    for( auto const& i : histogramMap )
    {
        printf("Key: %d - Value: %d\n", i.first, i.second );
    }

    return histogramMap.begin()->first;
}

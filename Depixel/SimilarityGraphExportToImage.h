#ifndef SIMILARITYGRAPHEXPORTTOIMAGE_H
#define SIMILARITYGRAPHEXPORTTOIMAGE_H

class SimilarityGraph;
class Image;

class SimilarityGraphExportToImage
{
public:
    SimilarityGraphExportToImage( SimilarityGraph* similarityGraph );

    void draw( Image* image, int factor );

private:

    SimilarityGraph* _similarityGraph;
};

#endif // SIMILARITYGRAPHEXPORTTOIMAGE_H

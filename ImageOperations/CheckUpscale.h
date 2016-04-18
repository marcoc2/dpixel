#ifndef CHECKUPSCALE_H
#define CHECKUPSCALE_H

#include <vector>

class SimilarityGraph;

class CheckUpscale
{
public:
    CheckUpscale();

    /**
     * @brief checkUpscale Check for upscale by constinuous horizontal line heuristic
     * @param similarityGraph
     * @return
     */
    static int checkUpscale( SimilarityGraph* similarityGraph );

private:

    /**
     * @brief createHistogram Create a histogram of numbers of consecutive nodes in a
     *                        continous horizontal line
     * @param adjacentInLine
     * @return
     */
    static int createHistogram( std::vector<int>& adjacentInLine );
};

#endif // CHECKUPSCALE_H

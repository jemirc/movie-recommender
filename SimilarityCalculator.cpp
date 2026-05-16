#include "SimilarityCalculator.h"

#include <cmath>

int SimilarityCalculator::calculate(const std::vector<Rating> &ratingsA, const std::vector<Rating> &ratingsB)
{
    int commonCount = 0;
    int scoreDiffSum = 0;

    for (const Rating &ratingA : ratingsA)
    {
        for (const Rating &ratingB : ratingsB)
        {
            if (ratingA.getMovieId() == ratingB.getMovieId())
            {
                commonCount++;

                const int diff = static_cast<int>(std::abs(ratingA.getScore() - ratingB.getScore()));
                scoreDiffSum += diff;
            }
        }
    }

    if (commonCount == 0)
    {
        return -100;
    }

    return commonCount * 10 - scoreDiffSum;
}

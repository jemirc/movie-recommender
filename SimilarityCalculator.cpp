#include "SimilarityCalculator.h"

#include <cmath>

double SimilarityCalculator::calculate(const std::vector<Rating> &ratingsA, const std::vector<Rating> &ratingsB)
{
    int commonCount = 0;
    double scoreDiffSum = 0.0;

    for (const Rating &ratingA : ratingsA)
    {
        for (const Rating &ratingB : ratingsB)
        {
            if (ratingA.getMovieId() == ratingB.getMovieId())
            {
                commonCount++;
                scoreDiffSum += std::abs(ratingA.getScore() - ratingB.getScore());
            }
        }
    }

    if (commonCount == 0)
    {
        return -100.0;
    }

    return commonCount * 10.0 - scoreDiffSum;
}

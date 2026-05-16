#pragma once

#include <utility>
#include <vector>

#include "Movie.h"
#include "MovieManager.h"
#include "RatingManager.h"

class Recommender
{
private:
    const MovieManager &movieManager;
    const RatingManager &ratingManager;

public:
    Recommender(const MovieManager &movieManager, const RatingManager &ratingManager);

    std::vector<std::pair<const Movie *, int>> recommend(int userId, int topKUsers = 3, int topNMovies = 5) const;
};

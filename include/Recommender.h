#pragma once

#include <string>
#include <utility>
#include <vector>

#include "Movie.h"
#include "MovieConstants.h"
#include "MovieManager.h"
#include "RatingManager.h"

class Recommender
{
private:
    const MovieManager &movieManager;
    const RatingManager &ratingManager;

public:
    Recommender(const MovieManager &movieManager, const RatingManager &ratingManager);

    std::vector<std::pair<const Movie *, double>> recommend(int userId,
                                                            int topKUsers = MovieConstants::DEFAULT_TOP_K_USERS,
                                                            int topNMovies = MovieConstants::DEFAULT_TOP_N_MOVIES,
                                                            const std::string &genre = "") const;
};

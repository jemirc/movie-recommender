#pragma once

#include <cstddef>
#include <vector>

#include "Rating.h"

class MovieManager;
class UserManager;

class RatingManager
{
private:
    std::vector<Rating> ratings;

public:
    RatingManager() = default;

    bool addRating(int userId, int movieId, double score, const UserManager &userManager, MovieManager &movieManager);
    std::vector<const Rating *> getRatingsByMovieId(int movieId) const;
    void printRatingsByMovieId(int movieId) const;

    const std::vector<Rating> &getRatings() const;
    std::size_t getRatingCount() const;
};

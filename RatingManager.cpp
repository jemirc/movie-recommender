#include "RatingManager.h"

#include <iostream>

#include "MovieManager.h"
#include "UserManager.h"

bool RatingManager::addRating(int userId, int movieId, double score, const UserManager &userManager, MovieManager &movieManager)
{
    if (score < 0.0 || score > 5.0)
    {
        return false;
    }

    if (userManager.findUserById(userId) == nullptr)
    {
        return false;
    }

    Movie *movie = movieManager.findMovieById(movieId);
    if (movie == nullptr)
    {
        return false;
    }

    for (const Rating &rating : ratings)
    {
        if (rating.getUserId() == userId && rating.getMovieId() == movieId)
        {
            return false;
        }
    }

    Rating rating(userId, movieId, score);
    ratings.push_back(rating);
    movie->addRating(rating.getScore());
    return true;
}

std::vector<const Rating *> RatingManager::getRatingsByMovieId(int movieId) const
{
    std::vector<const Rating *> matchedRatings;

    for (const Rating &rating : ratings)
    {
        if (rating.getMovieId() == movieId)
        {
            matchedRatings.push_back(&rating);
        }
    }

    return matchedRatings;
}

void RatingManager::printRatingsByMovieId(int movieId) const
{
    const std::vector<const Rating *> matchedRatings = getRatingsByMovieId(movieId);

    for (const Rating *rating : matchedRatings)
    {
        std::cout << *rating << std::endl;
    }
}

const std::vector<Rating> &RatingManager::getRatings() const
{
    return ratings;
}

std::size_t RatingManager::getRatingCount() const
{
    return ratings.size();
}

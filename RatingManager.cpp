#include "RatingManager.h"

#include <iostream>

#include "MovieManager.h"
#include "UserManager.h"

RatingManager::RatingManager()
    : lastRatingUpdated(false)
{
}

bool RatingManager::addRating(int userId, int movieId, double score, const UserManager &userManager, MovieManager &movieManager)
{
    lastRatingUpdated = false;

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

    Rating newRating(userId, movieId, score);

    for (Rating &rating : ratings)
    {
        if (rating == newRating)
        {
            const double oldScore = rating.getScore();
            if (!movie->updateRating(oldScore, score) || !rating.setScore(score))
            {
                return false;
            }

            lastRatingUpdated = true;
            return true;
        }
    }

    ratings.push_back(newRating);
    movie->addRating(newRating.getScore());
    return true;
}

bool RatingManager::wasLastRatingUpdated() const
{
    return lastRatingUpdated;
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

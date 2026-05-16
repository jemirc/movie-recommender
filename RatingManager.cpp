#include "RatingManager.h"

#include <iostream>

#include "MovieManager.h"
#include "UserManager.h"

RatingManager::RatingManager()
    : lastRatingUpdated(false)
{
}

void RatingManager::loadFromFile(const std::string &filename)
{
    (void)filename;
}

void RatingManager::saveToFile(const std::string &filename) const
{
    (void)filename;
}

std::size_t RatingManager::size() const
{
    return ratings.size();
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

    // 이미 평점을 준 적이 있는지 확인해서 있으면 업데이트
    for (Rating &rating : ratings)
    {
        if (rating == newRating)
        {
            const double oldScore = rating.getScore();
            // 영화의 평점 업데이트와 Rating 객체의 점수 업데이트 둘 다 성공해야 true 반환, 하나라도 실패하면 false 반환
            if (!movie->updateRating(oldScore, score) || !rating.setScore(score))
            {
                return false;
            }

            lastRatingUpdated = true;
            return true;
        }
    }

    // 없으면 새로 추가
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

std::size_t RatingManager::getRatingCount() const
{
    return size();
}

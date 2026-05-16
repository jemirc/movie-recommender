#include "RatingManager.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include "MovieManager.h"
#include "UserManager.h"

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
}
}

RatingManager::RatingManager()
    : lastRatingUpdated(false)
{
}

void RatingManager::loadFromFile(const std::string &filename)
{
    ratings.clear();
    lastRatingUpdated = false;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (shouldSkipLine(line))
        {
            continue;
        }

        std::stringstream ss(line);
        std::string userIdText;
        std::string movieIdText;
        std::string scoreText;

        if (!std::getline(ss, userIdText, ',') ||
            !std::getline(ss, movieIdText, ',') ||
            !std::getline(ss, scoreText))
        {
            continue;
        }

        try
        {
            const int userId = std::stoi(userIdText);
            const int movieId = std::stoi(movieIdText);
            const double score = std::stod(scoreText);
            ratings.push_back(Rating(userId, movieId, score));
        }
        catch (const std::exception &)
        {
            continue;
        }
    }
}

void RatingManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    for (const Rating &rating : ratings)
    {
        file << rating.getUserId() << ','
             << rating.getMovieId() << ','
             << rating.getScore() << '\n';
    }
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

std::vector<Rating> RatingManager::findByUser(int userId) const
{
    std::vector<Rating> matchedRatings;

    for (const Rating &rating : ratings)
    {
        if (rating.getUserId() == userId)
        {
            matchedRatings.push_back(rating);
        }
    }

    return matchedRatings;
}

std::vector<int> RatingManager::getAllUserIds() const
{
    std::set<int> uniqueUserIds;

    for (const Rating &rating : ratings)
    {
        uniqueUserIds.insert(rating.getUserId());
    }

    return std::vector<int>(uniqueUserIds.begin(), uniqueUserIds.end());
}

const std::vector<Rating> &RatingManager::getAllRatings() const
{
    return ratings;
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

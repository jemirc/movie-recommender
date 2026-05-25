#include "RatingManager.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

#include "CsvUtils.h"
#include "MovieManager.h"
#include "UserManager.h"

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
}

std::string scoreToText(double score)
{
    std::ostringstream output;
    output << score;
    return output.str();
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
        std::cerr << "평점 CSV 파일을 열 수 없습니다: " << filename << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        if (shouldSkipLine(line))
        {
            continue;
        }

        std::vector<std::string> fields;
        if (!CsvUtils::parseLine(line, fields) || fields.size() != 3)
        {
            continue;
        }

        try
        {
            const int userId = std::stoi(fields[0]);
            const int movieId = std::stoi(fields[1]);
            const double score = std::stod(fields[2]);
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
        std::cerr << "평점 CSV 파일을 저장할 수 없습니다: " << filename << std::endl;
        return;
    }

    for (const Rating &rating : ratings)
    {
        file << CsvUtils::makeLine({std::to_string(rating.getUserId()),
                                    std::to_string(rating.getMovieId()),
                                    scoreToText(rating.getScore())})
             << '\n';
    }
}

std::size_t RatingManager::size() const
{
    return ratings.size();
}

bool RatingManager::addRating(int userId, int movieId, double score, const UserManager &userManager, const MovieManager &movieManager)
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

    const Movie *movie = movieManager.findMovieById(movieId);
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
            if (!rating.setScore(score))
            {
                return false;
            }

            lastRatingUpdated = true;
            return true;
        }
    }

    // 없으면 새로 추가
    ratings.push_back(newRating);
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

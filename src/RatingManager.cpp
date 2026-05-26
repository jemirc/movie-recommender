#include "RatingManager.h"

#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stdexcept>

#include "MovieConstants.h"
#include "MovieManager.h"
#include "UserManager.h"

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
}

Rating parseRatingLine(const std::string &line)
{
    std::stringstream ss(line);
    std::string userIdText;
    std::string movieIdText;
    std::string scoreText;
    std::string extraText;

    if (!std::getline(ss, userIdText, ',') ||
        !std::getline(ss, movieIdText, ',') ||
        !std::getline(ss, scoreText, ',') ||
        std::getline(ss, extraText, ','))
    {
        throw std::invalid_argument("평점 CSV는 " + std::to_string(MovieConstants::RATING_CSV_FIELD_COUNT) + "개 값이어야 합니다");
    }

    const int userId = std::stoi(userIdText);
    const int movieId = std::stoi(movieIdText);
    const double score = std::stod(scoreText);
    if (score < MovieConstants::MIN_RATING_SCORE || score > MovieConstants::MAX_RATING_SCORE)
    {
        throw std::invalid_argument("평점 범위가 올바르지 않습니다");
    }

    return Rating(userId, movieId, score);
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
        throw std::runtime_error("평점 CSV 파일을 열 수 없습니다: " + filename);
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (shouldSkipLine(line))
        {
            continue;
        }

        try
        {
            ratings.push_back(parseRatingLine(line));
        }
        catch (const std::exception &e)
        {
            std::cerr << filename << " " << lineNumber << "번 줄 건너뜀: " << e.what() << std::endl;
        }
    }
}

void RatingManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("평점 CSV 파일을 저장할 수 없습니다: " + filename);
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

    if (score < MovieConstants::MIN_RATING_SCORE || score > MovieConstants::MAX_RATING_SCORE)
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

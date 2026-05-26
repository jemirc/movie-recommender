#include "Rating.h"
#include <iostream>

#include "MovieConstants.h"

using namespace std;
Rating::Rating(int userId, int movieId, double score)
    : userId(userId), movieId(movieId), score(score) {}

int Rating::getUserId() const
{
    return userId;
}

double Rating::getScore() const
{
    return score;
}

int Rating::getMovieId() const
{
    return movieId;
}

bool Rating::setScore(double score)
{
    if (score < MovieConstants::MIN_RATING_SCORE || score > MovieConstants::MAX_RATING_SCORE)
    {
        return false;
    }

    this->score = score;
    return true;
}

bool Rating::operator==(const Rating &other) const
{
    return userId == other.userId && movieId == other.movieId;
}

ostream &operator<<(ostream &os, const Rating &rating)
{
    os << "[평점] "
       << "사용자 ID: " << rating.userId
       << " | 영화 ID: " << rating.movieId
       << " | 점수: " << rating.score;
    return os;
}

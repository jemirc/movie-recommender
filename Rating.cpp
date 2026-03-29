#include "Rating.h"
#include <iostream>

using namespace std;
Rating::Rating(int userId, int movieId, double score)
    : userId(userId), movieId(movieId)
{
    if (score < 0.0 || score > 5.0)
    {
        cout << "점수는 0.0에서 5.0 사이여야 합니다. 0.0으로 초기화합니다." << endl;
        this->score = 0.0;
    }
    else
    {
        this->score = score;
    }
}

double Rating::getScore() const
{
    return score;
}

int Rating::getUserId() const
{
    return userId;
}

int Rating::getMovieId() const
{
    return movieId;
}

void Rating::display() const
{
    cout << "유저 id (" << userId << ")님이 "
         << "영화 id (" << movieId << ")에 "
         << score << " 별점을 줬습니다" << endl;
}
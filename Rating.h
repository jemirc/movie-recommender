#pragma once

#include <ostream>

class Rating
{
private:
    int userId;
    int movieId;
    double score;

public:
    Rating(int userId, int movieId, double score);
    double getScore() const;
    int getUserId() const;
    int getMovieId() const;
    bool setScore(double score);
    void display() const;
    bool operator==(const Rating &other) const;

    friend std::ostream &operator<<(std::ostream &os, const Rating &rating);
};

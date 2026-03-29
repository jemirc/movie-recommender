#pragma once

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
    void display() const;
};
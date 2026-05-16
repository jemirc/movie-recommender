#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "BaseManager.h"
#include "Rating.h"

class MovieManager;
class UserManager;

class RatingManager : public BaseManager
{
private:
    std::vector<Rating> ratings;
    bool lastRatingUpdated;

public:
    RatingManager();

    bool addRating(int userId, int movieId, double score, const UserManager &userManager, MovieManager &movieManager);
    void loadFromFile(const std::string &filename) override;
    void saveToFile(const std::string &filename) const override;
    std::size_t size() const override;
    bool wasLastRatingUpdated() const;
    std::vector<Rating> findByUser(int userId) const;
    std::vector<int> getAllUserIds() const;
    const std::vector<Rating> &getAllRatings() const;
    std::vector<const Rating *> getRatingsByMovieId(int movieId) const;
    void printRatingsByMovieId(int movieId) const;

    std::size_t getRatingCount() const;
};

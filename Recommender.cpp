#include "Recommender.h"

#include <algorithm>
#include <map>
#include <set>

#include "SimilarityCalculator.h"

Recommender::Recommender(const MovieManager &movieManager, const RatingManager &ratingManager)
    : movieManager(movieManager), ratingManager(ratingManager)
{
}

std::vector<std::pair<const Movie *, int>> Recommender::recommend(int userId, int topKUsers, int topNMovies) const
{
    if (topKUsers <= 0 || topNMovies <= 0)
    {
        return {};
    }

    const std::vector<Rating> myRatings = ratingManager.findByUser(userId);
    if (myRatings.empty())
    {
        return {};
    }

    std::set<int> myMovieIds;
    for (const Rating &rating : myRatings)
    {
        myMovieIds.insert(rating.getMovieId());
    }

    std::vector<std::pair<int, int>> similarities;

    for (int otherUserId : ratingManager.getAllUserIds())
    {
        if (otherUserId == userId)
        {
            continue;
        }

        const std::vector<Rating> otherRatings = ratingManager.findByUser(otherUserId);
        const int similarity = SimilarityCalculator::calculate(myRatings, otherRatings);

        if (similarity > -100)
        {
            similarities.push_back({otherUserId, similarity});
        }
    }

    std::sort(similarities.begin(), similarities.end(),
              [](const auto &left, const auto &right)
              {
                  return left.second > right.second;
              });

    std::map<int, int> movieScores;
    int usedUsers = 0;

    for (const auto &[otherUserId, similarity] : similarities)
    {
        if (usedUsers >= topKUsers)
        {
            break;
        }

        const std::vector<Rating> otherRatings = ratingManager.findByUser(otherUserId);
        for (const Rating &rating : otherRatings)
        {
            const int movieId = rating.getMovieId();

            if (myMovieIds.find(movieId) != myMovieIds.end())
            {
                continue;
            }

            if (rating.getScore() >= 4.0)
            {
                movieScores[movieId] += static_cast<int>(rating.getScore() * similarity);
            }
        }

        usedUsers++;
    }

    std::vector<std::pair<int, int>> sortedScores(movieScores.begin(), movieScores.end());
    std::sort(sortedScores.begin(), sortedScores.end(),
              [](const auto &left, const auto &right)
              {
                  return left.second > right.second;
              });

    std::vector<std::pair<const Movie *, int>> recommendations;

    for (const auto &[movieId, score] : sortedScores)
    {
        if (static_cast<int>(recommendations.size()) >= topNMovies)
        {
            break;
        }

        const Movie *movie = movieManager.findMovieById(movieId);
        if (movie != nullptr)
        {
            recommendations.push_back({movie, score});
        }
    }

    return recommendations;
}

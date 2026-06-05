#include "Recommender.h"

#include <algorithm>
#include <map>
#include <set>
#include <string>

#include "MovieConstants.h"
#include "SimilarityCalculator.h"

namespace
{
using UserSimilarity = std::pair<int, double>;
using MovieScore = std::pair<int, double>;
using Recommendation = std::pair<const Movie *, double>;

bool compareByScoreDescThenId(const std::pair<int, double> &left, const std::pair<int, double> &right)
{
    if (left.second != right.second)
    {
        return left.second > right.second;
    }

    return left.first < right.first;
}

std::set<int> collectRatedMovieIds(const std::vector<Rating> &ratings)
{
    std::set<int> movieIds;

    for (const Rating &rating : ratings)
    {
        movieIds.insert(rating.getMovieId());
    }

    return movieIds;
}

std::vector<UserSimilarity> calculateUserSimilarities(int userId,
                                                      const std::vector<Rating> &myRatings,
                                                      const RatingManager &ratingManager)
{
    std::vector<UserSimilarity> similarities;

    // 다른 사용자들과의 유사도만 계산해서 추천 후보 사용자 목록을 만드는거임
    for (int otherUserId : ratingManager.getAllUserIds())
    {
        if (otherUserId == userId)
        {
            continue;
        }

        const std::vector<Rating> otherRatings = ratingManager.findByUser(otherUserId);
        const double similarity = SimilarityCalculator::calculate(myRatings, otherRatings);

        if (similarity > MovieConstants::NO_SIMILARITY_SCORE)
        {
            similarities.push_back({otherUserId, similarity});
        }
    }

    std::sort(similarities.begin(), similarities.end(), compareByScoreDescThenId);
    return similarities;
}

std::map<int, double> calculateMovieScores(const std::vector<UserSimilarity> &similarities,
                                           const std::set<int> &myMovieIds,
                                           const RatingManager &ratingManager,
                                           int topKUsers)
{
    std::map<int, double> movieScores;
    int usedUsers = 0;

    // 비슷한 사용자들이 높게 준 영화만 점수화해서 추천 후보로 모으는거임
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

            if (rating.getScore() >= MovieConstants::MIN_RECOMMEND_RATING)
            {
                movieScores[movieId] += rating.getScore() * similarity;
            }
        }

        usedUsers++;
    }

    return movieScores;
}

std::vector<MovieScore> sortMovieScores(const std::map<int, double> &movieScores)
{
    std::vector<MovieScore> sortedScores(movieScores.begin(), movieScores.end());
    std::sort(sortedScores.begin(), sortedScores.end(), compareByScoreDescThenId);
    return sortedScores;
}

std::vector<Recommendation> buildRecommendations(const std::vector<MovieScore> &sortedScores,
                                                 const MovieManager &movieManager,
                                                 int topNMovies,
                                                 const std::string &genre)
{
    std::vector<Recommendation> recommendations;

    // 점수가 높은 영화 ID를 실제 Movie 포인터로 바꿔서 화면 출력에 넘기는거임
    for (const auto &[movieId, score] : sortedScores)
    {
        if (static_cast<int>(recommendations.size()) >= topNMovies)
        {
            break;
        }

        const Movie *movie = movieManager.findMovieById(movieId);
        if (movie != nullptr && (genre.empty() || movie->getGenre() == genre))
        {
            recommendations.push_back({movie, score});
        }
    }

    return recommendations;
}
}

Recommender::Recommender(const MovieManager &movieManager, const RatingManager &ratingManager)
    : movieManager(movieManager), ratingManager(ratingManager)
{
}

std::vector<std::pair<const Movie *, double>> Recommender::recommend(int userId,
                                                                     int topKUsers,
                                                                     int topNMovies,
                                                                     const std::string &genre) const
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

    const std::set<int> myMovieIds = collectRatedMovieIds(myRatings);
    const std::vector<UserSimilarity> similarities = calculateUserSimilarities(userId, myRatings, ratingManager);
    const std::map<int, double> movieScores = calculateMovieScores(similarities, myMovieIds, ratingManager, topKUsers);
    const std::vector<MovieScore> sortedScores = sortMovieScores(movieScores);
    return buildRecommendations(sortedScores, movieManager, topNMovies, genre);
}

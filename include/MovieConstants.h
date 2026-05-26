#pragma once

namespace MovieConstants
{
constexpr int MIN_RELEASE_YEAR = 1;
constexpr double MIN_RATING_SCORE = 0.0;
constexpr double MAX_RATING_SCORE = 5.0;
constexpr double MIN_RECOMMEND_RATING = 4.0;
constexpr double NO_SIMILARITY_SCORE = -100.0;
constexpr double COMMON_MOVIE_WEIGHT = 10.0;
constexpr int DEFAULT_TOP_K_USERS = 3;
constexpr int DEFAULT_TOP_N_MOVIES = 5;
constexpr int MOVIE_CSV_FIELD_COUNT = 4;
constexpr int USER_CSV_FIELD_COUNT = 3;
constexpr int RATING_CSV_FIELD_COUNT = 3;
}

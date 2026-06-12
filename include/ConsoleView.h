#pragma once

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "Movie.h"
#include "MovieManager.h"
#include "Rating.h"
#include "User.h"

namespace ConsoleView
{
std::string formatScore(double score);

void printMovieTable(const std::vector<const Movie *> &movies);
void printUserTable(const std::vector<const User *> &users);
void printRatingTable(const std::vector<const Rating *> &ratings);
void printRecommendationTable(const std::vector<std::pair<const Movie *, double>> &recommendations);
void printGenreStatisticsTable(const std::map<std::string, GenreStatistics> &statisticsByGenre);
void printKeyValueTable(const std::vector<std::pair<std::string, std::string>> &rows);
}

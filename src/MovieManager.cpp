#include "MovieManager.h"

#include <algorithm> // find, sort 같은 알고리즘 함수 쓰려고 넣은거임
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "ConsoleView.h"
#include "MovieConstants.h"

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
}

std::string formatCsvScore(double score)
{
    std::ostringstream output;
    output << std::fixed << std::setprecision(2) << score;
    return output.str();
}

std::string escapeCsvCell(const std::string &cell)
{
    bool shouldQuote = false;
    std::string escaped;

    for (char ch : cell)
    {
        if (ch == '"' || ch == ',' || ch == '\n' || ch == '\r')
        {
            shouldQuote = true;
        }

        if (ch == '"')
        {
            escaped += "\"\"";
        }
        else
        {
            escaped += ch;
        }
    }

    return shouldQuote ? "\"" + escaped + "\"" : escaped;
}

void writeCsvRow(std::ofstream &file, const std::vector<std::string> &cells)
{
    for (std::size_t index = 0; index < cells.size(); index++)
    {
        if (index > 0)
        {
            file << ',';
        }

        file << escapeCsvCell(cells[index]);
    }

    file << '\n';
}

std::unique_ptr<Movie> parseMovieLine(const std::string &line)
{
    std::stringstream ss(line);
    std::string idText;
    std::string title;
    std::string genre;
    std::string yearText;
    std::string extraText;

    if (!std::getline(ss, idText, ',') ||
        !std::getline(ss, title, ',') ||
        !std::getline(ss, genre, ',') ||
        !std::getline(ss, yearText, ',') ||
        std::getline(ss, extraText, ','))
    {
        throw std::invalid_argument("영화 CSV는 " + std::to_string(MovieConstants::MOVIE_CSV_FIELD_COUNT) + "개 값이어야 합니다");
    }

    const int id = std::stoi(idText);
    const int year = std::stoi(yearText);
    if (year < MovieConstants::MIN_RELEASE_YEAR)
    {
        throw std::invalid_argument("개봉 연도는 " + std::to_string(MovieConstants::MIN_RELEASE_YEAR) + " 이상이어야 합니다");
    }

    return std::make_unique<Movie>(id, title, genre, year);
}

std::string normalizeSearchText(const std::string &text)
{
    std::string normalized = text;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char ch)
                   {
                       return static_cast<char>(std::tolower(ch));
                   });
    return normalized;
}

bool containsIgnoringAsciiCase(const std::string &text, const std::string &keyword)
{
    return normalizeSearchText(text).find(normalizeSearchText(keyword)) != std::string::npos;
}

bool comesBeforeByRating(const Movie *left, const Movie *right)
{
    if (left->getAverageRating() != right->getAverageRating())
    {
        return left->getAverageRating() > right->getAverageRating();
    }

    if (left->getRatingCount() != right->getRatingCount())
    {
        return left->getRatingCount() > right->getRatingCount();
    }

    return left->getId() < right->getId();
}

bool comesBeforeByTitle(const Movie *left, const Movie *right)
{
    if (left->getTitle() != right->getTitle())
    {
        return left->getTitle() < right->getTitle();
    }

    return left->getId() < right->getId();
}

bool comesBeforeByLatest(const Movie *left, const Movie *right)
{
    if (left->getReleaseYear() != right->getReleaseYear())
    {
        return left->getReleaseYear() > right->getReleaseYear();
    }

    return left->getId() < right->getId();
}

int countTotalRatings(const std::vector<std::unique_ptr<Movie>> &movies)
{
    return std::accumulate(movies.begin(), movies.end(), 0,
                           [](int total, const std::unique_ptr<Movie> &movie)
                           {
                               return total + movie->getRatingCount();
                           });
}
}

std::string getMovieSortOptionLabel(MovieSortOption sortOption)
{
    switch (sortOption)
    {
    case MovieSortOption::Rating:
        return "평점순";
    case MovieSortOption::Title:
        return "가나다순";
    case MovieSortOption::Latest:
        return "최신순";
    }

    return "평점순";
}

MovieManager::MovieManager()
    : nextId(1)
{
}

int MovieManager::addMovie(const std::string &title, const std::string &genre, int year)
{
    int id = nextId;
    movies.push_back(std::make_unique<Movie>(id, title, genre, year));
    nextId++;
    return id;
}

void MovieManager::loadFromFile(const std::string &filename)
{
    movies.clear();
    nextId = 1;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("영화 CSV 파일을 열 수 없습니다: " + filename);
    }

    std::string line;
    int lineNumber = 0;
    int maxId = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (shouldSkipLine(line))
        {
            continue;
        }

        try
        {
            std::unique_ptr<Movie> movie = parseMovieLine(line);
            const int id = movie->getId();
            movies.push_back(std::move(movie));

            if (id > maxId)
            {
                maxId = id;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << filename << " " << lineNumber << "번 줄 건너뜀: " << e.what() << std::endl;
        }
    }

    nextId = maxId + 1;
}

void MovieManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("영화 CSV 파일을 저장할 수 없습니다: " + filename);
    }

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        file << movie->getId() << ','
             << movie->getTitle() << ','
             << movie->getGenre() << ','
             << movie->getReleaseYear() << '\n';
    }
}

std::size_t MovieManager::size() const
{
    return movies.size();
}

Movie *MovieManager::findMovieById(int id)
{
    // id로 순차 탐색해서 실제 객체 주소를 돌려주는거임
    for (const std::unique_ptr<Movie> &movie : movies)
    {
        if (movie->getId() == id)
        {
            return movie.get();
        }
    }

    return nullptr;
}

const Movie *MovieManager::findMovieById(int id) const
{
    // const 함수에서는 const 포인터로 돌려줘야 안전한거임
    for (const std::unique_ptr<Movie> &movie : movies)
    {
        if (movie->getId() == id)
        {
            return movie.get();
        }
    }

    return nullptr;
}

std::vector<const Movie *> MovieManager::getAllMovies(MovieSortOption sortOption) const
{
    std::vector<const Movie *> allMovies;
    allMovies.reserve(movies.size());

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        allMovies.push_back(movie.get());
    }

    sortMovies(allMovies, sortOption);
    return allMovies;
}

void MovieManager::sortMovies(std::vector<const Movie *> &moviesToSort, MovieSortOption sortOption) const
{
    switch (sortOption)
    {
    case MovieSortOption::Rating:
        std::sort(moviesToSort.begin(), moviesToSort.end(), comesBeforeByRating);
        break;
    case MovieSortOption::Title:
        std::sort(moviesToSort.begin(), moviesToSort.end(), comesBeforeByTitle);
        break;
    case MovieSortOption::Latest:
        std::sort(moviesToSort.begin(), moviesToSort.end(), comesBeforeByLatest);
        break;
    }
}

std::vector<const Movie *> MovieManager::searchMovies(const std::string &keyword) const
{
    std::vector<const Movie *> matchedMovies;

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        if (containsIgnoringAsciiCase(movie->getTitle(), keyword) ||
            containsIgnoringAsciiCase(movie->getGenre(), keyword))
        {
            matchedMovies.push_back(movie.get());
        }
    }

    return matchedMovies;
}

std::vector<const Movie *> MovieManager::searchMoviesByTitle(const std::string &keyword) const
{
    std::vector<const Movie *> matchedMovies;

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        if (containsIgnoringAsciiCase(movie->getTitle(), keyword))
        {
            matchedMovies.push_back(movie.get());
        }
    }

    return matchedMovies;
}

std::vector<const Movie *> MovieManager::filterMoviesByGenre(const std::string &genre) const
{
    std::vector<const Movie *> allMovies;
    std::vector<const Movie *> matchedMovies;

    allMovies.reserve(movies.size());
    for (const std::unique_ptr<Movie> &movie : movies)
    {
        allMovies.push_back(movie.get());
    }

    std::copy_if(allMovies.begin(), allMovies.end(), std::back_inserter(matchedMovies),
                 [&genre](const Movie *movie)
                 {
                     return movie->getGenre() == genre;
                 });

    return matchedMovies;
}

double MovieManager::getAverageRating() const
{
    const int totalRatingCount = countTotalRatings(movies);

    if (totalRatingCount == 0)
    {
        throw std::runtime_error("등록된 평점이 없습니다");
    }

    const double totalScore = std::accumulate(movies.begin(), movies.end(), 0.0,
                                              [](double total, const std::unique_ptr<Movie> &movie)
                                              {
                                                  return total + movie->getAverageRating() * movie->getRatingCount();
                                              });

    return totalScore / totalRatingCount;
}

std::map<std::string, GenreStatistics> MovieManager::getGenreStatistics() const
{
    std::map<std::string, GenreStatistics> statisticsByGenre;
    std::map<std::string, double> scoreSumByGenre;

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        GenreStatistics &statistics = statisticsByGenre[movie->getGenre()];
        statistics.movieCount++;
        statistics.ratingCount += movie->getRatingCount();
        scoreSumByGenre[movie->getGenre()] += movie->getAverageRating() * movie->getRatingCount();
    }

    for (auto &[genre, statistics] : statisticsByGenre)
    {
        if (statistics.ratingCount > 0)
        {
            statistics.averageRating = scoreSumByGenre[genre] / statistics.ratingCount;
        }
    }

    return statisticsByGenre;
}

std::vector<const Movie *> MovieManager::getTopRatedMovies(int limit) const
{
    std::vector<const Movie *> sortedMovies;

    if (limit <= 0)
    {
        return sortedMovies;
    }

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        if (movie->getRatingCount() > 0)
        {
            sortedMovies.push_back(movie.get());
        }
    }

    std::sort(sortedMovies.begin(), sortedMovies.end(),
              [](const Movie *left, const Movie *right)
              {
                  if (left->getAverageRating() != right->getAverageRating())
                  {
                      return left->getAverageRating() > right->getAverageRating();
                  }

                  if (left->getRatingCount() != right->getRatingCount())
                  {
                      return left->getRatingCount() > right->getRatingCount();
                  }

                  return left->getId() < right->getId();
              });

    if (static_cast<int>(sortedMovies.size()) > limit)
    {
        sortedMovies.resize(limit);
    }

    return sortedMovies;
}

void MovieManager::exportStatisticsToCsv(const std::string &filename, int topMovieLimit) const
{
    if (topMovieLimit <= 0)
    {
        throw std::invalid_argument("Top N 영화 수는 1 이상이어야 합니다");
    }

    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("통계 CSV 파일을 저장할 수 없습니다: " + filename);
    }

    writeCsvRow(file, {"section", "rank", "name", "movie_id", "genre", "release_year", "movie_count", "rating_count", "average_rating"});

    const int totalRatingCount = countTotalRatings(movies);
    writeCsvRow(file, {"overall", "", "전체 평균 평점", "", "", "", "", std::to_string(totalRatingCount),
                       totalRatingCount > 0 ? formatCsvScore(getAverageRating()) : "0.00"});

    const auto statisticsByGenre = getGenreStatistics();
    for (const auto &[genre, statistics] : statisticsByGenre)
    {
        writeCsvRow(file, {"genre", "", genre, "", "", "", std::to_string(statistics.movieCount),
                           std::to_string(statistics.ratingCount), formatCsvScore(statistics.averageRating)});
    }

    const auto topMovies = getTopRatedMovies(topMovieLimit);
    int rank = 1;
    for (const Movie *movie : topMovies)
    {
        writeCsvRow(file, {"top_movie", std::to_string(rank), movie->getTitle(), std::to_string(movie->getId()),
                           movie->getGenre(), std::to_string(movie->getReleaseYear()), "",
                           std::to_string(movie->getRatingCount()), formatCsvScore(movie->getAverageRating())});
        rank++;
    }
}

void MovieManager::printAllMovies() const
{
    ConsoleView::printMovieTable(getAllMovies(MovieSortOption::Rating));
}

void MovieManager::printMoviesSortedByRating() const
{
    ConsoleView::printMovieTable(getAllMovies(MovieSortOption::Rating));
}

void MovieManager::rebuildRatingsFrom(const std::vector<Rating> &ratings)
{
    for (const std::unique_ptr<Movie> &movie : movies)
    {
        movie->resetRatings();
    }

    for (const Rating &rating : ratings)
    {
        Movie *movie = findMovieById(rating.getMovieId());
        if (movie != nullptr)
        {
            movie->addRating(rating.getScore());
        }
    }
}

std::size_t MovieManager::getMovieCount() const
{
    // vector 안에 들어있는 영화 개수 반환
    return size();
}

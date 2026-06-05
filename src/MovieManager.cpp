#include "MovieManager.h"

#include <algorithm> // find, sort 같은 알고리즘 함수 쓰려고 넣은거임
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "MovieConstants.h"

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
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

std::vector<const Movie *> MovieManager::searchMoviesByTitle(const std::string &keyword) const
{
    std::vector<const Movie *> matchedMovies; // 검색 결과 담아둘 벡터

    // 제목 안에 키워드가 포함되어 있으면 결과에 넣는거임
    for (const std::unique_ptr<Movie> &movie : movies)
    {
        if (movie->getTitle().find(keyword) != std::string::npos)
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

void MovieManager::printAllMovies() const
{
    // 영화 정보 출력은 Movie의 operator<<를 그대로 활용하는거임
    for (const std::unique_ptr<Movie> &movie : movies)
    {
        std::cout << *movie << std::endl;
    }
}

void MovieManager::printMoviesSortedByRating() const
{
    std::vector<const Movie *> sortedMovies;
    sortedMovies.reserve(movies.size());

    for (const std::unique_ptr<Movie> &movie : movies)
    {
        sortedMovies.push_back(movie.get());
    }

    std::sort(sortedMovies.begin(), sortedMovies.end(),
              [](const Movie *left, const Movie *right)
              {
                  return *left < *right;
              });

    for (const Movie *movie : sortedMovies)
    {
        std::cout << *movie << std::endl;
    }
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

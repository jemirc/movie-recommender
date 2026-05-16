#include "MovieManager.h"

#include <algorithm> // find, sort 같은 알고리즘 함수 쓰려고 넣은거임
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
}
}

MovieManager::MovieManager()
    : nextId(1)
{
}

int MovieManager::addMovie(const std::string &title, const std::string &genre, int year)
{
    int id = nextId;
    movies.push_back(Movie(id, title, genre, year));
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
        return;
    }

    std::string line;
    int maxId = 0;

    while (std::getline(file, line))
    {
        if (shouldSkipLine(line))
        {
            continue;
        }

        std::stringstream ss(line);
        std::string idText;
        std::string title;
        std::string genre;
        std::string yearText;

        if (!std::getline(ss, idText, ',') ||
            !std::getline(ss, title, ',') ||
            !std::getline(ss, genre, ',') ||
            !std::getline(ss, yearText))
        {
            continue;
        }

        try
        {
            const int id = std::stoi(idText);
            const int year = std::stoi(yearText);
            movies.push_back(Movie(id, title, genre, year));

            if (id > maxId)
            {
                maxId = id;
            }
        }
        catch (const std::exception &)
        {
            continue;
        }
    }

    nextId = maxId + 1;
}

void MovieManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        return;
    }

    for (const Movie &movie : movies)
    {
        file << movie.getId() << ','
             << movie.getTitle() << ','
             << movie.getGenre() << ','
             << movie.getReleaseYear() << '\n';
    }
}

std::size_t MovieManager::size() const
{
    return movies.size();
}

Movie *MovieManager::findMovieById(int id)
{
    // id로 순차 탐색해서 실제 객체 주소를 돌려주는거임
    for (Movie &movie : movies)
    {
        if (movie.getId() == id)
        {
            return &movie;
        }
    }

    return nullptr;
}

const Movie *MovieManager::findMovieById(int id) const
{
    // const 함수에서는 const 포인터로 돌려줘야 안전한거임
    for (const Movie &movie : movies)
    {
        if (movie.getId() == id)
        {
            return &movie;
        }
    }

    return nullptr;
}

std::vector<const Movie *> MovieManager::searchMoviesByTitle(const std::string &keyword) const
{
    std::vector<const Movie *> matchedMovies; // 검색 결과 담아둘 벡터

    // 제목 안에 키워드가 포함되어 있으면 결과에 넣는거임
    for (const Movie &movie : movies)
    {
        if (movie.getTitle().find(keyword) != std::string::npos)
        {
            matchedMovies.push_back(&movie);
        }
    }

    return matchedMovies;
}

void MovieManager::printAllMovies() const
{
    // 영화 정보 출력은 Movie의 operator<<를 그대로 활용하는거임
    for (const Movie &movie : movies)
    {
        std::cout << movie << std::endl;
    }
}

void MovieManager::printMoviesSortedByRating() const
{
    std::vector<Movie> sortedMovies = movies;

    std::sort(sortedMovies.begin(), sortedMovies.end());

    for (const Movie &movie : sortedMovies)
    {
        std::cout << movie << std::endl;
    }
}

std::size_t MovieManager::getMovieCount() const
{
    // vector 안에 들어있는 영화 개수 반환
    return size();
}

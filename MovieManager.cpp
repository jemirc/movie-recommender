#include "MovieManager.h"

#include <algorithm> // find, sort 같은 알고리즘 함수 쓰려고 넣은거임
#include <iostream>

bool MovieManager::addMovie(const Movie &movie)
{
    // 같은 영화가 이미 있으면 또 넣지 않으려고 검사하는거임
    if (std::find(movies.begin(), movies.end(), movie) != movies.end())
    {
        return false;
    }

    // 중복 아니면 목록에 추가
    movies.push_back(movie);
    return true;
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

void MovieManager::sortMovies()
{
    // Movie에 구현한 operator< 기준으로 바로 정렬 가능
    std::sort(movies.begin(), movies.end());
}

const std::vector<Movie> &MovieManager::getMovies() const
{
    // 복사하지 말고 참조로 넘겨주는거임
    return movies;
}

std::size_t MovieManager::getMovieCount() const
{
    // vector 안에 들어있는 영화 개수 반환
    return movies.size();
}

#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "Movie.h"

// Movie 객체 여러개를 한곳에서 관리하려고 만든 클래스
class MovieManager
{
private:
    std::vector<Movie> movies; // 영화 목록은 vector로 들고가는거임
    int nextId;                // 다음에 추가될 영화 ID

public:
    MovieManager();

    int addMovie(const std::string &title, const std::string &genre, int year);       // ID는 자동으로 부여
    bool addMovie(const Movie &movie);                                                // 영화 추가할때 중복도 같이 검사하려는거임
    Movie *findMovieById(int id);                                                     // 수정 가능한 영화 찾을때
    const Movie *findMovieById(int id) const;                                         // 읽기 전용으로 찾을때
    std::vector<const Movie *> searchMoviesByTitle(const std::string &keyword) const; // 제목에 키워드가 들어가는 영화들 찾기
    void printAllMovies() const;                                                      // 전체 영화 목록 출력
    void printMoviesSortedByRating() const;                                           // 평점순으로 정렬해서 출력
    void sortMovies();                                                                // Movie의 operator< 기준으로 정렬

    const std::vector<Movie> &getMovies() const; // 전체 영화 목록 참조로 넘겨주기
    std::size_t getMovieCount() const;           // 현재 영화가 몇개 있는지 확인용
};

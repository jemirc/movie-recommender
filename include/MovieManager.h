#pragma once

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "BaseManager.h"
#include "Movie.h"
#include "Rating.h"

struct GenreStatistics
{
    int movieCount = 0;
    int ratingCount = 0;
    double averageRating = 0.0;
};

enum class MovieSortOption
{
    Rating,
    Title,
    Latest
};

std::string getMovieSortOptionLabel(MovieSortOption sortOption);

// Movie 객체 여러개를 한곳에서 관리하려고 만든 클래스
class MovieManager : public BaseManager
{
private:
    std::vector<std::unique_ptr<Movie>> movies; // vector는 유지하되 Movie 객체 주소는 안정적으로 보관
    int nextId;                // 다음에 추가될 영화 ID

public:
    MovieManager();

    int addMovie(const std::string &title, const std::string &genre, int year);       // ID는 자동으로 부여
    void loadFromFile(const std::string &filename) override;
    void saveToFile(const std::string &filename) const override;
    std::size_t size() const override;
    Movie *findMovieById(int id);                                                     // 수정 가능한 영화 찾을때
    const Movie *findMovieById(int id) const;                                         // 읽기 전용으로 찾을때
    std::vector<const Movie *> getAllMovies(MovieSortOption sortOption) const;        // 현재 영화 목록을 정렬 기준에 맞게 반환
    void sortMovies(std::vector<const Movie *> &movies, MovieSortOption sortOption) const;
    std::vector<const Movie *> searchMovies(const std::string &keyword) const;        // 제목/장르에 키워드가 들어가는 영화들 찾기
    std::vector<const Movie *> searchMoviesByTitle(const std::string &keyword) const; // 기존 제목 검색 호환용
    std::vector<const Movie *> filterMoviesByGenre(const std::string &genre) const;   // 장르가 같은 영화만 찾기
    double getAverageRating() const;                                                  // 전체 평점 평균 계산
    std::map<std::string, GenreStatistics> getGenreStatistics() const;                // 장르별 영화/평점 통계 계산
    std::vector<const Movie *> getTopRatedMovies(int limit) const;                    // 평균 평점 상위 영화 찾기
    void exportStatisticsToCsv(const std::string &filename, int topMovieLimit) const; // 통계 결과를 CSV 파일로 저장
    void printAllMovies() const;                                                      // 전체 영화 목록 출력
    void printMoviesSortedByRating() const;                                           // 평점순으로 정렬해서 출력
    void rebuildRatingsFrom(const std::vector<Rating> &ratings);
    std::size_t getMovieCount() const;           // 현재 영화가 몇개 있는지 확인용
};

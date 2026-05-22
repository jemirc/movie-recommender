#pragma once
#include <ostream>
#include <string>

class Movie
{
private:
    int id;
    std::string title;
    std::string genre;
    int releaseYear;
    double totalRating; // averageRating 제거 하신거임
    int ratingCount;    // 추가

public:
    Movie(); // 기본 생성자 추가
    Movie(int id, const std::string &title,
          const std::string &genre, int year);

    int getId() const;
    std::string getTitle() const; // getter들에 const들 다 붙여주심
    std::string getGenre() const;
    int getReleaseYear() const;      // getYear → getReleaseYear
    double getAverageRating() const; // getRating → getAverageRating
    int getRatingCount() const;      // 추가

    void addRating(double r); // 추가
    void resetRatings();
    bool updateRating(double oldRating, double newRating);
    bool operator==(const Movie &other) const;
    bool operator<(const Movie &other) const;

    // friends로 operator<< 선언해서 외부에서 private 접근 가능하게
    friend std::ostream &operator<<(std::ostream &os, const Movie &movie);
};

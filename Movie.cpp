#include "Movie.h"
// 기본 생성자도 만들어주신거임
Movie::Movie()
    : id(0), releaseYear(0),
      totalRating(0.0), ratingCount(0) {}

// 4인자 생성자
Movie::Movie(int id, const std::string &title,
             const std::string &genre, int year)
    : id(id), title(title), genre(genre),
      releaseYear(year),
      totalRating(0.0), ratingCount(0) {}

int Movie::getId() const { return id; }
std::string Movie::getTitle() const { return title; }
std::string Movie::getGenre() const { return genre; }
int Movie::getReleaseYear() const { return releaseYear; }
int Movie::getRatingCount() const { return ratingCount; }

// 이전것도 기억할 수 있게 구현 다 하신거임
double Movie::getAverageRating() const
{
    if (ratingCount == 0)
        return 0.0; // 0 나눗셈 방어
    return totalRating / ratingCount;
}

void Movie::addRating(double r)
{
    if (r < 0.0 || r > 5.0)
        return; // 유효성 검사
    totalRating += r;
    ratingCount++;
}

bool Movie::updateRating(double oldRating, double newRating)
{
    if (ratingCount == 0)
        return false;

    if (oldRating < 0.0 || oldRating > 5.0 || newRating < 0.0 || newRating > 5.0)
        return false;

    totalRating += newRating - oldRating;
    return true;
}

bool Movie::operator==(const Movie &other) const
{
    return id == other.id;
}

bool Movie::operator<(const Movie &other) const
{
    if (getAverageRating() != other.getAverageRating())
    {
        return getAverageRating() > other.getAverageRating();
    }

    return id < other.id;
}

std::ostream &operator<<(std::ostream &os, const Movie &movie)
{
    os << "[영화] "
       << "ID: " << movie.id
       << " | 제목: " << movie.title
       << " | 장르: " << movie.genre
       << " | 개봉 연도: " << movie.releaseYear
       << " | 평균 평점: " << movie.getAverageRating()
       << " | 평점 수: " << movie.ratingCount << "건";
    return os;
}

#include <iostream>
#include <vector>
#include "Movie.h"
#include "User.h"
#include "Rating.h"

using namespace std;

int main()
{
    vector<Movie> movies;
    vector<User> users;
    vector<Rating> ratings;

    // 1. 초기 데이터 세팅
    movies.emplace_back(1, "기생충", "드라마", 2019);
    movies.emplace_back(2, "인터스텔라", "SF", 2014);

    users.emplace_back(1, "이재민", "ljm00530@soongsil.ac.kr");
    users.emplace_back(2, "김하람", "haram@soongsil.ac.kr");

    cout << "------ [1] 초기 영화 상태 (0 나눗셈 방어 확인) ------" << endl;
    // 평점이 없을 때 getAverageRating()이 0을 잘 반환하는지 확인
    cout << movies[1] << endl;

    cout << "\n------ [2] 정상적인 평점 입력 및 반영 ------" << endl;
    // 이재민이 인터스텔라에 4.9점
    ratings.emplace_back(1, 2, 4.9);
    movies[1].addRating(4.9);

    // 김하람이 기생충에 5.0점
    ratings.emplace_back(2, 1, 5.0);
    movies[0].addRating(5.0);
    cout << "평점 반영 완료!\n";

    cout << "\n------ [3] 잘못된 평점 입력 케이스 ------" << endl;
    // 이재민이 기생충에 7.5점 입력 시도 (Rating 생성자에서 걸러지는지 확인)
    cout << ">> 7.5점 입력 시도 중..." << endl;
    ratings.emplace_back(1, 1, 7.5);
    // 주의: 실제 영화(movies[0])에는 잘못된 점수를 addRating 하지 않음

    cout << "\n------ [4] User 전체 목록 출력 ------" << endl;
    for (const User &u : users)
    {
        u.display();
    }

    cout << "\n------ [5] Rating 전체 내역 출력 ------" << endl;
    for (const Rating &r : ratings)
    {
        r.display();
    }

    cout << "\n------ [6] Movie 최종 상태 출력 ------" << endl;
    for (const Movie &m : movies)
    {
        cout << m << endl;
    }

    return 0;
}

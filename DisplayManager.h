#pragma once

#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"

// main의 큰 switch 안쪽에서 실행될 메뉴별 입출력만 따로 맡는 클래스
class DisplayManager
{
private:
    MovieManager &movieManager;   // 영화 데이터 처리는 기존 매니저한테 맡기는거임
    UserManager &userManager;     // 사용자 데이터 처리용
    RatingManager &ratingManager; // 평점 데이터 처리용

    void clearInput() const; // cin에 남은 개행이나 잘못된 입력 정리하는 용도

public:
    DisplayManager(MovieManager &movieManager, UserManager &userManager, RatingManager &ratingManager);

    void addMovieMenu();                // 1번 메뉴 안쪽 구현
    void searchMovieMenu() const;       // 2번 메뉴 안쪽 구현
    void printAllMoviesMenu() const;    // 3번 메뉴 안쪽 구현
    void printSortedMoviesMenu() const; // 4번 메뉴 안쪽 구현
    void addUserMenu();                 // 5번 메뉴 안쪽 구현
    void printAllUsersMenu() const;     // 6번 메뉴 안쪽 구현
    void addRatingMenu();               // 7번 메뉴 안쪽 구현
    void printMovieRatingsMenu() const; // 8번 메뉴 안쪽 구현
    void recommendMovieMenu() const;    // 9번 메뉴 안쪽 구현
};

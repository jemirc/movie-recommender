#include "DisplayManager.h"

#include <iostream>
#include <limits>
#include <string>

DisplayManager::DisplayManager(MovieManager &movieManager, UserManager &userManager, RatingManager &ratingManager)
    : movieManager(movieManager), userManager(userManager), ratingManager(ratingManager)
{
}

void DisplayManager::clearInput() const
{
    // 숫자 입력 뒤에 남은 개행 때문에 getline이 바로 끝나는거 방지하는거임
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void DisplayManager::addMovieMenu()
{
    // 영화 추가에 필요한 값 입력받고 MovieManager에 실제 추가를 맡기는거임
    int year = 0;
    std::string title;
    std::string genre;

    clearInput();

    std::cout << "제목: ";
    std::getline(std::cin, title);

    std::cout << "장르: ";
    std::getline(std::cin, genre);

    std::cout << "개봉 연도: ";
    std::cin >> year;
    clearInput();

    int id = movieManager.addMovie(title, genre, year);
    std::cout << "영화가 추가되었습니다. 영화 ID: " << id << std::endl;
}

void DisplayManager::searchMovieMenu() const
{
    // 제목 키워드만 입력받고 검색 자체는 MovieManager 검색 함수 쓰는거임
    std::string keyword;

    std::cout << "검색할 제목 키워드: ";
    clearInput();
    std::getline(std::cin, keyword);

    const auto matchedMovies = movieManager.searchMoviesByTitle(keyword);
    if (matchedMovies.empty())
    {
        std::cout << "검색 결과가 없습니다." << std::endl;
        return;
    }

    for (const Movie *movie : matchedMovies)
    {
        std::cout << *movie << std::endl;
    }
}

void DisplayManager::printAllMoviesMenu() const
{
    // 비어있을때 메시지만 여기서 처리하고, 실제 출력은 MovieManager한테 맡기는거임
    if (movieManager.getMovieCount() == 0)
    {
        std::cout << "등록된 영화가 없습니다." << std::endl;
        return;
    }

    movieManager.printAllMovies();
}

void DisplayManager::printSortedMoviesMenu() const
{
    // 평점순 출력도 목록이 있을때만 매니저 출력 함수 호출하는 구조임
    if (movieManager.getMovieCount() == 0)
    {
        std::cout << "등록된 영화가 없습니다." << std::endl;
        return;
    }

    movieManager.printMoviesSortedByRating();
}

void DisplayManager::addUserMenu()
{
    // 사용자 정보 입력받고 UserManager에 추가 요청하는거임
    std::string name;
    std::string email;

    clearInput();

    std::cout << "이름: ";
    std::getline(std::cin, name);

    std::cout << "이메일: ";
    std::getline(std::cin, email);

    int id = userManager.addUser(name, email);
    std::cout << "사용자가 추가되었습니다. 사용자 ID: " << id << std::endl;
}

void DisplayManager::printAllUsersMenu() const
{
    // 사용자 목록이 비었는지 확인한 뒤 출력은 UserManager 함수 재사용
    if (userManager.getUserCount() == 0)
    {
        std::cout << "등록된 사용자가 없습니다." << std::endl;
        return;
    }

    userManager.printAllUsers();
}

void DisplayManager::addRatingMenu()
{
    // 평점 등록은 사용자/영화 ID 검증까지 RatingManager가 처리하게 넘기는거임
    int userId = 0;
    int movieId = 0;
    double score = 0.0;

    std::cout << "사용자 ID: ";
    std::cin >> userId;
    std::cout << "영화 ID: ";
    std::cin >> movieId;
    std::cout << "평점(0.0 ~ 5.0): ";
    std::cin >> score;
    clearInput();

    if (ratingManager.addRating(userId, movieId, score, userManager, movieManager))
    {
        if (ratingManager.wasLastRatingUpdated())
        {
            std::cout << "기존 평점이 수정되었습니다." << std::endl;
        }
        else
        {
            std::cout << "평점이 등록되었습니다." << std::endl;
        }
        return;
    }

    std::cout << "평점을 등록할 수 없습니다. 사용자/영화 ID와 평점 범위를 확인하세요." << std::endl;
}

void DisplayManager::printMovieRatingsMenu() const
{
    // 영화가 있는지 먼저 확인하고, 있으면 해당 영화 평점 목록 출력하는거임
    int movieId = 0;

    std::cout << "조회할 영화 ID: ";
    std::cin >> movieId;
    clearInput();

    const Movie *movie = movieManager.findMovieById(movieId);
    if (movie == nullptr)
    {
        std::cout << "해당 영화가 존재하지 않습니다." << std::endl;
        return;
    }

    std::cout << *movie << std::endl;

    const auto matchedRatings = ratingManager.getRatingsByMovieId(movieId);
    if (matchedRatings.empty())
    {
        std::cout << "등록된 평점이 없습니다." << std::endl;
        return;
    }

    ratingManager.printRatingsByMovieId(movieId);
}

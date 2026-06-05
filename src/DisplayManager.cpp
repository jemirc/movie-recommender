#include "DisplayManager.h"

#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "MovieConstants.h"
#include "Recommender.h"

namespace
{
bool readIntValue(const std::string &prompt, int &value)
{
    while (true)
    {
        std::string line;

        std::cout << prompt;
        if (!std::getline(std::cin, line))
        {
            return false;
        }

        std::istringstream input(line);
        char extra = '\0';
        if ((input >> value) && !(input >> extra))
        {
            return true;
        }

        std::cout << "정수로 입력해 주세요." << std::endl;
    }
}

bool readDoubleValue(const std::string &prompt, double &value)
{
    while (true)
    {
        std::string line;

        std::cout << prompt;
        if (!std::getline(std::cin, line))
        {
            return false;
        }

        std::istringstream input(line);
        char extra = '\0';
        if ((input >> value) && !(input >> extra))
        {
            return true;
        }

        std::cout << "숫자로 입력해 주세요." << std::endl;
    }
}
}

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

    if (!readIntValue("개봉 연도: ", year))
    {
        std::cout << "입력이 종료되어 영화 추가를 취소합니다." << std::endl;
        return;
    }

    if (year <= 0)
    {
        std::cout << "개봉 연도는 1 이상의 숫자로 입력해 주세요." << std::endl;
        return;
    }

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
    std::ostringstream scorePrompt;

    scorePrompt << "평점(" << MovieConstants::MIN_RATING_SCORE
                << " ~ " << MovieConstants::MAX_RATING_SCORE << "): ";

    clearInput();

    if (!readIntValue("사용자 ID: ", userId) ||
        !readIntValue("영화 ID: ", movieId) ||
        !readDoubleValue(scorePrompt.str(), score))
    {
        std::cout << "입력이 종료되어 평점 등록을 취소합니다." << std::endl;
        return;
    }

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

    clearInput();

    if (!readIntValue("조회할 영화 ID: ", movieId))
    {
        std::cout << "입력이 종료되어 조회를 취소합니다." << std::endl;
        return;
    }

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

void DisplayManager::recommendMovieMenu() const
{
    int userId = 0;

    clearInput();

    if (!readIntValue("추천받을 사용자 ID: ", userId))
    {
        std::cout << "입력이 종료되어 추천을 취소합니다." << std::endl;
        return;
    }

    if (userManager.findUserById(userId) == nullptr)
    {
        std::cout << "해당 사용자가 없습니다." << std::endl;
        return;
    }

    if (ratingManager.findByUser(userId).empty())
    {
        std::cout << "해당 사용자의 평점이 없습니다." << std::endl;
        return;
    }

    Recommender recommender(movieManager, ratingManager);
    const auto recommendations = recommender.recommend(userId,
                                                       MovieConstants::DEFAULT_TOP_K_USERS,
                                                       MovieConstants::DEFAULT_TOP_N_MOVIES);

    if (recommendations.empty())
    {
        std::cout << "추천할 영화가 없습니다." << std::endl;
        return;
    }

    std::cout << "=== 추천 영화 ===" << std::endl;
    for (const auto &[movie, score] : recommendations)
    {
        std::cout << *movie << " | 추천 점수: " << score << std::endl;
    }
}

void DisplayManager::recommendMovieByGenreMenu() const
{
    int userId = 0;
    std::string genre;

    clearInput();

    if (!readIntValue("추천받을 사용자 ID: ", userId))
    {
        std::cout << "입력이 종료되어 추천을 취소합니다." << std::endl;
        return;
    }

    std::cout << "추천 장르: ";
    std::getline(std::cin, genre);

    if (genre.empty())
    {
        std::cout << "장르를 입력해 주세요." << std::endl;
        return;
    }

    if (userManager.findUserById(userId) == nullptr)
    {
        std::cout << "해당 사용자가 없습니다." << std::endl;
        return;
    }

    if (ratingManager.findByUser(userId).empty())
    {
        std::cout << "해당 사용자의 평점이 없습니다." << std::endl;
        return;
    }

    if (movieManager.filterMoviesByGenre(genre).empty())
    {
        std::cout << "해당 장르의 영화가 없습니다." << std::endl;
        return;
    }

    Recommender recommender(movieManager, ratingManager);
    const auto recommendations = recommender.recommend(userId,
                                                       MovieConstants::DEFAULT_TOP_K_USERS,
                                                       MovieConstants::DEFAULT_TOP_N_MOVIES,
                                                       genre);

    if (recommendations.empty())
    {
        std::cout << "해당 장르에서 추천할 영화가 없습니다." << std::endl;
        return;
    }

    std::cout << "=== " << genre << " 추천 영화 ===" << std::endl;
    for (const auto &[movie, score] : recommendations)
    {
        std::cout << *movie << " | 추천 점수: " << score << std::endl;
    }
}

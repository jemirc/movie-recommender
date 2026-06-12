#include "DisplayManager.h"

#include <exception>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include <termcolor/termcolor.hpp>

#include "ConsoleView.h"
#include "MovieConstants.h"
#include "Recommender.h"

namespace
{
constexpr int DIVIDER_WIDTH = 62;

void printDivider(char fill)
{
    std::cout << termcolor::bright_blue
              << std::string(DIVIDER_WIDTH, fill)
              << termcolor::reset << std::endl;
}

void printSectionHeader(const std::string &title)
{
    std::cout << std::endl;
    printDivider('=');
    std::cout << termcolor::bold << termcolor::bright_cyan
              << title
              << termcolor::reset << std::endl;
    printDivider('-');
}

void printInfoMessage(const std::string &message)
{
    std::cout << termcolor::bright_cyan << "[안내] "
              << termcolor::reset << message << std::endl;
}

void printSuccessMessage(const std::string &message)
{
    std::cout << termcolor::bright_green << "[완료] "
              << termcolor::reset << message << std::endl;
}

void printWarningMessage(const std::string &message)
{
    std::cout << termcolor::bright_yellow << "[확인] "
              << termcolor::reset << message << std::endl;
}

bool readIntValue(const std::string &prompt, int &value)
{
    while (true)
    {
        std::string line;

        std::cout << termcolor::bright_cyan << prompt << termcolor::reset;
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

        printWarningMessage("정수로 입력해 주세요.");
    }
}

bool readDoubleValue(const std::string &prompt, double &value)
{
    while (true)
    {
        std::string line;

        std::cout << termcolor::bright_cyan << prompt << termcolor::reset;
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

        printWarningMessage("숫자로 입력해 주세요.");
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

bool DisplayManager::readRecommendableUserId(int &userId) const
{
    if (!readIntValue("추천받을 사용자 ID: ", userId))
    {
        printInfoMessage("입력이 종료되어 추천을 취소합니다.");
        return false;
    }

    if (userManager.findUserById(userId) == nullptr)
    {
        printWarningMessage("해당 사용자가 없습니다.");
        return false;
    }

    if (ratingManager.findByUser(userId).empty())
    {
        printInfoMessage("해당 사용자의 평점이 없습니다.");
        return false;
    }

    return true;
}

void DisplayManager::printMainMenu() const
{
    std::cout << std::endl;
    printDivider('=');
    std::cout << termcolor::bold << termcolor::bright_cyan
              << "Movie Recommender"
              << termcolor::reset << std::endl;
    printDivider('-');
    std::cout << termcolor::bright_green << "현재 데이터  "
              << termcolor::reset
              << "영화 " << movieManager.getMovieCount() << "편"
              << " | 사용자 " << userManager.getUserCount() << "명"
              << " | 평점 " << ratingManager.getRatingCount() << "건" << std::endl;
    printDivider('-');

    std::cout << termcolor::bright_yellow << "[영화]" << termcolor::reset << std::endl;
    std::cout << "  1. 영화 추가" << std::endl;
    std::cout << "  2. 제목으로 검색" << std::endl;
    std::cout << "  3. 전체 목록 출력" << std::endl;
    std::cout << "  4. 평점순 정렬 출력" << std::endl
              << std::endl;

    std::cout << termcolor::bright_yellow << "[사용자]" << termcolor::reset << std::endl;
    std::cout << "  5. 사용자 추가" << std::endl;
    std::cout << "  6. 사용자 목록 출력" << std::endl
              << std::endl;

    std::cout << termcolor::bright_yellow << "[평점]" << termcolor::reset << std::endl;
    std::cout << "  7. 평점 입력" << std::endl;
    std::cout << "  8. 영화별 평점 보기" << std::endl
              << std::endl;

    std::cout << termcolor::bright_yellow << "[추천]" << termcolor::reset << std::endl;
    std::cout << "  9. 사용자별 영화 추천" << std::endl;
    std::cout << " 10. 장르 필터 추천" << std::endl
              << std::endl;

    std::cout << termcolor::bright_yellow << "[통계]" << termcolor::reset << std::endl;
    std::cout << " 11. 통계 보기" << std::endl
              << std::endl;

    std::cout << "  0. 종료" << std::endl;
    printDivider('=');
    std::cout << termcolor::bright_cyan << "선택 > " << termcolor::reset;
}

void DisplayManager::addMovieMenu()
{
    // 영화 추가에 필요한 값 입력받고 MovieManager에 실제 추가를 맡기는거임
    int year = 0;
    std::string title;
    std::string genre;

    printSectionHeader("영화 추가");
    clearInput();

    std::cout << "제목: ";
    std::getline(std::cin, title);

    std::cout << "장르: ";
    std::getline(std::cin, genre);

    if (title.empty() || genre.empty())
    {
        printWarningMessage("제목과 장르는 비워둘 수 없습니다.");
        return;
    }

    if (!readIntValue("개봉 연도: ", year))
    {
        printInfoMessage("입력이 종료되어 영화 추가를 취소합니다.");
        return;
    }

    if (year <= 0)
    {
        printWarningMessage("개봉 연도는 1 이상의 숫자로 입력해 주세요.");
        return;
    }

    int id = movieManager.addMovie(title, genre, year);
    printSuccessMessage("영화가 추가되었습니다. 영화 ID: " + std::to_string(id));
}

void DisplayManager::searchMovieMenu() const
{
    std::string keyword;

    printSectionHeader("영화 검색");
    printInfoMessage("제목 또는 장르를 대소문자 구분 없이 부분 검색합니다.");
    std::cout << "검색 키워드: ";
    clearInput();
    std::getline(std::cin, keyword);

    if (keyword.empty())
    {
        printWarningMessage("검색어를 입력해 주세요.");
        return;
    }

    const auto matchedMovies = movieManager.searchMovies(keyword);
    if (matchedMovies.empty())
    {
        printInfoMessage("검색 결과가 없습니다.");
        return;
    }

    printInfoMessage("검색 결과: " + std::to_string(matchedMovies.size()) + "건");
    ConsoleView::printMovieTable(matchedMovies);
}

void DisplayManager::printAllMoviesMenu() const
{
    // 비어있을때 메시지만 여기서 처리하고, 실제 출력은 MovieManager한테 맡기는거임
    printSectionHeader("전체 영화 목록");
    if (movieManager.getMovieCount() == 0)
    {
        printInfoMessage("등록된 영화가 없습니다.");
        return;
    }

    std::cout << "총 " << movieManager.getMovieCount() << "편" << std::endl;
    movieManager.printAllMovies();
}

void DisplayManager::printSortedMoviesMenu() const
{
    // 평점순 출력도 목록이 있을때만 매니저 출력 함수 호출하는 구조임
    printSectionHeader("평점순 영화 목록");
    if (movieManager.getMovieCount() == 0)
    {
        printInfoMessage("등록된 영화가 없습니다.");
        return;
    }

    movieManager.printMoviesSortedByRating();
}

void DisplayManager::addUserMenu()
{
    // 사용자 정보 입력받고 UserManager에 추가 요청하는거임
    std::string name;
    std::string email;

    printSectionHeader("사용자 추가");
    clearInput();

    std::cout << "이름: ";
    std::getline(std::cin, name);

    std::cout << "이메일: ";
    std::getline(std::cin, email);

    if (name.empty() || email.empty())
    {
        printWarningMessage("이름과 이메일은 비워둘 수 없습니다.");
        return;
    }

    int id = userManager.addUser(name, email);
    printSuccessMessage("사용자가 추가되었습니다. 사용자 ID: " + std::to_string(id));
}

void DisplayManager::printAllUsersMenu() const
{
    // 사용자 목록이 비었는지 확인한 뒤 출력은 UserManager 함수 재사용
    printSectionHeader("사용자 목록");
    if (userManager.getUserCount() == 0)
    {
        printInfoMessage("등록된 사용자가 없습니다.");
        return;
    }

    std::cout << "총 " << userManager.getUserCount() << "명" << std::endl;
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

    printSectionHeader("평점 입력");
    printInfoMessage("사용자 ID와 영화 ID를 확인한 뒤 평점을 입력하세요.");
    clearInput();

    if (!readIntValue("사용자 ID: ", userId) ||
        !readIntValue("영화 ID: ", movieId) ||
        !readDoubleValue(scorePrompt.str(), score))
    {
        printInfoMessage("입력이 종료되어 평점 등록을 취소합니다.");
        return;
    }

    if (ratingManager.addRating(userId, movieId, score, userManager, movieManager))
    {
        if (ratingManager.wasLastRatingUpdated())
        {
            printSuccessMessage("기존 평점이 수정되었습니다.");
        }
        else
        {
            printSuccessMessage("평점이 등록되었습니다.");
        }
        return;
    }

    printWarningMessage("평점을 등록할 수 없습니다. 사용자/영화 ID와 평점 범위를 확인하세요.");
}

void DisplayManager::printMovieRatingsMenu() const
{
    // 영화가 있는지 먼저 확인하고, 있으면 해당 영화 평점 목록 출력하는거임
    int movieId = 0;

    printSectionHeader("영화별 평점 보기");
    clearInput();

    if (!readIntValue("조회할 영화 ID: ", movieId))
    {
        printInfoMessage("입력이 종료되어 조회를 취소합니다.");
        return;
    }

    const Movie *movie = movieManager.findMovieById(movieId);
    if (movie == nullptr)
    {
        printWarningMessage("해당 영화가 존재하지 않습니다.");
        return;
    }

    ConsoleView::printMovieTable(std::vector<const Movie *>{movie});

    const auto matchedRatings = ratingManager.getRatingsByMovieId(movieId);
    if (matchedRatings.empty())
    {
        printInfoMessage("등록된 평점이 없습니다.");
        return;
    }

    printInfoMessage("평점 " + std::to_string(matchedRatings.size()) + "건");
    ratingManager.printRatingsByMovieId(movieId);
}

void DisplayManager::recommendMovieMenu() const
{
    int userId = 0;

    printSectionHeader("사용자별 영화 추천");
    clearInput();

    if (!readRecommendableUserId(userId))
    {
        return;
    }

    Recommender recommender(movieManager, ratingManager);
    const auto recommendations = recommender.recommend(userId,
                                                       MovieConstants::DEFAULT_TOP_K_USERS,
                                                       MovieConstants::DEFAULT_TOP_N_MOVIES);

    if (recommendations.empty())
    {
        printInfoMessage("추천할 영화가 없습니다.");
        return;
    }

    printRecommendationResults(recommendations, "추천 결과");
}

void DisplayManager::recommendMovieByGenreMenu() const
{
    int userId = 0;
    std::string genre;

    printSectionHeader("장르 필터 추천");
    clearInput();

    if (!readRecommendableUserId(userId))
    {
        return;
    }

    printAvailableGenres();
    std::cout << "추천 장르: ";
    std::getline(std::cin, genre);

    if (genre.empty())
    {
        printWarningMessage("장르를 입력해 주세요.");
        return;
    }

    if (movieManager.filterMoviesByGenre(genre).empty())
    {
        printWarningMessage("해당 장르의 영화가 없습니다.");
        return;
    }

    Recommender recommender(movieManager, ratingManager);
    const auto recommendations = recommender.recommend(userId,
                                                       MovieConstants::DEFAULT_TOP_K_USERS,
                                                       MovieConstants::DEFAULT_TOP_N_MOVIES,
                                                       genre);

    if (recommendations.empty())
    {
        printInfoMessage("해당 장르에서 추천할 영화가 없습니다.");
        return;
    }

    printRecommendationResults(recommendations, genre + " 추천 결과");
}

void DisplayManager::printAvailableGenres() const
{
    const auto statisticsByGenre = movieManager.getGenreStatistics();

    if (statisticsByGenre.empty())
    {
        return;
    }

    std::cout << termcolor::bright_cyan << "사용 가능한 장르: " << termcolor::reset;
    bool isFirst = true;
    for (const auto &[genre, statistics] : statisticsByGenre)
    {
        if (!isFirst)
        {
            std::cout << ", ";
        }

        std::cout << termcolor::bright_green << genre << termcolor::reset
                  << "(" << statistics.movieCount << "편)";
        isFirst = false;
    }
    std::cout << std::endl;
}

void DisplayManager::printRecommendationResults(const std::vector<std::pair<const Movie *, double>> &recommendations,
                                                const std::string &title) const
{
    printInfoMessage(title + ": " + std::to_string(recommendations.size()) + "편");
    ConsoleView::printRecommendationTable(recommendations);
}

void DisplayManager::showStatisticsMenu() const
{
    int menu = -1;
    bool isRunning = true;

    clearInput();

    while (isRunning)
    {
        printSectionHeader("통계 메뉴");
        std::cout << termcolor::bright_yellow << " 1." << termcolor::reset << " 전체 평균 평점" << std::endl;
        std::cout << termcolor::bright_yellow << " 2." << termcolor::reset << " 장르별 통계" << std::endl;
        std::cout << termcolor::bright_yellow << " 3." << termcolor::reset << " 평점 Top N 영화" << std::endl;
        std::cout << termcolor::bright_yellow << " 0." << termcolor::reset << " 돌아가기" << std::endl;
        printDivider('-');

        if (!readIntValue("선택 > ", menu))
        {
            printInfoMessage("입력이 종료되어 통계 메뉴를 닫습니다.");
            return;
        }

        try
        {
            switch (menu)
            {
            case 0:
                isRunning = false;
                break;
            case 1:
                printAverageRatingStatistics();
                break;
            case 2:
                printGenreStatistics();
                break;
            case 3:
                printTopRatedMoviesStatistics();
                break;
            default:
                printWarningMessage("올바른 메뉴 번호를 입력해 주세요.");
                break;
            }
        }
        catch (const std::exception &e)
        {
            printWarningMessage(std::string("통계를 출력할 수 없습니다: ") + e.what());
        }
    }
}

void DisplayManager::printAverageRatingStatistics() const
{
    printSectionHeader("전체 평균 평점");
    ConsoleView::printKeyValueTable({
        {"전체 평균 평점", ConsoleView::formatScore(movieManager.getAverageRating())},
        {"전체 평점 수", std::to_string(ratingManager.getRatingCount()) + "건"},
    });
}

void DisplayManager::printGenreStatistics() const
{
    const auto statisticsByGenre = movieManager.getGenreStatistics();
    std::string popularGenre;
    int maxRatingCount = 0;

    if (statisticsByGenre.empty())
    {
        printInfoMessage("등록된 영화가 없습니다.");
        return;
    }

    printSectionHeader("장르별 통계");
    ConsoleView::printGenreStatisticsTable(statisticsByGenre);

    for (const auto &[genre, statistics] : statisticsByGenre)
    {
        if (statistics.ratingCount > maxRatingCount)
        {
            popularGenre = genre;
            maxRatingCount = statistics.ratingCount;
        }
    }

    if (!popularGenre.empty())
    {
        printSuccessMessage("인기 장르: " + popularGenre + " (" + std::to_string(maxRatingCount) + "건)");
    }
}

void DisplayManager::printTopRatedMoviesStatistics() const
{
    int limit = 0;

    if (!readIntValue("출력할 영화 수: ", limit))
    {
        printInfoMessage("입력이 종료되어 Top N 출력을 취소합니다.");
        return;
    }

    if (limit <= 0)
    {
        printWarningMessage("1 이상의 숫자를 입력해 주세요.");
        return;
    }

    const auto topMovies = movieManager.getTopRatedMovies(limit);
    if (topMovies.empty())
    {
        printInfoMessage("평점이 등록된 영화가 없습니다.");
        return;
    }

    printSectionHeader("평점 Top " + std::to_string(topMovies.size()) + " 영화");
    ConsoleView::printMovieTable(topMovies);
}

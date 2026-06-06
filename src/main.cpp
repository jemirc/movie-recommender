#include <cstdlib>
#include <exception>
#include <iostream>
#include <limits>

#include "DisplayManager.h"
#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"

namespace
{
constexpr const char *MOVIE_DATA_FILE = "data/movie.csv";
constexpr const char *USER_DATA_FILE = "data/user.csv";
constexpr const char *RATING_DATA_FILE = "data/rating.csv";

void clearMenuInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool loadInitialData(MovieManager &movieManager, UserManager &userManager, RatingManager &ratingManager)
{
    try
    {
        movieManager.loadFromFile(MOVIE_DATA_FILE);
        userManager.loadFromFile(USER_DATA_FILE);
        ratingManager.loadFromFile(RATING_DATA_FILE);
        movieManager.rebuildRatingsFrom(ratingManager.getAllRatings());
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "초기 데이터 로딩 실패: " << e.what() << std::endl;
        return false;
    }
}

bool saveCurrentData(const MovieManager &movieManager, const UserManager &userManager, const RatingManager &ratingManager)
{
    try
    {
        movieManager.saveToFile(MOVIE_DATA_FILE);
        userManager.saveToFile(USER_DATA_FILE);
        ratingManager.saveToFile(RATING_DATA_FILE);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "데이터 저장 실패: " << e.what() << std::endl;
        return false;
    }
}

bool readMainMenu(int &menu)
{
    if (std::cin >> menu)
    {
        return true;
    }

    if (!std::cin.eof())
    {
        clearMenuInput();
    }

    return false;
}

void runMenuAction(int menu, DisplayManager &displayManager, bool &isRunning)
{
    switch (menu)
    {
    case 0:
        isRunning = false;
        break;
    case 1:
        displayManager.addMovieMenu();
        break;
    case 2:
        displayManager.searchMovieMenu();
        break;
    case 3:
        displayManager.printAllMoviesMenu();
        break;
    case 4:
        displayManager.printSortedMoviesMenu();
        break;
    case 5:
        displayManager.addUserMenu();
        break;
    case 6:
        displayManager.printAllUsersMenu();
        break;
    case 7:
        displayManager.addRatingMenu();
        break;
    case 8:
        displayManager.printMovieRatingsMenu();
        break;
    case 9:
        displayManager.recommendMovieMenu();
        break;
    case 10:
        displayManager.recommendMovieByGenreMenu();
        break;
    case 11:
        displayManager.showStatisticsMenu();
        break;
    default:
        std::cout << "올바른 메뉴 번호를 입력해 주세요." << std::endl;
        break;
    }
}
}

int main()
{
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;

    if (!loadInitialData(movieManager, userManager, ratingManager))
    {
        return EXIT_FAILURE;
    }

    DisplayManager displayManager(movieManager, userManager, ratingManager);
    bool isRunning = true;

    while (isRunning)
    {
        int menu = -1;
        displayManager.printMainMenu();

        if (!readMainMenu(menu))
        {
            if (std::cin.eof())
            {
                break;
            }

            continue;
        }

        runMenuAction(menu, displayManager, isRunning);
    }

    if (!saveCurrentData(movieManager, userManager, ratingManager))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

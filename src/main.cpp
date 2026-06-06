#include <cstdlib>
#include <exception>
#include <iostream>
#include <limits>

#include "DisplayManager.h"
#include "MovieManager.h"
#include "RatingManager.h"
#include "UserManager.h"

int main()
{
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;

    // 파일 자체를 못 열면 매니저에서 예외를 던지고, main에서 한 번만 잡는거임
    try
    {
        movieManager.loadFromFile("data/movie.csv");
        userManager.loadFromFile("data/user.csv");
        ratingManager.loadFromFile("data/rating.csv");
        movieManager.rebuildRatingsFrom(ratingManager.getAllRatings());
    }
    catch (const std::exception &e)
    {
        std::cerr << "초기 데이터 로딩 실패: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    DisplayManager displayManager(movieManager, userManager, ratingManager);
    int menu = -1;
    bool isRunning = true;

    // main에서 메뉴 번호 입력 실패했을때만 정리하려고 간단히 람다로 둔거임
    auto clearInput = []()
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    };

    while (isRunning)
    {
        displayManager.printMainMenu();

        if (!(std::cin >> menu))
        {
            if (std::cin.eof())
            {
                break;
            }

            clearInput();
            continue;
        }

        switch (menu)
        {
        case 0:
            // 0번은 프로그램 종료만 담당
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

    // 저장도 파일을 못 열 수 있으니까 종료 직전에 한 번 더 예외를 잡는거임
    try
    {
        movieManager.saveToFile("data/movie.csv");
        userManager.saveToFile("data/user.csv");
        ratingManager.saveToFile("data/rating.csv");
    }
    catch (const std::exception &e)
    {
        std::cerr << "데이터 저장 실패: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

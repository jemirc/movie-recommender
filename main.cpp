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
        // 큰 메뉴 화면은 main에서 바로 보여주고, 선택된 번호의 안쪽 처리만 DisplayManager로 넘기는 구조임
        std::cout << std::endl
                  << "=== Movie Recommender ===" << std::endl
                  << std::endl;
        std::cout << "[영화]" << std::endl;
        std::cout << "1. 영화 추가" << std::endl;
        std::cout << "2. 제목으로 검색" << std::endl;
        std::cout << "3. 전체 목록 출력" << std::endl;
        std::cout << "4. 평점순 정렬 출력" << std::endl
                  << std::endl;
        std::cout << "[사용자]" << std::endl;
        std::cout << "5. 사용자 추가" << std::endl;
        std::cout << "6. 사용자 목록 출력" << std::endl
                  << std::endl;
        std::cout << "[평점]" << std::endl;
        std::cout << "7. 평점 입력" << std::endl;
        std::cout << "8. 영화별 평점 보기" << std::endl
                  << std::endl;
        std::cout << "0. 종료" << std::endl;
        std::cout << std::endl;
        std::cout << "선택 > ";

        if (!(std::cin >> menu))
        {
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
        default:
            std::cout << "올바른 메뉴 번호를 입력해 주세요." << std::endl;
            break;
        }
    }

    return 0;
}

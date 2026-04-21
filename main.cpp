#include <iostream>
#include <limits>

#include "MovieManager.h"
#include "UserManager.h"

using namespace std;

// 메인 메뉴 화면 출력만 따로 빼둔거임
void printMainMenu()
{
    cout << "=== Movie Recommender ===" << endl;
    cout << "[영화]" << endl;
    cout << "1. 영화 추가" << endl;
    cout << "2. 제목으로 검색" << endl;
    cout << "3. 전체 목록 출력" << endl;
    cout << "4. 평점순 정렬 출력" << endl;
    cout << "[사용자]" << endl;
    cout << "5. 사용자 추가" << endl;
    cout << "6. 사용자 목록 출력" << endl;
    cout << "[평점]" << endl;
    cout << "7. 평점 입력" << endl;
    cout << "8. 영화별 평점 보기" << endl;
    cout << "0. 종료" << endl;
    cout << endl;
    cout << "선택 > ";
}

int main()
{
    MovieManager movieManager;
    UserManager userManager;
    int menu = -1;

    // 지금 main은 매니저 객체 만들고 메뉴 루프만 담당하는 구조로 가는거임
    while (true)
    {
        printMainMenu();

        // 숫자가 아닌 입력이 들어오면 스트림 상태 복구하고 다시 메뉴 보여주기
        if (!(cin >> menu))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (menu == 0)
        {
            break; // 0 입력하면 안전하게 반복문 종료
        }

        switch (menu)
        {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            // 세부 기능은 다음 단계에서 각 Manager 메서드랑 연결하면 되는거임
            break;
        default:
            break;
        }
    }

    return 0;
}

#include <iostream>
#include <limits>
#include <string>

#include "MovieManager.h"
#include "RatingManager.h"
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

void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void addMovieMenu(MovieManager &movieManager)
{
    int id = 0;
    int year = 0;
    string title;
    string genre;

    cout << "영화 ID: ";
    cin >> id;
    clearInput();

    cout << "제목: ";
    getline(cin, title);

    cout << "장르: ";
    getline(cin, genre);

    cout << "개봉 연도: ";
    cin >> year;
    clearInput();

    if (movieManager.addMovie(Movie(id, title, genre, year)))
    {
        cout << "영화가 추가되었습니다." << endl;
        return;
    }

    cout << "같은 ID의 영화가 이미 존재합니다." << endl;
}

void searchMovieMenu(const MovieManager &movieManager)
{
    string keyword;
    cout << "검색할 제목 키워드: ";
    clearInput();
    getline(cin, keyword);

    const vector<const Movie *> matchedMovies = movieManager.searchMoviesByTitle(keyword);
    if (matchedMovies.empty())
    {
        cout << "검색 결과가 없습니다." << endl;
        return;
    }

    for (const Movie *movie : matchedMovies)
    {
        cout << *movie << endl;
    }
}

void printAllMoviesMenu(const MovieManager &movieManager)
{
    if (movieManager.getMovieCount() == 0)
    {
        cout << "등록된 영화가 없습니다." << endl;
        return;
    }

    movieManager.printAllMovies();
}

void printSortedMoviesMenu(const MovieManager &movieManager)
{
    if (movieManager.getMovieCount() == 0)
    {
        cout << "등록된 영화가 없습니다." << endl;
        return;
    }

    movieManager.printMoviesSortedByRating();
}

void addUserMenu(UserManager &userManager)
{
    int id = 0;
    string name;
    string email;

    cout << "사용자 ID: ";
    cin >> id;
    clearInput();

    cout << "이름: ";
    getline(cin, name);

    cout << "이메일: ";
    getline(cin, email);

    if (userManager.addUser(User(id, name, email)))
    {
        cout << "사용자가 추가되었습니다." << endl;
        return;
    }

    cout << "같은 ID의 사용자가 이미 존재합니다." << endl;
}

void printAllUsersMenu(const UserManager &userManager)
{
    if (userManager.getUserCount() == 0)
    {
        cout << "등록된 사용자가 없습니다." << endl;
        return;
    }

    userManager.printAllUsers();
}

void addRatingMenu(RatingManager &ratingManager, const UserManager &userManager, MovieManager &movieManager)
{
    int userId = 0;
    int movieId = 0;
    double score = 0.0;

    cout << "사용자 ID: ";
    cin >> userId;
    cout << "영화 ID: ";
    cin >> movieId;
    cout << "평점(0.0 ~ 5.0): ";
    cin >> score;
    clearInput();

    if (ratingManager.addRating(userId, movieId, score, userManager, movieManager))
    {
        cout << "평점이 등록되었습니다." << endl;
        return;
    }

    cout << "평점을 등록할 수 없습니다. 사용자/영화 ID를 확인하거나 중복 평점인지 확인하세요." << endl;
}

void printMovieRatingsMenu(const MovieManager &movieManager, const RatingManager &ratingManager)
{
    int movieId = 0;
    cout << "조회할 영화 ID: ";
    cin >> movieId;
    clearInput();

    const Movie *movie = movieManager.findMovieById(movieId);
    if (movie == nullptr)
    {
        cout << "해당 영화가 존재하지 않습니다." << endl;
        return;
    }

    cout << *movie << endl;

    const vector<const Rating *> matchedRatings = ratingManager.getRatingsByMovieId(movieId);
    if (matchedRatings.empty())
    {
        cout << "등록된 평점이 없습니다." << endl;
        return;
    }

    ratingManager.printRatingsByMovieId(movieId);
}

int main()
{
    MovieManager movieManager;
    UserManager userManager;
    RatingManager ratingManager;
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
            addMovieMenu(movieManager);
            break;
        case 2:
            searchMovieMenu(movieManager);
            break;
        case 3:
            printAllMoviesMenu(movieManager);
            break;
        case 4:
            printSortedMoviesMenu(movieManager);
            break;
        case 5:
            addUserMenu(userManager);
            break;
        case 6:
            printAllUsersMenu(userManager);
            break;
        case 7:
            addRatingMenu(ratingManager, userManager, movieManager);
            break;
        case 8:
            printMovieRatingsMenu(movieManager, ratingManager);
            break;
        default:
            cout << "올바른 메뉴 번호를 입력해 주세요." << endl;
            break;
        }
    }

    return 0;
}

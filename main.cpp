#include "Movie.h"
#include <iostream>

int main()
{
    Movie m1(1, "기생충", "드라마", 2019);
    Movie m2(2, "인터스텔라", "SF", 2014);

    m1.addRating(4.8);
    m2.addRating(4.9);

    m1.display();
    // 1. 기생충 (2019)  평점: 4.8 (1건)
    m2.display();
    // 2. 인터스텔라 (2014)  평점: 4.9 (1건)

    m1.addRating(5.0);
    m1.addRating(6.0); // 유효성 검사 → 무시됨

    std::cout << m1.getTitle() << ": "
              << m1.getAverageRating() << std::endl;
    // 기생충: 4.9  (4.8 + 5.0) / 2
    return 0;
}
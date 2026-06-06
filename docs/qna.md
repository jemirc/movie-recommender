# M4 예상 Q&A

## 1. 왜 멤버 변수를 `private`으로 만들었나요?

객체의 내부 상태를 외부에서 직접 바꾸면 평점 수와 평균 평점처럼 서로 맞아야 하는 값이 깨질 수 있습니다. 그래서 `Movie`, `User`, `Rating`의 멤버 변수는 `private`으로 두고, 필요한 값만 getter나 검증된 메서드로 접근하게 했습니다.

## 2. getter/setter를 사용한 이유는 무엇인가요?

getter는 읽기 전용 접근을 제공하고, setter나 update 메서드는 값 변경 전에 검증을 넣을 수 있습니다. 예를 들어 `Rating::setScore`는 평점이 `0`에서 `5` 사이인지 확인한 뒤에만 값을 변경합니다.

## 3. 각 클래스의 책임은 무엇인가요?

- `Movie`, `User`, `Rating`: 순수 데이터와 기본 동작을 관리합니다.
- `MovieManager`, `UserManager`, `RatingManager`: 데이터 목록, 검색, 추가, CSV 입출력을 관리합니다.
- `SimilarityCalculator`: 두 사용자의 평점 유사도 계산만 담당합니다.
- `Recommender`: 유사 사용자 기반 추천 후보와 점수를 계산합니다.
- `DisplayManager`: 콘솔 입력과 출력, 메뉴 흐름을 담당합니다.

## 4. `BaseManager`에서 virtual 함수를 사용한 이유는?

`MovieManager`, `UserManager`, `RatingManager`가 모두 파일 로딩, 저장, 개수 확인이라는 공통 동작을 갖습니다. `BaseManager`는 이 공통 인터페이스를 강제하기 위해 `loadFromFile`, `saveToFile`, `size`를 순수 가상 함수로 선언했습니다.

## 5. virtual destructor가 필요한 이유는?

기반 클래스 포인터로 파생 객체를 다룰 가능성이 있을 때, 소멸자가 virtual이 아니면 파생 클래스의 소멸자가 호출되지 않을 수 있습니다. 그래서 `BaseManager`는 안전한 다형적 소멸을 위해 virtual destructor를 둡니다.

## 6. `vector`를 사용한 이유는 무엇인가요?

현재 데이터 규모에서는 순차 탐색과 전체 출력이 많고, 데이터가 메모리에 연속적으로 저장되는 `vector`가 단순하고 효율적입니다. 영화, 사용자, 평점 목록을 순회하는 코드도 range-for와 잘 맞습니다.

## 7. `map`과 `unordered_map` 차이는 무엇이고 왜 `map`을 썼나요?

`map`은 키 기준으로 자동 정렬되고, `unordered_map`은 해시 기반이라 평균 접근 속도가 빠르지만 순서가 보장되지 않습니다. 장르별 통계는 발표 화면에서 장르가 일정한 순서로 출력되는 편이 보기 좋아 `map`을 사용했습니다.

## 8. 람다는 어디서 사용했나요?

- `MovieManager::filterMoviesByGenre`에서 `std::copy_if` 조건으로 사용했습니다.
- `MovieManager::getAverageRating`에서 `std::accumulate` 누적 규칙으로 사용했습니다.
- `MovieManager::getTopRatedMovies`와 추천 점수 정렬에서 `std::sort` 비교 함수로 사용했습니다.

## 9. `new/delete`를 직접 사용했나요?

직접 `new/delete`는 사용하지 않았습니다. 영화 객체는 `std::unique_ptr<Movie>`로 보관해 자동으로 해제되도록 했고, 메모리 누수는 macOS `leaks`로 `0 leaks`를 확인했습니다.

## 10. 예외 처리 전략은 무엇인가요?

파일 자체를 열 수 없는 경우는 프로그램 실행에 필수적인 데이터가 없다는 뜻이므로 예외를 던지고 `main`에서 한 번 잡아 종료합니다. CSV의 일부 줄만 잘못된 경우에는 해당 줄만 건너뛰고 나머지 정상 데이터는 계속 로딩합니다. 통계처럼 빈 데이터에서 계산이 불가능한 경우도 예외나 안내 메시지로 처리합니다.

## 11. 유사도 계산 방식은 무엇인가요?

두 사용자가 공통으로 평가한 영화 수와 평점 차이를 이용합니다.

```text
similarity = commonMovieCount * COMMON_MOVIE_WEIGHT - scoreDiffSum
```

공통으로 본 영화가 많을수록 유사도가 커지고, 같은 영화에 대한 평점 차이가 클수록 유사도가 줄어듭니다.

## 12. 추천 결과는 어떻게 정렬하나요?

비슷한 사용자들이 높게 평가한 영화에 `평점 * 유사도`를 누적해 추천 점수를 만듭니다. 이후 추천 점수 내림차순으로 정렬하고, 점수가 같으면 영화 ID가 작은 순서로 정렬합니다.

## 13. 왜 이 알고리즘을 선택했나요?

과제 범위에서 사용자, 영화, 평점 클래스와 STL 자료구조를 활용하기 좋고, 결과를 설명하기 쉽기 때문입니다. 공통 영화 수와 평점 차이라는 직관적인 기준을 사용해 발표 중에도 칠판이나 슬라이드에서 계산식을 설명할 수 있습니다.

## 14. 영화가 1만 개로 늘어나면 어디가 느려질까요?

현재 구조는 사용자별 평점을 찾을 때 전체 평점 벡터를 순회하고, 유사도 계산에서도 사용자 쌍과 평점 목록을 반복 비교합니다. 영화와 평점이 많아지면 `RatingManager::findByUser`, 유사도 계산, 영화 ID 검색이 느려질 수 있습니다.

## 15. 어떻게 최적화할 수 있나요?

- 사용자 ID별 평점 목록을 `unordered_map<int, vector<Rating>>`으로 캐싱합니다.
- 영화 ID 검색은 `unordered_map<int, Movie*>` 인덱스를 추가합니다.
- 유사도 계산은 공통 영화만 빠르게 찾도록 사용자별 `movieId -> score` 맵을 사용합니다.
- 추천 후보 계산은 전체 정렬 대신 `partial_sort`나 우선순위 큐로 Top N만 뽑을 수 있습니다.

## 16. 데이터 구조를 어떻게 바꿀 수 있나요?

현재는 과제 규모에 맞게 `vector` 중심으로 구현했습니다. 데이터가 커지면 조회 성능을 위해 `unordered_map` 기반 인덱스를 추가하고, 원본 저장 순서를 유지해야 하는 목록은 `vector`로 유지하는 하이브리드 구조가 좋습니다.

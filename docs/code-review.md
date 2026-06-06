# M4 코드 리뷰 기록

## 자가 코드 리뷰 요약

M4 발표 전 체크리스트 기준으로 현재 코드를 점검했다. 기능 확장 이후 길어진 메뉴 흐름은 `main.cpp`와 `DisplayManager.cpp`에서 helper 함수로 분리했고, 추천 입력 검증과 추천 결과 출력도 재사용 가능한 함수로 정리했다.

## 구조 / 가독성

- 함수 하나가 한 가지 일을 하도록 `main.cpp`의 데이터 로딩, 저장, 메뉴 입력, 메뉴 실행을 분리했다.
- `DisplayManager::recommendMovieMenu`와 `DisplayManager::recommendMovieByGenreMenu`에서 중복되던 사용자 검증을 `readRecommendableUserId`로 분리했다.
- 추천 결과 출력은 `printRecommendationResults`로 모아 기본 추천과 장르 필터 추천이 같은 형식으로 출력되게 했다.
- `DisplayManager`는 콘솔 입출력, `MovieManager`는 영화 검색/통계, `Recommender`는 추천 계산을 맡도록 책임을 유지했다.
- README와 발표 문서에는 실행 방법, 클래스 구조, 알고리즘, 시연 흐름을 처음 보는 사람도 따라갈 수 있게 정리했다.

## C++ 품질 점검

- `const` 멤버 함수와 `const&` 인자를 유지했다.
- `std::unique_ptr<Movie>`를 사용해 직접 `new/delete` 없이 영화 객체를 관리한다.
- `std::copy_if`, `std::accumulate`, `std::sort`, `std::map`, 람다, 구조적 바인딩을 사용했다.
- 파일 I/O 실패는 `throw/catch`로 처리하고, 손상된 CSV 라인은 줄 번호와 함께 건너뛴다.
- 평균 평점, 평점 점수, 추천 점수 출력 포맷을 소수점 둘째 자리로 통일했다.
- 매직 넘버는 `MovieConstants` 또는 UI helper 상수로 분리했다.

## 메모리 점검

```text
leaks --atExit -- ./movie_recommender
Process: movie_recommender
0 leaks for 0 total leaked bytes
```

`valgrind`는 현재 환경에 설치되어 있지 않아 macOS 기본 도구인 `leaks`로 확인했다.

## 동작 점검

- `make` 빌드 성공
- 메뉴 `1`부터 `11`까지 임시 데이터로 전체 리허설 성공
- 사용자 `1` 기준 추천 결과 정상 출력
- 사용자 `1`, 장르 `액션` 기준 장르 필터 추천 정상 출력
- 통계 메뉴에서 전체 평균 `4.36`, 전체 평점 `80건`, 장르별 통계, Top 5 영화 정상 출력
- `data/movie.csv` 누락 시 초기 로딩 실패 메시지와 `exit=1` 확인
- 손상된 CSV 라인은 건너뛰고 정상 데이터로 실행되는 것 확인

## 남은 리스크

- CSV 파서는 쉼표가 포함된 영화 제목을 지원하지 않는다. 현재 과제용 데이터에는 쉼표가 없어 발표 범위에서는 문제 없다.
- 추천 알고리즘은 전체 사용자를 순회하므로 데이터가 매우 커지면 사용자별 평점 인덱스가 필요하다.

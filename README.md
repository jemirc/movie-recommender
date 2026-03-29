교수님 강의때 과제 설명해주신거 정리 요약

TODO Makefile도 만들기

Auto 변수를 이용해도 되긴 함, 아니면 강의자료 참고하던가

User Class와 Rating 클래스를 독립 설계하기 (강의자료 참고) h cpp 분리해서

User 클래스 독립 설계 30점
• id, name, email 멤버 변수 선언 (헤더/소스 분리)
• 매개변수 생성자 + 초기화 리스트 사용
• getId() / getName() 등 getter (const 정확성 준수)
• display() 메서드 구현


Rating 클래스 독립 설계 30점
• userId, movieId, score 멤버 변수
• score 유효성 검사: 0.0 ~ 5.0 범위 확인 / 객체를 생성할때 유효성을 검사하기, private 메서드 하나 만들고 생성자 할때 호출하면 될듯
• getScore() / getUserId() / getMovieId() (const)
• display() 구현

Movie 클래스 보완 (오늘 내용 적용) 20점
• averageRating → totalRating + ratingCount 로 변경
• 기본 생성자 Movie() 추가 구현
• addRating(double r) 유효성 검사 포함 구현
• getAverageRating() 0 나눗셈 방어 코드

Makefile + 빌드 + 통합 main.cpp 20점
• Makefile 작성 (의존성 정확히 명시)
• make, make run, make clean 정상 동작
• main.cpp 에서 세 클래스 + vector 동작 확인
• git add → commit → push 제출

Main에서 기존꺼 vector로 Movie 객체 넣고, 반복문으로(참조) 출력도 하기

스스로 설계이지만 강의 자료에 설계 가이드를 주시긴 하심

끝나고 체크리스트로 각 항목이 충족됐는지 확인하기

그리고 과제 제출한 뒤에 교수님이 우리 코드 바탕으로 교수님과 조교님이 질문 주실꺼라 함 (AI가 코드 작성한거 방지 + 이해도 검사용)
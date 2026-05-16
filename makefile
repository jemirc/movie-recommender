CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET = movie_recommender
OBJS = main.o Movie.o User.o Rating.o MovieManager.o UserManager.o RatingManager.o DisplayManager.o SimilarityCalculator.o Recommender.o

# 최종 실행 파일 만드는 규칙
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 각 소스 파일을 오브젝트 파일로 컴파일하는 규칙들
main.o: main.cpp DisplayManager.h MovieManager.h UserManager.h RatingManager.h Movie.h User.h Rating.h
	$(CXX) $(CXXFLAGS) -c $<

Movie.o: Movie.cpp Movie.h
	$(CXX) $(CXXFLAGS) -c $<

User.o: User.cpp User.h
	$(CXX) $(CXXFLAGS) -c $<

Rating.o: Rating.cpp Rating.h
	$(CXX) $(CXXFLAGS) -c $<

MovieManager.o: MovieManager.cpp MovieManager.h BaseManager.h Movie.h Rating.h
	$(CXX) $(CXXFLAGS) -c $<

UserManager.o: UserManager.cpp UserManager.h BaseManager.h User.h
	$(CXX) $(CXXFLAGS) -c $<

RatingManager.o: RatingManager.cpp RatingManager.h BaseManager.h Rating.h MovieManager.h UserManager.h
	$(CXX) $(CXXFLAGS) -c $<

DisplayManager.o: DisplayManager.cpp DisplayManager.h Recommender.h MovieManager.h UserManager.h RatingManager.h Movie.h User.h Rating.h SimilarityCalculator.h
	$(CXX) $(CXXFLAGS) -c $<

SimilarityCalculator.o: SimilarityCalculator.cpp SimilarityCalculator.h Rating.h
	$(CXX) $(CXXFLAGS) -c $<

Recommender.o: Recommender.cpp Recommender.h SimilarityCalculator.h MovieManager.h RatingManager.h Movie.h Rating.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

# 빌드하면서 생긴 파일들 지우는 용도
clean:
	rm -f $(TARGET) movie_app $(OBJS)

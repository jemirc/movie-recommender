CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET = movie_recommender
OBJS = main.o Movie.o User.o Rating.o MovieManager.o UserManager.o

# 최종 실행 파일 만드는 규칙
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# 각 소스 파일을 오브젝트 파일로 컴파일하는 규칙들
main.o: main.cpp MovieManager.h UserManager.h Movie.h User.h
	$(CXX) $(CXXFLAGS) -c $<

Movie.o: Movie.cpp Movie.h
	$(CXX) $(CXXFLAGS) -c $<

User.o: User.cpp User.h
	$(CXX) $(CXXFLAGS) -c $<

Rating.o: Rating.cpp Rating.h
	$(CXX) $(CXXFLAGS) -c $<

MovieManager.o: MovieManager.cpp MovieManager.h Movie.h
	$(CXX) $(CXXFLAGS) -c $<

UserManager.o: UserManager.cpp UserManager.h User.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean

# 빌드하면서 생긴 파일들 지우는 용도
clean:
	rm -f $(TARGET) movie_app $(OBJS)

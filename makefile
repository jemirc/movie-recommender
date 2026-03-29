CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
TARGET = movie_app
OBJS = main.o Movie.o User.o Rating.o

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

main.o: main.cpp Movie.h User.h Rating.h
	$(CXX) $(CXXFLAGS) -c $<

Movie.o: Movie.cpp Movie.h
	$(CXX) $(CXXFLAGS) -c $<

User.o: User.cpp User.h
	$(CXX) $(CXXFLAGS) -c $<

Rating.o: Rating.cpp Rating.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean run

clean:
	rm -f $(TARGET) $(OBJS)

run: $(TARGET)
	./$(TARGET)
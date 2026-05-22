CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -Iinclude
TARGET = movie_recommender
SRC_DIR = src
OBJ_DIR = obj
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

# 최종 실행 파일 만드는 규칙
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

# 빌드하면서 생긴 파일들 지우는 용도
clean:
	rm -f $(TARGET) movie_app $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d

-include $(DEPS)

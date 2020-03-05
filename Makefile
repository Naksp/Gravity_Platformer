BUILD_DIR=build

CXXFLAGS := -Wall -g -std=c++11
SFML_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

my_game: $(OBJS)
	g++ -o $(BUILD_DIR)/my_game $(OBJS) $(SFML_FLAGS) 

$(BUILD_DIR)/%.o: %.cpp
	g++ -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(BUILD_DIR)/*

run: my_game
	$(BUILD_DIR)/my_game
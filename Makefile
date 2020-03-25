BUILD_DIR=build

CXXFLAGS := -Wall -g -std=c++11
SFML_FLAGS := -lsfml-graphics -lsfml-window -lsfml-system

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(SRCS))

simple_platformer: $(OBJS)
	g++ -o $(BUILD_DIR)/simple_platformer $(OBJS) $(SFML_FLAGS) 
	@echo "Make Finished Successfully"

$(BUILD_DIR)/%.o: %.cpp
	g++ -o $@ -c $< $(CXXFLAGS)

clean:
	rm -f $(BUILD_DIR)/*

run: simple_platformer
	$(BUILD_DIR)/simple_platformer
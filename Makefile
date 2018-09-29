MAIN = bomberguy

SRC_DIR = ./src
BUILD_DIR = ./build

SRCS += src/audio.cpp
SRCS += src/map.cpp
SRCS += src/player.cpp
SRCS += src/enemy.cpp
SRCS += src/bomb.cpp
SRCS += src/keyboard.cpp
SRCS += src/screen.cpp
SRCS += src/controller.cpp
SRCS += src/main.cpp

OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

INCLUDES = -I./src/

LIBS = $(shell ncurses5-config --libs) -pthread -lportaudio -lsndfile

GFLAGS = -MD -MP -std=c++11 $(shell ncurses5-config --cflags) -g $(INCLUDES)

all: $(BUILD_DIR)/$(MAIN)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	g++ -c $(GFLAGS) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/$(MAIN): $(OBJS)
	g++ $^ $(LIBS) -o $@

.PHONY: clean

clean:
	@rm -rf $(BUILD_DIR)

-include $(DEPS)

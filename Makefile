MAIN = bomberguy

SRC_DIR = ./src
BUILD_DIR = ./build

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

INCLUDES = -I./src/

LIBS = $(shell ncurses5-config --libs) -lncurses -pthread -lportaudio -lsndfile

GFLAGS = -std=c++11 $(shell ncurses5-config --cflags) -g $(INCLUDES)

all: $(BUILD_DIR)/$(MAIN)

$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	g++ $(GFLAGS) $(CXXFLAGS) -c $< -o $@ $(LIBS)

$(BUILD_DIR)/$(MAIN): $(OBJS)
	g++ $(OBJS) -o $@ $(LIBS)

.PHONY: clean

clean:
	@rm -rf $(BUILD_DIR)

-include $(DEPS)

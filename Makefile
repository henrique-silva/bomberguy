MAIN = bomberguy

SRC = $(wildcard *.cpp)
GFLAGS = -std=c++11 $(shell ncurses5-config --cflags)
LIBS = $(shell ncurses5-config --libs) -lncurses -pthread -lportaudio

all: $(MAIN)

$(MAIN): $(SRC)
	g++ $(GFLAGS) -o $(MAIN) $(SRC) $(LIBS)

clean:
	@rm -f $(MAIN) *.o
	@rm -f *~
	@rm -f \#*

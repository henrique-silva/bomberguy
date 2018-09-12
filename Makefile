MAIN = bomberguy

SRC = $(wildcard *.cpp)
GFLAGS = -std=c++11 $(shell ncursesw5-config --cflags)
LIBS = $(shell ncursesw5-config --libs) -pthread -lportaudio

all: $(MAIN)

$(MAIN): $(SRC)
	g++ $(GFLAGS) -o $(MAIN) $(SRC) $(LIBS)

clean:
	@rm -f $(MAIN) *.o
	@rm -f *~
	@rm -f \#*

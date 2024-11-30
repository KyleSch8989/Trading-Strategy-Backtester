CC = g++
CFLAGS = -Wall -std=c++11

all : tradingSim

tradingSim: main.cpp
	$(CC) $(CFLAGS) -o tradingSim main.cpp

clean:
	rm -f tradingSim
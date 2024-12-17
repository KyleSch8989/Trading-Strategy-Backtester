CC = g++
CFLAGS = -Wall -std=c++11

TARGET = tradingSim
OBJ = main.o sma.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp strategies.h
	$(CC) $(CFLAGS) -c main.cpp

sma.o: sma.cpp strategies.h
	$(CC) $(CFLAGS) -c sma.cpp

clean:
	rm -f $(TARGET) $(OBJ)
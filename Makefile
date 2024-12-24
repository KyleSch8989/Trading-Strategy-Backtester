CC = g++
CFLAGS = -Wall -std=c++11 -pthread 
TARGET = tradingSim
OBJ = main.o sma.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lcurl -I$(shell brew --prefix nlohmann-json)/include

main.o: main.cpp strategies.h signal.h
	$(CC) $(CFLAGS) -c main.cpp  -lcurl 

sma.o: sma.cpp strategies.h signal.h
	$(CC) $(CFLAGS) -c sma.cpp -lcurl  

clean:
	rm -f $(TARGET) $(OBJ)
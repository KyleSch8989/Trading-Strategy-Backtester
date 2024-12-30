CC = g++
CFLAGS = -Wall -std=c++11 -pthread -I/usr/include -I/usr/include/grpc -I/usr/include/protobuf
LDFLAGS = -L/usr/local/lib -lgrpc++ -lprotobuf -lcurl

TARGET = tradingSim
OBJ = main.o sma.o trading.pb.o trading.grpc.pb.o

# Python code generation
PYTHON_OUT_DIR = python_generated
PROTOC_GEN_PYTHON = python -m grpc_tools.protoc

all: $(TARGET) python-gen

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lcurl -I$(shell brew --prefix nlohmann-json)/include $(LDFLAGS)

main.o: main.cpp strategies.h signal.h
	$(CC) $(CFLAGS) -c main.cpp  -lcurl 

sma.o: sma.cpp strategies.h signal.h
	$(CC) $(CFLAGS) -c sma.cpp -lcurl  

# Rules for Protobuf files for C++
trading.pb.cc trading.pb.h: trading.proto
	protoc --cpp_out=. trading.proto

trading.grpc.pb.cc trading.grpc.pb.h: trading.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` trading.proto

trading.pb.o: trading.pb.cc trading.pb.h
	$(CC) $(CFLAGS) -c -o $@ trading.pb.cc

trading.grpc.pb.o: trading.grpc.pb.cc trading.grpc.pb.h
	$(CC) $(CFLAGS) -c -o $@ trading.grpc.pb.cc

# Rule for generating Python code from Protobuf files
python-gen: trading.proto
	$(PROTOC_GEN_PYTHON) -I. --python_out=$(PYTHON_OUT_DIR) --grpc_python_out=$(PYTHON_OUT_DIR) trading.proto

# Clean rule to remove generated and object files
clean:
	rm -f $(TARGET) $(OBJ) *.pb.cc *.pb.h *.grpc.pb.cc *.grpc.pb.h
	rm -rf $(PYTHON_OUT_DIR)
cmake_minimum_required(VERSION 3.20)
project(backtester)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

add_library(protolib protos/service.proto)
target_link_libraries(protolib gRPC::grpc++)
target_include_directories(protolib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET protolib LANGUAGE cpp)
protobuf_generate(TARGET protolib LANGUAGE grpc
GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")


add_executable(backtester_server src/main.cpp src/signal.h src/strategies.h src/sma.cpp)
add_executable(sma src/sma.cpp src/signal.h src/strategies.h src/main.cpp)

target_link_libraries(backtester_server 
    PUBLIC
        protolib
        CURL::libcurl
    PRIVATE
        nlohmann_json::nlohmann_json
)

target_link_libraries(sma
    PUBLIC
        CURL::libcurl
        protolib
    PRIVATE
        nlohmann_json::nlohmann_json
)

set(PROTO_DIR ${CMAKE_SOURCE_DIR}/protos)
set(GENERATED_PYTHON_DIR ${CMAKE_CURRENT_BINARY_DIR}/protos)

add_custom_command(
    OUTPUT ${GENERATED_PYTHON_DIR}/service_pb2.py ${GENERATED_PYTHON_DIR}/service_pb2_grpc.py
    COMMAND python3 -m grpc_tools.protoc -I${PROTO_DIR} --grpc_python_out=${GENERATED_PYTHON_DIR} --python_out=${GENERATED_PYTHON_DIR} ${PROTO_DIR}/service.proto
)

add_custom_target(
    proto_py ALL
    DEPENDS ${GENERATED_PYTHON_DIR}/service_pb2.py ${GENERATED_PYTHON_DIR}/service_pb2_grpc.py
)





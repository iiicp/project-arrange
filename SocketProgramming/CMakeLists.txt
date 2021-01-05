cmake_minimum_required(VERSION 3.17)
project(SocketProgram)

set(CMAKE_CXX_STANDARD 14)

add_executable(SocketServer main_server.cpp)
add_executable(SocketClient main_client.cpp)
cmake_minimum_required(VERSION 3.14)
project(Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(Client main.cpp)

target_link_libraries(Client sfml-graphics sfml-system sfml-window)

if(WIN32)
    target_link_libraries(Client wsock32 ws2_32)
endif()
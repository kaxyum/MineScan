cmake_minimum_required(VERSION 3.26)
project(Checking)

set(CMAKE_CXX_STANDARD 17)

add_executable(Checking main.cpp
        Stage/Stage1.cpp
        Stage/Stage1.h
        Utils/Utils.cpp
        Utils/Utils.h)

target_link_libraries(Checking)

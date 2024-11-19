cmake_minimum_required(VERSION 3.29)
project(Quine_McCluskey_algorithm)

set(CMAKE_C_STANDARD 99)

add_executable(Quine_McCluskey_algorithm
    quine.c
    quinecplusplus.cpp)

#add_definitions(-DUSE_C_MAIN)

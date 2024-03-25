cmake_minimum_required(VERSION 3.25)
project(Laba_BD)

set(CMAKE_CXX_STANDARD 14)

include_directories(Lab1DB)

add_executable(Laba_BD
        Lab1DB/functions.cpp
        Lab1DB/functions.h
        Lab1DB/main.cpp)

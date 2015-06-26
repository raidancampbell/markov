cmake_minimum_required(VERSION 3.2)
project(markov)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(markov ${SOURCE_FILES}  Markov_Link.h)
cmake_minimum_required(VERSION 3.8)
project(testCpp)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES test.cpp ConcreteStrategy.cpp Context.cpp Strategy.hpp)
add_executable(testCpp ${SOURCE_FILES})

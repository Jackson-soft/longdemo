cmake_minimum_required(VERSION 3.8)
project(Strategy)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES Main.cpp ConcreteStrategy.hpp Context.hpp Strategy.hpp)
add_executable(Strategy ${SOURCE_FILES})

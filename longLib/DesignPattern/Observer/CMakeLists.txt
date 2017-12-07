cmake_minimum_required(VERSION 3.8)
project(Observer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES Main.cpp ConcreteObserver.hpp ConcreteSubject.hpp Observer.hpp Subject.hpp)
add_executable(Observer ${SOURCE_FILES})

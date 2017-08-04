cmake_minimum_required(VERSION 3.8)

project(asioDemo)

set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(Boost 1.63.0 COMPONENTS system REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_definitions(-DBOOST_ALL_NO_LIB)
add_definitions(-DBOOST_ALL_DYN_LINK)

add_executable(${PROJECT_NAME} "main.cpp")

TARGET_LINK_LIBRARIES(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})

cmake_minimum_required(VERSION 3.25)

project(test
    LANGUAGES CXX
)

Include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.3.2 # or a later release
    )

FetchContent_MakeAvailable(Catch2)

# find_package(Catch2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

cmake_minimum_required(VERSION 3.14)

# 项目信息
project(longServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(Boost REQUIRED)
find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(MySQL REQUIRED)
find_package(libpqxx REQUIRED)

# 查找当前目录下所有的文件并保存到DIR_SRCS变量
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src DIR_SRCS)
add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDES} ${fmt_INCLUDES}
            ${protobuf_INCLUDES} ${libpqxx_INCLUDES} ${MySQL_INCLUDES})
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${Boost_LIBRARIES} ${fmt_LIBRARIES} ${protobuf_LIBRARIES}
            ${libpqxx_LIBRARIES} ${MySQL_LIBRARIES})

set(unit_test my_test)
add_executable(${unit_test} test/main.cpp)
target_include_directories(
    ${unit_test}
    PRIVATE ${PROJECT_SOURCE_DIR}/include ${Boost_INCLUDES} ${fmt_INCLUDES}
            ${Catch2_INCLUDES} ${libpqxx_INCLUDES} ${MySQL_INCLUDES})
target_link_libraries(
    ${unit_test}
    PRIVATE ${Boost_LIBRARIES} ${fmt_LIBRARIES} ${Catch2_LIBRARIES}
            ${libpqxx_LIBRARIES} ${MySQL_LIBRARIES})

enable_testing()

add_test(NAME doctest COMMAND $<TARGET_FILE:${unit_test}> --success)

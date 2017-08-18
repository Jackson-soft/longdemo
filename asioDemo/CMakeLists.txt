cmake_minimum_required(VERSION 3.8)

project(asioDemo)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)

find_package(Boost COMPONENTS system REQUIRED)

aux_source_directory(. DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${PROTOBUF_LIBRARY})

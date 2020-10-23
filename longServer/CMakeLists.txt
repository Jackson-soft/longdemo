cmake_minimum_required(VERSION 3.14)

# 项目信息
project(longServer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
  Boost
  COMPONENTS
  REQUIRED)
find_package(fmt REQUIRED)

# 查找当前目录下所有的文件并保存到DIR_SRCS变量
# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src DIR_SRCS)

set(Example server)
add_executable(${Example} src/main.cpp)
# 添加头文件目录
target_include_directories(${Example}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${Example} PRIVATE ${Boost_LIBRARIES} fmt::fmt)

set(unit_test my_test)
add_executable(${unit_test} test/main.cpp)
target_link_libraries(${unit_test} PRIVATE ${Boost_LIBRARIES} fmt::fmt)

enable_testing()

add_test(NAME doctest COMMAND $<TARGET_FILE:${unit_test}> --success)

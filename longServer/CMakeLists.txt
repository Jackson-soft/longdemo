cmake_minimum_required(VERSION 3.14)

# 项目信息
project(longServer CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-g -Wall -o2") # 开启所有警告
set(CMAKE_CXX_FLAGS_DEBUG "-o0") # 调试不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

find_package(Boost COMPONENTS REQUIRED)
find_package(fmt REQUIRED)

# 查找当前目录下所有的文件并保存到DIR_SRCS变量
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src DIR_SRCS)

# 添加头文件目录
include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${Boost_INCLUDE_DIRS})

set(Test_Case testMain)
add_executable(${Test_Case} test/main_test.cpp)
target_link_libraries(${Test_Case} pthread ${Boost_LIBRARIES} fmt::fmt)

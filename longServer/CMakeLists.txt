cmake_minimum_required(VERSION 3.10)

# 项目信息
project(longServer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-g -Wall") # 开启所有警告
set(CMAKE_CXX_FLAGS_DEBUG "-o0") # 调试不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

find_package(Boost COMPONENTS REQUIRED)

# 查找当前目录下所有的文件并保存到DIR_SRCS变量
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src DIR_SRCS)

# 添加头文件目录
include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${Boost_INCLUDE_DIRS})

set(Test_Case TestMain)
add_executable(${Test_Case} test/TestMain.cpp)
target_link_libraries(${Test_Case} pthread ${Boost_LIBRARIES})

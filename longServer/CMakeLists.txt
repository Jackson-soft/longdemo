cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

#项目信息
project(longServer)

#查找当前目录下所有的文件并保存到DIR_SRCS变量
aux_source_directory(. DIR_SRCS)

#添加子目录
add_subdirectory(src)

#指定生成目标
add_executable(${PROJECT_NAME} ${DIR_SRCS})

#添加链接库
target_link_libraries(${PROJECT_NAME} pthread longlib)

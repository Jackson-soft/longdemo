cmake_minimum_required(VERSION 3.8)

# 项目信息
project(longServer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-g -Wall") # 开启所有警告
set(CMAKE_CXX_FLAGS_DEBUG "-o0") # 调试不优化
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# 设置boost使用静态链接
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS REQUIRED)

# 查找当前目录下所有的文件并保存到DIR_SRCS变量
aux_source_directory(${CMAKE_CURRENT_LIST_DIR}/src DIR_SRCS)

# 添加头文件目录
include_directories(${CMAKE_CURRENT_LIST_DIR}/include ${Boost_INCLUDE_DIRS})

# 指定生成目标
add_executable(${PROJECT_NAME} ${DIR_SRCS})

# 添加链接库
target_link_libraries(${PROJECT_NAME}
                      ${Boost_LIBRARIES}
                      pthread
                      stdc++fs)

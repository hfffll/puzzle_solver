cmake_minimum_required(VERSION 3.10)
project(PuzzleSolver)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含头文件目录
include_directories(include)

# 添加可执行文件
add_executable(puzzle_solver 
    src/main.cpp
    src/puzzle_solver.cpp
    src/search_algorithms.cpp
)

# 设置编译优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(puzzle_solver PRIVATE -O3)
else()
    target_compile_options(puzzle_solver PRIVATE -g -O0)
endif()
cmake_minimum_required(VERSION 4.3)
project(SegTree)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(SegTree main.cpp)

target_sources(SegTree
        PRIVATE
            FILE_SET CXX_MODULES FILES
                my_segment_tree.cppm
)